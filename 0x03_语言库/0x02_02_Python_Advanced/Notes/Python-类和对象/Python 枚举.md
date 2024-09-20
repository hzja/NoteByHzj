Python 枚举<br />enum 是一组绑定到唯一常数值的符号名称，并且具备可迭代性和可比较性的特性。可以使用 enum 创建具有良好定义的标识符，而不是直接使用魔法字符串或整数，也便于开发工程师的代码维护。
<a name="ePP9o"></a>
## 创建枚举
可以使用 class 语法创建一个枚举类型，方便进行读写，另外，根据函数 API 的描述定义，可以创建一个 enum 的子类，如下：
```python
from enum import Enum

class HttpStatus(Enum):
    OK = 200
    BAD_REQUEST = 400
    FORBIDDEN = 403
    NOT_FOUND = 404
    REQUEST_TIMEOUT = 408
    SERVICE_UNAVAILABLE = 500
```
注意：枚举属性值可以是任何东西: int, str 等。如果确切的值不重要，可以使用 auto 实例，并选择适当的值。如果将 auto 与其他值混合，则必须小心。**枚举类型中，不可以设置相同名称的 name，可以有相同的 value**。<br />enum 自带属性 name 和 value，日常工作中使用最多的也是这两个属性，打印看看结果：
```python
print('Member: {}'.format(HttpStatus.OK))               # Member: HttpStatus.OK
print('Member name: {}'.format(HttpStatus.OK.name))     # Member name: OK
print('Member value: {}'.format(HttpStatus.OK.value))   # Member value: 200
print(repr(HttpStatus.OK))                              # <enum 'HttpStatus'>
print(type(HttpStatus.OK))                              # <HttpStatus.OK: 200>
print(isinstance(HttpStatus.OK, HttpStatus))            # True
```
<a name="QIyVa"></a>
## 枚举迭代
枚举支持迭代和遍历顺序。举个例子：
```python
from enum import Enum, auto

# 创建
class HttpStatus(Enum):
    OK = 200
    BAD_REQUEST = 400
    FORBIDDEN = 403
    NOT_FOUND = 404
    REQUEST_TIMEOUT = 408
    SERVICE_UNAVAILABLE = 500
    OTHER = auto.value

# 迭代
for status in HttpStatus:
    print('{} : {}'.format(status.name, status.value))
```
打印结果：
```python
OK : 200
BAD_REQUEST : 400
FORBIDDEN : 403
NOT_FOUND : 404
REQUEST_TIMEOUT : 408
SERVICE_UNAVAILABLE : 500
OTHER : <object object at 0x000002863E1D7B10>
```
可以看出，遍历的每一个 status 是一个独立的枚举成员，拥有 name 和 value 属性。<br />另外，也可以使用如下形式来进行枚举遍历：
```python
for name, member in HttpStatus.__members__.items():
    print('{} : {}'.format(name, member))
```
<a name="jlfrA"></a>
## 枚举成员与属性访问

- 通过枚举 value 进行访问，访问需要使用元组()的形式
```python
print(HttpStatus(200))      # HttpStatus.OK
```

- 通过枚举 name 进行访问，访问需要使用列表[]的形式
```python
print(HttpStatus['OK'])     # HttpStatus.OK
```

