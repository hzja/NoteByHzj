Python
<a name="TantW"></a>
## 计时与性能分析
在开始优化之前，首先需要找到代码的哪一部分真正拖慢了整个程序。有时程序性能的瓶颈显而易见，但当不知道瓶颈在何处时，这里有一些帮助找到性能瓶颈的办法：<br />注：下列程序用作演示目的，该程序计算 e 的 X 次方（摘自 Python 文档）：
```python
# slow_program.py
from decimal import *
def exp(x):
    getcontext().prec += 2
    i, lasts, s, fact, num = 0, 0, 1, 1, 1
    while s != lasts:
        lasts = s
        i += 1
        fact *= i
        num *= x
        s += num / fact
    getcontext().prec -= 2
    return +s
exp(Decimal(150))
exp(Decimal(400))
exp(Decimal(3000))
```
<a name="BnhKs"></a>
### 最懒惰的「性能分析」
首先，最简单但说实话也很懒的方法——使用 Unix 的 time 命令：
```bash
~ $ time python3.8 slow_program.py
real    0m11,058s
user    0m11,050s
sys     0m0,008s
```
如果整个程序计时，这个命令即可完成目的，但通常是不够的
<a name="5Opyc"></a>
### 最细致的性能分析
另一个极端是 cProfile，它提供了「太多」的信息：
```bash
~ $ python3.8 -m cProfile -s time slow_program.py
         1297 function calls (1272 primitive calls) in 11.081 seconds
   Ordered by: internal time
   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        3   11.079    3.693   11.079    3.693 slow_program.py:4(exp)
        1    0.000    0.000    0.002    0.002 {built-in method _imp.create_dynamic}
      4/1    0.000    0.000   11.081   11.081 {built-in method builtins.exec}
        6    0.000    0.000    0.000    0.000 {built-in method __new__ of type object at 0x9d12c0}
        6    0.000    0.000    0.000    0.000 abc.py:132(__new__)
       23    0.000    0.000    0.000    0.000 _weakrefset.py:36(__init__)
      245    0.000    0.000    0.000    0.000 {built-in method builtins.getattr}
        2    0.000    0.000    0.000    0.000 {built-in method marshal.loads}
       10    0.000    0.000    0.000    0.000 <frozen importlib._bootstrap_external>:1233(find_spec)
      8/4    0.000    0.000    0.000    0.000 abc.py:196(__subclasscheck__)
       15    0.000    0.000    0.000    0.000 {built-in method posix.stat}
        6    0.000    0.000    0.000    0.000 {built-in method builtins.__build_class__}
        1    0.000    0.000    0.000    0.000 __init__.py:357(namedtuple)
       48    0.000    0.000    0.000    0.000 <frozen importlib._bootstrap_external>:57(_path_join)
       48    0.000    0.000    0.000    0.000 <frozen importlib._bootstrap_external>:59(<listcomp>)
        1    0.000    0.000   11.081   11.081 slow_program.py:1(<module>)
...
```

这里，结合 cProfile 模块和 time 参数运行测试脚本，使输出行按照内部时间（cumtime）排序。这提供了大量信息，上面看到的行只是实际输出的 10%。从输出结果可以看到 exp 函数是罪魁祸首，现在可以更加专注于计时和性能分析了
<a name="2Qe1I"></a>
### 计时专用函数
现在知道了需要关注哪里，那么可能只想要给运行缓慢的函数计时而不去管代码的其他部分。可以使用一个简单的装饰器来做到这点：
```python
def timeit_wrapper(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()  # Alternatively, you can use time.process_time()
        func_return_val = func(*args, **kwargs)
        end = time.perf_counter()
        print('{0:<10}.{1:<8} : {2:<8}'.format(func.__module__, func.__name__, end - start))
        return func_return_val
    return wrapper
```

