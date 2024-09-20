JavaSpringBootCache<br />说到Spring Boot缓存，那就不得不提JSR-107规范，它说明在Java中如何规范地使用缓存。<br />JSR是Java Specification Requests的简称，通常译为”Java 规范提案“。具体而言，是指向JCP(Java Community Process，Java标准制定组织)提出新增一个标准化技术规范的正式请求。任何人都可以提交JSR，通过一定的标准测试后，就可以向Java平台增添新的API和服务。JSR已成为Java界的一个重要标准。<br />JSR-107规范即JCache API，JCache规范定义了一种对Java对象临时在内存中进行缓存的方法，包括对象的创建、共享访问、假脱机（spooling）、失效、各JVM的一致性等，可被用于缓存JSP内最经常读取的数据，如产品目录和价格列表。利用JCache的缓存数据，可以加快大多数查询的反应时间（内部测试表明反应时间大约快15倍）。
<a name="CpeO9"></a>
## 一、JCache
在Spring Boot中使用缓存之前，有必要了解一下`JCache`。`JCache`定义了五个核心接口，分别是`CachingProvider`，`CacheManager`，`Cache`，`Entry`和`Expiry`。

- 一个`CachingProvider`可以创建和管理多个`**CacheManager**`，并且一个`CacheManager`只能被一个`CachingProvider`所拥有，而一个应用可以在运行期间访问多个`CachingProvider`。
- 一个CacheManager可以创建和管理多个唯一命名的`**Cache**`，且一个Cache只能被一个`CacheManager`所拥有，这些Cache存在于`CacheManager`的上下文中。
- Cache是一个类似Map的数据结构
- Entry是一个存储在Cache中的key-value对
- Expiry是指存储在Cache中的Entry的有效期，一旦超过这个时间，Entry将处于过期状态，即不可访问、更新和删除。缓存有效期可以通过`ExpiryPolicy`设置。

