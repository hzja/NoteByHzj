 时间 datetime<br />datetime模块提供了用于处理日期和时间的类和对应的方法，一般用于处理年、月、日、时、分、秒的统计和计算等需求。在支持日期时间数学运算的同时，实现的关注点更着重于如何能够更有效地解析其属性用于格式化输出和数据操作。<br />学习该模块，首先要有一个概念：日期和时间，要不然会混乱，这里进行狭义的理解即可：日期是日期，时间是时间，日常生活中有时候并没有很好的去区分。<br />日期：年月日，如 20210612<br />时间：时分秒，如 20:45:50<br />Python标准库中包含日期（date）和时间（time）的数据类型,还有日历方面的功能。要比较系统的学习该模块，需要了解下面的两个概念。<br />UTC（全球标准时间）:是全球范围内计时的科学标准，它基于精心维护的原子钟，在全球范围内精确到微秒，由于英文（CUT）和法文（TUC）的缩写不同，作为妥协，简称UTC。作为全球最精确的时间系统，天文学家、航海家、“太空跟踪网”(DSN) 以及其他科学性学科都使用它,它的参考点是英国格林威治标准时间（GMT）：地球本初子午线的午夜，也是 UTC 的午夜 (00:00:00.000000)。本地时间是根据地球上不同时区所处的位置调整 UTC 得来的，比如当美国加州的太平洋时间为正午 12:00:00 时，UTC 为 20:00:00。<br />夏令时（DST）:表示为了节约能源， 人为规定时间的意思。也叫夏时制，夏时令（Daylight Saving Time：DST），又称"日光节约时制"和"夏令时间”，在这一制度实行期间所采用的统一时间称为“夏令时间”。一般在天亮早的夏季人为将时间调快一小时，可以使人早起早睡，减少照明量，以充分利用光照资源，从而节约照明用电。各个采纳夏时制的国家具体规定不同。全世界有近110个国家每年要实行夏令时。<br />感知型”和“简单型”对象：日期和时间对象可以根据它们是否包含时区信息而分为“感知型”和“简单型”两类。充分掌握应用性算法和政治性时间调整信息例如时区和夏令时的情况下，一个 感知型 对象就能相对于其他感知型对象来精确定位自身时间点。感知型对象是用来表示一个没有解释空间的固定时间点。简单型对象没有包含足够多的信息来无歧义地相对于其他 date/time 对象来定位自身时间点。<br />时间戳：Unix时间戳是从1970年1月1日（UTC/GMT的午夜）开始所经过的秒数，不考虑闰秒。Unix时间戳（英文为Unix epoch, Unix time, POSIX time 或 Unix timestamp）
```python
import time time.time() 
1625483470.3409266
```
<a name="nbRGX"></a>
## 一、datetime模块概念
Python内置的时间模块`datetime`包含下面的模块包含六个类和两个常数，提供了用于处理日期和时间的类和对应的方法，一般用于处理年、月、日、时、分、秒的统计和计算等需求，且这些类型的对象都是不可变的。最常用的是`datetime.datetime`，有些人误以为就一个模块，其实是datetime模块下的datetime类。

| 常量、类或方法名 | 注解 |
| --- | --- |
| datetime.date | 一个理想化的简单型日期，它假设当今的公历在过去和未来永远有效。属性包含: year, month, day。 |
| datetime.time | 一个独立于任何特定日期的理想化时间，它假设每一天都恰好等于 24*60*60 秒。这里没有“闰秒”的概念） 包含属性：hour、minute、second、microsecond 、tzinfo。 |
| datetime.datetime | 是日期和时间的结合，属性包含year、month、day、hour、minute、second、 microsecond、 tzinfo.表示日期时间 |
| datetime.timedelta | 表示两个 date 对象或者 time 对象,或者 datetime 对象之间的时间间隔，精确到微秒。 |
| datetime.tzinfo | 一个描述时区信息对象的抽象基类。用来给datetime和time类提供自定义的时间调整概念（例如处理时区和/或夏令时）。 |
| datetime.timezone | 一个实现了tzinfo抽象基类的子类，用于表示相对于世界标准时间（UTC）的偏移量。 |
| datetime.MINYEAR | 常量，表示年份允许的最小值为1 |
| datetime.MAXYEAR | 常量，表示年份允许的最大值为9999 |

