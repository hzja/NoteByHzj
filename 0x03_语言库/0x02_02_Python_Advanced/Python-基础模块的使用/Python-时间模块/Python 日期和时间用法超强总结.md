Python日期和时间<br />时间无疑是生活各个方面中最关键的因素之一，因此，记录和跟踪时间变得非常重要。在 Python 中，可以通过其内置库跟踪日期和时间。来介绍关于 Python 中的日期和时间，一起来了解如何使用time和datetime模块查找和修改日期和时间。
<a name="rMTTa"></a>
## Python 中处理日期和时间的模块
Python 提供了time和datetime模块，可以轻松获取和修改日期和时间，下面来逐一了解一下。
<a name="ISZ5V"></a>
### time 模块
该模块包括使用时间执行各种操作所需的所有与时间相关的功能，它还允许访问多种用途所需的时钟类型。<br />**内置函数:**<br />请看下表，它描述了时间模块的一些重要内置功能。

| function | Description |
| --- | --- |
| `time()` | 返回自epoch以来经过的秒数 |
| `ctime()` | 以经过的秒数作为参数，返回当前日期和时间 |
| `sleep()` | 在给定的持续时间内停止线程的执行 |
| `time.struct_time Class` | 函数要么将此类作为参数，要么将其作为输出返回 |
| `localtime()` | 以自epoch以来经过的秒数作为参数，并以时间形式返回日期和时间。struct_time格式 |
| `gmtime()` | 与localtime()类似，返回时间。UTC格式的struct_time |
| `mktime()` | ocaltime()的倒数。获取包含9个参数的元组，并返回自epoch pas输出以来经过的秒数 |
| `asctime()` | 获取包含9个参数的元组，并返回表示相同参数的字符串 |
| `strftime()` | 获取包含9个参数的元组，并根据使用的格式代码返回表示相同参数的字符串 |
| `strptime()` | 分析字符串并及时返回。struct_time格式 |

**代码格式化:**<br />在用示例解释每个函数之前，先看一下所有合法的格式化代码的方式：

| Code | Description | Example |
| --- | --- | --- |
| %a | Weekday (short version) | Mon |
| %A | Weekday (full version) | Monday |
| %b | Month (short version) | Aug |
| %B | Month (full version) | August |
| %c | Local date and time version | Tue Aug 23 1:31:40 2019 |
| %d | Depicts the day of the month (01-31) | 07 |
| %f | Microseconds | 000000-999999 |
| %H | Hour (00-23) | 15 |
| %I | Hour (00-11) | 3 |
| %j | Day of the year | 235 |
| %m | Month Number (01-12) | 07 |
| %M | Minutes (00-59) | 44 |
| %p | AM / PM | AM |
| %S | Seconds (00-59) | 23 |
| %U | Week number of the year starting from Sunday (00-53) | 12 |
| %w | Weekday number of the week | Monday (1) |
| %W | Week number of the year starting from Monday (00-53) | 34 |
| %x | Local date | 06/07/22 |
| %X | Local time | 12:30:45 |
| %y | Year (short version) | 22 |
| %Y | Year (full version) | 2022 |
| %z | UTC offset | +0100 |
| %Z | Timezone | CST |
| %% | % Character | % |

struct_time 类具有以下属性：

| Attribute | Value |
| --- | --- |
| tm_year | 0000, .., 2019, …, 9999 |
| tm_mon | 1-12 |
| tm_mday | 1-31 |
| tm_hour | 0-23 |
| tm_min | 0-59 |
| tm_sec | 0-61 |
| tm_wday | 0-6  (Monday is 0) |
| tm_yday | 1-366 |
| tm_isdst | 0, 1, -1    (daylight savings time, -1 when unknown) |

