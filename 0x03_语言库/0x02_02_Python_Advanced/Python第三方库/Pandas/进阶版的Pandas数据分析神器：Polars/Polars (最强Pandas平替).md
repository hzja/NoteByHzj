Polars是一个用于操作结构化数据的高性能DataFrame库，可以说是平替pandas最有潜质的包。当然pandas目前作为Python数据分析的核心工具来说还是最强的。Polars其核心部分是用Rust编写的，但该库也提供了Python接口。它的主要特点包括：

- **快速**：Polars是从零开始编写的，紧密与机器结合，没有外部依赖。
- **I/O**：对所有常见数据存储层提供一流支持：本地、云存储和数据库。
- **易于使用**：以原始意图编写查询。Polars 在内部会使用其查询优化器确定执行最有效的方式。
- **离线处理**：Polars支持通过其流式API进行离线数据转换。这使您能够处理结果，而无需同时将所有数据存储在内存中。
- **并行处理**：Polars通过在可用的CPU核心之间分配工作负载，充分利用计算机性能，而无需额外配置。
- **矢量化查询引擎**：Polars使用 Apache Arrow，一种列式数据格式，以矢量化方式处理查询。它使用 SIMD 来优化CPU使用。

User guide：[https://pola-rs.github.io/polars/user-guide/](https://pola-rs.github.io/polars/user-guide/)<br />API reference：[https://pola-rs.github.io/polars/py-polars/html/reference/io.html](https://pola-rs.github.io/polars/py-polars/html/reference/io.html)
<a name="BvNth"></a>
## 介绍
Polars 的目标是提供一个闪电般快速的 DataFrame 库，具有以下特点：

- 利用计算机上所有可用的核心。
- 通过优化查询来减少不必要的工作/内存分配。
- 处理比可用 RAM 更大得多的数据集。
- 具有一致且可预测的 API。
- 具有严格的模式（在运行查询之前应该知道数据类型）。

Polars 是用 Rust 编写的，这使得它具有 C/C++ 性能，并允许它完全控制查询引擎中的性能关键部分。因此，Polars 为此付出了很大的努力：

- 减少冗余的复制。
- 高效地遍历内存缓存。
- 在并行性中最小化争用。
- 以块处理数据。
- 重用内存分配。
<a name="ZjIrE"></a>
## **基础**
<a name="wFy8D"></a>
### Series & DataFrames
Series 是一个一维数据结构。在一个 Series 中，所有元素都具有相同的数据类型（例如，整数、字符串）。下面的片段展示了如何创建一个简单的带有名称的 Series 对象。
```python
import polars as pl
import numpy as np

s = pl.Series("a", [1, 2, 3, 4, 5])
print(s)
s = pl.Series("a", [1, 2, 3, 4, 5])
print(s.min())
print(s.max())
s = pl.Series("a", ["polar", "bear", "arctic", "polar fox", "polar bear"])
s2 = s.str.replace("polar", "pola")
print(s2)
from datetime import date

start = date(2001, 1, 1)
stop = date(2001, 1, 9)
s = pl.date_range(start, stop, interval="2d", eager=True)
print(s.dt.day())
```
DataFrame 是一个二维数据结构，由一个或多个 Series 支持，可以看作是对一系列（例如列表）Series的抽象。在 DataFrame 上可以执行的操作与在 SQL 查询中执行的操作非常相似。可以进行 GROUP BY、JOIN、PIVOT，还可以定义自定义函数。
```python
from datetime import datetime

df = pl.DataFrame(
    {
        "integer": [1, 2, 3, 4, 5],
        "date": [
            datetime(2022, 1, 1),
            datetime(2022, 1, 2),
            datetime(2022, 1, 3),
            datetime(2022, 1, 4),
            datetime(2022, 1, 5),
        ],
        "float": [4.0, 5.0, 6.0, 7.0, 8.0],
    }
)

print(df)
print(df.head(3))
print(df.describe())
```
<a name="T88Ky"></a>
### Reading & writing
```python
import polars as pl
from datetime import datetime

df = pl.DataFrame(
    {
        "integer": [1, 2, 3],
        "date": [
            datetime(2022, 1, 1),
            datetime(2022, 1, 2),
            datetime(2022, 1, 3),
        ],
        "float": [4.0, 5.0, 6.0],
    }
)

print(df)
df.write_csv("output.csv")
df_csv = pl.read_csv("output.csv")
print(df_csv)
df_csv = pl.read_csv("output.csv", try_parse_dates=True)
print(df_csv)
```
<a name="m3Sj5"></a>
### Expressions
```python
import polars as pl

# 创建一个简单的 DataFrame
data = {'column1': [1, 2, 3],
        'column2': ['a', 'b', 'c']}
df = pl.DataFrame(data)

# 使用表达式进行选择
selected_df = df.select(['column1'])

# 使用表达式进行过滤
filtered_df = df.filter(df['column1'] > 1)
selected_df
filtered_df
```
<a name="HDGGV"></a>
## **拼接**
```python
df = pl.DataFrame(
    {
        "a": np.arange(0, 8),
        "b": np.random.rand(8),
        "d": [1, 2.0, np.NaN, np.NaN, 0, -5, -42, None],
    }
)

df2 = pl.DataFrame(
    {
        "x": np.arange(0, 8),
        "y": ["A", "A", "A", "B", "B", "C", "X", "X"],
    }
)
joined = df.join(df2, left_on="a", right_on="x")
print(joined)
stacked = df.hstack(df2)
print(stacked)
```
<a name="qIt0A"></a>
## **概念**
<a name="lHw1e"></a>
### Data types
Polars 完全基于 Arrow 数据类型，并由 Arrow 内存数组支持。这使得数据处理在缓存效率和跨进程通信方面得到良好支持。大多数数据类型都与 Arrow 的实现完全一致，但有一些例外，如 Utf8（实际上是 LargeUtf8）、Categorical 和 Object（支持有限）等。以下是一些数据类型：

| 分组 | 类型 | 详细信息 |
| --- | --- | --- |
| 数值 | Int8 | 8 位有符号整数。 |
| <br /> | Int16 | 16 位有符号整数。 |
| <br /> | Int32 | 32 位有符号整数。 |
| <br /> | Int64 | 64 位有符号整数。 |
| <br /> | UInt8 | 8 位无符号整数。 |
| <br /> | UInt16 | 16 位无符号整数。 |
| <br /> | UInt32 | 32 位无符号整数。 |
| <br /> | UInt64 | 64 位无符号整数。 |
|  | Float32 | 32 位浮点数。 |
| <br /> | Float64 | 64 位浮点数。 |
| 嵌套 | Struct | 结构数组表示为 Vec<Series>，用于在单个列中打包多个/异构值。 |
| <br /> | List | 列表数组包含包含列表值的子数组和一个偏移数组（在内部实际上是 Arrow 的 LargeList）。 |
| 时间 | Date | 日期表示，内部表示为距离 UNIX 纪元的天数，由 32 位有符号整数编码。 |
| <br /> | Datetime | 日期时间表示，内部表示为距离 UNIX 纪元的微秒数，由 64 位有符号整数编码。 |
| <br /> | Duration | 表示时间差的类型，内部表示为微秒。通过减去 Date/Datetime 创建。 |
| <br /> | Time | 时间表示，内部表示为距午夜的纳秒数。 |
| 其他 | Boolean | 布尔类型，实际上是按位打包的。 |
| <br /> | Utf8 | 字符串数据（实际上在内部是 Arrow 的 LargeUtf8）。 |
| <br /> | Binary | 以字节形式存储数据。 |
| <br /> | Object | 有限支持的数据类型，可以是任何值。 |
| <br /> | Categorical | 一组字符串的分类编码。 |

<a name="ybslX"></a>
### Contexts
Polars 已经开发了自己的领域特定语言（DSL）用于数据转换。该语言非常易于使用，允许进行复杂的查询，同时保持人类可读性。该语言的两个核心组件是上下文（Contexts）和表达式（Expressions），将在下一部分介绍表达式。<br />正如名称所示，上下文指的是需要评估表达式的上下文。有三个主要的上下文 [^1^]：

1. 选择（Selection）：`df.select([..]), df.with_columns([..])`
2. 过滤（Filtering）：`df.filter()`
3. 分组/聚合（Group by / Aggregation）：`df.group_by(..).agg([..])`
```python
df = pl.DataFrame(
    {
        "nrs": [1, 2, 3, None, 5],
        "names": ["foo", "ham", "spam", "egg", None],
        "random": np.random.rand(5),
        "groups": ["A", "A", "B", "C", "B"],
    }
)
print(df)
# 基于df 进行计算，得到新的表格
out = df.select(
    pl.sum("nrs"), # nrs的和
    pl.col("names").sort(), # names排序后的结果
    pl.col("names").first().alias("first name"), # names第一个值
    (pl.mean("nrs") * 10).alias("10xnrs"), # nrs的均值 * 10
)
print(out)
# 原始df 新增列
df = df.with_columns(
    pl.sum("nrs").alias("nrs_sum"),
    pl.col("random").count().alias("count"),
)
print(df)
out = df.filter(pl.col("nrs") > 2)
print(out)
out = df.group_by("groups").agg(
    pl.sum("nrs"),  # sum nrs by groups
    pl.col("random").count().alias("count"),  # count group members
    # sum random where name != null
    pl.col("random").filter(pl.col("names").is_not_null()).sum().name.suffix("_sum"),
    pl.col("names").reverse().alias("reversed names"),
)
print(out)
```
<a name="cSoAy"></a>
### Lazy / eager API
Polars支持两种操作模式：lazy（延迟）和eager（即时）。在eager API中，查询会立即执行，而在lazy API中，查询只有在“需要”时才会被评估。
```python
!wget https://mirror.coggle.club/dataset/heart.csv
!head heart.csv
df = pl.read_csv("heart.csv")
df_small = df.filter(pl.col("age") > 5)
df_agg = df_small.group_by("sex").agg(pl.col("chol").mean())
print(df_agg)
q = (
    pl.scan_csv("heart.csv")
    .filter(pl.col("age") > 5)
    .group_by("sex")
    .agg(pl.col("chol").mean())
)
# 生成了计算逻辑

df = q.collect() # 真正计算
print(df)
```
<a name="qXCrC"></a>
### Streaming API
[https://pola-rs.github.io/polars/user-guide/concepts/streaming/](https://pola-rs.github.io/polars/user-guide/concepts/streaming/)<br />Lazy API的另一个额外好处是它允许以流式方式执行查询。与一次性处理所有数据不同，Polars可以按批执行查询，能够处理大于内存的数据集。
```python
q = (
    pl.scan_csv("heart.csv")
    .filter(pl.col("age") > 5)
    .group_by("sex")
    .agg(pl.col("chol").mean())
)

df = q.collect(streaming=True)
print(df)
```
<a name="p67by"></a>
## **表达式**
<a name="Vva1K"></a>
### Basic operators
```python
df = pl.DataFrame(
    {
        "nrs": [1, 2, 3, None, 5],
        "names": ["foo", "ham", "spam", "egg", None],
        "random": np.random.rand(5),
        "groups": ["A", "A", "B", "C", "B"],
    }
)
print(df)
df_numerical = df.select(
    (pl.col("nrs") + 5).alias("nrs + 5"),
    (pl.col("nrs") - 5).alias("nrs - 5"),
    (pl.col("nrs") * pl.col("random")).alias("nrs * random"),
    (pl.col("nrs") / pl.col("random")).alias("nrs / random"),
)
print(df_numerical)
df_logical = df.select(
    (pl.col("nrs") > 1).alias("nrs > 1"),
    (pl.col("random") <= 0.5).alias("random <= .5"),
    (pl.col("nrs") != 1).alias("nrs != 1"),
    (pl.col("nrs") == 1).alias("nrs == 1"),
    ((pl.col("random") <= 0.5) & (pl.col("nrs") > 1)).alias("and_expr"),  # and
    ((pl.col("random") <= 0.5) | (pl.col("nrs") > 1)).alias("or_expr"),  # or
)
print(df_logical)
```
<a name="ydY9W"></a>
### Column selections
```python
from datetime import date, datetime

df = pl.DataFrame(
    {
        "id": [9, 4, 2],
        "place": ["Mars", "Earth", "Saturn"],
        "date": pl.date_range(date(2022, 1, 1), date(2022, 1, 3), "1d", eager=True),
        "sales": [33.4, 2142134.1, 44.7],
        "has_people": [False, True, False],
        "logged_at": pl.datetime_range(
            datetime(2022, 12, 1), datetime(2022, 12, 1, 0, 0, 2), "1s", eager=True
        ),
    }
).with_row_count("rn")
print(df)
out = df.select(pl.col("*"))

# Is equivalent to
out = df.select(pl.all())
print(out)
out = df.select(pl.col("*").exclude("logged_at", "rn"))
print(out)
out = df.select(pl.col("date", "logged_at").dt.to_string("%Y-%h-%d"))
print(out)
out = df.select(pl.col("^.*(as|sa).*$"))
print(out)
out = df.select(pl.col(pl.Int64, pl.UInt32, pl.Boolean).n_unique())
print(out)
import polars.selectors as cs
out = df.select(cs.numeric() - cs.first())
print(out)
out = df.select(cs.contains("rn"), cs.matches(".*_.*"))
print(out)
```
<a name="kGeXD"></a>
### Functions
```python
df = pl.DataFrame(
    {
        "nrs": [1, 2, 3, None, 5],
        "names": ["foo", "ham", "spam", "egg", "spam"],
        "random": np.random.rand(5),
        "groups": ["A", "A", "B", "C", "B"],
    }
)
print(df)
df_samename = df.select(pl.col("nrs") + 5)
print(df_samename)
df_alias = df.select(
    (pl.col("nrs") + 5).alias("nrs + 5"),
    (pl.col("nrs") - 5).alias("nrs - 5"),
)
print(df_alias)
df_alias = df.select(
    pl.col("names").n_unique().alias("unique"),
    pl.approx_n_unique("names").alias("unique_approx"),
)
print(df_alias)
df_conditional = df.select(
    pl.col("nrs"),
    pl.when(pl.col("nrs") > 2)
    .then(pl.lit(True))
    .otherwise(pl.lit(False))
    .alias("conditional"),
)
print(df_conditional)
```
<a name="z8SHj"></a>
## **转换**
类型转换（Casting）将列的底层 DataType 转换为新的数据类型。Polars 使用 Arrow 在内存中管理数据，并依赖于 Rust 实现中的计算核心 来执行转换。类型转换通过 `cast()` 方法实现。<br />cast 方法包括一个 strict 参数，该参数确定当 Polars 遇到无法从源 DataType 转换为目标 DataType 的值时的行为。默认情况下，`strict=True`，这意味着 Polars 将引发错误，通知用户转换失败，并提供无法转换的值的详细信息。另一方面，如果 `strict=False`，无法转换为目标 DataType 的任何值都将悄悄地转换为 null。
```python
df = pl.DataFrame(
    {
        "integers": [1, 2, 3, 4, 5],
        "big_integers": [1, 10000002, 3, 10000004, 10000005],
        "floats": [4.0, 5.0, 6.0, 7.0, 8.0],
        "floats_with_decimal": [4.532, 5.5, 6.5, 7.5, 8.5],
    }
)

print(df)
out = df.select(
    pl.col("integers").cast(pl.Float32).alias("integers_as_floats"),
    pl.col("floats").cast(pl.Int32).alias("floats_as_integers"),
    pl.col("floats_with_decimal")
    .cast(pl.Int32)
    .alias("floats_with_decimal_as_integers"),
)
print(out)
out = df.select(
    pl.col("integers").cast(pl.Int16).alias("integers_smallfootprint"),
    pl.col("floats").cast(pl.Float32).alias("floats_smallfootprint"),
)
print(out)
df = pl.DataFrame(
    {
        "integers": [1, 2, 3, 4, 5],
        "float": [4.0, 5.03, 6.0, 7.0, 8.0],
        "floats_as_string": ["4.0", "5.0", "6.0", "7.0", "8.0"],
    }
)

out = df.select(
    pl.col("integers").cast(pl.Utf8),
    pl.col("float").cast(pl.Utf8),
    pl.col("floats_as_string").cast(pl.Float64),
)
print(out)
df = pl.DataFrame(
    {
        "integers": [-1, 0, 2, 3, 4],
        "floats": [0.0, 1.0, 2.0, 3.0, 4.0],
        "bools": [True, False, True, False, True],
    }
)

out = df.select(pl.col("integers").cast(pl.Boolean), pl.col("floats").cast(pl.Boolean))
print(out)
from datetime import date, datetime

df = pl.DataFrame(
    {
        "date": pl.date_range(date(2022, 1, 1), date(2022, 1, 5), eager=True),
        "datetime": pl.datetime_range(
            datetime(2022, 1, 1), datetime(2022, 1, 5), eager=True
        ),
    }
)

out = df.select(pl.col("date").cast(pl.Int64), pl.col("datetime").cast(pl.Int64))
print(out)
```
<a name="qs7oa"></a>
### Strings
```python
df = pl.DataFrame({"animal": ["Crab", "cat and dog", "rab$bit", None]})

out = df.select(
    pl.col("animal").str.len_bytes().alias("byte_count"),
    pl.col("animal").str.len_chars().alias("letter_count"),
)
print(out)
out = df.select(
    pl.col("animal"),
    pl.col("animal").str.contains("cat|bit").alias("regex"),
    pl.col("animal").str.contains("rab$", literal=True).alias("literal"),
    pl.col("animal").str.starts_with("rab").alias("starts_with"),
    pl.col("animal").str.ends_with("dog").alias("ends_with"),
)
print(out)
```
<a name="iTjhB"></a>
### Aggregation
[https://pola-rs.github.io/polars/user-guide/expressions/aggregation/](https://pola-rs.github.io/polars/user-guide/expressions/aggregation/)
```python
df = pl.read_csv("heart.csv")
print(df)
q = (
    df.lazy()
    .group_by("sex")
    .agg(
        pl.count(),
        pl.col("cp"),
        pl.first("caa"),
    )
    .sort("count", descending=True)
    .limit(5)
)

df = q.collect()
print(df)
q = (
    df.lazy()
    .group_by("sex")
    .agg(
        (pl.col("cp") == 1).sum().alias("anti"),
        (pl.col("cp") == 2).sum().alias("pro"),
    )
    .sort("pro", descending=True)
    .limit(5)
)

df = q.collect()
print(df)
```
<a name="NNp5q"></a>
## **缺失值**
```python
df = pl.DataFrame(
    {
        "value": [1, None],
    },
)
print(df)
null_count_df = df.null_count()
print(null_count_df)
df = pl.DataFrame(
    {
        "col1": [1, 2, 3],
        "col2": [1, None, 3],
    },
)
print(df)
fill_literal_df = df.with_columns(
    pl.col("col2").fill_null(pl.lit(2)),
)
print(fill_literal_df)
fill_forward_df = df.with_columns(
    pl.col("col2").fill_null(strategy="forward"),
)
print(fill_forward_df)
fill_median_df = df.with_columns(
    pl.col("col2").fill_null(pl.median("col2")),
)
print(fill_median_df)
fill_interpolation_df = df.with_columns(
    pl.col("col2").interpolate(),
)
print(fill_interpolation_df)
```
<a name="lOLCp"></a>
### Window functions
[https://pola-rs.github.io/polars/user-guide/expressions/window/](https://pola-rs.github.io/polars/user-guide/expressions/window/)
```python
!wget https://cdn.coggle.club/Pokemon.csv
!head Pokemon.csv
# then let's load some csv data with information about pokemon
df = pl.read_csv("Pokemon.csv")
print(df.head())
out = df.select(
    "Type 1",
    "Type 2",
    pl.col("Attack").mean().over("Type 1").alias("avg_attack_by_type"),
    pl.col("Defense")
    .mean()
    .over(["Type 1", "Type 2"])
    .alias("avg_defense_by_type_combination"),
    pl.col("Attack").mean().alias("avg_attack"),
)
print(out)
filtered = df.filter(pl.col("Type 2") == "Psychic").select(
    "Name",
    "Type 1",
    "Speed",
)
print(filtered)
out = filtered.with_columns(
    pl.col(["Name", "Speed"]).sort_by("Speed", descending=True).over("Type 1"),
)
print(out)
```
<a name="SDLao"></a>
### Lists and Arrays
```python
weather = pl.DataFrame(
    {
        "station": ["Station " + str(x) for x in range(1, 6)],
        "temperatures": [
            "20 5 5 E1 7 13 19 9 6 20",
            "18 8 16 11 23 E2 8 E2 E2 E2 90 70 40",
            "19 24 E9 16 6 12 10 22",
            "E2 E0 15 7 8 10 E1 24 17 13 6",
            "14 8 E0 16 22 24 E1",
        ],
    }
)
print(weather)
out = weather.with_columns(pl.col("temperatures").str.split(" "))
print(out)
out = weather.with_columns(pl.col("temperatures").str.split(" ")).explode(
    "temperatures"
)
print(out)
out = weather.with_columns(pl.col("temperatures").str.split(" ")).with_columns(
    pl.col("temperatures").list.head(3).alias("top3"),
    pl.col("temperatures").list.slice(-3, 3).alias("bottom_3"),
    pl.col("temperatures").list.len().alias("obs"),
)
print(out)
```
<a name="BNLgM"></a>
## **变形**
<a name="RTjx9"></a>
### Joins
| 策略 | 描述 |
| --- | --- |
| inner | 返回两个数据框中具有匹配键的行。左框或右框中的非匹配行将被丢弃。 |
| left | 返回左数据框中的所有行，无论是否在右数据框中找到匹配项。非匹配行的右列将被填充为null。 |
| outer | 返回左右两个数据框中的所有行。如果在一个框中找不到匹配项，则从另一个框中的列将被填充为null。 |
| cross | 返回左框中的所有行与右框中的所有行的笛卡尔积。重复的行将被保留；左框与右框的交叉连接的表长度始终为len(A) × len(B)。 |
| asof | 在此连接中，匹配是根据_最近_的键而不是相等的键执行的左连接。 |
| semi | 返回左框中具有与右框中相同的连接键的所有行。 |
| anti | 返回左框中连接键_不_在右框中出现的所有行。 |

```python
df_customers = pl.DataFrame(
    {
        "customer_id": [1, 2, 3],
        "name": ["Alice", "Bob", "Charlie"],
    }
)
print(df_customers)
df_orders = pl.DataFrame(
    {
        "order_id": ["a", "b", "c"],
        "customer_id": [1, 2, 2],
        "amount": [100, 200, 300],
    }
)
print(df_orders)
df = df_customers.join(df_orders, on="customer_id", how="inner")
print(df)

df = df_customers.join(df_orders, on="customer_id", how="left")
print(df)

df = df_customers.join(df_orders, on="customer_id", how="outer")
print(df)

df = df_customers.join(df_orders, on="customer_id", how="cross")
print(df)
df_cars = pl.DataFrame(
    {
        "id": ["a", "b", "c"],
        "make": ["ford", "toyota", "bmw"],
    }
)
print(df_cars)
df_repairs = pl.DataFrame(
    {
        "id": ["c", "c"],
        "cost": [100, 200],
    }
)
print(df_repairs)
df = df_cars.join(df_repairs, on="id", how="inner")
print(df)

df = df_cars.join(df_repairs, on="id", how="semi")
print(df)

df = df_cars.join(df_repairs, on="id", how="anti")
print(df)
df_trades = pl.DataFrame(
    {
        "time": [
            datetime(2020, 1, 1, 9, 1, 0),
            datetime(2020, 1, 1, 9, 1, 0),
            datetime(2020, 1, 1, 9, 3, 0),
            datetime(2020, 1, 1, 9, 6, 0),
        ],
        "stock": ["A", "B", "B", "C"],
        "trade": [101, 299, 301, 500],
    }
)
print(df_trades)

df_quotes = pl.DataFrame(
    {
        "time": [
            datetime(2020, 1, 1, 9, 0, 0),
            datetime(2020, 1, 1, 9, 2, 0),
            datetime(2020, 1, 1, 9, 4, 0),
            datetime(2020, 1, 1, 9, 6, 0),
        ],
        "stock": ["A", "B", "C", "A"],
        "quote": [100, 300, 501, 102],
    }
)

print(df_quotes)
df_asof_join = df_trades.join_asof(df_quotes, on="time", by="stock")
print(df_asof_join)
df_asof_tolerance_join = df_trades.join_asof(
    df_quotes, on="time", by="stock", tolerance="1m"
)
print(df_asof_tolerance_join)
```
<a name="x6ppQ"></a>
### Concatenation
```python
df_v1 = pl.DataFrame(
    {
        "a": [1],
        "b": [3],
    }
)
df_v2 = pl.DataFrame(
    {
        "a": [2],
        "b": [4],
    }
)
df_vertical_concat = pl.concat(
    [
        df_v1,
        df_v2,
    ],
    how="vertical",
)
print(df_vertical_concat)
df_h1 = pl.DataFrame(
    {
        "l1": [1, 2],
        "l2": [3, 4],
    }
)
df_h2 = pl.DataFrame(
    {
        "r1": [5, 6],
        "r2": [7, 8],
        "r3": [9, 10],
    }
)
df_horizontal_concat = pl.concat(
    [
        df_h1,
        df_h2,
    ],
    how="horizontal",
)
print(df_horizontal_concat)
```
<a name="Qf7Po"></a>
### Pivots
```python
df = pl.DataFrame(
    {
        "foo": ["A", "A", "B", "B", "C"],
        "N": [1, 2, 2, 4, 2],
        "bar": ["k", "l", "m", "n", "o"],
    }
)
print(df)
out = df.pivot(index="foo", columns="bar", values="N", aggregate_function="first")
print(out)
q = (
    df.lazy()
    .collect()
    .pivot(index="foo", columns="bar", values="N", aggregate_function="first")
    .lazy()
)
out = q.collect()
print(out)
```
<a name="zoB8L"></a>
### Melts
```python
import polars as pl

df = pl.DataFrame(
    {
        "A": ["a", "b", "a"],
        "B": [1, 3, 5],
        "C": [10, 11, 12],
        "D": [2, 4, 6],
    }
)
print(df)
out = df.melt(id_vars=["A", "B"], value_vars=["C", "D"])
print(out)
```
