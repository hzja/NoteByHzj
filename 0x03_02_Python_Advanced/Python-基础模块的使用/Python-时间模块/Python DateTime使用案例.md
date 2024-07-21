Pyhton 

- 使用 time 模块展示当前日期和时间
- 将天、小时、分钟转换为秒
- 使用 Pandas 获取当前日期和时间
- 将字符串转换为日期时间对象
- 以毫秒为单位获取当前时间
- 以 MST、EST、UTC、GMT 和 HST 获取当前日期时间
- 从给定的日期当中获取星期几
- 计算两个日期时间对象之间的时差
- 将 5 分钟添加到 Unix 时间戳
- 在 Python 中遍历一系列日期
- 巴黎时间更改为纽约时间
- 使用 Python 获得最后7个工作日
- 从今天的日期和一个人的生日推算年龄
- 获得本月的第一个星期二
- 将整数转换为日期对象
- 当前日期减去 N 天的天数
- 比较两个日期
- 从 datetime 对象中提取年份
- 在 Python 中找到星期几
- 从当前日期获取 7 天前的日期
- 将两个日期时间对象之间的差值转换为秒
- 获得任何一个月的第三个星期五
- 从 Python 中的周数获取日期
- 获取特定日期的工作日
- 创建一个 15 分钟前的 DateTime
- 从特定日期获取周的开始和结束日期
- 两个日期之间的差异（以秒为单位）
- 以这种格式获取昨天的日期MMDDYY
- 从今天的日期获取上周三
- 所有可用时区的列表打印
- 获取指定开始日期和结束日期之间的日期范围
- 毫秒转换为数据
- 查找给定日期之后的第一个星期日的日期
- 将（Unix）时间戳秒转换为日期和时间字符串
- 以月为单位的两个日期之间的差异
- 将本地时间字符串转换为 UTC
- 获取当月的最后一个星期四
- 从特定日期查找一年中的第几周
- 从给定日期获取星期几
- 用 AM PM 打印当前时间
- 获得一个月的最后一天
- 从工作日值中获取工作日名称
- 将 N 小时数添加到当前日期时间
- 从当前日期获取年、月、日、小时、分钟
- 获取特定月份和年份的最后一个星期日
- 查找特定日期的年份中的哪一天
- 查找当前日期是工作日还是周末
- 组合 datetime.date 和 datetime.time 对象
- 获得每月的第 5 个星期一
- 将日期时间对象转换为日期对象
- 获取没有微秒的当前日期时间
- 将 N 秒数添加到特定日期时间
- 从当前日期获取两位数的月份和日期
- 从特定日期获取月份数据的开始和结束日期
- 以周为单位的两个日期之间的差异
- 将字符串格式的日期转换为 Unix 时间戳
- 获取最后一个周日和周六的日期
- 检查对象是否属于 datetime.date 类型
- 获取特定日期的周数
- 获取 UTC 时间
- 获取本周的开始和结束日期
- 两个日期之间的差异（以分钟为单位）
- 将日期时间对象转换为日期字符串
- 获得上周五
- 将 3 周添加到任何特定日期
- 在其他两个日期之间生成一个随机日期
- 查找从今天开始的第一个星期一的日期
- 两个日期之间的差异（以天为单位）
- 向当前日期添加六个月
- 将数据时间对象转换为 Unix（时间戳）
- 将年、月、日、时、分、秒的 N 个数字添加到当前日期时间
- 获取指定开始日期和结束日期之间的日期范围
- 减去 N 个年、月、日、时、分、秒到当前日期时间
- 获取指定年份和月份的月份第一天的工作日和月份的天数
- 打印特定年份的所有星期一
- 打印特定年份的日历
- 从月份编号中获取月份名称
- 从给定日期获取一周的开始和结束日期
- 根据当前日期查找上一个和下一个星期一的日期
- 获取当前季度的第一个日期和最后一个日期
<a name="Ai6Cj"></a>
## 1、使用 time 模块展示当前日期和时间
```python
import time
from time import gmtime, strftime
 
t = time.localtime()
print (time.asctime(t))
print(strftime("%a, %d %b %Y %H:%M:%S +0000", gmtime()))
print(strftime("%A", gmtime()))
print(strftime("%D", gmtime()))
print(strftime("%B", gmtime()))
print(strftime("%y", gmtime()))
 
# Convert seconds into GMT date
print(strftime("%a, %d %b %Y %H:%M:%S +0000", gmtime(1234567890)))
```
Output:
```python
Sun May 7 09:30:37 2017
Sun, 07 May 2017 04:00:37 +0000
Sunday
05/07/17
May
17
Fri, 13 Feb 2009 23:31:30 +0000
```
<a name="SjOvF"></a>
## 2、将天、小时、分钟转换为秒
```python
SECONDS_PER_MINUTE  = 60
SECONDS_PER_HOUR    = 3600
SECONDS_PER_DAY     = 86400
 
#Read the inputs from user
days    = int(input("Enter number of Days: "))
hours   = int(input("Enter number of Hours: "))
minutes = int(input("Enter number of Minutes: "))
seconds = int(input("Enter number of Seconds: "))
 
#Calculate the days, hours, minutes and seconds
total_seconds = days * SECONDS_PER_DAY
total_seconds = total_seconds + ( hours * SECONDS_PER_HOUR)
total_seconds = total_seconds + ( minutes * SECONDS_PER_MINUTE)
total_seconds = total_seconds + seconds
 
#Display the result
print("Total number of seconds: ","%d"%(total_seconds))
```
Output:
```python
Enter number of Days: 5
Enter number of Hours: 36
Enter number of Minutes: 24
Enter number of Seconds: 15
Total number of seconds: 563055
```
<a name="uDoVG"></a>
## 3、使用 Pandas 获取当前日期和时间
```python
import pandas as pd
print(pd.datetime.now())
print(pd.datetime.now().date())
print(pd.datetime.now().year)
print(pd.datetime.now().month)
print(pd.datetime.now().day)
print(pd.datetime.now().hour)
print(pd.datetime.now().minute)
print(pd.datetime.now().second)
print(pd.datetime.now().microsecond)
```
Output:
```python
2018-01-19 16:08:28.393553
2018-01-19
2018
1
19
16
8
28
394553
```
<a name="ZWoq6"></a>
## 4、将字符串转换为日期时间对象
```python
from datetime import datetime
from dateutil import parser

d1 = "Jan 7 2015  1:15PM"
d2 = "2015 Jan 7  1:33PM"

# If you know date format
date1 = datetime.strptime(d1, '%b %d %Y %I:%M%p')
print(type(date1))
print(date1)

# If you don't know date format
date2 = parser.parse(d2)
print(type(date2))
print(date2)
```
Output:
```python
class 'datetime.datetime'
2015-01-07 13:15:00

class 'datetime.datetime'
2015-01-07 13:33:00
```
<a name="rhKdk"></a>
## 5、以毫秒为单位获取当前时间
```python
import time
 
milliseconds = int(round(time.time() * 1000))
print(milliseconds)
```
Output:
```python
1516364270650
```
<a name="JqvYc"></a>
## 6、以 MST、EST、UTC、GMT 和 HST 获取当前日期时间
```python
from datetime import datetime
from pytz import timezone
 
mst = timezone('MST')
print("Time in MST:", datetime.now(mst))
 
est = timezone('EST')
print("Time in EST:", datetime.now(est))
 
utc = timezone('UTC')
print("Time in UTC:", datetime.now(utc))
 
gmt = timezone('GMT')
print("Time in GMT:", datetime.now(gmt))
 
hst = timezone('HST')
print("Time in HST:", datetime.now(hst))
```
Output:
```python
Time in MST: 2017-01-19 06:06:14.495605-07:00
Time in EST: 2017-01-19 08:06:14.496606-05:00
Time in UTC: 2017-01-19 13:06:14.496606+00:00
Time in GMT: 2017-01-19 13:06:14.496606+00:00
Time in HST: 2017-01-19 03:06:14.497606-10:00
```
<a name="hTiwe"></a>
## 7、从给定的日期当中获取星期几
```python
import datetime
 
dayofweek = datetime.date(2010, 6, 16).strftime("%A")
print(dayofweek)
# weekday Monday is 0 and Sunday is 6
print("weekday():", datetime.date(2010, 6, 16).weekday())
 
# isoweekday() Monday is 1 and Sunday is 7
print("isoweekday()", datetime.date(2010, 6, 16).isoweekday())
 
dayofweek = datetime.datetime.today().strftime("%A")
print(dayofweek)
print("weekday():", datetime.datetime.today().weekday())
print("isoweekday()", datetime.datetime.today().isoweekday())
```
Output:
```python
Wednesday
weekday(): 2
isoweekday() 3
Friday
weekday(): 4
isoweekday() 5
```
<a name="g5ROm"></a>
## 8、计算两个日期时间对象之间的时差
```python
import datetime
from datetime import timedelta
 
datetimeFormat = '%Y-%m-%d %H:%M:%S.%f'
date1 = '2016-04-16 10:01:28.585'
date2 = '2016-03-10 09:56:28.067'
diff = datetime.datetime.strptime(date1, datetimeFormat)\
    - datetime.datetime.strptime(date2, datetimeFormat)
 
print("Difference:", diff)
print("Days:", diff.days)
print("Microseconds:", diff.microseconds)
print("Seconds:", diff.seconds)
```
Output:
```python
Difference: 37 days, 0:05:00.518000
Days: 37
Microseconds: 518000
Seconds: 300
```
<a name="MGGQy"></a>
## 9、将 5 分钟添加到 Unix 时间戳
```python
import datetime
import calendar
 
future = datetime.datetime.utcnow() + datetime.timedelta(minutes=5)
print(calendar.timegm(future.timetuple()))
```
Output:
```python
1621069619
```
<a name="bgag0"></a>
## 10、在 Python 中遍历一系列日期
```python
import datetime

start = datetime.datetime.strptime("21-06-2020", "%d-%m-%Y")
end = datetime.datetime.strptime("05-07-2020", "%d-%m-%Y")
date_generated = [start + datetime.timedelta(days=x) for x in range(0, (end - start).days)]

for date in date_generated:
    print(date.strftime("%d-%m-%Y"))
```
Output:
```python
21-06-2020
22-06-2020
23-06-2020
24-06-2020
25-06-2020
26-06-2020
27-06-2020
28-06-2020
29-06-2020
30-06-2020
01-07-2020
02-07-2020
03-07-2020
04-07-2020
```
<a name="yKrBv"></a>
## 11、巴黎时间更改为纽约时间
```python
import pendulum
 
in_paris = pendulum.datetime(2016, 8, 7, 22, 24, 30, tz='Europe/Paris')
print(in_paris)
 
in_us = in_paris.in_timezone('America/New_York')
print(in_us)
```
Output:
```python
2016-08-07T22:24:30+02:00
2016-08-07T16:24:30-04:00
```
<a name="rpcAN"></a>
## 12、使用 Python 获得最后7个工作日
```python
from datetime import date
from datetime import timedelta
 
today = date.today()
 
for i in range(7):
    d = today - timedelta(days=i)
    if d.weekday() < 5:
        print(d)
```
Output:
```python
2021-05-18
2021-05-17
2021-05-14
2021-05-13
2021-05-12
```
<a name="Kn5xr"></a>
## 13、从今天的日期和一个人的生日推算年龄
```python
from datetime import date
 
 
def calculate_age(born):
    today = date.today()
    try:
        birthday = born.replace(year=today.year)
    except ValueError:
        birthday = born.replace(year=today.year, month=born.month + 1, day=1)
    if birthday > today:
        return today.year - born.year - 1
    else:
        return today.year - born.year
 
 
print(calculate_age(date(2001, 3, 1)))
```
Output:
```python
20
```
<a name="WXOYP"></a>
## 14、获得本月的第一个星期二
```python
import calendar
from datetime import datetime
 
c = calendar.Calendar(firstweekday=calendar.SUNDAY)
monthcal = c.monthdatescalendar(datetime.today().year, datetime.today().month)
 
try:
    tues = [day for week in monthcal for day in week if
            day.weekday() == calendar.TUESDAY and day.month == datetime.today().month][0]
    print(tues)
except IndexError:
    print('No date found')
```
Output:
```python
2021-05-04
```
<a name="NMUz7"></a>
## 15、将整数转换为日期对象
```python
from datetime import datetime

i = 1545730073
timestamp = datetime.fromtimestamp(i)

print(timestamp)
print(type(timestamp))
```
Output:
```python
2018-12-25 14:57:53
```
<a name="GiAEx"></a>
## 16、当前日期减去 N 天的天数
```python
from datetime import datetime, timedelta

d = datetime.today() - timedelta(days=5)
print(d)
```
Output:
```python
2021-05-10 12:59:14.867969
```
<a name="tqvdo"></a>
## 17、比较两个日期
```python
import datetime

a = datetime.datetime(2020, 12, 31, 23, 59, 59)
b = datetime.datetime(2020, 11, 30, 23, 59, 59)

print(a < b)
print(a > b)
```
Output:
```python
False
True
```
<a name="lJJV9"></a>
## 18、从 datetime 对象中提取年份
```python
import datetime
 
year = datetime.date.today().year
print(year)
```
Output:
```python
2021
```
<a name="ob3xw"></a>
## 19、在 Python 中找到星期几
```python
import pendulum
 
dt = pendulum.parse('2021-05-18')
print(dt.day_of_week)
 
dt = pendulum.parse('2021-05-01')
print(dt.day_of_week)
 
dt = pendulum.parse('2021-05-21')
print(dt.day_of_week)
```
Output:
```python
2
6
5
```
<a name="ZzAuE"></a>
## 20、从当前日期获取 7 天前的日期
```python
from datetime import datetime, timedelta
 
now = datetime.now()
 
for x in range(7):
    d = now - timedelta(days=x)
    print(d.strftime("%Y-%m-%d"))
```
Output:
```python
2021-05-18
2021-05-17
2021-05-16
2021-05-15
2021-05-14
2021-05-13
2021-05-12
```
<a name="mZWuf"></a>
## 21、将两个日期时间对象之间的差值转换为秒
```python
import datetime
 
time1 = datetime.datetime.strptime('19 01 2021', '%d %m %Y')
time2 = datetime.datetime.strptime('25 01 2021', '%d %m %Y')
 
difference = time2 - time1
print(difference)
 
seconds = difference.total_seconds()
print(seconds)
```
Output:
```python
6 days, 0:00:00
518400.0
```
<a name="CXp55"></a>
## 22、获得任何一个月的第三个星期五
```python
import calendar
 
c = calendar.Calendar(firstweekday=calendar.SUNDAY)
year = 2021
month = 5
monthcal = c.monthdatescalendar(year, month)
 
try:
    third_friday = [day for week in monthcal for day in week if
                    day.weekday() == calendar.FRIDAY and day.month == month][2]
    print(third_friday)
except IndexError:
    print('No date found')
```
Output:
```python
2021-05-21
```
<a name="KzlwY"></a>
## 23、从 Python 中的周数获取日期
```python
import datetime
from dateutil.relativedelta import relativedelta
 
week = 25
year = 2021
date = datetime.date(year, 1, 1) + relativedelta(weeks=+week)
print(date)
```
Output:
```python
2021-06-25
```
<a name="b2mHs"></a>
## 24、获取特定日期的工作日
```python
import datetime
 
print(datetime.date(2020, 5, 15).isocalendar()[2])
```
Output:
```python
5
```
<a name="easu1"></a>
## 25、创建一个 15 分钟前的 DateTime
```python
import datetime
 
dt = datetime.datetime.now() - datetime.timedelta(minutes=15)
print(dt)
```
Output:
```python
2021-05-15 22:25:55.897365
```
<a name="rHuph"></a>
## 26、从特定日期获取周的开始和结束日期
```python
import pendulum
 
dt = pendulum.datetime(2012, 9, 5)
 
start = dt.start_of('week')
print(start.to_datetime_string())
 
end = dt.end_of('week')
print(end.to_datetime_string())
```
Output:
```python
2012-09-03 00:00:00
2012-09-09 23:59:59
```
<a name="XUMRu"></a>
## 27、两个日期之间的差异（以秒为单位）
```python
from datetime import datetime
 
fmt = '%Y-%m-%d %H:%M:%S'
d1 = datetime.strptime('2020-01-01 17:31:22', fmt)
d2 = datetime.strptime('2020-01-03 17:31:22', fmt)
 
days_diff = d2 - d1
print(days_diff.days * 24 * 60 * 60)
```
Output:
```python
172800
```
<a name="uAv50"></a>
## 28、以这种格式获取昨天的日期MMDDYY
```python
from datetime import date, timedelta
 
yesterday = date.today() - timedelta(days=1)
print(yesterday.strftime('%m%d%y'))
```
Output:
```python
051421
```
<a name="Y0CEJ"></a>
## 29、从今天的日期获取上周三
```python
from datetime import date
from datetime import timedelta

today = date.today()

offset = (today.weekday() - 2) % 7
wednesday = today - timedelta(days=offset)
print(wednesday)
```
Output:
```python
2021-05-12
```
<a name="a5jht"></a>
## 30、所有可用时区的列表打印
```python
import pytz

for i in pytz.all_timezones:
    print(i)
```
Output:
```python
Africa/Abidjan
Africa/Accra
Africa/Addis_Ababa
Africa/Algiers
Africa/Asmara
Africa/Asmera
Africa/Bamako
Africa/Bangui
Africa/Banjul
Africa/Bissau
...
US/Mountain
US/Pacific
US/Samoa
UTC
Universal
W-SU
WET
Zulu
```
<a name="QdojN"></a>
## 31、获取指定开始日期和结束日期之间的日期范围
```python
import datetime
 
start = datetime.datetime.strptime("21-06-2020", "%d-%m-%Y")
end = datetime.datetime.strptime("05-07-2020", "%d-%m-%Y")
date_generated = [start + datetime.timedelta(days=x) for x in range(0, (end - start).days)]
 
for date in date_generated:
    print(date.strftime("%d-%m-%Y"))
```
Output:
```python
21-06-2020
22-06-2020
23-06-2020
24-06-2020
25-06-2020
26-06-2020
27-06-2020
28-06-2020
29-06-2020
30-06-2020
01-07-2020
02-07-2020
03-07-2020
04-07-2020
```
<a name="gfHXw"></a>
## 32、毫秒转换为数据
```python
import datetime

time_in_millis = 1596542285000
dt = datetime.datetime.fromtimestamp(time_in_millis / 1000.0, tz=datetime.timezone.utc)
print(dt)
```
Output:
```python
2020-08-04 11:58:05+00:00
```
<a name="fZ5Lp"></a>
## 33、查找给定日期之后的第一个星期日的日期
```python
import datetime
 
 
def next_weekday(d, weekday):
    days_ahead = weekday - d.weekday()
    if days_ahead <= 0:
        days_ahead += 7
    return d + datetime.timedelta(days_ahead)
 
 
d = datetime.date(2021, 5, 16)
next_sunday = next_weekday(d, 6)
print(next_sunday)
```
Output:
```python
2021-05-23
```
<a name="EKcUv"></a>
## 34、将（Unix）时间戳秒转换为日期和时间字符串
```python
from datetime import datetime

dateStr = datetime.fromtimestamp(1415419007).strftime("%A, %B %d, %Y %I:%M:%S")
print(type(dateStr))
print(dateStr)
```
Output:
```python
Saturday, November 08, 2014 09:26:47
```
<a name="ZtFCi"></a>
## 35、以月为单位的两个日期之间的差异
```python
from datetime import datetime
from dateutil import relativedelta
 
date1 = datetime.strptime('2014-01-12 12:00:00', '%Y-%m-%d %H:%M:%S')
date2 = datetime.strptime('2021-07-15 12:00:00', '%Y-%m-%d %H:%M:%S')
 
r = relativedelta.relativedelta(date2, date1)
print(r.months + (12 * r.years))
```
Output:
```python
90
```
<a name="yYoD3"></a>
## 36、将本地时间字符串转换为 UTC
```python
from datetime import *
from dateutil import *
from dateutil.tz import *
 
 
utc_zone = tz.gettz('UTC')
local_zone = tz.gettz('America/Chicago')
 
utc_zone = tz.tzutc()
local_zone = tz.tzlocal()
 
 
local_time = datetime.strptime("2020-10-25 15:12:00", '%Y-%m-%d %H:%M:%S')
print(local_time)
local_time = local_time.replace(tzinfo=local_zone)
print(local_time)
 
utc_time = local_time.astimezone(utc_zone)
print(utc_time)
 
utc_string = utc_time.strftime('%Y-%m-%d %H:%M:%S')
print(utc_string)
```
Output:
```python
2020-10-25 15:12:00
2020-10-25 15:12:00+05:30
2020-10-25 09:42:00+00:00
2020-10-25 09:42:00
```
<a name="pUOjW"></a>
## 37、获取当月的最后一个星期四
```python
import calendar
from datetime import datetime
 
month = calendar.monthcalendar(datetime.today().year, datetime.today().month)
 
thrusday = max(month[-1][calendar.THURSDAY], month[-2][calendar.THURSDAY])
print(thrusday)
```
Output:
```python
27
```
<a name="KxqX0"></a>
## 38、从特定日期查找一年中的第几周
```python
import pendulum
 
dt = pendulum.parse('2015-05-18')
print(dt.week_of_year)
 
dt = pendulum.parse('2019-12-01')
print(dt.week_of_year)
 
dt = pendulum.parse('2018-01-21')
print(dt.week_of_year)
```
Output:
```python
21
48
3
```
<a name="k1Ma5"></a>
## 39、从给定日期获取星期几
```python
import datetime
import calendar
 
dt = datetime.datetime(2021, 4, 25, 23, 24, 55, 173504)
print(calendar.day_name[dt.weekday()])
```
Output:
```python
Sunday
```
<a name="ODGeD"></a>
## 40、用 AM PM 打印当前时间
```python
from datetime import datetime
 
print(datetime.today().strftime("%I:%M %p"))
```
Output:
```python
10:11 PM
```
<a name="MnN3l"></a>
## 41、获得一个月的最后一天
```python
import calendar
 
print(calendar.monthrange(2002, 1)[1])
print(calendar.monthrange(2008, 6)[1])
print(calendar.monthrange(2012, 2)[1])
print(calendar.monthrange(2015, 2)[1])
```
Output:
```python
31
30
29
28
```
<a name="cmnYe"></a>
## 42、从工作日值中获取工作日名称
```python
import calendar
 
print(calendar.day_name[0])
print(calendar.day_name[1])
print(calendar.day_name[2])
print(calendar.day_name[3])
print(calendar.day_name[4])
print(calendar.day_name[5])
print(calendar.day_name[6])
```
Output:
```python
Monday
Tuesday
Wednesday
Thursday
Friday
Saturday
Sunday
```
<a name="JDQGQ"></a>
## 43、将 N 小时数添加到当前日期时间
```python
from datetime import datetime, timedelta

d = datetime.today() + timedelta(hours=18)
print(d)
```
Output:
```python
2021-05-16 07:36:08.189948
```
<a name="JKzRb"></a>
## 44、从当前日期获取年、月、日、小时、分钟
```python
import datetime
 
now = datetime.datetime.now()
print(now.year, now.month, now.day, now.hour, now.minute, now.second)
```
Output:
```python
2021 5 15 14 27 33
```
<a name="YoE15"></a>
## 45、获取特定月份和年份的最后一个星期日
```python
import calendar

month = calendar.monthcalendar(2021, 2)

last_sunday = max(month[-1][calendar.SUNDAY], month[-2][calendar.SUNDAY])
print(last_sunday)
```
Output:
```python
28
```
<a name="rbJxd"></a>
## 46、查找特定日期的年份中的哪一天
```python
import pendulum
 
dt = pendulum.parse('2015-05-18')
print(dt.day_of_year)
 
dt = pendulum.parse('2019-12-01')
print(dt.day_of_year)
 
dt = pendulum.parse('2018-01-21')
print(dt.day_of_year)
```
Output:
```python
138
335
21
```
<a name="omIV1"></a>
## 47、查找当前日期是工作日还是周末
```python
import datetime
 
weekno = datetime.datetime.today().weekday()
if weekno < 5:
    print("Weekday")
else:  # 5 Sat, 6 Sun
    print("Weekend")
```
Output:
```python
Weekday
```
<a name="hLsZ3"></a>
## 48、组合 `datetime.date` 和 `datetime.time` 对象
```python
import datetime
 
d = datetime.datetime.combine(datetime.date(2020, 11, 14),
                              datetime.time(10, 23, 15))
 
print(d)
```
Output:
```python
2020-11-14 10:23:15
```
<a name="Ul2NI"></a>
## 49、获得每月的第 5 个星期一
```python
import calendar
 
c = calendar.Calendar(firstweekday=calendar.SUNDAY)
year = 2016
month = 2
monthcal = c.monthdatescalendar(year, month)
 
try:
    fifth_monday = [day for week in monthcal for day in week if
                    day.weekday() == calendar.MONDAY and day.month == month][4]
    print(fifth_monday)
except IndexError:
    print('No date found')
```
Output:
```python
2016-02-29
```
<a name="SiIn6"></a>
## 50、将日期时间对象转换为日期对象
```python
from datetime import datetime

datetime_obj = datetime(2020, 12, 15, 10, 15, 45, 321474)
print(datetime_obj)

date_obj = datetime_obj.date()
print(date_obj)
```
Output:
```python
2020-12-15 10:15:45.321474
2020-12-15
```
<a name="dec8P"></a>
## 51、获取没有微秒的当前日期时间
```python
from datetime import datetime

print(datetime.now().isoformat(' ', 'seconds'))
```
Output:
```python
2021-05-15 12:55:45
```
<a name="vjybo"></a>
## 52、将 N 秒数添加到特定日期时间
```python
import datetime

a = datetime.datetime(2020, 12, 31, 23, 59, 45)
b = a + datetime.timedelta(seconds=30)

print(a)
print(b)
```
Output:
```python
2020-12-31 23:59:45
2021-01-01 00:00:15
```
<a name="CtK4U"></a>
## 53、从当前日期获取两位数的月份和日期
```python
import datetime
 
dt = datetime.datetime.now()
 
print(dt.strftime('%m'))
print('{:02d}'.format(dt.month))
print(f'{dt.month:02d}')
print('%02d' % dt.month)
 
print(dt.strftime('%d'))
print('{:02d}'.format(dt.day))
print(f'{dt.day:02d}')
print('%02d' % dt.day)
```
Output:
```python
05
05
05
05
15
15
15
15
```
<a name="N5Eoh"></a>
## 54、从特定日期获取月份数据的开始和结束日期
```python
import pendulum
 
dt = pendulum.datetime(2012, 9, 5)
 
start = dt.start_of('month')
print(start.to_datetime_string())
 
end = dt.end_of('month')
print(end.to_datetime_string())
```
Output:
```python
2012-09-01 00:00:00
2012-09-30 23:59:59
```
<a name="U3CGn"></a>
## 55、以周为单位的两个日期之间的差异
```python
from datetime import date
 
date1 = date(2020, 12, 23)
date2 = date(2021, 5, 11)
 
days = abs(date1 - date2).days
print(days // 7)
```
Output:
```python
19
```
<a name="QnQmR"></a>
## 56、将字符串格式的日期转换为 Unix 时间戳
```python
import datetime
 
stime = '15/05/2021'
print(datetime.datetime.strptime(stime, "%d/%m/%Y").timestamp())
```
Output:
```python
1621017000.0
```
<a name="p90kY"></a>
## 57、获取最后一个周日和周六的日期
```python
from datetime import datetime, timedelta
 
 
def prior_week_end():
    return datetime.now() - timedelta(days=((datetime.now().isoweekday() + 1) % 7))
 
 
def prior_week_start():
    return prior_week_end() - timedelta(days=6)
 
 
print('Sunday', format(prior_week_start()))
print('Saturday', format(prior_week_end()))
```
Output:
```python
Sunday 2021-05-09 13:13:30.057765
Saturday 2021-05-15 13:13:30.058912
```
<a name="dt6qL"></a>
## 58、检查对象是否属于 datetime.date 类型
```python
import datetime
 
x = '2012-9-1'
y = datetime.date(2012, 9, 1)
 
print(isinstance(x, datetime.date))
print(isinstance(y, datetime.date))
```
Output:
```python
False
True
```
<a name="gFHzW"></a>
## 59、获取特定日期的周数
```python
import datetime
 
print(datetime.date(2020, 5, 15).isocalendar()[1])
```
Output:
```python
20
```
<a name="RK9qu"></a>
## 60、获取 UTC 时间
```python
from datetime import datetime

dt = datetime.utcnow()
print(dt)
```
Output:
```python
2021-05-15 17:01:31.008808
```
<a name="zft1T"></a>
## 61、获取本周的开始和结束日期
```python
import pendulum

today = pendulum.now()

start = today.start_of('week')
print(start.to_datetime_string())

end = today.end_of('week')
print(end.to_datetime_string())
```
Output:
```python
2021-05-10 00:00:00
2021-05-16 23:59:59
```
<a name="RqpTO"></a>
## 62、两个日期之间的差异（以分钟为单位）
```python
from datetime import datetime
 
fmt = '%Y-%m-%d %H:%M:%S'
d1 = datetime.strptime('2010-01-01 17:31:22', fmt)
d2 = datetime.strptime('2010-01-03 17:31:22', fmt)
 
days_diff = d2 - d1
print(days_diff.days * 24 * 60)
```
Output:
```python
2880
```
<a name="jnTZZ"></a>
## 63、将日期时间对象转换为日期字符串
```python
import datetime
 
t = datetime.datetime(2020, 12, 23)
x = t.strftime('%m/%d/%Y')
print(x)
```
Output:
```python
12/23/2020
```
<a name="pds2P"></a>
## 64、获得上周五
```python
from datetime import date
from datetime import timedelta

today = date.today()

offset = (today.weekday() - 4) % 7
friday = today - timedelta(days=offset)
print(friday)
```
Output:
```python
2021-05-14
```
<a name="K9tao"></a>
## 65、将 3 周添加到任何特定日期
```python
import pendulum
 
dt = pendulum.datetime(2012, 2, 15)
dt = dt.add(weeks=3)
print(dt.to_date_string())
```
Output:
```python
2012-03-07
```
<a name="L07fZ"></a>
## 66、在其他两个日期之间生成一个随机日期
```python
import random
import time
 
 
def str_time_prop(start, end, time_format, prop):
    stime = time.mktime(time.strptime(start, time_format))
    etime = time.mktime(time.strptime(end, time_format))
    ptime = stime + prop * (etime - stime)
    return time.strftime(time_format, time.localtime(ptime))
 
 
def random_date(start, end, prop):
    return str_time_prop(start, end, '%m/%d/%Y %I:%M %p', prop)
 
 
print(random_date("1/1/2020 1:10 PM", "1/1/2021 1:10 AM", random.random()))
```
Output:
```python
02/25/2020 08:26 AM
```
<a name="lZlnu"></a>
## 67、查找从今天开始的第一个星期一的日期
```python
from dateutil.rrule import rrule, WEEKLY, MO
from datetime import date
 
next_monday = rrule(freq=WEEKLY, dtstart=date.today(), byweekday=MO, count=1)[0]
print(next_monday)
```
Output:
```python
2021-05-17 00:00:00
```
<a name="cqFFs"></a>
## 68、两个日期之间的差异（以天为单位）
```python
from datetime import date
 
d1 = date(2019, 8, 18)
d2 = date(2021, 12, 10)
 
days_diff = d2 - d1
print(days_diff.days)
```
Output:
```python
845
```
<a name="S3uSw"></a>
## 69、向当前日期添加六个月
```python
from datetime import datetime
from dateutil.relativedelta import *

date = datetime.now()
print(date)

date = date + relativedelta(months=+6)
print(date)
```
Output:
```python
2021-05-15 13:48:52.135612
2021-11-15 13:48:52.135612
```
<a name="qW5y8"></a>
## 70、将数据时间对象转换为 Unix（时间戳）
```python
import datetime
import time

# Saturday, October 10, 2015 10:10:00 AM
date_obj = datetime.datetime(2015, 10, 10, 10, 10)
print("Unix Timestamp: ", (time.mktime(date_obj.timetuple())))
```
Output:
```python
Unix Timestamp: 1444452000.0
```
<a name="r8VTK"></a>
## 71、将年、月、日、时、分、秒的 N 个数字添加到当前日期时间
```python
import datetime
from dateutil.relativedelta import relativedelta

add_days = datetime.datetime.today() + relativedelta(days=+6)
add_months = datetime.datetime.today() + relativedelta(months=+6)
add_years = datetime.datetime.today() + relativedelta(years=+6)

add_hours = datetime.datetime.today() + relativedelta(hours=+6)
add_mins = datetime.datetime.today() + relativedelta(minutes=+6)
add_seconds = datetime.datetime.today() + relativedelta(seconds=+6)

print("Current Date Time:", datetime.datetime.today())
print("Add 6 days:", add_days)
print("Add 6 months:", add_months)
print("Add 6 years:", add_years)
print("Add 6 hours:", add_hours)
print("Add 6 mins:", add_mins)
print("Add 6 seconds:", add_seconds)
```
Output:
```python
Current Date Time: 2017-04-04 18:32:10.192671
Add 6 days: 2017-04-10 18:32:10.191671
Add 6 months: 2017-10-04 18:32:10.192671
Add 6 years: 2023-04-04 18:32:10.192671
Add 6 hours: 2017-04-05 00:32:10.192671
Add 6 mins: 2017-04-04 18:38:10.192671
Add 6 seconds: 2017-04-04 18:32:16.192671
```
<a name="jxIIz"></a>
## 72、获取指定开始日期和结束日期之间的日期范围
```python
import datetime
 
start = datetime.datetime.strptime("2016-06-15", "%Y-%m-%d")
end = datetime.datetime.strptime("2016-06-30", "%Y-%m-%d")
date_array = \
    (start + datetime.timedelta(days=x) for x in range(0, (end-start).days))
 
for date_object in date_array:
    print(date_object.strftime("%Y-%m-%d"))
```
Output:
```python
2016-06-15
2016-06-16
2016-06-17
2016-06-18
2016-06-19
2016-06-20
2016-06-21
2016-06-22
2016-06-23
2016-06-24
2016-06-25
2016-06-26
2016-06-27
2016-06-28
2016-06-29
```
<a name="lJ5CX"></a>
## 73、减去 N 个年、月、日、时、分、秒到当前日期时间
```python
import datetime
from dateutil.relativedelta import relativedelta

sub_days = datetime.datetime.today() + relativedelta(days=-6)
sub_months = datetime.datetime.today() + relativedelta(months=-6)
sub_years = datetime.datetime.today() + relativedelta(years=-6)

sub_hours = datetime.datetime.today() + relativedelta(hours=-6)
sub_mins = datetime.datetime.today() + relativedelta(minutes=-6)
sub_seconds = datetime.datetime.today() + relativedelta(seconds=-6)

print("Current Date Time:", datetime.datetime.today())
print("Subtract 6 days:", add_days)
print("Subtract 6 months:", add_months)
print("Subtract 6 years:", add_years)
print("Subtract 6 hours:", add_hours)
print("Subtract 6 mins:", add_mins)
print("Subtract 6 seconds:", add_seconds)
```
Output:
```python
Current Date Time: 2017-04-04 18:36:29.213046
Subtract 6 days: 2017-03-29 18:36:29.213046
Subtract 6 months: 2016-10-04 18:36:29.213046
Subtract 6 years: 2011-04-04 18:36:29.213046
Subtract 6 hours: 2017-04-04 12:36:29.213046
Subtract 6 mins: 2017-04-04 18:30:29.213046
Subtract 6 seconds: 2017-04-04 18:36:23.213046
```
<a name="ul4Q1"></a>
## 74、获取指定年份和月份的月份第一天的工作日和月份的天数
```python
import calendar
 
print("Year:2002 - Month:2")
month_range = calendar.monthrange(2002, 2)
print("Weekday of first day of the month:", month_range[0])
print("Number of days in month:", month_range[1])
print()
print("Year:2010 - Month:5")
month_range = calendar.monthrange(2010, 5)
print("Weekday of first day of the month:", month_range[0])
print("Number of days in month:", month_range[1])
```
Output:
```python
Year:2002 - Month:2
Weekday of first day of the month: 4
Number of days in month: 28

Year:2010 - Month:5
Weekday of first day of the month: 5
Number of days in month: 31
```
<a name="q62bE"></a>
## 75、打印特定年份的所有星期一
```python
from datetime import date, timedelta

year = 2018
date_object = date(year, 1, 1)
date_object += timedelta(days=1-date_object.isoweekday())

while date_object.year == year:
    print(date_object)
    date_object += timedelta(days=7)
```
Output:
```python
2018-01-01
2018-01-08
2018-01-15
2018-01-22
2018-01-29
2018-02-05
2018-02-12
...
2018-11-12
2018-11-19
2018-11-26
2018-12-03
2018-12-10
2018-12-17
2018-12-24
2018-12-31
```
<a name="RW8FM"></a>
## 76、打印特定年份的日历
```python
import calendar
cal_display = calendar.TextCalendar(calendar.MONDAY)
# Year: 2019
# Column width: 1
# Lines per week: 1 
# Number of spaces between month columns: 0
# No. of months per column: 2
print(cal_display.formatyear(2019, 1, 1, 0, 2))
```
Output:
```python
略
```
<a name="RiLt6"></a>
## 77、从月份编号中获取月份名称
```python
import calendar
import datetime

# Month name from number
print("Month name from number 5:")
month_num = 1
month_abre = datetime.date(2015, month_num, 1).strftime('%b')
month_name = datetime.date(2015, month_num, 1).strftime('%B')
print("Short Name:", month_abre)
print("Full  Name:", month_name)

print("\nList of all months from calendar")
# Print list of all months from calendar
for month_val in range(1, 13):
    print(calendar.month_abbr[month_val], "-", calendar.month_name[month_val])
```
Output:
```python
Month name from number 5:
Short Name: Jan
Full Name: January

List of all months from calendar
Jan - January
Feb - February
Mar - March
Apr - April
May - May
Jun - June
Jul - July
Aug - August
Sep - September
Oct - October
Nov - November
Dec - December
```
<a name="Nkxfw"></a>
## 78、从给定日期获取一周的开始和结束日期
```python
from datetime import datetime, timedelta
 
date_str = '2018-01-14'
date_obj = datetime.strptime(date_str, '%Y-%m-%d')
 
start_of_week = date_obj - timedelta(days=date_obj.weekday())  # Monday
end_of_week = start_of_week + timedelta(days=6)  # Sunday
print(start_of_week)
print(end_of_week)
```
Output:
```python
2018-01-08 00:00:00
2018-01-14 00:00:00
```
<a name="Hi1sD"></a>
## 79、根据当前日期查找上一个和下一个星期一的日期
```python
import datetime

today = datetime.date.today()
last_monday = today - datetime.timedelta(days=today.weekday())
coming_monday = today + datetime.timedelta(days=-today.weekday(), weeks=1)
print("Today:", today)
print("Last Monday:", last_monday)
print("Coming Monday:", coming_monday)
```
Output:
```python
Today: 2018-01-21
Last Monday: 2018-01-15
Coming Monday: 2018-01-22
```
<a name="rvBA8"></a>
## 80、获取当前季度的第一个日期和最后一个日期
```python
from datetime import datetime, timedelta

current_date = datetime.now()
current_quarter = round((current_date.month - 1) / 3 + 1)
first_date = datetime(current_date.year, 3 * current_quarter - 2, 1)
last_date = datetime(current_date.year, 3 * current_quarter + 1, 1)\
+ timedelta(days=-1)

print("First Day of Quarter:", first_date)
print("Last Day of Quarter:", last_date)
```
Output:
```python
First Day of Quarter: 2018-01-01 00:00:00
Last Day of Quarter: 2018-03-31 00:00:00
```
