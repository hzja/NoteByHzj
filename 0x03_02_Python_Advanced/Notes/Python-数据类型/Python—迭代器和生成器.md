Python 迭代器 生成器
<a name="BBSCt"></a>
## 1、迭代器
<a name="KAoJG"></a>
### 1.1 容器
首先，在了解迭代器之前，需要知道什么是容器。<br />一切都是对象，对象的抽象就是类，而对象的集合就是容器。容器，就是有多个对象组成的东西。<br />比如：列表`[0,1,2]`，元组(1,2,3),字典`{'0:'0','1':"1'}`  集合`{1,2,3}`都是容器。所有的容器都是可迭代对象，也就是可以遍历元素。<br />可迭代对象：可以被转化为不依赖索引取值的容器，这样的对象就叫做可迭代对象，可以通过 `__iter__()` 来生成不依赖索引取值的容器。<br />现在写一个is_iterable判断是不是迭代器
```python
def is_iterable(param):
    try:
        iter(param)
        return True
    except TypeError:
        return False
params = [
    1234,
    '1234',
    [1, 2, 3, 4],
    set([1, 2, 3, 4]),
    {1:1, 2:2, 3:3, 4:4},
    (1, 2, 3, 4)
]
    
for param in params:
    print('{} is iterable? {}'.format(param, is_iterable(param)))
########## 输出 ##########
1234 is iterable? False
1234 is iterable? True
[1, 2, 3, 4] is iterable? True
{1, 2, 3, 4} is iterable? True
{1: 1, 2: 2, 3: 3, 4: 4} is iterable? True
(1, 2, 3, 4) is iterable? True
```
只要 `for i in` 的可以遍历，都是迭代器
<a name="JW1pI"></a>
### 1.2 取值
迭代器提供了一个`next`方法，调用这个方法，得到了容器的下一个对象或者一个stopiteration 的报错  ，没有下一个对象
```python
>>> a = iter("123")
>>> next(a)
'1'
>>> next(a)
'2'
>>> next(a)
'3'
>>> next(a)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration
```
但是迭代和枚举不完全一样<br />迭代并不知道总量是多少
<a name="3hQWH"></a>
## 2、生成器
那么什么又是生成器，和迭代器又有什么关系？<br />生成器就是一个迭代器的例子，如果说迭代器是人，那么生成器就是人中的一个人。<br />为什么会出来一个生成器，其实很简单声明一个迭代器很简单，但是很容易造成内存不够。<br />比如 `(i for i in range(1000000000))` 通过元组方式生成迭代器。<br />`[i for i in range(1000000000)]` 它也是一个迭代器，只不过太大了，跑不起来。于是生成器就出来了。<br />对比一下内存和消耗的时间，代码如下。
```python
import os
import psutil
# 显示当前 python 程序占用的内存大小
def show_memory_info(hint):
    pid = os.getpid()
    p = psutil.Process(pid)
    
    info = p.memory_full_info()
    memory = info.uss / 1024. / 1024
    print('{} memory used: {} MB'.format(hint, memory))
    
def test_iterator():
    show_memory_info('initing iterator')
    list_1 = [i for i in range(100000000)]
    show_memory_info('after iterator initiated')
    print(sum(list_1))
    show_memory_info('after sum called')
def test_generator():
    show_memory_info('initing generator')
    list_2 = (i for i in range(100000000))
    show_memory_info('after generator initiated')
    print(sum(list_2))
    show_memory_info('after sum called')
%time test_iterator()
%time test_generator()
########## 输出 ##########
initing iterator memory used: 48.9765625 MB
after iterator initiated memory used: 3920.30078125 MB
4999999950000000
after sum called memory used: 3920.3046875 MB
Wall time: 17 s
initing generator memory used: 50.359375 MB
after generator initiated memory used: 50.359375 MB
4999999950000000
after sum called memory used: 50.109375 MB
Wall time: 12.5 s
```
生成器还有一个`yield`关键字<br />`yield`在scrapy用的多。`yield`和`return`也很好区别，`return`就返回值，结束函数，`yield`只是保存，不会结束函数。
<a name="zlhQr"></a>
## 3、列表生成式与列表生成器
列表生成式即List Comprehensions，是Python内置的非常简单却强大的可以用来创建`list`的生成式。
```python
print([x * x for x in range(1, 11)])
print([x * x for x in range(1, 11) if x % 2 == 0])

##还可以使用两层循环，可以生成全排列：
print([m + n for m in 'ABC' for n in 'XYZ'])
print([str(x)+str(y) for x in range(1,6) for y in range(11,16)])

##for循环其实可以同时使用两个甚至多个变量，比如dict的items()可以同时迭代key和value：
d = {'x': 'A', 'y': 'B', 'z': 'C' }
print([k + '=' + v for k, v in d.items()])
```
结果：
```python
[1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
[4, 16, 36, 64, 100]
['AX', 'AY', 'AZ', 'BX', 'BY', 'BZ', 'CX', 'CY', 'CZ']
['111', '112', '113', '114', '115', '211', '212', '213', '214', '215', '311', '312', '313', '314', '315', '411', '412', '413', '414', '415', '511', '512', '513', '514', '515']
['x=A', 'y=B', 'z=C']
```
通过列表生成式，可以直接创建一个列表。但是，受到内存限制，列表容量肯定是有限的。而且，创建一个包含100万个元素的列表，不仅占用很大的存储空间，如果仅仅需要访问前面几个元素，那后面绝大多数元素占用的空间都白白浪费了。<br />所以，如果列表元素可以按照某种算法推算出来，那是否可以在循环的过程中不断推算出后续的元素呢？这样就不必创建完整的list，从而节省大量的空间。在Python中，这种一边循环一边计算的机制，称为生成器：generator。<br />只要把一个列表生成式的[]改成()，就创建了一个generator：
```python
g = (x * x for x in range(10))
```
generator保存的是算法，每次调用next(g)，就计算出g的下一个元素的值，直到计算到最后一个元素，没有更多的元素时，抛出StopIteration的错误。<br />用一个示例，感受一下生成器相对生成式的优势，首先创建一个查看当前内存情况的方法：
```python
import os
import psutil

## 显示当前 python 程序占用的内存大小
def show_memory_info(hint):
    pid = os.getpid()
    p = psutil.Process(pid)

    info = p.memory_full_info()
    memory = info.uss / 1024. / 1024
    print(f'{hint}内存使用: {memory} MB')
```
测试一下列表生成式：
```python
def test_iterator():
    show_memory_info('initing iterator')
    list_1 = [i for i in range(100000000)]
    show_memory_info('after iterator initiated')
    print(sum(list_1))
    show_memory_info('after sum called')

%time test_iterator()
```
结果：
```python
initing iterator内存使用: 48.69140625 MB
after iterator initiated内存使用: 3936.2890625 MB
4999999950000000
after sum called内存使用: 3936.29296875 MB
Wall time: 9.39 s
```
测试一下列表生成器：
```python
def test_generator():
    show_memory_info('initing generator')
    list_2 = (i for i in range(100000000))
    show_memory_info('after generator initiated')
    print(sum(list_2))
    show_memory_info('after sum called')

%time test_generator()
```
结果：
```python
initing generator内存使用: 48.8515625 MB
after generator initiated内存使用: 48.85546875 MB
4999999950000000
after sum called内存使用: 49.11328125 MB
Wall time: 7.95 s
```
声明一个迭代器很简单，`[i for i in range(100000000)]`就可以生成一个包含一亿元素的列表。每个元素在生成后都会保存到内存中，通过上面的代码可以看到，它们占用了巨量的内存，内存不够的话就会出现 OOM 错误。<br />不过，并不需要在内存中同时保存这么多东西，比如对元素求和，只需要知道每个元素在相加的那一刻是多少就行了，用完就可以扔掉了。
<a name="j4k7a"></a>
## 4、函数生成器(generator)
如果推算的算法比较复杂，用类似列表生成式的`for`循环无法实现的时候，还可以用函数来实现。<br />比如，著名的斐波拉契数列（Fibonacci），除第一个和第二个数外，任意一个数都可由前两个数相加得到：<br />1, 1, 2, 3, 5, 8, 13, 21, 34, ...<br />斐波拉契数列用列表生成式写不出来，但是用函数把它打印出来却很容易：
```python
def fib(max):
    n, a, b = 0, 0, 1
    while n < max:
        print(b)
        a, b = b, a + b
        n = n + 1

fib(6)
```
打印结果：
```python
1
1
2
3
5
8
```
上面的函数和生成器(`generator`)仅一步之遥，只要把`print(b)`改为`yield b`，`fib`函数就会变成生成器(`generator`)：
```python
def fib(max):
    n, a, b = 0, 0, 1
    while n < max:
        yield b
        a, b = b, a + b
        n = n + 1
```
这就是除了列表生成器以外定义`generator`的另一种方法。<br />如果一个函数定义中包含`yield`关键字，那么这个函数就不再是一个普通函数，而是一个`generator`：
```python
fib(6)
```
结果：
```python
<generator object fib at 0x0000000005F04A98>
```
在前面的列表生成器中说过，对于生成器可以使用`for`循环进行遍历：
```python
for i in fib(6):
    print(i)
```
打印结果：
```python
1
1
2
3
5
8
```
这里，最难理解的就是`generator`和函数的执行流程不一样。函数是顺序执行，遇到`return`语句或者最后一行函数语句就返回。而变成`generator`的函数，在每次调用`next()`的时候执行，遇到`yield`语句返回，再次执行时从上次返回的`yield`语句处继续执行。<br />举个简单的例子，定义一个`generator`，依次返回数字`1，3，5`：
```python
def odd():
    print('step 1')
    yield 1
    print('step 2')
    yield(3)
    print('step 3')
    yield(5)
```
调用该`generator`时，首先要生成一个`generator`对象，然后用`next()`函数不断获得下一个返回值：
```python
o = odd()
while True:
    print(next(o))
```
结果：
```python
step 1
1
step 2
3
step 3
5
---------------------------------------------------------------------------
StopIteration                             Traceback (most recent call last)
<ipython-input-7-554c5fb505f8> in <module>()
      1 o = odd()
      2 while True:
----> 3     print(next(o))

StopIteration: 
```
可以看到，`odd`不是普通函数，而是`generator`，在执行过程中，遇到`yield`就中断，下次又继续执行。执行3次`yield`后，已经没有`yield`可以执行了，所以，第4次调用`next()`就抛出`StopIteration`异常。<br />对于函数生成器(generator)来说，遇到`return`语句就是结束`generator`的指令（函数体最后一行语句其实隐式执行了return None），`for`循环随之结束。
<a name="JGHHU"></a>
## 5、迭代器和生成器的关系
其实生成器就是一种特殊的迭代器，而迭代器包括了生成器并不等价于生成器，它们都可以通过`next()`方法不断的获取下一个对象，都具备记忆已经读取的位置的特点。<br />例如：
```python
l = [1, 2, 3, 4]
l_iter = iter(l)
```
完成可以理解为产生了一个列表生成器：
```python
l = [1, 2, 3, 4]
l_iter = (i for i in l)
```
也可以理解成产生了一个函数生成器：
```python
l = [1, 2, 3, 4]

def func_generator(l):
    for i in l:
        yield i

l_iter = func_generator(l)
```
<a name="faI1X"></a>
## 6、利用生成器判断子序列详解
给定字符串s和t，判断s是否为t的子序列。字符串的一个子序列是原始字符串删除一些（也可以不删除）字符而不改变剩余字符相对位置形成的新字符串。（例如，"ace"是"abcde"的一个子序列，而"aec"不是）。
```python
def is_subsequence(s: str, t: str) -> bool:
    t = iter(t)
    return all(i in t for i in s)
 
print(is_subsequence("ace", "abcde"))
print(is_subsequence("aec", "abcde"))
```
首先`t = iter(t)`可以理解为产生了一个生成器：
```python
t = (i for i in t)
```
而`i in t`基本上等价于：
```python
while True:
    val = next(t)
    if val == i:
        yield True
```
测试一下：
```python
t = "abcde"
t = (i for i in t)
print('a' in t)
print('c' in t)
print(next(t))
```
结果：
```python
True
True
d
```
可以看到最后一行直接返回了匹配到c的下一个值'd'。<br />这样再测试：
```python
t = "abcde"
t = (i for i in t)
print('a' in t)
print('c' in t)
print('b' in t)
```
结果：
```python
True
True
False
```
于是就可以顺利的使用生成器计算子序列的每个元素是否满足条件：
```python
t = iter("abcde")
[i in t for i in "aec"]
```
结果：
```python
[True, True, False]
```
而`all()`函数即可判断是否全部都满足条件：
```python
print(all([True, True, False]), all([True, True, True]))
```
结果：
```python
False True
```
而上述代码`all(i in t for i in s)`没有申明`all([i in t for i in s])`列表生成式形式则代表是对一个列表生成器进行`all`运算。
