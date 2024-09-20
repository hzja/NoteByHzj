Java 集合 Collections<br />Collections 是 JDK 提供的一个工具类，位于 java.util 包下，提供了一系列的静态方法，方便对集合进行各种骚操作，算是集合框架的一个大管家。<br />Collections 的用法很简单，在 Intellij IDEA 中敲完 Collections。之后就可以看到它提供的方法了，大致看一下方法名和参数就能知道这个方法是干嘛的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635164159765-434e2843-36b9-4683-816c-76815a9fd3a0.webp#clientId=u63351981-0690-4&from=paste&id=u3f6a3e16&originHeight=1273&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ud5769ec0-d7b5-48df-8089-4e22e5dc4f3)<br />这里将这些方法做了一些分类，并列举了一些简单的例子。
<a name="W17WO"></a>
### 1、排序操作

- `reverse(List list)`：反转顺序
- `shuffle(List list)`：洗牌，将顺序打乱
- `sort(List list)`：自然升序
- `sort(List list, Comparator c)`：按照自定义的比较器排序
- `swap(List list, int i, int j)`：将 i 和 j 位置的元素交换位置

来看例子：
```java
List<String> list = new ArrayList<>();
list.add("二");
list.add("三");
list.add("四");
list.add("五");
list.add("六");

System.out.println("原始顺序：" + list);

// 反转
Collections.reverse(list);
System.out.println("反转后：" + list);

// 洗牌
Collections.shuffle(list);
System.out.println("洗牌后：" + list);

// 自然升序
Collections.sort(list);
System.out.println("自然升序后：" + list);

// 交换
Collections.swap(list, 2,4);
System.out.println("交换后：" + list);
```
输出后：
```java
原始顺序：[二, 三, 四, 五, 六]
反转后：[六, 五, 四, 三, 二]
洗牌后：[五, 二, 六, 三, 四]
自然升序后：[三, 二, 五, 六, 四]
交换后：[三, 二, 四, 六, 五]
```
<a name="Oiule"></a>
### 2、查找操作

- `binarySearch(List list, Object key)`：二分查找法，前提是 List 已经排序过了
- `max(Collection coll)`：返回最大元素
- `max(Collection coll, Comparator comp)`：根据自定义比较器，返回最大元素
- `min(Collection coll)`：返回最小元素
- `min(Collection coll, Comparator comp)`：根据自定义比较器，返回最小元素
- `fill(List list, Object obj)`：使用指定对象填充
- `frequency(Collection c, Object o)`：返回指定对象出现的次数

