Python logging 日志管理
<a name="vVlTT"></a>
## logger的常见使用

1. 日志等级
2. 实际开发中的日志记录
3. 日志输出到屏幕
4. 日志输入到文件
5. 完整代码实例
<a name="0VhfX"></a>
## 日志的等级
<a name="pUtFD"></a>
### 等级排序
debug  <  info < warn < error < fatal
<a name="4d66a7ba"></a>
### 各等级的含义

- debug：级别最低，用户开发过程中的调试
- info：打印程序运行是的正常的信息，用于替代print输出
- warn：打印警告信息，不影响程序的运行
- error：程序运行出错，可以修复，常用在except异常捕获之后
- fatal：非常严重，无法修复，程序继续运行的话后果非常严重
<a name="ax8Ug"></a>
### 使用注意事项
:::warning
设置了日志等级后，调用比等级低的日志记录函数则不会输出
:::
<a name="1BL8C"></a>
## 导入logging模块
```python
import logging
```
<a name="5Rlvd"></a>
## 指定日志输出级别
```python
logging.basicConfig(level=logging.DEBUG)

logging.debug('debug级别，一般用来打印一些调试信息，级别最低')
logging.info('info级别，一般用来打印一些正常的操作信息')
logging.warning('waring级别，一般用来打印警告信息')
logging.error('error级别，一般用来打印一些错误信息')
logging.critical('critical级别，一般用来打印一些致命的错误信息，等级最高')
```
<a name="Cnp3k"></a>
## 日志输出格式配置
```python
logging.basicConfig(format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s',
                    level=logging.DEBUG)
```
format 可以指定输出的内容和格式，其内置的参数如下：
```
%(name)s：Logger的名字
%(levelno)s：打印日志级别的数值
%(levelname)s：打印日志级别的名称
%(pathname)s：打印当前执行程序的路径，其实就是sys.argv[0]
%(filename)s：打印当前执行程序名
%(funcName)s：打印日志的当前函数
%(lineno)d：打印日志的当前行号
%(asctime)s：打印日志的时间
%(thread)d：打印线程ID
%(threadName)s：打印线程名称
%(process)d：打印进程ID
%(message)s：打印日志信息
```
<a name="O0mEx"></a>
## 输出日志内容到文件- 使用 `basicConfig` 配置日志文件路径
:::tips
以下配置后，日志会保存至日志文件，控制台不会输出内容
:::
只是在上面配置的基础上加上`filename`和 `filemode`参数，这样就可以把日志输出到 test.log 文件中了，如果没有这个文件的话会自动创建。<br />其中参数 `filemode`表示文件打开模式，不设的话默认为`'a'`，即追加模式，可以不设；也可以设为`'w'`，每次写日志会覆盖之前的日志。
```python
logging.basicConfig(format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s',
                    level=logging.DEBUG,
                    filename='test.log',
                    filemode='a')
```
<a name="523d11b7"></a>
## 日志文件和控制台日志的分别管理
先来了解一下logging的模块化设计
<a name="ERUii"></a>
### logging的模块化设计
 logging 库采取了模块化的设计，提供了许多组件：记录器、处理器、过滤器和格式化器。

