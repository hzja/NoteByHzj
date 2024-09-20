Java SQL Mybatis
<a name="sDLU5"></a>
## 一、什么是sql注入
SQL注入是比较常见的网络攻击方式之一，它不是利用操作系统的BUG来实现攻击，而是针对程序员编写时的疏忽，通过SQL语句，实现无账号登录，甚至篡改数据库。
<a name="rKXKl"></a>
## 二、SQL注入攻击的总体思路
1：寻找到SQL注入的位置<br />2：判断服务器类型和后台数据库类型<br />3：针对不同的服务器和数据库特点进行SQL注入攻击
<a name="KIrlt"></a>
## 三、SQL注入攻击实例
```sql
String sql = "select * from user_table where username=
' "+userName+" ' and password=' "+password+" '";

--当输入了上面的用户名和密码，上面的SQL语句变成：
SELECT * FROM user_table WHERE username=
''or 1 = 1 -- and password=''

"""
--分析SQL语句：
--条件后面username=”or 1=1 用户名等于 ” 或1=1 那么这个条件一定会成功；

--然后后面加两个-，这意味着注释，它将后面的语句注释，让他们不起作用，这样语句永远都--能正确执行，用户轻易骗过系统，获取合法身份。
--这还是比较温柔的，如果是执行
SELECT * FROM user_table WHERE
username='' ;DROP DATABASE (DB Name) --' and password=''
--其后果可想而知…
"""
```
<a name="E2fgG"></a>
## 四、如何防御SQL注入
注意：但凡有SQL注入漏洞的程序，都是因为程序要接受来自客户端用户输入的变量或URL传递的参数，并且这个变量或参数是组成SQL语句的一部分，对于用户输入的内容或传递的参数，应该要时刻保持警惕，这是安全领域里的「外部数据不可信任」的原则，纵观Web安全领域的各种攻击方式，大多数都是因为开发者违反了这个原则而导致的，所以自然能想到的，就是从变量的检测、过滤、验证下手，确保变量是开发者所预想的。
<a name="bRvef"></a>
### 1、检查变量数据类型和格式
如果SQL语句是类似`where id={$id}`这种形式，数据库里所有的id都是数字，那么就应该在SQL被执行前，检查确保变量id是int类型；如果是接受邮箱，那就应该检查并严格确保变量一定是邮箱的格式，其他的类型比如日期、时间等也是一个道理。总结起来：只要是有固定格式的变量，在SQL语句执行前，应该严格按照固定格式去检查，确保变量是预想的格式，这样很大程度上可以避免SQL注入攻击。<br />比如，前面接受username参数例子中，产品设计应该是在用户注册的一开始，就有一个用户名的规则，比如5-20个字符，只能由大小写字母、数字以及一些安全的符号组成，不包含特殊字符。此时应该有一个check_username的函数来进行统一的检查。不过，仍然有很多例外情况并不能应用到这一准则，比如文章发布系统，评论系统等必须要允许用户提交任意字符串的场景，这就需要采用过滤等其他方案了。
<a name="mxlAB"></a>
### 2、过滤特殊符号
对于无法确定固定格式的变量，一定要进行特殊符号过滤或转义处理。
<a name="tGHgH"></a>
### 3、绑定变量，使用预编译语句
MySQL的mysqli驱动提供了预编译语句的支持，不同的程序语言，都分别有使用预编译语句的方法<br />实际上，绑定变量使用预编译语句是预防SQL注入的最佳方式，使用预编译的SQL语句语义不会发生改变，在SQL语句中，变量用问号`?`表示，黑客即使本事再大，也无法改变SQL语句的结构
<a name="DY4ha"></a>
## 五、什么是sql预编译
<a name="WaHjR"></a>
### 1. 预编译语句是什么
通常一条sql在db接收到最终执行完毕返回可以分为下面三个过程：

- 词法和语义解析
- 优化sql语句，制定执行计划
- 执行并返回结果

