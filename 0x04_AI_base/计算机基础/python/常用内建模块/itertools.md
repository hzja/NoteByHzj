<a name="TvXVi"></a>
# count/cycle/repeat
```python
# count()
# 因为count()会创建一个无限的迭代器，所以上述代码会打印出自然数序列，
# 根本停不下来，只能按Ctrl+C退出。
>>> import itertools
>>> natuals = itertools.count(1)
>>> for n in natuals:
...     print(n)
...
1
2
3
...

# cycle()
# 同样停不下来。
>>> import itertools
>>> cs = itertools.cycle('ABC') # 注意字符串也是序列的一种
>>> for c in cs:
...     print(c)
...
'A'
'B'
'C'
'A'
'B'
'C'
...

# repeat()
# repeat()负责把一个元素无限重复下去，不过如果提供第二个参数就可以限定重复次数
>>> ns = itertools.repeat('A', 3)
>>> for n in ns:
...     print(n)
...
A
A
A

# takewhile()
# 无限序列虽然可以无限迭代下去，但是通常我们会通过takewhile()等函数
# 根据条件判断来截取出一个有限的序列：
>>> natuals = itertools.count(1)
>>> ns = itertools.takewhile(lambda x: x <= 10, natuals)
>>> list(ns)
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="FbeLd"></a>
# chain
```python
>>> for c in itertools.chain('ABC', 'XYZ'):
...     print(c)
# 迭代效果：'A' 'B' 'C' 'X' 'Y' 'Z'
```
<a name="Ybjka"></a>
# groupby
```python
# groupby()把迭代器中相邻的重复元素挑出来放在一起：
>>> for key, group in itertools.groupby('AAABBBCCAAA'):
...     print(key, list(group))
...
A ['A', 'A', 'A']
B ['B', 'B', 'B']
C ['C', 'C']
A ['A', 'A', 'A']

# 实际上挑选规则是通过函数完成的，只要作用于函数的两个元素返回的值相等，
# 这两个元素就被认为是在一组的，而函数返回值作为组的key。如果我们要忽略大小写分组，
# 就可以让元素'A'和'a'都返回相同的key：
>>> for key, group in itertools.groupby('AaaBBbcCAAa', lambda c: c.upper()):
...     print(key, list(group))
...
A ['A', 'a', 'a']
B ['B', 'B', 'b']
C ['c', 'C']
A ['A', 'A', 'a']
```
<a name="ji8rs"></a>
# Source
[https://www.liaoxuefeng.com/wiki/1016959663602400/1017783145987360](https://www.liaoxuefeng.com/wiki/1016959663602400/1017783145987360)
