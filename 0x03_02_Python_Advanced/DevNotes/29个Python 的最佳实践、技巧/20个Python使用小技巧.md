Python
<a name="ueyzQ"></a>
## 1、易混淆操作
本节对一些 Python 易混淆的操作进行对比。
<a name="c7NLv"></a>
### 1.1 有放回随机采样和无放回随机采样
```python
import random
random.choices(seq, k=1)  # 长度为k的list，有放回采样
random.sample(seq, k)     # 长度为k的list，无放回采样
```
<a name="wqHOe"></a>
### 1.2 lambda 函数的参数
```python
func = lambda y: x + y          # x的值在函数运行时被绑定
func = lambda y, x=x: x + y     # x的值在函数定义时被绑定
```
<a name="ScFLR"></a>
### 1.3 copy 和 deepcopy
```python
import copy
y = copy.copy(x)      # 只复制最顶层
y = copy.deepcopy(x)  # 复制所有嵌套部分
```
复制和变量别名结合在一起时，容易混淆：
```python
a = [1, 2, [3, 4]]

# Alias.
b_alias = a  
assert b_alias == a and b_alias is a

# Shallow copy.
b_shallow_copy = a[:]  
assert b_shallow_copy == a and b_shallow_copy is not a and b_shallow_copy[2] is a[2]

# Deep copy.
import copy
b_deep_copy = copy.deepcopy(a)  
assert b_deep_copy == a and b_deep_copy is not a and b_deep_copy[2] is not a[2]
```
对别名的修改会影响原变量，（浅）复制中的元素是原列表中元素的别名，而深层复制是递归的进行复制，对深层复制的修改不影响原变量。
<a name="wBiIw"></a>
## 2、常用工具
<a name="a3qr6"></a>
### 2.1 读写 CSV 文件
```python
import csv
# 无header的读写
with open(name, 'rt', encoding='utf-8', newline='') as f:  # newline=''让Python不将换行统一处理
    for row in csv.reader(f):
        print(row[0], row[1])  # CSV读到的数据都是str类型
with open(name, mode='wt') as f:
    f_csv = csv.writer(f)
    f_csv.writerow(['symbol', 'change'])

# 有header的读写
with open(name, mode='rt', newline='') as f:
    for row in csv.DictReader(f):
        print(row['symbol'], row['change'])
with open(name, mode='wt') as f:
    header = ['symbol', 'change']
    f_csv = csv.DictWriter(f, header)
    f_csv.writeheader()
    f_csv.writerow({'symbol': xx, 'change': xx})
```
注意，当 CSV 文件过大时会报错：_csv.Error: field larger than field limit (131072)，通过修改上限解决
```python
import sys
csv.field_size_limit(sys.maxsize)
```
csv 还可以读以 \t 分割的数据
```python
f = csv.reader(f, delimiter='\t')
```
<a name="dlWj6"></a>
### 2.2 迭代器工具
itertools 中定义了很多迭代器工具，例如子序列工具：
```python
import itertools
itertools.islice(iterable, start=None, stop, step=None)
# islice('ABCDEF', 2, None) -> C, D, E, F

itertools.filterfalse(predicate, iterable)         # 过滤掉predicate为False的元素
# filterfalse(lambda x: x < 5, [1, 4, 6, 4, 1]) -> 6

itertools.takewhile(predicate, iterable)           # 当predicate为False时停止迭代
# takewhile(lambda x: x < 5, [1, 4, 6, 4, 1]) -> 1, 4

itertools.dropwhile(predicate, iterable)           # 当predicate为False时开始迭代
# dropwhile(lambda x: x < 5, [1, 4, 6, 4, 1]) -> 6, 4, 1

itertools.compress(iterable, selectors)            # 根据selectors每个元素是True或False进行选择
# compress('ABCDEF', [1, 0, 1, 0, 1, 1]) -> A, C, E, F
```
序列排序：
```python
sorted(iterable, key=None, reverse=False)

itertools.groupby(iterable, key=None)              # 按值分组，iterable需要先被排序
# groupby(sorted([1, 4, 6, 4, 1])) -> (1, iter1), (4, iter4), (6, iter6)

itertools.permutations(iterable, r=None)           # 排列，返回值是Tuple
# permutations('ABCD', 2) -> AB, AC, AD, BA, BC, BD, CA, CB, CD, DA, DB, DC

itertools.combinations(iterable, r=None)           # 组合，返回值是Tuple
itertools.combinations_with_replacement(...)
# combinations('ABCD', 2) -> AB, AC, AD, BC, BD, CD
```
多个序列合并：
```python
itertools.chain(*iterables)                        # 多个序列直接拼接
# chain('ABC', 'DEF') -> A, B, C, D, E, F

import heapq
heapq.merge(*iterables, key=None, reverse=False)   # 多个序列按顺序拼接
# merge('ABF', 'CDE') -> A, B, C, D, E, F

zip(*iterables)                                    # 当最短的序列耗尽时停止，结果只能被消耗一次
itertools.zip_longest(*iterables, fillvalue=None)  # 当最长的序列耗尽时停止，结果只能被消耗一次
```
<a name="Wv4qk"></a>
### 2.3 计数器
计数器可以统计一个可迭代对象中每个元素出现的次数。
```python
import collections
# 创建
collections.Counter(iterable)

# 频次
collections.Counter[key]                 # key出现频次
# 返回n个出现频次最高的元素和其对应出现频次，如果n为None，返回所有元素
collections.Counter.most_common(n=None)

# 插入/更新
collections.Counter.update(iterable)
counter1 + counter2; counter1 - counter2  # counter加减

# 检查两个字符串的组成元素是否相同
collections.Counter(list1) == collections.Counter(list2)
```
<a name="ollF0"></a>
### 2.4 带默认值的 Dict
当访问不存在的 Key 时，defaultdict 会将其设置为某个默认值。
```python
import collections
collections.defaultdict(type)  # 当第一次访问dict[key]时，会无参数调用type，给dict[key]提供一个初始值
```
<a name="xvO1n"></a>
### 2.5 有序 Dict
```python
import collections
collections.OrderedDict(items=None)  # 迭代时保留原始插入顺序
```
<a name="SXgvm"></a>
## 3、高性能编程和调试
<a name="MQZ5w"></a>
### 3.1 输出错误和警告信息
向标准错误输出信息
```python
import sys
sys.stderr.write('')
```
输出警告信息
```python
import warnings
warnings.warn(message, category=UserWarning)  
# category的取值有DeprecationWarning, SyntaxWarning, RuntimeWarning, ResourceWarning, FutureWarning
```
控制警告消息的输出
```bash
$ python -W all     # 输出所有警告，等同于设置warnings.simplefilter('always')
$ python -W ignore  # 忽略所有警告，等同于设置warnings.simplefilter('ignore')
$ python -W error   # 将所有警告转换为异常，等同于设置warnings.simplefilter('error')
```
<a name="gdQ0R"></a>
### 3.2 代码中测试
有时为了调试，想在代码中加一些代码，通常是一些 print 语句，可以写为：
```python
# 在代码中的debug部分
if __debug__:
    pass
```
一旦调试结束，通过在命令行执行 -O 选项，会忽略这部分代码：
```bash
$ python -0 main.py
```
<a name="Y9K8L"></a>
### 3.3 代码风格检查
使用 pylint 可以进行不少的代码风格和语法检查，能在运行之前发现一些错误
```bash
pylint main.py
```
<a name="n1ZvW"></a>
### 3.4 代码耗时
耗时测试
```bash
$ python -m cProfile main.py
```
测试某代码块耗时
```python
# 代码块耗时定义
from contextlib import contextmanager
from time import perf_counter

@contextmanager
def timeblock(label):
    tic = perf_counter()
    try:
        yield
    finally:
        toc = perf_counter()
        print('%s : %s' % (label, toc - tic))

# 代码块耗时测试
with timeblock('counting'):
    pass
```
代码耗时优化的一些原则

- 专注于优化产生性能瓶颈的地方，而不是全部代码。
- 避免使用全局变量。局部变量的查找比全局变量更快，将全局变量的代码定义在函数中运行通常会快 15%-30%。
- 避免使用.访问属性。使用 `from module import name` 会更快，将频繁访问的类的成员变量 `self.member` 放入到一个局部变量中。
- 尽量使用内置数据结构。str, list, set, dict 等使用 C 实现，运行起来很快。
- 避免创建没有必要的中间变量，和 `copy.deepcopy()`。
- 字符串拼接，例如 `a + ':' + b + ':' + c` 会创造大量无用的中间变量，`':',join([a, b, c])` 效率会高不少。另外需要考虑字符串拼接是否必要，例如 `print(':'.join([a, b, c]))` 效率比 `print(a, b, c, sep=':')` 低。
