Java JUC AQS<br />为了解决原子性的问题，Java加入了锁机制，同时保证了可见性和顺序性。JDK1.5的并发包中新增了`Lock`接口以及相关实现类来实现锁功能，比`synchronized`更加灵活，开发者可根据实际的场景选择相应的实现类。这里注重讲解其不同衍生类的使用场景以及其内部AQS的原理。
<a name="XUp3N"></a>
## `Lock`特性
<a name="wDKWy"></a>
### 可重入
像`synchronized`和`ReentrantLock`都是可重入锁，可重入性表明了锁的分配机制是基于线程的分配，而不是基于方法调用的分配。<br />举个简单的例子，当一个线程已经获取到锁，当后续再获取同一个锁，直接获取成功。但获取锁和释放锁必须要成对出现。
<a name="Kvo9I"></a>
### 可响应中断
当线程因为获取锁而进入阻塞状态，外部是可以中断该线程的，调用方通过捕获`InterruptedException`可以捕获中断
<a name="enXo0"></a>
### 可设置超时时间
获取锁时，可以指定超时时间，可以通过返回值来判断是否成功获取锁
<a name="dyJYV"></a>
### 公平性
提供公平性锁和非公平锁（默认）两种选择。

- 公平锁，线程将按照他们发出请求的顺序来获取锁，不允许插队；
- 非公平锁，则允许插队：当一个线程发生获取锁的请求的时刻，如果这个锁是可用的，那这个线程将跳过所在队列里等待线程并获得锁。

