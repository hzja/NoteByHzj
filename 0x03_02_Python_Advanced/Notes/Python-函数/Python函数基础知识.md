Python 函数<br />函数是Python中最重要、最基础的代码组织和代码复用方式。根据经验，如果需要多次重复相同或类似的代码，就非常值得写一个可复用的函数。通过给一组Python语句一个函数名，形成的函数可以使代码更加可读。
函数声明时使用`def`关键字，返回时使用`return`关键字：
```python
def my_function(x, y, z=1.5):  
if z > 1:  
    return z * (x + y)  
else:  
    return z / (x + y)
```
有多条返回语句是没有问题的。如果Python达到函数的尾部时仍然没有遇到`return`语句，就会自动返回`None`。<br />每个函数都可以有位置参数和关键字参数。关键字参数最常用于指定默认值或可选参数。在前面的函数中，x和y是位置参数，z是关键字参数。这意味着函数可以通过以下任意一种方式进行调用：
```python
my_function(5, 6, z=0.7)  
my_function(3.14, 7, 3.5)  
my_function(10, 20)
```
函数参数的主要限制是关键字参数必须跟在位置参数后(如果有的话)。可以按照任意顺序指定关键字参数；这样可以不必强行记住函数参数的顺序，而只需用参数名指定。<br />也可以使用关键字参数向位置参数传参。在前面的例子中，也可以这样写：
```python
my_function(x=5, y=6, z=7)
my_function(y=6, x=5, z=7)
```
在部分场景中，这样做有助于代码可读性
<a name="AP3nW"></a>
## 1、命名空间、作用域和本地函数
函数有两种连接变量的方式：全局、本地。在Python中另一种更贴切地描述变量作用域的名称是命名空间。在函数内部，任意变量都是默认分配到本地命名空间的。本地命名空间是在函数被调用时生成的，并立即由函数的参数填充。当函数执行结束后，本地命名空间就会被销毁(除了一些特殊情况)。考虑以下函数：
```python
def func():  
    a = []  
    for i in range(5):  
        a.append(i)
```
当`func()`调用时，空的列表会被创建，五个元素被添加到列表，之后a会在函数退出时被销毁。假设像下面这样声明a：
```python
a = []  
def func():  
    for i in range(5):  
        a.append(i)
```
在函数外部给变量赋值是可以的，但是那变量必须使用`global`关键字声明为全局变量：
```python
In [168]: a = None  
In [169]: def bind_a_variable():  
.....:        global a  
.....:        a = []  
.....:        bind_a_variable()  
.....:  
In [170]: print(a)  
[]
```
global关键字的用法：通常全局变量用来存储系统中的某些状态。如果大量使用了全局变量，可能需要面向对象编程(使用类)
<a name="7zSbx"></a>
## 2、返回多个值
示例代码：
```python
def f():  
    a = 5  
    b = 6  
    c = 7  
    return a, b, c  
a, b, c = f()
```
在数据分析和其他科研应用，可能会发现经常需要返回多个值。这里实质上是返回了一个对象，也就是元组，而元组之后又被拆包为多个结果变量。在前面的例子中，可以用下面的代码代替：
```python
return_value = f()
```
在这个例子中，return_value是一个3个元素的元组。像之前那样一次返回多个值还有一种潜在的、更有吸引力的实现：
```python
def f():  
    a = 5  
    b = 6  
    c = 7  
    return {'a' : a, 'b' : b, 'c' : c}
```
具体用哪种技术取决于具体业务。
<a name="899fb7ab"></a>
## 3、函数是对象
由于Python的函数是对象，很多在其他语言中比较难的构造在Python中非常容易实现。假设正在做数据清洗，需要将一些变形应用到下列字符串列表中:
```python
In [171]: states = [' Alabama ', 'Georgia!', 'Georgia', 'georgia', 'FlOrIda',  
.....: 'south carolina##', 'West virginia?']
```
任何处理过用户提交数据的人都对这样的数据感到凌乱。为了使这些数据整齐、可用于分析，有很多是事情需要去做：去除空格、移除标点符号、调整适当的大小写。一种方式是使用内建的字符串方法，结合标准库中的正则表达式模块re：
```python
import re  
def clean_strings(strings):  
    result = []  
    for value in strings:  
        value = value.strip()  
        value = re.sub('[!#?]', '', value)  
        value = value.title()  
        result.append(value)  
    return result
```
结果如下：
```python
In [173]: clean_strings(states)  
Out[173]:  
['Alabama',  
'Georgia',  
'Georgia',  
'Georgia',  
'Florida',  
'South Carolina',  
'West Virginia']
```
另一种有用的实现就是将特定的列表操作应用到某个字符串的集合上：
```python
def remove_punctuation(value):  
	return re.sub('[!#?]', '', value)  
clean_ops = [str.strip, remove_punctuation, str.title]  
def clean_strings(strings, ops):  
    result = []  
    for value in strings:  
        for function in ops:  
            value = function(value)  
        result.append(value)  
    return result
```
结果如下：
```python
In [175]: clean_strings(states, clean_ops)  
Out[175]:  
['Alabama',  
'Georgia',  
'Georgia',  
'Georgia',  
'Florida',  
'South Carolina',  
'West Virginia']
```
像这种更为函数化的模式可以在更高层次上方便地修改字符串变换方法。clean_strings函数现在也具有更强的复用性、通用性。<br />可以将函数作为一个参数传给其他的函数，比如内建的`map`函数，可以将一个函数应用到一个序列上：
```python
In [176]: for x in map(remove_punctuation, states):  
.....:        print(x)  
Alabama  
Georgia  
Georgia  
georgia  
FlOrIda  
south carolina  
West virginia
```
<a name="e7497dbd"></a>
## 4、匿名(Lambda)函数
Python支持所谓的匿名或lambda函数。匿名函数是一种通过单个语句生成函数的方式，其结果是返回值。匿名函数使用`lambda`关键字定义，该关键字仅表达“声明一个匿名函数”的意思：
```python
def short_function(x):  
    return x * 2  
equiv_anon = lambda x: x * 2
```
匿名函数在数据分析中非常方便，因为在很多案例中数据变形函数都可以作为函数的参数。匿名函数代码量小(也更为清晰)，将它作为参数进行传值，比写一个完整的函数或者将匿名函数赋值给局部变量更好。举个例子，考虑下面的不佳示例：
```python
def apply_to_list(some_list, f):  
    return [f(x) for x in some_list]  
ints = [4, 0, 1, 5, 6]  
apply_to_list(ints, lambda x: x * 2)
```
也可以写成`[x * 2 for x in ints]` ，但是在这里能够简单地将一个自定义操作符传递给apply_to_list函数。<br />另一个例子，假设想要根据字符串中不同字母的数量对一个字符串集合进行排序：
```python
In [177]: strings = ['foo', 'card', 'bar', 'aaaa', 'abab']
```
这里可以将一个匿名函数传给列表的`sort`方法：
```python
In [178]: strings.sort(key=lambda x: len(set(list(x))))  
In [179]: strings  
Out[179]: ['aaaa', 'foo', 'abab', 'bar', 'card']
```
和`def`关键字声明的函数不同，匿名函数对象自身并没有一个显式的`__name__ `属性，这是`lambda`函数被称为匿名函数的一个原因。
<a name="SVczd"></a>
## 5、柯里化：部分函数应用
柯里化是计算机科学术语(以数学家Haskell Curry命名)，它表示通过部分参数应用的方式从已有的函数中衍生出新的函数。例如，假设有一个不重要的函数，其功能是将两个数加一起：
```python
def add_numbers(x, y):  
    return x + y
```
使用这个函数，可以衍生出一个只有一个变量的新函数，add_five，可以给参数加上5：
```python
add_five = lambda y: add_numbers(5, y)
```
第二个参数对于函数add_numers就是柯里化了。这里并没有什么神奇的地方，真正做的事只是定义了一个新函数，这个新函数调用了已经存在的函数。内建的`functools`模块可以使用`pratial`函数简化这种处理：
```python
from functools import partial  
add_five = partial(add_numbers, 5)
```
<a name="3Wbnn"></a>
## 6、生成器
通过一致的方式遍历序列，例如列表中的对象或者文件中的一行行内容，这是Python的一个重要特性。这个特性是通过迭代器协议来实现的，迭代器协议是一种令对象可遍历的通用方式。例如，遍历一个字典，获得字典的键：
```python
In [180]: some_dict = {'a': 1, 'b': 2, 'c': 3}  
In [181]: for key in some_dict:  
.....:        print(key)  
a   
b  
c
```
当写下`for key in some_dict` 的语句时，Python解释器首先尝试根据some_dict生成一个迭代器：
```python
In [182]: dict_iterator = iter(some_dict)  
In [183]: dict_iterator  
Out[183]: <dict_keyiterator at 0x7fbbd5a9f908>
```
迭代器就是一种用于在上下文中(比如for循环)向Python解释器生成对象的对象。大部分以列表或列表型对象为参数的方法都可以接收任意的迭代器对象。包括内建方法比如`min`、`max`和s`um`，以及类型构造函数比如`list`和`tuple`：
```python
In [184]: list(dict_iterator)  
Out[184]: ['a', 'b', 'c']
```
用迭代器构造新的可遍历对象是一种非常简洁的方式。普通函数执行并一次返回单个结果，而生成器则“惰性”地返回一个多结果序列，在每一个元素产生之后暂停，直到下一个请求。如需创建一个生成器，只需要在函数中将返回关键字`return`替换为`yield`关键字：
```python
def squares(n=10):  
    print('Generating squares from 1 to {0}'.format(n ** 2))  
    for i in range(1, n + 1):  
        yield i ** 2
```
当实际调用生成器时，代码并不会立即执行：
```python
In [186]: gen = squares()  
In [187]: gen  
Out[187]: <generator object squares at 0x7fbbd5ab4570>
```
直到请求生成器中的元素时，它才会执行它的代码：
```python
In [188]: for x in gen:  
.....: print(x, end=' ')  
Generating squares from 1 to 100  
1 4 9 16 25 36 49 64 81 100
```
<a name="0NF91"></a>
### 6.1 生成器表达式
生成器表达式来创建生成器更为简单。生成器表达式与列表、字典、集合的推导式很类似，创建一个生成器表达式，只需要将列表推导式的中括号替换为小括号即可：
```python
In [189]: gen = (x ** 2 for x in range(100))  
In [190]: gen  
Out[190]: <generator object <genexpr> at 0x7fbbd5ab29e8>
```
上面的代码与下面更为复杂的生成器是等价的：
```python
def _make_gen():  
    for x in range(100):  
        yield x ** 2  
        gen = _make_gen()
```
在很多情况下，生成器表达式可以作为函数参数用于替代列表推导式：
```python
In [191]: sum(x ** 2 for x in range(100))  
Out[191]: 328350  
In [192]: dict((i, i **2) for i in range(5))  
Out[192]: {0: 0, 1: 1, 2: 4, 3: 9, 4: 16}
```
<a name="LnQhJ"></a>
### 6.2 `itertools`模块
标准库中的`itertools`模块是适用于大多数数据算法的生成器集合。例如，`groupby`可以根据任意的序列和一个函数，通过函数的返回值对序列中连续的元素进行分组，参见下面的例子：
```python
In [193]: import itertools  
In [194]: first_letter = lambda x: x[0]  
In [195]: names = ['Alan', 'Adam', 'Wes', 'Will', 'Albert', 'Steven']  
In [196]: for letter, names in itertools.groupby(names, first_letter):  
.....:        print(letter, list(names)) # names是一个生成器  
A ['Alan', 'Adam']  
W ['Wes', 'Will']  
A ['Albert']  
S ['Steven']
```
下表是一些经常用到的`itertools`函数的列表。可以通过查询Python官方文档来获得更多关于内建工具库的信息。<br />

