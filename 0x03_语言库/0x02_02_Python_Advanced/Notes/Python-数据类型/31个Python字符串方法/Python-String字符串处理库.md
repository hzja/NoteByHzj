Python String 字符串
<a name="lc3RC"></a>
## string字符串处理库
字符串作为一种常见的数据类型，在日常中面临各式各样的字符串处理问题，这就要求必须掌握一些常用的字符串处理函数。<br />字符串的基本需求：查找、修改、删除、判断。
<a name="lKPmZ"></a>
### 查找
**常用于查找到函数有：**`**find()**`**、**`**index()**`**、**`**count()**`
<a name="ma2nw"></a>
#### `find()`
检测字符串是否包含特定字符，如果包含，则返回开始的索引；否则，返回-1。
```python
str='Welcome to DataScience'
print(str.find('Da'))
#11
print(str.find('wc'))
#-1
```
<a name="dZyEX"></a>
#### `index()`
检测字符串是否包含指定字符，如果包含，则返回开始的索引值；否则，提示错误。
```python
print(str.index('come'))
#3
print(str.index('Date'))
---------------------------------------------------------------------------
ValueError                                Traceback (most recent call last)
<ipython-input-12-85801a960777> in <module>
----> 1 print(str.index('Date'))
ValueError: substring not found
```
<a name="Jpf12"></a>
#### `count()`
返回str1在string中指定索引范围内`[start, end]`出现的次数。
```python
print(str.count('e'))
#4
print(str.count('Da'))
#1
```
<a name="VQOLg"></a>
### 修改
常见的用于字符串修改函数有`replace()`、`split()`、`join()`、`capitalize()`、`title()`、`upper()` 、`loewr()`、`ljust()`、`rjust()`、`center()`
<a name="5pRGD"></a>
#### `replace()`
把字符串中的 old（旧字符串）替换成 new(新字符串)，如果指定第三个参数max，则替换不超过 max 次。

- 该函数语法如下：<br />`str.replace(old, new[,max])`
- 参数说明：<br />`old` -- 将被替换的子字符串<br />`new` -- 新字符串，用于替换old子字符串
- `max` -- 可选字符串, 替换不超过 max 次
```python
str='Welcome to Data Science'
print(str.replace("Data", "data"))
#Welcome to data Science
print(str.replace("e", "E", 3))
#WElcomE to Data SciEnce
```
<a name="InGjy"></a>
#### `split()`

- 如果 maxsplit有指定值，则仅分割 `maxsplit` 个子字符串；
- 语法格式如下：<br />`str.split('分界符', maxSplit)`
- 参数说明：<br />`maxSplit`默认值为-1,表示根据定界符分割所有能分割的
- 返回值为列表
```python
str='Welcome to Data Science'
print(str.split(' '))  #用空格作为分节符
#['Welcome', 'to', 'Data', 'Science']
print(str.split('e',3))
#['W', 'lcom', ' to Data Sci', 'nce']
```
<a name="640dd2b4"></a>
#### `capitilize()`

- 将字符串的首字母大写，其余字母全部小写
- 语法结构如下：<br />`str.capitiliaze()`
- 无参数
```python
str='welcome to Data Science'
print(str.capitalize())
#Welcome to data science
```
<a name="cDX2n"></a>
#### `join()`

- 用于将序列中的元素以指定的字符连接生成一个新的字符串。
- 语法结构如下：<br />`str.join(sequence)`
- `sequence` -- 要连接的元素序列。
```python
seq='Welcome to Data Science'
str='-'
print(str.join(seq))
#W-e-l-c-o-m-e- -t-o- -D-a-t-a- -S-c-i-e-n-c-e
```
<a name="1mfIG"></a>
#### `title()`

- 将字符串中的所有单词的首字母大写，其余字母全部小写；
- 值得注意的是，这里单词的区分是以任何标点符号区分的，即，标点符号的前后都是一个独立的单词，字符串最后一个标点除外哦。详细看示例哈
- 语法结构如下：<br />`str.title()`<br />无参数
```python
str='welcome to data science'
print(str.title())
#Welcome To Data Science
str='welcome,to data-science.'   #加标点符号
print(str.title())
#Welcome,To Data-Science.
```
<a name="nDJBf"></a>
#### `lower()`

- 将字符串的所有字母转换为小写；
- 语法格式如下：<br />`str.lower()`<br />无参数
```python
str='WELCOME TO DATA SCIENCE'
print(str.lower())
#welcome to data science
```
<a name="06f7ffae"></a>
#### `upper()`

- 将字符串的所有字母转换为大写；
- 语法格式如下：

