Python<br />Python3解释器内置了很多函数，可以随时调用它们，**内置函数简介如下表**，直接点击函数名可直达详细用法。
<a name="QTsGP"></a>
## 1、Python内置函数表
| `abs()`求绝对值 | `delattr()` | `hash()` | `memoryview()`返回“内存视图”对象 | `set()`构建集合 |
| --- | --- | --- | --- | --- |
| `all()`iterable 对象所有元素为真返回True | `dict()`创建一个新的字典 | `help()`返回内置帮助信息 | `min()`返回最小元素 | `setattr()` |
| `any()`iterable 对象任一元素为真返回True | `dir()`返回对象有效属性列表 | `hex()`int对象转16进制 | `next()`返回iterator的下一个元素 | `slice()`返回切片索引值 |
| `ascii()` | `divmod()` | `id()` | `object()` | `sorted()`iterable 对象排序 |
| `bin()`int对象转二进制 | `enumerate()`返回序列对象元素索引和元素组成的元组 | `input()`键盘输入 | `oct()`转八进制 | `staticmethod()` |
| `bool()`返回True 或 False | `eval()` | `int()`转整数对象 | `open()`读文件 | `str()`转字符串 |
| `breakpoint()` | `exec()` | `isinstance()`判断是否是类的一个实例方法 | `ord()`返回Unicode 字符对应码点的整数 | `sum()`iterable数字元素求和 |
| `bytearray()` | `filter()`过滤序列元素 | `issubclass()`判断是否是子类 | `pow()`求幂 | `super()` |
| `bytes()` | `float()`返回浮点数 | `iter()`返回一个迭代器 | `print()`输出到屏幕 | `tuple()`构建元组 |
| `callable()`判断对象是否可执行 | `format()`格式化输出 | `len()`返回对象元素个数 | `property()` | `type()`返回传入对象的数据类型 |
| `chr()` | `frozenset()`构建不可变集合 | `list()`构建列表 | `range()`返回数字序列 | `vars()` |
| `classmethod()`把一个方法封装成类方法，一种装饰器 | `getattr()` | `locals()` | `repr()` | `zip()`返回多个literable对象元素一一对应的新迭代对象 |
| `compile()` | `globals()` | `map()`<br />将一个函数映射到输入列表的所有元素上 | `reversed()`返回反向迭代器 | `__import__()` |
| `complex()` | `hasattr()` | `max()`最大值 | `round()`四舍五入 | 

 |


---

