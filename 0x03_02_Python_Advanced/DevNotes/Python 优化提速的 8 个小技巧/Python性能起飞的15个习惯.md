Python<br />关于 Python 如何精确地测量程序的执行时间，这个问题看起来简单其实很复杂，因为程序的执行时间受到很多因素的影响，例如操作系统、Python 版本以及相关硬件（CPU 性能、内存读写速度）等。在同一台电脑上运行相同版本的语言时，上述因素就是确定的了，但是程序的睡眠时间依然是变化的，且电脑上正在运行的其他程序也会对实验有干扰，因此严格来说这就是《实验不可重复》。<br />关于计时比较有代表性的两个库就是`time`和`timeit`。<br />其中，time库中有`time()`、`perf_counter()`以及`process_time()`三个函数可用来计时⏲（以秒为单位），加后缀`_ns`表示以纳秒计时（自 Python3.7 始）。在此之前还有`clock()`函数，但是在 Python3.3 之后被移除了。上述三者的区别如下：

- `time()`精度上相对没有那么高，而且受系统的影响，适合表示日期时间或者大程序的计时。
- `perf_counter()`适合小一点的程序测试，会计算`sleep()`时间。
- `process_time()`适合小一点的程序测试，不计算`sleep()`时间。

与time库相比，timeit 有两个优点：

- `timeit` 会根据操作系统和 Python 版本选择最佳计时器。
- `timeit` 在计时期间会暂时禁用垃圾回收。

`timeit.timeit(stmt='pass', setup='pass', timer=<default timer>, number=1000000, globals=None)` 参数说明：

- `stmt='pass'`：需要计时的语句或者函数。
- `setup='pass'`：执行stmt之前要运行的代码。通常，它用于导入一些模块或声明一些必要的变量。
- `timer=<default timer>`：计时器函数，默认为`time.perf_counter()`。
- `number=1000000`：执行计时语句的次数，默认为一百万次。
- `globals=None`：指定执行代码的命名空间。

**本文所有的计时均采用**`**timeit**`**方法，且采用默认的执行次数一百万次**。<br />为什么要执行一百万次呢？因为测试程序很短，如果不执行这么多次的话，根本看不出差距。
<a name="knTMv"></a>
## 1、使用`map()`进行函数映射
✅ Exp1：将字符串数组中的小写字母转为大写字母。<br />测试数组为 `oldlist = ['life', 'is', 'short', 'i', 'choose', 'python']`。

- 方法一
```python
newlist = []
for word in oldlist:
    newlist.append(word.upper())
```

- 方法二
```python
list(map(str.upper, oldlist))
```
方法一耗时 **0.5267724000000005s**，方法二耗时 **0.41462569999999843s**，性能提升 **21.29%** 🚀
<a name="WuUFr"></a>
## 2、使用`set()`求交集
✅ Exp2：求两个list的交集。<br />测试数组：`a = [1,2,3,4,5]，b = [2,4,6,8,10]`。

- 方法一
```python
overlaps = []
for x in a:
    for y in b:
        if x == y:
            overlaps.append(x)
```

- 方法二
```python
list(set(a) & set(b))
```
方法一耗时 **0.9507264000000006s**，方法二耗时 **0.6148200999999993s**，性能提升 **35.33%** 🚀<br />关于`set()`的语法：|、&、-分别表示求并集、交集、差集。
<a name="u1hUD"></a>
## 3、使用`sort()`或`sorted()`排序
可以通过多种方式对序列进行排序，但其实自己编写排序算法的方法有些得不偿失。因为内置的 `sort()`或 `sorted()` 方法已经足够优秀了，且利用参数key可以实现不同的功能，非常灵活。二者的区别是`sort()`方法仅被定义在list中，而`sorted()`是全局方法对所有的可迭代序列都有效。<br />✅ Exp3：分别使用快排和`sort()`方法对同一列表排序。<br />测试数组：`lists = [2,1,4,3,0]`。

- 方法一
```python
def quick_sort(lists,i,j):
    if i >= j:
        return list
    pivot = lists[i]
    low = i
    high = j
    while i < j:
        while i < j and lists[j] >= pivot:
            j -= 1
        lists[i]=lists[j]
        while i < j and lists[i] <=pivot:
            i += 1
        lists[j]=lists[i]
    lists[j] = pivot
    quick_sort(lists,low,i-1)
    quick_sort(lists,i+1,high)
    return lists
```

