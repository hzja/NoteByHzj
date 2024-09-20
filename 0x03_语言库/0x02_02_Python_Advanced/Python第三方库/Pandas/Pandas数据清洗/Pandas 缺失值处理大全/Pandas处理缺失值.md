Python Pandas<br />pandas对象的所有描述性统计信息默认情况下是排除缺失值的。<br />pandas对象中表现缺失值的方式并不完美，但是它对大部分用户来说是有用的。对于数值型数据，pandas使用浮点值NaN（Not a Number来表示缺失值）。称NaN为容易检测到的标识值：<br />In :
```python
string_data = pd.Series(['aardvark', 'artichoke', np.nan, 'avocado'])

string_data
```
Out:
```python
0      aardvark
1     artichoke
2            NaN
3       avocado
dtype: object
```
In:
```python
string_data.isnull()
```
Out:
```python
0     False
1     False
2      True
3     False
dtype: bool
```
在pandas中，采用了R语言中的编程惯例，将缺失值成为NA，意思是not available（不可用）。在统计学应用中，NA数据可以是不存在的数据或者是存在但不可观察的数据（例如在数据收集过程中出现了问题）。**当清洗数据用于分析时，对缺失数据本身进行分析以确定数据收集问题或数据丢失导致的数据偏差通常很重要。**<br />Python内建的None值在对象数组中也被当作NA处理：<br />In:
```python
string_data[0] = None

string_data.isnull()
```
Out:
```python
0      True
1     False
2      True
3     False
dtype: bool
```
pandas项目持续改善处理缺失值的内部细节，但是用户API函数，比如`pandas.isnull`，抽象掉了很多令人厌烦的细节。处理缺失值的相关函数列表如下：

