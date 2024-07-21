Python
<a name="SOvZ3"></a>
### ▍1、`Slicing`
`slicing`切片，按照一定条件从列表或者元组中取出部分元素（比如特定范围、索引、分割值）
```python
s = '   hello   '
s = s[:]

print(s)
#    hello


s = '   hello   '
s = s[3:8]

print(s)
# hello
```
<a name="delDF"></a>
### ▍2、`strip()`
`strip()`方法用于移除字符串头尾指定的字符（默认为空格或换行符）或字符序列。
```python
s = '   hello   '.strip()

print(s)
# hello


s = '###hello###'.strip()

print(s)
# ###hello###
```
在使用`strip()`方法时，默认去除空格或换行符，所以`#`号并没有去除。<br />可以给`strip()`方法添加指定字符，如下所示。
```python
s = '###hello###'.strip('#')

print(s)
# hello
```
此外当指定内容不在头尾处时，并不会被去除。
```python
s = ' \n \t hello\n'.strip('\n')

print(s)
#
#      hello


s = '\n \t hello\n'.strip('\n')

print(s)
#      hello
```
第一个`\n`前有个空格，所以只会去取尾部的换行符。<br />最后`strip()`方法的参数是剥离其值的所有组合，这个可以看下面这个案例。
```python
s = 'www.baidu.com'.strip('cmow.')

print(s)
# baidu
```
最外层的首字符和尾字符参数值将从字符串中剥离。字符从前端移除，直到到达一个不包含在字符集中的字符串字符为止。<br />在尾部也会发生类似的动作。
<a name="U2SrG"></a>
### ▍3、`lstrip()`
移除字符串左侧指定的字符（默认为空格或换行符）或字符序列。
```python
s = '   hello   '.lstrip()

print(s)
# hello
```
同样的，可以移除左侧所有包含在字符集中的字符串。
```python
s = 'Arthur: three!'.lstrip('Arthur: ')

print(s)
# ee!
```
<a name="DTEpT"></a>
### ▍4、`rstrip()`
移除字符串右侧指定的字符（默认为空格或换行符）或字符序列。
```python
s = '   hello   '.rstrip()

print(s)
#    hello
```
<a name="UHfjn"></a>
### ▍5、`removeprefix()`
Python3.9中移除前缀的函数。
```python
# python 3.9
s = 'Arthur: three!'.removeprefix('Arthur: ')

print(s)
# three!
```
和`strip()`相比，并不会把字符集中的字符串进行逐个匹配。
<a name="WbQbs"></a>
### ▍6、`removesuffix()`
Python3.9中移除后缀的函数。
```python
s = 'HelloPython'.removesuffix('Python')

print(s)
# Hello
```
<a name="QUE1z"></a>
### ▍7、`replace()`
把字符串中的内容替换成指定的内容。
```python
s = 'string methods in python'.replace(' ', '-')

print(s)
# string-methods-in-python


s = 'string methods in python'.replace(' ', '')

print(s)
# stringmethodsinpython
```
<a name="zI6dp"></a>
### ▍8、`re.sub()`
`re`是正则的表达式，`sub`是`substitute`表示替换。<br />`re.sub`则是相对复杂点的替换。
```python
import re
s = "string    methods in python"
s2 = s.replace(' ', '-')
print(s2)
# string----methods-in-python


s = "string    methods in python"
s2 = re.sub("\s+", "-", s)
print(s2)
# string-methods-in-python
```
和`replace()`做对比，使用`re.sub()`进行替换操作，确实更高级点。
<a name="d0XPn"></a>
### ▍9、`split()`
对字符串做分隔处理，最终的结果是一个列表。
```python
s = 'string methods in python'.split()

print(s)
# ['string', 'methods', 'in', 'python']
```
当不指定分隔符时，默认按空格分隔。
```python
s = 'string methods in python'.split(',')

print(s)
# ['string methods in python']
```
此外，还可以指定字符串的分隔次数。
```python
s = 'string methods in python'.split(' ', maxsplit=1)

print(s)
# ['string', 'methods in python']
```
<a name="VOcSa"></a>
### ▍10、`rsplit()`
从右侧开始对字符串进行分隔。
```python
s = 'string methods in python'.rsplit(' ', maxsplit=1)

print(s)
# ['string methods in', 'python']
```
<a name="D2iaY"></a>
### ▍11、`join()`
`string.join(seq)`。以string作为分隔符，将seq中所有的元素(的字符串表示)合并为一个新的字符串。
```python
list_of_strings = ['string', 'methods', 'in', 'python']
s = '-'.join(list_of_strings)

print(s)
# string-methods-in-python


list_of_strings = ['string', 'methods', 'in', 'python']
s = ' '.join(list_of_strings)

print(s)
# string methods in python
```
<a name="Hf7w6"></a>
### ▍12、`upper()`
将字符串中的字母，全部转换为大写。
```python
s = 'simple is better than complex'.upper()

print(s)
# SIMPLE IS BETTER THAN COMPLEX
```
<a name="xKbB1"></a>
### ▍13、`lower()`
将字符串中的字母，全部转换为小写。
```python
s = 'SIMPLE IS BETTER THAN COMPLEX'.lower()

print(s)
# simple is better than complex
```
<a name="IcPwQ"></a>
### ▍14、`capitalize()`
将字符串中的首个字母转换为大写。
```python
s = 'simple is better than complex'.capitalize()

print(s)
# Simple is better than complex
```
<a name="UB5aG"></a>
### ▍15、`islower()`
判断字符串中的所有字母是否都为小写，是则返回True，否则返回False。
```python
print('SIMPLE IS BETTER THAN COMPLEX'.islower()) # False

print('simple is better than complex'.islower()) # True
```
<a name="LYt4h"></a>
### ▍16、`isupper()`
判断字符串中的所有字母是否都为大写，是则返回True，否则返回False。
```python
print('SIMPLE IS BETTER THAN COMPLEX'.isupper()) # True

print('SIMPLE IS BETTER THAN complex'.isupper()) # False
```
<a name="lQCwo"></a>
### ▍17、`isalpha()`
如果字符串至少有一个字符并且所有字符都是字母，则返回 True，否则返回 False。
```python
s = 'python'
print(s.isalpha())
# True

s = '123'
print(s.isalpha())
# False

s = 'python123'
print(s.isalpha())
# False

s = 'python-123'
print(s.isalpha())
# False
```
<a name="IMsua"></a>
### ▍18、`isnumeric()`
如果字符串中只包含数字字符，则返回 True，否则返回 False。
```python
s = 'python'
print(s.isnumeric())
# False

s = '123'
print(s.isnumeric())
# True

s = 'python123'
print(s.isnumeric())
# False

s = 'python-123'
print(s.isnumeric())
# False
```
<a name="izynQ"></a>
### ▍19、`isalnum()`
如果字符串中至少有一个字符并且所有字符都是字母或数字，则返回True，否则返回 False。
```python
s = 'python'
print(s.isalnum())
# True

s = '123'
print(s.isalnum())
# True

s = 'python123'
print(s.isalnum())
# True

s = 'python-123'
print(s.isalnum())
# False
```
<a name="LGBtC"></a>
### ▍20、`count()`
返回指定内容在字符串中出现的次数。
```python
n = 'hello world'.count('o')
print(n)
# 2

n = 'hello world'.count('oo')
print(n)
# 0
```
<a name="A046r"></a>
### ▍21、`find()`
检测指定内容是否包含在字符串中，如果是返回开始的索引值，否则返回-1。
```python
s = 'Machine Learning'
idx = s.find('a')

print(idx)
print(s[idx:])
# 1
# achine Learning

s = 'Machine Learning'
idx = s.find('aa')

print(idx)
print(s[idx:])
# -1
# g
```
此外，还可以指定开始的范围。
```python
s = 'Machine Learning'
idx = s.find('a', 2)

print(idx)
print(s[idx:])
# 10
# arning
```
<a name="YaSzR"></a>
### ▍22、`rfind()`
类似于`find()`函数，返回字符串最后一次出现的位置，如果没有匹配项则返回-1。
```python
s = 'Machine Learning'
idx = s.rfind('a')

print(idx)
print(s[idx:])
# 10
# arning
```
<a name="vU6gz"></a>
### ▍23、`startswith()`
检查字符串是否是以指定内容开头，是则返回 True，否则返回 False。
```python
print('Patrick'.startswith('P'))

# True
```
<a name="xrIgf"></a>
### ▍24、`endswith()`
检查字符串是否是以指定内容结束，是则返回 True，否则返回 False。
```python
print('Patrick'.endswith('ck'))

# True
```
<a name="whn5v"></a>
### ▍25、`partition()`
`string.partition(str)`，有点像`find()`和`split()`的结合体。<br />从str出现的第一个位置起，把字符串string分成一个3 元素的元组`(string_pre_str,str,string_post_str)`，如果string中不包含str则 string_pre_str==string。
```python
s = 'Python is awesome!'
parts = s.partition('is')
print(parts)
# ('Python ', 'is', ' awesome!')

s = 'Python is awesome!'
parts = s.partition('was')
print(parts)
# ('Python is awesome!', '', '')
```
<a name="onRO0"></a>
### ▍26、`center()`
返回一个原字符串居中，并使用空格填充至长度width的新字符串。
```python
s = 'Python is awesome!'
s = s.center(30, '-')

print(s)
# ------Python is awesome!------
```
<a name="eoTTG"></a>
### ▍27、`ljust()`
返回一个原字符串左对齐，并使用空格填充至长度width的新字符串。
```python
s = 'Python is awesome!'
s = s.ljust(30, '-')

print(s)
# Python is awesome!------------
```
<a name="pRbhz"></a>
### ▍28、`rjust()`
返回一个原字符串右对齐，并使用空格填充至长度width的新字符串。
```python
s = 'Python is awesome!'
s = s.rjust(30, '-')

print(s)
# ------------Python is awesome!
```
<a name="xi8Nz"></a>
### ▍29、f-Strings
f-string是格式化字符串的新语法。<br />与其他格式化方式相比，它们不仅更易读，更简洁，不易出错，而且速度更快！
```python
num = 1
language = 'Python'
s = f'{language} is the number {num} in programming!'

print(s)
# Python is the number 1 in programming!


num = 1
language = 'Python'
s = f'{language} is the number {num*8} in programming!'

print(s)
# Python is the number 8 in programming!
```
<a name="E9618"></a>
### ▍30、`swapcase()`
翻转字符串中的字母大小写。
```python
s = 'HELLO world'
s = s.swapcase()

print(s)
# hello WORLD
```
<a name="zpoQt"></a>
### ▍31、`zfill()`
```python
string.zfill(width)
```
返回长度为`width`的字符串，原字符串string右对齐，前面填充0。
```python
s = '42'.zfill(5)
print(s)
# 00042

s = '-42'.zfill(5)
print(s)
# -0042

s = '+42'.zfill(5)
print(s)
# +0042
```
