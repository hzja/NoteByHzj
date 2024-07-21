Python utils
<a name="tnGxK"></a>
### 1、整理字符串输入
整理用户输入的问题在编程过程中极为常见。通常情况下，将字符转换为小写或大写就够了，有时可以使用正则表达式模块「Regex」完成这项工作。但是如果问题很复杂，可能有更好的方法来解决：
```python
user_input = "This\nstring has\tsome whitespaces...\r\n"
character_map = {
    ord('\n') : ' ',
    ord('\t') : ' ',
    ord('\r') : None
}
user_input.translate(character_map)  # This string has some whitespaces...
```
在本例中，可以看到空格符「 n」和「 t」都被替换成了单个空格，「 r」都被删掉了。这只是个很简单的例子，可以更进一步，使用「unicodedata」程序包生成大型重映射表，并使用其中的「combining()」进行生成和映射，可以从字符串中删除所有重音符号。
<a name="BwcDJ"></a>
### 2、迭代器切片（Slice）
如果对迭代器进行切片操作，会返回一个「TypeError」，提示生成器对象没有下标，可以用一个简单的方案来解决这个问题：
```python
import itertools  
s = itertools.islice(range(50), 10, 20)
for val in s:  
    ...
```
可以使用「itertools.islice」创建一个「islice」对象，该对象是一个迭代器，可以产生想要的项。但需要注意的是，该操作要使用切片之前的所有生成器项，以及「islice」对象中的所有项。
<a name="ymyp7"></a>
### 3、跳过可迭代对象的开头
有时要处理一些以不需要的行（如注释）开头的文件。「itertools」再次提供了一种简单的解决方案：
```python
string_from_file = """  
// Author: ...  
// License: ...  
//  
// Date: ...  
Actual content... 
 """ 
import itertools  
for line in itertools.dropwhile(lambda line: line.startswith("//"), string_from_file.split("  
")):  
print(line)
```
这段代码只打印初始注释部分之后的内容。如果只想舍弃可迭代对象的开头部分（本示例中为开头的注释行），而又不知道要这部分有多长时，这种方法就很有用了。
<a name="xhC50"></a>
### 4、只包含关键字参数的函数 (kwargs)
当使用下面的函数时，创建仅仅需要关键字参数作为输入的函数来提供更清晰的函数定义，会很有帮助：
```python
def test(*, a, b):  
    pass  
test("value for a", "value for b") 
# TypeError: test() takes 0 positional arguments...  
test(a="value", b="value 2")  # Works...
```
在关键字参数之前加上一个「`*`」就可以解决这个问题。如果将某些参数放在「`*`」参数之前，它们显然是位置参数。
<a name="AnFWM"></a>
### 5、创建支持「with」语句的对象
都知道如何使用「with」语句打开文件或获取锁，但是自己可以实现自己上下文表达式吗？是的，可以使用「`__enter__`」和「`__exit__`」来实现上下文管理协议:
```python
class Connection:  
    def __init__(self):  
        ...  
    def __enter__(self):  
        # Initialize connection...  
    def __exit__(self, type, value, traceback):  
        # Close connection...  
with Connection() as c:  
    # __enter__() executes  
    ...  
    # conn.__exit__() executes
```
这是在 Python 中最常见的实现上下文管理的方法，但是还有更简单的方法:
```python
from contextlib import contextmanager  
@contextmanager  
def tag(name):  
    print(f"<{name}>")  
    yield  
    print(f"</{name}>")  
with tag("h1"):  
    print("This is Title.")
```
上面这段代码使用 contextmanager 的 manager 装饰器实现了内容管理协议。在进入 with 块时 tag 函数的第一部分（在 yield 之前的部分）就已经执行了，然后 with 块才被执行，最后执行 tag 函数的其余部分。
<a name="veKcr"></a>
### 6、用「slots」节省内存
如果曾经编写过一个创建了某种类的大量实例的程序，那么可能已经注意到，程序突然需要大量的内存。那是因为 Python 使用字典来表示类实例的属性，这使其速度很快，但内存使用效率却不是很高。通常情况下，这并不是一个严重的问题。但是，如果程序因此受到严重的影响，不妨试一下「`__slots__`」：
```python
class Person:  
    __slots__ = ["first_name", "last_name", "phone"]  
    def __init__(self, first_name, last_name, phone):  
        self.first_name = first_name  
        self.last_name = last_name  
        self.phone = phone
```
定义了「`__slots__`」属性时，Python 没有使用字典来表示属性，而是使用小的固定大小的数组，这大大减少了每个实例所需的内存。使用「`__slots__`」也有一些缺点：不能声明任何新的属性，只能使用「`__slots__`」上现有的属性。而且，带有「`__slots__`」的类不能使用多重继承。
<a name="1h6Gp"></a>
### 7、限制「CPU」和内存使用量
如果不是想优化程序对内存或 CPU 的使用率，而是想直接将其限制为某个确定的数字，Python 也有一个对应的库可以做到：
```python
import signal  
import resource  
import os  
# To Limit CPU time  
def time_exceeded(signo, frame):  
    print("CPU exceeded...")  
    raise SystemExit(1)  
def set_max_runtime(seconds):  
    # Install the signal handler and set a resource limit  
    soft, hard = resource.getrlimit(resource.RLIMIT_CPU)  
    resource.setrlimit(resource.RLIMIT_CPU, (seconds, hard))  
    signal.signal(signal.SIGXCPU, time_exceeded)  
# To limit memory usage  
def set_max_memory(size):  
    soft, hard = resource.getrlimit(resource.RLIMIT_AS)  
    resource.setrlimit(resource.RLIMIT_AS, (size, hard))
```
可以看到，在上面的代码片段中，同时包含设置最大 CPU 运行时间和最大内存使用限制的选项。在限制 CPU 的运行时间时，首先获得该特定资源（RLIMIT_CPU）的软限制和硬限制，然后使用通过参数指定的秒数和先前检索到的硬限制来进行设置。最后，如果 CPU 的运行时间超过了限制，将发出系统退出的信号。在内存使用方面，再次检索软限制和硬限制，并使用带「size」参数的「setrlimit」和先前检索到的硬限制来设置它。
<a name="hAc6M"></a>
### 8、控制可以/不可以导入什么
有些语言有非常明显的机制来导出成员（变量、方法、接口），例如在 Golang 中只有以大写字母开头的成员被导出。然而，在 Python 中，所有成员都会被导出（除非使用了「`__all__`」）：
```python
def foo():  
    pass  
def bar():  
    pass  
__all__ = ["bar"]
```
在上面这段代码中，只有「bar」函数被导出了。同样，可以让「`__all__`」为空，这样就不会导出任何东西，当从这个模块导入的时候，会造成「AttributeError」。
<a name="oWrHG"></a>
### 9、实现比较运算符的简单方法
为一个类实现所有的比较运算符（如 `__lt__` , `__le__` , `__gt__` , `__ge__`）是很繁琐的。有更简单的方法可以做到这一点吗？这种时候，「functools.total_ordering」就是一个很好的帮手：
```python
from functools import total_ordering  
@total_ordering  
class Number:  
    def __init__(self, value):  
        self.value = value  
    def __lt__(self, other):  
        return self.value < other.value  
    def __eq__(self, other):  
        return self.value == other.value  
print(Number(20) > Number(3))  
print(Number(1) < Number(5))  
print(Number(15) >= Number(15))  
print(Number(10) <= Number(2))
```
这里的工作原理究竟是怎样的呢？用「total_ordering」装饰器简化实现对类实例排序的过程。只需要定义`__lt__`和`__eq__`就可以了，它们是实现其余操作所需要的最小的操作集合（这里也体现了装饰器的作用——填补空白）。
<a name="7q0Ln"></a>
### 10、反转字符串
以下代码使用Python切片操作来反转字符串。
```python
# Reversing a string using slicing
my_string = "ABCDE"
reversed_string = my_string[::-1]
print(reversed_string)
# Output
# EDCBA
```
<a name="nVnZt"></a>
### 11、使用标题类（首字母大写）
以下代码可用于将字符串转换为标题类。这是通过使用字符串类中的title()方法来完成。
```python
my_string = "my name is chaitanya baweja"
# using the title() function of string class
new_string = my_string.title()
print(new_string)
# Output
# My Name Is Chaitanya Baweja
```
<a name="Cwph5"></a>
### 12、查找字符串的唯一要素
以下代码可用于查找字符串中所有的唯一要素。使用其属性，其中一套字符串中的所有要素都是唯一的。
```python
my_string = "aavvccccddddeee"
# converting the string to a set
temp_set = set(my_string)
# stitching set into a string using join
new_string = .join(temp_set)
print(new_string)
```
<a name="85Yrk"></a>
### 13、输出n次字符串或列表 
可以对字符串或列表使用乘法(*)。如此一来，可以按照需求将它们任意倍增。
```python
n = 3 # number of repetitions
my_string = "abcd"
my_list = [1,2,3]
print(my_string*n)
# abcdabcdabcd
print(my_list*n)
# [1,2,3,1,2,3,1,2,3]
import streamlit as st
```
一个有趣的用例是定义一个具有恒定值的列表，假设为零。
```python
n = 4
my_list = [0]*n # n denotes the length of the required list
# [0, 0, 0, 0]
```
<a name="E2Tp7"></a>
### 14、列表解析 
在其他列表的基础上，列表解析为创建列表提供一种优雅的方式。<br />以下代码通过将旧列表的每个对象乘两次，创建一个新的列表。
```python
# Multiplying each element in a list by 2
original_list = [1,2,3,4]
new_list = [2*x for x in original_list]
print(new_list)
# [2,4,6,8]
```
<a name="zCmpR"></a>
### 15、两个变量之间的交换值
Python可以十分简单地交换两个变量间的值，无需使用第三个变量。
```python
a = 1
b = 2
a, b = b, a
print(a) # 2
print(b) # 1
```
<a name="FQ71e"></a>
### 16、将字符串拆分成子字符串列表
通过使用.split()方法，可以将字符串分成子字符串列表。还可以将想拆分的分隔符作为参数传递。
```python
string_1 = "My name is Chaitanya Baweja"
string_2 = "sample/ string 2"
# default separator
print(string_1.split())
# [ My , name , is , Chaitanya , Baweja ]
# defining separator as /
print(string_2.split( / ))
# [ sample , string 2 ]
```
<a name="tlTmE"></a>
### 17、将字符串列表整合成单个字符串
join()方法将字符串列表整合成单个字符串。在下面的例子中，使用comma分隔符将它们分开。
```python
list_of_strings = [ My , name , is , Chaitanya , Baweja ]
# Using join with the comma separator
print( , .join(list_of_strings))
# Output
# My,name,is,Chaitanya,Baweja
```
<a name="01EoF"></a>
### 18、检查给定字符串是否是回文（Palindrome）
反转字符串已经在上文中讨论过。因此，回文成为Python中一个简单的程序.
```python
my_string = "abcba"
if my_string == my_string[::-1]:
    print("palindrome")
else:
    print("not palindrome")
# Output
# palindrome
```
<a name="r0Olw"></a>
### 19、列表的要素频率
有多种方式都可以完成这项任务，最常用Python的Counter 类。Python计数器追踪每个要素的频率，Counter()反馈回一个字典，其中要素是键，频率是值。<br />也使用most_common()功能来获得列表中的most_frequent element。
```python
# finding frequency of each element in a list
from collections import Counter
my_list = [ a , a , b , b , b , c , d , d , d , d , d ]
count = Counter(my_list) # defining a counter object
print(count) # Of all elements
# Counter({ d : 5, b : 3, a : 2, c : 1})
print(count[ b ]) # of individual element
# 3
print(count.most_common(1)) # most frequent element
# [( d , 5)]
```
<a name="933a3bdb"></a>
### 20、查找两个字符串是否为anagrams
Counter类的一个有趣应用是查找anagrams。<br />anagrams指将不同的词或词语的字母重新排序而构成的新词或新词语。<br />如果两个字符串的counter对象相等，那它们就是anagrams.
```python
From collections import Counter
str_1, str_2, str_3 = "acbde", "abced", "abcda"
cnt_1, cnt_2, cnt_3  = Counter(str_1), Counter(str_2), Counter(str_3)
if cnt_1 == cnt_2:
    print( 1 and 2 anagram )
if cnt_1 == cnt_3:
    print( 1 and 3 anagram )
```
<a name="7cc348c9"></a>
### 21、使用try-except-else块
通过使用try/except块，Python 中的错误处理得以轻松解决。在该块添加else语句可能会有用。当try块中无异常情况，则运行正常。<br />如果要运行某些程序，使用 finally，无需考虑异常情况。
```python
a, b = 1,0
try:
    print(a/b)
    # exception raised when b is 0
except ZeroDivisionError:
    print("division by zero")
else:
    print("no exceptions raised")
finally:
    print("Run this always")
```
<a name="3dIav"></a>
### 22、使用列举获取索引和值对
以下脚本使用列举来迭代列表中的值及其索引。
```python
my_list = [ a ,  b ,  c ,  d ,  e ]
for index, value in enumerate(my_list):
    print( {0}: {1} .format(index, value))
# 0: a
# 1: b
# 2: c
# 3: d
# 4: e
```
<a name="5ddbf238"></a>
### 23、检查对象的内存使用
以下脚本可用来检查对象的内存使用。
```python
import sys
num = 21
print(sys.getsizeof(num))
# In Python 2, 24
# In Python 3, 28
```
<a name="221842b6"></a>
### 24、合并两个字典
在Python 2 中，使用update()方法合并两个字典，而Python3.5 使操作过程更简单。<br />在给定脚本中，两个字典进行合并。使用了第二个字典中的值，以免出现交叉的情况。
```python
dict_1 = { apple : 9,  banana : 6}
dict_2 = { banana : 4, orange : 8}
combined_dict = {**dict_1, **dict_2}
print(combined_dict)
# Output
# { apple : 9, banana : 4, orange : 8}
```
<a name="mzKWj"></a>
### 25、执行一段代码所需时间
下面的代码使用time 软件库计算执行一段代码所花费的时间。
```python
import time
start_time = time.time()
# Code to check follows
a, b = 1,2
c = a+ b
# Code to check ends
end_time = time.time()
time_taken_in_micro = (end_time- start_time)*(10**6)
print(" Time taken in micro_seconds: {0} ms").format(time_taken_in_micro)
```
<a name="CaNNy"></a>
### 26、列表清单扁平化
有时不确定列表的嵌套深度，而且只想全部要素在单个平面列表中。<br />可以通过以下方式获得：
```python
from iteration_utilities import deepflatten
# if you only have one depth nested_list, use this
def flatten(l):
  return [item for sublist in l for item in sublist]
l = [[1,2,3],[3]]
print(flatten(l))
# [1, 2, 3, 3]
# if you don t know how deep the list is nested
l = [[1,2,3],[4,[5],[6,7]],[8,[9,[10]]]]
print(list(deepflatten(l, depth=3)))
# [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
若有正确格式化的数组，Numpy扁平化是更佳选择。
<a name="IyJ1p"></a>
### 27、列表取样
通过使用random软件库，以下代码从给定的列表中生成了n个随机样本。
```python
import random
my_list = [ a , b , c , d , e ]
num_samples = 2
samples = random.sample(my_list,num_samples)
print(samples)
# [ a , e ] this will have any 2 random values
```
强烈推荐使用secrets软件库生成用于加密的随机样本。<br />以下代码仅限用于Python 3。
```python
import secrets                              # imports secure module.
secure_random = secrets.SystemRandom()      # creates a secure random object.
my_list = [ a , b , c , d , e ]
num_samples = 2
samples = secure_random.sample(my_list, num_samples)
print(samples)
# [ e , d ] this will have any 2 random values
```
<a name="n3zwq"></a>
### 28、数字化
以下代码将一个整数转换为数字列表。
```python
num = 123456
# using map
list_of_digits = list(map(int, str(num)))
print(list_of_digits)
# [1, 2, 3, 4, 5, 6]
# using list comprehension
list_of_digits = [int(x) for x in str(num)]
print(list_of_digits)
# [1, 2, 3, 4, 5, 6]
```
<a name="uKgnY"></a>
### 29、检查唯一性
以下函数将检查一个列表中的所有要素是否唯一。
```python
def unique(l):
    if len(l)==len(set(l)):
        print("All elements are unique")
    else:
        print("List has duplicates")
