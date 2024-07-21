Python 类 对象
<a name="Y391s"></a>
## 类和对象
Python语言是一种面向对象的语言，因此在Python中创建一个类和对象是很容易的。<br />下面简单介绍一下**面向对象中的一些基本概念：**

- 类(class)：类是抽象的概念，它是一种数据结构，就好比一个模型，该模型是生活中具有相同属性（特征）和方法（动作）的同一类事务，可以用它来生产真实的物体（实例），对象是类的实例。
- 对象：对象就是具有类属性和方法的具体事物（就是基于类而创建的一个具体的事物），Python中一切皆对象。对象包括两个数据成员（类变量和实例变量）和方法。对象的属性一般指主要的特征和参量，而方法主要指函数。
- 类与对象的关系：对象是有类产生的。一个类为它的全部对象给出了一个统一的定义，而他的每个对象则是符合这种定义的一个实体，因此类和对象的关系就是抽象和具体的关系
- 实例化：创建一个类的实例，类的具体对象。
- 实例化对象：由类生产的一个具体对象就叫实例化对象，也就是对象。
- 方法：类中定义的函数。
- 类变量：类变量在整个实例化的对象中是公用的。类变量定义在类中且在函数体之外。类变量通常不作为实例变量使用。
- 数据成员：类变量或实例变量用于处理类及其实例对象的相关数据。
- 方法重写：如果从父类继承的方法不能满足子类的需求，可以对其进行改写，这个过程称为方法的覆盖（override重写）。
- 局部变量：定义在方法中的变量，只作用于当前实例的类。
- 实例变量：在类的声明中，属性是用变量来表示的，这种变量就称为实例变量，实例变量就是一个用self修饰的变量。
- 继承：即一个派生类(derived class)继承基类(base class)的字段和方法。继承也允许把一个派生类的对象作为一个基类对象对待。例如，有这样一个设计：一个Dog类型的对象派生自animal类，这是模拟"是一个（is-a）"关系（例图，Dog是一个Animal）。

和其它编程语言相比，Python 在尽可能不增加新的语法和语义的情况下加入了类机制。<br />Python中的类提供了面向对象编程的所有基本功能：类的继承机制允许多个基类，派生类可以覆盖基类中的任何方法，方法中可以调用基类中的同名方法。<br />对象可以包含任意数量和类型的数据。<br />**面向对象编程的特征：**

- 封装：对外部隐藏对象的工作细节
- 继承：子类自动共享父类之间数据和方法的机制
- 多态：可以对不同类的对象调用相同的方法，产生不同的结果
<a name="PXbhG"></a>
### 类定义
类实例化后，可以使用其属性。创建一个类之后，可以通过类名访问其属性。<br />语法格式如下：
```python
class ClassName(): #类的命名一般首字母大写，括号中参数可带可不带,带只能带继承的基类名称
'这里可写类的说明文档'
<statement-1>
.
.
.
<statement-N>
```
<a name="nQ00P"></a>
### 类对象
类对象支持两种操作：属性引用和实例化。<br />**属性引用：**使用和Python中所有的属性引用一样的标准语法：obj.name。<br />类对象创建后，类命名空间中所有的命名都是有效属性名。
```python
class MyClass:
   "演示一个简单的类实例"
   i='Fcant'
   def func(self):
       return 'Hello World'
   
#实例化类
x=MyClass()
#访问类的属性和方法
print('MyClass类的属性i为：',x.i)
print('MyClass类的方法func输出为',x.func())
#MyClass类的属性i为： Fcant
#MyClass类的方法func输出为 Hello World
```
类有一个名为 `__ init__()` 的特殊方法（构造方法），该方法在类实例化时会自动调用。<br />语法格式如下：
```python
def __init__(self):
   self.data=[]
__ init__ () 方法可以有参数，参数通过 __ init__() 传递到类的实例化操作上。例如:
class Special:
   def __init__(self,realpart,imagpart):
       self.r=realpart
       self.i=imagpart
x=Special('welcome',12)
print(x.r,x.i)      
#welcome 12
```
<a name="gnYmc"></a>
#### self
self 代表类的实例，而非类。<br />类的方法与普通的函数有一个区别—它们必须有一个额外的第一个参数名称，按照惯例它的名称是 self。
```python
class Test:
   def test(self):
       print(self)
       print(self.__class__)
       
t=Test()
t.test()
#<__main__.Test object at 0x000002698B8C56C8>
#<class '__main__.Test'>
```
从执行结果可以很明显的看出，self 代表的是类的实例，代表当前对象的地址，而 `self.class` 则指向类。<br />self 不是 Python 关键字，把他换成 datascience 也是可以正常执行的：
```python
class Test:
   def test(datascience):
       print(datascience)
       print(datascience.__class__)
t = Test()
t.test()
#<__main__.Test object at 0x000002698B8CC448>
#<class '__main__.Test'>
```
<a name="6vyEb"></a>
### 类属性与方法

