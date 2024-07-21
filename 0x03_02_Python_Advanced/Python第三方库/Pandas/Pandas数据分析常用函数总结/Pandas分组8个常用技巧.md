Pandas<br />pandas的groupby是数据处理中一个非常强大的功能。虽然很多同学已已经非常熟悉了，但有些小技巧还是要普及一下的。这里采用一个公开的数据集进行说明。
```python
import pandas as pd
iris = pd.read_csv('https://raw.githubusercontent.com/mwaskom/seaborn-data/master/iris.csv')
```
随机采样5条，数据是长这样子的。
```python
>>> iris.sample(5)
     sepal_length  sepal_width  petal_length  petal_width     species
95            5.7          3.0           4.2          1.2  versicolor
71            6.1          2.8           4.0          1.3  versicolor
133           6.3          2.8           5.1          1.5   virginica
4             5.0          3.6           1.4          0.2      setosa
33            5.5          4.2           1.4          0.2      setosa
```
因为是分组功能，所以被分的对象肯定是类别型的。在这个数据里，这里就以species进行分组举例。<br />首先，以species分组创建一个groupby的object。这里单独生成groupby对象是因为后面会反复用到，其实用的熟练了直接链接起来就可以了。
```python
iris_gb = iris.groupby('species')
```
<a name="qoZtX"></a>
## 一、创建频率表
假如想知道每个species类中的数量有多少，那么直接使用groupby的size函数即可，如下。
```python
>>> iris_gb.size()
species
setosa        50
versicolor    50
virginica     50
dtype: int64
```
<a name="NLg0S"></a>
## 二、计算常用的描述统计量
比如，想要按组计算均值，那么就用`mean()`函数。
```python
>>> # 计算均值
>>> iris_gb.mean()
            sepal_length  sepal_width  petal_length  petal_width
species                                                         
setosa             5.006        3.428         1.462        0.246
versicolor         5.936        2.770         4.260        1.326
virginica          6.588        2.974         5.552        2.026
```
默认情况下如果没有限制，那么`mean()`函数将对所有变量特征计算均值。如果希望只计算某一个变量的均值，可以指定该变量，如下所示。
```python
>>> # 单列
>>> iris_gb['sepal_length'].mean()
species
setosa        5.006
versicolor    5.936
virginica     6.588
Name: sepal_length, dtype: float64
>>> # 双列
>>> iris_gb[['sepal_length', 'petal_length']].mean()
            sepal_length  petal_length
species                               
setosa             5.006         1.462
versicolor         5.936         4.260
virginica          6.588         5.552
```
同理，其它描述性统计量`min()`、`max()`、medianhe和std都是一样的用法。
<a name="r7Vim"></a>
## 三、查找最大值（最小值）的索引
如果要查找每个组的最大值或最小值的索引时，有一个方便的功能可以直接使用。
```python
>>> iris_gb.idxmax()
            sepal_length  sepal_width  petal_length  petal_width
species                                                         
setosa                14           15            24           43
versicolor            50           85            83           70
virginica            131          117           118          100
```
如何应用呢？<br />比如想查找每组sepal_length最大值对应的整条记录时，就可以这样用。注意，这里是整条记录，相当于按sepal_length最大值这个条件进行了筛选。
```python
>>> sepal_largest = iris.loc[iris_gb['sepal_length'].idxmax()]
>>> sepal_largest
     sepal_length  sepal_width  petal_length  petal_width     species
14            5.8          4.0           1.2          0.2      setosa
50            7.0          3.2           4.7          1.4  versicolor
131           7.9          3.8           6.4          2.0   virginica
```
<a name="OlmUS"></a>
## 四、Groupby之后重置索引
很多时候，在groupby处理后还要进行其他操作。也就是说，想重置分组索引以使其成为正常的行和列。<br />第一种方法可能大家常用，就是通过`reset_index()`让乱序索引重置。
```python
>>> iris_gb.max().reset_index()
      species  sepal_length  sepal_width  petal_length  petal_width
0      setosa           5.8          4.4           1.9          0.6
1  versicolor           7.0          3.4           5.1          1.8
2   virginica           7.9          3.8           6.9          2.5
```
但其实，还有一个看上去更加友好的用法。可以在groupby的时候就设置`as_index`参数，也可以达到同样效果。
```python
>>> iris.groupby('species', as_index=False).max()
      species  sepal_length  sepal_width  petal_length  petal_width
0      setosa           5.8          4.4           1.9          0.6
1  versicolor           7.0          3.4           5.1          1.8
2   virginica           7.9          3.8           6.9          2.5
```
<a name="muU8z"></a>
## 五、多种统计量汇总
上面都是单个统计量的操作，那如果想同时操作好几个呢？<br />groupby还有一个超级棒的用法就是和聚合函数agg连起来使用。
```python
>>> iris_gb[['sepal_length', 'petal_length']].agg(["min", "mean"])

           sepal_length        petal_length       
                    min   mean          min   mean
species                                           
setosa              4.3  5.006          1.0  1.462
versicolor          4.9  5.936          3.0  4.260
virginica           4.9  6.588          4.5  5.552
```
在agg里面，只要列出统计量的名称即可，便可同时对每个列进行多维度统计。
<a name="VoTa1"></a>
## 六、特定列的聚合
上面是的多个操作对于每个列都是一样的。实际使用过程中，可能对于每个列的需求都是不一样的。<br />所以在这种情况下，可以通过为不同的列单独设置不同的统计量。
```python
>>> iris_gb.agg({"sepal_length": ["min", "max"], "petal_length": ["mean", "std"]})
           sepal_length      petal_length          
                    min  max         mean       std
species                                            
setosa              4.3  5.8        1.462  0.173664
versicolor          4.9  7.0        4.260  0.469911
virginica           4.9  7.9        5.552  0.551895
```
<a name="e4AYd"></a>
## 七、NamedAgg命名统计量
现在又有新的想法了。上面的多级索引看起来有点不太友好，想把每个列下面的统计量和列名分别合并起来。可以使用NamedAgg来完成列的命名。
```python
>>> iris_gb.agg(
...     sepal_min=pd.NamedAgg(column="sepal_length", aggfunc="min"),
...     sepal_max=pd.NamedAgg(column="sepal_length", aggfunc="max"),
...     petal_mean=pd.NamedAgg(column="petal_length", aggfunc="mean"),
...     petal_std=pd.NamedAgg(column="petal_length", aggfunc="std")
... )
            sepal_min  sepal_max  petal_mean  petal_std
species                                                
setosa            4.3        5.8       1.462   0.173664
versicolor        4.9        7.0       4.260   0.469911
virginica         4.9        7.9       5.552   0.551895
```
因为NamedAgg是一个元组，所以也可以直接赋值元组给新的命名，效果一样，但看上去更简洁。
```python
iris_gb.agg(
    sepal_min=("sepal_length", "min"),
    sepal_max=("sepal_length", "max"),
    petal_mean=("petal_length", "mean"),
    petal_std=("petal_length", "std")
)
```
<a name="UNC6P"></a>
## 八、使用自定义函数
上面agg聚合函数中都是通过添加一个统计量名称来完成操作的，除此之外也可直接给一个功能对象。
```python
>>> iris_gb.agg(pd.Series.mean)
            sepal_length  sepal_width  petal_length  petal_width
species                                                         
setosa             5.006        3.428         1.462        0.246
versicolor         5.936        2.770         4.260        1.326
virginica          6.588        2.974         5.552        2.026
```
不仅如此，名称和功能对象也可一起使用。
```python
iris_gb.agg(["min", pd.Series.mean])
```
更骚的是，还可以自定义函数，也都是可以的。
```python
>>> def double_length(x):
...     return 2*x.mean()
... 
>>> iris_gb.agg(double_length)
            sepal_length  sepal_width  petal_length  petal_width
species                                                         
setosa            10.012        6.856         2.924        0.492
versicolor        11.872        5.540         8.520        2.652
virginica         13.176        5.948        11.104        4.052
```
当然如果想更简洁，也可以使用lambda函数。总之，用法非常灵活，可以自由组合搭配。
```python
iris_gb.agg(lambda x: x.mean())
```
以上就是使用groupby过程中可能会用到的8个操作，如果熟练使用起来会发现这个功能是真的很强大。
