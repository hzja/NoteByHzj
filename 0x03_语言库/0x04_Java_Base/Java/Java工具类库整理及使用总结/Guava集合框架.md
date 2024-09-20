Java Guava
<a name="YoNBV"></a>
## 一、不可变集合
不可变集合，顾名思义就是说集合是不可被修改的。集合的数据项是在创建的时候提供，并且在整个生命周期中都不可改变，一旦不可变集合创建之后，是不可以对集合里面的做增,改操作的．为什么需要不可变集合，不可变集合的优势在哪里呢：

- 对不可靠的客户代码库来说，它使用安全，可以在未受信任的类库中安全的使用这些对象．
- 线程安全的：`immutable`对象在多线程下安全，没有竞态条件
- 不可变集合不需要考虑变化，因此可以节省时间和空间。所有不可变的集合都比它们的可变形式有更好的内存利用率（分析和测试细节）
- 不可变对象因为固定不变，可以作为常量来安全使用先看下Guava库里面提供了哪些不可变集合，以及这些集合和JDK里面集合的关系．直接用表格形式列出，如下所示;
| 可变集合接口 | 属于JDK还是Guava | 不可变版本 |
| --- | --- | --- |
| Collection | JDK | ImmutableCollection |
| List | JDK | ImmutableList |
| Set | JDK | ImmutableSet |
| SortedSet/NavigableSet | JDK | ImmutableSortedSet |
| Map | JDK | ImmutableMap |
| SortedMap | JDK | ImmutableSortedMap |
| Multiset | Guava | ImmutableMultiset |
| SortedMultiset | Guava | ImmutableSortedMultiset |
| Multimap | Guava | ImmutableMultimap |
| ListMultimap | Guava | ImmutableListMultimap |
| SetMultimap | Guava | ImmutableSetMultimap |
| BiMap | Guava | ImmutableBiMap |
| ClassToInstanceMap | Guava | ImmutableClassToInstanceMap |
| Table | Guava | ImmutableTable |

<a name="WWR1D"></a>
### 1.1 不可变集合的使用
Guava库里面不可变集合的使用主要在两个方面吧，一个是怎么创建不可变集合，另一个是怎么获取不可变集合里面的元素．关于怎么获取里面的元素，这个咱们不讲．和JDK里面使用集合是一样的．接下来要讲的主要在怎么去创建不可变集合．Guava库里面提供的每个不可变集合类都有如下三种方法．<br />如果尝试去修改不可变集合会跑出UnsupportedOperationException异常．

- 调用静态方法of创建`.of()`方法通过给定集合里面的元素来生成不可变集合．
```java
ImmutableList<String> imutableUserList = ImmutableList.of("jiangxi", "nanchang");
```

- 调用静态方法`copyOf`创建．可以简单的理解为`copyOf`方法就是讲对应的JDK集合转换为Guava不可变集合．如果想将`List`转换为不可变集合，就使用`ImmutableList.copyOf(list)`方法，如果想`Set`装换为不可变集合就调用`ImmutableSet.copyOf(set)`，其他的不可变集合也是类似的．
```java
List<String> userList = new ArrayList<>();
userList.add("jiangxi");
userList.add("nanchang");
ImmutableList<String> imutableUserList = ImmutableList.copyOf(userList);
```

- 调用静态方法`build`创建．
```java
ImmutableList imutableUserList = ImmutableList.builder()
    .add("jiangxi")
    .add("nanchang")
    .build();
```
Guava库里面每个不可变集合一般都有这三种方法来创建对应的不可变集合．当然了里面还有一些其他的方法，都可以到源码里面去看．都是很好理解的一些方法．
<a name="mnf7v"></a>
## 二、新集合类型
Guava引入了很多JDK没有的(`multisets`, `multimaps`, `tables`, bidirectional maps等)、但发现明显有用的新集合类型。这些新类型是为了和JDK集合框架共存，而没有往JDK集合抽象中硬塞其他概念。作为一般规则，Guava集合非常精准地遵循了JDK接口契约。将白了就是和JDK里面集合的使用是差不多的．
<a name="CBRKq"></a>
### 2.1 Multiset
Guava提供了一个新的Set类型`Multiset`，它可以多次添加相等的元素到Set里面去．`Multiset`有很多实现类．先看下Multiset里面的主要方法：

| 方法 | 描述 |
| --- | --- |
| count(E) | 给定元素在Multiset中的计数 |
| elementSet() | Multiset中不重复元素的集合，类型为Set |
| entrySet() | 和Map的entrySet类似，返回Set<multiset.entry>，其中包含的Entry支持getElement()和getCount()方法</multiset.entry |
| add(E, int) | 增加给定元素在Multiset中的计数 |
| remove(E, int) | 减少给定元素在Multiset中的计数 |
| setCount(E, int) | 设置给定元素在Multiset中的计数，不可以为负数 |
| size() | 返回集合元素的总个数（包括重复的元素） |

Guava提供了多种`Multiset`的实现类，大部分都是和JDK中Set类一一对应的，有些也是Guava特有的．

| Multiset实现类 | JDK对应 | 内部实现 |
| --- | --- | --- |
| HashMultiset | HashSet | HashMap |
| TreeMultiset | TreeSet | TreeMap |
| LinkedHashMultiset | LinkedHashSet | LinkedHashMap |
| ConcurrentHashMultiset | LinkedHashSet | ConcurrentHashMap |
| ImmutableMultiset | <br /> | ImmutableMap |
| SortedMultiset | 

 | SortedMultiset是Multiset 接口的变种,支持排序 |

<a name="J1Lsp"></a>
### 2.3 Multimap
每个有经验的Java程序员都在某处实现过`Map<k, List>`或`Map<k, Set>`，并且要忍受这个结构的笨拙。例如，`Map<k, Set>`通常用来表示非标定有向图。Guava的 `Multimap`可以很容易地把一个键映射到多个值。换句话说，Multimap是把键映射到任意多个值的一般方式。说白了就是一个key可以对应多个value．Multimap的使用和咱JDK里面Map的使用是类似的．<br />`Multimap`的各种实现类．<br /></k, </k, </k, 

| Multimap实现类 | 键行为类似 | 行为类似 |
| --- | --- | --- |
| ArrayListMultimap | HashMap | ArrayList |
| HashMultimap | HashMap | HashSet |
| LinkedListMultimap* | LinkedHashMap | LinkedList* |
| LinkedHashMultimap** | LinkedHashMap | LinkedHashMap |
| TreeMultimap | TreeMap | TreeSet |
| ImmutableListMultimap | ImmutableMap | ImmutableList |
| mmutableSetMultimap | ImmutableMap | ImmutableSet |

<a name="AFLn1"></a>
### 2.4 BiMap
BiMap是特殊的Map．BiMap提供了一种新的集合类型，它提供了key和value的双向关联的数据结构。平常咱们用的最多的都是通过key获取value．但是反过来通过value获取key就比较复杂了．BiMap就是来解决这个问题的．BiMap有一个神奇的方`法BiMap.inverse()`．可以把Map里面的key，value反过来．BiMap里面key和value都是唯一的不能重复．

| 对应的BiMap实现 | 键–值实现 | 值–键实现 |
| --- | --- | --- |
| HashBiMap | HashMap | HashMap |
| ImmutableBiMap | ImmutableMap | ImmutableMap |
| EnumBiMap | EnumMap | EnumMap |
| EnumHashBiMap | EnumMap | HashMap |

