Python
<a name="K3fkW"></a>
## 1、列表Stepping
这是一个 step 参数，可以通过采取几个步骤来分割你的列表。此外，可以使用 step 参数来反转整数。看看下面的代码示例：
```python
# 列表Stepping
data = [10, 20, 30, 40, 50]
print(data[::2]) # [10, 30, 50]
print(data[::3]) # [10, 40]
# 使用 stepping 翻转列表
print(data[::-1]) # [50, 40, 30, 20, 10]
```
<a name="QAxuz"></a>
## 2、`find()` 方法
`find()` 方法是一个很棒的功能，可以查找字符串中任何字符的任何起始索引号：
```python
# 查找方法
x = "Python"
y = "Hello From Python"
print(x.find("Python")) # 0
print(y.find("From"))  # 6
print(y.find("From Python")) #6
```
<a name="Yo5cG"></a>
## 3、`iter()`函数
`iter()` 方法对于没有任何循环帮助的迭代列表很有用。这是一个内置方法，因此不需要任何模块：
```python
# Iter() 
values = [1, 3, 4, 6]
values = iter(values) 
print(next(values)) # 1 
print(next(values)) # 3 
print(next(values)) # 4 
print(next(values)) # 6
```
<a name="ukdGE"></a>
## 4、Python 中的文档测试
Doctest 功能将让你测试你的功能并显示你的测试报告。如果你检查下面的示例，需要在三引号中编写一个测试参数，如下所示：
```python
# Doctest 
from doctest import testmod 
def Mul(x, y) -> int: 
    """
   这个函数返回 x 和 y 参数的 mul
   调用函数，然后是预期的输出：
   >>> Mul(4, 5) 
   20 
   >> > Mul(19, 20) 
   39 
   """ 
   return x * y 
# 调用 testmod 函数
testmod(name='Mul', verbose=True)
```
<a name="QzvgH"></a>
#### **输出：**
```
Trying:
    Mul(4, 5)
Expecting:
    20
ok
Trying:
    Mul(19, 20)
Expecting:
    39
**********************************************************************
File "__main__", line 10, in Mul.Mul
Failed example:
    Mul(19, 20)
Expected:
    39
Got:
    380
1 items had no tests:
Mul
**********************************************************************
1 items had failures:
   1 of   2 in Mul.Mul
2 tests in 2 items.
1 passed and 1 failed.
***Test Failed*** 1 failures.
```
<a name="xaBbk"></a>
## 5、Yield声明
Yield 语句是 Python 的另一个令人惊叹的特性，它的工作方式类似于 return 语句，但它不是终止函数并返回，而是返回到它返回给调用者的点：
```python
# Yield声明
def func(): 
    print(1) 
    yield 1 
    print(2) 
    yield 2 
    print(3) 
    yield 3 
    
for call in func(): 
    pass# 输出
# 1 
# 2 
# 3
```
<a name="bv0Bf"></a>
## 6、处理字典缺失键
有时正在访问的键不存在于字典中，这会导致键错误。为了处理丢失的键，可以使用**get() 方法**而不是**括号方法：**
```python
# 处理字典中的缺失值
dict_1 = {1："x"，2："y"}
# 不要使用get
print(dict_1[3]) # key error
# 使用get  
print(dict_1.get(3)) #  None
```
<a name="a36LW"></a>
## 7、For/Else 和 While/Else
你知道 Python 还支持带有 For 和 While 循环的 Else 吗？当你的循环完成其迭代而没有任何中断时，将执行此 else 语句。<br />下面的 For 循环示例 else 将执行，但在 While 循环中，添加了一个不会触发 else 语句的 break 语句：
```python
# 为/否则  
for x in range(5): 
    print(x) 
else: 
    print("Loop Completed") # 执行# While / Else 
i = 0 
while i < 5: 
    break 
else: 
    print("Loop Completed") # 未执行
```
<a name="EeDya"></a>
## 8、命名字符串格式化
此功能将替换字符串占位符中的值。当需要在字符串的不同占位符中添加值时，这会派上用场：
```python
# 命名格式化字符串
a = "Python" 
b = "工作"# Way 1 
string = "I looking for a {} Programming {}".format(a, b) 
print(string) # I looking for a Python Programming Job

#Way 2 
string = f"I looking for a {a} Programming {b}" 
print(string) # I looking for a Python Programming Job
```
<a name="LdYzw"></a>
## 9、设置递归限制
这是 Python 的另一个很棒的特性，它可以让你设置 Python 程序的递归限制。请查看以下代码示例以更好地理解：
```python
# 设置递归限制
import sys
sys.setrecursionlimit = 2000
print(sys.getrecursionlimit) # 2000
```
<a name="ku0vQ"></a>
## 10、条件参数
条件赋值功能使用三元运算符，可以根据特定条件在变量中赋值。看看下面的代码示例：
```python
# 条件参数
x = 5 if 2 > 4 else 2
print(x) # 2
y = 10 if 32 > 41 else 24
print(y) # 24
```
<a name="p8SXx"></a>
## 11、参数拆包
可以解压缩函数中的任何可迭代数据参数。看看下面的代码示例：
```python
# 参数解包  
def func(x, y): 
    print(x, y) 
list_1 = [100, 200] 
dict_1 = {'x':300, 'y':400} 
func(*list_1) 
func(**dict_1)  
# 输出
# 100 200 
# 300 400
```
<a name="kPXgO"></a>
## 12、Hello World!
如果键入以下代码并运行它，这是一个有趣的功能。Python 会用两个著名的词来迎接你。试试看。
```python
import __hello__
```
<a name="ihay8"></a>
## 13、多行字符串
此功能将向你展示如何编写不带三引号的多行字符串。看看下面的代码示例：
```python
# 多行字符串
str1= "Python学习" \ 
      "欢迎来 " \
      "Study"
print(str1) 
# Python学习，欢迎来 Study
```
