<a name="IDOAv"></a>
## 前言
在编程的世界里，常常被一些基础的、看似简单的工具所困扰。<br />比如在Java中，经常使用的ArrayList类。<br />它提供了一种方便的方式来管理和操作一个动态数组，但是你是否曾经停下来3思考过它是如何工作的呢？它的内部机制是什么？<br />如果对这些问题感到好奇，那么一起深入到Java的核心库中去，探索ArrayList的源代码，揭开它的神秘面纱。
<a name="pfzAX"></a>
### ArrayList 简单使用技巧
首先是 ArrayList 的简单使用技巧，主要是介绍 ArrayList 的几个常见方法：
```java
/**
 * 编写一个ArrayList的简单实用demo
 * ArrayList 的常见方法包括：
 *     add(element):添加元素
 *     get(index):获取下标元素
 *     remove(index):移除下标对应元素
 *     set(index,element):将index处的元素修改为element
 */
public static void main(String[] args) {
    // 创建 ArrayList 的对象
    ArrayList data = new ArrayList();
    // 添加元素
    data.add("Java面试教程");
    // 构造随机数并进行添加
    Random rnd = new Random();

    System.out.println("随机20个数字添加进入data集合");
    for (int i = 0; i < 20; i++) {
        data.add(rnd.nextInt(1000));
    }
    // 取出ArrayList里的元素进行打印
    System.out.println("取出ArrayList里的元素进行打印");
    for (int i = 0; i < data.size(); i++) {
        System.out.print(data.get(i) + "  ");
    }
    // 修改1号index成的元素为了不起
    System.out.println();
    data.set(1, "了不起");
    System.out.println("修改1号index成的元素为了不起：data.set(1, \"了不起\")");
    for (int i = 0; i < data.size(); i++) {
        System.out.print(data.get(i) + "  ");
    }
    System.out.println("");
    // 移除“了不起”元素
    System.out.println("移除“了不起”元素：data.remove(1)");
    data.remove(1);
    for (int i = 0; i < data.size(); i++) {
        System.out.print(data.get(i) + "  ");
    }
    System.out.println("");
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700529695181-e6b1843a-4fa4-44fe-904a-cedf54e073c0.png#averageHue=%23302f2e&clientId=u73b2aba4-00c6-4&from=paste&id=ud60f4d82&originHeight=195&originWidth=927&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1e79e4f4-5099-42d2-80e9-e4acb92c1f3&title=)<br />这段代码是一个简单的Java程序，主要演示了如何使用ArrayList类来存储、添加、修改和移除元素。<br />下面是对这段代码的详细解释：

1. 首先，创建一个名为data的ArrayList对象。ArrayList是一种动态数组，可以根据需要自动调整大小。
2. 然后，向data中添加一个字符串元素"Java面试教程"。
3. 接下来，创建一个Random对象rnd，用于生成随机数。
4. 使用for循环，向data中添加20个随机整数（范围在0到999之间）。
5. 使用另一个for循环，遍历data中的所有元素并打印它们。
6. 修改data中索引为1的元素为字符串"了不起"。
7. 再次使用for循环，打印修改后的data中的所有元素。
8. 最后，使用`remove`方法从data中移除索引为1的元素（即"了不起"），并打印移除元素后的data。

这段代码展示了ArrayList的基本操作，包括添加元素、获取元素、修改元素和移除元素。
<a name="SYgxd"></a>
## 源码分析
接下来，来看看关于`ArrayList`源码：
<a name="Iy5qk"></a>
### 1、ArrayList初始化
```java
// ArrayList 初始化时默认大小为10
private static final int DEFAULT_CAPACITY = 10;

// 用于空实例的共享空数组实例。其实就是直接初始化的话一个空数组
private static final Object[] EMPTY_ELEMENTDATA = {};

