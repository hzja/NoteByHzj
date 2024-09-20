PySpark 在 DataFrameReader 上提供了csv("path")将 CSV 文件读入 PySpark DataFrame 并保存或写入 CSV 文件的功能`dataframeObj.write.csv("path")`，这里实现将本地目录中的单个文件、多个文件、所有文件读入 DataFrame，应用一些转换，最后使用 PySpark 示例将 DataFrame 写回 CSV 文件。<br />PySpark 支持读取带有竖线、逗号、制表符、空格或任何其他分隔符文件的 CSV 文件。<br />**注意：** 开箱即用的 PySpark 支持将 CSV、JSON 和更多文件格式的文件读取到 PySpark DataFrame 中。
<a name="UXZfF"></a>
## 目录

- 读取多个 CSV 文件
- 读取目录中的所有 CSV 文件
- 读取 CSV 文件时的选项
   - 分隔符(delimiter)
   - 推断模式(inferschema)
   - 标题(header)
   - 引号(quotes)
   - 空值(nullValues)
   - 日期格式(dateformat)
- 使用用户指定的模式读取 CSV 文件
- 应用 DataFrame 转换
- 将 DataFrame 写入 CSV 文件
   - 使用选项
   - 保存模式
<a name="Iiwo8"></a>
## 1、将 CSV 文件读取到 DataFrame
使用DataFrameReader 的 `csv("path")` 或者 `format("csv").load("path")`，可以将 CSV 文件读入 PySpark DataFrame，这些方法将要读取的文件路径作为参数。当使用 `format("csv")` 方法时，还可以通过完全限定名称指定数据源，但对于内置源，可以简单地使用它们的短名称（csv、json、parquet、jdbc、text 等）。<br />请参阅 GitHub 上的数据集zipcodes.csv。<br />传送门：[https://github.com/spark-examples/pyspark-examples/blob/master/resources/zipcodes.csv](https://github.com/spark-examples/pyspark-examples/blob/master/resources/zipcodes.csv))
```python
spark = SparkSession.builder().master("local[1]")
          .appName("SparkByExamples.com")
          .getOrCreate()
df = spark.read.csv("/tmp/resources/zipcodes.csv")
df.printSchema()
```
使用完全限定的数据源名称，也可以执行以下操作。
```python
df = spark.read.format("csv")
                  .load("/tmp/resources/zipcodes.csv")
# 或者
df = spark.read.format("org.apache.spark.sql.csv")
                  .load("/tmp/resources/zipcodes.csv")
df.printSchema()
```
此示例将数据读取到 DataFrame 列"_c0"中，用于第一列和"_c1"第二列，依此类推。默认情况下，所有这些列的数据类型都被视为字符串。
```
root
 |-- _c0: string (nullable = true)
 |-- _c1: string (nullable = true)
 |-- _c2: string (nullable = true)
```
<a name="GUDpi"></a>
### 1.1 使用标题记录作为列名
如果输入文件中有一个带有列名的标题，则需要使用不提及这一点明确指定标题选项 `option("header", True)`，API 将标题视为数据记录。<br />需要使用`option("header", True)`显式地为`"header"`选项指定为`True`，若不设置，则默认将 `"header"` 标题作为一个数据记录。
```python
df2 = spark.read.option("header",True) \
     .csv("/tmp/resources/zipcodes.csv")

# df2 = spark.read.csv("/tmp/resources/zipcodes.csv",header=True)
```
如前所述，PySpark 默认将所有列读取为字符串（StringType）。在后面学习如何从标题记录中读取 schema (inferschema) 并根据数据派生`inferschema`列类型。
<a name="snjon"></a>
### 1.2 读取多个 CSV 文件
使用`read.csv()`方法还可以读取多个 csv 文件，只需通过逗号分隔作为路径传递所有文件名，例如：
```java
df = spark.read.csv("path1,path2,path3")
```
<a name="suS1c"></a>
### 1.3 读取目录中的所有 CSV 文件
只需将目录作为`csv()`方法的路径传递给该方法，就可以将目录中的所有 CSV 文件读取到 DataFrame 中。
```java
df = spark.read.csv("Folder path")
```
<a name="ZEY6j"></a>
## 2、读取 CSV 文件时的选项
PySpark 提供了多种处理 CSV 数据集文件的选项。以下是通过示例解释的一些最重要的选项。<br />可以使用链接 `option(self, key, value)` 来使用多个选项。该方法有个替代方法：`options(self, **options)`，效果是一样的。
<a name="Ybfth"></a>
### 2.1 Delimiter
选项 delimiter 用于指定 CSV 文件的列分隔符。默认情况下，它是 **逗号(,)** 字符。可使用此选项将其设置为任何字符，例如**管道(|)、制表符 (\t)、空格。** 这都需要根据实际的 CSV 数据集文件的具体形式设定。
```java
df3 = spark.read.options(delimiter=',') \
  .csv("C:/PyDataStudio/zipcodes.csv")
```
<a name="yRbZH"></a>
### 2.2 InferSchema
此选项的默认值是设置为 False，设置为 True 时，spark将自动根据数据推断列类型。
```java
df4 = spark.read.options(inferSchema='True',
                         delimiter=',') \
                .csv("PyDataStudio/zipcodes.csv")
```
或者，也可以通过链接`option()`方法来编写它。
```java
df4 = spark.read.option("inferSchema",True) \
                .option("delimiter",",") \
                .csv("PyDataStudio/zipcodes.csv")
```
<a name="Sbv96"></a>
### 2.3 Header
此选项用于读取 CSV 文件的第一行作为列名。默认情况下，此选项的值为 False ，并且所有列类型都假定为字符串。
```java
df5 = spark.read.options(header='True', 
                         inferSchema='True', 
                         delimiter=',') \
    .csv("PyDataStudio/zipcodes.csv")
```
<a name="Pb1w8"></a>
### 2.4 Quotes
当有一列带有用于拆分列的分隔符时，使用 quotes 选项指定引号字符，默认情况下它是''，并且引号内的分隔符将被忽略。但使用此选项，可以设置任何字符。
<a name="cs6Jn"></a>
### 2.5 NullValues
使用 nullValues 选项，可以将 CSV 中的字符串指定为空。例如，如果将"1900-01-01"在 DataFrame 上将值设置为 null 的日期列。
<a name="Nnh14"></a>
### 2.6 DateFormat
选项 dateFormat 用于设置输入 DateType 和 TimestampType 列的格式的选项。支持所有 `java.text.SimpleDateFormat` 格式。<br />**注意：** 除了上述选项，PySpark CSV API 还支持许多其他选项，可以查阅PySpark官方文档。
<a name="ZaRsi"></a>
## 3、使用用户自定义架构读取 CSV 文件
如果事先知道文件的架构并且不想使用inferSchema选项来指定列名和类型，请使用指定的自定义列名schema并使用`schema`选项键入。
```python
schema = StructType() \
      .add("RecordNumber",IntegerType(),True) \
      .add("Zipcode",IntegerType(),True) \
      .add("ZipCodeType",StringType(),True) \
      .add("City",StringType(),True) \
      .add("State",StringType(),True) \
      .add("LocationType",StringType(),True) \
      .add("Lat",DoubleType(),True) \
      .add("Long",DoubleType(),True) \
      .add("Xaxis",IntegerType(),True) \
      .add("Yaxis",DoubleType(),True) \
      .add("Zaxis",DoubleType(),True) \
      .add("WorldRegion",StringType(),True) \
      .add("Country",StringType(),True) \
      .add("LocationText",StringType(),True) \
      .add("Location",StringType(),True) \
      .add("Decommisioned",BooleanType(),True) \
      .add("TaxReturnsFiled",StringType(),True) \
      .add("EstimatedPopulation",IntegerType(),True) \
      .add("TotalWages",IntegerType(),True) \
      .add("Notes",StringType(),True)
      
df_with_schema = spark.read.format("csv") \
      .option("header", True) \
      .schema(schema) \
      .load("/PyDataStudio/zipcodes.csv")
```
<a name="wBzPF"></a>
## 4、应用 DataFrame 转换
从 CSV 文件创建 DataFrame 后，可以应用 DataFrame 支持的所有转换和操作。
<a name="rOLgg"></a>
## 5、将 DataFrame 写入 CSV 文件
使用PySpark DataFrameWriter 对象的`write()`方法将 PySpark DataFrame 写入 CSV 文件。
```python
df.write.option("header",True) \
 .csv("/PyDataStudio/spark_output/zipcodes")
```
<a name="tFnV3"></a>
### 5.1 Options
在编写 CSV 文件时，可以使用多个选项。例如，设置 header 为 `True` 将 DataFrame 列名作为标题记录输出，并用 `delimiter`在 CSV 输出文件中指定分隔符。
```python
df2.write.options(header='True',
                  delimiter=',') \
         .csv("/PyDataStudio/spark_output/zipcodes")
```
其他可用选项 `quote`, `escape`, `nullValue`, `dateFormat`, `quoteMode`。具体可以查看官方文档。
<a name="gxQKw"></a>
### 5.2 保存mode
PySpark DataFrameWriter 还有一个 `mode()` 方法来指定保存模式。

- `overwrite`– 模式用于覆盖现有文件。
- `append`– 将数据添加到现有文件。
- `ignore`– 当文件已经存在时忽略写操作。
- `error`– 这是一个默认选项，当文件已经存在时，它会返回错误。
```python
df2.write.mode('overwrite') \
         .csv("/PyDataStudio/spark_output/zipcodes")
# 也可以这样写
df2.write.format("csv") \
         .mode('overwrite') \
         .save("/PyDataStudio/spark_output/zipcodes")
```
<a name="Sfq2J"></a>
## 6、PySpark 读取 CSV 完整示例
```python
import pyspark
from pyspark.sql import SparkSession
from pyspark.sql.types import StructType,StructField, StringType, IntegerType 
from pyspark.sql.types import ArrayType, DoubleType, BooleanType
from pyspark.sql.functions import col,array_contains

spark = SparkSession.builder.appName('SparkByExamples.com').getOrCreate()

df = spark.read.csv("/PyDataStudio/zipcodes.csv")

df.printSchema()

df2 = spark.read.option("header",True) \
     .csv("/PyDataStudio/zipcodes.csv")
df2.printSchema()
   
df3 = spark.read.options(header='True', delimiter=',') \
  .csv("/PyDataStudio/zipcodes.csv")
df3.printSchema()

schema = StructType() \
      .add("RecordNumber",IntegerType(),True) \
      .add("Zipcode",IntegerType(),True) \
      .add("ZipCodeType",StringType(),True) \
      .add("City",StringType(),True) \
      .add("State",StringType(),True) \
      .add("LocationType",StringType(),True) \
      .add("Lat",DoubleType(),True) \
      .add("Long",DoubleType(),True) \
      .add("Xaxis",IntegerType(),True) \
      .add("Yaxis",DoubleType(),True) \
      .add("Zaxis",DoubleType(),True) \
      .add("WorldRegion",StringType(),True) \
      .add("Country",StringType(),True) \
      .add("LocationText",StringType(),True) \
      .add("Location",StringType(),True) \
      .add("Decommisioned",BooleanType(),True) \
      .add("TaxReturnsFiled",StringType(),True) \
      .add("EstimatedPopulation",IntegerType(),True) \
      .add("TotalWages",IntegerType(),True) \
      .add("Notes",StringType(),True)
      
df_with_schema = spark.read.format("csv") \
      .option("header", True) \
      .schema(schema) \
      .load("/PyDataStudio/zipcodes.csv")
df_with_schema.printSchema()

df2.write.option("header",True) \
         .csv("/PyDataStudio/spark_output/zipcodes123")
```
