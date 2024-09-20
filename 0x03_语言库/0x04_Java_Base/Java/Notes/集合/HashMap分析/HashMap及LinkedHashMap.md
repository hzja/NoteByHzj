Java HashMap LinkedHashMap<br />HashMap是Map族中最为常用的一种，也是Java Collection Framework的重要成员。HashMap和双向链表合二为一即是LinkedHashMap。所谓LinkedHashMap，其落脚点在HashMap，因此更准确地说，它是一个将所有Node节点链入一个双向链表的HashMap。<br />下面基于JDK 1.8的源码来学习HashMap及LinkedHashMap的数据结构、原理。不同JDK版本之间也许会有些许差异，但不影响原理学习，JDK8相比以前对HashMap的修改比较大。
<a name="RvkRB"></a>
## 1、HashMap概述
Map是 Key-Value键值对映射的抽象接口，该映射不包括重复的键，即一个键对应一个值。HashMap是Java Collection Framework的重要成员，也是Map族(如下图所示)中最为常用的一种。<br />**简单地说，HashMap是基于哈希表的Map接口的实现，以Key-Value的形式存在，即存储的对象是 Node (同时包含了Key和Value) 。**<br />在HashMap中，其会根据hash算法来计算key-value的存储位置并进行快速存取。特别地，HashMap最多只允许一条Node的key为Null，但允许多条Node的value为Null。此外，HashMap是Map 的一个非同步的实现。<br />以下是HashMap的类继承图<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633532163885-b189e997-8595-4a71-9b3a-0df482b34e5c.webp#clientId=ua9e5b3f4-4d34-4&from=paste&id=ub3177817&originHeight=486&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u933c41e6-adba-4388-bb4f-aac68db4311)<br />必须指出的是，虽然容器号称存储的是 Java 对象，但实际上并不会真正将 Java 对象放入容器中，只是在容器中保留这些对象的引用。也就是说，Java 容器实际上包含的是引用变量，而这些引用变量指向了要实际保存的 Java 对象。
<a name="KL39k"></a>
### 1.1、HashMap定义及构造函数
JDK中的定义为
```java
public class HashMap<K,V> extends AbstractMap<K,V> implements Map<K,V>, Cloneable, Serializable {
    //...
}
```
HashMap 一共提供了四个构造函数，其中 默认无参的构造函数 和 参数为Map的构造函数 为 Java Collection Framework 规范的推荐实现，其余两个构造函数则是 HashMap 专门提供的。
```java
public HashMap(int initialCapacity) {
    this(initialCapacity, DEFAULT_LOAD_FACTOR);
}
//仅仅将负载因子初始化为默认值
public HashMap() {
    this.loadFactor = DEFAULT_LOAD_FACTOR; 
    // all other fields defaulted
}
```
`HashMap(int initialCapacity, float loadFactor)`构造函数意在构造一个指定初始容量和指定负载因子的空HashMap，其源码如下：
```java
public HashMap(int initialCapacity, float loadFactor) {
    if (initialCapacity < 0)
        throw new IllegalArgumentException("Illegal initial capacity: " +
                                           initialCapacity);
    //容量最大为2的30次方
    if (initialCapacity > MAXIMUM_CAPACITY)
        initialCapacity = MAXIMUM_CAPACITY;
    if (loadFactor <= 0 || Float.isNaN(loadFactor))
        throw new IllegalArgumentException("Illegal load factor: " +
                                           loadFactor);
    this.loadFactor = loadFactor;
    //这里调用函数计算触发扩容的阈值，threshold/loadFactor就是容量
    this.threshold = tableSizeFor(initialCapacity);
}
```
以上构造函数的最后一行就是jdk8的修改，实际上在jdk7之前的版本，这个构造方法最后一行就是：
```java
table = new Entry[capacity];
```
但是jdk8的最后一行并没有立刻new出一个数组，而是调用了tableSizeFor方法，将结果赋值给了threshold变量。tableSizeFor方法源码如下，从注释就可以看出来，其目的是要获得大于cap的最小的2的幂。比如cap=10，则返回16。
```java
/**
 * Returns a power of two size for the given target capacity.
 */
static final int tableSizeFor(int cap) {
    int n = cap - 1;
    n |= n >>> 1;
    n |= n >>> 2;
    n |= n >>> 4;
    n |= n >>> 8;
    n |= n >>> 16;
    return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
}
```
<a name="DJliG"></a>
### 1.2、HashMap的数据结构
在Java中最常用的两种结构是数组和链表，几乎所有的数据结构都可以利用这两种来组合实现，HashMap就是这种应用的一个典型。<br />实际上，经典的HashMap就是一个链表数组，只是jdk1.8再次对经典hashMap的数据结构作了小幅调整，如下是当前HaspMap的数据结构：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633532163998-d889d242-8174-4459-83cd-65068483542f.webp#clientId=ua9e5b3f4-4d34-4&from=paste&id=ucc90d9a6&originHeight=619&originWidth=994&originalType=url&ratio=1&status=done&style=none&taskId=udbb7f10f-bc63-4ea0-843b-bb0d420cddf)<br />在JDK1.6和JDK1.7中，HashMap采用数组+链表实现，即使用链表处理冲突，同一hash值的key-value键值对都存储在一个链表里。但是当数组中一个位置上的元素较多，即hash值相等的元素较多时，通过key值依次查找的效率较低。<br />而在JDK1.8中，HashMap采用数组+链表+红黑树实现，当链表长度超过阈值8时，并且数组总容量超过64时，将链表转换为红黑树，这样大大减少了查找时间。从链表转换为红黑树后新加入键值对的效率降低，但查询、删除的效率都变高了。而当发生扩容或remove键值对导致原有的红黑树内节点数量小于6时，则又将红黑树转换成链表。<br />每一个HashMap都有一个Node类型的table数组，其中Node类型的定义如下：
```java
static class Node<K,V> implements Map.Entry<K,V> {
    final int hash;         // 声明 hash 值为 final 的
    final K key;            // 声明 key 为 final 的
    V value;                // 键值对的值
    Node<K,V> next;         // 指向下一个节点的引用

    Node(int hash, K key, V value, Node<K,V> next) {
        this.hash = hash;
        this.key = key;
        this.value = value;
        this.next = next;
    }
}
```
Node为HashMap的内部类，实现了Map.Entry接口，其包含了键key、值value、下一个节点next，以及hash值四个属性。事实上，Node是构成哈希表的基石，是哈希表所存储的元素的具体形式。值得注意的是，int类型的hash值及引用变量key都被声明成final，即不可变。
<a name="TrZq9"></a>
### 1.3、HashMap的快速存取
在HashMap中，最常用的两个操作就是：put(Key,Value)和get(Key)。都知道，HashMap中的Key是唯一的，那它是如何保证唯一性的呢？<br />首先想到的是用equals比较，没错，这样是可以实现的，但随着元素的增多，put和get的效率将越来越低，这里的时间复杂度是O(n)。也就是说，假如HashMap有1000个元素，那么put时就需要比较1000次，这是相当耗时的，远达不到HashMap快速存取的目的。<br />实际上，HashMap很少会用到equals方法，因为其内通过一个哈希表管理所有元素，利用哈希算法可以快速的存取元素。当调用put方法存值时，HashMap首先会调用Key的`hashCode`方法，然后基于此值获取Key的哈希码，通过哈希码快速找到某个位置，这个位置可以被称之为 `bucketIndex`。<br />根据equals方法与hashCode的协定可以知道，如果两个对象的`hashCode`不同，那么equals一定为 false；如果其hashCode相同，equals也不一定为true。所以，理论上，hashCode 可能存在碰撞的情况，当碰撞发生时，这时会取出bucketIndex桶内已存储的元素（如果该桶next引用不空，即有了链表也要遍历链表），并通过`hashCode()`和`equals()`来逐个比较以判断Key是否已存在。<br />如果已存在，则使用新Value值替换旧Value值，并返回旧Value值；如果不存在，则存放新的键值对<Key, Value>到链表中。因此，在HashMap中，equals()方法只有在哈希码碰撞时才会被用到。<br />结合源码来看HashMap的put操作：
```java
public V put(K key, V value) {
    return putVal(hash(key), key, value, false, true);
}

final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
               boolean evict) {
    Node<K,V>[] tab; Node<K,V> p; int n, i;
    //第一次put元素时，table数组为空，先调用resize生成一个指定容量的数组
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length;
    //hash值和n-1的与运算结果为桶的位置，如果该位置空就直接放置一个Node
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    //如果计算出的bucket不空，即发生哈希冲突，就要进一步判断
    else {
        Node<K,V> e; K k;
        //判断当前Node的key与要put的key是否相等
        if (p.hash == hash &&
            ((k = p.key) == key || (key != null && key.equals(k))))
            e = p;
        //判断当前Node是否是红黑树的节点
        else if (p instanceof TreeNode)
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        //以上都不是，说明要new一个Node，加入到链表中
        else {
            for (int binCount = 0; ; ++binCount) {
             //在链表尾部插入新节点，注意jdk1.8是在链表尾部插入新节点
                if ((e = p.next) == null) {
                    p.next = newNode(hash, key, value, null);
                    // 如果当前链表中的元素大于树化的阈值，进行链表转树的操作
                    if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                        treeifyBin(tab, hash);
                    break;
                }
                //在链表中继续判断是否已经存在完全相同的key
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    break;
                p = e;
            }
        }
        //走到这里，说明本次put是更新一个已存在的键值对的value
        if (e != null) { // existing mapping for key
            V oldValue = e.value;
            if (!onlyIfAbsent || oldValue == null)
                e.value = value;
            //在hashMap中，afterNodeAccess方法体为空，交给子类去实现
            afterNodeAccess(e);
            return oldValue;
        }
    }
    ++modCount;
    //如果当前size超过临界值，就扩容。注意是先插入节点再扩容
    if (++size > threshold)
        resize();
    //在hashMap中，afterNodeInsertion方法体为空，交给子类去实现
    afterNodeInsertion(evict);
    return null;
}
```
通过上述源码可以清楚了解到HashMap保存数据的过程。先计算key的hash值，然后根据hash值搜索在table数组中的索引位置，如果table数组在该位置处有元素，则查找是否存在相同的key，若存在则覆盖原来key的value，**否则将该元素保存在链表尾部，注意JDK1.7中采用的是头插法，即每次都将冲突的键值对放置在链表头，这样最初的那个键值对最终就会成为链尾，而JDK1.8中使用的是尾插法。** 此外，若table在该处没有元素，则直接保存。<br />对于hash函数，具体的来看下源码
```java
static final int hash(Object key) {
    int h;
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
}
```
以上可以看到`key==null`时，直接返回0，所以HashMap中键为NULL的键值对，一定在第一个桶中。<br />`h >>> 16`是用来取出h的高16位(`>>>`是无符号右移) 如下展示：
```java
0000 0100 1011 0011  1101 1111 1110 0001
 
>>> 16 
 
0000 0000 0000 0000  0000 0100 1011 0011
```
通过之前putVal的源码可以知道，HashMap是用`(length - 1) & hash`来计算数组下标的。绝大多数情况下length一般都小于`2^16`即小于65536。所以`hash & (length-1);`结果始终是hash的低16位与（length-1）进行`&`运算。要是能让hash的高16位也参与运算，会让得到的下标更加散列。<br />如何让高16也参与运算呢。方法就是让`hashCode()`和自己的高16位进行^运算。由于与运单和或运单都会使得结果偏向0或者1，并不是均匀的概念，所以用异或。<br />结合源码来看HashMap的`get`操作：
```java
public V get(Object key) {
    Node<K,V> e;
    return (e = getNode(hash(key), key)) == null ? null : e.value;
}

final Node<K,V> getNode(int hash, Object key) {
    Node<K,V>[] tab; Node<K,V> first, e; int n; K k;
    if ((tab = table) != null && (n = tab.length) > 0 &&
        (first = tab[(n - 1) & hash]) != null) {
        if (first.hash == hash && // always check first node
            ((k = first.key) == key || (key != null && key.equals(k))))
            return first;
        if ((e = first.next) != null) {
         //如果是红黑树，就调用树的查找方法，否则遍历链表直到找到
            if (first instanceof TreeNode)
                return ((TreeNode<K,V>)first).getTreeNode(hash, key);
            do {
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    return e;
            } while ((e = e.next) != null);
        }
    }
    return null;
}
```
在这里能够根据key快速的取到value，除了和HashMap的数据结构密不可分外，还和Node有莫大的关系。在前面就已经提到过，HashMap在存储过程中并没有将key/value分开来存储，而是当做一个整体key-value来处理的，这个整体就是Node对象。
<a name="dppxM"></a>
### 1.4、为什么HashMap的底层数组长度总是2的n次方
当底层数组的length为2的n次方时， `hash & (length - 1)` 就相当于对length取模，其效率要比直接取模高得多，这是HashMap在效率上的一个优化。<br />希望HashMap中的元素存放的越均匀越好。最理想的效果是，Node数组中每个位置都只有一个元素，这样，查询的时候效率最高，不需要遍历单链表，也不需要通过equals去比较Key，而且空间利用率最大。<br />那如何计算才会分布最均匀呢？正如上一节提到的，HashMap采用了一个分两步走的哈希策略：

