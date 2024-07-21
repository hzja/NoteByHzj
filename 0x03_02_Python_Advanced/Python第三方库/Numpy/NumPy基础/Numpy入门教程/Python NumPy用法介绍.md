Python Numpy
<a name="LinNt"></a>
## 介绍
NumPy是Python数值计算最重要的基础包，大多数提供科学计算的包都是用NumPy的数组作为构建基础。NumPy本身并没有提供多么高级的数据分析功能，理解NumPy数组以及面向数组的计算，将有助于更加高效地使用诸如Pandas之类的工具。<br />虽然NumPy提供了通用的数值数据处理的计算基础，但大多数读者可能还是想将Pandas作为统计和分析工作的基础，尤其是处理表格数据时。<br />NumPy的部分功能如下：

- ndarray，一个具有矢量算术运算和复杂广播能力的快速且节省空间的多维数组。
- 用于对整组数据进行快速运算的标准数学函数（无需编写循环）。
- 用于读写磁盘数据的工具以及用于操作内存映射文件的工具。
- 线性代数、随机数生成以及傅里叶变换功能。
- 用于集成由C、C++、Fortran等语言编写的代码的A C API。

NumPy之于数值计算特别重要是因为它可以高效处理大数组的数据。这是因为：

- 比起Python的内置序列，NumPy数组使用的内存更少。
- NumPy可以在整个数组上执行复杂的计算，而不需要Python的for循环。

使用下面格式约定，引入NumPy包：
```python
import numpy as np
```
<a name="aXjB0"></a>
## NumPy的ndarray：N维数组对象
NumPy最重要的是其N维数组对象（即ndarray），其中的所有元素必须是相同类型的。该对象是一个快速而灵活的大数据集容器，可以利用这种数组对整块数据执行数学运算，其语法跟标量元素之间的运算一样。
<a name="Bx1DY"></a>
### 创建ndarray

- 使用`np.array(list/tuple, dtype=np.float32)`函数，产生一个新的含有传入数据的ndarray对象。

第一个参数为元组、列表（相同数据类型），第二个参数为ndarray数组中的数据类型。当第二个参数为空时，NumPy将根据数据情况指定一个类型。<br />返回值为[ ]形式，元素间由空格分割。
```python
In [20]: arr1 = np.array([6, 7.5, 8, 0, 1])   #从列表创建
In [21]: pring(arr1)
Out[21]: [ 6. ,  7.5,  8. ,  0. ,  1. ]     #NumPy根据数据情况，指定了float类型

In [23]: arr2 = np.array([[1, 2, 3, 4], [5, 6, 7, 8],(1.2 , 2.3)])
In [24]: pring(arr2)
Out[24]: [[1, 2, 3, 4] [5, 6, 7, 8] (1.2 , 2.3)]
```

- 使用NumPy中的内置函数

`np.arange(begin,end,step,dtype=np.float32)`：begin为元素起始值（包含），end为元素结束值（不包含），step为步长（默认值为1），dtype为元素类型。如果只有一个参数n，则为从0到n-1；如有有两个参数n和m，则为从n到m-1；<br />`np.linspace(begin,end,number)`：创建包含number个元素的数组，并在指定的开始值（包含）和结束值（包含）之间平均间隔；<br />`np.ones(shape)`：根据shape生成一个全1数组，shape是元组类型，比如(2,3)；<br />`np.zeros(shape)`：根据shape生成一个全0数组，shape是元组类型，比如(2,3,4)；<br />`np.full(shape,val)`：根据shape生成一个数组，每个元素值都是val；<br />`np.eye(n)`：创建一个正方的n*n单位矩阵，对角线为1，其余为0；<br />`np.ones_like(a)`：根据数组a的形状生成一个全1数组；<br />`np.zeros_like(a)`：根据数组a的形状生成一个全0数组；<br />`np.full_like(a,val)`：根据数组a的形状生成一个每个元素值都是val的数组；<br />`np.concatenate()` 将两个或多个数组合并成一个新的数组。
```python
In [30]: arr3 = np.zeros((3, 6))
In [31]: print(arr3)
Out[30]: [[ 0.,  0.,  0.,  0.,  0.,  0.] [ 0.,  0.,  0.,  0.,  0.,  0.] [ 0.,  0.,  0.,  0.,  0.,  0.]]
```

- 从磁盘读取数据创建ndarray数组，将ndarray数组保存到磁盘（大部分情况会使用pandas或其它工具加载文本或表格数据）`np.load(fname)`<br />• fname : 文件名，以.npy为扩展名，压缩扩展名为`.npznp.save(fname, array)` 或 `np.savez(fname, array)`<br />• fname : 文件名，以.npy为扩展名，压缩扩展名为.npz<br />• array : 数组变量
<a name="gdIgt"></a>
### ndarray数组对象的属性

- `.ndim`：秩，即轴的数量或维度的数量
- `.shape`：ndarray对象的尺度，对于矩阵，n行m列
- `.size`：ndarray对象元素的个数，相当于`.shape`中n*m的值
- `.dtype`：ndarray对象的元素类型
- `.itemsize`：ndarray对象中每个元素的大小，以字节为单位
<a name="TDgxK"></a>
### ndarray数组对象的类型和维度变换

