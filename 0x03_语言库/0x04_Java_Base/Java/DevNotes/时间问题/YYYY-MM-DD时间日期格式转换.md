日期格式化
<a name="0Jxzw"></a>
## yyyy和YYYY
执行下面的测试代码
```java
@Test
public void testWeekBasedYear() {
  Calendar calendar = Calendar.getInstance();
  // 2019-12-31
  calendar.set(2019, Calendar.DECEMBER, 31);
  Date strDate1 = calendar.getTime();
  // 2020-01-01
  calendar.set(2020, Calendar.JANUARY, 1);
  Date strDate2 = calendar.getTime();
  // 大写 YYYY
  SimpleDateFormat formatYYYY = new SimpleDateFormat("YYYY/MM/dd");
  System.out.println("2019-12-31 转 YYYY/MM/dd 格式: " + formatYYYY.format(strDate1));
  System.out.println("2020-01-01 转 YYYY/MM/dd 格式: " + formatYYYY.format(strDate2));
  // 小写 YYYY
  SimpleDateFormat formatyyyy = new SimpleDateFormat("yyyy/MM/dd");
  System.out.println("2019-12-31 转 yyyy/MM/dd 格式: " + formatyyyy.format(strDate1));
  System.out.println("2020-01-01 转 yyyy/MM/dd 格式: " + formatyyyy.format(strDate2));
}
```
输出结果：
```
2019-12-31 转 YYYY/MM/dd 格式: 2020/12/31
2020-01-01 转 YYYY/MM/dd 格式: 2020/01/01
2019-12-31 转 yyyy/MM/dd 格式: 2019/12/31
2020-01-01 转 yyyy/MM/dd 格式: 2020/01/01
```
2019-12-31用YYYY/MM/dd 此刻变成了2020/12/31
<a name="U3qiI"></a>
### YYYY是什么？
> Java's DateTimeFormatter pattern "YYYY" gives you the week-based-year, (by default, ISO-8601 standard) the year of the Thursday of that week.
> 译：Java 的 DateTimeFormatter 模式“ YYYY”为提供了基于周的年份(默认情况下为 ISO-8601标准) ，即该周的星期四。

**例子：**<br />下面就是用YYYY格式化代码

- 12/29/2019 将会格式化到2019年 这一周还属于2019年
- 12/30/2019 将会格式化到2020年 这一周已经属于2020年

看字说话YYYY，week-based year 是 ISO 8601 规定的。<br />2019-12-31号这一天，按周算年份已经属于2020年了，格式化之后就变成2020年，后面的月份日期不变。
<a name="4yS1e"></a>
## dd和DD
执行下面的测试代码
```java
private static void tryit(int Y, int M, int D, String pat) {
  DateTimeFormatter fmt = DateTimeFormatter.ofPattern(pat);
  LocalDate dat = LocalDate.of(Y,M,D);
  String str = fmt.format(dat);
  System.out.printf("Y=%04d M=%02d D=%02d " +
    "formatted with " +
    "\"%s\" -> %s\n",Y,M,D,pat,str);
}
public static void main(String[] args){
  tryit(2020,01,20,"MM/DD/YYYY");
  tryit(2020,01,21,"DD/MM/YYYY");
  tryit(2020,01,22,"YYYY-MM-DD");
  tryit(2020,03,17,"MM/DD/YYYY");
  tryit(2020,03,18,"DD/MM/YYYY");
  tryit(2020,03,19,"YYYY-MM-DD");
}
```
输出结果：
```
Y=2020 M=01 D=20 formatted with "MM/DD/YYYY" -> 01/20/2020
Y=2020 M=01 D=21 formatted with "DD/MM/YYYY" -> 21/01/2020
Y=2020 M=01 D=22 formatted with "YYYY-MM-DD" -> 2020-01-22
Y=2020 M=03 D=17 formatted with "MM/DD/YYYY" -> 03/77/2020
Y=2020 M=03 D=18 formatted with "DD/MM/YYYY" -> 78/03/2020
Y=2020 M=03 D=19 formatted with "YYYY-MM-DD" -> 2020-03-79
```
最后的3个日期都错误了，这里的大写的DD代表的是处于这一年中那一天，不是处于这个月的那一天。
