Python Pandas<br />Pandas 也是一个 Python 包，它提供了快速、灵活以及具有显著表达能力的数据结构，旨在使处理结构化 (表格化、多维、异构) 和时间序列数据变得既简单又直观。<br />![](./img/1616571110406-241c0cc5-96a0-4901-8f64-46d465489937.webp)
<a name="VpGYZ"></a>
## Pandas 适用于以下各类数据

- 具有异构类型列的表格数据，如 SQL 表或 Excel 表；
- 有序和无序 (不一定是固定频率) 的时间序列数据；
- 带有行/列标签的任意矩阵数据（同构类型或者是异构类型）；
- 其他任意形式的统计数据集。事实上，数据根本不需要标记就可以放入 Pandas 结构中。
<a name="STvbC"></a>
## Pandas 擅长处理的类型如下所示

- 容易处理浮点数据和非浮点数据中的 缺失数据（用 NaN 表示）；
- 大小可调整性: 可以从 DataFrame 或者更高维度的对象中插入或者是删除列；
- 显式数据可自动对齐: 对象可以显式地对齐至一组标签内，或者用户可以简单地选择忽略标签，使 Series、 DataFrame 等自动对齐数据；
- 灵活的分组功能，对数据集执行拆分-应用-合并等操作，对数据进行聚合和转换；
- 简化将数据转换为 DataFrame 对象的过程，而这些数据基本是 Python 和 NumPy 数据结构中不规则、不同索引的数据；
- 基于标签的智能切片、索引以及面向大型数据集的子设定；
- 更加直观地合并以及连接数据集；
- 更加灵活地重塑、转置（pivot）数据集；
- 轴的分级标记 (可能包含多个标记)；
- 具有鲁棒性的 IO 工具，用于从平面文件 (CSV 和 delimited)、 Excel 文件、数据库中加在数据，以及从 HDF5 格式中保存 / 加载数据；
- 时间序列的特定功能: 数据范围的生成以及频率转换、移动窗口统计、数据移动和滞后等。
<a name="mxeiR"></a>
## Pandas 数据统计包的 6 种高效函数
<a name="r89Hc"></a>
### `read_csv(nrows=n)`
在不需要.csv 文件的情况下仍会完整地读取它。如果一个未知的.csv 文件有 10GB，那么读取整个.csv 文件将会非常不明智，不仅要占用大量内存，还会花很多时间。需要做的只是从.csv 文件中导入几行，之后根据需要继续导入。
```python
import io
import requests
# I am using this online data set just to make things easier for you guys
url = "https://raw.github.com/vincentarelbundock/Rdatasets/master/csv/datasets/AirPassengers.csv"
s = requests.get(url).content# read only first 10 rows
df = pd.read_csv(io.StringIO(s.decode( utf-8 )),nrows=10 , index_col=0)
```
<a name="gEGC8"></a>
### `map()`
`map()` 函数根据相应的输入来映射 Series 的值。用于将一个 Series 中的每个值替换为另一个值，该值可能来自一个函数、也可能来自于一个 dict 或 Series。
```python
# create a dataframe
dframe = pd.DataFrame(np.random.randn(4, 3), columns=list( bde ), index=[ India ,  USA ,  China ,  Russia ])#compute a formatted string from each floating point value in frame
changefn = lambda x:  %.2f  % x# Make changes element-wise
dframe[ d ].map(changefn)
```
<a name="Tb1Ji"></a>
### `apply()`
`apply()` 允许用户传递函数，并将其应用于 Pandas 序列中的每个值。
```python
# max minus mix lambda fn
fn = lambda x: x.max() - x.min()# Apply this on dframe that we ve just created above
dframe.apply(fn)
```
<a name="hI0gG"></a>
### `isin()`
`lsin()` 用于过滤数据帧。`Isin()` 有助于选择特定列中具有特定（或多个）值的行。
```python
# Using the dataframe we created for read_csv
filter1 = df["value"].isin([112]) 
filter2 = df["time"].isin([1949.000000])df [filter1 & filter2]
```
<a name="MmTJV"></a>
### `copy()`
`copy()` 函数用于复制 Pandas 对象。当一个数据帧分配给另一个数据帧时，如果对其中一个数据帧进行更改，另一个数据帧的值也将发生更改。为了防止这类问题，可以使用 `copy()` 函数。
```python
# creating sample series 
data = pd.Series([ India ,  Pakistan ,  China ,  Mongolia ])
# Assigning issue that we face
data1= data
# Change a value
data1[0]= USA 
# Also changes value in old dataframe
data# To prevent that, we use
# creating copy of series 
new = data.copy()# assigning new values 
new[1]= Changed value # printing data 
print(new) 
print(data)
```
<a name="TRB5A"></a>
### `select_dtypes()`
`select_dtypes()` 的作用是，基于 dtypes 的列返回数据帧列的一个子集。这个函数的参数可设置为包含所有拥有特定数据类型的列，亦或者设置为排除具有特定数据类型的列。
```python
# We ll use the same dataframe that we used for read_csv
framex =  df.select_dtypes(include="float64")# Returns only time column
```
最后，`pivot_table()` 也是 Pandas 中一个非常有用的函数。如果对 `pivot_table()` 在 excel 中的使用有所了解，那么就非常容易上手了。
```python
# Create a sample dataframe
school = pd.DataFrame({ A : [ Jay ,  Usher ,  Nicky ,  Romero ,  Will ],        B : [ Masters ,  Graduate ,  Graduate ,  Masters ,  Graduate ],        C : [26, 22, 20, 23, 24]})# Lets create a pivot table to segregate students based on age and course
table = pd.pivot_table(school, values = A , index =[ B ,  C ],                          columns =[ B ], aggfunc = np.sum, fill_value="Not Available") 
table
```
