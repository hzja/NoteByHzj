Java ThreadLocal
<a name="ACKe0"></a>
### 1、背景
ThreadLocal非常适合对Java多线程编程感兴趣的程序员作为入门类阅读，原因两方面：

1. 加上注释源码也不过七八百行。
2. 结构清晰，代码简洁。

本文重点导读ThreadLocal中的嵌套内部类ThreadLocalMap，对ThreadLocal本身API的介绍简略带过。<br />**读ThreadLocal源码，不读ThreadLocalMap的实现，和没看过没多大差别**。
<a name="S7MoK"></a>
### 2、两个问题
先回答两个问题：

1. 什么是ThreadLocal？<br />ThreadLocal类顾名思义可以理解为线程本地变量。也就是说如果定义了一个ThreadLocal，每个线程往这个ThreadLocal中读写是线程隔离，互相之间不会影响的。它提供了一种将可变数据通过每个线程有自己的独立副本从而实现线程封闭的机制。
2. 它大致的实现思路是怎样的？<br />Thread类有一个类型为`ThreadLocal.ThreadLocalMap`的实例变量threadLocals，也就是说每个线程有一个自己的ThreadLocalMap。ThreadLocalMap有自己的独立实现，可以简单地将它的key视作ThreadLocal，value为代码中放入的值（实际上key并不是ThreadLocal本身，而是它的一个弱引用）。每个线程在往某个ThreadLocal里塞值的时候，都会往自己的ThreadLocalMap里存，读也是以某个ThreadLocal作为引用，在自己的map里找对应的key，从而实现了线程隔离。
<a name="v0G8c"></a>
### 3、ThreadLocal的API
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639322484598-bc712e57-e729-4962-95d6-7e75bb45d49b.webp#clientId=u46db23f7-ebb6-4&from=paste&id=u7700501b&originHeight=191&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u35b2f300-ac6b-4e26-a0b9-35e8fb34db4&title=)
<a name="TGVk9"></a>
### 4、ThreadLocalMap的源码实现
ThreadLocalMap的源码实现，才是读ThreadLocal源码真正要领悟的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639322484772-1c824ba4-1833-4cb1-83b4-58396b21df35.webp#clientId=u46db23f7-ebb6-4&from=paste&id=u62fc619a&originHeight=559&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u96e12994-90d7-416d-93a7-eb0c20feb4a&title=)<br />ThreadLocalMap提供了一种为ThreadLocal定制的高效实现，并且自带一种基于弱引用的垃圾清理机制。<br />下面从基本结构开始一点点解读。
<a name="rsU2e"></a>
#### 4.1 存储结构
既然是个map（注意不要与java.util.map混为一谈，这里指的是概念上的map），当然得要有自己的key和value，上面回答的问题2中也已经提及，可以将其简单视作key为ThreadLocal，value为实际放入的值。之所以说是**简单视作**，因为实际上ThreadLocal中存放的是ThreadLocal的弱引用。来看看ThreadLocalMap里的节点是如何定义的。
```java
static class Entry extends WeakReference<java.lang.ThreadLocal<?>> {
    // 往ThreadLocal里实际塞入的值
    Object value;

    Entry(java.lang.ThreadLocal<?> k, Object v) {
        super(k);
        value = v;
    }
}
```
`Entry`便是ThreadLocalMap里定义的节点，它继承了WeakReference类，定义了一个类型为Object的value，用于存放塞到ThreadLocal里的值。
<a name="KE4O9"></a>
#### 4.2 为什么要弱引用
读到这里，如果不问不答为什么是这样的定义形式，为什么要用弱引用，等于没读懂源码。<br />因为如果这里使用普通的key-value形式来定义存储结构，实质上就会造成节点的生命周期与线程强绑定，只要线程没有销毁，那么节点在GC分析中一直处于可达状态，没办法被回收，而程序本身也无法判断是否可以清理节点。弱引用是Java中四档引用的第三档，比软引用更加弱一些，如果一个对象没有强引用链可达，那么一般活不过下一次GC。当某个ThreadLocal已经没有强引用可达，则随着它被垃圾回收，在ThreadLocalMap里对应的Entry的键值会失效，这为ThreadLocalMap本身的垃圾清理提供了便利。
<a name="PzXHC"></a>
#### 4.3 类成员变量与相应方法
```java
/**
 * 初始容量，必须为2的幂
 */
private static final int INITIAL_CAPACITY = 16;

/**
 * Entry表，大小必须为2的幂
 */
private Entry[] table;

/**
 * 表里entry的个数
 */
private int size = 0;

/**
 * 重新分配表大小的阈值，默认为0
 */
private int threshold;
```
可以看到，ThreadLocalMap维护了一个Entry表或者说Entry数组，并且要求表的大小必须为2的幂，同时记录表里面entry的个数以及下一次需要扩容的阈值。<br />显然这里会产生一个问题，**为什么必须是2的幂？**很好，但是目前还无法回答，带着问题接着往下读。
```java
/**
 * 设置resize阈值以维持最坏2/3的装载因子
 */
private void setThreshold(int len) {
    threshold = len * 2 / 3;
}

/**
 * 环形意义的下一个索引
 */
private static int nextIndex(int i, int len) {
    return ((i + 1 < len) ? i + 1 : 0);
}

/**
 * 环形意义的上一个索引
 */
private static int prevIndex(int i, int len) {
    return ((i - 1 >= 0) ? i - 1 : len - 1);
}
```
ThreadLocal需要维持一个最坏2/3的负载因子，对于负载因子相信应该不会陌生，在HashMap中就有这个概念。<br />ThreadLocal有两个方法用于得到上一个/下一个索引，注意这里实际上是环形意义下的上一个与下一个。<br />**由于ThreadLocalMap使用线性探测法来解决散列冲突，所以实际上Entry[]数组在程序逻辑上是作为一个环形存在的。**<br />关于开放寻址、线性探测等内容，可以参考网上资料或者TAOCP（《计算机程序设计艺术》）第三卷的6.4章节。<br />至此，已经可以大致勾勒出ThreadLocalMap的内部存储结构。下面是绘制的示意图。虚线表示弱引用，实线表示强引用。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639322484559-178f46ec-ea05-4ff9-8394-432a8d2b93fa.webp#clientId=u46db23f7-ebb6-4&from=paste&id=u5465a0f9&originHeight=376&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u22595941-48c3-458e-9622-34c811a95a0&title=)<br />ThreadLocalMap维护了Entry环形数组，数组中元素Entry的逻辑上的key为某个ThreadLocal对象（实际上是指向该ThreadLocal对象的弱引用），value为代码中该线程往该ThreadLoacl变量实际塞入的值。
<a name="QddtD"></a>
#### 4.4 构造函数
接下来再来看看ThreadLocalMap的一个构造函数
```java
/**
 * 构造一个包含firstKey和firstValue的map。
 * ThreadLocalMap是惰性构造的，所以只有当至少要往里面放一个元素的时候才会构建它。
 */
ThreadLocalMap(java.lang.ThreadLocal<?> firstKey, Object firstValue) {
    // 初始化table数组
    table = new Entry[INITIAL_CAPACITY];
    // 用firstKey的threadLocalHashCode与初始大小16取模得到哈希值
    int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);
    // 初始化该节点
    table[i] = new Entry(firstKey, firstValue);
    // 设置节点表大小为1
    size = 1;
    // 设定扩容阈值
    setThreshold(INITIAL_CAPACITY);
}
```
这个构造函数在set和get的时候都可能会被间接调用以初始化线程的ThreadLocalMap。
<a name="X5lUq"></a>
#### 4.5 哈希函数
重点看一下上面构造函数中的<br />`**int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);**`这一行代码。<br />ThreadLocal类中有一个被final修饰的类型为int的threadLocalHashCode，它在该ThreadLocal被构造的时候就会生成，相当于一个ThreadLocal的ID，而它的值来源于
```java
/*
 * 生成hash code间隙为这个魔数，可以让生成出来的值或者说ThreadLocal的ID较为均匀地分布在2的幂大小的数组中。
 */
private static final int HASH_INCREMENT = 0x61c88647;

private static int nextHashCode() {
    return nextHashCode.getAndAdd(HASH_INCREMENT);
}
```
可以看出，它是在上一个被构造出的ThreadLocal的ID/threadLocalHashCode的基础上加上一个魔数0x61c88647的。这个魔数的选取与斐波那契散列有关，0x61c88647对应的十进制为1640531527。斐波那契散列的乘数可以用(long) ((1L << 31) * (Math.sqrt(5) - 1))可以得到2654435769，如果把这个值给转为带符号的int，则会得到-1640531527。换句话说<br />`**(1L << 32) - (long) ((1L << 31) * (Math.sqrt(5) - 1))**`得到的结果就是1640531527也就是0x61c88647。通过理论与实践，当用0x61c88647作为魔数累加为每个ThreadLocal分配各自的ID也就是threadLocalHashCode再与2的幂取模，得到的结果分布很均匀。<br />ThreadLocalMap使用的是**线性探测法**，均匀分布的好处在于很快就能探测到下一个临近的可用slot，从而保证效率。这就回答了上文抛出的为什么大小要为2的幂的问题。为了优化效率。<br />对于`**& (INITIAL_CAPACITY - 1)**`，相信有过算法竞赛经验或是阅读源码较多的程序员，一看就明白，对于2的幂作为模数取模，可以用&(2n-1)来替代%2n，位运算比取模效率高很多。至于为什么，因为对2^n取模，只要不是低n位对结果的贡献显然都是0，会影响结果的只能是低n位。<br />**可以说在ThreadLocalMap中，形如**`**key.threadLocalHashCode & (table.length - 1)**`**（其中key为一个ThreadLocal实例）这样的代码片段实质上就是在求一个ThreadLocal实例的哈希值，只是在源码实现中没有将其抽为一个公用函数。**
<a name="xn6fx"></a>
#### 4.6 `getEntry`方法
这个方法会被ThreadLocal的`get`方法直接调用，用于获取map中某个ThreadLocal存放的值。
```java
private Entry getEntry(ThreadLocal<?> key) {
    // 根据key这个ThreadLocal的ID来获取索引，也即哈希值
    int i = key.threadLocalHashCode & (table.length - 1);
    Entry e = table[i];
    // 对应的entry存在且未失效且弱引用指向的ThreadLocal就是key，则命中返回
    if (e != null && e.get() == key) {
        return e;
    } else {
        // 因为用的是线性探测，所以往后找还是有可能能够找到目标Entry的。
        return getEntryAfterMiss(key, i, e);
    }
}

/*
 * 调用getEntry未直接命中的时候调用此方法
 */
private Entry getEntryAfterMiss(ThreadLocal<?> key, int i, Entry e) {
    Entry[] tab = table;
    int len = tab.length;

    // 基于线性探测法不断向后探测直到遇到空entry。
    while (e != null) {
        ThreadLocal<?> k = e.get();
        // 找到目标
        if (k == key) {
            return e;
        }
        if (k == null) {
            // 该entry对应的ThreadLocal已经被回收，调用expungeStaleEntry来清理无效的entry
            expungeStaleEntry(i);
        } else {
            // 环形意义下往后面走
            i = nextIndex(i, len);
        }
        e = tab[i];
    }
    return null;
}

/**
 * 这个函数是ThreadLocal中核心清理函数，它做的事情很简单：
 * 就是从staleSlot开始遍历，将无效（弱引用指向对象被回收）清理，即对应entry中的value置为null，将指向这个entry的table[i]置为null，直到扫到空entry。
 * 另外，在过程中还会对非空的entry作rehash。
 * 可以说这个函数的作用就是从staleSlot开始清理连续段中的slot（断开强引用，rehash slot等）
 */
private int expungeStaleEntry(int staleSlot) {
    Entry[] tab = table;
    int len = tab.length;

    // 因为entry对应的ThreadLocal已经被回收，value设为null，显式断开强引用
    tab[staleSlot].value = null;
    // 显式设置该entry为null，以便垃圾回收
    tab[staleSlot] = null;
    size--;

    Entry e;
    int i;
    for (i = nextIndex(staleSlot, len); (e = tab[i]) != null; i = nextIndex(i, len)) {
        ThreadLocal<?> k = e.get();
        // 清理对应ThreadLocal已经被回收的entry
        if (k == null) {
            e.value = null;
            tab[i] = null;
            size--;
        } else {
            /*
             * 对于还没有被回收的情况，需要做一次rehash。
             * 
             * 如果对应的ThreadLocal的ID对len取模出来的索引h不为当前位置i，
             * 则从h向后线性探测到第一个空的slot，把当前的entry给挪过去。
             */
            int h = k.threadLocalHashCode & (len - 1);
            if (h != i) {
                tab[i] = null;

                /*
                 * 在原代码的这里有句注释值得一提，原注释如下：
                 *
                 * Unlike Knuth 6.4 Algorithm R, we must scan until
                 * null because multiple entries could have been stale.
                 *
                 * 这段话提及了Knuth高德纳的著作TAOCP（《计算机程序设计艺术》）的6.4章节（散列）
                 * 中的R算法。R算法描述了如何从使用线性探测的散列表中删除一个元素。
                 * R算法维护了一个上次删除元素的index，当在非空连续段中扫到某个entry的哈希值取模后的索引
                 * 还没有遍历到时，会将该entry挪到index那个位置，并更新当前位置为新的index，
                 * 继续向后扫描直到遇到空的entry。
                 *
                 * ThreadLocalMap因为使用了弱引用，所以其实每个slot的状态有三种也即
                 * 有效（value未回收），无效（value已回收），空（entry==null）。
                 * 正是因为ThreadLocalMap的entry有三种状态，所以不能完全套高德纳原书的R算法。
                 *
                 * 因为expungeStaleEntry函数在扫描过程中还会对无效slot清理将之转为空slot，
                 * 如果直接套用R算法，可能会出现具有相同哈希值的entry之间断开（中间有空entry）。
                 */
                while (tab[h] != null) {
                    h = nextIndex(h, len);
                }
                tab[h] = e;
            }
        }
    }
    // 返回staleSlot之后第一个空的slot索引
    return i;
}
```
<br />回顾一下从ThreadLocal读一个值可能遇到的情况：<br />根据入参threadLocal的threadLocalHashCode对表容量取模得到index

