<a name="A1WsA"></a>
# 一、客户端的连接方式
<a name="80XRh"></a>
## 1、基于密码的连接方式
因为ansible是基于ssh协议进行连接所以在连接的时候一般要进行身份验证、验证的方式一般有两种密码和秘钥的连接方式。
```c
[root@ansible project]# pwd
/home/jiang/project
[root@ansible project]# ls
hosts
[root@ansible project]# cat hosts
[web]
192.168.122.115 ansible_ssh_user=root ansible_ssh_pass='jiang'
192.168.122.20  ansible_ssh_user=root ansible_ssh_pass='123'
[root@ansible project]# ansible web -m ping -i hosts
192.168.122.20 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}
192.168.122.115 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}
[root@ansible project]#

```
<a name="W1pOE"></a>
## 2、基于秘钥的方式进行连接
```c
[root@ansible project]# ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/root/.ssh/id_rsa):
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /root/.ssh/id_rsa.
Your public key has been saved in /root/.ssh/id_rsa.pub.
The key fingerprint is:
SHA256:kxnr2Gg47hnJBpa0KsPb5E+qpuzAL0WBSuwOBXtYdks root@ansible
The key's randomart image is:
+---[RSA 2048]----+
|o.+ E            |
| O.+ .           |
|*.o o   .        |
|o+ +     =       |
|o *     S        |
|o+ + o = .       |
|=o..B.+ o        |
|++*oo=           |
|==o*=.           |
+----[SHA256]-----+
[root@ansible project]# sshpass -p jiang ssh-copy-id 192.168.122.115
/usr/bin/ssh-copy-id: INFO: Source of key(s) to be installed: "/root/.ssh/id_rsa.pub"
/usr/bin/ssh-copy-id: INFO: attempting to log in with the new key(s), to filter out any that are already installed
/usr/bin/ssh-copy-id: INFO: 1 key(s) remain to be installed -- if you are prompted now it is to install the new keys

Number of key(s) added: 1

Now try logging into the machine, with:   "ssh '192.168.122.115'"
and check to make sure that only the key(s) you wanted were added.

[root@ansible project]# sshpass -p 123 ssh-copy-id 192.168.122.20
/usr/bin/ssh-copy-id: INFO: Source of key(s) to be installed: "/root/.ssh/id_rsa.pub"
/usr/bin/ssh-copy-id: INFO: attempting to log in with the new key(s), to filter out any that are already installed
/usr/bin/ssh-copy-id: INFO: 1 key(s) remain to be installed -- if you are prompted now it is to install the new keys

Number of key(s) added: 1

Now try logging into the machine, with:   "ssh '192.168.122.20'"
and check to make sure that only the key(s) you wanted were added.


[root@ansible project]# ansible web -m ping -i hosts
192.168.122.20 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}
192.168.122.115 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}
[root@ansible project]#

```
<a name="gDync"></a>
## 3、以别名的方式进行返回信息
```c
[root@ansible project]# cat hosts
[web]
web01 ansible_ssh_host=192.168.122.115
web02 ansible_ssh_host=192.168.122.20
[root@ansible project]# ansible web -m ping -i hosts
web02 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}
web01 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}
[root@ansible project]#
#注意现在的返回值就是别名而不是返回的ip地址

```
<a name="wKvrl"></a>
## 4、列出主机组当中的所有主机
```c
[root@ansible project]# cat hosts
[nginx]
192.168.122.115
192.168.122.20

[apache]
192.168.122.115
192.168.122.20
192.168.122.116
192.168.122.22
[root@ansible project]# ansible apache -i hosts --list-hosts
  hosts (4):
    192.168.122.115
    192.168.122.20
    192.168.122.116
    192.168.122.22
[root@ansible project]# ansible nginx -i hosts --list-hosts
  hosts (2):
    192.168.122.115
    192.168.122.20
[root@ansible project]# ansible all -i hosts --list-hosts
  hosts (4):
    192.168.122.115
    192.168.122.20
    192.168.122.116
    192.168.122.22
[root@ansible project]#
```
<a name="vRTps"></a>
## 5、取消ssh认证受认信息
在第一次通信的时候ssh会让我们输入一个yes来确认连接端是否合法，但是在大量的主机当中我们挨个输入yes是不现实的、这个时候我们应该取消ansible的ssh连接确认
```c
[root@ansible project]# > /root/.ssh/known_hosts
[root@ansible project]# cat /root/.ssh/known_hosts
[root@ansible project]# ansible web -m ping -i hosts
The authenticity of host '192.168.122.115 (192.168.122.115)' can't be established.
ECDSA key fingerprint is SHA256:w0xe0m1VJLPGngpi4fl8mOFhTMpLuCap8aCdtq0LSGg.
ECDSA key fingerprint is MD5:7c:d3:2f:f0:bf:a3:c3:d1:8f:da:bd:67:7b:40:f3:14.
Are you sure you want to continue connecting (yes/no)? The authenticity of host '192.168.122.20 (192.168.122.20)' can't be established.
ECDSA key fingerprint is SHA256:b91hBKmyfqcLGhYZFC2cjqiFqsiHSGVBWVG7AqrycaQ.
ECDSA key fingerprint is MD5:61:64:cf:10:f0:bb:17:52:a8:60:b3:93:1f:b7:33:0f.
Are you sure you want to continue connecting (yes/no)? yes
192.168.122.115 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}

192.168.122.20 | UNREACHABLE! => {
    "changed": false,
    "msg": "Failed to connect to the host via ssh: Host key verification failed.",
    "unreachable": true
}
[root@ansible project]#
[root@ansible project]# cat /root/.ssh/known_hosts		#这个时候我们发现要是刚才按了yes的那台主机的ssh秘钥信息是被保存到本地的没有保存到本地的是没法正常通信的
192.168.122.115 ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBBp2uq11lJIz+a3RZ78cWp0UpzICCNNSq/blgn6TbRvmaiwIFQP+ylERD76lJUcPfbhbIzDRYsSrVjPrCla7qLQ=
[root@ansible project]# grep ^host /etc/ansible/ansible.cfg	#修改配置文件取消ssh的连接受任信息
host_key_checking = False
[root@ansible project]#

[root@ansible project]# > /root/.ssh/known_hosts
[root@ansible project]# cat /root/.ssh/known_hosts
[root@ansible project]# ansible web -m ping -i hosts
192.168.122.20 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}
192.168.122.115 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "ping": "pong"
}
[root@ansible project]# cat /root/.ssh/known_hosts
192.168.122.115 ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBBp2uq11lJIz+a3RZ78cWp0UpzICCNNSq/blgn6TbRvmaiwIFQP+ylERD76lJUcPfbhbIzDRYsSrVjPrCla7qLQ=
192.168.122.20 ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBFFPxJXjXeEu8xwKUL0Zzr2lYAbikR1hBGCCXM7k+smiKSJzrT+Dnyx7dqseGOEojFqYhFivYQ3Ctvw/+raO+B0=
[root@ansible project]#
```
<a name="gd4RF"></a>
# 二、ansible的常用模块
<a name="SEk35"></a>
## 1、command模块
```c
[root@ansible project]# ansible web -m command -a "df -hT" -i hosts		#命令解释 web是hosts文件中的一个组 执行一个command的模块-a 追加一条命令 -i 指定主机清单
192.168.122.20 | CHANGED | rc=0 >>
Filesystem              Type      Size  Used Avail Use% Mounted on
/dev/mapper/centos-root xfs       8.5G  861M  7.7G  10% /
devtmpfs                devtmpfs  491M     0  491M   0% /dev
tmpfs                   tmpfs     498M     0  498M   0% /dev/shm
tmpfs                   tmpfs     498M  6.6M  491M   2% /run
tmpfs                   tmpfs     498M     0  498M   0% /sys/fs/cgroup
/dev/vda1               xfs       497M   97M  401M  20% /boot
192.168.122.115 | CHANGED | rc=0 >>
Filesystem                           Type      Size  Used Avail Use% Mounted on
/dev/mapper/centos_jiangjingwei-root xfs       6.7G  890M  5.9G  13% /
devtmpfs                             devtmpfs  491M     0  491M   0% /dev
tmpfs                                tmpfs     498M     0  498M   0% /dev/shm
tmpfs                                tmpfs     498M  6.6M  491M   2% /run
tmpfs                                tmpfs     498M     0  498M   0% /sys/fs/cgroup
/dev/vda1                            xfs       497M   96M  402M  20% /boot

[root@ansible project]# ansible web -m command -a "w" -i hosts
192.168.122.115 | CHANGED | rc=0 >>
 03:00:30 up  1:28,  1 user,  load average: 0.00, 0.01, 0.05
USER     TTY        LOGIN@   IDLE   JCPU   PCPU WHAT
root     pts/0     03:00    0.00s  0.10s  0.01s w
192.168.122.20 | CHANGED | rc=0 >>
 03:00:30 up  1:15,  1 user,  load average: 0.00, 0.01, 0.04
USER     TTY        LOGIN@   IDLE   JCPU   PCPU WHAT
root     pts/0     03:00    1.00s  0.11s  0.02s w
[root@ansible project]# ansible web -m command -a "w" -i hosts -f 10		#-f是指定并发返回数
192.168.122.115 | CHANGED | rc=0 >>
 03:02:24 up  1:30,  1 user,  load average: 0.00, 0.01, 0.05
USER     TTY        LOGIN@   IDLE   JCPU   PCPU WHAT
root     pts/0     03:02    0.00s  0.10s  0.01s w
192.168.122.20 | CHANGED | rc=0 >>
 03:02:23 up  1:17,  1 user,  load average: 0.16, 0.05, 0.05
USER     TTY        LOGIN@   IDLE   JCPU   PCPU WHAT
root     pts/0     03:02    0.00s  0.11s  0.01s w

[root@ansible project]# ansible web -m command -a "ps -ef|grep httpd" -i hosts -f 1
192.168.122.115 | FAILED | rc=1 >>
error: unsupported SysV option

Usage:
 ps [options]

 Try 'ps --help <simple|list|output|threads|misc|all>'
  or 'ps --help <s|l|o|t|m|a>'
 for additional help text.

For more details see ps(1).non-zero return code
192.168.122.20 | FAILED | rc=1 >>
error: unsupported SysV option

Usage:
 ps [options]

 Try 'ps --help <simple|list|output|threads|misc|all>'
  or 'ps --help <s|l|o|t|m|a>'
 for additional help text.

For more details see ps(1).non-zero return code
[root@ansible project]#

[root@ansible project]# ansible web -m shell -a "ps -ef|grep httpd" -i hosts -f 1
192.168.122.115 | CHANGED | rc=0 >>
root      2423  2422  0 03:12 pts/0    00:00:00 /bin/sh -c ps -ef|grep httpd
root      2425  2423  0 03:12 pts/0    00:00:00 grep httpd
192.168.122.20 | CHANGED | rc=0 >>
root      2725  2724  0 03:12 pts/0    00:00:00 /bin/sh -c ps -ef|grep httpd
root      2727  2725  0 03:12 pts/0    00:00:00 grep httpd

```
**备注：command与shell的区别是command不支持管道符| 在默认的情况下在-m后面不添加任何模块默认的是command模块**
<a name="wSQZi"></a>
## 2、yum模块的使用
```c
[root@ansible project]# ansible-doc yum		#获取帮助信息
[root@ansible project]# ansible web -m yum -a "name=httpd state=latest" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "changes": {
        "installed": [
            "httpd"
……

#安装当前最新的Apache软件使用epel仓库安装
[root@ansible project]# ansible web -m yum -a "name=httpd state=latest enablerepo=epel" -i hosts
192.168.122.115 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "changes": {
        "installed": [],
        "updated": []
#通过公网安装RPM包
[root@ansible project]# ansible web -m yum -a "name=https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/5.2/rhel/7/x86_64/zabbix-agent-5.2.1-1.el7.x86_64.rpm state=latest" -i hosts
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "changes": {
        "installed": [
            "https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/5.2/rhel/7/x86_64/zabbix-agent-5.2.1-1.el7.x86_64.rpm"
        ],
        "updated": []
    },
#删除安装的httpd软件包
[root@ansible project]# ansible web -m yum -a "name=httpd state=absent" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "changes": {
        "removed": [
            "httpd"
        ]
    },
    "msg": "",
    "rc": 0,
    "results": [
```
<a name="pq6Uk"></a>
## 3、copy模块的使用
```c
[root@ansible project]# grep -i ^listen httpd.conf
Listen 8080
[root@ansible project]# ansible web -m copy -a "src=httpd.conf dest=/etc/httpd/conf/httpd.conf owner=root group=root mode=644" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "checksum": "1fb15fc07df5cace0db41401584e59bcb72ecc04",
    "dest": "/etc/httpd/conf/httpd.conf",
    "gid": 0,
    "group": "root",
    "md5sum": "04e9239e7bd5d5b9b85864226d60eee5",
    "mode": "0644",
    "owner": "root",
    "size": 11755,
    "src": "/root/.ansible/tmp/ansible-tmp-1605776133.48-10279-229283114066961/source",
    "state": "file",
    "uid": 0
}
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "checksum": "1fb15fc07df5cace0db41401584e59bcb72ecc04",
    "dest": "/etc/httpd/conf/httpd.conf",
    "gid": 0,
    "group": "root",
    "md5sum": "04e9239e7bd5d5b9b85864226d60eee5",
    "mode": "0644",
    "owner": "root",
    "secontext": "system_u:object_r:httpd_config_t:s0",
    "size": 11755,
    "src": "/root/.ansible/tmp/ansible-tmp-1605776133.63-10281-109529279276342/source",
    "state": "file",
    "uid": 0
}
[root@ansible project]#

#验证
[root@ansible project]# ansible web -m shell -a "grep -i listen /etc/httpd/conf/httpd.conf" -i hosts
192.168.122.115 | CHANGED | rc=0 >>
# Listen: Allows you to bind Apache to specific IP addresses and/or
# Change this to Listen on specific IP addresses as shown below to
#Listen 12.34.56.78:80
Listen 8080
192.168.122.20 | CHANGED | rc=0 >>
# Listen: Allows you to bind Apache to specific IP addresses and/or
# Change this to Listen on specific IP addresses as shown below to
#Listen 12.34.56.78:80
Listen 8080
[root@ansible project]#

[root@ansible project]# grep -i ^listen httpd.conf
Listen 9090
[root@ansible project]# ansible web -m copy -a "src=httpd.conf dest=/etc/httpd/conf/httpd.conf owner=root group=root mode=644 backup=yes" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "backup_file": "/etc/httpd/conf/httpd.conf.3599.2020-11-19@03:59:42~",
    "changed": true,
    "checksum": "f71984ecaf595d3c8cb0e4b8c4b8e9ba281286e4",
    "dest": "/etc/httpd/conf/httpd.conf",
    "gid": 0,
    "group": "root",
    "md5sum": "8d2cdb50877c93f67aa6033c41fe907f",
    "mode": "0644",
    "owner": "root",
    "size": 11755,
    "src": "/root/.ansible/tmp/ansible-tmp-1605776382.02-10478-151050970892123/source",
    "state": "file",
    "uid": 0
}
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "backup_file": "/etc/httpd/conf/httpd.conf.3978.2020-11-19@03:59:41~",
    "changed": true,
    "checksum": "f71984ecaf595d3c8cb0e4b8c4b8e9ba281286e4",
    "dest": "/etc/httpd/conf/httpd.conf",
    "gid": 0,
    "group": "root",
    "md5sum": "8d2cdb50877c93f67aa6033c41fe907f",
    "mode": "0644",
    "owner": "root",
    "secontext": "system_u:object_r:httpd_config_t:s0",
    "size": 11755,
    "src": "/root/.ansible/tmp/ansible-tmp-1605776382.11-10480-240839147201343/source",
    "state": "file",
    "uid": 0
}
[root@ansible project]# ansible web -m shell -a "ls -l /etc/httpd/conf/httpd.conf*" -i hosts
192.168.122.20 | CHANGED | rc=0 >>
-rw-r--r--. 1 root root 11755 Nov 19 03:59 /etc/httpd/conf/httpd.conf
-rw-r--r--. 1 root root 11755 Nov 19 03:55 /etc/httpd/conf/httpd.conf.3978.2020-11-19@03:59:41~
192.168.122.115 | CHANGED | rc=0 >>
-rw-r--r-- 1 root root 11755 Nov 19 03:59 /etc/httpd/conf/httpd.conf
-rw-r--r-- 1 root root 11755 Nov 19 03:55 /etc/httpd/conf/httpd.conf.3599.2020-11-19@03:59:42~


#往远程主机写文件内容
[root@ansible project]# ansible web -m copy -a "content='this write file to http' dest=/var/www/html/index.html" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "checksum": "4410e022c5d321358434e8710c9f627f6226f475",
    "dest": "/var/www/html/index.html",
    "gid": 0,
    "group": "root",
    "md5sum": "2239c29d5987238724a34ea5ffcbd0f1",
    "mode": "0644",
    "owner": "root",
    "size": 9,
    "src": "/root/.ansible/tmp/ansible-tmp-1605776855.19-10802-260311347830909/source",
    "state": "file",
    "uid": 0
}
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "checksum": "4410e022c5d321358434e8710c9f627f6226f475",
    "dest": "/var/www/html/index.html",
    "gid": 0,
    "group": "root",
    "md5sum": "2239c29d5987238724a34ea5ffcbd0f1",
    "mode": "0644",
    "owner": "root",
    "secontext": "system_u:object_r:httpd_sys_content_t:s0",
    "size": 9,
    "src": "/root/.ansible/tmp/ansible-tmp-1605776855.34-10804-79094630708861/source",
    "state": "file",
    "uid": 0
}

#查看验证
[root@ansible project]# ansible web -m shell -a "cat /var/www/html/index.html" -i hosts
192.168.122.20 | CHANGED | rc=0 >>
this write file to http
192.168.122.115 | CHANGED | rc=0 >>
this write file to http

```
**备注：backup=yes表示的是要是copy的源文件发生了变化则保留之前的文件在拷贝到对方的机器上**
<a name="P37hW"></a>
## 4、file_get_url模块的使用
```c
[root@ansible project]# ansible web -m get_url -a "url=https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/5.0/rhel/7/x86_64/zabbix-agent-5.0.0-1.el7.x86_64.rpm dest=/var/www/html/zabbix" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "checksum_dest": null,
    "checksum_src": "09bd51ed637fe2a4eadadb0a2c64c6d952153574",
    "dest": "/var/www/html/zabbix",
    "elapsed": 4,
    "gid": 0,
    "group": "root",
    "md5sum": "9b9b20ffcd991685604d4cf174d9e507",
    "mode": "0644",
    "msg": "OK (451020 bytes)",
    "owner": "root",
    "size": 451020,
    "src": "/root/.ansible/tmp/ansible-tmp-1605782616.39-12201-120113885403160/tmpVB_Hit",
    "state": "file",
    "status_code": 200,
    "uid": 0,
    "url": "https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/5.0/rhel/7/x86_64/zabbix-agent-5.0.0-1.el7.x86_64.rpm"
}
[root@ansible project]# ansible web -m shell -a "ls -l /var/www/html/zabbix" -i hosts
192.168.122.20 | CHANGED | rc=0 >>
-rw-r--r--. 1 root root 451020 Nov 19 05:16 /var/www/html/zabbix
192.168.122.115 | CHANGED | rc=0 >>
-rw-r--r-- 1 root root 451020 Nov 19 05:15 /var/www/html/zabbix

#下载文件并进行md5值校验
[root@ansible project]# md5sum zabbix-agent-5.0.0-1.el7.x86_64.rpm
9b9b20ffcd991685604d4cf174d9e507  zabbix-agent-5.0.0-1.el7.x86_64.rpm

#使用正常md5值进行校验下载
[root@ansible project]# ansible web -m get_url -a "url=https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/5.0/rhel/7/x86_64/zabbix-agent-5.0.0-1.el7.x86_64.rpm checksum=md5:9b9b20ffcd991685604d4cf174d9e507  dest=/var/www/html/zabbix" -i hosts
192.168.122.20 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "checksum_dest": null,
    "checksum_src": null,
    "dest": "/var/www/html/zabbix",
#修改md5值进行下载会报错
[root@ansible project]# ansible web -m get_url -a "url=https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/5.0/rhel/7/x86_64/zabbix-agent-5.0.0-1.el7.x86_64.rpm checksum=md5:9b9b20ffcd991685604d4cf174d  dest=/var/www/html/zabbix" -i hosts
192.168.122.115 | FAILED! => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "checksum_dest": "09bd51ed637fe2a4eadadb0a2c64c6d952153574",
    "checksum_src": "09bd51ed637fe2a4eadadb0a2c64c6d952153574",
    "dest": "/var/www/html/zabbix/zabbix-agent-5.0.0-1.el7.x86_64.rpm",
    "elapsed": 3,
    "msg": "The checksum for /var/www/html/zabbix/zabbix-agent-5.0.0-1.el7.x86_64.rpm did not match 9b9b20ffcd991685604d4cf174d; it was 9b9b20ffcd991685604d4cf174d9e507.",
    "src": "/root/.ansible/tmp/ansible-tmp-1605783217.7-12608-197771077930201/tmpC4BtLy",
    "url": "https://mirrors.tuna.tsinghua.edu.cn/zabbix/zabbix/5.0/rhel/7/x86_64/zabbix-agent-5.0.0-1.el7.x86_64.rpm"
}

```
<a name="PoQwb"></a>
## 5、file模块的使用
```c
[root@ansible project]# ansible web -m file -a "path=/var/www/html/test.txt state=touch owner=apache group=apache mode=644" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "dest": "/var/www/html/test.txt",
    "gid": 48,
    "group": "apache",
    "mode": "0644",
    "owner": "apache",
    "size": 0,
    "state": "file",
    "uid": 48
}
[root@ansible project]# ansible web -m shell -a 'ls -l /var/www/html/test.txt' -i hosts
192.168.122.20 | CHANGED | rc=0 >>
-rw-r--r--. 1 apache apache 0 Nov 19 05:33 /var/www/html/test.txt
192.168.122.115 | CHANGED | rc=0 >>
-rw-r--r-- 1 apache apache 0 Nov 19 05:33 /var/www/html/test.txt

#创建目录
[root@ansible project]# ansible web -m file -a "path=/var/www/html/test state=directory owner=apache group=apache mode=644" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "gid": 48,
    "group": "apache",
    "mode": "0644",
    "owner": "apache",
    "path": "/var/www/html/test",
    "size": 6,
    "state": "directory",
    "uid": 48
}
[root@ansible project]# ansible web -m shell -a 'ls -ld /var/www/html/test' -i hosts
192.168.122.115 | CHANGED | rc=0 >>
drw-r--r-- 2 apache apache 6 Nov 19 05:35 /var/www/html/test
192.168.122.20 | CHANGED | rc=0 >>
drw-r--r--. 2 apache apache 6 Nov 19 05:35 /var/www/html/test
[root@ansible project]#
#修改文件的权限
[root@ansible project]# ansible web -m file -a "path=/var/www/html/test state=directory owner=apache group=apache mode=755" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "gid": 48,
    "group": "apache",
    "mode": "0755",
    "owner": "apache",
    "path": "/var/www/html/test",
    "size": 6,
    "state": "directory",
    "uid": 48
}
[root@ansible project]# ansible web -m shell -a 'ls -ld /var/www/html/test' -i hosts
192.168.122.20 | CHANGED | rc=0 >>
drwxr-xr-x. 2 apache apache 6 Nov 19 05:35 /var/www/html/test
192.168.122.115 | CHANGED | rc=0 >>
drwxr-xr-x 2 apache apache 6 Nov 19 05:35 /var/www/html/test
[root@ansible project]#

#以递归的方式修改文件的属主与属组
[root@ansible project]# ansible web -m file -a "path=/var/www/html/ owner=apache group=apache mode=755 recurse=yes" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "gid": 48,
    "group": "apache",
    "mode": "0755",
    "owner": "apache",
    "path": "/var/www/html/",
    "size": 62,
    "state": "directory",
    "uid": 48
}
[root@ansible project]# ansible web -m shell -a 'ls -ld /var/www/html/' -i hosts
192.168.122.20 | CHANGED | rc=0 >>
drwxr-xr-x. 3 apache apache 49 Nov 19 05:35 /var/www/html/
192.168.122.115 | CHANGED | rc=0 >>
drwxr-xr-x 4 apache apache 62 Nov 19 05:35 /var/www/html/
[root@ansible project]#
```
<a name="Rj3i8"></a>
## 6、service模块的使用
```c
[root@ansible project]# ansible web -m service -a "name=httpd state=started enabled=yes" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "enabled": true,
    "name": "httpd",
    "state": "started",
    "status": {
        "ActiveEnterTimestampMonotonic": "0",
        "ActiveExitTimestampMonotonic": "0",
        "ActiveState": "inactive",
        "After": "network.target remote-fs.target nss-lookup.target systemd-journald.socket basic.target system.slice",

```
<a name="ZAI7D"></a>
## 7、group模块的使用
```c
[root@ansible project]# ansible web -m group -a "name=news gid=9999" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "gid": 9999,
    "name": "news",
    "state": "present",
    "system": false
}
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "gid": 9999,
    "name": "news",
    "state": "present",
    "system": false
}
[root@ansible project]#

#创建一个系统组
[root@ansible project]# ansible web -m group -a "name=httpd gid=8888 system=yes" -i hosts
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "gid": 8888,
    "name": "httpd",
    "state": "present",
    "system": true
}
#将一个组删除
[root@ansible project]# ansible web -m group -a "name=httpd state=absent" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "name": "httpd",
    "state": "absent"
}

```
<a name="spN7H"></a>
## 8、user模块的使用
```c
#创建一个joh的用户uid为1040主组为adm
[root@ansible project]# ansible web -m user -a "name=joh uid=1040 group=adm" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "comment": "",
    "create_home": true,
    "group": 4,
    "home": "/home/joh",
    "name": "joh",
    "shell": "/bin/bash",
    "state": "present",
    "system": false,
    "uid": 1040
}
#创建系统用户joh登录的shell是/sbin/nologin追加bin sys两个用户
[root@ansible project]# ansible web -m user -a "name=joh shell=/sbin/nologin groups=bin,sys" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "append": false,
    "changed": true,
    "comment": "",
    "group": 4,
    "groups": "bin,sys",
    "home": "/home/joh",
    "move_home": false,
    "name": "joh",
    "shell": "/sbin/nologin",
    "state": "present",
    "uid": 1040
}

```
<a name="bY0cB"></a>
## 9、cron计划任务模块
```c
ansible web -m cron -a "name=ls_test job='ls / >/tmp/job.txt'" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "envs": [],
    "jobs": [
        "ls_test"
    ]
}
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "envs": [],
    "jobs": [
        "ls_test"
    ]
}
#执行端验证
[root@jiangjingwei ~]# crontab -l
#Ansible: ls_test
* * * * * ls / >/tmp/job.txt
[root@jiangjingwei ~]#

#删除计划任务
[root@ansible project]# ansible web -m cron -a "name=ls_test state=absent job='ls / >/tmp/job.txt'" -i hosts
192.168.122.115 | SUCCESS => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": false,
    "envs": [],
    "jobs": []
}
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "envs": [],
    "jobs": []
}
#每天的凌晨两点和五点执行一次ls / >/tmp/ls.txt
[root@ansible project]# ansible web -m cron -a "name=ls_job minute=0 hour=2,5 job='ls / >/tmp/ls.txt'" -i hosts
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "envs": [],
    "jobs": [
        "ls_job"
    ]
}
#执行端验证
[root@jiangjingwei ~]# crontab -l
#Ansible: ls_job
0 2,5 * * * ls / >/tmp/ls.txt
#删除一个计划任务
[root@ansible project]# ansible web -m cron -a "name=ls_job state=absent" -i hosts
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "envs": [],
    "jobs": []
}
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "envs": [],
    "jobs": []
}
[root@ansible project]# ansible web -m shell -a "crontab -l" -i hosts
192.168.122.115 | CHANGED | rc=0 >>

192.168.122.20 | CHANGED | rc=0 >>

[root@ansible project]#

```
<a name="reAO7"></a>
## 10、selinux挂载模块的使用
```c
[root@ansible project]# ansible web -m selinux -a "state=disabled" -i hosts
[WARNING]: SELinux state change will take effect next reboot
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "configfile": "/etc/selinux/config",
    "msg": "Config SELinux state changed from 'enforcing' to 'disabled'",
    "policy": "targeted",
    "reboot_required": true,
    "state": "disabled"
}

```
<a name="pxjKi"></a>
## 11、firewall模块的使用
     开启防火墙放行https流量
