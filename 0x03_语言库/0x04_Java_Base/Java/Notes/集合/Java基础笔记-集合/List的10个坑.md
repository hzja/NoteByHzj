JavaList
<a name="TRXAI"></a>
## 前言
主要来说一说List操作在实际使用中有哪些坑，以及面对这些坑的时候要怎么解决。
<a name="p9VwV"></a>
## `Arrays.asList`转换基本类型数组的坑
在实际的业务开发中，通常会进行数组转List的操作，通常会使用`Arrays.asList`来进行转换<br />但是在转换基本类型的数组的时候，却出现转换的结果和想象的不一致。<br />上代码
```java
int[] arr = {1, 2, 3}; 
List list = Arrays.asList(arr); 
System.out.println(list.size()); 
// 1
```
实际上，想要转成的List应该是有三个对象而现在只有一个
```java
public static List asList(T... a) { 
    return new ArrayList<>(a); 
}
```
可以观察到 `asList`方法 接收的是一个泛型T类型的参数，T继承Object对象<br />所以通过断点可以看到把 int数组 整体作为一个对象,返回了一个 `List<int[]>`<br />![Arrays.asList不好用](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663552891747-6da8c304-9286-4c7e-935b-207bf5351833.jpeg#clientId=u90909e8e-51ad-4&from=paste&id=rE5vB&originHeight=133&originWidth=815&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue5e7b024-782a-4a0d-9e78-2fcfa04a3d0&title=Arrays.asList%E4%B8%8D%E5%A5%BD%E7%94%A8 "Arrays.asList不好用")<br />**「那该如何解决呢？」**<br />方案一：Java8以上，利用`Arrays.stream(arr).boxed()`将装箱为`Integer`数组
```java
List collect = Arrays.stream(arr).boxed().collect(Collectors.toList()); System.out.println(collect.size()); 
System.out.println(collect.get(0).getClass()); 
// 3 
// class java.lang.Integer
```
方案二：声明数组的时候，声明类型改为包装类型
```java
Integer[] integerArr = {1, 2, 3}; 
List integerList = Arrays.asList(integerArr); 
System.out.println(integerList.size()); System.out.println(integerList.get(0).getClass()); 
// 3 
// class java.lang.Integer
```
<a name="HzFn1"></a>
## `Arrays.asList`返回的List不支持增删操作
将数组对象转成List数据结构之后，竟然不能进行增删操作了
```java
private static void asListAdd(){
    String[] arr = {"1", "2", "3"};
    List<String> strings = new ArrayList<>(Arrays.asList(arr));
    arr[2] = "4";
    System.out.println(strings.toString());
    Iterator<String> iterator = strings.iterator();
    while (iterator.hasNext()){
        if ("4".equals(iterator.next())){
            iterator.remove();
        }
    }
    strings.forEach(val ->{
        strings.remove("4");
        strings.add("3");
    });


    System.out.println(Arrays.asList(arr).toString());
}
```
```java
[1, 2, 4] 
Exception in thread "main" java.lang.UnsupportedOperationException at java.util.AbstractList.remove(AbstractList.java:161) at java.util.AbstractList$Itr.remove(AbstractList.java:374) at java.util.AbstractCollection.remove(AbstractCollection.java:293) at JavaBase.List.AsListTest.lambda$asListAdd$0(AsListTest.java:47) at java.util.Arrays$ArrayList.forEach(Arrays.java:3880) at JavaBase.List.AsListTest.asListAdd(AsListTest.java:46) at JavaBase.List.AsListTest.main(AsListTest.java:20)
```
初始化一个字符串数组，将字符串数组转换为 List，在遍历List的时候进行移除和新增的操作<br />抛出异常信息UnsupportedOperationException。<br />根据异常信息java.lang.UnsupportedOperationException，可以看到他是从`AbstractList`里面出来的，进入源码一看究竟<br />在什么时候调用到了这个 `AbstractList` 呢？<br />其实 `Arrays.asList(arr)` 返回的 `ArrayList` 不是 `java.util.ArrayList`，而是 Arrays的内部类
```java
private static class ArrayList<E> extends AbstractList<E>
        implements RandomAccess, java.io.Serializable{
    private static final long serialVersionUID = -2764017481108945198L;
    private final E[] a;
    ArrayList(E[] array) {
        a = Objects.requireNonNull(array);
    }

    @Override
    public E get(int index) {}

    @Override
    public E set(int index, E element) {...}

...
}
public abstract class AbstractList<E> extends AbstractCollection<E> implements List<E> {
    public boolean add(E e) {
        add(size(), e);
        return true;
    }
    public void add(int index, E element) {
        throw new UnsupportedOperationException();
    }

    public E remove(int index) {
        throw new UnsupportedOperationException();
    }

}
```
他是没有实现 AbstractList 中的 `add()` 和 `remove()` 方法,这里就很清晰了为什么不支持新增和删除，因为根本没有实现。
<a name="pJvZc"></a>
## 对原始数组的修改会影响到获得的那个List
一不小心修改了父List，却影响到了子List，在业务代码中，这会导致产生的数据发生变化，严重的话会造成影响较大的生产问题。<br />第二个坑的源码中，完成字符串数组转换为List之后，<br />将字符串数组的第三个对象的值修改为4，但是很奇怪在打印List的时候，发现List也发生了变化。
```java
public static <T> List<T> asList(T... a) {
    return new ArrayList<>(a);
}

ArrayList(E[] array) {
    a = Objects.requireNonNull(array);
}
```
`asList`中创建了 `ArrayList`，但是他直接引用了原本的数组对象<br />所以只要原本的数组对象一发生变化，List也跟着变化<br />所以在使用到引用的时候，需要特别的注意。<br />解决方案：重新new一个新的 `ArrayList` 来装返回的 List
```java
List strings = new ArrayList<>(Arrays.asList(arr));
```
<a name="GN4N7"></a>
## `java.util.ArrayList`如果不正确操作也不支持增删操作
在第二个坑的时候，说到了 `Arrays.asList` 返回的 List 不支持增删操作，<br />是因为他的自己实现了一个内部类 `ArrayList`，这个内部类继承了 `AbstractList` 没有实现 `add()` 和 `remove()` 方法导致操作失败。<br />但是第三个坑的时候，利用 java.util.ArrayList 包装了返回的 List，进行增删操作还是会失败，那是为什么呢？<br />删除方法逻辑：<br />![删除方法](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663554279720-50ef7ddc-2814-42b1-94fb-c84aa8fcb01d.jpeg#clientId=u90909e8e-51ad-4&from=ui&id=udce93577&originHeight=908&originWidth=2658&originalType=binary&ratio=1&rotation=0&showTitle=true&size=480080&status=done&style=none&taskId=ua113bfbe-026c-4cbd-8db3-6c0c69edfef&title=%E5%88%A0%E9%99%A4%E6%96%B9%E6%B3%95 "删除方法")<br />在foreach中操作增删，因为因为 `modCount` 会被修改，与第一步保存的数组修改次数不一致，抛出异常 `ConcurrentModificationException`<br />在正确操作是什么？总结了四种方式<br />![DM_20220919102257_004.JPG](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663554279098-b0724162-c026-4f10-9210-59536edd308d.jpeg#clientId=u90909e8e-51ad-4&from=ui&id=wBzgj&originHeight=990&originWidth=1704&originalType=binary&ratio=1&rotation=0&showTitle=false&size=302683&status=done&style=none&taskId=u4254af18-7517-483e-8ea2-09e8f2bbf23&title=)<br />正确操作
<a name="Y62ra"></a>
## ArrayList中的 `subList` 强转 `ArrayList` 导致异常
阿里《Java开发手册》上提过
> [强制] ArrayList的`sublist`结果不可強转成ArrayList,否则会抛出ClassCastException
> 异常，即java.util.RandomAccesSubList cannot be cast to java. util.ArrayList.
> 说明: subList 返回的是`ArrayList `的内部类`SubList`，并不是`ArrayList`，而是

`ArrayList`的一个视图，対于`SubList`子列表的所有操作最终会反映到原列表上。
```java
private static void subListTest(){

    List<String> names = new ArrayList<String>() {{

    add("one");

    add("two");

    add("three");

}};
    ArrayList strings = (ArrayList) names.subList(0, 1);
    System.out.println(strings.toString());
}
```
```java
Exception in thread "main" java.lang.ClassCastException: java.util.ArrayList$SubList cannot be cast to java.util.ArrayList
```
猜一下问题是有八九就是出现在subList这个方法上了
```java
private class SubList extends AbstractList<E> implements RandomAccess {

    private final AbstractList<E> parent;
    private final int parentOffset;
    private final int offset;
    int size;
    SubList(AbstractList<E> parent,

    int offset, int fromIndex, int toIndex) {
        this.parent = parent;
        this.parentOffset = fromIndex;
        this.offset = offset + fromIndex;
        this.size = toIndex - fromIndex;
        this.modCount = ArrayList.this.modCount;
    }
}
```
其实 SubList 是一个继承 AbstractList 的内部类，在 SubList 的构建函数中的将 List 中的部分属性直接赋予给自己<br />SubList 没有创建一个新的 List，而是直接引用了原来的 `List(this.parent = parent)`，指定了元素的范围<br />所以 subList 方法不能直接转成 `ArrayList`，他只是`ArrayList`的内部类，没有其他的关系<br />因为是引用的关系，所以在这里也需要特别的注意，如果对原来的List进行修改，会对产生的 subList结果产生影响。
```java
List<String> names = new ArrayList<String>() {{
    add("one");
    add("two");
    add("three");
}};

List strings = names.subList(0, 1);

strings.add(0, "ongChange");

System.out.println(strings.toString());

System.out.println(names.toString());
```
```java
[ongChange, one]

[ongChange, one, two, three]
```
对`subList`产生的List做出结构型修改，操作会反应到原来的List上，`ongChange`也添加到了names中<br />如果修改原来的List则会抛出异常`ConcurrentModificationException`
```java
List<String> names = new ArrayList<String>() {{

    add("one");
    add("two");
    add("three");

}};

List strings = names.subList(0, 1);

names.add("four");

System.out.println(strings.toString());

System.out.println(names.toString());
```
```java
Exception in thread "main" java.util.ConcurrentModificationException
```
原因：subList的时候记录`this.modCount`为3<br />![ConcurrentModificationException](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663554321825-8292fc64-ed2d-4420-ba77-dda11c617f3e.jpeg#clientId=u90909e8e-51ad-4&from=ui&id=u52367e28&originHeight=406&originWidth=1264&originalType=binary&ratio=1&rotation=0&showTitle=true&size=169901&status=done&style=none&taskId=u9fb45ff8-5ce4-40e5-91e7-1ef22f65235&title=ConcurrentModificationException "ConcurrentModificationException")<br />原来的List插入了一个新元素，导致`this.modCount`不第一次保存的不一致则抛出异常<br />解决方案：在操作`SubList`的时候，new一个新的`ArrayList`来接收创建`subList`结果的拷贝
```java
List strings = new ArrayList(names.subList(0, 1));
```
<a name="IunJO"></a>
## ArrayList中的`subList`切片造成OOM
在业务开发中的时候，他们经常通过subList来获取所需要的那部分数据<br />在上面的例子中，知道了`subList`所产生的List，其实是对原来List对象的引用<br />这个产生的List只是原来List对象的视图，也就是说虽然值切片获取了一小段数据，但是原来的List对象却得不到回收，这个原来的List对象可能是一个很大的对象<br />为了方便测试，将vm调整一下 `-Xms20m -Xmx40m`
```java
private static void subListOomTest(){

    IntStream.range(0, 1000).forEach(i ->{
        List<Integer> collect = IntStream.range(0, 100000).boxed().collect(Collectors.toList());
            data.add(collect.subList(0, 1));

        });

    }
}
```
```java
Exception in thread "main" java.lang.OutOfMemoryError: Java heap space
```
出现OOM的原因，循环1000次创建了1000个具有10万个元素的List<br />因为始终被`collect.subList(0, 1)`强引用，得不到回收<br />解决方式：

1. 在`subList`方法返回SubList，重新使用`new ArrayList`，来构建一个独立的`ArrayList`
```java
List list = new ArrayList<>(collect.subList(0, 1));
```

1. 利用Java8的Stream中的`skip`和`limit`来达到切片的目的
```java
List list = collect.stream().skip(0).limit(1).collect(Collectors.toList());
```
在这里可以看到，只要用一个新的容器来装结果，就可以切断与原始List的关系
<a name="JshHb"></a>
## `LinkedList`的插入速度不一定比`ArrayList`快
学习数据结构的时候，就已经得出了结论

- 对于数组，随机元素访问的时间复杂度是0(1)， 元素插入操作是O(n);
- 对于链表，随机元素访问的时间复杂度是O(n), 元素插入操作是0(1).

元素插入对于链表来说应该是他的优势<br />但是他就一定比数组快? 执行插入1000w次的操作
```java
private static void test(){
    StopWatch stopWatch = new StopWatch();
    int elementCount = 100000;
    stopWatch.start("ArrayList add");
    List<Integer> arrayList = IntStream.rangeClosed(1, elementCount).boxed().collect(Collectors.toCollection(ArrayList::new));
    // ArrayList插入数据
    IntStream.rangeClosed(0, elementCount).forEach(i ->arrayList.add(ThreadLocalRandom.current().nextInt(elementCount), 1));
    stopWatch.stop();

    stopWatch.start("linkedList add");
    List<Integer> linkedList = IntStream.rangeClosed(1, elementCount).boxed().collect(Collectors.toCollection(LinkedList::new));
    // ArrayList插入数据
    IntStream.rangeClosed(0, elementCount).forEach(i -> linkedList.add(ThreadLocalRandom.current().nextInt(elementCount), 1));
    stopWatch.stop();
    System.out.println(stopWatch.prettyPrint());
}
```
```java
StopWatch '': running time = 44507882 ns
---------------------------------------------
ns         %     Task name
---------------------------------------------
043836412  098%  elementCount 100 ArrayList add
000671470  002%  elementCount 100 linkedList add

StopWatch '': running time = 196325261 ns
---------------------------------------------
ns         %     Task name
---------------------------------------------
053848980  027%  elementCount 10000 ArrayList add
142476281  073%  elementCount 10000 linkedList add

StopWatch '': running time = 26384216979 ns
---------------------------------------------
ns         %     Task name
---------------------------------------------
978501580  004%  elementCount 100000 ArrayList add
25405715399  096%  elementCount 100000 linkedList add
```
看到在执行插入1万、10完次操作的时候，`LinkedList`的插入操作时间是 `ArrayList`的两倍以上<br />那问题主要就是出现在linkedList的 `add()`方法上
```java
public void add(int index, E element) {

    checkPositionIndex(index);

    if (index == size)
        linkLast(element);
    else
        linkBefore(element, node(index));
}
    
/**
* Returns the (non-null) Node at the specified element index.
    */
Node<E> node(int index) {

    // assert isElementIndex(index);

    if(index < (size >> 1)) {
        Node<E> x = first;
        for (int i = 0; i < index; i++)
            x = x.next;
        return x;
    } else {
        Node<E> x = last;
        for (int i = size - 1; i > index; i--)
            x = x.prev;
        return x;
    }
}
```
linkedList的 `add()`方法主要逻辑

1. 通过遍历找到那个节点的Node
2. 执行插入操作

`ArrayList`的 `add()`方法
```java
public void add(int index, E element) {
    rangeCheckForAdd(index);

    ensureCapacityInternal(size + 1);  // Increments modCount!!
    System.arraycopy(elementData, index, elementData, index + 1,
                     size - index);
    elementData[index] = element;
    size++;
}
```

1. 计算最小容量
2. 最小容量大于数组对象，则进行扩容
3. 进行数组复制,根据插入的index将数组向后移动一位
4. 最后在空位上插入新值

根据试验的测试，得出了在实际的随机插入中，`LinkedList`并没有比ArrayList的速度快<br />所以在实际的使用中，如果涉及到头尾对象的操作，可以使用LinkedList数据结构来进行增删的操作，发挥`LinkedList`的优势<br />最好再进行实际的性能测试评估，来得到最合适的数据结构。
<a name="MBnP3"></a>
## `CopyOnWriteArrayList`内存占用过多
`CopyOnWrite`，顾名思义就是写的时候会将共享变量新复制一份出来，这样做的好处是读操作完全无锁。<br />`CopyOnWriteArrayList`的`add()`方法
```java
public boolean add(E e) {
    // 获取独占锁
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        // 获取array
        Object[] elements = getArray();
        // 复制array到新数组，添加元素到新数组
        int len = elements.length;
        Object[] newElements = Arrays.copyOf(elements, len + 1);
        newElements[len] = e;
        // 替换数组
        setArray(newElements);
        return true;
    } finally {
        // 释放锁
        lock.unlock();
    }
}
```
`CopyOnWriteArrayList` 内部维护了一个数组，成员变量 array 就指向这个内部数组，所有的读操作都是基于新的array对象进行的。<br />因为上了独占锁，所以如果多个线程调用add()方法只有一个线程会获得到该锁，其他线程被阻塞，知道锁被释放， 由于加了锁，所以整个操作的过程是原子性操作<br />`CopyOnWriteArrayList` 会将 新的array复制一份，然后在新复制处理的数组上执行增加元素的操作，执行完之后再将复制的结果指向这个新的数组。<br />由于每次写入的时候都会对数组对象进行复制，复制过程不仅会占用双倍内存，还需要消耗 CPU 等资源，所以当列表中的元素比较少的时候，这对内存和 GC 并没有多大影响，但是当列表保存了大量元素的时候，<br />对 `CopyOnWriteArrayList` 每一次修改，都会重新创建一个大对象，并且原来的大对象也需要回收，这都可能会触发 GC，如果超过老年代的大小则容易触发Full GC，引起应用程序长时间停顿。
<a name="S7Vom"></a>
## `CopyOnWriteArrayList`是弱一致性的
```java
public Iterator<E> iterator() {
    return new COWIterator<E>(getArray(), 0);
}

static final class COWIterator<E> implements ListIterator<E> {
    /** Snapshot of the array */
    private final Object[] snapshot;
    /** Index of element to be returned by subsequent call to next.  */
    private int cursor;

    private COWIterator(Object[] elements, int initialCursor) {
        cursor = initialCursor;
        snapshot = elements;
    }

    public boolean hasNext() {
        return cursor < snapshot.length;
    }

    public boolean hasPrevious() {
        return cursor > 0;
    }

    @SuppressWarnings("unchecked")
    public E next() {
        if (! hasNext())
            throw new NoSuchElementException();
        return (E) snapshot[cursor++];
    }
```
调用`iterator`方法获取迭代器返回一个`COWIterator`对象<br />`COWIterator`的构造器里主要是 保存了当前的list对象的内容和遍历list时数据的下标。<br />snapshot是list的快照信息，因为`CopyOnWriteArrayList`的读写策略中都会使用`getArray()`来获取一个快照信息，生成一个新的数组。<br />所以在使用该迭代器元素时，其他线程对该lsit操作是不可见的，因为操作的是两个不同的数组所以造成弱一致性。
```java
private static void CopyOnWriteArrayListTest(){
    CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList();
    list.add("test1");
    list.add("test2");
    list.add("test3");
    list.add("test4");
    
    Thread thread = new Thread(() -> {
        System.out.println(">>>> start");
        list.add(1, "replaceTest");
        list.remove(2);
    });
    
    // 在启动线程前获取迭代器
    Iterator<String> iterator = list.iterator();

    thread.start();

    try {
        // 等待线程执行完毕
        thread.join();
    } catch (InterruptedException e) {
        e.printStackTrace();
    }

    while (iterator.hasNext()){
        System.out.println(iterator.next());
    }
}
```
```java
>>>> start
test1
test2
test3
test4
```
上面的demo中在启动线程前获取到了原来list的迭代器，<br />在之后启动新建一个线程，在线程里面修改了第一个元素的值，移除了第二个元素<br />在执行完子线程之后，遍历了迭代器的元素，发现子线程里面操作的一个都没有生效，这里提现了迭代器弱一致性。
<a name="lNHW5"></a>
## `CopyOnWriteArrayList`的迭代器不支持增删改
```java
private static void CopyOnWriteArrayListTest(){
    CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList<>();
    list.add("test1");
    list.add("test2");
    list.add("test3");
    list.add("test4");

    Iterator<String> iterator = list.iterator();

    while (iterator.hasNext()){
        if ("test1".equals(iterator.next())){
            iterator.remove();
        }
    }

    System.out.println(list.toString());
}
```
```java
Exception in thread "main" java.lang.UnsupportedOperationException
 at java.util.concurrent.CopyOnWriteArrayList$COWIterator.remove(CopyOnWriteArrayList.java:1178)
```
`CopyOnWriteArrayList `迭代器是只读的，不支持增删操作<br />`CopyOnWriteArrayList`迭代器中的 `remove()`和 `add()`方法，没有支持增删而是直接抛出了异常<br />因为迭代器遍历的仅仅是一个快照，而对快照进行增删改是没有意义的。
```java
/**
 * Not supported. Always throws UnsupportedOperationException.
 * @throws UnsupportedOperationException always; {@code remove}
 *         is not supported by this iterator.
 */
public void remove() {
    throw new UnsupportedOperationException();
}

/**
 * Not supported. Always throws UnsupportedOperationException.
 * @throws UnsupportedOperationException always; {@code set}
 *         is not supported by this iterator.
 */
public void set(E e) {
    throw new UnsupportedOperationException();
}

/**
 * Not supported. Always throws UnsupportedOperationException.
 * @throws UnsupportedOperationException always; {@code add}
 *         is not supported by this iterator.
 */
public void add(E e) {
    throw new UnsupportedOperationException();
}
```
<a name="MxrH0"></a>
## 总结
由于篇幅的限制，只对一些在业务开发中常见的关键点进行梳理和介绍<br />在实际的工作中，不单单是要清除不同类型容器的特性，还要选择适合的容器才能做到事半功倍。<br />主要介绍了`Arrays.asList`转换过程中的一些坑，以及因为操作不当造成的OOM和异常，<br />到最后介绍了线程安全类`CopyOnWriteArrayList`的一些坑，让我们认识到在丰富的API下藏着许多的陷阱。<br />在使用的过程中，需要更加充分的考虑避免这些隐患的发生。<br />最后一张思维导图来回顾一下~<br />![DM_20220919102257_006.JPG](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1663554361778-2ab979fa-17fd-4171-a014-6357d2246ca4.jpeg#clientId=u90909e8e-51ad-4&from=ui&id=ua9454bb6&originHeight=3366&originWidth=2274&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1579614&status=done&style=none&taskId=u2ff51a11-3414-4c25-9f12-ac4c9fbe1c8&title=)