- 如果index对应的slot就是要读的threadLocal，则直接返回结果
- 调用getEntryAfterMiss线性探测，过程中每碰到无效slot，调用expungeStaleEntry进行段清理；如果找到了key，则返回结果entry
- 没有找到key，返回null
<a name="xhHHM"></a>
#### 4.7 set方法
```java
private void set(ThreadLocal<?> key, Object value) {

    Entry[] tab = table;
    int len = tab.length;
    int i = key.threadLocalHashCode & (len - 1);
    // 线性探测
    for (Entry e = tab[i]; e != null; e = tab[i = nextIndex(i, len)]) {
        ThreadLocal<?> k = e.get();
        // 找到对应的entry
        if (k == key) {
            e.value = value;
            return;
        }
        // 替换失效的entry
        if (k == null) {
            replaceStaleEntry(key, value, i);
            return;
        }
    }

    tab[i] = new Entry(key, value);
    int sz = ++size;
    if (!cleanSomeSlots(i, sz) && sz >= threshold) {
        rehash();
    }
}

private void replaceStaleEntry(ThreadLocal<?> key, Object value,
                               int staleSlot) {
    Entry[] tab = table;
    int len = tab.length;
    Entry e;

    // 向前扫描，查找最前的一个无效slot
    int slotToExpunge = staleSlot;
    for (int i = prevIndex(staleSlot, len);
         (e = tab[i]) != null;
         i = prevIndex(i, len)) {
        if (e.get() == null) {
            slotToExpunge = i;
        }
    }

    // 向后遍历table
    for (int i = nextIndex(staleSlot, len);
         (e = tab[i]) != null;
         i = nextIndex(i, len)) {
        ThreadLocal<?> k = e.get();

        // 找到了key，将其与无效的slot交换
        if (k == key) {
            // 更新对应slot的value值
            e.value = value;

            tab[i] = tab[staleSlot];
            tab[staleSlot] = e;

            /*
             * 如果在整个扫描过程中（包括函数一开始的向前扫描与i之前的向后扫描）
             * 找到了之前的无效slot则以那个位置作为清理的起点，
             * 否则则以当前的i作为清理起点
             */
            if (slotToExpunge == staleSlot) {
                slotToExpunge = i;
            }
            // 从slotToExpunge开始做一次连续段的清理，再做一次启发式清理
            cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);
            return;
        }

        // 如果当前的slot已经无效，并且向前扫描过程中没有无效slot，则更新slotToExpunge为当前位置
        if (k == null && slotToExpunge == staleSlot) {
            slotToExpunge = i;
        }
    }

    // 如果key在table中不存在，则在原地放一个即可
    tab[staleSlot].value = null;
    tab[staleSlot] = new Entry(key, value);

    // 在探测过程中如果发现任何无效slot，则做一次清理（连续段清理+启发式清理）
    if (slotToExpunge != staleSlot) {
        cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);
    }
}

/**
 * 启发式地清理slot,
 * i对应entry是非无效（指向的ThreadLocal没被回收，或者entry本身为空）
 * n是用于控制控制扫描次数的
 * 正常情况下如果log n次扫描没有发现无效slot，函数就结束了
 * 但是如果发现了无效的slot，将n置为table的长度len，做一次连续段的清理
 * 再从下一个空的slot开始继续扫描
 * 
 * 这个函数有两处地方会被调用，一处是插入的时候可能会被调用，另外个是在替换无效slot的时候可能会被调用，
 * 区别是前者传入的n为元素个数，后者为table的容量
 */
private boolean cleanSomeSlots(int i, int n) {
    boolean removed = false;
    Entry[] tab = table;
    int len = tab.length;
    do {
        // i在任何情况下自己都不会是一个无效slot，所以从下一个开始判断
        i = nextIndex(i, len);
        Entry e = tab[i];
        if (e != null && e.get() == null) {
            // 扩大扫描控制因子
            n = len;
            removed = true;
            // 清理一个连续段
            i = expungeStaleEntry(i);
        }
    } while ((n >>>= 1) != 0);
    return removed;
}

private void rehash() {
    // 做一次全量清理
    expungeStaleEntries();

    /*
     * 因为做了一次清理，所以size很可能会变小。
     * ThreadLocalMap这里的实现是调低阈值来判断是否需要扩容，
     * threshold默认为len*2/3，所以这里的threshold - threshold / 4相当于len/2
     */
    if (size >= threshold - threshold / 4) {
        resize();
    }
}

/*
 * 做一次全量清理
 */
private void expungeStaleEntries() {
    Entry[] tab = table;
    int len = tab.length;
    for (int j = 0; j < len; j++) {
        Entry e = tab[j];
        if (e != null && e.get() == null) {
            /*
             * 个人觉得这里可以取返回值，如果大于j的话取了用，这样也是可行的。
             * 因为expungeStaleEntry执行过程中是把连续段内所有无效slot都清理了一遍了。
             */
            expungeStaleEntry(j);
        }
    }
}

/**
 * 扩容，因为需要保证table的容量len为2的幂，所以扩容即扩大2倍
 */
private void resize() {
    Entry[] oldTab = table;
    int oldLen = oldTab.length;
    int newLen = oldLen * 2;
    Entry[] newTab = new Entry[newLen];
    int count = 0;

    for (int j = 0; j < oldLen; ++j) {
        Entry e = oldTab[j];
        if (e != null) {
            ThreadLocal<?> k = e.get();
            if (k == null) {
                e.value = null; 
            } else {
                // 线性探测来存放Entry
                int h = k.threadLocalHashCode & (newLen - 1);
                while (newTab[h] != null) {
                    h = nextIndex(h, newLen);
                }
                newTab[h] = e;
                count++;
            }
        }
    }

    setThreshold(newLen);
    size = count;
    table = newTab;
}
```
回顾一下ThreadLocal的`set`方法可能会有的情况

