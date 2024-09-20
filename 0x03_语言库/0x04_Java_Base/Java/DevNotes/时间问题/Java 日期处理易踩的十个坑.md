Java
<a name="v4URu"></a>
## 前言
整理了 Java 日期处理的十个坑。
<a name="gMSdf"></a>
## 1、用 Calendar 设置时间的坑
<a name="Ua8O7"></a>
### 反例：
```java
Calendar c = Calendar.getInstance();
c.set(Calendar.HOUR, 10);
System.out.println(c.getTime());
```
<a name="YBVzR"></a>
### 运行结果：
```java
Thu Mar 26 22:28:05 GMT+08:00 2020
```
<a name="ZH6Xv"></a>
### 解析：
设置了 10 小时，但运行结果是 22 点，而不是 10 点。因为 `Calendar.HOUR` 默认是按 12 小时制处理的，需要使用 `Calendar.HOUR_OF_DAY`，因为它才是按 24 小时处理的。
<a name="KAcFc"></a>
### 正例：
```java
Calendar c = Calendar.getInstance();
c.set(Calendar.HOUR_OF_DAY, 10);
```
<a name="S6uJn"></a>
## 2、Java 日期格式化 YYYY 的坑
<a name="ZWUJI"></a>
### 反例：
```java
Calendar calendar = Calendar.getInstance();
calendar.set(2019, Calendar.DECEMBER, 31);
Date testDate = calendar.getTime();
SimpleDateFormat dtf = new SimpleDateFormat("YYYY-MM-dd");
System.out.println("2019-12-31 转 YYYY-MM-dd 格式后 " + dtf.format(testDate));
```
<a name="p9COu"></a>
### 运行结果：
```java
2019-12-31 转 YYYY-MM-dd 格式后 2020-12-31
```
<a name="OwBXe"></a>
### 解析：
为什么明明是 2019 年 12 月 31 号，就转了一下格式，就变成了 2020 年 12 月 31 号了？因为 YYYY 是基于周来计算年的，它指向当天所在周属于的年份，一周从周日开始算起，周六结束，只要本周跨年，那么这一周就算下一年的了。正确姿势是使用 yyyy 格式。
<a name="ezED5"></a>
### 正例：
```java
Calendar calendar = Calendar.getInstance();
calendar.set(2019, Calendar.DECEMBER, 31);
Date testDate = calendar.getTime();
SimpleDateFormat dtf = new SimpleDateFormat("yyyy-MM-dd");
System.out.println("2019-12-31 转 yyyy-MM-dd 格式后 " + dtf.format(testDate));
```
<a name="p2seL"></a>
## 3、Java日期格式化hh的坑。
<a name="Lhrcr"></a>
### 反例：
```java
String str = "2020-03-18 12:00";
SimpleDateFormat dtf = new SimpleDateFormat("yyyy-MM-dd hh:mm");
Date newDate = dtf.parse(str);
System.out.println(newDate);
```
<a name="MyLfa"></a>
### 运行结果：
```java
Wed Mar 18 00:00:00 GMT+08:00 2020
```
<a name="xeDnv"></a>
### 解析：
设置的时间是 12 点，为什么运行结果是 0 点呢？因为 hh 是 12 制的日期格式，当时间为 12 点，会处理为 0 点。正确姿势是使用 HH，它才是 24 小时制。
<a name="tvCak"></a>
### 正例：
```java
String str = "2020-03-18 12:00";
SimpleDateFormat dtf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
Date newDate = dtf.parse(str);
System.out.println(newDate);
```
<a name="CSESl"></a>
## 4、Calendar 获取的月份比实际数字少 1 即 (0-11)
<a name="b6sAE"></a>
### 反例：
```java
//获取当前月，当前是3月
Calendar calendar = Calendar.getInstance();
System.out.println("当前"+calendar.get(Calendar.MONTH)+"月份");
```
<a name="n3YQr"></a>
### 运行结果：
```java
当前2月份
```
<a name="THugC"></a>
### 解析：
> The first month of the year in the Gregorian and Julian calendarsis JANUARY which is 0;

