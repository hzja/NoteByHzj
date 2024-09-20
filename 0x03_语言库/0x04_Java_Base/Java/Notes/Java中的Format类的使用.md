Java Format
<a name="BLOkn"></a>
## Format
Foramt是一个抽象基类，其具体子类必须实现
```java
format(Object obj, StringBuffer toAppendTo, FieldPosition pos)
```
和
```java
parseObject(String source, ParsePosition pos)
```
两个抽象方法。<br />format方法用于将对象格式化为指定模式的字符串<br />parseObject方法用于将字符串重新解析为对象<br />Format的直接子类包括DateFormat、NumberFormat和MessageFormat。
<a name="TUctb"></a>
### 1、DateFormat
DateFormat根据当前语言环境格式化日期和时间。<br />DateFormat是一个抽象类，所以不能直接new创建实例对象。但该类为提供了工厂方法方便使用。
```java
1.getDateInstance()方法，获取格式化的日期，输出样式：2015-12-10
2.getDateTimeInstance()方法，获取格式化的日期和时间,输出样式：2015-12-10 10:21:41
3.getTimeInstance()方法，获取格式化的时间，输出样式：10:21:41
4.getInstance()方法，获取格式化的日期和时间，输出样式：15-12-10 上午10:21
```
也许会发现，在这些工厂方法中允许传入一个int参数，该参数允许设定格式化风格，从而得到相对理想的结果。下表中对应了不同的style值和输出样式（这些常量值都在DateFormat类中）

| 样式值 | 日期 | 时间 |
| --- | --- | --- |
| SHORT | 15-12-10 | 上午10:08 |
| MEDIUM | 2015-12-10 | 10:09:23 |
| LONG | 2015年12月10日 | 上午10时09分40秒 |
| FULL | 2015年12月10日 星期四 | 上午10时17分30秒 CST |
| DEFAULT | 2015-12-10 | 10:18:07 |

当然也可以指定语言环境获取该语言环境下的格式化日期和时间，
```java
DateFormat format = DateFormat.getDateInstance(DateFormat.DEFAULT,Locale.CANADA);//获取加拿大的格式化日期
```
<a name="SimpleDateFormat"></a>
#### SimpleDateFormat
SimpleDateFormat是DateFormat的一个具体类，它允许指定格式模式从而获取理想的格式化日期和时间。<br />通过SimpleDateFormat的构造方法可以传入一个格式模式字符串或者通过applyPattern(String pattern)方法添加一个格式模式字符串。<br />对于格式模式字符串，API为提供了丰富的模式元素，下面列出几个常用的模式元素

| 字母 | 日期或时间元素 | 示例 |
| --- | --- | --- |
| y | 年 | 2015 |
| M | 年中的月份 | 12 |
| w | 年中的周数 | 50 |
| W | 月份中的周数 | 02 |
| D | 年中的天数 | 344 |
| d | 月份中的天数 | 10 |
| F | 月份中的星期 | 02 |
| E | 星期中的天数 | 星期四、Thu |
| a | AM/PM标记 | 下午、PM |
| H | 一天中的小时数(0~23) | 21 |
| k | 一天中的小时数(1~24) | 21 |
| K | am/pm中的小时数(0~11) | 09 |
| h | am/pm中的小时数(1~12) | 09 |
| m | 小时中的分钟数 | 31 |
| s | 分钟中的秒数 | 08 |
| S | 毫秒数 | 716 |

如果设置Locale的话，会有不同的显示格式，比如如果设置Locale.ENGLISH，E会显示为英文格式，a显示为AM或PM
```java
Date date = new Date();
SimpleDateFormat format = new SimpleDateFormat("今天是yyyy-MM-dd E hh:mm:ss，是yyyy年的第DD天，在该月是第dd天");
System.out.println(format.format(date));
```
将会输出：今天是2015-12-10 星期四 09:38:16，是2015年的第344天，在该月是第10天
<a name="k6DDX"></a>
### 2、NumberFormat
NumberFormat根据当前语言环境格式化数字<br />NumberFormat同样是一个抽象基类，可以使用API中的工厂方法获取实例对象

