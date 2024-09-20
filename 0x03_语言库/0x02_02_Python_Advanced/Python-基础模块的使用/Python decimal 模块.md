Python decimal<br />在日常开发工作中浮点类型的使用还是比较普遍的，对于一些涉及资金金额的计算更是不能有丝毫误差，Python 的 decimal  模块为浮点型精确计算提供了支持。
<a name="fmjvH"></a>
## 1、简介
decimal 模块设计以十进制数、算术上下文和信号这三个概念为中心。十进制数是不可变的，它有一个符号，系数数字和一个指数，为了保持重要性，系数数字不会截断尾随零，十进制数也有特殊值，如：Infinity、-Infinity 和 NaN；算术上下文是指定精度、舍入规则、指数限制、指示操作结果的标志以及确定符号是否被视为异常的陷阱启用器的环境；信号是在计算过程中出现的异常条件组。对于 decimal 模块的设计原则，只需简单了解即可。
<a name="C3LDz"></a>
## 2、快速上手
<a name="fG2MI"></a>
### Decimal
看一下 Decimal 实例的基本使用。示例如下：
```python
from decimal import *

print(Decimal(1.1) + Decimal(3.3))
print(Decimal(1.1) - Decimal(3.3))
print(Decimal(1.1) * Decimal(3.3))
print(Decimal(1.1) / Decimal(3.3))

#输出结果
'''
4.399999999999999911182158030
-2.199999999999999733546474090
3.630000000000000097699626167
0.3333333333333333781908292778
'''
```
使用 `getcontext().prec` 设定有效数字。示例如下：
```python
from decimal import *

print(Decimal(1.1) / Decimal(3.3))
getcontext().prec = 2
print(Decimal(1.1) / Decimal(3.3))

#输出结果
'''
0.3333333333333333781908292778
0.33
'''
```
设置小数位数。示例如下：
```python

from decimal import *

print(Decimal(1.1) / Decimal(3.3))
print((Decimal(1.1) / Decimal(3.3)).quantize(Decimal('0.00')))

#输出结果
'''
0.3333333333333333781908292778
0.33
'''
```
转成字符串。示例如下：
```python
from decimal import *

str(Decimal('1.23465689').quantize(Decimal('0.00')))
```
<a name="cjAZV"></a>
### `adjusted()`
在移出系数最右边的数字之后返回调整后的指数，直到只剩下前导数字，该函数用于确定最高有效位相对于小数点的位置。示例如下：
```python
from decimal import *

print(Decimal('321e+5').adjusted())
```
<a name="Em90d"></a>
### `compare(other, context=None)`
比较两个 Decimal 实例的值。示例如下：
```python
from decimal import *

print(Decimal(3.3).compare(Decimal(1.1)))
```
<a name="gRRep"></a>
### `copy_abs()`
返回参数的绝对值。示例如下：
```python
from decimal import *

print(Decimal('-3.3').copy_abs())
```
<a name="XFADH"></a>
### `is_normal(context=None)`
如果参数是一个有限正规数，返回 True，如果参数是 0、次正规数、无穷大或是 NaN，返回 False。示例如下：
```python
from decimal import *

print(Decimal(1).is_normal())
```
<a name="IkZq8"></a>
### `is_zero()`
如果参数是 0，则返回 True，否则返回 False。示例如下：
```python
from decimal import *

print(Decimal(0).is_zero())
```
<a name="ff9gx"></a>
### `ln(context=None)`
返回操作数的自然对数（以 e 为底）。示例如下：
```python
from decimal import *

print(Decimal(1).ln())
```
<a name="qsjgD"></a>
### `log10(context=None)`
返回操作数的自然对数（以 10 为底）。示例如下：
```python
from decimal import *

print(Decimal(10).log10())
```
<a name="WLLkk"></a>
### `max(other, context=None)`
比较两个数值大小，并返回大的值。示例如下：
```python
from decimal import *

print(Decimal(1.1).max(Decimal(2.2)))
```
<a name="OQRUE"></a>
### max_mag(other, context=None)
比较两个数绝对值大小，并返回绝对值大的值。示例如下：
```python
from decimal import *

print(Decimal(-3.3).max_mag(Decimal(1.1)))
```
<a name="GYTsh"></a>
### `min(other, context=None)`
比较两个数值大小，并返回小的值。示例如下：
```python
from decimal import *

print(Decimal(1.1).min(Decimal(2.2)))
```
<a name="O3Ft5"></a>
### `min_mag(other, context=None)`
比较两个数绝对值大小，并返回绝对值小的值。示例如下：
```python
from decimal import *

print(Decimal(-3.3).min_mag(Decimal(1.1)))
```
<a name="hnsiX"></a>
### `is_signed()`
如果参数带有负号，则返回为 True，否则返回 False。示例如下：
```python
from decimal import *

print(Decimal(-3).is_signed())
```
<a name="W0LsX"></a>
## 参考：
[https://docs.python.org/zh-cn/3.7/library/decimal.html](https://docs.python.org/zh-cn/3.7/library/decimal.html)
