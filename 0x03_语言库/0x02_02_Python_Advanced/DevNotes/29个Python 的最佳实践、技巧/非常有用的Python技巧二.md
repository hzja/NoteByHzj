Python 
<a name="c9z73"></a>
## 34、Strings 和 tuples 是不可修改的
```python
name = "Fatos"
print(id(name))  # 4422282544

name = "fatos"
print(id(name))  # 4422346608

my_tuple = (1, 2, 3, 4)
print(id(my_tuple))  # 4499290128

my_tuple = ('a', 'b')
print(id(my_tuple))  # 4498867584
```
<a name="Frfvm"></a>
## 35、`Lists`, `sets`, 和 `dictionaries` 是不可修改的
这意味着可以在不丢失绑定的情况下更改对象：
```python
cities = ["Munich", "Zurich", "London"]
print(id(cities))  # 4482699712

cities.append("Berlin")
print(id(cities))  # 4482699712
```
下面是另一个集合的例子：
```python
my_set = {1, 2, 3, 4}
print(id(my_set))  # 4352726176

my_set.add(5)
print(id(my_set))  # 4352726176
```
<a name="zBuop"></a>
## 36、可以把集合转换为不可修改的集合
这样，就不能再修改它了:
```python
my_set = frozenset(['a', 'b', 'c', 'd'])

my_set.add("a")
```
如果这样做，就会抛出一个错误：
```python
AttributeError: 'frozenset' object has no attribute 'add'
```
<a name="Wm2J1"></a>
## 37、" if-elif "块可以在没有else块的情况下存在
但是，如果前面没有“if”，“elif”就不能独立存在：
```python
def check_number(number):
    if number > 0:
        return "Positive"
    elif number == 0:
        return "Zero"

    return "Negative"


print(check_number(1))  # Positive
```
<a name="zwcXZ"></a>
## 38、使用`sorted()`查看2个字符串是否是相同的字母但次序不一样
```python
def check_if_anagram(first_word, second_word):
    first_word = first_word.lower()
    second_word = second_word.lower()
    return sorted(first_word) == sorted(second_word)

print(check_if_anagram("testinG", "Testing"))  # True
print(check_if_anagram("Here", "Rehe"))  # True
print(check_if_anagram("Know", "Now"))  # False
```
<a name="fe54C"></a>
## 39、获取一个字符的Unicode值
```python
print(ord("A"))  # 65
print(ord("B"))  # 66
print(ord("C"))  # 66
print(ord("a"))  # 97
```
<a name="AsVtc"></a>
## 40、一行代码获取字典中所有的keys
```python
dictionary = {"a": 1, "b": 2, "c": 3}

keys = [i for i, _ in dictionary.items()]

print(keys)  # ['a', 'b', 'c']
```
<a name="Ab8mH"></a>
## 41、一行代码获取字典中所有的值
```python
dictionary = {"a": 1, "b": 2, "c": 3}

values = [i for _, i in dictionary.items()]

print(values)  # [1, 2, 3]
```
<a name="lFWOD"></a>
## 42、交换字典中的keys和values
```python
dictionary = {"a": 1, "b": 2, "c": 3}

reversed_dictionary = {j: i for i, j in dictionary.items()}

print(reversed)  # {1: 'a', 2: 'b', 3: 'c'}
```
<a name="YLIPW"></a>
## 43、可以将布尔型值转换为数字
```python
print(int(False))  # 0
print(float(True))  # 1.0
```
<a name="rC9JC"></a>
## 44、可以算术操作中使用布尔值
“False”是0，而“True”是1。
```python
x = 10
y = 12
result = (x - False)/(y * True)
print(result)  # 0.8333333333333334
```
<a name="pXBky"></a>
## 45、可以将任何数据的类型转换为布尔值
```python
print(bool(.0))  # False
print(bool(3))  # True
print(bool("-"))  # True
print(bool("string"))  # True
print(bool(" "))  # True
```
<a name="OMgeH"></a>
## 46、将一个值转换为复数
```python
print(complex(10, 2))  # (10+2j)
```
也可以将数字转换为十六进制数:
```python
print(hex(11))  # 0xb
```
<a name="p1lpq"></a>
## 47、把值加到列表的第一个位置
如果使用`append()`，将从右边插入新的值。<br />也可以使用*`insert()`*来指定要插入新元素的索引和元素。在例子中，把它插入到第一个位置，所以使用0作为下标：
```python
my_list = [3, 4, 5]

my_list.append(6)
my_list.insert(0, 2)
print(my_list)  # [2, 3, 4, 5, 6]
```
<a name="AdjNo"></a>
## 48、Lambda方法只能在一行里
在使用lambdas方法的时候，不能超过一行。<br />试试以下方法：
```python
comparison = lambda x: if x > 3:
                    print("x > 3")
                else:
                    print("x is not greater than 3")
```
将抛出以下错误:
```python
result = lambda x: if x > 3:
 ^
SyntaxError: invalid syntax
```
<a name="W5tKv"></a>
## 49、lambda中的条件语句应该始终包含“else”部分
试下下面的：
```python
comparison = lambda x: "x > 3" if x > 3
```
将得到以下错误:
```python
comparison = lambda x: "x > 3" if x > 3
                                          ^
SyntaxError: invalid syntax
```
<a name="wpNF3"></a>
## 50、`filter()` 返回一个新的对象
```python
my_list = [1, 2, 3, 4]

odd = filter(lambda x: x % 2 == 1, my_list)

print(list(odd))   # [1, 3]
print(my_list)  # [1, 2, 3, 4]
```
<a name="zopo7"></a>
## 51、`map()` 返回一个新的对象
```python
my_list = [1, 2, 3, 4]

squared = map(lambda x: x ** 2, my_list)

print(list(squared))   # [1, 4, 9, 16]
print(my_list)  # [1, 2, 3, 4]
```
<a name="oa7T7"></a>
## 52、`range()` 中有一个步长的参数，但是知道的并不多
```python
for number in range(1, 10, 3):
    print(number, end=" ")
# 1 4 7
```
<a name="G992H"></a>
## 53、`range()` 默认从0开始
所以根本不需要包含它。
```python
def range_with_zero(number):
    for i in range(0, number):
        print(i, end=' ')


def range_with_no_zero(number):
    for i in range(number):
        print(i, end=' ')


range_with_zero(3)  # 0 1 2
range_with_no_zero(3)  # 0 1 2
```
<a name="GKxdx"></a>
## 54、不需要将长度和0比较
如果长度大于0，则默认为True，所以不需要将其与0进行比较:
```python
def get_element_with_comparison(my_list):
    if len(my_list) > 0:
        return my_list[0]

def get_first_element(my_list):
    if len(my_list):
        return my_list[0]

elements = [1, 2, 3, 4]
first_result = get_element_with_comparison(elements)
second_result = get_element_with_comparison(elements)

print(first_result == second_result)  # True
```
<a name="fcfRE"></a>
## 55、可以在同一范围内多次定义相同的方法
但是，只有最后一个被调用，因为它覆盖了以前的。
```python
def get_address():
    return "First address"

def get_address():
    return "Second address"

def get_address():
    return "Third address"

print(get_address())  # Third address
```
<a name="hfZKh"></a>
## 56、可以访问私有属性
```python
class Engineer:
    def __init__(self, name):
        self.name = name
        self.__starting_salary = 62000

dain = Engineer('Dain')
print(dain._Engineer__starting_salary)  # 62000
```
<a name="CWYGH"></a>
## 57、查看对象的内存使用
```python
import sys

print(sys.getsizeof("bitcoin"))  # 56
```
<a name="nirFi"></a>
## 58、定义一个方法，调用的时候想传多少参数都可以
```python
def get_sum(*arguments):
    result = 0
    for i in arguments:
        result += i
    return result


print(get_sum(1, 2, 3))  # 6
print(get_sum(1, 2, 3, 4, 5))  # 15
print(get_sum(1, 2, 3, 4, 5, 6, 7))  # 28
```
<a name="brYBF"></a>
## 59、使用`super()` 或者父类的名字调用父类的初始化
使用`super()`调用父类初始化器:
```python
class Parent:
    def __init__(self, city, address):
        self.city = city
        self.address = address

class Child(Parent):
    def __init__(self, city, address, university):
        super().__init__(city, address)
        self.university = university

child = Child('Zürich', 'Rämistrasse 101', 'ETH Zürich')
print(child.university)  # ETH Zürich
```
使用父类的名称调用父类:
```python
class Parent:
    def __init__(self, city, address):
        self.city = city
        self.address = address


class Child(Parent):
    def __init__(self, city, address, university):
        Parent.__init__(self, city, address)
        self.university = university


child = Child('Zürich', 'Rämistrasse 101', 'ETH Zürich')
print(child.university)  # ETH Zürich
```
注意，使用`init()`和`super()`调用父类初始化器只能在子类的初始化器中使用。
<a name="KLSXK"></a>
## 60、可以在自己的类中重新定义“+”操作符
在两个int数据类型之间使用**+**操作符时，将得到它们的和。<br />然而，在两个字符串数据类型之间使用它时，将合并它们：
```python
print(10 + 1)  # Adding two integers using '+'
print('first' + 'second')  # Merging two strings '+'
```
这表示操作符重载。<br />也可以在自己的类中使用它:
```python
class Expenses:
    def __init__(self, rent, groceries):
        self.rent = rent
        self.groceries = groceries

    def __add__(self, other):
        return Expenses(self.rent + other.rent,
                        self.groceries + other.groceries)


april_expenses = Expenses(1000, 200)
may_expenses = Expenses(1000, 300)

total_expenses = april_expenses + may_expenses
print(total_expenses.rent)  # 2000
print(total_expenses.groceries)  # 500
```
<a name="NUYNp"></a>
## 61、还可以在自己的类中重新定义“<”和“=”操作符Y
下面是另一个可以自己定义的操作重载的例子：
```python
class Game:
    def __init__(self, score):
        self.score = score

    def __lt__(self, other):
        return self.score < other.score


first = Game(1)
second = Game(2)

print(first < second)  # True
```
类似地，就像前面的两种情况，可以根据自己的需要重写`eq()`函数：
```python
class Journey:
    def __init__(self, location, destination, duration):
        self.location = location
        self.destination = destination
        self.duration = duration

    def __eq__(self, other):
        return ((self.location == other.location) and
                (self.destination == other.destination) and
                (self.duration == other.duration))


first = Journey('Location A', 'Destination A', '30min')
second = Journey('Location B', 'Destination B', '30min')

print(first == second)
```
也可以类似地定义:

