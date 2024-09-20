Java<br />都知道Double会丢失精度，BigDecimal就不会么？希望你会正确使用！<br />我们基本已经形成了常识，需要用到金钱的地方要用 BigDecimal 而不是其他，都知道浮点型变量在进行计算的时候会出现丢失精度的问题。<br />那么，你知道其实 BigDecimal 也会丢失精度吗？而使用 BigDecimal 的背后又有什么值得去探究的地方吗？知其然，也知其所以然。<br />如下一段代码：
```java
System.out.println(0.05 + 0.01);    
System.out.println(1.0 - 0.42);    
System.out.println(4.015 * 100);    
System.out.println(123.3 / 100);
```
> 输出：<br />0.060000000000000005<br />0.5800000000000001<br />401.49999999999994<br />1.2329999999999999

可以看到在 Java 中进行浮点数运算的时候，会出现丢失精度的问题。那么如果在进行商品价格计算的时候，就会出现问题。<br />很有可能造成手中有 0.06 元，却无法购买一个 0.05 元和一个 0.01 元的商品。<br />因为如上所示，他们两个的总和为 0.060000000000000005。<br />这无疑是一个很严重的问题，尤其是当电商网站的并发量上去的时候，出现的问题将是巨大的。可能会导致无法下单，或者对账出现问题。所以接下来就可以使用 Java 中的 BigDecimal 类来解决这类问题。<br />**普及一下：**<br />Java 中 float 的精度为 6-7 位有效数字。double 的精度为 15-16 位。
<a name="MHOZv"></a>
### API
构造器：
```java
构造器                   描述  
BigDecimal(int)       创建一个具有参数所指定整数值的对象。  
BigDecimal(double)    创建一个具有参数所指定双精度值的对象。  
BigDecimal(long)      创建一个具有参数所指定长整数值的对象。  
BigDecimal(String)    创建一个具有参数所指定以字符串表示的数值的对象。
```
函数：
```
方法                    描述  
add(BigDecimal)       BigDecimal对象中的值相加，然后返回这个对象。  
subtract(BigDecimal)  BigDecimal对象中的值相减，然后返回这个对象。  
multiply(BigDecimal)  BigDecimal对象中的值相乘，然后返回这个对象。  
divide(BigDecimal)    BigDecimal对象中的值相除，然后返回这个对象。  
toString()            将BigDecimal对象的数值转换成字符串。  
doubleValue()         将BigDecimal对象中的值以双精度数返回。  
floatValue()          将BigDecimal对象中的值以单精度数返回。  
longValue()           将BigDecimal对象中的值以长整数返回。  
intValue()            将BigDecimal对象中的值以整数返回。
```
由于一般的数值类型，例如 double 不能准确的表示 16 位以上的数字。
<a name="pkTAA"></a>
### BigDecimal 精度也丢失
在使用 BigDecimal 时，使用它的 `BigDecimal(String)` 构造器创建对象才有意义。其他的如 `BigDecimal b = new BigDecimal(1)` 这种，还是会发生精度丢失的问题。如下代码：
```java
BigDecimal a = new BigDecimal(1.01);  
BigDecimal b = new BigDecimal(1.02);  
BigDecimal c = new BigDecimal("1.01");  
BigDecimal d = new BigDecimal("1.02");  
System.out.println(a.add(b));  
System.out.println(c.add(d));
```
> 输出：<br />2.0300000000000000266453525910037569701671600341796875<br />2.03

