PythonLambda<br />Python 提供了非常多的库和内置函数。有不同的方法可以执行相同的任务，而在 Python 中，有个万能之王函数：lambda 函数，它可以以不同的方式在任何地方使用。今天和大家一起研究下这个万能之王！
<a name="GWUwY"></a>
## Lambda 函数简介
Lambda函数也被称为匿名(没有名称)函数，它直接接受参数的数量以及使用该参数执行的条件或操作，该参数以冒号分隔，并返回最终结果。为了在大型代码库上编写代码时执行一项小任务，或者在函数中执行一项小任务，便在正常过程中使用lambda函数。
```python
lambda argument_list:expersion
```
argument_list是参数列表，它的结构与Python中函数(function)的参数列表是一样的
```python
a,b
a=1,b=2
*args
**kwargs
a,b=1,*args
空
....
```
expression是一个关于参数的表达式，表达式中出现的参数需要在argument_list中有定义，并且表达式只能是单行的。
```python
1
None
a+b
sum(a)
1 if a >10 else 0
[i for i in range(10)]
...
```
<a name="KxtiD"></a>
## 普通函数和Lambda函数的区别

1. **没有名称**Lambda函数没有名称，而普通操作有一个合适的名称。
2. **Lambda函数没有返回值**使用def关键字构建的普通函数返回值或序列数据类型，但在Lambda函数中返回一个完整的过程。假设想要检查数字是偶数还是奇数，使用lambda函数语法类似于下面的代码片段。
```python
b = lambda x: "Even" if x%2==0 else "Odd"
b(9)
```

1. **函数只在一行中**Lambda函数只在一行中编写和创建，而在普通函数的中使用缩进
2. **不用于代码重用**Lambda函数不能用于代码重用，或者不能在任何其他文件中导入这个函数。相反，普通函数用于代码重用，可以在外部文件中使用。
<a name="sWpdv"></a>
## 为什么要使用Lambda函数?
一般情况下，不使用Lambda函数，而是将其与高阶函数一起使用。高阶函数是一种需要多个函数来完成任务的函数，或者当一个函数返回任何另一个函数时，可以选择使用Lambda函数。
<a name="aeMU2"></a>
## 什么是高阶函数?
通过一个例子来理解高阶函数。假设有一个整数列表，必须返回三个输出。

- 一个列表中所有偶数的和
- 一个列表中所有奇数的和
- 一个所有能被三整除的数的和

