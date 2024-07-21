Python
<a name="tGW9l"></a>
### 魔法方法总结
字符串/字节序列表示相关：`__repr__`、`__str__`，`__format__`，`__bytes__`。这些主要是做一些格式化的操作。<br />数值转换：`__abs__`、`__bool__`、`__complex__`、`__int__`、`__float__`、`__hash__`、`__index__`。<br />集合模拟：`__len__`、`__getitem__`、`__setitem__`、`__delitem__`、`__contains__`。用的列表就包括了这些方法。<br />迭代枚举：`__iter__`、`__reversed__`、`__next__`。主要用于迭代器和生成器。<br />可调用模拟：`__call__`。主要是加上括号就能触发调用。<br />上下文管理：`__enter__`、`__exit__`。大家还记得with读取文件吗，其实就是上下文管理器。<br />实例创建和销毁：`__new__`、`__init__`、`__del__`。这些都是创建类中常常使用的。<br />属性相关：`__getattribute__`、`__getattr__`、`__setattr__`、`__delattr__`、`__setattr__`、`__dir__`。这五个方法主要是用于操作属性的。<br />属性描述符：`__get__`、`__set__`、`__del__`。用来定义属性描述符。
<a name="6QeF8"></a>
### 方法重写
如果父类方法的功能不能满足需求，可以在子类重写父类的方法，实例如下：
```python
class Parent:   #定义父类
   def myMethod(self):
       print('调用父类方法')
       
class Child(Parent):   #定义子类
   def myMethod(self):
       print('调用子类方法')
       
c=Child()     #子类实例
c.myMethod() #子类调用重写方法
super(Child,c).myMethod()  #用子类对象调用父类已被覆盖的方法
#super()是用于调用父类（超类）的一个方法。
#调用子类方法
#调用父类方法
```
<a name="SDrmr"></a>
### 魔法方法
在Python中，有一些内置好的特定的方法，这些方法在进行特定的操作时会自动被调用，称之为魔法方法，下面介绍几种常见的魔法方法。

