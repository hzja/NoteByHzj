Python
<a name="AXHNx"></a>
## 实用技巧
<a name="b2eBn"></a>
### all or any
Python 语言如此流行的众多原因之一，是因为它具有很好的可读性和表现力。
```python
x = [True, True, False]
if any(x):
    print("至少有一个True")
if all(x):
    print("全是True")
if any(x) and not all(x):
    print("至少一个True和一个False")
```
<a name="HeNib"></a>
### `collections`
Python 有一些很棒的默认数据类型，但有时它们的行为并不完全符合期望。<br />幸运的是，Python 标准库提供了 [collections 模块](https://docs.python.org/3/library/collections.html)。这个方便的附加组件提供了更多的数据类型。
```python
from collections import OrderedDict, Counter
# 记住键的添加顺序！
x = OrderedDict(a=1, b=2, c=3)
# 统计每个字符出现的频率
y = Counter("Hello World!")
```
<a name="QevKa"></a>
### `dir`
有没有想过如何查看 Python 对象内部并查看它具有哪些属性？在命令行中输入：
```python
dir() 
dir("Hello World") 
dir(dir)
```
当以交互方式运行 Python 以及动态探索正在使用的对象和模块时，这可能是一个非常有用的功能。在这里阅读更多[functions](https://docs.python.org/3/library/functions.html#dir)相关内容。
<a name="xJbQw"></a>
### `from __future__ import`
Python 流行的结果之一，总是有新版本正在开发中。新版本意味着新功能 —— 除非版本已过时。<br />不过不要担心。使用该[__future__模块](https://docs.python.org/2/library/future.html)可以用Python的未来版本导入功能。从字面上看，这就像时间旅行、魔法或其他东西。
```python
from __future__ import print_function
print("Hello World!")
```
<a name="kiydt"></a>
### `inspect`
Python 的[inspect模块](https://docs.python.org/3/library/inspect.html)非常适合了解幕后发生的事情。甚至可以调用它自己的方法！<br />下面的代码示例`inspect.getsource()` 用于打印自己的源代码。 `inspect.getmodule()` 还用于打印定义它的模块。<br />最后一行代码打印出它自己的行号。
```python
import inspect
print(inspect.getsource(inspect.getsource))
print(inspect.getmodule(inspect.getmodule))
print(inspect.currentframe().f_lineno)
```
当然，除了这些微不足道的用途，`inspect` 模块可以证明对理解自己的代码在做什么很有用。还可以使用它来编写自文档化代码。
<a name="j96x7"></a>
### `**kwargs`
在学习任何语言时，都会有许多里程碑。使用 Python 并理解神秘的`**kwargs`语法可能算作一个重要的里程碑。<br />字典对象前面的双星号[**kwargs](https://docs.python.org/3/tutorial/controlflow.html#keyword-arguments)允许将该字典的内容作为命名参数传递给函数。<br />字典的键是参数名称，值是传递给函数的值。甚至不需要调用它kwargs！
```python
dictionary = {"a": 1, "b": 2}
def someFunction(a, b):
    print(a + b)
    return
# 这些做同样的事情:
someFunction(**dictionary)
someFunction(a=1, b=2)
```
当编写可以处理未预先定义的命名参数的函数时，这很有用。
<a name="cYfJe"></a>
### 列表(list)推导式
关于 Python 编程，最喜欢的事情之一是它的[列表推导式](https://docs.python.org/3/tutorial/datastructures.html#list-comprehensions)。<br />这些表达式可以很容易地编写非常顺畅的代码，几乎与自然语言一样。
```python
In [49]: num_list = [num for num in range(0, 10)]

In [50]: num_list
Out[50]: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```
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
<a name="LzU8B"></a>
### map
Python 通过许多内置功能支持函数式编程。最有用的`map()`功能之一是函数——尤其是与[lambda 函数](https://docs.python.org/3/tutorial/controlflow.html#lambda-expressions)结合使用时。
```python
x = [1, 2, 3] 
y = map(lambda x : x + 1, x)
# 打印出 [2,3,4]
print(list(y))
```
在上面的示例中，`map()`将一个简单的 lambda 函数应用于x. 它返回一个映射对象，该对象可以转换为一些可迭代对象，例如列表或元组。
<a name="NfBN4"></a>
### Operator overloading
Python 提供对[运算符重载](https://docs.python.org/3/reference/datamodel.html#special-method-names)的支持，这是听起来像一个合法的计算机科学家的术语之一。<br />这实际上是一个简单的概念。有没有想过为什么 Python 允许使用+运算符来添加数字以及连接字符串？这就是操作符重载的作用。<br />可以定义以自己的特定方式使用 Python 的标准运算符符号的对象。并且可以在与正在使用的对象相关的上下文中使用它们。
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
<a name="zH3Q1"></a>
### pprint
Python 的默认`print`函数完成了它的工作。但是如果尝试使用`print`函数打印出任何大的嵌套对象，其结果相当难看。这个标准库的漂亮打印模块[pprint](https://docs.python.org/3/library/pprint.html)可以以易于阅读的格式打印出复杂的结构化对象。<br />这算是任何使用非平凡数据结构的 Python 开发人员的必备品。
```python
import requests
import pprint
url = 'https://randomuser.me/api/?results=1'
users = requests.get(url).json()
pprint.pprint(users)
```
<a name="eSfWO"></a>
### Queue
Python 标准库的 Queue 模块实现支持多线程。这个模块可以实现队列数据结构。这些是允许根据特定规则添加和检索条目的数据结构。<br />“先进先出”（FIFO）队列可以按添加顺序检索对象。“后进先出”(LIFO) 队列可以首先访问最近添加的对象。<br />最后，优先队列可以根据对象的排序顺序检索对象。
<a name="Tt0zV"></a>
### `__repr__`
在 Python 中定义类或对象时，提供一种将该对象表示为字符串的“官方”方式很有用。例如：
```python
>>> file = open('file.txt', 'r') 
>>> print(file) 
<open file 'file.txt', mode 'r' at 0x10d30aaf0>
```
这使得调试代码更加容易。将其添加到类定义中，如下所示：
```python
class someClass: 
    def __repr__(self): 
        return "<some description here>"
someInstance = someClass()
# 打印 <some description here>
print(someInstance)
```
<a name="ExR1t"></a>
### Type hints
Python 是一种动态类型语言。定义变量、函数、类等时不需要指定数据类型。这允许快速的开发时间。但是，没有什么比由简单的输入问题引起的运行时错误更烦人的了。<br />从 [Python 3.5](https://docs.python.org/3/library/typing.html) 开始，可以选择在定义函数时提供类型提示。
```python
def addTwo(x : Int) -> Int:
    return x + 2
```
还可以定义类型别名。
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
尽管不是强制性的，但类型注释可以使代码更易于理解。<br />它们还允许使用类型检查工具，在运行前捕获那些杂散的 TypeError。如果正在处理大型、复杂的项目，这是很有用的！
<a name="TYIGT"></a>
### uuid
通过Python 标准库的 [uuid 模块](https://docs.python.org/3/library/uuid.html)生成通用唯一 ID（或“UUID”）的一种快速简便的方法。
```python
import uuid
user_id = uuid.uuid4()
print(user_id)
```
这将创建一个随机的 128 位数字，该数字几乎肯定是唯一的。事实上，可以生成超过 2¹²² 种可能的 UUID。这超过了五个十进制 （或 5,000,000,000,000,000,000,000,000,000,000,000,000）。<br />在给定的集合中发现重复的概率极低。即使有一万亿个 UUID，重复存在的可能性也远低于十亿分之一。
<a name="LiQht"></a>
## 实用库
<a name="CJg5y"></a>
### bashplotlib
有没有想过在控制台中绘制图形吗？<br />Bashplotlib 是一个 Python 库，他能够在命令行(粗旷的环境)中绘制数据。
```bash
# 模块安装
pip install bashplotlib
```
```python
# 绘制实例
import numpy as np
from bashplotlib.histpgram import plot_hist
arr = np.ramdom.normal(size=1000, loc=0, scale=1)
plot_hist(arr, bincount=50)
```
```python
In [15]: import numpy as np

In [16]: from bashplotlib.histogram import plot_hist

In [17]: arr = np.random.normal(size=1000, loc=0, scale=1)

In [18]: plot_hist(arr, bincount=50)

 58|                          o
 55|                          o
 52|                          o
 49|                          oo
 46|                        o oo o o o
 43|                        o oooo o o
 40|                        oooooooo o  o
 37|                      o oooooooooo  o
 34|                      o oooooooooo  oo
 31|                      ooooooooooooooooo
 28|                    ooooooooooooooooooo
 25|                    oooooooooooooooooooo
 22|                    oooooooooooooooooooo
 19|                 o  ooooooooooooooooooooo
 16|                 o oooooooooooooooooooooo o
 13|                ooooooooooooooooooooooooo oo
 10|                oooooooooooooooooooooooooooo  o
  7|           oo ooooooooooooooooooooooooooooooo o
  4|         o ooooooooooooooooooooooooooooooooooooo  o
  1| o  o oooo oooooooooooooooooooooooooooooooooooooooo
    --------------------------------------------------
```
<a name="RGaOk"></a>
### emoji
[emoji](https://pypi.org/project/emoji/) 是日本在无线通信中所使用的视觉情感符号，绘指图画，文字指的则是字符，可用来代表多种表情，如笑脸表示笑、蛋糕表示食物等。在中国大陆，emoji通常叫做“小黄脸”，或者直称emoji。
```bash
# 安装模块
pip install emoji
```
```python
# 做个尝试
from emoji import emojize
print(emojize(":thumbs_up:"))

👍
```
<a name="DR8j7"></a>
### geogy
地理，对大多数程序员来说是一个具有挑战性的领域。在获取地理信息或者绘制地图时，也会遇到不少问题。这个[geopy 模块](https://geopy.readthedocs.io/en/latest/)让地理相关内容变得非常容易。
```bash
pip install geopy
```
它通过抽象一系列不同地理编码服务的 API 来工作。通过它，能够获得一个地方的完整街道地址、纬度、经度甚至海拔高度。<br />还有一个有用的距离类。它以偏好的测量单位计算两个位置之间的距离。
```python
from geopy import GoogleV3
place = "221b Baker Street, London"
location = GoogleV3().geocode(place)
print(location.address)
print(location.location)
```
<a name="ebA9k"></a>
### howdoi
当使用terminal终端编程时，通过在遇到问题后会在StackOverflow上搜索答案，完后会回到终端继续编程，此时有时会不记得之前查到的解决方案，此时需要重新查看StackOverflow，但又不想离开终端，那么此时需要用到这个有用的命令行工具[howdoi](https://github.com/gleitz/howdoi)。
```bash
pip install howdoi
```
无论有什么问题，都可以问它，它会尽力回复。
```bash
howdoi vertical align css
howdoi for loop in java
howdoi undo commits in git
```
但请注意——它会从 StackOverflow 的最佳答案中抓取代码。它可能并不总是提供最有用的信息......
```bash
howdoi exit vim
```
<a name="g0KxW"></a>
### Jedi
Jedi 库是一个自动完成和代码分析库。它使编写代码更快、更高效。<br />除非正在开发自己的 IDE，否则可能对使用[Jedi](https://jedi.readthedocs.io/en/latest/docs/usage.html)作为编辑器插件比较感兴趣。幸运的是，这已经有可用的负载！
<a name="nWf9J"></a>
### newspaper3k
如果还没有看过它，那么准备好被[Python newspaper module](https://pypi.org/project/newspaper3k/)模块震撼到。它可以从一系列领先的国际出版物中检索新闻文章和相关的元数据。可以检索图像、文本和作者姓名。它甚至有一些[内置的 NLP 功能](https://newspaper.readthedocs.io/en/latest/user_guide/quickstart.html#performing-nlp-on-an-article)。<br />因此，如果正在考虑在下一个项目中使用 BeautifulSoup 或其他一些 DIY 网页抓取库，使用本模块可以节省不少时间和精力。
```bash
pip install newspaper3k
```
<a name="f7hW0"></a>
### sh
Python 是一种很棒的脚本语言。有时使用标准的 os 和 subprocess 库可能有点头疼。<br />该[SH库](http://amoffat.github.io/sh/)可以像调用普通函数一样调用任何程序——对于自动化工作流和任务非常有用。
```python
import sh
sh.pwd()
sh.mkdir('new_folder')
sh.touch('new_file.txt')
sh.whoami()
sh.echo('This is great!')
```
<a name="TdU7S"></a>
### Virtual environments
可能同时在多个 Python 项目上工作。不幸的是，有时两个项目将依赖于相同依赖项的不同版本。<br />幸运的是，Python支持对 [虚拟环境](https://docs.python.org/3/tutorial/venv.html) 可以两全其美。从命令行：
```bash
python -m venv my-project 
source my-project/bin/activate 
pip install all-the-modules
```
现在，可以在同一台机器上运行 Python 的独立版本和安装。
<a name="R58uB"></a>
### wikipedia
维基百科有一个很棒的 API，它允许用户以编程方式访问无与伦比的完全免费的知识和信息。在[wikipedia模块](https://wikipedia.readthedocs.io/en/latest/quickstart.html)使访问该API非常方便。
```python
import wikipedia
result = wikipedia.page('freeCodeCamp')
print(result.summary)
for link in result.links:
    print(link)
```
和真实站点一样，该模块提供了多语言支持、页面消歧、随机页面检索，甚至还有一个`donate()`方法。
<a name="hJX9f"></a>
### xkcd
幽默是 Python 语言的一个关键特征，它是以英国喜剧小品剧[Python飞行马戏团](https://en.wikipedia.org/wiki/Monty_Python's_Flying_Circus)命名的。Python 的许多官方文档都引用了该节目最著名的草图。不过，Python 的幽默并不仅限于文档。试试运行下面的行：
```python
import antigravity
```
<a name="wYjhg"></a>
### YAML
[YAML](http://yaml.org/)指的是 “ 非标记语言” 。它是一种数据格式化语言，是 JSON 的超集。<br />与 JSON 不同，它可以存储更复杂的对象并引用它自己的元素。还可以编写注释，使其特别适合编写配置文件。该[PyYAML模块](https://pyyaml.org/wiki/PyYAMLDocumentation)可在Python中使用YAML。<br />安装并然后导入到项目中：
```bash
pip install pyyaml
```
```python
import yaml
```
PyYAML 允许存储任何数据类型的 Python 对象，以及任何用户定义类的实例。
<a name="kpvy3"></a>
### zip
遇到过需要从两个列表中形成字典吗？
```python
keys = ['a', 'b', 'c']
vals = [1, 2, 3]
zipped = dict(zip(keys, vals))
```
该`zip()`内置函数需要一系列可迭代的对象，并返回一个元组列表中。每个元组按位置索引对输入对象的元素进行分组。<br />还可以通过调用对象来“解压缩”对象*`zip()`。<br />如果想了解更多的python模块，可以参考[awesome-python](https://awesome-python.com/)。
<a name="SiNRs"></a>
## 参考资料
collections 模块: [https://docs.python.org/3/library/collections.html](https://docs.python.org/3/library/collections.html)<br />functions: [https://docs.python.org/3/library/functions.html#dir](https://docs.python.org/3/library/functions.html#dir)<br />emoji: [https://pypi.org/project/emoji/](https://pypi.org/project/emoji/)<br />__future__模块: [https://docs.python.org/2/library/future.html](https://docs.python.org/2/library/future.html)<br />geopy 模块: [https://geopy.readthedocs.io/en/latest/](https://geopy.readthedocs.io/en/latest/)<br />howdoi: [https://github.com/gleitz/howdoi](https://github.com/gleitz/howdoi)<br />inspect模块: [https://docs.python.org/3/library/inspect.html](https://docs.python.org/3/library/inspect.html)<br />Jedi: [https://jedi.readthedocs.io/en/latest/docs/usage.html](https://jedi.readthedocs.io/en/latest/docs/usage.html)<br />**kwargs: [https://docs.python.org/3/tutorial/controlflow.html#keyword-arguments](https://docs.python.org/3/tutorial/controlflow.html#keyword-arguments)<br />列表推导式: [https://docs.python.org/3/tutorial/datastructures.html#list-comprehensions](https://docs.python.org/3/tutorial/datastructures.html#list-comprehensions)<br />lambda 函数: [https://docs.python.org/3/tutorial/controlflow.html#lambda-expressions](https://docs.python.org/3/tutorial/controlflow.html#lambda-expressions)<br />Python newspaper module : [https://pypi.org/project/newspaper3k/](https://pypi.org/project/newspaper3k/)<br />内置的 NLP 功能: [https://newspaper.readthedocs.io/en/latest/user_guide/quickstart.html#performing-nlp-on-an-article](https://newspaper.readthedocs.io/en/latest/user_guide/quickstart.html#performing-nlp-on-an-article)<br />运算符重载的: [https://docs.python.org/3/reference/datamodel.html#special-method-names](https://docs.python.org/3/reference/datamodel.html#special-method-names)<br />pprint: [https://docs.python.org/3/library/pprint.html](https://docs.python.org/3/library/pprint.html)<br />Queue: [https://www.tutorialspoint.com/python3/python_multithreading.htm](https://www.tutorialspoint.com/python3/python_multithreading.htm)<br />SH库: [http://amoffat.github.io/sh/](http://amoffat.github.io/sh/)<br />Python 3.5: [https://docs.python.org/3/library/typing.html](https://docs.python.org/3/library/typing.html)<br />uuid 模块: [https://docs.python.org/3/library/uuid.html](https://docs.python.org/3/library/uuid.html)<br />虚拟环境: [https://docs.python.org/3/tutorial/venv.html](https://docs.python.org/3/tutorial/venv.html)<br />wikipedia模块: [https://wikipedia.readthedocs.io/en/latest/quickstart.html](https://wikipedia.readthedocs.io/en/latest/quickstart.html)<br />Python飞行马戏团: [https://en.wikipedia.org/wiki/Monty_Python's_Flying_Circus](https://en.wikipedia.org/wiki/Monty_Python's_Flying_Circus)<br />YAML: [http://yaml.org/](http://yaml.org/)<br />PyYAML模块: [https://pyyaml.org/wiki/PyYAMLDocumentation](https://pyyaml.org/wiki/PyYAMLDocumentation)<br />awesome-python: [https://awesome-python.com/](https://awesome-python.com/)
