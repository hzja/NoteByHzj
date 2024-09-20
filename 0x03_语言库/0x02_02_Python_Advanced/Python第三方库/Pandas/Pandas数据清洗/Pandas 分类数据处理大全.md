PythonPandas<br />`category`是pandas的一种**分类的定类数据类型**。和文本数据`.str.<methond>`一样，它也有访问器功能`.cat.<method>`。<br />这里将介绍：

- 什么是分类数据？
- 分类数据cat的处理方法
- 为什么要使用分类数据？
- 分类数据cat使用时的一些坑
<a name="BbWEn"></a>
## 什么是分类数据？
分类数据表达数值具有某种属性、类型和特征，也是理解的定类数据。比如，人口按性别分为男和女，按年龄分为老、中、少。<br />在计算机语言里，通常会用数字来表示，比如用1代表男，0代表女，但是0和1之间并没有大小关系，pandas中用category来表示分类数据。
<a name="Cn1YB"></a>
### 创建分类数据
创建数据时可以用dtpye来指定类型，比如：
```python
s = pd.Series(['a','b','c'],dtype='category')
s
------
0    a
1    b
2    c
dtype: category
Categories (3, object): ['a', 'b', 'c']
```
<a name="jOqLC"></a>
### 自动创建分类数据
在某些操作情况下会自动转变为分类类型，比如用`cut`进行分箱操作返回的分箱就是分类类型。
```python
pd.Series(pd.cut(range(1,10,2),3))
-----------------
0    (0.992, 3.667]
1    (0.992, 3.667]
2    (3.667, 6.333]
3      (6.333, 9.0]
4      (6.333, 9.0]
dtype: category
Categories (3, interval[float64]): [(0.992, 3.667] < (3.667, 6.333] < (6.333, 9.0]]
```
<a name="Si2D4"></a>
### 分类数据类型转换
直接用`astype`方法转换即可，如：
```python
s = pd.Series(['a','b','c'])
s
------
0    a
1    b
2    c
dtype: object

s.astype('category')
------
0    a
1    b
2    c
dtype: category
Categories (3, object): ['a', 'b', 'c']
```
<a name="tl8yX"></a>
### 自定义分类数据
除此之外，还可以通过`CategoricalDtype`自定义分类数据，自定义的类型适用于以上全部方法。<br />比如下面自定义了abc3个分类，并指定了顺序。然后就可以通过`dtype`指定自定义的数据类型了，d不在定义类型abc中，显示为空。
```python
from pandas.api.types import CategoricalDtype
# 自定义分类数据，有序
c= CategoricalDtype(categories=['a','b','c'],ordered=True)
pd.Series(list('abcabd'),dtype=c)
--------
0      a
1      b
2      c
3      a
4      b
5    NaN
dtype: category
Categories (3, object): ['a' < 'b' < 'c']
```
<a name="fWSLK"></a>
## 分类数据的处理方法
<a name="d7Jzy"></a>
### 修改分类
通过`.cat.rename_categories()`修改分类的名称。
```python
s = pd.Series(['a','b','c'],dtype='category')
# 指定分类为x、y、z
s.cat.categories = ['x','y','z']
0    x
1    y
2    z
dtype: category
Categories (3, object): ['x', 'y', 'z']

# 列表形式：修改分类类型为mno
s.cat.rename_categories(['m','n','o'])
# 字典形式：
s.cat.rename_categories({'x':'m','y':'n','z':'o'})
0    m
1    n
2    o
dtype: category
Categories (3, object): ['m', 'n', 'o']
```
<a name="e8O15"></a>
### 追加新分类
通过`.cat.add_categories()`追加分类。
```python
s.cat.add_categories(['r','t'])
0    x
1    y
2    z
dtype: category
Categories (5, object): ['x', 'y', 'z', 'r', 't']
```
<a name="LZcRO"></a>
### 删除分类
同理，也可以删除分类。有两种方法`remove_categories`和`remove_unused_categories`。
```python
# 删除指定的分类r和t
s.cat.remove_categories(['r','t'])
# 自动删除未使用的分类
s.cat.remove_unused_categories()
```
<a name="Mcgvg"></a>
### 顺序
默认情况下分类数据不自动排序，可以通过前面`CategoricalDtype`设置顺序，或者通过`.cat.as_ordered`设置。
```python
# 有序设置
s.cat.as_ordered()
0    x
1    y
2    z
dtype: category
Categories (3, object): ['x' < 'y' < 'z']
# 无序设置
s.cat.as_unordered()
# 重新排序
s.cat.reorder_categories(['y','x','z'], ordered=True)
```
<a name="QPXSs"></a>
## 为什么使用category数据类型？
总结一下，使用`category`有以下一些好处：

