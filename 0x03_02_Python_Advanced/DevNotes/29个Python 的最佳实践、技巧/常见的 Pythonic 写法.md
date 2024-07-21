Python 
<a name="cgHID"></a>
### 1、变量交换
交换两个变量的值，正常都会想利用一个中间临时变量来过渡。
```python
tmp = a
a = b
b = tmp
```
能用一行代码解决的（并且不影响可读性的），决不用三行代码。
```python
a,b = b,a
```
<a name="OALre"></a>
### 2、列表推导
下面是一个非常简单的 for 循环。
```python
my_list = []
for i in range(10):
    my_list.append(i*2)
```
在一个 for 循环中，如果逻辑比较简单，不如试用一下列表的列表推导式，虽然只有一行代码，但也逻辑清晰。
```python
my_list = [i*2 for i in range(10)]
```
<a name="ch4Wj"></a>
### 3、单行表达式
上面两个案例，都将多行代码用另一种方式写成了一行代码。<br />这并不意味着，代码行数越少，就越 Pythonic 。<br />比如下面这样写，就不推荐。
```python
print('hello'); print('world')
if x == 1: print('hello,world')
if <complex comparison> and <other complex comparison>:
    # do something
```
建议还是按照如下的写法来
```python
print（'hello'）
print（'world'）
if x == 1:
    print('hello,world')
cond1 = <complex comparison>
cond2 = <other complex comparison>
if cond1 and cond2:
    # do something
```
<a name="678b9475"></a>
### 4、带索引遍历
使用 for 循环时，如何取得对应的索引，初学者习惯使用 range + len 函数
```python
for i in range(len(my_list)):
    print(i, "-->", my_list[i])
```
更好的做法是利用 enumerate 这个内置函数
```python
for i,item in enumerate(my_list):
    print(i, "-->",item)
```
<a name="465f6"></a>
### 5、序列解包
使用 `*` 可以对一个列表解包
```python
a, *rest = [1, 2, 3]
# a = 1, rest = [2, 3]
a, *middle, c = [1, 2, 3, 4]
# a = 1, middle = [2, 3], c = 4
```
<a name="fHRqT"></a>
### 6、字符串拼接
如果一个列表（或者可迭代对象）中的所有元素都是字符串对象，想要将他们连接起来，通常做法是
```python
letters = ['s', 'p', 'a', 'm']
s=""
for let in letters:
    s += let
```
更推荐的做法是使用 join 函数
```python
letters = ['s', 'p', 'a', 'm']
word = ''.join(letters)
```
<a name="4nN5c"></a>
### 7、真假判断
判断一个变量是否为真（假），新手习惯直接使用 `==` 与 True、False、None 进行对比
```python
if attr == True:
    print('True!')
if attr == None:
    print('attr is None!')
```
实际上，`""`、`[]`、`{}` 这些没有任何元素的容器都是假值，可直接使用 `if not xx` 来判断。
```
if attr:
    print('attr is truthy!')
if not attr:
    print('attr is falsey!')
```
<a name="sVk6L"></a>
### 8、访问字典元素
当直接使用 `[]` 来访问字典里的元素时，若key不存在，是会抛异常的，一般会先判断一下是否有这个 key，有再取之。
```python
d = {'hello': 'world'}
if d.has_key('hello'):
    print(d['hello'])    # prints 'world'
else:
    print('default_value')
```
更推荐的做法是使用 `get` 来取，如果没有该 key 会默认返回 None（当然也可以设置默认返回值）
```python
d = {'hello': 'world'}
print(d.get('hello', 'default_value')) # prints 'world'
print(d.get('thingy', 'default_value')) # prints 'default_value'
```
<a name="xTJVr"></a>
### 9、操作列表
下面这段代码，会根据条件过滤过列表中的元素
```python
a = [3, 4, 5]
b = []
for i in a:
    if i > 4:
        b.append(i)
```
实际上可以使用列表推导或者高阶函数 filter 来实现
```python
a = [3, 4, 5]
b = [i for i in a if i > 4]
# Or:
b = filter(lambda x: x > 4, a)
```
除了 filter 之外，还有 map、reduce 这两个函数也很好用
```python
a = [3, 4, 5]
b = map(lambda i: i + 3, a)
# b: [6,7,8]
```
<a name="fafff9d5"></a>
### 10、文件读取
文件读取是非常常用的操作，在使用完句柄后，是需要手动调用 close 函数来关闭句柄的
```python
fp = open('file.txt')
print(fp.read())
fp.close()
```
如果代码写得太长，即使知道需要手动关闭句柄，却也会经常会漏掉。因此推荐养成习惯使用 `with open` 来读写文件，上下文管理器会自动执行关闭句柄的操作
```python
with open('file.txt') as fp:
    for line in fp.readlines():
        print(line)
```
<a name="m2QC6"></a>
### 11、代码续行
将一个长度较长的字符串放在一行中，是很影响代码可读性的(下面代码可向左滑动)
```python
long_string = 'For a long time I used to go to bed early. Sometimes, when I had put out my candle, my eyes would close so quickly that I had not even time to say “I’m going to sleep.”'
```
稍等注重代码可读性的人，会使用三个引号 `\`来续写
```python
long_string = 'For a long time I used to go to bed early. ' \
              'Sometimes, when I had put out my candle, ' \
              'my eyes would close so quickly that I had not even time to say “I’m going to sleep.”'
