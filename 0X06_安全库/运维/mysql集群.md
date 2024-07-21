<a name="jHOMP"></a>
## 内容：

- 分析目前系统架构中的数据库层存在的问题
- 分析MySQL数据库的集群方案
- 学习主从复制（读写分离）架构方案掌握MyCat数据库中间件的使用
- 掌握HAProxy复制均衡的使用
- 掌握PXC集群的使用
- 多种集群架构的综合应用

<a name="ZWuGG"></a>
## mysql忘记密码
```c
[root@mysql-slave ~]# mysql -uroot -pjiang
ERROR 1045 (28000): Access denied for user 'root'@'localhost' (using password: YES)
[root@mysql-slave ~]# systemctl stop mariadb
[root@mysql-slave ~]# mysqld_safe --skip-grant-tables --skip-networking &
[1] 13966
[root@mysql-slave ~]# 200916 21:03:59 mysqld_safe Logging to '/var/log/mariadb/mariadb.log'.
200916 21:03:59 mysqld_safe Starting mysqld daemon with databases from /var/lib/mysql

[root@mysql-slave ~]# mysql -uroot
Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 3
Server version: 5.5.65-MariaDB MariaDB Server

Copyright (c) 2000, 2018, Oracle, MariaDB Corporation Ab and others.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

MariaDB [(none)]> use mysql;
MariaDB [mysql]> update user set password=password("jiang") where user="root";
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0

MariaDB [mysql]>
[root@mysql-slave ~]# systemctl restart mariadb
Job for mariadb.service failed because the control process exited with error code. See "systemctl status mariadb.service" and "journalctl -xe" for details.
[root@mysql-slave ~]#
[root@mysql-slave ~]# systemctl status mariadb
● mariadb.service - MariaDB database server
   Loaded: loaded (/usr/lib/systemd/system/mariadb.service; enabled; vendor preset: disabled)
   Active: failed (Result: exit-code) since Wed 2020-09-16 21:02:14 EDT; 12s ago
  Process: 12886 ExecStartPost=/usr/libexec/mariadb-wait-ready $MAINPID (code=exited, status=0/SUCCESS)
  Process: 12885 ExecStart=/usr/bin/mysqld_safe --basedir=/usr (code=exited, status=0/SUCCESS)
  Process: 13621 ExecStartPre=/usr/libexec/mariadb-prepare-db-dir %n (code=exited, status=1/FAILURE)
 Main PID: 12885 (code=exited, status=0/SUCCESS)

Sep 16 21:02:14 mysql-slave systemd[1]: Starting MariaDB database server...
Sep 16 21:02:14 mysql-slave systemd[1]: mariadb.service: control process exited, code=exited status=1
Sep 16 21:02:14 mysql-slave systemd[1]: Failed to start MariaDB database server.
Sep 16 21:02:14 mysql-slave systemd[1]: Unit mariadb.service entered failed state.
Sep 16 21:02:14 mysql-slave systemd[1]: mariadb.service failed.
[root@mysql-slave ~]# ps -ef|grep mysqld
root      13353  12087  0 21:00 pts/0    00:00:00 /bin/sh /usr/bin/mysqld_safe --skip-grant-tables --skip-networking
mysql     13538  13353  0 21:00 pts/0    00:00:00 /usr/libexec/mysqld --basedir=/usr --datadir=/var/lib/mysql --plugin-dir=/usr/lib64/mysql/plugin --user=mysql --skip-grant-tables --skip-networking --log-error=/var/log/mariadb/mariadb.log --pid-file=/var/run/mariadb/mariadb.pid --socket=/var/lib/mysql/mysql.sock
root      13662  12087  0 21:02 pts/0    00:00:00 grep --color=auto mysqld
[root@mysql-slave ~]# kill -9 13353
[root@mysql-slave ~]# kill -9 13538
[1]+  Killed                  mysqld_safe --skip-grant-tables --skip-networking
[root@mysql-slave ~]# systemctl restart mariadb
```


重启mysql若发现一直卡住没法启动或者启动的时候直接报错则查看mysql的进程将进程杀掉之后重启服务。
<a name="0DD3g"></a>
## 
<a name="mYTJO"></a>
## 数据库存在的问题

DBserver方面我们只是使用了单节点服务，如果面对大并发，海量数据的存储，显然单节点的系统架构将存在很严重的问题，所以接下来，我们将实现MySQL的集群，来应对大并发、海量数据存储等问题。
<a name="Futpx"></a>
## 
<a name="UapX1"></a>
### 读写分离
我们一般应用对数据库而言都是“读多写少”，也就说对数据库读取数据的压力比较大，有一个思路就是说采用数据库集群的方案：其中一个是主库，负责写入数据，我们称之为：写库；其它都是从库，负责读取数据，我们称之为：读库；

1. 读库和写库的数据一致；<br />2. 写数据必须写到写库；<br />3. 读数据必须到读库；

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600238945479-6ffb0949-2ca4-4c2c-89ce-fb4cf2be8c03.png#align=left&display=inline&height=277&originHeight=554&originWidth=973&size=38210&status=done&style=none&width=486.5)<br />**缺点：**

