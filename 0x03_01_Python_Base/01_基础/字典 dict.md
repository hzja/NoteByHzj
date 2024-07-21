# 字典



字典是另一种可变容器模型，且可存储任意类型对象。

字典的每个键值 **key=>value** 对用冒号 **:** 分割，每个对之间用逗号(**,**)分割，整个字典包括在花括号 **{}** 中 ,格式如下所示：

```
d = {key1 : value1, key2 : value2, key3 : value3 }
```

**注意：****dict** 作为 Python 的关键字和内置函数，变量名不建议命名为 **dict**。

![py-dict-3](.\图片\py-dict-3.png)

键必须是唯一的，但值则不必。

值可以取任何数据类型，但键必须是不可变的，如字符串，数字。

一个简单的字典实例：

```
tinydict = {'name': 'runoob', 'likes': 123, 'url': 'www.runoob.com'}
```

![py-dict-2](.\图片\py-dict-2.png)

也可如此创建字典：

```python
tinydict1 = { 'abc': 456 }
tinydict2 = { 'abc': 123, 98.6: 37 }
```



## 创建字典

用大括号 **{ }** 创建空字典：

~~~ python 
# 使用大括号 {} 来创建空字典
emptyDict = {}
# 打印字典
print(emptyDict)
# 查看字典的数量
print("Length:", len(emptyDict))
# 查看类型
print(type(emptyDict))
~~~

以上实例输出结果：

```python
{}
Length: 0
<class 'dict'>
```



用内建函数 **dict()** 创建字典：

~~~ python
emptyDict = dict()
# 打印字典
print(emptyDict)
# 查看字典的数量
print("Length:",len(emptyDict))
# 查看类型
print(type(emptyDict))
~~~

以上实例输出结果：

```python
{}
Length: 0
<class 'dict'>
```



创建字典

~~~ python 
my_dict = {'name': 'Obama', 'age': 48, 'gender': '女',}
# 访问字典中的元素
my_dict['name']
# 只能通过下标去访问元素, 不支持索引
my_dict[0]
~~~



## 查询和遍历

> 查询 :
>
> 字典[键]  根据键取值，键值对不存在会报错
> 字典.get(键)  根据键取值，键值对不存在不会报错
> 字典.keys()  可进行遍历，获取所有键
> 字典.values()    可进行遍历，获取所有值
> 字典.items() 可进行遍历，获取所有(键，值)
> 遍历 for key in 字典  取出元组中的每个元素的 key

~~~ python
my_dict = {'name': 'Obama', 'age': 48, 'gender': '女'}
# 使用中括号方式访问字典中的元素，如果key不存在则报错

print(my_dict.get('name', 'abc'))  #前面的不存在才会查看后面元素

print('打印所有的key:', my_dict.keys())
print('打印所有的value:', my_dict.values())
print('打印所有的key-value:', my_dict.items())

# 遍历字典
for key, value in my_dict.items():
    print(key, value)  
for item in my_dict.items():
    print(item[0], item[1])
~~~



把相应键放入方括号中，如下:

~~~ python
#!/usr/bin/python3
tinydict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'}
print ("tinydict['Name']: ", tinydict['Name'])
print ("tinydict['Age']: ", tinydict['Age'])
~~~

以上实例输出：

```python
tinydict['Name']:  Runoob
tinydict['Age']:  7
```



如果用字典里没有的键访问数据会输出错误：

~~~ python
#!/usr/bin/python3
tinydict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'}
print ("tinydict['Alice']: ", tinydict['Alice'])
~~~

以上实例输出：

```python
Traceback (most recent call last):
  File "test.py", line 5, in <module>
    print ("tinydict['Alice']: ", tinydict['Alice'])
KeyError: 'Alice'
```



## 插入和删除

~~~ python
my_dict = {'name': 'Obama', 'age': 48, 'gender': '女'}
my_dict['name'] = 'Trump'
# 如果key存在会覆盖
~~~

~~~ python
# 删除操作
my_dict.pop('gender')

# 清空字典
my_dict.clear()
~~~



### 删除字典元素

能删单一元素也能清空字典，清空只需一项操作

显式删除一字典用del命令，如下实例：

~~~ python
#!/usr/bin/python3
 
tinydict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'}
 
del tinydict['Name'] # 删除键 'Name'
tinydict.clear()     # 清空字典
del tinydict         # 删除字典
 
print ("tinydict['Age']: ", tinydict['Age'])
print ("tinydict['School']: ", tinydict['School'])
~~~

但这会引发一异常，因为执行 del 操作后字典不再存在：

```
Traceback (most recent call last):
  File "/runoob-test/test.py", line 9, in <module>
    print ("tinydict['Age']: ", tinydict['Age'])
NameError: name 'tinydict' is not defined
```

**注：**del() 方法后面也会讨论



## 修改字典

向字典添加新内容方法是增加新的键/值对，修改或删除已有键/值对，如下实例:

~~~ python
#!/usr/bin/python3
tinydict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'}
tinydict['Age'] = 8               # 更新 Age
tinydict['School'] = "菜鸟教程"  # 添加信息

print ("tinydict['Age']: ", tinydict['Age'])
print ("tinydict['School']: ", tinydict['School'])
~~~

以上实例输出结果：

```python
tinydict['Age']:  8
tinydict['School']:  菜鸟教程
```



### 修改字典元素

