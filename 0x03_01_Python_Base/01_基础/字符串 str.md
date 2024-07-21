# 字符串 str 1.0

## 字符串遍历

~~~ python
# 字符串遍历
for ch in my_str4:
    print(ch,end=' ')
~~~

​     

## 字符串判断

~~~ python
### 方法说明
string.isalpha()   如果 string 至少有一个字符并且所有字符都是字母则返回 True
string.isdecimal() 如果 string 只包含数字则返回 True
string.islower()   如果 string 中包含至少一个区分大小写的字符，并且所有这些(区分大小写的)字符都是小写，则返回 True
string.isupper()   如果 string 中包含至少一个区分大小写的字符，并且所有这些(区分大小写的)字符都是大写，则返回 True
string.startswith(str) 检查字符串是否是以 str 开头，是则返回 True
string.endswith(str)   检查字符串是否是以 str 结束，是则返回 True

### 下面是代码
content = input('请输入内容:')
if content.isalpha():
    print('全部都是字母!')
if content.isdecimal():
    print('其中包含数字!')
if content.isdigit():
    print('输入的内容是数字!')
if content.islower():
    print('全是小写')
if content.startswith('ab'):
    print('字符串以ab开头')
~~~



## 查找和替换

~~~python
### 方法说明
string.find(str, start=0, end=len(string)) 检测 str 是否包含在 string 中，如果 start 和 end 指定范围，则检查是否包含在指定范围内，如果是返回开始的索引值，否则返回 -1
string.rfind(str, start=0, end=len(string))    类似于 find()，不过是从右边开始查找
string.index(str, start=0, end=len(string))    跟 find() 方法类似，不过如果 str 不在 string 会报错
string.rindex(str, start=0, end=len(string))   类似于 index()，不过是从右边开始
string.replace(old_str, new_str, num=string.count(old))    返回一个新字符串，把 string 中的 old_str 替换成 new_str，如果 num 指定，则替换不超过 num 次

### 下面是示例代码
my_str = 'abcdefghijcdklmn'
my_str.find('cd')
my_str.rfind('cd')
my_str.index('cd')
my_str.rindex('cd')
# 替换之后返回一个新的字符串
my_str.replace('cd', '888')
~~~



#### python replace函数替换无效

~~~ shell
str = "hello,china!"
str.replace("hell","well")
print(str)

hello，China！
进程已结束，推出代码0
~~~

写代码时发现这样替换之后并没有替换成功。

~~~ tex
原因:
	在Python中字符串是是不可变对象。
	所以字符串使用replace需要重新赋值，生成一个新的对象。
	之前没有重新引用，导致该变量 指向的是 以前的对象，实则已经发生变化，只是没有重新引用而已。
	所以要想打印出替换后的字符串需要重新赋值，如下：
~~~

~~~ shell
str = "hello,china!"
str = str.replace("hell","well")
print(str)

wello,china!
进程已结束，推出代码0
~~~



## 拆分和连接

~~~ python
# 返回元组，把字符串 string 分成一个3元素的元组 (str前面, str, str后面)
my_str = 'abcdghiefghijcdklmn'
print(my_str.partition('ghi'))

# 字符串拆分，返回列表，以 str 为分隔符拆分 string，如果 num 有指定值，则仅分隔 num + 1 个子字符串，str 默认包含 '\r', '\t', '\n' 和空格
my_str = 'aaa bbb ccc ddd'
my_str_list = my_str.split(' ')
print(my_str_list)

# 只拆第一个
txt = "Google#Runoob#Taobao#Facebook"
x = txt.split("#，1")
print(x)

# 列表拆分，返回字符串，以 string 作为分隔符，将 seq 中所有的元素（的字符串表示）合并为一个新的字符串
my_list = ['obama', 'john', 'smith', 'polly']
print('#'.join(my_list))

# 返回列表，按照行('\r', '\n', '\r\n')分隔
my_str = 'aaa\nbb\nccc'
print(my_str.splitlines())
~~~



## 大小写转换

