Java Condition
<a name="6qJf3"></a>
## 概要
在JDK1.5之后出了Condition，它可以实现在同步语义中的等待/通知，以此来实现线程之间通信或协同。Condition和Object的wait和notify/notify在用法和效果上都十分的类似。
<a name="0BCge"></a>
## 使用
<a name="PLZqy"></a>
### 例子
Condition经常可以用在生产者-消费者的场景中，看看官方的例子，这是一个经典的例子，看到会有熟悉的味道
```java
class BoundedBuffer {
    final Lock lock = new ReentrantLock();
    final Condition notFull  = lock.newCondition(); 
    final Condition notEmpty = lock.newCondition(); 
    final Object[] items = new Object[100];
    int putptr, takeptr, count;
    public void put(Object x) throws InterruptedException {
        lock.lock();
        try {
            while (count == items.length)
                notFull.await();
            items[putptr] = x;
            if (++putptr == items.length) putptr = 0;
            ++count;
            notEmpty.signal();
        } finally {
            lock.unlock();
        }
    }
    public Object take() throws InterruptedException {
        lock.lock();
        try {
            while (count == 0)
                notEmpty.await();
            Object x = items[takeptr];
            if (++takeptr == items.length) takeptr = 0;
            --count;
            notFull.signal();
            return x;
        } finally {
            lock.unlock();
        }
    }
}
```
上面是一个往有界的buffer数组写/取数据的例子，为了防止写数组溢出或读不到数据，用了Condition来做控制。可以看出Condition必须在临界区内使用，即必须先持有相应的锁，这跟synchronized和对象的监视器锁`wait()`/`notify()`很像。
<a name="ix5Tm"></a>
### 常用方法
Condition的使用比较简单，总的来说只有等待和唤醒两套方法
```java
//当前线程进入等待状态直到被通知（signal）或中断。
public final void await() throws InterruptedException
//跟上面一样，但不响应中断。
public final long awaitNanos(long nanosTimeout) throws InterruptedException
//跟上面一样，指定超时等待多长的时间
public final boolean awaitUntil(Date deadline) throws InterruptedException
//当跟上面一样，指定超时等待的某个时间点
public final boolean await(long time, TimeUnit unit) throws InterruptedException
```
```java
//唤醒一个等待在Condition队列上的线程
public final void signal()
//唤醒所有等待在Condition队列上的线程
public final void signalAll()
```
<a name="P1tmm"></a>
## 实现原理
Condition其实是`AbstractQueuedSynchronizer`内部实现的`ConditionObject`。Condition的实现跟AQS有很大的关系。<br />Condition的原理大致是，当调用wait方法时，同步队列的头节点，即锁的持有者释放锁，并为当前线程创建一个节点加入到条件队列等待；当signal时，会释放条件队列的节点，并把这个节点接入到同步队列中等待获得锁。
<a name="RbDJz"></a>
### 初始化
Condition的初始化，我们可以看看ReentrantLock的，其最终是新建一个`ConditionObject`对象，ConditionObject是AQS的一个内部类
```java
public Condition newCondition() {
    return sync.newCondition();
}
final ConditionObject newCondition() {
    return new ConditionObject();
}
```
每调用一次newCondition则会创建一个Condition对象，所以一个AQS可以对应多个Condition，可以使用多个条件队列，从上面BoundedBuffer的例子可以看出来。<br />Condition内部是一个condition queue条件队列。注意这个队列的节点Node，就是跟AQS用的是同一个
```java
/** First node of condition queue. */
//指向队列头的指针
private transient Node firstWaiter;
/** Last node of condition queue. */
//指向队列尾的指针
private transient Node lastWaiter;
```
<a name="5v5Eq"></a>
### await
await方法有几个版本，看看最普通的那个
```java
public final void await() throws InterruptedException {
    //响应中断，抛出中断异常
    if (Thread.interrupted())
        throw new InterruptedException();
    //生成一个CONDITION属性节点，加入到条件队列的尾部
    Node node = addConditionWaiter();
    //释放锁，savedState为释放锁之前AQS的共享状态变量state的值。
    //由于调用此方法必须获得锁，所以这里是锁的持有者主动释放锁；如果不是锁的持有者，则会报错
    int savedState = fullyRelease(node);
    int interruptMode = 0;
    //如果不是在同步队列则一直阻塞
    //当然如果是中断的话，也会break出来
    //当节点第一次进来时，由于在条件队列中，肯定不在同步队列中，所以会被park住
    //这里会一直block，直到在同步队列由前驱节点unpark唤醒
    while (!isOnSyncQueue(node)) {
        LockSupport.park(this);
        if ((interruptMode = checkInterruptWhileWaiting(node)) != 0)
            break;
    }
    //被唤醒后，由于此时是在临界区，所以必须再次获得锁才能继续。这里会阻塞直到获得锁
    //state必须为savedState，有点还原现场的意思
    if (acquireQueued(node, savedState) && interruptMode != THROW_IE)
        interruptMode = REINTERRUPT;
    if (node.nextWaiter != null) // clean up if cancelled
        unlinkCancelledWaiters();
    if (interruptMode != 0)
        reportInterruptAfterWait(interruptMode);
}
```
从await方法已经可以大概了解Condition的处理流程，下面对里面的每个方法一一分析一下
<a name="V2ZXD"></a>
#### 第一步：加入条件队列
```java
//addConditionWaiter方法用来新建一个CONDITION的节点，然后加到条件队列尾部
//上面说了，Condition持有firstWaiter和lastWaiter两个指针
private Node addConditionWaiter() {
    Node t = lastWaiter;
    //不等于Node.CONDITION说明被cancel了，则清理一下这些垃圾节点
    if (t != null && t.waitStatus != Node.CONDITION) {
        unlinkCancelledWaiters();
        t = lastWaiter;
    }
    //新建一个node，状态为CONDITION
    Node node = new Node(Thread.currentThread(), Node.CONDITION);
    if (t == null)
        firstWaiter = node;
    else
        //把尾部的next指向新建的node
        //注意，Node的nextWaiter属性是专门用来构成条件队列，也看出条件队列是一个单向队列；而Node的next属性则是用于同步队列
        t.nextWaiter = node;
    //尾部指向新建的node
    lastWaiter = node;
    return node;
}
```
<a name="V3Jri"></a>
#### 第二步：释放锁
```java
final int fullyRelease(Node node) {
    //默认释放锁失败
    boolean failed = true;
    try {
        //获得当前（即释放锁前）state的值
        int savedState = getState();
        //然后释放锁；释放多少？释放savedState这么多，因为有可能是重入锁；因为要完全的释放锁，才能让出锁给别人，所以这个方法叫fullyRelease
        //释放锁是调用AQS的release方法，而会最终调用子类的tryRelease方法，如果tryRelease返回失败，则release返回false
        if (release(savedState)) {
            failed = false;
            return savedState;
        } else {
            throw new IllegalMonitorStateException();
        }
    } finally {
        //如果tryRelease失败或者抛出异常，则把节点标记为CANCELLED，等待被清理掉
        //这里试想一种情况，如果一个线程在没有进入临界就调用Condition的await方法，会怎样？从我们的分析下来看没有问题，会在条件队列新加一个节点，直到这里就不行了，因为在tryRelease方法就会抛出IllegalMonitorStateException，然后就到这里。
        if (failed)
            node.waitStatus = Node.CANCELLED;
    }
}
```
```java
//如上所述，第一次肯定不在同步队列，会被挂起
//在signal时将节点从条件队列移到同步队列
//这个方法就是判断node是否已经移动到同步队列了
final boolean isOnSyncQueue(Node node) {
    //如果还是CONDITION则说明肯定还在条件队列中，因为移到同步队列会把状态更新为0
    //如果prev == null，则说明肯定还在条件队列中，因为移到同步队列肯定有前驱节点，这个prev属性只有在同步队列时才用到
    if (node.waitStatus == Node.CONDITION || node.prev == null)
        return false;
    //如果 node 已经有后继节点 next 的时候，那肯定是在阻塞队列了
    if (node.next != null) // If has successor, it must be on queue
        return true;
    //来到这里说明，node.waitStatus != Node.CONDITION && node.prev != null && node.next == null
    //但是node.prev != null 还不能说明node就在同步队列中，回顾一下AQS的进队操作，首先把node.prev指向tail，再通过cas把tail指向自己
    //但是第二步有可能失败，所以这里需要从tail开始遍历，如果能遍历到这个node，说明肯定在同步队列中
    return findNodeFromTail(node);
}
private boolean findNodeFromTail(Node node) {
    Node t = tail;
    for (;;) {
        if (t == node)
            return true;
        if (t == null)
            return false;
        t = t.prev;
    }
}
```
接下来的`acquireQueued`方法是AQS获取锁的方法，熟悉AQS的应该清楚，会一直阻塞直到排队被唤醒，竞争锁成功。<br />上述大致如图所示：<br />![640.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1608345365957-6b478308-ea7e-4d87-a1c7-01ce443f4b67.png#align=left&display=inline&height=867&originHeight=867&originWidth=1080&size=209431&status=done&style=shadow&width=1080)
<a name="vBVaO"></a>
### signal
唤醒同步队列有`signal`和`signalAll`方法，两者的区别是前者只唤醒同步队列的头节点，后者唤醒同步队列的所有节点，我们重点看看signal<br />唤醒操作通常由另一个线程来操作，就像生产者-消费者模式中，如果线程因为等待消费而挂起，那么当生产者生产了一个东西后，会调用 signal 唤醒正在等待的线程来消费。
```java
public final void signal() {
    //必须是当前锁的持有者
    if (!isHeldExclusively())
        throw new IllegalMonitorStateException();
    Node first = firstWaiter;
    if (first != null)
        //将条件队列的第一个节点释放掉
        doSignal(first);
}
private void doSignal(Node first) {
    do {
        //将firstWaiter指向first的下一个节点，因为first将要被释放掉，所以如果没有下一个节点，那么也将lastWaiter置为null
        if ( (firstWaiter = first.nextWaiter) == null)
            lastWaiter = null;
        //释放当前first节点
        first.nextWaiter = null;
        //如果转移失败且下一个节点不null，那么继续转移下一个节点
        //为什么转移失败下面说
    } while (!transferForSignal(first) &&
             (first = firstWaiter) != null);
}
final boolean transferForSignal(Node node) {
    //CAS 如果失败，说明此 node 的 waitStatus 已不是 Node.CONDITION，说明节点已经取消
    //既然已经取消，也就不需要转移了，返回false，继续转移一个节点
    if (!compareAndSetWaitStatus(node, Node.CONDITION, 0))
        return false;
    //把节点加入到同步队列中，调用的是AQS的enq方法
    //返回值p是node加入到同步队列的前驱节点
    Node p = enq(node);
    int ws = p.waitStatus;
    //ws > 0 表示前驱节点是取消状态，如果是，则unpark Node的线程。根据上面的await的分析，由于此时node已经在同步队列，所以unpark后还需要去获得锁
    //compareAndSetWaitStatus(p, ws, Node.SIGNAL) 是AQS的指定动作，把前驱节点设置为SIGNAL，意思是前驱节点要去唤醒后继节点
    //compareAndSetWaitStatus(p, ws, Node.SIGNAL) 会失败是由于恰好此时 p 的被取消了，所以也是和上面一个意思
    //这里的意思是，如果前驱节点取消了，就给当前节点一个机会去尝试获得锁
    if (ws > 0 || !compareAndSetWaitStatus(p, ws, Node.SIGNAL))
        LockSupport.unpark(node.thread);
    return true;
}
```
signal过程大致如图所示：<br />![640.webp](https://cdn.nlark.com/yuque/0/2020/webp/396745/1608345405368-b27d538e-a583-4012-a2b9-9c810381a2d4.webp#align=left&display=inline&height=878&originHeight=878&originWidth=1080&size=31230&status=done&style=shadow&width=1080)
<a name="95PDM"></a>
## 总结
Condition为在临界区中提供了协同的操作，试想想如果需要做到类似挂起/唤醒的操作，那么比较好的做法是通过一个队列来完成，而由于AbstractQueuedSynchronizer自身实现的存在，所以条件队列的节点跟同步队列用的是同一个类。Condition和Object的wait和notify/notify很像似，所以基本可以猜测实现原理也是差不多，只是前者在Java层面实现，后者在更底层的JVM实现。
