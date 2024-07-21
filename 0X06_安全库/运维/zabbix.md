
<a name="2933446f"></a>
# 一、安装zabbix闭坑指南

```bash
[root@zabbix-server zabbix]# sed -i 's/SELINUX=enforcing/SELINUX=disabled/' /etc/selinux/config
[root@zabbix-server zabbix]# setenforce 0
[root@zabbix-server zabbix]# systemctl stop firewall.service
```

配置zabbix安装源-------------使用的是阿里的源<br />[https://mirrors.aliyun.com/zabbix/zabbix/5.0/rhel/7/x86_64/](https://mirrors.aliyun.com/zabbix/zabbix/5.0/rhel/7/x86_64/)

![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871568-bfae2d71-2849-44fa-a06b-9f29fa43c95f.png#height=814&id=LIqY2&originHeight=814&originWidth=1153&originalType=binary&ratio=1&size=0&status=done&style=none&width=1153)
<a name="urC4L"></a>
### a、选择你要安装5.x版本我这里选择5.0的版本

```bash
[root@zabbix-server ~]# wget https://mirrors.aliyun.com/zabbix/zabbix/5.0/rhel/7/x86_64/zabbix-release-5.0-1.el7.noarch.rpm
[root@zabbix-server ~]# rpm -ivh zabbix-release-5.0-1.el7.noarch.rpm
```

1、修改下载地址

```bash
[root@zabbix-server ~]# sed -i 's#http://repo.zabbix.com#https://mirrors.aliyun.com/zabbix#' /etc/yum.repos.d/zabbix.repo
```

2、安装zabbix-server 和agent

```bash
[root@zabbix-server ~]# yum install zabbix-server-mysql zabbix-agent
```

3、装 Software Collections，便于后续安装高版本的 php

```bash
[root@zabbix-server ~]# yum install centos-release-scl -y
```

4、启用 zabbix 前端源，修改/etc/yum.repos.d/zabbix.repo,将[zabbix-frontend]下的 enabled 改为 1

```bash
[root@zabbix-server ~]# cat /etc/yum.repos.d/zabbix.repo
[zabbix]
name=Zabbix Official Repository - $basearch
baseurl=https://mirrors.aliyun.com/zabbix/zabbix/5.0/rhel/7/$basearch/
enabled=1
gpgcheck=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-ZABBIX-A14FE591

[zabbix-frontend]
name=Zabbix Official Repository frontend - $basearch
baseurl=https://mirrors.aliyun.com/zabbix/zabbix/5.0/rhel/7/$basearch/frontend
enabled=1
gpgcheck=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-ZABBIX-A14FE591

[zabbix-debuginfo]
name=Zabbix Official Repository debuginfo - $basearch
baseurl=https://mirrors.aliyun.com/zabbix/zabbix/5.0/rhel/7/$basearch/debuginfo/
enabled=0
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-ZABBIX-A14FE591
gpgcheck=1

[zabbix-non-supported]
name=Zabbix Official Repository non-supported - $basearch
baseurl=https://mirrors.aliyun.com/zabbix/non-supported/rhel/7/$basearch/
enabled=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-ZABBIX
gpgcheck=1
```

5、安装 zabbix 前端和相关环境

```bash
[root@zabbix-server ~]# yum install zabbix-web-mysql-scl zabbix-apache-conf-scl -y
```

6、安装数据库和配置数据库

```bash
[root@zabbix-server ~]# yum install mariadb-server -y
[root@zabbix-server ~]# systemctl enable --now mariadb
[root@zabbix-server ~]# mysql_secure_installation
[root@zabbix-server ~]#  mysql -uroot -p
Enter password:
Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 392
Server version: 5.5.65-MariaDB MariaDB Server

Copyright (c) 2000, 2018, Oracle, MariaDB Corporation Ab and others.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

MariaDB [(none)]>

MariaDB [(none)]> create database zabbix character set utf8 collate utf8_bin;
MariaDB [(none)]> create user zabbix@localhost identified by 'password'; #注意修改密码
MariaDB [(none)]> grant all privileges on zabbix.* to zabbix@localhost;
quit;
```

7、修改配置文件中的密码

![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871564-37c0bcb5-9bce-4654-8a9b-9ab144b7b430.png#height=298&id=kQBb5&originHeight=298&originWidth=876&originalType=binary&ratio=1&size=0&status=done&style=none&width=876)

```bash
vi /etc/zabbix/zabbix_server.conf 
DBHost=localhost 

DBName=zabbix

DBUser=zabbix

DBPassword=123456
```

8、使用以下命令导入 zabbix 数据库

```bash
[root@zabbix-server ~]# zcat /usr/share/doc/zabbix-server-mysql*/create.sql.gz | mysql -uzabbix -p password
```

<a name="ul35D"></a>
### b、接下来开始采坑了 这里提示没有选中数据库

```bash
[root@zabbix-server zabbix-server-mysql-5.0.2]# zcat /usr/share/doc/zabbix-server-mysql-5.0.2/create.sql.gz | mysql -uzabbix -ppassword
gzip: /usr/share/doc/zabbix-server-mysql-5.0.2/create.sql.gz: No such file or directory
```

在之前的安装都没出现这个问题现在出现这个如何解决呢？

```bash
[root@zabbix-server ~]# yum -y install nano
Loaded plugins: fastestmirror, langpacks
Loading mirror speeds from cached hostfile
 * base: mirrors.bfsu.edu.cn
 * centos-sclo-rh: mirrors.cqu.edu.cn
 * centos-sclo-sclo: mirrors.bfsu.edu.cn
 * extras: mirrors.cqu.edu.cn
 * updates: mirrors.cqu.edu.cn
Package nano-2.3.1-10.el7.x86_64 already installed and latest version
Nothing to do
[root@zabbix-server ~]# cd /usr/share/doc/zabbix-
zabbix-agent-5.0.2/        zabbix-release-5.0/        zabbix-server-mysql-5.0.2/ zabbix-web-5.0.2/
[root@zabbix-server ~]# cd /usr/share/doc/zabbix-server-mysql-5.0.2/
[root@zabbix-server zabbix-server-mysql-5.0.2]# ls
AUTHORS  ^c  ChangeLog  COPYING  create.sql  create.sql.gz  double.sql  NEWS  README
[root@zabbix-server zabbix-server-mysql-5.0.2]#
```

使用nano打开create.sql 在前面添加一句 use zabbix;<br />在mysql中是不区分大小写的。<br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871604-eef45964-e6af-43cf-aac7-401ef68c3d5a.png#height=413&id=x4DsH&originHeight=413&originWidth=952&originalType=binary&ratio=1&size=0&status=done&style=none&width=952)<br />写好之后按ctrl+ o 保存大写的Y退出<br />最后在导入数据文件

```bash
[root@zabbix-server zabbix-server-mysql-5.0.2]# cat create.sql | mysql -uzabbix -ppassword
```

修改 zabbix 的 php 配置文件 /etc/opt/rh/rh-php72/php-fpm.d/zabbix.conf 里的时区

```bash
php_value[date.timezone] = Asia/Shanghai
```

重启服务

```bash
[root@zabbix-server zabbix-server-mysql-5.0.2]# systemctl restart zabbix-server zabbix-agent httpd rh-php72-php-fpm
[root@zabbix-server zabbix-server-mysql-5.0.2]# systemctl enable zabbix-server zabbix-agent httpd rh-php72-php-fpm
```

<a name="1PldP"></a>
### c、要是启动失败
```bash
[root@zabbix-server ~]#  cat /var/log/messages |grep zabbix
Aug  8 07:23:39 localhost yum[14126]: Installed: zabbix-web-mysql-5.0.23-1.el7.noarch
Aug  8 07:23:40 localhost yum[14126]: Installed: zabbix-web-5.0.23-1.el7.noarch
Aug  8 07:23:41 localhost yum[14126]: Installed: zabbix-server-mysql-5.0.23-1.el7.x86_64
Aug  8 07:28:37 localhost kernel: zabbix_server[14734]: segfault at 18 ip 00007f4d9c32cc00 sp 00007fff684c4028 error 4 in libpthread-2.17.so[7f4d9c323000+16000]
Aug  8 07:28:37 localhost systemd: zabbix-server.service: control process exited, code=killed status=11
Aug  8 07:28:37 localhost systemd: Unit zabbix-server.service entered failed state.
```

是linux内核文件不支持 上传一个低版本的文件替换

**链接**：[https://pan.baidu.com/s/1dmd8IZFArsZaYRSofdD8Aw](https://pan.baidu.com/s/1dmd8IZFArsZaYRSofdD8Aw)<br />提取码：axkv

```bash
[root@zabbix-server ~]# ll gnutls-3.1.18-10.el7_0.x86_64.rpm
-rw-r--r--. 1 root root 623572 Aug  8 07:30 gnutls-3.1.18-10.el7_0.x86_64.rpm
[root@zabbix-server ~]# yum -y remove gnutls-dane-3.3.8-12.el7.x86_64
[root@zabbix-server ~]# rpm -Uvh --force  gnutls-3.1.18-8.el7.x86_64.rpm
```

**然后在重启服务**<br />用浏览器访问IP/zabbix<br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871576-964755d6-1226-4572-bcea-9e5d7d24f523.png#height=627&id=IkfkW&originHeight=627&originWidth=1046&originalType=binary&ratio=1&size=0&status=done&style=none&width=1046)<br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871560-599db48d-ed6a-41d0-8250-1e5298371e67.png#height=574&id=nTtES&originHeight=574&originWidth=1060&originalType=binary&ratio=1&size=0&status=done&style=none&width=1060)<br />最后一步出现![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871548-3c62ec6a-89cd-4023-9960-a8716ec17be0.png#height=557&id=BEd4P&originHeight=557&originWidth=918&originalType=binary&ratio=1&size=0&status=done&style=none&width=918)<br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871582-fb2b9ade-aba7-476c-b212-53ecdd0d91dc.png#height=656&id=qrrRD&originHeight=656&originWidth=1639&originalType=binary&ratio=1&size=0&status=done&style=none&width=1639)

提示我们没有权限去创建文件我们只好下载好后上传到指定的位置并赋予相当的权限<br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871598-572cce43-d4e8-4b66-a65b-2e4dddbb0be2.png#height=659&id=ZWKFm&originHeight=659&originWidth=1122&originalType=binary&ratio=1&size=0&status=done&style=none&width=1122)<br />下载好只好上传到/etc/zabbix/web/zabbix.conf.php然后给个766的权限就可以了

```bash
[root@zabbix-server zabbix]# mv zabbix.conf.php /etc/zabbix/web/
```

![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871606-24eba944-a6c8-4d94-ba1a-2eb09051e4a5.png#height=533&id=gCgOY&originHeight=533&originWidth=628&originalType=binary&ratio=1&size=0&status=done&style=none&width=628)<br />默认的账户密码是Admin zabbix

添加一台监控主机<br />可以直接通过下载一个agent文件然后离线安装

```bash
[root@centos ~]# wget https://mirrors.aliyun.com/zabbix/zabbix/5.0/rhel/7/x86_64/zabbix-agent-5.0.1-1.el7.x86_64.rpm
--2020-08-20 04:50:19--  https://mirrors.aliyun.com/zabbix/zabbix/5.0/rhel/7/x86_64/zabbix-agent-5.0.1-1.el7.x86_64.rpm
Resolving mirrors.aliyun.com (mirrors.aliyun.com)... 36.147.7.241, 117.157.246.115, 111.51.68.248, ...
Connecting to mirrors.aliyun.com (mirrors.aliyun.com)|36.147.7.241|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 452484 (442K) [application/x-redhat-package-manager]
Saving to: ‘zabbix-agent-5.0.1-1.el7.x86_64.rpm’

100%[=========================================================================================================================>] 452,484     1.89MB/s   in 0.2s

2020-08-20 04:50:20 (1.89 MB/s) - ‘zabbix-agent-5.0.1-1.el7.x86_64.rpm’ saved [452484/452484]
[root@centos ~]# rpm -ivh zabbix-agent-5.0.1-1.el7.x86_64.rpm
warning: zabbix-agent-5.0.1-1.el7.x86_64.rpm: Header V4 RSA/SHA512 Signature, key ID a14fe591: NOKEY
Preparing...                          ################################# [100%]
Updating / installing...
   1:zabbix-agent-5.0.1-1.el7         ################################# [100%]
```

指定zabbix-server的IP地址重启服务

```bash
[root@centos ~]# grep "192" /etc/zabbix/zabbix_agentd.conf
#       Example: Server=127.0.0.1,192.168.1.0/24,::1,2001:db8::/32,zabbix.example.com
Server=192.168.40.23
[root@centos ~]#
[root@centos ~]# systemctl restart zabbix-agent
.service
```

在web界面上添加一台监控的机器<br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871587-9d80598e-1dea-4195-bd23-f9cc8ecfc07a.png#height=696&id=yz2Sp&originHeight=696&originWidth=1796&originalType=binary&ratio=1&size=0&status=done&style=none&width=1796)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1602924314991-070bf8b3-56a5-461b-99a4-e3981e9b0417.png#height=130&id=ZxiYP&originHeight=259&originWidth=1268&originalType=binary&ratio=1&size=22130&status=done&style=none&width=634)<br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871558-738d7d76-a5e7-4643-80a1-95882a8ae9b0.png#height=98&id=dTKTK&originHeight=98&originWidth=1125&originalType=binary&ratio=1&size=0&status=done&style=none&width=1125)<br />**备注：记得关闭agent机器上面的防火墙和selinux不然的话很可能由于这些原因连接不上zabbix-server**<br />![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599916871640-96501784-2c7a-4ea9-9bc6-c314ab6f6ba0.png#height=174&id=Uv018&originHeight=174&originWidth=1667&originalType=binary&ratio=1&size=0&status=done&style=none&width=1667)
<a name="ifnJ5"></a>
## 二、zabbix报警媒介
<a name="MIu8b"></a>
### a、钉钉报警

1、钉钉报警监控机器人<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603006993589-43faf241-5d91-4a2f-a9f4-d2e076c59566.png#height=341&id=a4EEd&originHeight=681&originWidth=845&originalType=binary&ratio=1&size=43251&status=done&style=none&width=422.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603006725062-0e00aa74-9756-4aea-bd99-127575706d79.png#height=362&id=PacF5&originHeight=723&originWidth=887&originalType=binary&ratio=1&size=50787&status=done&style=none&width=443.5)<br />2、添加报警代码并且测试
```c
[root@zabbix-server alertscripts]# pwd
/usr/lib/zabbix/alertscripts
[root@zabbix-server alertscripts]# cat talk2.py
[root@zabbix-server alertscripts]# cat talk2.py
#!/usr/bin/python3
# -*- coding: utf-8 -*-

import requests
import json
import sys
import os

headers = {'Content-Type': 'application/json;charset=utf-8'}
api_url ="https://oapi.dingtalk.com/robot/send?access_token=499d700b8ed06922cf52acb2d78770790f196cc504321e752ea0fff01d4e9b3c"

def msg(text):
        json_text= {
        "msgtype": "text",
                "at": {"atMobiles": [
                                "18428312380"    #被@的手机号通过手机号确定用户
                        ],
                        "isAtAll": False         #是否@全体成员 True为@所有人
                },
                "text": {
                        "content": text
                }
        }
        s=json.dumps(json_text)
        print(requests.post(api_url,data=s,headers=headers).content)

if __name__ == '__main__':
        text = sys.argv[1]
        msg(text)

[root@zabbix-server alertscripts]# chmod +x talk2.py
[root@zabbix-server alertscripts]# ./talk2.py 钉钉报警测试
b'{"errcode":0,"errmsg":"ok"}'
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603007806255-c6b5c386-3d44-4dda-bbc8-0fb9d653d913.png#height=99&id=ZdIxM&originHeight=198&originWidth=555&originalType=binary&ratio=1&size=14445&status=done&style=none&width=277.5)
<a name="d6e6D"></a>
### b、邮箱报警
**1、注册一个邮箱建议不要使用qq邮箱、我在做实验的时候qq邮箱无论你怎么设置最后都是失败最后换成163就成功了**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603008282147-937850ba-7850-4d3d-8243-8901247decda.png#height=287&id=P1X9I&originHeight=573&originWidth=604&originalType=binary&ratio=1&size=169844&status=done&style=none&width=302)<br />**2、开通SMTP邮箱的授权码**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603008325422-341b3ba6-53c1-4d4f-99db-3b58042f6025.png#height=225&id=nNdjq&originHeight=450&originWidth=652&originalType=binary&ratio=1&size=35424&status=done&style=none&width=326)<br />**3、配置授权码信息**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603008336841-29928ad0-3ea1-4ced-921e-754657a07203.png#height=130&id=VDSjX&originHeight=259&originWidth=857&originalType=binary&ratio=1&size=22003&status=done&style=none&width=428.5)<br />**4、配置zabbix的邮箱报警信息**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603008186956-7885d433-e541-48c6-81ae-7f973bc074b4.png#height=421&id=BvMPC&originHeight=842&originWidth=1084&originalType=binary&ratio=1&size=58338&status=done&style=none&width=542)<br />**5、zabbix邮箱报警测试**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603008503385-047c500d-8cc8-4f5a-85e0-e8219b20a7e7.png#height=193&id=yspAm&originHeight=386&originWidth=775&originalType=binary&ratio=1&size=17034&status=done&style=none&width=387.5)<br />**6、验证收到zabbix端信息**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603008527380-7c68b752-9bc8-4361-8a14-77e6c0d671d7.png#height=150&id=MlyVv&originHeight=299&originWidth=988&originalType=binary&ratio=1&size=21282&status=done&style=none&width=494)
<a name="YuJRV"></a>
### c、微信报警
























