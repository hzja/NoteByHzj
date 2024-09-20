<a name="y8Mgz"></a>
## 前言
在编程的世界里，有一种神奇的工具，它小巧却强大，灵活而可靠，它是Java中最常用的数据结构之一，它就是HashMap。<br />是否曾经好奇过，这么强大的工具，其背后的实现原理是怎样的呢？<br />一起揭开HashMap源码的神秘面纱，探索这个Java世界中的瑞士军刀。<br />首先需要明白，HashMap并不是一个简单的哈希表，它是一个基于哈希表实现的Map接口的子类。<br />哈希表是一种数据结构，它通过哈希函数将键值对映射到数组的一个位置，从而实现快速查找。<br />而HashMap则在此基础上，增加了一些额外的功能和优化，使得它在处理大量数据时更加高效。
<a name="t0iNk"></a>
## 源码分析
那么，HashMap是如何工作的呢？它的工作原理可以简化为以下几个步骤：
<a name="uXXRI"></a>
### 初始化
初始化：当创建一个HashMap对象时，它会调用构造函数，创建一个新的数组和一个空的Node对象列表。
```java
/**
 * 系列构造方法，推荐在初始化时根据实际情况设置好初始容量，用好了可以显著减少 resize，提升效率
 */
public HashMap(int initialCapacity, float loadFactor) {
    //检查初始容量是否小于0，如果是则抛出异常。
    if (initialCapacity < 0)
        throw new IllegalArgumentException("Illegal initial capacity: " +
                                           initialCapacity);
    //检查初始容量是否大于最大容量，如果是则将初始容量设置为最大容量
    if (initialCapacity > MAXIMUM_CAPACITY)
        initialCapacity = MAXIMUM_CAPACITY;
    //检查负载因子是否小于等于0或者是非数字，如果是则抛出异常。
    if (loadFactor <= 0 || Float.isNaN(loadFactor))
        throw new IllegalArgumentException("Illegal load factor: " +
                                           loadFactor);
    //将传入的负载因子赋值给当前对象的负载因子。
    this.loadFactor = loadFactor;
    //根据初始容量计算阈值并赋值给当前对象的阈值
    this.threshold = tableSizeFor(initialCapacity);
}

//调用带默认负载因子的构造方法，传入初始容量。
public HashMap(int initialCapacity) {
this(initialCapacity, DEFAULT_LOAD_FACTOR);
}
//调用无参构造方法，将默认负载因子赋值给当前对象的负载因子。
public HashMap() {
    this.loadFactor = DEFAULT_LOAD_FACTOR; 
}
//调用带映射参数的构造方法，将传入的映射赋值给当前对象的键值对，并将默认负载因子赋值给当前对象的负载因子。
public HashMap(Map<? extends K, ? extends V> m) {
    this.loadFactor = DEFAULT_LOAD_FACTOR;
    putMapEntries(m, false);
}
```

