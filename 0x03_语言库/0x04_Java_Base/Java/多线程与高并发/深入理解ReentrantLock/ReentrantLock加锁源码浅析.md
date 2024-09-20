Java ReentrantLock<br />加锁逻辑将分成三个部分来看：

- 竞争锁
- 加入等待队列
- 阻塞等待
<a name="gXNFY"></a>
## 竞争锁
先从公平锁入手
```java
public void lock() {
    // sync的实例是new FairSync()
    sync.acquire(1);
}
// 加锁的代码就是这几行
public final void acquire(int arg) {
    if (!tryAcquire(arg) && acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
        selfInterrupt();
}
```
上述代码可以拆分成以下几段：
```java
// 竞争锁
tryAcquire(arg)
// 加入等待队列
addWaiter(Node.EXCLUSIVE)
// 阻塞等待
acquireQueued(node, arg)
```

- 竞争锁
```java
protected final boolean tryAcquire(int acquires) {
    // 获取当前线程
    final Thread current = Thread.currentThread();
    // 获取当前state状态
    int c = getState();
    // 如果当前state是没有任何线程抢占的话
    if (c == 0) {
        // 如果等待队列中有任何一个等待的节点，都不会抢占锁
        if (!hasQueuedPredecessors() &&
            // CAS抢占锁成功
            compareAndSetState(0, acquires)) {
            // 抢占成功后，标记当前线程已经抢占到锁了。
            setExclusiveOwnerThread(current);
            // 返回加锁成功
            return true;
        }
    }
    // 如果是同一个线程重复加锁的情况下
    else if (current == getExclusiveOwnerThread()) {
        // 在这种情况下，只是简单地操作state
        int nextc = c + acquires;
        if (nextc < 0)
            throw new Error("Maximum lock count exceeded");
        // 因为当前线程已经加锁成功了，再次加锁的话，直接在state上增加加锁次数即可。
        setState(nextc);
        // 返回加锁成功
        return true;
    }
    // 如果已经有别的线程加锁了，或者还有很多线程在排队等待，那么返回false加锁失败。
    return false;
}
```
上述代码分几部分：

1. 如果当前state=0，也就是没有任何线程抢占锁的情况下<br />1.1: 没有等待队列的情况下，可以CAS抢占锁<br />1.2: 有等待队列的话，该队列中第一个等待节点不是当前线程，不可以抢占锁，因为这是公平锁。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635574727353-6b2cfe35-df8c-4f1e-9e16-6503c10ff965.webp#averageHue=%23f5f4ef&clientId=u298e778a-5e05-4&from=paste&id=ud94720f3&originHeight=414&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaab44827-3876-43a5-8c35-0778f34f4b5&title=)<br />如果当前等待队列中还有任意节点，并且当前节点中的线程不是当前线程，说明有其他线程处于等待过程中，那么当前线程就应该乖乖排队去。<br />1.3: 有等待队列，并且当前第一个等待节点就是当前线程，可以抢占锁。这种情况会出现在线程刚从阻塞中被唤醒的时候。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635574727351-0bbf7c45-d254-466d-ab16-4bff7c2db7ef.webp#averageHue=%23f6f5ef&clientId=u298e778a-5e05-4&from=paste&id=u72d70582&originHeight=429&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6c1c4f33-c078-40ef-b2f3-a3491fb84b8&title=)<br />假如当前线程是被刚刚唤醒的，并且它处于等待队列中的第一个等待的位置，那么这个时候是可以去抢占锁的。

1. 如果已经抢占了锁的线程就是当前线程。这种情况叫做重入。<br />示例如下：
```java
ReentrantLock lock = new ReentrantLock();
try {
    // 加锁
    lock.lock();
    // 执行业务逻辑
    System.out.println("获取的锁");
    try {
        // 再次获取锁
        lock.lock();
        // 执行业务逻辑
        System.out.println("再次获取的锁");
    } finally {
        // 解锁
        lock.unlock();
    }
} finally {
    // 解锁
    lock.unlock();
}
```
小结一下：

