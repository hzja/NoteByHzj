Python<br />Python 内置函数中，和作用域相关的函数有 `locals`、`global`、`vars` 以及 `dir`。日常常用这些函数，但时常分不清准确用法，试来试去。下面来分析这些函数是什么意思，有什么差别，一劳永逸学会 Python 作用域的相关知识点。<br />先说明这些函数最直接的功能，然后对特殊情况进行解释，最后说明这些函数有什么用。
<a name="OsgcO"></a>
## 最直接的功能
最直接和常用的功能如下：

- `locals()` 返回当前局部作用域的符号表字典。
- `globals()` 返回全局作用域的符号表字典。
- `vars()` 相当于 `locals()`
- `dir()` 返回当前局部作用域的符号表字典对应的 `keys()`。

来看代码。
```python
>>> import time
>>> pi = 3.1415
>>> globals()
{'__spec__': None, 'pi': 3.1415, 'time': <module 'time' (built-in)>, '__loader__': <class'_frozen_importlib.BuiltinImporter'>, '__name__': '__main__', '__doc__': None, '__package__': None, '__builtins__': <module 'builtins' (built-in)>}
```
这里导入了 `time` 模块，定义了 `pi` 全局变量。调用 `globas()` 输出全局变量，可见是一些系统相关的内置属性，以及定义的 pi 和导入的 time 。<br />出于篇幅和方便阅读考虑，之后输出都不再列出 __ 开头的属性。上例输出，去掉 __ 开头属性为：`{'pi': 3.1415, 'time': <module 'time' (built-in)>}`，阅读更清晰。<br />继续调用 `locals()` 函数，输出当前局部作用域的变量表。因为 `locals()` 位于交互 REPL 界面，所以当前局部作用域实际就是全局作用域，所以两者输出一样。
```python
>>> locals() #去掉 __ 开头的属性。
{'pi': 3.1415, 'time': <module 'time' (built-in)>}
```
继续，因为 `vars()` 相当于 `locals()`，所以代码`globals() == locals() == vars()`成立，结果为 True 。<br />最后调用 `dir()`，返回当前局部作用域的符号表字典对应的 `keys()`。
```python
>>> dir() #去掉 __ 开头的属性
['pi', 'time']
```
因为 `dir()` 返回的是局部变量字典的 keys，又因为这里局部变量和全局变量一致，加上 `vars()` 又相当于 `locals()`，所以实际上，可以得到如下等式。
```python
>>> sorted(dir()) == sorted(globals().keys()) == sorted(locals().keys()) == sorted(vars().keys())
True
```
目前为止的分析中，`locals` 和 `globals` 都在全局作用域中，vars 和 dir 都没有参数，下面分析特殊情况。
<a name="Hgmdk"></a>
## 特殊情况
先来分析当 `locals` 和 `globals` 位于函数内部时的情况。
<a name="BMwIF"></a>
### `locals` 和 `globals` 位于函数内部
看如下代码
```python
zs = 'zhangsan'

def hello(name):
    print(globals())
    print(locals())
    print(vars())
    print(dir())
    print('Hello,%s!'%name ) 

ls = 'lisi'

hello(ls) 
```
在函数内部，`global()` 返回的是全局作用域变量，所以 zs，ls 和 hello 函数都有。而 `locals()` 返回局部变量和自由变量，这里没有闭包，只有函数形参 name。`vars()` 和 `dir()` 和上例相同，结果如下：
```python
{'ls': 'lisi', 'zs': 'zhangsan', 'hello': <function hello at 0x00667DB0>} 
{'name': 'lisi'}
{'name': 'lisi'}
['name']
Hello,lisi!
```
另外，确切地说，`globals()` 返回的是函数定义时的全局变量空间。所以，如果 zs 变量和 hello 函数定义在另一个文档 lib.py 中，在本程序中 `from lib import hello` 后，再执行，返回的是定义时的全局变量：`{'zs': 'zhangsan', 'hello': <function hello at 0x00667DB0>}`不会包含调用空间的全局变量 ls。<br />下面说明当 `vars` 和 `dir` 有参数时的情况。
<a name="LoX9y"></a>
### `vars` 和 `dir` 带参数执行
`vars` 和 `dir` 函数有参数时的功能如下：

- `vars(obj)` 返回 obj 的 dict。
- `dir(obj)` 返回 obj 的属性。

`vars(obj)` 返回模块、类、实例以及有 dict 属性的对象的 dict 属性。<br />模块、类以及实例中的 dict 指代模块的命名空间或属性字典。<br />如下代码，新建一个类和类的实例，分别对类和实例进行 vars 操作，可见：
```python
class Man:
    def __init__(self,age,name):
        self.age = age
        self.name = name

zs = Man(21,'zhangsan')
print(vars(Man))
print(vars(zs))
```
这其中，类返回的是编译相关的对象，诸如弱引用、模块名、文档等。对象返回的是属性，这里是对象属性 age 和 name。
```python
{'__weakref__': <attribute '__weakref__' of 'Man' objects>, '__dict__': <attribute '__dict__' of 'Man' objects>, '__module__': '__main__', '__doc__': None, '__init__': <function Man.__init__ at 0x00657390>}
{'age': 21, 'name': 'zhangsan'}
```
`dir(obj)` 返回对象的有效属性的名字，这个有效属性根据模块、类和对象等有所不同，大概是从 dict 中提取的，但又不全然这样。<br />虽然常在交互模式下 `dir()` 查询对象，但这查询并不完全。<br />看实例和类的差别。
```python
print(set(dir(zs))-set(dir(Man)))
```
以上代码对 zs 实例和 Man 类的 `dir()` 执行结果求差集，可知对象实例与类实例相比，多了 `{'age', 'name'}` 两个属性，这也是 zs 实例的应有之义。<br />分析了半天的 `globals`、`locals`、`vars`和 `dir`，它们有什么用呢？
<a name="XX84o"></a>
## 作用
<a name="MXGJd"></a>
### 了解、分析对象用法
常在 REPL 命令行中用 `dir()` 来查看类或对象支持的方法、拥有的属性，这在认识新包、编写程序需要提示时比较有作用。比如 `import time` 后，忘记 `time` 的方法了，可以 `dir(time)` 一下。<br />`globals` 和 `locals` 可以帮助认识函数的作用域，了解当前作用域的变量，分析局部、全局、自由变量。<br />另外，借助于 `globals` 和 `locals` 可以实现动态编写代码。
<a name="BzTSF"></a>
### 动态编写代码
如下代码，用 `exec` 动态执行 Python 代码，生成新的变量 s。这里 `exec` 第一个参数时源代码，第二个参数是源代码的全局变量环境，第三个参数是源代码的局部变量环境。<br />取当前全局变量字典（为了用全局变量 math 中的 pi）做第二个参数。<br />取当前局部变量字典，并稍作修改，加入 r 局部变量，形成 loc 字典做第三个参数。
```python
import math 
loc = locals()
loc['r'] = 10
print('exec',exec('s = math.pi * r * r'),globals(),loc)

print(s)
print(locals()['s'])
```
可得计算结果 314.159。用 `print(s)` 输出，说明这里的 s 是动态生成的。又用 `locals()['s']` 输出，说明这里的 s 赋值给了局部作用域字典。
<a name="Uled3"></a>
## 总结
先说明 `globals()`、`locals()`、`vars()`、`dir()` 这些函数最直接的功能，然后对特殊情况进行解释，最后说明这些函数在理解变量作用域、分析对象、动态编码中的作用。
