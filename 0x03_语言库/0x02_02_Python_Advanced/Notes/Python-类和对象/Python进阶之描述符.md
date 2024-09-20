Python 面向对象<br />在 Python 开发中，可能听说过「描述符」这个概念，由于很少直接使用它，所以大部分开发人员并不了解它的原理。<br />但作为熟练使用 Python，想要进阶建议还是了解一下描述符的原理，这也便于更深层次地理解 Python 的设计思想。<br />其实，在开发过程中，虽然没有直接使用到描述符，但是它在底层却无时不刻地被使用到，例如以下这些：

- `function`、`bound method`、`unbound method`
- 装饰器`property`、`staticmethod`、`classmethod`

是不是都很熟悉？<br />这些都与描述符有着千丝万缕的关系，来看一下描述符背后的工作原理。
<a name="XpD8r"></a>
## 什么是描述符？
在解释什么是「描述符」之前，先来看一个简单的例子。
```python
class A:
    x = 10
    
print(A.x) # 10
```
这个例子非常简单，在类 A 中定义了一个类属性 x，然后打印它的值。<br />其实，除了直接定类属性之外，还可以这样定义一个类属性：
```python
class Ten:
    def __get__(self, obj, objtype=None):
        return 10

class A:
    x = Ten()   # 属性换成了一个类
    
print(A.x) # 10
```
仔细看，这次类属性 x 不再是一个具体的值，而是一个类 Ten。Ten 中定义了一个 `__get__` 方法，返回具体的值。<br />在 Python 中，允许把一个类属性，托管给一个类，这个属性就是一个「描述符」。<br />换句话说，「描述符」是一个「绑定行为」的属性。<br />怎么理解这句话？<br />回忆一下，开发时，一般把「行为」叫做什么？是的，「行为」一般指的是一个方法。<br />所以也可以把「描述符」理解为：对象的属性不再是一个具体的值，而是交给了一个方法去定义。<br />可以想一下，如果用一个方法去定义一个属性，这么做的好处是什么？<br />有了方法，就可以在方法内实现自己的逻辑，最简单的，可以根据不同的条件，在方法内给属性赋予不同的值，就像下面这样：
```python
class Age:
    def __get__(self, obj, objtype=None):
        if obj.name == 'zhangsan':
            return 20
        elif obj.name == 'lisi':
            return 25
        else:
            return ValueError("unknow")

class Person:

    age = Age()

    def __init__(self, name):
        self.name = name

p1 = Person('zhangsan')
print(p1.age)   # 20

p2 = Person('lisi')
print(p2.age)   # 25

p3 = Person('wangwu')
print(p3.age)   # unknow
```
这个例子中，age 类属性被另一个类托管了，在这个类的 `__get__` 中，它会根据 Person 类的属性 name，决定 age 是什么值。<br />这只是一个非常简单的例子，可以看到，通过描述符的使用，可以轻易地改变一个类属性的定义方式。
<a name="hoCDt"></a>
## 描述符协议
了解了描述符的定义，现在把重点放到托管属性的类上。<br />其实，一个类属性想要托管给一个类，这个类内部实现的方法不能是随便定义的，它必须遵守「描述符协议」，也就是要实现以下几个方法：

- `__get__(self, obj, type=None)`
- `__set__(self, obj, value)`
- `__delete__(self, obj)`

只要是实现了以上几个方法的其中一个，那么这个类属性就可以称作描述符。<br />另外，描述符又可以分为「数据描述符」和「非数据描述符」：

- 只定义了 `__get__`，叫做非数据描述符
- 除了定义 `__get__` 之外，还定义了 `__set__` 或 `__delete__`，叫做数据描述符

