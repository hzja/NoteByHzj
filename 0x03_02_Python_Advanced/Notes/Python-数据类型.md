Python 数据类型 bool 元组 字符串 字典 集合
<a name="oAr9E"></a>
# `bool`布尔类型
布尔类型取值只有两种情况，表示真假值，即 yes or no

- Python中布尔值使用常量`True` 和 `False`来表示；注意大小写
- 比较运算符`<` `>` `==` 等返回的类型就是`bool`类型；
- 布尔类型通常在 `if` （判断）和 `while` （循环）语句中应用

下面是一个简单的案例演示
```python
print(True==1)
print(False==0)
print(2>1)
print(0>3)
'''
True
True
True
False
'''
```
<a name="QJhFq"></a>
# `string`字符串

- 和列表操作类似
- 切片 索引
- **特点是不可变性**

Python中单引号和双引号使用完全相同。使用三引号(`'''`或`"""`)可以指定一个多行字符串。
```python
print('data science')
print("data science")
print('''this is data science
Learning together''')
print("""this is data science
Learning together""")
'''
data science
data science
this is data science
Learning together
this is data science
Learning together
'''
```

- 转义符 `''`：反斜杠可以用来转义，使用r可以让反斜杠不发生转义。
- 如 `r"data science \n"`
- 则`\n`会显示，并不是换行。
- 按字面意义级联字符串，如`"this " "is " "data science"`会被自动转换为`this is data science`。

转义字符演示
```python
print('data\nscience')  #表示换行
print(r'data\nscience')  #加r则无特殊意义，正常输出
print('this','is','Fcant')  #逐个输出字符串
'''
data
science
data\nscience
this is Fcant
'''
```
字符串可以用 '`+`'运算符连接在一起，用 '`*`' 运算符重复。
```python
str='Fcant'
print(str+'你好')  #连接两个字符串
print(str*5)  #打印该字符串5次
#Fcant你好
#FcantFcantFcantFcantFcant
```
Python 中的字符串有两种索引方式，从左往右以 0 开始，从右往左以 -1<br />索引演示：
```python
str='Fcant'
print(str[1])  #打印该字符串中第一个元素
print(str[2:-1])  #打印该字符串中第三个元素到倒数第二个元素
#a
#taScienc
```
Python中的字符串不能改变。
```python
str='Fcant'
str[0]='s'
print(str[0])
---------------------------------------------------------------------------
TypeError                                 Traceback (most recent call last)
<ipython-input-31-f432ac1ed2d2> in <module>
     1 str='Fcant'
----> 2 str[0]='s'
     3 print(str[0])
TypeError: 'str' object does not support item assignment
```
Python 没有单独的字符类型，一个字符就是长度为 1 的字符串。字符串的截取的语法格式如下：`**变量[头下标:尾下标:步长]**`
```python
str='welcome to the Fcant'
print(str[0:13:2])
#wloet h
```
<a name="IjirE"></a>
# 列表
List（列表） 是 Python 中使用最频繁的数据类型。 

- 列表可以完成大多数集合类的数据结构实现。列表中元素的类型可以不相同，它支持数字，字符串甚至可以包含列表（所谓嵌套）。
- 列表是写在方括号 `[]` 之间、用逗号分隔开的元素列表。
- 和字符串一样，列表同样可以被索引和截取，列表被截取后返回一个包含所需元素的新列表。
- 列表截取的语法格式如下：`**变量[头下标：尾下标]**`
- 列表中的元素是可以改变的，很多操作和字符串类似

下面演示部分列表常用的操作
```python
a=['123','Fcant','4345','f','3']
b=['ppp']
print(a[2])  #输出第三个元素
a[0]=321  #讲列表a中的第一个元素替换
print(a[0])  #打印a中第一个元素
print(a[0:3])  #打印a中第一个到第三个元素。注意：python语言取左不取右
print(a[2:-1])  #打印a中第三个到倒数第二个元素
print(a+b)  #将列表a，b相加
print(b*3)  #将列表b乘以三
'''
4345
321
[321, 'Fcant', '4345']
['4345', 'f']
[321, 'Fcant', '4345', 'f', '3', 'ppp']
['ppp', 'ppp', 'ppp']
'''
```
<a name="dUOjT"></a>
## 列表的特点

