Java
<a name="r6azv"></a>
## 什么是CAS
所谓的AQS既是`compareAndSwap`的缩写，翻译过来既是“对比和交换”的意思。<br />那怎么去对比呢，画个小图理解下：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658409521746-bfb57c60-02cb-439c-8f2f-b7d898a431ad.jpeg)<br />3个值分别对应的含义：

- 新的值：需要修改过后的值
- 内存变量的值：在内存的值是多少
- 旧的预期值：这里相当于内存的值的一个副本，copy过来的如果内存的值等于旧的预期值，那么就把新的值替换为内存中的值，否则什么都不做这里就是自旋锁

`AtomicInteger`就是用到CAS来实现的。用代码理解下：
```c
// 类似上述全局内存共享变量V
AtomicInteger atomicInteger = new AtomicInteger(0);
// CAS操作
boolean b = atomicInteger.compareAndSet(0, 1);
System.out.println("是否修改成功:" + b);
System.out.println("修改后的值："+ atomicInteger.get());
```
打印：
> 是否修改成功：true 修改后的值：1

那么此时有多个线程同时请求如下：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658409522667-1c6e9671-d8e2-4be3-be6d-5e19c8a15ff5.jpeg)<br />线程T1，T2同时需要修改内存中的变量值“加1”操作流程如下

- 此时线程T1，T2内存中的值都为0
- T1：此时V=0，E=0，发现V=E，那么把V修改为N，那么现在V=1
- T2：此时V=1，E=0，发下V!=E，那么重新读取(自旋)内存中的值把E的值改成1，这时V=E了。
<a name="hMRp1"></a>
### ABA问题
**什么是ABA问题。假设变量0被线程B修改成了2，再被自己或者其他线程又修改成了0，那么对于线程A来说，并没有感知到。因为线程A对比了0和0还是相等的。这就是ABA问题。那么如何解决呢？**<br />**加版本号处理。怎么处理？**<br />**也就是不管哪个线程修改了变量值，都把版本号加一。那么其他线程读取的时候就感知到了。原来被修改过**<br />**底层其实是通过C语去实现的**<br />C语言指令：
```c
lock cmpxchg 
```
<a name="tTRkv"></a>
## 什么是AQS
所谓的AQS既是`AbstractQueuedSynchronizer`的缩写<br />`**AbstractQueuedSynchronizer**`**(AQS)是JDK中实现并发编程的核心，平时工作中经常用到的**`**ReentrantLock**`**，**`**CountDownLatch**`**等都是基于它来实现的。**<br />以`ReentrantLock`为例：
```java
ReentrantLock reentrantLock = new ReentrantLock();
reentrantLock.lock();
```
底层源码其实是通过`Sync`这个来实现的：
```java
public void lock() {
	sync.lock();
}
```
再点进去发现，其实就是`AbstractQueuedSynchronizer`的子类
```java
abstract static class Sync extends AbstractQueuedSynchronizer {
	private static final long serialVersionUID = -5179523762034025860L;

	/**
	 * Performs {@link Lock#lock}. The main reason for subclassing
	 * is to allow fast path for nonfair version.
	 */
	abstract void lock();
	...
}
```
此时发现lock抽象方法的实现有两个<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658409209918-53eeb52c-f430-4563-8cc6-a86bdff9784f.png#clientId=uf3c00530-9c88-4&from=paste&height=98&id=ubdc71ffa&originHeight=245&originWidth=2477&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54808&status=done&style=shadow&taskId=ubfd2ef40-ae2e-453d-b0c8-bc0396fdff1&title=&width=990.8)<br />分别是`FairSync`（公平锁）和`NonfairSync`（非公平锁）均属于`ReentrantLock`内部类。<br />那么怎么决定使用公平锁或非公平锁呢，可以通过`ReentrantLock`的构造函数，默认是用非公平锁。
```java
public ReentrantLock() {
	sync = new NonfairSync();
}
public ReentrantLock(boolean fair) {
	sync = fair ? new FairSync() : new NonfairSync();
}
```
<a name="r17bK"></a>
## 公平锁和非公平锁

- 公平锁：所谓的公平锁呢会按照严格的执行顺序来分配的。
- 非公平锁：竞争锁的线程允许来插队来抢占锁的资源。

