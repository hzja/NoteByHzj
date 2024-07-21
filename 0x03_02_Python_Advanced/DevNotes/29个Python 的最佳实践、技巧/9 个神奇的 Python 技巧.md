Python
<a name="hlgfZ"></a>
### 1、使用 `product()` 函数避免嵌套循环
当程序变得复杂时，不可避免地要编写嵌套循环。然而，嵌套循环会使程序更难阅读和维护。<br />幸运的是，可以通过内置的 `product()` 函数避免 Python 中的嵌套循环。<br />例如，有一个程序如下，其中包含 3 级嵌套的 for 循环：
```python
list_a = [1, 2020, 70]
list_b = [2, 4, 7, 2000]
list_c = [3, 70, 7]

for a in list_a:
    for b in list_b:
        for c in list_c:
            if a + b + c == 2077:
                print(a, b, c)
# 70 2000 7
```
为了使它更整洁可以使用 itertools 模块中的 `product()` 函数来优化代码：
```python
from itertools import product

list_a = [1, 2020, 70]
list_b = [2, 4, 7, 2000]
list_c = [3, 70, 7]

for a, b, c in product(list_a, list_b, list_c):
    if a + b + c == 2077:
        print(a, b, c)
# 70 2000 7
```
<a name="KTmoD"></a>
### 2、海象运算符：一个可爱的赋值表达式技巧
从 Python 3.8 开始，有一种称为“海象运算符”的新语法，它可以将值赋给变量作为更大表达式的一部分。<br />这种语法非常容易理解。比如想把下面两行 Python 代码写在一行中，怎么写呢？
```python
author = "Yang"
print(author)
# Yang
```
不幸的是，不能直接将赋值放入 `print()` 函数中。如果尝试，将会出现 TypeError：
```python
print(author="Yang")
# TypeError: 'author' is an invalid keyword argument for print()
```
多亏了 walrus 运算符，真的可以在一行中做到这一点：
```python
print(author:="Yang")
# Yang
```
<a name="u9Msl"></a>
### 3、三元条件运算符：在一行中编写一个简单的 if-else 结构
if-else 条件在编程世界中无处不在。为了让简单的逻辑易于表达，Python 提供了三元运算符。简而言之，它只允许将 if-else 条件放在一行中：
```python
min = a if a < b else b
```
显然，上面的代码比下面的要简洁得多：
```python
if a<b:
  min = a
else:
  min = b
```
<a name="hqZiZ"></a>
### 4、使用 Lambda 函数定义简单函数
如果只想定义一个简单的函数，可能不需要为它使用传统的语法。lambda 函数是一个更优雅的选择。<br />例如，下面的函数是计算斐波那契数列：
```python
def fib(x):
    if x<=1:
        return x
    else:
        return fib(x-1) + fib(x-2)
```
它工作得很好，但代码本身有点难看。写一个单行代码来实现相同的功能：
```python
fib = lambda x: x if x <= 1 else fib(x - 1) + fib(x - 2)
```
<a name="IclYI"></a>
### 5、列表理解：以 Pythonic 方式获取列表
说列表理解使代码优雅仍然是轻描淡写的说法。它可以节省大量的输入和时间，但仍然保持代码可读性。很少有编程语言可以做到这一点。
```python
Genius = ["Jerry", "Jack", "tom", "yang"]
L1 = [name if name.startswith('y') else 'Not Genius' for name in Genius]
print(L1)
# ['Not Genius', 'Not Genius', 'Not Genius', 'yang']
```
随意享受一下上面优雅的程序，想想如果没有列表理解技巧，需要写多少行代码。
<a name="bRj3X"></a>
### 6、利用 Python 中的高阶函数
Python 内置了一些高阶函数，方便编写一些通用逻辑。<br />例如，`map()` 函数是一个著名且经常使用的高阶函数。它接收两个参数，一个是函数，另一个是可迭代对象。执行 map 函数会将函数应用于可迭代对象的每个元素。
```python
names = ['yAnG', 'MASk', 'thoMas', 'LISA']
names = map(str.capitalize, names)
print(list(names))
# ['Yang', 'Mask', 'Thomas', 'Lisa']
```
如上例所示，借助 `map()` 函数，可以避免编写 for 循环以将名称列表中的每个单词大写。<br />另一个著名的高阶函数是 `reduce()`。顾名思义，它将一个函数应用到一个可迭代对象中，并为其执行累积操作。<br />例如，以下示例将列表转换为一个字符串：
```python
from functools import reduce

city = ['L', 'o', 'n', 'd', 'o', 'n', 2, 0, 2, 0]
city_to_str = reduce(lambda x, y: str(x) + str(y), city)
print(city_to_str)
# London2020
```
<a name="aGClo"></a>
### 7、联合运算符：合并字典的最简单方法
合并字典是日常 Python 编程中的常见需求。有很多方法可以做到这一点，但在 Python 3.9 之前，它们都很丑陋。<br />从 Python 3.9 开始，终于找到了最优雅的字典合并方式——使用联合运算符。
```python
cities_us = {'New York City': 'US', 'Los Angeles': 'US'}
cities_uk = {'London': 'UK', 'Birmingham': 'UK'}

cities = cities_us|cities_uk
print(cities)
# {'New York City': 'US', 'Los Angeles': 'US', 'London': 'UK', 'Birmingham': 'UK'}
```
如上例所示，可以简单地使用 | 运算符合并两个不同的词典。更重要的是，它还支持就地合并：
```python
cities_us = {'New York City': 'US', 'Los Angeles': 'US'}
cities_uk = {'London': 'UK', 'Birmingham': 'UK'}

cities_us |= cities_uk
print(cities_us)
# {'New York City': 'US', 'Los Angeles': 'US', 'London': 'UK', 'Birmingham': 'UK'}
```
<a name="uJwHC"></a>
### 8、F-Strings：Pythonic 字符串格式化技术
几乎每种编程语言都支持字符串格式化语法。但并不是每一个都像 Python 的 f 字符串技术那样优雅。
```python
pi = 3.1415926
print(f'Pi is approximately equal to {pi:.2f}')
# Pi is approximately equal to 3.14

id = 1  # need to print a 3-digit number
print(f"The id is {id:03d}")
# The id is 001

N = 1000000000  # need to add separator
print(f'His networth is ${N:,d}')
# His networth is $1,000,000,000
```
如上面的程序所示，使用 f-string 技巧，可以应用 Python 变量并在 f-string 中定义其格式规范。<br />还记得 C 编程语言的字符串格式化语法吗？是否同意 Python 的 f-string 语法更简单？<br />让 f-string 技术更令人惊叹的是可以在 f-strings 中嵌入表达式。嵌入表达式将在运行时进行评估。<br />以下示例将在 f 字符串的帮助下打印今天的时间：
```python
from datetime import datetime

print(f"Today is {datetime.today()}")
# Today is 2021-07-31 18:20:48.956829
```
<a name="hfcET"></a>
### 9、使用星号
如何将列表、元组和集合合并到一个列表中？<br />最优雅的方式是使用星号：
```python
A = [1, 2, 3]
B = (4, 5, 6)
C = {7, 8, 9}
L = [*A, *B, *C]
print(L)
# [1, 2, 3, 4, 5, 6, 8, 9, 7]
```
如上所述，星号可以用作可迭代对象的前缀来解包它们的项目。<br />除了解包 iterables 之外，星号还可以用于 Python 中的解构赋值：
```python
a, *mid, b = [1, 2, 3, 4, 5, 6]
print(a, mid, b)
# 1 [2, 3, 4, 5] 6
```
如上所示，在一个星号的帮助下，mid 变量接收中间的项目作为列表。
