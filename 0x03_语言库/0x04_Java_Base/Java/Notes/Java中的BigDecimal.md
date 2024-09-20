Java BigDecimal
<a name="22HZ5"></a>
## 1、经典问题：浮点数精度丢失
精度丢失的问题是在其他计算机语言中也都会出现，float和double类型的数据在执行二进制浮点运算的时候，并没有提供完全精确的结果。产生误差不在于数的大小，而是因为数的精度。
<a name="QDgty"></a>
### float和double
float 单精度浮点数在机内占 4 个字节，用 32 位二进制描述<br />double 双精度浮点数在机内占 8 个字节，用 64 位二进制描述<br />注意float型定义的数据末尾必须有"f"或"F"，为了和double区别<br />来写一段简单的程序来实验一下为什么它不行
```java
 System.out.println(2.0-1.4);
```
如果是有经验的开发人员，肯定觉得这么写出来是不是有问题？这直接减法减出来的数据应该不对，是的，结果肯定不对。
```java
0.6000000000000001
```
为什么运算结果有问题呢？那加法和乘法是不是都会有这种问题，确实会有这种问题，而这个问题，就得从计算机去开始讨论了，计算机并不能识别除了二进制数据以外的任何数据。也就是说，传递给计算机的是十进制的数据，但是计算机需要先把给的数据转换成二进制的数据，因为不能直接识别十进制的数据，这时候，2.0 是十进制的数据，转换成二进制的数据，而1.4呢？转换成二进制的数据反而出现了问题 1.4在二进制中，则是会出现1.399999。。。这样的数据，当进行数据转换的时候，就出现了2.0-1.399999这样的数据。<br />这个时候就有人问了，定义 float 类型为 1.4 的时候为什么不是 1.399999999呢？这就是不进行浮点计算的时候，在十进制里浮点数能正确显示。也就是说，如果知识定义了类型为 float 的话，但是不用这个数字去进行计算，那就没问题，但是一旦参与了运算，那就不行了。<br />这里简单讨论一下十进制数转二进制为什么会出现精度丢失的现象，十进制数分为整数部分和小数部分，分开来看看就知道原因为何：
<a name="DpZgj"></a>
### 十进制整数如何转化为二进制整数？
将被除数每次都除以2，只要除到商为0就可以停止这个过程。
```
5 / 2 = 2 余 1
2 / 2 = 1 余 0
1 / 2 = 0 余 1 
// 结果为 101
```
这个算法永远都不会无限循环，整数永远都可以使用二进制数精确表示，但小数呢？
<a name="M94BL"></a>
### 十进制小数如何转化为二进制数？
每次将小数部分乘2，取出整数部分，如果小数部分为0，就可以停止这个过程。
```
0.1 * 2 = 0.2 取整数部分0
0.2 * 2 = 0.4 取整数部分0
0.4 * 2 = 0.8 取整数部分0
0.8 * 2 = 1.6 取整数部分1
0.6 * 2 = 1.2 取整数部分1
0.2 * 2 = 0.4 取整数部分0 
//... 写到这就不必再写了，应该已经发现，
// 上面的过程已经开始循环，小数部分永远不能为0
```
这个算法有一定概率会存在无限循环，即无法用有限长度的二进制数表示十进制的小数，这就是精度丢失问题产生的原因。
<a name="iN2Y5"></a>
### 阿里手册定义
数据库 小数类型为 decimal，禁止使用 float 和 double。<br />在存储的时候，float 和 double 都存在精度损失的问题，很可能在比较值的时候，得到不正确的结果。如果存储的数据范围超过 decimal 的范围，建议将数据拆成整数和小数并分开存储。<br />Java程序：使用 BigDecimal 来定义值，再进行浮点数的运算操作<br />BigDecimal 是 Java 在 java.math 包中提供的API类，用来对超过16位有效位的数进行精确的运算
<a name="MkGST"></a>
## 2、`BigDecimal`概述
Java在java.math包中提供的API类`BigDecimal`，用来对超过16位有效位的数进行精确的运算。双精度浮点型变量double可以处理16位有效数，但在实际应用中，可能需要对更大或者更小的数进行运算和处理。<br />一般情况下，对于那些不需要准确计算精度的数字可以直接使用`Float`和`Double`处理，但是`Double.valueOf(String)` 和`Float.valueOf(String)`会丢失精度。所以开发中，如果需要精确计算的结果，则必须使用`BigDecimal`类来操作。<br />`BigDecimal`所创建的是对象，故不能使用传统的+、-、*、/等算术运算符直接对其对象进行数学运算，而必须调用其相对应的方法。方法中的参数也必须是`BigDecimal`的对象。构造器是类的特殊方法，专门用来创建对象，特别是带有参数的对象。
<a name="BVuFH"></a>
## 3、`BigDecimal`常用构造函数
<a name="4sQtX"></a>
### 3.1、常用构造函数

