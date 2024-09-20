MySQL
<a name="nVj7t"></a>
## 分布式锁的功能

1. 分布式锁使用者位于不同的机器中，锁获取成功之后，才可以对共享资源进行操作
2. 锁具有重入的功能：即一个使用者可以多次获取某个锁
3. 获取锁有超时的功能：即在指定的时间内去尝试获取锁，超过了超时时间，如果还未获取成功，则返回获取失败
4. 能够自动容错，比如：A机器获取锁lock1之后，在释放锁lock1之前，A机器挂了，导致锁lock1未释放，结果会lock1一直被A机器占有着，遇到这种情况时，分布式锁要能够自动解决，可以这么做：持有锁的时候可以加个持有超时时间，超过了这个时间还未释放的，其他机器将有机会获取锁
<a name="nIfSr"></a>
## 预备技能：乐观锁
通常修改表中一条数据过程如下：
```
t1：select获取记录R1
t2：对R1进行编辑
t3：update R1
```
来看一下上面的过程存在的问题：<br />如果A、B两个线程同时执行到t1，他们俩看到的R1的数据一样，然后都对R1进行编辑，然后去执行t3，最终2个线程都会更新成功，后面一个线程会把前面一个线程update的结果给覆盖掉，这就是并发修改数据存在的问题。<br />可以在表中新增一个版本号，每次更新数据时候将版本号作为条件，并且每次更新时候版本号+1，过程优化一下，如下：
```
t1：打开事务start transaction
t2：select获取记录R1,声明变量v=R1.version
t3：对R1进行编辑
t4：执行更新操作
    update R1 set version = version + 1 where user_id=#user_id# and version = #v#;
t5：t4中的update会返回影响的行数，将其记录在count中，然后根据count来判断提交还是回滚
    if(count==1){
        //提交事务
        commit;
    }else{
        //回滚事务
        rollback;
    }
```
上面重点在于步骤t4，当多个线程同时执行到t1，他们看到的R1是一样的，但是当他们执行到t4的时候，数据库会对update的这行记录加锁，确保并发情况下排队执行，所以只有第一个的update会返回1，其他的update结果会返回0，然后后面会判断count是否为1，进而对事务进行提交或者回滚。可以通过count的值知道修改数据是否成功了。<br />上面这种方式就乐观锁。**可以通过乐观锁的方式确保数据并发修改过程中的正确性。**
<a name="CFbZa"></a>
## 使用MySQL实现分布式锁
<a name="FULOX"></a>
### 建表
创建一个分布式锁表，如下
```sql
DROP TABLE IF EXISTS t_lock;
create table t_lock(
  lock_key varchar(32) PRIMARY KEY NOT NULL COMMENT '锁唯一标志',
  request_id varchar(64) NOT NULL DEFAULT '' COMMENT '用来标识请求对象的',
  lock_count INT NOT NULL DEFAULT 0 COMMENT '当前上锁次数',
  timeout BIGINT NOT NULL DEFAULT 0 COMMENT '锁超时时间',
  version INT NOT NULL DEFAULT 0 COMMENT '版本号，每次更新+1'
)COMMENT '锁信息表';
```
<a name="ZJHEB"></a>
### 分布式锁工具类：
```java
package com.example.sql;

import lombok.Builder;
import lombok.Getter;
import lombok.Setter;
import lombok.extern.slf4j.Slf4j;
import org.junit.Test;

import java.sql.*;
import java.util.Objects;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

@Slf4j
public class LockUtils {

    //将requestid保存在该变量中
    static ThreadLocal<String> requestIdTL = new ThreadLocal<>();

    /**
     * 获取当前线程requestid
     *
     * @return
     */
    public static String getRequestId() {
        String requestId = requestIdTL.get();
        if (requestId == null || "".equals(requestId)) {
            requestId = UUID.randomUUID().toString();
            requestIdTL.set(requestId);
        }
        log.info("requestId:{}", requestId);
        return requestId;
    }

    /**
     * 获取锁
     *
     * @param lock_key        锁key
     * @param locktimeout(毫秒) 持有锁的有效时间，防止死锁
     * @param gettimeout(毫秒)  获取锁的超时时间，这个时间内获取不到将重试
     * @return
     */
    public static boolean lock(String lock_key, long locktimeout, int gettimeout) throws Exception {
        log.info("start");
        boolean lockResult = false;
        String request_id = getRequestId();
        long starttime = System.currentTimeMillis();
        while (true) {
            LockModel lockModel = LockUtils.get(lock_key);
            if (Objects.isNull(lockModel)) {
                //插入一条记录,重新尝试获取锁
                LockUtils.insert(LockModel.builder().lock_key(lock_key).request_id("").lock_count(0).timeout(0L).version(0).build());
            } else {
                String reqid = lockModel.getRequest_id();
                //如果reqid为空字符，表示锁未被占用
                if ("".equals(reqid)) {
                    lockModel.setRequest_id(request_id);
                    lockModel.setLock_count(1);
                    lockModel.setTimeout(System.currentTimeMillis() + locktimeout);
                    if (LockUtils.update(lockModel) == 1) {
                        lockResult = true;
                        break;
                    }
                } else if (request_id.equals(reqid)) {
                    //如果request_id和表中request_id一样表示锁被当前线程持有者，此时需要加重入锁
                    lockModel.setTimeout(System.currentTimeMillis() + locktimeout);
                    lockModel.setLock_count(lockModel.getLock_count() + 1);
                    if (LockUtils.update(lockModel) == 1) {
                        lockResult = true;
                        break;
                    }
                } else {
                    //锁不是自己的，并且已经超时了，则重置锁，继续重试
                    if (lockModel.getTimeout() < System.currentTimeMillis()) {
                        LockUtils.resetLock(lockModel);
                    } else {
                        //如果未超时，休眠100毫秒，继续重试
                        if (starttime + gettimeout > System.currentTimeMillis()) {
                            TimeUnit.MILLISECONDS.sleep(100);
                        } else {
                            break;
                        }
                    }
                }
            }
        }
        log.info("end");
        return lockResult;
    }

    /**
     * 释放锁
     *
     * @param lock_key
     * @throws Exception
     */
    public static void unlock(String lock_key) throws Exception {
        //获取当前线程requestId
        String requestId = getRequestId();
        LockModel lockModel = LockUtils.get(lock_key);
        //当前线程requestId和库中request_id一致 && lock_count>0，表示可以释放锁
        if (Objects.nonNull(lockModel) && requestId.equals(lockModel.getRequest_id()) && lockModel.getLock_count() > 0) {
            if (lockModel.getLock_count() == 1) {
                //重置锁
                resetLock(lockModel);
            } else {
                lockModel.setLock_count(lockModel.getLock_count() - 1);
                LockUtils.update(lockModel);
            }
        }
    }

    /**
     * 重置锁
     *
     * @param lockModel
     * @return
     * @throws Exception
     */
    public static int resetLock(LockModel lockModel) throws Exception {
        lockModel.setRequest_id("");
        lockModel.setLock_count(0);
        lockModel.setTimeout(0L);
        return LockUtils.update(lockModel);
    }

    /**
     * 更新lockModel信息，内部采用乐观锁来更新
     *
     * @param lockModel
     * @return
     * @throws Exception
     */
    public static int update(LockModel lockModel) throws Exception {
        return exec(conn -> {
            String sql = "UPDATE t_lock SET request_id = ?,lock_count = ?,timeout = ?,version = version + 1 WHERE lock_key = ? AND  version = ?";
            PreparedStatement ps = conn.prepareStatement(sql);
            int colIndex = 1;
            ps.setString(colIndex++, lockModel.getRequest_id());
            ps.setInt(colIndex++, lockModel.getLock_count());
            ps.setLong(colIndex++, lockModel.getTimeout());
            ps.setString(colIndex++, lockModel.getLock_key());
            ps.setInt(colIndex++, lockModel.getVersion());
            return ps.executeUpdate();
        });
    }

    public static LockModel get(String lock_key) throws Exception {
        return exec(conn -> {
            String sql = "select * from t_lock t WHERE t.lock_key=?";
            PreparedStatement ps = conn.prepareStatement(sql);
            int colIndex = 1;
            ps.setString(colIndex++, lock_key);
            ResultSet rs = ps.executeQuery();
            if (rs.next()) {
                return LockModel.builder().
                        lock_key(lock_key).
                        request_id(rs.getString("request_id")).
                        lock_count(rs.getInt("lock_count")).
                        timeout(rs.getLong("timeout")).
                        version(rs.getInt("version")).build();
            }
            return null;
        });
    }

    public static int insert(LockModel lockModel) throws Exception {
        return exec(conn -> {
            String sql = "insert into t_lock (lock_key, request_id, lock_count, timeout, version) VALUES (?,?,?,?,?)";
            PreparedStatement ps = conn.prepareStatement(sql);
            int colIndex = 1;
            ps.setString(colIndex++, lockModel.getLock_key());
            ps.setString(colIndex++, lockModel.getRequest_id());
            ps.setInt(colIndex++, lockModel.getLock_count());
            ps.setLong(colIndex++, lockModel.getTimeout());
            ps.setInt(colIndex++, lockModel.getVersion());
            return ps.executeUpdate();
        });
    }

    public static <T> T exec(SqlExec<T> sqlExec) throws Exception {
        Connection conn = getConn();
        try {
            return sqlExec.exec(conn);
        } finally {
            closeConn(conn);
        }
    }

    @FunctionalInterface
    public interface SqlExec<T> {
        T exec(Connection conn) throws Exception;
    }

    @Getter
    @Setter
    @Builder
    public static class LockModel {
        private String lock_key;
        private String request_id;
        private Integer lock_count;
        private Long timeout;
        private Integer version;
    }

    private static final String url = "jdbc:mysql://localhost:3306/javacode2018?useSSL=false";        //数据库地址
    private static final String username = "root";        //数据库用户名
    private static final String password = "root123";        //数据库密码
    private static final String driver = "com.mysql.jdbc.Driver";        //mysql驱动

    /**
     * 连接数据库
     *
     * @return
     */
    public static Connection getConn() {
        Connection conn = null;
        try {
            Class.forName(driver);  //加载数据库驱动
            try {
                conn = DriverManager.getConnection(url, username, password);  //连接数据库
            } catch (SQLException e) {
                e.printStackTrace();
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return conn;
    }

    /**
     * 关闭数据库链接
     *
     * @return
     */
    public static void closeConn(Connection conn) {
        if (conn != null) {
            try {
                conn.close();  //关闭数据库链接
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }
}
```
上面代码中实现了开头列的分布式锁的所有功能，可以研究下获取锁的方法：lock，释放锁的方法：unlock。
<a name="LO4T7"></a>
### 测试用例
```java
package com.example.sql;

import lombok.extern.slf4j.Slf4j;
import org.junit.Test;

import static com.example.sql.LockUtils.lock;
import static com.example.sql.LockUtils.unlock;

@Slf4j
public class LockUtilsTest {

    //测试重复获取和重复释放
    @Test
    public void test1() throws Exception {
        String lock_key = "key1";
        for (int i = 0; i < 10; i++) {
            lock(lock_key, 10000L, 1000);
        }
        for (int i = 0; i < 9; i++) {
            unlock(lock_key);
        }
    }

    //获取之后不释放，超时之后被thread1获取
    @Test
    public void test2() throws Exception {
        String lock_key = "key2";
        lock(lock_key, 5000L, 1000);
        Thread thread1 = new Thread(() -> {
            try {
                try {
                    lock(lock_key, 5000L, 7000);
                } finally {
                    unlock(lock_key);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
        thread1.setName("thread1");
        thread1.start();
        thread1.join();
    }
}
```
test1方法测试了重入锁的效果。<br />test2测试了主线程获取锁之后一直未释放，持有锁超时之后被thread1获取到了。
<a name="wNA9w"></a>
## 遗留问题
上面分布式锁还需要考虑一个问题：比如A机会获取了key1的锁，并设置持有锁的超时时间为10秒，但是获取锁之后，执行了一段业务操作，业务操作耗时超过10秒了，此时机器B去获取锁时可以获取成功的，此时会导致A、B两个机器都获取锁成功了，都在执行业务操作，这种情况应该怎么处理？
