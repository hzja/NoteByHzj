Java 队列<br />`BlockingQueue`，是  java.util.concurrent  包提供的用于解决并发生产者 - 消费者问题的最有用的类。<br />它的特性是在任意时刻只有一个线程可以进行take或者put操作，并且`BlockingQueue`提供了超时`return null`的机制，在许多生产场景里都可以看到这个工具的身影。
<a name="LNqRz"></a>
## 队列类型
1.无限队列 （unbounded queue ） - 几乎可以无限增长<br />2.有限队列 （ bounded queue ） - 定义了最大容量
<a name="QjUSY"></a>
## 队列数据结构
队列实质就是一种存储数据的结构

- 通常用链表或者数组实现
- 一般而言队列具备FIFO先进先出的特性，当然也有双端队列（Deque）优先级队列
- 主要操作：入队（EnQueue）与出队（Dequeue）

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623854567223-f9621c8a-4471-45e9-9d13-86e0bd30e782.webp#clientId=u51f983dd-3dcf-4&from=paste&id=uaefc5fd1&originHeight=230&originWidth=743&originalType=url&ratio=3&status=done&style=shadow&taskId=u532b3b1d-593f-436f-ab3e-1c79147664d)
<a name="eXhql"></a>
## 常见的4中阻塞队列

- `ArrayBlockingQueue` 由数组支持的有界队列
- `LinkedBlockingQueue` 由链接节点支持的可选有界队列
- `PriorityBlockingQueue` 由优先级堆支持的无界优先级队列
- `DelayQueue` 由优先级堆支持的、基于时间的调度队列
<a name="fO4w6"></a>
### 1、`ArrayBlockingQueue`
队列基于数组实现，容量大小在创建`ArrayBlockingQueue`对象时已定义好数据结构如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623854567193-65900daa-fc7d-4480-8b7f-1cfeb222b5db.webp#clientId=u51f983dd-3dcf-4&from=paste&id=u20be6b99&originHeight=240&originWidth=761&originalType=url&ratio=3&status=done&style=shadow&taskId=ue1b86436-5471-4eda-8ee2-a39a5a623e8)
<a name="rIl9w"></a>
#### ①队列创建：
```java
BlockingQueue<String> blockingQueue = new ArrayBlockingQueue<>();
```
<a name="SuGAG"></a>
#### ②应用场景：
在线程池中有比较多的应用，生产者消费者场景
<a name="Jx2Fm"></a>
#### ③工作原理：
基于`ReentrantLock`保证线程安全，根据`Condition`实现队列满时的阻塞
<a name="fl13u"></a>
### 2、`LinkedBlockingQueue`
是一个基于链表的无界队列(理论上有界)
```java
BlockingQueue<String> blockingQueue = new LinkedBlockingQueue<>();
```
上面这段代码中，`blockingQueue` 的容量将设置为 `Integer.MAX_VALUE` 。<br />向无限队列添加元素的所有操作都将永远不会阻塞，[注意这里不是说不会加锁保证线程安全]，因此它可以增长到非常大的容量。<br />使用无限 `BlockingQueue` 设计生产者 - 消费者模型时最重要的是 消费者应该能够像生产者向队列添加消息一样快地消费消息 。<br />否则，内存可能会填满，然后就会得到一个 OutOfMemory 异常。
<a name="xnyp0"></a>
### 3、`DelayQueue `
由优先级堆支持的、基于时间的调度队列，内部基于无界队列`PriorityQueue`实现，而无界队列基于数组的扩容实现。
<a name="hJnqh"></a>
#### ①队列创建：
```java
BlockingQueue<String> blockingQueue = new DelayQueue();      
```
<a name="jW9ZF"></a>
#### ②要求：
入队的对象必须要实现Delayed接口，而Delayed集成自Comparable接口
<a name="CDXuh"></a>
#### ③应用场景：
电影票
<a name="Jw9lB"></a>
#### ④工作原理：
队列内部会根据时间优先级进行排序。延迟类线程池周期执行。
<a name="lKtIs"></a>
### 4、BlockingQueue API
`BlockingQueue` 接口的所有方法可以分为两大类：负责向队列添加元素的方法和检索这些元素的方法。在队列满/空的情况下，来自这两个组的每个方法的行为都不同。
<a name="jLry5"></a>
#### ①添加元素
| 方法 | 说明 |
| --- | --- |
| `add()` | 如果插入成功则返回 true，否则抛出 IllegalStateException 异常 |
| `put()` | 将指定的元素插入队列，如果队列满了，那么会阻塞直到有空间插入 |
| `offer()` | 如果插入成功则返回 true，否则返回 false |
| `offer(E e, long timeout, TimeUnit unit)` | 尝试将元素插入队列，如果队列已满，那么会阻塞直到有空间插入 |

<a name="wA0h1"></a>
#### ②检索元素
| 方法 | 说明 |
| --- | --- |
| `take()` | 获取队列的头部元素并将其删除，如果队列为空，则阻塞并等待元素变为可用 |
| `poll(long timeout, TimeUnit unit)` | 检索并删除队列的头部，如有必要，等待指定的等待时间以使元素可用，如果超时，则返回 null |