把这种普通语句称作Immediate Statements。<br />但是很多情况，一条sql语句可能会反复执行，或者每次执行的时候只有个别的值不同（比如`query`的`where`子句值不同，`update`的`set`子句值不同，`insert`的`values`值不同）。如果每次都需要经过上面的词法语义解析、语句优化、制定执行计划等，则效率就明显不行了。<br />所谓预编译语句就是将这类语句中的值用占位符替代，可以视为将sql语句模板化或者说参数化，一般称这类语句叫Prepared Statements或者Parameterized Statements<br />预编译语句的优势在于归纳为：一次编译、多次运行，省去了解析优化等过程；此外预编译语句能防止sql注入。　　当然就优化来说，很多时候最优的执行计划不是光靠知道sql语句的模板就能决定了，往往就是需要通过具体值来预估出成本代价。
<a name="Tfu1N"></a>
### 2. MySQL的预编译功能
注意MySQL的老版本（4.1之前）是不支持服务端预编译的，但基于目前业界生产环境普遍情况，基本可以认为MySQL支持服务端预编译。<br />下面来看一下MySQL中预编译语句的使用。
<a name="rlRtd"></a>
#### （1）建表
首先有一张测试表t，结构如下所示：
```sql
mysql> show create table t\G
*************************** 1. row ***************************
       Table: t
Create Table: CREATE TABLE `t` (
  `a` int(11) DEFAULT NULL,
  `b` varchar(20) DEFAULT NULL,
  UNIQUE KEY `ab` (`a`,`b`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
```
<a name="zxO2Y"></a>
#### （2）编译
接下来通过 `PREPARE stmt_name FROM preparable_stm`的语法来预编译一条sql语句
```sql
mysql> prepare ins from 'insert into t select ?,?';
Query OK, 0 rows affected (0.00 sec)
Statement prepared
```
<a name="SiofW"></a>
#### （3）执行
通过`EXECUTE stmt_name [USING @var_name [, @var_name] ...]`的语法来执行预编译语句
```sql
mysql> set @a=999,@b='hello';
Query OK, 0 rows affected (0.00 sec)
 
mysql> execute ins using @a,@b;
Query OK, 1 row affected (0.01 sec)
Records: 1  Duplicates: 0  Warnings: 0
 
mysql> select * from t;
+------+-------+
| a    | b     |
+------+-------+
|  999 | hello |
+------+-------+
1 row in set (0.00 sec)
```
可以看到，数据已经被成功插入表中。<br />MySQL中的预编译语句作用域是session级，但可以通过`max_prepared_stmt_count`变量来控制全局最大的存储的预编译语句。
```sql
mysql> set @@global.max_prepared_stmt_count=1;
Query OK, 0 rows affected (0.00 sec)
 
mysql> prepare sel from 'select * from t';
ERROR 1461 (42000): Can't create more than max_prepared_stmt_count statements (current value: 1)
```
当预编译条数已经达到阈值时可以看到MySQL会报如上所示的错误。
<a name="sgcZx"></a>
#### （4）释放
如果想要释放一条预编译语句，则可以使用`DEALLOCATEDROPPREPARE stmt_name`的语法进行操作：
```sql
mysql> deallocate prepare ins;
Query OK, 0 rows affected (0.00 sec)
```
<a name="NmHQW"></a>
## 六、为什么PrepareStatement可以防止sql注入
原理是PrepareStatement采用了预编译的方法，先将SQL语句中可被客户端控制的参数集进行编译，生成对应的临时变量集，再使用对应的设置方法，为临时变量集里面的元素进行赋值，赋值函数`setString()`，会对传入的参数进行强制类型检查和安全检查，所以就避免了SQL注入的产生。下面具体分析
<a name="ndr2d"></a>
### 1、为什么Statement会被sql注入
因为Statement之所以会被sql注入是因为SQL语句结构发生了变化。比如：
```sql
"select * from tablename where username='"+uesrname+  
"'and password='"+password+"'"
```
在用户输入'`or true or`'之后sql语句结构改变。
```sql
select * from tablename where username='' or true or '' and password=''
```
这样本来是判断用户名和密码都匹配时才会计数，但是经过改变后变成了或的逻辑关系，不管用户名和密码是否匹配该式的返回值永远为true;
<a name="vobgE"></a>
### 2、为什么Preparement可以防止SQL注入
因为Preparement样式为
```sql
select * from tablename where username=? and password=?
```
该SQL语句会在得到用户的输入之前先用数据库进行预编译，这样的话不管用户输入什么用户名和密码的判断始终都是并的逻辑关系，防止了SQL注入。<br />简单总结，参数化能防注入的原因在于，语句是语句，参数是参数，参数的值并不是语句的一部分，数据库只按语句的语义跑，至于跑的时候是带一个普通背包还是一个怪物，不会影响行进路线，无非跑的快点与慢点的区别。
<a name="W5ESV"></a>
## 七、Mybatis是如何防止SQL注入的
1、首先看一下下面两个sql语句的区别：
```xml
<select id="selectByNameAndPassword" parameterType="java.util.Map" resultMap="BaseResultMap">
  select id, username, password, role
  from user
  where username = #{username,jdbcType=VARCHAR}
  and password = #{password,jdbcType=VARCHAR}
</select>

~
<select id="selectByNameAndPassword" parameterType="java.util.Map" resultMap="BaseResultMap">
  select id, username, password, role
  from user
  where username = ${username,jdbcType=VARCHAR}
  and password = ${password,jdbcType=VARCHAR}
</select>
```
<a name="Xr33J"></a>
### Mybatis中的`#`和`$`的区别：
1、`#`将传入的数据都当成一个字符串，会对自动传入的数据加一个双引号。如：`where username=#{username}`，如果传入的值是111,那么解析成sql时的值为`where username="111"`, 如果传入的值是id，则解析成的sql为`where username="id"`。<br />2、`$`将传入的数据直接显示生成在sql中。如：`where username=${username}`，如果传入的值是111,那么解析成sql时的值为`where username=111`；如果传入的值是`drop table user;`，则解析成的sql为：`select id, username, password, role from user where username=;drop table user;`<br />3、`#`方式能够很大程度防止SQL注入，`$`方式无法防止SQL注入。<br />4、`$`方式一般用于传入数据库对象，例如传入表名.<br />5、一般能用`#`的就别用`$`，若不得不使用“`${xxx}`”这样的参数，要手工地做好过滤工作，来防止sql注入攻击。<br />6、在MyBatis中，“`${xxx}`”这样格式的参数会直接参与SQL编译，从而不能避免注入攻击。但涉及到动态表名和列名时，只能使用“`${xxx}`”这样的参数格式。所以，这样的参数需要在代码中手工进行处理来防止注入。
:::success
【结论】在编写MyBatis的映射语句时，尽量采用“`#{xxx}`”这样的格式。若不得不使用“`${xxx}`”这样的参数，要手工地做好过滤工作，来防止SQL注入攻击。
:::
<a name="Wu6QL"></a>
### Mybatis是如何做到防止sql注入的
MyBatis框架作为一款半自动化的持久层框架，其SQL语句都要自己手动编写，这个时候当然需要防止SQL注入。其实，MyBatis的SQL是一个具有“**输入+输出**”的功能，类似于函数的结构，参考上面的两个例子。<br />其中，parameterType表示了输入的参数类型，resultType表示了输出的参数类型。回应上文，如果想防止SQL注入，理所当然地要在输入参数上下功夫。上面代码中使用`#`的即输入参数在SQL中拼接的部分，传入参数后，打印出执行的SQL语句，会看到SQL是这样的：
```sql
select id, username, password, role from user where username=? and password=?
```
不管输入什么参数，打印出的SQL都是这样的。这是因为MyBatis启用了预编译功能，在SQL执行前，会先将上面的SQL发送给数据库进行编译；执行时，直接使用编译好的SQL，替换占位符“`?`”就可以了。因为SQL注入只能对编译过程起作用，所以这样的方式就很好地避免了SQL注入的问题。<br />【底层实现原理】MyBatis是如何做到SQL预编译的呢？其实在框架底层，是JDBC中的PreparedStatement类在起作用，PreparedStatement是Statement的子类，它的对象包含了编译好的SQL语句。这种“准备好”的方式不仅能提高安全性，而且在多次执行同一个SQL时，能够提高效率。原因是SQL已编译好，再次执行时无需再编译。