它们两者有什么区别，会在下面详述。<br />现在来看一个包含 `__get__` 和 `__set__` 方法的描述符例子：
```python
# coding: utf8

class Age:

    def __init__(self, value=20):
        self.value = value

    def __get__(self, obj, type=None):
        print('call __get__: obj: %s type: %s' % (obj, type))
        return self.value

    def __set__(self, obj, value):
        if value <= 0:
            raise ValueError("age must be greater than 0")
        print('call __set__: obj: %s value: %s' % (obj, value))
        self.value = value

class Person:

    age = Age()

    def __init__(self, name):
        self.name = name

p1 = Person('zhangsan')
print(p1.age)
# call __get__: obj: <__main__.Person object at 0x1055509e8> type: <class '__main__.Person'>
# 20

print(Person.age)
# call __get__: obj: None type: <class '__main__.Person'>
# 20

p1.age = 25
# call __set__: obj: <__main__.Person object at 0x1055509e8> value: 25

print(p1.age)
# call __get__: obj: <__main__.Person object at 0x1055509e8> type: <class '__main__.Person'>
# 25

p1.age = -1
# ValueError: age must be greater than 0
```
在这例子中，类属性 age 是一个描述符，它的值取决于 Age 类。<br />从输出结果来看，当获取或修改 age 属性时，调用了 Age 的 `__get__` 和 `__set__` 方法：

- 当调用 `p1.age` 时，`__get__` 被调用，参数 obj 是 Person 实例，type 是 `type(Person)`
- 当调用 Person.age 时，`__get__` 被调用，参数 obj 是 None，type 是 `type(Person)`
- 当调用 `p1.age = 25`时，`__set__` 被调用，参数 obj 是 Person 实例，value 是25
- 当调用 `p1.age = -1`时，`__set__` 没有通过校验，抛出 ValueError

其中，调用 `__set__` 传入的参数，比较容易理解，但是对于 __get__ 方法，通过类或实例调用，传入的参数是不同的，这是为什么？<br />这就需要了解一下描述符的工作原理。
<a name="fXiHz"></a>
## 描述符的工作原理
要解释描述符的工作原理，首先需要先从属性的访问说起。<br />在开发时，不知道有没有想过这样一个问题：通常写这样的代码 `a.b`，其背后到底发生了什么？<br />这里的 a 和 b 可能存在以下情况：

1. a 可能是一个类，也可能是一个实例，这里统称为对象
2. b 可能是一个属性，也可能是一个方法，方法其实也可以看做是类的属性

其实，无论是以上哪种情况，在 Python 中，都有一个统一的调用逻辑：

1. 先调用 `__getattribute__` 尝试获得结果
2. 如果没有结果，调用 `__getattr__`

用代码表示就是下面这样：
```python
def getattr_hook(obj, name):
    try:
        return obj.__getattribute__(name)
    except AttributeError:
        if not hasattr(type(obj), '__getattr__'):
            raise
    return type(obj).__getattr__(obj, name) 
```
这里需要重点关注一下 `__getattribute__`，因为它是所有属性查找的入口，它内部实现的属性查找顺序是这样的：

1. 要查找的属性，在类中是否是一个描述符
2. 如果是描述符，再检查它是否是一个数据描述符
3. 如果是数据描述符，则调用数据描述符的 `__get__`
4. 如果不是数据描述符，则从 `__dict__` 中查找
5. 如果 `__dict__` 中查找不到，再看它是否是一个非数据描述符
6. 如果是非数据描述符，则调用非数据描述符的 `__get__`
7. 如果也不是一个非数据描述符，则从类属性中查找
8. 如果类中也没有这个属性，抛出 `AttributeError` 异常

写成代码就是下面这样：
```python
# 获取一个对象的属性
def __getattribute__(obj, name):
    null = object()
    # 对象的类型 也就是实例的类
    objtype = type(obj)
    # 从这个类中获取指定属性
    cls_var = getattr(objtype, name, null)
    # 如果这个类实现了描述符协议
    descr_get = getattr(type(cls_var), '__get__', null)
    if descr_get is not null:
        if (hasattr(type(cls_var), '__set__')
            or hasattr(type(cls_var), '__delete__')):
            # 优先从数据描述符中获取属性
            return descr_get(cls_var, obj, objtype)
    # 从实例中获取属性
    if hasattr(obj, '__dict__') and name in vars(obj):
        return vars(obj)[name]
    # 从非数据描述符获取属性
    if descr_get is not null:
        return descr_get(cls_var, obj, objtype)
    # 从类中获取属性
    if cls_var is not null:
        return cls_var
    # 抛出 AttributeError 会触发调用 __getattr__
    raise AttributeError(name)
```
如果不好理解，最好写一个程序测试一下，观察各种情况下的属性的查找顺序。<br />到这里可以看到，在一个对象中查找一个属性，都是先从 `__getattribute__` 开始的。<br />在 `__getattribute__` 中，它会检查这个类属性是否是一个描述符，如果是一个描述符，那么就会调用它的 __get__ 方法。但具体的调用细节和传入的参数是下面这样的：

