JavaBigDecimal<br />`BigDecimal` 是大厂 Java 面试常问的一个知识点。<br />《阿里巴巴 Java 开发手册》中提到：“为了避免精度丢失，可以使用 `BigDecimal` 来进行浮点数的运算”。<br />浮点数的运算竟然还会有精度丢失的风险吗？确实会！<br />示例代码：
```java
float a = 2.0f - 1.9f;
float b = 1.8f - 1.7f;
System.out.println(a);// 0.100000024
System.out.println(b);// 0.099999905
System.out.println(a == b);// false
```
**为什么浮点数 float 或 double 运算的时候会有精度丢失的风险呢？**<br />这个和计算机保存浮点数的机制有很大关系。都知道计算机是二进制的，而且计算机在表示一个数字时，宽度是有限的，无限循环的小数存储在计算机时，只能被截断，所以就会导致小数精度发生损失的情况。这也就是解释了为什么浮点数没有办法用二进制精确表示。<br />就比如说十进制下的 0.2 就没办法精确转换成二进制小数：
```java
// 0.2 转换为二进制数的过程为，不断乘以 2，直到不存在小数为止，
// 在这个计算过程中，得到的整数部分从上到下排列就是二进制的结果。
0.2 * 2 = 0.4 -> 0
0.4 * 2 = 0.8 -> 0
0.8 * 2 = 1.6 -> 1
0.6 * 2 = 1.2 -> 1
0.2 * 2 = 0.4 -> 0（发生循环）
...
```
关于浮点数的更多内容，建议看一下[**计算机系统基础（四）浮点数**](http://kaito-kidd.com/2018/08/08/computer-system-float-point/)这篇文章。
<a name="CLEI9"></a>
## `BigDecimal` 介绍
`BigDecimal` 可以实现对浮点数的运算，不会造成精度丢失。<br />通常情况下，大部分需要浮点数精确运算结果的业务场景（比如涉及到钱的场景）都是通过 `BigDecimal` 来做的。<br />《阿里巴巴 Java 开发手册》中提到：**浮点数之间的等值判断，基本数据类型不能用 **`**==**`** 来比较，包装数据类型不能用 equals 来判断。**<br />具体原因在上面已经详细介绍了，这里就不多提了。<br />想要解决浮点数运算精度丢失这个问题，可以直接使用 `BigDecimal` 来定义浮点数的值，然后再进行浮点数的运算操作即可。
```java
BigDecimal a = new BigDecimal("1.0");
BigDecimal b = new BigDecimal("0.9");
BigDecimal c = new BigDecimal("0.8");

BigDecimal x = a.subtract(b);
BigDecimal y = b.subtract(c);

System.out.println(x.compareTo(y));// 0
```
<a name="d5OaI"></a>
## `BigDecimal` 常见方法
<a name="MYSdD"></a>
### 创建
在使用 `BigDecimal` 时，为了防止精度丢失，推荐使用它的`BigDecimal(String val)`构造方法或者 `BigDecimal.valueOf(double val)` 静态方法来创建对象。<br />《阿里巴巴 Java 开发手册》对这部分内容也有提到，如下图所示。
<a name="GNBkv"></a>
### 加减乘除
`add` 方法用于将两个 `BigDecimal` 对象相加，`subtract` 方法用于将两个 `BigDecimal` 对象相减。`multiply` 方法用于将两个 `BigDecimal` 对象相乘，`divide` 方法用于将两个 `BigDecimal` 对象相除。
```java
BigDecimal a = new BigDecimal("1.0");
BigDecimal b = new BigDecimal("0.9");
System.out.println(a.add(b));// 1.9
System.out.println(a.subtract(b));// 0.1
System.out.println(a.multiply(b));// 0.90
System.out.println(a.divide(b));// 无法除尽，抛出 ArithmeticException 异常
System.out.println(a.divide(b, 2, RoundingMode.HALF_UP));// 1.11
```
这里需要注意的是，在使用 `divide` 方法的时候尽量使用 3 个参数版本，并且`RoundingMode` 不要选择 `UNNECESSARY`，否则很可能会遇到 `ArithmeticException`（无法除尽出现无限循环小数的时候），其中 `scale` 表示要保留几位小数，`roundingMode` 代表保留规则。
```java
public BigDecimal divide(BigDecimal divisor, int scale, RoundingMode roundingMode) {
    return divide(divisor, scale, roundingMode.oldMode);
}
```
保留规则非常多，这里列举几种:
```java
public enum RoundingMode {
   // 2.5 -> 3 , 1.6 -> 2
   // -1.6 -> -2 , -2.5 -> -3
    UP(BigDecimal.ROUND_UP),
   // 2.5 -> 2 , 1.6 -> 1
   // -1.6 -> -1 , -2.5 -> -2
    DOWN(BigDecimal.ROUND_DOWN),
    // 2.5 -> 3 , 1.6 -> 2
   // -1.6 -> -1 , -2.5 -> -2
    CEILING(BigDecimal.ROUND_CEILING),
    // 2.5 -> 2 , 1.6 -> 1
   // -1.6 -> -2 , -2.5 -> -3
    FLOOR(BigDecimal.ROUND_FLOOR),
    // 2.5 -> 3 , 1.6 -> 2
   // -1.6 -> -2 , -2.5 -> -3
    HALF_UP(BigDecimal.ROUND_HALF_UP),
   //......
}
```
<a name="eiqJv"></a>
### 大小比较
`a.compareTo(b)` : 返回 -1 表示 a 小于 b，0 表示 a 等于 b ， 1 表示 a 大于 b。
```java
BigDecimal a = new BigDecimal("1.0");
BigDecimal b = new BigDecimal("0.9");
System.out.println(a.compareTo(b));// 1
```
<a name="eyhuy"></a>
### 保留几位小数
通过 `setScale`方法设置保留几位小数以及保留规则。保留规则有挺多种，不需要记，IDEA 会提示。
```java
BigDecimal m = new BigDecimal("1.255433");
BigDecimal n = m.setScale(3,RoundingMode.HALF_DOWN);
System.out.println(n);// 1.255
```
<a name="QELtG"></a>
## `BigDecimal` 等值比较问题
《阿里巴巴 Java 开发手册》中提到：<br />`BigDecimal` 使用 `equals()` 方法进行等值比较出现问题的代码示例：
```java
BigDecimal a = new BigDecimal("1");
BigDecimal b = new BigDecimal("1.0");
System.out.println(a.equals(b));//false
```
这是因为 `equals()` 方法不仅仅会比较值的大小（value）还会比较精度（scale），而 `compareTo()` 方法比较的时候会忽略精度。<br />1.0 的 scale 是 1，1 的 scale 是 0，因此 `a.equals(b)` 的结果是 false。<br />`compareTo()` 方法可以比较两个 `BigDecimal` 的值，如果相等就返回 0，如果第 1 个数比第 2 个数大则返回 1，反之返回-1。
```java
BigDecimal a = new BigDecimal("1");
BigDecimal b = new BigDecimal("1.0");
System.out.println(a.compareTo(b));//0
```
<a name="TH6uH"></a>
## 总结
浮点数没有办法用二进制精确表示，因此存在精度丢失的风险。<br />不过，Java 提供了`BigDecimal` 来操作浮点数。`BigDecimal` 的实现利用到了 `BigInteger` （用来操作大整数），所不同的是 `BigDecimal` 加入了小数位的概念。
<a name="L37T7"></a>
## 参考资料
计算机系统基础（四）浮点数: [http://kaito-kidd.com/2018/08/08/computer-system-float-point/](http://kaito-kidd.com/2018/08/08/computer-system-float-point/)
