Python
<a name="Opzgi"></a>
### 1、“for” 循环中的“Else”条件
除了目前为止已经看到的所有Python代码之外，有可能错过了下面的“for-else”。<br />这是一个用于循环遍历列表的“for-else”方法，尽管对列表进行了迭代，但仍然有一个“else”条件，这是非常不寻常的。<br />这是在Java、Ruby或JavaScript等其他编程语言中所没有看到的。看一个实践中的例子。<br />假设要检查列表中是否没有奇数。来遍历一下：
```python
numbers = [2, 4, 6, 8, 1]

for number in numbers:
    if number % 2 == 1:
        print(number)
        break
else:
    print("No odd numbers")
```
如果发现一个奇数，那么这个数字将被打印，因为`break`将被执行，而`else`分支将被跳过。<br />否则，如果`break`从未被执行，执行流将继续执行`else`分支。<br />在这个例子中，将输出1。
<a name="SUrai"></a>
### 2、使用命名的变量从列表中取元素
```python
my_list = [1, 2, 3, 4, 5]
one, two, three, four, five = my_list
```
<a name="V7k8V"></a>
### 3、使用heapq从列表中获取最大或最小的元素
```python
import heapq

scores = [51, 33, 64, 87, 91, 75, 15, 49, 33, 82]

print(heapq.nlargest(3, scores))  # [91, 87, 82]

print(heapq.nsmallest(5, scores))  # [15, 33, 33, 49, 51]
```
<a name="bNjfO"></a>
### 4、把列表中的值作为参数传递给方法
可以使用" `*` "提取列表中的所有元素：
```python
my_list = [1, 2, 3, 4]

print(my_list)  # [1, 2, 3, 4]

print(*my_list)  # 1 2 3 4
```
将列表中的所有元素作为方法参数传递时，这很有用：
```python
def sum_of_elements(*arg):
    total = 0
    for i in arg:
        total += i

    return total


result = sum_of_elements(*[1, 2, 3, 4])
print(result)  # 10
```
<a name="ADYpB"></a>
### 5、获取列表的所有中间元素
```python
_, *elements_in_the_middle, _ = [1, 2, 3, 4, 5, 6, 7, 8]

print(elements_in_the_middle)  # [2, 3, 4, 5, 6, 7]
```
<a name="ssCgc"></a>
### 6、一行赋值多个变量
```python
one, two, three, four = 1, 2, 3, 4
```
<a name="rEpTu"></a>
### 7、列表推导
可以使用推导如，让将列表中的每个数字都取二次方:
```python
numbers = [1, 2, 3, 4, 5]
squared_numbers = [num * num for num in numbers]

print(squared_numbers)
```
推导不仅仅局限于使用列表。还可以将它们与字典、集合和生成器一起使用。<br />看另一个例子，使用字典推导将一个字典的值提升到二阶：
```python
dictionary = {'a': 4, 'b': 5}
squared_dictionary = {key: num * num for (key, num) in dictionary.items()}

print(squared_dictionary)  # {'a': 16, 'b': 25}
```
<a name="tNdKv"></a>
### 8、通过`Enum`枚举相同概念的相关项
来自文档：`Enum`是绑定到唯一值的一组符号名。它们类似于全局变量，但它们提供了更有用的`repr()`、分组、类型安全和其他一些特性。<br />下面是例子：
```python
from enum import Enum

class Status(Enum):
    NO_STATUS = -1
    NOT_STARTED = 0
    IN_PROGRESS = 1
    COMPLETED = 2

print(Status.IN_PROGRESS.name)  # IN_PROGRESS
print(Status.COMPLETED.value)  # 2
```
<a name="zUXCo"></a>
### 9、不使用循环来重复字符串
```python
name = "Banana"
print(name * 4)  # BananaBananaBananaBanana
```
<a name="WKPx2"></a>
### 10、像数学式子一样比较3个数字
如果有一个值，比较它是否在其他两个值之间，有一个简单的表达式，在数学中使用：
```python
1 < x < 10
```
这是在小学学过的代数表达式。但是，也可以在Python中使用相同的表达式。<br />可能已经做过这种形式的比较：
```python
1 < x and x < 10
```
现在，只需要在Python中使用以下代码：
```python
1 < x < 10
```
这在Ruby中行不通，Ruby是一种旨在让程序员开心的编程语言。这在JavaScript中也可以工作。
<a name="VdvCV"></a>
### 11、在单条语句中合并字典
从Python 3.9开始可用：
```python
first_dictionary = {'name': 'Fatos', 'location': 'Munich'}second_dictionary = {'name': 'Fatos', 'surname': 'Morina',                     'location': 'Bavaria, Munich'}result = first_dictionary | second_dictionaryprint(result)  # {'name': 'Fatos', 'location': 'Bavaria, Munich', 'surname': 'Morina'}
```
<a name="IvsXX"></a>
### 12、在`tuple`中找到元素的索引
```python
books = ('Atomic habits', 'Ego is the enemy', 'Outliers', 'Mastery')print(books.index('Mastery'))   # 3
```
<a name="TE5Un"></a>
### 13、把字符串列表转换成一个列表
假设在一个字符串函数中获得输入，但它应该是一个列表：
```python
input = "[1,2,3]"
```
不需要那种格式，需要的是一个列表:
```python
input = [1,2,3]
```
或者可能从一个API调用得到以下响应：
```python
input = [[1, 2, 3], [4, 5, 6]]
```
所要做的就是导入模块`ast`，然后调用它的方法`literal_eval`，而不用去写复杂的正则表达式：
```python
import astdef string_to_list(string):    
    return ast.literal_eval(string)
```
这就是需要做的。<br />现在会得到一个列表作为结果，或者列表的列表，如下所示：
```python
import ast

def string_to_list(string):
    return ast.literal_eval(string)

string = "[[1, 2, 3],[4, 5, 6]]"
my_list = string_to_list(string)
print(my_list)  # [[1, 2, 3], [4, 5, 6]]
```
<a name="coLgH"></a>
### 14、使用命名参数避免 “trivial” 错误
假设想求两个数的差。差不是可交换的：
```python
a - b != b -a
```
可能会忘记参数的顺序，这可能会导致“trivial”错误：
```python
def subtract(a, b):
    return a - b

print((subtract(1, 3)))  # -2
print((subtract(3, 1)))  # 2
```
为了避免这种潜在的错误，可以简单地使用命名参数，参数的顺序不再重要：
```python
def subtract(a, b):
    return a - b

print((subtract(a=1, b=3)))  # -2
print((subtract(b=3, a=1)))  # -2
```
<a name="fuJtp"></a>
### 15、使用单个`print()`语句打印多个元素
```python
print(1, 2, 3, "a", "z", "this is here", "here is something else")
```
<a name="BGDo3"></a>
### 16、一行打印多个元素
```python
print("Hello", end="")
print("World")  # HelloWorld
print("Hello", end=" ")
print("World")  # Hello World
print('words',   'with', 'commas', 'in', 'between', sep=', ')
# words, with, commas, in, between
```
<a name="C7aat"></a>
### 17、打印多个值，每个值之间使用自定义分隔符
可以很容易地做高级打印：
```python
print("29", "01", "2022", sep="/")  # 29/01/2022

print("name", "domain.com", sep="@")  # name@domain.com
```
<a name="waVzz"></a>
### 18、不能在变量名的开头使用数字
```python
four_letters = "abcd" # this works4_letters = “abcd” # this doesn’t work
```
<a name="cLlGC"></a>
### 19、不能在变量名的开头使用操作符
```python
+variable = "abcd"  # this doesn’t work
```
<a name="IbY9A"></a>
### 20、不能把0作为数字的第一个数字
```python
number = 0110 # this doesn't work
```
<a name="bHQmC"></a>
### 21、可以在变量名的任何位置使用下划线字符
这意味着，在任何想要的地方，在变量名中，想要多少次就有多少次：
```python
a______b = "abcd"  # this works
_a_b_c_d = "abcd"  # this also works
```
不推荐使用它，但如果看到像这样奇怪的变量命名，要知道它实际上是一个有效的变量名。
<a name="NNleJ"></a>
### 22、可以用下划线分隔较大的数字
这样读起来更容易。
```python
print(1_000_000_000)  # 1000000000
print(1_234_567)  # 1234567
```
<a name="s9WPe"></a>
### 23、颠倒列表的顺序
```python
my_list = ['a', 'b', 'c', 'd']

my_list.reverse()

print(my_list)  # ['d', 'c', 'b', 'a']
```
<a name="aVysV"></a>
### 24、使用`step`函数对字符串切片
```python
my_string = "This is just a sentence"
print(my_string[0:5])  # This

# Take three steps forward
print(my_string[0:10:3])  # Tsse
```
<a name="s4Jeb"></a>
### 25、反向切片
```python
my_string = "This is just a sentence"
print(my_string[10:0:-1])  # suj si sih

# Take two steps forward
print(my_string[10:0:-2])  # sjs i
```
<a name="R4Lyx"></a>
### 26、只有开始或结束索引的部分切片
表示切片的开始和结束的索引可以是可选的。
```python
my_string = "This is just a sentence"
print(my_string[4:])  # is just a sentence

print(my_string[:3])  # Thi
```
<a name="QoK6o"></a>
### 27、Floor 除法
```python
print(3/2)  # 1.5
print(3//2)  # 1
```
<a name="pTGDu"></a>
### 28、`==` 和 “`is`” 的差别
"`is`"检查两个变量是否指向内存中的同一个对象。<br />"`==`"比较这两个对象的值是否相等。
```python
first_list = [1, 2, 3]
second_list = [1, 2, 3]

# Is their actual value the same?
print(first_list == second_list)  # True

# Are they pointing to the same object in memory
print(first_list is second_list)  
# False, since they have same values, but in different objects in memory


third_list = first_list

print(third_list is first_list)  
# True, since both point to the same object in memory
```
<a name="nR9pG"></a>
### 29、更改分配给另一个变量的变量的值
当一个变量被赋值给另一个变量时，它的值实际上被复制到第二个变量中。<br />这意味着第一个变量之后的任何变化都不会反映在第二个变量中：
```python
first = "An initial value"
second = first
first = "An updated value"

print(first)  # An updated value
print(second)  # An initial value
```
<a name="ZIV22"></a>
### 30、检查一个字符串是否大于另一个字符串
```python
first = "abc"
second = "def"
print(first < second)  # True
second = "ab"
print(first < second)  # False
```
<a name="E8UQ2"></a>
### 31、检查字符串是不是从特定字符开始的
```python
my_string = "abcdef"
print(my_string.startswith("b"))  # False
```
<a name="U8E65"></a>
### 32、使用`id()`找到变量的唯一id
```python
print(id(1))  # 4325776624
print(id(2))  # 4325776656
print(id("string"))  # 4327978288
```
<a name="b32Cd"></a>
### 33、`Integers`, `floats`, `strings`, `booleans`, `sets`以及`tuples`是不可修改的
当将变量赋给整数、浮点数、字符串、布尔值、集合和元组等不可变类型时，该变量将指向内存中的对象。<br />如果给该变量赋了另一个值，原来的对象仍然在内存中，但是指向它的变量丢失了：
```python
number = 1
print(id(number))  # 4325215472
print(id(1))  # 4325215472

number = 3
print(id(number))  # 4325215536
print(id(1))  # 4325215472
```