- 类中的函数叫方法
- 类中的变量叫属性
<a name="GlM9f"></a>
#### 类的私有属性
`__private_attrs`：两个下划线开头，声明该属性为私有，不能在类的外部被使用或直接访问。在类内部的方法中使用时`self.private_attrs`。
<a name="GH3UO"></a>
#### 类的方法
在类的内部，使用def关键字来定义一个方法，与一般函数定义不同，类方法必须包含参数self，且为第一个参数，self代表的是类的实例。
> 注意：self这个名字不是定死的，也可以使用this，但最好还是用常用的self。

<a name="inmOf"></a>
#### 类的私有方法
`__ private_method`：两个下划线开头，声明该方法为私有方法，只能在类的内部调用 ，不能在类的外部调用。`self.__private_methods`。
<a name="L8umy"></a>
#### 实例
**私有属性**的实例如下
```python
class PrivateEX:
   __secretCount = 0  # 私有变量
   publicCount = 0    # 公开变量
   
   def count(self):
       self.__secretCount += 1
       self.publicCount += 1
       print (self.__secretCount)
       
counter = PrivateEX()
counter.count()
counter.count()
print (counter.publicCount)
print (counter.__secretCount)  # 报错，实例不能访问私有变量
#1
#2
#2
---------------------------------------------------------------------------
AttributeError                            Traceback (most recent call last)
<ipython-input-1-f0d64b9d6654> in <module>
    12 counter.count()
    13 print (counter.publicCount)
---> 14 print (counter.__secretCount)  # 报错，实例不能访问私有变量
AttributeError: 'PrivateEX' object has no attribute '__secretCount'
```
类的**私有方法**实例如下：
```python
class Private:
   def __init__(self, name, url):
       self.name = name       # public
       self.__url = url   # private
   def who(self):
       print('name : ', self.name)
       print('url : ', self.__url)
       
   def __foo(self):          # 私有方法
       print('这是私有方法')
       
   def foo(self):            # 公共方法
       print('这是公共方法')
       self.__foo()
       
x = Private('Fcant', 'HelloWorld')
x.who()        # 正常输出
x.foo()        # 正常输出
x.__foo()      # 报错 外部不能调用私有方法
'''
name : Fcant
url : HelloWorld
这是公共方法
这是私有方法
'''
---------------------------------------------------------------------------
AttributeError                            Traceback (most recent call last)
<ipython-input-8-0696acdddb15> in <module>
    18 x.who()        # 正常输出
    19 x.foo()        # 正常输出
---> 20 x.__foo()      # 报错 外部不能调用私有方法
AttributeError: 'Private' object has no attribute '__foo'
```
通过类的实例化，可以创建多个实例对象。<br />下面的例子为多个对象的实例化：
```python
class my_class():
   num=100
   def print_num(self):
       print('num is %d'%(self.num))
       
class_1=my_class() #实例化
class_2=my_class() #实例化
class_1.print_num()
class_2.print_num()
#num is 100
#num is 100
class_1.num=10  #修改实例属性
my_class.num=50  #修改类属性
class_1.print_num()
class_2.print_num()
#num is 10
#num is 50
class_1.num2=5  #新增类属性
print(class_1.num2)
#5
```
<a name="iuVbL"></a>
#### 类增加/修改属性
从外部添加/修改属性
```python
class  Example():
   pass
Example.num=100
print(Example.num)
#100
从内部添加/修改属性
class Example():
   @classmethod
   def add_num(cls):
       cls.num=input('请输入：')
       print(cls.num)
       
Example.add_num()
#请输入：2
#2
```
在这里介绍一下上段代码所用到的`@classmethod`<br />一般来说，要使用某个类的方法，需要先实例化一个对象再调用方法。<br />而使用`@staticmethod`或`@classmethod`，就可以不需要实例化，直接类名.方法名()来调用。这有利于组织代码，把某些应该属于某个类的函数给放到那个类里去，同时有利于命名空间的整洁,既然`@staticmethod`和`@classmethod`都可以直接类名.方法名()来调用，那他们有什么区别呢？<br />**现从他们使用区别来看**：

- `@staticmethod`不需要表示自身对象的self和自身类的cls参数，就跟使用函数一样。
- `@classmethod`也不需要self参数，但第一个参数需要是表示自身类的cls参数。
- 如果在`@staticmethod`中要调用到这个类的一些属性方法，只能直接类名.属性名或类名.方法名。
- 而`@classmethod`因为持有cls参数，可以来调用类的属性，类的方法，实例化对象等，避免硬编码。
- 类方法不但能调用类属性，还能直接调用类中的其他方法

下面为一个成绩录入的例子
```python
class List():
   @classmethod
   def enter(cls):
       cls.name=input('请输入学生姓名：')
       cls.grade=int(input('请输入考试成绩：'))
       
   @classmethod
   def count(cls):
       if cls.grade>=60:
           return '及格'
       else:
           return '不及格'
       
   @classmethod
   def result(cls):
       print(cls.count())
       
List.enter()
List.result()
'''
请输入学生姓名：xiaowang
请输入考试成绩：32
不及格
'''
```
