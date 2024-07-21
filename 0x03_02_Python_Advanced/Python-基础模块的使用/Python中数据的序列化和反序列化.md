Python 序列化 反序列化 JSON模块 pickle模块
<a name="MUdg4"></a>
## JSON
JSON(JavaScript Object Notation, JS 对象简谱) 是一种轻量级的数据交换格式。<br />它基于 ECMAScript (欧洲计算机协会制定的js规范)的一个子集，采用完全独立于编程语言的文本格式来存储和表示数据。简洁和清晰的层次结构使得 JSON 成为理想的数据交换语言。易于人阅读和编写，同时也易于机器解析和生成，并有效地提升网络传输效率。
<a name="MC5MI"></a>
### 「JS对象」
```javascript
var student = {
    name: 'Fcant',
    age: 18,
    feature : ['高', '富', '帅']
}
```
<a name="h6lDr"></a>
### 「JSON字符串」
```python
{
    "name": "Fcant",
    "age": 18,
    "feature": ['高', '富', '帅']
}
```
<a name="gDAbm"></a>
### 「Python字典」
```python
{
    'name': 'Fcant',
    'age': 18
    'feature' : ['高', '富', '帅']
}
```
:::tips
注意点：Json字符串必须用双引号（即：""）来包括, 值可以是字符串、数字、true、false、null、列表，或字典。Python字典可以用单引号，也可以用双引号。
:::
<a name="I8d4c"></a>
## Python中的序列化和反序列化
通过文件操作，可以将字符串写入到一个本地文件。但是，如果是一个对象(例如列表、字典、元组等)，就无法直接写入到一个文件里，需要对这个对象进行序列化，然后才能写入到文件里。

- 序列化：将数据从内存持久化保存到硬盘的过程。
- 反序列化：将数据从硬盘加载到内存的过程。

Python 里存入数据只支持存入字符串和二进制。

- json:将Python里的数据(str/list/tuple/dict)等转换成为对应的json。
- pickle:将Python里任意的对象转换成为二进制。

Python中提供了JSON和pickle两个模块用来实现数据的序列化和反序列化。
<a name="rDsci"></a>
## JSON模块
JSON(JavaScriptObjectNotation, JS对象简谱)是一种轻量级的数据交换格式，它基于 ECMAScript 的一个子集，采用完全独立于编程语言的文本格式来存储和表示数据。JSON的本质是字符串，区别在于json里要是用双引号表示字符串。<br />json模块也提供了四个常用的方法：`dumps`、`dump`、`loads`、`load`，用于字符串 和 Python数据类型间进行转换。

| APi | 描述 |
| --- | --- |
| `json.dumps(obj)` | 将Python数据转化为json |
| `json.loads(s)` | 将json数据转换为Python的数据 |
| `json.dump(obj, fp)` | 转换为json并保存到文件中 |
| `json.load(fp)` | 从文件中读取json，并转化为Python数据 |

