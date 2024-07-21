Python
<a name="VGlcI"></a>
## 1、字符串反转
使用Python切片反转字符串：
```python
# Reversing a string using slicing

my_string = "ABCDE"
reversed_string = my_string[::-1]

print(reversed_string)

# Output
# EDCBA
```
<a name="sNT18"></a>
## 2、每个单词的第一个字母大写
使用`title`函数方法：
```python
my_string = "my name is chaitanya baweja"

# using the title() function of string class
new_string = my_string.title()

print(new_string)

# Output
# My Name Is Chaitanya Baweja
```
<a name="iEjcj"></a>
## 3、字符串查找唯一元素
使用集合的概念查找字符串的唯一元素:
```python
my_string = "aavvccccddddeee"

# converting the string to a set
temp_set = set(my_string)

# stitching set into a string using join
new_string = ''.join(temp_set)

print(new_string)

# output
# cdvae
```
<a name="W42Xr"></a>
## 4、重复打印字符串和列表n次
可以使用乘法符号（`*`）打印字符串或列表多次：
```python
n = 3 # number of repetitions

my_string = "abcd"
my_list = [1,2,3]

print(my_string*n)
# abcdabcdabcd

print(my_list*n)
# [1,2,3,1,2,3,1,2,3]
```
<a name="KVjcg"></a>
## 5、列表生成
```python
# Multiplying each element in a list by 2

original_list = [1,2,3,4]

new_list = [2*x for x in original_list]

print(new_list)
# [2,4,6,8]
```
<a name="LTVcs"></a>
## 6、变量交换
```python
a = 1
b = 2

a, b = b, a

print(a) # 2
print(b) # 1
```
<a name="irIFt"></a>
## 7、字符串拆分为子字符串列表
使用`.split()`函数：
```python
string_1 = "My name is Chaitanya Baweja"
string_2 = "sample/ string 2"

# default separator ' '
print(string_1.split())
# ['My', 'name', 'is', 'Chaitanya', 'Baweja']

# defining separator as '/'
print(string_2.split('/'))
# ['sample', ' string 2']
```
<a name="mE5Uo"></a>
## 8、多个字符串组合为一个字符串
```python
list_of_strings = ['My', 'name', 'is', 'Chaitanya', 'Baweja']

# Using join with the comma separator
print(','.join(list_of_strings))

# Output
# My,name,is,Chaitanya,Baweja
```
<a name="Uc3pW"></a>
## 9、检测字符串是否为回文
```python
my_string = "abcba"

if my_string == my_string[::-1]:
    print("palindrome")
else:
    print("not palindrome")

# Output
# palindrome
```
<a name="Qher2"></a>
## 10、统计列表中元素的次数
```python
# finding frequency of each element in a list
from collections import Counter

my_list = ['a','a','b','b','b','c','d','d','d','d','d']
count = Counter(my_list) # defining a counter object

print(count) # Of all elements
# Counter({'d': 5, 'b': 3, 'a': 2, 'c': 1})

print(count['b']) # of individual element
# 3

print(count.most_common(1)) # most frequent element
# [('d', 5)]
```
<a name="gbCKv"></a>
## 11、判断两个字符串是否为Anagrams
Anagrams的含义为两个单词中，每个英文单词（不含大小写）出现的次数相同，使用`Counter`类判断两个字符串是否为Anagrams。
```python
from collections import Counter

str_1, str_2, str_3 = "acbde", "abced", "abcda"
cnt_1, cnt_2, cnt_3  = Counter(str_1), Counter(str_2), Counter(str_3)

if cnt_1 == cnt_2:
    print('1 and 2 anagram')
if cnt_1 == cnt_3:
    print('1 and 3 anagram')

# output
# 1 and 2 anagram
```
<a name="bb3ac"></a>
## 12、使用try-except-else-block模块
`except`获取异常处理：
```python
a, b = 1,0

try:
    print(a/b)
    # exception raised when b is 0
except ZeroDivisionError:
    print("division by zero")
else:
    print("no exceptions raised")
finally:
    print("Run this always")

# output
# division by zero
# Run this always
```
<a name="MOoET"></a>
## 13、使用枚举函数得到key/value对
```python
my_list = ['a', 'b', 'c', 'd', 'e']

for index, value in enumerate(my_list):
    print('{0}: {1}'.format(index, value))

# 0: a
# 1: b
# 2: c
# 3: d
# 4: e
```
<a name="WuG0l"></a>
## 14、检查对象的内存使用情况
```python
import sys

num = 21

print(sys.getsizeof(num))

# In Python 2, 24
# In Python 3, 28
```
<a name="wuGza"></a>
## 15、合并字典
```python
dict_1 = {'apple': 9, 'banana': 6}
dict_2 = {'banana': 4, 'orange': 8}

combined_dict = {**dict_1, **dict_2}

print(combined_dict)
# Output
# {'apple': 9, 'banana': 4, 'orange': 8}
```
<a name="LbZx7"></a>
## 16、计算执行一段代码所花费的时间
使用`time`类计算运行一段代码所花费的时间：
```python
import time

start_time = time.time()
# Code to check follows
for i in range(10**5):
    a, b = 1,2
    c = a+ b
# Code to check ends
end_time = time.time()
time_taken_in_micro = (end_time- start_time)*(10**6)

print(time_taken_in_micro)

# output
# 18770.217895507812
```
<a name="Ty3Hu"></a>
## 17、列表展开
```python
from iteration_utilities import deepflatten

# if you only have one depth nested_list, use this
def flatten(l):
  return [item for sublist in l for item in sublist]

l = [[1,2,3],[3]]
print(flatten(l))
# [1, 2, 3, 3]

# if you don't know how deep the list is nested
l = [[1,2,3],[4,[5],[6,7]],[8,[9,[10]]]]

print(list(deepflatten(l, depth=3)))
# [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
<a name="nCaPT"></a>
## 18、列表采样
```python
import random

my_list = ['a', 'b', 'c', 'd', 'e']
num_samples = 2

samples = random.sample(my_list,num_samples)
print(samples)
# [ 'a', 'e'] this will have any 2 random values
```
<a name="PNOVr"></a>
## 19、数字化
将整数转化成数字列表：
```python
num = 123456

# using map
list_of_digits = list(map(int, str(num)))

print(list_of_digits)
# [1, 2, 3, 4, 5, 6]

# using list comprehension
list_of_digits = [int(x) for x in str(num)]

print(list_of_digits)
# [1, 2, 3, 4, 5, 6]
```
<a name="X5ByW"></a>
## 20、检查列表元素的唯一性
检查列表中每个元素是否为唯一的：
```python
def unique(l):
    if len(l)==len(set(l)):
        print("All elements are unique")
    else:
        print("List has duplicates")

unique([1,2,3,4])
# All elements are unique

unique([1,1,2,3])
# List has duplicates
```