> 修改  
>
> 字典[键] = 数据 键不存在，会添加键值对；键存在，会修改键值对的值
> 字典.setdefault(键，数据)  
>
> 字典.update(字典2)

~~~ python
# 键值对不存在，则添加键值对；存在则不做处理
my_dict.setdefault('age', 18)

# 取出字典2的键值对，键值对不存在，添加键值对；存在则修改值
my_dict2 = {'name': 'Obama', 'age': 10, 'gender': '女', 'attack': 100}
my_dict.update(my_dict2)
~~~



## 字典内置函数&方法

Python字典包含以下内置函数

| 序号 | 函数及描述                                                   | 实例                                                         |
| :--- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 1    | len(dict) 计算字典元素个数，即键的总数。                     | `>>> tinydict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'} >>> len(tinydict) 3` |
| 2    | str(dict) 输出字典，可以打印的字符串表示。                   | `>>> tinydict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'} >>> str(tinydict) "{'Name': 'Runoob', 'Class': 'First', 'Age': 7}"` |
| 3    | type(variable) 返回输入的变量类型，如果变量是字典就返回字典类型。 | `>>> tinydict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'} >>> type(tinydict) <class 'dict'>` |



Python字典包含以下内置方法

| 序号 | 函数及描述                                                   |
| :--- | :----------------------------------------------------------- |
| 1    | [dict.clear()](https://www.runoob.com/python3/python3-att-dictionary-clear.html) 删除字典内所有元素 |
| 2    | [dict.copy()](https://www.runoob.com/python3/python3-att-dictionary-copy.html) 返回一个字典的浅复制 |
| 3    | [dict.fromkeys()](https://www.runoob.com/python3/python3-att-dictionary-fromkeys.html) 创建一个新字典，以序列seq中元素做字典的键，val为字典所有键对应的初始值 |
| 4    | [dict.get(key, default=None)](https://www.runoob.com/python3/python3-att-dictionary-get.html) 返回指定键的值，如果键不在字典中返回 default 设置的默认值 |
| 5    | [key in dict](https://www.runoob.com/python3/python3-att-dictionary-in.html) 如果键在字典dict里返回true，否则返回false |
| 6    | [dict.items()](https://www.runoob.com/python3/python3-att-dictionary-items.html) 以列表返回一个视图对象 |
| 7    | [dict.keys()](https://www.runoob.com/python3/python3-att-dictionary-keys.html) 返回一个视图对象 |
| 8    | [dict.setdefault(key, default=None)](https://www.runoob.com/python3/python3-att-dictionary-setdefault.html) 和get()类似, 但如果键不存在于字典中，将会添加键并将值设为default |
| 9    | [dict.update(dict2)](https://www.runoob.com/python3/python3-att-dictionary-update.html) 把字典dict2的键/值对更新到dict里 |
| 10   | [dict.values()](https://www.runoob.com/python3/python3-att-dictionary-values.html) 返回一个视图对象 |
| 11   | [pop(key[,default])](https://www.runoob.com/python3/python3-att-dictionary-pop.html) 删除字典 key（键）所对应的值，返回被删除的值。 |
| 12   | [popitem()](https://www.runoob.com/python3/python3-att-dictionary-popitem.html) 返回并删除字典中的最后一对键和值。 |



## 字典键的特性

字典值可是任何python 对象，既可是标准对象也可是用户定义的，但键不行；

两个重要的点需记住：

1）不允许同一键出现两次，创建时如同一个键被赋值两次，后一个值被记住，如下实例：

~~~ python
#!/usr/bin/python3
tinydict = {'Name': 'Runoob', 'Age': 7, 'Name': '小菜鸟'}
print ("tinydict['Name']: ", tinydict['Name'])
~~~

以上实例输出结果：

```python
tinydict['Name']:  小菜鸟
```



2）键须不可变所以可用数字或字符串或元组充当而用列表就不行，如下实例：

~~~ python
#!/usr/bin/python3
tinydict = {['Name']: 'Runoob', 'Age': 7}
print ("tinydict['Name']: ", tinydict['Name'])
~~~

以上实例输出结果：

```python
Traceback (most recent call last):
  File "test.py", line 3, in <module>
    tinydict = {['Name']: 'Runoob', 'Age': 7}
TypeError: unhashable type: 'list'
```



## defaultdict

当用普通字典时用法一般是dict={}，添加元素只要dict[element] =value；调用时也dict[element] = xxx；但前提是element如不在字典里就报错如：

![9539185-01591b77c5c8ef05](.\图片\9539185-01591b77c5c8ef05.webp)

这时defaultdict排上用场，defaultdict作用在于当字典里key不存在但被查找时返回的不是keyError而是一默认值；

这个factory_function可是list、set、str等，作用是当key不存在时返回工厂函数默认值，如list对应[ ]，str对应的是空字符串，set对应set( )，int对应0，如下举例：

~~~ python
from collections import defaultdict

dict1 = defaultdict(int)
dict2 = defaultdict(set)
dict3 = defaultdict(str)
dict4 = defaultdict(list)
dict1[2] = 'two'
print("dict1[1]: %s" % dict1[1])
print("dict1[1]: %s" % dict2[1])
print("dict1[1]: %s" % dict3[1])
print("dict1[1]: %s" % dict4[1])
~~~

输出：

~~~python
dict1[1]: 0
dict1[1]: set()
dict1[1]: 
dict1[1]: []
~~~