下面详细介绍**比较好用的内置函数**：
<a name="LtLEm"></a>
## 2、好用的内置函数
<a name="kA7lR"></a>
### enumerate
语法：`enumerate(sequence, start=0)`返回每个序列对象中元素的索引值和元素组成的元组。索引值默认从0开始，可以自定义索引开始号（-1，1等都可以）。
```python
my_list = ['apple', 'banana', 'grapes', 'pear', 'apple'] 
for i in enumerate(my_list):#默认start从0开始
    print(i)#i为一个元组

"""
(0, 'apple')
(1, 'banana')
(2, 'grapes')
(3, 'pear')
(4, 'apple')
"""
my_list = ['apple', 'banana', 'grapes', 'pear', 'apple'] 
for i in enumerate(my_list,1):#设置start为1
    print(i)

"""
(1, 'apple')
(2, 'banana')
(3, 'grapes')
(4, 'pear')
(5, 'apple')
"""
```
<a name="FCEeI"></a>
### filter
语法：`filter(function, iterable)`过滤iterable对象中的元素，返回一个由所有符合要求（function判断后为True）的元素所构成的新可迭代对象。
```python
filter_numbers = filter(lambda x: x < 0, range(-6, 6))
#range(-6, 6)中元素经过lambda x: x < 0判断为True的返回。
list(filter_numbers)#filter_numbers为一个迭代对象，加list转换为列表。

# [-6, -5, -4, -3, -2, -1]
```
<a name="p5EqT"></a>
### map
语法：`map(function, iterable, ...)`将function运用于iterable中每个元素，收集结果输出为一个新的iterable对象。匿名函数(lambdas)经常配合map函数一起使用。
```python
items = [1, 2, 3, 4, 5]
list(map(lambda x: x**2, items))#lambda x: x**2作用于items中每个元素
# [1, 4, 9, 16, 25]

list(map(lambda x, y: x + y, range(-2, 2), range(1, 5)))#传入两个iterable对象range(-2, 2), range(1, 5)
# [-1, 1, 3, 5]
```
<a name="tkAtP"></a>
### print
语法：`print(*objects, sep=' ', end='\n', file=sys.stdout, flush=False)`sep, end, file 和 flush 如果存在，必须以关键字参数的形式给出。<br />*objects，每一次要输出的多个对象，每个对象之间使用，分割；<br />sep，定义输出时，每个对象之间的分割符，默认为一个空格；<br />end，每一次输出多个对象后末尾结束符号，默认为换行符；<br />file，要写入的对象文件，默认为 `sys.stdout`<br />flush，输出是否被缓存通常决定于 file，但如果 flush 关键字参数为真值，流会被强制刷新。
```python
with open("./new1.txt","w") as f:#写入模式打开文件new1.txt
    print("haha","crycry",sep="@@",end="!",file=f,flush=True)#设置分隔符为@@，末尾符号为，输出文件为new1.txt
```
<a name="CWDsC"></a>
### range
range返回一个iterable对象。常运用于for循环中。语法：`range(start,stop,step)`，`start`,`stop`,`step`只能为整数。<br />`start`，计数开始处，默认为0开始；<br />`stop`，计数结束处，不包含stop；<br />`step`，步长，隔几个数计数一次。
```python
In [3]: list(range(5))#默认start为0忽略
Out[3]: [0, 1, 2, 3, 4]

In [4]: list(range(0,5))#默认步长为1忽略
Out[4]: [0, 1, 2, 3, 4]

In [13]: list(range(0,5,1))#步长为1
Out[13]: [0, 1, 2, 3, 4]

In [12]: list(range(-5,-1))#步长为负
Out[12]: [-5, -4, -3, -2]
```
<a name="A9etL"></a>
### round
语法：`round(float,n)`，可粗放的理解为取`float`对象的四舍五入值。
```python
In [22]: round(1.55)#默认四舍五入取整数
Out[22]: 2

In [23]: round(1.55,1)
Out[23]: 1.6

In [24]: round(0.5,1)
Out[24]: 0.5

In [25]: round(0.5)#尽然为0，而不是数学中的1
Out[25]: 0


In [26]: round(2.675,2)#这个例子更开脑洞，尽然不会返回2.68，跟浮点数的精度有关。
Out[26]: 2.67

In [27]: print('{:.20f}'.format(2.675))#看看2.675精度为20时输出的是个什么鬼
2.67499999999999982236#这就可以理解为什么不是2.68了
```
**Python实现数学中的四舍五入**
```python
def new_round(_float, _len):
    """
    Parameters
    ----------
    _float: float
    _len: int, 指定四舍五入需要保留的小数点后几位数为_len
     
    Returns
    -------
    type ==> float, 返回四舍五入后的值
    """
    if isinstance(_float, float):
        if str(_float)[::-1].find('.') <= _len:
            return(_float)
        if str(_float)[-1] == '5':
            return(round(float(str(_float)[:-1]+'6'), _len))
        else:
            return(round(_float, _len))
    else:
        return(round(_float, _len))
new_round(2.675,2)

# 2.68
```
<a name="TFuAK"></a>
### zip
语法：`zip(*iterables)`，将多个iterable对象元素从左到右一一对应拆分为元组，然后再组成一个iterable对象。
```python
list1 = range(1,5,1)
list2 = list("abcd")
list3 = range(1,3)
list(zip(list1,list2))
# [(1, 'a'), (2, 'b'), (3, 'c'), (4, 'd')]

list(zip(list1,list3))#iterable对象不一致时，取短迭代对象的长度
# [(1, 1), (2, 2)]

dict(zip(list1,list2))#zip常用来生成字典
# {1: 'a', 2: 'b', 3: 'c', 4: 'd'}
```
<a name="nyb7f"></a>
## 参考资料
[https://docs.python.org/zh-cn/3.7/library/functions.html](https://docs.python.org/zh-cn/3.7/library/functions.html)
