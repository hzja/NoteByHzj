Python
<a name="ADIgg"></a>
## Lists Snippets
先从最常用的数据结构列表开始
<a name="wvAeS"></a>
### №1：将两个列表合并成一个字典
假设在 Python 中有两个列表，希望将它们合并为字典形式，其中一个列表的项作为字典的键，另一个作为值。这是在用 Python 编写代码时经常遇到的一个非常常见的问题<br />但是为了解决这个问题，需要考虑几个限制，比如两个列表的大小，两个列表中元素的类型，以及其中是否有重复的元素，尤其是将使用的元素作为 key 时。可以通过使用 zip 等内置函数来解决这些问题
```python
keys_list = ['A', 'B', 'C']
values_list = ['blue', 'red', 'bold']

#There are 3 ways to convert these two lists into a dictionary
#1- Using Python's zip, dict functionz
dict_method_1 = dict(zip(keys_list, values_list))

#2- Using the zip function with dictionary comprehensions
dict_method_2 = {key:value for key, value in zip(keys_list, values_list)}

#3- Using the zip function with a loop
items_tuples = zip(keys_list, values_list) 
dict_method_3 = {} 
for key, value in items_tuples: 
    if key in dict_method_3: 
        pass # To avoid repeating keys.
    else: 
        dict_method_3[key] = value
```
<a name="Q4gi2"></a>
### №2：将两个或多个列表合并为一个包含列表的列表
另一个常见的任务是当有两个或更多列表时，希望将它们全部收集到一个大列表中，其中较小列表的所有第一项构成较大列表中的第一个列表<br />例如，如果有 4 个列表 [1,2,3], ['a','b','c'], ['h','e','y'] 和 [4,5, 6]，想为这四个列表创建一个新列表；它将是 [[1,'a','h',4], [2,'b','e',5], [3,'c','y',6]]
```python
def merge(*args, missing_val = None):
    #missing_val will be used when one of the smaller lists is shorter tham the others.
    #Get the maximum length within the smaller lists.
    max_length = max([len(lst) for lst in args])
    outList = []
    for i in range(max_length):
        result.append([args[k][i] if i < len(args[k]) else missing_val for k in range(len(args))])
        return outList
```
<a name="CqzjF"></a>
### №3：对字典列表进行排序
这一组日常列表任务是排序任务，根据列表中包含的元素的数据类型，将采用稍微不同的方式对它们进行排序。
```python
dicts_lists = [
    {
        "Name": "James",
        "Age": 20,
    },
    {
        "Name": "May",
        "Age": 14,
    },
    {
        "Name": "Katy",
        "Age": 23,
    }
]

#There are different ways to sort that list
#1- Using the sort/ sorted function based on the age
dicts_lists.sort(key=lambda item: item.get("Age"))

#2- Using itemgetter module based on name
from operator import itemgetter
f = itemgetter('Name')
dicts_lists.sort(key=f)
```
<a name="igiUT"></a>
### №4：对字符串列表进行排序
经常面临包含字符串的列表，需要按字母顺序、长度或想要或应用程序需要的任何其他因素对这些列表进行排序
```python
my_list = ["blue", "red", "green"]

#1- Using sort or srted directly or with specifc keys
my_list.sort() #sorts alphabetically or in an ascending order for numeric data 
my_list = sorted(my_list, key=len) #sorts the list based on the length of the strings from shortest to longest. 
# You can use reverse=True to flip the order

#2- Using locale and functools 
import locale
from functools import cmp_to_key
my_list = sorted(my_list, key=cmp_to_key(locale.strcoll)) 
```
<a name="UFDTN"></a>
### №5：根据另一个列表对列表进行排序
有时可能需要使用一个列表来对另一个列表进行排序，因此，将有一个数字列表（索引）和一个想使用这些索引进行排序的列表
```python
a = ['blue', 'green', 'orange', 'purple', 'yellow']
b = [3, 2, 5, 4, 1]
#Use list comprehensions to sort these lists
sortedList =  [val for (_, val) in sorted(zip(b, a), key=lambda x: \
          x[0])]
```
<a name="Idb3K"></a>
### №6：将列表映射到字典
列表代码片段的最后一个任务，如果给定一个列表并将其映射到字典中，也就是说，想将列表转换为带有数字键的字典
```python
mylist = ['blue', 'orange', 'green']
#Map the list into a dict using the map, zip and dict functions
mapped_dict = dict(zip(itr, map(fn, itr)))
```
<a name="uflPf"></a>
## Dictionary Snippets
现在处理的数据类型是字典
<a name="cwex8"></a>
### №7：合并两个或多个字典
假设有两个或多个字典，并且希望将它们全部合并为一个具有唯一键的字典
```python
from collections import defaultdict
#merge two or more dicts using the collections module
def merge_dicts(*dicts):
    mdict = defaultdict(list)
    for dict in dicts:
        for key in dict:
            res[key].append(d[key])
    return dict(mdict)
```
<a name="djwMC"></a>
### №8：反转字典
一个非常常见的字典任务是如果有一个字典并且想要翻转它的键和值，键将成为值，而值将成为键<br />当这样做时，需要确保没有重复的键。值可以重复，但键不能，并确保所有新键都是可以 hashable 的
```python
my_dict = {
  "brand": "Ford",
  "model": "Mustang",
  "year": 1964
}
#Invert the dictionary based on its content
#1- If we know all values are unique.
my_inverted_dict = dict(map(reversed, my_dict.items()))

#2- If non-unique values exist
from collections import defaultdict
my_inverted_dict = defaultdict(list)
{my_inverted_dict[v].append(k) for k, v in my_dict.items()}

#3- If any of the values are not hashable
my_dict = {value: key for key in my_inverted_dict for value in my_inverted_dict[key]}
```
<a name="MgPha"></a>
## String Snippets
接下来是字符串的处理
<a name="NKo7Q"></a>
### №9：使用 f 字符串
格式化字符串可能是几乎每天都需要完成的一项任务，在 Python 中有多种方法可以格式化字符串，使用 f 字符串是比较好的选择
```python
#Formatting strings with f string.
str_val = 'books'
num_val = 15
print(f'{num_val} {str_val}') # 15 books
print(f'{num_val % 2 = }') # 1
print(f'{str_val!r}') # books

#Dealing with floats
price_val = 5.18362
print(f'{price_val:.2f}') # 5.18

#Formatting dates
from datetime import datetime;
date_val = datetime.utcnow()
print(f'{date_val=:%Y-%m-%d}') # date_val=2021-09-24
```
<a name="tiAE4"></a>
### №10：检查子串
一项非常常见的任务就是检查字符串是否在与字符串列表中
```python
addresses = ["123 Elm Street", "531 Oak Street", "678 Maple Street"]
street = "Elm Street"

#The top 2 methods to check if street in any of the items in the addresses list
#1- Using the find method
for address in addresses:
    if address.find(street) >= 0:
        print(address)

#2- Using the "in" keyword 
for address in addresses:
    if street in address:
        print(address)
```
<a name="atvjS"></a>
### №11：以字节为单位获取字符串的大小
有时，尤其是在构建内存关键应用程序时，需要知道字符串使用了多少内存
```python
str1 = "hello"
str2 = "😀"

def str_size(s):
    return len(s.encode('utf-8'))

str_size(str1)
str_size(str2)
```
<a name="vfcXu"></a>
## Input/ Output operations
最后来看看输入输出方面的代码片段
<a name="JDLco"></a>
### №12：检查文件是否存在
在数据科学和许多其他应用程序中，经常需要从文件中读取数据或向其中写入数据，但要做到这一点，需要检查文件是否存在，因此，需要确保代码不会因 IO 错误而终止
```python
#Checking if a file exists in two ways
#1- Using the OS module
import os 
exists = os.path.isfile('/path/to/file')

#2- Use the pathlib module for a better performance
from pathlib import Path
config = Path('/path/to/file') 
if config.is_file(): 
    pass
```
<a name="rNTbn"></a>
### №13：解析电子表格
另一种非常常见的文件交互是从电子表格中解析数据，使用 CSV 模块来有效地执行该任务
```python
import csv
csv_mapping_list = []
with open("/path/to/data.csv") as my_data:
    csv_reader = csv.reader(my_data, delimiter=",")
    line_count = 0
    for line in csv_reader:
        if line_count == 0:
            header = line
        else:
            row_dict = {key: value for key, value in zip(header, line)}
            csv_mapping_list.append(row_dict)
        line_count += 1
```
