Python<br />函数重载指的是有多个同名的函数，但是它们的签名或实现却不同。当调用一个重载函数 fn 时，程序会检验传递给函数的实参/形参，并据此而调用相应的实现。
```cpp
int area(int length, int breadth) {
  return length * breadth;
}

float area(int radius) {
  return 3.14 * radius * radius;
}
```
在以上例子中（用 c++ 编写），函数 area 被重载了两个实现。第一个函数接收两个参数（都是整数），表示矩形的长度和宽度，并返回矩形的面积。另一个函数只接收一个整型参数，表示圆的半径。<br />当像 area(7) 这样调用函数 area 时，它会调用第二个函数，而 area(3,4) 则会调用第一个函数。
<a name="rYRXW"></a>
## 为什么 Python 中没有函数重载？
Python 不支持函数重载。当定义了多个同名的函数时，后面的函数总是会覆盖前面的函数，因此，在一个命名空间中，每个函数名仅会有一个登记项（entry）。
> 注：这里说 Python 不支持函数重载，指的是在不用语法糖的情况下。使用 functools 库的 singledispatch 装饰器，Python 也可以实现函数重载。原文作者在文末的注释中专门提到了这一点。

通过调用 `locals()` 和 `globals()` 函数，可以看到 Python 的命名空间中有什么，它们分别返回局部和全局命名空间。
```python
def area(radius):
    return 3.14 * radius ** 2

>>> locals()
{
    ...
    'area': <function area at 0x10476a440>,
...
}
```
在定义一个函数后，接着调用 `locals()` 函数，会看到它返回了一个字典，包含了定义在局部命名空间中的所有变量。字典的键是变量的名称，值是该变量的引用/值。<br />当程序在运行时，若遇到另一个同名函数，它就会更新局部命名空间中的登记项，从而消除两个函数共存的可能性。因此 Python 不支持函数重载。这是在创造语言时做出的设计决策，但这并不妨碍实现它，所以来重载一些函数吧。
<a name="IocXV"></a>
## 在Python中实现函数重载
已经知道 Python 是如何管理命名空间的，如果想要实现函数重载，就需要这样做：

- 维护一个虚拟的命名空间，在其中管理函数定义
- 根据每次传递的参数，设法调用适当的函数

为了简单起见，在实现函数重载时，通过不同的参数数量来区分同名函数。
<a name="Zer61"></a>
## 把函数封装起来
创建了一个名为Function的类，它可以封装任何函数，并通过重写的`__call__`方法来调用该函数，还提供了一个名为key的方法，该方法返回一个元组，使该函数在整个代码库中是唯一的。
```python
from inspect import getfullargspec

class Function(object):
    """Function类是对标准的Python函数的封装"""
    def __init__(self, fn):
        self.fn = fn

    def __call__(self, *args, **kwargs):
        """当像函数一样被调用时，它就会调用被封装的函数，并返回该函数的返回值"""
        return self.fn(*args, **kwargs)

    def key(self, args=None):
        """返回一个key，能唯一标识出一个函数（即便是被重载的）"""
        # 如果不指定args，则从函数的定义中提取参数
        if args is None:
            args = getfullargspec(self.fn).args

        return tuple([
            self.fn.__module__,
            self.fn.__class__,
            self.fn.__name__,
            len(args or []),
        ])
```
在上面的代码片段中，key函数返回一个元组，该元组唯一标识了代码库中的函数，并且记录了：

- 函数所属的模块
- 函数所属的类
- 函数名
- 函数接收的参数量

