Python 高阶函数
<a name="jKZtj"></a>
## 高阶函数的定义
变量可以指向函数，函数的参数能接收变量，那么一个函数就可以接收另一个函数作为参数，这种函数就称之为高阶函数。比如:
```python
def add(x,y,f):
   return f(x) + f(y)
op = add(1,1,abs)  #这里把python内置函数abs作为参数传递给add
print (op)
#2
```
内置函数：一般都是因为使用频率比较频繁或是是元操作，所以通过内置函数的形式提供出来，通过对python的内置函数分类分析可以看出来：基本的数据操 作基本都是一些数学运算（当然除了加减乘除）、逻辑操作、集合操作、基本IO操作，然后就是对于语言自身的反射操作，还有就是字符串操作，也是比较常用 的，尤其需要注意的是反射操作。
> 比如常用的常用的`abs`、`round`等

<a name="rI8Wq"></a>
## 内置高阶函数
<a name="t0RRu"></a>
### 1、Lambda 函数
Lambda 函数是一种比较小的匿名函数——匿名是指它实际上没有函数名。<br />Python 函数通常使用 `def a_function_name()` 样式来定义，但对于 lambda 函数，根本没为它命名。这是因为 lambda 函数的功能是执行某种简单的表达式或运算，而无需完全定义函数。<br />lambda 函数可以使用任意数量的参数，但表达式只能有一个。
```python
x = lambda a, b : a * b
print(x(5, 6)) # prints '30'
x = lambda a : a*3 + 3
print(x(3)) # prints '12'
```
执行了一些简单的数学运算，而无需定义整个函数。这是 Python 的众多特征之一。
<a name="FiwsM"></a>
### 2、`filter`函数
`filter()` 函数用于过滤序列，过滤掉不符合条件的元素，返回由符合条件元素组成的新列表。该接收两个参数，第一个为函数，第二个为序列，序列的每个元素作为参数传递给函数进行判，然后返回 True 或 False，最后将返回 True 的元素放到新列表中。
> - 语法：`filter(function, iterable)`
> - 参数：function---- 判断函数，iterable -- 可迭代对象
> - 返回值：返回列表

**实例如下**
```python
def is_odd(n):
   return n % 2 == 1
alist = filter(is_odd, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
print(list(alist))
#[1, 3, 5, 7, 9]
```
<a name="59pVp"></a>
### 3、map函数
`map()` 是一种内置的 Python 函数，它可以将函数应用于各种数据结构中的元素，如列表或字典。对于这种运算来说，这是一种非常干净而且可读的执行方式。<br />接收一个函数 f 和一个或多个序列list，并通过把函数 f 依次作用在 序列list 的每个元素上，得到一个新的 list 并返回。
> - 语法：`map(function, iterable, ...)`
> - 参数：function -- 函数，iterable -- 一个或多个序列
> - 返回值：Python 2.x 返回列表/Python 3.x 返回迭代器

**实例如下**
```python
list_=[1,2,3,4,5]
def square(x) :            # 计算平方数
    return x ** 2
list_2=list(map(square,list_)) # 计算列表各个元素的平方
print(list_2)
#[1, 4, 9, 16, 25]
```
**使用 lambda 匿名函数**
```python
list_3=list(map(lambda x: x ** 2, list_)) # 使用 lambda 匿名函数
print(list_3)
[1, 4, 9, 16, 25]
```
**实例2**
```python
# 提供了两个列表，对相同位置的列表数据进行相加
list_4=list(map(lambda x, y: x + y, [1, 3, 5, 7, 9], [2, 4, 6, 8, 10]))
print(list_4)
#[3, 7, 11, 15, 19]
```
<a name="gZKlO"></a>
### 4、sorted函数

- 根据 iterable 中的项返回一个新的已排序列表。
- 具有两个可选参数，它们都必须指定为关键字参数。
   - key：指定带有单个参数的函数，用于从 iterable 的每个元素中提取用于比较的键 (例如 `key=str.lower`)。默认值为 None (直接比较元素)        
   - reverse：为一个布尔值，如果设为 True，则每个列表元素将按反向顺序比较进行排序
```python
# 学号 姓名 成绩
d = {'001': ['张三', 88], '002': ['李四', 90], '003': ['王五', 73]}
# 按成绩排序  降序  由高到低
d1 = list(sorted(d.items(), key=lambda x: x[1][1], reverse=True))      
print('学号 姓名 成绩')
for item in d1:
    print(item[0], item[1][0], item[1][1])
```
运行结果如下：
```python
学号 姓名 成绩
002 李四 90
001 张三 88
003 王五 73
```
输入一组数到列表nums，请找到列表中任意两个元素相加能够等于9的元素，形成一个元组。<br />使其小数在前大数在后，如：(2,7),(1,8)。重复的元组元素只保留一个，结果按元组第一个元素从大到小顺序输出
```python
# 输入一组数到列表nums，请找到列表中任意两个元素相加能够等于9的元素，形成一个元组
# 使其小数在前大数在后，如：(2,7),(1,8)。重复的元组元素只保留一个，结果按元组第一个元素从大到小顺序输出
def get_tuple(num_list):
    temp_list = []
    for i in num_list:
        if (9 - i) in num_list:
            min_ = (9 - i) if (i >= (9 - i)) else i
            max_ = i if min_ == (9 - i) else (9 - i)
            if (min_, max_) not in temp_list:
                temp_list.append((min_, max_))
    return temp_list
nums = input("numbers：")
# 列表推导式
num_list = [int(i) for i in nums.split(',')]
result_list = get_tuple(num_list)
# 按列表里每个元组的第一个元素从大到小排序  降序
result_list = sorted(result_list, key=lambda x: x[0], reverse=True)
print(result_list)
```
运行结果如下：
```python
numbers：3,4,5,7,2,8,1,6,9,0
[(4, 5), (3, 6), (2, 7), (1, 8), (0, 9)]
```
<a name="hzWmQ"></a>
### 5、reduce函数
`reduce()`函数接收的参数和 `map()`类似，但是行为不同。`reduce()` 函数会对参数序列中元素进行累积。`reduce()`传入的函数 f 必须接收两个参数,用传给 reduce 中的函数 f（有两个参数）先对集合中的第 1、2 个元素进行操作，得到的结果再与第三个数据用 f 函数运算，最后得到一个结果。
> - 语法：`reduce(function, iterable[, initializer])`
> - 参数：function -- 函数，有两个参数。iterable -- 可迭代对象。initializer -- 可选，初始参数