可见论丢失精度 BigDecimal 显的更为过分。但是使用 Bigdecimal 的 `BigDecimal(String)` 构造器的变量在进行运算的时候却没有出现这种问题。<br />究其原因计算机组成原理里面都有，它们的编码决定了这样的结果。<br />long 可以准确存储 19 位数字，而 double 只能准备存储 16 位数字。<br />double 由于有 exp 位，可以存 16 位以上的数字，但是需要以低位的不精确作为代价。如果需要高于 19 位数字的精确存储，则必须用 BigInteger 来保存，当然会牺牲一些性能。<br />所以一般使用 BigDecimal 来解决商业运算上丢失精度的问题的时候，声明 BigDecimal 对象的时候一定要使用它构造参数为 String 的类型的构造器。<br />同时这个原则 Effective Java 和 MySQL 必知必会中也都有提及。float 和 double 只能用来做科学计算和工程计算。商业运算中要使用 BigDecimal。<br />而且从源码的注释中官方也给出了说明，如下是 BigDecimal 类的 double 类型参数的构造器上的一部分注释说明：
```java
/* The results of this constructor can be somewhat unpredictable.    
     * One might assume that writing {@codenew BigDecimal(0.1)} in    
     * Java creates a {@code BigDecimal} which is exactly equal to    
     * 0.1 (an unscaled value of 1, with a scale of 1), but it is    
     * actually equal to    
     * 0.1000000000000000055511151231257827021181583404541015625.    
     * This is because 0.1 cannot be represented exactly as a    
     * {@codedouble} (or, for that matter, as a binary fraction of    
     * any finite length).  Thus, the value that is being passed    
     * <i>in</i> to the constructor is not exactly equal to 0.1,    
     * appearances notwithstanding.    
       ……    
        * When a {@codedouble} must be used as a source for a    
     * {@code BigDecimal}, note that this constructor provides an    
     * exact conversion; it does not give the same result as    
     * converting the {@codedouble} to a {@code String} using the    
     * {@link Double#toString(double)} method and then using the    
     * {@link #BigDecimal(String)} constructor.  To get that result,    
     * use the {@codestatic} {@link #valueOf(double)} method.    
     * </ol>
     */
public BigDecimal(double val) {    
    this(val,MathContext.UNLIMITED);    
}
```
第一段也说的很清楚它只能计算的无限接近这个数，但是无法精确到这个数。<br />第二段则说，如果要想准确计算这个值，那么需要把 double 类型的参数转化为 String 类型的。并且使用 BigDecimal(String) 这个构造方法进行构造。去获取结果。
<a name="nFEa5"></a>
### 正确运用 BigDecimal
另外，BigDecimal 所创建的是对象，不能使用传统的 +、-、*、/ 等算术运算符直接对其对象进行数学运算，而必须调用其相对应的方法。方法中的参数也必须是 BigDecimal 的对象，由刚才所罗列的 API 也可看出。<br />在一般开发过程中，数据库中存储的数据都是 float 和 double 类型的。在进行拿来拿去运算的时候还需要不断的转化，这样十分的不方便。这里写了一个工具类：
```java
public class BigDecimalUtil {    

    private BigDecimalUtil() {    

    }    

    public static BigDecimal add(double v1, double v2) {// v1 + v2    
        BigDecimal b1 = new BigDecimal(Double.toString(v1));    
        BigDecimal b2 = new BigDecimal(Double.toString(v2));    
        return b1.add(b2);    
    }    

    public static BigDecimal sub(double v1, double v2) {    
        BigDecimal b1 = new BigDecimal(Double.toString(v1));    
        BigDecimal b2 = new BigDecimal(Double.toString(v2));    
        return b1.subtract(b2);    
    }    

    public static BigDecimal mul(double v1, double v2) {    
        BigDecimal b1 = new BigDecimal(Double.toString(v1));    
        BigDecimal b2 = new BigDecimal(Double.toString(v2));    
        return b1.multiply(b2);    
    }    

    public static BigDecimal div(double v1, double v2) {    
        BigDecimal b1 = new BigDecimal(Double.toString(v1));    
        BigDecimal b2 = new BigDecimal(Double.toString(v2));    
        // 2 = 保留小数点后两位   ROUND_HALF_UP = 四舍五入    
        return b1.divide(b2, 2, BigDecimal.ROUND_HALF_UP);// 应对除不尽的情况    
    }    
}
```
该工具类提供了 double 类型的基本的加减乘除运算。直接调用即可。
