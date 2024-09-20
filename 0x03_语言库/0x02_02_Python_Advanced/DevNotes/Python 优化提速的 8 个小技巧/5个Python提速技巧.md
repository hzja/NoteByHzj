Python<br />优化原则：

1. 先保证代码可以正确运行，再进行性能优化
2. 优化的选择通常是牺牲空间换取时间，所以需要权衡代价
3. 着重优化代码耗时的部分，通篇优化通常会降低代码的可读性
<a name="aiq0N"></a>
## 1、定义耗时装饰器
```python
# 可监控程序运行时间
import time
def clock(func):
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = func(*args, **kwargs)
        end_time = time.time()
        print("共耗时: %s秒" % round(end_time - start_time, 2))
        return result
    return wrapper
```
<a name="Ymtdh"></a>
## 2、避免使用全局变量
```python
start_time = time.time()
size = 10000
for x in range(size):
    for y in range(size):
        z = x * y
end_time = time.time()
print('共耗时：%s秒' % round(end_time - start_time, 2))

# 共耗时：11.78秒，不推荐
```
```python
# 使用局部变量
@clock
def multiplication():
    size = 10000
    for x in range(size):
        for y in range(size):
            z = x * y
multiplication()

# 共耗时: 5.5秒，提速50%
```
<a name="w8uy3"></a>
## 3、避免访问方法(函数)的属性，尽量使用`from X import X`
```python
import math
@clock
def computeSqrt():
    result = []
    for i in range(10000000):
        # math方法访问sqrt属性
        result.append(math.sqrt(i))
computeSqrt()

# 不推荐，共耗时: 2.09秒
```
```python
# 使用from X import X，直接访问sqrt
from math import sqrt
@clock
def computeSqrt():
    result = []
    for i in range(10000000):
        result.append(sqrt(i))
computeSqrt()

# 推荐，共耗时: 1.75秒
```
在【1】中讲到，局部变量的查找会比全局变量更快，因此对于频繁访问的变量`append`，通过将其改为局部变量可以加速运行。
```python
from math import sqrt
@clock
def computeSqrt():
    result = []
    # 赋值给局部变量
    append = result.append
    for i in range(10000000):
        append(sqrt(i))
computeSqrt()

# 推荐，共耗时: 1.45秒
```
<a name="qENOR"></a>
## 4、遍历优化
```python
# 使用while进行遍历
@clock
def circulate():
    i = 0
    li = []
    append = li.append
    while i < 10000000:
        append(i*2)
        i += 1
    return li
circulate()

# 不推荐，共耗时:1.48秒
```
```python
@clock
def circulate():
    li = []
    append = li.append
    # 使用for代替while
    for i in range(10000000):
        append(i*2)
    return li
circulate()

# for优于while，共耗时:1.09秒

@clock
def circulate():
    # 使用列表推导式
    return [i*2 for i in range(10000000)]
circulate()

# 推荐列表推导式，共耗时:0.88秒。但不适用于复杂计算。
```
<a name="NSXrs"></a>
## 5、减少内层for循环的计算
```python
from math import sqrt
@clock
def inner():
    size = 10000
    for x in range(size):
        for y in range(size):
            # 相当于在重复计算sqrt(x)
            z = sqrt(x) + sqrt(y)
inner() 

# 不推荐，共耗时:19.00秒
```
```python
from  math import sqrt
@clock
def inner():
    size = 10000
    for x in range(size):
        # 只计算一次sqrt(x)，然后将它存了起来
        sqrt_x = sqrt(x)
        for y in range(size):
            z = sqrt_x + sqrt(y)
inner() 

# 推荐，共耗时:10.22秒
```
<a name="KuW34"></a>
## 6、使用Numpy数据类型进行运算
因为Numpy底层是用C语言实现的，而Python这种脚本语言相比C/C++这种编译语言在效率和性能方面有天然劣势，所以可以引入Numpy包，对数据进行类型转换后再进行计算。
```python
import numpy as np

li = [i for i in range(10000000)]

@clock
def npSpeed():
    # 使用Python方法
    sum(li)
npSpeed() 

# 共耗时0.79秒
```
```python
import numpy as np

li = np.array([i for i in range(100000000)])
@clock
def npSpeed():
    # 使用Numpy方法
    np.sum(li)
npSpeed() 

# 共耗时0.11秒，速度约是Python的8倍
```
Numpy的优势在数据量越大时，体现的也会更加明显。所以在机器学习与深度学习项目任务中，Numpy的使用就非常频繁。