~~~ python
### 方法说明
string.lower() 返回新字符串，转换 string 中所有大写字符为小写
string.upper() 返回新字符串，转换 string 中的小写字母为大写
1) 文本对齐
方法说明
string.ljust(width)    返回新字符串，基于原字符串左对齐，并使用空格填充至长度 width
string.rjust(width)    返回新字符串，基于原字符串右对齐，并使用空格填充至长度 width
string.center(width)   返回新字符串，基于原字符串居中，并使用空格填充至长度 width
2) 去除空白字符
方法说明
string.lstrip()    返回新字符串，截掉 string 左边（开始）的空白字符
string.rstrip()    返回新字符串，截掉 string 右边（末尾）的空白字符
string.strip() 返回新字符串，截掉 string 左右两边的空白字符

### 下面是示例代码
my_str1 = 'abc'my_str2 = 'ABC'print(my_str1.upper())
print(my_str2.lower())

my_str3 = '   abc   'print(my_str3.lstrip())
print(my_str3.rstrip())
print(my_str3.strip())

my_str4 = 'abcd'new_string = my_str4.ljust(20)
new_string = my_str4.rjust(20)
new_string = my_str4.center(20)
print(new_string)
~~~



## join拆分合并字符串

~~~python
my_list = ['aaa', 'bbb', 100, 200] # 之前100是int类型

for index in range(len(my_list)):
	my_list[index] = str(my_list[index])
	print(my_list)
new_string = '#'.join(my_list)
print(new_string)
~~~



## 字符串切片

~~~ python
my_str = 'abcdefg'


# 如何获得b字符?
print(my_str[1])

# Python支持在中括号里存放两个整数
print(my_str[1: 5])

# 切片语法 左闭右开的区间
# 如何获得defg
print(my_str[3:7])

# 字符串下表支持负数
print(my_str[-1])

# 第二个参数省略的话，默认到最后
# 第一个参数省略的话，默认从0开始
# 第三个参数省略的话，默认步长是1
print('AAA:', my_str[::])

# 步长也可以是负数 （回文，倒序排列）
print(my_str[::-1])
~~~



# 字符串 str 2.0

 字符串是 Python 中最常用数据类型，可使用引号( **'** 或 **"** )创建字符串，创建字符串很简单，只要为变量分配一值即可，如：

~~~ python
var1 = 'Hello World!'
var2 = "Runoob"
~~~



## 访问字符串中的值

不支持单字符类型，单字符在 Python 中作为一字符串使用，

访问子字符串可使用方括号 **[]** 截取字符串，截取语法如下：

~~~ tex
变量[头下标:尾下标]
~~~

> 索引值以 **0** 为开始值，**-1** 为从末尾的开始位置

![123456-20200923-1](D:\Note\python\基础\图片\123456-20200923-1.svg)



![python-str-runoob](D:\Note\python\基础\图片\python-str-runoob.png)



实例：

~~~ python
#!/usr/bin/python3
var1 = 'Hello World!'
var2 = "Runoob"
 
print ("var1[0]: ", var1[0])
print ("var2[1:5]: ", var2[1:5])
~~~

实例结果：

~~~ python
var1[0]:  H
var2[1:5]:  unoo
~~~



## 字符串更新

可截取字符串一部分与其他字段拼接，如下实例：

~~~ python
#!/usr/bin/python3
var1 = 'Hello World!'
print ("已更新字符串 : ", var1[:6] + 'Runoob!')
~~~

实例执行结果：

```python
已更新字符串 :  Hello Runoob!
```



## 转义字符

需在字符中使用特殊字符时用反斜杠 **\ ** 转义字符，如下表：

| 转义字符    | 描述                                                         | 实例                                                         |
| :---------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| \(在行尾时) | 续行符                                                       | `>>> print("line1 \ ... line2 \ ... line3") line1 line2 line3 >>> ` |
| \\          | 反斜杠符号                                                   | `>>> print("\\") \`                                          |
| \'          | 单引号                                                       | `>>> print('\'') '`                                          |
| \"          | 双引号                                                       | `>>> print("\"") "`                                          |
| \a          | 响铃                                                         | `>>> print("\a")`执行后电脑有响声。                          |
| \b          | 退格(Backspace)                                              | `>>> print("Hello \b World!") Hello World!`                  |
| \000        | 空                                                           | `>>> print("\000") >>> `                                     |
| \n          | 换行                                                         | `>>> print("\n")  >>>`                                       |
| \v          | 纵向制表符                                                   | `>>> print("Hello \v World!") Hello        World! >>>`       |
| \t          | 横向制表符                                                   | `>>> print("Hello \t World!") Hello    World! >>>`           |
| \r          | 回车，将 **\r** 后面的内容移到字符串开头，并逐一替换开头部分的字符，直至将 **\r** 后面的内容完全替换完成。 | `>>> print("Hello\rWorld!") World! >>> print('google runoob taobao\r123456') 123456 runoob taobao` |
| \f          | 换页                                                         | `>>> print("Hello \f World!") Hello        World! >>> `      |
| \yyy        | 八进制数，y 代表 0~7 的字符，例如：\012 代表换行。           | `>>> print("\110\145\154\154\157\40\127\157\162\154\144\41") Hello World!` |
| \xyy        | 十六进制数，以 \x 开头，y 代表的字符，例如：\x0a 代表换行    | `>>> print("\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21") Hello World!` |
| \other      | 其它的字符以普通格式输出                                     |                                                              |



