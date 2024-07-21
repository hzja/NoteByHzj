PythonDatetime<br />在 Python 中有各种数据类型，例如整数、浮点数、字符串等。同时在开发脚本或各种算法当中，应该经常会使用日期和时间。在日常生活中，可以用多种不同的格式来表示日期和时间，例如，7 月 4 日、2022 年 3 月 8 日、22:00 或 2022 年 12 月 31 日 23:59:59。它们使用整数和字符串的组合，或者也可以使用浮点数来表示一天、一分钟等等，各种各样的时间表示方式，确实让人眼花缭乱。<br />不过还好，Python 有 datetime 模块，可以通过它轻松地操作表示日期和时间的对象。<br />学习以下内容：

- **Python 中 datetime 模块的使用**
- **使用 Python 日期时间函数将字符串转换为日期时间对象，反之亦然**
- **从日期时间对象中提取日期和时间**
- **使用时间戳**
- **对日期和时间执行算术运算**
- **使用时区**
- **创建一个倒数计时器来确定距离 2023 年新年还有多长时间**

Let's do it!
<a name="ecDY2"></a>
## **如何在 Python 中使用日期时间**
正如之前所看到的，在编程中表示日期和时间是一项非常有挑战的事情。首先，必须以标准的、普遍接受的格式来表示它们。幸运的是，国际标准化组织 (ISO) 制定了一个全球标准 ISO 8601，它将与日期和时间相关的对象表示为 YYYY-MM-DD HH:MM:SS，其信息范围从最重要的（年，YYYY）到 最不重要的（秒，SS）。这种格式的每一部分都表示为一个四位数或两位数。<br />Python 中的 datetime 模块有 5 个主要类（模块的一部分）：

- **date 操作日期对象**
- **time 操作时间对象**
- **datetime 是日期和时间的组合**
- **timedelta 允许使用时间区间**
- **tzinfo 允许使用时区**

