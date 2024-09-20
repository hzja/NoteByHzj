JavaSpringBoot
<a name="GoZWc"></a>
## 背景
有个小项目需要使用延迟任务，谈到延迟任务，第一时间一闪而过的就是使用消息队列来做，比如RabbitMQ的死信队列又或者RocketMQ的延迟队列，但是奈何这是一个小项目，并没有引入MQ，也不太想因为一个延迟任务就引入MQ，增加系统复杂度，所以这个方案直接就被pass了。<br />虽然基于MQ这个方式走不通了，但是这个项目中使用到Redis，所以就想是否能够使用Redis来代替MQ实现延迟队列的功能，于是就查了一下有没有现成可用的方案，查到了两种方案，并且还仔细研究对比了这两个方案，发现要想很好的实现延迟队列，并不简单。
<a name="AU5gS"></a>
## 监听过期key
基于监听过期key的方式来实现延迟队列是查到的第一个方案，为了弄懂这个方案实现的细节，还特地去扒了扒官网，还真有所收获
<a name="NDsPA"></a>
### 1、Redis发布订阅模式
一谈到发布订阅模式，其实一想到的就是MQ，只不过Redis也实现了一套，并且跟MQ贼像，如图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491462-ff3d34d7-9750-4bbe-819f-588ea140f4a3.png#averageHue=%234c453e&clientId=u89f78e6d-c242-4&from=paste&id=ua4ef7246&originHeight=167&originWidth=426&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3bbcb66b-082b-4a8d-bb87-dc501867297&title=)<br />图中的channel的概念跟MQ中的topic的概念差不多，可以把channel理解成MQ中的topic。<br />生产者在消息发送时需要到指定发送到哪个channel上，消费者订阅这个channel就能获取到消息。
<a name="BMPdl"></a>
### 2、keyspace notifications
在Redis中，有很多默认的channel，只不过向这些channel发送消息的生产者不是编写的代码，而是Redis本身。当消费者监听这些channel时，就可以感知到Redis中数据的变化。<br />这个功能Redis官方称为keyspace notifications，字面意思就是键空间通知。<br />这些默认的channel被分为两类：

- 以`__keyspace@<db>__:`为前缀，后面跟的是key的名称，表示监听跟这个key有关的事件。举个例子，现在有个消费者监听了`__keyspace@0__:sanyou`这个channel，sanyou就是Redis中的一个普通key，那么当sanyou这个key被删除或者发生了其它事件，那么消费者就会收到sanyou这个key删除或者其它事件的消息
- 以`__keyevent@<db>__:`为前缀，后面跟的是消息事件类型，表示监听某个事件同样举个例子，现在有个消费者监听了`__keyevent@0__:expired`这个channel，代表了监听key的过期事件。那么当某个Redis的key过期了（expired），那么消费者就能收到这个key过期的消息。如果把expired换成del，那么监听的就是删除事件。具体支持哪些事件，可从官网查。

上述db是指具体的数据库，Redis不是默认分为16个库么，序号从0-15，所以db就是0到15的数字，示例中的0就是指0对应的数据库。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491496-161c42b7-2a9a-427d-94f3-0605251089af.png#averageHue=%23f3eae9&clientId=u89f78e6d-c242-4&from=paste&id=ufb06b65d&originHeight=208&originWidth=322&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufd08641e-4ba7-4294-b349-b67e7f8639e&title=)
<a name="SPPSu"></a>
### 3、延迟队列实现原理
通过对上面的两个概念了解之后，应该就对监听过期key的实现原理一目了然了，其实就是当这个key过期之后，Redis会发布一个key过期的事件到`__keyevent@<db>__:expired`这个channel，只要服务监听这个channel，那么就能知道过期的Key，从而就算实现了延迟队列功能。<br />所以这种方式实现延迟队列就只需要两步：

