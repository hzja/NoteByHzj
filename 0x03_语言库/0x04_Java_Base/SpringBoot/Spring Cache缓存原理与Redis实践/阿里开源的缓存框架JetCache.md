<a name="AeY5o"></a>
## JetCache 是什么？
JetCache 是一个基于 Java 的缓存系统封装，提供统一的 API 和注解来简化缓存的使用。JetCache 提供了比 SpringCache 更加强大的注解，可以原生的支持 TTL、两级缓存、分布式自动刷新，还提供了Cache接口用于手工缓存操作。当前有四个实现，`RedisCache`、`TairCache`（此部分未在 github 开源）、`CaffeineCache`和一个简易的`LinkedHashMapCache`，要添加新的实现也是非常简单的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692624527691-f93e9ef3-86c2-4cf8-b15b-70562cc45c25.png#averageHue=%2316191f&clientId=u1da3ef26-e666-4&from=paste&id=u52a50d93&originHeight=594&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u46e55caf-dbb8-4bc8-808c-f3e536dde08&title=)<br />JetCache 全部特性如下:

- 通过统一的 API 访问 Cache 系统
- 通过注解实现声明式的方法缓存，支持 TTL 和两级缓存
- 通过注解创建并配置Cache实例
- 针对所有Cache实例和方法缓存的自动统计
- Key 的生成策略和 Value 的序列化策略是可以配置的
- 分布式缓存自动刷新，分布式锁 (2.2+)
- 异步 Cache API (2.2+，使用 Redis 的 lettuce 客户端时)
- Spring Boot 支持

相关地址：