- Logger 暴露了应用程序代码能直接使用的接口。
- Handler 将（记录器产生的）日志记录发送至合适的目的地。
- Filter 提供了更好的粒度控制，它可以决定输出哪些日志记录。
- Formatter 指明了最终输出中日志记录的内容和格式。
<a name="E8o6f"></a>
#### Loggers
Logger 对象要做三件事情。首先，它们向应用代码暴露了许多方法，这样应用可以在运行时记录消息。其次，记录器对象通过严重程度（默认的过滤设施）或者过滤器对象来决定哪些日志消息需要记录下来。第三，记录器对象将相关的日志消息传递给所有感兴趣的日志处理器。<br />常用的记录器对象的方法分为两类：配置和发送消息。<br />这些是最常用的配置方法：<br />`Logger.setLevel()`指定logger将会处理的最低的安全等级日志信息, `debug`是最低的内置安全等级，`critical`是最高的内建安全等级。例如，如果严重程度为`INFO`，记录器将只处理`INFO`，`WARNING`，`ERROR`和`CRITICAL`消息，DEBUG消息被忽略。<br />`Logger.addHandler()` 和 `Logger.removeHandler()` 从记录器对象中添加和删除处理程序对象。处理器详见Handlers。<br />`Logger.addFilter()` 和 `Logger.removeFilter()` 从记录器对象添加和删除过滤器对象。<br />简单地说，其中 logger 是负责记录日志消息的，然后要把这些日志消息放到哪里，交给 Handler 处理，Filter 则帮过滤信息（不限于通过级别过滤），Formatter 就是跟上面的 format 一个意思，用来设置日志内容和格式。
<a name="ps2by"></a>
#### Handlers
`处理程序`对象负责将适当的日志消息（基于日志消息的严重性）分派到处理程序的指定目标。`Logger` 对象可以通过`addHandler()`方法增加零个或多个handler对象。举个例子，一个应用可以将所有的日志消息发送至日志文件，所有的错误级别（error）及以上的日志消息发送至标准输出，所有的严重级别（critical）日志消息发送至某个电子邮箱。在这个例子中需要三个独立的处理器，每一个负责将特定级别的消息发送至特定的位置。<br />常用的有4种： 
<a name="0T71H"></a>
##### 1) `logging.StreamHandler` -> 控制台输出 
使用这个Handler可以向类似与`sys.stdout`或者`sys.stderr`的任何文件对象(file object)输出信息。<br />它的构造函数是：
```python
StreamHandler([strm])
```
其中strm参数是一个文件对象。默认是sys.stderr
<a name="pWVmi"></a>
##### 2) `logging.FileHandler`  -> 文件输出
和`StreamHandler`类似，用于向一个文件输出日志信息。不过`FileHandler`会打开这个文件。它的构造函数是：
```python
FileHandler(filename[,mode])
```
filename是文件名，必须指定一个文件名。<br />mode是文件的打开方式。默认是'a'，即添加到文件末尾。
<a name="KAZaw"></a>
##### 3) `logging.handlers.RotatingFileHandler` -> 按照大小自动分割日志文件，一旦达到指定的大小重新生成文件 
这个Handler类似于上面的`FileHandler`，但是它可以管理文件大小。当文件达到一定大小之后，它会自动将当前日志文件改名，然后创建 一个新的同名日志文件继续输出。比如日志文件是chat.log。当chat.log达到指定的大小之后，`RotatingFileHandler`自动把 文件改名为chat.log.1。不过，如果chat.log.1已经存在，会先把chat.log.1重命名为chat.log.2。最后重新创建 chat.log，继续输出日志信息。它的构造函数是：
```python
RotatingFileHandler( filename[, mode[, maxBytes[, backupCount]]])
```
其中filename和mode两个参数和`FileHandler`一样。<br />`maxBytes`用于指定日志文件的最大文件大小。如果maxBytes为0，意味着日志文件可以无限大，这时上面描述的重命名过程就不会发生。<br />`backupCount`用于指定保留的备份文件的个数。比如，如果指定为2，当上面描述的重命名过程发生时，原有的chat.log.2并不会被更名，而是被删除。
<a name="IIpQM"></a>
##### 4) `logging.handlers.TimedRotatingFileHandler`  -> 按照时间自动分割日志文件 
这个`Handler`和`RotatingFileHandler`类似，不过，它没有通过判断文件大小来决定何时重新创建日志文件，而是间隔一定时间就自动创建新的日志文件。重命名的过程与`RotatingFileHandler`类似，不过新的文件不是附加数字，而是当前时间。它的构造函数是：
```python
TimedRotatingFileHandler( filename [,when [,interval [,backupCount]]])
```
其中`filename`参数和`backupCount`参数和`RotatingFileHandler`具有相同的意义。`interval`是时间间隔。<br />`when`参数是一个字符串。表示时间间隔的单位，不区分大小写。它有以下取值：
```
S 秒
M 分
H 小时
D 天
W 每星期（interval==0时代表星期一）
midnight 每天凌晨
```
配置方法：

- `setLevel()` 方法和日志对象的一样，指明了将会分发日志的最低级别。为什么会有两个`setLevel()`方法？记录器的级别决定了消息是否要传递给处理器。每个处理器的级别决定了消息是否要分发。
- `setFormatter()`为该处理器选择一个格式化器。
- `addFilter()`和`removeFilter()`分别配置和取消配置处理程序上的过滤器对象。
<a name="Formatters"></a>
#### Formatters
Formatter对象设置日志信息最后的规则、结构和内容，默认的时间格式为`%Y-%m-%d %H:%M:%S`，下面是Formatter常用的一些信息

