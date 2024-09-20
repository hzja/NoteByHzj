为什么 ArrayList 内部使用 Object 而不是泛型 E？<br />这个问题在 StackOverflow 上很火，本文基于它上面的回答，翻译了这部分内容，分享给大家！
> [Why does the ArrayList implementation use Object[]?](https://stackoverflow.com/questions/25695011/)

<a name="zOzs8"></a>
## 源码实现
ArrayList 是个泛型类。
```java
public class ArrayList<E> extends AbstractList<E>
        implements List<E>, RandomAccess, Cloneable, java.io.Serializable
```
内部使用 Object 数组存储元素：
```java
transient Object[] elementData;
```
并在以下方法中将其转换为 E 类型。
```java
public E get(int index) {
    rangeCheck(index);
    return elementData(index);
}

E elementData(int index) {
    return (E) elementData[index];
}
```
<a name="pScqt"></a>
## 为什么不能使用`E[]`
Java 泛型要达到的效果：对于 `ArrayList<E>` 这样的类型，期望`ArrayList<String>`表现为：每次出现 E，则替换为 String 。换句话说以下代码：
```java
private Object[] elementData = new Object[size];

public E get(int i) {
    return (E) elementData[i];
}

String str = list.get(0);
```
应该变成这样：
```java
private Object[] elementData = new Object[size];

public String get(int i) {
    return (String) elementData[i];
}

String str = list.get(0);
```
但实际上并不是这样工作的。为了向后兼容，Java 泛型是通过类型删除实现的，其中 `E`实际上被替换为`Object`，并转换为 String。这意味着代码实际上变成了这样：
```java
private Object[] elementData = new Object[size];

public Object get(int i) {
    return elementData[i];
}

String str = (String) list.get(0);
```
投向(E)已经消失，并重新出现在调用站点。如果调用站点忽略了结果，则类型转换将完全消失！这就是它给出“未检查”警告的原因。<br />现在想象一下如果elementData有类型E[]，也就是说，代码如下所示：
```java
private E[] elementData = (E[]) new Object[size];

public E get(int i) {
    return elementData[i];
}

String str = list.get(0);
```
由于类型擦除，它会变成与上面相同的东西。但是如果像希望的那样具体化了泛型，它会看起来像这样：
```java
private String[] elementData = (String[]) new Object[size];
// ClassCastException: Object[] is not a String[]
```
本质上，已经编写了一些应该在运行时崩溃的代码。<br />好了，是时候抛出另外一个问题了。
> 问一个 jdk 设计的问题：为什么 hashmap 的 put 方法的 key 类型为泛型 K，而 get 方法的 key 类型为 Object？