unique([1,2,3,4])
# All elements are unique
unique([1,1,2,3])
# List has duplicates
```
<a name="qiufi"></a>
### 30、使用slice函数命名切片
使用大量硬编码的索引值会很快搞乱维护性和可读性。一种做法是对所有索引值使用常量，但是可以做得更好：
```python
# ID   First Name   Last Name
line_record = "2        John         Smith"
ID = slice(0, 8)
FIRST_NAME = slice(9, 21)
LAST_NAME = slice(22, 27)
name = f"{line_record[FIRST_NAME].strip()} {line_record[LAST_NAME].strip()}"
# name == "John Smith"
```
在此例中，可以避免神秘的索引，方法是先使用 slice 函数命名它们，然后再使用它们。还可以通过 `.start`、`.stop`和 `.stop` 属性，来了解 slice 对象的更多信息。
<a name="yx7ZB"></a>
### 31、在运行时提示用户输入密码
许多命令行工具或脚本需要用户名和密码才能操作。因此，如果碰巧写了这样的程序，getpass 模块很有用：
```python
import getpass
user = getpass.getuser()
password = getpass.getpass()
# Do Stuff...
```
这个非常简单的包通过提取当前用户的登录名，可以提示用户输入密码。但是须注意，并非每个系统都支持隐藏密码。Python 会尝试警告，因此切记在命令行中阅读警告信息。
<a name="rCanA"></a>
### 32、查找单词/字符串的相近匹配
现在，关于 Python 标准库中一些晦涩难懂的特性。如果发现自己需要使用Levenshtein distance 【Levenshtein distance: [https://en.wikipedia.org/wiki/Levenshtein_distance](https://en.wikipedia.org/wiki/Levenshtein_distance)】之类的东西，来查找某些输入字符串的相似单词，那么 Python 的 difflib 提供支持。
```python
import difflib
difflib.get_close_matches('appel', ['ape', 'apple', 'peach', 'puppy'], n=2)
# returns ['apple', 'ape']
```
`difflib.get_close_matches` 会查找最佳的“足够好”的匹配。在这里，第一个参数与第二个参数匹配。还可以提供可选参数 n ，该参数指定要返回的最多匹配结果。另一个可选的关键字参数 cutoff （默认值为 0.6），可以设置字符串匹配得分的阈值。
<a name="eOuod"></a>
### 33、使用IP地址
如果必须使用 Python 做网络开发，可能会发现 ipaddress 模块非常有用。一种场景是从 CIDR（无类别域间路由 Classless Inter-Domain Routing）生成一系列 IP 地址：
```python
import ipaddress
net = ipaddress.ip_network('74.125.227.0/29')  # Works for IPv6 too
# IPv4Network('74.125.227.0/29')
for addr in net:
    print(addr)
