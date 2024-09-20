Java8<br />Java 处理日期、日历和时间的不足之处：将 java.util.Date 设定为可变类型，以及 SimpleDateFormat 的非线程安全使其应用非常受限。然后就在 java8 上面增加新的特性。<br />全新 API 的众多好处之一就是，明确了日期时间概念，例如：瞬时（instant）、 长短（duration）、日期、时间、时区和周期。<br />同时继承了 Joda 库按人类语言和计算机各自解析的时间处理方式。不同于老版本，新 API 基于 ISO 标准日历系统，java.time 包下的所有类都是不可变类型而且线程安全。
<a name="HcaeZ"></a>
### 关键类

- `Instant`：瞬时实例。
- `LocalDate`：本地日期，不包含具体时间 例如：2014-01-14 可以用来记录生日、纪念日、加盟日等。
- `LocalTime`：本地时间，不包含日期。
- `LocalDateTime`：组合了日期和时间，但不包含时差和时区信息。
- `ZonedDateTime`：最完整的日期时间，包含时区和相对 UTC 或格林威治的时差。

新 API 还引入了 `ZoneOffSet` 和 `ZoneId` 类，使得解决时区问题更为简便。解析、格式化时间的 `DateTimeFormatter`类也全部重新设计。

| 1 | `Instant` | 时间戳 |
| --- | --- | --- |
| 2 | `Duration` | 持续时间、时间差 |
| 3 | `LocalDate` | 只包含日期，比如：2021-01-10 |
| 4 | `LocalTime` | 只包含时间，比如：23:12:10 |
| 5 | `LocalDateTime` | 包含时间和日期，比如：2021-01-15 23:14:21 |
| 6 | `Period` | 时间段 |
| 7 | `ZoneOffset` | 时间偏移量，比如：+8:00 |
| 8 | `ZonedDateTime` | 带时区的时间 |
| 9 | `Clock` | 时钟，比如获取目前美国纽约的时间 |
| 10 | `java.time.format.DateTimeFormatter` | 时间格式化 |

