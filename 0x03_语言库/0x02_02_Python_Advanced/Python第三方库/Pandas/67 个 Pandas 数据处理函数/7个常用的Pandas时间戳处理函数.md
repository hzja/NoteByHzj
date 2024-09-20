PythonPandas<br />在零售、经济和金融等行业，数据总是由于货币和销售而不断变化，生成的所有数据都高度依赖于时间。 如果这些数据没有时间戳或标记，实际上很难管理所有收集的数据。Python 程序允许使用 NumPy timedelta64 和 datetime64 来操作和检索时间序列数据。 sklern库中也提供时间序列功能，但 pandas 提供了更多且好用的函数。<br />Pandas 库中有四个与时间相关的概念

- **日期时间**：日期时间表示特定日期和时间及其各自的时区。它在 pandas 中的数据类型是 `datetime64[ns]` 或 `datetime64[ns, tz]`。
- **时间增量**：时间增量表示时间差异，它们可以是不同的单位。示例："天、小时、减号"等。换句话说，它们是日期时间的子类。
- **时间跨度**：时间跨度被称为固定周期内的相关频率。时间跨度的数据类型是 period[freq]。
- **日期偏移**：日期偏移有助于从当前日期计算选定日期，日期偏移量在 pandas 中没有特定的数据类型。