- `.astype(np.float64)`：将ndarray数组元素从一个类型转换成另一个类型，返回一个新数组。如果将浮点数转换成整数，则小数部分将会被截取删除。（类型变换）
- `.reshape(shape)`：不改变原数组元素，返回一个新的shape维度的数组（维度变换）
- `.resize(shape)`：与`.reshape()`功能一致，但修改原数组（维度变换）
- `.swapaxes(ax1,ax2)` 将数组n个维度中两个维度进行调换（维度变换）
- `.flatten()`：对数组进行降维，返回折叠后的一维数组，原数组不变（维度变换）
- `.tolist()`：将N维数组转换成列表（维度变换）
<a name="VTIZl"></a>
### ndarray数组的索引和切片
具体使用参考<br />[https://docs.scipy.org/doc/numpy/user/quickstart.html#indexing-slicing-and-iterating](https://docs.scipy.org/doc/numpy/user/quickstart.html#indexing-slicing-and-iterating)
<a name="KC7JQ"></a>
### ndarray数组的运算

- 数组与标量之间的运算，都会作用于数组的每一个元素；
- 大小相同的数组之间的任何算术运算，都会将运算应用到元素级；
- 大小相同的数组之间的比较运算，都会将运算应用到元素级并生成布尔值数组；
- `np.abs(arr)`\`np.fabs(arr)`：计算数组arr各元素的绝对值
- `np.sqrt(arr)`：计算数组arr各元素的平方根
- `np.square(arr)`：计算数组arr各元素的平方
- `np.log(arr)`\`np.log10(arr)`\`np.log2(arr)`：计算数组arr各元素的自然对数、10底对数和2底对数
- `np.ceil(arr)`\`np.floor(arr)`：计算数组arr各元素的ceiling值 或 floor值
- `np.rint(arr)` 计算数组arr各元素的四舍五入值
- `np.modf(arr)` 将数组arr各元素的小数和整数部分以两个独立数组形式返回
- `np.cos(arr)`\`np.cosh(arr)`\`np.sin(arr)`\`np.sinh(arr)`\`np.tan(arr)`\`np.tanh(arr)`计算数组arr各元素的普通型和双曲型三角函数
- `np.exp(arr)` 计算数组arr各元素的指数值
- `np.sign(arr)` 计算数组arr各元素的符号值，1(+), 0, ‐1(‐)
<a name="leb6I"></a>
### 利用ndarray进行数据处理
<a name="JuJQu"></a>
### 排序
ndarray数组通过`.sort()`函数排序，多维数组时传入轴编号
<a name="iZGlq"></a>
### NumPy的随机数函数

- `np.random.rand(d0,d1,..,dn)`：根据d0‐dn创建随机数数组，浮点数，[0,1)，均匀分布
- `np.random.randn(d0,d1,..,dn)`：根据d0‐dn创建随机数数组，标准正态分布
- `np.random.randint(low[,high,shape])`：根据shape创建随机整数或整数数组，范围是`[low, high)`
- `np.random.seed(s)`：随机数种子，s是给定的种
- `np.random.shuffle(a)`：根据数组a的第1轴进行随排列，改变数组x
- `np.random.permutation(a)`：根据数组a的第1轴产生一个新的乱序数组，不改变数组x
- `np.random.choice(a[,size,replace,p])`：从一维数组a中以概率p抽取元素，形成size形状新数组 replace表示是否可以重用元素，默认为False
- `np.random.uniform(low,high,size)`：产生具有均匀分布的数组，low起始值，high结束值，size形状
- `np.random.normal(loc,scale,size)`：产生具有正态分布的数组，loc均值，scale标准差，size形状
- `np.random.poisson(lam,size)`：产生具有泊松分布的数组，lam随机事件发生率，size形状
<a name="AXR2G"></a>
### NumPy的统计类函数

- `np.sum(a, axis=None)`：根据给定轴axis计算数组a相关元素之和，axis整数或元组
- `np.mean(a, axis=None)`：根据给定轴axis计算数组a相关元素的期望，axis整数或元组
- `np.average(a,axis=None,weights=None)`：根据给定轴axis计算数组a相关元素的加权平均值
- `np.std(a, axis=None)`：根据给定轴axis计算数组a相关元素的标准差
- `np.var(a, axis=None)`：根据给定轴axis计算数组a相关元素的方差
- `np.min(a)\max(a)`：计算数组a中元素的最小值、最大值
- `np.argmin(a)\argmax(a)`：计算数组a中元素最小值、最大值的降一维后下标
- `np.unravel_index(index, shape)`：根据shape将一维下标index转换成多维下标
- `np.ptp(a)`：计算数组a中元素最大值与最小值的差
- `np.median(a)`：计算数组a中元素的中位数（中值）
<a name="LtOne"></a>
### NumPy的梯度函数
梯度：连续值之间的变化率，即斜率<br />XY坐标轴连续三个X坐标对应的Y轴值：a, b, c，其中，b的梯度是：(c‐a)/2

- `np.gradient(f)`：计算数组f中元素的梯度，当f为多维时，返回每个维度梯度
