Python 装饰器
<a name="OO0qi"></a>
## 装饰器
装饰器(decorators)是一个可以用于改变一个 Python 函数对象行为的函数。它们可以应用于类和函数，可以做很多非常有趣的事情！<br />装饰器可以用来缩短代码、加速代码并彻底改变代码在 Python 中的行为方式。
<a name="IvamC"></a>
## 1、`@lru_cache`
此列表中的第一个装饰器来自 functools 模块。<br />该模块包含在标准库中，非常易于使用。它还包含比这个装饰器更酷的功能。<br />此装饰器可用于使用缓存加速函数的连续运行。当然，这应该在使用时记住一些关于缓存的注意事项，但在通用使用情况下，大多数时候这个装饰器是值得使用的。<br />能够用一个简单的装饰器来加速代码是非常棒的。<br />可以从这样的装饰器中受益的函数的一个很好的例子是递归函数，例如计算阶乘的函数：
```python
def factorial(n):
    return n * factorial(n-1) if n else 1
```
递归在计算时间上可能非常困难，但添加此装饰器有助于显着加快此函数的连续运行速度。
```python
@lru_cache
def factorial(n):
    return n * factorial(n-1) if n else 1
```
现在每当运行这个函数时，前几个阶乘计算将被保存到缓存中。<br />因此，下次调用该函数时，只需要计算之前使用的阶乘之后的阶乘。<br />当然，并不是所有的阶乘计算都会被保存，但是很容易理解为什么这个装饰器的一个很好的应用程序来加速一些自然很慢的代码。
<a name="iLpt6"></a>
## 2、`@jit`
JIT 是即时编译(Just In Time)的缩写。通常每当在 Python 中运行一些代码时，发生的第一件事就是编译。<br />这种编译会产生一些开销，因为类型被分配了内存，并存储为未分配但已命名的别名。使用即时编译，在执行时才进行编译。<br />在很多方面，可以将其视为类似于并行计算的东西，其中 Python 解释器同时处理两件事以节省一些时间。<br />Numba JIT 编译器因将这一概念提供到 Python 中而闻名。与`@lru_cache` 类似，可以非常轻松地调用此装饰器，并立即提高代码的性能。Numba 包提供了 jit 装饰器，它使运行更密集的软件变得更加容易，而不必进入 C。<br />以下案例使用`@jit` 装饰器加速蒙特卡洛方法计算。
```python
from numba import jit
import random

@jit(nopython=True)
def monte_carlo_pi(nsamples):
    acc = 0
    for i in range(nsamples):
        x = random.random()
        y = random.random()
        if (x ** 2 + y ** 2) < 1.0:
            acc += 1
    return 4.0 * acc / nsamples
```
<a name="kHlPu"></a>
## 3、`@do_twice`
do_twice 装饰器的功能与它的名字差不多。此装饰器可用于通过一次调用运行两次函数。这当然有一些用途，它对调试特别有用。<br />它可以用于测量两个不同迭代的性能。以 Functools 为例，可以让一个函数运行两次，以检查是否有改进。该函数由 Python 中的装饰器模块提供，该模块位于标准库中。
```python
from decorators import do_twice
@do_twice
def timerfunc():
%timeit factorial(15)
```
<a name="YSR4u"></a>
## 4、`@count_calls`
count_calls 装饰器可用于提供有关函数在软件中使用多少次的信息。<br />像 do_twice 一样，这当然可以在调试时派上用场。<br />当添加到给定的函数时，将收到一个输出，该函数每次运行时已经运行了多少次。这个装饰器也在标准库的装饰器模块中。
```python
from decorators import count_calls
@count_calls
def function_example():
	print("Hello World!")
  
function_example()
function_example()
function_example()
```
<a name="gGxNV"></a>
## 5、`@dataclass`
这个装饰器可用于快速编写类中常见的标准方法，这些方法通常会在编写的类中找到。<br />这个装饰器来自 `dataclass` 模块。这个模块也在标准库中，所以不需要 PIP 来尝试这个例子！
```python
from dataclasses import dataclass

@dataclass
class Food:
    name: str
    unit_price: float
    stock: int = 0

    def stock_value(self) -> float:
        return(self.stock * self.unit_price)
```
这段代码将自动创建一个初始化函数 `init()`，其中包含填充类中数据所需的位置参数。<br />它们也将自动提供给 `self`，因此无需编写一个很长的函数来将一些数据参数放入类中。
<a name="Visn6"></a>
## 6、`@singleton`
为了理解单例装饰器的用途，首先需要了解单例(`singleton`)是什么。从某种意义上说，单例是全局变量类型的一个版本。<br />这意味着类型被定义为只存在一次。尽管这些在 C++ 等语言中很常见，但在 Python 中却很少见到。使用单例，可以创建一个只使用一次的类并改变类，而不是通过初始化来构造新的类型。<br />通常，单例装饰器是由用户自己编写的，实际上并不是导入的。<br />这是因为单例仍然是对单例装饰器中提供的模板的引用。可以命名一个单例函数并编写一个包装器，以便在类上使用这个装饰器：
```python
def singleton(cls):
	instances = {}
def wrapper(*args, \*\*kwargs):
    if cls not in instances:
    	instances[cls] = cls(*args, \*\*kwargs)
    return instances[cls]
return wrapper

@singleton
class cls:
	def func(self):
```
另一种方法是使用元类!
<a name="QfNOC"></a>
## 7、`@use_unit`
在科学计算中经常派上用场的一种装饰器是 `@use_unit` 装饰器。<br />此装饰器可用于更改返回结果的表示单位。这对于那些不想在数据中添加度量单位但仍希望人们知道这些单位是什么的人很有用。<br />这个装饰器也不是在任何模块中真正可用，但它是非常常见的，对科学应用程序非常有用。
```python
def use_unit(unit):
    """Have a function return a Quantity with given unit"""
use_unit.ureg = pint.UnitRegistry()
def decorator_use_unit(func):
@functools.wraps(func)
def wrapper_use_unit(*args, \*\*kwargs):
    value = func(*args, \*_kwargs)
return value _ use_unit.ureg(unit)
return wrapper_use_unit
return decorator_use_unit

@use_unit("meters per second")
def average_speed(distance, duration):
    return distance / duration
```
<a name="vkASR"></a>
## 8、`@singledispatch`
`Functools` 凭借非常有用的`@singledispatch` 装饰器再次在此列表中脱颖而出。<br />单调度是一种编程技术，在许多编程语言中都很常见，因为它是一种非常棒的编程方式。单调度可以在很多方面扮演相同的角色。<br />这个装饰器使得在 Python 中使用多类型数据变得更加容易, 尤其当希望通过同一方法传递多种类型数据时，情况更是如此。
```python
@singledispatch
def fun(arg, verbose=False):
    if verbose:
        print("Let me just say,", end=" ")
        print(arg)

@fun.register
def \_(arg: int, verbose=False):
    if verbose:
        print("Strength in numbers, eh?", end=" ")
        print(arg)

@fun.register
def \_(arg: list, verbose=False):
    if verbose:
        print("Enumerate this:")
    for i, elem in enumerate(arg):
        print(i, elem)
```
<a name="b1prN"></a>
## 9、任务超时退出
日常在使用的各种网络请求库时都带有 `timeout` 参数，例如：`request` 库<br />这个参数可以使请求超时就不再继续了，直接抛出超时错误，避免等太久<br />如果自己开发的方法也希望增加这个功能，该如何做呢？<br />方法很多，但最简单直接的是使用并发库 `futures`，为了使用方便，将其封装成了一个装饰器，代码如下：
```python
import functools
from concurrent import futures

executor = futures.ThreadPoolExecutor(1)

def timeout(seconds):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kw):
            future = executor.submit(func, *args, **kw)
            return future.result(timeout=seconds)
        return wrapper
    return decorator
```
定义了以上函数，就有了一个超时结束的装饰器，下面可以测试一下：
```python
import time

@timeout(1)
def task(a, b):
    time.sleep(1.2)
    return a+b

task(2, 3)
```
结果：
```python
---------------------------------------------------------------------------
TimeoutError                              Traceback (most recent call last)
...
D:\Anaconda3\lib\concurrent\futures\_base.py in result(self, timeout)
    432                 return self.__get_result()
    433             else:
--> 434                 raise TimeoutError()
    435 
    436     def exception(self, timeout=None):

TimeoutError:
```
上面通过装饰器定义了函数的超时时间为 1 秒，通过睡眠模拟函数执行超过 1 秒时，成功的抛出了超时异常<br />程序能够在超时时间内完成时：
```python
@timeout(1)
def task(a, b):
    time.sleep(0.9)
    return a+b

task(2, 3)
```
结果：
```python
5
```
可以看到，顺利的得到了结果<br />这样就可以通过一个装饰器给任何函数增加超时时间，这个函数在规定时间内还处理不完就可以直接结束任务<br />前面将这个装饰器将所需的变量定义到了外部，其实还可以通过类装饰器进一步封装，代码如下：
```python
import functools
from concurrent import futures

class timeout:
    __executor = futures.ThreadPoolExecutor(1)

    def __init__(self, seconds):
        self.seconds = seconds

    def __call__(self, func):
        @functools.wraps(func)
        def wrapper(*args, **kw):
            future = timeout.__executor.submit(func, *args, **kw)
            return future.result(timeout=self.seconds)
        return wrapper
```
经测试使用类装饰器能得到同样的效果。
:::warning
注意：使用` @functools.wraps` 的目的是因为被装饰的 `func` 函数元信息会被替换为 `wrapper` 函数的元信息，而 `@functools.wraps(func)` 将 `wrapper` 函数的元信息替换为 `func` 函数的元信息。最终虽然返回的是 `wrapper` 函数，元信息却依然是原有的 `func` 函数
:::
在函数式编程中，函数的返回值是函数对象被称为闭包
<a name="s558y"></a>
## 10、日志记录
如果需要记录部分函数的执行时间，函数执行前后打印一些日志，装饰器是一种很方便的选择<br />代码如下：
```python
import time
import functools
 
def log(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        res = func(*args, **kwargs)
        end = time.perf_counter()
        print(f'函数 {func.__name__} 耗时 {(end - start) * 1000} ms')
        return res
    return wrapper
```
装饰器 log 记录某个函数的运行时间，并返回其执行结果<br />测试一下：
```python
@log
def now():
    print('2021-7-1')
    
now()
```
结果：
```python
2021-7-1
```
函数 now 耗时 0.09933599994838005 ms
<a name="Pssq6"></a>
## 11、缓存
如果经常调用一个函数，而且参数经常会产生重复，如果把结果缓存起来，下次调用同样参数时就会节省处理时间<br />定义函数：
```python
import math
import random
import time

def task(x):
    time.sleep(0.01)
    return round(math.log(x**3 / 15), 4)
```
执行：
```python
%%time
for i in range(500):
    task(random.randrange(5, 10))
```
结果：
```python
Wall time: 5.01 s
```
此时如果使用缓存的效果就会大不一样，实现缓存的装饰器有很多，这里使用 
```python
functools 包下的 LRU 缓存：
from functools import lru_cache

@lru_cache()
def task(x):
    time.sleep(0.01)
    return round(math.log(x**3 / 15), 4)
```
执行：
```python
%%time
for i in range(500):
    task(random.randrange(5, 10))
```
结果：
```python
Wall time: 50 ms
```
<a name="hKEbJ"></a>
## 12、约束某个函数的可执行次数
如果希望程序中的某个函数在整个程序的生命周期中只执行一次或 N 次，可以写一个这样的装饰器：
```python
import functools

class allow_count:
    def __init__(self, count):
        self.count = count
        self.i = 0

    def __call__(self, func):
        @functools.wraps(func)
        def wrapper(*args, **kw):
            if self.i >= self.count:
                return
            self.i += 1
            return func(*args, **kw)
        return wrapper
```
测试：
```python
@allow_count(3)
def job(x):
    x += 1
    return x

for i in range(5):
    print(job(i))
```
结果：
```python
1
2
3
None
None
```