- `sub()` for-
- `mul()` for *****
- `truediv()` for/
- `ne()` for!=
- `ge()` for >=
- `gt()` for >
<a name="Ba4ix"></a>
## 62、可以为类的对象定义一个自定义的可打印版本
```python
class Rectangle:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def __repr__(self):
        return repr('Rectangle with area=' + str(self.a * self.b))


print(Rectangle(3, 4))  # 'Rectangle with area=12'
```
<a name="OL2MV"></a>
## 63、交换字符串中的字符大小写
```python
string = "This is just a sentence."
result = string.swapcase()print(result)  # tHIS IS JUST A SENTENCE.
```
<a name="VNHTG"></a>
## 64、检查是否所有字符都是字符串中的空格
```python
string = "  "
result = string.isspace()print(result)  # True
```
<a name="rETmY"></a>
## 65、检查字符串中的所有字符是否都是字母或数字
```python
name = "Password"
print(name.isalnum())  # True, because all characters are alphabetsname = "Secure Password "
print(name.isalnum())  # False, because it contains whitespacesname = "S3cur3P4ssw0rd"
print(name.isalnum())  # Truename = "133"
print(name.isalnum())  # True, because all characters are numbers
```
<a name="ojUVi"></a>
## 66、检查字符串中的所有字符是否都是字母
```python
string = "Name"
print(string.isalpha())  # Truestring = "Firstname Lastname"
print(string.isalpha())  # False, because it contains whitespacestring = “P4ssw0rd”
print(string.isalpha())  # False, because it contains numbers
```