- 可存放多个值
- 有序。按照从左到右的顺序定义列表元素，下标从0开始按顺序访问
- 可修改指定索引位置的值，是可变的。
<a name="f3Bv1"></a>
## 列表常见的操作
<a name="25e371cf"></a>
### 增加 `append`
```python
names=['data','science']
names
#['data', 'science']
names.append('excellent')
names
#['data', 'science', 'excellent']
```
<a name="25ff43e4"></a>
### 插入 `insert`/`extend`
```python
names.insert(2,"数据科学")
names
#['data', 'science', '数据科学', 'excellent']
name=['数据科学','学姐','学长']
names.extend(name)
names
#['data', 'science', '数据科学', 'excellent', '数据科学', '学姐', '学长']
```
<a name="7GARn"></a>
### 嵌套 `insert`
```python
names
#['data', 'science', '数据科学', 'excellent', '数据科学', '学姐', '学长']
names.insert(2,[0,0,4])
names
#['data', 'science', [0, 0, 4], '数据科学', 'excellent', '数据科学', '学姐', '学长']
```
<a name="lZ0AT"></a>
### 直接删除 `del`
```python
names
#['data', 'science', [0, 0, 4], '数据科学', 'excellent', '数据科学', '学姐', '学长']
del names[2]
names
#['data', 'science', '数据科学', 'excellent', '数据科学', '学姐', '学长']
```
<a name="yrkV7"></a>
### 删除最后一个元素 `pop`
```python
names
#['data', 'science', '数据科学', 'excellent', '数据科学', '学姐', '学长']
names.pop()  #默认删除最后一个元素并返回被删除的值
names.pop(1)
names
#['data', '数据科学', 'excellent', '数据科学', '学姐']
```
<a name="FMLmN"></a>
### 删除指定的元素`remove`
```python
names.remove('Shon')
names
#['data2', '数据科学', 'excellent', '数据科学']
```
<a name="4762e385"></a>
### 清空 `clear`
```python
name
#['数据科学', '学姐', '学长']
name.clear()
name
#[]
```
<a name="8347a927"></a>
### 修改
```python
names
#['data', '数据科学', 'excellent', '数据科学', '学姐']
names[0]='data2'
names[-1]='Shon'
names
#['data2', '数据科学', 'excellent', '数据科学', 'Shon']
```
<a name="HlmgP"></a>
### 查操作 `index`/`count`
```python
names
#['data2', '数据科学', 'excellent', '数据科学', 'Shon']
names.index('excellent')  #返回从左开始匹配到的第一个excellent的索引
#2
names.count('excellent')   #返回元素“excellent”的个数
#1
'data2' in names
#True
names
#['data2', '数据科学', 'excellent', '数据科学', 'Shon']
```
<a name="3d7a8ef0"></a>
### 切片操作
就像切面包，一片一片的把列表切开<br />切片操作是列表操作很重要的一部分。
```python
a=['123','987','3','dc','真的','r']
a
#['123', '987', '3', 'dc', '真的', 'r']
a[0:2]
#['123', '987']
a[4:-1]
#['真的']
a[0:-1]
#['123', '987', '3', 'dc', '真的']
a[4:]
#['真的', 'r']
#步长
a[0:4:2]
#['123', '3']
a[0::2]
#['123', '3', '真的']
a[-1:-6:-1]
#['r', '真的', 'dc', '3', '987']
a[0::]
#['123', '987', '3', 'dc', '真的', 'r']
```
<a name="4977b458"></a>
### 反转
```python
a[::-1]
#['r', '真的', 'dc', '3', '987', '123']
a[::-2]
#['r', 'dc', '987']
```
<a name="c360e994"></a>
### 排序
```python
b=[4,90,54,23,44,456,1,4]
b
#[4, 90, 54, 23, 44, 456, 1, 4]
b.sort()
b
#[1, 4, 4, 23, 44, 54, 90, 456]
m='数据科学'
m
#'数据科学'
m[::-1]
#'学科据数'
```
<a name="f375cebb"></a>
### 循环列表
for循环
```python
for i in b:
   print(i)
'''
1
4
4
23
44
54
90
456
'''
```
<a name="cda9f200"></a>
# 元组
`tuple`和`list`十分相似，但是`tuple`是不可变的，即不能修改`tuple`，元组通过圆括号中用逗号分割的项定义。

