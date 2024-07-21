Python
<a name="M46yl"></a>
### 1、列表推导式
使用列表推导式创建一个列表。
```python
>>> [n*10 for n in range(5)]
[0, 10, 20, 30, 40]
```
<a name="q327z"></a>
### 2、交换变量
一行代码交换两个变量的值。
```python
>>> m, n = 1, 2
>>> m, n = n, m
>>> m
2
>>> n
1
```
<a name="rCDiZ"></a>
### 3、连续比较
链式比较操作符。
```python
>>> m, n = 3, 'c'
>>> 1 < m < 5
True
>>> 'd' < n < 'f'
False
```
<a name="XBpXi"></a>
### 4、序列切片
通过切片快速获取序列片段。
```python
>>> lst = [1, 2, 3, 4, 5]
>>> lst[:3]  # 取前三个数
[1, 2, 3]
>>> lst[::-1]  # 逆序
[5, 4, 3, 2, 1]
>>> lst[::2]  # 步长为2
[1, 3, 5]
>>> lst[::-2]  # 逆序步长为2
[5, 3, 1]
```
<a name="cAOQg"></a>
### 5、切片快速增删序列
通过切片替换序列片段的值。
```python
>>> lst = [1, 2, 3, 4, 5]
>>> lst[1:3] = []
>>> lst
[1, 4, 5]
>>> lst[1:3] = ['a', 'b', 'c', 'd']
>>> lst
[1, 'a', 'b', 'c', 'd']
```
<a name="cwkO0"></a>
### 6、`%timeit` 计算运行时间
计算 10000 次列表推导式创建列表所用时间。
```python
%timeit -n 10000 [n for n in range(5)]

# 2.41 µs ± 511 ns per loop (mean ± std. dev. of 7 runs, 10000 loops each)
```
<a name="H83q9"></a>
### 7、三元表达式
获取 m，n 中较大的值。
```python
>>> m = 4
>>> n = 2
>>> if m > n:
        print('m')
else:
        print('n')

m
>>> 'm' if m > n else 'n'
'm'
```
还有一种比较少见的写法。
```python
>>> m = 4
>>> n = 2
>>> ("n", "m")[m > n]
'm'
```
格式：`(<on_true>, <on_false>)[condition]`
<a name="SsCNC"></a>
### 8、解包(`unpack`)
可迭代对象都支持解包。
```python
>>> a, b, *c = [1, 2, 3, 4]
>>> a
1
>>> b
2
>>> c
[3, 4]
>>> print(*range(3))
0 1 2
```
<a name="edpIK"></a>
### 9、`lambda` 匿名函数
lambda 函数可以接收任意多个参数 (包括可选参数) ，返回单个表达式值。 lambda 函数不能包含命令，只能包含一个表达式。
```python
>>> def func(x, y):
 return x * y

>>> func(2, 3)
6
>>> func = lambda x, y: x * y
>>> func(2, 3)
6
```
<a name="sdSNh"></a>
### 10、`map` 序列映射
对序列中每一个元素调用指定函数，返回迭代器。
```python
>>> def func(x):
       return x ** 2

>>> list(map(func, [1,2,3]))
[1, 4, 9]
```
使用 lambda 表达式。
```python
>>> list(map(lambda x: x ** 2, [1, 2, 3]))
[1, 4, 9]
```
<a name="F6OYw"></a>
### 11、`filter` 过滤序列
过滤掉不符合条件的元素，返回迭代器。
```python
>>> def func(x): 
       return x % 3 == 0
  
>>> list(filter(func, [1, 2 ,3]))
[3]
```
使用 lambda 表达式。
```python
>>> list(filter(lambda x: x % 3 == 0, [1, 2, 3]))
[3]
```
<a name="Uv4UY"></a>
### 12、获取序列组合
获取两个序列每个元素两两组合的结果。
```python
>>> list1 = ['a', 'b']
>>> list2 = ['1', '2']
>>> [(m, n) for m in list1 for n in list2]
[('a', '1'), ('a', '2'), ('b', '1'), ('b', '2')]
>>> from itertools import product
>>> list(product(list1, list2))
[('a', '1'), ('a', '2'), ('b', '1'), ('b', '2')]
```
<a name="aW5MU"></a>
### 13、随机选取序列元素
随机选取序列中的一个元素。
```python
>>> from random import choice
>>> lst = [1, 2, 3, 4]
>>> choice(lst)
3
```
随机选取序列中的多个元素(可重复)。k 值指定数量。
```python
>>> from random import choices
>>> lst = [1, 2, 3, 4]
>>> choices(lst, k=3)
[4, 3, 4]
```
随机选取序列中的多个元素(不重复)。k 值指定数量。
```python
>>> from random import sample
>>> lst = [1, 2, 3, 4]
>>> sample(lst, k=3)
[4, 3, 2]
```
<a name="eVWoz"></a>
### 14、序列元素计数
统计序列每个元素出现的次数。
```python
>>> from collections import Counter
>>> s = 'python+py'
>>> counter = Counter(s)
>>> counter
Counter({'p': 2, 'y': 2, 't': 1, 'h': 1, 'o': 1, 'n': 1, '+': 1})
```
返回的结果类似字典，可以使用字典的相关方法。
```python
>>> counter.keys()
dict_keys(['p', 'y', 't', 'h', 'o', 'n', '+'])
>>> counter.values()
dict_values([2, 2, 1, 1, 1, 1, 1])
>>> counter.items()
dict_items([('p', 2), ('y', 2), ('t', 1), ('h', 1), ('o', 1), ('n', 1), ('+', 1)])
```
统计出现次数最多的两个元素。
```python
>>> counter.most_common(2)
[('p', 2), ('y', 2)]
```
<a name="ZLmtQ"></a>
### 15、字典排序
字典按照键(key)降序排序。
```python
>>> dic = {'d': 2, 'c': 1, 'a': 3, 'b': 4}
>>> sort_by_key = sorted(dic.items(), key=lambda x: x[0], reverse=False)
>>> {key: value for key, value in sort_by_key}
{'a': 3, 'b': 4, 'c': 1, 'd': 2}
```
字典按照值(value)降序排序。
```python
>>> dic = {'d': 2, 'c': 1, 'a': 3, 'b': 4}
>>> sort_by_value = sorted(dic.items(), key=lambda x: x[1], reverse=False)
>>> {key: value for key, value in sort_by_value}
{'c': 1, 'd': 2, 'a': 3, 'b': 4}
```
<a name="M8I5B"></a>
### 16、字典合并
```python
>>> dict1 = {'name': '静香', 'age': 18}
>>> dict2 = {'name': '静香', 'sex': 'female'}
```
<a name="LxCu4"></a>
#### `update()` 更新字典。
```python
>>> dict1.update(dict2)
>>> dict1
{'name': '静香', 'age': 18, 'sex': 'female'}
```
<a name="gXoY1"></a>
#### 字典推导式
```python
>>> {k: v for dic in [dict1, dict2] for k, v in dic.items()}
{'name': '静香', 'age': 18, 'sex': 'female'}
```
<a name="SUIWi"></a>
#### 元素拼接
```python
>>> dict(list(dict1.items()) + list(dict2.items()))
{'name': '静香', 'age': 18, 'sex': 'female'}
```
<a name="gqHa7"></a>
#### `chain()` 可以将序列连接，返回可迭代对象。
```python
from itertools import chain
>>> dict(chain(dict1.items(), dict2.items()))
{'name': '静香', 'age': 18, 'sex': 'female'}
```
<a name="Dpp1W"></a>
#### `collections.ChainMap` 可以将多个字典或映射，并将它们合并。
```python
>>> from collections import ChainMap
>>> dict(ChainMap(dict2, dict1))
{'name': '静香', 'age': 18, 'sex': 'female'}
```
<a name="wuiq6"></a>
#### 在 Python3.5 以上的版本中，可以通过字典解包进行合并。
```python
>>> {**dict1, **dict2}
{'name': '静香', 'age': 18, 'sex': 'female'}
```
<a name="VGMpy"></a>
### 17、`zip` 打包
`zip()` 将序列中对应的元素打包成一个个的元组，然后返回由这些元组组成的迭代器。<br />如果序列的元素个数不一致，则返回列表长度与最短的对象相同。
```python
>>> list1 = [1, 2, 3]
>>> list2 = [4, 5, 6]
>>> list3 = ['a', 'b', 'c', 'd']
>>> res = zip(list1, list2)
>>> res
<zip object at 0x0000013C13F62200>
>>> list(res)
[(1, 4), (2, 5), (3, 6)]
>>> list(zip(list2, list3))
[(4, 'a'), (5, 'b'), (6, 'c')]
```
<a name="Pb6C5"></a>
### 18、`enumerate` 遍历
`enumerate` 函数可以将可迭代对象组合成一个索引序列，这样遍历时就可以同时获取索引与对应的值。
```python
>>> lst = ['a', 'b', 'c']
>>> for index, char in enumerate(lst):
 print(index, char)

 
0 a
1 b
2 c
```
<a name="XX6z0"></a>
### 19、`any()` & `all()`
<a name="c0noo"></a>
#### `any(iterable)`

- 对于迭代过程中的元素 i，只有所有元素 `bool(i)` 的结果都为 False，`any` 的结果才返回 False。
- 如果迭代是空，返回 False。
<a name="viPBk"></a>
#### `all(iterable)`

- 对于迭代过程中的元素 i，如果存在一个 `bool(i)` 的结果为 False，则 `all` 结果返回 False。
- 如果可迭代对象为空，则返回 True。
```python
>>> any('')
False
>>> any([])
False
>>> any([1, 0, ''])
True
>>> any([0, '', []])
False
>>> all([])
True
>>> all([1, 0, ''])
False
>>> all([1, 2, 3])
True
```
<a name="JMv0X"></a>
### 20、用 `**` 代替 `pow`
求 x 的 y 次方，使用 `**` 速度更快。
```python
%timeit -n 10000 c = pow(2,10)
# 911 ns ± 107 ns per loop (mean ± std. dev. of 7 runs, 10000 loops each)
%timeit -n 10000 c = 2 ** 10
# 131 ns ± 46.8 ns per loop (mean ± std. dev. of 7 runs, 10000 loops each)
```
