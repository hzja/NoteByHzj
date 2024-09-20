Python
<a name="Mz96f"></a>
## 带任意数量参数的函数
Python允许定义可选参数。但还有一个方法，可以定义函数任意数量的参数。<br />首先，看下面是一个只定义可选参数的例子：
```python
def function(arg1="", arg2=""):
    print "arg1: {0}".format(arg1)
    print "arg2: {0}".format(arg2)

function("Hello", "World")
# prints args1: Hello
# prints args2: World

function()
# prints args1:
# prints args2:
```
现看看怎么定义一个可以接受任意参数的函数。利用元组来实现。
```python
def foo(*args):  
    # just use "*" to collect all remaining arguments into a tuple
    numargs = len(args)
    print "Number of arguments: {0}".format(numargs)
    for i, x in enumerate(args):
        print "Argument {0} is: {1}".format(i, x)

foo()
# Number of arguments: 0

foo("hello")
# Number of arguments: 1
# Argument 0 is: hello

foo("hello", "World", "Again")
# Number of arguments: 3
# Argument 0 is: hello
# Argument 1 is: World
# Argument 2 is: Again
```
:::info
补充：一般的函数定义方式是`def fun(*args,**kwargs)`，可以在许多Python源码中发现这种定义，其中`*args`表示任何多个无名参数，它本质是一个元组`tuple`；`**kwargs`表示关键字参数，它本质上是一个字典`dict`。
:::
<a name="znUMU"></a>
## 使用`Glob()`查找文件
大多Python函数有着长且具有描述性的名字。但是命名为`glob()`的函数可能不知道它是干什么的除非从别处已经熟悉它了。它像是一个更强大版本的`listdir()`函数。可以通过使用模式匹配来搜索文件。
```python
import glob

# get all py files
files = glob.glob('*.py')
print files

# Output
# ['arg.py', 'g.py', 'shut.py', 'test.py']
```
可以像下面这样查找多个文件类型：
```python
import itertools as it, glob

def multiple_file_types(*patterns):
    return it.chain.from_iterable(glob.glob(pattern) \
            for pattern in patterns)

for filename in multiple_file_types("*.txt", "*.py"):  
    # add as many filetype arguements
    print filename

# output
# =========#
# test.txt
# arg.py
# g.py
# shut.py
# test.py
```
如果想得到每个文件的绝对路径，可以在返回值上调用`realpath()`函数：
```python
import itertools as it, glob, os

def multiple_file_types(*patterns):
    return it.chain.from_iterable(glob.glob(pattern) \
            for pattern in patterns)

for filename in multiple_file_types("*.txt", "*.py"):  
    # add as many filetype arguements
    realpath = os.path.realpath(filename)
    print realpath

# output
#=========#
# C:\xxx\pyfunc\test.txt
# C:\xxx\pyfunc\arg.py
# C:\xxx\pyfunc\g.py
# C:\xxx\pyfunc\shut.py
# C:\xxx\pyfunc\test.py
```
<a name="pbiwH"></a>
## 调试
下面的例子使用`inspect`模块。该模块用于调试目的时是非常有用的，它的功能远比这里描述的要多。<br />展示一些用例。
```python
import logging, inspect

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s %(levelname)-8s %(filename)s:%(lineno)-4d: %(message)s',
                    datefmt='%m-%d %H:%M',
                    )
logging.debug('A debug message')
logging.info('Some information')
logging.warning('A shot across the bow')

def test():
    frame, filename, line_number, function_name, lines, index = \
        inspect.getouterframes(inspect.currentframe())[1]
    print(frame, filename, line_number, function_name, lines, index)

test()

# Should print the following (with current date/time of course)
# 10-19 19:57 INFO     test.py:9   : Some information
# 10-19 19:57 WARNING  test.py:10  : A shot across the bow
# (, 'C:/xxx/pyfunc/magic.py', 16, '', ['test()\n'], 0)
```
<a name="o5PyD"></a>
## 生成唯一ID
在有些情况下需要生成一个唯一的字符串。很多人使用`md5()`函数来达到此目的，但它确实不是以此为目的。其实有一个名为`uuid()`的Python函数是用于这个目的的。
```python
import uuid

result = uuid.uuid1()
print result

# output => various attempts
# 9e177ec0-65b6-11e3-b2d0-e4d53dfcf61b
# be57b880-65b6-11e3-a04d-e4d53dfcf61b
# c3b2b90f-65b6-11e3-8c86-e4d53dfcf61b
```
即使字符串是唯一的，但它们后边的几个字符看起来很相似。这是因为生成的字符串与电脑的MAC地址是相联系的。为了减少重复的情况，可以使用这两个函数。
```python
import hmac, hashlib

key = '1'
data = 'a'
print hmac.new(key, data, hashlib.sha256).hexdigest()

m = hashlib.sha1()
m.update("The quick brown fox jumps over the lazy dog")
print m.hexdigest()

# c6e693d0b35805080632bc2469e1154a8d1072a86557778c27a01329630f8917
# 2fd4e1c67a2d28fced849ee1bb76e7391b93eb12
```
<a name="a7VFs"></a>
## 序列化
曾经需要将一个复杂的变量存储在数据库或文本文件中时，不需要想一个奇特的方法将数组或对象格转化为式化字符串，因为Python已经提供了此功能。
```python
import pickle

variable = ['hello', 42, [1, 'two'], 'apple']

# serialize content
file = open('serial.txt', 'w')
serialized_obj = pickle.dumps(variable)
file.write(serialized_obj)
file.close()

# unserialize to produce original content
target = open('serial.txt', 'r')
myObj = pickle.load(target)

print serialized_obj
print myObj

# output
# (lp0
# S'hello'
# p1
# aI42
# a(lp2
# I1
# aS'two'
# p3
# aaS'apple'
# p4
# a.
# ['hello', 42, [1, 'two'], 'apple']
```
这是一个原生的Python序列化方法。然而近几年来JSON变得流行起来，Python添加了对它的支持。现在可以使用JSON来编解码。
```python
import json

variable = ['hello', 42, [1, 'two'], 'apple']
print "Original {0} - {1}".format(variable, type(variable))

# encoding
encode = json.dumps(variable)
print "Encoded {0} - {1}".format(encode, type(encode))

# deccoding
decoded = json.loads(encode)
print "Decoded {0} - {1}".format(decoded, type(decoded))

# output

# Original ['hello', 42, [1, 'two'], 'apple'] - <type 'list'="">
# Encoded ["hello", 42, [1, "two"], "apple"] - <type 'str'="">
# Decoded [u'hello', 42, [1, u'two'], u'apple'] - <type 'list'="">
```
这样更紧凑，而且最重要的是与JavaScript和许多其他语言兼容。然而对于复杂的对象，其中的一些信息可能丢失。
<a name="WPCAr"></a>
## 压缩字符
当谈起压缩时通常想到文件，比如ZIP结构。在Python中也可以压缩长字符：
```python
import zlib

string = """   Lorem ipsum dolor sit amet, consectetur
                adipiscing elit. Nunc ut elit id mi ultricies
                adipiscing. Nulla facilisi. Praesent pulvinar,
                sapien vel feugiat vestibulum, nulla dui pretium orci,
                non ultricies elit lacus quis ante. Lorem ipsum dolor
                sit amet, consectetur adipiscing elit. Aliquam
                pretium ullamcorper urna quis iaculis. Etiam ac massa
                sed turpis tempor luctus. Curabitur sed nibh eu elit
                mollis congue. Praesent ipsum diam, consectetur vitae
                ornare a, aliquam a nunc. In id magna pellentesque
                tellus posuere adipiscing. Sed non mi metus, at lacinia
                augue. Sed magna nisi, ornare in mollis in, mollis
                sed nunc. Etiam at justo in leo congue mollis.
                Nullam in neque eget metus hendrerit scelerisque
                eu non enim. Ut malesuada lacus eu nulla bibendum
                id euismod urna sodales. """

print "Original Size: {0}".format(len(string))

compressed = zlib.compress(string)
print "Compressed Size: {0}".format(len(compressed))

decompressed = zlib.decompress(compressed)
print "Decompressed Size: {0}".format(len(decompressed))

# output

# Original Size: 1022
# Compressed Size: 423
# Decompressed Size: 1022
```
<a name="xejUF"></a>
## 注册`Shutdown`函数
有个模块叫`atexit`，它可以在脚本运行完后立马执行一些代码。<br />假如想在脚本执行结束时测量一些基准数据，比如运行了多长时间：
```python
import atexit
import time
import math

def microtime(get_as_float=False):
    if get_as_float:
        return time.time()
    else:
        return '%f %d' % math.modf(time.time())

start_time = microtime(False)
atexit.register(start_time)

def shutdown():
    global start_time
    print "Execution took: {0} seconds".format(start_time)

atexit.register(shutdown)

# Execution took: 0.297000 1387135607 seconds
# Error in atexit._run_exitfuncs:
# Traceback (most recent call last):
#   File "C:\Python27\lib\atexit.py", line 24, in _run_exitfuncs
#     func(*targs, **kargs)
# TypeError: 'str' object is not callable
# Error in sys.exitfunc:
# Traceback (most recent call last):
#   File "C:\Python27\lib\atexit.py", line 24, in _run_exitfuncs
#     func(*targs, **kargs)
# TypeError: 'str' object is not callable
```
看来很简单。只需要将代码添加到脚本的最底层，它将在脚本结束前运行。但如果脚本中有一个致命错误或者脚本被用户终止，它可能就不运行了。<br />当使用`atexit.register()`时，代码都将执行，不论脚本因为什么原因停止运行。
