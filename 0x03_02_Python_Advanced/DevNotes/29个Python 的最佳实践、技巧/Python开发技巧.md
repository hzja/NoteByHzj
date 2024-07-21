Python
<a name="5c8257d5ad7c1d01fd38b632188be9fe"></a>
## 1、对输入的字符串“消毒”
对用户输入的内容“消毒”，这问题几乎适用于编写的所有程序。通常将字符转换为小写或大写就足够了，有时还可以使用正则表达式来完成工作，但是对于复杂的情况，还有更好的方法：
```python
user_input = "This
string has	some whitespaces...
"
character_map = {
    ord( 
    ) :    ,
    ord( 	 ) :    ,
    ord( 
    ) : None
}
user_input.translate(character_map)  # This string has some whitespaces... "
```
在此示例中，可以看到空格字符“ ”和“ ”被单个空格替换了，而“ ”则被完全删除。这是一个简单的示例，但是可以更进一步，使用`unicodedata` 库及其 `combining()` 函数，来生成更大的重映射表（remapping table），并用它来删除字符串中所有的重音。
<a name="f928307f444d7236d32c02747bd84f83"></a>
## 2、对迭代器切片
如果尝试直接对迭代器切片，则会得到 TypeError ，提示说该对象不可取下标（not subscriptable），但是有一个简单的解决方案：
```python
import itertools
s = itertools.islice(range(50), 10, 20)  # <itertools.islice object at 0x7f70fab88138>
for val in s:
    ...
```
使用`itertools.islice`，可以创建一个 `islice` 对象，该对象是一个迭代器，可以生成所需的内容。但是这有个重要的提醒，即它会消耗掉切片前以及切片对象 `islice` 中的所有元素。
<a name="98da328de079766576d3312484892ac0"></a>
## 3、跳过可迭代对象的开始
有时候必须处理某些文件，它们以可变数量的不需要的行（例如注释）为开头。itertools 再次提供了简单的解决方案：
```python
string_from_file = """
// Author: ...
// License: ...
//
// Date: ...
Actual content...
"""
import itertools
for line in itertools.dropwhile(lambda line:line.startswith("//"), string_from_file.split("
                                                                                          ")):
                                                                                          print(line)
```
这段代码仅会打印在初始的注释部分之后的内容。如果只想丢弃迭代器的开头部分（在此例中是注释），并且不知道有多少内容，那么此方法很有用。
<a name="722d0e2aaa26a18a7611654030266139"></a>
## 4、仅支持关键字参数（kwargs）的函数
当需要函数提供（强制）更清晰的参数时，创建仅支持关键字参数的函数，可能会挺有用：
```python
def test(*, a, b):
    pass
test("value for a", "value for b")  # TypeError: test() takes 0 positional arguments...
test(a="value", b="value 2")  # Works...
```
可以在关键字参数之前，放置单个 * 参数来轻松解决此问题。如果将位置参数放在 * 参数之前，则显然也可以有位置参数。
<a name="6c5b996df48bcca83664ce19d6cf6460"></a>
## 5、创建支持 `with` 语句的对象
都知道如何使用 `with` 语句，例如打开文件或者是获取锁，但是可以实现自己的么？是的，可以使用`__enter__`和`__exit__` 方法来实现上下文管理器协议：
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
这是在 Python 中实现上下文管理的最常见方法，但是还有一种更简单的方法：
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
上面的代码段使用 `contextmanager` 装饰器实现了内容管理协议。`tag` 函数的第一部分（yield 之前）会在进入 `with` 语句时执行，然后执行 `with` 的代码块，最后会执行 tag 函数的剩余部分。
<a name="3360a5e2815255245a382c2760743140"></a>
## 5、用`__slots__`节省内存
如果曾经编写过一个程序，该程序创建了某个类的大量实例，那么可能已经注意到程序突然就需要大量内存。那是因为 Python 使用字典来表示类实例的属性，这能使其速度变快，但内存不是很高效。通常这不是个问题，但是，如果程序遇到了问题，可以尝试使用`__slots__` ：
```python
class Person:
    __slots__ = ["first_name", "last_name", "phone"]
    def __init__(self, first_name, last_name, phone):
        self.first_name = first_name
        self.last_name = last_name
        self.phone = phone
```
这里发生的是，当定义`__slots__`属性时，Python 使用固定大小的小型数组，而不是字典，这大大减少了每个实例所需的内存。使用`__slots__`还有一些缺点——无法声明任何新的属性，并且只能使用在`__slots__`中的属性。同样，带有`__slots__`的类不能使用多重继承。
<a name="dc311df59e81cec30616f511bd532ed0"></a>
## 6、限制CPU和内存使用量
如果不是想优化程序内存或 CPU 使用率，而是想直接将其限制为某个固定数字，那么 Python 也有一个库能做到：
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
在这里，可以看到两个选项，可设置最大 CPU 运行时间和内存使用上限。对于 CPU 限制，首先获取该特定资源（RLIMIT_CPU）的软限制和硬限制，然后通过参数指定的秒数和先前获取的硬限制来设置它。最后，如果超过 CPU 时间，将注册令系统退出的信号。至于内存，再次获取软限制和硬限制，并使用带有 size 参数的setrlimit 和获取的硬限制对其进行设置。
<a name="55a1c71ab1fc24d311de3b3ba040347a"></a>
## 8、控制可以import的内容
某些语言具有非常明显的用于导出成员（变量、方法、接口）的机制，例如Golang，它仅导出以大写字母开头的成员。另一方面，在 Python 中，所有内容都会被导出，除非使用`__all__ `：
```python
def foo():
    pass
def bar():
    pass
__all__ = ["bar"]
```
使用上面的代码段，可以限制`from some_module import *` 在使用时可以导入的内容。对于以上示例，通配导入时只会导入 `bar`。此外，可以将`__all__`设为空，令其无法导出任何东西，并且在使用通配符方式从此模块中导入时，将引发 AttributeError。
<a name="3c73e5cf38ade7a7266c7bce9bead1db"></a>
## 9、比较运算符的简便方法
为一个类实现所有比较运算符可能会很烦人，因为有很多的比较运算符——`__lt__`、`__le__`、`__gt__`或`__ge__`。但是，如果有更简单的方法呢？`functools.total_ordering` 可救场：
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
这到底如何起作用的？`total_ordering` 装饰器用于简化为类实例实现排序的过程。只需要定义`__lt__` 和`__eq__`，这是最低的要求，装饰器将映射剩余的操作——它填补了空白。
<a name="32e6b8db7d7eaf9f26c674951d6c6e77"></a>
## 10、使用`slice`函数命名切片
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
在此例中，可以避免神秘的索引，方法是先使用 slice 函数命名它们，然后再使用它们。还可以通过 `.start`、`.stop`和 `.stop` 属性，来了解 `slice` 对象的更多信息。
<a name="2711fbf8dddf7f698f91ab0b344e6be7"></a>
## 11、在运行时提示用户输入密码
许多命令行工具或脚本需要用户名和密码才能操作。因此，如果碰巧写了这样的程序，可能会发现 getpass 模块很有用：
```python
import getpass
user = getpass.getuser()
password = getpass.getpass()
# Do Stuff...
```
这个非常简单的包通过提取当前用户的登录名，可以提示用户输入密码。但是须注意，并非每个系统都支持隐藏密码。Python 会尝试警告，因此切记在命令行中阅读警告信息。
<a name="f0086ef6afd2f71cb411a48c981c8e1f"></a>
## 12、查找单词/字符串的相近匹配
现在，关于 Python 标准库中一些晦涩难懂的特性。如果发现自己需要使用Levenshtein distance之类的东西，来查找某些输入字符串的相似单词，那么 Python 的 difflib 会提供支持。
```python
import difflib
difflib.get_close_matches( appel , [ ape ,  apple ,  peach ,  puppy ], n=2)
# returns [ apple ,  ape ]
```
`difflib.get_close_matches` 会查找最佳的“足够好”的匹配。在这里，第一个参数与第二个参数匹配。还可以提供可选参数 n ，该参数指定要返回的最多匹配结果。另一个可选的关键字参数 cutoff （默认值为 0.6），可以设置字符串匹配得分的阈值。
<a name="f0fbe12153fcaf916480db8299f2d647"></a>
## 13、使用IP地址
如果必须使用 Python 做网络开发，可能会发现 ipaddress 模块非常有用。一种场景是从 CIDR（无类别域间路由 Classless Inter-Domain Routing）生成一系列 IP 地址：
```python
import ipaddress
net = ipaddress.ip_network( 74.125.227.0/29 )  # Works for IPv6 too
# IPv4Network( 74.125.227.0/29 )
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
但是请注意，ipaddress 模块和其它与网络相关的模块之间只有有限的互通性。例如，不能将 IPv4Network 实例当成地址字符串——需要先使用 str 转换它们。
<a name="64a4947a3caf83537cbfc514587325bb"></a>
## 14、在Shell中调试程序崩溃
如果拒绝使用 IDE，并在 Vim 或 Emacs 中进行编码的人，那么可能会遇到这样的情况：拥有在 IDE 中那样的调试器会很有用。<br />只要用python3.8 -i 运行程序——一旦程序终止了， -i 会启动交互式 shell，在那可以查看所有的变量和调用函数。整洁，但是使用实际的调试器（pdb ）会如何呢？用以下程序（script.py ）：
```python
def func():
    return 0 / 0
