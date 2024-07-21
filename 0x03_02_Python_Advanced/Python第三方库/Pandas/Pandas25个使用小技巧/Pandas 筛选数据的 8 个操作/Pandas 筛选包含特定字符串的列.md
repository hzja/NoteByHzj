PythonPandas<br />Pandas中字符串的操作是相当的灵活，内置了大量的相关方法用于字符串的加工处理，并且许多方法都是支持正则表达式的。<br />筛选包含特定字符串的列可以根据特定字符串在列名中的位置有这么几种情形：

- 这个特定字符串在列名首
- 这个特定字符串在列名尾
- 这个特定字符串在列名中
- 这个特定字符串在列名的任意位置上

筛选特定的列数据基本有两种思路：

- 使用pandas字符串方法+布尔索引
- 使用pandas提供的filter进行筛选
<a name="MkgQZ"></a>
## 使用pandas字符串方法+布尔索引
为处理字符串数据，Pandas通过`.str`提供了许多字符串处理方法：

- `contains` 判断某字符串中是否包含
- `startswith`
- `endswith`

这些方法均返回布尔值的序列，可用于布尔索引。
```python
import pandas as pd
data = pd.DataFrame({'A1':[1,2,3],'B1':[1,2,3],"1B11":[4,5,6],"11B":[4,3,7]})
data
```
| <br /> | A1 | B1 | 1B11 | 11B |
| --- | --- | --- | --- | --- |
| 0 | 1 | 1 | 4 | 4 |
| 1 | 2 | 2 | 5 | 3 |
| 2 | 3 | 3 | 6 | 7 |

<a name="LMIyd"></a>
### 这个特定字符串在列名的任意位置上
例如筛选列名中包含B的列，这时只需给`contains`传入字符串'B'即可得到布尔数组
```python
data.columns.str.contains('B')
array([False,  True,  True,  True])
```
使用上述布尔数组作为索引，筛选符合要求的列
```python
data.loc[:,data.columns.str.contains('B')]
```
| <br /> | B1 | 1B11 | 11B |
| --- | --- | --- | --- |
| 0 | 1 | 4 | 4 |
| 1 | 2 | 5 | 3 |
| 2 | 3 | 6 | 7 |

如果特定字符并不是某一具体的字符串，而是某一类的字符串，比如包含两个连续数字或其他特征，这时候也可以结合正则表达式来实现筛选。
```python
data.columns.str.contains('\d{2}',regex=True)
array([False, False,  True,  True])
```
regex=True 表示正在使用正则表达式 \d{2} 表示两个数字构成的字符串，使用上述代码得到相应的布尔数组，进行布尔索引即可。
```python
data.loc[:,data.columns.str.contains('\d{2}',regex=True)]
```
| <br /> | 1B11 | 11B |
| --- | --- | --- |
| 0 | 4 | 4 |
| 1 | 5 | 3 |
| 2 | 6 | 7 |

1B11 11B中均包含两个连续的数字，因此符合条件
<a name="aVaRA"></a>
### 这个特定字符在字符串的开头
比如要求这特定的字符必须在字符串的开头，可以使用`startswith`
```python
data.columns.str.startswith("B")
array([False,  True, False, False])
```
上述代码筛选以B开头的列
```python
data.loc[:,data.columns.str.startswith("B")]
```
| <br /> | B1 |
| --- | --- |
| 0 | 1 |
| 1 | 2 |
| 2 | 3 |

这样就筛选出了符合条件的列。如果不是固定的字符，而是某一类字符，那么这时候需要使用`contains+正则表达式的组合`，`startswith`不支持正则表达式。
```python
data.columns.str.contains("^\d{2}",regex=True)
array([False, False, False,  True])
```
正则表达式`"^\d{2}"`表示以两个数字开头的字符串
```python
data.loc[:,data.columns.str.contains("^\d{2}",regex=True)]
```
| <br /> | 11B |
| --- | --- |
| 0 | 4 |
| 1 | 3 |
| 2 | 7 |

<a name="ZozQF"></a>
### 这个特定字符在字符串的尾部
比如要求这特定的字符必须在字符串的尾部，可以使用`endswith`
```python
data.columns.str.endswith("B")
array([False, False, False,  True])
```
上述代码筛选以B结尾的列，得到相应的布尔数组，使用布尔数组筛选数据，代码如下：
```python
data.loc[:,data.columns.str.endswith("B")]
```
| <br /> | 11B |
| --- | --- |
| 0 | 4 |
| 1 | 3 |
| 2 | 7 |

这样就筛选出了符合条件的列。如果不是固定的字符，而是某一类字符，那么这时候需要使用`contains+正则表达式的组合`，`endswith`不支持正则表达式。
```python
data.columns.str.contains("\d{2}$",regex=True)
array([False, False,  True, False])
```
正则表达式`"\d{2}$"`表示以两个数字结尾的字符串
```python
data.loc[:,data.columns.str.contains("\d{2}$",regex=True)]
```
| <br /> | 1B11 |
| --- | --- |
| 0 | 4 |
| 1 | 5 |
| 2 | 6 |

<a name="qXWda"></a>
### 这个特定字符串在字符串的中部
这种需求需要使用正则表达式来去实现
```python
data.columns.str.contains("^[^B]+B[^B]+$",regex=True)
array([False, False,  True, False])
```
正则表达式`"^[^B]+B[^B]+$"`表示以非B字符开头、结尾且包含B的列名
```python
data.loc[:,data.columns.str.contains("^[^B]+B[^B]+$",regex=True)]
```
| <br /> | 1B11 |
| --- | --- |
| 0 | 4 |
| 1 | 5 |
| 2 | 6 |

当然在进行字符串处理的过程中除了使用字符串特定函数之外还可以使用map apply 再结合自定义函数实现更复杂的处理
<a name="n7eqS"></a>
## 使用pandas提供的`filter`进行筛选
Pandas 的 `filter` 方法根据指定的索引标签对数据框行或列查询子集。<br />DataFrame 使用时的语法为：
```python
df.filter(
    items=None,
    like: 'str | None' = None,
    regex: 'str | None' = None,
    axis=None,
) -> 'FrameOrSeries'
```
参数：

- items：list-like，对应轴的标签名列表
- like：str，支持对应标签名的模糊名查询
- regex：str (正则表达式)，按正则表达式查询标签名
- axis：`{0 or 'index', 1 or 'columns', None}, default None`，要筛选的轴，表示为索引（int）或轴名称（str）。默认情况下为列名，'index' 为 Series，`'columns'` 为 DataFrame

返回：<br />与输入对象类型相同<br />需要注意的是，此方法不会对数据帧的数据内容进行过滤，仅应用于按标签筛选。<br />其中的参数 items，like，和 regex parameters 被强制执行为相互排斥，即只能有一个存在。<br />筛选列名中间包括B的可以直接使用正则表达式筛选，代码如下：
```python
data.filter(regex="^[^B]+B[^B]+$",axis=1)
```
| <br /> | 1B11 |
| --- | --- |
| 0 | 4 |
| 1 | 5 |
| 2 | 6 |

筛选列名以两个数字结尾的列，代码如下：
```python
data.filter(regex="\d{2}$",axis=1)
```
| <br /> | 1B11 |
| --- | --- |
| 0 | 4 |
| 1 | 5 |
| 2 | 6 |