```
也可以使用括号包裹 `()`
```python
long_string = (
    "For a long time I used to go to bed early. Sometimes, "
    "when I had put out my candle, my eyes would close so quickly "
    "that I had not even time to say “I’m going to sleep.”"
)
```
导包的时候亦是如此
```python
from some.deep.module.inside.a.module import (
    a_nice_function, another_nice_function, yet_another_nice_function)
```
<a name="FF4ze"></a>
### 12、显式代码
有时候出于需要，会使用一些特殊的魔法来使代码适应更多的场景不确定性。
```python
def make_complex(*args):
    x, y = args
    return dict(**locals())
```
但若非必要，请不要那么做。无端增加代码的不确定性，会让原先本就动态的语言写出更加动态的代码。
```python
def make_complex(x, y):
    return {'x': x, 'y': y}
```
<a name="6d4688d1"></a>
### 13、使用占位符
对于暂不需要，却又不得不接收的的变量，请使用占位符
```python
filename = 'foobar.txt'
basename, _, ext = filename.rpartition('.')
```
<a name="ea14e3cc"></a>
### 14、链式比较
对于下面这种写法
```python
score = 85
if score > 80 and score < 90:
    print("良好")
```
其实还有更好的写法
```python
score = 85
if 80 < score < 90:
    print("良好")
```
如果理解了上面的链式比较操作，那么应该知道为什么下面这行代码输出的结果是 False
```python
>>> False == False == True 
False
```
<a name="JTdJf"></a>
### 15、三目运算
对于简单的判断并赋值
```python
age = 20
if age > 18:
    type = "adult"
else:
    type = "teenager"
```
其实是可以使用三目运算，一行搞定。
```python
age = 20  
b = "adult" if age > 18 else "teenager"
```
<a name="xaztq"></a>
### 16、`enumerate()`替代`range(len())`**
遍历列表，是开发过程中经常会涉及到的一种操作。大多数Python开发者都习惯于使用`range(len())`语法，这一种方式是很多教程、书籍上介绍的，因此，很多同学也就默认选择使用这种方式去遍历列表。<br />在遍历列表里，使用`enumerate()`枚举函数是一个更佳的选择，因为它可以同时获取**索引**和**当前项**，这对在很多场景下是非常实用的。而使用`range(len())`却不能兼顾索引和当前项。<br />下面看一下2种方式的对比：
```python
data = [1, 2, -3, -4]
# range(len())
for i in range(len(data)):
    if data[i] < 0:
        data[i] = 0
# enumerate()
data = [1, 2, -3, -4]
for idx, num in enumerate(data):
    if num < 0:
        data[idx] = 0