也就是1月对应的是下标 0，依次类推。因此获取正确月份需要加 1。
<a name="j6OKU"></a>
### 正例：
```java
//获取当前月，当前是3月
Calendar calendar = Calendar.getInstance();
System.out.println("当前"+(calendar.get(Calendar.MONTH)+1)+"月份");
```
<a name="KoSqR"></a>
## 5、Java 日期格式化 DD 的坑
<a name="EWpXh"></a>
### 反例：
```java
Calendar calendar = Calendar.getInstance();
calendar.set(2019, Calendar.DECEMBER, 31);
Date testDate = calendar.getTime();
SimpleDateFormat dtf = new SimpleDateFormat("yyyy-MM-DD");
System.out.println("2019-12-31 转 yyyy-MM-DD 格式后 " + dtf.format(testDate));
```
<a name="Mmz71"></a>
### 运行结果：
```java
2019-12-31 转 yyyy-MM-DD 格式后 2019-12-365
```
<a name="lXeFg"></a>
### 解析：
DD 和 dd 表示的不一样，DD 表示的是一年中的第几天，而 dd 表示的是一月中的第几天，所以应该用的是 dd。
<a name="FijOE"></a>
### 正例：
```java
Calendar calendar = Calendar.getInstance();
calendar.set(2019, Calendar.DECEMBER, 31);
Date testDate = calendar.getTime();
SimpleDateFormat dtf = new SimpleDateFormat("yyyy-MM-dd");
System.out.println("2019-12-31 转 yyyy-MM-dd 格式后 " + dtf.format(testDate));
```
<a name="y5ypq"></a>
## 6、`SimleDateFormat`的format 初始化问题
<a name="PZF1w"></a>
### 反例：
```java
SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
System.out.println(sdf.format(20200323));
```
<a name="FVAk8"></a>
### 运行结果：
```java
1970-01-01
```
<a name="PWt5I"></a>
### 解析：
用 format 格式化日期是，要输入的是一个 Date 类型的日期，而不是一个整型或者字符串。
<a name="XnL8Z"></a>
### 正例：
```java
Calendar calendar = Calendar.getInstance();
calendar.set(2020, Calendar.MARCH, 23);
SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
System.out.println(sdf.format(calendar.getTime()));
```
<a name="kAjQ1"></a>
## 7、日期本地化问题
<a name="W3i9x"></a>
### 反例：
```java
String dateStr = "Wed Mar 18 10:00:00 2020";
DateTimeFormatter formatter = DateTimeFormatter.ofPattern("EEE MMM dd HH:mm:ss yyyy");
LocalDateTime dateTime = LocalDateTime.parse(dateStr, formatter);
System.out.println(dateTime);
```
<a name="HWi1g"></a>
### 运行结果：
```java
Exception in thread "main" java.time.format.DateTimeParseException: Text 'Wed Mar 18 10:00:00 2020' could not be parsed at index 0
    at java.time.format.DateTimeFormatter.parseResolved0(DateTimeFormatter.java:1949) 
    at java.time.format.DateTimeFormatter.parse(DateTimeFormatter.java:1851) 
    at java.time.LocalDateTime.parse(LocalDateTime.java:492) 
    at com.example.demo.SynchronizedTest.main(SynchronizedTest.java:19)
```
<a name="iB4sy"></a>
### 解析：
`DateTimeFormatter` 这个类默认进行本地化设置，如果默认是中文，解析英文字符串就会报异常。可以传入一个本地化参数（`Locale.US`）解决这个问题。
<a name="AFvSt"></a>
### 正例：
```java
String dateStr = "Wed Mar 18 10:00:00 2020";
DateTimeFormatter formatter = DateTimeFormatter.ofPattern("EEE MMM dd HH:mm:ss yyyy",Locale.US);
LocalDateTime dateTime = LocalDateTime.parse(dateStr, formatter);
System.out.println(dateTime);
```
<a name="fq6pu"></a>
## 8、SimpleDateFormat 解析的时间精度问题
<a name="sqQHV"></a>
### 反例：
```java
SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
String time = "2020-03";
System.out.println(sdf.parse(time));
```
<a name="mjrFN"></a>
### 运行结果：
```java
Exception in thread "main" java.text.ParseException: Unparseable date: "2020-03"
    at java.text.DateFormat.parse(DateFormat.java:366) 
    at com.example.demo.SynchronizedTest.main(SynchronizedTest.java:19)
```
<a name="MZO15"></a>
### 解析：
`SimpleDateFormat` 可以解析长于/等于它定义的时间精度，但是不能解析小于它定义的时间精度。
<a name="czfcN"></a>
### 正例：
```java
SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM");
String time = "2020-03";
System.out.println(sdf.parse(time));
```
<a name="nThxm"></a>
## 9、`SimpleDateFormat` 的线性安全问题
<a name="IwfVD"></a>
### 反例：
```java
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RegexMatches {
    public static void main(String args[]) {

        // String to be scanned to find the pattern.
        String line = "This order was placed for QT3000! OK?";
        String pattern = "(.*)(\\d+)(.*)";

        // Create a Pattern object
        Pattern r = Pattern.compile(pattern);

        // Now create matcher object.
        Matcher m = r.matcher(line);
        if (m.find()) {
            System.out.println("Found value: " + m.group(0));
            System.out.println("Found value: " + m.group(1));
            System.out.println("Found value: " + m.group(2));
        } else {
            System.out.println("NO MATCH");
        }
    }
}
```
<a name="hFagI"></a>
### 运行结果：
```java
Exception in thread "pool-1-thread-49" java.lang.NumberFormatException: For input string: "5151."
at java.lang.NumberFormatException.forInputString(NumberFormatException.java:65) 
at java.lang.Long.parseLong(Long.java:589) 
at java.lang.Long.parseLong(Long.java:631) 
at java.text.DigitList.getLong(DigitList.java:195) 
at java.text.DecimalFormat.parse(DecimalFormat.java:2051) 
at java.text.SimpleDateFormat.subParse(SimpleDateFormat.java:2162) 
at java.text.SimpleDateFormat.parse(SimpleDateFormat.java:1514) 
at java.text.DateFormat.parse(DateFormat.java:364) 
at com.example.demo.SimpleDateFormatTest.lambda$main$0(SimpleDateFormatTest.java:19) 
at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) 
at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) 
at java.lang.Thread.run(Thread.java:748)Exception in thread "pool-1-thread-47" java.lang.NumberFormatException: For input string: "5151."
at java.lang.NumberFormatException.forInputString(NumberFormatException.java:65) 
at java.lang.Long.parseLong(Long.java:589) 
at java.lang.Long.parseLong(Long.java:631) 
at java.text.DigitList.getLong(DigitList.java:195) 
at java.text.DecimalFormat.parse(DecimalFormat.java:2051) 
at java.text.SimpleDateFormat.subParse(SimpleDateFormat.java:2162) 
at java.text.SimpleDateFormat.parse(SimpleDateFormat.java:1514) 
at java.text.DateFormat.parse(DateFormat.java:364) 
at com.example.demo.SimpleDateFormatTest.lambda$main$0(SimpleDateFormatTest.java:19) 
at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) 
at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) 
at java.lang.Thread.run(Thread.java:748)
```
<a name="Ohrhe"></a>
### 解析：
全局变量的 `SimpleDateFormat`，在并发情况下，存在安全性问题。

