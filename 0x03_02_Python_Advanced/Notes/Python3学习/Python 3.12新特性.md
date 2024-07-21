Python
<a name="JgGw2"></a>
## 改善报错信息
来自官方标准库的模块现在可以在报NameError时提示问题原因，比如
```python
>>> sys.version_info
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'sys' is not defined. Did you forget to import 'sys'?
```
当实例内报NameError时也会提示问题原因，比如成员方法引用了未定义的变量，而这个变量名又和成员变量名相同时会提示用户是否忘记加self，比如
```python
>>> class A:
...    def __init__(self):
...        self.blech = 1
...
...    def foo(self):
...        somethin = blech

>>> A().foo()
Traceback (most recent call last):
  File "<stdin>", line 1
    somethin = blech
               ^^^^^
NameError: name 'blech' is not defined. Did you mean: 'self.blech'?
```
当导入模块时`import`和`from`写反报SyntaxError时也会提示原因，比如
```python
>>> import a.y.z from b.y.z
Traceback (most recent call last):
  File "<stdin>", line 1
    import a.y.z from b.y.z
    ^^^^^^^^^^^^^^^^^^^^^^^
SyntaxError: Did you mean to use 'from ... import ...' instead?
```
在导入模块错误报ImportError时也会提示应该导入哪个对象，比如
```python
>>> from collections import chainmap
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: cannot import name 'chainmap' from 'collections'. Did you mean: 'ChainMap'?
```
<a name="Th9DR"></a>
## 新功能
<a name="rhxuZ"></a>
### PEP701 f-string的语义格式化
现在f-string内的表达式可以是任何合法的Python表达式，包括反斜杠、Unicode转义、多行表达式、注释和重复使用的引号种类。

- 重复使用的引号种类：在Python 3.12版本中，用户可以在表达式里重复使用f-string使用过的引号种类，比如这里重复使用了双引号
```python
>>> songs = ['Take me back to Eden', 'Alkaline', 'Ascensionism']
>>> f"This is the playlist: {", ".join(songs)}"
'This is the playlist: Take me back to Eden, Alkaline, Ascensionism'
```
因为这个改动，现在Python可以内嵌多个f-string了，比如以前只能内嵌4层
```
>>> f"""{f'''{f'{f"{1+1}"}'}'''}"""
'2'
```
现在则没有这种限制
```
>>> f"{f"{f"{f"{f"{f"{1+1}"}"}"}"}"}"
'2'
```

- 多行表达式和注释：以前在f-string中必须把表达式写在一行内，可读性不高。现在没有这种限制，并且可以包含注释。
```python
>>> f"This is the playlist: {", ".join([
... Take me back to Eden',  # My, my, those eyes like fire
... 'Alkaline',              # Not acid nor alkaline
... 'Ascensionism'           # Take to the broken skies at last
...])}"
'This is the playlist: Take me back to Eden, Alkaline, Ascensionism'
```

