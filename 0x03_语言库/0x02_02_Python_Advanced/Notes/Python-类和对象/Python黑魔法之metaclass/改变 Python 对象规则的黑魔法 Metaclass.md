Python
<a name="NdZtN"></a>
## Python类的上帝-type
说到metaclass，首先必须清楚一个最基础的概念就是对象是类的实例，而类是type的实例，重复一遍：

1. 对象是类的实例
2. 类是type的实例

在面向对象的编程模型中，类就相当于一个房子的设计图纸，而对象则是根据这个设计图纸建出来的房子。<br />下图中，玩具模型就可以代表一个类，而具体生产出来的玩具就可以代表一个对象。<br />总之，类就是创建对象的模板。<br />而type又是创建类的模板，那么就可以通过type创建自己想要的类。<br />比如定义一个 Hello 的 class：
```python
class Hello(object):
	def hello(self, name='world'):
		print('Hello, %s.' % name)
```
当 Python 解释器载入 hello 模块时，就会依次执行该模块的所有语句，执行结果就是动态创建出一个 Hello 的 class对象。<br />`type()`函数既可以查看一个类型或变量的类型，也可以根据参数创建出新的类型，比如上面那段类的定义本质上就是：
```python
def hello(self, name='world'):
    print('Hello, %s.' % name)
Hello = type('Hello', (object,), dict(hello=hello))
```
`type()`函数创建class 对象，依次传入 3 个参数：

- class 类的名称；
- 继承的父类集合，注意 Python 支持多重继承，如果只有一个父类，别忘了 tuple 的单元素写法；
- class 的方法名称与函数绑定以及字段名称与对应的值，这里把函数 fn 绑定到方法名 hello 上。

通过 `type()` 函数创建的类和直接写 class 是完全一样的，因为 Python 解释器遇到 class 定义时，仅仅是扫描一下class 定义的语法，然后调用 `type()` 函数创建出 class。<br />正常情况下，肯定都是用 class Xxx... 来定义类，但是`type()` 函数允许动态创建出类来，这意味着Python这门动态语言支持运行期动态创建类。可能感受不到这有多强大，要知道想在静态语言运行期创建类，必须构造源代码字符串再调用编译器，或者借助一些工具生成字节码实现，本质上都是动态编译，会非常复杂。
<a name="OGnjk"></a>
## metaclass到底是什么
那type和metaclass有什么关系呢？metaclass到底是什么呢？<br />metaclass 其实就是type或type的子类，通过继承type，重载`__call__`运算符，便可以在class类对象创建时作出一些修改。<br />对于类 MyClass：
```python
class MyClass():
	pass
```
其实相当于：
```python
class MyClass(metaclass = type):
	pass
```
一旦把它的 metaclass 设置成 MyMeta：
```python
class MyClass(metaclass = MyMeta):
	pass
```
MyClass 就不再由原生的 type 创建，而是会调用 MyMeta 的`__call__`运算符重载。
```python
class = type(classname, superclasses, attributedict) 
## 变为了
class = MyMeta(classname, superclasses, attributedict)
```
对于具有继承关系的类：
```python
class Foo(Bar):
	pass
```
Python做了如下的操作：

- Foo中有`__metaclass__`这个属性吗？如果是，Python会通过`__metaclass__`创建一个名字为Foo的类(对象)
- 如果Python没有找到`__metaclass__`，它会继续在Bar（父类）中寻找`__metaclass__`属性，并尝试做和前面同样的操作。
- 如果Python在任何父类中都找不到`__metaclass__`，它就会在模块层次中去寻找`__metaclass__`，并尝试做同样的操作。
- 如果还是找不到`__metaclass__`，Python就会用内置的type来创建这个类对象。

