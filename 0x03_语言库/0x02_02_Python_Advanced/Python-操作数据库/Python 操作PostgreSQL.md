Python 数据库 PostgreSQL
<a name="WzZ7U"></a>
## 1、PgSQL 使用
Python 操作 PgSQL，需要先安装依赖包「 psycopg2 」
```bash
# 安装依赖包
pip3 install psycopg2
```
接下来，就可以使用 Python 来操作数据库了
<a name="jH88d"></a>
### 数据库连接及游标对象
使用 psycopg2 中的「 `connect()` 」方法连接数据库，创建数据库连接对象及游标对象
```python
import psycopg2

# 获得连接对象
# database：数据库名称
# user：用户名
# password：密码
# host：数据库ip地址
# port：端口号，默认为5432
conn = psycopg2.connect(database="db_name", user="postgres", password="pwd", host="127.0.0.1", port="5432")

# 获取游标对象
cursor = conn.cursor()
```
获取游标对象后，就可以执行 SQL，进而操作数据库了
<a name="KPbjx"></a>
### 插入数据
首先，编写插入数据的 SQL 语句及参数（ 可选 ）
```python
# 构建SQL语句
# 方式一：直带参数
 sql = "INSERT INTO student (name,age) \
                        VALUES (%s, '%s')" % \
          ('xag',23)

# 方式二：参数分离
sql = """INSERT INTO student (name,age) VALUES (%s, %s)"""
# 参数
params = ('xag',23)
```
然后，使用游标对象执行 SQL
```python
# 执行sql
# 注意：params可选，根据上面的参数方式来选择设置
cursor.execute(sql,[params])
```
接着，使用连接对象提交事务
```python
 # 事务提交
conn.commit()
```
最后，释放游标对象及数据库连接对象
```python
# 释放游标对象及数据库连接对象
cursor.close()
conn.close()
```
<a name="W96P8"></a>
### 查询数据
游标对象的 `fetchone()`、`fetchmany(size)`、`fetchall()` 这 3个函数即可以实现单条数据查询、多条数据查询、全部数据查询
```python
# 获取一条记录
one_data = cursor.fetchone()
print(one_data)

# 获取2条记录
many_data = cursor.fetchmany(2)
print(many_data)

# 获取全部数据
all_data = cursor.fetchall()
print(all_data)
```
需要注意的是，条件查询与上面的插入操作类似，条件语句可以将参数分离出来
```python
# 条件查询 SQL语句
sql = """SELECT * FROM student where id = %s;"""

# 对应参数，参数结尾以逗号结尾
params = (1,)

# 执行SQL
cursor.execute(sql, params)

# 获取所有数据
datas = cursor.fetchall()
print(datas)
```
<a name="RmDso"></a>
### 更新数据
更新操作和上面操作一样，唯一不同的是，执行完 SQL 后，需要使用连接对象提交事务，才能将数据真实更新到数据库中
```python
def update_one(conn, cursor):
    """更新操作"""
    # 更新语句
    sql = """update student set name = %s where id = %s  """
    params = ('AirPython', 1,)

    # 执行语句
    cursor.execute(sql, params)

    # 事务提交
    conn.commit()

    # 关闭数据库连接
    cursor.close()
    conn.close()
```
<a name="RH2lD"></a>
### 删除数据
删除数据同更新数据操作类似
```python
def delete_one(conn, cursor):
    """删除操作"""
    # 语句及参数
    sql = """delete from  student where id = %s  """
    params = (1,)

    # 执行语句
    cursor.execute(sql, params)

    # 事物提交
    conn.commit()

    # 关闭数据库连接
    cursor.close()
    conn.close()
```
<a name="HntA3"></a>
## 2、最后
通过上面操作，可以发现 Python 操作 PgSQl 与 Mysql 类似，但是在原生 SQL 编写上两者还是有很多差异性，更多关于 PgSQL 的操作可以参考下面链接：[https://www.postgresql.org/docs/14/index.html](https://www.postgresql.org/docs/14/index.html)
