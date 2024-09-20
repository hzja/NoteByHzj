Python
<a name="dxvtp"></a>
## 1、反射算术运算符
可能知道 Python 里面的魔法函数，比如 `__add__` 和 `__sub__` 代表 + - 运算符，表示 obj +/- something，但你可能不知道还有一个 `__radd__`，`__rsub__` 函数，可以表示 something +/- obj。<br />举例如下：
```python
class Dog:
    def __add__(self, other):
        return "from __add__"
    def __radd__(self, other):
        return "from __radd__"
dog = Dog()
print(dog + 1)   # from __add__
print(1 + dog)   # from __radd__
```
<a name="kmHql"></a>
## 2、`__getattr__` vs `__getattribute__`
`__getattr__` 魔术方法只有在试图获取不存在的属性时才会被调用，`__getattribute__` 在每次尝试访问属性时都会被调用。<br />代码如下：
```python
class Dog:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    def __getattr__(self, key):
        return f"{key} not found"
dog = Dog("taidi", 5)
print(dog.name)    # taidi
print(dog.age)     # 5
print(dog.breed)   # breed not found
```
```python
class Dog:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    def __getattribute__(self, key):
        return f"{key} not found"
dog = Dog("taidi", 5)
print(dog.name)    # name not found
print(dog.age)     # age not found
print(dog.breed)   # breed not found
```
<a name="SYejW"></a>
## 3、`super().__init__()` 的另一种写法
```python
class Animal:
    def __init__(self, name, age):
        self.name = name
        self.age = age
class Dog(Animal):
    def __init__(self, name, age, breed):
        super().__init__(name, age)
        self.breed = breed
```
等价于：
```python
class Animal:
    def __init__(self, name, age):
        self.name = name
        self.age = age
class Dog(Animal):
    def __init__(self, name, age, breed):
        Animal.__init__(self, name, age)
        self.breed = breed
```
请注意，`Animal.__init__(self, name, age)` 不能少了 self 参数。
<a name="wPJDo"></a>
## 4、检查子类的方法
```python
class Animal: pass
class Dog(Animal): pass
class Cat(Animal): pass
class GermanSheperd(Dog): pass
print(Animal.__subclasses__())
# [<class '__main__.Dog'>, <class '__main__.Cat'>]
```
不过，`.__subclasses__()` 只能检查直接子类。
<a name="U651j"></a>
## 5、多重继承时同名函数子类用的是哪一个？
```python
class A:
    def test(self):
        print("A")
class B:
    def test(self):
        print("B")
class C(A, B):
    pass

C().test()   # A
```
A 和 B 都有 test 方法，那么 C 到底继承了哪一个呢？在 Python 中，最左边的类优先。<br />在这里，A 是最左边的父类，因此 A 的 test 方法被继承。<br />多重继承让人困惑，不用为好。
<a name="YCNvc"></a>
## 6、`__invert__` 魔法函数
```python
class Dog:
    def __invert__(self):
        return "test"
dog = Dog()
print(~dog)   # test
```
`~` 运算符代表“按位非”，通常用于反转内容。一个更有意义的例子如下：
```python
class Coordinate:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __str__(self):
        return f"({self.x}, {self.y})"
    def __invert__(self):
        return Coordinate(-self.x, -self.y)
a = Coordinate(3, 4)
b = ~a
print(a, b)   # (3, 4) (-3, -4)
```
<a name="Gffbu"></a>
## 7、不使用 class 来创建类
```python
def init(self, name, age):
    self.name = name
    self.age = age
def bark(self):
    print("woof")
Dog = type("Dog", (), {"__init__":init, "bark":bark})


dog = Dog("taidi", 10)
print(dog.name)
print(dog.age)

# taidi
# 10
```
在这里，将 3 个参数传递给 type 以创建类。<br />第一个参数 `__name__` 是类的名称 第二个参数 `__bases__` 是一个包含父类的元组 第三个参数 `__dict__` 是一个包含属性和方法的字典。<br />等价于：
```python
class Dog:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    def bark(self):
        print("woof")
```
