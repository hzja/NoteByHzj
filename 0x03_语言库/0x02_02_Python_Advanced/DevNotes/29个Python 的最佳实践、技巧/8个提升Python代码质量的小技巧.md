Python
<a name="u0Mjx"></a>
### 1、`enumerate()`替代`range(len())`
问题：遍历一个列表，把里面小于0的值置为0。<br />遍历列表，是开发过程中经常会涉及到的一种操作。<br />大多数Python开发者都习惯于使用`range(len())`语法，这一种方式是很多教程、书籍上介绍的，因此，很多同学也就默认选择使用这种方式去遍历列表。<br />在遍历列表里，使用`enumerate()`枚举函数是一个更佳的选择，因为它可以同时获取索引和当前项，这对在很多场景下是非常实用的。而使用`range(len())`却不能兼顾索引和当前项。<br />下面看一下2种方式的对比：
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
<a name="rNnUl"></a>
### 2、列表表达式替代`for`循环
问题：求一个列表中所有制的平方。<br />如果使用比较常用的`for`循环方式是这样的：
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
<a name="qEyfN"></a>
### 3、使用`set`去重
问题：对一个列表中的元素进行去重。<br />当看到这个问题，获取有的同学会想到很多复杂的方法，遍历、字典....<br />如果使用`set`一行代码就可以实现列表元素的去重。<br />因为，`set`是一种无序的集合，所以，它会自动去除列表中的重复元素。
```python
my_list = [1,2,3,4,5,6,7,7,7]
set(my_list)
# set([1, 2, 3, 4, 5, 6, 7])
```
<a name="V610m"></a>
### 4、利用生成器节省内存
问题：如果列表中有10000个元素，该如何节省内存？<br />如果元素比较少，使用列表是一个比较好的选择。如果元素多到一定程度，那么列表就变得非常耗费内存。<br />形象的解释生成器，就如同它的名称一样，它每次只生成一个元素，当调用它的时候，它会逐步生成下一个元素。如果不调用它的话，它就是一个非常节省内存的函数。<br />下面来对比一下，
```python
import sys

my_list = [i for i in range(10000)]
print(sys.getsizeof(my_list), 'bytes') # 87616 bytes

my_gen = (i for i in range(10000))
print(sys.getsizeof(my_gen), 'bytes') # 128 bytes
```
可以看出，同样是10000个元素，在内存占用方面，使用列表是使用生成器的684.5倍。
<a name="cFywJ"></a>
### 5、使用`.get()`和`.setdefault()`访问字典
问题：访问一个字典中的值。<br />当通过`key`访问字典时，如果字典中没有这个K-V值，那么，它会报错、终止程序，并返回KeyError。<br />所以更好的方法是在字典上使用`.get()`方法。这也会返回键的值，但是如果键不可用，它不会引发键错误。相反，它返回指定的默认值，如果没有指定它，则返回None。
```python
my_dict = {'item': 'football', 'price': 10.00}
price = my_dict['count'] # KeyError!

# better:
price = my_dict.get('count', 0) # optional default value
```
使用字典进行计数是一种常用的操作。<br />在这个过程中，需要首先判断字典中是否存在`key`，然后赋给默认值，而使用`.setdefault()`可以直接给字典设定默认值。
<a name="BC3JO"></a>
### 6、使用`collections.Counter`计数
问题：统计列表字段中的元素出现次数，筛选出现频率最高的元素。<br />在项目开发过中，计数、统计频率，是经常会遇到的问题。<br />而Python自带的标准模块`collections.Counter`提供了很多好用、强大的技术方法，只需要一行代码，就可以完成很多复杂逻辑才能完成的工作。<br />例如，要统计一个列表中出现频率最高的元素，可以这样实现：
```python
from collections import Counter

my_list = [10, 10, 10, 5, 5, 2, 9, 9, 9, 9, 9, 9]
counter = Counter(my_list)

most_common = counter.most_common(2)
print(most_common) # [(9, 6), (10, 3)]
print(most_common[0]) # (9, 6)
print(most_common[0][0]) # 9
```
<a name="JFC4X"></a>
### 7、使用`**`合并字段
问题：2个字典，把元素合并到同一个字段里<br />不需要经过2层遍历去读取字典中的元素，然后在把这些元素合并到同一个字典中，只需要简单的双星号`**`就可以实现这个需求。<br />这种语法是自Python 3.5以来的新语法，在Python3.5之前无法使用。<br />下面来看一下示例：
```python
d1 = {'name': 'Alex', 'age': 25}
d2 = {'name': 'Alex', 'city': 'New York'}
merged_dict = {**d1, **d2}
print(merged_dict)
# {'name': 'Alex', 'age': 25, 'city': 'New York'}
```
<a name="dhECs"></a>
### 8、使用`if x in list`简化条件语句
问题：判断是否等于列表中的某个元素的值。<br />习惯于C/C++、Java等其他编程语言进行开发的同学，当遇到条件语句时，会选择`==`或者`!=`来进行判断。<br />如果需要判断的条件较多的话，这样需要写很长的语句，例如，
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
