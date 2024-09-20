Java
<a name="lM5lS"></a>
## 字符串格式
在 Java 中格式化字符串的最常见方法是使用`String.format()`。如果有一个“java sprintf”，那就是它。
```java
String output = String.format("%s = %d", "joe", 35);
```
对于格式化的控制台输出，可以使用`printf()`或`System.out`和`System.err` `PrintStreams`的`format()`方法。
```java
System.out.printf("My name is: %s%n", "joe");
```
创建一个Formatter 并将其链接到StringBuilder。使用`format()`方法格式化的输出将附加到StringBuilder。
```java
StringBuilder sbuf = new StringBuilder();
Formatter fmt = new Formatter(sbuf);
fmt.format("PI = %f%n", Math.PI);
System.out.print(sbuf.toString());
// you can continue to append data to sbuf here.
```
<a name="TTxOp"></a>
## 格式说明符
以下是支持的所有转换说明符的快速参考：

| 说明符 | 适用于 | 输出 |
| --- | --- | --- |
| %a | 浮点数（BigDecimal除外） | 浮点数的十六进制输出 |
| %b | 任意种类 | 如果为非空，则为“真”，如果为空，则为“假” |
| %c | 特点 | Unicode 字符 |
| %d | 整数（包括字节、短整数、整数、长整数、大整数） | 十进制整数 |
| %e | 浮点 | 科学计数法中的十进制数 |
| %f | 浮点 | 十进制数 |
| %g | 浮点 | 十进制数，可能采用科学计数法，具体取决于精度和值。 |
| %h | 任意种类 | 来自 hashCode() 方法的十六进制字符串值。 |
|  %n | 没有 | 特定于平台的行分隔符。 |
| %o | 整数（包括字节、短整数、整数、长整数、大整数） | 八进制数 |
| %s | 任意种类 | 字符串值 |
| %t | 日期/时间（包括 long、Calendar、Date 和 TemporalAccessor） | %t 是日期/时间转换的前缀。在此之后需要更多的格式化标志。请参阅下面的日期/时间转换。 |
| %x | 整数（包括字节、短整数、整数、长整数、大整数） | 十六进制字符串。 |

<a name="tJxQT"></a>
## 日期和时间格式
注意：在下表中使用带有“%T”而不是“%t”的格式化字符会使输出大写。

|  FLAG | NOTES |
| --- | --- |
|  %tA | 星期的全名，例如“ Sunday”、“ Monday” |
|  %ta | 工作日的缩写名称，例如“ Sun”、“ Mon”等。 |
|  %tB | 月份的全名，例如“ January”、“ February”等。 |
| %tb | 缩写月份名称，例如“ Jan”、“ Feb”等。 |
|  %tC | 用两位数格式化的年份的世纪部分，例如“00”到“99”。 |
|  %tc | 用“ ”格式化的日期和时间，%ta %tb %td %tT %tZ %tY例如“ Fri Feb 17 07:45:42 PST 2017” |
| %tD | 日期格式为“ %tm/%td/%ty” |
|  %td | 用两位数字格式化的月份中的日期。例如“ 01”到“ 31”。 |
|  %te | 月份中没有前导 0 的日期，例如“1”到“31”。 |
| %tF | 带有“”的 ISO 8601 格式日期%tY-%tm-%td。 |
| %tH | 24 小时制的一天中的小时，例如“ 00”到“ 23”。 |
| %th | 与 %tb 相同。 |
| %tI | 12 小时制的一天中的小时，例如“ 01” – “ 12“。 |
| %tj | 用前导 0 格式化的一年中的一天，例如“ 001”到“ 366”。 |
| %tk | 没有前导 0 的 24 小时制时钟，例如“ 0”到“ 23”。 |
| %tl | 没有前导 0 的 12 小时点击的一天中的小时，例如“ 1”到“ 12”。 |
| %tM | 小时内的分钟格式化为前导 0，例如“ 00”到“ 59”。 |
| %tm

 | 以 0 开头的月份格式，例如“ 01”到“ 12”。 |
