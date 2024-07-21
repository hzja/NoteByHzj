CentOS7 Zabbix4.0
<a name="da09dc64"></a>
### zabbix介绍
Zabbix 是一个基于 WEB 界面的提供分布式系统监视以及网络监视功能的企业级的开源解决方案。它能监视各种网络参数，保证服务器系统的安全运营；并提供灵活的通知机制以让系统管理员快速定位/解决存在的各种问题。
<a name="2baebd9c"></a>
### zabbix软件组成

- zabbix-server: 监控服务端<br />
- zabbix-agent: 监控客户端<br />
- zabbix-web: 监控网站服务<br />
- php: 处理动态请求<br />
- mysql: 数据库存储监控数据<br />
- zabbix: 负责收集agent信息汇总告知zabbix-server<br />
<a name="1663c88c"></a>
### 关闭防火墙和selinux
```bash
[root@zabbix ~]# setenforce 0
[root@zabbix ~]# sed -ri '/^SELINUX=/cSELINUX=disabled' /etc/sysconfig/selinux 
[root@zabbix ~]# systemctl stop firewalld ; systemctl disable firewalld
```
<a name="176c5037"></a>
### 添加zabbix源和epel源
```bash
## 这里使用的是清华源
[root@zabbix ~]# rpm -ivh https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/4.0/rhel/7/x86_64/zabbix-release-4.0-1.el7.noarch.rpm 
 ## 添加epel源
[root@zabbix ~]# wget -O /etc/yum.repos.d/epel.repo http://mirrors.aliyun.com/repo/epel-7.repo
```
<a name="845a1060"></a>
### 安装zabbix和相关服务组件
```bash
## 把这两个包先拿下来,因为安装的时候网络问题会出现安装超时
[root@zabbix ~]# wget https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/4.0/rhel/7/x86_64/{zabbix-server-mysql-4.0.20-1.el7.x86_64.rpm,zabbix-web-4.0.22-1.el7.noarch.rpm} 
## 安装zabbix的服务程序和web程序
[root@zabbix ~]# yum install -y httpd php zabbix-server-mysql-4.0.20-1.el7.x86_64.rpm zabbix-web-mysql 
## 安装数据库服务
[root@zabbix ~]# yum install -y mariadb-server 
```
<a name="8da3bc65"></a>
### 修改zabbix配置文件
```bash
## 修改zabbix数据库密码为zabbix
[root@zabbix ~]# sed -ri.bak '/# DBPassword=/cDBPassword=zabbix' /etc/zabbix/zabbix_server.conf 
## 修改时区为国内
[root@zabbix ~]# sed -ri.bak 's/ #(.*)date.timezone.*/\1date.timezone Asia\/Shanghai/' /etc/httpd/conf.d/zabbix.conf 
```
<a name="6cde5efd"></a>
### 配置数据库服务
```bash
## 启动数据库
[root@zabbix ~]# systemctl start mariadb 
## 初始化数据库
[root@zabbix ~]# mysql_secure_installation 
NOTE: RUNNING ALL PARTS OF THIS SCRIPT IS RECOMMENDED FOR ALL MariaDB
      SERVERS IN PRODUCTION USE!  PLEASE READ EACH STEP CAREFULLY!
In order to log into MariaDB to secure it, we'll need the current
password for the root user.  If you've just installed MariaDB, and
you haven't set the root password yet, the password will be blank,
so you should just press enter here.
Enter current password for root (enter for none): 
OK, successfully used password, moving on...
Setting the root password ensures that nobody can log into the MariaDB
root user without the proper authorisation.
Set root password? [Y/n] y
New password: 
Re-enter new password: 
Password updated successfully!
Reloading privilege tables..
 ... Success!
By default, a MariaDB installation has an anonymous user, allowing anyone
to log into MariaDB without having to have a user account created for
them.  This is intended only for testing, and to make the installation
go a bit smoother.  You should remove them before moving into a
production environment.
Remove anonymous users? [Y/n] y
 ... Success!
Normally, root should only be allowed to connect from 'localhost'.  This
ensures that someone cannot guess at the root password from the network.
Disallow root login remotely? [Y/n] y
 ... Success!
By default, MariaDB comes with a database named 'test' that anyone can
access.  This is also intended only for testing, and should be removed
before moving into a production environment.
Remove test database and access to it? [Y/n] n
 ... skipping.
Reloading the privilege tables will ensure that all changes made so far
will take effect immediately.
Reload privilege tables now? [Y/n] y
 ... Success!
Cleaning up...
All done!  If you've completed all of the above steps, your MariaDB
installation should now be secure.
Thanks for using MariaDB!
[root@zabbix ~]# mysql -uroot -p ##登录数据库
Enter password: 
Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 8
Server version: 5.5.65-MariaDB MariaDB Server
Copyright (c) 2000, 2018, Oracle, MariaDB Corporation Ab and others.
Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
MariaDB [(none)]> create database zabbix character set utf8 collate utf8_bin; ##创建zabbix数据库 
Query OK, 1 row affected (0.00 sec)
MariaDB [(none)]> grant all privileges on zabbix.* to zabbix@localhost identified by 'zabbix'; ##创建数据库管理用户
Query OK, 0 rows affected (0.00 sec)
MariaDB [(none)]> quit
Bye
[root@zabbix ~]# zcat /usr/share/doc/zabbix-server-mysql-4.0.20/create.sql.gz  | mysql -uzabbix -pzabbix zabbix  ##将数据表写入到zabbix库中
```
<a name="dc7a8913"></a>
### 启动zabbix和相关服务
```
[root@zabbix ~]# systemctl start zabbix-server.service httpd mariadb.service
[root@zabbix ~]# systemctl enable zabbix-server.service httpd mariadb.service
Created symlink from /etc/systemd/system/multi-user.target.wants/zabbix-server.service to /usr/lib/systemd/system/zabbix-server.service.
Created symlink from /etc/systemd/system/multi-user.target.wants/httpd.service to /usr/lib/systemd/system/httpd.service.
Created symlink from /etc/systemd/system/multi-user.target.wants/mariadb.service to /usr/lib/systemd/system/mariadb.service.
```
<a name="fe718dfc"></a>
### 登录zabbix的web界面,进行初始化
初始化地址[http://172.16.210.56/zabbix/setup.php](http://172.16.210.56/zabbix/setup.php)<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307878-0ce96e02-2af8-4c85-a820-e95e0ac2dcfc.png#align=left&display=inline&height=515&originHeight=515&originWidth=883&size=0&status=done&style=none&width=677)<br />点击Next step<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307873-6724f6ee-bd97-49c8-b7b4-8604db0870a9.png#align=left&display=inline&height=461&originHeight=461&originWidth=783&size=0&status=done&style=none&width=783)<br />检查php的一些变量条件,如果没有报错的信息可以点击next step<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307858-a77b2337-34d6-4e4e-b187-2f7f0dc21840.png#align=left&display=inline&height=549&originHeight=549&originWidth=904&size=0&status=done&style=none&width=904)<br />填写好对应的数据库密码和帐号点击Next step<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307870-e13eb865-b55e-4a6e-b085-374836a00600.png#align=left&display=inline&height=618&originHeight=618&originWidth=1032&size=0&status=done&style=none&width=1032)<br />设置主机和端口还有监控页面名字,然后点击Next step<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307898-36f1746e-74ec-4c0e-9329-e21d4aac2fc0.png#align=left&display=inline&height=508&originHeight=508&originWidth=880&size=0&status=done&style=none&width=677)<br />确认信息.然后点击Next step<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307902-5a2e2c9d-50c0-4613-828b-cdaa5264fbc7.png#align=left&display=inline&height=533&originHeight=533&originWidth=880&size=0&status=done&style=none&width=677)<br />点击Finish<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307906-932f1c51-8e4d-42cb-a6e5-d1420904e505.png#align=left&display=inline&height=380&originHeight=380&originWidth=401&size=0&status=done&style=none&width=677)<br />帐号Admin 密码zabbix ,再点Sign in<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307922-4b541f97-3732-49f4-94ef-44e6c78161af.png#align=left&display=inline&height=587&originHeight=587&originWidth=1080&size=0&status=done&style=none&width=677)<br />登录成功。
<a name="eee1c0c4"></a>
### 更改zabbix界面语言为中文
点击右上角的小人<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307864-a0efab56-47b5-4669-bc99-42e89a6c84c1.png#align=left&display=inline&height=485&originHeight=485&originWidth=414&size=0&status=done&style=none&width=677)<br />选择Chinese<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307925-f34d67e5-ba22-46a7-9b7f-10892e93ec56.png#align=left&display=inline&height=369&originHeight=369&originWidth=314&size=0&status=done&style=none&width=677)<br />再点击update<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602834307947-c81db55a-e5e1-484f-bd19-0bbb3887b1e5.png#align=left&display=inline&height=653&originHeight=653&originWidth=1080&size=0&status=done&style=none&width=1080)<br />更改成功!
