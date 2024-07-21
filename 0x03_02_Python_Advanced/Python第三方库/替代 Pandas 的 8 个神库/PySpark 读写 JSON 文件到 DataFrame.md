<a name="qPak9"></a>
##### 将具有单行记录和多行记录的 JSON 文件读取到 PySpark DataFrame 中，还要学习一次读取单个和多个文件以及使用不同的保存选项将 JSON 文件写回 DataFrame。
PySpark SQL 提供 `read.json("path")` 将单行或多行（多行）JSON 文件读取到 PySpark DataFrame 并 `write.json("path")` 保存或写入 JSON 文件的功能，学习如何读取单个文件、多个文件、目录中的所有文件进入 DataFrame 并使用 Python 示例将 DataFrame 写回 JSON 文件。<br />**注意：** 开箱即用的 PySpark API 支持将 JSON 文件和更多文件格式读取到 PySpark DataFrame 中。
<a name="F1qeU"></a>
## 目录

- 从多行读取 JSON 文件
- 一次读取多个文件
- 读取目录中的所有文件
- 使用用户自定义架构读取文件
- 使用 PySpark SQL 读取 JSON 文件
- 读取 JSON 文件时的选项
   - NullValues
   - DateFormat
- 应用 DataFrame 转换
   - 将 PySpark DataFrame 写入 JSON 文件
   - 编写 JSON 文件时的 PySpark 选项
- PySpark 保存模式
- 源代码供参考

