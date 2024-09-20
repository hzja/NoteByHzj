JavaSpring
<a name="sMjsJ"></a>
### 安装Redis
下载地址：[https://redis.io/download](https://redis.io/download)
<a name="xNbxp"></a>
### pom.xml中引入Redis客户端依赖库
```xml
<dependency>
  <groupId>org.redisson</groupId>
  <artifactId>redisson</artifactId>
  <version>3.13.3</version>
</dependency>
```
<a name="evYyy"></a>
### 项目中创建Redis配置文件
新建com/javacode2018/cache/demo2/redis.yml，内容如下：
```yaml
singleServerConfig:
  address: "redis://127.0.0.1:6379"
  password: null
  clientName: null
  database: 7 #选择使用哪个数据库0~15
  idleConnectionTimeout: 10000
  connectTimeout: 10000
  timeout: 3000
codec:
  class: "org.redisson.codec.JsonJacksonCodec"
```
<a name="IP6rJ"></a>
### 创建Redis相关的bean
```java
import org.redisson.Redisson;
import org.redisson.api.RedissonClient;
import org.redisson.config.Config;
import org.redisson.spring.cache.RedissonSpringCacheManager;
import org.springframework.cache.CacheManager;
import org.springframework.cache.annotation.EnableCaching;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;

import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

@ComponentScan
@EnableCaching //@1
public class MainConfig2 {
    @Bean //@2
    public CacheManager cacheManager() throws IOException {
        RedissonSpringCacheManager cacheManager = new RedissonSpringCacheManager(this.redissonClient());
        cacheManager.setCacheNames(Arrays.asList("cache1"));
        return cacheManager;
    }

    @Bean //@3
    public RedissonClient redissonClient() throws IOException {
        InputStream is = MainConfig2.class.getResourceAsStream("/com/javacode2018/cache/demo2/redis.yml");
        Config config = Config.fromYAML(is);
        return Redisson.create(config);
    }
}
```
@1：开启spring cache功能。<br />@2：自定义spring中cache管理器，这个地方定义了一个redis类型的管理器，底层使用Redis来作为缓存的存储介质。<br />@3：通过redis.yml配置文件来创建一个`RedissonClient`，用于和Redis进行交互。
<a name="kmkR4"></a>
### 来个测试类
```java
import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Component;

import java.util.Arrays;
import java.util.List;

@Component
public class BookService {

    @Cacheable(cacheNames = "cache1", key = "#root.targetClass.name+'-'+#root.method.name")
    public List<String> list() {
        System.out.println("---模拟从db中获取数据---");
        return Arrays.asList("java高并发", "springboot", "springcloud");
    }

}
```
<a name="TKGcp"></a>
### 测试用例
```java
@Test
public void test7() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig2.class);

    BookService bookService = context.getBean(BookService.class);
    System.out.println(bookService.list());
    System.out.println(bookService.list());

    {
        System.out.println("下面打印出cache1缓存中的key列表");
        RedissonSpringCacheManager cacheManager = context.getBean(RedissonSpringCacheManager.class);
        RedissonCache cache1 = (RedissonCache) cacheManager.getCache("cache1");
        cache1.getNativeCache().keySet().stream().forEach(System.out::println);
    }
}
```
<a name="yhvdf"></a>
### 运行输出
```
---模拟从db中获取数据---
[java高并发, springboot, springcloud]
[java高并发, springboot, springcloud]
下面打印出cache1缓存中的key列表
com.javacode2018.cache.demo2.BookService-list
```
此时数据已经进入Redis了，可以用Redis客户端工具RedisDesktopManager看一下。
