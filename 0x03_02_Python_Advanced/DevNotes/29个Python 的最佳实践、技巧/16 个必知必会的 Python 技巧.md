Python
<a name="hQmYL"></a>
## 1、三元运算符
三元运算符是 if-else 语句的简写。语法是`value_if_true if condition else value_if_false`。三元运算符是一行代码，可以替代多行 if-else 语句，使代码更加简洁。
```python
a = 5 
b = 10 
max = a if a > b else b  # value_if_true if condition else value_if_false

print(max)
# 10
```
上面的代码通过检查“a”是否大于“b”，如果为真则返回“a”，如果为假则返回“b”。
<a name="nbdKC"></a>
## 2、枚举函数
`enumerate()`函数向可迭代对象添加一个计数器，并以枚举对象的形式返回。当想要遍历列表并跟踪索引时，此函数很有用。
```python
fruits = ['apple', 'banana', 'mango'] 
for index, fruit in enumerate(fruits): 
    print(index, fruit)

# 0 apple
# 1 banana
#2  mango
```
<a name="wzkE6"></a>
## 3、压缩函数
zip()函数聚合来自每个可迭代对象的元素并返回一个元组迭代器。当想同时遍历两个或多个列表时，此函数很有用。
```python
list1 = [1, 2, 3] 
list2 = ['a', 'b', 'c'] 
for x, y in zip(list1, list2):
    print(x, y)

# 1 a
# 2 b
# 3 c
```
<a name="M2YMn"></a>
## 4、列表生成式
列表生成式是一种从现有列表或任何可迭代对象创建列表的简洁方法。这是一种可以替代 for 循环的单行代码，使代码更加高效，并使代码的可读性更强。
```python
squared_numbers = [x**2 for x in range(1, 6)]

print(squared_numbers)
# [1, 4, 9, 16, 25]
```
<a name="L1OrH"></a>
## 5、匿名函数
Lambda 函数是使用lambda关键字定义的匿名函数。当需要编写一次性的小函数并且不想使用关键字def来定义命名函数时，它们很有用。
```python
add = lambda x, y: x + y 

result = add(3, 4)

print(result)
# 7
```
<a name="CoEhD"></a>
## 6、`any()`和`all()`函数
`any()`函数和`all()`函数返回True或False基于 iterable 中元素的真实性。如果 iterable 中的任何元素为真，则函数any()返回True，如果 iterable 中的所有元素都为真，则函数all()返回True。
```python
numbers = [1, 2, 3, 0, 4] 
result = any(numbers) # True 
result = all(numbers) # False。0使结果为False
```
<a name="u2Pcm"></a>
## 7、迭代模块
`itertools`模块提供了一组函数来处理迭代器。该模块中的函数包括`chain`、`product`和`permutations`。
```python
import itertools 
numbers = [1, 2, 3] 
result = list(itertools.permutations(numbers)) 


# 输出所有排列组合 
# [(1, 2, 3), (1, 3, 2), (2, 1, 3), (2, 3, 1), (3, 1, 2), (3, 2, 1)]
```
<a name="aB5PA"></a>
## 8、生成器
生成器是一种可迭代的类型，它可以即时生成值，而不是将它们存储在内存中。它是使用yield关键字定义的，用于创建自定义迭代器。
```python
# 使用yield关键字创建生成器 
def fibonacci_series(n):
    a, b = 0, 1
    for i in range(n):
        yield a
        a, b = b, a + b

# 输出迭代器中的值 
for number in fibonacci_series(10):
    print(number)

# 0
# 1
# 1
# 2
# 3
# 5
# 8
# 13
# 21
# 34
```
<a name="n6MAg"></a>
## 9、装饰器
装饰器是一种修改函数或类行为的方法。使用@符号进行定义，可用于向函数添加功能，例如日志记录、计时或身份验证。
```python
def log_function(func):
    def wrapper(*args, **kwargs):
        print(f'Running {func.__name__}')
        result = func(*args, **kwargs)
        print(f'{func.__name__} returned {result}')
        return result
    return wrapper

@log_function
def add(x, y):
    return x + y


print(add(5,7))

# 运行add函数，返回值为12
```
<a name="AdYEf"></a>
## 10、使用多个函数参数
在 Python 中，可以使用*和**运算符来处理多个函数参数。*运算符用于将参数列表作为单独的位置参数进行传递，运算符**用于传递关键字参数的字典。
```python
def print_arguments(*args, **kwargs):
    print(args)
    print(kwargs)

print_arguments(1, 2, 3, name='John', age=30)

# (1, 2, 3)
# {'name': 'John', 'age': 30}
```
<a name="Cf6YK"></a>
## 11、动态导入
根据用户输入或配置导入模块时，可以使用模块动态导入模块`importlib`。
```python
import importlib

module_name = 'math'
module = importlib.import_module(module_name)
result = module.sqrt(9)
```
<a name="NmZMM"></a>
## 12、字典生成式
字典生成式是一种从现有字典或任何可迭代对象创建字典的简洁方法。它是一种可以替代 for 循环的单行代码，使代码更加高效，代码可读性更强。
```python
squared_numbers = {x: x**2 for x in range(1, 6)}
print(squared_numbers)

# {1: 1, 2: 4, 3: 9, 4: 16, 5: 25}
```
<a name="wJ7Fu"></a>
## 13、可调用对象
在 Python 中，任何可以称为函数的对象都称为可调用对象，包括函数、方法、类，甚至是定义`__call__`方法的对象。
```python
class Adder:
    def __call__(self, x, y):
        return x + y

adder = Adder()
result = adder(3, 4)

print(result)
#7
```
<a name="tR5Qj"></a>
## 14、用下划线分隔大数字/字符
大数字很难一眼看出来是多大，在 Python 中可以用下划线来使数字更易读。
```python
num_test = 100_345_405 # 一个大数字

print(num_test)
# 100345405
```
<a name="S5mqN"></a>
## 15、快速合并两个字典
可以使用以下代码在 Python 中快速合并 2两个字典。
```python
dictionary_one = {"a": 1, "b": 2}
dictionary_two = {"c": 3, "d": 4}

merged = {**dictionary_one, **dictionary_two}

print(merged)  
# {'a': 1, 'b': 2, 'c': 3, 'd': 4}
```
<a name="OB0Km"></a>
## 16、列表、集合和字典是可变的
可变意味着可以更改或更新对象（列表、集合或字典），而无需更改内存中对象的指针。实际效果可见如下示例。<br />在下面的示例中，通过添加一个新城市来更新城市列表，可以看到 ID（对象指针）保持不变，集合和字典也是如此。
```python
cities = ["Munich", "Zurich", "London"]
print(id(cities)) # 2797174365184
cities.append("Berlin")
print(id(cities)) # 2797174365184
```
```python
# 集合 

my_set = {1, 2, 3}
print(id(my_set))  # 2797172976992
my_set.add(4)
print(id(my_set))  # 2797172976992
```
```python
# 字典 

thisdict = {
    "brand": "Ford",
    "model": "Mustang",
    "year": 1964
}
print(id(thisdict))  #2797174128256
thisdict["engine"] = "2500cc"
print(id(thisdict))  #2797174128256
```
