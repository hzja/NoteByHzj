Python metaclass<br />对于 metaclass 这种特性，有两种极端的观点：

- 这种特性太牛逼了，是无所不能的阿拉丁神灯，必须找机会用上才能显示自己的 Python 实力。
- 这个特性太危险，会蛊惑人心去滥用，一旦打开就会释放恶魔，让代码难以维护。

来看看，metaclass 到底是阿拉丁神灯，还是潘多拉魔盒。
<a name="MUoWP"></a>
## 什么是 metaclass
很多书都会翻译成 元类，仅从字面理解， meta 的确是元，本源，翻译没毛病。但理解时，应该把元理解为描述数据的超越数据，事实上，metaclass 的 meta 起源于希腊词汇 meta，包含两种意思：

- “Beyond”，例如技术词汇 metadata，意思是描述数据的超越数据。
- “Change”，例如技术词汇 metamorphosis，意思是改变的形态。

因此可以理解为 metaclass 为描述类的超类，同时可以改变子类的形态。可能会问了，这和元数据的定义差不多么，这种特性在编程中有什么用？<br />用处非常大。在没有 metaclass 的情况下，子类继承父类，父类是无法对子类执行操作的，但有了 metaclass，就可以对子类进行操作，就像装饰器那样可以动态定制和修改被装饰的类，metaclass 可以动态的定制或修改继承它的子类。
<a name="lpfjS"></a>
## metaclass 能解决什么问题？
已经知道了 metaclass 可以像装饰器那样定制和修改继承它的子类，这里就说下它能解决什么实际问题。比方说，在一个智能语音助手的大型项目中，有 1 万个语音对话场景，每一个场景都是不同团队开发的。作为智能语音助手的核心团队成员，不可能去了解每个子场景的实现细节。<br />在动态配置实验不同场景时，经常是今天要实验场景 A 和 B 的配置，明天实验 B 和 C 的配置，光配置文件就有几万行量级，工作量不可谓不小。而应用这样的动态配置理念，就可以让引擎根据文本配置文件，动态加载所需要的 Python 类。<br />如果还不是很清楚，那么 YAML 应该知道，它是一个家喻户晓的 Python 工具，可以方便地序列化和反序列化数据，YAMLObject 可以让它的任意子类支持序列化和反序列化（serialization & deserialization）。序列化和反序列化应该清楚吧：

- 序列化：当程序运行时，所有的变量或者对象都是存储到内存中的，一旦程序调用完成，这些变量或者对象所占有的内存都会被回收。而为了实现变量和对象持久化的存储到磁盘中或在网络上进行传输，需要将变量或者对象转化为二进制流的方式。而将其转化为二进制流的过程就是序列化。
- 反序列化：而反序列化就是说程序运行的时候不能从磁盘中进行读取，需要将序列化的对象或者变量从磁盘中转移到内存中，同时也会将二进制流转换为原来的数据格式。把这一过程叫做反序列化。

