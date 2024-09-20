Python 数据库 MongoDB<br />MongoDB 是基于分布式存储，由 C++ 编写的开源的 NoSql 数据库<br />MongoDB 的内容存储类似 JSON 对象，数据结构包含 3 种<br />分别是：

- 数据库 - Databases对应关系型数据库中的数据库（Database）
- 集合 - Collection对应关系型数据库中的 Table 表（Table）
- 文档 - Document对应数据库表中的一条数据（Row Data）
<a name="HgUkl"></a>
## 1、准备
Python 操作 MongoDB，常见的两种方式是：Pymongo、Mongoengine<br />其中

- Mongoengine：面相对象，针对文档型数据库的 ORM，直接继承于 Document 文档，对文档进行增删改查
- Pymongo：通过 JSON 和 MongoDB 进行通信，将 MongoDB 的数据映射成 Python 内置的数据类型

首先，通过 pip3 命令安装依赖
```bash
# 安装依赖
# pymongo
pip3 install pymongo

# mongoengine
pip3 install mongoengine
```
下面分别对 Pymongo 和 Mongoengine 进行说明
<a name="El9ky"></a>
## 2、PyMongo
首先，创建一个数据库连接对象<br />创建数据库连接对象有二种方式，分别是：多参数、字符串拼接
```python
import pymongo

# 创建数据库连接对象的两种方式
# 方式一:多参数
self.client = pymongo.MongoClient(host='ip地址', port=27017, username="root", password="123456",
                                          authMechanism="SCRAM-SHA-1")
# 方式二：拼接
# self.client = pymongo.MongoClient('mongodb://root:123456@ip地址:27017/')
```
接着，通过数据库连接对象指定要操作的数据库和操作集合<br />比如：要操作数据库 temp 中的 students 集合
```python
# 指定要操作的数据库：temp
self.db = self.client.temp

# 指定要操作集合students
self.collection_students = self.db.students
```
接着，来实现增删改查操作
<a name="xUGSS"></a>
### 1、新增
新增包含：新增单条数据和多条数据<br />单条数据插入对应的方法是：
```python
insert_one(dict)
```
该方法的返回值类型为 InsertOneResult<br />通过 inserted_id 属性，可以获取插入数据的 _id 值
```python
temp_data = {
    "id": "1",
    "name": "xag",
    "age": 18
}

# 1、直接调用集合的insert_one()方法插入数据(插入一条数据)
result = self.collection_students.insert_one(temp_data)

# 返回值为InsertOneResult，通过inserted_id属性获取_id的值
print(result.inserted_id)
```
多条数据插入对应的方法是：
```python
insert_many([dict1,dict2...])
```
该方法的返回值类型为 `InsertManyResult`<br />通过 `inserted_ids` 属性，可以获取插入数据的 _id 属性值列表
```python
# 2、插入多条数据-insert_many()
result = self.collection_students.insert_many([temp_data, temp_data2])

# 返回值为InsertManyResult，通过inserted_ids属性获取插入数据的_id列表值
print(result.inserted_ids)
```
<a name="qzGW0"></a>
### 2、查询
使用 PyMongo 查询 MongoDB 数据库，常用方法如下：

- 通过某一个属性键值对，去查询一条记录 - `find_one()`
- 通过 ObjectId 值去查询某一条记录 - `find_one()`
- 通过某一个属性键值对，去查询多条记录 - `find()`
- 通过大于、小于、等于等条件去比较查询
- 正则匹配查询

前面 3 种查询方式，由于比较简单，直接给出实例：
```python
def manage_query(self):
    """查询数据"""
    # 1、通过某个属性键值对，去查询一条记录 find_one()
    # 返回值为字典类型
    # {'_id': ObjectId('5f5c437cfe49fa9a16664179'), 'id': '1', 'name': 'xag', 'age': 18}
    result = self.collection_students.find_one({"name": "xag"})
    print(result)

    # 2、通过ObjectId值去查询某一条记录
    result = self.collection_students.find_one({"_id": ObjectId('5f5c437cfe49fa9a16664179')})
    print(result)

    # 3.1 查询多条记录 find()
    # 返回值为一个游标(生成器)，pymongo.cursor.Cursor
    result_lists = self.collection_students.find({"name":"xag"})
    print(result_lists)
    for item in result_lists:
        print(item)
```
条件比较查询，包含：大于($gt)、大于等于($gte)、小于($lt)、小于等于($lte)、不等于($ne)、在范围内($in)、不在范围内($nin)<br />比如：查询年龄大于 18 岁的数据
```python
# 3.2 条件比较查询，包含大于($gt)、大于等于($gte)、小于($lt)、小于等于($lte)、不等于($ne)、在范围内($in)、不在范围内($nin)
# 查询年龄大于18岁的记录
result = self.collection_students.find({'age': {'$gt': 18}})
for item in result:
    print(item)
```
正则匹配查询，包含：