关于`BiMap`里面各个类api方法都是很容易懂的．
<a name="MBzhI"></a>
### 2.5 Table
当需要多个索引的数据结构的时候，通常情况下，只能用这种丑陋的`Map<firstname,Map>`来实现。为此Guava提供了一个新的集合类型－Table集合类型，来支持这种数据结构的使用场景。Table支持“row”和“column”，Table是Guava提供的一个接口 Interface Table，由rowKey+columnKey+value组成它有两个键，一个值，和一个n行三列的数据表类似，n行取决于Table对对象中存储了多少个数据．<br /></firstname,<br />对于Table而言里面的每个value都有两个`key(rowKey,columnKey)`<br />Table主要方法介绍，每个方法其实都很好理解：
```java
public interface Table<R, C, V> {

    /**
     * 指定行，列对应的值是否存在
     */
    boolean contains(
            @Nullable @CompatibleWith("R") Object rowKey,
            @Nullable @CompatibleWith("C") Object columnKey);

    /**
     * 指定行对应的值是否存在
     */
    boolean containsRow(@Nullable @CompatibleWith("R") Object rowKey);

    /**
     * 指定列对应的值是否存在
     */
    boolean containsColumn(@Nullable @CompatibleWith("C") Object columnKey);

    /**
     * 值是否存在
     */
    boolean containsValue(@Nullable @CompatibleWith("V") Object value);

    /**
     * 获取指定行，列对应的值
     */
    V get(
            @Nullable @CompatibleWith("R") Object rowKey,
            @Nullable @CompatibleWith("C") Object columnKey);

    /** Returns {@code true} if the table contains no mappings. */
    boolean isEmpty();

    /**　获取元素个数 */
    int size();

    // Mutators

    /** 清空 */
    void clear();

    /**
     * 放入元素
     */
    @CanIgnoreReturnValue
    @Nullable
    V put(R rowKey, C columnKey, V value);

    /**
     * 放入元素
     */
    void putAll(Tableextends R, ? extends C, ? extends V> table);

    /**
     * 移除元素
     */
    @CanIgnoreReturnValue
    @Nullable
    V remove(
            @Nullable @CompatibleWith("R") Object rowKey,
            @Nullable @CompatibleWith("C") Object columnKey);

    /**
     * 指定行的所有数据
     */
    Map row(R rowKey);

    /**
     * 指定列的所有数据
     */
    Map column(C columnKey);

    /**
     * 用元素类型为Table.Cell<R, C, V>的Set表现Table<R, C, V>。
     * Cell类似于Map.Entry，但它是用行和列两个键区分的。
     */
    Set<table.cell> cellSet();

    /**
     * 获取Table里面所有行的key
     */
    Set rowKeySet();

    /**
     * 获取Table里面所有列的key
     */
    Set columnKeySet();

    /**
     * 获取Table里面所有的值
     */
    Collection values();

    /**
     * 用Map<R, Map<C, V>>表现Table<R, C, V>
     */
    Map<r, Map> rowMap();

    /**
     * 用Map<C, Map<R, V>>表现Table<R, C, V>
     */
    Map<c, Map> columnMap();
}
```
</c, </r, </table.cell<br />Table的实现类，以及内部是用啥数据类型实现的．

| 对应的Table实现 | 内部实现 |
| --- | --- |
| HashBasedTable | HashMap |
| TreeBasedTable | TreeMap |
| ImmutableTable | ImmutableMap |
| ArrayTable | 二维数组 |

<a name="TGhtt"></a>
### 2.6 `ClassToInstanceMap`
`ClassToInstanceMap`提供了一种是用Class作为Key, 对应实例作为Value的途径．他定义了`T getInstance(Class<T>)`和`T putInstance(Class<T> T)`两个方法, 这两个方法消除了元素类型转换的过程并保证了元素在Map中是类型安全的．使用`ClassToInstanceMap`的唯一目的就是消除类型转换过程中可能产生的错误．比如在传递参数的时候就可以用上`ClassToInstanceMap`了．

| ClassToInstanceMap实现类 | 解释 |
| --- | --- |
| MutableClassToInstanceMap | 可变类型的ClassToInstanceMap |
| ImmutableClassToInstanceMap | 不可变更的ClassToInstanceMap，在对这个Map构造完成后就不可再变更 |

```java
ClassToInstanceMap classToInstanceMap = MutableClassToInstanceMap.create();
classToInstanceMap.putInstance(Integer.class, 10);
classToInstanceMap.putInstance(Float.class, 10L);
classToInstanceMap.putInstance(String.class, "abc");
```
<a name="Puvqj"></a>
### 2.7 `RangeSet`
`RangeSet`描述了一组不相连的、非空的区间。当把一个区间添加到可变的`RangeSet`时，所有相连的区间会被合并，空区间会被忽略。<br />关于`RangeSet`咱们的关注点在两个方面:一个是`RangeSet`的值是可以比较的所以`RangeSet`里面的值对应的对象需要实现`Comparable`接口、第二个是范围，guava里面通过Range类来表示范围。关于Range里面方法也做一个简单的介绍(不同的函数代表不同的区间),如下:

| 概念 | 表示范围 | guava Range类对应方法 |
| --- | --- | --- |
| (a..b) | {x | a < x < b} |
| [a..b] | {x | a <= x <= b} |
| [a..b) | {x | a <= x < b} |
| (a..b] | {x | a < x <= b} |
| (a..+∞) | {x | x > a} |
| [a..+∞) | {x | x >= a} |
| (-∞..b) | {x | x < b} |
| (-∞..b] | {x | x <= b} |
| (-∞..+∞) | all values | all() |

RangeSet方法介绍，如下：
```java
public interface RangeSet {

    /** 是否包含值 */
    boolean contains(C value);

    /**
     * 获取值所在的区间
     */
    Range rangeContaining(C value);

    /**
     * 判断RangeSet中是否有任何区间和给定区间交叉
     */
    boolean intersects(Range otherRange);

    /**
     * 判断RangeSet中是否有任何区间包括给定区间
     */
    boolean encloses(Range otherRange);

    /**
     * 同上，如果RangeSet的范围都在当前RangeSet里面则返回true
     */
    boolean enclosesAll(RangeSet other);

    /**
     * 同上，给定的参数的范围都在当前RangeSet里面则返回true
     */
    default boolean enclosesAll(Iterable<range> other)</range {
        for (Range range : other) {
            if (!encloses(range)) {
                return false;
            }
        }
        return true;
    }

    /** 是否为null */
    boolean isEmpty();

    /**
     * 返回包括RangeSet中所有区间的最小区间
     */
    Range span();

    /**
     * 用Set<range>表现RangeSet，这样可以遍历其中的Range
     */</range
    Set<range> asRanges();

    /**
     * 返回组成此范围集的断开连接的范围的降序视图
     */
    Set<range> asDescendingSetOfRanges();

    /**
     * 返回RangeSet的补集视图。complement也是RangeSet类型,包含了不相连的、非空的区间。
     */
    RangeSet complement();

    /**
     * 返回RangeSet与给定Range的交集视图。这扩展了传统排序集合中的headSet、subSet和tailSet操作
     */
    RangeSet subRangeSet(Range view);

    /**
     * 增加一个范围区间
     */
    void add(Range range);

    /**
     * 移除一个范围区间，如果有交叉的话，会被分割
     */
    void remove(Range range);

    /**
     * 清空
     */
    void clear();

    /**
     * 增加参数里面所有的区间到当前RangeSet里面去
     */
    void addAll(RangeSet other);

    /**
     * 同上把参数给定的区间都增加到当前RangeSet里面去
     */
    default void addAll(Iterable<range> ranges)</range {
        for (Range range : ranges) {
            add(range);
        }
    }

    /**
     * 移除参数给定的所有区间，如果有交集的情况会拆分
     */
    void removeAll(RangeSet other);

    /**
     * 移除参数给定的所有区间，如果有交集的情况会拆分
     */
    default void removeAll(Iterable<range> ranges)</range {
        for (Range range : ranges) {
            remove(range);
        }
    }

}
```
</range</range<br />`RangeSet`实现类，guava提供了两个实现类，有兴趣的也可以进去看看源码，如下：

| RangeSet实现类 | 解释 |
| --- | --- |
| ImmutableRangeSet | 是一个不可修改的RangeSet |
| TreeRangeSet | 利用树的形式来实现 |

`RangeSet`简单使用
```java
RangeSet rangeSet = TreeRangeSet.create();
rangeSet.add(Range.closed(1, 10));
System.out.println(rangeSet);   // [[1..10]]

rangeSet.add(Range.closedOpen(11, 15));
System.out.println(rangeSet);   // [[1..10], [11..15)]

rangeSet.add(Range.open(15, 20));
System.out.println(rangeSet);   // [[1..10], [11..15), (15..20)]

rangeSet.add(Range.openClosed(0, 0));
System.out.println(rangeSet);   // [[1..10], [11..15), (15..20)]

rangeSet.remove(Range.open(5, 10));
System.out.println(rangeSet);   // [[1..5], [10..10], [11..15), (15..20)]
```
<a name="Sz4RX"></a>
### 2.8 `RangeMap`
angeMap描述了”不相交的、非空的区间”到特定值的映射。和`RangeSet`不同，`RangeMap`不会合并相邻的映射，即便相邻的区间映射到相同的值。说的简单直白一点`RangeMap`就是以区间作为键。<br />`RangeMap`方法介绍，如下：
```java
public interface RangeMap {
    /**
     * 获取范围对应的元素(子范围也是可以的)
     */
    @Nullable
    V get(K key);

    /**
     * 如果范围映射中存在此类范围，则返回包含此键及其关联值的范围
     * 1\. 先根据范围找到值
     * 2\. 在根据值找到，所有范围
     */
    @Nullable
    Map.Entry<range, V> getEntry(K key);

    /**
     * 返回包含此RangeMap中范围的最小范围
     */
    Range span();

    /**
     * 放入一个元素
     */
    void put(Range range, V value);

    /**
     * 将范围映射到指定值，将此范围与具有与此范围相连的相同值的任何现有范围合并
     */
    void putCoalescing(Range range, V value);

    /** 放入值 */
    void putAll(RangeMap rangeMap);

    /** 清空 */
    void clear();

    /**
     * 移除
     */
    void remove(Range range);

    /**
     * 将此范围映射的视图作为不可修改的Map返回
     */
    Map<range, V> asMapOfRanges();

    /**
     * 将此范围映射的视图作为不可修改的Map返回
     */
    Map<range, V> asDescendingMapOfRanges();

    /**
     * 返回此范围映射的与范围相交的部分的视图
     */
    RangeMap subRangeMap(Range range);
}
```
</range</range</range<br />RangeMap有两个实现类。有兴趣的大家可以看看源码。