假想一个很傻的例子，决定在模块里所有的类的属性都应该是大写形式。有好几种方法可以办到，但其中一种就是通过在模块级别设定`__metaclass__`：
```python
class UpperAttrMetaClass(type):
    ## __new__ 是在__init__之前被调用的特殊方法
    ## __new__是用来创建对象并返回之的方法
    ## 而__init__只是用来将传入的参数初始化给对象
    ## 很少用到__new__，除非希望能够控制对象的创建
    ## 这里，创建的对象是类，希望能够自定义它，所以这里改写__new__
    ## 如果希望的话，也可以在__init__中做些事情
    ## 还有一些高级的用法会涉及到改写__call__特殊方法，但是这里不用
    def __new__(cls, future_class_name, future_class_parents, future_class_attr):
        ##遍历属性字典，把不是__开头的属性名字变为大写
        newAttr = {}
        for name,value in future_class_attr.items():
            if not name.startswith("__"):
                newAttr[name.upper()] = value

        ## 方法1：通过'type'来做类对象的创建
        ## return type(future_class_name, future_class_parents, newAttr)

        ## 方法2：复用type.__new__方法，这就是基本的OOP编程
        ## return type.__new__(cls, future_class_name, future_class_parents, newAttr)

        ## 方法3：使用super方法
        return super(UpperAttrMetaClass, cls).__new__(cls, future_class_name, future_class_parents, newAttr)


class Foo(object, metaclass = UpperAttrMetaClass):
    bar = 'bip'

print(hasattr(Foo, 'bar'))
## 输出: False
print(hasattr(Foo, 'BAR'))
## 输出:True

f = Foo()
print(f.BAR)
## 输出:'bip'
```
<a name="pKbaC"></a>
## 简易ORM框架的设计
ORM全称“Object Relational Mapping”，即对象-关系映射，就是把关系数据库的一行映射为一个对象，也就是一个类对应一个表，这样，写代码更简单，不用直接操作SQL语句。<br />现在设计一下ORM框架的调用接口，比如用户想通过User类来操作对应的数据库表User，期待他写出这样的代码：
```python
class User(Model):
    ## 定义类的属性到列的映射：
    id = IntegerField('id')
    name = StringField('username')
    email = StringField('email')
    password = StringField('password')

## 创建一个实例：
u = User(id=12345, name='xiaoxiaoming', email='test@orm.org', password='my-pwd')
## 保存到数据库：
u.save()
```
上面的接口通过常规方法很难或几乎很难实现，但通过metaclass就会相对比较简单。核心思想就是通过metaclass修改类的定义，将类的所有Field类型的属性，用一个额外的字典去保存，然后从原定义中删除。对于User创建对象时传入的参数（id=12345, name='xiaoxiaoming'等）可以模仿字典的实现或直接继承dict类保存起来。<br />其中，父类Model和属性类型StringField、IntegerField是由ORM框架提供的，剩下的魔术方法比如save()全部由metaclass自动完成。虽然metaclass的编写会比较复杂，但ORM的使用者用起来却异常简单。<br />首先定义Field类，它负责保存数据库表的字段名和字段类型：
```python
class Field(object):

    def __init__(self, name, column_type):
        self.name = name
        self.column_type = column_type

    def __str__(self):
        return '<%s:%s>' % (self.__class__.__name__, self.name)
```
在Field的基础上，进一步定义各种类型的Field，比如StringField，IntegerField等等：
```python
class StringField(Field):

    def __init__(self, name):
        super(StringField, self).__init__(name, 'varchar(100)')

class IntegerField(Field):

    def __init__(self, name):
        super(IntegerField, self).__init__(name, 'bigint')
```
下一步，编写ModelMetaclass：
```python
class ModelMetaclass(type):

    def __new__(cls, name, bases, attrs):
        if name == 'Model':
            return type.__new__(cls, name, bases, attrs)
        print('Found model: %s' % name)
        mappings = dict()
        for k, v in attrs.items():
            if isinstance(v, Field):
                print('Found mapping: %s ==> %s' % (k, v))
                mappings[k] = v
        for k in mappings.keys():
            attrs.pop(k)
        attrs['__mappings__'] = mappings  ## 保存属性和列的映射关系
        attrs.setdefault('__table__', name) ## 当未定义__table__属性时，表名直接使用类名
        return type.__new__(cls, name, bases, attrs)
```
以及基类Model：
```python
class Model(dict, metaclass=ModelMetaclass):

    def __init__(self, **kw):
        super(Model, self).__init__(**kw)

    def __getattr__(self, key):
        try:
            return self[key]
        except KeyError:
            raise AttributeError(r"'Model' object has no attribute '%s'" % key)

    def __setattr__(self, key, value):
        self[key] = value

    def save(self):
        fields = []
        params = []
        args = []
        for k, v in self.__mappings__.items():
            fields.append(v.name)
            params.append('?')
            args.append(getattr(self, k, None))
        sql = 'insert into %s (%s) values (%s)' % (self.__table__, ','.join(fields), ','.join(params))
        print('SQL: %s' % sql)
        print('ARGS: %s' % str(args))
```
在`ModelMetaclass`中，一共做了几件事情：