<a name="r2xSd"></a>
### API使用示例
```python
import json
my_dict = {'a':'1','b':'2','c':'3','d':'4'}
print(type(my_dict))
a = json.dumps(my_dict)
print(a)
print(type(a))
b=json.loads(a)
print(b)
print(type(b))

# 输出如下
<class 'dict'>
{"a": "1", "b": "2", "c": "3", "d": "4"}
<class 'str'> #json的字符串
{'a': '1', 'b': '2', 'c': '3', 'd': '4'}
<class 'dict'>

——————————
>>> import json
>>> print (json.dumps('中国'))
"\u4e2d\u56fd"
>>> # 这里有一个ensure_ascii参数，表示是否用ascii解析
>>> print(json.dumps('中国', ensure_ascii=False))
"中国"
——————————
#json.dump() 和 json.load() 来编码和解码JSON数据,用于处理文件。

import json
my_dict = {'a':'1','b':'2','c':'3','d':'4'}
json.dump(my_dict,open('a.txt','w'))
print(json.load(open('a.txt','r')))

# 输出如下
会生成一个"a.txt"文件
{'a': '1', 'b': '2', 'c': '3', 'd': '4'}
```
<a name="Pm6pM"></a>
### 使用`JSON`实现序列化
<a name="aANOe"></a>
#### `dumps`方法的作用是把对象转换成为字符串，它本身不具备将数据写入到文件的功能。
```python
import json
file = open('names.txt', 'w')
names = ['zhangsan', 'lisi', 'wangwu', 'jerry', 'henry', 'merry', 'chris']
# file.write(names)  出错，不能直接将列表写入到文件里

# 可以调用 json的dumps方法，传入一个对象参数
result = json.dumps(names)

# dumps 方法得到的结果是一个字符串
print(type(result))  # <class 'str'>

# 可以将字符串写入到文件里
file.write(result)

file.close()
```
<a name="AeJQL"></a>
#### `dump`方法可以在将对象转换成为字符串的同时，指定一个文件对象，把转换后的字符串写入到这个文件里。
```python
import json

file = open('names.txt', 'w')
names = ['zhangsan', 'lisi', 'wangwu', 'jerry', 'henry', 'merry', 'chris']

# dump方法可以接收一个文件参数，在将对象转换成为字符串的同时写入到文件里
json.dump(names, file)
file.close()
```
<a name="wDWun"></a>
### 使用JSON实现反序列化
<a name="edNhS"></a>
#### `loads`方法需要一个字符串参数，用来将一个字符串加载成为Python对象。
```python
import json

# 调用loads方法，传入一个字符串，可以将这个字符串加载成为Python对象
result = json.loads('["zhangsan", "lisi", "wangwu", "jerry", "henry", "merry", "chris"]')
print(type(result))  # <class 'list'>
```
<a name="uDKkI"></a>
#### `load`方法可以传入一个文件对象，用来将一个文件对象里的数据加载成为Python对象。
```python
import json
# 以可读方式打开一个文件
file = open('names.txt', 'r')
# 调用load方法，将文件里的内容加载成为一个Python对象
result = json.load(file)
print(result)
file.close()
```
<a name="nRd3b"></a>
## pickle模块
和json模块类似，pickle模块也有`dump`和`dumps`方法可以对数据进行序列化，同时也有`load`和`loads`方法进行反序列化。区别在于，`json`模块是将对象转换成为字符串，而pickle模块是将对象转换成为二进制。<br />pickle模块里方法的使用和`json`里方法的使用大致相同，需要注意的是，pickle是将对象转换成为二进制，所以，如果想要把内容写入到文件里，这个文件必须要以二进制的形式打开。
<a name="QTpyH"></a>
### 使用pickle模块实现序列化
序列化的方法为 `pickle.dump()`和`pickle.dumps()`。
<a name="U2XJy"></a>
#### `dumps`方法将Python数据转换成为二进制
`pickle.dumps()`方法的参数如下：<br />`pickle.dumps(obj, protocol=None,*,fix_imports=True)`<br />`pickle.dumps()`方法跟 `pickle.dump()`方法的区别在于，`pickle.dumps()`方法不需要写入文件中，它是直接返回一个序列化的bytes对象。
```python
import pickle
names = ['张三', '李四', '杰克', '亨利']
b_names = pickle.dumps(names)
# print(b_names)
file = open('names.txt', 'wb')
file.write(b_names)  # 写入的是二进制，不是存文本
file.close()
```
下面具体一个具体示例。
```python
>>> import pickle
>>> data = [{'a': 'A', 'b': 2, 'c': 2.22}]
>>> # 使用 pickle.dumps() 可以将一个对象转换为二进制字符串（dump string）：
>>> data_string = pickle.dumps(data)
>>> data
[{'a': 'A', 'b': 2, 'c': 2.22}]
>>>< data_string
b'\x80\x03]q\x00}q\x01(X\x01\x00\x00\x00aq\x02X\x01\x00\x00\x00Aq\x03X\x01\x00\x00\x00bq\x04K\x02X\x01\x00\x00\x00cq\x05G@\x01\xc2\x8f\\(\xf5\xc3ua.'
```
`dumps` 可以接受一个可省略的 protocol 参数（默认为 0），`protocol`参数不同，表示进行的编码协议不同，得到的`data_string`也不同。<br />如果 `protocol` 参数指定为负数，那么将调用当前的最高级的编码协议进行编码
```python
>>> data_string_1 = pickle.dumps(data, 1)
>>> data_string_1
b']q\x00}q\x01(X\x01\x00\x00\x00aq\x02X\x01\x00\x00\x00Aq\x03X\x01\x00\x00\x00bq\x04K\x02X\x01\x00\x00\x00cq\x05G@\x01\xc2\x8f\\(\xf5\xc3ua.'
>>> data_string__1 = pickle.dumps(data, -1)
>>> data_string__1
b'\x80\x04\x95#\x00\x00\x00\x00\x00\x00\x00]\x94}\x94(\x8c\x01a\x94\x8c\x01A\x94\x8c\x01b\x94K\x02\x8c\x01c\x94G@\x01\xc2\x8f\\(\xf5\xc3ua.'
```
<a name="OoeLq"></a>
#### `dump`方法将Python数据转换成为二进制，同时保存到指定文件
dump该方法的相关参数如下：<br />`pickle.dump(obj, file, protocol=None,*,fix_imports=True)`<br />该方法实现的是将序列化后的对象obj以二进制形式写入文件file中，进行保存。
```python
import pickle
names = ['张三', '李四', '杰克', '亨利']
file2 = open('names.txt', 'wb')
pickle.dump(names, file2)
file2.close()
```
如果需要保存为data.pkl，可以使用dump方法，具体代码如下。
```python
import pickle
with open('data.pkl', 'wb') as f:
    pickle.dump(data, f)
```
<a name="Y7otx"></a>
### 使用pickle模块实现反序列化
虽然 pickle 编码的字符串并不一定可读，但是可以用`pickle.loads()`来从这个字符串中恢复原对象中的内容（load string）。也就是dumps和loads有相反的作用。
<a name="pBz7w"></a>
#### `loads`方法，将二进制加载成为Python数据
`pickle.loads()`方法的参数如下：<br />`pickle.loads(bytesobject, *,fiximports=True, encoding="ASCII". errors="strict")`
```python
>>> # pickle.loads() 来从这个字符串中恢复原对象中的内容（load string）：
>>> pickle.loads(data_string)
[{'a': 'A', 'b': 2, 'c': 2.22}]
```
如果指定了`protocol`参数，需要这些格式中恢复对象时，不需要指定所用的协议，`pickle.loads()` 会自动识别：
```python
>>> pickle.loads(data_string_1)
[{'a': 'A', 'b': 2, 'c': 2.22}]
>>> pickle.loads(data_string__1)
[{'a': 'A', 'b': 2, 'c': 2.22}]
```
```python
import pickle
file1 = open('names.txt', 'rb')
x = file1.read()
y = pickle.loads(x)
print(y)
file1.close()
```
<a name="EAOFw"></a>
#### `load`方法，读取文件，并将文件的二进制内容加载成为Python数据
反序列化的方法还有`pickle.load()`，该方法的相关参数如下：<br />`pickle.load(file, *,fix_imports=True, encoding="ASCII". errors="strict")`
```python
import pickle
file3 = open('names.txt', 'rb')
z = pickle.load(file3)
print(z)
```
如果使用load，需要读取data.pkl文件。
```python
pkl_file = open('data.pkl', 'rb')
print(pickle.load(pkl_file))
```
:::info
`pickle.loads()`方法跟 `pickle.load()`方法的区别在于， `pickle.loads()`方法是直接从`bytes`对象中读取序列化的信息，而非从文件中读取。
:::
<a name="5fef6c83"></a>
## JSON与pickle区别
<a name="xc9uY"></a>
### json模块