考虑这么一种情况：A线程持有锁，B线程请求这个锁，因此B线程被挂起；A线程释放这个锁时，B线程将被唤醒，因此再次尝试获取锁；与此同时，C线程也请求获取这个锁，那么C线程很可能在B线程被完全唤醒之前获得、使用以及释放这个锁。这是种双赢的局面，B获取锁的时刻（B被唤醒后才能获取锁）并没有推迟，C更早地获取了锁，并且吞吐量也获得了提高。在大多数情况下，非公平锁的性能要高于公平锁的性能。<br />另外，这个公平性是针对线程而言的，不能依赖此来实现业务上的公平性，应该由开发者自己控制，比如通过FIFO队列来保证公布。
<a name="S7NC2"></a>
### 读写锁
允许读锁和写锁分离，读锁与写锁互斥，但是多个读锁可以共存，适用于读频次远大于写频次的场景
<a name="v84DL"></a>
### 丰富的API
提供了多个方法来获取锁相关的信息，可以帮助开发者监控和排查问题<br />`isFair()` //判断锁是否是公平锁 <br />`isLocked()` //判断锁是否被任何线程获取了 <br />`isHeldByCurrentThread()` //判断锁是否被当前线程获取了 <br />`hasQueuedThreads()` //判断是否有线程在等待该锁 <br />`getHoldCount()` //查询当前线程占有lock锁的次数 <br />`getQueueLength()` // 获取正在等待此锁的线程数
<a name="oaI5r"></a>
## 锁的使用
<a name="GT7KD"></a>
### `ReentrantLock`
独占锁的实现，拥有上面列举的除读写锁之外的所有特性，使用比较简单
```java
class X {
    // 创建独占锁实例
    private final ReentrantLock lock = new ReentrantLock();
    // ...

    public void m() {
        lock.lock();  // block until condition holds
        try {
            // ... method body
        } finally {
            // 必须要释放锁，unlock与lock成对出现
            lock.unlock()
            }
    }
}
```
<a name="ry7qZ"></a>
### `ReentrantReadWriteLock`
读写锁的实现，拥有上面列举的所有特性。并且写锁可降级为读锁，反之不行。
```java
class CachedData {
    Object data;
    volatile boolean cacheValid;
    final ReentrantReadWriteLock rwl = new ReentrantReadWriteLock();

    void processCachedData() {
        rwl.readLock().lock();
        if (!cacheValid) {
            // Must release read lock before acquiring write lock
            rwl.readLock().unlock();
            rwl.writeLock().lock();
            try {
                // Recheck state because another thread might have
                // acquired write lock and changed state before we did.
                if (!cacheValid) {
                    data = ...
                        cacheValid = true;
                }
                // Downgrade by acquiring read lock before releasing write lock
                rwl.readLock().lock();
            } finally {
                rwl.writeLock().unlock(); // Unlock write, still hold read
            }
        }

        try {
            use(data);
        } finally {
            rwl.readLock().unlock();
        }
    }
}
```
<a name="dgnYz"></a>
### `StampedLock`
`StampedLock`也是一种读写锁，提供两种读模式：乐观读和悲观读。乐观读允许读的过程中也可以获取写锁后写入！这样一来，读的数据就可能不一致，所以，需要一点额外的代码来判断读的过程中是否有写入。<br />乐观锁的意思就是乐观地估计读的过程中大概率不会有写入，因此被称为乐观锁。反过来，悲观锁则是读的过程中拒绝有写入，也就是写入必须等待。显然乐观锁的并发效率更高，但一旦有小概率的写入导致读取的数据不一致，需要能检测出来，再读一遍就行。
```java
public class Point {
    private final StampedLock stampedLock = new StampedLock();

    private double x;
    private double y;

    public void move(double deltaX, double deltaY) {
        long stamp = stampedLock.writeLock(); // 获取写锁
        try {
            x += deltaX;
            y += deltaY;
        } finally {
            stampedLock.unlockWrite(stamp); // 释放写锁
        }
    }

    public double distanceFromOrigin() {
        long stamp = stampedLock.tryOptimisticRead(); // 获得一个乐观读锁
        // 注意下面两行代码不是原子操作
        // 假设x,y = (100,200)
        double currentX = x;
        // 此处已读取到x=100，但x,y可能被写线程修改为(300,400)
        double currentY = y;
        // 此处已读取到y，如果没有写入，读取是正确的(100,200)
        // 如果有写入，读取是错误的(100,400)
        if (!stampedLock.validate(stamp)) { // 检查乐观读锁后是否有其他写锁发生
            stamp = stampedLock.readLock(); // 获取一个悲观读锁
            try {
                currentX = x;
                currentY = y;
            } finally {
                stampedLock.unlockRead(stamp); // 释放悲观读锁
            }
        }
        return Math.sqrt(currentX * currentX + currentY * currentY);
    }
}
```
<a name="sUVT0"></a>
### `Condition`
`Condition`成为条件队列或条件变量，为一个线程挂起执行（等待）提供了一种方法，直到另一线程通知某些状态条件现在可能为真为止。由于对该共享状态信息的访问发生在不同的线程中，因此必须由互斥锁对其其进行保护。<br />`await`方法：必须在获取锁之后的调用，表示释放当前锁，阻塞当前线程；等待其他线程调用锁的`signal`或`signalAll`方法，线程唤醒重新获取锁。<br />`Lock`配合`Condition`，可以实现`synchronized` 与 对象（`wait`，`notify`）同样的效果，来进行线程间基于共享变量的通信。但优势在于同一个锁可以由多个条件队列，当某个条件满足时，只需要唤醒对应的条件队列即可，避免无效的竞争。
```java
// 此类实现类似阻塞队列（ArrayBlockingQueue）
class BoundedBuffer {
    final Lock lock = new ReentrantLock();
    final Condition notFull  = lock.newCondition(); 
    final Condition notEmpty = lock.newCondition(); 

    final Object[] items = new Object[100];
    int putptr, takeptr, count;

    public void put(Object x) throws InterruptedException {
        lock.lock();
        try {
            while (count == items.length)
                notFull.await();
            items[putptr] = x;
            if (++putptr == items.length) putptr = 0;
            ++count;
            notEmpty.signal();
        } finally {
            lock.unlock();
        }
    }

    public Object take() throws InterruptedException {
        lock.lock();
        try {
            while (count == 0)
                notEmpty.await();
            Object x = items[takeptr];
            if (++takeptr == items.length) takeptr = 0;
            --count;
            notFull.signal();
            return x;
        } finally {
            lock.unlock();
        }
    }
}
```
<a name="Iu5Fl"></a>
### `BlockingQueue`
`BlockingQueue`阻塞队列实际上是一个生产者/消费者模型，当队列长度大于指定的最大值，生产线程就会被阻塞；反之当队列元素为空时，消费线程就会被阻塞；同时当消费成功时，就会唤醒阻塞的生产者线程；生产成功就会唤醒消费者线程；<br />内部使用就是`ReentrantLock` + `Condition`来实现的，可以参照上面的示例。
<a name="BtcHs"></a>
### `CountDownLatch`
称之为倒计时器锁，初始化指定数值，调用`countDown`可以对数值减一，当数值减为0时，就会唤醒所有因为调用`await`方法而阻塞的线程。<br />可以达到一组线程等待另外一组线程都完成任务的效果。
```java
class Driver { // ...
    void main() throws InterruptedException {
        CountDownLatch startSignal = new CountDownLatch(1);
        CountDownLatch doneSignal = new CountDownLatch(N);

        for (int i = 0; i < N; ++i) // create and start threads
            new Thread(new Worker(startSignal, doneSignal)).start();

        doSomethingElse();            // don't let run yet
        startSignal.countDown();      // let all threads proceed
        doSomethingElse();
        doneSignal.await();           // wait for all to finish
    }
}

class Worker implements Runnable {
    private final CountDownLatch startSignal;
    private final CountDownLatch doneSignal;
    Worker(CountDownLatch startSignal, CountDownLatch doneSignal) {
        this.startSignal = startSignal;
        this.doneSignal = doneSignal;
    }
    public void run() {
        try {
            startSignal.await();
            doWork();
            doneSignal.countDown();
        } catch (InterruptedException ex) {} // return;
    }

    void doWork() { ... }
}
```
<a name="C6CHN"></a>
### `CyclicBarrier`
称之为同步屏障，它使得一组线程互相等待，直到到达某个公共屏障点。<br />初始化指定数值，调用`await`方法会使得线程阻塞，直到指定数量的线程都调用`await`方法时，所有被阻塞的线程会被唤醒，继续执行。<br />与`CountDownLatch`的区别是，`CountDownLatch`是一组线程等待另外一组线程，而`CyclicBarrier`是一组线程之间相互等待。
<a name="CEpM1"></a>
### `Semaphore`
称之为信号量，与互斥锁`ReentrantLock`用法类似，区别就是`Semaphore`共享的资源是多个，允许多个线程同时竞争成功。
<a name="iB3fy"></a>
## `AQS`原理
`AQS`是`AbstractQueuedSynchronizer`的缩写，中文 抽象队列同步器，是构建各类锁和同步器的基础实现。内部维护了共享变量state （int类型） 和 双向队列 （包含头指针和尾指针）<br />![2021-05-05-16-33-10-986225.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204414337-42a064d3-685b-4b6a-80f7-2e8f0487ed22.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=u954a38cd&originHeight=305&originWidth=640&originalType=binary&size=13041&status=done&style=shadow&taskId=u85d65159-d53e-4fa7-8546-5a6d4b0a13c)
<a name="LIRSl"></a>
### 并发问题解决
<a name="s9WT1"></a>
#### 原子性
`Unsafe.compareAndSwapXXX` 实现CAS更改 state 和 队列指针 内部依赖CPU提供的原子指令
<a name="Pvfnu"></a>
#### 可见性与有序性
`volatile` 修饰 `state` 与 队列指针 （`prev`/`next`/`head`/`tail`）
<a name="ydgnx"></a>
#### 线程阻塞与唤醒
`Unsafe.park` `Unsafe.parkNanos` `Unsafe.unpark`
> Unsafe类是在sun.misc包下，不属于Java标准。提供了内存管理、对象实例化、数组操作、CAS操作、线程挂起与恢复等功能，Unsafe类提升了Java运行效率，增强了Java语言底层的操作能力。很多Java的基础类库，包括一些被广泛使用的高性能开发库都是基于Unsafe类开发的，比如Netty、Cassandra、Hadoop、Kafka等