在构建生产者 - 消费者程序时，这些方法是 `BlockingQueue` 接口中最重要的构建块。
<a name="Vk5B4"></a>
## 多线程生产者-消费者示例
接下来创建一个由两部分组成的程序 - 生产者 ( Producer ) 和消费者 ( Consumer ) 。<br />生产者将生成一个 0 到 100 的随机数(十全大补丸的编号)，并将该数字放在 BlockingQueue 中。<br />创建 16 个线程（潘金莲）用于生成随机数并使用 put() 方法阻塞，直到队列中有可用空间。<br />需要记住的重要一点是，需要阻止消费者线程无限期地等待元素出现在队列中。<br />从生产者(潘金莲)向消费者(武大郎)发出信号的好方法是，不需要处理消息，而是发送称为毒 （ poison ） 丸 （ pill ） 的特殊消息。<br />需要发送尽可能多的毒 （ poison ） 丸 （ pill ） ，因为有消费者(武大郎)。<br />然后当消费者从队列中获取特殊的毒 （ poison ） 丸 （ pill ）消息时，它将优雅地完成执行。<br />以下生产者的代码：
```java
@Slf4j
public class NumbersProducer implements Runnable {
    private final int poisonPill;
    private final int poisonPillPerProducer;
    private BlockingQueue<Integer> numbersQueue;
    public NumbersProducer(BlockingQueue<Integer> numbersQueue,
                           int poisonPill,
                           int poisonPillPerProducer) {
        this.numbersQueue = numbersQueue;
        this.poisonPill = poisonPill;
        this.poisonPillPerProducer = poisonPillPerProducer;
    }
    @Override
    public void run() {
        try {
            generateNumbers ();
        } catch (InterruptedException e) {
            Thread.currentThread ().interrupt ();
        }
    }
    private void generateNumbers() throws InterruptedException {
        for (int i = 0; i < 100; i++) {
            numbersQueue.put (ThreadLocalRandom.current ().nextInt (100));
            log.info ("潘金莲-{}号,给武大郎的泡药！", Thread.currentThread ().getId ());
        }
        for (int j = 0; j < poisonPillPerProducer; j++) {
            numbersQueue.put (poisonPill);
            log.info ("潘金莲-{}号,往武大郎的药里放入第{}颗毒丸！", Thread.currentThread ().getId (), j + 1);
        }
    }
}
```
生成器构造函数将 `BlockingQueue` 作为参数，用于协调生产者和使用者之间的处理。<br />可以看到方法 `generateNumbers()` 将 100 个元素（生产100副药给武大郎吃）放入队列中。<br />它还需要有毒 （ poison ） 丸 （ pill ） 消息，以便知道在执行完成时放入队列的消息类型。<br />该消息需要将 poisonPillPerProducer 次放入队列中。<br />每个消费者将使用 `take()` 方法从 `BlockingQueue` 获取一个元素，因此它将阻塞，直到队列中有一个元素。<br />从队列中取出一个 Integer 后，它会检查该消息是否是毒 （ poison ） 丸 （ pill ），如果是，则完成一个线程的执行。<br />否则，它将在标准输出上打印出结果以及当前线程的名称。
```java
@Slf4j
public class NumbersConsumer implements Runnable {
    private final int poisonPill;
    private BlockingQueue<Integer> queue;
    public NumbersConsumer(BlockingQueue<Integer> queue, int poisonPill) {
        this.queue = queue;
        this.poisonPill = poisonPill;
    }
    @Override
    public void run() {
        try {
            while (true) {
                Integer number = queue.take ();
                if (number.equals (poisonPill)) {
                    return;
                }
                log.info ("武大郎-{}号,喝药-编号:{}", Thread.currentThread ().getId (), number);
            }
        } catch (InterruptedException e) {
            Thread.currentThread ().interrupt ();
        }
    }
}
```
需要注意的重要事项是队列的使用。<br />与生成器构造函数中的相同，队列作为参数传递。<br />可以这样做，是因为 `BlockingQueue` 可以在线程之间共享而无需任何显式同步。<br />既然有生产者和消费者，就可以开始计划。需要定义队列的容量，并将其设置为 10个元素。<br />创建4 个生产者线程，并且创建等于可用处理器数量的消费者线程：
```java

public class Main {
    public static void main(String[] args) {
        int BOUND = 10;
        int N_PRODUCERS = 16;
        int N_CONSUMERS = Runtime.getRuntime ().availableProcessors ();
        int poisonPill = Integer.MAX_VALUE;
        int poisonPillPerProducer = N_CONSUMERS / N_PRODUCERS;
        int mod = N_CONSUMERS % N_PRODUCERS;
        BlockingQueue<Integer> queue = new LinkedBlockingQueue<> (BOUND);
        //潘金莲给武大郎熬药
        for (int i = 1; i < N_PRODUCERS; i++) {
            new Thread (new NumbersProducer (queue, poisonPill, poisonPillPerProducer)).start ();
        }
        //武大郎开始喝药
        for (int j = 0; j < N_CONSUMERS; j++) {
            new Thread (new NumbersConsumer (queue, poisonPill)).start ();
        }
        //潘金莲开始投毒，武大郎喝完毒药GG
        new Thread (new NumbersProducer (queue, poisonPill, poisonPillPerProducer + mod)).start ();
    }
}
```
`BlockingQueue` 是使用具有容量的构造创建的。正在创造 4 个生产者和 N 个消费者（武大郎）。<br />将毒 （ poison ） 丸 （ pill ）消息指定为 `Integer.MAX_VALUE`，因为生产者在正常工作条件下永远不会发送这样的值。<br />这里要注意的最重要的事情是 `BlockingQueue` 用于协调它们之间的工作。
