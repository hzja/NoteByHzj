Python
<a name="uo2QY"></a>
## 1、`round()` 函数
以下代码的结果是什么：
```python
print(round(9/2))
print(round(7/2))
print(round(3/2))
```
答案：4、4、2<br />为什么 `print(round(7/2))` 输出4 而不是 3，或者为什么 `print(round(9/2))` 输出 4 而不是 5？<br />这是因为，在 python 中，round 函数实现了银行式的四舍五入，其中所有的半值都被四舍五入到最接近的偶数。
<a name="B69i9"></a>
## 2、实例
猜猜输出？
```python
class A:
    ans = 9   
    def __init__(self):
        self.answer = 10
        self.__add__ = lambda x, y: x.answer + y    

    def __add__(self, y):
        return self.answer - y
print(A() + 4)
```
答案：6<br />现在，你一定想知道我们是如何得到 6 的！这是因为，为了解析属性名称，Python 首先在实例级别搜索它，然后，在类级别搜索，然后在父类中搜索。这适用于除 **dunder** 方法之外的所有内容。在搜索它们时，Python 会跳过实例检查并直接在类中搜索。<br />因此 (10–4) = 6 就是答案。
<a name="ouJ0Y"></a>
## 3、数学不起作用的地方
猜猜这种情况下的输出？
```python
print(max(-0.0, 0.0))
```
答案：-0.0<br />你一定已经想到了 0.0 作为答案。但答案是-0.0。<br />为什么会这样？出现这种情况有两个原因。<br />负零和零在 Python 中被视为相等。<br />而在 python 的 max 函数中，如果多个项目是最大的，则该函数返回遇到的第一个。<br />因此 max 函数返回第一次出现的零，即 -0.0
<a name="s5aSS"></a>
## 4、Lazy Operators
猜猜这种情况下的输出？
```python
print(all([]))
print(any([]))
```
答案：true，false<br />函数 `all()` 有点复杂，因为它代表了空洞真理的概念。与链式惰性逻辑运算符一样，该算法是查找第一个 false 元素，如果没有找到则返回 True。由于空序列中没有错误元素，因此 `print(all([]))` 打印 True。<br />在函数 `any()` 中，如果可迭代的任何元素为真，它将返回 True。Python 中的逻辑运算符是懒惰的！该算法查找第一次出现的真元素，如果没有找到，则返回 False。由于序列为空，因此没有元素可以为真，因此 `print(any([]))` 打印 False。
<a name="UPZ45"></a>
## 5、我想要-n倍更多
猜猜输出？
```python
print("Can you give 50 claps to this story?" * (-1))
```
答案：它输出一个空字符串<br />这是因为 n 小于 0 的值被视为 0（这给出了与 s 相同类型的空序列）<br />如果 n(数字) 大于 0 而不是 -1，则字符串被打印 n 次。
<a name="yt044"></a>
## 6、Python 中的一切都是对象！
猜猜输出？
```python
print(isinstance(object, type))
print(isinstance(type, object))
print(isinstance(type, type))
print(isinstance(object, object))
```
答案：True, True, True, True<br />这是因为一切都是python中的对象。所有类型，如 int、str 和 object 都是类型类的实例，它是一个对象，因为在 python 中一切都是对象。
<a name="XS6yQ"></a>
## 7、`sum()` 函数
猜猜这种情况下的输出？
```python
print(sum(""))
print(sum("", []))
print(sum("", {}))
```
答案：0、[]、{}<br />为了理解这一点，来看看 `sum()` 是如何工作的——总和（可迭代，/，开始=0）<br />总和从左到右开始和可迭代的项目并返回总数。iterable 的项一般是数字，起始值不允许是字符串。因此，在上述所有情况下，""都被视为空序列，因此 sum 将简单地将起始参数作为总结果返回。
<a name="XGKKN"></a>
## 8、 Python 很懒！
猜猜这种情况下的输出？
```python
class follow:
   def func(self):
      return follow()
a = follow()
follow = int
print(a.func())
```
答案：0<br />发生这种情况是因为 python 函数中的代码仅在调用时执行。这意味着只有在实际调用该方法时才会引发所有 NameErrors 并且变量将被绑定。<br />因此，在例子中，在方法定义期间，Python 允许引用尚未定义的类。但是，在执行过程中，Python 会从外部范围绑定名称 follow，这意味着函数方法将返回一个新创建的 int 实例。<br />有点混乱吧？
<a name="l7pED"></a>
## 9、属性错误？
猜猜这种情况下的输出？
```python
print(sum([a.imag
   for a in [
      0, 5, 10e9, float('inf'), float('nan')
   ]
]))
```
答案：0.0<br />你一定认为这会出错，对吧？它不会。这是因为 Python 中的所有数值类型，如 int、float 等；从基础对象类继承，它们都返回包括 Infinity 和 NaN 在内的实部和虚部。
<a name="f8wJW"></a>
## 10、数学不再适用！
猜猜这种情况下的输出？
```python
a=(1 << 53)+1
print(a+1.0 > a)
```
答案：错误<br />现在这个解释会很长，所以请耐心等待:)<br />首先，由于任意精度算术（长算术）的反直觉行为。Python 支持 long 类型的非常大的整数，但是 Python 中浮点精度的限制是有限的。<br />数字是 2⁵³+ 1 = 9007199254740993<br />其次，是浮点精度限制，这意味着它不能完全表示为 Python 浮点数，因此，为了执行 x + 1.0，python 将 a 强制转换为浮点数，将其四舍五入为 Python 可以轻松表示的 9007199254740992.0，然后向其添加 1.0。但由于相同的表示限制，它将其设置回 9007199254740992.0。<br />第三是由于比较规则。与其他语言不同，Python 不会在 float 与 int 比较时抛出错误，也不会尝试将两个操作数转换为相同的类型。相反，他们比较实际的数值。并且因为 9007199254740992.0 低于 9007199254740993，所以它打印 False。
