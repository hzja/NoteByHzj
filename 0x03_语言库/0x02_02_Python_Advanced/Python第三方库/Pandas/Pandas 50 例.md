Python Pandas
<a name="qiOVW"></a>
## 准备需要的数据集
先准备生成一些随机数，作为后面需要用到的数据集
```python
index = pd.date_range("1/1/2000", periods=8)

series = pd.Series(np.random.randn(5), index=["a", "b", "c", "d", "e"])

df = pd.DataFrame(np.random.randn(8, 3), index=index, columns=["A", "B", "C"])
```
<a name="Ar49N"></a>
## Head and tail
`head()`和`tail()`方法是用来查看数据集当中的前几行和末尾几行的，默认是查看5行，当然也可以自行设定行数
```python
series2 = pd.Series(np.random.randn(100))
series2.head()
```
output
```python
0    0.733801
1   -0.740149
2   -0.031863
3    2.515542
4    0.615291
dtype: float64
```
同理
```python
series2.tail()
```
output
```python
95   -0.526625
96   -0.234975
97    0.744299
98    0.434843
99   -0.609003
dtype: float64
```
<a name="skjHf"></a>
## 数据的统计分析
在pandas当中用`describe()`方法来对表格中的数据做一个概括性的统计分析，例如
```python
series2.describe()
```
output
```python
count    100.000000
mean       0.040813
std        1.003012
min       -2.385316
25%       -0.627874
50%       -0.029732
75%        0.733579
max        2.515542
dtype: float64
```
当然，也可以设置好输出的分位
```python
series2.describe(percentiles=[0.05, 0.25, 0.75, 0.95])
```
output
```python
count    100.000000
mean       0.040813
std        1.003012
min       -2.385316
5%        -1.568183
25%       -0.627874
50%       -0.029732
75%        0.733579
95%        1.560211
max        2.515542
dtype: float64
```
对于**离散型**的数据来说，`describe()`方法给出的结果则会简洁很多
```python
s = pd.Series(["a", "a", "b", "b", "a", "a", "d", "c", "d", "a"])
s.describe()
```
output
```python
count     10
unique     4
top        a
freq       5
dtype: object
```
要是表格中既包含了离散型数据，也包含了连续型的数据，默认的话，`describe()`是会针对**连续型数据**进行统计分析
```python
df2 = pd.DataFrame({"a": ["Yes", "Yes", "No", "No"], "b": np.random.randn(4)})
df2.describe()
```
output
```python
              b
count  4.000000
mean   0.336053
std    1.398306
min   -1.229344
25%   -0.643614
50%    0.461329
75%    1.440995
max    1.650898
```
当然也可以指定让其强制统计分析离散型数据或者连续型数据
```python
df2.describe(include=["object"])
```
output
```python
          a
count     4
unique    2
top     Yes
freq      2
```
同理，也可以指定连续型的数据进行统计分析
```python
df2.describe(include=["number"])
```
output
```python
              b
count  4.000000
mean  -0.593695
std    0.686618
min   -1.538640
25%   -0.818440
50%   -0.459147
75%   -0.234401
max    0.082155
```
如果都要去做统计分析，可以这么来执行
```python
df2.describe(include="all")
```
output
```python
          a         b
count     4  4.000000
unique    2       NaN
top     Yes       NaN
freq      2       NaN
mean    NaN  0.292523
std     NaN  1.523908
min     NaN -1.906221
25%     NaN -0.113774
50%     NaN  0.789560
75%     NaN  1.195858
max     NaN  1.497193
```
<a name="i9pHF"></a>
## 最大/最小值的位置
`idxmin()`和`idxmax()`方法是用来查找表格当中最大/最小值的**位置**，返回的是值的索引
```python
s1 = pd.Series(np.random.randn(5))
s1
```
output
```python
s1.idxmin(), s1.idxmax()
```
output
```python
(0, 3)
```
用在DataFrame上面的话，如下
```python
df1 = pd.DataFrame(np.random.randn(5, 3), columns=["A", "B", "C"])
df1.idxmin(axis=0)
```
output
```python
A    4
B    2
C    1
dtype: int64
```
同理，将`axis`参数改成1
```python
df1.idxmin(axis=1)
```
output
```python
0    C
1    C
2    C
3    B
4    A
dtype: object
```
<a name="oINDa"></a>
## `value_counts()`方法
pandas当中的`value_counts()`方法主要用于数据表的计数以及排序，用来查看表格当中，指定列有多少个不同的数据值并且计算不同值在该列当中出现的**次数**，先来看一个简单的例子
```python
df = pd.DataFrame({'城市': ['北京', '广州', '上海', '上海', '杭州', '成都', '香港', '南京', '北京', '北京'],
                   '收入': [10000, 10000, 5500, 5500, 4000, 50000, 8000, 5000, 5200, 5600],
                   '年龄': [50, 43, 34, 40, 25, 25, 45, 32, 25, 25]})
df["城市"].value_counts()
```
output
```python
北京    3
上海    2
广州    1
杭州    1
成都    1
香港    1
南京    1
Name: 城市, dtype: int64
```
可以看到北京出现了3次，上海出现了2次，并且默认采用的是**降序**来排列的，下面来看一下用升序的方式来排列一下收入这一列
```python
df["收入"].value_counts(ascending=True)
```
output
```python
4000     1
50000    1
8000     1
5000     1
5200     1
5600     1
10000    2
5500     2
Name: 收入, dtype: int64
```
同时里面也还可以利用参数`normalize=True`，来计算不同值的计数占比
```python
df['年龄'].value_counts(ascending=True,normalize=True)
```
output
```python
50    0.1
43    0.1
34    0.1
40    0.1
45    0.1
32    0.1
25    0.4
Name: 年龄, dtype: float64
```
<a name="NWwzp"></a>
## 数据分组
可以使用`cut()`方法以及`qcut()`方法来对表格中的连续型数据分组，首先看一下`cut()`方法，假设下面这组数据代表的是小组每个成员的年龄
```python
ages = np.array([2,3,10,40,36,45,58,62,85,89,95,18,20,25,35,32])
pd.cut(ages, 5)
```
output
```python
[(1.907, 20.6], (1.907, 20.6], (1.907, 20.6], (39.2, 57.8], (20.6, 39.2], ..., (1.907, 20.6], (1.907, 20.6], (20.6, 39.2], (20.6, 39.2], (20.6, 39.2]]
Length: 16
Categories (5, interval[float64, right]): [(1.907, 20.6] < (20.6, 39.2] < (39.2, 57.8] <
                                           (57.8, 76.4] < (76.4, 95.0]]
```
由上可以看到用`cut()`方法将数据平分成了5个区间，且区间两边都有扩展以包含**最大值和最小值**，当然也可以给每一个区间加上标记
```python
pd.cut(ages, 5, labels=[u"婴儿",u"少年",u"青年",u"中年",u"老年"])
```
output
```python
['婴儿', '婴儿', '婴儿', '青年', '少年', ..., '婴儿', '婴儿', '少年', '少年', '少年']
Length: 16
Categories (5, object): ['婴儿' < '少年' < '青年' < '中年' < '老年']
```
而对于`qcut()`方法来说，可以指定区间来进行分组，例如
```python
pd.qcut(ages, [0,0.5,1], labels=['小朋友','大孩子'])
```
output
```python
['小朋友', '小朋友', '小朋友', '大孩子', '大孩子', ..., '小朋友', '小朋友', '小朋友', '小朋友', '小朋友']
Length: 16
Categories (2, object): ['小朋友' < '大孩子']
```
这里将年龄这组数据分成两部分[0, 0.5, 1]，一组是标上标记小朋友，另一组是大孩子，不过通常情况下，用的`cut()`方法比较多
<a name="c08AM"></a>
## 引用函数
要是在表格当中引用其他的方法，或者是自建的函数，可以使用通过pandas当中的以下这几个方法