1. `public HashMap(int initialCapacity, float loadFactor)`：这是一个带有初始容量和负载因子的构造方法。初始容量是HashMap在创建时可以容纳的元素数量，而负载因子是一个浮点数，表示HashMap在扩容之前可以达到的最大填充程度。如果初始容量小于0或者大于最大容量（`MAXIMUM_CAPACITY`），则会抛出异常。如果负载因子小于等于0或者是非数字（NaN），也会抛出异常。
2. `public HashMap(int initialCapacity)`：这是一个只带有初始容量参数的构造方法，它调用了上一个构造方法，并使用默认的负载因子（`DEFAULT_LOAD_FACTOR`）。
3. `public HashMap()`：这是一个无参构造方法，它设置了默认的负载因子（`DEFAULT_LOAD_FACTOR`）和其他字段的默认值。
4. `public HashMap(Map<? extends K, ? extends V> m)`：这是一个带有Map参数的构造方法，它首先设置了默认的负载因子，然后调用了`putMapEntries`方法将传入的Map中的所有键值对放入HashMap中。
<a name="UXlCH"></a>
### 插入
向HashMap中插入一个键值对时，首先会使用键的`hashCode()`方法计算出其在数组中的一个位置，然后检查该位置是否已经有Node对象存在。<br />如果不存在，则创建一个新的Node对象并放入该位置；如果存在，则更新该Node对象的value字段。
```java
public V put(K key, V value) {
    return putVal(hash(key), key, value, false, true);
}

final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
               boolean evict) {
    Node<K,V>[] tab; Node<K,V> p; int n, i;
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length; // 如果哈希表为空或者长度为0，则调用resize()方法进行扩容
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null); // 如果对应位置为空，则创建一个新的节点并放入该位置
    else {
        Node<K,V> e; K k;
        if (p.hash == hash &&
            ((k = p.key) == key || (key != null && key.equals(k))))
            e = p; // 如果当前节点的哈希值和键都匹配，则将当前节点赋值给e
        else if (p instanceof TreeNode)
            e = ((TreeNode<K,V>)p).putTre; // 如果当前节点是TreeNode类型，则调用putTre方法获取要插入的节点
        else {
            for (int binCount = 0; ; ++binCount) {
                if ((e = p.next) == null) {
                    p.next = newNode(hash, key, value, null); // 如果当前节点的下一个节点为空，则创建一个新的节点并放入当前节点的下一个位置
                    if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                        treeifyBin(tab, hash); // 如果当前节点的层数大于等于阈值，则将当前节点转换为树节点
                    break;
                }
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    break; // 如果找到匹配的节点，则跳出循环
                p = e; // 否则继续查找下一个节点
            }
        }
        if (e != null) { // existing mapping for key
            V oldValue = e.value;
            if (!onlyIfAbsent || oldValue == null)
                e.value = value; // 如果键已经存在，且onlyIfAbsent为false或者旧值为null，则更新键对应的值
            afterNodeAccess(e); // 访问节点后的操作
            return oldValue; // 返回旧值
        }
    }
    ++modCount; // 修改计数器加1
    if (++size > threshold)
        resize(); // 如果当前大小大于阈值，则进行扩容操作
    afterNodeInsertion(evict); // 插入节点后的操作
    return null; // 返回null
}

/**
 * 扩容为原容量的两倍，并将存在的元素 放到新的数组上
 */
final Node<K,V>[] resize() {
    Node<K,V>[] oldTab = table; // 保存旧的哈希表数组
    int oldCap = (oldTab == null) ? 0 : oldTab.length; // 获取旧的哈希表容量
    int oldThr = threshold; // 获取旧的阈值
    int newCap, newThr = 0; // 初始化新的容量和阈值
    if (oldCap > 0) { // 如果旧的容量大于0
        if (oldCap >= MAXIMUM_CAPACITY) { // 如果旧的容量已经达到最大值
            threshold = Integer.MAX_VALUE; // 将阈值设置为最大值
            return oldTab; // 返回旧的哈希表数组
        }
        else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                 oldCap >= DEFAULT_INITIAL_CAPACITY) // 如果旧的容量的两倍小于最大容量且大于等于默认初始容量
            newThr = oldThr << 1; // 将阈值的两倍作为新的阈值
    }
    else if (oldThr > 0) // 如果旧的阈值大于0（即初始容量放在阈值中）
        newCap = oldThr; // 新的容量等于旧的阈值
    else {               // 初始阈值为0表示使用默认值
        newCap = DEFAULT_INITIAL_CAPACITY; // 新的容量等于默认初始容量
        newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY); // 新的阈值等于默认初始容量乘以默认负载因子
    }
    if (newThr == 0) { // 如果新的阈值为0
        float ft = (float)newCap * loadFactor; // 计算新的阈值
        newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                  (int)ft : Integer.MAX_VALUE); // 如果新的容量小于最大容量且新的阈值小于最大容量，则新的阈值为新的容量，否则为最大值
    }
    threshold = newThr; // 更新阈值为新的阈值
    @SuppressWarnings({"rawtypes","unchecked"})
    Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap]; // 创建新的哈希表数组
    table = newTab; // 将新的哈希表数组赋值给table
    if (oldTab != null) { // 如果旧的哈希表数组不为空
        for (int j = 0; j < oldCap; ++j) { // 遍历旧的哈希表数组
            Node<K,V> e; // 保存当前节点
            if ((e = oldTab[j]) != null) { // 如果当前节点不为空
                oldTab[j] = null; // 将当前节点设为空
                if (e.next == null) // 如果当前节点的下一个节点为空
                    newTab[e.hash & (newCap - 1)] = e; // 将当前节点放入新的哈希表数组的对应位置
                else if (e instanceof TreeNode) // 如果当前节点是TreeNode类型
                    ((TreeNode<K,V>)e).split(this, newTab, j, oldCap); // 将当前节点分裂成两部分
                else { // 保留顺序
                    Node<K,V> loHead = null, loTail = null; // 定义两个指针
                    Node<K,V> hiHead = null, hiTail = null;
                    Node<K,V> next;
                    do {
                        next = e.next;
                        if ((e.hash & oldCap) == 0) { // 如果当前节点的哈希值与旧的容量的余数为0
                            if (loTail == null) // 如果左指针为空
                                loHead = e; // 将当前节点设为左指针
                            else
                                loTail.next = e; // 否则将当前节点设为左指针的下一个节点
                            loTail = e; // 将当前节点设为左指针的尾节点
                        }
                        else { // 如果当前节点的哈希值与旧的容量的余数不为0
                            if (hiTail == null) // 如果右指针为空
                                hiHead = e; // 将当前节点设为右指针
                            else
                                hiTail.next = e; // 否则将当前节点设为右指针的下一个节点
                            hiTail = e; // 将当前节点设为右指针的尾节点
                        }
                    } while ((e = next) != null);
                    if (loTail != null) { // 如果左指针不为空
                        loTail.next = null; // 将左指针的下一个节点设为空
                        newTab[j] = loHead; // 将左指针的头节点放入新的哈希表数组的对应位置
                    }
                    if (hiTail != null) { // 如果右指针不为空
                        hiTail.next = null; // 将右指针的下一个节点设为空
                        newTab[j + oldCap] = hiHead; // 将右指针的头节点放入新的哈希表数组的对应位置
                    }
                }
            }
        }
    }
    return newTab; // 返回新的哈希表数组
}
```

