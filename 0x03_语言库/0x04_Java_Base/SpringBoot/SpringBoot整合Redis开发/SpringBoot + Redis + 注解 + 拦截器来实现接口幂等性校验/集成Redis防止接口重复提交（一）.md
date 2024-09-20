<a name="F4dyl"></a>
## 一、摘要
**对于下单流量不算高的系统，可以通过请求唯一ID+数据表增加唯一索引约束这种方案来实现防止接口重复提交**！<br />随着业务的快速增长，每一秒的下单请求次数，可能从几十上升到几百甚至几千。<br />面对这种下单流量越来越高的场景，此时数据库的访问压力会急剧上升，上面这套方案全靠数据库来解决，会特别吃力！<br />对于这样的场景，可以选择引入**缓存中间件**来解决，可选的组件有 redis、memcache 等。<br />下面，以引入redis缓存数据库服务器，介绍具体的解决方案！
<a name="p5q3W"></a>
## 二、方案实践
先来看一张图，这张图就是本次方案的核心流程图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668475083139-ca3b2485-9c26-4fd0-a65a-f5924a83c27c.png#averageHue=%23f8f7f7&clientId=u8bcfba0a-35a4-4&from=paste&id=uea409db9&originHeight=248&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua4fd1cbd-820d-432a-9771-561a2f02d2f&title=)<br />实现的逻辑，流程如下：

1. 当用户进入订单提交界面的时候，调用后端获取请求唯一 ID，同时后端将请求唯一ID存储到redis中再返回给前端，前端将唯一 ID 值埋点在页面里面
2. 当用户点击提交按钮时，后端检查这个请求唯一 ID 是否存在，如果不存在，提示错误信息；如果存在，继续后续检查流程
3. 使用redis的分布式锁服务，对请求 ID 在限定的时间内进行加锁，如果加锁成功，继续后续流程；如果加锁失败，说明服务正在处理，请勿重复提交
4. 最后一步，如果加锁成功后，需要将锁手动释放掉，以免再次请求时，提示同样的信息；同时如果任务执行成功，需要将redis中的请求唯一 ID 清理掉
5. 至于数据库是否需要增加字段唯一索引，理论上可以不用加，如果加了更保险

引入缓存服务，防止重复提交的大体思路如上，实践代码如下！
<a name="qaU09"></a>
#### 2.1、引入 redis 组件
项目是基于SpringBoot版本进行构建，添加相关的redis依赖环境如下：
```xml
<!-- 引入springboot -->
<parent>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-parent</artifactId>
  <version>2.1.0.RELEASE</version>
</parent>

......

<!-- Redis相关依赖包，采用jedis作为客户端 -->
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
特别注意：**由于每个项目环境不一样，具体的依赖包需要和工程版本号匹配**！
<a name="Dh877"></a>
#### 2.2、添加 redis 环境配置
在全局配置application.properties文件中，添加redis相关服务配置如下
```
# Redis数据库索引（默认为0）
spring.redis.database=1
# Redis服务器地址
spring.redis.host=127.0.0.1
# Redis服务器连接端口
spring.redis.port=6379
# Redis服务器连接密码（默认为空）
spring.redis.password=
# Redis服务器连接超时配置
spring.redis.timeout=1000

# 连接池配置
spring.redis.jedis.pool.max-active=8
spring.redis.jedis.pool.max-wait=1000
spring.redis.jedis.pool.max-idle=8
spring.redis.jedis.pool.min-idle=0
spring.redis.jedis.pool.time-between-eviction-runs=100
```
在使用redis之前，请确保redis服务器是启动状态，并且能正常访问！
<a name="tgr93"></a>
#### 2.3、编写获取请求唯一ID的接口，同时将唯一ID存入redis
```java
@RestController
@RequestMapping("api")
public class SubmitTokenController {