- `SimpleDateFormat` 继承了 `DateFormat`；
- `DateFormat` 类中维护了一个全局的 Calendar 变量；
- `sdf.parse(dateStr)` 和 `sdf.format(date)`，都是由 `Calendar` 引用来储存的；
- 如果 `SimpleDateFormat` 是 `static` 全局共享的，`Calendar` 引用也会被共享；
- 又因为 `Calendar` 内部并没有线程安全机制，所以全局共享的 `SimpleDateFormat` 不是线性安全的。

解决 `SimpleDateFormat` 线性不安全问题，有三种方式：

- 将 `SimpleDateFormat` 定义为局部变量；
- 使用 `ThreadLocal`；
- 方法加同步锁 `synchronized`。
<a name="WsfXN"></a>
### 正例：
```java

public class SimpleDateFormatTest {
    private static final String DATE_FORMAT = "yyyy-MM-dd HH:mm:ss";
    private static ThreadLocal < DateFormat > threadLocal = new ThreadLocal < DateFormat > ();
    public static DateFormat getDateFormat() {
        DateFormat df = threadLocal.get();
        if (df == null) {
            df = new SimpleDateFormat(DATE_FORMAT);
            threadLocal.set(df);
        }
        return df;
    }
    public static String formatDate(Date date) throws ParseException {
        return getDateFormat().format(date);
    }
    public static Date parse(String strDate) throws ParseException {
        return getDateFormat().parse(strDate);
    }
    public static void main(String[] args) {
        ThreadPoolExecutor threadPoolExecutor = new ThreadPoolExecutor(10, 100, 1, TimeUnit.MINUTES, new LinkedBlockingQueue < > (1000));
        while (true) {
            threadPoolExecutor.execute(() - > {
                try {
                    String dateString = formatDate(new Date());
                    Date parseDate = parse(dateString);
                    String dateString2 = formatDate(parseDate);
                    System.out.println(dateString.equals(dateString2));
                } catch (ParseException e) {
                    e.printStackTrace();
                }
            });
        }
    }
}
```
<a name="KWsvf"></a>
## 10、Java日期的夏令时问题
<a name="bLB9o"></a>
### 反例：
```java
TimeZone.setDefault(TimeZone.getTimeZone("Asia/Shanghai"));
SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
System.out.println(sdf.parse("1986-05-04 00:30:00"));
```
<a name="YoA7O"></a>
### 运行结果：
```java
Sun May 04 01:30:00 CDT 1986
```
<a name="YIJzE"></a>
### 解析：
先了解一下夏令时：
> 夏令时，表示为了节约能源，人为规定时间的意思。一般在天亮早的夏季人为将时间调快一小时，可以使人早起早睡，减少照明量，以充分利用光照资源，从而节约照明用电。各个采纳夏时制的国家具体规定不同。目前全世界有近110个国家每年要实行夏令时。
> 1986年4月，中国中央有关部门发出“在全国范围内实行夏时制的通知”，具体作法是：每年从四月中旬第一个星期日的凌晨2时整（北京时间），将时钟拨快一小时。(1992年起，夏令时暂停实行。)
> 夏时令这几个时间可以注意一下，1986-05-04, 1987-04-12, 1988-04-10, 1989-04-16, 1990-04-15, 1991-04-14.

结合 Demo 代码，中国在 1986-05-04 当天还在使用夏令时，时间被拨快了 1 个小时。所以 0 点 30 分打印成了 1 点 30 分。如果要打印正确的时间，可以考虑修改时区为东 8 区。
<a name="vadQd"></a>
### 正例：
```java
TimeZone.setDefault(TimeZone.getTimeZone("GMT+8"));
SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
System.out.println(sdf.parse("1986-05-04 00:30:00"));
```