此外，将使用 zoneinfo 模块，它提供了一种处理时区的更加现代的方式，以及 dateutil 包，它包含许多有用的函数来处理日期和时间。<br />导入 datetime 模块并创建第一个日期和时间对象：
```python
# From the datetime module import date
from datetime import date

# Create a date object of 2000-02-03
date(2022, 2, 3)
```
Output:
```python
datetime.date(2022, 2, 3)
```
在上面的代码中，从模块中导入了日期类，然后创建了 2022 年 2 月 3 日的 `datetime.date` 对象。需要注意的是，用于创建该对象的数字顺序与 ISO 8061 中的完全相同 （但省略了 0 并且只写了一个数字的月份和日期）。<br />关于时间的编码都是基于现实的，所以假设要创建一个 2000-26-03 的对象：
```python
# Create a date object of 2000-26-03
date(2000, 26, 3)
```
Output:
```python
---------------------------------------------------------------------------

ValueError                                Traceback (most recent call last)

Input In [2], in 
      1 # Create a date object of 2000-26-03
----> 2 date(2000, 26, 3)

ValueError: month must be in 1..12
```
得到 ValueError: month must be in 1..12，毫无疑问，日历中没有第 26 个月，抛出异常。<br />看看如何创建一个 `datetime.time` 对象：
```python
# From the datetime module import time
from datetime import time

# Create a time object of 05:35:02
time(5, 35, 2)
```
Output:
```python
datetime.time(5, 35, 2)
```
现在，如果想要在一个对象中同时包含日期和时间怎么办？应该使用 datetime 类：
```python
# From the datetime module import datetime
from datetime import datetime

# Create a datetime object of 2000-02-03 05:35:02
datetime(2000, 2, 3, 5, 35, 2)
```
Output:
```python
datetime.datetime(2000, 2, 3, 5, 35, 2)
```
不出意外，成功创建了 datetime 对象。还可以更明确地将关键字参数传递给 datetime 构造函数：
```python
datetime(year=2000, month=2, day=3, hour=5, minute=35, second=2)
```
Output:
```python
datetime.datetime(2000, 2, 3, 5, 35, 2)
```
如果只传入三个参数（年、月和日）会怎样，是否会报错呢
```python
# Create a datetime object of 2000-02-03
datetime(2000, 2, 3)
```
Output:
```python
datetime.datetime(2000, 2, 3, 0, 0)
```
可以看到，现在对象中有两个零（分别代表）小时和分钟。同时秒数也被省略了。<br />在许多情况下，想知道当前的确切时间。可以使用 `datetime` 类的 `now()` 方法：
```python
# Time at the moment
now = datetime.now()
now
```
Output:
```python
datetime.datetime(2022, 8, 1, 0, 9, 39, 611254)
```
得到一个日期时间对象，这里最后一个数字是微秒。<br />如果只需要今天的日期，可以使用 `date` 类的 `today()` 方法：
```python
today = date.today()
today
```
Output:
```python
datetime.date(2022, 8, 1)
```
如果只需要时间，就必须访问 `datetime.now()` 对象的小时、分钟和秒属性，并将它们传递给时间构造函数：
```python
time(now.hour, now.minute, now.second)
```
Output:
```python
datetime.time(11, 33, 25)
```
还可以使用 `isocalendar()` 函数从日期时间对象中提取周数和天数。它将返回一个包含 ISO 年份、周数
```python
# isocalendar() returns a 3-item tuple with ISO year, week number, and weekday number
now.isocalendar()
```
和工作日数的三项元组：<br />Output:
```python
datetime.IsoCalendarDate(year=2022, week=7, weekday=1)
```
在 ISO 格式中，一周从星期一开始，到星期日结束。一周中的天数由从 1（星期一）到 7（星期日）的数字编码。如果想访问这些元组元素之一，需要使用括号表示法：
```python
# Access week number
week_number = now.isocalendar()[1]
week_number
```
Output:
```python
7
```
<a name="i3bT3"></a>
## **从字符串中提取日期**
在数据科学和一般编程中，主要使用以数十种不同格式存储为字符串的日期和时间，具体取决于地区、公司或需要的信息粒度。有时，需要日期和确切时间，但在其他情况下，只需要年份和月份。该如何从字符串中提取需要的数据，以便将其作为日期时间（日期、时间）对象来操作呢？
<a name="okYT0"></a>
### `**fromisoformat()**`** 和 **`**isoformat()**`
学习的第一个将日期字符串转换为日期对象的函数是 `fromisoformat`，这样称呼它是因为它使用 ISO 8601 格式（即 YYYY-MM-DD），看一个例子：
```python
# Convert a date string into a date object
date.fromisoformat("2022-12-31")
```
Output:
```python
datetime.date(2022, 12, 31)
```
ISO 格式也包含时间，但如果不能将它传递给函数：
```python
date.fromisoformat("2022-12-31 00:00:00")
```
Output:
```python
---------------------------------------------------------------------------

ValueError                                Traceback (most recent call last)

Input In [13], in 
----> 1 date.fromisoformat("2022-12-31 00:00:00")

ValueError: Invalid isoformat string: '2022-12-31 00:00:00'
```
当然，也可以进行逆向运算，将 datetime 对象转换为 ISO 格式的日期字符串，应该使用 `isoformat()`：
```python
# Convert a datetime object into a string in the ISO format
date(2022, 12, 31).isoformat()
```
Output:
```python
'2022-12-31'
```
<a name="DfsLc"></a>
### `**strptime()**`
为了解决上述 ValueError 问题，可以使用 `strptime()` 函数，该函数可以将任意日期/时间字符串转换为日期时间对象。字符串不一定需要遵循 ISO 格式，但应该指定字符串的哪一部分代表哪个日期或时间单位（年、小时等）。看一个例子，首先，将使用严格的 ISO 格式将字符串转换为日期时间对象：
```python
# Date as a string
iso_date = "2022-12-31 23:59:58"

# ISO format
iso_format = "%Y-%m-%d %H:%M:%S"

# Convert the string into a datetime object
datetime.strptime(iso_date, iso_format)
```
Output:
```python
datetime.datetime(2022, 12, 31, 23, 59, 58)
```
在第一行，创建一个日期/时间字符串。在第二行中，使用特殊代码指定字符串的格式，该代码包含一个百分号，后跟一个编码日期或时间单位的字符。最后，在第三行中，使用 `strptime()` 函数将字符串转换为日期时间对象。这个函数有两个参数：字符串和字符串的格式。<br />上面使用的代码还可以编码其他日期和时间单位，如工作日、月份名称、周数等。

| **代码** | **示例** | **说明** |
| --- | --- | --- |
| %A | Monday | 完整的工作日名称 |
| %B | December | 全月名称 |
| %W | 2 | 周数（星期一为一周的第一天） |

