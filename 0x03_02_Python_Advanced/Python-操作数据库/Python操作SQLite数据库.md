Python SQLite
<a name="50I7U"></a>
## SQLite数据库
SQLite是一个进程内的库，实现了自给自足的、无服务器的、零配置的、事务性的 SQL 数据库引擎。它是一个零配置的数据库，这意味着与其他数据库不一样，不需要在系统中配置。<br />就像其他数据库，SQLite 引擎不是一个独立的进程，可以按应用程序需求进行静态或动态连接。SQLite 直接访问其存储文件。
<a name="JBoOg"></a>
## 数据类型
SQLite 数据类型是一个用来指定任何对象的数据类型的属性。SQLite 中的每一列，每个变量和表达式都有相关的数据类型。<br />您可以在创建表的同时使用这些数据类型。SQLite 使用一个更普遍的动态类型系统。在 SQLite 中，值的数据类型与值本身是相关的，而不是与它的容器相关。<br />SQLite的主要数据类型有：NULL、INTEGER、REAL、TEXT、BLOB
<a name="uf3v8"></a>
## Python操作SQLite数据库
<a name="ca4ef8a7"></a>
### 导入模块
```python
import sqlite3
```
<a name="lCJP8"></a>
### 创建/打开数据库
在调用connect函数的时候，指定库名称，如果指定的数据库存在就直接打开这个数据库，如果不存在就新创建一个再打开。
```python
cx = sqlite3.connect("E:/test.db")  #这里创一个文件
```
也可以创建数据库在内存中。
```python
con = sqlite3.connect(":memory:")
```
<a name="qBtGJ"></a>
### 数据库连接对象
 打开数据库时返回的对象cx就是一个数据库连接对象，它可以有以下操作：

1. commit()--事务提交  <br />
2. rollback()--事务回滚  <br />
3. close()--关闭一个数据库连接  <br />
4. cursor()--创建一个游标<br />
<a name="NUVbe"></a>
### 使用游标查询数据库
```python
c = conn.cursor() #需要使用游标对象SQL语句查询数据库，获得查询对象。
```
游标对象有以下的操作：

1. execute()--执行sql语句  <br />
2. executemany--执行多条sql语句  <br />
3. close()--关闭游标  <br />
4. fetchone()--从结果中取一条记录，并将游标指向下一条记录  <br />
5. fetchmany()--从结果中取多条记录  <br />
6. fetchall()--从结果中取出所有记录  <br />
7. scroll()--游标滚动  <br />
<a name="Utl2l"></a>
#### 创建表
```python
c.execute('''CREATE TABLE stocks(date text,trans text,symbol text,gty real,price real)''')
```
<a name="f1w0z"></a>
#### 向表中插入一条数据
```python
c.execute('''insert into stocks values('2016-01-05','BUY','RHAT',100,35.14)''')
```
<a name="oZj9Z"></a>
### 完整的案例演示
<a name="Ao5jh"></a>
#### 连接数据库
```python
import sqlite3
#数据库名
db_name = "test.db"
#表名
table_name = "catalog"
conn = sqlite3.connect(db_name)
```
<a name="4DsbB"></a>
#### 打开游标
```python
rs=conn.cursor()
```
<a name="hV1QY"></a>
#### 建表
```python
sql = 'create table ' + table_name + ' (id varchar(20) primary key, pid integer, name varchar(10))'
try:
rs.execute(sql)
print("建表成功")
except:
print("建表失败")
```
<a name="2a0LR"></a>
#### 增删改查
```python
#增：增加三条记录
sql = "Insert into " + table_name + " values ('001', 1, '张三')"
try:
rs.execute(sql)
#提交事务
conn.commit()
print("插入成功")
except:
print("插入失败")
sql = "Insert into " + table_name + " values ('002', 2, '李四')"
try:
rs.execute(sql)
#提交事务
conn.commit()
print("插入成功")
except:
print("插入失败")
sql = "Insert into " + table_name + " values ('003', 3, '王五')"
try:
rs.execute(sql)
#提交事务
conn.commit()
print("插入成功")
except:
print("插入失败")
#删：删除pid等于3的记录
sql = "Delete from " + table_name + " where pid = 3"
try:
rs.execute(sql)
conn.commit()
print("删除成功")
except:
print("删除失败")
#改：将pid等于2的记录的pid改为1
sql = "Update " + table_name + " set pid = 1 where pid = 2"
try:
rs.execute(sql)
conn.commit()
print("修改成功")
except:
print("修改失败")
#查：查询数据库中所有表名
sql = "Select name From sqlite_master where type = 'table'"
res = rs.execute(sql)
print(res.fetchall())
#查询表中所有记录
sql = "Select * from " + table_name
try:
res = rs.execute(sql)
   print(res.fetchall())
except:
print([])
```
<br />