//构造一个具有指定初始容量的空列表,初始化ArrayList,传入初始化时的大小
public ArrayList(int initialCapacity) {
    if (initialCapacity > 0) {
        this.elementData = new Object[initialCapacity];
    } else if (initialCapacity == 0) {
        this.elementData = EMPTY_ELEMENTDATA;
    } else {
        throw new IllegalArgumentException("Illegal Capacity: "+
                initialCapacity);
    }
}
//构造一个初始容量为10的空列表。
//如果不传入大小的话就默认大小是10，那么这里就有一个问题：
//上面插入的元素超过了10，继续插入元素就会进行拷贝扩容，性能不是特别高。
//所以一般情况下初始化时给定一个比较靠谱的数组大小，避免到时候导致元素不断拷贝
public ArrayList() {
    this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
}
```
创建ArrayList对象时，如果没有指定大小参数，那么默认情况下，它会被初始化为一个包含10个元素的数组。<br />这意味着，每当尝试插入超过10个元素时，ArrayList会进行数组拷贝和扩容操作。<br />这种频繁的数组拷贝和扩容会导致性能消耗较大。<br />因此，为了优化性能，建议在初始化ArrayList时，为其指定一个相对较大的容量大小。
<a name="D3Sw7"></a>
### 2、ArrayList的add方法
<a name="iuZv1"></a>
#### `public boolean add(E e)` 方法：
这个方法用于向ArrayList中添加一个元素。<br />首先，它调用`ensureCapacityInternal(size + 1)`来确保ArrayList的容量足够容纳新元素。<br />然后，将新元素添加到ArrayList的末尾，并将数组的大小加1。<br />最后，返回true表示添加成功。
```java
public boolean add(E e) {
    // 增量modCount
    ensureCapacityInternal(size + 1);  
    elementData[size++] = e;
    return true;
}
//这个方法用于确保ArrayList的容量足够容纳新元素。它首先调用ensureExplicitCapacity(calculateCapacity(elementData, minCapacity))来确定所需的最小容量
private void ensureCapacityInternal(int minCapacity) {
        ensureExplicitCapacity(calculateCapacity(elementData, minCapacity));
}
//这个方法用于计算ArrayList的容量。如果传入的elementData是默认的空元素数组，则返回默认容量和最小容量中的较大值；否则，返回传入的最小容量
private static int calculateCapacity(Object[] elementData, int minCapacity) {
        if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
            return Math.max(DEFAULT_CAPACITY, minCapacity);
        }
        return minCapacity;
}
//这个方法用于确保ArrayList的容量足够容纳新元素。它首先增加modCount（用于记录集合修改次数），然后检查是否需要扩容。如果需要的最小容量减去当前数组长度大于0，就调用grow(minCapacity)方法进行扩容操作。
private void ensureExplicitCapacity(int minCapacity) {
        modCount++;
        // overflow-conscious code
        if (minCapacity - elementData.length > 0)
            grow(minCapacity);
}
```
<a name="cUZQb"></a>
#### `public void add(int index, E element)` 方法：
这个方法用于在指定索引位置插入一个元素。<br />首先，它调用`rangeCheckForAdd(index)`来检查索引是否在有效范围内。<br />然后，再次调用`ensureCapacityInternal(size + 1)`来确保ArrayList的容量足够容纳新元素。<br />接下来，使用`System.arraycopy()`方法将指定索引位置之后的所有元素向后移动一个位置，为新元素腾出空间。<br />然后将新元素插入到指定索引位置，并将数组的大小加1。
```java
public void add(int index, E element) {
    rangeCheckForAdd(index);
    ensureCapacityInternal(size + 1);  
    System.arraycopy(elementData, index, elementData, index + 1,
            size - index);
    elementData[index] = element;
    size++;
}
//这个方法用于检查给定的索引是否在有效范围内。如果索引大于当前列表的大小或小于0，就会抛出IndexOutOfBoundsException异常。
private void rangeCheckForAdd(int index) {
        if (index > size || index < 0)
            throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
}
//这个方法用于确保ArrayList的容量足够容纳新元素。它首先调用ensureExplicitCapacity(calculateCapacity(elementData, minCapacity))来确定所需的最小容量
private void ensureCapacityInternal(int minCapacity) {
        ensureExplicitCapacity(calculateCapacity(elementData, minCapacity));
}
//这个方法用于计算ArrayList的容量。如果传入的elementData是默认的空元素数组，则返回默认容量和最小容量中的较大值；否则，返回传入的最小容量
private static int calculateCapacity(Object[] elementData, int minCapacity) {
        if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
            return Math.max(DEFAULT_CAPACITY, minCapacity);
        }
        return minCapacity;
}
//这个方法用于确保ArrayList的容量足够容纳新元素。它首先增加modCount（用于记录集合修改次数），然后检查是否需要扩容。如果需要的最小容量减去当前数组长度大于0，就调用grow(minCapacity)方法进行扩容操作。
private void ensureExplicitCapacity(int minCapacity) {
        modCount++;
        // overflow-conscious code
        if (minCapacity - elementData.length > 0)
            grow(minCapacity);
}
```
<a name="Qn8Kd"></a>
#### `public void add(E e)` 方法（带并发修改检查）：
这个方法用于在没有锁的情况下向ArrayList中添加一个元素。<br />首先，它调用`checkForComodification()`来检查ArrayList是否被其他线程修改过。<br />然后，尝试在当前游标位置插入新元素，并更新游标。<br />如果发生`IndexOutOfBoundsException`异常，说明当前线程试图修改一个不存在的元素，此时会抛出`ConcurrentModificationException`异常。
```java
public void add(E e) {
    checkForComodification();
    try {
        int i = cursor;
        ArrayList.this.add(i, e);
        cursor = i + 1;
        lastRet = -1;
        expectedModCount = modCount;
    } catch (IndexOutOfBoundsException ex) {
        throw new ConcurrentModificationException();
    }
}
//它用于检查对ArrayList的修改是否被其他线程同时进行
final void checkForComodification() {
    //通过比较modCount（表示ArrayList在当前方法调用之前被修改的次数）和expectedModCount（表示ArrayList在进入该方法时期望的修改次数）来判断是否有并发修改发生
    //果两者不相等，说明在当前方法调用期间有其他线程对ArrayList进行了修改，而当前线程没有获取到正确的数据状态，因此抛出ConcurrentModificationException异常。
    if (modCount != expectedModCount)
        throw new ConcurrentModificationException();
}
```
<a name="gPavJ"></a>
### 3、ArrayList的set方法
<a name="yi2SR"></a>
#### `public E set(int index, E element)`：
这个方法用于在ArrayList的指定索引位置设置一个新的元素。<br />首先，它会调用`rangeCheck(index)`来检查索引是否在有效范围内。<br />然后，它会获取该索引位置的旧值，并将新元素设置到该位置。<br />最后，它返回旧值。
```java
public E set(int index, E element) {
    rangeCheck(index);
    E oldValue = elementData(index);
    elementData[index] = element;
    return oldValue;
}
//这个方法用于检查给定的索引是否在有效范围内。如果索引小于0或大于等于ArrayList的大小，将抛出IndexOutOfBoundsException异常。
private void rangeCheck(int index) {
    if (index < 0 || index >= this.size)
        throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
}
```
<a name="jPqL8"></a>
#### `public void set(E e)`：
这个方法用于在ArrayList的最后一个元素位置设置一个新的元素。<br />首先，它会检查`lastRet`是否小于0，如果是，那么它会抛出一个`IllegalStateException`异常。<br />然后，它会调用`checkForComodification()`来检查ArrayList是否被并发修改。<br />接着，它会尝试调用`ArrayList.this.set(lastRet, e)`来设置新元素。<br />如果发生`IndexOutOfBoundsException`异常，那么它会抛出一个`ConcurrentModificationException`异常。
```java
public void set(E e) {
    if (lastRet < 0)
        throw new IllegalStateException();
    checkForComodification();

    try {
        ArrayList.this.set(lastRet, e);
    } catch (IndexOutOfBoundsException ex) {
        throw new ConcurrentModificationException();
    }
}
```
<a name="Y2G6c"></a>
### 4、ArrayList的get方法
<a name="nULt6"></a>
#### `public E get(int index)`：
这是一个公开方法，返回类型为E，参数为一个整数index，表示要获取元素的索引位置。
```java
public E get(int index) {
    rangeCheck(index);
    checkForComodification();
    return ArrayList.this.elementData(offset + index);
}
//这个方法用于检查给定的索引是否在有效范围内。如果索引超出范围，它将抛出一个IndexOutOfBoundsException异常。
private void rangeCheck(int index) {
    if (index < 0 || index >= this.size)
        throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
}
//这个方法用于检查ArrayList是否被并发修改。如果在调用此方法时ArrayList正在被修改（例如，其他线程正在添加或删除元素），那么它将抛出一个ConcurrentModificationException异常。
private void checkForComodification() {
    if (ArrayList.this.modCount != this.modCount)
        throw new ConcurrentModificationException();
}
//这是获取指定索引位置元素的主要逻辑。首先，它会计算偏移量（offset）与索引的和，得到实际的数组下标。然后，它会调用ArrayList的内部方法elementData()来获取该下标位置的元素，并将其返回。
E elementData(int index) {
    return (E) elementData[index];
}
```
`get(int index)`的方法，用于获取ArrayList中指定索引位置的元素。<br />在获取元素之前，它会调用`rangeCheck(index)`来检查索引是否在有效范围内，然后调用`checkForComodification()`来检查ArrayList是否被并发修改。<br />如果一切正常，它会计算实际的数组下标，并调用ArrayList的内部方法`elementData()`来获取该下标位置的元素，并将其返回。
<a name="oLUDc"></a>
### 5、ArrayList 的 `remove` 方法
<a name="OPtSp"></a>
#### `public void remove()`：
这个方法用于移除ArrayList的最后一个元素。<br />首先，它会检查`lastRet`是否小于0，如果是，那么它会抛出一个`IllegalStateException`异常。<br />然后，它会调用`checkForComodification()`来检查ArrayList是否被并发修改。<br />接着，它会尝试调用`ArrayList`的内部方法`remove(lastRet)`来移除最后一个元素。<br />如果发生`IndexOutOfBoundsException`异常，那么它会抛出一个`ConcurrentModificationException`异常。
```java
public void remove() {
    if (lastRet < 0)
        throw new IllegalStateException();
    checkForComodification();

    try {
        ArrayList.this.remove(lastRet);
        cursor = lastRet;
        lastRet = -1;
        expectedModCount = modCount;
    } catch (IndexOutOfBoundsException ex) {
        throw new ConcurrentModificationException();
    }
}
```
<a name="jyiJu"></a>
#### `public E remove(int index)`：
这个方法用于移除ArrayList中指定索引位置的元素。<br />首先，它会调用`rangeCheck(index)`来检查索引是否在有效范围内。<br />然后，它会调用`checkForComodification()`来检查ArrayList是否被并发修改。<br />接着，它会调用父类（即ArrayList）的`remove(parentOffset + index)`方法来移除指定索引位置的元素，并将返回的元素保存在`result`变量中。<br />最后，它会更新ArrayList的修改计数器和大小，并返回移除的元素。
```java
public E remove(int index) {
    // 进行index是否越界的判断
    rangeCheck(index);
    checkForComodification();
    E result = parent.remove(parentOffset + index);
    this.modCount = parent.modCount;
    this.size--;
    return result;
}
```
<a name="TPlcY"></a>
#### `public E remove(E element)`：
这个方法用于移除ArrayList中第一个出现的指定元素。<br />首先，它会调用`rangeCheck(index)`来检查元素是否在有效范围内。<br />然后，它会调用`checkForComodification()`来检查ArrayList是否被并发修改。<br />接着，它会获取指定元素的旧值，并计算需要移动的元素数量。如果需要移动的元素数量大于0，那么它会使用`System.arraycopy()`方法将后面的元素向前移动一位。<br />最后，它会将ArrayList的大小减1，并将最后一个元素设置为null，然后返回移除的元素。
```java
public E remove(int index) {
    rangeCheck(index);
    modCount++;
    E oldValue = elementData(index);
    int numMoved = size - index - 1;
    if (numMoved > 0)
        System.arraycopy(elementData, index+1, elementData, index,
                numMoved);
    elementData[--size] = null;
    return oldValue;
}

