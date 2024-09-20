Python<br />虽然许多数据工作者认为 Python 是一种有效的编程语言，但纯 Python 程序比C、Rust 和 Java 等编译语言中的对应程序运行得更慢，为了更好地监控和优化Python程序，将和大家一起学习如何使用 Python 计时器来监控程序运行的速度，以便正对性改善代码性能。<br />为了更好地掌握 Python 计时器的应用，后面还补充了有关**Python类、上下文管理器和装饰器**的背景知识。因篇幅限制，其中利用上下文管理器和装饰器优化 Python 计时器，将在后续文章学习，不在本篇文章范围内。
<a name="YhIfv"></a>
## **Python 计时器**
首先，向某段代码中添加一个**Python 计时器**以监控其性能。
<a name="sLWKe"></a>
### Python 定时器函数
Python 中的内置[**time**](https://docs.python.org/3/library/time.html)模块中有几个可以测量时间的函数：

- `**monotonic()**`
- `**perf_counter()**`
- `**process_time()**`
- `**time()**`

Python 3.7 引入了几个新函数，如[**thread_time()**](https://docs.python.org/3/library/time.html#time.thread_time)，以及上述所有函数的**纳秒**版本，以_ns后缀命名。例如，`perf_counter_ns()`是`perf_counter()`的纳秒版本的。<br />`perf_counter()`<br />返回性能计数器的值（以秒为单位），即具有最高可用分辨率的时钟以测量短持续时间。<br />首先，使用`perf_counter()`创建一个 Python 计时器。将把它与其他 Python 计时器函数进行比较，看看 `perf_counter()` 的优势。
<a name="adjYS"></a>
#### **示例**
创建一个脚本，定义一个简短的函数：从清华云上下载一组数据。
```python
import requests
def main():
    source_url = 'https://cloud.tsinghua.edu.cn/d/e1ccfff39ad541908bae/files/?p=%2Fall_six_datasets.zip&dl=1'
    headers = {'User-Agent': 'Mozilla/5.0'}
    res = requests.get(source_url, headers=headers)
    with open('dataset/datasets.zip', 'wb') as f:
        f.write(res.content)
       
if __name__=="__main__":
    main()
```
可以使用 Python 计时器来监控该脚本的性能。
<a name="MRiqX"></a>
### 第一个 Python 计时器
现在使用函数`time.perf_counter()`函数创建一个计时器，这是一个非常适合针对部分代码的性能计时的计数器。<br />`perf_counter()`从某个未指定的时刻开始测量时间（以秒为单位），这意味着对该函数的单个调用的返回值没有用。但当查看对`perf_counter()`两次调用之间的差异时，可以计算出两次调用之间经过了多少秒。
```python
>>> import time
>>> time.perf_counter()
394.540232282

>>> time.perf_counter()  # 几秒钟后
413.31714087
```
在此示例中，两次调用 `perf_counter()` 相隔近 19 秒。可以通过计算两个输出之间的差异来确认这一点：**413.31714087 - 394.540232282 = 18.78**。<br />现在可以将 Python 计时器添加到示例代码中：
```python
# download_data.py
import requests
import time
def main():
    tic = time.perf_counter()
    source_url = 'https://cloud.tsinghua.edu.cn/d/e1ccfff39ad541908bae/files/?p=%2Fall_six_datasets.zip&dl=1'
    headers = {'User-Agent': 'Mozilla/5.0'}
    res = requests.get(source_url, headers=headers)
    with open('dataset/datasets.zip', 'wb') as f:
        f.write(res.content)
    toc = time.perf_counter()
    print(f"该程序耗时: {toc - tic:0.4f} seconds")
if __name__=="__main__":
    main()
```
**注意**`**perf_counter()**`**通过计算两次调用之间的差异来打印整个程序运行所花费的时间。**<br />`print()`函数中 f 字符串前面的表示这是一个 **f-string** ，这是格式化文本字符串的较为便捷的方式。:0.4f是一个格式说明符，表示数字，toc - tic应打印为带有四位小数的十进制数。<br />运行程序可以看到程序经过的时间：
```bash
该程序耗时: 0.026 seconds
```
就是这么简单。接下来一起学习**如何将 Python 计时器包装到一个类**、一个上下文管理器和一个装饰器中（该系列后续两篇文章，待更新），这样可以更加一致和方便使用计时器。
<a name="VS17N"></a>
## **一个 Python 定时器类**
这里至少需要一个变量来存储 Python 计时器的状态。接下来创建一个与手动调用 `perf_counter()` 相同的类，但更具可读性和一致性。<br />创建和更新`Timer`类，使用该类以多种不同方式对代码进行计时。
```bash
$ python -m pip install codetiming
```
<a name="yzNZ4"></a>
### 理解 Python 中的类
**Class类**是面向对象编程的主要构建块。类本质上是一个模板，可以使用它来创建**对象**。<br />在 Python 中，当需要对需要跟踪特定状态的事物进行建模时，类非常有用。一般来说，类是属性的集合，称为**属性**，以及行为，称为**方法**。
<a name="TpbkA"></a>
### 创建 Python 计时器类
类有利于跟踪**状态**。在`Timer`类中，想要跟踪计时器何时开始以及已经多少时间。对于Timer类的第一个实现，将添加一个`._start_time`属性以及`.start()`和`.stop()`方法。将以下代码添加到名为 timer.py 的文件中：
```python
# timer.py
import time
class TimerError(Exception):
    """一个自定义异常，用于报告使用Timer类时的错误"""

class Timer:
    def __init__(self):
        self._start_time = None

    def start(self):
        """Start a new timer"""
        if self._start_time is not None:
            raise TimerError(f"Timer is running. Use .stop() to stop it")
        self._start_time = time.perf_counter()

    def stop(self):
        """Stop the timer, and report the elapsed time"""
        if self._start_time is None:
            raise TimerError(f"Timer is not running. Use .start() to start it")

        elapsed_time = time.perf_counter() - self._start_time
        self._start_time = None
        print(f"Elapsed time: {elapsed_time:0.4f} seconds")
```
这里需要花点时间仔细地浏览代码，会发现一些不同的事情。<br />首先定义了一个TimerError Python 类。该(Exception)符号表示TimerError **继承**自另一个名为Exception的父类。使用这个内置类进行错误处理。不需要向TimerError添加任何属性或方法，但自定义错误可以更灵活地处理`Timer`内部问题。<br />接下来自定义`Timer`类。当从一个类创建或**实例化**一个对象时，代码会调用特殊方法`.__init__()`初始化实例。在这里定义的第一个`Timer`版本中，只需初始化._start_time属性，将用它来跟踪 Python 计时器的状态，计时器未运行时它的值为`None`。计时器运行后，用它来跟踪计时器的启动时间。<br />**注意：** `._start_time`的第一个下划线(`_`)前缀是Python约定。它表示`._start_time`是`Timer`类的用户不应该操作的内部属性。<br />当调用`.start()`启动新的 Python 计时器时，首先检查计时器是否运行。然后将`perf_counter()`的当前值存储在`._start_time`中。<br />另一方面，当调用`.stop()`时，首先检查Python计时器是否正在运行。如果是，则将运行时间计算为`perf_counter()`的当前值与存储在`._start_time`中的值的差值。最后，重置`._start_time`，以便重新启动计时器，并打印运行时间。<br />以下是使用Timer方法：
```python
from timer import Timer
t = Timer()
t.start()
# 几秒钟后
t.stop()
```
```python
Elapsed time: 3.8191 seconds
```
将此示例与前面直接使用`perf_counter()`的示例进行比较。代码的结构相似，但现在代码更清晰了，这也是使用类的好处之一。通过仔细选择类、方法和属性名称，可以使代码非常具有描述性!
<a name="mNZWf"></a>
### 使用 Python 计时器类
现在`Timer`类中写入download_data.py。只需要对以前的代码进行一些更改：
```python
# download_data.py
import requests
from timer import Timer
def main():
    t = Timer()
    t.start()
    source_url = 'https://cloud.tsinghua.edu.cn/d/e1ccfff39ad541908bae/files/?p=%2Fall_six_datasets.zip&dl=1'
    headers = {'User-Agent': 'Mozilla/5.0'}
    res = requests.get(source_url, headers=headers)
    with open('dataset/datasets.zip', 'wb') as f:
        f.write(res.content)
    t.stop()
if __name__=="__main__":
    main()
```
注意，该代码与之前使用的代码非常相似。除了使代码更具可读性之外，Timer还负责将经过的时间打印到控制台，使得所用时间的记录更加一致。运行代码时，得到的输出几乎相同：
```python
Elapsed time: 0.502 seconds
...
```
打印经过的时间Timer可能是一致的，但这种方法好像不是很灵活。下面添加一些更加灵活的东西到代码中。
<a name="DlwbT"></a>
### 增加更多的便利性和灵活性
到目前为止，已经了解到类适用于想要封装状态并确保代码一致性的情况。在这里将一起给 Python 计时器加入更多便利性和灵活性，那**怎么做呢？**

- 在报告消耗的时间时，使用**可调整的文本和格式**
- 将**日志记录**打印到控制台、写入到日志文件或程序的其他部分
- 创建一个可以在多次调用中**可积累**的Python计时器
- 构建 Python 计时器的**信息表示**

首先，自定义用于报告所用时间的文本。在前面的代码中，文本 `f"Elapsed time: {elapsed_time:0.4f} seconds"` 被生硬编码到 `.stop()` 中。如若想使得类代码更加灵活， **可以使用实例变量**，其值通常作为参数传递给`.__init__()`并存储到 self 属性。为方便起见，还可以提供合理的默认值。<br />要添加`.text`为Timer实例变量，可执行以下操作timer.py：
```python
# timer.py
def __init__(self, text="Elapsed time: {:0.4f} seconds"):
    self._start_time = None
    self.text = text
```
注意，默认文本`"Elapsed time: {:0.4f} seconds"`是作为一个常规字符串给出的，而不是f-string。这里不能使用f-string，因为f-string会立即计算，当实例化Timer时，代码还没有计算出消耗的时间。<br />**注意：** 如果要使用f-string来指定`.text`，则需要使用双花括号来转义实际经过时间将替换的花括号。<br />如：`f"Finished {task} in {{:0.4f}} seconds"`。如果task的值是"reading"，那么这个f-string将被计算为"Finished reading in {:0.4f} seconds"。<br />在`.stop()`中，`.text`用作模板并使用`.format()`方法填充模板：
```python
# timer.py
def stop(self):
    """Stop the timer, and report the elapsed time"""
    if self._start_time is None:
        raise TimerError(f"Timer is not running. Use .start() to start it")

    elapsed_time = time.perf_counter() - self._start_time
    self._start_time = None
    print(self.text.format(elapsed_time))
```
在此更新为timer.py之后，可以将文本更改如下：
```python
from timer import Timer
t = Timer(text="You waited {:.1f} seconds")
t.start()
# 几秒钟后
t.stop()
```
```python
You waited 4.1 seconds
```
接下来，不只是想将消息打印到控制台，还想保存时间测量结果，这样可以便于将它们存储在数据库中。可以通过从`.stop()`返回`elapsed_time`的值来实现这一点。然后，调用代码可以选择忽略该返回值或保存它以供以后处理。<br />如果想要将Timer集成到日志logging中。要支持计时器的日志记录或其他输出，需要更改对`print()`的调用，以便用户可以提供自己的日志记录函数。这可以用类似于之前定制的文本来完成:
```python
# timer.py
# ...
class Timer:
    def __init__(
        self,
        text="Elapsed time: {:0.4f} seconds",
        logger=print
    ):
        self._start_time = None
        self.text = text
        self.logger = logger
    # 其他方法保持不变
    def stop(self):
        """Stop the timer, and report the elapsed time"""
        if self._start_time is None:
            raise TimerError(f"Timer is not running. Use .start() to start it")
        elapsed_time = time.perf_counter() - self._start_time
        self._start_time = None
        if self.logger:
            self.logger(self.text.format(elapsed_time))

        return elapsed_time
```
不是直接使用`print()`，而是创建另一个实例变量 `self.logger`，引用一个接受字符串作为参数的函数。除此之外，还可以对文件对象使用`logging.info()`或`.write()`等函数。还要注意if中，它允许通过传递`logger=None`来完全关闭打印。<br />以下是两个示例，展示了新功能的实际应用：
```python
from timer import Timer
import logging
t = Timer(logger=logging.warning)
t.start()
# 几秒钟后
t.stop()  # A few seconds later
```
```python
WARNING:root:Elapsed time: 3.1610 seconds  
3.1609658249999484
```
```python
t = Timer(logger=None)
t.start()
# 几秒钟后
value = t.stop()
value
```
```python
4.710851433001153
```
接下来第三个改进是**积累时间度量的能力**。例如，在循环中调用一个慢速函数时，希望以命名计时器的形式添加更多的功能，并使用一个字典来跟踪代码中的每个Python计时器。<br />扩展**download_data.py**脚本。
```python
# download_data.py
import requests
from timer import Timer
def main():
    t = Timer()
    t.start()
    source_url = 'https://cloud.tsinghua.edu.cn/d/e1ccfff39ad541908bae/files/?p=%2Fall_six_datasets.zip&dl=1'
    headers = {'User-Agent': 'Mozilla/5.0'}
    for i in range(10):
        res = requests.get(source_url, headers=headers)
        with open('dataset/datasets.zip', 'wb') as f:
            f.write(res.content)
    t.stop()
if __name__=="__main__":
    main()
```
这段代码的一个微妙问题是，不仅要测量下载数据所需的时间，还要测量 Python 存储数据到磁盘所花费的时间。这可能并重要，有时候这两者所花费的时间可以忽略不计。但还是希望有一种方法可以精确地计时没一个步骤，将会更好。<br />有几种方法可以在不改变Timer当前实现的情况下解决这个问题，且只需要几行代码即可实现。<br />首先，将引入一个名为`.timers`的字典作为Timer的类变量，此时Timer的所有实例将共享它。通过在任何方法之外定义它来实现它:
```python
class Timer:
    timers = {}
```
类变量可以直接在类上访问，也可以通过类的实例访问：
```python
>>> from timer import Timer
>>> Timer.timers
{}

>>> t = Timer()
>>> t.timers
{}

>>> Timer.timers is t.timers
True
```
在这两种情况下，代码都返回相同的空类字典。<br />接下来向 Python 计时器添加可选名称。可以将该名称用于两种不同的目的：

1. 在代码中**查找**经过的时间
2. **累加**同名定时器

要向Python计时器添加名称，需要对 timer.py 进行更改。首先，Timer 接受 name 参数。第二，当计时器停止时，运行时间应该添加到 .timers 中:
```python
# timer.py
# ...
class Timer:
    timers = {}
    def __init__(
        self,
        name=None,
        text="Elapsed time: {:0.4f} seconds",
        logger=print,
    ):
        self._start_time = None
        self.name = name
        self.text = text
        self.logger = logger

        # 向计时器字典中添加新的命名计时器
        if name:
            self.timers.setdefault(name, 0)

    # 其他方法保持不变
    
    def stop(self):
        """Stop the timer, and report the elapsed time"""
        if self._start_time is None:
            raise TimerError(f"Timer is not running. Use .start() to start it")
        elapsed_time = time.perf_counter() - self._start_time
        self._start_time = None
        if self.logger:
            self.logger(self.text.format(elapsed_time))
        if self.name:
            self.timers[self.name] += elapsed_time
        return elapsed_time
```
注意，在向`.timers`中添加新的Python计时器时，使用了`.setdefault()`方法。它只在没有在字典中定义name的情况下设置值，如果name已经在`.timers`中使用，那么该值将保持不变，此时可以积累几个计时器:
```python
>>> from timer import Timer
>>> t = Timer("accumulate")
>>> t.start()

>>> t.stop()  # A few seconds later
Elapsed time: 3.7036 seconds
3.703554293999332

>>> t.start()

>>> t.stop()  # A few seconds later
Elapsed time: 2.3449 seconds
2.3448921170001995

>>> Timer.timers
{'accumulate': 6.0484464109995315}
```
现在可以重新访问download_data.py并确保仅测量下载数据所花费的时间：
```python
# download_data.py
import requests
from timer import Timer
def main():
    t = Timer("download", logger=None)
    source_url = 'https://cloud.tsinghua.edu.cn/d/e1ccfff39ad541908bae/files/?p=%2Fall_six_datasets.zip&dl=1'
    headers = {'User-Agent': 'Mozilla/5.0'}
    for i in range(10):
        t.start()
        res = requests.get(source_url, headers=headers)
        t.stop()
        with open('dataset/datasets.zip', 'wb') as f:
            f.write(res.content)
    download_time = Timer.timers["download"]
    print(f"Downloaded 10 dataset in {download_time:0.2f} seconds")
    
if __name__=="__main__":
    main()
```
现在有了一个非常简洁的版本，Timer它一致、灵活、方便且信息丰富！也可以将本节中所做的许多改进应用于项目中的其他类型的类。
<a name="ul7kF"></a>
### Timer改进
最后一个改进Timer，以交互方式使用它时使其更具信息性。下面操作是实例化一个计时器类，并查看其信息：
```python
>>> from timer import Timer
>>> t = Timer()
>>> t
<timer.Timer object at 0x7f0578804320>
```
最后一行是 Python 表示对象的默认方式。从这个结果中看到的信息，并不是很明确，接下来对其进行改进。<br />这里介绍一个 `dataclasses` 类，该类仅包含在 Python 3.7 及更高版本中。
```bash
pip install dataclasses
```
可以使用`@dataclass`装饰器将 Python 计时器转换为数据类
```python
# timer.py
import time
from dataclasses import dataclass, field
from typing import Any, ClassVar
# ...
@dataclass
class Timer:
    timers: ClassVar = {}
    name: Any = None
    text: Any = "Elapsed time: {:0.4f} seconds"
    logger: Any = print
    _start_time: Any = field(default=None, init=False, repr=False)

    def __post_init__(self):
        """Initialization: add timer to dict of timers"""
        if self.name:
            self.timers.setdefault(self.name, 0)

    # 其余代码不变
```
此代码替换了之前的 `.__init__()` 方法。请注意数据类如何使用类似于之前看到的用于定义所有变量的类变量语法的语法。事实上，`.__init__()`是根据类定义中的注释变量自动为数据类创建的。<br />如果需要注释变量以使用数据类。可以使用此注解向代码添加类型提示。如果不想使用类型提示，那么可以使用 Any 来注释所有变量。接下来很快就会学习如何将实际类型提示添加到数据类中。<br />以下是有关 Timer 数据类的一些注意事项：

- 第 6 行：`@dataclass` 装饰器将 `Timer` 定义为数据类。
- 第 8 行：数据类需要特殊的 ClassVar 注释来指定 `.timers` 是一个类变量。
- 第 9 到 11 行：`.name`、`.text` 和 `.logger` 将被定义为 Timer 上的属性，可以在创建 Timer 实例时指定其值。它们都有给定的默认值。
- 第 12 行：回想一下 `._start_time` 是一个特殊属性，用于跟踪 Python 计时器的状态，但它应该对用户隐藏。使用 `dataclasses.field()`， `._start_time` 应该从 `.__init__()` 和 Timer 的表示中删除。
- 除了设置实例属性之外，可以使用特殊的 `.__post_init__()` 方法进行初始化。这里使用它将命名的计时器添加到 `.timers`。

新 Timer 数据类与之前的常规类使用功能一样，但它现在有一个很好的**信息表示**：
```python
from timer import Timer
t = Timer()
t
```
```python
Timer(name=None, 
  text='Elapsed time: {:0.4f} seconds',
  logger=<built-in function print>)
```
```python
t.start()
# 几秒钟后
t.stop()
```
```python
Elapsed time: 6.7197 seconds
6.719705373998295
```
<a name="EN5y1"></a>
## **总结**
现在有了一个非常简洁的 Timer 版本，它一致、灵活、方便且信息丰富！还可以将本文中所做的许多改进应用于项目中的其他类型的类。<br />现在访问当前的完整源代码Timer。会注意到在代码中添加了类型提示以获取额外的文档：
```python
# timer.py

from dataclasses import dataclass, field
import time
from typing import Callable, ClassVar, Dict, Optional

class TimerError(Exception):
    """A custom exception used to report errors in use of Timer class"""

@dataclass
class Timer:
    timers: ClassVar[Dict[str, float]] = {}
    name: Optional[str] = None
    text: str = "Elapsed time: {:0.4f} seconds"
    logger: Optional[Callable[[str], None]] = print
    _start_time: Optional[float] = field(default=None, init=False, repr=False)

    def __post_init__(self) -> None:
        """Add timer to dict of timers after initialization"""
        if self.name is not None:
            self.timers.setdefault(self.name, 0)

    def start(self) -> None:
        """Start a new timer"""
        if self._start_time is not None:
            raise TimerError(f"Timer is running. Use .stop() to stop it")

        self._start_time = time.perf_counter()

    def stop(self) -> float:
        """Stop the timer, and report the elapsed time"""
        if self._start_time is None:
            raise TimerError(f"Timer is not running. Use .start() to start it")

        # Calculate elapsed time
        elapsed_time = time.perf_counter() - self._start_time
        self._start_time = None

        # Report elapsed time
        if self.logger:
            self.logger(self.text.format(elapsed_time))
        if self.name:
            self.timers[self.name] += elapsed_time

        return elapsed_time
```
**总结下：** 使用类创建 Python 计时器有几个好处：

- **可读性：**仔细选择类和方法名称，代码将更自然地阅读。
- **一致性：**将属性和行为封装到属性和方法中，代码将更易于使用。
- **灵活性：**使用具有默认值而不是硬编码值的属性，代码将是可重用的。

这个类非常灵活，几乎可以在任何需要监控代码运行时间的情况下使用它。但是，在接下来的部分中，将和大家一起了解如何使用上下文管理器和装饰器，这将更方便地对代码块和函数进行计时。
<a name="sTB17"></a>
## 参考资料
time: [_https://docs.python.org/3/library/time.html_](https://docs.python.org/3/library/time.html)<br />thread_time(): [_https://docs.python.org/3/library/time.html#time.thread_time_](https://docs.python.org/3/library/time.html#time.thread_time)
