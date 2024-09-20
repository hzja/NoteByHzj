Python 时间<br />DeLorean 是一个 Python 的第三方模块，基于 pytz 和 dateutil 开发的，用于处理 Python 中日期时间的格式转换。<br />由于时间转换是一个足够微妙的问题，DeLorean 希望为移位、操作和生成日期时间提供一种更干净、更省事的解决方案。比如，实例化字符串形式的时间对象，Delorean 只需要 parse 指定字符串，不需要声明其格式就可以进行转换。
<a name="NLV9S"></a>
## 1、准备
开始之前，要确保 Python 和 pip 已经成功安装在电脑上。<br />请选择以下任一种方式输入命令安装依赖：<br />1. Windows 环境 打开 Cmd (开始-运行-CMD)。<br />2. MacOS 环境 打开 Terminal (command+ 空格输入 Terminal)。<br />3. 如果用的是 VSCode 编辑器 或 Pycharm，可以直接使用界面下方的Terminal.
```bash
pip install Delorean
```
<a name="W5ikY"></a>
## 2、Delorean 基础使用
轻松获取当前时间：
```python
from delorean import Delorean

d = Delorean()
print(d)
# Delorean(datetime=datetime.datetime(2021, 10, 6, 9, 5, 57, 611589), timezone='UTC')
```
将 datetime 格式的时间转化为 Delorean：
```python
import datetime
from delorean import Delorean

d = Delorean()
print(d)
d = Delorean(datetime=datetime.datetime(2018, 5, 10, 8, 52, 23, 560811), timezone='UTC')
# 这里默认的是UTC时间
print(d)
# Delorean(datetime=datetime.datetime(2021, 10, 6, 9, 5, 57, 611589), timezone='UTC')
# Delorean(datetime=datetime.datetime(2018, 5, 10, 8, 52, 23, 560811), timezone='UTC')
```
转换为国内时区：
```python
import datetime
from delorean import Delorean

d = Delorean(datetime=datetime.datetime(2018, 5, 10, 8, 52, 23, 560811), timezone='UTC')
d = d.shift("Asia/Shanghai")
print(d)
# Delorean(datetime=datetime.datetime(2018, 5, 10, 16, 52, 23, 560811), timezone='Asia/Shanghai')
```
输出为 datetime、date 也不在话下:
```python
import datetime
from delorean import Delorean

d = Delorean(datetime=datetime.datetime(2018, 5, 10, 8, 52, 23, 560811), timezone='UTC')
d = d.shift("Asia/Shanghai")
print(d.datetime)
print(d.date)
# 2018-05-10 16:52:23.560811+08:00
# 2018-05-10
```
查看无时区时间及时间戳：
```python
import datetime
from delorean import Delorean

d = Delorean(datetime=datetime.datetime(2018, 5, 10, 8, 52, 23, 560811), timezone='UTC')
d = d.shift("Asia/Shanghai")
print(d.epoch)
print(d.naive)
# 1525942343.560811
# 2018-05-10 08:52:23.560811
```
用 unix 时间戳初始化 Delorean：
```python
from delorean import epoch
d = epoch(1357971038.102223).shift("Asia/Shanghai")
print(d)
# Delorean(datetime=datetime.datetime(2013, 1, 12, 14, 10, 38, 102223), timezone='Asia/Shanghai')
```
Delorean 支持 timedelta 的时间加减法。Delorean 可以使用 timedelta 进行加减，得到一个 Delorean 对象：
```python
import datetime
from delorean import Delorean

d = Delorean(datetime=datetime.datetime(2018, 5, 10, 8, 52, 23, 560811), timezone='UTC')
d = d.shift("Asia/Shanghai")
print(d)
d2 = d + datetime.timedelta(hours=2)
print(d2)
d3 = d - datetime.timedelta(hours=3)
print(d3)
# Delorean(datetime=datetime.datetime(2018, 5, 10, 16, 52, 23, 560811), timezone='Asia/Shanghai')
# Delorean(datetime=datetime.datetime(2018, 5, 10, 18, 52, 23, 560811), timezone='Asia/Shanghai')
# Delorean(datetime=datetime.datetime(2018, 5, 10, 13, 52, 23, 560811), timezone='Asia/Shanghai')
```
<a name="xgAS7"></a>
## 3、Delorean 高级使用
通常情况下不关心有多少微妙或者多少秒，因此 Delorean 提供了非常方便的过滤方式：
```python
from delorean import Delorean

d = Delorean()
print(d)
# Delorean(datetime=datetime.datetime(2019, 3, 14, 4, 0, 50, 597357), timezone='UTC')
d.truncate('second')
# Delorean(datetime=datetime.datetime(2019, 3, 14, 4, 0, 50), timezone='UTC')
d.truncate('hour')
# Delorean(datetime=datetime.datetime(2019, 3, 14, 4, 0), timezone='UTC')
d.truncate('month')
# Delorean(datetime=datetime.datetime(2019, 3, 1, 0, 0), timezone='UTC')
d.truncate('year')
# Delorean(datetime=datetime.datetime(2019, 1, 1, 0, 0), timezone='UTC')
```
另外，datetime 格式的字符串处理的时候转换需要标明各种各样的格式，在 Delorean 直接 parse 就可以了：
```python
from delorean import parse
parse("2011/01/01 00:00:00 -0700")
# Delorean(datetime=datetime.datetime(2011, 1, 1, 0, 0), timezone=pytz.FixedOffset(-420))
parse("2018-05-06")
# Delorean(datetime=datetime.datetime(2018, 6, 5, 0, 0), timezone='UTC')
```
