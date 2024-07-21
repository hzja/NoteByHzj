Python<br />有用的 Python 单行代码片段，只需一行代码即可解决特定编码问题！
<a name="WOdkT"></a>
## 1、一行 For 循环
for 循环是一个多行语句，但是在 Python 中，可以使用列表推导式方法在一行中编写 for 循环。以过滤小于**250**的值为例，查看下面的代码示例。
```python
#For循环在一行
mylist = [200, 300, 400, 500]
#正常方式
result = [] for x in mylist: 
    if x > 250: 
        result.append(x) 
print(result) # [300, 400, 500]#一行代码方式
result = [x for x in mylist if x > 250] 
print(result) # [300, 400, 500]
```
<a name="wwTvr"></a>
## 2、一行 While 循环
这个 One-Liner 片段将展示如何在一行中使用 While 循环代码，已经展示了两种方法。
```python
#方法 1 Single Statement 
while True: print(1) #infinite 1
	#方法 2 多语句
x = 0 
while x < 5: print(x); x= x + 1 # 0 1 2 3 4 5
```
<a name="L39QM"></a>
## 3、一行 IF Else 语句
要在一行中编写 IF Else 语句，将使用三元运算符。三元的语法是**“[on true] if [expression] else [on false]”**。<br />在下面的示例代码中展示了 3 个示例，以清楚地了解如何将三元运算符用于一行 if-else 语句。要使用 Elif 语句，必须使用多个三元运算符。
```python
#if Else 在一行中
#Example 1 if else
print("Yes") if 8 > 9 else print("No") # No
#Example 2 if elif else 
E = 2 
print("High") if E == 5 else print("数据STUDIO") if E == 2 else 
print("Low") # 数据STUDIO 

#Example 3 only if
if 3 > 2: print("Exactly") # Exactly
```
<a name="ayIs9"></a>
## 4、一行合并字典
这个 单行代码段将展示如何使用一行代码将两个字典合并为一个。下面展示了两种合并字典的方法。
```python
# 在一行中合并字典
d1 = { 'A': 1, 'B': 2 } 
d2 = { 'C': 3, 'D': 4 }
#方法 1 
d1.update(d2) 
print(d1) # {'A': 1, 'B': 2, 'C': 3, 'D': 4}
#方法 2 
d3 = {**d1, **d2} 
print(d3) # {'A': 1, 'B': 2, 'C': 3, 'D': 4}
```
<a name="pMQfU"></a>
## 5、一行函数
有两种方法可以在一行中编写函数，在第一种方法中，将使用与三元运算符或单行循环方法相同的函数定义。<br />第二种方法是用 lambda 定义函数。查看下面的示例代码以获得更清晰的理解。
```python
#函数在一行中
#方法一
def fun(x): return True if x % 2 == 0 else False 
print(fun(2)) # False
#方法2
fun = lambda x : x % 2 == 0 
print(fun(2)) # True 
print(fun(3)) # False
```
<a name="LxDp7"></a>
## 6、一行递归
这个单行代码片段将展示如何在一行中使用递归。将使用一行函数定义和一行 if-else 语句。下面是查找斐波那契数的示例。
```python
# 单行递归
#Fibonaci 单行递归示例
def Fib(x): return 1 if x in {0, 1} else Fib(x-1) + Fib(x-2)
print(Fib(5)) # 8
print(Fib(15)) # 987
```
<a name="aehLj"></a>
## 7、一行数组过滤
Python 列表可以通过使用列表推导方法在一行代码中进行过滤。以过滤偶数列表为例。
```python
# 一行中的数组过滤
mylist = [2, 3, 5, 8, 9, 12, 13, 15]
#正常方式
result = [] 
for x in mylist: 
    if x % 2 == 0: 
        result.append(x)
print(result) # [2, 8, 12]
# 单线方式
result = [x for x in mylist if x % 2 == 0] 
print(result) # [2, 8, 12]
```
<a name="JY5aF"></a>
## 8、一行异常处理
使用异常处理来处理 Python 中的运行时错误。可以在一行中编写这个 Try except 语句吗？通过使用 `**exec()**` 语句，可以做到这一点。
```python
# 一行异常处理
#原始方式
try:
    print(x)
except:
    print("Error")
#单行方式
exec('try:print(x) \nexcept:print("Error")') # 错误
```
<a name="bB0Z2"></a>
## 9、一行列表转字典
可以使用 Python **enumerate()** 函数将 List 转换为一行字典。在`**enumerate()**` 中传递列表并使用`**dict()**` 将最终输出转换为字典格式。
```python
# 字典在一行
mydict = ["John", "Peter", "Mathew", "Tom"]
mydict = dict(enumerate(mydict))
print(mydict) # {0: 'John', 1: 'Peter', 2: 'Mathew', 3: 'Tom'}
```
<a name="RURxr"></a>
## 10、一行多变量
Python 允许在一行中进行多个变量赋值。下面的示例代码将展示如何做到这一点。
```python
#多行变量
#正常方式
x = 5 
y = 7 
z = 10 
print(x , y, z) # 5 7 10
#单行方式
a, b, c = 5, 7, 10 
print(a, b, c) # 5 7 10
```
<a name="L3SF7"></a>
## 11、一行交换值
交换是编程中一项有趣的任务，并且总是需要第三个变量名称 temp 来保存交换值。这个单行代码段将向你展示如何在没有任何临时变量的情况下交换一行中的值。
```python
#换成一行
#正常方式
v1 = 100
v2 = 200
temp = v1
v1 = v2 
v2 = temp
print(v1, v2) # 200 100
# 单行交换
v1, v2 = v2, v1 
print(v1, v2) # 200 100
```
<a name="UAYVS"></a>
## 12、一行排序
排序是编程中的一个普遍问题，Python 有许多内置的方法来解决这个排序问题。下面的代码示例将展示如何在一行中进行排序。
```python
# 在一行中排序
mylist = [32, 22, 11, 4, 6, 8, 12] 
# 方法 1
mylist.sort() 
print(mylist) # # [4, 6, 8, 11, 12, 22, 32]
print(sorted(mylist)) # [4, 6, 8, 11, 12, 22, 32]
```
<a name="rbOam"></a>
## 13、一行读取文件
不使用语句或正常读取方法，也可以正确读取一行文件。
```python
#一行读取文件
#正常方式
with open("data.txt", "r") as file: 
    data = file.readline() 
    print(data) # Hello world
#单行方式
data = [line.strip() for line in open("data.txt","r")] 
print(data) # ['hello world', 'Hello Python']
```
<a name="c2qI5"></a>
## 14 一行类
上课总是多线工作。但是在 Python 中，有一些方法可以在一行代码中使用类特性。
```python
# 一行中的类
#普通方式
class Emp: 
    def __init__(self, name, age): 
        self.name = name 
        self.age = age
        emp1 = Emp("云朵君", 22) 
print(emp1.name, emp1.age) # 云朵君 22
#单行方式
#方法 1 带有动态 Artibutes 的 Lambda

Emp = lambda:None; Emp.name = "云朵君"; Emp.age = 22
print(Emp.name, Emp.age) # 云朵君 22

#方法 2 
from collections import namedtuple
Emp = namedtuple('Emp', ["name", "age"]) ("云朵君", 22) 
print(Emp.name, Emp.age) # 云朵君 22
```
<a name="KtQVg"></a>
## 15、一行分号
一行代码片段中的分号将展示如何使用分号在一行中编写多行代码。
```python
# 一行分号
# 例 1 
a = "Python"; b = "编程"; c = "语言"; print(a, b, c)
# 输出
# Python 编程语言
```
<a name="Io4k7"></a>
## 16、一行打印
这不是很重要的片段，但有时当不需要使用循环来执行任务时它很有用。
```python
# 一行打印
#正常方式
for x in range(1, 5):
    print(x) # 1 2 3 4
#单行方式
print(*range(1, 5)) # 1 2 3 4
print(*range(1, 6)) # 1 2 3 4 5
```
<a name="MmGAn"></a>
## 17、一行map函数
Map 函数是适用的高阶函数。这将函数应用于每个元素。下面是如何在一行代码中使用 map 函数的示例。
```python
#在一行中map
print(list(map(lambda a: a + 2, [5, 6, 7, 8, 9, 10])))
# 输出
# [7, 8, 9, 10, 11, 12]
```
<a name="UIL3r"></a>
## 18、删除列表第一行中的 Mul 元素
现在可以使用 `**del**` 方法在一行代码中删除 List 中的多个元素，而无需进行任何修改**。**
```python
# 删除一行中的Mul元素
mylist = [100, 200, 300, 400, 500]
del mylist[1::2]
print(mylist) # [100, 300, 500]
```
<a name="Lavx4"></a>
## 19、一行打印图案
现在不再需要使用**for**循环来打印相同的图案。可以使用 `**print**` 语句和星号 (*) 在一行代码中执行相同的操作。
```python
# 在一行中打印图案# 
# 正常方式
for x in range(3):
    print('😀')
# 输出
# 😀 😀 😀
#单行方式
print('😀' * 3) # 😀 😀 😀 
print('😀' * 2) # 😀 😀 
print('😀' * 1) # 😀
```
<a name="V4WWJ"></a>
## 20、一行查找质数
此代码段将展示如何编写单行代码来查找范围内的素数。
```python
# 查找质数
print(list(filter(lambda a: all(a % b != 0 for b in range(2, a)),
                  range(2,20))))
# 输出
# [2, 3, 5, 7, 11, 13, 17, 19]
```
