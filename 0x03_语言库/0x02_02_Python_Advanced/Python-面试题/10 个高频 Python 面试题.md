<a name="RA0Sf"></a>
## 一、Python中的整数缓存机制
面试官：<br />请解释在 Python shell 解释器上执行的代码的以下结果：
```python
>>> a=256
>>> b=256
>>> a is b
True
>>> x=257
>>> y=257
>>> x is y
False
```
回答：<br />这是因为 Python 中的整数缓存机制。为了节省时间和内存成本，Python 总是预先加载 [-5, 256] 范围内的所有小整数。<br />因此，[-5, 256] 中的所有整数都已经保存在内存中。当声明此范围内的新整数变量时，Python 只是将缓存的整数引用到它，不会创建任何新对象。<br />因此，对结果的解释是：<br />当变量 a 和 b 分配给 256 时，它们被引用到存储 256 的相同内存位置。他们指向同一个对象。<br />当变量 a 和 b 被分配给 257 时，它们是不同内存位置的两个不同对象，因为 257 不在小整数缓存范围内。<br />由于 is 运算符要比较两个变量的内存位置，`a is b` 应该输出 True ，`x is y` 应该输出 False。
<a name="U2ZNI"></a>
## 二、如何避免嵌套循环
面试官：<br />下面的程序使用了嵌套循环，你将如何优化它？
```python
list_a = [1, 2020, 70]
list_b = [2, 4, 7, 2000]
list_c = [3, 70, 7]

for a in list_a:
    for b in list_b:
        for c in list_c:
            if a + b + c == 2077:
                print(a, b, c)
# 70 2000 7
```
回答：<br />可以使用 itertools 中的 product 函数对其进行优化:
```python
from itertools import product

list_a = [1, 2020, 70]
list_b = [2, 4, 7, 2000]
list_c = [3, 70, 7]

for a, b, c in product(list_a, list_b, list_c):
    if a + b + c == 2077:
        print(a, b, c)
# 70 2000 7
```
因为 product 函数生成输入迭代的笛卡尔积。它可以帮助我们在很多场景中避免嵌套循环。
<a name="o26e7"></a>
## 三、 类方法和静态方法
面试官：<br />在 Python 类中，类方法和静态方法有什么区别？<br />回答：<br />类方法是 Python 类中的第一个参数是类本身的方法。用 cls 这个参数来表示。<br />类方法不仅可以由实例调用，也可以由类直接调用。<br />静态方法是 Python 类中没有类或实例参数的方法。<br />因为静态方法不包含有关特定类或实例的参数。可以将其定义为类外的独立函数，并将其用作类外的其他普通函数。
<a name="baE7y"></a>
## 四、eval 函数的使用
面试官：<br />能否用一行 Python 代码中实现一个函数，它将接收两个数字 a 和 b 一个字符串 op。op 代表算术运算符，例如 “+”、“-”、“*”和“/” 。函数需要返回 op 的计算结果 a op b。<br />回答：
```python
def cal(a, b, op): return eval(f'{a} {op} {b}')
```
面试官：<br />好！那你能谈谈 eval 函数的缺点，以及为什么它不适合在生产中使用吗？<br />回答：<br />在生产环境中，应该仔细检查用户输入以避免意外问题。eval 函数会立即执行输入，这可能造成远程代码执行，这是非常危险的，强烈建议不要在生产环境使用 eval，即使你做了很多安全检查。
<a name="EDkf5"></a>
## 五、抽象类
面试官：<br />在面向对象编程中，有一个概念叫做抽象类。Python 也支持抽象类吗？<br />回答：<br />支持。Python 标准库有个模块 abc，它为抽象类提供功能。<br />通过继承类 abc.ABC，可以将类定义为抽象类，借助于装饰器 abc.abstractmethod，可以将方法定义为抽象方法。<br />例如：
```python
from abc import ABC, abstractmethod

class Animal(ABC):
    @abstractmethod
    def move(self):
        pass
```
<a name="hlkdy"></a>
## 六、函数传参的机制
面试官：<br />Python 是按值还是按引用传参？<br />回答：<br />既不是按值传递，也不是按引用传递，Python 自有一套机制，都知道 Python 的世界里，万物皆对象，从这个方面讲，传递给函数的，都是对象的地址，这有点像引用的概念，但是 Python 的对象分为可变对象和不可变对象，不可变对象就是不可修改的对象，例如：数字、字符串、元组，在不可变对象上的自增操作会新创建一个对象。<br />如果从其他编程语言过来学 Python 的，可以简单的这么理解：对于不可变对象，是按值传递，函数内部不会修改不可变对象，对于可变对象，是按引用（地址）传递，函数内部的修改会影响到参数本身。
<a name="nHm7C"></a>
## 七、Python 中的垃圾回收
面试官：<br />Python 如何收集垃圾（无用的对象）？<br />回答：<br />Python 使用一种称为引用计数的方法来决定何时需要在内存中收集对象。<br />简单来说，Python 会计算每个对象的引用计数，当一个对象没有引用时，会自动收集。
<a name="G8sKw"></a>
## 八、在 Python 函数中接收无限数量的参数
面试官：<br />请解释函数参数中星号的以下用法：
```python
def func(*args, **kwargs):
    pass
```
回答：<br />按照惯例，如果无法清楚地确定其参数的数量，会像示例一样定义一个 Python 函数。<br />以单个星号为前缀的参数 args 表示可以将任意数量的位置参数保存到元组中，args 就是这个元组的名称。<br />以两个星号为前缀的参数 kwargs 表示可以将任意数量的位置参数保存到字典中，kwargs 就是这个字典的名称。
<a name="Tb6xy"></a>
## 九、 Lambda 函数
面试官：<br />Python 中的 lambda 函数是什么？你能否提供一个利用 lambda 函数强大功能的示例？<br />回答：<br />lambda 函数，或称为匿名函数，是一个没有函数名的简单 Python 函数。<br />编写 lambda 函数的模板是：
```
lambda 参数：表达式
```
使用它的一个很好的场景是 sort 函数，比如：
```python
leaders = ["Warren Buffett", "Yang Zhou", "Tim Cook", "Elon Musk"]
print(leaders)
# ['Warren Buffett', 'Yang Zhou', 'Tim Cook', 'Elon Musk']
leaders.sort(key=lambda x: len(x))
print(leaders)
# ['Tim Cook', 'Yang Zhou', 'Elon Musk', 'Warren Buffett']
```
<a name="uctf6"></a>
## 十、Python 中的一些推导式
面试官：<br />Python中的推导式是什么？<br />回答：<br />推导式技巧是 Python 中的语法糖。它可以帮助轻松生成特定的数据结构。Python 中有四种类型的推导式：

1. 列表推导式
2. 生成器推导式
3. 集合推导式
4. 字典推导式

例如，可以如下生成字典：
```python
Entrepreneurs = ["Yang", "Mark", "steve", "jack", "tom"]
D1 = {id: name for id, name in enumerate(Entrepreneurs) if name[0].isupper()}
print(D1)
# {0: 'Yang', 1: 'Mark'}
```
