Java ReentrantLock<br />同步锁`synchronized`和重入锁`ReentrantLock`都是用于并发程序设计必不可少的手段，在JDK 5.0早期版本中，同步锁性能远远低于重入锁，但是在6.0版本之后，jdk对同步锁做了大量的优化，使得同步锁跟重入锁性能差距并不大，并且jdk团队表示，同步锁还有进一步升级优化的空间
<a name="GQgTL"></a>
## 一、重入锁的概念
重入的概念是指在同一个线程内部，这种锁是可以反复进入的。
```java
lock.lock();
lock.lock();
// do something
lock.unlock();
lock.unlock();
```
一个线程可多次获取锁，但同时也要释放相同的次数，否则该线程将持续拥有锁，其他线程将无法进入临界区。
<a name="fYned"></a>
## 二、重入锁的几个重要方法
![2021-10-01-11-42-00-141740.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633059982846-4c9cfc75-ffc0-4b5a-bf90-0d4af07eaa4f.jpeg#clientId=u03fe1928-9540-4&from=ui&id=u53020a21&originHeight=324&originWidth=640&originalType=binary&ratio=1&size=23251&status=done&style=none&taskId=u4bb42892-7166-4929-b75d-d3b27839367)<br />`lock`：获取锁，如果锁被其他线程占用，则休眠等待。<br />`lockInterruptibly`：获取锁，可以被其他线程所中断<br />`tryLock`：尝试获取锁，不等待<br />`tryLock(time, timeUnit)`:在一定时间内尝试获取锁<br />`unlock`: 释放锁
<a name="cbNS9"></a>
### 2.1 中断响应
对于`synchronized`来说，一个线程要么获取到锁开始执行，要么继续等待。但是对于重入锁来说，提供了更灵活的一种机制，那就是在等待锁的过程中，可以取消对锁的请求，这样可以有效避免死锁的可能。
<a name="GClYb"></a>
### 2.2 锁申请等待时间
中断响应是一种通过外部通知中断对锁的请求，从而避免死锁的一种机制。除此之外，还有一种机制，那就是等待限时。
```java
tryLock(long timeout, TimeUnit unit)
```
<a name="plTaY"></a>
### 2.3 公平锁、非公平锁
重入锁默认是非公平锁
```java
public ReentrantLock() {
     sync = new NonfairSync();
}
```
可以通过构造函数参数实现公平锁
```java
public ReentrantLock(boolean fair) {
     sync = fair ? new FairSync() : new NonfairSync();
}
```
如果是非公平锁，在并发场景下，系统会随机从等待队列中挑选一个线程。如果是公平锁，系统会维护一个有序队列，会按照进入队列的次序有序执行，因此公平锁虽然避免了饥饿现象，但是会需要更高的成本来维护这个有序队列。
<a name="xgq9N"></a>
### 2.4 AQS的组成结构
重入锁的加锁和解锁过程主要有AQS完成，AQS维护了一个双向链表，每个节点Node存储一个线程及线程的状态，Head节点代表正在持有锁的线程。<br />![2021-10-01-11-42-00-340745.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633059982867-44788fc3-e54a-4a1d-b2de-e480d1e7d72c.jpeg#clientId=u03fe1928-9540-4&from=ui&id=vO7fJ&originHeight=287&originWidth=640&originalType=binary&ratio=1&size=11545&status=done&style=shadow&taskId=u09f29fe1-5d34-4c3a-9fe9-d56751d0fa8)<br />节点状态：<br />`CANCELLED`: 当前节点超时或者中断被取消<br />`SIGNAL`：当前节点的后续节点处于等待状态<br />`CONDITION`：当前节点再等待condition<br />`PROPAGATE`：状态向后传播
```java
static final class Node {
    /** waitStatus状态：当前节点被取消调度*/
    static final int CANCELLED =  1;
    /** waitStatus状态：后续节点等待被唤醒*/
    static final int SIGNAL    = -1;
    /** waitStatus状态：当前节点等待在condition上，
          * 当其他线程调用了Condition的signal()方法后，
          * CONDITION状态的结点将从等待队列转移到同步队列中，
          * 等待获取同步锁*/
    static final int CONDITION = -2;
    /** waitStatus状态：共享模式下，会唤醒后续节点及后续的后续节点*/
    static final int PROPAGATE = -3;

    /** 节点状态*/
    volatile int waitStatus;
}
```
当线程获取锁失败之后，就通过addWaiter加入到同步队列中（加入到尾部），自旋判断自己是否是链表的头节点，如果是头节点，就不断参试获取资源，获取成功后则退出同步队列。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633059689690-529a15ed-0db5-4f2e-a525-43b8afaec5ba.webp#clientId=u03fe1928-9540-4&from=paste&id=u2957d89f&originHeight=255&originWidth=640&originalType=url&ratio=1&status=done&style=shadow&taskId=u87132ca1-7ede-4cda-b642-1dea01a16a7)
<a name="DpXGh"></a>
## 三、重入锁的实现原理
首先看下重入锁的类图关系<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633059689753-20049ab2-7dac-47ba-869a-5e69226653ff.webp#clientId=u03fe1928-9540-4&from=paste&id=u3c4abdb4&originHeight=368&originWidth=640&originalType=url&ratio=1&status=done&style=shadow&taskId=uca6586d7-518d-4077-b2d0-c2d0335d3cf)<br />`ReentrantLock`定义了内部类`Sync`，`Sync`继承自`AbstractQueuedSynchronizer`（简称AQS），是一个同步等待队列，本质上是一个带有头尾指针的双向链表。<br />AQS中有一个很重要的变量, 不同的组件中代表的含义有所不同，而在重入锁组件中，代表被线程重入的次数，为0表示没有线程持有锁。
```java
private volatile int state;
```
<a name="w3gkR"></a>
### 3.1 公平锁的加锁过程
`sync.lock`时调用AQS的`acquire`方法，这是一种模板设计模式，即AQS中定义了整体的处理流程，但是具体的实现细节会根据锁类型的不同，放到子类方法中执行。
```java
public final void acquire(int arg) {
    // 尝试获取锁
    if (!tryAcquire(arg) &&
        // 获取锁失败的线程安全的加入到等待队列中
        acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
        // 自行了断
        selfInterrupt();
}
```
下面来看看每一步的实现细节
<a name="DBJxt"></a>
#### 公平锁的`tryAcquire`
`tryAcquire`是一个钩子方法。实现细节放在具体的子类中。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633059689772-28ac5b43-230c-4c87-890d-ca306a43a23d.webp#clientId=u03fe1928-9540-4&from=paste&id=u26643fa6&originHeight=852&originWidth=640&originalType=url&ratio=1&status=done&style=shadow&taskId=u839c3848-7e94-4080-992d-eb224b7914d)
```java
protected final boolean tryAcquire(int acquires) {
    final Thread current = Thread.currentThread();
    //获取当前锁的状态
    int c = getState();
    // 没有线程占用
    if (c == 0) {
        // 如果当前线程排在队列中的第一个，并且CAS抢占锁成功
        if (!hasQueuedPredecessors() &&
            compareAndSetState(0, acquires)) {
            // 设置占用锁的线程为当前线程
            setExclusiveOwnerThread(current);
            return true;
        }
    }
    // 判断占有锁的线程是否是当前线程
    else if (current == getExclusiveOwnerThread()) {
        int nextc = c + acquires;
        if (nextc < 0)
            throw new Error("Maximum lock count exceeded");
        // 重入次数加1，acquires是由acquire(1)传入
        setState(nextc);
        return true;
    }
    return false;
}
```
<a name="mNBei"></a>
#### `acquireQueued(addWaiter())`
线程在同步队列中会尝试获取锁,失败则被阻塞,被唤醒后会不停的重复这个过程,直到线程真正持有了锁,并将自身结点置于队列头部。对算法感兴趣的同学可以自行深入研究一下。
<a name="BKlk4"></a>
#### `addWaiter`入队过程（队列不为空时）
1、根据当前线程创建一个新的Node节点<br />2、如果队尾元素不为空，则通过CAS操作插入到队尾<br />3、如果队列为空，`new`一个节点并设置为对头head<br />![2021-10-01-11-42-00-762742.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633060006365-1c6d77ab-f1c1-4001-841f-ae20729ca947.jpeg#clientId=u03fe1928-9540-4&from=ui&id=u97a4655e&originHeight=291&originWidth=640&originalType=binary&ratio=1&size=10626&status=done&style=shadow&taskId=ue4139c16-145e-41aa-a011-4683e5e41c4)
```java
private Node addWaiter(Node mode) {
    Node node = new Node(Thread.currentThread(), mode);
    // Try the fast path of enq; backup to full enq on failure
    Node pred = tail;
    if (pred != null) {
        node.prev = pred;
        if (compareAndSetTail(pred, node)) {
            pred.next = node;
            return node;
        }
    }
    enq(node);
    return node;
}
//入队逻辑
private Node enq(final Node node) {
    for (;;) {
        Node t = tail;
        if (t == null) { // Must initialize
            if (compareAndSetHead(new Node()))
                tail = head;
        } else {
            node.prev = t;
            if (compareAndSetTail(t, node)) {
                t.next = node;
                return t;
            }
        }
    }
}
```
在元素入队之后，又做了哪些操作呢？<br />1、通过自旋检测是否有资格获取锁，如果获取到了锁，则将当前节点设置为头节点<br />2、如果不是Head->next节点或获取锁失败，则阻塞当前线程
```java
final boolean acquireQueued(final Node node, int arg) {
    boolean failed = true;
    try {
        boolean interrupted = false;
        for (;;) {
            // 获取当前节点的prev节点
            final Node p = node.predecessor();
            // 如果是头节点，则去尝试获取锁
            if (p == head && tryAcquire(arg)) {
                // 获取锁成功，则将当前节点设置为头节点
                setHead(node);
                p.next = null; // help GC
                failed = false;
                return interrupted;
            }
            // 如果没有获取到锁，先判断是否需要阻塞当前线程
            if (shouldParkAfterFailedAcquire(p, node) &&
                // 通过LockSupport.park(this)原语阻塞当前线程
                parkAndCheckInterrupt())
                interrupted = true;
        }
    } finally {
        if (failed)
            cancelAcquire(node);
    }
}
```
这里最难理解的是这句,为什么当前节点的前节点是头节点时尝试获取锁呢？因为头节点代表当前正在持有锁的线程，该线程执行结束释放锁时，会唤醒队列中阻塞的线程，被唤醒的线程会尝试去获取锁。
```java
if (p == head && tryAcquire(arg)) {
    ...
}
```
<a name="C1kBU"></a>
### 3.2 公平锁的解锁过程
```java
public void unlock() {
    sync.release(1);
}

public final boolean release(int arg) {
    // 释放锁
    if (tryRelease(arg)) {
        Node h = head;
        // 队列不为空
        if (h != null && h.waitStatus != 0)
            // 唤醒队列中阻塞的线程
            unparkSuccessor(h);
        return true;
    }
    return false;
}
```
释放锁的具体逻辑如下
```java
protected final boolean tryRelease(int releases) {
    // 次数-1
    int c = getState() - releases;
    // 判断是否是当前线程
    if (Thread.currentThread() != getExclusiveOwnerThread())
        throw new IllegalMonitorStateException();
    boolean free = false;
    // 释放后没有线程占用
    if (c == 0) {
        free = true;
        // 将占有锁线程置为空
        setExclusiveOwnerThread(null);
    }
    // 更新状态
    setState(c);
    return free;
}
```
释放完成后唤醒后续节点
```java
private void unparkSuccessor(Node node) {
    //node是当前线程所在节点
    int ws = node.waitStatus;
    // 状态置为0
    if (ws < 0)
        compareAndSetWaitStatus(node, ws, 0);
    // 找到下个需要被唤醒的节点
    Node s = node.next;
    if (s == null || s.waitStatus > 0) {
        s = null;
        for (Node t = tail; t != null && t != node; t = t.prev)
            if (t.waitStatus <= 0)
                s = t;
    }
    if (s != null)
        //唤醒线程
        LockSupport.unpark(s.thread);
}
```
<a name="z7hwz"></a>
### 3.3 非公平锁和公平锁的实现原理基本类似
非公平锁在尝试获取锁时不会判断是否是队列中的第一个元素，而是直接进行CAS比较。 释放锁的流程是一样的。
```java
final boolean nonfairTryAcquire(int acquires) {
    final Thread current = Thread.currentThread();
    int c = getState();
    if (c == 0) {
        if (compareAndSetState(0, acquires)) {
            setExclusiveOwnerThread(current);
            return true;
        }
    }
    else if (current == getExclusiveOwnerThread()) {
        int nextc = c + acquires;
        if (nextc < 0) // overflow
            throw new Error("Maximum lock count exceeded");
        setState(nextc);
        return true;
    }
    return false;
}
```
<a name="gIEqL"></a>
## 四、总结
重入锁的实现主要有三个关键的知识点<br />1、同步等待队列，线程在尝试请求锁失败后，会进入到等待队列中等待被唤醒。如果有线程释放了锁，系统会唤醒等待队列中的一个线程。<br />2、原子性：原子状态是通过CAS操作来存储当前锁的状态（即`state`：重入次数）<br />3、通过`LockSupport.park`，`LockSupport.unpark`来挂起和恢复线程，未获取到锁的线程会被挂起。
