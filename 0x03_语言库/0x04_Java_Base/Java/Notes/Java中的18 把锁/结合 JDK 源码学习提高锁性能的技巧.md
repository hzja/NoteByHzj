Java锁
<a name="RR5AP"></a>
## 1、减少锁持有时间
对于使用锁进行并发控制的应用程序而言，在锁竞争过程中，单个线程对锁的持有时间与系统性能有着直接的关系。<br />如果线程持有锁的时间越长，那么相对地，锁的竟争程度也就越激烈。<br />可以想象一下，如果要求 100 个人各自填写自己的身份信息，但是只给他们一支笔，那么如果每个人拿着笔的时间都很长，总体所花的时间就会很长。<br />如果真的只有一支笔共享给 100 个人用，那么最好让每个人花尽量少的时间持笔，务必做到想好了再拿笔写，千万不能拿着笔才去思考这表格应该怎么填。<br />程序开发也是类似的，应该尽可能地减少对某个锁的占有时间，以减少线程间互斥的可能。<br />以下面的代码段为例:
```java
public synchronized void syncMethod(){
    othercode1(); 
    mutextMethod(); 
    othercode2();
}
```
在 `syncMethod()` 方法中，假设只有 `mutextMethod()` 方法是有同步需要的，而 o`thercode1()` 方法和 `othercode2()` 方法并不需要做同步控制。<br />如果 `othercode1()` 和 `othercode2()` 分别是重量级的方法，则会花费较长的 CPU 时间。<br />如果在并发量较大时，使用这种对整个方法做同步的方案，则会导致等待线程大量增加。<br />因为一个线程，在进入该方法时获得内部锁，只有在所有任务都执行完后，才会释放锁<br />一个较为优化的解决方案是，**只在必要时进行同步，这样就能明显减少线程持有锁的时间，提高系统的吞吐量**。
```java
public void syncMethod(){
    othercode1(); 
    synchronized(this){
        mutextMethod(); 
    }
    othercode2();
}
```
在改进的代码中只针对 `mutextMethod()` 方法做了同步，锁占用的时间相对较短，因此能有更高的并行度。<br />这种技术手段在 JDK 的源码包中也可以很容易地找到，比如处理正则表达式的 `Pattern` 类。
```java
/**
 * Creates a matcher that will match the given input against this pattern.
 *
 * @param  input
 *         The character sequence to be matched
 *
 * @return  A new matcher for this pattern
 */
public Matcher matcher(CharSequence input) {
    if (!compiled) {
        synchronized(this) {
            if (!compiled)
                compile();
        }
    }
    Matcher m = new Matcher(this, input);
    return m;
}
```
Watcher 方法有条件地进行锁申请，只有在表达式未编译时，进行局部的加锁。<br />这种处理方式大大提高了 Watcher 方法的执行效率和可靠性。
> 注意：减少锁的持有时间有助于降低锁冲突的可能性，进而提升系统的并发能力。

