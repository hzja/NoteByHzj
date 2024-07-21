Python<br />python中的魔法方法是一些可以对类添加“魔法”的特殊方法，它们经常是两个下划线包围来命名的。<br />Python的魔法方法，也称为dunder(双下划线)方法。大多数的时候，将它们用于简单的事情，例如构造函数(`**init**`)、字符串表示(`__str__`， **repr**)或算术运算符(**add**/**mul**)。其实还有许多你可能没有听说过的但是却很好用的方法，在这里整理这些魔法方法!
<a name="hgUM6"></a>
## 迭代器的大小
都知道`__len__`方法，可以用它在容器类上实现`len()`函数。但是，如果想获取实现迭代器的类对象的长度怎么办?
```python
it = iter(range(100))
print(it.__length_hint__())
# 100
next(it)
print(it.__length_hint__())
# 99

a = [1, 2, 3, 4, 5]
it = iter(a)
print(it.__length_hint__())
# 5
next(it)
print(it.__length_hint__())
# 4
a.append(6)
print(it.__length_hint__())
# 5
```
需要做的就是实现`__length_hint__`方法，这个方法是迭代器上的内置方法(不是生成器)，正如上面看到的那样，并且还支持动态长度更改。但是，正如他的名字那样，这只是一个提示（hint），并不能保证完全准确:对于列表迭代器，可以得到准确的结果，但是对于其他迭代器则不确定。但是即使它不准确，它也可以获得需要的信息，正如PEP 424中解释的那样
:::danger
length_hint must return an integer (else a TypeError is raised) or NotImplemented, and is not required to be accurate.  It may return a value that is either larger or smaller than the actual size of the container.  A return value of NotImplemented indicates that there is no finite length estimate.  It may not return a negative value (else a ValueError is raised).
:::
<a name="fSz1t"></a>
## 元编程
大部分很少看到的神奇方法都与元编程有关，虽然元编程可能不是每天都需要使用的东西，但有一些方便的技巧可以使用它。<br />一个这样的技巧是使用`__init_subclass__`作为扩展基类功能的快捷方式，而不必处理元类:
```python
class Pet:
    def __init_subclass__(cls, /, default_breed, **kwargs):
        super().__init_subclass__(**kwargs)
        cls.default_breed = default_breed

class Dog(Pet, default_name="German Shepherd"):
    pass
```
上面的代码向基类添加关键字参数，该参数可以在定义子类时设置。在实际用例中可能会在想要处理提供的参数而不仅仅是赋值给属性的情况下使用此方法。<br />看起来非常晦涩并且很少会用到，但其实你可能已经遇到过很多次了，因为它一般都是在构建API时使用的，例如在SQLAlchemy或Flask Views中都使用到了。<br />另一个元类的神奇方法是`__call__`。这个方法允许自定义调用类实例时发生的事情:
```python
class CallableClass:
    def __call__(self, *args, **kwargs):
        print("I was called!")

instance = CallableClass()

instance()
# I was called!
```
可以用它来创建一个不能被调用的类:
```python
class NoInstances(type):
    def __call__(cls, *args, **kwargs):
        raise TypeError("Can't create instance of this class")

class SomeClass(metaclass=NoInstances):
    @staticmethod
    def func(x):
        print('A static method')

instance = SomeClass()
# TypeError: Can't create instance of this class
```
对于只有静态方法的类，不需要创建类的实例就用到了这个方法。<br />另一个类似的场景是单例模式——一个类最多只能有一个实例：
```python
class Singleton(type):
    def __init__(cls, *args, **kwargs):
        cls.__instance = None
        super().__init__(*args, **kwargs)

    def __call__(cls, *args, **kwargs):
        if cls.__instance is None:
            cls.__instance = super().__call__(*args, **kwargs)
            return cls.__instance
        else:
            return cls.__instance

class Logger(metaclass=Singleton):
    def __init__(self):
        print("Creating global Logger instance")
```
Singleton类拥有一个私有`__instance`——如果没有，它会被创建并赋值，如果它已经存在，它只会被返回。<br />假设有一个类，创建它的一个实例而不调用`__init__`。`__new__` 方法可以帮助解决这个问题:
```python
class Document:
    def __init__(self, text):
        self.text = text

bare_document = Document.__new__(Document)
print(bare_document.text)
# AttributeError: 'Document' object has no attribute 'text'

setattr(bare_document, "text", "Text of the document")
```
在某些情况下，可能需要绕过创建实例的通常过程，上面的代码演示了如何做到这一点。不调用`Document(…)`，而是调用`Document.**new**(Document)`，它创建一个裸实例，而不调用`__init__`。因此，实例的属性(在本例中为text)没有初始化，需要额外使用`setattr`函数赋值(它也是一个魔法的方法`__setattr__`)。<br />为什么要这么做呢。因为可能会想要替代构造函数，比如:
```python
class Document:
    def __init__(self, text):
        self.text = text

    @classmethod
    def from_file(cls, file): # Alternative constructor
        d = cls.__new__(cls)
        # Do stuff...
        return d
```
这里定义`from_file`方法，它作为构造函数，首先使用`__new__`创建实例，然后在不调用`__init__`的情况下配置它。<br />下一个与元编程相关的神奇方法是`__getattr__`。当普通属性访问失败时调用此方法。这可以用来将对缺失方法的访问/调用委托给另一个类:
```python
class String:
    def __init__(self, value):
        self._value = str(value)

    def custom_operation(self):
        pass

    def __getattr__(self, name):
        return getattr(self._value, name)

s = String("some text")
s.custom_operation() # Calls String.custom_operation()
print(s.split()) # Calls String.__getattr__("split") and delegates to str.split
# ['some', 'text']

print("some text" + "more text")
# ... works
print(s + "more text")
# TypeError: unsupported operand type(s) for +: 'String' and 'str'
```
为类添加一些额外的函数(如上面的`custom_operation`)定义string的自定义实现。但是并不想重新实现每一个字符串方法，比如split、join、capitalize等等。这里就可以使用`__getattr__`来调用这些现有的字符串方法。<br />虽然这适用于普通方法，但请注意，在上面的示例中，魔法方法`__add__`（提供的连接等操作）没有得到委托。所以，如果想让它们也能正常工作，就必须重新实现它们。
<a name="aIYCn"></a>
## 自省(introspection)
最后一个与元编程相关的方法是`__getattribute__`。它一个看起来非常类似于前面的`__getattr__`，但是他们有一个细微的区别，`__getattr__`只在属性查找失败时被调用，而`__getattribute__`是在尝试属性查找之前被调用。<br />所以可以使用`__getattribute__`来控制对属性的访问，或者可以创建一个装饰器来记录每次访问实例属性的尝试:
```python
def logger(cls):
    original_getattribute = cls.__getattribute__

    def getattribute(self, name):
        print(f"Getting: '{name}'")
        return original_getattribute(self, name)

    cls.__getattribute__ = getattribute
    return cls

@logger
class SomeClass:
    def __init__(self, attr):
        self.attr = attr

    def func(self):
        ...

instance = SomeClass("value")
instance.attr
# Getting: 'attr'
instance.func()
# Getting: 'func'
```
装饰器函数logger 首先记录它所装饰的类的原始`__getattribute__`方法。然后将其替换为自定义方法，该方法在调用原始的`__getattribute__`方法之前记录了被访问属性的名称。
<a name="Pwjgn"></a>
## 魔法属性
到目前为止，只讨论了魔法方法，但在Python中也有相当多的魔法变量/属性。其中一个是`__all__`：
```python
# some_module/__init__.py
__all__ = ["func", "some_var"]

some_var = "data"
some_other_var = "more data"

def func():
    return "hello"

    # -----------

from some_module import *

print(some_var)
# "data"
print(func())
# "hello"

print(some_other_var)
# Exception, "some_other_var" is not exported by the module
```
这个属性可用于定义从模块导出哪些变量和函数。创建了一个Python模块…/some_module/单独文件(**init**.py)。在这个文件中定义了2个变量和一个函数，只导出其中的2个(func和some_var)。如果尝试在其他Python程序中导入some_module的内容，只能得到2个内容。<br />但是要注意，`__all__`变量只影响上面所示的`* import`，仍然可以使用显式的名称导入函数和变量，比如`import some_other_var from some_module`。<br />另一个常见的双下划线变量(模块属性)是`__file__`。这个变量标识了访问它的文件的路径:
```python
from pathlib import Path

print(__file__)
print(Path(__file__).resolve())
# /home/.../directory/examples.py

# Or the old way:
import os
print(os.path.dirname(os.path.abspath(__file__)))
# /home/.../directory/
```
这样就可以结合`__all__`和`__file__`，可以在一个文件夹中加载所有模块:
```python
# Directory structure:
# .
# |____some_dir
#   |____module_three.py
#   |____module_two.py
#   |____module_one.py

from pathlib import Path, PurePath
modules = list(Path(__file__).parent.glob("*.py"))
print([PurePath(f).stem for f in modules if f.is_file() and not f.name == "__init__.py"])
# ['module_one', 'module_two', 'module_three']
```
最后一个重要的属性是的是`__debug__`。它可以用于调试，但更具体地说，它可以用于更好地控制断言:
```python
# example.py
def func():
    if __debug__:
        print("debugging logs")

    # Do stuff...

func()
```
如果使用`python example.py`正常运行这段代码，将看到打印出“调试日志”，但是如果使用`python -O example.py`，优化标志(-O)将把`__debug__`设置为false并删除调试消息。因此，如果在生产环境中使用-O运行代码，就不必担心调试过程中被遗忘的打印调用，因为它们都不会显示。
<a name="Auegb"></a>
## 创建自己魔法方法？
可以创建自己的方法和属性吗?是的，可以这样做，但不应该这么做。<br />双下划线名称是为Python语言的未来扩展保留的，不应该用于自己的代码。如果决定在代码中使用这样的名称，那么将来如果它们被添加到Python解释器中，这就与你的代码不兼容了。所以对于这些方法，只要记住和使用就好了。
