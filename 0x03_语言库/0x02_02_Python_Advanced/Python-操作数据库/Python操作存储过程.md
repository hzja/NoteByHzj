Python 存储过程
<a name="HOAWB"></a>
## 1、前言
如果项目涉及复杂的 SQL 处理，就可以将这些操作封装成「 存储过程 」，公开入参及出参
<a name="cmeVC"></a>
## 2、存储过程
存储过程，全称为「 Stored Procedure 」<br />可以将它看成一个接口，内部会封装一些常用的操作，可以直接进行调用<br />存储过程的常见操作如下：
<a name="Pm2U3"></a>
### 2-1  管理存储过程

- 创建
- 查询
- 删除
```plsql
# 1、创建一个存储过程
# 存储过程名称为：xag
delimiter $
create procedure xag()
begin
...
end $

# 2.1 通过数据库名查询所有存储过程
# 比如：数据库名为xag
select `name` from mysql.proc where db = 'xag' and `type` = 'PROCEDURE';

# 2.2 查询存储过程中状态信息
show procedure status;

# 3.通过存储过程名称，删除一个存储过程
DROP PROCEDURE  IF EXISTS xag;
```
其中<br />使用「 create procedure 存储过程名称 」创建一个存储过程，接着在 begin 和 end 之间编写具体的操作逻辑
<a name="fdC3x"></a>
### 2-2  变量定义及赋值
使用关键字「 `declare` 」可以定义一个变量
```plsql
# 变量定义
# 比如：定义一个变量name，类型为字符串
# 默认值为 null
...
declare name varchar(255) default null;
...
```
给变量赋值有 2 种方式：普通 `set` 语法、`select into` 语法<br />其中

- `set` 语法可以通过表达式设置变量的值
- `select into` 语法是通过查询数据库表，将查询结果设置到变量中
```plsql
# 变量定义
declare name varchar(255) default null;

# 变量赋值
# set语法
set name = 'xag';

# select into语法
# 查询name_table表中的第一条记录中的name值，保存到name变量中
select name into name from name_table limit 1;
```
<a name="JWIyG"></a>
### 2-3  条件判断 `if`
比如，通过年龄判断年级（ if 语句）
```plsql
...
declare age int default 23;
declare grade varchar(255) default null;

# if语句
if age <=5 then
set grade = '幼儿园';
elseif age >= 6 and age < 12 then
set grade = '小学';
elseif age >=12 and age < 15 then
set grade = '初中';
elseif age >=15 and age < 18 then
set grade = '高中';
elseif age >=18 then
set grade = '其他';
end if;
...
```
<a name="fscVP"></a>
### 2-4  循环 `while`
比如，计算 1-10 数值的和，设置到变量 total 上
```plsql
...
# 总和
declare total int default 0;

# 结束值
declare end_number int default 10;

# 临时值
declare temp int default 0;

# while循环
while temp <= end_number do
# 设置值
set total = total + temp;
set temp = temp + 1;
end while;
...
```
<a name="LGMMj"></a>
### 2-5  入参和出参
为了使编写的存储过程更加实用，需要在常见存储过程时，设置出参和入参<br />语法格式如下：
```python
# 创建一个存储过程
create procedure proce_name([in/out/inout] 参数名 参数类型)
```
其中

- 默认传入值为入参，即 `in`
- `out` 代表出参，作为返回值返回
- 如果设置为 `inout`，则代表既能作为出参，也可以作为入参
<a name="Z8rVs"></a>
## 3、实战一下
使用 Python 调用存储过程非常方便<br />首先，编写一个存储过程<br />比如，这里定义了一个存储过程，传入两个入参和一个出参，将两个入参的乘积作为出参返回
```python
# 定义一个存储过程
delimiter $
create procedure num_multi(in num1 int,in num2 int,out multiply_result int)
begin
# 两个入参相乘，然后设置到出参中去
set multiply_result = num1 * num2;
end $
```
然后，在数据库中进行调用测试<br />使用关键字「 `call` 」调用存储过程，使用 `select` 查看返回值
```python
# 调用存储过程
call num_multi(1,3,@multiply_result);
select @multiply_result;
```
接着，利用数据库配置信息创建连接及游标对象
```python
import pymysql

PY_MYSQL_CONN_DICT = {
    "host": '127.0.0.1',
    "port": 3306,
    "user": 'root',
    "passwd": 'root',
    "db": 'test_db'
}

# 数据库连接
db_conn = pymysql.connect(**PY_MYSQL_CONN_DICT)

# 游标
db_cursor = db_conn.cursor(cursor=pymysql.cursors.DictCursor)

最后，使用函数「 callproc 」调用存储过程名称及所有参数，获取返回值
在执行完存储过程后，需要通过游标对象的「 execute 」函数获取出参及入参
db_cursor.callproc('num_multi', args=(3, 6, -1))

# 获取入参及出参
db_cursor.execute('SELECT @_num_multi_0, @_num_multi_1, @_num_multi_2')
# 出参值
output_result = db_cursor.fetchone()['@_num_multi_2']

# 出参值
print(output_result)
```
需要注意的是，如果存储过程涉及到更新、新增等操作，需要显式调用 `commit()` 函数，才会真正提交到数据库中
