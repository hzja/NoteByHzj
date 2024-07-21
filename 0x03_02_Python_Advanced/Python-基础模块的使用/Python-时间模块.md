Python 时间模块<br />Python内置一个时间模块`datetime`，提供关于时间的表达。记录时间无所不在，日志文件，程序运行起始时间和时长，销量预测的特征等等，都能看到时间的身影。
<a name="hDzLp"></a>
## 1、核心逻辑
`datetime`模块提供日期和时间各自分类的对象，日期处理相关的对象`date`，时间处理相关的`time`，日期和时间的完整结合对象`datetime`.<br />日期和时间的加减操作得到`timedelta`对象.<br />此时此刻 2020-8-28 21:45，这个时间是本地时间，很明显纽约时间肯定不是此值，柏林时间也肯定不是这个值。Python为支持不同地区的时间表达，特意抽象出`tzinfo`对象，并有一个默认实现对象.<br />以上就是`datetime`模块的几个核心对象以及对应的现实意义。
<a name="kr490"></a>
## 2、`date`、`time`和`datetime`对象
下面介绍最基本3个对象的最基本用法。首先，从`datetime`模块导入3个对象
```python
In [1]: from datetime import date,time,datetime
```
构造一个日期`date`实例，2020年9月1日：
```python
In [2]: date(2020,9,1)                                                          
Out[2]: datetime.date(2020, 9, 1)
```
构造一个时间`time`实例，10点10分0秒：
```python
In [3]: time(10,10,0)                                                           
Out[3]: datetime.time(10, 10)
```
构造一个日期+时间的完整`datetime`实例，2020年9月1日 10点10分0秒：
```python
In [4]: datetime(2020,9,1,10,10,10)                                             
Out[4]: datetime.datetime(2020, 9, 1, 10, 10, 10)
```
自己构造时间没什么意义，更有意义的是打印当前时间，比如此时程序启动打印下时间，如果程序可能运行十几天，很明显使用日期+时间的完整`datetime`实例。<br />此方法归属于`datetime`类上的方法，所以无须构造`datetime`实例，直接如下：
```python
datetime.today() # datetime类的today方法
Out[5]: datetime.datetime(2020, 8, 28, 22, 0, 47, 439509)
```
打印结果显示年月日时分秒毫秒 还可以使用类方法`now`:
```python
In [6]: datetime.now()                                                          
Out[6]: datetime.datetime(2020, 8, 28, 22, 1, 28, 737166)
```
直接打印当前时间，返回日期+时间的字符串结果：
```python
In [7]: print(datetime.now())                           
2020-08-28 22:02:57.217572
```
如果不想显示毫秒，这就涉及到日期+时间的打印格式化问题。使用`datetime`类方法`strftime`（string format time），用法如下：
```python
In [8]: datetime.strftime(datetime.now(),'%Y-%m-%d %H:%M:%S')                   
Out[8]: '2020-08-28 22:06:20'
```
这就涉及到打印格式化字符，常用的几个：

| **字符** | **意义** |
| --- | --- |
| %Y | 完整的年份 |
| %y | 去掉世纪的年份 |
| %m | 月份（01 - 12） |
| %d | 一个月中的第几天（01 - 31） |
| %H | 一天中的第几个小时（24小时制，00 - 23） |
| %M | 分钟数（00 - 59） |
| %S | 秒（01 - 61） |
| %w | 一个星期中的第几天（0 - 6，0是星期天） |
| %Z | 时区的名字（如果不存在为空字符） |

