Python
<a name="Cd7QO"></a>
## 1、最简单的原地更新
字典对象内置了一个 update 方法，用于把另一个字典更新到自己身上。
```python
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> profile.update(ext_info)
>>> print(profile)
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```
如果想使用 update 这种最简单、最地道原生的方法，但又不想更新到自己身上，而是生成一个新的对象，那请使用深拷贝。
```python
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> from copy import deepcopy
>>>
>>> full_profile = deepcopy(profile)
>>> full_profile.update(ext_info)
>>>
>>> print(full_profile)
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
>>> print(profile)
{"name": "xiaoming", "age": 27}
```
<a name="vuh5C"></a>
## 2、先解包再合并字典
使用 `**` 可以解包字典，解包完后再使用 dict 或者 `{}` 就可以合并。
```python
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> full_profile01 = {**profile, **ext_info}
>>> print(full_profile01)
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
>>>
>>> full_profile02 = dict(**profile, **ext_info)
>>> print(full_profile02)
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```
若不知道 `dict(**profile, **ext_info)` 底层做了什么处理，可以将它等价于
```python
>>> dict((("name", "xiaoming"), ("age", 27), ("gender", "male")))
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```
<a name="aOqmx"></a>
## 3、借助 itertools
在 Python 里有一个非常强大的内置模块，它专门用于操作可迭代对象。<br />字典也是可迭代对象，自然就可以想到，可以使用 `itertools.chain()` 函数先将多个字典（可迭代对象）串联起来，组成一个更大的可迭代对象，然后再使用 dict 转成字典。
```python
>>> import itertools
>>>
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>>
>>> dict(itertools.chain(profile.items(), ext_info.items()))
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```
<a name="UIiyz"></a>
## 4、借助 ChainMap
如果可以引入一个辅助包， `ChainMap` 也可以达到和 `itertools` 同样的效果。
```python
>>> from collections import ChainMap
>>>
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> dict(ChainMap(profile, ext_info))
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```
使用 ChainMap 有一点需要注意，当字典间有重复的键时，只会取第一个值，排在后面的键值并不会更新掉前面的（使用 itertools 就不会有这个问题）。
```python
>>> from collections import ChainMap
>>>
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info={"age": 30}
>>> dict(ChainMap(profile, ext_info))
{'name': 'xiaoming', 'age': 27}
```
<a name="7F5b6"></a>
## 5、使用`dict.items()`合并
在 Python 3.9 之前，其实就已经有 `|` 操作符了，只不过它通常用于对集合（set）取并集。<br />利用这一点，也可以将它用于字典的合并，只不过得绕个弯子，有点不好理解。<br />先利用 `items` 方法将 dict 转成 dict_items，再对这两个 dict_items 取并集，最后利用 dict 函数，转成字典。
```python
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> full_profile = dict(profile.items() | ext_info.items())
>>> full_profile
{'gender': 'male', 'age': 27, 'name': 'xiaoming'}
```
如果嫌这样太麻烦，也可以简单点，直接使用 list 函数再合并（示例为 Python 3.x ）
```python
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> dict(list(profile.items()) + list(ext_info.items()))
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```
若在 Python 2.x 下，可以直接省去 list 函数。
```python
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> dict(profile.items() + ext_info.items())
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```
<a name="Y3oQy"></a>
## 6、最酷炫的字典解析式
Python 里对于生成列表、集合、字典，有一套非常 Pythonnic 的写法。<br />那就是列表解析式，集合解析式和字典解析式，通常是 Python 发烧友的最爱，具体示例代码如下：
```python
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> {k:v for d in [profile, ext_info] for k,v in d.items()}
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```
<a name="fgPhG"></a>
## 7、Python 3.9 新特性
在 Python 3.9.04a 版本中，新增了一个新操作符，操作符：`|`， PEP584 将它称之为合并操作符（Union Operator），用它可以很直观地合并多个字典。
```python
>>> profile = {"name": "xiaoming", "age": 27}
>>> ext_info = {"gender": "male"}
>>>
>>> profile | ext_info
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
>>>
>>> ext_info | profile
{'gender': 'male', 'name': 'xiaoming', 'age': 27}
>>>
```
除了 `|` 操作符之外，还有另外一个操作符 `|=`，类似于原地更新。
```python
>>> ext_info |= profile
>>> ext_info
{'gender': 'male', 'name': 'xiaoming', 'age': 27}
>>>
>>>
>>> profile |= ext_info
>>> profile
{'name': 'xiaoming', 'age': 27, 'gender': 'male'}
```

