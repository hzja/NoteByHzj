Python<br />在 Python 中，**可调用**对象是可以使用一对括号调用的任何对象，也可以选择使用一系列参数。函数、类和方法都是 Python 中可调用对象的常见示例。除此之外，还可以通过在类中添加 `.__call__()` 特殊方法来创建产生**可调用实例的**自定义类。<br />带有 `.__call__()` 方法的类实例的行为类似于函数，它提供了一种灵活方便的方法来为你的对象添加功能。作为一个 Python 开发者，了解如何创建和使用可调用实例是一项宝贵的技能。<br />接下来一起学习：

- 理解Python中**可调用对象的**概念
- 通过为类提供 `.__call__()` 方法来创建**可调用实例**
- 理解 `.__init__()` 和 `.__call__()` 之间的**区别**
- 使用可调用实例解决**实际问题的**几个示例代码

本文预备知识，应该熟悉Python面向对象编程的基础知识，包括如何定义和使用类和方法。熟悉 Python 装饰器和策略设计模式也会有所帮助。还应该理解状态state的概念。
<a name="a632M"></a>
## 理解Python中的可调用对象
Python 中的可调用对象是可以使用一对括号和一系列参数（如果需要）调用的任何对象。在与 Python 的日常交互中，会发现不同的可调用实例。其中一些包括

- 内置函数和类
- 使用 def 关键字创建的用户自定义函数
- 使用 lambda 关键字编写的匿名函数
- 自定义类的构造函数
- 实例、类和静态方法
- 实现 `.__call__()` 方法的类的实例
- 从函数返回的闭包
- 使用 `yield` 关键字定义的生成函数
- 使用 `async` 关键字创建的异步函数和方法

