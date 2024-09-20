Redis 发布订阅<br />假设有这么一个业务场景，在网站下单支付以后，需要通知库存服务进行发货处理。<br />上面业务实现不难，只要让库存服务提供给相关的接口，下单支付之后只要调用库存服务即可。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603985380486-0671bd95-074c-4290-8975-834c1acd8e3c.webp#align=left&display=inline&height=81&originHeight=81&originWidth=411&size=0&status=done&style=shadow&width=411)<br />后面如果又有新的业务，比如说积分服务，他需要获取下单支付的结果，然后增加用户的积分。<br />这个实现也不难，让积分服务同样提供一个接口，下单支付之后只要调用库存服务即可。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603985380441-93a522a7-c20c-4aad-a3ae-f569cd885390.webp#align=left&display=inline&height=331&originHeight=331&originWidth=421&size=0&status=done&style=shadow&width=421)<br />如果就两个业务需要获取下单支付的结果，那也还好，程序改造也快。可是随着业务不断的发展，越来越多的新业务说是要下单支付的结果。<br />这时会发现上面这样的系统架构存在很多问题：<br />第一，下单支付业务与其他业务重度耦合，每当有个新业务需要支付结果，就需要改动下单支付的业务。<br />第二，如果调用业务过多，会导致下单支付接口响应时间变长。另外，如果有任一下游接口响应变慢，就会同步导致下单支付接口响应也变长。<br />第三，如果任一下游接口失败，可能导致数据不一致的情况。比如说下图，先调用 A，成功之后再调用 B，最后再调用 C。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603985380474-414050ba-4169-43f7-90be-034ca1349814.webp#align=left&display=inline&height=371&originHeight=371&originWidth=471&size=0&status=done&style=shadow&width=471)<br />如果在调用 B 接口的发生异常，此时可能就导致下单支付接口返回失败，但是此时 A 接口其实已经调用成功，这就代表它内部已经处理下单支付成功的结果。<br />这样就会导致 A，B，C 三个下游接口，A 获取成功获取支付结果，但是 B，C 没有拿到，导致三者系统数据不一致的情况。<br />其实仔细想一下，对于下单支付业务来讲，它其实不需要关心下游调用结果，只要有某种机制通知能通知到他们就可以了。<br />讲到这里，这就需要引入今天需要介绍发布订阅机制。
<a name="DRfkg"></a>
## Redis 发布与订阅
Redis 提供了基于「发布/订阅」模式的消息机制，在这种模式下，消息发布者与订阅者不需要进行直接通信。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603985380519-05e120c8-9e6e-4085-9bef-cc04b72ed518.webp#align=left&display=inline&height=392&originHeight=392&originWidth=711&size=0&status=done&style=shadow&width=711)<br />如上图所示，消息发布者只需要想指定的频道发布消息，订阅该频道的每个客户端都可以接受到到这个消息。<br />使用 Redis 发布订阅这种机制，对于上面业务，下单支付业务只需要向**「支付结果」**这个频道发送消息，其他下游业务订阅**「支付结果」**这个频道，就能收相应消息，然后做出业务处理即可。<br />这样就可以解耦系统上下游之间调用关系。<br />接下来看下，来看下如何使用 Redis 发布订阅功能。<br />Redis 中提供了一组命令，可以用于发布消息，订阅频道，取消订阅以及按照模式订阅。<br />首先来看下如何发布一条消息，其实很简单只要使用 **「publish」** 指令:
```
publish channel message
```
使用 **「publish」** 指令向 **「pay_result」** 这个频道发送了一条消息。可以看到 redis 返回 0 ，这其实代表当前订阅者个数，由于此时没有订阅，所以返回结果为 0 。<br />接下来使用 **「subscribe」** 订阅一个或多个频道
```bash
subscribe channel [channel ...]
```
订阅 **「pay_result」** 这个频道，当有其他客户端往这个频道发送消息，<br />当前订阅者就会收到消息。<br />在使用订阅命令，需要主要几点：<br />第一，客户端执行订阅指令之后，就会进入订阅状态，之后就只能接收 **「subscribe」**、**「psubscribe」**、**「unsubscribe」**、**「punsubscribe」** 这四个命令。<br />第二，新订阅的客户端，是**「无法收到这个频道之前的消息」**，这是因为 Redis 并不会对发布的消息持久化的。
> 相比于很多专业 MQ，比如 kafka、rocketmq 来说，  redis 发布订阅功能就显得有点简陋了。不过 redis 发布订阅功能胜在简单，如果当前场景可以容忍这些缺点，还是可以选择使用的。