- Github：[**https://github.com/alibaba/jetcache/blob/master/docs/CN/Readme.md**](https://github.com/alibaba/jetcache/blob/master/docs/CN/Readme.md)
- 中文文档：[**https://github.com/alibaba/jetcache/blob/master/docs/CN/Readme.md**](https://github.com/alibaba/jetcache/blob/master/docs/CN/Readme.md)
<a name="slEMh"></a>
## SpringCache vs JetCache
SpringCache 和 JetCache 两者的对比如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692624527615-b358ceef-8fef-40ff-8615-2594187852c0.png#averageHue=%23fafafa&clientId=u1da3ef26-e666-4&from=paste&id=u770d3912&originHeight=788&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc80c4459-b446-4666-ad7c-146e0e14274&title=)
<a name="dE4hr"></a>
## 快速入门
<a name="mepMv"></a>
### 创建缓存实例
通过 `@CreateCache` 注解创建一个缓存实例，默认超时时间是 100 秒
```java
@Autowired
private CacheManager cacheManager;

private Cache<Long, UserDO> userCache;

@PostConstruct
public void init() {
    QuickConfig qc = QuickConfig.newBuilder("userCache") // name用于统计信息展示名字
    .expire(Duration.ofSeconds(100))
    //.cacheType(CacheType.BOTH) // 创建一个两级缓存
    //.localLimit(100) // 本地缓存元素个数限制，只对CacheType.LOCAL和CacheType.BOTH有效
    //.syncLocal(true) // 两级缓存的情况下，缓存更新时发消息让其它JVM实例中的缓存失效，需要配置broadcastChannel才生效。
    .build();
    userCache = cacheManager.getOrCreateCache(qc);
}
```
用起来就像 map 一样：
```java
UserDO user = userCache.get(123L);
userCache.put(123L, user);
userCache.remove(123L);
```
<a name="PIs0I"></a>
### 创建方法缓存
使用`@Cached` 方法可以为一个方法添加上缓存。JetCache 通过 Spring AOP 生成代理，来支持缓存功能。注解可以加在接口方法上也可以加在类方法上，但需要保证是个 Spring bean。
```java
public interface UserService {
    @Cached(name="UserService.getUserById", expire = 3600)
    User getUserById(long userId);
}
```
<a name="IWERV"></a>
### 基本配置（使用 Spring Boot）
如果使用 Spring Boot，可以按如下的方式配置（这里使用了 jedis 客户端连接 redis，也可以使用lettuce客户端）。<br />POM：
```xml
<dependency>
  <groupId>com.alicp.jetcache</groupId>
  <artifactId>jetcache-starter-redis</artifactId>
  <version>${jetcache.latest.version}</version>
</dependency>
```
配置一个 SpringBoot 风格的 application.yml 文件，把他放到资源目录中
```yaml
jetcache:
  statIntervalMinutes: 15
  areaInCacheName: false
  local:
    default:
      type: linkedhashmap
      keyConvertor: fastjson
  remote:
    default:
      type: redis
      keyConvertor: fastjson2
      broadcastChannel: projectA
      valueEncoder: java
      valueDecoder: java
      poolConfig:
        minIdle: 5
        maxIdle: 20
        maxTotal: 50
      host: 127.0.0.1
      port: 6379
```
配置的解释如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692624527561-b64e260d-ffdc-4be3-a313-ceb9a14b0528.png#averageHue=%2324282e&clientId=u1da3ef26-e666-4&from=paste&id=ua8e0852b&originHeight=957&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucdccbe09-5f58-4387-81c7-212718cedea&title=)<br />然后创建一个 App 类放在业务包的根下，`EnableMethodCache`，`EnableCreateCacheAnnotation`这两个注解分别激活`Cached`和`CreateCache`注解，其他和标准的 Spring Boot 程序是一样的。这个类可以直接main方法运行。
```java
package com.company.mypackage;

import com.alicp.jetcache.anno.config.EnableCreateCacheAnnotation;
import com.alicp.jetcache.anno.config.EnableMethodCache;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
@EnableMethodCache(basePackages = "com.company.mypackage")
@EnableCreateCacheAnnotation
public class MySpringBootApp {
    public static void main(String[] args) {
        SpringApplication.run(MySpringBootApp.class);
    }
}
```
<a name="hUpAf"></a>
### 未使用 SpringBoot 的配置方式
如果没有使用 SpringBoot，可以按下面的方式配置（这里使用 jedis 客户端连接 redis 为例）。
```xml
<dependency>
  <groupId>com.alicp.jetcache</groupId>
  <artifactId>jetcache-anno</artifactId>
  <version>${jetcache.latest.version}</version>
</dependency>
<dependency>
  <groupId>com.alicp.jetcache</groupId>
  <artifactId>jetcache-redis</artifactId>
  <version>${jetcache.latest.version}</version>
</dependency>
```
配置了这个`JetCacheConfig`类以后，可以使用`@CreateCache`和`@Cached`注解。
```java
package com.company.mypackage;

import java.util.HashMap;
import java.util.Map;

import com.alicp.jetcache.anno.CacheConsts;
import com.alicp.jetcache.anno.config.EnableCreateCacheAnnotation;
import com.alicp.jetcache.anno.config.EnableMethodCache;
import com.alicp.jetcache.anno.support.GlobalCacheConfig;
import com.alicp.jetcache.anno.support.SpringConfigProvider;
import com.alicp.jetcache.embedded.EmbeddedCacheBuilder;
import com.alicp.jetcache.embedded.LinkedHashMapCacheBuilder;
import com.alicp.jetcache.redis.RedisCacheBuilder;
import com.alicp.jetcache.support.Fastjson2KeyConvertor;
import com.alicp.jetcache.support.JavaValueDecoder;
import com.alicp.jetcache.support.JavaValueEncoder;
import org.apache.commons.pool2.impl.GenericObjectPoolConfig;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.util.Pool;

@Configuration
@EnableMethodCache(basePackages = "com.company.mypackage")
@EnableCreateCacheAnnotation // deprecated in jetcache 2.7, 如果不用@CreateCache注解可以删除
@Import(JetCacheBaseBeans.class) //need since jetcache 2.7+
public class JetCacheConfig {

    @Bean
    public Pool<Jedis> pool(){
        GenericObjectPoolConfig pc = new GenericObjectPoolConfig();
        pc.setMinIdle(2);
        pc.setMaxIdle(10);
        pc.setMaxTotal(10);
        return new JedisPool(pc, "localhost", 6379);
    }

    //@Bean for jetcache <=2.6
    //public SpringConfigProvider springConfigProvider() {
    //    return new SpringConfigProvider();
    //}

    @Bean
    public GlobalCacheConfig config(Pool<Jedis> pool){
    // public GlobalCacheConfig config(SpringConfigProvider configProvider, Pool<Jedis> pool){ // for jetcache <=2.5
        Map localBuilders = new HashMap();
        EmbeddedCacheBuilder localBuilder = LinkedHashMapCacheBuilder
                .createLinkedHashMapCacheBuilder()
                .keyConvertor(FastjsonKeyConvertor.INSTANCE);
        localBuilders.put(CacheConsts.DEFAULT_AREA, localBuilder);

        Map remoteBuilders = new HashMap();
        RedisCacheBuilder remoteCacheBuilder = RedisCacheBuilder.createRedisCacheBuilder()
                .keyConvertor(Fastjson2KeyConvertor.INSTANCE)
                .valueEncoder(JavaValueEncoder.INSTANCE)
                .valueDecoder(JavaValueDecoder.INSTANCE)
                .broadcastChannel("projectA")
                .jedisPool(pool);
        remoteBuilders.put(CacheConsts.DEFAULT_AREA, remoteCacheBuilder);

        GlobalCacheConfig globalCacheConfig = new GlobalCacheConfig();
        // globalCacheConfig.setConfigProvider(configProvider); // for jetcache <= 2.5
        globalCacheConfig.setLocalCacheBuilders(localBuilders);
        globalCacheConfig.setRemoteCacheBuilders(remoteBuilders);
        globalCacheConfig.setStatIntervalMinutes(15);
        //globalCacheConfig.setAreaInCacheName(false); for jetcache <=2.6

        return globalCacheConfig;
    }
}
```
<a name="JsIBl"></a>
## JetCache 多级缓存支持
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692624527653-76d6fdfb-7c98-49c5-b960-172fe179f5ed.png#averageHue=%23627681&clientId=u1da3ef26-e666-4&from=paste&id=u5a7786db&originHeight=504&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u36ba0044-5bb0-489f-957e-bcce596fe52&title=)<br />多级缓存是 JetCache 最具特色的能力，可以非常方便快速的构建 缓存积木，从而用最低的成本提供最高的性能。

- 多级缓存本身可以有不同的过期时间，所以会构建出多级滑动窗口。
- 多级缓存非常简单直接的避免缓存击穿，缓存雪廓等问题。
- JetCache 默认的 Annotation 缓存支持两级缓存，但可以扩展至 N 级。
- 多级缓存可以添加任意自己实现的缓存类型（甚至可以包括 MongoDB, SearchEngin)。
- 多级缓存可以用大部分高级功能，通过`cache.config()`Api 来为多级 缓存添加各种能力。