- 如果 a 是一个实例，调用细节为：
```python
type(a).__dict__['b'].__get__(a, type(a))
```

- 如果 a 是一个类，调用细节为：
```python
a.__dict__['b'].__get__(None, a)
```
所以就能看到上面例子输出的结果。
<a name="UcLcD"></a>
## 数据描述符和非数据描述符
了解了描述符的工作原理，继续来看数据描述符和非数据描述符的区别。<br />从定义上来看，它们的区别是：

- 只定义了 `__get__`，叫做非数据描述符
- 除了定义 `__get__` 之外，还定义了 `__set__` 或 `__delete__`，叫做数据描述符

此外，从上面描述符调用的顺序可以看到，在对象中查找属性时，数据描述符要优先于非数据描述符调用。<br />在之前的例子中，定义了 `__get__` 和 `__set__`，所以那些类属性都是数据描述符。<br />再来看一个非数据描述符的例子：
```python
class A:

    def __init__(self):
        self.foo = 'abc'

    def foo(self):
        return 'xyz'

print(A().foo)  # 输出什么？
```
这段代码，定义了一个相同名字的属性和方法 foo，如果现在执行 `A().foo`，会输出什么结果？<br />答案是 abc。<br />为什么打印的是实例属性 foo 的值，而不是方法 foo 呢？<br />这就和非数据描述符有关系了。<br />执行 `dir(A.foo)`，观察结果：
```python
print(dir(A.foo))
# [... '__get__', '__getattribute__', ...]
```
看到了吗？A 的 foo 方法其实实现了 `__get__`，在上面的分析已经得知：只定义 `__get__` 方法的对象，它其实是一个非数据描述符，也就是说，在类中定义的方法，其实本身就是一个非数据描述符。<br />所以，在一个类中，如果存在相同名字的属性和方法，按照上面所讲的 `__getattribute__` 中查找属性的顺序，这个属性就会优先从实例中获取，如果实例中不存在，才会从非数据描述符中获取，所以在这里优先查找的是实例属性 foo 的值。<br />到这里可以总结一下关于描述符的相关知识点：

- 描述符必须是一个类属性
- `__getattribute__` 是查找一个属性（方法）的入口
- `__getattribute__` 定义了一个属性（方法）的查找顺序：数据描述符、实例属性、非数据描述符、类属性
- 如果重写了 `__getattribute__` 方法，会阻止描述符的调用
- 所有方法其实都是一个非数据描述符，因为它定义了 `__get__`
<a name="msKDt"></a>
## 描述符的使用场景
了解了描述符的工作原理，那描述符一般用在哪些业务场景中呢？<br />在这里用描述符实现了一个属性校验器，可以参考这个例子，在类似的场景中去使用它。<br />首先定义一个校验基类 `Validator`，在 `__set__` 方法中先调用 `validate` 方法校验属性是否符合要求，然后再对属性进行赋值。
```python
class Validator:

    def __init__(self):
        self.data = {}

    def __get__(self, obj, objtype=None):
        return self.data[obj]

    def __set__(self, obj, value):
        # 校验通过后再赋值
        self.validate(value)
        self.data[obj] = value

    def validate(self, value):
        pass    
```
接下来，定义两个校验类，继承 Validator，然后实现自己的校验逻辑。
```python
class Number(Validator):

    def __init__(self, minvalue=None, maxvalue=None):
        super(Number, self).__init__()
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

class String(Validator):

    def __init__(self, minsize=None, maxsize=None):
        super(String, self).__init__()
        self.minsize = minsize
        self.maxsize = maxsize

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
```
最后，使用这个校验类：
```python
class Person:

    # 定义属性的校验规则 内部用描述符实现
    name = String(minsize=3, maxsize=10)
    age = Number(minvalue=1, maxvalue=120)

    def __init__(self, name, age):
        self.name = name
        self.age = age

# 属性符合规则
p1 = Person('zhangsan', 20)
print(p1.name, p1.age)

# 属性不符合规则
p2 = person('a', 20)
# ValueError: Expected 'a' to be no smaller than 3
p3 = Person('zhangsan', -1)
# ValueError: Expected -1 to be at least 1
```
现在，对 Person 实例进行初始化时，就可以校验这些属性是否符合预定义的规则了。
<a name="Q2PBc"></a>
## function与method
再来看一下，在开发时经常看到的 `function`、`unbound method`、`bound method` 它们之间到底有什么区别？<br />来看下面这段代码：
```python
class A:

    def foo(self):
        return 'xyz'

print(A.__dict__['foo']) # <function foo at 0x10a790d70>
print(A.foo)     # <unbound method A.foo>
print(A().foo)   # <bound method A.foo of <__main__.A object at 0x10a793050>>
```
从结果可以看出它们的区别：

