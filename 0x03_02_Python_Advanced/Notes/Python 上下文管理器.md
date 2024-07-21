Python <br />在 Python 开发中，经常会使用到 `with` 语法块，例如在读写文件时，保证文件描述符的正确关闭，避免资源泄露问题。<br />有没有思考过， `with` 背后是如何实现的？常常听到的上下文管理器究竟是什么？
<a name="ChGiU"></a>
# with语法块
在讲解 `with` 语法之前，先来看一下不使用 `with` 的代码如何写？<br />在操作一个文件时，代码可以这么写：
```python
# 打开文件
f = open('file.txt')
for line in f:
    # 读取文件内容 执行其他操作
    # do_something...
# 关闭文件
f.close()
```
这个例子非常简单，就是打开一个文件，然后读取文件中的内容，最后关闭文件释放资源。<br />但是，代码这么写会有一个问题：在打开文件后，如果要对读取到的内容进行其他操作，在这操作期间发生了异常，这就会导致文件句柄无法被释放，进而导致资源的泄露。<br />如何解决这个问题？<br />也很简单，使用 `try ... finally` 来优化代码：
```python
# 打开文件
f = open('file.txt')
try:
    for line in f:
        # 读取文件内容 执行其他操作
        # do_something...
finally:
    # 保证关闭文件
    f.close()
```
这么写的好处是，在读取文件内容和操作期间，无论是否发生异常，都可以保证最后能释放文件资源。<br />但这么优化，代码结构会变得很繁琐，每次都要给代码逻辑增加 `try ... finally` 才可以，可读性变得很差。<br />针对这种情况，就可以使用 `with` 语法块来解决这个问题：
```python
with open('file.txt') as f:
    for line in f:
        # do_something...
```
使用 `with` 语法块可以完成之前相同的功能，而且这么写的好处是，代码结构变得非常清晰，可读性也很好。<br />明白了 `with` 的作用，那么 `with` 究竟是如何运行的呢？
<a name="UP0LM"></a>
# 上下文管理器
首先来看一下 `with` 的语法格式：
```python
with context_expression [as target(s)]:
    with-body
```
`with` 语法非常简单，只需要 `with` 一个表达式，然后就可以执行自定义的业务逻辑。<br />但是，`with` 后面的表达式是可以任意写的吗？<br />答案是否定的。要想使用 `with` 语法块，`with` 后面的的对象需要实现「上下文管理器协议」。<br />什么是「上下文管理器协议」？<br />一个类在 Python 中，只要实现以下方法，就实现了「上下文管理器协议」：

- `__enter__`：在进入 `with` 语法块之前调用，返回值会赋值给 `with` 的 `target`
- `__exit__`：在退出 `with` 语法块时调用，一般用作异常处理

来看实现了这 2 个方法的例子：
```python
class TestContext:

    def __enter__(self):
        print('__enter__')
        return 1

    def __exit__(self, exc_type, exc_value, exc_tb):
        print('exc_type: %s' % exc_type)
        print('exc_value: %s' % exc_value)
        print('exc_tb: %s' % exc_tb)

with TestContext() as t:
    print('t: %s' % t)
    
# Output:
# __enter__
# t: 1
# exc_type: None
# exc_value: None
# exc_tb: None

```
在这个例子中，定义了 `TestContext` 类，它分别实现了 `__enter__` 和 `__exit__` 方法。<br />这样一来，就可以把 `TestContext` 当做一个「上下文管理器」来使用，也就是通过 `with TestContext() as t` 方式来执行。<br />从输出结果可以看到，具体的执行流程如下：

- `__enter__` 在进入 `with` 语句块之前被调用，这个方法的返回值赋给了 `with` 后的 `t` 变量
- `__exit__` 在执行完 `with` 语句块之后被调用

如果在 `with` 语句块内发生了异常，那么 `__exit__` 方法可以拿到关于异常的详细信息：

- `exc_type`：异常类型
- `exc_value`：异常对象
- `exc_tb`：异常堆栈信息

