MySQL Druid<br />一些老系统 MySQL 驱动版本较低，存在一些危险漏洞，于是把一些老应用的 MySQL 驱动程序升级到了 8.0。然而升级后，在并发量较高时，查看监控打点，Druid 连接池拿到连接并执行 SQL 的时间大部分都超过 200ms。<br />本文详细的分析了这次“破案”的全过程。<br />对系统进行压测，发现出现大量线程阻塞的情况，线程 dump 信息如下：
```java
"http-nio-5366-exec-48" #210 daemon prio=5 os_prio=0 tid=0x00000000023d0800 nid=0x3be9 waiting for monitor entry [0x00007fa4c1400000]
   java.lang.Thread.State: BLOCKED (on object monitor)
        at org.springframework.boot.web.embedded.tomcat.TomcatEmbeddedWebappClassLoader.loadClass(TomcatEmbeddedWebappClassLoader.java:66)
        - waiting to lock <0x0000000775af0960> (a java.lang.Object)
        at org.apache.catalina.loader.WebappClassLoaderBase.loadClass(WebappClassLoaderBase.java:1186)
        at com.alibaba.druid.util.Utils.loadClass(Utils.java:220)
        at com.alibaba.druid.util.MySqlUtils.getLastPacketReceivedTimeMs(MySqlUtils.java:372)
```
通过上面的信息得知，有线程 BLOCKED 了，BLOCKED 的位置在 com.alibaba.druid.util.Utils.loadClass(Utils.java:220)，于是重点的查看这一块的代码，最终发现了问题。

