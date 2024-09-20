Python

- Python 的标准库非常广泛，提供了各种各样的工具。该库包含内置模块（用C编写），可以访问系统功能，例如 Python 程序员无法访问的文件 I / O，以及用 Python 编写的模块，这些模块为许多问题提供标准化解决方案。其中一些模块明确地旨在通过将平台特定的内容抽象为平台中立的 API 来鼓励和增强 Python 程序的可移植性。
- Python 的标准库(standard library) 是 Python 的一个组成部分，也是 Python 的利器，它可以让编程事半功倍。
<a name="DjO4r"></a>
## 1、操作系统接口
<a name="lzuvq"></a>
### 1.1 os 模块简介
os 模块提供了很多与操作系统相关联的函数，这使得程序员们在编程的时候能利用函数灵活操作与使用，如果希望程序能够与平台无关的话，运用这个模块中的功能就尤为重要。在使用 os 模块前，需要先 `import os` 引入模块。以下方法只做介绍，具体的应用可以使用 help(os) 查看帮助文档，最重要的是实际操作。
<a name="kLuiP"></a>
### 1.1.1 操作系统相关调用和操作
```python
os.name                        获取操作系统平台
os.environ                    一个 dictionary 包含环境变量的映射关系
print(os.environ)             输出环境变量值
os.system()                   用来运行shell命令
os.chdir(dir)                 改变当前目录 
os.chdir(‘F:\WprkSpace’)      注意符号转义
os.getegid()                  得到有效组id
os.getgid()                   得到组id
os.getuid()                   得到用户id
os.geteuid()                  得到有效用户id
os.setegid os.setegid() os.seteuid() os.setuid()  设置id
os.getgruops()                得到用户组名称列表
os.getlogin()                 得到用户登录名称
os.getenv                     得到环境变量
os.putenv                     设置环境变量
os.umask                      设置umask
os.system(cmd)                利用系统调用，运行cmd命令
```
<a name="rzJei"></a>
### 1.1.2 文件目录相关操作
```python
os.getcwd()                   # 获取现在的工作目录
os.listdir()                  获取某个目录下的所有文件名
os.remove()                   删除某个文件
os.path.exists()              检验给出的路径是否真地存在
os.path.isfile()              判断是否为文件;若是，返回值为真
os.path.isdir()               判断是否为文件夹;若是，返回值为真
os.path.abspath(name)         获得绝对路径
os.path.splitext()            分离文件名与扩展名
os.path.split()               把一个路径拆分为目录+文件名的形式
os.path.join(path,name)       连接目录与文件名或目录
os.path.basename(path)        返回文件名
os.path.dirname(path)         返回文件路径
```
<a name="itWvF"></a>
### 1.2 shutil 模块-高级文件操作
shutil 是高级的文件，文件夹，压缩包处理模块。常用方法如下：
```python
# 将文件内容拷贝到另一个文件中
shutil.copyfileobj(fsrc, fdst[, length])

# 拷贝文件
shutil.copyfile(src, dst, *, follow_symlinks=True)

# 仅拷贝权限。内容、组、用户均不变
shutil.copymode(src, dst)

# 仅拷贝状态信息，包括：mode bits, atime, mtime, flags
shutil.copystat(src, dst)

# 拷贝文件和权限
shutil.copy(src, dst)

# 拷贝文件和状态信息
shutil.copy2(src, dst)

# 递归的去拷贝文件夹
shutil.ignore_patterns(*patterns)
shutil.copytree(src, dst, symlinks=False, ignore=None)

# 递归删除文件夹
shutil.rmtree(path[, ignore_errors[, onerror]])

# 递归的去移动文件，它类似mv命令，其实就是重命名。
shutil.move(src, dst)

# 创建压缩包并返回文件路径，例如：zip、tar
shutil.make_archive(base_name, format,...)
```
<a name="L49QQ"></a>
## 2、命令行参数
<a name="iLZ1W"></a>
### 2.1 sys 模块
通用实用程序脚本通常需要处理命令行参数。这些参数作为列表存储在 sys 模块的 argv 属性中
<a name="CvNk0"></a>
### 2.2 argparse 模块
argparse 模块提供了一种处理命令行参数的机制。它应该总是优先于直接手工处理 `sys.argv`。
<a name="UcU9e"></a>
## 3、文件通配符 glob
glob 模块提供了一个在目录中使用通配符搜索创建文件列表的函数
<a name="wAusT"></a>
## 4、错误输出重定向和程序终止
错误输出重定向和终止程序使用 sys 模块，sys 模块还具有 stdin ， stdout 和 stderr 的属性。后者对于发出警告和错误消息非常有用，即使在 stdout 被重定向后也可以看到它们。
```python
sys.stderr.write('Warning, log file not found starting a new one\n')
Warning, log file not found starting a new one
```
终止脚本：
```python
sys.exit()
```
<a name="gUvgY"></a>
## 5、字符串模式匹配
<a name="WVvKt"></a>
### 5.1 正则表达式
字符串模式匹配通常也称为正则表达式，使用Python中的 re 标准库，re 模块为高级字符串处理提供正则表达式工具。对于复杂的匹配和操作，正则表达式提供简洁，优化的解决方案，具体的用法后续的文章会单独做详细操作介绍。
<a name="KF47Q"></a>
## 6、数学
<a name="qacqB"></a>
### 6.1 math 模块
数学的计算与应用使用 math 模块，math 模块提供了对浮点数学的底层函数访问；
<a name="izQj0"></a>
### 6.2 random 模块
random 模块提供了进行随机选择的工具
<a name="ZaKcB"></a>
### 6.3 statistics
statistics 模块计算数值数据的基本统计属性（均值，中位数，方差等）
<a name="gFkqe"></a>
## 7、互联网访问
有许多模块可用于访问互联网和处理互联网协议。

