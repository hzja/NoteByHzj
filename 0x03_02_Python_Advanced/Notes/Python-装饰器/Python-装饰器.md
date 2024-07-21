Python 装饰器
<a name="0cREZ"></a>
### 1、装饰器
对于受到封装的原函数比如`f`来说，装饰器能够在`f`函数**执行前**或者**执行后**分别运行一些代码。
<a name="QMEj3"></a>
### 2、装饰器的结构
装饰器也是一个函数，它装饰原函数`f`或类`cls`后，再返回一个函数`g`<br />装饰一个函数：
```python
def decorator(f):
  # 定义要返回的函数
  def g():
    print('函数f执行前的动作')
    f()
    print('函数f执行后的动作')
  return g
```
装饰一个类：
```python
def decorator(cls):
  # 定义要返回的函数
  def g():
    print('类cls执行前的动作')
    f()
    print('类cls执行后的动作')
  return g
```
使用装饰器很简单，`@+自定义装饰器` 装饰要想装饰的函数。
<a name="TFUgH"></a>
### 3、装饰器的作用
要想理解装饰器为什么要有这种结构，要首先想明白装饰器的目标是什么。<br />它的价值在于为原函数`f`增加一些行为，前提必须不能破坏函数`f`，所以肯定不能改变f的内部结构，所以只能在调用f前后定义一些行为。<br />同时，装饰器函数`decorator`返回值又是什么？可以思考下，返回一个函数是再好不过的了，它包装了原函数`f`.
<a name="FPbjD"></a>
### 4、装饰一个函数
`printStar`函数接收一个函数`f`，返回值也是一个函数，所以满足装饰器的结构要求，所以`printStar`是一个装饰器。
```python
def printStar(f):
    def g():
        print('*'*20)
        f()
        print('*'*20)
    return g
```
printStar装饰器实现f函数执行前、后各打印20个*字符。<br />使用printStar:
```python
@printStar
def f():
    print('hello world')
```
调用：
```python
if __name__ == '__main__':
   ### 改变函数功能
   f()
```
打印结果：
```python
********************
hello world
********************
```
可以很方便的装饰要想装饰的其他函数，如下：
```python
@printStar
def g():
    print('welcome to Python')
```
<a name="uTzf9"></a>
### 5、装饰一个类
除了可以装饰函数f外，还可以装饰类`cls`，两者原理都是一样的。<br />下面给出一个装饰器实现单例模式的例子，所谓单例就是类只有唯一实例，不能有第二个。
```python
def singleton(cls):
   instance = {}
   def get_instance(*args, **kwargs):
       if cls not in instance:
           instance[cls] = cls(*args, **kwargs)
       return instance[cls]
   return get_instance
```
定义字典`instance`，键值对分别为类和实例，这样确保只`cls()`一次。<br />使用装饰器`singleton`修饰类：
```python
@singleton
class CorePoint:
   pass
```
测试：
```python
if __name__ == '__main__':
   ### 改变类的功能
   c1 = CorePoint()
   c2 = CorePoint()
   print(c1 is c2) # True
```
<a name="jIdi7"></a>
### 6、装饰器层叠
上面原函数`f`不仅能被一个装饰器修饰，还能被n多个装饰器修饰。<br />下面再定义一个装饰器`printLine`，被修饰函数执行前后打印20个 -
```python
def printLine(f):
    def g():
        print('-'*20)
        f()
        print('-'*20)
    return g
```
使用上文定义好的`printStar`和`printLine`同时装饰函数`f`：
```python
@printStar
@printLine
def f():
    print('hello world')
```
此时再调用函数`f`：
```python
if __name__ == '__main__':
   ### 改变函数功能
   f()
```
打印结果：
```python
********************
--------------------
hello world
--------------------
********************
```
`f`被装饰后，先打印`*`，再打印 -<br />层叠多一层，原函数`f`就变强大一层。使用装饰器，还能实现功能抽离，进一步实现松耦合。
<a name="PdQub"></a>
### 7、使用注意
打印原函数f的名字`__name__`，结果为`f`
```python
In [1]: def f(): 
   ...:     pass 
In [4]: f.__name__                                                              
Out[4]: 'f'
```
但是，被装饰后函数名字f变为g，这不是所期望！
```python
@printStar
def f():
  pass
f()
f.__name__ # g
```
Python提供的解决方案：使用functools模块中的`wraps`装饰器：
```python
from functools import wraps
def printStar(f):
    @wraps(f)
    def g():
        print('*'*20)
        f()
        print('*'*20)
    return g
```
此时再打印被装饰后f的名字，显示f，正常！
<a name="7sWqw"></a>
### 8、装饰器传参
首先有一个这样的装饰器文件路径`helper/log_helper.py`
```python
import traceback
from functools import wraps
from loguru import logger
def my_logger(count):
    def step1(foo):
        @wraps(foo)
        def step2(*args, **kwargs):
            try:
                result = foo(*args, **kwargs)
                logger.info(f"{result=},{count=}")
            except Exception:
                logger.exception(traceback.format_exc())
        return step2
    return step1
```
然后有个文件需要引用这个装饰器`demo.py`
```python
from helper.log_helper import my_logger
class Demo:
    @my_logger(count=2)
    def main(self):
        return "in main function"
if __name__ == '__main__':
    d = Demo()
    d.main()
```
输出结果如下
```
2020-10-16 11:43:12.001 | INFO     | helper.log_helper:step2:18 - result='in main function',count=2
```
这个装饰器的作用很简单，就是获取当前函数的返回值，和传入的count值。<br />好，现在问题来了？如果给装饰器的参数传值呢，也就是说count=2，是通过传值的形式。想到方案可能是这样
```python
from helper.log_helper import my_logger
COUNT=2
class Demo:
    @my_logger(count=COUNT)
    def main(self):
        return "in main function"
if __name__ == '__main__':
    d = Demo()
    d.main()
```
ok，这样确实可以，还可以使用再简化一步
```python
from functools import partial
from helper.log_helper import my_logger
COUNT=2
my_logger = partial(my_logger,count=2)
class Demo:
    @my_logger()
    def main(self):
        return "in main function"
if __name__ == '__main__':
    d = Demo()
    d.main()
```
暂时来看搞定了传参数的问题，这时候如果外界调用了Demo类的main方法，并且向指定count的值怎么办呢？<br />外界调用Demo类传参的唯一途径就是向`__init__`里进行传参数，按照这个思路只能这么写了，
```python
class Demo:
    def __init__(self):
        count =2
    @my_logger(count=self.count)
    def main(self):
        return "in main function"
```
但是这样并不可以，得到错误信息
```
NameError: name 'self' is not defined
```
在装饰器中无法使用`self.`形式的参数,难道这个问题解决不了么?
<a name="0TLMT"></a>
#### 问题解决
在Python3.7之前确实没什么可行的方案。<br />知道在Python3.7的时候引入了dataclasses，可以通过它来简化`__init__`。<br />改下代码
```python
from functools import partial
from helper.log_helper import my_logger
from dataclasses import dataclass
@dataclass()
class Demo:
    count: int = 2
    logger: my_logger = partial(my_logger, count)
    @logger()
    def main(self):
        return "in main function"
if __name__ == '__main__':
    d = Demo()
    d.main()
```
如果使用Python3.8那么可以直接忽略掉dataclass
```python
class Demo:
    count: int = 2
    logger: my_logger = partial(my_logger, count)
    @logger()
    def main(self):
        return "in main function"
```
<a name="u76iv"></a>
### 9、装饰器的其他应用场景
1、代码都写好了，现在要求插入日志。<br />2、代码都写好了，现在要求加入计时功能、性能测试。<br />3、代码都写好了，现在要求一个函数变成事务性操作。<br />4、代码都写好了，现在又要求增加权限验证。
