Python<br />Python的 `eval()` 允许从基于字符串或基于编译代码的输入中计算任意Python表达式。当从字符串或编译后的代码对象的任何输入中动态计算Python表达式时，此函数非常方便。<br />从如下两个方面展开学习。

- **Python的 **`**eval()**`** 如何工作**
- **如何使用 **`**eval()**`** 来动态地计算任意基于字符串或基于编译代码的输入**

**如何使用 Python 的 **`**eval()**`** 来编码一个交互式地计算数学表达式的应用程序**。通过这个例子，将把所学到的关于 `eval()` 的一切应用于一个实际问题。
<a name="Mx1CY"></a>
## Python 的 `eval()`
可以使用内置的 Python [**eval()**](https://docs.python.org/3/library/functions.html#eval) 从基于字符串或基于编译代码的输入中动态地计算表达式。如果向 `eval()` 传递一个字符串，那么该函数会解析它，将其编译为[**字节码**](https://docs.python.org/3/glossary.html#term-bytecode)，并将其作为一个 Python 表达式进行计算。但是如果用一个编译过的代码对象调用 `eval()`，那么该函数只执行计算步骤，如果用相同的输入多次调用 `eval()`，这就非常方便了。<br />Python的 `eval()` 的定义如下。
```python
eval(expression[, globals[, locals]])
```
该函数需要一个第一个参数，称为`expression`，它包含了需要计算的表达式。`eval()`还需要两个可选参数。

1. `**globals**`
2. `**locals**`

在接下来的内容中，将学习这些参数是什么，以及 `eval()` 如何使用它们来即时计算Python 表达式。<br />**注意：** 也可以使用 [**exec()**](https://docs.python.org/3/library/functions.html#exec) 来动态地执行 Python 代码。`eval()` 和 `exec()` 的主要区别是，`eval()` 只能执行或计算表达式，而 `exec()` 可以执行任何一段 Python代码。
<a name="gpXW8"></a>
### **第一个参数：**`**expression**`
`**eval()**` 的第一个参数称为 `expression`，它是一个必需的参数，用于保存函数的 **基于字符串** 或 **基于编译码的** 输入。当调用 `eval()` 时，`expression` 的内容被作为 Python 表达式进行计算。下面是使用基于字符串的输入的例子。
```python
>>> eval("2 ** 8")
256
>>> eval("1024 + 1024")
2048
>>> eval("sum([8, 16, 32])")
56
>>> x = 100
>>> eval("x * 2")
200
```
当用一个字符串作为参数调用 `eval()` 时，该函数返回对输入字符串进行计算的结果。默认情况下，`eval()`可以访问全局变量名，如上例中的x。<br />为了计算一个基于字符串的表达式，Python 的 `eval()` 运行以下步骤。

1. **解析表达式**
2. **将其编译为字节码**
3. **将其作为一个Python表达式进行计算**
4. **返回计算的结果**

`eval()`的第一个参数 expression 强调了该函数只作用于表达式，并非[**复合语句**](https://docs.python.org/3/reference/compound_stmts.html)。Python 文档对 **expression** 的定义如下。
<a name="mNYwv"></a>
#### `expression`
一段可以被计算为某种值的语法。换句话说，表达式是表达式元素的累积，如字面意义、名称、属性访问、运算符或函数调用，它们都返回一个值。与许多其他语言相比，并非所有的语言结构都是表达式。也有一些语句不能作为表达式使用，如 `while`。此外赋值也是语句，不是表达式。([**来源**](https://docs.python.org/3/glossary.html#term-expression))<br />另一方面，Python **statement** 有如下定义。
<a name="EJDwB"></a>
#### `statement`
`statement`是一个套件（一个代码 "块"）的一部分。`statement`要么是一个表达式，要么是带有关键字的几个结构体之一，如 if、while或for。([**来源**](https://docs.python.org/3/glossary.html#term-statement))<br />如果向`eval()`传递一个复合语句，那么会得到一个 SyntaxError。下面的例子是用`eval()`来执行一个if语句。
```python
>>> x = 100
>>> eval("if x: print(x)")
  File "<string>", line 1
    if x: print(x)
    ^
SyntaxError: invalid syntax
```
上面报错是因为 `eval()` 只接受表达式。任何其它语句，如 `if`、`for`、`while`、`import`、`def` 或 `class`，都会引发错误。<br />**注意：**` for` 循环是一个复合语句，但是 `for` 关键字也可以用在推导式中，此时它被认为是表达式。可以使用`eval()` 来计算推导式，即使它们使用了 `for` 关键字。<br />`eval()`也不允许进行赋值操作。
```python
>>> eval("pi = 3.1416")
  File "<string>", line 1
    pi = 3.1416
       ^
SyntaxError: invalid syntax
```
如果将一个赋值操作作为参数传递给 `eval()` ，那么会得到一个 SyntaxError。赋值操作是语句，而不是表达式，语句不允许与 `eval()` 一起使用。<br />当解析器不理解输入的表达式时，也会得到一个 SyntaxError。在下面的例子中计算一个违反 Python 语法的表达式。
```python
>>> # Incomplete expression
>>> eval("5 + 7 *")
  File "<string>", line 1
    5 + 7 *
          ^
SyntaxError: unexpected EOF while parsing
```
所以，不能把一个违反 Python 语法的表达式传给 `eval()` 。在上面的例子中，尝试计算一个不完整的表达式 ("5 + 7 *") 时抛出一个 SyntaxError，因为分析器不理解表达式的语法。<br />也可以把已编译的代码对象传递给 `eval()` 。因此可以使用函数 [**compile()**](https://docs.python.org/3/library/functions.html#compile)，一个内置函数，可以将输入的字符串编译成[**代码对象**](https://docs.python.org/3/library/stdtypes.html#code-objects) 或 [**AST 对象**](https://docs.python.org/3/library/ast.html#ast.AST)，这样就可以用 `eval()` 来计算它。<br />如何使用`compile()`的细节超出了本文的范围，但这里可以快速了解一下它的前三个必要参数。

1. **source 保存要编译的源代码。这个参数可以接受普通字符串、**[**字节字符串**](https://docs.python.org/3/library/stdtypes.html#bytes-objects)**和AST对象。**
2. **filename 给出读取代码的文件。如果要使用一个基于字符串的输入，那么这个参数的值应该是"<string>"。**
3. **mode 指定了想得到哪种编译后的代码。如果想用**`**eval()**`**来处理编译后的代码，那么这个参数应该被设置为 "eval"。**

可以使用 `compile()` 向`eval()`提供代码对象，而不是普通的字符串。
```python
>>> # 算术运算
>>> code = compile("5 + 4", "<string>", "eval")
>>> eval(code)
9
>>> code = compile("(5 + 7) * 2", "<string>", "eval")
>>> eval(code)
24
>>> import math
>>> # 一个球体的体积
>>> code = compile("4 / 3 * math.pi * math.pow(25, 3)", "<string>", "eval")
>>> eval(code)
65449.84694978735
```
如果使用 `compile()` 来编译要传递给`eval()`的表达式，那么`eval()`会经过以下步骤。

1. **计算编译后的代码**
2. **返回计算的结果**

如果使用基于编译码的输入调用 `eval()` ，那么该函数会执行计算步骤并立即返回结果。当需要多次计算同一个表达式时，这可能很方便。在这种情况下，最好预先编译表达式，并在随后调用 `eval()` 时重复使用产生的字节码。<br />如果事先编译了输入表达式，那么连续调用`eval()`将运行得更快，因为不会重复**解析**和**编译**的步骤。如果正在计算复杂的表达式，不需要的重复会导致高的CPU时间和过度的内存消耗。
<a name="DdBuB"></a>
### **第二个参数：**`**globals**`
`eval()` 的第二个参数 `**globals**`，可选的，字典类型，为 `eval()` 提供一个全局命名空间。通过 `globals` 告诉 `eval()` 在计算表达式时要使用哪些全局变量名。<br />全局变量名是所有那些在当前全局范围或命名空间中可用的变量名。可以从代码的任何地方访问它们。<br />在字典中传递给 globals 的所有名字在执行时都可以提供给 `eval()` 。请看下面的例子，它展示了如何使用一个自定义的字典来为 `eval()` 提供一个全局命名空间。
```python
>>> x = 100  # 一个全局变量
>>> eval("x + 100", {"x": x})
200
>>> y = 200  # 另一个全局变量
>>> eval("x + y", {"x": x})
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "<string>", line 1, in <module>
NameError: name 'y' is not defined
```
如果为 `eval()` 的 globals 参数提供一个自定义字典，那么 `eval()` 将只接受这些名字作为 `globals`。在这个自定义字典之外定义的任何全局变量名都不能从 `eval()` 内部访问。这就是为什么试图在上述代码中访问 y 时，Python 会引发一个 NameError。传递给 globals 的字典不包括 y。<br />可以通过在字典中列出名字来插入 globals，然后这些名字在求值过程中就会出现。例如，如果在 globals 中插入了 y，那么在上面的例子中对 "x + y" 的求值将如期进行。
```python
>>> eval("x + y", {"x": x, "y": y})
300
```
因为把 y 添加到了自定义 globals 字典中，所以成功计算 "x + y" 的值，得到的预期返回值 300。<br />也可以提供不存在于当前全局范围的变量名。此时需要为每个名字提供一个具体的值。`eval()`在运行时将把这些变量名解释为全局变量名。
```python
>>> eval("x + y + z", {"x": x, "y": y, "z": 300})
600
>>> z
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'z' is not defined
```
尽管z没有在当前的全局范围内定义，但是这个变量在全局中的值是300，此时`eval()`可以访问z，就像它是一个全局变量一样。<br />globals 背后的机制是相当灵活的，可以向 globals 传递任何可见的变量（全局、局部、或者非局部）。还可以传递自定义的键值对，比如上面例子中的 "z": 300，那么`eval()` 将把它们全部作为全局变量处理。<br />关于 globals 中的注意事项，如果提供给它的自定义字典不包含键值 "__builtins__"，那么在表达式被解析之前，对内置字典的引用将自动插入 "__builtins__" 下面。这可以确保 `eval()` 在计算表达式时可以完全访问所有的 Python 内置变量名。<br />下面的例子表明，即使给 globals 提供了一个空的字典，对 `eval()` 的调用仍然可以访问 Python 的内置变量名。
```python
>>> eval("sum([2, 2, 2])", {})
6
>>> eval("min([1, 2, 3])", {})
1
>>> eval("pow(10, 2)", {})
100
```
在上面的代码中，向 globals 提供了一个空的字典 (`{}`)。由于这个字典不包含一个叫做 "__builtins__" 的键，Python 会自动插入一个指向 builtins 中名字的引用。这样，`eval()` 在解析表达式时就可以完全访问所有 Python 的内置名字。<br />如果调用 `eval()` 而没有将自定义字典传递给 globals ，那么参数将默认为在调用 `eval()`的环境中 `globals()` 返回的字典:
```python
>>> x = 100  #  一个全局变量
>>> y = 200  # 另一个全局变量
>>> eval("x + y")  # 访问两个全局变量
300
```
当调用 `eval()` 而不提供 globals 参数时，该函数使用 `globals()` 返回的字典作为其全局命名空间来计算表达式。所以，在上面的例子中，可以自由地访问 x 和 y，因为它们是包含在当前全局范围内的全局变量。
<a name="TQpSQ"></a>
### **第三个参数：**`**locals**`
Python 的 `eval()` 第三个参数 `**locals**` ，可选参数，字典类型。此时这个字典包含了 `eval()` 在计算表达式时作为局部变量名使用的变量。<br />局部变量名是那些在一个给定的函数内定义的名称（变量、函数、类等等）。局部名称只在封闭的函数内可见。在编写函数时定义这些变量名。<br />因为 `eval()` 已经写好了，所以不能在它的代码或局部范围内添加局部变量名。然而可以向 `locals` 传递一个字典，`eval()`会把这些名字当作本地名字。
```python
>>> eval("x + 100", {}, {"x": 100})
200
>>> eval("x + y", {}, {"x": 100})
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "<string>", line 1, in <module>
NameError: name 'y' is not defined
```
第一个调用 `eval()` 的第二个字典保存了变量 x。这个变量被 `eval()` 解释为一个局部变量。换句话说，它被看作是在 `eval()` 中定义的一个变量。<br />可以在表达式中使用 x，并且 `eval()` 可以访问它。相反，如果使用y，那么会得到一个 NameError，因为y没有定义在 globals 命名空间或 locals 命名空间。<br />和 globals 一样，可以向 locals 传递任何可见的变量（全局、局部或非局部）。也可以传递自定义的键值对，比如 "x"。`eval()`将把它们全部作为局部变量处理。<br />注意，要给 locals 提供一个字典，首先需要给 globals 提供一个字典。不能在 `eval()` 中使用关键字参数。
```python
>>> eval("x + 100", locals={"x": 100})
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: eval() takes no keyword arguments
```
如果在调用 `eval()` 时使用关键字参数，那么抛出一个 TypeError。这是因为 `eval()` 不接受关键字参数，所以在提供 `locals` 字典之前，需要先提供一个 `globals` 字典。<br />如果没有给 `locals` 传递一个字典，那么它就默认为传递给 `globals` 的字典。这里有一个例子，给 `globals` 传递了一个空的字典，而 `locals` 没有传递任何值。
```python
>>> x = 100
>>> eval("x + 100", {})
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "<string>", line 1, in <module>
NameError: name 'x' is not defined
```
鉴于没有给 `locals` 提供一个自定义的字典，这个参数默认为传递给 globals 的字典。此时`eval()` 无法访问 x，因为 `globals` 持有一个空的字典。<br />`**globals**`** 和 **`**locals**`** 之间的主要实际区别是**，如果"__builtins__"键不存在，Python 会自动插入 `globals` 中。无论是否为 `globals` 提供了一个自定义的字典，这都会发生。此外，如果给 locals 提供了一个自定义的字典，那么在执行 `eval()` 的过程中，这个字典将保持不变。
<a name="C8BT2"></a>
## 用 `eval()` 计算表达式
可以使用Python的`eval()`来计算任何一种Python表达式，但不包括Python语句，如基于关键字的复合语句或赋值语句。<br />当需要动态地计算表达式，而使用其它 Python 技术或工具会大大增加开发时间和精力时，`eval()` 可以很方便。<br />在这一节中，将学习如何使用 Python 的 `eval()` 来计算布尔、数学和通用的 Python 表达式。
<a name="jW2bP"></a>
### **布尔表达式**
**布尔表达式** 是Python表达式，当解释器对其进行计算时返回一个真值(True 或者 False)。它们通常用在if语句中，以检查某些条件是否为真或假。由于布尔表达式不是复合语句，可以使用`eval()`来计算它们。
```python
>>> x = 100
>>> y = 100
>>> eval("x != y")
False
>>> eval("x < 200 and y > 100")
False
>>> eval("x is y")
True
>>> eval("x in {50, 100, 150, 200}")
True
```
可以用 `eval()` 来处理使用以下任何Python运算符的布尔表达式。

- **值比较运算符：< , > , <=, >=, ==, !=**
- **逻辑（布尔）运算符：and, or, not**
- **成员测试运算符：in, not in**
- **身份运算符：is, is not**

在所有情况下，该函数都会返回正在计算的表达式的真值。<br />思考，为什么应该使用`eval()`而不是直接使用布尔表达式呢？假设需要实现一个条件语句，想临时改变条件。
```python
>>> def func(a, b, condition):
...     if eval(condition):
...         return a + b
...     return a - b
...
>>> func(2, 4, "a > b")
-2
>>> func(2, 4, "a < b")
6
>>> func(2, 2, "a is b")
4
```
在`func()`中，使用`eval()`来计算所提供的条件，并根据计算的结果返回a+b或a-b。在上面的例子中，只使用了几个不同的条件，但还可以使用任何数量的其他条件，只要坚持使用在`func()`中定义的名称a和b。<br />现在想象一下，如果不使用Python的`eval()`，将如何实现这样的东西。那会花更少的代码和时间吗？不可能！
<a name="KAKhP"></a>
### **数学表达式**
Python 的 `eval()` 的一个常见用例是对基于字符串的输入进行 math 表达式的计算。例如，创建一个 Python 计算器，那么可以使用 `eval()` 来计算用户的输入并返回计算结果。<br />下面的例子演示了如何使用`eval()`与数学一起进行math运算。
```python
>>> # Arithmetic operations
>>> eval("5 + 7")
12
>>> eval("5 * 7")
35
>>> eval("5 ** 7")
78125
>>> eval("(5 + 7) / 2")
6.0
>>> import math
>>> # 一个圆的面积
>>> eval("math.pi * pow(25, 2)")
1963.4954084936207
>>> # 球体的体积
>>> eval("4 / 3 * math.pi * math.pow(25, 3)")
65449.84694978735
>>> # 直角三角形的斜边
>>> eval("math.sqrt(math.pow(10, 2) + math.pow(15, 2))")
18.027756377319946
```
当使用`eval()`来计算数学表达式时，可以传入任何种类或复杂程度的表达式，`eval()`会解析它们，计算它们，如果一切正常，就会给预期结果。
<a name="sTfkm"></a>
### **通用表达式**
前面已经学会了如何在布尔和 math 表达式中使用 `eval()` 。然而，可以在更复杂的 Python 表达式中使用 `eval()` ，这些表达式包括函数调用、对象创建、属性访问、列表推导式等等。<br />例如，可以调用一个内置函数或用标准或第三方模块导入的函数。
```python
>>> # 运行echo命令
>>> import subprocess
>>> eval("subprocess.getoutput('echo Hello, World')")
'Hello, World'
>>> # 启动Firefox（如果有的话）
>>> eval("subprocess.getoutput('firefox')")
''
```
在这个例子中，使用 Python 的 `eval()` 来执行一些系统命令。可以用这个功能做大量有用的事情。然而，`eval()`也会有一些严重的安全风险，比如允许一个恶意的用户在机器中运行系统命令或任何任意的代码。
<a name="LWQFT"></a>
## 参考资料
eval():[https://docs.python.org/3/library/functions.html#eval](https://docs.python.org/3/library/functions.html#eval)<br />字节码: [https://docs.python.org/3/glossary.html#term-bytecode](https://docs.python.org/3/glossary.html#term-bytecode)<br />**eval()**: [https://docs.python.org/3/library/functions.html#exec](https://docs.python.org/3/library/functions.html#exec)<br />复合语句: [https://docs.python.org/3/reference/compound_stmts.html](https://docs.python.org/3/reference/compound_stmts.html)<br />来源: [https://docs.python.org/3/glossary.html#term-expression](https://docs.python.org/3/glossary.html#term-expression)<br />来源: [https://docs.python.org/3/glossary.html#term-statement](https://docs.python.org/3/glossary.html#term-statement)<br />compile(): [https://docs.python.org/3/library/functions.html#compile](https://docs.python.org/3/library/functions.html#compile)<br />代码对象: [https://docs.python.org/3/library/stdtypes.html#code-objects](https://docs.python.org/3/library/stdtypes.html#code-objects)<br />AST 对象: [https://docs.python.org/3/library/ast.html#ast.AST](https://docs.python.org/3/library/ast.html#ast.AST)<br />字节字符串: [https://docs.python.org/3/library/stdtypes.html#bytes-objects](https://docs.python.org/3/library/stdtypes.html#bytes-objects)<br />值比较运算符**:** [https://docs.python.org/3/reference/expressions.html#value-comparisons](https://docs.python.org/3/reference/expressions.html#value-comparisons)<br />逻辑（布尔）运算符: [https://docs.python.org/3/reference/expressions.html#boolean-operations](https://docs.python.org/3/reference/expressions.html#boolean-operations)<br />成员测试运算符: [https://docs.python.org/3/reference/expressions.html#membership-test-operations](https://docs.python.org/3/reference/expressions.html#membership-test-operations)<br />身份运算符: [https://docs.python.org/3/reference/expressions.html#is-not](https://docs.python.org/3/reference/expressions.html#is-not)