<a name="TdFhy"></a>
## 2、减小锁粒度
减小锁粒度也是一种削弱多线程锁竟争的有效手段。<br />对于 HashMap 来说，最重要的两个方法就是 `get()` 和 `put()`。<br />一种最自然的想法就是，对整个 HashMap 加锁从而得到一个线程安全的对象，但是这样做，加锁粒度太大。<br />对于 `ConcurrentHashMap` 类（JDK7），它内部进一步细分了若干个小的 HashMap ，称之为段( SEGMENT )。<br />在默认情况下，一个 ConcurrentHashMap 类可以被细分为 16 个段。<br />如果需要在 ConcurrentHashMap 类中增加一个新的表项，并不是将整个 HashMap 加锁，而是首先根据 hashcode 得到该表项应该被存放到哪个段中，然后对该段加锁，并完成 `put()` 方法操作。<br />在多线程环境中，如果多个线程同时进行 `put()` 方法操作，只要被加入的表项不存放在同一个段中，线程间便可以做到真正的并行。<br />由于默认有 16 个段，因此，如果够幸运的话， ConcurrentHashMap 类可以接受 16 个线程同时插入(如果都插入不同的段中)，从而大大提升其春吐量。<br />下面代码显示了 `put()` 方法操作的过程。<br />根据 key 获得对应段的序号。接着得到段，然后将数据插入给定的段中。
```java
public V put(K key, V value) {
    Segment<K,V> s;
    if (value == null)
        throw new NullPointerException();
    //计算key的hash值
    int hash = hash(key);
    //初始化的时候，segmentShift是28，segmentMask是15
    //这里是取hash的高四位，然后再跟15做一个与操作
    int j = (hash >>> segmentShift) & segmentMask;
    //初始化的时候只初始化了segment[0]，其他还是null值。所以使用ensureSegment初始化segment[j]
    if ((s = (Segment<K,V>)UNSAFE.getObject          // nonvolatile; recheck
              (segments, (j << SSHIFT) + SBASE)) == null) //  in ensureSegment
        s = ensureSegment(j);
    return s.put(key, hash, value, false);
}
```
但是，减小锁粒度会带来一个新的问题，即**当系统需要取得全局锁时，其消耗的资源会比较多。**<br />仍然以 ConcurrentHashMap 类为例，虽然其 `put()` 方法很好地分离了锁，但是当试图访问 ConcurrentHashMap 类的全局信息时，就需要同时取得所有段的锁方能顺利实施。<br />比如 ConcurrentHashMap 类的 size 方法，它将返回 ConcurrentHashMap 类的有效表项的数量，即 ConcurrentHashMap 类的全部有效表项之和。<br />要获取这个信息需要取得所有子段的锁，因此，其 `size()` 方法的部分代码如下:
```java
/**
 * Returns the number of key-value mappings in this map.  If the
 * map contains more than <tt>Integer.MAX_VALUE</tt> elements, returns
 * <tt>Integer.MAX_VALUE</tt>.
 *
 * @return the number of key-value mappings in this map
 */
public int size() {
    // Try a few times to get accurate count. On failure due to
    // continuous async changes in table, resort to locking.
    final Segment<K,V>[] segments = this.segments; //map数据从segments中拿取
    int size; //统计的大小
    boolean overflow; // true if size overflows 32 bits //size过大的溢出情况
    long sum;         // sum of modCounts 统计modCounts的值
    long last = 0L;   // previous sum 最近的一个sum值
    int retries = -1; // first iteration isn't retry 重试的次数
    try {
        for (;;) { //一直循环统计size直至segment结构没有发生变化
            if (retries++ == RETRIES_BEFORE_LOCK) {  //如果已经重试2次，到达第三次
                for (int j = 0; j < segments.length; ++j)
                    ensureSegment(j).lock(); // force creation //把segment加上锁
            }
            sum = 0L;
            size = 0;
            overflow = false;
            for (int j = 0; j < segments.length; ++j) {
                Segment<K,V> seg = segmentAt(segments, j);
                if (seg != null) {
                    sum += seg.modCount;
                    int c = seg.count;
                    if (c < 0 || (size += c) < 0)
                        overflow = true;
                }
            }
            if (sum == last)
                break;
            last = sum;
        }
    } finally {
        if (retries > RETRIES_BEFORE_LOCK) { //有重试3次以上的情况
            for (int j = 0; j < segments.length; ++j)
                segmentAt(segments, j).unlock();  //把segment解锁
        }
    }
    return overflow ? Integer.MAX_VALUE : size;
}
```
可以看到在计算总数时，先要获得所有段的锁再求和。<br />但是， ConcurrentHashMap 类的 size 方法并不总是这样执行的，事实上， size 方法会先使用无锁的方式求和，如果失败才会尝试这种加锁的方法。<br />但不管怎么说，在高并发场合 ConcurrentHashMap 类的 size 方法的性能依然要差于同步的 HashMap 因此，只有在类似于`size()` 方法获取全局信息的方法调用并不频繁时，这种减小锁粒度的方法才能在真正意义上提高系统的吞吐量。
> 注意：所谓减小锁粒度，就是指缩小锁定对象的范国，从而降低锁冲突的可能性进而提高系统的并发能力。

