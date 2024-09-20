Python 数据库 Sqlite<br />Sqlite 是一种 嵌入式数据库，数据库就是一个文件，体积很小，底层由 C 语言编写，经常被集成到移动应用程序中<br />事实上，Python 内置了 `sqlite3` 模块，不需要安装任何依赖，就可以直接操作 Sqlite 数据库
<a name="R5tkl"></a>
## 1、准备
和 Python 操作 Mysql 类似，操作 Sqlite 主要包含下面 2 种方式：

- sqlite3 + 原生 SQL
- SQLAlchemy + ORM
<a name="WMEPk"></a>
## 2、sqlite3 + 原生 SQL
由于 Python 内置了 sqlite3 模块，这里直接导入就可以使用了
```python
# 导入内置模块sqlite3
import sqlite3
```
首先使用 sqlite3 的 `connnect()` 方法创建一个数据库连接对象，如果数据库不存在，就自动在对应目录下新建一个数据库文件
```python
# 创建数据库连接对象，如果数据库不存在，就自动新建一个数据库文件
# 还可以指定其他参数，包含：超时时间
 self.conn = sqlite3.connect(self.path_db)
```
然后，通过数据库连接对象获取一个操作数据库的 游标实例
```python
# 获取操作数据库的游标对象
self.cursor = self.conn.cursor()
```
接着，使用数据库连接对象执行创建表的 SQL 语句，在数据库内新建一张表
```python
# 创建表
SQL_CREATE_TABLE = '''CREATE TABLE IF NOT EXISTS PEOPLE
       (ID INT PRIMARY KEY     NOT NULL,
       NAME           TEXT    NOT NULL,
       AGE            INT     NOT NULL);'''

def create_db_table(self):
    """
    初始化表
    :return:
    """
    self.conn.execute(SQL_CREATE_TABLE)
```
接下来，通过增删改查来操作数据表
<a name="fckkW"></a>
### 1、新增
同样以新增单条数据和多条数据为例<br />对于单条数据的插入，只需要编写一条插入的 SQL 语句，然后作为参数执行上面数据库连接对象的 `execute(sql)` 方法，最后使用数据库连接对象的 `commit()` 方法将数据提交到数据库中
```python
# 插入一条数据
SQL_INSERT_ONE_DATA = "INSERT INTO PEOPLE(id,name,age) VALUES(3,'xag',23);"

def insert_one(self):
    """新增一条数据"""
    try:
        self.conn.execute(SQL_INSERT_ONE_DATA)
        # 必须要提交，才能正确执行
        self.conn.commit()
    except Exception as e:
        self.conn.rollback()
        print('插入一条记录失败，回滚~')
```
需要注意的是，插入操作经常会因为主键原因导致新增异常，所以需要捕获异常，执行回滚操作<br />使用数据库连接对象的 `executemany()` 方法，传入插入的 SQL 语句及 位置变量列表，可以实现一次插入多条数据
```python
# 插入多条数据（3个变量，包含：id、name、value）
SQL_INSERT_MANY_DATA = 'INSERT INTO PEOPLE (id,name,age) VALUES(?,?,?);'

# 待插入的数据
self.data = [(4, '张三', 11), (5, '李四', 12), (6, '王五', 13)]

def insert_many(self, data):
    """新增多条数据"""
    try:
        self.conn.executemany(SQL_INSERT_MANY_DATA, data)
        self.conn.commit()
    except Exception as e:
        self.conn.rollback()
        print('插入多条记录失败，回滚~')
```
<a name="MKCXm"></a>
### 2、查询
查询分为 2 步，分别是：

- 通过游标对象执行查询的 SQL 语句
- 调用游标对象的方法获取查询结果