<a name="eVQR8"></a>
## 二、`datetime.date`类
<a name="bfOsc"></a>
### 1、date类介绍
作用：date类用来处理日期信息<br />语法：date类是datetime的内嵌类，实例化语法：datetime.date(year, month, day)<br />参数：year年份、month月份及day日期，所有参数都是必要的， 参数必须是在下面范围内的整数<br />MINYEAR <= year <= MAXYEAR<br />1 <= month <= 12<br />1 <= day<= 给定年月对应的天数<br />如果参数不在这些范围内，则抛出 ValueError 异常。
```python
import datetimet = datetime.date(2019,8,26)
print(type(t))
print(t.day,t.month,t.year) 
# <class 'datetime.date'>26 8 2019
```
通过内置函数dir，可以查看date类的所有方法和属性
```python
from datetime import date
print(dir(date))
['ctime', 'day', 'fromisocalendar', 'fromisoformat', 'fromordinal', 'fromtimestamp', 'isocalendar', 'isoformat', 'isoweekday', 'max', 'min', 'month', 'replace', 'resolution', 'strftime', 'timetuple', 'today', 'toordinal', 'weekday', 'year']
```
<a name="f88EQ"></a>
### 2、类方法
<a name="ZwLbU"></a>
#### `date.today()`
作用：返回当前的本地日期，等价于`date.fromtimestamp(time.time())`。
```python
from datetime import date 
date.today()
datetime.date(2021, 7, 4)
```
<a name="hO942"></a>
#### `date.fromtimestamp()`
作用：返回对应于POSIX时间戳的当地时间，例如 `time.time()` 返回的就是时间戳。这可能引发 OverflowError，如果时间戳数值超出所在平台 C `localtime()` 函数的支持范围的话，并且会在 `localtime()` 出错时引发 OSError。通常该数值会被限制在 1970 年至 2038 年之间。请注意在时间戳概念包含闰秒的非 POSIX 系统上，闰秒会被 `fromtimestamp()` 所忽略。<br />在 3.3 版更改: 引发 OverflowError 而不是 ValueError，如果时间戳数值超出所在平台 C `localtime()` 函数的支持范围的话，并会在 `localtime()` 出错时引发 OSError 而不是 ValueError。<br />用法：`date.fromtimestamp(timestamp)`
```python
from datetime import date
date.fromtimestamp(1339119900000/1e3).strftime('%Y-%m-%d %H:%M')
'2012-06-07 00:00'
date.fromtimestamp(1339120800000/1e3).strftime('%Y-%m-%d %H:%M')
'2012-06-07 00:00'
```
注意： unix时间戳是从1970年1月1日（UTC/GMT的午夜）开始所经过的秒数，不考虑闰秒。
<a name="WPuOd"></a>
#### `date.fromordinal()`
作用：返回对应于预期格列高利历序号的日期，其中公元 1 年 1 月 1 晶的序号为 1。<br />用法：`date.fromordinal(ordinal)`
```python
from datetime import date
d = date.fromordinal(730920) 
# 730920th day after 1. 1. 0001d
datetime.date(2002, 3, 11)
```
<a name="O1jsF"></a>
#### `date.fromisoformat()`
作用：返回一个对应于以 YYYY-MM-DD 格式给出的 date_string 的 date 对象<br />用法：`date.fromisoformat(date_string)`
```python
from datetime import date
date.fromisoformat('2019-12-04')
datetime.date(2019, 12, 4)
```
这是 `date.isoformat()` 的逆操作。它只支持 YYYY-MM-DD 格式。更通用的要用`strptime`
```python
d.isoformat()
'2002-03-11'
```
<a name="sSMR0"></a>
#### `date.fromisocalendar()`
作用：返回指定 year, week 和 day 所对应 ISO 历法日期的 date。这是函数 `date.isocalendar()` 的逆操作。<br />用法：`date.fromisocalendar(year, week, day)`<br />今年的42周第三天是10月20号
```python
date.fromisocalendar(2021, 42, 3)
datetime.date(2021, 10, 20)
```
<a name="pVtm5"></a>
### 3、类属性
<a name="tOxv9"></a>
#### `date.min`
最小的日期 `date(MINYEAR, 1, 1) `。
```python
from datetime import date
date.mindatetime.date(1, 1, 1)
```
<a name="N8ETj"></a>
#### `date.max`
最大的日期 ，date(MAXYEAR, 12, 31)。
```python
from datetime import date
date.maxdatetime.date(9999, 12, 31)
```
<a name="U7ekw"></a>
#### `date.resolution`
两个日期对象的最小间隔，`timedelta(days=1)`。
```python
from datetime import date
date.resolutiondatetime.timedelta(days=1)
```
<a name="c541Q"></a>
#### `date.year`
在 MINYEAR 和 MAXYEAR 之间，包含边界。
```python
from datetime import date 
d = date.today()
#生成一个date对象d
datetime.date(2021, 7, 5)
d.year
2021
```
<a name="PFPfo"></a>
#### `date.month`
1 至 12（含）
```python
#接上述案例
d.month
7
```
<a name="GtHzu"></a>
#### `date.day`
返回1到指定年月的天数间的数字。
```python
#接上述案例
d.day
5
```
<a name="HNRm5"></a>
### 4、实例方法
<a name="wUr2r"></a>
#### `date.replace()`
作用：返回一个具有同样值的日期，除非通过任何关键字参数给出了某些形参的新值。<br />用法：`date.replace(year=self.year, month=self.month, day=self.day)`
```python
from datetime import date
d = date(2002, 12, 31)
d.replace(day=26)
datetime.date(2002, 12, 26)
```
<a name="Rfj19"></a>
#### `date.timetuple()`
作用：返回一个 `time.struct_time`，即 `time.localtime()` 所返回的类型。hours, minutes 和 seconds 值均为 0，且 DST 旗标值为 -1。<br />用法：`date.timetuple(d)`
```python
d = date(2002, 12, 31)
date.timetuple(d)
time.struct_time(tm_year=2002, tm_mon=12, tm_mday=31, tm_hour=0, tm_min=0, tm_sec=0, tm_wday=1, tm_yday=365, tm_isdst=-1)
```
<a name="dNqkr"></a>
#### `date.toordinal()`
作用：返回日期的预期格列高利历序号，其中公元 1 年 1 月 1 日的序号为 1。对于任意 date 对象 
```python
d，date.fromordinal(d.toordinal()) == d。
d = date(1, 1, 1)
d.toordinal()
1
d = date(2002, 12, 31)
d.toordinal()
731215
date.fromordinal(d.toordinal())
datetime.date(2002, 12, 31)
```
<a name="UE4Cj"></a>
#### `date.weekday()`
作用：返回一个整数代表星期几，星期一为0，星期天为6。
```python
date(2002, 12, 4).weekday()
2
```
<a name="wgI1y"></a>
#### `date.isoweekday()`
作用：返回一个整数代表星期几，星期一为1，星期天为7。
```python
date(2002, 12, 4).isoweekday()
3
```
<a name="qIdoW"></a>
#### `date.isocalendar()`
作用：返回一个由三部分组成的 named tuple 对象: year, week 和 weekday。<br />ISO 历法是一种被广泛使用的格列高利历。ISO 年由 52 或 53 个完整星期构成，每个星期开始于星期一结束于星期日。一个 ISO 年的第一个星期就是（格列高利）历法的一年中第一个包含星期四的星期。这被称为 1 号星期，这个星期四所在的 ISO 年与其所在的格列高利年相同。<br />例如，2004 年的第一天是星期四，因此 ISO 2004 年的第一个星期开始于 2003 年 12 月 29 日星期一，结束于 2004 年 1 月 4 日星期日.
```python
from datetime import date
date(2003, 12, 29).isocalendar()
(2004, 1, 1) 
# 旧版本
datetime.IsoCalendarDate(year=2004, week=1, weekday=1) 
# 新版本
date(2004, 1, 4).isocalendar()
datetime.IsoCalendarDate(year=2004, week=1, weekday=7)
```
在 3.9 版更改: 结果由元组改为 named tuple。
<a name="vl8wp"></a>
#### `date.isoformat()`
作用：返回一个以 ISO 8601 格式 YYYY-MM-DD 来表示日期的字符串<br />用法：`date.isoformat(d)`
```python
from datetime import date
date(2021, 10, 1).isoformat()
'2021-10-01'
```
这是 `date.fromisoformat()` 的逆操作。
<a name="JGZeW"></a>
#### `date.__str__()`
作用：对于日期对象 d, `str(d)` 等价于 `d.isoformat()` 。<br />用法：`date.__str__(d)`
```python
from datetime import date
date(2021, 10, 1).__str__()
'2021-10-01'
str(date(2021, 10, 1))
'2021-10-01'
```
<a name="AVE1V"></a>
#### `date.ctime()`
作用：返回一个表示日期的字符串，在原生 C `ctime()` 函数 (`time.ctime()` 会发起调用该函数，但 `date.ctime()` 则不会) 遵循 C 标准的平台上。
```python
from datetime import date
date(2021, 10, 1).ctime()
'Fri Oct  1 00:00:00 2021'
```
`d.ctime()` 等效于 `time.ctime(time.mktime(d.timetuple()))`
```python
d=date(2021, 10, 1)import time
time.ctime(time.mktime(d.timetuple()))
'Fri Oct  1 00:00:00 2021'
```
<a name="X7bBu"></a>
#### `date.strftime()`
作用：翻译下就是 str from time，将给定格式的日期时间对象转换为字符串。日期时间对象=>字符串，控制日期时间对象的输出格式，date、datetime、time对象都支持`strftime(format)` 方法，可用来创建由一个显式格式字符串所控制的表示时间的字符串。要获取格式指令的完整列表，查看文末列表。<br />用法：`date.strftime(format)`<br />掌握了这个方法，随心所以的转换日期格式
```python
import datetimedt=datetime.date(2021, 10, 1)
#创建一个日期对象
dt.strftime("%Y-%m-%d")
'2021-10-01'
dt.strftime("%Y年%m月%d日")
'2021年10月01日'
dt.strftime("%Y//%m//%d")
'2021//10//01'
dt.strftime("%Y-%m-%d %H:%M")
'2021-10-01 00:00'
dt.strftime("%A, %d. %B %Y %I:%M%p")
'Friday, 01. October 2021 12:00AM'
```
<a name="HfepG"></a>
#### `date.__format__(format)`
与 `date.strftime()` 相同。此方法使得为 date 对象指定以 格式化字符串字面值 表示的格式化字符串以及使用 `str.format()` 进行格式化成为可能。要获取格式指令的完整列表，请看文末附表
```python
import datetimedt=datetime.date(2021, 10, 1)
#创建一个日期对象
dt.__format__("%Y年%m月%d日")
'2021年10月01日'
```
<a name="jhiyZ"></a>
### 4、支持的运算
| date2 = date1 + timedelta | date2 等于从 date1 减去 timedelta.days 天。(1) |
| --- | --- |
| date2 = date1 - timedelta | 计算 date2 的值使得 date2 + timedelta == date1。(2) |
| timedelta = date1 - date2 | (3) |
| date1 < date2 | 如果 date1 的时间在 date2 之前则认为 date1 小于 date2 。(4) |