# 74.125.227.0
# 74.125.227.1
# 74.125.227.2
# 74.125.227.3
# ...
```
另一个不错的功能是检查 IP 地址的网络成员资格：
```python
ip = ipaddress.ip_address("74.125.227.3")
ip in net
# True
ip = ipaddress.ip_address("74.125.227.12")
ip in net
# False
```
但是请注意，ipaddress 模块和其它与网络相关的模块之间只有有限的互通性。例如，不能将 IPv4Network 实例当成地址字符串——需要先使用 str 转换它们。更多有趣的模块参考: [https://docs.python.org/3/howto/ipaddress.html](https://docs.python.org/3/howto/ipaddress.html)
<a name="a36d968d"></a>
### 34、在Shell中调试程序崩溃
如果在 Vim 或 Emacs 中进行编码开发，那么可能会遇到这样的情况：拥有在 IDE 中那样的调试器会很有用。<br />只要用`python3.8 -i` 运行程序——一旦程序终止了， -i 会启动交互式 shell，可以查看所有的变量和调用函数。整洁，但是使用实际的调试器（pdb ）会如何呢？用以下程序（script.py ）：
```python
def func():
    return 0 / 0
func()
```
并使用`python3.8 -i script.py`运行脚本：
```python
# Script crashes...
Traceback (most recent call last):
  File "script.py", line 4, in <module>
    func()
  File "script.py", line 2, in func
    return 0 / 0