所有这些不同的可调用程序都有一些共同点。它们都实现了 `.__call__()` 特殊方法。为了证实这一点，可以使用内置的 `dir()` 函数，它将对象作为参数，并返回对象的属性和方法列表：
```python
>>> dir(abs)
[
    '__call__',
    '__class__',
    ...
]

>>> dir(all)
[
    '__call__',
    '__class__',
    ...
]

>>> def greet():
...     print("Hello, World!")
...

>>> dir(greet)
[
    '__annotations__',
    '__builtins__',
    '__call__',
    ...
]
```
在前两个示例中，使用内置的 `abs()` 和 `all()` 作为参数调用了 `dir()`。在这两个示例中，都可以看到 `.__call__()` 方法出现在输出中。<br />最后定义了一个自定义函数，用于向屏幕`print`一条消息。这个函数也有 `.__call__()`，注意如何使用这个方法来调用函数：
```python
>>> greet.__call__()
Hello, World!
```
注意使用 `.__call__()` 和直接使用 `greet()` 调用函数的效果是一样的。<br />**注意：**尽管可以直接调用特殊方法，如 `.__call__()`，但这样做并不是推荐或最佳的做法。相反，请像往常一样调用函数。<br />现在，所有这些在内部是如何工作的？<br />当运行 `callable_object(*args, **kwargs)` 时，Python 内部会将操作转换为 `callable_object.__call__(*args, **kwargs)`。常规函数的参数与 `.__call__()` 中使用的参数相同。换句话说，每当调用一个可调用对象时，Python 会使用传入可调用对象的参数在幕后自动运行它的 `.__call__()` 方法。<br />看看下面的自定义类：
```python
>>> class SampleClass:
...     def method(self):
...         print("You called method()!")
...

>>> type(SampleClass)
<class 'type'>

>>> dir(type)
[
    '__abstractmethods__',
    '__annotations__',
    '__base__',
    '__bases__',
    '__basicsize__',
    '__call__',
    ...
]

>>> sample_instance = SampleClass()
>>> dir(sample_instance.method)
[
    '__call__',
    '__class__',
    ...
]
```
在 Python 中，一切都是对象。像 SampleClass 这样的类是类型对象，可以通过调用 `type()` 来确认，调用类型对象作为参数，或者通过访问 `.__class__` 属性来确认。<br />SampleClass 的类构造函数使用 `type.__call__()`。这就是为什么可以调用 `SampleClass()` 得到一个新实例。因此，类构造函数是返回底层类的新实例的可调用对象。<br />在上面的示例中，可以观察到方法对象，如 `sample_instance.method`，也有一个 `.__call__()` 特殊方法，将它们变成可调用对象。这里的主要启示是，要成为可调用对象，对象需要有一个 `.__call__()` 方法。<br />如果检查闭包、生成器函数或异步函数，那么将得到类似的结果。总能在可调用对象中找到 `.__call__()` 方法。
<a name="fYEzi"></a>
## 检查对象是否可调用
如果需要检查一个 Python 对象是否可调用，那么可以使用内置的 `callable()` 函数，如下：
```python
>>> callable(abs)
True
>>> callable(all)
True

>>> callable(greet)
True

>>> callable(SampleClass)
True

>>> callable(sample_instance)
False
```
函数 `callable()` 将对象作为参数，如果对象可调用，则返回 True。否则返回 False。<br />**注意：**因为 `dir()` 检查给定对象是否有 `.__call__()` 方法，可以用它来检查该对象是否可调用。尽管 `dir()` 在测试代码和示例时比较有用，但在需要快速检查对象是否可调用时并不是特别有用。相比之下，`callable()` 函数是一个谓词函数，可以直接在上下文中使用它。<br />在上面的示例中，除了 sample_instance 之外，所有被测试对象都是可调用的。这是可以预见的，因为 SampleClass 没有为它的实例实现 `.__call__()` 方法。可以通过编写 `.__call__()` 方法使自定义类的实例可被调用。在下面的章节中，一起学习把类的实例变成可调用对象的基础知识。<br />但首先要注意的是，有时`callable()`可能会产生误报：
```python
>>> class NonCallable:
...     def __call__(self):
...         raise TypeError("not really callable")
...

>>> instance = NonCallable()
>>> callable(instance)
True

>>> instance()
Traceback (most recent call last):
    ...
TypeError: not really callable
```
在本例中，`callable()`返回True。然而，这个自定义类的实例不是可调用的，如果调用它们，会得到一个错误。因此，`callable()` 只能保证目标实例来自一个实现了 `.__call__()` 方法的类。
<a name="sOwba"></a>
## 使用`.__call__()`创建可调用实例
如果给定类的实例是可调用的，那么需要在底层类中实现 `.__call__()` 特殊方法。这个方法能够像调用普通 Python 函数那样调用类的实例。<br />与其它特殊方法不同，`.__call__()` 对它必须接受的参数没有特殊要求。它和其它实例方法一样，以 self 作为第一个参数，并且可以根据需要接受任意多的额外参数。<br />下面的示例说明了带有 `.__call__()` 方法的类实例是如何工作的：
```python
# counter.py

class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        self.count += 1

    def __call__(self):
        self.increment()
```
在这个 Counter 类中，有一个 `.count` 实例属性来跟踪当前计数。然后，有一个 `.increment()` 方法，每次调用它时都将计数加 1。最后，添加一个 `.__call__()` 方法。在这个示例中，`.__call__()` 返回到调用 `.increment()`，为运行递增操作提供了快捷方式。<br />看看该类产品在实践中是如何工作的：
```python
>>> from counter import Counter

>>> counter = Counter()

>>> counter.increment()
>>> counter.count
1

>>> counter()
>>> counter.count
2
>>> counter()
>>> counter.count
3
```
创建 Counter 实例后，调用 `.increment()`。该调用将 `.count` 属性递增 1，可以通过访问该属性来确认。在其余的示例中，利用类中有 `.__call__()` 方法这一事实，直接调用该实例来增加计数。<br />在本例中，`.__call__()` 提供了运行计数递增操作的快速快捷方式。这一特性为类提供了一个方便的用户友好界面。<br />上例中的 `.__call__()` 方法不带任何参数。该方法也不返回显式值。然而，在自定义类中如何编写 `.__call__()` 方法没有限制。因此，可以让它们接受参数、返回值，甚至像 Counter 类示例中那样产生副作用。<br />第二个例子是下面的类，它可以创建可调用对象来计算不同的幂：
```python
# power.py

class PowerFactory:
    def __init__(self, exponent=2):
        self.exponent = exponent

    def __call__(self, base):
        return base**self.exponent
```
在这个示例中，PowerFactory 类将指数作为参数，稍后将使用它运行不同的幂运算。`.__call__()`方法接收一个基本参数，并使用先前提供的指数计算其幂次。最后，该方法返回计算结果。
```python
>>> from power import PowerFactory

>>> square_of = PowerFactory(2)
>>> square_of(3)
9
>>> square_of(6)
36

>>> cube_of = PowerFactory(3)
>>> cube_of(3)
27
>>> cube_of(6)
216
```
在这里，使用 PowerFactory 创建了两个不同的可调用实例。第一个实例将数字提升到2的幂级数，而第二个实例将数字提升到3的幂级数。<br />在这个示例中，当调用 `square_of()` 或 `cube_of()` 时，需要传递 base 作为参数，因为这些调用会返回到调用 `.__call__()`，而 `.__call__()`需要一个 base 参数。最后，请注意是如何从每次调用中返回幂的。这是因为 `.__call__()` 返回计算所需幂的结果。<br />在自定义类中定义 `.__call__()` 方法允许将这些类的实例作为普通的 Python 函数来使用。这个特性在很多情况下都会派上用场，将在 Python 的 `.__call__()` 应用一节中学习到。<br />在跳转到可调用实例的常见用例之前，将探讨 `.__init__()` 和 `.__call__()` 方法之间的区别。这两个方法和它们在 Python 类中的相应角色可能会迷惑 Python 初学者。
<a name="upXJP"></a>
## `.__init__()`vs`.__call__()`
区分 `.__init__()` 和 `.__call__()` 在 Python 类中的作用，对于刚刚开始学习 Python 语言或其面向对象特性的开发者来说，可能比较难以理解。然而，这两个方法是非常不同的，而且每个方法都有特定的目标。<br />`.__init__()` 方法是实例初始化器。每当通过调用类的构造函数创建一个类的实例时，Python 自动调用这个方法。`.__init__()` 的参数与类构造函数的参数相同，它们通常为实例属性提供初始值。<br />同时，`.__call__()` 方法将实例转化为可调用对象。每当调用一个给定类的具体实例时，Python 自动调用这个方法。<br />为了说明这两种方法的区别，请看下面的示例类：
```python
class Demo:
    def __init__(self, attr):
        print(f"Initialize an instance of {self.__class__.__name__}")
        self.attr = attr
        print(f"{self.attr = }")

    def __call__(self, arg):
        print(f"Call an instance of {self.__class__.__name__} with {arg}")
```
这个 Demo 类实现了`.__init__()`和 `.__call__()`。在 `.__init__()`中，打印一条消息并初始化 `.attr` 属性。在 `.__call__()`中，只打印了一条消息，这样当方法被调用时，就可以知道给定的参数。<br />该类产品的工作原理如下：
```python
>>> demo = Demo("Some initial value")
Initialize an instance of Demo
self.attr = 'Some initial value'

>>> demo("Hello!")
Call an instance of Demo with Hello!
```
每个方法在类中都有不同的作用。`.__init__()`方法在创建类的实例时被调用。它的主要目的是用合理的初始值初始化实例属性。<br />可以在所有的 Python 类中找到 `.__init__()` 。有些类有一个显式的实现，有些类从父类继承这个方法。在许多情况下，object 是提供这个方法的类：
```python
>>> dir(object)
[
    ...
    '__gt__',
    '__hash__',
    '__init__',
    ...
]
```
记住 object 是所有 Python 类的最终父类。因此，即使不在自定义类中定义一个显式的 `.__init__()` 方法，该类仍然会继承 object 的默认实现。<br />与此相反，`.__call__()` 方法在调用包含类的具体实例时运行，例如本例中的 demo。`.__call__()` 的目的是将实例转换为可调用对象。换句话说，它的目的是**创建可以像调用普通函数那样调用的对象**。大多数 Python 类不实现这个方法。只有当需要将实例作为函数使用时，自定义类才会有这个方法。<br />至此已经说明了 `.__call__()` 和 `.__init__()` 之间的区别。现在继续学习 `.__call__()` 并探索如何在 Python 代码中利用这个方法。
<a name="tx2sb"></a>
## 调用`.__call__()`
编写产生可调用实例的类在一些情况下非常有用。例如，可以在需要时利用可调用实例：

