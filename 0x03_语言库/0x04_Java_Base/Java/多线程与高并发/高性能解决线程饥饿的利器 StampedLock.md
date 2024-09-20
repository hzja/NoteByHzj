线程饥饿 JDK8 StampedLock
<a name="Ko2vk"></a>
## 概览
在 JDK 1.8 引入 `StampedLock`，可以理解为对 `ReentrantReadWriteLock` 在某些方面的增强，在原先读写锁的基础上新增了一种叫乐观读（Optimistic Reading）的模式。该模式并不会加锁，所以不会阻塞线程，会有更高的吞吐量和更高的性能。<br />带着问题来看`StampedLock`带来了什么…

- 有了`ReentrantReadWriteLock`，为何还要引入`StampedLock`？
- 什么是乐观读？
- 在读多写少的并发场景下，`StampedLock`如何解决写线程难以获取锁的线程“饥饿”问题？
- 什么样的场景使用？
- 实现原理分析，是通过 AQS 实现还是其他的？
<a name="LptoN"></a>
## 特性
它的设计初衷是作为一个内部工具类，用于开发其他线程安全的组件，提升系统性能，并且编程模型也比`ReentrantReadWriteLock` 复杂，所以用不好就很容易出现死锁或者线程安全等莫名其妙的问题。
<a name="mjShz"></a>
### 三种访问数据模式

- Writing（独占写锁）：`writeLock` 方法会使线程阻塞等待独占访问，可类比`ReentrantReadWriteLock` 的写锁模式，同一时刻有且只有一个写线程获取锁资源；
- Reading（悲观读锁）：`readLock`方法，允许多个线程同时获取悲观读锁，悲观读锁与独占写锁互斥，与乐观读共享。
- Optimistic Reading（乐观读）：这里需要注意了，是乐观读，并没有加锁。也就是不会有 CAS 机制并且没有阻塞线程。仅当当前未处于 Writing 模式 `tryOptimisticRead` 才会返回非 0 的邮戳（Stamp），如果在获取乐观读之后没有出现写模式线程获取锁，则在方法`validate`返回 true ，**允许多个线程获取乐观读以及读锁。同时允许一个写线程获取写锁**。
<a name="19dffe23"></a>
### 支持读写锁相互转换
`ReentrantReadWriteLock` 当线程获取写锁后可以降级成读锁，但是反过来则不行。<br />`StampedLock`提供了读锁和写锁相互转换的功能，使得该类支持更多的应用场景。
<a name="1INof"></a>
### 注意事项