1. `getCurrencyInstance()`方法，根据当前语言环境获取货币数值格式。传递Locale对象可以获取指定语言环境下的货币数值格式，比如
```java
NumberFormat format = NumberFormat.getCurrencyInstance(.CANADA);
System.out.println(format.format(439.6));
```
将会输出：$439.60

2. `getInstance()`和`getNumberInstance()`方法都会获取到常规数值格式
3. `getIntegerInstance()`方法获取常规整数值格式，如果需要格式化的数值为小数，则会将数值四舍五入为最接近的整数
4. `getPercentInstance()`方法获取百分比的数值格式

`NumberFormat`有两个具体实现子类`DecimalFormat`和`ChoiceFormat`
<a name="znWK9"></a>
#### `DecimalFormat`
`DecimalFormat`同`SimpleDateFormat`类似，允许指定格式模式获取想要的格式化数值<br />`DecimalFormat`类对于数值的小数部分，默认显示3位小数，在去掉超出小数点后面3位的部分时，会将数值四舍五入为最接近的数值格式化输出。可以对这个默认进行设置<br />`setMaximumFractionDigits(int newValue)`方法，设置小数部分中允许的最大数字位数<br />`setMinimumFractionDigits(int newValue)`方法，设置小数部分中允许的最小数字位数，如果原数小数位数不够的话，会补零。<br />对于数值的整数部分，默认3个数字为一组进行显示，同样对此也可以自定义，使用setGroupingSize(int i)方法，设置分组中一组的位数。<br />`setGroupingUsed(boolean value)`方法设置是否使用分组，true表示使用，false表示取消分组<br />`setMaximumIntegerDigits(int newValue)`方法设置整数部分允许的最大数字位数<br />`setMinimumIntegerDigits(int newValue)`方法设置整数部分允许的最小数字位数<br />在````的构造方法中，允许传入格式模式字符串输出想要的格式化数值，格式模式元素包含如下：<br />0 | 表示一个数字，被格式化数值不够的位数会补0<br />| 表示一个数字，被格式化数值不够的位数会忽略<br />. | 小数点分隔符的占位符<br />, | 分组分隔符的占位符<br />· | 缺省负数前缀<br />% | 将数值乘以100并显示为百分数<br />\u2030 | 将数值乘以1000并显示为千分数<br />案例：
```java
DecimalFormat format1 = new DecimalFormat("#\u2030");
System.out.println(format1.format(0.3345));//输出334‰
        
DecimalFormat format2 = new DecimalFormat("##.##");
System.out.println(format2.format(12.345));//输出12.35
        
DecimalFormat format3 = new DecimalFormat("0000.00");
System.out.println(format3.format(12.345));//输出0012.35
        
DecimalFormat format4 = new DecimalFormat("#.##%");
System.out.println(format4.format(12.345));//输出1234.5%
```
<a name="nBF6S"></a>
#### `ChoiceFormat`
`ChoiceFormat`允许将格式化运用到某个范围的数，通常与`MessageFormat`一同使用。`ChoiceFormat`在构造方法中接收一个format数组和一个limits数组，这两个数组的长度必须相等，例如：
```
limits = {1,2,3,4,5,6,7}
formats = {"Sun","Mon","Tue","Wed","Thur","Fri","Sat"}
```
limits数组实际上是个区间，可开可闭，并且必须按升序排列，如果不按升序排列，格式化结果将会不正确，还可以使用\u221E(表示无穷大)。<br />`ChoiceFormat`的匹配公式
> limit[j] <= X <limit[j+1]