<a name="gAimU"></a>
## 3、 用读写分离锁来替换独占锁
使用读写分离锁 ReadWriteLock 可以提高系统的性能。<br />使用读写分离锁来替代独占锁是减小锁粒度的一种特殊情况。<br />如果说减小锁粒度是通过分割数据结构实现的，那么读写分离锁则是对系统功能点的分割在读多写少的场合，读写锁对系统性能是很有好处的。<br />因为如果系统在读写数据时均只使用独占锁，那么读操作和写操作间、读操作和读操作间、写操作和写操作间均不能做到真正的并发，并且需要相互等待。<br />而读操作本身不会影响数据的完整性和一致性。<br />因此，从理论上讲，在大部分情况下，可以允许多线程同时读，读写锁正是实现了这种功能。<br />注意:在读多写少的场合使用读写锁可以有效提升系统的并发能力。
<a name="LP80X"></a>
## 4、锁分离
如果将读写锁的思想进一步延伸，就是锁分离。<br />读写锁根据读写操作功能上的不同，进行了有效的锁分离。<br />依据应用程序的功能特点，使用类似的分离思想，也可以对独占锁进行分离。<br />一个典型的案例就是 `java.util.concurrent.LinkedBlockingQueue` 的实现。<br />在 LinkedBlockingQueue 的实现中， `take()` 函数和 `put()` 函数分别实现了从队列中取得数据和往队列中增加数据的功能。<br />虽然两个函数都对当前队列进行了修改操作，但由于 LinkedBlockingQueue 是基于链表的，因此两个操作分别作用于队列的前端和尾端，从理论上说，两者并不冲突。<br />如果使用独占锁，则要求在两个操作进行时获取当前队列的独占锁，那么 `take()` 方法和 `put()` 方法就不可能真正的并发，在运行时，它们会彼此等待对方释放锁资源。<br />在这种情况下，锁竞争会相对比较激烈，从而影响程序在高并发时的性能。<br />因此，在 JDK 的实现中，并没有采用这样的方式，取而代之的是用两把不同的锁分离了 `take()` 方法和 `put()` 方法的操作。
```java
/** Lock held by take, poll, etc */
private final ReentrantLock takeLock = new ReentrantLock();

/** Wait queue for waiting takes */
private final Condition notEmpty = takeLock.newCondition();

/** Lock held by put, offer, etc */
private final ReentrantLock putLock = new ReentrantLock();

/** Wait queue for waiting puts */
private final Condition notFull = putLock.newCondition();
```
以上代码片段定义了 `takeLock` 和 `putLock` ，它们分别在 `take()` 方法和 `put()` 方法中使用。<br />因此， `take()` 方法和 `put()` 方法就此相互独立，它们之间不存在锁竞争关系，只需要在 `take()` 方法和 take() 方法间、 `put()` 方法和 `put()` 方法间分别对 takeLock 和 putLock 进行竞争。<br />从而，削弱了锁竞争的可能性。<br />`put()` 和 `take() `方法的源码解析如下:
```java
/**
 * 在队尾插一个元素
 * 如果队列满了，一直阻塞，直到队列不满了或者线程被中断
 */
public void put(E e) throws InterruptedException {
    if (e == null) throw new NullPointerException();
    int c = -1;
    final ReentrantLock putLock = this.putLock;//入队锁
    final AtomicInteger count = this.count;//当前队列中的元素个数
    putLock.lockInterruptibly();//加锁
    try {
        while (count.get() == capacity) {//如果队列满了 
            /*
             * 加入notFull等待队列，直到队列元素不满了，
             * 被其他线程使用notFull.signal()唤醒
             */
            notFull.await();
        }
        enqueue(e);//入队
        c = count.getAndIncrement();//入队数量+1
        if (c + 1 < capacity)
            notFull.signal();
    } finally {
        putLock.unlock();
    }
    if (c == 0)
        signalNotEmpty();
}
```
```java
/**
 * 出队：
 * 如果队列空了，一直阻塞，直到队列不为空或者线程被中断
 */
public E take() throws InterruptedException {
    E x;
    int c = -1;
    final AtomicInteger count = this.count;//获取队列中的元素总量
    final ReentrantLock takeLock = this.takeLock;
    takeLock.lockInterruptibly();//获取出队锁
    try {
        while (count.get() == 0) {//如果没有元素，一直阻塞
            /*
             * 加入等待队列， 一直等待条件notEmpty（即被其他线程唤醒）
             * （唤醒其实就是，有线程将一个元素入队了，然后调用notEmpty.signal()唤醒其他等待这个条件的线程，同时队列也不空了）
             */
            notEmpty.await();
        }
        x = dequeue();//出队
        c = count.getAndDecrement();//元素数量-1
        if (c > 1)
            notEmpty.signal();
    } finally {
        takeLock.unlock();
    }
    if (c == capacity)
        signalNotFull();
    return x;
}
```
<a name="xRsMI"></a>
## 5、锁粗化
通常情况下，为了保证多线程间的有效并发，会要求每个线程持有锁的时间尽量短，即在使用完公共资源后，应该立即释放锁。<br />只有这样，等待在这个锁上的其他线程才能尽早地获得资源执行任务。<br />但是，凡事都有一个度，如果对同一个锁不停地进行请求、同步和释放，其本身也会消耗系统宝贵的资源，反而不利于性能的优化。<br />为此，虚拟机在遇到一连串连续地对同一个锁不断进行请求和释放的操作时，便会把所有的锁操作整合成对锁的一次请求，从而减少对锁的请求同步的次数，这个操作叫作锁的粗化。
```java
public void demoMethod(){
    synchronized(lock){
        // 做一件事情
    }
    // 做其他不需要的同步的工作，但能很快执行完毕
    synchronized (lock){
        // 做另一件事情
    }
}
```
上面的代码段会被整合成如下形式
```java
public void demoMethod(){
    synchronized(lock){
        // 做一件事情
        // 做其他不需要的同步的工作，但能很快执行完毕
        // 做另一件事情
    }
}
```
在开发过程中，大家也应该有意识地在合理的场合进行锁的粗化，尤其当在循环内请求锁时。<br />以下是一个循环内请求锁的例子，在这种情况下，意味着每次循环都有申请锁和释放锁的操作。<br />但在这种情况下，显然是没有必要的。
```java
for (int i=0; i<CIRCLE; i++){
    synchronized(lock){
    }
}
```
所以，一种更加合理的做法应该是在外层只请求一次锁:
```java
synchronized(lock){
    for (int i=0; i<CIRCLE; i++){
    }
}
```
注意:性能优化就是根据运行时的真实情况对各个资源点进行权衡折中的过程。锁粗化的思想和减少锁持有时间是相反的，但在不同的场合，它们的效果并不相同，因此要根据实际情况进行权衡。
<a name="nTf0K"></a>
## 6、避免热点域
锁分离和锁分段都能提高可伸缩性，因为它们都能使不同的线程在不同的数据(或者同一个数据的不同部分)上操作，而不会相互干扰。<br />如果程序采用锁分段技术，那么一定要表现出在锁上的竞争频率高于在锁保护的数据上发生竞争的频率。<br />如果一个锁保护两个独立变量 X 和 Y ，并且线程 A 想要访问 X ，而线程 B 想要访问 Y (这类似于在 Serverstatus 中，一个线程调用 adduser ，而另一个线程调用 addquery )，那么这两个线程不会在任何数据上发生竞争，即使它们会在同一个锁上发生竞争。<br />当每个操作都请求多个变量时，锁的粒度将很难降低。<br />这是在性能与可伸缩性之间相互制衡的另一个方面，一些常见的优化措施，例如将一些反复计算的结果缓存起来，都会引人一些“热点域( HotField )”，而这些热点域往往会限制可伸缩性当实现 HashMap 时，你需要考虑如何在 size 方法中计算 Map 中的元素数量。<br />最简单的方法就是，在每次调用时都统计一次元素的数量。<br />一种常见的优化措施是，在插入和移除元素时更新一个计数器，虽然这在 put 和 remove 等方法中略微増加了一些开销，以确保计数器是最新的值，但这将把 size 方法的开销从 O(N) 降低到 O(1)。<br />在单线程或者采用完全同步的实现中，使用一个独立的计数能很好地提高类似 size 和 isEmpty 这些方法的执行速度，但却导致更难以提升实现的可伸缩性，因为每个修改 Map 的操作都需要更新这个共享的计数器。<br />即使使用锁分段技术来实现散列链，那么在对计数器的访问进行同步时，也会重新导致在使用独占锁时存在的可伸缩性问题。<br />一个看似性能优化的措施——缓存 size 操作的结果，已经变成了一个可伸缩性问题。<br />在这种情况下，计数器也被称为热点域，因为每个导致元素数量发生变化的操作都需要访问它。<br />从上面 ConcurrentHashMap 的 `size()` 的源码解析也可以看出， ConcurrentHashMap 中的 size 将对每个分段进行枚举并将每个分段中的元素数量相加，而不是维护一个全局计数。<br />为了避免枚举每个元素， ConcurrentHashMap 为每个分段都维护了一个独立的计数，并通过每个分段的锁来维护这个值。
