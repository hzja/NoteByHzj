SpringBoot Redis 集群
<a name="yiiOG"></a>
## 配置参数
这里假设已经有了一个 `Redis` 的集群环境，项目中需要调整以下几个部分

1. 修改配置参数，集群的节点和密码配置；
2. 确保引入的 `Jedis` 版本支持设置密码，`spring-data-redis` 1.8 以上，`SpringBoot` 1.5 以上才支持设置密码；
3. 注入 `RedisTemplate`；
4. 编写工具类；
<a name="KPf0T"></a>
## 修改配置参数
```yaml
############### Redis 集群配置 #########################
spring.custome.redis.cluster.nodes=172.20.0.1:7001,172.20.0.2:7002,172.20.0.3:7003
spring.custome.redis.cluster.max-redirects=3
spring.custome.redis.cluster.max-active=500
spring.custome.redis.cluster.max-wait=-1
spring.custome.redis.cluster.max-idle=500
spring.custome.redis.cluster.min-idle=20
spring.custome.redis.cluster.timeout=3000
spring.custome.redis.cluster.password=redis.cluster.password
```
<a name="wW6jc"></a>
## 引入依赖（如果需要）
确保 `SpringBoot` 的版本大于 1.4.x 如果不是的话，采用如下配置，先排除 `SpringBoot` 中旧版本 `Jedis` 和 `spring-data-redis`，再依赖高版本的 `Jedis` 和 `spring-data-redis`。
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
  <!-- 1.4 版本 SpringBoot 中 Jedis 不支持密码登录 -->
  <exclusions>
    <exclusion>
      <groupId>redis.clients</groupId>
      <artifactId>jedis</artifactId>
    </exclusion>
    <exclusion>
      <groupId>org.springframework.data</groupId>
      <artifactId>spring-data-redis</artifactId>
    </exclusion>
  </exclusions>
</dependency>
<!-- 手动依赖 Jedis 和 spring-data-redis-->
<dependency>
  <groupId>redis.clients</groupId>
  <artifactId>jedis</artifactId>
  <version>2.9.0</version>
</dependency>
<dependency>
  <groupId>org.springframework.data</groupId>
  <artifactId>spring-data-redis</artifactId>
  <version>1.8.0.RELEASE</version>
