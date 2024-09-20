Java Redis<br />在项目中，Redis 不应该被当作传统数据库来使用；储存大量没有过期时间的数据。如果储存大量无过期时间，而且无效的key的话；再加上 Redis 本身的过期策略没有被正确设置，就会大量占用内存。这样就会导致再多的内存资源也不够用。最近在项目中，就遇到这样的情况。<br />情况大致是这样，项目中采用 Redis 二级存储（自己造的一个概念）。大概的意思就是先使用 UUID 生成一个 uuid 作为 value；这个 uuid 作为一个版本 id；存到缓存 key 为 ACL_CACHE_VERSION_KEY 中，即
```bash
SET ACL_CACHE_VERSION_KEY uuid
```
然后在不 uuid 作为另外一个key的一部分。即 ACL_USER_ + uuid，比如 uuid 的值为 26a26b84-578d-40bf-ab15-aeb188a56393，则key 为ACL_USER_26a26b84-578d-40bf-ab15-aeb188a56393，缓存到 key 的数据为
```bash
HMSET ACL_USER_26a26b84-578d-40bf-ab15-aeb1 id1 12345 id2 45678
```
因为 ACL_USER_26a26b84-578d-40bf-ab15-aeb188a56393 的是否过期是通过程序中生成新版本的 uuid，存储新的用户权限数据后；在程序中删除。因为程序的不健壮，导致有大量过期的版本没有及时删除。经过长年的积累导致 Redis 存在大量这些无效版本的 key。<br />在测试环境中，可以使用 keys 命令，模糊查询到需要的 key，但这个操作只适合在测试环境中使用，不适合在生产环境中使用，原因是 Redis 是单线程运行的，当 Redis 中的数据量很大时，由于此操作会遍历所有数据，并将结果一次性全部返回，执行时间会比较长，从而导致后续操作等待，直接影响系统的正常运行。解决的办法是使用scan命令：
```bash
scan cursor [MATCH pattern] [COUNT count]
```

- `cursor`：表示游标，从 0 开始，此命令执行完后会返回一个新的 cursor值。如果cursor!="0"，则表示还有 key 未返回，需要再调用 `scan`，并使用此新的 cursor 值，来获取下一批 key；如果`cursor=="0"`，则表示遍历结束。
- `pattern`：表示模糊匹配的样式
- `count`：表示一批最多返回多少条记录，默认为10

集群环境下使用 jedis 的代码实现
```java
import java.util.HashSet;
import java.util.Set;

import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.JedisCluster;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.ScanParams;
import redis.clients.jedis.ScanResult;

public class RedisTester {

    public static JedisCluster getJedisCluster() {
        JedisPoolConfig config = new JedisPoolConfig();
        config = new JedisPoolConfig();
        config.setMaxTotal(60000);// 设置最大连接数
        config.setMaxIdle(1000); // 设置最大空闲数
        config.setMaxWaitMillis(3000);// 设置超时时间
        config.setTestOnBorrow(true);

        // 集群结点
        Set<HostAndPort> jedisClusterNode = new HashSet<HostAndPort>();
        jedisClusterNode.add(new HostAndPort("yun-1.cacheyum.com", 6379));
        jedisClusterNode.add(new HostAndPort("yun-2.cacheyum.com", 6379));
        jedisClusterNode.add(new HostAndPort("yun-3.cacheyum.com", 6379));
        jedisClusterNode.add(new HostAndPort("yun-4.cacheyum.com", 6379));
        jedisClusterNode.add(new HostAndPort("yun-5.cacheyum.com", 6379));
        jedisClusterNode.add(new HostAndPort("yun-6.cacheyum.com", 6379));
        JedisCluster jc = new JedisCluster(jedisClusterNode, 1000, 1000, 1000, "admin321", config);
        return jc;
    }

    public static void scanCluster() {
        JedisCluster redis = getJedisCluster();
        redis.getClusterNodes().values().stream().forEach(pool -> {
            boolean done = false;
            String cur = "0";
            try (Jedis jedisNode = pool.getResource()) {
                while (!done) {
                    ScanParams scanParams = new ScanParams();
                    scanParams.match(" ACL_USER_*");
                    scanParams.count(10);
                    ScanResult<String> resp = jedisNode.scan(cur, scanParams);
                    for (String result : resp.getResult()) {
                        System.out.println("key: " + result);
                    }
                    cur = resp.getStringCursor();
                    System.out.println("cursor: " + cur);
                    if (cur.equals("0")) {
                        done = true;
                    }
                }
            }
        });
    }

    public static void main(String[] args) {
        scanCluster();
    }

}
```
单机情况下 jedis 代码实现
```java
import java.util.HashSet;
import java.util.Set;

import redis.clients.jedis.HostAndPort;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.ScanParams;
import redis.clients.jedis.ScanResult;

public class RedisTester {


    public static Jedis getAloneRedis() {
        Jedis jedis = new Jedis("127.0.0.1", 6379);
        System.out.println("connect successfully");
        jedis.auth("admin321");
        return jedis;
    }

    public void scanAlone() {
        Jedis jedis = getAloneRedis();
        String cursor = "0";
        do {
            ScanParams scanParams = new ScanParams();
            scanParams.match("ACL_USER_*");
            scanParams.count(10);
            ScanResult<String> sr = jedis.scan(cursor, scanParams);
            List<String> resultList = sr.getResult();
            for (String result : resultList) {
                System.out.println("key: " + result);
            }
            cursor = sr.getStringCursor();
            System.out.println("cursor: " + cursor);
        } while (!cursor.equals("0"));
    }

    public static void main(String[] args) {
        getAloneRedis();
    }

}
```
使用以下代码就可以找到那些版本的 uuid 是无效的。找到后，再调用 del 指令删除；或者为了更加保险，调用 `expire` 加个过期时间。让 key 在某个时间内失效也可以。
