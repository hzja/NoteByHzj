Java AQS
<a name="p6JmQ"></a>
## **AQS的背景由来**
在JDK1.5的时候就引入了JUC包，也就是从这一个包开始，提供了一系列支持并发的组件。这一些组件呢是通过一个叫同步器来维护着的，同步器主要是维护这几个功能：第一个是同步状态的管理，第二个是同步状态的更新和检查。下面来看看juc是由什么组件构成的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647668541167-d522be04-fd09-403e-a794-c6e8832bd269.png#clientId=u6057d50f-108f-4&from=paste&id=u26629459&originHeight=841&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue8a88b83-7c26-45df-b3fe-d4b3d2aa797&title=)<br />同步器可以用来实现其他形式的同步器。就好比如用可重入锁实现了信号量，反之信号量也能实现可重入锁，这样的方式会明显具有不灵活性、复杂性。所以的话通过AQS这个组件来构建juc的工具类，这就是AQS的产生背景了。
<a name="nYx3P"></a>
## **AQS的结构**
分析下面的AQS结构可以看出来有同步器的状态变更操作，线程阻塞和释放操作、插入和移出队列操作。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647668541284-5f0247db-3589-48f0-97e3-fbc9d116085d.png#clientId=u6057d50f-108f-4&from=paste&id=u116f062a&originHeight=563&originWidth=761&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uff6ea48e-1a13-42d8-b02d-0efc863ca74&title=)
<a name="Afnkm"></a>
### 同步状态：
AQS类是使用了单个int来保存同步状态的，并且把get/setState和`compareAndSet`暴露出去，来读取与更新这个同步状态。这里面的state是volatile，并且又通过CAS指令来实现了`compareAndSetState`，使得当同步状态同步的状态有一个一致的期望值的时候，才会被原子设置新值。这样就可以达到了同步状态的原子性管理，确保了同步状态的原子性，可见性和有序性。
<a name="nYxO5"></a>
### **阻塞**：
阻塞和解除线程它都是属于Java内的内置管程，没有其他不是Java内置的API可以让线程阻塞和解除线程阻塞。
<a name="eJ6MP"></a>
### 队列：
整个框架的核心就是怎么管理这个线程阻塞队列，这个队列是先进先出队列，所以的话是不支持线程的优先级同步。同步队列自身没有使用到底层的锁来构造不是阻塞的数据结构。<br />入队操作：因为CLH队列是FIFO队列，所以新节点来的时候，是需要插入到队列的尾结点。<br />出队操作：遵循FIFO的规则，首节点在释放线程的同步状态时，会把后续的节点唤醒，而后续的节点再获得AQS同步状态，所以的话在设置首节点就不需要像入队那样操作。只需要把首节点设置为源节点的后续节点，同时又断开源节点后续节点的引用即可。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647668541307-36e42ceb-27e0-4ee7-b8d8-d4e76a7cc495.png#clientId=u6057d50f-108f-4&from=paste&id=ueb308fa6&originHeight=721&originWidth=960&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubfb40d5f-0c96-471c-ac67-138e8503211&title=)<br />**条件队列：** 队列除了有同步队列、还有条件队列。AQS只可以有一个同步队列，但是可以有多个条件队列，有一个ConditionObject类，给维护同步的类使用。ConditionObject类实现了Condition接口，这个接口提供了Object管程式方法，还有超时、检测和监控的这些方法。ConditionObject将条件跟其他的同步操作结合了。<br />ConditionObject和AQS是共用的内部节点，有属于自己的条件队列。
<a name="jy23H"></a>
### **方法结构**
| **组件** | **数据结构** |
| --- | --- |
| 同步状态 | volatile int state |
| 阻塞 | LockSupport类 |
| 队列 | Node节点 |
| 条件队列 | ConditionObject |

<a name="zi5AC"></a>
## **独占式同步状态**
独占式的同步状态是调用了acquire，主要是完成同步状态的获取、加入同步队列。同步队列自旋等相关的操作，逻辑是这样的：首先是调用了子类实现tryAcquire的方法，这个方法可以保证线程的安全获取同步状态，这时候如果是获取失败的话，就构造了独占式的同步节点，然后再通过addwaiter方法把这个节点加入到同步队列的尾部，最后调用了acquireQueued方法，就让该节点自旋方式获取同步状态。
```java
public final void acquire(int arg) {
    if (!tryAcquire(arg) &&
        acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
        selfInterrupt();
    }
```
`tryAcquire()`尝试直接去获取资源，如果成功则直接返回，AQS里面未实现但没有定义成abstract，因为独占模式下只用实现tryAcquire-tryRelease，而共享模式下只用实现`tryAcquireShared`-`tryReleaseShared`，类似设计模式里面的适配器模式<br />`addWaiter()` 根据不同模式将线程加入等待队列的尾部，有`Node.EXCLUSIVE`互斥模式、`Node.SHARED`共享模式；如果队列不为空，则以通过`compareAndSetTail`方法以CAS将当前线程节点加入到等待队列的末尾。否则通过`enq(node)`方法初始化一个等待队列<br />`acquireQueued()`使线程在等待队列中获取资源，一直获取到资源后才返回，如果在等待过程中被中断，则返回true，否则返回false
<a name="roIto"></a>
### 节点自旋
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647668541195-2325c7e9-dd4b-47ba-8604-43182fb483cf.png#clientId=u6057d50f-108f-4&from=paste&id=u02c051d2&originHeight=372&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue5368f0e-2fd4-4252-a071-ec9ed5acb49&title=)
<a name="KIc8g"></a>
### 独占式获取同步状态流程图
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647668541314-9bc68192-aa27-4e07-aec5-7a7bfd4b25e6.png#clientId=u6057d50f-108f-4&from=paste&id=u7b7d5607&originHeight=761&originWidth=884&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2724ed3f-c80e-41d2-b32e-2baa193b61b&title=)
<a name="HehQW"></a>
## **AQS的应用**

- **ReentrantLock：**ReentrantLock使用AQS同步状态来存储锁重复持有的次数。当锁被线程获取时，ReentrantLock也会记录下当前获得锁的线程标识，以检查是否是重复获取，以及当错误的线程试图进行解锁操作时检测是否存在非法状态异常。
- **ReentrantReadWriteLock：**ReentrantReadWriteLock类使用AQS同步状态中来保存写锁持有的次数和读锁的持有次数。ReadLock则通过使用acquireShared方法来支持多个读线程。
- **Semaphore：**Semaphore类（信号量）使用AQS同步状态来保存信号量的当前计数。它里面定义的acquireShared方法会减少计数，或当计数为非正值时阻塞线程；tryRelease方法会增加计数，在计数为正值时还要解除线程的阻塞。
- **CountDownLatch：**CountDownLatch使用AQS同步状态来表示计数。当该计数为0时，所有的acquire操作对应到CountDownLatch中才能通过。
- **FutureTask：**FutureTask类使用AQS同步状态来表示某个异步计算任务的运行状态（初始化、运行中、被取消和完成）。设置（FutureTask的set方法）或取消（FutureTask的cancel方法）一个FutureTask时会调用AQS的release操作，等待计算结果的线程的阻塞解除是通过AQS的acquire操作实现的。
- **SynchronousQueues：**SynchronousQueues类使用了内部的等待节点，这些节点可以用于协调生产者和消费者。同时，它使用AQS同步状态来控制当某个消费者消费当前一项时，允许一个生产者继续生产，反之亦然。