再看几个使用其他格式的示例：
```python
# European date as a string
european_date = "31-12-2022"

# European format
european_format = "%d-%m-%Y"

# Convert the string into a datetime object
datetime.strptime(european_date, european_format)
```
Output:
```python
datetime.datetime(2022, 12, 31, 0, 0)
```
如上所示，字符串已成功转换，但还有额外的零表示时间字段，看一个使用其他代码的示例：
```python
# Full month name date
full_month_date = "12 September 2022"

# Full month format
full_month_format = "%d %B %Y"

# Convert the string into a datetime object
datetime.strptime(full_month_date, full_month_format)
```
Output:
```python
datetime.datetime(2022, 9, 12, 0, 0)
```
还是可以正常转换，但是需要注意的是，定义的格式应该与日期字符串的格式相匹配。因此，如果有空格、冒号、连字符或其他字符来分隔时间单位，那么它们也应该在代码字符串中。否则，Python 将抛出 ValueError：
```python
# Full month name date
full_month_date = "12 September 2022"

# Wrong format (missing space)
full_month_format = "%d%B %Y"

# Convert the string into a datetime object
datetime.strptime(full_month_date, full_month_format)
```
Output:
```python
---------------------------------------------------------------------------

ValueError                                Traceback (most recent call last)

Input In [18], in 
      5 full_month_format = "%d%B %Y"
      7 # Convert the string into a datetime object
----> 8 datetime.strptime(full_month_date, full_month_format)

File ~/coding/dataquest/articles/using-the-datetime-package/env/lib/python3.10/_strptime.py:568, in _strptime_datetime(cls, data_string, format)
    565 def _strptime_datetime(cls, data_string, format="%a %b %d %H:%M:%S %Y"):
    566     """Return a class cls instance based on the input string and the
    567     format string."""
--> 568     tt, fraction, gmtoff_fraction = _strptime(data_string, format)
    569     tzname, gmtoff = tt[-2:]
    570     args = tt[:6] + (fraction,)

File ~/coding/dataquest/articles/using-the-datetime-package/env/lib/python3.10/_strptime.py:349, in _strptime(data_string, format)
    347 found = format_regex.match(data_string)
    348 if not found:
--> 349     raise ValueError("time data %r does not match format %r" %
    350                      (data_string, format))
    351 if len(data_string) != found.end():
    352     raise ValueError("unconverted data remains: %s" %
    353                       data_string[found.end():])

ValueError: time data '12 September 2022' does not match format '%d%B %Y'
```
可以看到，即使缺少一个空格也可能导致错误！
<a name="M5m8L"></a>
## **将日期时间对象转换为字符串**
<a name="FVuYs"></a>
### `**strftime()**`
在 Python 中，还可以使用 `strftime()` 函数将日期时间对象转换为字符串。它有两个参数：一个日期时间对象和输出字符串的格式。
```python
# Create a datetime object
datetime_obj = datetime(2022, 12, 31)

# American date format
american_format = "%m-%d-%Y"

# European format
european_format = "%d-%m-%Y"

# American date string
print(f"American date string: {datetime.strftime(datetime_obj, american_format)}.")

# European date string
print(f"European date string: {datetime.strftime(datetime_obj, european_format)}.")
```
Output:
```python
American date string: 12-31-2022.
European date string: 31-12-2022.
```
采用相同的日期时间对象并将其转换为两种不同的格式。还可以指定其他格式，例如完整的月份名称后跟日期和年份。
```python
full_month = "%B %d, %Y"
datetime.strftime(datetime_obj, full_month)
```
Output:
```python
'December 31, 2022'
```
另一种使用 strftime 的方法是将它放在 datetime 对象之后：
```python
datetime_obj = datetime(2022, 12, 31, 23, 59, 59)
full_datetime = "%B %d, %Y %H:%M:%S"
datetime_obj.strftime(full_datetime)
```
Output:
```python
'December 31, 2022 23:59:59'
```
在实际使用当中，如果想提取不同年份 12 月 31 日的工作日名称，`strftime()` 可能很方便：
```python
# Extract the weekday name of December 31
weekday_format = "%A"

for year in range(2022, 2026):
    print(f"Weekday of December 31, {year} is {date(year, 12, 31).strftime(weekday_format)}.")
```
Output:
```python
Weekday of December 31, 2022 is Saturday.
Weekday of December 31, 2023 is Sunday.
Weekday of December 31, 2024 is Tuesday.
Weekday of December 31, 2025 is Wednesday.
```
<a name="QPuJn"></a>
## **时间戳**
在编程中，通常会看到以 Unix 时间戳格式存储的日期和时间，这种格式将任何日期表示为数字。一般情况时间戳是从 1970 年 1 月 1 日 00:00:00 UTC（协调世界时）开始的 Unix 纪元经过的秒数。可以使用 `timestamp()` 函数计算这个数字：
```python
new_year_2023 = datetime(2022, 12, 31)
datetime.timestamp(new_year_2023)
```
Output:
```python
1672441200.0
```
1672441200 就是从 Unix 纪元开始到 2022 年 12 月 31 日之间的秒数。<br />可以使用 `fromtimestamp()` 函数执行逆运算：
```python
datetime.fromtimestamp(1672441200)
```
Output:
```python
datetime.datetime(2022, 12, 31, 0, 0)
```
<a name="Jt6QJ"></a>
## **带日期的算术运算**
有时可能想要计算两个日期之间的差异或对日期和时间执行其他算术运算。幸运的是，Python 的工具包中有许多工具可以执行此类计算。
<a name="zoZa0"></a>
### **基本算术运算**
可以执行的第一个操作是计算两个日期之间的差异。为此，使用减号：
```python
# Instatiate two dates
first_date = date(2022, 1, 1)
second_date = date(2022, 12, 31)

# Difference between two dates
date_diff = second_date - first_date

# Function to convert datetime to string
def dt_string(date, date_format="%B %d, %Y"):
    return date.strftime(date_format)

print(f"The number of days and hours between {dt_string(first_date)} and {dt_string(second_date)} is {date_diff}.")
```
Output:
```python
The number of days and hours between January 01, 2022 and December 31, 2022 is 364 days, 0:00:00
```
看看 first_date - second_date 返回什么类型的对象：
```python
type(date_diff)
```
Output:
```python
datetime.timedelta
```
此对象的类型是 `datetime.timedelta`，它的名称中有 delta，指的是一个绿色字母 delta，在科学和工程中，描述了一种变化<实际上，这里它代表了时间上的变化（差异）。<br />如果只对两个日期之间的天数感兴趣怎么办？可以访问 timedelta 对象的不同属性，其中之一称为 `.days`。
```python
print(f"The number of days between {dt_string(first_date)} and {dt_string(second_date)} is {(date_diff).days}.")
```
Output:
```python
The number of days between January 01, 2022 and December 31, 2022 is 364.
```
<a name="wML3k"></a>
### `**timedelta()**`** 时间增量**
现在知道了 `timedelta` 对象，是时候介绍 `timedelta()` 函数了。它允许通过加减时间单位（如天、年、周、秒等）对时间对象执行许多算术运算。例如，可能想知道从现在起 30 天后是一周中的哪一天。为此，必须创建一个表示当前时间的对象和一个定义添加到其中的时间量的 timedelta 对象：
```python
# Import timedelta
from datetime import timedelta

# Current time
now = datetime.now()

# timedelta of 30 days
one_month = timedelta(days=30)

# Day in one month/using dt_string function defined above
print(f"The day in 30 days is {dt_string(now + one_month)}.")
```
Output:
```python
The day in 30 days is March 16, 2022.
```
如果查看 timedelta 函数的帮助页面 (`help(timedelta)`)，会看到它有以下参数：`days=0, seconds=0, microseconds=0, milliseconds=0, minutes=0, hours= 0，周=0`。因此还可以练习在日期中添加或减去其他时间单位。例如，可以计算 2030 年新年前 12 小时的时间：
```python
# New year 2030
new_year_2030 = datetime(2030, 1, 1, 0, 0)

# timedelta of 12 hours
twelve_hours = timedelta(hours=12)

# Time string of 12 hours before New Year 2023
twelve_hours_before = (new_year_2030 - twelve_hours).strftime("%B %d, %Y, %H:%M:%S")

# Print the time 12 hours before New Year 2023
print(f"The time twelve hours before New Year 2030 will be {twelve_hours_before}.")
```
Output:
```python
The time twelve hours before New Year 2030 will be December 31, 2029, 12:00:00.
```
还可以组合 `timedelta()` 函数的多个参数来计算出更具体的时间。例如，从现在起 27 天 3 小时 45 分钟后的时间是多少？
```python
# Current time
now = datetime.now()

# Timedelta of 27 days, 3 hours, and 45 minutes
specific_timedelta = timedelta(days=27, hours=3, minutes=45)

# Time in 27 days, 3 hours, and 45 minutes
twenty_seven_days = (now + specific_timedelta).strftime("%B %d, %Y, %H:%M:%S")

print(f"The time in 27 days, 3 hours, and 45 minutes will be {twenty_seven_days}.")
```
Output:
```python
The time in 27 days, 3 hours, and 45 minutes will be March 13, 2022, 15:18:39.
```
<a name="Oc7X5"></a>
### `**relativedelta()**`** 相对增量**
可以从帮助页面中看到该功能不允许使用几个月或几年。为了克服这个限制，可以使用 dateutil 包中的 `relativedelta` 函数。此函数与 `timedelta()` 非常相似，但它扩展了更多的功能。<br />例如，想从当前时间中减去 2 年 3 个月 4 天 5 小时：
```python
# Import relativedelta
from dateutil.relativedelta import relativedelta

# Current time
now = datetime.now()

# relativedelta object
relative_delta = relativedelta(years=2, months=3, days=4, hours=5)

two_years = (now - relative_delta).strftime("%B %d, %Y, %H:%M:%S")

print(f"The time 2 years, 3 months, 4 days, and 5 hours ago was {two_years}.")
```
Output:
```python
The time 2 years, 3 months, 4 days, and 5 hours ago was November 10, 2019, 06:33:40.
```
还可以使用 `relativedelta()` 来计算两个日期时间对象之间的差异：
```python
relativedelta(datetime(2030, 12, 31), now)
```
Output:
```python
relativedelta(years=+8, months=+10, days=+16, hours=+12, minutes=+26, seconds=+19, microseconds=+728345)
```
这些算术运算可能看起来非常抽象和不切实际，但实际上，它们在许多应用程序中都很有用。<br />比如说，脚本中的某个操作应该只在特定日期前 30 天执行。可以定义一个保存当前时间的变量，并为其添加一个 30 天的 timedelta 对象，如果今天是这一天，就会触发相关操作！<br />还有，假设正在使用 pandas 处理数据集，其中一列包含一些日期。想象一下，有一个数据集，其中保存着公司一年中的每一天的利润。想要创建另一个数据集，该数据集将保存距当前日期正好一年的日期，并预测每一天的利润，此时一定会在日期上使用算术计算！
<a name="vsVID"></a>
## **使用时区**
下面来看一看时区，它们可以有不同的形式。还应该知道，一些地区实施夏令时 (DST)，而另一些地区则没有。<br />Python 区分两种类型的日期和时间对象：`naive` 和 `aware`。一个 `naive` 对象不保存任何有关时区的信息，而 `aware` 对象则保存了它。<br />首先，看一个 `naive` 时间对象：
```python
# Import tzinfo
from datetime import tzinfo

# Naive datetime
naive_datetime = datetime.now()

# Naive datetime doesn't hold any timezone information
type(naive_datetime.tzinfo)
```
Output:
```python
NoneType
```
从 Python 3.9 开始，使用 Internet Assigned Numbers Authority 数据库实现了时区的具体实现，实现此功能的模块称为 zoneinfo。<br />使用 `zoneinfo`，特别是 ZoneInfo 类创建一个感知日期时间对象，它是 `datetime.tzinfo` 抽象类的一个实现：
```python
# Import ZoneInfo
from zoneinfo import ZoneInfo

utc_tz = ZoneInfo("UTC")

# Aware datetime object with UTC timezone
dt_utc = datetime.now(tz=utc_tz)

# The type of an aware object implemented with ZoneInfo is zoneinfo.ZoneInfo
type(dt_utc.tzinfo)
```
Output:
```python
zoneinfo.ZoneInfo
```
对于拥有 aware 的 datetime 对象具有有关时区的信息（实现为 `zoneinfo.ZoneInfo` 对象）。<br />看几个例子，想确定中欧和加利福尼亚的当前时间。<br />首先，可以在 `zoneinfo 中列出所有可用的时区：
```python
import zoneinfo