- urllib.request 用于从URL检索数据
- smtplib 用于发送邮件
<a name="EKuQ3"></a>
## 8、时间和日期
datetime 模块提供了以简单和复杂的方式操作日期和时间的类。虽然支持日期和时间算法，但实现的重点是有效的成员提取以进行输出格式化和操作。该模块还支持可感知时区的对象。
<a name="IT10T"></a>
### datetime模块
datetime 模块是 Python 内置的功能模块，它可以实现对日期的算数运算，以指定的方式格式化日期。datetime 模块内含有一个同名的 datetime 类，该类中包含多个操作日期的函数，例如：`datetime.now()`、`datetime.fromtimestamp()`、`datetime.timedelta()`等，下面逐一举例说明。
<a name="X5uLC"></a>
#### `datetime()`构造函数
datetime 类提供了一个`now()`的方法可以获取当前日期和时间，还提供了带参数的构造函数`datetime()`，可以通过传入特定的数字返回不同的datetime 对象。例如:
```python
import datetime
#当前日期和时间
print(datetime.datetime.now())
# 2019-09-30 22:19:37.582514

#获取指定时间
datetest = datetime.datetime(2019,9,30,22,22,0)
print(datetest)
# 2019-09-30 22:22:00

#获取日期的年月日时分秒
print(str(datetest.year)+"-"+str(datetest.month)+"-"+str(datetest.day)+" "+str(datetest.hour)+":"+str(datetest.minute)+":"+str(datetest.second))
# 2019-9-30 22:22:0
```
<a name="XH3TZ"></a>
#### `fromtimestamp()`函数
`fromtimestamp()`函数可以将时间戳转换成 datetime 对象。例如：
```python
import datetime
dt1 = datetime.datetime.fromtimestamp(10000)
dt2 = datetime.datetime.fromtimestamp(time.time())

print(dt1)
print(dt2)
```
```python
1970-01-01 10:46:40
2019-09-30 23:28:47.629210
```
<a name="bmKo3"></a>
#### `strptime()`和`strftime()`函数
使用`strptime()`函数可以将日期字符串转换成 datetime 类型，`strftime()`函数可以将 datetime 类型转换成字符串。例如：
```python
import datetime
#日期转换
datestr = datetime.strptime('2019-9-30 22:10:00', '%Y-%m-%d %H:%M:%S')
now = datetime.now()
print(datestr)
print(now.strftime('%a, %b %d %H:%M'))
```
```python
2019-09-30 22:10:00
Tue, Oct 01 00:02
```
<a name="L9SYh"></a>
#### `timedelta()`函数
`timedelta()`函数返回一个 timedelta 类型的数据，它表示一段时间而不是一个时刻，多用于日期的增加和减少场景。例如：
```python
import datetime
#日期增加和减少
now = datetime.datetime.now()
print(now)

newdate = now + datetime.timedelta(hours=10)
print(newdate)

newdate = now - datetime.timedelta(days=1)
print(newdate)
```
```python
2019-10-01 00:23:50.152118
2019-10-01 10:23:50.152118
2019-09-30 00:23:50.152118
```
<a name="ECsfM"></a>
### time模块
与 datetime 模块有所不同，time 模块主要功能是读取系统时钟的当前时间。其中，`time.time()` 和 `time.sleep()` 是两个最常用的模块。
<a name="onnCW"></a>
#### `time()`函数
`time.time()` 函数返回的值是带小数点的，它表示从 Unix 纪元（1970年1月1日0点）到执行代码那一刻所经历的时间的秒数，这个数字称为UNIX纪元时间戳。例如：
```python
import time
print ("当前时间戳为:", time.time())
```
```python
当前时间戳为: 1569770357.6496012
```
在项目开发中，经常需要计算一段代码的执行时间，就可以用纪元时间戳来实现。例如：
```python
import time
def calculateTime():
    item = 1
    for i in range(1,100000):
        item = item + i
    return item

startTime = time.time()
result = calculateTime()
endTime = time.time()
print('计算结果：'+ str(result))
print('执行时间：'+ str(endTime - startTime))
```
```python
计算结果：4999950001
执行时间：0.020943403244018555
```
在代码中，函数`calculateTime()`是需要执行的代码块，变量 startTime 表示开始时间，变量 endTime 表示结束时间，endTime-startTime表示代码块运行的间隔时间。
<a name="FSc4a"></a>
#### `sleep()`函数
如果需要让程序暂停一下，可以使用`time.sleep()`函数。`sleep()`函数有个参数，表示需要暂停的秒数。例如：
```python

import time
for i in range(2):
    print('one')
    print(time.time())
    time.sleep(1)
    print('two')
    print(time.time())
    time.sleep(1)
print('运行完成')
```
```python
one
1569772121.6350794
two
1569772122.637142
one
1569772123.639813
two
1569772124.6423109
运行完成
```
从上面程序的执行结果可以看出以下几点：

