Python装饰器
<a name="PeK1P"></a>
## 工程实例
装饰器是一个基础概念也是让初学者很迷糊的概念，但每个中大型工程里面都会用到，搞不清楚的话就会看得云里雾里，很多功能找不到在哪里实现的。比如以下这个工程实例：（出自Featdepth模型源码）
```python
from mono.model.registry import MONO
from mmcv import Config

def main():
    args = parse_args()
    cfg = Config.fromfile(args.config)  
    model_name = cfg.model['name']    # 返回模型名称字符串
    model = MONO.module_dict[model_name](cfg.model)
    ……

if __name__ == '__main__':
    main()
```
看到这里就会疑惑 `model = MONO.module_dictmodelname(cfg.model)`这一句中module_dict是哪里来的，来看看导入的MONO模块：
```python
class Registry(object):
    def __init__(self, name):
        self._name = name
        self._module_dict = dict()

    @property
    def name(self):
        return self._name

    @property
    def module_dict(self):
        return self._module_dict

    def _register_module(self, module_class):
        """Register a module.

        Args:
            module (:obj:`nn.Module`): Module to be registered.
        """
        if not issubclass(module_class, nn.Module):
            raise TypeError(
                'module must be a child of nn.Module, but got {}'.format(
                    module_class))
        module_name = module_class.__name__
        if module_name in self._module_dict:
            raise KeyError('{} is already registered in {}'.format(
                module_name, self.name))
        self._module_dict[module_name] = module_class

    def register_module(self, cls):
        self._register_module(cls)
        return cls

MONO = Registry('mono')
```
原来MONO是一个实例化的Registry对象，里面有module_dict这个属性，但是这个字典里面目前是空的，如何取到里面的元素呢？<br />可以发现mono.model有个_init_.py文件，这个文件在模块导入时候会自动执行，里面内容是：
```python
from .mono_baseline.net import Baseline
from .mono_autoencoder.net import autoencoder
from .mono_fm.net import mono_fm
from .mono_fm_joint.net import mono_fm_joint
```
说明这些.net文件在导入的时候也被执行了。随便打开其中一个.net文件：
```python
rom ..registry import MONO

@MONO.register_module
class autoencoder(nn.Module):
    def __init__(self, options):
        super(autoencoder, self).__init__()
        self.opt = options
        ……
```
这里的`@MONO.register_module`就是用到了装饰器，这个函数在Registry类中定义，功能就是将被装饰的类以类名为key，存入module_dict中，所以在本文最开始的代码中：
```python
model = MONO.module_dict[model_name](cfg.model)
```
这一句就可以用模型名称（model_name）从module_dict中取到，同时进行实例化了。<br />可以说这是装饰器的一个很典型的应用，可以实现设计模式中的工厂模式，很方便地将各个模块在定义时即注册到工厂中，从而可以很灵活地使用。通过这个例子深入研究装饰器，结合网上的各种教程，可以总结出以下关键点：
<a name="qwLMW"></a>
## 1、装饰器是闭包的语法糖
所谓语法糖及语法上完全等价的表达方式。装饰器和闭包是完全等价的，及本身是一个函数，接收的参数是函数，返回的还是一个函数：
```python
def funcA(func):
    def funcB():
        print('funcA')
        func()
    return funcB
```
此时如果用funcA来装饰一个函数：
```python
@funcA
def funcC():
    print('funcC')
```
等价于闭包的写法：
```python
funcC = funcA(funcC)
```
运行结果：
```python
funcC()
```
输出：
```python
funcA
funcC
```
可见装饰器和闭包的功能都是对函数功能进行增强，变成一个新的增强函数。
<a name="mR8gl"></a>
## 2、装饰器在定义时及执行，在被装饰函数被调用时不再执行
在funcA中加一句打印：
```python
def funcA(func):
    print('funcAAA')
    def funcB():
        print('funcA')
        func()
    return funcB

@funcA
def funcC():
    print('funcC')
```
在此模块被导入或执行时（funcA和funcC均未被调用），funcA及被执行：<br />输出：
```python
funcAAA
```
而在funcC被调用时不会再输出funcAAA：
```python
funcC()
```
输出：
```python
funcA
funcC
```
这是因为此时funC已经变成了funcA返回的新函数funcB，被调用的时候和funcA已经没有关系了，而且funcC也不存在了：
```python
print(funcC.__name__)
```
输出：
```python
funcB
```
<a name="hS9oz"></a>
## 3、多个装饰器的执行顺序：定义时从下往上，调用时从上往下
```python
def func_a(func):
    print(1)
    def func_a1():
        print(11)
        func()
    return func_a1

def func_b(func):
    print(2)
    def func_b1():
        print(22)
        func()
    return func_b1

@func_a
@func_b
def func_c():
    print(3)
```
直接执行输出：
```python
2
1
```
调用时：
```python
func_c()
```
输出：
```python
11
22
3
```
这个其实也好理解，写成闭包的形式：`func_c = func_a(func_b(func_c))` 因为内层优先级高，所以定义时从内向外执行，而在调用时，func_c已经变成了func_a的内层函数funca1，里面又包含了func_b的内层函数func_b1，所以从外向内执行，相当于入栈和出栈。
<a name="fbGXl"></a>
## 4、带参数和返回值的装饰函数写法
<a name="P59wA"></a>
### 被装饰函数有参数和返回值：
```python
def func_a(func):
    def func_a1(*args, **kwargs):
        a = func(*args, **kwargs) + 1
        return a
    return func_a1

@func_a
def func_b(num):
    return num
```
调用：
```python
m = func_b(100)
print(m)
```
输出：
```python
101
```
可见如果被装饰函数有参数和返回值，则在装饰器的内层函数中设置相应的参数和返回值，和被装饰函数保持一致即可。参数可写成通用模式：*args, **kwargs
<a name="KKjLP"></a>
### 装饰器有参数和返回值：
```python
def func_a(num):
    def func_a1(func):
        def func_a2(*args, **kwargs):
            a = func(*args, **kwargs) + num
            return a
        return func_a2
    return func_a1


@func_a(100)
def func_b(a):
    return a
```
调用：
```python
m = func_b(1000)
print(m)
```
输出：
```python
1100
```
这时候看看被装饰函数func_b变成了什么：
```python
print(func_b.__name__)
```
输出：
```python
func_a2
```
可见如果装饰器有参数和返回值，则需要在装饰器中再加一层，也就是一共三层，第外层接收装饰器的参数，中间层接收被装饰函数，最内层接收被装饰函数的参数。这个时候被装饰函数还是变成了装饰器的最内层函数，只是装饰器要多一层接收自己的参数而已。
<a name="o32ZA"></a>
## 5、装饰器或被装饰对象是类的情况
<a name="MLD4a"></a>
### 被装饰对象是类：
```python
def wrapClass(cls):
    def inner(para):
        print('param name:', para)
        return cls(para)
    return inner

@wrapClass
class Foo():
    def __init__(self, a):
        self.a = a

    def fun(self):
        print('self.a =', self.a)
```
调用：
```python
Foo('apple')
```
输出：
```python
param name: apple
```
可见如果被装饰对象是类，那装饰器返回的也应该是类
<a name="EjqfX"></a>
### 装饰器是类：
先看类装饰类的情况：
```python
class ShowClassName(object):
    def __init__(self, cls):
        self._cls = cls

    def __call__(self, a):
        print('class name:', self._cls.__name__)
        return self._cls(a)


@ShowClassName
class Foobar(object):
    def __init__(self, a):
        self.value = a

    def fun(self):
        print(self.value)
```
调用：
```python
a = Foobar('apple')
```
输出：
```python
class name: Foobar
```
再看类装饰函数的情况：
```python
class ShowFunName():
    def __init__(self, func):
        self._func = func

    def __call__(self, a):
        print('function name:', self._func.__name__)
        return self._func(a)


@ShowFunName
def Bar(a):
    return a
```
调用：
```python
print(Bar('apple'))
```
输出：
```python
function name: Bar
apple
```
可见如果装饰器是类，在调用被装饰对象的时候会调用装饰器的`__call__`方法，相当于装饰器是函数情况的内层函数，如果被装饰对象是类则返回类，是函数则返回函数。总之无论装饰器还是被装饰对象是类，本质上和函数装饰函数是一样的，都是返回对被装饰对象的增强。
