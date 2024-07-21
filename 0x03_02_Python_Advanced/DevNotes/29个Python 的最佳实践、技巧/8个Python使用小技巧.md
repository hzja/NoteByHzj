Python
<a name="VU2JX"></a>
### 1、 使用下划线分割数值较大的数字
平时输入比较大的数值时，都会先习惯性多数几次位数以便确保数值正确：<br />请在三秒钟内告诉我下面num的值是多少？
```
num = 1000000000
```
使用下划线分割后，更适合人类辨认：
```python
num1 = 1_000_000_000 # 欧美划分习惯
num2 = 10_0000_0000  # 中国划分习惯
print(num1,num2,end=" ")
```
输出结果：
```
1000000000 1000000000
```
<a name="VxzSE"></a>
### 2、列表变成不可变的“列表”
`frozenset`是Python的一种内置类型，和普通集合set的最大区别是它是可散列、不可变的集合，不能修改其中的元素，也因此可以作为集合的元素、字典的键。
```python
f_set = frozenset([1, 2, 3, 4])
for n in f_set:
    print(n)
```
输出结果：
```
1
2
3
4
```
再变回正常列表：
```python
f_set = frozenset([1, 2, 3, 4])
# for n in f_set:
#     print(n)

lst = list(f_set)
lst.append(5)
print(lst)
```
输出结果：
```
[1, 2, 3, 4, 5]
```
<a name="m7yQA"></a>
### 3、交换字典的键、值位置
```python
dictionary = {"a": 1, "b": 2, "c": 3}
reversed_dictionary = {j: i for i, j in dictionary.items()}
print(reversed_dictionary)
```
输出结果：
```
{1: 'a', 2: 'b', 3: 'c'}
```
注意：字典的键是唯一的，如果交换前某几个item的值是相同的，则最后的item的键为准。
```python
dictionary = {"a": 1, "b": 2, "c": 3, "d":3}  #  "c": 3, "d":3 值相同
reversed_dictionary = {j: i for i, j in dictionary.items()}
print(reversed_dictionary)
```
输出结果：
```
{1: 'a', 2: 'b', 3: 'd'}
```
<a name="HG8gd"></a>
### 4、生成特定范围的奇/偶数
```python
for n in range(10, 20, 2):
    print(n)
```
输出结果：
```
10
12
14
16
18
```
<a name="dS0Nm"></a>
### 5、访问字典中不存在的键时，避免报错
定义了一个字典但取值的时候发现该键不存在，代码就会报错，<br />用这个方式可以避免这类错误的产生：
```python
person_info = {"name": "Maishu", "sex": "Male"}
print(person_info["age"])
```
错误如下。
```
print(person_info["age"])
          ~~~~~~~~~~~^^^^^^^
KeyError: 'age'
```
加上`defaultdict()`，
```python
from collections import defaultdict

person_info = defaultdict(str)
person_info['name'] = "Maishu"
person_info['sex'] = "Male"
print(person_info["age"])
```
`defaultdict(param)` 相当于给字典设置一个默认值

- `defaultdict(int)`， 则默认value值为`0`
- `defaultdict(str)`， 则默认value值为`''`
- `defaultdict(set)`， 则默认value值为`set()`
- `defaultdict(list)`， 则默认value值为`[]`
<a name="NjIJp"></a>
### 6、寻找模块安装的位置
本地存在多个Python版本（虚拟环境）时，可以用这个方法确定当前Python的模块是在哪个文件夹中，查看源码也方便。
```python
import requests

print(requests)
```
输出结果：
```
<module 'requests' from 'D:\\Python311\\Lib\\site-packages\\requests\\__init__.py'>
```
<a name="B0qlP"></a>
### 7、获取列表的所有中间元素
```python
_,_,*elements,_,_ = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
print(elements)
```
输出结果：
```
[3, 4, 5, 6, 7, 8, 9, 10]
```
`_,`可以多写几个来确定占位。
<a name="hNKNf"></a>
### 8、使用列表中的元素定义多个变量
```python
my_variables = [1, 2, 3, 4, 5]
a, b, c, d, e = my_variables

print(a, b, c, d, e, end="\n")
```
输出结果：
```
1 2 3 4 5
```
好像元组也可以这么操作。
```python
my_variables = 1, 2, 3, 4, 5
print(type(my_variables))

a,b,c,d,e = my_variables
print(a,b,c,d,e,end="\n")
```
输出结果：
```
1 2 3 4 5
```