接着，将该装饰器按如下方式应用在待测函数上：
```python
@timeit_wrapper
def exp(x):
    ...
print('{0:<10} {1:<8} {2:^8}'.format('module', 'function', 'time'))
exp(Decimal(150))
exp(Decimal(400))
exp(Decimal(3000))
```
得到如下输出：
```bash
~ $ python3.8 slow_program.py
module     function   time  
__main__  .exp      : 0.003267502994276583
__main__  .exp      : 0.038535295985639095
__main__  .exp      : 11.728486061969306
```
此时需要考虑想要测量哪一类时间。time 库提供了 time.perf_counter 和 time.process_time 两种时间。其区别在于，perf_counter 返回绝对值，其中包括了 Python 程序并不在运行的时间，因此它可能受到机器负载的影响。而 process_time 只返回用户时间（除去了系统时间），也就是只有进程运行时间。
<a name="1a798"></a>
## 让程序更快
现在到了真正有趣的部分了，让 Python 程序跑得更快！通过通用的想法和策略。使用这些策略，可以对程序性能产生巨大的影响，有时甚至可以带来高达 30% 的提速。
<a name="DZX8y"></a>
### 使用内置的数据类型
这一点非常明显。内置的数据类型非常快，尤其相比于树或链表等自定义类型而言。这主要是因为内置数据类型使用 C 语言实现，使用 Python 实现的代码在运行速度上和它们没法比。
<a name="kmTy9"></a>
### 使用 lru_cache 实现缓存/记忆
用一个简单例子再次进行说明：
```python
import functools
import time
# caching up to 12 different results
@functools.lru_cache(maxsize=12)
def slow_func(x):
    time.sleep(2)  # Simulate long computation
    return x
slow_func(1)  # ... waiting for 2 sec before getting result
slow_func(1)  # already cached - result returned instantaneously!
slow_func(3)  # ... waiting for 2 sec before getting result
```
上面的函数使用 time.sleep 模拟了繁重的计算过程。当第一次使用参数 1 调用函数时，它等待了 2 秒钟后返回了结果。当再次调用时，结果已经被缓存起来，所以它跳过了函数体，直接返回结果。
<a name="f8WMT"></a>
### 使用局部变量
这和每个作用域中变量的查找速度有关。之所以说「每个作用域」，是因为这不仅仅关乎局部变量或全局变量。事实上，就连函数中的局部变量、类级别的属性和全局导入函数这三者的查找速度都会有区别。函数中的局部变量最快，类级别属性（如 self.name）慢一些，全局导入函数（如 time.time）最慢。<br />可以通过这种看似没有必要的代码组织方式来提高效率：
```python
#  Example #1
class FastClass:
    def do_stuff(self):
        temp = self.value  # this speeds up lookup in loop
        for i in range(10000):
            ...  # Do something with `temp` here
#  Example #2
import random
def fast_function():
    r = random.random
    for i in range(10000):
        print(r())  # calling `r()` here, is faster than global random.random()
```
<a name="MIe9i"></a>
### 使用函数
这也许有些反直觉，因为调用函数会让更多的东西入栈，进而在函数返回时为程序带来负担，但这其实和之前的策略相关。如果只是把所有代码扔进一个文件而没有把它们放进函数，那么它会因为众多的全局变量而变慢。因此，可以通过将所有代码封装在 main 函数中并调用它来实现加速，如下所示：
```python
def main():
    ...  # All your previously global code
main()
```
<a name="x4Pa3"></a>
### 不要访问属性
另一个可能让程序变慢的东西是用来访问对象属性的点运算符（.）。这个运算符会引起程序使用__getattribute__进行字典查找，进而为程序带来不必要的开销。那么怎么避免（或者限制）使用它呢？
```python
#  Slow:
import re
def slow_func():
    for i in range(10000):
        re.findall(regex, line)  # Slow!
#  Fast:
from re import findall
def fast_func():
    for i in range(10000):
        findall(regex, line)  # Faster!
```
<a name="tav4j"></a>
### 当心字符串
当在循环中使用取模运算符（%s）或 .format() 时，字符串操作会变得很慢。有没有更好的选择呢？只需要使用 f-string 即可，它可读性更强，代码更加紧凑，并且速度更快！基于这一观点，如下从快到慢列出了可以使用的一系列方法：
```python
f'{s} {t}'  # Fast!
s + '  ' + t 
' '.join((s, t))
'%s %s' % (s, t) 
'{} {}'.format(s, t)
Template('$s $t').substitute(s=s, t=t)  # Slow!
```
生成器本质上并不会更快，因为它们的目的是惰性计算，以节省内存而非节省时间。然而，节省的内存会让程序运行更快。为什么呢？如果有一个大型数据集，并且没有使用生成器（迭代器），那么数据可能造成 CPU 的 L1 缓存溢出，进而导致访存速度显著变慢。