![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1659867423590-50c7bb18-b18e-42cb-a11f-a16d6a05317e.jpeg)
<a name="B6mL5"></a>
## 二、Spring Cache原理
Spring 3.1开始，引入了Spring Cache，即Spring 缓存抽象。通过定义`org.springframework.cache.Cache`和`org.springframework.cache.CacheManager`接口来统一不同的缓存技术，并支持使用JCache注解简化开发过程。<br />Cache接口为缓存的组件规范定义，包含缓存的各种操作集合。Spring中为Cache接口提供了各种`xxxCache`的实现：`RedisCache`，`EhCacheCache`，`ConcurrentMapCache`等。<br />通过部分源码详细了解一下Cache接口和`CacheManager`接口。
<a name="dA4aP"></a>
### Cache接口
```java
public interface Cache {
    //Cache名称
    String getName();

    //Cache负责缓存的对象
    Object getNativeCache();

    /**
     * 获取key对应的ValueWrapper
     * 没有对应的key，则返回null
     * key对应的value是null，则返回null对应的ValueWrapper
     */
    @Nullable
    Cache.ValueWrapper get(Object key);

    //返回key对应type类型的value
    @Nullable
    <T> T get(Object key, @Nullable Class<T> type);

    //返回key对应的value，没有则缓存Callable::call，并返回
    @Nullable
    <T> T get(Object key, Callable<T> valueLoader);

    //缓存目标key-value（替换旧值），不保证实时性
    void put(Object key, @Nullable Object value);

    //插入缓存，并返回该key对应的value；先调用get，不存在则用put实现
    @Nullable
    default Cache.ValueWrapper putIfAbsent(Object key, @Nullable Object value) {
        Cache.ValueWrapper existingValue = this.get(key);
        if (existingValue == null) {
            this.put(key, value);
        }

        return existingValue;
    }

    //删除缓存，不保证实时性
    void evict(Object key);

    //立即删除缓存：返回false表示剔除前不存在制定key活不确定是否存在；返回true，表示该key之前存在
    default boolean evictIfPresent(Object key) {
        this.evict(key);
        return false;
    }

    //清除所有缓存，不保证实时性
    void clear();

    //立即清楚所有缓存，返回false表示清除前没有缓存或不能确定是否有；返回true表示清除前有缓存
    default boolean invalidate() {
        this.clear();
        return false;
    }

    public static class ValueRetrievalException extends RuntimeException {
        @Nullable
        private final Object key;

        public ValueRetrievalException(@Nullable Object key, Callable<?> loader, Throwable ex) {
            super(String.format("Value for key '%s' could not be loaded using '%s'", key, loader), ex);
            this.key = key;
        }

        @Nullable
        public Object getKey() {
            return this.key;
        }
    }

    //缓存值的一个包装器接口，实现类为SimpleValueWrapper
    @FunctionalInterface
    public interface ValueWrapper {
        @Nullable
        Object get();
    }
}
```
可以看出，Cache接口抽象了缓存的 get put evict 等相关操作。
<a name="O35Ob"></a>
### `AbstractValueAdaptingCache`
```java
public abstract class AbstractValueAdaptingCache implements Cache {

    //是否允许Null值
    private final boolean allowNullValues;

    protected AbstractValueAdaptingCache(boolean allowNullValues) {
        this.allowNullValues = allowNullValues;
    }

    public final boolean isAllowNullValues() {
        return this.allowNullValues;
    }

    @Nullable
    public ValueWrapper get(Object key) {
        return this.toValueWrapper(this.lookup(key));
    }

    @Nullable
    public <T> T get(Object key, @Nullable Class<T> type) {
        //查询到的缓存值做fromStoreValue转换
        Object value = this.fromStoreValue(this.lookup(key));
        //转换后非null值且无法转换为type类型则抛出异常
        if (value != null && type != null && !type.isInstance(value)) {
            throw new IllegalStateException("Cached value is not of required type [" + type.getName() + "]: " + value);
        } else {
            return value;
        }
    }

    //从缓存中获取key对应的value，子类实现
    @Nullable
    protected abstract Object lookup(Object key);

    //对于从缓存中获取的值，允许为空且值为NullValue时，处理为null
    @Nullable
    protected Object fromStoreValue(@Nullable Object storeValue) {
        return this.allowNullValues && storeValue == NullValue.INSTANCE ? null : storeValue;
    }

    //对于要插入缓存的null值，在允许null值的情况下处理为NullValue；否则抛出异常IllegalArgumentException
    protected Object toStoreValue(@Nullable Object userValue) {
        if (userValue == null) {
            if (this.allowNullValues) {
                return NullValue.INSTANCE;
            } else {
                throw new IllegalArgumentException("Cache '" + this.getName() + "' is configured to not allow null values but null was provided");
            }
        } else {
            return userValue;
        }
    }

    //get操作依据，查询到缓存值非null，则fromStoreValue转换后包装成SimpleValueWrapper返回
    @Nullable
    protected ValueWrapper toValueWrapper(@Nullable Object storeValue) {
        return storeValue != null ? new SimpleValueWrapper(this.fromStoreValue(storeValue)) : null;
    }
}
```
抽象类`AbstractValueAdaptingCache`实现了Cache接口，主要抽象了对NULL值的处理逻辑。

