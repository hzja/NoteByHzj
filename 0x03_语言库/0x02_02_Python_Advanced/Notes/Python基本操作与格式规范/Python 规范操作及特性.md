Python
<a name="sNSyT"></a>
### 1.断言
Python 的断言语句是一种调试辅助功能，不是用来处理运行时错误的机制。<br />assert 在条件为 False 的时候触发，后面的内容是报错信息。
```python
import sys
assert sys.version_info >= (3, 7), "请在Python3.7及以上环境执行"
```
如果这个项目要求最低是 Python3.7 的环境，那么如果使用 Python3.6 来运行这个项目，就会出现这个错误信息。
```python
Traceback (most recent call last):
  File "/Users/chennan/pythonproject/demo/nyandemo.py", line 3, in <module>
    assert sys.version_info > (3, 7), "请在Python3.7以上环境执行"
AssertionError: 请在Python3.7以上环境执行
```
提前中止项目
<a name="MnbqD"></a>
### 2.巧妙的放置逗号
合理的格式化列表里面的元素，更容易维护<br />一般在写列表的时候会这样
```python
l = ["apple", "banana", "orange"]
```
使用下面的方式可以更加清晰的区分每一个项目，习惯性的在末尾加个逗号，防止下次添加元素遗漏了逗号，看着也更 Pythonic
```python
l = [
    "apple", 
    "banana", 
    "orange",
]
```
<a name="fJOyq"></a>
### 3.下划线、双下划线以及其他
<a name="odiHQ"></a>
#### 前置单下划线 : _var
1.是一种约定,前置单下划线的方法和变量只在内部使用<br />2.在使用通配符导包的使用 `from xx import *` 这种,不用导入前置单下划线的变量,除非定义了 `__all__` 覆盖了这个行为。_PEP8_ 一般不建议通过这种方式导包。
<a name="1PMbk"></a>
#### 后置单下划线:  var_
如果使用的变量名被 Python 中的关键字占用,比如要声明 `class` 这个变量，这时候可以在其后面加个单下划线 `class_`<br />这个也是 _PEP8_ 里约定的
<a name="VsKDT"></a>
#### 前置双下划线: __var
前置双下划线会让 Python 解释器重写属性名称，防止被子类中的命名覆盖。
```python
class Test:
    def __init__(self):
        self.foo = 11
        self.__bar = 2
t = Test()
print(dir(t))
```
查看类的属性可以发现 `self.__bar` 变为了 `_Test__bar`，这也称之为名称改写 (_name mangling_)，解释器会更改变量的名称，防止拓展这个类型时命名冲突。
```python
['_Test__bar', '__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'foo']
```
这个时候如果想访问 `__bar`, 怎么办呢，可以通过 `t._Test__bar`进行访问。<br />如果继承一下Test然后重写 `__bar`会咋样呢
```python
class ExtendTest(Test):
    def __init__(self):
        super().__init__()
        self.foo = "overridden"
        self.__bar = "overridden"
et = ExtendTest()
print(et.foo)
print(et.__bar)
```
发现出现了错误<br />`AttributeError: 'ExtendTest' object has no attribute '__bar'`<br />原因就是前面的一样，因为解释器把 `__bar`的名字给改了防止父类的这个变量被改写了。<br />可以分别访问这两个类的 `__bar`发现他们是同时存在的，确实没有被覆盖。
```python
print(et._Test__bar)
print(et._ExtendTest__bar)
```
得到结果
```
2
overridden
```
 `__bar` 在英语中一般都是叫做 `dunderbar`。<br />除了双下划线的变量，双下划线的方法名也可以被解释器名称改写。
