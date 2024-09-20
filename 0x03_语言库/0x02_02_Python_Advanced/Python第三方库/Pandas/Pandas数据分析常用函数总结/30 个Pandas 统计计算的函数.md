Python Pandas
<a name="QSSkH"></a>
## 1、数据预览
本文案例演示数据来自国家数据中心的各地区最近5年的国民生产总值数据。<br />[各地区生产总值.xlsx](https://www.yuque.com/attachments/yuque/0/2021/xlsx/396745/1630109821581-9c1fc100-e8e4-4ac0-ae1a-3ffdabf8eab8.xlsx?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fxlsx%2F396745%2F1630109821581-9c1fc100-e8e4-4ac0-ae1a-3ffdabf8eab8.xlsx%22%2C%22name%22%3A%22%E5%90%84%E5%9C%B0%E5%8C%BA%E7%94%9F%E4%BA%A7%E6%80%BB%E5%80%BC.xlsx%22%2C%22size%22%3A11693%2C%22type%22%3A%22application%2Fvnd.openxmlformats-officedocument.spreadsheetml.sheet%22%2C%22ext%22%3A%22xlsx%22%2C%22status%22%3A%22done%22%2C%22taskId%22%3A%22u8296c41c-9ab9-4c50-9175-e8862865739%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22u34e290df%22%2C%22card%22%3A%22file%22%7D)
```python
In [1]: df.head() # 预览前5条数据
Out[1]: 
       地区    2020年    2019年    2018年    2017年    2016年
0     北京市  36102.6  35445.1  33106.0  29883.0  27041.2
1     天津市  14083.7  14055.5  13362.9  12450.6  11477.2
2     河北省  36206.9  34978.6  32494.6  30640.8  28474.1
3     山西省  17651.9  16961.6  15958.1  14484.3  11946.4
4  内蒙古自治区  17359.8  17212.5  16140.8  14898.1  13789.3

In [2]: df.info() # 查看各字段数据类型、条数及空值数
<class 'pandas.core.frame.DataFrame'>
RangeIndex: 32 entries, 0 to 31
Data columns (total 6 columns):
 #   Column  Non-Null Count  Dtype  
---  ------  --------------  -----  
 0   地区      32 non-null     object 
 1   2020年   31 non-null     float64
 2   2019年   31 non-null     float64
 3   2018年   31 non-null     float64
 4   2017年   31 non-null     float64
 5   2016年   31 non-null     float64
dtypes: float64(5), object(1)
memory usage: 1.6+ KB
```
<a name="TbzKi"></a>
## 2、描述统计
`describe`函数方法 可以返回数据集的描述性统计
```python
Signature:
df.describe(
    percentiles=None,
    include=None,
    exclude=None,
    datetime_is_numeric=False,
) -> 'FrameOrSeries'
Docstring:
Generate descriptive statistics.
```
对于`Dataframe`类型来说，每行对应一个统计指标，分别是总数、平均值、标准差、最小值、四分位（默认是25/50/75）和最大值。
```python
In [3]: df.describe()
Out[3]: 
               2020年          2019年         2018年         2017年         2016年
count      31.000000      31.000000     31.000000     31.000000     31.000000
mean    32658.551613   31687.758065  29487.661290  26841.819355  24224.148387
std     26661.811640   25848.652250  24136.181387  22161.575235  20008.278500
min      1902.700000    1697.800000   1548.400000   1349.000000   1173.000000
25%     13940.650000   13826.300000  13104.700000  12381.800000  11634.800000
50%     25115.000000   24667.300000  22716.500000  20210.800000  18388.600000
75%     42612.500000   41110.350000  37508.750000  33835.250000  30370.250000
max    110760.900000  107986.900000  99945.200000  91648.700000  82163.200000
```
上面的描述性统计表，可以看到2020年一共有31个地区有数据，GDP均值为3.26万亿，最高的11.07万亿，最低的0.19万亿等。<br />可以看到，还有参数可以进行自定义选择，作用如下：<br />`percentiles`可以自定指定分位数
```python
In [4]: df.describe(percentiles=[.2, .4, .6, .8])
Out[4]: 
               2020年          2019年         2018年         2017年         2016年
count      31.000000      31.000000     31.000000     31.000000     31.000000
mean    32658.551613   31687.758065  29487.661290  26841.819355  24224.148387
std     26661.811640   25848.652250  24136.181387  22161.575235  20008.278500
min      1902.700000    1697.800000   1548.400000   1349.000000   1173.000000
20%     13698.500000   13544.400000  12809.400000  11159.900000  10427.000000
40%     22156.700000   21237.100000  19627.800000  17790.700000  16116.600000
50%     25115.000000   24667.300000  22716.500000  20210.800000  18388.600000
60%     36102.600000   34978.600000  32494.600000  29676.200000  26307.700000
80%     43903.900000   45429.000000  42022.000000  37235.000000  33138.500000
max    110760.900000  107986.900000  99945.200000  91648.700000  82163.200000
```
`include`，`exclude`分别指定和排除数据类型，比如
```python
df.describe(include=[np.number]) # 指定数字类型的字段
df.describe(exclude=[np.float]) # 排除浮点类型的字段
```
可以看到，默认情况下describe指定的都是数字类型的，地区字段部分的未参与，如果想参与可以通过include='all'来指定。
```python
In [5]: df.describe(include='all')
Out[5]: 
         地区      2020年      2019年     2018年     2017年     2016年
count    32      31.00      31.00     31.00     31.00     31.00
unique   32        NaN        NaN       NaN       NaN       NaN
top     北京市        NaN        NaN       NaN       NaN       NaN
freq      1        NaN        NaN       NaN       NaN       NaN
...     ...        ...        ...       ...       ...       ...
25%     NaN   13940.65   13826.30  13104.70  12381.80  11634.80
50%     NaN   25115.00   24667.30  22716.50  20210.80  18388.60
75%     NaN   42612.50   41110.35  37508.75  33835.25  30370.25
max     NaN  110760.90  107986.90  99945.20  91648.70  82163.20

[11 rows x 6 columns]
```
案例数据中，地区字段下的数据都是`object`类型，非数字相关。可以发现在描述统计结果中，它新增了`unique`、`top`和`frep`三个指标，相反这三个指标对于纯数字类型的字段列是没有的。这三个指标分别对应非重复数、最大值和频率（如有重复的），比如下面这个单独案例：
```python
In [6]: s = pd.Series(['red','blue','black','grey','red','grey'])

In [7]: s.describe()
Out[7]: 
count       6
unique      4
top       red
freq        2
dtype: object
```
在`descripe`函数中还有一个参数`datetime_is_numeric`，如果是对时间类型数据的统计描述需要赋值`True`。
```python
In [8]: s = pd.Series([np.datetime64("2000-01-01"),
    ...:                np.datetime64("2010-01-01"),
    ...:                np.datetime64("2010-01-01")
    ...:                ])

In [9]: s.describe()
FutureWarning: Treating datetime data as categorical rather than numeric in `.describe` is deprecated and will be removed in a future version of pandas. Specify `datetime_is_numeric=True` to silence this warning and adopt the future behavior now.
  s.describe()
Out[9]: 
count                       3
unique                      2
top       2010-01-01 00:00:00
freq                        2
first     2000-01-01 00:00:00
last      2010-01-01 00:00:00
dtype: object

In [10]: s.describe(datetime_is_numeric=True)
Out[10]: 
count                      3
mean     2006-09-01 08:00:00
min      2000-01-01 00:00:00
25%      2004-12-31 12:00:00
50%      2010-01-01 00:00:00
75%      2010-01-01 00:00:00
max      2010-01-01 00:00:00
dtype: object
```
在日常数据处理中，除了描述统计里的这些统计维度外，还会用到其他一些统计计算，比如方差、众数等等。
<a name="TvMkn"></a>
## 3、统计计算
这里演示常见的统计计算函数方法，默认情况下都是按列统计，也可以指定按行，具体见下方演示
```python
# 最大值
In [11]: df.max(numeric_only=True)
Out[11]: 
2020年    110760.9
2019年    107986.9
2018年     99945.2
2017年     91648.7
2016年     82163.2
dtype: float64

# 最小值
In [12]: df.min(numeric_only=True)
Out[12]: 
2020年    1902.7
2019年    1697.8
2018年    1548.4
2017年    1349.0
2016年    1173.0
dtype: float64

# 平均值 （统计项的计算，建议指定数据类型为仅数字，可以通过axis指定是行列，默认是列）
In [13]: df.mean(axis=1, numeric_only=True)
Out[13]: 
0     32315.58
1     13085.98
2     32559.00
3     15400.46
        ...   
28     2683.66
29     3432.18
30    12198.96
31         NaN
Length: 32, dtype: float64
```
以下部分不做具体演示，仅介绍函数功能，所有这些在使用的时候都要注意下原始数据类型，非数字类型可能会出现报错
```python
df.sum() # 求和
df.corr() # 相关系数
df.cov() # 协方差
df.count() # 非空计数
df.abs() # 绝对值
df.median() # 中位数
df.mode() # 众数
df.std() # 标准差
df.var() # 无偏方差
df.sem() # 均值的标准误差
df.mad() # 平均绝对差
df.prod() # 连乘
df.cumprod() # 累乘
df.cumsum() # 累加
df.nunique() # 非重复计数
df.idxmax() # 最大值索引名（注意是索引名，区别于argmax）
df.idxmin() # 最小值索引名
df.sample(5) # 随机抽样5条数据 
df.skew() # 样本偏度（第三阶）
df.kurt() # 样本偏度（第四阶）
df.quantile() # 样本分位数
df.rank() # 排名
df.pct_change() # 变化率
df.value_counts() # 不重复值及数量
s.argmax() # 最大值索引（自动索引），dataframe没有
s.argmin() # 最小值索引（自动索引），dataframe没有
```
其实，在每个函数中都有别的参数可以让函数功能更强大，具体大家可以自行尝试，这里简单举个别例子。
```python
>>> s = pd.Series([90, 91, 85])
>>> s
0    90
1    91
2    85
dtype: int64

>>> s.pct_change()
0         NaN
1    0.011111
2   -0.065934
dtype: float64

>>> s.pct_change(periods=2) # 隔2行求变化率（默认是隔1行）
0         NaN
1         NaN
2   -0.055556
dtype: float64
```
除了上述这些函数外，以下几个函数也常用到
```python
# 某列最大的前5行数据
In [14]: df.nlargest(5,columns='2020年')
Out[14]: 
     地区     2020年     2019年    2018年    2017年    2016年
18  广东省  110760.9  107986.9  99945.2  91648.7  82163.2
9   江苏省  102719.0   98656.8  93207.6  85869.8  77350.9
14  山东省   73129.0   70540.5  66648.9  63012.1  58762.5
10  浙江省   64613.3   62462.0  58002.8  52403.1  47254.0
15  河南省   54997.1   53717.8  49935.9  44824.9  40249.3

# 某列最小的前五行数据
In [15]: df.nsmallest(5,columns='2020年')
Out[15]: 
         地区   2020年   2019年   2018年   2017年   2016年
25    西藏自治区  1902.7  1697.8  1548.4  1349.0  1173.0
28      青海省  3005.9  2941.1  2748.0  2465.1  2258.2
29  宁夏回族自治区  3920.5  3748.5  3510.2  3200.3  2781.4
20      海南省  5532.4  5330.8  4910.7  4497.5  4090.2
27      甘肃省  9016.7  8718.3  8104.1  7336.7  6907.9
```
<a name="nrakE"></a>
### 加减乘除四则运算等
可以用运算符号，也可以用函数方法；同样可以传一个值，也可以传一个`DataFrame`或`Serice`。
```python
'''
Among flexible wrappers 
(`add`, `sub`, `mul`, `div`, `mod`, `pow`) 
to arithmetic 
operators: 
`+`, `-`, `*`, `/`, `//`, `%`, `**`.
'''
df + 1
df.add(1)
```
