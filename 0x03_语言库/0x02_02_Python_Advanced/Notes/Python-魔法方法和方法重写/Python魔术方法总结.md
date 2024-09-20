Python
<a name="TTGcD"></a>
## 见过这几个魔术方法吗？
下列这几个魔术方法经常被使用，不知道你是否认识它：
```
__init__
__new__
__add__
__len__
```
前后都有双下划线的方法在Python中被称为魔术方法。<br />之所以被称为魔术方法，是因为它们有特殊的含义，用一个例子来说明。
<a name="DhrlQ"></a>
## 一个例子学懂魔术方法
可以给整数，或者字符串做加法：
```python
> 3 + 2
5
> 'hello' + 'maishu'
'hellomaishu'
```
来定义一个表示城市的类，它有两个属性名字(name)和人口数(pop):
```python
class City:
    def __init__(self, name, pop):
        self.name = name
        self.pop = pop

city1 = City('青岛市', 900)
city2 = City('海口市', 600)

city3 = city1 + city2
```
在程序的最后一行，试图把两个城市相加，可惜程序报错了：
```
TypeError: unsupported operand type(s) for +: 'City' and 'City'
```
错误上说，City不支持加号。如何让它支持加号呢？<br />很简单，只要给城市添加一个魔术方法`__add__`就搞定了：
```python
class City:
    def __init__(self, name, pop):
        self.name = name
        self.pop = pop

    def __add__(self, city):
        new_name = self.name + city.name 
        new_pop = self.pop + city.pop 
        return City(new_name, new_pop)

city1 = City('青岛市', 900)
city2 = City('海口市', 600)

city3 = city1 + city2
```
给City增加了一个`__add__`方法，它的逻辑很简单，就是把名字相拼，人口相加，然后创建了一个新的城市。<br />这样程序的最后一行就不会报错了。<br />这说明这个`__add__`方法有一定魔力，当用加号`+`的时候，Python的解释器会自动去寻找`__add__`方法，如果这个对象没有这个方法，就报错了。<br />如果再City加上下面这几个方法，那么City就可以做加减乘除了：
```python
__sub__
__div__
__mul__
```
整数之所以可以做加减乘除，也正是因为它有这几个方法。<br />在交互式Python下运行`dir(int)`命令，仔细找一下是否有这几个方法：
```python
>>> dir(int)
['__abs__', '__add__', '__and__', '__bool__', '__ceil__',
 '__class__', '__delattr__', '__dir__', '__divmod__', '__doc__', '__eq__', '__float__', '__floor__', '__floordiv__',
 '__format__', '__ge__', '__getattribute__', '__getnewargs__', 
 '__gt__', '__hash__', '__index__', '__init__', 
 '__init_subclass__', '__int__', '__invert__',
 '__le__', '__lshift__', '__lt__', '__mod__',
 '__mul__', '__ne__', '__neg__', '__new__', '__or__', 
 '__pos__', '__pow__', '__radd__', '__rand__', '__rdivmod__', 
 '__reduce__', '__reduce_ex__', '__repr__', '__rfloordiv__', 
 '__rlshift__', '__rmod__', '__rmul__', '__ror__', '__round__', 
 '__rpow__', '__rrshift__', '__rshift__', '__rsub__', '__rtruediv__',
 '__rxor__', '__setattr__', '__sizeof__', '__str__', '__sub__', 
 '__subclasshook__', '__truediv__', '__trunc__', '__xor__', 
 'as_integer_ratio', 'bit_length', 'conjugate', 'denominator', 
 'from_bytes', 'imag', 'numerator', 'real', 'to_bytes']
```
再试一下`dir(str)`，会发现str没有`__sub__`和`__div__`方法，所以它只能做加法和乘法，不能做减法，和除法。
<a name="Hnov3"></a>
## 魔术方法的定义
魔术方法对Python至关重要，可以说Python就是运行在魔术方法的轮子之上。

- 魔术方法是形如`__xxx__`的方法。
- 魔术方法都是内定的，具有特定的含义。
- 自己随便定义一个`__xxx__`方法是没用的。
- 每个Python的运算符的背后都有一个魔术方法，比如加法对应`__add__`
- 熟悉的for循环就运行在魔术方法之上，被循环的对象（list等）之所以可以被循环，是因为它们有`__iter__`和`__next__`两个魔术方法。
- 熟悉的`len()`函数，`del`操作符等都有背后的魔术方法。
- 知道为什么魔术方法是内定的了吧？因为它们和运算符是对应的，可以说是一个萝卜一个坑。
<a name="HveII"></a>
## 魔术方法的好处
魔术方法可以说是Python的一个优点，因为它可以非常灵活的实现运算符。<br />以上面的City的例子，通过实现魔术方法可以让City支持各种操作，包括可以被循环等。<br />使用最多的魔术方法一定是`__init__`方法，因为定义类就需要实现这个方法。
<a name="SA14f"></a>
## 魔术属性
除了魔术方法，还有魔术属性，就是一些形如`__yyy__`的属性。这些属性通常是Python自动设置的属性，但可以使用这些属性。比如：
```python
class City:
    #--省略代码--

print(__name__)
print(__file__)
```
`__name__`是一个很重要的特殊属性，可以用它来判定当前模块是被直接调用的，还是被`import`进来的。<br />`__file__`会打印出当前文件所在的目录和文件名。
<a name="aua31"></a>
## 查看所有魔术方法和属性
下面的代码从内存中找出所有对象，并打印每个对象中的魔术方法：
```python
import gc
print("\n".join(sorted({attrname for item in gc.get_objects() for attrname in dir(item) if attrname.startswith("__")})))
```
<a name="H3cvb"></a>
## 常用魔术方法
<a name="QozAl"></a>
### 初始化和构造
| 初始化和构造 | 描述 |
| --- | --- |
| `__new__(cls, other)` | 对象实例化时被调用. |
| `__init__(self, other)` | new 方法被调用时调用. |
| `__del__(self)` | 对象销毁时被调用. |