时间序列分析至关重要，因为它们可以帮助了解随着时间的推移影响趋势或系统模式的因素。在数据可视化的帮助下，分析并做出后续决策。<br />现在来看几个使用这些函数的例子。
<a name="t0pVx"></a>
## 1、查找特定日期的某一天的名称
```python
import pandas as pd 
day = pd.Timestamp('2021/1/5') 
day.day_name()
```
```python
'Tuesday'
```
上面的程序是显示特定日期的名称。第一步是导入 panda 的并使用 Timestamp 和 day_name 函数。"Timestamp"功能用于输入日期，"day_name"功能用于显示指定日期的名称。
<a name="WxX1O"></a>
## 2、执行算术计算
```python
import pandas as pd 
 
day = pd.Timestamp('2021/1/5') 
day1 = day + pd.Timedelta("3 day") 
day1.day_name() 
 
day2 = day1 + pd.offsets.BDay() 
day2.day_name()
```
```python
'Monday'
```
在第一个代码中，显示三天后日期名称。"Timedelta"功能允许输入任何天单位（天、小时、分钟、秒）的时差。<br />在第二个代码中，使用"offsets.BDay()"函数来显示下一个工作日。换句话说，这意味着在星期五之后，下一个工作日是星期一。
<a name="exdAX"></a>
## 3、使用时区信息来操作转换日期时间
<a name="U3nVj"></a>
### 获取时区的信息
```python
import pandas as pd 
import numpy as np 
from datetime import datetime 
 
dat_ran = dat_ran.tz_localize("UTC") 
dat_ran
```
```python
DatetimeIndex(
['2021-01-01 00:00:00+00:00', '2021-01-01 00:01:00+00:00',
 '2021-01-01 00:02:00+00:00', '2021-01-01 00:03:00+00:00',
 '2021-01-01 00:04:00+00:00', '2021-01-01 00:05:00+00:00',
 '2021-01-01 00:06:00+00:00', '2021-01-01 00:07:00+00:00',
 '2021-01-01 00:08:00+00:00', '2021-01-01 00:09:00+00:00',
 ...
 '2021-01-04 23:51:00+00:00', '2021-01-04 23:52:00+00:00',
 '2021-01-04 23:53:00+00:00', '2021-01-04 23:54:00+00:00',
 '2021-01-04 23:55:00+00:00', '2021-01-04 23:56:00+00:00',
 '2021-01-04 23:57:00+00:00', '2021-01-04 23:58:00+00:00',
 '2021-01-04 23:59:00+00:00', '2021-01-05 00:00:00+00:00'],             
dtype='datetime64[ns, UTC]',
length=5761, freq='T')
```
<a name="Qre0a"></a>
### 转换为美国时区
```python
dat_ran.tz_convert("US/Pacific")
```
```python
DatetimeIndex(
['2020-12-31 16:00:00-08:00', '2020-12-31 16:01:00-08:00',
 '2020-12-31 16:02:00-08:00', '2020-12-31 16:03:00-08:00',
 '2020-12-31 16:04:00-08:00', '2020-12-31 16:05:00-08:00',
 '2020-12-31 16:06:00-08:00', '2020-12-31 16:07:00-08:00',
 '2020-12-31 16:08:00-08:00', '2020-12-31 16:09:00-08:00',
 ...
 '2021-01-04 15:51:00-08:00', '2021-01-04 15:52:00-08:00',
 '2021-01-04 15:53:00-08:00', '2021-01-04 15:54:00-08:00',
 '2021-01-04 15:55:00-08:00', '2021-01-04 15:56:00-08:00',
 '2021-01-04 15:57:00-08:00', '2021-01-04 15:58:00-08:00',
 '2021-01-04 15:59:00-08:00', '2021-01-04 16:00:00-08:00'],
dtype='datetime64[ns, US/Pacific]', 
length=5761, freq='T')
```
代码的目标是更改日期的时区。首先需要找到当前时区。这是"`tz_localize()`"函数完成的。现在知道当前时区是"UTC"。使用"`tz_convert()`"函数，转换为美国/太平洋时区。
<a name="XMyHE"></a>
## 4、使用日期时间戳
```python
import pandas as pd 
import numpy as np 
from datetime import datetime 
dat_ran = pd.date_range(start = '1/1/2021', end = '1/5/2021', freq = 'Min') 
print(type(dat_ran[110]))
```
```python
<class 'pandas._libs.tslibs.timestamps.
Timestamp'>
```
<a name="hK6pz"></a>
## 5、创建日期系列
```python
import pandas as pd 
import numpy as np 
from datetime import datetime 
dat_ran = pd.date_range(start = '1/1/2021', end = '1/5/2021', freq = 'Min') 
print(dat_ran)
```
```python
DatetimeIndex(
['2021-01-01 00:00:00', '2021-01-01 00:01:00',
 '2021-01-01 00:02:00', '2021-01-01 00:03:00',
 '2021-01-01 00:04:00', '2021-01-01 00:05:00',
 '2021-01-01 00:06:00', '2021-01-01 00:07:00',
 '2021-01-01 00:08:00', '2021-01-01 00:09:00',
 ...
 '2021-01-04 23:51:00', '2021-01-04 23:52:00',
 '2021-01-04 23:53:00', '2021-01-04 23:54:00',
 '2021-01-04 23:55:00', '2021-01-04 23:56:00',
 '2021-01-04 23:57:00', '2021-01-04 23:58:00',
 '2021-01-04 23:59:00', '2021-01-05 00:00:00'],
dtype='datetime64[ns]', 
length=5761, freq='T')
```
上面的代码生成了一个日期系列的范围。使用"date_range"函数，输入开始和结束日期，可以获得该范围内的日期。
<a name="VQZcB"></a>
## 6、操作日期序列
```python
import pandas as pd 
from datetime import datetime 
import numpy as np 
 
dat_ran = pd.date_range(start ='1/1/2019', end ='1/08/2019',freq ='Min') 
df = pd.DataFrame(dat_ran, columns =['date']) 
df['data'] = np.random.randint(0, 100, size =(len(dat_ran))) 
print(df.head(5))
```
```python
                 date  data
0 2019-01-01 00:00:00    68
1 2019-01-01 00:01:00    77
2 2019-01-01 00:02:00    78
3 2019-01-01 00:03:00    64
4 2019-01-01 00:04:00    42
```
在上面的代码中，使用"DataFrame"函数将字符串类型转换为dataframe。最后"`np.random.randint()`"函数是随机生成一些假定的数据。
<a name="QQY0i"></a>
## 7、使用时间戳数据对数据进行切片
```python
import pandas as pd 
from datetime import datetime 
import numpy as np 
dat_ran = pd.date_range(start ='1/1/2019', end ='1/08/2019', freq ='Min') 

df = pd.DataFrame(dat_ran, columns =['date']) 
df['data'] = np.random.randint(0, 100, size =(len(dat_ran))) 
string_data = [str(x) for x in dat_ran] 

print(string_data[1:5])
```
```python
['2019-01-01 00:01:00', 
 '2019-01-01 00:02:00', 
 '2019-01-01 00:03:00', 
 '2019-01-01 00:04:00']
```
上面代码是是第6条的的延续。在创建dataframe并将其映射到随机数后，对列表进行切片。