1. 主从之间的同步，是异步完成，也就意味着这是弱一致性
2. 可能会导致，数据写入主库后，应用程序读取从库获取不到数据，或者可能会丢失数据，对于数据安全性要求比较高的应用是不合适的
3. 该问题可以通过PXC集群解决
<a name="Ps9Wa"></a>
### 中间件
应用程序只需要连接到中间件即可，无需连接多个数据库节点应用程序无需区分读写操作，对中间件直接进行读写操作即可在中间件中进行区分读写操作，读发送到从节点，写发送到主节点<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600240659860-c1f5ff06-d422-472d-9b5f-b360c02fc3d9.png#align=left&display=inline&height=312&originHeight=623&originWidth=751&size=42825&status=done&style=none&width=375.5)<br />该架构也存在问题，中间件的性能成为了系统的瓶颈<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600240846040-d9226e32-4b97-450a-8faa-6a19ee99d0b6.png#align=left&display=inline&height=302&originHeight=604&originWidth=678&size=44562&status=done&style=none&width=339)<br />这样的话，中间件的可靠性得到了保证，但是也带来了新的问题，应用系统依然是需要连接到2个中间件，又为应用系统带来了复杂度。
<a name="0sepi"></a>
### 负载均衡
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600241594693-0f5432a5-a3ae-4d35-a696-a9e8bdcc1743.png#align=left&display=inline&height=262&originHeight=524&originWidth=732&size=43747&status=done&style=none&width=366)<br />为了解决以上问题，我们将继续优化架构，在应用程序和中间件之间增加proxy代理，由代理来完成负载均衡的功能，应用程序只需要对接到proxy即可。<br />至此，主从复制架构的高可用架构才算是搭建完成。
<a name="ddf8Q"></a>
### pxc集群
在前面的架构中，都是基于MySQL主从的架构，那么在主从架构中，弱一致性问题依然没有解决，如果在需要强一致性的需求中，显然这种架构是不能应对的，比如：交易数据。PXC提供了读写强一致性的功能，可以保证数据在任何一个节点写入的同时可以同步到其它节点，也就意味着可以存其它的任何节点进行读取操作，无延迟。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600242213308-7fb7cf4d-2b7c-4776-8605-707b1176443d.png#align=left&display=inline&height=320&originHeight=639&originWidth=639&size=46048&status=done&style=none&width=319.5)

<a name="2VuXA"></a>
## 环境介绍
| 主机名 | 角色 |
| --- | --- |
| mysql-master | master |
| mysql-slave | slave |

<a name="Z0Q0b"></a>
### 一主一从配置
<a name="sZ6RP"></a>
#### 两台服务器都开启log-bin日志
```c
[root@mysql-master ~]# cat /etc/my.cnf
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
log-bin=mysql-bin			#开启log-bin日志功能
server-id=23				#服务器id号唯一
# Settings user and group are ignored when systemd is used.
# If you need to run mysqld under a different user or group,
# customize your systemd unit file for mariadb according to the
# instructions in http://fedoraproject.org/wiki/Systemd

[mysqld_safe]
log-error=/var/log/mariadb/mariadb.log
pid-file=/var/run/mariadb/mariadb.pid

#
# include all files from the config directory
#
!includedir /etc/my.cnf.d

[root@mysql-master ~]# systemctl restart mariadb
```
<a name="fK7EI"></a>
#### 主服务器配置
```c
MariaDB [(none)]> grant replication slave on *.* to slave@'192.168.40.24' identified by "123456";
Query OK, 0 rows affected (0.03 sec)

MariaDB [(none)]> show master status;
+------------------+----------+--------------+------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin.000001 |   137891 |              |                  |
+------------------+----------+--------------+------------------+
1 row in set (0.00 sec)

MariaDB [(none)]>
```
<a name="O18va"></a>
#### 从服务器配置
```c
MariaDB [(none)]> change master to master_user='slave',master_password='123456',master_host="192.168.40.23",master_log_file="mysql-bin.000001",master_log_pos=137891;
Query OK, 0 rows affected (0.00 sec)

MariaDB [(none)]>
[root@mysql-slave ~]# cat /var/lib/mysql/master.info 		#查看同步到主服务器的授权信息
18
mysql-bin.000001
137891
192.168.40.23
slave
123456
3306
60
0





0
1800.000

0
MariaDB [(none)]> slave start;
Query OK, 0 rows affected (0.00 sec)

MariaDB [(none)]> show slave status\G;
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
                  Master_Host: 192.168.40.23
                  Master_User: slave
                  Master_Port: 3306
                Connect_Retry: 60
              Master_Log_File: mysql-bin.000001
          Read_Master_Log_Pos: 958836
               Relay_Log_File: mariadb-relay-bin.000002
                Relay_Log_Pos: 821474
        Relay_Master_Log_File: mysql-bin.000001
             Slave_IO_Running: Yes		#连接到主库，并读取主库的日志到本地，生成本地日志文件。一定要是yes的状态
            Slave_SQL_Running: Yes		#读取本地日志文件，并执行日志里的SQL命令。一定要是yes的状态如果出现NO的解决方法 mysql> slave stop; mysql> set GLOBAL SQL_SLAVE_SKIP_COUNTER=1; mysql> slave start;
MariaDB [(none)]> show databases;	#查看是否同步到主服务器的信息
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| zabbix             |
+--------------------+
4 rows in set (0.00 sec)

MariaDB [(none)]>

```
**备注：**要是做一主多从可以master节点上面对另外的机器授权从服务器进行接收<br />**<br />**在主服务器中查看模式**
```c
MariaDB [mysql]> show global variables like'binlog%';
+-----------------------------------------+-----------+
| Variable_name                           | Value     |
+-----------------------------------------+-----------+
| binlog_annotate_row_events              | OFF       |
| binlog_cache_size                       | 32768     |
| binlog_checksum                         | NONE      |
| binlog_direct_non_transactional_updates | OFF       |
| binlog_format                           | STATEMENT |
| binlog_optimize_thread_scheduling       | ON        |
| binlog_stmt_cache_size                  | 32768     |
+-----------------------------------------+-----------+
7 rows in set (0.00 sec)

MariaDB [mysql]>
```
在查看二进制日志相关参数内容中，会发现默认的模式为ROW，其实在MySQL中提供了有3种模式，基于SQL语句的复制(statement-based replication, SBR)，基于行的复制(row-based replication, RBR)，混合模式复制(mixed-based replication, MBR)。对应的，binlog的格式也有三种：STATEMENT，ROW，MIXED。

