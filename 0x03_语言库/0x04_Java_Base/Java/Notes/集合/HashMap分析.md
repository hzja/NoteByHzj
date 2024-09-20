Java HashMap
<a name="ni2JP"></a>
## 前言
HashMap是一个非常重要的集合，日常使用也非常的频繁，同时也是面试重点。本文并不打算讲解基础的使用api，而是深入HashMap的底层，讲解关于HashMap的重点知识。需要读者对散列表和HashMap有一定的认识。HashMap本质上是一个散列表，那么就离不开散列表的三大问题：散列函数、哈希冲突、扩容方案；同时作为一个数据结构，必须考虑多线程并发访问的问题，也就是线程安全。这四大重点则为学习HashMap的重点，也是HashMap设计的重点。HashMap属于Map集合体系的一部分，同时继承了Serializable接口可以被序列化，继承了Cloneable接口可以被复制。他的的继承结构如下：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617932746310-457c3c4c-95ff-4ea8-b5d1-a062695c04cf.png#height=455&id=nvZWP&originHeight=455&originWidth=572&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=572)<br />HashMap并不是全能的，对于一些特殊的情景下的需求官方拓展了一些其他的类来满足，如线程安全的ConcurrentHashMap、记录插入顺序的LinkHashMap、给key排序的TreeMap等。文章内容主要讲解四大重点：散列函数、哈希冲突、扩容方案、线程安全，再补充关键的源码分析和相关的问题。本文所有内容如若未特殊说明，均为JDK1.8版本。
<a name="GEItV"></a>
## 哈希函数
哈希函数的目标是计算key在数组中的下标。判断一个哈希函数的标准是：散列是否均匀、计算是否简单。HashMap哈希函数的步骤：

1. 对key对象的hashcode进行扰动
2. 通过取模求得数组下标

