Java
<a name="oWQf7"></a>
## 前提
最近在做有需求在做分布式限流，调研的限流框架大概有<br />1、spring cloud gateway集成redis限流，但属于网关层限流<br />2、阿里Sentinel，功能强大、带监控平台<br />3、srping cloud hystrix，属于接口层限流，提供线程池与信号量两种方式<br />4、其他：Redission、手撸代码<br />实际需求情况属于业务端限流，Redission更加方便，使用更加灵活，下面介绍下Redission分布式限流如何使用及原理：
<a name="ksoks"></a>
## 一、使用
使用很简单、如下
```java
// 1、 声明一个限流器
RRateLimiter rateLimiter = redissonClient.getRateLimiter(key);
 
// 2、 设置速率，5秒中产生3个令牌
rateLimiter.trySetRate(RateType.OVERALL, 3, 5, RateIntervalUnit.SECONDS);
 
// 3、试图获取一个令牌，获取到返回true
rateLimiter.tryAcquire(1)
```
<a name="lLkbk"></a>
## 二、原理
<a name="TROaY"></a>
### **1、**`**getRateLimiter**`
```java
// 声明一个限流器 名称 叫key
redissonClient.getRateLimiter(key)
```
<a name="HuJMp"></a>
### **2、**`**trySetRate**`
`trySetRate`方法跟进去底层实现如下：
```java
@Override
public RFuture<Boolean> trySetRateAsync(RateType type, long rate, long rateInterval, RateIntervalUnit unit) {
	return commandExecutor.evalWriteAsync(getName(), LongCodec.INSTANCE, RedisCommands.EVAL_BOOLEAN,
										  "redis.call('hsetnx', KEYS[1], 'rate', ARGV[1]);"
										  + "redis.call('hsetnx', KEYS[1], 'interval', ARGV[2]);"
										  + "return redis.call('hsetnx', KEYS[1], 'type', ARGV[3]);",
										  Collections.<Object>singletonList(getName()), rate, unit.toMillis(rateInterval), type.ordinal());
}
```
举个例子，更容易理解：<br />比如下面这段代码，5秒中产生3个令牌，并且所有实例共享（`RateType.OVERALL`所有实例共享、`RateType.CLIENT`单实例端共享）
```java
trySetRate(RateType.OVERALL, 3, 5, RateIntervalUnit.SECONDS);
```
那么redis中就会设置3个参数：
```java
hsetnx,key,rate,3

hsetnx,key,interval,5

hsetnx,key,type,0
```
接着看`tryAcquire(1)`方法：底层源码如下
```java
private <T> RFuture<T> tryAcquireAsync(RedisCommand<T> command, Long value) {
    return commandExecutor.evalWriteAsync(getName(), LongCodec.INSTANCE, command,
            "local rate = redis.call('hget', KEYS[1], 'rate');"  //1
          + "local interval = redis.call('hget', KEYS[1], 'interval');"  //2
          + "local type = redis.call('hget', KEYS[1], 'type');" //3
          + "assert(rate ~= false and interval ~= false and type ~= false, 'RateLimiter is not initialized')" //4
          
          + "local valueName = KEYS[2];" //5
          + "if type == 1 then "
              + "valueName = KEYS[3];" //6
          + "end;"
          
          + "local currentValue = redis.call('get', valueName); " //7
          + "if currentValue ~= false then " 
                 + "if tonumber(currentValue) < tonumber(ARGV[1]) then " //8
                     + "return redis.call('pttl', valueName); "
                 + "else "
                     + "redis.call('decrby', valueName, ARGV[1]); " //9
                     + "return nil; "
                 + "end; "
          + "else " //10
                 + "redis.call('set', valueName, rate, 'px', interval); " 
                 + "redis.call('decrby', valueName, ARGV[1]); "
                 + "return nil; "
          + "end;",
            Arrays.<Object>asList(getName(), getValueName(), getClientValueName()), 
            value, commandExecutor.getConnectionManager().getId().toString());
}
```
第1、2、3备注行是获取上一步set的3个值：rate、interval、type，如果这3个值没有设置，直接返回rateLimiter没有被初始化。<br />第5备注行声明一个变量叫valueName 值为KEYS[2],KEYS[2]对应的值是`getValueName()`方法，`getValueName()`返回的就是上面第一步`getRateLimiter`设置的key；如果type=1，表示全局共享，那么valueName 的值改为取KEYS[3]，KEYS[3]对应的值为`getClientValueName()`，查看`getClientValueName()`源码：
```java
String getClientValueName() {
	return suffixName(getValueName(), commandExecutor.getConnectionManager().getId().toString());
}
```
`ConnectionManager().getId()`如下：
```java
public interface ConnectionManager {
	
	UUID getId();
	
	// 省略...
}
```
这个`getId()`是每个客户端初始化的时候生成的UUID，即每个客户端的getId是唯一的，这也就验证了`trySetRate`方法中`RateType.ALL`与`RateType.PER_CLIENT`的作用。

- **接着看第7标准行，获取valueName对应的值currentValue；首次获取肯定为空，那么看第10标准行else的逻辑**
- `**set valueName 3 px 5**`**，设置key=valueName value=3 过期时间为5秒**
- `**decrby valueName 1**`**，将上面valueName的值减1**
- **那么如果第二次访问，第7标注行返回的值存在，将会走第8标注行，紧接着走如下判断**
- **如果当前valueName的值也就是3，小于要获得的令牌数量(**`**tryAcquire**`**方法中的入参)，那么说明当前时间内（key的有效期5秒内），令牌的数量已经被用完，返回pttl(key的剩余过期时间)；反之说明桶中有足够的令牌，获取之后将会把桶中的令牌数量减1，至此结束。**
<a name="txjCS"></a>
## 总结
Redission分布式限流采用令牌桶思想和固定时间窗口，`trySetRate`方法设置桶的大小，利用redis key过期机制达到时间窗口目的，控制固定时间窗口内允许通过的请求量。