如果读入一个时间列，此时type为str，为了对此作时间运算，需要将其转化为datetime，使用`strptime`(string parse time)，它是`datetime`的类方法：
```python
In [11]: datetime.strptime('2020-08-28 22:06:20','%Y-%m-%d %H:%M:%S')           
Out[11]: datetime.datetime(2020, 8, 28, 22, 6, 20)
```
字符型日期+时间要想正确转化为`datetime`对象，字符串和格式必须要匹配，否则会抛错：
```python
In [13]: datetime.strptime('2020-08-28 22:06:20',\
                           '%Y/%m/%d %H:%M:%S') 
ValueError: time data '2020-08-28 22:06:20' 
does not match format '%Y/%m/%d %H:%M:%S'
```
<a name="YmtTN"></a>
### `datatime`
<a name="TQmzH"></a>
#### `datetime.now()`  &  `datetime.today()`
```python
import datetime
print(datetime.datetime.now())
 
# datetime.today()
#2020-09-21 23:13:05.245157
print(datetime.datetime.today())
#2020-09-21 23:13:57.062886
```
<a name="q8LB5"></a>
#### `datetime.strptime()`
```python
print(datetime.datetime.strptime('2020/08/09',format('%Y/%m/%d')))
#2020-08-09 00:00:00
print(datetime.datetime.strptime('20-08-09',format('%y-%m-%d')))
#2020-08-09 00:00:00
```
<a name="twiTA"></a>
#### `datetime.strftime()`
接收以时间元组，并返回以可读字符串表示的当地时间，格式由参数format决定。
```python
dt = datetime.datetime.now()  
print("======================")
print('时间：(%Y-%m-%d %H:%M:%S %f): ',dt.strftime( '%Y-%m-%d %H:%M:%S %f' ) )
print('时间：(%Y-%m-%d %H:%M:%S %p): ' , dt.strftime( '%y-%m-%d %I:%M:%S %p' ) )
print('星期缩写%%a: %s '  % dt.strftime( '%a' ))  
print('星期全拼%%A: %s '  % dt.strftime( '%A' ))  
print('月份缩写%%b: %s '  % dt.strftime( '%b' ) )
print('月份全批%%B: %s '  % dt.strftime( '%B' ))  
print('日期时间%%c: %s '  % dt.strftime( '%c' ) )
print('今天是这周的第%s天 '  % dt.strftime( '%w' ) )
print( '今天是今年的第%s天 '  % dt.strftime( '%j' ))  
print('今周是今年的第%s周 '  % dt.strftime( '%U' ) )
print('今天是当月的第%s天 '  % dt.strftime( '%d' ))
 
'''
======================
时间：(%Y-%m-%d %H:%M:%S %f): 2020-09-21 23:30:51 573645
时间：(%Y-%m-%d %H:%M:%S %p): 20-09-21 11:30:51 PM
星期缩写%a: Mon
星期全拼%A: Monday
月份缩写%b: Sep
月份全批%B: September
日期时间%c: Mon Sep 21 23:30:51 2020
今天是这周的第1天
今天是今年的第265天
今周是今年的第38周
今天是当月的第21天
'''
```
<a name="yorKU"></a>
### 日期函数`time`模块函数
表示日期时间常用三种形式：