使用**\r ** 实现百分比进度：

~~~ python
import time

for i in range(101):
    print("\r{:3}%".format(i),end=' ')
    time.sleep(0.05)
~~~



以下实例使用了不同转义字符演示单引号、换行符、制表符、退格符、换页符、ASCII、二进制、八进制数和十六进制数效果：

~~~ python
print('\'Hello, world!\'')  # 输出：'Hello, world!'

print("Hello, world!\nHow are you?")  # 输出：Hello, world!
                                        #       How are you?

print("Hello, world!\tHow are you?")  # 输出：Hello, world!    How are you?

print("Hello,\b world!")  # 输出：Hello world!

print("Hello,\f world!")  # 输出：
                           # Hello,
                           #  world!

print("A 对应的 ASCII 值为：", ord('A'))  # 输出：A 对应的 ASCII 值为： 65

print("\x41 为 A 的 ASCII 代码")  # 输出：A 为 A 的 ASCII 代码

decimal_number = 42
binary_number = bin(decimal_number)  # 十进制转换为二进制
print('转换为二进制:', binary_number)  # 转换为二进制: 0b101010

octal_number = oct(decimal_number)  # 十进制转换为八进制
print('转换为八进制:', octal_number)  # 转换为八进制: 0o52

hexadecimal_number = hex(decimal_number)  # 十进制转换为十六进制
print('转换为十六进制:', hexadecimal_number) # 转换为十六进制: 0x2a
~~~



## 字符串运算符

下表实例变量 a 值为字符串 "Hello"，b 变量值为字符串 "Python"：

| 操作符 | 描述                                                         | 实例                            |
| :----- | :----------------------------------------------------------- | :------------------------------ |
| +      | 字符串连接                                                   | a + b 输出结果： HelloPython    |
| *      | 重复输出字符串                                               | a*2 输出结果：HelloHello        |
| []     | 通过索引获取字符串中字符                                     | a[1] 输出结果 **e**             |
| [ : ]  | 截取字符串中的一部分，遵循**左闭右开**原则，str[0:2] 是不包含第 3 个字符的。 | a[1:4] 输出结果 **ell**         |
| in     | 成员运算符 - 如果字符串中包含给定的字符返回 True             | **'H' in a** 输出结果 True      |
| not in | 成员运算符 - 如果字符串中不包含给定的字符返回 True           | **'M' not in a** 输出结果 True  |
| r/R    | 原始字符串 - 原始字符串：所有的字符串都是直接按照字面的意思来使用，没有转义特殊或不能打印的字符。 原始字符串除在字符串的第一个引号前加上字母 **r**（可以大小写）以外，与普通字符串有着几乎完全相同的语法。 | `print( r'\n' ) print( R'\n' )` |
| %      | 格式字符串                                                   | 请看下一节内容。                |



~~~ python
#!/usr/bin/python3
 
a = "Hello"
b = "Python"
 
print("a + b 输出结果：", a + b)
print("a * 2 输出结果：", a * 2)
print("a[1] 输出结果：", a[1])
print("a[1:4] 输出结果：", a[1:4])
 
if( "H" in a) :
    print("H 在变量 a 中")
else :
    print("H 不在变量 a 中")
 
if( "M" not in a) :
    print("M 不在变量 a 中")
else :
    print("M 在变量 a 中")
 
print (r'\n')
print (R'\n')
~~~

以上实例输出结果为：

```python
a + b 输出结果： HelloPython
a * 2 输出结果： HelloHello
a[1] 输出结果： e
a[1:4] 输出结果： ell
H 在变量 a 中
M 不在变量 a 中
\n
\n
```



