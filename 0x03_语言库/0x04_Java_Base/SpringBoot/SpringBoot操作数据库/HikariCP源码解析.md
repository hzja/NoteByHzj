JavaSpringBoot<br />HikariCP是SpringBoot默认的数据库连接池，它毫不谦虚的的起了一个叫做光的名字，来看一下Hikari中的`ConcurrentBag`。
<a name="d9uPg"></a>
## 核心数据结构
多线程代码一个让人比较头疼的问题，就是每个API都懂，但就是不会用。很多对concurrent包倒背如流的同学，在面对现实的问题时，到最后依然不得不被迫加上`Lock`或者`synchronized`。<br />`ConcurrentBag`是一个Lock free的数据结构，主要用作数据库连接的存储，可以说整个HikariCP的核心就是它。删掉乱七八糟的注释和异常处理，可以说关键的代码也就百十来行，但里面的道道却非常的多。<br />`ConcurrentBag`速度很快，要达到这个目标，就需要一定的核心数据结构支持。
```java
private final CopyOnWriteArrayList<T> sharedList;
private final ThreadLocal<List<Object>> threadList;
private final AtomicInteger waiters;
private final SynchronousQueue<T> handoffQueue;
```

- `sharedList` 用来缓存所有的连接，是一个`CopyOnWriteArrayList`结构。
- `threadList` 用来缓存某个线程所使用的所有连接，相当于快速引用，是一个ThreadLocal类型的ArrayList。
- `waiters` 当前正在获取连接的等待者数量。`AtomicInteger`，就是一个自增对象。当waiters的数量大于0时候，意味着有线程正在获取资源。
- `handoffQueue` 0容量的快速传递队列，`SynchronousQueue`类型的队列，非常有用。

