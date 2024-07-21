Python装饰器
<a name="WIwh1"></a>
## 介绍
`@property`是Python的一种装饰器，是用来修饰方法的。
<a name="Zi3qW"></a>
## 1、作用
可以使用`@property`装饰器来创建只读属性，`@property`装饰器会将方法转换为相同名称的只读属性,可以与所定义的属性配合使用，这样可以防止属性被修改。
<a name="jKFoG"></a>
## 2、使用场景
<a name="xurai"></a>
### 2.1 修饰方法，使方法可以像属性一样访问
```python
class DataSet(object):
    @property
    def method_with_property(self): ##含有@property
        return 15
    def method_without_property(self): ##不含@property
        return 15

l = DataSet()
print(l.method_with_property) # 加了@property后，可以用调用属性的形式来调用方法,后面不需要加()。
print(l.method_without_property())  #没有加@property , 必须使用正常的调用方法的形式，即在后面加()
```
两个都输出为15。
```python
class DataSet(object):
    @property
    def method_with_property(self): ##含有@property
        return 15
l = DataSet()
print(l.method_with_property（）) # 加了@property后，可以用调用属性的形式来调用方法,后面不需要加()。
```
如果使用`property`进行修饰后，又在调用的时候，方法后面添加了`()`， 那么就会显示错误信息：
```
TypeError: 'int' object is not callable
```
也就是说添加`@property`后，这个方法就变成了一个属性，如果后面加入了`()`，那么就是当作函数来调用，而它却不是callable（可调用）的。
```python
class DataSet(object):
    def method_without_property(self): ##不含@property
        return 15
l = DataSet()
print(l.method_without_property） #没有加@property , 必须使用正常的调用方法的形式，即在后面加()
```
没有使用`property`修饰，它是一种方法，如果把括号去掉，不会报错输出的就会是方法存放的地址。
<a name="Bm5L4"></a>
### 2.2 防止属性被修改
由于Python进行属性的定义时，没办法设置私有属性，因此要通过`@property`的方法来进行设置。这样可以隐藏属性名，让用户进行使用的时候无法随意修改。
```python
class DataSet(object):
    def __init__(self):
        self._images = 1
        self._labels = 2 #定义属性的名称
    @property
    def images(self): #方法加入@property后，这个方法相当于一个属性，这个属性可以让用户进行使用，而且用户有没办法随意修改。
        return self._images 
    @property
    def labels(self):
        return self._labels
l = DataSet()
#用户进行属性调用的时候，直接调用images即可，而不用知道属性名_images，因此用户无法更改属性，从而保护了类的属性。
print(l.images) # 加了@property后，可以用调用属性的形式来调用方法,后面不需要加()。
```