- 支持索引和切片操作
- 可以使用 `in`查看一个元素是否在`tuple`中。
- 空元组`()`
- 只含有一个元素的元组`("a",)` #需要加个逗号

优点：`tuple`比`list`速度快；对不需要修改的数据进行‘写保护’，可以是代码更安全
:::info
tuple与list可以相互转换，使用内置的函数`list()`和`tuple()`。
:::
```python
l = [1, 2, 3]
print( l )# [1, 2, 3]
t = tuple(l)
print(t) # (1, 2, 3)
l = list(t)
print (l) #[1, 2, 3]
```
元组最通常的用法是用在打印语句，如下例：
```python
name = "Runsen"
age = 20
print( "Name: %s; Age: %d") % (name, age)
# Name: Runsen; Age: 20
```
<a name="cUmQC"></a>
## 元组的方法
```python
#索引、长度、切片、循环
a=(2,3,4,5,'32','你好')
a
#[2, 3, 4, 5, '32', '你好']
a[4]  #元组中第5个元素
#'32'
len(a)  #元组a的长度
#6
a[0:3]  #取该元组中第一个到第三个的值
#[2, 3, 4]
```
<a name="9023003b"></a>
### 遍历元组
```python
for i in a:
   print(i)
2
3
4
5
32
你好
```
<a name="iX2Kl"></a>
### 查询判断
```python
'32' in a  #判断字符串32是否在元组a中
#True
32 in a  #判断整数32是否在元组a中
#False
```
**如果不可变的元组中包含可变的元素（例如列表），则元组可变**
```python
tuple_=(3,4,2,'元组',[3,4,'d','2'],'32')
tuple_
#(3, 4, 2, '元组', [3, 4, 'd', '2'], '32')
#更改数据会报错
tuple_[3]=1
tuple_
---------------------------------------------------------------------------
TypeError                                 Traceback (most recent call last)
<ipython-input-9-13fb7bda7784> in <module>
----> 1 tuple_[3]=1
     2 tuple_
TypeError: 'tuple' object does not support item assignment
#更改元组中的列表部分
tuple_[4][0]='数据'
tuple_
#(3, 4, 2, '元组', ['数据', 4, 'd', '2'], '32')
```
<a name="5pmpe"></a>
### `count(value)`
返回元组中值为value的元素的个数
```python
t = (1, 2, 3, 1, 2, 3)
print (t.count(2) )# 2
```
<a name="oxRNt"></a>
### `index(value, [start, [stop]])`
返回列表中第一个出现的值为value的索引，如果没有，则异常 `ValueError`
```
t = (1, 2, 3, 1, 2, 3)
print( t.index(3) )# 2
try:
    print (t.index(4))
except ValueError as V:
    print(V)  # there is no 4 in tuple
```
<a name="SWWtZ"></a>
# 字典
**关键点：**

- 健与值用冒号：分开
- 项与项用逗号，分开

**特性：**