1. `put(K key, V value)`：向HashMap中添加一个键值对。如果键已经存在，根据`onlyIfAbsent`参数决定是否更新值。如果`evict`参数为true，则在插入新键值对时触发驱逐策略。
2. `putVal(int hash, K key, V value, boolean onlyIfAbsent, boolean evict)`：这是一个重载方法，用于在不触发驱逐策略的情况下更新键值对的值。
3. `resize()`：当HashMap中的元素数量超过阈值时，将HashMap的容量扩大一倍。扩容过程中，会重新计算每个桶的位置，并将原来的元素重新映射到新的桶中。
4. `final Node<K,V>[] resize()`：这是一个静态内部类Node的数组，表示HashMap中的桶。这个函数负责创建新的桶数组，并将原来的元素重新映射到新的桶中。
<a name="vgAjz"></a>
### 查找
当需要查找一个键对应的值时，同样会先计算出键的`hashCode()`值，然后根据该值找到数组中的一个位置。<br />如果该位置的Node对象包含要查找的键，则返回该Node对象的value字段；否则，返回null。
```java
public V get(Object key) {
    Node<K,V> e;
    return (e = getNode(hash(key), key)) == null ? null : e.value;
}

/**
 * 根据 hash 和 key 获取相应的 Node节点
 */
final Node<K,V> getNode(int hash, Object key) {
    Node<K,V>[] tab; Node<K,V> first, e; int n; K k;
    // 1. 定位键值对所在桶的位置，如果该位置有元素，则获取第一个元素
    if ((tab = table) != null && (n = tab.length) > 0 &&
            (first = tab[(n - 1) & hash]) != null) {
        // 如果hash和key都与 第一个元素相同，则第一个元素就是我们要获取的，直接返回
        if (first.hash == hash &&
                ((k = first.key) == key || (key != null && key.equals(k))))
            return first;
        if ((e = first.next) != null) {
            // 2. 如果 first 是 TreeNode 类型，则调用黑红树查找方法
            if (first instanceof TreeNode)
                return ((TreeNode<K,V>)first).getTreeNode(hash, key);
            // 3. 对链表进行查找
            do {
                if (e.hash == hash &&
                        ((k = e.key) == key || (key != null && key.equals(k))))
                    return e;
            } while ((e = e.next) != null);
        }
    }
    return null;
}

/**
 * 还记HashMap底层的动态数组的定义吗 transient Node<K,V>[] table
 * 这里很明显是一个单向链表结构
 */
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

    public final K getKey()        { return key; }
    public final V getValue()      { return value; }
    public final String toString() { return key + "=" + value; }

    public final int hashCode() {
        return Objects.hashCode(key) ^ Objects.hashCode(value);
    }

    public final V setValue(V newValue) {
        V oldValue = value;
        value = newValue;
        return oldValue;
    }

    public final boolean equals(Object o) {
        if (o == this)
            return true;
        if (o instanceof Map.Entry) {
            Map.Entry<?,?> e = (Map.Entry<?,?>)o;
            if (Objects.equals(key, e.getKey()) &&
                    Objects.equals(value, e.getValue()))
                return true;
        }
        return false;
    }
}

/**
 * JDK8 加入的 红黑树TreeNode内部类，红黑树的方法比较复杂，这里只展示一些重要的
 * 属性结构代码
 */
static final class TreeNode<K,V> extends LinkedHashMap.Entry<K,V> {
    // 红黑树链
    TreeNode<K,V> parent;  
    TreeNode<K,V> left;
    TreeNode<K,V> right;
    // 删除后需要取消下一个链接
    TreeNode<K,V> prev;    
    // 颜色，true红，false黑
    boolean red;
    TreeNode(int hash, K key, V val, Node<K,V> next) {
        super(hash, key, val, next);
    }
}
```