1. `StampedLock`是不可重入锁，如果当前线程已经获取了写锁，再次重复获取的话就会死锁；
2. 都不支持 `Conditon` 条件将线程等待；
3. `StampedLock` 的写锁和悲观读锁加锁成功之后，都会返回一个 stamp；然后解锁的时候，需要传入这个 stamp。
<a name="0IszJ"></a>
### 详解乐观读带来的性能提升
那为何 `StampedLock` 性能比 `ReentrantReadWriteLock` 好？<br />关键在于`StampedLock` 提供的乐观读， `ReentrantReadWriteLock` 支持多个线程同时获取读锁，但是当多个线程同时读的时候，所有的写线程都是阻塞的。<br />`StampedLock` **的乐观读允许一个写线程获取写锁，所以不会导致所有写线程阻塞，也就是当读多写少的时候，写线程有机会获取写锁，减少了线程饥饿的问题，吞吐量大大提高。**<br />这里可能就会有疑问，竟然同时允许多个乐观读和一个先线程同时进入临界资源操作，那读取的数据可能是错的怎么办？<br />**是的，乐观读不能保证读取到的数据是最新的，所以将数据读取到局部变量的时候需要通过 `lock.validate(stamp)` 校验是否被写线程修改过，若是修改过则需要上悲观读锁，再重新读取数据到局部变量。**<br />同时由于乐观读并不是锁，所以没有线程唤醒与阻塞导致的上下文切换，性能更好。<br />其实跟数据库的“乐观锁”有异曲同工之妙，它的实现思想很简单。举个数据库的例子。<br />在生产订单的表 product_doc 里增加了一个数值型版本号字段 version，每次更新 product_doc 这个表的时候，都将 version 字段加 1。
```sql
select id，... ，version
from product_doc
where id = 123
```
在更新的时候匹配 version 才执行更新。
```sql
update product_doc
set version = version + 1,...
where id = 123 and version = 5
```
数据库的乐观锁就是查询的时候将 version 查出来，更新的时候利用 version 字段验证，若是相等说明数据没有被修改，读取的数据是安全的。<br />这里的 version 就类似于 `StampedLock` 的 Stamp。
<a name="18fBI"></a>
## 使用示例
模仿写一个将用户 id 与用户名数据保存在 共享变量 idMap 中，并且提供 put 方法添加数据、get 方法获取数据、以及 putIfNotExist 先从 map 中获取数据，若没有则模拟从数据库查询数据并放到 map 中。
```java
public class CacheStampedLock {
    /**
     * 共享变量数据
     */
    private final Map<Integer, String> idMap = new HashMap<>();
    private final StampedLock lock = new StampedLock();
    /**
     * 添加数据，独占模式
     */
    public void put(Integer key, String value) {
        long stamp = lock.writeLock();
        try {
            idMap.put(key, value);
        } finally {
            lock.unlockWrite(stamp);
        }
    }
    /**
     * 读取数据，只读方法
     */
    public String get(Integer key) {
        // 1. 尝试通过乐观读模式读取数据，非阻塞
        long stamp = lock.tryOptimisticRead();
        // 2. 读取数据到当前线程栈
        String currentValue = idMap.get(key);
        // 3. 校验是否被其他线程修改过,true 表示未修改，否则需要加悲观读锁
        if (!lock.validate(stamp)) {
            // 4. 上悲观读锁，并重新读取数据到当前线程局部变量
            stamp = lock.readLock();
            try {
                currentValue = idMap.get(key);
            } finally {
                lock.unlockRead(stamp);
            }
        }
        // 5. 若校验通过，则直接返回数据
        return currentValue;
    }
    /**
     * 如果数据不存在则从数据库读取添加到 map 中,锁升级运用
     * @param key
     * @param value 可以理解成从数据库读取的数据，假设不会为 null
     * @return
     */
    public String putIfNotExist(Integer key, String value) {
        // 获取读锁，也可以直接调用 get 方法使用乐观读
        long stamp = lock.readLock();
        String currentValue = idMap.get(key);
        // 缓存为空则尝试上写锁从数据库读取数据并写入缓存
        try {
            while (Objects.isNull(currentValue)) {
                // 尝试升级写锁
                long wl = lock.tryConvertToWriteLock(stamp);
                // 不为 0 升级写锁成功
                if (wl != 0L) {
                    // 模拟从数据库读取数据, 写入缓存中
                    stamp = wl;
                    currentValue = value;
                    idMap.put(key, currentValue);
                    break;
                } else {
                    // 升级失败，释放之前加的读锁并上写锁，通过循环再试
                    lock.unlockRead(stamp);
                    stamp = lock.writeLock();
                }
            }
        } finally {
            // 释放最后加的锁
            lock.unlock(stamp);
        }
        return currentValue;
    }
}
```
上面的使用例子中，需要引起注意的是 `get()`和 `putIfNotExist()` 方法，第一个使用了乐观读，使得读写可以并发执行，第二个则是使用了读锁转换成写锁的编程模型，先查询缓存，当不存在的时候从数据库读取数据并添加到缓存中。<br />在使用乐观读的时候一定要按照固定模板编写，否则很容易出 bug，总结一下乐观读编程模型的模板：
```java
public void optimisticRead() {
    // 1. 非阻塞乐观读模式获取版本信息
    long stamp = lock.tryOptimisticRead();
    // 2. 拷贝共享数据到线程本地栈中
    copyVaraibale2ThreadMemory();
    // 3. 校验乐观读模式读取的数据是否被修改过
    if (!lock.validate(stamp)) {
        // 3.1 校验未通过，上读锁
        stamp = lock.readLock();
        try {
            // 3.2 拷贝共享变量数据到局部变量
            copyVaraibale2ThreadMemory();
        } finally {
            // 释放读锁
            lock.unlockRead(stamp);
        }
    }
    // 3.3 校验通过，使用线程本地栈的数据进行逻辑操作
    useThreadMemoryVarables();
}
```
<a name="u7hGV"></a>
## 使用场景和注意事项
对于读多写少的高并发场景 `StampedLock`的性能很好，通过乐观读模式很好的解决了写线程“饥饿”的问题，可以使用`StampedLock` 来代替`ReentrantReadWriteLock` ，但是需要注意的是 **StampedLock 的功能仅仅是 ReadWriteLock 的子集**，在使用的时候，还是有几个地方需要注意一下。

