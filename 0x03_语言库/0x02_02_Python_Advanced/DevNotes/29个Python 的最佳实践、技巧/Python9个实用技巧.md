Python
<a name="SpCzl"></a>
### 整理字符串输入
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
在本例中，可以看到空格符「 n」和「 t」都被替换成了单个空格，「 r」都被删掉了。这只是个很简单的例子，可以更进一步，使用「`unicodedata`」程序包生成大型重映射表，并使用其中的「`combining()`」进行生成和映射。
<a name="itcoH"></a>
### 迭代器切片（Slice）
如果对迭代器进行切片操作，会返回一个「TypeError」，提示生成器对象没有下标，但是可以用一个简单的方案来解决这个问题：
```python
import itertools  
s = itertools.islice(range(50), 10, 20)  # &lt;itertools.islice object at 0x7f70fab88138&gt;  
for val in s:  
    ... 
```
可以使用「`itertools.islice`」创建一个「`islice`」对象，该对象是一个迭代器，可以产生想要的项。但需要注意的是，该操作要使用切片之前的所有生成器项，以及「`islice`」对象中的所有项。
<a name="DYdbO"></a>
### 跳过可迭代对象的开头
有时要处理一些以不需要的行（如注释）开头的文件。「`itertools`」再次提供了一种简单的解决方案：
```python
string_from_file = """
// Author: ...
// License: ...
//
// Date: ...
Actual content...
"""

import itertools

for line in itertools.dropwhile(lambda line: line.startswith("//"), string_from_file.split("\n")):
    print(line)
```
这段代码只打印初始注释部分之后的内容。如果只想舍弃可迭代对象的开头部分（本示例中为开头的注释行），而又不知道要这部分有多长时，这种方法就很有用了。
<a name="SF7GK"></a>
### 只包含关键字参数的函数 (`kwargs`)
当使用下面的函数时，创建仅仅需要关键字参数作为输入的函数来提供更清晰的函数定义，会很有帮助：
```python
def test(*, a, b):  
    pass  
test("value for a", "value for b")  # TypeError: test() takes 0 positional arguments...  
test(a="value", b="value 2")  # Works... 
```
在关键字参数之前加上一个「」就可以解决这个问题。如果将某些参数放在「」参数之前，它们显然是位置参数。
<a name="icmva"></a>
### 创建支持「`with`」语句的对象
举例而言，都知道如何使用「`with`」语句打开文件或获取锁，但是可以实现自己上下文表达式吗？是的，可以使用「`enter`」和「`exit`」来实现上下文管理协议:
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
上面这段代码使用 `contextmanager` 的 `manager` 装饰器实现了内容管理协议。在进入 `with` 块时 tag 函数的第一部分（在 `yield` 之前的部分）就已经执行了，然后 `with` 块才被执行，最后执行 tag 函数的其余部分。
<a name="u4AXa"></a>
### 用「`slots`」节省内存
如果曾经编写过一个创建了某种类的大量实例的程序，那么可能已经注意到，程序突然需要大量的内存。那是因为 Python 使用字典来表示类实例的属性，这使其速度很快，但内存使用效率却不是很高。通常情况下，这并不是一个严重的问题。但是，如果程序因此受到严重的影响，不妨试一下「`slots`」：
```python
class Person:  
    __slots__ = ["first_name", "last_name", "phone"]  
    def __init__(self, first_name, last_name, phone):  
        self.first_name = first_name  
        self.last_name = last_name  
        self.phone = phone 
```
当定义了「`slots`」属性时，Python 没有使用字典来表示属性，而是使用小的固定大小的数组，这大大减少了每个实例所需的内存。使用「`slots`」也有一些缺点：不能声明任何新的属性，只能使用「`slots`」上现有的属性。而且，带有「`slots`」的类不能使用多重继承。
<a name="yFHxY"></a>
### 限制「CPU」和内存使用量
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
可以看到，在上面的代码片段中，同时包含设置最大 CPU 运行时间和最大内存使用限制的选项。在限制 CPU 的运行时间时，首先获得该特定资源（RLIMIT_CPU）的软限制和硬限制，然后使用通过参数指定的秒数和先前检索到的硬限制来进行设置。最后，如果 CPU 的运行时间超过了限制，将发出系统退出的信号。在内存使用方面，再次检索软限制和硬限制，并使用带「`size`」参数的「`setrlimit`」和先前检索到的硬限制来设置它。
<a name="vURsj"></a>
### 控制可以/不可以导入什么
有些语言有非常明显的机制来导出成员（变量、方法、接口），例如在 Golang 中只有以大写字母开头的成员被导出。然而，在 Python 中，所有成员都会被导出（除非使用了「`all`」）：
```python
def foo():  
    pass  
def bar():  
    pass  
__all__ = ["bar"]
```
在上面这段代码中，只有「bar」函数被导出了。同样，可以让「`all`」为空，这样就不会导出任何东西，当从这个模块导入的时候，会造成「AttributeError」。
<a name="wtoP4"></a>
### 实现比较运算符的简单方法
为一个类实现所有的比较运算符（如 lt , le , gt , ge）是很繁琐的。有更简单的方法可以做到这一点吗？这种时候，「`functools.total_ordering`」就是一个很好的帮手：
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
这里的工作原理究竟是怎样的呢？用「`total_ordering`」装饰器简化实现对类实例排序的过程。只需要定义「`lt`」和「`eq`」就可以了，它们是实现其余操作所需要的最小的操作集合（这里也体现了装饰器的作用——填补空白）。
