Python
<a name="XJ6PP"></a>
## 1、类型相关
69个内置函数中，与类型相关的指，把入参包装为某种类型，这样的内置函数包括：
```python
bool()  #d布尔型
int()  #d整形
str()  #d字符型
tuple() #d元包型
dict() #d字典型
list() #d列表型
zip() #可迭代对象聚合，(,)
complex() #d复数型
float() #d浮点型
bytes() #d字节型数组
bytearray() #d字数数组
range() #d不可更改的序列
object() #d无属性的根类
set() # 集合类型
frozenset() # 冻结集合类型，不允许修改
slice() # 返回一个slice对象，其中start, stop, step等都是只读的
```
比如`bool(x)`，将入参`x`封装为`Boolean`类型，返回值为`True`或`False`
```python
bool(10)
>>> True
bool(0)
>>> False
bool(\'false\')
>>> True
bool(0.0)
>>> False
```
<a name="jwNxH"></a>
## 2、数理统计相关
有的内置函数可以完成简单的数理统计工作，这样的内置函数包括：
```python
abs() #d绝对值
min() #d最小
max() #d最大
sum() #d求和
pow() #d求次幂
all() #d所有元素为true则为true
any() #d至少一个元素为true则为true
divmod() #d(商，余数)
round() #四舍五入
len() #参数元素个数
```
`any()`函数代码等价于以下5行代码：
```python
def any(iterable):
    for element in iterable:
        if element:
            return True
    return False
```
`divmod`函数：
```python
divmod(10,8)
>>> (1,2)
```
<a name="SECNY"></a>
## 3、进制转换
有些内置函数可以帮助实现进制转换，比如：
```python
chr() #unicode编码
ord() #chr()反操作
bin() #转化为ob开头的二进制字符
hex() #转化为ox开头的十六进制字符
ascii() #可打印表示对象，类似于 repr() 
oct() # 转化为0o开头的八进制字符
```
<a name="aqrDS"></a>
## 4、面向对象相关
Python提供与对象属性相关的操作函数，它们为满足Python属性的动态调整提供了可能。
```python
setattr(object, name, value) #为对象设置属性
delattr(object, name) # 删除命名的属性
getattr(object,name) #获取属性的取值，如果对象无此属性，会抛异常
getattr(object,name, 123) #即便无此属性，也不会抛异常，会返回123
hasattr(object,name) # 判断name属性是否属于object
isinstance(object, classinfo) #判断object是classinfo的实例吗
issubclass(class, classinfo) # 判断class是否为classinfo的子类
super() #调用父类， 方法
property() #特性相关，@property标记为属性
type() #返回实例的类型
vars() # 返回对象的信息等
classmethod() # 转化方法为类方法
staticmethod() #方法是静态方法
```
比如，
```python
setattr(x,\'footbar\',12) #等价于 x.footbar=12
issubclass(list,object) 
>>> True
issubclass(object,list)
>>> False
```
<a name="QI4Aq"></a>
## 5、迭代器相关
`next`，`reversed`，`iter()`，`enumerate()`这些都是与迭代相关的函数，比如以下，就是返回一个逆向迭代器：
```python
rev = reversed([1,-2,4,0])
for i in rev:
  print(i)
 >>> 0
 4
 -2
 1
```
iter函数使用例子
```python
iter(\'abc\')
>>> <str_iterator at 0x8b9fba8>
for i in iter(\'abc\'):
  print(i)
>>> a
b
c
```
<a name="s6xiD"></a>
## 6、`map`函数
`map`函数的原型为：`map(function, iterable, ...)` ，返回一个迭代器，在每一个可迭代对象的元素上应用function。<br />`map`应用举例：
```python
def f(x):
    return x**2
mymap = map(f, [1,2,3]) # 返回一个迭代器

for i in mymap:
    print(i)
    
>>> 1
    4
    9
```
<a name="FVD20"></a>
## 7、排序相关
sorted返回一个排序好的列表，比如：
```python
li = [1,-2,4,0]
sorted(li)
>>> [-2,0,1,4]
```
<a name="QWJlN"></a>
## 8、其他
`compile`函数与源码编译相关；<br />`memoryview`函数与内存视图相关；<br />help查看帮助；<br />`dir`查看对象的方法和属性；更多见下表：
```python
hash() # 返回对象的哈希码
id() #返回一个对象的标识
input()  #与标准输出相关
breakpoint() #调试相关
exec() #动态执行Python代码
callable() #判断对象是否可调用
format() #对象格式化
```
`format`是一个比较常用的，用于格式化输出的函数，详细的格式化参数标准形式，参考文档：<br />[https://docs.python.org/3/library/string.html#formatspec](https://docs.python.org/3/library/string.html#formatspec)<br />参考官网：<br />[https://docs.python.org/3/library/functions.html](https://docs.python.org/3/library/functions.html)