<a name="vtVNl"></a>
## 三、`datetime.time`类
<a name="Lzm3G"></a>
### 1、`time`类概述
作用：一个 time 对象代表某日的（本地）时间，它独立于任何特定日期，并可通过 tzinfo 对象来调整。<br />语 法：
```python
datetime.time(hour=0, minute=0, second=0, microsecond=0, tzinfo=None, *, fold=0)
```
参 数：<br />所有参数都是可选的，tzinfo 可以是 None，或者是一个 tzinfo 子类的实例。其余的参数必须是在下面范围内的整数：

- 0 <= hour < 24
- 0 <= minute < 60
- 0 <= second < 60
- 0 <= microsecond < 1000000
- fold in [0, 1]

如果给出一个此范围以外的参数，则会引发 ValueError。所有参数值默认为 0，只有 tzinfo 默认为 None。 <br />通过内置函数dir，可以查看time类的所有方法和属性
```python
from datetime import timeprint(dir(time))
['dst', 'fold', 'fromisoformat', 'hour', 'isoformat', 'max', 'microsecond', 'min', 'minute', 'replace', 'resolution', 'second', 'strftime', 'tzinfo', 'tzname', 'utcoffset']
```
<a name="Frg11"></a>
### 2、`time`类属性
<a name="wazT9"></a>
#### `time.min`
早最的可表示 time, `time(0, 0, 0, 0)`。
```python
from datetime import time
time.mindatetime.time(0, 0)
```
<a name="BsGpO"></a>
#### `time.max`
最晚的可表示 time, `time(23, 59, 59, 999999)`。
```python
from datetime import time
time.maxdatetime.time(23, 59, 59, 999999)
```
<a name="cruUI"></a>
#### `time.resolution`
两个不相等的 time 对象之间可能的最小间隔，`timedelta(microseconds=1)`，但是请注意 time 对象并不支持算术运算。
```python
from datetime import time
time.resolutiondatetime.timedelta(microseconds=1)
```
<a name="cUnaY"></a>
### 3、time类实例属性
<a name="FxEKf"></a>
#### `time.hour`
取值范围是 range(24)。
```python
from datetime import time
t = time(hour=12, minute=34, second=56, microsecond=123456)
t.hour
12
```
<a name="KpXnD"></a>
#### `time.minute`
取值范围是 range(60)。
```python
t.minute
34
```
<a name="lDoFe"></a>
#### `time.second`
取值范围是 range(60)。
```python
t.second
56
```
<a name="S2Czk"></a>
#### `time.microsecond`
取值范围是 range(1000000)。
```python
t.microsecond
123456
```
<a name="QaoCl"></a>
#### `time.tzinfo`
作为 tzinfo 参数被传给 time 构造器的对象，如果没有传入值则为 None。
```python
t.tzinfo
```
<a name="WM4UV"></a>
#### `time.fold`
取值范围是 [0, 1]。用于在重复的时间段中消除边界时间歧义。（当夏令时结束时回拨时钟或由于政治原因导致当明时区的 UTC 时差减少就会出现重复的时间段。） 取值 0 (1) 表示两个时刻早于（晚于）所代表的同一边界时间。
```python
t.fold
0
```
<a name="untgT"></a>
### 4、time类实例方法
<a name="LXnEK"></a>
#### `time.replace()`
作用：返回一个具有同样属性值的 time，除非通过任何关键字参数指定了某些属性值。<br />语法：`time.replace(hour=self.hour, minute=self.minute, second=self.second, microsecond=self.microsecond, tzinfo=self.tzinfo, *, fold=0)`
```python

from datetime import time
t = time(hour=12, minute=34, second=56, microsecond=123456)
t.replace(hour=23, minute=12,)
datetime.time(23, 12, 56, 123456)
```
<a name="JQ5BP"></a>
#### `time.isoformat()`
作用：返回表示为 ISO 8601 格式之一的时间字符串<br />语法：`time.isoformat(timespec='auto')`
```python
from datetime import time
t = time(hour=12, minute=34, second=56, microsecond=123456)
t.isoformat()
'12:34:56.123456'
t.isoformat(timespec='minutes')
'12:34'
dt = time(hour=12, minute=34, second=56, microsecond=0)
dt.isoformat(timespec='microseconds')
'12:34:56.000000'
dt.isoformat(timespec='auto')
'12:34:56'
```
<a name="T5T7F"></a>
#### `time.fromisoformat()`
作用：返回对应于 `time.isoformat()` 所提供的某种 time_string 格式的 time。特别地，此函数支持以下格式的字符串：HH[:MM[:SS[.fff[fff]]]][+HH:MM[:SS[.ffffff]]]<br />语法：`time.fromisoformat(time_string)`
```python
time.fromisoformat('04:23:01')
datetime.time(4, 23, 1)

time.fromisoformat('04:23:01.000384')
datetime.time(4, 23, 1, 384)
time.fromisoformat('04:23:01+04:00')
datetime.time(4,23,1, tzinfo=datetime.timezone(datetime.timedelta(seconds=14400)))
```
<a name="fuoGt"></a>
#### `time.__str__()`
作用：对于时间对象 t, `str(t)` 等价于 `t.isoformat()`<br />语法：`time.__str__(t)`
```python
from datetime import time
t = time(hour=12, minute=34, second=56, microsecond=123456)
time.__str__(t)
'12:34:56.123456'
str(t)
'12:34:56.123456'
```
<a name="YJOKy"></a>
#### `time.strftime()`
作用：时间->字符串，返回一个由显式格式字符串所指明的代表时间的字符串<br />语法：`time.strftime(format)`
```python
from datetime import time
t = time(hour=12, minute=3, second=56)
t.strftime('%H:%M:%S')
#用:分隔
'12:03:56'
#用%分隔t.strftime('%H%%%M%%%S')
'12%03%56'
```
<a name="YZ8kI"></a>
#### `time.__format__()`
作用：与 `time.strftime()` 相同<br />语法：`time.__format__(format)`
```python
from datetime import time
t = time(hour=12, minute=3, second=56)
t.__format__('%H:%M:%S')
'12:03:56'
t.__format__('%H%%%M%%%S')
'12%03%56'
```
<a name="FyHz6"></a>
#### `time.utcoffset()`
作用：如果 tzinfo 为 None，则返回 None，否则返回 `self.tzinfo.utcoffset(None)`，并且在后者不返回 None 或一个幅度小于一天的 a timedelta 对象时将引发异常。
<a name="rCLLD"></a>
#### `time.dst()`
作用：如果 tzinfo 为 None，则返回 None，否则返回 `self.tzinfo.dst(None)`，并且在后者不返回 None 或者一个幅度小于一天的 timedelta 对象时将引发异常。
<a name="XpxxN"></a>
#### `time.tzname()`
作用：如果 tzinfo 为 None，则返回 None，否则返回 `self.tzinfo.tzname(None)`，如果后者不返回 None 或者一个字符串对象则将引发异常。
```python
from datetime import time, tzinfo, timedelta
class TZ1(tzinfo):
    def utcoffset(self, dt):
        return timedelta(hours=1)
    def dst(self, dt):
        return timedelta(0)
    def tzname(self,dt):
        return "+01:00"
            
    def  __repr__(self):
        return f"{self.__class__.__name__}()"
t = time(12, 10, 30, tzinfo=TZ1())
tdatetime.time(12, 10, 30, tzinfo=TZ1())
t.isoformat()
'12:10:30+01:00'
t.dst()
datetime.timedelta(0)
t.tzname()
'+01:00'
t.strftime("%H:%M:%S %Z")
'12:10:30 +01:00'
'The {} is {:%H:%M}.'.format("time", t)
'The time is 12:10.'
```
<a name="Xpm36"></a>
## 四、`datetime.datetime`类
<a name="p8dEo"></a>
### 1、datetime概述
作用：date类可以同时获得日期和时间信息，datetime 对象是包含来自 date 对象和 time 对象的所有信息的单一对象。与 date 对象一样，datetime 假定当前的格列高利历向前后两个方向无限延伸；与 time 对象一样，datetime 假定每一天恰好有 3600*24 秒。<br />语法：
```python
datetime.datetime(year, month, day, hour=0, minute=0, second=0, microsecond=0, tzinfo=None, *, fold=0)
```
参数：year, month 和 day 参数是必须的。tzinfo 可以是 None 或者是一个 tzinfo 子类的实例。其余的参数必须是在下面范围内的整数：

