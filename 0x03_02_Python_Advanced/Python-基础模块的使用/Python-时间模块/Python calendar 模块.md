Python calendar<br />calendar 是与日历相关的模块，该模块定义了很多类型，主要包括：Calendar、TextCalendar、HTMLCalendar，其中 Calendar 是 TextCalendar 和 HTMLCalendar 的基类，这些类有着十分丰富的日历处理方法。
<a name="kEOwz"></a>
## 1、calendar 模块
对于简单的文本日历，使用 calendar 模块直接调用的方法或属性即可，下面来具体看一下。
<a name="h0mVj"></a>
### `setfirstweekday(firstweekday)`
设置每一周的开始（0 表示星期一，6 表示星期天），calendar 提供了 MONDAY、TUESDAY、WEDNESDAY、THURSDAY、FRIDAY、SATURDAY 和 SUNDAY 几个常量方便使用。例如，设置每周的第一天为星期天，示例如下：
```python
import calendar
calendar.setfirstweekday(calendar.SUNDAY)
```
<a name="m61fh"></a>
### `firstweekday()`
返回当前设置的每星期的第一天的数值，默认为星期一。示例如下：
```python
import calendar
print(calendar.firstweekday())
```
<a name="n39J5"></a>
### `isleap(year)`
如果 year 是闰年则返回 True，否则返回 False。示例如下：
```python
import calendar
print(calendar.isleap(9102))
```
<a name="UEQ4d"></a>
### `leapdays(y1, y2)`
返回在范围 y1 至 y2 （包含 y1 和 y2 ）之间的闰年的年数，其中 y1 和 y2 是年份。示例如下：
```python
import calendar
print(calendar.leapdays(1945,2019))
```
<a name="fkH3j"></a>
### `weekday(year, month, day)`
返回一周中的某一天 （ 0 ~ 6 代表周一至周日）。示例如下：
```python
import calendar
print(calendar.weekday(2019,10,1))
```
<a name="iMbhV"></a>
### `monthrange(year, month)`
返回指定年、月份第一天是星期几和这个月的天数。示例如下：
```python
import calendar
print(calendar.monthrange(2019, 10))
```
<a name="DckBb"></a>
### `monthcalendar(year, month)`
返回表示一个月的日历的矩阵，每一行代表一周，此月份外的日子由零表示，每周默认从周一开始，可使用 `setfirstweekday()` 改变默认值。示例如下：
```python
import calendar
print(calendar.monthcalendar(2019,10))
```
<a name="AZGMx"></a>
### `prmonth(theyear, themonth, w=0, l=0)`
打印由 `month()` 返回的一个月的日历，每日宽度间隔为 w 字符，l 是每星期的行数。示例如下：
```python
import calendar
print(calendar.prmonth(2019,10))
```
<a name="A5kMU"></a>
### `prcal(year, w=0, l=0, c=6, m=3)`
打印由 `calendar()` 返回的整年的日历。示例如下：
```python
import calendar
print(calendar.prcal(2019))
```
<a name="js7YE"></a>
### `day_name`
在当前的语言环境下表示星期几的列表。示例如下：
```python
import calendar
print(calendar.day_name[0])
```
<a name="oDn9h"></a>
### `day_abbr`
在当前语言环境下表示星期几缩写的列表。示例如下：
```python
import calendar
print(calendar.day_abbr[0])
```
<a name="NNJgs"></a>
### `month_name`
在当前语言环境下表示一年中月份的列表，一月的月号为 1，它的长度为 13 且 month_name[0] 是空字符串。示例如下：
```python
import calendar
print(calendar.month_name[1])
```
<a name="y1btE"></a>
### `month_abbr`
在当前语言环境下表示月份简写的列表。示例如下：
```python
import calendar
print(calendar.month_abbr[1])
```
<a name="V50sk"></a>
## 2、Calendar 类
Calendar 对象提供了一些可被用于准备日历数据格式化的方法，这个类本身不执行任何格式化操作，这部分任务应由子类来完成，下面具体看一下。
<a name="l4B46"></a>
### `iterweekdays()`
返回一个迭代器，迭代器的内容为一星期的数字。示例如下：
```python
from calendar import Calendar
c = Calendar()
print(list(c.iterweekdays()))
```
<a name="zHko9"></a>
### `itermonthdates(year, month)`
返回一个迭代器，迭代器的内容为 year 年 month 月（1-12）的日期。示例如下：
```python
from calendar import Calendar
c = Calendar()
print(list(c.itermonthdates(2019,10)))
```
<a name="yJi9s"></a>
### `itermonthdays2(year, month)`
返回一个迭代器，迭代器的内容与 `itermonthdates()` 类似为 year 年 month 月的日期，但不受 `datetime.date` 范围的限制，迭代器中的元素为一个由日期和代表星期几的数字组成的的元组。示例如下：
```python
from calendar import Calendar
c = Calendar()
print(list(c.itermonthdays2(2019,10)))
```
<a name="sWKAG"></a>
### `itermonthdays3(year, month)`
返回一个迭代器，迭代器的内容与 `itermonthdates()` 类似为 year 年 month 月的日期，不受 `datetime.date` 范围的限制，迭代器的元素为一个由年、月、日组成的元组。示例如下：
```python
from calendar import Calendar
c = Calendar()
print(list(c.itermonthdays3(2019,10)))
```
<a name="tJIdH"></a>
## 3、TextCalendar 类
TextCalendar 对象提供了一些生成纯文本日历的方法，下面具体看一下。
<a name="EyTXq"></a>
### `formatmonth(theyear, themonth, w=0, l=0)`
返回一个多行字符串来表示指定年月的日历，w 为日期的宽度，始终保持日期居中，l 指定了每星期占用的行数；以上这些还依赖于构造器或者 `setfirstweekday()` 方法指定的周的第一天是哪一天。示例如下：
```python
from calendar import TextCalendar
tc = TextCalendar()
print(tc.formatmonth(2019,10))
```
<a name="hCard"></a>
### `prmonth(theyear, themonth, w=0, l=0)`
与 `formatmonth()` 方法一样，返回一个月的日历。示例如下：
```python
from calendar import TextCalendar
tc = TextCalendar()
print(tc.prmonth(2019,10))
```
<a name="XWjd9"></a>
### `formatyear(theyear, w=2, l=1, c=6, m=3)`
返回一个多行字符串，这个字符串为一个 m 列日历，可选参数 w、l、c 分别表示日期列数、周的行数、月之间的间隔；以上这些还依赖于构造器或者 `setfirstweekday()` 指定哪一天为一周的第一天。示例如下：
```python
from calendar import TextCalendar
tc = TextCalendar()
print(tc.formatyear(2019))
```
<a name="gvJH1"></a>
### `pryear(theyear, w=0, l=0, c=6, m=3)`
与 `formatyear()` 方法一样，返回一整年的日历。示例如下：
```python
from calendar import TextCalendar
tc = TextCalendar()
print(tc.pryear(2019))
```
<a name="EvJYB"></a>
## 4、HTMLCalendar 类
HTMLCalendar 对象提供了一些生成 HTML 日历的方法，下面具体看一下。
<a name="QSJ7K"></a>
### `formatmonth(theyear, themonth, withyear=True)`
返回一个 HTML 表格作为指定年月的日历，withyear 为 True，则年份将会包含在表头，否则只显示月份。示例如下：
```python
from calendar import HTMLCalendar
hc = HTMLCalendar()
print(hc.formatmonth(2019,10))
```
<a name="pxLp9"></a>
### `formatyear(theyear, width=3)`
返回一个 HTML 表格作为指定年份的日历，width 用于规定每一行显示月份的数量。示例如下：
```python
from calendar import HTMLCalendar
hc = HTMLCalendar()
print(hc.formatyear(2019))
```
<a name="RHGhO"></a>
### `formatyearpage(theyear, width=3, css='calendar.css', encoding=None)`
返回一个完整的 HTML 页面作为指定年份的日历，width 用于规定每一行显示的月份数量，css 为层叠样式表的名字，如果不使用任何层叠样式表，可以使用 None，encoding 为输出页面的编码 (默认为系统的默认编码)。示例如下：
```python
from calendar import HTMLCalendar
hc = HTMLCalendar()
print(hc.formatyearpage(2019))
```
