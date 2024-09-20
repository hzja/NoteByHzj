Python
<a name="GRP9l"></a>
## 67、根据参数从右侧移除字符
```python
string = "This is a sentence with "
# Remove trailing spaces from the right
print(string.rstrip())  # "This is a sentence with"
string = "this here is a sentence…..,,,,aaaaasd"
print(string.rstrip(“.,dsa”))  # "this here is a sentence"
```
类似地，可以根据参数从左边删除字符:
```python
string = "ffffffffFirst"
print(string.lstrip(“f”))  # First
```
<a name="oL0tS"></a>
## 68、检查一个字符串是否代表一个数字
```python
string = "seven"
print(string.isdigit())  # Falsestring = "1337"
print(string.isdigit())  # Truestring = "5a"
print(string.isdigit())  # False, because it contains the character 'a'string = "2**5"
print(string.isdigit())  # False
```
<a name="Z2Pk2"></a>
## 69、检查一个字符串是否代表一个中文数字
```python
# 42673 in Arabic numerals
string = "四二六七三"

print(string.isdigit())  # False
print(string.isnumeric())  # True
```
<a name="aCSj3"></a>
## 70、检查一个字符串是否所有的单词都以大写字母开头
```python
string = "This is a sentence"
print(string.istitle())  # False

string = "10 Python Tips"
print(string.istitle())  # True

string = "How to Print A String in Python"
# False, because of the first characters being lowercase in "to" and "in"
print(string.istitle())

string = "PYTHON"
print(string.istitle())  # False. It's titlelized version is "Python"
```
<a name="J2IBg"></a>
## 71、也可以在元组中使用负索引
```python
numbers = (1, 2, 3, 4)

print(numbers[-1])  # 4
print(numbers[-4])  # 1
```
<a name="spewI"></a>
## 72、在元组中嵌套列表和元组
```python
mixed_tuple = (("a"*10, 3, 4), ['first', 'second', 'third'])

print(mixed_tuple[1])  # ['first', 'second', 'third']
print(mixed_tuple[0])  # ('aaaaaaaaaa', 3, 4)
```
<a name="lqNjM"></a>
## 73、快速计算满足条件的元素在列表中出现的次数
```python
names = ["Besim", "Albert", "Besim", "Fisnik", "Meriton"]

print(names.count("Besim"))  # 2
```
<a name="Jlptg"></a>
## 74、使用`slice()`可以方便的得到最近的元素
```python
my_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
slicing = slice(-4, None)

# Getting the last 3 elements from the list
print(my_list[slicing])  # [4, 5, 6]

# Getting only the third element starting from the right
print(my_list[-3])  # 4
```
也可以使用`slice()`来完成其他常见的切片任务，比如:
```python
string = "Data Science"

# start = 1, stop = None (don't stop anywhere), step = 1
# contains 1, 3 and 5 indices
slice_object = slice(5, None)

print(string[slice_object])   # Science
```
<a name="FN2c2"></a>
## 75、计算元素在元组中出现的次数
```python
my_tuple = ('a', 1, 'f', 'a', 5, 'a')

print(my_tuple.count('a'))  # 3
```
<a name="eWWNJ"></a>
## 76、获取元组中元素的索引
```python
my_tuple = ('a', 1, 'f', 'a', 5, 'a')

print(my_tuple.index('f'))  #  2
```
<a name="jff57"></a>
## 77、通过跳转获取子元组
```python
my_tuple = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

print(my_tuple[::3])  # (1, 4, 7, 10)
```
<a name="d2ZPU"></a>
## 78、从索引开始获取子元组
```python
my_tuple = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

print(my_tuple[3:])  # (4, 5, 6, 7, 8, 9, 10)
```
<a name="FXhT0"></a>
## 79、从列表、集合或字典中删除所有元素
```python
my_list = [1, 2, 3, 4]
my_list.clear()
print(my_list)  # []

my_set = {1, 2, 3}
my_set.clear()
print(my_set)  # set()

my_dict = {"a": 1, "b": 2}
my_dict.clear()
print(my_dict)  # {}
```
<a name="FHXmz"></a>
## 80、合并2个集合
一种方法是使用方法`union()`，它将作为合并的结果返回一个新的集合:
```python
first_set = {4, 5, 6}
second_set = {1, 2, 3}

print(first_set.union(second_set))  # {1, 2, 3, 4, 5, 6}
```
另一个是方法update，它将第二个集合的元素插入到第一个集合中:
```python
first_set = {4, 5, 6}
second_set = {1, 2, 3}

first_set.update(second_set)

print(first_set)  # {1, 2, 3, 4, 5, 6}
```
<a name="h8StW"></a>
## 81、打印函数内的条件语句
```python
def is_positive(number):
    print("Positive" if number > 0 else "Negative")  # Positive

is_positive(-3)
```
<a name="zWqgT"></a>
## 82、一个if语句中包含多个条件
```python
math_points = 51
biology_points = 78
physics_points = 56
history_points = 72

my_conditions = [math_points > 50, biology_points > 50,
                 physics_points > 50, history_points > 50]

if all(my_conditions):
    print("Congratulations! You have passed all of the exams.")
else:
    print("I am sorry, but it seems that you have to repeat at least one exam.")
# Congratulations! You have passed all of the exams.
```
<a name="co49r"></a>
## 83、在一个if语句中至少满足一个条件
```python
math_points = 51
biology_points = 78
physics_points = 56
history_points = 72

my_conditions = [math_points > 50, biology_points > 50,
                 physics_points > 50, history_points > 50]

if any(my_conditions):
    print("Congratulations! You have passed all of the exams.")
else:
    print("I am sorry, but it seems that you have to repeat at least one exam.")
# Congratulations! You have passed all of the exams.
```
<a name="oD7nB"></a>
## 84、任何非空字符串都被计算为True
```python
print(bool("Non empty"))  # True
print(bool(""))  # False
```
<a name="ePQxB"></a>
## 85、任何非空列表、元组或字典都被求值为True
```python
print(bool([]))  # False
print(bool(set([])))  # False

print(bool({}))  # False
print(bool({"a": 1}))  # True
```
<a name="KbwXJ"></a>
## 86、其他计算为`False`的值是`None`、“`False`”和数字0
```python
print(bool(False))  # False
print(bool(None))  # False
print(bool(0))  # False
```
<a name="qgCf5"></a>
## 87、不能仅仅通过在函数中提及全局变量来改变它的值
```python
string = "string"

def do_nothing():
  string = "inside a method"

do_nothing()

print(string)  # string
```
也需要使用访问修饰符`global`:
```python
string = "string"

def do_nothing():
    global string
    string = "inside a method"

do_nothing()

print(string)  # inside a method
```
<a name="DzF3L"></a>
## 88、使用“`collections`”中的`Counter`计算字符串或列表中的元素数量
```python
from collections import Counter

result = Counter("Banana")
print(result)  # Counter({'a': 3, 'n': 2, 'B': 1})

result = Counter([1, 2, 1, 3, 1, 4, 1, 5, 1, 6])
print(result)  # Counter({1: 5, 2: 1, 3: 1, 4: 1, 5: 1, 6: 1})
```
<a name="tKRZB"></a>
## 89、使用`Counter`检查是否2个字符串包含相同的字符
```python
from collections import Counter

def check_if_anagram(first_string, second_string):
    first_string = first_string.lower()
    second_string = second_string.lower()
    return Counter(first_string) == Counter(second_string)

print(check_if_anagram('testinG', 'Testing'))  # True
print(check_if_anagram('Here', 'Rehe'))  # True
print(check_if_anagram('Know', 'Now'))  # False
```
也可以使用`sorted()`检查两个字符串是否具有相同的字符：
```python
def check_if_anagram(first_word, second_word):
    first_word = first_word.lower()
    second_word = second_word.lower()
    return sorted(first_word) == sorted(second_word)
    
print(check_if_anagram("testinG", "Testing"))  # True
print(check_if_anagram("Here", "Rehe"))  # True
print(check_if_anagram("Know", "Now"))  # False
```
<a name="c3S7V"></a>
## 90、使用" itertools "中的" Count "计算元素的数量
```python
from itertools import count

my_vowels = ['a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U']

current_counter = count()

string = "This is just a sentence."

for i in string:
    if i in my_vowels:
        print(f"Current vowel: {i}")
        print(f"Number of vowels found so far: {next(current_counter)}")
```
这是控制台中的结果:
```python
Current vowel: i
Number of vowels found so far: 0
Current vowel: i
Number of vowels found so far: 1
Current vowel: u
Number of vowels found so far: 2
Current vowel: a
Number of vowels found so far: 3
Current vowel: e
Number of vowels found so far: 4
Current vowel: e
Number of vowels found so far: 5
Current vowel: e
Number of vowels found so far: 6
```
<a name="Q4JiZ"></a>
## 91、根据字符串或列表的频率对元素进行排序
来自`collections`模块的`Counter`默认情况下不会根据元素的频率来排序。
```python
from collections import Counter

result = Counter([1, 2, 3, 2, 2, 2, 2])
print(result)  # Counter({2: 5, 1: 1, 3: 1})
print(result.most_common())  # [(2, 5), (1, 1), (3, 1)]
```
<a name="xaW66"></a>
## 92、在一行中找到列表中出现频次最高的元素
```python
my_list = ['1', 1, 0, 'a', 'b', 2, 'a', 'c', 'a']

print(max(set(my_list), key=my_list.count))  # a
```
<a name="LXgXO"></a>
## 93、`copy()`和`deepcopy()`的区别
来自文档中的解释:<br />浅拷贝构造一个新的复合对象，然后(在可能的范围内)在其中插入对原始对象的引用。深拷贝构造一个新的复合对象，然后递归地将在原始对象中找到的对象的副本插入其中。<br />更全面的描述：<br />浅拷贝意味着构造一个新的集合对象，然后用对原始集合中的子对象的引用填充它。从本质上说，浅拷贝的深度只有一层。拷贝过程不会递归，因此不会创建子对象本身的副本。深拷贝使拷贝过程递归。这意味着首先构造一个新的集合对象，然后用在原始集合对象中找到的子对象的副本递归地填充它。以这种方式拷贝对象将遍历整个对象树，以创建原始对象及其所有子对象的完全独立的克隆。<br />这里是`copy()`的例子：
```python
first_list = [[1, 2, 3], ['a', 'b', 'c']]

second_list = first_list.copy()

first_list[0][2] = 831

print(first_list)  # [[1, 2, 831], ['a', 'b', 'c']]
print(second_list)  # [[1, 2, 831], ['a', 'b', 'c']]
```
这个是`deepcopy()` 的例子:
```python
import copy

first_list = [[1, 2, 3], ['a', 'b', 'c']]

second_list = copy.deepcopy(first_list)

first_list[0][2] = 831

print(first_list)  # [[1, 2, 831], ['a', 'b', 'c']]
print(second_list)  # [[1, 2, 3], ['a', 'b', 'c']]
```
<a name="WK99p"></a>
## 94、当试图访问字典中不存在的键时，可以避免抛出错误
如果使用一个普通的字典，并试图访问一个不存在的键，那么将得到一个错误:
```python
my_dictonary = {"name": "Name", "surname": "Surname"}
print(my_dictonary["age"])  
```
下面是抛出的错误：
```python
KeyError: 'age'
```
可以使用`defaultdict()`：来避免这种错误
```python
from collections import defaultdict

my_dictonary = defaultdict(str)
my_dictonary['name'] = "Name"
my_dictonary['surname'] = "Surname"

print(my_dictonary["age"])  
```
<a name="fjXdf"></a>
## 95、可以构建自己的迭代器
```python
class OddNumbers:
    def __iter__(self):
        self.a = 1
        return self

    def __next__(self):
        x = self.a
        self.a += 2
        return x

odd_numbers_object = OddNumbers()
iterator = iter(odd_numbers_object)

print(next(iterator))  # 1
print(next(iterator))  # 3
print(next(iterator))  # 5
```
<a name="BkRVW"></a>
## 96、可以用一行从列表中删除重复项
```python
my_set = set([1, 2, 1, 2, 3, 4, 5])
print(list(my_set))  # [1, 2, 3, 4, 5]
```

