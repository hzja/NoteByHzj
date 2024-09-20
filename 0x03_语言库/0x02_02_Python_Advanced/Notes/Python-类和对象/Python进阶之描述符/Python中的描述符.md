Python 描述符
<a name="mWorW"></a>
## 描述符概念
要了解描述符，理解这两个关键字即可。类，托管。<br />描述符本质上就是一个类，可以将其他类的属性托管给这个类（描述符）。
<a name="Z6lYB"></a>
## 小试牛刀
<a name="jHwYf"></a>
### 类
描述符本质上是个类，定义一个普通的类肯定不是描述符，只要有`__get__`，`__set__`，`__delete__`魔术方法之一的类就是描述符。
<a name="wzIwf"></a>
### 托管
来举一个简单的例子，来看看怎么把类属性托管给描述符。
```python
class A:

    x = 1

    
a = A()
print(a.x)
```
```python
1
```
在类中，可以访问类属性，那么定义一个描述符看看。
```python
class Foo:

    def __get__(self, instance, owner):
        return 10


class A:

    x = Foo()


a = A()
print(a.x)
```
```python
10
```
定义一个描述符，其中有`__get__`魔术方法；在A类中，将描述符赋值给类的`x`属性，这样`x`属性就被描述符托管了，当访问`x`属性时，就会触发描述符的get方法。
<a name="YwmL0"></a>
## 描述符方法
<a name="ngsJq"></a>
### `__get__`魔术方法
上面的内容了解了数据描述符，那现在再仔细的学习下描述符的各种方法和使用，首先是`__get__`方法。

- 触发时间：在访问对象成员属性的时候触发。
- 作用：设置属性的值。
- 参数：`self`为描述符对象，`instance`为被托管的类的对象，`owner`为被托管的类。
- 返回值：属性值。

再来举一个例子，例如想把用户的名称转换为ID，而且ID只显示姓，其余为*代替。
```python
class Foo:

    def __get__(self, instance, owner):
        return instance._name[0] + (len(instance._name) - 1) * "*"


class User:

    ID = Foo()

    def __init__(self, name):
        self._name = name


user = User('李四')
print(user.ID)
user2 = User('刘麻子')
print(user2.ID)
```
```python
李*
刘**
```
<a name="LniVe"></a>
### `__set__`魔术方法

- 触发时间：在设置对象成员属性的时候触发。
- 作用：对成员的值进行设置管理。
- 参数：`self`为描述符对象，`instance`为被托管的类的对象，`value`为设置的值。
- 返回值：无。

接着上面的案例，设置一个`age`属性，并通过描述符来管理，当`age`大于等于0时，返回age值，如果小于0，就报错。
```python
class Foo:

    def __get__(self, instance, owner):
        return instance._name[0] + (len(instance._name) - 1) * "*"


class Age:

    def __init__(self, age=0):
        self.age = age

    def __get__(self, instance, owner):
        return self.age

    def __set__(self, instance, value):
        if value >= 0:
            self.age = value
        else:
            raise ValueError("年龄必须大于等于0")


class User:

    ID = Foo()
    age = Age()

    def __init__(self, name):
        self._name = name


user = User('李四')
user.age = 24
print(user.age)
user.age = -1
```
```python
24
ValueError: 年龄必须大于等于0
```
<a name="pQSHH"></a>
### `__delete__`魔术方法

- 触发时间：在删除对象成员属性的时候触发。
- 作用：对成员的值进行删除管理。
- 参数：`self`为描述符对象，`instance`为被托管的类的对象。
- 返回值：无。

例如手动删除`age`属性，但是在`__delete__`中不做操作，`age`属性还是能访问的到。
```python
class Age:

    def __init__(self, age=0):
        self.age = age

    def __get__(self, instance, owner):
        return self.age

    def __set__(self, instance, value):
        if value >= 0:
            self.age = value
        else:
            raise ValueError("年龄必须大于等于0")

    def __delete__(self, instance):
        pass


class User:

    ID = Foo()
    age = Age()

    def __init__(self, name):
        self._name = name


user = User('李四')
user.age = 24
print(user.age)
del user.age
print(user.age)
```
```python
24
24
```
除了定义类（`__get__`，`__set__`，`__delete__`）这种描述符方式外，还有其他两种设置描述符的方式。
<a name="BIEW0"></a>
### `property`函数
这种写法使用的并不是很多，定义好获取、设置和删除属性的方法，把他们传入到`property`函数中即可。
```python
class User:

    def __init__(self, _age=0):
        self._age = _age

    def getage(self):
        return self._age

    def setage(self, value):
        if value >= 0:
            self._age = value
        else:
            raise ValueError("年龄必须大于等于0")

    age = property(getage, setage)


user = User()
print(user.age)
user.age = -1
```
```python
0
ValueError: 年龄必须大于等于0
```
<a name="XhYCY"></a>
### `property`装饰器
最后一种方法是使用`property`装饰器，装饰器下的`age`函数，其实就是管理`age`属性的，然后通过`setter`和`deleter`分别设置和删除属性。
```python
class User:

    def __init__(self):
        self._age = 0

    @property
    def age(self):
        return self._age

    @age.setter
    def age(self, value):
        if value >= 0:
            self._age = value
        else:
            raise ValueError("年龄必须大于等于0")

    @age.deleter
    def age(self):
        pass


user = User()
print(user.age)
user.age = -1
```
```python
0
ValueError: 年龄必须大于等于0
```
