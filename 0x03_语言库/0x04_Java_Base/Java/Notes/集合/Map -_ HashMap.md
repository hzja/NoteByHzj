面试题JavaMap
<a name="XXd22"></a>
## 一、 Map
<a name="nND62"></a>
### 1.1 Map 接口
在 Java 中。Map 提供了键——值的映射关系。映射不能包含重复的键，并且每个键只能映射到一个值。<br />以 Map 键——值映射为基础，java.util 提供了 HashMap（最常用）、 TreeMap、Hashtble、LinkedHashMap 等数据结构。<br />衍生的几种 Map 的主要特点：

- HashMap：最常用的数据结构。键和值之间通过 Hash函数 来实现映射关系。当进行遍历的 key 是无序的
- TreeMap：使用红黑树构建的数据结构，因为红黑树的原理，可以很自然的对 key 进行排序，所以 TreeMap 的 key 遍历时是默认按照自然顺序（升序）排列的。
- LinkedHashMap：保存了插入的顺序。遍历得到的记录是按照插入顺序的。
<a name="ZlQmJ"></a>
### 1.2 Hash 散列函数
Hash （散列函数）是把任意长度的输入通过散列算法变换成固定长度的输出。Hash 函数的返回值也称为 哈希值 哈希码 摘要或哈希。Hash作用如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1643160500631-4bfcecea-a94d-4607-bb2f-07752fb54580.jpeg)<br />Hash 函数可以通过选取适当的函数，可以在时间和空间上取得较好平衡。<br />解决 Hash 的两种方式：拉链法和线性探测法
<a name="KxBE8"></a>
### 1.3 键值关系的实现
```java
interface Entry<K,V>
```
在 HashMap 中基于链表的实现
```java
static class Node<K,V> implements Map.Entry<K,V> {
    final int hash;
    final K key;
    V value;
    Node<K,V> next;

    Node(int hash, K key, V value, Node<K,V> next) {
        this.hash = hash;
        this.key = key;
        this.value = value;
        this.next = next;
    }
```
用树的方式实现：
```java
static final class TreeNode<K,V> extends LinkedHashMap.Entry<K,V> {
    TreeNode<K,V> parent;  // red-black tree links
    TreeNode<K,V> left;
    TreeNode<K,V> right;
    TreeNode<K,V> prev;    // needed to unlink next upon deletion
    boolean red;
    TreeNode(int hash, K key, V val, Node<K,V> next) {
        super(hash, key, val, next);
    }
```
<a name="PbODp"></a>
### 1.4 Map 约定的 API
<a name="SiUzt"></a>
#### 1.4.1 Map 中约定的基础 API
基础的增删改查：
```java
int size();  // 返回大小
boolean isEmpty(); // 是否为空
boolean containsKey(Object key); // 是否包含某个键
boolean containsValue(Object value); // 是否包含某个值
V get(Object key); // 获取某个键对应的值
V put(K key, V value); // 存入的数据
V remove(Object key); // 移除某个键
void putAll(Map<? extends K, ? extends V> m); //将将另一个集插入该集合中
void clear();  // 清除
Set<K> keySet(); //获取 Map的所有的键返回为 Set集合
Collection<V> values(); //将所有的值返回为 Collection 集合
Set<Map.Entry<K, V>> entrySet(); // 将键值对映射为 Map.Entry，内部类 Entry 实现了映射关系的实现。并且返回所有键值映射为 Set 集合。
boolean equals(Object o);
int hashCode(); // 返回 Hash 值
default boolean replace(K key, V oldValue, V newValue); // 替代操作
default V replace(K key, V value);
```
<a name="l9UNV"></a>
#### 1.4.2 Map 约定的较为高级的 API
```java
default V getOrDefault(Object key, V defaultValue); //当获取失败时，用 defaultValue 替代。

default void forEach(BiConsumer<? super K, ? super V> action)  // 可用 lambda 表达式进行更快捷的遍历

default void replaceAll(BiFunction<? super K, ? super V, ? extends V> function);

default V putIfAbsent(K key, V value);

default V computeIfAbsent(K key,
            Function<? super K, ? extends V> mappingFunction);

default V computeIfPresent(K key,
            BiFunction<? super K, ? super V, ? extends V> remappingFunction);

default V compute(K key,
            BiFunction<? super K, ? super V, ? extends V> remappingFunction)

default V merge(K key, V value,
            BiFunction<? super V, ? super V, ? extends V> remappingFunction)
```
<a name="VOAJX"></a>
#### 1.4.3 Map 高级 API 的使用

