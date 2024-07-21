# 类 class

## 示例

~~~python
# 定义1302类
class room:
    #定义sleep方法
    def sleep(self):
        print("睡觉")
    def eat(self):
        print("吃雪糕")
    def myname(self):
        print("我的名字是%s" % (self.name))

#定义树人对象
tree_people = room()
#树人有可以调用方法
tree_people.sleep()
tree_people.eat()
#树人可以定义方法
tree_people.eat = "吃米饭"
print(tree_people.eat)
#树人可以定义属性
tree_people.age = "22"
print(tree_people.age)
tree_people.name = "树人"
tree_people.myname()
~~~



##  封装

封装的本质是将事物相关的属性和方法封装在一个类里面，调用类创建实例时不关心类内部代码细节，相当于一个黑箱子只需该实例(黑箱)能给出想要结果即可



## 继承

子类可复用父类属性或方法，子类还可提供自己属性和方法



### 约束

方法一：

使用类的继承实现约束子类中必须要实现某个方法，否则就会报错

~~~ python
class BasePlugin(object):
    """
    基类，用于做约束。约束子类中必须实现process方法。
    """
    def process(self):
        raise NotImplementedError("%s中必须实现process方法" %self.__class__.__name__)

class DiskPlugin(BasePlugin):
    pass

class MemoryPlugin(BasePlugin):
    pass

obj = MemoryPlugin()
obj.process()
~~~

方法二：

~~~ python
import abc

class Base(abc.ABCMeta):
    @abc.abstractmethod
    def func(self):
        pass
    
class Foo(Base):
    def func(self):
        pass
~~~



## 多态

鸭子类型语言中，函数/方法可以接受一个任意类型的对象作为参数/返回值，只要该对象实现了代码后续用到的属性和方法就不会报错

<strong>一句话概括:同一个方法，不同对象调用该方法，实现的功能不一样，最直观的例子就是python中的”+”运算方法，在数字相加时，1+2=3，是正常意义上的加法，但是，’a’ + ‘b’ = ‘ab’，就是字符串的拼接，用在列表上：[1] + [2] = [1, 2]，就是列表拼接；</strong>



## 实例方法、静态方法和类方法

- 实例方法：由对象调用；至少一个self参数；执行实例方法时，自动将调用该方法的对象赋值给self；
- 类方法：由类调用； 至少一个cls参数；执行类方法时，自动将调用该方法的类赋值给cls；
- 静态方法：由类调用；无默认参数；

~~~ python
class Foo(object):
    def __init__(self, name):
        self.name = name

    def ord_func(self):
        """ 定义实例方法，至少有一个self参数 """
        # print(self.name)
        print('实例方法')

    @classmethod
    def class_func(cls):
        """ 定义类方法，至少有一个cls参数 """
        print('类方法')

    @staticmethod
    def static_func():
        """ 定义静态方法 ，无默认参数"""
        print('静态方法')



f = Foo("中国")
# 调用实例方法
f.ord_func()

# 调用类方法
Foo.class_func()

# 调用静态方法
Foo.static_func()
~~~

##### 对比

- 相同点：对于所有的方法而言，均属于类，所以 在内存中也只保存一份
- 不同点：方法调用者不同、调用方法时自动传入的参数不同。



## 装饰器

装饰器本质上是一个函数或类，它可以让其他函数在不需要修改任何代码的前提下增加额外功能，装饰器的返回值也是一个函数/类对象。可以理解为面向切面编程，比如：插入日志、性能测试、事务处理、缓存、权限校验等场景。有了装饰器，我们就可以抽离出大量与函数功能本身无关的雷同代码到装饰器中并继续重用。



### 闭包

msg是一个局部变量，在print_msg函数执行之后应该就不会存在了。但是嵌套函数引用了这个变量，将这个局部变量封闭在了嵌套函数中，这样就形成了一个闭包。

~~~ python
# print_msg是外围函数
def print_msg():
    msg = "I'm closure"
    
    # printer是嵌套函数
    def printer():
        print(msg)
    return printer

closure = print_msg()
closure()
>>> I'm closure
~~~

**原理：**在函数内部再定义一个函数，并且这个函数用到了外边函数的变量，那么将这个函数以及用到的一些变量称之为闭包



### 装饰器

#### 无参数的装饰器

~~~python
from time import ctime, sleep

def timefun(func):
    def wrapped_func():
        print("%s called at %s" % (func.__name__, ctime()))
        func()
    return wrapped_func

@timefun
def foo():
    print("I am foo")

foo()
>>> foo called at Wed Apr 29 03:57:16 2020
>>> I am foo

# 第一步：执行timefun(func)，将foo先作为参数赋值给func
# 第一步：调用wrapped_func()
# 第三步：执行func(),func里保存的是原foo函数对象
~~~



#### 带参数的装饰器

~~~ python
from time import ctime, sleep

def timefun(func):
    def wrapped_func(a, b):
        print("%s called at %s" % (func.__name__, ctime()))
        print(a, b)
        func(a, b)
    return wrapped_func

