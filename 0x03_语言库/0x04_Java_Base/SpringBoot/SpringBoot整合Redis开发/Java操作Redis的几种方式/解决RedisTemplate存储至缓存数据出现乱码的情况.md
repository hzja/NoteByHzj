Java SpringBoot Redis<br />`RedisTemplate`是Spring对于Redis的封装。`RedisTemplate`中定义了对5种数据结构操作。
```java
redisTemplate.opsForList();//操作list
redisTemplate.opsForValue();//操作字符串
redisTemplate.opsForCluster();//集群时使用
redisTemplate.opsForGeo();//地理位置时使用
redisTemplate.opsForHash();//操作hash
redisTemplate.opsForSet();//操作set
redisTemplate.opsForZSet();//操作有序set
```
<a name="YA87u"></a>
## 与`StringRedisTemplate`的区别
`StringRedisTemplate`继承`RedisTemplate`。<br />它们采用的序列化策略不同:<br />* `StringRedisTemplate`默认采用的是String的序列化策略，保存的key和value都是采用此策略序列化保存的。<br />* `RedisTemplate`默认采用的是JDK的序列化策略，保存的key和value都是采用此策略序列化保存的。<br />`RedisTemplate`和`StringRedisTemplate`它们存取的数据是相互独立的。
<a name="T9cQy"></a>
## 解决办法
上文已经提及，在动手的过程中，采用的是RedisTemplate，在传递String类型的数据结构后，查看缓存会发现数据乱码现象。这时候需要修改`RedisTemplate`的序列化策略。
```java
RedisSerializer<String> stringSerializer = new StringRedisSerializer();
      redisTemplate.setKeySerializer(stringSerializer);
      redisTemplate.setValueSerializer(stringSerializer);
      redisTemplate.setHashKeySerializer(stringSerializer);
      redisTemplate.setHashValueSerializer(stringSerializer);
```
但是注意一点，由于采用了String的序列化策略，所以只接受value值类型为String的参数。<br />如果传递了Integer类型的参数，直接使用`toString()`方法存入缓存。
```java
ops.set("stock", redPacket.getStock().toString(),TIME_OUT, TimeUnit.SECONDS);
```
这样就解决了乱码问题。
<a name="fkQvB"></a>
## 附：SpringBoot启动实例化配置
```java
@Configuration
public class RedisConfigurtion {
	@Autowired
	private RedisTemplate redisTemplate;
	@Bean
	public RedisTemplate<String, Object> stringSerializerRedisTemplate() {
		RedisSerializer<String> stringSerializer = new StringRedisSerializer();
		redisTemplate.setKeySerializer(stringSerializer);
		redisTemplate.setValueSerializer(stringSerializer);
		redisTemplate.setHashKeySerializer(stringSerializer);
		redisTemplate.setHashValueSerializer(stringSerializer);
		return redisTemplate;
	}
}
```