- 探测过程中slot都不无效，并且顺利找到key所在的slot，直接替换即可
- 探测过程中发现有无效slot，调用`replaceStaleEntry`，效果是最终一定会把key和value放在这个slot，并且会尽可能清理无效slot
   - 在`replaceStaleEntry`过程中，如果找到了key，则做一个swap把它放到那个无效slot中，value置为新值
   - 在`replaceStaleEntry`过程中，没有找到key，直接在无效slot原地放entry
- 探测没有发现key，则在连续段末尾的后一个空位置放上entry，这也是线性探测法的一部分。放完后，做一次启发式清理，如果没清理出去key，并且当前table大小已经超过阈值了，则做一次`rehash`，`rehash`函数会调用一次全量清理slot方法也即`expungeStaleEntries`，如果完了之后table大小超过了threshold - threshold / 4，则进行扩容2倍
<a name="nwTF2"></a>
#### 4.8 `remove`方法
```java
/**
 * 从map中删除ThreadLocal
 */
private void remove(ThreadLocal<?> key) {
    Entry[] tab = table;
    int len = tab.length;
    int i = key.threadLocalHashCode & (len - 1);
    for (Entry e = tab[i];
         e != null;
         e = tab[i = nextIndex(i, len)]) {
        if (e.get() == key) {
            // 显式断开弱引用
            e.clear();
            // 进行段清理
            expungeStaleEntry(i);
            return;
        }
    }
}
```
remove方法相对于getEntry和set方法比较简单，直接在table中找key，如果找到了，把弱引用断了做一次段清理。
<a name="styQe"></a>
### 5、ThreadLocal与内存泄漏
关于ThreadLocal是否会引起内存泄漏也是一个比较有争议性的问题，其实就是要看对内存泄漏的准确定义是什么。<br />认为ThreadLocal会引起内存泄漏的说法是因为如果一个ThreadLocal对象被回收了，往里面放的value对于**【当前线程->当前线程的threadLocals(**`**ThreadLocal.ThreadLocalMap**`**对象）->Entry数组->某个entry.value】**这样一条强引用链是可达的，因此value不会被回收。<br />认为ThreadLocal不会引起内存泄漏的说法是因为`ThreadLocal.ThreadLocalMap`源码实现中自带一套自我清理的机制。<br />之所以有关于内存泄露的讨论是因为在有线程复用如线程池的场景中，一个线程的寿命很长，大对象长期不被回收影响系统运行效率与安全。如果线程不会复用，用完即销毁了也不会有ThreadLocal引发内存泄露的问题。《Effective Java》一书中的第6条对这种内存泄露称为**unintentional object retention**(无意识的对象保留）。<br />当仔细读过ThreadLocalMap的源码，可以推断，如果在使用的ThreadLocal的过程中，显式地进行`remove`是个很好的编码习惯，这样是不会引起内存泄漏。<br />那么如果没有显式地进行remove呢？只能说如果对应线程之后调用ThreadLocal的`get`和`set`方法都有**很高的概率**会顺便清理掉无效对象，断开value强引用，从而大对象被收集器回收。<br />但无论如何，应该考虑到何时调用ThreadLocal的`remove`方法。一个比较熟悉的场景就是对于一个请求一个线程的server如tomcat，在代码中对web api作一个切面，存放一些如用户名等用户信息，在连接点方法结束后，再显式调用remove。
<a name="S2MsI"></a>
### 6、`InheritableThreadLocal`原理
对于InheritableThreadLocal，本文不作过多介绍，只是简单略过。<br />ThreadLocal本身是线程隔离的，InheritableThreadLocal提供了一种父子线程之间的数据共享机制。<br />它的具体实现是在Thread类中除了threadLocals外还有一个**inheritableThreadLocals**对象。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639322484771-fe24cb01-ef50-4d0f-a384-d5c62dfb3c2e.webp#clientId=u46db23f7-ebb6-4&from=paste&id=ufe6c9916&originHeight=191&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u23e37e1c-f439-4bfe-be9a-058fccc7e78&title=)<br />在线程对象初始化的时候，会调用ThreadLocal的**createInheritedMap**从父线程的**inheritableThreadLocals**中把有效的entry都拷过来<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639322484652-f8988a81-7173-4532-b36f-9ad0ed6db888.webp#clientId=u46db23f7-ebb6-4&from=paste&id=uc05a90ce&originHeight=60&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udd065048-cc48-4a5a-8601-201826941cd&title=)<br />可以看一下其中的具体实现
```java
private ThreadLocalMap(ThreadLocalMap parentMap) {
    Entry[] parentTable = parentMap.table;
    int len = parentTable.length;
    setThreshold(len);
    table = new Entry[len];

    for (int j = 0; j < len; j++) {
        Entry e = parentTable[j];
        if (e != null) {
            @SuppressWarnings("unchecked")
            ThreadLocal<Object> key = (ThreadLocal<Object>) e.get();
            if (key != null) {
                // 这里的childValue方法在InheritableThreadLocal中默认实现为返回本身值，可以被重写
                Object value = key.childValue(e.value);
                Entry c = new Entry(key, value);
                int h = key.threadLocalHashCode & (len - 1);
                while (table[h] != null)
                    h = nextIndex(h, len);
                table[h] = c;
                size++;
            }
        }
    }
}
```
还是比较简单的，做的事情就是以父线程的`**inheritableThreadLocalMap**`为数据源，过滤出有效的entry，初始化到自己的`**inheritableThreadLocalMap**`中。其中`childValue`可以被重写。<br />需要注意的地方是`**InheritableThreadLocal**`只是在子线程创建的时候会去拷一份父线程的`**inheritableThreadLocals**`。如果父线程是在子线程创建后再set某个`InheritableThreadLocal`对象的值，对子线程是不可见的。
<a name="q6CSJ"></a>
### 7、总结
重点介绍了ThreadLocal中ThreadLocalMap的大致实现原理以及ThreadLocal内存泄露的问题以及简略介绍`InheritableThreadLocal`。作为Josh Bloch和Doug Lea两位大师之作，ThreadLocal本身实现的算法与技巧还是很优雅的。在开发过程中，ThreadLocal用到恰到好处的话，可以消除一些代码的重复。但也要注意过度使用ThreadLocal很容易加大类之间的耦合度与依赖关系（开发过程可能会不得不过度考虑某个ThreadLocal在调用时是否已有值，存放的是哪个类放的什么值）。
