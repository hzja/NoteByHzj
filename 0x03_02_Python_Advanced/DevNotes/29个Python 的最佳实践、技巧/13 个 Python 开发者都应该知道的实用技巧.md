Python<br />都知道 Python 是目前比较流行的编程语言之一，也是每个开发人员的最爱。这里分享13关于Python开发的实用技巧，这些技巧将帮助你进行日常开发，可以提升工作效率，节省开发时间，个人觉得很用，希望它也能帮助到你。<br />现在开始吧。
<a name="vUImx"></a>
### 1、传递参数而不声明
这个技巧将让您在函数中传递无限参数，而无需在函数体中声明它们。
```python
# Pass Arguments
def Test_func(*numbers):
mul = 1
    for n in numbers:
        mul = mul * n
print(mul)
Test_func(1, 3, 4) # 12
```
<a name="MvLM3"></a>
### 2、迭代列表
可能使用循环来迭代列表，但 python 有什么惊人的迭代列表并在其上执行函数吗？看下面示例代码。
```python
# Smart way to iterate
mylst = [11, 22, 33, 44, 55]
new = [x * 2 for x in mylst]
print(new) # [22, 44, 66, 88, 110]
```
<a name="gIJ0s"></a>
### 3、更短的库名称
有时很难在一个程序的多行代码中使用长名称库，本技巧将展示如何以简单的 Python 方式缩短库名称。
```python
# Shorter Library Names
import pandas as pd 
import numpy as np
import tkinter as tk 
import time as t
```
<a name="liX1b"></a>
### 4、Pyforest
如果不需要安装 Pandas、NumPy、SciPy、nltk 等流行模块和其他 python 流行库，推荐使用这个库。在使用 Pyforest 时，不需要导入这些模块，可以简单地调用它们的函数。
```python
# pip install pyforest
import pyforest
a = np.array([[1, 2], [3, 5]])
```
如果看过上面的示例代码，就会知道没有导入 NumPy 模块并直接使用它们的功能，所以说，这就是使用 Pyforest 的好处。
<a name="cXA3c"></a>
### 5、多输入
这个简单的技巧使用一行代码获取多个输入。
```python
# Take Multiple Input
data = input("Enter num with Spaces: ").split()
print(data)
# Input
1 2 3
# Output
['1', '2', '3']
```
<a name="UvC3W"></a>
### 6、修剪原始数据
这个很棒的技巧以干净的格式修剪原始数据或 Web 数据，有时会遇到一些粗大的原始数据，其中包括一些额外的空格、特殊字符等，要清理它，可以查看以下示例代码。
```python
# Trim Data
data = "     Hello"
print(data.strip(" ")) # Hello
data = "      Hello Pythoneer"
print(data.lstrip(" ")) # Hello Pythoneer
data = "Hello Coder$$$"
print(data.rstrip("$")) # Hello Coder
```
<a name="fXnpp"></a>
### 7、运行时处理错误
运行时错误总是让人头疼，为了避免这种头痛，可以在 Python 中使用运行时错误处理语句。
```python
# Handling Runtime Error
x = 6
try:
    if 5 > 3:
        x = x * y
else:
        x = x + y
except:
    print("Y is not defined")
# Output
Error Caught
Y is not defined
```
<a name="N38NN"></a>
### 8、lambda 函数
本技巧将指导您使用 lambda 函数在一行中编写小函数，查看下面的示例代码以了解 lambda 的工作原理。
```python
# One liner functions
#example 1
mul = lambda x : x * 2
print(mul(3)) # 6
#example 2 
mul = lambda x,y : x * y * 2
print(mul(1,2)) # 4
```
<a name="qlkWQ"></a>
### 9、Yield的魔力
Yield 是 Python 中的一个关键字，用于在不破坏其当前状态和局部变量的情况下从函数返回，并且当再次调用该函数时，yield 将从最后一个 yield 语句执行该函数。
```python
# Yield
def func():
    yield 1
    yield 2
    yield 3
    yield 4
for x in func():
    print(x)
#Output:
1
2
3
4
```
通过上面的示例代码，可以知道，当第一个 yield 返回并且在整个循环中再次调用函数时，它从最后一个 yield 开始，这次是第二个 yield 语句调用。
<a name="CJpht"></a>
### 10、局部变量和全局变量
这个技巧将指导您如何在函数中声明全局变量和局部变量，通过查看下面的示例代码，可以了解它们的区别。
```python
# Local and Global Variables
# Local varibles
a = 5
b = 6
# Global
def func():
    global a
    a = 6 * 2
global a 
a = 0
```
<a name="JTqfk"></a>
### 11、智能字典
很多 Python程序员使用括号方法来访问字典中键值的数据，但是，当遇到关键错误时，程序将停止，为了避免这个运行时发生错误，可以获取一种方法来访问字典中的键值。
```python
# Dictionary in Smart way
mydict = {"a" : 10, "b" : 20, "c" : 30}
# Best way
mydict.get(mydict["d"]) # None
# default way
mydict["d"] # KeyError
```
<a name="nBenN"></a>
### 12、智能数据交换
这个简单的技巧将向您展示，在不使用临时变量的情况下交换两个值的友好方法。
```python
d1 = 55
d2 = 66
d2, d1 = d1, d2
print(d1, d2)
#output
66 55
```
<a name="NJlB9"></a>
### 13、Division 2.0
当需要对两个数字进行除法并以商和余数的形式获得结果时，此提示和技巧很有用，查看下面的代码示例以获得更好的理解。
```python
# Division
x = 5
y = 3
div = divmod(x, y)
print(div) # (1, 2) --> (Quotient, Remainder)
```