func()
```
并使用python3.8 -i script.py运行脚本：
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
大多数时候，打印语句和错误信息就足以进行调试，但是有时候，需要四处摸索，以了解程序内部正在发生的事情。在这些情况下，可以设置断点，然后程序执行时将在断点处停下，可以检查程序，例如列出函数参数、表达式求值、列出变量、或如上所示仅作单步执行。<br />pdb 是功能齐全的 Python shell，理论上可以执行任何东西，但是还需要一些调试命令，可在此处【4】找到。
<a name="8531a41d9bfc38a16d6e5b3ea715dd3c"></a>
## 15、在一个类中定义多个构造函数
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
可能倾向于将替代构造函数的所有逻辑放入`__init__`，并使用`*args` 、`**kwargs` 和一堆 if 语句，而不是使用类方法来解决。那可能行得通，但是却变得难以阅读和维护。<br />因此，建议将很少的逻辑放入`__init__`，并在单独的方法/构造函数中执行所有操作。这样，对于类的维护者和用户而言，得到的都是干净的代码。
<a name="5714295445927705a1405d27c0a15735"></a>
## 16、使用装饰器缓存函数调用
是否曾经编写过一种函数，它执行昂贵的 I/O 操作或一些相当慢的递归，而且该函数可能会受益于对其结果进行缓存（存储）？如果有，那么有简单的解决方案，即使用 functools 的lru_cache :
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
在此例中们用了可缓存的 GET 请求（最多 32 个缓存结果）。还可以看到，可以使用 `cache_info` 方法检查函数的缓存信息。装饰器还提供了 `clear_cache` 方法，用于使缓存结果无效。<br />此函数不应与具有副作用的函数一起使用，或与每次调用都创建可变对象的函数一起使用。
<a name="a561da25c730e9293f782ce546671b36"></a>
## 17、在可迭代对象中查找最频繁出现的元素
在列表中查找最常见的元素是非常常见的任务，可以使用 for 循环和字典（map），但是这没必要，因为 collections 模块中有 Counter 类：
```python
from collections import Counter
cheese = ["gouda", "brie", "feta", "cream cheese", "feta", "cheddar",
          "parmesan", "parmesan", "cheddar", "mozzarella", "cheddar", "gouda",
          "parmesan", "camembert", "emmental", "camembert", "parmesan"]
cheese_count = Counter(cheese)
print(cheese_count.most_common(3))
# Prints: [( parmesan , 4), ( cheddar , 3), ( gouda , 2)]
```
实际上，Counter 只是一个字典，将元素与出现次数映射起来，因此可以将其用作普通字典：<br />python <br />`print(cheese_count["mozzarella"])`<br />¨K40K<br />`cheese_count["mozzarella"] += 1`<br />`print(cheese_count["mozzarella"])`<br />¨K41K<br />除此之外，还可以使用 `update(more_words)` 方法轻松添加更多元素。Counter 的另一个很酷的特性是可以使用数学运算（加法和减法）来组合和减去 Counter 的实例。
