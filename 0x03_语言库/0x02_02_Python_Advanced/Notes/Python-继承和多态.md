Python 继承 多态
<a name="2dde3029"></a>
## 继承
<a name="lzqXR"></a>
### 什么是继承？

- 继承是一种创建新的类的方式，新创建的叫子类，继承的叫父类、超类、基类。<br />
- 特点：子类可以使用父类的属性（特征、技能）<br />
- 继承是类与类之间的关系<br />
<a name="kyJwy"></a>
### 为什么要继承？

- 减少代码冗余、提高重用性<br />
<a name="P3nKa"></a>
### 如何使用继承？
<a name="jaI0Y"></a>
#### 继承分为单继承和多继承
先看**单继承**
```python
class A(object):
   def pp(self):
       print('pp A')
class B(A):
   def pp(self):
       super().pp()
       print("pp B")
b = B()
b.pp()
#pp A
#pp B
```

- super（）常用的方法是在__ init __()方法中确保父类被正确的初始化了：<br />
- super(cls,inst).__ init__()   #cls,init 可以省略
```python
class A(object):
   def __init__(self):
       self.x = 1
class B(A):
   def __init__(self):
       super(B,self).__init__()
       self.x = self.x +1
       print(self.x)
b = B()
#2
```
也可以直接调用父类的一个方法 ：
```python
A.__ init__(self)
class A(object):
   def __init__(self):
       self.x = 1
class B(A):
   def __init__(self):
       A.__init__(self)
       self.x = self.x +1
       print(self.x)
b = B()
#2
```
多继承例子：
```python
class A(object):
   def __init__(self):
       print("Enter A")
class B(A):
   def __init__(self):
       print('Enter B')
       super(B,self).__init__()
       print('Leave B')
class C(A):
   def __init__(self):
       print('Enter C')
       super(C, self).__init__()
       print('Leave C')
class D(B,C):
   def __init__(self):
       print('Enter D')
       super(D,self).__init__()
       print("Leave D")
d = D()
'''
Enter D
Enter B
Enter C
Enter A
Leave C
Leave B
Leave D
'''
```
使用__ bases__方法可以获取子类继承的类
```python
class Parent2():
   print('我是第二个爹')
class Parent():
   print('我是第一个爹')
   
class SubClass(Parent, Parent2):
   print('我是子类')
print(SubClass.__bases__)
#注意，如果sub = SubClass(),sub是没有__bases__方法的
'''
我是第二个爹
我是第一个爹
我是子类
(<class '__main__.Parent'>, <class '__main__.Parent2'>)
'''
```
<a name="QuqSt"></a>
### super()的用法
<a name="dAGrP"></a>
#### super()的本质
Python中实现继承，对于定义的每一个类，Python会计算出一个所谓的方法解析顺序(MRO)列表。这个MRO列表就是一个简单的所有基类的线性顺序表。为了实现继承，Python会在MRO列表上从左到右开始查找基类，直到找到第一个匹配这个属性的类为止。<br />而这个MRO列表的构造是通过一个C3线性化算法来实现的。它实际上就是合并所有父类的MRO列表并遵循如下三条准则：

- 子类会先于父类被检查<br />
- 多个父类会根据它们在列表中的顺序被检查<br />
- 如果对下一个类存在两个合法的选择，选择第一个父类<br />

虽然名义上来说super是用来调用父类中的方法，但是super实际上是在MRO表中找到下一个匹配的类。super原型如下：
```python
def super(cls, inst):
  mro = inst.__class__.mro()
  return mro[mro.index(cls) + 1]
```
两个参数 cls 和 inst 分别做了两件事：

1. inst 负责生成 MRO 的 list<br />
2. 通过 cls 定位当前 MRO 中的 index, 并返回 mro[index + 1]<br />

关于super的概念大家不用深究，只要掌握其语法会用了就行。
<a name="kkxxl"></a>
## 多态

- 一个操作的意义取决于被操作对象类型，相同的消息给与不同的对象会引发不同的动作。<br />
- 多态意味着变量并不知道引用的对象是什么，根据引用对象的不同，表现不同的行为方式。<br />
- 在处理多态对象时，只需要关注他的接口即可。<br />
- 同一个操作作用于不同的对象，可以有不同的解释，产生不同的执行结果。<br />
:::danger
注意：多态和多态性是两种不同的概念
:::
<a name="znUPR"></a>
### 多态的概念
传统意义上的多态指一类事物有多种形态，比如动物类，可以有猫，狗，猪等等。（一个抽象类有多个子类，因而多态的概念依赖于继承）<br />用代码表示如下：
```python
import abc
class Animal(metaclass=abc.ABCMeta):  #同一类事物：动物
   @abc.abstractmethod
   def talk(self):
       pass
class Cat(Animal):  #动物的形态之一：猫
   def talk(self):
       print('say miaomiao')
       
class Dog(Animal):   #动物的形态之二：狗
   def talk(self):
       print('say wangwang')
       
class Pig(Animal):
   def talk(self):
       print('say aoao')
```
<a name="YxSXb"></a>
### 多态性
多态性是指具有不同功能的函数可以使用相同的函数名，这样就可以用一个函数名调用不同内容的函数。在面向对象方法中一般是这样表述多态性：向不同的对象发送同一条消息，不同的对象在接收时会产生不同的行为（即方法）。也就是说，每个对象可以用自己的方式去响应共同的消息。所谓消息，就是调用函数，不同的行为就是指不同的实现，即执行不同的函数。
```python
import abc
class Animal(metaclass=abc.ABCMeta):  #同一类事物：动物
   @abc.abstractmethod
   def talk(self):
       pass
class Cat(Animal):  #动物的形态之一：猫
   def talk(self):
       print('say miaomiao')
       
class Dog(Animal):   #动物的形态之二：狗
   def talk(self):
       print('say wangwang')
       
class Pig(Animal):
   def talk(self):
       print('say aoao')
       
c=Cat()
d=Dog()
p=Pig()
def func(obj):
   obj.talk()
   
func(c)
func(d)
func(p)
#say miaomiao
#say wangwang
#say aoao
```
综上可以说，**多态性是 : 一个接口,多种实现**
<a name="QsY7K"></a>
### 多态性的好处

- 增加了程序的灵活性,以不变应万变，不论对象千变万化，使用者都是同一种形式去调用，如func(obj)<br />
- 增加了程序额可扩展性,通过继承animal类创建了一个新的类，使用者无需更改自己的代码，还是用func(obj)去调用<br />