# Will return a long list of timezones (opens many files!)
zoneinfo.available_timezones()
```
Output:
```python
{'Africa/Abidjan',
 'Africa/Accra',
 'Africa/Addis_Ababa',
 'Africa/Algiers',
 'Africa/Asmara',
 'Africa/Asmera',
 'Africa/Bamako',
 'Africa/Bangui',
 'Africa/Banjul',
 'Africa/Bissau',
 'Africa/Blantyre',
 'Africa/Brazzaville',
 'Africa/Bujumbura',
 'Africa/Cairo',
 'Africa/Casablanca',
 'Africa/Ceuta',
 'Africa/Conakry',
 'Africa/Dakar',
 'Africa/Dar_es_Salaam',
 'Africa/Djibouti',
 'Africa/Douala',
 ...
 'build/etc/localtime'}
```
现在可以使用 ZoneInfo 来确定不同区域的当前时间：
```python
# Function to convert datetime into ISO formatted time
def iso(time, time_format="%Y-%m-%d %H:%M:%S"):
    return time.strftime(time_format)

# CET time zone
cet_tz = ZoneInfo("Europe/Paris")

# PST time zone
pst_tz = ZoneInfo("America/Los_Angeles")

# Current time in Central Europe
dt_cet = datetime.now(tz=cet_tz)

