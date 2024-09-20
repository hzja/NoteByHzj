JavaRedisson
<a name="apRKt"></a>
## 一、摘要
**什么是 Redisson**？来自于官网上的描述内容如下！
> Redisson 是一个在 Redis 的基础上实现的 Java 驻内存数据网格客户端（In-Memory Data Grid）。它不仅提供了一系列的 redis 常用数据结构命令服务，还提供了许多分布式服务，例如分布式锁、分布式对象、分布式集合、分布式远程服务、分布式调度任务服务等等。

相比于 Jedis、Lettuce 等基于 redis 命令封装的客户端，Redisson 提供的功能更加高端和抽象！<br />更多功能特性和开发文档说明，可用移步github进行获取，访问地址如下：[https://github.com/redisson/redisson/wiki/](https://github.com/redisson/redisson/wiki/)<br />接下来就一起来聊一下，如何使用 Redisson 操作 Redis 中的字符串、哈希、列表、集合、有序集合，以及布隆过滤器和分布式锁等功能。
<a name="Hfq7K"></a>
## 二、Redisson
<a name="Rq8eG"></a>
### 2.1、基本使用
跟过去一样，首先创建一个 maven 项目，添加Redisson依赖包。
```xml
<dependency>
  <groupId>org.redisson</groupId>
  <artifactId>redisson</artifactId>
  <version>3.13.6</version>
</dependency>
```
单机环境下，简单样例如下！
```java
public class RedissonMain {

    public static void main(String[] args) {
        Config config = new Config();
        config.useSingleServer()
                .setAddress("redis://127.0.0.1:6379")
                .setPassword("123456")
                .setDatabase(0);
        //获取客户端
        RedissonClient redissonClient = Redisson.create(config);
        //获取所有的key
        redissonClient.getKeys().getKeys().forEach(key -> System.out.println(key));
        //关闭客户端
        redissonClient.shutdown();
    }
}
```
ps：创建 RedissonClient 对象实例的方式多钟多样，可以直接通过在代码中设置 Redis 服务的相关参数创建，也可以通过加载 JSON 格式、 YAML 格式或者 Spring XML 配置文件来创建，详细的参数配置可用移步上文提到的 Redisson 开发文档。
<a name="PDjzo"></a>
### 2.2、字符串操作
Redisson 支持通过RBucket对象来操作字符串数据结构，通过RBucket实例可以设置value或设置value和有效期，简单样例如下！
```java
//字符串操作
RBucket<String> rBucket =  redissonClient.getBucket("strKey");
// 设置value和key的有效期
rBucket.set("张三", 30, TimeUnit.SECONDS);
// 通过key获取value
System.out.println(redissonClient.getBucket("strKey").get());
```
<a name="qdTqU"></a>
### 2.3、对象操作
Redisson 支持将对象作为value存入redis，被存储的对象事先必须要实现序列化接口Serializable，否则会报错，简单样例如下！
```java
public class Student implements Serializable {

    private Long id;

    private String name;

    private Integer age;

   //set、get...

    @Override
    public String toString() {
        return "Student{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}
```
```java
//Student对象
Student student = new Student();
student.setId(1L);
student.setName("张三");
student.setAge(18);

//对象操作
RBucket<Student> rBucket =  redissonClient.getBucket("objKey");
// 设置value和key的有效期
rBucket.set(student, 30, TimeUnit.SECONDS);
// 通过key获取value
System.out.println(redissonClient.getBucket("objKey").get());
```
<a name="Isi1S"></a>
### 2.4、哈希操作
Redisson 支持通过RMap对象来操作哈希数据结构，简单样例如下！
```java
//哈希操作
RMap<String, String> rMap = redissonClient.getMap("mapkey");
// 设置map中key-value
rMap.put("id", "123");
rMap.put("name", "赵四");
rMap.put("age", "50");

//设置过期时间
rMap.expire(30, TimeUnit.SECONDS);
// 通过key获取value
System.out.println(redissonClient.getMap("mapkey").get("name"));
```
<a name="oBL2L"></a>
### 2.5、列表操作
Redisson 支持通过RList对象来操作列表数据结构，简单样例如下！
```java
//字符串操作
RList<Student> rList = redissonClient.getList("listkey");

Student student1 = new Student();
student1.setId(1L);
student1.setName("张三");
student1.setAge(18);
rList.add(student1);

Student student2 = new Student();
student2.setId(2L);
student2.setName("李四");
student2.setAge(19);
rList.add(student2);

//设置过期时间
rList.expire(30, TimeUnit.SECONDS);
// 通过key获取value
System.out.println(redissonClient.getList("listkey"));
```
<a name="XECpV"></a>
### 2.6、集合操作
Redisson 支持通过RSet对象来操作集合数据结构，简单样例如下！
```java
//字符串操作
RSet<Student> rSet = redissonClient.getSet("setkey");

Student student1 = new Student();
student1.setId(1L);
student1.setName("张三");
student1.setAge(18);
rSet.add(student1);

Student student2 = new Student();
student2.setId(2L);
student2.setName("李四");
student2.setAge(19);
rSet.add(student2);

//设置过期时间
rSet.expire(30, TimeUnit.SECONDS);
// 通过key获取value
System.out.println(redissonClient.getSet("setkey"));
```
<a name="fN7Ti"></a>
### 2.6、有序集合操作
Redisson 支持通过RSortedSet对象来操作有序集合数据结构，在使用对象来存储之前，实体对象必须先实现Comparable接口，并重写比较逻辑，否则会报错，简单样例如下！
```java
public class Student implements Serializable, Comparable<Student> {

    private Long id;

    private String name;

    private Integer age;

    //get、set.....

    @Override
    public String toString() {
        return "Student{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", age=" + age +
                '}';
    }

    @Override
    public int compareTo(Student obj) {
        return this.getId().compareTo(obj.getId());
    }
}
```
```java
//有序集合操作
RSortedSet<Student> sortSetkey = redissonClient.getSortedSet("sortSetkey");

Student student1 = new Student();
student1.setId(1L);
student1.setName("张三");
student1.setAge(18);
sortSetkey.add(student1);

Student student2 = new Student();
student2.setId(2L);
student2.setName("李四");
student2.setAge(19);
sortSetkey.add(student2);

// 通过key获取value
System.out.println(redissonClient.getSortedSet("sortSetkey"));
```
<a name="Odkgw"></a>
### 2.7、布隆过滤器
布隆过滤器（Bloom Filter）是 1970 年由布隆提出的。它实际上是一个很长的二进制向量和一系列随机映射函数。<br />布隆过滤器可以用于检索一个元素是否在一个集合中。它的优点是空间效率和查询时间都比一般的算法要好的多，缺点是有一定的误识别率和删除困难。<br />Redisson 支持通过`RBloomFilter`对象来操作布隆过滤器，简单样例如下！
```java
RBloomFilter rBloomFilter = redissonClient.getBloomFilter("seqId");
// 初始化预期插入的数据量为10000和期望误差率为0.01
rBloomFilter.tryInit(10000, 0.01);
// 插入部分数据
rBloomFilter.add("100");
rBloomFilter.add("200");
rBloomFilter.add("300");
//设置过期时间
rBloomFilter.expire(30, TimeUnit.SECONDS);
// 判断是否存在
System.out.println(rBloomFilter.contains("300"));
System.out.println(rBloomFilter.contains("200"));
System.out.println(rBloomFilter.contains("999"));
```
<a name="t3aPt"></a>
### 2.8、分布式自增ID
ID 是数据的唯一标识，传统的做法是利用 UUID 和数据库的自增 ID。<br />但由于 UUID 是无序的，不能附带一些其他信息，因此实际作用有限。<br />随着业务的发展，数据量会越来越大，需要对数据进行分表，甚至分库。分表后每个表的数据会按自己的节奏来自增，这样会造成 ID 冲突，因此这时就需要一个单独的机制来负责生成唯一 ID，redis 原生支持生成全局唯一的 ID。<br />简单样例如下！
```java
final String lockKey = "aaaa";
//通过redis的自增获取序号
RAtomicLong atomicLong = redissonClient.getAtomicLong(lockKey);
//设置过期时间
atomicLong.expire(30, TimeUnit.SECONDS);
// 获取值
System.out.println(atomicLong.incrementAndGet());
```
<a name="ROloK"></a>
### 2.9、分布式锁
Redisson 最大的亮点，也是使用最多的功能，就是提供了强大的**分布式锁实现**，特点是：使用简单、安全！<br />简单使用样例如下！
```java
Config config = new Config();
config.useSingleServer()
        .setAddress("redis://127.0.0.1:6379")
        .setPassword("123456")
        .setDatabase(0);
RedissonClient redissonClient = Redisson.create(config);
//获取锁对象实例
final String lockKey = "abc";
RLock rLock = redissonClient.getLock(lockKey);

try {
    //尝试5秒内获取锁，如果获取到了，最长60秒自动释放
    boolean res = rLock.tryLock(5L, 60L, TimeUnit.SECONDS);
    if (res) {
        //成功获得锁，在这里处理业务
        System.out.println("获取锁成功");
    }
} catch (Exception e) {
    System.out.println("获取锁失败，失败原因：" + e.getMessage());
} finally {
    //无论如何, 最后都要解锁
    rLock.unlock();
}

//关闭客户端
redissonClient.shutdown();
```
以上是单机环境下的分布式锁实现逻辑，如果是集群环境下，应该如何处理呢？<br />Redisson 提供`RedissonRedLock`操作类，也被称为**红锁**，实现原理简单的总结有以下几点：

- 1.如果有多个 redis 集群的时候，当且仅当从大多数（N/2+1，比如有3个 redis 节点，那么至少有2个节点）的 Redis 节点都取到锁，并且获取锁使用的总耗时小于锁失效时间时，锁才算获取成功
- 2.如果获取失败，客户端会在所有的 Redis 实例上进行解锁操作
- 3.集群环境下，redis 服务器直接不存在任何复制或者其他隐含的分布式协调机制，否则会存在实效的可能

`RedissonRedLock`简单使用样例如下！
```java
Config config1 = new Config();
config1.useSingleServer().setAddress("redis://192.168.3.111:6379").setPassword("a123456").setDatabase(0);
RedissonClient redissonClient1 = Redisson.create(config1);

Config config2 = new Config();
config2.useSingleServer().setAddress("redis://192.168.3.112:6379").setPassword("a123456").setDatabase(0);
RedissonClient redissonClient2 = Redisson.create(config2);

Config config3 = new Config();
config3.useSingleServer().setAddress("redis://192.168.3.113:6379").setPassword("a123456").setDatabase(0);
RedissonClient redissonClient3 = Redisson.create(config3);

//获取多个 RLock 对象
final String lockKey = "abc";
RLock lock1 = redissonClient1.getLock(lockKey);
RLock lock2 = redissonClient2.getLock(lockKey);
RLock lock3 = redissonClient3.getLock(lockKey);

//根据多个 RLock 对象构建 RedissonRedLock （最核心的差别就在这里）
RedissonRedLock redLock = new RedissonRedLock(lock1, lock2, lock3);

try {
    //尝试5秒内获取锁，如果获取到了，最长60秒自动释放
    boolean res = redLock.tryLock(5L, 60L, TimeUnit.SECONDS);
    if (res) {
        //成功获得锁，在这里处理业务
        System.out.println("获取锁成功");

    }
} catch (Exception e) {
    System.out.println("获取锁失败，失败原因：" + e.getMessage());
} finally {
    //无论如何, 最后都要解锁
    redLock.unlock();
}
```
<a name="MVJY8"></a>
### 2.10、集群模式
以上介绍的都是单机模式，如果是集群环境，可以采用如下方式进行配置：
```java
Config config = new Config();
config.useClusterServers()
    .setScanInterval(2000) // 集群状态扫描间隔时间，单位是毫秒
    //可以用"rediss://"来启用SSL连接
    .addNodeAddress("redis://127.0.0.1:7000", "redis://127.0.0.1:7001")
    .addNodeAddress("redis://127.0.0.1:7002");

RedissonClient redisson = Redisson.create(config);
```
<a name="mVpSp"></a>
### 2.11、哨兵模式
哨兵模式，参数配置方式如下：
```java
Config config = new Config();
config.useSentinelServers()
    .setMasterName("mymaster")
    //可以用"rediss://"来启用SSL连接
    .addSentinelAddress("127.0.0.1:26389", "127.0.0.1:26379")
    .addSentinelAddress("127.0.0.1:26319");

RedissonClient redisson = Redisson.create(config);
```
<a name="yIiUv"></a>
### 2.12、主从模式
主从模式，参数配置方式如下：
```java
Config config = new Config();
config.useMasterSlaveServers()
    //可以用"rediss://"来启用SSL连接
    .setMasterAddress("redis://127.0.0.1:6379")
    .addSlaveAddress("redis://127.0.0.1:6389", "redis://127.0.0.1:6332", "redis://127.0.0.1:6419")
    .addSlaveAddress("redis://127.0.0.1:6399");

RedissonClient redisson = Redisson.create(config);
```
<a name="VwmQa"></a>
## 三、小结
Redisson和Jedis、Lettuce的区别：

- Jedis：Redis 官方推出的用于通过 Java 连接 Redis 客户端的一个工具包，它提供了全面的类似于 Redis 原生命令的支持，是目前使用最广的一款 java 客户端。
- Lettuce：一个可扩展的线程安全的 Redis 客户端，通讯框架基于 Netty 开发，支持高级的 Redis 特性，比如哨兵，集群，管道，自动重新连接等特性。从 Spring Boot 2.x 开始， Lettuce 已取代 Jedis 成为首选 Redis 的客户端。
- Redisson：一款架设在 Redis 基础上，通讯基于 Netty 的综合的、新型的中间件，是企业级开发中使用 Redis 的最佳范本。

总结下来，Jedis 把 Redis 命令封装的非常全面，Lettuce 则进一步丰富了 Api，支持 Redis 各种高级特性。<br />但是两者并没有进一步深化，只给了你操作 Redis 数据库的工具，而 Redisson 则是基于 Redis、Lua 和 Netty 建立起了一套的分布式解决方案，比如分布式锁的实现，分布式对象的操作等等。<br />在实际使用过程中，Lettuce + Redisson组合使用的比较多，两者相铺相成。<br />关于分布式锁实现的应用，**生产环境推荐尽量采用单点环境来实现，基本上解决绝大部分的分布式锁问题**，如果当前服务的环境确实很复杂，可以采用`RedissonRedLock`来实现。