</dependency>
```
<a name="fdyHS"></a>
## 注入 `RedisTemplate`
注入 `RedisTemplate` 需要三个组件，分别是`JedisConnectionFactory` 、`RedisClusterConfiguration`、`JedisPoolConfig`，下面是注入`RedisTempalte` 的代码。先根据配置创建 `JedisConnectFactory` 同时需要配置 `RedisClusterConfiguration`、`JedisPoolConfig`，最后将`JedisConnectionFactory` 返回用于创建`RedisTemplate`
```java
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.commons.pool2.impl.GenericObjectPoolConfig;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Primary;
import org.springframework.data.redis.connection.RedisClusterConfiguration;
import org.springframework.data.redis.connection.RedisNode;
import org.springframework.data.redis.connection.jedis.JedisClientConfiguration;
import org.springframework.data.redis.connection.jedis.JedisConnectionFactory;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.serializer.Jackson2JsonRedisSerializer;
import org.springframework.data.redis.serializer.StringRedisSerializer;
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
public class RedisClusterConfig {
    @Bean(name = "redisTemplate")
    @Primary
    public RedisTemplate redisClusterTemplate(@Value("${spring.custome.redis.cluster.nodes}") String host,
                                     @Value("${spring.custome.redis.cluster.password}") String password,
                                     @Value("${spring.custome.redis.cluster.timeout}") long timeout,
                                     @Value("${spring.custome.redis.cluster.max-redirects}") int maxRedirect,
                                     @Value("${spring.custome.redis.cluster.max-active}") int maxActive,
                                     @Value("${spring.custome.redis.cluster.max-wait}") int maxWait,
                                     @Value("${spring.custome.redis.cluster.max-idle}") int maxIdle,
                                     @Value("${spring.custome.redis.cluster.min-idle}") int minIdle) {
        JedisConnectionFactory connectionFactory =  jedisClusterConnectionFactory(host, password,
                timeout, maxRedirect, maxActive, maxWait, maxIdle, minIdle);
        return createRedisClusterTemplate(connectionFactory);
    }
    private JedisConnectionFactory jedisClusterConnectionFactory(String host, String password,
                                                                   long timeout, int maxRedirect, int maxActive, int maxWait, int maxIdle, int minIdle) {
        RedisClusterConfiguration redisClusterConfiguration = new RedisClusterConfiguration();
        List<RedisNode> nodeList = new ArrayList<>();
        String[] cNodes = host.split(",");
        //分割出集群节点
        for (String node : cNodes) {
            String[] hp = node.split(":");
            nodeList.add(new RedisNode(hp[0], Integer.parseInt(hp[1])));
        }
        redisClusterConfiguration.setClusterNodes(nodeList);
        redisClusterConfiguration.setPassword(password);
        redisClusterConfiguration.setMaxRedirects(maxRedirect);
        // 连接池通用配置
        GenericObjectPoolConfig genericObjectPoolConfig = new GenericObjectPoolConfig();
        genericObjectPoolConfig.setMaxIdle(maxIdle);
        genericObjectPoolConfig.setMaxTotal(maxActive);
        genericObjectPoolConfig.setMinIdle(minIdle);
        genericObjectPoolConfig.setMaxWaitMillis(maxWait);
        genericObjectPoolConfig.setTestWhileIdle(true);
        genericObjectPoolConfig.setTimeBetweenEvictionRunsMillis(300000);
        JedisClientConfiguration.DefaultJedisClientConfigurationBuilder builder = (JedisClientConfiguration.DefaultJedisClientConfigurationBuilder) JedisClientConfiguration
                .builder();
        builder.connectTimeout(Duration.ofSeconds(timeout));
        builder.usePooling();
        builder.poolConfig(genericObjectPoolConfig);
        JedisConnectionFactory connectionFactory = new JedisConnectionFactory(redisClusterConfiguration, builder.build());
        // 连接池初始化
        connectionFactory.afterPropertiesSet();
        return connectionFactory;
    }
    private RedisTemplate createRedisClusterTemplate(JedisConnectionFactory redisConnectionFactory) {
        RedisTemplate<String, Object> redisTemplate = new RedisTemplate<>();
        redisTemplate.setConnectionFactory(redisConnectionFactory);
        Jackson2JsonRedisSerializer<Object> jackson2JsonRedisSerializer = new Jackson2JsonRedisSerializer<>(Object.class);
        ObjectMapper om = new ObjectMapper();
        om.setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.ANY);
        om.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
        jackson2JsonRedisSerializer.setObjectMapper(om);
        StringRedisSerializer stringRedisSerializer = new StringRedisSerializer();
        // key采用String的序列化方式
        redisTemplate.setKeySerializer(stringRedisSerializer);
        // hash的key也采用String的序列化方式
        redisTemplate.setHashKeySerializer(stringRedisSerializer);
        // value序列化方式采用jackson
        redisTemplate.setValueSerializer(jackson2JsonRedisSerializer);
        // hash的value序列化方式采用jackson
        redisTemplate.setHashValueSerializer(jackson2JsonRedisSerializer);
        redisTemplate.afterPropertiesSet();
        return redisTemplate;
    }
}
```
> 这里一定要注意 Jedis 的 Spring-data-redis 的版本支持设置密码，毕竟生产环境是一定要配置密码的。

<a name="ZCehy"></a>
## 编写工具类
其实到这里基本上已经完成了，可以看到 `SpringBoot` 项目接入 `Redis` 集群还是比较简单的，而且如果之前单机环境就是采用`RedisTemplate` 的话，现在也就不需要编写工具类，之前的操作依旧有效。工具类代码太长，只贴部分。
```java
	/**
     *  删除KEY
     * @param key
     * @return
     */
    public boolean delete(String key) {
        try {
            return getTemplate().delete(key);
        } catch (Exception e) {
            log.error("redis hasKey() is error");
            return false;
        }
    }
    /**
     * 普通缓存获取
     *
     * @param key 键
     * @return 值
     */
    public Object get(String key) {
        return key == null ? null : getTemplate().opsForValue().get(key);
    }
    /**
     * 普通缓存放入
     *
     * @param key   键
     * @param value 值
     * @return true成功 false失败
     */
    public boolean set(String key, Object value) {
        try {
            getTemplate().opsForValue().set(key, value);
            return true;
        } catch (Exception e) {
            log.error("redis set() is error");
            return false;
        }
    }
    /**
     * 普通缓存放入并设置时间
     *
     * @param key   键
     * @param value 值
     * @param time  时间(秒) time要大于0 如果time小于等于0 将设置无限期
     * @return true成功 false 失败
     */
    public boolean set(String key, Object value, long time) {
        try {
            if (time > 0) {
                getTemplate().opsForValue().set(key, value, time, TimeUnit.SECONDS);
            } else {
                set(key, value);
            }
            return true;
        } catch (Exception e) {
            log.error("redis set() is error");
            return false;
        }
    }
    /**
     * 计数器
     *
     * @param key 键
     * @return 值
     */
    public Long incr(String key) {
        return getTemplate().opsForValue().increment(key);
    }
    public Long incrBy(String key, long step) {
        return getTemplate().opsForValue().increment(key, step);
    }
    /**
     * HashGet
     *
     * @param key  键 不能为null
     * @param item 项 不能为null
     * @return 值
     */
    public Object hget(String key, String item) {
        return getTemplate().opsForHash().get(key, item);
    }
    /**
     * 获取hashKey对应的所有键值
     *
     * @param key 键
     * @return 对应的多个键值
     */
    public Map<Object, Object> hmget(String key) {
        return getTemplate().opsForHash().entries(key);
    }
    /**
     * 获取hashKey对应的批量键值
     * @param key
     * @param values
     * @return
     */
    public List<Object> hmget(String key, List<String> values) {
        return getTemplate().opsForHash().multiGet(key, values);
    }
```
