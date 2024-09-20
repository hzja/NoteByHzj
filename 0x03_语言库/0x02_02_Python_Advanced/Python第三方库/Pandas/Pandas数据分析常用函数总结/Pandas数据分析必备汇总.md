Python Pandas
<a name="cbRJz"></a>
## 一、Pandas两大数据结构的创建
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `pd.Series(对象,index=[ ])` | 创建Series。对象可以是列表\\ndarray、字典以及DataFrame中的某一行或某一列 |
| 2 | `pd.DataFrame(data,columns = [ ],index = [ ])` | 创建DataFrame。columns和index为指定的列、行索引，并按照顺序排列 |

举例：用pandas创建数据表：
```python
df = pd.DataFrame({"id":[1001,1002,1003,1004,1005,1006], 
                   "date":pd.date_range('20130102', periods=6),
                   "city":['Beijing ', 'SH', ' guangzhou ', 'Shenzhen', 'shanghai', 'BEIJING '],
                   "age":[23,44,54,32,34,32],
                   "category":['100-A','100-B','110-A','110-C','210-A','130-F'],
                   "price":[1200,np.nan,2133,5433,np.nan,4432]},
                  columns =['id','date','city','category','age','price'])
```
<a name="SuzyO"></a>
## 二、DataFrame常见方法
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `df.head()` | 查询数据的前五行 |
| 2 | `df.tail()` | 查询数据的末尾5行 |
| 3 | `pandas.qcut()` | 基于秩或基于样本分位数将变量离散化为等大小桶 |
| 4 | `pandas.cut()` | 基于分位数的离散化函数 |
| 5 | `pandas.date_range()` | 返回一个时间索引 |
| 6 | `df.apply()` | 沿相应轴应用函数 |
| 7 | `Series.value_counts()` | 返回不同数据的计数值 |
| 8 | `df.reset_index()` | 重新设置index，参数drop = True时会丢弃原来的索引，设置新的从0开始的索引，常与groupby()一起用 |

举例：重新索引
```python
df_inner.reset_index()
```
<a name="KvxMp"></a>
## 三、数据索引
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `.values` | 将DataFrame转换为ndarray二维数组 |
| 2 | `.append(idx)` | 连接另一个Index对象，产生新的Index对象 |
| 3 | `.insert(loc,e)` | 在loc位置增加一个元素 |
| 4 | `.delete(loc)` | 删除loc位置处的元素 |
| 5 | `.union(idx)` | 计算并集 |
| 6 | `.intersection(idx)` | 计算交集 |
| 7 | `.diff(idx)` | 计算差集，产生新的Index对象 |
| 8 | `.reindex(index, columns ,fill_value, method, limit, copy )` | 改变、重排Series和DataFrame索引，会创建一个新对象，如果某个索引值当前不存在，就引入缺失值。 |
| 9 | `.drop()` | 删除Series和DataFrame指定行或列索引。 |
| 10 | .loc[行标签，列标签] | 通过标签查询指定的数据，第一个值为行标签，第二值为列标签。 |
| 11 | df.iloc[行位置，列位置] | 通过默认生成的数字索引查询指定的数据。 |

举例：按索引提取单行的数值
```python
df_inner.loc[3]
```
<a name="ZKSPj"></a>
## 四、DataFrame选取和重新组合数据的方法
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `df[val]` | 从DataFrame选取单列或一组列；在特殊情况下比较便利：布尔型数组（过滤行）、切片（行切片）、或布尔型DataFrame（根据条件设置值） |
| 2 | `df.loc[val]` | 通过标签，选取DataFrame的单个行或一组行 |
| 3 | `df.loc[：,val]` | 通过标签，选取单列或列子集 |
| 4 | `df.1oc[val1,val2]` | 通过标签，同时选取行和列 |
| 5 | `df.iloc[where]` | 通过整数位置，从DataFrame选取单个行或行子集 |
| 6 | `df.iloc[where_i,where_j]` | 通过整数位置，同时选取行和列 |
| 7 | `df.at[1abel_i,1abel_j]` | 通过行和列标签，选取单一的标量 |
| 8 | `df.iat[i,j]` | 通过行和列的位置（整数），选取单一的标量 |
| 9 | `reindex` | 通过标签选取行或列 |
| 10 | `get_value` | 通过行和列标签选取单一值 |
| 11 | `set_value` | 通过行和列标签选取单一值 |

举例：使用`iloc`按位置区域提取数据
```python
df_inner.iloc[:3,:2] #冒号前后的数字不再是索引的标签名称，而是数据所在的位置，从0开始，前三行，前两列。
```
<a name="Jd4Wq"></a>
## 五、排序
| 序号 | 函数 | 说明 |
| --- | --- | --- |
| 1 | `.sort_index(axis=0, ascending=True)` | 根据指定轴索引的值进行排序 |
| 2 | `Series.sort_values(axis=0, ascending=True)` | 只能根据0轴的值排序。 |
| 3 | `DataFrame.sort_values(by, axis=0, ascending=True)` | 参数by为axis轴上的某个索引或索引列表。 |