| %tN | 纳秒格式为 9 位数字和前导 0，例如“000000000”到“999999999”。 |
| %tp | 特定于区域设置的“am”或“pm”标记。 |
| %tQ | 自 1970 年 1 月 1 日 00:00:00 UTC 纪元以来的毫秒数。 |
| %tR | 时间格式为 24 小时制，例如“ %tH:%tM”。 |
| %tr | 时间格式为 12 小时，例如“ %tI:%tM:%tS %Tp”。 |
| %tS | 分钟内的秒数采用 2 位数字格式，例如“00”到“60”。支持闰秒需要“60”。 |
| %ts | 自 1970 年 1 月 1 日 00:00:00 UTC 纪元以来的秒数。 |
| %tT | 时间格式为 24 小时制，例如“ %tH:%tM:%tS”。 |
| %tY | 年份格式为 4 位数字，例如“ 0000”到“ 9999”。 |
| %ty | 年份格式为 2 位数字，例如“ 00”到“ 99”。 |
| %tZ | 时区缩写。例如“ UTC”、“ PST”等。 |
| %tz | 与 GMT 的时区偏移量，例如“-0800“. |

<a name="EyAUG"></a>
## 参数索引
`$`参数索引指定为在“ ”之后以“ ”结尾的数字，`%`并在参数列表中选择指定的参数。
```java
String.format("%2$s", 32, "Hello"); // 打印: "Hello"
```
<a name="G0G3A"></a>
## 格式化整数
使用`%d`格式说明符，您可以使用所有整数类型的参数，包括 byte、short、int、long 和 BigInteger。
<a name="LqysR"></a>
### 默认格式：
```java
String.format("%d", 93); 
// 打印：93
```
<a name="kB7qQ"></a>
### 指定宽度：
```java
String.format("|%20d|", 93); 
// 打印: | 93|
```
<a name="hYbOi"></a>
### 在指定宽度内左对齐：
```java
String.format("|%-20d|", 93); 
// 打印: |93 |
```
<a name="M0qbl"></a>
### 用零填充：
```java
String.format("|%020d|", 93); 
// 打印: |00000000000000000093|
```
<a name="jAKAA"></a>
### 用“+”打印正数：
```java
String.format("|%+20d|", 93); 
// 打印: | +93|
```
<a name="KmLL5"></a>
### 正数前的空格
正常情况下，负数包含“-”。
```java
String.format("|% d|", 93); 
// prints: | 93| 
String.format("|% d|", -36); 
// 打印: |-36|
```
<a name="ZnmXS"></a>
### 使用特定于语言环境的千位分隔符：
对于美国语言环境，它是“，”：
```java
String.format("|%,d|", 10000000); // 打印: |10,000,000|
```
<a name="j6I2A"></a>
### 将负数括在括号（“()”）中并跳过“-”：
```java
String.format("|%(d|", -36); 
// 打印: |(36)|
```
<a name="GSm2K"></a>
### 八进制输出：
```java
String.format("|%o|"), 93); 
// 打印: 135
```
<a name="TH9gb"></a>
### 十六进制输出：
```java
String.format("|%x|", 93); 
// 打印: 5d
```
<a name="TvpAT"></a>
### 八进制和十六进制输出的替代表示：
打印带有前导“ ”的八进制数字和带有前导“ 0”的十六进制数字0x。
```java
String.format("|%#o|", 93);
// 打印: 0135

String.format("|%#x|", 93);
// 打印: 0x5d

String.format("|%#X|", 93);
// 打印: 0X5D
```
<a name="ARbQW"></a>
## 字符串和字符转换
默认格式：
<a name="Vq6oN"></a>
### 打印整个字符串
```java
String.format("|%s|", "Hello World"); 
// 打印: "Hello World"
```
<a name="UhQUq"></a>
### 指定字段长度
```java
String.format("|%30s|", "Hello World"); 
// 打印: | Hello World|
```
<a name="iUkAg"></a>
### 左对齐文本
```java
String.format("|%-30s|", "Hello World"); 
// 打印: |Hello World |
```
<a name="IrV0W"></a>
### 指定最大字符数
```java
String.format("|%.5s|", "Hello World"); 
// 打印: |Hello|
```
<a name="j81iY"></a>
### 字段宽度和最大字符数
```java
String.format("|%30.5s|", "Hello World");
| Hello|
```
