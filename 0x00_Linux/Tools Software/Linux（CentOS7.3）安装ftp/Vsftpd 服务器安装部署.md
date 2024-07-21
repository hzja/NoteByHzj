运维 FTP
<a name="Dj52Y"></a>
## 一、文件传输协议

- FTP

文件传输协议(FTP)是一种基于TCP协议在客C/S架构的协议，占用20和21端口。

- TFTP简单文件传输协议(TFTP)是一种基于UDP，C/S架构的协议，占用69端口TFTP的命令功能不如FTP服务强大，甚至不能遍历目录，在安全性方面也弱于FTP服务因为TFTP不需要客户端的权限认证，也就减少了无谓的系统和网络带宽消耗，效率更高
<a name="wghtH"></a>
### 1.1 FTP 协议概述
一般来讲，人们将计算机联网的首要目的就是获取资料，而文件传输是一种非常重要的获取资料的方式。今天的互联网是由几千万台个人计算机、工作站、服务器、小型机、大型机、巨型机等具有不同型号、不同架构的物理设备共同组成的，而且即便是个人计算机，也可能会装有Windows、Linux、UNIX、Mac等不同的操作系统。为了能够在如此复杂多样的设备之间解决问题解决文件传输问题，文件传输协议（FTP）应运而生。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631510197071-67b20810-8f7c-4c0a-a58d-58dce1a95793.webp#averageHue=%23fdfefb&clientId=ua4e69f1f-aee9-4&from=paste&id=u8adaec30&originHeight=196&originWidth=725&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=stroke&taskId=u9412ff2f-7fc4-4bab-891a-42e31881e72&title=)<br />FTP是一种在互联网中进行文件传输的协议，基于客户端/服务器（C/S）模式，默认使用20、21号端口，其中端口20（数据端口）用于进行数据传输，端口21（命令端口）用于接受客户端发出的相关FTP命令与参数。<br />FTP服务器普遍部署于内网中，具有容易搭建、方便管理的特点。而且有些FTP客户端工具还可以支持文件的多点下载以及断点续传技术，因此FTP服务得到了广大用户的青睐。

- 命令连接传输TCP的文件管理类命令给服务端直到用户退出才关闭连接，否则一直连接
- 数据连接数据传输，按需创建及关闭的连接数据传输可以为文本或二进制格式传输
<a name="JEA6B"></a>
### 1.2 FTP 工作模式
FTP服务器是按照FTP协议在互联网上提供文件存储和访问服务的主机，FTP客户端则是向服务器发送连接请求，以建立数据传输链路的主机。FTP协议有下面两种工作模式。

- 主动模式：FTP服务器主动向客户端发起连接请求命令连接：Client:50000 --> Server:21数据连接：Server:20/tcp --> Client:50000+1模式缺点：客户端有防火墙，一般会禁止服务的主动连接

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631510196972-4edf6fa5-904d-4762-89e0-d40778a56f88.webp#averageHue=%23f8f8f8&clientId=ua4e69f1f-aee9-4&from=paste&id=u2e5ba161&originHeight=325&originWidth=449&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=stroke&taskId=u7867cccd-eaba-4adc-b843-e0cc5ceccf1&title=)

- 被动模式：FTP服务器等待客户端发起连接请求(FTP的默认工作模式)命令连接：Client:50000 --> Server:21数据连接：Client:50000+1 --> Server:随机端口 ==> 通过命令连接得知这个随机端口模式缺点：服务器端需要修改防火墙规则并开放21和这个随机端口

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631510196883-f35b3cb4-15f9-41c9-841b-4d4295f5df1e.webp#averageHue=%23f7f7f7&clientId=ua4e69f1f-aee9-4&from=paste&id=u6bd8b6cf&originHeight=327&originWidth=457&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=stroke&taskId=u056117e4-c641-4ebf-8b08-173f4952fa8&title=)<br />防火墙一般是用于过滤从外网进入内网的流量，因此有些时候需要将FTP的工作模式设置为主动模式，才可以传输数据。但是因为客户端主机一般都设有防火墙，会禁止服务器的连接请求，所有适当的iptables规则变得越来越重要了。
<a name="FdElQ"></a>
### 1.3 FTP 服务程序
由于FTP是一种基于（C/S）客户端/服务器模式的协议，所有有很多的程序能够提供FTP服务的功能。
<a name="zoZgY"></a>
#### 服务端程序