1. `StampedLock`是不可重入锁，使用过程中一定要注意；
2. 悲观读、写锁都不支持条件变量 `Conditon` ，当需要这个特性的时候需要注意；
3. 如果线程阻塞在 StampedLock 的 readLock() 或者 writeLock() 上时，此时调用该阻塞线程的 interrupt() 方法，会导致 CPU 飙升。所以，**使用 StampedLock 一定不要调用中断操作，如果需要支持中断功能，一定使用可中断的悲观读锁 readLockInterruptibly() 和写锁 writeLockInterruptibly()**。这个规则一定要记清楚。
<a name="BbbVN"></a>
## 原理分析
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603155903347-ca62b91c-c639-4a8d-8881-8c5268d52dd4.webp#align=left&display=inline&height=486&originHeight=486&originWidth=697&size=0&status=done&style=shadow&width=697)StapedLock局部变量<br />发现它并不像其他锁一样通过定义内部类继承 `AbstractQueuedSynchronizer`抽象类然后子类实现模板方法实现同步逻辑。但是实现思路还是有类似，依然使用了 CLH 队列来管理线程，通过同步状态值 state 来标识锁的状态。<br />其内部定义了很多变量，这些变量的目的还是跟 `ReentrantReadWriteLock` 一样，将状态为按位切分，通过位运算对 state 变量操作用来区分同步状态。<br />比如写锁使用的是第八位为 1 则表示写锁，读锁使用 0-7 位，所以一般情况下获取读锁的线程数量为 1-126，超过以后，会使用 readerOverflow int 变量保存超出的线程数。
<a name="Ht60m"></a>
### 自旋优化
对多核 CPU 也进行一定优化，NCPU 获取核数，当核数目超过 1 的时候，线程获取锁的重试、入队钱的重试都有自旋操作。主要就是通过内部定义的一些变量来判断，如图所示。
<a name="gJD2B"></a>
### 等待队列
队列的节点通过 WNode 定义，如上图所示。等待队列的节点相比 AQS 更简单，只有三种状态分别是：

- 0：初始状态；
- -1：等待中；
- 取消；

