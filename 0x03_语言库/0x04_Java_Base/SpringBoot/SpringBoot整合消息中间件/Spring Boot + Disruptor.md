JavaSpringBootDisruptor
<a name="mS3S8"></a>
## Disruptor 介绍
Disruptor 是一个开源的高性能内存队列，由英国外汇交易公司 LMAX 开发的，获得了 2011 年的 Oracle 官方的 Duke's Choice Awards(Duke 选择大奖)。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673504619824-0f53103b-10e6-4996-8826-6c6f61b713eb.png#averageHue=%23161a20&clientId=u3c4e8f24-695b-4&from=paste&id=u4d345aaf&originHeight=773&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u94d19443-2ef8-45e3-9b18-35466d03b20&title=)<br />“Duke 选择大奖”旨在表彰过去一年里全球个人或公司开发的、最具影响力的 Java 技术应用，由甲骨文公司主办。含金量非常高！<br />专门找到了 Oracle 官方当年颁布获得 Duke's Choice Awards 项目的那篇文章（文章地址：[**https://blogs.oracle.com/java/post/and-the-winners-arethe-dukes-choice-award**](https://blogs.oracle.com/java/post/and-the-winners-arethe-dukes-choice-award)） 。从文中可以看出，同年获得此大奖荣誉的还有大名鼎鼎的 Netty 、JRebel 等项目。<br />![2011 年的 Oracle 官方的 Duke's Choice Awards](https://cdn.nlark.com/yuque/0/2023/png/396745/1673504619820-6c004152-eec0-4efb-b0e3-164d996609f9.png#averageHue=%23efeceb&clientId=u3c4e8f24-695b-4&from=paste&id=u50fc7e03&originHeight=1218&originWidth=961&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u484b4ec0-e8e7-4213-a2f0-d3623e7508a&title=2011%20%E5%B9%B4%E7%9A%84%20Oracle%20%E5%AE%98%E6%96%B9%E7%9A%84%20Duke%27s%20Choice%20Awards "2011 年的 Oracle 官方的 Duke's Choice Awards")<br />并且，有一些知名的开源项目到了 Disruptor ，就比如性能强大的 Java 日志框架 [**Log4j 2**](https://github.com/apache/logging-log4j2) 和蚂蚁金服分布式链路跟踪组件 [**SOFATracer**](https://github.com/sofastack/sofa-tracer) 就是基于 Disruptor 来做的异步日志，相关阅读：[**蚂蚁金服分布式链路跟踪组件 SOFATracer 中 Disruptor 实践（含源码）**](https://www.sofastack.tech/blog/sofa-trcaer-disruptor-practice/)。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673504619825-10db8a33-7596-4173-8e42-51cc0c4c308e.png#averageHue=%23181c22&clientId=u3c4e8f24-695b-4&from=paste&id=uaab8108d&originHeight=527&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue5d42256-7a87-44d4-861c-efe26829446&title=)<br />Disruptor 提供的功能类似于 Kafka、RocketMQ 这类分布式队列，不过，其作为范围是 JVM(内存)。

- Github 地址：[**https://github.com/LMAX-Exchange/disruptor**](https://github.com/LMAX-Exchange/disruptor)
- 官方教程：[**https://lmax-exchange.github.io/disruptor/user-guide/index.html**](https://lmax-exchange.github.io/disruptor/user-guide/index.html)

Disruptor 解决了 JDK 内置线程安全队列的性能和内存安全问题。<br />JDK 中常见的线程安全的队列如下：

| 队列名字 | 锁 | 是否有界 |
| --- | --- | --- |
| ArrayBlockingQueue | 加锁（ReentrantLock） | 有界 |
| LinkedBlockingQueue | 加锁（ReentrantLock） | 有界 |
| LinkedTransferQueue | 无锁（CAS） | 无界 |
| ConcurrentLinkedQueue | 无锁（CAS） | 无界 |

从上表中可以看出：这些队列要不就是加锁有界，要不就是无锁无界。而加锁的的队列势必会影响性能，无界的队列又存在内存溢出的风险。<br />因此，一般情况下，都是不建议使用 JDK 内置线程安全队列。<br />Disruptor 就不一样了！它在无锁的情况下还能保证队列有界，并且还是线程安全的。<br />不过， **Disruptor 的基本使用非常简单，最重要的还是要搞懂其原理，明白它是如何被设计成这么厉害的并发框架。**
<a name="XSg7x"></a>
## Disruptor 核心概念

- `**Event**`：可以把 Event 理解为存放在队列中等待消费的消息对象。
- `**EventFactory**`：事件工厂用于生产事件，在初始化 Disruptor 类的时候需要用到。
- `**EventHandler**`：`Event` 在对应的 Handler 中被处理，可以将其理解为生产消费者模型中的消费者。
- `**EventProcessor**`：`EventProcessor` 持有特定消费者(Consumer)的 Sequence，并提供用于调用事件处理实现的事件循环(Event Loop)。
- `**Disruptor**`：事件的生产和消费需要用到Disruptor 对象。
- `**RingBuffer**`：RingBuffer（环形数组）用于保存事件。
- `**WaitStrategy**`：等待策略。决定了没有事件可以消费的时候，事件消费者如何等待新事件的到来。
- `**Producer**`：生产者，只是泛指调用 Disruptor 发布事件的用户代码，Disruptor 没有定义特定接口或类型。
- `**ProducerType**`：指定是单个事件发布者模式还是多个事件发布者模式（发布者和生产者的意思类似，个人比较喜欢用发布者）。
- `**Sequencer**`：`Sequencer` 是 Disruptor 的真正核心。此接口有两个实现类 `SingleProducerSequencer`、`MultiProducerSequencer` ，它们定义在生产者和消费者之间快速、正确地传递数据的并发算法。

![LMAX Disruptor User Guide](https://cdn.nlark.com/yuque/0/2023/png/396745/1673504619833-8271ec14-5673-43ff-b720-ba6d330afc46.png#averageHue=%23f0f0ed&clientId=u3c4e8f24-695b-4&from=paste&id=u677c7bd3&originHeight=536&originWidth=806&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue4b06e0a-538b-43a5-bcfc-71907b366d4&title=LMAX%20Disruptor%20User%20Guide "LMAX Disruptor User Guide")
<a name="piHW9"></a>
## Disruptor 实战
要使用 Disruptor 实现一个最基本的生产消费模型的整个步骤是下面这样的（标准的生产消费者模型）：

1. **定义事件（Event）**：可以把 Event 理解为存放在队列中等待消费的消息对象。
2. **创建事件工厂**：事件工厂用于生产事件，在初始化 Disruptor 类的时候需要用到。
3. **创建处理事件的 Handler**：Event 在对应的 Handler 中被处理，可以将其理解为生产消费者模型中的消费者。
4. **创建并启动 Disruptor**：事件的生产和消费需要用到Disruptor 对象。
5. **发布事件**：发布的事件保存在 Disruptor 的环形数组中。
6. **关闭 Disruptor**：类似于线程池的关闭。

整个步骤看似比较复杂，其实，逻辑还是比较简单的。需要围绕事件（Event）和Disruptor来做文章。<br />可以在 Mavan 仓库找到 Disruptor 的最新 jar 包。<br />Disruptor 的 Maven 仓库地址：[**https://search.maven.org/artifact/com.lmax/disruptor**](https://search.maven.org/artifact/com.lmax/disruptor)<br />Maven ：
```xml
<dependency>
    <groupId>com.lmax</groupId>
    <artifactId>disruptor</artifactId>
    <version>3.4.4</version>
</dependency>
```
Gradle：
```
implementation 'com.lmax:disruptor:3.4.4'
```
<a name="pLn2B"></a>
### 1、定义事件
先来定义一个代表日志事件的类：LogEvent 。<br />事件中包含了一些和事件相关的属性，比如这里定义的 LogEvent 对象中就有一个用来表示日志消息内容的属性：message。
```java
public class LogEvent {
    private String message;
    //省略了 Getter/Setter
}
```
这里只是为了演示，实际项目中，一个标准日志事件对象所包含的属性肯定不是只有一个 message （可以参考 log4j2 对 Disruptor 的使用）。
<a name="VpIkV"></a>
### 2、创建事件工厂
创建一个工厂类 `LogEventFactory` 用来创建 `LogEvent` 对象。<br />`LogEventFactory` 继承 `EventFactory` 接口并实现了 `newInstance()` 方法 。
```java
public class LogEventFactory implements EventFactory<LogEvent> {
    @Override
    public LogEvent newInstance() {
        return new LogEvent();
    }
}
```
<a name="ZiUfR"></a>
### 3、创建处理事件的 `Handler`
创建一个用于处理后续发布的事件的类：`LogEventHandler `。<br />`LogEventHandler` 继承 `EventHandler` 接口并实现了 `onEvent()` 方法 。
```java
public class LogEventHandler implements EventHandler<LogEvent> {
    @Override
    public void onEvent(LogEvent logEvent, long sequence, boolean endOfBatch) throws Exception {
        System.out.println(logEvent.getMessage());
    }
}
```
`EventHandler` 接口的 `onEvent()` 方法共有 3 个参数：

- `event`：待消费/处理的事件
- `sequence`：正在处理的事件在环形数组（`RingBuffer`）中的位置
- `endOfBatch`：表示这是否是来自环形数组（`RingBuffer`）中一个批次的最后一个事件（批量处理事件）
```java
public interface EventHandler<T>
    void onEvent(T event, long sequence, boolean endOfBatch) throws Exception;
}
```
<a name="mOewp"></a>
### 4、初始化 Disruptor
这里定义一个方法用于获取 Disruptor 对象。
```java
private static Disruptor<LogEvent> getLogEventDisruptor() {
    // 创建 LogEvent 的工厂
    LogEventFactory logEventFactory = new LogEventFactory();
    // Disruptor 的 RingBuffer 缓存大小
    int bufferSize = 1024 * 1024;
    // 生产者的线程工厂
    ThreadFactory threadFactory = new ThreadFactory() {
        final AtomicInteger threadNum = new AtomicInteger(0);

        @Override
        public Thread newThread(Runnable r) {
            return new Thread(r, "LogEventThread" + " [#" + threadNum.incrementAndGet() + "]");
        }
    };
    //实例化 Disruptor
    return new Disruptor<>(
            logEventFactory,
            bufferSize,
            threadFactory,
            // 单生产者
            ProducerType.SINGLE,
            // 阻塞等待策略
            new BlockingWaitStrategy());
}
```
Disruptor 的推荐使用的构造函数如下：
```java
public class Disruptor<T> {
  public Disruptor(
          final EventFactory<T> eventFactory,
          final int ringBufferSize,
          final ThreadFactory threadFactory,
          final ProducerType producerType,
          final WaitStrategy waitStrategy){
      this(
          RingBuffer.create(producerType, eventFactory, ringBufferSize, waitStrategy),
          new BasicExecutor(threadFactory));
  }

......
}
```
需要传递 5 个参数：

- `eventFactory`：自定义的时间工厂。
- `ringBufferSize`：指定 RingBuffer 的容量大小。
- `threadFactory`：自定义的线程工厂。Disruptor 的默认线程池是自定义的，只需要传入线程工厂即可。
- `producerType`：指定是单个事件发布者模式还是多个事件发布者模式（发布者和生产者的意思类似，个人比较喜欢用发布者）。
- `waitStrategy`：等待策略，决定了没有事件可以消费的时候，事件消费者如何等待新事件的到来。

`ProducerType` 的源码如下，它是一个包含两个变量的枚举类型
```java
public enum ProducerType{
    SINGLE,
    MULTI
}
```

- `SINGLE`：单个事件发布者模式，不需要保证线程安全。
- `MULTI`：多个事件发布者模式，基于 CAS 来保证线程安全。

`WaitStrategy`（等待策略）接口的实现类中只有两个方法：

- `waitFor()`：等待新事件的到来。
- `signalAllWhenBlocking()`：唤醒所有等待的消费者。
```java
public interface WaitStrategy{
    long waitFor(long sequence, Sequence cursor, Sequence dependentSequence, SequenceBarrier barrier)
        throws AlertException, InterruptedException, TimeoutException;
    void signalAllWhenBlocking();
}
```
`WaitStrategy` 的实现类共有 8 个，也就是说共有 8 种等待策略可供选择。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673504619834-5bb87e60-6d3d-495d-9c80-a026aed7b62b.png#averageHue=%23333943&clientId=u3c4e8f24-695b-4&from=paste&id=u19810c6c&originHeight=484&originWidth=1009&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uef6b7a3a-04ac-4094-92d3-17000dd3b71&title=)<br />除了上面介绍的这个构造函数之外，Disruptor 还有一个只有 3 个参数构造函数。<br />使用这个构造函数创建的 Disruptor 对象会默认使用 `ProducerType.MULTI`（多个事件发布者模式）和 `BlockingWaitStrategy`（阻塞等待策略） 。
```java
public Disruptor(final EventFactory<T> eventFactory, final int ringBufferSize, final ThreadFactory threadFactory){
    this(RingBuffer.createMultiProducer(eventFactory, ringBufferSize), new BasicExecutor(threadFactory));
}
```
<a name="J2CTc"></a>
### 5、发布事件
```java
//获取 Disruptor 对象
Disruptor<LogEvent> disruptor = getLogEventDisruptor();
//绑定处理事件的Handler对象
disruptor.handleEventsWith(new LogEventHandler());
//启动 Disruptor
disruptor.start();
//获取保存事件的环形数组（RingBuffer）
RingBuffer<LogEvent> ringBuffer = disruptor.getRingBuffer();
//发布 10w 个事件
for (int i = 1; i <= 100000; i++) {
    // 通过调用 RingBuffer 的 next() 方法获取下一个空闲事件槽的序号
    long sequence = ringBuffer.next();
    try {
        LogEvent logEvent = ringBuffer.get(sequence);
        // 初始化 Event，对其赋值
        logEvent.setMessage("这是第%d条日志消息".formatted(i));
    } finally {
        // 发布事件
        ringBuffer.publish(sequence);
    }
}
// 关闭 Disruptor
disruptor.shutdown();
```
上面的代码中，通过 Disruptor 的 `handleEventsWith` 方法来绑定处理事件的 Handler 对象。<br />Disruptor 可以设置多个处理事件的 Handler，并且可以灵活的设置消费者的处理顺序，串行，并行都是可以的。<br />就比如下面的代码表示 Handler1 和 Handler2 是并行执行，最后再执行 Handler3 。
```
disruptor.handleEventsWith(new Handler1(), new Handler2()).handleEventsWith(new Handler3());
```
<a name="MElrd"></a>
### 6、结果
```
这是第1条日志消息
这是第2条日志消息
这是第3条日志消息
......
这是第99999条日志消息
这是第100000条日志消息
```
从打印结果可以看出，发布的 10w 个事件已经成功被处理。
<a name="RDrRw"></a>
## 总结
Disruptor 提供的功能类似于 Kafka、RocketMQ 这类分布式队列，不过，其作为范围是 JVM(内存)。

- Github 地址：[**https://github.com/LMAX-Exchange/disruptor**](https://github.com/LMAX-Exchange/disruptor)
- 官方教程：[**https://lmax-exchange.github.io/disruptor/user-guide/index.html**](https://lmax-exchange.github.io/disruptor/user-guide/index.html)

Disruptor 在无锁的情况下还能保证队列有界，并且还是线程安全的，性能非常强，比较适合单机场景需要使用生产者-消费者模式的项目。
<a name="OmLwp"></a>
## 参考资料
Log4j 2: [_https://github.com/apache/logging-log4j2_](https://github.com/apache/logging-log4j2)<br />SOFATracer: [_https://github.com/sofastack/sofa-tracer_](https://github.com/sofastack/sofa-tracer)<br />蚂蚁金服分布式链路跟踪组件 SOFATracer 中 Disruptor 实践（含源码）: [_https://www.sofastack.tech/blog/sofa-trcaer-disruptor-practice/_](https://www.sofastack.tech/blog/sofa-trcaer-disruptor-practice/)
