Python 日志 logging<br />logging 是 python 标准模块，用于记录和处理程序中的日志。<br />功能很强大，官方文档很详细，网上也有大量的说明和教程，但是对很多初次接触的同学来说，会存在一些障碍。<br />一是因为标准库文档太过繁琐，需要较高的理论基础，着急用时，常常被文档搞晕。<br />二是大部分说明材料要么是官方文档的罗列，要么是简单的应用，对实际应用帮助不大。<br />从应用上的一些问题开始，探讨一下日志神器 logging 极其背后的原理。
<a name="Dm8R2"></a>
## 该用 `logging.debug` 还是 `logger.debug` ？
`debug` 是日志模块中的一个日志等级为 DEBUG 的日志生成方法，还有 `info`、`warning`、`error`、`critial`，这里用 `debug` 做为代表进行说明。<br />常会看到，一会儿用 `logging.debug` 记录日志，一会儿又用 `logger.debug` 记录日志，到底该用什么？<br />先看代码:
```python
import logging

logging.debug('调试信息')

logger = logging.getLogger()

logger.debug('调试信息')
```
首先 `logging` 是作为一个模块被引入的。`logging.debug` 用的是 `logging` 模块的模块方法。<br />`logger` 是用 `logging.getLogger()` 生成的，是一个 日志对象，`logger.debug` 调用的是 `logger` 这个日志对象的方法。<br />上面的代码中 `logging.debug` 和 `logger.debug` 的效果完全是一样的。<br />这是因为，为了让开发者方便使用，`logging` 模块提供了一些列模块方法，如 `debug`，在引入模块后，就可以直接使用。这样开发者就不必关心日志模块的细节，像用 `print` 一样输出日志。<br />如果需要对日志输出进行定制化，比如将日志输出到文件中，过滤某些级别的日志，就需要创建或者得到一个实际的日志对象来处理，如上面代码中通过 `getLogger` 方法得到的日志对象。<br />程序设计里要避免重复的设计，如果模块方法采用一套机制，日志对象上的方法采用另一套机制，就会出现重复造轮子的问题。<br />所以在使用模块方法，logging 其实创建了一个日志对象 —— root logger。<br />也就是 `logging.debug` 这个调用，实质上是调用 root logger 的日志方法。<br />相当于默认情况下 root logger 会作为日志处理对象。<br />如何获得 root logger 对象呢？<br />通过不带参数的 `logging.getLogger()` 方法获得。<br />那么 `logging.debug`  和 `rootLogger.debug` 是一会事，可以理解（但不严谨）为 `logging.debug` 是 `rootlogger.debug` 的快捷方式。
<a name="JDi5u"></a>
## 日志树
稍加留意就会观察到，程序是有层次结构的，通过相互引用，调用形成一个树状结构。<br />程序加载的地方是树根，比如 python 中要运行的代码文件，称之为 `main`。从树根开始长出其他枝叶。对于一个模块来说，又会形成一个自己的树。<br />如何用日志清楚地记录层次结构呢？<br />虽然直接打印出调用堆栈也可以看到调用结构，不过不太直观，缺乏业务逻辑描述。<br />而用 `print` 来打印出层次结构，需要编写大量的代码才能反射出（通过运行状态获取代码状态的一种方式）调用环境。<br />`logging` 提供了完毕的解决方案。<br />前面提到的 root logger 就是整个日志树的根，其他所有的 logger 都是从 root logger 伸展出来的枝叶。只要通过 `getLogger(loggername)` 方法获得的 logger 对象，都是伸展自 root logger 的。<br />如何向下伸展呢？<br />很简单，就像引用模块的层次关系一样，用 . 分隔层次就好了，例如：
```python
logger = logging.getLogger('mod1.mod2.mod3')

logger.debug("调试信息")
```
语句 `logging.getLogger('mod1.mod2.mod3')` 实际上创建了三个 logger，名称分别是 mod1、mod1.mod2 和 mod1.mod2.mod3<br />mod1 为根，mod1.mod2 为子，mod1.mod2.mod3 为孙。<br />如果在 mod1 上设置了日志处理器（handler），那么其他两个的日志对象都会用到这个处理器。<br />这样不但记录的日志更清晰而且，可以为同一个根的日志对象设置可以共享的日志处理方式。<br />这样感觉也不方便，需要些那么多层次，如何才能更方便呢？在下面的 实践参考 里会有说明。
<a name="bB6kP"></a>
## `logging.basicConfig` 的功与过
来看看一个很常用的设置方法 `basicConfig`。<br />它可以方便的设置日志处理和记录方式，如没必要，不用为每个日志对象单独设置。<br />根据第一节的分析，直接使用模块方法，用的其实是 root logger，那么就能明白 `basicConfig` 设置了 root logger 的日志处理方式。<br />这就意味着:<br />一旦设置了通过 `logging.basicConfig` 设置了日志处理方式，其他所有日志都很受到影响。<br />另外 `basicConfig` 是个一次性方法，即:<br />只有第一次设置有效，其后设置无效<br />本来是个一劳永逸的方法。<br />但用错了地方，就会很麻烦。<br />看下例子：
```python
__all__ = ['Connection', 'ConnectionPool', 'logger']

warnings.filterwarnings('error', category=pymysql.err.Warning)
# use logging module for easy debug
logging.basicConfig(format='%(asctime)s %(levelname)8s: %(message)s', datefmt='%m-%d %H:%M:%S')
logger = logging.getLogger(__name__)
logger.setLevel('WARNING')
```
这段代码中，用 `logging.basicConfig` 对日志做了设置，意思是后面的日志都按照这样的方式输出。<br />但它是一个底层模块 —— [pymysqlpool](https://pypi.org/project/pymysql-pool/)。<br />pymysqlpool 封装了 [pymysql](https://pypi.org/project/PyMySQL/) 模块，提供了链接池特性，在多线程处理数据库场景下很有用。<br />也就是说，pymysqlpool 只会被引用加载，不会作为 `main` 被加载，这就比较尴尬了，因为 main 中对日志的设置就没有效果。<br />作为一个服务类模块（相对于业务的底层模块），不要通过 `basicConfig` 来设置日志模式，要么通过自己专属的日志对象来设置，要么不去设置，统一交给 `main` 去设置，例如：
```python
logger = logging.getLogger(__name__)

fmt = logging.Formatter("%(asctime)s %(levelname)8s: %(message)s", datefmt='%m-%d %H:%M:%S')
hdl = logging.StreamHandler()
hdl.setFormatter(fmt)
logger.addHandler(hdl)

logger.setLevel('WARNING')
```
如果为了测试，可以在测试的初始化方法中，使用 `basicConfig` 来设置，因为测试时，模块往往是被作为程序入库加载的。
<a name="gf7fK"></a>
## 实践参考
了解了日志模块的一下特性，和其中的原理之后，这里有几条实践参考。

1. 不要再子模块中使用 `logging.basicConfig` 设置日志模式
2. 强烈建议在任何模块中通过 `logger = logging.getLogger(__name__)` 来创建日志对象 因为 `__name__` 代表的就是模板被加载的引用名称。<br />例如 `from a.b.c import b`  模块 c 中的 `__name__` 值就为 `a.b.c`。<br />而且这个引用名称刚好符合 `logger` 定义的层次结构。
3. 通过命令行参数设置不同类型的日志，见代码：
```python
import logging
import argparse
logger = logging.getLogger(__name__)

def create_args_parse():
    parser = argparse.ArgumentParser(description="参数列表")
    parser.add_argument('-d', '--debug', action='store_true', help='调试模式')
    # 加入其他命令行参数
        
    return parser

def set_logger(debug):
    formatter = logging.Formatter('%(asctime)s - %(levelname)8s - %(name)s - %(filename)s:%(lineno)d - %(thread)d- %(funcName)s:\t%(message)s')
    if debug:
        hd = logging.StreamHandler()
        logger.setLevel(logging.DEBUG)
        hd.setFormatter(formatter)
    else:
        hd = logging.FileHandler(f'{__name__}.log', 'a', encoding='utf-8')
        logger.setLevel(logging.INFO)
        hd.setFormatter(formatter)
    logger.addHandler(hd)

if __name__ == '__main__':
   parser = create_args_parse()
   args = parser.parse_args()
   debug = args.debug
   set_logger(debug)
   ...
```
代码有点长，但不难懂。

- `create_args_parse` 方法用于解析命令行参数，其中定义了一个 debug 参数，表示开启调试模式
- `set_logger` 方法接收一个是否为调试模式的参数，根据是否为调试模式，设置不同的日志模式
- `main` 中，首先调用 `create_args_parse` 获得命令行参数对象，然后从中解析出参数，提取 debug 模式，传送给 `set_logger` 方法，设置日志模式

这样只需要在运行程序时，加上参数 `-d` 就可以让日志打印到终端上，不加，日志就会自动去 `__main__.log` 日志文件中去了。
<a name="yMEJH"></a>
## 总结
python 提供了很多便利的功能，有些需要真的用到才能有所体会，所以在遇到问题时，需要多研究一下，找到其中的特点和内在的原理或机制，这样就能更好的应用了。<br />在理解了 logging 的原理之后，已经在很多项目中发挥了巨大作用，而且再也不必纠结于怎么用，如何更合理等这些问题了。