```python
class ManglingMethod:
    def __mangled(self):
        return 42
    def call_it(self):
        return self.__mangled()
md = ManglingMethod()
md.call_it()
md.__mangled()
```
运行之后得到出错信息
```
AttributeError: 'ManglingMethod' object has no attribute '__mangled'
```
<a name="HFHui"></a>
#### 前后双下划线: __var__
所谓的魔法方法，它的名称不会被解释器所改变,但是就命名约定而言最好避免使用这种形式变量和方法名
<a name="NiBnm"></a>
#### 单下划线: _
1.`_` 可以表示变量是临时的或者是无关紧要的
```python
for _ in rang(5):
    print("hello")
```
2.在数字之前使用还可以当作是千位分隔符
```python
for i in range(1000_000):
    print(i)
```
3.在解包元组的时候可以当作是占位符。
```python
car = ("red", "auto", 12, 332.4 )
color,_,_,mileage = car
print(color)
print(_mileage)
```
4.如果使用命令行模式的话，`_` 可以获取先前计算的结果
```python
>>>  20+5
25
>>> _
25
>>> print(_)
25
```
<a name="3H3Uq"></a>
### 4.自定义异常类
有以下代码
```python
def validate(name):
    if len(name) < 10:
        raise ValueError
```
如果在其他文件中调用这个方法，
```python
validate("lisa")
```
在不理解这个方法的作用的时候，如果名字验证失败时，调用栈会打印出以下信息
```python
Traceback (most recent call last):
  File "/Users/chennan/pythonproject/demo/nyandemo.py", line 57, in <module>
    validate("lisa")
  File "/Users/chennan/pythonproject/demo/nyandemo.py", line 55, in validate
    raise ValueError
ValueError
```
这个栈调试回溯中的信息指出了，出现了错误的值，但是并不知道为什么出错了，所以这个时候就需要跟进这个 validate 一探究竟，<br />这个时候就可以自己定义一个异常类
```python
class NameTooShortException(ValueError):
   def __str__(self):
       return "输入的名字长度必须大于等于10"
def validate(name):
    if len(name) < 10:
        raise NameTooShortException(name)
validate("lisa")
```
这样如果再出现错误，就可以知道为什么错了，同时调用法也方便捕获指定的异常，不用再使用 ValueError。
```python
try:
    validate("lisa")
except NameTooShortException as e:
    print(e)
```
<a name="xIU7Y"></a>
### 5.Python字节码
Cpython 解释器执行时，首先将其翻译成一系列的字节码指令。字节码是 Python 虚拟机的中间语言，可以提高程序的执行效率<br />Cpython 不直接执行人类可读的源码，而是执行由编译器解析和语法语义分析产生的紧凑的数、变量和引用。<br />这样，再次执行相同程序时能节省时间和内存。因为编译步骤产生的字节码会以 `.pyc` 和 `.pyo` 文件的形式缓存在硬盘上，所以执行字节码比再次执行相同的Python文件速度更快。
```python
def greet(name):
    return 'hello, ' + name + '!'
#__code__可以获取greet函数用到的虚拟机指令，常量和变量
gc = greet.__code__
print(gc.co_code)  # 指令流
print(gc.co_consts)  # 常量
print(gc.co_varnames)  # 传过来的参数
dis.dis(greet)
```
结果
```python
b'd\x01|\x00\x17\x00d\x02\x17\x00S\x00'
(None, 'hello, ', '!')
('name',)
 70           0 LOAD_CONST               1 ('hello, ')
              2 LOAD_FAST                0 (name)
              4 BINARY_ADD
              6 LOAD_CONST               2 ('!')
              8 BINARY_ADD
             10 RETURN_VALUE
```
解释器在索引1处('hello, ')查找常量，并放入栈中，然后将 name 的变量内容放入栈<br />Cpython 虚拟机是基于栈式虚拟机，栈就是虚拟机的内部数据结构。<br />栈只支持两种动作:入栈和出栈<br />入栈：将一个值添加到栈顶<br />出栈:删除并返回栈顶的值。<br />假设栈初始为空，在执行前两个操作码(opcode)之后，虚拟的内容(0是最上面的元素)<br />比如传入的name为lisa.
```python
0: 'lisa'
1: 'hello, '
```
_BINARY_ADD_ 指令从栈中弹出两个字符串值，并将他们连接起来<br />然后再次将结果压入栈中。
```python
0:'hello, lisa'
```
然后由下一个 _LOAD_CONST_ 将'!'压入栈。<br />此时的结果
```python
0:'!'
1:'hello, lisa'
```
下一个 _BINARY_ADD_ 操作码再次将这两个字符串从栈中弹出并连接之后压入栈，生成最终结果
```python
0:'hello, lisa!'
```
最后字节码 _RETURN_VALUE_ ，告诉虚拟机当前位于栈顶的是该函数的返回值。