| %(name)s | Logger的名字 |
| --- | --- |
| %(levelno)s | 数字形式的日志级别 |
| %(levelname)s | 文本形式的日志级别 |
| %(pathname)s | 调用日志输出函数的模块的完整路径名，可能没有 |
| %(filename)s | 调用日志输出函数的模块的文件名 |
| %(module)s | 调用日志输出函数的模块名 |
| %(funcName)s | 调用日志输出函数的函数名 |
| %(lineno)d | 调用日志输出函数的语句所在的代码行 |
| %(created)f | 当前时间，用UNIX标准的表示时间的浮 点数表示 |
| %(relativeCreated)d | 输出日志信息时的，自Logger创建以 来的毫秒数 |
| %(asctime)s | 字符串形式的当前时间。默认格式是 “2003-07-08 16:49:45,896”。逗号后面的是毫秒 |
| %(thread)d | 线程ID。可能没有 |
| %(threadName)s | 线程名。可能没有 |
| %(process)d | 进程ID。可能没有 |
| %(message)s | 用户输出的消息 |

<a name="weoqQ"></a>
### 使用模块的方式，重新记录日志
```python
logger = logging.getLogger('test')
logger.debug('debug级别，一般用来打印一些调试信息，级别最低')
logger.info('info级别，一般用来打印一些正常的操作信息')
logger.warning('waring级别，一般用来打印警告信息')
logger.error('error级别，一般用来打印一些错误信息')
logger.critical('critical级别，一般用来打印一些致命的错误信息，等级最高')
```
首先第一行 getLogger 获取了一个记录器，其中命名标识了这个 Logger。然后下面的输出方式跟一开始 logging 的用法是很相似的，看起来很简单。但这样是不行，运行后会报错：
```
No handlers could be found for logger "test"
```
报错指明没有为这个logger指定handler，它不知道要怎么处理日志，要输出到哪里去。就给他加一个Handler，Handler的种类有很多，常用的有4种：

- `logging.StreamHandler` -> 控制台输出
- `logging.FileHandler` -> 文件输出
- `logging.handlers.RotatingFileHandler` -> 按照大小自动分割日志文件，一旦达到指定的大小重新生成文件
- `logging.handlers.TimedRotatingFileHandler` -> 按照时间自动分割日志文件

现在先使用最简单的`StreamHandler`把日志输出到控制台：
```python
logger = logging.getLogger('test')
stream_handler = logging.StreamHandler()
logger.addHandler(stream_handler)
```
这样就可以在控制台看到：
```
waring级别，一般用来打印警告信息
error级别，一般用来打印一些错误信息
critical级别，一般用来打印一些致命的错误信息，等级最高
```
还是少了几条日志，因为没有设置日志级别，同样设置一下级别，并且也使用`Formatter`模块设置一下输出格式。
```python
logger = logging.getLogger('test')
logger.setLevel(level=logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s')
stream_handler = logging.StreamHandler()
stream_handler.setLevel(logging.DEBUG)
stream_handler.setFormatter(formatter)
logger.addHandler(stream_handler)
```
发现`Formatter`是给handler设置的，因为handler是负责把日志输出到哪里，所以是给它设置格式，而不是给logger；从上面的代码看到 level 设置了两次，原因是给logger设置是告诉它要记录哪些级别的日志，给handler设是告诉它要输出哪些级别的日志，相当于进行了两次过滤。这样的好处在于，当有多个日志去向时，比如既保存到文件，又输出到控制台，就可以分别给他们设置不同的级别；logger 的级别是先过滤的，所以被 logger 过滤的日志 handler 也是无法记录的，这样就可以只改 logger 的级别而影响所有输出。两者结合可以更方便地管理日志记录的级别。<br />有了handler，就可以很方便地同时将日志输出到控制台和文件：
```python
logger = logging.getLogger('test')
logger.setLevel(level=logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s')
file_handler = logging.FileHandler('test2.log')
file_handler.setLevel(level=logging.INFO)
file_handler.setFormatter(formatter)
stream_handler = logging.StreamHandler()
stream_handler.setLevel(logging.DEBUG)
stream_handler.setFormatter(formatter)
logger.addHandler(file_handler)
logger.addHandler(stream_handler)
```
只需要多加一个FileHandler即可。
<a name="LdxBG"></a>
## 自动分割日志文件
有时候需要对日志文件进行分割，以方便日志的管理。python 提供了两个处理器，方便分割文件：

- `logging.handlers.RotatingFileHandler` -> 按照大小自动分割日志文件，一旦达到指定的大小重新生成文件
- `logging.handlers.TimedRotatingFileHandler` -> 按照时间自动分割日志文件

使用方法跟上面的 Handler 类似，只是需要添加一些参数配置，比如`when='D'`表示以天为周期切分文件
```python
from logging import handlers
time_rotating_file_handler = handlers.TimedRotatingFileHandler(filename='rotating_test.log', when='D')
time_rotating_file_handler.setLevel(logging.DEBUG)
time_rotating_file_handler.setFormatter(formatter)
logger.addHandler(time_rotating_file_handler)
```
若改为`when='S'`，则以秒为周期进行切割，其中没有后缀的为最新日志文件。