1. `public V get(Object key)`：根据给定的键获取对应的值。首先通过调用`getNode(hash(key), key)`方法获取与该键关联的节点，如果节点为空则返回null，否则返回节点的值。
2. `final Node<K,V> getNode(int hash, Object key)`：根据给定的哈希值和键获取对应的节点。首先定位到包含该键值对的桶位置，如果桶中存在元素，则获取第一个元素。如果该元素是TreeNode类型，则调用黑红树查找方法进行查找；否则，在链表中进行查找。如果找到匹配的节点，则返回该节点；否则返回null。
3. `static class Node<K,V>`：表示映射表中的一个节点，实现了`Map.Entry`接口。它包含了键、值、哈希值、下一个节点等信息，并提供了获取键、值、哈希码、设置新值、比较相等等方法。
4. `static final class TreeNode<K,V>`：表示红黑树中的节点，继承自`LinkedHashMap.Entry`类。它包含了父节点、左子节点、右子节点、前一个节点等属性，以及颜色信息。红黑树是一种自平衡的二叉搜索树，这里只展示了部分属性结构代码。
<a name="AEWwi"></a>
### 删除
当需要从HashMap中删除一个键值对时，首先会根据键的`hashCode()`值找到数组中的一个位置，然后检查该位置的Node对象是否包含要删除的键。<br />如果是，则将其从链表中移除；如果不是，则什么都不做。
```java
/**
 * 从该映射中删除指定键的映射（如果存在）。
 */
public V remove(Object key) {
    Node<K,V> e;
    return (e = removeNode(hash(key), key, null, false, true)) == null ?
        null : e.value;
}

/**
 * 实现Map.remove和相关方法。
 */
final Node<K,V> removeNode(int hash, Object key, Object value,
                           boolean matchValue, boolean movable) {
    Node<K,V>[] tab; Node<K,V> p; int n, index;
    if ((tab = table) != null && (n = tab.length) > 0 &&
        (p = tab[index = (n - 1) & hash]) != null) { // 获取对应哈希值的节点
        Node<K,V> node = null, e; K k; V v;
        if (p.hash == hash && // 如果节点的哈希值与给定的哈希值匹配
            ((k = p.key) == key || (key != null && key.equals(k)))) // 并且键也匹配
            node = p; // 将该节点赋值给node变量
        else if ((e = p.next) != null) { // 如果当前节点不是最后一个节点
            if (p instanceof TreeNode) // 如果当前节点是TreeNode类型
                node = ((TreeNode<K,V>)p).getTreeNode(hash, key); // 获取树中对应的节点
            else { // 如果当前节点是链表类型
                do {
                    if (e.hash == hash && // 如果下一个节点的哈希值与给定的哈希值匹配
                        ((k = e.key) == key || (key != null && key.equals(k)))) // 并且键也匹配
                        node = e; // 将该节点赋值给node变量
                    p = e; // 将当前节点更新为下一个节点
                } while ((e = e.next) != null); // 循环遍历链表，直到找到匹配的节点或链表结束
            }
        }
        if (node != null && (!matchValue || (v = node.value) == value ||
                             (value != null && value.equals(v)))) { // 如果找到了匹配的节点，并且满足条件
            if (node instanceof TreeNode) // 如果节点是TreeNode类型
                ((TreeNode<K,V>)node).removeTreeNode(this, tab, movable); // 调用TreeNode的removeTreeNode方法进行移除操作
            else if (node == p) // 如果移除的是头节点
                tab[index] = node.next; // 更新链表头节点
            else
                p.next = node.next; // 更新其他节点的next指针
            ++modCount; // 修改计数器加一
            --size; // 大小减一
            afterNodeRemoval(node); // 执行节点移除后的操作
            return node; // 返回被移除的节点
        }
    }
    return null; // 如果没有找到匹配的节点，则返回null
}
```

