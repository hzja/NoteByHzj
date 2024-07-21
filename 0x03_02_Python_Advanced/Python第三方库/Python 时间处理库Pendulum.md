Python 时间处理
<a name="g2COr"></a>
## 安装Pendulum
```bash
pip install pendulum
```
安装后，展示一下：
```python
import pendulum

d1 = pendulum.yesterday()  # 昨天
# 2021-10-02T00:00:00+08:00

d2 = pendulum.today() # 今天
# 2021-10-03T00:00:00+08:00

d3 = pendulum.tomorrow() # 明天
# 2021-10-04T00:00:00+08:00

d2.diff(d1).in_days() # 相差多少天
# 1

d2.diff(d1).in_hours() # 相差多少小时
# 47

pendulum.now() # 现在的时间
```
是不是感觉很有亲和力，各种操作都符合人性化。<br />而且将 datetime 和 time 两个库合并了，再也不用纠结应该用哪个了。<br />下面浏览一下 Pendulum 的更大威力吧。
<a name="GaHtK"></a>
## 搞定时区
时区处理是比较麻烦地事情，还好只使用一个时区，省去了很多麻烦。<br />看例子：
```python
import pendulum

dt1 = pendulum.datetime(2021, 10, 3)
print(dt2.timezone.name) # UTC

dt2 = pendulum.datetime(2021, 10, 3, tz="Asia/Shanghai")

print(dt2.timezone.name) # Asia/Shanghai
```

- 不指定，就是默认的 UTC 0 时区
- 创建时间对象时可以指定时区，例如常用的时区名称是 `Asia/Shanghai`
- 通过 `timezone.name` 属性，可查看时间对象的具体时区，或者直接用 `timezone_name` 直接获取，另外，属性 `timezone` 可以简写成 `tz`