- 反斜杠和Unicode字符：以前在f-string中不能使用反斜杠和Unicode转义。这会使得有的Unicode字符无法在f-string中使用，现在没有这种限制了。
```python
>>> print(f"This is the playlist: {"\n".join(songs)}")
This is the playlist: Take me back to Eden
Alkaline
Ascensionism
>>> print(f"This is the playlist: {"\N{BLACK HEART SUIT}".join(songs)}")
This is the playlist: Take me back to Eden♥Alkaline♥Ascensionism
```
一个有用的副作用就是现在报错的位置更精准了。比如在3.11中解释器不能告诉用户SyntaxError错误产生的位置。
```python
>>> my_string = f"{x z y}" + f"{1 + 1}"
  File "<stdin>", line 1
    (x z y)
     ^^^
SyntaxError: f-string: invalid syntax. Perhaps you forgot a comma?
```
现在可以获取到报错位置了。
```python
>>> my_string = f"{x z y}" + f"{1 + 1}"
  File "<stdin>", line 1
    my_string = f"{x z y}" + f"{1 + 1}"
                   ^^^
SyntaxError: invalid syntax. Perhaps you forgot a comma?
```
<a name="tXGLK"></a>
### PEP709 内联行为
现在解释器会把字典、列表和集合理解成内联行为，而不是只用一次的数据结构，这样做会让程序运行速度提升一倍。但是，迭代器并不会理解成内联行为。<br />译者：这部分关系到虚拟机帧栈的优化，其API对普通用户并无感知。
<a name="lIAsp"></a>
### PEP 688 Buffer协议可以暴露给用户使用
这个提案介绍了如何让Python用户使用到buffer协议，只需要让类实现`__buffer__()`方法就可以把它当作Buffer类使用。<br />与此同时，新的`collections.abc.Buffer`抽象基类提供了标准的暴露buffer的方式，比如在类型注解里。在新的`inspect.BufferFlags`枚举可以表示自定义的buffer资源。
<a name="GzxCc"></a>
## 与类型注解相关的新功能
<a name="hY57C"></a>
### PEP 692 用`TypedDict`注解**kwargs类型
PEP 484 介绍了如何注解函数签名中`**kwargs`的类型，但是所有的`**kwargs`类型都一样。这份提案提供了一种更精确的类型注解方案，比如
```python
from typing import TypedDict, Unpack

class Movie(TypedDict):
    name: str
    year: int

def foo(**kwargs: Unpack[Movie]): ...
```
<a name="jGbqI"></a>
### PEP 698 静态类型注解的override装饰器
`typing`模块中加入了一个新的装饰器`typing.override()`，它表明被它修饰的方法需要复写其父类的同名方法（译者注：类似于Java的@Overide）。它可以让类型检查该方法是否正确复写了父类的方法。
```python
from typing import override

class Base:
    def get_color(self) -> str:
        return "blue"

class GoodChild(Base):
    @override  # ok: overrides Base.get_color
    def get_color(self) -> str:
        return "yellow"

class BadChild(Base):
    @override  # type checker error: does not override Base.get_color
    def get_colour(self) -> str:
        return "red"
```
<a name="d2Idt"></a>
### PEP 695 参数类型语法
在PEP 484 中，Python对泛型类和方法类型注解的支持有点啰嗦且不够精确，并需要一套更直白的类型声明方案。本提案引入了一种新的、简洁的、直白的类型注解方案。
```python
def max[T](args: Iterable[T]) -> T:
    ...

class list[T]:
    def __getitem__(self, index: int, /) -> T:
        ...

    def append(self, element: T) -> None:
        ...
```
此外，本方案还引入了一种新type别名声明方案，并可以通过`type`创建一个TypeAliasType实例。
```python
type Point = tuple[float, float]
```
类型别名同样可以加入泛型。
```python
type Point[T] = tuple[T, T]
```
新的语法规则允许声明TypeVarTuple和ParamSpec，就像声明TypeVar一样。
```python
type IntFunc[**P] = Callable[P, int]  # ParamSpec
type LabeledTuple[*Ts] = tuple[str, *Ts]  # TypeVarTuple
type HashableSequence[T: Hashable] = Sequence[T]  # TypeVar with bound
type IntOrStrSequence[T: (int, str)] = Sequence[T]  # TypeVar with constraints
```
类型别名、范围以及限制类型只有在解释器需要的时候创建，也就是说别名可以在代码其他地方被重写。<br />参数类型的声明作用于声明的范围，对其外部是不生效的。举个例子，函数参数的类型注解可以作用于其派生类的方法或该类的其他地方。然而，它不能作用于模块范围内的其他地方，即使这个地方位于该类的定义的后面。具体使用方法可以参考Type parameter lists章节。<br />为了支持这种范围的类型注解，现在虚拟机引入了一种新的范围——注解范围（annotation scope）。在大多数情况下，这个范围等同于函数的范围，但是它会和不同的类的范围发生关联。在Python 3.13中，所有的类型注解都会在这个范围内。
<a name="kfrar"></a>
## 其他语言改动（部分）

- 增加了环境变量PYTHONPERFSUPPORT、命令行参数-X perf以及API `sys.activate_stack_trampoline()`、`sys.deactivate_stack_trampoline()`和`sys.is_stack_trampoline_active()`以支持Linux优化（Python support for the Linux perf profiler）
- 如果底层字典结构是可哈希的，那么`types.MappingProxyType`实例现在也是可哈希的
- 语法分析器现在可以分析空字节
- 现在GC只会在字节码之间的暂停点运行，而不是分配内存的时候运行。另外，GC还会在调用`PyErr_CheckSignals()`时运行。这样，在Python的C扩展中解释器可以执行大量的C语言代码而不去执行Python代码，以便减少GC运行的可能性。
- 可以在生成的数据中使用海象运算符（:=）来赋值，比如`[(b := 1) for a, b.prop in some_iter]`
- slice对象现在是可哈希的，所以可以用作字典的键。
- `sum()`方法现在用了新的求和算法，所以现在更精确了。
<a name="lEGRG"></a>
## 小结
以上是Python 3.12语法层面的改动。可以看出，现在Python委员会的发力点一个是类型注解，另一个是GC的使用效率。他们这么做也很好理解，这两个问题一直是为人诟病的症结。弱类型语言使得Python成为不了大型项目的开发语言，而效率低下的GC也是阻碍Python往前一步的绊脚石。