- 使用 hash() 方法用于对Key的`hashCode`进行重新计算，以防止质量低下的`hashCode()`函数实现。由于hashMap的支撑数组长度总是2的倍数，通过右移可以使低位的数据尽量的不同，从而使Key的hash值的分布尽量均匀；
- 使用hash & (length - 1) 方法进行取余运算，以使每个键值对的插入位置尽量分布均匀；

由于length是2的整数幂，length-1的低位就全是1，高位全部是0。在与hash值进行低位&运算时，低位的值总是与原来hash值相同，高位&运算时值为0。这就保证了不同的hash值发生碰撞的概率比较小，这样就会使得数据在table数组中分布较均匀，查询速度也较快。
<a name="m18eI"></a>
### 1.5、HashMap的扩容
随着HashMap中元素的数量越来越多，发生碰撞的概率将越来越大，所产生的子链长度就会越来越长，这样势必会影响HashMap的存取速度。为了保证HashMap的效率，**系统必须要在某个临界点进行扩容处理，该临界点就是HashMap中元素的数量在数值上等于threshold（table数组长度*加载因子）。** 但是，不得不说，扩容是一个非常耗时的过程，因为它需要重新计算这些元素在新table数组中的位置并进行复制处理。<br />首先回答一个问题，在插入一个临界节点时，HashMap是先扩容后插入还是先插入后扩容？这样选取的优势是什么？<br />答案是：**先插入后扩容。通过查看putVal方法的源码可以发现是先执行完新节点的插入后，然后再做size是否大于threshold的判断的。**
```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
               boolean evict) {
  ...
    //如果插入新结点后的size超过了临界值，就扩容，注意是先插入节点再扩容
    if (++size > threshold)
        resize();
    //在hashMap中，afterNodeInsertion方法体为空，交给子类去实现
    afterNodeInsertion(evict);
    return null;
}
```
为什么是先插入后扩容？源码已经很清楚的表达了扩容原因，调用put不一定是新增数据，还可能是覆盖掉原来的数据，这里就存在一个key的比较问题。<br />以先扩容为例，**先比较是否是新增的数据，再判断增加数据后是否要扩容，这样比较会浪费时间，而先插入后扩容，就有可能在中途直接通过return返回了（本次put是覆盖操作，size不变不需要扩容），这样可以提高效率的。**<br />JDK1.8相对于JDK1.7对HashMap的实现有较大改进，做了很多优化，链表转红黑树是其中的一项，其实扩容方法JDK1.8也有优化，与JDK1.7有较大差别。<br />JDK1.8中resize方法源码如下：
```java
final Node<K,V>[] resize() {
    Node<K,V>[] oldTab = table;
    int oldCap = (oldTab == null) ? 0 : oldTab.length;
    int oldThr = threshold;
    int newCap, newThr = 0;
    if (oldCap > 0) {
     // 原来的容量就已经超过最大值就不再扩容了，就只好随你碰撞去吧
        if (oldCap >= MAXIMUM_CAPACITY) {
            threshold = Integer.MAX_VALUE;
            return oldTab;
        }
        // 没超过最大值，就扩容为原来的2倍
        else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                 oldCap >= DEFAULT_INITIAL_CAPACITY)
            newThr = oldThr << 1; // double threshold
    }
    else if (oldThr > 0) // initial capacity was placed in threshold
        newCap = oldThr;
    else {               // zero initial threshold signifies using defaults
        newCap = DEFAULT_INITIAL_CAPACITY;
        newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
    }
    // 计算新的resize上限，即新的threshold值
    if (newThr == 0) {
        float ft = (float)newCap * loadFactor;
        newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                  (int)ft : Integer.MAX_VALUE);
    }
    threshold = newThr;
    @SuppressWarnings({"rawtypes","unchecked"})
        Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
    table = newTab;
    if (oldTab != null) {
     // 把旧的bucket都移动到新的buckets中
        for (int j = 0; j < oldCap; ++j) {
            Node<K,V> e;
            if ((e = oldTab[j]) != null) {
                oldTab[j] = null;
                if (e.next == null)
                    newTab[e.hash & (newCap - 1)] = e;
                else if (e instanceof TreeNode)
                    ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
                else { // preserve order
                    Node<K,V> loHead = null, loTail = null;
                    Node<K,V> hiHead = null, hiTail = null;
                    Node<K,V> next;
                    do {
                        next = e.next;
                        //原来的桶索引值
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
                    // 扩容后，键值对在新table数组中的位置与旧数组中一样
                    if (loTail != null) {
                        loTail.next = null;
                        newTab[j] = loHead;
                    }
                    // 扩容后，键值对在新table数组中的位置与旧数组中不一样
                    // 新的位置=原来的位置 + oldCap
                    if (hiTail != null) {
                        hiTail.next = null;
                        newTab[j + oldCap] = hiHead;
                    }
                }
            }
        }
    }
    return newTab;
}
```
必要的位置已经加了注释。最让人疑惑的有两个点：

