Python
<a name="fbfXP"></a>
### 目录
1、列表推导式<br />2、枚举函数<br />3、通过函数返回多个值<br />4、像在数学中一样比较多个数字<br />5、将字符串转换为字符串列表<br />6、For-Else 方法<br />7、从列表中找到n个最大/小的元素<br />8、函数参数以列表值传递<br />9、重复整个字符串<br />10、从列表中找到元素的索引<br />11、在同一行中打印多个元素<br />12、分隔大数字以易于阅读<br />13、反转列表的切片<br />14、"`is`" 和 "`==`" 的区别<br />15、在一行代码中合并 2 个字典<br />16、识别字符串是否以特定字母开头<br />17、获取字符的Unicode<br />18、获取字典的键值对<br />19、在数学运算中使用布尔值<br />20、在列表的特定位置添加值<br />21、过滤器`filter()`函数<br />22、创建没有参数边界的函数<br />23、一次迭代两个或多个列表<br />24、改变句子中字母的大小写<br />25、检查对象使用的内存大小<br />26、`Map()` 函数<br />27、反转整个字符串<br />28、代码块的执行时间<br />29、删除字符串的左侧或右侧字符<br />30、在元组或列表中查找元素的索引<br />31、清空列表或集合中元素<br />32、连接两个集合<br />33、根据频率对列表的值排序<br />34、从列表中删除重复值<br />35、列表中元素连接为句子<br />36、一次从函数返回多个值<br />37、找出两个列表之间的差异<br />38、将两个列表合并为一个字典<br />39、执行字符串表示的代码<br />40、字符串格式化
<a name="xXs9w"></a>
### 1、列表推导式
列表的元素可以在一行中循环遍历。
```python
numbers = [1, 2, 3, 4, 5, 6, 7, 8]
even_numbers = [number for number in numbers if number % 2 == 0]
print(even_numbers)
```
输出
```python
[1,3,5,7]
```
同样可以使用字典、集合和生成器来完成推导式。
```python
dictionary = {'first_num': 1, 'second_num': 2,
              'third_num': 3, 'fourth_num': 4}
oddvalues = {key: value for (key, value) in dictionary.items() if value % 2 != 0}
print(oddvalues)Output: {'first_num': 1, 
                         'third_num': 3}
```
<a name="XiuG8"></a>
### 2、枚举函数
Enumerate (枚举) 是一个很有用的函数，用于迭代对象，如列表、字典或文件。该函数生成一个元组，其中包括通过对象迭代获得的值以及循环计数器（从 0 的起始位置）。当希望根据索引编写代码时，循环计数器就派上用场了。<br />来看一个示例，其中第一个和最后一个元素会被区别对待。
```python
sentence = 'Just do It'
length = len(sentence)
for index, element in enumerate(sentence):
    print('{}: {}'.format(index, element))
    if index == 0:
        print('The first element!')
    elif index == length - 1:
        print('The last element!')
```
输出
```python
0: J
The first element!
1: u
2: s
3: t
4:
5: d
6: o
7:
8: I
9: t
The last element!
```
也可以使用 enumerate 函数枚举文件。在下面的示例中，在跳出循环之前打印 CSV 文件的前 10 行。并且可以在任何文件上使用该函数。
```python
with open('heart.csv') as f:
    for i, line in enumerate(f):
        if i == 10:
            break
        print(line)
```
<a name="cvJ5P"></a>
### 3、通过函数返回多个值
在设计函数时，经常希望返回多个值。在这里介绍两种典型的方法。
<a name="tecii"></a>
#### 方法一
最简单的是返回一个元组。这种方法通常只在有两个或三个值要返回时使用。但当元组中有更多值时，很容易忘记项目的顺序。<br />下面的代码部分是一个示例函数，它根据学生的 ID 号将学生的名字和姓氏作为元组返回。
```python
# 返回元组
def get_student(id_num):
    if id_num == 0:
        return 'Hello', 'Fcant'
    elif id_num == 1:
        return '猴子', '小'
    else:
        raise Exception('没有学生的id是: {}'.format(id_num))
```
当使用数字 0 调用函数时，注意到它返回一个具有两个值的元组：'Taha' 和 'Nate' 。
```python
Student = get_student(0)
print('名字: {}, 姓氏: {}'.format(Student[0], 
       Student[1]))
```
输出
```python
名字：Hello，姓氏：Fcant
```
<a name="lxY2n"></a>
#### 方法二
第二个选择是返回字典。因为字典是键值对，可以对返回的值进行命名，这比元组更直观。<br />方法二的实现方式和方法一一样，只是返回一个字典。
```python
# 返回字典
def get_data(id_num):
    if id_num == 0:
        return {'first_name': 'Hello', 
                'last_name': 'Fcant', 
                'title': '数据', 
                'department': 'A', 
                'date_joined': '20201001'}
    elif id_num == 1:
        return {'first_name': '猴子', 
                'last_name': '小', 
                'title': '机器学习研习院',
                'department': 'B', 
                'date_joined': '20201019'}
    else:
        raise Exception('没有员工的id是: {}'.format(id_num))
```
当结果是字典时，通过键引用特定值会更容易。正在调用 `id_num = 0` 的函数。
```python
employee = get_data(0)
print('first_name: {}, nlast_name: {}, ntitle: {}, ndepartment: {}, ndate_joined: {}'.format(
  
    employee['first_name'], employee['last_name'], 
    employee['title'], employee['department'], 
    employee['date_joined']))
```
输出
```python
first_name: Hello,  
last_name: Fcant,  
title: 数据,  
department: A,  
date_joined: 20201001
```
<a name="aYWNA"></a>
### 4、像在数学中一样比较多个数字
如果有一个值并希望将其与其他两个值进行比较，可以使用以下基本数学表达式： `1<x<30`<br />这就是在小学学到的代数表达式。同样的语句也可以在 Python 中使用。应该用过如下的比较方式：
```python
1<x and x<30
```
在 Python 中实现上述比较方式的另一种比较方法是：`1<x<30`
```python
x = 5
print(1<x<30)
```
输出
```python
True
```
<a name="vcMK1"></a>
### 5、将字符串转换为字符串列表
假设将函数的输入作为字符串，但它应该是这样的列表：
```python
输入 = [[1, 2, 3], [4, 5, 6]]
```
其实无需处理复杂的正则表达式，只需导入模块`'ast'`并调用其函数`literal_eval`：
```python
import ast
def string_to_list(string):
    return ast.literal_eval(string)
string = "[[1, 2, 3],[4, 5, 6]]"
my_list = string_to_list(string)
print(my_list)
```
输出
```python
[[1, 2, 3], [4, 5, 6]]
```
<a name="ji5d5"></a>
### 6、For-Else 方法
此方法用于在列表上应用循环。通常，当遍历应用的列表时，可以使用 for 循环。但是在这种方法中，可以在循环中传递一个 else 条件，这种情况极为罕见。其他编程语言不支持这种方法。<br />看看它在一般情况下是如何工作的：如果要检查列表中是否有偶数。
```python
number_List = [1, 3, 7, 9,8]

for number in number_List:
    if number % 2 == 0:
        print(number)
        break
    else:
    print("No even numbers!!")
```
输出
```python
8
```
如果找到偶数，则将打印该数字，并且 else 部分将不会执行，因为传递了一个 break 语句。如果 break 语句从不执行，则 else 块将执行。
<a name="ueKEz"></a>
### 7、从列表中找到N个最大/小的元素
通过使用`'heapq'`模块，可以从列表中找到 n-largest 或 n-smallest 元素。
```python
import heapq
numbers = [80, 25, 68, 77, 95, 88, 30, 55, 40, 50]
print(heapq.nlargest(5, numbers))
print(heapq.nsmallest(5, numbers))
```
输出
```python
[95, 88, 80, 77, 68] [25, 30, 40, 50, 55]
```
<a name="l2HUi"></a>
### 8、函数参数以列表值传递
可以使用'*'访问列表的所有元素。
```python
def Summation(*arg):
    sum = 0
    for i in arg:
        sum += i
    return sum
result = Summation(*[8,5,10,7])
print(result)
```
输出
```python
30
```
<a name="F499r"></a>
### 9、重复整个字符串
只需将字符串乘以一个数字，即希望字符串重复的次数。
```python
value = "数据FCANT"
print(value * 3)  
print("-" *31)
```
输出
```python
数据FCANT数据FCANT数据FCANT
----------------------------    
```
<a name="EK4pE"></a>
### 10、从列表中找到元素的索引
使用`".index"`从列表中查找元素的索引。
```python
cities= ['Vienna', 'Amsterdam', 'Paris', 'Berlin']
print(cities.index('Berlin'))
```
输出
```python
3
```
<a name="Unb0V"></a>
### 11、在同一行中打印多个元素
```python
print("数据", end="")
print("FCANT")
print("数据", end=" ")
print("FCANT")
print('Data', 'science', 'Machine', 
      'Learning', sep=', ')
```
输出
```python
数据FCANT
数据 FCANT
Data, science, Machine, Learning
```
<a name="qmmNX"></a>
### 12、分隔大数字以易于阅读
有时，当尝试打印一个大数字时，传递整个数字会非常混乱且难以阅读。然而可以使用下划线，使其易于阅读，打印结果并不会显示下划线。
```python
print(5_000_000_000_000)
print(7_543_291_635)
```
输出
```python
5000000000000  
7543291635
```
<a name="wBkJT"></a>
### 13、反转列表的切片
当对列表进行切片时，需要传递最小、最大和步长。要以相反的顺序进行切片，只需要传递一个负步长。
```python
sentence = "数据FCANT Hello"
print(sentence[21:0:-1])
# 向前走两步
print(sentence[21:0:-2])
```
输出
```python
>>> sentence = "数据FCANT Hello"
>>> print(sentence[21:0:-1])
olleH TNACF据
>>> print(sentence[21:0:-2])
olHTAF
```
<a name="CeyoN"></a>
### 14、`"is"` 和 `"=="` 的区别
如果要检查两个变量是否指向同一个对象，则需要使用`'is'`。但是如果要检查两个变量是否相同，则需要使用`'=='`。
```python
list1 = [7, 9, 4]
list2 = [7, 9, 4]
print(list1 == list2) 
print(list1 is list2)
list3 = list1
print(list3 is list1)
```
输出
```python
True  
False  
True
```
第一个语句是 True，因为 list1 和 list2 都持有相同的值，所以它们是相等的。第二个语句为 False，因为值指向内存中的不同变量，第三个语句为 True，因为 list1 和 list3 都指向内存中的公共对象。
<a name="tTo81"></a>
### 15、在一行代码中合并 2 个字典
```python
first_dct = {"London": 1, "Paris": 2}
second_dct = {"Tokyo": 3, "Seol": 4}
merged = {**first_dct, **second_dct}
print(merged)
```
输出
```python
{‘London’: 1, ‘Paris’: 2, ‘Tokyo’: 3, ‘Seol’: 4}
```
<a name="CEACz"></a>
### 16、识别字符串是否以特定字母开头
如果需要知道字符串是否以特定字母开头，那么可以使用常见的索引方法。但是也可以使用一个名为 `'startswith'` 的函数，它会告诉你一个字符串是否以特定的单词开头。
```python
sentence = "Data Studio"
print(sentence.startswith("d"))
print(sentence.startswith("o"))
```
输出
```python
False
True
```
<a name="FNzso"></a>
### 17、获取字符的Unicode
如果需要知道一个字符的 `Unicode`，那么需要使用一个名为`'ord'`的函数，并在函数中传递想知道其 `Unicode` 的字符。
```python
print(ord("T"))
print(ord("A")) 
print(ord("h")) 
print(ord("a"))
```
输出
```python
84  
65  
104  
97
```
<a name="DzOm5"></a>
### 18、获取字典的键值对
如果想以不同的方式访问字典的键和值，可以使用名为`'items()'`的函数来实现。
```python
cities = {'London': 1, 'Paris': 2, 'Tokyo': 3, 'Seol': 4}
for key, value in cities.items():
    print(f"Key: {key} and Value: {value}")
```
输出
```python
Key: London and Value: 1  
Key: Paris and Value: 2  
Key: Tokyo and Value: 3  
Key: Seol and Value: 4
```
<a name="FaKy0"></a>
### 19、在数学运算中使用布尔值
`False`被视为 0，`True`被视为 1
```python
x = 9
y = 3
outcome = (x - False)/(y * True)
print(outcome)
```
输出
```python
3.0
```
<a name="UEe83"></a>
### 20、在列表的特定位置添加值
如果想使用`'append'` 功能向列表添加值，但它会在列表的最后位置添加一个值。如果想在列表的特定位置添加值怎么办？可以使用名为 `'insert'` 的函数在列表的特定位置插入值。<br />语法
```python
list_name.insert(position, value)
cities = ["London", "Vienna", "Rome"]
cities.append("Seoul")
print("After append:", cities)
cities.insert(0, "Berlin")
print("After insert:", cities)
```
输出
```python
After append: ['London', 'Vienna', 
               'Rome', 'Seoul']   
After insert: ['Berlin', 'London', 
               'Vienna', 'Rome', 'Seoul']
```
<a name="sCkXL"></a>
### 21、过滤器 `filter()` 函数
过滤器`filter()`函数的工作顾名思义。它通过内部传递的特定函数来过滤特定的迭代器。并返回一个迭代器。<br />语法
```python
filter(function, iterator)
mixed_number = [8, 15, 25, 30,34,67,90,5,12]
filtered_value = filter(lambda x: x > 20, mixed_number)
print(f"Before filter: {mixed_number}") 
print(f"After filter: {list(filtered_value)}")
```
输出
```python
Before filter:[8, 15, 25, 30, 34, 67, 90, 5, 12] 
After filter:[25, 30, 34, 67, 90]
```
<a name="nMYlC"></a>
### 22、创建没有参数边界的函数
可以无需在意参数而创建一个函数。可以在调用函数时传递任意数量的参数。
```python
def multiplication(*arguments):
    mul = 1
    for i in arguments:
        mul = mul * i
    return mul
print(multiplication(3, 4, 5))
print(multiplication(5, 8, 10, 3))
print(multiplication(8, 6, 15, 20, 5))
```
输出
```python
60  
1200  
72000
```
<a name="w4dFy"></a>
### 23、一次迭代两个或多个列表
可以使用 `enumerate` 函数迭代单个列表，但是当有两个或多个列表时，也可以使用`'zip()'`函数迭代它们。
```python
capital = ['Vienna', 'Paris', 'Seoul',"Rome"]
countries = ['澳大利亚', '法国', '韩国',"意大利"]
for cap, country in zip(capital, countries):
    print(f"{cap} 是 {country} 的首都")
```
输出
```python
Vienna 是 澳大利亚 的首都  
Paris 是 法国 的首都  
Seoul 是 韩国 的首都  
Amsterdam 是 意大利 的首都
```
<a name="BxTND"></a>
### 24、改变句子中字母的大小写
如果想改变字母的大小写，即大写到小写，小写到大写，那么可以使用一个叫做`'swapcase'`的函数实现这一功能。
```python
sentence = "Data FCANT"
changed_sen = sentence.swapcase()
print(changed_sen)
```
输出
```python
dATA fcant
```
<a name="OZ8Zj"></a>
### 25、检查对象使用的内存大小
要检查对象使用的内存，首先导入 `'sys'` 库，然后使用该库中名为 `'getsizeof'` 的方法。它将返回对象使用的内存大小。
```python
import sys
mul = 5*6
print(sys.getsizeof(mul))
```
输出
```python
28
```
<a name="KWG0F"></a>
### 26、`Map()` 函数
'`Map()`' 函数用于特定的功能应用到一个给定的迭代器。<br />语法
```python
map(function, iterator)
values_list = [8, 10, 6, 50]
quotient = map(lambda x: x/2, values_list)
print(f"Before division: {values_list}")
print(f"After division: {list(quotient)}")
```
输出
```python
Before division：[8, 10, 6, 50]   
After division：[4.0, 5.0, 3.0, 25.0]
```
<a name="w57Rz"></a>
### 27、反转整个字符串
要反转字符串，可以使用切片方法。
```python
value = "TNACF ataD"
print("Reverse is:", value[::-1])
```
输出
```python
Reverse is: Data FCANT
```
<a name="m4Z4t"></a>
### 28、代码块的执行时间
当训练机器学习或深度学习模型，或者只是运行一个代码块时，获取需要检查运行代码块花费了多少时间。可以选择在代码块的顶部使用一个魔法函数`'%%time'`。它将显示运行代码块所花费的时间。
```python
%%time
sentence = " Data FCANT."
changed_sen = sentence.swapcase()
print(changed_sen)
```
输出
```python
  dATA fcant.  
 CPU times: user 145 µs, sys: 578 µs, 
 total: 723 µs  
 Wall time: 1.04 ms
```
<a name="ZM5S4"></a>
### 29、删除字符串的左侧或右侧字符
有两个函数称为 `'rstrip()'` 和 `'lstrip()'`,`'rstrip()`' 用于从字符串右侧删除某个字符，而 `'lstrip()'` 用于从字符串左侧删除某个字符。两个函数的默认值都是空格。但是可以传递特定字符以将它们从字符串中删除。
```python
sentence1 = "Data Fcant     "
print(f"After removing the right space: {sentence1.rstrip()}") 
sentence2 = "        Data Fcant"
print(f"After removing the left space: {sentence2.lstrip()}")
sentence3 = "Data Fcant .,bbblllg"
print("After applying rstrip:", sentence3.rstrip(".,blg"))
```
输出
```python
After removing the right space: Data Fcant  
After removing the left space: Data Fcant  
After applying rstrip: Data Fcant
```
可以通过在其中运行 for 循环来计算元素在列表中出现的次数。但是可以更轻松地做到这一点，只需调用名为`'count'`的列表中的方法即可。
```python
cities= ["Amsterdam", "Berlin", "New York", 
         "Seoul", "Tokyo", "Paris", "Paris",
         "Vienna","Paris"]
print("Paris appears", cities.count("Paris"), "times in the list")
```
输出
```python
Paris appears 3 times in the list
```
<a name="CE3Bv"></a>
### 30、在元组或列表中查找元素的索引
只需在该元组或列表上调用一个名为`'index'`的简单方法，就可以在该元组或列表中找到元素的索引。
```python
cities_tuple = ("Berlin", "Paris", 5, "Vienna", 10)
print(cities_tuple.index("Paris")) 
cities_list = ['Vienna', 'Paris', 'Seoul',"Amsterdam"]
print(cities_list.index("Amsterdam"))
```
输出
```python
1  
3
```
<a name="BakMB"></a>
### 31、清空列表或集合中元素
可以通过在列表或集合上应用称为'clear'的方法从列表或集合中删除所有元素。
```python
cities_list = ['Vienna', 'Paris', 'Seoul',"Amsterdam"]
print(f"Before removing from the list: {cities_list}")
cities_list.clear()
print(f"After removing from the list: {cities_list}")
cities_set = {'Vienna', 'Paris', 'Seoul',"Amsterdam"}
print(f"Before removing from the set: {cities_set}")
cities_set.clear()
print(f"After removing from the set: {cities_set}")
```
输出
```python
Before removing from the list: ['Vienna', 
              'Paris', 'Seoul', 'Amsterdam']  
After removing from the list: []  
Before removing from the set: {'Seoul', 
              'Amsterdam', 'Paris', 'Vienna'}  
After removing from the set: set()
```
<a name="AMwTD"></a>
### 32、连接两个集合
要加入两个集合，可以应用称为`union()`的方法。它将加入应用该方法的两个列表。
```python
set1 = {'Vienna', 'Paris', 'Seoul'}
set2 = {"Tokyo", "Rome",'Amsterdam'}
print(set1.union(set2))
```
输出
```python
{'Seoul', 'Rome', 'Paris', 
 'Amsterdam', 'Tokyo', 'Vienna'}
```
<a name="c2Fuj"></a>
### 33、根据频率对列表的值排序
首先，使用名为 `collections` 的模块中的`'counter'`来测量每个值的频率，然后对计数器的结果应用名为`'most_common'`的方法，根据频率对列表中的值进行排序。
```python
from collections import Counter
count = Counter([7, 6, 5, 6, 8, 6, 6, 6])
print(count)
print("根据频率对值进行排序:", count.most_common())
```
输出：
```python
Counter({6: 5, 7: 1, 5: 1, 8: 1})  
根据频率对值进行排序:[(6, 5), (7, 1), (5, 1), (8, 1)]
```
<a name="wGdRO"></a>
### 34、从列表中删除重复值
首先将列表转换为集合，这将删除重复值，因为集合不包含重复值。然后再次将集合转换为列表，这样就可以轻松地从列表中删除重复的值。
```python
cities_list = ['Vienna', 'Paris', 'Seoul',
               "Amsterdam","Paris","Amsterdam", "Paris"]
cities_list = set(cities_list)
print("从列表中删除重复值后:", list(cities_list))
```
输出
```python
从列表中删除重复值后:['Vienna', 'Amsterdam', 
                   'Seoul', 'Paris']
```
<a name="Kkgmi"></a>
### 35、列表中元素连接为句子
通过使用称为`'join'`的方法，可以连接列表的所有单个元素并生成单个字符串或句子。
```python
words_list = ["Hello", "World", "Fcant"]
print(" ".join(words_list))
```
输出
```python
HelloWorldFcant
```
<a name="cgIUb"></a>
### 36、一次从函数返回多个值
可以在 python 中做到一次从一个函数返回多个值。
```python
def calculation(number):
    mul = number*2
    div = number/2
    summation = number+2
    subtract = number-2
    return mul, div, summation, subtract
mul, div, summation, subtract = calculation(10)
print("乘法:", mul)
print("除法:", div)
print("加法:", summation)
print("减法:", subtract)
```
输出
```python
乘法: 20  
除法: 5.0   
加法: 12  
减法: 8
```
<a name="onCKp"></a>
### 37、找出两个列表之间的差异
首先，将列表转换为集合，然后对这些集合应用称为`'symmetric_difference'`的方法。这将返回这两个列表之间的差异。
```python
cities_list1 = ['Vienna', 'Paris', 'Seoul',"Amsterdam", "Berlin", "London"]
cities_list2 = ['Vienna', 'Paris', 'Seoul',"Amsterdam"]
cities_set1 = set(cities_list1)
cities_set2 = set(cities_list2)
difference = list(cities_set1.symmetric_difference(cities_set2))
print(difference)
```
输出
```python
['Berlin', 'London']
```
<a name="wk6Q8"></a>
### 38、将两个列表合并为一个字典
首先，在这两个列表上应用 zip 函数，然后将 zip 函数的输出转换为字典。将两个列表转换为一个字典就是这么容易。
```python
number = [1, 2, 3]
cities = ['维也纳', '巴黎', '首尔']
result = dict(zip(number, cities))
print(result)
```
输出
```python
{1:'维也纳', 2:'巴黎', 3:'首尔'}
```
<a name="EpbJf"></a>
### 39、执行字符串表示的代码
将字符串编译成python能识别或可执行的代码，也可以将文字读成字符串再编译。
```python
s  = "print('helloworld')"
r = compile(s,"<string>", "exec")
exec(r)
```
输出
```python
helloworld
```
<a name="sLVq5"></a>
### 40、字符串格式化
格式化输出字符串，`format(value, format_spec)`实质上是调用了`value`的`format(format_spec)`方法。
```python
print("i am {0},age{1}".format("tom",18)) 
```
输出
```python
i am tom,age18
```
| 3.1415926 | {:.2f} | 3.14 | 保留小数点后两位 |
| --- | --- | --- | --- |
| 3.1415926 | {:+.2f} | 3.14 | 带符号保留小数点后两位 |
| -1 | {:+.2f} | -1 | 带符号保留小数点后两位 |
| 2.71828 | {:.0f} | 3 | 不带小数 |
| 5 | {:0>2d} | 5 | 数字补零 (填充左边, 宽度为2) |
| 5 | {:x<4d} | 5xxx | 数字补x (填充右边, 宽度为4) |
| 10 | {:x<4d} | 10xx | 数字补x (填充右边, 宽度为4) |
| 1000000 | {:,} | 1,000,000 | 以逗号分隔的数字格式 |
| 0.25 | {:.2%} | 25.00% | 百分比格式 |
| 1000000000 | {:.2e} | 1.00E+09 | 指数记法 |
| 18 | {:>10d} | ' 18' | 右对齐 (默认, 宽度为10) |
| 18 | {:<10d} | '18 ' | 左对齐 (宽度为10) |
| 18 | {:^10d} | ' 18 ' | 中间对齐 (宽度为10) |

