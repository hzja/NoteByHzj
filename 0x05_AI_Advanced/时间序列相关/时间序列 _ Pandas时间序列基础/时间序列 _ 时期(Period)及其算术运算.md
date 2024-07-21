时间序列<br />首先导入需要用到的包
```python
import pandas as pd
import numpy as np
```
| 时间类型 | Python中的类型 |
| --- | --- |
| 时间戳 | timestamp |
| 时间间隔 | timedelta |
| 时期 | period |

<a name="SJSUA"></a>
## `pd.Period()`构造时期
时期（period）表示的是时间区间，比如数日、数月、数季、数年等。
```python
>>> import pandas as pd
# Period类所表示的就是这种数据类型，其构造函数需要用到一个字符串或整数
>>> p = pd.Period('2010',freq = 'A-DEC') 
>>> p 
# 这个Period对象表示的是从2010年1月1日到2010年12月31日之间的整段时间
Period('2010', 'A-DEC')

# 只需对Period对象加上或减去一个整数即可达到根据其频率进行位移的
>>> p + 5 
Period('2015', 'A-DEC')
>>> p - 4
Period('2006', 'A-DEC')
# 如果两个Period对象拥有相同频率，则它们的差就是它们之间的单位数量
>>> pd.Period('2015',freq='A-DEC') - p
5
```
<a name="uhu27"></a>
## `pd.period_range()`构造时期范围
<a name="oGfyC"></a>
### `period_range`函数可用于创建规则的时期范围：
```python
>>> rng = pd.period_range('2012-1-1','2012-6-30',freq = 'M')
>>> rng
PeriodIndex(['2012-01', '2012-02', '2012-03', '2012-04', '2012-05', '2012-06'], dtype='period[M]', freq='M')

# PeriodIndex类保存了一组Period，它可以在任何pandas数据结构中被用作轴索引
>>> ped = pd.Series(np.random.randn(len(rng)), index = rng)
>>> ped
2012-01   -2.411963
2012-02    0.893100
2012-03    1.438209
2012-04    0.193747
2012-05   -1.469531
2012-06    0.943145
Freq: M, dtype: float64
>>> ped.index
PeriodIndex(['2012-01', '2012-02', '2012-03', '2012-04', '2012-05', '2012-06'], dtype='period[M]', freq='M')
```
<a name="O0Mad"></a>
### 使用PeriodIndex类将一个字符串数组转换为一段时期
```python
# PeriodIndex类的构造函数允许直接使用一组字符串表示一段时期
>>> values =['2010Q1','2010Q2','2010Q3']
>>> index = pd.PeriodIndex(values,freq = 'Q-DEC')
>>> index
PeriodIndex(['2010Q1', '2010Q2', '2010Q3'], dtype='period[Q-DEC]', freq='Q-DEC')
```
<a name="NRkhW"></a>
## 时间的频率转换
<a name="MMJqM"></a>
### Period和PeriodIndex对象都可以通过其asfreq方法被转换成别的频率
一个年度时期，希望将其转换为当年年初或年末的一个月度时期
```python
>>> p = pd.Period('2012',freq = 'A-DEC')
>>> p
Period('2012', 'A-DEC')
>>> p.asfreq('M',how = 'start')
Period('2012-01', 'M')
>>> p.asfreq('Q','End')
Period('2012Q4', 'Q-DEC')
```
可以将`Period('2012','A-DEC')`看做一个被划分为多个月度时期的时间段中的游标。下图对此进行了说明在将高频率转换为低频率时，超时期（superperiod）是由子时期（subperiod）所属的位置决定的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652257492312-fb084d7e-2992-442b-9eca-54aae3408196.png#clientId=uf289bd6c-35ff-4&from=paste&id=uf263dfe3&originHeight=362&originWidth=1018&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u351a9cc0-8e95-4af2-acd3-6eeb43fd40f&title=)<br />在A-JUN频率中，月份"2007年8月"实际上是属于周期"2008年"的：
```python
>>> p = pd.Period('Aug-2007', freq = 'M')
>>> p
Period('2007-08', 'M')
>>> p.asfreq('A-JUN')
Period('2008', 'A-JUN')
```
完整的PeriodIndex或TimeSeries的频率转换方式相同
```python
>>> rng = pd.period_range('2001','2008', freq = 'A-DEC')
>>> ts = pd.Series(np.random.randn(len(rng)), index = rng)
>>> ts
2001    0.859500
2002   -1.213256
2003   -0.591023
2004   -0.393317
2005    1.015904
2006    0.278310
2007    1.860136
2008    0.119815
Freq: A-DEC, dtype: float64
```
根据年度时期的第一个月，每年的时期被取代为每月的时期。
```python
ts.asfreq('M',how = 'start') 
2001-01    0.859500
2002-01   -1.213256
2003-01   -0.591023
2004-01   -0.393317
2005-01    1.015904
2006-01    0.278310
2007-01    1.860136
2008-01    0.119815
Freq: M, dtype: float64
```
每年的最后一个工作日，可以使用"B"频率，并指明想要该时期的末尾
```python
ts.asfreq('B',how = 'end')
2001-12-31    0.859500
2002-12-31   -1.213256
2003-12-31   -0.591023
2004-12-31   -0.393317
2005-12-30    1.015904
2006-12-29    0.278310
2007-12-31    1.860136
2008-12-31    0.119815
Freq: B, dtype: float64
```
<a name="v5HBl"></a>
## 按季度计算的时期频率
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652257492228-5cfbaa0d-3c12-4237-be39-16f0d6fd47fd.png#clientId=uf289bd6c-35ff-4&from=paste&id=u020a1c2b&originHeight=419&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u95f50155-a7b7-4803-a8b5-3c2739393a8&title=)
<a name="Q5PcE"></a>
### 季度型数据
许多季度型数据都会涉及"财年末"，通常是一年12个月的最后一个日历日或工作日。时期"2012Q4"根据财年末的不同会有不同含义。
```python
>>> p = pd.Period('2012Q4',freq = 'Q-FEB') # Q-FEB是指2月末的工作日是财政年末
>>> p
Period('2012Q4', 'Q-FEB')
>>> p.asfreq('D','start')
Period('2011-12-01', 'D')
>>> p.asfreq('D','end')
Period('2012-02-29', 'D')

>>> from pandas.tseries.offsets import Hour
>>> pm4 = (p.asfreq('B','end') - 1).asfreq('T','start') + 16 * Hour()
>>> pm4
Period('2012-02-28 16:00', 'T')
```
<a name="Snb1U"></a>
### 季度型范围
```python
>>> rng = pd.period_range('2012Q1','2012Q4',freq = 'Q-DEC')
>>> rs = pd.Series(np.random.randn(len(rng)), index = rng)
>>> rs
2012Q1    3.271631
2012Q2    0.030144
2012Q3    0.778939
2012Q4   -0.785795
Freq: Q-DEC, dtype: float64
        
>>> rng_new = (rs.index.asfreq('B','end') - 1
              ).asfreq('T','start') + 16 * Hour()
>>> rng_new
PeriodIndex(['2012-03-29 16:00', '2012-06-28 16:00',
             '2012-09-27 16:00', '2012-12-28 16:00'],
            dtype='period[T]', freq='T')

>>> rs1 = rs.copy()
>>> rs.index
PeriodIndex(['2012Q1', '2012Q2', '2012Q3', '2012Q4']
            , dtype='period[Q-DEC]', freq='Q-DEC')
```
<a name="ERYGQ"></a>
#### 将period 转换为 timestamp
```python
>>> rs1.index = rng_new
>>> rs1
2012-03-29 16:00    3.271631
2012-06-28 16:00    0.030144
2012-09-27 16:00    0.778939
2012-12-28 16:00   -0.785795
Freq: T, dtype: float64

>>> rs.index = rng_new.to_timestamp()   
>>> rs
2012-03-29 16:00:00    3.271631
2012-06-28 16:00:00    0.030144
2012-09-27 16:00:00    0.778939
2012-12-28 16:00:00   -0.785795
dtype: float64
    
>>> rs.index
DatetimeIndex(['2012-03-29 16:00:00', '2012-06-28 16:00:00',
               '2012-09-27 16:00:00', '2012-12-28 16:00:00'],
              dtype='datetime64[ns]', freq=None)

>>> rs1.index
PeriodIndex(['2012-03-29 16:00', '2012-06-28 16:00',
             '2012-09-27 16:00', '2012-12-28 16:00'],
            dtype='period[T]', freq='T')
```
<a name="JVhAY"></a>
#### Timestamp与Period互转
将Timestamp转换为Period（及其反向过程）<br />通过使用`to_period`方法，可以将由时间戳索引的Series和DataFrame对象转换为以时期索引。
```python
>>> rng = pd.date_range('2000-1-1',periods = 3, freq = 'M')
>>> ts = pd.Series(np.random.randn(len(rng)), index = rng)
>>> ts
2000-01-31    0.185383
2000-02-29   -0.304269
2000-03-31    1.447754
Freq: M, dtype: float64
        
>>> pts = ts.to_period()
>>> pts
2000-01    0.185383
2000-02   -0.304269
2000-03    1.447754
Freq: M, dtype: float64
        
>>> rng = pd.date_range('2000-1-29',periods = 6, freq = 'D')
>>> ts2 = pd.Series(np.random.randn(len(rng)), index = rng)
>>> ts2
2000-01-29   -1.246242
2000-01-30    2.271754
2000-01-31    0.935833
2000-02-01   -0.527219
2000-02-02   -0.385504
2000-02-03    0.504284
Freq: D, dtype: float64
```
新PeriodIndex的频率默认是从时间戳推断而来的，也可以指定任何别的频率
```python
>>> ts2.to_period('M') # 结果中允许存在重复时期
2000-01   -1.246242
2000-01    2.271754
2000-01    0.935833
2000-02   -0.527219
2000-02   -0.385504
2000-02    0.504284
Freq: M, dtype: float64
```
要转换回时间戳，使用`to_timestamp`即可。
```python
>>> pts = ts2.to_period()
>>> ts3 = pts.to_timestamp(how = 'end')
>>> ts3
2000-01-29 23:59:59.999999999   -1.246242
2000-01-30 23:59:59.999999999    2.271754
2000-01-31 23:59:59.999999999    0.935833
2000-02-01 23:59:59.999999999   -0.527219
2000-02-02 23:59:59.999999999   -0.385504
2000-02-03 23:59:59.999999999    0.504284
Freq: D, dtype: float64
```
<a name="XIdHM"></a>
## 通过数组创建PeriodIndex
某些数据集中时间信息是分开在多个列存放的，可以通过PeriodIndex的参数将这些列组合在一起。
```python
>>> year = [2017, 2017, 2017, 2017,
            2018, 2018, 2018, 2018]
>>> quarter = [1,2,3,4,1,2,3,4]
>>> index = pd.PeriodIndex(year=year,quarter=quarter,freq='Q-DEC')
>>> index
PeriodIndex(['2017Q1', '2017Q2', '2017Q3',
             '2017Q4', '2018Q1', '2018Q2',
             '2018Q3', '2018Q4'],
            dtype='period[Q-DEC]', freq='Q-DEC')
```
