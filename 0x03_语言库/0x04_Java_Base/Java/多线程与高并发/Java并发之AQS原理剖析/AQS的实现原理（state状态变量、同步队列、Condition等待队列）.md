Java AQS state Condition
<a name="pRRhG"></a>
## 一、 总体思想
通过尝试获取共享变量 state 的结果来对线程的状态作出处理。<br />获取成功的线程CAS修改state的之后直接进行自己的处理。<br />未能成功获取共享变量的线程会被封装成结点放入 一个队列中，然后 自旋的检查自己的状态，看是否能再次去获取state资源，获取成功则退出当前自旋状态，获取失败则找一个安全的点（成功的找到一个状态<0前驱结点，然后将其状态设置为`SIGNAL`），调用`LockSupport.park()`方法进入waiting状态。然后等待被前驱结点调用release方法（实际上是调用 `LockSupport.unPark()`）或者被中断唤醒。
<a name="YPsrA"></a>
## 二、 独占式获取资源的过程
```java
public final void acquire(int arg) {
    if (!tryAcquire(arg) &&
        acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
        selfInterrupt();
}
```
调用 `acquire(int)`模板方法进行资源的获取：<br />调用使用AQS同步类自己实现的`tryAcquire(int)` 方法进行获取，获取成功，就直接返回了。<br />获取失败调用Node `addWaiter(Node.EXCLUSIVE)` 方法将当前线程封装成一个Node结点，放入队列的尾部：<br />首先直接直接使用CAS尝试一次看能否设置成功，能够设置成功的话返回Node(Node结点的next与pre都是使用`volatile`修饰的)；<br />未能成功设置成功的话调用`enq(node);` （自旋+CAS+volatile）去自旋的不断尝试，直到放到队列的尾部。<br />放入队列尾部后，接下来调用`boolean acquireQueued(final Node node, int arg)` 方法，找到一个可用的前驱结点（状态<0）将其结点状态设置为`SIGNAL`（执行完通知后继结点），然后就可以进入`waiting`状态：<br />在一个自旋过程中，判断当前结点的前驱结点是否是head，是的话使用`tryAcquire()`去获取资源，获取成功后将head设置为当前结点，然后 return 当前线程是否是被中断的。<br />如果当前结点的前驱结点不是head：
```java
if (shouldParkAfterFailedAcquire(p, node) &&parkAndCheckInterrupt()){
    //如果等待过程中被中断过，哪怕只有那么一次，就将interrupted标记为true
    interrupted = true;
}
```
调用`shouldParkAfterFailedAcquire`(前驱结点, 当前结点)去找可用的当前结点的可用的前驱结点，并把前驱结点的状态设置为`SIGNAL`，设置成功返回true。然后调用`parkAndCheckInterrupt()`去将当前线程状态置为`waiting`状态，并在从`waiting`状态返回时获取线程是否是被中断的。
```java
private final boolean parkAndCheckInterrupt() {
    LockSupport.park(this);//调用park()使线程进入waiting状态
    return Thread.interrupted();//如果被唤醒，查看自己是不是被中断的。
}
```
<a name="jXSa5"></a>
## 三、独占模式下释放资源
调用 `release(int)`去执行释放过程：<br />先使用 `tryRelease(int)`去尝试能否成功释放，不能直接返回false；<br />若`tryRelease`释放成功（state==0）了，则拿到head，然后找到一个可用的结点（状态值<=0），调用`LockSupport.unpark(thread);`方法，唤醒可用结点中的线程。
<a name="C6VQS"></a>
## 四、共享模式
<a name="pkNAd"></a>
### 1. 共享模式下获取资源（acquireShared(int)）
共享模式下，AQS使用者调用 `int tryAcquireShared(int)`来进行资源尝试获取，与`tryAcquire(int)`返回`boolean`不同，`tryAcquireShared`尝试获取资源时，对返回值做了约定：<br />即 >=0 代表申请资源成功（值为剩余资源），直接返回；<0 代表申请资源失败，然后调用 `doAcquireShared(arg)` 将未申请成功的线程封装成结点放入队列，然后进行等待状态，直到被唤醒。<br />这里整体与独占式一样，但在第一次进入自旋或者被唤醒后发现前驱是head时的处理不一样，这里会在进行`tryAcquireShared`后，会再调用`setHeadAndPropagate(node, r);`方法将head设置为自己，同时将本次未用完的资源分配给后面等待的线程。
<a name="lL6K9"></a>
### 2. 共享模式下释放资源（`releaseShared(int)`）
这里与独占式一样，也是从head向后，一直找到一个可用的后继结点，然后唤醒他（`LockSupport.unPark()`）。
<a name="ORBS1"></a>
## 五、`ReentrantLock`
<a name="ifYu3"></a>
### 1、公平锁与非公平锁的体现
有`acquire`方法可知，只要`tryAcquire`返回成功，则代表获取锁成功。所以是否公平的实现是在 `tryAcquire`方法的具体实现。<br />`ReentrantLock`的非公平实现：<br />即，在`state==0`时，未看是否队列中已经存在等待的线程结点，而直接去CAS了。
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
`Reentrant`的公平锁实现：<br />这里多加了个`asQueuedPredecessors()`方法的判断，即去看队列中是否已经存在着等待的结点。所以若判断存在`tryAcquire`直接返回false，对应的`acquire`模板方法会把当前线程也加入到队列中。
```java

protected final boolean tryAcquire(int acquires) {
    final Thread current = Thread.currentThread();
    int c = getState();
    if (c == 0) {
        if (!hasQueuedPredecessors() &&
            compareAndSetState(0, acquires)) {
            setExclusiveOwnerThread(current);
            return true;
        }
    }
    else if (current == getExclusiveOwnerThread()) {
        int nextc = c + acquires;
        if (nextc < 0)
            throw new Error("Maximum lock count exceeded");
        setState(nextc);
        return true;
    }
    return false;
}
```
从这里也可以看出为啥非公平锁会比公平锁效率高了：因为可能存在省去一次加入队列进入`waiting`状态，并被从`waiting`状态唤醒而造成的用户态线程与内核态线程的切换带来的性能消耗。<br />这里只是可能，因为可能此刻可用的资源已被队列中唤醒的线程拿到了。<br />**六、**`**Condition**`**原理**<br />总的来说，`Condition`的本质就是等待队列和同步队列的交互：<br />当一个持有锁的线程调用`Condition.await()`时，它会执行以下步骤：<br />1.构造一个新的等待队列节点加入到等待队列队尾<br />2.释放锁，也就是将它的同步队列节点从同步队列队首移除<br />3.自旋，直到它在等待队列上的节点移动到了同步队列（通过其他线程调用signal()）或被中断<br />4.阻塞当前节点，直到它获取到了锁，也就是它在同步队列上的节点排队排到了队首。<br />当一个持有锁的线程调用`Condition.signal()`时，它会执行以下操作：<br />从等待队列的队首开始，尝试对队首节点执行唤醒操作；如果`CANCELLED`，就尝试唤醒下一个节点；如果再`CANCELLED`则继续迭代。<br />对每个节点执行唤醒操作时，首先将节点加入同步队列，此时`await()`操作的步骤3的解锁条件就已经开启了。然后分两种情况讨论：<br />如果先驱节点的状态为`CANCELLED(>0)` 或设置先驱节点的状态为`SIGNAL`失败，那么就立即唤醒当前节点对应的线程，此时await()方法就会完成步骤3，进入步骤4.<br />如果成功把先驱节点的状态设置为了SIGNAL，那么就不立即唤醒了。等到先驱节点成为同步队列首节点并释放了同步状态后，会自动唤醒当前节点对应线程的，这时候`await()`的步骤3才执行完成，而且有很大概率快速完成步骤4.