- `**allowNullValues**`属性表示是否允许处理NULL值的缓存
- `**fromStoreValue**`方法处理NULL值的`get`操作，在属性`allowNullValues`为true的情况下，将NullValue处理为NULL
- `**toStoreValue**`方法处理NULL值得`put`操作，在属性`allowNullValues`为true的情况下，将NULL处理为NullValue，否则抛出异常
- `**toValueWrapper**`方法提供`Cache`接口中`get`方法的默认实现，从缓存中读取值，再通过`fromStoreValue`转化，最后包装为`SimpleValueWrapper`返回
- `**ValueWrapper get(Object key)**`和`**T get(Object key, @Nullable Classtype)**`方法基于上述方法实现
- `**ValueWrapper get(Object key)**`和`**@Nullable Classtype)**`方法基于上述方法实现
- `**lookup**`抽象方法用于给子类获取真正的缓存值
<a name="ecllp"></a>
### `ConcurrentMapCache`
```java
public class ConcurrentMapCache extends AbstractValueAdaptingCache {
    private final String name;
    //定义ConcurrentMap缓存
    private final ConcurrentMap<Object, Object> store;
    //如果要缓存的是值对象的copy，则由此序列化代理类处理
    @Nullable
    private final SerializationDelegate serialization;

    public ConcurrentMapCache(String name) {
        this(name, new ConcurrentHashMap(256), true);
    }

    //默认允许处理null
    public ConcurrentMapCache(String name, boolean allowNullValues) {
        this(name, new ConcurrentHashMap(256), allowNullValues);
    }

    //默认serialization = null
    public ConcurrentMapCache(String name, ConcurrentMap<Object, Object> store, boolean allowNullValues) {
        this(name, store, allowNullValues, (SerializationDelegate)null);
    }

    protected ConcurrentMapCache(String name, ConcurrentMap<Object, Object> store, boolean allowNullValues, @Nullable SerializationDelegate serialization) {
        super(allowNullValues);
        Assert.notNull(name, "Name must not be null");
        Assert.notNull(store, "Store must not be null");
        this.name = name;
        this.store = store;
        this.serialization = serialization;
    }

    //serialization不为空，缓存值对象的copy
    public final boolean isStoreByValue() {
        return this.serialization != null;
    }

    public final String getName() {
        return this.name;
    }

    public final ConcurrentMap<Object, Object> getNativeCache() {
        return this.store;
    }

    //实现lookup：store#get
    @Nullable
    protected Object lookup(Object key) {
        return this.store.get(key);
    }

    //基于ConcurrentMap::computeIfAbsent方法实现；get和put的值由fromStoreValue和toStoreValue处理Null
    @Nullable
    public <T> T get(Object key, Callable<T> valueLoader) {
        return this.fromStoreValue(this.store.computeIfAbsent(key, (k) -> {
            try {
                return this.toStoreValue(valueLoader.call());
            } catch (Throwable var5) {
                throw new ValueRetrievalException(key, valueLoader, var5);
            }
        }));
    }

    public void put(Object key, @Nullable Object value) {
        this.store.put(key, this.toStoreValue(value));
    }

    @Nullable
    public ValueWrapper putIfAbsent(Object key, @Nullable Object value) {
        Object existing = this.store.putIfAbsent(key, this.toStoreValue(value));
        return this.toValueWrapper(existing);
    }

    public void evict(Object key) {
        this.store.remove(key);
    }

    public boolean evictIfPresent(Object key) {
        return this.store.remove(key) != null;
    }

    public void clear() {
        this.store.clear();
    }

    public boolean invalidate() {
        boolean notEmpty = !this.store.isEmpty();
        this.store.clear();
        return notEmpty;
    }

    protected Object toStoreValue(@Nullable Object userValue) {
        Object storeValue = super.toStoreValue(userValue);
        if (this.serialization != null) {
            try {
                return this.serialization.serializeToByteArray(storeValue);
            } catch (Throwable var4) {
                throw new IllegalArgumentException("Failed to serialize cache value '" + userValue + "'. Does it implement Serializable?", var4);
            }
        } else {
            return storeValue;
        }
    }

    protected Object fromStoreValue(@Nullable Object storeValue) {
        if (storeValue != null && this.serialization != null) {
            try {
                return super.fromStoreValue(this.serialization.deserializeFromByteArray((byte[])((byte[])storeValue)));
            } catch (Throwable var3) {
                throw new IllegalArgumentException("Failed to deserialize cache value '" + storeValue + "'", var3);
            }
        } else {
            return super.fromStoreValue(storeValue);
        }
    }
}
```
`ConcurrentMapCache`继承了抽象类`AbstractValueAdaptingCache`，是Spring的默认缓存实现。它支持对缓存对象copy的缓存，由SerializationDelegate serialization 处理序列化，默认为 null 即基于引用的缓存。缓存相关操作基于基类 `AbstractValueAdaptingCache` 的 null 值处理，默认允许为 null。
<a name="z7EzS"></a>
### `CacheManager`
```java
public interface CacheManager {
    @Nullable
    //获取指定name的Cache，可能延迟创建
    Cache getCache(String name);

    //获取当前CacheManager下的Cache name集合
    Collection<String> getCacheNames();
}
```
`CacheManager` 基于 name 管理一组 Cache。当然，`CacheManager`也有很多实现类，如`ConcurrentMapCacheManager`、`AbstractCacheManager`及`SimpleCacheManager`，这些`xxxCacheManager`类都是为了制定Cache的管理规则，这里就不再深入探讨了。
<a name="gkOew"></a>
## 三、Spring Cache实践
除了第二章中提到的Cache接口和`CacheManager`接口，在使用Spring 缓存抽象时，还会用到一些JCache注解。