- `BigDecimal(int)`：创建一个具有参数所指定整数值的对象
- `BigDecimal(double)`：创建一个具有参数所指定双精度值的对象
- `BigDecimal(long)`：创建一个具有参数所指定长整数值的对象
- `BigDecimal(String)`：创建一个具有参数所指定以字符串表示的数值的对象
<a name="HiDVK"></a>
### 3.2、使用问题分析
使用示例：
```java
BigDecimal a =new BigDecimal(0.1);
System.out.println("a values is:"+a);
System.out.println("=====================");
BigDecimal b =new BigDecimal("0.1");
System.out.println("b values is:"+b);
```
结果示例：
```
a values is:0.1000000000000000055511151231257827021181583404541015625
=====================
b values is:0.1
```
原因分析：<br />1）参数类型为double的构造方法的结果有一定的不可预知性。有人可能认为在Java中写入`new BigDecimal(0.1)`所创建的`BigDecimal`正好等于 0.1（非标度值 1，其标度为 1），但是它实际上等于0.1000000000000000055511151231257827021181583404541015625。这是因为0.1无法准确地表示为 double（或者说对于该情况，不能表示为任何有限长度的二进制小数）。这样，传入到构造方法的值不会正好等于 0.1（虽然表面上等于该值）。<br />2）String 构造方法是完全可预知的：写入 `new BigDecimal("0.1")` 将创建一个 `BigDecimal`，它正好等于预期的 0.1。因此，比较而言， 通常建议优先使用String构造方法。<br />3）当double必须用作`BigDecimal`的源时，请注意，此构造方法提供了一个准确转换；它不提供与以下操作相同的结果：先使用`Double.toString(double)`方法，然后使用`BigDecimal(String)`构造方法，将double转换为String。要获取该结果，请使用`static valueOf(double)`方法。<br />其实在点击到这个方法看源码的时候，注释都提醒慎重使用了。
```java
 * The results of this constructor can be somewhat unpredictable.     这个构造函数可以有些不可预测的结果
 * One might assume that writing {@code new BigDecimal(0.1)} in
 * Java creates a {@code BigDecimal} which is exactly equal to
 * 0.1 (an unscaled value of 1, with a scale of 1), but it is
 * actually equal to
 * 0.1000000000000000055511151231257827021181583404541015625.
 * This is because 0.1 cannot be represented exactly as a
 * {@code double} (or, for that matter, as a binary fraction of
 * any finite length).  Thus, the value that is being passed
 * <i>in</i> to the constructor is not exactly equal to 0.1,
 * appearances notwithstanding.
```
也就是说存在精度损失风险，在精确计算或值比较的场景中可能会导致业务逻辑异常
<a name="i6a6c"></a>
## 4、BigDecimal常用方法详解
<a name="svhWH"></a>
### 4.1、常用方法

