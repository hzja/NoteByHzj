Linux 用户 组<br />Linux是一个多任务，多用户的操作系统。多个用户可以在同一时间操作系统执行不同的任务，一个用户也可以多个人登陆操作。一般来说root的权限最大。
<a name="bL8Wi"></a>
## 1、用户管理
<a name="5433651a"></a>
### useradd：添加用户
```bash
# 添加用户,进入/home文件夹/创建用户和主目录
$ cd /home
# 查看
$ ll
# 创建组名
$ useradd -m fcant
# 查看
$ ll
# 创建用户名
$ useradd -mg fcant fcc
# 查看
[root@mobai home]# ll
总用量 0
drwx------. 2 fcant fcant 62 5月  29 11:25 fcant
drwx------. 2 fcc   fcant 62 5月  29 11:25 fcc
```
<a name="N5dOI"></a>
### passwd：给用户设置密码
```bash
# 设置密码
$ passwd fcant
```
<a name="ir6rC"></a>
### su：切换用户
```bash
# 切换用户
$ su fcant
```
<a name="0facd94c"></a>
### userdel：删除用户
```bash
# 切换回主目录
$ su root 
# 删除用户组
$ userdel -r fcc
```
<a name="ed25072a"></a>
### usermod：修改用户登录名
```bash
# 修改登录名称
$ usermod -1 Rose fcc
# 修改用户的shell类型
$ usermod -s /bin/bash fcant
```
<a name="b43de125"></a>
### 给普通用户设置sudo超级管理员权限
```bash
# 编辑配置文件
$ vim /etc/sudoers
# 切换用户
$ su fcant
# 设置权限
$ sudo useradd -m 用户名
# 查看
$ cd /home
```
<a name="GAum0"></a>
## 2、组管理
<a name="e1d08420"></a>
### groupadd：添加组
```bash
# 添加组
$ groupadd [组名]
# 查看组
$ cat /etc/group
```
<a name="68e8211c"></a>
### gpaddwd：将用户从组中删除
```bash
# 用户从指定组中删除
$ gpasswd -d 用户 组名
# 查看
$ ll
# 查看配置文件
$ cat /etc/group
```
<a name="048d09e3"></a>
### groupmod：修改组名
```bash
# 修改组名
$ groupmod -n 新组名 组名
```
<a name="3c9e34ba"></a>
### groupdel：删除组
```bash
# 删除组
$ groupdel 组名
# 查看
$ cat /etc/group
```
<a name="S3deY"></a>
### 用户添加指定组
```bash
# 添加组
$ groupadd [组名]
# 查看组
$ cat /etc/group
```
<a name="U0bGe"></a>
## 3、用户组信息在Linux中的文件管理
<a name="93284fb6"></a>
### Linux角色分类
超级用户：root (user id --UID) 0 最高的管理权限。<br />普通用户：
```
系统用户 UID:1-999(rhel7  centos7) 1-499(rhel6)
本地用户 UID:1000+(rhel7 centos7)  500+(rhel6)
```
UID：用户身份标示,唯一标示。
<a name="f94eec39"></a>
### Linux组分类
组：<br />组分类：<br />根据账号的功能分类：

- 超级用户组：root GID:0
- 普通用户组 ：
   - 系统用户组：GID 1---999
   - 本地用户组：GID 1000+
<a name="t46Ej"></a>
### Linux用户和组的关系
一个用户属于一个组，也可以属于多个组，多个用户可以在一个组，可以在多个组。
<a name="24d5ebff"></a>
### Linux用户和组的相关配置文件

1. /etc/passwd：用户 and 属性信息。
2. /etc/group：组 and 属性信息。
3. /etc/shadow：用户密码 and 属性信息。
4. /etc/gshadow：组密码 and 属性信息。
5. /etc/default/useradd：设置添加用户规则文件。
6. /etc/login.defs：设置用户账号限制。
<a name="18b11e57"></a>
### Linux用户信息文件
用户管理中最重要的一个文件，这个文件是对所有用户开放的，每一行存一个用户的信息，每个属性之间用冒号分割。
```
cat /etc/passwd
root:x:0:0:root:/root:/bin/zsh
bin:x:1:1:bin:/bin:/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
adm:x:3:4:adm:/var/adm:/sbin/nologin
lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
```
举例：root:x:0:0:root:/root:/bin/zsh