- wu-ftpd一个Internet上最流行的FTP守护程序，功能十分强大，可以构建多种类型FTP服务器。
- proftpd一个Unix平台上或是类Unix平台上的FTP服务器开源程序，任何人只要遵守GPL版权声明都可以随意修改源码。proftpd亦开发了有图形用户界面的FTP服务端软体称为gProFTPd，可以提供图形界面的操作。针对Wu-FTP的弱项而开发的，除了改进的安全性，还具备许多Wu-FTP没有的特点，能以Stand-alone、xinetd模式运行等。
- pureftp

pureftp是免费和安全的FTP服务器，它注重效率和易用性。

- vsftpdvsftpd是very secure FTP daemon的缩写，安全性是它的一个最大的特点，可以运行在多种平台之上提供安全的FTP服务。在CentOS等系统以及多个互联网厂商都在使用的FTP服务程序。
- ServUServ-U是一种被广泛运用在Windows平台的的FTP服务器软件，可以设定多个FTP服务器、限定登录用户的权限、登录主目录及空间大小等优点。
<a name="ETRkb"></a>
#### 客户端程序

- 命令行ftplftp, lftpgetwget, curl因为FTP协议是通过TCP明文传送的，所有为了安全可以使用ftps(SSL), sftp(SSH)工具
- 图形化_ filezilla_ gftp_ gProFTPd_ flashfxp * cuteftp
<a name="bVgmZ"></a>
#### FTP服务的响应码
1xx: 信息<br />2xx: 成功类的状态码<br />3xx: 提示需进一步提供补充类信息的状态码<br />4xx: 客户端错误<br />5xx: 服务端错误
<a name="fOJy9"></a>
## 二、vsftpd 服务程序
vsftpd(非常安全的FTP守护进程)是一款运行在Linux操作系统上的 FTP 服务程序，不仅完全开源而且免费，此外，还具有很高的安全性、传输速度，以及支持虚拟用户验证等其他FTP服务程序具备的特点。
<a name="ntXe4"></a>
### 2.1 安装服务

- 安装服务端程序
```bash
[root@localhost ~]# yum install vsftpd
Loaded plugins: langpacks, product-id, subscription-manager
………………省略部分输出信息………………
================================================================================
Installing:
vsftpd x86_64 3.0.2-9.el7 rhel 166 k
Transaction Summary
================================================================================
Install 1 Package
vsftpd.x86_64 0:3.0.2-9.el7
Complete!

[root@linuxprobe ~]# yum install ftp
Loaded plugins: langpacks, product-id, subscription-manager
………………省略部分输出信息………………
Install 1 Package
ftp.x86_64 0:0.17-66.el7
Complete!
```

- 关闭本机防火墙
```bash
# iptables防火墙管理工具默认禁止了FTP传输协议的端口号
[root@localhost ~]# iptables -F
```

- 配置 FTP 服务
```bash
# 程序的主配置文件为/etc/vsftpd/vsftpd.conf
[root@localhost ~]# cat /etc/vsftpd/vsftpd.conf
anonymous_enable=YES
local_enable=YES
write_enable=YES
local_umask=022
dirmessage_enable=YES
xferlog_enable=YES
connect_from_port_20=YES
xferlog_std_format=YES
listen=NO
listen_ipv6=YES
pam_service_name=vsftpd
userlist_enable=YES
tcp_wrappers=YES
```
<a name="ubATk"></a>
### 2.2 配置文件
这里主要说的是，CentOS6中的配置文件位置，在CentOS7中有可能不同。

