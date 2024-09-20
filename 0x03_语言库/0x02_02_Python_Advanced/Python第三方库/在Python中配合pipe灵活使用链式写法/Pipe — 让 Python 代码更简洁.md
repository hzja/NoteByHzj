Python<br />map和filter是Python中的两种高效函数，用于处理可迭代对象。然而，如果同时使用map和filter，代码会显得很乱。
```python
>>> arr = [1, 2, 3, 4, 5]
>>> list(map(lambda x: x * 2, filter(lambda x: x % 2 == 0, arr)))
"""
[4, 8]
"""
```
如果能像下面这样使用管道`|`在一个迭代器上应用多个方法，那不是很好吗？
```python
>>> from pipe import select, where
>>> arr =[1, 2, 3, 4, 5]
>>> list(arr
··· 	| where( lambda x: x % 2 == 0 )
··· 	| select( lambda x: x * 2))
"""
[4, 8]
"""
```
<a name="nw9ad"></a>
## 什么是Pipe？
[**Pipe**](https://github.com/JulienPalard/Pipe)是一个Python库，使你能够在Python中使用管道。一个管道（|）将一个方法的结果传递给另一个方法。<br />Pipe很受欢迎，因为它使我们的代码在对Python迭代器应用多种方法时看起来更干净。由于Pipe中的函数屈指可数，所以学习Pipe也非常容易。在这篇文章中，云朵君将和大家一起学习这个很酷的方法～<br />首先需要安装Pipe：
```bash
pip install pipe
```
<a name="eNpP3"></a>
## Where
迭代器中的过滤元素--where<br />与SQL类似，Pipe的where方法也可以用来过滤迭代表中的元素。
```python
>>> from pipe import where
>>> arr = [1, 2, 3, 4, 5]
>>> list(arr | where(lambda x: x % 2 == 0))
"""
[2, 4]
"""
```
<a name="tlEO2"></a>
## Select
将一个函数应用到一个迭代器上—select<br />select方法与map方法类似。select将一个方法应用于迭代器的每个元素。<br />在下面的代码中，我使用select将列表中的每个元素乘以2。
```python
>>> from pipe import select
>>> arr = [1, 2, 3, 4, 5]
>>> list(arr | select(lambda x: x * 2))
"""
[2, 4, 6, 8, 10]
"""
```
现在，你可能想知道：如果where和select的功能与map和filter相同，为什么还需要这些方法？<br />因为可以使用管道在另一个方法之后插入一个方法。因此，使用管道可以去除嵌套的小括号，使代码更容易阅读。
```python
>>> from pipe import select, where
>>> arr =[1, 2, 3, 4, 5]
>>> list(arr
··· 	| where( lambda x: x % 2 == 0 )
··· 	| select( lambda x: x * 2))
"""
[4, 8]
"""
```
<a name="CXMH7"></a>
## Chain
迭代序列的链路--chain<br />处理嵌套的迭代器可能是一件很痛苦的事情。可以使用chain来链接一连串的迭代变量。
```python
>>> from pipe import chain
>>> nested = [[1, 2, [3]], [4, 5]]
>>> list(nested | chain)
"""
[1, 2, [3], 4, 5]
"""
```
尽管应用链式后迭代器的嵌套程度降低了，仍然有一个嵌套的列表。要处理一个深度嵌套的列表，可以使用 traverse 来代替。
<a name="kVoZX"></a>
## Traverse
递归展开迭代器--traverse<br />traverse方法可以用来递归地展开迭代器。因此，可以用这个方法把一个深度嵌套的列表变成一个一维列表。
```python
>>> from pipe import traverse
>>> nested = [[1, 2, [3]], [4, 5]]
>>> list(nested | traverse)
"""
[1, 2, 3, 4, 5]
"""
```
把这个方法和select方法整合起来，以获得一个字典的值，并把列表扁平化。
```python
>>> from pipe import traverse, select
>>> fruits = [
... { "name" : "apple", "price": [2, 5]}
... { "name" : "orange", "price": 4},
... { "name" : "grape", "price": 5},
...]
>>> list( fruits
		| select( lambda fruit: fruit["price"])
    	| traverse)
"""
[2, 5, 4, 5]
"""
```
<a name="NaivB"></a>
## 列表中的元素分组
有时，使用某个函数对列表中的元素进行分组可能是有用的。这可以用groupby方法轻松实现。<br />为了看看这个方法是如何工作的，把一个数字列表变成一个字典，根据数字是偶数还是奇数来分组。
```python
>>> from pipe import groupby, select
>>> list(
... 	( 1, 2, 3, 4, 5, 6, 7, 8, 9)
... 	| groupby(lambda x: "Even" if x % 2==0 else "0dd")
... 	| select(lambda x: {x[0]: list(x[1])})
... )
"""
[{'Even': [2, 4, 6, 8]}, {'0dd': [1, 3, 5, 7, 9]}]
"""
```
在上面的代码中，使用groupby将数字分组为Even组和Odd组。应用这个方法后的输出看起来像下面这样：<br />接下来，使用select将一个元组变成一个字典的列表，其键是元组中的第一个元素，值是元组中的第二个元素。<br />为了只获得大于2的值，可以在选择方法中加入where方法：
```python
>>> from pipe import groupby, select, where
>>> list(
... 	( 1, 2, 3, 4, 5, 6, 7, 8, 9)
... 	| groupby(lambda x: "Even" if x % 2==0 else "0dd")
... 	| select(lambda x: {x[0]: list(x[1] | where(lambda x: x>2))})
... )
"""
[{'Even': [4, 6, 8]}, {'0dd': [3, 5, 7, 9]}]
"""
```
请注意，输出中不再有2和1。
<a name="PV5j3"></a>
## Dedup
使用一个键来重复取值--`dedup`<br />`dedup`方法删除列表中的重复部分。
```python
>>> from pipe import dedup
>>> arr = [1, 2, 2, 3, 4, 5, 6, 6, 7, 9, 3, 3, 1]
>>> list(arr | dedup)
"""
[1, 2, 3, 4, 5, 6, 7, 9]
"""
```
这听起来可能没什么意思，因为set方法可以做同样的事情。然而，这种方法更加灵活，因为它使你能够使用一个键获得独特的元素。<br />例如，可以用这个方法来获得一个小于5的唯一元素和另一个大于或等于5的唯一元素。
```python
>>> from pipe import dedup
>>> arr = [1, 2, 2, 3, 4, 5, 6, 6, 7, 9, 3, 3, 1]
>>> list(arr | dedup(lambda key: key < 5))
"""
[1, 5]
"""
```
现在，把这个方法与 select 和 where 结合起来，得到一个有重复键和 None 值的字典的值。
```python
>>> from pipe import groupby, select, where
>>> data = [
...     {"name": "apple", "count": 2},
...     {"name": "orange", "count": 4},
...     {"name": "grape", "count": None},
...     {"name": "orange", "count": 7},
... ]
>>> list(
... data
... | dedup(key=lambda fruit: fruit["name"])
... | select(lambda fruit: fruit["count"])
... | where(lambda count: isinstance( count, int))
... )
"""
[2, 4]
"""
```
在上面的代码中：

- 移除同名的元素
- 获得count的值
- 只选择整数的值。

在几行代码中，可以将多个方法应用于一个迭代器，同时仍然保持代码的简洁。
<a name="RaDId"></a>
## 参考资料
Pipe: [_https://github.com/JulienPalard/Pipe_](https://github.com/JulienPalard/Pipe)
