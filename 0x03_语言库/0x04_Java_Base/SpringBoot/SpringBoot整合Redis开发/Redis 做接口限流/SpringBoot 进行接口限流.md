JavaSpringBoot
<a name="MntnY"></a>
## 1、序言
联网系统通常都要面对大并发大流量的请求，在突发情况下（最常见的场景就是秒杀、抢购），瞬时大流量会直接将系统打垮，无法对外提供服务。为了防止出现这种情况最常见的解决方案之一就是限流，当请求达到一定的并发数或速率，就进行等待、排队、降级、拒绝服务等。<br />限流是对某一时间窗口内的请求数进行限制，保持系统的可用性和稳定性，防止因流量暴增而导致的系统运行缓慢或宕机。
<a name="QirAP"></a>
### 常见的限流算法有三种：
<a name="Q1lxb"></a>
#### 1）计数器限流
计数器限流算法是最为简单粗暴的解决方案，主要用来限制总并发数，比如数据库连接池大小、线程池大小、接口访问并发数等都是使用计数器算法。<br />如：使用 AomicInteger 来进行统计当前正在并发执行的次数，如果超过域值就直接拒绝请求，提示系统繁忙。
<a name="ZztRt"></a>
#### 2）漏桶算法
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690865198592-0fd056f3-7f67-4a5c-b3bc-f8f127da236b.png#averageHue=%23d0d0d0&clientId=u4c15041c-332d-4&from=paste&id=u89b60941&originHeight=656&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc99c5546-6ff5-4ab5-b4e8-637c686db96&title=)<br />漏桶算法思路很简单，把水比作是请求，漏桶比作是系统处理能力极限，水先进入到漏桶里，漏桶里的水按一定速率流出，当流出的速率小于流入的速率时，由于漏桶容量有限，后续进入的水直接溢出（拒绝请求），以此实现限流。
<a name="kAzCL"></a>
#### 3）令牌桶算法
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690865198536-52a7f9bf-dd2f-42fc-a2f1-2d86b366d9d0.png#averageHue=%23d2d1cf&clientId=u4c15041c-332d-4&from=paste&id=u8a474963&originHeight=627&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaac8bbda-828a-4c31-bc03-65350cc34c5&title=)<br />令牌桶算法的原理也比较简单，可以理解成医院的挂号看病，只有拿到号以后才可以进行诊病。<br />系统会维护一个令牌（token）桶，以一个恒定的速度往桶里放入令牌（token），这时如果有请求进来想要被处理，则需要先从桶里获取一个令牌（token），当桶里没有令牌（token）可取时，则该请求将被拒绝服务。令牌桶算法通过控制桶的容量、发放令牌的速率，来达到对请求的限制。
<a name="gMpeQ"></a>
## 2、单机版
对于单机版，直接使用Guava即可。<br />Google开源工具包Guava提供了限流工具类`RateLimiter`，该类基于令牌桶算法实现流量限制，使用十分方便，而且十分高效，实现步骤如下：
<a name="WZ4gw"></a>
### 第一步：引入guava依赖包
```xml
<dependency>
  <groupid>com.google.guava</groupid>
  <artifactid>guava</artifactid>
  <version>30.1-jre</version>
</dependency>
```
<a name="LkSDX"></a>
### 第二步：给接口加上限流逻辑
```java
@Slf4j
@RestController
@RequestMapping("/limit")
public class LimitController {
    /**
     * 限流策略 ：1秒钟2个请求
     */
    private final RateLimiter limiter = RateLimiter.create(2.0);

    private DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

    @GetMapping("/test1")
    public String testLimiter() {
        //500毫秒内，没拿到令牌，就直接进入服务降级
        boolean tryAcquire = limiter.tryAcquire(500, TimeUnit.MILLISECONDS);

        if (!tryAcquire) {
            log.warn("进入服务降级，时间{}", LocalDateTime.now().format(dtf));
            return "当前排队人数较多，请稍后再试！";
        }

        log.info("获取令牌成功，时间{}", LocalDateTime.now().format(dtf));
        return "请求成功";
    }
}
```
<a name="wpAUs"></a>
### 第三步：体验效果
通过访问测试地址：http://127.0.0.1:8080/limit/test1，反复刷新并观察后端日志<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690865198603-d205d8df-3368-4158-b594-56383370ac5a.png#averageHue=%232c2c2c&clientId=u4c15041c-332d-4&from=paste&id=u10bd3a50&originHeight=312&originWidth=695&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u21c4f365-ceca-4f20-a654-31d4e5d52ff&title=)<br />初体验后，就可以对其进行解耦了，对于这样的限流，全部写在controller中是冗余的，可以使用注解方式来优化代码：
<a name="AgwYT"></a>
### ①加入AOP依赖
```xml
<dependency>
  <groupid>org.springframework.boot</groupid>
  <artifactid>spring-boot-starter-aop</artifactid>
</dependency>
```
<a name="sD2VA"></a>
### ②自定义限流注解
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD})
@Documented
public @interface Limit {
    /**
     * 资源的key,唯一
     * 作用：不同的接口，不同的流量控制
     */
    String key() default "";