其中X表示使用format方法传入的值，j表示limit数组中的索引。当且仅当上述公式成立时，X匹配j，如果不能匹配，则会根据X是太小还是太大，匹配limits数组的第一个索引或最后一个索引，然后使用匹配的limits数组中的索引，去formats数组中寻找相同索引的值。例子：
```java
double[] limits = { 3, 4, 5, 6, 7, 8, 9 };
String[] formats = { "星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日" };
ChoiceFormat format = new ChoiceFormat(limits, formats);
System.out.println(format.format(2.5));//将会输出"星期一"
/**3.6介于3和4之间，所以会匹配3，又由于3在limits数组中的索引是0，所以会在formats数组徐照索引0的值，即输出"星期一"
*/
System.out.println(format.format(3.6));
```
下面看一下`ChoiceFormat`类中的几个常用方法

1. `nextDouble(double d)`静态方法查找大于d的最小double值，用在limits数组中，从而使limits数组形成一个右开区间数组，例如

limits = {0,1,ChoiceFormat.nextDouble(1)}

2. `nextDouble(double d, boolean positive)`静态方法，如果positive参数为true，表示查找大于d的最小double值；如果positive参数为false，表示查找小于d的最大double值，这样就可以使limits形成一个左开区间数组。
3. `previousDouble(double d)`静态方法，查找小于d的最大double值

ChoiceFormat类的构造方法也允许传入一个模式字符串，format方法会根据这个模式字符串执行格式化操作。一个模式元素的格式如下：
> doubleNum [占位符] formatStr

占位符可以使用#、<  、\u2264(<=)
```java
ChoiceFormat cf = new ChoiceFormat("1#is 1 | 1<is more than 1");
System.out.println(cf.format(1));//输出"is 1"
System.out.println(cf.format(2));//输出"is more than 1"
System.out.println(cf.format(0));//输出"is 1"
```
由上面的例子可以看出，模式字符串中的每个模式元素之间使用"|"分割，"|"前后可以添加空格以美化代码，而且必须按照升序进行书写，否则会出现 `java.lang.IllegalArgumentException` 的运行时异常。<br />观看ChoiceFormat类的源码得知，实际上在内部，模式字符串还是被转换为limits和formats两个数组。在源码中
```java
public ChoiceFormat(String newPattern)  {
    applyPattern(newPattern);
}
/** applyPattern(newPattern)方法的部分源码
*/
public void applyPattern(String newPattern) {
    ...
        choiceLimits = new double[count];
    System.arraycopy(newChoiceLimits, 0, choiceLimits, 0, count);
    choiceFormats = new String[count];
    System.arraycopy(newChoiceFormats, 0, choiceFormats, 0, count);
    ...
    }
}
```
可以看出`ChoiceFormat(String newPattern)`调用了`applyPattern(String newPattern)`方法，在`applyPattern`方法中对`newPattern`字符串进行解析，然后将解析后的数据放置到ChoiceFormat类的两个私有属性`double[] choiceLimits`和`String[] choiceFormats`中，然后使用格式化方法即可。
<a name="rZ5k3"></a>
### 3、`MessageFormat`
`MessageFormat`提供了以语言环境无关的生成连接消息的方式。<br />常用`MessageFormat`的静态方法format，该方法接收一个字符串的模式和一组对象(对象数组)，按照模式形式将格式化的对象插入到模式中，然后返回字符串结果。<br />`MessageFormat`的格式模式元素（`FormatElement`）形式如下:<br />{ArgumentIndex}<br />{ArgumentIndex,FormatType}<br />{ArgumentIndex,FormatType,FormatStyle}<br />其中`ArgumentIndex`对象数组中的索引，从0开始，<br />FormatType包括number、date、 time、choice，<br />FormatStyle包括short、medium、long、full、integer、currency、percent、SubformatPattern(子模式)，<br />在MessageFormat类的内部，FormatType和FormatStyle实际上是创建格式元素的Format示例<br />number对应了`NumberFormat`，其子格式对应了`DecimalFormat`<br />date和time对应了`DateFormat`，其资格是对应了`SimpleDateFormat`<br />choice对应了`ChoiceFormat`<br />可以直接使用`MessageFormat`类中的静态方法format，像这样：
```java
/**这是源码注释中的一个例子* 在这个例子中静态方法format第一个参数是字符串类型的，
* 即模式字符串，第二个参数是个可变参数，实际上就是一个Object类型的数组。
* 在模式字符串中使用"{}"标识一个FormatElement。"{}"中的ArgumentIndex对应Object数组中响应索引处的值。
*/
int planet = 7;
String event = "a disturbance in the Force";
String result = MessageFormat.format("At {1,time} on {1,date}, there was {2} on planet {0,number,integer}.",
                  planet, new Date(), event);
System.out.println(result);
```
//输出：At 20:39:15 on 2015-12-11, there was a disturbance in the Force on planet 7.<br />也可以使用`MessageFormat`的构造方法传入pattern string（模式字符串），然后调用普通的format方法。<br />不仅被允许使用`MessageFormat`类中提供默认的`FormatElement`去format这些对象，还可以设置自己的Format对象format这些Object。
```java
/**在这个例子中，MessageFormat和ChoiceFormat被结合使用* MessageFormat类中有3个方法值关注
* 1.setFormatByArgumentIndex(int argumentIndex, Format newFormat)//
* 2.setFormats(Format[] newFormats)
* 3.setFormat(int formatElementIndex, Format newFormat)
* 在这个例子当中，在MessageFormat的模式字符串的FormatElement（即{}中的内容）中
* 索引为0的地方将使用ChoiceFormat的格式去格式化。
* 如果在set的Format中仍具有FormatElement，则会递归调用MessageFormat的format方法。
*/
MessageFormat form = new MessageFormat("The disk \"{1}\" contains {0}.");
double[] filelimits = { 0, 1, 2 };
String[] filepart = { "no files", "one file", "{0,number} files" };
ChoiceFormat fileform = new ChoiceFormat(filelimits, filepart);
form.setFormatByArgumentIndex(0, fileform);
int fileCount = 1273;
String diskName = "MyDisk";
Object[] testArgs = { new Long(fileCount), diskName };
System.out.println(form.format(testArgs));
//输出：The disk "MyDisk" contains 1,273 files.
```
<a name="1wf3v"></a>
### 4、String类中的`format`方法
format方法使用占位符进行格式化<br />常规类型、字符类型和数值类型的占位符格式：<br />%[index$][标识][最小宽度][.精度]转换符<br />日期和时间类型的占位符格式：<br />%[index$][标识][最小宽度]转换符<br />与参数不对应的占位符格式：<br />%[标识][最小宽度]转换符<br />其中index表示参数列表中的位置上的值<br />可用标识：

