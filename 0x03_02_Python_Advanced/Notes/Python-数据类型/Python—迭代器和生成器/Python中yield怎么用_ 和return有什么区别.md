Python<br />return 关键字用在中函数，后面跟一个表达式或者啥也不写，表示该函数被调用时会返回一个值（如果不写东西就返回一个None）
```python
def function():
    return 10

value = function()
print(value)  # 10
```
程序执行到`return`这行代码时，该函数的直接就结束了，哪怕后面还有代码也不会执行。因此有时候也用`return`来提前结束函数
```python
def function(name):
    if name=='人妖':
        return
    else:
        print("你好", name)
```
再来说`yield`<br />理解`yield`这个关键字之前，需要先理解一些前置知识，主要迭代器和生成器的概念。
<a name="k7sNb"></a>
### 什么是迭代器
顾名思义，迭代器就是用于迭代操作（for 循环）的对象，它像列表一样可以迭代获取其中的每一个元素，任何实现了 `__next__` 方法的对象都可以称为迭代器。<br />它与列表的区别在于，调用迭代器的时候，不像列表把所有元素一次性加载到内存，而是以一种延迟计算（lazy evaluation）方式返回元素，这正是它的优点。比如列表含有中一千万个整数，需要占超过400M的内存，而迭代器只需要几十个字节的空间。因为它并没有把所有元素装载到内存中，而是等到调用 next 方法时候才返回该元素（按需调用 call by need 的方式），本质上 for 循环就是不断地调用迭代器的next方法，直到遇到异常StopIteration结束循环。<br />以斐波那契数列为例来实现一个迭代器：
```python
class Fib:
    def __init__(self, n):
        self.prev = 0
        self.cur = 1
        self.n = n

    def __iter__(self):
        return self

    def __next__(self):
        if self.n > 0:
            value = self.cur
            self.cur = self.cur + self.prev
            self.prev = value
            self.n -= 1
            return value
        else:
            raise StopIteration()


f = Fib(10)
print([i for i in f])  # [1, 1, 2, 3, 5, 8, 13, 21, 34, 55]
```
<a name="dvCUA"></a>
### 什么是生成器
普通函数用 `return` 返回一个值，在 Python 中还有一种函数，用关键字 `yield` 来返回值，这种函数叫生成器函数，函数被调用时会返回一个生成器对象而不是yield后面的值。<br />生成器本质上还是一个迭代器，也是用在迭代操作中，因此它有和迭代器一样的特性，唯一的区别在于实现方式上不一样，后者更加简洁<br />最简单的生成器函数：
```python
>>> def func(n):
...     yield n*2
...
>>> func
<function func at 0x00000000029F6EB8>
>>> g = func(5)
>>> g
<generator object func at 0x0000000002908630>
>>>
```
func 就是一个生成器函数，调用该函数时返回对象就是生成器 g ，这个生成器对象的行为和迭代器是非常相似的，可以用在 for 循环等场景中。注意 yield 对应的值在函数被调用时不会立刻返回，而是调用next方法时才返回
```python
>>> g = func(5)
>>> next(g)
10

>>> g = func(5)
>>> for i in g:
...     print(i)
...
10
```
一个函数中可以有多个yield语句，有几个yield语句有可以迭代几次。
```python
def generate():
    yield 1
    yield 2
    yield 3


g = generate()

print(next(g)) # 1
print(next(g)) # 2
print(next(g)) # 3
print(next(g)) # 报错 StopIteration
```
那为什么要用生成器呢？显然，它没有那么多冗长代码了，而且性能上一样的高效，为什么不用呢？来看看用生成器实现斐波那契数列有多简单。
```python
def fib(n):
    prev, curr = 0, 1
    while n > 0:
        n -= 1
        yield curr
        prev, curr = curr, curr + prev

print([i for i in fib(10)])
#[1, 1, 2, 3, 5, 8, 13, 21, 34, 55]
```
<a name="UkxOQ"></a>
### 生成器表达式
曾经介绍过列表推导式（list comprehension），生成器表达式与列表推导式长的非常像，但是它俩返回的对象不一样，前者返回生成器对象，后者返回列表对象。
```python
>>> g = (x*2 for x in range(10))
>>> type(g)
<type 'generator'>
>>> l = [x*2 for x in range(10)]
>>> type(l)
<type 'list'>
```
前面已经介绍过生成器的优势，就是迭代海量数据时，显然生成器更合适。
