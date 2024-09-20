PythonPandas<br />虽然目前dask，cudf等包的出现，使得数据处理大大得到了加速，但是并不是每个人都有比较好的gpu，非常多的朋友仍然还在使用pandas工具包，但有时候真的很无奈，pandas的许多问题都需要使用`apply`函数来进行处理，而`apply`函数是非常慢的，这里就介绍如何加速`apply`函数600倍的技巧。
<a name="aNPrw"></a>
## 实验对比
<a name="laE7s"></a>
### 1、Apply(Baseline)
以Apply为例，原始的Apply函数处理下面这个问题，需要**18.4s**的时间。
```python
import pandas as pd
import numpy as np
df = pd.DataFrame(np.random.randint(0, 11, size=(1000000, 5)), columns=('a','b','c','d','e'))
def func(a,b,c,d,e):
    if e == 10:
        return c*d
    elif (e < 10) and (e>=5):
        return c+d
    elif e < 5:
        return a+b
%%time
df['new'] = df.apply(lambda x: func(x['a'], x['b'], x['c'], x['d'], x['e']), axis=1)
CPU times: user 17.9 s, sys: 301 ms, total: 18.2 s
Wall time: 18.4 s
```
<a name="wVy6Y"></a>
### 2、Swift加速
因为处理是并行的，所以可以使用Swift进行加速，在使用Swift之后，相同的操作在个人机器上可以提升到7.67s。
```python
%%time
# !pip install swifter
import swifter
df['new'] = df.swifter.apply(lambda x : func(x['a'],x['b'],x['c'],x['d'],x['e']),axis=1)
HBox(children=(HTML(value='Dask Apply'), FloatProgress(value=0.0, max=16.0), HTML(value='')))

CPU times: user 329 ms, sys: 240 ms, total: 569 ms
Wall time: 7.67 s
```
<a name="ei1dw"></a>
### 3、向量化
使用Pandas和Numpy的最快方法是将函数向量化。如果操作是可以直接向量化的话，那么就尽可能的避免使用：

- **for循环；**
- **列表处理；**
- **apply等操作**

在将上面的问题转化为下面的处理之后，时间缩短为：421 ms。
```python
%%time
df['new'] = df['c'] * df['d'] #default case e = =10
mask = df['e'] < 10
df.loc[mask,'new'] = df['c'] + df['d']
mask = df['e'] < 5
df.loc[mask,'new'] = df['a'] + df['b']
CPU times: user 134 ms, sys: 149 ms, total: 283 ms
Wall time: 421 ms
```
<a name="ZbMki"></a>
### 4、类别转化+向量化
先将上面的类别转化为int16型，再进行相同的向量化操作，发现时间缩短为：116 ms。
```python
for col in ('a','b','c','d'):
    df[col] = df[col].astype(np.int16) 
%%time
df['new'] = df['c'] * df['d'] #default case e = =10
mask = df['e'] < 10
df.loc[mask,'new'] = df['c'] + df['d']
mask = df['e'] < 5
df.loc[mask,'new'] = df['a'] + df['b']
CPU times: user 71.3 ms, sys: 42.5 ms, total: 114 ms
Wall time: 116 ms
```
<a name="Qj5Vg"></a>
### 5、转化为values处理
在能转化为`.values`的地方尽可能转化为`.values`，再进行操作。

- **此处先转化为**`**.values**`**等价于转化为numpy，这样向量化操作会更加快捷。**

于是，上面的操作时间又被缩短为：74.9ms。
```python
%%time
df['new'] = df['c'].values * df['d'].values #default case e = =10
mask = df['e'].values < 10
df.loc[mask,'new'] = df['c'] + df['d']
mask = df['e'].values < 5
df.loc[mask,'new'] = df['a'] + df['b']
CPU times: user 64.5 ms, sys: 12.5 ms, total: 77 ms
Wall time: 74.9 ms
```
<a name="RJJ4R"></a>
## 实验汇总
通过上面的一些小的技巧，将简单的Apply函数加速了几百倍，具体的：

- **Apply: 18.4 s**
- **Apply + Swifter: 7.67 s**
- **Pandas vectorizatoin: 421 ms**
- **Pandas vectorization + data types: 116 ms**
- **Pandas vectorization + values + data types: 74.9ms**