- 配置文件主配置文件：/etc/vsftpd/vsftpd.conf配置文件目录：/etc/vsftpd/*.conf服务启动脚本：/etc/rc.d/init.d/vsftpd用户认证配置文件：/etc/pam.d/vsftpd
- 共享目录匿名用户(映射为ftp用户)共享资源位置：/var/ftp系统用户通过ftp访问的资源的位置：用户自己的家目录虚拟用户通过ftp访问的资源的位置：给虚拟用户指定的映射成为的系统用户的家目录
<a name="cbsh1"></a>
### 2.3 配置参数
常用配置参数都为主配置文件，/etc/vsftpd/vsftpd.conf的常用配置。

- 通用基础配置

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631510196832-7ba41bb2-27d7-461f-97b0-d2f9c2405056.webp#averageHue=%23f8fbfa&clientId=ua4e69f1f-aee9-4&from=paste&id=u7d548b07&originHeight=349&originWidth=671&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=stroke&taskId=u4c4c7a2a-cca1-400c-b839-fc4fb58d94c&title=)

- 匿名用户的配置

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631510196910-18a1a55b-196f-43ce-a032-567475a3d840.webp#averageHue=%23f2f4f1&clientId=ua4e69f1f-aee9-4&from=paste&id=ud5de1996&originHeight=280&originWidth=723&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=stroke&taskId=u4861514d-861c-4441-ab23-2fcd206f84c&title=)

- 系统用户的配置

![2021-09-13-13-22-38-522099.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631510580421-832de2d7-0d01-4333-bde4-92a19e004f35.png#averageHue=%23faf8f5&clientId=ua4e69f1f-aee9-4&from=ui&id=uf2cbc553&originHeight=416&originWidth=749&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18176&status=done&style=stroke&taskId=u1b721d31-e695-49df-b2c2-5679d1f5e59&title=)

- 日志功能

![2021-09-13-13-22-38-642092.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631510580419-81283602-610c-4bfb-87b0-1bc7f7a2316c.png#averageHue=%23f9f5f1&clientId=ua4e69f1f-aee9-4&from=ui&id=ouZFR&originHeight=138&originWidth=520&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4943&status=done&style=stroke&taskId=uc56a5b63-8c84-42e2-a59a-af4033c2fb5&title=)
<a name="RKQbX"></a>
## 三、vsftpd 认证模式
vsftpd 作为更加安全的文件传输的服务程序，允许用户以三种认证模式登录到 FTP 服务器上。

- 匿名开放模式匿名开放模式是一种最不安全的认证模式，任何人都可以无需密码验证而直接登录到FTP服务器。这种模式一般用来访问不重要的公开文件，在生产环境中尽量不要存放重要文件，不建议在生产环境中如此行事。
- 本地用户模式本地用户模式是通过Linux系统本地的账户密码信息进行认证的模式，相较于匿名开放模式更安全，而且配置起来相对简单。但是如果被黑客破解了账户的信息，就可以畅通无阻地登录FTP服务器，从而完全控制整台服务器。
- 虚拟用户模式虚拟用户模式是这三种模式中最安全的一种认证模式，它需要为FTP服务单独建立用户数据库文件，虚拟出用来进行口令验证的账户信息，而这些账户信息在服务器系统中实际上是不存在的，仅供FTP服务程序进行认证使用。这样，即使黑客破解了账户信息也无法登录服务器，从而有效降低了破坏范围和影响。
<a name="fnXHW"></a>
### 3.1 匿名访问模式
vsftpd 服务程序默认开启了匿名开放模式，需要做的就是开放匿名用户的上传、下载文件的权限，以及让匿名用户创建、删除、更名文件的权限。
```bash
# 匿名访问模式主配置文件
[root@localhost ~]# vim /etc/vsftpd/vsftpd.conf
anonymous_enable=YES
anon_umask=022
anon_upload_enable=YES
anon_mkdir_write_enable=YES
anon_other_write_enable=YES

local_enable=YES
write_enable=YES
local_umask=022
dirmessage_enable=YES
xferlog_enable=YES
connect_from_port_20=YES
xferlog_std_format=YES
listen=NO
listen_ipv6=YES
pam_service_name=vsftpd
userlist_enable=YES
tcp_wrappers=YES

# 重启服务
[root@localhost ~]# systemctl restart vsftpd

# 服务程序加入到开机启动项中，以保证服务器在重启后依然能够正常提供传输服务
[root@localhost ~]# systemctl enable vsftpd
ln -s '/usr/lib/systemd/system/vsftpd.service' '/etc/systemd/system/multi-user.target.wants/vsftpd.service

# 在vsftpd服务程序的匿名开放认证模式下，其账户统一为anonymous，密码为空
# 连接到FTP服务器后，默认访问的是/var/ftp目录，可以在其中进行创建、删除等操作
[root@localhost ~]# ftp 192.168.10.10
Connected to 192.168.10.10 (192.168.10.10).
220 (vsFTPd 3.0.2)
Name (192.168.10.10:root): anonymous
331 Please specify the password.
Password:此处敲击回车即可
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> cd pub
250 Directory successfully changed.
ftp> mkdir files
550 Permission denied.

# 系统显示拒绝创建目录，这是为什么呢？
# 查看该目录的权限得知，只有root管理员才有写入权限，开放ftp用户权限(该账户在系统中已经存在)
[root@localhost ~]# ls -ld /var/ftp/pub
drwxr-xr-x. 3 root root 16 Jul 13 14:38 /var/ftp/pub

[root@localhost ~]# chown -Rf ftp /var/ftp/pub
[root@localhost ~]# ls -ld /var/ftp/pub
drwxr-xr-x. 3 ftp root 16 Jul 13 14:38 /var/ftp/pub

[root@localhost ~]# ftp 192.168.10.10
………………省略部分输出信息………………
ftp> mkdir files
550 Create directory operation failed.

# 可以发现还是无法创建目录，发现是SELinux服务在捣乱了
# 使用getsebool命令查看与FTP相关的SELinux域策略都有哪些
[root@localhost ~]# getsebool -a | grep ftp
ftp_home_dir --> off
ftpd_anon_write --> off
ftpd_connect_all_unreserved --> off
ftpd_connect_db --> off
ftpd_full_access --> off
ftpd_use_cifs --> off
ftpd_use_fusefs --> off
ftpd_use_nfs --> off
ftpd_use_passive_mode --> off
httpd_can_connect_ftp --> off
httpd_enable_ftp_server --> off
sftpd_anon_write --> off
sftpd_enable_homedirs --> off
sftpd_full_access --> off
sftpd_write_ssh_home --> off
tftp_anon_write --> off
tftp_home_dir --> off

# 根据经验和策略的名称判断出是ftpd_full_access--> off策略规则导致了操作失败
[root@localhost ~]# setsebool -P ftpd_full_access=on

# 此时，匿名用户就可以正常使用FTP服务了
[root@linuxprobe ~]# ftp 192.168.10.10
………………省略部分输出信息………………
ftp> mkdir files
257 "/pub/files" created
ftp> rename files database
350 Ready for RNTO.
250 Rename successful.
ftp> exit
221 Goodbye.
```
<a name="ZpEdr"></a>
### 3.2 本地用户模式
如果大家之前用的是匿名开放模式，现在就可以将它关了，然后开启本地用户模式。
```bash
# 本地用户模式主配置文件
[root@localhost ~]# vim /etc/vsftpd/vsftpd.conf
anonymous_enable=NO

local_enable=YES
write_enable=YES
local_umask=022
dirmessage_enable=YES
connect_from_port_20=YES
listen=NO
listen_ipv6=YES

pam_service_name=vsftpd
userlist_enable=YES
tcp_wrappers=YES
xferlog_enable=YES
xferlog_std_format=YES

# 同样需要重启服务和开机自启动
[root@localhost ~]# systemctl restart vsftpd

# 服务程序加入到开机启动项中，以保证服务器在重启后依然能够正常提供传输服务
[root@localhost ~]# systemctl enable vsftpd
ln -s '/usr/lib/systemd/system/vsftpd.service' '/etc/systemd/system/multi-user.target.wants/vsftpd.service

# 现在已经完全可以本地用户的身份登录FTP服务器了，但是使用root无法登陆
[root@localhost ~]# ftp 192.168.10.10
Connected to 192.168.10.10 (192.168.10.10).
220 (vsFTPd 3.0.2)
Name (192.168.10.10:root): root
530 Permission denied.
Login failed.
ftp>

# 这是因为，为了系统的安全，默认禁止root等用户登录FTP服务被系统拒绝访问
# 因为vsftpd服务程序所在的目录中，默认存放着两个名为用户名单的文件，ftpusers和user_list
# 在ftpusers和user_list两个用户文件中将root用户删除就可以登录了
[root@localhost ~]# cat /etc/vsftpd/user_list
root
bin
daemon

[root@localhost ~]# cat /etc/vsftpd/ftpusers
root
bin
daemon

# 在采用本地用户模式登录FTP服务器后，默认访问的是该用户的家目录，因此不存在写入权限不足的情况
# 如果不关闭SELinux，则需要再次开启SELinux域中对FTP服务的允许策略
[root@localhost ~]# setsebool -P ftpd_full_access=on

# 即可以使用系统用户进行FTP服务的登录了
[root@localhost ~]# ftp 192.168.10.10
Connected to 192.168.10.10 (192.168.10.10).
220 (vsFTPd 3.0.2)
Name (192.168.10.10:root): escape
331 Please specify the password.
Password:此处输入该用户的密码
230 Login successful.
Remote system type is UNIX.
```
<a name="Zqu0w"></a>
### 3.3 虚拟用户模式(文本文件)
认证模式：vsftpd + pam + file

- 第一步：创建用于进行 FTP 认证的用户数据库文件这里使用文本文件进行用户认证数据库文件中奇数行为账户名，偶数行为密码
```bash
# 编辑虚拟用户文件
[root@localhost ~]# cd /etc/vsftpd/
[root@localhost vsftpd]# vim vuser.list
zhangsan
redhat
lisi
redhat

# 明文信息既不安全，也不符合让vsftpd服务程序直接加载的格式
# 因此需要使用db_load命令用哈希算法将原始的明文信息文件转换成数据库文件
# 降低数据库文件的权限，然后再把原始的明文信息文件删除
[root@localhost vsftpd]# db_load -T -t hash -f vuser.list vuser.db
[root@localhost vsftpd]# file vuser.db
vuser.db: Berkeley DB (Hash, version 9, native byte-order)

[root@localhost vsftpd]# chmod 600 vuser.db
[root@localhost vsftpd]# rm -f vuser.list
```

- 第二步：创建 vsftpd 服务程序用于存储文件的根目录以及虚拟用户映射的系统本地用户FTP服务用于存储文件的根目录指的是，当虚拟用户登录后所访问的默认位置可以把这个系统本地用户的家目录设置为/var目录并设置不允许登录FTP服务器
```bash
[root@localhost ~]# useradd -d /var/ftproot -s /sbin/nologin virtual

[root@localhost ~]# ls -ld /var/ftproot/
drwx------. 3 virtual virtual 74 Jul 14 17:50 /var/ftproot/

[root@localhost ~]# chmod -Rf 755 /var/ftproot/
```

- 第三步：建立支持虚拟用户的 PAM 认证文件PAM是一种认证机制，通过一些动态链接库和统一的API把系统提供的服务与认证方式分开PAM是可插拔认证模块，使得系统管理员可以根据需求灵活调整服务程序的不同认证方式
```bash
#新建一个用于虚拟用户认证的PAM文件vsftpd.vu
#PAM文件内的db=参数为使用db_load命令生成的账户密码数据库文件的路径，但不用写数据库文件的后缀

[root@localhost ~]# vim /etc/pam.d/vsftpd.vu
auth       required     pam_userdb.so    db=/etc/vsftpd/vuser
account    required     pam_userdb.so    db=/etc/vsftpd/vuser
```

- 第四步：在 vsftpd.conf 文件中添加支持配置在vsftpd服务程序的主配置文件中默认就带有参数`pam_service_name=vsftpd`表示登录FTP服务器时是根据`/etc/pam.d/vsftpd`文件进行安全认证的
```bash
# 要做的就是把vsftpd主配置文件中原有的PAM认证文件vsftpd修改为新建的vsftpd.vu文件即可
[root@localhost ~]# vim /etc/vsftpd/vsftpd.conf
anonymous_enable=NO
pam_service_name=vsftpd.vu
user_config_dir=/etc/vsftpd/vusers_dir

local_enable=YES
guest_enable=YES
guest_username=virtual
allow_writeable_chroot=YES
write_enable=YES
local_umask=022
dirmessage_enable=YES
xferlog_enable=YES
connect_from_port_20=YES
xferlog_std_format=YES
listen=NO
listen_ipv6=YES
userlist_enable=YES
tcp_wrappers=YES
```

- 第五步：为虚拟用户设置不同的权限只需新建一个目录，在里面分别创建两个以zhangsan和lisi命名的文件在每个文件中，对用户分别进行配置，达到管理不用用户权限的效果
```bash
[root@localhost ~]# mkdir /etc/vsftpd/vusers_dir/
[root@localhost ~]# cd /etc/vsftpd/vusers_dir/

[root@localhost vusers_dir]# touch lisi

[root@localhost vusers_dir]# vim zhangsan
anon_upload_enable=YES
anon_mkdir_write_enable=YES
anon_other_write_enable=YES
```

- 第六步：设置 SELinux 域允许策略
```bash
# 先按照前面实验中的步骤开启SELinux域的允许策略，以免再次出现操作失败的情况
[root@localhost ~]# setsebool -P ftpd_full_access=on

[root@localhost ~]# getsebool -a | grep ftp
ftp_home_dir –> off
ftpd_anon_write –> off
ftpd_connect_all_unreserved –> off
ftpd_connect_db –> off
ftpd_full_access –> off
ftpd_use_cifs –> off
ftpd_use_fusefs –> off
ftpd_use_nfs –> off
ftpd_use_passive_mode –> off
httpd_can_connect_ftp –> off
httpd_enable_ftp_server –> off
sftpd_anon_write –> off
sftpd_enable_homedirs –> off
sftpd_full_access –> on
sftpd_write_ssh_home –> off
tftp_anon_write –> off
tftp_home_dir –> off
```

- 第七步：使用虚拟 FTP 用户访问测试
```bash
[root@localhost ~]# ftp 192.168.10.10
Connected to 192.168.10.10 (192.168.10.10).
220 (vsFTPd 3.0.2)
Name (192.168.10.10:root): lisi
331 Please specify the password.
Password:此处输入虚拟用户的密码
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> mkdir files
550 Permission denied.
ftp> exit
221 Goodbye.

[root@localhost ~]# ftp 192.168.10.10
Connected to 192.168.10.10 (192.168.10.10).
220 (vsFTPd 3.0.2)
Name (192.168.10.10:root): zhangsan
331 Please specify the password.
Password:此处输入虚拟用户的密码
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> mkdir files
257 "/files" created
ftp> rename files database
350 Ready for RNTO.
250 Rename successful.
ftp> rmdir database
250 Remove directory operation successful.
ftp> exit
221 Goodbye.
```
<a name="hublw"></a>
### 3.4 虚拟用户模式(MySQL 数据库)
认证模式：vsftpd + pam + mysql
<a name="RkpTX"></a>
#### 第一步：安装所需要程序
```bash
# 安装mysql和pam_mysql，pam_mysql由epel源提供
$ yum -y install vsftpd mysql-server mysql-devel pam_mysql
```
<a name="qluIJ"></a>
#### 第二步：创建虚拟用户账号
<a name="yhO0L"></a>
##### 1、准备数据库及相关表
首先请确保mysql服务已经正常启动。而后，按需要建立存储虚拟用户的数据库即可，这里将其创建为vsftpd数据库
```sql
mysql> create database vsftpd;
```
授权vsftpd用户可以通过localhost主机操作select权限vsftpd库的所有表，密码为www.escapelife.com
```sql
mysql> grant select on vsftpd.* to vsftpd@localhost identified by 'www.escapelife.com';
mysql> grant select on vsftpd.* to vsftpd@127.0.0.1 identified by 'www.escapelife.com';
mysql> flush privileges;
```
（这里是本机安装所有授权localhost主机，如果不同需要使用不同的主机地址）<br />（mysql> grant select on vsftpd.* to vsftpd@'192.168.31.%' identified by 'www.escapelife.com';）<br />（mysql> flush privileges;）

创建相关表
```sql
mysql> use vsftpd;
mysql> create table users (
    -> id INT UNSIGNED AUTO_INCREMENT NOT NULL,
    -> name VARCHAR(50) BINARY NOT NULL,
    -> password char(48) BINARY NOT NULL,
    -> PRIMARY KEY(id)
    -> );
```
<a name="hBtTm"></a>
##### 2、添加测试的虚拟用户
根据需要添加所需要的用户，需要说明的是，这里将其密码为了安全起见应该使用PASSWORD函数加密后存储
```sql
mysql> INSERT INTO users(name, password) VALUES('tom', password('escapelife'));
mysql> INSERT INTO users(name, password) VALUES('bob', password('escapelife'));
SELETC * FROM user;
```
<a name="rpSND"></a>
#### 第三步：配置 vsftpd
<a name="GvyTA"></a>
##### 1、建立pam认证所需文件
```bash
$ vi /etc/pam.d/vsftpd.mysql
```
添加如下两行，主要32位和64位库文件的不同位置，模块路径可不写表示pam模块路径<br />指定用户、密码、主机、数据库、表、用户名字段、密码字段、密码的加密方式<br />密码的加密方式可以通过安装的pam_mysql的文档查看，/usr/share/doc/pam_mysql-0.7/README   rpm -ql pam_mysql
```bash
auth required /lib64/security/pam_mysql.so user=vsftpd passwd=www.escapelife.com host=localhost db=vsftpd table=users usercolumn=name passwdcolumn=password crypt=2
account required /lib64/security/pam_mysql.so user=vsftpd passwd=www.escapelife.com host=localhost db=vsftpd table=users usercolumn=name passwdcolumn=password crypt=2

（auth required /lib64/security/pam_mysql.so user=vsftp passwd=www.escapelife.com host=192.168.31.71 db=vsftpd table=users usercolumn=name passwdcolumn=password crypt=2）
（account required /lib64/security/pam_mysql.so user=vsftp passwd=www.escapelife.com host=192.168.31.71 db=vsftpd table=users usercolumn=name passwdcolumn=password crypt=2）
```
注意：由于mysql的安装方式不同，pam_mysql.so基于unix sock连接mysql服务器时可能会出问题，此时，建议授权一个可远程连接的mysql并访问vsftpd数据库的用户。
<a name="cfSED"></a>
##### 2、修改vsftpd的配置文件，使其适应mysql认证
建立虚拟用户映射的系统用户及对应的目录
```bash
$ useradd -s /sbin/nologin -d /var/ftproot vuser
$ chmod go+rx /var/ftproot
```
请确保/etc/vsftpd.conf中已经启用了以下选项
```bash
anonymous_enable=YES
local_enable=YES
write_enable=YES
anon_upload_enable=NO
anon_mkdir_write_enable=NO
chroot_local_user=YES
```
而后添加以下选项
```bash
guest_enable=YES
guest_username=vuser
```
并确保`pam_service_name`选项的值如下所示
```bash
pam_service_name=vsftpd.mysql
```
<a name="mJHZb"></a>
#### 第四步：启动 vsftpd 服务
```bash
# 启动vsftpd服务
$ service vsftpd start
$ chkconfig vsftpd on

# 查看端口开启情况
$ netstat -tnlp | grep :21
tcp      0      0 0.0.0.0:21      0.0.0.0:*      LISTEN      23286/vsftpd

# 使用虚拟用户登录，验证配置结果
$ ftp localhost
```
<a name="fRMef"></a>
#### 第五步：配置虚拟用户具有不同的访问权限
```bash
# vsftpd可以在配置文件目录中为每个用户提供单独的配置文件以定义其ftp服务访问权限，每个虚拟用户的配置文件名同虚拟用户的用户名。配置文件目录可以是任意未使用目录，只需要在vsftpd.conf指定其路径及名称即可。

# 1、配置vsftpd为虚拟用户使用配置文件目录
$ vim vsftpd.conf
user_config_dir=/etc/vsftpd/vusers_config

# 2、创建所需要目录，并为虚拟用户提供配置文件
$ mkdir /etc/vsftpd/vusers_config/
$ cd /etc/vsftpd/vusers_config/
$ touch tom bob

# 3、配置虚拟用户的访问权限
# 虚拟用户对vsftpd服务的访问权限是通过匿名用户的相关指令进行的
# 如果需要让tom用户具有上传文件的权限，可以修改/etc/vsftpd/vusers_config/tom文件
anon_upload_enable={YES|NO}
anon_mkdir_write_enable={YES|NO}
anon_other_write_enable={YES|NO}
```