    /**
     * 最多的访问限制次数
     */
    double permitsPerSecond () ;

    /**
     * 获取令牌最大等待时间
     */
    long timeout();

    /**
     * 获取令牌最大等待时间,单位(例:分钟/秒/毫秒) 默认:毫秒
     */
    TimeUnit timeunit() default TimeUnit.MILLISECONDS;

    /**
     * 得不到令牌的提示语
     */
    String msg() default "系统繁忙,请稍后再试.";
}
```
<a name="FMRpO"></a>
### ③使用AOP切面拦截限流注解
```java
@Slf4j
@Aspect
@Component
public class LimitAop {
    /**
     * 不同的接口，不同的流量控制
     * map的key为 Limiter.key
     */
    private final Map<string, ratelimiter=""> limitMap = Maps.newConcurrentMap();

    @Around("@annotation(com.zxh.limit.Limit)")
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable{
        MethodSignature signature = (MethodSignature) joinPoint.getSignature();
        Method method = signature.getMethod();
        //拿limit的注解
        Limit limit = method.getAnnotation(Limit.class);
        if (limit != null) {
            //key作用：不同的接口，不同的流量控制
            String key=limit.key();
            RateLimiter rateLimiter = null;
            //验证缓存是否有命中key
            if (!limitMap.containsKey(key)) {
                // 创建令牌桶
                rateLimiter = RateLimiter.create(limit.permitsPerSecond());
                limitMap.put(key, rateLimiter);
                log.info("新建了令牌桶={}，容量={}",key,limit.permitsPerSecond());
            }
            rateLimiter = limitMap.get(key);
            // 拿令牌
            boolean acquire = rateLimiter.tryAcquire(limit.timeout(), limit.timeunit());
            // 拿不到命令，直接返回异常提示
            if (!acquire) {
                log.debug("令牌桶={}，获取令牌失败",key);
                this.responseFail(limit.msg());
                return null;
            }
        }
        return joinPoint.proceed();
    }

    /**
     * 直接向前端抛出异常
     * @param msg 提示信息
     */
    private void responseFail(String msg)  {
        HttpServletResponse response=((ServletRequestAttributes) RequestContextHolder.getRequestAttributes()).getResponse();
        response.setCharacterEncoding("UTF-8");
        response.setHeader("Content-type", "text/html;charset=UTF-8");
        PrintWriter pr=response.getWriter();
        pr.write(msg);
     }
}
```
<a name="KSQCG"></a>
### ④给需要限流的接口加上注解
```java
@Slf4j
@RestController
@RequestMapping("/limit")
public class LimitController {

    @GetMapping("/test2")
    @Limit(key = "limit2", permitsPerSecond = 1, timeout = 500, timeunit = TimeUnit.MILLISECONDS,msg = "当前排队人数较多，请稍后再试！")
    public String limit2() {
        log.info("令牌桶limit2获取令牌成功");
        return "ok";
    }