- `pipe()`
- `apply()`和`applymap()`
- `agg()`和`transform()`
<a name="x48nb"></a>
### `pipe()`方法
首先来看`pipe()`这个方法，可以将自己定义好的函数，以链路的形式一个接着一个传给要处理的数据集上
```python
def extract_city_name(df):
    df["state_name"] = df["state_and_code"].str.split(",").str.get(0)
    return df

def add_country_name(df, country_name=None):
    df["state_and_country"] = df["state_name"] + country_name
    return df
```
然后用`pip()`这个方法来将上面定义的函数串联起来
```python
df_p = pd.DataFrame({"city_and_code": ["Arizona, AZ"]})
df_p = pd.DataFrame({"state_and_code": ["Arizona, AZ"]})
df_p.pipe(extract_city_name).pipe(add_country_name, country_name="_USA")
```
output
```python
  state_and_code state_name state_and_country
0    Arizona, AZ    Arizona       Arizona_USA
```
<a name="nqqJm"></a>
### `apply()`方法和`applymap()`方法
`apply()`方法可以对表格中的数据按照行或者是列方向进行处理，默认是按照列方向，如下
```python
df.apply(np.mean)
```
output
```python
A   -0.101751
B   -0.360288
C   -0.637433
dtype: float64
```
当然，也可以通过`axis`参数来进行调节
```python
df.apply(np.mean, axis = 1)
```
output
```python
0   -0.803675
1   -0.179640
2   -1.200973
3    0.156888
4    0.381631
5    0.049274
6    1.174923
7    0.612591
dtype: float64
```
除此之外，也可以直接调用匿名函数`lambda`的形式
```python
df.apply(lambda x: x.max() - x.min())
```
output
```python
A    1.922863
B    2.874672
C    1.943930
dtype: float64
```
也可以调用自己定义的函数方法
```python
df = pd.DataFrame(np.random.randn(5, 3), columns=["A", "B", "C"])
def normalize(x):
    return (x - x.mean()) / x.std()
```
用上`apply()`方法
```python
df.apply(normalize)
```
output
```python
          A         B         C
0  1.149795  0.390263 -0.813770
1  0.805843 -0.532374  0.859627
2  0.047824 -0.085334 -0.067179
3 -0.903319 -1.215023  1.149538
4 -1.100144  1.442467 -1.128216
```
`apply()`方法作用于数据集当中的**每个行或者是列**，而`applymap()`方法则是对数据集当中的**所有元素都进行处理**
```python
df = pd.DataFrame({'key1' : ['a', 'c', 'b', 'b', 'd'],
                   'key2' : ['one', 'two', 'three', 'two', 'one'],
                   'data1' : np.arange(1, 6),
                   'data2' : np.arange(10,15)})
```
output
```python
  key1   key2  data1  data2
0    a    one      1     10
1    c    two      2     11
2    b  three      3     12
3    b   four      4     13
4    d   five      5     14
```
来自定义一个函数
```python
def add_A(x):
    return "A" + str(x)
    
df.applymap(add_A)
```
output
```python
  key1    key2 data1 data2
0   Aa    Aone    A1   A10
1   Ac    Atwo    A2   A11
2   Ab  Athree    A3   A12
3   Ab   Afour    A4   A13
4   Ad   Afive    A5   A14
```
然后也可以通过`lambda()`自定义函数方法，然后来去除掉这个`A`
```python
df.applymap(add_A).applymap(lambda x: x.split("A")[1])
```
output
```python
  key1   key2 data1 data2
0    a    one     1    10
1    c    two     2    11
2    b  three     3    12
3    b   four     4    13
4    d   five     5    14
```
<a name="bfd6i"></a>
## `agg()`方法和`transform()`方法
`agg()`方法本意上是聚合函数，可以将用于统计分析的**一系列方法**都放置其中，并且放置多个
```python
df = pd.DataFrame(np.random.randn(5, 3), columns=["A", "B", "C"])
df.agg(np.sum)
```
output
```python
A    0.178156
B    3.233845
C   -0.859622
dtype: float64
```
当然，当中的`np.sum`部分也可以用字符串来表示，例如
```python
df.agg("sum")
```
output
```python
A   -0.606484
B   -1.491742
C   -1.732083
dtype: float64
```
尝试在当中放置多个统计分析的函数方法
```python
df.agg(["sum", "mean", "median"])
```
output
```python
               A         B         C
sum     1.964847  3.855801  0.630042
mean    0.392969  0.771160  0.126008
median  0.821005  0.714804 -0.273685
```
当然也可以和`lambda`匿名函数混合着搭配
```python
df.agg(["sum", lambda x: x.mean()])
```
output
```python
                 A         B         C
sum      -0.066486 -1.288341 -1.236244
<lambda> -0.013297 -0.257668 -0.247249
```
或者和自己定义的函数方法混合着用
```python
def my_mean(x):
    return x.mean()
    
df.agg(["sum", my_mean])
```
output
```python
                A         B         C
sum     -4.850201 -1.544773  0.429007
my_mean -0.970040 -0.308955  0.085801
```
与此同时，在`agg()`方法中添加字典，实现不同的列使用不同的函数方法
```python
df.agg({"A": "sum", "B": "mean"})
```
output
```python
A   -0.801753
B    0.097550
dtype: float64
```
例如
```python
df.agg({"A": ["sum", "min"], "B": "mean"})
```
output
```python
             A         B
sum   0.911243       NaN
min  -0.720225       NaN
mean       NaN  0.373411
```
而当数据集当中既有连续型变量，又有离散型变量的时候，用`agg()`方法则就会弄巧成拙了
```python
df = pd.DataFrame(
    {
        "A": [1, 2, 3],
        "B": [1.0, 2.0, 3.0],
        "C": ["test1", "test2", "test3"],
        "D": pd.date_range("20211101", periods=3),
    }
)

df.agg(["min", "sum"])
```
output
```python
     A    B                C          D
min  1  1.0            test1 2021-11-01
sum  6  6.0  test1test2test3        NaT
```
出来的结果可能并非是用户所想要的了，而至于`transform()`方法，其效果和用法都和`agg()`方法及其的相似，这边也就不多做赘述
<a name="TlMi2"></a>
## 索引和列名的重命名
针对索引和列名的重命名，可以通过`pandas`当中的`rename()`方法来实现，例如有这样一个数据集
```python
df1 = pd.DataFrame(np.random.randn(5, 3), columns=["A", "B", "C"],
                   index = ["a", "b", "c", "d", "e"])
```
output
```python
          A         B         C
a  0.343690  0.869984 -1.929814
b  1.025613  0.470155 -0.242463
c -0.400908 -0.362684  0.226857
d -1.339706 -0.302005 -1.784452
e -0.957026 -0.813600  0.215098
```
可以这样来操作
```python
df1.rename(columns={"A": "one", "B": "two", "C": "three"},
                 index={"a": "apple", "b": "banana", "c": "cat"})
```
output
```python
             one       two     three
apple   0.383813  0.588964 -0.162386
banana -0.462068 -2.938896  0.935492
cat    -0.059807 -1.987281  0.095432
d      -0.085230  2.013733 -1.324039
e      -0.678352  0.306776  0.808697
```
当然可以拆开来，单独对行或者是列进行重命名，对列的重命名可以这么来做
```python
df1.rename({"A": "one", "B": "two", "C": "three"}, axis = "columns")
```
output
```python
        one       two     three
a -0.997108 -1.383011  0.474298
b  1.009910  0.286303  1.120783
c  1.130700 -0.566922  1.841451
d -0.350438 -0.171079 -0.079804
e  0.988050 -0.524604  0.653306
```
对行的重命名则可以这么来做
```python
df1.rename({"a": "apple", "b": "banana", "c": "cat"}, axis = "index")
```
output
```python
               A         B         C
apple   0.590589 -0.311803 -0.782117
banana  1.528043 -0.944476 -0.337584
cat     1.326057 -0.087368  0.041444
d       1.079768 -0.098314 -0.210999
e       1.654869  1.170333  0.506194
```
<a name="TW6ZM"></a>
## 排序
在pandas当中，可以针对数据集当中的值来进行排序
```python
df1 = pd.DataFrame(
    {"one": [2, 1, 1, 1], "two": [1, 3, 2, 4], "three": [5, 4, 3, 2]}
)
```
output
```python
   one  two  three
0    2    1      5
1    1    3      4
2    1    2      3
3    1    4      2
```
按照“three”这一列当中的数值来进行排序
```python
df1.sort_values(by = "three")
```
output
```python
   one  two  three
3    1    4      2
2    1    2      3
1    1    3      4
0    2    1      5
```
也可以依照多列进行排序
```python
df1.sort_values(by = ["one", "two"])
```
output
```python
   one  two  three
2    1    2      3
1    1    3      4
3    1    4      2
0    2    1      5
```
在“one”这一列相等的时候，比较“two”这一列数值的大小，在排序的过程当中，默认采用的都是升序，可以改成降序来进行编排
```python
df1.sort_values("two", ascending=False)
```
output
```python
   one  two  three
3    1    4      2
1    1    3      4
2    1    2      3
0    2    1      5
```
<a name="x5TzE"></a>
## 数据类型的转换
最后涉及到的是数据类型的转换，在这之前，先得知道如何来查看数据的类型，pandas当中有相应的方法可以处理
```python
df2 = pd.DataFrame(
    {
        "A": pd.Series(np.random.randn(5), dtype="float16"),
        "B": pd.Series(np.random.randn(5)),
        "C": pd.Series(np.array(np.random.randn(5), dtype="uint8")),
    }
)
```
output
```python
          A         B    C
0 -0.498779 -0.501512    0
1 -0.055817 -0.528227  254
2 -0.914551  0.763298    1
3 -0.916016  1.366833    0
4  1.993164  1.834457    0
```
通过`dtypes`属性来查看数据的类型
```python
A    float16
B    float64
C      uint8
dtype: object
```
而通过`astype()`方法来实现数据类型的转换
```python
df2["B"].astype("int64")
```
output
```python
0    0
1    0
2    0
3    2
4    1
Name: B, dtype: int64
```
<a name="HQSu5"></a>
## 根据数据类型来筛选
与此同时，也可以根据相对应的数据类型来进行筛选，运用pandas当中的`select_dtypes`方法，先来创建一个数据集包含了各种数据类型的
```python
df = pd.DataFrame(
    {
        "string_1": list("abcde"),
        "int64_1": list(range(1, 6)),
        "uint8_1": np.arange(3, 8).astype("u1"),
        "float64_1": np.arange(4.0, 9.0),
        "bool1": [True, False, True, True, False],
        "bool2": [False, True, False, False, True],
        "dates_1": pd.date_range("now", periods=5),
        "category_1": pd.Series(list("ABCDE")).astype("category"),
    }
)
```
output
```python
  string_1  int64_1  uint8_1  ...  bool2                      dates_1  category_1
0      a      1      3  ...  False 2021-11-10 10:43:05.957685         A
1      b      2      4  ...   True 2021-11-11 10:43:05.957685         B
2      c      3      5  ...  False 2021-11-12 10:43:05.957685         C
3      d      4      6  ...  False 2021-11-13 10:43:05.957685         D
4      e      5      7  ...   True 2021-11-14 10:43:05.957685         E
```
先来查看一下各个列的数据类型
```python
df.dtypes
```
output
```python
string_1              object
int64_1                int64
uint8_1                uint8
float64_1            float64
bool1                   bool
bool2                   bool
dates_1       datetime64[ns]
category_1          category
dtype: object
```
筛选类型为**布尔值**的数据
```python
df.select_dtypes(include=[bool])
```
output
```python
   bool1  bool2
0   True  False
1  False   True
2   True  False
3   True  False
4  False   True
```
筛选出数据类型为**整型**的数据
```python
df.select_dtypes(include=['int64'])
```
output
```python
   int64_1
0      1
1      2
2      3
3      4
4      5
```