## 字符串格式化

Python 支持格式化字符串输出 。尽管可能用到非常复杂的表达式，但最基本用法是将一值插入到一有字符串格式符 %s 的字符串中；

Python 中字符串格式化使用与 C 中 sprintf 函数一样的语法；



实例：

~~~ python
#!/usr/bin/python3
 
print ("我叫 %s 今年 %d 岁!" % ('小明', 10))
~~~

实例输出结果：

~~~ python
我叫 小明 今年 10 岁!
~~~



### 字符串格式化符号

| 符  号 | 描述                                 |
| :----- | :----------------------------------- |
| %c     | 格式化字符及其ASCII码                |
| %s     | 格式化字符串                         |
| %d     | 格式化整数                           |
| %u     | 格式化无符号整型                     |
| %o     | 格式化无符号八进制数                 |
| %x     | 格式化无符号十六进制数               |
| %X     | 格式化无符号十六进制数（大写）       |
| %f     | 格式化浮点数字，可指定小数点后的精度 |
| %e     | 用科学计数法格式化浮点数             |
| %E     | 作用同%e，用科学计数法格式化浮点数   |
| %g     | %f和%e的简写                         |
| %G     | %f 和 %E 的简写                      |
| %p     | 用十六进制数格式化变量的地址         |



### 格式化操作符辅助指令

| 符号  | 功能                                                         |
| :---- | :----------------------------------------------------------- |
| *     | 定义宽度或者小数点精度                                       |
| -     | 用做左对齐                                                   |
| +     | 在正数前面显示加号( + )                                      |
| <sp>  | 在正数前面显示空格                                           |
| #     | 在八进制数前面显示零('0')，在十六进制前面显示'0x'或者'0X'(取决于用的是'x'还是'X') |
| 0     | 显示的数字前面填充'0'而不是默认的空格                        |
| %     | '%%'输出一个单一的'%'                                        |
| (var) | 映射变量(字典参数)                                           |
| m.n.  | m 是显示的最小总宽度,n 是小数点后的位数(如果可用的话)        |

