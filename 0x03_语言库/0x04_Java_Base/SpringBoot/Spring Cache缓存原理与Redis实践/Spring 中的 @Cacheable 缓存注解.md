JavaSpring
<a name="jukj5"></a>
## 1、什么是缓存
第一个问题，首先要搞明白什么是缓存，缓存的意义是什么。<br />对于普通业务，如果要查询一个数据，一般直接select数据库进行查找。但是在高流量的情况下，直接查找数据库就会成为性能的瓶颈。因为数据库查找的流程是先要从磁盘拿到数据，再刷新到内存，再返回数据。磁盘相比于内存来说，速度是很慢的，为了提升性能，就出现了基于内存的缓存。<br />这种基于内存的缓存，由于无法跟磁盘频繁进行存储，所以无法保证数据的完整性，随时有可能丢失，所以架构一般使用数据库加缓存的方式，数据库用来持久化数据，缓存用来处理大流量。
<a name="BsmQ5"></a>
## 2、本地缓存和集中式缓存
缓存按照存储方式可以分为这本地缓存和集中式缓存。<br />本地缓存顾名思义就是存储在本地上，例如静态变量就可以说是一种本地缓存，存储在了JVM中，或者说自己本地搭建的项目用的redis也算是本地缓存，因为缓存和应用都在一台机器上。<br />本地缓存效率很高，直接读取内存，没有网络延迟，但是可用性很低，因为出现单点故障的话，数据库和系统都会宕机。<br />对于大型项目来说，都会有集中式缓存，例如redis集群。缓存和应用服务器是分离的，服务器需要通过网络请求从缓存获取数据，一般应用服务器也会采取集群的方式，这样可以保证高可用，数据不易丢失，而且也能保证各个服务器的缓存数据一致。<br />对于分布式应用来说，本地缓存还会出现缓存不一致的问题，因为每个服务器的本地缓存都是独立的。
<a name="uJC3m"></a>
## 3、本地缓存的优点
刚才说了这么多本地缓存的缺点，那为什么还要用呢？<br />因为如果都放在集中式缓存中，网络延迟会成为性能的瓶颈。因为不在本地内存，读取的时间需要加上网络通信的时间。所以在对性能要求更大或者缓存内容不需要持久化、不需要一致性的情况下，本地缓存更适合。<br />所以一般的大型项目都采用本地缓存和集中式缓存混合使用的方式。
<a name="q5jm5"></a>
## 4、Spring对于缓存的支持
终于说到正题，本地缓存可以通过spring更简单的管理和使用。<br />SpringBoot和SpringMVC都支持缓存，其中`CacheManager`是Spring提供的缓存接口。
<a name="y0jCN"></a>
### 4.1 Spring支持的`CacheManager`
| `**CacheManager**` | **描述** |
| --- | --- |
| `SimpleCacheManager` | 使用简单的 Collection 来存储缓存，主要用于测试 |
| `ConcurrentMapCacheManager` | 使用 ConcurrentMap 来存储缓存 |
| `NoOpCacheManager` | 仅测试用途，不会实际缓存数据 |
| `EhCacheCacheManager` | 使用 EhCache 作为缓存技术 |
| `GuavaCacheManager` | 使用 Google Guava 的 GuavaCache 作为缓存技术 |
| `HazelcastCacheManager` | 使用 Hazelcast 作为缓存技术 |
| `JCacheCacheManager` | 支持 JCache(JSR-107) 标准的实现作为缓存技术，如 ApacheCommonsJCS |
| `RedisCacheManager` | 使用 Redis 作为缓存技术 |

看着非常多，实际上正常用的只有`ConcurrentMapCacheManager`，`EhCacheCacheManager`，`GuavaCacheManager`（一般使用redis，需要更灵活的对redis键值进行操作，所以不用`RedisCacheManager`），重点去讲一下这个`GuavaCacheManager`。
<a name="Ncho5"></a>
### 4.2 `GuavaCache`
Guava是谷歌开源的Java库，其中的代表就有这个缓存。<br />GuavaCache的原理大概是LRU+ConcurrentHashMap，加载在JVM的本地缓存
<a name="vwxTL"></a>
### 4.3 引入依赖
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-cache</artifactId>
    </dependency>
<dependency>
     <groupId>com.google.guava</groupId>
     <artifactId>guava</artifactId>
     <version>18.0</version>
</dependency>
//有可能需要这个
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-context-support</artifactId>
    <version>4.2.5.RELEASE</version>
</dependency>
```
<a name="uXm7L"></a>
### 4.4 创建配置类
```java
@EnableCaching
@Configuration
public class GuavaCacheConfig {
 
    @Bean
    public CacheManager cacheManager() {
        GuavaCacheManager cacheManager = new GuavaCacheManager();
        cacheManager.setCacheBuilder(
                CacheBuilder.newBuilder().
                        expireAfterWrite(3, TimeUnit.MINUTES));
        return cacheManager;
    }
}
```
`@EnableCaching`用来开启注解功能，这里设置的失效时间是3分钟。<br />Guava Cache 除了代码中提到的设置缓存过期时间的策略外，还有其他的策略。下面是 Guava Cache 设置缓存过期时间的策略：

- `**expireAfterAccess**`：当缓存项在指定的时间段内没有被读或写就会被回收。
- `**expireAfterWrite**`：当缓存项在指定的时间段内没有更新就会被回收，如果认为缓存数据在一段时间后数据不再可用，那么可以使用该种策略。
- `**refreshAfterWrite**`：当缓存项上一次更新操作之后的多久会被刷新。
<a name="icA3G"></a>
### 4.5 缓存注解
标题终于出现了

| 注解 | 解释 |
| --- | --- |
| `@Cacheable` | 在方法执行前 Spring 先查看缓存中是否有数据，若有，则直接返回缓存数据；若无数据，调用方法将方法返回值放入缓存中 |
| `@CachePut` | 无论怎样，都会将方法的返回值放到缓存中。 |
| `@CacheEvict` | 将一条或多条数据从缓存中删除 |
| `@Caching` | 可以通过 `@Caching` 注解组合多个注解策略在一个方法上 |

这里就主要解释下`@Cacheable`的用法，因为这个比较常见（其他的没用过）
<a name="m1tej"></a>
### 4.6 `@Cacheable`的用法
常用参数有

| **参数** | **解释** |
| --- | --- |
| value | 名字1 |
| key | 名字2 |
| `condition` | 条件判断，`condition="#id>1"`表示id大于1的才缓存 |
| `unless` | 条件判断 ，`unless="#id==1"`表示id等于1的不缓存 |

`#`代表的是EL表达式<br />这里的key和value和我们以为的缓存键值对是不一样的<br />value+key 只是缓存键的名字，真正的值是方法的返回值。<br />举一个例子
```java
@Cacheable(value = "olympic_match_new_action",key = "'get_relate_news_'+#rsc")
public List<MatchNewsVO> getRelateNews(String rsc){
    ....       
}
```
一般value取service名，key取方法名，取名按照数据库的下划线方式。后面那个#rsc指的是传进来的参数，这些都是键。返回的List就是缓存的值。
<a name="IwOHK"></a>
## 5、`@Cacheable`失效的原因
在配置正常的情况下，亲历的失效原因就是一个类的方法调用了带有缓存的方法，结果缓存失效。<br />使用service的A方法，想调用这个service的缓存B方法，这样是不行的。<br />原因是`@Cacheable`是由AOP代理实现，生成了带有缓存的代理类。其他类想调用这个类的缓存方法时，会去调用这个代理类的方法，实现缓存功能。但是类内部调用这个方法，就不会去调用代理类的方法，导致缓存失效