    @GetMapping("/test3")
    @Limit(key = "limit3", permitsPerSecond = 2, timeout = 500, timeunit = TimeUnit.MILLISECONDS,msg = "系统繁忙，请稍后再试！")
    public String limit3() {
        log.info("令牌桶limit3获取令牌成功");
        return "ok";
    }
}
```
<a name="SxuLh"></a>
### ⑤体验效果
通过访问测试地址：http://127.0.0.1:8080/limit/test2，反复刷新并观察输出结果<br />正常响应是返回"ok"，触发限流时显示"系统繁忙，请稍后再试！"
<a name="cv79w"></a>
## 3、分布式版
上述单机版并不能解决分布式的问题，那么分布式限流可以采用 Redis + Lua 实现。
<a name="sUPTP"></a>
### 第一步：引入Redis依赖包
```xml
<dependency>  
  <groupId>org.springframework.boot</groupId>  
  <artifactId>spring-boot-starter-data-redis</artifactId>  
</dependency>
```
<a name="hErOK"></a>
### 第二步：配置Redis
```java
@Configuration
public class RedisConfig {

    @Bean
    public RedisTemplate<String, Object> redisTemplate(RedisConnectionFactory factory) {
        RedisTemplate<String, Object> template = new RedisTemplate<>();
        template.setConnectionFactory(factory);

        // 使用Jackson2JsonRedisSerializer来序列化和反序列化redis的value值（默认使用JDK的序列化方式）
        Jackson2JsonRedisSerializer<Object> serializer = new Jackson2JsonRedisSerializer<>(Object.class);
        ObjectMapper mapper = new ObjectMapper();
        mapper.setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.ANY);
        mapper.activateDefaultTyping(LaissezFaireSubTypeValidator.instance,
                ObjectMapper.DefaultTyping.NON_FINAL, JsonTypeInfo.As.PROPERTY);
        serializer.setObjectMapper(mapper);

        template.setValueSerializer(serializer);
        // 使用StringRedisSerializer来序列化和反序列化redis的key值
        template.setKeySerializer(new StringRedisSerializer());
        template.afterPropertiesSet();

