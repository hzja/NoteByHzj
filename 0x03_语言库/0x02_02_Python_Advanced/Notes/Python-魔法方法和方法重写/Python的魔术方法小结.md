Python
<a name="xcW2r"></a>
## 特殊属性
| 属性 | 含义 |
| --- | --- |
| `__name__` | 类、函数、方法的名字 |
| `__module__` | 类定义所在的模块名 |
| `__class__` | 对象或类所属的类 |
| `__bases__` | 类的基类的元组 |
| `__doc__` | 类、函数的文档字符串，如果没有定义则为None |
| `__mro__` | 类的mro，`class.mro()`返回的结果保存在此 |
| `__dict__` | 类或者实例的属性，可写的字典 |
| `__dir__` | 返回类或者对象的所有成员名称列表，`dir()`函数就是调用`__dir__()`，如果提供`__dir__()`，则返回属性的列表，否则会尽量从`__dict__`属性中收集信息 |

<a name="l0DHQ"></a>
## 魔术方法
<a name="HFm25"></a>
### 分类

- 创建、初始化与销毁
   - `__init__` 和 `__del__`
- `hash`
- `bool`
- 可视化
- 运算符重载
- 容器和大小
- 可调用对象
- 上下文管理
- 反射
<a name="aDoUb"></a>
### `__hash__`
`hash()`的返回值，返回一个整数。如果定义这个方法该类实例就是可hash的。
```python
class A:
    def __init__(self, name, age=18):
        self.name = name
    
    def __hash__(self):
        return 1
    
    def __repr__(self):
        return self.name

print(hash(A('tom')))
print((A('tom'),A('tom')))
print([A('tom'),A('tom')])
print('~~~~~~~~~~~~~')
s = {A('tom'),A('tom')}
print(s)
print({tuple('t'),tuple('t')})
print({('tom',),('tom',)})
print({'tom','tom'})
```
上例说明一个问题：哈希值相等并不意味着元素相等。两个元素可以有相同的哈希值（哈希冲突）。上例中，如果想在set中剔除相同的key，需要加上`__eq__`魔术方法
```python
class A:
    def __init__(self,name,age=18):
        self.name = name

    def __hash__(self):
        return 1

    def __eq__(self,other):
        return self.name == other.name

    def __repr__(self):
        return self.name

print({A('tom'),A('tom')})
```
<a name="X8Mg8"></a>
### `__eq__`
对应==操作符，判断两个对象是否相等，返回bool值<br />和hash魔术方法的比较：

- `__hash__`只是返回hash值作为set的key，但是去重需要`__eq__`来判断两个对象是否相等
- hash值相等只是hash冲突，不能说明两个对象相等
- 因此，一般提供`__hash__`方法只是为了作为set或者dict的key
- 不可hash对象`isinstance(p1,collections.Hashable)`一定是False
<a name="EqwJZ"></a>
### `__bool__`
内建函数`bool()`,对象放在逻辑表达式的位置，调用这个函数返回bool值，没有定义`__bool__()`就找`__len__()`返回长度，非0为真，如果__len__也没有定义，则所有实例都为真
<a name="DvoOd"></a>
## 可视化魔术方法
<a name="iLkJm"></a>
### `__repr__`
内建函数`repr()`对一个对象获取字符串表达，调用该方法返回字符串表达，如果`__repr__`也没有定义，就直接返回object的定义，显示内存地址信息
<a name="LQIpi"></a>
### `__str__`
`str()`函数，内建函数`format()`、`print()`函数调用，需要返回对象的字符串表达。如果没有定义，就去调用`__repr__`方法。返回字符串表达，如果`__repr__`没有定义，就直接返回对象的内存地址信息
<a name="X3J9H"></a>
### `__bytes__`
`bytes()`函数调用，返回一个对象的`bytes`表达，返回`bytes`对象
```python
class A:
    def __init__(self,name,age=18):
        self.name = name
        self.age = age

    def __repr__(self):
        return 'repr:{},{}'.format(self.name,self.age)

    def __str__(self):
        return 'str: {},{}'.format(self.name,self.age)

    def __bytes__(self):
        import json
        return json.dumps(self.__dict__).encode()

print(A('tom'))
print([A('tom')]) # 调用了列表的__repr__
print([str(A('tom'))])
```
<a name="Sl2gi"></a>
## 运算符重载
| 运算符 | 特殊方法 | 含义 |
| --- | --- | --- |
| <,<=,==,>,>=,!= | `__lt__`,`__le__`,`__eq__`,`__gt__`,`__ge__`,`__ne__` | 比较运算符 |
| +,-,*,/,%,/,**，divmod | `__add__`,`__sub__`,`__mul__`,`__truediv__`,`__mod__`,`__floordiv__`,`__pow__`,`__divmod__` | 算术运算符，移位、位运算也有对应的方法 |