来看一个发生异常的例子，观察 `__exit__` 方法拿到的异常信息是怎样的：
```python
with TestContext() as t:
    # 这里会发生异常
    a = 1 / 0 
    print('t: %s' % t)

# Output:
# __enter__
# exc_type: <type 'exceptions.ZeroDivisionError'>
# exc_value: integer division or modulo by zero
# exc_tb: <traceback object at 0x10d66dd88>
# Traceback (most recent call last):
#   File "base.py", line 16, in <module>
#     a = 1 / 0
# ZeroDivisionError: integer division or modulo by zero
```
从输出结果可以看到，当 `with` 语法块内发生异常后，`__exit__` 输出了这个异常的详细信息，其中包括异常类型、异常对象、异常堆栈。<br />如果需要对异常做特殊处理，就可以在这个方法中实现自定义逻辑。<br />回到最开始讲的，使用 `with` 读取文件的例子。之所以 `with` 能够自动关闭文件资源，就是因为内置的文件对象实现了「上下文管理器协议」，这个文件对象的 `__enter__` 方法返回了文件句柄，并且在 `__exit__` 中实现了文件资源的关闭，另外，当 `with` 语法块内有异常发生时，会抛出异常给调用者。<br />伪代码可以这么写：
```python
class File:

    def __enter__(self):
        return file_obj

    def __exit__(self, exc_type, exc_value, exc_tb):
        # with 退出时释放文件资源
        file_obj.close()
        # 如果 with 内有异常发生 抛出异常
        if exc_type is not None:
            raise exception
```
这里小结一下，通过对 `with` 的学习，了解了 `with` 非常适合用需要对于上下文处理的场景，例如操作文件、Socket，这些场景都需要在执行完业务逻辑后，释放资源。
<a name="zmX2A"></a>
# contextlib模块
对于`需要`上下文管理的场景，除了自己实现 `__enter__` 和 `__exit__` 之外，还有更简单的方式来做吗？<br />答案是肯定的。可以使用 Python 标准库提供的 `contextlib` 模块，来简化代码。<br />使用 `contextlib` 模块，可以把上下文管理器当成一个「装饰器」来使用。<br />其中，`contextlib` 模块提供了 `contextmanager` 装饰器和 `closing` 方法。<br />下面通过例子来看一下它们是如何使用的。
<a name="nebI7"></a>
## `contextmanager`装饰器
先来看 `contextmanager` 装饰器的使用：
```python
from contextlib import contextmanager

@contextmanager
def test():
    print('before')
    yield 'hello'
    print('after')

with test() as t:
    print(t)

# Output:
# before
# hello
# after
```
在这个例子中，使用 `contextmanager` 装饰器和 `yield`配合，实现了和前面上下文管理器相同的功能，它的执行流程如下：

1. 执行 `test()` 方法，先打印出 `before`
2. 执行 `yield 'hello'`，`test` 方法返回，`hello` 返回值会赋值给 `with` 语句块的 `t` 变量
3. 执行 `with` 语句块内的逻辑，打印出 `t` 的值 `hello`
4. 又回到 `test` 方法中，执行 `yield` 后面的逻辑，打印出 `after`

这样一来，当使用这个 `contextmanager` 装饰器后，就不用再写一个类来实现上下文管理协议，只需要用一个方法装饰对应的方法，就可以实现相同的功能。<br />不过有一点需要注意：在使用 `contextmanager` 装饰器时，如果被装饰的方法内发生了异常，那么需要在自己的方法中进行异常处理，否则将不会执行 `yield` 之后的逻辑。
```python
@contextmanager
def test():
    print('before')
    try:
        yield 'hello'
        # 这里发生异常 必须自己处理异常逻辑 否则不会向下执行
        a = 1 / 0 
    finally:
        print('after')

with test() as t:
    print(t)
```
<a name="vBWaN"></a>
## closing方法
再来看 `contextlib` 提供的 `closing` 方法如何使用。<br />`closing` 主要用在已经实现 `close` 方法的资源对象上：
```python
from contextlib import closing

class Test():

    # 定义了 close 方法才可以使用 closing 装饰器
    def close(self):
        print('closed')

# with 块执行结束后 自动执行 close 方法
with closing(Test()):
    print('do something')
    
# Output:
# do something
# closed
```
从执行结果可以看到，`with` 语句块执行结束后，会自动调用 `Test` 实例的 `close` 方法。<br />所以，对于需要自定义关闭资源的场景，可以使用这个方法配合 `with` 来完成。
<a name="lYC1a"></a>
## `contextlib`的实现
学习完了 `contextlib` 模块的使用，最后来看一下 `contextlib` 模块是究竟是如何实现的？<br />`contextlib` 模块相关的源码如下：
```python
class _GeneratorContextManagerBase:

    def __init__(self, func, args, kwds):
        # 接收一个生成器对象 (方法内包含 yield 的方法就是一个生成器)
        self.gen = func(*args, **kwds)
        self.func, self.args, self.kwds = func, args, kwds
        doc = getattr(func, "__doc__", None)
        if doc is None:
            doc = type(self).__doc__
        self.__doc__ = doc

class _GeneratorContextManager(_GeneratorContextManagerBase,
                               AbstractContextManager,
                               ContextDecorator):

    def __enter__(self):
        try:
            # 执行生成器 代码会运行生成器方法的 yield 处
            return next(self.gen)
        except StopIteration:
            raise RuntimeError("generator didn't yield") from None

    def __exit__(self, type, value, traceback):
        # with 内没有异常发生
        if type is None:
            try:
                # 继续执行生成器
                next(self.gen)
            except StopIteration:
                return False
            else:
                raise RuntimeError("generator didn't stop")
        # with 内发生了异常
        else:
            if value is None:
                value = type()
            try:
                # 抛出异常
                self.gen.throw(type, value, traceback)
            except StopIteration as exc:
                return exc is not value
            except RuntimeError as exc:
                if exc is value:
                    return False
                if type is StopIteration and exc.__cause__ is value:
                    return False
                raise
            except:
                if sys.exc_info()[1] is value:
                    return False
                raise
            raise RuntimeError("generator didn't stop after throw()")

def contextmanager(func):
    @wraps(func)
    def helper(*args, **kwds):
        return _GeneratorContextManager(func, args, kwds)
    return helper

class closing(AbstractContextManager):
    def __init__(self, thing):
        self.thing = thing
    def __enter__(self):
        return self.thing
    def __exit__(self, *exc_info):
        self.thing.close()
```
源码中已经添加好了注释，可以详细看一下。<br />`contextlib` 源码中逻辑其实比较简单，其中 `contextmanager` 装饰器实现逻辑如下：