| 函数 | 描述 |
| :--- | :--- |
| `combinations(iterable,  k)` | 根据iterable参数中的所有元素生成一个包含所有可能K元组的序列，忽略元素的顺序，也不进行替代(需要替代请参考函数 combinations_with_replacement ) |
| `permutations(iterable,  k)` | 根据itrable参数中的按顺序生成包含所有可能K元组的序列 |
| `groupby(iterable[,  keyfunc])` | 根据每一个独一的Key生成 (key, sub-iterator) 元组 |
| `product(*iterables,  repeat=1)` | 以元组的形式，根据输入的可遍历对象们生成笛卡尔积，与嵌套的for循环类似 |

<a name="0o9Jt"></a>
## 7、错误和异常处理
优雅地处理Python的错误或异常是构建稳定程序的重要组成部分。在数据分析应用中，很多函数只能处理特定的输入。例如，Python的float函数可以将字符串转换为浮点数字，但是对不正确的输入会产生ValueError:
```python
In [197]: float('1.2345')  
Out[197]: 1.2345  
In [198]: float('something')  
---------------------------------------------------------------------------  
ValueError Traceback (most recent call last)  
<ipython-input-198-439904410854> in <module>()  
----> 1 float('something')  
ValueError: could not convert string to float: 'something'
```
假设想要在`float`函数运行失败时可以优雅地返回输入参数。可以通过将`float`函数写入一个`try/except`代码段来实现：
```python
def attempt_float(x):  
    try:  
        return float(x)  
    except:  
        return x
```
如果`float(x)`执行时抛出了异常，则代码段中的`except`部分代码将会被执行：
```python
In [200]: attempt_float('1.2345')  
Out[200]: 1.2345  
In [201]: attempt_float('something')  
Out[201]: 'something
```
除了ValueError，`float`函数还会抛出其他的异常：
```python
In [202]: float((1, 2))  
---------------------------------------------------------------------------  
TypeError Traceback (most recent call last)  
<ipython-input-202-842079ebb635> in <module>()  
----> 1 float((1, 2))  
TypeError: float() argument must be a string or a number, not 'tuple'
```
可能只想处理ValueError，因为TypeError(输入的不是字符串或数值)可能表明程序中有个合乎语法的错误。为了实现这个目的，在except后面写下异常类型：
```python
def attempt_float(x):  
    try:  
        return float(x)  
    except ValueError:  
        return x
```
然后可以得到：
```python
In [204]: attempt_float((1, 2))  
---------------------------------------------------------------------------  
TypeError Traceback (most recent call last)  
<ipython-input-204-9bdfd730cead> in <module>()  
----> 1 attempt_float((1, 2))  
<ipython-input-203-3e06b8379b6b> in attempt_float(x)  
1 def attempt_float(x):  
2 try:  
----> 3 return float(x)  
4 except ValueError:  
5 return x  
TypeError: float() argument must be a string or a number, not 'tuple'
```
可以通过将多个异常类型写成元组的方式同事捕获多个异常(小括号是必不可少的)：
```python
def attempt_float(x):  
    try:  
        return float(x)  
    except (TypeError, ValueError):  
        return x
```
某些情况下，可能想要处理一个异常，但是希望一部分代码无论try代码块是否报错都要执行。为了实现这个目的，使用`finally`关键字：
```python
f = open(path, 'w')  
try:  
    write_to_file(f)  
finally:  
    f.close()
```
这样，可以让`f`在程序结束后总是关闭。类似的，可以使用`else`来执行当`try`代码块成功执行时才会执行的代码：
```python
f = open(path, 'w')  
try:  
    write_to_file(f)  
except:  
    print('Failed')  
else:  
    print('Succeeded')  
finally:  
    f.close()
```
<a name="3dkFu"></a>
### IPython中的异常
如果正在`%run`一个脚本或执行任何语句报错时，IPython将会默认打印出完整的调用堆栈跟踪(报错追溯)，会将堆栈中每个错误点附近的几行上下文代码打印出：
```python
In [10]: %run examples/ipython_bug.py  
---------------------------------------------------------------------------  
AssertionError Traceback (most recent call last)  
/home/wesm/code/pydata-book/examples/ipython_bug.py in <module>()  
13 throws_an_exception()  
14  
---> 15 calling_things()  
/home/wesm/code/pydata-book/examples/ipython_bug.py in calling_things()  
11 def calling_things():  
12 works_fine()  
---> 13 throws_an_exception()  
14  
15 calling_things()  
/home/wesm/code/pydata-book/examples/ipython_bug.py in throws_an_exception()  
7 a = 5  
8 b = 6  
----> 9 assert(a + b == 10)  
10  
11 def calling_things():  
AssertionError:
```
比标准Python解释器提供更多额外的上下文是IPython的一大进步(标准Python解释器不提供任何额外的上下文)。可以使用`%xmode`命令来控制上下文的数量，可以从`Plain`(普通)模式(与标准Python解释器一致)切换到`Verbose`(复杂)模式(可以显示函数的参数值以及更多有用信息)。
