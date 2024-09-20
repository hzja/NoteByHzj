JavaRedis
<a name="fqigo"></a>
## 一、摘要
Lettuce 是 Redis 的一款高级 Java 客户端，与 Jedis 并列成为最热门的客户端之一，目前已成为 SpringBoot 2.0 版本默认的 redis 客户端。<br />相比老牌 Jedis，Lettuce 属于后起之秀，不仅功能丰富，而且提供了很多新的功能特性，比如异步操作、响应式编程等等，同时还解决了 Jedis 中线程不安全的问题。
<a name="Lo57S"></a>
## 二、Lettuce
<a name="EQ7EO"></a>
### 2.1、基本使用
首先，创建一个 maven 项目，引入lettuce-core包，就可以使用了。
```xml
<dependency>
  <groupId>io.lettuce</groupId>
  <artifactId>lettuce-core</artifactId>
  <version>5.3.1.RELEASE</version>
</dependency>
```
使用 lettuce 连接 redis，测试是否能正常联通！
```java
public class LettuceMain {

    public static void main(String[] args) {
        RedisURI redisUri = RedisURI.builder()
                .withHost("127.0.0.1")
                .withPort(6379)
                .withPassword("111111")
                .withTimeout(Duration.of(10, ChronoUnit.SECONDS))
                .build();
        RedisClient redisClient = RedisClient.create(redisUri);
        StatefulRedisConnection<String, String> connection = redisClient.connect();
        RedisCommands<String, String> commands = connection.sync();
        System.out.println(commands.ping());
        connection.close();
        redisClient.shutdown();
    }
}
```
<a name="Z2Xp9"></a>
### 2.2、同步操作
基本上只要是 Jedis 支持的同步命令操作，Lettuce 都支持。<br />下面，以同步操作字符串为例，Lettuce 的 api 操作如下！
```java
public class LettuceSyncMain {

    public static void main(String[] args) {
        RedisURI redisUri = RedisURI.builder()
                .withHost("127.0.0.1").withPort(6379).withPassword("111111")
                .withTimeout(Duration.of(10, ChronoUnit.SECONDS))
                .build();
        RedisClient redisClient = RedisClient.create(redisUri);
        StatefulRedisConnection<String, String> connection = redisClient.connect();
        //获取同步操作命令工具
        RedisCommands<String, String> commands = connection.sync();

        System.out.println("清空数据："+commands.flushdb());
        System.out.println("判断某个键是否存在："+commands.exists("username"));
        System.out.println("新增<'username','xmr'>的键值对："+commands.set("username", "xmr"));
        System.out.println("新增<'password','password'>的键值对："+commands.set("password", "123"));
        System.out.println("获取<'password'>键的值："+commands.get("password"));
        System.out.println("系统中所有的键如下：" + commands.keys("*"));
        System.out.println("删除键password:"+commands.del("password"));
        System.out.println("判断键password是否存在："+commands.exists("password"));
        System.out.println("设置键username的过期时间为5s:"+commands.expire("username", 5L));
        System.out.println("查看键username的剩余生存时间："+commands.ttl("username"));
        System.out.println("移除键username的生存时间："+commands.persist("username"));
        System.out.println("查看键username的剩余生存时间："+commands.ttl("username"));
        System.out.println("查看键username所存储的值的类型："+commands.type("username"));

        connection.close();
        redisClient.shutdown();
    }
}
```
<a name="TnX4O"></a>
### 2.3、异步操作
除此之外，Lettuce 还支持异步操作，将上面的操作改成异步处理，结果如下！
```java
public class LettuceASyncMain {

    public static void main(String[] args) throws Exception {
        RedisURI redisUri = RedisURI.builder()
                .withHost("127.0.0.1").withPort(6379).withPassword("111111")
                .withTimeout(Duration.of(10, ChronoUnit.SECONDS))
                .build();
        RedisClient redisClient = RedisClient.create(redisUri);
        StatefulRedisConnection<String, String> connection = redisClient.connect();
        //获取异步操作命令工具
        RedisAsyncCommands<String, String> commands = connection.async();

        System.out.println("清空数据："+commands.flushdb().get());
        System.out.println("判断某个键是否存在："+commands.exists("username").get());
        System.out.println("新增<'username','xmr'>的键值对："+commands.set("username", "xmr").get());
        System.out.println("新增<'password','password'>的键值对："+commands.set("password", "123").get());
        System.out.println("获取<'password'>键的值："+commands.get("password").get());
        System.out.println("系统中所有的键如下：" + commands.keys("*").get());
        System.out.println("删除键password:"+commands.del("password").get());
        System.out.println("判断键password是否存在："+commands.exists("password").get());
        System.out.println("设置键username的过期时间为5s:"+commands.expire("username", 5L).get());
        System.out.println("查看键username的剩余生存时间："+commands.ttl("username").get());
        System.out.println("移除键username的生存时间："+commands.persist("username").get());
        System.out.println("查看键username的剩余生存时间："+commands.ttl("username").get());
        System.out.println("查看键username所存储的值的类型："+commands.type("username").get());

        connection.close();
        redisClient.shutdown();
    }
}
```
<a name="C1Zok"></a>
### 2.4、响应式编程
Lettuce 除了支持异步编程以外，还支持响应式编程，Lettuce 引入的响应式编程框架是Project Reactor，如果没有响应式编程经验可以先自行了解一下，访问地址[https://projectreactor.io/](https://projectreactor.io/)。<br />响应式编程使用案例如下：
```java
public class LettuceMain {

    public static void main(String[] args) throws Exception {
        RedisURI redisUri = RedisURI.builder()
                .withHost("127.0.0.1").withPort(6379).withPassword("111111")
                .withTimeout(Duration.of(10, ChronoUnit.SECONDS))
                .build();
        RedisClient redisClient = RedisClient.create(redisUri);
        StatefulRedisConnection<String, String> connection = redisClient.connect();
        //获取响应式API操作命令工具
        RedisReactiveCommands<String, String> commands = connection.reactive();

        Mono<String> setc = commands.set("name", "mayun");
        System.out.println(setc.block());
        Mono<String> getc = commands.get("name");
        getc.subscribe(System.out::println);
        Flux<String> keys = commands.keys("*");
        keys.subscribe(System.out::println);

        //开启一个事务，先把count设置为1，再将count自增1
        commands.multi().doOnSuccess(r -> {
            commands.set("count", "1").doOnNext(value -> System.out.println("count1：" +  value)).subscribe();
            commands.incr("count").doOnNext(value -> System.out.println("count2：" +  value)).subscribe();
        }).flatMap(s -> commands.exec())
                .doOnNext(transactionResult -> System.out.println("transactionResult：" + transactionResult.wasDiscarded())).subscribe();

        Thread.sleep(1000 * 5);
        connection.close();
        redisClient.shutdown();
    }
}
```
<a name="bojlw"></a>
### 2.5、发布和订阅
Lettuce 还支持 redis 的消息发布和订阅，具体实现案例如下：
```java
public class LettuceReactiveMain1 {

    public static void main(String[] args) throws Exception {
        RedisURI redisUri = RedisURI.builder()
                .withHost("127.0.0.1").withPort(6379).withPassword("111111")
                .withTimeout(Duration.of(10, ChronoUnit.SECONDS))
                .build();
        RedisClient redisClient = RedisClient.create(redisUri);
        //获取发布订阅操作命令工具
        StatefulRedisPubSubConnection<String, String> pubsubConn = redisClient.connectPubSub();
        pubsubConn.addListener(new RedisPubSubListener<String, String>() {
            @Override
            public void unsubscribed(String channel, long count) {
                System.out.println("[unsubscribed]" + channel);
            }
            @Override
            public void subscribed(String channel, long count) {
                System.out.println("[subscribed]" + channel);
            }
            @Override
            public void punsubscribed(String pattern, long count) {
                System.out.println("[punsubscribed]" + pattern);
            }
            @Override
            public void psubscribed(String pattern, long count) {
                System.out.println("[psubscribed]" + pattern);
            }
            @Override
            public void message(String pattern, String channel, String message) {
                System.out.println("[message]" + pattern + " -> " + channel + " -> " + message);
            }
            @Override
            public void message(String channel, String message) {
                System.out.println("[message]" + channel + " -> " + message);
            }
        });
        RedisPubSubAsyncCommands<String, String> pubsubCmd = pubsubConn.async();
        pubsubCmd.psubscribe("CH");
        pubsubCmd.psubscribe("CH2");
        pubsubCmd.unsubscribe("CH");

        Thread.sleep(100 * 5);
        pubsubConn.close();
        redisClient.shutdown();
    }
}
```
<a name="dEwDp"></a>
### 2.6、客户端资源与参数配置
Lettuce 客户端的通信框架集成了 Netty 的非阻塞 IO 操作，客户端资源的设置与 Lettuce 的性能、并发和事件处理紧密相关，如果不是特别熟悉客户端参数配置，不建议在没有经验的前提下凭直觉修改默认值，保持默认配置就行。<br />非集群环境下，具体的配置案例如下：
```java
public class LettuceMain {

    public static void main(String[] args) throws Exception {
        ClientResources resources = DefaultClientResources.builder()
                .ioThreadPoolSize(4) //I/O线程数
                .computationThreadPoolSize(4) //任务线程数
                .build();
        RedisURI redisUri = RedisURI.builder()
                .withHost("127.0.0.1").withPort(6379).withPassword("111111")
                .withTimeout(Duration.of(10, ChronoUnit.SECONDS))
                .build();
        ClientOptions options = ClientOptions.builder()
                .autoReconnect(true)//是否自动重连
                .pingBeforeActivateConnection(true)//连接激活之前是否执行PING命令
                .build();
        RedisClient client = RedisClient.create(resources, redisUri);
        client.setOptions(options);
        StatefulRedisConnection<String, String> connection = client.connect();
        RedisCommands<String, String> commands = connection.sync();
        commands.set("name", "关羽");
        System.out.println(commands.get("name"));

        connection.close();
        client.shutdown();
        resources.shutdown();
    }
}
```
集群环境下，具体的配置案例如下：
```java
public class LettuceMain {

    public static void main(String[] args) throws Exception {
        ClientResources resources = DefaultClientResources.builder()
                .ioThreadPoolSize(4) //I/O线程数
                .computationThreadPoolSize(4) //任务线程数
                .build();
        RedisURI redisUri = RedisURI.builder()
                .withHost("127.0.0.1").withPort(6379).withPassword("111111")
                .withTimeout(Duration.of(10, ChronoUnit.SECONDS))
                .build();
        ClusterClientOptions options = ClusterClientOptions.builder()
                .autoReconnect(true)//是否自动重连
                .pingBeforeActivateConnection(true)//连接激活之前是否执行PING命令
                .validateClusterNodeMembership(true)//是否校验集群节点的成员关系
                .build();
        RedisClusterClient client = RedisClusterClient.create(resources, redisUri);
        client.setOptions(options);
        StatefulRedisClusterConnection<String, String> connection = client.connect();
        RedisAdvancedClusterCommands<String, String> commands = connection.sync();
        commands.set("name", "张飞");
        System.out.println(commands.get("name"));

        connection.close();
        client.shutdown();
        resources.shutdown();
    }
}
```
<a name="biXTj"></a>
### 2.7、线程池配置
Lettuce 连接设计的时候，就是线程安全的，所以一个连接可以被多个线程共享，同时 lettuce 连接默认是自动重连的，使用单连接基本可以满足业务需求，大多数情况下不需要配置连接池，多连接并不会给操作带来性能上的提升。<br />但在某些特殊场景下，比如事物操作，使用连接池会是一个比较好的方案，那么如何配置线程池呢？
```java
public class LettuceMain {

    public static void main(String[] args) throws Exception {
        RedisURI redisUri = RedisURI.builder()
                .withHost("127.0.0.1")
                .withPort(6379)
                .withPassword("111111")
                .withTimeout(Duration.of(10, ChronoUnit.SECONDS))
                .build();
        RedisClient client = RedisClient.create(redisUri);
        //连接池配置
        GenericObjectPoolConfig poolConfig = new GenericObjectPoolConfig();
        poolConfig.setMaxIdle(2);

        GenericObjectPool<StatefulRedisConnection<String, String>> pool = ConnectionPoolSupport.createGenericObjectPool(client::connect, poolConfig);
        StatefulRedisConnection<String, String> connection = pool.borrowObject();
        RedisCommands<String, String> commands = connection.sync();
        commands.set("name", "张飞");
        System.out.println(commands.get("name"));

        connection.close();
        pool.close();
        client.shutdown();
    }
}
```
<a name="a6nGM"></a>
### 2.8、主从模式配置
redis 一般采用主从复制模式，搭建高可用的架构，简单的说就一个主节点，多个从节点，自动从主节点同步最新数据。<br />Lettuce 支持自动发现主从模式下的节点信息，然后保存到本地，具体配置如下：
```java
public class LettuceMain {

    public static void main(String[] args) throws Exception {
        //这里只需要配置一个节点的连接信息，不一定需要是主节点的信息，从节点也可以;可以自动发现主从节点
        RedisURI uri = RedisURI.builder().withHost("192.168.31.111").withPort(6379).withPassword("123456").build();
        RedisClient client = RedisClient.create(uri);
        StatefulRedisMasterReplicaConnection<String, String> connection = MasterReplica.connect(client, StringCodec.UTF8, uri);
        //从节点读取数据
        connection.setReadFrom(ReadFrom.REPLICA);

        RedisCommands<String, String> commands = connection.sync();
        commands.set("name", "张飞");
        System.out.println(commands.get("name"));

        connection.close();
        client.shutdown();
    }
}
```
当然也可以手动指定集群节点来加载，具体配置如下：
```java
public class LettuceMain {

    public static void main(String[] args) throws Exception {
        //集群节点
        List<RedisURI> uris = new ArrayList();
        uris.add(RedisURI.builder().withHost("192.168.31.111").withPort(6379).withPassword("111111").build());
        uris.add(RedisURI.builder().withHost("192.168.31.112").withPort(6379).withPassword("111111").build());
        uris.add(RedisURI.builder().withHost("192.168.31.113").withPort(6379).withPassword("111111").build());

        RedisClient client = RedisClient.create();
        StatefulRedisMasterReplicaConnection<String, String> connection = MasterReplica.connect(client, StringCodec.UTF8, uris);
        //从节点读取数据
        connection.setReadFrom(ReadFrom.REPLICA);

        RedisCommands<String, String> commands = connection.sync();
        commands.set("name", "张飞");
        System.out.println(commands.get("name"));

        connection.close();
        client.shutdown();
    }
}
```
<a name="C73pX"></a>
### 2.9、哨兵模式配置
哨兵模式，也是 redis 实现服务高可用的一大亮点，具体配置实现如下：
```java
public class LettuceMain {

    public static void main(String[] args) throws Exception {
        //集群节点
        List<RedisURI> uris = new ArrayList();
        uris.add(RedisURI.builder().withSentinel("192.168.31.111", 26379).withSentinelMasterId("mymaster").withPassword("123456").build());
        uris.add(RedisURI.builder().withSentinel("192.168.31.112", 26379).withSentinelMasterId("mymaster").withPassword("123456").build());
        uris.add(RedisURI.builder().withSentinel("192.168.31.113", 26379).withSentinelMasterId("mymaster").withPassword("123456").build());

        RedisClient client = RedisClient.create();
        StatefulRedisMasterReplicaConnection<String, String> connection = MasterReplica.connect(client, StringCodec.UTF8, uris);
        //从节点读取数据
        connection.setReadFrom(ReadFrom.REPLICA);

        RedisCommands<String, String> commands = connection.sync();
        commands.set("name", "赵云");
        System.out.println(commands.get("name"));

        connection.close();
        client.shutdown();
    }
}
```
<a name="KoWY7"></a>
### 2.10、Cluster 集群模式配置
Cluster 集群模式，是之后推出的一种高可用的架构模型，主要是采用分片方式来存储数据，具体配置如下：
```java
public class LettuceReactiveMain4 {

    public static void main(String[] args) throws Exception {
        Set<RedisURI> uris = new HashSet<>();
        uris.add(RedisURI.builder().withHost("192.168.31.111").withPort(7000).withPassword("123456").build());
        uris.add(RedisURI.builder().withHost("192.168.31.112").withPort(7000).withPassword("123456").build());
        uris.add(RedisURI.builder().withHost("192.168.31.113").withPort(7000).withPassword("123456").build());
        uris.add(RedisURI.builder().withHost("192.168.31.114").withPort(7000).withPassword("123456").build());
        uris.add(RedisURI.builder().withHost("192.168.31.115").withPort(7000).withPassword("123456").build());
        uris.add(RedisURI.builder().withHost("192.168.31.116").withPort(7001).withPassword("123456").build());

        RedisClusterClient client = RedisClusterClient.create(uris);
        StatefulRedisClusterConnection<String, String> connection = client.connect();
        RedisAdvancedClusterCommands<String, String> commands = connection.sync();
        commands.set("name", "关羽");
        System.out.println(commands.get("name"));

        //选择从节点,只读
        NodeSelection<String, String> replicas = commands.replicas();
        NodeSelectionCommands<String, String> nodeSelectionCommands = replicas.commands();
        Executions<List<String>> keys = nodeSelectionCommands.keys("*");
        keys.forEach(key -> System.out.println(key));

        connection.close();
        client.shutdown();
    }
}
```
<a name="uJqCs"></a>
## 三、小结
Lettuce 相比老牌的 Jedis 客户端，功能更加强大，不仅解决了线程安全的问题，还支持异步和响应式编程，支持集群，Sentinel，管道和编码器等等功能。<br />以上介绍的可能只是冰山一角，如果想要了解更多的信息，可以访问它的官网地址：[https://lettuce.io/](https://lettuce.io/)。
