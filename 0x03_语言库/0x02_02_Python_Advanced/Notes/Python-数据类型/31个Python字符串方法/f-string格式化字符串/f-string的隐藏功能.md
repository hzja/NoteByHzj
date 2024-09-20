Python<br />![](./img/1669164007053-a455681f-6966-4a82-81d0-3e3d89c22b78.png)
<a name="qvdgx"></a>
## 前言
说起f-string语句，第一时间的反应就是这是一个**「字符串格式化占位符」**的使用方法。<br />在Python3.6之前，都是用%s表示字符串占位的。
```python
name = "fcant"

print("我的名字是%s" %name)
```
这看起来有些怪异，为什么要用%s夹在字符串中呢？看下这个位置的%s的值是多少还要往后看，如果变量很多不就很糟心了。。<br />此外还有一种`format`的写法，
```python
>>>"{} {}".format("hello", "world")    # 不设置指定位置，按默认顺序
'hello world'

>>> "{0} {1}".format("hello", "world")  # 设置指定位置
'hello world'
```
比%s占位写法好一点，但是想看具体的值还是要看后面的尾部。<br />**「直到f-string语法的出现，一洗上面两种方式的诟病。」**
<a name="rL8R9"></a>
## f-string真香！
<a name="U60lx"></a>
### 占位输出字符串
这个功能不多讲，非常香的一个特性。
```python
name = "fcant"

print(f"我的名字是{fcant}。" )
```
执行结果：
```
我的名字是fcant
```
<a name="hHsol"></a>
### 反向输出变量名称
不知道了吧，f-string还能反向输出当前值对应的变量名，debug必用神器！
```python
name = "fcant"

print(f"我的名字是{name = }" )
```
执行结果：
```
我的名字是name = 'fcant'
```
此外在{}中可以写一些计算语句，输出时会同时输出算式和得出的结果：
```python
a = 100
print(f"{a / 3 = }")
```
执行结果：
```
a / 3 = 33.333333333333336
```
<a name="CHBxz"></a>
### 语法格式化日期
以前如果想定制化下日期的输出格式，会这样写：
```python
import datetime

now = datetime.datetime.now()
print(now.strftime("%Y-%m-%d %H:%M:%S"))
```
执行结果：
```
2022-11-22 20:08:01
```
现在可以用f-string语句替代`now.strftime`方法：
```python
import datetime

now = datetime.datetime.now()
print(f'{now:%Y-%m-%d %H:%M:%S}')
```
执行结果：
```
2022-11-22 20:09:14
```
<a name="bwcVX"></a>
### 快速进制转换输出
```python
num = 1024
print(f'{num:b}')  # 2进制
print(f'{num:o}')  # 8进制
print(f'{num:x}')  # 16进制小写
print(f'{num:X}')  # 16进制大写
print(f'{num:c}')  # ASCII编码
```
执行结果：
```
10000000000
2000
400
400
Ѐ
```
<a name="A5Uz1"></a>
### 设置对齐方式
如果在输出字符串时有层次或间隔要求，这个方法必用！
```python
s = "fcant"

print(f"{s: ^30}")   # 居中对齐
print(f"{s: >30}")   # 右对齐
print(f"{s: <30}")   # 左对齐
```
执行结果：
```
fcant
                        fcant
fcant
```
`f"{s: <30}"s`：后面有个空格，这是给定的填充符号，也可以试试其他字符，数字30则格式化以后的字符串的长度。
```python
s = "fcant"

print(f"{s:~^10}")   # 居中对齐
print(f"{s:~>10}")   # 右对齐
print(f"{s:~<10}")   # 左对齐
```
输出结果:
```
~~fcant~~
~~~~fcant
fcant~~~~
```
<a name="jAZIV"></a>
### 格式化浮点数
```python
f = 1024.1024
print(f"{f :.2f}") # 保留两位小数
```
输出结果:
```
1024.10
```
<a name="BD28O"></a>
### `!s`和`!r`
这两符号在以前的format语句中就有，其中 `!s`选择用于`__str__`设置对象格式，而`!r`选择`__repr__`设置值格式。<br />`!s`和`!r` 在f-string语句中的使用：
```python
text = 'fcant'

print(f'{text!s}') 
print(f'{text!r}') 

print(text.__str__())
print(text.__repr__())
```
输出结果：
```
fcant
'fcant'
fcant
'fcant'
```
<a name="rEn8g"></a>
## 后记
f-string作为3.6后存在的新特性，使Python简洁的特性又提升了一个层次。
