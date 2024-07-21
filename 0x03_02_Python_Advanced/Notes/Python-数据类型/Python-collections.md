Python collections<br />这里主要介绍OrderedDict类、defaultdict类、Counter类、namedtuple类和deque类。
<a name="CDTbG"></a>
### collections
collections是实现了特定目标的容器，以提供Python标准内建容器 dict , list , set , 和 tuple 的替代选择。<br />collections模块：实现了特定目标的容器，以提供Python标准内建容器 dict、list、set、tuple 的替代选择。<br />`Counter`：字典的子类，提供了可哈希对象的计数功能。<br />`defaultdict`：字典的子类，提供了一个工厂函数，为字典查询提供了默认值。<br />`OrderedDict`：字典的子类，保留了他们被添加的顺序。<br />`namedtuple`：创建命名元组子类的工厂函数。<br />`deque`：类似列表容器，实现了在两端快速添加(append)和弹出(pop)。<br />`ChainMap`：类似字典的容器类，将多个映射集合到一个视图里面。
<a name="jBwjP"></a>
### Counter
计数器（counter）以字典的形式返回序列中各个字符出现的次数，值为key，次数为value<br />Counter是一个dict子类，主要是用来对访问的对象的频率进行计数。
```python
>>> import collections
>>> # 统计字符出现的次数
... collections.Counter('hello world')
Counter({'l': 3, 'o': 2, 'h': 1, 'e': 1, ' ': 1, 'w': 1, 'r': 1, 'd': 1})
>>> # 统计单词个数
... collections.Counter('hello world hello lucy'.split())
Counter({'hello': 2, 'world': 1, 'lucy': 1})
```
Counter是对字典类型的补充，用于追踪值得出现次数 。
```python
import collections

counter = collections.Counter("My name is Runsen")
print(counter)
```
输出如下
```python
Counter({' ': 3, 'n': 3, 'e': 2, 's': 2, 'M': 1, 'y': 1, 'a': 1, 'm': 1, 'i': 1, 'R': 1, 'u': 1})
```
取得元素重复次数的值
```python
print(counter[' '])
3
```
常用方法：

