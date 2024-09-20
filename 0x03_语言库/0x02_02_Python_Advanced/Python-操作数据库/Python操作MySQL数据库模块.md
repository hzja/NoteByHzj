Python MySQL<br />mysql-connector-python是MySQL官方提供的<br />PyMySQL支持python2和python3<br />MySQLdb只支持python2
<a name="cDsGx"></a>
### 安装PyMySQL模块
命令行：
```bash
pip install pymysql
```
<a name="CfNuE"></a>
### 导入PyMySQL模块
```python
import pymysql
```
<a name="h69rp"></a>
### 获得连接对象
```python
conn=pymysql.connect(host=' ',user=' ',password=' ',database=' ',charset='utf-8')
```
获取游标对象通过游标c对数据进行CRUD
<a name="UX035"></a>
### 提交事务
```python
conn.commit()
```
<a name="Fz7AG"></a>
### 关闭游标
```python
c.close()
```
<a name="76183218"></a>
### 关闭连接
```python
conn.close()
```
<a name="pgcQ4"></a>
### 案例详解
<a name="qEXPx"></a>
#### 创建数据库
```python
'''创建数据库'''
import pymysql
#打开数据库连接，不需要指定数据库，因为需要创建数据库
conn = pymysql.connect('localhost',user = "root",passwd = "root")
#获取游标
cursor=conn.cursor()
#创建pythonBD数据库
cursor.execute('CREATE DATABASE IF NOT EXISTS pythonDB DEFAULT CHARSET utf8 COLLATE utf8_general_ci;')
cursor.close()#先关闭游标
conn.close()#再关闭数据库连接
print('创建pythonBD数据库成功')
```
<a name="GGJ9j"></a>
#### 创建表
```python
import pymysql
#打开数据库连接
conn = pymysql.connect('localhost',user = "root",passwd = "root",db = "myemployees")
#获取游标
cursor=conn.cursor()
print(cursor)
#创建user表
cursor.execute('drop table if exists user')
sql="""CREATE TABLE IF NOT EXISTS `user` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `name` varchar(255) NOT NULL,
    `age` int(11) NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=0"""
cursor.execute(sql)
cursor.close()#先关闭游标
conn.close()#再关闭数据库连接
print('创建数据表成功')
```
<a name="g6iK4"></a>
#### 表中插入单条数据
```python
'''插入单条数据'''
import pymysql
#打开数据库连接，不指定数据库
conn=pymysql.connect('localhost','root','root')
conn.select_db('pythondb')
cur=conn.cursor()#获取游标
#创建user表
cur.execute('drop table if exists user')
sql="""CREATE TABLE IF NOT EXISTS `user` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `name` varchar(255) NOT NULL,
    `age` int(11) NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=0"""
cur.execute(sql)
insert=cur.execute("insert into user values(1,'tom',18)")
print('添加语句受影响的行数：',insert)
#另一种插入数据的方式，通过字符串传入值
sql="insert into user values(%s,%s,%s)"
cur.execute(sql,(3,'kongsh',20))
cur.close()
conn.commit()
conn.close()
print('sql执行成功')
```
<a name="qPmGF"></a>
#### 查询数据
```python
'''fetchone'''
import pymysql
#打开数据库连接
conn=pymysql.connect('localhost','root','root')
conn.select_db('myemployees')
#获取游标
cur=conn.cursor()
cur.execute("select * from user;")
while 1:
   res=cur.fetchone()
   if res is None:
       #表示已经取完结果集
       break
   print (res)
cur.close()
conn.commit()
conn.close()
print('sql执行成功')
'''
(1, 'tom', 18)
(3, 'kongsh', 20)
sql执行成功
'''
```
<a name="F9X0S"></a>
### CRUD
 CRUD是指在做计算处理时的增加(Create)、读取查询(Retrieve)、更新(Update)和删除(Delete)几个单词的首字母简写。
<a name="sn1Jb"></a>
#### 增删改的两种操作
操作单条：`c.execute('SQL语句',(参数值))`<br />操作多条：`c.executemany('SQL语句',(多个参数值))`
<a name="glmEA"></a>
#### 查的三种操作
拿单条：`c.fetchone()`<br />拿多条：`fetchmany(size)`<br />拿所有：`c.fetchall()`