来看例子：
```java
System.out.println("最大元素：" + Collections.max(list));
System.out.println("最小元素：" + Collections.min(list));
System.out.println("出现的次数：" + Collections.frequency(list, "二"));

// 没有排序直接调用二分查找，结果是不确定的
System.out.println("排序前的二分查找结果：" + Collections.binarySearch(list, "二"));
Collections.sort(list);
// 排序后，查找结果和预期一致
System.out.println("排序后的二分查找结果：" + Collections.binarySearch(list, "二"));

Collections.fill(list, "八");
System.out.println("填充后的结果：" + list);
```
输出后：
```java
原始顺序：[二, 三, 四, 五, 六]
最大元素：四
最小元素：三
出现的次数：1
排序前的二分查找结果：0
排序后的二分查找结果：1
填充后的结果：[八, 八, 八, 八, 八]
```
<a name="Wxcxy"></a>
### 3、同步控制
HashMap 是线程不安全的，其实 ArrayList 也是线程不安全的，没法在多线程环境下使用，那 Collections 工具类中提供了多个 `synchronizedXxx` 方法，这些方法会返回一个同步的对象，从而解决多线程中访问集合时的安全问题。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635164159714-3154b2cb-14bc-477e-80cb-d03dcc06595a.webp#clientId=u63351981-0690-4&from=paste&id=u364761bf&originHeight=444&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=uc100821b-bee8-4ecd-828c-ed89c6b5065)<br />使用起来也非常的简单：
```java
SynchronizedList synchronizedList = Collections.synchronizedList(list);
```
看一眼 `SynchronizedList` 的源码就明白了，不过是在方法里面使用 `synchronized` 关键字加了一层锁而已。
```java
static class SynchronizedList<E>
    extends SynchronizedCollection<E>
    implements List<E> {
    private static final long serialVersionUID = -7754090372962971524L;

    final List<E> list;

    SynchronizedList(List<E> list) {
        super(list);
        this.list = list;
    }

    public E get(int index) {
        synchronized (mutex) {return list.get(index);}
    }
    
    public void add(int index, E element) {
        synchronized (mutex) {list.add(index, element);}
    }
    public E remove(int index) {
        synchronized (mutex) {return list.remove(index);}
    }
}
```
那这样的话，其实效率和那些直接在方法上加 `synchronized` 关键字的 `Vector`、`Hashtable` 差不多（JDK 1.0 时期就有了），而这些集合类基本上已经废弃了，几乎不怎么用。
```java
public class Vector<E>
    extends AbstractList<E>
    implements List<E>, RandomAccess, Cloneable, java.io.Serializable
{

    public synchronized E get(int index) {
        if (index >= elementCount)
            throw new ArrayIndexOutOfBoundsException(index);

        return elementData(index);
    }

    public synchronized E remove(int index) {
        modCount++;
        if (index >= elementCount)
            throw new ArrayIndexOutOfBoundsException(index);
        E oldValue = elementData(index);

        int numMoved = elementCount - index - 1;
        if (numMoved > 0)
            System.arraycopy(elementData, index+1, elementData, index,
                             numMoved);
        elementData[--elementCount] = null; // Let gc do its work

        return oldValue;
    }
}
```
正确的做法是使用并发包下的 `CopyOnWriteArrayList`、`ConcurrentHashMap`。这些放到并发编程时再讲。
<a name="DL28q"></a>
### 4、不可变集合

- `emptyXxx()`：制造一个空的不可变集合
- `singletonXxx()`：制造一个只有一个元素的不可变集合
- `unmodifiableXxx()`：为指定集合制作一个不可变集合

举个例子：
```java
List emptyList = Collections.emptyList();
emptyList.add("非空");
System.out.println(emptyList);
```
这段代码在执行的时候就抛出错误了。
```java
Exception in thread "main" java.lang.UnsupportedOperationException
 at java.util.AbstractList.add(AbstractList.java:148)
 at java.util.AbstractList.add(AbstractList.java:108)
 at com.itwanger.s64.Demo.main(Demo.java:61)
```
这是因为 `Collections.emptyList()` 会返回一个 Collections 的内部类 `EmptyList`，而 `EmptyList` 并没有重写父类 AbstractList 的 `add(int index, E element)` 方法，所以执行的时候就抛出了不支持该操作的 UnsupportedOperationException 了。<br />这是从分析 add 方法源码得出的原因。除此之外，`emptyList` 方法是 `final` 的，返回的 EMPTY_LIST 也是 `final` 的，种种迹象表明 emptyList 返回的就是不可变对象，没法进行增伤改查。
```java
public static final <T> List<T> emptyList() {
    return (List<T>) EMPTY_LIST;
}

public static final List EMPTY_LIST = new EmptyList<>();
```
<a name="LDiFA"></a>
### 5、其他
还有两个方法比较常用：

- `addAll(Collection<? super T> c, T... elements)`，往集合中添加元素
- `disjoint(Collection<?> c1, Collection<?> c2)`，判断两个集合是否没有交集

举个例子：
```java
List<String> allList = new ArrayList<>();
Collections.addAll(allList, "九","十","二");
System.out.println("addAll 后：" + allList);

System.out.println("是否没有交集：" + (Collections.disjoint(list, allList) ? "是" : "否"));
```
输出后：
```java
原始顺序：[二, 三, 四, 五, 六]
addAll 后：[九, 十, 二]
是否没有交集：否
```

---

整体上，Collections 工具类作为集合框架的大管家，提供了一些非常便利的方法。
