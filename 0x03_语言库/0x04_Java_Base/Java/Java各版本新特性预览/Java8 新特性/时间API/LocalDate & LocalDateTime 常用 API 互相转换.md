Java LocalDate LocalDateTime<br />在平常时开发过程中，总有很多奇奇怪怪的时间需要互相转换，比如 Date to String , Date to DateTime、String to Date、String to DateTime、Date to TimeStamp等等。
<a name="v3f9d"></a>
### 1、LocalDateTime to String(将LocalDateTime 转为自定义的时间格式字符串
```java
public static String localDateTimeToString(LocalDateTime localDateTime, String format) {
    DateTimeFormatter formatter = DateTimeFormatter.ofPattern(format);
    return localDateTime.format(formatter);
}
```
<a name="jsZO4"></a>
### 2、timestamp to LocalDateTime (将long类型的timeStamp转为LocalDateTime )
```java
public static LocalDateTime timestampToLocalDateTime(long timestamp) {
    Instant instant = Instant.ofEpochMilli(timestamp);
    ZoneId zone = ZoneId.systemDefault();
    return LocalDateTime.ofInstant(instant, zone);
}
```
<a name="y5fHJ"></a>
### 3、LocalDateTime to timestamp ( 将LocalDateTime转为long类型的timeStamp )
```java
public static long localDateTimeToTimestamp(LocalDateTime localDateTime) {
    ZoneId zone = ZoneId.systemDefault();
    Instant instant = localDateTime.atZone(zone).toInstant();
    return instant.toEpochMilli();
}
```
<a name="D99AD"></a>
### 4、LocalDateTime to Date ( 将LocalDateTime to Date )
```java
public static Date localDateTimeToDate(LocalDateTime localDateTime) {
    ZoneId zone = ZoneId.systemDefault();
    Instant instant = localDateTime.atZone(zone).toInstant();
    return Date.from(instant);
}
```
<a name="g9zav"></a>
### 5、String to 自定义格式 ( Formatter ) 的LocalDateTime ( 将时间字符串转为自定义格式的LocalDateTime )
```java
public static LocalDateTime parseStringToDateTime(String time, String format) {
    DateTimeFormatter df = DateTimeFormatter.ofPattern(format);
    return LocalDateTime.parse(time, df);
}
```
<a name="jRO8n"></a>
### 6、Date to LocalDateTime ( 将Date转为LocalDateTime )
```java
public static LocalDateTime dateToLocalDateTime(Date date) {
    Instant instant = date.toInstant();
    ZoneId zone = ZoneId.systemDefault();
    return LocalDateTime.ofInstant(instant, zone);
}
```
<a name="a5aIP"></a>
### 7、Date to LocalDate（ 将Date 转为 LocalDate ）
```java
public static LocalDate dateToLocalDate(Date date) {
    Instant instant = date.toInstant();
    ZoneId zone = ZoneId.systemDefault();
    LocalDateTime localDateTime = LocalDateTime.ofInstant(instant, zone);
    return localDateTime.toLocalDate();
}
```
<a name="qucFw"></a>
### 8、LocalDate to Date
```java
public static Date localDateToDate(LocalDate localDate) {
    ZoneId zone = ZoneId.systemDefault();
    Instant instant = localDate.atStartOfDay().atZone(zone).toInstant();
    return Date.from(instant);
}
```
<a name="vWG8m"></a>
### 9、Date to LocalTime ( 将 Date 转为 LocalTime )
```java
public static LocalTime dateToLocalTime(Date date) {
    Instant instant = date.toInstant();
    ZoneId zone = ZoneId.systemDefault();
    LocalDateTime localDateTime = LocalDateTime.ofInstant(instant, zone);
    return localDateTime.toLocalTime();
}
```
<a name="bZINB"></a>
### 10、LocalTime to Date
```java
public static Date localTimeToDate(LocalTime localTime , LocalDate localDate) {
    LocalDateTime localDateTime = LocalDateTime.of(localDate, localTime);
    ZoneId zone = ZoneId.systemDefault();
    Instant instant = localDateTime.atZone(zone).toInstant();
    return Date.from(instant);
}
```
<a name="BK3lO"></a>
### 11、获取任意日期
```java
public static LocalDate anyDay(LocalDate localDate) {
    /**
        * 当月第一天-写法1
        */
    //第一天传入1，第二天2，第三天3，第四天4，第五天5,......
    LocalDate dayOfMonth = localDate.withDayOfMonth(2);
    /**
        * 当月第一天-写法2
        */
    LocalDate with = localDate.with(TemporalAdjusters.firstDayOfMonth());
    //当月最后一天
    LocalDate lastDate = localDate.with(TemporalAdjusters.lastDayOfMonth());

    //下一天
    LocalDate plusDays = dayOfMonth.plusDays(1);
    //前一天
    LocalDate minusDays = dayOfMonth.minusDays(1);

    // 取2014年1月第一个周一
    LocalDate firstMondayOf2014 = LocalDate.parse("2014-01-01").with(TemporalAdjusters.firstInMonth(DayOfWeek.MONDAY));

    return dayOfMonth;
}
```
