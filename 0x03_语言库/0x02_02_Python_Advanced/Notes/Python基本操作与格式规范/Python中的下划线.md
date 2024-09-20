Python<br />Python有很多地方使用下划线。在不同场合下，有不同含义：比如`_var`表示内部变量；`__var`表示私有属性；`__var__`表示魔术方法；这些含义有的是程序员群体的约定，如`_var`；有的是Python解释器规定的形式，如`__var`。<br />目前常见的用法有五种：

- `_`用于临时变量
- `var_`用于解决命名冲突问题
- `_var`用于保护变量
- `__var`用于私有变量
- `__var__`用于魔术方法

下面具体看看这些下划线应用场景。
<a name="ypqti"></a>
## 一、_用于临时变量
单下划线一般用于表示临时变量，在REPL、for循环和元组拆包等场景中比较常见。
<a name="K4pcy"></a>
### 1.1 REPL
单下划线在REPL中关联的是上一次计算的非None结果。
```python
>>> 1+1
2
>>> _
2
>>> a=2+2
>>> _
2
```
`1+1`，结果为2，赋值给`_`；而赋值表达式`a=2+2a`为`4`，但整个表达式结果为`None`，故不会关联到`_`。这有点类似日常大家使用的计算器中的ANS按键，直接保存了上次的计算结果。
<a name="MVRe5"></a>
### 1.2 for循环中的`_`
for循环中`_`作为临时变量用。下划线来指代没什么意义的变量。例如在如下函数中，只关心函数执行次数，而不关心具体次序的情况下，可以使用`_`作为参数。
```python
nums = 13
for _ in range(nums):
    fun_oper()
```
<a name="fWBNw"></a>
### 1.3 元组拆包中的`_`
第三个用法是元组拆包，赋值的时候可以用_来表示略过的内容。如下代码忽略北京市人口数，只取得名字和区号。
```python
>>> city,_,code = ('Beijing',21536000,'010')
>>> print(city,code)
Beijing 010
```
如果需要略过的内容多于一个的话，可以使用`*`开头的参数，表示忽略多个内容。如下代码忽略面积和人口数，只取得名字和区号
```python
city,*_,code = ('Beijing',21536000,16410.54,'010')
```
<a name="cjJZ7"></a>
### 1.4 国际化函数
在一些国际化编程中，`_`常用来表示翻译函数名。例如`gettext`包使用时：
```python
import gettext
zh = gettext.tranlation('dict','locale',languages=['zh_CN'])
zh.install()
_('hello world')
```
依据设定的字典文件，其返回相应的汉字“你好世界”。
<a name="w9BpQ"></a>
### 1.5 大数字表示形式
`_`也可用于数字的分割，这在数字比较长的时候常用。
```python
>>> a = 9_999_999_999
>>> a
9999999999
```
a的值自动忽略了下划线。这样用`_`分割数字，有利于便捷读取比较大的数。
<a name="UiiPz"></a>
## 二、var_用于解决命名冲突问题
变量后面加一个下划线。主要用于解决命名冲突问题，元编程中遇时Python保留的关键字时，需要临时创建一个变量的副本时，都可以使用这种机制。
```python
def type_obj_class(name,class_):
    pass

def tag(name,*content,class_):
    pass
```
以上代码中出现的`class`是Python的保留关键字，直接使用会报错，使用下划线后缀的方式解决了这个问题。
<a name="GhmJG"></a>
## 三、`_var`用于保护变量
前面一个下划线，后面加上变量，这是仅供内部使用的“保护变量”。比如函数、方法或者属性。<br />这种保护不是强制规定，而是一种程序员的约定，解释器不做访问控制。一般来讲这些属性都作为实现细节而不需要调用者关心，随时都可能改变，编程时虽然能访问，但是不建议访问。<br />这种属性，只有在导入时，才能发挥保护作用。而且必须是`from XXX import *`这种导入形式才能发挥保护作用。<br />使用`from XXX import *`是一种通配导入（wildcard import），这是Python社区不推荐的方式，因为根本搞不清到底导入了什么属性、方法，很可能搞乱命名空间。PEP8推荐的导入方式是`from XXX import aVar , b_func , c_func`这种形式。<br />比如在下例汽车库函数tools.py里定义的“保护属性”：发动机型号和轮胎型号，这属于实现细节，没必要暴露给用户。当使用`from tools import *` 语句调用时，其实际并没有导入所有`_`开头的属性，只导入了普通`drive`方法。
```python
_moto_type = 'L15b2'
_wheel_type = 'michelin'

def drive():
    _start_engine()
    _drive_wheel()

def _start_engine():
    print('start engine %s'%_moto_type)
    
def _drive_wheel():
    print('drive wheel %s'%_wheel_type)
```
查看命令空间`print(vars())`可见，只有`drive`函数被导入进来，其他下划线开头的“私有属性”都没有导入进来。
```python
{'__name__': '__main__', '__doc__': None, '__package__': None, '__loader__': <_frozen_importlib_external.SourceFileLoader object at 0x005CF868>, '__spec__': None, '__annotations__':{}, '__builtins__': <module 'builtins' (built-in)>, '__file__': '.\\xiahuaxian.py', '__cached__': None, 'walk': <function walk at 0x01DA8C40>, 'root': '.\\__pycache__', '_': [21536000, 16410.54], 'dirs': ['tools.cpython-38.pyc'], 'city': 'Beijing', 'code': '010', 'drive': <function drive at 0x01DBC4A8>}
```
<a name="r3Rvv"></a>
### 3.1 突破保护属性
之所以说是“保护”并不是“私有”，是因为Python没有提供解释器机制来控制访问权限。依然可以访问这些属性：
```python
import tools
tools._moto_type = 'EA211'
tools.drive()
```
以上代码，以越过“保护属性”。此外，还有两种方法能突破这个限制，一种是将“私有属性”添加到tool.py文件的`__all__`列表里，使`from tools import *`也导入这些本该隐藏的属性。
```python
__all__ = ['drive','_moto_type','_wheel_type']
```
另一种是导入时指定“受保护属性”名。
```python
from tools import drive,_start_engine
_start_engine()
```
甚至是，使用`import tools`也可以轻易突破保护限制。所以可见，“保护属性”是一种简单的隐藏机制，只有在`from tools import *`时，由解释器提供简单的保护，但是可以轻易突破。这种保护更多地依赖程序员的共识：不访问、修改“保护属性”。除此之外，有没有更安全的保护机制呢？有，就是下一部分讨论的私有变量。
<a name="ykceH"></a>
## 四、`__var`用于私有变量
私有属性解决的之前的保护属性保护力度不够的问题。变量前面加上两个下划线，类里面作为属性名和方法都可以。两个下划线属性由Python的改写机制来实现对这个属性的保护。<br />看下面汽车例子中，品牌为普通属性，发动机为“保护属性”，车轮品牌为“私有属性”。
```python
class Car:
    def __init__(self):
        self.brand = 'Honda'
        self._moto_type = 'L15B2'
        self.__wheel_type = 'michelin'

    def drive(self):
        print('Start the engine %s,drive the wheel %s,I get a running %s car'%
        (self._moto_type,
        self.__wheel_type,
        self.brand))
```
用`var(car1)`查看下具体属性值，
```python
['_Car__wheel_type', '__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_moto_type', 'brand', 'drive']
```
可见，实例化`car1`中，普通属性`self.brand`和保护属性`self._moto_type`都得以保存，两个下划线的私有属性`__wheel_type`没有了。取而代之的是`_Car_wheel_type`这个属性。这就是改写机制（Name mangling）。两个下划线的属性，被改写成带有类名前缀的变量，这样子类很难明明一个和如此复杂名字重名的属性。保证了属性不被重载，保证了其的私有性。
<a name="yCMuY"></a>
### 4.1 突破私有属性
这里“私有变量”的实现，是从解释器层面给与的改写，保护了私有变量。但是这个机制并非绝对安全，依然可以通过`obj._ClasssName__private`来访问`__private`私有属性。
```python
car1.brand = 'Toyota'
car1._moto_type = '6AR-FSE'
car1._Car__wheel_type = 'BRIDGESTONE'
car1.drive()
```
结果
```python
Start the engine 6AR-FSE,\
drive the wheel BRIDGESTONE,\
I get a running Toyota car
```
可见，对改写机制改写的私有变量，虽然保护性加强了，但依然可以访问并修改。只是这种修改，只是一种杂耍般的操作，并不可取。
<a name="xXRRZ"></a>
## 五、`__var__`用于魔术方法
变量前面两个下划线，后面两个下划线。这是Python当中的魔术方法，一般是给系统程序调用的。例如上例中的`__init__`就是类的初始化魔术方法，还有支持`len`函数的`__len__`方法，支持上下文管理器协议的`__enter__`和`__exit__`方法，支持迭代器协议的`__iter__`方法，支持格式化显示的`__repr__`和`__str__`方法等等。这里为上例的Car类添加魔术方法`__repr__`来支持格式化显示。
```python
def __repr__(self):
    return '***Car %s:with %s Engine,%sWheel***'%
	(self.brand,self._moto_type,self.__wheel_type)
```
未添加`__repr__`魔术方法之前，`print(car1)`结果为`<__main__.Car object at 0x0047F7F0>`，这个结果让人看的一头雾水，增加`repr`魔术方法之后，显示结果为`***Car Toyota:with 6AR-FSE Engine,BRIDGESTONE Wheel***`清晰明了，利于调试。这就是魔术方法的功效：支持系统调用，改进用户类表现，增加协议支持，使用户类表现得更像系统类。
<a name="F1BrU"></a>
### 5.1 Python魔术方法分类
以下所有魔术方法均需要在前后加上`__`，这里省略了这些双下划线。

- 一元运算符 neg pos abs invert
- 转换 complex int float round inex
- 算术运算 add sub mul truediv floordiv mod divmod pow lshift rshift and xor or

算术运算除and之外，前面再加上r，表示反运算。除dimod外，前面加上`i`，表示就地运算。

- 比较 lt le eq ne gt ge
- 类属性 getattr getattribute setattr delattr dir get set delete
- 格式化 bytes hash bool format
- 类相关 init del new
- 列表 getitem
- 迭代器 iter next
- 上下文管理器 enter exit
<a name="SuHem"></a>
## 六、总结
总之，下划线在Python当中应用还是很广泛的，甚至可以说Python对下划线有所偏爱。可以看到`_`常用于临时变量，在REPL，`for`循环，元组拆包和国际化中得到了广泛应用。`var_`用于解决命名冲突问题，使用时比较简单易懂的。`_var`对变量的保护，只是一种脆弱的保护，更多依靠程序员的约定。`__var`用于私有变量，借助改写机制支持，已经支持了私有变量，但是仍然存在漏洞。对`__var__`用于魔术方法，进行了一个简单的介绍，魔术方法较多，但是理解并不复杂。希望以后可以进一步介绍这些魔术方法。