1. 初始化一个 `_GeneratorContextManager` 类，构造方法接受了一个生成器 `gen`
2. 这个类实现了上下文管理器协议 `__enter__` 和 `__exit__`
3. 执行 `with` 时会进入到 `__enter__` 方法，然后执行这个生成器，执行时会运行到 `with` 语法块内的 `yield` 处
4. `__enter__` 返回 `yield` 的结果
5. 如果 `with` 语法块没有发生异常，`with` 执行结束后，会进入到 `__exit__` 方法，再次执行生成器，这时会运行 `yield` 之后的代码逻辑
6. 如果 `with` 语法块发生了异常，`__exit__` 会把这个异常通过生成器，传入到 `with` 语法块内，也就是把异常抛给调用者

再来看 `closing` 的实现，`closing` 方法就是在 `__exit__` 方法中调用了自定义对象的 `close`，这样当 `with` 结束后就会执行定义的 `close` 方法。
<a name="j00vB"></a>
# 使用场景
学完了上下文管理器，那么它们具体会用在什么场景呢？<br />下面举几个常用的例子来演示下，可以参考一下结合自己的场景使用。
<a name="aYS7r"></a>
## Redis分布式锁
```python
from contextlib import contextmanager

@contextmanager
def lock(redis, lock_key, expire):
    try:
        locked = redis.set(lock_key, 'locked', expire)
        yield locked
    finally:
        redis.delete(lock_key)

# 业务调用 with 代码块执行结束后 自动释放锁资源
with lock(redis, 'locked', 3) as locked:
    if not locked:
        return
    # do something ...
```
在这个例子中，实现了 `lock` 方法，用于在 Redis 上申请一个分布式锁，然后使用 `contextmanager` 装饰器装饰了这个方法。<br />之后业务在调用 `lock` 方法时，就可以使用 `with` 语法块了。<br />`with` 语法块的第一步，首先判断是否申请到了分布式锁，如果申请失败，则业务逻辑直接返回。如果申请成功，则执行具体的业务逻辑，当业务逻辑执行完成后，`with` 退出时会自动释放分布式锁，就不需要每次都手动释放锁了。
<a name="AdIPb"></a>
## Redis事物和管道
```python
from contextlib import contextmanager

@contextmanager
def pipeline(redis):
    pipe = redis.pipeline()
    try:
        yield pipe
        pipe.execute()
    except Exception as exc:
        pipe.reset()
            
# 业务调用 with 代码块执行结束后 自动执行 execute 方法
with pipeline(redis) as pipe:
    pipe.set('key1', 'a', 30)
    pipe.zadd('key2', 'a', 1)
    pipe.sadd('key3', 'a')
```
在这个例子中，定义了 `pipeline` 方法，并使用装饰器 `contextmanager` 让它变成了一个上下文管理器。<br />之后在调用 `with pipeline(redis) as pipe` 时，就可以开启一个事物和管道，然后在 `with` 语法块内向这个管道中添加命令，最后 `with` 退出时会自动执行 `pipeline` 的 `execute` 方法，把这些命令批量发送给 Redis 服务端。<br />如果在执行命令时发生了异常，则会自动调用 `pipeline` 的 `reset` 方法，放弃这个事物的执行。
<a name="YuoKO"></a>
# 总结
主要介绍了 Python 上下文管理器的使用及实现。<br />首先介绍了不使用 `with` 和使用 `with` 操作文件的代码差异，然后了解到使用 `with` 可以让代码结构更加简洁。之后探究了 `with` 的实现原理，只要实现 `__enter__` 和 `__exit__` 方法的实例，就可以配合 `with` 语法块来使用。<br />之后介绍了 Python 标准库的 `contextlib` 模块，它提供了实现上下文管理更好的使用方式，可以使用 `contextmanager` 装饰器和 `closing` 方法来操作资源。
