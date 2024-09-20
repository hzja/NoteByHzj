Python
<a name="URa0l"></a>
### 1、检查 Python 的最低版本
可以在代码中检查 Python 的版本，以确保用户没有在不兼容的版本中运行脚本。检查方式如下：
```python
if not sys.version_info > (2, 7):
   # berate your user for running a 10 year
   # python version
elif not sys.version_info >= (3, 5):
   # Kindly tell your user (s)he needs to upgrade
   # because you're using 3.5 features
```
<a name="TwkdN"></a>
### 2、IPython
IPython 本质上就是一个增强版的shell。就冲着自动补齐就值得一试，而且它的功能还不止于此，它还有很多让人爱不释手的命令，例如：

- %cd：改变当前的工作目录
- %edit：打开编辑器，并关闭编辑器后执行键入的代码
- %env：显示当前环境变量
- %pip install [pkgs]：无需离开交互式shell，就可以安装软件包
- %time 和 %timeit：测量执行Python代码的时间

完整的命令列表，请点击此处查看（[https://ipython.readthedocs.io/en/stable/interactive/magics.html](https://ipython.readthedocs.io/en/stable/interactive/magics.html)）。<br />还有一个非常实用的功能：引用上一个命令的输出。In 和 Out 是实际的对象。可以通过 Out[3] 的形式使用第三个命令的输出。<br />IPython 的安装命令如下：
```bash
pip3 install ipython
```
<a name="dzSak"></a>
### 3、列表推导式
可以利用列表推导式，避免使用循环填充列表时的繁琐。列表推导式的基本语法如下：
```python
[ expression for item in list if conditional ]
```
举一个基本的例子：用一组有序数字填充一个列表：
```python
mylist = [i for i in range(10)]
print(mylist)
# [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```
由于可以使用表达式，所以也可以做一些算术运算：
```python
squares = [x**2 for x in range(10)]
print(squares)
# [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
```
甚至可以调用外部函数：
```python
def some_function(a):
    return (a + 5) / 2
my_formula = [some_function(i) for i in range(10)]
print(my_formula)
# [2, 3, 3, 4, 4, 5, 5, 6, 6, 7]
```
最后，还可以使用 ‘if’ 来过滤列表。在如下示例中，只保留能被2整除的数字：
```python
filtered = [i for i in range(20) if i%2==0]
print(filtered)
# [0, 2, 4, 6, 8, 10, 12, 14, 16, 18]
```
<a name="aqU59"></a>
### 4、检查对象使用内存的状况
可以利用 sys.getsizeof() 来检查对象使用内存的状况：
```python
import sys
mylist = range(0, 10000)
print(sys.getsizeof(mylist))
# 48
```
为什么这个巨大的列表仅包含48个字节？<br />因为这里的 range 函数返回了一个类，只不过它的行为就像一个列表。在使用内存方面，range 远比实际的数字列表更加高效。<br />可以试试看使用列表推导式创建一个范围相同的数字列表： 
```python
import sys
myreallist = [x for x in range(0, 10000)]
print(sys.getsizeof(myreallist))
# 87632
```
<a name="zjHDn"></a>
### 5、返回多个值
Python 中的函数可以返回一个以上的变量，而且还无需使用字典、列表或类。如下所示：
```python
def get_user(id):
    # fetch user from database
    # ....
    return name, birthdate
name, birthdate = get_user(4)
```
如果返回值的数量有限当然没问题。但是，如果返回值的数量超过3个，那么就应该将返回值放入一个（数据）类中。
<a name="pwo0g"></a>
### 6、使用数据类
Python从版本3.7开始提供数据类。与常规类或其他方法（比如返回多个值或字典）相比，数据类有几个明显的优势：

- 数据类的代码量较少
- 可以比较数据类，因为数据类提供了 __eq__ 方法
- 调试的时候，可以轻松地输出数据类，因为数据类还提供了 __repr__ 方法
- 数据类需要类型提示，因此可以减少Bug的发生几率 

数据类的示例如下：
```python
from dataclasses import dataclass
@dataclass
class Card:
    rank: str
    suit: str
card = Card("Q", "hearts")
print(card == card)
# True
print(card.rank)
# 'Q'
print(card)
Card(rank='Q', suit='hearts')
```
详细的使用指南请点击这里（[https://realpython.com/python-data-classes/](https://realpython.com/python-data-classes/)）。
<a name="6ejUr"></a>
### 7、交换变量
如下的小技巧很巧妙，可以为节省多行代码：
```python
a = 1
b = 2
a, b = b, a
print (a)
# 2
print (b)
# 1
```
<a name="fQRew"></a>
### 8、合并字典（Python 3.5以上的版本）
从Python 3.5开始，合并字典的操作更加简单了：
```python
dict1 = { 'a': 1, 'b': 2 }
dict2 = { 'b': 3, 'c': 4 }
merged = { **dict1, **dict2 }
print (merged)
# {'a': 1, 'b': 3, 'c': 4}
```
如果 key 重复，那么第一个字典中的 key 会被覆盖。
<a name="5aBZV"></a>
### 9、字符串的首字母大写
```python
mystring = "10 awesome python tricks"
print(mystring.title())
'10 Awesome Python Tricks'
```
<a name="ac688f17"></a>
### 10、将字符串分割成列表
可以将字符串分割成一个字符串列表。在如下示例中，利用空格分割各个单词：
```python
mystring = "The quick brown fox"
mylist = mystring.split(' ')
print(mylist)
# ['The', 'quick', 'brown', 'fox']
```
<a name="bzOnV"></a>
### 11、根据字符串列表创建字符串
与上述技巧相反，可以根据字符串列表创建字符串，然后在各个单词之间加入空格：
```python
mylist = ['The', 'quick', 'brown', 'fox']
mystring = " ".join(mylist)
print(mystring)
# 'The quick brown fox'
```
为什么不是 mylist.join(" ")，这是个好问题！<br />根本原因在于，函数 String.join() 不仅可以联接列表，而且还可以联接任何可迭代对象。将其放在String中是为了避免在多个地方重复实现同一个功能。
<a name="j6lDN"></a>
### 12、表情符
![](./img/1603847542454-334207c5-dacb-44b8-8b5c-9504769316c7.webp)<br />有些人非常喜欢表情符，而有些人则深恶痛绝。但是在分析社交媒体数据时，表情符可以派上大用场。<br />首先安装表情符模块：
```python
pip3 install emoji
```
安装完成后，可以按照如下方式使用：
```python
import emoji
result = emoji.emojize('Python is :thumbs_up:')
print(result)
# 'Python is 👍'
# You can also reverse this:
result = emoji.demojize('Python is 👍')
print(result)
# 'Python is :thumbs_up:'
```
更多有关表情符的示例和文档，请点击此处（[https://pypi.org/project/emoji/](https://pypi.org/project/emoji/)）。
<a name="a9jQW"></a>
### 13、列表切片
列表切片的基本语法如下：
```python
a[start:stop:step]
```
start、stop 和 step 都是可选项。如果不指定，则会使用如下默认值：

- start：0
- end：字符串的结尾
- step：1

示例如下：
```python
# We can easily create a new list from 
# the first two elements of a list:
first_two = [1, 2, 3, 4, 5][0:2]
print(first_two)
# [1, 2]
# And if we use a step value of 2, 
# we can skip over every second number
# like this:
steps = [1, 2, 3, 4, 5][0:5:2]
print(steps)
# [1, 3, 5]
# This works on strings too. In Python,
# you can treat a string like a list of
# letters:
mystring = "abcdefdn nimt"[::2]
print(mystring)
# 'aced it'
```
<a name="3cd4313c"></a>
### 14、反转字符串和列表
可以利用如上切片的方法来反转字符串或列表。只需指定 step 为 -1，就可以反转其中的元素：
```python
revstring = "abcdefg"[::-1]
print(revstring)
# 'gfedcba'
revarray = [1, 2, 3, 4, 5][::-1]
print(revarray)
# [5, 4, 3, 2, 1]
```
<a name="278521f1"></a>
### 15、显示图片
首先需要安装 Pillow，这是一个 Python 图片库的分支：
```python
pip3 install Pillow
```
接下来，可以将如下图片下载到一个名叫 kittens.jpg 的文件中：<br />![](./img/1603847542368-b9d54f42-322d-4d5a-9736-60f5a0438955.webp)<br />然后，就可以通过如下 Python 代码显示上面的图片：
```python
from PIL import Image
im = Image.open("kittens.jpg")
im.show()
print(im.format, im.size, im.mode)
# JPEG (1920, 1357) RGB
```
Pillow 还有很多显示该图片之外的功能。它可以分析、调整大小、过滤、增强、变形等等。完整的文档，请点击这里（[https://pillow.readthedocs.io/en/stable/](https://pillow.readthedocs.io/en/stable/)）。
<a name="fda98620"></a>
### 16、`map()`
Python 有一个自带的函数叫做 `map()`，语法如下：
```python
map(function, something_iterable)
```
所以，需要指定一个函数来执行，或者一些东西来执行。任何可迭代对象都可以。在如下示例中，指定了一个列表：
```python
def upper(s):
    return s.upper()
mylist = list(map(upper, ['sentence', 'fragment']))
print(mylist)
# ['SENTENCE', 'FRAGMENT']
# Convert a string representation of
# a number into a list of ints.
list_of_ints = list(map(int, "1234567")))
print(list_of_ints)
# [1, 2, 3, 4, 5, 6, 7]
```
看看能不能用 `map()` 替代某处的循环。
<a name="RHxRq"></a>
### 17、获取列表或字符串中的唯一元素
如果利用函数 `set()` 创建一个集合，就可以获取某个列表或类似于列表的对象的唯一元素：
```python
mylist = [1, 1, 2, 3, 4, 5, 5, 5, 6, 6]
print (set(mylist))
# {1, 2, 3, 4, 5, 6}
# And since a string can be treated like a 
# list of letters, you can also get the 
# unique letters from a string this way:
print (set("aaabbbcccdddeeefff"))
# {'a', 'b', 'c', 'd', 'e', 'f'}
```
<a name="Ef9cH"></a>
### 18、查找出现频率最高的值
可以通过如下方法查找出现频率最高的值：
```python
test = [1, 2, 3, 4, 2, 2, 3, 1, 4, 4, 4]
print(max(set(test), key = test.count))
# 4
```

- `max()` 会返回列表的最大值。参数 key 会接受一个参数函数来自定义排序，在本例中为 `test.count`。该函数会应用于迭代对象的每一项。
- `test.count` 是 list 的内置函数。它接受一个参数，而且还会计算该参数的出现次数。因此，`test.count(1)` 将返回2，而 `test.count(4)` 将返回4。
- `set(test)` 将返回 test 中所有的唯一值，也就是 `{1, 2, 3, 4}`。

因此，这一行代码完成的操作是：首先获取 test 所有的唯一值，即`{1, 2, 3, 4}`；然后，max 会针对每一个值执行 `list.count`，并返回最大值。
<a name="fmDy8"></a>
### 19、创建一个进度条
可以创建自己的进度条，听起来很有意思。但是，更简单的方法是使用 progress 包：
```bash
pip3 install progress
```
接下来，就可以轻松地创建进度条了：
```python
from progress.bar import Bar
bar = Bar('Processing', max=20)
for i in range(20):
    # Do some work
    bar.next()
bar.finish()
```
<a name="mmyYz"></a>
### 20、在交互式shell中使用_（下划线运算符）
通过下划线运算符获取上一个表达式的结果，例如在 IPython 中，可以这样操作：
```python
In [1]: 3 * 3
Out[1]: 9In [2]: _ + 3
Out[2]: 12
```
Python Shell 中也可以这样使用。另外，在 IPython shell 中，还可以通过 Out[n] 获取表达式 In[n] 的值。例如，在如上示例中，Out[1] 将返回数字9。
<a name="gaCE3"></a>
### 21、快速创建Web服务器
通过下面的命令可以快速启动一个Web服务，并提供当前目录的内容：
```python
python3 -m http.server
```
当与同事共享某个文件，或测试某个简单的HTML网站时，就可以考虑这个方法。
<a name="51kV0"></a>
### 22、多行字符串
虽然可以用三重引号将代码中的多行字符串括起来，但是这种做法并不理想。所有放在三重引号之间的内容都会成为字符串，包括代码的格式，如下所示。<br />另一种更好的解决办法，这种方法不仅可以将多行字符串连接在一起，而且还可以保证代码的整洁。唯一的缺点是需要明确指定换行符。
```python
s1 = """Multi line strings can be put
        between triple quotes. It's not ideal
        when formatting your code though"""
print (s1)
# Multi line strings can be put
#         between triple quotes. It's not ideal
#         when formatting your code though
s2 = ("You can also concatenate multiple\n" +
        "strings this way, but you'll have to\n"
        "explicitly put in the newlines")
print(s2)
# You can also concatenate multiple
# strings this way, but you'll have to
# explicitly put in the newlines
```
<a name="Ei6Vr"></a>
### 23、条件赋值中的三元运算符
这种方法可以让代码更简洁，同时又可以保证代码的可读性：
```python
[on_true] if [expression] else [on_false]
```
示例如下：
```python
x = "Success!" if (y == 2) else "Failed!"
```
<a name="0310477a"></a>
### 24、统计元素的出现次数
可以使用集合库中的 `Counter` 来获取列表中所有唯一元素的出现次数，`Counter` 会返回一个字典：
```python
from collections import Counter
mylist = [1, 1, 2, 3, 4, 5, 5, 5, 6, 6]
c = Counter(mylist)
print(c)
# Counter({1: 2, 2: 1, 3: 1, 4: 1, 5: 3, 6: 2})
# And it works on strings too:
print(Counter("aaaaabbbbbccccc"))
# Counter({'a': 5, 'b': 5, 'c': 5})
```
<a name="yVwyM"></a>
### 25、比较运算符的链接
可以在 Python 中将多个比较运算符链接到一起，如此就可以创建更易读、更简洁的代码：
```python
x = 10
# Instead of:
if x > 5 and x < 15:
    print("Yes")
# yes
# You can also write:
if 5 < x < 15:
    print("Yes")
# Yes
```
<a name="tfKzR"></a>
### 26、添加颜色
可以通过 Colorama，设置终端的显示颜色：
```python
from colorama import Fore, Back, Style
print(Fore.RED + 'some red text')
print(Back.GREEN + 'and with a green background')
print(Style.DIM + 'and in dim text')
print(Style.RESET_ALL)
print('back to normal now')
```
<a name="6a44872a"></a>
### 27、日期的处理
python-dateutil 模块作为标准日期模块的补充，提供了非常强大的扩展，可以通过如下命令安装： 
```bash
pip3 install python-dateutil
```
可以利用该库完成很多神奇的操作。举一个例子：模糊分析日志文件中的日期：
```python
from dateutil.parser import parse
logline = 'INFO 2020-01-01T00:00:01 Happy new year, human.'
timestamp = parse(log_line, fuzzy=True)
print(timestamp)
# 2020-01-01 00:00:01
```
只需记住：当遇到常规 Python 日期时间功能无法解决的问题时，就可以考虑 python-dateutil ！
<a name="17AH8"></a>
### 28、整数除法
在 Python 2 中，除法运算符（/）默认为整数除法，除非其中一个操作数是浮点数。因此，可以这么写：
```python
# Python 2
5 / 2 = 2
5 / 2.0 = 2.5
```
在 Python 3 中，除法运算符（/）默认为浮点除法，而整数除法的运算符为 //。因此，需要这么写：
```python
Python 3
5 / 2 = 2.5
5 // 2 = 2
```
这项变更背后的动机，请参阅 PEP-0238（[https://www.python.org/dev/peps/pep-0238/](https://www.python.org/dev/peps/pep-0238/)）。
<a name="meGge"></a>
### 29、通过chardet 来检测字符集
可以使用 chardet 模块来检测文件的字符集。在分析大量随机文本时，这个模块十分实用。安装方法如下：
```bash
pip install chardet
```
安装完成后，就可以使用命令行工具 chardetect 了，使用方法如下：
```python
chardetect somefile.txt
somefile.txt: ascii with confidence 1.0
```
也可以在编程中使用该库，完整的文档请点击这里（[https://chardet.readthedocs.io/en/latest/usage.html](https://chardet.readthedocs.io/en/latest/usage.html)）。