扰动是为了让hashcode的随机性更高，第二步取模就不会让所以的key都聚集在一起，提高散列均匀度。扰动可以看到`hash()`方法：
```java
static final int hash(Object key) {
    int h;
    // 获取到key的hashcode，在高低位异或运算
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
}
```
也就是低16位是和高16位进行异或，高16位保持不变。一般的数组长度都会比较短，取模运算中只有低位参与散列；高位与低位进行异或，让高位也得以参与散列运算，使得散列更加均匀。具体运算如下图（图中为了方便采用8位进行演示，32位同理）：![image.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1617932747260-8cfd687c-ed01-48d6-ae14-e959c48accc4.gif#height=1&id=QFLdC&originHeight=1&originWidth=1&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70&status=done&style=none&title=&width=1)img对hashcode扰动之后需要对结果进行取模。HashMap在jdk1.8并不是简单使用%进行取模，而是采用了另外一种更加高性能的方法。HashMap控制数组长度为2的整数次幂，好处是对hashcode进行求余运算和让hashcode与数组长度-1进行位与运算是相同的效果。如下图：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617932746497-e3b89320-c145-4a1b-a23a-c39a2637da70.webp#height=240&id=ttqIJ&originHeight=240&originWidth=570&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=570)<br />但位与运算的效率却比求余高得多，从而提升了性能。在扩容运算中也利用到了此特性，后面会讲。取模运算的源码看到`putVal()`方法，该方法在`put()`方法中被调用：
```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
               boolean evict) {
    ...
    // 与数组长度-1进行位与运算，得到下标
    if ((p = tab[i = (n - 1) & hash]) == null)
        ...
}
```
完整的hash计算过程可以参考下图：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617932746779-efa8fcd3-dcd8-45b4-a940-1551161d3364.png#height=764&id=LFXVS&originHeight=764&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)上面提到HashMap的数组长度为2的整数次幂，那么HashMap是如何控制数组的长度为2的整数次幂的？修改数组长度有两种情况：

1. 初始化时指定的长度
2. 扩容时的长度增量

先看第一种情况。默认情况下，如未在HashMap构造器中指定长度，则初始长度为16。16是一个较为合适的经验值，他是2的整数次幂，同时太小会频繁触发扩容、太大会浪费空间。如果指定一个非2的整数次幂，会自动转化成大于该指定数的最小2的整数次幂。如指定6则转化为8，指定11则转化为16。结合源码来分析，当初始化指定一个非2的整数次幂长度时，HashMap会调用`tableSizeFor()`方法：
```java
public HashMap(int initialCapacity, float loadFactor) {
    ...
    this.loadFactor = loadFactor;
    // 这里调用了tableSizeFor方法
    this.threshold = tableSizeFor(initialCapacity);
}
static final int tableSizeFor(int cap) {
    // 注意这里必须减一
    int n = cap - 1;
    n |= n >>> 1;
    n |= n >>> 2;
    n |= n >>> 4;
    n |= n >>> 8;
    n |= n >>> 16;
    return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
}
```
`tableSizeFor()`方法的看起来很复杂，作用是使得最高位1后续的所有位都变为1，最后再+1则得到刚好大于initialCapacity的最小2的整数次幂数。如下图（这里使用了8位进行模拟，32位也是同理）：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617932747070-3775390a-832b-459c-b56f-d3858607a749.png#height=494&id=HG0Xp&originHeight=494&originWidth=514&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=514)<br />那为什么必须要对cap进行-1之后再进行运算呢？如果指定的数刚好是2的整数次幂，如果没有-1结果会变成比他大两倍的数，如下：<br />00100 --高位1之后全变1--> 00111 --加1---> 01000<br />第二种改变数组长度的情况是扩容。HashMap每次扩容的大小都是原来的两倍，控制了数组大小一定是2的整数次幂，相关源码如下：
```java
final Node<K,V>[] resize() {
    ...
    if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                 oldCap >= DEFAULT_INITIAL_CAPACITY)
            // 设置为原来的两倍
            newThr = oldThr << 1;
    ...
}
```
<a name="rmioL"></a>
### 小结

1. HashMap通过高16位与低16位进行异或运算来让高位参与散列，提高散列效果；
2. HashMap控制数组的长度为2的整数次幂来简化取模运算，提高性能；
3. HashMap通过控制初始化的数组长度为2的整数次幂、扩容为原来的2倍来控制数组长度一定为2的整数次幂。
<a name="NKVk4"></a>
## 哈希冲突解决方案 
再优秀的hash算法永远无法避免出现hash冲突。hash冲突指的是两个不同的key经过hash计算之后得到的数组下标是相同的。解决hash冲突的方式很多，如开放定址法、再哈希法、公共溢出表法、链地址法。HashMap采用的是链地址法，jdk1.8之后还增加了红黑树的优化，如下图：![image.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1617932747283-390b0e62-469c-41b7-896b-c602e32bf55b.gif#height=1&id=sSTnX&originHeight=1&originWidth=1&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70&status=done&style=none&title=&width=1)img出现冲突后会在当前节点形成链表，而当链表过长之后，会自动转化成红黑树提高查找效率。红黑树是一个查找效率很高的数据结构，时间复杂度为O(logN)，但红黑树只有在数据量较大时才能发挥它的优势。关于红黑树的转化，HashMap做了以下限制。

- 当链表的长度>=8且数组长度>=64时，会把链表转化成红黑树。
- 当链表长度>=8，但数组长度<64时，会优先进行扩容，而不是转化成红黑树。
- 当红黑树节点数<=6，自动转化成链表。

那就有了以下问题：<br />**为什么需要数组长度到64才会转化红黑树？**<br />当数组长度较短时，如16，链表长度达到8已经是占用了最大限度的50%，意味着负载已经快要达到上限，此时如果转化成红黑树，之后的扩容又会再一次把红黑树拆分平均到新的数组中，这样非但没有带来性能的好处，反而会降低性能。所以在数组长度低于64时，优先进行扩容。<br />**为什么要大于等于8转化为红黑树，而不是7或9？**<br />树节点的比普通节点更大，在链表较短时红黑树并未能明显体现性能优势，反而会浪费空间，在链表较短是采用链表而不是红黑树。在理论数学计算中（装载因子=0.75），链表的长度到达8的概率是百万分之一；把7作为分水岭，大于7转化为红黑树，小于7转化为链表。红黑树的出现是为了在某些极端的情况下，抗住大量的hash冲突，正常情况下使用链表是更加合适的。注意，红黑树在jdk1.8之后出现的，jdk1.7采用的是数组+链表模式。
<a name="i1dYL"></a>
### 小结

1. HashMap采用链地址法，当发生冲突时会转化为链表，当链表过长会转化为红黑树提高效率。
2. HashMap对红黑树进行了限制，让红黑树只有在极少数极端情况下进行抗压。
<a name="YRHhm"></a>
## 扩容方案 
当HashMap中的数据越来越多，那么发生hash冲突的概率也就会越来越高，通过数组扩容可以利用空间换时间，保持查找效率在常数时间复杂度。那什么时候进行扩容？由HashMap的一个关键参数控制：装载因子。装载因子=HashMap中节点数/数组长度，他是一个比例值。当HashMap中节点数到达装载因子这个比例时，就会触发扩容；也就是说，装载因子控制了当前数组能够承载的节点数的阈值。如数组长度是16，装载因子是0.75，那么可容纳的节点数是16*0.75=12。装载因子的数值大小需要仔细权衡。装载因子越大，数组利用率越高，同时发生哈希冲突的概率也就越高；装载因子越小，数组利用率降低，但发生哈希冲突的概率也降低了。所以装载因子的大小需要权衡空间与时间之间的关系。在理论计算中，0.75是一个比较合适的数值，大于0.75哈希冲突的概率呈指数级别上升，而小于0.75冲突减少并不明显。HashMap中的装载因子的默认大小是0.75，没有特殊要求的情况下，不建议修改他的值。那么在到达阈值之后，HashMap是如何进行扩容的呢？HashMap会把数组长度扩展为原来的两倍，再把旧数组的数据迁移到新的数组，而HashMap针对迁移做了优化：使用HashMap数组长度是2的整数次幂的特点，以一种更高效率的方式完成数据迁移。JDK1.7之前的数据迁移比较简单，就是遍历所有的节点，把所有的节点依次通过hash函数计算新的下标，再插入到新数组的链表中。这样会有两个缺点：1、每个节点都需要进行一次求余计算；2、插入到新的数组时候采用的是头插入法，在多线程环境下会形成链表环。jdk1.8之后进行了优化，原因在于他控制数组的长度始终是2的整数次幂，每次扩展数组都是原来的2倍，带来的好处是key在新的数组的hash结果只有两种：在原来的位置，或者在原来位置+原数组长度。具体为什么可以看下图：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617932746973-4056a99c-afca-43e4-9673-6ac9e1a6128f.png#height=387&id=DpQfJ&originHeight=387&originWidth=563&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=563)<br />从图中可以看到，在新数组中的hash结果，仅仅取决于高一位的数值。如果高一位是0，那么计算结果就是在原位置，而如果是1，则加上原数组的长度即可。这样只需要判断一个节点的高一位是1 or 0就可以得到他在新数组的位置，而不需要重复hash计算。HashMap把每个链表拆分成两个链表，对应原位置或原位置+原数组长度，再分别插入到新的数组中，保留原来的节点顺序，如下：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617932746965-e7acabb4-e445-4d99-97f0-3bb1a3009b03.png#height=405&id=qUBm5&originHeight=405&originWidth=978&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=978)
<a name="atfGh"></a>
### 小结

1. 装载因子决定了HashMap扩容的阈值，需要权衡时间与空间，一般情况下保持0.75不作改动；
2. HashMap扩容机制结合了数组长度为2的整数次幂的特点，以一种更高的效率完成数据迁移，同时避免头插法造成链表环。
<a name="NuO2n"></a>
## 线程安全
HashMap作为一个集合，主要功能则为CRUD，也就是增删查改数据，那么就肯定涉及到多线程并发访问数据的情况。并发产生的问题，需要特别关注。HashMap并不是线程安全的，在多线程的情况下无法保证数据的一致性。举个例子：HashMap下标2的位置为null，线程A需要将节点X插入下标2的位置，在判断是否为null之后，线程被挂起；此时线程B把新的节点Y插入到下标2的位置；恢复线程A，节点X会直接插入到下标2，覆盖节点Y，导致数据丢失，如下图：jdk1.7及以前扩容时采用的是头插法，这种方式插入速度快，但在多线程环境下会造成链表环，而链表环会在下一次插入时找不到链表尾而发生死循环。那如果结果数据一致性问题呢？解决这个问题有三个方案：

- 采用Hashtable
- 调用`Collections.synchronizeMap()`方法来让HashMap具有多线程能力
- 采用ConcurrentHashMap

前两个方案的思路是相似的，均是每个方法中，对整个对象进行上锁。Hashtable是老一代的集合框架，很多的设计均以及落后，他在每一个方法中均加上了synchronize关键字保证线程安全。
```java
// Hashtable
public synchronized V get(Object key) {...}
public synchronized V put(K key, V value) {...}
public synchronized V remove(Object key) {...}
public synchronized V replace(K key, V value) {...}
...
```
第二种方法是返回一个SynchronizedMap对象，这个对象默认每个方法会锁住整个对象。如下源码：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617932746985-91c4e79d-fabe-4e59-bde1-8008fdf5a288.webp#height=393&id=mxFRe&originHeight=393&originWidth=932&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=932)这里的mutex是什么呢？直接看到构造器：
```java
final Object      mutex;        // Object on which to synchronize
SynchronizedMap(Map<K,V> m) {
    this.m = Objects.requireNonNull(m);
    // 默认为本对象
    mutex = this;
}
SynchronizedMap(Map<K,V> m, Object mutex) {
    this.m = m;
    this.mutex = mutex;
}
```
可以看到默认锁的就是本身，效果和Hashtable其实是一样的。这种简单粗暴锁整个对象的方式造成的后果是：

- 锁是非常重量级的，会严重影响性能。
- 同一时间只能有一个线程进行读写，限制了并发效率。

ConcurrentHashMap的设计就是为了解决此问题。他通过降低锁粒度+CAS的方式来提高效率。简单来说，ConcurrentHashMap锁的并不是整个对象，而是一个数组的一个节点，那么其他线程访问数组其他节点是不会互相影响，极大提高了并发效率；同时ConcurrentHashMap读操作并不需要获取锁，如下图：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617932746919-fc9ab6cc-8002-4a39-bea9-91ecf475e511.png#height=534&id=DOOii&originHeight=534&originWidth=821&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=821)使用了上述的三种解决方案是不是绝对线程安全？先观察下面的代码：
```java
ConcurrentHashMap<String, String> map = new ConcurrentHashMap<>();
map.put("abc","123");
Thread1:
if (map.containsKey("abc")){
    String s = map.get("abc");
}
Thread2:
map.remove("abc");
```
当Thread1调用containsKey之后释放锁，Thread2获得锁并把“abc”移除再释放锁，这个时候Thread1读取到的s就是一个null了，也就出现了问题了。所以ConcurrentHashMap类或者`Collections.synchronizeMap()`方法或者Hashtable都只能在一定的限度上保证线程安全，而无法保证绝对线程安全。关于线程安全，还有一个fast-fail问题，即快速失败。当使用HashMap的迭代器遍历HashMap时，如果此时HashMap发生了结构性改变，如插入新数据、移除数据、扩容等，那么Iteractor会抛出fast-fail异常，防止出现并发异常，在一定限度上保证了线程安全。如下源码：
```java
final Node<K,V> nextNode() {
    ...
    if (modCount != expectedModCount)
        throw new ConcurrentModificationException();
   ...
}
```
创建Iteractor对象时会记录HashMap的modCount变量，每当HashMap发生结构性改变时，modCount会加1。在迭代时判断HashMap的modCount和自己保存的expectedModCount是否一致即可判断是否发生了结构性改变。fast-fail异常只能当做遍历时的一种安全保证，而不能当做多线程并发访问HashMap的手段。若有并发需求，还是需要使用上述的三种方法。**小结**

1. HashMap并不能保证线程安全，在多线程并发访问下会出现意想不到的问题，如数据丢失等
2. HashMap1.8采用尾插法进行扩容，防止出现链表环导致的死循环问题
3. 解决并发问题的的方案有Hashtable、`Collections.synchronizeMap()`、ConcurrentHashMap。其中最佳解决方案是ConcurrentHashMap
4. 上述解决方案并不能完全保证线程安全
5. 快速失败是HashMap迭代机制中的一种并发安全保证
<a name="Ejdz2"></a>
## 源码解析
<a name="PKlEV"></a>
### 关键变量的理解
HashMap源码中有很多的内部变量，这些变量会在下面源码分析中经常出现，首先需要理解这些变量的意义。
```java
// 存放数据的数组
transient Node<K,V>[] table;
// 存储的键值对数目
transient int size;
// HashMap结构修改的次数，主要用于判断fast-fail
transient int modCount;
// 最大限度存储键值对的数目(threshodl=table.length*loadFactor)，也称为阈值
int threshold;
// 装载因子，表示可最大容纳数据数量的比例
final float loadFactor;
// 静态内部类，HashMap存储的节点类型；可存储键值对，本身是个链表结构。
static class Node<K,V> implements Map.Entry<K,V> {...}
```
<a name="L5INL"></a>
### 扩容
HashMap源码中把初始化操作也放到了扩容方法中，因而扩容方法源码主要分为两部分：确定新的数组大小、迁移数据。详细的源码分析如下，写了非常详细的注释，可选择查看。扩容的步骤在上述已经讲过了，可以自行结合源码，分析HashMap是如何实现上述的设计。
```java
final Node<K,V>[] resize() {
    // 变量分别是原数组、原数组大小、原阈值；新数组大小、新阈值
    Node<K,V>[] oldTab = table;
    int oldCap = (oldTab == null) ? 0 : oldTab.length;
    int oldThr = threshold;
    int newCap, newThr = 0;
    // 如果原数组长度大于0
    if (oldCap > 0) {
        // 如果已经超过了设置的最大长度(1<<30,也就是最大整型正数)
        if (oldCap >= MAXIMUM_CAPACITY) {
            // 直接把阈值设置为最大正数
            threshold = Integer.MAX_VALUE;
            return oldTab;
        }
        else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                 oldCap >= DEFAULT_INITIAL_CAPACITY)
            // 设置为原来的两倍
            newThr = oldThr << 1; 
    }
    // 原数组长度为0，但最大限度不是0，把长度设置为阈值
    // 对应的情况就是新建HashMap的时候指定了数组长度
    else if (oldThr > 0) 
        newCap = oldThr;
    // 第一次初始化，默认16和0.75
    // 对应使用默认构造器新建HashMap对象
    else {               
        newCap = DEFAULT_INITIAL_CAPACITY;
        newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
    }
    // 如果原数组长度小于16或者翻倍之后超过了最大限制长度，则重新计算阈值
    if (newThr == 0) {
        float ft = (float)newCap * loadFactor;
        newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                  (int)ft : Integer.MAX_VALUE);
    }
    threshold = newThr;
    @SuppressWarnings({"rawtypes","unchecked"})
    // 建立新的数组
    Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
    table = newTab;
    if (oldTab != null) {
        // 循环遍历原数组，并给每个节点计算新的位置
        for (int j = 0; j < oldCap; ++j) {
            Node<K,V> e;
            if ((e = oldTab[j]) != null) {
                oldTab[j] = null;
                // 如果他没有后继节点，那么直接使用新的数组长度取模得到新下标
                if (e.next == null)
                    newTab[e.hash & (newCap - 1)] = e;
                // 如果是红黑树，调用红黑树的拆解方法
                else if (e instanceof TreeNode)
                    ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
                // 新的位置只有两种可能：原位置，原位置+老数组长度
                // 把原链表拆成两个链表，然后再分别插入到新数组的两个位置上
                // 不用多次调用put方法
                else { 
                    // 分别是原位置不变的链表和原位置+原数组长度位置的链表
                    Node<K,V> loHead = null, loTail = null;
                    Node<K,V> hiHead = null, hiTail = null;
                    Node<K,V> next;
                    // 遍历老链表，判断新增判定位是1or0进行分类
                    do {
                        next = e.next;
                        if ((e.hash & oldCap) == 0) {
                            if (loTail == null)
                                loHead = e;
                            else
                                loTail.next = e;
                            loTail = e;
                        }
                        else {
                            if (hiTail == null)
                                hiHead = e;
                            else
                                hiTail.next = e;
                            hiTail = e;
                        }
                    } while ((e = next) != null);
                    // 最后赋值给新的数组
                    if (loTail != null) {
                        loTail.next = null;
                        newTab[j] = loHead;
                    }
                    if (hiTail != null) {
                        hiTail.next = null;
                        newTab[j + oldCap] = hiHead;
                    }
                }
            }
        }
    }
    // 返回新数组
    return newTab;
}
```
<a name="Fezby"></a>
### 添加数值
调用`put()`方法添加键值对，最终会调用`putVal()`来真正实现添加逻辑。代码解析如下：
```java
public V put(K key, V value) {
    // 获取hash值，再调用putVal方法插入数据
    return putVal(hash(key), key, value, false, true);
}
// onlyIfAbsent表示是否覆盖旧值，true表示不覆盖，false表示覆盖，默认为false
// evict和LinkHashMap的回调方法有关，不在本文讨论范围
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
               boolean evict) {
    // tab是HashMap内部数组，n是数组的长度，i是要插入的下标，p是该下标对应的节点
    Node<K,V>[] tab; Node<K,V> p; int n, i;
    // 判断数组是否是null或者是否是空，若是，则调用resize()方法进行扩容
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length;
    // 使用位与运算代替取模得到下标
    // 判断当前下标是否是null，若是则创建节点直接插入，若不是，进入下面else逻辑
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    else {
        // e表示和当前key相同的节点，若不存在该节点则为null
        // k是当前数组下标节点的key
        Node<K,V> e; K k;
        // 判断当前节点与要插入的key是否相同，是则表示找到了已经存在的key
        if (p.hash == hash &&
            ((k = p.key) == key || (key != null && key.equals(k))))
            e = p;
        // 判断该节点是否是树节点，如果是调用红黑树的方法进行插入
        else if (p instanceof TreeNode)
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        // 最后一种情况是直接链表插入
        else {
            for (int binCount = 0; ; ++binCount) {
                if ((e = p.next) == null) {
                    p.next = newNode(hash, key, value, null);
                    // 长度大于等于8时转化为红黑树
                    // 注意，treeifyBin方法中会进行数组长度判断，
                    // 若小于64，则优先进行数组扩容而不是转化为树
                    if (binCount >= TREEIFY_THRESHOLD - 1) 
                        treeifyBin(tab, hash);
                    break;
                }
                // 找到相同的直接跳出循环
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    break;
                p = e;
            }
        }
        // 如果找到相同的key节点，则判断onlyIfAbsent和旧值是否为null
        // 执行更新或者不操作，最后返回旧值
        if (e != null) { 
            V oldValue = e.value;
            if (!onlyIfAbsent || oldValue == null)
                e.value = value;
            afterNodeAccess(e);
            return oldValue;
        }
    }
    // 如果不是更新旧值，说明HashMap中键值对数量发生变化
    // modCount数值+1表示结构改变
    ++modCount;
    // 判断长度是否达到最大限度，如果是则进行扩容
    if (++size > threshold)
        resize();
    // 最后返回null（afterNodeInsertion是LinkHashMap的回调）
    afterNodeInsertion(evict);
    return null;
}
```
代码中关于每个步骤有了详细的解释，这里来总结一下：

1. 总体上分为两种情况：找到相同的key和找不到相同的key。找了需要判断是否更新并返回旧value，没找到需要插入新的Node、更新节点数并判断是否需要扩容。
2. 查找分为三种情况：数组、链表、红黑树。数组下标i位置不为空且不等于key，那么就需要判断是否树节点还是链表节点并进行查找。
3. 链表到达一定长度后需要扩展为红黑树，当且仅当链表长度>=8且数组长度>=64。

最后画一张图总体再加深一下整个流程的印象：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617932747376-dd97b908-87b6-420a-b569-467dd16be51f.png#height=790&id=UbSKa&originHeight=790&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="CAgkK"></a>
## 其他问题
<a name="H5Quv"></a>
### 为什么jdk1.7以前控制数组的长度为素数，而jdk1.8之后却采用的是2的整数次幂？
答：素数长度可以有效减少哈希冲突；JDK1.8之后采用2的整数次幂是为了提高求余和扩容的效率，同时结合高低位异或的方法使得哈希散列更加均匀。为何素数可以减少哈希冲突？若能保证key的hashcode在每个数字之间都是均匀分布，那么无论是素数还是合数都是相同的效果。例如hashcode在1~20均匀分布，那么无论长度是合数4，还是素数5，分布都是均匀的。而如果hashcode之间的间隔都是2，如1,3,5...,那么长度为4的数组，位置2和位置4两个下标无法放入数据，而长度为5的数组则没有这个问题。长度为合数的数组会使间隔为其因子的hashcode聚集出现，从而使得散列效果降低。
<a name="5X4Li"></a>
### 为什么插入HashMap的数据需要实现hashcode和equals方法？对这两个方法有什么要求？
答：通过hashcode来确定插入下标，通过equals比较来寻找数据；两个相等的key的hashcode必须相等，但拥有相同的hashcode的对象不一定相等。这里需要区分好他们之间的区别：hashcode就像一个人的名，相同的人名字肯定相等，但是相同的名字不一定是同个人；equals比较内容是否相同，一般由对象覆盖重写，默认情况下比较的是引用地址；“==”引用队形比较的是引用地址是否相同，值对象比较的是值是否相同。HashMap中需要使用hashcode来获取key的下标，如果两个相同的对象的hashcode不同，那么会造成HashMap中存在相同的key；所以equals返回相同的key他们的hashcode一定要相同。HashMap比较两个元素是否相同采用了三种比较方法结合：`p.hash == hash && ((k = p.key) == key || (key != null && key.equals(k)))` 。
