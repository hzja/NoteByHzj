PythonNumpy<br />Numpy 是一个强大的 Python 计算库。它提供了广泛的数学函数，可以对数组和矩阵执行各种操作。本文中将整理一些基本和常用的数学操作。

- 基本数学运算：Numpy 提供了许多基本数学函数，用于对数组执行加、减、乘、除等运算。这些函数包括 `numpy.add()`、`numpy.subtract()`、`numpy.multiply()` 和 `numpy.divide()`。
- 线性代数函数：Numpy 还提供了许多线性代数函数，用于执行矩阵乘法、行列式和求逆等运算。这些函数包括 `numpy.dot()`、`numpy.linalg.det()` 和 `numpy.linalg.inv()`。
- 统计和概率函数：Numpy 提供了许多统计和概率函数，用于执行均值、中位数、标准差和相关性等操作。这些函数包括 `numpy.mean()`、`numpy.median()`、`numpy.std()` 和 `numpy.corrcoef()`。
- 三角函数和对数函数：Numpy 还提供了许多三角函数和对数函数，用于执行正弦、余弦、正切和对数等运算。这些函数包括 `numpy.sin()`、`numpy.cos()`、`numpy.tan()` 和 `numpy.log()`。
<a name="E5dZ3"></a>
## 基本数学运算
介绍基本的数学运算:
<a name="uPlzZ"></a>
### **加法**
使用`numpy.add()`逐个添加两个数组元素。例如，要添加两个数组a和b，可以使用以下代码：
```python
import numpy as np
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
c = np.add(a, b)
print(c) # Output: [5, 7, 9]
```
也可以使用+运算符：
```python
c = a + b
print(c) # Output: [5, 7, 9]
```
<a name="kfs2g"></a>
### **减法**
`numpy.subtract()`可用于从另一个元素中减去一个数组。例如，要从数组a中减去数组b，可以使用以下代码：
```python
import numpy as np
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
c = np.subtract(a, b)
print(c) # Output: [-3, -3, -3]
```
也可以使用-运算符：
```python
c = a - b
print(c) # Output: [-3, -3, -3]
```
<a name="Kv5q6"></a>
### **乘法**
`numpy.multiply()`函数可用于按元素将两个数组相乘。例如，要将两个数组a和b相乘，可以使用以下代码：
```python
import numpy as np
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
c = np.multiply(a, b)
print(c) # Output: [4, 10, 18]
```
也可以使用`*`运算符：
```python
c = a * b
print(c) # Output: [4, 10, 18]
```
要说明的一点是，这个是逐元素乘法，点积乘法使用dot，在后面会介绍。所以这个操作要求两个变量的维度相同，如果不同则会首先进行广播操作。
<a name="OQYdM"></a>
### **除法**
`numpy.divide()`函数可用于将一个数组除以另一个元素。例如，要用数组a除以数组b，可以使用以下代码：
```python
import numpy as np
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
c = np.divide(a, b)
print(c) # Output: [0.25, 0.4, 0.5]
```
也可以使用/运算符：
```python
c = a / b
print(c) # Output: [0.25, 0.4, 0.5]
```
再次说明：上述所有函数都是在输入数组上以element wise的方式应用的，也就是逐元素方式，所以它们返回一个与输入形状相同的数组。
<a name="uhdLl"></a>
## 线性代数函数
最常见的是线性代数函数有
<a name="CrVQJ"></a>
### **点积**
`numpy.dot()`函数可用于计算两个数组的点积。例如，要计算两个1-D数组a和b的点积，可以使用以下代码：
```python
import numpy as np
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
c = np.dot(a, b)
print(c) # Output: 32
```
或者直接使用@操作符
```python
c = a @ b
print(c) # Output: 32
```
<a name="OHv0f"></a>
### **矩阵乘法**
`numpy.matmul()`函数可用于执行两个数组的矩阵乘法。例如，要执行两个2-D数组a和b的矩阵乘法，可以使用以下代码：
```python
import numpy as np
a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])
c = np.matmul(a, b)
print(c)
# Output:
# [[19 22]
# [43 50]]
```
可以使用@运算符来执行矩阵乘法：
```python
c = a @ b
print(c)
# Output:
# [[19 22]
# [43 50]]
```
<a name="hvdpF"></a>
### **转置**
`numpy.transpose()`函数可用于转置数组。例如，要转置一个2-D数组a，可以使用以下代码：
```python
import numpy as np
a = np.array([[1, 2], [3, 4]])
b = np.transpose(a)
print(b)
# Output:
# [[1 3]
# [2 4]]
```
也可以直接使用.T属性来转置数组：
```python
b = a.T
print(b)
# Output:
# [[1 3]
# [2 4]]
```
<a name="UriK0"></a>
### **行列式**
`numpy.linalg.det()`函数可用于计算正方形数组的行列式。例如，要计算二维数组a的行列式，可以使用以下代码：
```python
import numpy as np
a = np.array([[1, 2], [3, 4]])
d = np.linalg.det(a)
print(d) # Output: -2.000000000000000
```
注意，输入数组必须是正方形数组，即它必须有相同的行数和列数。
<a name="cOtuB"></a>
### 逆
`numpy.linalg.inv()`函数可用于计算正方形数组的逆_inverse_ 。例如，要计算一个2-D数组a的逆，可以使用以下代码：
```python
import numpy as np
a = np.array([[1, 2], [3, 4]])
b = np.linalg.inv(a)
print(b)
# Output:
# [[-2.   1. ]
# [ 1.5 -0.5]]
```
需要注意的是，输入数组必须是方阵，而且行列式必须非零。否则，numpy将引发LinAlgError。<br />以上就是常用的线性代数函数，还有更多函数来计算矩阵和数组上的线性代数运算，可以查看Numpy文档。
<a name="wqnjl"></a>
## 三角函数和对数函数
Numpy中包含了一些最常用的三角函数包括`Numpy.sin()`、`Numpy.cos()`、`Numpy.tan()`、`Numpy.arcsin()`、`Numpy.arccos()`、`Numpy.arctan()`或`Numpy.log()`。`numpy.sin()`的例子：
```python
import numpy as np
a = np.array([0, 30, 45, 60, 90])
b = np.sin(a)
print(b)
# Output: [ 0.         0.5         0.70710678 0.8660254   1.       ]
```
`numpy.log`计算自然对数是指数函数的倒数，因此log(exp(x)) = x。自然对数是以e为底的对数。
```python
import numpy as np
np.log([1, np.e, np.e**2, 0])
#array([ 0.,   1.,   2., -Inf])
```
以上就是Numpy中常用的数学函数的总结，希望对你有所帮助，另外就是Numpy的文档非常详尽，如果想寻找什么函数，可以直接进行查询：[https://numpy.org/doc/](https://numpy.org/doc/)