```c
[root@ansible project]# ansible web -m firewalld -a "service=https permanent=yes state=enabled" -i hosts
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "msg": "Permanent operation, Changed service https to enabled"
}
192.168.122.115 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "msg": "Permanent operation, Changed service https to enabled"
}
[root@ansible project]#

```
查看防火墙的规则
```c
[root@ansible project]# ansible web -m shell -a "firewall-cmd --list-all" -i hosts
192.168.122.20 | CHANGED | rc=0 >>
public (default, active)
  interfaces: eth0
  sources:
  services: dhcpv6-client ssh
  ports:
  masquerade: no
  forward-ports:
  icmp-blocks:
  rich rules:

192.168.122.115 | CHANGED | rc=0 >>
public (default, active)
  interfaces: eth0
  sources:
  services: dhcpv6-client ssh
  ports:
  masquerade: no
  forward-ports:
  icmp-blocks:
  rich rules:
```
**可以发现的是防火墙的规则并没有生效需要手动的重载才能生效**

放行所有8080-8090所有tcp端口的流量临时和永久都生效
```c
[root@ansible project]# ansible web -m firewalld -a "port=8080-8090/tcp permanent=yes state=enabled immediate=yes" -i hosts
192.168.122.20 | CHANGED => {
    "ansible_facts": {
        "discovered_interpreter_python": "/usr/bin/python"
    },
    "changed": true,
    "msg": "Permanent and Non-Permanent(immediate) operation, Changed port 8080-8090/tcp to enabled"
}
[root@ansible project]# ansible web -m shell -a "firewall-cmd --list-all" -i hosts
192.168.122.20 | CHANGED | rc=0 >>
public (default, active)
  interfaces: eth0
  sources:
  services: dhcpv6-client ssh
  ports: 8080-8090/tcp
  masquerade: no
  forward-ports:
  icmp-blocks:
  rich rules:
```

