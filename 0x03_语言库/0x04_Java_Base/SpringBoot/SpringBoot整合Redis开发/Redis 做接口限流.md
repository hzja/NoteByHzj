JavaSpringBootRedis<br />Redis 除了做缓存，还能干很多很多事情：分布式锁、限流、处理请求接口幂等性。。。
<a name="MgdyX"></a>
## 1、准备工作
首先创建一个 Spring Boot 工程，引入 Web 和 Redis 依赖，同时考虑到接口限流一般是通过注解来标记，而注解是通过 AOP 来解析的，所以还需要加上 AOP 的依赖，最终的依赖如下：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-aop</artifactId>
</dependency>
```
然后提前准备好一个 Redis 实例，这里项目配置好之后，直接配置一下 Redis 的基本信息即可，如下：
```
spring.redis.host=localhost
spring.redis.port=6379
spring.redis.password=123
```
准备工作就算是到位了。
<a name="pwMZ9"></a>
## 2、限流注解
接下来创建一个限流注解，将限流分为两种情况：

1. 针对当前接口的全局性限流，例如该接口可以在 1 分钟内访问 100 次。
2. 针对某一个 IP 地址的限流，例如某个 IP 地址可以在 1 分钟内访问 100 次。

针对这两种情况，创建一个枚举类：
```java
public enum LimitType {
    /**
     * 默认策略全局限流
     */
    DEFAULT,
    /**
     * 根据请求者IP进行限流
     */
    IP
}
```
接下来创建限流注解：
```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface RateLimiter {
    /**
     * 限流key
     */
    String key() default "rate_limit:";

    /**
     * 限流时间,单位秒
     */
    int time() default 60;

    /**
     * 限流次数
     */
    int count() default 100;

    /**
     * 限流类型
     */
    LimitType limitType() default LimitType.DEFAULT;
}
```
第一个参数限流的 key，这个仅仅是一个前缀，将来完整的 key 是这个前缀再加上接口方法的完整路径，共同组成限流 key，这个 key 将被存入到 Redis 中。<br />另外三个参数好理解，就不多说了。<br />好了，将来哪个接口需要限流，就在哪个接口上添加 `@RateLimiter` 注解，然后配置相关参数即可。
<a name="pk326"></a>
## 3、定制 RedisTemplate
在 Spring Boot 中，其实更习惯使用 Spring Data Redis 来操作 Redis，不过默认的 RedisTemplate 有一个小坑，就是序列化用的是 JdkSerializationRedisSerializer，不知道小伙伴们有没有注意过，直接用这个序列化工具将来存到 Redis 上的 key 和 value 都会莫名其妙多一些前缀，这就导致用命令读取的时候可能会出错。<br />例如存储的时候，key 是 name，value 是 javaboy，但是当在命令行操作的时候，get name 却获取不到想要的数据，原因就是存到 redis 之后 name 前面多了一些字符，此时只能继续使用 RedisTemplate 将之读取出来。<br />用 Redis 做限流会用到 Lua 脚本，使用 Lua 脚本的时候，就会出现上面说的这种情况，所以需要修改 RedisTemplate 的序列化方案。<br />可能有小伙伴会说为什么不用 StringRedisTemplate 呢？StringRedisTemplate 确实不存在上面所说的问题，但是它能够存储的数据类型不够丰富，所以这里不考虑。<br />修改 RedisTemplate 序列化方案，代码如下：
```java
@Configuration
public class RedisConfig {
    
    @Bean
    public RedisTemplate<Object, Object> redisTemplate(RedisConnectionFactory connectionFactory) {
        RedisTemplate<Object, Object> redisTemplate = new RedisTemplate<>();
        redisTemplate.setConnectionFactory(connectionFactory);
        // 使用Jackson2JsonRedisSerialize 替换默认序列化(默认采用的是JDK序列化)
        Jackson2JsonRedisSerializer<Object> jackson2JsonRedisSerializer = new Jackson2JsonRedisSerializer<>(Object.class);
        ObjectMapper om = new ObjectMapper();
        om.setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.ANY);
        om.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
        jackson2JsonRedisSerializer.setObjectMapper(om);
        redisTemplate.setKeySerializer(jackson2JsonRedisSerializer);
        redisTemplate.setValueSerializer(jackson2JsonRedisSerializer);
        redisTemplate.setHashKeySerializer(jackson2JsonRedisSerializer);
        redisTemplate.setHashValueSerializer(jackson2JsonRedisSerializer);
        return redisTemplate;
    }
}
```
这个也没啥好说的，key 和 value 都使用 Spring Boot 中默认的 jackson 序列化方式来解决。
<a name="g8Bmy"></a>
## 4、开发 Lua 脚本
Redis 中的一些原子操作可以借助 Lua 脚本来实现，想要调用 Lua 脚本，有两种不同的思路：

1. 在 Redis 服务端定义好 Lua 脚本，然后计算出来一个散列值，在 Java 代码中，通过这个散列值锁定要执行哪个 Lua 脚本。
2. 直接在 Java 代码中将 Lua 脚本定义好，然后发送到 Redis 服务端去执行。

Spring Data Redis 中也提供了操作 Lua 脚本的接口，还是比较方便的，所以这里就采用第二种方案。<br />在 resources 目录下新建 lua 文件夹专门用来存放 lua 脚本，脚本内容如下：
```lua
local key = KEYS[1]
local count = tonumber(ARGV[1])
local time = tonumber(ARGV[2])
local current = redis.call('get', key)
if current and tonumber(current) > count then
  return tonumber(current)
end
current = redis.call('incr', key)
if tonumber(current) == 1 then
  redis.call('expire', key, time)