1. `public V remove(Object key)`：从映射中移除指定键的映射关系。首先通过调用`removeNode(hash(key), key, null, false, true)`方法获取与该键关联的节点，如果节点存在，则返回该节点的值；否则返回null。
2. `final Node<K,V> removeNode(int hash, Object key, Object value, boolean matchValue, boolean movable)`：实现`Map.remove`和相关方法。根据给定的哈希值、键、值等信息，找到要移除的节点。如果节点存在且满足匹配条件（`matchValue`为true时），则将节点从链表中移除，并返回该节点；否则返回null。具体解释如下：
- 根据给定的哈希值、键、值等信息，在哈希表中找到要移除的节点。
- 如果节点存在且满足匹配条件（`matchValue`为true时），则将节点从链表中移除，并返回该节点；否则返回null。
- 如果节点是TreeNode类型，则调用`((TreeNode<K,V>)p).getTreeNode(hash, key)`方法获取树节点，并在树中进行移除操作。
- 如果节点不是TreeNode类型，则在链表中进行移除操作。
- 移除节点后，更新哈希表的大小和修改计数器，并执行节点移除后的操作。
- 首先调用`removeNode(hash(key), key, null, false, true)`方法获取与键关联的节点。
- 如果节点存在，则返回节点的值；否则返回null。
- `remove(Object key)`方法：
- `removeNode(int hash, Object key, Object value, boolean matchValue, boolean movable)`方法：