Python2.6 新增一种格式化字符串函数 [str.format()](https://www.runoob.com/python/att-string-format.html)，其增强了字符串格式化功能



## 三引号

三引号允许一个字符串跨多行，字符串中可以包含换行符、制表符以及其他特殊字符

实例

~~~ python
#!/usr/bin/python3
para_str = """这是一个多行字符串的实例
多行字符串可以使用制表符
TAB ( \t )。
也可以使用换行符 [ \n ]。
"""
print (para_str)
~~~

实例执行结果

```
这是一个多行字符串的实例
多行字符串可以使用制表符
TAB (    )。
也可以使用换行符 [ 
 ]。
```



三引号让程序员从引号和特殊字符串泥潭里解脱，自始至终保持一小块字符串格式是WYSIWY，一典型用例是当需要一块HTML或者SQL时用字符串组合其中特殊字符串转义将非常繁琐。

~~~ sql
errHTML = '''
<HTML><HEAD><TITLE>
Friends CGI Demo</TITLE></HEAD>
<BODY><H3>ERROR</H3>
<B>%s</B><P>
<FORM><INPUT TYPE=button VALUE=Back
ONCLICK="window.history.back()"></FORM>
</BODY></HTML>
'''
cursor.execute('''
CREATE TABLE users (  
login VARCHAR(8), 
uid INTEGER,
prid INTEGER)
''')
~~~



## f-string

f-string 是 python3.6 之后版本添加的称为字面量格式化字符串，是新格式化字符串语法；

之前习惯用百分号 (%)：

~~~ python
>>> name = 'Runoob'
>>> 'Hello %s' % name
'Hello Runoob'
~~~



**f-string** 格式化字符串以 **f** 开头，后面跟着字符串，字符串中的表达式用大括号 {} 包起来，它会将变量或表达式计算后的值替换进去，实例如下：

~~~ python
>>> name = 'Runoob'
>>> f'Hello {name}'  # 替换变量
'Hello Runoob'
>>> f'{1+2}'         # 使用表达式
'3'

>>> w = {'name': 'Runoob', 'url': 'www.runoob.com'}
>>> f'{w["name"]}: {w["url"]}'
'Runoob: www.runoob.com'
~~~



用这种方式明显更简单而不用再判断使用 %s还是 %d

Python 3.8 版本可使用 **=** 符号拼接运算表达式与结果：

~~~ python
>>> x = 1
>>> print(f'{x+1}')   # Python 3.6
2

>>> x = 1
>>> print(f'{x+1=}')   # Python 3.8
x+1=2
~~~



## Unicode 字符串

Python2中普通字符串以8位ASCII码存储而Unicode字符串则存储为16位unicode字符串，这样能表示更多字符集。使用语法是字符串前加前缀 **u**。Python3中所有字符串都是Unicode字符串。



## 字符串内建函数

| 序号 | 方法及描述                                                   |
| :--- | :----------------------------------------------------------- |
| 1    | [capitalize()](https://www.runoob.com/python3/python3-string-capitalize.html) 将字符串的第一个字符转换为大写 |
| 2    | [center(width, fillchar)](https://www.runoob.com/python3/python3-string-center.html)返回一个指定的宽度 width 居中的字符串，fillchar 为填充的字符，默认为空格。 |
| 3    | [count(str, beg= 0,end=len(string))](https://www.runoob.com/python3/python3-string-count.html) 返回 str 在 string 里面出现的次数，如果 beg 或者 end 指定则返回指定范围内 str 出现的次数 |
| 4    | [bytes.decode(encoding="utf-8", errors="strict")](https://www.runoob.com/python3/python3-string-decode.html) Python3 中没有 decode 方法，但我们可以使用 bytes 对象的 decode() 方法来解码给定的 bytes 对象，这个 bytes 对象可以由 str.encode() 来编码返回。 |
| 5    | [encode(encoding='UTF-8',errors='strict')](https://www.runoob.com/python3/python3-string-encode.html) 以 encoding 指定的编码格式编码字符串，如果出错默认报一个ValueError 的异常，除非 errors 指定的是'ignore'或者'replace' |
| 6    | [endswith(suffix, beg=0, end=len(string))](https://www.runoob.com/python3/python3-string-endswith.html) 检查字符串是否以 suffix 结束，如果 beg 或者 end 指定则检查指定的范围内是否以 suffix 结束，如果是，返回 True,否则返回 False。 |
| 7    | [expandtabs(tabsize=8)](https://www.runoob.com/python3/python3-string-expandtabs.html) 把字符串 string 中的 tab 符号转为空格，tab 符号默认的空格数是 8 。 |
| 8    | [find(str, beg=0, end=len(string))](https://www.runoob.com/python3/python3-string-find.html) 检测 str 是否包含在字符串中，如果指定范围 beg 和 end ，则检查是否包含在指定范围内，如果包含返回开始的索引值，否则返回-1 |
| 9    | [index(str, beg=0, end=len(string))](https://www.runoob.com/python3/python3-string-index.html) 跟find()方法一样，只不过如果str不在字符串中会报一个异常。 |
| 10   | [isalnum()](https://www.runoob.com/python3/python3-string-isalnum.html) 如果字符串至少有一个字符并且所有字符都是字母或数字则返 回 True，否则返回 False |
| 11   | [isalpha()](https://www.runoob.com/python3/python3-string-isalpha.html) 如果字符串至少有一个字符并且所有字符都是字母或中文字则返回 True, 否则返回 False |
| 12   | [isdigit()](https://www.runoob.com/python3/python3-string-isdigit.html) 如果字符串只包含数字则返回 True 否则返回 False.. |
| 13   | [islower()](https://www.runoob.com/python3/python3-string-islower.html) 如果字符串中包含至少一个区分大小写的字符，并且所有这些(区分大小写的)字符都是小写，则返回 True，否则返回 False |
| 14   | [isnumeric()](https://www.runoob.com/python3/python3-string-isnumeric.html) 如果字符串中只包含数字字符，则返回 True，否则返回 False |
| 15   | [isspace()](https://www.runoob.com/python3/python3-string-isspace.html) 如果字符串中只包含空白，则返回 True，否则返回 False. |
| 16   | [istitle()](https://www.runoob.com/python3/python3-string-istitle.html) 如果字符串是标题化的(见 title())则返回 True，否则返回 False |
| 17   | [isupper()](https://www.runoob.com/python3/python3-string-isupper.html) 如果字符串中包含至少一个区分大小写的字符，并且所有这些(区分大小写的)字符都是大写，则返回 True，否则返回 False |
| 18   | [join(seq)](https://www.runoob.com/python3/python3-string-join.html) 以指定字符串作为分隔符，将 seq 中所有的元素(的字符串表示)合并为一个新的字符串 |
| 19   | [len(string)](https://www.runoob.com/python3/python3-string-len.html) 返回字符串长度 |
| 20   | [ljust(width[, fillchar\])](https://www.runoob.com/python3/python3-string-ljust.html) 返回一个原字符串左对齐,并使用 fillchar 填充至长度 width 的新字符串，fillchar 默认为空格。 |
| 21   | [lower()](https://www.runoob.com/python3/python3-string-lower.html) 转换字符串中所有大写字符为小写. |
| 22   | [lstrip()](https://www.runoob.com/python3/python3-string-lstrip.html) 截掉字符串左边的空格或指定字符。 |
| 23   | [maketrans()](https://www.runoob.com/python3/python3-string-maketrans.html) 创建字符映射的转换表，对于接受两个参数的最简单的调用方式，第一个参数是字符串，表示需要转换的字符，第二个参数也是字符串表示转换的目标。 |
| 24   | [max(str)](https://www.runoob.com/python3/python3-string-max.html) 返回字符串 str 中最大的字母。 |
| 25   | [min(str)](https://www.runoob.com/python3/python3-string-min.html) 返回字符串 str 中最小的字母。 |
| 26   | [replace(old, new [, max\])](https://www.runoob.com/python3/python3-string-replace.html) 把 将字符串中的 old 替换成 new,如果 max 指定，则替换不超过 max 次。 |
| 27   | [rfind(str, beg=0,end=len(string))](https://www.runoob.com/python3/python3-string-rfind.html) 类似于 find()函数，不过是从右边开始查找. |
| 28   | [rindex( str, beg=0, end=len(string))](https://www.runoob.com/python3/python3-string-rindex.html) 类似于 index()，不过是从右边开始. |
| 29   | [rjust(width,[, fillchar\])](https://www.runoob.com/python3/python3-string-rjust.html) 返回一个原字符串右对齐,并使用fillchar(默认空格）填充至长度 width 的新字符串 |
| 30   | [rstrip()](https://www.runoob.com/python3/python3-string-rstrip.html) 删除字符串末尾的空格或指定字符。 |
| 31   | [split(str="", num=string.count(str))](https://www.runoob.com/python3/python3-string-split.html) 以 str 为分隔符截取字符串，如果 num 有指定值，则仅截取 num+1 个子字符串 |
| 32   | [splitlines([keepends\])](https://www.runoob.com/python3/python3-string-splitlines.html) 按照行('\r', '\r\n', \n')分隔，返回一个包含各行作为元素的列表，如果参数 keepends 为 False，不包含换行符，如果为 True，则保留换行符。 |
| 33   | [startswith(substr, beg=0,end=len(string))](https://www.runoob.com/python3/python3-string-startswith.html) 检查字符串是否是以指定子字符串 substr 开头，是则返回 True，否则返回 False。如果beg 和 end 指定值，则在指定范围内检查。 |
| 34   | [strip([chars\])](https://www.runoob.com/python3/python3-string-strip.html) 在字符串上执行 lstrip()和 rstrip() |
| 35   | [swapcase()](https://www.runoob.com/python3/python3-string-swapcase.html) 将字符串中大写转换为小写，小写转换为大写 |
| 36   | [title()](https://www.runoob.com/python3/python3-string-title.html) 返回"标题化"的字符串,就是说所有单词都是以大写开始，其余字母均为小写(见 istitle()) |
| 37   | [translate(table, deletechars="")](https://www.runoob.com/python3/python3-string-translate.html) 根据 table 给出的表(包含 256 个字符)转换 string 的字符, 要过滤掉的字符放到 deletechars 参数中 |
| 38   | [upper()](https://www.runoob.com/python3/python3-string-upper.html) 转换字符串中的小写字母为大写 |
| 39   | [zfill (width)](https://www.runoob.com/python3/python3-string-zfill.html) 返回长度为 width 的字符串，原字符串右对齐，前面填充0 |
| 40   | [isdecimal()](https://www.runoob.com/python3/python3-string-isdecimal.html) 检查字符串是否只包含十进制字符，如果是返回 true，否则返回 false。 |