| `Cache` | 缓存接口，定义缓存操作。实现有`RedisCache`、`EhCacheCache`、`ConcurrentMapCache`等 |
| --- | --- |
| `CacheManager` | 缓存管理，管理各种缓存（Cache）组件 |
| `@Cacheable` | 主要针对方法配置，能够根据方法的请求参数对其结果进行缓存 |
| `@CacheEvict` | 清空缓存 |
| `@CachePut` | 保证方法被调用，又希望结果被缓存。 |
| `@EnableCaching` | 开启基于注解的缓存 |
| `keyGenerator` | 缓存数据时key生成策略 |
| `serialize` | 缓存数据时Value序列化策略 |

`@Cacheable`、`@CacheEvict`和`@CachePut`三个注解都是对方法进行配置，主要参数如下图所示：

| `**@Cacheable**`**/**`**@CachePut**`**/**`**@CacheEvict**`** 主要的参数** |  |  |
| --- | --- | --- |
| value | 缓存的名称，在Spring配置文件中定义，必须指定至少一个 | 例如：<br />`@Cacheable(value = **"mycache"**)`或者`@Cacheable(value = {**"cache1"**, **"cache2"**})` |
| key | 缓存的key，可以为空，如果指定要按照SpEL表达式编写，如果不指定，则缺省按照方法的所有参数进行组合 | 例如：<br />`@Cacheable(value = **"testcache", key="#userName"**)` |
| condition | 缓存的条件，可以为空，使用SpEL编写，返回true或者false，只有为true才进行缓存/清除缓存，在调用方法之前之后都能判断 | 例如：<br />`@Cacheable(value = **"testcache", condition="#userName.length()>2"**)` |
| allEntries<br />**（**`**@CacheEvict**`**）** | 是否清空所有缓存内容，缺省为false，如果指定为true，则方法调用后将立即清空所有缓存 | 例如：<br />`@Cacheable(value = **"testcache", **allEntries=true)` |
| beforeInvocation**（**`**@CacheEvict**`**）** | 是否在方法执行前就全清空，缺省为false，如果指定为true，则在方法还没有执行的时候就清空缓存，缺省情况下，如果方法执行抛出，则不会清空缓存 | 例如：<br />`@CacheEvict(value = **"testcache"**, beforeInvocation = **true**)` |
| unless<br />**（**`**@CachePut**`**）**<br />**（**`**@Cacheable**`**）** | 用于否决缓存的，不像condition，该表达式只在方法执行之后判断，此时可以拿到返回值result进行判断，条件为true不会缓存，false才缓存 | 例如：<br />`@Cacheable(value = **"testcache"**, unless = **"#result==null"**)` |

Cache中可用的SpEL表达式如下图所示：

| **Cache SpEL available metadata** |  |  |  |
| --- | --- | --- | --- |
| **名字** | **位置** | **描述** | **示例** |
| methodName | root object | 当前被调用的方法名 | `#root.methodName` |
| method | root object | 当前被调用的方法 | `#root.method.name` |
| target | root object | 当前被调用的目标对象 | `#root.target` |
| targetClass | root object | 当前被调用的目标对象类 | `#root.targetClass` |
| args | root object | 当前被调用的参数列表 | `#root.args[0]` |
| caches | root object | 当前方法调用使用的缓存列表（如`@Cacheable(value = {**"cache1"**, **"cache2"**})`），则有两个Cache | `#root.caches[0]name` |
| argument name | evaluation context | 方法参数的名字，可以直接 `#参数名`，也可以使用 `#p0`或`#a0` 的形式，0代表参数的索引； | `#ban`、`#a0`、`#p0` |
| result | evaluation context | 方法执行后的返回值（仅当方法执行之后的判断有效，如"unless"，"cache put"的表达式"cache evict"的表达式`beforeInvocation = **true**`） | `#result` |