- 发送延迟任务，key是延迟消息本身，过期时间就是延迟时间
- 监听`__keyevent@<db>__:expired`这个channel，处理延迟任务
<a name="UQKKA"></a>
### 4、demo
好了，基本概念和核心原理都说完了之后，又到了show me the code环节。<br />好巧不巧，Spring已经实现了监听`__keyevent@*__:expired`这个channel这个功能，`__keyevent@*__:expired`中的*代表通配符的意思，监听所有的数据库。<br />所以demo写起来就很简单了，只需3步即可<br />引入pom
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
    <version>2.2.5.RELEASE</version>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <version>2.2.5.RELEASE</version>
</dependency>
```
配置类
```java
@Configuration
public class RedisConfiguration {

    @Bean
    public RedisMessageListenerContainer redisMessageListenerContainer(RedisConnectionFactory connectionFactory) {
        RedisMessageListenerContainer redisMessageListenerContainer = new RedisMessageListenerContainer();
        redisMessageListenerContainer.setConnectionFactory(connectionFactory);
        return redisMessageListenerContainer;
    }

    @Bean
    public KeyExpirationEventMessageListener redisKeyExpirationListener(RedisMessageListenerContainer redisMessageListenerContainer) {
        return new KeyExpirationEventMessageListener(redisMessageListenerContainer);
    }

}
```
`KeyExpirationEventMessageListener`实现了对`__keyevent@*__:expired`channel的监听<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491535-d8c41c1c-2bfb-46c3-9b59-34e869055e3d.png#averageHue=%232e2c2b&clientId=u89f78e6d-c242-4&from=paste&id=u5070f7d8&originHeight=392&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uda3c08e3-9f22-4b08-aa8e-2dc29540968&title=)<br />当`KeyExpirationEventMessageListener`收到Redis发布的过期Key的消息的时候，会发布`RedisKeyExpiredEvent`事件<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491434-cbc801c1-dcee-4893-8237-d93abbbf7ee9.png#averageHue=%23302c2b&clientId=u89f78e6d-c242-4&from=paste&id=u0ea4d9b8&originHeight=695&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0f71d371-2270-4a4d-8854-735e7cab155&title=)<br />所以只需要监听`RedisKeyExpiredEvent`事件就可以拿到过期消息的Key，也就是延迟消息。<br />对`RedisKeyExpiredEvent`事件的监听实现`MyRedisKeyExpiredEventListener`
```java
@Component
public class MyRedisKeyExpiredEventListener implements ApplicationListener<RedisKeyExpiredEvent> {

    @Override
    public void onApplicationEvent(RedisKeyExpiredEvent event) {
        byte[] body = event.getSource();
        System.out.println("获取到延迟消息：" + new String(body));
    }

}
```
整个工程目录也简单<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491486-64d9abfb-9134-4495-8765-1ce8a9541fea.png#averageHue=%233e4244&clientId=u89f78e6d-c242-4&from=paste&id=u72527b78&originHeight=292&originWidth=644&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u61dcf170-fd97-483f-8c73-afbad1eca30&title=)<br />代码写好，启动应用<br />之后直接通过Redis命令设置消息，就没通过代码发送消息了，消息的key为sanyou，值为task，值不重要，过期时间为5s
```
set sanyou task 

