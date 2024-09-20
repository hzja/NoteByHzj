JavaReentrantLock
<a name="WQdna"></a>
## 简单使用
在看它的源码之前，先来做个简单的使用说明。当在IDEA中创建了一个简单的Demo之后，它会给出以下提示<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666967967455-216a346e-7cf7-4eb2-b594-ca5aadc652b6.png#clientId=ueb829079-b0e0-4&from=paste&id=u6ee7c089&originHeight=106&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc7cbb20a-e365-49fd-9f38-b64efd43d13&title=)
<a name="w5d28"></a>
### 提示文字
在使用阻塞等待获取锁的方式中，必须在`try`代码块之外，并且在加锁方法与`try`代码块之间没有任何可能抛出异常的方法调用，避免加锁成功后，在`finally`中无法解锁。

- 1、如果在`lock`方法与`try`代码块之间的方法调用抛出异常，那么无法解锁，造成其它线程无法成功获取锁。
- 2、如果`lock`方法在`try`代码块之内，可能由于其它方法抛出异常，导致在`finally`代码块中，`unlock`对未加锁的对象解锁，它会调用AQS的`tryRelease`方法（取决于具体实现类），抛出`IllegalMonitorStateException`异常。
- 3、在`Lock`对象的`lock`方法实现中可能抛出`unchecked`异常，产生的后果与说明二相同。

`java.concurrent.LockShouldWithTryFinallyRule.rule.desc`<br />还举了两个例子，正确案例如下：
```java
Lock lock = new XxxLock();
// ...
lock.lock();
try {
    doSomething();
    doOthers();
} finally {
    lock.unlock();
}
```
错误案例如下：
```java
Lock lock = new XxxLock();
// ...
try {
    // 如果在此抛出异常，会直接执行 finally 块的代码
    doSomething();
    // 不管锁是否成功，finally 块都会执行
    lock.lock();
    doOthers();

} finally {
    lock.unlock();
}
```
<a name="m3hn3"></a>
## AQS
上边的案例中加锁调用的是`lock()`方法，解锁用的是`unlock()`方法，而通过查看源码发现它们都是调用的内部静态抽象类`Sync`的相关方法。
```java
abstract static class Sync extends AbstractQueuedSynchronizer
```
Sync 是通过继承`AbstractQueuedSynchronizer`来实现的，没错，`AbstractQueuedSynchronizer`就是AQS的全称。AQS内部维护着一个FIFO的双向队列（CLH），`ReentrantLock`也是基于它来实现的，先来张图感受下。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967967473-c54e5fc9-1b52-48ad-b85f-e56bc08df387.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u0babab9f&originHeight=550&originWidth=1074&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8c78a953-2c86-4c77-9df8-0bd327c68b7&title=)
<a name="a8Igj"></a>
### Node 属性
```java
//此处是 Node 的部分属性
static final class Node {

    //排他锁标识
    static final Node EXCLUSIVE = null;

    //如果带有这个标识，证明是失效了
    static final int CANCELLED =  1;

    //具有这个标识，说明后继节点需要被唤醒
    static final int SIGNAL = -1;

    //Node对象存储标识的地方
    volatile int waitStatus;

    //指向上一个节点
    volatile Node prev;

    //指向下一个节点
    volatile Node next;

    //当前Node绑定的线程
    volatile Thread thread;

    //返回前驱节点即上一个节点，如果前驱节点为空，抛出异常
    final Node predecessor() throws NullPointerException {
        Node p = prev;
        if (p == null)
            throw new NullPointerException();
        else
            return p;
    }
}
```
对于里边的`waitStatus`属性，需要做个解释：**（非常重要）**

