Python 函数
<a name="RTW1D"></a>
### 1、`reduce()`
`reduce()` 是 functools 模块下面的一个函数，接收两个参数，一个是函数对象，一个是可迭代对象（比如list）， reduce每次会把迭代对象中的下一个元素作用在函数上做累积计算，最后得到一个值。
```python
# 创建函数
def add(a, b):
    result = a + b
    print(f"{a} + {b} = {result}")
    return result


from functools import reduce

result = reduce(add, [1, 2, 3, 4])
print("结果：", result)
```
输出
```python
1 + 2 = 3
3 + 3 = 6
6 + 4 = 10
结果： 10
```
执行过程：第一次将列表中的前两个数取出来作为函数`add`的参数，第二次将上一次函数add的返回值与列表的第3个数作为参数，依此类推，最后得到一个值。这就是`reduce`的作用。有点像万物归一的感觉。<br />当然，如果只是计算列表中的元素之和，大可不必绕这么大弯子用reduce来处理，直接用 sum 函数就可以解决。
```python
result = sum([1, 2, 3, 4])
```
如果是计算列表中元素的乘积，python并没有内置的函数直接计算，这时候可以借用`reduce`来处理
```python
def mul(a, b):
    return a * b

result = reduce(mul, [1, 2, 3, 4])
print("结果：", result)
```
输出
```python
结果： 24
```
或者使用 lambda 匿名函数
```python
result = reduce(lambda a, b: a * b, [1, 2, 3, 4])
```
甚至可以直接使用operator模块下的乘法操作符函数
```python
from operator import mul
result = reduce(mul, [1, 2, 3, 4])
print("结果：", result)
```
最后会发现解法其实很多种，不过应该记住python之禅里面那句话：<br />There should be one-- and preferably only one --obvious way to do it.<br />用最合适的方式去做一件事
<a name="s2x9S"></a>
### 2、`split()`
split 接收一个参数，用于将字符串切割成列表，比如一段英文字符串按照空格切割就可以统计出单词的个数，
```python
words = "python is the best programming language"
words = words.split(" ")
print(words)
```
输出
```python
['column1', 'column2', 'column3']
```
<a name="CWJLZ"></a>
### 3、`enumerate()`
`enumerate` 函数用于迭代列表等可迭代对象，它的使用场景一般出现在需要获取列表的下标位置时，直接用`for`循环去迭代列表时，是拿不到元素下标位置的，而 `enumerate` 就可以获取，否则还得自己去定义一个索引变量。
```python
words = ['python', 'is', 'the', 'best', 'programming', 'language']
index = 0
for w in words:
    print(index, w)
    index += 1

0 python
1 is
2 the
3 best
4 programming
5 language
```
使用 enumerate 函数，处理起来就更优雅了
```python
for index, w in enumerate(words):
    print(index, w)

0 python
1 is
2 the
3 best
4 programming
5 language
```
<a name="pipLa"></a>
### 4、`map()`
`map`是一个与reduce函数对应的函数，Google的map/reduce框架的思想其实就是从这两个函数借鉴而来的。map函数用于把一个列表通过函数处理，映射成一个新的列表。例如给列表的每个元素做平方，将列表元素转换成字符串，得到一个新的列表。
```python
result = map(lambda x: str(x), [1, 2, 3, 4])
print(list(result))
result = map(lambda x: x * x, [1, 2, 3, 4]))
print(list(result))
```
输出：
```python
['1', '2', '3', '4']
[1, 4, 9, 16]
```
此外，map 函数还可以接受多个列表参数，使得多个列表合并为一个列表成为可能，例如，将两个列表相同位置的元素相加得到一个新的列表
```python
def merge(x, y):
    return x + y


result = map(merge, [1, 2, 3], [3, 2, 1])
print(list(result))
```
输出
```python
[4, 4, 4]
```
<a name="rgGFS"></a>
### 5、`getattr()`
`getattr()` 返回对象属性对应的值，接受两个参数，第一个是对象，第二个是属性名，这个函数通常用户动态或者某个对象下面的某些属性的值，看例子：
```python
class Foo:
    def __init__(self):
        self.a = 10


foo = Foo()

a = getattr(foo, "a")
print(a)
```
输出
```python
10
```
可能会问，直接 foo.a 不就可以获取a属性的值了吗？正常情况是这样没错，如果是在不知道什么情况下要获取什么属性的值时，这时候getattr就可以派上用场了。初学者可能还体验不到，当尝试去写些框架级的代码时，要想起来有这样的函数可以使用就行。
<a name="HI9FL"></a>
### 6、`slice()`
`slice` 是一个切片函数，切片操作可能使用过，通过切片来获取列表的子集， 例如：
```python
s = [1,2,3,4]
>>> s[1:3]  # 获取列表s中第1到第3之间的元素组成的子列表
```
`"1:3"` 其就是 就是 `slice(1:3)` 函数的缩写方式，前者就像是语法糖
```python
s = [1, 2, 3, 4]
print(s[slice(1, 3)])
```
通常实际应用过程中，直接用语法糖的写法就可以，没必要用`slice`函数进行切片，但是至少应该知道`slice`是怎么用的。
<a name="y4MGA"></a>
### 7、`sorted()`
sorted 函数应该日常代码中，算是一个高频函数了，用于将列表等可迭代对象进行排序，它不会改变原列表的顺序，而是返回一个新的列表。默认按照升序排列
```python
nums = [4, 5, 6, 3, 1]

print(sorted(nums))
```
输出
```python
[1, 3, 4, 5, 6]
```
如果想要降序排列，则需要指定第二个参数：`reverse=True`
```python
nums = [4, 5, 6, 3, 1]
print(sorted(nums, reverse=True))  # [6, 5, 4, 3, 1]
```
`sorted` 函数的强大之处远不止如此，因为还可以自定义排序规则，比如参与比较是一个自定义的类Student， 需要按照Student里面的年龄age进行排序，这时候需要自定义排序因子函数
```python
def my_sort_key(s):
    return s.age


class Student:
    def __init__(self, age):
        self.age = age

    def __str__(self):
        return f"Student({self.age})"


s1 = Student(12)
s2 = Student(2)
s3 = Student(30)

new_list = (sorted([s1, s2, s3], key=my_sort_key))
for i in new_list:
    print(i)
```
输出：
```python
Student(2)
Student(12)
Student(30)
```
<a name="XBxND"></a>
### 8、format
format 函数曾经字符串格式化最常用的函数，使用也是非常简单，但自从f字符串出现之后，format 的功能逐渐被取代，但是3.6之前还是可以常见到该函数的应用场景。
```python
s = "{} is first name"
print(s.format("liu"))
```
如果需要占位符比较多搞不清次序的话，可以给每个占位符一个名字，这样就不拍对不上位置了
```python
s = "{first_name} is first name"
print(s.format(first_name="liu"))
```
<a name="NyHhx"></a>
### 9、`join()`
join 也是比较常用的一个内置函数，它可以将列表对象用指定的字符作为元素之间的连接，转换为字符串。
```python
words = ['python', 'is', 'the', 'best', 'programming', 'language']

print(" ".join(words)) # 用空格连接 python is the best programming language
```
<a name="wkeTP"></a>
### 10、`type`
type 是python最难理解的一个内置函数了，新手可能以为type就是一个用来查看某个对象的类型是什么，例如：
```python
print(type(10)) # <class 'int'>
print(type([])) # <class 'list'>
print(type("s"))#  <class 'str'>
```
它的另一个作用是可以用type来创建类，一般情况下，都用关键字 class 来定义一个类，而type也可以用来创建类
```python
>>> Person = type("Person", (), {"live":True})
>>> Person
<class '__main__.Person'>
```
第一个参数 Person是类的名字， 第二个参数用来指定父类是谁， 第三个参数是这个类的类属性有哪些。上面这段代码等价于：
```python
>>> class Person:
...     live = True
...
>>> Person
<class '__main__.Person'>
```
创建Person这类的type函数其实是一个叫“元类”的东西。
