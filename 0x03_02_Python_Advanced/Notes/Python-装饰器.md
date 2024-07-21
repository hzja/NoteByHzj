Python 装饰器<br />装饰器(Decorators)是 Python 的一个重要部分。简单地说：它们是修改其他函数的功能的函数。有助于让代码更简短，也更Pythonic（Python范儿）。
<a name="ijl5u"></a>
### 1、装饰器的定义
就是给已有函数增加额外功能的函数，它本质上就是一个闭包函数。装饰器的功能特点:<br />1、不修改已有函数的源代码<br />2、不修改已有函数的调用方式<br />3、给已有函数增加额外的功能<br />4、闭包和装饰器的区分：<br />如果闭包函数的参数有且只有一个，并且是函数类型，那么这个闭包函数称之为装饰器。写代码要遵循开放封闭原则，它规定已经实现的功能代码不允许被修改，但可以被扩展。
<a name="WWMrO"></a>
### 2、装饰器的示例代码
```python
# 定义装饰器
def decorator(func):
    def inner():
        # 在内部函数里面对已有函数进行装饰
        print('已添加登录认证')
        func()
    return inner
def comment():
    print('发表评论')
# 调用装饰器对已有函数进行装饰，左边的comment=inner
comment = decorator(comment)
# 调用方式不变
comment()
```
<a name="uJGNR"></a>
### 3、装饰器的语法糖写法
如果有多个函数都需要添加登录验证的功能，每次都需要编写 `func = decorator(func)` 这样代码对已有函数进行装饰，这种做法还是比较麻烦。<br />Python给提供了一个装饰函数更加简单的写法，那就是语法糖，语法糖的书写格式是: @装饰器名字，通过语法糖的方式也可以完成对已有函数的装饰。
```python
# 定义装饰器
def decorator(func):
    def inner():
        # 在内部函数里面对已有函数进行装饰
        print('已添加登录认证')
        func()
    return inner
@decorator  # comment = decorator(comment) 装饰器语法糖对该代码进行了封装 左边comment=inner
def comment():
    print('发表评论')
# 调用方式不变
comment()
```
<a name="Yn4cs"></a>
### 4、装饰器的执行时机
当前模块加载完成以后，装饰器会立即执行，对已有函数进行装饰。
```python
# 定义装饰器
def decorator(func):
    print('装饰器执行了')
    def inner():
        # 在内部函数里面对已有函数进行装饰
        print('已添加登录认证')
        func()
    return inner
@decorator  # comment = decorator(comment) 装饰器语法糖对该代码进行了封装 左边comment=inner
def comment():
    print('发表评论')
```
<a name="YQYLR"></a>
### 5、装饰器的使用
<a name="WkOpM"></a>
#### 5.1 装饰器的使用场景
```
1.函数执行时间的统计。
2.输出日志信息。
```
<a name="epays"></a>
#### 5.2 装饰器实现已有函数执行时间的统计
```python
import time
def decorator(func):
    def inner():
        # 获取时间距离1970-1-1 0:0:1的时间差
        begin = time.time()
        func()
        end = time.time()
        result = end - begin
        print(f'函数执行完成耗时：{result}')
    return inner
@decorator
def work():
    for i in range(10000):
        print(i)
        
work()
```
<a name="aGKj1"></a>
### 6、通用装饰器的使用
通用装饰器：可以装饰任意类型的函数。使用装饰器装饰已有函数的时候，内部函数的类型和要装饰的已有函数的类型保持一致。
<a name="zcS32"></a>
#### 6.1 装饰带有参数的函数
```python
def decorator(func):
    def inner(num1, num2):
        print('正在努力执行加法计算')
        func(num1, num2)
    return inner
@decorator
def add_num(num1, num2):
    result = num1 + num2
    print(f'结果为：{result}')
add_num(1, 2)
```
<a name="SPUfc"></a>
#### 6.2 装饰带有参数、返回值的函数
```python
def decorator(func):
    def inner(num1, num2):
        print('正在努力执行加法计算')
        num = func(num1, num2)
        return num
    return inner
@decorator
def add_num(num1, num2):
    result = num1 + num2
    return result
result = add_num(1, 2)
print(f'结果为：{result}')
```
<a name="RMXA2"></a>
#### 6.3 装饰带有不定长参数、返回值的函数
```python
def decorator(func):
    def inner(*args, **kwargs):
        print('正在努力执行加法计算')
        # *args:把元组里面的每一个元素，按照位置参数的方式进行传参
        # **kwargs:把字典里面的每一个键值对，按照关键字的方式进行传参
        num = func(*args, **kwargs)
        return num
    return inner
@decorator
def add_num(*args, **kwargs):
    result = 0
    for value in args:
        result += value
    for value in kwargs.values():
        result += value
    return result
result = add_num(1, 2, a=3)
print(f'结果为：{result}')
```
<a name="ANqka"></a>
### 7、多个装饰器的使用
多个装饰器的装饰过程：由内到外的一个装饰过程，先执行内部的装饰器，在执行外部的装饰器。
```python
def make_div(func):
    print('make_div装饰器执行了')
    def inner():
        result = '<div>' + func() + '</div>'
        return result
    return inner
def make_p(func):
    print('make_p装饰器执行了')
    def inner():
        result = '<p>' + func() + '</p>'
        return result
    return inner
# 原理剖析：content = make_div(make_p(content))
# 分布拆解：content = make_p(content)，内部装饰器完成，content = make_p.inner
#          content = make_div(make_p.inner)
@make_div
@make_p
def content():
    return '人生苦短，我用python'
c = content()
print(c)
make_p装饰器执行了
make_div装饰器执行了
<div><p>人生苦短，我用python</p></div>
```
<a name="3zW0w"></a>
### 8、带有参数的装饰器
带有参数的装饰器就是使用装饰器装饰函数的时候可以传入指定参数，语法格式: @装饰器(参数,…)。使用带有参数的装饰器，其实是在装饰器外面又包裹了一个函数，使用该函数接收参数，返回是装饰器，因为 @ 符号需要配合装饰器实例使用。
```python
def return_decorator(flag):
    # 装饰器只能接收一个参数并且是函数类型
    def decorator(func):
        def inner(a, b):
            if flag == '+':
                print('正在努力执行加法计算')
            elif flag == '-':
                print('正在努力执行减法计算')
            func(a, b)
        return inner
    # 当调用函数的时候可以返回一个装饰器decorato
    return decorator
@return_decorator('+')  # decorator = return_decorator('+'), @decorator => add_num = decorator(add_num)
def add_num(a, b):
    result = a + b
    print(result)
    
@return_decorator('-')
def sub_num(a, b):
    result = a - b
    print(result)
add_num(1, 2)
sub_num(1, 2)
```
<a name="mjbTr"></a>
### 9、类装饰器的使用
类装饰器：使用类装饰已有函数
```python
class MyDecorator(object):
    def __init__(self, func):
        self.__func = func
    # 实现__call__方法，表示对象是一个可调用对象，可以像调用函数一样进行调用
    def __call__(self, *args, **kwargs):
        # 对已有函数进行封装
        print('马上就下班')
        self.__func()
@MyDecorator  # @MyDecorator => show = MyDecorator(show)
def show():
    print('快要下雪啦')
# 执行show，就相当于执行MyDecorator类创建的实例对象，show() => 对象()
show()
马上就下班
快要下雪啦
```
:::info
扩展：函数之所以能够调用，是因为函数内部实现了 `call` 方法。
:::
<a name="6Oi8x"></a>
### 10、装饰器应用场景
1、收集函数的操作或错误日志记录。<br />2、验证函数的使用权限。<br />3、计算函数的运行时间。<br />4、在ORM/DB模型操作时，通过属性方法动态地获取关联的数据。<br />5、函数数据的缓存。<br />6、定制函数的输入和输出（序列化和反序列化）。