```
<a name="BU0iH"></a>
### 17、利用生成器节省内存**
如果元素比较少，使用列表是一个比较好的选择。如果元素多到一定程度，那么列表就变得非常耗费内存。<br />形象的解释生成器，就如同它的名称一样，它每次只生成一个元素，当调用它的时候，它会逐步生成下一个元素。如果不调用它的话，它就是一个非常节省内存的函数。<br />下面来对比一下，
```python
import sys
my_list = [i for i in range(10000)]
print(sys.getsizeof(my_list), 'bytes') # 87616 bytes
my_gen = (i for i in range(10000))
print(sys.getsizeof(my_gen), 'bytes') # 128 bytes
```
可以看出，同样是10000个元素，在内存占用方面，使用列表是使用生成器的**684.5**倍。
<a name="NNrV6"></a>
### 18、使用`.get()`和`.setdefault()`访问字典**
通过`key`访问字典时，如果字典中没有这个`K-V`值，那么，它会报错、终止程序，并返回KeyError。<br />所以更好的方法是在字典上使用`.get()`方法。这也会返回键的值，但是如果键不可用，它不会引发键错误。相反，它可以返回指定的默认值，如果没有指定它，则返回None。
```python
my_dict = {'item': 'football', 'price': 10.00}
price = my_dict['count'] # KeyError!
# better:
price = my_dict.get('count', 0) # optional default value
```
使用字典进行计数是一种常用的操作。<br />在这个过程中，需要首先判断字典中是否存在`key`，然后赋给默认值，而使用`.setdefault()`可以直接给字典设定默认值。
<a name="5odMF"></a>
### 19、使用`collections.Counter`计数**
在项目开发过中，计数、统计频率，是经常会遇到的问题。<br />而Python自带的标准模块`collections.Counter`提供了很多好用、强大的技术方法，只需要一行代码，就可以完成很多复杂逻辑才能完成的工作。<br />例如要统计一个列表中出现频率最高的元素，可以这样实现：
```python
from collections import Counter
my_list = [10, 10, 10, 5, 5, 2, 9, 9, 9, 9, 9, 9]
counter = Counter(my_list)
most_common = counter.most_common(2)
print(most_common) # [(9, 6), (10, 3)]
print(most_common[0]) # (9, 6)
print(most_common[0][0]) # 9
```
<a name="eHKB0"></a>
### 20、使用`**`合并字段
有2个字典，把元素合并到同一个字段里。不需要经过2层遍历去读取字典中的元素，然后在把这些元素合并到同一个字典中，只需要简单的双星号`**`就可以实现这个需求。<br />这种语法是自Python 3.5以来的新语法，在Python3.5之前无法使用。<br />下面来看一下示例：
```python
d1 = {'name': 'Alex', 'age': 25}
d2 = {'name': 'Alex', 'city': 'New York'}
merged_dict = {**d1, **d2}
print(merged_dict)
# {'name': 'Alex', 'age': 25, 'city': 'New York'}
```
<a name="39Eme"></a>
### 21、使用`if x in list`简化条件语句
**问题：**判断是否等于列表中的某个元素的值。<br />习惯于C/C++、Java等其他编程语言进行开发的同学，当遇到条件语句时，会选择`==`或者`!=`来进行判断。<br />如果需要判断的条件较多的话，这样需要写很长的语句，例如，
```python
colors = ["red", "green", "blue"]
c = "red"
if c == "red"or c == "green"or c == "blue":
    print("is main color")
```
而在Python中，对于条件语句进行了很大程度的简化，可以使用`in`来解决这个问题，只需要简短的一行代码就可以完成。
```python
colors = ["red", "green", "blue"]
c = "red"
if c in colors:
    print("is main color")
```
<a name="KeJZf"></a>
### 22、列表表达式替代`for`循环
求一个列表中所有制的平方。如果使用比较常用的`for`循环方式是这样的：
```python
squares = []
for i in range(10):
    squares.append(i*i)
```
而使用列表表达式，是下面这样的：
```python
squares = [i*i for i in range(10)]
```
一行代码就可以实现`for`循环3行代码才能实现的功能。<br />列表表达式非常强大，它还可以结合条件语句进行使用。<br />不过，不要过度使用列表表达式。因为，它使代码变得简单的同时也增加了阅读理解的成本。所以，在一些复杂的语句不建议使用列表表达式。
<a name="dSP6A"></a>
### 23、使用`set`去重
对一个列表中的元素进行去重。当看到这个问题，或许有的同学会想到很多复杂的方法，遍历、字典....<br />如果使用`set`一行代码就可以实现列表元素的去重。<br />因为，`set`是一种无序的集合，所以，它会自动去除列表中的重复元素。
```python
my_list = [1,2,3,4,5,6,7,7,7]
set(my_list)
# set([1, 2, 3, 4, 5, 6, 7])
```