| RangeMap实现类 | 解释 |
| --- | --- |
| ImmutableRangeMap | 不可以修改的RangeMap |
| TreeRangeMap | 利用树的形式来实现 |

RangeMap简单使用：
```java
RangeMap rangeMap = TreeRangeMap.create();
rangeMap.put(Range.closed(1, 10), "foo"); //{[1,10] => "foo"}
rangeMap.put(Range.open(3, 6), "bar"); //{[1,3] => "foo", (3,6) => "bar", [6,10] => "foo"}
rangeMap.put(Range.open(10, 20), "foo"); //{[1,3] => "foo", (3,6) => "bar", [6,10] => "foo", (10,20) => "foo"}
rangeMap.remove(Range.closed(5, 11)); //{[1,3] => "foo", (3,5) => "bar", (11,20) => "foo"}
```
<a name="VmA46"></a>
## 三、强大的集合工具类
提供java.util.Collections中没有的集合工具。任何对JDK集合框架有经验的程序员都熟悉和喜欢java.util.Collections包含的工具方法。Guava沿着这些路线提供了更多的工具方法：适用于所有集合的静态方法。这是Guava最流行和成熟的部分之一。

| 集合接口 | 属于JDK还是Guava | 对应的Guava工具类 |
| --- | --- | --- |
| Iterable | JDK | Iterables |
| Collection | JDK | Collections2：不要和java.util.Collections混淆 |
| List | JDK | Lists |
| Set | JDK | Sets |
| SortedSet | JDK | Sets |
| Map | JDK | Maps |
| SortedMap | JDK | Maps |
| Queue | JDK | Queues |
| Multiset | Guava | Multisets |
| Multimap | Guava | Multimaps |
| BiMap | Guava | Maps |
| Table | Guava | Tables |