除了上面的功能以外的，Redis 还支持模式匹配的订阅方式。简单来说，客户端可以订阅一个带 `*` 号的模式，如果某些频道的名字与这个模式匹配，那么当其他客户端发送给消息给这些频道时，订阅这个模式的客户端也将会到收到消息。<br />使用 Redis 订阅模式，需要使用一个新的指令 **「psubscribe」**。<br />执行下面这个指令：
```
psubscribe pay.*
```
那么一旦有其他客户端往  **「pay」** 开头的频道，比如 `pay_result`、`pay_xxx`，都可以收到消息。<br />如果需要取消订阅模式，需要使用相应`punsubscribe` 指令，比如取消上面订阅的模式：
```
punsubscribe pay.*
```
<a name="4CsbY"></a>
## Redis 客户端发布订阅使用方式
<a name="GqdyY"></a>
### 基于 Jedis 开发发布/订阅
看完 Redis 发布订阅指令，来看下 Java Redis 客户端如何使用发布订阅。
> 下面的例子主要基于 Jedis，maven 版本为：

```xml
<dependency>
  <groupId>redis.clients</groupId>
  <artifactId>jedis</artifactId>
  <version>3.1.0</version>
</dependency>
```
其他 Redis 客户端大同小异。<br />jedis 发布代码比较简单，只需要调用 `Jedis` 类的 `publish` 方法。
```java
// 生产环境千万不要这么使用，推荐使用 JedisPool 线程池的方式 
Jedis jedis = new Jedis("localhost", 6379);
jedis.auth("xxxxx");
jedis.publish("pay_result", "hello world");
```
订阅的代码就相对复杂了，需要继承 `JedisPubSub`实现里面的相关方法，一旦有其他客户端往订阅的频道上发送消息，将会调用 `JedisPubSub` 相应的方法。
```java
private static class MyListener extends JedisPubSub {
    @Override
    public void onMessage(String channel, String message) {
        System.out.println("收到订阅频道：" + channel + " 消息：" + message);
    }
    @Override
    public void onPMessage(String pattern, String channel, String message) {
        System.out.println("收到具体订阅频道：" + channel + "订阅模式：" + pattern + " 消息：" + message);
    }
}
```
其次需要调用 `Jedis` 类的 `subscribe` 方法：
```java
Jedis jedis = new Jedis("localhost", 6379);
jedis.auth("xxx");
jedis.subscribe(new MyListener(), "pay_result");
```
当有其他客户端往 `pay_result`频道发送消息时，订阅将会收到消息。<br />不过需要注意的是，`jedis#subscribe` 是一个阻塞方法，调用之后将会阻塞主线程的，所以如果需要在正式项目使用需要使用异步线程运行。
<a name="H5pFZ"></a>
### 基于 Spring-Data-Redis 开发发布订阅
原生 jedis 发布订阅操作，相对来说还是有点复杂。现在很多应用已经基于 SpringBoot 开发，使用 `spring-boot-starter-data-redis` ，可以简化发布订阅开发。<br />首先需要引入相应的 startter 依赖：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
  <exclusions>
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
```
> 这里使用 Jedis 当做底层连接客户端，所以需要排除 lettuce，然后引入 Jedis 依赖。

然后需要创建一个消息接收类，里面需要有方法消费消息：
```java
@Slf4j
public class Receiver {
    private AtomicInteger counter = new AtomicInteger();
    public void receiveMessage(String message) {
        log.info("Received <" + message + ">");
        counter.incrementAndGet();
    }
    public int getCount() {
        return counter.get();
    }
}
```
接着只需要注入 Spring- Redis 相关 Bean，比如：

- `StringRedisTemplate`，用来操作 Redis 命令
- `MessageListenerAdapter` ，消息监听器，可以在这个类注入上面创建消息接受类`Receiver`
- `RedisConnectionFactory`, 创建 Redis 底层连接
```java
@Configuration
public class MessageConfiguration {
    @Bean
    RedisMessageListenerContainer container(RedisConnectionFactory connectionFactory,
                                            MessageListenerAdapter listenerAdapter) {
        RedisMessageListenerContainer container = new RedisMessageListenerContainer();
        container.setConnectionFactory(connectionFactory);
        // 订阅指定频道使用 ChannelTopic
        // 订阅模式使用 PatternTopic
        container.addMessageListener(listenerAdapter, new ChannelTopic("pay_result"));
        return container;
    }
    @Bean
    MessageListenerAdapter listenerAdapter(Receiver receiver) {
        // 注入 Receiver，指定类中的接受方法
        return new MessageListenerAdapter(receiver, "receiveMessage");
    }
    @Bean
    Receiver receiver() {
        return new Receiver();
    }
    @Bean
    StringRedisTemplate template(RedisConnectionFactory connectionFactory) {
        return new StringRedisTemplate(connectionFactory);
    }
}
```
最后使用 `StringRedisTemplate#convertAndSend` 发送消息，同时 `Receiver` 将会收到一条消息。
```java
@SpringBootApplication
public class MessagingRedisApplication {
    public static void main(String[] args) throws InterruptedException {
        ApplicationContext ctx = SpringApplication.run(MessagingRedisApplication.class, args);
        StringRedisTemplate template = ctx.getBean(StringRedisTemplate.class);
        Receiver receiver = ctx.getBean(Receiver.class);
        while (receiver.getCount() == 0) {
            template.convertAndSend("pay_result", "Hello from Redis!");
            Thread.sleep(500L);
        }
        System.exit(0);
    }
}
```
<a name="XuxvA"></a>
## Redis 发布订阅实际应用
<a name="U6phe"></a>
### Redis Sentinel 节点发现
**「Redis Sentinel」** 是 Redis 一套高可用方案，可以在主节点故障的时候，自动将从节点提升为主节点，从而转移故障。<br />主要来看下 **「Redis Sentinel」** 如何使用发布订阅机制。<br />**「Redis Sentinel」**  节点主要使用发布订阅机制，实现新节点的发现，以及交换主节点的之间的状态。<br />如下所示，每一个  **「Sentinel」**  节点将会定时向 `_sentinel_:hello` 频道发送消息，并且每个 **「Sentinel」** 都会订阅这个节点。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603985380610-dbd025df-3367-4d79-abb1-59f80b626bcf.webp#align=left&display=inline&height=501&originHeight=501&originWidth=771&size=0&status=done&style=shadow&width=771)<br />这样一旦有节点往这个频道发送消息，其他节点就可以立刻收到消息。<br />这样一旦有的新节点加入，它往这个频道发送消息，其他节点收到之后，判断本地列表并没有这个节点，于是就可以当做新的节点加入本地节点列表。<br />除此之外，每次往这个频道发送消息内容可以包含节点的状态信息，这样可以作为后面 **「Sentinel」** 领导者选举的依据。<br />以上都是对于 Redis 服务端来讲，对于客户端来讲，也可以用到发布订阅机制。<br />当  **「Redis Sentinel」** 进行主节点故障转移，这个过程各个阶段会通过发布订阅对外提供。<br />对于客户端来讲，比较关心切换之后的主节点，这样及时切换主节点的连接（旧节点此时已故障，不能再接受操作指令），<br />客户端可以订阅 `+switch-master`频道，一旦  **「Redis Sentinel」**  结束了对主节点的故障转移就会发布主节点的的消息。
<a name="CpVUc"></a>
### redission 分布式锁
redission 开源框架提供一些便捷操作 Redis 的方法，其中比较出名的 redission  基于 Redis 的实现分布式锁。<br />来看下 Redis 的实现分布式锁中如何使用 Redis 发布订阅机制，提高加锁的性能。<br />首先来看下 redission   加锁的方法：
```java
Redisson redisson = ....
RLock redissonLock = redisson.getLock("xxxx");
redissonLock.lock();
```
`RLock` 继承自 Java 标准的 `Lock` 接口,调用 `lock` 方法,如果当前锁已被其他客户端获取，那么当前加锁的线程将会被阻塞，直到其他客户端释放这把锁。<br />这里其实有个问题，当前阻塞的线程如何感知分布式锁已被释放呢？<br />这里其实有两种实现方法：<br />第一钟，定时查询分布时锁的状态，一旦查到锁已被释放（Redis 中不存在这个键值），那么就去加锁。<br />实现伪码如下：
```java
while (true) {
    boolean result=lock();
    if (!result) {
        Thread.sleep(N);
    }
}
```
这种方式实现起来起来简单，不过缺点也比较多。<br />如果定时任务时间过短，将会导致查询次数过多，其实这些都是无效查询。<br />如果定时任务休眠时间过长，那又会导致加锁时间过长，导致加锁性能不好。<br />那么第二种实现方案，就是采用服务通知的机制，当分布式锁被释放之后，客户端可以收到锁释放的消息，然后第一时间再去加锁。<br />这个服务通知的机制可以使用 Redis 发布订阅模式。<br />当线程加锁失败之后，线程将会订阅 `redisson_lock__channel_xxx`（xx 代表锁的名称） 频道，使用异步线程监听消息，然后利用 Java 中 `Semaphore` 使当前线程进入阻塞。<br />一旦其他客户端进行解锁，redission  就会往这个`redisson_lock__channel_xxx` 发送解锁消息。<br />等异步线程收到消息，将会调用 `Semaphore` 释放信号量，从而让当前被阻塞的线程唤醒去加锁。<br />通过发布订阅机制，被阻塞的线程可以及时被唤醒，减少无效的空转的查询，有效的提高的加锁的效率。**
<a name="mazae"></a>
## 总结
主要介绍 Redis 发布订阅功能，主要对应的 Redis 命令为:

- **「subscribe channel [channel ...]」**  订阅一个或多个频道
- **「unsubscribe channel」**  退订指定频道
- **「publish channel message」** 发送消息
- **「psubscribe pattern」**  订阅指定模式
- **「punsubscribe pattern」**  退订指定模式

可以利用 Redis  发布订阅功能，实现的简单 MQ 功能，实现上下游的解耦。<br />不过需要注意了，由于 Redis 发布的消息不会被持久化，这就会导致新订阅的客户端将不会收到历史消息。<br />所以，如果当前的业务场景不能容忍这些缺点，那还是用专业 MQ 吧。<br />最后介绍了两个使用 Redis 发布订阅功能使用场景供大家参考。