- key-value结构；
- key必须是不可变类型，唯一性；
- 可存放多个value，可修改，不唯一；
- 无序；
- 查询速度快，不受dict大小影响；
<a name="AnslW"></a>
## 字典的基本操作
<a name="8cf36547"></a>
### 字典的创建
<a name="7teyt"></a>
#### 方式一
```python
#直接给出key和value
info={'name':'小王',
    'num':'20192812',
    'gender':'女'}
info
#{'name': '小王', 'num': '20192812', 'gender': '女'}
```
<a name="mLiLq"></a>
#### 方式二
```python
#key和value分别赋值
keys=[1,2,3,4,5]
{}.fromkeys(keys)
#{1: None, 2: None, 3: None, 4: None, 5: None}
{}.fromkeys(keys,100)
#{1: 100, 2: 100, 3: 100, 4: 100, 5: 100}
```
<a name="hEo6N"></a>
#### 方式三：`dict.fromkeys(seq,val=None)`
创建并返回一个新字典，以序列seq中元素做字典的键，val为字典所有键对应的初始值(默认为None)
```python
l = [1, 2, 3]
t = (1, 2, 3)
d3 = {}.fromkeys(l)
print (d3) #{1: None, 2: None, 3: None}
d4 = {}.fromkeys(t, "default")
print(d4) #{1: 'default', 2: 'default', 3: 'default'}
```
<a name="07ygk"></a>
### 字典添加键值
`d[key] = value`，如果字典中已有`key`，则为其赋值为`value`，否则添加新的键值对`key/value`；
```python
d = {}
d["1"] = "one"
d["2"] = "two"
d["3"] = "three"
```
<a name="OyPVF"></a>
#### `setdefault(key,[default])`
若字典中有key，则返回vlaue值，若没有key，则加上该key，值为default，默认None
```python
d = {1:"one", 2:"two", 3:"three"}
print (d.setdefault(1))  #one
print (d.setdefault(5))  #None
print( d)  #{1: 'one', 2: 'two', 3: 'three', 5: None}
print (d.setdefault(6, "six")) #six
print (d)  #{1: 'one', 2: 'two', 3: 'three', 5: None, 6: 'six'}
```
<a name="II1sE"></a>
### 字典的增加
```python
#字典的增加
info['age']='20'
info
#{'name': '小王', 'num': '20192812', 'gender': '女', 'age': '20'}
```
<a name="wvsMV"></a>
### 字典的复制 `copy()`
<a name="u55k4"></a>
#### `copy()`
返回字典的一个副本(浅复制)
```python
d1 = {"1":"one", "2":"two"}
d2 = d1.copy()
print( d2 )#{'1': 'one', '2': 'two'}
print(d1 == d2) # True
print(d1 is d2) # False
```
浅复制值相同，但是对象不同，内存地址不同。
<a name="yBpS3"></a>
#### `update(dict2)`
把dict2的元素加入到dict中去，键字重复时会覆盖dict中的键值
```python
d = {1:"one", 2:"two", 3:"three"}
d2 = {1:"first", 4:"forth"}
d.update(d2)
print (d)  #{1: 'first', 2: 'two', 3: 'three', 4: 'forth'}
```
<a name="RgrmH"></a>
### 字典的删除 `pop`/`del`/`clear`
<a name="d65dc774"></a>
#### `pop(key, [default])`
若字典中key键存在，删除并返回`dict[key]`，若不存在，且未给出`default`值，引发KeyError异常
```python
d9 = {1:"one", 2:"two", 3:"three"}
print (d9.pop(1) )#one
print( d9) #{2: 'two', 3: 'three'}
print( d9.pop(5, None)) #None
try:
    d9.pop(5)  # raise KeyError
except KeyError, ke:
    print ( "KeyError:", ke) #KeyError:5
```
<a name="uRjTH"></a>
#### `del d[key]`
使用`del d[key]`可以删除键值对；
```python
del d["3"]
```
<a name="nKic1"></a>
#### `popitem()`
删除任意键值对，并返回该键值对，如果字典为空，则产生异常KeyError
```
d10 = {1:"one", 2:"two", 3:"three"}
print (d10.popitem() ) #(1, 'one')
print (d10)  #{2: 'two', 3: 'three'}
```
<a name="Wwo0x"></a>
#### `clear()`
删除字典中所有元素
```python
info.clear()
info
#{}
```
<a name="btJ5w"></a>
### 字典的修改 `update`
```python
#字典的修改
dic={'name':'小王',
    'num':'20192898',
    'gender':'男'}
dic2={'name':'小王',
    'num':'20192812',
    'gender':'女',
    'age':'20'}
dic2
#{'name': '小王', 'num': '20192812', 'gender': '女', 'age': '20'}
dic.update(dic2)   #合并后重合的key被覆盖
dic
#{'name': '小王', 'num': '20192812', 'gender': '女', 'age': '20'}
```
<a name="Vkgyp"></a>
### 查操作
```python
#查操作
'age'in dic  #判断该元素是否在字典中
#True
'age2'in dic
#False
dic.get('name')  #读取name的值
#'小王'
dic.keys()  #获取所有key值
#dict_keys(['name', 'num', 'gender', 'age'])
dic.values()  #获取所有value值
#dict_values(['小王', '20192812', '女', '20'])
dic.items()  #以列表返回可遍历的(键, 值) 元组数组
#dict_items([('name', '小王'), ('num', '20192812'), ('gender', '女'), ('age', '20')])
```
<a name="lSK8o"></a>
#### `get(key,[default])`
返回字典`dict`中键`key`对应值，如果字典中不存在此键，则返回`default`的值(`default`默认值为`None`)
```python
d5 = {1:"one", 2:"two", 3:"three"}
print (d5.get(1) )#one
print (d5.get(5)) #None
print (d5.get(5, "test") )#test
```
<a name="d9dddbb8"></a>
#### `has_key(key)`
判断字典中是否有键key
```python
d6 = {1:"one", 2:"two", 3:"three"}
print( d6.has_key(1) ) #True
print (d6.has_key(5))  #False
```
<a name="bbYeU"></a>
#### `items()`
返回一个包含字典中(键, 值)对元组的列表
```python
d7 = {1:"one", 2:"two", 3:"three"}
for item in d7.items():
    print (item)
#(1, 'one')
#(2, 'two')
#(3, 'three')
for key, value in d7.items():
    print ("%s -- %s" % (key, value))
#1 -- one
#2 -- two
#3 -- three
```
<a name="jRoHX"></a>
#### `keys()`
返回一个包含字典中所有键的列表
```python
d8 = {1:"one", 2:"two", 3:"three"}
for key in d8.keys():
    print (key)
#1
#2
#3
```
<a name="qeTAG"></a>
#### `values()`
返回一个包含字典中所有值的列表
```python
d8 = {1:"one", 2:"two", 3:"three"}
for value in d8.values():
    print( value)
#one
#two
#three
```
<a name="bHmyx"></a>
#### `viewitems()`
返回一个view对象，（key, value）pair的列表，类似于视图。优点是，如果字典发生变化，view会同步发生变化。在 迭代过程中，字典不允许改变，否则会报异常
```python
d = {1:"one", 2:"two", 3:"three"}
for key, value in d.viewitems():
    print ("%s - %s" % (key, value))
#1 - one
#2 - two
#3 - three
```
<a name="10EFj"></a>
#### `viewkeys()`
返回一个view对象，key的列表
```python
d = {1:"one", 2:"two", 3:"three"}
for key in d.viewkeys():
    print( key)
#1
#2
#3
```
<a name="8b45a89d"></a>
#### `viewvalues()`
返回一个view对象，value的列表
```python
d = {1:"one", 2:"two", 3:"three"}
for value in d.viewvalues():
    print (value)
#one
#two
#three
```
<a name="FYQLQ"></a>
### 字典的循环 三种
**第一种**
```python
for i in dic:
   print(i)       #遍历Key值
'''
name
num
gender
age
'''
```
**第二种**
```python
for i in dic.items():  #遍历字典
   print(i)
'''
('name', '小王')
('num', '20192812')
('gender', '女')
('age', '20')
'''
```
**第三种**
```python
for i,j in dic.items():  #遍历字典
   print(i,j)
'''
name 小王
num 20192812
gender 女
age 20
'''
```
<a name="wD8Ph"></a>
## 集合
<a name="a3PXB"></a>
### 集合的特点

