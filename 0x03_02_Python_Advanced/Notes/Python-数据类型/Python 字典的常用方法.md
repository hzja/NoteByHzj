Python 字典<br />字典(Dictionary)是Python提供的一种常用的数据结构，它用于存放具有映射关系的数据。<br />**字典由键（key）和值（value）成对组成，键和值中间以冒号：隔开，项之间用逗号隔开，整个字典由大括号**`**{}**`**括起来**。格式如下：
```python
dic = {key1 : value1, key2 : value2 }
```
字典也被称作关联数组或哈希表。下面是几种常见的字典创建方式：
```python
# 方法1
dic1 = { 'Author' : 'Python' , 'age' : 99 , 'sex' : '男' }
# 方法2
lst = [('Author', 'Python'), ('age', 99), ('sex', '男')]
dic2 = dict(lst)
# 方法3
dic3 = dict( Author = 'Python', age = 99, sex = '男')
# 方法4
list1 = ['Author', 'age', 'sex']
list2 = ['Python', 99, '男']
dic4 = dict(zip(list1, list2))
```
字典创建的方式还有很多种。**<br />字典由 dict 类代表，可以使用 `dir(dict)` 来查看该类包含哪些方法，输入命令，可以看到如下输出结果：
```python
methods = dir(dict)
print('methods = ',methods)
methods = ['__class__', '__contains__', '__delattr__', '__delitem__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__getitem__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__iter__', '__le__', '__len__', '__lt__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__reversed__', '__setattr__', '__setitem__', '__sizeof__', '__str__', '__subclasshook__', 'clear', 'copy', 'fromkeys', 'get', 'items', 'keys', 'pop', 'popitem', 'setdefault', 'update', 'values']
```
字典的方法和属性有很多种，这里重点介绍以下11种方法：
```python
['clear', 'copy', 'fromkeys', 'get', 'items', 'keys', 'pop', 'popitem', 'setdefault', 'update', 'values']
```
<a name="C0FoF"></a>
### 1、`dict.clear()`
`clear()` 用于清空字典中所有元素（键-值对），对一个字典执行 `clear()` 方法之后，该字典就会变成一个空字典。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', 99, '男']
dic1 = dict(zip(list1, list2))
# dic1 = {'Author': 'Python', 'age': 99, 'sex': '男'}
dic1.clear()
# dic1 = {}
```
<a name="xDmeJ"></a>
### 2、`dict.copy()`
`copy()` 用于返回一个字典的浅拷贝。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', 99, '男']
dic1 = dict(zip(list1, list2))
dic2 = dic1 # 浅拷贝: 引用对象
dic3 = dic1.copy() # 浅拷贝：深拷贝父对象（一级目录），子对象（二级目录）不拷贝，还是引用
dic1['age'] = 18
# dic1 = {'Author': 'Python', 'age': 18, 'sex': '男'}
# dic2 = {'Author': 'Python', 'age': 18, 'sex': '男'}
# dic3 = {'Author': 'Python', 'age': 99, 'sex': '男'}
```
其中 dic2 是 dic1 的引用，所以输出结果是一致的，dic3 父对象进行了深拷贝，不会随dic1 修改而修改，子对象是浅拷贝所以随 dic1 的修改而修改，注意父子关系。
<a name="yfX3R"></a>
#### 拓展深拷贝：`copy.deepcopy()`
```python
import copy
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
dic2 = dic1
dic3 = dic1.copy()
dic4 = copy.deepcopy(dic1)
dic1['age'].remove(18)
dic1['age'] = 20
# dic1 = {'Author': 'Python', 'age': 20, 'sex': '男'}
# dic2 = {'Author': 'Python', 'age': 20, 'sex': '男'}
# dic3 = {'Author': 'Python', 'age': [99], 'sex': '男'}
# dic4 = {'Author': 'Python', 'age': [18, 99], 'sex': '男'}
```
dic2 是 dic1 的引用，所以输出结果是一致的；dic3 父对象进行了深拷贝，不会随dic1 修改而修改，子对象是浅拷贝所以随 dic1 的修改而修改；dic4 进行了深拷贝，递归拷贝所有数据，相当于完全在另外内存中新建原字典，所以修改dic1不会影响dic4的数据
<a name="rexOJ"></a>
### 3、`dict.fromkeys()`
`fromkeys()` 使用给定的多个键创建一个新字典，值默认都是 None，也可以传入一个参数作为默认的值。
```python
list1 = ['Author', 'age', 'sex']
dic1 = dict.fromkeys(list1)
dic2 = dict.fromkeys(list1, 'Python')
# dic1 = {'Author': None, 'age': None, 'sex': None}
# dic2 = {'Author': 'Python', 'age': 'Python', 'sex': 'Python'}
```
<a name="mR1wr"></a>
### 4、`dict.get()`
`get()` 用于返回指定键的值，也就是根据键来获取值，在键不存在的情况下，返回 None，也可以指定返回值。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
Author = dic1.get('Author')
# Author = Python
phone = dic1.get('phone')
# phone = None
phone = dic1.get('phone','12345678')
# phone = 12345678
```
<a name="o1OJt"></a>
### 5、`dict.items()`
`items()` 获取字典中的所有键-值对，一般情况下可以将结果转化为列表再进行后续处理。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
items = dic1.items()
print('items = ', items)
print(type(items))
print('items = ', list(items))
# items = dict_items([('Author', 'Python'), ('age', [18, 99]), ('sex', '男')])
# <class 'dict_items'>
# items = [('Author', 'Python'), ('age', [18, 99]), ('sex', '男')]
```
<a name="bYCI2"></a>
### 6、`dict.keys()`
`keys()` 返回一个字典所有的键。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
keys = dic1.keys()
print('keys = ', keys)
print(type(keys))
print('keys = ', list(keys))
# keys = dict_keys(['Author', 'age', 'sex'])
# <class 'dict_keys'>
# keys = ['Author', 'age', 'sex']
```
<a name="CTHv1"></a>
### 7、`dict.pop()`
pop() 返回指定键对应的值，并在原字典中删除这个键-值对。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
sex = dic1.pop('sex')
print('sex = ', sex)
print('dic1 = ',dic1)
# sex = 男
# dic1 = {'Author': 'Python', 'age': [18, 99]}
```
<a name="Z7SSo"></a>
### 8、`dict.popitem()`
`popitem()` 删除字典中的最后一对键和值。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
dic1.popitem()
print('dic1 = ',dic1)
# dic1 = {'Author': 'Python', 'age': [18, 99]}
```
<a name="CRyuf"></a>
### 9、`dict.setdefault()`
`s``etdefault()` 和 `get()` 类似, 但如果键不存在于字典中，将会添加键并将值设为default。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
dic1.setdefault('Author', '')
print('dic1 = ',dic1)
# dic1 = {'Author': 'Python', 'age': [18, 99], 'sex': '男'}
dic1.setdefault('name', 'Hello')
print('dic1 = ',dic1)
# dic1 = {'Author': 'Python', 'age': [18, 99], 'sex': '男', 'name': 'Hello'}
```
<a name="VWpxm"></a>
### 10、`dict.update(dict1)`
`update()` 字典更新，将字典dict1的键-值对更新到dict里，如果被更新的字典中己包含对应的键-值对，那么原键-值对会被覆盖，如果被更新的字典中不包含对应的键-值对，则添加该键-值对。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
print('dic1 = ',dic1)
# dic1 = {'Author': 'Python', 'age': [18, 99], 'sex': '男'}
list3 = ['Author', 'phone' ]
list4 = ['Hello', 12345678]
dic2 = dict(zip(list3, list4))
print('dic2 = ',dic2)
# dic2 = {'Author': 'Hello', 'phone': 12345678}
dic1.update(dic2)
print('dic1 = ',dic1)
# dic1 = {'Author': 'Hello', 'age': [18, 99], 'sex': '男', 'phone': 12345678}
```
<a name="IqYZx"></a>
### 11、`dict.values()`
`values()` 返回一个字典所有的值。
```python
list1 = ['Author', 'age', 'sex']
list2 = ['Python', [18,99], '男']
dic1 = dict(zip(list1, list2))
values = dic1.values()
print('values = ', values)
print(type(values))
print('values = ', list(values))
# values = dict_values(['Python', [18, 99], '男'])
# <class 'dict_values'>
# values = ['Python', [18, 99], '男']
```