- `add(BigDecimal)`：`BigDecimal`对象中的值相加，返回`BigDecimal`对象
- `subtract(BigDecimal)`：`BigDecimal`对象中的值相减，返回`BigDecimal`对象
- `multiply(BigDecimal)`：`BigDecimal`对象中的值相乘，返回`BigDecimal`对象
- `divide(BigDecimal)`：`BigDecimal`对象中的值相除，返回`BigDecimal`对象
- `toString()`：将`BigDecimal`对象中的值转换成字符串
- `doubleValue()`：将`BigDecimal`对象中的值转换成双精度数
- `floatValue()`：将`BigDecimal`对象中的值转换成单精度数
- `longValue()`：将`BigDecimal`对象中的值转换成长整数
- `intValue()`：将`BigDecimal`对象中的值转换成整数
<a name="o8TI2"></a>
### 4.2、`BigDecimal`大小比较
Java中对`BigDecimal`比较大小一般用的是bigdemical的`compareTo`方法
```java
int a = bigdemical.compareTo(bigdemical2)
```
返回结果分析：
```java
a = -1,表示bigdemical小于bigdemical2；
a = 0,表示bigdemical等于bigdemical2；
a = 1,表示bigdemical大于bigdemical2；
```
举例：a大于等于b
```java
new bigdemica(a).compareTo(new bigdemical(b)) >= 0
```
<a name="UynNu"></a>
### 4.3、`BigDecimal`保留小数点问题
`ROUND_DOWN`：向零方向舍入<br />`ROUND_UP`：向远离0的方向舍入<br />`ROUND_CEILING`：向正无穷方向舍入<br />`ROUND_FLOOR`：向负无穷方向舍入<br />`ROUND_HALF_DOWN`：相当于五舍六入<br />`ROUND_HALF_UP`：相当于四舍五入(经常使用)
<a name="xUCVv"></a>
## 5、`BigDecimal`格式化
由于`NumberFormat`类的`format()`方法可以使用`BigDecimal`对象作为其参数，可以利用`BigDecimal`对超出16位有效数字的货币值，百分值，以及一般数值进行格式化控制。<br />以利用`BigDecimal`对货币和百分比格式化为例。首先，创建`BigDecimal`对象，进行`BigDecimal`的算术运算后，分别建立对货币和百分比格式化的引用，最后利用`BigDecimal`对象作为`format()`方法的参数，输出其格式化的货币值和百分比。
```java
NumberFormat currency = NumberFormat.getCurrencyInstance(); //建立货币格式化引用 
NumberFormat percent = NumberFormat.getPercentInstance();  //建立百分比格式化引用 
percent.setMaximumFractionDigits(3); //百分比小数点最多3位 
BigDecimal loanAmount = new BigDecimal("15000.48"); //贷款金额
BigDecimal interestRate = new BigDecimal("0.008"); //利率   
BigDecimal interest = loanAmount.multiply(interestRate); //相乘
System.out.println("贷款金额:\t" + currency.format(loanAmount)); 
System.out.println("利率:\t" + percent.format(interestRate)); 
System.out.println("利息:\t" + currency.format(interest));
```
结果：
```
贷款金额: ￥15,000.48 利率: 0.8% 利息: ￥120.00
```
`BigDecimal`格式化保留2为小数，不足则补0：
```java
public class NumberFormat {
    public static void main(String[] s){
        System.out.println(formatToNumber(new BigDecimal("3.435")));
        System.out.println(formatToNumber(new BigDecimal(0)));
        System.out.println(formatToNumber(new BigDecimal("0.00")));
        System.out.println(formatToNumber(new BigDecimal("0.001")));
        System.out.println(formatToNumber(new BigDecimal("0.006")));
        System.out.println(formatToNumber(new BigDecimal("0.206")));
    }
    /**
     * @desc 1.0~1之间的BigDecimal小数，格式化后失去前面的0,则前面直接加上0。
     * 2.传入的参数等于0，则直接返回字符串"0.00"
     * 3.大于1的小数，直接格式化返回字符串
     * @param obj传入的小数
     * @return
     */
    public static String formatBigDecimalToStr(BigDecimal obj) {
        DecimalFormat df = new DecimalFormat("#.00");
        if(obj.compareTo(BigDecimal.ZERO)==0) {
            return "0.00";
        }else if(obj.compareTo(BigDecimal.ZERO)>0&&obj.compareTo(new BigDecimal(1))<0){
            return "0"+df.format(obj).toString();
        }else {
            return df.format(obj).toString();
        }
    }
    
    // 简化版
    public static String formatBigDecimalToStr(BigDecimal premium) {
        final DecimalFormat decimalFormat = new DecimalFormat("#0.00");
        return decimalFormat.format(premium);
    }
    
}
```
结果为：
```
3.44
0.00
0.00
0.00
0.01
0.21
```
<a name="5J3xi"></a>
## 6、`BigDecimal`常见异常
<a name="SjZ6C"></a>
### 6.1、除法的时候出现异常
`java.lang.ArithmeticException: Non-terminating decimal expansion; no exact representable decimal result`<br />原因分析：<br />通过`BigDecimal`的`divide`方法进行除法时当不整除，出现无限循环小数时，就会抛异常：
```java
java.lang.ArithmeticException: Non-terminating decimal expansion; no exact representable decimal result.
```
解决方法：
> `divide`方法设置精确的小数点，如：`divide(xxxxx,2)`

