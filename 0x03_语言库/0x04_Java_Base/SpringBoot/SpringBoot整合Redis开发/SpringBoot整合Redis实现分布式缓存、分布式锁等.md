JavaSpringBootRedis
<a name="RIOAA"></a>
## 一、摘要
在当前流行的微服务以及分布式集群环境下，Redis 的使用场景可以说非常的广泛，能解决集群环境下系统中遇到的不少技术问题，在此列举几个使用 redis 经常用到的功能！

- 分布式缓存：在分布式的集群架构中，将缓存存储在内存中会出现很多的问题，比如用户回话信息，因为这部分信息需要与其他机器共享，此时利用 Redis 可以很好的解决机器之间数据共享的问题，缓存也是 Redis 中使用最多的场景
- 分布式锁：在高并发的情况下，需要一个锁来防止并发带来的脏数据，Java 自带的锁机制显然对进程间的并发并不好使，此时利用 Redis 的单线程特性，实现分布式锁控制
- 接口限流：在集群环境下，可以利用 redis 的分布式自增ID功能，精准的统计每个接口在指定时间内的请求次数，利用这个特性，可以定向限制某个接口恶意频刷

当然 Redis 的使用场景并不仅仅只有这么多，还有很多未列出的场景，如发布/订阅，分布锁集合等。<br />现实中大部分的微服务项目，都是基于 SpringBoot 框架进行快速开发，在  SpringBoot 项目中应该如何使用 Redis 呢？<br />代码实践如下！
<a name="vMIqP"></a>
## 二、代码实践
<a name="SnjnI"></a>
### 2.1、添加 redis 相关依赖包
实际上，在 SpringBoot 项目中，使用redis非常简单，开发者只需要在项目中添加如下的依赖即可！
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```
官方推荐的 java 版本的 redis 客户端，一共有三个，分别是Jedis、Lettuce和Redisson，其中大部分场景下，使用Jedis或者Lettuce就足够了。<br />在 SpringBoot 1.x 版本里面，spring-boot-starter-data-redis默认集成的客户端是Jedis；从 SpringBoot 2.x 开始，spring-boot-starter-data-redis默认集成的客户端是Lettuce。<br />以springBoot-2.1.0版本为例，打开spring-boot-starter-data-redis依赖配置，核心配置如下！
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd" xmlns="http://maven.apache.org/POM/4.0.0"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starters</artifactId>
    <version>2.1.0.RELEASE</version>
  </parent>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
  <version>2.1.0.RELEASE</version>
  <name>Spring Boot Data Redis Starter</name>

  <!--省略其他配置...-->
 
  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter</artifactId>
      <version>2.1.0.RELEASE</version>
      <scope>compile</scope>
    </dependency>
    <dependency>
      <groupId>org.springframework.data</groupId>
      <artifactId>spring-data-redis</artifactId>
      <version>2.1.2.RELEASE</version>
      <scope>compile</scope>
      <exclusions>
        <exclusion>
          <artifactId>jcl-over-slf4j</artifactId>
          <groupId>org.slf4j</groupId>
        </exclusion>
      </exclusions>
    </dependency>
    <dependency>
      <groupId>io.lettuce</groupId>
      <artifactId>lettuce-core</artifactId>
      <version>5.1.2.RELEASE</version>
      <scope>compile</scope>
    </dependency>
  </dependencies>

</project>
```
可以很清晰的看到，spring-boot-starter-data-redis默认集成的客户端是Lettuce。
<a name="hemNH"></a>
### 2.2、配置 redis 相关连接信息
依赖包添加完成之后，还需要在application.properties全局配置文件中，添加相关的 redis 配置信息。
```
# Redis数据库索引（默认为0）
spring.redis.database=0
# Redis服务器地址
spring.redis.host=127.0.0.1
# Redis服务器连接端口
spring.redis.port=6379
# Redis服务器连接密码（默认为空）
spring.redis.password=
# 连接池最大连接数（使用负值表示没有限制） 默认 8
spring.redis.lettuce.pool.max-active=8
# 连接池最大阻塞等待时间（使用负值表示没有限制） 默认 -1
spring.redis.lettuce.pool.max-wait=-1
# 连接池中的最大空闲连接 默认 8
spring.redis.lettuce.pool.max-idle=8
# 连接池中的最小空闲连接 默认 0
spring.redis.lettuce.pool.min-idle=0
```
最后，来跑一个最简单的单元测试，看看是否能联通（**确保 redis 的服务端已经启动**）。
```java
@Autowired
private StringRedisTemplate stringRedisTemplate;

@Test
public void test() throws Exception {
    String uuid = UUID.randomUUID().toString();
    stringRedisTemplate.opsForValue().set(uuid, uuid, 60, TimeUnit.SECONDS);
    System.out.println(stringRedisTemplate.opsForValue().get(uuid));
}
```
如果控制台输出正常，说明基本配置已经完成，如果有错误，看错误信息然后依次排查！
<a name="jAghT"></a>
### 2.3、重新配置 RedisTemplate 的序列化策略
SpringBoot 提供了一个高度封装的RedisTemplate类来操作redis的各个命令，开发者无需关心具体的客户端 api 问题，通过RedisTemplate提供的方法，就可以操作redis，方便开发者可以无成本替换 java 客户端。<br />当存储对象的时候，RedisTemplate默认采用的是 Jdk 提供的序列化工具库，该工具有个要求，缓存的对象必须显式实现序列化接口，才能保存。<br />通常情况下，会自定义RedisTemplate的序列化策略，采用Jackson将对象转成json，查询的时候将json转成对象。<br />具体实现如下：
```java
@Configuration
public class RedisTemplateConfig {

    /**
     * 默认是JDK的序列化策略，这里配置redisTemplate采用的是Jackson2JsonRedisSerializer的序列化策略
     * @param factory
     * @return
     */
    @Bean
    public RedisTemplate<String, Object> redisTemplate(RedisConnectionFactory factory) {
        // 使用Jackson2JsonRedisSerializer来序列化和反序列化redis的value值（默认使用JDK的序列化方式）
        Jackson2JsonRedisSerializer<Object> jacksonSeial = new Jackson2JsonRedisSerializer<>(Object.class);

        // 使用Jackson序列号对象
        ObjectMapper objectMapper = new ObjectMapper();
        // 指定要序列化的域，field,get和set,以及修饰符范围，ANY是都有包括private和public
        objectMapper.setVisibility(PropertyAccessor.ALL,JsonAutoDetect.Visibility.ANY);
        // 指定序列化输入的类型，类必须是非final修饰的，final修饰的类，比如String,Integer等会抛出异常
        objectMapper.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
        jacksonSeial.setObjectMapper(objectMapper);

        // 使用RedisTemplate对象
        RedisTemplate<String, Object> template = new RedisTemplate<>();
        // 配置连接工厂
        template.setConnectionFactory(factory);
        // 使用StringRedisSerializer来序列化和反序列化redis的key值
        template.setKeySerializer(new StringRedisSerializer());
        // 值采用json序列化
        template.setValueSerializer(jacksonSeial);
        // 使用StringRedisSerializer来序列化和反序列化redis的hash-key值
        template.setHashKeySerializer(new StringRedisSerializer());
        // 值采用json序列化
        template.setHashValueSerializer(jacksonSeial);

        //执行后续方法
        template.afterPropertiesSet();
        return template;
    }
}
```
<a name="cijHU"></a>
### 2.4、RedisTemplate 使用介绍
redis 提供的数据结构很丰富，支持字符串、哈希表、列表、集合、有序集合等数据类型的存储，`RedisTemplate`对这五种数据结构分别定义了不同的操作类，具体如下：