<a name="oTqbS"></a>
### 一元运算符
| 一元运算符 | 解释 |
| --- | --- |
| `__pos__(self)` | 实现‘+’号一元运算符，比如：+a. |
| `__neg__(self)` | 实现‘-’号一元运算符，比如：-a. |
| `__abs__(self)` | 调用abs()方法时被调用. |
| `__invert__(self)` | 实现‘~’号一元运算符，比如：~a. |
| `__round__(self,n)` | 调用`round()`方法时被调用. |
| `__floor__(self)` | 调用`math.floor()`方法时被调用. |
| `__ceil__(self)` | 调用`math.ceil()`方法时被调用. |
| `__trunc__(self)` | 调用`math.trunc()`方法时被调用. |

<a name="wTmXh"></a>
### 增量赋值
| 增量赋值 | 解释 |
| --- | --- |
| `__iadd__(self, other)` | 使用‘+=’时被调用. 比如：a += b. |
| `__isub__(self, other)` | 使用‘-=’时被调用. 比如：a -= b. |
| `__imul__(self, other)` | 使用‘*=’时被调用. 比如：a *= b. |
| `__ifloordiv__(self, other)` | 使用‘//=’时被调用. 比如 a //= b. |
| `__idiv__(self, other)` | 使用‘/=’时被调用. 比如 a /= b. |
| `__itruediv__(self, other)` | 实现真除法。注意只有只用了 from future import division 的时候才会起作用 |
| `__imod__(self, other)` | 使用‘%=’时被调用. 比如 a %= b. |
| `__ipow__(self, other)` | 使用‘**=’时被调用. 比如：a **= b. |
| `__ilshift__(self, other)` | 使用‘<<=’时被调用. 比如：a <<= b. |
| `__irshift__(self, other)` | 使用‘>>=’时被调用. 比如：a >>= b. |
| `__iand__(self, other)` | 使用‘&=’时被调用. 比如：a &= b. |
| `__ior__(self, other)` | 使用‘&#124;=’时被调用. 比如：a &#124;= b. |
| `__ixor__(self, other)` | 使用‘^=’时被调用. 比如：a ^= b. |

<a name="otsjK"></a>
### 类型转换
| 类型转换 | 解释 |
| --- | --- |
| `__int__(self)` | 调用`int()`方法时被调用. |
| `__float__(self)` | 调用`float()`方法时被调用. |
| `__complex__(self)` | 调用`complex()`方法时被调用. |
| `__oct__(self)` | 调用`oct()`方法时被调用. |
| `__hex__(self)` | 调用`hex()`方法时被调用. |
| `__index__(self)` | 当对象是被应用在切片表达式中时，实现整形强制转换. |
| `__trunc__(self)` | 调用`math.trunc()`方法时被调用. |

<a name="Y08uB"></a>
### 字符串
| 字符串 | 解释 |
| --- | --- |
| `__str__(self)` | 调用`str()`方法时被调用. |
| `__repr__(self)` | 调用`repr()`方法时被调用. |
| `__unicode__(self)` | 调用`unicode()`方法时被调用. |
| `__format__(self, formatstr)` | 调用`string.format()`方法时被调用. |
| `__hash__(self)` | 调用`hash()`方法时被调用. |
| `__nonzero__(self)` | 调用`bool()`方法时被调用. |
| `__dir__(self)` | 调用`dir()`方法时被调用. |
| `__sizeof__(self)` | 调用`sys.getsizeof()`方法时被调用. |

<a name="eiZSl"></a>
### 属性魔术方法
| 属性魔术方法 | 解释 |
| --- | --- |
| `__getattr__(self, name)` | 定义当用户试图获取一个不存在的属性时的行为. |
| `__setattr__(self, name, value)` | 定义对属性的赋值行为，无论属性是否存在. |
| `__delattr__(self, name)` | 定义删除属性的行为. |

<a name="xMev8"></a>
### 操作符
| 操作符 | 解释 |
| --- | --- |
| `__add__(self, other)` | 实现加法（+）. |
| `__sub__(self, other)` | 实现减法（-）. |
| `__mul__(self, other)` | 实现乘法（*）. |
| `__floordiv__(self, other)` | 实现地板除（//）. |
| `__truediv__(self, other)` | 实现真除法（/）. |
| `__mod__(self, other)` | 实现取模运算（%）. |
| `__pow__(self, other[, modulo])` | 实现指数运算(**）. |
| `__lt__(self, other)` | 实现小于运算（<）. |
| `__le__(self, other)` | 实现小于等于运算（<=）. |
| `__eq__(self, other)` | 定义了等号的行为（==）. |
| `__ne__(self, other)` | 定义了不等号的行为（!=）. |
| `__ge__(self, other)` | 定义了大于等于运算（>=）. |

