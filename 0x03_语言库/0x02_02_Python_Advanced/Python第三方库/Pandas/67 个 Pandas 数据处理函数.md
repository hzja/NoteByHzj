Python Pandas<br />总结一下 “Pandas数据处理” 几个方面重要的知识，拿来即用，随查随查。

- 导入数据
- 导出数据
- 查看数据
- 数据选取
- 数据处理
- 数据分组和排序
- 数据合并
```python
# 在使用之前，需要导入pandas库
import pandas as pd
```
<a name="wzfJT"></a>
## 导入数据
这里总结7个常见用法。
```python
pd.DataFrame() # 自己创建数据框，用于练习
pd.read_csv(filename) # 从CSV文件导入数据
pd.read_table(filename) # 从限定分隔符的文本文件导入数据
pd.read_excel(filename) # 从Excel文件导入数据
pd.read_sql(query,connection_object) # 从SQL表/库导入数据
pd.read_json(json_string) # 从JSON格式的字符串导入数据
pd.read_html(url) # 解析URL、字符串或者HTML文件，抽取其中的tables表格
```
<a name="jEzoV"></a>
## 导出数据
这里总结5个常见用法。
```python
df.to_csv(filename) #导出数据到CSV文件
df.to_excel(filename) #导出数据到Excel文件
df.to_sql(table_name,connection_object) #导出数据到SQL表
df.to_json(filename) #以Json格式导出数据到文本文件

# 将多个df对象写入同一工作簿下的多个工作表
writer = pd.ExcelWriter('test.xlsx', index=False) 
df1.to_excel(writer, sheet_name='Sheet1')
df2.to_excel(writer, sheet_name='Sheet2')
writer.save()
```
<a name="n3vqd"></a>
## 查看数据
这里总结11个常见用法。
```python
df.head(n) # 预览前n行数据

df.tail(n) # 预览后n行数据

df.shape() # 查看行数和列数

df.info() # 查看索引、数据类型和内存信息等

df.columns() # 查看字段名称

df.describe() # 查看数据类型为数值的字段的描述统计信息

s.value_counts(dropna=False) # 查看Series对象的唯一值和计数

df.apply(pd.Series.value_counts) # 查看DataFrame对象中每一列的唯一值和计数

df.isnull().any() # 查看是否存在缺失值

df[df[column_name].duplicated()] # 查看column_name字段数据重复的数据信息

df[df[column_name].duplicated()].count() # 查看column_name字段数据重复的个数
```
<a name="XfujV"></a>
## 数据选取
这里总结10个常见用法。
```python
df[col] # 根据列名，并以Series的形式返回列
df[[col1,col2]] # 以DataFrame形式返回多列
s.iloc[0] # 按位置选取数据
s.loc['index_one'] # 按索引选取数据
df.iloc[0,:] # 返回第一行
df.iloc[0,0] # 返回第一列的第一个元素
df.loc[0,:] # 返回第一行（索引为默认的数字时，用法同df.iloc），但需要注意的是loc是按索引,iloc参数只接受数字参数
df.ix[[:5],["col1","col2"]] # 返回字段为col1和col2的前5条数据，可以理解为loc和iloc的结合体。
df.at[5,"col1"] # 选择索引名称为5，字段名称为col1的数据
df.iat[5,0] # 选择索引排序为5，字段排序为0的数据
```
<a name="OoazX"></a>
## 数据处理
这里总结16个常见用法。
```python
df.columns= ['a','b','c'] # 重命名列名（需要将所有列名列出，否则会报错）
pd.isnull() # 检查DataFrame对象中的空值，并返回⼀个Boolean数组
pd.notnull() # 检查DataFrame对象中的⾮空值，并返回⼀个Boolean数组
df.dropna() # 删除所有包含空值的行
df.dropna(axis=1) # 删除所有包含空值的列
df.dropna(axis=1,thresh=n) # 删除所有小于n个⾮空值的行
df.fillna(value=x) # 用x替换DataFrame对象中所有的空值，支持
df[column_name].fillna(x)
s.astype(float) # 将Series中的数据类型更改为float类型
s.replace(1,'one') # 用‘one’代替所有等于1的值
s.replace([1,3],['one','three']) # 用'one'代替1，用'three'代替3
df.rename(columns=lambdax:x+1) # 批量更改列名
df.rename(columns={'old_name':'new_ name'}) # 选择性更改列名
df.set_index('column_one') # 将某个字段设为索引，可接受列表参数，即设置多个索引
df.reset_index("col1") # 将索引设置为col1字段，并将索引新设置为0,1,2...
df.rename(index=lambdax:x+1) # 批量重命名索引
```
<a name="9Ntsy"></a>
## 数据分组、排序、透视
这里总结13个常见用法。
```python
df.sort_index().loc[:5] # 对前5条数据进行索引排序
df.sort_values(col1) # 按照列col1排序数据，默认升序排列
df.sort_values(col2,ascending=False) # 按照列col1降序排列数据
df.sort_values([col1,col2],ascending=[True,False]) # 先按列col1升序排列，后按col2降序排列数据
df.groupby(col) # 返回一个按列col进行分组的Groupby对象
df.groupby([col1,col2]) # 返回一个按多列进行分组的Groupby对象
df.groupby(col1)[col2].agg(mean) # 返回按列col1进行分组后，列col2的均值,agg可以接受列表参数，agg([len,np.mean])
df.pivot_table(index=col1,values=[col2,col3],aggfunc={col2:max,col3:[ma,min]}) # 创建一个按列col1进行分组，计算col2的最大值和col3的最大值、最小值的数据透视表
df.groupby(col1).agg(np.mean) # 返回按列col1分组的所有列的均值,支持
df.groupby(col1).col2.agg(['min','max'])
data.apply(np.mean) # 对DataFrame中的每一列应用函数np.mean
data.apply(np.max,axis=1) # 对DataFrame中的每一行应用函数np.max
df.groupby(col1).col2.transform("sum") # 通常与groupby连用，避免索引更改
```
<a name="PEaum"></a>
## 数据合并
这里总结5个常见用法。
```python
df1.append(df2) # 将df2中的行添加到df1的尾部
df.concat([df1,df2],axis=1,join='inner') # 将df2中的列添加到df1的尾部,值为空的对应行与对应列都不要
df1.join(df2.set_index(col1),on=col1,how='inner') # 对df1的列和df2的列执行SQL形式的join，默认按照索引来进行合并，如果df1和df2有共同字段时，会报错，可通过设置lsuffix,rsuffix来进行解决，如果需要按照共同列进行合并，就要用到set_index(col1)
pd.merge(df1,df2,on='col1',how='outer') # 对df1和df2合并，按照col1，方式为outer
pd.merge(df1,df2,left_index=True,right_index=True,how='outer') # 与 df1.join(df2, how='outer')效果相同
```
