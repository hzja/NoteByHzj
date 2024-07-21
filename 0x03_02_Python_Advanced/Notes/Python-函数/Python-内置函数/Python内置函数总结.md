Python 函数
<a name="gDumm"></a>
### 1、`abs()`
绝对值或复数的模
```python
In [1]: abs(-6)
Out[1]: 6
```
<a name="RJQJy"></a>
### 2、`all()`
接受一个迭代器，如果迭代器的所有元素都为真，那么返回True，否则返回False
```python
In [2]: all([1,0,3,6])
Out[2]: False

In [3]: all([1,2,3])
Out[3]: True
```
<a name="HEGqe"></a>
### 3、`any()`
接受一个迭代器，如果迭代器里有一个元素为真，那么返回True，否则返回False
```python
In [4]: any([0,0,0,[]])
Out[4]: False

In [5]: any([0,0,1])
Out[5]: True
```
<a name="cl9kd"></a>
### 4、`ascii()`
调用对象的`repr()` 方法，获得该方法的返回值
```python
In [30]: class Student():
    ...:     def __init__(self,id,name):
    ...:         self.id = id
    ...:         self.name = name
    ...:     def __repr__(self):
    ...:         return 'id = '+self.id +', name = '+self.name

In [33]: print(xiaoming)
id = 001, name = xiaoming

In [34]: ascii(xiaoming)
Out[34]: 'id = 001, name = xiaoming'
```
<a name="gAr63"></a>
### 5、`dict()`
创建数据字典
```python
In [92]: dict()
Out[92]: {}

In [93]: dict(a='a',b='b')
Out[93]: {'a': 'a', 'b': 'b'}

In [94]: dict(zip(['a','b'],[1,2]))
Out[94]: {'a': 1, 'b': 2}

In [95]: dict([('a',1),('b',2)])
Out[95]: {'a': 1, 'b': 2}
```
<a name="g9Jj6"></a>
### 6、`dir()`
不带参数时返回当前范围内的变量，方法和定义的类型列表；带参数时返回参数的属性，方法列表。
```python
In [96]: dir(xiaoming)
Out[96]:
['__class__',
 '__delattr__',
 '__dict__',
 '__dir__',
 '__doc__',
 '__eq__',
 '__format__',
 '__ge__',
 '__getattribute__',
 '__gt__',
 '__hash__',
 '__init__',
 '__init_subclass__',
 '__le__',
 '__lt__',
 '__module__',
 '__ne__',
 '__new__',
 '__reduce__',
 '__reduce_ex__',
 '__repr__',
 '__setattr__',
 '__sizeof__',
 '__str__',
 '__subclasshook__',
 '__weakref__',

 'name']
```
<a name="KCPEd"></a>
### 7、`divmod()`
分别取商和余数
```python
In [97]: divmod(10,3)
Out[97]: (3, 1)
```
<a name="aTmPV"></a>
### 8、`isinstance(object, classinfo)`
判断object是否为类classinfo的实例，是返回true
```python
In [20]: class Student():
    ...:     ...:     def __init__(self,id,name):
    ...:     ...:         self.id = id
    ...:     ...:         self.name = name
    ...:     ...:     def __repr__(self):
    ...:     ...:         return 'id = '+self.id +', name = '+self.name
    ...:

In [21]: xiaoming = Student('001','xiaoming')

In [22]: isinstance(xiaoming,Student)
Out[22]: True
```
<a name="sWjLs"></a>
### 9、issubclass(class, classinfo)
如果class是classinfo类的子类，返回True：
```python
In [27]: class undergraduate(Student):
    ...:     def studyClass(self):
    ...:         pass
    ...:     def attendActivity(self):
    ...:         pass
    ...:

In [28]: issubclass(undergraduate,Student)
Out[28]: True

In [29]: issubclass(object,Student)
Out[29]: False

In [30]: issubclass(Student,object)
Out[30]: True
```
如果class是classinfo元组中某个元素的子类，也会返回True
```python
In [26]: issubclass(int,(int,float))
Out[26]: True
```
<a name="M3TKg"></a>
### 10、`iter(object, sentinel)`
返回一个可迭代对象，sentinel可省略
```python
In [72]: lst = [1,3,5]

In [73]: for i in iter(lst):
    ...:     print(i)
    ...:
1
3
5
```
sentinel 理解为迭代对象的哨兵，一旦迭代到此元素，立即终止：
```python
In [81]: class TestIter(object):
    ...:         def __init__(self):
    ...:             self.l=[1,3,2,3,4,5]
    ...:             self.i=iter(self.l)
    ...:         def __call__(self):  #定义了__call__方法的类的实例是可调用的
    ...:             item = next(self.i)
    ...:             print ("__call__ is called,which would return",item)
    ...:             return item
    ...:         def __iter__(self): #支持迭代协议(即定义有__iter__()函数)
    ...:             print ("__iter__ is called!!")
    ...:             return iter(self.l)
    ...:

In [82]:     t = TestIter()
    ...:     t1 = iter(t, 3)
    ...:     for i in t1:
    ...:         print(i)
    ...:
__call__ is called,which would return 1
1
__call__ is called,which would return 3
```
<a name="nh788"></a>
### 11、`max(iterable,*[, key, default])`
返回最大值：
```python
In [99]: max(3,1,4,2,1)
Out[99]: 4

In [100]: max((),default=0)
Out[100]: 0

In [89]: di = {'a':3,'b1':1,'c':4}
In [90]: max(di)
Out[90]: 'c'

In [102]: a = [{'name':'xiaoming','age':18,'gender':'male'},{'name':'
     ...: xiaohong','age':20,'gender':'female'}]
In [104]: max(a,key=lambda x: x['age'])
Out[104]: {'name': 'xiaohong', 'age': 20, 'gender': 'female'}
```
<a name="H5zIS"></a>
### 12、`min(iterable,*[, key, default])`
返回最小值
<a name="oNycc"></a>
### 13、`memoryview(obj)`
返回由给定实参创建的“内存视图”对象， Python 代码访问一个对象的内部数据，只要该对象支持 缓冲区协议 而无需进行拷贝
<a name="fKBCa"></a>
### 14、`next(iterator,[, default])`
返回可迭代对象的下一个元素
```python
In [129]: it = iter([5,3,4,1])

In [130]: next(it)
Out[130]: 5

In [131]: next(it)
Out[131]: 3

In [132]: next(it)
Out[132]: 4

In [133]: next(it)
Out[133]: 1

In [134]: next(it,0) #迭代到头，默认返回值为0
Out[134]: 0

In [135]: next(it)
----------------------------------------------------------------------
StopIteration                        Traceback (most recent call last)
<ipython-input-135-bc1ab118995a> in <module>
----> 1 next(it)

StopIteration:
```
<a name="DDnUq"></a>
### 15、`object()`
返回一个没有特征的新对象。object 是所有类的基类。
```python
In [137]: o = object()

In [138]: type(o)
Out[138]: object
```