<a name="riUTX"></a>
## 7、BigDecimal总结
<a name="bFCGn"></a>
### 7.1、总结
在需要精确的小数计算时再使用`BigDecimal`，`BigDecimal`的性能比`double`和`float`差，在处理庞大，复杂的运算时尤为明显。故一般精度的计算没必要使用`BigDecimal`。<br />尽量使用参数类型为`String`的构造函数。<br />`BigDecimal`都是不可变的（immutable）的， 在进行每一次四则运算时，都会产生一个新的对象 ，所以在做加减乘除运算时要记得要保存操作后的值。
<a name="TMfoP"></a>
### 7.2、工具类推荐
```java
package com.vivo.ars.util;
import java.math.BigDecimal;
/**
 * 用于高精确处理常用的数学运算
 */
public class ArithmeticUtils {
    //默认除法运算精度
    private static final int DEF_DIV_SCALE = 10;
    /**
     * 提供精确的加法运算
     *
     * @param v1 被加数
     * @param v2 加数
     * @return 两个参数的和
     */
    public static double add(double v1, double v2) {
        BigDecimal b1 = new BigDecimal(Double.toString(v1));
        BigDecimal b2 = new BigDecimal(Double.toString(v2));
        return b1.add(b2).doubleValue();
    }
    /**
     * 提供精确的加法运算
     *
     * @param v1 被加数
     * @param v2 加数
     * @return 两个参数的和
     */
    public static BigDecimal add(String v1, String v2) {
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v2);
        return b1.add(b2);
    }
    /**
     * 提供精确的加法运算
     *
     * @param v1    被加数
     * @param v2    加数
     * @param scale 保留scale 位小数
     * @return 两个参数的和
     */
    public static String add(String v1, String v2, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException(
                    "The scale must be a positive integer or zero");
        }
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v2);
        return b1.add(b2).setScale(scale, BigDecimal.ROUND_HALF_UP).toString();
    }
    /**
     * 提供精确的减法运算
     *
     * @param v1 被减数
     * @param v2 减数
     * @return 两个参数的差
     */
    public static double sub(double v1, double v2) {
        BigDecimal b1 = new BigDecimal(Double.toString(v1));
        BigDecimal b2 = new BigDecimal(Double.toString(v2));
        return b1.subtract(b2).doubleValue();
    }
    /**
     * 提供精确的减法运算。
     *
     * @param v1 被减数
     * @param v2 减数
     * @return 两个参数的差
     */
    public static BigDecimal sub(String v1, String v2) {
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v2);
        return b1.subtract(b2);
    }
    /**
     * 提供精确的减法运算
     *
     * @param v1    被减数
     * @param v2    减数
     * @param scale 保留scale 位小数
     * @return 两个参数的差
     */
    public static String sub(String v1, String v2, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException(
                    "The scale must be a positive integer or zero");
        }
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v2);
        return b1.subtract(b2).setScale(scale, BigDecimal.ROUND_HALF_UP).toString();
    }
    /**
     * 提供精确的乘法运算
     *
     * @param v1 被乘数
     * @param v2 乘数
     * @return 两个参数的积
     */
    public static double mul(double v1, double v2) {
        BigDecimal b1 = new BigDecimal(Double.toString(v1));
        BigDecimal b2 = new BigDecimal(Double.toString(v2));
        return b1.multiply(b2).doubleValue();
    }
    /**
     * 提供精确的乘法运算
     *
     * @param v1 被乘数
     * @param v2 乘数
     * @return 两个参数的积
     */
    public static BigDecimal mul(String v1, String v2) {
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v2);
        return b1.multiply(b2);
    }
    /**
     * 提供精确的乘法运算
     *
     * @param v1    被乘数
     * @param v2    乘数
     * @param scale 保留scale 位小数
     * @return 两个参数的积
     */
    public static double mul(double v1, double v2, int scale) {
        BigDecimal b1 = new BigDecimal(Double.toString(v1));
        BigDecimal b2 = new BigDecimal(Double.toString(v2));
        return round(b1.multiply(b2).doubleValue(), scale);
    }
    /**
     * 提供精确的乘法运算
     *
     * @param v1    被乘数
     * @param v2    乘数
     * @param scale 保留scale 位小数
     * @return 两个参数的积
     */
    public static String mul(String v1, String v2, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException(
                    "The scale must be a positive integer or zero");
        }
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v2);
        return b1.multiply(b2).setScale(scale, BigDecimal.ROUND_HALF_UP).toString();
    }
    /**
     * 提供（相对）精确的除法运算，当发生除不尽的情况时，精确到
     * 小数点以后10位，以后的数字四舍五入
     *
     * @param v1 被除数
     * @param v2 除数
     * @return 两个参数的商
     */
    public static double div(double v1, double v2) {
        return div(v1, v2, DEF_DIV_SCALE);
    }
    /**
     * 提供（相对）精确的除法运算。当发生除不尽的情况时，由scale参数指
     * 定精度，以后的数字四舍五入
     *
     * @param v1    被除数
     * @param v2    除数
     * @param scale 表示表示需要精确到小数点以后几位。
     * @return 两个参数的商
     */
    public static double div(double v1, double v2, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException("The scale must be a positive integer or zero");
        }
        BigDecimal b1 = new BigDecimal(Double.toString(v1));
        BigDecimal b2 = new BigDecimal(Double.toString(v2));
        return b1.divide(b2, scale, BigDecimal.ROUND_HALF_UP).doubleValue();
    }
    /**
     * 提供（相对）精确的除法运算。当发生除不尽的情况时，由scale参数指
     * 定精度，以后的数字四舍五入
     *
     * @param v1    被除数
     * @param v2    除数
     * @param scale 表示需要精确到小数点以后几位
     * @return 两个参数的商
     */
    public static String div(String v1, String v2, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException("The scale must be a positive integer or zero");
        }
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v1);
        return b1.divide(b2, scale, BigDecimal.ROUND_HALF_UP).toString();
    }
    /**
     * 提供精确的小数位四舍五入处理
     *
     * @param v     需要四舍五入的数字
     * @param scale 小数点后保留几位
     * @return 四舍五入后的结果
     */
    public static double round(double v, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException("The scale must be a positive integer or zero");
        }
        BigDecimal b = new BigDecimal(Double.toString(v));
        return b.setScale(scale, BigDecimal.ROUND_HALF_UP).doubleValue();
    }
    /**
     * 提供精确的小数位四舍五入处理
     *
     * @param v     需要四舍五入的数字
     * @param scale 小数点后保留几位
     * @return 四舍五入后的结果
     */
    public static String round(String v, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException(
                    "The scale must be a positive integer or zero");
        }
        BigDecimal b = new BigDecimal(v);
        return b.setScale(scale, BigDecimal.ROUND_HALF_UP).toString();
    }
    /**
     * 取余数
     *
     * @param v1    被除数
     * @param v2    除数
     * @param scale 小数点后保留几位
     * @return 余数
     */
    public static String remainder(String v1, String v2, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException(
                    "The scale must be a positive integer or zero");
        }
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v2);
        return b1.remainder(b2).setScale(scale, BigDecimal.ROUND_HALF_UP).toString();
    }
    /**
     * 取余数  BigDecimal
     *
     * @param v1    被除数
     * @param v2    除数
     * @param scale 小数点后保留几位
     * @return 余数
     */
    public static BigDecimal remainder(BigDecimal v1, BigDecimal v2, int scale) {
        if (scale < 0) {
            throw new IllegalArgumentException(
                    "The scale must be a positive integer or zero");
        }
        return v1.remainder(v2).setScale(scale, BigDecimal.ROUND_HALF_UP);
    }
    /**
     * 比较大小
     *
     * @param v1 被比较数
     * @param v2 比较数
     * @return 如果v1 大于v2 则 返回true 否则false
     */
    public static boolean compare(String v1, String v2) {
        BigDecimal b1 = new BigDecimal(v1);
        BigDecimal b2 = new BigDecimal(v2);
        int bj = b1.compareTo(b2);
        boolean res;
        if (bj > 0)
            res = true;
        else
            res = false;
        return res;
    }
}
```