@timefun
def foo(a, b):   <--这里带参数
    print(a+b)

foo(3, 5)
>>> foo called at Wed Apr 29 04:37:53 2020
>>> I am foo

# 第一步：执行timefun(func)，将foo先作为参数赋值给func
# 第一步：调用wrapped_func(a, b)，即等价调用foo(a, b)，wrapped_func会获取foo中的参数
# 第三步：执行func()，func里保存的是原foo函数对象
~~~



#### 带不定长参数的装饰器

~~~python
from time import ctime, sleep

def timefun(func):
    def wrapped_func(*args, **kwargs):
        print("%s called at %s"%(func.__name__, ctime()))
        func(*args, **kwargs)
    return wrapped_func

@timefun
def foo(a, b, c):
    print(a+b+c)

foo(3,5,7)
>>> foo called at Wed Apr 29 04:42:49 2020
>>> 15

# 第一步：执行timefun(func)，将foo先作为参数赋值给func
# 第一步：调用wrapped_func(*args, **kwargs)，即等价调用foo(a, b, c)，wrapped_func会获取foo中的参数
# 第三步：执行func(*args, **kwargs)，func里保存的是原foo函数对象
~~~



### 实例方法、类方法、静态方法的区别

普通实例方法，第一个参数需要是self，它表示一个具体的实例本身；

而对于@classmethod，它的第一个参数不是self，是cls，它表示这个类本身；

如果用了@staticmethod，那么就可以无视这个self，而将这个方法当成一个普通的函数使用；



### @classmethod（类方法）

**当我们需要和类直接进行交互，而不需要和实例进行交互时。类方法与实例方法类似，但是传递的不是类的实例，而是类本身，第一个参数是cls。我们可以用类的实例调用类方法，也可以直接用类名来调用。**

~~~python
class A:
    class_attr = "attr"
    def __init__(self):
        pass        
    
    @classmethod
    def class_foo(cls):
        print("log is enabled")

# 类方法可以通过（类名.方法）来调用：
A.class_foo()
~~~



### @staticmethod（静态方法）

**方法中既不使用类对象,也不使用实例对象节省性能 **  **，静态方法类似普通方法，参数里面不用self。这些方法和类相关，但是又不需要类和实例中的任何信息、属性等等。如果把这些方法写到类外面，这样就把和类相关的代码分散到类外，使得之后对于代码的理解和维护都是巨大的障碍。而静态方法就是用来解决这一类问题的。**



比如我们检查是否开启了日志功能，这个和类相关，但是跟类的属性和实例都没有关系。

~~~ python
class A:
    class_attr = "attr"
    def __init__(self):
        pass     
    
    @staticmethod
    def static_foo():
        if log_enabled:
            print("log is enabled")

A.static_foo()
~~~



### @property（将方法变成属性调用）

**作用：**我们可以使用@property装饰器来创建只读属性，@property装饰器会将方法转换为相同名称的只读属性，可以与所定义的属性配合使用，这样可以防止属性被修改。

~~~ python
class DataSet(object):
    def __init__(self):
        self._images = 1
        self._labels = 2 #定义属性的名称
        
    @property
    def images(self): #方法加入@property后，这个方法相当于一个属性，这个属性可以让用户进行使用，而且用户有没办法随意修改。
        return self._images 
    
    @property
    def labels(self):
        return self._labels
l = DataSet()

# 加了@property后，可以用调用属性的形式来调用方法,后面不需要加()。
print(l.images)
print(l.labels)
~~~

**原理：**用户进行属性调用的时候，直接调用images即可，而不用知道属性名_images，因此用户无法更改属性，从而保护了类的属性。



## 魔法属性

~~~ python
__init__ :      构造函数，在生成对象时调用
__del__ :       析构函数，释放对象时使用
__repr__ :      打印，转换
__setitem__ :   按照索引赋值
__getitem__:    按照索引获取值
__len__:        获得长度
__cmp__:        比较运算
__call__:       调用
__add__:        加运算
__sub__:        减运算
__mul__:        乘运算
__div__:        除运算
__mod__:        求余运算
__pow__:        幂
~~~



#### 1.__ doc __

- 表示类的描述信息，Python自建，无需自定义。

~~~ python
class Foo:
    """ 描述类信息，这是用于看片的神奇 """
    def func(self):
        pass
print(Foo.__doc__)
#输出：类的描述信息
~~~



####  2.__ module __ 和 __ class __

+ __ module __ 表示当前操作对象在那个模块
+ __ class __表示当前操作对象属于哪个类

<code>test.py</code>

~~~ python
class Person(object):
    def __init__(self):
        self.name = 'laowang'
~~~

<code>main.py</code>

~~~ python
from test import Person
obj = Person()
print(obj.__module__)  # 输出 test 即：输出模块
print(obj.__class__)  # 输出 test.Person 即：输出类
~~~



#### 3.__ init __ 和 __ new __