- `elements()`：返回一个迭代器，每个元素重复计算的个数，如果一个元素的计数小于1,就会被忽略。
- `most_common([n])`：返回一个列表，提供n个访问频率最高的元素和计数
- `subtract([iterable-or-mapping])`：从迭代对象中减去元素，输入输出可以是0或者负数
- `update([iterable-or-mapping])`：从迭代对象计数元素或者从另一个 映射对象 (或计数器) 添加。
```python
>>> c = collections.Counter('hello world hello lucy'.split())
>>> c
Counter({'hello': 2, 'world': 1, 'lucy': 1})
>>> # 获取指定对象的访问次数，也可以使用get方法
... c['hello']
2
>>> # 查看元素
... list(c.elements())
['hello', 'hello', 'world', 'lucy']
>>> c1 = collections.Counter('hello world'.split())
>>> c2 = collections.Counter('hello lucy'.split())
>>> c1
Counter({'hello': 1, 'world': 1})
>>> c2
Counter({'hello': 1, 'lucy': 1})
>>> # 追加对象，+或者c1.update(c2)
... c1+c2
Counter({'hello': 2, 'world': 1, 'lucy': 1})
>>> # 减少对象，-或者c1.subtract(c2)
... c1-c2
Counter({'world': 1})
>>> # 清除
... c.clear()
>>> c
Counter()
```
`elements()`取得计数器中的所有元素。<br />注：此处非所有元素集合，而是包含所有元素集合的迭代器.
```python
counter = collections.Counter('abcabcabcdabcdef') 
print(counter)
# 输出如下
Counter({'a': 4, 'b': 4, 'c': 4, 'd': 2, 'e': 1, 'f': 1})

print(counter.elements())
# 输出如下
<itertools.chain object at 0x0000025B1477BF98>

print(list(counter.elements()))
# 输出如下
['a', 'a', 'a', 'a', 'b', 'b', 'b', 'b', 'c', 'c', 'c', 'c', 'd', 'd', 'e', 'f']
```
将`Counter`按照`value`从大到小排列，获取前N个元素，需要使用函数`most_common`
```python
# most_common(N)数量从大到小排列，获取前N个元素 
print(counter.most_common(3))
# 输出如下
[('a', 4), ('b', 4), ('c', 4)]
```
`sorted`将`Counter`中的`key`进行排序，返回的是所有key的列表
```python
# sorted()列出所有不同的元素并排序 
print(sorted(counter))

# 输出如下
['a', 'b', 'c', 'd', 'e', 'f']
```
将Counter转换成字符串，字符串的`join`方法可以解决。注意不是原来的模样。
```python
# 转换成字符串 
print(''.join(counter.elements()))
# aaaabbbbccccddef
print(''.join(list(counter.elements())))
```
`update()`更新计数器，其实在原本的`counter`更新计数器，如果原来没有，则新建`key`，如果有value则加一
```python
# update()更新计数器，
d = collections.Counter('a') 
counter.update(d) 
print(counter) 
# 输出如下
Counter({'a': 5, 'b': 4, 'c': 4, 'd': 2, 'e': 1, 'f': 1})
```
`update()`更新计数器，那么`subtract()`相减计数器的`values`，即原来的计数器中的每一个元素的数量减去后添加的元素的数量
```python
counter.subtract('abdabcabcg') 
print(counter)

# 输出如下
Counter({'a': 2, 'c': 2, 'b': 1, 'd': 1, 'e': 1, 'f': 1, 'g': -1})
```
<a name="LAmfk"></a>
### defaultdict
返回一个新的类似字典的对象。defaultdict 是内置 dict 类的子类。<br />`class collections.defaultdict([default_factory[, ...]])`<br />在使用字典的时候, 如果在使用不存在的key的时候发生KeyError这样的一个报错, 这时候就该defaultdict登场了。<br />defaultdict接受一个工厂函数作为参数来构造：`dict =defaultdict( factory_function)`<br />这个`factory_function`可以是`list`、`set`、`str`等等，作用是当key不存在时，返回的是工厂函数的默认值，比如`list`对应`[]`，`str`对应的是空字符串，`set`对应`set()`，`int`对应0，如下举例：
```python
from collections import defaultdict

dict1 = defaultdict(int)
dict2 = defaultdict(set)
dict3 = defaultdict(str)
dict4 = defaultdict(list)
dict1[2] ='two'

print(dict1[1])
print(dict2[1])
print(dict3[1])
print(dict4[1])

# 输出如下
0
set()

[]
```
defaultdict类接受类型名称作为初始化函数的参数，这样使得默认值的取值更加灵活。
```python
s = 'mynameisrunsen'
d = collections.defaultdict(int)
for k in s:
    d[k] += 1
print(d)

# 输出如下
defaultdict(<class 'int'>, {'m': 2, 'y': 1, 'n': 3, 'a': 1, 'e': 2, 'i': 1, 's': 2, 'r': 1, 'u': 
```
```python
>>> d = collections.defaultdict()
>>> d
defaultdict(None, {})
>>> e = collections.defaultdict(str)
>>> e
defaultdict(<class 'str'>, {})
```
例子<br />`defaultdict`的一个典型用法是使用其中一种内置类型（如str、int、list或dict等）作为默认工厂，这些内置类型在没有参数调用时返回空类型。
```python
>>> e = collections.defaultdict(str)
>>> e
defaultdict(<class 'str'>, {})
>>> e['hello']
''
>>> e
defaultdict(<class 'str'>, {'hello': ''})
>>> # 普通字典调用不存在的键时，报错
... e1 = {}
>>> e1['hello']
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
KeyError: 'hello'
```
<a name="Q8I87"></a>
#### 使用 int 作为 default_factory
```python
>>> fruit = collections.defaultdict(int)
>>> fruit['apple'] = 2
>>> fruit
defaultdict(<class 'int'>, {'apple': 2})
>>> fruit['banana']  # 没有对象时，返回0
0
>>> fruit
defaultdict(<class 'int'>, {'apple': 2, 'banana': 0})
```
<a name="22dd038d"></a>
#### 使用 list 作为 default_factory
```python
>>> s = [('yellow', 1), ('blue', 2), ('yellow', 3), ('blue', 4), ('red', 1)]
>>> d = collections.defaultdict(list)
>>> for k,v in s:
...     d[k].append(v)
...
>>> d
defaultdict(<class 'list'>, {'yellow': [1, 3], 'blue': [2, 4], 'red': [1]})
>>> d.items()
dict_items([('yellow', [1, 3]), ('blue', [2, 4]), ('red', [1])])
>>> sorted(d.items())
[('blue', [2, 4]), ('red', [1]), ('yellow', [1, 3])]
```
<a name="0b1c69d3"></a>
#### 使用 dict 作为 default_factory
```python
>>> nums = collections.defaultdict(dict)
>>> nums[1] = {'one':1}
>>> nums
defaultdict(<class 'dict'>, {1: {'one': 1}})
>>> nums[2]
{}
>>> nums
defaultdict(<class 'dict'>, {1: {'one': 1}, 2: {}})
```
<a name="008eefa9"></a>
#### 使用 set 作为 default_factory
```python
>>> types = collections.defaultdict(set)
>>> types['手机'].add('华为')
>>> types['手机'].add('小米')
>>> types['显示器'].add('AOC')
>>> types
defaultdict(<class 'set'>, {'手机': {'华为', '小米'}, '显示器': {'AOC'}})
```
<a name="F5lI4"></a>
#### 练习
有如下值集合`[11,22,33,44,55,66,77,88,99,99]`。<br />下面将所有大于 55的值保存至字典的第一个key中，将小于 55 的值保存至第二个key的值中。<br />下面通过defaultdict默认字典对列表进行划分。
```python
all_list = [11,22,33,44,55,66,77,88,99]
dic = collections.defaultdict(list)
for i in all_list:
    if i > 55:
        dic['k1'].append(i)
    else:
        dic['k2'].append(i)
print(dic)
# 输出如下
defaultdict(<class 'list'>, {'k2': [11, 22, 33, 44, 55], 'k1': [66, 77, 88, 99]})
```
也可以使用字典，具体代码如下。
```python
all_list = [11,22,33,44,55,66,77,88,99]
dic = {}
for i in all_list:
    if i > 55:
        if "k1" in dic.keys():
            dic['k1'].append(i)
        else:
            dic['k1'] = [i,]
    else:
        if "k2" in dic.keys():
            dic['k2'].append(i)
        else:
            dic['k2'] = [i,]
print(dic)
# 输出如下
{'k2': [11, 22, 33, 44, 55], 'k1': [66, 77, 88, 99]}
```
<a name="OrderedDict"></a>
### OrderedDict
Python字典中的键的顺序是任意的，它们不受添加的顺序的控制。使用dict时要保持Key的顺序，可以用OrderedDict。<br />`collections.OrderedDict` 类提供了保留他们添加顺序的字典对象。
```python
>>> o = collections.OrderedDict()
>>> o['k1'] = 'v1'
>>> o['k3'] = 'v3'
>>> o['k2'] = 'v2'
>>> o
OrderedDict([('k1', 'v1'), ('k3', 'v3'), ('k2', 'v2')])
```
如果在已经存在的 key 上添加新的值，将会保留原来的 key 的位置，然后覆盖 value 值。
```python
>>> o['k1'] = 666
>>> o
OrderedDict([('k1', 666), ('k3', 'v3'), ('k2', 'v2')])
>>> dict(o)
{'k1': 666, 'k3': 'v3', 'k2': 'v2'}
```
```python
from collections import OrderedDict 
dic = OrderedDict() 
dic['k1'] = 'v1' 
dic['k2'] = 'v2' 
dic['k3'] = 'v3' 
print(dic) 
# 输出如下
OrderedDict([('k1', 'v1'), ('k2', 'v2'), ('k3', 'v3')])

# 字典所有的键 
print(dic.keys())
# 输出如下

odict_keys(['k1', 'k2', 'k3'])
    
# 字典所有值 
print(dic.values())
# 输出如下
odict_values(['v1', 'v2', 'v3'])
    
# items() 方法以列表返回可遍历的(键, 值) 元组数组 
print(dic.items())

# 输出如下
odict_items([('k1', 'v1'), ('k2', 'v2'), ('k3', 'v3')])
    

#pop()方法，删除指定的键值 
dic.pop('k1')  
print(dic) 
# 输出如下
OrderedDict([('k2', 'v2'), ('k3', 'v3')])


#popitem()方法，默认删除字典最后一个元素 
dic.popitem() 
print(dic) 
# 输出如下
OrderedDict([('k2', 'v2')])

# update()更新字典 
dic.update({'k1':'v1111','k10':'v10'}) 
print(dic) 
# 输出如下
OrderedDict([('k2', 'v2'), ('k1', 'v1111'), ('k10', 'v10')])
```
OrderedDict和字典操作完全相同，区别在于OrderedDict的Key会按照插入的顺序排列，不是Key本身排序
```python
>>> from collections import OrderedDict
>>> d = dict([('a', 1), ('b', 2), ('c', 3)])
>>> d # dict的Key是无序的
{'a': 1, 'c': 3, 'b': 2}
>>> od = OrderedDict([('a', 1), ('b', 2), ('c', 3)])
>>> od # OrderedDict的Key是有序的
OrderedDict([('a', 1), ('b', 2), ('c', 3)])
```
<a name="H0roC"></a>
### namedtuple
namedtuple是用来创建一个自定义的tuple对象，并且规定了tuple元素的个数，并可以用属性而不是索引来引用tuple的某个元素。<br />使用命名元组的步骤：
```python
# 将元组封装为一个类，可以通过字段名（属性名）来访问元组中的值
# 支持元组的所有操作
from collections import namedtuple
# 1、定义一个类
Runsen = namedtuple('Fcant', ['name','sex','age'])
# 2、创建对象
runsen = Runsen("Fcant", "帅男", 21)
# 3、获取命名元组的值
print(runsen[1])    # 支持元组的索引取值  
print(runsen[-2:])  # 支持切片
print(runsen.name)  # 支持通过字段名来取值 
# _fields，获取命名元组的所有属性名
print(runsen._fields)
# _asdict方法，将元组转化为字典
print(runsen._asdict())

# 输出如下

帅男
('帅男', 21)
Fcant
('name', 'sex', 'age')
OrderedDict([('name', 'Fcant'), ('sex', '帅男'), ('age', 21)])
```
三种定义命名元组的方法：第一个参数是命名元组的构造器（如下的：Person1，Person2，Person3）
```python
>>> P1 = collections.namedtuple('Person1',['name','age','height'])
>>> P2 = collections.namedtuple('Person2','name,age,height')
>>> P3 = collections.namedtuple('Person3','name age height')
```
实例化命名元组
```python
>>> lucy = P1('lucy',23,180)
>>> lucy
Person1(name='lucy', age=23, height=180)
>>> jack = P2('jack',20,190)
>>> jack
Person2(name='jack', age=20, height=190)
>>> lucy.name  # 直接通过 实例名.属性 来调用
'lucy'
>>> lucy.age
23
```
<a name="7t7Jd"></a>
### deque
deque支持从任意一端增加和删除元素。更为常用的两种结构，就是栈和队列。<br />`collections.deque` 返回一个新的双向队列对象，从左到右初始化（用方法 `append()`），从 `iterable`（迭代对象）数据创建。如果 iterable 没有指定，新队列为空。<br />`collections.deque` 队列支持线程安全，对于从两端添加（`append`）或者弹出（`pop`），复杂度O(1)。<br />虽然 list 对象也支持类似操作，但是这里优化了定长操作（`pop(0)`、`insert(0,v)`）的开销。<br />如果 maxlen 没有指定或者是 None ，deque 可以增长到任意长度。否则，deque 就限定到指定最大长度。一旦限定长度的 deque 满了，当新项加入时，同样数量的项就从另一端弹出。<br />支持的方法：

