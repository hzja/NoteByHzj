Python Numpy
<a name="xEyqq"></a>
## Numpy的常用方法
<a name="XM8hO"></a>
### 生成函数
<a name="VH14S"></a>
### 生成ndarray对象的函数，注意所有`np`即为`numpy`的简写
`np.arange(n)`: 生成指定范围的一个数据序列，返回的是`ndarray`对象。其实`np.arange()`的使用很类似内置函数`range()`，只不过`range()`函数生成的数据类似一个列表，而`np.arange()`生成的是`ndarray`对象。<br />`np.array(list)`：将一个列表转成`ndarray`对象。
```python
import numpy as np #导入numpy模块，起别名为np 
import warnings 
warnings.filterwarnings("ignore") # 忽略警告信息

data = np.arange(10)
display(data)

data1= list(range(10)) 
display(type(data1)) 
data1=np.array(data1)
display(data1)

# 当然使用np.array()还可以创建二维的数组
array = [[1,2],[3,4],[5,6],[7,8],[9,10]] 
array = np.array(array) 
display(array) 
# 如果是一维数组：向量 
# 如果是二维数组，那么就看做是一个矩阵
```
结果：
```python
array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
list
array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
array([[ 1,  2],
       [ 3,  4],
       [ 5,  6],
       [ 7,  8],
       [ 9, 10]])
```
<a name="qAT6X"></a>
### 使用`np.ones()`、`np.zeros()`、`np.full()` 生成`ndarray`对象
`np.ones(N)` : 生成一个N长度全1的`ndarray`对象<br />`np.zeros(N)` : 生成一个N长度全0的`ndarray`对象<br />`np.full(N)` : 生成一个N长度全为指定值的`ndarray`对象
```python
import numpy as np
array1 = np.ones((3,4)) # 创建值全为1的数组。display(array1)
array2 = np.zeros((3,4)) # 创建值全为0的数组。display(array2)
array3 = np.full((3,4),6) # 创建值全为指定值的数组。display(array3)
```
结果：
```python
array([[1., 1., 1., 1.],
       [1., 1., 1., 1.],
       [1., 1., 1., 1.]])
array([[0., 0., 0., 0.],
       [0., 0., 0., 0.],
       [0., 0., 0., 0.]])
array([[6, 6, 6, 6],
       [6, 6, 6, 6],
       [6, 6, 6, 6]])
```
大家可以发现`ones`和`zeros`得到的数组是浮点型的。那如何设置它的数据类型呢？<br />A. 在声明的时候可以使用，比如`array1 = np.ones((3,4),dtype=np.int)`<br />B. 使用`ndarray`对象`.astype`(数据类型)进行类型转换<br />Numpy的数据类型有哪些呢？

| 类型 | 类型代码 | 说明 |
| --- | --- | --- |
| int8、uint8 | i1、u1 | 有符号和无符号8位整型（1字节) |
| int16、uint16 | i2、u2 | 有符号和无符号16位整型（2字节) |
| int32、uint32 | i4、u4 | 有符号和无符号32位整型（4字节) |
| int64、uint64 | i8、u8 | 有符号和无符号64位整型（8字节) |
| float16 | f2 | 半精度浮点数 |
| float32 | f4、f | 单精度浮点数 |
| float64 | f8、d | 双精度浮点数 |
| float128 | f16、g | 扩展精度浮点数 |
| complex64 | c8 | 分别用两个32位表示的复数 |
| complex128 | c16 | 分别用两个64位表示的复数 |
| complex256 | c32 | 分别用两个128位表示的复数 |
| bool | ? | 布尔型 |
| object | o | 布尔型 |
| string | Sn | 固定长度字符串，每个字符1字节，如S10 |
| unicode | Un | 固定长度Unicode，字节数由系统决定，如U10 |

