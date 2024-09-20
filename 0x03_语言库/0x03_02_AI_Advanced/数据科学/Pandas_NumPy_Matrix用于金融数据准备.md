数据特征 Pandas Numpy Matrix<br />数据准备是一项必须具备的技术，是一个迭代且灵活的过程，可以用于查找、组合、清理、转换和共享数据集，包括用于分析/商业智能（BI）、数据科学/机器学习（ML）和自主数据集成中。具体来说，数据准备是在处理和分析之前对原始数据进行清洗和转换的过程，通常包括重新格式化数据、更正数据和组合数据集来丰富数据等。
<a name="pnm2U"></a>
## 股市数据获取的几个模块
<a name="PB7rM"></a>
### Tushare
Tushare是一个免费、开源的python财经数据接口包。主要实现对股票等金融数据从数据采集、清洗加工到数据存储的过程，能够为金融分析人员提供快速、整洁、和多样的便于分析的数据，为他们在数据获取方面极大地减轻工作量，使他们更加专注于策略和模型的研究与实现上。
<a name="P2012"></a>
#### 首先使用pip安装第三方依赖库`tushare`下载股市数据。（国内）
```python
pip install tushare -i https://pypi.douban.com/simple/
```
<a name="O5W8b"></a>
#### 然后在[tushare.pro](https://tushare.pro)注册用户，注意获取自己的`token`。使用`daily`函数获取日线数据。
```python
# 导入tushare库
import tushare as ts
# 设置token
ts.set_token('your token here')
# 初始化pro接口
pro = ts.pro_api()
# 获取日线数据
df = pro.daily(ts_code='000001.SZ', start_date='20180701', end_date='20180718'
```
<a name="nhjro"></a>
### Baostock
证券宝([http://www.baostock.com](http://www.baostock.com))是一个免费、开源的证券数据平台（无需注册）。提供大量准确、完整的证券历史行情数据、上市公司财务数据等。通过python API获取证券数据信息，满足量化交易投资者、数量金融爱好者、计量经济从业者数据需求。
<a name="M3sQR"></a>
#### 首先使用pip安装第三方依赖库baostock下载股市数据。（国内）
```python
pip install baostock -i https://pypi.douban.com/simple/
```
<a name="sJOTA"></a>
#### 然后使用`query_history_k_data_plus`函数获取日线数据
```python
import baostock as bs
import pandas as pd

# 登陆系统
lg = bs.login()
# 获取沪深A股历史K线数据
rs_result = bs.query_history_k_data_plus("sh.600000",
                                         fields="date,open,high, low, close,preclose,volume,amount,adjustflag",
                                         start_date='2017-07-01', 
                                         end_date='2017-12-31', 
                                         frequency="d", 
                                         adjustflag="3")
df_result = rs_result.get_data()
# 登出系统
bs.logout()
```
<a name="X5GqR"></a>
### Yfinance
`yfinance`的老版本是`fix_yahoo_finance`，二者都可以使用，推荐使用新版本。
<a name="p2v23"></a>
#### 首先使用pip安装第三方依赖库`fix_yahoo_finance`下载`yahoo`股市数据。（国外）
```python
pip install fix_yahoo_finance -i https://pypi.douban.com/simple/
```
如果发生报错：`ModuleNotFoundError: No module named 'yfinance'`，则需要事先安装`'yfinance'`，最新版本已经将`fix_yahoo_finance`调整`'yfinance'`为
```python
pip install yfinance -i https://pypi.douban.com/simple/
```
<a name="bNTwu"></a>
#### 然后使用`pdr_override`函数获取日线数据
```python
import yfinance as yf
# 输入
symbol = 'AMD'
start = '2014-01-01'
end = '2018-08-27'

dataset=yf.pdr_override(symbol,start,end)
dataset.head()
```
下面开始运用数据处理最常用的第三方模块Pandas和NumPy获取数据，为后续数据分析、机器学习做数据准备。
<a name="rhLTH"></a>
## Pandas
Pandas 是基于NumPy 的一种工具，该工具是为解决数据分析任务而创建的。Pandas 纳入了大量库和一些标准的数据模型，提供了高效地操作大型数据集所需的工具。Pandas提供了大量能快速便捷地处理数据的函数和方法。它是使Python成为强大而高效的数据分析环境的重要因素之一。
```python
# Pandas Library
>>> import pandas as pd
>>> import warnings
>>> warnings.filterwarnings("ignore")
```
<a name="Sbxco"></a>
### 获取中国平安股票数据
```python
>>> import baostock as bs
# 登陆系统
>>> lg = bs.login()
# 获取沪深A股历史K线数据
>>> rs_result = bs.query_history_k_data_plus("sh.601318",  
                        fields="date,open,high,low,close,volume",   
                        start_date='2018-01-01',   
                        end_date='2021-03-31',  
                        frequency="d",  
                        adjustflag="3")
>>> df_result = rs_result.get_data()
# 登出系统 
>>> bs.logout()
login success!
logout success!
<baostock.data.resultset.ResultData at 0x28d05a44ac8>

>>> df_result.head()
```
![2021-05-12-20-14-01-425513.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823566642-8b89879f-6c31-43f4-80c7-5ba6a133da00.png#clientId=ub69ec0dc-a381-4&from=ui&id=u737e09fc&originHeight=332&originWidth=830&originalType=binary&size=27049&status=done&style=shadow&taskId=ufcf594dd-f643-468e-83ed-42e651a7a28)
<a name="f9Q7R"></a>
### 获取腾讯股票数据
```python
>>> import yfinance as yf

>>> symbol = 'TCEHY'
>>> start = '2016-01-01'
>>> end = '2021-03-31'
>>> yf.pdr_override()

>>> dataset = yf.download(symbol,start,end)
[*********************100%***********************]  1 of 1 completed

>>> dataset.head()
```
![2021-05-12-20-14-01-487346.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823583730-0fbdc6a7-376e-4027-b268-3086e594e200.png#clientId=ub69ec0dc-a381-4&from=ui&id=u5f7b85dc&originHeight=396&originWidth=1016&originalType=binary&size=35517&status=done&style=shadow&taskId=ue8d2408b-ae87-4ca2-bf55-e6b5d689ad2)
<a name="wrpFn"></a>
### 查看尾部数据
```python
dataset.tail()
```
![2021-05-12-20-14-01-541244.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823595297-fb4ab439-2a20-47cd-9a0f-55ecdbd7c588.png#clientId=ub69ec0dc-a381-4&from=ui&id=u561f789b&originHeight=400&originWidth=1042&originalType=binary&size=35092&status=done&style=shadow&taskId=u824979a7-51e9-48a1-adb9-8a126b61387)
```python
>>> dataset.shape
(1320, 6)
```
<a name="s3wp3"></a>
### 滚动窗口计算
```python
dataset.rolling(window=5).mean() # 求最后4行的均值
```
![2021-05-12-20-14-01-626997.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823610413-2b32f1e4-764c-4bd7-bfe3-5464c45a562e.png#clientId=ub69ec0dc-a381-4&from=ui&id=ucffd772a&originHeight=780&originWidth=1080&originalType=binary&size=53424&status=done&style=shadow&taskId=udbbeef0f-278e-4932-aa93-244723e6d79)
<a name="rIBo2"></a>
#### 函数解析
```python
dataset.rolling(
    window,
    min_periods=None,
    center=False,
    win_type=None,
    on=None,
    axis=0,
    closed=None,)
```
提供滚动窗口计算。<br />`window`：也可以省略不写。表示时间窗的大小，注意有两种形式（int or offset）。如果使用int，则数值表示计算统计量的观测值的数量即向前几个数据。 如果是offset类型，表示时间窗的大小。<br />`min_periods`：每个窗口最少包含的观测值数量，小于这个值的窗口结果为NA。值可以是int，默认None。offset情况下，默认为1。<br />`center`: 把窗口的标签设置为居中。布尔型，默认False，居右<br />`win_type`: 窗口的类型。截取窗的各种函数。字符串类型，默认为None。各种类型<br />`on`: 可选参数。对于dataframe而言，指定要计算滚动窗口的列。值为列名。<br />`axis`: int、字符串，默认为0，即对列进行计算<br />`closed`：定义区间的开闭，支持int类型的window。对于offset类型默认是左开右闭的即默认为right。可以根据情况指定为left both等。
<a name="Pd8AU"></a>
### 指数加权函数ewm
```python
>>> dataset['ewm'] = dataset['Adj Close'].ewm(span=20,
                                              min_periods=0,
                                              adjust=False,
                                              ignore_na=False).mean()
>>> dataset.head()
```
![2021-05-12-20-14-01-684817.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823707485-5f85ef52-41ce-41e7-8996-8b3254c8c554.png#clientId=ub69ec0dc-a381-4&from=ui&id=uc3073b7a&originHeight=360&originWidth=1080&originalType=binary&size=67789&status=done&style=shadow&taskId=uf134b111-308e-44e2-bbe0-249597d5072)
<a name="XK2bz"></a>
#### 函数解析
```python
DataFrame.ewm(com=None,  
            span=None,  
            halflife=None,  
            alpha=None,  
            min_periods=0,  
            adjust=True,  
            ignore_na=False,  
            axis=0,  
            times=None)
```
提供指数加权（EW）函数。可用EW功能：`mean()`，`var()`，`std()`，`corr()`，`cov()`。参数：`com`，`span`，`halflife`，或`alpha`必须提供。<br />`com float`, optional  根据质心指定衰减，<br />`span float`, optional  跨度，根据跨度指定衰减，<br />`halflife float`, str, timedelta, optional  半衰期，根据半衰期指定衰减，<br />如果`times`指定，则观察值衰减到其值一半的时间单位（str或timedelta）。仅适用于`mean()`，半衰期值不适用于其他功能。<br />`alpha float`, optional  直接地指定平滑系数，<br />`min_periods int`, default 0  窗口中具有值的最小观察数（否则结果为NA）。<br />`adjust bool`, default True  调整，在开始期间除以递减的调整因子，以解决相对权重的不平衡问题（将EWMA视为移动平均值）。

- 当`adjust=True`（默认）时，EW功能是使用权重计算的。例如，该系列的EW移动平均值将会
- 当`adjust=False`为时，将以递归方式计算指数加权函数：

`ignore_na bool`, default False  计算权重时忽略缺失值；指定True重现0.15.0之前的行为。

- 当`ignore_na=False`（默认）时，权重基于绝对位置。例如，权重和用于计算的最终加权平均数，如果`adjust=True`，则权重分别是和 1。如果`adjust=False`，权重分别是和
- 当`ignore_na=True`时，权重基于相对位置。例如，权重和用于计算的最终加权平均数，如果`adjust=True`，则权重分别是和 1。如果`adjust=False`，权重分别是和和 
<a name="P5j9t"></a>
### 提取数据
```python
# iloc[] 
>>> print(dataset.iloc[0][0])
19.59000015258789

# iat[] 
>>> print(dataset.iat[0,0])
19.59000015258789
```
`at`的使用方法与`loc`类似，但是比`loc`有更快的访问数据的速度，而且只能访问单个元素，不能访问多个元素。`iat`与`iloc`类似。
```python
# loc[]
>>> print(df.loc[0]['High'])
19.65999984741211
# iloc[]
>>> print(df.iloc[0])

Date 2015-12-31 00:00:00Open 19.59High 19.66Low 19.51Close 19.62Adj Close               19.3691Volume 382600ewm 19.3691Name: 0, dtype: object
```
<a name="ZpCxN"></a>
### 重置索引
```python
>>> df = dataset.reset_index()
>>> df.head()
```
![2021-05-12-20-14-01-752637.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823722195-0e8e0e8a-2377-4203-a54e-9337d108c933.png#clientId=ub69ec0dc-a381-4&from=ui&id=u22c6dce7&originHeight=301&originWidth=1080&originalType=binary&size=63723&status=done&style=shadow&taskId=u625fa597-87aa-49f5-87ee-a7454bac78b)
<a name="MiFiL"></a>
### 删除列
```python
# 删除数据
>>> new_df = df.drop(['Date'], axis=1)
>>> new_df.head()
```
![2021-05-12-20-14-01-804498.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823734816-f1b15bc9-72fb-45fc-95a3-cad07571b7de.png#clientId=ub69ec0dc-a381-4&from=ui&id=u6c8c868f&originHeight=334&originWidth=1038&originalType=binary&size=32850&status=done&style=shadow&taskId=u00b74538-34aa-4d1f-b8a8-46639f1053c)
<a name="pXoV9"></a>
### 添加列
```python
# 添加日期
>>> new_column = df['Date']
>>> new_df['Date'] = new_column
>>> new_df.head()
```
![2021-05-12-20-14-01-873332.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823750292-61cbe460-a577-447d-8929-9c3ed74f0e26.png#clientId=ub69ec0dc-a381-4&from=ui&id=u032676a6&originHeight=297&originWidth=1080&originalType=binary&size=63643&status=done&style=shadow&taskId=u8970bdff-6fbd-49a0-921d-ad144fed3b0)
<a name="aIkVs"></a>
### 移动列
```python
# 将 Date 移动至第一列
>>> cols = list(new_df)
>>> cols.insert(0, cols.pop(cols.index('Date')))
>>> cols
['Date', 'Open', 'High', 'Low', 'Close', 'Adj Close', 'Volume', 'ewm']

# loc是DataFrame有索引标记的值的
>>> new_df = new_df.loc[:, cols]
>>> new_df.head()
```
![2021-05-12-20-14-01-941135.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823763844-e65bd33a-e6cc-4ac7-aaac-972c6da7281c.png#clientId=ub69ec0dc-a381-4&from=ui&id=u45c4a59d&originHeight=302&originWidth=1080&originalType=binary&size=64380&status=done&style=none&taskId=u42a1cbb3-a96b-4201-a9fd-e4bef40cb6f)
<a name="WXKj1"></a>
### 删除列
```python
>>> del new_df['Close']
>>> new_df.head()
```
![2021-05-12-20-14-01-988008.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823786472-3132d621-caf9-413f-af1c-a8a9681a04f5.png#clientId=ub69ec0dc-a381-4&from=ui&id=u21cf2641&originHeight=330&originWidth=1060&originalType=binary&size=35793&status=done&style=none&taskId=u7a44cd86-48a1-4514-888e-7abfc506436)
<a name="whUqC"></a>
### 重命名列
<a name="Jfqus"></a>
#### 重命名
```python
>>> new_df = new_df.rename(index=str,  
                           columns={'Adj Close':'Close'})
>>> new_df.head()
```
![2021-05-12-20-14-02-047849.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823796324-e847bd7b-402b-46ed-8f22-f4ecdbc807fa.png#clientId=ub69ec0dc-a381-4&from=ui&id=ub70cc631&originHeight=326&originWidth=1042&originalType=binary&size=35021&status=done&style=shadow&taskId=ub40221b0-b5f7-456c-886b-746ecdfc304)
<a name="D4S80"></a>
#### 列名改为大写
```python
>>> new_df.rename(str.upper, axis='columns')
```
![2021-05-12-20-14-02-104735.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823814191-c0fc528c-a7bb-41bf-9020-824fd0758909.png#clientId=ub69ec0dc-a381-4&from=ui&id=u402bdeb3&originHeight=668&originWidth=1080&originalType=binary&size=119970&status=done&style=shadow&taskId=u6bd73e3e-7ec7-443d-8f75-93ffa73a691)
<a name="QiSuY"></a>
#### 列名改为小写
```python
# column name to lower case
new_df.rename(str.lower, axis='columns')
```
![2021-05-12-20-14-02-188471.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823829093-0551109f-3f94-4efc-bb70-abdc6d8a2110.png#clientId=ub69ec0dc-a381-4&from=ui&id=u6ae55b65&originHeight=689&originWidth=1080&originalType=binary&size=123930&status=done&style=shadow&taskId=uba1d5e06-88b0-4803-ac90-f6273a60ff4)
<a name="vF23T"></a>
### 选择多个列
```python
>>> new_df[new_df.columns[1:5]]
```
![2021-05-12-20-14-02-240337.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823841572-faedefd8-ffeb-4ed5-be3b-33a5cfe690ac.png#clientId=ub69ec0dc-a381-4&from=ui&id=u0ac98c52&originHeight=718&originWidth=678&originalType=binary&size=42077&status=done&style=shadow&taskId=u0b3a80c5-402e-4b84-95b2-b273db9fe36)
<a name="JZJNS"></a>
### 选择多个行
```python
>>> new_df[1:4]
```
![2021-05-12-20-14-02-286211.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620823856762-4f1af1ba-e6af-4f65-b6e7-98d99032e694.png#clientId=ub69ec0dc-a381-4&from=ui&id=u145f620c&originHeight=228&originWidth=1048&originalType=binary&size=25121&status=done&style=none&taskId=ua4dd7807-f4be-4f05-9a62-1bc7569619c)
<a name="sJ2lZ"></a>
### 创建Dataframe
```python
>>> stock_df = pd.DataFrame([[123.50, 145.35, 165.50],   
                             [152.35, 154.67, 160.35],   
                             [201.25, 236.54, 254.69]], 
                            columns=['IBM', 'Apple', 'Tesla'])
>>> stock_df
```
|  | IBM | Apple | Tesla |
| --- | --- | --- | --- |
| 0 | 123.50 | 145.35 | 165.50 |
| 1 | 152.35 | 154.67 | 160.35 |
| 2 | 201.25 | 236.54 | 254.69 |

<a name="JXIBZ"></a>
### 替换数据
```python
>>> stock_df = stock_df.replace([201.25, 145.35], [888, 888])
>>> stock_df
```
| IBM | Apple | Tesla |  |
| --- | --- | --- | --- |
| 0 | 123.50 | 888.00 | 165.50 |
| 1 | 152.35 | 154.67 | 160.35 |
| 2 | 888.00 | 236.54 | 254.69 |

```python
>>> stock_df = stock_df.replace([165.50, 160.35], ['NaN', 'NaN'])
>>> stock_df
```
| IBM | Apple | Tesla |  |
| --- | --- | --- | --- |
| 0 | 123.5 | 888 | NaN |
| 1 | 152.35 | 154.67 | NaN |
| 2 | 888 | 236.54 | 254.69 |

<a name="msbcu"></a>
### 获取数据框数据
```python
>>> for index, row in stock_df.iterrows() :
...     print(row['IBM'], row['Apple'])
123.5 888.0
152.35 154.67
888.0 236.54
```
<a name="qCsUP"></a>
## NumPy
NumPy是专为简化Python中的数组运算而设计的，每个NumPy数组都具有以下属性：

- `ndim`：维数。
- `shape`：每一维的大小。
- `size`：数组中元素的总数。
- `dtype`：数组的数据类型（例如int、float、string等）。
```python
# Numpy 模块
>>> import numpy as np
```
<a name="R1TVL"></a>
### 将数据集转换为numpy
```python
# 将打开的DataFrame转换为numpy数组
>>> Open_array = np.array(dataset['Open'])
>>> Open_array  
array([19.59000015, 19.12999916, 19.09000015, ..., 79.44000244,
       79.19000244, 78.56999969])
```
<a name="ghT5Y"></a>
### 查看单个元素
```python
>>> print("First element:", Open_array [0])
>>> print("Second element:", Open_array [1])
>>> print("Second last element:", Open_array[-1])
First element: 19.59000015258789
Second element: 19.1299991607666
Second last element: 78.56999969482422
```
<a name="ywqWs"></a>
### 查看多个元素
```python
>>> print(Open_array[2:5]) # 第3个到第5个 
[19.09000015 18.63999939 18.04999924]  

>>> print(Open_array[:-5]) # 从开始到最后第4个  
[19.59000015 19.12999916 19.09000015 ... 80.69999695 81.90000153
 81.65000153]  

>>> print(Open_array[5:])  # 第6个到最后
[18.32999992 17.76000023 17.64999962 ... 79.44000244 79.19000244
 78.56999969]  

>>> print(Open_array[:])   # 开始到最后  
[19.59000015 19.12999916 19.09000015 ... 79.44000244 79.19000244
 78.56999969]
```
<a name="PTsSp"></a>
### 修改元素
<a name="thT0y"></a>
#### 改变第一个元素
```python
>>> volume = np.array(dataset['Volume'])

>>> volume[0] = 0    
>>> print(volume)  
[      0  469100  170300 ... 5923100 3468900 1715900]
```
<a name="ZBPuL"></a>
#### 改变多个元素
```python
# 更改第3到5个元素
>>> volume[2:5] = np.array([4, 6, 8])   
>>> print(volume)      
[      0  469100       4 ... 5923100 3468900 1715900]
```
<a name="N47px"></a>
### 增加元素
```python
>>> add_numbers = np.array([1, 2, 3])
>>> np.concatenate((volume, add_numbers)
array([     0, 469100,      4, ...,      1,      2,      3], dtype=int64)
>>> np.append(volume, add_numbers, axis=0)
array([     0, 469100,      4, ...,      1,      2,      3], dtype=int64)
```
<a name="v6v28"></a>
### 删除元素
<a name="XIedM"></a>
#### 直接删除
```python
>>> print(volume)
[    0  469100     4 ... 5923100 3468900 1715900]
>>> np.delete(volume, 1) 
array([    0,     4,     6, ..., 5923100, 3468900, 1715900], dtype=int64)
```
<a name="IB2wc"></a>
#### 转换为`list()`来使用`remove`
```python
>>> volume = volume.tolist() # Convert tolist() to use remove
>>> volume
```
![2021-05-12-20-14-02-355027.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620821914000-f2efb493-d0a6-4d63-9c6a-95d2f1a05489.png#clientId=ub69ec0dc-a381-4&from=ui&id=uf97d28ce&originHeight=362&originWidth=1080&originalType=binary&size=32062&status=done&style=shadow&taskId=ub335ee43-0b20-46ee-8914-8bc2e6fdb44)
```python
>>> volume.remove(0)
>>> print(volume)
```
![2021-05-12-20-14-02-504626.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620821925822-ea8bbeaa-3e91-4a84-89bb-225c5b8a93c9.png#clientId=ub69ec0dc-a381-4&from=ui&id=ufe613a98&originHeight=336&originWidth=1080&originalType=binary&size=424096&status=done&style=shadow&taskId=u96739f48-c8f2-4eba-b993-87b5c4390d6)
```python
>>> print(volume.pop(2))
>>> print(volume)
```
![2021-05-12-20-14-02-655222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620821938068-36ed72d4-6610-4fae-be55-336ca196b8a2.png#clientId=ub69ec0dc-a381-4&from=ui&id=u5b04a143&originHeight=338&originWidth=1080&originalType=binary&size=402618&status=done&style=none&taskId=u447367b3-a3f7-45e7-97ac-747698b9293)
<a name="qTEnF"></a>
## Matrix
在数学中，矩阵（Matrix）是一个按照长方阵列排列的复数或实数集合。由 `m × n` 个数`aij`排成的`m行n列`的数表称为`m行n列`的矩阵，简称`m × n`矩阵。矩阵运算在科学计算中非常重要，而矩阵的基本运算包括矩阵的加法，减法，数乘，转置，共轭和共轭转置 。
```python
>>> import numpy as np # 对于矩阵和线性代数
>>> import numpy.matlib
```

:::warning
报错提示：这里用到`Matrix`时，需要将含有`Matrix`的子包`matlib`导入，不然会报错。`module 'numpy' has no attribute 'matlib'`
:::
这是因为`numpy`。`matlib`是`numpy`的可选子包，必须单独导入。如果您只导入`numpy`而不导入子包`matlib`，那么Python将把`matlib`作为`numpy`包的属性来查找。如果没有导入`numpy.matlib`，这个属性没有被分配给`numpy`。
```python
>>> import yfinance as yf
>>> symbol = 'TCEHY'
>>> start = '2021-03-20'
>>> end = '2021-03-31'
>>> yf.pdr_override()
>>> dataset = yf.download(symbol,start,end)
>>> df = dataset.drop(['High', 'Low', 'Close', 'Volume'], axis=1)
>>> df.head()
```
| Date | Open | Adj Close |
| --- | --- | --- |
| 2021-03-19 | 80.699997 | 82.639999 |
| 2021-03-22 | 81.900002 | 82.769997 |
| 2021-03-23 | 81.650002 | 80.930000 |
| 2021-03-24 | 80.930000 | 76.809998 |
| 2021-03-25 | 77.309998 | 77.419998 |

<a name="uTsZo"></a>
### 转换为矩阵
```python
>>> A = np.matrix(df)
>>> A
matrix([[80.69999695, 82.63999939],
       [81.90000153, 82.76999664],
       [81.65000153, 80.93000031],
       [80.93000031, 76.80999756],
       [77.30999756, 77.41999817],
       [79.44000244, 80.98000336],
       [79.19000244, 78.69000244],
       [78.56999969, 78.83000183]])
```
因为`matrix`很多操作不方便，如添加修改某个元素。这里可以先用`array`函数读写完毕以后，再用`matrix`函数让它恢复矩阵类型。
<a name="hpfNr"></a>
### 转换数组
```python
>>> A = np.array(df)
>>> A
array([[80.69999695, 82.63999939],
       [81.90000153, 82.76999664],
       [81.65000153, 80.93000031],
       [80.93000031, 76.80999756],
       [77.30999756, 77.41999817],
       [79.44000244, 80.98000336],
       [79.19000244, 78.69000244],
       [78.56999969, 78.83000183]])
```
<a name="uA6Cj"></a>
### 访问元素
```python
>>> type(A)
numpy.ndarray

>>> print("A =", A)
A = [[80.69999695 82.63999939]
 [81.90000153 82.76999664]
 [81.65000153 80.93000031]
 [80.93000031 76.80999756]
 [77.30999756 77.41999817]
 [79.44000244 80.98000336]
 [79.19000244 78.69000244]
 [78.56999969 78.83000183]]

>>> print("A[1] =", A[1])      # 第2行
A[1] = [81.90000153 82.76999664]

>>> print("A[1][2] =", A[1][1])   # 第2行的第2个元素
A[1][2] = 82.7699966430664

>>> print("A[0][-1] =", A[0][-1])  # 第1行的最后第1个元素A[0][-1] = 82.63999938964844
```
<a name="l0uvP"></a>
### 取出元素放到列表中
```python
>>> column = [];        # 空列表
>>> for row in A:
...     column.append(row[1]) 

>>> print("Second column =", column)
Second column = [82.63999938964844, 82.7699966430664, 80.93000030517578, 76.80999755859375,   
                 77.41999816894531, 80.9800033569336,8.69000244140625, 78.83000183105469]
```
<a name="UbxCT"></a>
### arange and shape
```python
>>> A = np.arange(4)
>>> print('A =', A)
A = [0 1 2 3]
>>> B = np.arange(12).reshape(2, 6)
>>> print('B =', B)
B = [[ 0  1  2  3  4  5]
     [ 6  7  8  9 10 11]]
```
<a name="xns3u"></a>
### 矩阵的运算
```python
>>> A = np.array(dataset['High'], dataset['Low'])
>>> B = np.array(dataset['Open'], dataset['Close'])
>>> print(A)
array([82.63999939, 82.84999847, 81.94000244, 81.16000366,   
       78.19000244, 80.98000336, 79.80000305, 79.41000366])
```
<a name="aGV1K"></a>
#### 矩阵加法
```python
>>> C = A + B
>>> print("矩阵加法: \n", C)

矩阵加法: 
[163.33999634 164.75     163.59000397 162.09000397   
155.5 160.4200058  158.99000549 157.98000336]
```
<a name="vIbUE"></a>
#### 矩阵减法
```python
>>> D = A - B
>>> print("矩阵减法: \n", D)

矩阵减法: 
 [1.94000244 0.94999695 0.29000092 0.23000336  
  0.88000488 1.54000092 0.61000061 0.84000397]
```
<a name="HQuf2"></a>
#### 两个矩阵的乘法
```python
>>> E = A.dot(B)
>>> print("两矩阵点乘: \n", E)
两矩阵点乘: 
 51749.67010773317
```
<a name="gjqk3"></a>
#### 矩阵转置
```python
>>> T = A.transpose()
>>> print("矩阵转置: \n", T)

矩阵转置: 
 [82.63999939 82.84999847 81.94000244 81.16000366   
  78.19000244 80.98000336 79.80000305 79.41000366]
```
<a name="RMKkm"></a>
### 访问矩阵元素、行和列
<a name="mofLu"></a>
#### 访问一维矩阵单个元素
```python
# 第一个元素 
>>> print("A[0] =", A[0])
A[0] = 82.63999938964844

# 第三个元素
>>> print("A[2] =", A[2])
A[2] = 81.94000244140625

# 最后一个元素 
>>> print("A[-1] =", A[-1])  
A[-1] = 79.41000366210938
```
<a name="SWmpE"></a>
#### 访问多维矩阵单个元素
```python
>>> A = np.array(dataset[['Open', 'Low', 'High', 'Close']])
>>> print("矩阵A: \n", A)
矩阵A: 
 [[80.69999695 80.09999847 82.63999939 82.63999939]
 [81.90000153 81.62999725 82.84999847 82.76999664]
 [81.65000153 80.91000366 81.94000244 80.93000031]
 [80.93000031 76.59999847 81.16000366 76.80999756]
 [77.30999756 76.80000305 78.19000244 77.41999817]
 [79.44000244 77.81999969 80.98000336 80.98000336]
 [79.19000244 78.06999969 79.80000305 78.69000244]
 [78.56999969 78.02999878 79.41000366 78.83000183]]

# 第一行的第一个元素
>>> print("A[0][0] =", A[0][0])  
A[0][0] = 80.69999694824219

# 第二行的第三个元素
>>> print("A[1][2] =", A[1][2])
A[1][2] = 82.8499984741211

# 最后一行的最后一个元素
>>> print("A[-1][-1] =", A[-1][-1])
A[-1][-1] = 78.83000183105469
```
<a name="mEhnp"></a>
#### 访问矩阵的行
```python
 # 第一行
>>> print("A[0] =", A[0])
A[0] = [80.69999695 80.09999847 82.63999939 82.63999939]

# 第三行
>>> print("A[2] =", A[2]) 
A[2] = [81.65000153 80.91000366 81.94000244 80.93000031]

# 最后一行
>>> print("A[-1] =", A[-1]) 
A[-1] = [78.56999969 78.02999878 79.41000366 78.83000183]
```
<a name="FE3F6"></a>
#### 访问矩阵的列
```python
 # 第一列
>>> print("A[:,0] =",A[:,0])
A[:,0] = [80.69999695 81.90000153 81.65000153 80.93000031 77.30999756 79.44000244 79.19000244 78.56999969]

# 第四列
>>> print("A[:,3] =", A[:,3]) 
A[:,3] = [82.63999939 82.76999664 80.93000031 76.80999756 77.41999817 80.98000336 78.69000244 78.83000183]

# 最后一列
>>> print("A[:,-1] =", A[:,-1]) 
A[:,-1] = [82.63999939 82.76999664 80.93000031 76.80999756 77.41999817 80.98000336 78.69000244 78.83000183]
```
<a name="kpwbe"></a>
#### 访问多维矩阵多个元素
```python
# 没有逗号相隔，默认获取行
# 第3到第5个元素
>>> print(A[2:5])    
[[81.65000153 80.91000366 81.94000244 80.93000031]
 [80.93000031 76.59999847 81.16000366 76.80999756]
 [77.30999756 76.80000305 78.19000244 77.41999817]]

# 第1到第3个元素
>>> print(A[:-5])
[[80.69999695 80.09999847 82.63999939 82.63999939]
 [81.90000153 81.62999725 82.84999847 82.76999664]
 [81.65000153 80.91000366 81.94000244 80.93000031]]

# 第6行到最后一个元素
>>> print(A[5:])  
[[79.44000244 77.81999969 80.98000336 80.98000336]
 [79.19000244 78.06999969 79.80000305 78.69000244]
 [78.56999969 78.02999878 79.41000366 78.83000183]]

# 第1个到最后一个元素
>>> print(A[:]) 

[[80.69999695 80.09999847 82.63999939 82.63999939]
 [81.90000153 81.62999725 82.84999847 82.76999664]
 [81.65000153 80.91000366 81.94000244 80.93000031]
 [80.93000031 76.59999847 81.16000366 76.80999756]
 [77.30999756 76.80000305 78.19000244 77.41999817]
 [79.44000244 77.81999969 80.98000336 80.98000336]
 [79.19000244 78.06999969 79.80000305 78.69000244]
 [78.56999969 78.02999878 79.41000366 78.83000183]]

# 反转一个列表
>>> print(A[::-1]) 
[[78.56999969 78.02999878 79.41000366 78.83000183]
 [79.19000244 78.06999969 79.80000305 78.69000244]
 [79.44000244 77.81999969 80.98000336 80.98000336]
 [77.30999756 76.80000305 78.19000244 77.41999817]
 [80.93000031 76.59999847 81.16000366 76.80999756]
 [81.65000153 80.91000366 81.94000244 80.93000031]
 [81.90000153 81.62999725 82.84999847 82.76999664]
 [80.69999695 80.09999847 82.63999939 82.63999939]]

# 前2行，前4列
>>> print(A[:2, :4]) 
[[80.69999695 80.09999847 82.63999939 82.63999939]
 [81.90000153 81.62999725 82.84999847 82.76999664]]

# 前1行，所有列
>>> print(A[:1,])  
[[80.69999695 80.09999847 82.63999939 82.63999939]]

# 所有行，第3列
>>> print(A[:,2])  
[82.63999939 82.84999847 81.94000244 81.16000366 78.19000244 80.98000336 79.80000305 79.41000366]

# 所有行，第3到5列
>>> print(A[:, 2:5])  # 共4列，只能取到第3和第4列
[[82.63999939 82.63999939]
 [82.84999847 82.76999664]
 [81.94000244 80.93000031]
 [81.16000366 76.80999756]
 [78.19000244 77.41999817]
 [80.98000336 80.98000336]
 [79.80000305 78.69000244]
 [79.41000366 78.83000183]]
```