- `append(x)`：添加x到右端。
- `appendleft(x)`：添加x到左端。
- `clear()`：清除所有元素，长度变为0。
- `copy()`：创建一份浅拷贝。
- `count(x)`：计算队列中个数等于x的元素。
- `extend(iterable)`：在队列右侧添加iterable中的元素。
- `extendleft(iterable)`：在队列左侧添加iterable中的元素，注：在左侧添加时，iterable参数的顺序将会反过来添加。
- `index(x[,start[,stop]])`：返回第 x 个元素（从 start 开始计算，在 stop 之前）。返回第一个匹配，如果没找到的话，抛出 ValueError 。
- `insert(i,x)`：在位置 i 插入 x 。注：如果插入会导致一个限长deque超出长度 maxlen 的话，就抛出一个 IndexError 。
- `pop()`：移除最右侧的元素。
- `popleft()`：移除最左侧的元素。
- `remove(value)`：移去找到的第一个 value。没有抛出ValueError。
- `reverse()`：将deque逆序排列。返回 None 。
- `maxlen`：队列的最大长度，没有限定则为None。
```python
>>> d = collections.deque(maxlen=10)
>>> d
deque([], maxlen=10)
>>> d.extend('python')
>>> [i.upper() for i in d]
['P', 'Y', 'T', 'H', 'O', 'N']
>>> d.append('e')
>>> d.appendleft('f')
>>> d.appendleft('g')
>>> d.appendleft('h')
>>> d
deque(['h', 'g', 'f', 'p', 'y', 't', 'h', 'o', 'n', 'e'], maxlen=10)
>>> d.appendleft('i')
>>> d
deque(['i', 'h', 'g', 'f', 'p', 'y', 't', 'h', 'o', 'n'], maxlen=10)
>>> d.append('m')
>>> d
deque(['h', 'g', 'f', 'p', 'y', 't', 'h', 'o', 'n', 'm'], maxlen=10)
```
deque的常见操作
```python
#定义一个空的双向队列
d = collections.deque()

#从右端增加元素
d.extend("Runsen") 
d.append("Maoli")
d.append("Python")
d.append("king")

#从左端增加元素
d.appendleft('left')
print(d)
# 输出如下 (注意：extend和append的区别)
deque(['left', 'R', 'u', 'n', 's', 'e', 'n', 'Maoli', 'Python', 'king'])

# reverse()反转队列 
print(d.reverse())
d.reverse()
print(d)
# 输出如下
None
deque(['king', 'Python', 'Maoli', 'n', 'e', 's', 'n', 'u', 'R', 'left'])


d.reverse()
d.extend(['qq','ww','ee']) 
print(d) 
# deque(['left', 'R', 'u', 'n', 's', 'e', 'n', 'Maoli', 'Python', 'king', 'qq', 'ww', 'ee'])


# count()计数 
print(d.count('R'))
# 输出如下
1

# clear()清空队列 
d.clear() 
print(d) 
# 输出如下
deque([])

# index()取得元素下标 
print(d.index('Maoli'))
# 输出如下
7

# insert()指定位置插入元素 
d.insert(1,'Runsen') 
print(d) 
# deque(['left', 'Runsen',R', 'u', 'n', 's', 'e', 'n', 'Maoli', 'Python', 'king', 'qq', 'ww', 'ee'])
```
<a name="aozvo"></a>
### ChainMap
问题背景是有多个字典或者映射，想把它们合并成为一个单独的映射，有人说可以用update进行合并，这样做的问题就是新建了一个数据结构以致于对原来的字典进行更改的时候不会同步。如果想建立一个同步的查询方法，可以使用 ChainMap。<br />可以用来合并两个或者更多个字典，当查询的时候，从前往后依次查询。简单使用：
```python
>>> d1 = {'apple':1,'banana':2}
>>> d2 = {'orange':2,'apple':3,'pike':1}
>>> combined1 = collections.ChainMap(d1,d2)
>>> combined2 = collections.ChainMap(d2,d1)
>>> combined1
ChainMap({'apple': 1, 'banana': 2}, {'orange': 2, 'apple': 3, 'pike': 1})
>>> combined2
ChainMap({'orange': 2, 'apple': 3, 'pike': 1}, {'apple': 1, 'banana': 2})
>>> for k,v in combined1.items():
...     print(k,v)
...
orange 2
apple 1
pike 1
banana 2
>>> for k,v in combined2.items():
...     print(k,v)
...
apple 3
banana 2
orange 2
pike 1
```
有一个注意点就是当对ChainMap进行修改的时候总是只会对第一个字典进行修改，如果第一个字典不存在该键，会添加。
```python
>>> d1 = {'apple':1,'banana':2}
>>> d2 = {'orange':2,'apple':3,'pike':1}
>>> c = collections.ChainMap(d1,d2)
>>> c
ChainMap({'apple': 1, 'banana': 2}, {'orange': 2, 'apple': 3, 'pike': 1})
>>> c['apple']
1
>>> c['apple'] = 2
>>> c
ChainMap({'apple': 2, 'banana': 2}, {'orange': 2, 'apple': 3, 'pike': 1})
>>> c['pike']
1
>>> c['pike'] = 3
>>> c
ChainMap({'apple': 2, 'banana': 2, 'pike': 3}, {'orange': 2, 'apple': 3, 'pike': 1})
```
从原理上面讲，ChainMap 实际上是把放入的字典存储在一个队列中，当进行字典的增加删除等操作只会在第一个字典上进行，当进行查找的时候会依次查找，`new_child()` 方法实质上是在列表的第一个元素前放入一个字典，默认是{}，而 parents 是去掉了列表开头的元素。
```python
>>> a = collections.ChainMap()
>>> a['x'] = 1
>>> a
ChainMap({'x': 1})
>>> b = a.new_child()
>>> b
ChainMap({}, {'x': 1})
>>> b['x'] = 2
>>> b
ChainMap({'x': 2}, {'x': 1})
>>> b['y'] = 3
>>> b
ChainMap({'x': 2, 'y': 3}, {'x': 1})
>>> a
ChainMap({'x': 1})
>>> c = a.new_child()
>>> c
ChainMap({}, {'x': 1})
>>> c['x'] = 1
>>> c['y'] = 1
>>> c
ChainMap({'x': 1, 'y': 1}, {'x': 1})
>>> d = c.parents
>>> d
ChainMap({'x': 1})
>>> d is a
False
>>> d == a
True
```
```python
>>> a = {'x':1,'z':3}
>>> b = {'y':2,'z':4}
>>> c = collections.ChainMap(a,b)
>>> c
ChainMap({'x': 1, 'z': 3}, {'y': 2, 'z': 4})
>>> c.maps
[{'x': 1, 'z': 3}, {'y': 2, 'z': 4}]
>>> c.parents
ChainMap({'y': 2, 'z': 4})
>>> c.parents.maps
[{'y': 2, 'z': 4}]
>>> c.parents.parents
ChainMap({})
>>> c.parents.parents.parents
ChainMap({})
```
