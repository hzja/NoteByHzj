# try模块



## 什么是异常？

异常即是一个事件，该事件会在程序执行过程中发生，影响了程序的正常执行。

一般情况下，在Python无法正常处理程序时就会发生一个异常。

异常是Python对象，表示一个错误。

当Python脚本发生异常时我们需要捕获处理它，否则程序会终止执行。



## 异常处理

1. 可有效避免程序报错退出
2. 可获取具体错误信息
   + <b>try</b>：(“try中代码没出现异常时执行try分支”)
   + <b>except</b>：(“try中代码出现异常时执行except分支”)
   + <b>else</b>：(“try中代码没出现异常时执行else分支”)
   + <b>finally</b>：(“无论try中是否出现异常最终都执行finally”)
   + <b>raise</b>：("可创建异常")



## 示例

~~~ python
try:  # try后边必须跟随except/finally,可以不跟随else
    print(num)
    
except FileNotFoundError:  # 只捕获指定类型的错误
    print("提示用户: 文件不存在")
except NameError as error:  # 使用as error 可以获取到具体的错误信息
    print("提示开发者: 出现命名错误 %s" % error)
except BaseException as error:  # BaseException可以捕获所有类型的错误  是所有异常类的基类
    print("出现异常 %s" % error)
else:
    print("try中代码没有出现异常时,执行else分支")
finally:
    print("无论try中是否出现异常,最终都会执行finally")
print("一些处理")

>>>提示开发者: 出现命名错误 name 'num' is not defined
>>>无论try中是否出现异常,最终都会执行finally
>>>一些处理
~~~



## 自定义异常

通过创建一个新异常类，程序可命名其异常。异常应该是典型的继承自Exception类，通过直接或间接方式，一旦执行了raise语句则raise语句后语句将不能执行，但前面的仍可执行

~~~ python
class PhoneNumError(Exception):  # 自定义异常必须继承Exception
    pass

# 在你定义以上类后，你可以触发该异常，如下所示
try:
    phone_num = input("请输入手机号:")
    if not phone_num.isdecimal():  # 不全是数字
        raise PhoneNumError("包含非数字字符")
    elif len(phone_num) != 11:  # 手机号码位数错误
        raise PhoneNumError("手机号码位数错误")
except PhoneNumError as error:
    print("提示用户: 手机号码错误 %s" % error)
print("保存数据")
~~~



## python标准异常

| 异常名称                  | 描述                                               |
| ------------------------- | -------------------------------------------------- |
|                           |                                                    |
| BaseException             | 所有异常的基类                                     |
| SystemExit                | 解释器请求退出                                     |
| KeyboardInterrupt         | 用户中断执行(通常是输入^C)                         |
| Exception                 | 常规错误的基类                                     |
| StopIteration             | 迭代器没有更多的值                                 |
| GeneratorExit             | 生成器(generator)发生异常来通知退出                |
| StandardError             | 所有的内建标准异常的基类                           |
| ArithmeticError           | 所有数值计算错误的基类                             |
| FloatingPointError        | 浮点计算错误                                       |
| OverflowError             | 数值运算超出最大限制                               |
| ZeroDivisionError         | 除(或取模)零 (所有数据类型)                        |
| AssertionError            | 断言语句失败                                       |
| AttributeError            | 对象没有这个属性                                   |
| EOFError                  | 没有内建输入,到达EOF 标记                          |
| EnvironmentError          | 操作系统错误的基类                                 |
| IOError                   | 输入/输出操作失败                                  |
| OSError                   | 操作系统错误                                       |
| WindowsError              | 系统调用失败                                       |
| ImportError               | 导入模块/对象失败                                  |
| LookupError               | 无效数据查询的基类                                 |
| IndexError                | 序列中没有此索引(index)                            |
| KeyError                  | 映射中没有这个键                                   |
| MemoryError               | 内存溢出错误(对于Python 解释器不是致命的)          |
| NameError                 | 未声明/初始化对象 (没有属性)                       |
| UnboundLocalError         | 访问未初始化的本地变量                             |
| ReferenceError            | 弱引用(Weak reference)试图访问已经垃圾回收了的对象 |
| RuntimeError              | 一般的运行时错误                                   |
| NotImplementedError       | 尚未实现的方法                                     |
| SyntaxError               | Python 语法错误                                    |
| IndentationError          | 缩进错误                                           |
| TabError                  | Tab 和空格混用                                     |
| SystemError               | 一般的解释器系统错误                               |
| TypeError                 | 对类型无效的操作                                   |
| ValueError                | 传入无效的参数                                     |
| UnicodeError              | Unicode 相关的错误                                 |
| UnicodeDecodeError        | Unicode 解码时的错误                               |
| UnicodeEncodeError        | Unicode 编码时错误                                 |
| UnicodeTranslateError     | Unicode 转换时错误                                 |
| Warning                   | 警告的基类                                         |
| DeprecationWarning        | 关于被弃用的特征的警告                             |
| FutureWarning             | 关于构造将来语义会有改变的警告                     |
| OverflowWarning           | 旧的关于自动提升为长整型(long)的警告               |
| PendingDeprecationWarning | 关于特性将会被废弃的警告                           |
| RuntimeWarning            | 可疑的运行时行为(runtime behavior)的警告           |
| SyntaxWarning             | 可疑的语法的警告                                   |
| UserWarning               | 用户代码生成的警告                                 |