1. 打印one和打印two之间每次都间隔了一秒，因为`time.time()`函数输出结果的精确度比较高，会存在些许误差。
2. `time.sleep()`函数会阻塞代码，只有当`time.sleep()`中的秒数流逝后，才会执行后续代码
<a name="B9TfO"></a>
## 9、数据压缩
Python 中常见的数据存档和压缩格式由模块直接支持，包括：zlib, gzip, bz2, lzma, zipfile 和 tarfile。
<a name="QMM3n"></a>
## 10、性能测试
一些Python用户对了解同一问题的不同方法的相对性能产生了浓厚的兴趣。Python提供了一种可以立即回答这些问题的测量工具。<br />例如，元组封包和拆包功能相比传统的交换参数可能更具吸引力。timeit 模块可以快速演示在运行效率方面一定的优势
```python
>>> from timeit import Timer
>>> Timer('t=a; a=b; b=t', 'a=1; b=2').timeit()
0.57535828626024577
>>> Timer('a,b = b,a', 'a=1; b=2').timeit()
0.54962537085770791
```
与 timeit 的精细粒度级别相反， profile 和 pstats 模块提供了用于在较大的代码块中识别时间关键部分的工具。
<a name="kuAjz"></a>
## 11、质量控制
开发高质量软件的一种方法是在开发过程中为每个函数编写测试，并在开发过程中经常运行这些测试。
<a name="HcMEn"></a>
### 11.1 doctest
doctest 模块提供了一个工具，用于扫描模块并验证程序文档字符串中嵌入的测试。测试构造就像将典型调用及其结果剪切并粘贴到文档字符串一样简单。这通过向用户提供示例来改进文档，并且它允许doctest模块确保代码保持对文档的真实
<a name="exYj4"></a>
### 11.2 unittest
unittest 模块不像 doctest 模块那样易于使用，但它允许在一个单独的文件中维护更全面的测试集
<a name="tYHjk"></a>
## 12、自带电池
Python 有“自带电池”的理念。Python 自带电池指的是 Python 内置的模块，通过其包的复杂和强大功能可以最好地看到这一点。例如:

