Java StringJoiner
<a name="SloOd"></a>
### StringJoiner 基本使用
字面上理解就是用来拼接字符串的，一般需要分隔符进行拼接，如：
> hello, guys, AAA

这种三段式由 "," 分隔的字符串，在 Java 8 之前要使用 StringBuilder/ StringBuffer 进行拼接，如：
```java
StringBuilder sb = new StringBuilder();
sb.append("hello");
sb.append(",");
sb.append("guys");
sb.append(",");
sb.append("AAA");
String str = sb.toString();
```
都是相同的分隔符逗号，这样拼接显然过于傻瓜式，如果要拼接的字符串非常多，代码会十分难看，写完感觉要崩溃。<br />然而 Java 8 之后有了 `StringJoiner`，这件事件就变得更简单，分隔符这种全部一次性交给 StringJoiner 即可。
<a name="s1ty4"></a>
#### StringJoiner 基本使用示例
```java
public static void main(String[] args) {
    StringJoiner stringJoiner = new StringJoiner(",");
    stringJoiner.add("hello");
    stringJoiner.add("guys");
    stringJoiner.add("AAA");
    System.out.println(stringJoiner.toString());
}
```
输出：
> hello,guys,AAA

可以看到，这样写十分的干净清爽，也省去了许多没有必要的分隔符拼接操作，代码更优雅、只是可读性没 StringBuilder 直接拼接那么直观。** **
<a name="H4r0l"></a>
### StringJoiner 详细介绍
StringJoiner 的类结构图：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1606973503195-9218cdcf-004b-4071-af73-e9edce49fcbb.png#height=325&id=gTZlk&originHeight=976&originWidth=1039&originalType=binary&size=98646&status=done&style=shadow&width=346.3333333333333)<br />它的父类就是 Object，这些成员变量和方法都有什么用呢？
<a name="vULn2"></a>
#### 成员变量

- `prefix`：拼接后的字符串前缀
- `delimiter`：拼接时的字符串分隔符
- `suffix`：拼接后的字符串后缀
- `value`：拼接后的值
- `emptyValue`：空值的情况，value为 null 时返回
<a name="e4II5"></a>
#### 构造方法
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1606973595090-acc808b2-22b0-4fb2-ae42-220370666bee.png#height=331&id=jkS4G&originHeight=994&originWidth=2919&originalType=binary&size=349487&status=done&style=shadow&width=973)<br />提供了两个构造方法，一个必须带分隔符，另一个必须带分隔符、前缀、后缀，可以看了 `emptyValue` 默认为前缀+后缀组成。
<a name="0BAax"></a>
#### 公开方法

- `setEmptyValue`：设置空值
- `toString`：转换成 String
- `add`：添加字符串
- `merge`：从另一个 StringJoiner 合并
- `length`：长度（包括前缀后缀）
<a name="yCy6H"></a>
### 流式 API
重点看下 add 方法源码：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1606973686501-d5e10609-5c14-4220-bf49-685aef772bc2.png#height=321&id=xpZSp&originHeight=964&originWidth=2651&originalType=binary&size=209242&status=done&style=shadow&width=883.6666666666666)<br />可以看到内部其实就是用的 StringBuilder 进行封装的，首次创建会先拼接前缀，后续先添加分隔符，再添加字符串。<br />其实 String 底层也是用的 StringBuilder<br />另外一点，add 方法就是返回 StringJoiner 本身，所以可以像 StringBuilder/ StringBuffer 一样进行流式处理。<br />如上面的示例一行代码搞定：
```java
public static void main(String[] args) {
    StringJoiner stringJoiner = new StringJoiner(",").add("hello").add("guys").add("AAA");
    System.out.println(stringJoiner.toString());
}
```
<a name="lZwEP"></a>
### 前后缀拼接
在示例中需要指定前后缀：
```java
public static void main(String[] args) {
    StringJoiner stringJoiner = new StringJoiner(",", "[", "]");
    stringJoiner.add("hello");
    stringJoiner.add("guys");
    stringJoiner.add("AAA");
    System.out.println(stringJoiner.toString());
}
```
输出：
> [hello,guys,AAA]

如上所示，前后都带中括号进行了包装。
<a name="RHufa"></a>
### 空值处理
没有拼接任何字符串的几个空值处理场景。
<a name="vPomM"></a>
#### 输出空白字符串
```java
public static void main(String[] args) {
    StringJoiner stringJoiner = new StringJoiner(",");
    System.out.println(stringJoiner.toString());
}
```
<a name="AYKiB"></a>
#### 输出前后缀
```java
public static void main(String[] args) {
    StringJoiner stringJoiner = new StringJoiner(",", "[", "]");
}
```
输出：
> []

<a name="mTGxW"></a>
#### 输出指定字符串
通过 `setEmptyValue` 进行设置。
```java
public static void main(String[] args) {
 StringJoiner stringJoiner = new StringJoiner(",", "[", "]");
 stringJoiner.setEmptyValue("void");
 System.out.println(stringJoiner.toString());
}
```
输出：
> void

<a name="22I1u"></a>
### String.join()
`String.join()` 这是针对 StringJoiner 又封装了一层的 API，同样出自 Java 8，可以传入动态参数或者迭代器。

- java.lang.String#join(java.lang.CharSequence, java.lang.CharSequence...)
- java.lang.String#join(java.lang.CharSequence, java.lang.Iterable<? extends java.lang.CharSequence>)

源码如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1606973874857-06a24044-8ce1-49bc-9832-c05493460260.png#height=453&id=BCGcm&originHeight=1359&originWidth=2569&originalType=binary&size=356901&status=done&style=shadow&width=856.3333333333334)<br />看源码，这两个方法只能进行简单的拼接操作，不能添加前后缀、空值设置处理等。<br />来个示例：
```java
public static void main(String[] args) {
    String str = String.join(",", "hello", "guys", "AAA");
    System.out.println(str);
}
```
可以看到，简单处理这样使用更简便。
