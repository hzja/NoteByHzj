Java
<a name="tAyc6"></a>
## 一、Caffeine介绍
<a name="ErFeK"></a>
### 1、缓存介绍
缓存(Cache)在代码世界中无处不在。从底层的CPU多级缓存，到客户端的页面缓存，处处都存在着缓存的身影。缓存从本质上来说，是一种空间换时间的手段，通过对数据进行一定的空间安排，使得下次进行数据访问时起到加速的效果。<br />就Java而言，其常用的缓存解决方案有很多，例如数据库缓存框架EhCache，分布式缓存Memcached等，这些缓存方案实际上都是为了提升吞吐效率，避免持久层压力过大。<br />对于常见缓存类型而言，可以分为本地缓存以及分布式缓存两种，Caffeine就是一种优秀的本地缓存，而Redis可以用来做分布式缓存
<a name="scbMU"></a>
### 2、Caffeine介绍
Caffeine官方：[https://github.com/ben-manes/caffeine](https://github.com/ben-manes/caffeine)<br />Caffeine是基于Java 1.8的高性能本地缓存库，由Guava改进而来，而且在Spring5开始的默认缓存实现就将Caffeine代替原来的Google Guava，官方说明指出，其缓存命中率已经接近最优值。实际上Caffeine这样的本地缓存和`ConcurrentMap`很像，即支持并发，并且支持O(1)时间复杂度的数据存取。二者的主要区别在于：

- `ConcurrentMap`将存储所有存入的数据，直到显式将其移除；
- Caffeine将通过给定的配置，自动移除“不常用”的数据，以保持内存的合理占用。

因此，一种更好的理解方式是：Cache是一种带有存储和移除策略的Map。<br />![2023-05-14-21-27-08.206144000.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1684071001844-ba2a46ec-b993-4fbb-9074-0a9dcb54c12f.png#averageHue=%23faf9f8&clientId=ubc20d5eb-b833-4&from=ui&id=u9eeb7c07&originHeight=318&originWidth=640&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=611940&status=done&style=none&taskId=ucf8e0b44-4ed3-4936-8c98-ac5a6fd8cc2&title=)
<a name="gXufp"></a>
## 二、Caffeine基础
使用Caffeine，需要在工程中引入如下依赖
```xml
<dependency>
  <groupId>com.github.ben-manes.caffeine</groupId>
  <artifactId>caffeine</artifactId>
  <!--https://mvnrepository.com/artifact/com.github.ben-manes.caffeine/caffeinez找最新版-->
  <version>3.0.5</version>
</dependency>
```
<a name="kJkQj"></a>
### 1、缓存加载策略
<a name="KRUQv"></a>
#### 1.1 Cache手动创建
最普通的一种缓存，无需指定加载方式，需要手动调用`put()`进行加载。需要注意的是`put()`方法对于已存在的key将进行覆盖，这点和Map的表现是一致的。在获取缓存值时，如果想要在缓存值不存在时，原子地将值写入缓存，则可以调用`get(key, k -> value)`方法，该方法将避免写入竞争。调用`invalidate()`方法，将手动移除缓存。<br />在多线程情况下，当使用`get(key, k -> value)`时，如果有另一个线程同时调用本方法进行竞争，则后一线程会被阻塞，直到前一线程更新缓存完成；而若另一线程调用`getIfPresent()`方法，则会立即返回null，不会被阻塞。
```java
Cache<Object, Object> cache = Caffeine.newBuilder()
          //初始数量
          .initialCapacity(10)
          //最大条数
          .maximumSize(10)
          //expireAfterWrite和expireAfterAccess同时存在时，以expireAfterWrite为准
          //最后一次写操作后经过指定时间过期
          .expireAfterWrite(1, TimeUnit.SECONDS)
          //最后一次读或写操作后经过指定时间过期
          .expireAfterAccess(1, TimeUnit.SECONDS)
          //监听缓存被移除
          .removalListener((key, val, removalCause) -> { })
          //记录命中
          .recordStats()
          .build();

  cache.put("1","张三");
  //张三
  System.out.println(cache.getIfPresent("1"));
  //存储的是默认值
  System.out.println(cache.get("2",o -> "默认值"));
```
<a name="Tz6h8"></a>
#### 1.2 Loading Cache自动创建
`LoadingCache`是一种自动加载的缓存。其和普通缓存不同的地方在于，当缓存不存在/缓存已过期时，若调用`get()`方法，则会自动调用`CacheLoader.load()`方法加载最新值。调用`getAll()`方法将遍历所有的key调用`get()`，除非实现了`CacheLoader.loadAll()`方法。使用`LoadingCache`时，需要指定`CacheLoader`，并实现其中的`load()`方法供缓存缺失时自动加载。<br />在多线程情况下，当两个线程同时调用`get()`，则后一线程将被阻塞，直至前一线程更新缓存完成。
```java
LoadingCache<String, String> loadingCache = Caffeine.newBuilder()
        //创建缓存或者最近一次更新缓存后经过指定时间间隔，刷新缓存；refreshAfterWrite仅支持LoadingCache
        .refreshAfterWrite(10, TimeUnit.SECONDS)
        .expireAfterWrite(10, TimeUnit.SECONDS)
        .expireAfterAccess(10, TimeUnit.SECONDS)
        .maximumSize(10)
        //根据key查询数据库里面的值，这里是个lamba表达式
        .build(key -> new Date().toString());
```
<a name="xNAcR"></a>
#### 1.3 Async Cache异步获取
AsyncCache是Cache的一个变体，其响应结果均为`CompletableFuture`，通过这种方式，AsyncCache对异步编程模式进行了适配。默认情况下，缓存计算使用`ForkJoinPool.commonPool()`作为线程池，如果想要指定线程池，则可以覆盖并实现`Caffeine.executor(Executor)`方法。`synchronous()`提供了阻塞直到异步缓存生成完毕的能力，它将以Cache进行返回。<br />在多线程情况下，当两个线程同时调用`get(key, k -> value)`，则会返回同一个`CompletableFuture`对象。由于返回结果本身不进行阻塞，可以根据业务设计自行选择阻塞等待或者非阻塞。
```java
AsyncLoadingCache<String, String> asyncLoadingCache = Caffeine.newBuilder()
        //创建缓存或者最近一次更新缓存后经过指定时间间隔刷新缓存；仅支持LoadingCache
        .refreshAfterWrite(1, TimeUnit.SECONDS)
        .expireAfterWrite(1, TimeUnit.SECONDS)
        .expireAfterAccess(1, TimeUnit.SECONDS)
        .maximumSize(10)
        //根据key查询数据库里面的值
        .buildAsync(key -> {
            Thread.sleep(1000);
            return new Date().toString();
        });

//异步缓存返回的是CompletableFuture
CompletableFuture<String> future = asyncLoadingCache.get("1");
future.thenAccept(System.out::println);
```
<a name="vfUPs"></a>
### 2、驱逐策略
驱逐策略在创建缓存的时候进行指定。常用的有基于容量的驱逐和基于时间的驱逐。<br />基于容量的驱逐需要指定缓存容量的最大值，当缓存容量达到最大时，Caffeine将使用LRU策略对缓存进行淘汰；基于时间的驱逐策略如字面意思，可以设置在最后访问/写入一个缓存经过指定时间后，自动进行淘汰。<br />驱逐策略可以组合使用，任意驱逐策略生效后，该缓存条目即被驱逐。

- LRU 最近最少使用，淘汰最长时间没有被使用的页面。
- LFU 最不经常使用，淘汰一段时间内使用次数最少的页面
- FIFO 先进先出

Caffeine有4种缓存淘汰设置

- 大小 （LFU算法进行淘汰）
- 权重 （大小与权重 只能二选一）
- 时间
- 引用 （不常用，本文不介绍）
```java
@Slf4j
public class CacheTest {
    /**
     * 缓存大小淘汰
     */
    @Test
    public void maximumSizeTest() throws InterruptedException {
        Cache<Integer, Integer> cache = Caffeine.newBuilder()
                //超过10个后会使用W-TinyLFU算法进行淘汰
                .maximumSize(10)
                .evictionListener((key, val, removalCause) -> {
                    log.info("淘汰缓存：key:{} val:{}", key, val);
                })
                .build();

        for (int i = 1; i < 20; i++) {
            cache.put(i, i);
        }
        Thread.sleep(500);//缓存淘汰是异步的

        // 打印还没被淘汰的缓存
        System.out.println(cache.asMap());
    }

    /**
     * 权重淘汰
     */
    @Test
    public void maximumWeightTest() throws InterruptedException {
        Cache<Integer, Integer> cache = Caffeine.newBuilder()
                //限制总权重，若所有缓存的权重加起来>总权重就会淘汰权重小的缓存
                .maximumWeight(100)
                .weigher((Weigher<Integer, Integer>) (key, value) -> key)
                .evictionListener((key, val, removalCause) -> {
                    log.info("淘汰缓存：key:{} val:{}", key, val);
                })
                .build();

        //总权重其实是=所有缓存的权重加起来
        int maximumWeight = 0;
        for (int i = 1; i < 20; i++) {
            cache.put(i, i);
            maximumWeight += i;
        }
        System.out.println("总权重=" + maximumWeight);
        Thread.sleep(500);//缓存淘汰是异步的

        // 打印还没被淘汰的缓存
        System.out.println(cache.asMap());
    }


    /**
     * 访问后到期（每次访问都会重置时间，也就是说如果一直被访问就不会被淘汰）
     */
    @Test
    public void expireAfterAccessTest() throws InterruptedException {
        Cache<Integer, Integer> cache = Caffeine.newBuilder()
                .expireAfterAccess(1, TimeUnit.SECONDS)
                //可以指定调度程序来及时删除过期缓存项，而不是等待Caffeine触发定期维护
                //若不设置scheduler，则缓存会在下一次调用get的时候才会被动删除
                .scheduler(Scheduler.systemScheduler())
                .evictionListener((key, val, removalCause) -> {
                    log.info("淘汰缓存：key:{} val:{}", key, val);

                })
                .build();
        cache.put(1, 2);
        System.out.println(cache.getIfPresent(1));
        Thread.sleep(3000);
        System.out.println(cache.getIfPresent(1));//null
    }

    /**
     * 写入后到期
     */
    @Test
    public void expireAfterWriteTest() throws InterruptedException {
        Cache<Integer, Integer> cache = Caffeine.newBuilder()
                .expireAfterWrite(1, TimeUnit.SECONDS)
                //可以指定调度程序来及时删除过期缓存项，而不是等待Caffeine触发定期维护
                //若不设置scheduler，则缓存会在下一次调用get的时候才会被动删除
                .scheduler(Scheduler.systemScheduler())
                .evictionListener((key, val, removalCause) -> {
                    log.info("淘汰缓存：key:{} val:{}", key, val);
                })
                .build();
        cache.put(1, 2);
        Thread.sleep(3000);
        System.out.println(cache.getIfPresent(1));//null
    }
}
```
<a name="bFMws"></a>
### 3、刷新机制
`refreshAfterWrite()`表示x秒后自动刷新缓存的策略可以配合淘汰策略使用，注意的是刷新机制只支持`LoadingCache`和`AsyncLoadingCache`
```java
private static int NUM = 0;

@Test
public void refreshAfterWriteTest() throws InterruptedException {
    LoadingCache<Integer, Integer> cache = Caffeine.newBuilder()
            .refreshAfterWrite(1, TimeUnit.SECONDS)
            //模拟获取数据，每次获取就自增1
            .build(integer -> ++NUM);

    //获取ID=1的值，由于缓存里还没有，所以会自动放入缓存
    System.out.println(cache.get(1));// 1

    // 延迟2秒后，理论上自动刷新缓存后取到的值是2
    // 但其实不是，值还是1，因为refreshAfterWrite并不是设置了n秒后重新获取就会自动刷新
    // 而是x秒后&&第二次调用getIfPresent的时候才会被动刷新
    Thread.sleep(2000);
    System.out.println(cache.getIfPresent(1));// 1

    //此时才会刷新缓存，而第一次拿到的还是旧值
    System.out.println(cache.getIfPresent(1));// 2
}
```
<a name="DeYoU"></a>
### 4、统计
```java
LoadingCache<String, String> cache = Caffeine.newBuilder()
        //创建缓存或者最近一次更新缓存后经过指定时间间隔，刷新缓存；refreshAfterWrite仅支持LoadingCache
        .refreshAfterWrite(1, TimeUnit.SECONDS)
        .expireAfterWrite(1, TimeUnit.SECONDS)
        .expireAfterAccess(1, TimeUnit.SECONDS)
        .maximumSize(10)
        //开启记录缓存命中率等信息
        .recordStats()
        //根据key查询数据库里面的值
        .build(key -> {
            Thread.sleep(1000);
            return new Date().toString();
        });


cache.put("1", "shawn");
cache.get("1");

/*
 * hitCount :命中的次数
 * missCount:未命中次数
 * requestCount:请求次数
 * hitRate:命中率
 * missRate:丢失率
 * loadSuccessCount:成功加载新值的次数
 * loadExceptionCount:失败加载新值的次数
 * totalLoadCount:总条数
 * loadExceptionRate:失败加载新值的比率
 * totalLoadTime:全部加载时间
 * evictionCount:丢失的条数
 */
System.out.println(cache.stats());
```
<a name="Y3AYh"></a>
### 5、总结
上述一些策略在创建时都可以进行自由组合，一般情况下有两种方法

- 设置 `maxSize`、`refreshAfterWrite`，不设置 `expireAfterWrite`/`expireAfterAccess`，设置`expireAfterWrite`当缓存过期时会同步加锁获取缓存，所以设置`expireAfterWrite`时性能较好，但是某些时候会取旧数据，适合允许取到旧数据的场景
- 设置 `maxSize`、`expireAfterWrite`/`expireAfterAccess`，不设置 `refreshAfterWrite` 数据一致性好，不会获取到旧数据，但是性能没那么好（对比起来），适合获取数据时不耗时的场景
<a name="fhS0E"></a>
## 三、SpringBoot整合Caffeine
<a name="a2zEM"></a>
### 1、`@Cacheable`相关注解
<a name="FIXg5"></a>
#### 1.1 相关依赖
如果要使用`@Cacheable`注解，需要引入相关依赖，并在任一配置类文件上添加`@EnableCaching`注解
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-cache</artifactId>
</dependency>
```
<a name="aHYPS"></a>
#### 1.2 常用注解

- `**@Cacheable**`：表示该方法支持缓存。当调用被注解的方法时，如果对应的键已经存在缓存，则不再执行方法体，而从缓存中直接返回。当方法返回null时，将不进行缓存操作。
- `**@CachePut**`：表示执行该方法后，其值将作为最新结果更新到缓存中，每次都会执行该方法。
- `**@CacheEvict**`：表示执行该方法后，将触发缓存清除操作。
- `**@Caching**`：用于组合前三个注解，例如：
```java
@Caching(cacheable = @Cacheable("CacheConstants.GET_USER"),
         evict = {@CacheEvict("CacheConstants.GET_DYNAMIC",allEntries = true)}
public User find(Integer id) {
    return null;
}
```
<a name="THi76"></a>
#### 1.3 常用注解属性

- `**cacheNames**`**/value**：缓存组件的名字，即`cacheManager`中缓存的名称。
- `**key**`：缓存数据时使用的key。默认使用方法参数值，也可以使用SpEL表达式进行编写。
- `**keyGenerator**`：和key二选一使用。
- `**cacheManager**`：指定使用的缓存管理器。
- `**condition**`：在方法执行开始前检查，在符合`condition`的情况下，进行缓存
- `**unless**`：在方法执行完成后检查，在符合unless的情况下，不进行缓存
- `**sync**`：是否使用同步模式。若使用同步模式，在多个线程同时对一个key进行load时，其他线程将被阻塞。
<a name="zJZgU"></a>
#### 1.4 缓存同步模式
`sync`开启或关闭，在`Cache`和`LoadingCache`中的表现是不一致的：

- `Cache`中，`sync`表示是否需要所有线程同步等待
- `LoadingCache`中，sync表示在读取不存在/已驱逐的key时，是否执行被注解方法
<a name="SybK0"></a>
### 2、实战
<a name="g19o6"></a>
#### 2.1 引入依赖
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-cache</artifactId>
</dependency>

<dependency>
  <groupId>com.github.ben-manes.caffeine</groupId>
  <artifactId>caffeine</artifactId>
</dependency>
```
<a name="JXV3H"></a>
#### 2.2 缓存常量`CacheConstants`
创建缓存常量类，把公共的常量提取一层，复用，这里也可以通过配置文件加载这些数据，例如`@ConfigurationProperties`和`@Value`
```java
public class CacheConstants {
    /**
     * 默认过期时间（配置类中我使用的时间单位是秒，所以这里如 3*60 为3分钟）
     */
    public static final int DEFAULT_EXPIRES = 3 * 60;
    public static final int EXPIRES_5_MIN = 5 * 60;
    public static final int EXPIRES_10_MIN = 10 * 60;

    public static final String GET_USER = "GET:USER";
    public static final String GET_DYNAMIC = "GET:DYNAMIC";

}
```
<a name="SB27e"></a>
#### 2.3 缓存配置类`CacheConfig`
```java
@Configuration
@EnableCaching
public class CacheConfig {
    /**
     * Caffeine配置说明：
     * initialCapacity=[integer]: 初始的缓存空间大小
     * maximumSize=[long]: 缓存的最大条数
     * maximumWeight=[long]: 缓存的最大权重
     * expireAfterAccess=[duration]: 最后一次写入或访问后经过固定时间过期
     * expireAfterWrite=[duration]: 最后一次写入后经过固定时间过期
     * refreshAfterWrite=[duration]: 创建缓存或者最近一次更新缓存后经过固定的时间间隔，刷新缓存
     * weakKeys: 打开key的弱引用
     * weakValues：打开value的弱引用
     * softValues：打开value的软引用
     * recordStats：开发统计功能
     * 注意：
     * expireAfterWrite和expireAfterAccess同事存在时，以expireAfterWrite为准。
     * maximumSize和maximumWeight不可以同时使用
     * weakValues和softValues不可以同时使用
     */
    @Bean
    public CacheManager cacheManager() {
        SimpleCacheManager cacheManager = new SimpleCacheManager();
        List<CaffeineCache> list = new ArrayList<>();
        //循环添加枚举类中自定义的缓存，可以自定义
        for (CacheEnum cacheEnum : CacheEnum.values()) {
            list.add(new CaffeineCache(cacheEnum.getName(),
                                       Caffeine.newBuilder()
                                       .initialCapacity(50)
                                       .maximumSize(1000)
                                       .expireAfterAccess(cacheEnum.getExpires(), TimeUnit.SECONDS)
                                       .build()));
        }
        cacheManager.setCaches(list);
        return cacheManager;
    }
}
```
<a name="duDoI"></a>
#### 2.4 调用缓存
这里要注意的是Cache和`@Transactional`一样也使用了代理，类内调用将失效
```java
/**
 * value：缓存key的前缀。
 * key：缓存key的后缀。
 * sync：设置如果缓存过期是不是只放一个请求去请求数据库，其他请求阻塞，默认是false（根据个人需求）。
 * unless：不缓存空值,这里不使用，会报错
 * 查询用户信息类
 * 如果需要加自定义字符串，需要用单引号
 * 如果查询为null，也会被缓存
 */
@Cacheable(value = CacheConstants.GET_USER,key = "'user'+#userId",sync = true)
@CacheEvict
public UserEntity getUserByUserId(Integer userId){
    UserEntity userEntity = userMapper.findById(userId);
    System.out.println("查询了数据库");
    return userEntity;
}
```