比如：<br />要获取所有数据，可以使用游标对象的 `fetchall()` 方法<br />要获取第一条满足条件的数据，可以使用 `fetchone()` 方法<br />另外，`fetchmany(num)` 可以查询固定数量的数据
```python
# 查询的SQL语句
SQL_QUERY_ONE_DATA = "SELECT * FROM PEOPLE WHERE id={}"

def query_one(self, id):
    """
    查询一条数据
    :param id:
    :return:
    """
    self.cursor.execute(SQL_QUERY_ONE_DATA.format(id))

    # fetchone():查询第一条数据
    # fetchall()：查询所有数据
    # fetchmany(1):查询固定的数量的数据
    result = self.cursor.fetchall()
    print(type(result))
    print(result)
```
<a name="oVazC"></a>
### 3、更新
和 新增操作 类似，更新操作也是通过数据库连接对象去执行更新的 SQL 语句，最后执行提交操作，将数据真实更新到数据表中<br />以更新某一条记录为例
```python
# 更新数据
SQL_UPDATE_ONE_DATA = "UPDATE PEOPLE SET NAME = '{}',AGE={} where id = {}"

def update_one(self, id, name, age):
    """
    修改一条记录
    :param id:
    :param name:
    :param age:
    :return:
    """
    sql_update = SQL_UPDATE_ONE_DATA.format(name, age, id)
    print(sql_update)
    self.conn.execute(sql_update)
    self.conn.commit()
```
<a name="nkPMo"></a>
### 4、删除
删除操作同查询、新增操作类似，只需要执行删除的 SQL 语句即可<br />以删除某一条记录为例
```python
# 删除数据
SQL_DEL_ONE_DATA = "DELETE FROM PEOPLE where id ={}"

def del_one(self, id):
    """通过id去删除一条数据"""
    sql_del = SQL_DEL_ONE_DATA.format(id)
    self.conn.execute(sql_del)
    self.conn.commit()
```
最后，同样需要将游标对象和数据库连接对象，资源释放
```python
def teardown(self):
    # 关闭游标和数据库连接，避免资源浪费
    self.cursor.close()
    self.conn.close()
```
<a name="aGcEf"></a>
## 3、SQLAlchemy + ORM
使用 SQLAlchemy 操作 sqlite 数据库同样先需要安装依赖库
```bash
# 安装依赖包
pip3 install sqlalchemy
```
通过内置方法 `declarative_base()` 创建一个基础类 Base<br />然后，自定义一个 Base 类的子类，内部通过定义静态变量指定表名、表的字段
```python
from sqlalchemy import Column, Integer, String, create_engine
from sqlalchemy.ext.declarative import declarative_base

# 基础类
Base = declarative_base()


# 自定义的表
class People(Base):
    # 表名
    __tablename__ = 'people'

    # 定义字段
    id = Column(Integer, primary_key=True)
    name = Column(String)
    age = Column(Integer)

    def __repr__(self):
        """
        便于打印结果
        :return:
        """
        return "<People(id:{},name:{},age:{})".format(self.id, self.name, self.age)
```
接着，通过 SQLAlchemy 的 `create_engine(sqlite数据库路径)` 方法中创建数据库连接对象<br />格式为：`sqlite:///数据库相对路径`
```python
# 创建数据库连接
engine = create_engine('sqlite:///./xh.db', echo=True)
```
最后，通过数据库引擎在数据库中创建表结构，并实例化一个 数据库会话对象<br />PS：数据库会话对象内置的方法非常方便进行增删改查操作
```python
# 创建表结构
# checkfirst：判断表是否存在，如果存在，就不重复创建
Base.metadata.create_all(engine, checkfirst=True)

# 实例化会话
self.session = sessionmaker(bind=engine)()
```
这样所有的准备工作已经完成，接下来可以进行增删改查操作了
<a name="XgIz7"></a>
### 1、新增
新增操作同样以新增一条和多条记录为例，它们分别对应会话对象的 `add()`、`add_all()` 方法<br />对于一条记录的新增操作，只需要实例化一个 People 对象，执行上面的会话对象的 `add(instance)` 和 `commit()` 两个方法，即可以将数据插入到数据表中
```python
def add_one_data(self):
    """新增一条数据"""
    # 创建一个表的实例对象
    people = People(name='xag1', age=24)
    self.session.add(people)

    # 必须提交，才能更新到数据库中
    self.session.commit()
```
如果需要一次插入多条数据，只需要调用 `add_all(列表数据)` 即可
```python
def add_datas(self, data):
    """
    新增多条数据
    :return:
    """
    self.session.add_all(data)
    self.session.commit()
```
<a name="AUPMG"></a>
### 2、查询
查询数据表的操作对应会话对象的 `query()` 方法<br />同时，还可以结合 `all()`、`first()`、`filter_by(限制条件)` 级联方法限制要查询的数据<br />以查询所有记录和根据 id 查询一条记录为例
```python
def query_one_data(self, id):
    """
    通过id去查询一条数据
    :param id:
    :return:
    """
    # 通过id去查询数据，取第一条
    people = self.session.query(People).filter_by(id=id).first()
    print(people)
    print(type(people))

def query_all(self):
    """
    查询所有数据
    :return:
    """
    peoples = self.session.query(People).all()
    print(peoples)
    print(type(peoples))
```
<a name="ofDzU"></a>
### 3、更新
更新操作一般做法是：

- query 查询出待更新的对象
- 直接更新对象中的数据
- 使用会话对象提交修改，完成更新操作
```python
def update1(self, id, name, age):
    """
    更新记录
    :param id:
    :param name:
    :param age:
    :return:
    """
    # 更新步骤：先查询、修改数据、然后确认修改
    people_temp = self.session.query(People).filter_by(id=id).first()

    # 修改数据
    people_temp.name = name
    people_temp.age = age

    # 确认提交修改
    self.session.commit()
```
需要指出的是，这里也可以使用内置方法 `update()` ，对上面的更新操作进行简写
```python
def update2(self, id, name, age):
    """
    更新记录方法2
    :param id:
    :param name:
    :param age:
    :return:
    """
    self.session.query(People).filter_by(id=id).update({People.name: name, People.age: age})
    self.session.commit()
```
<a name="TxwuB"></a>
### 4、删除
和更新操作一样，删除操作也有两种实现方式<br />第一种方式的思路是，先查询，后删除，最后提交会话完成删除操作<br />以按照 id 删除某一条记录为例：
```python
def del_one_data1(self, id):
    """
    删除一条数据方法1
    :param id:
    :return:
    """
    people_temp = self.session.query(People).filter_by(id=id).first()

    # 判断是否为空
    if people_temp is not None:
        self.session.delete(people_temp)
        self.session.commit()
    else:
        print('此条记录不存在，删除失败！')
```
需要注意的是，查询的结果必须判断是否为空，否则直接执行删除操作，可以会抛出异常<br />另外一种方式是，直接使用级联函数将上面的删除操作进行简写
```python
def del_one_data2(self, id):
    """
    删除一条数据方法2
    :param id:
    :return:
    """
    try:
        self.session.query(People).filter_by(id=id).first().delete()
    except Exception as e:
        print('删除失败')
```
同样，这种删除操作需要捕获异常，避免查询的结果为空的情况<br />最后，完成所有操作之后，需要结束会话、销毁数据库引擎
```python
def teardown(self):
    """
    释放资源
    :return:
    """
    # 结束会话
    self.session.close()

    # 销毁引擎
    self.engine.dispose()
```
