Python
<a name="zfDKc"></a>
### 1、处理用户的多个输入
有时需要从用户那里获得多个输入，以便使用循环或任何迭代，一般的写法如下：
```python
# bad practice码
n1 = input("enter a number : ")
n2 = input("enter a number : ")
n2 = input("enter a number : ")
print(n1, n2, n3)
```
但是更好的处理方法如下：
```python
# good practice
n1, n2, n3 = input("enter a number : ").split()
print(n1, n2, n3)
```
<a name="tmYA6"></a>
### 2、处理多个条件语句
如果在代码中需要检查多个条件语句，此时可以使用 `all()` 或`any()` 函数来实现目标。一般来说，当有多个 and 条件时使用 `all()`，当有多个 or 条件时使用 `any()`。这种用法将使代码更加清晰易读，可以方便在调试时不会遇到麻烦。<br />对于`all()`的一般例子如下：
```python
size = "lg"
color = "blue"
price = 50
# bad practice
if size == "lg" and color == "blue" and price < 100:
    print("Yes, I want to but the product.")
```
更好的处理方法如下：
```python
# good practice
conditions = [
    size == "lg",
    color == "blue",
    price < 100,
]
if all(conditions):
    print("Yes, I want to but the product.")
```
对于`any()`的一般例子如下：
```python
# bad practice
size = "lg"
color = "blue"
price = 50
if size == "lg" or color == "blue" or price < 100:
    print("Yes, I want to but the product.")
```
更好的处理方法如下：
```python
# good practice
conditions = [
    size == "lg",
    color == "blue",
    price < 100,
]
if any(conditions):
    print("Yes, I want to but the product.")
```
<a name="HXkl6"></a>
### 3、判断数字奇偶性
这很容易实现，从用户那里得到输入，将其转换为整数，检查 对数字2的求余操作，如果余数为零，则它是偶数。
```python
print('odd' if int(input('Enter a number: '))%2 else 'even')
```
<a name="Wkvbj"></a>
### 4、交换变量
在Python中如果需要交换变量的值，无需定义临时变量来操作。一般使用如下代码来实现变量交换：
```python
v1 = 100
v2 = 200
# bad practice
temp = v1
v1 = v2
v2 = temp
```
但是更好的处理方法如下：
```python
v1 = 100
v2 = 200
# good practice
v1, v2 = v2, v1
```
<a name="RzCcu"></a>
### 5、判断字符串是否为回文串
将字符串进行反转最简单的实现方式为 `[::-1]`，代码如下：
```python
print("John Deo"[::-1])
```
<a name="ZP94j"></a>
### 6、反转字符串
在Python中判断一个字符串是否为回文串，只需要使用语句
```python
string.find(string[::-1])== 0 ,示例代码如下:
v1 = "madam" # is a palindrome string
v2 = "master" # is not a palindrome string
print(v1.find(v1[::-1]) == 0) # True
print(v1.find(v2[::-1]) == 0) # False
```
<a name="eGodQ"></a>
### 7、尽量使用 Inline if statement
大多数情况下，在条件之后只有一个语句，因此使用Inline if statement 可以编写更简洁的代码。举例如下，一般的写法为：
```python
name = "ali"
age = 22
# bad practices
if name:
    print(name)
if name and age > 18:
    print("user is verified")
```
但是更好的处理方法如下：
```python
# a better approach
print(name if name else "")
""" here you have to define the else condition too"""
# good practice 
name and print(name)
age > 18 and name and print("user is verified")
```
<a name="NY07k"></a>
### 8、删除list中的重复元素
不需要遍历整个list列表来检查重复元素，可以简单地使用 `set()` 来删除重复元素，代码如下：
```python
lst = [1, 2, 3, 4, 3, 4, 4, 5, 6, 3, 1, 6, 7, 9, 4, 0]
print(lst)
unique_lst = list(set(lst))
print(unique_lst)
```
<a name="KbF96"></a>
### 9、找到list中重复最多的元素
在Python中可以使用 `max()` 函数并传递 `list.count` 作为key，即可找出列表list中重复次数最多的元素，代码如下：
```python
lst = [1, 2, 3, 4, 3, 4, 4, 5, 6, 3, 1, 6, 7, 9, 4, 0]
most_repeated_item = max(lst, key=lst.count)
print(most_repeated_item)
```
<a name="HrO8F"></a>
### 10、list 生成式
Python中最喜欢的功能就是list comprehensions，这个特性可以编写非常简洁功能强大的代码，而且这些代码读起来几乎像自然语言一样通俗易懂。举例如下：
```python
numbers = [1,2,3,4,5,6,7]
evens = [x for x in numbers if x % 2 is 0]
odds = [y for y in numbers if y not in evens]
cities = ['London', 'Dublin', 'Oslo']
def visit(city):
    print("Welcome to "+city)
for city in cities:
    visit(city)
```
<a name="UlcOU"></a>
### 11、使用`*args`传递多个参数
在Python中可以使用`*args`来向函数传递多个参数，举例如下：
```python
def sum_of_squares(n1, n2)
    return n1**2 + n2**2
print(sum_of_squares(2,3))
# output: 13
"""
what ever if you want to pass, multiple args to the function 
as n number of args. so let's make it dynamic.
""" 
def sum_of_squares(*args):
    return sum([item**2 for item in args])
# now you can pass as many parameters as you want
print(sum_of_squares(2, 3, 4))
print(sum_of_squares(2, 3, 4, 5, 6))
```
<a name="Ww138"></a>
### 12、在循环时处理下标
有时在工作中，想要获得循环中元素的下标，一般来说，比较优雅的写法如下：
```python
lst = ["blue", "lightblue", "pink", "orange", "red"]
for idx, item in enumerate(lst):
     print(idx, item)
```
<a name="QDrkn"></a>
### 13、拼接list中多个元素
在Python中一般使用`Join()` 函数来将list中所有元素拼接到一起，当然也可以在拼接的时候添加拼接符号，样例如下：
```python
names = ["john", "sara", "jim", "rock"]
print(", ".join(names))
```
<a name="rUY4A"></a>
### 14、将两个字典进行合并
在Python中可以使用`{**dict_name, **dict_name2, … }`将多个字典进行合并，样例如下：
```python
d1 = {"v1": 22, "v2": 33}
d2 = {"v2": 44, "v3": 55}
d3 = {**d1, **d2}
print(d3)
```
结果如下：
```python
{'v1': 22, 'v2': 44, 'v3': 55}
```
<a name="D4Hvo"></a>
### 15、使用两个list生成一个字典
在Python中，如果需要将两个列表中对应的元素组成字典，那么可以使用 zip 功能来方便地做到这一点。代码如下：
```python
keys = ['a', 'b', 'c']
vals = [1, 2, 3]
zipped = dict(zip(keys, vals))
```
<a name="mzmOa"></a>
### 16、字典按照value进行排序
在Python中使用`sorted()`函数来按照字典的value来对其进行排序，代码如下：
```python
d = {
    "v1": 80,
    "v2": 20,
    "v3": 40,
    "v4": 20,
    "v5": 10,
}
sorted_d = dict(sorted(d.items(), key=lambda item: item[1]))
print(sorted_d)
```
当然也可以使用`itemgetter()`来替代上述 lambda函数，代码如下：
```python
from operator import itemgetter
sorted_d = dict(sorted(d.items(), key=itemgetter(1)))
```
更进一步，也可以通过传递 `reverse=True` 对其进行降序排序：
```python
sorted_d = dict(sorted(d.items(), key=itemgetter(1), reverse=True))
```
<a name="OYiIu"></a>
### 17、Pretty print
在Python中使用`Print()`函数，有时候的输出丑陋，此时使用`pprint`可以使输出更加美观，样例如下：
```python
from pprint import pprint
data = {
    "name": "john deo",
    "age": "22",
    "address": {"contry": "canada", "state": "an state of canada :)", "address": "street st.34 north 12"},
    "attr": {"verified": True, "emialaddress": True},
}
print(data)
pprint(data)
```
输出如下：
```python
{'name': 'john deo', 'age': '22', 'address': {'contry': 'canada', 'state': 'an state of canada :)', 'address': 'street st.34 north 12'}, 'attr': {'verified': True, 'emialaddress': True}}
{'address': {'address': 'street st.34 north 12',
			 'contry': 'canada',
			 'state': 'an state of canada :)'},
 'age': '22',
 'attr': {'emialaddress': True, 'verified': True},
 'name': 'john deo'}
```
可见使用pprint函数可以让字典的输出更加容易阅读。