1. 登陆用户名：root
2. 用户密码占位符：x
3. 用户UID：0
4. 用户组GID：0
5. 对用户账户的描述：root
6. 用户家目录位置：/root
7. 用户默认的shell：/bin/zsh (默认是/bin/bash)
<a name="VqGI1"></a>
### Linux密码信息文件
该文件只有root用户有读权限，每一行存一个用户的记录，每个属性用冒号分割。
```bash
➜  / cat /etc/shadow
root:$6$Jgse2L6L$Az/O00:18095:0:99999:7:::
bin:*:17110:0:99999:7:::
daemon:*:17110:0:99999:7:::
adm:*:17110:0:99999:7:::
lp:*:17110:0:99999:7:::
sync:*:17110:0:99999:7:::
shutdown:*:17110:0:99999:7:::
halt:*:17110:0:99999:7:::
mail:*:17110:0:99999:7:::
operator:*:17110:0:99999:7:::
games:*:17110:0:99999:7:::
ftp:*:17110:0:99999:7:::
nobody:*:17110:0:99999:7:::
systemd-bus-proxy:!!:17396::::::
systemd-network:!!:17396::::::
dbus:!!:17396::::::
polkitd:!!:17396::::::
tss:!!:17396::::::
sshd:!!:17396::::::
postfix:!!:17396::::::
chrony:!!:17396::::::
ntp:!!:17396::::::
nscd:!!:17396::::::
tcpdump:!!:17396::::::
dockerroot:!!:18095::::::
mysql:!!:18413::::::
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594543955762-51c0f02e-88a9-4370-b534-510fed844e95.png#averageHue=%23363636&height=528&id=cODbK&originHeight=1584&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1859406&status=done&style=none&title=&width=1107.6666666666667)<br />举例：bin:*:17110:0:99999:7::: 

1. 登录用户名：bin
2. 加密的密码：* 号代表密码被锁定
3. 最近更改密码的日期：17110代表到1970-1-1密码不可改的天数
4. 密码修改期限：99999 代表永远不用改,如果是其他数字则表示从1970-1-1内的多少天必须修改密码。
5. 更改密码最大有效天数：代表密码保持有效的最大天数。
6. 密码过期警告：密码到正式失效前有多少天(-1,永远不提示)
7. 密码过期后多少天禁用用户：可登陆，单不能操作。
8. 用户被禁用日期：多少天后账号过期，不能登陆。
9. 保留参数
<a name="49a48cd4"></a>
### Linux组信息文件
用户组的组信息存放在这，一行存一个组记录，属性用冒号分割。
```bash
➜  / cat /etc/group
root:x:0:
bin:x:1:
daemon:x:2:
sys:x:3:
adm:x:4:
tty:x:5:
disk:x:6:
lp:x:7:
mem:x:8:
kmem:x:9:
wheel:x:10:
cdrom:x:11:
mail:x:12:postfix
man:x:15:
dialout:x:18:
floppy:x:19:
games:x:20:
tape:x:30:
video:x:39:
ftp:x:50:
lock:x:54:
audio:x:63:
nobody:x:99:
users:x:100:
utmp:x:22:
utempter:x:35:
input:x:999:
systemd-journal:x:190:
systemd-bus-proxy:x:998:
systemd-network:x:192:
dbus:x:81:
polkitd:x:997:
ssh_keys:x:996:
dip:x:40:
tss:x:59:
sshd:x:74:
postdrop:x:90:
postfix:x:89:
chrony:x:995:
ntp:x:38:
nscd:x:28:
tcpdump:x:72:
cgred:x:994:
dockerroot:x:993:
mysql:x:1000:
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594544050688-a81ebc12-f71a-4185-ac4e-8b6a136ad009.png#averageHue=%23373737&height=528&id=Fhbnk&originHeight=1585&originWidth=961&originalType=binary&ratio=1&rotation=0&showTitle=false&size=534052&status=done&style=none&title=&width=320.3333333333333)<br />举例：root:x:0:

1. 组名：root
2. 组密码占位符：x
3. 组GID：0
<a name="cPHRT"></a>
## 4、id—查看用户的 UID\GID 及所属的用户组等信息
<a name="5XDh0"></a>
### 命令简介
id 命令用于查看用户的 UID\GID 及所属的用户组等信息。<br />需要注意的就是：UID在系统中是唯一的，也是用户的身份标识，但GID可以对应多个UID(简单理解就是一个用户可以加入多个用户组)。
<a name="TvNCF"></a>
### 语法格式
```
id [OPTIONS][用户名称]
```
<a name="eS5uG"></a>
### 选项说明
```
-g  #显示用户所属群组的ID
-G   #显示用户所属附加群组的ID
-n  #显示用户，所属群组或附加群组的名称
-r  #显示实际ID
-u  #显示用户ID
-Z   #仅打印当前用户的安全上下文
-help   #打印帮助信息
-version      #打印版本信息
```
<a name="5JZos"></a>
### 应用举例
```bash
[root@centos7 ~]# id
uid=0(root) gid=0(root) groups=0(root)
#root 的 UID = 0，GID = 0
[root@centos7 ~]# id -a
uid=0(root) gid=0(root) groups=0(root)
[root@centos7 ~]# id -g
0
[root@centos7 ~]# id -G
0
[root@centos7 ~]# id mingongge
uid=1000(mingongge) gid=1000(mingongge) groups=1000(mingongge)
```
