Java
<a name="n86jl"></a>
### Java中有哪些队列呢

- `ArrayBlockingQueue` 使用ReentrantLock
- `LinkedBlockingQueue` 使用ReentrantLock
- `ConcurrentLinkedQueue` 使用CAS
- 等等

使用锁的性能比较低，尽量使用无锁设计。接下来就来认识下Disruptor。
<a name="uN8YV"></a>
### Disruptor简单使用
Github地址：[https://github.com/LMAX-Exchange/disruptor/wiki/Performance-Results](https://github.com/LMAX-Exchange/disruptor/wiki/Performance-Results)<br />先简单介绍下：

- Disruptor它是一个开源的并发框架，并获得2011 Duke’s程序框架创新奖【Oracle】，能够在无锁的情况下实现网络的Queue并发操作。英国外汇交易公司LMAX开发的一个高性能队列，号称单线程能支撑每秒600万订单~
- 日志框架Log4j2 异步模式采用了Disruptor来处理
- 局限呢，他就是个内存队列，也就是说无法支撑分布式场景。

![2022-12-07-21-46-44.133169200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421198140-c982dca4-9d9a-43b6-9ebc-54f9f8e33a6e.png#averageHue=%23f4f4f5&clientId=u9284130a-1c60-4&from=ui&id=ua9ca7a56&originHeight=1384&originWidth=1450&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6031603&status=done&style=none&taskId=u5417b210-c76c-480a-a8fd-99f8db92a03&title=)![2022-12-07-21-46-44.538034100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421197486-00864f11-8e9c-42c9-bfae-914c3a769c95.png#averageHue=%23fefefe&clientId=u9284130a-1c60-4&from=ui&id=uf7ef113a&originHeight=797&originWidth=1500&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3593170&status=done&style=none&taskId=u4a3e3de6-b626-4656-8876-793d747ad01&title=)
<a name="DftPJ"></a>
### 简单使用
数据传输对象
```java
@Data
public class EventData {
    private Long value;
}
```
消费者
```java
public class EventConsumer implements WorkHandler<EventData> {

    /**
     * 消费回调
     * @param eventData
     * @throws Exception
     */
    @Override
    public void onEvent(EventData eventData) throws Exception {
        Thread.sleep(5000);
        System.out.println(Thread.currentThread() + ", eventData:" + eventData.getValue());
    }
}
```
生产者
```java
public class EventProducer {

    private final RingBuffer<EventData> ringBuffer;

    public EventProducer(RingBuffer<EventData> ringBuffer) {
        this.ringBuffer = ringBuffer;
    }

    public void sendData(Long v){
        // cas展位
        long next = ringBuffer.next();
        try {
            EventData eventData = ringBuffer.get(next);
            eventData.setValue(v);
        } finally {
            // 通知等待的消费者
            System.out.println("EventProducer send success, sequence:"+next);
            ringBuffer.publish(next);
        }
    }
}
```
测试类
```java
public class DisruptorTest {

    public static void main(String[] args) {
        // 2的n次方
        int bufferSize = 8;

        Disruptor<EventData> disruptor = new Disruptor<EventData>(
            () -> new EventData(), // 事件工厂
            bufferSize,            // 环形数组大小
            Executors.defaultThreadFactory(),       // 线程池工厂
            ProducerType.MULTI,    // 支持多事件发布者
            new BlockingWaitStrategy());    // 等待策略

        // 设置消费者
        disruptor.handleEventsWithWorkerPool(
            new EventConsumer(),
            new EventConsumer(),
            new EventConsumer(),
            new EventConsumer());

        disruptor.start();

        RingBuffer<EventData> ringBuffer = disruptor.getRingBuffer();
        EventProducer eventProducer = new EventProducer(ringBuffer);
        long i  = 0;
        for(;;){
            i++;
            eventProducer.sendData(i);
            try {
                Thread.sleep(1500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
```
<a name="YqmWp"></a>
### 核心组件
基于上面简单例子来看确实很简单，Disruptor封装好了生产消费模型的实现，接下来看下他是基于哪些核心组件来支撑起一个高性能无锁队列呢？

- `**RingBuffer**`**：** 环形数组，底层使用数组entries，在初始化时填充数组，避免不断新建对象带来的开销。后续只会对entries做更新操作

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421062782-41ec331b-c2c4-4b78-afd3-bdb5dbb54c03.png#averageHue=%23f9f9f9&clientId=u9284130a-1c60-4&from=paste&id=ua1e48c85&originHeight=354&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9423b1ee-50cb-4e83-964d-07052ff9b08&title=)<br />![2022-12-07-21-46-44.862317100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421411703-4dd628b8-0eed-4cae-95c1-384ce5fbdfcc.png#averageHue=%232f2e2c&clientId=u9284130a-1c60-4&from=ui&id=u7053e2ad&originHeight=206&originWidth=1380&originalType=binary&ratio=1&rotation=0&showTitle=false&size=854508&status=done&style=none&taskId=u9995b707-f264-4a92-863b-49a56fac0c1&title=)![2022-12-07-21-46-45.049390800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421412710-189f0526-2f3a-423a-9984-7ccdc59af398.png#averageHue=%232c2b2b&clientId=u9284130a-1c60-4&from=ui&id=uf47c8e9d&originHeight=830&originWidth=1500&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3741956&status=done&style=none&taskId=u64ba9c1f-5946-4a99-930d-637cfb8c411&title=)

- `**Sequencer**`**：** 核心管家
   - 定义生产同步的实现：`SingleProducerSequencer`单生产、`MultiProducerSequencer`多生产
   - 当前写的进度Sequence cursor
   - 所有消费者进度的数组`Sequence[] gatingSequences`
   - `MultiProducerSequencer`可用区`availableBuffer`【利用空间换取查询效率】
- **Sequence：** 本身就是一个序号器用来标识处理进度，也可以当做是一个`atomicInteger`; 还有另外一个特点，为了解决伪共享问题而引入的：缓存行填充。这个在后面介绍。
- **workProcessor：**处理Event的循环，在循环中获取Disruptor的事件，然后把事件分配给各个handler
- **EventHandler：**负责业务逻辑的handler，自己实现。
- **WaitStrategy：**消费者 如何等待 事件的策略，定义了如下策略
   - `SleepingWaitStrategy`：自旋 + yield + sleep
   - `BlockingWaitStrategy`：加锁，适合CPU资源紧张（不需要切换线程），系统吞吐量无要求的
   - `YieldingWaitStrategy`：自旋 + yield + 自旋
   - `BusySpinWaitStrategy`：自旋，减少线程之前切换
   - `PhasedBackoffWaitStrategy`：自旋 + yield + 自定义策略
<a name="pT3Ll"></a>
### 带着问题来解析代码？
**1、多生产者如何保证消息生产不会相互覆盖。【如何达到互斥效果】**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421063432-ff90511d-0e96-4afb-9b8d-644b7dfe3369.png#averageHue=%23fafafa&clientId=u9284130a-1c60-4&from=paste&id=uce55c2dd&originHeight=434&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud1fcfe8c-1d80-4911-9502-5e6172bc1b6&title=)<br />每个线程获取不同的一段数组空间，然后通过CAS判断这段空间是否已经分配出去。<br />接下来看下多生产类`MultiProducerSequencer`中`next`方法【获取生产序号】
```java
// 消费者上一次消费的最小序号 // 后续第二点会讲到
private final Sequence gatingSequenceCache = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);
// 当前进度的序号
protected final Sequence cursor = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);
// 所有消费者的序号 //后续第二点会讲到
protected volatile Sequence[] gatingSequences = new Sequence[0];

public long next(int n){
    if (n < 1){
        throw new IllegalArgumentException("n must be > 0");
    }
    long current;
    long next;
    do{
        // 当前进度的序号，Sequence的value具有可见性，保证多线程间线程之间能感知到可申请的最新值
        current = cursor.get();
        // 要申请的序号空间:最大序列号
        next = current + n;
    
        long wrapPoint = next - bufferSize;
        // 消费者最小序列号
        long cachedGatingSequence = gatingSequenceCache.get();
        // 大于一圈 || 最小消费序列号>当前进度
        if (wrapPoint > cachedGatingSequence || cachedGatingSequence > current){
            long gatingSequence = Util.getMinimumSequence(gatingSequences, current);
            // 说明大于1圈，并没有多余空间可以申请
            if (wrapPoint > gatingSequence){
                LockSupport.parkNanos(1); // TODO, should we spin based on the wait strategy?
                continue;
            }
            // 更新最小值到Sequence的value中
            gatingSequenceCache.set(gatingSequence);
        }
            // CAS成功后更新当前Sequence的value
        else if (cursor.compareAndSet(current, next)){
            break;
        }
    }
    while (true);
    return next;
}
```
**2、生产者向序号器申请写的序号，如序号正在被消费，**`**Sequencer**`**是如何知道哪些序号是可以被写入的呢？【未消费则被覆盖如何处理】**<br />从`gatingSequences`中取得最小的序号，生产者最多能写到这个序号的后一位。通俗来讲就是申请的序号不能大于最小消费者序号一圈【申请到最大序列号-buffersize 要小于/等于 最小消费的序列号】的时候， 才能申请到当前写的序号<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421063379-cdbbb8d4-3d74-4729-ad7a-d0a22fd84986.png#averageHue=%23fbfbfb&clientId=u9284130a-1c60-4&from=paste&id=u93bb3699&originHeight=420&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf45e399c-0ae7-45e7-ae18-89eebb3780f&title=)
```java
public final EventHandlerGroup<T> handleEventsWithWorkerPool(final WorkHandler<T>... workHandlers){
    return createWorkerPool(new Sequence[0], workHandlers);
}


EventHandlerGroup<T> createWorkerPool(final Sequence[] barrierSequences, final WorkHandler<? super T>[] workHandlers){
    final SequenceBarrier sequenceBarrier = ringBuffer.newBarrier(barrierSequences);
    final WorkerPool<T> workerPool = new WorkerPool<>(ringBuffer, sequenceBarrier, exceptionHandler, workHandlers);


    consumerRepository.add(workerPool, sequenceBarrier);

    final Sequence[] workerSequences = workerPool.getWorkerSequences();

    updateGatingSequencesForNextInChain(barrierSequences, workerSequences);

    return new EventHandlerGroup<>(this, consumerRepository, workerSequences);
}

private void updateGatingSequencesForNextInChain(final Sequence[] barrierSequences, final Sequence[] processorSequences){
    if (processorSequences.length > 0){
        // 消费者启动后就会将所有消费者存放入AbstractSequencer中gatingSequences
        ringBuffer.addGatingSequences(processorSequences);
        for (final Sequence barrierSequence : barrierSequences)
            {
                ringBuffer.removeGatingSequence(barrierSequence);
            }
        consumerRepository.unMarkEventProcessorsAsEndOfChain(barrierSequences);
    }
}
```
**3、在多生产者情况下，生产者是申请到一段可写入的序号，然后再写入这些序号中，那么消费者是如何感知哪些序号是可以被消费的呢？【借问提1图说明】**<br />这个前提是多生产者情况下，第一点说过每个线程获取不同的一段数组空间，那么现在单单通过序号已经不够用了，`MultiProducerSequencer`使用了int 数组 【`availableBuffer`】来标识当前序号是否可用。当生产者成功生产事件后会将`availableBuffer`中当前序列号置为1标识可以读取。<br />如此消费者可以读取的的最大序号就是`availableBuffer`中第一个不可用序号-1。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421063643-41d9a49f-3da7-4e05-a32e-b9900290912d.png#averageHue=%23fcfcfc&clientId=u9284130a-1c60-4&from=paste&id=uab227afa&originHeight=506&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5e95427f-4437-4e16-a44b-ef288a0633e&title=)<br />初始化`availableBuffer`流程
```java
public MultiProducerSequencer(int bufferSize, final WaitStrategy waitStrategy){
    super(bufferSize, waitStrategy);
    // 初始化可用数组
    availableBuffer = new int[bufferSize];
    indexMask = bufferSize - 1;
    indexShift = Util.log2(bufferSize);
    initialiseAvailableBuffer();
}
// 初始化默认availableBuffer为-1
private void initialiseAvailableBuffer(){
    for (int i = availableBuffer.length - 1; i != 0; i--)
        {
            setAvailableBufferValue(i, -1);
        }

    setAvailableBufferValue(0, -1);
}

// 生产者成功生产事件将可用区数组置为1
public void publish(final long sequence){
    setAvailable(sequence);
    waitStrategy.signalAllWhenBlocking();
}

private void setAvailableBufferValue(int index, int flag){
    long bufferAddress = (index * SCALE) + BASE;
    UNSAFE.putOrderedInt(availableBuffer, bufferAddress, flag);
}
```
消费者消费流程
```java
// WorkProcessor类中消费run方法
public void run(){
    boolean processedSequence = true;
    long cachedAvailableSequence = Long.MIN_VALUE;
    long nextSequence = sequence.get();
    T event = null;
    while (true){
        try{
            // 先通过cas获取消费事件的占有权
            if (processedSequence){
                processedSequence = false;
                do{
                    nextSequence = workSequence.get() + 1L;
                    sequence.set(nextSequence - 1L);
                }
                    while (!workSequence.compareAndSet(nextSequence - 1L, nextSequence));
            }
            // 数据就绪，可以消费
            if (cachedAvailableSequence >= nextSequence){
                event = ringBuffer.get(nextSequence);
                // 触发回调函数
                workHandler.onEvent(event);
                processedSequence = true;
            }else{
                // 获取可以被读取的下标
                cachedAvailableSequence = sequenceBarrier.waitFor(nextSequence);
            }
        }
        // ....省略
    }

    notifyShutdown();

    running.set(false);
}


public long waitFor(final long sequence) throws AlertException, InterruptedException, TimeoutException{
    checkAlert();
    // 这个值获取的current write 下标，可以认为全局消费下标。此处与每一段的write1和write2下标区分开
    long availableSequence = waitStrategy.waitFor(sequence, cursorSequence, dependentSequence, this);

    if (availableSequence < sequence){
        return availableSequence;
    }
    // 通过availableBuffer筛选出第一个不可用序号 -1
    return sequencer.getHighestPublishedSequence(sequence, availableSequence);
}

public long getHighestPublishedSequence(long lowerBound, long availableSequence){
    // 从current read下标开始， 循环至 current write,如果碰到availableBuffer 为-1 直接返回
    for (long sequence = lowerBound; sequence <= availableSequence; sequence++){
        if (!isAvailable(sequence)){
            return sequence - 1;
        }
    }

    return availableSequence;
}
```
<a name="ySOyU"></a>
### 解决伪共享问题
<a name="cj608"></a>
##### 什么是伪共享问题呢？
为了提高CPU的速度，Cpu有高速缓存Cache，该缓存最小单位为缓存行CacheLine，他是从主内存复制的Cache的最小单位，通常是64字节。一个Java的long类型是8字节，因此在一个缓存行中可以存8个long类型的变量。如果访问一个long数组，当数组中的一个值被加载到缓存中，它会额外加载另外7个。因此能非常快地遍历这个数组。<br />伪共享问题是指，当多个线程共享某份数据时，线程1可能拉到线程2的数据在其cache line中，此时线程1修改数据，线程2取其数据时就要重新从内存中拉取，两个线程互相影响，导致数据虽然在cache line中，每次却要去内存中拉取。<br />![2022-12-07-21-46-45.869177900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421654091-ad8ebb0d-4c10-4aa5-93d9-aaff7b0c6c5d.png#averageHue=%23f6f6f6&clientId=u9284130a-1c60-4&from=ui&id=ue325c44e&originHeight=1562&originWidth=1510&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7088949&status=done&style=none&taskId=ue7cbc886-79b8-44ca-b047-30029c4ea88&title=)
<a name="HA0nN"></a>
##### `Disruptor`是如何解决的呢？
在value前后统一都加入7个Long类型进行填充，线程拉取时，不论如何都会占满整个缓存<br />![2022-12-07-21-46-46.074187600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670421703140-e2cc58d5-5801-4cd0-b499-f8ab5b9346e4.png#averageHue=%232e2b2b&clientId=u9284130a-1c60-4&from=ui&id=ud114ee03&originHeight=1176&originWidth=1802&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6368998&status=done&style=none&taskId=ub26fc08f-0d00-42d2-8a52-9dbf754e249&title=)
<a name="WJTFs"></a>
### 回顾总结：`Disruptor`为何能称之为高性能的无锁队列框架呢？

- 缓存行填充，避免缓存频繁失效。【Java8中也引入`@sun.misc.Contended`注解来避免伪共享】
- 无锁竞争：通过CAS 【二阶段提交】
- 环形数组：数据都是覆盖，避免GC
- 底层更多的使用位运算来提升效率