- function 准确来说就是一个函数，并且它实现了 `__get__` 方法，因此每一个 function 都是一个非数据描述符，而在类中会把 function 放到 `__dict__` 中存储
- 当 function 被实例调用时，它是一个 bound method
- 当 function 被类调用时， 它是一个 unbound method

function 是一个非数据描述符，之前已经讲到了。<br />而 bound method 和 unbound method 的区别就在于调用方的类型是什么，如果是一个实例，那么这个 function 就是一个 bound method，否则它是一个 unbound method。
<a name="cn63K"></a>
## property/staticmethod/classmethod
再来看 property、staticmethod、classmethod。<br />这些装饰器的实现，默认是 C 来实现的。<br />其实，也可以直接利用 Python 描述符的特性来实现这些装饰器，<br />property 的 Python 版实现：
```python
class property:

    def __init__(self, fget=None, fset=None, fdel=None, doc=None):
        self.fget = fget
        self.fset = fset
        self.fdel = fdel
        self.__doc__ = doc

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self.fget
        if self.fget is None:
            raise AttributeError(), "unreadable attribute"
        return self.fget(obj)

    def __set__(self, obj, value):
        if self.fset is None:
            raise AttributeError, "can't set attribute"
        return self.fset(obj, value)

    def __delete__(self, obj):
        if self.fdel is None:
            raise AttributeError, "can't delete attribute"
        return self.fdel(obj)

    def getter(self, fget):
        return type(self)(fget, self.fset, self.fdel, self.__doc__)

    def setter(self, fset):
        return type(self)(self.fget, fset, self.fdel, self.__doc__)

    def deleter(self, fdel):
        return type(self)(self.fget, self.fset, fdel, self.__doc__)


```
staticmethod 的 Python 版实现：
```python
class staticmethod:

    def __init__(self, func):
        self.func = func

    def __get__(self, obj, objtype=None):
        return self.func
```
classmethod 的 Python 版实现：
```python
class classmethod:

    def __init__(self, func):
        self.func = func

    def __get__(self, obj, klass=None):
        if klass is None:
            klass = type(obj)
        def newfunc(*args):
            return self.func(klass, *args)
        return newfunc
```
除此之外，还可以实现其他功能强大的装饰器。<br />由此可见，通过描述符可以实现强大而灵活的属性管理功能，对于一些要求属性控制比较复杂的场景，可以选择用描述符来实现。
<a name="wBr6q"></a>
## 总结
主要讲了 Python 描述符的工作原理。<br />首先，从一个简单的例子了解到，一个类属性是可以托管给另外一个类的，这个类如果实现了描述符协议方法，那么这个类属性就是一个描述符。此外，描述符又可以分为数据描述符和非数据描述符。<br />之后又分析了获取一个属性的过程，一切的入口都在 `__getattribute__` 中，这个方法定义了寻找属性的顺序，其中实例属性优先于数据描述符调用，数据描述符要优先于非数据描述符调用。<br />另外又了解到，方法其实就是一个非数据描述符，如果在类中定义了相同名字的实例属性和方法，按照 `__getattribute__` 中的属性查找顺序，实例属性优先访问。<br />最后分析了 `function` 和 `method` 的区别，以及使用 Python 描述符也可以实现 `property`、`staticmethod`、`classmethod` 装饰器。<br />Python 描述符提供了强大的属性访问控制功能，可以在需要对属性进行复杂控制的场景中去使用它。