画个小图理解下：
<a name="GQi0O"></a>
### 公平锁<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1658408924207-ed7d6e24-e552-4b3f-8b31-fb60df3cedee.webp#clientId=uf3c00530-9c88-4&from=paste&id=u6d216ae0&originHeight=574&originWidth=1182&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u08aaf380-2ffe-4f46-96b9-1e6ad5ec9d9&title=)
此时有4个线程分别是**T1,T2,T3,T4**,**同时**调用这个方法。那么必然只会有**一个线程**会拿到锁。<br />假设此时T1线程拿到了锁，那么其他T2,T3,T4会阻塞。那么作为公平锁怎么处理呢，让其实现一个**公平**状态<br />可以使其T2,T3,T4放入一个链表中等待，那么T1释放的锁，这个时候呢T1不会唤醒T2,T3,T4，只会唤醒（LockSupport.unpark）T2，**交给T2持有锁**，<br />那么T1呢，加入链表后续等待。就变成了这样。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1658408913328-678ea9ed-5ab6-454b-a746-dd8a00fc0999.webp#clientId=uf3c00530-9c88-4&from=paste&id=ue18b7ee2&originHeight=264&originWidth=1013&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua512ec53-ffe3-4453-aa0c-e7c2b4b46f4&title=)<br />**那什么是非公平的呢**？非公平其实就是通过cpu去争抢，在多线程的场景下如果哪个线程抢到了锁，那么就会持有这把锁。<br />有的时候就会发现多线程的场景下会有相同的线程去执行。而并非上述的T1,T2,T3,T4按照顺序去执行。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1658408902237-ccb1ebfe-6eff-414f-9f78-c6b5aa82dba5.webp#clientId=uf3c00530-9c88-4&from=paste&id=u4c2382d0&originHeight=551&originWidth=1200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u710d159e-8c8f-4853-ab7a-2cb629b11f4&title=)
<a name="O0OB1"></a>
### 非公平锁
首先T1,T2,T3,来CAS抢占锁，假设此时T1抢占成功了，那么这时T2,T3会阻塞，怎么阻塞呢，他们会加入到一个链表中等待，<br />_注意：这是一个双向链表，且有头有尾。首节点是获取到锁的节点，其他的都是阻塞的节点_<br />这时在总结写公平锁与非公平锁的**优缺点**：

- 优点：所有的线程都能得到资源，不会饿死在队列中。
- 缺点：吞吐量会下降很多，队列里面除了第一个线程，其他的线程都会阻塞，cpu唤醒阻塞线程的开销会很大。

非公平锁：多个线程去获取锁的时候，会直接去尝试获取，获取不到，再去进入等待队列，如果能获取到，就直接获取到锁。

- 优点：可以减少CPU唤醒线程的开销，整体的吞吐效率会高点，CPU也不必取唤醒所有线程，会减少唤起线程的数量。
- 缺点：可能也发现了，这样可能导致队列中间的线程一直获取不到锁或者长时间获取不到锁，导致饿死。
<a name="LABat"></a>
## 非公平锁的设计理念
其实像ReentrantLock还是像synchronized等都是一样，默认都是非公平锁的策略，之所以这么设计，是考虑到性能这方面的原因，<br />因为如果按照公平锁的策略去进行阻塞等待，同时AQS又唤醒正在等待的线程，这里会涉及到一个内核态的切换，对性能会有一定的影响。如果是非公平锁呢，当前线程正好在上一个线程释放的临界点去抢占到了锁这意味着这个线程不会进行内核态的切换<br />AQS双向链表的作用?

1. 线程中断 需要删除当前线程被封装的Node节点 这是就变成了链表的删除操作<br />如果想从CLH 单向链表中间删除一个 Node，因为只维护了前一个节点的指针，想要知道后一个节点的指针的话，不通过从tail开始使用快慢指针遍历是无法办到的。

因此直接维护prev、next指针，以降低删除操作的复杂性。说白了就是降低删除的复杂度

2. 唤醒后续线程 而当多线程竞争时，CLH的轮询是非常耗费性能的，无论是对CPU还是总线来说，都是一种巨大的压力。

所以在CLH的prev基础上增加了next。
