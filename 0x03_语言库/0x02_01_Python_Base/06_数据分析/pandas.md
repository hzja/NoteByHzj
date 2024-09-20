# pandas

## 概述

1. Pandas是Python的一个数据分析包，该工具为解决数据分析任务而创建。
2. Pandas纳入大量库和标准数据模型，提供高效的操作数据集所需的工具。
3. Pandas提供大量能使我们快速便捷地处理数据的函数和方法。
4. Pandas是字典形式，基于NumPy创建，让NumPy为中心的应用变得更加简单。

(函数功能不会的话可以通过[chatgpt](https://texttools.cn/)搜索下函数功能)



## 组成结构

~~~ python
Series = 1darray + index

DataFrame = 2darray + index + columns

Panel = 3darray + index + columns + item
~~~

~~~ python
import pandas as pd
~~~



## 一维数据表Series

~~~ python
#通过列表创建Series
a = pd.Series([1,21,4,15,12],index=list("abced")).astype("int")
a = pd.Series(range(5),index=list("abcde"))
print(a)
a     1
b    21
c     4
e    15
d    12
dtype: int64
~~~

~~~ python
#通过字典创建Series
b = {"name":"xiaoming","age":30,"tele":100086}
c = pd.Series(b)
print(c)
name    xiaoming
age           30
tele      100086
dtype: object
    
#通过索引取值
print(c[0])
xiaoming

print(b["tele"])
100086
~~~

~~~ python
#查看所有index
print(a.index)
Index(['a', 'b', 'c', 'e', 'd'], dtype='object')

#查看所有values
print(a.values)
[ 1 21  4 15 12]
~~~

~~~ python
#pandas支持where用法，但是不太一样
print(a.where(a > 0))
a    NaN
b    1.0
c    2.0
d    3.0
e    4.0
dtype: float64
~~~



## 二维数据表DataFrame

> 用字典创建，index需要自定义
>
> 字典中的键对应DataFrame中的columns
>
> 字典中的值对应DataFrame中的values
>
> DataFrame中的index需要自定义

~~~ python
#通过列表创建DataFrame
a = pd.DataFrame(np.arange(12).reshape(3,4),index=list('abc'),columns=list('ABCD'))
print(a)
  A  B   C   D
a  0  1   2   3
b  4  5   6   7
c  8  9  10  11
~~~

~~~ python 
#通过字典创建DataFrame
b = {"name":["xiaoming","xiaohong"],"age":[30,32],"tele":[100086,10010]}
c = pd.DataFrame(b)
print(c)
  name  age    tele
0  xiaoming   30  100086
1  xiaohong   32   10010
~~~