- `ValueOperations`：操作最简单的K-V数据
- `ListOperations`：操作list类型的数据
- `HashOperations`：操作hash类型的数据
- `SetOperations`：操作set类型的数据
- `ZSetOperations`：操作zset类型的数据

相关的 api 操作如下！
<a name="tjIy1"></a>
#### 2.4.1、操作字符串的 api
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = RedisApplication.class)
public class RedisTest {

    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    @Test
    public void test() throws Exception {
        // 设置值，默认不过期
        stringRedisTemplate.opsForValue().set("userName", "张三");

        // 获取值
        String value = stringRedisTemplate.opsForValue().get("userName");
        System.out.println("获取userName对应的值：" +  value);

        // 设置值并且设置2秒过期时间，过期之后自动删除
        stringRedisTemplate.opsForValue().set("email", "123@123.com", 2, TimeUnit.SECONDS);
        Thread.sleep(1000);
        System.out.println("获取email过期时间（单位秒）：" + stringRedisTemplate.getExpire("email"));
        System.out.println("获取email对应的值：" +  stringRedisTemplate.opsForValue().get("email"));
        Thread.sleep(1000);
        System.out.println("获取email对应的值：" +  stringRedisTemplate.opsForValue().get("email"));

        // 删除key
        Boolean result = stringRedisTemplate.delete("userName");
        System.out.println("删除userName结果：" +  result);
    }
}
```
<a name="KGAfh"></a>
#### 2.4.2、操作对象的 api
```java
public class UserVo {