- **new**是在实例创建之前被调用的，因为它的任务就是创建实例然后返回该实例对象
- **i****nit**是初始化方法，通过类创建对象时，自动触发执行
- 只有在**new**返回一个cls的实例时后面的**init**才能被调用
- 当创建一个新实例时调用**new**，初始化一个实例时用**init**

~~~ python
class Cat:
    def __new__(cls, name):
        print("创建对象")
        return object.__new__(cls)

    def __init__(self, name):
        print("对象初始化")
        self.name = name    # 自动执行类中的 __init__ 方法

    def __str__(self):
        return "%s" % self.name


lanmao = Cat("蓝猫")
lanmao.age = 20
print(lanmao)

创建对象
对象初始化
蓝猫
~~~



#### 4.__ del __

- 当程序执行完时，自动触发执行。

注：此方法一般无须定义，因为Python是一门高级语言，程序员在使用时无需关心内存的分配和释放，因为此工作都是交给Python解释器来执行，所以，__del__的调用是由解释器在进行垃圾回收时自动触发执行的。

~~~ python 
class Foo:
    def __del__(self):
        print("我被回收了！")
~~~



#### 5.__ call __

- 对象后面加括号，触发执行。

注：__init__方法的执行是由创建对象触发的，即：`对象 = 类名()` ；而对于 __call__ 方法的执行是由对象后加括号触发的，即：`对象()` 或者 `类()()`

~~~ python  
class Foo:
    def __call__(self, *args, **kwargs):
        print('__call__')
obj = Foo()
obj()  # 执行 __call__
~~~



#### 6.__ dict __

- 类或对象中的所有属性

列出类或对象中的所有成员！非常重要和有用的一个属性，Python自建，无需用户自己定义。

~~~ python
class Province(object):
    country = 'China'
    def __init__(self, name, count):
        self.name = name
        self.count = count
        
    def func(self, *args, **kwargs):
        print('func')
        
# 获取类的属性，即：类属性、方法、
print(Province.__dict__)
# 输出：{'__dict__': <attribute '__dict__' of 'Province' objects>, '__module__': '__main__', 'country': 'China', '__doc__': None, '__weakref__': <attribute '__weakref__' of 'Province' objects>, 'func': <function Province.func at 0x101897950>, '__init__': <function Province.__init__ at 0x1018978c8>}

# 获取 对象obj1 的属性
obj1 = Province('山东', 10000)
print(obj1.__dict__)
# 输出：{'count': 10000, 'name': '山东'}

# 获取 对象obj1 的属性
obj2 = Province('山西', 20000)
print(obj2.__dict__)
# 输出：{'count': 20000, 'name': '山西'}
~~~



#### 7.__ str __

+ 如果一个类中定义了__ str __ 方法，则打印对象时默认输出该方法返回值

~~~ python
class Foo:
    def __str__(self):
        return 'laowang'
    
obj = Foo()
print(obj)
# 输出：laowang
~~~



#### 8.__ getitem __ 、 __ setitem __  、 __ delitem __ 

- 用于索引操作，如字典。以上分别表示获取、设置、删除数据

取值、赋值、删除这“三剑客”的套路，在Python中，我们已经见过很多次了，比如前面的@property装饰器。

Python中，标识符后面加圆括号，通常代表执行或调用方法的意思。而在标识符后面加中括号[]，通常代表取值的意思。Python设计了`__getitem__()`、`__setitem__()`、`__delitem__()`这三个特殊成员，用于执行与中括号有关的动作。它们分别表示取值、赋值、删除数据。

~~~ python
# -*- coding:utf-8 -*-
class Foo(object):
    def __getitem__(self, key):
        print('__getitem__', key)
    def __setitem__(self, key, value):
        print('__setitem__', key, value)
    def __delitem__(self, key):
        print('__delitem__', key)
        
obj = Foo()
result = obj['k1']      # 自动触发执行 __getitem__
obj['k2'] = 'laowang'   # 自动触发执行 __setitem__
del obj['k1']           # 自动触发执行 __delitem__
~~~



#### 9.__ getslice __ 、 __ setslice __ 、 __delslice __

- 该三个方法用于分片操作，如：列表

~~~ python
# -*- coding:utf-8 -*-
class Foo(object):
    def __getslice__(self, i, j):
        print('__getslice__', i, j)
    def __setslice__(self, i, j, sequence):
        print('__setslice__', i, j)
    def __delslice__(self, i, j):
        print('__delslice__', i, j)
obj = Foo()
obj[-1:1]                   # 自动触发执行 __getslice__
obj[0:1] = [11,22,33,44]    # 自动触发执行 __setslice__
del obj[0:2]                # 自动触发执行 __delslice__
~~~



## 高内聚低耦合

内聚：每个模块尽可能独立完成自己的功能，不依赖于模块外部的代码。 

耦合：模块与模块之间接口的复杂程度，模块之间联系越复杂耦合度越高，牵一发而动全身。 

目的：使得模块的“可重用性”、“移植性”大大增强 