另外还有一个字段 cowait ，通过该字段指向一个栈，保存读线程。结构如图所示<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1603155903486-9ccab7f7-656f-4427-a307-bf430b80ac56.png#align=left&display=inline&height=304&originHeight=304&originWidth=643&size=0&status=done&style=none&width=643)<br />WNode<br />同时定义了两个变量分别指向头结点与尾节点。
```java
/** Head of CLH queue */
private transient volatile WNode whead;
/** Tail (last) of CLH queue */
private transient volatile WNode wtail;
```
另外有一个需要注意点就是 cowait， 保存所有的读节点数据，使用的是头插法。<br />当读写线程竞争形成等待队列的数据如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603155903428-61bf724d-7b9e-4b95-995b-2993adbf37ea.webp#align=left&display=inline&height=347&originHeight=347&originWidth=685&size=0&status=done&style=shadow&width=685)<br />队列
<a name="5BhG8"></a>
### 获取写锁
```java
public long writeLock() {
    long s, next;  // bypass acquireWrite in fully unlocked case only
    return ((((s = state) & ABITS) == 0L &&
             U.compareAndSwapLong(this, STATE, s, next = s + WBIT)) ?
            next : acquireWrite(false, 0L));
}
```
获取写锁，如果获取失败则构建节点放入队列，同时阻塞线程，需要注意的时候该方法不响应中断，如需中断需要调用 `writeLockInterruptibly()`。否则会造成高 CPU 占用的问题。<br />`(s = state) & ABITS` 标识读锁和写锁未被使用，那么直接执行 `U.compareAndSwapLong(this, STATE, s, next = s + WBIT))` CAS 操作将第八位设置 1，标识写锁占用成功。CAS 失败的话则调用 `acquireWrite(false, 0L)`加入等待队列，同时将线程阻塞。<br />另外`acquireWrite(false, 0L)` 方法很复杂，运用大量自旋操作，比如自旋入队列。
<a name="97s7T"></a>
### 获取读锁
```java
public long readLock() {
    long s = state, next;  // bypass acquireRead on common uncontended case
    return ((whead == wtail && (s & ABITS) < RFULL &&
             U.compareAndSwapLong(this, STATE, s, next = s + RUNIT)) ?
            next : acquireRead(false, 0L));
}
```
<a name="ulhDR"></a>
### 获取读锁关键步骤
`(whead == wtail && (s & ABITS) < RFULL`如果队列为空并且读锁线程数未超过限制，则通过 `U.compareAndSwapLong(this, STATE, s, next = s + RUNIT))`CAS 方式修改 state 标识获取读锁成功。<br />否则调用 `acquireRead(false, 0L)` 尝试使用自旋获取读锁，获取不到则进入等待队列。
<a name="3TrcI"></a>
#### acquireRead
当 A 线程获取了写锁，B 线程去获取读锁的时候，调用 acquireRead 方法，则会加入阻塞队列，并阻塞 B 线程。方法内部依然很复杂，大致流程梳理后如下：

1. 如果写锁未被占用，则立即尝试获取读锁，通过 CAS 修改状态为标志成功则直接返回。
2. 如果写锁被占用，则将当前线程包装成 WNode 读节点，并插入等待队列。**如果是写线程节点则直接放入队尾，否则放入队尾专门存放读线程的 WNode cowait 指向的栈**。栈结构是头插法的方式插入数据，最终唤醒读节点，从栈顶开始。
<a name="GEMYD"></a>
### 释放锁
无论是 `unlockRead` 释放读锁还是 `unlockWrite`释放写锁，总体流程基本都是通过 CAS 操作，修改 state 成功后调用 release 方法唤醒等待队列的头结点的后继节点线程。

1. 想将头结点等待状态设置为 0 ，标识即将唤醒后继节点。
2. 唤醒后继节点通过 CAS 方式获取锁，如果是读节点则会唤醒 cowait 锁指向的栈所有读节点。
<a name="uDjNP"></a>
#### 释放读锁
`unlockRead(long stamp)` 如果传入的 stamp 与锁持有的 stamp 一致，则释放非排它锁，内部主要是通过自旋 + CAS 修改 state 成功，在修改 state 之前做了判断是否超过读线程数限制，若是小于限制才通过 CAS 修改 state 同步状态，接着调用 release 方法唤醒 whead 的后继节点。
<a name="MsSW0"></a>
#### 释放写锁
`unlockWrite(long stamp)` 如果传入的 stamp 与锁持有的 stamp 一致，则释放写锁，whead 不为空，且当前节点状态 status ！= 0 则调用 release 方法唤醒头结点的后继节点线程。
<a name="jBU6G"></a>
## 总结
StampedLock 并不能完全代替`ReentrantReadWriteLock` ，在读多写少的场景下因为乐观读的模式，允许一个写线程获取写锁，解决了写线程饥饿问题，大大提高吞吐量。<br />在使用乐观读的时候需要注意按照编程模型模板方式去编写，否则很容易造成死锁或者意想不到的线程安全问题。<br />它不是可重入锁，且不支持条件变量 `Conditon`。并且线程阻塞在 readLock() 或者 writeLock() 上时，此时调用该阻塞线程的 interrupt() 方法，会导致 CPU 飙升。如果需要中断线程的场景，一定要注意调用**悲观读锁 readLockInterruptibly() 和写锁 writeLockInterruptibly()**。<br />另外唤醒线程的规则和 AQS 类似，先唤醒头结点，不同的是 `StampedLock` 唤醒的节点是读节点的时候，会唤醒此读节点的 cowait 锁指向的栈的所有读节点，但是唤醒与插入的顺序相反。
