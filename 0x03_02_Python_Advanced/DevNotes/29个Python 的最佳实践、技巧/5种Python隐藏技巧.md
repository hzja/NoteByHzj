Python 
<a name="SrSEb"></a>
## 1、`...` 对象
在Python中`...`代表着一个名为 `Ellipsis` 的对象。根据官方说明，它是一个特殊值，通常可以作为**空函数的占位符**，或是**用于**`**Numpy**`**中的切片操作**。<br />如：
```python
def my_awesome_function():
    ...
```
等同于：
```python
def my_awesome_function():
    Ellipsis
```
当然，也可以使用pass或者字符串作为占位符：
```python
def my_awesome_function():
    pass


def my_awesome_function():
    "An empty, but also awesome function"
```
他们最终的效果都是相同的。<br />接下来讲讲`...`对象是如何在Numpy中体现出作用的，创建一个 `3x3x3` 的矩阵数组，然后获取所有最内层矩阵的第二列：
```python
>>> import numpy as np
>>> array = np.arange(27).reshape(3, 3, 3)
>>> array
array([[[ 0, 1, 2],
        [ 3, 4, 5],
        [ 6, 7, 8]],

       [[ 9, 10, 11],
        [12, 13, 14],
        [15, 16, 17]],

       [[18, 19, 20],
        [21, 22, 23],
        [24, 25, 26]]])
```
为了获取最层矩阵的第二列，传统方法可能是这样的：
```python
>>> array[:, :, 1]
array([[ 1, 4, 7],
       [10, 13, 16],
       [19, 22, 25]])
```
如果会用`...`对象，则是这样的：
```python
>>> array[..., 1]
array([[ 1, 4, 7],
       [10, 13, 16],
       [19, 22, 25]])
```
不过请注意， `...` 对象仅可用于Numpy，不适用于Python内置数组。
<a name="yErjD"></a>
## 2、解压迭代对象
解压迭代对象是一个非常方便的特性：
```python
>>> a, *b, c = range(1, 11)
>>> a
1
>>> c
10
>>> b
[2, 3, 4, 5, 6, 7, 8, 9]
```
或者是：
```python
>>> a, b, c = range(3)
>>> a
0
>>> b
1
>>> c
2
```
同理，与其写这样的代码：
```python
>>> lst = [1]
>>> a = lst[0]
>>> a
1
>>> (a, ) = lst
>>> a
1
```
不如跟解压迭代对象一样，进行更优雅的赋值操作：
```python
>>> lst = [1]
>>> [a] = lst
>>> a
1
```
虽然这看起来有点蠢，但是比前一种写法更优雅一些。
<a name="judyS"></a>
## 3、展开的艺术
数组展开有各种千奇百怪的姿势，比如说：
```python
>>> l = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
>>> flattened = [elem for sublist in l for elem in sublist]
>>> flattened
[1, 2, 3, 4, 5, 6, 7, 8, 9]
```
如果对`reduce`和`lambda`有一定了解，建议使用更优雅的方式：
```python
>>> from functools import reduce
>>> reduce(lambda x,y: x+y, l)
[1, 2, 3, 4, 5, 6, 7, 8, 9]
```
`reduce`和`lambda`组合起来，就能针对 l 数组内的每个子数组做拼接操作。<br />当然，还有更神奇的方式：
```python
>>> sum(l, [])
[1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> # 其实相当于 [] + [1, 2, 3] + [4, 5, 6] + [7, 8, 9]
```
这样对二维数组做`sum`操作，就能使二维数组内的每个元素做“加”法拼接起来。<br />同样的道理，如果对三位数组做`sum`操作，就能使其变为二维数组，此时再对二维数组做`sum`操作，就能展开为一维数组。<br />虽然这个技巧很出色，但并不推荐使用，因为可读性太差了。
<a name="QC6xS"></a>
## 4、下划线 `_` 变量
每当在Python解释器，IPython或Django Console中运行表达式时，Python都会将输出的值绑定到 `_` 变量中：
```python
>>> nums = [1, 3, 7]
>>> sum(nums)
11
>>> _
11
>>>
```
由于它是一个变量，可以随时覆盖它，或像普通变量一样操作它：
```python
>>> 9 + _
20
>>> a = _
>>> a
20
```
<a name="Z1unJ"></a>
## 5、多种用途的else
很多人都不知道，`else` 可以被用于许多地方，除了典型的 `if else`, 还可以在循环和异常处理里用到它。
<a name="C5iyu"></a>
### 循环
如果需要判断循环里是否处理了某个逻辑，通常情况下会这么做：
```python
found = False
a = 0

while a < 10:
    if a == 12:
        found = True
    a += 1
if not found:
    print("a was never found")
```
如果引入else，可以少用一个变量：
```python
a = 0

while a < 10:
    if a == 12:
        break
    a += 1
else:
    print("a was never found")
```
<a name="wvLaX"></a>
### 异常处理
可以在 `try ... except ...` 中使用 else 编写未捕获到异常时的逻辑：
```python
In [13]: try:
    ...: {}['lala']
    ...: except KeyError:
    ...: print("Key is missing")
    ...: else:
    ...: print("Else here")
    ...:
Key is missing
```
这样，如果程序没有异常，则会走else分支：
```python
In [14]: try:
    ...: {'lala': 'bla'}['lala']
    ...: except KeyError:
    ...: print("Key is missing")
    ...: else:
    ...: print("Else here")
    ...:
Else here
```
如果经常做异常处理，就会知道这个技巧相当方便。
