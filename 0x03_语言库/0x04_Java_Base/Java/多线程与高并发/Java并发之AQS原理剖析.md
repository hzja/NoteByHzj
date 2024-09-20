Java 并发 AQS
<a name="Af6rw"></a>
### 概述
`AbstractQueuedSynchronizer`可以称为抽象队列同步器。<br />AQS有独占模式和共享模式两种：

- 独占模式

公平锁：<br />非公平锁：

- 共享模式
<a name="cq4WO"></a>
### 数据结构
<a name="ApMkT"></a>
#### 基本属性
```java
/**
 * 同步等待队列的头结点
 */
private transient volatile Node head;

/**
 * 同步等待队列的尾结点
 */
private transient volatile Node tail;

/**
 * 同步资源状态
 */
private volatile int state;
```
<a name="xJDcl"></a>
#### 内部类
```java
static final class Node {
    /**
     * 标记节点为共享模式
     */
    static final Node SHARED = new Node();
    /**
     * 标记节点为独占模式
     */
    static final Node EXCLUSIVE = null;

    static final int CANCELLED =  1;
    static final int SIGNAL    = -1;
    static final int CONDITION = -2;
    static final int PROPAGATE = -3;

    /**
     *   CANCELLED:  值为1，表示当前的线程被取消
     *   SIGNAL: 值为-1，表示当前节点的后继节点包含的线程需要运行，也就是unpark；
     *   CONDITION:  值为-2，表示当前节点在等待condition，也就是在condition队列中；
     *   PROPAGATE:  值为-3，表示当前场景下后续的acquireShared能够得以执行；
     *   0:  表示当前节点在sync队列中，等待着获取锁。
     *  表示当前节点的状态值
     */
    volatile int waitStatus;

    /**
     * 前置节点
     */
    volatile Node prev;

    /**
     * 后继节点
     */
    volatile Node next;

    /**
     * 节点同步状态的线程
     */
    volatile Thread thread;

    /**
     * 存储condition队列中的后继节点
     */
    Node nextWaiter;

    /**
     * 是否为共享模式
     */
    final boolean isShared() {
        return nextWaiter == SHARED;
    }

    /**
     * 获取前驱结点
     */
    final Node predecessor() throws NullPointerException {
        Node p = prev;
        if (p == null)
            throw new NullPointerException();
        else
            return p;
    }

    Node() {    // Used to establish initial head or SHARED marker
    }

    Node(Thread thread, Node mode) {     // Used by addWaiter
        this.nextWaiter = mode;
        this.thread = thread;
    }

    Node(Thread thread, int waitStatus) { // Used by Condition
        this.waitStatus = waitStatus;
        this.thread = thread;
    }
}
```
<a name="DuUT0"></a>
### 主要方法解析
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623044370008-6be98d05-9ac5-4e75-aad3-53c52a06f91c.png#clientId=u97c8444d-4a33-4&from=paste&id=u7967c9c5&originHeight=477&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u6a576b61-6745-4cb0-a3ec-2df46d10f69)
<a name="lgHlh"></a>
#### `tryAcquire/tryAcquireShared(int arg)`
独占/共享模式获取锁；由子类实现，仅仅获取锁，获取锁失败时不进行阻塞排队。
<a name="jhG9K"></a>
#### `tryRelease/tryReleaseShared(int arg)`
独占/共享模式释放锁；由子类实现，仅仅释放锁，释放锁成功不对后继节点进行唤醒操作。
<a name="kUjtq"></a>
#### `acquire/acquireShared(int arg)`
独占/共享模式获取锁，如果线程被中断唤醒，会返回线程中断状态，不会抛异常中止执行操作（忽略中断）。
<a name="AVOtg"></a>
#### `acquireInterruptibly/acquireSharedInterruptibly(int arg)`
独占/共享模式获取锁，线程如果被中断唤醒，则抛出InterruptedException异常（中断即中止）。
<a name="Yob0Y"></a>
#### `tryAcquireNanos/tryAcquireSharedNanos(int arg, long nanosTimeout)`
独占/共享时间中断模式获取锁，线程如果被中断唤醒，则抛出InterruptedException异常（中断即中止）；如果超出等待时间则返回加锁失败。
<a name="e8BVb"></a>
#### `release/releaseShared(int arg)`
独占/共享模式释放锁。
<a name="bsm6K"></a>
#### `addWaiter(Node mode)`
将给定模式节点进行入队操作。
```java
private Node addWaiter(Node mode) {
    // 根据指定模式，新建一个当前节点的对象
    Node node = new Node(Thread.currentThread(), mode);
    // Try the fast path of enq; backup to full enq on failure
    Node pred = tail;
    if (pred != null) {
        // 将当前节点的前置节点指向之前的尾结点
        node.prev = pred;
        // 将当前等待的节点设置为尾结点（原子操作）
        if (compareAndSetTail(pred, node)) {
            // 之前尾结点的后继节点设置为当前等待的节点
            pred.next = node;
            return node;
        }
    }
    enq(node);
    return node;
}
```
<a name="ns6NF"></a>
#### `enq(final Node node)`
将节点设置为尾结点。注意这里会进行自旋操作，确保节点设置成功。因为等待的线程需要被唤醒操作；如果操作失败，当前节点没有与其他节点没有引用指向关系，一直就不会被唤醒（除非程序代码中断线程）。
```java
private Node enq(final Node node) {
    for (;;) {
        Node t = tail;
        // 判断尾结点是否为空，尾结点初始值是为空
        if (t == null) { // Must initialize
            // 尾结点为空，需要初始化
            if (compareAndSetHead(new Node()))
                tail = head;
        } else {
            // 设置当前节点设置为尾结点
            node.prev = t;
            if (compareAndSetTail(t, node)) {
                t.next = node;
                return t;
            }
        }
    }
}

```
<a name="xOBCm"></a>
#### `acquireQueued(final Node node, int arg)`
已经在队列当中的节点，准备阻塞获取锁。在阻塞前会判断前置节点是否为头结点，如果为头结点；这时会尝试获取下锁（因为这时头结点有可能会释放锁）。
```java
final boolean acquireQueued(final Node node, int arg) {
    boolean failed = true;
    try {
        boolean interrupted = false;
        for (;;) {
            // 当前节点的前置节点
            final Node p = node.predecessor();
            // 入队前会先判断下该节点的前置节点是否是头节点（此时头结点有可能会释放锁）；然后尝试去抢锁
            // 在非公平锁场景下有可能会抢锁失败，这时候会继续往下执行 阻塞线程
            if (p == head && tryAcquire(arg)) {
                //如果抢到锁，将头节点后移（也就是将该节点设置为头结点）
                setHead(node);
                p.next = null; // help GC
                failed = false;
                return interrupted;
            }
            // 如果前置节点不是头结点，或者当前节点抢锁失败；通过shouldParkAfterFailedAcquire判断是否应该阻塞
            // 当前置节点的状态为SIGNAL=-1，才可以安全被parkAndCheckInterrupt阻塞线程
            if (shouldParkAfterFailedAcquire(p, node) &&
                parkAndCheckInterrupt())
                // 该线程已被中断
                interrupted = true;
        }
    } finally {
        if (failed)
            cancelAcquire(node);
    }
}
```
<a name="ExlBz"></a>
#### `shouldParkAfterFailedAcquire(Node pred, Node node)`
检查和更新未能获取锁节点的状态，返回是否可以被安全阻塞。
```java
private static boolean shouldParkAfterFailedAcquire(Node pred, Node node) {
    int ws = pred.waitStatus;   // 获取前置节点的状态
    if (ws == Node.SIGNAL)
        /*
             * 前置节点的状态waitStatus为SIGNAL=-1，当前线程可以安全的阻塞
             */
        return true;
    if (ws > 0) {
        /*
             * 如果前置节点的状态waitStatus>0，即waitStatus为CANCELLED=1（无效节点），需要从同步状态队列中取消等待（移除队列）
             */
        do {
            node.prev = pred = pred.prev;
        } while (pred.waitStatus > 0);
        pred.next = node;
    } else {
        /*
             * 将前置状态的waitStatus修改为SIGNAL=-1，然后当前节点才可以被安全的阻塞
             */
        compareAndSetWaitStatus(pred, ws, Node.SIGNAL);
    }
    return false;
}
```
<a name="PQG5T"></a>
#### `parkAndCheckInterrupt()`
阻塞当前节点，返回当前线程的中断状态。
```java
private final boolean parkAndCheckInterrupt() {
    LockSupport.park(this); //阻塞
    return Thread.interrupted();
}
```
<a name="ohlBK"></a>
#### `cancelAcquire(Node node)`
取消进行的获取锁操作，在非忽略中断模式下，线程被中断唤醒抛异常时会调用该方法。
```java
//  将当前节点的状态设置为CANCELLED，无效的节点，同时移除队列
private void cancelAcquire(Node node) {
    if (node == null)
        return;

    node.thread = null;
    Node pred = node.prev;
    while (pred.waitStatus > 0)
        node.prev = pred = pred.prev;

    Node predNext = pred.next;
    node.waitStatus = Node.CANCELLED;
    if (node == tail && compareAndSetTail(node, pred)) {
        compareAndSetNext(pred, predNext, null);
    } else {
        int ws;
        if (pred != head &&
            ((ws = pred.waitStatus) == Node.SIGNAL ||
             (ws <= 0 && compareAndSetWaitStatus(pred, ws, Node.SIGNAL))) &&
            pred.thread != null) {
            Node next = node.next;
            if (next != null && next.waitStatus <= 0)
                compareAndSetNext(pred, predNext, next);
        } else {
            unparkSuccessor(node);
        }

        node.next = node; // help GC
    }
}
```
<a name="g4FzW"></a>
#### `hasQueuedPredecessors()`
判断当前线程是否应该排队。<br />1.第一种结果——返回true：（1.1和1.2同时存在，1.2.1和1.2.2有一个存在）<br />1.1  h != t为true，说明头结点和尾结点不相等，表示队列中至少有两个不同节点存在，至少有一点不为null。<br />1.2  ((s = h.next) == null || s.thread != Thread.currentThread())为true<br />1.2.1   (s = h.next) == null为true，表示头结点之后没有后续节点。<br />1.2.2   (s = h.next) == null为false，s.thread != Thread.currentThread()为true<br />表示头结点之后有后续节点，但是头节点的下一个节点不是当前线程<br />2.第二种结果——返回false，无需排队。（2.1和2.2有一个存在）<br />2.1  h != t为false，即h == t；表示h和t同时为null或者h和t是同一个节点，无后续节点。<br />2.2  h != t为true，((s = h.next) == null || s.thread != Thread.currentThread())为false<br />表示队列中至少有两个不同节点存在，同时持有锁的线程为当前线程。
```java
public final boolean hasQueuedPredecessors() {
    Node t = tail; // Read fields in reverse initialization order
    Node h = head;
    Node s;
    return h != t &&
        ((s = h.next) == null || s.thread != Thread.currentThread());
}
```
