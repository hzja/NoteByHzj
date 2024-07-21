Python
<a name="cSYxS"></a>
### `__getattribute__`方法
先来定义一个简单的类，访问属性没问题。
```python
class User:

    def __init__(self, name, sex):
        self.name = name
        self.sex = sex


u1 = User('li', 'male')
print(u1.name)
```
```
li
```
接着加入`__getattribute__`方法后，访问属性就会交给该方法处理。
```python
class User:

    def __getattribute__(self, item):
        return 6

    def __init__(self, name, sex):
        self.name = name
        self.sex = sex


u1 = User('li', 'male')
print(u1.name)
```
```
6
```
更加神奇的是，不管这个属性存不存在，都会返回6。
```python
class User:

    def __getattribute__(self, item):
        return 6

    def __init__(self, name, sex):
        self.name = name
        self.sex = sex


u1 = User('li', 'male')
print(u1.test)
```
```
6
```

- **触发时间：在访问对象成员属性的时候触发，无论是否存在。**
- **作用：对属性进行一些处理。**
- **参数：self为当前对象，item是访问属性名称的字符串。**
- **返回值：属性值。**

这里需要注意的是返回值千万不能用`self.name`，这样会无限递归下去，可以使用object的`__getattribute__`方法来访问。
```python
class User:

    def __getattribute__(self, item):
        return object.__getattribute__(self, item)

    def __init__(self, name, sex):
        self.name = name
        self.sex = sex


u1 = User('li', 'male')
print(u1.name)
```
```
li
```
<a name="sj8s7"></a>
### `__getattr__`方法

- **触发时间：在访问不存在的对象属性的时候触发。**
- **作用：访问不存在的属性时候，不会报错。**
- **参数：self为当前对象，item是访问属性名称的字符串。**
- **返回值：属性值。**

举个简单例子，就是用户可能会输出属性名称，那都统一返回name属性。
```python
class User:

    def __getattr__(self, item):
        return self.name

    def __init__(self, name, sex):
        self.name = name
        self.sex = sex


u1 = User('li', 'male')
print(u1.na2me)
```
```
li
```
<a name="IiHGG"></a>
### `__setattr__`方法

- **触发时间：添加和修改对象属性的时候触发。**
- **作用：限制添加和修改对象属性的操作。**
- **参数：self为当前对象，key为设置对象属性名称， value为设置的值。**
- **返回值：无。**

例如，让用户可以改`name`，不能改`sex`。
```python
class User:

    sex = 'male'

    def __setattr__(self, key, value):
        if key == 'sex':
            pass
        else:
            object.__setattr__(self, key, value)

    def __init__(self, name):
        self.name = name


u1 = User('li')
u1.name = 'test'
u1.sex = 'female'
print(u1.name, u1.sex)
```
```
test male
```
<a name="Hug1B"></a>
### `__delattr__`方法

- **触发时间：删除对象属性的时候触发。**
- **作用：限制添加和修改对象属性的操作。**
- **参数：self为当前对象，item为删除对象属性名称。**
- **返回值：无。**

可以让用户删除`sex`，但是不能删除`name`。
```python
class User:

    def __delattr__(self, item):
        if item == 'sex':
            pass
        else:
            object.__delattr__(self, item)

    def __init__(self, name, sex):
        self.name = name
        self.sex = sex


u1 = User('li', 'male')
del u1.sex
print(u1.sex)
del u1.name
print(u1.name)
```
```
male
AttributeError: 'User' object has no attribute 'name'
```
<a name="bifcm"></a>
### `__dir__`方法
该方法返回类或者对象的所有成员名称列表，用的不是很多。
<a name="fNLLM"></a>
### 属性访问顺序

- `**__getattribute__**`
- **数据描述符**
- **当前对象的属性**
- **类的属性**
- **非数据描述符**
- **父类的属性**
- `**__getattr__**`
