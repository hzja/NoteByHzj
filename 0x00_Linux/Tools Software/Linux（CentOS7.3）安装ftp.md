Linux CentOS ftp <br />FTP是用于在网络上进行文件传输的一套标准协议，使用客户/服务器模式。它属于网络传输协议的应用层。文件传送（file transfer）和文件访问（file access）之间的区别在于：前者由FTP提供，后者由如NFS等应用系统提供。<br />在FTP的使用当中，用户经常遇到两个概念："下载"（Download）和"上传"（Upload）。<br />"下载"文件就是从远程主机拷贝文件至自己的计算机上；"上传"文件就是将文件从自己的计算机中拷贝至远程主机上。用Internet语言来说，用户可通过客户机程序向（从）远程主机上传（下载）文件，由于FTP的文件传输是明文方式，具有一定危险性，所以就诞生了一种更加安全的传输方式vsftp
<a name="4kfmd"></a>
## 1、VSFTP是一个比FTP更安全的软件具有以下特点

1. vsftp一般以普通用户运行，降低了进程的权限，提高了安全性
2. 任何需要执行较高权限的指令都需要上层程序的许可
3. ftp的命令都被整合到了vsftp中，不需要系统额外提供命令
4. 用于chroot功能，可以改变用户的根目录，限制用户只能在自己的家目录
5. vsftpd 是一个基于GPL发布的FTP服务器软件。其中的vs是“ Very Secure”的缩写，由此名称缩写可以看出，本服务器的初衷就是服务的安全性。
6. vsftpd是RedHat Linux默认使用的ftp服务端软件。
7. vsftpd不再依赖于xinetd服务
8. vsftpd可同时允许匿名（ anonymous ）与本地用户(local)访问,还可以支持虚拟用户。
<a name="3094-1561777754996"></a>
## 2、VSFTP连接类型
控制连接：TCP 21，用于发送FTP命令信息<br />数据连接：TCP 20，用于上传、下载数据
<a name="hOR7z"></a>
## 3、VSFTP下载前的检查
<a name="b0vfT"></a>
### 关闭SElinux-编辑 `/etc/selinux/config` ，设置SELINUX=disabled
```bash
$ vim /etc/selinux/config
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595778921543-d770882d-abae-463a-961c-0f4d2ef77bfc.png#averageHue=%23373737&height=583&id=XkZkz&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2019461&status=done&style=none&title=&width=1107.6666666666667)
<a name="zLJNo"></a>
### 关闭防火墙
```bash
[root@iZuligp6e1dyzfZ selinux]# systemctl stop firewalld.service
[root@iZuligp6e1dyzfZ selinux]# systemctl disable firewalld.service
```
<a name="EEQeC"></a>
### 查看当前系统是否已经安装vsftp
```bash
[root@iZuligp6e1dyzfZ selinux]# rpm -qa | grep vsftpd
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595779167724-b88284f1-6e9a-42e5-a454-94cafad4904e.png#averageHue=%23888888&height=57&id=M3jLC&originHeight=170&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=123301&status=done&style=none&title=&width=1107.6666666666667)<br />没有反馈则未安装
<a name="nKGDI"></a>
## 4、VSFTP安装-使用yum安装
```bash
[root@iZuligp6e1dyzfZ selinux]# yum install -y vsftpd
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595779272399-efe4d863-af6c-4c43-a582-f8aab0724e40.png#averageHue=%23404040&height=583&id=kpTND&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2054018&status=done&style=none&title=&width=1107.6666666666667)
<a name="aljZo"></a>
##  5、vsftp启动
```bash
[root@iZuligp6e1dyzfZ selinux]# systemctl start vsftpd
```
<a name="kE495"></a>
## 6、vsftp服务器的访问
vsftp服务器的访问：在浏览器输入： `ftp://127.0.0.1:21`<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595779536978-cf64d83a-6e8b-4686-874a-a116beed4332.png#averageHue=%23fefefe&height=686&id=q2x5w&originHeight=2058&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=293548&status=done&style=shadow&title=&width=1280)
<a name="435Qy"></a>
## 7、vsftp用户配置
<a name="SgxsT"></a>
### 创建vsftp用户
```bash
[root@iZuligp6e1dyzfZ /]# useradd fcant
```
<a name="Pu5iB"></a>
### 为创建的用户设置密码
```bash
[root@iZuligp6e1dyzfZ /]# echo "fcant" | passwd fcant --stdin
Changing password for user fcant.
passwd: all authentication tokens updated successfully.
```
<a name="diWGG"></a>
### 限制该用户只能访问ftp不能直接登录ftp
```bash
[root@iZuligp6e1dyzfZ /]# usermod -s /sbin/nologin fcant
```
<a name="7gLJ7"></a>
### 设置用户的写入权限
```bash
[root@iZuligp6e1dyzfZ /]# usermod -a -G root fcant
```
<a name="TnETf"></a>
## 8、vsftp文件存储管理
<a name="NhF0o"></a>
### 创建FTP存储目录
`mkdir /local/ftp/storage/ -p` 创建主目录，不可上传文件<br />`mkdir /local/ftp/storage/upload` 文件存储目录
```bash
[root@iZuligp6e1dyzfZ /]# mkdir /local/ftp/storage/ -p
[root@iZuligp6e1dyzfZ /]# mkdir /local/ftp/storage/upload
```
<a name="aJKVq"></a>
### 设置目录的访问权限
```bash
[root@iZuligp6e1dyzfZ /]# chmod a-w /local/ftp/storage/ && chmod 777 -R /local/ftp/storage/upload/
```
<a name="a0aYq"></a>
## 9、修改系统用户的存储目录
```bash
[root@iZuligp6e1dyzfZ ftp]# vim /etc/vsftpd/vsftpd.conf
```
```bash
# 指定系统用户的存储目录
local_root=/local/ftp/storage/upload/ 
# 指定匿名用户的存储目录
anon_root=/local/ftp/storage/upload/ 
```
<a name="SwTxk"></a>
## 10、配置FTP权限
<a name="2312-1561786486142"></a>
### vsftpd的配置目录：/etc/vsftpd
vsftpd.conf 主要配置文件（注意备份）<br />ftpusers 配置禁止访问 FTP 服务器的用户列表<br />user_list 配置用户访问控制<br />建议备份vsftpd.conf文件： cp vsftpd.conf vsftpd.conf.bak<br />绝对路径： cp /etc/vsftpd/vsftpd.conf /etc/vsftpd/vsftpd.conf.bak
<a name="5793-1561786486142"></a>
### 关闭匿名访问和切换根目录权限
```bash
[root@iZuligp6e1dyzfZ ftp]# vim vsftpd.conf
```
```bash
# 禁用匿名用户
anonymous_enable=NO 
# 禁止切换根目录 删除此行
chroot_local_user=YES 
```
<a name="ZZE1q"></a>
## 11、为不同的用户配置不同的FTP根目录
为每个用户配置不同的ftp目录
<a name="t78LC"></a>
### 取消总配置文件中的存储目录
```bash
[root@iZuligp6e1dyzfZ ftp]# vim /etc/vsftpd/vsftpd.conf
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595820305297-6cbe5430-3d41-4963-ba46-379bac33c2a3.png#averageHue=%23383838&height=611&id=AvVOn&originHeight=1834&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2187201&status=done&style=none&title=&width=1107.6666666666667)
<a name="eHtVL"></a>
### 创建第二个用户、设置密码、设置登录权限
```bash
[root@iZuligp6e1dyzfZ ftp]# useradd fcc
[root@iZuligp6e1dyzfZ ftp]# echo "fcc" | passwd fcc --stdin
Changing password for user fcc.
passwd: all authentication tokens updated successfully.
[root@iZuligp6e1dyzfZ ftp]# usermod -s /sbin/nologin fcc
```
<a name="YdN8f"></a>
### 为每个用户添加单独的配置
<a name="vbOSs"></a>
#### ①在/etc/vsftpd/目录下创建userconfig
```bash
[root@iZuligp6e1dyzfZ ftp]# cd /etc/vsftpd/
[root@iZuligp6e1dyzfZ vsftpd]# mkdir userconfig
[root@iZuligp6e1dyzfZ vsftpd]# cd userconfig/
```
<a name="6nx6Q"></a>
#### ②将用户的配置文件的目录添加到总配置文件中
```bash
[root@iZuligp6e1dyzfZ userconfig]# vim /etc/vsftpd/vsftpd.conf
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595820840220-1f7b9724-6fd4-4f88-a52d-a7a5ceace607.png#averageHue=%23383838&height=611&id=unmcP&originHeight=1834&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2165927&status=done&style=none&title=&width=1107.6666666666667)
<a name="LLZH7"></a>
#### ③创建用户的存储根目录-并设置目录的访问权限
```bash
[root@iZuligp6e1dyzfZ userconfig]# cd /local/ftp/storage/upload/
[root@iZuligp6e1dyzfZ upload]# ll
total 185312
-rw-r--r-- 1 fcant fcant 189756259 Jul 27 01:15 jdk-8u161-linux-x64.tar.gz
[root@iZuligp6e1dyzfZ upload]# mkdir fcc
[root@iZuligp6e1dyzfZ upload]# mkdir fcant
[root@iZuligp6e1dyzfZ upload]# ll
total 185320
drwxr-xr-x 2 root  root       4096 Jul 27 11:49 fcant
drwxr-xr-x 2 root  root       4096 Jul 27 11:48 fcc
-rw-r--r-- 1 fcant fcant 189756259 Jul 27 01:15 jdk-8u161-linux-x64.tar.gz
[root@iZuligp6e1dyzfZ upload]# chmod a-w /local/ftp/storage/ && chmod 777 -R /local/ftp/storage/upload/
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595821823032-9da3db45-67ba-4333-917f-a82c79457b6e.png#averageHue=%23373737&height=232&id=qcKVD&originHeight=695&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=835644&status=done&style=none&title=&width=1107.6666666666667)
<a name="cwRLo"></a>
#### ④在userconfig目录下创建用户名的配置文件，并设置根目录
<a name="0haZy"></a>
##### 为fcc用户通过fcc文件配置存储目录
```bash
[root@iZuligp6e1dyzfZ userconfig]# vim fcc
```
```bash
local_root=/local/ftp/storage/upload/fcc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595821318636-fe864709-915d-4e66-a443-998fc681f4e1.png#averageHue=%23373737&height=611&id=niGia&originHeight=1834&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1982394&status=done&style=none&title=&width=1107.6666666666667)
<a name="GThPR"></a>
##### 为fcant用户通过fcant文件配置存储目录
```bash
[root@iZuligp6e1dyzfZ userconfig]# vim fcant
```
```bash
local_root=/local/ftp/storage/upload/fcant
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595821583447-43afabc7-aba6-47a1-8249-f751e5039123.png#averageHue=%23373737&height=611&id=tapL2&originHeight=1834&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1983519&status=done&style=none&title=&width=1107.6666666666667)
<a name="pxvIn"></a>
##### 配置好后在userconfig目录下可以看到两个配置文件
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595821710376-bc4c630c-1b51-47f6-9ad7-c64bbf9879ce.png#averageHue=%23393939&height=145&id=CgK89&originHeight=435&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=504975&status=done&style=none&title=&width=1107.6666666666667)
<a name="C29YW"></a>
### 设置用户的写入权限
```bash
[root@iZuligp6e1dyzfZ upload]# usermod -a -G root fcant
[root@iZuligp6e1dyzfZ upload]# usermod -a -G root fcc
```
<a name="c8t8Y"></a>
### 重启vsftpd服务使其生效
```bash
[root@iZuligp6e1dyzfZ /]# systemctl restart vsftpd
```
<a name="9vWnE"></a>
### 测试（使用不同帐号上传查看配置的存储目录下的文件上传情况）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595823855809-2e257155-d055-42b1-9abd-41488886f7e1.png#averageHue=%23faf9f9&height=554&id=uxYQ7&originHeight=1663&originWidth=3022&originalType=binary&ratio=1&rotation=0&showTitle=false&size=457389&status=done&style=shadow&title=&width=1007.3333333333334)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595824277211-61b80683-7f97-4e6c-bc8e-0e9491be51e8.png#averageHue=%23373737&height=89&id=NI0uU&originHeight=268&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=362124&status=done&style=none&title=&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595824693392-e3b72adc-83f8-4fd5-a77f-3d4b2a43232f.png#averageHue=%23faf9f9&height=554&id=grZqP&originHeight=1663&originWidth=3090&originalType=binary&ratio=1&rotation=0&showTitle=false&size=511099&status=done&style=shadow&title=&width=1030)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595824765345-239eece0-aacd-4c61-915f-daeca6b42077.png#averageHue=%23383838&height=198&id=JDnnY&originHeight=593&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=724087&status=done&style=none&title=&width=1107.6666666666667)
<a name="y9047"></a>
### 配置后重启服务生效
```bash
[root@iZuligp6e1dyzfZ /]# systemctl restart vsftpd
```
<a name="Dtso1"></a>
## vsftp.conf配置参数说明
<a name="UCXod"></a>
### 系统用户的配置
```bash
local_enable=YES # 是否允许本地用户登录
write_enable=YES # 是否允许本地用户的写权限(是否允许上传)
listen=YES # 是否开启监听
listen_port=2121 # 修改连接端口
pam_service_name=vsftpd # 服务名称
local_umask=022 # 默认的umask码(本地用户的掩码信息)
diremssage_enable=YES # 是否显示目录说明文件
connect_from_prot_20=YES # 是否确定端口传输来自20
xferlog_ftd_format=YES # 是否使用标准的ftp xferlog模式
```
<a name="7b3be0d3"></a>
### 匿名登录设置(匿名用户的常用配置)
```bash
anonymous_enable=NO # 是否允许匿名登录
anno_upload_enable=YES #是否允许匿名用户上传权限
anno_mkdir_write_enable=YES #是否允许匿名用户可创建目录及其文件
anno_other_write_ebable=YES #匿名用户是否除了写权限是否拥有删除和修改的权限
anno_world_readable_only=YES #匿名用户是否拥有只读权限
no_anno_password=YES #匿名用户是否跳过密码检测
anno_umask=077 #匿名用户创建文件的掩码权限
```
<a name="8652-1561797550415"></a>
### 限制目录
```bash
chroot_local_user=yes # 限制所有用户都在家目录(禁锢所有ftp用户在其家目录下)
chroot_list_enable=YES # 调用限制在家目录的用户名单(是否将系统用户限制在自己的home目录下)
chroot_list_file=/etc/vsftpd/chroot_list # 限制在家目录的用户名单所在路径(列表不受限制的用户)
```
<a name="57848c21"></a>
### 日志设置
```bash
xferlog_file=/var/log/vsftpd.log # 日志文件路径设置
xferlog_enable=YES # 是否记录ftp传输过程
use_localtime=YES
```
默认情况下，vsftpd 是用GMT做为它的时间的，所以和操作系统的时间不一致，加入这个命令来同步vsftpd与操作系统的时间
<a name="mYGVb"></a>
## 使用Window自带的FTP连接FTP服务器
<a name="1ZcgP"></a>
### 检查Window的FTP功能是否开启
<a name="aba1dd9a"></a>
#### ①进入控制面板-程序
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595783022292-6d73ce9b-5edd-4504-9881-8e3d1cfdb8c4.png#averageHue=%23fefdfd&height=492&id=ryetE&originHeight=1477&originWidth=2816&originalType=binary&ratio=1&rotation=0&showTitle=false&size=329979&status=done&style=shadow&title=&width=938.6666666666666)
<a name="HeXAN"></a>
#### ②启用或关闭Window功能
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595783052224-4abbb14b-aaa0-4a57-a5b5-1cd34e92d9e1.png#averageHue=%23fefefe&height=491&id=OlzhM&originHeight=1473&originWidth=2815&originalType=binary&ratio=1&rotation=0&showTitle=false&size=173845&status=done&style=shadow&title=&width=938.3333333333334)
<a name="CS3no"></a>
#### ③检查FTP服务、FTP扩展性、TFTP Client是否勾选开启
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595783134235-44c3f64a-b840-4d3a-8851-679b2c368131.png#averageHue=%23ebeae8&height=314&id=pLxGr&originHeight=943&originWidth=968&originalType=binary&ratio=1&rotation=0&showTitle=false&size=138715&status=done&style=none&title=&width=322.6666666666667)
<a name="VGXoz"></a>
### 更改浏览器连接属性
<a name="9lDOS"></a>
#### ①控制面板选择Internet选项
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595783563060-aab0d10b-e8e7-4794-9520-26d7a72a8d25.png#averageHue=%23fefefe&height=491&id=XJYZp&originHeight=1473&originWidth=2816&originalType=binary&ratio=1&rotation=0&showTitle=false&size=197365&status=done&style=shadow&title=&width=938.6666666666666)
<a name="YTpbG"></a>
#### ②取消勾选-使用被动FTP
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595783650549-6fcf8682-ff22-4683-b050-b16b0aaaa794.png#averageHue=%23f2f1f0&height=523&id=OvJfx&originHeight=1568&originWidth=1222&originalType=binary&ratio=1&rotation=0&showTitle=false&size=208047&status=done&style=none&title=&width=407.3333333333333)
<a name="SArVI"></a>
### 在服务器的URL输入 `ftp://主机IP/` 即可连接FTP服务器，进行文件的上传下载（其他权限需要进行赋予授权：如删除）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595783864399-cff18e59-b6db-4125-9caf-22e9e1293b14.png#averageHue=%23e3c085&height=503&id=Y1Dma&originHeight=1510&originWidth=2193&originalType=binary&ratio=1&rotation=0&showTitle=false&size=334023&status=done&style=none&title=&width=731)