<a name="eXcEP"></a>
# 三、playbook的初体验
<a name="j8Nn5"></a>
## 1、nfs服务的部署
```yaml
[root@ansible project]# cat nfs.yaml
- hosts: 192.168.122.115
  tasks:
    - name: install nfs server
      yum: name=nfs-utils state=present

    - name: configure nfs server
      copy: src=./exports.j2 dest=/etc/exports

    - name: create directory for mount
      file: path=/nfs_server state=directory

    - name: create nfs group
      group: name=www gid=1021

    - name: create nfs user
      user: name=www uid=1021 group=www

    - name: start nfs server
      service: name=nfs.service enabled=yes state=started


- hosts: 192.168.122.20
  tasks:
    - name: create mount directory
      file:  path=/nfs_client state=directory

    - name: mount nfs_server
      mount: src=192.168.122.115:/nfs_server path=/nfs_client opts=defaults fstype=nfs state=present
#客户端验证
[root@kvm-jiang ~]# df -hT
Filesystem                  Type      Size  Used Avail Use% Mounted on
/dev/mapper/centos-root     xfs       8.5G  982M  7.6G  12% /
devtmpfs                    devtmpfs  491M     0  491M   0% /dev
tmpfs                       tmpfs     498M     0  498M   0% /dev/shm
tmpfs                       tmpfs     498M   13M  485M   3% /run
tmpfs                       tmpfs     498M     0  498M   0% /sys/fs/cgroup
/dev/vda1                   xfs       497M   97M  401M  20% /boot
tmpfs                       tmpfs     100M     0  100M   0% /run/user/0
192.168.122.115:/nfs_server nfs4      6.7G  978M  5.8G  15% /nfs_client

```
<a name="Mdi1U"></a>
## 2、http服务的部署
```yaml
[root@ansible project]# cat http.yaml
- hosts: web
  tasks:
    - name: install httpd server
      yum: name=httpd state=present

    - name: configure httpd server
      copy: src=./httpd.conf dest=/etc/httpd/conf/httpd.conf
      notify: restart httpd server

    - name: start httpd server
      service: name=httpd state=started enabled=yes

  handlers:
    - name: restart httpd server
      service: name=httpd state=restarted

```
<a name="yDrUr"></a>
## 3、vars变量的使用
方式1
```yaml

[root@ansible project]# cat vars.yaml
- hosts: web

  vars:
    - http_package: httpd
    - ftp_package: vsftpd

  tasks:
    - name: install {{ http_package }}
      yum: name={{ http_package }} state=present

    - name: install {{ ftp_package }}
      yum: name={{ ftp_package }} state=present
[root@ansible project]#
[root@ansible project]# ansible-playbook  vars.yaml  -i hosts

PLAY [web] ****************************************************************************

TASK [Gathering Facts] ****************************************************************
ok: [192.168.122.20]
ok: [192.168.122.115]
```
方式二
```yaml
[root@ansible project]# cat vars.yaml
- hosts: web

  vars:
    - http_package: httpd
    - ftp_package: vsftpd

  tasks:
    - name: install {{ http_package }} {{ ftp_package }}
      yum:
        name:
          - "{{ http_package }}"
          - "{{ ftp_package }}"
        state: present
[root@ansible project]# ansible-playbook   vars.yaml  -i hosts

PLAY [web] ******************************************************************************************************************************************************************************

TASK [Gathering Facts] ******************************************************************************************************************************************************************
```
#方式3
```yaml
[root@ansible project]# cat vars_public.yaml
http_package: httpd
ftp_package: vsftpd
[root@ansible project]# cat vars.yaml
- hosts: web
  vars_files: ./vars_public.yaml

  tasks:
    - name: install {{ http_package }} {{ ftp_package }}
      yum:
        name:
          - "{{ http_package }}"
          - "{{ ftp_package }}"
        state: present

[root@ansible project]# ansible-playbook vars.yaml -i hosts

PLAY [web] ******************************************************************************************************************************************************************************

TASK [Gathering Facts] ******************************************************************************************************************************************************************
ok: [192.168.122.20]
ok: [192.168.122.115]

TASK [install httpd vsftpd] *************************************************************************************************************************************************************
ok: [192.168.122.20]
ok: [192.168.122.115]

PLAY RECAP ******************************************************************************************************************************************************************************
192.168.122.115            : ok=2    changed=0    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0
192.168.122.20             : ok=2    changed=0    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0

[root@ansible project]#

```
#方式4
```c
[root@ansible project]# mkdir host_vars			#这个是针对主机的配置、必须是这个名称
[root@ansible project]# mkdir group_vars		#这个是针对组的配置、必须是这个名称
[root@ansible project]# vim group_vars/web
[root@ansible project]# cat group_vars/web		#定义两个变量
wget_package: wget
tree_package: tree
[root@ansible project]# cat vars_group.yaml
- hosts: web
  tasks:
    - name: install rpm packages "{{ wget_package }}" "{{ tree_package }}"
      yum:
        name:
          - "{{ wget_package }}"
          - "{{ tree_package }}"
        state: present
[root@ansible project]# ansible-playbook vars_group.yaml -i hosts

PLAY [web] ****************************************************************************

TASK [Gathering Facts] ****************************************************************
ok: [192.168.122.115]
ok: [192.168.122.20]

```
<a name="BYEqB"></a>
## 4、优先级