- 将属性赋予另一个 enum 成员
```python
number = HttpStatus.OK
print(number)               # HttpStatus.OK
```
<a name="Mby8K"></a>
## 枚举值唯一
上面创建的枚举类中，value 值是可以重复的，如果不想枚举类中的值重复可以是用装饰器 `@unique`，举例如下：
```python
from enum import Enum, unique

# 创建
@unique
class HttpStatus(Enum):
    OK = 200
    BAD_REQUEST = 400
    FORBIDDEN = 403
    NOT_FOUND = 404
    REQUEST_TIMEOUT = 408
    SERVICE_UNAVAILABLE = 500
    OTHER = 200
```
运行后，报如下异常：
```python
ValueError: duplicate values found in <enum 'HttpStatus'>: OTHER -> OK
```
查看源代码，发现加入此装饰器的枚举类型，`unique` 方法会将其 `__members__.items()` 进行遍历，追加到 `duplicates` 列表中，如果发现列表不为空，则抛出如上异常信息。
<a name="moaEb"></a>
## 枚举自动赋值
此功能用于在使用枚举时，只在意枚举的标识符的含义而不在意值的情况下，但是如果需要与字符串或整数混合使用就要额外注意。下面贴上官方的示例：
```python
import unittest
from enum import auto, Enum

class TestEnum(unittest.TestCase):
    
    def test_auto_number(self):
        class Color(Enum):
            red = auto()
            blue = auto()
            green = auto()
    
        self.assertEqual(list(Color), [Color.red, Color.blue, Color.green])
        self.assertEqual(Color.red.value, 1)
        self.assertEqual(Color.blue.value, 2)
        self.assertEqual(Color.green.value, 3)
    
    def test_auto_name(self):
        class Color(Enum):
            def _generate_next_value_(self, start, count, last):
                return self
    
            red = auto()
            blue = auto()
            green = auto()
    
        self.assertEqual(list(Color), [Color.red, Color.blue, Color.green])
        self.assertEqual(Color.red.value, 'red')
        self.assertEqual(Color.blue.value, 'blue')
        self.assertEqual(Color.green.value, 'green')
```
可以发现，使用 `auto()` 得到的是整数自增型，如果需要别的方式，只需要在枚举类中，重写 `_generate_next_value_` 方法。
<a name="Wco7A"></a>
## 枚举比较
枚举对象可以进行比较，但是不能进行值比较，如果需要进行值比较则需要枚举对象继承 IntEnum 对象，举个例子：
```python
import unittest
from enum import Enum, IntEnum

class TestEnum(unittest.TestCase):
    class Season(IntEnum):
        SPRING = 1
        SUMMER = 2
        AUTUMN = 3
        WINTER = 4

    def test_comparisons(self):
        season = self.Season

        self.assertEqual(season.SPRING, 1)

        class Part(Enum):
            SPRING = 1
            CLIP = 2
            BARREL = 3
            
        self.assertNotEqual(Part.SPRING, 1)
        self.assertNotEqual(Part.SPRING, season.SPRING)

TestEnum().test_comparisons()
```
上面的测试例子当中，创建了两个继承类型不一样的枚举类，可以看到继承了 `IntEnum` 的 Season 可以进行值的比较，而继承了 Enum 的 Part 则不能进行值比较，并且 IntEnum 类型与 Enum 类型也不能进行比较，即使属性和值一样。
<a name="nNStz"></a>
## 枚举方法
枚举中可以定义枚举类自身特有的方法，也可以复写一些已经在基类中定义好的方法，比如: `__init__`, `__str__`,`__repr__`,`__hash__`,`__format__` 等。举个例子：
```python
from enum import Enum

class Mood(Enum):
    FUNKY = (1, "hello")
    HAPPY = (3, "world")

    def describe(self):
        return self.name, self.value

    def __init__(self, num, nice):
        self.num = num
        self.nice = nice

    def __str__(self):
        return 'my custom str! {0}'.format(self.value)

    @classmethod
    def favorite_mood(cls):
        return cls.HAPPY

    @property
    def testValue(self):
        return self.nice + ':' + str(self.num)
```
上面定义了一个枚举类，其中 value 是一个枚举类型，可以定义 `__init__` 方法去对应元组中的值，也复写了 `__str__` 方法。<br />打印方法看看效果：
```python
print(Mood.favorite_mood())     # my custom str! (3, 'world')
print(Mood.HAPPY.describe())    # ('HAPPY', (3, 'world'))
print(str(Mood.FUNKY))          # my custom str! (1, 'hello')
print(Mood.FUNKY.testValue)     # hello:1
```
从输出结果看，自定义和复写的方法都已经成功的应用到了 Mood 类中。
<a name="I4M8t"></a>
## 枚举继承
不同于 java 中的枚举类，Python 中的枚举类是可以被继承的，但是被继承的枚举类规定其不能定义任何成员，但可以定义抽象方法。举例如下：
```python

class EnumExtend(unittest.TestCase):

    def test_extending(self):
        class Shade(Enum):
            def shade(self):
                print(self.name)

        class Color(Shade):
            red = 1
            green = 2
            blue = 3
        with self.assertRaises(TypeError):
            class MoreColor(Color):
                cyan = 4
                magenta = 5
                yellow = 6

    def test_extending2(self):
        class Shade(Enum):
            def shade(self):
                return self.name

        class Color(Shade):
            def hex(self):
                return '%s nice!' % self.value

        class MoreColor(Color):
            cyan = 4
            magenta = 5
            yellow = 6
        self.assertEqual(MoreColor.magenta.shade(), 'magenta')
        self.assertEqual(MoreColor.magenta.hex(), '5 nice!')
```
测试用例可以完美运行，可以发现：第一个方法中，抛出了 TypeError 的异常；第二个测试方法中，`MoreColor` 继承了 `Color`， `Color` 继承了 `Shade`，并且可以通过子类调用父类中的方法。
<a name="qhtNA"></a>
## 总结
主要介绍了 enum 模块的基础知识，包含枚举的创建、枚举成员和属性的访问、枚举方法的创建、枚举的继承等。其中新版中的 `_ignore_`、`_order_`、`_missing_`等可以学习官网的例子，另外 enum 的子类 `IntEnum`、`IntFlag`等也是比较常用的枚举基类，这里简单的介绍了 `IntEnum`， 而 `IntFlag` 相比与 `IntEnum` 多了 `&`, `|`, `^`, `~` 的操作，其他的子类大家感兴趣也可以了解。