expire sanyou 5
```
如果上面都理论都正确，不出意外的话，5s后`MyRedisKeyExpiredEventListener`应该可以监听到sanyou这个key过期的消息，也就相当于拿到了延迟任务，控制台会打印出获取到延迟消息：sanyou。<br />于是满怀希望，静静地等待了5s。。<br />5、4、3、2、1，时间一到，查看控制台，但是控制台并没有按照预期打印出上面那句话。<br />为什么会没打印出？难道是代码写错了？正当准备检查代码的时候，官网的一段话道出了真实原因。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491823-1f6bb9cb-2ba5-435c-82a3-1eded4ddd486.png#averageHue=%23fefefe&clientId=u89f78e6d-c242-4&from=paste&id=u4ea5bf89&originHeight=668&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5e2e70c9-a9b0-4ff1-ab6b-bb828babdd2&title=)<br />翻译一下上面这段话讲的内容。<br />上面这段话主要讨论的是key过期事件的时效性问题，首先提到了Redis过期key的两种清除策略，就是面试八股文常背的两种：

- 惰性清除。当这个key过期之后，访问时，这个Key才会被清除
- 定时清除。后台会定期检查一部分key，如果有key过期了，就会被清除

再后面那段话是核心，意思是说，key的过期事件发布时机并不是当这个key的过期时间到了之后就发布，而是这个key在Redis中被清理之后，也就是真正被删除之后才会发布。<br />到这终于明白了，上面的例子中即使设置了5s的过期时间，但是当5s过去之后，只要两种清除策略都不满足，没人访问sanyou这个key，后台的定时清理的任务也没扫描到sanyou这个key，那么就不会发布key过期的事件，自然而然也就监听不到了。<br />至于后台的定时清理的任务什么时候能扫到，这个没有固定时间，可能一到过期时间就被扫到，也可能等一定时间才会被扫到，这就可能会造成了客户端从发布到监听到的消息时间差会大于等于过期时间，从而造成一定时间消息的延迟，这就着实有点坑了。。
<a name="a1eCB"></a>
### 5、坑
除了上面测试demo的时候遇到的坑之外，在深入研究之后，还发现了一些更离谱的坑。
<a name="vRd3K"></a>
#### 丢消息太频繁
Redis的丢消息跟MQ不一样，因为MQ都会有消息的持久化机制，可能只有当机器宕机了，才会丢点消息，但是Redis丢消息就很离谱，比如说服务在重启的时候就消息会丢消息。<br />Redis实现的发布订阅模式，消息是没有持久化机制，当消息发布到某个channel之后，如果没有客户端订阅这个channel，那么这个消息就丢了，并不会像MQ一样进行持久化，等有消费者订阅的时候再给消费者消费。<br />所以说，假设服务重启期间，某个生产者或者是Redis本身发布了一条消息到某个channel，由于服务重启，没有监听这个channel，那么这个消息自然就丢了。
<a name="DD6Ub"></a>
#### 消息消费只有广播模式
Redis的发布订阅模式消息消费只有广播模式一种。<br />所谓的广播模式就是多个消费者订阅同一个channel，那么每个消费者都能消费到发布到这个channel的所有消息。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491873-8fefb73e-6d91-4645-930d-61bf076d001a.png#averageHue=%233f3933&clientId=u89f78e6d-c242-4&from=paste&id=uecd8500e&originHeight=197&originWidth=436&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u52b49d77-b2bc-447f-a288-a58bf33be39&title=)<br />如图，生产者发布了一条消息，内容为sanyou，那么两个消费者都可以同时收到sanyou这条消息。<br />所以，如果通过监听channel来获取延迟任务，那么一旦服务实例有多个的话，还得保证消息不能重复处理，额外地增加了代码开发量。
<a name="fGRDG"></a>
#### 接收到所有key的某个事件
这个不属于Redis发布订阅模式的问题，而是Redis本身事件通知的问题。<br />当消费者监听了以`__keyevent@<db>__:`开头的消息，那么会导致所有的key发生了事件都会被通知给消费者。<br />举个例子，某个消费者监听了`__keyevent@*__:expired`这个channel，那么只要key过期了，不管这个key是张三还会李四，消费者都能收到。<br />所以如果只想消费某一类消息的key，那么还得自行加一些标记，比如消息的key加个前缀，消费的时候判断一下带前缀的key就是需要消费的任务。<br />所以，综上能够得出一个非常重要的结论，那就是监听Redis过期Key这种方式实现延迟队列，**不稳定，坑贼多！**<br />那有没有比较靠谱的延迟队列的实现方案呢？这就不得不提到第二种方案了。
<a name="Crw2z"></a>
## Redisson实现延迟队列
Redisson他是Redis的儿子（Redis son），基于Redis实现了非常多的功能，其中最常使用的就是Redis分布式锁的实现，但是除了实现Redis分布式锁之外，它还实现了延迟队列的功能。<br />先来个demo，后面再来说说这种实现的原理。
<a name="QPo0d"></a>
### 1、demo
引入pom
```xml
<dependency>
    <groupId>org.redisson</groupId>
    <artifactId>redisson</artifactId>
    <version>3.13.1</version>
