Java AQS
<a name="Yu0xT"></a>
## 一、概念
注意：因为`ReentrantLock` 类可以实现公平锁和非公平锁，所以本文的讲解以该类为主。
<a name="uMjXM"></a>
### 1.1 公平锁
多个线程按照申请锁的顺序去获得锁，线程会直接进入队列去排队，永远都是队列的第一位才能得到锁。
<a name="ax60N"></a>
### 1.2 非公平锁
多个线程去获取锁的时候，会直接去尝试获取，获取不到，再去进入等待队列，如果能获取到，就直接获取到锁。在这个过程中，会和队列中的线程竞争，得到锁的顺序并不一定是先到先得。
<a name="wy2Tu"></a>
### 1.3 `ReentrantLock` 类
`ReentrantLock`是一个可重入且独占式的锁，并且同时实现了公平锁和非公平锁，所以本文的讲解以该类为主。
<a name="GPL8C"></a>
##### 1.3.1 `Sync`
Sync是`RenntrantLock`类的一个内部抽象类，官方解释为是一个帮助类。该类继承了AQS，所以具备AQS的大部分属性和方法，同时又重写了一些必要方法：`tryRelease()`,`isHeldExclusively()`,也就是说继承该类的类都具有该方法。<br />同时该类有新加一些自己的方法，供具体的实现类使用：`nonfairTryAcquire()`，`newCondition()`。`nonfairTryAcquire()`是为了非公平锁模式下竞争锁，`newCondition()`，最终是为了创建条件队列。
<a name="fPniP"></a>
##### 1.3.2 `NonfairSync`
`NonfairSync`继承了`Sync`,是一个实现类，重要的方法有：`lock()`,`tryAcquire(int acquires)`
<a name="o2Kid"></a>
##### 1.3.3 `FairSync`
`FairSync`继承了`Sync`,是一个实现类，重要的方法有：`lock()`,`tryAcquire(int acquires)`<br />总结：公平锁和非公平锁在概念上的区分：是否优先进入队列。优先进入队列的线程是公平锁，优先抢占资源的是非公平锁。
<a name="aRwl8"></a>
## 二、公平锁和非公平锁重要方法
<a name="XKTBj"></a>
### 2.1 `lock`
使用`RenntrantLock`构造函数可以构建公平锁和非公平锁，两者区别主要体现在`lock`方法中，具体有两处，构造方法如下：
```java
//默认非公平锁
public ReentrantLock() {
    sync = new NonfairSync();
}

//传入true，可以构建公平锁
public ReentrantLock(boolean fair) {
    sync = fair ? new FairSync() : new NonfairSync();
}
```
`ReentrantLock`的获取锁方法的路径：`lock()`->`acquire()`，其中`acquire()`方法可以参考AQS之理论知识（一）中的讲解，两者的区别是`lock`方法以及不同实现类重写的`tryAcquire()`。
<a name="bCR36"></a>
##### 2.2.1 NonfairSync 的lock
```java
final void lock() {
    //区别一：非公平锁竞争锁资源会先去竞争锁，而公平锁只会在锁状态为0时才会竞争锁
    if (compareAndSetState(0, 1))
        setExclusiveOwnerThread(Thread.currentThread());
    else
        acquire(1);
}
```
调用`NonfairSync` 的`lock`，直接竞争锁，成功的话直接返回。失败的话，执行`acquire()`，因为AQS是模块方法，所以`NonfairSync` 需要重写`tryAcquire()`，在这个方法中当锁状态为0时直接竞争锁，无需查看队列是否有线程。
<a name="ncN7a"></a>
##### 2.2.2 `FairSync`的`lock`
```java
final void lock() {
    acquire(1);
}

protected final boolean tryAcquire(int acquires) {
    final Thread current = Thread.currentThread();
    int c = getState();
    if (c == 0) {
        //区别二：查看队列里面是否有节点，有的话，加入队列
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
调用`FairSync`的`lock`，直接执行`acquire()`，因为AQS是模块方法，所以`FairSync`需要重写`tryAcquire()`，在这个方法中当锁状态为0时直接竞争锁，需要查看队列是否有线程，有线程的话将新线程添加到队列中，其他业务和非公平锁一致。<br />总结：区别一：非公平锁竞争锁资源会先去竞争锁，而公平锁只会在锁状态为0时才会竞争锁；<br />区别二：公平锁查看队列里面是否有节点，有的话，加入队列，没有的话直接区抢锁；而非公平锁则是直接抢锁。
<a name="qX0er"></a>
### 2.2 `unlock`
```java
public void unlock() {
    sync.release(1);
}

public final boolean release(int arg) {
    if (tryRelease(arg)) {
        Node h = head;
        if (h != null && h.waitStatus != 0)
            unparkSuccessor(h);
        return true;
    }
    return false;
}
```
`RenntrantLock`的解锁调用的是AQS的`release()`,而解锁调用的`tryRelease(arg)`是两者共用的。所以解锁的步骤依然是两大步骤：
<a name="cEiES"></a>
#### 2.2.1 更改锁状态，如果锁被完全释放，同时将独占锁绑定的线程清空；
<a name="VE484"></a>
#### 2.2.2 唤醒队列的节点绑定的线程。
<a name="JUZJl"></a>
### 2.3 `await`
因为`await`方法最终调用的是`Condition`中的方法，所以AQS中`await`方法无法重写，都是公用AQS现成的方法。
<a name="CWqY4"></a>
### 2.4 `signal`
因为`signal`方法最终调用的是`Condition`中的方法，所以AQS中`signal`方法无法重写，都是公用AQS现成的方法。
<a name="qJKuz"></a>
## 三、总结
<a name="BaFwg"></a>
##### 3.1 公平锁和非公平锁在概念上的区分：是否优先进入队列。优先进入队列的线程是公平锁，优先抢占资源的是非公平锁；
<a name="GYeuZ"></a>
##### 3.2公平锁和非公平锁的在`ReentrantLock`实现上的区别：
区别一：非公平锁竞争锁资源会先去竞争锁，而公平锁只会在锁状态为0时才会竞争锁；<br />区别二：公平锁查看队列里面是否有节点，有的话，加入队列，没有的话直接区抢锁；而非公平锁则是直接抢锁。
