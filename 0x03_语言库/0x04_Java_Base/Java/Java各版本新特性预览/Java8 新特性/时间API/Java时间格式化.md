Java
<a name="CNH6a"></a>
## 新API的时间格式化
新的时间API的时间格式化由`java.time.format.DateTimeFormatter`负责。
<a name="V8FJE"></a>
### 本地化时间
结合枚举`FormatStyle`定义的风格，`DateTimeFormatter`预定义了基于本地（`Locale`）风格的时间格式。来看这段代码：
```java
String format = DateTimeFormatter.ofLocalizedDateTime(FormatStyle.MEDIUM)
    .format(ZonedDateTime.now());
```
如果在中国，格式化结果：
```java
2022年1月6日 下午4:22:01
```
如果在美国：
```java
Jan 6, 2022, 4:21:10 PM
```
有三个静态方法及其重载来格式化本地化时间，具体已经整理成了思维导图：<br />![本地化日期时间的处理](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641555138096-f46b17d6-52f9-4bcb-bc94-a74f0d0e3533.webp#clientId=u3074293f-4de2-4&from=paste&id=udde481d2&originHeight=587&originWidth=978&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u8cff4fd9-4d29-41c2-92a7-1bb923a2784&title=%E6%9C%AC%E5%9C%B0%E5%8C%96%E6%97%A5%E6%9C%9F%E6%97%B6%E9%97%B4%E7%9A%84%E5%A4%84%E7%90%86 "本地化日期时间的处理")
<a name="W4nQo"></a>
### ISO/RFC规范格式
`DateTimeFormatter`还内置了ISO和RFC的时间格式，基于内置的`DateTimeFormatter`静态实例。举个例子：
```java
// 静态实例
DateTimeFormatter isoWeekDateFormatter = DateTimeFormatter.ISO_WEEK_DATE;
// 执行格式化
String format = isoWeekDateFormatter.format(LocalDateTime.now());
// format = 2022-W01-4
System.out.println("format = " + format);
```
其它的如下表格所示：<br />![内置ISO、RFC规范格式](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641555138120-83df01bd-cfd7-4b21-af53-d58afdd61803.webp#clientId=u3074293f-4de2-4&from=paste&id=uaf29cba5&originHeight=626&originWidth=825&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uc021cc6f-d690-4fd7-b54e-dd4d96d96ae&title=%E5%86%85%E7%BD%AEISO%E3%80%81RFC%E8%A7%84%E8%8C%83%E6%A0%BC%E5%BC%8F "内置ISO、RFC规范格式")
<a name="IkDHs"></a>
### 范式格式化
这种方式应该是最常用的方式了。通过字母和符号来构建一个范式（Patterns），使用`ofPattern(String)`或者`ofPattern(String, Locale)`方法传递构建的范式。例如，d MMM uuuu将把2011-12-03格式化为2011年12月3日。从一个模式中创建的格式可以根据需要多次使用，它是不可改变的，并且是线程安全的。<br />看点没见过的：
```java
// 最后面是两个V 不是W 单个V会报错 
String pattern = "G uuuu'年'MMMd'日' ZZZZZ VV";
String format= DateTimeFormatter.ofPattern(pattern).format(ZonedDateTime.now());
// format = 2022-W01-4
System.out.println("format = " + format);
```
输出：
```java
format = 公元 2022年1月7日 +08:00 Asia/Shanghai
```
表格整理好了，自己试一试：<br />![格式化符号对照表](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641555138108-571baad3-454b-4e3d-b3b3-96f45a4ab117.webp#clientId=u3074293f-4de2-4&from=paste&id=u5f464903&originHeight=1629&originWidth=821&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u959107cc-0e16-4a6e-818b-5798d625834&title=%E6%A0%BC%E5%BC%8F%E5%8C%96%E7%AC%A6%E5%8F%B7%E5%AF%B9%E7%85%A7%E8%A1%A8 "格式化符号对照表")
