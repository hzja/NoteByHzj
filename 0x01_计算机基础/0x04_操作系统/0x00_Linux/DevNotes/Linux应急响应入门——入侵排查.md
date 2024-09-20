Linux
<a name="BQFVn"></a>
## 账号安全
<a name="npni3"></a>
### 1、用户信息文件 `/etc/passwd`
```bash
# 格式：account:password:UID:GID:GECOS:directory:shell
# 用户名：密码：用户ID：组ID：用户说明：家目录：登陆之后的 shell
root:x:0:0:root:/root:/bin/bash
```
```bash
# 查看可登录用户：
cat /etc/passwd | grep /bin/bash
# 查看UID=0的用户
awk -F: '$3==0{print $1}' /etc/passwd
# 查看sudo权限的用户
more /etc/sudoers | grep -v "^#\|^$" | grep "ALL=(ALL)"
```
:::tips
注意：无密码只允许本机登陆，远程不允许登陆
:::
<a name="4A4y4"></a>
### 2、影子文件：`/etc/shadow`
```
# 用户名：加密密码：密码最后一次修改日期：两次密码的修改时间间隔：密码有效期：密码修改到期到的警告天数：密码过期之后的宽限天数：账号失效时间：保留
root:$6$oGs1PqhL2p3ZetrE$X7o7bzoouHQVSEmSgsYN5UD4.kMHx6qgbTqwNVC5oOAouXvcjQSt.Ft7ql1WpkopY0UV9ajBwUt1DpYxTCVvI/:16809:0:99999:7:::
```
<a name="X8821"></a>
### 3、查看当前登录用户及登录时长
```bash
who     # 查看当前登录系统的所有用户（tty 本地登陆  pts 远程登录）
w       # 显示已经登录系统的所用用户，以及正在执行的指令
uptime  # 查看登陆多久、多少用户，负载状态
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1610287436680-1d7d69cc-c94a-4c99-bd53-9d3b99b1a115.png#height=217&id=fZ0BD&originHeight=652&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=753777&status=done&style=none&title=&width=1107.6666666666667)
<a name="WBIpT"></a>
### 4、排查用户登录信息
<a name="8DrLP"></a>
#### 查看最近登录成功的用户及信息
```bash
# 显示logged in表示用户还在登录
# pts表示从SSH远程登录
# tty表示从控制台登录，就是在服务器旁边登录
last
```
<a name="Evy9e"></a>
#### 查看最近登录失败的用户及信息
```bash
# ssh表示从SSH远程登录
# tty表示从控制台登录
sudo lastb
```
<a name="cWO9Q"></a>
#### 显示所有用户最近一次登录信息
```bash
lastlog
```
在排查服务器的时候，黑客没有在线，可以使用`last`命令排查黑客什么时间登录的有的黑客登录时，会将`/var/log/wtmp`文件删除或者清空，这样就无法使用last命令获得有用的信息了。
:::info
**在黑客入侵之前，必须使用**`**chattr +a**`**对**`**/var/log/wtmp**`**文件进行锁定，避免被黑客删除**
:::
<a name="b15c4e64"></a>
### 5、sudo用户列表
```bash
/etc/sudoers
```
<a name="3b96ae4c"></a>
## 入侵排查
```bash
# 查询特权用户特权用户(uid 为0)：
awk -F: '$3==0{print $1}' /etc/passwd
# 查询可以远程登录的帐号信息：
awk '/\$1|\$6/{print $1}' /etc/shadow
# 除root帐号外，其他帐号是否存在sudo权限。如非管理需要，普通帐号应删除sudo权限：
more /etc/sudoers | grep -v "^#\|^$" | grep "ALL=(ALL)"
# 禁用或删除多余及可疑的帐号
usermod -L user    # 禁用帐号，帐号无法登录，/etc/shadow 第二栏为 ! 开头
userdel user       # 删除 user 用户
userdel -r user    # 将删除 user 用户，并且将 /home 目录下的 user 目录一并删除
```
**通过**`**.bash\_history**`**文件查看帐号执行过的系统命令：**<br />打开 /home 各帐号目录下的 .bash_history，查看普通帐号执行的历史命令。<br />为历史的命令增加登录的 IP 地址、执行命令时间等信息：
```bash
# 1、保存1万条命令：
sed -i 's/^HISTSIZE=1000/HISTSIZE=10000/g' /etc/profile
# 2、在/etc/profile的文件尾部添加如下行数配置信息：
USER_IP=`who -u am i 2>/dev/null | awk '{print $NF}' | sed -e 's/[()]//g'`
if [ "$USER_IP" = "" ]
then
USER_IP=`hostname`
fi
export HISTTIMEFORMAT="%F %T $USER_IP `whoami` "
shopt -s histappend
export PROMPT_COMMAND="history -a"
# 3、让配置生效
source /etc/profile
```
:::danger
注意：历史操作命令的清除：`history -c`
:::
该操作并不会清除保存在文件中的记录，因此需要手动删除`.bash\_profile`文件中的记录
<a name="u453j"></a>
### 检查端口连接情况
```bash
netstat -antlp | more
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1610287604986-b60dc32a-3906-44cd-9b8c-a5fb838847d1.png#height=583&id=mtTv2&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2223658&status=done&style=none&title=&width=1107.6666666666667)<br />使用 ps 命令，分析进程，得到相应pid号:
```bash
ps aux | grep 6666
```
![image.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1610287305431-09c2b1e7-5a99-4de5-a13b-b5df8fc829b2.gif#height=1&id=EBfj7&originHeight=1&originWidth=1&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70&status=done&style=none&title=&width=1)查看 pid 所对应的进程文件路径：
```bash
# $PID 为对应的 pid 号
ls -l /proc/$PID/exe 或 file /proc/$PID/exe
```
分析进程：
```bash
# 根据pid号查看进程
lsof -p 6071
# 通过服务名查看该进程打开的文件
lsof -c sshd
# 通过端口号查看进程：
lsof -i :22
```
查看进程的启动时间点：
```bash
ps -p 6071 -o lstart
```
根据pid强行停止进程：
```bash
kill -9 6071
```
:::tips
**注意：** 如果找不到任何可疑文件，文件可能被删除，这个可疑的进程已经保存到内存中，是个内存进程。这时需要查找PID 然后kill掉。
:::
<a name="9d7c2ef7"></a>
### 检查开机启动项
系统运行级别示意图：

| 运行级别 | 含义 |
| --- | --- |
| 0 | 关机 |
| 1 | 单用户模式，可以想象为windows的安全模式，主要用于系统修复 |
| 2 | 不完全的命令行模式，不含NFS服务 |
| 3 | 完全的命令行模式，就是标准字符界面 |
| 4 | 系统保留 |
| 5 | 图形模式 |
| 6 | 重启动 |

查看运行级别命令：
```bash
runlevel
```
开机启动配置文件:
```bash
/etc/rc.local
/etc/rc.d/rc[0~6].d
```
启动Linux系统时，会运行一些脚本来配置环境——rc脚本。在内核初始化并加载了所有模块之后，内核将启动一个守护进程叫做`init`或`init.d`。这个守护进程开始运行`/etc/init.d/rc`中的一些脚本。这些脚本包括一些命令，用于启动运行Linux系统所需的服务<br />开机执行脚本的两种方法：

- 在`/etc/rc.local`的`exit 0`语句之间添加启动脚本。脚本必须具有可执行权限
- 用`update-rc.d`命令添加开机执行脚本

1、编辑修改`/etc/rc.local`<br />2、`update-rc.d`：此命令用于安装或移除System-V风格的初始化脚本连接。脚本是存放在`/etc/init.d/`目录下的，当然可以在此目录创建连接文件连接到存放在其他地方的脚本文件。<br />此命令可以指定脚本的执行序号，序号的取值范围是 0-99，序号越大，越迟执行。
:::success
**当需要开机启动自己的脚本时，只需要将可执行脚本丢在**`**/etc/init.d**`**目录下，然后在**`**/etc/rc.d/rc_.d**`**文件中建立软链接即可**
:::
语法：

- `update-rc.d 脚本名或服务 <remove|defaults|disable|enable>`
```bash
#1、在/etc/init.d目录下创建链接文件到后门脚本：
ln -s /home/b4yi/kali-6666.elf /etc/init.d/backdoor
#2、用 update-rc.d 命令将连接文件 backdoor 添加到启动脚本中去
sudo update-rc.d backdoor defaults 99
```
开机即执行。
<a name="b05rt"></a>
### 入侵排查
```bash
more /etc/rc.local
/etc/rc.d/rc[0~6].d
ls -l /etc/rc.d/rc3.d/
```
<a name="ha9w6"></a>
#### 计划任务排查
需要注意的几处利用cron的路径：
```bash
crontab -l  # 列出当前用户的计时器设置
crontab -r  # 删除当前用户的cron任务
```
上面的命令实际上是列出了`/var/spool/cron/crontabs/root`该文件的内容。

- `/etc/crontab`只允许root用户修改
- `/var/spool/cron/`存放着每个用户的crontab任务，每个任务以创建者的名字命名
- `/etc/cron.d/`将文件写到该目录下，格式和`/etc/crontab`相同
- 把脚本放在`/etc/cron.hourly/`、`/etc/cron.daily/`、`/etc/cron.weekly/`、`/etc/cron.monthly/`目录中，让它每小时/天/星期/月执行一次

小技巧：
```bash
more /etc/cron.daily/*  查看目录下所有文件
```
<a name="AvD63"></a>
#### 恶意脚本检查
重点关注以下目录中是否存在恶意脚本;
```bash
/var/spool/cron/* 
/etc/crontab
/etc/cron.d/*
/etc/cron.daily/* 
/etc/cron.hourly/* 
/etc/cron.monthly/*
/etc/cron.weekly/
/etc/anacrontab
/var/spool/anacron/*
```
<a name="0IqDR"></a>
#### 安装的服务功能排查
查询已安装的服务：<br />RPM 包安装的服务：
```bash
chkconfig  --list  查看服务自启动状态，可以看到所有的RPM包安装的服务
ps aux | grep crond 查看当前服务
系统在3与5级别下的启动项 
中文环境
chkconfig --list | grep "3:启用\|5:启用"
英文环境
chkconfig --list | grep "3:on\|5:on"
```
源码包安装的服务:
```bash
查看服务安装位置 ，一般是在/user/local/
service httpd start
搜索/etc/rc.d/init.d/  查看是否存在
```
<a name="1Hstk"></a>
### 异常文件检查
按照三种方式查找修改的文件：
> - 按照名称
> - 依据文件大小
> - 按照时间查找

- 根据名称查找文件
```bash
find / -name a.Test
# 如果文件名记不全，可使用通配符*来补全
# 如果不区分大小写，可以将-name 替换为-iname
```

- 依据文件大小查找：
```bash
find / -size +1000M
# +1000M表示大于1000M的文件，-10M代表小于10M的文件
```

- 依据时间查找：
```bash
# -atime 文件的访问时间
# -mtime 文件内容修改时间
# -ctime 文件状态修改时间（文件权限，所有者/组，文件大小等，当然文件内容发生改变，ctime也会随着改变）
# 要注意：系统进程/脚本访问文件，atime/mtime/ctime也会跟着修改，不一定是人为的修改才会被记录
# 查找最近一天以内修改的文件：
find / -mtime -1 -ls  | more 
# 查找50天前修改的文件：
find ./ -mtime +50 -ls
```

- 根据属主和属组查找：
```bash
-user 根据属主查找
-group 根据属组查找
-nouser 查找没有属主的文件
-nogroup 查找没有属组的文件
# 查看属主是root的文件
find ./ -user root -type f
# -type f表示查找文件，-type d表示查找目录
# 注意：系统中没有属主或者没有属组的文件或目录，也容易造成安全隐患，建议删除。
```

- 按照CPU使用率从高到低排序：
```bash
ps -ef --sort -pcpu
```

- 按照内存使用率从高到低排序：
```bash
ps -ef --sort -pmem
```
补充：<br />1、查看敏感目录，如/tmp目录下的文件，同时注意隐藏文件夹，以“..”为名的文件夹具有隐藏属性。<br />2、得到发现WEBSHELL、远控木马的创建时间，如何找出同一时间范围内创建的文件？<br />可以使用find命令来查找，如`find /opt -iname "*" -atime 1 -type f` 找出 /opt 下一天前访问过的文件。<br />3、针对可疑文件可以使用 stat 进行创建修改时间。
<a name="xfHVA"></a>
### 系统日志检查
日志默认存放位置：`/var/log/`<br />必看日志：`secure、history`<br />查看日志配置情况：`more /etc/rsyslog.conf`

| 日志文件 | 说明 |
| --- | --- |
| /var/log/cron | 记录了系统定时任务相关的日志 |
| /var/log/cups | 记录打印信息的日志 |
| /var/log/dmesg | 记录了系统在开机时内核自检的信息，也可以使用dmesg命令直接查看内核自检信息 |
| /var/log/mailog | 记录邮件信息 |
| /var/log/message | 记录系统重要信息的日志。这个日志文件中会记录Linux系统的绝大多数重要信息，如果系统出现问题时，首先要检查的就应该是这个日志文件 |
| /var/log/btmp | 记录错误登录日志，这个文件是二进制文件，不能直接vi查看，而要使用lastb命令查看 |
| /var/log/lastlog | 记录系统中所有用户最后一次登录时间的日志，这个文件是二进制文件，不能直接vi，而要使用lastlog命令查看 |
| /var/log/wtmp | 永久记录所有用户的登录、注销信息，同时记录系统的启动、重启、关机事件。同样这个文件也是一个二进制文件，不能直接vi，而需要使用last命令来查看 |
| /var/log/utmp | 记录当前已经登录的用户信息，这个文件会随着用户的登录和注销不断变化，只记录当前登录用户的信息。同样这个文件不能直接vi，而要使用w,who,users等命令来查询 |
| /var/log/secure | 记录验证和授权方面的信息，只要涉及账号和密码的程序都会记录，比如SSH登录，su切换用户，sudo授权，甚至添加用户和修改用户密码都会记录在这个日志文件中 |

```bash
/var/log/wtmp 登录进入，退出，数据交换、关机和重启纪录
/var/log/lastlog 文件记录用户最后登录的信息，可用 lastlog 命令来查看。
/var/log/secure 记录登入系统存取数据的文件，例如 pop3/ssh/telnet/ftp 等都会被记录。
/var/log/cron 与定时任务相关的日志信息
/var/log/message 系统启动后的信息和错误日志
/var/log/apache2/access.log apache access log
```
/var/log/messages 一包括整体系统信息，其中也包含系统启动期间的日志。此外。mail，cron，daemon，kern和auth等内容也记录在var/log/messages日志中。<br />/var/log/dmesg 一包含内核缓冲信息（kernel ring buffer)。在系统启动时，会在屏幕上显示许多与硬件有关的信息。可以用dmesg查看它们。<br />/var/log/auth.log -包含系统授权信息,包括用户登录和使用的权限机制等。<br />/var/ log/boot.log -包含系统启动时的日志。<br />/var/log/daemon. log -包含各种系统后台守护进程日志信息。<br />/var/log/dpkg.log -包括安装或dpkg命令清除软件包的日志。<br />/var/log/kern.log -包含内核产生的日志，有助于在定制内核时解决问题。<br />/var/1og/lastlog -记录所有用户的最近信息。这不是一个ASCII文件，因此需要用lastlog命令查看内容。<br />/var/log/maillog、/var/log/mail.log 一包含来着系统运行电子邮件服务器的日志信息。例如，sendmail日志信息就全部送到这个文件中。<br />/var/log/user.log -记录所有等级用户信息的日志。<br />/var/log/Xorg.x.log -来自X的日志信息。<br />/var/log/alternatives.log -更新替代信息都记录在这个文件中。<br />/var/log/btmp -记录所有失败登录信息。使用last命令可以查看btmp文件。例如，"`last -f /var/log/btmp | more`"。<br />/var/log/cups -涉及所有打印信息的日志。<br />/var/log/anaconda.log –在安装Linux时，所有安装信息都储存在这个文件中。<br />/var/log/yum.log -包含使用yum安装的软件包信息。<br />/var/log/cron - 每当cron进程开始一个工作时，就会将相关信息记录在这个文件中。<br />/var/log/secure -包含验证和授权方面信息。例如osshd会将所有信息记录（其中包括失败登录〉在这里。<br />/var/log/wtmp或/var/log/utmp - 包含登录信息。使用wtmp可以找出谁正在登陆进入系统，谁使用命令显示这个文件或信息等。<br />/var/log/faillog -包含用户登录失败信息。此外，错误登录命令也会记录在本文件中。<br />除了上述Log文件以外，/var/log还基于系统的具体应用包含以下一些子目录:<br />/var/log/httpd/或/var/log/apache2 一包含服务器access_log和error_log信息。<br />/var/log/lighttpd/-包含light HTTPD的access_log和error_log。<br />/var/log/mail/–这个子目录包含邮件服务器的额外日志。<br />/var/ log/prelink/-包含.so文件被prelink修改的信息。<br />/var/log/audit/-包含被Linux audit daemon储存的信息。<br />/var/log/samba/ -包含由samba存储的信息。<br />/var/log/sa/ -包含每日由sysstat软件包收集的sar文件。<br />/var/log/sssd/ -用于守护进程安全服务。<br />日志分析技巧：
```bash
1、定位有多少IP在爆破主机的root帐号：    
grep "Failed password for root" /var/log/secure | awk '{print $11}' | sort | uniq -c | sort -nr | more
定位有哪些IP在爆破：
grep "Failed password" /var/log/secure|grep -E -o "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"|uniq -c
爆破用户名字典是什么？
grep "Failed password" /var/log/secure|perl -e 'while($_=<>){ /for(.*?) from/; print "$1\n";}'|uniq -c|sort -nr
 
2、登录成功的IP有哪些：   
grep "Accepted " /var/log/secure | awk '{print $11}' | sort | uniq -c | sort -nr | more
登录成功的日期、用户名、IP：
grep "Accepted " /var/log/secure | awk '{print $1,$2,$3,$9,$11}' 
3、增加一个用户kali日志：
Jul 10 00:12:15 localhost useradd[2382]: new group: name=kali, GID=1001
Jul 10 00:12:15 localhost useradd[2382]: new user: name=kali, UID=1001, GID=1001, home=/home/kali
, shell=/bin/bash
Jul 10 00:12:58 localhost passwd: pam_unix(passwd:chauthtok): password changed for kali
#grep "useradd" /var/log/secure 
4、删除用户kali日志：
Jul 10 00:14:17 localhost userdel[2393]: delete user 'kali'
Jul 10 00:14:17 localhost userdel[2393]: removed group 'kali' owned by 'kali'
Jul 10 00:14:17 localhost userdel[2393]: removed shadow group 'kali' owned by 'kali'
# grep "userdel" /var/log/secure
5、su切换用户：
Jul 10 00:38:13 localhost su: pam_unix(su-l:session): session opened for user good by root(uid=0)
sudo授权执行:
sudo -l
Jul 10 00:43:09 localhost sudo:    good : TTY=pts/4 ; PWD=/home/good ; USER=root ; COMMAN
```
<a name="vlwHy"></a>
## WebShell查杀
河马 WebShell 查杀：[http://www.shellpub.com](http://www.shellpub.com)
<a name="6nsPF"></a>
## Linux安全检查脚本
[https://github.com/grayddq/GScan](https://github.com/grayddq/GScan)<br />[https://github.com/ppabc/security_check](https://github.com/ppabc/security_check)<br />[https://github.com/T0xst/linux](https://github.com/T0xst/linux)
