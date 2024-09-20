JavaBigDecimal<br />提起BigDecimal，相信大家都使用过，之所以总结这篇呢，是因为最近发现项目中使用的不是太规范，在某些场景下甚至出现代码抛出异常的情况，<br />所以就总结了这篇，希望大家在使用时，可以少踩一些坑。
<a name="MDyNt"></a>
## 1、基本运算
<a name="vrioY"></a>
### 1.1 加法
```java
BigDecimal number1 = new BigDecimal("88.88");
BigDecimal number2 = new BigDecimal("11.12");

BigDecimal number3 = number1.add(number2);
System.out.println("number1 add number2 = " + number3);
```
输出结果：
> number1 add number2 = 100.00

<a name="IBR9J"></a>
### 1.2 减法
```java
BigDecimal number1 = new BigDecimal("88.88");
BigDecimal number2 = new BigDecimal("11.12");

BigDecimal number3 = number1.subtract(number2);
System.out.println("number1 subtract number2 = " + number3);
```

输出结果：
> number1 subtract number2 = 77.76

<a name="Gc75d"></a>
### 1.3 乘法
```java
BigDecimal number1 = new BigDecimal("88.88");
BigDecimal number2 = new BigDecimal("11.12");

BigDecimal number3 = number1.multiply(number2);
System.out.println("number1 multiply number2 = " + number3);
```

输出结果：
> number1 multiply number2 = 988.3456

<a name="cQqzj"></a>
### 1.4 除法
```java
BigDecimal number1 = new BigDecimal("88");
BigDecimal number2 = new BigDecimal("11");

BigDecimal number3 = number1.divide(number2);
System.out.println("number1 divide number2 = " + number3);
```
输出结果：
> number1 divide number2 = 8

因为上面2个数可以整除，所以这么用没有问题，不过一但不能被整除，这么用就会有潜在的风险，会抛出java.lang.ArithmeticException异常，所以**强烈建议像下面这样使用**：
```java
BigDecimal number1 = new BigDecimal("88.88");
BigDecimal number2 = new BigDecimal("11.12");

BigDecimal number3 = number1.divide(number2, 2, RoundingMode.HALF_UP);
System.out.println("number1 divide number2 = " + number3);
```
输出结果：
> number1 divide number2 = 7.99

此时使用的`divide()`方法源码如下所示：
```java
public BigDecimal divide(BigDecimal divisor, int scale, RoundingMode roundingMode) {
    return divide(divisor, scale, roundingMode.oldMode);
}
```
这里的scale指的是要保留的小数位数，传的是2，即保留2位小数。<br />这里的`roundingMode`指的是舍入模式，这里传的是`RoundingMode.HALF_UP`，即经常使用的四舍五入模式。
<a name="rNxWL"></a>
### 1.5 保留小数位数
如果想对`BigDecimal`类型保留小数位数，可以使用`setScale()`方法，使用方法如下所示：
```java
BigDecimal number1 = new BigDecimal("88.88");
BigDecimal number2 = new BigDecimal("11.12");

BigDecimal number3 = number1.multiply(number2);
System.out.println("number1 multiply number2 = " + number3);

// 保留3位小数,四舍五入
BigDecimal number4 = number3.setScale(3, RoundingMode.HALF_UP);
System.out.println("number3 setScale = " + number4);
```
输出结果：
> number1 multiply number2 = 988.3456
> number3 setScale = 988.346

<a name="bRhbb"></a>
### 1.6 比较大小
`BigDecimal`比较大小，可以使用`compareTo()`方法，使用方法如下所示：
```java
BigDecimal number1 = new BigDecimal("88.88");
BigDecimal number2 = new BigDecimal("11.11");
BigDecimal number3 = new BigDecimal("99.99");
BigDecimal number4 = new BigDecimal("88.88");

System.out.println("number1 compareTo number2 = " + number1.compareTo(number2));
System.out.println("number1 compareTo number3 = " + number1.compareTo(number3));
System.out.println("number1 compareTo number4 = " + number1.compareTo(number4));
```
输出结果：
> number1 compareTo number2 = 1
> number1 compareTo number3 = -1
> number1 compareTo number4 = 0