        return template;
    }

}
```
<a name="l56Wh"></a>
### 第三步：自定义限流注解
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD})
@Documented
public @interface RedisLimit {
    /**
     * 资源的key,唯一
     * 作用：不同的接口，不同的流量控制
     */
    String key() default "";

    /**
     * 最多的访问限制次数
     */
    long permitsPerSecond() default 2;

    /**
     * 过期时间也可以理解为单位时间，单位秒，默认60
     */
    long expire() default 60;


    /**
     * 得不到令牌的提示语
     */
    String msg() default "系统繁忙,请稍后再试.";
}
```
<a name="cvsBM"></a>
### 第四步：创建限流异常
代码中并未体现异常拦截的代码，开发时根据时间情况进行拦截并处理即可
```java
public class RedisLimitException extends RuntimeException{
    public RedisLimitException(String msg) {
        super( msg );
    }
}
```
<a name="rtk4r"></a>
### 第五步：在resouces文件夹下创建lua文件 rateLimiter.lua
```lua
--获取KEY
local key = KEYS[1]

local limit = tonumber(ARGV[1])

local curentLimit = tonumber(redis.call('get', key) or "0")

if curentLimit + 1 > limit
  then return 0
else
  -- 自增长 1
  redis.call('INCRBY', key, 1)
  -- 设置过期时间
  redis.call('EXPIRE', key, ARGV[2])
  return curentLimit + 1
end
```
<a name="W5gUO"></a>
### 第六步：使用AOP切面拦截限流注解
```java
@Slf4j
@Aspect
@Component
public class RedisLimitAop {

    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    private DefaultRedisScript<Long> redisScript; 

    @PostConstruct
    public void init(){
        redisScript = new DefaultRedisScript<>();
        redisScript.setResultType(Long.class);
        redisScript.setScriptSource(new ResourceScriptSource(new ClassPathResource("rateLimiter.lua")));
    }
    @Pointcut("@annotation(com.zxh.limit.redis.RedisLimit)")
    private void check() {

    }

    @Before("check()")
    public void before(JoinPoint joinPoint) {
        MethodSignature signature = (MethodSignature) joinPoint.getSignature();
        Method method = signature.getMethod();

        //拿到RedisLimit注解，如果存在则说明需要限流
        RedisLimit redisLimit = method.getAnnotation(RedisLimit.class);

        if(redisLimit != null){
            //获取redis的key
            String key  = redisLimit.key();
            String className = method.getDeclaringClass().getName();
            String name = method.getName();

            String limitKey = key + className + method.getName();

            log.info(limitKey);

            if(StringUtils.isEmpty(key)){
                throw new RedisLimitException( "key cannot be null" );
            }

            long limit = redisLimit.permitsPerSecond();

            long expire = redisLimit.expire();

            List<String> keys = new ArrayList<>();
            keys.add( key );
            String luaScript = buildLuaScript();
            RedisScript<Long> redisScript = new DefaultRedisScript<>( luaScript, Long.class );

            Long count = stringRedisTemplate.execute( redisScript, keys, String.valueOf(limit), String.valueOf(expire) );

            log.info( "Access try count is {} for key={}", count, key );

            if (count != null && count == 0) {
                log.debug("令牌桶={}，获取令牌失败",key);
                throw new RedisLimitException(redisLimit.msg());
            }
        }

    }
}
```
<a name="tkQuP"></a>
### 第七步：给需要限流的接口加上注解
```java
@Slf4j
@RestController
@RequestMapping("/limit/redis")
public class LimitRedisController {

    /**
     * 基于Redis AOP限流
     */
    @GetMapping("/test")
    @RedisLimit(key = "redis-limit:test", permitsPerSecond = 2, expire = 1, msg = "当前排队人数较多，请稍后再试！")
    public String test() {
        log.info("限流成功。。。");
        return "ok";
    }

}
```
<a name="eLSQF"></a>
### 第八步：体验效果
通过访问测试地址：http://127.0.0.1:8080/limit/redis/test，反复刷新并观察输出结果。<br />正常响应是返回"ok"，触发限流时显示"当前排队人数较多，请稍后再试！"（上述代码未体现异常拦截处理，这里会出现500的错误）
<a name="RTv72"></a>
## 4、整合版
当然，每个项目都加入这个代码也是有些繁杂的，为了可以方便的进行切换两种场景，可以将其封装为starter，然后按需引入即可。
<a name="TZELq"></a>
### 第1步、创建通用模块cloud-limiter-starter
首先在父项目下创建一个模块<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690865198600-69521d43-10d5-496e-9d31-43d05ef4c3d8.png#averageHue=%23f1f3f8&clientId=u4c15041c-332d-4&from=paste&id=ucd611b3b&originHeight=634&originWidth=854&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u005127eb-3aa9-49f1-952a-62c3164be7d&title=)<br />然后在pom文件中引入相关依赖
```xml
<dependencies>
  <dependency>
    <groupId>com.google.guava</groupId>
    <artifactId>guava</artifactId>
  </dependency>
  <!--SpringFramework-->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <scope>provided</scope>
  </dependency>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
    <scope>provided</scope>
  </dependency>
  <dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <scope>provided</scope>
  </dependency>
  <dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-aspects</artifactId>
    <scope>provided</scope>
  </dependency>

</dependencies>
```
<a name="VJ3yr"></a>
### 第2步、实现限流功能
①创建限流接口<br />既然有两种限流机制，按照套路肯定得先创建一个限流接口，就叫LimiterManager吧。
```java
public interface LimiterManager {
    boolean tryAccess(Limiter limiter);
}
```
②分别实现Redis的限流功能和Guava的限流功能，这里只给出核心代码。<br />Guava限流的核心实现GuavaLimiter
```java
@Slf4j
public class GuavaLimiter implements LimiterManager{
    private final Map<String, RateLimiter> limiterMap = Maps.newConcurrentMap();

    @Override
    public boolean tryAccess(Limiter limiter) {
        RateLimiter rateLimiter = getRateLimiter(limiter);
        if (rateLimiter == null) {
            return false;
        }

        boolean access = rateLimiter.tryAcquire(1,100, TimeUnit.MILLISECONDS);

        log.info("{} access :{}",limiter.getKey() , access);

        return access;
    }
}
```
Redis限流的核心实现`RedisLimiter`
```java
@Slf4j
public class RedisLimiter implements LimiterManager{

    private final StringRedisTemplate stringRedisTemplate;

    public RedisLimiter(StringRedisTemplate stringRedisTemplate) {
        this.stringRedisTemplate = stringRedisTemplate;
    }

    @Override
    public boolean tryAccess(Limiter limiter) {

        String key = limiter.getKey();
        if (StringUtils.isEmpty(key)) {
            throw new LimiterException( "redis limiter key cannot be null" );
        }

        List<String> keys = new ArrayList<>();
        keys.add( key );

        int seconds = limiter.getSeconds();
        int limitCount = limiter.getLimitNum();

        String luaScript = buildLuaScript();

        RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);

        Long count = stringRedisTemplate.execute( redisScript, keys, "" + limitCount, "" + seconds );

        log.info( "Access try count is {} for key={}", count, key );

        return count != null && count != 0;
    }
}
```
<a name="n1bmM"></a>
### 第3步、创建配置类
编写配置类根据配置文件注入限流实现类，当配置文件中属性 `limit.type=local` 时启用Guava限流机制，当`limit.type=redis` 时启用Redis限流机制。
```java
@Configuration
public class LimiterConfigure {

    @Bean
    @ConditionalOnProperty(name = "limit.type",havingValue = "local")
    public LimiterManager guavaLimiter(){
        return new GuavaLimiter();
    }


    @Bean
    @ConditionalOnProperty(name = "limit.type",havingValue = "redis")
    public LimiterManager redisLimiter(StringRedisTemplate stringRedisTemplate){
        return new RedisLimiter(stringRedisTemplate);
    }
}
```
<a name="wh8le"></a>
### 第4步、创建AOP
创建一个AOP进行切面拦截
```java
@Aspect
@EnableAspectJAutoProxy(proxyTargetClass = true) //使用CGLIB代理
@Conditional(LimitAspectCondition.class)
public class LimitAspect {

    @Setter(onMethod_ = @Autowired)
    private LimiterManager limiterManager;

    @Pointcut("@annotation(com.zxh.limit.aop.Limit)")
    private void check() {

    }

    @Before("check()")
    public void before(JoinPoint joinPoint){
        MethodSignature signature = (MethodSignature) joinPoint.getSignature();
        Method method = signature.getMethod();

        Limit limit = method.getAnnotation(Limit.class);
        if(limit != null){

            Limiter limiter = Limiter.builder().limitNum(limit.limitNum())
                    .seconds(limit.seconds())
                    .key(limit.key()).build();

            if(!limiterManager.tryAccess(limiter)){
                throw new LimiterException( "There are currently many people , please try again later!" );
            }
        }
    }
}
```
这里使用自定义条件选择器
```java
public class LimitAspectCondition implements Condition {
    @Override
    public boolean matches(ConditionContext conditionContext, AnnotatedTypeMetadata annotatedTypeMetadata) {
        //检查配置文件是否包含limit.type属性
        return conditionContext.getEnvironment().containsProperty(ConfigConstant.LIMIT_TYPE);
    }
}
```
<a name="ogegI"></a>
### 第5步、创建spring.factories文件，引导SpringBoot加载配置类
```
## AutoConfiguration
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
  com.zxh.limit.config.LimiterConfigure,\
  com.zxh.limit.aop.LimitAspect
```
完整目录结构如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690865198561-50c12618-075a-434b-b29e-be960966c76a.png#averageHue=%23f3f5f9&clientId=u4c15041c-332d-4&from=paste&id=ufaa57405&originHeight=1244&originWidth=718&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua505f1db-169b-4213-a9a5-e781e69a31a&title=)
<a name="a19ZY"></a>
### 第6步、在项目中引用限流组件
①引入依赖
```xml
<dependency>
  <groupId>com.zxh</groupId>
  <artifactId>cloud-limit-starter</artifactId>
</dependency>
```
②在application.properties中设置加载的限流组件
```
limit.type = redis
```
如果不配置此属性则不加载对应限流功能。<br />③在需要限流的接口上加上注解
```java
@Limit(key = "Limiter:test",limitNum = 3,seconds = 1)
```
对于SpringBoot自定义的starter是非常有用的，毕竟写代码的思想就是减少冗余，提高效率，便于维护。
