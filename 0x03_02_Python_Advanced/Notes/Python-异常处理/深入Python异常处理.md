Python 异常处理<br />通常来说，程序中的错误至少包括两种，一种是语法错误，另一种则是异常。
<a name="mypXu"></a>
## 语法错误
所谓语法错误，应该很清楚，也就是写的代码不符合编程规范，无法被识别与执行，比如下面这个例子的语法错误<br />下面的代码无法被识别和执行
```python
if name is not None
    print(name)
```
上面的代码If 语句漏掉了冒号，不符合 Python 的语法规范，所以程序就会报错invalid syntax。
<a name="MMpMt"></a>
## 异常
异常则是指程序的语法正确，也可以被执行，但在执行过程中遇到了错误，抛出了异常。<br />比如，最常见的除数不能为0。变量没有定义。数据类型的运算。
```python
10 / 0
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ZeroDivisionError: integer division or modulo by zero
```
```python
order * 2
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'order' is not defined
```
```python
1 + [1, 2]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: unsupported operand type(s) for +: 'int' and 'list'
```
上面报的是常见的报错，比如ZeroDIvision NameError 和 typeError<br />还有很多其他异常的类型如keyError 字典的键找不到和FileNotFoundError 文件不存在
<a name="IaJjX"></a>
## `try except`
使用Python的异常处理语句，可以非常优雅地处理发生的异常。<br />下面是Python的异常处理语句的模板语法
```python
try:
    # 可能触发异常的语句块
except:  
    # 这里执行异常处理的相关代码，打印输出等
else:
    # 如果没有异常则执行else中的代码
finally：
    # 不管代码是否异常，都会执行，一般是资源的关闭和释放
```
首先，检测try语句块中的错误，except语句捕获异常信息并处理。如果在try子句执行时没有发生异常，Python将执行else语句后的语句，然后控制流通过整个try语句。<br />比如看下面的例子。
```python
try:
    print(a*2)
except TypeError:
    print("TypeError")
except:
    print("Not Type Error & Error noted")
```
```python
Not Type Error & Error noted
```
由于a没有定义，报的是NameError而不是TypeError。异常最终被except:部分的程序捕捉。<br />这里，Runsen补充一下非常重要的知识点：
<a name="mfbaX"></a>
## 抛出异常
抛出异常模板：raise异常类名(附加异常信息) 。下面是示例代码：
```python
s = "RunsenRunsen"
try:
    if len(s) > 10:
        raise Exception("超过10个字符")
except Exception as err:
    print(err)
```
```python
超过10个字符
```
有时产生的异常，不想在当前处理，那么就可以使用raise抛出异常。下面是示例代码：
```python
def division():
    a = float(input('输入被除数：'))
    b = float(input("输入除数："))
    if a < 0 or b < 0:
        raise Exception("我是Runsen，要求：输入的数不能小于0。") #出现负数抛出异常。
    c = a / b
    print(a,'÷',b,'=',c)
    
try:
    division()
except Exception as d:
    print('出错了，',d)
```
运行结果：
```python
输入被除数：5
输入除数：-1
出错了， 我是小学生，输入的数不能小于0。
输入被除数：5
输入除数：0
出错了， float division by zero
```
<a name="aLNNX"></a>
## 万能异常
因为异常分了不同的种类，如果不知道，那么使用exception异常处理就足够了，它可以接收任何异常
```python
value = 'hello'
try:
    int(value)
#万能异常处理  
except Exception as e:
    print(e)
```
<a name="Dva03"></a>
## 自定义异常
实际开发中，有时候系统提供的异常类型不能满足开发的需求。这时候可以通过创建一个新的异常类来拥有自己的异常。异常类继承自 Exception 类，可以直接继承，或者间接继承。
```python
# 自定义异常类 MyError ，继承普通异常基类 Exception
class MyError(Exception):
        def __init__(self, value):
            self.value = value
        def __str__(self):
            return repr(self.value)
try:
    num = input("请输入数字：")
    if not num.isdigit():  # 判断输入的是否是数字
        raise MyError(num)  # 输入的如果不是数字，手动指定抛出异常
except MyError as e:
    print("MyError：请输入数字。您输入的是：", e.value)
```
```python
请输入数字：1
请输入数字：Runsen
MyError：请输入数字。您输入的是：Runsen
```
<a name="Lup2i"></a>
## 扩展
大型社交网站的后台，需要针对用户发送的请求返回相应记录。用户记录往往储存在 key-value 结构的数据库中，每次有请求过来后，拿到用户的 ID，并用 ID 查询数据库中此人的记录，就能返回相应的结果。而数据库返回的原始数据，往往是 json string 的形式，这就需要首先对 json string 进行 decode（解码），可能很容易想到下面的方法：
```python
import json
raw_data = queryDB(uid) # 根据用户的 id，返回相应的信息。queryDB这里是一个函数
data = json.loads(raw_data)
```
上面的代码是不是就足够呢？<br />`json.loads()`函数中，如果输入的字符串不符合规范，那么就无法解码，就会抛出异常。<br />raw_data一定是json吗？，因此写之前就应该考虑如何处理异常
```python
try:
    data = json.loads(raw_data)
except JSONDecodeError as err:
    print('JSONDecodeError: {}'.format(err))
```
| 异常名称 | 描述 |
| --- | --- |
| BaseException | 所有异常的基类 |
| SystemExit | 解释器请求退出 |
| KeyboardInterrupt | 用户中断执行(通常是输入^C) |
| Exception | 常规错误的基类 |
| StopIteration | 迭代器没有更多的值 |
| GeneratorExit | 生成器(generator)发生异常来通知退出 |
| StandardError | 所有的内建标准异常的基类 |
| ArithmeticError | 所有数值计算错误的基类 |
| FloatingPointError | 浮点计算错误 |
| OverflowError | 数值运算超出最大限制 |
| ZeroDivisionError | 除(或取模)零 (所有数据类型) |
| AssertionError | 断言语句失败 |
| AttributeError | 对象没有这个属性 |
| EOFError | 没有内建输入,到达EOF 标记 |
| EnvironmentError | 操作系统错误的基类 |
| IOError | 输入/输出操作失败 |
| OSError | 操作系统错误 |
| WindowsError | 系统调用失败 |
| ImportError | 导入模块/对象失败 |
| LookupError | 无效数据查询的基类 |
| IndexError | 序列中没有此索引(index) |
| KeyError | 映射中没有这个键 |
| MemoryError | 内存溢出错误(对于Python 解释器不是致命的) |
| NameError | 未声明/初始化对象 (没有属性) |
| UnboundLocalError | 访问未初始化的本地变量 |
| ReferenceError | 弱引用(Weak reference)试图访问已经垃圾回收了的对象 |
| RuntimeError | 一般的运行时错误 |
| NotImplementedError | 尚未实现的方法 |
| SyntaxError | Python 语法错误 |
| IndentationError | 缩进错误 |
| TabError | Tab 和空格混用 |
| SystemError | 一般的解释器系统错误 |
| TypeError | 对类型无效的操作 |
| ValueError | 传入无效的参数 |
| UnicodeError | Unicode 相关的错误 |
| UnicodeDecodeError | Unicode 解码时的错误 |
| UnicodeEncodeError | Unicode 编码时错误 |
| UnicodeTranslateError | Unicode 转换时错误 |
| Warning | 警告的基类 |
| DeprecationWarning | 关于被弃用的特征的警告 |
| FutureWarning | 关于构造将来语义会有改变的警告 |
| OverflowWarning | 旧的关于自动提升为长整型(long)的警告 |
| PendingDeprecationWarning | 关于特性将会被废弃的警告 |
| RuntimeWarning | 可疑的运行时行为(runtime behavior)的警告 |
| SyntaxWarning | 可疑的语法的警告 |
| UserWarning | 用户代码生成的警告 |