- **内存使用情况**：对于重复值很多的字符串列，`category`可以大大减少将数据存储在内存中所需的内存量;
- **运行性能**：进行了一些优化，可以提高某些操作的执行速度
- **算法库的适用**：在某些情况下，一些算法模型需要`category`这种类型。比如，知道lightgbm相对于xgboost优化的一个点就是可以处理分类变量，而在构建模型时需要指定哪些列是分类变量，并将它们调整为category作为超参数传给模型。

一个简单的例子。
```python
df_size = 100_000
df1 = pd.DataFrame(
    {
        "float_1": np.random.rand(df_size),
        "species": np.random.choice(["cat", "dog", "ape", "gorilla"], size=df_size),
    }
)
df1_cat = df1.astype({"species": "category"})
```
创建了两个DataFrame，其中df1包含了species并且为object类型，df1_cat复制了df1，但指定了species为category类型。
```python
>> df1.memory_usage(deep=True)
Index          128
float_1     800000
species    6100448
dtype: int64
```
就内存使用而言，可以直接看到包含字符串的列的成本是多高。`**species**`**列的字符串大约占用了6MB，如果这些字符串较长，则将会更多。**
```python
>> df1_cat.memory_usage(deep=True)
Index         128
float_1    800000
species    100416
dtype: int64
```
再看转换为category类别后的内存使用情况。**有了相当大的改进，使用的内存减少了大约60倍**。没有对比，就没有伤害。<br />这就是使用category的其中一个好处。
<a name="SOL2h"></a>
## 使用category的一些坑！
但爱之深，责之切呀，category有很多坑要注意，这里东哥总结出以下几点，供大家参考。
<a name="W2Kww"></a>
### 1、category列的操作
好吧，这部分应该才是大家较为关心的，因为经常会遇到一些莫名其妙的报错或者感觉哪里不对，又不知道问题出在哪里。<br />首先，说明一下：**使用category的时候需要格外小心，因为如果姿势不对，它就很可能变回object** 。而变回object的结果就是，会降低代码的性能（因为强制转换类型成本很高），并会消耗内存。<br />日常面对category类型的数据，肯定是要对其进行操作的，比如做一些转换。下面看一个例子，要分别对category和object类型进行同样的字符串大写操作，使用accessor的.str方法。<br />在非category字符串上：
```python
>> %timeit df1["species"].str.upper()
25.6 ms ± 2.07 ms per loop (mean ± std. dev. of 7 runs, 10 loops each)
```
在category字符串上：
```python
>> %timeit df1_cat["species"].str.upper()
1.85 ms ± 41.1 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
```
结果很明显了。在这种情况下，速度提高了大约14倍（因为内部优化会让.str.upper()仅对分类的唯一类别值调用一次，然后根据结果构造一个seires，而不是对结果中的每个值都去调用一次）。<br />怎么理解？假设现有一个列叫animal，其类别有cat和dog两种，假设样本为10000个，4000个cat和6000个dog。那么如果用对`category`本身处理，意味着只分别对cat和dog两种类别处理一次，一共两次就解决。如果对每个值处理，那就需要样本数量10000次的处理。<br />尽管从时间上有了一些优化，然而这种方法的使用也是有一些问题的。看一下内存使用情况。
```python
>> df1_cat["species"].str.upper().memory_usage(deep=True)
6100576
```
意外的发现category类型丢了。。结果竟是一个object类型，数据压缩的效果也没了，现在的结果再次回到刚才的6MB内存占用。<br />这是因为使用str会直接让原本的category类型强制转换为object，所以内存占用又回去了，这是为什么最开始说要格外小心。<br />**解决方法就是：直接对**`**category**`**本身操作而不是对它的值操作。** 要直接使用cat的方法来完成转换操作，如下。
```python
%timeit df1_cat["species"].cat.rename_categories(str.upper)
239 µs ± 13.9 µs per loop (mean ± std. dev. of 7 runs, 1000 loops each)
```
可以看到，这个速度就更快了，**因为省去了将**`**category**`**类别转换为object的时间，并且内存占用也非常少**。因此，这才是最优的做法。
<a name="pE3iS"></a>
### 2、与`category`列的合并
还是上面那个例子，但是这次增加了habitat一列，并且species中增加了sanke。
```python
df2 = pd.DataFrame(
    {
        "species": ["cat", "dog", "ape", "gorilla", "snake"],
        "habitat": ["house", "house", "jungle", "jungle", "jungle"],
    }
)
df2_cat = df2.astype({"species": "category", "habitat": "category"})
```
和前面一样，创建该数据集的一个category版本，并创建了一个带有object字符串的版本。如果将两个object列合并在一起的，没什么意思，因为大家都知道会发生什么，object+ object= object而已。<br />**把object列合并到**`**category**`**列上**<br />接着上面的例子。
```python
>> df1.merge(df2_cat, on="species").dtypes
float_1     float64
species      object
habitat    category
dtype: object
```
左边的df1中species列为object,右边的df2_cat中species列为category。可以看到，当合并时，在结果中的合并列会得到**category+ object= object**。<br />这显然不行了，又回到原来那样了。再试下其他情况。<br />**两个category列的合并**
```python
>> df1_cat.merge(df2_cat, on="species").dtypes
float_1     float64
species      object
habitat    category
dtype: object
```
结果是：**category+ category= object？**<br />有点想打人了，但是别急，看看为啥。<br />**在合并中，为了保存分类类型，两个category类型必须是完全相同的。** 这个与pandas中的其他数据类型略有不同，例如所有float64列都具有相同的数据类型，就没有什么区分。<br />而当讨论category数据类型时，该数据类型实际上是由该特定类别中存在的一组值来描述的，因此一个类别包含["cat", "dog", "mouse"]与类别包含`["cheese", "milk", "eggs"]`是不一样的。上面的例子之所以没成功，是因为多加了一个snake。<br />因此，可以得出结论：