现在有 1 万个不同格式的 YAML 配置文件，本来需要写 1 万个类来加载这些配置文件，有了 metaclass，只需要实现一个 metaclass 超类，然后再实现一个子类继承这个 metaclass，就可以根据不同的配置文件自动拉取不同的类，这极大地提高了效率。
<a name="FgUeZ"></a>
## 通过一个实例来理解 metaclass
请手动在 ipython 中写代码，看看每一步都输出了什么，这样可以彻底的理解类的创建和实例化步骤。
```python
In[15]: class Mymeta(type):
   ...:     def __init__(self, name, bases, dic):
   ...:         super().__init__(name, bases, dic)
   ...:         print('===>Mymeta.__init__')
   ...:         print(self.__name__)
   ...:         print(dic)
   ...:         print(self.yaml_tag)
   ...: 
   ...:     def __new__(cls, *args, **kwargs):
   ...:         print('===>Mymeta.__new__')
   ...:         print(cls.__name__)
   ...:         return type.__new__(cls, *args, **kwargs)
   ...: 
   ...:     def __call__(cls, *args, **kwargs):
   ...:         print('===>Mymeta.__call__')
   ...:         obj = cls.__new__(cls)
   ...:         cls.__init__(cls, *args, **kwargs)
   ...:         return obj
   ...: 
In[16]: 
In[16]: 
In[16]: class Foo(metaclass=Mymeta):
   ...:     yaml_tag = '!Foo'
   ...: 
   ...:     def __init__(self, name):
   ...:         print('Foo.__init__')
   ...:         self.name = name
   ...: 
   ...:     def __new__(cls, *args, **kwargs):
   ...:         print('Foo.__new__')
   ...:         return object.__new__(cls)
   ...:     
===>Mymeta.__new__
Mymeta
===>Mymeta.__init__
Foo
{'__module__': '__main__', '__qualname__': 'Foo', 'yaml_tag': '!Foo', '__init__': <function Foo.__init__ at 0x0000000007EF3828>, '__new__': <function Foo.__new__ at 0x0000000007EF3558>}
!Foo

In[17]: foo = Foo('foo')
===>Mymeta.__call__
Foo.__new__
Foo.__init__

In[18]:
```
从上面的运行结果可以发现在定义 `class Foo()` 定义时，会依次调用 MyMeta 的 `__new__` 和 `__init__` 方法构建 Foo 类，然后在调用 `foo = Foo()` 创建类的实例对象时，才会调用 MyMeta 的 `__call__` 方法来调用 Foo 类的 `__new__` 和 `__init__` 方法。<br />把上面的例子运行完之后就会明白很多了，正常情况下在父类中是不能对子类的属性进行操作，但是元类可以。换种方式理解：元类、装饰器、类装饰器都可以归为元编程。
<a name="F5gtL"></a>
## Python 底层语言设计层面是如何实现 metaclass 的？
要理解 metaclass 的底层原理，需要深入理解 Python 类型模型。下面，将分三点来说明。
<a name="beZro"></a>
### 第一，所有的 Python 的用户定义类，都是 `type` 这个类的实例。
事实上，类本身不过是一个名为 `type` 类的实例。在 Python 的类型世界里，`type` 这个类就是造物的上帝。这可以在代码中验证：
```python
In [2]: # Python 3和Python 2类似
   ...: class MyClass:
   ...:   pass
   ...:
   ...: instance = MyClass()
   ...:
in [3]: type(instance)
   ...:
Out[2]: __main__.MyClass
In [4]: type(MyClass)
   ...:
Out[4]: type
In [5]:
```
可以看到，instance 是 MyClass 的实例，而 MyClass 不过是“上帝” type 的实例。
<a name="NwKAq"></a>
### 第二，用户自定义类，只不过是 type 类的 `__call__` 运算符重载
当定义一个类的语句结束时，真正发生的情况，是 Python 调用 type 的 `__call__` 运算符。简单来说，当定义一个类时，写成下面这样时：
```python
class MyClass:
    data = 1
```
Python 真正执行的是下面这段代码：
```python
class = type(classname, superclasses, attributedict)
```
这里等号右边的 `type(classname, superclasses, attributedict)`，就是 type 的 `__call__` 运算符重载，它会进一步调用：
```python
type.__new__(typeclass, classname, superclasses, attributedict)
type.__init__(class, classname, superclasses, attributedict)
```
当然，这一切都可以通过代码验证，比如
```python
In [5]: class MyClass:
   ...:     data = 1
   ...:
   ...: instance = MyClass()
   ...:

In [6]: MyClass, instance
   ...:
Out[6]: (__main__.MyClass, <__main__.MyClass at 0x4ef5188>)

In [7]: instance.data
   ...:
Out[7]: 1

In [8]: MyClass = type('MyClass', (), {'data': 1})
   ...: instance = MyClass()
   ...:

In [9]: MyClass, instance
   ...:
Out[9]: (__main__.MyClass, <__main__.MyClass at 0x4f40748>)

In [10]: instance.data
    ...:
Out[10]: 1

In [11]:
```
由此可见，正常的 MyClass 定义，和手工去调用 `type` 运算符的结果是完全一样的。
<a name="N2mQ2"></a>
### 第三，metaclass 是 `type` 的子类，通过替换 `type` 的 `__call__` 运算符重载机制，“超越变形”正常的类
其实，理解了以上几点，也就明白了，正是 Python 的类创建机制，给了 metaclass 大展身手的机会。<br />一旦把一个类型 MyClass 的 metaclass 设置成 MyMeta，MyClass 就不再由原生的 type 创建，而是会调用 MyMeta 的 `__call__` 运算符重载。
```python
class = type(classname, superclasses, attributedict) 
# 变为了
class = MyMeta(classname, superclasses, attributedict)
```
<a name="UFS08"></a>
## 使用 metaclass 的风险
不过，凡事有利必有弊，尤其是 metaclass 这样“逆天”的存在。正如看到的那样，metaclass 会"扭曲变形"正常的 Python 类型模型。所以，如果使用不慎，对于整个代码库造成的风险是不可估量的。<br />换句话说，metaclass 仅仅是给小部分 Python 开发者，在开发框架层面的 Python 库时使用的。而在应用层，metaclass 往往不是很好的选择。