- 方法二
```python
lists.sort()
```
方法一耗时 **2.4796975000000003s**，方法二耗时 **0.05551999999999424s**，性能提升 **97.76%** 🚀<br />顺带一提，`sorted()`方法耗时 **0.1339823999987857s**。<br />可以看出，`sort()`作为list专属的排序方法还是很强的，`sorted()`虽然比前者慢一点，但是胜在它“不挑食”，它对所有的可迭代序列都有效。<br />**扩展**：如何定义`sort()`或`sorted()`方法的key❓

1. 通过lambda定义
```python
#学生：（姓名，成绩，年龄）
students = [('john', 'A', 15),('jane', 'B', 12),('dave', 'B', 10)]
students.sort(key = lambda student: student[0]) #根据姓名排序
sorted(students, key = lambda student: student[0])
```

2. 通过`operator`定义
```python
import operator

students = [('john', 'A', 15),('jane', 'B', 12),('dave', 'B', 10)]
students.sort(key=operator.itemgetter(0))
sorted(students, key = operator.itemgetter(1, 0)) #先对成绩排序，再对姓名排序
```
operator的`itemgetter()`适用于普通数组排序，`attrgetter()`适用于对象数组排序

1. 通过`cmp_to_key()`定义，最为灵活
```python
import functools

def cmp(a,b):
    if a[1] != b[1]:
        return -1 if a[1] < b[1] else 1 #先按照成绩升序排序
    elif a[0] != b[0]:
        return -1 if a[0] < b[0] else 1 #成绩相同，按照姓名升序排序
    else:
        return -1 if a[2] > b[2] else 1 #成绩姓名都相同，按照年龄降序排序 

students = [('john', 'A', 15),('john', 'A', 14),('jane', 'B', 12),('dave', 'B', 10)]
sorted(students, key = functools.cmp_to_key(cmp))
```
<a name="RBWF6"></a>
## 4、使用`collections.Counter()`计数
✅ Exp4：统计字符串中每个字符出现的次数。<br />测试数组：`sentence='life is short, i choose python'`。

- 方法一
```python
counts = {}
for char in sentence:
    counts[char] = counts.get(char, 0) + 1
```

- 方法二
```python
from collections import Counter
Counter(sentence)
```
方法一耗时 **2.8105250000000055s**，方法二耗时 **1.6317423000000062s**，性能提升 **41.94%** 🚀
<a name="qf9kI"></a>
## 5、使用列表推导
列表推导（list comprehension）短小精悍。在小代码片段中，可能没有太大的区别。但是在大型开发中，它可以节省一些时间。<br />✅ Exp5：对列表中的奇数求平方，偶数不变。<br />测试数组：`oldlist = range(10)`。

- 方法一
```python
newlist = []
for x in oldlist:
    if x % 2 == 1:
        newlist.append(x**2)
```

- 方法二
```python
[x**2 for x in oldlist if x%2 == 1]
```
方法一耗时 **1.5342976000000021s**，方法二耗时 **1.4181957999999923s**，性能提升 **7.57%** 🚀
<a name="IKsMB"></a>
## 6、使用 `join()` 连接字符串
大多数人都习惯使用+来连接字符串。但其实，这种方法非常低效。因为，+操作在每一步中都会创建一个新字符串并复制旧字符串。更好的方法是用 `join()` 来连接字符串。关于字符串的其他操作，也尽量使用内置函数，如`isalpha()`、`isdigit()`、`startswith()`、`endswith()`等。<br />✅ Exp6：将字符串列表中的元素连接起来。<br />测试数组：`oldlist = ['life', 'is', 'short', 'i', 'choose', 'python']`。

- 方法一
```python
sentence = ""
for word in oldlist:
    sentence += word
```

- 方法二
```python
"".join(oldlist)
```
方法一耗时 **0.27489080000000854s**，方法二耗时 **0.08166570000000206s**，性能提升 **70.29%** 🚀<br />join还有一个非常舒服的点，就是它可以指定连接的分隔符，举个例子👇
```python
oldlist = ['life', 'is', 'short', 'i', 'choose', 'python']
sentence = "//".join(oldlist)
print(sentence)
```
```
life//is//short//i//choose//python
```
<a name="hnBb7"></a>
## 7、使用`x, y = y, x`交换变量
✅ Exp6：交换x，y的值。<br />测试数据：`x, y = 100, 200`。

- 方法一
```python
temp = x
x = y
y = temp
```

- 方法二
```python
x, y = y, x
```
方法一耗时 **0.027853900000010867s**，方法二耗时 **0.02398730000000171s**，性能提升 **13.88%** 🚀
<a name="akhhF"></a>
## 8、使用`while 1`取代`while True`
在不知道确切的循环次数时，常规方法是使用`while True`进行无限循环，在代码块中判断是否满足循环终止条件。虽然这样做没有任何问题，但while 1的执行速度比`while True`更快。因为它是一种数值转换，可以更快地生成输出。<br />✅ Exp8：分别用while 1和while True循环 100 次。