- `getOrDefault()` 当这个通过 key获取值，对应的 key 或者值不存在时返回默认值，避免在使用过程中 null 出现，避免程序异常。
- `ForEach()` 传入 `BiConsumer` 函数式接口，表达的含义其实和 `Consumer` 一样，都拥有 `accept` 方法，只是 `BiConsumer` 多了一个 `andThen()` 方法，接收一个`BiConsumer`接口，先执行本接口的，再执行传入的参数的 `accept` 方法。
```java
Map<String, String> map = new HashMap<>();
map.put("a", "1");
map.put("b", "2");
map.put("c", "3");
map.put("d", "4");
map.forEach((k, v) -> {
    System.out.println(k+"-"+v);
});
```
<a name="LKLPr"></a>
### 1.5 从 Map 走向 HashMap
HashMap 是 Map的一个实现类，也是 Map 最常用的实现类。
<a name="exC6Z"></a>
####  1.5.1 HashMap 的继承关系
```java
public class HashMap<K,V> extends AbstractMap<K,V>
    implements Map<K,V>, Cloneable, Serializable
```
在 HashMap 的实现过程中，解决 Hash冲突的方法是拉链法。因此从原理来说 HashMap 的实现就是 数组 + 链表（数组保存链表的入口）。<br />当链表过长，为了优化查询速率，HashMap 将链表转化为红黑树（数组保存树的根节点），使得查询速率为 log(n)，而不是链表的 O(n)。

