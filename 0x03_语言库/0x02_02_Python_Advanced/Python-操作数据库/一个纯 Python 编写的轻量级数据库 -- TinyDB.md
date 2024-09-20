Python<br />TinyDB 是一个纯 Python 编写的轻量级数据库，一共只有1800行代码，没有外部依赖项。<br />TinyDB的目标是降低小型 Python 应用程序使用数据库的难度，对于一些简单程序而言与其用 SQL 数据库，不如就用TinyDB， 因为它有如下特点：

- **轻便：**当前源代码有 1800 行代码（大约 40% 的文档）和 1600 行测试代码。
- **可随意迁移：**在当前文件夹下生成数据库文件，不需要任何服务，可以随意迁移。
- **简单：**TinyDB 通过提供简单干净的 API 使得用户易于使用。
- **用纯 Python 编写：** TinyDB 既不需要外部服务器，也不需要任何来自 PyPI 的依赖项。
- **适用于 Python 3.6+ 和 PyPy3：** TinyDB 适用于所有现代版本的 Python 和 PyPy。
- **强大的可扩展性：**可以通过编写中间件修改存储的行为来轻松扩展 TinyDB。
- **100% 测试覆盖率：**无需解释。
<a name="GrjrM"></a>
## 1、准备
开始之前，需要先安装它
```bash
pip install tinydb
```
<a name="fa819"></a>
## 2、简单的增删改查示例
初始化一个DB文件：
```python
from tinydb import TinyDB
db = TinyDB('db.json')
```
这样就在当前文件夹下生成了一个名为 `db.json` 的数据库文件。<br />往里面**插入数据**：
```python
from tinydb import TinyDB
db = TinyDB('db.json')
db.insert({'type': 'apple', 'count': 7})
db.insert({'type': 'peach', 'count': 3})
```
可以看到，可以直接往数据库里插入字典数据，不需要任何处理。下面是批量插入的方法：
```python
db.insert_multiple([
    {'name': 'John', 'age': 22},
    {'name': 'John', 'age': 37}])
db.insert_multiple({'int': 1, 'value': i} for i in range(2))
```
<a name="feo3v"></a>
### 查询所有数据：
```python
from tinydb import TinyDB
db = TinyDB('db.json')
db.all()
# [{'count': 7, 'type': 'apple'}, {'count': 3, 'type': 'peach'}]
```
除了 `.all()` 还可以使用for循环遍历db：
```python
from tinydb import TinyDB
db = TinyDB('db.json')
for item in db:
    print(item)
# {'count': 7, 'type': 'apple'}
# {'count': 3, 'type': 'peach'}
```
如果需要**搜索特定数据**，可以使用`Query()`：
```python
from tinydb import TinyDB
db = TinyDB('db.json')
Fruit = Query()
db.search(Fruit.type == 'peach')
# [{'count': 3, 'type': 'peach'}]
db.search(Fruit.count > 5)
# [{'count': 7, 'type': 'apple'}]
```
<a name="hytuF"></a>
### 更新数据：
```python
from tinydb import TinyDB
db = TinyDB('db.json')
db.update({'foo': 'bar'})

# 删除某个Key
from tinydb.operations import delete
db.update(delete('key1'), User.name == 'John')
```
<a name="VwlfB"></a>
### 删除数据：
删除数据也可以使用类似的条件语句：
```python
from tinydb import TinyDB
db = TinyDB('db.json')
db.remove(Fruit.count < 5)
db.all()
# [{'count': 10, 'type': 'apple'}]
```
<a name="IWz7J"></a>
### 清空整个数据库：
```python
from tinydb import TinyDB
db = TinyDB('db.json')
db.truncate()
db.all()
# []
```
<a name="rAOTB"></a>
## 3、高级查询
除了点操作符访问数据，还可以用原生的dict访问表示法：
```python
# 写法1
db.search(User.country-code == 'foo')
# 写法2
db.search(User['country-code'] == 'foo')
```
这两种写法是等效的。<br />另外在常见的查询运算符(==, <, >, ...)之外，TinyDB还支持`where`语句：
```python
from tinydb import where
db.search(where('field') == 'value')
```
这等同于：
```python
db.search(Query()['field'] == 'value')
```
这种语法还能访问嵌套字段：
```python
db.search(where('birthday').year == 1900)
# 或者
db.search(where('birthday')['year'] == 1900)
```
Any 查询方法：
```python
db.search(Group.permissions.any(Permission.type == 'read'))
# [{'name': 'user', 'permissions': [{'type': 'read'}]},
# {'name': 'sudo', 'permissions': [{'type': 'read'}, {'type': 'sudo'}]},
# {'name': 'admin', 'permissions':
# [{'type': 'read'}, {'type': 'write'}, {'type': 'sudo'}]}]
```
检查单个项目是否包含在列表中：
```python
db.search(User.name.one_of(['jane', 'john']))
```
TinyDB还支持和Pandas类似的逻辑操作：
```python
# Negate a query:
db.search(~ (User.name == 'John'))
# Logical AND:
db.search((User.name == 'John') & (User.age <= 30))
# Logical OR:
db.search((User.name == 'John') | (User.name == 'Bob'))
```
TinyDB的介绍就到这里，还可以访问他们的官方文档，查看更多的使用方法：<br />[https://tinydb.readthedocs.io/en/latest/usage.html](https://tinydb.readthedocs.io/en/latest/usage.html)<br />尤其是想基于TinyDB做些存储优化的同学，可以详细阅读 Storage & Middleware 章节。