//这个方法用于检查给定的索引是否在有效范围内。如果索引超出范围，它将抛出一个IndexOutOfBoundsException异常。
private void rangeCheck(int index) {
    if (index < 0 || index >= this.size)
        throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
}
//这是获取指定索引位置元素的主要逻辑。首先，它会计算偏移量（offset）与索引的和，得到实际的数组下标。然后，它会调用ArrayList的内部方法elementData()来获取该下标位置的元素，并将其返回。
E elementData(int index) {
    return (E) elementData[index];
}
```
<a name="CTVUD"></a>
### 动态扩容和拷贝
```java
private void ensureCapacityInternal(int minCapacity) {
    ensureExplicitCapacity(calculateCapacity(elementData, minCapacity));
}

private void ensureExplicitCapacity(int minCapacity) {
    modCount++;
    if (minCapacity - elementData.length > 0)
        grow(minCapacity);
}

private void grow(int minCapacity) {
    int oldCapacity = elementData.length;
    // 这里做了位运算，相当于数组扩容了1.5倍
    int newCapacity = oldCapacity + (oldCapacity >> 1);
    if (newCapacity - minCapacity < 0)
        newCapacity = minCapacity;
    if (newCapacity - MAX_ARRAY_SIZE > 0)
        newCapacity = hugeCapacity(minCapacity);
    // 随后进行元素拷贝
    elementData = Arrays.copyOf(elementData, newCapacity);
}
```
这里会涉及三个方法：`ensureCapacityInternal(int minCapacity), ensureExplicitCapacity(int minCapacity)` 和 `grow(int minCapacity)`。<br />这三个方法都是用于管理ArrayList的容量：

1. `ensureCapacityInternal(int minCapacity)`：这个方法用于确保ArrayList的最小容量至少为`minCapacity`。首先，它会调用`ensureExplicitCapacity(int minCapacity)`方法来确保`ArrayList`的显式容量至少为`minCapacity`。
2. `ensureExplicitCapacity(int minCapacity)`：这个方法用于确保ArrayList的显式容量至少为`minCapacity`。首先，它会将`modCount`（表示`ArrayList`被修改的次数）加1。然后，如果`minCapacity - elementData.length > 0`（即当前的容量小于需要的最小容量），它会调用`grow(int minCapacity)`方法来扩容。
3. `grow(int minCapacity)`：这个方法用于扩容`ArrayList`。首先，它会获取当前的元素数组的长度，并将其赋值给`oldCapacity`。然后，它会通过位运算将数组长度扩大1.5倍，并将结果赋值给`newCapacity`。接着，如果`newCapacity`小于`minCapacity`，它会将`newCapacity`设置为`minCapacity`。然后，如果`newCapacity`大于`MAX_ARRAY_SIZE`（即`ArrayList`的最大容量），它会调用`hugeCapacity(int minCapacity)`方法来获取一个更大的容量。最后，它会使用`Arrays.copyOf(elementData, newCapacity)`方法来创建一个新的元素数组，并将原数组的元素复制到新数组中。

