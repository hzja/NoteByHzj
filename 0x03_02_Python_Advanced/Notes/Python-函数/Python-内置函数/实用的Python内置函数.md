Python 内置函数
<a name="kVyrA"></a>
### `all(iterable)`
如果可迭代的对象(数组，字符串，列表等，下同)中的元素都是 true (或者为空)的话返回 True 。
```python
_all = True
for item in iterable:
    if not item:
        _all = False
        break
if _all:
    # do stuff
```
更简便的写法是：
```python
if all(iterable):
    # do stuff
```
<a name="MKhqY"></a>
### `any(iterable)`
如果可迭代的对象中任何一个元素为 true 的话返回 True 。如果可迭代的对象为空则返回 False 。
```python
_any = False
for item in iterable:
    if item:
        _any = True
        break
if _any:
    # do stuff
```
更简便的写法是：
```python
if any(iterable):
    # do stuff
```
<a name="ZWTuq"></a>
### `cmp(x, y)`
比较两个对象 x 和 y 。x < y 的时候返回负数， x ==y 的时候返回 0， x > y 的时候返回正数。
```python
def compare(x,y):
    if x < y:
        return -1
    elif x == y:
        return 0
    else:
        return 1
```
完全可以使用一句 `cmp(x, y)` 来替代。
<a name="loRzb"></a>
### `dict([arg])`
使用 arg 提供的条目生成一个新的字典。<br />arg 通常是未知的，但是它很方便！比如说，如果想把一个含两个元组的列表转换成一个字典，可以这么做。
```python
l = [('Knights', 'Ni'), ('Monty', 'Python'), ('SPAM', 'SPAAAM')]
d = dict()
for tuple in l:
    d[tuple[0]] = tuple[1]
# {'Knights': 'Ni', 'Monty': 'Python', 'SPAM': 'SPAAAM'}
```
或者这样：
```python
l = [('Knights', 'Ni'), ('Monty', 'Python'), ('SPAM', 'SPAAAM')]
d = dict(l) # {'Knights': 'Ni', 'Monty': 'Python', 'SPAM': 'SPAAAM'}
```
<a name="I3hMP"></a>
### `enumerate(iterable [,start=0])`
一般的遍历可能会这么写：
```python
for i in range(len(list)):
    # do stuff with list[i], for example, print it
    print i, list[i]
```
可以使用 enumerate() 来提高可读性。
```python
for i, item in enumerate(list):
    # so stuff with item, for example print it
    print i, item
```
<a name="PdZUq"></a>
### `isinstance(object, classinfo)`
如果 object 参数是 classinfo 参数的一个实例或者子类(直接或者间接)的话返回 True 。<br />想检验一个对象的类型的时候，第一个想到的应该是使用 `type()` 函数。
```python
if type(obj) == type(dict):
    # do stuff
elif type(obj) == type(list):
    # do other stuff
...
```
或者可以这么写：
```python
if isinstance(obj, dict):
    # do stuff
elif isinstance(obj, list):
    # do other stuff
...
```
<a name="KhxtJ"></a>
### `pow(x, y [,z])`
返回 x 的 y 次幂(如果 z 存在的话则以 z 为模)。<br />如果想计算 x 的 y 次方，以 z 为模，那么可以这么写：
```python
mod = (x ** y) % z
```
但是当 x=1234567， y=4567676， z=56 的时候电脑足足跑了 64 秒！<br />不要用 ** 和 % 了，使用 `pow(x, y, z)` 吧！这个例子可以写成 `pow(1234567, 4567676, 56)` ，只用了 0.034 秒就出了结果！
<a name="XJMMO"></a>
### `zip([iterable, ])`
这个函数返回一个含元组的列表，具体请看例子。
```python
l1 = ('You gotta', 'the')
l2 = ('love', 'built-in')
out = []
if len(l1) == len(l2):
    for i in range(len(l1)):
        out.append((l1[i], l2[i]))
# out = [('You gotta', 'love'), ('the', 'built-in)]
```
或者这么写：
```python
l1 = ['You gotta', 'the']
l2 = ['love', 'built-in']
out = zip(l1, l2) # [('You gotta', 'love'), ('the', 'built-in)]
```
如果想得到倒序的话加上 `*` 操作符就可以了。
```python
print zip(*out)
# [('You gotta', 'the'), ('love', 'built-in')]
```