- `__ new__(cls[,* argv])`
- `__ new__` 是在一个对象实例化的时候所调用的第一个方法，它的第一个参数是这个类，其他的参数是用来直接传递给 `__ init__` 方法
- `__ new__` 决定是否要使用该 `__ init__` 方法，因为 `__ new__` 可以调用其他类的构造方法或者直接返回别的实例对象来作为本类的实例，如果 __ new__ 没有返回实例对象，则 `__ init__` 不会被调用
- `__ new__` 主要是用于继承一个不可变的类型比如一个 tuple 或者 string
- `cls`：代表一个类的名称
- `self`：代表一个实例对象的名称
```python
class myMethod(str):
   def __new__(cls,string):
       string=string.upper()
       return str.__new__(cls,string)
   
X=myMethod('DataScience')
print(X)
#DATASCIENCE
```
<a name="gzcXF"></a>
#### `__ init__`：
初始化函数，在创建实例对象为其赋值时使用，在`__ new__` 之后，`__ init __` 必须至少有一个参数self，就是这个 `__ new__` 返回的实例，`__ init__` 是在 `__ new__` 的基础上可以完成一些其它初始化的动作，`__ init__`不需要返回值。
```python
class Dog:
   def __init__(self,color,brand):
       self.color=color
       self.brand=brand
puppy=Dog('黑色','拉布拉多')
print(puppy)
#<__main__.Dog object at 0x0000028EC0609DC8>
```
下面再来一个结合`__ init__` 和`__ new__`两个魔法方法的例子：
```python
class A(object):
   pass
class B(A):
   def __init__(self):
       print('__init__被调用')
   def __new__(cls):
       print('__new__被调用')
       print(id(cls))
       return object.__new__(A)
       #注意此处采用了参数A而不是cls，__new__没有正确返回当前类cls的实例
b=B()
print(b)
print(type(b))  #类型所继承的基类
print(id(A))
print(id(B))
'''
__new__被调用
2812126085768
<__main__.A object at 0x0000028EC0574108>
<class '__main__.A'>
2812126083880
2812126085768
'''
```
从运行结果可以看出，`__ new__` 中的参数cls和B的id是相同的，表明`__ new__` 中默认的参数cls就是B类本身，而在`return`时，并没有正确返回当前类cls的实例，而是返回了其父类A的实例，因此`__ init__`这一魔法方法并没有被调用，此时`__ new__`虽然是写在B类中的，但其创建并返回的是一个A类的实例对象。<br />现在将`return`中的参数A变为cls，再来看一下运行结果：
```python
class A(object):
   pass
class B(A):
   def __init__(self):
       print('__init__被调用')
   def __new__(cls):
       print('__new__被调用')
       print(id(cls))
       return object.__new__(cls)
       #注意此处采用了参数A而不是cls，__new__没有正确返回当前类cls的实例
b=B()
print(b)
print(type(b))  #类型所继承的基类
print(id(A))
print(id(B))
'''
__new__被调用
2812126087656
__init__被调用
<__main__.B object at 0x0000028EC057C148>
<class '__main__.B'>
2812126086712
2812126087656
'''
```
可以看出，当`__ new__` 正确返回其当前类cls的实例对象时，`__ init__`被调用了，此时创建并返回的是一个B类的实例对象。
<a name="0nakZ"></a>
#### `__ class__`：
获得已知对象的类 ( `对象.__ class__`)。<br />基础格式如下：
```python
class My:
   pass
a=A()
print(a.__class__)
#<class '__main__.A'>
```
`__ class__`在下面这种情况中是有用的：即当一个类中的某个成员变量是所有该类的对象的公共变量时.<br />下面看一个例子：
```python
class My:
   count=0
   def addcount(self):
       self.__class__.count +=1
       
a=My()
a.addcount()
print(a.count)
print('*'*50)
b=My()
b.addcount()
print(b.count)
#1
#**************************************************
#2
```
从运行结果可以看出，虽然a和b是两个不同的My类的实例对象，但采用了`__ class__` 之后，分别调用两个对象的`addcount`方法之后，获取到的对象的`count`属性却是在不断累加的，此时`self.__ class__ .count`不再是单纯的某个对象私有的属性，而是类的所有实例对象的共有属性,它相当于self.A.count。若将`self.__ class__ .count += 1`变为`self.count += 1`，此时`__ class__`的效果就十分明显了。
```python
class My:
   count=0
   def addcount(self):
       self.count +=1
       
a=My()
a.addcount()
print(a.count)
print('*'*50)
b=My()
b.addcount()
print(b.count)
#1
#**************************************************
#1
```
<a name="MMZUA"></a>
#### `__ str__`：
在将对象转换成字符串  `str(对象)`  测试的时候，打印对象的信息，`__ str__` 方法必须要`return`一个字符串类型的返回值，作为对实例对象的字符串描述，`__ str__` 实际上是被`print`函数默认调用的，当要`print`（实例对象）时，默认调用`__ str__` 方法，将其字符串描述返回。如果不是要用`str()`函数转换。当打印一个类的时候，那么`print`首先调用的就是类里面的定义的`__ str__`。
```python
class My():
   def __init__(self,name):
       self.name=name
   def __str__(self):
       return('我是My类的实例对象my，我的名字叫%s'%self.name)
   
my=My('小王')
print(My)
print(my)
#<class '__main__.My'>
#我是My类的实例对象my，我的名字叫小王
```
<a name="ACjmx"></a>
#### 类的专有方法

- `__ init__` : 构造函数，在生成对象时调用
- `__ del__` : 析构函数，释放对象时使用
- `__ repr__` : 打印，转换
- `__ setitem__` : 按照索引赋值
- `__ getitem__`: 按照索引获取值
- `__ len__` : 获得长度
- `__ cmp__` : 比较运算
- `__ call__` : 函数调用
- `__ add__` : 加运算
- `__ sub__` : 减运算
- `__ mul__` : 乘运算
- `__ truediv__` : 除运算
- `__ mod__` : 求余运算
- `__ pow__` : 乘方
<a name="pWkHw"></a>
### 总结

- 修改类方法和属性，会影响实例方法和属性
- 一个实例属性的修改，不会影响另一个实例属性
- 类方法可以修改，但实例方法不允许修改