由输出结果可以看出：<br />当number1小于number2时，返回-1，<br />当number1等于number2时，返回0，<br />当number1大于number2时，返回1。
<a name="mRsfh"></a>
## 2、踩坑总结
<a name="udatk"></a>
### 2.1 NullPointerException异常
在使用BigDecimal类型进行计算时，比如上面提到的加、减、乘、除、比较大小时，一定要保证参与计算的两个值不能为空，否则会抛出java.lang.NullPointerException异常。<br />比如下面的2段代码，都会抛出异常：
```java
BigDecimal number1 = null;
BigDecimal number2 = new BigDecimal("11.12");

BigDecimal number3 = number1.add(number2);
System.out.println("number1 add number2 = " + number3);
```
```java
BigDecimal number1 = new BigDecimal("88.88");
BigDecimal number2 = null;

BigDecimal number3 = number1.add(number2);
System.out.println("number1 add number2 = " + number3);
```
抛出的异常如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669641005311-d3ba7c1d-ff09-43c4-850d-1433bed7f0b6.png#averageHue=%23372e2d&clientId=uf5b2983c-0d5a-4&from=paste&id=u00e284e9&originHeight=129&originWidth=966&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucc7e861c-5383-4fd8-ba3c-05bf071da08&title=)
<a name="iojb8"></a>
### 2.2 ArithmeticException异常
一次在使用`BigDecimal`的`divide`方法时，抛出java.lang.ArithmeticException异常，错误代码如下所示：
```java
// 含税金额
BigDecimal inclusiveTaxAmount = new BigDecimal("1000");
// 税率
BigDecimal taxRate = new BigDecimal("0.13");
// 不含税金额 = 含税金额 / (1+税率)
BigDecimal exclusiveTaxAmount = inclusiveTaxAmount.divide(BigDecimal.ONE.add(taxRate));

System.out.println(exclusiveTaxAmount);
```
运行时抛出以下异常：<br />java.lang.ArithmeticException: Non-terminating decimal expansion; no exact representable decimal result.<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669641005344-fa4e5213-9dfc-4249-a28d-3f371615b753.png#averageHue=%23352c2c&clientId=uf5b2983c-0d5a-4&from=paste&id=u4c6b52f8&originHeight=153&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uebc28ab9-aded-4249-8ace-6bb2bb153a0&title=)<br />报错原因是因为无法整除，导致结果是无限循环小数：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669641005229-2878faa9-2006-48e2-84a9-66a546c999b3.png#averageHue=%23e1edf5&clientId=uf5b2983c-0d5a-4&from=paste&id=ue220b643&originHeight=224&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8df76d1f-548a-483e-b9ec-53ba5e4b901&title=)<br />解决方案是指定下舍入模式，比如最常用的四舍五入模式：
```java
// 不含税金额 = 含税金额 / (1+税率)
BigDecimal exclusiveTaxAmount = inclusiveTaxAmount.divide(BigDecimal.ONE.add(taxRate),RoundingMode.HALF_UP);
```
此时不再报错，输出结果为：
> 885

但这里的需求是保留2位小数，四舍五入，因此代码应该是下面这样的：
```java
// 不含税金额 = 含税金额 / (1+税率)
BigDecimal exclusiveTaxAmount = inclusiveTaxAmount.divide(BigDecimal.ONE.add(taxRate), 2, RoundingMode.HALF_UP);
```
此时的输出结果为：
> 884.96

如果IDEA装了阿里巴巴代码规范插件，如果不指定`RoundingMode`，会有下面这样的提示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669641005336-34660dfe-22f9-453a-83ed-b9133dbd954e.png#averageHue=%237c7462&clientId=uf5b2983c-0d5a-4&from=paste&id=u630e62c8&originHeight=604&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u33795a2e-2240-4118-b3b3-a04f39c1a0f&title=)
<a name="npTcQ"></a>
### 2.3 BigDecimal转String，科学计数法问题
结论：将BigDecimal转换为String，推荐使用`toPlainString()`，而不是`toString()`<br />可能你的写法是下面这样的：
```java
BigDecimal amount = new BigDecimal("3450.67");
System.out.println(amount.toString());
```
输出结果：
> 3450.67

大部分情况下，这么使用并没有问题，不过在某些场景下，这么使用就容易踩坑，比如下面的3个示例：
```java
System.out.println( new BigDecimal("0.000000000000").toString());
```
输出结果：
> 0E-12

```java
BigDecimal bigDecimal = new BigDecimal("1E+11");
System.out.println(bigDecimal.toString());
```
输出结果：
> 1E+11

还有个更为常用的场景：抹零，也容易踩坑，比如下面所示的代码，预期的输出结果是3550，但实际上并不是：
```java
BigDecimal bigDecimal = new BigDecimal("3550.00");
System.out.println(bigDecimal.stripTrailingZeros().toString());
```
输出结果：
> 3.55E+3

使用`toPlainString()`方法可以避免这个问题，如下所示：
```java
System.out.println( new BigDecimal("0.000000000000").toPlainString());
System.out.println( new BigDecimal("1E+11").toPlainString());
System.out.println(new BigDecimal("3550.00").stripTrailingZeros().toPlainString());
```
输出结果：
> 0.000000000000
> 100000000000
> 3550

其实，`BigDecimal`提供了3个转换为String的方法，分别为：

1. `toString()` 某些场景下使用科学计数法

上面的踩坑就是因为toString()在某些场景下使用科学计数法的原因

2. `toPlainString()` 不使用任何计数法
3. `toEngineeringString()` 某些场景下使用工程计数法

这里简单提下科学计数法和工程计数法的区别：<br />科学记数法，是将数字表示成10的幂的倍数的形式。<br />工程记数法，是在科学记数法基础上，将10的幂限制为3的倍数。<br />举例：

| 原始值 | 科学技术法 | 工程计数法 |
| --- | --- | --- |
| 2700 | 2.7 x 10³ | 2.7 x 10³ |
| 27000 | 2.7 x 10⁴ | 27 x 10³ |
| 270000 | 2.7 x 10⁵ | 270 x 10³ |
| 2700000 | 2.7 x 10⁶ | 2.7 x 10⁶ |

示例代码：
```java
BigDecimal bigDecimal = new BigDecimal("270000.00").stripTrailingZeros();
System.out.println(bigDecimal.toString());
System.out.println(bigDecimal.toPlainString());
System.out.println(bigDecimal.toEngineeringString());
```
输出结果：
> 2.7E+5
> 270000
> 270E+3

<a name="lpOBP"></a>
## 3、使用规范
尽量不要在项目中使用`new BigDecimal("0")`，而是使用`BigDecimal`提供的常量`BigDecimal.ZERO`。
```java
BigDecimal zero = BigDecimal.ZERO;
BigDecimal one = BigDecimal.ONE;
BigDecimal ten = BigDecimal.TEN;
```