使用 `read.json("path")` 或者 `read.format("json").load("path")` 方法将文件路径作为参数，可以将 JSON 文件读入 PySpark DataFrame。<br />与读取 CSV 不同，默认情况下，来自输入文件的 JSON 数据源推断模式。<br />此处使用的 zipcodes.json 文件可以从 GitHub 项目下载。<br />传送门：[https://github.com/spark-examples/pyspark-examples/blob/master/resources/zipcodes.json](https://github.com/spark-examples/pyspark-examples/blob/master/resources/zipcodes.json)
```python
# Read JSON file into dataframe
df = spark.read.json("PyDataStudio/zipcodes.json")
df.printSchema()
df.show()
```
当使用 `format("json")` 方法时，还可以通过其完全限定名称指定数据源，如下所示。
```python
# Read JSON file into dataframe
df = spark.read.format('org.apache.spark.sql.json') \
        .load("PyDataStudio/zipcodes.json")
```
<a name="EIvaN"></a>
## 从多行读取 JSON 文件
PySpark JSON 数据源在不同的选项中提供了多个读取文件的选项，使用`multiline`选项读取分散在多行的 JSON 文件。默认情况下，多行选项设置为 false。<br />下面是要读取的输入文件，同样的文件也可以在Github上找到。<br />传送门：[https://github.com/spark-examples/pyspark-examples/blob/master/resources/multiline-zipcode.json](https://github.com/spark-examples/pyspark-examples/blob/master/resources/multiline-zipcode.json)
```json
[{
  "RecordNumber": 2,
  "Zipcode": 704,
  "ZipCodeType": "STANDARD",
  "City": "PASEO COSTA DEL SUR",
  "State": "PR"
},
{
  "RecordNumber": 10,
  "Zipcode": 709,
  "ZipCodeType": "STANDARD",
  "City": "BDA SAN LUIS",
  "State": "PR"
}]
```
使用`read.option("multiline","true")`
```python
# Read multiline json file
multiline_df = spark.read.option("multiline","true") \
      .json("PyDataStudio/multiline-zipcode.json")
multiline_df.show()    
```
<a name="zTq41"></a>
## 一次读取多个文件
还可以使用`read.json()`方法从不同路径读取多个 JSON 文件，只需通过逗号分隔传递所有具有完全限定路径的文件名，例如
```python
# Read multiple files
df2 = spark.read.json(
    ['resources/zipcode1.json',
     'resources/zipcode2.json'])
df2.show()  
```
<a name="HqPhP"></a>
## 读取目录中的所有文件
只需将目录作为`json()`方法的路径传递给该方法，就可以将目录中的所有 JSON 文件读取到 DataFrame 中。
```python
# Read all JSON files from a folder
df3 = spark.read.json("resources/*.json")
df3.show()
```
<a name="W7OEt"></a>
## 使用用户自定义架构读取文件
PySpark Schema 定义了数据的结构，换句话说，它是 DataFrame 的结构。PySpark SQL 提供 StructType 和 StructField 类以编程方式指定 DataFrame 的结构。<br />如果事先知道文件的架构并且不想使用`inferSchema`选项来指定列名和类型，请使用指定的自定义列名schema并使用schema选项键入。<br />使用 PySpark StructType 类创建自定义 Schema，下面启动这个类并使用添加方法通过提供列名、数据类型和可为空的选项向其添加列。
```python
# Define custom schema
schema = StructType([
      StructField("RecordNumber",IntegerType(),True),
      StructField("Zipcode",IntegerType(),True),
      StructField("ZipCodeType",StringType(),True),
      StructField("City",StringType(),True),
      StructField("State",StringType(),True),
      StructField("LocationType",StringType(),True),
      StructField("Lat",DoubleType(),True),
      StructField("Long",DoubleType(),True),
      StructField("Xaxis",IntegerType(),True),
      StructField("Yaxis",DoubleType(),True),
      StructField("Zaxis",DoubleType(),True),
      StructField("WorldRegion",StringType(),True),
      StructField("Country",StringType(),True),
      StructField("LocationText",StringType(),True),
      StructField("Location",StringType(),True),
      StructField("Decommisioned",BooleanType(),True),
      StructField("TaxReturnsFiled",StringType(),True),
      StructField("EstimatedPopulation",IntegerType(),True),
      StructField("TotalWages",IntegerType(),True),
      StructField("Notes",StringType(),True)
  ])

df_with_schema = spark.read.schema(schema) \
        .json("PyDataStudio/zipcodes.json")
df_with_schema.printSchema()
df_with_schema.show()
```
<a name="Vu6aJ"></a>
## 使用 PySpark SQL 读取 JSON 文件
PySpark SQL 还提供了一种读取 JSON 文件的方法，方法是使用 spark.sqlContext.sql(“将 JSON 加载到临时视图”) 直接从读取文件创建临时视图
```python
spark.sql("CREATE OR REPLACE TEMPORARY VIEW zipcode USING json OPTIONS" + 
      " (path 'PyDataStudio/zipcodes.json')")
spark.sql("select * from zipcode").show()
```
<a name="mCtte"></a>
## 读取 JSON 文件时的选项
<a name="DouPu"></a>
### NullValues
使用 nullValues 选项，可以将 JSON 中的字符串指定为 null。例如，如果想考虑一个值为 1900-01-01 的日期列，则在 DataFrame 上设置为 null。
<a name="vpWgR"></a>
### DateFormat
选项 dateFormat用于设置输入 DateType 和 TimestampType 列的格式的选项。支持所有 java.text.SimpleDateFormat 格式。<br />**注意：**除了上述选项外，PySpark JSON 数据集还支持许多其他选项。
<a name="Dvmzz"></a>
## 应用 DataFrame 转换
从 JSON 文件创建 PySpark DataFrame 后，可以应用 DataFrame 支持的所有转换和操作。
<a name="Fp6fQ"></a>
## 将 PySpark DataFrame 写入 JSON 文件
在 DataFrame 上使用 PySpark DataFrameWriter 对象 write 方法写入 JSON 文件。
```python
df2.write.json("/PyDataStudio/spark_output/zipcodes.json")
```
<a name="AD1rq"></a>
### 编写 JSON 文件时的 PySpark 选项
在编写 JSON 文件时，可以使用多个选项。如 nullValue，dateFormat
<a name="OU2uU"></a>
### PySpark 保存模式
PySpark DataFrameWriter 还有一个方法 mode() 来指定 SaveMode；此方法的参数采用overwrite, append, ignore, errorifexists.

- `overwrite` – 模式用于覆盖现有文件
- `append` – 将数据添加到现有文件
- `ignore` – 当文件已经存在时忽略写操作
- `errorifexists` 或 `error` – 这是文件已存在时的默认选项，它返回错误
```python
df2.write.mode('Overwrite') \
   .json("/PyDataStudio/spark_output/zipcodes.json")
```
<a name="LjmmL"></a>
## 源代码供参考
此示例也可在GitHub PySpark 示例项目中获得以供参考。
```python
# https://github.com/spark-examples/pyspark-examples/blob/master/pyspark-read-json.py
from pyspark.sql import SparkSession
from pyspark.sql.types import StructType,StructField, StringType, IntegerType,BooleanType,DoubleType
spark = SparkSession.builder \
    .master("local[1]") \
    .appName("SparkByExamples.com") \
    .getOrCreate()

# Read JSON file into dataframe    
df = spark.read.json("PyDataStudio/zipcodes.json")
df.printSchema()
df.show()

# Read multiline json file
multiline_df = spark.read.option("multiline","true") \
      .json("PyDataStudio/multiline-zipcode.json")
multiline_df.show()

#Read multiple files
df2 = spark.read.json(
    ['PyDataStudio/zipcode2.json','PyDataStudio/zipcode1.json'])
df2.show()    

#Read All JSON files from a directory
df3 = spark.read.json("PyDataStudio/*.json")
df3.show()

# Define custom schema
schema = StructType([
      StructField("RecordNumber",IntegerType(),True),
      StructField("Zipcode",IntegerType(),True),
      StructField("ZipCodeType",StringType(),True),
      StructField("City",StringType(),True),
      StructField("State",StringType(),True),
      StructField("LocationType",StringType(),True),
      StructField("Lat",DoubleType(),True),
      StructField("Long",DoubleType(),True),
      StructField("Xaxis",IntegerType(),True),
      StructField("Yaxis",DoubleType(),True),
      StructField("Zaxis",DoubleType(),True),
      StructField("WorldRegion",StringType(),True),
      StructField("Country",StringType(),True),
      StructField("LocationText",StringType(),True),
      StructField("Location",StringType(),True),
      StructField("Decommisioned",BooleanType(),True),
      StructField("TaxReturnsFiled",StringType(),True),
      StructField("EstimatedPopulation",IntegerType(),True),
      StructField("TotalWages",IntegerType(),True),
      StructField("Notes",StringType(),True)
  ])

df_with_schema = spark.read.schema(schema) \
        .json("PyDataStudio/zipcodes.json")
df_with_schema.printSchema()
df_with_schema.show()

# Create a table from Parquet File
spark.sql("CREATE OR REPLACE TEMPORARY VIEW zipcode3 USING json OPTIONS" + 
      " (path 'PyDataStudio/zipcodes.json')")
spark.sql("select * from zipcode3").show()

# PySpark write Parquet File
df2.write.mode('Overwrite').json("/PyDataStudio/spark_output/zipcodes.json")
```