    private String email;

    private String name;

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public UserVo(String email, String name) {
        this.email = email;
        this.name = name;
    }

    public UserVo() {
    }

    @Override
    public String toString() {
        return "UserVo{" +
                "email='" + email + '\'' +
                ", name='" + name + '\'' +
                '}';
    }
}
```
```java
@Autowired
private RedisTemplate redisTemplate;

@Test
public void test() throws Exception {
    // 设置对象值，并且2秒自动过期
    ValueOperations<String, UserVo> operations = redisTemplate.opsForValue();
    UserVo user = new UserVo("aa@126.com", "张三");
    operations.set("user", user, 2, TimeUnit.SECONDS);

    //获取对象值
    UserVo userVo = operations.get("user");
    System.out.println(userVo.toString());
    System.out.println("获取user过期时间（单位秒）：" + redisTemplate.getExpire("user"));


    //删除key
    Boolean deleteValue = redisTemplate.delete("user");
    System.out.println("删除userName结果：" +  deleteValue);
}
```
<a name="RLD2V"></a>
#### 2.4.3、操作列表的 api
```java
@Autowired
private RedisTemplate redisTemplate;

@Test
public void test() throws Exception {
    // 向列表中添加数据
    ListOperations<String, UserVo> operations = redisTemplate.opsForList();
    // 往List左侧插入一个元素
    operations.leftPush("userList", new UserVo("aa@126.com", "张三"));
    operations.leftPush("userList", new UserVo("bb@126.com", "里斯"));
    //往 List 右侧插入一个元素
    operations.rightPush("userList", new UserVo("cc@126.com", "王五"));
    operations.rightPush("userList", new UserVo("dd@126.com", "赵六"));
    // 获取List 大小
    Long size = operations.size("userList");
    System.out.println("获取列表总数：" + size);
    //遍历整个List
    List<UserVo> allUserVo1 = operations.range("userList", 0, size);
    System.out.println("遍历列表所有数据：" + JacksonUtils.toJson(allUserVo1));
    //遍历整个List，-1表示倒数第一个即最后一个
    List<UserVo> allUserVo2 = operations.range("userList", 0, -1);
    System.out.println("遍历列表所有数据：" + JacksonUtils.toJson(allUserVo2));
    //从 List 左侧取出第一个元素，并移除
    Object userVo1 = operations.leftPop("userList", 200, TimeUnit.MILLISECONDS);
    System.out.println("从左侧取出第一个元素并移除：" + userVo1.toString());
    //从 List 右侧取出第一个元素，并移除
    Object userVo2 = operations.rightPop("userList", 200, TimeUnit.MILLISECONDS);
    System.out.println("从右侧取出第一个元素并移除：" + userVo2.toString());

}
```
<a name="lUqrH"></a>
#### 2.4.4、操作哈希的 api
```java
@Autowired
private RedisTemplate redisTemplate;

@Test
public void test() throws Exception {
    // 向hash中添加数据
    HashOperations<String, String, Integer> operations = redisTemplate.opsForHash();
    //Hash 中新增元素。
    operations.put("score", "张三", 2);
    operations.put("score", "里斯", 1);
    operations.put("score", "王五", 3);
    operations.put("score", "赵六", 4);

    Boolean hasKey = operations.hasKey("score", "张三");
    System.out.println("检查是否存在【score】【张三】：" + hasKey);
    Integer value = operations.get("score", "张三");
    System.out.println("获取【score】【张三】的值：" + value);
    Set<String> keys = operations.keys("score");
    System.out.println("获取hash表【score】所有的key集合：" + JacksonUtils.toJson(keys));
    List<Integer> values = operations.values("score");
    System.out.println("获取hash表【score】所有的value集合：" + JacksonUtils.toJson(values));
    Map<String,Integer> map = operations.entries("score");
    System.out.println("获取hash表【score】下的map数据：" + JacksonUtils.toJson(map));
    Long delete = operations.delete("score", "里斯");
    System.out.println("删除【score】中key为【里斯】的数据：" + delete);
    Boolean result = redisTemplate.delete("score");
    System.out.println("删除整个key：" + result);

}
```
<a name="QBz4e"></a>
#### 2.4.5、操作集合的 api
```java
@Autowired
private RedisTemplate redisTemplate;