| 标识 | 含义 |
| --- | --- |
| - | 在最小宽度内左对齐，不可与0标识一起使用 |
| 0 | 若内容长度不足最小宽度，则在左边用0来填充 |
| # | 对8进制和16进制，8进制前添加一个0,16进制前添加0x |
| + | 结果总包含一个+或-号 |
| 空格 | 正数前加空格，负数前加-号 |
| , | 只用与十进制，每3位数字间用,分隔 |
| ( | 若结果为负数，则用括号括住，且不显示符号 |

可用转换符：

| 转换符 | 含义 |
| --- | --- |
| b | 布尔类型，只要实参为非false的布尔类型，均格式化为字符串true，否则为字符串false |
| n | 平台独立的换行符, 也可通过System.getProperty("line.separator")获取 |
| f | 浮点数型（十进制）。显示9位有效数字，且会进行四舍五入。如99.99 |
| a | 浮点数型（十六进制） |
| e | 指数类型。如9.38e+5 |
| g | 浮点数型（比%f，%a长度短些，显示6位有效数字，且会进行四舍五入） |
| s | 字符串类型 |
| c | 字符类型 |


```java
String result1 = String.format("小明今年%d岁,他住在%s,他的月工资有%.2f", 25,"北京市",6633.435);System.out.println(result1);//输出:小明今年25岁,他住在北京市,他的月工资有6633.44
/*****************************************************/
double num = 123.4567899;
String result2 = String.format("%e", num);
System.out.println(result2);//输出：1.234568e+02
```
<a name="3niAq"></a>
### 注意
:::danger
Format中的子类都是不同步，所以需要注意线程安全问题
:::
