Python 异常处理
<a name="thSGN"></a>
## 异常
Python中有两种错误很容易辨认：**语法错误和异常**。
<a name="HZdjY"></a>
### 语法错误
python的语法错误又称为解析错，多数是语法格式上出现问题。比如：
```python
print（'Hello World'）  #这里的括号为中文格式下的，所以报错
 File "<ipython-input-1-f552c294b592>", line 1
   print（'Hello World'）
        ^
SyntaxError: invalid character in identifier
```
<a name="DI5fb"></a>
### 处理异常
即使python程序的语法是正确的，在运行它时也可能报错。<br />运行过程中出现的错误称为异常。<br />大多数的异常不会被程序自动处理，因此产生错误信息（即报错）
<a name="e2f583cd"></a>
#### 异常捕捉可以用try/except语句
```python
while True:
   try:
       x=int(input('请输入一个数字：'))
       break
   except ValueError:
       print('您输入的不是数字，请再次尝试输入！')
'''
请输入一个数字：s
您输入的不是数字，请再次尝试输入！
请输入一个数字：3
'''
```
try 语句按照如下方式工作；

- 首先，执行 try 子句（在关键字 try 和关键字 except 之间的语句）。<br />
- 如果没有异常发生，忽略 except 子句，try 子句执行后结束。<br />
- 如果在执行 try 子句的过程中发生了异常，那么 try 子句余下的部分将被忽略。如果异常的类型和 except 之后的名称相符，那么对应的 except 子句将被执行。<br />
- 如果一个异常没有与任何的 except 匹配，那么这个异常将会传递给上层的 try 中。<br />
<a name="302f1ec9"></a>
#### 捕获指定异常
```python
try:
<语句>
except<异常名>:
print('异常说明')
一个例子：
try:
   f = open("file-not-exists", "r")   #试图打开一个本不存在的文件
except IOError as e:   #IOErro：指要打开的文件不存在的错误提示，Try语句的错误与expect匹配，执行print语句。
   print("open exception: %s: %s" %(e.errno, e.strerror))
#open exception: 2: No such file or directory
```
<a name="8a57eab4"></a>
#### 捕获多个异常
捕获多个异常有两种方式，**第一种是一个except同时处理多个异常，不区分优先级**：
```python
try:
    <语句>
except (<异常名1>, <异常名2>, ...):
print('异常说明')
```
**第二种是区分优先级的：**
```python
try: <语句>
except <异常名1>:
print('异常说明1')
except <异常名2>:
print('异常说明2')
except <异常名3>:
print('异常说明3')
```
实例如下：
```python
import sys
try:
   f = open('myfile.txt')   #打开文件
   s = f.readline()  #按行读取
   i = int(s.strip())  #都成整数类型
except OSError as err:
   print("OS error: {0}".format(err))   #文件读取错误优先解决
except ValueError:
   print("Could not convert data to an integer.")
except:
   print("Unexpected error:", sys.exc_info()[0])
   raise
#OS error: [Errno 2] No such file or directory: 'myfile.txt'
```
该种异常处理语法的规则是：

- 执行try下的语句，如果引发异常，则执行过程会跳到第一个except语句。<br />
- 如果第一个except中定义的异常与引发的异常匹配，则执行该except中的语句。<br />
- 如果引发的异常不匹配第一个except，则会搜索第二个except，允许编写的except数量没有限制。<br />
- 如果所有的except都不匹配，则异常会传递到下一个调用本代码的最高层try代码中。<br />
<a name="r6tj6"></a>
### 异常的else
如果判断完没有某些异常之后还想做其他事，就可以使用下面这样的else语句。
```python
try:
<语句>
except <异常名1>:
print('异常说明1')
except <异常名2>:
print('异常说明2')
else:
<语句>  # try语句中没有异常则执行此段代码
```
<a name="qHJ3t"></a>
### 异常的finally
try...finally...语句无论是否发生异常都将会执行最后的代码。
```python
try:
<语句>
finally:
<语句>
```
一个实例如下：
```python
str1 = 'hello world'
try:
   int(str1)
except IndexError as e:
   print(e)
except KeyError as e:
   print(e)
except ValueError as e:
   print(e)
else:
   print('try内没有异常')
finally:
   print('无论异常与否,都会执行我')
#invalid literal for int() with base 10: 'hello world'
#无论异常与否,都会执行我
```
<a name="W8Djo"></a>
### 自定义异常
<a name="VoU8F"></a>
#### 自定义类继承Exception
可以通过创建一个新的异常类来拥有自己的异常。异常类继承自 Exception 类，可以直接继承，或者间接继承。<br />在try语句块中，抛出用户自定义的异常后执行except部分，变量 e 是用于创建MyError类的实例。<br />实例如下：
```python
class MyError(Exception):
   def __init__(self, value):
       self.value = value
   def __str__(self):
       return repr(self.value)
try:
   raise MyError(2*2)
except MyError as e:
   print('My exception occurred, value:', e.value)
#My exception occurred, value: 4
raise MyError('oops!')
---------------------------------------------------------------------------
MyError                                   Traceback (most recent call last)
<ipython-input-16-3e3ae366964f> in <module>
----> 1 raise MyError('oops!')
MyError: 'oops!'
```
在上述例子中，类 Exception 默认的 __ init__() 被覆盖。<br />当创建一个模块有可能抛出多种不同的异常时，一种通常的做法是为这个包建立一个基础异常类，然后基于这个基础类为不同的错误情况创建不同的子类:
```python
class Error(Exception):
   """Base class for exceptions in this module."""
   pass
class InputError(Error):
   """Exception raised for errors in the input.
  Attributes:
      expression -- input expression in which the error occurred
      message -- explanation of the error
  """
   def __init__(self, expression, message):
       self.expression = expression
       self.message = message
class TransitionError(Error):
   """Raised when an operation attempts a state transition that's not
  allowed.
  Attributes:
      previous -- state at beginning of transition
      next -- attempted new state
      message -- explanation of why the specific transition is not allowed
  """
   def __init__(self, previous, next, message):
       self.previous = previous
       self.next = next
       self.message = message
```
大多数的异常的名字都以"Error"结尾，就跟标准的异常命名一样。
<a name="zbM5G"></a>
#### raise自定义异常类
raise语法格式如下：
```python
raise [Exception [, args [, traceback]]]
```
语句中Exception是异常的类型（例如ValueError），参数是一个异常参数值。该参数是可选的，如果不提供，异常的参数是"None"。最后一个参数是跟踪异常对象，也是可选的（在实践中很少使用）。
```python
def not_zero(num):
   try:
       if num == 0:
           raise ValueError('参数错误')
       return num
   except Exception as e:
       print(e)
not_zero(0)
#参数错误
```