被重写的`__call__`方法会调用被封装的函数，并返回计算的值（这没有啥特别的）。这使得Function的实例可以像函数一样被调用，并且它的行为与被封装的函数完全一样。
```python
def area(l, b):
    return l * b

>>> func = Function(area)
>>> func.key()
('__main__', <class 'function'>, 'area', 2)
>>> func(3, 4)
12
```
在上面的例子中，函数area被封装在Function中，并被实例化成func。key() 返回一个元组，其第一个元素是模块名`__main__`，第二个是类`<class 'function'>`，第三个是函数名area，而第四个则是该函数接收的参数数量，即 2。<br />这个示例还显示出，可以像调用普通的 area函数一样，去调用实例 func，当传入参数 3 和 4时，得到的结果是 12，这正是调用 area(3,4) 时会得到的结果。当接下来运用装饰器时，这种行为将会派上用场。
<a name="sAe2T"></a>
## 构建虚拟的命名空间
要创建一个虚拟的命名空间，用于存储在定义阶段收集的所有函数。<br />由于只有一个命名空间/注册表，创建了一个单例类，并把函数保存在字典中。该字典的键不是函数名，而是从 key 函数中得到的元组，该元组包含的元素能唯一标识出一个函数。<br />通过这样，就能在注册表中保存所有的函数，即使它们有相同的名称（但不同的参数），从而实现函数重载。
```python
class Namespace(object):
    """Namespace是一个单例类，负责保存所有的函数"""
    __instance = None

    def __init__(self):
        if self.__instance is None:
            self.function_map = dict()
            Namespace.__instance = self
        else:
            raise Exception("cannot instantiate a virtual Namespace again")

    @staticmethod
    def get_instance():
        if Namespace.__instance is None:
            Namespace()
        return Namespace.__instance

    def register(self, fn):
        """在虚拟的命名空间中注册函数，并返回Function类的可调用实例"""
        func = Function(fn)
        self.function_map[func.key()] = fn
        return func
```
`Namespace`类有一个`register`方法，该方法将函数 fn 作为参数，为其创建一个唯一的键，并将函数存储在字典中，最后返回封装了 fn 的Function的实例。这意味着 register 函数的返回值也是可调用的，并且（到目前为止）它的行为与被封装的函数 fn 完全相同。
```python
def area(l, b):
    return l * b

>>> namespace = Namespace.get_instance()
>>> func = namespace.register(area)
>>> func(3, 4)
12
```
<a name="mIG6Z"></a>
## 使用装饰器作为钩子
既然已经定义了一个能够注册函数的虚拟命名空间，那么，还需要一个钩子来在函数定义期间调用它。在这里，会使用 Python 装饰器。<br />在 Python 中，装饰器用于封装一个函数，并允许在不修改该函数的结构的情况下，向其添加新功能。装饰器把被装饰的函数 fn 作为参数，并返回一个新的函数，用于实际的调用。新的函数会接收原始函数的 args 和 kwargs，并返回最终的值。<br />以下是一个装饰器的示例，演示了如何给函数添加计时功能。
```python
import time

def my_decorator(fn):
    """这是一个自定义的函数，可以装饰任何函数，并打印其执行过程的耗时"""
    def wrapper_function(*args, **kwargs):
        start_time = time.time()
        # 调用被装饰的函数，并获取其返回值
        value = fn(*args, **kwargs)
        print("the function execution took:", time.time() - start_time, "seconds")
        # 返回被装饰的函数的调用结果
        return value
    return wrapper_function

@my_decorator
def area(l, b):
    return l * b

>>> area(3, 4)
the function execution took: 9.5367431640625e-07 seconds
12
```
在上面的例子中，定义了一个名为 my_decorator 的装饰器，它封装了函数 area，并在标准输出上打印出执行 area 所需的时间。<br />每当解释器遇到一个函数定义时，就会调用装饰器函数 my_decorator（用它封装被装饰的函数，并将封装后的函数存储在 Python 的局部或全局命名空间中），对于我们来说，它是在虚拟命名空间中注册函数的理想钩子。<br />因此，创建了名为overload的装饰器，它能在虚拟命名空间中注册函数，并返回一个可调用对象。
```python
def overload(fn):
    """用于封装函数，并返回Function类的一个可调用对象"""
    return Namespace.get_instance().register(fn)
```
overload装饰器借助命名空间的 `.register()` 函数，返回 Function 的一个实例。现在，无论何时调用函数（被 overload 装饰的），它都会调用由 `.register()` 函数所返回的函数——Function 的一个实例，其`**__call__**`方法会在调用期间使用指定的 args 和 kwargs 执行。<br />现在剩下的就是在 Function 类中实现`__call__`方法，使得它能根据调用期间传入的参数而调用相应的函数。
<a name="um1WA"></a>
## 从命名空间中找到正确的函数
想要区别出不同的函数，除了通常的模块、类和函数名以外，还可以依据函数的参数数量，因此，在虚拟的命名空间中定义了一个 get 方法，它会从 Python 的命名空间中读取待区分的函数以及实参，最后依据参数的不同，返回出正确的函数。没有更改 Python 的默认行为，因此在原生的命名空间中，同名的函数只有一个。<br />这个 get 函数决定了会调用函数的哪个实现（如果重载了的话）。找到正确的函数的过程非常简单——先使用 key 方法，它利用函数和参数来创建出唯一的键（正如注册时所做的那样），接着查找这个键是否存在于函数注册表中；如果存在，则获取其映射的实现。
```python
def get(self, fn, *args):
    """从虚拟命名空间中返回匹配到的函数，如果没找到匹配，则返回None"""
    func = Function(fn)
    return self.function_map.get(func.key(args=args))
```
get 函数创建了 Function 类的一个实例，这样就可以复用类的 key 函数来获得一个唯一的键，而不用再写创建键的逻辑。然后，这个键将用于从函数注册表中获取正确的函数。
<a name="HfrkL"></a>
## 实现函数的调用
前面说过，每次调用被 overload 装饰的函数时，都会调用 Function 类中的`__call__`方法。需要让`__call__`方法从命名空间的 get 函数中，获取出正确的函数，并调用之。<br />`__call__`方法的实现如下：
```python
def __call__(self, *args, **kwargs):
    """重写能让类的实例变可调用对象的__call__方法"""
    # 依据参数，从虚拟命名空间中获取将要调用的函数
    fn = Namespace.get_instance().get(self.fn, *args)
    if not fn:
        raise Exception("no matching function found.")
        # 调用被封装的函数，并返回调用的结果
    return fn(*args, **kwargs)
```
该方法从虚拟命名空间中获取正确的函数，如果没有找到任何函数，它就抛出一个 Exception，如果找到了，就会调用该函数，并返回调用的结果。
<a name="zBTr5"></a>
## 运用函数重载
准备好所有代码后，定义了两个名为 area 的函数：一个计算矩形的面积，另一个计算圆的面积。下面定义了两个函数，并使用overload装饰器进行装饰。
```python
@overload
def area(l, b):
    return l * b

@overload
def area(r):
    import math
    return math.pi * r ** 2

>>> area(3, 4)
12
>>> area(7)
153.93804002589985
```
当用一个参数调用 area 时，它返回了一个圆的面积，当传递两个参数时，它会调用计算矩形面积的函数，从而实现了函数 area 的重载。
> 原作者注：从 Python 3.4 开始，Python 的 functools.singledispatch 支持函数重载。从 Python 3.8 开始，functools.singledispatchmethod 支持重载类和实例方法。感谢 Harry Percival 的指正。

