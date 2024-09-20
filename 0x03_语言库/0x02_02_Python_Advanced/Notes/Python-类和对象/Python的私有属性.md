顾名思义，私有属性藏匿在代码的深处，只有**开发者**才能触及到它们。
<a name="CmfYw"></a>
## 私有属性长什么样？

1. 类中的属性如果加了两个下划线 `__` 代表这个属性是类私有的；
2. 属性私有化之后 不能在外部直接访问， 但是在内部是可以访问的。

示例代码：
```python
class MysteriousBox:
    def __init__(self):
        self.__secret = "这是一个绝密宝盒"

    def open_box(self):
        return self.__secret

box = MysteriousBox()
print(f"我想看看秘密：{box.__secret}")
print(f"打开宝盒，你将发现：{box.open_box()}")
```
运行之后：
```
File "d:\ms\test.py", line 10, in <module>       
    print(f"我想看看秘密：{box.__secret}")
                    ^^^^^^^^^^^^
AttributeError: 'MysteriousBox' object has no attribute '__secret'
```
如果把`print(f"我想看看秘密：{box.__secret}")`这行注释掉，再次运行代码就能借助`open_box`拿到`__secret`了：
```
打开宝盒，你将发现：这是一个绝密宝盒
```
**至此，再回过头看看最开始的那两条定义。**
<a name="tq5k2"></a>
## 私有化方法

1. 私有化方法跟私有化属性的概念一样，有些重要的方法，不允许外部调用，防止子类意外重写，把普通的方法设置为私有化方法；
2. 私有化方法，即在方法名前面加两个下划线。 示例代码：
```python
class MysteriousBox:
    def __init__(self):
        self.secret = "这是一个绝密宝盒"

    def __open_box(self):
        return self.secret

    def open_box(self):
        return self.__open_box()

box = MysteriousBox()

print(f"打开宝盒，你将发现：{box.open_box()}")
print(f"打开宝盒，你将发现：{box.__open_box()}")
```
运行之后：
```
打开宝盒，你将发现：这是一个绝密宝盒
Traceback (most recent call last):
  File "d:\ms\test.py", line 14, in <module>
    print(f"打开宝盒，你将发现：{box.__open_box()}")
                       ^^^^^^^^^^^^^^
AttributeError: 'MysteriousBox' object has no attribute '__open_box'. Did you mean: 'open_box'?
```
可以发现更外层的`box.open_box()`是能成功被调用的，而私有方法`box.__open_box()`在**直接调用**的时候报错了。
<a name="fpyMZ"></a>
## 最后总结
<a name="VERHe"></a>
### 定义及作用
私有化的实例（对象）属性是指在类定义中以两个下划线 "`__`" 开头的属性。这种属性被称为**私有属性**，其作用是限制对属性的直接访问。<br />私有属性不能在类的外部**直接访问**，但可以在类的内部的任何方法中自由使用。<br />这种封装性有助于确保对象的**数据安全性**和**一致性**，同时隐藏实现细节。
<a name="esDmk"></a>
### 不可继承
子类不能继承父类的私有属性，但可以继承父类的公共属性和行为。这是面向对象编程中的一个重要概念，称为封装性。<br />私有属性是父类内部的实现细节，**子类无法直接访问或继承这些属性**。<br />然而，子类可以继承父类的公共属性和方法，以便重用和扩展父类的功能。
