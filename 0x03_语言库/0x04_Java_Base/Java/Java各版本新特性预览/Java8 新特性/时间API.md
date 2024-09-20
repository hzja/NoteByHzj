Java 时间日期<br />Java 8 推出了全新的日期时间API。<br />Java处理日期、日历和时间的不足之处：将 `java.util.Date` 设定为可变类型，以及 `SimpleDateFormat` 的非线程安全使其应用非常受限。然后就在 java8 上面增加新的特性。<br />在旧版的 Java 中，日期时间 API 存在诸多问题，例如：

- **非线程安全**：`java.util.Date` 是非线程安全的，所有的日期类都是可变的，这是Java日期类最大的问题之一。
- **设计很差**：Java的日期/时间类的定义并不一致，在`java.util`和`java.sql`的包中都有日期类，此外用于格式化和解析的类被定义在`java.text`包中。`java.util.Date`同时包含日期和时间，而`java.sql.Date`仅包含日期，将其纳入`java.sql`包并不合理。另外这两个类都有相同的名字，这本身就是一个非常糟糕的设计。
- **时区处理麻烦**：日期类并不提供国际化，没有时区支持，因此 Java 引入了`java.util.Calendar`和`java.util.TimeZone`类，但他们同样存在上述所有的问题。

因为上面这些原因，诞生了第三方库`Joda-Time`，可以替代 Java 的时间管理 API 。<br />Java 8 中新的时间和日期管理 API 深受`Joda-Time`影响，并吸收了很多`Joda-Time`的精华，新的`java.time`包包含了所有关于日期、时间、时区、`Instant`（跟日期类似但是精确到纳秒）、`Duration`（持续时间）和时钟操作的类。<br />新设计的 API 认真考虑了这些类的不变性，如果某个实例需要修改，则返回一个新的对象。<br />全新API的众多好处之一就是，明确了日期时间概念，例如：`瞬时（instant）`、 `长短（duration）`、`日期`、`时间`、`时区`和`周期`。<br />同时继承了Joda 库按人类语言和计算机各自解析的时间处理方式。不同于老版本，新API基于ISO标准日历系统，java.time包下的所有类都是不可变类型而且线程安全。

- `Instant`：瞬时实例。
- `LocalDate`：本地日期，不包含具体时间 例如：2014-01-14 可以用来记录生日、纪念日、加盟日等。
- `LocalTime`：本地时间，不包含日期。
- `LocalDateTime`：组合了日期和时间，但不包含时差和时区信息。
- `ZonedDateTime`：最完整的日期时间，包含时区和相对UTC或格林威治的时差。

新API还引入了 `ZoneOffSet` 和 `ZoneId` 类，使得解决时区问题更为简便。解析、格式化时间的 `DateTimeFormatter`类也全部重新设计。
<a name="SXuLY"></a>
## 1、`Clock`类——获取当前的纳秒时间和日期
`Clock`类使用时区来返回当前的纳秒时间和日期。`Clock`可以替代`System.currentTimeMillis()`和`TimeZone.getDefault()`，实例如下：
```java
final Clock clock = Clock.systemUTC();
System.out.println( clock.instant() );
System.out.println( clock.millis() );
```
输出结果是
```
2021-02-24T12:24:54.678Z
1614169494678
```
<a name="dqbt2"></a>
## 2、`LocalDate`、`LocalTime`、`LocalDateTime`类
`LocalDate`、`LocalTime`、`LocalDateTime`类的实例是不可变的对象，分别表示使用ISO-8601日历系统的日期、时间、日期和时间。它们提供了简单的日期或时间，并不包含当前的时间信息，也不包含与时区相关的信息。<br />注：ISO-8601日历系统是国际标准化组织指定的现代公民的日期和时间的表示法。<br />**核心类**
```
LocalDate：不包含具体时间的日期。
LocalTime：不含日期的时间。
LocalDateTime：包含了日期及时间。
```
<a name="zvGlc"></a>
### `LocalDate`常用API——只获取年月日