![2021-06-16-09-45-44-158938.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623807980449-d3e9699c-78b6-4b43-bb23-192840d0de1d.png#clientId=u5791f52d-9b9d-4&from=ui&id=u3aa1384c&originHeight=429&originWidth=797&originalType=binary&ratio=3&size=1027907&status=done&style=shadow&taskId=uae7b9706-6f14-491a-b339-f633756e7b2)
<a name="jmBGy"></a>
### <br />根本原因分析
```java
public class MySqlUtils {

    public static long getLastPacketReceivedTimeMs(Connection conn) throws SQLException {
        if (class_connectionImpl == null && !class_connectionImpl_Error) {
            try {
                class_connectionImpl = Utils.loadClass("com.mysql.jdbc.MySQLConnection");
            } catch (Throwable error){
                class_connectionImpl_Error = true;
            }
        }

        if (class_connectionImpl == null) {
            return -1;
        }

        if (method_getIO == null && !method_getIO_error) {
            try {
                method_getIO = class_connectionImpl.getMethod("getIO");
            } catch (Throwable error){
                method_getIO_error = true;
            }
        }

        if (method_getIO == null) {
            return -1;
        }

        if (class_MysqlIO == null && !class_MysqlIO_Error) {
            try {
                class_MysqlIO = Utils.loadClass("com.mysql.jdbc.MysqlIO");
            } catch (Throwable error){
                class_MysqlIO_Error = true;
            }
        }

        if (class_MysqlIO == null) {
            return -1;
        }

        if (method_getLastPacketReceivedTimeMs == null && !method_getLastPacketReceivedTimeMs_error) {
            try {
                Method method = class_MysqlIO.getDeclaredMethod("getLastPacketReceivedTimeMs");
                method.setAccessible(true);
                method_getLastPacketReceivedTimeMs = method;
            } catch (Throwable error){
                method_getLastPacketReceivedTimeMs_error = true;
            }
        }

        if (method_getLastPacketReceivedTimeMs == null) {
            return -1;
        }

        try {
            Object connImpl = conn.unwrap(class_connectionImpl);
            if (connImpl == null) {
                return -1;
            }

            Object mysqlio = method_getIO.invoke(connImpl);
            Long ms = (Long) method_getLastPacketReceivedTimeMs.invoke(mysqlio);
            return ms.longValue();
        } catch (IllegalArgumentException e) {
            throw new SQLException("getLastPacketReceivedTimeMs error", e);
        } catch (IllegalAccessException e) {
            throw new SQLException("getLastPacketReceivedTimeMs error", e);
        } catch (InvocationTargetException e) {
            throw new SQLException("getLastPacketReceivedTimeMs error", e);
        }
    }
}
```
MySqlUtils 中的`getLastPacketReceivedTimeMs()`方法会加载`com.mysql.jdbc.MySQLConnection`这个类，但在 MySQL 驱动 8.0 中类名改为`com.mysql.cj.jdbc.ConnectionImpl`，所以 MySQL 驱动8.0 中加载不到`com.mysql.jdbc.MySQLConnection`<br />`getLastPacketReceivedTimeMs()`方法实现中，如果`Utils.loadClass("com.mysql.jdbc.MySQLConnection")`加载不到类并抛出异常，会修改变量 `class_connectionImpl_Error`，下次调用不会再进行加载
```java
public class Utils {

    public static Class<?> loadClass(String className) {
        Class<?> clazz = null;

        if (className == null) {
            return null;
        }

        try {
            return Class.forName(className);
        } catch (ClassNotFoundException e) {
            // skip
        }

        ClassLoader ctxClassLoader = Thread.currentThread().getContextClassLoader();
        if (ctxClassLoader != null) {
            try {
                clazz = ctxClassLoader.loadClass(className);
            } catch (ClassNotFoundException e) {
                // skip
            }
        }

        return clazz;
    }
}
```
但是，在 Utils 的`loadClass()`方法中同样 catch了`ClassNotFoundException`，这就导致`loadClass()`在加载不到类的时候，并不会抛出异常，从而会导致每调用一次`getLastPacketReceivedTimeMs()`方法，就会加载一次 `MySQLConnection `这个类<br />线程 dump 信息中可以看到是在调用 `TomcatEmbeddedWebappClassLoader` 的`loadClass()`方法时，导致线程阻塞的。
```java
public class TomcatEmbeddedWebappClassLoader extends ParallelWebappClassLoader {

    public Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
        synchronized (JreCompat.isGraalAvailable() ? this : getClassLoadingLock(name)) {
            Class<?> result = findExistingLoadedClass(name);
            result = (result != null) ? result : doLoadClass(name);
            if (result == null) {
                throw new ClassNotFoundException(name);
            }
            return resolveIfNecessary(result, resolve);
        }
    }
}
```
这是因为 `TomcatEmbeddedWebappClassLoader` 在加载类的时候，会加`synchronized`锁，这就导致每调用一次`getLastPacketReceivedTimeMs()`方法，就会加载一次`com.mysql.jdbc.MySQLConnection`，而又始终加载不到，在加载类的时候会加 `synchronized` 锁，所以会出现线程阻塞，性能下降的现象。
<a name="SBKtP"></a>
### getLastPacketReceivedTimeMs()方法调用时机
```java
public abstract class DruidAbstractDataSource extends WrapperAdapter implements DruidAbstractDataSourceMBean, DataSource, DataSourceProxy, Serializable {

    protected boolean testConnectionInternal(DruidConnectionHolder holder, Connection conn) {
        String sqlFile = JdbcSqlStat.getContextSqlFile();
        String sqlName = JdbcSqlStat.getContextSqlName();

        if (sqlFile != null) {
            JdbcSqlStat.setContextSqlFile(null);
        }
        if (sqlName != null) {
            JdbcSqlStat.setContextSqlName(null);
        }
        try {
            if (validConnectionChecker != null) {
                boolean valid = validConnectionChecker.isValidConnection(conn, validationQuery, validationQueryTimeout);
                long currentTimeMillis = System.currentTimeMillis();
                if (holder != null) {
                    holder.lastValidTimeMillis = currentTimeMillis;
                    holder.lastExecTimeMillis = currentTimeMillis;
                }

                if (valid && isMySql) { // unexcepted branch
                    long lastPacketReceivedTimeMs = MySqlUtils.getLastPacketReceivedTimeMs(conn);
                    if (lastPacketReceivedTimeMs > 0) {
                        long mysqlIdleMillis = currentTimeMillis - lastPacketReceivedTimeMs;
                        if (lastPacketReceivedTimeMs > 0 //
                            && mysqlIdleMillis >= timeBetweenEvictionRunsMillis) {
                            discardConnection(holder);
                            String errorMsg = "discard long time none received connection. "
                                + ", jdbcUrl : " + jdbcUrl
                                + ", jdbcUrl : " + jdbcUrl
                                + ", lastPacketReceivedIdleMillis : " + mysqlIdleMillis;
                            LOG.error(errorMsg);
                            return false;
                        }
                    }
                }

                if (valid && onFatalError) {
                    lock.lock();
                    try {
                        if (onFatalError) {
                            onFatalError = false;
                        }
                    } finally {
                        lock.unlock();
                    }
                }

                return valid;
            }

            if (conn.isClosed()) {
                return false;
            }

            if (null == validationQuery) {
                return true;
            }

            Statement stmt = null;
            ResultSet rset = null;
            try {
                stmt = conn.createStatement();
                if (getValidationQueryTimeout() > 0) {
                    stmt.setQueryTimeout(validationQueryTimeout);
                }
                rset = stmt.executeQuery(validationQuery);
                if (!rset.next()) {
                    return false;
                }
            } finally {
                JdbcUtils.close(rset);
                JdbcUtils.close(stmt);
            }

            if (onFatalError) {
                lock.lock();
                try {
                    if (onFatalError) {
                        onFatalError = false;
                    }
                } finally {
                    lock.unlock();
                }
            }

            return true;
        } catch (Throwable ex) {
            // skip
            return false;
        } finally {
            if (sqlFile != null) {
                JdbcSqlStat.setContextSqlFile(sqlFile);
            }
            if (sqlName != null) {
                JdbcSqlStat.setContextSqlName(sqlName);
            }
        }
    }
}
```
只有`DruidAbstractDataSource`的`testConnectionInternal()`方法中会调用`getLastPacketReceivedTimeMs()`方法<br />`testConnectionInternal()`是用来检测连接是否有效的，在获取连接和归还连接时都有可能会调用该方法，这取决于Druid检测连接是否有效的参数<br />「Druid检测连接是否有效的参数」：

- `testOnBorrow`：每次获取连接时执行`validationQuery`检测连接是否有效（会影响性能）
- `testOnReturn`：每次归还连接时执行`validationQuery`检测连接是否有效（会影响性能）
- `testWhileIdle`：申请连接的时候检测，如果空闲时间大于`timeBetweenEvictionRunsMillis`，执行`validationQuery`检测连接是否有效

应用中设置了`testOnBorrow=true`，每次获取连接时，都会去抢占`synchronized`锁，所以性能下降的很明显
<a name="G6Oyy"></a>
### 解决方案
经验证，使用Druid 1.x版本<=1.1.22会出现该bug，解决方案就是升级至Druid 1.x版本>=1.1.23或者Druid 1.2.x版本