- 里面的元素不可变，不能在集合中存放列表或字典，而字符串、元组、数字等不可变类型可以存放
- 天生去重，在集合中无法存放相同的元素
- 无序。不能像列表一样通过索引来标记其元素在列表中的位置，例如{1，2，3}和{2，1，3}是同一个集合。
<a name="o1iPn"></a>
### 集合的操作
<a name="aTts8"></a>
#### 集合的创建
```python
#集合的创建
s={1,2,3,'32','data','文字','d','f'}
s
#{1, 2, 3, '32', 'd', 'data', 'f', '文字'}
type(s)  #查看类型
#set
```
<a name="YVCf3"></a>
#### 列表转集合
```python
#列表转集合
l=[1,2,3,'df','ffd']
l
#[1, 2, 3, 'df', 'ffd']
set(l)
#{1, 2, 3, 'df', 'ffd'}
```
<a name="Bp1UJ"></a>
#### 集合的新增 `add`
```python
#新增
s.add('科学')
s
#{1, 2, 3, '32', 'd', 'data', 'f', '文字', '科学'}
```
<a name="Dz61E"></a>
#### 集合的删除 `discard`/`remove`/`pop`
**法一**
```python
#删除
s.discard('32')
s
#{1, 2, 3, 'd', 'data', 'f', '文字', '科学'}
```
**法二**
```python
s.remove(2)
s
#{1, 3, 'd', 'data', 'f', '文字', '科学'}
```
**法三**
```python
s.pop()
#1
s
#{3, 'd', 'data', 'f', '文字', '科学'}
```
<a name="RPGxB"></a>
#### 集合的循环
```python
#循环
for i in s:
   print(i)
'''
3
科学
文字
data
d
f
'''
```
<a name="Jh4Ex"></a>
#### 集合的关系运算
```python
#集合的关系运算
s={1,2,3,'32','data','文字','d','f'}
s2={'science','学长','学员','e','f',3,2,6,7}
```
**交集**
```python
s&s2
#{2, 3, 'f'}
```
**并集**
```python
#并集
s | s2
#{1, 2, 3, '32', 6, 7, 'd', 'data', 'e', 'f', 'science', '学员', '学长', '文字'}
```
**差集**
```python
#差集
s-s2
#{1, '32', 'd', 'data', '文字'}
```
**对称差集**
```python
#对称差集
s ^ s2
#{1, '32', 6, 7, 'd', 'data', 'e', 'science', '学员', '学长', '文字'}
```
**判断两个集合是否包含相同的元素，不包含返回true，包含返回false**
```python
s.isdisjoint(s2)
#False
s.isdisjoint({123})
#True
```
**子集**
```python
#是否为子集的判断
s.issubset(s2)
#False
{1,2,3}.issubset({1,2,3,4,5})
#True
```
<a name="wfcjw"></a>
# 序列
序列类型是指容器内的元素从0开始的索引顺序访问，一次可以访问一个或者多个元素；列表、元组和字符串都是序列。序列的三个主要特点是

- 索引操作符和切片操作符
- 索引可以得到特定元素
- 切片可以得到部分序列

**索引操作符和切片操作符**
```python
numbers = ["zero", "one", "two", "three", "four"]
  
print (numbers[1] )# one
print (numbers[-1] )# four
#print( numbers[5]) # raise IndexError
print (numbers[:]) # ['zero', 'one', 'two', 'three', 'four']
print (numbers[3:]) # ['three', 'four']
print (numbers[:2]) # ['zero', 'one']
print (numbers[2:4] )# ['two', 'three']
print (numbers[1:-1] )# ['one', 'two', 'three']
```
切片操作符中的第一个数（冒号之前）表示切片开始的位置，第二个数（冒号之后）表示切片到哪里结束。<br />如果不指定第一个数，Python就从序列首开始。如果没有指定第二个数，则Python会停止在序列尾。<br />注意，返回的序列从开始位置 开始 ，刚好在结束位置之前 结束。即开始位置是包含在序列切片中的，而结束位置被排斥在切片外。可以用负数做切片。负数用在从序列尾开始计算的位置。
