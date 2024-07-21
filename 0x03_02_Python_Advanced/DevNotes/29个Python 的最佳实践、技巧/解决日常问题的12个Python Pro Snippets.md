Python
<a name="RIyH8"></a>
### 1、多参数功能
使 Python 成为顶级编程语言的一件事是它为程序员提供了自由，可以在函数调用中传递无限的参数，而无需在函数参数中声明它们，看看下面的代码示例。
```python
# Multi Arg Function
def test(*arg):
    print(arg) # (1, 3, 60, 23, 435)
test(1, 3, 60, 23, 435)
```
<a name="egsda"></a>
### 2、Multiple Input
可以获取多个输入并将它们划分为不同的变量，在下面的示例中，使用了 input 方法和 `split()` 方法，它们将根据空格对输入数据进行切片。
```python
# Multiple Input
x, y, z = input("Enter data: ").split()
print(x, y, z)
```
<a name="iDKOA"></a>
### 3、`Time.Sleep`
`Time.sleep()` 是一个 Python 内置模块，可以让程序休眠几分钟、几小时和几秒钟，只需要导入时间模块，然后调用 `time.sleep()` 并在括号中传递一个数字。请记住，python 在该模块中始终将数字计为秒。
```python
for x in range(1, 10):
    print(x)
    time.sleep(5)
# output:
# 1
# sleep for 5
# 2
# sleep for 5
# .
# .
# 10
```
<a name="J05jU"></a>
### 4、Pro Swapping
曾使用临时变量或第三方变量进行交换，但 Python 有一个很棒的功能，允许使用更少的代码和没有临时变量进行交换。看看下面的代码示例。
```python
# Pro Swaping
v1 = 10
v2 = 20
#Long way
temp=v1
v1 = v2
v2 = temp
print(v1, v2) # 20 10
# Pro Way
[v2, v1] = [v1, v1]
print(v1, v2) # 20 10
```
<a name="yY08Y"></a>
### 5、Fetch Vowels
现在可以更快、更轻松地获取vowels，使用以下代码片段示例从任何字符串数据中获取vowels。
```python
# Fetch Vowels
def Fetch_Vowels(data):
    return [x for x in data if x in 'aeiou']
print(Fetch_Vowels("World of Codding")) # ['o', 'o', 'o', 'i']
print(Fetch_Vowels("Program")) # ['o', 'a']
```
<a name="Nxzrp"></a>
### 6、找到最常见的值
假设有一长串重复元素，任务是在其中找到最频繁或最常出现的值。每个程序员的第一个方法是使用循环方法，但是这段代码会节省大部分时间。
```python
# Find most frequent
def most_freq(list):
    return max(set(list), key=list.count)
Test = [10, 10, 20, 20, 10, 30, 30, 30, 20, 10]
print(most_freq(Test)) # 10
```
<a name="m7c1a"></a>
### 7、隐藏输入密码
Python 中没有隐藏密码的功能，但是可以使用内置模块名称 getpass 来隐藏输入中的密码。<br />该模块的工作方式与 Linux 密码输入相同，可以在其中输入密码，但在没有完成输入之前无法在控制台屏幕上看到它。
```python
# hidden Password
import getpass
password = getpass.getpass('Type Password: ')
print(password)
```
<a name="ockLt"></a>
### 8、重置递归限制
现在，此代码段将帮助你将递归限制重置为任意数字，默认设置为 1000，这意味着如果达到 1000 递归限制，程序将抛出错误，为了避免这种情况，可以使用以下代码段。
```python
import sys
print(sys.getrecursionlimit()) # 1000
sys.setrecursionlimit(1010)
print(sys.getrecursionlimit()) # 1010
```
<a name="wdlxe"></a>
### 9、打印K次字符串
这个简单的代码片段将帮助你在控制台屏幕上打印字符串 K 次，而无需使用循环。当需要生成相同的字符串模式时，这会派上用场。
```python
# K times Strings
py = "GoPython "
print(py * 3) # GoPython GoPython GoPython
print(py * 5) # GoPython GoPython GoPython GoPython GoPython
```
<a name="qZevy"></a>
### 10、使用方法反向列表
你可能使用循环方式来反转 Python 中的列表，但是通过使用内置方法反转列表来简化操作。
```python
# Reverse list with Method
mylist = ["X", "Y", "Z"]
mylist.reverse()
print(mylist) # ['Z', 'Y', 'X']
```
<a name="vmOHS"></a>
### 11、获取随机数
此代码段将帮助你使用内置模块 random 从任何范围获取随机数。
```python
# Generate Randome Number
import random
print(random.randint(1, 10)) # some random 10
print(random.randint(5, 30)) # some random 30
print(random.randint(91, 230)) # some random 94
```
<a name="h69Lp"></a>
### 12、在运行时获取 Python 版本
现在，可以使用以下代码段在运行时检查 python 版本。
```python
# Get Python Version on Runtime
import platform
print(platform.python_version()) # 3.7.9
```