# Current time in California
dt_pst = datetime.now(tz=pst_tz)

print(f"Current time in Central Europe is {iso(dt_cet)}.")
print(f"Current time in California is {iso(dt_pst)}.")
```
Output:
```python
Current time in Central Europe is 2022-02-14 11:33:42.
Current time in California is 2022-02-14 02:33:42.
```
打印 `datetime.now(tz=cet_tz)`：
```python
print(datetime.now(tz=cet_tz))
```
Output:
```python
2022-02-14 11:33:43.048967+01:00
```
可以看到有 +01:00，它表示 UTC 偏移量。事实上，CET 时区比 UTC 早一小时。<br />此外，ZoneInfo 类处理夏令时。例如，可以将一天（24 小时）添加到 DST 更改发生的一天。
```python
# Define timedelta
time_delta = timedelta(days=1)

# November 5, 2022, 3 PM
november_5_2022 = datetime(2022, 11, 5, 15, tzinfo=pst_tz) # Note that we should use tzinfo in the datetime construct
print(november_5_2022)

# Add 1 day to November 5, 2022
print(november_5_2022 + time_delta)
```
Output:
```python
2022-11-05 15:00:00-07:00
2022-11-06 15:00:00-08:00
```
正如我们所见，偏移量从 -07:00 变为 -08:00，但时间保持不变（15:00）。
<a name="fusZk"></a>
## **2023 年新年倒数计时器**
New Your City 的时代广场在新年前夜吸引了成千上万的人。应用到目前为止所学的一切来为时代广场除夕创建一个倒数计时器！<br />在这里，将使用 dateutil 包中的 tz，它允许设置本地时区来演示 dateutil 包的实用程序。但是也可以使用 zoneinfo 中的 build/etc/localtime 时区来做同样的事情。
```python
from zoneinfo import ZoneInfo
from dateutil import tz
from datetime import datetime