<a name="sKLMw"></a>
## 总结
Python 不支持函数重载，但是通过使用它的基本结构，捣鼓了一个解决方案。<br />使用装饰器和虚拟的命名空间来重载函数，并使用参数的数量作为区别函数的因素。还可以根据参数的类型（在装饰器中定义）来区别函数——即重载那些参数数量相同但参数类型不同的函数。<br />重载能做到什么程度，这仅仅受限于getfullargspec函数和我们的想象。使用前文的思路，可能会实现出一个更整洁、更干净、更高效的方法，所以，请尝试实现一下吧。<br />正文到此结束。以下附上完整的代码：
```python
# 模块：overload.py
from inspect import getfullargspec


class Function(object):
    """Function is a wrap over standard python function
  An instance of this Function class is also callable
  just like the python function that it wrapped.
  When the instance is "called" like a function it fetches
  the function to be invoked from the virtual namespace and then
  invokes the same.
  """

    def __init__(self, fn):
        self.fn = fn

    def __call__(self, *args, **kwargs):
        """Overriding the __call__ function which makes the
    instance callable.
    """
        # fetching the function to be invoked from the virtual namespace
        # through the arguments.
        fn = Namespace.get_instance().get(self.fn, *args)
        if not fn:
            raise Exception("no matching function found.")
            # invoking the wrapped function and returning the value.
        return fn(*args, **kwargs)

    def key(self, args=None):
        """Returns the key that will uniquely identifies
    a function (even when it is overloaded).
    """
        if args is None:
            args = getfullargspec(self.fn).args
        return tuple([
            self.fn.__module__,
            self.fn.__class__,
            self.fn.__name__,
            len(args or []),
        ])


class Namespace(object):
    """Namespace is the singleton class that is responsible
  for holding all the functions.
  """
    __instance = None

    def __init__(self):
        if self.__instance is None:
            self.function_map = dict()
            Namespace.__instance = self
        else:
            raise Exception("cannot instantiate Namespace again.")

    @staticmethod
    def get_instance():
        if Namespace.__instance is None:
            Namespace()
        return Namespace.__instance

    def register(self, fn):
        """registers the function in the virtual namespace and returns
    an instance of callable Function that wraps the function fn.
    """
        func = Function(fn)
        specs = getfullargspec(fn)
        self.function_map[func.key()] = fn
        return func

    def get(self, fn, *args):
        """get returns the matching function from the virtual namespace.
    return None if it did not fund any matching function.
    """
        func = Function(fn)
        return self.function_map.get(func.key(args=args))


def overload(fn):
    """overload is the decorator that wraps the function
  and returns a callable object of type Function.
  """
    return Namespace.get_instance().register(fn)
```
最后，演示代码如下：
```python
from overload import overload

@overload
def area(length, breadth):
    return length * breadth

@overload
def area(radius):
    import math
    return math.pi * radius ** 2

@overload
def area(length, breadth, height):
    return 2 * (length * breadth + breadth * height + height * length)

@overload
def volume(length, breadth, height):
    return length * breadth * height

@overload
def area(length, breadth, height):
    return length + breadth + height

@overload
def area():
    return 0

print(f"area of cuboid with dimension (4, 3, 6) is: {area(4, 3, 6)}")
print(f"area of rectangle with dimension (7, 2) is: {area(7, 2)}")
print(f"area of circle with radius 7 is: {area(7)}")
print(f"area of nothing is: {area()}")
print(f"volume of cuboid with dimension (4, 3, 6) is: {volume(4, 3, 6)}")
```