ZeroDivisionError: division by zero
>>> import pdb
>>> pdb.pm()  # Post-mortem debugger
> script.py(2)func()
-> return 0 / 0
(Pdb)
```
看到了崩溃的地方，现在设置一个断点：
```python
def func():
    breakpoint()  # import pdb; pdb.set_trace()
    return 0 / 0
func()
```
现在再次运行它：
```python
script.py(3)func()
-> return 0 / 0
(Pdb)  # we start here
(Pdb) step
ZeroDivisionError: division by zero
> script.py(3)func()
-> return 0 / 0
(Pdb)
```
大多数时候，打印语句和错误信息就足以进行调试，但是有时候，需要四处摸索，以了解程序内部正在发生的事情。在这些情况下，可以设置断点，然后程序执行时将在断点处停下，可以检查程序，例如列出函数参数、表达式求值、列出变量、或如上所示仅作单步执行。<br />pdb 是功能齐全的 Python shell，理论上可以执行任何东西，但是还需要一些调试命令，可以参考[https://docs.python.org/3/library/pdb.html%23debugger-commands#debugger-commands](https://docs.python.org/3/library/pdb.html%23debugger-commands#debugger-commands)。
<a name="GT35O"></a>
### 35、在一个类中定义多个构造函数
函数重载是编程语言（不含 Python）中非常常见的功能。即使不能重载正常的函数，仍然可以使用类方法重载构造函数：
```python
import datetime
class Date:
    def __init__(self, year, month, day):
        self.year = year
        self.month = month
        self.day = day
    @classmethod
    def today(cls):
        t = datetime.datetime.now()
        return cls(t.year, t.month, t.day)