**STATEMENT模式（SBR）<br />**<br />每一条会修改数据的sql语句会记录到binlog中。<br />优点是并不需要记录每一条sql语句和每一行的数据变化，减少了binlog日志量，节约IO，提高性能。<br />缺点是在某些情况下会导致master-slave中的数据不一致(如sleep()函数， last_insert_id()，以及user-definedfunctions(udf)等会出现问题)

**ROW模式（RBR**）<br />不记录每条sql语句的上下文信息，仅需记录哪条数据被修改了，修改成什么样了。而且不会出现某些特定情况下的存储过程、或function、或trigger的调用和触发无法被正确复制的问题。缺点是会产生大量的日志，尤其是altertable的时候会让日志暴涨。<br />**<br />**MIXED模式（MBR）<br />**<br />以上两种模式的混合使用，一般的复制使用STATEMENT模式保存binlog，对于STATEMENT模式无法复制的操作使用ROW模式保存binlog，MySQL会根据执行的SQL语句选择日志保存方式。建议使用MIXED模式。<br />建议使用MIXED模式

<a name="Fuj3z"></a>
## MyCat中间件

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1600248151637-50249aaf-937c-49e7-971c-6fdd8f8430b5.png#align=left&display=inline&height=264&originHeight=528&originWidth=1233&size=78985&status=done&style=none&width=616.5)<br />[http://www.mycat.org.cn/](http://www.mycat.org.cn/)

<a name="bjD5D"></a>
### 优势

一个彻底开源的，面向企业应用开发的大数据库集群<br />支持事务、ACID、可以替代MySQL的加强版数据库<br />一个可以视为MySQL集群的企业级数据库，用来替代昂贵的Oracle集群<br />一个融合内存缓存技术、NoSQL技术、HDFS大数据的新型SQL Server结合传统数据库和新型分布式数据仓库的新一代企业级数据库产品<br />一个新颖的数据库中间件产品

<a name="xc6z2"></a>
### 功能

1.垂直分片 水平分片<br />2.分表 分库<br />3.逻辑表 逻辑库<br />4.心跳检测  读写分离<br />5.分片字段 分片规则 SQL路由

<a name="LfgEj"></a>
### 安装
上传二进制文件到/opt目录解压然后移动到/usr/local
```c
[root@mysql-master opt]# tar -xf Mycat-server-1.6.7.5-release-20200422133810-linux.tar.gz
[root@mysql-master opt]# cp -r mycat/ /usr/local/
[root@mysql-master opt]# cd /usr/local/
[root@mysql-master local]# ll
total 0
drwxr-xr-x. 2 root root  6 Jun  9  2014 bin
drwxr-xr-x. 2 root root  6 Jun  9  2014 etc
drwxr-xr-x. 2 root root  6 Jun  9  2014 games
drwxr-xr-x. 2 root root  6 Jun  9  2014 include
drwxr-xr-x. 2 root root  6 Jun  9  2014 lib
drwxr-xr-x. 2 root root  6 Jun  9  2014 lib64
drwxr-xr-x. 2 root root  6 Jun  9  2014 libexec
drwxr-xr-x. 6 root root 50 Sep 16 08:49 mycat
drwxr-xr-x. 2 root root  6 Jun  9  2014 sbin
drwxr-xr-x. 5 root root 46 Mar  2  2020 share
drwxr-xr-x. 2 root root  6 Jun  9  2014 src
[root@mysql-master local]#
[root@mysql-master mycat]# ls
bin  catlet  conf  lib
[root@mysql-master mycat]#
```
