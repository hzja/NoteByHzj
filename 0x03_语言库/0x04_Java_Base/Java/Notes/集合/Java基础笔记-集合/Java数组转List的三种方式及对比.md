Java 数组 集合List
<a name="Qo0FX"></a>
## 一、最常见方式（未必最佳）
通过 `Arrays.asList(strArray)` 方式,将数组转换List后，不能对List增删，只能查改，否则抛异常。<br />关键代码：`List list = Arrays.asList(strArray);`
```java
private void testArrayCastToListError() {
    String[] strArray = new String[2];
    List list = Arrays.asList(strArray);
    //对转换后的list插入一条数据
    list.add("1");
    System.out.println(list);
}
```
执行结果：
```java
Exception in thread "main" java.lang.UnsupportedOperationException
 at java.util.AbstractList.add(AbstractList.java:148)
 at java.util.AbstractList.add(AbstractList.java:108)
 at com.darwin.junit.Calculator.testArrayCastToList(Calculator.java:19)
 at com.darwin.junit.Calculator.main(Calculator.java:44)
```
程序在list.add("1")处，抛出异常：UnsupportedOperationException。<br />原因解析：<br />`Arrays.asList(strArray)`返回值是`java.util.Arrays`类中一个私有静态内部类`java.util.Arrays.ArrayList`，它并非`java.util.ArrayList`类。`java.util.Arrays.ArrayList`类具有 set()，get()，contains()等方法，但是不具有添加`add()`或删除`remove()`方法,所以调用`add()`方法会报错。<br />使用场景：`Arrays.asList(strArray)`方式仅能用在将数组转换为List后，不需要增删其中的值，仅作为数据源读取使用。
<a name="grUi2"></a>
## 二、数组转为List后，支持增删改查的方式
通过ArrayList的构造器，将`Arrays.asList(strArray)`的返回值由`java.util.Arrays.ArrayList`转为`java.util.ArrayList`。<br />关键代码：`ArrayList<String> list = new ArrayList<String>(Arrays.asList(strArray)) ;`
```java
private void testArrayCastToListRight() {
    String[] strArray = new String[2];
    ArrayList<String> list = new ArrayList<String>(Arrays.asList(strArray)) ;
    list.add("1");
    System.out.println(list);
}
```
执行结果：成功追加一个元素“1”。
```java
[null, null, 1]
```
使用场景：需要在将数组转换为List后，对List进行增删改查操作，在List的数据量不大的情况下，可以使用。
<a name="TOhVO"></a>
## 三、通过集合工具类`Collections.addAll()`方法(最高效)
通过`Collections.addAll(arrayList, strArray)`方式转换，根据数组的长度创建一个长度相同的List，然后通过`Collections.addAll()`方法，将数组中的元素转为二进制，然后添加到List中，这是最高效的方法。<br />关键代码：
```java
ArrayList< String> arrayList = new ArrayList<String>(strArray.length);
Collections.addAll(arrayList, strArray);
```
测试：
```java
private void testArrayCastToListEfficient(){
    String[] strArray = new String[2];
    ArrayList< String> arrayList = new ArrayList<String>(strArray.length);
    Collections.addAll(arrayList, strArray);
    arrayList.add("1");
    System.out.println(arrayList);
}
```
执行结果：同样成功追加一个元素“1”。
```java
[null, null, 1]
```
使用场景：需要在将数组转换为List后，对List进行增删改查操作，在List的数据量巨大的情况下，优先使用，可以提高操作速度。<br />注：附上`Collections.addAll()`方法源码：
```java
public static <T> boolean addAll(Collection<? super T> c, T... elements) {
    boolean result = false;
    for (T element : elements)
        result |= c.add(element);//result和c.add(element)按位或运算,然后赋值给result
    return result;
}
```
<a name="FMO8g"></a>
## 四、数组类型如果是整型数组，转为List时，会报错？
答案： 在`JDK1.8`环境中测试，这三种转换方式是没有问题的。对于`Integer[]`整型数组转List的方法和测试结果如下：

1. 方式一：不支持增删
```java
Integer[] intArray1 = new Integer[2];
List<Integer> list1 = Arrays.asList(intArray1);
System.out.println(list1);
```
运行结果：
```java
[null, null]
```

2. 方式二：支持增删
```java
Integer[] intArray2 = new Integer[2];
List<Integer> list2 = new ArrayList<Integer>(Arrays.asList(intArray2)) ;
list2.add(2);
System.out.println(list2);
```
运行结果：
```java
[null, null, 2]
```

3. 方式三：支持增删，且数据量大最高效
```java
Integer[] intArray3 = new Integer[2];
List<Integer> list3 = new ArrayList<Integer>(intArray3.length);
Collections.addAll(list3, intArray3);
list3.add(3);
System.out.println(list3);
```
运行结果：
```java
[null, null, 3]
```
综上，整型`Integer[]`数组转`List<Integer>`的正确方式应该是这样的。<br />易错点：可能出现的错误可能是这样转换的：
```java
int[] intArray1 = new int[2];
List<Integer> list1 = Arrays.asList(intArray1);//此处报错！！！
```
报错原因：等号两边类型不一致，编译不通过。<br />那么在声明数组时，用`int[]` 还是`Integer[]`，哪种声明方式才能正确的转为`List`呢？<br />答案： 只能用`Integer[]`转`List<Integer>`，即只能用基本数据类型的包装类型，才能直接转为`List`。<br />原因分析如下：<br />来看`List`在Java源码中的定义：
```java
public interface List<E> extends Collection<E> {省略…}
```
再来看`Arrays.asList()`的在Java源码定义：
```java
public static <T> List<T> asList(T... a) {
    return new ArrayList<>(a);
}
```

- 从上述源码中可以看出，`List`声明时，需要传递一个泛型`<E>`作为形参，`asList()`参数类型也是泛型中的通配类型`<T>`。Java中所有的泛型必须是引用类型。
- 什么是引用类型？`Integer`是引用类型， `int`是基本数据类型，不是引用类型。这就是为什么java中没有`List<int>`，而只有`List<Integer>`。
- 举一反三：其他8种基本数据类型`byte、short、int、long、float、double、char`也都不是引用类型，所以8种基本数据类型都不能作为List的形参。但`String、数组、class、interface`是引用类型，都可以作为List的形参，所以存在`List<Runnable>`接口类型的集合、`List<int[]>`数组类型的集合、`List<String>`类的集合。但不存在`list<byte>`、`list<short>` 等基本类型的集合。

有了上述基础知识后，再来看为什么下面两行代码第二行能编译通过，第三行却编译报错？
```java
int[] intArray1 = new int[1]; 
Arrays.asList(intArray1);//编译不报错
List<Integer> list1 = Arrays.asList(intArray1);//编译报错
```
答案：

- 第二行代码，`Arrays.asList()`方法的入参是个引用类型的`int[]`,那么返回值类型一定是`List<int[]>` ,其完整代码是：`List<int[]> intsArray = Arrays.asList(intArray1);`，所以编译通过，没问题。
- 第三行报错，因为等号两边的类型不一致，左边：`List<Integer>`,右边`List<int[]>`，所以编译时就报错。
<a name="zcN4m"></a>
## 总结
现在应该明白，为什么`int[]`不能直接转换为`List<Integer>`，而`Integer[]`就可以转换为`List<Integer>`了吧。因为`List`中的泛型必须是引用类型，`int`是基本数据类型，不是引用类型，但`int`的包装类型`Integer`是`class`类型，属于引用类型，所以`Integer`可以作为`List`形参，`List<Integer>`在java中是可以存在的，但不存在`List<int>`类型。