- 为什么`(e.hash & oldCap)== 0`时就放入lo链表，否则就是hi链表;
- 为什么 `j + oldCap`就是键值对在新的table数组中的位置；

其实这里包含着一些数学技巧。类似于上一小节为什么HashMap中数组的长度总是取2的整数次幂。<br />查看源码，发现扩容时，使用的是2次幂的扩展即长度扩为原来2倍，所以，元素的位置要么是在原位置，要么是在原位置再移动2次幂的位置。<br />看下图可以明白这句话的意思，n为table的长度，图中上半部分表示扩容前的key1和key2两个Node的索引位置，图中下半部分表示扩容后key1和key2两个Node新的索引位置。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633532164045-3e95690d-a72a-4076-8754-e5280de8a6b1.webp#clientId=ua9e5b3f4-4d34-4&from=paste&id=u0f02c40d&originHeight=296&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u3a65efca-406d-4477-a9b3-f365b283cc8)<br />元素在重新计算hash之后，因为n变为2倍，那么n-1在高位多1bit，因此新的index就会发生这样的变化：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633532164018-705c8d2e-66c7-41bd-8b39-e230806f58e7.webp#clientId=ua9e5b3f4-4d34-4&from=paste&id=u5bdad52d&originHeight=118&originWidth=589&originalType=url&ratio=1&status=done&style=shadow&taskId=u0319988d-a981-4232-8d81-1f0c5cd365d)<br />因此，在扩充HashMap的时候，不需要像JDK1.7的实现那样重新hash，只需要看看原来的hash值新增的那个bit是1还是0就好了，是0的话索引没变，是1的话索引变成“原索引+oldCap”，这个设计确实非常的巧妙，既省去了重新hash值的时间，而且同时，由于新增的1bit是0还是1可以认为是随机的，因此resize的过程，均匀的把之前的冲突的节点分散到新的bucket了，这一块就是JDK1.8新增的优化点。
<a name="wFoSw"></a>
### 1.6、HashMap为什么引入红黑树而不是AVL树
上面这个问题也可以理解为：有了二叉查找树、平衡树（AVL）为啥还需要红黑树？<br />二叉查找树，也称有序二叉树（ordered binary tree），或已排序二叉树（sorted binary tree），是指一棵空树或者具有下列性质的二叉树：