`ConcurrentBag`里面的元素，为了能够无锁化操作，需要使用一些变量来标识现在处于的状态。抽象的接口如下：
```java
public interface IConcurrentBagEntry{
    int STATE_NOT_IN_USE = 0;
    int STATE_IN_USE = 1;
    int STATE_REMOVED = -1;
    int STATE_RESERVED = -2;

    boolean compareAndSet(int expectState, int newState);
    void setState(int newState);
    int getState();
}
```
有了这些数据结构的支持，`ConcurrentBag`就可以实现它光的宣称了。
<a name="EwQSL"></a>
## 获取连接
连接的获取是`borrow`方法，还可以传入一个timeout作为超时控制。
```java
public T borrow(long timeout, final TimeUnit timeUnit) throws InterruptedException
```
首先，如果某个线程执行非常快，使用了比较多的连接，就可以使用ThreadLocal的方式快速获取连接对象，而不用跑到大池子里面去获取。代码如下。
```java
// Try the thread-local list first
final var list = threadList.get();
for (int i = list.size() - 1; i >= 0; i--) {
    final var entry = list.remove(i);
    final T bagEntry = weakThreadLocals ? ((WeakReference<T>) entry).get() : (T) entry;
    if (bagEntry != null && bagEntry.compareAndSet(STATE_NOT_IN_USE, STATE_IN_USE)) {
        return bagEntry;
    }
}
```
都知道，包括ArrayList和HashMap一些基础的结构，都是Fail Fast的，如果在遍历的时候，删掉一些数据，有可能会引起问题。幸运的是，由于List是从`ThreadLocal`获取的，它首先就避免了线程安全的问题。<br />接下来就是遍历。这段代码采用的是尾遍历（头遍历会出现错误），用于快速的从列表中找到一个可以复用的对象，然后使用CAS来把状态置为使用中。但如果对象正在被使用，则直接删除它。<br />在`ConcurrentBag`里，每个`ThreadLocal`最多缓存50个连接对象引用。<br />当`ThreadLocal`里找不到可复用的对象，它就会到大池子里去拿。也就是下面这段代码。
```java
// Otherwise, scan the shared list ... then poll the handoff queue
final int waiting = waiters.incrementAndGet();
try {
   for (T bagEntry : sharedList) {
      if (bagEntry.compareAndSet(STATE_NOT_IN_USE, STATE_IN_USE)) {
         // If we may have stolen another waiter's connection, request another bag add.
         if (waiting > 1) {
            listener.addBagItem(waiting - 1);
         }
         return bagEntry;
      }
   }

   listener.addBagItem(waiting);
   
   // 还拿不到，就需要等待别人释放了
   timeout = timeUnit.toNanos(timeout);
   do {
      final var start = currentTime();
      final T bagEntry = handoffQueue.poll(timeout, NANOSECONDS);
      if (bagEntry == null || bagEntry.compareAndSet(STATE_NOT_IN_USE, STATE_IN_USE)) {
         return bagEntry;
      }

      timeout -= elapsedNanos(start);
   } while (timeout > 10_000);

   return null;
}
finally {
   waiters.decrementAndGet();
}

```
首先要注意，这段代码可能是由不同的线程执行的，所以必须要考虑线程安全问题。由于shardList是线程安全的`CopyOnWriteArrayList`，适合读多写少的场景，可以直接进行遍历。<br />这段代码的目的是一样的，需要从sharedList找到一个空闲的连接对象。这里把自增的waiting变量传递到外面的代码进行处理，主要是由于想要根据waiting的大小来确定是否创建新的对象。<br />如果无法从池子里获取连接，则需要等待别的线程释放一些资源。<br />创建对象的过程是异步的，要想获取它，还需要依赖一段循环代码。`while`循环代码是纳秒精度，会尝试从`handoffQueue`里获取。最终会调用`SynchronousQueue`的`transfer`方法。
<a name="O9Ufr"></a>
## 归还连接
有借就有还，当某个连接使用完毕，它将被归还到池子中。
```java
public void requite(final T bagEntry){
    bagEntry.setState(STATE_NOT_IN_USE);

    for (var i = 0; waiters.get() > 0; i++) {
        if (bagEntry.getState() != STATE_NOT_IN_USE || handoffQueue.offer(bagEntry)) {
            return;
        }
        else if ((i & 0xff) == 0xff) {
            parkNanos(MICROSECONDS.toNanos(10));
        }
        else {
            Thread.yield();
        }
    }

    final var threadLocalList = threadList.get();
    if (threadLocalList.size() < 50) {
        threadLocalList.add(weakThreadLocals ? new WeakReference<>(bagEntry) : bagEntry);
    }
}

```
首先，把这个对象置为可用状态。然后，代码会进入一个循环，等待使用方把这个连接接手过去。当连接处于`STATE_NOT_IN_USE`状态，或者队列中的数据被取走了，那么就可以直接返回了。<br />由于`waiters.get()`是实时获取的，有可能长时间一直大于0，这样代码就会变成死循环，浪费CPU。代码会尝试不同层次的睡眠，一个是每隔255个waiter睡10ns，一个是使用yield让出cpu时间片。<br />如果归还连接的时候并没有被其他线程获取到，那么最后会把归还的连接放入到相对应的`ThreadLocal`里，因为对一个连接来说，借和还，通常是一个线程。
<a name="BwQ7Z"></a>
## 知识点
看起来平平无奇的几行代码，为什么搞懂了就能Hold住大部分的并发编程场景呢？主要还是这里面的知识点太多。下面简单罗列一下，可以逐个攻破。

1. 使用`ThreadLocal`来缓存本地资源引用，使用线程封闭的资源来减少锁的冲突
2. 采用读多写少的线程安全的`CopyOnWriteArrayList`来缓存所有对象，几乎不影响读取效率
3. 使用基于CAS的`AtomicInteger`来计算等待者的数量，无锁操作使得计算更加快速
4. 0容量的交换队列`SynchronousQueue`，使得对象传递更加迅速
5. 采用`compareAndSet`的CAS原语来控制状态的变更，安全且效率高。很多核心代码都是这么设计的
6. 在循环中使用park、yield等方法，避免死循环占用大量CPU
7. 需要了解并发数据结构中的`offer`、`poll`、`peek`、`put`、`take`、`add`、`remove`方法的区别，并灵活应用
8. CAS在设置状态时，采用了`volatile`关键字修饰，对于`volatile`的使用也是一个常见的优化点
9. 需要了解`WeakReference`弱引用在垃圾回收时候的表现
