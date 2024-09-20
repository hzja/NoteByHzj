Python 装饰器 
<a name="NvMZw"></a>
## 1、`@staticmethod`
装饰器 `@staticmethod` 修饰的方法称为：静态方法，和普通的函数没有什么区别
<a name="ePsIj"></a>
#### 1、要调用一个静态方法，一般使用形式是：「 `类名.方法名()` 」
```python
class Web(object):
    @staticmethod
    def foo_staticmethod():
        """静态方法"""
        pass
if __name__ == '__main__':
    # 直接使用类名+方法名调用
    Web.foo_staticmethod()
```
当然，也可以实例化一个类对象，通过这个对象去调用静态方法，但是不建议使用这种方式
```python
# 实例化一个对象
instance = Web()
# 使用实例对象去调用静态方法（不建议）
instance.foo_staticmethod()
```
<a name="3v1Mb"></a>
#### 2、针对类中定义的静态变量，可以使用「 `类名.变量名` 」 的形式去访问
```python
class Web(object):
    # 静态变量（类变量）
    name = "Python_Web"
    @staticmethod
    def foo_staticmethod():
        """静态方法"""
        # 引用静态变量
        print(Web.name)
```
<a name="B2rPT"></a>
#### 3、静态方法内部使用其他静态方法、类方法，同样是使用「 `类名.方法名()` 」去调用
```python
class Web(object):
    # 静态变量（类变量）
    name = "Python_Web"
    # 类方法
    @classmethod
    def foo_classmethod_other(cls):
        print('类方法被调用！')
    # 另外一个静态方法
    @staticmethod
    def foo_staticmethod_other():
        print('另外一个静态方法被调用！')
    @staticmethod
    def foo_staticmethod():
        """静态方法"""
        # 调用其他静态方法
        print(Web.foo_staticmethod_other()) 
        # 调用类方法
        print(Web.foo_classmethod_other())
```
<a name="xlUNy"></a>
#### 4、静态方法内部调用普通方法，访问实例属性
普通方法和实例属性都必须通过实例对象去引用，不能直接使用类名去访问
```python
class Web(object):
    def __init__(self):
        self.desc = "实例属性，不共享"
    def norm_method(self):
        """普通方法"""
        print('普通方法被调用！')
    @staticmethod
    def foo_staticmethod():
        """静态方法"""
        instance = Web()
        # 获取实例属性
        print(instance.desc)
        # 调用普通方法
        instance.norm_method()
```
<a name="02fec881"></a>
#### 5、子类的使用
在子类中调用父类定义好的静态方法，只需要将类名替换为子类名称即可
```python
class Web(object):
    @staticmethod
    def foo_staticmethod(arg1, arg2):
        pass
class Django(Web):
    """子类"""
    pass
if __name__ == '__main__':
    # 1、使用类名（父类）去调用静态方法
    Web.foo_staticmethod("Hello", ",AirPython")
    # 2、使用类名（子类）去调用静态方法
    Django.foo_staticmethod("Hello", ",AirPython")
```
<a name="35Jpl"></a>
### 2、`@classmethod`
装饰器 `@classmethod` 修饰的方法称为：类方法，在使用的时候，会将类本身作为第一个参数 cls 传递给类方法
```python
# 类方法，第一个参数为cls，代表类本身
@classmethod
def foo_classmethod(cls):
    pass
```
其中，cls 代表外层类本身，可以实例化，也可以直接调用静态方法、类方法、静态变量
<a name="3gypv"></a>
#### 1、要调用一个类方法，一般使用形式是：「 `类名.方法名()` 」
```python
class Web(object):
    # 类方法，第一个参数为cls，代表类本身
    @classmethod
    def foo_classmethod(cls):
        pass
if __name__ == '__main__':
    # 使用类名去调用类方法
    Web.foo_classmethod()
```
和静态方法类似，也可以实例化一个类对象，通过这个对象去调用静态方法，但是不建议使用这种方式
<a name="7ab79388"></a>
#### 2、调用静态变量
静态方法内部引用静态变量有两种方式，分别是：

- 「 `类名.变量名` 」
- 「 `cls.变量名` 」

注意：由于 cls 代表就是外层类本身，所以这两种方式等效
```python
class Web(object):
    # 静态变量（类变量）
    name = "Python_Web"
    # 类方法，第一个参数为cls，代表类本身
    @classmethod
    def foo_classmethod(cls):
        # 调用静态变量方式一
        print(cls.name)
        # 调用静态变量方式二
        print(Web.name)
```
<a name="bisQw"></a>
#### 3、类方法内部调用其他类方法、静态方法
在一个类方法内部，可以使用「 `类名.类方法名()` 」、「 `类名.静态方法名()` 」的形式去调用方法
```python
class Web(object):
    # 静态方法
    @staticmethod
    def foo_staticmethod():
        print('静态方法被调用！')
    # 其他类方法
    @classmethod
    def foo_classmethod_other(cls):
        print('另外一个类方法被调用！')
    # 类方法，第一个参数为cls，代表类本身
    @classmethod
    def foo_classmethod(cls):
        # 调用其他类方法
        cls.foo_classmethod_other()
        # 调用静态方法
        cls.foo_staticmethod()
if __name__ == '__main__':
    Web.foo_classmethod()
```
<a name="L8fyM"></a>
#### 4、类方法内部调用普通方法，访问实例属性
需要通过 cls 变量实例化一个类对象，然后通过这个对象去调用普通方法和实例属性
```python
class Web(object):
    def __init__(self):
        self.desc = "实例属性，不共享"
    def norm_method(self):
        """普通方法"""
        print('普通方法被调用！')
    # 类方法，第一个参数为cls，代表类本身
    @classmethod
    def foo_classmethod(cls):
        # 如果要调用实例属性，必须使用cls实例化一个对象，然后再去引用
        print(cls().desc)
        # 如果要调用普通方法，必须使用cls实例化一个对象，然后再去引用
        cls().norm_method()
```
<a name="XxRwu"></a>
#### 5、子类的使用
在子类中调用父类定义好的类方法，只需要将类名替换为子类名称即可，代码和静态方法类似。
<a name="0mhOI"></a>
### 3.区别
下面总结一下普通方法、静态方法、类方法的区别

- 普通方法：第一个参数 self 代表实例对象本身，可以使用 self 直接引用定义的实例属性和普通方法；如果需要调用静态方法和类方法，通过「 `类名.方法名()` 」调用即可。
- 静态方法：使用「 `类名.静态变量` 」引用静态变量，利用「 `类名.方法名()` 」调用其他静态方法和类方法；如果需要调用普通方法，需要先实例化一个对象，然后利用对象去调用普通方法。
- 类方法：第一个参数 cls 代表类本身（等价），通过「 `cls.静态变量` 」或「 `类名.静态变量` 」引用静态变量，利用「 `cls.方法名()` 」或「 `类名.方法名()` 」去调用静态方法和类方法；如果需要调用普通方法，需要先实例化一个对象，然后利用对象去调用普通方法。
- 静态方法和类方法是针对类定义的，除了可以使用类名去调用，也可以使用实例对象去调用，但是不建议。
<a name="nY1Vp"></a>
### 4.最后
一般来说，如果方法内部涉及到实例对象属性的操作，建议用普通方法；如果方法内部没有操作实例属性的操作，仅仅包含一些工具性的操作，建议使用静态方法；而如果需要对类属性，即静态变量进行限制性操作，则建议使用类方法。