- 将对象转换成为字符串，不管是在哪种操作系统，哪种编程语言里，字符串都是可识别的。
- json就是用来在不同平台间传递数据的。
- 并不是所有的对象都可以直接转换成为一个字符串，下标列出了Python对象与json字符串的对应关系。
| Python | JSON |
| --- | --- |
| dict | object |
| list,tuple | array |
| str | string |
| int,float | number |
| True | true |
| False | false |
| None | null |

- 如果是一个自定义对象，默认无法装换成为json字符串，需要手动指定JSONEncoder。
- 如果是将一个json串重新转换成为对象，这个对象里的方法就无法使用了。
```python
import json
class MyEncode(json.JSONEncoder):
    def default(self, o):
        # return {"name":o.name,"age":o.age}
        return o.__dict__

class Person(object):
    def __init__(self, name, age):
        self.name = name
        self.age = age

      def eat(self):
          print(self.name+'正在吃东西')

p1 = Person('zhangsan', 18)

# 自定义对象想要转换成为json字符串，需要给这个自定义对象指定JSONEncoder
result = json.dumps(p1, cls=MyEncode)
print(result)  # {"name": "zhangsan", "age": 18}

# 调用loads方法将对象加载成为一个对象以后，得到的结果是一个字典
p = json.loads(result)
print(type(p))
```
<a name="KHKwM"></a>
### pickle模块

- pickle序列化是将对象按照一定的规则转换成为二进制保存，它不能跨平台传递数据。
- pickle的序列化会将对象的所有数据都保存。
