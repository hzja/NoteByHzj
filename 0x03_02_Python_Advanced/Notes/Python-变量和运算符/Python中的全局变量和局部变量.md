在Python中，全局变量和局部变量在作用域和访问权限上有明显的区别。全局变量是在整个程序中可访问的变量，而局部变量则仅在其被定义的函数内部可用。
<a name="ZiCTr"></a>
## 1、全局变量
```python
# 定义全局变量
global_var = 10

# 全局变量在函数内部也是可访问的
def func_using_global():
    print(f"访问全局变量 global_var 的值：{global_var}")

# 调用函数
func_using_global()
```
在上面的示例中，global_var 被定义为全局变量，因此它可以在函数 `func_using_global()` 中访问和使用。
<a name="Ocw1N"></a>
## 2、局部变量
```python
# 定义函数，内部有一个局部变量
def func_creating_local():
    local_var = 20
    print(f"局部变量 local_var 的值：{local_var}")

# 调用函数
func_creating_local()

# 尝试在函数外部访问局部变量会导致 NameError
# print(f"尝试在函数外部访问局部变量 local_var：{local_var}")
```
在此示例中，local_var 被定义为函数 `func_creating_local()` 内的局部变量。尝试在函数外部访问局部变量将导致 NameError，因为局部变量的作用域仅限于其被定义的函数内部。
<a name="Vrnd5"></a>
## 3、全局变量在函数内部的修改
```python
global_var = 10

def modify_global():
    global global_var
    global_var = 30
    print(f"在函数内部修改全局变量 global_var 的值为：{global_var}")

modify_global()
print(f"全局变量 global_var 修改后的值为：{global_var}")
```
在此示例中，通过在函数内部使用 global 关键字，成功地修改了全局变量 global_var 的值为 30，并且这个修改是持久的，影响了整个程序。
<a name="stEoV"></a>
## 4、局部变量与全局变量的命名冲突
在编程中，当全局变量与局部变量同名时，可能会导致混淆和意外结果。下面是一个示例：
```python
var = 10  # 全局变量

def func():
    var = 20  # 局部变量
    print(f"在函数内部的局部变量 var 值为：{var}")

func()
print(f"在函数外部的全局变量 var 值为：{var}")
```
这里存在全局变量 var 和函数内部的同名局部变量 var。在函数内部，局部变量 var 的值为 20，但在函数外部，全局变量 var 的值仍为 10。这种命名冲突可能会带来混淆，因此编写代码时需要特别注意。
<a name="EzYP7"></a>
## 5、使用 `nonlocal` 关键字修改嵌套函数中的变量
Python中，还存在一种嵌套函数的情况，即在函数内定义函数。这时，可以使用 `nonlocal` 关键字来修改外层函数的局部变量。
```python
def outer_func():
    outer_var = 30  # 外层函数的局部变量

    def inner_func():
        nonlocal outer_var
        outer_var = 40  # 修改外层函数的局部变量
        print(f"嵌套函数内部修改外部函数变量 outer_var 值为：{outer_var}")

    inner_func()
    print(f"外部函数变量 outer_var 修改后的值为：{outer_var}")

outer_func()
```
这个例子展示了嵌套函数中使用 `nonlocal` 关键字修改了外层函数的局部变量 outer_var 的值，使其从 30 变为 40。
<a name="ehsL1"></a>
## 总结
总结全局变量和局部变量在Python中的区别至关重要。全局变量在整个程序中可被访问，而局部变量仅在其定义的函数内部可见。这种区别涉及作用域和访问权限，影响程序的结构和灵活性。同时，对于避免命名冲突和程序逻辑的清晰性也至关重要。<br />合理使用全局变量和局部变量可以提高代码的可读性和维护性。同时，对于嵌套函数中局部变量的修改，`nonlocal` 关键字在修改外层函数的变量值时是非常有用的。通过理解这些概念，能够更好地利用Python的强大功能和灵活性，写出更加优雅和健壮的代码。全局变量和局部变量的正确使用对于构建优秀的程序至关重要。