- MINYEAR <= year <= MAXYEAR,
- 1 <= month <= 12,
- 1 <= day <= 指定年月的天数,
- 0 <= hour < 24,
- 0 <= minute < 60,
- 0 <= second < 60,
- 0 <= microsecond < 1000000,
- fold in [0, 1].

如果参数不在这些范围内，则抛出 ValueError 异常。<br />注意：microsecond-微秒，百万分之一秒<br />datetime类是date和time的结合体，包括date与time的所有信息，date和time类中具有的方法和属性，datetime类都具有。可以使用内置函数dir查看该模块支持的方法
```python
print(dir(datetime.datetime))
[ 'astimezone', 'combine', 'ctime', 'date', 'day', 'dst', 'fold', 'fromisoformat', 'fromordinal', 'fromtimestamp', 'hour', 'isocalendar', 'isoformat', 'isoweekday', 'max', 'microsecond', 'min', 'minute', 'month', 'now', 'replace', 'resolution', 'second', 'strftime', 'strptime', 'time', 'timestamp', 'timetuple', 'timetz', 'today', 'toordinal', 'tzinfo', 'tzname', 'utcfromtimestamp', 'utcnow', 'utcoffset', 'utctimetuple', 'weekday', 'year']
```
<a name="nCTYE"></a>
### 2、`datetime`类方法
<a name="dL0qE"></a>
#### `datetime.today()`
作用：获取今天的时间，返回datetime对象，其中 tzinfo 为 None。此方法的功能等价于 `now()`，但是不带 tz 形参。<br />语法：`datetime.datetime.today(tz=None )`
```python
from datetime import datetime
datetime.today() 
datetime.datetime(2021, 7, 5, 17, 58, 21, 359135)
```
注：359135，microsecond-微秒，百万分之一秒
<a name="XPDMR"></a>
#### `datetime.now()`
作用：返回当前时间，返回datetime对象。<br />语法：`datetime.datetime.now(tz=None)`
```python
from datetime import datetime
#获取现在时间
datetime.now()
atetime.datetime(2021, 6, 27, 21, 59, 53, 950783)
#调整下格式
datetime.now().strftime('%Y-%m-%d %H:%M:%S')
'2021-06-27 22:01:49'
```
<a name="IANaM"></a>
#### `datetime.utcnow()`
作用：返回表示当前 UTC 时间的 date 和 time，其中 tzinfo 为 None。<br />语法：`datetime.utcnow()`
```python
from datetime import datetime
datetime.utcnow()
datetime.datetime(2021, 7, 6, 2, 46, 7, 229768)
datetime.now()
datetime.datetime(2021, 7, 6, 10, 46, 3, 67817)
```
可以看到，`now()`与`utcnow()`相差了差不多8小时
<a name="AoCXO"></a>
#### `datetime.fromtimestamp()`
作用： 将时间戳表示为本地时间的简单日期时间对象， 如果可选参数 tz 为 None 或未指定，时间戳会被转换为所在平台的本地日期和时间，返回的 datetime 对象将为天真型。<br />语法：`datetime.datetime.fromtimestamp(timestamp, tz=None)`
```python
import timeimport datetime
tt = time.time()
tt
1625542045.2471695
datetime.datetime.fromtimestamp(tt)
datetime.datetime(2021, 7, 6, 11, 27, 25, 247169)
#取整数后，精确到秒，否则精确到微秒，一微秒=百万分之一秒
datetime.datetime.fromtimestamp(1625542045)
datetime.datetime(2021, 7, 6, 11, 27, 25)
#例如，使用日期2008-12-27午夜UTC(自纪元起40*356*86400秒)
datetime.datetime.fromtimestamp(40 * 356 * 86400)
datetime.datetime(2008, 12, 27, 1, 0)
```
<a name="TNn7U"></a>
#### `datetime.utcfromtimestamp()`
作用：将时间戳表示为本地时间的UTC日期时间对象<br />语法：`datetime.datetime.utcfromtimestamp(timestamp)`
```python
import datetime
#获取本地当前时间
dt=datetime.datetime.now()  
#获取时间戳    
dt.timestamp()  
1625549035.048533
datetime.datetime.utcfromtimestamp(dt.timestamp())
datetime.datetime(2021, 7, 6, 5, 23, 55, 48533)
```
<a name="LdoT7"></a>
#### `datetime.fromordinal()`
作用：返回对应于预期格列高利历序号的 datetime，其中公元 1 年 1 月 1 日的序号为 1。除非 `1 <= ordinal <= datetime.max.toordinal()` 否则会引发 ValueError。结果的 hour, minute, second 和 microsecond 值均为 0，并且 tzinfo 值为 None。<br />语法：`datetime.fromordinal(ordinal)`
```python
datetime.datetime.fromordinal(730920)
datetime.datetime(2002, 3, 11, 0, 0)
```
<a name="t564l"></a>
#### `datetime.combine()`
作用：返回一个date对象和一个time对象合成的date对象, 如果提供了 tzinfo 参数，其值会被用来设置结果的 tzinfo 属性，否则将使用 time 参数的 tzinfo 属性。<br />语法：`datetime.combine(date, time, tzinfo=self.tzinfo)`
```python
d = date(2005, 7, 14)
t = time(12, 30)
datetime.combine(d, t)
datetime.datetime(2005, 7, 14, 12, 30)
```
对于任意 datetime 对象 d，`d = datetime.combine(d.date(), d.time(), d.tzinfo)`。如果 date 是一个 datetime 对象，它的时间部分和 tzinfo 属性会被忽略。
<a name="dfwXP"></a>
#### `datetime.fromisoformat()`
作用：返回一个对应于 `date.isoformat()` 和 `datetime.isoformat()` 所提供的某一种 date_string 的 datetime 对象。特别地，此函数支持以下格式的字符串：`YYYY-MM-DD[*HH[:MM[:SS[.fff[fff]]]][+HH:MM[:SS[.ffffff]]]]`，其中 * 可以匹配任意的单个字符。<br />语法：`datetime.fromisoformat(date_string)`
```python
from datetime import datetime
datetime.fromisoformat('2011-11-04')
datetime.datetime(2011, 11, 4, 0, 0)
datetime.fromisoformat('2011-11-04T00:05:23')
datetime.datetime(2011, 11, 4, 0, 5, 23)
datetime.fromisoformat('2011-11-04 00:05:23.283')
datetime.datetime(2011, 11, 4, 0, 5, 23, 283000)
datetime.fromisoformat('2011-11-04 00:05:23.283+00:00')
datetime.datetime(2011, 11, 4, 0, 5, 23, 283000, tzinfo=datetime.timezone.utc)
datetime.fromisoformat('2011-11-04T00:05:23+04:00')   
datetime.datetime(2011, 11, 4, 0, 5, 23,tzinfo=datetime.timezone(datetime.timedelta(seconds=14400)))
```
<a name="lz3Fz"></a>
#### `datetime.fromisocalendar()`
作用：返回以 year, week 和 day 值指明的 ISO 历法日期所对应的 datetime。该datetime 对象的非日期部分将使用其标准默认值来填充。这是函数 `datetime.isocalendar()` 的逆操作。<br />语法：`datetime.fromisocalendar(year, week, day)`
```python
from datetime import datetime
datetime.fromisocalendar(2021, 10, 1) 
datetime.datetime(2021, 3, 8, 0, 0)
```
<a name="c44Jt"></a>
#### `datetime.strptime()`
作用：按照特定时间格式将字符串转换（解析）为时间类型。返回一个由显式格式字符串所指明的代表时间的字符串。要获取格式指令的完整列表，请参阅 `strftime()` 和 `strptime()` 的行为。<br />语法：`datetime.strptime(date_string, format)`
```python
dt = datetime.strptime("21/11/06 16:30", "%d/%m/%y %H:%M")
dt
datetime.datetime(2006, 11, 21, 16, 30)
```
<a name="QR3Nd"></a>
### 3、类属性
<a name="WMWQx"></a>
#### `datetime.min`
最早的可表示 datetime，`datetime(MINYEAR, 1, 1, tzinfo=None)`。
```python
from datetime import datetime
datetime.mindatetime.datetime(1, 1, 1, 0, 0)
```
<a name="f39qN"></a>
#### `datetime.max`
最晚的可表示 datetime，`datetime(MAXYEAR, 12, 31, 23, 59, 59, 999999, tzinfo=None)`。
```python
from datetime import datetime
datetime.maxdatetime.datetime(9999, 12, 31, 23, 59, 59, 999999)
```
<a name="VQwni"></a>
#### `datetime.resolution`
两个不相等的 datetime 对象之间可能的最小间隔，`timedelta(microseconds=1)`。
```python
from datetime import datetime
datetime.resolutiondatetime.timedelta(microseconds=1)
```
<a name="kR9uQ"></a>
### 4、实例属性-只读
<a name="zVGqJ"></a>
#### `datetime.year`
在 MINYEAR 和 MAXYEAR 之间，包含边界。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).year
2021
```
<a name="M63gp"></a>
#### `datetime.month`
1 至 12（含）
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).month
5
```
<a name="XGIJv"></a>
#### `datetime.day`
返回1到指定年月的天数间的数字。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).day
20
```
<a name="dBvQH"></a>
#### `datetime.hour`
取值范围是 range(24)。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).hour
13
```
<a name="pO3qo"></a>
#### `datetime.minute`
取值范围是 range(60)。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).minute1
4
```
<a name="QTb1G"></a>
#### `datetime.second`
取值范围是 range(60)。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).second
0
```
<a name="ExjwV"></a>
#### `datetime.microsecond`
取值范围是 range(1000000)。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).microsecond
0
```
<a name="x8PbV"></a>
#### `datetime.tzinfo`
作为 tzinfo 参数被传给 datetime 构造器的对象，如果没有传入值则为 None。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).fold
```
<a name="NYAOK"></a>
#### `datetime.fold`
取值范围是 [0, 1]。用于在重复的时间段中消除边界时间歧义。（当夏令时结束时回拨时钟或由于政治原因导致当明时区的 UTC 时差减少就会出现重复的时间段。） 取值 0 (1) 表示两个时刻早于（晚于）所代表的同一边界时间。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).fold
0
```
<a name="Aipdm"></a>
### 5、datetime实例方法
<a name="TiLk4"></a>
#### `datetime.date()`
作用：返回具有同样 year, month 和 day 值的 date 对象。<br />用法：`datetime.date()`
```python
datetime(2021,5,20,13,14,0,0).date()
datetime.date(2021, 5, 20)
```
<a name="iaP8c"></a>
#### `datetime.time()`
作用：返回具有同样 hour, minute, second, microsecond 和 fold 值的 time 对象。tzinfo 值为 None。另请参见 timetz() 方法。<br />用法：`datetime.time()`
```python
datetime(2021,5,20,13,14,0,0).time()
datetime.time(13, 14)
```
<a name="TXyjE"></a>
#### `datetime.timetz()`
作用：返回具有同样 hour, minute, second, microsecond, fold 和 tzinfo 属性性的 time 对象。另请参见 `time()` 方法。<br />用法：`datetime.timetz()`
```python
datetime(2021,5,20,13,14,0,0).timetz()
datetime.time(13, 14)
```
<a name="q4j1S"></a>
#### `datetime.replace()`
作用：返回一个具有同样属性值的 datetime，除非通过任何关键字参数为某些属性指定了新值。请注意可以通过指定 `tzinfo=None` 来从一个感知型 datetime 创建一个简单型 datetime 而不必转换日期和时间数据。<br />用法：`datetime.replace(year=self.year, month=self.month, day=self.day, hour=self.hour, minute=self.minute, second=self.second, microsecond=self.microsecond, tzinfo=self.tzinfo, *, fold=0)`
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).replace(year=2035, month=12)
datetime.datetime(2035, 12, 20, 13, 14)
```
<a name="CDjro"></a>
#### `datetime.astimezone()`
作用：返回一个具有新的 tzinfo 属性 tz 的 datetime 对象，并会调整日期和时间数据使得结果对应的 UTC 时间与 self 相同，但为 tz 时区的本地时间。<br />用法：`datetime.astimezone(tz=None)`
```python
def astimezone(self, tz):
    if self.tzinfo is tz:
        return self
    # Convert self to UTC, and attach the new time zone object.    
    utc = (self - self.utcoffset()).replace(tzinfo=tz)
    # Convert from UTC to tz's local time.
    return tz.fromutc(utc)
```
<a name="jY5nH"></a>
#### `datetime.timetuple()`
返回一个 `time.struct_time`，即 t`ime.localtime()` 所返回的类型。
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).timetuple()
time.struct_time(tm_year=2021, tm_mon=5, tm_mday=20, tm_hour=13, tm_min=14, tm_sec=0, tm_wday=3, tm_yday=140, tm_isdst=-1)
```
<a name="l7wsc"></a>
#### `datetime.utctimetuple()`
作用：如果 datetime 实例 d 为简单型，这类似于 `d.timetuple()`，不同之处在于 tm_isdst 会强制设为 0，无论 `d.dst()` 返回什么结果。DST 对于 UTC 时间永远无效。返回UTC时间元组<br />语法：datetime对象.utctimetuple()
```python
from datetime import datetime
datetime(2021,5,20,13,14,0,0).utctimetuple()
time.struct_time(tm_year=2021, tm_mon=5, tm_mday=20, tm_hour=13, tm_min=14, tm_sec=0, tm_wday=3, tm_yday=140, tm_isdst=0)
```
<a name="TfgZ9"></a>
#### `datetime.toordinal()`
作用：返回日期的预期格列高利历序号。与 `self.date().toordinal()` 相同。<br />用法：`datetime.toordinal()`
```python
datetime(2021, 5, 4).toordinal()
737914
```
<a name="eXK9i"></a>
#### `datetime.timestamp()`
作用：返回对应于 datetime 实例的 POSIX 时间戳。此返回值是与 `time.time()` 返回值类似的 float 对象。<br />用法：`datetime.timestamp()`
```python
from datetime import datetime
datetime(2021, 5, 4).timestamp()
1620057600.0
```
<a name="ij5My"></a>
#### `datetime.weekday()`
作用：返回一个整数代表星期几，星期一为 0，星期天为 6。相当于 `self.date().weekday()`。另请参阅 `isoweekday()`。<br />用法：`datetime.weekday()`
```python
import datetime
datetime.datetime(2021, 5, 4).weekday()
1
```
<a name="QKCUc"></a>
#### `datetime.isoweekday()`
作用：返回一个整数代表星期几，星期一为 1，星期天为 7。相当于 `self.date().isoweekday()`。另请参阅 `weekday()`, `isocalendar()`。<br />用法：`datetime.isoweekday()`
```python
import datetime
datetime.datetime(2021, 5, 4).isoweekday()
2
```
<a name="iV9sq"></a>
#### `datetime.isocalendar()`
作用：返回一个由三部分组成的元组: year, week 和 weekday，哪一年、第几周和、周几，等同于 `self.date().isocalendar()`。<br />用法：`datetime.isocalendar()`
```python
import datetimedt = datetime.datetime.now()
print(dt)
2021-07-06 14:22:35.920877
dt.isocalendar()
(2021, 27, 2)
```
<a name="pM79o"></a>
#### `datetime.isoformat()`
作用：返回一个以 ISO 8601 格式表示的日期和时间字符串<br />用法：`datetime.isoformat(sep='T', timespec='auto')`
```python
from datetime import datetime, timezone
datetime(2019, 5, 18, 15, 17, 8, 132263).isoformat()
'2019-05-18T15:17:08.132263'
datetime(2019, 5, 18, 15, 17, tzinfo=timezone.utc).isoformat()
'2019-05-18T15:17:00+00:00'
```
<a name="GXtmO"></a>
#### `datetime.__str__()`
作用：对于 datetime 实例 d，`str(d)` 等价于 `d.isoformat(' ')`。<br />语法：`datetime.__str__(d)`
```python
from datetime import datetime
# __str__
datetime.__str__(datetime(2019, 5, 18, 15, 17, 8, 132263))
'2019-05-18 15:17:08.132263'
# str
str(datetime(2019, 5, 18, 15, 17, 8, 132263))
'2019-05-18 15:17:08.132263'
```
<a name="W27M0"></a>
#### `datetime.ctime()`
作用：返回一个表示日期和时间的字符串，输出字符串将 并不 包括时区信息，无论输入的是感知型还是简单型。<br />用法：datetime.ctime()
```python
from datetime import datetime
datetime(2002, 12, 4, 20, 30, 40).ctime()
'Wed Dec  4 20:30:40 2002'
#d.ctime() 等效于:time.ctime(time.mktime(d.timetuple()))
```
在原生 C `ctime()` 函数 (`time.ctime()` 会发起调用该函数，但 `datetime.ctime()` 则不会) 遵循 C 标准的平台上。
<a name="wZyon"></a>
### `datetime.strftime()`
作用：将给定格式的日期时间对象转换为字符串。日期时间对象=>字符串，控制日期时间对象的输出格式，date、datetime、time对象都支持`strftime(format)` 方法，可用来创建由一个显式格式字符串所控制的表示时间的字符串。要获取格式指令的完整列表，查看文末列表。<br />用法：`datetime.strftime(format)`
```python
import datetime
dt=datetime.datetime(2006, 11, 21, 16, 30)
dt.strftime("%Y-%m-%d %H:%M")
'2006-11-21 16:30'
dt.strftime("%Y-%m-%d")
'2006-11-21'
dt.strftime("%A, %d. %B %Y %I:%M%p")
'Tuesday, 21. November 2006 04:30PM'
```
对比：<br />strftime：将给定格式的日期时间对象转换为字符串。日期时间对象=>字符串，控制输出格式<br />strptime：将字符串解析为给定格式的日期时间对象。字符串=>日期时间对象，解析字符串

| 

 | strftime | strptime |
| --- | --- | --- |
| 用法 | 根据给定的格式将对日期时间象转换为字符串 | 将字符串解析为给定相应格式的datetime 对象 |
| 类型 | 实例方法 | 类方法 |
| 方法 | date; datetime; time | datetime |
| 用法 | `strftime(format)` | `strptime(date_string, format)` |
| 示例 | datetime.datetime(2006, 11, 21, 16, 30) =>  '2006-11-21 16:30' | "21/11/06 16:30"  => datetime.datetime(2006, 11, 21, 16, 30) |

<a name="jFgKt"></a>
#### `datetime.__format__()`
作用：与 `datetime.strftime()` 相同。<br />语法：`datetime.__format__(format)`
```python
import datetime
dt=datetime.datetime(2006, 11, 21, 16, 30)
dt.__format__("%Y-%m-%d %H:%M")'2006-11-21 16:30'
```
<a name="ofMTG"></a>
## 五、`datetime.timedelta`
<a name="NAU1e"></a>
### 1、模块概述
作用：timedelta 对象表示两个 date 或者 time 的时间间隔。<br />语法：`datetime.timedelta(days=0, seconds=0, microseconds=0, milliseconds=0, minutes=0, hours=0, weeks=0)`<br />参数：只有 days, seconds 和 microseconds 会存储在内部。参数单位的换算规则如下：

- 1毫秒会转换成1000微秒。
- 1分钟会转换成60秒。
- 1小时会转换成3600秒。
- 1星期会转换成7天。

并且 days, seconds, microseconds 会经标准化处理以保证表达方式的唯一性，即：

- 0 <= microseconds < 1000000
- 0 <= seconds < 3600*24 (一天的秒数)
- -999999999 <= days <= 999999999 

下面的例子演示了如何对 days, seconds 和 microseconds 以外的任意参数执行“合并”操作并标准化为以上三个结果属性
```python
from datetime import timedelta
delta = timedelta(days=50,seconds=27,microseconds=10,milliseconds=29000,minutes=5,hours=8,weeks=2)
#只有 days, seconds 和 microseconds 会存储在内部，其他会被转换
deltadatetime.timedelta(days=64, seconds=29156, microseconds=10)
```
通过dir查看该类的方法和属性
```python
import datetime
dir(datetime.timedelta)
['days','max','microseconds', 'min', 'resolution', 'seconds', 'total_seconds']
```
<a name="YwuW6"></a>
### 2、类属性
<a name="UpRRQ"></a>
#### `timedelta.min`
The most negative timedelta object, timedelta(-999999999).
```python
from datetime import timedelta
timedelta.mindatetime.timedelta(days=-999999999)
```
<a name="VMf5f"></a>
#### `timedelta.max`
```python
from datetime import timedelta
timedelta.maxdatetime.timedelta(days=999999999, seconds=86399, microseconds=999999)
```
<a name="lnPUh"></a>
#### `timedelta.resolution`
两个不相等的 timedelta 类对象最小的间隔为 `timedelta(microseconds=1)`。<br />需要注意的是，因为标准化的缘故，timedelta.max > -timedelta.min，-timedelta.max 不可以表示一个 timedelta 类对象。
```python
from datetime import timedelta
timedelta.resolutiondatetime.timedelta(microseconds=1)
```
<a name="zarct"></a>
### 3、实例属性（只读）
<a name="L8EZp"></a>
#### `timedelta.days`
-999999999 至 999999999 ，含999999999
```python
from datetime import time
deltadelta = timedelta(days=50,seconds=27,microseconds=10,milliseconds=29000,minutes=5,hours=8,weeks=2)
deltadatetime.timedelta(days=64, seconds=29156, microseconds=10)
delta.days64
```
<a name="Iu0jv"></a>
#### `timedelta.seconds`
0 至 86399，包含86399
```python
delta.seconds
29156
```
<a name="lvS9L"></a>
#### `timedelta.microseconds`
0 至 999999，包含999999
```python
delta.microseconds
10
```
<a name="ai2ln"></a>
### 4、实例方法
<a name="vbOHN"></a>
#### `timedelta.total_seconds()`
作用：返回时间间隔包含了多少秒。等价于 `td / timedelta(seconds=1)`。对于其它单位可以直接使用除法的形式 (例如 `td / timedelta(microseconds=1))`。<br />需要注意的是，时间间隔较大时，这个方法的结果中的微秒将会失真（大多数平台上大于270年视为一个较大的时间间隔）。
```python
from datetime import time
deltayear = timedelta(days=1)
timedelta.total_seconds(year)
86400.0
```
<a name="Orp9v"></a>
### 5、支持的运算
| 运算 | 结果 |
| --- | --- |
| t1 = t2 + t3 | t2 和 t3 的和。运算后 t1-t2 == t3 and t1-t3 == t2 必为真值。(1) |
| t1 = t2 - t3 | t2 减 t3 的差。运算后 t1 == t2 - t3 and t2 == t1 + t3 必为真值。(1)(6) |
| t1 = t2 * i or t1 = i * t2 | 乘以一个整数。运算后假如 i != 0 则 t1 // i == t2 必为真值。 |
| 

 | In general, t1 * i == t1 * (i-1) + t1 is true. (1) |
| t1 = t2 * f or t1 = f * t2 | 乘以一个浮点数，结果会被舍入到 timedelta 最接近的整数倍。精度使用四舍五偶入奇不入规则。 |
| f = t2 / t3 | 总时间 t2 除以间隔单位 t3 (3)。返回一个 float 对象。 |
| t1 = t2 / f or t1 = t2 / i | 除以一个浮点数或整数。结果会被舍入到 timedelta 最接近的整数倍。精度使用四舍五偶入奇不入规则。 |
| t1 = t2 // i or t1 = t2 // t3 | 计算底数，其余部分（如果有）将被丢弃。在第二种情况下，将返回整数。（3） |
| t1 = t2 % t3 | 余数为一个 timedelta 对象。(3) |
| q, r = divmod(t1, t2) | 通过 : q = t1 // t2 (3) and r = t1 % t2 计算出商和余数。q是一个整数，r是一个 timedelta 对象。 |
| +t1 | 返回一个相同数值的 timedelta 对象。 |
| -t1 | 等价于 timedelta(-t1.days, -t1.seconds, -t1.microseconds), 和 t1* -1. (1)(4) |
| abs(t) | 当 t.days >= 0``时等于 +\\ *t*, 当 ``t.days < 0 时 -t 。(2) |
| str(t) | 返回一个形如 [D day[s], ][H]H:MM:SS[.UUUUUU] 的字符串，当 t 为负数的时候， D 也为负数。(5) |
| repr(t) | 返回一个 timedelta 对象的字符串表示形式，作为附带正规属性值的构造器调用。 |

注:<br />1）结果正确，但可能会溢出。<br />2）结果正确，不会溢出。<br />3）除以0将会抛出异常 `ZeroDivisionError`。<br />4）`-timedelta.max` 不是一个 timedelta 类对象。<br />5）timedelta 对象的字符串表示形式类似于其内部表示形式被规范化。对于负时间增量，这会导致一些不寻常的结果。例如:
```python
timedelta(hours=-5)
datetime.timedelta(days=-1, seconds=68400)
print(_)-1 
day, 19:00:00
```
6）表达式 t2 - t3 通常与 t2 + (-t3) 是等价的，除非 t3 等于 timedelta.max; 在这种情况下前者会返回结果，而后者则会溢出。
```python
from datetime import timedelta
year = timedelta(days=365)
ten_years = 10 * year
ten_years
datetime.timedelta(days=3650)ten_years.days // 36510
nine_years = ten_years - year
nine_years
datetime.timedelta(days=3285)
three_years = nine_years // 3
three_years, three_years.days // 365(datetime.timedelta(days=1095), 3)
```
<a name="xATFC"></a>
## 六、`datetime.tzinfo`（时区）
```python
class datetime.tzinfo
```
这是一个抽象基类，也就是说该类不应被直接实例化。请定义 tzinfo 的子类来捕获有关特定时区的信息。
<a name="BbSfG"></a>
## 七、`datetime.timezone`
`timezone` 类是 tzinfo 的子类，它的每个实例都代表一个以与 UTC 的固定时差来定义的时区。
<a name="RjX3A"></a>
## 八、常量模块
<a name="fgRoD"></a>
### `datetime.MINYEAR`
date 或者 datetime 对象允许的最小年份。常量 MINYEAR 是 1 。
```python
import datetime datetime.MINYEAR1
```
<a name="W09wC"></a>
### `datetime.MAXYEAR`
date 或 datetime 对象允许最大的年份。常量 MAXYEAR 是 9999 。
```python
import datetime datetime.MAXYEAR9999
```