- `xmlrpc.client` 和 `xmlrpc.server` 模块使远程过程调用实现了几乎无关紧要的任务。尽管有模块名称，但不需要直接了解或处理XML。
- email 包是一个用于管理电子邮件的库，包括MIME和其他：基于 RFC 2822 的邮件文档。与 smtplib 和 poplib 实际上发送和接收消息不同，电子邮件包具有完整的工具集，用于构建或解码复杂的消息结构（包括附件）以及实现互联网编码和标头协议。
- json 包为解析这种流行的数据交换格式提供了强大的支持。
- csv 模块支持以逗号分隔值格式直接读取和写入文件，这些格式通常由数据库和电子表格支持。
- sqlite3 模块是SQLite数据库库的包装器，提供了一个可以使用稍微非标准的SQL语法更新和访问的持久数据库。
<a name="j2J3o"></a>
## 13、格式化输出
<a name="astdX"></a>
### 13.1 reprlib 模块
reprlib 模块提供了一个定制化版本的 `repr()` 函数，用于缩略显示大型或深层嵌套的容器对象，将容器中的对象按照一定的规律输出 reprlib 模块包含了一个类、一实例对象、一方法

1. `class reprlib.ReprRepr`类，该类提供格式化服务，对于实现与内置的 `repr()` 类似的函数很有用;添加了不同对象类型的大小限制，以避免生成过长的表示。
2. `reprlib.aReprRepr` 类的实例，用于提供下面描述的 `Repr()` 函数。更改此对象的属性将影响 `repr()` 和 Python 调试器使用的大小限制。
3. `reprlib.repr(obj)`这是 aRepr 的 `repr()` 方法。它返回一个与内置同名函数返回的字符串类似的字符串，但对大多数大小都有限制
4. `@reprlib.recursive_repr(fillvalue="...")`方法的装饰器，用于检测同一线程中的递归调用。如果执行递归调用，则返回fillvalue，否则执行通常的调用。

例如:
```python
# 导入模块
from reprlib import recursive_repr
class MyList(list):
    @recursive_repr()
    def __repr__(self):
        return '<' + '|'.join(map(repr, self)) + '>'
m = MyList('abc')
m.append(m)
m.append('x')
print(m)
```
输出结果为：
```python
<'a'|'b'|'c'|...|'x'>
```
Repr 对象具有的属性

- `Repr.maxlevel` --- 递归表示的深度限制，默认是6
- `Repr.maxdict`
- `Repr.maxlist`
- `Repr.maxtuple`
- `Repr.maxset`
- `Repr.maxfrozenset`
- `Repr.maxdeque`
- `Repr.maxarray` ----命名对象类型的条目数限制，maxdict是4，maxarray是5，其它是6
- `Repr.maxlong` ---- 表示一个整数最大字符数，默认40
- `Repr.maxstring` ---- 表示一个字符串最大字符数，默认30
- `Repr.maxother` ---- 表示其他类型的最大字符数，默认20

