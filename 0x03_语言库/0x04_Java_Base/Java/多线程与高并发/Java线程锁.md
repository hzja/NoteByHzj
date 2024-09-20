Java
<a name="OwsyF"></a>
### 多线程
说到锁，那么绕不开的就是这个多线程，在出现了进程之后，操作系统的性能得到了大大的提升。虽然进程的出现解决了操作系统的并发问题，但是人们仍然不满足，人们逐渐对实时性有了要求。<br />使⽤多线程的理由之一是和进程相比，它是一种非常花销小，切换快，更”节俭”的多任务操作方式。<br />在 Linux 系统下，启动⼀个新的进程必须分配给它独立的地址空间，建立众多的数据表来维护它的代码段、堆栈段和数据段，这是一种”昂贵”的多任务工作方式。<br />而在进程中的同时运行多个线程，它们彼此之间使用相同的地址空间，共享大部分数据，启动一个线程所花费的空间远远小于启动一个进程所花费的空间，而且，线程间彼此切换所需的时间也远远小于进程间切换所需要的时间。<br />也就是说，使用多线程，更多的是直接来优化业务逻辑，就是是提升代码的质量和可用性，并且能解决一些问题，所以就会有这个多线程的使用了。
<a name="T8kJH"></a>
### 多线程的问题
既然多线程能够解决一些问题，那么势必也会有一些小小的缺点，而这些缺点也是应该去解决的内容。<br />比如多线程的并发问题；<br />由于多个线程是共同占有所属进程的资源和地址空间的，那么就会存在一个问题：<br />如果多个线程要同时访问某个资源，怎么处理？<br />在 Java 并发编程中，经常遇到多个线程访问同一个共享资源 ，这时候作为开发者必须考虑如何维护数据一致性，这就是 Java 锁机制(同步问题)的来源。<br />Java提供了多种多线程锁机制的实现方式，常见的有：

- `synchronized`
- `ReentrantLock`
- `Semaphore`
- `AtomicInteger`

每种机制都有优缺点与各自的适用场景，必须熟练掌握他们的特点才能在Java多线程应用开发时得心应手。<br />接下来就分开来说一下四个机制的优缺点：
<a name="mbrl7"></a>
### `synchronized`
Java开发人员都认识 `synchronized `，使用它来实现多线程的同步操作是非常简单的，只要在需要同步的对方的方法、类或代码块中加入该关键字，它能够保证在同一个时刻最多只有一个线程执行同一个对象的同步代码，可保证修饰的代码在执行过程中不会被其他线程干扰。<br />使用 `synchronized` 修饰的代码具有原子性和可见性，在需要进程同步的程序中使用的频率非常高，可以满足⼀般的进程同步要求。
```java
synchronized (obj) {
    //方法
    …….
}
```
到了Java1.6，`synchronized`进行了很多的优化，有适应自旋、锁消除、锁粗化、轻量级锁及偏向锁等，<br />效率有了本质上的提高。在之后推出的Java1.7与1.8中，均对该关键字的实现机理做了优化。<br />需要说明的是，当线程通过 `synchronized` 等待锁时是不能被 `Thread.interrupt()` 中断的，因此程序设计时必须检查确保合理，否则可能会造成线程死锁的尴尬境地。<br />强烈推荐在多线程应用程序中使用该关键字，因为实现方便，后续工作由 JVM 来完成，可靠性高。只有在确定锁机制是当前多线程程序的性能瓶颈时，才考虑使用其他机制，如 `ReentrantLock` 等
<a name="WSeng"></a>
### `ReentrantLock`
可重入锁，顾名思义，这个锁可以被线程多次重复进入进行获取操作。<br />ReentantLock 继承接口 Lock 并实现了接口中定义的方法，除了能完成 `synchronized` 所能完成的所有工作外，还提供了诸如可响应中断锁、可轮询锁请求、定时锁等避免多线程死锁的方法。<br />Lock 实现的机理依赖于特殊的 CPU 指定，可以认为不受 JVM 的约束，并可以通过其他语言平台来完成底层的实现。在并发量较小的多线程应用程序中，`ReentrantLock` 与 `synchronized` 性能相差无几，但在高并发量的条件下，`synchronized` 性能会迅速下降几十倍，而 `ReentrantLock` 的性能却能依然维持⼀个水准。<br />因此建议在高并发量情况下使用 `ReentrantLock`。<br />一般 `ReentrantLock` 是分为公平锁和非公平锁。<br />公平锁指的是锁的分配机制是公平的，通常先对锁提出获取请求的线程会先被分配到锁。<br />反之，JVM按随机、就近原则分配锁的机制则称为不公平锁。
```java
Lock lock = new ReentrantLock();
try {
    lock.lock();
    //…进⾏任务操作5 }
finally {
    lock.unlock();
}
```
上面的是伪代码，但是需要注意一个比较有意思的地方，那么就是**使用**`**ReentrantLock**`**必须在**`**finally**`**控制块中进行解锁操作**
<a name="TKqt9"></a>
### `Semaphore`
`Semaphore`基本能完成`ReentrantLock`的所有工作，使用方法也与之类似，通过`acquire()`与`release()`方法来获得和释放临界资源。<br />其实 `ReentrantLock` 和 `synchronized` 都是互斥锁，也就是说相当于只存在一个临界资源，因此同时最多只能给一个线程提供服务。<br />调用`Semaphore.acquire()` 方法，它本质上是调用的`acquireSharedInterruptibly(int)`，参数为1。
```java
// arg 等于 1
public final void acquireSharedInterruptibly(int arg)
        throws InterruptedException {
    if (Thread.interrupted())
        throw new InterruptedException();
    // 关于tryAcquireShared,Semaphore有两种实现
    // 一种是公平锁,另一种是非公平锁. 这分析非公平锁.
    if (tryAcquireShared(arg) < 0)
        // 调用 AQS#doAcquireSharedInterruptibly(1) 方法
        doAcquireSharedInterruptibly(arg);
}
```
而他的释放锁，调用的是 `release()` 方法，其中调用的则是下面：
```java
// 释放共享锁
public final boolean releaseShared(int arg) {
    // 调用Semaphore#tryReleaseShared方法.
    if (tryReleaseShared(arg)) {
        // tryReleaseShared释放成功, 则释放双向链表中head的后继
        doReleaseShared();
        return true;
    }
    return false;
}
```
<a name="ltZbk"></a>
### `AtomicInteger`
在多线程程序中，诸如 `++i` 或 `i++` 等运算不具有原子性，是不安全的线程操作之一。通常会使用 `synchronized` 将该操作变成一个原子操作，但 JVM 为此类操作特意提供了一些同步类，使得使用更方便，且使程序运行效率变得更高。通过相关资料显示，通常 `AtomicInteger` 的性能是 `ReentantLock` 的好几倍。<br />而 `AtomicInteger` 用于多线程下线程安全的数据读写操作，避免使用锁同步，底层采用CAS实现，内部的存储值使用 `volatile` 修饰，因此多线程之间是修改可见的。<br />用法如下：
```java
private AtomicInteger counter = new AtomicInteger(0);//初始计数为0
// dosomething，执行操作之后，计数即可
int count = counter.incrementAndget();
```
