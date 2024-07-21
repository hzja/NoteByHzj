Python
<a name="FJ3oo"></a>
## 1、集合
开发人员常常忘记 Python 也有集合数据类型，大家都喜欢使用列表处理一切。集合（set）是什么？简单来说就是：集合是一组无序事物的汇集，不包含重复元素。如果熟练掌握集合及其逻辑，那么很多问题都可以迎刃而解。举个例子，如何获取一个单词中出现的字母？
```python
myword = "NanananaBatman"
set(myword)
{'N', 'm', 'n', 'B', 'a', 't'}
```
这个例子就来自 Python 的官方文档，再举一个例子，如何获取一个列表的各个元素，且不重复？
```python
# first you can easily change set to list and other way around
mylist = ["a", "b", "c","c"]
# let's make a set out of it
myset = set(mylist)
# myset will be:
{'a', 'b', 'c'}
# and, it's already iterable so you can do:
for element in myset:
    print(element)
# but you can also convert it to list again:
mynewlist = list(myset)
# and mynewlist will be:
['a', 'b', 'c']
```
可以看到，“c”元素不再重复出现了。只有一个地方需要注意，mylist 与 mynewlist 之间的元素顺序可能会有所不同：
```python
mylist = ["c", "c", "a","b"]
mynewlist = list(set(mylist))
# mynewlist is:
['a', 'b', 'c']
```
下面，来进一步深入。<br />假设某些实体之间有一对多的关系，举个更加具体的例子：用户与权限。通常，一个用户可以拥有多个权限。现在假设某人想要修改多个权限，即同时添加和删除某些权限，应当如何解决这个问题？
```python
# this is the set of permissions before change;
original_permission_set = {"is_admin","can_post_entry", "can_edit_entry", "can_view_settings"}
# this is new set of permissions;
new_permission_set = {"can_edit_settings","is_member", "can_view_entry", "can_edit_entry"}
# now permissions to add will be:
new_permission_set.difference(original_permission_set)
# which will result:
{'can_edit_settings', 'can_view_entry', 'is_member'}
# As you can see can_edit_entry is in both sets; so we do notneed
# to worry about handling it
# now permissions to remove will be:
original_permission_set.difference(new_permission_set)
# which will result:
{'is_admin', 'can_view_settings', 'can_post_entry'}
# and basically it's also true; we switched admin to member, andadd
# more permission on settings; and removed the post_entrypermission
```
总的来说，不要害怕使用集合，它们能解决很多问题。
<a name="WWOS0"></a>
## 2、日历
当开发与日期和时间有关的功能时，有些信息可能非常重要，比如某一年的这个月有多少天。这个问题看似简单，但是日期和时间是一个非常有难度的话题，而且日历的实现问题非常多，简直就是噩梦，因为需要考虑大量的极端情况。<br />那么，究竟如何才能找出某个月有多少天呢？
```python
import calendar
calendar.monthrange(2020, 12)
# will result:
(1, 31)
# BUT! you need to be careful here, why? Let's read thedocumentation:
help(calendar.monthrange)
# Help on function monthrange in module calendar:
# monthrange(year, month)
#     Return weekday (0-6~ Mon-Sun) and number of days (28-31) for
#    year, month.
# As you can see the first value returned in tuple is a weekday,
# not the number of the first day for a given month; let's try
# to get the same for 2021
calendar.monthrange(2021, 12)
(2, 31)
# So this basically means that the first day of December 2021 isWed
# and the last day of December 2021 is 31 (which is obvious,cause
# December always has 31 days)
# let's play with February
calendar.monthrange(2021, 2)
(0, 28)
calendar.monthrange(2022, 2)
(1, 28)
calendar.monthrange(2023, 2)
(2, 28)
calendar.monthrange(2024, 2)
(3, 29)
calendar.monthrange(2025, 2)
(5, 28)
# as you can see it handled nicely the leap year;
```
某个月的第一天当然非常简单，就是 1 号。但是，“某个月的第一天是周X”，如何使用这条信息呢？可以很容易地查到某一天是周几：
```python
calendar.monthrange(2024, 2)
(3, 29)
# means that February 2024 starts on Thursday
# let's define simple helper:
weekdays = ["Monday", "Tuesday","Wednesday", "Thursday", "Friday","Saturday", "Sunday"]
# now we can do something like:
weekdays[3]
# will result in:
'Thursday'
# now simple math to tell what day is 15th of February 2020:
offset = 3  # it's thefirst value from monthrange
for day in range(1, 29):
    print(day,weekdays[(day + offset - 1) % 7])
1 Thursday
2 Friday
3 Saturday
4 Sunday
...
18 Sunday
19 Monday
20 Tuesday
21 Wednesday
22 Thursday
23 Friday
24 Saturday
...
28 Wednesday
29 Thursday
# which basically makes sense;
```
也许这段代码不适合直接用于生产，可以使用 datetime 更容易地查找星期：
```python
from datetime import datetime
mydate = datetime(2024, 2, 15)
datetime.weekday(mydate)
# will result:
3
# or:
datetime.strftime(mydate, "%A")
'Thursday'
```
总的来说，日历模块有很多有意思的地方，值得慢慢学习：
```python
# checking if year is leap:
calendar.isleap(2021)  #False
calendar.isleap(2024)  #True
# or checking how many days will be leap days for given yearspan:
calendar.leapdays(2021, 2026) # 1
calendar.leapdays(2020, 2026) # 2
# read the help here, as range is: [y1, y2), meaning that second
# year is not included;
calendar.leapdays(2020, 2024) # 1
```
<a name="liRCL"></a>
## 3、枚举有第二个参数
是的，枚举有第二个参数，可能很多有经验的开发人员都不知道。下面来看一个例子：
```python
mylist = ['a', 'b', 'd', 'c', 'g', 'e']
for i, item in enumerate(mylist):
    print(i, item)
# Will give:
0 a
1 b
2 d
3 c
4 g
5 e
# but, you can add a start for enumeration:
for i, item in enumerate(mylist, 16):
    print(i, item)
# and now you will get:
16 a
17 b
18 d
19 c
20 g
21 e
```
第二个参数可以指定枚举开始的地方，比如上述代码中的 `enumerate(mylist,16)`。如果需要处理偏移量，则可以考虑这个参数。
<a name="CbqgS"></a>
## 4、if-else 逻辑
经常需要根据不同的条件，处理不同的逻辑，经验不足的开发人员可能会编写出类似下面的代码：
```python
OPEN = 1
IN_PROGRESS = 2
CLOSED = 3
def handle_open_status():
    print('Handling openstatus')
def handle_in_progress_status():
    print('Handling inprogress status')
def handle_closed_status():
    print('Handling closedstatus')
def handle_status_change(status):
    if status == OPEN:
       handle_open_status()
    elif status ==IN_PROGRESS:
        handle_in_progress_status()
    elif status == CLOSED:
       handle_closed_status()
handle_status_change(1)  #Handling open status
handle_status_change(2)  #Handling in progress status
handle_status_change(3)  #Handling closed status
```
虽然这段代码看上去也没有那么糟，但是如果有 20 多个条件呢？<br />那么，究竟应该怎样处理呢？
```python
from enum import IntEnum
class StatusE(IntEnum):
    OPEN = 1
    IN_PROGRESS = 2
    CLOSED = 3
def handle_open_status():
    print('Handling openstatus')
def handle_in_progress_status():
    print('Handling inprogress status')
def handle_closed_status():
    print('Handling closedstatus')
handlers = {
    StatusE.OPEN.value:handle_open_status,
   StatusE.IN_PROGRESS.value: handle_in_progress_status,
    StatusE.CLOSED.value:handle_closed_status
}
def handle_status_change(status):
    if status not inhandlers:
         raiseException(f'No handler found for status: {status}')
    handler =handlers[status]
    handler()
handle_status_change(StatusE.OPEN.value)  # Handling open status
handle_status_change(StatusE.IN_PROGRESS.value)  # Handling in progress status
handle_status_change(StatusE.CLOSED.value)  # Handling closed status
handle_status_change(4)  #Will raise the exception
```
在 Python 中这种模式很常见，它可以让代码看起来更加整洁，尤其是当方法非常庞大，而且需要处理大量条件时。
<a name="cSOdj"></a>
## 5、`enum` 模块
`enum` 模块提供了一系列处理枚举的工具函数，最有意思的是 `Enum` 和 `IntEnum`。来看个例子：
```python
from enum import Enum, IntEnum, Flag, IntFlag
class MyEnum(Enum):
    FIRST ="first"
    SECOND ="second"
    THIRD ="third"
 
class MyIntEnum(IntEnum):
    ONE = 1
    TWO = 2
    THREE = 3
# Now we can do things like:
MyEnum.FIRST  #
# it has value and name attributes, which are handy:
MyEnum.FIRST.value  #'first'
MyEnum.FIRST.name  #'FIRST'
# additionally we can do things like:
MyEnum('first')  #, get enum by value
MyEnum['FIRST']  #, get enum by name
```
使用 IntEnum 编写的代码也差不多，但是有几个不同之处：
```python
MyEnum.FIRST == "first"  # False
# but
MyIntEnum.ONE == 1  # True
# to make first example to work:
MyEnum.FIRST.value == "first"  # True
```
在中等规模的代码库中，enum 模块在管理常量方面可以提供很大的帮助。enum 的本地化可能有点棘手，但也可以实现：
```python
from enum import Enum
from django.utils.translation import gettext_lazy as _
class MyEnum(Enum):
    FIRST ="first"
    SECOND ="second"
    THIRD ="third"
    @classmethod
    def choices(cls):
        return [
            (cls.FIRST.value, _('first')),
            (cls.SECOND.value, _('second')),
            (cls.THIRD.value, _('third'))
         ]
# And later in eg. model definiton:
some_field = models.CharField(max_length=10,choices=MyEnum.choices())
```
<a name="TrhBY"></a>
## 6、iPython
iPython 就是交互式 Python，它是一个交互式的命令行 shell，有点像 Python 解释器。<br />首先，需要安装 iPython：
```python
pip install ipython
```
接下来，只需要在输入命令的时候，将 Python 换成 ipython：
```python
# you should see something like this after you start:
Python 3.8.5 (default, Jul 28 2020, 12:59:40)
Type 'copyright', 'credits' or 'license' for more information
IPython 7.18.1 -- An enhanced Interactive Python. Type '?' forhelp.
In [1]:
```
ipython 支持很多系统命令，比如 ls 或 cat，tab 键可以显示提示，而且还可以使用上下键查找前面用过的命令。
