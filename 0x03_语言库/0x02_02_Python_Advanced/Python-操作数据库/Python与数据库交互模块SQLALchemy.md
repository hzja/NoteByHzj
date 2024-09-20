Python<br />SQLALchemy这个模块是Python当中最有名的ORM框架，该框架是建立在数据库API之上，使用关系对象映射进行数据库的操作，简而言之便是：将对象转换成SQL，然后使用数据API执行SQL并获取执行结果。<br />通过一个简单的案例在说明一下。例如想要在MySQL当中新建一个表格，首先需要连接上数据库，代码如下
```python
# 连接数据库
sql_connect = 'mysql+pymysql://root:123456@localhost:3306/sql_prac?charset=utf8'
engine = create_engine(sql_connect)
DBSession = sessionmaker(bind=engine)
# 创建对象的基类:
BaseModel = declarative_base()
```
<a name="yrwRW"></a>
## 定义表结构
对于新创建的表格，命名为是“User”，同时还需要定义表结构，代码如下
```python
#定义对象
class User(BaseModel):
    # 表名
    __tablename__ = 'user'
    # 表结构，其中ID设为是主键，并且是自动增加的
    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(20))
    age = Column(Integer)
```
<a name="b0mE9"></a>
## 创建以及删除表
对于创建表以及删除表的操作，代码如下
```python
#创建映射的数据库表
def init_db():
    BaseModel.metadata.create_all(engine)

#删除映射的数据库表
def drop_db():
    BaseModel.metadata.drop_all(engine)
```
<a name="Ak1CL"></a>
## 插入数据
可以尝试往新建的表格当中插入几个值，代码如下
```python
def insert_data(name_1, age_1):
    # 创建session对象,相当于MySQLdb里面的游标
    session = DBSession()
    # 创建新User对象:
    new_user = User(name=name_1, age=age_1)
    # 添加到session:
    session.add(new_user)
    # 提交添加数据的操作
    session.commit()
    # 关闭session
    session.close()
    
if __name__ == "__main__":
    insert_data(name_1="Mike", age_1=20)
    insert_data(name_1="John", age_1=35)
    .......
```
运行后的结果如下图所示<br />![](./img/1649067111504-69044132-8ca3-4100-87b5-eb4b8f6a4819.png)
<a name="mvh43"></a>
## 查询
要是想要查询表格中的数据，可以这么来做
```python
# 创建Session:
session = DBSession()
# 创建Query查询，filter是where条件，最后调用one()返回唯一行，如果调用all()则返回所有行:
user = session.query(User).filter(User.name == 'Tom').one()
# 打印类型和对象的name属性和age属性:
print(user.name, user.age)
# 关闭Session:
session.close()
```
要是调用的是`all()`则返回所有行，因此需要通过for循环遍历出来的结果然后打印，代码如下
```python
users = session.query(User).filter(User.name == 'John').all()
for u in users:
    print(u.name, u.age)
```
<a name="koBN4"></a>
## 更新和删除数据
尝试来更新表格中的一些数据，代码如下
```python
# 创建Session:
session = DBSession()
# 可以进行多条数据更新
user = session.query(User).filter(User.id == 3)
user.update({User.age: 30})
# 提交数据
session.commit()
# 关闭Session
session.close()
```
通过“ID”来锁定要更新的数据的位置，然后通过调用`update()`方法将其年龄改成指定的值。与此同时还可以来删除表格当中的一些值，代码如下
```python
# 创建Session
session = DBSession()
# 删除哪些数据
user = session.query(User).filter(User.id == 5).one()
session.delete(user)
# 提交数据
session.commit()
# 关闭session
session.close()
```
同样也是通过“ID”来锁定要删除数据的位置，然后调用`delete()`方法。
<a name="ezhCx"></a>
## 直接运行SQL语句
当然在创建session之后，也可以在里面直接运行SQL语句，例如想要查看一下总共有哪些数据库，代码如下
```python
session = DBSession()
print(session.execute('show databases').fetchall())
session.close()
```
或者是想返回表格中的所有数据，代码如下
```python
session = DBSession()
print(session.execute('select * from user').fetchall())
session.close()
```
<a name="lkLO7"></a>
## DataFrame到MySQL数据库
同时也可以批量的将excel或者csv文件当中的数据批量的导入到MySQL数据库当中，先通过Pandas读取文件中的数据，代码如下
```python
sql_connect = 'mysql+pymysql://用户名:密码@ip地址:端口号/数据库名称?charset=utf8'
engine = create_engine(sql_connect)
df = pd.read_excel("sqlalchemy_test1.xlsx")
df.to_sql("user", engine, index=False, if_exists='append')
```
当然也可以从数据库的某个表格当中来读取数据，代码如下
```python
df = pd.read_sql("表格名", engine)
print(df.head())
```
<a name="ZKMtL"></a>
## 数据库和Excel之间的导入导出
<a name="fRNYw"></a>
### 数据库->Excel
使用Python代码实现数据从数据库导入到Excel其实很简单，三行代码就可以搞定！<br />![](./img/1650717171927-17155451-acca-4269-a859-0c66e2b64df6.png)<br />首先看一下数据库里面有一个 department这个部门表。这个表里有六条数据，分别代表不同的部门。<br />接下来看一下这个Python代码，首先导入需要用到的库SQLAlchemy，这是Python中最有名的ORM工具。<br />全称Object Relational Mapping（对象关系映射）。
<a name="ZIrrC"></a>
#### 为什么要使用SQLAlchemy？
它可以将代码从底层数据库及其相关的SQL特性中抽象出来。<br />特点是操纵Python对象而不是SQL查询，也就是在代码层面考虑的是对象，而不是SQL，体现的是一种程序化思维，这样使得Python程序更加简洁易读。
<a name="RlVN8"></a>
#### 具体使用方法如下
```python
from sqlalchemy import create_engine
import pandas as pd

# 创建数据库连接
engine = create_engine('mysql+pymysql://root:211314@localhost/hong')

# 读取mysql数据
db = pd.read_sql(sql='select * from hong.department', con=engine)

# 导出数据到excel
db.to_excel('部门数据.xlsx')
```
第一行代码就是首先创建数据库的连接。这里的用户名是root，密码是211314，因为这里启动的是本地的数据库服务，所以是localhost。斜杠后面跟的是这个数据库的名称hong。<br />第二行代码就是使用pandas的`read_sql()`查询MySQL表department中的数据，第二行代码就是将查询出来的数据通过pandas的`to_excel()`写到本地。<br />执行结果成功写入本地**Excel**文件<br />![](./img/1650717171826-30c51590-b6ca-44e7-b502-3cb5f852a62b.png)
<a name="PaHHI"></a>
### Excel->数据库
接下来再看看如何将本地的xlsx数据写入到MySQL文件中。
```python
from sqlalchemy import create_engine
import pandas as pd

# 创建数据库连接
engine = create_engine('mysql+pymysql://root:211314@localhost/hong')

# 读取xlsx文件
df = pd.read_excel('模拟数据.xlsx')

# 导入到mysql数据库
df.to_sql(name='test_data', con=engine, index=False, if_exists='replace')
```
同样第一行代码就是首先创建数据库的连接<br />第二行代码使用Pandas的`read_excel()`读取本地文件。如下:<br />这是用Python的faker模拟出来的一百条数据<br />![](./img/1650717171814-440607e1-15f6-4e0c-a918-c2689355510a.png)<br />第三步使用Pandas的`to_sql()`方法将读取到的数据写入到MySQL中<br />代码执行完成后返回MySQL中hong数据库发现多出了一个test_data的表。<br />![](./img/1650717171896-cb8001dc-83a3-42fc-b309-2112bfda3cbd.png)<br />打开看一下。那这个数据就跟本地的数据是一样的。所以。这里用到三行代码从数据库向**Excel**导入数据，又用了三行代码从**Excel**向数据库导入数据。
<a name="BFGHG"></a>
### 总结一下
双向数据导入，都是3行代码即可实现。<br />**从数据库向Excel导入数据：**<br />1、用sqlalchemy创建数据库连接<br />2、用pandas的`read_sql`读取数据库的数据<br />3、用pandas的`to_csv`把数据存入csv文件<br />**从Excel向数据库导入数据：**<br />1、用sqlalchemy创建数据库连接<br />2、用pandas的`read_csv`读取csv的数据<br />3、用pandas的`to_sql`把数据存入数据库
