JavaSpringBootRedis
<a name="C1S5s"></a>
## 前言
在开发分布式高并发系统时有三把利器用来保护系统：缓存、降级、限流。
<a name="WqvVy"></a>
### 缓存
缓存的目的是提升系统访问速度和增大系统处理容量
<a name="YAUZf"></a>
### 降级
降级是当服务出现问题或者影响到核心流程时，需要暂时屏蔽掉，待高峰或者问题解决后再打开
<a name="IFsJj"></a>
### 限流
限流的目的是通过对并发访问/请求进行限速，或者对一个时间窗口内的请求进行限速来保护系统，一旦达到限制速率则可以拒绝服务、排队或等待、降级等处理<br />本文主要讲的是api接口限流相关内容，虽然不是论述高并发概念中的限流， 不过道理都差不多。通过限流可以让系统维持在一个相对稳定的状态，为更多的客户提供服务。<br />api接口的限流主要应用场景有：

- 电商系统（特别是6.18、双11等）中的秒杀活动，使用限流防止使用软件恶意刷单；
- 各种基础api接口限流：例如天气信息获取，IP对应城市接口，百度、腾讯等对外提供的基础接口，都是通过限流来实现免费与付费直接的转换。
- 被各种系统广泛调用的api接口，严重消耗网络、内存等资源，需要合理限流。
<a name="Bus6o"></a>
## API限流实战
<a name="nZwnw"></a>
### 一、SpringBoot中集成Redis
SpringBoot中集成Redis相对比较简单，步骤如下：
<a name="OuMpM"></a>
#### 1.1 引入Redis依赖
```xml
<!--springboot redis依赖-->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```
<a name="d60YT"></a>
#### 1.2 在application.yml中配置Redis
```yaml
spring:
  redis:
    database: 3 # Redis数据库索引（默认为0）
    host: 127.0.0.1 # Redis服务器地址
    port: 6379 # Redis服务器连接端口
    password: 123456 # Redis服务器连接密码（默认为空）
    timeout: 2000  # 连接超时时间（毫秒）
    jedis:
      pool:
        max-active: 200         # 连接池最大连接数（使用负值表示没有限制）
        max-idle: 20         # 连接池中的最大空闲连接
        min-idle: 0         # 连接池中的最小空闲连接
        max-wait: -1       # 连接池最大阻塞等待时间（使用负值表示没有限制）
```
<a name="bIm21"></a>
#### 1.3 配置RedisTemplate
```java
/**
 * @Description: redis配置类
 */
@Configuration
@EnableCaching
public class RedisConfig extends CachingConfigurerSupport {
 
    /**
     * RedisTemplate相关配置
     * 使redis支持插入对象
     *
     * @param factory
     * @return 方法缓存 Methods the cache
     */
    @Bean
    public RedisTemplate<String, Object> redisTemplate(RedisConnectionFactory factory) {
        RedisTemplate<String, Object> template = new RedisTemplate<>();
        // 配置连接工厂
        template.setConnectionFactory(factory);
        // 设置key的序列化器
        template.setKeySerializer(new StringRedisSerializer());
        // 设置value的序列化器
        //使用Jackson 2，将对象序列化为JSON
        Jackson2JsonRedisSerializer jackson2JsonRedisSerializer = new Jackson2JsonRedisSerializer(Object.class);
        //json转对象类，不设置默认的会将json转成hashmap
        ObjectMapper om = new ObjectMapper();
        om.setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.ANY);
        om.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
        jackson2JsonRedisSerializer.setObjectMapper(om);
        template.setValueSerializer(jackson2JsonRedisSerializer);
        return template;
    }
}
```
以上，已经完成Redis的集成，后续使用可以直接注入`RedisTemplate`，如下所示：
```java
@Autowired
private RedisTemplate<String, Object> redisTemplate;
```
<a name="pc462"></a>
### 二、实现限流
<a name="rVin5"></a>
#### 2.1 添加自定义`AccessLimit`注解
使用注解方式实现接口的限流操作，方便而优雅。
```java
/**
 * @Description:
 */
@Inherited
@Documented
@Target({ElementType.FIELD, ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
public @interface AccessLimit {
 
    /**
     * 指定second 时间内 API请求次数
     */
    int maxCount() default 5;
 
    /**
     * 请求次数的指定时间范围  秒数(redis数据过期时间)
     */
    int second() default 60;
}
```
<a name="dVK4a"></a>
#### 2.2 编写拦截器
限流的思路