<a name="YpPj2"></a>
#### AQS内部有两种模式：独占模式和共享模式
AQS 的设计是基于模板方法的，使用者需要继承 AQS 并重写指定的方法。不同的自定义同步器争用共享资源的方式不同，比如可重入、公平性等都是子类来实现。<br />自定义同步器在实现时只需要实现共享资源state的获取与释放方式即可，至于具体线程等待队列的维护（如获取资源失败入队/唤醒出队等），由AQS内部处理。
<a name="IoOvY"></a>
### 独占模式

- 只有一个线程都能够获取到锁
- 锁释放后需要唤醒后继节点
<a name="vlFrc"></a>
#### AQS提供的独占模式相关的方法
```java
// 获取独占锁（线程阻塞直至获取成功）
public final void acquire(int)
// 获取独占锁，可被中断
public final void acquireInterruptibly(int) 
// 获取独占锁，可被中断 和 指定超时时间
public final boolean tryAcquireNanos(int, long) 
// 释放独占锁（释放锁后，将等待队列中第一个等待节点唤醒 ）
public final boolean release(int) 
```
<a name="X7ZFp"></a>
#### AQS子类需要实现的独占模式相关的方法
```java
// 尝试获取独占锁
protected boolean tryAcquire(int)
// 尝试释放独占锁
protected boolean tryRelease(int)
```
<a name="CpgDx"></a>
#### 获取独占锁的流程
![2021-05-05-16-33-11-250669.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204457439-955c8a89-550a-4c8e-ac18-10f7e181e581.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=u9e04e41e&originHeight=343&originWidth=640&originalType=binary&size=15767&status=done&style=shadow&taskId=u0255ff72-bb0c-43b4-8b98-9681a400e54)![2021-05-05-16-33-11-396279.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204457436-62dc81ea-f4c5-453c-acbc-b9b4700ab0eb.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=u05c782af&originHeight=340&originWidth=640&originalType=binary&size=12066&status=done&style=shadow&taskId=ud70b707c-f084-4d39-b0d2-59b25e57d53)

