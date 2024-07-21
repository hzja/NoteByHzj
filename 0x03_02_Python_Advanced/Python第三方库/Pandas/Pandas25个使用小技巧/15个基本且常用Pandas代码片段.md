PythonPandas<br />Pandas提供了强大的数据操作和分析功能，是数据科学的日常基本工具。在本文中，将介绍最常用的15个Pandas代码片段。这些片段将帮助简化数据分析任务，从数据集中提取有价值的见解。
<a name="CHiFm"></a>
## 1、过滤数据
Pandas提供了多种方法来过滤数据。
```python
import pandas as pd

# Create a DataFrame
data = {'Name': ['Alice', 'Bob', 'Charlie', 'David'],
        'Age': [25, 30, 35, 40]}

df = pd.DataFrame(data)

# Filter rows where Age is greater than 30
filtered_df = df[df['Age'] > 30]
print(filtered_df)
```
<a name="OF9BT"></a>
## 2、分组和聚合数据
```python
# Grouping by a column and calculating the mean
grouped = df.groupby('Age').mean()
print(grouped)
```
<a name="hKUKF"></a>
## 3、数据缺失值
```python
# Check for missing values
missing_values = df.isnull().sum()

# Fill missing values with a specific value
df['Age'].fillna(0, inplace=True)
```
<a name="oDSz8"></a>
## 4、将函数应用于列
`apply()` 函数允许在 DataFrame 的行或列上应用自定义函数，以实现更复杂的数据处理和转换操作。
```python
df['Age'] = df['Age'].apply(lambda x: x * 2)
```
<a name="q0vIc"></a>
## 5、连接DataFrames
这里的连接主要是行的连接，也就是说将两个相同列结构的DataFrame进行连接
```python
# Concatenate two DataFrames
df1 = pd.DataFrame({'A': ['A0', 'A1'], 'B': ['B0', 'B1']})
df2 = pd.DataFrame({'A': ['A2', 'A3'], 'B': ['B2', 'B3']})

result = pd.concat([df1, df2], ignore_index=True)
print(result)
```
<a name="ab45d"></a>
## 6、合并DataFrames
这里的合并指的是列的合并，也就是说根据一个或若干个相同的列，进行合并
```python
# Merge two DataFrames
left = pd.DataFrame({'key': ['A', 'B', 'C'], 'value': [1, 2, 3]})
right = pd.DataFrame({'key': ['B', 'C', 'D'], 'value': [4, 5, 6]})

merged = pd.merge(left, right, on='key', how='inner')
print(merged)
```
<a name="bBiGc"></a>
## 7、数据透视表
`pivot_table` 是用于数据透视的重要函数之一。它根据一个或多个列的值对数据进行重新排列和汇总，以便更好地理解数据的结构和关系。
```python
# Creating a pivot table
pivot_table = df.pivot_table(index='Name', 
                             columns='Age',
                             values='Value')
print(pivot_table)
```
<a name="qMMeB"></a>
## 8、处理时间/日期类型数据
```python
# Converting a column to DateTime
df['Date'] = pd.to_datetime(df['Date'])
```
<a name="kQucv"></a>
## 9、数据重塑
`pandas.melt()` 是用于将宽格式（wide format）的数据表格转换为长格式（long format）。这个函数通常用于数据重塑（data reshaping）操作，以便更容易进行数据分析和可视化。
```python
pandas.melt(frame, id_vars=None, value_vars=None,
            var_name=None, value_name='value', 
            col_level=None)
```
参数说明：

- `frame`：要进行重塑操作的数据表格（DataFrame）。
- `id_vars`：需要保留的列，它们将成为长格式中的标识变量（identifier variable），不被"融化"。
- `value_vars`：需要"融化"的列，它们将被整合成一列，并用新的列名表示。
- `var_name`：用于存储"融化"后的列名的新列的名称。
- `value_name`：用于存储"融化"后的值的新列的名称。
- `col_level`：如果输入数据是多级索引（MultiIndex），则可以指定在哪个级别上应用"融化"操作。

下面是一个示例，演示如何使用 `melt()` 函数将宽格式数据转换为长格式，假设有以下的宽格式数据表格 df：
```
ID Name Math English History
 0   1   Amy   90       85       88
 1   2   Bob   78       92       76
 2   3 John   88       79       90
```
要将 Math、English 和 History 列"融化"为一个长格式数据表格，可以这样做：
```python
melted_df = pd.melt(df, id_vars=['ID', 'Name'], 
                    value_vars=['Math', 'English', 'History'],
                    var_name='Subject', value_name='Score')
```
转换后的长格式数据表格 melted_df 如下所示：
```
ID Name Subject Score
 0   1   Amy     Math     90
 1   2   Bob     Math     78
 2   3 John     Math     88
 3   1   Amy English     85
 4   2   Bob English     92
 5   3 John English     79
 6   1   Amy History     88
 7   2   Bob History     76
 8   3 John History     90
```
通过这种方式，可以将宽格式数据表格中的多列数据整合到一个列中，以更容易进行分析、可视化或其他操作。`melt()` 函数在数据清洗和转换阶段非常有用。<br />`melt()` 或者可以理解为上面`pivot_table` 或者`unstack`的反操作。
<a name="aPc2g"></a>
## 10、分类数据
`astype('category')` 是用于将一列数据类型转换为分类（Category）类型的方法。将数据列转换为分类类型有助于节省内存和提高性能，特别是当数据列中包含有限的不同取值时。
```python
# Encoding categorical variables
df['Category'] = df['Category'].astype('category')
df['Category'] = df['Category'].cat.codes
```
<a name="taRmr"></a>
## 11、数据抽样
```python
# Randomly sample rows from a DataFrame
sampled_df = df.sample(n=2)
```
<a name="SpgdZ"></a>
## 12、计算累加和
```python
# Calculating cumulative sum
df['Cumulative_Sum'] = df['Values'].cumsum()
```
<a name="BzDGQ"></a>
## 13、删除重复的数据
```python
# Removing duplicate rows
df.drop_duplicates(subset=['Column1', 'Column2'], 
                   keep='first', inplace=True)
```
<a name="mebcv"></a>
## 14、创建虚拟变量
`pandas.get_dummies()` 是 Pandas 中用于执行独热编码（One-Hot Encoding）的函数。
```python
# Creating dummy variables for categorical data
dummy_df = pd.get_dummies(df, columns=['Category'])
```
<a name="Wz8Pj"></a>
## 15、数据导出
有很多个to方法，可以到导出不同的格式
```python
# Exporting DataFrame to CSV
df.to_csv('output.csv', index=False)
```
<a name="EAtHs"></a>
## 总结
以上这15个Pandas代码片段是日常最常用的数据操作和分析操作。熟练的掌握它，并将它们合并到工作流程中，可以提高处理和探索数据集的效率和效果。