@Test
public void test() throws Exception {
    // 向集合中添加数据
    SetOperations<String, String> operations = redisTemplate.opsForSet();
    //向集合中添加元素,set元素具有唯一性
    operations.add("city", "北京","上海", "广州", "深圳", "武汉");
    Long size = operations.size("city");
    System.out.println("获取集合总数：" + size);
    //判断是否是集合中的元素
    Boolean isMember = operations.isMember("city", "广州");
    System.out.println("检查集合中是否存在指定元素：" + isMember);
    Set<String> cityNames = operations.members("city");
    System.out.println("获取集合所有元素：" + JacksonUtils.toJson(cityNames));
    Long remove = operations.remove("city", "广州");
    System.out.println("删除指定元素结果：" + remove);
    //移除并返回集合中的一个随机元素
    String cityName = operations.pop("city");
    System.out.println("移除并返回集合中的一个随机元素：" + cityName);
}
```
<a name="T7fxr"></a>
#### 2.4.6、操作有序集合的 api
```java
@Autowired
private RedisTemplate redisTemplate;

@Test
public void test() throws Exception {
    // 向有序集合中添加数据
    ZSetOperations<String, String> operations = redisTemplate.opsForZSet();
    //向有序集合中添加元素,set元素具有唯一性
    operations.add("cityName", "北京", 100);
    operations.add("cityName", "上海", 95);
    operations.add("cityName", "广州", 75);
    operations.add("cityName", "深圳", 85);
    operations.add("cityName", "武汉", 70);

    //获取变量指定区间的元素。0, -1表示全部
    Set<String> ranges = operations.range("cityName", 0, -1);
    System.out.println("获取有序集合所有元素：" + JacksonUtils.toJson(ranges));
    Set<String> byScores = operations.rangeByScore("cityName", 85, 100);
    System.out.println("获取有序集合所有元素（按分数从小到大）："+ JacksonUtils.toJson(byScores));
    Long zCard = operations.zCard("cityName");
    System.out.println("获取有序集合成员数: " + zCard);
    Long remove = operations.remove("cityName", "武汉");
    System.out.println("删除某个成员数结果: " + remove);

}
```
<a name="ZMKIg"></a>
#### 2.4.7、操作分布锁相关的 api
```java
@Autowired
private StringRedisTemplate stringRedisTemplate;


@Test
public void test(){
    //分布式自增ID
    for (int i = 0; i < 10; i++) {
        Long incrementId = stringRedisTemplate.opsForValue().increment("orderId");
        //设置2秒后自动过期
        stringRedisTemplate.expire("orderId", 2, TimeUnit.SECONDS);
        System.out.println("orderId当前值：" +  incrementId);
    }

    //分布式加锁，5秒自动过期
    boolean lock = lock("LOCK", "test", 5L);
    System.out.println("加锁结果：" +  lock);
    boolean unlock = releaseLock("LOCK", "test");
    System.out.println("解锁结果：" +  unlock);
}



/**
 * 直接加锁
 * @param key
 * @param value
 * @param expire
 * @return
 */
public boolean lock(String key,String value, Long expire){
    String luaScript = "if redis.call('setnx', KEYS[1], ARGV[1]) == 1 then return redis.call('expire', KEYS[1], ARGV[2]) else return 0 end";
    RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);
    Long result = stringRedisTemplate.execute(redisScript, Collections.singletonList(key), value, String.valueOf(expire));
    return result.equals(Long.valueOf(1));
}


/**
 * 释放锁
 * @param key
 * @param value
 * @return
 */
public boolean releaseLock(String key,String value){
    String luaScript = "if redis.call('get', KEYS[1]) == ARGV[1] then return redis.call('del', KEYS[1]) else return 0 end";
    RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);
    Long result = stringRedisTemplate.execute(redisScript, Collections.singletonList(key),value);
    return result.equals(Long.valueOf(1));
}
```
<a name="IQLRm"></a>
### 2.5、如果要换成 jedis，如果更换？
从 SpringBoot 2.x 开始，spring-boot-starter-data-redis默认集成的客户端是Lettuce，但是有的项目使用了Jedis依赖包相关的代码，如何无缝替换呢？<br />在pom.xml文件中，添加Jedis依赖包，排除相关的包即可，示例代码如下：
```xml
<!-- Redis相关依赖包 -->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
    <exclusions>
        <exclusion>
            <groupId>redis.clients</groupId>
            <artifactId>jedis</artifactId>
        </exclusion>
        <exclusion>
            <artifactId>lettuce-core</artifactId>
            <groupId>io.lettuce</groupId>
        </exclusion>
    </exclusions>