<a name="He1tW"></a>
### `@functools.total_ordering`装饰器
实现了各种算术运算符的重载，可以简化代码，但是少用这个装饰器
<a name="gA8Jh"></a>
## 容器相关方法
| 方法 | 意义 |
| --- | --- |
| `__len__` | 内建函数`len()`，返回对象的长度，如果把对象当容器类型看，就如同list或者dict。`__boool__`函数调用的时候，如果没有`__bool__()`方法，则会看`__len__()`方法是否存在，存在即返回非0为真 |
| `__iter__` | 迭代容器时调用，返回一个新的迭代器 |
| `__contains__` | in成员运算符，没有实现就调用`__iter__`方法遍历 |
| `__getitem__` | 实现`self[key]`访问。序列对象，key接受整数为索引，或者切片。对于set和dict，key为`hashable`。key不存在则引发keyerror异常 |
| `__setitem__` | 和`__getitem__`的访问类似，是设置值的方法 |
| `__missing__` | 字典或其子类使用。`**getitem**()`调用时，key不存在执行该方法 |

<a name="ymWKZ"></a>
## 可调用对象
python中一切皆对象，包括函数<br />调用函数时其实是调用对象的`__call__`方法。
<a name="LrSx1"></a>
## 上下文管理
当一个类同时实现了`__enter__()`和`__exit__()`方法，它就属于上下文管理的对象<br />`__enter__`进入此对象相关的上下文。如果存在该方法，`with`会把该方法的返回值作为绑定到as字句中的指定变量上<br />exit 退出与此对象相关的上下文方法，保证了发生异常时不会影响exit魔术方法的
```python
class Point:
    def __init__(self):
        print('init')
    def __enter__(self):
        print('enter')
    def __exit__(self,exc_type,exc_val,exc_tb):
        print('exit')

with Point() as f:
    print('do sth')
```
`with … as …`语法，实际上是把`__enter__`方法赋值给as后的变量。
<a name="B4Uy8"></a>
### 参数

- `__enter__`方法没有其他参数
- `__exit__`方法有三个参数，如果上下文退出的时候没有异常，这三个参数都是None：
   - `exc_type` 异常类型
   - `exc_value` 异常值
   - `traceback` 异常的追踪信息
   - `__exit__`方法返回一个等效True的值，则压制异常，否则，继续抛出异常
```python
class Point:
    def __init__(self):
        print('init')

    def __enter__(self):
        print('enter')
        return
    def __exit__(self,exc_type,exc_val,exc_tb):
        print(exc_type)
        print(exc_val)
        print(exc_tb)
        print('exit')
        return "abc"

p=Point()
with p as f:
    raise Exception('New Error')
    print('do sth')
print('outer')
```
<a name="k22nh"></a>
### 上下文应用场景

