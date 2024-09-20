Python MySQL
<a name="TeUHq"></a>
## 1、通用步骤
其实，这里有一个通用步骤，都是写死了的，照做就行。
```python
# 1. 导入相关库
import pymysql

# 2. 链接MySQL服务器
db = pymysql.connect(host='localhost' , user='root' , password='******' , port=3306 ,db='spiders' , charset='utf8')

# 3. 创建一个cursor游标对象
cursor = db.cursor()

# 4. 在这一步写你的sql语句
sql = 'select version()'

# 5. 执行sql语句
cursor.execute(sql)

# 6. 断开连接
db.close()
```
可以看出，整个过程就是第四步那里不同。归根到底，其实就是写sql。
<a name="egQLg"></a>
## 2、需要特别说明的知识点
<a name="HOeln"></a>
### Ⅰ 各参数说明
```python
db = pymysql.connect(host='localhost' , user='root' , password='******' , port=3306 ,db='spiders' , charset='utf8')
```
这一行代码很长，里面涉及到好几个参数，一一介绍如下：

- 参数 1 ：mysql 服务器所在的主机 IP
- 参数 2 ：用户名；
- 参数 3 ：密码；
- 参数 4 ：连接的 mysql 主机的端口，默认是 3306；
- 参数 5 ：连接的数据库名；
- 参数 6 ：通信采用的编码方式，默认是'gb2312'，要求与数据库创建时指定的编码一致，否则中文会乱码；
<a name="w9b4g"></a>
### Ⅱ cursor游标对象
游标，通俗的解释就是"游动的标志"，这是数据库中一个很重要的概念。<br />有时候，执行一条查询语句的时候，往往会得到N条返回结果，执行sql语句取出这些返回结果的接口(起始点)，就是游标。沿着这个游标，可以一次取出一行记录。<br />当不使用游标功能，去执行`select * from student where age > 20;`这条语句的时候，如果有1000条返回结果，系统会一次性将1000条记录返回到界面中，没有选择，也不能做其他操作。<br />当开启了游标功能，再去执行这条语句的时候，系统会先找到这些行，先存放起来，然后提供了一个游标接口。当需要数据的时候，就借助这个游标去一行行的取出数据，每取出—条记录，游标指针就朝前移动一次，一直到取完最后一行数据后。
<a name="HcdSo"></a>
## 3、Python增删改查操作
<a name="cWmjY"></a>
### ① 创建一个数据表
```python
import pymysql

db = pymysql.connect(host='192.168.3.47' , user='root',password='******' , port=3306 , db='spiders' , charset='utf8')

cursor = db.cursor()

# 检查表是否存在，如果存在删除
cursor.execute('drop table if exists students')

# 创建表
sql = 'create table students(id int auto_increment primary key not null,name varchar(10) not null,age int not null)'

cursor.execute(sql)

db.close()
```
注：以后用代码创建表的机会并不多，表一般都是提前创建好的。
<a name="MSfbg"></a>
### ② 插入数据
```python
import pymysql

db = pymysql.connect(host='192.168.3.47' , user='root',password='******' , port=3306 , db='spiders' , charset='utf8')

cursor = db.cursor()

# 插入数据
sql = 'insert into students(name,age) values(%s,%s)'
try:
   cursor.execute(sql,('孙悟空',100000))
   db.commit()
except:print("插入失败")
   db.rollback()

db.close()
```
注 1：插入数据一定要用 `try…except…`语句，因为万一没插入成功，其余代码都无法执行。<br />注 2：`import pymysql`，此模块是默认开启mysql的事务功能的，因此，进行“增”、“删”、“改”的时候，一定要使用`db.commit()`提交事务，否则就看不见所插入的数据。
<a name="hHRg1"></a>
### ③ 更新数据
```python
import pymysql

db = pymysql.connect(host='192.168.3.47' , user='root',password='******' , port=3306 , db='spiders' , charset='utf8')

cursor = db.cursor()

# 更新数据
sql = 'update students set age =%s where name=%s'
try:
   cursor.execute(sql,(30,"郭卫华"))
   db.commit()
except:
   print("插入失败")
   db.rollback()

db.close()
```
<a name="YHk1L"></a>
### ④ 删除操作
```python
import pymysql

db = pymysql.connect(host='192.168.3.47' , user='root',password='******' , port=3306 , db='spiders' , charset='utf8')

cursor = db.cursor()

# 删除数据
sql = 'delete from students where age=100000'
try:
   cursor.execute(sql)
   db.commit()
except:
   print("插入失败")
   db.rollback()

db.close()
```
<a name="rAwiG"></a>
### ⑤ 查询操作

- `fetchone()`功能：获取下一个查询结果集，结果集是一个对象。
- `fetchall()`功能：接收全部返回的行。
```python
import pymysql

db = pymysql.connect(host='192.168.3.47' , user='root',password='******' , port=3306 , db='spiders' , charset='utf8')

cursor = db.cursor()

# 查询数据
sql = 'select * from students where age>60'
try:
   cursor.execute(sql)
   reslist = cursor.fetchall()
   for row in reslist:
     print("%d--%d" %(row[0],row[1],… row[n]))
except:
   print("插入失败")
   db.rollback()

db.close()
```
<a name="FlMU7"></a>
## 4、封装一个类
```python
# 注：把下面类写在 studentsql 文件中
import pymysql
class StudentsSql():
  def __init__(self,host,user,port,dbname,charset):
     self.host = host
     self.user = user
     self.port = port
     self.dbname = dbname
     self.charset = charset
  
  def connet(sef):
     self.db = pymysql.connect(self.host, self.user, self.port,
     self.dbname, self.charset)
 
  def close(self):
     self.cursor.close()
     self.db.close()

  def get_one(self,sql):
     res = None
     try:
        self.connect()
        self.cursor.execute(sql)
        res = self.cursor.fetchone()
        self.close()
     except:
        print("查询失败")
     return res

  def get_all(self,sql):
     res = None
     try:
        self.connect()
        self.cursor.execute(sql)
        res = self.cursor.fetchall()
        self.close()
     except:
        print("查询失败")
     return res

  def inset(self,sql):
     return self.__edit(sql)

  def update(self,sql):
     return self.__edit(sql)

  def delete(self,sql):
     return self.__edit(sql)

  def __edit(self,sql):
     count = 0
     try:
        self.connect()
        count = self.cursor.execute(sql)
        self.db.commit()
        self.close()
     except:
        print("事务提交失败")
        self.db.rollback()
     return count
```
上述类封装成功，以后只需要调用即可。
```python
from studentsql import StudentsSql
s = StudentsSql("host='192.168.3.47' , user='root', password='******' , port=3306 , db='spiders' , charset='utf8'")
res = s.get_all('select * from students where age>60')
for row in res:
    print("%d--%d" %(row[0],row[1],… row[n]))
```
