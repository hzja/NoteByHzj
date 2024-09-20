<a name="i8nZE"></a>
### 1、导入numpy库并取别名为np (★☆☆)
(提示: import … as …)
```python
import numpy as np
```
<a name="H2iLn"></a>
### 2、打印输出numpy的版本和配置信息 (★☆☆)
(提示: np.version, np.show_config)
```python
print(np.__version__)
print(np.show_config())
```
<a name="oETEH"></a>
### 3、创建一个长度为10的空向量 (★☆☆)
(提示: np.zeros)
```python
Z = np.zeros(10)
print(Z)
```
<a name="Tej3S"></a>
### 4、如何找到任何一个数组的内存大小？(★☆☆)
(提示: size, itemsize)
```python
Z = np.zeros((10,10))
print("%d bytes" % (Z.size * Z.itemsize))
```
<a name="XIUAn"></a>
### 5、如何从命令行得到numpy中add函数的说明文档? (★☆☆)
(提示: np.info)
```python
import numpy
numpy.info(numpy.add)
```
<a name="c8lCB"></a>
### 6、创建一个长度为10并且除了第五个值为1的空向量 (★☆☆)
(提示: array[4])
```python
Z = np.zeros(10)
Z[4] = 1
print(Z)
```
<a name="vf7th"></a>
### 7、创建一个值域范围从10到49的向量(★☆☆)
(提示: np.arange)
```python
Z = np.arange(10,50)
print(Z)
```
<a name="HNeID"></a>
### 8、反转一个向量(第一个元素变为最后一个) (★☆☆)
(提示: array[::-1])
```python
Z = np.arange(50)
Z = Z[::-1]
print(Z)
```
<a name="xaC3X"></a>
### 9、创建一个 3x3 并且值从0到8的矩阵(★☆☆)
(提示: reshape)
```python
Z = np.arange(9).reshape(3,3)
print(Z)
```
<a name="lzWcF"></a>
### 10、找到数组[1,2,0,0,4,0]中非0元素的位置索引 (★☆☆)
(提示: np.nonzero)
```python
nz = np.nonzero([1,2,0,0,4,0])
print(nz)
```
<a name="Pvm0k"></a>
### 11、创建一个 3x3 的单位矩阵 (★☆☆)
(提示: np.eye)
```python
Z = np.eye(3)
print(Z)
```
<a name="aEZbS"></a>
### 12、创建一个 3x3x3的随机数组 (★☆☆)
(提示: np.random.random)
```python
Z = np.random.random((3,3,3))
print(Z)
```
<a name="du3Jf"></a>
### 13、创建一个 10x10 的随机数组并找到它的最大值和最小值 (★☆☆)
(提示: min, max)
```python
Z = np.random.random((10,10))
Zmin, Zmax = Z.min(), Z.max()
print(Zmin, Zmax)
```
<a name="IE1to"></a>
### 14、创建一个长度为30的随机向量并找到它的平均值 (★☆☆)
(提示: mean)
```python
Z = np.random.random(30)
m = Z.mean()
print(m)
```
<a name="P7l77"></a>
### 15、创建一个二维数组，其中边界值为1，其余值为0 (★☆☆)
(提示: array[1:-1, 1:-1])
```python
Z = np.ones((10,10))
Z[1:-1,1:-1] = 0
print(Z)
```
<a name="bhJOb"></a>
### 16、对于一个存在在数组，如何添加一个用0填充的边界? (★☆☆)
(提示: np.pad)
```python
Z = np.ones((5,5))
Z = np.pad(Z, pad_width=1, mode='constant', constant_values=0)
print(Z)
```
<a name="UsURI"></a>
### 17、下面表达式运行的结果是什么？(★☆☆)
(提示: NaN = not a number, inf = infinity) (提示：NaN : 不是一个数，inf : 无穷)
```python
# 表达式                           # 结果
0 * np.nan                        nan
np.nan == np.nan                  False
np.inf > np.nan                   False
np.nan - np.nan                   nan
0.3 == 3 * 0.1                    False
```
<a name="lFTtY"></a>
### 18、创建一个 5x5的矩阵，并设置值1,2,3,4落在其对角线下方位置 (★☆☆)
(提示: np.diag)
```python
Z = np.diag(1+np.arange(4),k=-1)
print(Z)
```
<a name="fBEvy"></a>
### 19、创建一个8x8 的矩阵，并且设置成棋盘样式 (★☆☆)
(提示: array[::2])
```python
Z = np.zeros((8,8),dtype=int)
Z[1::2,::2] = 1
Z[::2,1::2] = 1
print(Z)
```
<a name="EgduI"></a>
### 20、考虑一个 (6,7,8) 形状的数组，其第100个元素的索引(x,y,z)是什么?
(提示: np.unravel_index)
```python
print(np.unravel_index(100,(6,7,8)))
```
<a name="AKAA5"></a>
### 21、用tile函数去创建一个 8x8的棋盘样式矩阵(★☆☆)
(提示: np.tile)
```python
Z = np.tile( np.array([[0,1],[1,0]]), (4,4))
print(Z)
```
<a name="VfHhF"></a>
### 22、对一个5x5的随机矩阵做归一化(★☆☆)
(提示: (x - min) / (max - min))
```python
Z = np.random.random((5,5))
Zmax, Zmin = Z.max(), Z.min()
Z = (Z - Zmin)/(Zmax - Zmin)
print(Z)
```
<a name="EGQJQ"></a>
### 23、创建一个将颜色描述为(RGBA)四个无符号字节的自定义dtype？(★☆☆)
(提示: np.dtype)
```python
color = np.dtype([("r", np.ubyte, 1),
                  ("g", np.ubyte, 1),
                  ("b", np.ubyte, 1),
                  ("a", np.ubyte, 1)])
color
```
<a name="sVl3V"></a>
### 24、一个5x3的矩阵与一个3x2的矩阵相乘，实矩阵乘积是什么？(★☆☆)
(提示: np.dot | @)
```python
Z = np.dot(np.ones((5,3)), np.ones((3,2)))
print(Z)
```
<a name="LfjOZ"></a>
### 25、给定一个一维数组，对其在3到8之间的所有元素取反 (★☆☆)
(提示: >, <=)
```python
Z = np.arange(11)
Z[(3 < Z) & (Z <= 8)] *= -1
print(Z)
```
<a name="xJghj"></a>
### 26、下面脚本运行后的结果是什么? (★☆☆)
(提示: np.sum)
```python
# Author: Jake VanderPlas               # 结果

print(sum(range(5),-1))                 9
from numpy import *                     
print(sum(range(5),-1))                 10    #numpy.sum(a, axis=None)
```
<a name="cJPQc"></a>
### 27、考虑一个整数向量Z,下列表达合法的是哪个? (★☆☆)
（提示：这里还有“位运算符”）
```python
Z**Z                        True
2 << Z >> 2                 False
Z <- Z                      True
1j*Z                        True  #复数           
Z/1/1                       True
Z<Z>Z                       False
```
<a name="BZXq9"></a>
### 28、下面表达式的结果分别是什么？(★☆☆)
```python
np.array(0) / np.array(0)                           nan
np.array(0) // np.array(0)                          0
np.array([np.nan]).astype(int).astype(float)        -2.14748365e+09
```
<a name="F1a97"></a>
### 29、如何从零位开始舍入浮点数组？(★☆☆)
(提示: np.uniform, np.copysign, np.ceil, np.abs)
```python
# Author: Charles R Harris

Z = np.random.uniform(-10,+10,10)
print (np.copysign(np.ceil(np.abs(Z)), Z))
```
<a name="lAUfT"></a>
### 30、如何找出两个数组公共的元素? (★☆☆)
(提示: np.intersect1d)
```python
Z1 = np.random.randint(0, 10, 10)
Z2 = np.random.randint(0, 10, 10)
print (np.intersect1d(Z1, Z2))
```
<a name="ANhud"></a>
### 31、如何忽略所有的 numpy 警告(尽管不建议这么做)? (★☆☆)
(提示: np.seterr, np.errstate)
```python
# Suicide mode on
defaults = np.seterr(all="ignore")
Z = np.ones(1) / 0

# Back to sanity
_ = np.seterr(**defaults)

# 另一个等价的方式， 使用上下文管理器（context manager）
with np.errstate(divide='ignore'):
    Z = np.ones(1) / 0
```
<a name="fsgrh"></a>
### 32、下面的表达式是否为真? (★☆☆)
(提示: 虚数)
```python
np.sqrt(-1) == np.emath.sqrt(-1)     Faslse
```
<a name="Cs5jL"></a>
### 33、如何获得昨天，今天和明天的日期? (★☆☆)
(提示: np.datetime64, np.timedelta64)
```python
yesterday = np.datetime64('today', 'D') - np.timedelta64(1, 'D')
today = np.datetime64('today', 'D')
tomorrow = np.datetime64('today', 'D') + np.timedelta64(1, 'D')
```
<a name="OEmDL"></a>
### 34、怎么获得所有与2016年7月的所有日期? (★★☆)
(提示: np.arange(dtype=datetime64['D']))
```python
Z = np.arange('2016-07', '2016-08', dtype='datetime64[D]')
print (Z)
```
<a name="QJHKi"></a>
### 35、如何计算 ((A+B)*(-A/2)) (不使用中间变量)? (★★☆)
(提示: np.add(out=), np.negative(out=), np.multiply(out=), np.divide(out=))
```python
A = np.ones(3) * 1
B = np.ones(3) * 1
C = np.ones(3) * 1
np.add(A, B, out=B)
np.divide(A, 2, out=A)
np.negative(A, out=A)
np.multiply(A, B, out=A)
```
<a name="l4DUe"></a>
### 36、用5种不同的方法提取随机数组中的整数部分 (★★☆)
(提示: %, np.floor, np.ceil, astype, np.trunc)
```python
Z = np.random.uniform(0, 10, 10)
print (Z - Z % 1)
print (np.floor(Z))
print (np.cell(Z)-1)
print (Z.astype(int))
print (np.trunc(Z))
```
<a name="P61Oc"></a>
### 37、创建一个5x5的矩阵且每一行的值范围为从0到4 (★★☆)
(提示: np.arange)
```python
Z = np.zeros((5, 5))
Z += np.arange(5)
print (Z)
```
<a name="cFD2v"></a>
### 38、如何用一个生成10个整数的函数来构建数组 (★☆☆)
(提示: np.fromiter)
```python
def generate():
    for x in range(10):
        yield x
Z = np.fromiter(generate(), dtype=float, count=-1)
print (Z)
```
<a name="hlhvQ"></a>
### 39、创建一个大小为10的向量， 值域为0到1，不包括0和1 (★★☆)
(提示: np.linspace)
```python
Z = np.linspace(0, 1, 12, endpoint=True)[1: -1]
print (Z)
```
<a name="X25Q3"></a>
### 40、创建一个大小为10的随机向量，并把它排序 (★★☆)
(提示: sort)
```python
Z = np.random.random(10)
Z.sort()
print (Z)
```
<a name="l2lNO"></a>
### 41、对一个小数组进行求和有没有办法比np.sum更快? (★★☆)
(提示: np.add.reduce)
```python
# Author: Evgeni Burovski

Z = np.arange(10)
np.add.reduce(Z)

# np.add.reduce 是numpy.add模块中的一个ufunc(universal function)函数,C语言实现
```
<a name="N07SH"></a>
### 42、如何判断两和随机数组相等 (★★☆)
(提示: np.allclose, np.array_equal)
```python
A = np.random.randint(0, 2, 5)
B = np.random.randint(0, 2, 5)

# 假设array的形状(shape)相同和一个误差容限（tolerance）
equal = np.allclose(A,B)
print(equal)

# 检查形状和元素值，没有误差容限（值必须完全相等）
equal = np.array_equal(A,B)
print(equal)
```
<a name="ut1uy"></a>
### 43、把数组变为只读 (★★☆)
(提示: flags.writeable)
```python
Z = np.zeros(5)
Z.flags.writeable = False
Z[0] = 1
```
<a name="U6HXF"></a>
### 44、将一个10x2的笛卡尔坐标矩阵转换为极坐标 (★★☆)
(提示: np.sqrt, np.arctan2)
```python
Z = np.random.random((10, 2))
X, Y = Z[:, 0], Z[:, 1]
R = np.sqrt(X**2 + Y**2)
T = np.arctan2(Y, X)
print (R)
print (T)
```
<a name="kRi1V"></a>
### 45、创建一个大小为10的随机向量并且将该向量中最大的值替换为0(★★☆)
(提示: argmax)
```python
Z = np.random.random(10)
Z[Z.argmax()] = 0
print (Z)
```
<a name="ioExF"></a>
### 46、创建一个结构化数组，其中x和y坐标覆盖[0, 1]x[1, 0]区域 (★★☆)
(提示: np.meshgrid)
```python
Z = np.zeros((5, 5), [('x', float), ('y', float)])
Z['x'], Z['y'] = np.meshgrid(np.linspace(0, 1, 5), np.linspace(0, 1, 5))
print (Z)
```
<a name="bJ7N8"></a>
### 47、给定两个数组X和Y，构造柯西(Cauchy)矩阵C () (★★☆)
(提示: np.subtract.outer)
```python
# Author: Evgeni Burovski

X = np.arange(8)
Y = X + 0.5
C = 1.0 / np.subtract.outer(X, Y)
print (C)
print(np.linalg.det(C)) # 计算行列式
```
<a name="zddYw"></a>
### 48、打印每个numpy 类型的最小和最大可表示值 (★★☆)
(提示: np.iinfo, np.finfo, eps)
```python
for dtype in [np.int8, np.int32, np.int64]:
    print(np.iinfo(dtype).min)
    print(np.iinfo(dtype).max)
for dtype in [np.float32, np.float64]:
    print(np.finfo(dtype).min)
    print(np.finfo(dtype).max)
    print(np.finfo(dtype).eps)
```
<a name="vuhxp"></a>
### 49、如何打印数组中所有的值？(★★☆)
(提示: np.set_printoptions)
```python
np.set_printoptions(threshold=np.nan)
Z = np.zeros((16,16))
print(Z)
```
<a name="Y0c2X"></a>
### 50、如何在数组中找到与给定标量接近的值? (★★☆)
(提示: argmin)
```python
Z = np.arange(100)
v = np.random.uniform(0, 100)
index = (np.abs(Z-v)).argmin()
print(Z[index])
```
<a name="A0F8p"></a>
### 51、创建表示位置(x, y)和颜色(r, g, b, a)的结构化数组 (★★☆)
(提示: dtype)
```python
Z = np.zeros(10, [('position', [('x', float, 1), 
                                ('y', float, 1)]),
                  ('color',    [('r', float, 1), 
                                ('g', float, 1), 
                                ('b', float, 1)])])
print (Z)
```
<a name="OJ8Ys"></a>
### 52、思考形状为(100, 2)的随机向量，求出点与点之间的距离 (★★☆)
(提示: np.atleast_2d, T, np.sqrt)
```python
Z = np.random.random((100, 2))
X, Y = np.atleast_2d(Z[:, 0], Z[:, 1])
D = np.sqrt((X-X.T)**2 + (Y-Y.T)**2)
print (D)

# 使用scipy库可以更快
import scipy.spatial

Z = np.random.random((100,2))
D = scipy.spatial.distance.cdist(Z,Z)
print(D)
```
<a name="X7VU4"></a>
### 53、如何将类型为float(32位)的数组类型转换位integer(32位)? (★★☆)
(提示: astype(copy=False))
```python
Z = np.arange(10, dtype=np.int32)
Z = Z.astype(np.float32, copy=False)
print(Z)
```
<a name="JZUoN"></a>
### 54、如何读取下面的文件? (★★☆)
(提示: np.genfromtxt)
```python
1, 2, 3, 4, 5
6,  ,  , 7, 8
,  , 9,10,11

# 先把上面保存到文件example.txt中
# 这里不使用StringIO， 因为Python2 和Python3 在这个地方有兼容性问题
Z = np.genfromtxt("example.txt", delimiter=",")  
print(Z)
```
<a name="D23Qp"></a>
### 55、numpy数组枚举(enumerate)的等价操作? (★★☆)
(提示: np.ndenumerate, np.ndindex)
```python
Z = np.arange(9).reshape(3,3)
for index, value in np.ndenumerate(Z):
    print(index, value)
for index in np.ndindex(Z.shape):
    print(index, Z[index])
```
<a name="n2SSY"></a>
### 56、构造一个二维高斯矩阵(★★☆)
(提示: np.meshgrid, np.exp)
```python
X, Y = np.meshgrid(np.linspace(-1, 1, 10), np.linspace(-1, 1, 10))
D = np.sqrt(X**2 + Y**2)
sigma, mu = 1.0, 0.0
G = np.exp(-( (D-mu)**2 / (2.0*sigma**2) ))
print (G)
```
<a name="enMQe"></a>
### 57、如何在二维数组的随机位置放置p个元素? (★★☆)
(提示: np.put, np.random.choice)
```python
# Author: Divakar

n = 10
p = 3
Z = np.zeros((n,n))
np.put(Z, np.random.choice(range(n*n), p, replace=False),1)
print(Z)
```
<a name="Dudbu"></a>
### 58、减去矩阵每一行的平均值 (★★☆)
(提示: mean(axis=,keepdims=))
```python
# Author: Warren Weckesser

X = np.random.rand(5, 10)

# 新
Y = X - X.mean(axis=1, keepdims=True)

# 旧
Y = X - X.mean(axis=1).reshape(-1, 1)

print(Y)
```
<a name="E4SqW"></a>
### 59、如何对数组通过第n列进行排序? (★★☆)
(提示: argsort)
```python
# Author: Steve Tjoa

Z = np.random.randint(0,10,(3,3))
print(Z)
print(Z[ Z[:,1].argsort() ])
```
<a name="qtrer"></a>
### 60、如何判断一个给定的二维数组存在空列? (★★☆)
(提示: any, ~)
```python
# Author: Warren Weckesser

Z = np.random.randint(0,3,(3,10))
print((~Z.any(axis=0)).any())
```
<a name="OCt6d"></a>
### 61、从数组中找出与给定值最接近的值 (★★☆)
(提示: np.abs, argmin, flat)
```python
Z = np.random.uniform(0,1,10)
z = 0.5
m = Z.flat[np.abs(Z - z).argmin()]
print(m)
```
<a name="KZMuV"></a>
### 62、思考形状为(1, 3)和(3, 1)的两个数组形状，如何使用迭代器计算它们的和? (★★☆)
(提示: np.nditer)
```python
A = np.arange(3).reshape(3, 1)
B = np.arange(3).reshape(1, 3)
it = np.nditer([A, B, None])
for x, y, z in it:
    z[...] = x + y
print (it.operands[2])
```
<a name="JGmfM"></a>
### 63、创建一个具有name属性的数组类 (★★☆)
(提示: class method)
```python
class NameArray(np.ndarray):
    def __new__(cls, array, name="no name"):
        obj = np.asarray(array).view(cls)
        obj.name = name
        return obj
    def __array_finalize__(self, obj):
        if obj is None: return
        self.info = getattr(obj, 'name', "no name")

Z = NameArray(np.arange(10), "range_10")
print (Z.name)
```
<a name="LcOuG"></a>
### 64、给定一个向量，如何让在第二个向量索引的每个元素加1(注意重复索引)? (★★★)
(提示: np.bincount | np.add.at)
```python
# Author: Brett Olsen

Z = np.ones(10)
I = np.random.randint(0,len(Z),20)
Z += np.bincount(I, minlength=len(Z))
print(Z)

# Another solution
# Author: Bartosz Telenczuk
np.add.at(Z, I, 1)
print(Z)
```
<a name="YTkru"></a>
### 65、如何根据索引列表I将向量X的元素累加到数组F? (★★★)
(提示: np.bincount)
```python
# Author: Alan G Isaac

X = [1,2,3,4,5,6]
I = [1,3,9,3,4,1]
F = np.bincount(I,X)
print(F)
```
<a name="QB8Vz"></a>
### 66、思考(dtype = ubyte)的(w, h, 3)图像，计算唯一颜色的值(★★★)
(提示: np.unique)
```python
# Author: Nadav Horesh

w,h = 16,16
I = np.random.randint(0,2,(h,w,3)).astype(np.ubyte)
F = I[...,0]*256*256 + I[...,1]*256 +I[...,2]
n = len(np.unique(F))
print(np.unique(I))
```
<a name="tLifQ"></a>
### 67、思考如何求一个四维数组最后两个轴的数据和(★★★)
(提示: sum(axis=(-2,-1)))
```python
A = np.random.randint(0,10,(3,4,3,4))
# 传递一个元组(numpy 1.7.0)
sum = A.sum(axis=(-2,-1))
print(sum)

# 将最后两个维度压缩为一个
# (适用于不接受轴元组参数的函数)
sum = A.reshape(A.shape[:-2] + (-1,)).sum(axis=-1)
print(sum)
```
<a name="lCGcK"></a>
### 68、考虑一维向量D，如何使用相同大小的向量S来计算D的子集的均值，其描述子集索引？(★★★)
(提示: np.bincount)
```python
# Author: Jaime Fernández del Río

D = np.random.uniform(0,1,100)
S = np.random.randint(0,10,100)
D_sums = np.bincount(S, weights=D)
D_counts = np.bincount(S)
D_means = D_sums / D_counts
print(D_means)

# Pandas solution as a reference due to more intuitive code
import pandas as pd
print(pd.Series(D).groupby(S).mean())
```
<a name="HThTz"></a>
### 69、如何获得点积的对角线？(★★★)
(提示: np.diag)
```python
# Author: Mathieu Blondel

A = np.random.uniform(0,1,(5,5))
B = np.random.uniform(0,1,(5,5))

# Slow version  
np.diag(np.dot(A, B))

# Fast version
np.sum(A * B.T, axis=1)

# Faster version
np.einsum("ij,ji->i", A, B)
```
<a name="W96Tl"></a>
### 70.考虑向量[1,2,3,4,5]，如何建立一个新的向量，在每个值之间交错有3个连续的零？(★★★)
(提示: array[::4])
```python
# Author: Warren Weckesser

Z = np.array([1,2,3,4,5])
nz = 3
Z0 = np.zeros(len(Z) + (len(Z)-1)*(nz))
Z0[::nz+1] = Z
print(Z0)
```
<a name="jTDKd"></a>
### 71、考虑一个维度(5,5,3)的数组，如何将其与一个(5,5)的数组相乘？(★★★)
(提示: array[:, :, None])
```python
A = np.ones((5,5,3))
B = 2*np.ones((5,5))
print(A * B[:,:,None])
```
<a name="QEcQj"></a>
### 72、如何对一个数组中任意两行做交换? (★★★)
(提示: array[[]] = array[[]])
```python
# Author: Eelco Hoogendoorn

A = np.arange(25).reshape(5,5)
A[[0,1]] = A[[1,0]]
print(A)
```
<a name="D5rZp"></a>
### 73、思考描述10个三角形（共享顶点）的一组10个三元组，找到组成所有三角形的唯一线段集 (★★★)
(提示: repeat, np.roll, np.sort, view, np.unique)
```python
# Author: Nicolas P. Rougier

faces = np.random.randint(0,100,(10,3))
F = np.roll(faces.repeat(2,axis=1),-1,axis=1)
F = F.reshape(len(F)*3,2)
F = np.sort(F,axis=1)
G = F.view( dtype=[('p0',F.dtype),('p1',F.dtype)] )
G = np.unique(G)
print(G)
```
<a name="DUz68"></a>
### 74、给定一个二进制的数组C，如何生成一个数组A满足np.bincount(A)==C? (★★★)
(提示: np.repeat)
```python
# Author: Jaime Fernández del Río

C = np.bincount([1,1,2,3,4,4,6])
A = np.repeat(np.arange(len(C)), C)
print(A)
```
<a name="iw9IB"></a>
### 75、如何通过滑动窗口计算一个数组的平均数? (★★★)
(提示: np.cumsum)
```python
# Author: Jaime Fernández del Río

def moving_average(a, n=3) :
    ret = np.cumsum(a, dtype=float)
    ret[n:] = ret[n:] - ret[:-n]
    return ret[n - 1:] / n
Z = np.arange(20)
print(moving_average(Z, n=3))
```
<a name="rVsQJ"></a>
### 76、思考以为数组Z，构建一个二维数组，其第一行是(Z[0],Z[1],Z[2])， 然后每一行移动一位，最后一行为 (Z[-3],Z[-2],Z[-1]) (★★★)
(提示: from numpy.lib import stride_tricks)
```python
# Author: Joe Kington / Erik Rigtorp
from numpy.lib import stride_tricks

def rolling(a, window):
    shape = (a.size - window + 1, window)
    strides = (a.itemsize, a.itemsize)
    return stride_tricks.as_strided(a, shape=shape, strides=strides)
Z = rolling(np.arange(10), 3)
print(Z)
```
<a name="prcd6"></a>
### 77、如何对布尔值取反，或改变浮点数的符号(sign)? (★★★)
(提示: np.logical_not, np.negative)
```python
# Author: Nathaniel J. Smith

Z = np.random.randint(0,2,100)
np.logical_not(Z, out=Z)

Z = np.random.uniform(-1.0,1.0,100)
np.negative(Z, out=Z)
```
<a name="znwtK"></a>
### 78、思考两组点集P0和P1去描述一组线(二维)和一个点p,如何计算点p到每一条线 i (P0[i],P1[i])的距离？(★★★)
```python
def distance(P0, P1, p):
    T = P1 - P0
    L = (T**2).sum(axis=1)
    U = -((P0[:,0]-p[...,0])*T[:,0] + (P0[:,1]-p[...,1])*T[:,1]) / L
    U = U.reshape(len(U),1)
    D = P0 + U*T - p
    return np.sqrt((D**2).sum(axis=1))

P0 = np.random.uniform(-10,10,(10,2))
P1 = np.random.uniform(-10,10,(10,2))
p  = np.random.uniform(-10,10,( 1,2))
print(distance(P0, P1, p))
```
<a name="c7Oyl"></a>
### 79、考虑两组点集P0和P1去描述一组线(二维)和一组点集P，如何计算每一个点 j(P[j]) 到每一条线 i (P0[i],P1[i])的距离? (★★★)
```python
# Author: Italmassov Kuanysh

# based on distance function from previous question
P0 = np.random.uniform(-10, 10, (10,2))
P1 = np.random.uniform(-10,10,(10,2))
p = np.random.uniform(-10, 10, (10,2))
print(np.array([distance(P0,P1,p_i) for p_i in p]))
```
<a name="dtNN4"></a>
### 80、思考一个任意的数组，编写一个函数，该函数提取一个具有固定形状的子部分，并以一个给定的元素为中心(在该部分填充值) (★★★)
(提示: minimum, maximum)
```python
# Author: Nicolas Rougier

Z = np.random.randint(0,10,(10,10))
shape = (5,5)
fill  = 0
position = (1,1)

R = np.ones(shape, dtype=Z.dtype)*fill
P  = np.array(list(position)).astype(int)
Rs = np.array(list(R.shape)).astype(int)
Zs = np.array(list(Z.shape)).astype(int)R_start = np.zeros((len(shape),)).astype(int)
R_stop  = np.array(list(shape)).astype(int)
Z_start = (P-Rs//2)
Z_stop  = (P+Rs//2)+Rs%2

R_start = (R_start - np.minimum(Z_start,0)).tolist()
Z_start = (np.maximum(Z_start,0)).tolist()
R_stop = np.maximum(R_start, (R_stop - np.maximum(Z_stop-Zs,0))).tolist()
Z_stop = (np.minimum(Z_stop,Zs)).tolist()

r = [slice(start,stop) for start,stop in zip(R_start,R_stop)]
z = [slice(start,stop) for start,stop in zip(Z_start,Z_stop)]
R[r] = Z[z]
print(Z)
print(R)
```
<a name="OzrPC"></a>
### 81、考虑一个数组Z = [1,2,3,4,5,6,7,8,9,10,11,12,13,14],如何生成一个数组R = [[1,2,3,4], [2,3,4,5], [3,4,5,6], ...,[11,12,13,14]]? (★★★)
(提示: stride_tricks.as_strided)
```python
# Author: Stefan van der Walt

Z = np.arange(1,15,dtype=np.uint32)
R = stride_tricks.as_strided(Z,(11,4),(4,4))
print(R)
```
<a name="lSuJ6"></a>
### 82、计算矩阵的秩 (★★★)
(提示: np.linalg.svd)
```python
# Author: Stefan van der Walt

Z = np.random.uniform(0,1,(10,10))
U, S, V = np.linalg.svd(Z) # Singular Value Decomposition
rank = np.sum(S > 1e-10)
print(rank)
```
<a name="fZvSL"></a>
### 83、如何找出数组中出现频率最高的值?(★★★)
(提示: np.bincount, argmax)
```python
Z = np.random.randint(0,10,50)
print(np.bincount(Z).argmax())
```
<a name="Bbzbd"></a>
### 84、从一个10x10的矩阵中提取出连续的3x3区块(★★★)
(提示: stride_tricks.as_strided)
```python
# Author: Chris Barker

Z = np.random.randint(0,5,(10,10))
n = 3
i = 1 + (Z.shape[0]-3)
j = 1 + (Z.shape[1]-3)
C = stride_tricks.as_strided(Z, shape=(i, j, n, n), strides=Z.strides + Z.strides)
print(C)
```
<a name="FSs0Y"></a>
### 85.创建一个满足 Z[i,j] == Z[j,i]的二维数组子类 (★★★)
(提示: class method)
```python
# Author: Eric O. Lebigot
# Note: only works for 2d array and value setting using indices

class Symetric(np.ndarray):
    def __setitem__(self, index, value):
        i,j = index
        super(Symetric, self).__setitem__((i,j), value)
        super(Symetric, self).__setitem__((j,i), value)

def symetric(Z):
    return np.asarray(Z + Z.T - np.diag(Z.diagonal())).view(Symetric)

S = symetric(np.random.randint(0,10,(5,5)))
S[2,3] = 42
print(S)
```
<a name="Spide"></a>
### 86、考虑p个 nxn 矩阵和一组形状为(n,1)的向量，如何直接计算p个矩阵的乘积(n,1)? (★★★)
(提示: np.tensordot)
```python
# Author: Stefan van der Walt

p, n = 10, 20
M = np.ones((p,n,n))
V = np.ones((p,n,1))
S = np.tensordot(M, V, axes=[[0, 2], [0, 1]])
print(S)

# It works, because:
# M is (p,n,n)
# V is (p,n,1)
# Thus, summing over the paired axes 0 and 0 (of M and V independently),
# and 2 and 1, to remain with a (n,1) vector.
```
<a name="zj7U0"></a>
### 87、对于一个16x16的数组，如何得到一个区域的和(区域大小为4x4)? (★★★)
(提示: np.add.reduceat)
```python
# Author: Robert Kern

Z = np.ones((16,16))
k = 4
S = np.add.reduceat(np.add.reduceat(Z, np.arange(0, Z.shape[0], k), axis=0), np.arange(0, Z.shape[1], k), axis=1)
print(S)
```
<a name="ggyXM"></a>
### 88、如何利用numpy数组实现Game of Life? (★★★)
(提示: Game of Life , Game of Life有哪些图形？)
```python
# Author: Nicolas Rougier

def iterate(Z):
    # Count neighbours
    N = (Z[0:-2,0:-2] + Z[0:-2,1:-1] + Z[0:-2,2:] +
         Z[1:-1,0:-2]                + Z[1:-1,2:] +
         Z[2:  ,0:-2] + Z[2:  ,1:-1] + Z[2:  ,2:])

    # Apply rules
    birth = (N==3) & (Z[1:-1,1:-1]==0)
    survive = ((N==2) | (N==3)) & (Z[1:-1,1:-1]==1)
    Z[...] = 0
    Z[1:-1,1:-1][birth | survive] = 1
    return Z

Z = np.random.randint(0,2,(50,50))
for i in range(100): Z = iterate(Z)
print(Z)
```
<a name="aaSep"></a>
### 89、如何找到一个数组的第n个最大值? (★★★)
(提示: np.argsort | np.argpartition)
```python
Z = np.arange(10000)
np.random.shuffle(Z)
n = 5

# Slow
print (Z[np.argsort(Z)[-n:]])

# Fast
print (Z[np.argpartition(-Z,n)[:n]])
```
<a name="uPdFy"></a>
### 90、给定任意个数向量，创建笛卡尔积(每一个元素的每一种组合) (★★★)
(提示: np.indices)
```python
# Author: Stefan Van der Walt

def cartesian(arrays):
    arrays = [np.asarray(a) for a in arrays]
    shape = (len(x) for x in arrays)

    ix = np.indices(shape, dtype=int)
    ix = ix.reshape(len(arrays), -1).T

    for n, arr in enumerate(arrays):
        ix[:, n] = arrays[n][ix[:, n]]

    return ix

print (cartesian(([1, 2, 3], [4, 5], [6, 7])))
```
<a name="K65Vx"></a>
### 91、如何从一个常规数组中创建记录数组(record array)? (★★★)
(提示: np.core.records.fromarrays)
```python
Z = np.array([("Hello", 2.5, 3),
              ("World", 3.6, 2)])
R = np.core.records.fromarrays(Z.T, 
                               names='col1, col2, col3',
                               formats = 'S8, f8, i8')
print(R)
```
<a name="HeN2V"></a>
### 92、思考一个大向量Z, 用三种不同的方法计算它的立方 (★★★)
(提示: np.power, *, np.einsum)
```python
# Author: Ryan G.

x = np.random.rand(5e7)

%timeit np.power(x,3)
%timeit x*x*x
%timeit np.einsum('i,i,i->i',x,x,x)
```
<a name="B9Fxk"></a>
### 93、考虑两个形状分别为(8,3) 和(2,2)的数组A和B. 如何在数组A中找到满足包含B中元素的行？(不考虑B中每行元素顺序)？(★★★)
(提示: np.where)
```python
# Author: Gabe Schwartz

A = np.random.randint(0,5,(8,3))
B = np.random.randint(0,5,(2,2))

C = (A[..., np.newaxis, np.newaxis] == B)
rows = np.where(C.any((3,1)).all(1))[0]
print(rows)
```
<a name="isaoE"></a>
### 94、思考一个10x3的矩阵，如何分解出有不全相同值的行 (如 [2,2,3]) (★★★)
```python
# Author: Robert Kern

Z = np.random.randint(0,5,(10,3))
print(Z)
# solution for arrays of all dtypes (including string arrays and record arrays)
E = np.all(Z[:,1:] == Z[:,:-1], axis=1)
U = Z[~E]
print(U)
# soluiton for numerical arrays only, will work for any number of columns in Z
U = Z[Z.max(axis=1) != Z.min(axis=1),:]
print(U)
```
<a name="Z0u8I"></a>
### 95、将一个整数向量转换为二进制矩阵 (★★★)
(提示: np.unpackbits)
```python
# Author: Warren Weckesser

I = np.array([0, 1, 2, 3, 15, 16, 32, 64, 128])
B = ((I.reshape(-1,1) & (2**np.arange(8))) != 0).astype(int)
print(B[:,::-1])

# Author: Daniel T. McDonald

I = np.array([0, 1, 2, 3, 15, 16, 32, 64, 128], dtype=np.uint8)
print(np.unpackbits(I[:, np.newaxis], axis=1))
```
<a name="qvN3K"></a>
### 96、给定一个二维数组，如何提取出唯一的行?(★★★)
(提示: np.ascontiguousarray)
```python
# Author: Jaime Fernández del Río

Z = np.random.randint(0,2,(6,3))
T = np.ascontiguousarray(Z).view(np.dtype((np.void, Z.dtype.itemsize * Z.shape[1])))
_, idx = np.unique(T, return_index=True)
uZ = Z[idx]
print(uZ)
```
<a name="FtU8A"></a>
### 97、考虑两个向量A和B，写出用einsum等式对应的inner, outer, sum, mul函数 (★★★)
(提示: np.einsum)
```python
# Author: Alex Riley
# Make sure to read: http://ajcr.net/Basic-guide-to-einsum/

A = np.random.uniform(0,1,10)
B = np.random.uniform(0,1,10)

np.einsum('i->', A)       # np.sum(A)
np.einsum('i,i->i', A, B) # A * B
np.einsum('i,i', A, B)    # np.inner(A, B)
np.einsum('i,j->ij', A, B)    # np.outer(A, B)
```
<a name="XBKgV"></a>
### 98、考虑一个由两个向量描述的路径(X,Y)，如何用等距样例(equidistant samples)对其进行采样(sample)(★★★)?
(提示: np.cumsum, np.interp)
```python
# Author: Bas Swinckels

phi = np.arange(0, 10*np.pi, 0.1)
a = 1
x = a*phi*np.cos(phi)
y = a*phi*np.sin(phi)

dr = (np.diff(x)**2 + np.diff(y)**2)**.5 # segment lengths
r = np.zeros_like(x)
r[1:] = np.cumsum(dr)                # integrate path
r_int = np.linspace(0, r.max(), 200) # regular spaced path
x_int = np.interp(r_int, r, x)       # integrate path
y_int = np.interp(r_int, r, y)
```
<a name="qrFET"></a>
### 99、给定一个整数n 和一个二维数组X，从X中选择可以被解释为从多n度的多项分布式的行，即这些行只包含整数对n的和. (★★★)
(提示: np.logical_and.reduce, np.mod)
```python
# Author: Evgeni Burovski

X = np.asarray([[1.0, 0.0, 3.0, 8.0],
                [2.0, 0.0, 1.0, 1.0],
                [1.5, 2.5, 1.0, 0.0]])
n = 4
M = np.logical_and.reduce(np.mod(X, 1) == 0, axis=-1)
M &= (X.sum(axis=-1) == n)
print(X[M])
```
<a name="SfZZo"></a>
### 100、对于一个一维数组X，计算它boostrapped之后的95%置信区间的平均值. (★★★)
(提示: np.percentile)
```python
# Author: Jessica B. Hamrick

X = np.random.randn(100) # random 1D array
N = 1000 # number of bootstrap samples
idx = np.random.randint(0, X.size, (N, X.size))
means = X[idx].mean(axis=1)
confint = np.percentile(means, [2.5, 97.5])
print(confint)
```
来源： https://github.com/rougier/numpy-100