1. 在当前类（比如User）中查找定义的类的所有属性，如果找到一个Field属性，就把它保存到一个`__mappings__`的dict中，同时从类属性中删除该Field属性（避免实例的属性遮盖类的同名属性）；
2. 当类中未定义`__table__`字段时，直接将类名保存到`__table__`字段中作为表名。

在Model类中，就可以定义各种操作数据库的方法，比如`save()`，`delete()`，`find()`，`update`等等。<br />实现了`save()`方法，把一个实例保存到数据库中。因为有表名，属性到字段的映射和属性值的集合，就可以构造出INSERT语句。<br />测试：
```python
u = User(id=12345, name='xiaoxiaoming', email='test@orm.org', password='my-pwd')
u.save()
```
输出如下：
```python
Found model: User
Found mapping: id ==> <IntegerField:id>
Found mapping: name ==> <StringField:username>
Found mapping: email ==> <StringField:email>
Found mapping: password ==> <StringField:password>
SQL: insert into User (id,username,email,password) values (?,?,?,?)
ARGS: [12345, 'xiaoxiaoming', 'test@orm.org', 'my-pwd']
```
测试2：
```python
class Blog(Model):
    __table__ = 'blogs'
    id = IntegerField('id')
    user_id = StringField('user_id')
    user_name = StringField('user_name')
    name = StringField('user_name')
    summary = StringField('summary')
    content = StringField('content')


b = Blog(id=12345, user_id='user_id1', user_name='xxm', name='orm框架的基本运行机制', summary="简单讲述一下orm框架的基本运行机制",
         content="此处省略一万字...")
b.save()
```
输出：
```python
Found model: Blog
Found mapping: id ==> <IntegerField:id>
Found mapping: user_id ==> <StringField:user_id>
Found mapping: user_name ==> <StringField:user_name>
Found mapping: name ==> <StringField:user_name>
Found mapping: summary ==> <StringField:summary>
Found mapping: content ==> <StringField:content>
SQL: insert into blogs (id,user_id,user_name,user_name,summary,content) values (?,?,?,?,?,?)
ARGS: [12345, 'user_id1', 'xxm', 'orm框架的基本运行机制', '简单讲述一下orm框架的基本运行机制', '此处省略一万字...']
```
可以看到，`save()`方法已经打印出了可执行的SQL语句，以及参数列表，只需要真正连接到数据库，执行该SQL语句，就可以完成真正的功能。
<a name="eeYA4"></a>
## YAML序列化工具的实现原理浅析
YAML是一个家喻户晓的 Python 工具，可以方便地序列化 / 逆序列化结构数据。<br />官方文档：[https://pyyaml.org/wiki/PyYAMLDocumentation](https://pyyaml.org/wiki/PyYAMLDocumentation)<br />安装：<br />pip install pyyaml

YAMLObject 的任意子类支持序列化和反序列化（serialization & deserialization）。比如说下面这段代码：
```python
import yaml


class Monster(yaml.YAMLObject):
    yaml_tag = '!Monster'

    def __init__(self, name, hp, ac, attacks):
        self.name = name
        self.hp = hp
        self.ac = ac
        self.attacks = attacks

    def __repr__(self):
        return f"{self.__class__.__name__}(name={self.name}, hp={self.hp}, ac={self.ac}, attacks={self.attacks})"


monster1 = yaml.load("""
--- !Monster
name: Cave spider
hp: [2,6]
ac: 16
attacks: [BITE, HURT]
""")
print(monster1, type(monster1))

monster2 = Monster(name='Cave lizard', hp=[3, 6], ac=16, attacks=['BITE', 'HURT'])
print(yaml.dump(monster2))
```
运行结果：
```python
Monster(name=Cave spider, hp=[2, 6], ac=16, attacks=['BITE', 'HURT']) <class '__main__.Monster'>
!Monster
ac: 16
attacks: [BITE, HURT]
hp: [3, 6]
name: Cave lizard
```
这里面调用统一的 `yaml.load()`，就能把任意一个 yaml 序列载入成一个 Python Object；而调用统一的 `yaml.dump()`，就能把一个 YAMLObject 子类序列化。<br />对于 `load()` 和 `dump()` 的使用者来说，他们完全不需要提前知道任何类型信息，这让超动态配置编程成了可能。比方说，在一个智能语音助手的大型项目中，有 1 万个语音对话场景，每一个场景都是不同团队开发的。作为智能语音助手的核心团队成员，不可能去了解每个子场景的实现细节。<br />在动态配置实验不同场景时，经常是今天要实验场景 A 和 B 的配置，明天实验 B 和 C 的配置，光配置文件就有几万行量级，工作量不可谓不小。而应用这样的动态配置理念，就可以让引擎根据配置文件，动态加载所需要的 Python 类。<br />对于 YAML 的使用者也很方便，只要简单地继承 `yaml.YAMLObject`，就能让 Python Object 具有序列化和逆序列化能力。<br />据说即使是在大厂 Google 的 Python 开发者，发现能深入解释 YAML 这种设计模式优点的人，大概只有 10%。而能知道类似 YAML 的这种动态序列化 / 逆序列化功能正是用 metaclass 实现的人，可能只有 1% 了。而能够将YAML 怎样用 metaclass 实现动态序列化 / 逆序列化功能讲出一二的可能只有 0.1%了。<br />对于YAMLObject 的 `load()`和`dump()`功能，简单来说，需要一个全局的注册器，让 YAML 知道，序列化文本中的 `!Monster` 需要载入成 Monster 这个 Python 类型，Monster 这个 Python 类型需要被序列化为`!Monster` 标签开头的字符串。<br />一个很自然的想法就是，那建立一个全局变量叫 registry，把所有需要逆序列化的 YAMLObject，都注册进去。比如下面这样：
```python
registry = {}
 
def add_constructor(target_class):
    registry[target_class.yaml_tag] = target_class
```
然后，在 Monster 类定义后面加上下面这行代码：
```python
add_constructor(Monster)
```
这样的缺点很明显，对于 YAML 的使用者来说，每一个 YAML 的可逆序列化的类 Foo 定义后，都需要加上一句话`add_constructor(Foo)`。这无疑给开发者增加了麻烦，也更容易出错，毕竟开发者很容易忘了这一点。<br />更优雅的实现方式自然是通过metaclass 解决了这个问题，YAML 的源码正是这样实现的：
```python
class YAMLObjectMetaclass(type):
    def __init__(cls, name, bases, kwds):
        super(YAMLObjectMetaclass, cls).__init__(name, bases, kwds)
        if 'yaml_tag' in kwds and kwds['yaml_tag'] is not None:
            cls.yaml_loader.add_constructor(cls.yaml_tag, cls.from_yaml)
            cls.yaml_dumper.add_representer(cls, cls.to_yaml)
    ## 省略其余定义
 
class YAMLObject(metaclass=YAMLObjectMetaclass):
    yaml_loader = Loader
    yaml_dumper = Dumper
    ## 省略其余定义
```
可以看到，YAMLObject 把 metaclass 声明成了 YAMLObjectMetaclass，YAMLObjectMetaclass则会改变YAMLObject类和其子类的定义，就是下面这行代码将YAMLObject 的子类加入到了yaml的两个全局注册表中：
```python
cls.yaml_loader.add_constructor(cls.yaml_tag, cls.from_yaml)
cls.yaml_dumper.add_representer(cls, cls.to_yaml)
```
YAML 应用 metaclass，拦截了所有 YAMLObject 子类的定义。也就是说，在定义任何 YAMLObject 子类时，Python 会强行插入运行上面这段代码，把之前想要的`add_constructor(Foo)`和`add_representer(Foo)`给自动加上。所以 YAML 的使用者，无需自己去手写`add_constructor(Foo)`和`add_representer(Foo)`。
<a name="elkyy"></a>
## 总结
这次分享主要是简单的浅析了 metaclass 的实现机制。通过实现一个orm框架并解读 YAML 的源码，相信已经对metaclass 有了不错的理解。<br />metaclass 是 Python 黑魔法级别的语言特性，它可以改变类创建时的行为，这种强大的功能使用起来务必小心。