- 通过路径：ip的作为key，访问次数为value的方式对某一用户的某一请求进行唯一标识
- 每次访问的时候判断key是否存在，是否count超过了限制的访问次数
- 若访问超出限制，则应response返回msg：请求过于频繁给前端予以展示
```java
/**
 * @Description: 访问拦截器
 */
@Component
public class AccessLimitInterceptor implements HandlerInterceptor {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());
 
    @Autowired
    private RedisTemplate<String, Object> redisTemplate;
 
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        try {// Handler 是否为 HandlerMethod 实例
            if (handler instanceof HandlerMethod) {
                // 强转
                HandlerMethod handlerMethod = (HandlerMethod) handler;
                // 获取方法
                Method method = handlerMethod.getMethod();
                // 是否有AccessLimit注解
                if (!method.isAnnotationPresent(AccessLimit.class)) {
                    return true;
                }
                // 获取注解内容信息
                AccessLimit accessLimit = method.getAnnotation(AccessLimit.class);
                if (accessLimit == null) {
                    return true;
                }
                int seconds = accessLimit.second();
                int maxCount = accessLimit.maxCount();
 
                // 存储key
                String key = request.getRemoteAddr() + ":" + request.getContextPath() + ":" + request.getServletPath();
 
                // 已经访问的次数
                Integer count = (Integer) redisTemplate.opsForValue().get(key);
                System.out.println("已经访问的次数:" + count);
                if (null == count || -1 == count) {
                    redisTemplate.opsForValue().set(key, 1, seconds, TimeUnit.SECONDS);
                    return true;
                }
 
                if (count < maxCount) {
                    redisTemplate.opsForValue().increment(key);
                    return true;
                }
 
                if (count >= maxCount) {
                    logger.warn("请求过于频繁请稍后再试");
                    return false;
                }
            }
            return true;
        } catch (Exception e) {
            logger.warn("请求过于频繁请稍后再试");
            e.printStackTrace();
        }
        return true;
    }
}
```
<a name="CaJ1c"></a>
#### 2.3 注册拦截器并配置拦截路径和不拦截路径
```java
/**
 * @Description: 访问拦截器配置
 */
@Configuration
public class IntercepterConfig  implements WebMvcConfigurer {
 
    @Autowired
    private AccessLimitInterceptor accessLimitInterceptor;
 
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(accessLimitInterceptor)
                .addPathPatterns("/**").excludePathPatterns("/static/**","/login.html","/user/login");
    }
}
```
<a name="TzTqS"></a>
#### 2.4 使用AccessLimit
```java
/**
 * @Description:
 */
@RestController
@RequestMapping("access")
public class AccessLimitController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());
 
    /**
     * 限流测试
     */
    @GetMapping
    @AccessLimit(maxCount = 3,second = 60)
    public String limit(HttpServletRequest request) {
        logger.error("Access Limit Test");
        return "限流测试";
    }
 
}
```
<a name="oyq4Y"></a>
#### 2.5 测试
![2022-11-16-09-38-14.203329000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668562720626-ee07b6fa-56b6-4c7c-a787-4afd44c27b88.png#averageHue=%23323232&clientId=u0650cec0-0c05-4&from=ui&id=ue98333f8&originHeight=243&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=788919&status=done&style=none&taskId=u16c9fbf2-9299-4fab-8abe-6dca3bd38c5&title=)