1. 秒为单位的浮点数
2. `struct_time` 元组
3. 时间字符串
```python
import time;  # 引入time模块
ticks = time.time()
print ("当前时间戳为:", ticks)
 
当前时间戳为: 1600704429.9781017
```
<a name="EIVhS"></a>
#### 秒为单位的浮点数 转换为 时间元组
```python
tm_struct = time.localtime(ticks)
print(tm_struct)
 
time.struct_time(tm_year=2020, tm_mon=9, tm_mday=22, tm_hour=0, tm_min=8, tm_sec=2, tm_wday=1, tm_yday=266, tm_isdst=0)
```
有时需要得到格林威治时间，那么可以用`**time.gmtime()**`** 函数**：
```python
#格林威治时间
print(time.gmtime(ticks))
 
 
time.struct_time(tm_year=2020, tm_mon=9, tm_mday=21, tm_hour=16, tm_min=9, tm_sec=18, tm_wday=0, tm_yday=265, tm_isdst=0)
```
<a name="6b61b1c1"></a>
#### 时间元组转换为秒为单位的浮点数
```python
print(time.mktime(tm_struct))
 
1600704621.0
```
<a name="lFvwp"></a>
#### 时间元组转换为时间字符串
转换方法有很多，最简单的是 `**asctime()**`**:**
```python
time_str = time.asctime( tm_struct )
print(time_str)
 
"""
Tue Sep 22 00:12:14 2020
"""
```
如果要精确控制格式，可以使用 `**strftime()**`**:**
```python
tm_1=time.strftime("%Y-%m-%d %H:%M:%S", tm_struct)
tm_2=time.strftime("%a %b %d %H:%M:%S %Y", tm_struct)
print(tm_1)
print(tm_2)
 
"""
2020-09-22 00:14:13
Tue Sep 22 00:14:13 2020
"""
```
<a name="yseps"></a>
#### 时间字符串转换为时间元组
函数 `**time.strptime()**` 可以完成这个转换，下面是个例子
```python
tm2_struct = time.strptime('2020-09-22 01:34:50', "%Y-%m-%d %H:%M:%S")
print(tm2_struct)
 
"""
time.struct_time(tm_year=2020, tm_mon=9, tm_mday=22, tm_hour=1, tm_min=34, tm_sec=50, tm_wday=1, tm_yday=266, tm_isdst=-1)
"""
```
<a name="Z7Mwv"></a>
#### 秒为单位的浮点数转换为时间字符串
`**time.ctime()**` 类似于 `**time.asctime()**`, 请看下面的例子：
```python
tm_3=time.ctime(ticks)
print(tm_3)
 
"""
Tue Sep 22 00:17:54 2020
 
"""
#如果想精确控制输出的字符串，目前没有现成的函数。估计只能先转成元组，然后在格式化成字符串。
#转换函数：
 
def time2str(ticks, fmt='%a %b %d %H:%M:%S %Y'):
     tm = time.localtime(ticks)
     return time.strftime(fmt, tm)
```
<a name="l0alM"></a>
#### 时间字符串转换为秒为单位的浮点数
```python
def str2time(str, fmt='%a %b %d %H:%M:%S %Y')：
     tm = time.strptime(str, fmt)
     return time.mktime(tm)
 
# 这里也是参考函数
```
<a name="36e60a67"></a>
## 3、基本运算
有时需要求偏离某个时间的时间，`timedelta`对象能满足此需求。<br />比如，求当前时间的前12小时的日期+时间。<br />首先，导入`timedelta`类：
```python
In [15]: from datetime import timedelta
```
直接使用当前时间减去`timedelta`表示的12小时长度，注意第一个参数的含义为days，所以除以 24：
```python
In [16]: datetime.now() - timedelta(12/24)                                      
Out[16]: datetime.datetime(2020, 8, 28, 10, 22, 44, 287246)
```
由上面这个用法，可以总结为：<br />`datetime1 - timedelta1 = datetime2`<br />所以 `datetime1 - datetime2 = timedelta1`，故两个时间相减得到`timedelta`类型的实例。<br />除此之外，还有一个小方法，可能会用到，就是datetime类上的`combine`方法，它能组合date实例和time实例为`datetime`实例，如下所示：
```python
In [17]: datetime.combine(date(2020,9,1),time(10,10,0))                         
Out[17]: datetime.datetime(2020, 9, 1, 10, 10)
```
<a name="Tg1IK"></a>
## 4、关于`tzinfo`
为了更好统一全球时间，世界规定了一个UTC时间，即全球统一时间，比如假设与之相比北京时间比它早8小时，曼谷比它早7小时等。<br />比如打印当前时间时，
```python
In [6]: print(datetime.now())
2020-08-28 22:33:35.393709
```
以上显示的这个时间，其实并不完整，它是所在地的时间，但是其他国家的开发者看到这个时间时，或许以为是UTC标准下的时间。若是这样解读，显然会和实际有一个时差问题。<br />在打印格式化时添加时区信息可以吗，实验一下：
```python
In [19]: datetime.strftime(datetime.now(),\
                           '%Y-%m-%d %H:%M:%S %Z')               
Out[19]: '2020-08-28 22:39:44 '
```
时区信息为空，所以没能解决问题。之所以时区信息会为空，是因为`datetime.now()`时未给定`tzinfo`值。<br />所以，需要自己重新定义一个`tzinfo`，即实现一个`tzinfo`对象。<br />此类`BJinfo`继承`tzinfo`，然后实现其中的3个方法：
```python
from datetime import tzinfo
class BJinfo(tzinfo):
    """BJinfo"""
    def utcoffset(self, dt):
        return timedelta(hours=8)
    def tzname(self, dt):
        return "UTC 8"
    def dst(self, dt):
        return timedelta(hours=8)
```
此时再打印当前时间时，赋上`tzinfo`值:
```python
nowt = datetime.now(tz=BJinfo())
In [32]: In [6]: print(nowt)                               
2020-08-28 22:52:20.328446+08:00
```
再格式化打印时区信息：
```python
In [36]: datetime.strftime(nowt,\ 
    ...:     '%Y-%m-%d %H:%M:%S %Z')                     
Out[36]: '2020-08-28 22:52:20 UTC 8'
```
透过时区信息`BJinfo` 定义的三个方法，便能确认时间`2020-08-28 22:52:20`是比UTC快8个小时的时区下，所对应的一个时间。
<a name="wnDsb"></a>
## 5、时间日期的应用场景
<a name="0pMOn"></a>
### 1.简介
在编写代码时，往往涉及时间、日期、时间戳的相互转换。
<a name="qmKzP"></a>
### 2.示例
```python
# 引入模块
import time, datetime
```
<a name="rM9Vw"></a>
#### 2.1 `str`类型的日期转换为时间戳
```python
# 字符类型的时间
 
testStr = '2020-09-21 23:26:33'
 
# 转为 时间数组
timeArray = time.strptime(testStr, "%Y-%m-%d %H:%M:%S")
print(timeArray)
 
# timeArray可以调用tm_year等
print(timeArray.tm_year)  # 2020
 
# 转为时间戳
timeStamp = int(time.mktime(timeArray))
print(timeStamp)  # 1600701993
 
# 结果如下
time.struct_time(tm_year=2020, tm_mon=9, tm_mday=21, tm_hour=23, tm_min=26, tm_sec=33, tm_wday=0, tm_yday=265, tm_isdst=-1)
2020
1600701993
```
<a name="LQ3CG"></a>
#### 2.2 更改`str`类型日期的显示格式
```python
testStr1 = "2020-09-21 23:40:00"
# 转为数组
timeArray = time.strptime(testStr1, "%Y-%m-%d %H:%M:%S")
# 转为其它显示格式
otherStyleTime = time.strftime("%Y/%m/%d %H:%M:%S", timeArray)
print (otherStyleTime)
 
tss3 = "2020/09/21 23:40:00"
timeArray = time.strptime(tss3, "%Y/%m/%d %H:%M:%S")
otherStyleTime = time.strftime("%Y-%m-%d %H:%M:%S", timeArray)
print (otherStyleTime)
 
 
"""
-----------------------
2020/09/21 23:40:00
2020-09-21 23:40:00
======================
 
"""
```
<a name="ugqmF"></a>
#### 2.3 时间戳转换为指定格式的日期
```python
# 使用time
timeStamp = 1600701993
timeArray = time.localtime(timeStamp)
otherStyleTime = time.strftime("%Y--%m--%d %H:%M:%S", timeArray)
print(otherStyleTime)  # 2020--09--21 23:26:33
# 使用datetime
timeStamp = 1600101993
dateArray = datetime.datetime.fromtimestamp(timeStamp)
otherStyleTime = dateArray.strftime("%Y--%m--%d %H:%M:%S")
print(otherStyleTime)  # 2020--09--15 00:46:33
# 使用datetime，指定utc时间，相差8小时
timeStamp = 1600201993
dateArray = datetime.datetime.utcfromtimestamp(timeStamp)
otherStyleTime = dateArray.strftime("%Y--%m--%d %H:%M:%S")
print(otherStyleTime)  # 2020--09--15 20:33:13
```
<a name="y7Kl6"></a>
#### 2.4 获取当前时间并且用指定格式显示
```python
print("+++++++++++++++++++++++++++++++++++++++++++")
# time获取当前时间戳
now = int(time.time())     # 1533952277
timeArray = time.localtime(now)
print (timeArray)
otherStyleTime = time.strftime("%Y--%m--%d %H:%M:%S", timeArray)
print (otherStyleTime)
 
# 结果如下
"""
time.struct_time(tm_year=2020, tm_mon=9, tm_mday=21, tm_hour=23, tm_min=45, tm_sec=53, tm_wday=0, tm_yday=265, tm_isdst=0)
2020--09--21 23:45:53
 
"""
 
 
# datetime获取当前时间，数组格式
now = datetime.datetime.now()
print (now)
otherStyleTime = now.strftime("%Y--%m--%d %H:%M:%S")
print (otherStyleTime)
 
# 结果如下：
 
"""
2020-09-21 23:45:53.543487
2020--09--21 23:45:53
"""
```

