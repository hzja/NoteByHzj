Java 读写锁
<a name="f6UCm"></a>
## 1、StampedLock特性
StampedLock是JDK 8新增的读写锁，跟读写锁不同，它并不是由AQS实现的。它的state为一个long型变量，状态的设计也不同于读写锁，且提供了三种模式来控制 read/write 的获取，并且内部实现了自己的同步等待队列。
<a name="jt6li"></a>
### 1.1、StampedLock读写锁
**写锁：**使用writeLock方法获取，当锁不可用时会阻塞，获取成功后返回一个与这个写锁对应的stamp，在unlockWrite方法中，需要通过这个stamp来释放与之对应的锁。在tryWriteLock同样也会提供这个stamp。当在write模式中获取到写锁时，读锁不能被获取，并且所有的乐观读锁验证(validate方法)都会失败。<br />**读锁：**使用readLock方法获取，当超出可用资源时（类似AQS的state设计）会阻塞。同样的，在获取锁成功后也会返回stamp，作用与上述相同。tryReadLock同样如此。<br />**乐观读锁：**使用tryOptimisticRead方法获取，只有在写锁可用时才能成功获取乐观读锁，获取成功后也会返回一个stamp。validate方法可以根据这个stamp来判断写锁是否被获取。这种模式可以理解为一个弱化的读锁(weak version of a read-lock)，它在任何时候都能被破坏。乐观读模式常被用在短的只读的代码段，用来减少争用并提高吞吐量。乐观读区域应该只读取字段，并将它们保存在本地变量中，以便在验证（validate方法）后使用。在乐观读模式中字段的读取可能会不一致，所以可能需要反复调用validate()来检查一致性。例如，当首次读取一个对象或数组引用，然后访问其中一个的字段、元素或方法时，这些步骤通常是必需的。
<a name="jbgHB"></a>
### 1.2、三种模式的转换
StampedLock可以将三种模式是锁进行有条件的互相转换。<br />**将其他锁转换为写锁**`**tryConvertToWriteLock()**`**：**<br />当前邮戳为持有写锁模式，直接返回当前的邮戳；<br />当前邮戳为持有读锁模式，则会释放读锁并获取写锁，并返回写锁邮戳；<br />当前邮戳持有乐观锁，通过CAS立即获取写锁，成功则返回写锁邮戳；失败则返回0；<br />**将其他锁转换为读锁**`**tryConvertToReadLock**`**：**<br />当前邮戳为持有写锁模式，则会释放写锁并获取读锁，并返回读锁邮戳；<br />当前邮戳为持有读锁模式，则直接返回当前读锁邮戳；<br />当前邮戳持有乐观锁，通过CAS立即获取读锁，则返回读锁邮戳；否则，获取失败返回0；<br />**将其他锁转换为乐观锁**`**tryConvertToOptimisticRead**`**：**<br />当前邮戳为持有读或写锁，则直接释放读写锁，并返回释放后的观察者邮戳值；<br />当前邮戳持有乐观锁，若乐观锁邮戳有效，则返回观察者邮戳；
<a name="Ebcxs"></a>
### 1.3、StampedLock的应用场景
StampedLock一般作为线程安全的内部工具类。它的使用依赖于对数据、对象和方法的内部属性有一定的了解。StampedLock 是不可重入的，所以在锁的内部不能调用其他尝试重复获取锁的方。一个stamp如果在很长时间都没有使用或验证，在很长一段时间之后可能就会验证失败。StampedLocks是可序列化的，但是反序列化后变为初始的非锁定状态，所以在远程锁定中是不安全的。
<a name="SkUXK"></a>
### 1.4、StampedLock的公平性
StampedLock 的调度策略不会始终偏向读线程或写线程，所有的"try"方法都是尽最大努力获取，并不一定遵循任何调度或公平策略。从"try"方法获取或转换锁失败返回0时，不会携带任何锁的状态信息。由于`StampedLock`支持跨多个锁模式的协调使用，它不会直接实现Lock或`ReadWriteLock`接口。但是，如果应用程序需要`Lock`的相关功能，它可以通过`asReadLock()`、`asWriteLock()`和`asReadWriteLock()`方法返回一个`Lock`视图。
<a name="RhAeD"></a>
## 2、源码分析
<a name="IKdhC"></a>
### 2.1、主要属性
```java
//获取CPU的可用线程数量，用于确定自旋的时候循环次数
private static final int NCPU = Runtime.getRuntime().availableProcessors();

//根据NCPU确定自旋的次数限制(并不是一定这么多次，因为实际代码中是随机的)
private static final int SPINS = (NCPU > 1) ? 1 << 6 : 0;

//头节点上的自旋次数
private static final int HEAD_SPINS = (NCPU > 1) ? 1 << 10 : 0;

//头节点上的最大自旋次数
private static final int MAX_HEAD_SPINS = (NCPU > 1) ? 1 << 16 : 0;

//等待自旋锁溢出的周期数
private static final int OVERFLOW_YIELD_RATE = 7; // must be power 2 - 1

//在溢出之前读线程计数用到的bit位数
private static final int LG_READERS = 7;

//一个读状态单位：0000 0000 0001
private static final long RUNIT = 1L;
//一个写状态单位：0000 1000 0000
private static final long WBIT  = 1L << LG_READERS;
//读状态标识：0000 0111 1111
private static final long RBITS = WBIT - 1L;
//读锁计数的最大值：0000 0111 1110
private static final long RFULL = RBITS - 1L;
//读线程个数和写线程个数的掩码：0000 1111 1111
private static final long ABITS = RBITS | WBIT;
//// 读线程个数的反数，高25位全部为1:1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0000
private static final long SBITS = ~RBITS; // note overlap with ABITS

// state的初始值
private static final long ORIGIN = WBIT << 1;

// 中断标识
private static final long INTERRUPTED = 1L;

// 节点状态值，等待/取消
private static final int WAITING   = -1;
private static final int CANCELLED =  1;

// 节点模式，读模式/写模式
private static final int RMODE = 0;
private static final int WMODE = 1;

//等待队列的头节点
private transient volatile WNode whead;
//等待队列的尾节点
private transient volatile WNode wtail;

// 锁状态
private transient volatile long state;
////因为读状态只有7位很小，所以当超过了128之后将使用此变量来记录
private transient int readerOverflow;
```
<a name="Yvk78"></a>
### 2.2、node节点实现
```java
//等待队列的节点实现
static final class WNode {
    //前驱节点
    volatile WNode prev;
    //后继节点
    volatile WNode next;
    //读线程使用的链表
    volatile WNode cowait;    // list of linked readers
    //等待的线程
    volatile Thread thread;   // non-null while possibly parked
    //节点状态
    volatile int status;      // 0, WAITING, or CANCELLED
    //节点模式
    final int mode;           // RMODE or WMODE
    WNode(int m, WNode p) { mode = m; prev = p; }
}
```
<a name="wLbRm"></a>
### 2.3、state状态实现
**state状态说明：**