<a name="evxvb"></a>
### `@EnableCaching`
这个注解表示开启基于注解的缓存，一般放在主程序类前面，如下所示：
```java
@EnableCaching
public class SpringBootCacheApplication {
    public static void main(String[] args) {
        SpringApplication.run(SpringBootCacheApplication.class, args);
    }
}
```
<a name="AfdXQ"></a>
### `@Cacheable`
这个注解放在方法前，可以将方法的运行结果进行缓存，之后就不用调用方法了，直接从缓存中取值即可。常用属性可以见上图。以下是常见用法：
```java
@Cacheable(cacheNames = {"emp"}, key = "#id", conditon = "mid>0", unless = "#result == null")
public Employee getEmp(Integer id) {
    Employee emp = employeeMapper.getEmpById(id);
    return emp;
}
```
`@Cacheable`注解中常用的参数有cacheNames/value（指定缓存组件的名字，可以指定多个）、key（缓存数据时使用的key，默认使用方法参数的值，也可以自定义）、`keyGenerator`（key的生成器，可以自定义，key与`keyGenerator`二选一）、`cacheManager`（指定缓存管理器，或者使用cacheResolver指定获取解析器）、condition（符合条件才缓存）、unless（符合条件则不缓存，可以获取方法运行结果进行判断）、sync（是否使用异步模式，不可与unless一起使用）。`@Cacheable`的运行原理：