<a name="QV1la"></a>
### 3.1 Iterables
```java
public final class Iterators {

    /**
     * 返回不可修改的迭代器
     */
    public static  Iterable unmodifiableIterable(final Iterableextends T> iterable);
    @Deprecated
    public static  Iterable unmodifiableIterable(ImmutableCollection iterable);

    /** 元素个数 */
    public static int size(Iterable iterable);

    /**
     * 是否包含指定元素
     */
    public static boolean contains(Iterable iterable, @Nullable Object element);

    /**
     * 移除集合类的元素
     */
    @CanIgnoreReturnValue
    public static boolean removeAll(Iterable removeFrom, Collection elementsToRemove);

    /**
     * 交集，完全属于
     */
    @CanIgnoreReturnValue
    public static boolean retainAll(Iterable removeFrom, Collection elementsToRetain);

    /**
     * 移除满足条件的元素
     */
    @CanIgnoreReturnValue
    public static  boolean removeIf(Iterable removeFrom, Predicatesuper T> predicate);

    /** 移除第一个满足添加的元素，并且返回该元素 */
    static  @Nullable T removeFirstMatching(
            Iterable removeFrom, Predicatesuper T> predicate);

    /**
     * 如果两个iterable中的所有元素相等且顺序一致，返回true
     */
    public static boolean elementsEqual(Iterable iterable1, Iterable iterable2);

    /**
     * 获取iterable中唯一的元素，如果iterable为空或有多个元素，则快速失败
     */
    public static  T getOnlyElement(Iterable iterable);
    public static  @Nullable T getOnlyElement(
            Iterableextends T> iterable, @Nullable T defaultValue);

    /**
     * 返回迭代器里面满足指定类型的元素对应的数据
     */
    @GwtIncompatible // Array.newInstance(Class, int)
    public static  T[] toArray(Iterableextends T> iterable, Class type);

    /**
     * 把迭代器里面的元素放入到数组里面去
     */
    static  T[] toArray(Iterableextends T> iterable, T[] array);

    /**
     * 把迭代器里面的元素放入到数组里面去
     */
    static Object[] toArray(Iterable iterable);

    /**
     * 把迭代器里面的元素放入到集合里面去
     */
    private static  Collection castOrCopyToCollection(Iterable iterable);

    /**
     * 集合里面的元素都添加到迭代器里面去
     */
    @CanIgnoreReturnValue
    public static  boolean addAll(Collection addTo, Iterableextends T> elementsToAdd);

    /**
     * 返回对象在iterable中出现的次数
     */
    public static int frequency(Iterable iterable, @Nullable Object element) {
        if ((iterable instanceof Multiset)) {
            return ((Multiset) iterable).count(element);
        } else if ((iterable instanceof Set)) {
            return ((Set) iterable).contains(element) ? 1 : 0;
        }
        return Iterators.frequency(iterable.iterator(), element);
    }

    /**
     * 返回一个循环迭代器，可以比作是双向链表，最后一个又和第一个连接起来
     */
    public static  Iterable cycle(final Iterable iterable);
    @SafeVarargs
    public static  Iterable cycle(T... elements);

    /**
     * 串联起来
     */
    public static  Iterable concat(Iterableextends T> a, Iterableextends T> b);
    public static  Iterable concat(
            Iterableextends T> a, Iterableextends T> b, Iterableextends T> c)
    public static  Iterable concat(
            Iterableextends T> a,
            Iterableextends T> b,
            Iterableextends T> c,
            Iterableextends T> d);
    @SafeVarargs
    public static  Iterable concat(Iterableextends T>... inputs);
    public static  Iterable concat(Iterableextends Iterableextends T>> inputs);

    /**
     * 对迭代器做划分，多少个元素一组， 每个分组没满个数的会填null
     * [a, b, c, d, e]} with a partition size of 3 yields {[[a, b, c], [d, e, null]]}
     */
    public static  Iterable<List> partition(final Iterable iterable, final int size);

    /**
     * 对迭代器做划分，多少个元素一组， 每个分组没满个数的会填null
     * [a, b, c, d, e]} with a partition size of 3 yields {[[a, b, c], [d, e]]}
     */
    public static  Iterable<List> paddedPartition(final Iterable iterable, final int size);

    /**
     * 过滤出满足条件的值
     */
    public static  Iterable filter(
            final Iterable unfiltered, final Predicatesuper T> retainIfTrue);

    /**
     * 过滤出指定类型的元素
     */
    @SuppressWarnings("unchecked")
    @GwtIncompatible // Class.isInstance
    public static  Iterable filter(final Iterable unfiltered, final Class desiredType);

    /**
     * 迭代器里面只要有一个元素满足条件就返回true
     */
    public static  boolean any(Iterable iterable, Predicatesuper T> predicate);

    /**
     * 迭代器里面的每个元素是否都满足条件
     */
    public static  boolean all(Iterable iterable, Predicatesuper T> predicate);

    /**
     * 获取满足条件的值
     */
    public static  T find(Iterable iterable, Predicatesuper T> predicate);

    /**
     * 获取满足条件的值，如果没有找到返回defaultValue
     */
    public static  @Nullable T find(
            Iterableextends T> iterable, Predicatesuper T> predicate, @Nullable T defaultValue);

    /**
     * 获取满足条件的值，值用Optional报装
     */
    public static  Optional tryFind(Iterable iterable, Predicatesuper T> predicate);

    /**
     * 获取满足条件记录的位置
     */
    public static  int indexOf(Iterable iterable, Predicatesuper T> predicate);

    /**
     * 对迭代器里面的每个记录做相应的转换
     */
    public static  Iterable transform(
            final Iterable fromIterable, final Functionsuper F, ? extends T> function);

    /**
     * 获取指定位置记录
     */
    public static  T get(Iterable iterable, int position);

    /**
     * 获取指定位置的的记录，如果没有找到返回defaultValue
     */
    public static  @Nullable T get(
            Iterableextends T> iterable, int position, @Nullable T defaultValue);

    /**
     * 获取第一个记录，如果没有找到就用defaultValue代替
     */
    public static  @Nullable T getFirst(Iterableextends T> iterable, @Nullable T defaultValue);

    /**
     * 获取最后一个记录
     */
    public static  T getLast(Iterable iterable);

    /**
     * 获取最后一个记录，如果没有获取到就是defaultValue
     */
    public static  @Nullable T getLast(Iterableextends T> iterable, @Nullable T defaultValue);

    /**
     * 获取最后一个记录
     */
    private static  T getLastInNonemptyList(List list);

    /**
     * 返回跳过指定元素的Iterable
     */
    public static  Iterable skip(final Iterable iterable, final int numberToSkip);

    /**
     * 返回一个（可能）被截取的iterable，元素个数最多为给定值
     */
    public static  Iterable limit(final Iterable iterable, final int limitSize);

    /**
     * 返回一个用于过滤、转换集合中的数据
     */
    public static  Iterable consumingIterable(final Iterable iterable);

    // Methods only in Iterables, not in Iterators

    /**
     * 判断可迭代对象元素是否为null
     */
    public static boolean isEmpty(Iterable iterable);

    /**
     * 获取排序之后的可迭代对象
     */
    @Beta
    public static  Iterable mergeSorted(
            final Iterableextends Iterableextends T>> iterables,
            final Comparatorsuper T> comparator);

}
```
<a name="dlvQb"></a>
### 3.2 Collections2
```java
public final class Collections2 {

    /**
     * 过滤
     */
    public static  Collection filter(Collection unfiltered, Predicate super E> predicate);

    /**
     * 转换
     */
    public static  Collection transform(
            Collection fromCollection, Functionsuper F, T> function);

    /**
     * 先将元素排序，在排列
     */
    @Beta
    public static <e extends comparable super E>> Collection<List> orderedPermutations(
            Iterable elements);
    @Beta
    public static  Collection<List> orderedPermutations(
            Iterable elements, Comparator super E> comparator);

    /**
     * 直接排列
     */
    @Beta
    public static  Collection<List> permutations(Collection elements);

}
```
</e extends comparable
<a name="WoM3k"></a>
### 3.3 Lists
```java
public final class Lists {

    /**
     * 构造ArrayList
     */
    @GwtCompatible(serializable = true)
    public static  ArrayList newArrayList();
    @SafeVarargs
    @GwtCompatible(serializable = true)
    public static  ArrayList newArrayList(E... elements);
    @GwtCompatible(serializable = true)
    public static  ArrayList newArrayList(Iterable extends E> elements);
    @GwtCompatible(serializable = true)
    public static  ArrayList newArrayList(Iterator extends E> elements);

    /**
     * 构造一个带初始化带大小initialArraySize的ArrayList实例
     */
    @GwtCompatible(serializable = true)
    public static  ArrayList newArrayListWithCapacity(int initialArraySize);

    /**
     * 构造一个期望长度为estimatedSize的ArrayList实例
     */
    @GwtCompatible(serializable = true)
    public static  ArrayList newArrayListWithExpectedSize(int estimatedSize);

    // LinkedList

    /**
     * 获取LinkedList
     */
    @GwtCompatible(serializable = true)
    public static  LinkedList newLinkedList();
    @GwtCompatible(serializable = true)
    public static  LinkedList newLinkedList(Iterable extends E> elements);

    /**
     * CopyOnWriteArrayList 读写分离,线程安志的List
     */
    @GwtIncompatible // CopyOnWriteArrayList
    public static  CopyOnWriteArrayList newCopyOnWriteArrayList();
    @GwtIncompatible // CopyOnWriteArrayList
    public static  CopyOnWriteArrayList newCopyOnWriteArrayList(
            Iterable extends E> elements);

    /**
     * 转坏为List
     */
    public static  List asList(@Nullable E first, E[] rest);
    public static  List asList(@Nullable E first, @Nullable E second, E[] rest);

    /**
     * 对集合做笛卡尔操作
     * 假设集合A={a,b},集合B={0,1,2},则两个集合的笛卡尔积为{(a,0),(a,1),(a,2),(b,0),(b,1),(b,2)}
     */
    public static  List<List> cartesianProduct(List extends List extends B>> lists);
    @SafeVarargs
    public static  List<List> cartesianProduct(List extends B>... lists);

    /**
     * 对list里面的每个元素做转换
     */
    public static  List transform(
            List fromList, Functionsuper F, ? extends T> function);

    /**
     * 对list做分区处理
     */
    public static  List<List> partition(List list, int size);

    /**
     * String转换成不可变更的ImmutableList
     */
    public static ImmutableList charactersOf(String string);

    /**
     * CharSequence转list
     */
    @Beta
    public static List charactersOf(CharSequence sequence);

    /**
     * 反转
     */
    public static  List reverse(List list);

}
```
<a name="Fl0Y0"></a>
### 3.4 Sets
```java
public final class Sets {

    /**
     * 返回一个包含给定枚举元素的不可变的Set实例
     */
    // http://code.google.com/p/google-web-toolkit/issues/detail?id=3028
    @GwtCompatible(serializable = true)
    public static <e extends enum> ImmutableSet immutableEnumSet(
            E anElement, E... otherElements);
    @GwtCompatible(serializable = true)
    public static <e extends enum> ImmutableSet immutableEnumSet(Iterable elements);

    /**
     * 返回一个Collector
     */
    public static <e extends enum> Collector<e, ?, immutableset> toImmutableEnumSet();

    /**
     * 返回一个EnumSet实例
     */
    public static <e extends enum> EnumSet newEnumSet();

    // HashSet

    /**
     * 返回一个可变的HashSet实例
     */
    public static  HashSet newHashSet();
    public static  HashSet newHashSet(E... elements);
    public static  HashSet newHashSet(Iterable elements);
    public static  HashSet newHashSet(Iterator elements);

    /**
     * 构造一个期望长度为expectedSize的HashSet实例
     */
    public static  HashSet newHashSetWithExpectedSize(int expectedSize);

    /**
     * 创建一个线程安全的Set，由ConcurrentHashMap的实例支持，因此进行了相同的并发性担保，
     * 与HashSet不同的是，这个Set不允许null元素，该Set是可序列化的。
     */
    public static  Set newConcurrentHashSet();

    /**
     * 创建一个线程安全的Set，包含给定的元素，由ConcurrentHashMap的实例支持，因此进行了相同的并发性担保，
     * 与 HashSet不同的是，这个Set不允许null元素，该Set是可序列化的
     */
    public static  Set newConcurrentHashSet(Iterable elements);

    // LinkedHashSet

    /**
     * 创建一个可变的、空的LinkedHashSet实例
     */
    public static  LinkedHashSet newLinkedHashSet();

    /**
     * 构造一个包含给定元素的LinkedHashSet实例
     */
    public static  LinkedHashSet newLinkedHashSet(Iterable elements);

    /**
     * 构造一个期望长度为expectedSize的LinkedHashSet实例
     */
    public static  LinkedHashSet newLinkedHashSetWithExpectedSize(int expectedSize);

    // TreeSet

    /**
     * 返回一个可变的空的TreeSet实例
     */
    public static  TreeSet newTreeSet();

    /**
     * 返回一个可变的包含给定元素的TreeSet实例
     */
    public static  TreeSet newTreeSet(Iterable elements);

    /**
     * 创建一个具有给定的比较器可变TreeSet的实例
     */
    public static  TreeSet newTreeSet(Comparator comparator);

    /**
     * 创建一个空的Set
     */
    public static  Set newIdentityHashSet() {
        return Collections.newSetFromMap(Maps.newIdentityHashMap());
    }

    /**
     * 生成一个CopyOnWriteArraySet，CopyOnWriteArraySet读写分离，线程安全
     */
    @GwtIncompatible // CopyOnWriteArraySet
    public static  CopyOnWriteArraySet newCopyOnWriteArraySet();
    @GwtIncompatible // CopyOnWriteArraySet
    public static  CopyOnWriteArraySet newCopyOnWriteArraySet(Iterable elements);

    /**
     * 创建一个枚举EnumSet
     */
    public static <e extends enum> EnumSet complementOf(Collection collection);

    /**
     * 创建一个枚举EnumSet,并且里面是值是除了type类型之外的值
     */
    public static <e extends enum> EnumSet complementOf(
            Collection collection, Class type);

    /**
     * 基于指定的Map对象创建一个新的Set对象
     */
    @Deprecated
    public static  Set newSetFromMap(Map map);

    /**
     * 合集，并集
     */
    public static  com.google.common.collect.Sets.SetView union(final Set set1, final Set set2);

    /**
     * 交集
     */
    public static  com.google.common.collect.Sets.SetView intersection(final Set set1, final Set set2);

    /**
     * 差集
     */
    public static  com.google.common.collect.Sets.SetView difference(final Set set1, final Set set2);

    /**
     * 对等差分
     * 给出两个集合 (如集合 A = {1, 2, 3} 和集合 B = {2, 3, 4}),
     * 而数学术语 "对等差分" 的集合就是指由所有只在两个集合其中之一的元素组成的集合(A △ B = C = {1, 4})
     */
    public static  com.google.common.collect.Sets.SetView symmetricDifference(
            final Set set1, final Set set2);

    /**
     * 过滤
     */
    // TODO(kevinb): how to omit that last sentence when building GWT javadoc?
    public static  Set filter(Set unfiltered, com.google.common.base.Predicate predicate);
    public static  SortedSet filter(SortedSet unfiltered, com.google.common.base.Predicate predicate);
    @GwtIncompatible // NavigableSet
    @SuppressWarnings("unchecked")
    public static  NavigableSet filter(
            NavigableSet unfiltered, com.google.common.base.Predicate predicate);

    /**
     * 对Set做笛卡尔操作
     *
     * 假设集合A={a,b},集合B={0,1,2},则两个集合的笛卡尔积为{(a,0),(a,1),(a,2),(b,0),(b,1),(b,2)}
     */
    public static  Set<list> cartesianProduct(List> sets);
    @SafeVarargs
    public static  Set<list> cartesianProduct(Set... sets) {
        return cartesianProduct(Arrays.asList(sets));
    }

    /**
     *
     * 返回Set的所有可能子集的集合。
     * 例如 powerSet(ImmutableSet.of(1, 2))} returns the set {@code {{}, {1}, {2}, {1, 2}}}.
     */
    @GwtCompatible(serializable = false)
    public static  Set<set> powerSet(Set set) {
        return new com.google.common.collect.Sets.PowerSet(set);
    }

    /**
     * 返回大小为size的Set的所有子集的集合
     * 例如 combinations(ImmutableSet.of(1, 2, 3), 2)} returns the set {@code {{1, 2}, {1, 3}, {2, 3}}}.
     */
    @Beta
    public static  Set<set> combinations(Set set, final int size);

    /**
     * 返回一个不可修改的NavigableSet
     */
    public static  NavigableSet unmodifiableNavigableSet(NavigableSet set);

    /**
     * 返回一个同步的（线程安全的）NavigableSet
     */
    @GwtIncompatible // NavigableSet
    public static  NavigableSet synchronizedNavigableSet(NavigableSet navigableSet) {
        return Synchronized.navigableSet(navigableSet);
    }

    /**
     * 获取范围内的Set
     */
    @Beta
    @GwtIncompatible // NavigableSet
    public static <k extends comparableNavigableSet subSet(
            NavigableSet set, Range range);
}
```
</k extends comparable</set</set</list</list</e extends enum</e extends enum</e extends enum</e, ?, immutableset</e extends enum</e extends enum</e extends enum
<a name="faYO9"></a>
### 3.5 Maps
```java
public final class Maps {

    /**
     * 创建ImmutableMap -- 不可以修改的Map
     */
    @GwtCompatible(serializable = true)
    public static <k extends enum, V> ImmutableMap immutableEnumMap(
            Map;

    /**
     * 创建Collector
     */
    public static <t, k extends enum, V> Collector<t, ?, immutablemap> toImmutableEnumMap(
            java.util.function.Functionsuper T, ? extends K> keyFunction,
            java.util.function.Functionsuper T, ? extends V> valueFunction);
    public static <t, k extends enum, V> Collector<t, ?, immutablemap> toImmutableEnumMap(
            java.util.function.Functionsuper T, ? extends K> keyFunction,
            java.util.function.Functionsuper T, ? extends V> valueFunction,
            BinaryOperator mergeFunction);

    /**
     * 创建HashMap
     */
    public static  HashMap newHashMap();
    public static  HashMap newHashMap(Map map);

    /**
     * 创建LinkedHashMap
     */
    public static  LinkedHashMap newLinkedHashMap();
    public static  LinkedHashMap newLinkedHashMap(Map map);

    /**
     * 构造一个期望长度为estimatedSize的LinkedHashMap实例
     */
    public static  LinkedHashMap newLinkedHashMapWithExpectedSize(int expectedSize);

    /**
     * ConcurrentMap -- 是一个能够支持并发访问的java.util.map集合
     */
    public static  ConcurrentMap newConcurrentMap();

    /**
     * TreeMap
     */
    public static  TreeMap newTreeMap();
    public static  TreeMap newTreeMap(SortedMap;
    public static  TreeMap newTreeMap(@Nullable Comparator comparator);

    /**
     * 创建一个EnumMap
     */
    public static <k extends enum, V> EnumMap newEnumMap(Class type);
    public static <k extends enum, V> EnumMap newEnumMap(Map;

    /**
     * 创建一个空的Map
     */
    public static  IdentityHashMap newIdentityHashMap() {
        return new IdentityHashMap<>();
    }

    /**
     * 差集
     */
    @SuppressWarnings("unchecked")
    public static  MapDifference difference(
            Map left, Map right);
    public static  MapDifference difference(
            Map left,
            Map right,
            Equivalencesuper V> valueEquivalence);
    public static  SortedMapDifference difference(
            SortedMap right);

    /**
     * 转换为Map
     */
    public static  Map asMap(Set set, Functionsuper K, V> function);
    /**
     * 转换为SortedMap
     */
    public static  SortedMap asMap(SortedSet set, Functionsuper K, V> function);
    /**
     * 转换为NavigableMap
     */
    @GwtIncompatible // NavigableMap
    public static  NavigableMap asMap(
            NavigableSet set, Functionsuper K, V> function);

    /**
     * value做相应的转换之后，生成ImmutableMap
     */
    public static  ImmutableMap toMap(
            Iterable keys, Functionsuper K, V> valueFunction);
    public static  ImmutableMap toMap(
            Iterator keys, Functionsuper K, V> valueFunction);

    /**
     * key做相应的转换之后，生成ImmutableMap
     */
    @CanIgnoreReturnValue
    public static  ImmutableMap uniqueIndex(
            Iterable values, Functionsuper V, K> keyFunction);
    @CanIgnoreReturnValue
    public static  ImmutableMap uniqueIndex(
            Iterator values, Functionsuper V, K> keyFunction);

    /**
     * 属性文件里面读到的内容Properties转换为ImmutableMap
     */
    @GwtIncompatible // java.util.Properties
    public static ImmutableMap fromProperties(Properties properties);

    /**
     * 生成不可以修改的Entry
     */
    @GwtCompatible(serializable = true)
    public static  Entry immutableEntry(@Nullable K key, @Nullable V value);

    /**
     * BiMapConverter
     */
    public static  Converter asConverter(final BiMap bimap);

    /**
     * 线程安全的BiMap
     */
    public static  BiMap synchronizedBiMap(BiMap bimap);

    /**
     * 不可以修改的BiMap
     */
    public static  BiMap unmodifiableBiMap(BiMap bimap);

    /**
     * 转换值
     */
    public static  Map transformValues(
            Map fromMap, Functionsuper V1, V2> function);
    public static  SortedMap transformValues(
            SortedMap fromMap, Functionsuper V1, V2> function);
    @GwtIncompatible // NavigableMap
    public static  NavigableMap transformValues(
            NavigableMap fromMap, Functionsuper V1, V2> function);

    /**
     * 转换
     */
    public static  Map transformEntries(
            Map fromMap, EntryTransformersuper K, ? super V1, V2> transformer);
    public static  SortedMap transformEntries(
            SortedMap fromMap, EntryTransformersuper K, ? super V1, V2> transformer);
    @GwtIncompatible // NavigableMap
    public static  NavigableMap transformEntries(
            final NavigableMap fromMap, EntryTransformersuper K, ? super V1, V2> transformer);

    @GwtIncompatible // NavigableMap
    private static class TransformedEntriesNavigableMap<K, V1, V2>
            extends TransformedEntriesSortedMap<K, V1, V2> implements NavigableMap<K, V2> {

        TransformedEntriesNavigableMap(
                NavigableMap fromMap, EntryTransformersuper K, ? super V1, V2> transformer) {
            super(fromMap, transformer);
        }

        @Override
        public Entry ceilingEntry(K key) {
            return transformEntry(fromMap().ceilingEntry(key));
        }

        @Override
        public K ceilingKey(K key) {
            return fromMap().ceilingKey(key);
        }

        @Override
        public NavigableSet descendingKeySet() {
            return fromMap().descendingKeySet();
        }

        @Override
        public NavigableMap descendingMap() {
            return transformEntries(fromMap().descendingMap(), transformer);
        }

        @Override
        public Entry firstEntry() {
            return transformEntry(fromMap().firstEntry());
        }

        @Override
        public Entry floorEntry(K key) {
            return transformEntry(fromMap().floorEntry(key));
        }

        @Override
        public K floorKey(K key) {
            return fromMap().floorKey(key);
        }

        @Override
        public NavigableMap headMap(K toKey) {
            return headMap(toKey, false);
        }

        @Override
        public NavigableMap headMap(K toKey, boolean inclusive) {
            return transformEntries(fromMap().headMap(toKey, inclusive), transformer);
        }

        @Override
        public Entry higherEntry(K key) {
            return transformEntry(fromMap().higherEntry(key));
        }

        @Override
        public K higherKey(K key) {
            return fromMap().higherKey(key);
        }

        @Override
        public Entry lastEntry() {
            return transformEntry(fromMap().lastEntry());
        }

        @Override
        public Entry lowerEntry(K key) {
            return transformEntry(fromMap().lowerEntry(key));
        }

        @Override
        public K lowerKey(K key) {
            return fromMap().lowerKey(key);
        }

        @Override
        public NavigableSet navigableKeySet() {
            return fromMap().navigableKeySet();
        }

        @Override
        public Entry pollFirstEntry() {
            return transformEntry(fromMap().pollFirstEntry());
        }

        @Override
        public Entry pollLastEntry() {
            return transformEntry(fromMap().pollLastEntry());
        }

        @Override
        public NavigableMap subMap(
                K fromKey, boolean fromInclusive, K toKey, boolean toInclusive) {
            return transformEntries(
                    fromMap().subMap(fromKey, fromInclusive, toKey, toInclusive), transformer);
        }

        @Override
        public NavigableMap subMap(K fromKey, K toKey) {
            return subMap(fromKey, true, toKey, false);
        }

        @Override
        public NavigableMap tailMap(K fromKey) {
            return tailMap(fromKey, true);
        }

        @Override
        public NavigableMap tailMap(K fromKey, boolean inclusive) {
            return transformEntries(fromMap().tailMap(fromKey, inclusive), transformer);
        }

        private @Nullable Entry transformEntry(@Nullable Entry entry) {
            return (entry == null) ? null : Maps.transformEntry(transformer, entry);
        }

        @Override
        protected NavigableMap fromMap() {
            return (NavigableMap) super.fromMap();
        }
    }

    static  Predicate<entrysuper K> keyPredicate) {
        return compose(keyPredicate, Maps.keyFunction());
    }

    static  Predicate<entrysuper V> valuePredicate) {
        return compose(valuePredicate, Maps.valueFunction());
    }

    /**
     * 根据key过滤
     */
    public static  Map filterKeys(
            Map unfiltered, final Predicatesuper K> keyPredicate);
    public static  SortedMap filterKeys(
            SortedMap unfiltered, final Predicatesuper K> keyPredicate);
    @GwtIncompatible // NavigableMap
    public static  NavigableMap filterKeys(
            NavigableMap unfiltered, final Predicatesuper K> keyPredicate);
    public static  BiMap filterKeys(
            BiMap unfiltered, final Predicatesuper K> keyPredicate);

    /**
     * 根据值过滤
     */
    public static  Map filterValues(
            Map unfiltered, final Predicatesuper V> valuePredicate);
    public static  SortedMap filterValues(
            SortedMap unfiltered, final Predicatesuper V> valuePredicate);
    @GwtIncompatible // NavigableMap
    public static  NavigableMap filterValues(
            NavigableMap unfiltered, final Predicatesuper V> valuePredicate);
    public static  BiMap filterValues(
            BiMap unfiltered, final Predicatesuper V> valuePredicate);

    /**
     * 过滤
     */
    public static  Map filterEntries(
            Map unfiltered, Predicatesuper Entry> entryPredicate);
    public static  SortedMap filterEntries(
            SortedMap unfiltered, Predicatesuper Entry> entryPredicate);
    @GwtIncompatible // NavigableMap
    public static  NavigableMap filterEntries(
            NavigableMap unfiltered, Predicatesuper Entry> entryPredicate);
    public static  BiMap filterEntries(
            BiMap unfiltered, Predicatesuper Entry> entryPredicate);

    /**
     * 返回一个不可修改的NavigableMap
     */
    @GwtIncompatible // NavigableMap
    public static  NavigableMap unmodifiableNavigableMap(
            NavigableMap;

    /**
     * 返回一个同步的（线程安全的）NavigableMap
     */
    @GwtIncompatible // NavigableMap
    public static  NavigableMap synchronizedNavigableMap(
            NavigableMap navigableMap);

    /**
     * key范围内的Map
     */
    @Beta
    @GwtIncompatible // NavigableMap
    public static <k extends comparable<? super K>, V> NavigableMap subMap(
            NavigableMap map, Range range);
}
```
</k extends comparable<? </entry</entry</k extends enum</k extends enum</t, ?, immutablemap</t, k extends enum</t, ?, immutablemap</t, k extends enum</k extends enum
<a name="IMEzh"></a>
### 3.6 Queues
```java
public final class Queues {

    /**
     * 创建ArrayBlockingQueue
     */
    @GwtIncompatible // ArrayBlockingQueue
    public static  ArrayBlockingQueue newArrayBlockingQueue(int capacity);
    // ArrayDeque

    /**
     * 创建ArrayDeque
     */
    public static  ArrayDeque newArrayDeque();

    /**
     * 创建ArrayDeque
     */
    public static  ArrayDeque newArrayDeque(Iterable elements);

    // ConcurrentLinkedQueue

    /** 创建ConcurrentLinkedQueue */
    @GwtIncompatible // ConcurrentLinkedQueue
    public static  ConcurrentLinkedQueue newConcurrentLinkedQueue();

    /**
     * 创建ConcurrentLinkedQueue
     */
    @GwtIncompatible // ConcurrentLinkedQueue
    public static  ConcurrentLinkedQueue newConcurrentLinkedQueue(
            Iterable elements);

    // LinkedBlockingDeque

    /**
     * LinkedBlockingDeque
     */
    @GwtIncompatible // LinkedBlockingDeque
    public static  LinkedBlockingDeque newLinkedBlockingDeque();

    /**
     * LinkedBlockingDeque
     */
    @GwtIncompatible // LinkedBlockingDeque
    public static  LinkedBlockingDeque newLinkedBlockingDeque(int capacity);

    /**
     * LinkedBlockingDeque
     */
    @GwtIncompatible // LinkedBlockingDeque
    public static  LinkedBlockingDeque newLinkedBlockingDeque(Iterable elements);

    // LinkedBlockingQueue

    /** LinkedBlockingQueue */
    @GwtIncompatible // LinkedBlockingQueue
    public static  LinkedBlockingQueue newLinkedBlockingQueue();

    /**
     * LinkedBlockingQueue
     */
    @GwtIncompatible // LinkedBlockingQueue
    public static  LinkedBlockingQueue newLinkedBlockingQueue(int capacity);

    /**
     * LinkedBlockingQueue
     */
    @GwtIncompatible // LinkedBlockingQueue
    public static  LinkedBlockingQueue newLinkedBlockingQueue(Iterable elements);

    /**
     * PriorityBlockingQueue
     */
    @GwtIncompatible // PriorityBlockingQueue
    public static  PriorityBlockingQueue newPriorityBlockingQueue();

    /**
     * PriorityBlockingQueue
     */
    @GwtIncompatible // PriorityBlockingQueue
    public static  PriorityBlockingQueue newPriorityBlockingQueue(
            Iterable elements);

    // PriorityQueue

    /**
     * PriorityQueue
     */
    public static  PriorityQueue newPriorityQueue();

    /**
     * PriorityQueue
     */
    public static  PriorityQueue newPriorityQueue(
            Iterable elements);

    // SynchronousQueue

    /** SynchronousQueue 同步Queue,线程安全 */
    @GwtIncompatible // SynchronousQueue
    public static  SynchronousQueue newSynchronousQueue();

    /**
     * 一次性的从BlockingQueue获取多少个数据
     */
    @Beta
    @CanIgnoreReturnValue
    @GwtIncompatible // BlockingQueue
    @SuppressWarnings("GoodTime") // should accept a java.time.Duration
    public static  int drain(
            BlockingQueue q,
            Collectionsuper E> buffer,
            int numElements,
            long timeout,
            TimeUnit unit)
            throws InterruptedException;

    /**
     * 从BlockingQueue里面获取数据，和drain的区别就是当有异常的时候也会停止获取
     */
    @Beta
    @CanIgnoreReturnValue
    @GwtIncompatible // BlockingQueue
    @SuppressWarnings("GoodTime") // should accept a java.time.Duration
    public static  int drainUninterruptibly(
            BlockingQueue q,
            Collectionsuper E> buffer,
            int numElements,
            long timeout,
            TimeUnit unit);

    /**
     * 获取同步Queue, 同步Queue是线程安全的
     */
    public static  Queue synchronizedQueue(Queue queue);

    /**
     * 获取同步的Deque，Deque是双端队列
     */
    public static  Deque synchronizedDeque(Deque deque);
}
```
<a name="dFUjU"></a>
### 3.7 Multisets
```java
public final class Multisets {

    /**
     * 用于生成一个Collector，T代表流中的一个一个元素，R代表最终的结果。参考Collector.of()的实现
     */
    public static <t, e, m extends multiset> Collector
            java.util.function.Functionsuper T, E> elementFunction,
            java.util.function.ToIntFunctionsuper T> countFunction,
            java.util.function.Supplier multisetSupplier);

    /**
     * 转换为不可以修改的Multiset
     */
    public static  Multiset unmodifiableMultiset(Multiset multiset);

    /**
     * 转换为不可以修改的Multiset
     */
    @Deprecated
    public static  Multiset unmodifiableMultiset(ImmutableMultiset multiset);

    /**
     * 转换为不可以修改的SortedMultiset
     */
    @Beta
    public static  SortedMultiset unmodifiableSortedMultiset(SortedMultiset sortedMultiset);

    /**
     * Returns an immutable multiset entry with the specified element and count. The entry will be
     * serializable if {@code e} is.
     *
     * @param e the element to be associated with the returned entry
     * @param n the count to be associated with the returned entry
     * @throws IllegalArgumentException if {@code n} is negative
     */
    public static  Multiset.Entry immutableEntry(@Nullable E e, int n) {
        return new com.google.common.collect.Multisets.ImmutableEntry(e, n);
    }

    /**
     * 过滤
     */
    @Beta
    public static  Multiset filter(Multiset unfiltered, Predicatesuper E> predicate);

    /**
     * 交集
     */
    public static  Multiset intersection(
            final Multiset multiset1, final Multiset multiset2);

    /**
     * 合集
     */
    @Beta
    public static  Multiset sum(
            final Multiset multiset1, final Multiset multiset2);

    /**
     * 差集
     */
    @Beta
    public static  Multiset difference(
            final Multiset multiset1, final Multiset multiset2);

    /**
     * 对任意o，如果subMultiset.count(o)<=superMultiset.count(o)，返回true
     * 和containsAll()是有区别的，containsAll是指包含所有不重复的元素。举两个例子
     *
     * Multiset multiset1 = HashMultiset.create();
     * multiset1.add("a", 2);
     * multiset1.add("b");
     *
     * Multiset multiset2 = HashMultiset.create();
     * multiset2.add("a", 5);
     *
     * multiset1.containsAll(multiset2); // 返回true；因为包含了所有不重复元素，
     *
     * Multisets.containsOccurrences(multiset1, multiset2); // returns false
     */
    @CanIgnoreReturnValue
    public static boolean containsOccurrences(Multiset superMultiset, Multiset subMultiset);

    /**
     * 修改multisetToModify，以保证任意o都符合multisetToModify.count(o)<=multisetToRetain.count(o)
     */
    @CanIgnoreReturnValue
    public static boolean retainOccurrences(
            Multiset multisetToModify, Multiset multisetToRetain);

    /**
     * 对occurrencesToRemove中的重复元素，仅在multisetToModify中删除相同个数。
     * Multiset multiset1 = HashMultiset.create();
     * multiset1.add("a", 2);
     *
     * Multiset multiset2 = HashMultiset.create();
     * multiset2.add("a", 5);
     *
     * multiset2.removeOccurrences(multiset1); // multiset2 现在包含3个"a"
     */
    @CanIgnoreReturnValue
    public static boolean removeOccurrences(
            Multiset multisetToModify, Iterable occurrencesToRemove);
    @CanIgnoreReturnValue
    public static boolean removeOccurrences(
            Multiset multisetToModify, Multiset occurrencesToRemove);

    /**
     * 返回Multiset的不可变拷贝，并将元素按重复出现的次数做降序排列
     */
    @Beta
    public static  ImmutableMultiset copyHighestCountFirst(Multiset multiset);
}
```
</t, e, m extends multiset
<a name="Ojg8z"></a>
### 3.8 Multimaps
```java
public final class Multimaps {

    /**
     * 用于生成一个Collector，T代表流中的一个一个元素，R代表最终的结果。参考Collector.of()的实现
     */
    @Beta
    public static <t, k, v, m extends multimap> Collector
            java.util.function.Functionsuper T, ? extends K> keyFunction,
            java.util.function.Functionsuper T, ? extends V> valueFunction,
            java.util.function.Supplier multimapSupplier);

    /**
     * 用于生成一个Collector，T代表流中的一个一个元素，R代表最终的结果。参考Collector.of()的实现
     */
    @Beta
    public static <t, k, v, m extends multimap> Collector
            java.util.function.Functionsuper T, ? extends K> keyFunction,
            java.util.function.Functionsuper T, ? extends Stream> valueFunction,
            java.util.function.Supplier multimapSupplier);

    /**
     * 生成Multimap
     */
    public static  Multimap newMultimap(
            Map<k, collection> map, final Supplier> factory)</k, collection;

    /**
     * 生成ListMultimap
     */
    public static  ListMultimap newListMultimap(
            Map<k, collection> map, final Supplier> factory)</k, collection;

    /**
     * 生成SetMultimap
     */
    public static  SetMultimap newSetMultimap(
            Map<k, collection> map, final Supplier> factory)</k, collection;

    /**
     * 生成SortedSetMultimap
     */
    public static  SortedSetMultimap newSortedSetMultimap(
            Map<k, collection> map, final Supplier> factory)</k, collection;

    /**
     * 反转Multimap
     */
    @CanIgnoreReturnValue
    public static <k, v, m extends multimap> M invertFrom(
            Multimap source, M dest);

    /**
     * 生成同步Multimap -- 线程安全
     */
    public static  Multimap synchronizedMultimap(Multimap multimap);

    /**
     * 生成不可以修改的Multimap
     */
    public static  Multimap unmodifiableMultimap(Multimap delegate);
    @Deprecated
    public static  Multimap unmodifiableMultimap(ImmutableMultimap delegate);

    /**
     * 生成同步SetMultimap -- 线程安全
     */
    public static  SetMultimap synchronizedSetMultimap(SetMultimap multimap);

    /**
     * 生成不可以修改的SetMultimap
     */
    public static  SetMultimap unmodifiableSetMultimap(SetMultimap delegate);
    @Deprecated
    public static  SetMultimap unmodifiableSetMultimap(
            ImmutableSetMultimap delegate);

    /**
     * 生成同步SortedSetMultimap -- 线程安全
     */
    public static  SortedSetMultimap synchronizedSortedSetMultimap(
            SortedSetMultimap multimap);

    /**
     * 生成不可以修改的SortedSetMultimap
     */
    public static  SortedSetMultimap unmodifiableSortedSetMultimap(
            SortedSetMultimap delegate);

    /**
     * 生成同步的ListMultimap -- 线程安全
     */
    public static  ListMultimap synchronizedListMultimap(ListMultimap multimap);

    /**
     * 生成不可以修改的ListMultimap
     */
    public static  ListMultimap unmodifiableListMultimap(ListMultimap delegate);
    @Deprecated
    public static  ListMultimap unmodifiableListMultimap(
            ImmutableListMultimap delegate);

    /**
     * 生成不可修改的Collection
     */
    private static  Collection unmodifiableValueCollection(Collection collection);

    /**
     * 生成不可修改的Collection
     */
    private static  Collection<entry> unmodifiableEntries(
            Collection<entry> entries);

    /**
     * ListMultimap转换为Map
     */
    @Beta
    @SuppressWarnings("unchecked")
    // safe by specification of ListMultimap.asMap()
    public static  Map<k, list> asMap(ListMultimap multimap);

    /**
     * SetMultimap转换为Map
     */
    @Beta
    @SuppressWarnings("unchecked")
    // safe by specification of SetMultimap.asMap()
    public static  Map<k, set> asMap(SetMultimap multimap);

    /**
     * SortedSetMultimap转换为Map
     */
    @Beta
    @SuppressWarnings("unchecked")
    // safe by specification of SortedSetMultimap.asMap()
    public static  Map<k, sortedset> asMap(SortedSetMultimap multimap);

    /**
     * Multimap转换为Map
     */
    @Beta
    public static  Map<k, collection> asMap(Multimap multimap);

    /**
     * Map转换为SetMultimap
     */
    public static  SetMultimap forMap(Map map);

    /**
     * 对Value值做转换
     */
    public static  Multimap transformValues(
            Multimap fromMultimap, final Functionsuper V1, V2> function);
    public static  ListMultimap transformValues(
            ListMultimap fromMultimap, final Functionsuper V1, V2> function);

    /**
     * 对Entry值做转换
     */
    public static  Multimap transformEntries(
            Multimap fromMap, EntryTransformersuper K, ? super V1, V2> transformer);
    public static  ListMultimap transformEntries(
            ListMultimap fromMap, EntryTransformersuper K, ? super V1, V2> transformer);

    /**
     *
     * 按照共同的特定属性，组装成ImmutableListMultimap
     *
     * ImmutableSet digits = ImmutableSet.of("zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine");
     * Function lengthFunction = new Function() {
     *     public Integer apply(String string) {
     *         return string.length();
     *     }
     * };
     *
     * ImmutableListMultimap digitsByLength= Multimaps.index(digits, lengthFunction);
     * /*
     * *  digitsByLength maps:
     * *  3 => {"one", "two", "six"}
     * *  4 => {"zero", "four", "five", "nine"}
     * *  5 => {"three", "seven", "eight"}
     *
     */
    public static  ImmutableListMultimap index(
            Iterable values, Functionsuper V, K> keyFunction);
    public static  ImmutableListMultimap index(
            Iterator values, Functionsuper V, K> keyFunction);

    /**
     * 根据key过滤
     */
    public static  SetMultimap filterKeys(
            SetMultimap unfiltered, final Predicatesuper K> keyPredicate);
    public static  ListMultimap filterKeys(
            ListMultimap unfiltered, final Predicatesuper K> keyPredicate);

    /**
     * 根据值过滤
     */
    public static  Multimap filterValues(
            Multimap unfiltered, final Predicatesuper V> valuePredicate);
    public static  SetMultimap filterValues(
            SetMultimap unfiltered, final Predicatesuper V> valuePredicate);

    /**
     * 根据Entry过滤
     */
    public static  Multimap filterEntries(
            Multimap unfiltered, Predicatesuper Entry> entryPredicate);
    public static  SetMultimap filterEntries(
            SetMultimap unfiltered, Predicatesuper Entry> entryPredicate);
}
```
</k, collection</k, sortedset</k, set</k, list</entry</entry</k, v, m extends multimap</t, k, v, m extends multimap</t, k, v, m extends multimap
<a name="zvPX2"></a>
### 3.9 Tables
```java
public final class Tables {

    /**
     * 用于生成一个Collector，T代表流中的一个一个元素，R代表最终的结果。参考Collector.of()的实现
     */
    @Beta
    public static <t, r, c, v, i extends Table> Collector
            java.util.function.Functionsuper T, ? extends R> rowFunction,
            java.util.function.Functionsuper T, ? extends C> columnFunction,
            java.util.function.Functionsuper T, ? extends V> valueFunction,
            java.util.function.Supplier tableSupplier);
    public static <t, r, c, v, i extends Table> Collector
            java.util.function.Functionsuper T, ? extends R> rowFunction,
            java.util.function.Functionsuper T, ? extends C> columnFunction,
            java.util.function.Functionsuper T, ? extends V> valueFunction,
            BinaryOperator mergeFunction,
            java.util.function.Supplier tableSupplier);

    /**
     * 生成一个单元格对象
     */
    public static  Cell immutableCell(
            @Nullable R rowKey, @Nullable C columnKey, @Nullable V value);

    /**
     * 允许你把Table<C, R, V>转置成Table<R, C, V
     */
    public static  Table transpose(Table table);

    /**
     * 允许你指定Table用什么样的map实现行和列
     */
    @Beta
    public static  Table newCustomTable(
            Map<r, Map> backingMap, Supplierextends Map> factory);

    /**
     * 对value做转换
     */
    @Beta
    public static  Table transformValues(
            Table fromTable, Functionsuper V1, V2> function);

    /**
     * 返回不可修改的Table
     */
    public static  Table unmodifiableTable(
            Tableextends R, ? extends C, ? extends V> table);

    /**
     * 生成不可修改的RowSortedTable
     */
    @Beta
    public static  RowSortedTable unmodifiableRowSortedTable(
            RowSortedTable<r, ? extends C, ? extends V> table);

    /**
     * 生成同步Table -- 线程安全
     */
    public static  Table synchronizedTable(Table table);

}
```
</r, ? </r, </t, r, c, v, i </t, r, c, v, i 
<a name="iHtVj"></a>
## 四、扩展工具类
有时候需要实现自己的集合扩展。也许要在元素被添加到列表时增加特定的行为，或者想实现一个`Iterable`，其底层实际上是遍历数据库查询的结果集。Guava也提供了若干工具方法，以便让类似的工作变得更简单。
<a name="JepS4"></a>
### 4.1 `Forwarding`装饰器
针对所有类型的集合接口，Guava都提供了`Forwarding`抽象类以简化装饰者模式的使用。`Forwarding`抽象类定义了一个抽象方法：`delegate()`，可以覆盖这个方法来返回被装饰对象。所有其他方法都会直接委托给`delegate()`。例如说：`ForwardingList.get(int)`实际上执行了`delegate().get(int)`。<br />通过创建`ForwardingXXX`的子类并实现`delegate()`方法，可以选择性地覆盖子类的方法来增加装饰功能，而不需要自己委托每个方法。<br />讲的更加直接一点就是，通过`delegate()`返回一个委托对象，这样可以对委托对象的一些方法的前后做一些自己的操作。<br />Forwarding装饰器对应的类有很多，随便列出几个比如：`ForwardingList`(托管List的一些方法)、`ForwardingMap`(托管Map的一些方法)、`ForwardingDeque`(托管`Deque`的一些方法)等等。<br />以`ForwardingList`来举一个例子，让大家明白`Forwarding`装饰器的使用。自定义`ListWithDefault`继承`ForwardingList`实现一个从List里面获取数据的时候如果为空则用一个默认值代替。当然实际使用过程中，大家需要根据不同的场讲做不同的变换。
```java
import java.util.*;
import com.google.common.collect.*;

public class ListWithDefault<E> extends ForwardingList<E> {
    final E defaultValue;
    final List delegate;

    ListWithDefault(List delegate, E defaultValue) {
        this.delegate = delegate;
        this.defaultValue = defaultValue;
    }
    @Override protected List delegate() {
        return delegate;
    }
    @Override public E get(int index) {
        E v = super.get(index);
        return (v == null ? defaultValue : v);
    }
    @Override public Iterator iterator() {
        final Iterator iter = super.iterator();
        return new ForwardingIterator() {
            @Override protected Iterator delegate() {
                return iter;
            }
            @Override public E next() {
                E v = super.next();
                return (v == null ? defaultValue : v); 
            }
        };
    }
}
```
<a name="NF49c"></a>
### 4.2 `PeekingIterator`
`Iterators`提供一个`Iterators.peekingIterator(Iterator)`方法，来把`Iterator`包装为`PeekingIterator`，这是`Iterator`的子类，它能让你事先窥视`[peek()]`到下一次调用`next()`返回的元素。<br />`PeekingIterator`相对`Iterator`多了一个函数`peek()`函数。`peek()`让我们可以提前看到下一个元素。<br />关于`PeekingIterator`的使用举个例子，复制一个List，并去除连续的重复元素。
```java
List result = Lists.newArrayList();
PeekingIterator iter = Iterators.peekingIterator(source.iterator());
while (iter.hasNext()) {
    E current = iter.next();
    while (iter.hasNext() && iter.peek().equals(current)) {
        //跳过重复的元素
        iter.next();
    }
    result.add(current);
}
```
<a name="t25tQ"></a>
### 4.3 `AbstractIterator`
实现自己的`Iterator`，`AbstractIterator`提供了一个`abstract computeNext()`函数，可以介入`next()`函数的获取。当然到没有元素的时候记得调用`endOfData();`<br />关于`AbstractIterator`的使用，也从网上找到一个例子，如下，要包装一个`iterator`以跳过空值。
```java
public static Iterator<String> skipNulls(final Iterator<String> in) {
    return new AbstractIterator<String>() {
        protected String computeNext() {
            while (in.hasNext()) {
                String s = in.next();
                if (s != null) {
                    return s;
                }
            }
            return endOfData();
        }
    };
}
```
<a name="FdX8F"></a>
### 4.5 AbstractSequentialIterator
`AbstractSequentialIterator`可以看做是迭代器的另一种实现方式。`AbstractSequentialIterator`的源码也很简单，可以一起来看下，源码如下
```java
public abstract class AbstractSequentialIterator<T> extends UnmodifiableIterator<T> {
    private @Nullable T nextOrNull;

    /**
     * 我们需要提供第一个元素
     */
    protected AbstractSequentialIterator(@Nullable T firstOrNull) {
        this.nextOrNull = firstOrNull;
    }

    /**
     * 我们需要实现的方法
     */
    @Nullable
    protected abstract T computeNext(T previous);

    /**
     * 是否有下一个元素
     */
    @Override
    public final boolean hasNext() {
        return nextOrNull != null;
    }

    /**
     * 在调用next()方法获取next元素之后，又通过computeNext()方法把接下来的一个元素给算出来了
     */
    @Override
    public final T next() {
        if (!hasNext()) {
            throw new NoSuchElementException();
        }
        try {
            return nextOrNull;
        } finally {
            nextOrNull = computeNext(nextOrNull);
        }
    }
}
```
通过源码的阅读，`AbstractSequentialIterator`就是通过指定第一个元素之后，在根据`computeNext()`函数依据某种规则产生下一个元素。<br />用一个例子来说明`AbstractSequentialIterator`的使用，每一个数都是前一个数的两倍。
```java
Iterator powersOfTwo = new AbstractSequentialIterator(1) { // 注意初始值1!
    protected Integer computeNext(Integer previous) {
        // 下一个数是前数的两倍，一直到2的3次方
        return (previous == 1 << 3) ? null : previous * 2;
    }
};
```