<a name="fz0dz"></a>
## 二、HashMap
```java
/*
 * @author  Doug Lea
 * @author  Josh Bloch
 * @author  Arthur van Hoff
 * @author  Neal Gafter
 * @see     Object#hashCode()
 * @see     Collection
 * @see     Map
 * @see     TreeMap
 * @see     Hashtable
 * @since   1.2
 */
```
首先 HashMap 由 Doug Lea 和 Josh Bloch 两位大师的参与。同时 Java 的 Collections 集合体系，并发框架 Doug Lea 也做出了不少贡献。
<a name="owvRv"></a>
### 2.1 基本原理
对于一个插入操作，首先将键通过 Hash 函数转化为数组的下标。若该数组为空，直接创建节点放入数组中。若该数组下标存在节点，即 Hash 冲突，使用拉链法，生成一个链表插入。图片<br />![2022-01-26-09-19-46-406823.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1643160021266-caf2521a-e305-4c1e-8780-b80227fee7d0.png#averageHue=%231bc2c4&clientId=u502a2c9b-0c09-4&from=ui&id=ud4a2a009&originHeight=845&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2743152&status=done&style=shadow&taskId=u6f6e2307-15ba-4df7-bc19-9a709e84a51&title=)<br />如果存在 Hash 冲突，使用拉链法插入，可以在这个链表的头部插入，也可以在链表的尾部插入，所以在 JDK 1.7 中使用了头部插入的方法，JDK 1.8 后续的版本中使用尾插法。<br />JDK 1.7 使用头部插入的可能依据是最近插入的数据是最常用的，但是头插法带来的问题之一，在多线程会链表的复制会出现死循环。所以 JDK 1.8 之后采用的尾部插入的方法。<br />在 HashMap 中，前面说到的 数组+链表 的数组的定义
```java
transient Node<K,V>[] table;
```
链表的定义：
```java
static class Node<K,V> implements Map.Entry<K,V>
```
<a name="HV6eV"></a>
#### 2.1.2 提供的构造函数
```java

public HashMap() { // 空参
    this.loadFactor = DEFAULT_LOAD_FACTOR; // all other fields defaulted
}
public HashMap(int initialCapacity) { //带有初始大小的，一般情况下，需要规划好 HashMap 使用的大小，因为对于一次扩容操作，代价是非常的大的
    this(initialCapacity, DEFAULT_LOAD_FACTOR);
}
public HashMap(int initialCapacity, float loadFactor); // 可以自定义负载因子  
public HashMap(int initialCapacity, float loadFactor); // 可以自定义负载因子
```
三个构造函数，都没有完全的初始化 HashMap，当第一次插入数据时，才进行堆内存的分配，这样提高了代码的响应速度。
<a name="RWLoX"></a>
### 2.2 HashMap 中的 Hash函数定义
```java
static final int hash(Object key) {
    int h;
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16); // 将 h 高 16 位和低 16 位 进行异或操作。
}
// 采用 异或的原因：两个进行位运算，在与或异或中只有异或到的 0 和 1 的概率是相同的，而&和|都会使得结果偏向0或者1。
```
这里可以看到，Map 的键可以为 null，且 hash 是一个特定的值 0。<br />Hash 的目的是获取数组 table 的下标。Hash 函数的目标就是将数据均匀的分布在 table 中。<br />先看看如何通过 hash 值得到对应的数组下标。第一种方法：`hash%table.length()`。但是除法操作在 CPU 中执行比加法、减法、乘法慢的多，效率低下。第二种方法 `table[(table.length - 1) & hash]` 一个与操作一个减法，仍然比除法快。这里的约束条件为  `table.length = 2^N`。
```java
table.length =16;
table.length -1 = 15 1111 1111;
//任何一个数与之与操作，获取到这个数的低 8 位，其他位为 0
```
上面的例子可以获取到对应的下标，而 `(h = key.hashCode()) ^ (h >>> 16)` 让高 16 也参与运算，让数据充分利用，一般情况下 table 的索引不会超过 216，所以高位的信息就直接抛弃了，`^ (h >>> 16)` 在数据量较少的情况下，也可以使用高位的信息。如果 table 的索引超过 216， `hashCode()` 的高 16 为 和 16 个 0 做异或得到的 Hash 也是公平的。
<a name="KJZPV"></a>
### 2.3 HashMap 的插入操作
上面已经知道如果通过 Hash 获取到 对应的 table 下标，因此将对应的节点加入到链表就完成了一个 Map 的映射，的确 JDK1.7 中的 HashMap 实现就是这样。看一看 JDK 为实现现实的 put 操作。定位到 `put()` 操作。
```java
public V put(K key, V value) {
    return putVal(hash(key), key, value, false, true);
}
```
可以看到 put 操作交给了 putVal 来进行通用的实现。
```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent, boolean evict);
//onlyIfAbsent  如果当前位置已存在一个值，是否替换，false是替换，true是不替换
//evict  钩子函数的参数，LinkedHashMap 中使用到，HashMap 中无意义。
```
<a name="htCNW"></a>
#### 2.3.1 `putVal` 的流程分析
其实 `putVal()` 流程的函数非常的明了。这里挑了几个关键步骤来引导。<br />是否第一次插入，true 调用 `resize()` 进行调整，其实此时 `resize()` 是进行完整的初始化，之后直接赋值给对应索引的位置。
```java
if ((tab = table) == null || (n = tab.length) == 0) // 第一次 put 操作， tab 没有分配内存，通过 redize() 方法分配内存，开始工作。
    n = (tab = resize()).length;
if ((p = tab[i = (n - 1) & hash]) == null)
    tab[i] = newNode(hash, key, value, null);
```
如果链表已经转化为树，则使用树的插入。
```java
else if (p instanceof TreeNode)
    e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
```
用遍历的方式遍历每个 Node，如果遇到键相同，或者到达尾节点的next 指针将数据插入，记录节点位置退出循环。若插入后链表长度为 8 则调用 `treeifyBin()` 是否进行树的转化 。
```java
for (int binCount = 0; ; ++binCount) {
    if ((e = p.next) == null) {
        p.next = newNode(hash, key, value, null);
        if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
            treeifyBin(tab, hash);
        break;
    }
    if (e.hash == hash &&
        ((k = e.key) == key || (key != null && key.equals(k))))
        break;
    p = e;
}
```
对键重复的操作：更新后返回旧值，同时还取决于`onlyIfAbsent`，普通操作中一般为 true，可以忽略。
```java
if (e != null) { // existing mapping for key
    V oldValue = e.value;
    if (!onlyIfAbsent || oldValue == null)
        e.value = value;
    afterNodeAccess(e); //钩子函数，进行后续其他操作，HashMap中为空，无任何操作。
    return oldValue;
}
```
后续的数据维护。
<a name="tbvZ3"></a>
#### 2.3.2 modCount 的含义
fail-fast 机制是java集合(Collection)中的一种错误机制。当多个线程对同一个集合的内容进行操作时，就可能会产生fail-fast事件。例如：当某一个线程A通过iterator去遍历某集合的过程中，若该集合的内容被其他线程所改变了；那么线程A访问集合时，就会抛出`ConcurrentModificationException`异常，产生fail-fast事件。一种多线程错误检查的方式，减少异常的发生。<br />一般情况下，多线程环境使用 `ConcurrentHashMap` 来代替 `HashMap`
<a name="Yw7XG"></a>
### 2.4 `resize()` 函数
HashMap 扩容的特点：默认的table 表的大小事 16，threshold 为 12。负载因子 loadFactor .75，这些都是可以构造是更改。以后扩容都是 2 倍的方式增加。<br />至于为何是0.75 代码的注释中也写了原因，对 Hash函数构建了泊松分布模型，进行了分析。
<a name="eDLyl"></a>
#### 2.4.1 HashMap 预定义的一些参数
```java
static final int DEFAULT_INITIAL_CAPACITY = 1 << 4; // aka 16  HashMap 的默认大小。为什么使用 1 <<4
static final int MAXIMUM_CAPACITY = 1 << 30; // 最大容量
static final float DEFAULT_LOAD_FACTOR = 0.75f; // 加载因子，扩容使用

static final int UNTREEIFY_THRESHOLD = 6;//  树结构转化为链表的阈值
static final int TREEIFY_THRESHOLD = 8;  //  链表转化为树结构的阈值
static final int MIN_TREEIFY_CAPACITY = 64; // 链表转变成树之前，还会有一次判断，只有数组长度大于 64 才会发生转换。这是为了避免在哈希表建立初期，多个键值对恰好被放入了同一个链表中而导致不必要的转化。

// 定义的有关变量
int threshold;   // threshold表示当HashMap的size大于threshold时会执行resize操作
```
这些变量都是和 HashMap 的扩容机制有关，将会在下文中用到。
<a name="MO6cf"></a>
#### 2.4.2 `resize()` 方法解析
```java
Node<K,V>[] oldTab = table;
int oldCap = (oldTab == null) ? 0 : oldTab.length;
int oldThr = threshold;
int newCap, newThr = 0; // 定义了 旧表长度、旧表阈值、新表长度、新表阈值

if (oldCap > 0) {  // 插入过数据，参数不是初始化的
    if (oldCap >= MAXIMUM_CAPACITY) {  // 如果旧的表长度大于 1 << 30;
        threshold = Integer.MAX_VALUE; // threshold 设置 Integer 的最大值。也就是说我们可以插入 Integer.MAX_VALUE 个数据
        return oldTab; // 直接返回旧表的长度，因为表的下标索引无法扩大了。
    }
    else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY && //
             oldCap >= DEFAULT_INITIAL_CAPACITY)  //新表的长度为旧表的长度的 2 倍。
        newThr = oldThr << 1; // double threshold 新表的阈值为同时为旧表的两倍
}
else if (oldThr > 0) //   public HashMap(int initialCapacity, float loadFactor)   中的  this.threshold = tableSizeFor(initialCapacity);  给正确的位置
    newCap = oldThr;
else {               // zero initial threshold signifies using defaults ，如果调用了其他两个构造函数，则下面代码初始化。因为他们都没有对其 threshold 设置，默认为 0，
    newCap = DEFAULT_INITIAL_CAPACITY;
    newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
}
if (newThr == 0) { // 修正 threshold，例如上面的   else if (oldThr > 0)  部分就没有设置。
    float ft = (float)newCap * loadFactor;
    newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
              (int)ft : Integer.MAX_VALUE);
}
threshold = newThr;
@SuppressWarnings({"rawtypes","unchecked"})
```
当一些参数设置正确后便开始扩容。
```java
Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
```
当扩容完毕之后，自然就是将原表中的数据搬到新的表中。下面代码完成了该任务。
```java
if (oldTab != null) {
   for (int j = 0; j < oldCap; ++j) {
      ....
   }
}
```
如何正确的，快速的扩容调整每个键值节点对应的下标？第一种方法：遍历节点再使用 `put()` 加入一遍，这种方法实现，但是效率低下。第二种，手动组装好链表，加入到相应的位置。显然第二种比第一种高效，因为第一种 `put()` 还存在其他不属于这种情况的判断，例如重复键的判断等。<br />所以 JDK 1.8 也使用了第二种方法。可以继续使用`e.hash & (newCap - 1)`找到对应的下标位置，对于旧的链表，执行`e.hash & (newCap - 1)` 操作，只能产生两个不同的索引。一个保持原来的索引不变，另一个变为 原来索引 + oldCap(因为 newCap 的加入产生导致索引的位数多了 1 位，即就是最左边的一个，且该位此时结果为 1，所以相当于 原来索引 + oldCap)。所以可以使用 `if ((e.hash & oldCap) == 0)` 来确定出索引是否来变化。<br />因此这样就可以将原来的链表拆分为两个新的链表，然后加入到对应的位置。为了高效，手动的组装好链表再存储到相应的下标位置上。
```java
oldCap  = 16
newCap  = 32
hash       : 0001 1011
oldCap-1   : 0000 1111
结果为     :  0000 1011  对应的索引的 11
-------------------------
e.hash & oldCap 则定于 1,则需要进行调整索引
oldCap  = 16
hash       : 0001 1011
newCap-1   : 0001 1111
结果为     :  0001 1011
相当于 1011 + 1 0000 原来索引 + oldCap
```
```java
for (int j = 0; j < oldCap; ++j)  // 处理每个链表
```
特殊条件处理
```java
Node<K,V> e;
if ((e = oldTab[j]) != null) {
    oldTab[j] = null;
    if (e.next == null)  // 该 链表只有一个节点，那么直接复制到对应的位置，下标由 e.hash & (newCap - 1) 确定
        newTab[e.hash & (newCap - 1)] = e;
    else if (e instanceof TreeNode) // 若是 树，该给树的处理程序
        ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
```
普通情况处理：
```java
else { // preserve order
    Node<K,V> loHead = null, loTail = null;  // 构建原来索引位置 的链表，需要的指针
    Node<K,V> hiHead = null, hiTail = null; // 构建 原来索引 + oldCap 位置 的链表需要的指针
    Node<K,V> next;
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
    } while ((e = next) != null); // 将原来的链表划分两个链表
    if (loTail != null) { // 将链表写入到相应的位置
        loTail.next = null;
        newTab[j] = loHead;
    }
    if (hiTail != null) {
        hiTail.next = null;
        newTab[j + oldCap] = hiHead;
    }
}
```
到此 `resize()` 方法的逻辑完成了。总的来说 `resize()` 完成了 HashMap 完整的初始化，分配内存和后续的扩容维护工作。
<a name="kixDd"></a>
### 2.5 `remove` 解析
```java
public V remove(Object key) {
    Node<K,V> e;
    return (e = removeNode(hash(key), key, null, false, true)) == null ?
        null : e.value;
}
```
将 `remove` 删除工作交给内部函数 `removeNode()` 来实现。
```java
final Node<K,V> removeNode(int hash, Object key, Object value,
                           boolean matchValue, boolean movable) {
    Node<K,V>[] tab; Node<K,V> p; int n, index;
    if ((tab = table) != null && (n = tab.length) > 0 &&
        (p = tab[index = (n - 1) & hash]) != null) {  // 获取索引，
        Node<K,V> node = null, e; K k; V v;
        if (p.hash == hash &&
            ((k = p.key) == key || (key != null && key.equals(k)))) // 判断索引处的值是不是想要的结果
            node = p;
        else if ((e = p.next) != null) { // 交给树的查找算法
            if (p instanceof TreeNode)
                node = ((TreeNode<K,V>)p).getTreeNode(hash, key);
            else {
                do { // 遍历查找
                    if (e.hash == hash &&
                        ((k = e.key) == key ||
                         (key != null && key.equals(k)))) {
                        node = e;
                        break;
                    }
                    p = e;
                } while ((e = e.next) != null);
            }
        }
        if (node != null && (!matchValue || (v = node.value) == value ||
                             (value != null && value.equals(v)))) {
            if (node instanceof TreeNode)  //树的删除
                ((TreeNode<K,V>)node).removeTreeNode(this, tab, movable);
            else if (node == p) // 修复链表，链表的删除操作
                tab[index] = node.next;
            else
                p.next = node.next;
            ++modCount;
            --size;
            afterNodeRemoval(node);
            return node;
        }
    }
    return null;
}
```
<a name="KglML"></a>
## 三、 HashMap 从链表到红黑树的转变
如果链表的长度（冲突的节点数）已经达到8个，此时会调用 `treeifyBin()` ，`treeifyBin()` 首先判断当前hashMap 的 table的长度，如果不足64，只进行resize，扩容table，如果达到64，那么将冲突的存储结构为红黑树。在源码还有这样的一个字段。
```java
static final int UNTREEIFY_THRESHOLD = 6;
// 这样表明了从红黑树转化为链表的阈值为 6，为何同样不是 8 那？
// 如果插入和删除都在 8 附近，将多二者相互转化将浪费大量的时间，对其性能影响。
// 如果是的二者转化的操作不平衡，偏向一方，则可以避免此类影响。
```
<a name="I0SD0"></a>
### 3.1 红黑树的数据结构
```java
static final class TreeNode<K,V> extends LinkedHashMap.Entry<K,V> {
    TreeNode<K,V> parent;  // red-black tree links
    TreeNode<K,V> left;
    TreeNode<K,V> right;
    TreeNode<K,V> prev;    // 删除后需要取消链接，指向前一个节点（原链表中的前一个节点）
    boolean red;
}
```
因为 继承了 `LinkedHashMap.Entry<K,V>` ，所以存储的数据还是在Entry 中：
```java
static class Node<K,V> implements Map.Entry<K,V> {
    final int hash;
    final K key;
    V value;
    Node<K,V> next;

    Node(int hash, K key, V value, Node<K,V> next) {
        this.hash = hash;
        this.key = key;
        this.value = value;
        this.next = next;
    }
}
```
<a name="lwLqe"></a>
### 3.2 承上启下的 `treeifyBin()`
`treeifyBin()` 决定了一个链表何时转化为一个红黑树。`treeifyBin()` 有两种格式：
```java
final void treeifyBin(Node<K,V>[] tab, int hash);
final void treeify(Node<K,V>[] tab);
```
```java
final void treeifyBin(Node<K,V>[] tab, int hash) { // 简单的 Node 修改为 TreeNode，同时维护了 prev 属性。
    int n, index; Node<K,V> e;
    if (tab == null || (n = tab.length) < MIN_TREEIFY_CAPACITY)
        resize();
    else if ((e = tab[index = (n - 1) & hash]) != null) {
        TreeNode<K,V> hd = null, tl = null;
        do {
            TreeNode<K,V> p = replacementTreeNode(e, null);
            if (tl == null)
                hd = p;
            else {
                p.prev = tl;
                tl.next = p;
            }
            tl = p;
        } while ((e = e.next) != null);
        if ((tab[index] = hd) != null)
            hd.treeify(tab);  // 真正生成红黑树的
    }
}
```
```java
TreeNode<K,V> replacementTreeNode(Node<K,V> p, Node<K,V> next) {
    return new TreeNode<>(p.hash, p.key, p.value, next);
} // 实现 Node 链表节点到 TreeNode 节点的转化。
```
下面函数真正实现了链表的红黑树的转变。首先构建一个标准查询二叉树，然后在标准查询二叉树然后调整为一个红黑树。而 `balanceInsertion()` 实现了调整。
```java

/**
         * Forms tree of the nodes linked from this node.
         */
final void treeify(Node<K,V>[] tab) {
    TreeNode<K,V> root = null;
    for (TreeNode<K,V> x = this, next; x != null; x = next) {
        next = (TreeNode<K,V>)x.next;
        x.left = x.right = null;
        if (root == null) { // 第一次转化过程，将链表的头节点作为根节点。
            x.parent = null;
            x.red = false;  // 红黑树的定义 根节点必须为黑色
            root = x;
        }
        else {
            K k = x.key;
            int h = x.hash;
            Class<?> kc = null;
            for (TreeNode<K,V> p = root;;) {
                int dir, ph;
                K pk = p.key;
                if ((ph = p.hash) > h)  //// 通过 Hash 的大小来确定插入顺序
                    dir = -1; // dir 大小顺序的标识
                else if (ph < h)
                    dir = 1;
                else if ((kc == null && //当 两个 Hash 的值相同，进行特殊的方法，确定大小。
                          (kc = comparableClassFor(k)) == null) || // Returns x's Class if it is of the form "class C implements Comparable ", else null. 如果 key类的 源码书写格式为 C implement Comparable<C> 那么返回该类类型 C, 如果间接实现也不行。如果是 String 类型，直接返回 String.class
                         (dir = compareComparables(kc, k, pk)) == 0)   //    ((Comparable)k).compareTo(pk)); 强制转换后进行对比，若 dir == 0,则 tieBreakOrder()，继续仲裁
                    dir = tieBreakOrder(k, pk);  // 首先通过二者的类类型进行比较，如果相等的话，使用 (System.identityHashCode(a) <= System.identityHashCode(b) 使用原始的 hashcode，不是重写的在对比。

                TreeNode<K,V> xp = p; // 遍历的，上一个节点
                if ((p = (dir <= 0) ? p.left : p.right) == null) { //通过 dir，将 p 向下查找，直到 p 为 null，找到一个插入时机
                    x.parent = xp;
                    if (dir <= 0)
                        xp.left = x;
                    else
                        xp.right = x;
                    root = balanceInsertion(root, x); //进行二叉树的调整
                    break;
                }
            }
        }
    }
    moveRootToFront(tab, root);
}
```
<a name="W1fIa"></a>
### 3.3 将一个二叉树转化为红黑树的操作-`balanceInsertion()`
当红黑树中新增节点的时候需要调用`balanceInsertion`方法来保证红黑树的特性。<br />如果想要了解红黑树的插入过程那么必须对红黑树的性质有一个较为清晰的了解。<br />红黑树的性质：

- 每个结点或是红色的，或是黑色的
- 根节点是黑色的
- 每个叶结点（NIL）是黑色的
- 如果一个节点是红色的，则它的两个儿子都是黑色的。
- 对于每个结点，从该结点到其叶子结点构成的所有路径上的黑结点个数相同。
```java
static <K,V> TreeNode<K,V> balanceInsertion(TreeNode<K,V> root,
                                            TreeNode<K,V> x) {
    x.red = true;  // 插入的子节点必须为 red
    for (TreeNode<K,V> xp, xpp, xppl, xppr;;) { //// x 当前处理节点 xp父节点 xpp祖父节点 xppl祖父左节点 xppr 祖父右节点
        if ((xp = x.parent) == null) { // 如果 当前处理节点为根节点，满足红黑树的性质，结束循环
            x.red = false;
            return x;
        }
        else if (!xp.red || (xpp = xp.parent) == null)
            return root;
        if (xp == (xppl = xpp.left)) {
            if ((xppr = xpp.right) != null && xppr.red) {
                xppr.red = false;
                xp.red = false;
                xpp.red = true;
                x = xpp;
            }
            else {
                if (x == xp.right) {
                    root = rotateLeft(root, x = xp);
                    xpp = (xp = x.parent) == null ? null : xp.parent;
                }
                if (xp != null) {
                    xp.red = false;
                    if (xpp != null) {
                        xpp.red = true;
                        root = rotateRight(root, xpp);
                    }
                }
            }
        }
        else {
            if (xppl != null && xppl.red) {
                xppl.red = false;
                xp.red = false;
                xpp.red = true;
                x = xpp;
            }
            else {
                if (x == xp.left) {
                    root = rotateRight(root, x = xp);
                    xpp = (xp = x.parent) == null ? null : xp.parent;
                }
                if (xp != null) {
                    xp.red = false;
                    if (xpp != null) {
                        xpp.red = true;
                        root = rotateLeft(root, xpp);
                    }
                }
            }
        }
    }
}
```
TreeNode 红黑树总结<br />TreeNode 完整的实现了一套红黑树的增删改查的规则。实现参考了《算法导论》
```java
/* ------------------------------------------------------------ */
// Red-black tree methods, all adapted from CLR
```
这里推荐一个红黑树动画演示网站 [https://rbtree.phpisfuture.com/](https://rbtree.phpisfuture.com/) 红黑树是一个不严格的平衡二叉查找树，高度近似 log(N)。
<a name="Zdpho"></a>
## 四、HashMap 的扩展
Map中 key 有一个性质，就是 key 不能重复，而 Java Set 的含义：集合中不能有重复的元素。HashMap 的实现已经足够的优秀。那么是否可以用 key 的性质来实现 Set ？的确 JDK 中的 HashSet 就是这样做的。
```java
public class HashSet<E>
    extends AbstractSet<E>
    implements Set<E>, Cloneable, java.io.Serializable{
    private transient HashMap<E,Object> map;
      // Dummy value to associate with an Object in the backing Map
    private static final Object PRESENT = new Object();
}
```
PRESENT 就是存进 Map 中的 value，而 key 正是 Set 语义的实现。而且可以判断出 HashSet 中是允许存入 Null 值的。