</dependency>
<dependency>
    <groupId>redis.clients</groupId>
    <artifactId>jedis</artifactId>
</dependency>
<dependency>
    <groupId>org.apache.commons</groupId>
    <artifactId>commons-pool2</artifactId>
</dependency>
```
最后，在application.properties中，添加jedis相关配置，内容如下：
```
# Redis数据库索引（默认为0）
spring.redis.database=1
# Redis服务器地址
spring.redis.host=127.0.0.1
# Redis服务器连接端口
spring.redis.port=6379
# Redis服务器连接密码（默认为空）
spring.redis.password=
# Redis服务器连接超时配置
spring.redis.timeout=1000

# 连接池配置
spring.redis.jedis.pool.max-active=8
spring.redis.jedis.pool.max-wait=1000
spring.redis.jedis.pool.max-idle=8
spring.redis.jedis.pool.min-idle=0
spring.redis.jedis.pool.time-between-eviction-runs=100
```
<a name="bvynw"></a>
### 2.6、手动封装一个分布式锁实现类
默认情况下，无论是Jedis还是Lettuce，都没有为我们提供redis分布式锁的实现，因此我们自己进行封装，当然你也可以直接添加Redisson包，里面也提供了分布式锁实现的相关 API。<br />如果当前 redis 是单机环境，或者哨兵模式，完全可以自行封装一个分布式锁实现类，具体代码如下：
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.data.redis.core.script.DefaultRedisScript;
import org.springframework.data.redis.core.script.RedisScript;
import org.springframework.stereotype.Component;

import java.time.Duration;
import java.util.Collections;

/**
 * redis分布式锁服务类
 * 采用LUA脚本实现，保证加锁、解锁操作原子性
 */
@Component
public class RedisLockService {

    /**
     * 分布式锁过期时间，单位秒
     */
    private static final Long DEFAULT_LOCK_EXPIRE_TIME = 60L;

    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    /**
     * 尝试在指定时间内加锁
     * @param key
     * @param value
     * @param timeout 锁等待时间
     * @return
     */
    public boolean tryLock(String key,String value, Duration timeout){
        long waitMills = timeout.toMillis();
        long currentTimeMillis = System.currentTimeMillis();
        do {
            boolean lock = lock(key, value, DEFAULT_LOCK_EXPIRE_TIME);
            if (lock) {
                return true;
            }
            try {
                Thread.sleep(1L);
            } catch (InterruptedException e) {
                Thread.interrupted();
            }
        } while (System.currentTimeMillis() < currentTimeMillis + waitMills);
        return false;
    }

    /**
     * 直接加锁
     * @param key
     * @param value
     * @param expire
     * @return
     */
    public boolean lock(String key,String value, Long expire){
        String luaScript = "if redis.call('setnx', KEYS[1], ARGV[1]) == 1 then return redis.call('expire', KEYS[1], ARGV[2]) else return 0 end";
        RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);
        Long result = stringRedisTemplate.execute(redisScript, Collections.singletonList(key), value, String.valueOf(expire));
        return result.equals(Long.valueOf(1));
    }


    /**
     * 释放锁
     * @param key
     * @param value
     * @return
     */
    public boolean releaseLock(String key,String value){
        String luaScript = "if redis.call('get', KEYS[1]) == ARGV[1] then return redis.call('del', KEYS[1]) else return 0 end";
        RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);
        Long result = stringRedisTemplate.execute(redisScript, Collections.singletonList(key),value);
        return result.equals(Long.valueOf(1));
    }
}
```
<a name="KrNuB"></a>
## 三、关于 key 的设计
通常情况下，对key采用如下方式进行设计，以便与其他项目中的key错开，避免发生冲突！
```
固定前缀:项目名:数据库名:表名:字段名:具体的值
```
其次，无论什么时候，只要有可能就利用key超时的优势，尽可能避免数据永久存储，因为一旦所有的key都永久存储，大量无效的key，会服务器资源非常严重不足，甚至不可用！