时区名称可在这里查：[时区名称](https://docs.aws.amazon.com/zh_cn/redshift/latest/dg/time-zone-names.html)<br />Pendulum 时区只支持用 时区名称 设置时区
<a name="C53G6"></a>
### 时区运算
不同时区的时间可以用来比较、运算和转化，例如：
```python
import pendulum

first = pendulum.datetime(2012, 9, 5, 23, 26, 11, 0, tz='America/Toronto')
second = pendulum.datetime(2012, 9, 5, 20, 26, 11, 0, tz='America/Vancouver')

first.to_datetime_string()
# 2012-09-05 23:26:11

first.timezone_name
# America/Toronto
second.to_datetime_string()
# 2012-09-05 20:26:11
second.timezone_name
# America/Vancouver

first == second
# True
first != second
# False
first > second
# False
first < second
# False

first = first.on(2012, 1, 1).at(0, 0, 0)
second = second.on(2012, 1, 1).at(0, 0, 0)
# tz is still America/Vancouver for second

first == second
# False
first != second
# True
first > second
# False
first < second
# True

second.diff(first).in_hours()
# 3
```

- 时区 `America/Toronto` 和 `America/Vancouver` 相差 3 小时
- 设置相差 3 小时的时间，两者是相等的
- 设置完全相同的时间，比较时是不相等的，计算它们之间的差异，可以看到相差 3 小时
- `diff` 方法用于与另一个时间对象比较，`in_hours` 方法是时间区间的方法，可以将区间转化为小时，同理还有 `in_days`、`in_years` 等
<a name="Y4EQI"></a>
### 时区切换
切换时区很方便，只要调用时间对象的 `in_timezone` 或者 缩写形式 `in_tz` 传入参数 时区名称 就可以了：
```python
import pendulum
in_perk = pendulum.now()
in_perk
# DateTime(2021, 10, 4, 12, 10, 59, 986047, tzinfo=Timezone('Asia/Shanghai'))

in_perk.in_timezone("America/New_York")
# DateTime(2021, 10, 4, 0, 7, 50, 106336, tzinfo=Timezone('America/New_York'))
```
<a name="S5jAs"></a>
## 时间运算
时间运算包括比较、计算差异 和 增减。上面例子中已经计算了不同时区的时间差异，下面详细的说一下。
<a name="HiLTJ"></a>
### 比较
比较很简单，对两个时间对象做比较就可以了，支持 `==`、`!=`、`>`、`>=`、`<`、`<=`，比较的结果是 `True` 或 `False`
<a name="dE9YS"></a>
### 计算差异
前面时区的例子里已经看到了，使用 `diff` 方法来计算差异，会返回一个时间区间(Period)对象<br />使用 `diff` 时，如果不提供比较参加，就会默认和当前时间比较。
```python
import pendulum

first = pendulum.datetime(2012, 1, 31, 0)
second = pendulum.datetime(2012, 2, 1, 0)
first.diff(second)
# <Period [2012-01-31T00:00:00+00:00 -> 2012-02-01T00:00:00+00:00]>

first.diff()
# <Period [2012-01-31T00:00:00+00:00 -> 2021-10-04T04:44:56.337989+00:00]>
```
对于时间区间（`period`）来说，处理前面提到的 `in_days` 等方法，将区间转化为一个单位的数量，还可以直接使用对应的属性，例如 `days`、`years`、`weeks` 等，效果是一样的。
<a name="qFOJq"></a>
### 增减
如果要的时间增加或者减少某些时间，可以使用 `add` 和 `subtract` 方法：
```python
import pendulum

dt = pendulum.datetime(2012, 1, 31)

dt = dt.add(years=5)
# '2017-01-31 00:00:00'
dt = dt.add(years=1)
# '2018-01-31 00:00:00'
dt = dt.subtract(years=1)
# '2017-01-31 00:00:00'
dt = dt.subtract(years=5)
# '2012-01-31 00:00:00'

dt = dt.add(months=60)
# '2017-01-31 00:00:00'
dt = dt.add(months=1)
# '2017-02-28 00:00:00'
dt = dt.subtract(months=1)
# '2017-01-28 00:00:00'
dt = dt.subtract(months=60)
# '2012-01-28 00:00:00'

dt = dt.add(days=29)
# '2012-02-26 00:00:00'
dt = dt.add(days=1)
# '2012-02-27 00:00:00'
dt = dt.subtract(days=1)
# '2012-02-26 00:00:00'
dt = dt.subtract(days=29)
# '2012-01-28 00:00:00'

dt = dt.add(weeks=3)
# '2012-02-18 00:00:00'
dt = dt.add(weeks=1)
# '2012-02-25 00:00:00'
dt = dt.subtract(weeks=1)
# '2012-02-18 00:00:00'
dt = dt.subtract(weeks=3)
# '2012-01-28 00:00:00'

dt = dt.add(hours=24)
# '2012-01-29 00:00:00'
dt = dt.add(hours=1)
# '2012-02-25 01:00:00'
dt = dt.subtract(hours=1)
# '2012-02-29 00:00:00'
dt = dt.subtract(hours=24)
# '2012-01-28 00:00:00'

dt = dt.add(minutes=61)
# '2012-01-28 01:01:00'
dt = dt.add(minutes=1)
# '2012-01-28 01:02:00'
dt = dt.subtract(minutes=1)
# '2012-01-28 01:01:00'
dt = dt.subtract(minutes=24)
# '2012-01-28 00:00:00'

dt = dt.add(seconds=61)
# '2012-01-28 00:01:01'
dt = dt.add(seconds=1)
# '2012-01-28 00:01:02'
dt = dt.subtract(seconds=1)
# '2012-01-28 00:01:01'
dt = dt.subtract(seconds=61)
# '2012-01-28 00:00:00'

dt = dt.add(years=3, months=2, days=6, hours=12, minutes=31, seconds=43)
# '2015-04-03 12:31:43'
dt = dt.subtract(years=3, months=2, days=6, hours=12, minutes=31, seconds=43)
# '2012-01-28 00:00:00'
```

- `add` 和 `subtract` 方法参数一致，支持 `years`、`months`、`weeks` 等多种时间单位，而且可以一起设置
- 时间单位参数可以支持负数，相当于 `add` 和 `subtract` 可以相互替换
- 时间单位参数还支持小数，比如加上一天半可以写成 `dt.add(days=1.5)`
<a name="h56lT"></a>
## 时间调整
时间调整很有用，在[约马程序](https://mp.weixin.qq.com/s?__biz=MzkxNDI3NjcwMw==&mid=2247493875&idx=1&sn=e027db3808562ad4aaaee3b722b12ad0&scene=21#wechat_redirect) 中，需要计算下一个周一，当时只能通过日期 API 摸：
```python
import pendulum

dt = pendulum.datetime(2012, 1, 31, 12, 0, 0)

dt.start_of('day')
# '2012-01-31 00:00:00'

dt.end_of('day')
# '2012-01-31 23:59:59'

dt.start_of('month')
# '2012-01-01 00:00:00'

dt.end_of('month')
# '2012-01-31 23:59:59'

dt.start_of('year')
# '2012-01-01 00:00:00'

dt.end_of('year')
# '2012-12-31 23:59:59'

dt.start_of('decade')
# '2010-01-01 00:00:00'

dt.end_of('decade')
# '2019-12-31 23:59:59'

dt.start_of('century')
# '2000-01-01 00:00:00'

dt.end_of('century')
# '2099-12-31 23:59:59'

dt.start_of('week')
# '2012-01-30 00:00:00'
dt.day_of_week == pendulum.MONDAY
# True # ISO8601 week starts on Monday

dt.end_of('week')
# '2012-02-05 23:59:59'
dt.day_of_week == pendulum.SUNDAY
# True # ISO8601 week ends on SUNDAY

dt.next(pendulum.WEDNESDAY)
# '2012-02-01 00:00:00'
dt.day_of_week == pendulum.WEDNESDAY
# True

dt = pendulum.datetime(2012, 1, 1, 12, 0, 0)
dt.next()
# '2012-01-08 00:00:00'
dt.next(keep_time=True)
# '2012-01-08T12:00:00+00:00'

dt = pendulum.datetime(2012, 1, 31, 12, 0, 0)
dt.previous(pendulum.WEDNESDAY)
# '2012-01-25 00:00:00'
dt.day_of_week == pendulum.WEDNESDAY
# True

dt = pendulum.datetime(2012, 1, 1, 12, 0, 0)
dt.previous()
# '2011-12-25 00:00:00'
dt.previous(keep_time=True)
# '2011-12-25 12:00:00'

start = pendulum.datetime(2014, 1, 1)
end = pendulum.datetime(2014, 1, 30)
start.average(end)
# '2014-01-15 12:00:00'

# others that are defined that are similar
# and tha accept month, quarter and year units
# first_of(), last_of(), nth_of()
```

- `start_of` 方法用于计算某个起始时间，可以是 天、年、月、周，甚至可以是世纪。end_of 是类似的，用于计算结束
- `next` 方法用于计算以一个星期，不加参数就是计算下个星期的今天，也可以指定计算下一个哪天，比如下个星期一：`dt.next(pendulum.MONDAY)`。`previous` 是类似的，用于计算向前的天
- `average` 方法用于计算两个时间的中间时间，简直太方便了
<a name="XdR0c"></a>
## 时间转字符串
时间对象是一个复杂的对象，不方便看和读，就需要将起转化为字符串，或者将字符串表示的时间转化为时间对象。<br />Pendulum 提供和很多方便的方式：
```python
import pendulum

dt = pendulum.datetime(1975, 12, 25, 14, 15, 16)
print(dt)
# '1975-12-25T14:15:16+00:00'

dt.to_date_string()
# '1975-12-25'

dt.to_formatted_date_string()
# 'Dec 25, 1975'

dt.to_time_string()
# '14:15:16'

dt.to_datetime_string()
# '1975-12-25 14:15:16'

dt.to_day_datetime_string()
'Thu, Dec 25, 1975 2:15 PM'

# You can also use the format() method
dt.format('dddd Do [of] MMMM YYYY HH:mm:ss A')
'Thursday 25th of December 1975 02:15:16 PM'

# Of course, the strftime method is still available
dt.strftime('%A %-d%t of %B %Y %I:%M:%S %p')
'Thursday 25th of December 1975 02:15:16 PM'
```

- `to_date_string` 转化日期
- `to_datetime_string` 转化日期和时间
- `to_time_string` 转化时间
- `to_formatted_date_string` 转化为英文书写形式
- `format` 安装指定格式转化
- `strftime` 同 `datetime` 的格式化方法

时间转化字符串，如何将字符串转化为时间类型呢？<br />使用 `parse` 方法就好了，看下例子：
```python
import pendulum

dt = pendulum.parse('1975-05-21T22:00:00')
print(dt)
# '1975-05-21T22:00:00+00:00

# You can pass a tz keyword to specify the timezone
dt = pendulum.parse('1975-05-21T22:00:00', tz='Europe/Paris')
print(dt)
# '1975-05-21T22:00:00+01:00'

# Not ISO 8601 compliant but common
dt = pendulum.parse('1975-05-21 22:00:00')

dt = pendulum.parse('31-01-01', strict=False)

dt = pendulum.parse('31/01/01', strict=False)

dt = pendulum.parse('31/1/1', strict=False)
```

- 可以直接转化，也可以在转化时指定时区
- 支持多种时间格式，如果不是标准的时间格式，需要添加参数 `strict=False`，这样 Pendulum 就会尽最大可能去猜
- 更多的时间格式请参考 [Pendulum Parsing 文档](https://pendulum.eustace.io/docs/#parsing)
<a name="s4rSe"></a>
## 人性化
现在看到 Pendulum 的强大了吧，不过还有个令人叫绝的功能，就是人性化时间。<br />如果注意搜索引擎的结果的话，就能看到，很多时间被表示为：1天前，2周后等等。<br />现在直接看例子：
```python
import pendulum

pendulum.now().subtract(days=1).diff_for_humans()
# '1 day ago'

pendulum.now().diff_for_humans(pendulum.now().subtract(years=1))
# '1 year after'

dt = pendulum.datetime(2011, 8, 1)
dt.diff_for_humans(dt.add(months=1))
# '1 month before'
dt.diff_for_humans(dt.subtract(months=1))
# '1 month after'

pendulum.now().add(seconds=5).diff_for_humans()
# '5 seconds from now'

pendulum.now().subtract(days=24).diff_for_humans()
# '3 weeks ago'

pendulum.now().subtract(days=24).diff_for_humans(absolute=True)
# '3 weeks'

pendulum.set_locale('zh')
pendulum.now().subtract(days=24).diff_for_humans()
# '3周前'

pendulum.now().add(seconds=5).diff_for_humans()
# '5秒钟后'

oneday = pendulum.now().diff(pendulum.now().add(days=1))
oneday.in_words()
# '1天'
```

- `diff_for_humans` 可以将时间区间直接输出成人性化时间
- 参数 `absolute` 的作用是给出一个人性化的时间间隔，而不是相对于现在的说法
- 默认情况下输出的是英语写法，如果要让说中文，通过 `pendulum.set_locale('zh')` 就可以了，厉害吧
- 对于一个时间区域来说，可以用 in_words 来输出人性化时间

这里注意，对于 `dt.diff_for_humans(dt.subtract(months=1))` 这样的，相对一个日期的时间差异人性化输出，不支持中文，会报错，所以建议先计算出两个日期的差异，再用 `in_words` 做人性化输出，如果有必要，在其后加上前 或 后。<br />希望这个 bug 能早日修复
<a name="BLVUZ"></a>
### 参考资料
Pendulum 工具: [https://pendulum.eustace.io/](https://pendulum.eustace.io/)<br />时区名称: [https://docs.aws.amazon.com/zh_cn/redshift/latest/dg/time-zone-names.html](https://docs.aws.amazon.com/zh_cn/redshift/latest/dg/time-zone-names.html)<br />Pendulum Parsing 文档: [https://pendulum.eustace.io/docs/#parsing](https://pendulum.eustace.io/docs/#parsing)