1. 增强功能。在代码执行前后增强代码，类似装饰器的功能
2. 资源管理。打开了资源需要关闭，例如文件对象，网络连接，数据库连接等等
3. 权限验证。执行代码之前做权限的验证，在`__enter__`中处理
<a name="iot52"></a>
## `contextlib.contextmanager`
这是一个装饰器，实现了上下文管理，他装饰一个函数，而不用像类一样实现`__enter__`和`__exit__`方法。
<a name="l7ugl"></a>
## 反射相关的函数和方法
<a name="BfJJo"></a>
### 反射内置函数

- `getattr(object,name[,default])` 通过name返回object的属性值。当属性不存在，将使用default返回，没有default抛出异常，name必须是字符串
- `setattr(object,name,value)` object属性存在则覆盖，不存在则新增
- `hasattr(object,name)` 判断对象是否有这个名字的属性，name必须为字符串
```python
class Point:
    def __init__(self,x,y):
        self.x = x
        self.y = y

    def __str__(self):
        return "Point({},{})".format(self.x, self.y)

    def show(self):
        print(self)

p1 = Point(4,5)
p2 = Point(10,10)
print(repr(p1),repr(p2),sep='\n')
print(p1.__dict__)
setattr(p1,'y',16)
setattr(p1,'z',10)
print(getattr(p1,'__dict__'))

# 动态增加方法
# 为类增加方法
if not hasattr(Point,'add'):
    setattr(Point,'add',lambda self,other: Point(self.x + other.x, self.y + other.y))

print(Point.add)
print(p1.add)
print(p1.add(p2)) # 绑定


# 为示例增加方法，未绑定
if not hasattr(p1,'sub'):
    setattr(p1,'sub',lambda self,other:Point(self.x - other.x, self.y - other.y))

print(p1.sub(p1,p1))
print(p1.sub)

# add在谁里面，sub在谁里面
print(p1.__dict__)
print(Point.__dict__)
```
<a name="Tp1n4"></a>
### 反射相关的魔术方法
<a name="yIU3B"></a>
#### `__getattr__()`
```python
class Base:
    n = 0

class Point(Base):
    z = 6
    def __init__(self,x,y):
        self.x = x
        self.y = y

    def show(self):
        print(self.x,self.y)

    def __getattr__(self,item):
        return "missing {}".format(item)

p1 = Point(4,5)
print(p1.x)
print(p1.z)
print(p1.n)
print(p1.t) # missing
```
一个类的属性会按照继承关系找，如果找不到，就会执行`__getattr__()`方法，如果没有这个方法，就会抛出异常表示找不到属性
<a name="QbiHO"></a>
#### `setattr()`
```python
class Point(Base):
    z = 6
    def __init__(self,x,y):
        self.x = x
        self.y = y

    def show(self):
        print(self.x,self.y)

    def __getattr__(self,item):
        return "missing {}".format(kry,value)

    def __setattr__(self,key,value):
        print("setattr {}={}".format(key,value))
        self.__dict__[key] = value
```
可以拦截对示例属性的增加、修改操作，如果要设置生效，需要自己操作实例的`__dict__`
<a name="Gkha2"></a>
#### `__getattribute__`方法
```python
class Base:
    n = 0

class Point(Base):
    z = 6
    def __init__(self,x,y):
        self.x = x
        self.y = y

    def __getattr__(self,item):
        return "missing {}".format(item)

    def __getattribute__(self,item):
        return item

p1 = Point(4,5)
print(p1.__dict__)
print(p1.x)
print(p1.z)
print(p1.n)
print(p1.t)
print(Point.__dict__)
print(Point.z)
```
示例的所有属性访问，第一个都会调用`__getattribute__`，它阻止了属性的查找，该方法应该返回（计算后的）值或者抛出异常。它的return值将作为属性查找的结果。它的return值将作为属性查找的结果，如果抛出异常，则直接调用`__getattr__`方法，因为属性没有找到。<br />`__getattribute__`方法中为了避免在该方法中无限递归，它的实现中应该永远调用基类的同名方法访问需要的任何属性，除非明确地知道`__getattribute__`方法用来做什么。