- 调用子类`tryAcquire`尝试获取锁，获取成功，直接返回
- 通过自旋CAS将当前线程封装成节点加入队列末尾
- 循环等待或尝试`tryAcquire`获取锁
   - 判断前置节点如果为head，则尝试获取锁
   - 根据队列中节点状态，决定是否需要阻塞当前线程
   - `tryAcquire`获取锁成功后，将当前节点设置为`head` 并 返回
- 如果当前线程中断或超时，则执行`cancelAcquire`
   - 将当前节点状态置为`CANCELED`，并从队列删除
   - 如果前置节点为`Head`，则将后置节点唤醒

释放独占锁的流程<br />![2021-05-05-16-33-11-666624.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204501573-25cbfe87-105d-4cf3-8537-b0296921685a.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=udc532570&originHeight=313&originWidth=640&originalType=binary&size=10592&status=done&style=shadow&taskId=u7dedbe9e-03bf-4e84-ab3a-08d5eaa74cc)
<a name="q7Ahq"></a>
### 共享模式

- 多个线程都能够获取到锁
- 锁释放后需要唤醒后继节点
- 锁获取后如果还有资源需要唤醒后继共享节点
<a name="Cm2ZF"></a>
#### AQS提供的共享模式相关的方法
```java
// 获取共享锁（线程阻塞直至获取成功）
public final void acquireShared(int) 
// 获取共享锁，可被中断
public final acquireSharedInterruptibly(int) 
// 获取共享锁，可被中断 和 指定超时时间
public final tryAcquireSharedNanos(int, long)  
// 获取共享锁
public final boolean releaseShared(int) 
```
<a name="qoaZl"></a>
#### AQS子类需要实现的共享模式相关的方法
```java
// 尝试获取共享锁
protected int tryAcquireShared(int)
// 尝试释放共享锁
protected boolean tryReleaseShared(int) 
```
<a name="PuHrm"></a>
#### 获取共享锁的流程
![2021-05-05-16-33-11-867085.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204260509-35099d58-1f43-4ac7-bcf9-40ad53f364c8.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=ueb9ec2ed&originHeight=314&originWidth=640&originalType=binary&size=16479&status=done&style=shadow&taskId=u40a69c38-8f26-4836-9197-03ae815ba1f)

- 调用子类`tryAcquireShared`尝试获取锁，获取成功，直接返回
- 通过自旋CAS将当前线程封装成节点加入队列末尾
- 循环等待或尝试`tryAcquireShared`获取锁
   - 如果资源有剩余或者原先的head节点状态为`SIGNAL`/`PROPAGATE`，则调用`doReleaseShared`
   - 如果当前`head`节点状态为`SIGNAL`，唤醒后继节点
   - 如果当前`head`节点状态为`ZERO`，将head节点状态置为`PROPAGATE`
   - 判断前置节点如果为`head`，则尝试获取锁
   - 根据队列中节点状态，决定是否需要阻塞当前线程
   - `tryAcquireShared`获取锁成功后，将当前节点设置为`head`
- 如果当前线程中断或超时，则执行`cancelAcquire`
   - 将当前节点状态置为`CANCELED`，并从队列删除
   - 如果前置节点为`Head`，则将后置节点唤醒
<a name="WofGE"></a>
#### 释放共享锁的流程
![2021-05-05-16-33-12-071059.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204235745-59a68686-6ea2-4b02-8619-68a1c5a62cfb.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=uc8aa96aa&originHeight=268&originWidth=640&originalType=binary&size=10891&status=done&style=shadow&taskId=u60e590d4-f723-484f-93a1-75c32d25d3d)
<a name="asbeg"></a>
#### 等待队列中节点的状态变化
![2021-05-05-16-33-12-282417.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204247556-72dd8cd9-4e57-468c-981a-5a7bebc350b9.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=u72773db4&originHeight=203&originWidth=640&originalType=binary&size=12872&status=done&style=shadow&taskId=u4a17f373-998d-4e95-92c3-e5a1453168c)
<a name="PsWSq"></a>
### `ReentrantLock`示例
<a name="F47PN"></a>
#### `tryAcquire`逻辑
![2021-05-05-16-33-12-486388.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204210544-d4f07225-ff69-4cae-92ea-fe898ccf4999.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=uadbadc8f&originHeight=244&originWidth=640&originalType=binary&size=9965&status=done&style=shadow&taskId=ua6488ff6-591b-4488-9fb1-76c70f733ab)
<a name="xYcFw"></a>
#### `tryRelease`逻辑
![2021-05-05-16-33-12-685846.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620204219542-28c0a66f-a297-4132-91d5-21315259f942.jpeg#clientId=u86738d50-f7bc-4&from=ui&id=u8ba24e2b&originHeight=240&originWidth=640&originalType=binary&size=8812&status=done&style=shadow&taskId=u05d7b05f-88d2-4fd7-a6ec-108cd59c0f3)
