Java ArrayList
<a name="OnW4h"></a>
### 构造函数
ArrayList 有三个构造函数，默认不带参数的构造函数就是初始化一个空数组。
```java
//一个空数组
private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};

//实际存储元素的数组
transient Object[] elementData; 

public ArrayList() {
    this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
}
```
带一个 int 类型的容量参数的构造函数，可以指定 ArrayList 的容量大小。
```java
//空数组
private static final Object[] EMPTY_ELEMENTDATA = {};

public ArrayList(int initialCapacity) {
    if (initialCapacity > 0) {
        //容量大于 0 就构建一个 Object 的数组
        this.elementData = new Object[initialCapacity];
    } else if (initialCapacity == 0) {
        //容量等于 0 就是一个空数组
        this.elementData = EMPTY_ELEMENTDATA;
    } else {
        //容量小于 0 抛出异常
        throw new IllegalArgumentException("Illegal Capacity: "+
                                            initialCapacity);
    }
}
```
带一个集合的构造函数，将集合转换成 `Object[]` 类型后拷贝到 `elementData` 中。
```java
public ArrayList(Collection<? extends E> c) {
    //集合转为数组
    Object[] a = c.toArray();
    if ((size = a.length) != 0) {
        //集合是不是 ArrayList 类型
        if (c.getClass() == ArrayList.class) {
            elementData = a;
        } else {
            //将集合元素拷贝成 Object[] 到 elementData
            elementData = Arrays.copyOf(a, size, Object[].class);
        }
    } else {
        //空元素
        elementData = EMPTY_ELEMENTDATA;
    }
}
```
<a name="hZdFg"></a>
### 常用函数
<a name="uz3JI"></a>
#### `add()`
先从 ArrayList 最常用的方法 `add()` 开始讲起，`add()` 方法就是将元素添加到 `elementData` 的末尾。平均时间复杂度为 O(1)。
```java
//ArrayList.add()
public boolean add(E e) {
    //检查数组是否存放的下
    ensureCapacityInternal(size + 1); 
    //添加到末尾
    elementData[size++] = e;
    return true;
}

//ArrayList.ensureCapacityInternal()
private void ensureCapacityInternal(int minCapacity) {
    ensureExplicitCapacity(calculateCapacity(elementData, minCapacity));
}

//ArrayList.calculateCapacity()
private static int calculateCapacity(Object[] elementData, int minCapacity) {
    //检查时不是默认时的空数组，是默认时的空数组，初始化的容量就是 10
    if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
        return Math.max(DEFAULT_CAPACITY, minCapacity);
    }
    return minCapacity;
}

//ArrayList.ensureExplicitCapacity()
private void ensureExplicitCapacity(int minCapacity) {
    modCount++;

    if (minCapacity - elementData.length > 0)
        grow(minCapacity);
}

//最大容量
private static final int MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8;

//ArrayList.grow()
private void grow(int minCapacity) {
    int oldCapacity = elementData.length;
    //新长度时原来长度的 1.5 倍
    int newCapacity = oldCapacity + (oldCapacity >> 1);
    //新长度小于实际容量，就用实际容量
    if (newCapacity - minCapacity < 0)
        newCapacity = minCapacity;
    //新长度太大了，就用最大的容量
    if (newCapacity - MAX_ARRAY_SIZE > 0)
        newCapacity = hugeCapacity(minCapacity);
    //copy 成一个新数组
    elementData = Arrays.copyOf(elementData, newCapacity);
}
```

1. 扩容的大小为原长度+1/2的原长度
2. 如果扩容长度比传入的最小容量小，则使用最小容量，如果扩容长度超过设定的最大容量，则实际用最大容量
3. 初始化默认长度为10，当添加到11个长度时，容量为15
<a name="KOHLC"></a>
#### `add(int index, E element)`
将元素插入到指定的位置，主要的操作是将指定位置之后的元素往后移动一个位置，空出 index 位置。平均时间复杂度为 O(n)。
```java
//ArrayList.add(int index, E element)
public void add(int index, E element) {
    //index的越界检查
    rangeCheckForAdd(index);
    //扩容
    ensureCapacityInternal(size + 1); 
    //将 index 之后的所有元素 copy 到往后挪动一位
    System.arraycopy(elementData, index, elementData, index + 1,
                        size - index);

    //将 index 位置放入新元素
    elementData[index] = element;
    //数量 + 1
    size++;
}
```
<a name="oD1Ns"></a>
#### `get()`
`get()` 应该是第二个常用的函数，可以返回随机位置的元素。需要注意的是越界时，超过容量返回的是 `IndexOutOfBoundsException` 异常，index 太小返回的是 `ArrayIndexOutOfBoundsException` 异常。平均时间复杂度为 O(1)。
```java
//ArrayList.get()
public E get(int index) {
    //index 越界检查
    rangeCheck(index);
    //返回指定位置的元素
    return elementData(index);
}

//ArrayList.rangeCheck()
private void rangeCheck(int index) {
    if (index >= size)
        throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
}
//ArrayList.elementData()
E elementData(int index) {
    return (E) elementData[index];
}
```
<a name="HLAUU"></a>
#### `remove()`
删除指定位置的元素，并返回删除的元素。平均时间复杂度为 O(n)。
```java
//ArrayList.remove()
public E remove(int index) {
    //越界检查
    rangeCheck(index);

    modCount++;
    //取出元素
    E oldValue = elementData(index);
    //拷贝数组，将 index 之后的元素往前移动一个位置
    int numMoved = size - index - 1;
    if (numMoved > 0)
        System.arraycopy(elementData, index+1, elementData, index,
                            numMoved);
    //最后一个元素置 null
    elementData[--size] = null; 

    return oldValue;
}
```
<a name="okqpD"></a>
#### `remove(Object o)`
删除指定的元素，需要循环数组删除第一个指定的元素。平均时间复杂度为 O(n)。
```java
//ArrayList.remove(Object o)
public boolean remove(Object o) {
    if (o == null) {
        //循环整个数组，找出第一个需要删除的元素
        for (int index = 0; index < size; index++)
            if (elementData[index] == null) {
                fastRemove(index);
                return true;
            }
    } else {
        //循环整个数组，找出第一个需要删除的元素
        for (int index = 0; index < size; index++)
            if (o.equals(elementData[index])) {
                fastRemove(index);
                return true;
            }
    }
    return false;
}

//ArrayList.fastRemove
private void fastRemove(int index) {
    modCount++;
    int numMoved = size - index - 1;
    if (numMoved > 0)
        System.arraycopy(elementData, index+1, elementData, index,
                            numMoved);
    elementData[--size] = null; 
}
```
<a name="ES3Ja"></a>
### 总结
![2021-09-04-13-14-17-090662.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732470414-88fd95d4-dea4-4ac0-afa5-c816a20aac21.png#averageHue=%232f2f2e&clientId=u7977b876-2566-4&from=ui&id=u0c7887c4&originHeight=387&originWidth=760&originalType=binary&ratio=1&rotation=0&showTitle=false&size=16943&status=done&style=none&taskId=u74ea768f-ddb8-474b-8442-9c4e9895b8c&title=)

1. ArrayList 内部用一个数组存储元素，容量不够时会扩容原来的一半。
2. ArrayList 实现了 `RandomAccess`，支持随机访问。
3. ArrayList 实现了 `Cloneable`，支持被拷贝克隆。
4. ArrayList 删除指定元素和指定位置上的元素的效率不高，需要遍历数组。