现在看几个 time 模块的例子
<a name="EhqGF"></a>
#### 使用time模块查找日期和时间
使用上表中描述的内置函数和格式化代码，可以在 Python 中轻松获取日期和时间。
```python
import time
#time
a=time.time()           #total seconds since epoch
print("Seconds since epoch :",a,end='n----------n')
#ctime
print("Current date and time:")
print(time.ctime(a),end='n----------n') 
#sleep
time.sleep(1)     #execution will be delayed by one second
#localtime
print("Local time :")
print(time.localtime(a),end='n----------n')
#gmtime
print("Local time in UTC format :")
print(time.gmtime(a),end='n-----------n')
#mktime
b=(2019,8,6,10,40,34,1,218,0)
print("Current Time in seconds :")
print( time.mktime(b),end='n----------n')
#asctime
print("Current Time in local format :")
print( time.asctime(b),end='n----------n')
#strftime
c = time.localtime() # get struct_time
d = time.strftime("%m/%d/%Y, %H:%M:%S", c)
print("String representing date and time:")
print(d,end='n----------n')
#strptime
print("time.strptime parses string and returns it in struct_time format :n")
e = "06 AUGUST, 2019"
f = time.strptime(e, "%d %B, %Y")
print(f)
```
Output:
```
Seconds since epoch : 1565070251.7134922
———-
Current date and time:
Tue Aug 6 11:14:11 2019
———-
Local time :
time.struct_time(tm_year=2019, tm_mon=8, tm_mday=6, tm_hour=11, tm_min=14, tm_sec=11, tm_wday=1, tm_yday=218, tm_isdst=0)
———-
Local time in UTC format :
time.struct_time(tm_year=2019, tm_mon=8, tm_mday=6, tm_hour=5, tm_min=44, tm_sec=11, tm_wday=1, tm_yday=218, tm_isdst=0)
———–
Current Time in seconds :
1565068234.0
———-
Current Time in local format :
Tue Aug 6 10:40:34 2019
———-
String representing date and time:
08/06/2019, 11:14:12
———-
time.strptime parses string and returns it in struct_time format :

time.struct_time(tm_year=2019, tm_mon=8, tm_mday=6, tm_hour=0, tm_min=0, tm_sec=0, tm_wday=1, tm_yday=218, tm_isdst=-1)
```
<a name="JvmrD"></a>
### datetime 模块
与time模块类似，datetime模块包含处理日期和时间所必需的所有方法。<br />**内置功能：**<br />下表介绍了本模块中的一些重要功能：

| function | Description |
| --- | --- |
| `datetime()` | datetime 的构造函数 |
| `datetime.today()` | 返回当前本地日期和时间 |
| `datetime.now()` | 返回当前本地日期和时间 |
| `date()` | 以年、月、日为参数，创建相应的日期 |
| `time()` | 以小时、分钟、秒、微秒和tzinfo作为参数，并创建相应的日期 |
| `date.fromtimestamp()` | 转换秒数以返回相应的日期和时间 |
| `timedelta()` | 它是不同日期或时间之间的差异（持续时间） |

<a name="nJkeT"></a>
#### 使用 datetime 查找日期和时间
现在尝试实现这些函数，以使用datetime模块在 Python 中查找日期和时间。
```python
import datetime
#datetime constructor
print("Datetime constructor:n")
print(datetime.datetime(2019,5,3,8,45,30,234),end='n----------n')

#today
print("The current date and time using today :n")
print(datetime.datetime.today(),end='n----------n')

#now
print("The current date and time using today :n")
print(datetime.datetime.now(),end='n----------n')

#date
print("Setting date :n")
print(datetime.date(2019,11,7),end='n----------n')

#time
print("Setting time :n")
print(datetime.time(6,30,23),end='n----------n')

#date.fromtimestamp
print("Converting seconds to date and time:n")
print(datetime.date.fromtimestamp(23456789),end='n----------n')

#timedelta
b1=datetime.timedelta(days=30, seconds=0, microseconds=0, milliseconds=0, minutes=0, hours=4, weeks=8)
b2=datetime.timedelta(days=3, seconds=0, microseconds=0, milliseconds=0, minutes=0, hours=4, weeks=8)
b3=b2-b1
print(type(b3))
print("The resultant duration = ",b3,end='n----------n')

#attributes
a=datetime.datetime.now()   #1
print(a)
print("The year is :",a.year)

print("Hours :",a.hour)
```
Output:
```
Datetime constructor:

2019-05-03 08:45:30.000234
———-
The current date and time using today :

2019-08-06 13:09:56.651691
———-
The current date and time using today :

2019-08-06 13:09:56.651691
———-
Setting date :

2019-11-07
———-
Setting time :

06:30:23
———-
Converting seconds to date and time:
1970-09-29
———-
<class ‘datetime.timedelta’>
The resultant duration = -27 days, 0:00:00
———-
2019-08-06 13:09:56.653694
The year is : 2019
Hours : 13
```