def main():
	"""Main function."""
	# Set current time in our local time zone
	now = datetime.now(tz=tz.tzlocal())

	# New York City time zone
	nyc_tz = ZoneInfo("America/New_York")

	# New Year 2023 in NYC
	new_year_2023 = datetime(2023, 1, 1, 0, 0, tzinfo=nyc_tz)

	# Compute the time left to New Year in NYC
	countdown = relativedelta(new_year_2023, now)

	# Print time left to New Year 2023
	print(f"New Year in New Your City will come on: {new_year_2023.strftime('%B %-d, %Y %H:%M:%S')}.")
	print(f"Time left to New Year 2023 in NYC is: {countdown.months} months, {countdown.days} days, {countdown.hours} hours, {countdown.minutes} minutes, {countdown.seconds} seconds.")

if __name__ == "__main__":
	main()
```
Output:
```python
New Year in New Your City will come on: January 1, 2023 00:00:00.
Time left to New Year 2023 in NYC is: 10 months, 17 days, 18 hours, 26 minutes, 16 seconds.
```
将代码包装在 `main()` 函数中，现在可以在 .py 文件中使用它。在这个脚本中，处理了时区，创建了一个 datetime 对象，使用 `strftime()` 将其转换为字符串，甚至访问了 relativedelta 对象的时间属性！