例如：
```python
# 递归实例演示
import reprlib
a = [1,2,3,[4,5],6,7]
reprlib.aRepr.maxlevel = 1
print(reprlib.repr(a))
```
输出结果为：
```python
[1, 2, 3, [...], 6, 7]
```
<a name="GbbN4"></a>
### 13.2  pprint 模块
pprint 模块提供了更加复杂的打印控制，其输出的内置对象和用户自定义对象能够被解释器直接读取。当输出结果过长而需要折行时，“美化输出机制”会添加换行符和缩进，以更清楚地展示数据结构
<a name="o9KkU"></a>
### 13.3  textwrap 模块
textwrap 模块能够格式化文本段落，以适应给定的屏幕宽度， 该模块提供了一些便利功能以及可以完成所有工作的类。如果只是包装或填充一个或两个文本字符串，那么便利功能应该足够好；否则应该使用一个模块化功能提高效率。
<a name="JPdok"></a>
### 13.4 locale 模块
locale 模块处理与特定地域文化相关的数据格式。locale 模块的 format 函数包含一个 grouping 属性，可直接将数字格式化为带有组分隔符的样式
<a name="ZnadK"></a>
## 14、string 模板
string 模块包含一个通用的 Template 类，具有适用于最终用户的简化语法。它允许用户在不更改应用逻辑的情况下定制自己的应用
<a name="FAB44"></a>
## 15、使用二进制数据记录格式
struct 模块提供了 pack() 和 unpack() 函数，用于处理不定长度的二进制记录格式。下面的例子展示了在不使用 zipfile 模块的情况下，如何循环遍历一个 ZIP 文件的所有头信息。Pack 代码 "H" 和 "I" 分别代表两字节和四字节无符号整数。"<" 代表它们是标准尺寸的小尾型字节序
<a name="cgYX8"></a>
## 16、多线程
线程是一种对于非顺序依赖的多个任务进行解耦的技术。多线程可以提高应用的响应效率，当接收用户输入的同时，保持其他任务在后台运行。一个有关的应用场景是，将 I/O 和计算运行在两个并行的线程中，在程序编写过程中，熟悉多线程的应用能提高代码运行效率
<a name="PcO0U"></a>
## 17、日志
Python 日志模块 logging 模块提供功能齐全且灵活的日志记录系统。在最简单的情况下，日志消息被发送到文件或 sys.stderr。日志系统可以直接从 Python 配置，也可以从用户配置文件加载，以便自定义日志记录而无需更改应用程序。
<a name="EiBeU"></a>
## 18、弱引用
Python 会自动进行内存管理（对大多数对象进行引用计数并使用 garbage collection 来清除循环引用）。当某个对象的最后一个引用被移除后不久就会释放其所占用的内存
<a name="ko5qI"></a>
## 19、用于操作列表的工具
许多对于数据结构的需求可以通过内置列表 类型来满足。但是，有时也会需要具有不同效费比的替代实现。
<a name="dQZhx"></a>
### 19.1 array 模块
array 模块提供了一种 array() 对象，它类似于列表，但只能存储类型一致的数据且存储密集更高。下面的例子演示了一个以两个字节为存储单元的无符号二进制数值的数组 (类型码为 "H")，而对于普通列表来说，每个条目存储为标准 Python 的 int 对象通常要占用16 个字节:
```python
>>> from array import array
>>> a = array('H', [4000, 10, 700, 22222])
>>> sum(a)
26932
>>> a[1:3]
array('H', [10, 700])
```
<a name="hZ3IV"></a>
### 19.2  collections 模块
collections 模块提供了一种 `deque()` 对象，它类似于列表，但从左端添加和弹出的速度较快，而在中间查找的速度较慢。此种对象适用于实现队列和广度优先树搜索:
```python
>>> from collections import deque
>>> d = deque(["task1", "task2", "task3"])
>>> d.append("task4")
>>> print("Handling", d.popleft())
Handling task1
unsearched = deque([starting_node])
def breadth_first_search(unsearched):
    node = unsearched.popleft()
    for m in gen_moves(node):
        if is_goal(m):
            return m
        unsearched.append(m)
```
在替代的列表实现以外，标准库也提供了其他工具，例如 bisect 模块具有用于操作排序列表的函数:
```python
>>> import bisect
>>> scores = [(100, 'perl'), (200, 'tcl'), (400, 'lua'), (500, 'python')]
>>> bisect.insort(scores, (300, 'ruby'))
>>> scores
[(100, 'perl'), (200, 'tcl'), (300, 'ruby'), (400, 'lua'), (500, 'python')]
```
<a name="eVOqj"></a>
### 19.3 heapq 模块
heapq 模块提供了基于常规列表来实现堆的函数。最小值的条目总是保持在位置零。这对于需要重复访问最小元素而不希望运行完整列表排序的应用来说非常有用:
```python
>>> from heapq import heapify, heappop, heappush
>>> data = [1, 3, 5, 7, 9, 2, 4, 6, 8, 0]
>>> heapify(data)                      # rearrange the list into heap order
>>> heappush(data, -5)                 # add a new entry
>>> [heappop(data) for i in range(3)]  # fetch the three smallest entries
[-5, 0, 1]
```
<a name="uj8Zh"></a>
## 20、十进制浮点运
decimal 模块提供了一种 Decimal 数据类型用于十进制浮点运算。相比内置的 float 二进制浮点实现，该类特别适用于以下几种场景：

- 财务应用和其他需要精确十进制表示的用途，
- 控制精度，
- 控制四舍五入以满足法律或监管要求，
- 跟踪有效小数位，或
- 用户期望结果与手工完成的计算相匹配的应用程序。

例如，使用十进制浮点和二进制浮点数计算 70 美分手机和 5％ 税的总费用，会产生的不同结果。如果结果四舍五入到最接近的分数差异会更大
