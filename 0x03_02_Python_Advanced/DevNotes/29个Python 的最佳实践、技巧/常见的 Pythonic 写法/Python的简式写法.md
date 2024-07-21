Python
<a name="Ng3KQ"></a>
### 1、两个变量值互换
```python
>>> a=1
>>> b=2
>>> a,b=b,a
>>> a
2
>>> b
1
```
<a name="vm3v8"></a>
### 2、连续赋值
```python
a = b = c = 50
```
<a name="oi7uE"></a>
### 3、自动解包
```python
>>> a,b,c = [1,2,3]
>>> a
1
>>> b
2
>>> c
3
>>>
>>>
>>> a, *others = [1,2,3,4]
>>> a
1
>>> others
[2, 3, 4]
>>>
```
<a name="98nDA"></a>
### 4、链式比较
```python
a = 15
if (10 < a < 20):
    print("Hi")
```
等价于
```python
a = 15
if (a>10 and a<20):
    print("Hi")
```
<a name="aGUJo"></a>
### 5、重复列表
```python
>>> [5,2]*4
[5, 2, 5, 2, 5, 2, 5, 2]
```
<a name="6EwOj"></a>
### 6、重复字符串
```python
>>> "hello"*3
'hellohellohello'
```
<a name="t7VqE"></a>
### 7、三目运算
```python
age = 30
slogon = "Hello" if  age == 30 else "Hi"
```
等价于
```python
if age = 30:
    slogon = "Hello"
else:
    slogon = "Hi"
```
<a name="sKjOD"></a>
### 8、字典合并
```python
>>> a= {"a":1}
>>> b= {"b":2}
>>> {**a, **b}
{'a': 1, 'b': 2}
>>>
```
<a name="roFV3"></a>
### 9、字符串反转
```python
>>> s = "i love python"
>>> s[::-1]
'nohtyp evol i'
>>>
```
<a name="Mjunm"></a>
### 10、列表转字符串
```python
>>> s = ["i", "love", "pyton"]
>>> " ".join(s)
'i love pyton'
>>>
```
<a name="MI0S6"></a>
### 11、for else 语句
检查列表foo是否有0，有就提前结束查找，没有就是打印“未发现”
```python
found = False
for i in foo:
    if i == 0:
        found = True
        break
if not found: 
    print("未发现")
```
如果用 for else 语法来写可以省几行代码
```python
for i in foo:
    if i == 0:
        break
else:
    print("未发现")
```
<a name="5ig1I"></a>
### 12、字典推导式
```python
>>> m = {x: x**2 for x in range(5)}
>>> m
{0: 0, 1: 1, 2: 4, 3: 9, 4: 16}
>>>
```
<a name="W6tMI"></a>
### 13、用Counter查找列表中出现最多的元素
```python
>>> content = ["a", "b", "c", "a", "d", "c", "a"]
>>> from collections import Counter
>>> c = Counter(content)
>>> c.most_common(1)
[('a', 3)]
>>>
```
出现第1多的元素是a，一共出现3次， 你也可以用类似的方法找出第二多或者第三多的
<a name="1yJt7"></a>
### 14、默认值字典
给字典中的value设置为列表，普通方法
```python
>>> d = dict()
if 'a' not in d:
    d['a'] = []
d['a'].append(1)
```
使用`defaultdict`默认字典构建一个初始值为空列表的字典
```python
from collections import defaultdict
d = defaultdict(list)
d['a'].append(1)
```
<a name="1IGBd"></a>
### 15、赋值表达式
这是3.8的新特性，赋值表达式又成为海象运算符:=， 可以将变量赋值和表达式放在一行，看代码
```python
>>> import re
>>> data = "hello123world"
>>> match = re.search("(\d+)", data)  # 3
>>> if match:                         # 4
...     num = match.group(1)
... else:
...     num = None
>>> num
'123'
```
第3、4行 可以合并成一行代码
```python
>>> if match:=re.search("(\d+)", data):
...     num = match.group(1)
... else:
...     num = None
...
>>> num
'123'
```
<a name="fSQ8S"></a>
### 16、isinstance
`isinstance` 函数可用于判断实例的类型，其实第二个参数可以是多个数据类型组成的元组。例如：
```python
isinstance(x, (int, float))
# 等价于
isinstance(x, int) or isinstance(x, float)
```
类似的函数还有字符串的startswith，endswith，例如：
```python
s.startswith(('"""', "'''"))
# 等价于
s.startswith("'''") or s.startswith('"""')
```
<a name="sM3Xg"></a>
### 17、用 http.server 共享文件
```python
# python3
python3 -m http.server
# python2
python -m SimpleHTTPServer 8000
```
可以在浏览器共享文件目录，方便在局域网共享文件
<a name="tdNKT"></a>
### 18、zip 函数实现字典键值对互换
```python
>>> lang = {"python":".py", "java":".java"}
>>> dict(zip(lang.values(), lang.keys()))
{'.java': 'java', '.py': 'python'}
```
<a name="MfNnS"></a>
### 19、查找列表中出现次数最多的数字
```python
test = [1, 2, 3, 4, 2, 2, 3, 1, 4, 4, 4, 5]
>>> max(set(test), key=test.count)
4
```
<a name="k4SvM"></a>
### 20、使用 slots 节省内存
```python
class MyClass(object):
    def __init__(self, name, identifier):
        self.name = name
        self.identifier = identifier
        self.set_up()
print(sys.getsizeof(MyClass))
class MyClass(object):
    __slots__ = ['name', 'identifier']
    def __init__(self, name, identifier):
        self.name = name
        self.identifier = identifier
        self.set_up()
print(sys.getsizeof(MyClass))
# In Python 3.5
# 1-> 1016
# 2-> 888
```
<a name="3d0vS"></a>
### 21、扩展列表
```python
>>> i = ['a','b','c']
>>> i.extend(['e','f','g'])
>>> i
['a', 'b', 'c', 'e', 'f', 'g']
>>>
```
<a name="mlADc"></a>
### 22、列表负数索引
```python
>>> a = [ 1, 2, 3]
>>> a[-1]
3
```
<a name="xYMIp"></a>
### 23、列表切片
```python
>>> a = [0,1,2,3,4,5,6,7,8,9]
>>> a[3:6] # 第3个到第6个之间的元素
[3, 4, 5]
>>> a[:5] # 前5个元素
[0, 1, 2, 3, 4]
>>> a[5:] # 后5个元素
[5, 6, 7, 8, 9]
>>> a[::] # 所有元素（拷贝列表）
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> a[::2] # 偶数项
[0, 2, 4, 6, 8]
>>> a[1::2] # 奇数项
[1, 3, 5, 7, 9]
>>> a[::-1]  # 反转列表
[9, 8, 7, 6, 5, 4, 3, 2, 1, 0]
```
<a name="0l19z"></a>
### 24、二维数组变一维数组
```python
import itertools
>>> a = [[1, 2], [3, 4], [5, 6]]
>>> i = itertools.chain(*a)
>>> list(i)
[1, 2, 3, 4, 5, 6]
```
<a name="kHW54"></a>
### 25、有索引的迭代
```python
>>> a = ['Merry', 'Christmas ', 'Day']
>>> for i, x in enumerate(a):
...     print '{}: {}'.format(i, x)
...
0: Merry
1: Christmas
2: Day
```
<a name="ekjT4"></a>
### 26、列表推导式
```python
>>> le = [x*2 for x in range(10)]
>>> le  # 每个数取平方
[0, 2, 4, 6, 8, 10, 12, 14, 16, 18]
>>> le = [x for x in range(10) if x%2 == 0]
>>> le  # 获取偶数项
[0, 2, 4, 6, 8]
```
<a name="cUtHH"></a>
### 27、生成器表达式
```python
>>> ge = (x*2 for x in range(10))
>>> ge
<generator object <genexpr> at 0x01948A50>
>>> next(ge)
0
>>> next(ge)
2
>>> next(ge)
4
...
>>> next(ge)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration
```
<a name="YQGHm"></a>
### 28、集合推导式
```python
Python
>>> nums = {n**2 for n in range(10)}
>>> nums
{0, 1, 64, 4, 36, 9, 16, 49, 81, 25}
```
<a name="n5I8Y"></a>
### 29、判断key是否存在字典中
```python
>>> d = {"1":"a"}
>>> d['2']
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
KeyError: '2'
>>> '1' in d
True
>>> d['1']
'a'
>>> d.get("1")
'a'
>>> d.get("2")
>>>
```
<a name="TLjSu"></a>
### 30、装饰器
```python
from functools import wraps
def tags(tag_name):
    def tags_decorator(func):
        @wraps(func)
        def func_wrapper(name):
            return "<{0}>{1}</{0}>".format(tag_name, func(name))
        return func_wrapper
    return tags_decorator
@tags("p")
def get_text(name):
    """returns some text"""
    return "Hello " + name
print(get_text("Python"))
>>><p>Hello Python</p>
```
<a name="oa07F"></a>
### 31、字典子集
```python
>>> def sub_dicts(d, keys):
...     return {k:v for k, v in d.items() if k in keys}
...
>>> sub_dicts({1:"a", 2:"b", 3:"c"}, [1,2])
{1: 'a', 2: 'b'}
```
<a name="WzL90"></a>
### 32、反转字典
```python
>>> d = {'a': 1, 'b': 2, 'c': 3, 'd': 4}
>>>
>>> zip(d.values(), d.keys())
<zip object at 0x019136E8>
>>> z = zip(d.values(), d.keys())
>>> dict(z)
{1: 'a', 2: 'b', 3: 'c', 4: 'd'}
```
<a name="yKElG"></a>
### 33、具名元组
```python
>>> from collections import namedtuple
>>> Point = namedtuple("Point", "x,y")
>>> p = Point(x=1, y=2)
>>> p.x
1
>>> p[0]
1
>>> p.y
2
>>> p[1]
2
```
<a name="kp6fF"></a>
### 34、设置字典默认值
```python
>>> d = dict()
>>> if 'a' not in d:
...     d['a'] = []
...
>>> d['a'].append(1)
>>> d
{'a': [1]}
>>> d.setdefault('b',[]).append(2)
>>> d
{'a': [1], 'b': [2]}
>>>
```
<a name="YbNRY"></a>
### 35、有序字典
```python
>>> d = dict((str(x), x) for x in range(10))
>>> d.keys() # key 无序
dict_keys(['0', '1', '5', '9', '4', '6', '7', '8', '2', '3'])
>>> from collections import OrderedDict
>>> m = OrderedDict((str(x), x) for x in range(10))
>>> m.keys() # key 按照插入的顺序排列
odict_keys(['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'])
```
<a name="mJ71y"></a>
### 36、列表中最大最小的前n个数
```python
>>> import heapq
a = [51, 95, 14, 65, 86, 35, 85, 32, 8, 98]
>>> heapq.nlargest(5,a)
[98, 95, 86, 85, 65]
>>> heapq.nsmallest(5,a)
[8, 14, 32, 35, 51]
>>>
```
<a name="t6fKt"></a>
### 37、打开文件
```python
>>> with open('foo.txt', 'w') as f:
...     f.write("hello")
...
```
<a name="dv5xL"></a>
### 38、两个列表组合成字典
```python
list_1 = ["One","Two","Three"]
list_2 = [1,2,3]
dictionary = dict(zip(list_1, list_2))
print(dictionary)
```
<a name="XdXDt"></a>
### 39、去除列表中重复元素
```python
my_list = [1,4,1,8,2,8,4,5]
my_list = list(set(my_list))
print(my_list)
```
<a name="nW1AL"></a>
### 40、打印日历
```python
import calendar
>>> print(calendar.month(2021, 1))
    January 2021
Mo Tu We Th Fr Sa Su
             1  2  3
 4  5  6  7  8  9 10
11 12 13 14 15 16 17
18 19 20 21 22 23 24
25 26 27 28 29 30 31
```
<a name="v0ERp"></a>
### 41、匿名函数
```python
def add(a, b):
    return a+b
```
等价于
```python
>>> add = lambda a,b:a+b
>>> add(1,2)
3
```
