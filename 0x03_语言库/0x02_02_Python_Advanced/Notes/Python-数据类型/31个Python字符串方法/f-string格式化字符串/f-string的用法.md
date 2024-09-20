Python<br />f-string 是 Python3.6 版本开始引入的特性，想必很多 Python 用户都基础性的使用过，通过它可以更加方便地向字符串中嵌入自定义内容，但 f-string 真正蕴含的功能远比大多数用户知道的要丰富，来一起探索它们！
<a name="Ofg3P"></a>
### 基础用法
f-string 最基础的用法很简单，如下例所示，在前缀f的字符串中向`{}`内直接填入要嵌入的值、变量或计算表达式：
```python
print(f'1+1 = {2}')
# 输出：1+1 = 2
 
a = 1 + 1
print(f'1+1 = {a}')
# 输出：1+1 = 2
 
print(f'1+1 = {1+1}')
# 输出：1+1 = 2
```
<a name="VqMD1"></a>
### 自记录表达式
从 Python3.8 版本开始，f-string 引入了自记录表达式，可以按照下面的写法快速输出一些计算表达式：
```python
import math
a = 8
print(f'{math.log(8) = }')

# 输出：math.log(8) = 2.0794415416798357
```
<a name="tUNTf"></a>
### 多行 f-string
通过最外层包裹小括号，可以像下面这样非常灵活地撰写多行 f-string：
```python
a = 1
b = 2
c = 3
s = (
    f'{a = }\n'
    f'{b = }\n'
    f'{c = }\n'
)
print(s)

# 输出：
a = 1
b = 2
c = 3
```
<a name="MrlRI"></a>
### 控制浮点数精度
在 f-string 中可以按照下列方式灵活地控制浮点数的小数位数：
```python
import math
pi = math.pi
print(f'{pi:.3f}')
print(f'{pi:.8f}')

# 输出
3.142
3.14159265
```
<a name="TMWbL"></a>
### 进制转换输出
在 f-string 中，可以直接在输出时将目标转换成不同的进制进行输出：
```python
number_10 = 1024
print(f'{number_10:b}')  # 2进制
print(f'{number_10:o}')  # 8进制
print(f'{number_10:x}')  # 16进制小写
print(f'{number_10:X}')  # 16进制大写
print(f'{number_10:c}')  # ASCII编码

# 输出：
10000000000
2000
400
400
Ѐ
```
<a name="XTqBs"></a>
### 字符串对齐
f-string 默认为右对齐，可以修改对齐方式：
```python
text = 'python'
print(f'{text:>10}')  # 右对齐
print(f'{text:<10}')  # 左对齐
print(f'{text:^10}')  # 居中对齐
print(f'{text:*^10}')  # 居中对齐，用*补齐空白
n = 10
print(f'{text:#^{n}}')  # 参数嵌套，居中对齐，用~补齐空白

# 输出：
    python
python
  python
**python**
##python##
```
<a name="u1LcY"></a>
### 标准化显示宽度
当需要对 f-string 打印内容的显示最小宽度进行限制时，譬如打印出类似表格的结构，可以这样写：
```python
for x in range(1,11):
    print(f'{x:02}|{x**2:3}/{x**5:6}')
    
# 输出：
01|  1|     1
02|  4|    32
03|  9|   243
04| 16|  1024
05| 25|  3125
06| 36|  7776
07| 49| 16807
08| 64| 32768
09| 81| 59049
10|100|100000
```
<a name="DCW27"></a>
### 格式化日期
针对日期类型的变量，可以参考下面的方式，直接在 f-string 中快速地进行格式化：
```python
import datetime
now = datetime.datetime.now()
print(f'{now:%Y年%m月%d号%H点%M分%S秒}')
 
# 输出：
2022年05月21号17点04分08秒
```
<a name="RD6va"></a>
### 科学计数
可以实现指定位数的科学计数法显示方式：
```python
import math
pi = math.pi
print(f'{pi*100:.10e}')
 
# 输出
3.1415926536e+02
```
<a name="QcyI3"></a>
### 控制有效数字位数
还可以控制所显示数字的有效数字位数，即从左开始第一个不为0的数字往右一共显示的个数，当位数低于整数部分时会自动变成科学计数法格式：
```python
a = 1312.3123123123123
print(f'{a:.10g}')
#输出：1312.312312

a = 1312.3123123123123
print(f'{a:.3g}')
#输出：1.31e+03
```