首先假设用普通函数来处理这个问题。在这种情况下，将声明三个不同的变量来存储各个任务，并使用一个for循环处理并返回结果三个变量。该方法常规可正常运行。<br />现在使用Lambda函数来解决这个问题，那么可以用三个不同的Lambda函数来检查一个待检验数是否是偶数，奇数，还是能被三整除，然后在结果中加上一个数。
```python
def return_sum(func, lst):
    result = 0
    for i in lst:
        # if val satisfies func
        if func(i):
            result = result + i
    return result


lst = [11, 14, 21, 56, 78, 45, 29, 28]
x = lambda a: a % 2 == 0
y = lambda a: a % 2 != 0
z = lambda a: a % 3 == 0
print(return_sum(x, lst))
print(return_sum(y, lst))
print(return_sum(z, lst))
```
这里创建了一个高阶函数，其中将Lambda函数作为一个部分传递给普通函数。其实这种类型的代码在互联网上随处可见。然而很多人在使用Python时都会忽略这个函数，或者只是偶尔使用它，但其实这些函数真的非常方便，同时也可以节省更多的代码行。接下来一起看看这些高阶函数。
<a name="apgHX"></a>
## Python内置高阶函数
<a name="XOLhq"></a>
### Map函数
`**map()**` 会根据提供的函数对指定序列做映射。<br />Map函数是一个接受两个参数的函数。第一个参数 function 以参数序列中的每一个元素调用 function 函数，第二个是任何可迭代的序列数据类型。返回包含每次 function 函数返回值的新列表。
```python
map(function, iterable, ...)
```
Map函数将定义在迭代器对象中的某种类型的操作。假设要将数组元素进行平方运算，即将一个数组的每个元素的平方映射到另一个产生所需结果的数组。
```python
arr = [2,4,6,8] 
arr = list(map(lambda x: x*x, arr)) 
print(arr)
```
可以以不同的方式使用Map函数。假设有一个包含名称、地址等详细信息的字典列表，目标是生成一个包含所有名称的新列表。
```python
students = [
            {"name": "John Doe",
             "father name": "Robert Doe",
             "Address": "123 Hall street"
             },
            {
              "name": "Rahul Garg",
              "father name": "Kamal Garg",
              "Address": "3-Upper-Street corner"
            },
            {
              "name": "Angela Steven",
             "father name": "Jabob steven",
             "Address": "Unknown"
            }
]
print(list(map(lambda student: student['name'], students)))
>>> ['John Doe', 'Rahul Garg', 'Angela Steven']
```
上述操作通常出现在从数据库或网络抓取获取数据等场景中。
<a name="Z4jOH"></a>
## Filter函数
Filter函数根据给定的特定条件过滤掉数据。即在函数中设定过滤条件，迭代元素，保留返回值为True 的元素。Map 函数对每个元素进行操作，而 filter 函数仅输出满足特定要求的元素。<br />假设有一个水果名称列表，任务是只输出那些名称中包含字符“g”的名称。
```python
fruits = ['mango', 'apple', 'orange', 'cherry', 'grapes'] 
print(list(filter(lambda fruit: 'g' in fruit, fruits)))
```
`**filter(function or None, iterable) --> filter object**`<br />返回一个迭代器，为那些函数或项为真的可迭代项。如果函数为None，则返回为真的项。
<a name="hhU9q"></a>
## Reduce函数
这个函数比较特别，不是 Python 的内置函数，需要通过`from functools import reduce` 导入。Reduce 从序列数据结构返回单个输出值，它通过应用一个给定的函数来减少元素。
```python
reduce(function, sequence[, initial]) -> value
```
将包含两个参数的函数(function)累计应用于序列(sequence)的项，从左到右，从而将序列reduce至单个值。<br />如果存在initial，则将其放在项目之前的序列，并作为默认值时序列是空的。<br />假设有一个整数列表，并求得所有元素的总和。且使用reduce函数而不是使用for循环来处理此问题。
```python
from functools import reduce
lst = [2,4,6,8,10]
print(reduce(lambda x, y: x+y, lst))
>>> 30
```
还可以使用 reduce 函数而不是for循环从列表中找到最大或最小的元素。
```python
lst = [2,4,6,8]
# 找到最大元素
print(reduce(lambda x, y: x if x>y else y, lst))
# 找到最小元素
print(reduce(lambda x, y: x if x<y else y, lst))
```
<a name="MMjKY"></a>
## 高阶函数的替代方法
<a name="fNcH4"></a>
### 列表推导式
其实列表推导式只是一个for循环，用于添加新列表中的每一项，以从现有索引或一组元素创建一个新列表。之前使用map、filter和reduce完成的工作也可以使用列表推导式完成。然而，相比于使用Map和filter函数，很多人更喜欢使用列表推导式，也许是因为它更容易应用和记忆。<br />同样使用列表推导式将数组中每个元素进行平方运算，水果的例子也可以使用列表推导式来解决。
```python
arr = [2,4,6,8]
arr = [i**2 for i in arr]
print(arr)
fruit_result = [fruit for fruit in fruits if 'g' in fruit]
print(fruit_result)
```
<a name="ziJWL"></a>
### 字典推导式
与列表推导式一样，使用字典推导式从现有的字典创建一个新字典。还可以从列表创建字典。<br />假设有一个整数列表，需要创建一个字典，其中键是列表中的每个元素，值是列表中的每个元素的平方。
```python
lst = [2,4,6,8]
D1 = {item:item**2 for item in lst}
print(D1)
>>> {2: 4, 4: 16, 6: 36, 8: 64}
# 创建一个只包含奇数元素的字典
arr = [1,2,3,4,5,6,7,8]
D2 = {item: item**2 for item in arr if item %2 != 0}
print(D2)
>>> {1: 1, 3: 9, 5: 25, 7: 49}
```
<a name="z1me0"></a>
## 一个简单应用
<a name="CmrQP"></a>
### 如何快速找到多个字典的公共键
<a name="A1oO0"></a>
#### **方法一**
```python
dl = [d1, d2, d3] # d1, d2, d3为字典，目标找到所有字典的公共键
[k for k in dl[0] if all(map(lambda d: k in d, dl[1:]))]
```
<a name="B1pUi"></a>
##### 例
```python
dl = [{1:'life', 2: 'is'}, 
      {1:'short', 3: 'i'}, 
      {1: 'use', 4: 'python'}]
[k for k in dl[0] if all(map(lambda d: k in d, dl[1:]))]
# 1
```
<a name="tdHOS"></a>
##### 解析
```python
# 列表表达式遍历dl中第一个字典中的键
[k for k in dl[0]]
# [1, 2]

# lambda 匿名函数判断字典中的键，即k值是否在其余字典中
list(map(lambda d: 1 in d, dl[1:]))
# [True, True]
list(map(lambda d: 2 in d, dl[1:]))
#[False, False]

# 列表表达式条件为上述结果([True, True])全为True,则输出对应的k值
#1
```
<a name="nEsMz"></a>
#### **方法二**
```python
# 利用集合（set）的交集操作
from functools import reduce
# reduce(lambda a, b: a*b, range(1,11)) # 10!
reduce(lambda a, b: a & b, map(dict.keys, dl))
```
<a name="QTBhO"></a>
## 写在最后
目前已经学习了Lambda函数是什么，以及Lambda函数的一些使用方法。随后又一起学习了Python中的高阶函数，以及如何在高阶函数中使用lambda函数。<br />除此之外，还学习了高阶函数的替代方法：在列表推导式和字典推导式中执行之前操作。虽然这些方法看似简单，或者说之前已经见到过这类方法，但很可能很少使用它们。可以尝试在其他更加复杂的函数中使用它们，以便使代码更加简洁。
