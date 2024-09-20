Python函数式<br />函数式编程到底是什么？本文将详解其概念，同时分享怎样在 Python 中使用函数式编程。主要内容包括列表解析式和其他形式的解析式。
<a name="HmEPt"></a>
## **函数式模型**
在命令式模型中，执行程序的方式是给计算机一系列指令让它执行。执行过程中计算机会改变状态。例如，比如 A 的初始值是 5，后来改变了 A 的值。那么 A 就是个变量，而变量的意思就是包含的值会改变。<br />而在函数式模式中，不需要告诉计算机做什么，而是告诉计算机是什么。比如数字的最大公约数是什么，1 到 n 的乘积是什么等等。<br />因此，变量是不能被改变的。变量一旦被设置，就永远保持同一个值（注意在纯粹的函数式语言中，它们不叫变量）。因此，在函数式模型中，函数没有副作用。副作用就是函数对函数外的世界做出的改变。来看看下面这段Python代码的例子：
```python
a = 3
def some_func():
    global a
    a = 5

some_func()
print(a)
```
代码的输出是 5。在函数式模型中，改变变量的值是完全不允许的，让函数影响函数外的世界也是不允许的。函数唯一能做的就是做一些计算然后返回一个值。<br />可能会想：“没有变量也没有副作用？这有什么好的？”好问题。<br />如果函数使用同样的参数调用两次，那么可以保证它会返回同样的结果。如果学过数学函数，肯定知道这样做的好。这叫做引用透明性（referential transparency）。由于函数没有副作用，那么可以加速计算某个东西的程序。比如，如果程序知道 func(2)返回 3，那么可以将这个值保存在表中，这样就不需要重复运行早已知道结果的函数了。<br />通常，函数式编程不使用循环，而是使用递归。递归是个数学概念，通常的意思是“把结果作为自己的输入”。使用递归函数，函数可以反复调用自己。下面就是个使用Python定义的递归函数的例子：
```python
def factorial_recursive(n):
    # Base case: 1! = 1
    if n == 1:
        return 1

    # Recursive case: n! = n * (n-1)!
    else:
        return n * factorial_recursive(n-1)
```
函数式编程语言也是懒惰的。懒惰的意思是，除非到最后一刻，否则它们不会执行计算或做任何操作。如果代码要求计算2+2，那么函数式程序只有在真正用到计算结果的时候才会去计算。马上就会介绍Python中的这种懒惰。
<a name="oGyQ1"></a>
## **映射**
要理解映射（map），首先需要理解什么是可迭代对象。可迭代对象（iterable）指任何可以迭代的东西。通常是列表或数组，但 Python 还有许多其他可迭代对象。甚至可以自定义对象，通过实现特定的魔术方法使其变成可迭代对象。<br />魔术方法就像 API 一样，能让对象更有 Python 风格。要让对象变成可迭代对象，需要实现以下两个魔术方法：
```python
class Counter:
    def __init__(self, low, high):
        # set class attributes inside the magic method __init__
        # for "inistalise"
        self.current = low
        self.high = high

    def __iter__(self):
        # first magic method to make this object iterable
        return self

    def __next__(self):
        # second magic method
        if self.current > self.high:
            raise StopIteration
        else:
            self.current += 1
            return self.current - 1
```
第一个魔术方法“**iter**”（双下划线iter）返回迭代子，通常在循环开始时调用。__next__则返回迭代的下一个对象。<br />可以打开命令行试一下下面的代码：
```python
for c in Counter(3, 8):
    print(c)
```
这段代码将会输出：
```python
3
4
5
6
7
8
```
在 Python 中，迭代器就是只实现了__iter__魔术方法的对象。也就是说，可以访问对象中都包含的位置，但无法遍历整个对象。一些对象实现了__next__魔术方法，但没有实现__iter__魔术方法，比如集合（本文稍后会讨论）。假设涉及到的一切对象都是可迭代的对象。<br />现在知道了什么是可迭代的对象，回过头来讨论下映射函数。映射可以对可迭代对象中的每个元素执行指定的函数。通常，对列表中的每个元素执行函数，但要知道映射其实可以针对绝大多数可迭代对象使用。
```python
map(function, iterable)
```
假设有一个列表由以下数字组成：
```python
[1, 2, 3, 4, 5]
```
希望得到每个数字的平方，那么代码可以写成这样：
```python
x = [1, 2, 3, 4, 5]
def square(num):
    return num*num

print(list(map(square, x)))
```
Python中的函数式函数是懒惰的。如果不加“list()”，那么函数只会将可迭代对象保存下来，而不会保存结果的列表。需要明确地告诉Python“把它转换成列表”才能得到结果。<br />在Python中一下子从不懒惰的函数求值转换到懒惰的函数似乎有点不适应。但如果能用函数式的思维而不是过程式的思维，那么最终会适应的。<br />这个“square(num)”的确不错，但总觉得有点不对劲。难道为了仅使用一次的map就得定义整个函数吗？其实可以使用lambda函数（匿名函数）。
<a name="ieB8o"></a>
## **Lambda 表达式**
Lambda表达式就是只有一行的函数。比如下面这个lambda表达式可以求出给定数字的平方：
```python
square = lambda x: x * x
```
运行下面的代码：
```python
>>> square(3)
9
```
“参数去哪儿了？这究竟是啥意思？看起来根本不像函数啊？”<br />嗯，的确是不太容易懂……但还是应该能够理解的。上面的代码把什么东西赋给了变量“square”。就是这个东西：
```python
lambda x:
```
它告诉Python这是个lambda函数，输入的名字为x。冒号后面的一切都是对输入的操作，然后它会自动返回操作的结果。<br />这样求平方的代码可以简化成一行：
```python
x = [1, 2, 3, 4, 5]
print(list(map(lambda num: num * num, x)))
```
有了lambda表达式，所有参数都放在左边，操作都放在右边。虽然看上去有点乱，但不能否认它的作用。实际上能写出只有懂得函数式编程的人才能看懂的代码还是有点小兴奋的。而且把函数变成一行也非常酷。
<a name="w8qg6"></a>
## **归纳**
归纳（reduce）是个函数，它把一个可迭代对象变成一个东西。通常在列表上进行计算，将列表归纳成一个数字。归纳的代码看起来长这样：
```python
reduce(function, list)
```
上面的函数可以使用lambda表达式。<br />列表的乘积就是把所有数字乘到一起。可以这样写代码：
```python
product = 1
x = [1, 2, 3, 4]
for num in x:
    product = product * num
```
但使用归纳，可以写成这样：
```python
from functools import reduce

product = reduce((lambda x, y: x * y),[1, 2, 3, 4])
```
这样能得到同样的结果。这段代码更短，而且借助函数式编程，这段代码更简洁。
<a name="UyIu8"></a>
## **过滤**
过滤（filter）函数接收一个可迭代对象，然后过滤掉对象中一切不需要的东西。通常过滤接收一个函数和一个列表。它会针对列表中的每个元素执行函数，如果函数返回True，则什么都不做。如果函数返回False，则从列表中去掉那个元素。<br />语法如下：
```python
filter(function, list)
```
来看一个简单的例子。没有过滤，代码要写成这样：
```python
x = range(-5, 5)
new_list = []

for num in x:
    if num < 0:
        new_list.append(num)
```
使用过滤可以写成这样：
```python
x = range(-5, 5)
all_less_than_zero = list(filter(lambda num: num < 0, x))
```
<a name="B85qk"></a>
## **高阶函数**
高阶函数接收函数作为参数，返回另一个函数。一个非常简单的例子如下所示：
```python
def summation(nums):
    return sum(nums)

def action(func, numbers):
    return func(numbers)

print(action(summation, [1, 2, 3]))

# Output is 6
```
或者更简单“返回函数”的例子：
```python
def rtnBrandon():
    return "brandon"
def rtnJohn():
    return "john"

def rtnPerson():
    age = int(input("What's your age?"))

    if age == 21:
        return rtnBrandon()
    else:
        return rtnJohn()
```
还记得之前说过函数式编程语言没有变量吗？实际上高阶函数能很容易做到这一点。如果只需要在一系列函数中传递数据，那么数据根本不需要保存到变量中。<br />Python 中的所有函数都是顶级对象。顶级对象是拥有一个或多个以下特征的对象：