    /**
     * SubmitToken过期时间
     */
    private static final Integer EXPIRE_TIME = 60;

    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    /**
     * 获取getSubmitToken
     * @return
     */
    @RequestMapping("getSubmitToken")
    public ResResult getSubmitToken(){
        String uuid = UUID.randomUUID().toString();
        //存入redis
        stringRedisTemplate.opsForValue().set(uuid, uuid, EXPIRE_TIME, TimeUnit.SECONDS);
        return ResResult.getSuccess(uuid);
    }
}
```
<a name="C5x6I"></a>
#### 2.4、编写服务验证逻辑，通过 aop 代理方式实现
首先创建一个`@SubmitToken`注解，通过这个注解来进行方法代理拦截！
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD})
@Documented
public @interface SubmitToken {

}
```
编写方法代理服务，增加防止重复提交的验证，实现了逻辑如下！
```java
@Order(1)
@Aspect
@Component
public class SubmitTokenAspect {

    private static final Logger LOGGER = LoggerFactory.getLogger(SubmitTokenAspect.class);

    /**
     * 获取分布式锁等待时间，单位秒
     */
    private static final Long LOCK_REDIS_WAIT_TIME = 3L;

    /**
     * 分布式锁前缀
     */
    private static final String LOCK_KEY_PREFIX = "SUBMIT:TOKEN:LOCK";

    /**
     * 默认锁对应的值
     */
    private static final String DEFAULT_LOCK_VALUE = "DEFAULT_LOCK_VALUE";


    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    @Autowired
    private RedisLockService redisLockService;

    /**
     * 方法调用环绕拦截
     */
    @Around(value = "@annotation(com.example.submittoken.config.annotation.SubmitToken)")
    public Object doAround(ProceedingJoinPoint joinPoint){
        HttpServletRequest request = getHttpServletRequest();
        if(Objects.isNull(request)){
            return ResResult.getSysError("请求参数不能为空！");
        }
        String submitToken = request.getHeader("submitToken");
        if(StringUtils.isEmpty(submitToken)){
            return ResResult.getSysError("submitToken不能为空！");
        }
        //检查submitToken是否存在
        String submitTokenValue = stringRedisTemplate.opsForValue().get(submitToken);
        if(StringUtils.isEmpty(submitTokenValue)){
            return ResResult.getSysError(ResResultEnum.SUBMIT_ERROR_MESSAGE);
        }
        //尝试加锁
        String lockKey = LOCK_KEY_PREFIX + submitToken;
        boolean lock = redisLockService.tryLock(lockKey, DEFAULT_LOCK_VALUE, Duration.ofSeconds(LOCK_REDIS_WAIT_TIME));
        if(!lock){
            return ResResult.getSysError("服务正在处理，请勿重复提交！");
        }
        try {
            //继续执行后续流程
            Object result = joinPoint.proceed();
            //任务执行成功，清除submitToken缓存
            stringRedisTemplate.delete(submitToken);
            return result;
        } catch (CommonException e) {
            return ResResult.getSysError(e.getMessage());
        } catch (Throwable e) {
            LOGGER.error("业务处理发生异常，错误信息：",e);
            return ResResult.getSysError(ResResultEnum.DEFAULT_ERROR_MESSAGE);
        } finally {
            //执行完毕之后，手动将锁释放
            redisLockService.releaseLock(lockKey, DEFAULT_LOCK_VALUE);
        }
    }

    /**
     * 获取请求对象
     * @return
     */
    private HttpServletRequest getHttpServletRequest(){
        RequestAttributes ra = RequestContextHolder.getRequestAttributes();
        ServletRequestAttributes sra = (ServletRequestAttributes)ra;
        HttpServletRequest request = sra.getRequest();
        return request;
    }
}
```
部分校验逻辑用到了redis分布式锁，具体实现逻辑如下：
```java
/**
 * redis分布式锁服务类
 * 采用LUA脚本实现，保证加锁、解锁操作原子性
 *
 */
@Component
public class RedisLockService {

    /**
     * 分布式锁过期时间，单位秒
     */
    private static final Long DEFAULT_LOCK_EXPIRE_TIME = 60L;

    @Autowired
    private StringRedisTemplate stringRedisTemplate;

    /**
     * 尝试在指定时间内加锁
     * @param key
     * @param value
     * @param timeout 锁等待时间
     * @return
     */
    public boolean tryLock(String key,String value, Duration timeout){
        long waitMills = timeout.toMillis();
        long currentTimeMillis = System.currentTimeMillis();
        do {
            boolean lock = lock(key, value, DEFAULT_LOCK_EXPIRE_TIME);
            if (lock) {
                return true;
            }
            try {
                Thread.sleep(1L);
            } catch (InterruptedException e) {
                Thread.interrupted();
            }
        } while (System.currentTimeMillis() < currentTimeMillis + waitMills);
        return false;
    }

    /**
     * 直接加锁
     * @param key
     * @param value
     * @param expire
     * @return
     */
    public boolean lock(String key,String value, Long expire){
        String luaScript = "if redis.call('setnx', KEYS[1], ARGV[1]) == 1 then return redis.call('expire', KEYS[1], ARGV[2]) else return 0 end";
        RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);
        Long result = stringRedisTemplate.execute(redisScript, Collections.singletonList(key), value, String.valueOf(expire));
        return result.equals(Long.valueOf(1));
    }


    /**
     * 释放锁
     * @param key
     * @param value
     * @return
     */
    public boolean releaseLock(String key,String value){
        String luaScript = "if redis.call('get', KEYS[1]) == ARGV[1] then return redis.call('del', KEYS[1]) else return 0 end";
        RedisScript<Long> redisScript = new DefaultRedisScript<>(luaScript, Long.class);
        Long result = stringRedisTemplate.execute(redisScript, Collections.singletonList(key),value);
        return result.equals(Long.valueOf(1));
    }
}
```
<a name="KcUwe"></a>
#### 2.5、在相关的业务接口上，增加`@SubmitToken`注解即可
```java
@RestController
@RequestMapping("order")
public class OrderController {

    @Autowired
    private OrderService orderService;

    /**
     * 下单
     * @param request
     * @return
     */
    @SubmitToken
    @PostMapping(value = "confirm")
    public ResResult confirm(@RequestBody OrderConfirmRequest request){
        //调用订单下单相关逻辑
        orderService.confirm(request);
        return ResResult.getSuccess();
    }
}
```
**整套方案完全基于redis来实现，同时结合redis的分布式锁来实现请求限流，之所以选择redis，是因为它是一个内存数据库，性能比关系型数据库强太多，即使每秒的下单请求量在几千，也能很好的应对，为关系型数据库起到降压作用**！<br />**特别注意的地方：使用redis的分布式锁，推荐单机环境，如果redis是集群环境，可能会导致锁短暂无效**！
<a name="ns3vZ"></a>
## 三、小结
随着下单流量逐渐上升，通过查询数据库来检查当前服务请求是否重复提交这种方式，可能会让数据库的请求查询频率变得非常高，数据库的压力会倍增。<br />此时可以引入redis缓存，将通过查询数据库来检查当前请求是否重复提交这种方式，转移到通过查询缓存来检查当前请求是否重复提交，可以很好的给数据库降压！