<a name="j8aV2"></a>
### 1、Java 8中获取今天的日期
Java 8 中的 `LocalDate` 用于表示当天日期。和`java.util.Date`不同，它只有日期，不包含时间。当仅需要表示日期时就用这个类。
```java
import java.time.LocalDate;
public class Demo01 {
    public static void main(String[] args) {
        LocalDate today = LocalDate.now();
        System.out.println("今天的日期:"+today);
    }
}
```
<a name="ThR0u"></a>
### 2、Java 8中获取年、月、日信息
LocalDate 提供了获取年、月、日的快捷方法，其实例还包含很多其它的日期属性。通过调用这些方法就可以很方便的得到需要的日期信息，不用像以前一样需要依赖java.util.Calendar类了。
```java
import java.time.LocalDate;
public class Demo02 {
    public static void main(String[] args) {
        LocalDate today = LocalDate.now();
        int year = today.getYear();
        int month = today.getMonthValue();
        int day = today.getDayOfMonth();
        System.out.println("year:"+year);
        System.out.println("month:"+month);
        System.out.println("day:"+day);
    }
}
```
<a name="pcwCd"></a>
### 3、Java 8中处理特定日期
通过静态工厂方法`now()`非常容易地创建了当天日期，还可以调用另一个有用的工厂方法`LocalDate.of()`创建任意日期， 该方法需要传入年、月、日做参数，返回对应的`LocalDate`实例。这个方法的好处是解决了老API的设计错误，比如年度起始于1900，月份是从0开 始等等。
```java
import java.time.LocalDate;
public class Demo03 {
    public static void main(String[] args) {
        LocalDate date = LocalDate.of(2018,2,6);
        System.out.println("自定义日期:"+date);
    }
}
```
<a name="Aa87m"></a>
### 4、Java 8中判断两个日期是否相等
现实生活中有一类时间处理就是判断两个日期是否相等。在项目开发的时候总会遇到这样子的问题。<br />下面这个例子会帮助你用 Java 8 的方式去解决，LocalDate 重载了 equal 方法。<br />注意，如果比较的日期是字符型的，需要先解析成日期对象再作判断。<br />请看下面的例子：
```java
import java.time.LocalDate;
public class Demo04 {
    public static void main(String[] args) {
        LocalDate date1 = LocalDate.now();
        LocalDate date2 = LocalDate.of(2018,2,5);
        if(date1.equals(date2)){
            System.out.println("时间相等");
        }else{
            System.out.println("时间不等");
        }
    }
}
```
<a name="xqWhS"></a>
### 5、Java 8中检查像生日这种周期性事件
Java 中另一个日期时间的处理就是检查类似生日、纪念日、法定假日（国庆以及春节）、或者每个月固定时间发送邮件给客户 这些周期性事件。<br />Java 中如何检查这些节日或其它周期性事件呢？答案就是`MonthDay`类。这个类组合了月份和日，去掉了年，这意味着你可以用它判断每年都会发生事件。<br />和这个类相似的还有一个`YearMonth`类。这些类也都是不可变并且线程安全的值类型。下面通过 `MonthDay`来检查周期性事件：
```java
import java.time.LocalDate;
import java.time.MonthDay;
public class Demo05 {
    public static void main(String[] args) {
        LocalDate date1 = LocalDate.now();
        LocalDate date2 = LocalDate.of(2018,2,6);
        MonthDay birthday = MonthDay.of(date2.getMonth(),date2.getDayOfMonth());
        MonthDay currentMonthDay = MonthDay.from(date1);
        if(currentMonthDay.equals(birthday)){
            System.out.println("是你的生日");
        }else{
            System.out.println("你的生日还没有到");
        }
    }
}
```
只要当天的日期和生日匹配，无论是哪一年都会打印出祝贺信息。可以把程序整合进系统时钟，看看生日时是否会受到提醒，或者写一个单元测试来检测代码是否运行正确。
<a name="3Znex"></a>
### 6、Java 8中获取当前时间
```java
import java.time.LocalTime;
public class Demo06 {
    public static void main(String[] args) {
        LocalTime time = LocalTime.now();
        System.out.println("获取当前的时间,不含有日期:"+time);
    }
}
```
可以看到当前时间就只包含时间信息，没有日期
<a name="OuFjK"></a>
### 7、Java 8中在现有的时间上增加小时
通过增加小时、分、秒来计算将来的时间很常见。Java 8除了不变类型和线程安全的好处之外，还提供了更好的`plusHours()`方法替换`add()`，并且是兼容的。注意，这些方法返回一个全新的`LocalTime`实例，由于其不可变性，返回后一定要用变量赋值。
```java
import java.time.LocalTime;
public class Demo07 {
    public static void main(String[] args) {
        LocalTime time = LocalTime.now();
        LocalTime newTime = time.plusHours(3);
        System.out.println("三个小时后的时间为:"+newTime);
    }
}
```
<a name="4Bu3V"></a>
### 8、Java 8如何计算一周后的日期
和上个例子计算3小时以后的时间类似，这个例子会计算一周后的日期。`LocalDate`日期不包含时间信息，它的`plus()`方法用来增加天、周、月，`ChronoUnit`类声明了这些时间单位。由于`LocalDate`也是不变类型，返回后一定要用变量赋值。
```java
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
public class Demo08 {
    public static void main(String[] args) {
        LocalDate today = LocalDate.now();
        System.out.println("今天的日期为:"+today);
        LocalDate nextWeek = today.plus(1, ChronoUnit.WEEKS);
        System.out.println("一周后的日期为:"+nextWeek);
    }
}
```
可以看到新日期离当天日期是7天，也就是一周。可以用同样的方法增加1个月、1年、1小时、1分钟甚至一个世纪，更多选项可以查看Java 8 API中的`ChronoUnit`类
<a name="4DDbq"></a>
### 9、Java 8计算一年前或一年后的日期
利用`minus()`方法计算一年前的日期
```java
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
public class Demo09 {
    public static void main(String[] args) {
        LocalDate today = LocalDate.now();
        LocalDate previousYear = today.minus(1, ChronoUnit.YEARS);
        System.out.println("一年前的日期 : " + previousYear);
        LocalDate nextYear = today.plus(1, ChronoUnit.YEARS);
        System.out.println("一年后的日期:"+nextYear);
    }
}
```
<a name="bysww"></a>
### 10、Java 8的`Clock`时钟类
Java 8增加了一个Clock时钟类用于获取当时的时间戳，或当前时区下的日期时间信息。以前用到`System.currentTimeInMillis()`和`TimeZone.getDefault()`的地方都可用Clock替换。
```java
import java.time.Clock;
public class Demo10 {
    public static void main(String[] args) {
        // Returns the current time based on your system clock and set to UTC.
        Clock clock = Clock.systemUTC();
        System.out.println("Clock : " + clock.millis());
        // Returns time based on system clock zone
        Clock defaultClock = Clock.systemDefaultZone();
        System.out.println("Clock : " + defaultClock.millis());
    }
}
```
<a name="SXaqX"></a>
### 11、用Java判断日期是早于还是晚于另一个日期
另一个工作中常见的操作就是如何判断给定的一个日期是大于某天还是小于某天？在Java 8中，LocalDate类有两类方法`isBefore()`和`isAfter()`用于比较日期。调用`isBefore()`方法时，如果给定日期小于当前日期则返回`true`。
```java
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
public class Demo11 {
    public static void main(String[] args) {
        LocalDate today = LocalDate.now();
        LocalDate tomorrow = LocalDate.of(2018,2,6);
        if(tomorrow.isAfter(today)){
            System.out.println("之后的日期:"+tomorrow);
        }
        LocalDate yesterday = today.minus(1, ChronoUnit.DAYS);
        if(yesterday.isBefore(today)){
            System.out.println("之前的日期:"+yesterday);
        }
    }
}
```
<a name="atBf6"></a>
### 12、Java 8中处理时区
Java 8不仅分离了日期和时间，也把时区分离出来了。现在有一系列单独的类如ZoneId来处理特定时区，`ZoneDateTime`类来表示某时区下的时间。这在Java 8以前都是 `GregorianCalendar`类来做的。下面这个例子展示了如何把本时区的时间转换成另一个时区的时间。
```java
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
public class Demo12 {
    public static void main(String[] args) {
        // Date and time with timezone in Java 8
        ZoneId america = ZoneId.of("America/New_York");
        LocalDateTime localtDateAndTime = LocalDateTime.now();
        ZonedDateTime dateAndTimeInNewYork  = ZonedDateTime.of(localtDateAndTime, america );
        System.out.println("Current date and time in a particular timezone : " + dateAndTimeInNewYork);
    }
}
```
<a name="NOOOK"></a>
### 13、使用`YearMonth`表示信用卡到期这类固定日期
与 `MonthDay`检查重复事件的例子相似，`YearMonth`是另一个组合类，用于表示信用卡到期日、FD到期日、期货期权到期日等。还可以用这个类得到 当月共有多少天，`YearMonth`实例的`lengthOfMonth()`方法可以返回当月的天数，在判断2月有28天还是29天时非常有用。
```java
import java.time.*;
public class Demo13 {
    public static void main(String[] args) {
        YearMonth currentYearMonth = YearMonth.now();
        System.out.printf("Days in month year %s: %d%n", currentYearMonth, currentYearMonth.lengthOfMonth());
        YearMonth creditCardExpiry = YearMonth.of(2019, Month.FEBRUARY);
        System.out.printf("Your credit card expires on %s %n", creditCardExpiry);
    }
}
```
<a name="K1stq"></a>
### 14、如何在Java 8中检查闰年
```java
import java.time.LocalDate;
public class Demo14 {
    public static void main(String[] args) {
        LocalDate today = LocalDate.now();
        if(today.isLeapYear()){
            System.out.println("This year is Leap year");
        }else {
            System.out.println("2018 is not a Leap year");
        }
    }
}
```
<a name="hmAJJ"></a>
### 15、计算两个日期之间的天数和月数
有一个常见日期操作是计算两个日期之间的天数、周数或月数。在Java 8中可以用`java.time.Period`类来做计算。<br />下面这个例子中，计算了当天和将来某一天之间的月数。
```java
import java.time.LocalDate;
import java.time.Period;
public class Demo15 {
    public static void main(String[] args) {
        LocalDate today = LocalDate.now();
        LocalDate java8Release = LocalDate.of(2018, 12, 14);
        Period periodToNextJavaRelease = Period.between(today, java8Release);
        System.out.println("Months left between today and Java 8 release : "
                + periodToNextJavaRelease.getMonths() );
    }
}
```
<a name="NtbYc"></a>
### 16、在Java 8中获取当前的时间戳
`Instant`类有一个静态工厂方法`now()`会返回当前的时间戳，如下所示：
```java
import java.time.Instant;
public class Demo16 {
    public static void main(String[] args) {
        Instant timestamp = Instant.now();
        System.out.println("What is value of this instant " + timestamp.toEpochMilli());
    }
}
```
时间戳信息里同时包含了日期和时间，这和`java.util.Date`很像。实际上`Instant`类确实等同于 Java 8之前的`Date`类，可以使用`Date`类和`Instant`类各自的转换方法互相转换，例如：`Date.from(Instant)` 将`Instant`转换成`java.util.Date`，`Date.toInstant()`则是将`Date`类转换成`Instant`类。
<a name="Mi1LA"></a>
### 17、Java 8中使用预定义的格式化工具去解析或格式化日期
```java
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
public class Demo17 {
    public static void main(String[] args) {
        String dayAfterTommorrow = "20180205";
        LocalDate formatted = LocalDate.parse(dayAfterTommorrow,
                DateTimeFormatter.BASIC_ISO_DATE);
        System.out.println(dayAfterTommorrow+"  格式化后的日期为:  "+formatted);
    }
}
```
<a name="LoJKB"></a>
### 18、字符串互转日期类型
```java
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
public class Demo18 {
    public static void main(String[] args) {
        LocalDateTime date = LocalDateTime.now();
        DateTimeFormatter format1 = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
        //日期转字符串
        String str = date.format(format1);
        System.out.println("日期转换为字符串:"+str);
        DateTimeFormatter format2 = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
        //字符串转日期
        LocalDate date2 = LocalDate.parse(str,format2);
        System.out.println("日期类型:"+date2);
    }
}
```
<a name="rbYqp"></a>
### 19、按月生成开始结束时间范围内的时间区间
```java
@Test
public void generatorDateTest() {
    LocalDate startDate = LocalDate.parse("2021-06-15", DateTimeFormatter.ISO_LOCAL_DATE);
    LocalDate endDate = LocalDate.parse("2021-08-16", DateTimeFormatter.ISO_LOCAL_DATE);
    Period period = Period.between(startDate, endDate);
    int months = period.getYears() * 12 + period.getMonths();
    for (int i = 0; i <= months; i++) {
        if (i == 0) {
            System.out.printf("Start:"+ startDate);
            System.out.println("End:"+ startDate.with(TemporalAdjusters.lastDayOfMonth()));
        }else if (i == months) {
            System.out.println("Start:"+ startDate.plusMonths(i).with(TemporalAdjusters.firstDayOfMonth()));
            System.out.println("End:"+ endDate);
        }else {
            System.out.println("Start:"+ startDate.plusMonths(i).with(TemporalAdjusters.firstDayOfMonth()));
            System.out.println("End:"+ startDate.plusMonths(i).with(TemporalAdjusters.lastDayOfMonth()));
        }
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657953439995-70315db0-98fa-4564-a575-4e560b35be87.png#averageHue=%23f8f8f7&clientId=ue0dc59e9-b0b9-4&from=paste&height=274&id=u598161fb&originHeight=411&originWidth=1774&originalType=binary&ratio=1&rotation=0&showTitle=false&size=65750&status=done&style=shadow&taskId=u4ef9edfa-396d-4147-97b0-24fa0d71646&title=&width=1182.6666666666667)
<a name="BNIyK"></a>
### 20、包含时差信息的日期和时间
ZoneOffset 类用来表示时区，举例来说印度与 GMT 或 UTC 标准时区相差+05:30，可以通过 `ZoneOffset.of()`静态方法来 获取对应的时区。一旦得到了时差就可以通过传入 `LocalDateTime` 和 `ZoneOffset` 来创建一个 `OffSetDateTime` 对象。
```java
//static
public static void zoneOffset(){
    LocalDateTime datetime = LocalDateTime.of(2018, Month.FEBRUARY, 14, 19, 30);
    ZoneOffset offset = ZoneOffset.of("+05:30");
    OffsetDateTime date = OffsetDateTime.of(datetime, offset);
    System.out.println("Date and Time with timezone offset in Java : " + date);
}
```
输出
```
Date and Time with timezone offset in Java : 2018-02-14T19:30+05:30
```