end
return tonumber(current)
```
这个脚本其实不难，大概瞅一眼就知道干啥用的。KEYS 和 ARGV 都是一会调用时候传进来的参数，tonumber 就是把字符串转为数字，redis.call 就是执行具体的 redis 指令，具体流程是这样：

1. 首先获取到传进来的 key 以及 限流的 count 和时间 time。
2. 通过 get 获取到这个 key 对应的值，这个值就是当前时间窗内这个接口可以访问多少次。
3. 如果是第一次访问，此时拿到的结果为 nil，否则拿到的结果应该是一个数字，所以接下来就判断，如果拿到的结果是一个数字，并且这个数字还大于 count，那就说明已经超过流量限制了，那么直接返回查询的结果即可。
4. 如果拿到的结果为 nil，说明是第一次访问，此时就给当前 key 自增 1，然后设置一个过期时间。
5. 最后把自增 1 后的值返回就可以了。

其实这段 Lua 脚本很好理解。<br />接下来在一个 Bean 中来加载这段 Lua 脚本，如下：
```java
@Bean
public DefaultRedisScript<Long> limitScript() {
    DefaultRedisScript<Long> redisScript = new DefaultRedisScript<>();
    redisScript.setScriptSource(new ResourceScriptSource(new ClassPathResource("lua/limit.lua")));
    redisScript.setResultType(Long.class);
    return redisScript;
}
```
Lua 脚本现在就准备好了。
<a name="zJVjH"></a>
## 5、注解解析
接下来就需要自定义切面，来解析这个注解了，来看看切面的定义：
```java
@Aspect
@Component
public class RateLimiterAspect {
    private static final Logger log = LoggerFactory.getLogger(RateLimiterAspect.class);

    @Autowired
    private RedisTemplate<Object, Object> redisTemplate;

    @Autowired
    private RedisScript<Long> limitScript;

    @Before("@annotation(rateLimiter)")
    public void doBefore(JoinPoint point, RateLimiter rateLimiter) throws Throwable {
        String key = rateLimiter.key();
        int time = rateLimiter.time();
        int count = rateLimiter.count();

        String combineKey = getCombineKey(rateLimiter, point);
        List<Object> keys = Collections.singletonList(combineKey);
        try {
            Long number = redisTemplate.execute(limitScript, keys, count, time);
            if (number==null || number.intValue() > count) {
                throw new ServiceException("访问过于频繁，请稍候再试");
            }
            log.info("限制请求'{}',当前请求'{}',缓存key'{}'", count, number.intValue(), key);
        } catch (ServiceException e) {
            throw e;
        } catch (Exception e) {
            throw new RuntimeException("服务器限流异常，请稍候再试");
        }
    }

    public String getCombineKey(RateLimiter rateLimiter, JoinPoint point) {
        StringBuffer stringBuffer = new StringBuffer(rateLimiter.key());
        if (rateLimiter.limitType() == LimitType.IP) {
            stringBuffer.append(IpUtils.getIpAddr(((ServletRequestAttributes) RequestContextHolder.currentRequestAttributes()).getRequest())).append("-");
        }
        MethodSignature signature = (MethodSignature) point.getSignature();
        Method method = signature.getMethod();
        Class<?> targetClass = method.getDeclaringClass();
        stringBuffer.append(targetClass.getName()).append("-").append(method.getName());
        return stringBuffer.toString();
    }
}
```
这个切面就是拦截所有加了 `@RateLimiter` 注解的方法，在前置通知中对注解进行处理。

1. 首先获取到注解中的 key、time 以及 count 三个参数。
2. 获取一个组合的 key，所谓的组合的 key，就是在注解的 key 属性基础上，再加上方法的完整路径，如果是 IP 模式的话，就再加上 IP 地址。以 IP 模式为例，最终生成的 key 类似这样：rate_limit:127.0.0.1-org.javaboy.ratelimiter.controller.HelloController-hello（如果不是 IP 模式，那么生成的 key 中就不包含 IP 地址）。
3. 将生成的 key 放到集合中。
4. 通过 redisTemplate.execute 方法取执行一个 Lua 脚本，第一个参数是脚本所封装的对象，第二个参数是 key，对应了脚本中的 KEYS，后面是可变长度的参数，对应了脚本中的 ARGV。
5. 将 Lua 脚本执行的结果与 count 进行比较，如果大于 count，就说明过载了，抛异常就行了。

好了，大功告成了。
<a name="VtW1p"></a>
## 6、接口测试
接下来就进行接口的一个简单测试，如下：
```java
@RestController
public class HelloController {
    @GetMapping("/hello")
    @RateLimiter(time = 5,count = 3,limitType = LimitType.IP)
    public String hello() {
        return "hello>>>"+new Date();
    }
}
```
每一个 IP 地址，在 5 秒内只能访问 3 次。<br />这个自己手动刷新浏览器都能测试出来。
<a name="M6UAS"></a>
## 7、全局异常处理
由于过载的时候是抛异常出来，所以还需要一个全局异常处理器，如下：
```java
@RestControllerAdvice
public class GlobalException {
    @ExceptionHandler(ServiceException.class)
    public Map<String,Object> serviceException(ServiceException e) {
        HashMap<String, Object> map = new HashMap<>();
        map.put("status", 500);
        map.put("message", e.getMessage());
        return map;
    }
}
```
这是一个小 demo，就不去定义实体类了，直接用 Map 来返回 JSON 了。<br />最后看看过载时的测试效果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652848004831-3566bb93-7192-4aa3-bf8d-64f3ff59581e.png#clientId=uf7985e50-2057-4&from=paste&id=u52201420&originHeight=228&originWidth=694&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u978b6a19-2c80-4d45-8fd2-02a076bea43&title=)<br />这就是使用 Redis 做限流的方式。
