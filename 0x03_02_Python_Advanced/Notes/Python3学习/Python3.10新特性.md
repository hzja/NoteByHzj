Python 新特性
<a name="VsbxD"></a>
### 新的 Union Type表达
新版本简化了 Union Type 的使用 ，改为更为简洁的`|`<br />旧版：
```python
from typing import Union
a: Union[int, str] = 1
```
新的版本：
```python
a: str | int = 1
```
二者完全等价：
```python
Union[int, str] == int | str # True
```
这类变化在其他地方也相似：
```python
# 旧版:
# def f(list: List[Union[int, str]], param: Optional[int]) -> Union[float, str]
def f(list: List[int | str], param: int | None) -> float | str:
    pass

f([1, "abc"], None)

# 旧版： 
# typing.List[typing.Union[str, int]]
typing.List[str | int]
list[str | int]

# 旧版： 
# typing.Dict[str, typing.Union[int, float]]
typing.Dict[str, int | float]
dict[str, int | float]
```
该特性也可用于 `isinstance`和`issubclass`
```python
# True
isinstance("FunnySaltyFish", int|str)

# True 
issubclass(str, str|int)
```
<a name="AN9pd"></a>
### `zip` 可选严格模式
zip新增可选参数`strict`， 当该选项为True时，传入`zip`的两个可迭代项长度必须相等，否则将抛出 `ValueError`<br />旧版（及不加此参数），当二者长度不等时，以长度较小的为准
```python
names = ["a","b","c","d"]
numbers = [1,2,3]
z = zip(names,numbers)
for each in z:
    print(each)
    
# ('a', 1)
# ('b', 2)
# ('c', 3)
```
设置`strict`为`True`
```python
# ...
z = zip(names,numbers,strict=True)
# ...

d:\projects\python\learn\Py310.py in <module>
      3 numbers = [1,2,3]
      4 z = zip(names,numbers,strict=True)
----> 5 for each in z:
      6     print(each)

ValueError: zip() argument 2 is shorter than argument 1
```
<a name="SqC5r"></a>
### 带括号的上下文管理器
`with`可以加括号了
```python
with (CtxManager() as example):
    ...

with (
    CtxManager1(),
    CtxManager2()
):
    ...

with (CtxManager1() as example,
      CtxManager2()):
    ...

with (CtxManager1(),
      CtxManager2() as example):
    ...

with (
    CtxManager1() as example1,
    CtxManager2() as example2
):
    ...
```
如
```python
import pathlib
p = pathlib.Path()
p1 = p/"text1.txt" # 内容：文本1的内容
p2 = p/"text2.txt" # 内容：文本2的内容
with(
    p1.open(encoding="utf-8") as f1,
    p2.open(encoding="utf-8") as f2
):
    print(f1.read(), f2.read(), sep="\n") 
    
    # 文本1的内容
 # 文本2的内容
```
<a name="wIQJu"></a>
### 显式类型别名
使用 TypeAlias 显式标注类型别名，提高可读性<br />旧的方式：
```python
x = int
def plus_int(a:x,b:x) -> x:
    return a+b
```
可以看到，x很容易被搞混<br />新的方式：使用 `TypeAlias`表明这是个别名
```python
from typing import TypeAlias
x : TypeAlias = int
def plus_int(a:x,b:x) -> x:
    return a+b
```
<a name="jmdW3"></a>
### `match...case`语句
就是其他语言的switch-case，python终于提供了支持，还是加强版的<br />完整语法参见：[PEP 634 -- Structural Pattern Matching: Specification | Python.org]()<br />举几个例子：<br />基本的类型匹配：
```python
day = 6
match day:
    case 1:
        print("星期一")
    case 6 | 7:
        print("周末")
    case _ : 
        print("其他情况")
```
subject：这在处理命令行参数的时候特别有用
```python
"""
    @copyright : [FunnySaltyFish](https://funnysaltyfish.github.io)
    @date : 2021/10/05 21:08:42
"""
command = "save 1.txt"
# 试着把command改成 list / copy 1.txt 2.txt 看看效果
match command.split(" "):
    case ["list"]:
        print("列出文件~")
    case ["save", file_name]:
        print(f"保存文件到 {file_name}")
    case ["copy",source,target]:
        print(f"拷贝 {source} -> {target}")
```
也可以匹配对象：
```python
class Person():
    pass

class Student(Person):
    def __init__(self, id: int) -> None:
        self.id = id

class Teacher(Person):
    def __init__(self, name: str) -> None:
        self.name = name

a = Student(1)
# a = Student(2)
# a = Teacher("FunnySaltyFish")
match a:
    case Student(id = 2):
        print(f"这是位学生，且id正好是2")
    case Student():
        print(f"这是学生，id为{a.id}")
    case Teacher():
        print(f"这是老师, 姓名为{a.name}")
```
当然也可以匹配字典：
```python
d = {
    "name" : "李四", # 张三
    "age" : 18,
    "hobby" : "阅读"
} 
match d:
    case {"name":"张三", **args}:
        # **收集其他参数
        print("这是张三", args) # 这是张三 {'age': 18, 'hobby': '阅读'}
    case {"name" : name , "age" : age, "hobby": hobby}:
        print(f"我叫{name}, 今年{age}岁, 喜欢{hobby}") #我叫李四,今年18岁，喜欢阅读
```
更复杂的还有结合Guard、匹配捕获等使用，具体可以参见：[PEP 635 -- Structural Pattern Matching: Motivation and Rationale | Python.org](https://www.python.org/dev/peps/pep-0635/#id15: https://link.juejin.cn?target=https%3A%2F%2Fwww.python.org%2Fdev%2Fpeps%2Fpep-0635%2F%23id15) 和 [PEP 636 -- Structural Pattern Matching: Tutorial | Python.org](https://www.python.org/dev/peps/pep-0636/: https://link.juejin.cn?target=https%3A%2F%2Fwww.python.org%2Fdev%2Fpeps%2Fpep-0636%2F)
<a name="InvRS"></a>
### 更友好的报错提示
现在，当括号、引号未闭合时，Python会抛出更加清晰明了的错误
```python
str = "未闭合的str

File "d:\projects\python\learn\Py310.py", line 90
    str = "未闭合的str
          ^
SyntaxError: unterminated string literal (detected at line 90)


 arr = [1, 2, 2, 3
    
  File "d:\projects\python\learn\Py310探索.py", line 91
    arr = [1, 2, 2, 3
          ^
SyntaxError: '[' was never closed
```
<a name="J4Cnh"></a>
### 其他一些更新
<a name="nqz9k"></a>
#### distutils 被弃用
推荐使用 `setuptools`
<a name="niuJD"></a>
#### 需要 OpenSSL 1.1.1 及以上版本
<a name="km1dk"></a>
#### 移除 `Py_UNICODE`编码API
<a name="C3GYF"></a>
#### `PyUnicodeObject`的`wstr`被弃用，并将在之后移除