1. 方法运行前，程序会使用`cacheManager`根据cacheNames获取Cache，如果没有对应名称的Cache，则自动创建一个Cache。
2. 使用key去Cache中查找对应的缓存内容。key默认使用`SimpleKeyGenerator`生成，其生成策略如下：
- 如果没有参数，`key=new SimpleKey()`
- 如果只有一个参数，key=参数值
- 如果有多个参数，`key=new SimpleKey(params)`
1. 没有查到缓存值，则调用目标方法
2. 以步骤二中返回的key，目标方法返回的结果为value，存入缓存
<a name="tDYp8"></a>
### `@CachePut`
`@CachePut`注解先调用目标方法，然后再缓存目标方法的结果。
```java
@CachePut(value = "emp", key = "#result.id")
public Employee updateEmp(Employee employee) {
    employeeMapper.updateEmp(employee);
    return employee;
}
```
<a name="TQzGr"></a>
### `@CacheEvict`
`@CacheEvict`用于清除缓存，可以通过key指定需要清除的缓存，`allEntries`置为`true`表示删除所有缓存。
```java
@CacheEvict(value = "emp", key = "#id", allEntries = true)
public void deleteEmp(Integer id) {
    employeeMapper.deleteEmpById(id);
}
```
默认删除缓存行为在方法执行之后（`beforeInvocation=false`），如果方法运行异常，则该缓存不会被删除。但可以通过设置`beforeInvocation = true`，将删除缓存行为在方法执行之前。
<a name="UveWL"></a>
### `@Caching`&`@CacheConfig`
`@Caching`注解中包含了`@Cacheable`、`@CachePut`和`@CacheEvict`注解，可以同时指定多个缓存规则。示例如下所示：
```java
@Caching(
    cacheable = {
        @Cacheable(value = "emp", key = "#lastName")
    },
    put = {
        @CachePut(value = "emp", key = "#result.id")
        @CachePut(value = "emp", key = "#result.email")
    }
)
```
`@CacheConfig`注解放在类上，抽取缓存的公共配置，如`cacheNames`、`cacheManager`等，这样就不用在每个缓存注解中重复配置了。
<a name="lL09A"></a>
## 四、Redis测试
Spring Boot里面默认使用的`Cache`和`CacheManager`分别是`ConcurrentMapCache`和`ConcurrentMapCacheManager`，将数据存储在`ConcurrentMap`中。<br />然而，在实际开发过程中，一般会使用一些缓存中间件，如Redis、Memcached和Encache等。接下来，演示一下Redis环境搭建与测试。
<a name="CEp2R"></a>
### Redis环境搭建
Redis是一个开源的、内存中的数据结构存储系统，可以作为数据库、缓存和消息中间件。这里选择用Docker搭建Redis环境。首先需要下载镜像，然后启动，具体命令如下：
```bash
// 默认拉取最新的Redis镜像
docker pull redis
// 启动Redis容器
docker run -d -p 6379:6379 --name myredis redis
```
接下来，使用Redis Desktop Manager软件连接Redis，Redis的端口号默认为6379。<br />RedisAutoConfiguration.java文件里面定义了`StringRedisTemplate`（操作字符串）和`RedisTemplate`组件，将组件自动注入代码中，即可使用。两个组件都有针对Redis不同数据类型的处理方法。Redis常见的五大数据类型：<br />String（字符串）、List（列表）、Set（集合）、Hash（散列）、ZSet（有序集合）
```java
stringRedisTemplate.opsForValue()[字符串]
stringRedisTemplate.opsForList()[列表]
stringRedisTemplate.opsForSet()[集合]
stringRedisTemplate.opsForHash()[散列]
stringRedisTemplate.opsForZSet()[有序集合]
```
下面是使用示例：
```java
public class RedisTest {

    @Autowired
    StringRedisTemplate stringRedisTemplate;

    @Autowired
    RedisTemplate redisTemplate;

    // 测试保存数据
    public void test01 {
        stringRedisTemplate.opsForValue().append("msg","hello");//存入数据
        String s = stringRedisTemplate.opsForValue().get("msg");//获取数据
        stringRedisTemplate.opsForList().leftPush("mylist","1");
    }

    //测试保存对象
    public void test02 {
        Employee emp = new Employee();
        //默认使用jdk序列化机制，将对象序列化后的数据保存至Redis中
        redisTemplate.opsForValue().set("emp-01", empById);
    }
}
```
如果想将对象以json的方式保存，可将对象转换为json或改变`RedisTemplate`中的默认序列化规则。后者的参考代码如下，首先找到Redis的自动加载类`RedisAutoConfiguration`，自定义一个`RedisTemplate`，然后放入容器中。
```java
public class MyRedisConfig {

    @Bean
    public RedisTemplate<Object, Employee>      empRedisTemplate(RedisConnectionFactory redisConnectionFactory) throws UnknownHostException {
        RedisTemplate<Object, Employee> template = new RedisTemplate<Object, Employee>();
        template.setConnectionFactory(redisConnectionFactory);
        Jackson2JsonRedisSerializer<Employee> ser = new Jackson2JsonRedisSerializer<Employee>(Employee.class);
        template.setDefaultSerizlizer(ser);
        return template;
    }
}
```
<a name="HXMQg"></a>
### 自定义`CacheManager`
当Spring Boot项目中引入Redis的starter依赖时，会将`RedisCacheManager`作为默认的`CacheManager`。`RedisCacheManager`管理`RedisCache`，后者使用`RedisTemplate`操作Redis，默认序列化机制是jdk。如果需要改变Redis序列化机制，可以自定义`CacheManager`。参考代码如下：
```java
@Bean
public RedisCacheManager employeeCacheManager(RedisTemplate<Object,Object> employeeRedisTemplate) {
    RedisCacheManager cacheManager = new RedisCacheManager(employeeRedisTemplate);
    // 将cacheNames作为key的前缀
    cacheManager.setUserPrefix(true);
    return cacheManager;
}
```
自定义`RedisCache`和`CacheManager`都可以更改缓存方式，两者区别在于：前者用于自定义缓存层，后者将缓存交给spring管理（spring cache中不止Redis）。
