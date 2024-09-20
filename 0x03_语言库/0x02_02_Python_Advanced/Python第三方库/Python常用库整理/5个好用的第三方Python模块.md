Pyhton
<a name="laWaj"></a>
## **xmltodict**
首先给大家介绍的第三方模块叫做xmltodict，从名字上就可以非常直观地看出，该模块的作用在于将xml格式的数据转换成字典，要是没有安装该模块的童鞋，可以通过pip命令来进行安装
```bash
pip install xmltodict
```
假设有如下所示的xml格式的数据
```xml
<?xml version='1.0' encoding='utf-8'?>
<mydocument has="an attribute">
  <and>
    <many>elements</many>
    <many>more elements</many>
  </and>
  <plus a="complex" b="hello">
    element as well
  </plus>
</mydocument>
```
尝试来读取当中的数据，用到xmltodict模块，代码如下
```python
xml_result = open('testdb.xml', 'r')
xml_dict = xmltodict.parse(xml_result.read())
print(type(xml_dict))
print(xml_dict)
```
output
```
<class 'dict'>
{'mydocument': {'@has': 'an attribute', 'and': {'many': ['elements', 'more elements']}, 'plus': {'@a': 'complex', '@b': 'hello', '#text': 'element as well'}}}
```
也可以尝试来读取当中的一些数据，代码如下
```python
xml_dict.get("mydocument").get("and")
```
output
```
{'many': ['elements', 'more elements']}
```
当然还可以转换回去，转换成xml格式的数据，这里需要用到的是`unparse()`方法，代码如下
```python
dict_xml = xmltodict.unparse(xml_dict, pretty=True)
```
<a name="eSPUP"></a>
## **UltraJSON**
ultrajson的底层是用C来编写的，因此作为一个超高速的json编码器和解码器，可以用作是其他大多数json解析器的替换，先用pip命令来将其安装
```bash
pip install ujson
```
来尝试使用该模块来解析一下如下的数据，代码如下
```python
import ujson
ujson.dumps([{"key1": "value1"}, {"key2": "value2"}])
```
output
```
'[{"key1":"value1"},{"key2":"value2"}]'
```
使用`dumps()`方法输出的数据就是以字符串的形式来展现的，与此同时呢还有`loads()`方法针对字符串格式的数据进行转换
```python
ujson_result = ujson.loads("""[{"key1": "value1"}, {"key2": "value2"}]""")
```
要是想要获取列表当中第一个元素的值，就可以这么来做了
```python
ujson_result[0].get("key1")
```
output
```
value1
```
<a name="rsQfH"></a>
## **arrow**
Arrow提供了一个友好而且非常易懂的方法，用于创建时间、计算时间、格式化时间，还可以对时间做转化、提取、兼容python datetime类型。根据其文档的描述，Arrow模块旨在帮助使用者用更少的代码来处理日期和时间<br />例如想要知道当前的时间，代码如下
```python
import arrow
now = arrow.now()
print(now)
```
output
```
2022-12-05T05:04:19.353774+08:00
```
当然可以用`format()`方法来格式化日期和时间，具体代码如下
```python
import arrow
now = arrow.now()
year = now.format('YYYY')
print("Year: {0}".format(year))
date = now.format('YYYY-MM-DD')
print("Date: {0}".format(date))
date_time = now.format('YYYY-MM-DD HH:mm:ss')
print("Date and time: {0}".format(date_time))
date_time_zone = now.format('YYYY-MM-DD HH:mm:ss ZZ')
print("Date and time and zone: {0}".format(date_time_zone))
```
output
```
Year: 2022
Date: 2022-12-05
Date and time: 2022-12-05 10:41:01
Date and time and zone: 2022-12-05 10:41:01 +08:00
```
与此同时，还能查看一下当下其他时区中的时间，代码如下
```python
utc = arrow.now()
print(utc.to('US/Pacific').format('HH:mm:ss'))
print(utc.to('Europe/Paris').format('HH:mm:ss'))
print(utc.to('Europe/Moscow').format('HH:mm:ss'))
```
output
```
18:48:14
03:48:14
05:48:14
```
<a name="KA81k"></a>
## **fire**
目前Python的命令行模块有argparse(Python的标准库)、click等，但是这些库在使用上都比较麻烦，本文介绍的Fire模块用起来十分的方便，并且可用于任何Python对象自动生成命令行接口，来看下面这个例子
```python
import fire

def hello(name="World"):
    return "Hello %s!" % name

if __name__ == '__main__':
    fire.Fire(hello)
```
可以在命令行中运行python 文件名.py，得到结果Hello World!，或者可以添加上相应的参数
```bash
python 文件名.py --name=Tom
```
output
```
Hello Tom!
```
当不知道怎么来添加这些参数的时候，就可以通过`--help`来进行查看，
```bash
python 文件名.py --help
```
output
```
INFO: Showing help with the command '文件名.py -- --help'.

NAME
    文件名.py

SYNOPSIS
    文件名.py <flags>

FLAGS
    --name=NAME
        Default: 'World'
```
可以来写一个计算器对象来做一个输入数字的平方计算，代码如下
```python
class Calculator(object):
    """A simple calculator class."""

    def square(self, number):
        return number ** 2

if __name__ == '__main__':
    fire.Fire(Calculator)
```
可以这样来尝试，代码如下
```bash
python 文件名.py square 10
```
output
```
100
```
或者是
```bash
python 文件名.py square --number=5
```
output
```
25
```
<a name="wXpca"></a>
## **tinydb**
tinydb是一个用纯Python编写的轻量级数据库，从名字上面就能够看出来它十分的轻便，它的由来就是为了降低小型Python应用程序使用数据库的难度，对于一些简单的程序而言与其是用SQL数据库，还不如就使用Tinydb，先用pip命令安装一下
```bash
pip install tinydb
```
先初始化一个DB文件，代码如下
```python
from tinydb import TinyDB
db = TinyDB('db.json')
```
同时也可以往里面添加几条数据，调用的方法是`insert()`或者是`insert_multiple()`，代码如下
```python
db.insert({'type': 'apple', 'count': 10})
db.insert({'type': 'banana', 'count': 20})

db.insert_multiple([
    {'name': 'John', 'age': 22},
    {'name': 'John', 'age': 37}])
```
要是想要查看所有的数据，调用的就是`all()`方法了，代码如下
```python
from tinydb import TinyDB
db = TinyDB('db.json')
db.all()
```
output
```
[{'type': 'apple', 'count': 10}, {'type': 'banana', 'count': 20}, {'name': 'John', 'age': 30}, {'name': 'Tom', 'age': 45}]
```
当需要搜索指定的数据时，就需要用到`Query()`方法，代码如下
```python
from tinydb import TinyDB, Query
db = TinyDB('db.json')
Fruit = Query()
db.search(Fruit.type == 'apple')
```
output
```
[{'type': 'apple', 'count': 10}]
```
或者是要更新数据的时候，就用到`update()`方法
```python
db.update({'type': 'apple', 'count': 40})
Fruit = Query()
db.search(Fruit.type == 'apple')
```
output
```
[{'type': 'apple', 'count': 40
```
而要是清空整个数据库的话，调用的则是`truncate()`方法，代码如下
```python
from tinydb import TinyDB
db = TinyDB('db.json')
db.truncate()
db.all()
```
output
```
[]
```
删除某条数据的话，调用的是`remove()`方法，
```python
db.remove(Fruit.count < 15)
db.all()
```
output
```python
[{'type': 'banana', 'count': 20}]
```
