Python<br />在介绍`@property`前先介绍`property()`。<br />`property()`构造函数返回property属性。其语法是：
```python
property(fget=None, fset=None, fdel=None, doc=None)
```
`**property()**`**参数：**

1. `fget (optional)` - 用于获取属性值，默认为None。
2. `fset (optional)` - 用于设置属性值，默认为None。
3. `fdel (optional)` - 用于删除属性值。默认为None。
4. `doc (optional)` - 包含该属性的文档(docstring)的字符串。默认为None。

`**property()**`**返回值：**<br />`property()`从给定的`getter`、`setter`和`deleter`返回`property`属性。

1. 如果没有给出参数，`property()`返回一个不包含任何`getter`、`setter`或`deleter`的基属性属性。
2. 如果没有提供doc，`property()`接受`getter`函数的文档字符串。

**示例1：使用getter、setter和deleter创建属性**
```python
class Person:
    def __init__(self, name):
        self._name = name

    def get_name(self):
        print('Getting name')
        return self._name

    def set_name(self, value):
        print('Setting name to ' + value)
        self._name = value

    def del_name(self):
        print('Deleting name')
        del self._name

    # Set property to use get_name, set_name
    # and del_name methods
    name = property(get_name, set_name, del_name, 'Name property')

p = Person('Adam')
print(p.name)
p.name = 'John'
del p.name
```
结果：
```python
Getting name
The name is: Adam
Setting name to John
Deleting name
```
这里，`_name`被用作**私有变量**，用于存储Person的名称。<br />还设置：

1. `getter`方法`get_name()`来获取人员的姓名，
2. `setter`方法`set_name()`来设置人员的姓名，
3. `deleter`方法`del_name()`来删除人员的姓名。

**通过调用**`**property()**`**方法来设置一个新的属性**name。<br />如程序所示，内部引用`p.name`调用`get_name()`作为`getter`, `set_name()`作为`setter`, `del_name()`作为`deleter`，通过方法内部的打印输出。<br />**示例2：使用**`**@property**`**装饰器**<br />可以使用Python**装饰器**`@property`来分配`getter`、`setter`和`deleter`，而不是使用`property()`。
```python
class Person:
    def __init__(self, name):
        self._name = name

    @property
    def name(self):
        print('Getting name')
        return self._name

    @name.setter
    def name(self, value):
        print('Setting name to ' + value)
        self._name = value

    @name.deleter
    def name(self):
        print('Deleting name')
        del self._name

p = Person('Adam')
print('The name is:', p.name)
p.name = 'John'
del p.name
```
输出：
```python
Getting name
The name is: Adam
Setting name to John
Deleting name
```
这里使用了`@property`装饰器，而不是`property()`。

1. 首先，指定`name()`方法也是Person的一个属性。这是通过**在**`**getter**`**方法之前使用**`**@property**`**来实现**的。
2. 接下来，**使用属性名来指定**`**setter**`**和**`**deleter**`。这是通过使用`@name`完成的。`@name.setter`用于`setter`方法，`@name.deleter`用于`deleter`方法。

在定义`getter`、`setter`和`deleter`时，使用了相同的方法`name()`。现在，无论何时使用 `p.name`，它都会在内部调用相应的`getter`、`setter`和`deleter`。