- CANCELLED(1)：当前节点取消获取锁。当等待超时或被中断(响应中断)，会触发变更为此状态，进入该状态后节点状态不再变化；
- SIGNAL(-1)：后面节点等待当前节点唤醒；
- CONDITION(-2)：`Condition`中使用，当前线程阻塞在`Condition`，如果其他线程调用了`Condition`的`signal`方法，这个结点将从等待队列转移到同步队列队尾，等待获取同步锁；
- PROPAGATE(-3)：共享模式，前置节点唤醒后面节点后，唤醒操作无条件传播下去；
- 0：中间状态，当前节点后面的节点已经唤醒，但是当前节点线程还没有执行完成；
<a name="endlp"></a>
### AQS 属性
```java
// 头结点
private transient volatile Node head;

// 尾结点
private transient volatile Node tail;

//0->1 拿到锁，大于0 说明当前已经有线程占用了锁资源
private volatile int state;
```
先简单了解下AQS的构造以帮助大家更好的理解ReentrantLock，至于深层次的东西先不做展开！
<a name="SovAD"></a>
## 加锁
对AQS的结构有了基本了解之后，正式进入主题——加锁。从源码中可以看出锁被分为**公平锁**和**非公平锁**。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967967507-d58a7491-8f12-494f-afc7-1c5c0834cead.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=ud390f007&originHeight=433&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7d26e16a-62fb-4ca2-8948-a1de59411d8&title=)
```java
/**
 * 公平锁代码
 */
final void lock() {
    acquire(1);
}

/**
 * 非公平锁代码
 */
final void lock() {
    if (compareAndSetState(0, 1))
        setExclusiveOwnerThread(Thread.currentThread());
    else
        acquire(1);
}
```
初步查看代码发现非公平锁似乎包含公平锁的逻辑，所以就从“非公平锁”开始。
<a name="aDaK3"></a>
### 非公平锁
```java
final void lock() {
    //通过 CAS 的方式尝试将 state 从0改为1，
    //如果返回 true，代表修改成功，获得锁资源;
    //如果返回false，代表修改失败，未获取锁资源
    if (compareAndSetState(0, 1))
        // 将属性exclusiveOwnerThread设置为当前线程，该属性是AQS的父类提供的
        setExclusiveOwnerThread(Thread.currentThread());
    else
        acquire(1);
}
```
`compareAndSetState()`：底层调用的是`unsafe`的`compareAndSwapInt`，该方法是原子操作；<br />假设有两个线程（t1、t2）在竞争锁资源，线程1获取锁资源之后，执行`setExclusiveOwnerThread`操作，设置属性值为当前线程t1<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967967531-7b511185-6f37-43f4-a07d-108e02344dda.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=uac03b955&originHeight=544&originWidth=370&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u687fc0be-3850-4237-997d-e0ac00150f2&title=)<br />此时，当t2想要获取锁资源，调用`lock()`方法之后，执行`compareAndSetState(0, 1)`返回false，会走else执行`acquire()`方法。
<a name="Zx6zH"></a>
### 方法查看
```java
public final void accquire(int arg) {
    // tryAcquire 再次尝试获取锁资源，如果尝试成功，返回true，尝试失败返回false
    if (!tryAcquire(arg) &&
        // 走到这，代表获取锁资源失败，需要将当前线程封装成一个Node，追加到AQS的队列中
        acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
    // 线程中断
    selfInterrupt();
}
```
`accquire()`中涉及的方法比较多，将进行拆解，一个一个来分析，顺序：`tryAcquire()` -> `addWaiter()` -> `acquireQueued()`
<a name="hIurh"></a>
#### 查看 `tryAcquire()` 方法
```java
//AQS中
protected boolean tryAcquire(int arg) {
    //AQS 是基类，具体实现在自己的类中实现，去查看“非公平锁”中的实现
    throw new UnsupportedOperationException();
}

//ReentrantLock 中
protected final boolean tryAcquire(int acquires) {
    return nonfairTryAcquire(acquires);
}


final boolean nonfairTryAcquire(int acquires) {
    // 获取当前线程
    final Thread current = Thread.currentThread();
    //获取AQS 的 state 
    int c = getState();
    // 如果 state 为0，代表尝试再次获取锁资源
    if (c == 0) {
        // 步骤同上：通过 CAS 的方式尝试将 state 从0改为1，
        //如果返回 true，代表修改成功，获得锁资源;
        //如果返回false，代表修改失败，未获取锁资源
        if (compareAndSetState(0, acquires)) {
            //设置属性为当前线程
            setExclusiveOwnerThread(current);
            return true;
        }
    }
        //当前占有锁资源的线程是否是当前线程，如果是则证明是可重入操作
    else if (current == getExclusiveOwnerThread()) {
        //将 state + 1
        int nextc = c + acquires;
        //为什么会小于 0 呢？因为最大值 + 1 后会将符号位的0改为1 会变成负数(可参考Integer.MAX_VALUE + 1)
        if (nextc < 0) // overflow
            //加1后小于0，超出锁可重入的最大值，抛异常
            throw new Error("Maximum lock count exceeded");
        //设置 state 状态
        setState(nextc);
        return true;
    }
    return false;
}
```
因为线程1已经获取到了锁，此时state为1，所以不走`nonfairTryAcquire()`的if。又因为当前是线程2，不是占有当前锁的线程1，所以也不会走`else if`，即`tryAcquire()`方法返回false。
<a name="Q1CVr"></a>
#### 查看 `addWaiter()` 方法
走到本方法中，代表获取锁资源失败。`addWaiter()`将没有获取到锁资源的线程甩到队列的尾部。
```java
private Node addWaiter(Node mode) {
    //创建 Node 类，并且设置 thread 为当前线程，设置为排它锁
    Node node = new Node(Thread.currentThread(), mode);
    // 获取 AQS 中队列的尾部节点
    Node pred = tail;
    // 如果 tail == null，说明是空队列，
    // 不为 null，说明现在队列中有数据，
    if (pred != null) {
        // 将当前节点的 prev 指向刚才的尾部节点，那么当前节点应该设置为尾部节点
        node.prev = pred;
        // CAS 将 tail 节点设置为当前节点
        if (compareAndSetTail(pred, node)) {
            // 将之前尾节点的 next 设置为当前节点
            pred.next = node;
            // 返回当前节点
            return node;
        }
    }
    enq(node);
    return node;
}
```
当tail不为空，即队列中有数据时，来图解一下`pred!=null`代码块中的代码。初始化状态如下，pred指向尾节点，node指向新的节点。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967967470-fbd3442b-f5ef-4f5e-b20e-bf3e8efc213d.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u5e14c0e8&originHeight=609&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9336ed57-b86f-4623-bd6b-571b241f8f1&title=)<br />`node.prev = pred;`将node的前驱节点设置为pred指向的节点<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968120-3b06ac5d-63e9-4c71-84b8-4970f9453d51.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u3f31083d&originHeight=596&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a3c4412-877a-4496-ad45-81730ee4f32&title=)<br />`compareAndSetTail(pred, node)`通过CAS的方式尝试将当前节点node设置为尾结点，此处假设设置成功，则FIFO队列的tail指向node节点。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968159-e1b23532-5e77-40a2-80b2-7c93a0512378.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u7f72b614&originHeight=591&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufb226ab7-5d78-49e7-8c1d-e9bf3950fa0&title=)<br />`pred.next = node;`将pred节点的后继节点设置为node节点，此时node节点成功进入FIFO队列尾部。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968207-c764d81a-352b-40bd-96a7-92a4be0c0635.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u7a2f0e0d&originHeight=610&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u56d92a98-8822-4b41-b267-b6ca8d51b39&title=)<br />而当pred为空，即队列中没有节点或将node节点设置为尾结点失败时，会走`enq()`方法。列举的例子就符合pred为空的情况，就以例子为基础继续分析吧。
```java
//现在没人排队，我是第一个 || 前边CAS失败也会进入这个位置重新往队列尾巴去塞
private Node enq(final Node node) {
    //死循环
    for (;;) {
        //重新获取tail节点
        Node t = tail;
        // 没人排队，队列为空
        if (t == null) {
            // 初始化一个 Node 为 head，而这个head 没有意义
            if (compareAndSetHead(new Node()))
                // 将头尾都指向了这个初始化的Node，第一次循环结束
                tail = head;
        } else {
            // 有人排队，往队列尾巴塞
            node.prev = t;
            // CAS 将 tail 节点设置为当前节点
            if (compareAndSetTail(t, node)) {
                //将之前尾节点的 next 设置为当前节点
                t.next = node;
                return t;
            }
        }
    }
}
```
进入死循环，首先会走if方法的逻辑，通过CAS的方式尝试将一个新节点设置为head节点，然后将tail也指向新节点。可以看出队列中的头节点只是个初始化的节点，没有任何意义。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968252-0b618a2b-be1b-4a69-8c3d-a23d51143165.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=uf39d9f27&originHeight=617&originWidth=1074&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u109061d4-79f9-4331-8d78-04a6280b4d1&title=)<br />继续走死循环中的代码，此时t不为null，所以会走else方法。将node的前驱节点指向t，通过CAS方式将当前节点node设置为尾结点，然后将t的后继节点指向node。此时线程2的节点就被成功塞入FIFO队列尾部。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968203-d5ef845a-612a-46b2-956b-69ba9895f663.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u94cc3629&originHeight=639&originWidth=1062&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ude066859-758c-47a6-8f5d-742a21e991f&title=)
<a name="Oubqt"></a>
#### 查看 `acquireQueued()`方法
将已经在队列中的`node`尝试去获取锁否则挂起。
```java
final boolean acquireQueued(final Node node, int arg) {
    // 获取锁资源的标识,失败为 true，成功为 false
    boolean failed = true;
    try {
        // 线程中断的标识，中断为 true，不中断为 false
        boolean interrupted = false;
        for (;;) {
            // 获取当前节点的上一个节点
            final Node p = node.predecessor();
            //p为头节点，尝试获取锁操作
            if (p == head && tryAcquire(arg)) {
                setHead(node);
                p.next = null;
                // 将获取锁失败标识置为false
                failed = false;
                // 获取到锁资源，不会被中断
                return interrupted;
            }
            // p 不是 head 或者 没拿到锁资源，
            if (shouldParkAfterFailedAcquire(p, node) &&
                // 基于 Unsafe 的 park方法，挂起线程
                parkAndCheckInterrupt())
                interrupted = true;
        }
    } finally {
        if (failed)
            cancelAcquire(node);
    }
}
```
这里又出现了一次死循环，首先获取当前节点的前驱节点p，如果p是头节点(头节点没有意义)，说明node是head后的第一个节点，此时当前获取锁资源的线程1可能会释放锁，所以线程2可以再次尝试获取锁。<br />假设获取成功，证明拿到锁资源了，将node节点设置为head节点，并将node节点的pre和thread设置为null。因为拿到锁资源了，node节点就不需要排队了。<br />将头节点p的next置为null，此时p节点就不在队列中存在了，可以帮助GC回收(可达性分析)。failed设置为false，表明获取锁成功；interrupted为false，则线程不会中断。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968742-9610945f-9851-4222-8ffe-6f20f3adbfb1.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u583227c0&originHeight=690&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue2840a5e-1835-4981-ac05-3ab55691fdc&title=)<br />如果p不是head节点或者没有拿到锁资源，会执行下边的代码，因为线程1没有释放锁资源，所以线程2获取锁失败，会继续往下执行。
```java
//该方法的作用是保证上一个节点的waitStatus状态为-1（为了唤醒后继节点）
private static boolean shouldParkAfterFailedAcquire(Node pred, Node node) {
    //获取上一个节点的状态,该状态为-1，才会唤醒下一个节点。
    int ws = pred.waitStatus;
    // 如果上一个节点的状态是SIGNAL即-1，可以唤醒下一个节点，直接返回true
    if (ws == Node.SIGNAL)
        return true;
    // 如果上一个节点的状态大于0，说明已经失效了
    if (ws > 0) {
        do {
            // 将node 的节点与 pred 的前一个节点相关联，并将前一个节点赋值给 pred
            node.prev = pred = pred.prev;
        } while (pred.waitStatus > 0); // 一直找到小于等于0的
        // 将重新标识好的最近的有效节点的 next 指向当前节点
        pred.next = node;
    } else {
        // 小于等于0，但是不等于-1，将上一个有效节点状态修改为-1
        compareAndSetWaitStatus(pred, ws, Node.SIGNAL);
    }
    return false;
}
```
只有节点的状态为-1，才会唤醒后一个节点，如果节点状态未设置，默认为0。<br />图解一下ws>0的过程，因为ws>0的节点为失效节点，所以do...while中会重复向前查找前驱节点，直到找到第一个ws<=0的节点为止，将node节点挂到该节点上。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968750-3e15315d-8ad9-4955-a7a0-c4dc198a5e2c.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u596c7688&originHeight=661&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u255814e1-642a-4331-9e0b-152c9bc6194&title=)<br />pred是头结点且未设置状态，所以状态为0，会走else。通过CAS尝试将pred节点的`waitStatus`设置为-1，表明node节点需要被pred唤醒。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968724-462d883b-4628-4e22-b321-ad13d8e9c746.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u42b0e87b&originHeight=646&originWidth=1047&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud38c47b7-8341-4da8-830a-32af7bdf250&title=)<br />`shouldParkAfterFailedAcquire()`返回false，继续执行`acquireQueued()`中的死循环。<br />步骤和上边一样，node的前驱节点还是head，继续尝试获取锁。如果线程1释放了锁，线程2就可以拿到，返回true；否则继续调用`shouldParkAfterFailedAcquire()`，因为上一步已经将前驱结点的ws设置为-1了，所以直接返回true。<br />执行`parkAndCheckInterrupt()`方法，通过`UNSAFE.park();`方法阻塞当前线程2。等以后执行`unpark`方法的时候，如果node是头节点后的第一个节点，会进入`acquireQueued()`方法中走`if (p == head && tryAcquire(arg))`的逻辑获取锁资源并结束死循环。
<a name="fpeNT"></a>
#### 查看`cancelAcquire()`方法
该方法执行的机率约等于0，为什么这么说呢？因为针对failed属性，只有JVM内部出现问题时，才可能出现异常，执行该方法。
```java
// node 为当前节点
private void cancelAcquire(Node node) {
    if (node == null)
        return;
    node.thread = null;
    // 上一个节点
    Node pred = node.prev;
    // 节点状态大于0，说明节点失效
    while (pred.waitStatus > 0)
        node.prev = pred = pred.prev;
    
    // 将第一个不是失效节点的后继节点声明出来
    Node predNext = pred.next;
    // 节点状态变为失效
    node.waitStatus = Node.CANCELLED;
    // node为尾节点，cas设置pred为尾节点
    if (node == tail && compareAndSetTail(node, pred)) {
        //cas将pred的next设置为null
        compareAndSetNext(pred, predNext, null);
    } else {
        int ws;
        // 中间节点
        // 如果上一个节点不是head 节点
        if (pred != head &&
            ((ws = pred.waitStatus) == Node.SIGNAL ||
             // 前边已经判断了大于0的操作，
             // pred 是需要唤醒后继节点的，所以当 waitStatus 不为 -1 时，需要将 pred 节点的 waitStatus 设置为 -1 
             (ws <= 0 && compareAndSetWaitStatus(pred, ws, Node.SIGNAL))) &&
            pred.thread != null) {
            Node next = node.next;
            if (next != null && next.waitStatus <= 0)
                // CAS 尝试将 pred 的 next 指向当前节点的 next
                compareAndSetNext(pred, predNext, next);
        } else {
            // head 节点，唤醒后继节点
            unparkSuccessor(node);
        }
    
        node.next = node; // help GC
    }
}
```
执行到while时找到前驱节点中最近的有效节点，把当前节点node挂到有效节点后边，可以过滤掉当前节点前的失效节点。声明出有效节点的第一个后继无效节点`predNext`，并把当前的node节点状态设置为失效状态。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968772-19e749ea-bd91-4830-b414-0b841457da3e.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u42535e41&originHeight=351&originWidth=730&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uab90a278-806a-46ba-b689-6cd7c9b5112&title=)<br />if中的操作：如果当前节点是尾节点，CAS尝试将最近的有效节点设置为尾节点，并将尾节点的next设置为null。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967968990-e8537913-494f-48aa-8faa-b6e2598e86b4.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u278b95c0&originHeight=389&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua6215c7c-514a-4400-9bb1-fe5bb6f8dcb&title=)<br />else中的操作：<br />如果pred节点不是头结点即中间节点，并且pred的`waitStatus`为-1或者waitStatus<=0，为了让pred节点能唤醒后继节点，需要设置为-1，并且pred节点的线程不为空。获取node节点的后继节点，如果后继节点有效，CAS尝试将pred的next指向node节点的next。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967969264-7f86702d-6828-421e-9730-bd7e8fe381a3.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=uc1b14c3f&originHeight=291&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uacf49f7a-5d51-4340-bb31-98cac3bac1a&title=)<br />当其他节点来找有效节点的时候走当前node的prev这条线，而不是再一个一个往前找，可以提高效率。<br />如果是头结点则唤醒后继节点。<br />最后将node节点的next指向自己。
<a name="a9SFe"></a>
## 解锁
释放锁是不区分公平锁和非公平锁的，释放锁的核心是将state由大于 0 的数置为 0。废话不多说，直接上代码
```java
//释放锁方法
public void unlock() {
    sync.release(1);
}


public final boolean release(int arg) {
    //尝试释放锁资源，如果释放成功，返回true
    if (tryRelease(arg)) {
        Node h = head;
        // head 不为空且 head 的 ws 不为0（如果为0，代表后边没有其他线程挂起）
        if (h != null && h.waitStatus != 0)
            // AQS的队列中有 node 在排队，并且线程已经挂起
            // 需要唤醒被挂起的 Node
            unparkSuccessor(h);
        return true;
    }
    // 代表释放一次没有完全释放
    return false;
}
```
如果释放锁成功，需要获取head节点。如果头结点不为空且`waitStatus`不为0，则证明有node在排队，执行唤醒挂起其他node的操作。
<a name="A2ud5"></a>
### 查看`tryRelease()`方法
```java
protected final boolean tryRelease(int releases) {
    //获取当前锁的状态，先进行减1操作，代表释放一次锁资源
    int c = getState() - releases;
    //如果释放锁的线程不是占用锁的线程，直接抛出异常
    if (Thread.currentThread() != getExclusiveOwnerThread())
        throw new IllegalMonitorStateException();
    boolean free = false;
    // 如果 c 为0 ，代表锁完全释放了，如果不为0，代表锁之前重入了，一次没释放掉，等待下次再次执行时，再次判断
    if (c == 0) {
        // 释放锁标志为 true，代表完全释放了
        free = true;
        // 将占用互斥锁的标识置为 null
        setExclusiveOwnerThread(null);
    }
    // 设置 state 状态
    setState(c);
    return free;
}
```
例子中线程1占用锁资源，线程1释放锁之后，state为0。进入if操作，将释放标志更新为true，将FIFO队列的`exclusiveOwnerThread`标志置为null。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967969345-b105cb65-8c56-47e1-83c5-3216bbfc2f19.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=uc15c0c82&originHeight=579&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua3bc356a-f860-4d21-be37-4fc16b44f31&title=)
<a name="AHvmB"></a>
### 查看`unparkSuccessor()`方法
用于唤醒AQS中被挂起的线程。
```java
// 注意当前的 node 节点是 head 节点
private void unparkSuccessor(Node node) {
    //获取 head 的状态
    int ws = node.waitStatus;
    if (ws < 0)
        // CAS 将 node 的 ws 设置为0，代表当前 node 接下来会舍弃
        compareAndSetWaitStatus(node, ws, 0);
    
    // 获取头节点的下一个节点
    Node s = node.next;
    // 如果下一个节点为null 或者 下一个节点为失效节点，需要找到离 head 最近的有效node
    if (s == null || s.waitStatus > 0) {
        s = null;
        // 从尾节点开始往前找不等于null且不是node的节点
        for (Node t = tail; t != null && t != node; t = t.prev)
            // 如果该节点有效，则将s节点指向t节点
            if (t.waitStatus <= 0)
                s = t;
    }
    // 找到最近的node后，直接唤醒
    if (s != null)
        LockSupport.unpark(s.thread);
    }
```
<a name="bN6mD"></a>
#### 问题解析：为什么要从尾结点往前查找呢？
因为在`addWaiter`方法中是先给prev指针赋值，最后才将上一个节点的next指针赋值，为了避免防止丢失节点或者跳过节点，必须从后往前找。<br />举例中head节点的状态为-1，通过CAS的方式将head节点的`waitStatus`设置为0。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967969360-63d4073c-519d-426f-8710-5508aa8dceca.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u4537b0ca&originHeight=631&originWidth=1034&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc7534b48-4e8d-4ef9-aae8-906b5b88c6f&title=)<br />头结点的后继节点是线程2所在的节点，不为null，所以这边会执行`unpark`操作，从上边的`acquireQueued()`内的`parkAndCheckInterrupt()`方法继续执行。
```
private final boolean parkAndCheckInterrupt() {
    LockSupport.park(this);
    //返回目标线程是否中断的布尔值:中断返回true，不中断返回false，且返回后会重置中断状态为未中断
    return Thread.interrupted();
}
```
因为线程2未中断，所以返回false。继续执行`acquireQueued()`中的死循环
```java
for (;;) {
    // 获取当前节点的上一个节点
    final Node p = node.predecessor();
    //p为头节点，尝试获取锁操作
    if (p == head && tryAcquire(arg)) {
        setHead(node);
        p.next = null;
        // 将获取锁失败标识置为false
        failed = false;
        // 获取到锁资源，不会被中断
        return interrupted;
    }
    // p 不是 head 或者 没拿到锁资源，
    if (shouldParkAfterFailedAcquire(p, node) &&
        // 基于 Unsafe 的 park方法，挂起线程
        parkAndCheckInterrupt())
        interrupted = true;
}
```
此时p是头节点，且能获取锁成功，将`exclusiveOwnerThread`设置为线程2，即线程2 获取锁资源。<br />将node节点设置为head节点，并将node节点的pre和thread设置为null。因为拿到锁资源了，node节点就不需要排队了。<br />将头节点p的next置为null，此时p节点就不在队列中存在了，可以帮助GC回收(可达性分析)。failed设置为false，表明获取锁成功；`interrupted`为false，则线程不会中断。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967969378-818ba81d-4817-4af6-95d6-4b67e8098e8c.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u4ea3a97b&originHeight=704&originWidth=936&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub499cdd3-645c-4a8b-9881-f507763aaa0&title=)
<a name="gA1gt"></a>
#### 为什么被唤醒的线程要调用`Thread.interrupted()`清除中断标记
从上边的方法可以看出，当`parkAndCheckInterrupt()`方法返回true时，即`Thread.interrupted()`方法返回了true，也就是该线程被中断了。为了让被唤醒的线程继续执行后续获取锁的操作，就需要让中断的线程像没有被中断过一样继续往下执行，所以在返回中断标记的同时要清除中断标记，将其设置为false。<br />清除中断标记之后不代表该线程不需要中断了，所以在`parkAndCheckInterrupt()`方法返回true时，要自己设置一个中断标志`interrupted = true`，为的就是当获取到锁资源执行完相关的操作之后进行中断补偿，故而需要执行`selfInterrupt()`方法中断线程。<br />以上就是加锁解锁的图解过程了。最后再来说一下公平锁和非公平锁的区别。
<a name="NOf5H"></a>
## 区别
前边已经说过了，似乎非公平锁包含了公平锁的全部操作。打开公平锁的代码，可以发现`accquire()`方法中只有该方法的实现有点区别。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666967969516-03757caf-614c-43fe-9976-6cb2c09a7ca0.jpeg#clientId=ueb829079-b0e0-4&from=paste&id=u45b86a89&originHeight=649&originWidth=788&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u98497ee5-6862-42dd-ab0e-8bf9ea0da09&title=)<br />`hasQueuedPredecessors()`返回false时才会尝试获取锁资源。该方法代码实现如下
```java
public final boolean hasQueuedPredecessors() {
    Node t = tail; 
    Node h = head;
    Node s;
    return h != t &&
        ((s = h.next) == null || s.thread != Thread.currentThread());
}
```

- `h==t`时，队列为空，表示没人排队，可以获取锁资源；
- 队列不为空，头结点有后继节点不为空且s节点获取锁的线程是当前线程也可以获取锁资源，代表锁重入操作；
<a name="Xp96R"></a>
## 总结
以上就是全部内容了，在最后再做个总结：

- 代码使用要合乎规范，避免加锁成功后，在`finally`中无法解锁；
- 理解AQS的FIFO队列以及Node的相关属性，尤其注意`waitStatus`的状态；
- 利用图加深对非公平锁源码的理解；