**实例如下**
```python
from functools import reduce   #自 Python3 之后，这个函数从全局命名空间中移除，放在了 functools模块，因为如果想正确执行，必须这样
def add(x,y):   #两数想家
   return x+y
reduce(add, [1,2,3,4,5])
#15
>>> reduce(lambda x, y: x+y, [1,2,3,4,5])  # 使用 lambda 匿名函数
#15
def f(a,b):
   return a+b
print(reduce(f,list_,10))  #这里的第三个参数是做为初始值的
#25
```
<a name="3YINx"></a>
### 6、Itertools 模块
Python 的 Itertools 模块是处理迭代器的工具集合。迭代器是一种可以在 for 循环语句（包括列表、元组和字典）中使用的数据类型。<br />使用 Itertools 模块中的函数可以执行很多迭代器操作，这些操作通常需要多行函数和复杂的列表理解。关于 Itertools 的神奇之处，请看以下示例：
```python
from itertools import *
# Easy joining of two lists into a list of tuples
for i in izip([1, 2, 3], ['a', 'b', 'c']):
    print i
# ('a', 1)
# ('b', 2)
# ('c', 3)
# The count() function returns an interator that 
# produces consecutive integers, forever. This 
# one is great for adding indices next to your list 
# elements for readability and convenience
for i in izip(count(1), ['Bob', 'Emily', 'Joe']):
    print i
# (1, 'Bob')
# (2, 'Emily')
# (3, 'Joe')    
# The dropwhile() function returns an iterator that returns 
# all the elements of the input which come after a certain 
# condition becomes false for the first time. 
def check_for_drop(x):
    print 'Checking: ', x
    return (x > 5)
for i in dropwhile(should_drop, [2, 4, 6, 8, 10, 12]):
    print 'Result: ', i
# Checking: 2
# Checking: 4
# Result: 6
# Result: 8
# Result: 10
# Result: 12
# The groupby() function is great for retrieving bunches
# of iterator elements which are the same or have similar 
# properties
a = sorted([1, 2, 1, 3, 2, 1, 2, 3, 4, 5])
for key, value in groupby(a):
    print(key, value), end=' ')
# (1, [1, 1, 1])
# (2, [2, 2, 2]) 
# (3, [3, 3]) 
# (4, [4]) 
# (5, [5])
```
<a name="evySQ"></a>
### 7、Generator 函数
Generator 函数是一个类似迭代器的函数，即它也可以用在 for 循环语句中。这大大简化了代码编写，而且相比简单的 for 循环，它节省了很多内存。<br />比如把 1 到 1000 的所有数字相加，以下代码块的第一部分展示了如何使用 for 循环来进行这一计算。<br />如果列表很小，比如 1000 行，计算所需的内存还行。但如果列表巨长，比如十亿浮点数，这样做就会出现问题了。使用这种 for 循环，内存中将出现大量列表，但不是每个人都有无限的 RAM 来存储这么多东西的。Python 中的 `range()` 函数也是这么干的，它在内存中构建列表。<br />代码中第二部分展示了使用 Python generator 函数对数字列表求和。generator 函数创建元素，并只在必要时将其存储在内存中，即一次一个。这意味着，如果要创建十亿浮点数，只能一次一个地把它们存储在内存中！Python 2.x 中的 `xrange()` 函数就是使用 generator 来构建列表。<br />上述例子说明：如果想为一个很大的范围生成列表，那么就需要使用 generator 函数。如果内存有限，比如使用移动设备或边缘计算，使用这一方法尤其重要。<br />也就是说，如果想对列表进行多次迭代，并且它足够小，可以放进内存，那最好使用 for 循环或 Python 2.x 中的 range 函数。因为 generator 函数和 xrange 函数将会在每次访问它们时生成新的列表值，而 Python 2.x range 函数是静态的列表，而且整数已经置于内存中，以便快速访问。
```python
# (1) Using a for loopv
numbers = list()
for i in range(1000):
    numbers.append(i+1)
total = sum(numbers)
# (2) Using a generator
 def generate_numbers(n):
     num, numbers = 1, []
     while num < n:
           numbers.append(num)
     num += 1
     return numbers
 total = sum(generate_numbers(1000))
 # (3) range() vs xrange()
 total = sum(range(1000 + 1))
 total = sum(xrange(1000 + 1))
```