- `getYear()`    int    获取当前日期的年份
- `getMonth()`    Month    获取当前日期的月份对象
- `getMonthValue()`    int    获取当前日期是第几月
- `getDayOfWeek()`    DayOfWeek    表示该对象表示的日期是星期几
- `getDayOfMonth()`    int    表示该对象表示的日期是这个月第几天
- `getDayOfYear()`    int    表示该对象表示的日期是今年第几天
- `withYear(int year)`    LocalDate    修改当前对象的年份
- `withMonth(int month)`    LocalDate    修改当前对象的月份
- `withDayOfMonth(int dayOfMonth)`    LocalDate    修改当前对象在当月的日期
- `plusYears(long yearsToAdd)`    LocalDate    当前对象增加指定的年份数
- `plusMonths(long monthsToAdd)`    LocalDate    当前对象增加指定的月份数
- `plusWeeks(long weeksToAdd)`    LocalDate    当前对象增加指定的周数
- `plusDays(long daysToAdd)`    LocalDate    当前对象增加指定的天数
- `minusYears(long yearsToSubtract)`    LocalDate    当前对象减去指定的年数
- `minusMonths(long monthsToSubtract)`    LocalDate    当前对象减去注定的月数
- `minusWeeks(long weeksToSubtract)`    LocalDate    当前对象减去指定的周数
- `minusDays(long daysToSubtract)`    LocalDate    当前对象减去指定的天数
- `compareTo(ChronoLocalDate other)`    int    比较当前对象和other对象在时间上的大小，返回值如果为正，则当前对象时间较晚，
- `isBefore(ChronoLocalDate other)`    boolean    比较当前对象日期是否在other对象日期之前
- `isAfter(ChronoLocalDate other)`    boolean    比较当前对象日期是否在other对象日期之后
- `isEqual(ChronoLocalDate other)`    boolean    比较两个日期对象是否相等
<a name="iwkPE"></a>
#### 创建`LocalDate`
```java
//获取当前年月日  
LocalDate localDate = LocalDate.now();  
//构造指定的年月日  
LocalDate localDate1 = LocalDate.of(2019, 9, 10);
```
<a name="bvwih"></a>
#### 获取年、月、日、星期几
```java
int year = localDate.getYear();  
int year1 = localDate.get(ChronoField.YEAR);  
Month month = localDate.getMonth();  
int month1 = localDate.get(ChronoField.MONTH_OF_YEAR);  
int day = localDate.getDayOfMonth();  
int day1 = localDate.get(ChronoField.DAY_OF_MONTH);  
DayOfWeek dayOfWeek = localDate.getDayOfWeek();  
int dayOfWeek1 = localDate.get(ChronoField.DAY_OF_WEEK);
```
<a name="Zh5ae"></a>
### `LocalTime`——只获取几点几分几秒
<a name="ieyg8"></a>
#### 创建`LocalTime`
```java
LocalTime localTime = LocalTime.of(13, 51, 10);  
LocalTime localTime1 = LocalTime.now();
```
<a name="9N5Sz"></a>
#### 获取时分秒
```java
//获取小时  
int hour = localTime.getHour();  
int hour1 = localTime.get(ChronoField.HOUR_OF_DAY);  
//获取分  
int minute = localTime.getMinute();  
int minute1 = localTime.get(ChronoField.MINUTE_OF_HOUR);  
//获取秒  
int second = localTime.getSecond();  
int second1 = localTime.get(ChronoField.SECOND_OF_MINUTE);
```
<a name="a5gp5"></a>
### `LocalDateTime`——获取年月日时分秒，等于`LocalDate`+`LocalTime`
<a name="6lzad"></a>
#### 创建`LocalDateTime`
```java
LocalDateTime localDateTime = LocalDateTime.now();  
LocalDateTime localDateTime1 = LocalDateTime.of(2019, Month.SEPTEMBER, 10, 14, 46, 56);  
LocalDateTime localDateTime2 = LocalDateTime.of(localDate, localTime);  
LocalDateTime localDateTime3 = localDate.atTime(localTime);  
LocalDateTime localDateTime4 = localTime.atDate(localDate);
```
<a name="SW5BN"></a>
#### 获取`LocalDate`
```java
LocalDate localDate2 = localDateTime.toLocalDate();
```
<a name="pdso2"></a>
#### 获取`LocalTime`
```java
LocalTime localTime2 = localDateTime.toLocalTime();
```
<a name="dbnxU"></a>
## 3、`Instant`类——时间戳、获取秒数
<a name="nM0w5"></a>
### 创建`Instant`对象
```java
Instant instant = Instant.now();
```
<a name="j2gNs"></a>
### 获取秒数
```java
long currentSecond = instant.getEpochSecond();
```
<a name="354e8659"></a>
### 获取毫秒数
```java
long currentMilli = instant.toEpochMilli();
```
```java
// 2.Instant:时间戳（以Unix元年：1970年1月1日 00:00:00 到某个时间之间的毫秒值
@Test
public void instantTest() {
    // 默认获取UTC时区
    Instant instant = Instant.now();
    System.out.println(instant);

    // 在原来的时间上加8个小时
    OffsetDateTime offsetDateTime = instant.atOffset(ZoneOffset.ofHours(8));
    System.out.println(offsetDateTime);

    System.out.println(instant.toEpochMilli());
    // 在Unix元年时间戳加60秒
    Instant ofEpochSecond = Instant.ofEpochSecond(60);
    System.out.println(ofEpochSecond);
}
```
<a name="1kmXC"></a>
## 4、时间计算
<a name="tYoxq"></a>
### `Duration`：计算两个时间的时间差
```java
/**
 * Period:计算两个日期之间的差
 */
@Test
public void calTimeTest() throws InterruptedException {
    System.out.println("------------计算两个时间之间的间隔-----------");
    Instant now = Instant.now();
    Thread.sleep(1000);
    Instant end = Instant.now();
    Duration duration = Duration.between(now, end);
    System.out.println(duration.toMillis());

    System.out.println("--------------------------");
    LocalTime localTime = LocalTime.now();
    Thread.sleep(1000);
    LocalTime l = LocalTime.now();
    Duration between = Duration.between(localTime, l);
    System.out.println(between.toMillis());
}
```
<a name="aceb8271"></a>
### `Period`：计算两个日期之间的差
```java
/**
 * Duration:计算两个时间的时间差
 */
@Test
public void calTimeTest() throws InterruptedException {
    System.out.println("------------计算两个日期之间的间隔-----------");

    LocalDate localDate = LocalDate.of(2025, 6, 12);
    LocalDate date = LocalDate.now();
    Period period = Period.between(date, localDate);
    System.out.println(period.getYears());
    System.out.println(period.getMonths());
    System.out.println(period.getDays());
}
```
<a name="gY7cQ"></a>
## 5、日期的操纵-时间校正器
<a name="Peteg"></a>
### `TemporalAdjuster`：时间校正器
有时可能需要获取将日期调整至“下个周日”等操作。
```java
LocalDate nextSunday = LocalDate.now().with(
    TemporalAdjusters.next(DayOfWeek.SUNDAY)
);
// 修改年为2019  
localDateTime = localDateTime.withYear(2020);
// 修改为2022  
localDateTime = localDateTime.with(ChronoField.YEAR, 2022);
```
`TemporalAdjusters`：该类通过静态方法提供了大量的常用`TemporalAdjuster`的实现。
```java
// TemporalAdjuster ： 时间校正器
@Test
public void temporalAdjusterTest() {
    LocalDateTime now = LocalDateTime.now();
    System.out.println(now);

    LocalDateTime localDateTime = now.withDayOfMonth(10);
    System.out.println(localDateTime);

    LocalDateTime friday = now.with(TemporalAdjusters.next(DayOfWeek.FRIDAY));
    System.out.println(friday);

    // 自定义：下一个工作日
    LocalDateTime nextWorkDay = now.with((d) -> {
        LocalDateTime dateTime = (LocalDateTime) d;
        DayOfWeek dayOfWeek = dateTime.getDayOfWeek();
        if (dayOfWeek.equals(DayOfWeek.FRIDAY)) {
            return dateTime.plusDays(3);
        } else if (dateTime.equals(DayOfWeek.FRIDAY)) {
            return dateTime.plusDays(2);
        } else {
            return dateTime.plusDays(1);
        }
    });
    System.out.println(nextWorkDay);
}
```
<a name="Qxs0f"></a>
#### 通过`firstDayOfYear()`返回了当前日期的第一天日期
```java
import java.time.LocalDate;
import static java.time.temporal.TemporalAdjusters.firstDayOfYear;

public class TestTemporalAdjusters {

    public static void main(String[] args) {
        LocalDate.now().with(firstDayOfYear());
    }

}
```
<a name="NOLtb"></a>
## 6、`DateTimeFormatter`——格式化时间和日期
<a name="37cef521"></a>
### 解析时间
```java
LocalDate localDate1 = LocalDate.parse("20190910", DateTimeFormatter.BASIC_ISO_DATE);  
LocalDate localDate2 = LocalDate.parse("2019-09-10", DateTimeFormatter.ISO_LOCAL_DATE);
```
和SimpleDateFormat相比，DateTimeFormatter是线程安全的。
```java
// DateTimeFormatter:格式化时间日期
@Test
public void dateTimeFormatterTest() {
    // 使用官方提供的格式
    DateTimeFormatter ISO_DATE = DateTimeFormatter.ISO_DATE;
    LocalDateTime now = LocalDateTime.now();

    String formatDate = now.format(ISO_DATE);
    System.out.println(formatDate);

    // 自定义格式
    DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern("yyyy年MM月dd日 HH:mm:ss");
    String selfDate = dateTimeFormatter.format(now);
    System.out.println(selfDate);

    // 字符串日期转回日期类型
    LocalDateTime parseDate = now.parse(selfDate, dateTimeFormatter);
    System.out.println(parseDate);
}
```
<a name="JMvRV"></a>
### 将`Date`类型的日期转换为`LocalDateTime`
```java
/**
 * 将Date类型的日期转换为LocalDateTime
 *
 * @param date Date类型的对象
 * @return LocalDateTime
 * @author @author Fcant 下午 18:27 2020/6/30/0030
 */
public static LocalDateTime dateToLocalDateTime(Date date) {
    return LocalDateTime.ofInstant(date.toInstant(), ZoneId.systemDefault());
}
```
<a name="XQJdn"></a>
### 将`LocalDate`日期进行格式化
```java
/**
 * 将LocalDate日期进行格式化
 *
 * @param date 格式化的Date对象
 * @param format 格式化的样式："yyyy-MM-dd HH:mm:ss"
 * @return String
 * @author @author Fcant 下午 18:27 2020/6/30/0030
 */
public static String formatDateTime(Date date, String format) {
    LocalDateTime localDateTime = dateToLocalDateTime(date);
    return formatLocalDateTime(localDateTime, format);
}

/**
 * 格式化LocalDateTime
 *
 * @param localDateTime 要格式化的localDateTime
 * @param format 格式化的样式："yyyy-MM-dd HH:mm:ss"
 * @return String
 * @author @author Fcant 下午 18:27 2020/6/30/0030
 */
public static String formatLocalDateTime(LocalDateTime localDateTime, String format) {
    return DateTimeFormatter.ofPattern(format).format(localDateTime);
}
```
<a name="yjQzi"></a>
### 格式化`LocalDateTime`
```java
/**
 * 格式化LocalDateTime
 *
 * @param localDateTime 要格式化的localDateTime
 * @param format 格式化的样式："yyyy-MM-dd HH:mm:ss"
 * @return String
 * @author @author Fcant 下午 18:27 2020/6/30/0030
 */
public static String formatLocalDateTime(LocalDateTime localDateTime, String format) {
    return DateTimeFormatter.ofPattern(format).format(localDateTime);
}
```
<a name="cn3wC"></a>
### `String`类型的日期转换为`LocalDateTime`
```java
/**
 * String类型的日期转换为LocalDateTime
 *
 * @param date String类型的日期参数
 * @param fmt String类型的日期的格式
 * @return LocalDateTime
 * @author Fcant 下午 17:22 2020/7/9/0009
 */
public static LocalDateTime stringToLocal(String date, String fmt) {
    DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern(fmt);
    return LocalDateTime.parse(date, dateTimeFormatter);
}
```
<a name="at5DE"></a>
### 将`LocalDateTime`转换为`Date`类型的日期
```java
/**
 * 将LocalDateTime转换为Date类型的日期
 *
 * @param localDateTime 要转换的LocalDateTime参数
 * @return Date
 * @author Fcant 下午 17:21 2020/7/9/0009
 */
public static Date localToDate(LocalDateTime localDateTime) {
    ZoneId zoneId = ZoneId.systemDefault();
    ZonedDateTime zonedDateTime = localDateTime.atZone(zoneId);
    return Date.from(zonedDateTime.toInstant());
}
```
<a name="Ka64N"></a>
## 7、时区的操作
Java8加入了对时区的支持，带时区的时间分别为：<br />`ZoneDate`、`ZoneTime`、`ZoneDateTime`，其中每个时区都对应着ID，地区ID都为“{区域}/{城市}”的格式，例如Asis/Shanghai等<br />`ZoneId`：该类中包含了所有的时区信息。<br />`getAvailableZoneIDs()`：可以获取所有时区信息；<br />`of(id)`：用指定的时区信息获取ZoneId对象。
```java
// ZoneDate、ZoneTime、ZoneDateTime
@Test
public void zoneDateTimeTest() {
    Set<String> availableZoneIds = ZoneId.getAvailableZoneIds();
    availableZoneIds.forEach(System.out::println);

    // 根据指定时区获取当前时间
    LocalDateTime now = LocalDateTime.now(ZoneId.of("Africa/Nairobi"));
    System.out.println(now);

    // 获取时间并为之指定时区
    LocalDateTime localDateTime = LocalDateTime.now();
    ZonedDateTime zonedDateTime = localDateTime.atZone(ZoneId.of("US/Pacific"));
    System.out.println(zonedDateTime);
}
```
```java
// 获取当前时间日期
final ZonedDateTime zonedDatetime = ZonedDateTime.now();
//获取指定时钟的日期时间
final ZonedDateTime zonedDatetimeFromClock = ZonedDateTime.now( clock );
//获取纽约时区的当前时间日期
final ZonedDateTime zonedDatetimeFromZone = ZonedDateTime.now( ZoneId.of("America/New_York") );

System.out.println( zonedDatetime );
System.out.println( zonedDatetimeFromClock );
System.out.println( zonedDatetimeFromZone );
```
```java
2021-02-24T20:42:27.238+08:00[Asia/Shanghai]
2021-02-24T12:42:27.238Z
2021-02-24T07:42:27.241-05:00[America/New_York]
```
<a name="oNQeB"></a>
## 8、SpringBoot中应用`LocalDateTime`
<a name="IMfV4"></a>
### 将`LocalDateTime`字段以时间戳的方式返回给前端
添加日期转化类
```java
public class LocalDateTimeConverter extends JsonSerializer<LocalDateTime> {  
    @Override  
    public void serialize(LocalDateTime value, JsonGenerator gen, SerializerProvider serializers) throws IOException {  
    gen.writeNumber(value.toInstant(ZoneOffset.of("+8")).toEpochMilli());  
    }  
}
```
并在`LocalDateTime`字段上添加
```
@JsonSerialize(using = LocalDateTimeConverter.class)
```
注解，如下：
```java
@JsonSerialize(using = LocalDateTimeConverter.class)  
protected LocalDateTime gmtModified;
```
<a name="6CQUN"></a>
### 将`LocalDateTime`字段以指定格式化日期的方式返回给前端
在`LocalDateTime`字段上添加
```java
@JsonFormat(shape=JsonFormat.Shape.STRING, pattern="yyyy-MM-dd HH:mm:ss")
```
注解即可，如下：
```java
@JsonFormat(shape=JsonFormat.Shape.STRING, pattern="yyyy-MM-dd HH:mm:ss")  
protected LocalDateTime gmtModified;
```
<a name="OiMiL"></a>
### 对前端传入的日期进行格式化
在`LocalDateTime`字段上添加
```java
@DateTimeFormat(pattern = "yyyy-MM-dd HH:mm:ss")
```
注解即可，如下：
```java
@DateTimeFormat(pattern = "yyyy-MM-dd HH:mm:ss")  
protected LocalDateTime gmtModified;
```