- 方法一
```python
i = 0
while True:
    i += 1
    if i > 100:
        break
```

- 方法二
```python
i = 0
while 1:
    i += 1
    if i > 100:
        break
```
方法一耗时 **3.679268300000004s**，方法二耗时 **3.607847499999991s**，性能提升**1.94%** 🚀
<a name="CSnfB"></a>
## 9、使用装饰器缓存
将文件存储在高速缓存中有助于快速恢复功能。Python 支持装饰器缓存，该缓存在内存中维护特定类型的缓存，以实现最佳软件驱动速度。使用lru_cache装饰器来为斐波那契函数提供缓存功能，在使用`fibonacci`递归函数时，存在大量的重复计算，例如`fibonacci(1)`、`fibonacci(2)`就运行了很多次。而在使用了`lru_cache`后，所有的重复计算只会执行一次，从而大大提高程序的执行效率。<br />✅ Exp9：求斐波那契数列。<br />测试数据：`fibonacci(7)`。

- 方法一
```python
def fibonacci(n):
    if n == 0:
        return 0
    elif n == 1:
        return 1
    return fibonacci(n - 1) + fibonacci(n-2)
```

- 方法二
```python
import functools

@functools.lru_cache(maxsize=128)
def fibonacci(n):
    if n == 0:
        return 0
    elif n == 1:
        return 1
    return fibonacci(n - 1) + fibonacci(n-2)
```
方法一耗时 **3.955014900000009s**，方法二耗时 **0.05077979999998661s**，性能提升 **98.72%** 🚀<br />**❗注意事项：**

- 缓存是按照参数作为键，也就说在参数不变时，被lru_cache装饰的函数只会执行一次。
- 所有参数必须可哈希，例如list不能作为被lru_cache装饰的函数的参数。
```python
import functools

@functools.lru_cache(maxsize=100)
def demo(a, b):
    print('我被执行了')
    return a + b
if __name__ == '__main__':
    demo(1, 2)
    demo(1, 2)
```
我被执行了（**执行了两次demo(1, 2)，却只输出一次**）
```python
from functools import lru_cache

@lru_cache(maxsize=100)
def list_sum(nums: list):
    return sum(nums)

if __name__ == '__main__':
    list_sum([1, 2, 3, 4, 5])
```
TypeError: unhashable type: 'list'<br />`functools.lru_cache(maxsize=128, typed=False)`的两个可选参数：

- `maxsize`代表缓存的内存占用值，超过这个值之后，就的结果就会被释放，然后将新的计算结果进行缓存，其值应当设为 2 的幂。
- `typed`若为`True`，则会把不同的参数类型得到的结果分开保存。
<a name="zvvFm"></a>
## 10、减少点运算符(`.`)的使用
点运算符(`.`)用来访问对象的属性或方法，这会引起程序使用`__getattribute__()`和`__getattr__()`进行字典查找，从而带来不必要的开销。尤其注意，在循环当中，更要减少点运算符的使用，应该将它移到循环外处理。<br />这启发我们应该尽量使用`from ... import ...`这种方式来导包，而不是在需要使用某方法时通过点运算符来获取。其实不光是点运算符，其他很多不必要的运算都尽量移到循环外处理。<br />✅ Exp10：将字符串数组中的小写字母转为大写字母。<br />测试数组为 `oldlist = ['life', 'is', 'short', 'i', 'choose', 'python']`。

- 方法一
```python
newlist = []
for word in oldlist:
    newlist.append(str.upper(word))
```

- 方法二
```python
newlist = []
upper = str.upper
for word in oldlist:
    newlist.append(upper(word))
```
方法一耗时 **0.7235491999999795s**，方法二耗时 **0.5475435999999831s**，性能提升 **24.33%** 🚀
<a name="LuQw4"></a>
## 11、使用for循环取代while循环
当知道具体要循环多少次时，使用for循环比使用while循环更好。<br />✅ Exp12：使用for和while分别循环 100 次。

- 方法一
```python
i = 0
while i < 100:
    i += 1
```

- 方法二
```python
for _ in range(100):
    pass
```
方法一耗时 **3.894683299999997s**，方法二耗时 **1.0198077999999953s**，性能提升**73.82%** 🚀
<a name="a7T7d"></a>
## 12、使用Numba.jit加速计算
Numba 可以将 Python 函数编译码为机器码执行，大大提高代码执行速度，甚至可以接近 C 或 FORTRAN 的速度。它能和 Numpy 配合使用，在 for 循环中或存在大量计算时能显著地提高执行效率。<br />✅ Exp12：求从 1 加到 100 的和。