- 在调用之间**保持状态**
- **缓存先前计算**的结果值
- 实现简单方便的**API**

尽管可以使用函数或具有常规方法的类来解决所有这些问题，但在某些情况下，使用可调用实例可能是一个不错的选择。尤其是当已经有了一个现成的类，但又需要类似函数的行为时。<br />在下面的章节中，将写一些实际的例子来说明 Python 中可调用实例的这些用例。
<a name="KvIFx"></a>
### 编写有状态的可调用程序
有时，可能希望编写**在调用之间保留状态的可调用对象**，这就是通常所说的**有状态可调用**对象。<br />例如，编写一个可调用对象，**从数据流中获取连续的数值并计算它们的累计平均值**。在调用之间，可调用对象必须跟踪先前传递的值。<br />为了解决这个问题，可以使用类似下面这样的闭包：
```python
>>> def cumulative_average():
...     data = []
...     def average(new_value):
...         data.append(new_value)
...         return sum(data) / len(data)
...     return average
...

>>> stream_average = cumulative_average()

>>> stream_average(12)
12.0
>>> stream_average(13)
12.5
>>> stream_average(11)
12.0
>>> stream_average(10)
11.5
```
在 `cumulative_average()` 中，使用一个名为 data 的局部变量来保存调用之间的数据。然后定义一个名为 `average()` 的内部函数。该函数在每次调用时获取一个新值并将其追加到 data 中。然后函数计算并返回当前存储数据的平均值。<br />最后，`cumulative_average()` 返回内部函数。实际上，它返回一个闭包，这是一个特殊的对象，将 `average()` 函数和它的非本地作用域打包在一起。在本例中，闭包包括数据变量。<br />一旦写完了 `cumulative_average()`，就可以创建像 `stream_average` 这样的自定义闭包。这个对象是可调用的，所以可以把它作为一个函数来计算数据流的累计平均值，就像在上面最后的示例中所做的那样。<br />尽管闭包可以在调用之间保留状态，但这些工具可能难以理解和处理。从这个意义上说，编写一个带有 `.__call__()` 方法的类可以简化这项工作，使代码更可读、更明确。<br />下面是如何使用带有 `.__call__()` 方法的类来解决上述问题：
```python
# cumulative_average.py

class CumulativeAverager:
    def __init__(self):
        self.data = []

    def __call__(self, new_value):
        self.data.append(new_value)
        return sum(self.data) / len(self.data)
```
在这个示例中，类有一个名为 `.data` 的实例属性来保存数据。`.__call__()` 方法在每次调用中获取一个新值，将该值追加到 `.data`，最后计算并返回平均值。<br />在这种情况下，代码具有很强的可读性。`.data` 属性保留调用之间的状态，而 `.__call__()` 方法计算累计平均值。下面是该类的实际工作方式：
```python
>>> from cumulative_average import CumulativeAverager

>>> stream_average = CumulativeAverager()
>>> stream_average(12)
12.0
>>> stream_average(13)
12.5
>>> stream_average(11)
12.0
>>> stream_average(10)
11.5
>>> stream_average.data
[12, 13, 11, 10]
```
`CumulativeAverager`的实例是可调用的，它保留先前输入的值，并在每次调用中计算累计平均值。这种方法使代码更易于推理。要编写这个类，不必了解 Python 中闭包工作的复杂性。<br />另一个优点是可以通过.data属性直接访问当前数据。
<a name="rolOm"></a>
### 缓存计算值
可调用实例的另一个常见用例是当需要在调用之间缓存计算数据的有状态可调用实例时，需要优化某些算法时，这将非常方便。<br />例如，想计算一个给定数字的阶乘。由于需要多次计算，因此需要提高计算效率。一种方法是缓存已经计算过的值，这样就不必一直重新计算。<br />下面的类使用 `.__call__()` 和缓存实现了这一结果：
```python
# factorial.py
class Factorial:
    def __init__(self):
        self.cache = {0: 1, 1: 1}

    def __call__(self, number):
        if number not in self.cache:
            self.cache[number] = number * self(number - 1)
        return self.cache[number]
```
在这个类中，使用一个字典来缓存已经计算过的阶乘值。字典键保存已传递的数字，字典值保存已计算的阶乘。<br />`.__call__()` 方法检查当前输入的数字是否已经在 `.cache` 字典中。如果是这样，该方法将返回相应的值，而不会再次运行计算。这种行为优化了算法，使其更快。<br />如果当前输入的数字不在`.cache`字典中，那么该方法将递归计算阶乘，缓存结果，并将最终值返回给调用者。<br />该类的工作原理如下：
```python
>>> from factorial import Factorial
>>> factorial_of = Factorial()
>>> factorial_of(4)
24
>>> factorial_of(5)
120
>>> factorial_of(6)
720

>>> factorial_of.cache
{0: 1, 1: 1, 2: 2, 3: 6, 4: 24, 5: 120, 6: 720}
```
每次调用阶乘实例都会检查缓存中是否有已经计算过的值。该实例只计算那些尚未传递的数值的阶乘。请注意，所有输入值及其对应的阶乘最终都存储在 `.cache` 字典中。
<a name="g3kKj"></a>
### 创建清晰便捷的API
编写能够产生可调用实例的类，还可以为库、包和模块设计方便的、用户友好的应用程序编程接口（API）。<br />例如，假设正在编写一个用于创建GUI应用程序的新颖库。库将有一个`MainWindow`类，它提供了创建GUI应用程序主窗口的所有功能。<br />该类将有几个方法，包括在屏幕上绘制窗口的 `.show()` 方法。在这种情况下，可以提供类似下面的 `.__call__()` 方法：
```python
class MainWindow:
    def show(self):
        print("Showing the app's main window...")

    def __call__(self):
        self.show()

    # ...
```
在本例中，`.__call__()` 方法回退到调用 `.show()` 方法。这种实现使我们能够通过调用 `.show()` 或窗口实例本身来显示主窗口：
```python
window = MainWindow()
window() # 或者 window.show()
```
在本示例中，`.__call__()` 提供了在屏幕上显示应用程序窗口的便捷快捷方式。这可以改善用户体验。因此，这个技巧是为 Python 项目创建友好和直观界面的好方法。<br />`.__call__()` 可以改进API的另一个用例，当有一个类，它的主要目的是提供一个动作或行为。例如，需要一个 Logger 类，它负责将消息记录到文件中：
```python
# logger.py

class Logger:
    def __init__(self, filename):
        self.filename = filename

    def __call__(self, message):
        with open(self.filename, mode="a", encoding="utf-8") as log_file:
            log_file.write(message + "\n")
```
在本例中，Logger 的主要用途是将消息写入提供的日志文件。通过实现 `.__call__()`方法，提供了一个通过像函数一样调用对象来访问该功能的快捷方式。
<a name="pvhkS"></a>
## 探索`.__call__()`的高级用例
到目前为止，已经学到了很多关于在类中使用 `.__call__()` 方法创建可调用实例的知识。这个方法在 Python 中还有一些高级用例。其中一个用例是当你想创建基于类的装饰器时。在这种情况下，`.__call__()` 方法是唯一的方法，因为它实现了可调用实例。<br />`.__call__()` 的另一个用例是当需要在 Python 中实现策略设计模式时。在这种情况下，可以利用 `.__call__()` 来创建为我们的不同策略提供实现的类。<br />在下面的章节中，将学习如何使用 `.__call__()` 来创建基于类的装饰器，以及如何在 Python 中实现策略模式。
<a name="j5MIO"></a>
### 编写基于类的装饰器
Python 的装饰器是可调用的，它将另一个可调用方法作为参数并扩展它的行为，而不显式地修改它的代码。**装饰器提供了一个很好的工具来为现有的可调用程序添加新的功能**。<br />查找和编写**基于函数的装饰器是**很常见的。然而，也可以利用 `.__call__()` 特殊方法编写**基于类的装饰器。**<br />假设想创建一个装饰器**来测量自定义函数的执行时间**。下面的代码展示了如何将该装饰器写成一个类：
```python
# timing.py
import time
class ExecutionTimer:
    def __init__(self, func):
        self.func = func

    def __call__(self, *args, **kwargs):
        start = time.perf_counter()
        result = self.func(*args, **kwargs)
        end = time.perf_counter()
        print(f"{self.func.__name__}() took {(end - start) * 1000:.4f} ms")
        return result
```
ExecutionTimer 类在实例化时将一个函数对象作为参数。`.__call__()` 方法对该函数对象进行操作。在本例中，`.__call__()` 使用 *args 和 **kwargs 通用参数来处理输入函数所需的任何参数。<br />接下来，使用 `time.perf_counter()` 获取输入函数执行前后的时间。然后打印一条包含函数名称和执行时间（以毫秒为单位）的消息。最后一步是返回调用输入函数的结果。<br />有了这个类，可以立即开始测量 Python 函数的执行时间：
```python
>>> from timing import ExecutionTimer

>>> @ExecutionTimer
... def square_numbers(numbers):
...     return [number ** 2 for number in numbers]
...

>>> square_numbers(list(range(100)))
square_numbers() took 0.0069 ms
[
    0,
    1,
    4,
    9,
    16,
    25,
    ...
]
```
在此代码片段中，有一个接收数字列表并返回平方值列表的函数。希望测量该函数的执行时间，因此使用 `@ExecutionTimer` 装饰器来实现该目的。<br />一旦函数被装饰，无论何时运行它，都会收到一条包含函数名称和执行时间（以毫秒为单位）的消息，然后得到函数的返回值。<br />现在，在装饰器中添加一个重复参数。这个参数将允许多次运行输入函数并计算平均执行时间：
```python
# timing.py

import time

class ExecutionTimer:
    def __init__(self, repetitions=1):
        self.repetitions = repetitions

    def __call__(self, func):
        def timer(*args, **kwargs):
            result = None
            total_time = 0
            print(f"Running {func.__name__}() {self.repetitions} times")
            for _ in range(self.repetitions):
                start = time.perf_counter()
                result = func(*args, **kwargs)
                end = time.perf_counter()
                total_time += end - start
            average_time = total_time / self.repetitions
            print(
                f"{func.__name__}() takes "
                f"{average_time * 1000:.4f} ms on average"
            )
            return result

        return timer
```
`ExecutionTimer`的更新版本与最初的实现有很大不同。类初始化器将repetitions作为一个参数，需要提供该参数作为装饰器调用的一部分。<br />在 `.__call__()`中，将输入函数作为参数，然后创建一个内部函数来处理输入函数的执行。在这个内部函数中，使用 for 循环多次运行输入函数并计算总的执行时间。<br />接下来，计算平均执行时间，并像往常一样打印一条信息。最后，返回输入函数的结果。请注意，`.__call__()` 返回由 timer 表示的函数对象。<br />完成这些更改后，请再次尝试`ExecutionTimer`。注意，要访问新版本的`ExecutionTimer`，需要重新加载timing.py文件或重新启动当前的交互式会话：
```python
>>> from timing import ExecutionTimer

>>> @ExecutionTimer(repetitions=100)
... def square_numbers(numbers):
...     return [number ** 2 for number in numbers]
...

>>> square_numbers(list(range(100)))
Running square_numbers() 100 times
square_numbers() takes 0.0073 ms on average
[
    0,
    1,
    4,
    9,
    16,
    25,
    ...
]
```
装饰器现在可以运行目标函数的特定次数，并计算平均执行时间。
<a name="whdoe"></a>
### 实现策略设计模式
**策略设计模式**可以定义一系列类似的算法，并使它们在运行时可以互换。换句话说，该模式为给定类型的问题实现了不同的解决方案，每个解决方案都捆绑在一个特定的对象中。然后，可以动态地选择合适的解决方案。<br />**Tips：** 策略设计模式在在C++或Java语言中也非常有用。使用这种模式可以将函数作为参数传递给其他函数。<br />作为如何使用 `.__call__()` 实现策略模式的示例，假设需要根据特定条件将某些数据序列化为 JSON 或 YAML。在这种情况下，可以使用策略模式。有一个类将数据序列化为 JSON，另一个类将数据序列化为 YAML。<br />在下面的示例中，将编写一个解决方案来解决问题。首先需要使用 `pip install pyyaml`，因为 Python 标准库没有提供处理 YAML 数据的适当工具。
```python
# serializing.py

import json
import yaml

class JsonSerializer:
    def __call__(self, data):
        return json.dumps(data, indent=4)

class YamlSerializer:
    def __call__(self, data):
        return yaml.dump(data)

class DataSerializer:
    def __init__(self, serializing_strategy):
        self.serializing_strategy = serializing_strategy

    def serialize(self, data):
        return self.serializing_strategy(data)
```
在本示例中，有 `JsonSerializer` 和 `YamlSerializer` 类，它们代表序列化策略。它们的 `.__call__()` 方法使用适当的工具将输入数据分别序列化为 JSON 和 YAML。<br />然后是`DataSerializer`类，它提供了更高级别的类。将使用该类对数据进行序列化。首先，需要提供一个具体序列化器类的可调用实例：
```python
>>> from serializing import DataSerializer, JsonSerializer, YamlSerializer

>>> data = {
...     "name": "Jane Doe",
...     "age": 30,
...     "city": "Salt Lake City",
...     "job": "Python Developer",
... }

>>> serializer = DataSerializer(JsonSerializer())
>>> print(f"JSON:\n{serializer.serialize(data)}")
JSON:
{
    "name": "Jane Doe",
    "age": 30,
    "city": "Salt Lake City",
    "job": "Python Developer"
}

>>> # Switch strategy
>>> serializer.serializing_strategy = YamlSerializer()
>>> print(f"YAML:\n{serializer.serialize(data)}")
YAML:
age: 30
city: Salt Lake City
job: Python Developer
name: Jane Doe
```
在该代码片段中，有一个包含一些示例数据的字典。为了处理这些数据，使用`JsonSerializer`作为参数创建了一个`DataSerializer`实例。完成这一步后，实例就可以将字典转换为JSON格式。<br />在最后一个示例中，改变了序列化策略，并使用数据序列化器将数据转换为YAML代码。有其他有用的数据序列化器的想法吗？
<a name="iE4Fs"></a>
## 结论
已经学习了很多 Python 中的**可调用实例**，特别是如何在自定义类中使用 `.__call__()` 特殊方法来定义它们。现在知道了如何创建产生对象的类，这些对象可以像普通函数一样被调用。可以为面向对象程序增加灵活性和功能性。<br />**学会了如何：**

- 理解Python中的**可调用语言**
- 使用 `.__call__()` 方法编写**可**调用**实例**
- 掌握 `.__init__()` 和 `__.call()__` 的**区别**
- 实现使用可调用实例解决**实际问题的**各种示例

有了这些知识，就可以在 Python 代码中设计和实现可调用实例。这将能够解决各种常见问题，例如在调用之间保留状态、缓存数据、编写基于类的装饰器等等。
<a name="a9iZ9"></a>
## 参考资料
callable(): [_https://docs.python.org/3/library/functions.html?highlight=object#callable_](https://docs.python.org/3/library/functions.html?highlight=object#callable)
