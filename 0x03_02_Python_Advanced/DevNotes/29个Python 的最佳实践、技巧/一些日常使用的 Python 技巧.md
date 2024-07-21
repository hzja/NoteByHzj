Python
<a name="NY6uz"></a>
## ALL OR ANY
Python 之所以成为这么一门受欢迎的语言一个原因是它的可读性和表达能力非常强。Python 也因此经常被调侃为“可执行的伪代码”:
```python
x = [True, True, False]
if any(x):
    print("At least one True")
if all(x):
    print("Not one False")
if any(x) and not all(x):
    print("At least one True and one False")
```
<a name="IX6C5"></a>
## BASHPLOTIB
要在控制台绘图可以安装下面的模块
```bash
$ pip install bashplotlib
```
现在，控制台中就可以有图了
<a name="ULZHv"></a>
## COLLECTIONS
Python 有一些很棒的默认数据类型，但是有时候他们并不会像希望的那样发挥作用。<br />幸运的是，Python 标准库提供了 collection 模块。它可以使用更为多样数据类型。
```python
from collections import OrderedDict, Counter
# Remembers the order the keys are added!
x = OrderedDict(a=1, b=2, c=3)
# Counts the frequency of each character
y = Counter("Hello World!")
```
<a name="bx5mm"></a>
## DIR
面对一个 Python 对象，是否曾想过可以直接看到其属性？也许可以试试以下的代码:
```python
>>> dir()
>>> dir("Hello World")
>>> dir(dir)
```
这是运行 Python 的时候一个非常有用的功能，用于动态探索使用的对象和模块。
<a name="wNPbD"></a>
## EMOJI
![](./img/1632099855849-234b06ed-01b3-4c04-8a7b-dd9847852fc0.webp)
```bash
$ pip install emoji
```
用 python 来创建表情包，也可以。
```python
from emoji import emojize
print(emojize(":thumbs_up:"))

👍
```
<a name="RmTXW"></a>
## `FROM_FUTURE_IMPORT`
Python 非常受欢迎，这也就导致了它的版本更新非常快，新的版本往往会有很多新特性。不更新，就无法使用。<br />然而，不要害怕。`future` 模块可以导入未来版本的功能。
```python
from __future__ import print_function
print("Hello World!")
```
<a name="WKcJj"></a>
## GEOPY
对于程序猿来说地理可能是一个非常有挑战性的领域。但是，geopy 模块则让它变得非常简单。
```bash
$ pip install geopy
```
它通过提取一系列不同地理编码服务的 api 来工作，能够获得一个地方的完整街道地址、纬度、经度，甚至海拔。<br />这里面同时还包含一个有用的“距离”类别。它能使用选定的度量去计算了两个地点之间的距离。
```python
from geopy import GoogleV3
place = "221b Baker Street, London"
location = GoogleV3().geocode(place)
print(location.address)
print(location.location)
```
<a name="hDjNm"></a>
## HOWDOI
有时候碰到了一个编程问题，觉得自己之前明明见过它的解决方法，但是却记不起来具体是怎么样的了。于是想要去 StackOverflow 上找，但又不想离开这个终端。这个时候，需要下面这个工具——howdoi
```bash
$ pip install howdoi
```
遇到的任何问题都可以问它，它会尽他所能给返回一个答案。
```bash
$ howdoi vertical align css
$ howdoi for loop in java
$ howdoi undo commits in git
```
需要注意的是——它只从 StackOverflow 最顶端的答案中抓取代码。所以它返回的不总是最有用的信息…
```bash
$ howdoi exit vim
```
<a name="V4MqB"></a>
## INSPECT
Python 的 inspect 模块用于收集 Python 对象的信息，可以获取类或函数的参数的信息，源码，解析堆栈等等<br />下方的代码样例使用了 `inspect.getsource()` 来打印它自身的源码。同样还使用了 `inspect.getmodule()` 来打印定义了 `inspect.getmodule()` 的模块。最后一行代码则是打印了本行代码所在的行号。在本例中，就是 4 。
```python
import inspect
print(inspect.getsource(inspect.getsource))
print(inspect.getmodule(inspect.getmodule))
print(inspect.currentframe().f_lineno)
```
`inspect` 模块可以有效地知道代码是如何工作的，也可以利用它来完成一些个人的源码。
<a name="gZ5jd"></a>
## JEDI
Jedi 库是一个代码自动补齐和静态分析的库。它可以更快更高效地书写代码。<br />除非在开发自己的编辑器，否则可能会非常喜欢将 Jedi 作为自己的编辑插件。<br />可能已经正在使用 Jedi 而只是没发现。IPython 项目就是利用 Jedi 来实现其自动补全功能。
<a name="et9C6"></a>
## `**KWARGS`
无论学习那种语言，在这条学习之路上总有那么一些里程碑。在 Python 的编程学习中，理解神秘的 `**kwargs` 语法应该算是一个重要的里程碑。<br />双星“`**`”放在字典的前面可以让将字典的内容作为命名参数传递给函数。字典的键是参数的名字，键的值作为参数的值传递给函数。如下所示:
```python
dictionary = {"a": 1, "b": 2}
def someFunction(a, b):
    print(a + b)
    return
# these do the same thing:
someFunction(**dictionary)
someFunction(a=1, b=2)
```
当想要创建一个函数，它需要能处理事先没有定义过的参数，那么就要用到前面提到的技巧了。
<a name="IS9bb"></a>
## LIST COMPREHENSIONS
List comprehensions(列表推导式)<br />这种表达式可以写出像自然语言一样易于理解并且还很简洁的代码。
```python
numbers = [1,2,3,4,5,6,7]
evens = [x for x in numbers if x % 2 is 0]
odds = [y for y in numbers if y not in evens]
cities = ['London', 'Dublin', 'Oslo']
def visit(city):
    print("Welcome to "+city)
for city in cities:
    visit(city)
```
<a name="PUULT"></a>
## MAP
Python 有许多非常有用的内置函数。其中一个就是 `map()`——特别是和 lambda 函数相结合的时候。
```python
x = [1, 2, 3]
y = map(lambda x : x + 1 , x)
# prints out [2,3,4]
print(list(y))
```
在这个例子中，`map()` 对 x 中的每一个元素都应用了一个简单的 lambda 函数。它会返回一个 map 对象，这个对象可以被转化成可迭代对象，如列表或者元组。
<a name="FzQB3"></a>
## NEWSPAPER3K
newspaper3k，如果还没有见过它，那么可能会被这个 Python newspaper 模块所惊艳到。<br />它可以检索到一系列国际领先出版物中的新闻和相关的元数据。可以检索图片、文本和作者名。它甚至有一些内置的自然语言处理功能。所以，如果正在考虑使用 BeautifulSoup 或其他自制的爬虫库来应用于下一个项目。那么可以试试 `$ pip install newspaper3k`。
<a name="qYsFi"></a>
## OPERATOR OVERLOADING(操作符重载)
Python 支持操作符重载。“操作符重载”其实是个简单的概念，是否曾经想过为什么 Python 可以使用“+”操作符来同时实现加法和连接字符串？这就是操作符重载在发挥作用。<br />可以定义使用 Python 标准操作符符号的对象，这可以在特定的环境中使用特定的对象，就像下方的例子一样。
```python
class Thing:
    def __init__(self, value):
        self.__value = value
    def __gt__(self, other):
        return self.__value > other.__value
    def __lt__(self, other):
        return self.__value < other.__value
something = Thing(100)
nothing = Thing(0)
# True
something > nothing
# False
something < nothing
# Error
something + nothing
```
<a name="BMSr3"></a>
## PPRINT
Python 的默认 print 函数可以满足日常的输出任务，但如果要打印更大的、嵌套式的对象，那么使用默认的 print 函数打印出来的内容会很丑陋。<br />这个时候就需要 pprint 了，它可以让复杂的结构型对象以可读性更强的格式显示。这对于经常要面对非普通数据结构的 Python 开发者来说是必不可少的工具。
```python
import requests
import pprint
url = 'https://randomuser.me/api/?results=1'
users = requests.get(url).json()
pprint.pprint(users)
```
<a name="xpE4K"></a>
## QUEUE(队列)
Python 支持多线程，它是通过标准库中的 Queue 模块来实现的。这个模块可以实现队列数据结构。这种数据结构可以根据特定的规则添加和检索条目。<br />“先进先出”（FIFO）队列可以按照添加对象的顺序来检索他们。“后进先出”（LIFO）队列可以首先访问最近添加的对象。最后，优先队列可以根据他们排序的顺序进行检索。
<a name="RLu2s"></a>
## `__repr__`
当定义一个类的时候，提供一个方法可以返回用来表示该类对象的可打印字符串会非常有用。例如:
```python
>>> file = open('file.txt', 'r')
>>> print(file)
<open file 'file.txt', mode 'r' at 0x10d30aaf0>

这使得 debug 更加方便，具体的定义方式如下:
class someClass:
    def __repr__(self):
        return "<some description here>"
someInstance = someClass()
# prints <some description here>
print(someInstance)
```
<a name="fNRLZ"></a>
## sh
sh 库可以像调用方法那样调用系统中的命令。
```python
import sh
sh.pwd()
sh.mkdir('new_folder')
sh.touch('new_file.txt')
sh.whoami()
sh.echo('This is great!')
```
<a name="OAgJE"></a>
## TYPE HINT(类型提示)
Python 是一种动态类型语言。当定义变量、函数、类别的时候，不需要指定数据的类型。这可以大大提升开发速度，但也是有代价的。可能会因为一个简单的输入问题而导致运行出错。<br />在 Python3.5 之后，这就不是问题了，在定义函数的时候可以自主选择要不要提供类型提示。
```python
def addTwo(x : Int) -> Int:
    return x + 2
```
还可以定义类型的别名:
```python
from typing import List
Vector = List[float]
Matrix = List[Vector]
def addMatrix(a : Matrix, b : Matrix) -> Matrix:
  result = []
  for i,row in enumerate(a):
    result_row =[]
    for j, col in enumerate(row):
      result_row += [a[i][j] + b[i][j]]
    result += [result_row]
  return result
x = [[1.0, 0.0], [0.0, 1.0]]
y = [[2.0, 1.0], [0.0, -2.0]]
z = addMatrix(x, y)
```
虽然不是强制性的，类型注释可以让代码理解起来更加简单。它们也允许使用类型检测工具在运行之前捕获这些零散的类型错误。如果正在从事大型、复杂的项目，那么类型注释也许会非常有帮助
<a name="MoMfz"></a>
## UUID
通过 Python 标准库中的 uuid 模块，可以快速并简单地生成统一的唯一ID（又称UUID）.
```python
import uuid
user_id = uuid.uuid4()
print(user_id)
```
UUID 是128位的全局唯一标识符，通常由32字节的字符串表示。它可以保证时间和空间的唯一性，也称为 GUID，全称为：UUID —— Universally Unique IDentifier，Python 中叫 UUID。它通过 MAC 地址、时间戳、命名空间、随机数、伪随机数来保证生成ID的唯一性。
<a name="DmD95"></a>
## VRITUAL ENVIRONMENTS
可能经常要处理不止一个 Python 项目，不幸的是，有时候不同项目会依赖不同的Python版本。这个时候，应该在系统里安装哪个 Python 版本呢？<br />幸运的是，Python 可以支持建立不同的虚拟环境来满足不同的版本需求。
```bash
python -m venv my-project
source my-project/bin/activate
pip install all-the-modules 
```
现在可以在一台机器上安装和运行各个独立版本的 Python。太棒了！
<a name="rY451"></a>
## WIKIPEDIA
Wikipedia 有一个很棒的 API，它可以让用户通过编程访问到维基的词条内容。使用 Python 中的 wikipedia 模块可以以最便捷的方式访问该 API。
```python
import wikipedia
result = wikipedia.page('freeCodeCamp')
print(result.summary)
for link in result.links:
    print(link)
```
与真实站点一样，该模块支持多种语言、页面消除歧义、随机页面检索，甚至还有 donate() 方法。
<a name="gNdNm"></a>
## YAML
YAML 是 “YAML 不是一种标记语言”的外语缩写。它是一个数据格式语言，是 JSON 的父集。和 JSON 不同的是，它可以存储更复杂的对象，并且可以引用自身的元素。还可以写注释，这让 YAML 特别适合于书写配置文件。<br />PyYAML 模块可以使用 Python 调用 YAML。使用下列语句安装:
```bash
$ pip install pyyaml
```
然后导入到项目中:
```python
import yaml
```
PyYAML 能够储存任何数据类型的 Python 对象，以及任何用户定义类的实例。
<a name="xPVRa"></a>
## ZIP
是否曾想要让两个列表中的元素逐个映射，组合成字典？那么应该使用 zip。
```python
keys = ['a', 'b', 'c']
vals = [1, 2, 3]
zipped = dict(zip(keys, vals))
```
内置函数 `zip()` 接收若干可迭代对象，然后返回一个由多个元组组成的列表。每个元组根据输入对象的位置索引对其元素进行分组。还可以使用 `*zip()` 来“解压”对象。