- `dropna`：根据每个标签的值是否是缺失数据来筛选轴标签，并根据允许丢失的数据量来确定阈值
- `fillna`：用某些值填充缺失的数据或使用插值方法(如“ffill”或“bfill”)。
- `isnull`：返回表明哪些值是缺失值的布尔值
- `notnull`：`isnull`的反作用函数
<a name="ANBZ7"></a>
## 1、过滤缺失值
有多种**过滤**缺失值的方法。虽然可以使用`pandas.isnull`和布尔值索引手动地过滤缺失值，但`dropna`在过滤缺失值时是非常有用的。
<a name="Xak95"></a>
### 在Series上使用`dropna`，它会返回Series中所有的非空数据及其索引值
In:
```python
from numpy import nan as NA
data = pd.Series([1, NA, 3.5, NA, 7])
data.dropna()
```
Out:
```python
0     1.0
2     3.5
4     7.0
dtype: float64
```
上面的例子与下面的代码是等价的：<br />In:
```python
data[data.notnull()]
```
Out:
```python
0     1.0
2     3.5
4     7.0
dtype: float64
```
当处理DataFrame对象时，事情会稍微更复杂一点。可能想要删除全部为NA或包含有NA的行或列。
<a name="cmo6b"></a>
### `dropna`默认情况下会删除包含缺失值的行
In:
```python
data = pd.DataFrame([[1., 6.5, 3.], [1., NA, NA]
                     [NA, NA, NA], [NA, 6.5, 3.]])
cleaned = data.dropna()
data
```
Out:
```python
   0     1     2
0  1.0  6.5  3.0
1  1.0  NaN  NaN
2  NaN  NaN  NaN
3  NaN  6.5  3.0
```
In:
```python
cleaned
```
Out:
```python
   0     1     2
0  1.0  6.5  3.0
```
<a name="m4WEm"></a>
### 传入`how='all'`时，将删除所有值均为NA的行
In:
```python
data.dropna(how='all')
```
Out:
```python
     0    1    2
0  1.0  6.5  3.0
1  1.0  NaN  NaN
3  NaN  6.5  3.0
```
<a name="PrXiH"></a>
### 如果要用同样的方式去删除列，传入参数`axis=1`
In:
```python
data[4] = NA
data
```
Out:
```python
     0    1    2   4
0  1.0  6.5  3.0 NaN
1  1.0  NaN  NaN NaN
2  NaN  NaN  NaN NaN
3  NaN  6.5  3.0 NaN
```
In:
```python
data.dropna(axis=1, how='all')
```
Out:
```python
     0    1    2
0  1.0  6.5  3.0
1  1.0  NaN  NaN
2  NaN  NaN  NaN
3  NaN  6.5  3.0
```
过滤DataFrame的行的相关方法往往涉及时间序列数据。假设只想保留包含一定数量的观察值的行。可以用`thresh`参数来表示：<br />In:
```python
df = pd.DataFrame(np.random.randn(7, 3))

df.iloc[:4, 1] = NA

df.iloc[:2, 2] = NA

df
```
Out:
```python
          0         1         2
0 -0.204708       NaN       NaN
1 -0.555730       NaN       NaN
2  0.092908       NaN  0.769023
3  1.246435       NaN -1.296221
4  0.274992  0.228913  1.352917
5  0.886429 -2.001637 -0.371843
6  1.669025 -0.438570 -0.539741
```
In:
```python
df.dropna()
```
Out:
```python
         0         1         2
4 0.274992  0.228913  1.352917
5 0.886429 -2.001637 -0.371843
6 1.669025 -0.438570 -0.539741
```
In:
```python
df.dropna(thresh=2)
```
Out:
```python
         0         1         2
2 0.092908       NaN  0.769023
3 1.246435       NaN -1.296221
4 0.274992  0.228913  1.352917
5 0.886429 -2.001637 -0.371843
6 1.669025 -0.438570 -0.539741
```
<a name="aSpf3"></a>
## 2、补全缺失值
有时可能需要以多种方式**补全**“漏洞”，而不是过滤缺失值（也可能丢弃其他数据）。<br />大多数情况下，主要使用`fillna`方法来补全缺失值。
<a name="pIaRN"></a>
### 调用`fillna`时，可以使用一个常数来替代缺失值
In:
```python
df.fillna(0)
```
Out:
```python
          0         1         2
0 -0.204708  0.000000  0.000000
1 -0.555730  0.000000  0.000000
2  0.092908  0.000000  0.769023
3  1.246435  0.000000 -1.296221
4  0.274992  0.228913  1.352917
5  0.886429 -2.001637 -0.371843
6  1.669025 -0.438570 -0.539741
```
<a name="IX1BC"></a>
### 在调用`fillna`时使用字典，可以为不同列设定不同的填充值
In:
```python
df.fillna({1: 0.5, 2: 0})
```
Out:
```python
          0         1         2
0 -0.204708  0.500000  0.000000
1 -0.555730  0.500000  0.000000
2  0.092908  0.500000  0.769023
3  1.246435  0.500000 -1.296221
4  0.274992  0.228913  1.352917
5  0.886429 -2.001637 -0.371843
6  1.669025 -0.438570 -0.539741
```
<a name="RAnrm"></a>
### `fillna`返回的是一个新的对象，但也可以修改已经存在的对象
In:
```python
_ = df.fillna(0, inplace=True)
df
```
Out:
```python
          0         1         2
0 -0.204708  0.000000  0.000000
1 -0.555730  0.000000  0.000000
2  0.092908  0.000000  0.769023
3  1.246435  0.000000 -1.296221
4  0.274992  0.228913  1.352917
5  0.886429 -2.001637 -0.371843
6  1.669025 -0.438570 -0.539741
```
<a name="OFrsq"></a>
### 用于重建索引的相同的插值方法也可以用于`fillna`
In:
```python
df = pd.DataFrame(np.random.randn(6, 3))

df.iloc[2:, 1] = NA

df.iloc[4:, 2] = NA

df
```
Out:
```python
          0         1         2
0  0.476985  3.248944 -1.021228
1 -0.577087  0.124121  0.302614
2  0.523772       NaN  1.343810
3 -0.713544       NaN -2.370232
4 -1.860761       NaN       NaN
5 -1.265934       NaN       NaN
```
In:
```python
df.fillna(method='ffill')
```
Out:
```python
          0         1         2
0  0.476985  3.248944 -1.021228
1 -0.577087  0.124121  0.302614
2  0.523772  0.124121  1.343810
3 -0.713544  0.124121 -2.370232
4 -1.860761  0.124121 -2.370232
5 -1.265934  0.124121 -2.370232
```
In:
```python
df.fillna(method='ffill', limit=2)
```
Out:
```python
          0         1         2
0  0.476985  3.248944 -1.021228
1 -0.577087  0.124121  0.302614
2  0.523772  0.124121  1.343810
3 -0.713544  0.124121 -2.370232
4 -1.860761       NaN -2.370232
5 -1.265934       NaN -2.370232
```
<a name="S4h96"></a>
### 使用`fillna`可以完成很多带有一点创造性的工作。
例如，可以将Series的平均值或中位数用于填充缺失值：<br />In:
```python
data = pd.Series([1., NA, 3.5, NA, 7])
data.fillna(data.mean())
```
Out:
```python
0     1.000000
1     3.833333
2     3.500000
3     3.833333
4     7.000000
dtype: float64
```
以下是`fillna`的函数参数。

- `value`：标量值或字典型对象用于填充缺失值
- `method`：插值方法，如果没有其他参数，默认是'ffill'
- `axis`：需要填充的轴，默认`axis=0`
- `inplace`：修改被调用的对象，而不是生成一个备份
- `limit`：用于前向或后向填充时最大的填充范围
