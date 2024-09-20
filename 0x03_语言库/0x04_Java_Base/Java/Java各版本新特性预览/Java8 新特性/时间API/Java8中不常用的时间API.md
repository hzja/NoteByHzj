Java
<a name="XpJki"></a>
## Instant
最近使用java.time.Instant比较多，因为计算**JWT**的过期时间和发行时间都用的是它。有人会问为什么不使用LocalDateTime呢？经过一番查询才明白了为什么要使用Instant。<br />时间是一条向前不断延伸的时间线。定义过期时间的时候肯定是一个瞬时时间点，而Instant正好可以用来表示时间线上的一个时间戳，常用来记录事件时间戳。<br />![时间戳示意图](https://cdn.nlark.com/yuque/0/2022/png/396745/1649291145177-4e2dab79-a42d-4618-b707-3f7a001e0edc.png#clientId=ua328580d-8dd2-4&from=paste&id=u11f50287&originHeight=264&originWidth=817&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uc446c134-c87b-4787-a6a5-ca66612d378&title=%E6%97%B6%E9%97%B4%E6%88%B3%E7%A4%BA%E6%84%8F%E5%9B%BE "时间戳示意图")<br />Instant的范围需要存储一个超过long类型极限的数字，因此它被设计为两个部分：

- seconds 从计算机元年（1970-01-01T00:00:00Z）开始的秒数，可能是负数哦。
- nanos 纳秒数，用来修正时间以保证准确性，始终是正数，而且小于999,999,999。
<a name="kSgRM"></a>
### 常用API的使用
如果使用`Instant.now()`获取当前时间戳，采用的是UTC时间，并非北京时间。所以需要加上时区：
```java
Instant issuedAt = Clock.system(ZoneId.of("Asia/Shanghai")).instant();
```
`TemporalAccessor`的实现类，比如常用的LocalDate、LocalDateTime都可以转换为Instant，例如：
```java
Instant current = Instant.from(LocalDateTime.now());

Instant instant = LocalDateTime.now().toInstant(ZoneOffset.ofHours(8));
```
如果在计算机元年以前，需要用负值表示：
```java
Instant instantBefore1970 = Instant.ofEpochSecond(-13434234, 343434);
```
还有其它一些API就不一一介绍了，请务必记住，它通常用来记录事件发生的瞬时时刻。
<a name="mc9I6"></a>
## Duration
这是一个持续时间，通常表示持续了多少时间。它记录了一个时间量，可以来自一个开始时间和结束时间，也可以来自一些时间计量单位。它同样用**秒数**和**纳秒**来存储时间量，秒数同样可以是负数，纳秒只能是正数。<br />![Java 8 Duration](https://cdn.nlark.com/yuque/0/2022/png/396745/1649291145297-a43621d0-17f3-4f90-8ecd-e4d697c7075e.png#clientId=ua328580d-8dd2-4&from=paste&id=ufd7b704c&originHeight=335&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u43cea2aa-4276-45d0-b7aa-518ad406d47&title=Java%208%20Duration "Java 8 Duration")
<a name="sNkMk"></a>
### 常用API的使用
比如花了3天时间写了个需求：
```java
Duration days = Duration.ofDays(3);
```
再精确点，从2022年4月2日到2022年4月5日，共3天：
```java
Duration duration = Duration.between(LocalDateTime.of(2022, 4, 2,0,0),
		LocalDateTime.of(2022, 4, 5,0,0));
```
注意这里只能使用能够表示到秒的`Temporal`实现，比如LocalDate只能表示到天，是不行的。<br />还有持续了多少年、多少月、多少分钟、多少小时、多少秒。
<a name="ZHo4H"></a>
### 格式化
格式基于 **ISO-8601** 持续时间格式`PnDTnHnMn.nS` ，其中天数被认为是 24 小时。字符串以可选符号开头，由 ASCII 负号或正号表示。如果为负，则整个周期都被否定。接下来是大写或小写的 **ASCII** 字母“P”。然后有四个部分，每个部分由一个数字和一个后缀组成。这些部分具有“D”、“H”、“M”和“S”的 ASCII 后缀，表示天、小时、分钟和秒，接受大写或小写。后缀必须按顺序出现。ASCII 字母“T”必须出现在小时、分钟或秒部分的第一次出现（如果有）之前。必须存在四个部分中的至少一个，如果存在“T”，则必须在“T”之后至少有一个部分。每个部分的数字部分必须由一个或多个 ASCII 数字组成。该数字可以以 ASCII 负号或正号为前缀。天数、小时数和分钟数必须解析为long 。秒数必须解析为带有可选分数的long整数。小数点可以是点或逗号。小数部分可能有 0 到 9 个数字。<br />实力

| 格式 | 含义 |
| --- | --- |
| PT20.345S | 20.345秒 |
| PT15M | 15分钟 |
| PT10H | 10小时 |
| P2D | 2天 |
| P2DT3H4M | 2天3小时4分钟 |
| P-6H3M | 减6小时加3分钟，等于减5小时57分钟 |
| -P6H3M | 否定全部，减6小时3分钟 |
| -P-6H+3M | 加6小时减3分钟，等于持续5小时57分钟 |

<a name="ySpr5"></a>
## Period
Period感觉和Duration非常类似，只不过它建立在年月日上，以年、月和日为单位对时间量或时间量进行建模。非常容易理解这里就不细说了，记住它最多精确到天就够了，它同样是用来记录持续时间的，只不过粒度粗了一些。
<a name="UDjPN"></a>
### 常用API
持续了2年：
```java
Period years = Period.ofYears(2);
// 其它时间单位就不演示了。
```
这里可以使用**周**进行初始化：
```java
Period weeks = Period.ofWeeks(3);
```
新冠从2019年12月持续到2022年4月
```java
Period covid19Period = Period.between(LocalDate.of(2019,12,1),
                LocalDate.of(2022,4,7));
```
<a name="PLkNG"></a>
### 格式化
基于 ISO-8601 句点格式PnYnMnD和PnW 。字符串以可选符号开头，由 ASCII 负号或正号表示。如果为负，则整个周期都被否定。接下来是大写或小写的 ASCII 字母“P”。然后有四个部分，每个部分由一个数字和一个后缀组成。必须存在四个部分中的至少一个。这些部分具有“Y”、“M”、“W”和“D”的 ASCII 后缀，表示年、月、周和日，接受大写或小写。后缀必须按顺序出现。每个部分的数字部分必须由 ASCII 数字组成。该数字可以以 ASCII 负号或正号为前缀。该数字必须解析为int 。前导加号/减号和其他单位的负值不是 ISO-8601 标准的一部分。此外，ISO-8601 不允许在PnYnMnD和PnW格式之间混合。任何基于周的输入乘以 7 并视为天数。<br />示例：

| 格式 | 含义 |
| --- | --- |
| P2Y | 两年 |
| P3M | 仨月 |
| P4W | 四周 |
| P5D | 五天 |
| P1Y2M3D | 一年俩月零3天 |
| P1Y2M3W4D | 一年俩月三周零四天，Period.of(1, 2, 25),3x7+4=25 |
| P-1Y2M | Period.of(-1, 2, 0) |
| -P1Y2M | Period.of(-1, -2, 0) |

这个用来做日历计算比较合适一些。