d = Date.today()
print(f"{d.day}/{d.month}/{d.year}")
# 14/9/2019
```
如果倾向于将替代构造函数的所有逻辑放入`__init__`，并使用`*args` 、`**kwargs` 和一堆 if 语句，而不是使用类方法来解决。那可能行得通，但是却变得难以阅读和维护。<br />因此，建议将很少的逻辑放入`__init__`，并在单独的方法/构造函数中执行所有操作。这样，对于类的维护者和用户而言，得到的都是干净的代码。
<a name="xXjBC"></a>
### 36、使用装饰器缓存函数调用
是否曾经编写过一种函数，它执行昂贵的 I/O 操作或一些相当慢的递归，而且该函数可能会受益于对其结果进行缓存（存储）？如果有，那么有简单的解决方案，即使用 functools 的`lru_cache` :
```python
from functools import lru_cache
import requests
@lru_cache(maxsize=32)
def get_with_cache(url):
    try:
        r = requests.get(url)
        return r.text
    except:
        return "Not Found"
for url in ["https://google.com/",
            "https://martinheinz.dev/",
            "https://reddit.com/",
            "https://google.com/",
            "https://dev.to/martinheinz",
            "https://google.com/"]:
    get_with_cache(url)
print(get_with_cache.cache_info())
# CacheInfo(hits=2, misses=4, maxsize=32, currsize=4)
```
在此例中，用了可缓存的 GET 请求（最多 32 个缓存结果）。还可以看到，可以使用 cache_info 方法检查函数的缓存信息。装饰器还提供了 `clear_cache` 方法，用于使缓存结果无效。<br />此函数不应与具有副作用的函数一起使用，或与每次调用都创建可变对象的函数一起使用。
<a name="Y1x70"></a>
### 37、在可迭代对象中查找最频繁出现的元素
在列表中查找最常见的元素是非常常见的任务，可以使用 for 循环和字典（map），但是这没必要，因为 collections 模块中有 Counter 类：
```python
from collections import Counter
cheese = ["gouda", "brie", "feta", "cream cheese", "feta", "cheddar",
          "parmesan", "parmesan", "cheddar", "mozzarella", "cheddar", "gouda",
          "parmesan", "camembert", "emmental", "camembert", "parmesan"]
cheese_count = Counter(cheese)
print(cheese_count.most_common(3))
# Prints: [('parmesan', 4), ('cheddar', 3), ('gouda', 2)]
```
实际上，Counter 只是一个字典，将元素与出现次数映射起来，因此可以将其用作普通字典：<br />`python print(cheese_count["mozzarella"]) ¨K40K cheese_count["mozzarella"] += 1 print(cheese_count["mozzarella"]) ¨K41K`<br />除此之外，还可以使用 `update(more_words)` 方法轻松添加更多元素。Counter 的另一个很酷的特性是可以使用数学运算（加法和减法）来组合和减去 Counter 的实例。
