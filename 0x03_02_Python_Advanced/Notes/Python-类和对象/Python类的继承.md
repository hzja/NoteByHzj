Python 继承
<a name="aQAR9"></a>
## 继承
面向对象三大特征：封装、继承、多态<br />面向对象编程 (OOP) 语言的一个主要功能就是“继承”，所谓继承就是使现有的类无需编码便可以拥有原有类的方法和属性。<br />被继承的类可以称之为父类、基类、超类。继承的类可以称之为子类、派生类。派生和继承是一体两面，从父类向子类看就是派生，从子类向父类看就是继承。子类和父类的关系可以用“is a”类表示，即子类是父类的一种，是一个更具体、更加强大的父类。Python支持单继承和多继承。
<a name="wE7HB"></a>
## 单继承
在开始给大家介绍编程之前，首先给需要大家了解，在Python中所有的类默认继承object。也就是说如果定义一个Person类，则默认会继承object。<br />继承的格式：
```python
class 父类名: # 默认继承object，但是都是省略了object
......
class 子类名(父类名):
....
```
```python
# 没有继承的时候
class Person:
    def __init__(self, name):
        self.name = name
        self.age = 18

    def eat(self):
        print(self.name + "正在吃饭...")
        
class Student:
    def __init__(self, name):
        self.name = name
        self.age = 18

    def eat(self):
        print(self.name + "正在吃饭...")
        
    def study(self):
      print('我要好好学习！')

class Programer:
    def __init__(self, name):
        self.name = name
        self.age = 18

    def eat(self):
        print(self.name + "正在吃饭...")
    
    def program(self):
      print('编写程序中...')
```
但是发现三个类中有很多相同的代码，这时候就成了代码冗余。此时就需要通过继承解决问题。<br />将每个类中冗余的代码提取到父类中，然后子类继承父类的就可以了。<br />按照上面继承的格式“改装”代码如下：
```python
class Person:
    def __init__(self, name):
        self.name = name
        self.age = 18

    def eat(self):
        print(self.name + "正在吃饭...")
        
class Student(Person):        
    def study(self):
      print('我要好好学习！')

class Programer:
    def program(self):
      print('编写程序中...')
```
此时代码是不是就变得很简练了，可以创建一个Student对象或者Programer对象，调用一下eat方法看看是否有打印。
<a name="uYgCS"></a>
## 构造方法的继承
在上面的代码基础上，创建一个Student对象。
```python
s = Student('大宝')
s.eat()
s.study()
```
但是此时想在学生对象创建的时候就初始化一个学生的班级，如何实现呢？<br />父类`__init__`调用方式：

1. `super(当前类名,self)._init_(实参列表)`
2. `super()._init_(实参列表)`
3. `父类名._init_(self,其它参数)`
```python
class Person:
    def __init__(self, name):
        self.name = name
        self.age = 18

    def eat(self):
        print(self.name + "正在吃饭...")

class Student(Person):  
    def __init__(self,name,clazz):
      # 调用父类的构造方法(3种实现方式)
        # super(Student,self).__init__(name)
        # super().__init__(name)
        Person.__init__(self,name)
        self.clazz = clazz
                
    def study(self):
        print(f'我在{self.clazz},我要好好学习！')


s = Student('大宝','一年级3班')
print(s)
s.eat()
s.study()
print(s.age)
```
<a name="G9v8D"></a>
## 方法的重写
有的时候从父类继承的方法在子类往往不能满足需求，则需要在自己的类中定义一个同名的方法，那这种操作称作重写。<br />比如父类Person中的`eat`方法不能满足子类Student的需求了，此时就需要在Student中重写此方法，代码如下：
```python
class Student(Person):  
    def __init__(self,name,clazz):
        super().__init__(name)
        self.clazz = clazz
                
    def study(self):
        print(f'我在{self.clazz},我要好好学习！')
        
    # 重写eat方法
    def eat(self,food):
     # 此时可以调用父类原有的方法通过关键字super，然后再添加自己的代码
     super().eat()
     print(f'{self.name}最喜欢的食物是:{food}')

# 创建对象
s = Student('大宝','一年级3班')
s.eat()
```
结果：
```python
大宝正在吃饭...
大宝最喜欢的食物是:汉堡
```
可以发现打印结果是重写后的eat方法，所以记住一点：对象在调用的时候先判断当前类是否存在此方法，如果存在调用自己的，不存在去父类找，如果父类都没有则报错。<br />继承注意事项：

1. 并不是所有的都可以继承！私有的是继承不了的。即父类的私有属性和私有方法是无法继承的。
2. Python中的**`super()`**方法设计目的是用来解决多继承时父类的查找问题，所以在单继承中用不用 super 都没关系；但是，使用 `super()` 是一个好的习惯。一般我们在子类中需要调用父类的方法时才会这么用。
3. `super()`的好处就是可以避免直接使用父类的名字.主要用于多重继承
```python
class A:
    def m(self):
        print('A')


class B:
    def m(self):
        print('B')


class C(A):
    def m(self):
        print('C')
        super().m()


c = C()
c.m()
```
这样做的好处就是：如果要改变子类继承的父类（由A改为B），只需要修改一行代码（class C(A): -> class C(B)）即可，而不需要在class C的大量代码中去查找、修改基类名，另外一方面代码的可移植性和重用性也更高。
<a name="HHp2s"></a>
## 多继承
所谓多继承就是一个子类可以继承多个父类。格式：
```python
class 父类A:
......
class 父类B:
......
class 子类名(父类A,父类B,..): # 即可以通过逗号分隔跟多个父类
....
```
```python
class A:
    def m(self):
        print('A')
 
class B:
    def m(self):
        print('B')
  
class C:
    def print_c(self):
        print('CCC')
 
class D(A,B,C):
    def m(self):
        print('D')
        super().m()
 
d = D()
d.m()
d.print_c()
```
结果：
```python
D
A
CCC
```
当对象d调用`m()`函数的时候，为什么？因为首先在当前类搜索是否存在m函数，如果存在则打印结果，不存在则去父类找。那`super().m()`调用的时候搜索父类的顺序是什么呢？是按照继承时括号里面父类的顺序依次查找是否存在，所以先判断A类是否有m函数，有则调用，没有继续向下查找。<br />当然也可以通过调用：`类名.__mro__`来查看查找m的顺序。
```python
print(D.__mro__)
```
结果：
```python
(<class 'main.D'>, <class 'main.A'>, <class 'main.B'>, <class 'main.C'>, <class 'object'>)
```
但是如果把代码改成如下：
```python
class A:
    def m(self):
        print('A')
 
class B:
    def m(self):
        print('B')
  
class C(A,B):
    def print_c(self):
        print('CCC')

class D:
    def m(self):
        print('D')    
 
class E(C,D):
    def m(self):
        print(E)
        super().m()

print(E.__mro__)
```