</dependency>
```
封装了一个RedissonDelayQueue类
```java
@Component
@Slf4j
public class RedissonDelayQueue {

    private RedissonClient redissonClient;

    private RDelayedQueue<String> delayQueue;
    private RBlockingQueue<String> blockingQueue;

    @PostConstruct
    public void init() {
        initDelayQueue();
        startDelayQueueConsumer();
    }

    private void initDelayQueue() {
        Config config = new Config();
        SingleServerConfig serverConfig = config.useSingleServer();
        serverConfig.setAddress("redis://localhost:6379");
        redissonClient = Redisson.create(config);

        blockingQueue = redissonClient.getBlockingQueue("SANYOU");
        delayQueue = redissonClient.getDelayedQueue(blockingQueue);
    }

    private void startDelayQueueConsumer() {
        new Thread(() -> {
            while (true) {
                try {
                    String task = blockingQueue.take();
                    log.info("接收到延迟任务:{}", task);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, "SANYOU-Consumer").start();
    }

    public void offerTask(String task, long seconds) {
        log.info("添加延迟任务:{} 延迟时间:{}s", task, seconds);
        delayQueue.offer(task, seconds, TimeUnit.SECONDS);
    }

}
```
这个类在创建的时候会去初始化延迟队列，创建一个`RedissonClient`对象，之后通过`RedissonClient`对象获取到`RDelayedQueue`和`RBlockingQueue`对象，传入的队列名字叫SANYOU，这个名字无所谓。<br />当延迟队列创建之后，会开启一个延迟任务的消费线程，这个线程会一直从`RBlockingQueue`中通过take方法阻塞获取延迟任务。<br />添加任务的时候是通过`RDelayedQueue`的offer方法添加的。<br />controller类，通过接口添加任务，延迟时间为5s
```java
@RestController
public class RedissonDelayQueueController {

    @Resource
    private RedissonDelayQueue redissonDelayQueue;

    @GetMapping("/add")
    public void addTask(@RequestParam("task") String task) {
        redissonDelayQueue.offerTask(task, 5);
    }

}
```
启动项目，在浏览器输入如下连接，添加任务<br />http://localhost:8080/add?task=sanyou<br />静静等待5s，成功获取到任务。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491879-d7693dab-59ee-4b57-9a9b-dd60b84e747b.png#averageHue=%23312f2f&clientId=u89f78e6d-c242-4&from=paste&id=uc2d984cb&originHeight=232&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1b0def1e-c943-418b-a027-5581e3bed02&title=)
<a name="mkjGL"></a>
### 2、实现原理
如下图就是上面demo中，一个延迟队列会在Redis内部使用到的channel和数据类型<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491888-5e938887-b389-4d27-8f45-fe643d8652a2.png#averageHue=%23e8d7d1&clientId=u89f78e6d-c242-4&from=paste&id=ubc035b74&originHeight=291&originWidth=341&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u50f3f29c-840e-49fa-b117-a5b26ae3b44&title=)<br />SANYOU前面的前缀都是固定的，Redisson创建的时候会拼上前缀。

- `redisson_delay_queue_timeout:SANYOU`，sorted set数据类型，存放所有延迟任务，按照延迟任务的到期时间戳（提交任务时的时间戳 + 延迟时间）来排序的，所以列表的最前面的第一个元素就是整个延迟队列中最早要被执行的任务，这个概念很重要
- `redisson_delay_queue:SANYOU`，list数据类型，也是存放所有的任务，但是研究下来发现好像没什么用。。
- SANYOU，list数据类型，被称为目标队列，这个里面存放的任务都是已经到了延迟时间的，可以被消费者获取的任务，所以上面demo中的`RBlockingQueue`的take方法是从这个目标队列中获取到任务的
- `redisson_delay_queue_channel:SANYOU`，是一个channel，用来通知客户端开启一个延迟任务

有了这些概念之后，再来看看整体的运行原理图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786491987-d26a3d95-62f6-414b-ac18-ddd992605c7a.png#averageHue=%23fae0c4&clientId=u89f78e6d-c242-4&from=paste&id=u3b21c805&originHeight=521&originWidth=574&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5630ee41-0845-4ef6-88ab-7961179069e&title=)

- 生产者在提交任务的时候将任务放到`redisson_delay_queue_timeout:SANYOU`中，分数就是提交任务的时间戳+延迟时间，就是延迟任务的到期时间戳
- 客户端会有一个延迟任务，为了区分，后面都说是客户端延迟任务。这个延迟任务会向Redis Server发送一段lua脚本，Redis执行lua脚本中的命令，并且是原子性的

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786492245-ef57164f-c9b5-4acb-a136-6de8798a6dbe.png#averageHue=%232b2b2b&clientId=u89f78e6d-c242-4&from=paste&id=u6550286e&originHeight=675&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2bc21ed6-7ac9-4a63-98ac-d39f497e7f3&title=)<br />这段lua脚本主要干了两件事：

- 将到了延迟时间的任务从`redisson_delay_queue_timeout:SANYOU`中移除，存到SANYOU这个目标队列
- 获取到`redisson_delay_queue_timeout:SANYOU`中目前最早到过期时间的延迟任务的到期时间戳，然后发布到`redisson_delay_queue_channel:SANYOU`这个channel中

当客户端监听到`redisson_delay_queue_channel:SANYOU`这个channel的消息时，会再次提交一个客户端延迟任务，延迟时间就是消息（最早到过期时间的延迟任务的到期时间戳）- 当前时间戳，这个时间其实也就是`redisson_delay_queue_channel:SANYOU`中最早到过期时间的任务还剩余的延迟时间。<br />此处可以等待10s，好好想想。。<br />这样，一旦时间来到了上面说的最早到过期时间任务的到期时间戳，`redisson_delay_queue_timeout:SANYOU`中上面说的最早到过期时间的任务已经到期了，客户端的延迟任务也同时到期，于是开始执行lua脚本操作，及时将到了延迟时间的任务放到目标队列中。然后再次发布剩余的延迟任务中最早到期的任务到期时间戳到channe中，如此循环往复，一直运行下去，保证`redisson_delay_queue_timeout:SANYOU`中到期的数据能及时放到目标队列中。<br />所以，上述说了一大堆的主要的作用就是保证到了延迟时间的任务能够及时被放到目标队列。<br />这里再补充两个特殊情况，图中没有画出：<br />第一个就是如果`redisson_delay_queue_timeout:SANYOU`是新添加的任务（队列之前有或者没有任务）是队列中最早需要被执行的，也会发布消息到channel，之后就按时上面说的流程走了。<br />添加任务代码如下，也是通过lua脚本来的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676786492297-4a6de264-c953-48d5-b262-1e3b9aae25d0.png#averageHue=%232c2b2b&clientId=u89f78e6d-c242-4&from=paste&id=uc3aa63de&originHeight=615&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4b88795d-0f4d-4544-9edd-2fa53a731ea&title=)<br />第二种特殊情况就是项目启动的时候会执行一次客户端延迟任务。项目在重启时，由于没有客户端延迟任务的执行，可能会出现`redisson_delay_queue_timeout:SANYOU`队列中有到期但是没有被放到目标队列的可能，重启就执行一次就是为了保证到期的数据能被及时放到目标队列中。
<a name="Onm6P"></a>
### 3、与第一种方案比较
现在来比较一下第一种方案和Redisson的这种方案，看看有没有第一种方案的那些坑。<br />第一个任务延迟的问题，Redisson方案理论上是没有延迟的，但是当消息数量增加，消费者消费缓慢这个情况下可能会导致延迟任务消费的延迟。<br />第二个丢消息的问题，Redisson方案很大程度上减轻了丢消息的可能性，因为所有的任务都是存在list和sorted set两种数据类型中，Redis有持久化机制，就算Redis宕机了，也就可能会丢一点点数据。<br />第三个广播消费任务的问题，这个是不会出现的，因为每个客户端都是从同一个目标队列中获取任务的。<br />第四个问题是Redis内部channel发布事件的问题，跟这种方案不沾边，就更不可能存在了。<br />所以，通过上面的对比可以看出，Redisson这种实现方案就显得更加的靠谱了。