`str.upper()`<br />无参数
```python
str='welcome to data science'
print(str.upper())
#WELCOME TO DATA SCIENCE
```
<a name="To8GO"></a>
#### `ljust()`

- 将字符串左对齐，并使用空格填充至指定长度len；
- 语法格式如下：<br />`str.ljust(len)`
```python
str='Welcome to data science'
print('str的原长度为%d'%(len(str)))
print('str处理后的长度为%d'%(len(str.ljust(30))))
print(str.ljust(30))
'''
str的原长度为23
str处理后的长度为30
Welcome to data science  
'''
```
<a name="xiDkL"></a>
#### `rjust()`

- 将字符串右对齐，并使用空格填充至指定长度`len`；
- 语法格式如下：<br />`str.rjust(len)`
```python
str='Welcome to data science'
print('str的原长度为%d'%(len(str)))
print('str处理后的长度为%d'%(len(str.rjust(30))))
print(str.rjust(30))
#str的原长度为23
#str处理后的长度为30
#       Welcome to data science
```
<a name="aXwXR"></a>
#### `center()`

- 将字符串居中，并使用空格填充至指定长度len；
- 语法格式如下：<br />`str.center(len)`
```python
str='Welcome to Data Science'
print(str.center(30))
print('str的原长度为%d'%(len(str)))
print('str处理后的长度为%d'%(len(str.center(30))))
#   Welcome to Data Science    
#str的原长度为23
#str处理后的长度为30
```
<a name="YD1uh"></a>
### 删除
常用的字符串删除方法有：`**lstrip()**`**、**`**rstripe()**`**、**`**stripe()**`
<a name="pShEh"></a>
#### `lstrip()`

- 去掉字符串左边的空白字符；
- 语法格式如下：<br />`str.lstrip()`
```python
str='   Welcome to Data Science'
print(str)
print(str.lstrip())
#   Welcome to Data Science
#Welcome to Data Science
```
<a name="D2Txx"></a>
#### `rstrip()`

- 去掉字符串右边的空白字符；
- 语法格式如下：<br />`str.rstrip()`
```python
str='Welcome to Data Science   '
print(str)
print(str.rstrip())
#Welcome to Data Science  
#Welcome to Data Science
```
<a name="8PEh3"></a>
#### `strip()`

- 去掉字符串两边的空白字符；
- 语法格式如下：<br />`str.strip()`
```python
str='   Welcome to Data Science   '
print(str)
print(str.strip())
#   Welcome to Data Science  
#Welcome to Data Science
```
<a name="qURMg"></a>
### 判断
常见的用于判断字符串的函数有：`startwith()`、`endwith()`、`isalpha()`、`isdigit()`、`isalnum()` 、`isspace()`
<a name="30213c31"></a>
#### `startswith()`

- 检查字符串str是否 以字符串str1开头，若是，则返回True；否则，返回False；
- 语法结构如下：<br />`str.startswith()`
```python
str='Welcome to Data Science'
print(str.startswith('W'))
print(str.startswith('w'))
#True
#False
```
<a name="llI52"></a>
#### `endswith()`

- 检查字符串str是否 以字符串str1结尾，若是，则返回True；否则，返回False；
- 语法结构如下：<br />`str.endswith()`
```python
str='Welcome to Data Science'
print(str.endswith('e'))
print(str.endswith('c'))
#True
#False
```
<a name="s0AA2"></a>
#### `isalpha()`

- 如果字符串str中只包含字母，则返回True；否则，返回False；
- 语法结构如下：<br />`str.isalpha()`
```python
str='Welcome to Data Science'
print(str.isalpha())   #包含空格
#False
str_='asd'
print(str_.isalpha())
#True
```
<a name="QrvPI"></a>
#### `isdigit()`

- 如果字符串str中只包含数字，则返回True；否则，返回False；
- 语法结构如下：<br />`str.isdigit()`
```python
str='Welcome to Data Science'
print(str.isdigit())
#False
str_='12345'
print(str_.isdigit())
#True
```
<a name="k0CJP"></a>
#### `isalnum()`

- 检测字符串是否由字母和数字组成。
- 语法结构如下：<br />`str.isalnum()`
```python
str='Welcome to Data Science'
print(str.isalnum())
#False
str_='123data'
print(str_.isalnum())
#True
```
<a name="ARG60"></a>
#### `isspace()`

- 如果字符串str中只包含空格，则返回True；否则，返回False；
- 语法结构如下：<br />`str.isspace()`
```python
str='Welcome to Data Science'
print(str.isspace())
#False
```

