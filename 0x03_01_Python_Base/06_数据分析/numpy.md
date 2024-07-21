# numpy

numpy提供了python对多维数组对象的支持：ndarray，具有矢量运算能力，快速、节省空间。numpy支持高级大量的维度数组与矩阵运算，此外也针对数组运算提供大量的数学函数库。(函数功能不会的话可以通过[chatgpt](https://texttools.cn/)搜索下函数功能)

~~~ python
import numpy as np
#使用numpy生成数组,得到ndarray的类型
~~~



## 一维数组

~~~ python
t1 = np.array([1,2,3,]).astype(int)
print(t1)
[1 2 3]

print(type(t1))
<class 'numpy.ndarray'>

print(t1.shape)
(3,)
~~~

~~~ python
t3 = np.arange(4,10,2).astype(int)
print(t3)
[0 1 2 3 4 5 6 7 8 9]

print(t3.dtype)
<class 'numpy.ndarray'>
~~~



## 二维数组

~~~ python
t1 = np.array([[1,2,3,],[4,5,6]])
print(t1)
[[1 2 3]
 [4 5 6]]

print(t1.shape)
(2, 3)
~~~

~~~ python
t3 = np.arange(0,10).reshape(2,5)
print(t3)
[[0 1 2 3 4]
[5 6 7 8 9]]
~~~



## 三维数组

~~~ python 
t1 = np.array([[1,2,3,],[4,5,6],[7,8,9]])
print(t1)
[[1 2 3]
 [4 5 6]
 [7,8,9]]

print(t1.shape)
(3, 3, 3)
~~~

~~~ python
t3 = np.arange(0,27).reshape(3,3,3)
print(t3)
[[[ 0  1  2]
  [ 3  4  5]
  [ 6  7  8]]
[[ 9 10 11]
  [12 13 14]
  [15 16 17]]
[[18 19 20]
  [21 22 23]
  [24 25 26]]]
~~~



## numpy中常见的数据类型

~~~ python
#numpy中的int8类型
t4 = np.array(range(1,4),dtype="i1")
print(t4)
[1 2 3]

print(t4.dtype)
int8
~~~

~~~ python
#numpy中的bool类型
t5 = np.array([1,1,0,1,0,0],dtype=bool)
print(t5)
[ True  True False  True False False]

print(t5.dtype)
bool
~~~

~~~ python
#调整数据类型
t6 = t5.astype("int8")
print(t6)
[1 1 0 1 0 0]

print(t6.dtype)
int8
~~~

~~~ python
#numpy中随机的小数
t7 = np.array([random.random() for i in range(10)])
print(t7)
[0.04822733 0.04058194 0.09178354 0.44006835 0.18112031 0.55750956
0.21100435 0.14519092 0.18817335 0.85813295]

print(t7.dtype)
float64
~~~



![1576331071094-497666c8-f60c-43c4-b2ad-689402adcc6d](D:\Note\python\数据分析\图片\1576331071094-497666c8-f60c-43c4-b2ad-689402adcc6d.png)



~~~ python
#生成全零的数组
a = np.zeros(5).astype(int)
#生成全一的数组
b = np.ones(5).astype(int)
#生成斜角对称的数组
c = np.eye(5).astype(int)
print(a)
[0 0 0 0 0]

print(b)
[1 1 1 1 1]

print(c)
[[1 0 0 0 0]
 [0 1 0 0 0]
 [0 0 1 0 0]
 [0 0 0 1 0]
 [0 0 0 0 1]]

# 随机数种子，10是给定的种子值，加上这句话，可以保证每次生成相同的随机数
np.random.seed(10)
t = np.random.randint(0,20,(3,4))
print(t)
[[ 9  4 15  0]
 [17 16 17  8]
 [ 9  0 10  8]]
# 深拷贝，a和b互不影响
a =b.copy()
~~~



## 数组之间计算

![1576331267579-a1eeccc8-35f2-4e8f-b285-e17e4902836f](D:\Note\python\数据分析\图片\1576331267579-a1eeccc8-35f2-4e8f-b285-e17e4902836f.png)

怎么理解呢?	

可以把维度指的是shape所对应的数字个数

那么问题来了:	

shape为(3,3,3)的数组能够和(3,2)的数组进行计算么? (不可以)

shape为(3,3,2)的数组能够和(3,2)的数组进行计算么? (可以)

有什么好处呢?	

举个例子:每列的数据减去列的平均值的结果



## 轴(axis)

![1576331333107-409d2ac8-bf66-46dd-89bc-4c5d2573d4cf](D:\Note\python\数据分析\图片\1576331333107-409d2ac8-bf66-46dd-89bc-4c5d2573d4cf.png)



~~~ python
t3 = np.arange(0,18).reshape(6,3,)
print(t3)
[[ 0  1  2]
 [ 3  4  5]
 [ 6  7  8]
 [ 9 10 11]
 [12 13 14]
 [15 16 17]]

t4 = t3.transpose() #调用了矩阵的transpose()方法转置矩阵
t4 = t3.T #使用了矩阵T属性转置矩阵
print(t4)
[[ 0  3  6  9 12 15]
 [ 1  4  7 10 13 16]
 [ 2  5  8 11 14 17]]

t5 = t3.swapaxes(1,0)#将数组t3行列交换生成新数组t5,具体将t3第一列变成t5第一行
print(t5)
[[ 0  3  6  9 12 15]
 [ 1  4  7 10 13 16]
 [ 2  5  8 11 14 17]]
~~~



## 数组拼接

~~~ python 
import numpy as np
a = np.arange(0,12).reshape(2,6)
print(a)
[[ 0  1  2  3  4  5]
 [ 6  7  8  9 10 11]]

b = np.arange(12,24).reshape(2,6)
print(b)
[[12 13 14 15 16 17]
 [18 19 20 21 22 23]]

# 竖直拼接
# 将两numpy数组a和b按垂直方向堆叠生成新数组c。具体若a和b列数相同则c列数与ab相同且行数为ab行数和;若ab列数不同则抛出一ValueError异常;
c = np.vstack((a,b)) 
print(c)
[[ 0  1  2  3  4  5]
 [ 6  7  8  9 10 11]
 [12 13 14 15 16 17]
 [18 19 20 21 22 23]]

# 水平拼接
#两数组水平方向拼接，第二数组列数加第一数组列数
d = np.hstack((a,b))
print(d)
[[ 0  1  2  3  4  5 12 13 14 15 16 17]
 [ 6  7  8  9 10 11 18 19 20 21 22 23]]
~~~



## 数组行列交换

~~~ python
a = np.arange(0,12).reshape(3,4)
print(a)
[[ 0  1  2  3]
 [ 4  5  6  7]
 [ 8  9 10 11]]

#查看最后一列
print(a[:,-1])
[ 3  7 11]

#查看最后一行
print(a[-1,:])
[ 8  9 10 11]

#行交换
e = a[[0,1,2],:] = a[[2,1,0],:]
print(e)
[[ 8  9 10 11]
 [ 4  5  6  7]
 [ 0  1  2  3]]

 #列交换
f = a[:,[0,1,2]] = a[:,[2,1,0]]
print(f)
[[ 2  1  0]
 [ 6  5  4]
 [10  9  8]]
~~~



## 处理csv文件

> CSV:Comma-Separated Value,逗号分隔值文件
>
> 显示：表格状态
>
> 源文件：换行和逗号分隔行列的格式化文本,每一行的数据表示一条记录
>



由于csv便于展示,读取和写入,所以很多地方也是用csv的格式存储和传输中小型的数据，一般用numpy.loadtxt()函数从文本文件中加载数据同时也可从包含数字和文本的文件中读取数据并存储为numpy数组

~~~ python 
# 基本语法
numpy.loadtxt(fname, dtype=<class 'float'>, comments='#', delimiter=None, converters=None, skiprows=0, usecols=None, unpack=False, ndmin=0, encoding='bytes', max_rows=None)
~~~

| 参数       | 参数含义                                   |
| ---------- | ------------------------------------------ |
| fname      | 要读取的文件名或文件路径                   |
| dtype      | 返回数组数据类型，默认为"float"            |
| comment    | 注释字符，默认为"#"                        |
| delimiter  | 分隔符，默认为任何空格字符                 |
| converters | 一个字典，用于列转换成特定数据类型         |
| skiprows   | 要跳过行数，默认为0                        |
| usecols    | 要读取列索引，默认为所有列                 |
| unpack     | 若为True则将列解包为单独数组               |
| ndmin      | 返回数组最小维数                           |
| encoding   | 文件编码，默认为"bytes"                    |
| max_rows   | 要读取最大行数，默认为None，表示读取所有行 |



![1576331626192-6bffdb33-d1db-4e00-b505-e0eea7ecc5cb](D:\Note\python\数据分析\图片\1576331626192-6bffdb33-d1db-4e00-b505-e0eea7ecc5cb.png)





~~~ python
import numpy as np
us_file_path = "./youtube_video_data/US_video_data_numbers.csv"
uk_file_path = "./youtube_video_data/GB_video_data_numbers.csv"#必须指定分隔符，默认是科学技术法需要自己指定字符类型，unpack反转（行变列，列变行，如下图）
t1 = np.loadtxt(us_file_path,delimiter=",",dtype="int",unpack=True)
t2 = np.loadtxt(us_file_path,delimiter=",",dtype="int")
print(t1)
print(t2)
~~~

![1576331636496-cdb48e9f-48fa-4bbd-9f99-7a9aaef02821](D:\Note\python\数据分析\图片\1576331636496-cdb48e9f-48fa-4bbd-9f99-7a9aaef02821.png)

~~~ python 
#取行
print(t2[0])

#取连续的多行
print(t2[2:])

#取不连续的多行
print(t2[[2,8,10]])

#取列
print(t2[:,0])

#取连续的多列
print(t2[:,2:])

#取不连续的多列
print(t2[:,[0,2]])
#取多个不相邻的点

#选出来的结果是（0，0） （2，1） （2，3）
c = t2[[0,2,2],[0,1,3]]
print(c)

# 把数组中小于10的数字替换为88
a = np.arange(0,12).reshape(3,4)
print(a<8)
[[ True  True  True  True]
 [ True  True  True  True]
 [False False False False]]
a[a<8] = "88"
print(a)
[[88 88 88 88]
 [88 88 88 88]
 [ 8  9 10 11]]

# 小于5的替换为5，大于8的替换为8
a = np.arange(0,12).reshape(3,4)
b = a.clip(5,8)
print(b)

# numpy里的三目运算，小于5的替换为0，其余替换为10
b = np.where(a<5,0,10)
print(b)
a = np.arange(12).reshape(3,4)

#数组求和
print(np.sum(a))

#数组行求和
print(np.sum(a,axis=0))

#数组列求和
print(np.sum(a,axis=1))
~~~



## nan和inf

 nan(或NAN或Nan):not a number表示不是一个数字

> 什么时候numpy中会出现NAN：
>
> 1. 读本地文件类型为float时若缺则出现NAN；
>
> 2. 做了一不合适计算时(如无穷大(inf)减去无穷大)；



inf(或-inf或inf):infinity,inf表示正无穷或-inf表示负无穷

> 什么时候出现inf(或-inf或+inf):
>
> 如一数字除以0(python中直接报错，numpy中是一inf或者-inf)
>

![1576331736214-5a92a3c0-22bb-4956-87ed-0798ed2a2839](D:\Note\python\数据分析\图片\1576331736214-5a92a3c0-22bb-4956-87ed-0798ed2a2839.png)



![1576331743768-204ae58a-0085-4243-b0d2-b88e03c85e40](D:\Note\python\数据分析\图片\1576331743768-204ae58a-0085-4243-b0d2-b88e03c85e40.png)

| 操作   | 函数                                  |
| ------ | ------------------------------------- |
| 求和   | t.sum(axis=None)                      |
| 均值   | t.mean(a,axis=None)受离群点影响较大   |
| 中值   | np.median(t,axis=None)                |
| 最大值 | t.max(axis=None)                      |
| 最小值 | t.min(axis=None)                      |
| 极值   | np.ptp(t,axis=None)最大值与最小值之差 |
| 标准差 | t.std(axis=None)                      |
