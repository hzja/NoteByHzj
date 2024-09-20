Python

- 在 Python 中创建一个类及其对象
- 在 Python 中创建一个空类
- 在 Python 中使用 `Type` 创建类
- 在 Python 中创建和调用类的方法
- 使用 `__init__()` 方法为数据属性赋值
- 在 Python 中更新对象属性
- 在 Python 中删除对象属性和对象
- 在 Python 中检查和比较对象的类型
- 在Python中将对象的所有属性复制到另一个对象
- 在 Python 中迭代对象属性
- 在 Python 中打印对象的所有属性
- 在python中在运行时创建类的数据属性
- 在函数中将对象的实例作为参数传递
- 在 Python 中创建和使用自定义 Self 参数
- 使用self参数来维护对象的状态
- 在 Python 中创建和使用静态类变量
- 在 Python 中的一个函数上使用多个装饰器
- 在 Python 中的方法中同时访问 `cls` 和 `self`
- 从装饰器访问实例方法的类
- 使用给定的装饰器获取 Python 类的所有方法
- 装饰一个 class
- 将类字段作为参数传递给类方法上的装饰器
- 在 Python 中创建多个传入参数列表的类变量
- Python 中的 wraps 装饰器
- 使用可选参数构建装饰器
- 在 Python 中将参数传递给装饰器
- `@property` 装饰器
- 类和函数的装饰器
- Python 中带参数和返回值的装饰器
- Python 使用参数 `wraps` 装饰器
- Python 装饰器获取类名
- 简单装饰器示例
- 在 Python 中使用 `print()` 打印类的实例
- 在 Python 中的类中将装饰器定义为方法
- 获取在 Python 中修饰的给定类的所有方法
- 带参数和不带参数的 Python 装饰器
- Python 中带有 self 参数的类方法装饰器
- 在 Python 中的另一个类中使用隐藏的装饰器
- 装饰器内部的 self 对象
- 在 Python 中将多个装饰器应用于单个函数
- Python 装饰器获取类实例
- `__init__` 和 `__call__` 有什么区别
- 在 Python 中使用 `__new__` 和 `__init__`
- Python 中的迭代重载方法
- 在 Python 中使用迭代器反转字符串
- Python 中 `__reversed__` 魔术方法
- Python 中的 `__getitem__` 和 `__setitem__`
- 在 Python 中使用 `__getattr__` 和 `__setattr__` 进行属性赋值
- 什么是 `__del__` 方法以及如何调用它
- 创建类的私有成员
- 一个 Python 封装的例子
- 一个 Python 组合的例子
- 一个Python聚合的例子
- Python 中的单级、多级和多级继承
- 在 Python 中获取一个类的父类
- Python 中的多态性
- 访问 Child 类中的私有成员
- Python 中的抽象类
- 创建一个抽象类来覆盖 Python 中的默认构造函数
- 使一个抽象类继承另一个抽象类
- Python 中的 `super` 是做什么的
- `super()` 如何在多重继承中与 `__init__()` 方法一起工作
- 将 `super` 与类方法一起使用
- mro 是做什么的
- Python 中的元类是什么
- 元类的具体案例
- 在 Python 中使用元类的单例类
- `@staticmethod` 和 `@classmethod` 有什么区别
- Python 中的装饰器是什么
- 制作函数装饰器链
<a name="BxvsH"></a>
## 1、在 Python 中创建一个类及其对象
```python
class Employee:
    salary = 10000
    name = "John Doe"

emp1 = Employee()
print(emp1.salary)
print(emp1.name)
```
Output:
```python
10000
John Doe
```
<a name="YiKXF"></a>
## 2、在 Python 中创建一个空类
```python
class Employee:
    pass

e1 = Employee()
print(e1)
 
e1.name = "John Doe"
print(e1.name)
```
Output:
```python
<__main__.Employee object at 0x0000000002DA51D0>
John Doe
```
<a name="xOXs4"></a>
## 3、在 Python 中使用 `Type` 创建类
```python
e1 = type('Employee', (), {})()
print(e1)

e1.name = "John Doe"
print(e1.name)
```
Output:
```python
<__main__.Employee object at 0x0000000002DCC780>
John Doe
```
<a name="Cndfx"></a>
## 4、在 Python 中创建和调用类的方法
```python
class Employee:
    salary = 10000
    name = "John Doe"
 
    def tax(self):
        print(self.salary * 0.10)

emp1 = Employee()
print(emp1.salary)
print(emp1.name)
emp1.tax()
```
Output:
```python
10000
John Doe
1000.0
```
<a name="Z6t40"></a>
## 5、使用 `init()` 方法为数据属性赋值
```python
class Employee:
    def __init__(self, salary, name):
        self.salary = salary
        self.name = name

emp1 = Employee(10000, "John Doe")
print(emp1.salary)
print(emp1.name)
```
Output:
```python
10000
John Doe
```
<a name="w0XnS"></a>
## 6、在 Python 中更新对象属性
```python
class Employee:
    def __init__(self, salary, name):
        self.salary = salary
        self.name = name

emp1 = Employee(10000, "John Doe")
print(emp1.salary)
 
emp1.salary = 20000
print(emp1.salary)
```
Output:
```python
10000
20000
```
<a name="Kx594"></a>
## 7、在 Python 中删除对象属性和对象
```python
class Employee:
    def __init__(self, salary, name):
        self.salary = salary
        self.name = name

emp1 = Employee(10000, "John Doe")

del emp1.salary     # Delete object property
del emp1            # Delete object
```
Output:
```python
哈哈
```
<a name="BHXcu"></a>
## 8、在 Python 中检查和比较对象的类型
```python
class Test(object):
    pass

print(type(Test))

obj1 = Test()
print(type(obj1))

obj2 = Test()
print(type(obj1) is type(obj2))
```
Output:
```python
< class 'type' >
< class '__main__.Test' >
True
```
<a name="UBmTS"></a>
## 9、在Python中将对象的所有属性复制到另一个对象
```python
class MyClass(object):
    def __init__(self):
        super(MyClass, self).__init__()
        self.foo = 1
        self.bar = 2

obj1 = MyClass()
obj2 = MyClass()
 
obj1.foo = 25
obj2.__dict__.update(obj1.__dict__)
 
print(obj1.foo)
print(obj2.foo)
```
Output:
```python
25
25
```
<a name="W6Jwy"></a>
## 10、在 Python 中迭代对象属性
```python
class A():
    m = 1
    n = 2

    def __int__(self, x=1, y=2, z=3):
        self.x = x
        self._y = y
        self.__z__ = z

    def xyz(self):
        print(x, y, z)

obj = A()
print(dir(obj))
print([a for a in dir(obj) if not a.startswith('__')])
```
Output:
```python
['__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__int__', '__le__', '__lt__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'm', 'n', 'xyz']
['m', 'n', 'xyz']
```
<a name="ZqPEm"></a>
## 11、在 Python 中打印对象的所有属性
```python
class Animal(object):
    def __init__(self):
        self.eyes = 2
        self.name = 'Dog'
        self.color= 'Spotted'
        self.legs= 4
        self.age  = 10
        self.kids = 0

animal = Animal()
animal.tail = 1
 
temp = vars(animal)
for item in temp:
    print(item, ':', temp[item])
```
Output:
```python
kids : 0
eyes : 2
name : Dog
color : Spotted
tail : 1
legs : 4
age : 10
```
<a name="Vo8SJ"></a>
## 12、在python中在运行时创建类的数据属性
```python
class Employee:
    pass

emp1 = Employee()
setattr(emp1, 'Salary', 12000)
 
emp2 = Employee()
setattr(emp2, 'Age', 25)
 
print(emp1.Salary)
print(emp2.Age)
```
Output:
```python
12000
25
```
<a name="hgyZV"></a>
## 13、在函数中将对象的实例作为参数传递
```python
class Vehicle:
    def __init__(self):
        self.trucks = []

    def add_truck(self, truck):
        self.trucks.append(truck)

class Truck:
    def __init__(self, color):
        self.color = color

    def __repr__(self):
        return "{}".format(self.color)

def main():
    v = Vehicle()
    for t in 'Red Blue Black'.split():
        t = Truck(t)
        v.add_truck(t)
    print(v.trucks)


if __name__ == "__main__":
    main()
```
Output:
```python
[Red, Blue, Black]
```
<a name="MZu8I"></a>
## 14、在 Python 中创建和使用自定义 `Self` 参数
```python
class Employee:
    def __init__(person, salary, name):
        person.salary = salary
        person.name = name
 
    def print_details(emp):
        print(str(emp.salary) + ' : ' + emp.name)

emp1 = Employee(10000, 'John Doe')
emp1.print_details()
```
Output:
```python
10000 : John Doe
```
<a name="Esu5J"></a>
## 15、使用`self`参数来维护对象的状态
```python
class State(object):
    def __init__(self):
        self.field = 5.0

    def add(self, x):
        self.field += x

    def mul(self, x):
        self.field *= x

    def div(self, x):
        self.field /= x

    def sub(self, x):
        self.field -= x

s = State()
print(s.field)

s.add(2)         # Self is implicitly passed.
print(s.field)

s.mul(2)         # Self is implicitly passed.
print(s.field)

s.div(2)         # Self is implicitly passed.
print(s.field)

s.sub(2)         # Self is implicitly passed.
print(s.field)
```
Output:
```python
5.0
7.0
14.0
7.0
5.0
```
<a name="EUHQo"></a>
## 16、在 Python 中创建和使用静态类变量
```python
class Employee:
    age = 25

print(Employee.age)

e = Employee()
print(e.age)

e.age = 30
print(Employee.age)   # 25
print(e.age)          # 30
```
Output:
```python
25
25
25
30
```
<a name="OUeae"></a>
## 17、在 Python 中的一个函数上使用多个装饰器
```python
def my_decorator(func):
    def wrapper():
        print("Step - 1")
        func()
        print("Step - 3")
    return wrapper

def repeat(func):
    def wrapper():
        func()
        func()
        func()
    return wrapper

@my_decorator
@repeat
def start_steps():
    print("Step - 2")


start_steps()
```
Output:
```python
Step - 1
Step - 2
Step - 2
Step - 2
Step - 3
```
<a name="ncJs3"></a>
## 18、在 Python 中的方法中同时访问 `cls` 和 `self`
```python
class MyClass:
    __var2 = 'var2'
    var3 = 'var3'

    def __init__(self):
        self.__var1 = 'var1'

    def normal_method(self):
        print(self.__var1)

    @classmethod
    def class_method(cls):
        print(cls.__var2)

    def my_method(self):
        print(self.__var1)
        print(self.__var2)
        print(self.__class__.__var2)

if __name__ == '__main__':
    print(MyClass.__dict__['var3'])

clzz = MyClass()
clzz.my_method()
```
Output:
```python
var3
var1
var2
var2
```
<a name="DfAIZ"></a>
## 19、从装饰器访问实例方法的类
```python
class Decorator(object):
    def __init__(self, decoratee_enclosing_class):
        self.decoratee_enclosing_class = decoratee_enclosing_class

    def __call__(self, original_func):
        def new_function(*args, **kwargs):
            print('decorating function in ', self.decoratee_enclosing_class)
            original_func(*args, **kwargs)
        return new_function


class Bar(object):
    @Decorator('Bar')
    def foo(self):
        print('in foo')


class Baz(object):
    @Decorator('Baz')
    def foo(self):
        print('in foo')


print('before instantiating Bar()')
b = Bar()
print('calling b.foo()')
b.foo()
```
Output:
```python
before instantiating Bar()
calling b.foo()
decorating function in  Bar
in foo
```
<a name="XBEoG"></a>
## 20、使用给定的装饰器获取 Python 类的所有方法
```python
import inspect

def deco(func):
    return func

def deco2():
    def wrapper(func):
        pass
    return wrapper

class Test(object):
    @deco
    def method(self):
        pass

    @deco2()
    def method2(self):
        pass

def methodsWithDecorator(cls, decoratorName):
    sourcelines = inspect.getsourcelines(cls)[0]
    for i, line in enumerate(sourcelines):
        line = line.strip()
        if line.split('(')[0].strip() == '@' + decoratorName:  # leaving a bit out
            nextLine = sourcelines[i + 1]
            name = nextLine.split('def')[1].split('(')[0].strip()
            yield(name)

print(list(methodsWithDecorator(Test, 'deco')))
print(list(methodsWithDecorator(Test, 'deco2')))
```
Output:
```python
['method']
['method2']
```
<a name="rS4Sd"></a>
## 21、装饰一个 class
```python
from functools import wraps

def dec(msg='default'):
    def decorator(klass):
        old_foo = klass.foo

        @wraps(klass.foo)
        def decorated_foo(self, *args, **kwargs):
            print('@decorator pre %s' % msg)
            old_foo(self, *args, **kwargs)
            print('@decorator post %s' % msg)
        klass.foo = decorated_foo
        return klass
    return decorator


@dec('foo decorator')
class Foo(object):
    def foo(self, *args, **kwargs):
        print('foo.foo()')


@dec('subfoo decorator')
class SubFoo(Foo):
    def foo(self, *args, **kwargs):
        print('subfoo.foo() pre')
        super(SubFoo, self).foo(*args, **kwargs)
        print('subfoo.foo() post')


@dec('subsubfoo decorator')
class SubSubFoo(SubFoo):
    def foo(self, *args, **kwargs):
        print('subsubfoo.foo() pre')
        super(SubSubFoo, self).foo(*args, **kwargs)
        print('subsubfoo.foo() post')


SubSubFoo().foo()
```
Output:
```python
@decorator pre subsubfoo decorator
subsubfoo.foo() pre
@decorator pre subfoo decorator
subfoo.foo() pre
@decorator pre foo decorator
foo.foo()
@decorator post foo decorator
subfoo.foo() post
@decorator post subfoo decorator
subsubfoo.foo() post
@decorator post subsubfoo decorator
```
<a name="TLKaz"></a>
## 22、将类字段作为参数传递给类方法上的装饰器
```python
import functools

# imagine this is at some different place and cannot be changed


def check_authorization(some_attr, url):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            print(f"Welcome Message: '{url}'...")
            return func(*args, **kwargs)
        return wrapper
    return decorator

# another dummy function to make the example work


def do_work():
    print("work is done...")


def custom_check_authorization(some_attr):
    def decorator(func):
        # assuming this will be used only on this particular class
        @functools.wraps(func)
        def wrapper(self, *args, **kwargs):
            # get url
            url = self.url
            # decorate function with original decorator, pass url
            return check_authorization(some_attr, url)(func)(self, *args, **kwargs)
        return wrapper
    return decorator


class Client(object):
    def __init__(self, url):
        self.url = url

    @custom_check_authorization("some_attr")
    def get(self):
        do_work()


# create object
client = Client('Hello World')

# call decorated function
client.get()
```
Output:
```python
Welcome Message: 'Hello World'...
work is done...
```
<a name="Aet8v"></a>
## 23、在 Python 中创建多个传入参数列表的类变量
```python
class Employee(object):
    def __init__(self, **kwargs):
        for key in kwargs:
            setattr(self, key, kwargs[key])

emp = Employee(age=25, name="John Doe")
print(emp.age)
print(emp.name)
```
Output:
```python
25
John Doe
```
<a name="AukjL"></a>
## 24、Python 中的 wraps 装饰器
```python
from functools import wraps

def decorator_func_with_args(arg1, arg2):
    def decorator(f):
        @wraps(f)
        def wrapper(*args, **kwargs):
            print("Before orginal function with decorator args:", arg1, arg2)
            result = f(*args, **kwargs)
            print("Ran after the orginal function")
            return result
        return wrapper
    return decorator

@decorator_func_with_args("test1", "test2")
def hello(name):
    """A function which prints a greeting to the name provided.
    """
    print('Hello ', name)
    return 25

print("Starting script..")
x = hello('John')
print("The value of x is:", x)
print("The wrapped functions docstring is:", hello.__doc__)
print("The wrapped functions name is:", hello.__name__)
```
Output:
```python
Starting script..
Before orginal function with decorator args: test1 test2
Hello  John
Ran after the orginal function
The value of x is: 25
The wrapped functions docstring is: A function which prints a greeting to the name provided.
    
The wrapped functions name is: hello
```
<a name="EAwl0"></a>
## 25、使用可选参数构建装饰器
```python
def d(arg):
    if callable(arg):  # Assumes optional argument isn't.
        def newfn():
            print('my default message')
            return arg()
        return newfn
    else:
        def d2(fn):
            def newfn():
                print(arg)
                return fn()
            return newfn
        return d2


@d('This is working')
def hello():
    print('hello world !')


@d  # No explicit arguments will result in default message.
def hello2():
    print('hello2 world !')


@d('Applying it twice')
@d('Would also work')
def hello3():
    print('hello3 world !')


hello()
hello2()
hello3()
```
Output:
```python
This is working
hello world !
my default message
hello2 world !
Applying it twice
Would also work
hello3 world !
```
<a name="iQBy1"></a>
## 26、在 Python 中将参数传递给装饰器
```python
def decorator_maker_with_arguments(decorator_arg1, decorator_arg2, decorator_arg3):
    def decorator(func):
        def wrapper(function_arg1, function_arg2, function_arg3):
            print("The wrapper can access all the variables\n"
                  "\t- from the decorator maker: {0} {1} {2}\n"
                  "\t- from the function call: {3} {4} {5}\n"
                  "and pass them to the decorated function"
                  .format(decorator_arg1, decorator_arg2, decorator_arg3,
                          function_arg1, function_arg2, function_arg3))
            return func(function_arg1, function_arg2, function_arg3)

        return wrapper

    return decorator


@decorator_maker_with_arguments("canada", "us", "brazil")
def decorated_function_with_arguments(function_arg1, function_arg2, function_arg3):
    print("This is the decorated function and it only knows about its arguments: {0}"
          " {1}" " {2}".format(function_arg1, function_arg2, function_arg3))


decorated_function_with_arguments("france", "germany", "uk")
```
Output:
```python
The wrapper can access all the variables
	- from the decorator maker: canada us brazil
	- from the function call: france germany uk
and pass them to the decorated function
This is the decorated function and it only knows about its arguments: france germany uk
```
<a name="U71XT"></a>
## 27、`@property` 装饰器
```python
class Currency:
    def __init__(self, dollars, cents):
        self.total_cents = dollars * 100 + cents
 
    @property
    def dollars(self):
        return self.total_cents // 100
 
    @dollars.setter
    def dollars(self, new_dollars):
        self.total_cents = 100 * new_dollars + self.cents
 
    @property
    def cents(self):
        return self.total_cents % 100
 
    @cents.setter
    def cents(self, new_cents):
        self.total_cents = 100 * self.dollars + new_cents
 
 
currency = Currency(10, 20)
print(currency.dollars, currency.cents, currency.total_cents)
 
currency.dollars += 5
print(currency.dollars, currency.cents, currency.total_cents)
 
currency.cents += 15
print(currency.dollars, currency.cents, currency.total_cents)
```
Output:
```python
10 20 1020
15 20 1520
15 35 1535
```
<a name="rYvIq"></a>
## 28、类和函数的装饰器
```python
from functools import wraps

def decorator(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        print('sth to log: %s : %s' % (func.__name__, args))
        return func(*args, **kwargs)
    return wrapper


class Class_test(object):
    @decorator
    def sum_func(self, a, b):
        print('class sum: %s' % (a + b))
        return a + b


print(Class_test.sum_func(1, 5, 16))
```
Output:
```python
sth to log: sum_func : (1, 5, 16)
class sum: 21
21
```
<a name="Vm85i"></a>
## 29、Python 中带参数和返回值的装饰器
```python
def calculation(func):
    def wrapper(*args, **kwargs):

        print("Inside the calculation function")

        num_sum = func(*args, **kwargs)
        print("Before return from calculation function")

        return num_sum

    return wrapper


@calculation
def addition(a, b):
    print("Inside the addition function")
    return a + b


print("Sum =", addition(5, 10))
```
Output:
```python
Inside the calculation function
Inside the addition function
Before return from calculation function
Sum = 15
```
<a name="jJEpw"></a>
## 30、Python 使用参数 `wraps` 装饰器
```python
from functools import wraps

def decorator_func_with_args(arg1, arg2):
    def decorator(f):
        @wraps(f)
        def wrapper(*args, **kwargs):
            print("Before orginal function with decorator args:", arg1, arg2)
            result = f(*args, **kwargs)
            print("Ran after the orginal function")
            return result
        return wrapper
    return decorator

@decorator_func_with_args("test1", "test2")
def hello(name):
    """A function which prints a greeting to the name provided.
    """
    print('Hello ', name)
    return 25


print("Starting script..")
x = hello('John')
print("The value of x is:", x)
print("The wrapped functions docstring is:", hello.__doc__)
print("The wrapped functions name is:", hello.__name__)
```
Output:
```python
Starting script..
Before orginal function with decorator args: test1 test2
Hello  John
Ran after the orginal function
The value of x is: 25
The wrapped functions docstring is: A function which prints a greeting to the name provided.
    
The wrapped functions name is: hello
```
<a name="spP27"></a>
## 31、Python 装饰器获取类名
```python
def print_name(*args):
    def _print_name(fn):
        def wrapper(*args, **kwargs):
            print('{}.{}'.format(fn.__module__, fn.__qualname__))
            return fn(*args, **kwargs)
        return wrapper
    return _print_name


class A():
    @print_name()
    def a():
        print('Hi from A.a')


@print_name()
def b():
    print('Hi from b')


A.a()
b()
```
Output:
```python
__main__.A.a
Hi from A.a
__main__.b
Hi from b
```
<a name="ZLBSu"></a>
## 32、简单装饰器示例
```python
def my_decorator(func):
    def wrapper():
        print("Step - 1")
        func()
        print("Step - 3")
    return wrapper


@my_decorator
def start_steps():
    print("Step - 2")


start_steps()
```
Output:
```python
Step - 1
Step - 2
Step - 3
```
<a name="UAQmc"></a>
## 33、在 Python 中使用 `print()` 打印类的实例
```python
class Element:
    def __init__(self, name, city, population):
        self.name = name
        self.city = city
        self.population = population

    def __str__(self):
        return str(self.__class__) + '\n' + '\n'.join(('{} = {}'.format(item, self.__dict__[item]) for item in self.__dict__))


elem = Element('canada', 'tokyo', 321345)
print(elem)
```
Output:
```python
name = canada
city = tokyo
population = 321345
```
<a name="XGAHb"></a>
## 34、在 Python 中的类中将装饰器定义为方法
```python
class myclass:
    def __init__(self):
        self.cnt = 0

    def counter(self, function):
        """
    this method counts the number of runtime of a function
    """
        def wrapper(**args):
            function(self, **args)
            self.cnt += 1
            print('Counter inside wrapper: ', self.cnt)
        return wrapper


global counter_object
counter_object = myclass()


@counter_object.counter
def somefunc(self):
    print("Somefunc called")


somefunc()
print(counter_object.cnt)

somefunc()
print(counter_object.cnt)

somefunc()
print(counter_object.cnt)
```
Output:
```python
Somefunc called
Counter inside wrapper:  1
1
Somefunc called
Counter inside wrapper:  2
2
Somefunc called
Counter inside wrapper:  3
3
```
<a name="tTaoB"></a>
## 35、获取在 Python 中修饰的给定类的所有方法
```python
class awesome(object):
    def __init__(self, method):
        self._method = method

    def __call__(self, obj, *args, **kwargs):
        return self._method(obj, *args, **kwargs)

    @classmethod
    def methods(cls, subject):
        def g():
            for name in dir(subject):
                method = getattr(subject, name)
                if isinstance(method, awesome):
                    yield name, method
        return {name: method for name, method in g()}


class Robot(object):
    @awesome
    def think(self):
        return 0

    @awesome
    def walk(self):
        return 0

    def irritate(self, other):
        return 0


print(awesome.methods(Robot))
```
Output:
```python
{'think': <__main__.awesome object at 0x00000213C052AAC0>, 'walk': <__main__.awesome object at 0x00000213C0E33FA0>}
```
<a name="kI00K"></a>
## 36、带参数和不带参数的 Python 装饰器
```python
def someDecorator(arg=None):
    def decorator(func):
        def wrapper(*a, **ka):
            if not callable(arg):
                print(arg)
                return func(*a, **ka)
            else:
                return 'xxxxx'
        return wrapper

    if callable(arg):
        return decorator(arg)  # return 'wrapper'
    else:
        return decorator  # ... or 'decorator'


@someDecorator(arg=1)
def my_func():
    print('my_func')


@someDecorator
def my_func1():
    print('my_func1')


if __name__ == "__main__":
    my_func()
    my_func1()
```
Output:
```python
1
my_func
```
<a name="JD1PD"></a>
## 37、Python 中带有 `self` 参数的类方法装饰器
```python
def check_authorization(f):
    def wrapper(*args):
        print('Inside wrapper function argement passed :', args[0].url)
        return f(*args)
    return wrapper


class Client(object):
    def __init__(self, url):
        self.url = url

    @check_authorization
    def get(self):
        print('Inside get function argement passed :', self.url)


Client('Canada').get()
```
Output:
```python
Inside wrapper function argement passed : Canada
Inside get function argement passed : Canada
```
<a name="JkZUV"></a>
## 38、在 Python 中的另一个类中使用隐藏的装饰器
```python
class TestA(object):
    def _decorator(foo):
        def magic(self):
            print("Start magic")
            foo(self)
            print("End magic")
        return magic

    @_decorator
    def bar(self):
        print("Normal call")

    _decorator = staticmethod(_decorator)


class TestB(TestA):
    @TestA._decorator
    def bar(self):
        print("Override bar in")
        super(TestB, self).bar()
        print("Override bar out")


print("Normal:")
test = TestA()
test.bar()
print('-' * 10)

print("Inherited:")
b = TestB()
b.bar()
```
Output:
```python
Normal:
Start magic
Normal call
End magic
----------
Inherited:
Start magic
Override bar in
Start magic
Normal call
End magic
Override bar out
End magic
```
<a name="cb4mB"></a>
## 39、装饰器内部的 `self` 对象
```python
import random


def only_registered_users(func):
    def wrapper(handler):
        print('Checking if user is logged in')
        if random.randint(0, 1):
            print('User is logged in. Calling the original function.')
            func(handler)
        else:
            print('User is NOT logged in. Redirecting...')
    return wrapper


class MyHandler(object):

    @only_registered_users
    def get(self):
        print('Get function called')


m = MyHandler()
m.get()
```
Output:
```python
Checking if user is logged in
User is logged in. Calling the original function.
Get function called
```
<a name="pKhTs"></a>
## 40、在 Python 中将多个装饰器应用于单个函数
```python
def multiplication(func):
    def wrapper(*args, **kwargs):
        num_sum = func(*args, **kwargs)
        print("Inside the multiplication function", num_sum)
        return num_sum * num_sum

    return wrapper


def addition(func):
    def wrapper(*args, **kwargs):
        num_sum = func(*args, **kwargs)
        print("Inside the addition function", num_sum)
        return num_sum + num_sum

    return wrapper


@addition
@multiplication
def calculation(a):
    print("Inside the calculation function", a)
    return a


print("Sum =", calculation(5))
```
Output:
```python
Inside the calculation function 5
Inside the multiplication function 5
Inside the addition function 25
Sum = 50
```
<a name="AH2RC"></a>
## 41、Python 装饰器获取类实例
```python
class MySerial():
    def __init__(self):
        pass  # I have to have an __init__

    def write(self, *args):
        print(args[0])
        pass  # write to buffer

    def read(self):
        pass  # read to buffer

    @staticmethod
    def decorator(func):
        def func_wrap(cls, *args, **kwargs):
            cls.ser.write(func(cls, *args, **kwargs))
            return cls.ser.read()
        return func_wrap


class App():
    def __init__(self):
        self.ser = MySerial()

    @MySerial.decorator
    def myfunc(self):
        self = 100
        return ['canada', 'australia']


App().myfunc()
```
Output:
```python
['canada', 'australia']
```
<a name="gjGEm"></a>
## 42、`init` 和 `call` 有什么区别
```python
class Counter:
    def __init__(self):
        self._weights = []
        for i in range(0, 2):
            self._weights.append(1)
        print(str(self._weights[-2]) + " No. from __init__")

    def __call__(self, t):
        self._weights = [self._weights[-1], self._weights[-1]
                         + self._weights[-1]]
        print(str(self._weights[-1]) + " No. from __call__")


num_count = Counter()
for i in range(0, 4):
    num_count(i)
```
Output:
```python
1 No. from __init__
2 No. from __call__
4 No. from __call__
8 No. from __call__
16 No. from __call__
```
<a name="naDlJ"></a>
## 43、在 Python 中使用 `new` 和 `init`
```python
class Shape:
    def __new__(cls, sides, *args, **kwargs):
        if sides == 3:
            return Triangle(*args, **kwargs)
        else:
            return Square(*args, **kwargs)


class Triangle:
    def __init__(self, base, height):
        self.base = base
        self.height = height

    def area(self):
        return (self.base * self.height) / 2


class Square:
    def __init__(self, length):
        self.length = length

    def area(self):
        return self.length*self.length


a = Shape(sides=3, base=2, height=12)
b = Shape(sides=4, length=2)

print(str(a.__class__))
print(a.area())

print(str(b.__class__))
print(b.area())
```
Output:
```python
class '__main__.Triangle'
12.0
class '__main__.Square'
4
```
<a name="rWTHT"></a>
## 44、Python 中的迭代重载方法
```python
class Counter:
    def __init__(self, low, high):
        self.current = low
        self.high = high

    def __iter__(self):
        return self

    def __next__(self):
        if self.current > self.high:
            raise StopIteration
        else:
            self.current += 1
            return self.current - 1


for num in Counter(5, 15):
    print(num)
```
Output:
```python
5
6
..
..
15
```
<a name="JNKZn"></a>
## 45、在 Python 中使用迭代器反转字符串
```python
class Reverse:
    def __init__(self, data):
        self.data = data
        self.index = len(data)

    def __iter__(self):
        return self

    def __next__(self):
        if self.index == 0:
            raise StopIteration
        self.index = self.index - 1
        return self.data[self.index]


test = Reverse('Python')
for char in test:
    print(char)
```
Output:
```python
n
o
h
t
y
P
```
<a name="zor1I"></a>
## 46、Python 中 `reversed` 魔术方法
```python
class Count:
    def __init__(self, start, end):
        self.start = start
        self.end = end
        self.current = None

    def __iter__(self):
        self.current = self.start
        while self.current < self.end:
            yield self.current
            self.current += 1

    def __next__(self):
        if self.current is None:
            self.current = self.start
        if self.current > self.end:
            raise StopIteration
        else:
            self.current += 1
            return self.current-1

    def __reversed__(self):
        self.current = self.end
        while self.current >= self.start:
            yield self.current
            self.current -= 1


obj1 = Count(0, 5)
for i in obj1:
    print(i)

obj2 = reversed(obj1)
for i in obj2:
    print(i)
```
Output:
```python
0
1
2
....
2
1
0
```
<a name="qf5Rv"></a>
## 47、Python 中的 `getitem` 和 `setitem`
```python
class Counter(object):
    def __init__(self, floors):
        self._floors = [None]*floors

    def __setitem__(self, floor_number, data):
        self._floors[floor_number] = data

    def __getitem__(self, floor_number):
        return self._floors[floor_number]


index = Counter(4)
index[0] = 'ABCD'
index[1] = 'EFGH'
index[2] = 'IJKL'
index[3] = 'MNOP'

print(index[2])
```
Output:
```python
IJKL
```
<a name="H5r9v"></a>
## 48、在 Python 中使用 `getattr` 和 `setattr` 进行属性赋值
```python
class Employee(object):
    def __init__(self, data):
        super().__setattr__('data', dict())
        self.data = data

    def __getattr__(self, name):
        if name in self.data:
            return self.data[name]
        else:
            return 0

    def __setattr__(self, key, value):
        if key in self.data:
            self.data[key] = value
        else:
            super().__setattr__(key, value)


emp = Employee({'age': 23, 'name': 'John'})
print(emp.age)
print(emp.name)
print(emp.data)
print(emp.salary)

emp.salary = 50000
print(emp.salary)
```
Output:
```python
23
John
{'age': 23, 'name': 'John'}
0
50000
```
<a name="ZqTNl"></a>
## 49、什么是 `del` 方法以及如何调用它
```python
class Employee():
    def __init__(self, name='John Doe'):
        print('Hello ' + name)
        self.name = name

    def developer(self):
        print(self.name)

    def __del__(self):
        print('Good Bye ' + self.name)


emp = Employee('Mark')
print(emp)

emp = 'Rocky'
print(emp)
```
Output:
```python
Hello Mark
<__main__.Employee object at 0x00000000012498D0>
Good Bye Mark
Rocky
```
<a name="yX4CS"></a>
## 50、创建类的私有成员
```python
class Test(object):
    __private_var = 100
    public_var = 200

    def __private_func(self):
        print('Private Function')

    def public_func(self):
        print('Public Function')
        print(self.public_var)

    def call_private(self):
        self.__private_func()
        print(self.__private_var)


t = Test()
print(t.call_private())
print(t.public_func())
```
Output:
```python
Private Function
100
None
Public Function
200
None
```
<a name="Z48Ma"></a>
## 51、一个 Python 封装的例子
```python
class Encapsulation:
    __name = None

    def __init__(self, name):
        self.__name = name

    def get_name(self):
        return self.__name


pobj = Encapsulation('Rocky')
print(pobj.get_name())
```
Output:
```python
Rocky
```
<a name="UNpGc"></a>
## 52、一个 Python 组合的例子
```python
class Salary:
    def __init__(self, pay):
        self.pay = pay

    def get_total(self):
        return (self.pay*12)


class Employee:
    def __init__(self, pay, bonus):
        self.pay = pay
        self.bonus = bonus
        self.obj_salary = Salary(self.pay)

    def annual_salary(self):
        return "Total: " + str(self.obj_salary.get_total() + self.bonus)


obj_emp = Employee(600, 500)
print(obj_emp.annual_salary())
```
Output:
```python
Total: 7700
```
<a name="Ajxw6"></a>
## 53、一个Python聚合的例子
```python
class Salary:
    def __init__(self, pay):
        self.pay = pay

    def get_total(self):
        return (self.pay*12)


class Employee:
    def __init__(self, pay, bonus):
        self.pay = pay
        self.bonus = bonus

    def annual_salary(self):
        return "Total: " + str(self.pay.get_total() + self.bonus)


obj_sal = Salary(600)
obj_emp = Employee(obj_sal, 500)
print(obj_emp.annual_salary())
```
Output:
```python
Total: 7700
```
<a name="GNliI"></a>
## 54、Python 中的单级、多级和多级继承
```python
# Single inheritence
class Apple:
    manufacturer = 'Apple Inc'
    contact_website = 'www.apple.com/contact'
    name = 'Apple'

    def contact_details(self):
        print('Contact us at ', self.contact_website)


class MacBook(Apple):
    def __init__(self):
        self.year_of_manufacture = 2018

    def manufacture_details(self):
        print('This MacBook was manufactured in {0}, by {1}.'
              .format(self.year_of_manufacture, self.manufacturer))


macbook = MacBook()
macbook.manufacture_details()


# Multiple inheritence
class OperatingSystem:
    multitasking = True
    name = 'Mac OS'


class MacTower(OperatingSystem, Apple):
    def __init__(self):
        if self.multitasking is True:
            print('Multitasking system')
            # if there are two superclasses with the sae attribute name
            # the attribute of the first inherited superclass will be called
        # the order of inhertence matters
        print('Name: {}'.format(self.name))


mactower = MacTower()


# Multilevel inheritence
class MusicalInstrument:
    num_of_major_keys = 12


class StringInstrument(MusicalInstrument):
    type_of_wood = 'Tonewood'


class Guitar(StringInstrument):
    def __init__(self):
        self.num_of_strings = 6
        print('The guitar consists of {0} strings,' +
              'it is made of {1} and can play {2} keys.'
              .format(self.num_of_strings,
                      self.type_of_wood, self.num_of_major_keys))


guitar = Guitar()
```
Output:
```python
This MacBook was manufactured in 2018, by Apple Inc.
Multitasking system
Name: Mac OS
The guitar consists of 6 strings, it is made of Tonewood and can play 12 keys.
```
<a name="WNh67"></a>
## 55、在 Python 中获取一个类的父类
```python
class A(object):
    pass


class B(object):
    pass


class C(A, B):
    pass


print(C.__bases__)
```
Output:
```python
(< class '__main__.A' >, < class '__main__.B' >)
```
<a name="X82DX"></a>
## 56、Python 中的多态性
```python
# Creating a shape Class
class Shape:
    width = 0
    height = 0

    # Creating area method
    def area(self):
        print("Parent class Area ... ")


# Creating a Rectangle Class
class Rectangle(Shape):

    def __init__(self, w, h):
        self.width = w
        self.height = h

    # Overridding area method
    def area(self):
        print("Area of the Rectangle is : ", self.width*self.height)


# Creating a Triangle Class
class Triangle(Shape):

    def __init__(self, w, h):
        self.width = w
        self.height = h

    # Overridding area method
    def area(self):
        print("Area of the Triangle is : ", (self.width*self.height)/2)


rectangle = Rectangle(10, 20)
triangle = Triangle(2, 10)

rectangle.area()
triangle.area()
```
Output:
```python
Area of the Rectangle is :  200
Area of the Triangle is :  10.0
```
<a name="av7t1"></a>
## 57、访问 Child 类中的私有成员
```python
class Human():

    # Private var
    __privateVar = "this is __private variable"

    # Constructor method
    def __init__(self):
        self.className = "Human class constructor"
        self.__privateVar = "this is redefined __private variable"

    # Public method
    def showName(self, name):
        self.name = name
        return self.__privateVar + " " + name

    # Private method
    def __privateMethod(self):
        return "Private method"

    # Public method that returns a private variable
    def showPrivate(self):
        return self.__privateMethod()

    def showProtecded(self):
        return self._protectedMethod()


class Male(Human):
    def showClassName(self):
        return "Male"

    def showPrivate(self):
        return self.__privateMethod()

    def showProtected(self):
        return self._protectedMethod()


class Female(Human):
    def showClassName(self):
        return "Female"

    def showPrivate(self):
        return self.__privateMethod()


human = Human()
print(human.className)
print(human.showName("Vasya"))
print(human.showPrivate())

male = Male()
print(male.className)
print(male.showClassName())

female = Female()
print(female.className)
print(female.showClassName())
```
Output:
```python
Human class constructor
this is redefined __private variable Vasya
Private method
Human class constructor
Male
Human class constructor
Female
```
<a name="HvpYa"></a>
## 58、Python 中的抽象类
```python
from abc import ABC, abstractmethod


class AbstractClass(ABC):
    def __init__(self, value):
        self.value = value
        super().__init__()

    @abstractmethod
    def eat(self):
        pass


class Parents(AbstractClass):
    def eat(self):
        return "Eat solid food " + str(self.value) + " times each day."


class Babies(AbstractClass):
    def eat(self):
        return "Milk only " + str(self.value) + " times or more each day."


food = 3
adult = Parents(food)
print('Adult')
print(adult.eat())

infant = Babies(food)
print('Infants')
print(infant.eat())
```
Output:
```python
Adult
Eat solid food 3 times each day.
Infants
Milk only 3 times or more each day.
```
<a name="S9GE9"></a>
## 59、创建一个抽象类来覆盖 Python 中的默认构造函数
```python
from abc import ABCMeta, abstractmethod


class AbstractClass(object, metaclass=ABCMeta):
    @abstractmethod
    def __init__(self, n):
        self.n = n


class Employee(AbstractClass):
    def __init__(self, salary, name):
        self.salary = salary
        self.name = name


emp1 = Employee(10000, "John Doe")
print(emp1.salary)
print(emp1.name)
```
Output:
```python
10000
John Doe
```
<a name="VkoIO"></a>
## 60、使一个抽象类继承另一个抽象类
```python
from abc import ABC, abstractmethod


class A(ABC):
    def __init__(self, username):
        self.username = username
        super().__init__()

    @abstractmethod
    def name(self):
        pass


class B(A):
    @abstractmethod
    def age(self):
        pass


class C(B):
    def name(self):
        print(self.username)

    def age(self):
        return


c = C('Test1234')
c.name()
```
Output:
```python
Test1234
```
<a name="tIGH6"></a>
## 61、Python 中的 `super` 是做什么的
```python
class A(object):
    def __init__(self, profession):
        print(profession)

class B(A):
    def __init__(self):
        print('John Doe')
        super().__init__('Developer')

b = B()
```
Output:
```python
John Doe
Developer
```
<a name="zAkfP"></a>
## 62、`super()` 如何在多重继承中与 `init()` 方法一起工作
```python
class F:
    def __init__(self):
        print('F%s' % super().__init__)
        super().__init__()

class G:
    def __init__(self):
        print('G%s' % super().__init__)
        super().__init__()

class H:
    def __init__(self):
        print('H%s' % super().__init__)
        super().__init__()

class E(G, H):
    def __init__(self):
        print('E%s' % super().__init__)
        super().__init__()

class D(E, F):
    def __init__(self):
        print('D%s' % super().__init__)
        super().__init__()

class C(E, G):
    def __init__(self):
        print('C%s' % super().__init__)
        super().__init__()

class B(C, H):
    def __init__(self):
        print('B%s' % super().__init__)
        super().__init__()

class A(D, B, E):
    def __init__(self):
        print('A%s' % super().__init__)
        super().__init__()

a = A()
print(a)
```
Output:
```python
A bound method D.__init__ of __main__.A object at 0x000000000369CFD0
D bound method B.__init__ of __main__.A object at 0x000000000369CFD0
B bound method C.__init__ of __main__.A object at 0x000000000369CFD0
C bound method E.__init__ of __main__.A object at 0x000000000369CFD0
E bound method G.__init__ of __main__.A object at 0x000000000369CFD0
G bound method H.__init__ of __main__.A object at 0x000000000369CFD0
H bound method F.__init__ of __main__.A object at 0x000000000369CFD0
F method-wrapper '__init__' of A object at 0x000000000369CFD0
__main__.A object at 0x000000000369CFD0
```
<a name="iHDRe"></a>
## 63、将 super 与类方法一起使用
```python
class A(object):
    @classmethod
    def name(self, employee):
        print('Employee Name: ', employee)


class B(A):
    @classmethod
    def name(self, employee):
        super(B, self).name(employee)


B.name('John Doe')
```
Output:
```python
Employee Name:  John Doe
```
<a name="qDaXD"></a>
## 64、mro 是做什么的
```python
class A(object):
    def dothis(self):
        print('From A class')


class B1(A):
    def dothis(self):
        print('From B1 class')
    pass


class B2(object):
    def dothis(self):
        print('From B2 class')
    pass


class B3(A):
    def dothis(self):
        print('From B3 class')


# Diamond inheritance
class D1(B1, B3):
    pass


class D2(B1, B2):
    pass


d1_instance = D1()
d1_instance.dothis()
print(D1.__mro__)

d2_instance = D2()
d2_instance.dothis()
print(D2.__mro__)
```
Output:
```python
From B1 class
(class '__main__.D1', class '__main__.B1', )
From B1 class
(class '__main__.D2', class '__main__.B1', , class '__main__.B2', class 'object')
```
<a name="hJvpX"></a>
## 65、Python 中的元类是什么
```python
def _addMethod(fldName, clsName, verb, methodMaker, dict):
    compiledName = _getCompiledName(fldName, clsName)
    methodName = _getMethodName(fldName, verb)
    dict[methodName] = methodMaker(compiledName)


def _getCompiledName(fldName, clsName):
    if fldName[:2] == "__" and fldName[-2:] != "__":
        return "_%s%s" % (clsName, fldName)
    else:
        return fldName


def _getMethodName(fldName, verb):
    s = fldName.lstrip("_")
    return verb + s.capitalize()


def _makeGetter(compiledName):
    return lambda self: self.__dict__[compiledName]


def _makeSetter(compiledName):
    return lambda self, value: setattr(self, compiledName, value)


class Accessors(type):
    def __new__(cls, clsName, bases, dict):
        for fldName in dict.get("_READ", []) + dict.get("_READ_WRITE", []):
            _addMethod(fldName, clsName, "get", _makeGetter, dict)
        for fldName in dict.get("_WRITE", []) + dict.get("_READ_WRITE", []):
            _addMethod(fldName, clsName, "set", _makeSetter, dict)
        return type.__new__(cls, clsName, bases, dict)


class Employee(object, metaclass=Accessors):
    _READ_WRITE = ['name', 'salary', 'title', 'bonus']

    def __init__(self, name, salary, title, bonus=0):
        self.name = name
        self.salary = salary
        self.title = title
        self.bonus = bonus


b = Employee('John Doe', 25000, 'Developer', 5000)
print('Name:', b.getName())
print('Salary:', b.getSalary())
print('Title:', b.getTitle())
print('Bonus:', b.getBonus())
```
Output:
```python
Name: John Doe
Salary: 25000
Title: Developer
Bonus: 5000
```
<a name="xnmYK"></a>
## 66、元类的具体案例
```python
class UpperAttrNameMetaClass(type):
    def __new__(cls, clsname, bases, attrdict, *args, **kwargs):
        print('1. Create a new type, from ' +
              ' UpperAttrNameMetaClass.__new__')
        new_attrs = dict()
        for attr, value in attrdict.items():
            if not callable(value) and not str(attr).startswith('__'):
                new_attrs[attr.upper()] = value
            else:
                new_attrs[attr] = value

        cls_obj = super().__new__(cls, clsname, bases, new_attrs,
                                  *args, **kwargs)
        return cls_obj

    def __init__(self, clsname, bases, attrdict):
        self.test = 'test'
        super().__init__(clsname, bases, attrdict)
        print('2. Initialize new type, increase test attribute,' +
              'from UpperAttrNameMetaClass.__init__')

    def __call__(self, *args, **kwargs):
        print('3. Instantiate the new class,' +
              ' from UpperAttrNameMetaClass.__call__')
        new_obj = self.__new__(self, *args, **kwargs)
        new_obj.__init__(*args, **kwargs)
        return new_obj


class ObjectNoInitMetaClass(type):
    def __call__(cls, *args, **kwargs):
        if len(args):
            raise TypeError('Must use keyword argument ' +
                            ' for key function')
        new_obj = cls.__new__(cls)
        for k, v in kwargs.items():
            setattr(new_obj, k.upper(), v)
        return new_obj


class Pig(object, metaclass=UpperAttrNameMetaClass):
    size = 'Big'

    def __new__(cls, *args, **kwargs):
        print('4. Call __new__ in the __call__ of the metaclass,' +
              ' from Pig.__new__')
        obj = object.__new__(cls)
        return obj

    def __init__(self):
        print('5. After the new object is instantiated in ' +
              'the __call__ of the metaclass,the object is promoted,' +
              ' from Pig.__init__')
        self.name = 'Mark'

    def talk(self):
        print(self.name)


Pig().talk()
print(Pig.__dict__)
print(Pig.SIZE)


class AnyOne(metaclass=ObjectNoInitMetaClass):
    pass


foo = AnyOne(name='John', age=28)
print(foo.NAME, foo.AGE)
print(foo.__dict__)
```
Output:
```python
1. Create a new type, from  UpperAttrNameMetaClass.__new__
2. Initialize new type, increase test attribute,from UpperAttrNameMetaClass.__init__
3. Instantiate the new class, from UpperAttrNameMetaClass.__call__
4. Call __new__ in the __call__ of the metaclass, from Pig.__new__
5. After the new object is instantiated in the __call__ of the metaclass,the object is promoted, from Pig.__init__
Mark
{'__doc__': None, 'test': 'test', '__weakref__': , 'SIZE': 'Big', '__init__': , '__dict__': , '__module__': '__main__', '__new__': , 'talk': }
Big
John 28
{'AGE': 28, 'NAME': 'John'}
```
<a name="PEhdc"></a>
## 67、在 Python 中使用元类的单例类
```python
class SingleInstanceMetaClass(type):
    def __init__(self, name, bases, dic):
        self.__single_instance = None
        super().__init__(name, bases, dic)

    def __call__(cls, *args, **kwargs):
        if cls.__single_instance:
            return cls.__single_instance
        single_obj = cls.__new__(cls)
        single_obj.__init__(*args, **kwargs)
        cls.__single_instance = single_obj
        return single_obj


class Setting(metaclass=SingleInstanceMetaClass):
    def __init__(self):
        self.db = 'MySQL'
        self.port = 3306


bar1 = Setting()
bar2 = Setting()

print(bar1 is bar2)
print(bar1.db, bar1.port)
bar1.db = 'ORACLE'
print(bar2.db, bar2.port)
```
Output:
```python
True
MySQL 3306
ORACLE 3306
```
<a name="aDSzw"></a>
## 68、`@staticmethod` 和 `@classmethod` 有什么区别
```python
class Employee:
    @classmethod
    def classmthd(*args):
        return args

    @staticmethod
    def staticmthd(*args):
        return args


print(Employee.classmthd())
print(Employee.classmthd('test'))

print(Employee.staticmthd())
print(Employee.staticmthd('test'))
```
Output:
```python
(class '__main__.Employee',)
(class '__main__.Employee', 'test')
()
('test',)
```
<a name="jU78W"></a>
## 69、Python 中的装饰器是什么
```python
def message(param1, param2):
    def wrapper(wrapped):
        class WrappedClass(wrapped):
            def __init__(self):
                self.param1 = param1
                self.param2 = param2
                super(WrappedClass, self).__init__()

            def get_message(self):
                return "message %s %s" % (self.param1, self.param2)

        return WrappedClass
    return wrapper


@message("param1", "param2")
class Pizza(object):
    def __init__(self):
        pass


pizza_with_message = Pizza()
print(pizza_with_message.get_message())
```
Output:
```python
message param1 param2
```
<a name="OUY7o"></a>
## 70、制作函数装饰器链
```python
def benchmark(func):
    """
    A decorator that prints the time a function takes
    to execute.
    """
    import time

    def wrapper(*args, **kwargs):
        t = time.clock()
        res = func(*args, **kwargs)
        print("{0} {1}".format(func.__name__, time.clock()-t))
        return res
    return wrapper


def logging(func):
    """
    A decorator that logs the activity of the script.
    (it actually just prints it, but it could be logging!)
    """
    def wrapper(*args, **kwargs):
        res = func(*args, **kwargs)
        print("{0} {1} {2}".format(func.__name__, args, kwargs))
        return res
    return wrapper


def counter(func):
    """
    A decorator that counts and prints the number of times a
    function has been executed
    """
    def wrapper(*args, **kwargs):
        wrapper.count = wrapper.count + 1
        res = func(*args, **kwargs)
        print("{0} has been used: {1}x".format(func.__name__, wrapper.count))
        return res
    wrapper.count = 0
    return wrapper


@counter
@benchmark
@logging
def letter_range(start, stop, step=1):
    start = ord(start.lower())
    stop = ord(stop.lower())
    for str_lst in range(start, stop, step):
        yield chr(str_lst)


print(list(letter_range("a", "f")))
print('\n')
print(list(letter_range("m", "z", 2)))
```
Output:
```python
letter_range ('a', 'f') {}
wrapper 0.0009437184107374183
wrapper has been used: 1x
['a', 'b', 'c', 'd', 'e']


letter_range ('m', 'z', 2) {}
wrapper 3.131164480070134e-05
wrapper has been used: 2x
['m', 'o', 'q', 's', 'u', 'w', 'y']
```