<a name="Cb6dk"></a>
## 97、打印模块所在的位置
```python
import torch

print(torch)  # <module 'torch' from '/Users/...'
```
<a name="SMa46"></a>
## 98、可以使用"`not in`"来检查值是否不属于列表
```python
odd_numbers = [1, 3, 5, 7, 9]
even_numbers = []

for i in range(9):
    if i not in odd_numbers:
        even_numbers.append(i)

print(even_numbers)  # [0, 2, 4, 6, 8]
```
<a name="Rk1Zk"></a>
## 99、`sort()` 和 `sorted()`的差别
`sort()`对原始列表进行排序。<br />`sorted()`返回一个新的排序列表。
```python
groceries = ['milk', 'bread', 'tea']

new_groceries = sorted(groceries)
# new_groceries = ['bread', 'milk', 'tea']

print(new_groceries)

# groceries = ['milk', 'bread', 'tea']
print(groceries)

groceries.sort()

# groceries = ['bread', 'milk', 'tea']
print(groceries)
```
<a name="tctlz"></a>
## 100、使用uuid模块生成唯一的id
UUID代表统一唯一标识符。
```python
import uuid

# Generate a UUID from a host ID, sequence number, and the current time
print(uuid.uuid1())  # 308490b6-afe4-11eb-95f7-0c4de9a0c5af

# Generate a random UUID
print(uuid.uuid4())  # 93bc700b-253e-4081-a358-24b60591076a
```