1. 如果当前锁未被抢占，并且没有其他线程等待，那么直接抢占锁
2. 如果当前锁未被抢占，有其他线程等待，不可用抢占锁
3. 如果当前锁被当前线程抢占了，那么直接重入即可
4. 不符合上述情况，直接加锁失败。也就是锁被其他线程抢占了，或者目前还有其他线程处于等待中，都会导致公平锁加锁失败。
```java
// 判断等待队列中是否有其他线程等待
public final boolean hasQueuedPredecessors() {
    Node h, s;
    // 如果等待队列头节点不为空，说明等待队列已经创建出来了。否则直接返回false。
    if ((h = head) != null) {
        // 如果头节点后面的节点为空，或者该节点的状态是取消状态
        if ((s = h.next) == null || s.waitStatus > 0) {
            s = null; // traverse in case of concurrent cancellation
            // 从后往前遍历，直至最后一个状态小于等于0的节点。只有小于等于0的节点才是正常的可以竞争锁的节点。
            for (Node p = tail; p != h && p != null; p = p.prev) {
                // 发现小于等于0的节点，就赋值给s
                if (p.waitStatus <= 0)
                    s = p;
            }
        }
        // 如果最终得到的节点不为空。有可能当前没有任何等待的节点，s=null。
        // 并且这个不为空的等待线程不是当前线程。其实就是说明前面还有其他线程排队。
        if (s != null && s.thread != Thread.currentThread())
            // 返回true，说明有其他线程在排队。
            return true;
    }
    // 1.如果等待队列不存在，直接返回false
    // 2.如果当前等待队列中，没有任何其他节点的waitStatus<=0
    return false;
}
```
至此，线程竞争锁的逻辑就完毕了。
<a name="JxZ5H"></a>
## 加入等待队列
```java
private Node addWaiter(Node mode) {
    // 创建一个节点，该节点默认
    // waitStatus=0, thread=currentThread
    Node node = new Node(mode);
    // 开启自旋
    for (;;) {
        // 取出尾节点
        Node oldTail = tail;
        // 如果尾节点不为空
        if (oldTail != null) {
            // 设置node的前一个节点为尾节点
            node.setPrevRelaxed(oldTail);
            // CAS把尾节点设置为node
            if (compareAndSetTail(oldTail, node)){
                // 如果CAS设置成功，那么就把oldTail的next引用设置成node
                oldTail.next = node;
                // 返回node节点
                return node;
            }
        } else {
            // 如果尾节点为null，说明等待队列还不存在，这个时候就要准备初始化等待队列。
            // 初始化完毕后继续自旋，最终把新创建的节点添加进等待队列
            initializeSyncQueue();
        }
    }
}
// 初始化等待队列。其实是一个双向链表，所以只要初始化head、tail节点即可。
private final void initializeSyncQueue() {
    Node h;
    // CAS设置head节点。如果head节点为null，就设置为new Node()。该node节点waitStatus=0，thread=null。
    if (HEAD.compareAndSet(this, null, (h = new Node())))
        // 头节点设置成功后，尾节点初始化为同一个节点。
        tail = h;
}
```
<a name="kDX3S"></a>
### 1、初始化等待队列
```java
// 初始化等待队列。其实是一个双向链表，所以只要初始化head、tail节点即可。
private final void initializeSyncQueue() {
    Node h;
    // CAS设置head节点。如果head节点为null，就设置为new Node()。该node节点waitStatus=0，thread=null。
    if (HEAD.compareAndSet(this, null, (h = new Node())))
        // 头节点设置成功后，尾节点初始化为同一个节点。
        tail = h;
}
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635574727446-a75903a2-ca5a-4e6d-b528-d49dfa2d9d22.webp#averageHue=%23b8c17b&clientId=u298e778a-5e05-4&from=paste&id=u9588332f&originHeight=392&originWidth=356&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u670555b4-0fdc-4ea1-8bce-d78636b5693&title=)
<a name="gshth"></a>
### 2、添加新的节点
```java
// 创建新节点
Node node = new Node(mode);
// 取出尾节点
Node oldTail = tail;
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635574727418-0f09d1a5-613d-444c-8a87-e561bbce4a48.webp#averageHue=%23f8f7f3&clientId=u298e778a-5e05-4&from=paste&id=u24fe87ef&originHeight=245&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufff1d990-830d-42ce-8a20-2e2e6aa0937&title=)
```java
// 设置node的前一个节点为尾节点
node.setPrevRelaxed(oldTail);
// CAS把尾节点设置为node
if (compareAndSetTail(oldTail, node)){
// 如果CAS设置成功，那么就把oldTail的next引用设置成node
oldTail.next = node;
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635574727387-89c73e36-3cf7-4e79-9e4d-da75283b00bd.webp#averageHue=%23f8f7f2&clientId=u298e778a-5e05-4&from=paste&id=u35fc2520&originHeight=239&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2763352d-3f51-4fb8-9dbe-da93079560f&title=)<br />经过上面几步，新的节点就被添加到等待队列中了。<br />有一个注意点需要提的是：<br />为什么判断等待队列是否存在，使用的是if(tail!=null)，而不是if(head!=null)?<br />这个问题其实跟初始化等待队列有关系，初始化的时候是使用CAS设置head节点，成功后再设置tail节点。也就是说，队列初始化完毕的标识是tail!=null。<br />如果使用if(head!=null)来判断队列已经存在，那么有可能此时tail还没有初始化完毕。就会导致使用tail节点的时候空指针异常。
<a name="VetPv"></a>
## 阻塞等待
```java
final boolean acquireQueued(final Node node, int arg) {
    // 默认线程未被打断
    boolean interrupted = false;
    try {
        // 开启自旋
        for (;;) {
            // 获取当前节点的前一个节点
            final Node p = node.predecessor();
            // 如果前一个节点是head节点，那么就尝试竞争锁
            if (p == head && tryAcquire(arg)) {
                // 竞争锁成功，把当前节点设置为head节点
                setHead(node);
                // 把前一个节点和当前节点断开
                // 因为当前节点已经设置为head节点了，之前的head就可以GC了
                p.next = null; // help GC
                // 返回是否当前线程被打断。
                // 这个返回结果的作用会被用在lockInterruptibly()这个方法上。
                // lock()方法可忽略。
                return interrupted;
            }
            // 判断当前节点是否应该阻塞。
            if (shouldParkAfterFailedAcquire(p, node))
                // 下面这个代码可以翻译成：
                // if(parkAndCheckInterrupt()){
                //     interrupted = true;
                // }
                interrupted |= parkAndCheckInterrupt();
        }
    } catch (Throwable t) {
        // 抛出任何异常，都直接取消当前节点正在竞争锁的操作
        // 如果在等待队列中，就从等待队列中移除。
        // 如果当前线程已经抢占到锁了，那么就解锁。
        cancelAcquire(node);
        // 如果当前线程已经被中断
        if (interrupted)
            // 重新设置中断信号
            selfInterrupt();
        // 抛出当前异常
        throw t;
    }
}
```
<a name="rEGUw"></a>
### 1、获取当前节点的上一个节点
```java
// 获取当前节点的前一个节点
final Node p = node.predecessor();

