Pandas<br />Pandas是一个开源Python库，广泛用于数据操作和分析任务。<br />它提供了高效的数据结构和功能，使用户能够有效地操作和分析结构化数据。<br />凭借其广泛的功能，Pandas 对于数据清理、预处理、整理和探索性数据分析等活动具有很大的价值。<br />Pandas的核心数据结构是Series和DataFrame。<br />Series是一个一维标记数组，可以容纳多种数据类型。DataFrame则是一种二维表状结构，由行和列组成，类似于电子表格或SQL表。<br />利用这些数据结构以及广泛的功能，用户可以快速加载、转换、过滤、聚合和可视化数据。<br />Pandas与其他流行的Python库（如NumPy、Matplotlib和scikit-learn）快速集成。<br />这种集成促进了数据操作、分析和可视化的工作流程。<br />由于其直观的语法和广泛的功能，Pandas已成为数据科学家、分析师和研究人员在 Python中处理表格或结构化数据的首选工具。 <br />介绍Pandas的所有重要功能，并清晰简洁地解释它们的用法。
<a name="XWBMV"></a>
## 1、使用Pandas导入数据并读取文件
要使用pandas导入数据和读取文件，可以使用库提供的`read_*`函数。
```python
# 导入Pandas
import pandas as pd 

# 使用Pandas读取文件

# 读取CSV文件
df = pd.read_csv('file.csv')

# 读取Excel文件
df = pd.read_excel('file.xlsx')

# 读取JSON文件
df = pd.read_json('file.json')

# 读取Sql查询
pd.read_sql(query, connection_object)

# 读取Parquet文件
df = pd.read_parquet('file.parquet')

# 从url读取HTML表
url='https://www.example.com/table.html'
tables = pd.read_html(url)
```
<a name="cw8xV"></a>
## 2、查看和检查对象
在Pandas中处理数据时，可以使用多种方法来查看和检查对象，例如DataFrame和Series。
```python
# 用于显示数据的前n行
df.head(n)

# 用于显示数据的后n行
df.tail(n)

# 用于获取数据的行数和列数
df.shape

# 用于获取数据的索引、数据类型和内存信息
df.info()
```
<a name="tZOpu"></a>
## 3、使用Pandas进行数据选择
Pandas提供了各种数据选择方法，允许从DataFrame或Series中提取特定数据。
```python
# 用于获取带有标签列的series
df[column]

# 选择多列
df[['column_name1', 'column_name2']]

# 通过标签选择单行
df.loc[label] 

# 通过标签选择多行
df.loc[[label1, label2, label3]]

# 通过整数索引选择单行
df.iloc[index]

# 通过整数索引选择多行
df.iloc[start_index:end_index]

# 根据条件过滤行
df[df['column_name'] > 5 ] 

# 使用多个条件过滤行
df[(df['column_name1'] > 5) & (df['column_name2'] == 'value')]

# 通过标签选择特定的行和列
df.loc[row_labels, column_labels]

# 通过整数索引选择特定的行和列
df.iloc[row_indices, column_indices] 

# 根据条件选择数据框中的行和列
df.loc[df['column_name'] > 5, ['column_name1', 'column_name2']]
```
<a name="HXqEm"></a>
## 4、数据清洗
数据清洗是数据预处理阶段的重要步骤，在此阶段对数据进行转换和修改以确保其准确性、一致性和可靠性。
```python
# 检查缺失值
df.isnull() 

# 删除有缺失值的行
df.dropna()

# 用特定值填充缺失值
df.fillna(value) 

# 插入缺失值
df.interpolate()

# 检查重复行
df.duplicated()

# 删除重复行
df.drop_duplicates()

# 计算z分数
z_scores = (df - df.mean()) / df.std()

# 根据z分数识别离群值
= df[z_scores > threshold]

# 删除离群值
df_cleaned = df[z_scores <= threshold]

# 替换列中的值
df['column_name'] = df['column_name'].str.replace('old_value', 'new_value')

# 删除前/尾空格
df['column_name'] = df['column_name'].str.strip()

# 将字符串转换为小写
df['column_name'] = df['column_name'].str.lower()

# 将列转换为不同的数据类型
df['column_name'] = df['column_name'].astype('new_type')

# 将列转换为日期时间
df['date_column'] = pd.to_datetime(df['date_column'])

# 重命名列名
df.columns = ['Cat', 'Mat', 'Xat'] 

# 重置DataFrame的索引
df.reset_index()
```
<a name="LezFQ"></a>
## 5、过滤、排序和分组
Pandas是一个强大的Python库，用于数据操作和分析。<br />它提供了各种函数来过滤、排序和分组DataFrame中的数据。
```python
# 根据条件过滤行
df_filtered = df[df['column_name'] > 5]

# 按单列对DataFrame进行排序
df_sorted = df.sort_values('column_name')

# 按多列对DataFrame进行排序
df_sorted = df.sort_values(['column_name1', 'column_name2'], ascending=[True, False])

# 按单列对DataFrame进行分组并计算另一列的平均值
grouped_data = df.groupby('column_name')['other_column'].mean()

# 按多列对DataFrame进行分组并计算另一列的总和
grouped_data = df.groupby(['column_name1', 'column_name2'])['other_column'].sum()

# 计算列的总和
sum_value = df['column_name'].sum ()

# 计算列的平均值
mean_value = df['column_name'].mean()

# 计算列的最大值
max_value = df['column_name'].max()

# 计算列的最小值
min_value = df[ 'column_name' ].min()

# 统计列中非空值的个数
count = df['column_name'].count() 

# 对DataFrame进行分组并重置索引
grouped_data = df.groupby('column_name')['other_column'].sum().reset_index()
```
<a name="vq3UV"></a>
## 6、加入/合并
在pandas中，可以使用各种函数基于公共列或索引来连接或组合多个DataFrame。
```python
# 将df中的行添加到df2的末尾
df.append(df2)

# 将df中的列添加到df2的末尾
pd.concat([df, df2])

# 对列A执行外连接
outer_join = pd.merge(df1, df2, on='A', how='outer'), axis =1)

# 对列A执行内连接
inner_join = pd.merge(df1, df2, on='A', how='inner')

# 对列A执行左连接
left_join = pd.merge(df1, df2, on='A', how='left') 

# 对列A执行右连接
right_join = pd.merge(df1, df2, on='A', how='right')
```
<a name="LM1ea"></a>
## 7、Pandas中的统计
Pandas提供了广泛的统计函数和方法来分析DataFrame或Series中的数据。
```python
# 计算数值列的描述性统计
df.describe()

# 计算某列的总和
df['column_name'].sum()

# 计算某列的平均值
df['column_name'].mean()

# 计算某列的最大值
df['column_name'].max()

# 计算某列中非空值的数量
df['column_name'].count()

# 计算列中某个值的出现次数
df['column_name'].value_counts()
```
<a name="EzD5s"></a>
## 8、导出数据
Pandas是一个用于数据操作和分析的强大Python库。<br />它提供了将数据导出为不同格式的各种功能。
```python
# 以csv格式导出, 不带行索引导出
df.to_csv('filename.csv', index=False)

# 以Excel格式导出, 不带行索引导出
data.to_excel('filename.xlsx', index=False)

# 导出Json格式
data.to_json('filename.json', orient='records') 

# 以SQL格式导出
data.to_sql('table_name', con=engine, if_exists='replace', index=False)
```