- 若任意节点的左子树不空，则左子树上所有结点的值均小于它的根结点的值；
- 若任意节点的右子树不空，则右子树上所有结点的值均大于它的根结点的值；
- 任意节点的左、右子树也分别为二叉查找树;
- 没有键值相等的节点（no duplicate nodes）

因为一棵由N个结点随机构造的二叉查找树的高度为logN，所以顺理成章，二叉查找树的一般操作的执行时间为O(logN)。但二叉查找树若退化成了一棵具有N个结点的线性链后，则这些操作最坏情况运行时间为O(N)。<br />可想而知，不能让这种情况发生，为了解决这个问题，于是引申出了平衡二叉树，即AVL树，它对二叉查找树做了改进，在每插入一个节点的时候，必须保证每个节点对应的左子树和右子树的树高度差不超过1。如果超过了就对其进行左旋或右旋，使之平衡。<br />虽然平衡树解决了二叉查找树退化为近似链表的缺点，能够把查找时间控制在 O(logN)，不过却不是最佳的，因为平衡树要求每个节点的左子树和右子树的高度差至多等于1，这个要求实在是太严了，导致每次进行插入/删除节点的时候，几乎都会破坏平衡树的规则，进而都需要通过左旋和右旋来进行调整，使之再次成为一颗符合要求的平衡树。<br />显然，如果在那种插入、删除很频繁的场景中，平衡树需要频繁着进行调整，这会使平衡树的性能大打折扣，为了解决这个问题，于是有了红黑树。**红黑树是不符合AVL树的平衡条件的，即每个节点的左子树和右子树的高度最多差1的二叉查找树，但是提出了为节点增加颜色，红黑树是用非严格的平衡来换取增删节点时候旋转次数的降低，任何不平衡都会在三次旋转之内解决，相较于AVL树为了维持平衡的开销要小得多。**<br />AVL树是严格平衡树，因此在增加或者删除节点的时候，根据不同情况，旋转的次数比红黑树要多，**所以红黑树的插入效率相对于AVL树更高。单单在查找方面比较效率的话，由于AVL高度平衡，因此AVL树的查找效率比红黑树更高。**<br />对主要的几种平衡树作个比较，发现红黑树有着良好的稳定性和完整的功能，性能表现也很不错，综合实力强，在诸如STL的场景中需要稳定表现。实际应用中，若搜索的频率远远大于插入和删除，那么选择AVL，如果发生搜索和插入删除操作的频率差不多，应该选择红黑树。
<a name="jo5cS"></a>
### 1.7、HashMap的线程不安全
所有人都知道HashMap是线程不安全的，应该使用ConcurrentHashMap。但是为什么HashMap是线程不安全的呢？<br />首先需要强调一点，**HashMap的线程不安全体现在会造成死循环、数据丢失、数据覆盖这些问题。其中死循环和数据丢失是在JDK1.7中出现的问题，在JDK1.8中已经得到解决，然而1.8中仍会有数据覆盖的问题，即在并发执行HashMap的put操作时会发生数据覆盖的情况。**<br />首先扩容会造成HashMap的线程不安全，根源就在JDK1.7的transfer函数中。transfer方法将原有Entry数组的元素拷贝到新的Entry数组里。JDK1.7中HashMap的transfer函数源码如下：
```java
void transfer(Entry[] newTable) {
    //src引用了旧的Entry数组
    Entry[] src = table; 
    int newCapacity = newTable.length;
    //遍历旧的Entry数组
    for (int j = 0; j < src.length; j++) {
        //取得旧Entry数组的每个元素
        Entry<K,V> e = src[j]; 
        if (e != null) {
            src[j] = null;
            //释放旧Entry数组的对象引用（for循环后，旧的Entry数组不再引用任何对象）
            do {
                Entry<K,V> next = e.next;
                int i = indexFor(e.hash, newCapacity); 
                //重新计算每个元素在数组中的位置
                e.next = newTable[i]; //标记[1]
                newTable[i] = e; //将元素放在数组上
                e = next; 
                //访问下一个Entry链上的元素
            } while (e != null);
        }
    }
}
```
这段代码是HashMap的扩容操作，重新定位每个桶的下标，并采用头插法将元素迁移到新数组中。**头插法会将链表的顺序翻转，这也是在多线程环境下会形成死循环的关键点。** 扩容造成死循环和数据丢失的详细过程这里不再赘述，可以搜索很多分析这个过程的文章。<br />JDK1.8的源码中已经没有transfer函数，因为JDK1.8直接在resize函数中完成了数据迁移。此外JDK1.8在进行元素插入时使用的是尾插法。为什么多线程环境下JDK1.8的HashMap会出现数据覆盖的情况呢，来看一下JDK1.8中的putVal源码：
```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
               boolean evict) {
    Node<K,V>[] tab; Node<K,V> p; int n, i;
    //第一次put元素时，table数组为空，先调用resize生成一个指定容量的数组
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length;
    //hash值和n-1的与运算结果为桶的位置，如果该位置空就直接放置一个Node
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    //如果计算出的bucket不空，即发生哈希冲突，就要进一下判断
    else {
        Node<K,V> e; K k;
        //判断当前Node的key与要put的key是否相等
        if (p.hash == hash &&
            ((k = p.key) == key || (key != null && key.equals(k))))
            e = p;
        //判断当前Node是否是红黑树的节点
        else if (p instanceof TreeNode)
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        //以上都不是，说明要new一个Node，加入到链表中
        else {
            for (int binCount = 0; ; ++binCount) {
             //进入这个if说明是到达链表尾部
                if ((e = p.next) == null) {
                    p.next = newNode(hash, key, value, null);
                    if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                        treeifyBin(tab, hash);
                    break;
                }
                //在链表中继续判断是否已经存在完全相同的key
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    break;
                p = e;
            }
        }
        //走到这里，说明本次put是更新一个已存在的键值对的value
        if (e != null) { // existing mapping for key
            V oldValue = e.value;
            if (!onlyIfAbsent || oldValue == null)
                e.value = value;
            //在hashMap中，afterNodeAccess方法体为空，交给子类去实现
            afterNodeAccess(e);
            return oldValue;
        }
    }
    //下面两个自增操作都不是原子的
    ++modCount;
    if (++size > threshold)
        resize();
    //在hashMap中，afterNodeInsertion方法体为空，交给子类去实现
    afterNodeInsertion(evict);
    return null;
}
```
其中`if((p = tab[i = (n - 1) & hash]) == null)`是判断是否出现hash碰撞，假设两个线程A、B都在进行put操作，并且hash函数计算出的插入下标是相同的，当线程A执行完这行代码后由于时间片耗尽导致被挂起，而线程B得到时间片后在该下标处插入了元素，完成了正常的插入，然后线程A获得时间片，由于之前已经进行了hash碰撞的判断，所以此时不会再进行判断，而是直接进行插入，这就导致了线程B插入的数据被线程A覆盖了，从而线程不安全。<br />除此之外，还有就是代码的末尾部分有个++size，这样想，还是线程A、B，这两个线程同时进行put操作时，假设当前HashMap的size大小为10，当线程A执行到size自增这行代码时，从主内存中获得size的值为10后准备进行+1操作，但是由于时间片耗尽只好让出CPU，线程B拿到CPU还是从主内存中拿到size的值10进行+1操作，完成了put操作并将size=11写回主内存，由于size不是volatile修改的变量，然后线程A再次拿到CPU后不会再从主内存中加载一次size的值，而是使用自己工作内存中的副本，继续执行加1，当执行完put操作后，还是将size=11写回主内存，此时，线程A、B都执行了一次put操作，但是size的值只增加了1，所有说还是由于数据覆盖又导致了线程不安全。
<a name="QTS71"></a>
## 2、LinkedHashMap概述
HashMap是Java Collection Framework的重要成员，也是Map族中最为常用的一种。不过遗憾的是，HashMap是无序的，也就是说，迭代HashMap所得到的元素顺序并不是它们最初放置到HashMap的顺序。HashMap的这一缺点往往会造成诸多不便，因为在有些场景中，的确需要用到一个可以保持插入顺序的Map。<br />庆幸的是，JDK为解决了这个问题，它为HashMap提供了一个子类 —— LinkedHashMap。虽然LinkedHashMap增加了时间和空间上的开销，但是它通过维护一个额外的双向链表保证了迭代顺序==。特别地，==该迭代顺序可以是插入顺序，也可以是访问顺序。<br />因此，根据链表中元素的顺序可以将LinkedHashMap分为：保持插入顺序的LinkedHashMap和保持访问顺序的LinkedHashMap，其中LinkedHashMap的默认实现是按插入顺序排序的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633532163872-0ff6e914-b414-434c-8980-4f0b8387f9d2.webp#clientId=ua9e5b3f4-4d34-4&from=paste&id=u2d5e13e0&originHeight=1072&originWidth=924&originalType=url&ratio=1&status=done&style=none&taskId=ub9151a18-0107-402a-979e-e6cc1bfe745)<br />更直观地，下图很好地还原了LinkedHashMap的原貌：HashMap和双向链表的密切配合和分工合作造就了LinkedHashMap。特别需要注意的是，next用于维护HashMap各个桶中的Entry链，before、after用于维护LinkedHashMap的双向链表，虽然它们的作用对象都是Entry，但是各自分离，是两码事儿。<br />![2021-10-06-23-14-41-470931.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633533296069-06cd3a8e-85e8-4781-8080-0ab0b8a3e2ed.png#clientId=ua9e5b3f4-4d34-4&from=ui&id=u71849cf7&originHeight=888&originWidth=1080&originalType=binary&ratio=1&size=281000&status=done&style=shadow&taskId=u8de56e7b-186e-4ac6-bdb7-031b2811ed2)<br />特别地，由于LinkedHashMap是HashMap的子类，所以`LinkedHashMap`自然会拥有HashMap的所有特性。比如，LinkedHashMap也最多只允许一条Entry的键为Null(多条会覆盖)，但允许多条Entry的值为Null。<br />此外，LinkedHashMap 也是 Map 的一个非同步的实现。此外，LinkedHashMap还可以用来实现LRU (Least recently used, 最近最少使用)算法。
<a name="woO2y"></a>
### 2.1、LinkedHashMap定义及构造函数
本质上，HashMap和双向链表合二为一即是LinkedHashMap。JDK1.8中LinkedHashMap的定义源码如下：
```java
public class LinkedHashMap<K,V>
    extends HashMap<K,V>
    implements Map<K,V>
{
 /**
  * HashMap.Node subclass for normal LinkedHashMap entries.
  */
 static class Entry<K,V> extends HashMap.Node<K,V> {
  //再加两个引用，分别指向前一个插入的Entry与后一个插入的Entry
     Entry<K,V> before, after;
     Entry(int hash, K key, V value, Node<K,V> next) {
         super(hash, key, value, next);
     }
 }

 /**
  * The head (eldest) of the doubly linked list.
  * 头节点引用
  */
 transient LinkedHashMap.Entry<K,V> head;

 /**
  * The tail (youngest) of the doubly linked list.
  * 尾节点引用
  */
 transient LinkedHashMap.Entry<K,V> tail;

 /**
  * The iteration ordering method for this linked hash map: <tt>true</tt>
  * for access-order, <tt>false</tt> for insertion-order.
  * true表示按照访问顺序迭代，false时表示按照插入顺序 
  * @serial
  */
 final boolean accessOrder;

 ... 
}
```
`LinkedHashMap`采用的hash算法和HashMap相同，但是它重新定义了`Entry`。`LinkedHashMap`中的`Entry`继承了`HashMap.Node`，但增加了两个指针before 和 after，它们分别用于维护双向链接列表。**特别需要注意的是，**`**next**`**用于维护HashMap各个Node的连接顺序，before、after用于维护Entry插入的先后顺序。**<br />`LinkedHashMap`的5大构造函数都是在HashMap的构造函数的基础上实现的，分别如下：
```java
public LinkedHashMap(int initialCapacity, float loadFactor) {
    super(initialCapacity, loadFactor);
    accessOrder = false;
}


public LinkedHashMap(int initialCapacity) {
    super(initialCapacity);
    accessOrder = false;
}


public LinkedHashMap() {
    super();
    accessOrder = false;
}


public LinkedHashMap(Map<? extends K, ? extends V> m) {
    super();
    accessOrder = false;
    putMapEntries(m, false);
}

public LinkedHashMap(int initialCapacity,
                     float loadFactor,
                     boolean accessOrder) {
    super(initialCapacity, loadFactor);
    this.accessOrder = accessOrder;
}
```
<a name="Ckfco"></a>
### 2.2、LinkedHashMap的快速存取
在HashMap中最常用的两个操作就是：`put(Key,Value)` 和 `get(Key)`。同样地，在 `LinkedHashMap` 中最常用的也是这两个操作。对于`put(Key,Value)`方法而言，`LinkedHashMap`完全继承了HashMap的 `put(Key,Value)` 方法，只是对`putVal(hash,key, value, onlyIfAbsent,evict)`方法所调用的`afterNodeAccess`方法和`afterNodeInsertion`方法进行了重写；对于get(Key)方法，`LinkedHashMap`则直接对它进行了重写。<br />下面结合JDK源码看 LinkedHashMap 的存取实现。<br />`HashMap`的`putVal`源码，上一节中已经分析过，直接来看`LinkedHashMap`对`afterNodeAccess`和`afterNodeInsertion`方法的实现：
```java
void afterNodeInsertion(boolean evict) { // possibly remove eldest
    LinkedHashMap.Entry<K,V> first;
    if (evict && (first = head) != null && removeEldestEntry(first)) {
        K key = first.key;
        removeNode(hash(key), key, null, false, true);
    }
}
 
/**
 * 如果map应该删除最老的节点，返回true
 * 这个方法在被put和putAll方法被调用，当向map中插入一个新的entry时被执行。这个方法提供了当一个新的entry被添加到linkedHashMap中，删除最老节点的机会。
 * 
 * 这个方法是很有用的，可以通过删除最老节点来减少内存消耗，避免溢出。
 * 
 * 简单的例子：这个方法的重写将map的最大值设为100，到100时，每次增一个entry，就删除一次最老节点。
 * 
 *     private static final int MAX_ENTRIES = 100;
 *
 *     protected boolean removeEldestEntry(Map.Entry eldest) {
 *        return size() > MAX_ENTRIES;
 *     }
 *
 * 这个方法一般不会直接修改map，而是通过返回true或者false来控制是否修改map。
 *
 *
 * @param    eldest 最老的节点（即头节点）
 * @return   如果map应该删除头节点就返回true，否则返回false
 */
protected boolean removeEldestEntry(Map.Entry<K,V> eldest) {
    return false;
}
```
以上`afterNodeInsertion`方法由于`removeEldestEntry`方法默认一直返回的false而无执行意义。也就意味着如果想要让它有意义必须重写`removeEldestEntry`方法。
```java
void afterNodeAccess(Node<K,V> e) { // move node to last
    LinkedHashMap.Entry<K,V> last;
    if (accessOrder && (last = tail) != e) {
        LinkedHashMap.Entry<K,V> p =
            (LinkedHashMap.Entry<K,V>)e, b = p.before, a = p.after;
        p.after = null;
        if (b == null)
            head = a;
        else
            b.after = a;
        if (a != null)
            a.before = b;
        else
            last = b;
        if (last == null)
            head = p;
        else {
            p.before = last;
            last.after = p;
        }
        tail = p;
        ++modCount;
    }
}
```
可见仅有`accessOrder`为`true`时，且访问节点不等于尾节点时，该方法才有意义。通过`before`/`after`重定向，将新访问节点链接为链表尾节点。<br />`LinkedHashMap`的get操作：
```java
public V get(Object key) {
    Node<K,V> e;
    if ((e = getNode(hash(key), key)) == null)
        return null;
    //当accessOrder为true时，才会出现个性化逻辑
    if (accessOrder)
        afterNodeAccess(e);
    return e.value;
}


//以下是HashMap中的getNode方法
final Node<K,V> getNode(int hash, Object key) {
    Node<K,V>[] tab; Node<K,V> first, e; int n; K k;
    if ((tab = table) != null && (n = tab.length) > 0 &&
        (first = tab[(n - 1) & hash]) != null) {
        if (first.hash == hash && // always check first node
            ((k = first.key) == key || (key != null && key.equals(k))))
            return first;
        if ((e = first.next) != null) {
            if (first instanceof TreeNode)
                return ((TreeNode<K,V>)first).getTreeNode(hash, key);
            do {
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    return e;
            } while ((e = e.next) != null);
        }
    }
    return null;
}
```
在`LinkedHashMap`的get方法中，通过HashMap中的`getNode`方法获取Node对象。<br />注意这里的`afterNodeAccess`方法，如果链表中元素的排序规则是按照插入的先后顺序排序的话，该方法什么也不做；如果链表中元素的排序规则是按照访问的先后顺序排序的话，则将e移到链表的末尾处。
<a name="Q6fBL"></a>
### 2.3、LinkedHashMap与LRU算法
到此为止，已经分析完了`LinkedHashMap`的存取实现，这与`HashMap`大体相同。`LinkedHashMap`区别于HashMap最大的一个不同点是，前者是有序的，而后者是无序的。为此，`LinkedHashMap`增加了两个属性用于保证顺序，分别是双向链表头结点header和标志位`accessOrder`。`header`是`LinkedHashMap`所维护的双向链表的头结点，而`accessOrder`用于决定具体的迭代顺序。<br />当`accessOrder`标志位为`true`时，表示双向链表中的元素按照访问的先后顺序排列，可以看到，虽然`Entry`插入链表的顺序依然是按照其`put`到`LinkedHashMap`中的顺序，但`put`和`get`方法均有判断`accessOrder`的值。<br />如果`accessOrder`为`true`，put时将新插入的元素放入到双向链表的尾部，`get`时将当前访问的Entry移到双向链表的尾部。当标志位`accessOrder`的值为`false`时，表示双向链表中的元素按照`Entry`插入`LinkedHashMap`到中的先后顺序排序，即每次`put`到`LinkedHashMap`中的`Entry`都放在双向链表的尾部，这样遍历双向链表时，`Entry`的输出顺序便和插入的顺序一致，这也是默认的双向链表的存储顺序。<br />测试代码
```java
@Test
public void testLinkedHashMap() {
    Map<String, String> map = new HashMap<>(128);
    System.out.println("------HashMap------");
    map.put("first", "a");
    map.put("second", "b");
    map.put("third", "c");
    map.put("fourth", "d");
    map.put("fifth", "e");
    map.put("sixth", "f");
    map.forEach((key,value) -> {
        System.out.println("key=" + key + ",value=" + value);
    });

    map.clear();
    System.out.println("------LinkedHashMap------");
    map = new LinkedHashMap<>(128);
    map.put("first", "a");
    map.put("second", "b");
    map.put("third", "c");
    map.put("fourth", "d");
    map.put("fifth", "e");
    map.put("sixth", "f");

    map.forEach((key,value) -> {
        System.out.println("key=" + key + ",value=" + value);
    });
}
```
运行结果如下，`HashMap`不保证有序而`LinkedHashMap`默认按迭代顺序和插入的顺序一致。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633532164765-467bf5b9-3b0a-46f6-9bba-453cf283a366.webp#clientId=ua9e5b3f4-4d34-4&from=paste&id=u00336c44&originHeight=546&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u9eb96f71-a382-45a6-bba4-6e052fe3989)<br />前面介绍的`LinkedHashMap`的五种构造方法，前四个构造方法都将`accessOrder`设为`false`，说明默认是按照插入顺序排序的；而第五个构造方法可以自定义传入的`accessOrder`的值。当要用`LinkedHashMap`实现LRU算法时，就需要调用该构造方法并将`accessOrder`置为`true`。<br />使用`LinkedHashMap`实现LRU的必要前提是将`accessOrder`标志位设为true以便开启按访问顺序排序的模式。可以看到，无论是`put`方法还是`get`方法，都会导致目标`Entry`成为最近访问的`Entry`，因此就把该`Entry`加入到了双向链表的末尾。这样，便把最近使用的Entry放入到了双向链表的后面。多次操作后，双向链表前面的`Entry`便是最近没有使用的，这样当节点个数满的时候，删除最前面的`Entry`即可，因为它就是最近最少使用的`Entry`。
```java
public class SomeTest {
    @Test
    public void testLru() {
        LRU<Character, Integer> lru = new LRU<>(8);
        String s = "abcdefghijkl";
        for (int i = 0; i < s.length(); i++) {
            lru.put(s.charAt(i), i + 1);
        }
        System.out.println("LRU的大小： " + lru.size());
        System.out.println(lru);
        System.out.println("LRU的中key为h的value值： " + lru.get('h'));
        System.out.println(lru);
        lru.put('z', 26);
        System.out.println(lru);
    }

    public static class LRU<K, V> extends LinkedHashMap<K, V> {

        private int cacheSize;

        public LRU(int cacheSize) {
            super(cacheSize, 0.75f, true);
            this.cacheSize = cacheSize;
        }

        /**
         * 重写LinkedHashMap中的removeEldestEntry方法，当LRU中元素多余cacheSize个时，删除最老的节点（即最不经常使用的元素）
         * @param eldest
         * @return
         */
        protected boolean removeEldestEntry(Map.Entry<K,V> eldest) {
            return size() > getCacheSize();
        }

        public int getCacheSize() {
            return cacheSize;
        }

    }
}
```
运行结果：
```java
LRU的大小：8
{e=5, f=6, g=7, h=8, i=9, j=10, k=11, l=12}
LRU的中key为h的value值：8
{e=5, f=6, g=7, i=9, j=10, k=11, l=12, h=8}
{f=6, g=7, i=9, j=10, k=11, l=12, h=8, z=26}
```
