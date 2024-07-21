Python Pandas
<a name="YMnRB"></a>
## 1. `concat`
`concat`是`pandas`中专门用于数据连接合并的函数，功能非常强大，支持纵向合并和横向合并，默认情况下是纵向合并，具体可以通过参数进行设置。
```python
pd.concat(
    objs: 'Iterable[NDFrame] | Mapping[Hashable, NDFrame]',
    axis=0,
    join='outer',
    ignore_index: 'bool' = False,
    keys=None,
    levels=None,
    names=None,
    verify_integrity: 'bool' = False,
    sort: 'bool' = False,
    copy: 'bool' = True,
) -> 'FrameOrSeriesUnion'
```
在函数方法中，各参数含义如下：<br />`objs`: 用于连接的数据，可以是`DataFrame`或`Series`组成的列表<br />`axis=0` : 连接的方式，默认为0也就是纵向连接，可选 1 为横向连接<br />`join='outer'`：合并方式，默认为 `inner`也就是交集，可选 `outer` 为并集<br />`ignore_index`: 是否保留原有的索引<br />`keys=None`：连接关系，使用传递的值作为一级索引<br />`levels=None`：用于构造多级索引<br />`names=None`：索引的名称<br />`verify_integrity`: 检测索引是否重复，如果为True则有重复索引会报错<br />`sort`: 并集合并方式下，对columns排序<br />`copy`: 是否深度拷贝<br />接下来，就对该函数功能进行演示
<a name="GEZtV"></a>
### 基础连接
```python
In [1]: import pandas as pd

In [2]: s1 = pd.Series(['a', 'b'])

In [3]: s2 = pd.Series(['c', 'd'])

In [4]: s1
Out[4]: 
0    a
1    b
dtype: object

In [5]: s2
Out[5]: 
0    c
1    d
dtype: object

In [6]: pd.concat([s1, s2])
Out[6]: 
0    a
1    b
0    c
1    d
dtype: object

In [7]: df1 = pd.DataFrame([['a', 1], ['b', 2]],
   ...:                     columns=['letter', 'number'])

In [8]: df2 = pd.DataFrame([['c', 3], ['d', 4]],
   ...:                     columns=['letter', 'number'])

In [9]: pd.concat([df1, df2])
Out[9]: 
  letter  number
0      a       1
1      b       2
0      c       3
1      d       4
```
<a name="EmAi6"></a>
### 横向连接
```python
In [10]: pd.concat([df1, df2], axis=1)
Out[10]: 
  letter  number letter  number
0      a       1      c       3
1      b       2      d       4
```
默认情况下，`concat`是取并集，如果两个数据中有个数据没有对应行或列，则会填充为空值`NaN`。
<a name="Jh51O"></a>
### 合并交集
```python
In [11]: df3 = pd.DataFrame([['c', 3, 'cat'], ['d', 4, 'dog']],
    ...:                     columns=['letter', 'number', 'animal'])

In [12]: df1
Out[12]: 
  letter  number
0      a       1
1      b       2

In [13]: df3
Out[13]: 
  letter  number animal
0      c       3    cat
1      d       4    dog

In [14]: pd.concat([df1, df3], join='inner')
Out[14]: 
  letter  number
0      a       1
1      b       2
0      c       3
1      d       4
```
<a name="PtZat"></a>
### 索引重置（不保留原有索引）
```python
In [15]: pd.concat([df1, df3], join='inner', ignore_index=True)
Out[15]: 
  letter  number
0      a       1
1      b       2
2      c       3
3      d       4
# 以下方式和上述的输出结果等价
In [16]: pd.concat([df1, df3], join='inner').reset_index(drop=True)
Out[16]: 
  letter  number
0      a       1
1      b       2
2      c       3
3      d       4
```
<a name="InJFg"></a>
### 指定索引
```python
In [17]: pd.concat([df1, df3], keys=['df1','df3'])
Out[17]: 
      letter  number animal
df1 0      a       1    NaN
    1      b       2    NaN
df3 0      c       3    cat
    1      d       4    dog

In [18]: pd.concat([df1, df3], keys=['df1','df3'], names=['df名称','行ID'])
Out[18]: 
         letter  number animal
df名称 行ID                      
df1  0        a       1    NaN
     1        b       2    NaN
df3  0        c       3    cat
     1        d       4    dog
```
<a name="UmcSz"></a>
### 检测重复
如果索引出现重复，则无法通过检测，会报错
```python
In [19]: pd.concat([df1, df3], verify_integrity=True)
Traceback (most recent call last):
...
ValueError: Indexes have overlapping values: Int64Index([0, 1], dtype='int64')
```
<a name="OrIOk"></a>
### 合并并集下`columns`排序
```python
In [21]: pd.concat([df1, df3], sort=True)
Out[21]: 
  animal letter  number
0    NaN      a       1
1    NaN      b       2
0    cat      c       3
1    dog      d       4
```
`DataFrame`与`Series`合并
```python
In [22]: pd.concat([df1, s1])
Out[22]: 
  letter  number    0
0      a     1.0  NaN
1      b     2.0  NaN
0    NaN     NaN    a
1    NaN     NaN    b

In [23]: pd.concat([df1, s1], axis=1)
Out[23]: 
  letter  number  0
0      a       1  a
1      b       2  b
# 新增列一般可选以下两种方式
In [24]: df1.assign(新增列=s1)
Out[24]: 
  letter  number 新增列
0      a       1   a
1      b       2   b

In [25]: df1['新增列'] = s1

In [26]: df1
Out[26]: 
  letter  number 新增列
0      a       1   a
1      b       2   b
```
以上就`concat`函数方法的一些功能，相比之下，另外一个函数`append`也可以用于数据追加（纵向合并）
<a name="o7dyv"></a>
## 2. `append`
`append`主要用于追加数据，是比较简单直接的数据合并方式。
```python
df.append(
    other,
    ignore_index: 'bool' = False,
    verify_integrity: 'bool' = False,
    sort: 'bool' = False,
) -> 'DataFrame'
```
在函数方法中，各参数含义如下：<br />`other`: 用于追加的数据，可以是`DataFrame`或`Series`或组成的列表<br />`ignore_index`: 是否保留原有的索引<br />`verify_integrity`: 检测索引是否重复，如果为True则有重复索引会报错<br />`sort`: 并集合并方式下，对columns排序<br />接下来，就对该函数功能进行演示
<a name="Y7wm1"></a>
### 基础追加
```python
In [41]: df1.append(df2)
Out[41]: 
  letter  number
0      a       1
1      b       2
0      c       3
1      d       4

In [42]: df1.append([df1,df2,df3])
Out[42]: 
  letter  number animal
0      a       1    NaN
1      b       2    NaN
0      a       1    NaN
1      b       2    NaN
0      c       3    NaN
1      d       4    NaN
0      c       3    cat
1      d       4    dog
```
`columns`重置（不保留原有索引）
```python
In [43]: df1.append([df1,df2,df3], ignore_index=True)
Out[43]: 
  letter  number animal
0      a       1    NaN
1      b       2    NaN
2      a       1    NaN
3      b       2    NaN
4      c       3    NaN
5      d       4    NaN
6      c       3    cat
7      d       4    dog
```
<a name="oh8LY"></a>
### 检测重复
如果索引出现重复，则无法通过检测，会报错
```python
In [44]: df1.append([df1,df2], verify_integrity=True)
Traceback (most recent call last):
...
ValueError: Indexes have overlapping values: Int64Index([0, 1], dtype='int64')
```
<a name="f8ZgY"></a>
### 索引排序
```python
In [46]: df1.append([df1,df2,df3], sort=True)
Out[46]: 
  animal letter  number
0    NaN      a       1
1    NaN      b       2
0    NaN      a       1
1    NaN      b       2
0    NaN      c       3
1    NaN      d       4
0    cat      c       3
1    dog      d       4
```
<a name="P09bi"></a>
### 追加Series
```python
In [49]: s = pd.Series({'letter':'s1','number':9})

In [50]: s
Out[50]: 
letter    s1
number     9
dtype: object

In [51]: df1.append(s)
Traceback (most recent call last):
...
TypeError: Can only append a Series if ignore_index=True or if the Series has a name

In [53]: df1.append(s, ignore_index=True)
Out[53]: 
  letter  number
0      a       1
1      b       2
2     s1       9
```
<a name="mmQTO"></a>
### 追加字典
这个在爬虫的时候比较好使，每爬取一条数据就合并到`DataFrame`类似数据中存储起来
```python
In [54]: dic = {'letter':'s1','number':9}

In [55]: df1.append(dic, ignore_index=True)
Out[55]: 
  letter  number
0      a       1
1      b       2
2     s1       9
```
<a name="YWlK3"></a>
## 3. `merge`
`merge`函数方法类似SQL里的`join`，可以是`pd.merge`或者`df.merge`，区别就在于后者待合并的数据是
```python
pd.merge(
    left: 'DataFrame | Series',
    right: 'DataFrame | Series',
    how: 'str' = 'inner',
    on: 'IndexLabel | None' = None,
    left_on: 'IndexLabel | None' = None,
    right_on: 'IndexLabel | None' = None,
    left_index: 'bool' = False,
    right_index: 'bool' = False,
    sort: 'bool' = False,
    suffixes: 'Suffixes' = ('_x', '_y'),
    copy: 'bool' = True,
    indicator: 'bool' = False,
    validate: 'str | None' = None,
) -> 'DataFrame'
```
在函数方法中，关键参数含义如下：<br />`left`: 用于连接的左侧数据<br />`right`: 用于连接的右侧数据<br />`how`: 数据连接方式，默认为 `inner`，可选`outer`、`left`和`right`<br />`on`: 连接关键字段，左右侧数据中需要都存在，否则就用`left_on`和`right_on`<br />`left_on`: 左侧数据用于连接的关键字段<br />`right_on`: 右侧数据用于连接的关键字段<br />`left_index`: True表示左侧索引为连接关键字段<br />`right_index`: True表示右侧索引为连接关键字段<br />`suffixes`: `'Suffixes' = ('_x', '_y')`,可以自由指定，就是同列名合并后列名显示后缀<br />`indicator`: 是否显示合并后某行数据的归属来源<br />接下来，就对该函数功能进行演示
<a name="bkWZP"></a>
### 基础合并
```python
In [55]: df1 = pd.DataFrame({'key': ['foo', 'bar', 'bal'],
    ...:                     'value2': [1, 2, 3]})

In [56]: df2 = pd.DataFrame({'key': ['foo', 'bar', 'baz'],
    ...:                     'value1': [5, 6, 7]})

In [57]: df1.merge(df2)
Out[57]: 
   key  value2  value1
0  foo       1       5
1  bar       2       6
```
<a name="ovoMk"></a>
### 其他连接方式
```python
In [58]: df1.merge(df2, how='left')
Out[58]: 
   key  value2  value1
0  foo       1     5.0
1  bar       2     6.0
2  bal       3     NaN

In [59]: df1.merge(df2, how='right')
Out[59]: 
   key  value2  value1
0  foo     1.0       5
1  bar     2.0       6
2  baz     NaN       7

In [60]: df1.merge(df2, how='outer')
Out[60]: 
   key  value2  value1
0  foo     1.0     5.0
1  bar     2.0     6.0
2  bal     3.0     NaN
3  baz     NaN     7.0

In [61]: df1.merge(df2, how='cross')
Out[61]: 
  key_x  value2 key_y  value1
0   foo       1   foo       5
1   foo       1   bar       6
2   foo       1   baz       7
3   bar       2   foo       5
4   bar       2   bar       6
5   bar       2   baz       7
6   bal       3   foo       5
7   bal       3   bar       6
8   bal       3   baz       7
```
<a name="eDEkY"></a>
### 指定连接键
可以指定单个连接键，也可以指定多个连接键
```python
In [62]: df1 = pd.DataFrame({'lkey1': ['foo', 'bar', 'bal'],
    ...:                     'lkey2': ['a', 'b', 'c'],
    ...:                     'value2': [1, 2, 3]})

In [63]: df2 = pd.DataFrame({'rkey1': ['foo', 'bar', 'baz'],
    ...:                     'rkey2': ['a', 'b', 'c'],
    ...:                     'value2': [5, 6, 7]})
    
In [64]: df1
Out[64]: 
  lkey1 lkey2  value2
0   foo     a       1
1   bar     b       2
2   bal     c       3

In [65]: df2
Out[65]: 
  rkey1 rkey2  value2
0   foo     a       5
1   bar     b       6
2   baz     c       7

In [66]: df1.merge(df2, left_on='lkey1', right_on='rkey1')
Out[66]: 
  lkey1 lkey2  value2_x rkey1 rkey2  value2_y
0   foo     a         1   foo     a         5
1   bar     b         2   bar     b         6

In [67]: df1.merge(df2, left_on=['lkey1','lkey2'], right_on=['rkey1','rkey2'])
Out[67]: 
  lkey1 lkey2  value2_x rkey1 rkey2  value2_y
0   foo     a         1   foo     a         5
1   bar     b         2   bar     b         6
```
<a name="DCLr2"></a>
### 指定索引为键
```python
Out[68]: df1.merge(df2, left_index=True, right_index=True)
Out[68]: 
  lkey1 lkey2  value2_x rkey1 rkey2  value2_y
0   foo     a         1   foo     a         5
1   bar     b         2   bar     b         6
2   bal     c         3   baz     c         7
```
<a name="YdJbA"></a>
### 设置重复列后缀
```python
In [69]: df1.merge(df2, left_on='lkey1', right_on='rkey1', suffixes=['左','右'])
Out[69]: 
  lkey1 lkey2  value2左 rkey1 rkey2  value2右
0   foo     a        1   foo     a        5
1   bar     b        2   bar     b        6
```
<a name="ULLVe"></a>
### 连接指示
新增一列用于显示数据来源
```python
In [70]: df1.merge(df2, left_on='lkey1', right_on='rkey1', suffixes=['左','右'], how='outer',
    ...:           indicator=True
    ...:       )
Out[70]: 
  lkey1 lkey2  value2左 rkey1 rkey2  value2右      _merge
0   foo     a      1.0   foo     a      5.0        both
1   bar     b      2.0   bar     b      6.0        both
2   bal     c      3.0   NaN   NaN      NaN   left_only
3   NaN   NaN      NaN   baz     c      7.0  right_only
```
<a name="uKEXs"></a>
## 4. `join`
`join`就有点想`append`之于`concat`，用于数据合并
```python
df.join(
    other: 'FrameOrSeriesUnion',
    on: 'IndexLabel | None' = None,
    how: 'str' = 'left',
    lsuffix: 'str' = '',
    rsuffix: 'str' = '',
    sort: 'bool' = False,
) -> 'DataFrame'
```
在函数方法中，关键参数含义如下：<br />`other`: 用于合并的右侧数据<br />`on`: 连接关键字段，左右侧数据中需要都存在，否则就用`left_on`和`right_on`<br />`how`: 数据连接方式，默认为 `inner`，可选`outer`、`left`和`right`<br />`lsuffix`: 左侧同名列后缀<br />`rsuffix`：右侧同名列后缀<br />接下来，就对该函数功能进行演示
```python
In [71]: df = pd.DataFrame({'key': ['K0', 'K1', 'K2', 'K3', 'K4', 'K5'],
    ...:                     'A': ['A0', 'A1', 'A2', 'A3', 'A4', 'A5']})

In [72]: other = pd.DataFrame({'key': ['K0', 'K1', 'K2'],
    ...:                        'B': ['B0', 'B1', 'B2']})

In [73]: df
Out[73]: 
  key   A
0  K0  A0
1  K1  A1
2  K2  A2
3  K3  A3
4  K4  A4
5  K5  A5

In [74]: other
Out[74]: 
  key   B
0  K0  B0
1  K1  B1
2  K2  B2

In [75]: df.join(other, on='key')
Traceback (most recent call last):
...
ValueError: You are trying to merge on object and int64 columns. If you wish to proceed you should use pd.concat
```
如果想用key关键字， 则需要key是索引。
<a name="Mh24L"></a>
### 指定key
```python
In [76]: df.set_index('key').join(other.set_index('key'))
Out[76]: 
      A    B
key         
K0   A0   B0
K1   A1   B1
K2   A2   B2
K3   A3  NaN
K4   A4  NaN
K5   A5  NaN

In [77]: df.join(other.set_index('key'), on='key')
Out[77]: 
  key   A    B
0  K0  A0   B0
1  K1  A1   B1
2  K2  A2   B2
3  K3  A3  NaN
4  K4  A4  NaN
5  K5  A5  NaN
```
<a name="Fwfvv"></a>
### 指定重复列后缀
```python
In [78]: df.join(other, lsuffix='_左', rsuffix='右')
Out[78]: 
  key_左   A key右    B
0    K0  A0   K0   B0
1    K1  A1   K1   B1
2    K2  A2   K2   B2
3    K3  A3  NaN  NaN
4    K4  A4  NaN  NaN
5    K5  A5  NaN  NaN
```
其他参数就不多做介绍了，和`merge`基本一样。
<a name="AlYST"></a>
## 5. `combine`
在数据合并的过程中，可能需要对对应位置的值进行一定的计算，pandas提供了`combine`和`combine_first`函数方法来进行这方面的合作操作。
```python
df.combine(
    other: 'DataFrame',
    func,
    fill_value=None,
    overwrite: 'bool' = True,
) -> 'DataFrame'
```
比如，数据合并的时候取单元格最小的值
```python
In [79]: df1 = pd.DataFrame({'A': [0, 0], 'B': [4, 4]})

In [80]: df2 = pd.DataFrame({'A': [1, 1], 'B': [3, 3]})

In [81]: df1
Out[81]: 
   A  B
0  0  4
1  0  4

In [82]: df2
Out[82]: 
   A  B
0  1  3
1  1  3

In [83]: take_smaller = lambda s1, s2: s1 if s1.sum() < s2.sum() else s2

In [84]: df1.combine(df2, take_smaller)
Out[84]: 
   A  B
0  0  3
1  0  3

# 也可以调用numpy的函数
In [85]: import numpy as np

In [86]: df1.combine(df2, np.minimum)
Out[86]: 
   A  B
0  0  3
1  0  3
```
<a name="oxAIB"></a>
### `fill_value`填充缺失值
```python
In [87]: df1 = pd.DataFrame({'A': [0, 0], 'B': [None, 4]})

In [87]: df2 = pd.DataFrame({'A': [1, 1], 'B': [3, 3]})

In [88]: df1
Out[88]: 
   A    B
0  0  NaN
1  0  4.0

In [89]: df2
Out[89]: 
   A  B
0  1  3
1  1  3

In [90]: df1.combine(df2, take_smaller, fill_value=-88)
Out[90]: 
   A     B
0  0 -88.0
1  0   4.0
```
<a name="uryuy"></a>
### `overwrite=False`保留
```python
In [91]: df1 = pd.DataFrame({'A': [0, 0], 'B': [4, 4]})

In [92]: df2 = pd.DataFrame({'B': [3, 3], 'C': [-10, 1], }, index=[1, 2])

In [93]: df1
Out[93]: 
   A  B
0  0  4
1  0  4

In [94]: df2
Out[94]: 
   B   C
1  3 -10
2  3   1

In [95]: df1.combine(df2, take_smaller)
Out[95]: 
    A    B     C
0 NaN  NaN   NaN
1 NaN  3.0 -10.0
2 NaN  3.0   1.0
# 保留A列原有的值
In [96]: df1.combine(df2, take_smaller, overwrite=False)
Out[96]: 
     A    B     C
0  0.0  NaN   NaN
1  0.0  3.0 -10.0
2  NaN  3.0   1.0
```
<a name="hKYwz"></a>
### 另外一个`combine_first`
```python
df.combine_first(other: 'DataFrame') -> 'DataFrame'
```
当df中元素为空采用other里的进行替换，结果为并集合并
```python
In [97]: df1 = pd.DataFrame({'A': [None, 0], 'B': [None, 4]})

In [98]: df2 = pd.DataFrame({'A': [1, 1], 'B': [3, 3]})

In [99]: df1
Out[99]: 
     A    B
0  NaN  NaN
1  0.0  4.0

In [100]: df2
Out[100]: 
   A  B
0  1  3
1  1  3

In [101]: df1.combine_first(df2)
Out[101]: 
     A    B
0  1.0  3.0
1  0.0  4.0

In [102]: df1 = pd.DataFrame({'A': [None, 0], 'B': [4, None]})

In [103]: df2 = pd.DataFrame({'B': [3, 3], 'C': [1, 1]}, index=[1, 2])

In [104]: df1
Out[104]: 
     A    B
0  NaN  4.0
1  0.0  NaN

In [105]: df2
Out[105]: 
   B  C
1  3  1
2  3  1

In [106]: df1.combine_first(df2)
Out[106]: 
     A    B    C
0  NaN  4.0  NaN
1  0.0  3.0  1.0
2  NaN  3.0  1.0
```
<a name="SLoor"></a>
## 总结
以上就本次介绍的关于Pandas数据合并的全部内容，相比之下可以发现：

- `append`主要用于纵向追加数据，比较简单直接；
- `concat`功能最强大，不仅可以纵向合并数据还可以横向合并数据而且支持很多其他条件设置；
- `merge`则主要用于横向合并数据，类似SQL里的join连接；
- `join`则比较简单，用于横向合并数据，条件相对苛刻；
- `combine`更像是按照元素进行合并，根据一定的条件（函数规则）来进行数据合并。