- bit0—bit6为作为读锁计数，当超出RFULL（126）时，用readerOverflow作为读锁计数。当获取到读锁时，state加RUINT(值为1)；当释放读锁时，state减去RUINT。
- bit7为写锁标识，其值为1表示已获取写锁，值为0表示已获取读锁。当线程获取写锁或释放写锁时，都会将state加WBIT；
- bit8—bit64：表示写锁版本，不论获取写锁还是释放写锁，其值都会改变。<br />初始状态bit8为1，其他位为0。

**state常用状态标识：**

- **RUNIT：**<br />0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0001
- **WBIT：**<br />0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1000 0000
- **RBITS：**<br />0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0111 1111
- **RFULL：**<br />0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0111 1110
- **ABITS：**<br />0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111
- **SBITS：**<br />1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0000
- **ORIGIN：**<br />0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0001 0000 0000

**state常用状态判断：**

- 有无线程获取写状态：state < WBIT，true：无，false：有；
- 读状态是否溢出：(state & ABITS) < RFULL，true；否；false：是；
- 获取读状态： state + RUNIT(或者readerOverflow + 1)
- 获取写状态: state + WBIT
- 释放读状态： state - RUNIT(或者readerOverflow - 1)
- 释放写状态： (s += WBIT) == 0L ? ORIGIN : s
- 是否为写锁： (state & WBIT) != 0L
- 是否为读锁： (state & RBITS) != 0L
<a name="EVKoP"></a>
### 2.4、写锁获取及释放
**获取写锁：**
```java
public long writeLock() {
    long s, next;  
    //（state & ABITS）获取低8位，判断有无读/写锁存在，
    //有其他读锁则bit0-bit6不为0，有其他写锁则bit7不为0，
    //因此如果有别的写锁或者读锁存在将失败
    //尝试CAS获取写锁
    //失败调用acquireWrite继续获取写锁
    return ((((s = state) & ABITS) == 0L &&
             U.compareAndSwapLong(this, STATE, s, next = s + WBIT)) ?
            next : acquireWrite(false, 0L));
}

private long acquireWrite(boolean interruptible, long deadline) {
    //node为当前节点，p为当前节点的前驱节点
    WNode node = null, p;

    // 第一次自旋——主要进行入队工作
    for (int spins = -1;;) { // spin while enqueuing
        long m, s, ns;
        //（state&ABITS）为0表示无读/写锁，则尝试CAS获取写锁
        if ((m = (s = state) & ABITS) == 0L) {
            if (U.compareAndSwapLong(this, STATE, s, ns = s + WBIT))
                return ns;
        }
        else if (spins < 0)
            // 如果自旋次数小于0，则计算自旋的次数
            // 如果当前有写锁（m == WBIT）独占，且队列无元素（wtail == whead），
            // 说明当前节点的前驱节点为获取独占锁的节点，锁很快就会释放，
            // 就自旋SPINS次就行了，如果自旋完了还没轮到自己才入队
            // 否则自旋次数为0
            spins = (m == WBIT && wtail == whead) ? SPINS : 0;
        else if (spins > 0) {
            // 当自旋次数大于0时，当前这次自旋随机减一次自旋次数
            if (LockSupport.nextSecondarySeed() >= 0)
                --spins;
        }
        else if ((p = wtail) == null) { // initialize queue
            // 如果队列未初始化，新建一个空节点并初始化头节点和尾节点
            WNode hd = new WNode(WMODE, null);
            if (U.compareAndSwapObject(this, WHEAD, null, hd))
                wtail = hd;
        }
        else if (node == null)
            // 如果新增节点还未初始化，则新建之，并赋值其前置节点为尾节点
            node = new WNode(WMODE, p);
        else if (node.prev != p)
            // 如果新增节点的前驱节点不是尾节点，
            // 则更新新增节点的前驱节点为新的尾节点
            node.prev = p;
        else if (U.compareAndSwapObject(this, WTAIL, p, node)) {
            // 尝试更新新增节点为新的尾节点成功，则退出循环
            p.next = node;
            break;
        }
    }

    // 第二次自旋——主要进行阻塞并等待唤醒
    for (int spins = -1;;) {
        // h为头节点，np为新增节点的前置节点，pp为前前置节点，ps为前置节点的状态
        WNode h, np, pp; int ps;
        // 如果头节点等于前置节点，说明快轮到自己了
        if ((h = whead) == p) {
            if (spins < 0)
                // 自旋次数小于0，则初始化自旋次数
                spins = HEAD_SPINS;
            else if (spins < MAX_HEAD_SPINS)
                // 自旋次数小于头结点最大自旋次数，则增加自旋次数
                spins <<= 1;

            // 第三次自旋，不断尝试获取写锁    
            for (int k = spins;;) { // spin at head
                long s, ns;
                //无读写锁则CAS获取写锁，成功则更新节点信息
                if (((s = state) & ABITS) == 0L) {
                    if (U.compareAndSwapLong(this, STATE, s,
                                             ns = s + WBIT)) {
                        whead = node;
                        node.prev = null;
                        return ns;
                    }
                }
                // 随机立减自旋次数，当自旋次数减为0时跳出循环再重试
                else if (LockSupport.nextSecondarySeed() >= 0 &&
                         --k <= 0)
                    break;
            }
        }
        //头节点为空？
        else if (h != null) { // help release stale waiters
            WNode c; Thread w;
            // 如果头节点的cowait链表（栈）不为空，唤醒里面的所有节点
            while ((c = h.cowait) != null) {
                if (U.compareAndSwapObject(h, WCOWAIT, c, c.cowait) &&
                    (w = c.thread) != null)
                    U.unpark(w);
            }
        }

        // 如果头节点没有变化
        if (whead == h) {
            // 如果尾节点有变化，则更新
            if ((np = node.prev) != p) {
                if (np != null)
                    (p = np).next = node;   // stale
            }
            else if ((ps = p.status) == 0)
                // 如果尾节点状态为0，则更新成WAITING
                U.compareAndSwapInt(p, WSTATUS, 0, WAITING);
            else if (ps == CANCELLED) {
                // 如果尾节点状态为取消，则把它从链表中删除
                if ((pp = p.prev) != null) {
                    node.prev = pp;
                    pp.next = node;
                }
            }
            else {
                // 有超时时间的处理
                long time; // 0 argument to park means no timeout
                if (deadline == 0L)
                    time = 0L;
                //时间以过期则取消节点    
                else if ((time = deadline - System.nanoTime()) <= 0L)
                    return cancelWaiter(node, node, false);

                //设置线程blocker    
                Thread wt = Thread.currentThread();
                U.putObject(wt, PARKBLOCKER, this);
                node.thread = wt;
                //1、当前节点前驱节点状态为WAITING；
                //2、当前节点的前驱节点不是头节点或有读写锁已经被获取；
                //3、头结点为改变；
                //4、当前节点的前驱节点未改变；
                //当以上4个条件都满足时将当前节点进行阻塞
                if (p.status < 0 && (p != h || (state & ABITS) != 0L) &&
                    whead == h && node.prev == p)
                    U.park(false, time);  // emulate LockSupport.park
                node.thread = null;
                //设置线程blocker为空
                U.putObject(wt, PARKBLOCKER, null);
                //当前节点百中断则取消节点
                if (interruptible && Thread.interrupted())
                    return cancelWaiter(node, node, true);
            }
        }
    }
}
```
**写锁释放：**
```java
public void unlockWrite(long stamp) {
    WNode h;
    //因为写锁是独占锁，可以简单判断state != stamp；
    //或者bit7为0，即stamp状态为无写锁
    if (state != stamp || (stamp & WBIT) == 0L)
        throw new IllegalMonitorStateException();

    //修改state状态，state += WBIT；溢出则初始化为ORIGIN        
    state = (stamp += WBIT) == 0L ? ORIGIN : stamp;
    //头节点不为空，且状态正常则释放头结点的锁
    if ((h = whead) != null && h.status != 0)
        release(h);
}

private void release(WNode h) {
    if (h != null) {
        WNode q; Thread w;
        //设置头节点状态为0
        U.compareAndSwapInt(h, WSTATUS, WAITING, 0);

        //头节点下个节点为空或状态为CANCEL，则从尾节点前向遍历，
        //找到头结点后面的第一个有效节点（状态为0或WAITTING）
        if ((q = h.next) == null || q.status == CANCELLED) {
            for (WNode t = wtail; t != null && t != h; t = t.prev)
                if (t.status <= 0)
                    q = t;
        }
        //唤醒下一个有效节点
        if (q != null && (w = q.thread) != null)
            U.unpark(w);
    }
}
```
<a name="yn9SB"></a>
### 2.5、读锁获取及释放
**读锁获取：**
```java
public long readLock() {
    long s = state, next;  // bypass acquireRead on common uncontended case
    //同步队列为空，读锁计数值未超过最大值（无写锁），CAS获取锁成功，则直接返回邮戳值；
    //否则acquireRead获取读锁
    return ((whead == wtail && (s & ABITS) < RFULL &&
             U.compareAndSwapLong(this, STATE, s, next = s + RUNIT)) ?
            next : acquireRead(false, 0L));
}

private long acquireRead(boolean interruptible, long deadline) {
    WNode node = null, p;
    for (int spins = -1;;) {
        WNode h;
        //如果同步队列头节点等于尾节点，说明队列中没有节点或者只有一个节点
        //则自旋一段时间，等待头结点释放锁后获取锁
        if ((h = whead) == (p = wtail)) {
            for (long m, s, ns;;) {
                //（state & ABITS）小于RFULL，表示无写锁，则直接CAS获取读锁；
                //否则若（state & ABITS）小于WBIT表示无写锁但读锁计数已溢出，
                //则CAS更新读锁计数获取读锁
                if ((m = (s = state) & ABITS) < RFULL ?
                    U.compareAndSwapLong(this, STATE, s, ns = s + RUNIT) :
                    (m < WBIT && (ns = tryIncReaderOverflow(s)) != 0L))
                    return ns;
                //（state & ABITS）大于WBIT，表示写锁已经被占，则自旋    
                else if (m >= WBIT) {
                    //随机减自旋次数
                    if (spins > 0) {
                        if (LockSupport.nextSecondarySeed() >= 0)
                            --spins;
                    }
                    else {
                        //自旋完还没获取到读锁？
                        if (spins == 0) {
                            WNode nh = whead, np = wtail;
                            //判断稳定性（有没有被修改），跳出循环
                            if ((nh == h && np == p) || (h = nh) != (p = np))
                                break;
                        }
                        //初始化spins
                        spins = SPINS;
                    }
                }
            }
        }
        //自旋获取失败，则进行节点初始化相关的处理
        //尾节点为空，则初始化队列
        if (p == null) { // initialize queue
            WNode hd = new WNode(WMODE, null);
            if (U.compareAndSwapObject(this, WHEAD, null, hd))
                wtail = hd;
        }
        //初始化代表当前读线程的节点
        else if (node == null)
            node = new WNode(RMODE, p);
        //head==tail或者队列tail.mode不为读状态，
        //那么将当前线程的节点node加入到队列尾部并跳出外层循环    
        else if (h == p || p.mode != RMODE) {
            if (node.prev != p)
                node.prev = p;
            else if (U.compareAndSwapObject(this, WTAIL, p, node)) {
                p.next = node;
                break;
            }
        }
        //如果head!= tail说明队列中已经有线程在等待或者tail.mode是读状态RMODE，
        //那么CAS方式将当前线程的节点node加入到tail节点的cowait链中
        else if (!U.compareAndSwapObject(p, WCOWAIT,
                                         node.cowait = p.cowait, node))
            node.cowait = null;
        else {
            for (;;) {
                WNode pp, c; Thread w;
                ////如果head不为空那么尝试去解放head的cowait链中的节点
                if ((h = whead) != null && (c = h.cowait) != null &&
                    U.compareAndSwapObject(h, WCOWAIT, c, c.cowait) &&
                    (w = c.thread) != null) // help release
                    U.unpark(w);
                //如果tail节点的前驱就是head或者head==tail或者tail节点的前驱是null
                //也就是说当前node所在的节点（因为node可能在cowait链中）
                //的前驱就是head或者head已经被释放了为null    
                if (h == (pp = p.prev) || h == p || pp == null) {
                    long m, s, ns;
                    do {
                        //如果没有写状态被占有那么自旋方式尝试获取读状态，成功则返回stamp
                        if ((m = (s = state) & ABITS) < RFULL ?
                            U.compareAndSwapLong(this, STATE, s,
                                                 ns = s + RUNIT) :
                            (m < WBIT &&
                             (ns = tryIncReaderOverflow(s)) != 0L))
                            return ns;
                    } while (m < WBIT);
                }
                //判断是否稳定
                if (whead == h && p.prev == pp) {
                    long time;
                    //如果tail的前驱是null或者head==tail或者tail已经被取消了(p.status > 0)
                    //直接将node置为null跳出循环，回到最开的for循环中去再次尝试获取同步状态
                    if (pp == null || h == p || p.status > 0) {
                        node = null; // throw away
                        break;
                    }
                    if (deadline == 0L)
                        time = 0L;
                    //如果超时则取消当前线程    
                    else if ((time = deadline - System.nanoTime()) <= 0L)
                        return cancelWaiter(node, p, false);
                    Thread wt = Thread.currentThread();
                    U.putObject(wt, PARKBLOCKER, this);
                    node.thread = wt;

                    //tail的前驱不是head或者当前只有写线程获取到同步状态
                    //判断稳定性
                    if ((h != pp || (state & ABITS) == WBIT) &&
                        whead == h && p.prev == pp)
                        U.park(false, time);
                    node.thread = null;
                    U.putObject(wt, PARKBLOCKER, null);
                    //中断的话取消
                    if (interruptible && Thread.interrupted())
                        return cancelWaiter(node, p, true);
                }
            }
        }
    }

     //如果队列中没有节点或者tail的mode是WMODE写状态，
     //那么node被加入到队列的tail之后进入这个循环
    for (int spins = -1;;) {
        WNode h, np, pp; int ps;
        //如果p(node的前驱节点)就是head，那么自旋方式尝试获取同步状态
        if ((h = whead) == p) {
            //第一次循环，设置自旋次数
            if (spins < 0)
                spins = HEAD_SPINS;
            //自旋次数增加    
            else if (spins < MAX_HEAD_SPINS)
                spins <<= 1;
            for (int k = spins;;) { // spin at head
                long m, s, ns;
                //自旋方式尝试获取同步状态
                //获取成功的话将node设置为head并解放node的cowait链中的节点并返回stamp
                if ((m = (s = state) & ABITS) < RFULL ?
                    U.compareAndSwapLong(this, STATE, s, ns = s + RUNIT) :
                    (m < WBIT && (ns = tryIncReaderOverflow(s)) != 0L)) {
                    WNode c; Thread w;
                    whead = node;
                    node.prev = null;
                    while ((c = node.cowait) != null) {
                        if (U.compareAndSwapObject(node, WCOWAIT,
                                                   c, c.cowait) &&
                            (w = c.thread) != null)
                            U.unpark(w);
                    }
                    return ns;
                }
                //如果有写线程获取到了同步状态(因为可能有写线程闯入)那么随机的--k控制循环次数
                else if (m >= WBIT &&
                         LockSupport.nextSecondarySeed() >= 0 && --k <= 0)
                    break;
            }
        }
        //如果head不为null，解放head的cowait链中的节点
        else if (h != null) {
            WNode c; Thread w;
            while ((c = h.cowait) != null) {
                if (U.compareAndSwapObject(h, WCOWAIT, c, c.cowait) &&
                    (w = c.thread) != null)
                    U.unpark(w);
            }
        }
         //判断稳定性
        if (whead == h) {
            if ((np = node.prev) != p) {
                if (np != null)
                    (p = np).next = node;   // stale
            }
            //尝试设tail的状态位WAITING表示后面还有等待的节点
            else if ((ps = p.status) == 0)
                U.compareAndSwapInt(p, WSTATUS, 0, WAITING);
            //如果tail已经取消了    
            else if (ps == CANCELLED) {
                if ((pp = p.prev) != null) {
                    node.prev = pp;
                    pp.next = node;
                }
            }
            else {
                //超时判定
                long time;
                if (deadline == 0L)
                    time = 0L;
                else if ((time = deadline - System.nanoTime()) <= 0L)
                    return cancelWaiter(node, node, false);
                Thread wt = Thread.currentThread();
                U.putObject(wt, PARKBLOCKER, this);
                node.thread = wt;
                //阻塞等待
                if (p.status < 0 &&
                    (p != h || (state & ABITS) == WBIT) &&
                    whead == h && node.prev == p)
                    U.park(false, time);
                node.thread = null;
                U.putObject(wt, PARKBLOCKER, null);
                //中断处理
                if (interruptible && Thread.interrupted())
                    return cancelWaiter(node, node, true);
            }
        }
    }
}
```
**读锁释放：**
```java
public void unlockRead(long stamp) {
    long s, m; WNode h;
    for (;;) {
        //写计数相关的bit位有改变，或读计数相关的bit位为0，或有写锁，则异常
        if (((s = state) & SBITS) != (stamp & SBITS) ||
            (stamp & ABITS) == 0L || (m = s & ABITS) == 0L || m == WBIT)
            throw new IllegalMonitorStateException();
        //读锁计数未溢出？    
        if (m < RFULL) {
            if (U.compareAndSwapLong(this, STATE, s, s - RUNIT)) {
                if (m == RUNIT && (h = whead) != null && h.status != 0)
                    release(h);
                break;
            }
        }
       //读锁计数溢出？    
        else if (tryDecReaderOverflow(s) != 0L)
            break;
    }
}
```
<a name="rQJ2d"></a>
### 2.6、乐观锁获取
```java
public long tryOptimisticRead() {
    long s;
    //当无写锁，则返回（state & SBITS），即高位的写计数；
    //否则返回0，即获取乐观锁失败
    return (((s = state) & WBIT) == 0L) ? (s & SBITS) : 0L;
}
```
<a name="lHokX"></a>
### 2.7、锁模式的转换
**转换为写锁：**将其他模式（如写锁、读锁、乐观锁）的锁转换为写锁
```java
public long tryConvertToWriteLock(long stamp) {
    long a = stamp & ABITS, m, s, next;
    //写锁的状态未变？
    while (((s = state) & SBITS) == (stamp & SBITS)) {
        //当前无写/读锁？CAS获取写锁
        if ((m = s & ABITS) == 0L) {
            if (a != 0L)
                break;
            if (U.compareAndSwapLong(this, STATE, s, next = s + WBIT))
                return next;
        }
        //当前有写锁，且状态不变，则直接返回原本的版本邮戳
        else if (m == WBIT) {
            //写线程不为当前线程？错误，跳出循环
            if (a != m)
                break;
            return stamp;
        }
        //无写锁，有一个读锁，即当前线程为获取读锁的线程
        //直接cas将读锁释放并获取写锁，即state=（state - RUNIT + WBIT）
        else if (m == RUNIT && a != 0L) {
            if (U.compareAndSwapLong(this, STATE, s,
                                     next = s - RUNIT + WBIT))
                return next;
        }
        //当还有其他线程获取读锁时，自旋等待其他读锁释放
        else
            break;
    }
    return 0L;
}
```
**主要处理：**<br />当锁状态为无读/写锁时，通过CAS获取写锁；<br />当当前线程已获取写锁时，直接返回原本的邮戳；<br />当只有一个读锁，即当只有当前线程获取读锁，没有其他的读锁时，CAS释放读锁并获取写锁；<br />当获取读锁的线程数大于1，即除当前线程还有其他线程也获取到读锁时，自旋等待其他读锁释放；<br />**转换为读锁：**将读/写锁转换为读锁；
```java
public long tryConvertToReadLock(long stamp) {
    long a = stamp & ABITS, m, s, next; WNode h;
    //无写锁？
    while (((s = state) & SBITS) == (stamp & SBITS)) {

        //无读/写锁，则CAS获取读锁
        if ((m = s & ABITS) == 0L) {
            //邮戳错误？退出循环
            if (a != 0L)
                break;
            //读锁计数未溢出？    
            else if (m < RFULL) {
                if (U.compareAndSwapLong(this, STATE, s, next = s + RUNIT))
                    return next;
            }
            //读锁计数溢出？
            else if ((next = tryIncReaderOverflow(s)) != 0L)
                return next;
        }
        //有写锁，且写锁为当前线程？则释放读锁并唤醒后续写锁线程
        else if (m == WBIT) {
            //写锁不为当前线程，错误，跳出循环
            if (a != m)
                break;
            state = next = s + (WBIT + RUNIT);
            if ((h = whead) != null && h.status != 0)
                release(h);
            return next;
        }
        //当前线程已经获取得读锁？
        else if (a != 0L && a < WBIT)
            return stamp;
        else
            break;
    }
    return 0L;
}
```
**主要处理：**<br />若无读/写锁，则CAS获取读锁；<br />若写锁为当前线程，则CAS释放写锁并获取读锁；<br />若当前线程已经获取读锁，则返回原本的邮戳；<br />**转换为乐观锁：**
```java
public long tryConvertToOptimisticRead(long stamp) {
    long a = stamp & ABITS, m, s, next; WNode h;
    U.loadFence();
    for (;;) {
        //写锁状态变更？
        if (((s = state) & SBITS) != (stamp & SBITS))
            break;
        //无读/写锁？直接返回state    
        if ((m = s & ABITS) == 0L) {
            if (a != 0L)
                break;
            return s;
        }
        //已经获取写锁？则直接释放写锁
        else if (m == WBIT) {
            if (a != m)
                break;
            state = next = (s += WBIT) == 0L ? ORIGIN : s;
            if ((h = whead) != null && h.status != 0)
                release(h);
            return next;
        }
        //状态为无读锁或无写锁？
        else if (a == 0L || a >= WBIT)
            break;
        //读锁计数未溢出    
        else if (m < RFULL) {
            if (U.compareAndSwapLong(this, STATE, s, next = s - RUNIT)) {
                if (m == RUNIT && (h = whead) != null && h.status != 0)
                    release(h);
                return next & SBITS;
            }
        }
        //读锁计数溢出？
        else if ((next = tryDecReaderOverflow(s)) != 0L)
            return next & SBITS;
    }
    return 0L;
}
```
**主要处理：**<br />若当前线程已经获取写锁，则直接释放写锁；<br />若当前线程已经获取读锁，则CAS释放读锁；<br />当锁时，返回状态值state；
