Python 元编程<br />提到元这个字，也许会想到元数据，元数据就是描述数据本身的数据，元类就是类的类，相应的元编程就是描述代码本身的代码，元编程就是关于创建操作源代码(比如修改、生成或包装原来的代码)的函数和类。主要技术是使用装饰器、元类、描述符类。<br />这里主要目的是向大家介绍这些元编程技术，并且给出实例来演示它们是怎样定制化源代码的行为。

<a name="pyZN8"></a>
## 装饰器
装饰器就是函数的函数，它接受一个函数作为参数并返回一个新的函数，在不改变原来函数代码的情况下为其增加新的功能，比如最常用的计时装饰器：
```python
from functools import wraps

def timeit(logger=None):
    """
    耗时统计装饰器，单位是秒，保留 4 位小数
    """

    def decorator(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            start = time.time()
            result = func(*args, **kwargs)
            end = time.time()
            if logger:
                logger.info(f"{func.__name__} cost {end - start :.4f} seconds")
            else:
                print(f"{func.__name__} cost {end - start :.4f} seconds")
            return result

        return wrapper

    return decorator
```
(注：比如上面使用 `@wraps(func)` 注解是很重要的， 它能保留原始函数的元数据) 只需要在原来的函数上面加上 `@timeit()` 即可为其增加新的功能：
```python
@timeit()
def test_timeit():
    time.sleep(1)

test_timeit()
#test_timeit cost 1.0026 seconds
```
上面的代码跟下面这样写的效果是一样的：
```python
test_timeit = timeit(test_timeit)
test_timeit()
```
<a name="mIYy7"></a>
## 装饰器的执行顺序
当有多个装饰器的时候，他们的调用顺序是怎么样的？<br />假如有这样的代码，请问是先打印 Decorator1 还是 Decorator2 ?
```python
from functools import wraps

def decorator1(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        print('Decorator 1')
        return func(*args, **kwargs)
    return wrapper

def decorator2(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        print('Decorator 2')
        return func(*args, **kwargs)
    return wrapper

@decorator1
@decorator2
def add(x, y):
    return x + y

add(1,2)

# Decorator 1
# Decorator 2
```
回答这个问题之前，先打个形象的比喻，装饰器就像函数在穿衣服，离它最近的最先穿，离得远的最后穿，上例中 decorator1 是外套，decorator2 是内衣。
```python
add = decorator1(decorator2(add))
```
在调用函数的时候，就像脱衣服，先解除最外面的 decorator1，也就是先打印 Decorator1，执行到 `return func(*args, **kwargs)` 的时候会去解除 decorator2，然后打印 Decorator2，再次执行到 `return func(*args, **kwargs)` 时会真正执行 `add()` 函数。<br />需要注意的是打印的位置，如果打印字符串的代码位于调用函数之后，像下面这样，那输出的结果正好相反：
```python
def decorator1(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        print('Decorator 1')
        return result
    return wrapper

def decorator2(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        print('Decorator 2')
        return result
    return wrapper
```
装饰器不仅可以定义为函数，也可以定义为类，只要确保它实现了`__call__()` 和 `__get__()` 方法。
<a name="RVvCP"></a>
## 元类
Python 中所有类（object）的元类，就是 `type` 类，也就是说 Python 类的创建行为由默认的 `type` 类控制，打个比喻，`type` 类是所有类的祖先。可以通过编程的方式来实现自定义的一些对象创建行为。<br />定一个类继承 `type` 类 A，然后让其他类的元类指向 A，就可以控制 A 的创建行为。典型的就是使用元类实现一个单例：
```python
class Singleton(type):
    def __init__(self, *args, **kwargs):
        self._instance = None
        super().__init__(*args, **kwargs)

    def __call__(self, *args, **kwargs):
        if self._instance is None:
            self._instance = super().__call__(*args, **kwargs)
            return self._instance
        else:
            return self._instance


class Spam(metaclass=Singleton):
    def __init__(self):
        print("Spam!!!")
```
元类 `Singleton` 的`__init__`和`__new__` 方法会在定义 Spam 的期间被执行，而 `__call__`方法会在实例化 `Spam` 的时候执行。
<a name="J4Q0Z"></a>
## `descriptor` 类（描述符类）
descriptor 就是任何一个定义了 `__get__()`，`__set__()`或 `__delete__()`的对象，描述器让对象能够自定义属性查找、存储和删除的操作。这里举[官方文档](https://docs.python.org/zh-cn/3/howto/descriptor.html)一个自定义验证器的例子。<br />定义验证器类，它是一个描述符类，同时还是一个抽象类：
```python
from abc import ABC, abstractmethod

class Validator(ABC):

    def __set_name__(self, owner, name):
        self.private_name = '_' + name

    def __get__(self, obj, objtype=None):
        return getattr(obj, self.private_name)

    def __set__(self, obj, value):
        self.validate(value)
        setattr(obj, self.private_name, value)

    @abstractmethod
    def validate(self, value):
        pass
```
自定义验证器需要从 Validator 继承，并且必须提供 `validate()` 方法以根据需要测试各种约束。<br />这是三个实用的数据验证工具：<br />OneOf 验证值是一组受约束的选项之一。
```python
class OneOf(Validator):

    def __init__(self, *options):
        self.options = set(options)

    def validate(self, value):
        if value not in self.options:
            raise ValueError(f'Expected {value!r} to be one of {self.options!r}')
```
Number 验证值是否为 int 或 float。根据可选参数，它还可以验证值在给定的最小值或最大值之间。
```python
class Number(Validator):

    def __init__(self, minvalue=None, maxvalue=None):
        self.minvalue = minvalue
        self.maxvalue = maxvalue

    def validate(self, value):
        if not isinstance(value, (int, float)):
            raise TypeError(f'Expected {value!r} to be an int or float')
        if self.minvalue is not None and value < self.minvalue:
            raise ValueError(
                f'Expected {value!r} to be at least {self.minvalue!r}'
            )
        if self.maxvalue is not None and value > self.maxvalue:
            raise ValueError(
                f'Expected {value!r} to be no more than {self.maxvalue!r}'
            )
```
String 验证值是否为 str。根据可选参数，它可以验证给定的最小或最大长度。它还可以验证用户定义的 predicate。
```python
class String(Validator):

    def __init__(self, minsize=None, maxsize=None, predicate=None):
        self.minsize = minsize
        self.maxsize = maxsize
        self.predicate = predicate

    def validate(self, value):
        if not isinstance(value, str):
            raise TypeError(f'Expected {value!r} to be an str')
        if self.minsize is not None and len(value) < self.minsize:
            raise ValueError(
                f'Expected {value!r} to be no smaller than {self.minsize!r}'
            )
        if self.maxsize is not None and len(value) > self.maxsize:
            raise ValueError(
                f'Expected {value!r} to be no bigger than {self.maxsize!r}'
            )
        if self.predicate is not None and not self.predicate(value):
            raise ValueError(
                f'Expected {self.predicate} to be true for {value!r}'
            )
```
实际应用时这样写：
```python
class Component:

    name = String(minsize=3, maxsize=10, predicate=str.isupper)
    kind = OneOf('wood', 'metal', 'plastic')
    quantity = Number(minvalue=0)

    def __init__(self, name, kind, quantity):
        self.name = name
        self.kind = kind
        self.quantity = quantity
```
描述器阻止无效实例的创建：
```python
>>> Component('Widget', 'metal', 5)      # Blocked: 'Widget' is not all uppercase
Traceback (most recent call last):
    ...
ValueError: Expected <method 'isupper' of 'str' objects> to be true for 'Widget'

>>> Component('WIDGET', 'metle', 5)      # Blocked: 'metle' is misspelled
Traceback (most recent call last):
    ...
ValueError: Expected 'metle' to be one of {'metal', 'plastic', 'wood'}

>>> Component('WIDGET', 'metal', -5)     # Blocked: -5 is negative
Traceback (most recent call last):
    ...
ValueError: Expected -5 to be at least 0
>>> Component('WIDGET', 'metal', 'V')    # Blocked: 'V' isn't a number
Traceback (most recent call last):
    ...
TypeError: Expected 'V' to be an int or float

>>> c = Component('WIDGET', 'metal', 5)  # Allowed:  The inputs are valid
```
<a name="o6z6M"></a>
## 最后的话
关于 Python 的元编程，总结如下：<br />如果希望某些函数拥有相同的功能，希望不改变原有的调用方式、不写重复代码、易维护，可以使用装饰器来实现。<br />如果希望某一些类拥有某些相同的特性，或者在类定义实现对其的控制，可以自定义一个元类，然后让它类的元类指向该类。<br />如果希望实例的属性拥有某些共同的特点，就可以自定义一个描述符类。