- 在运行时生成
- 赋值给某个数据结构中的变量或元素
- 作为参数传递给函数
- 作为函数的结果返回

所以，所有 Python 中的函数都是对象，都可以用作高阶函数。
<a name="kgR36"></a>
## **部分函数**
部分函数有点难懂，但非常酷。通过它，不需要提供完整的参数就能调用函数。来看个例子。要创建一个函数，它接收两个参数，一个是底，另一个是指数，然后返回底的指数次幂，代码如下：
```python
def power(base, exponent):
	return base ** exponent
```
现在需要一个求平方的函数，可以这么写：
```python
def square(base):
	return power(base, 2)
```
这段代码没问题，但如果需要立方函数怎么办？或者四次方函数呢？是不是得一直定义新的函数？这样做也行，但是程序员总是很懒的。如果需要经常重复一件事情，那就意味着一定有办法提高速度，避免重复。可以用部分函数实现这一点。下面是使用部分函数求平方的例子：
```python
from functools import partial

square = partial(power, exponent=2)
print(square(2))

# output is 4
```
这是不是很苦？事先告诉 Python 第二个参数，这样只需要提供一个参数就能调用需要两个参数的函数了。<br />还可以使用循环来生成直到能计算 1000 次方的所有函数。
```python
from functools import partial

powers = []
for x in range(2, 1001):
	powers.append(partial(power, exponent = x))

print(powers[0](3))
# output is 9
```
<a name="FQo3g"></a>
## **函数式编程不够 Python**
也许注意到了，这里许多函数式编程都用到了列表。除了归纳和部分函数之外，所有其他函数都生成列表。Guido（Python发明人）不喜欢在 Python 中使用函数式的东西，因为 Python 有自己的方法来生成列表。<br />在 Python IDLE 中敲“import this”，可以看到下面的内容：
```python
>>> import this
The Zen of Python, by Tim Peters
Beautiful is better than ugly.
Explicit is better than implicit.
Simple is better than complex.
Complex is better than complicated.
Flat is better than nested.
Sparse is better than dense.
Readability counts.
Special cases aren’t special enough to break the rules.
Although practicality beats purity.
Errors should never pass silently.
Unless explicitly silenced.
In the face of ambiguity, refuse the temptation to guess.
There should be one — and preferably only one — obvious way to do it.
Although that way may not be obvious at first unless you’re Dutch.
Now is better than never.
Although never is often better than *right* now.
If the implementation is hard to explain, it’s a bad idea.
If the implementation is easy to explain, it may be a good idea.
Namespaces are one honking great idea — let’s do more of those!
```
这就是Python之禅。这首诗表明了什么叫做Python风格。要指出的是这句话：<br />There should be one — and preferably only one — obvious way to do it. （任何事情应该有一个且只有一个方法解决。）<br />在 Python 中，映射和过滤能做到的事情，列表解析式（稍后介绍）也能做到。这就打破了 Python 之禅，因此说函数式编程的部分不够“Python”。<br />另一个常被提及的地方就是lambda。在Python中，lambda函数就是个普通的函数。lambda只是个语法糖。这两者是等价的：
```python
foo = lambda a: 2

def foo(a):
	return 2
```
普通的函数能做到一切 lambda 能做到的事情，但反过来却不行。lambda 不能完成普通函数能完成的一切事情。<br />关于为何函数式编程不适合Python生态系统曾有过一次讨论。也许注意到，之前提到了列表解析式，现在就来介绍下什么是列表解析式。
<a name="bNAsU"></a>
## **列表解析式**
任何能用映射或过滤完成的事情都可以用列表解析式完成。这就是要学的东西。<br />列表解析式是 Python 生成列表的方式。语法如下：
```python
[function for item in iterable]
```
要想求列表中每个数字的平方，可以这么写：
```python
print([x * x for x in [1, 2, 3, 4]])
```
可以看到，给列表中的每个元素应用了一个函数。那么怎样才能实现过滤呢？先来看看之前的这段代码：
```python
x = range(-5, 5)

all_less_than_zero = list(filter(lambda num: num < 0, x))
print(all_less_than_zero)
```
可以将它转换成下面这种使用列表解析式的方式：
```python
x = range(-5, 5)

all_less_than_zero = [num for num in x if num < 0]
```
像这样，列表解析式支持 if 语句。这样就不需要写一堆函数来实现了。实际上，如果需要生成某种列表，那么很有可能使用列表解析式更方便、更简洁。<br />如果想求所有小于 0 的数字的平方呢？使用 Lambda、映射和过滤可以写成：
```python
x = range(-5, 5)

all_less_than_zero = list(map(lambda num: num * num, list(filter(lambda num: num < 0, x))))
```
看上去似乎很长，而且有点复杂。用列表解析式只需写成：
```python
x = range(-5, 5)

all_less_than_zero = [num * num for num in x if num < 0]
```
不过列表解析式只能用于列表。映射和过滤能用于一切可迭代对象。那为什么还要用列表解析式呢？其实，解析式可以用在任何可迭代的对象上。
<a name="nd6JG"></a>
## **其他解析式**
可以在任何可迭代对象上使用解析式。<br />任何可迭代对象都可以用解析式生成。从 Python 2.7 开始，甚至可以用解析式生成字典（哈希表）。
```python
# Taken from page 70 chapter 3 of Fluent Python by Luciano Ramalho

DIAL_CODES = [
    (86, 'China'),
    (91, 'India'),
    (1, 'United States'),
    (62, 'Indonesia'),
    (55, 'Brazil'),
    (92, 'Pakistan'),
    (880, 'Bangladesh'),
    (234, 'Nigeria'),
    (7, 'Russia'),
    (81, 'Japan'),
    ]

>>> country_code = {country: code for code, country in DIAL_CODES}
>>> country_code
{'Brazil': 55, 'Indonesia': 62, 'Pakistan': 92, 'Russia': 7, 'China': 86, 'United States': 1, 'Japan': 81, 'India': 91, 'Nigeria': 234, 'Bangladesh': 880}
>>> {code: country.upper() for country, code in country_code.items() if code < 66}
{1: 'UNITED STATES', 7: 'RUSSIA', 62: 'INDONESIA', 55: 'BRAZIL'}
```
只要是可迭代对象，就可以用解析式生成。来看个集合的例子，简单来说就是：<br />集合是元素的列表，但列表中没有重复的元素<br />元素的顺序不重要
```python
# taken from page 87, chapter 3 of Fluent Python by Luciano Ramalho

>>> from unicodedata import name
>>> {chr(i) for i in range(32, 256) if 'SIGN' in name(chr(i), '')}
{'×', '¥', '°', '£', '©', '#', '¬', '%', 'µ', '>', '¤', '±', '¶', '§', '<', '=', '®', '$', '÷', '¢', '+'}
```
可以看到，集合使用字典同样的大括号。Python非常聪明。它会查看是否在大括号中提供了额外的值，来判断是集合解析式还是字典解析式。
<a name="hIZEv"></a>
## **结论**
函数式编程很美、很纯净。函数式代码可以写得非常干净，但也可以写得很乱。一些 Python 程序员不喜欢在 Python 中使用函数式的模型，不过大家可以根据自己的喜好，记得用最好的工具完成工作。
