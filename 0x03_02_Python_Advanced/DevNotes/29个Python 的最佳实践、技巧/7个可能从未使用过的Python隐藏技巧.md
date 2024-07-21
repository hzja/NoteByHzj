Python
<a name="ac7xg"></a>
### 1、功能属性
这种 hack 类似于类和对象概念。现在，可以声明，也可以稍后在程序中使用调用的函数属性。<br />下面展示一个示例代码。
```python
# Function Attributes.
def func():
    func.name = "Haider Imtiaz"
    func.age = 22
    func.Profession = "Python developer"
func()
print("Name: ", func.name)
print("Age: ", func.age)
print("Profession: ", func.Profession)
# Output
Name:  Haider Imtiaz
Age:  22
Profession:  Python developer
```
<a name="l2vWS"></a>
### 2、不完整代码的占位符
可以使用关键字密码作为不完整代码的占位符。下面展示了一些在函数、类等中使用 Pass 关键字的例子。
```python
# Place Holders
def func():
    pass
class data:
    pass
for x in range(5):
    pass
if 2 == 4:
    pass
else:
    pass
```
<a name="DykQG"></a>
### 3、 `eval()` 函数
`Eval()` 函数接受一个字符串参数，如果字符串参数是表达式，那么 ，`eval()` 将评估表达式。<br />下面展示了一个示例代码。
```python
# Eval
#example 1
x = 5
y = eval('x + 2')
print(y) # 7
#example 2
x = 2
y = eval('x ** 3')
print(y)# 8
```
<a name="d1zR8"></a>
### 4、在 Python 解析器中使用以下命令启动文件托管服务器
```bash
# Starting a Web server
python -m http.server 5000
```
当运行上述命令时，将看到以下屏幕，该屏幕将显示启动服务器托管的 ip:port。
```bash
Serving HTTP on 0.0.0.0 port 5000 (http://0.0.0.0:5000/) ...
```
<a name="m9lWa"></a>
### 5、无限参数
这个 hack 将展示如何在函数调用中传递无限的参数。下面展示了示例代码。
```python
# Unlimited Arguments
def func(*arg):
    print("Argument Passed: ", len(arg))

func(1, 2, 4, 5, 6, 7)
# Output
# Argument Passed: 6
```
<a name="pnVYx"></a>
### 6、`Zip()` 方法
zip 方法采用两个可迭代的内容并将它们合并为一个元组，可以使用此 zip 方法来迭代列表、字典。下面展示一个示例代码以便更好地理解。
```python
# Zip method
list1 = ["Python", "JavaScript", "C#", "Dart"]
list2 = ["Machine Learning", "Web Developer", "Software", "App Dev"]
for x, y in zip(list1, list2):
    print(x, y)

# Output
Python Machine Learning
JavaScript Web Developer
C# Software
Dart App Dev
```
<a name="SPH5a"></a>
### 7、旋转列表
这个简单的技巧将展示如何轻松旋转列表。看看下面的示例代码。
```python
# Rotating list
lst = [10, 20, 30, 40, 50]
#rotate left
lst = lst[1:] + [lst[0]]
print(lst) # [20, 30, 40, 50, 10]
# rotate right
lst = [lst[-1]] + lst[:-1] 
print(lst) # [50, 10, 20, 30, 40]
```
