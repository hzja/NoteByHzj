Pandas<br />本文中将使用如下缩写：
```python
df #任何pandas DataFrame对象 
s #任何pandas series对象
```
首先需要先导入2个库：
```python
import pandas as pd
import numpy as np
```
<a name="Gxus7"></a>
### 从各种不同的来源和格式导入数据
```python
pd.read_csv(filename) # 从CSV文件 
pd.read_table(filename) # 从分隔的文本文件（例如CSV）中 
pd.read_excel(filename) # 从Excel文件 
pd.read_sql(query, connection_object) # 从SQL表/数据库中读取 
pd.read_json(json_string) # 从JSON格式的字符串，URL或文件中读取。
pd.read_html(url) # 解析html URL，字符串或文件，并将表提取到数据帧列表 
pd.read_clipboard() # 获取剪贴板的内容并将其传递给 read_table() 
pd.DataFrame(dict) # 从字典中，列名称的键，列表中的数据的值
```
<a name="CCt23"></a>
### 导出数据
使用以下命令将DataFrame导出为CSV，.xlsx，SQL或JSON。
```python
df.to_csv(filename) # 写入CSV文件 
df.to_excel(filename) # 写入Excel文件 
df.to_sql(table_name, connection_object) # 写入SQL表 
df.to_json(filename) # 以JSON格式写入文件
```
<a name="JBiFK"></a>
### 创建测试对象
```python
pd.DataFrame(np.random.rand(20,5))               # 5列20行随机浮点数 
pd.Series(my_list)                               # 从一个可迭代的序列创建一个序列 my_list 
df.index = pd.date_range('1900/1/30', periods=df.shape[0]) # 添加日期索引
```
<a name="kwgpm"></a>
### 查看、检查数据
```python
df.head(n)                       # DataFrame的前n行 
df.tail(n)                       # DataFrame的最后n行 
df.shape                         # 行数和列数 
df.info()                        # 索引，数据类型和内存信息 
df.describe()                    # 数值列的摘要统计信息 
s.value_counts(dropna=False)     # 查看唯一值和计数 
df.apply(pd.Series.value_counts) # 所有列的唯一值和计数
```
<a name="Z5QaB"></a>
### 数据选取
使用这些命令选择数据的特定子集。
```python
df[col]               # 返回带有标签col的列 
df[[col1, col2]]      # 返回列作为新的DataFrame 
s.iloc[0]             # 按位置选择 
s.loc['index_one']    # 按索引选择 
df.iloc[0,:]          # 第一行 
df.iloc[0,0]          # 第一栏的第一元素
```
<a name="IXy9x"></a>
### 数据清理
使用这些命令可以执行各种数据清理任务。
```python
df.columns = ['a','b','c']                  # 重命名列 
pd.isnull()                                 # 空值检查，返回Boolean Arrray 
pd.notnull()                                # 与pd.isnull() 相反 
df.dropna()                                 # 删除所有包含空值的行 
df.dropna(axis=1)                           # 删除所有包含空值的列 
df.dropna(axis=1,thresh=n)                  # 删除所有具有少于n个非null值的行 
df.fillna(x)                                # 将所有空值替换为x 
s.fillna(s.mean())                          # 用均值替换所有空值（均值可以用统计模块中的几乎所有函数替换 ） 
s.astype(float)                             # 将系列的数据类型转换为float 
s.replace(1,'one')                          # 1 用 'one' 
s.replace([1,3],['one','three'])            # 替换所有等于的值 替换为所有1 'one' ，并 3 用 'three' 
df.rename(columns=lambda x: x + 1)          # 列的重命名 
df.rename(columns={'old_name': 'new_ name'})# 选择性重命名 
df.set_index('column_one')                  # 更改索引 
df.rename(index=lambda x: x + 1)            # 大规模重命名索引
```
<a name="LQ3lY"></a>
### 筛选，排序和分组依据
使用这些命令可以对数据进行过滤，排序和分组。
```python
df[df[col] > 0.5]                      # 列 col 大于 0.5 
df[(df[col] > 0.5) & (df[col] < 0.7)]  # 小于 0.7 大于0.5的行 
df.sort_values(col1)                   # 按col1升序对值进行排序 
df.sort_values(col2,ascending=False)   # 按col2 降序对值进行 排序 
df.sort_values([col1,col2],ascending=[True,False]) #按 col1 升序排序，然后 col2 按降序排序 
df.groupby(col)                        #从一个栏返回GROUPBY对象 df.groupby([col1,col2]) # 返回来自多个列的groupby对象 
df.groupby(col1)[col2]                 # 返回中的值的平均值 col2，按中的值分组 col1 （平均值可以用统计模块中的几乎所有函数替换 ） 
df.pivot_table(index=col1,values=[col2,col3],aggfunc=mean) # 创建一个数据透视表组通过 col1 ，并计算平均值的 col2 和 col3 
df.groupby(col1).agg(np.mean)          # 在所有列中找到每个唯一col1 组的平均值 
df.apply(np.mean)                      #np.mean() 在每列上应用该函数 
nf.apply(np.max,axis=1)                # np.max() 在每行上应用功能
```
<a name="L4lim"></a>
### 数据合并
使用这些命令可以将多个数据帧组合为一个数据帧。
```python
df1.append(df2)                   # 将df2添加 df1的末尾 （各列应相同） 
pd.concat([df1, df2],axis=1)      # 将 df1的列添加到df2的末尾 （行应相同） 
df1.join(df2,on=col1,how='inner') # SQL样式将列 df1 与 df2 行所在的列col 具有相同值的列连接起来。'how'可以是一个 'left'， 'right'， 'outer'， 'inner'
```
<a name="fFAuY"></a>
### 数据统计
使用这些命令来执行各种统计测试。（这些也都可以应用于系列。）
```python
df.describe()    # 数值列的摘要统计信息 
df.mean()        # 返回均值的所有列 
df.corr()        # 返回DataFrame中各列之间的相关性 
df.count()       # 返回非空值的每个数据帧列中的数字 
df.max()         # 返回每列中的最高值 
df.min()         # 返回每一列中的最小值 
df.median()      # 返回每列的中位数 
df.std()         # 返回每列的标准偏
```