- `$regex`：匹配正则表达式
- `$exists`：属性是否存在
- `$type`：数据类型判断
- `$mod`：数据模操作
- `$text`：文本包含查询
- `$where`：高级条件查询

比如，查询 name 值以 "xag" 开头的数据
```python
# 正则匹配查询
results = self.collection_students.find({'name': {'$regex': '^xag.*'}})
for item in results:
    print(item)
```
关于查询更加复杂的功能可以参考：[https://docs.mongodb.com/manual/reference/operator/query/](https://docs.mongodb.com/manual/reference/operator/query/)
<a name="mrOoC"></a>
### 3、更新
更新操作包含：更新一条记录和更新多条记录<br />其中，更新一条记录对应的方法是：
```python
update_one(query,update_content)
```
参数包含：查询的条件、要修改的内容
```python
# 1、修改一条记录 update_one(query,update_data)
# 方法中有两个参数，分别是：查询条件、要修改的内容
# 查询条件
query_condition = {"name": "xag"}
# 要修改的内容
update_content = {"$set": {"name": "星安果"}}
# 使用update_one() 方法进行更新一条记录
result = self.collection_students.update_one(query_condition, update_content)
```
通过返回的结果可以获取查询匹配的记录个数及影响的记录个数
```python
# matched_count:匹配的记录个数
# modified_count：影响的记录个数
print(result.matched_count, result.modified_count)
```
更新多条记录对应的方法是：
```python
update_many(query,update_content)
方法中的参数、返回值与修改单条记录类似
# 2、修改多条记录 update_many(query,update_data)
# 查询条件
query_condition = {"name": {"$regex": "^星.*"}}
# 要修改的内容
update_content = {"$set": {"name": "xag"}}
# 将文档中name以星开头的记录都设置为xag
result = self.collection_students.update_many(query_condition, update_content)
print(result)
print(result.matched_count, result.modified_count)
```
<a name="J5K0n"></a>
### 4、删除
删除同样包含：删除查询到的第一条记录、删除查询到的所有记录<br />分别对应的方法是：`delete_one(query)`、`delete_many(query)`<br />另外，在返回结果中可以获取到真实被删除的数目
```python
def manage_remove(self):
    """
    删除操作
    :return:
    """
    # 1、删除查询到的第一条记录 delete_one()
    # result = self.collection_students.delete_one({'name': "xag2"})
    # print(result)
    # 删除的数目
    # print(result.deleted_count)

    # 2、删除多条记录 delete_many()
    result = self.collection_students.delete_many({'name': "xag"})
    print(result)
    # 删除的数目
    print(result.deleted_count)
```
<a name="RJOOG"></a>
### 5、计数和排名
常用的方法包含：

- `limit(num)`：限制返回的结果数量
- `skip(num)`：忽略 num 个元素，从 num + 1 个元素开始查看

`count_documents()`：查看集合中所有的文档数量，也可以根据条件去查询满足的文档数量<br />`sort()`：升序或者降序
```python
def manage_count_and_sort(self):
    """
    计数和排序
    :return:
    """
    # 1、限制返回的结果数量 - limit()
    # result = self.collection_students.find().limit(2)
    # for item in result:
    #     print(item)

    # 2、偏移  skip()
    # 比如：忽略前面两个元素，从第3个元素开始查看
    # result = self.collection_students.find().skip(2)
    # print([result['name'] for result in result])

    # 3.1 查询出集合中所有的文档数量 count_documents()
    # result = self.collection_students.count_documents({})
    # print(result)

    # 3.2 根据条件去查询，然后判断结果数目
    # query_regex = {'name': {'$regex': '^xag.*'}}
    # result = self.collection_students.count_documents(query_regex)
    # print(result)

    # 4、排序 sort()
    # pymongo.ASCENDING:升序，DESCENDING:降序
    result = self.collection_students.find().sort('name', pymongo.DESCENDING)
    print([result['name'] for result in result])
```
<a name="GBnS3"></a>
## 3、Mongoengine
在使用 Mongoengine 操作 MongoDB 之前，需要先定义一个 Document 的子类<br />该子类对应 MongoDB 中的文档，内部加入的静态变量（包含：类型、长度等）对应数据库文档中的数据
```python
from mongoengine import *

# Document的子类，对应文档对象
class Student(Document):
    name = StringField(required=True, max_length=500)
    age = IntField(required=True, default=18)
    create_time = DateTimeField(default=datetime.now)

    # 配置元数据
    # 指定集合为student
    meta = {'collection': 'student', 'strict': False}
```
利用 Mongoengine 内置的 `connect()` 方法，连接指定的数据库
```python
# 连接数据库temp
def __init__(self):
    # 连接数据库
    # 数据库名称：temp
    # auth方式：SCRAM-SHA-1
    result = connect('temp', host='ip地址', port=27017,
                     username='root', password='123456', authentication_source='admin',
                     authentication_mechanism="SCRAM-SHA-1")
    print(result)
```
接着，来实现增删改查操作
<a name="dAvE2"></a>
### 1、新增
使用 Mongoengine 新增一条记录到数据库非常方便<br />只需要实例化一个文档对象，调用 `save()` 方法，即可以存储一条记录到数据库当中
```python
def insert(self):
    """
    插入数据
    :return:
    """
    person = Student(name='xag2', age=20)
    person.save()
```
<a name="Zghi3"></a>
### 2、查询
常见的查询操作包含：

- 查询集合中的所有记录
- 查询第一条记录
- 通过主键 _ID，来查询数据
- 条件查询

对应的代码如下：
```python
def query(self):
    """
    普通查询
    :return:
    """
    # 1、查看集合中所有数据
    # students = Student.objects.all()
    # print([item['name'] for item in students])

    # 2、查询第一条记录
    # student = Student.objects.first()
    # print(student.name, student.age, student.create_time)

    # 3、通过主键_ID来查询数据
    result = Student.objects.filter(pk="5f5c5b34f5b0c049707a1710").first()
    print(result.name, result.age, result.create_time)

    # 4、条件查询
    # 查询年龄在18-20岁的数据
    # __gte：大于等于；__lte:小于等于
    # 默认是升序，可以加一个：-，代表逆序
    # students = Student.objects(age__gte=18, age__lte=20).order_by('name')
    students = Student.objects(age__gte=18, age__lte=20).order_by('-name')
    # for item in students:
    #     print(item.name, item.age, item.create_time)
```
值得一提的是，Mongoengine 提供了关键字 Q 来实现高级查询<br />比如：查询 name 字段值为 xag，年龄为 18 岁的数据
```python
def query_advance(self):
    """
    高级查询
    :return:
    """
    # 查看name为xag，age为18的记录的第一条
    student = Student.objects(Q(name="xag") & Q(age=18)).first()
    print(student.name, student.age, student.create_time)
```
进阶操作可以参考：[https://docs.mongoengine.org/guide/querying.html](https://docs.mongoengine.org/guide/querying.html)
<a name="lpj4S"></a>
### 3、更新
Mongoengine 提供了 `filter()` 和 `update()` 两个方法，分别用于过滤待更新的数据，指定的更新内容
```python
def update(self):
    """
    更新记录
    :return:
    """
    # 1、修改所有记录
    # 修改name为xag的年龄都减少1岁
    # 增加一岁：inc__age=1
    # 减少一岁：dec__age=1
    # Student.objects.filter(name="xag").update(dec__age=1)
    # Student.objects.filter(name="xag").update(inc__age=1)

    # name为xag，age小于18的的所有记录，更新为age=23
    # __lt：小于
    # __lte:小于等于
    # __gt:大于
    # __gte:大于等于
    # Student.objects.filter(name="xag", age__lt=18).update(age=23)
    # Student.objects.filter(age__lte=20).update(age=23)
```
如果只需要更改查询到的第一条记录，可以使用 `update_one()` 方法
```python
# 2、修改一条记录
# 年龄减少5岁
Student.objects.filter(name='xag').update_one(dec__age=5)
```
<a name="cMUkC"></a>
### 4、删除
删除操作对应 `delete()` 方法<br />同样，可以利用 `filter()` 和 `first()` 方法限制要删除的范围
```python
def delete(self):
    """
    删除数据
    :return:
    """
    # 1、删除查询到的第一条记录
    # Student.objects.filter(name="xag").first().delete()

    # 2、删除多条记录
    # Student.objects.filter(name="xag").delete()

    # 删除name值以xag开头的所有记录
    Student.objects.filter(name__startswith="xag").delete()
```
