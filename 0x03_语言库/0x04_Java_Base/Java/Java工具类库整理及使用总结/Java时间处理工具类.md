Java 时间工具类<br />xk-time 是时间转换，时间计算，时间格式化，时间解析，日历，时间cron表达式和时间NLP等的工具，使用Java8，线程安全，简单易用，多达70几种常用日期格式化模板，支持Java8时间类和Date，轻量级，无第三方依赖。
<a name="JKcJO"></a>
### 为什么要开发这个工具？
<a name="sYFjq"></a>
#### （1）Java8以前的Date API设计不太好，使用不方便，往往会有线程安全问题。
xk-time工具包，使用java8 api，其中`Instant`、`LocalDate`、`LocalDateTime`、`LocalTime`、`ZonedDateTime`等都是线程安全的类，而且增加了更丰富的方法，在此基础上开发相关工具类，线程安全，让使用更方便。
<a name="vhuT9"></a>
#### （2）常见的DateUtil，往往将时间转换，计算，格式化，解析等功能都放在同一个类中，导致类功能复杂，方法太多，查找不方便。
xk-time工具包，将上面功能按照时间转换，时间计算，时间格式化解析分成3个工具类：`DateTimeConverterUtil`，`DateTimeCalculatorUtil`，`DateTimeFormatterUtil`，每个类只做一个种功能，方便使用。
<a name="pHua4"></a>
#### （3）为了将与时间紧密相关的节假日、农历、二十四节气、十二星座、十二生肖、十二时辰和日历等功能集中起来开发成工具，方便使用。
<a name="BvabF"></a>
### 主要功能说明
<a name="SlajX"></a>
#### 1.日期转换工具类   `DateTimeConverterUtil`
包含`Date`、`LocalDate`、`LocalDateTime`、`LocalTime`、`Instant`、`ZonedDateTime`、`YearMonth`、`Timestamp`和`long`等互相转换    <br />注意，`ZonedDateTime`相关的转换，尤其是其他时间转`ZonedDateTime`，要注意时间和对应时区一致。<br />详细使用可以查看相关测试代码。
<a name="M9iHI"></a>
#### 2.日期计算工具类  `DateTimeCalculatorUtil`
包括： <br />（1）获取时间属性方法（支持年月日时分秒毫秒，星期，时间戳等），`get*` 比如`getYear(Date date) `获取年部分，`getMonthCnLong(Date date)`获取月份中文，`getDayOfWeekCn(Date date)`，获取星期中文。<br />（2）获取时间加操作方法，`plus*` 比如`plusYears(Date date, long amountToAdd)` 当前时间年增加`amountToAdd`值。<br />（3）获取时间减操作方法，`minus*` 比如`minusYears(Date date, long amountToSubtract)` 当前时间年减少`amountToSubtract`值。<br />（4）获取时间修改属性方法，`with*` 比如`withYear(Date date, long newValue)` 修改当前时间年值为`newValue`。<br />（5）获取比较2个时间方法，`between*` 比如`betweenYears(Date startInclusive, Date endExclusive)` 比较2个时间，获取年部分。<br />（6）其他常用方法，比如`isLeapYear(Date date)` 判断是否闰年，`isWeekend(Date date)` 判断是否周末，`isExpiry(String yearMonthStr)` 是否过期等<br />（7）时区转换计算方法，`transform*`，比如`transform(ZonedDateTime zonedDateTime, String zoneId)`<br />（8）比较2个时间大小和相等方法，`compare*`，比如`compare(Date date1, Date date2)`<br />（9）获取准确的起始时间方法，`start`,`end`，比如`startTimeOfMonth()` 当月起始时间 当月第一天日期+00:00:00，`endTimeOfMonth()` 当月最后一天日期+23:59:59  精确到秒；`endAccuracyTimeOf*`，精确到毫秒（Date），精确到纳秒（`LocalDateTime`）。<br />（10）相同月日比较判断方法，`isSameMonthDay`，`betweenNextSameMonthDay`，`nextSameMonthDay*`， 比如用于生日，节日等周期性的日期比较判断。<br />（11）星座计算方法，`getConstellation*`，比如`getConstellationNameCn(String monthDayStr)`，根据日期计算星座。<br />（12）计算指定年月或起始时间区间的时间列表，`get*List`， 比如`getDateList(int year, int month)`，计算指定年月的时间列表。<br />（13）减少时间精度方法，`reduceAccuracyTo*`， 比如`reduceAccuracyToDay(Date date)`，减少时间精度到天，其他补0，返回如，2020-04-23 00:00:00。<br />（14）获取时间戳方法，`getEpoch*`， 比如`getEpochMilli()`获取时间戳，`getEpochMilliFormat()`获取时间戳格式化字符串（yyyy-MM-dd HH:mm:ss）<br />（15）计算年龄方法，`getAge*`， 比如`getAge(Date birthDay)`，通过生日计算年龄。<br />（16）判断是否到生日方法，`isBirthDay*`， 比如`isBirthDay(Date birthDay)`，根据生日判断当前日期是否到生日。<br />（17）周数计算方法，`weekof*`， 比如`weekOfMonth(Date date)`，日期所在月中第几周。<br />（18）判断星期一，星期五方法，`isMonday`,`isZhouYi`， 比如`isZhouYi(Date date)`，是否为周一。<br />（19）十二时辰计算方法，`getTwelveTwo*`， 比如`getTwelveTwo(Date date)`，获取指定时间对应的十二时辰。<br />（20）季度计算方法，`getQuarter*`， 比如`getQuarter(Date date)`，获取指定时间对应的季度。<br />（21）获取季度准确的起始时间方法（四个季度），`startTimeOf*Quarter`， 比如`startTimeOfFirstQuarter(int year)`，获取指定年的第一季度。<br />（22） 获取年准确的起始时间方法，`startTimeOfYear`， 比如`startTimeOfYear(int year)`，获取指定年的开始时间。<br />（23）常用时间（明天，下周，下月，明年等）计算方法，比如`tomorrow()`，计算明天，返回Date。<br />（24）修改星期值方法 `withDayOfWeek*`，比如`withDayOfWeek(Date date, long newValue)`，修改星期为指定值newValue，返回Date。<br />（25）中国工作日计算（将放假信息包含在内），包括判断当前日期是否为工作日和下一个工作日等方法， `isChineseWorkDay`，`nextChineseWorkDay`，  <br />比如  `isChineseWorkDay(Date, String holidayData)`，`nextChineseWorkDay(Date date, String holidayData)`，  <br />节假日数据holidayData，如果节假日数据不支持年份，将使用周一到周五为工作日来判断。<br />详细使用可以查看相关测试代码。
<a name="qGhVZ"></a>
#### 3.日期格式化和解析工具类  `DateTimeFormatterUtil`
包含常用日期格式如： <br />yyyy-MM-dd  <br />HH:mm:ss  <br />yyyy-MM-dd HH:mm:ss  <br />yyyy-MM-dd HH:mm:ss.SSS  <br />yyyy-MM-dd HH:mm:ss.SSSSSS  <br />yyyy-MM-dd HH:mm:ss.SSSSSSSSS  <br />yyyy-MM-dd'T'HH:mm:ssZ等等 ，支持毫秒、微秒和纳秒等精确时间。<br />（1）格式化方法， `format*`， 比如`formatToDateStr(Date date)` 格式化，返回日期部分，如：yyyy-MM-dd； <br />`format(Date date, DateTimeFormatter formatter)` formatter 可以选择已定义好的`formatter`比如YYYY_MM_DD_HH_MM_SS_FMT（yyyy-MM-dd HH:mm:ss）格式化日期。<br />（2）解析方法， parse*， 比如`parseDateStrToDate(String text)` 解析日期yyyy-MM-dd，返回Date； <br />`parseToDate(String text, DateTimeFormatter formatter)` 根据 `formatter`解析为 Date。<br />（3）自动解析方法，根据字符串特点自动识别解析，`smartParse*`，比如`smartParseToDate(String text)` 自动解析Date。<br />（4）ISO格式（包含T）自动解析方法，根据字符串特点自动识别解析，`parseIso*`，比如`parseIsoToDate(String text)` 自动解析Date。<br />（5）解析时间戳方法, `parseEpochMilli*`， 比如`parseEpochMilliToDate(String text)`，解析时间戳为Date，如 1590224790000。<br />（6）解析Date默认格式，`parseDateDefaultStr*`，比如`parseDateDefaultStrToDate(String text)`  <br />解析 EEE MMM dd HH:mm:ss zzz yyyy 比如：Sat May 23 17:06:30 CST 2020 为Date。<br />（7）自定义时区格式化方法，比如 `format(Date date, DateTimeFormatter formatter, String zoneId)`，根据`zoneId`格式化Date。<br />（8）自定义模板格式化方法，比如 `format(Date date, String dateFormatPattern)`，根据`dateFormatPattern`格式化Date。<br />（9）自定义模板解析方法，比如 `parseToDate(String text, String dateFormatPattern)`，根据`dateFormatPattern`格式化Date。<br />（10）`Timestamp`默认格式（yyyy-mm-dd hh:mm:ss.fffffffff 其中 fffffffff 纳秒，省略后面的0）格式化方法。   <br />比如 `formatTimestampStyle(Date date)`。<br />（11）`Timestamp`默认格式（yyyy-mm-dd hh:mm:ss.fffffffff 其中 fffffffff 纳秒，省略后面的0）解析方法。   <br />比如 `parseTimestampStyleToDate(String text)`。<br />（12）验证日期格式是否正确方法，`isValidDate*`， 比如`isValidDate(String text)`，验证yyyy-MM-dd 格式字符串是否正确。<br />（13）根据自定义模板数组解析方法， 比如`parseToDate(String text, String[] dateFormatPatterns)`，`dateFormatPatterns` 支持多种模板，只要其中一个解析成功就返回对应Date。<br />（14）解析自然语言时间，今天，明天，下周，下月，明年，昨天，上周，上月，去年等， 比如`parseNaturalLanguageToDate(String text)`,  <br />`parseNaturalLanguageToDate(String text, MapnaturalLanguageMap)` 支持自定义解析自然语言时间map<br />注意：格式化和解析与系统时区不同的时间时，使用自定义时区格式化方法，或可以使用withZone方法重新设置时区，比如： <br />`YYYY_MM_DD_HH_MM_SS_SSS_FMT.withZone(ZoneId.of("Europe/Paris")` 。<br />详细使用可以查看相关测试代码。
<a name="Opl5I"></a>
#### 4.日历工具类  `CalendarUtil`
包括： <br />（1）生成指定时间的日历（包含年、月和日层级关系的日历）方法，`generateCalendar*` 比如`generateCalendar(int year, int month)` 生成指定年月的日历。 <br />（2）生成指定时间的日历（包含年、月和日层级关系的日历），包含农历和所有节假日信息方法，`generateCalendarWithHoliday*`， 比`generateCalendarWithHoliday(int year, int month, MaplocalHolidayMap,MapchineseHolidayMap, MapdateTypeMap)`生成指定年月的日历，包含农历和所有节假日信息，可以自定义节假日和工作日等。<br />详细使用可以查看相关测试代码。
<a name="WsKSN"></a>
#### 5.农历日期类 `LunarDate`
包含： <br />（1）农历日期年月日计算。 <br />（2）农历岁次，生肖属相计算。 <br />（3）二十四节气计算等  <br />注意：仅支持公历1900-2100年的农历转换。<br />详细使用可以查看相关测试代码。
<a name="UbGyQ"></a>
#### 6.节假日计算工具类 `HolidayUtil`
包含： <br />（1）公历节假日计算， `getLocalHoliday*` 比如`getLocalHoliday(Date date)` 计算date的公历节日，`getLocalHoliday(Date date, MaplocalHolidayMap)` 可以传入自定义公历节日数据。 <br />（2）农历节假日计算， `getChineseHoliday*` 比如`getChineseHoliday(Date date)` 计算date的农历节日，`getChineseHoliday(Date date, MapchineseHolidayMap)` 可以传入自定义农历节日数据。 <br />（3）二十四节气计算， `getSolarTerm*` 比如`getSolarTerm(Date date)` 计算date的二十四节气。<br />注意：农历和二十四节气使用农历日期类 `LunarDate`，仅支持公历1900-2100年的计算。<br />详细使用可以查看相关测试代码。
<a name="f02MW"></a>
#### 7.Cron表达式工具类 `CronExpressionUtil`
cron表达式从左到右（用空格隔开）：秒（0-59） 分（0-59） 小时（0-23） 日期（1-31） 月份（1-12的整数或者 JAN-DEC） 星期（1-7的整数或者 SUN-SAT （1=SUN）） 年份（可选，1970-2099）  <br />所有字段均可使用特殊字符：, - * / 分别是枚举，范围，任意，间隔  <br />日期另外可使用：? L W 分别是任意，最后，有效工作日(周一到周五)  <br />星期另外可使用：? L # 分别是任意，最后，每个月第几个星期几  <br />常用cron表达式： <br />（1）`0 0 2 1 * ? * `  表示在每月的1日的凌晨2点触发  <br />（2）`0 15 10 ? * MON-FRI`   表示周一到周五每天上午10:15执行作业  <br />（3）`0 15 10 ? * 6L 2002-2006`   表示2002-2006年的每个月的最后一个星期五上午10:15执行作  <br />（4）`0 0/30 9-17 * * ?`   朝九晚五工作时间内每半小时  <br />（5）`0 15 10 L * ?`    每月最后一日的上午10:15触发  <br />（6）`0 15 10 ? * 6#3`   每月的第三个星期五上午10:15触发<br />包含  <br />（1）验证和格式化Cron表达式方法，`isValidExpression`和`formatExpression`。 <br />（2）生成下一个或多个执行时间方法，`getNextTime`和`getNextTimeList`。 <br />（3）生成下一个或多个执行时间的日期格式化（yyyy-MM-dd HH:mm:ss）方法，`getNextTimeStr`和`getNextTimeStrList`。 <br />（4）对比Cron表达式下一个执行时间是否与指定date相等方法，`isSatisfiedBy`。<br />注意：底层使用 quartz的`CronExpression`处理。<br />详细使用可以查看相关测试代码。
<a name="G0zGL"></a>
#### 8.计算耗时工具 CostUtil
计算耗时工具，支持秒，毫秒，纳秒<br />包括： <br />（1）计算耗时，返回耗时结果。 <br />（2）计算耗时，自定义任务名称，输出耗时结果。 <br />（3）计算耗时，返回精确计时，带3小数的结果，使用`ROUND_DOWN`舍弃超过3位的小数部分等。<br />详细使用可以查看相关测试代码。
<a name="t14xO"></a>
#### 9.时间自然语言分析工具类（NLP） TimeNLPUtil
包括功能： <br />（1）以当前时间为基础分析时间自然语言。 <br />（2）以指定时间为基础分析时间自然语言。<br />修改自 [https://github.com/shinyke/Time-NLP](https://github.com/shinyke/Time-NLP)  <br />做了一些修改如下： <br />（1）封装属性，重命名使符合驼峰命名标准。 <br />（2）将加载正则资源文件改为单例加载。 <br />（3）将类按照功能重新划分为单独的多个类。 <br />（4）使用Java8日期API重写。 <br />（5）增加注释说明，优化代码。 <br />（6）修复原项目中的issue：标准时间yyyy-MM-dd、yyyy-MM-dd HH:mm:ss和yyyy-MM-dd HH:mm解析问题。 <br />（7）修复原项目中的issue：1小时后，1个半小时后，1小时50分钟等解析问题；并且支持到秒，比如50秒后，10分钟30秒后等。   <br />（8）修复原项目中的issue：修复当前时间是上午10点，那么下午三点 会识别为明天下午三点问题。 <br />（9）修复原项目中的issue：修复小数解析异常问题。<br />注意：NLP会有一定的识别失败率，在不断迭代开发提高成功率。
<a name="DXgP8"></a>
### 安装项目
<a name="ZBd4Z"></a>
#### （1）Maven
```xml
<dependency>  
  <groupId>com.github.xkzhangsan</groupId>    
  <artifactId>xk-time</artifactId>       
  <version>3.0.1</version>    
</dependency>    
```
<a name="g2aVy"></a>
#### （2）Gradle
```groovy
compile group: 'com.github.xkzhangsan', name: 'xk-time', version: '3.0.1'
```
注意：Android谨慎使用，Android端因为需要兼容低版本而不支持Java8，建议继续使用其他工具，如果有需要本项目相关的功能，可以参考源码实现！
<a name="sPZd8"></a>
### 项目地址
开源地址：[https://gitee.com/xkzhangsan/xk-time](https://gitee.com/xkzhangsan/xk-time)