举例：按照索引列排序
```python
df_inner.sort_index()
```
<a name="t5bCS"></a>
## 六、相关分析和统计分析
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `.idxmin()` | 计算数据最小值所在位置的索引（自定义索引） |
| 2 | `.idxmax()` | 计算数据最大值所在位置的索引（自定义索引） |
| 3 | `.argmin()` | 计算数据最小值所在位置的索引位置（自动索引） |
| 4 | `.argmax()` | 计算数据最大值所在位置的索引位置（自动索引） |
| 5 | `.describe()` | 针对各列的多个统计汇总，用统计学指标快速描述数据的概要 |
| 6 | `.sum()` | 计算各列数据的和 |
| 7 | `.count()` | 非NaN值的数量 |
| 8 | `.mean()` | 计算数据的算术平均值 |
| 9 | `.median()` | 计算算术中位数 |
| 10 | `.var()` | 计算数据的方差 |
| 11 | `.std()` | 计算数据的标准差 |
| 12 | `.corr()` | 计算相关系数矩阵 |
| 13 | `.cov()` | 计算协方差矩阵 |
| 14 | `.corrwith()` | 利用DataFrame的corrwith方法，可以计算其列或行跟另一个Series或DataFrame之间的相关系数。 |
| 15 | `.min()` | 计算数据的最小值 |
| 16 | `.max()` | 计算数据的最大值 |
| 17 | `.diff()` | 计算一阶差分，对时间序列很有效 |
| 18 | `.mode()` | 计算众数，返回频数最高的那（几）个 |
| 19 | `.mean()` | 计算均值 |
| 20 | `.quantile()` | 计算分位数（0到1） |
| 21 | `.isin()` | 用于判断矢量化集合的成员资格，可用于过滤Series中或DataFrame列中数据的子集 |
| 22 | `.unique()` | 返回一个Series中的唯一值组成的数组。 |
| 23 | `.value_counts()` | 计算一个Series中各值出现的频率。 |

举例：判断city列的值是否为北京
```python
df_inner['city'].isin(['beijing'])
```
<a name="JQoir"></a>
## 七、分组的方法
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `DataFrame.groupby()` | 分组函数 |
| 2 | `pandas.cut()` | 根据数据分析对象的特征，按照一定的数值指标，把数据分析对象划分为不同的区间部分来进行研究，以揭示其内在的联系和规律性。 |

举例：`.groupby`用法
```python
group_by_name=salaries.groupby('name') 
print(type(group_by_name))
```
输出结果为：
```python
<class 'pandas.core.groupby.DataFrameGroupBy'>
```
<a name="ZL7wz"></a>
## 八、读写文本格式数据的方法
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `read_csv` | 从文件、URL、文件型对象中加载带分隔符的数据。默认分隔符为逗号 |
| 2 | `read_table` | 从文件、URL、文件型对象中加载带分隔符的数据。默认分隔符为制表符（t） |
| 3 | `read_ `fwf | 读取定宽列格式数据（也就是说，没有分隔符） |
| 4 | `read_clipboard` | 读取剪贴板中的数据，可以看做read_table的剪贴板版。再将网页转换为表格时很有用 |
| 5 | `read_excel` | 从ExcelXLS或XLSXfile 读取表格数据 |
| 6 | `read_hdf` | 读取pandas写的HDF5文件 |
| 7 | `read_html` | 读取HTML文档中的所有表格 |
| 8 | `read_json` | 读取JSON字符串中的数据 |
| 9 | `read_msgpack` | 二进制格式编码的pandas数据 |
| 10 | `read_pickle` | 读取Python pickle格式中存储的任意对象 |
| 11 | `read_sas` | 读取存储于SAS系统自定义存储格式的SAS数据集 |
| 12 | `read_sql` | 读取SQL 查询结果为pandas的DataFrame |
| 13 | `read_stata` | 读取Stata文件格式的数据集 |
| 14 | `read_feather` | 读取 Feather二进制文件格式 |

举例：导入CSV或者xlsx文件
```python
df = pd.DataFrame(pd.read_csv('name.csv',header=1))
df = pd.DataFrame(pd.read_excel('name.xlsx'))
```
<a name="i9db6"></a>
## 九、处理缺失数据
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `.fillna(value,method,limit,inplace)` | 填充缺失值 |
| 2 | `.dropna()` | 删除缺失数据 |
| 3 | `.info()` | 查看数据的信息，包括每个字段的名称、非空数量、字段的数据类型 |
| 4 | `.isnull()` | 返回一个同样长度的值为布尔型的对象（Series或DataFrame），表示哪些值是缺失的 |

举例：查看数据表基本信息（维度、列名称、数据格式等等）
```python
df.info()
```
<a name="nm7dG"></a>
## 十、数据转换
| 序号 | 方法 | 说明 |
| --- | --- | --- |
| 1 | `.replace(old, new)` | 用新的数据替换老的数据，如果希望一次性替换多个值，old和new可以是列表。默认会返回一个新的对象，传入inplace=True可以对现有对象进行就地修改。 |
| 2 | `.duplicated()` | 判断各行是否是重复行，返回一个布尔型Series。 |
| 3 | `.drop_duplicates()` | 删除重复行，返回删除后的DataFrame对象。 |

举例：删除后出现的重复值：
```python
df['city'].drop_duplicates()
```
