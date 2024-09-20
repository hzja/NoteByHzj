Python Pandas
<a name="K6Nx1"></a>
## 选取有空值的行
在观察数据结构时，该方法可以快速定位存在缺失值的行。
```python
df = pd.DataFrame({'A': [0, 1, 2],
                   'B': [0, 1, None],
                   'C': [0, None, 2]})
df[df.isnull().T.any()]
```
输出：
```python
  A   B   C           A   B   C
0 0 0.0 0.0         1 1 1.0 NaN
1 1 1.0 NaN   -->   2 2 NaN 2.0
2 2 NaN 2.0
```
<a name="lyGyE"></a>
## 快速替换列值
实际数据处理经常会根据一些限定条件来替换列中的值。
```python
df = pd.DataFrame({'name':['Python', 'Java', 'C']})

# 第一种方式
df['name'].replace('Java', 'JavaScript', inplace=True)
# 第二种方式
df.loc[df['name'].str.contains('Java'), 'name'] = 'JavaScript'
```
输出：
```python
     name                   name
0  Python          0      Python
1    Java    --->  1  JavaScript
2       C          2           C
```
<a name="qZRwQ"></a>
## 对列进行分区
很多情况下，对于数值类型的数据，需要分区来计算每个区间数据出现的频率。这时用 `pd.cut` 就能很好的解决这一问题。
```python
import random

age = random.sample(range(90), 20)
cut_res = pd.cut(age, bins=[0, 18, 35, 60, 90])
# cut_res type：<class 'pandas.core.arrays.categorical.Categorical'>
cut_res.value_counts()
```
输出：
```python
(0, 18]     6
(18, 35]    1
(35, 60]    6
(60, 90]    7
```
<a name="xFtt6"></a>
## 将一列分为多列
在文本数据清洗时，一些列中存在分隔符('``', '`,`', '`:`')分隔的值，只需将该列根据分隔符进行 `split` 即可。
```python
import pandas as pd

df = pd.DataFrame({'address': ['四川省 成都市',
                               '湖北省 武汉市',
                               '浙江省 杭州市']})
res = df['address'].str.split(' ', expand=True)  
res.columns = ['province', 'city']
```
输出：
```python
  province city
0 四川省    成都市
1 湖北省    武汉市
2 浙江省    杭州市
```
`expand`参数选择是否扩展为 DataFrame，False 则返回 Series
<a name="ZoVTR"></a>
## 中文筛选
同样在清洗过程中，往往会出现一些不需要的中文字段，这时直接用 `str.contains` 筛选即可。
```python
df = pd.DataFrame({'mobile_phone':
                   ['15928765644',
                    '15567332235',
                    '暂无']})
df[~df['mobile_phone'].str.contains('[\u4e00-\u9fa5]')]
```
输出：
```python
  mobile_phone         mobile_phone
0 15928765644        0 15928765644
1 15567332235   -->  1 15567332235
2 暂无
```
<a name="uer2y"></a>
## 更改列的位置
有时需要调整列的位置，当数据列较少时，可以用下面的方式
```python
df = pd.DataFrame({'name': ['A', 'B', 'C'],
				   'age': [10, 20, 30],
				   'gender': [0, 1, 0]})
df = df[['name', 'gender', 'age']]
```
输出：
```python
 name age gender    name gender age
0   A  10 0        0   A 0       10
1   B  20 1   -->  1   B 1       20
2   C  30 0        2   C 0       30
```
如果列较多，那么，一个个列举出来会比较繁琐，推荐下面插入的方式。
```python
col = df['gender']
df.drop('gender', axis=1, inplace=True)
df.insert(1, 'gender', col)
```