final Node predecessor() {
    // 上一个节点
    Node p = prev;
    // 如果为null，直接抛异常
    if (p == null)
        throw new NullPointerException();
    else
        // 返回上一个节点
        return p;
}
```
<a name="sJJs9"></a>
### 2、如果上一个节点为head节点
```java
// 获取当前节点的前一个节点
final Node p = node.predecessor();
// 如果前一个节点是head节点，那么就尝试竞争锁
if (p == head && tryAcquire(arg))
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635574727763-2daa11aa-de42-42b1-ba19-ae744de6b640.webp#averageHue=%23f8f6f1&clientId=u298e778a-5e05-4&from=paste&id=ube091f65&originHeight=243&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6d1a5390-cc68-4499-ade3-ee122b0e082&title=)
<a name="CPtsB"></a>
### 3、抢占成功锁后
```java
// 竞争锁成功，把当前节点设置为head节点
setHead(node);
// 把前一个节点和当前节点断开
p.next = null;
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635574727720-c7f4e5cf-5764-46f1-8144-387f9b1069cd.webp#averageHue=%23f7f5f0&clientId=u298e778a-5e05-4&from=paste&id=ue4539992&originHeight=236&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9482d832-b2c3-4822-986f-4baacf84b6e&title=)
<a name="u1BkP"></a>
### 4、判断当前节点的状态
```java
private static boolean shouldParkAfterFailedAcquire(Node pred, Node node) {
    // 获取前一个节点的状态
    int ws = pred.waitStatus;
    // 如果状态等于-1。Node.SIGNAL的值就是-1
    if (ws == Node.SIGNAL)
        // 直接返回true，这个时候就要准备阻塞。
        return true;
    // 如果状态值大于0，说明是要取消的节点。
    if (ws > 0) {
        // 跳过“取消”状态节点
        do {
            node.prev = pred = pred.prev;
        } while (pred.waitStatus > 0);

        pred.next = node;
    } else {
        // ws小于等于0的话，直接把前一个节点的状态置为-1
        // 因为新创建的节点初始化状态是0，
        // 那么意味着执行到这里后，还要返回去重新自旋一次才能返回true。
        pred.compareAndSetWaitStatus(ws, Node.SIGNAL);
    }
    // 返回false
    return false;
}
```
<a name="d6pEh"></a>
### 5、当前线程阻塞
```java
private final boolean parkAndCheckInterrupt() {
    // 阻塞当前线程。
    // 1\. 调用LockSupport.unpark()才能重新唤醒被阻塞的线程。
    // 2.调用thread.interrupt()也可以唤醒阻塞线程。
    LockSupport.park(this);
    // 判断当前线程是否被打断。
    // 如果当前线程是被打断的，那么返回true，否则返回false。
    return Thread.interrupted();
}
```
小结一下：

1. 先获取当前节点的前一个节点，如果是head节点，那么尝试竞争锁
   1. 竞争锁成功后，重置head节点，返回false（代表没有被打断）。
2. 如果前一个节点状态小于等于0，那么置为-1。
   1. 重新自旋一次，从第一步开始
   2. 如果前一个节点状态等于-1，返回true，准备阻塞。
3. 调用`LockSupport.park()`阻塞当前线程，直至`unpark()`或者`interrupt()`唤醒当前线程。
   1. 通过`unpark()`唤醒，没有被打断，返回false
   2. 通过`interrupt()`唤醒，被打断，返回true。
4. 被唤醒的线程又开始自旋，直至获取到锁后返回是否被打断的结果。
   1. 如果是被打断后获取锁返回，那么返回true。
   2. 否则返回false。
```java
public final void acquire(int arg) {
    // 尝试获取锁
    if (!tryAcquire(arg) &&
    // addWaiter(Node.EXCLUSIVE)：竞争锁失败后，添加到等待队列
    // acquireQueued(node, arg)：阻塞等待，自旋获取锁后，返回判断是否被打断
acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
        // 如果被打断，需要恢复中断信号
        selfInterrupt();
}

// 其实就是重新中断一次。
// 因为执行过Thread.interrupted()方法后，会让中断信号重置为false。
static void selfInterrupt() {
    Thread.currentThread().interrupt();
}
```