<a name="Pl5R7"></a>
### 可以使用`numpy.random`方法生成随机数组成的数组
`np.random.rand` 生成指定形状的0­～1之间的随机数<br />`np.random.random` 生成指定形状的0­～1之间的随机数<br />`np.random.randn` 标准正态分布<br />`np.random.normal` 指定均值和方法的正态分布<br />`np.random.randint` 生成指定数值范围内的随机整数<br />`np.random.seed` 按照种子来生成随机数，种子一样，则生成的结果必一致<br />`np.random.shuffle` 打乱数组元素顺序<br />`np.random.uniform` 均匀分布<br />`np.random.choice` 按照指定概率从指定集合中，生成随机数<br />`**np.random.rand**`** 与**`**np.random.random**`**两者类似**，都是产生0­～1之间的随机数，但是函数的参数不一样。
```python
import numpy as np 
d1 = np.random.random((2,3)) #注意区别多（少）了一对() 
d2 = np.random.rand(2,3) 
display(d1,d2)
```
`**np.random.randn()**` 服从正态分布的从**0­～1**之间的随机数
```python
dn = np.random.randn(2,3) 
display(dn)
```
`**np.random.normal()**` 指定均值和方差的正态分布
```python
# loc：float  此概率分布的均值（对应着整个分布的中心centre）
# scale：float 此概率分布的标准差（对应于分布的宽度，scale越大越矮胖，scale越小，越瘦高）
# size：int or tuple of ints  输出的shape，默认为None，只输出一个值
dn= np.random.normal(loc=4, scale=0.01, size=4) 
dn1= np.random.normal(loc=4, scale=0.01, size=(2,4)) 
display(dn,dn1)
```
`**np.random.randint()**` 随机整数
```python
# 在1-10范围里，随机生成5个int32类型的数值，生成的结果集可能出现重复数值 
arr1 = np.random.randint(1, high=10, size=5, dtype=np.int32) 
display(arr1) # 生成一个指定范围[-10,0]的随机二维数组 

arr2 = np.random.randint(-10, high=10, size=(4, 6), dtype=np.int32) 
display(arr2)
```
`**np.random.choice()**` 从指定数据集中，随机抽**选一个数据**
```python
# 从数值集合中，按照指定概率生成随机数，参数P的总和一定得是1 
arr = np.random.choice([0,3,5,7,8,9], p=[0.1, 0.2, 0.3, 0.4,0.2,0.5]) 
display(arr)
```
<a name="b3weK"></a>
## 数学函数与运算操作
<a name="WCow3"></a>
### 数学相关
Numpy提供了许多数学操作相关函数，常用的函数有：<br />abs / fabs ：绝对值<br />ceil / floor ：向上/向下取整<br />log / log2 / log10 ：对数<br />exp ：e为底的指数<br />modf ：将浮点数num分解成整数部分和小数部分。<br />sin / sinh / cos / cosh ：正玄、余玄...<br />sqrt ：开平方
```python
import numpy as np
arr = np.array([[-1,1.2,0.8,-3.9],[-6,1,-1.8,0.9]]) 
display(arr) 

#绝对值 
a = np.abs(arr) 
display(a) 

#向上/向下取整 
b = np.ceil(arr) 
b1 = np.floor(arr) 
display(b,b1) 

# 对数函数
c = np.log(arr) 
display(c) 

# e为底数的指数函数 
d = np.exp(arr) 
display(d) 

# 浮点数拆分成 整数 和 浮点数 两部分 
# 返回一个元组，含有两个元素（数组类型），第一个元素返回小数部分，第二个元素返回整数部分。 
e = np.modf(arr) 
display(e) 

#开平方 
f = np.sqrt(arr) 
display(f)
```
**运算相关的有：**<br />数组与数（一维数组与一维数组）的运算（加+、减­、乘*、除/、取整//、取模%、平方 * * 、立方 * * * ）：<br />加：“`+`” 或者`np.add(a, b)`<br />减：“­`-`” 或者`np.subtract(a, b)`<br />乘：“`*`” 或者`np.multiply(a, b)`<br />除：“`/`” 或者`np.divide(a, b)`<br />次方：“ `**` ” ，如2^7=2**7<br />取整函数：“ // ”或者 `np.ceil()`, `np.floor()`, `np.trunc()/fix()`, `np.rint()`, `np.around()`<br />取模：“%”或者`np.mod()`和`np.remainder()`<br />矩阵乘积：`np.dot(a, b)`<br />判断：> 、 >= 、< 、 <= 、 == 、 !=<br />条件：`all()` 、`any()`<br />逻辑条件：& 、 | 、 !<br />在Numpy中的运算基本都是遵守广播机制的。<br />现在看一下关系运算符：
```python
import numpy as np 
arr1 = np.array([1,2,3,4,5]) 
arr2 = np.array([0,2,-5,-4,8]) 
display(arr1>arr2,arr1<arr2,arr1!=arr2,arr1==arr2)
```
结果如下：
```python
array([ True, False, True, True, False])
array([False, False, False, False, True])
array([ True, False, True, True, True])
array([False, True, False, False, False])
```
其中`all()`和`any()`返回的也是`bool`类型的结果，`any()`表示只要有一个`True` 就返回 `True`，`all()`表示所有为`True` 就返回 `True`
```python
display(np.all(arr1),np.any(arr2)) 
```
逻辑运算符的使用：
```python
np.all(arr1<0) & np.all(arr2>0)
```
<a name="DOCBS"></a>
### 数组的链接（拼接、组合）和拆分
`np.concatenate` 对多个数组按指定轴的方向进行连接。<br />`np.stack` / `np.vstack` / `np.hstack` / `np.dstack` / `np.tile` 组合<br />`np.split` / `np.hsplit` / `np.vsplit` / `np.dsplit` 拆分<br />**排序与去重**<br />`np.unique(arr,return_index,return_inverse,return_counts,axis)` : 返回`ndarray`中的元素，排除重复元素之后，并进行排序<br />`np.sort( ndarray)` : 作为函数使用时，不更改被排序的原始`arrays`，返回副本
```python
arr = np.array([[1,2,3,4,5,5,6,6,6,8],[2,2,3,4,5,5,6,6,7,9]])
arr = np.unique(arr) 
display(arr)

arr1 = np.array([1,5,6,6,6,2,3,4,5]) 
arr1 = np.sort(arr1) 
display(arr1)
```
结果：
```python
array([1, 2, 3, 4, 5, 6, 7, 8, 9])
array([1, 2, 3, 4, 5, 5, 6, 6, 6])
```
<a name="B2YN4"></a>
### 改变形状与数组扁平化
可以通过数组对象的`reshape`方法（或者`np`的`reshape`函数）来改变数组的形状。
```python
arr1 = np.arange(30).reshape(3, 10) 
arr2 = np.arange(30).reshape(5, -1)

# 也可以使用
a = np.arange(10) 
b = np.reshape(a, (2,5)) 
display(b)
```
可以通过调用`ravel`或`flatten`方法，对数组对象进行扁平化处理。<br />np.ravel / ravel<br />flatten<br />shape, reshape, resize, ravel<br />二者的区别在于，ravel返回原数组的视图，而flatten返回原数组的拷贝。
```python
x = np.arange(16).reshape(4, 4) 
display(x.ravel()) 
display(np.ravel(x)) 
display(x.flatten())
```
结果：
```python
array([ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15])
array([ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15])
array([ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15])
```
**统计函数**<br />Numpy（或数组对象）具有如下常用的统计函数。<br />mean / sum / prod(乘积)<br />max / min / amax / amin<br />argmax / argmin<br />std / var<br />cumsum / cumprod<br />all / any<br />median / percentile
```python
import numpy as np 
a = np.random.randint(1,20,size=(4,5)) # 随机生成4行5列的数组
print(np.mean(a)) # 平均值
print(np.max(a))  # 最大值
print(np.min(a)) # 最小值
print(np.sum(a)) # 求和
print(np.prod(a)) # 求积
print(np.std(a)) # 求标准差
print(np.var(a)) # 求方差
print(np.median(a)) # 求中位数
print(np.argmax(a)) # 最大值索引
print(np.argmin(a)) # 最小值索引
```
当然也可以指定`axis`进行计算，比如按照轴进行计算，比如`axis=0`即列轴计算
```python
print(np.mean(a,axis=0)) # 平均值
print(np.max(a,axis=0))  # 最大值
print(np.min(a,axis=0)) # 最小值
print(np.sum(a,axis=0)) # 求和
```
<a name="TjTzU"></a>
## Ndarray对象属性与索引使用
<a name="QSkTU"></a>
### Ndarray对象的属性
ndim ­ 维度数<br />shape ­ 维度/形状<br />dtype ­ 元数数据类型<br />size ­ 元素个数<br />itemsize ­ 返回数组元素占用空间的大小。以字节为单位<br />nbytes ­ 总字节数 = `size * itemsize`<br />T ­ 数组对象的转置视图<br />flat ­ 扁平迭代器
```python
arr = np.array([[1,2,3,4],[5,6,7,8],[9,10,11,12]]) 
print(arr.ndim)  #维度数 
print(arr.shape)  #维度形状 
print(arr.dtype)  #元数数据类型 
print(arr.size)  #元素个数 
print(arr.itemsize)  # 返回数组元素占用空间的大小。以字节为单位 
print(arr.nbytes)  #返回占用空间的总长度 
```
<a name="YGRa9"></a>
### Ndarray对象的切片和索引
| `ndarray[n]` | 选取第n+1个元素 |
| --- | --- |
| `ndarray[n:m]` | 选取第n+1到第m个元素 |
| `ndarray[:]` | 选取全部元素 |
| `ndarray[n:]` | 选取第n+1到最后一个元素 |
| `ndarray[:n]` | 选取第0到第n个元素 |
| `ndarray[ bool_ndarray ]`. 注：`bool_ndarray`表示`bool`类型的`ndarray` | 选取为true的元素 |
| `ndarray[[x,y,m,n]]`... | 选取顺序和序列为x、y、m、n的ndarray |
| `ndarray[n,m]` | 选取第n+1行第m+1个元素 |
| `ndarray[n,m,...]` | 选取n行n列....的元素 |

当然Numpy的方法有很多也是可以转成`ndarray`中的方法使用，比如统计函数
```python
import numpy as np 
a = np.random.randint(1,20,size=(4,5)) # 随机生成4行5列的数组
print(np.mean(a)) # 平均值
print(a.mean())  # 通过ndarray对象调用mean方法
print(np.max(a))  # 最大值
print(a.max())  # 通过ndarray对象调用max方法
```