- category1+ category2=object
- category1+ category1=category1

因此，**解决办法就是：两个category类别一模一样，让其中一个等于另外一个**。
```python
>> df1_cat.astype({"species": df2_cat["species"].dtype}).merge(
       df2_cat, on="species"
   ).dtypes

float_1     float64
species    category
habitat    category
dtype: object
```
<a name="TgVdA"></a>
### 3、category列的分组
用category类列分组时，一旦误操作就会发生意外，结果是Dataframe会被填成空值，还有可能直接跑死。。<br />**当对category列分组时，默认情况下，即使category类别的各个类不存在值，也会对每个类进行分组。**<br />一个例子来说明。
```python
habitat_df = (
    df1_cat.astype({"species": df2_cat["species"].dtype})
           .merge(df2_cat, on="species")
)
house_animals_df = habitat_df.loc[habitat_df["habitat"] == "house"]
```
这里采用habitat_df，从上面例子得到的，筛选habitat为house的，只有dog和cat是house，看下面分组结果。
```python
>> house_animals_df.groupby("species")["float_1"].mean()
species
ape             NaN
cat        0.501507
dog        0.501023
gorilla         NaN
snake           NaN
Name: float_1, dtype: float64
```
在`groupby`中得到了一堆空值。**默认情况下，当按category列分组时，即使数据不存在，pandas也会为该类别中的每个值返回结果**。略坑，如果数据类型包含很多不存在的，尤其是在多个不同的category列上进行分组，将会极其损害性能。<br />因此，**解决办法**是：可以传递`observed=True`到`groupby`调用中，这确保了仅获取数据中有值的组。
```python
>> house_animals_df.groupby("species", observed=True)["float_1"].mean()
species
cat    0.501507
dog    0.501023
Name: float_1, dtype: float64
```
<a name="ArYOL"></a>
### 4、category列的索引
仍以上面例子举例，使用groupby-unstack实现了一个交叉表，species作为列，habitat作为行，均为category类型。
```python
>> species_df = habitat_df.groupby(["habitat", "species"], observed=True)["float_1"].mean().unstack()
>> species_df

species       cat       ape       dog   gorilla
habitat                                        
house    0.501507       NaN  0.501023       NaN
jungle        NaN  0.501284       NaN  0.501108
```
这好像看似也没什么毛病，继续往下看。为这个交叉表添加一个新列new_col，值为1。
```python
>> species_df["new_col"] = 1
TypeError: 'fill_value=new_col' is not present in this Categorical's categories
```
正常情况下，上面这段代码是完全可以的，但这里报错了，为什么？<br />**原因是**：species和habitat现在均为category类型。使用.unstack()会把species索引移到列索引中（类似pivot交叉表的操作）。而当添加的新列不在species的分类索引中时，就会报错。<br />总结一下，pandas的category类型非常有用，可以带来一些良好的性能优势。但是它也很娇气，使用过程中要尤为小心，确保category类型在整个流程中保持不变，避免变回object。本文介绍的4个点注意点：

- **category列的变换操作**：直接对category本身操作而不是对它的值操作。这样可以保留分类性质并提高性能。
- **category列的合并**：合并时注意，要保留category类型，且每个dataframe的合并列中的分类类型必须完全匹配。
- **category列的分组**：默认情况下，获得数据类型中每个值的结果，即使数据中不存在该结果。可以通过设置`observed=True`调整。
- **category列的索引**：当索引为category类型的时候，注意是否可能与类别变量发生奇怪的交互作用。
