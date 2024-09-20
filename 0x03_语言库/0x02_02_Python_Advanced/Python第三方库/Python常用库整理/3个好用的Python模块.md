Python
<a name="wtuG7"></a>
### Psutil
Python当中的`Psutil`模块是个跨平台库，它能够轻松获取系统运行的进程和系统利用率，包括CPU、内存、磁盘、网络等信息，它的安装也非常的简单，命令行
```bash
pip install psutil
```
暂时只罗列几个常用的方法，例如想要查看一下CPU的利用率
```python
psutil.cpu_percent()
```
返回的结果表示的是当前系统范围的CPU利用率百分比，如果要查看系统中CPU的个数，代码如下
```python
## 逻辑CPU的个数
psutil.cpu_count()

## 物理CPU的个数
psutil.cpu_count(logical=False)
```
又或者想要查看一下系统中的物理内存，代码如下
```python
## 剩余的物理内存
free = str(round(psutil.virtual_memory().free / (1024.0 * 1024.0 * 1024.0), 2))

## 物理内存总共有
total = str(round(psutil.virtual_memory().total / (1024.0 * 1024.0 * 1024.0), 2))
```
而如果想要查看单个磁盘的信息，就直接调用`disk_usage()`方法
```python
print(psutil.disk_usage('C:\\'))
```
而去获取所有磁盘的信息，调用的则是`disk_partitions()`方法
```python
print(psutil.disk_partitions())
```
另外也还能够获取到系统的启动时间
```python
from datetime import datetime
print(u"系统启动时间: %s" % datetime.fromtimestamp(psutil.boot_time()).strftime("%Y-%m-%d %H:%M:%S"))
```
<a name="ilwtu"></a>
### Pendulum
一般用`datatime`模块来处理日期、时间等数据，但是不得不说在于`datatime`模块也有自身的一些限制，例如在处理时区时就会显得有些不足，这次来看一下`Pendulum`模块<br />首先用pip命令行来进行安装
```bash
pip install pendulum
```
`pendulum`模块最令人印象深刻的功能是时区，例如想要知道“巴黎”此时的时间，可以这么来做
```python
now_in_paris = pendulum.now('Europe/Paris')
print(now_in_paris)
```
output
```python
2022-01-22T14:59:06.484816+01:00
```
还可以知道当天的日期
```python
d1 = pendulum.yesterday() # 昨天
d2 = pendulum.today() # 今天
d3 = pendulum.tomorrow() # 明天
```
output
```python
2022-01-21T00:00:00+08:00 # 昨天的日期
2022-01-22T00:00:00+08:00 # 今天
2022-01-23T00:00:00+08:00 # 明天
```
还可以在时间的数据上进行加、减，调用的是`add`和`subtract`方法
```python
dt = pendulum.datetime(2022, 1, 22)
dt_years_add = dt.add(years=5)
print(dt_years_add)
dt_years_subtract = dt.subtract(years=1)
print(dt_years_subtract)
dt_month_add = dt.add(months=60)
print(dt_month_add)
dt_month_subtract = dt.subtract(months=60)
print(dt_month_subtract)
```
output
```python
2027-01-22T00:00:00+00:00
2021-01-22T00:00:00+00:00
2027-01-22T00:00:00+00:00
2017-01-22T00:00:00+00:00
```
要是希望将时间数据转换成字符串，就可以这么来做，代码如下
```python
dt = pendulum.datetime(2022, 1, 23, 15, 16, 10)
```
要是需要的是前缀的日期字符串，则可以这么来做
```python
dt.to_date_string()
```
output
```python
2022-01-23
```
而要是需要的是后缀的时间字符串，则可以这么来做
```python
dt.to_time_string()
```
output
```python
15:16:10
```
当然有时候日期和时间都需要，代码如下
```python
dt.to_datetime_string()
```
output
```python
2022-01-23 15:16:10
```
或者是
```python
dt.to_day_datetime_string()
```
output
```python
Sun, Jan 23, 2022 3:16 PM
```
当然该模块还有其他很多强大的功能，具体的大家可以去看它的文档，最后要说的是其人性化时间的输出功能。<br />如果平时用搜素引擎的话，就会看到有很多内容的时间被标成了“1天前”、“1周后”等等，这个在`pendulum`模块当中也能够轻而易举的实现
```python
print(pendulum.now().subtract(days=1).diff_for_humans())
## '1 day ago'

print(pendulum.now().diff_for_humans(pendulum.now().subtract(years=1)))
## '1 year after'

print(pendulum.now().subtract(days=24).diff_for_humans())
## '3 weeks ago'
```
可能有些人要是英文看不懂的话，也可以切换到中文，如下
```python
print(pendulum.now().subtract(days=14).diff_for_humans())
## '2周前'

print(pendulum.now().add(seconds=5).diff_for_humans())
## '5秒钟后'
```
<a name="ARy64"></a>
### Pyfiglet
pyfiglet是一个专门用来生成艺术字的模块，并且支持有多种艺术字的字体，来看一下下面这个例子
```python
result = pyfiglet.figlet_format("Python", font="larry3d")
print(result)
```
output
```python
 ____           __    __                         
/\  _`\        /\ \__/\ \                        
\ \ \L\ \__  __\ \ ,_\ \ \___     ___     ___    
 \ \ ,__/\ \/\ \\ \ \/\ \  _ `\  / __`\ /' _ `\  
  \ \ \/\ \ \_\ \\ \ \_\ \ \ \ \/\ \L\ \/\ \/\ \ 
   \ \_\ \/`____ \\ \__\\ \_\ \_\ \____/\ \_\ \_\
    \/_/  `/___/> \\/__/ \/_/\/_/\/___/  \/_/\/_/
             /\___/                              
             \/__/                               
```
要是不喜欢上面的字体，可以通过下面的代码
```python
pyfiglet.FigletFont.getFonts()
```
在输出的所有字体当中任选一个来进行艺术字的塑造
