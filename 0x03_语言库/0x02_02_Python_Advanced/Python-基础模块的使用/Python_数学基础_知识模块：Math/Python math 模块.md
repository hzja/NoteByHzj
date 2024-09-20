Python math
<a name="l5F5S"></a>
## 1、简介
math 模块提供了对 C 标准定义的数学函数的访问，但该模块并不支持复数运算，如果想使用复数预算需使用 cmath 模块，将支持计算复数的函数区分开的目的，来自于大多数开发者并不愿意像数学家一样需要学习复数的概念，开发工作几乎用不到复数，得到一个异常而不是一个复数结果也能让更早地监测到传递的参数中包含复数。可以先使用 dir 函数查看 math 中包含的内容。示例如下：
```python
import math
print(dir(math))
```
<a name="gfZ81"></a>
## 2、常用函数
<a name="QmOCE"></a>
### `ceil(x)`
返回 x 的上限，即大于或者等于 x 的最小整数，如果 x 不是一个浮点数，返回一个 Integral 值。示例如下：
```python
import math

print(math.ceil(-1))
print(math.ceil(1.024))
```
<a name="VXQlp"></a>
### `copysign(x, y)`
返回一个基于 x 的绝对值和 y 的符号的浮点数。示例如下：
```python
import math

#-1 的符号（-）赋给 1
print(math.copysign(1,-1))
#输出结果
#-1.0
```
<a name="TdgTY"></a>
### `fabs(x)`
返回 x 的绝对值。示例如下：
```python
import math

print(math.fabs(-1))
```
<a name="fZ2Hd"></a>
### `factorial(x)`
以一个整数返回 x 的阶乘，如果 x 不是整数或为负数时则将引发 ValueError。示例如下：
```python
import math

print(math.factorial(6))
print(math.factorial(-6))
```
<a name="aBDf9"></a>
### `floor(x)`
返回 x 的向下取整，小于或等于 x 的最大整数，如果 x 不是浮点数，则返回 Integral 值。示例如下：
```python
import math

print(math.floor(-1.024))
print(math.floor(1024))
```
<a name="zQpqe"></a>
### `fmod(x, y)`
得到 x/y 的余数，其值是一个浮点数。示例如下：
```python
import math

print(math.fmod(1,2))
```
<a name="xYrE1"></a>
### `frexp(x)`
返回 x 的尾数和指数对 (m, e)，m 是一个浮点数，e 是一个整数，正好是 x == m * 2**e，如果 x 为零，则返回 (0.0, 0) ，否则返回 0.5 <= abs(m) < 1 。示例如下：
```python
import math

print(math.frexp(1024))
```
<a name="jTttq"></a>
### `fsum(iterable)`
返回迭代中的精确浮点值。示例如下：
```python
import math

print(math.fsum([1,2,3,4,5,6]))
```
<a name="PrnRT"></a>
### `gcd(a, b)`
返回整数 a 和 b 的最大公约数。示例如下：
```python
import math

print(math.gcd(3, 9))
```
<a name="wLY5f"></a>
### `exp(x)`
返回 e 的 x 幂，其中 e = 2.718281... 是自然对数的基数。示例如下：
```python
import math

print(math.exp(6))
```
<a name="gMzgf"></a>
### `expm1(x)`
返回 e 的 x 次幂减 1。示例如下：
```python
import math

print(math.expm1(6))
```
<a name="ayURv"></a>
### `log(x, base=None)`
返回 x 的自然对数，默认以 e 为基数，base 参数给定时，将 x 的对数返回给定的 base，计算式为：log(x)/log(base)。示例如下：
```python
import math

print(math.log(10,24))
```
<a name="y88oW"></a>
### `log1p(x)`
返回 x+1 的自然对数(基数为 e)的值。示例如下：
```python
import math

print(math.log1p(1024))
```
<a name="ahCur"></a>
### `log2(x)`
返回 x 以 2 为底的对数，通常比 log(x, 2) 更准确。示例如下：
```python
import math

print(math.log2(1024))
```
log10(x) 与之类似，返回 x 底为 10 的对数。
<a name="KkUK1"></a>
### `pow(x, y)`
将返回 x 的 y 次幂。示例如下：
```python
import math

print(math.pow(6,2))
```
<a name="G5B8A"></a>
### `sqrt(x)`
返回 x 的平方根。示例如下：
```python
import math

print(math.sqrt(1024))
```
<a name="nlIPM"></a>
### `acos(x)`
以弧度为单位返回 x 的反余弦值，x 为 -1 到 1 之间的数值，如果 x 是大于 1，会产生一个错误。示例如下：
```python
import math

print(math.acos(0.5))
print(math.acos(3))
```
<a name="RpiKG"></a>
### `asin(x)`
以弧度为单位返回 x 的反正弦值。示例如下：
```python
import math

print(math.asin(0.5))
```
<a name="vndK0"></a>
### `atan(x)`
以弧度为单位返回 x 的反正切值。示例如下：
```python
import math

print(math.atan(0.5))
```
<a name="KETq5"></a>
### `cos(x)`
返回 x 弧度的余弦值。示例如下：
```python
import math

print(math.cos(0.5))
```
<a name="l2pvk"></a>
### `sin(x)`
返回 x 弧度的正弦值。示例如下：
```python
import math

print(math.sin(0.5))
```
<a name="AhaZt"></a>
### `tan(x)`
返回 x 弧度的正切值。示例如下：
```python
import math

print(math.sin(0.5))
```
<a name="LNNaz"></a>
## 参考
[https://docs.python.org/zh-cn/3.7/library/math.html](https://docs.python.org/zh-cn/3.7/library/math.html)