- 方法一
```python
def my_sum(n):
    x = 0
    for i in range(1, n+1):
        x += i
    return x
```

- 方法二
```python
from numba import jit

@jit(nopython=True) 
def numba_sum(n):
    x = 0
    for i in range(1, n+1):
        x += i
    return x
```
方法一耗时 **3.7199997000000167s**，方法二耗时 **0.23769430000001535s**，性能提升 **93.61%** 🚀
<a name="cBCQZ"></a>
## 13、使用Numpy矢量化数组
矢量化是 NumPy 中的一种强大功能，可以将操作表达为在整个数组上而不是在各个元素上发生。这种用数组表达式替换显式循环的做法通常称为矢量化。<br />在 Python 中循环数组或任何数据结构时，会涉及很多开销。NumPy 中的向量化操作将内部循环委托给高度优化的 C 和 Fortran 函数，从而使 Python 代码更加快速。<br />✅ Exp13：两个长度相同的序列逐元素相乘。<br />测试数组：`a = [1,2,3,4,5], b = [2,4,6,8,10]`

- 方法一
```python
[a[i]*b[i] for i in range(len(a))]
```

- 方法二
```python
import numpy as np
a = np.array([1,2,3,4,5])
b = np.array([2,4,6,8,10])
a*b
```
方法一耗时 **0.6706845000000214s**，方法二耗时 **0.3070132000000001s**，性能提升 **54.22%** 🚀
<a name="XY1gu"></a>
## 14、使用`in`检查列表成员
若要检查列表中是否包含某成员，通常使用`in`关键字更快。<br />✅ Exp14：检查列表中是否包含某成员。<br />测试数组：`lists = ['life', 'is', 'short', 'i', 'choose', 'python']`

- 方法一
```python
def check_member(target, lists):
    for member in lists:
        if member == target:
            return True
    return False
```

- 方法二
```python
if target in lists:
    pass
```
方法一耗时 **0.16038449999999216s**，方法二耗时 **0.04139250000000061s**，性能提升 **74.19%** 🚀
<a name="QW9iF"></a>
## 15、使用itertools库迭代
itertools是用来操作迭代器的一个模块，其函数主要可以分为三类：无限迭代器、有限迭代器、组合迭代器。<br />✅ Exp15：返回列表的全排列。<br />测试数组：["Alice", "Bob", "Carol"]

- 方法一
```python
def permutations(lst):
    if len(lst) == 1 or len(lst) == 0:
        return [lst]
    result = []
    for i in lst:
        temp_lst = lst[:]
        temp_lst.remove(i)
        temp = permutations(temp_lst)
        for j in temp:
            j.insert(0, i)
            result.append(j)
    return result
```

- 方法二
```python
import itertools
itertools.permutations(["Alice", "Bob", "Carol"])
```
方法一耗时 **3.867292899999484s**，方法二耗时 **0.3875405000007959s**，性能提升 **89.98%** 🚀<br />**扩展**：[itertools库详解](https://docs.python.org/3/library/itertools.html)
<a name="ZsUXo"></a>
## 结语
总结下来其实就是下面这两条原则：
<a name="ZmDw6"></a>
### 1、尽量使用内置库函数
内置库函数由专业的开发人员编写并经过了多次测试，很多库函数的底层是用C语言开发的。因此，这些函数总体来说是非常高效的（比如`sort()`、`join()`等），自己编写的方法很难超越它们，还不如省省功夫，不要重复造轮子了，何况你造的轮子可能更差。所以，如果函数库中已经存在该函数，就直接拿来用。
<a name="OeyF8"></a>
### 2、尽量使用优秀的第三方库
有很多优秀的第三方库，它们的底层可能是用 C 和 Fortran 来实现的，像这样的库用起来绝对不会吃亏，比如前文提到的 Numpy 和 Numba，它们带来的提升都是非常惊人的。类似这样的库还有很多，比如Cython、PyPy等，这里只是抛砖引玉。<br />其实加快 Python 代码执行速度的方法还有很多，比如**避免使用全局变量**、**使用最新版本**、**使用合适的数据结构**、**利用if条件的惰性**等等，这里就不一一例举了。这些方法都需要亲身去实践才会有深刻的感受和理解，但最根本的方法就是保持对编程的热情和对最佳实践的追求，这才是能不断突破自我、勇攀高峰的不竭动力源泉！👏
