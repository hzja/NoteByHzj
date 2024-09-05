Linux at
<a name="TFqDR"></a>
## 任务类型
定时:   按照一定的周期重复持续的做某个事。<br />临时:   执行一次以后任务就结束了，只一次。
<a name="lHJHc"></a>
## 任务类型对应的命令
crontab( 定时任务): 按照规定的周期循环执行。<br />at (临时任务): 执行一次就结束的任务。
<a name="KzsgU"></a>
## 安装 at 命令
一般来讲，大部分 Linux 发行版都已经安装了 at 命令，不确定的话可以使用 `at -V` 命令查看，如果有版本返回，说明就可以正常使用 at 命令。
```bash
$ at -V
at version 3.1.13
```
可能还需要启动 at 守护进程，称为 `atd `。在大多数 Linux 系统上，可以使用 `systemctl` 命令启用 atd 服务并将它们设置为从现在开始自动启动：
```bash
$ sudo systemctl enable --now atd
```
<a name="WJ2fc"></a>
### at的使用
命令：at<br />语法:    at 时间<br />常用参数:
```
-c  查看任务详情
-f：指定包含具体指令的任务文件
-q：指定新任务的队列名称
-l：显示待执行任务的列表
-d：删除指定的待执行任务
-m：任务执行完成后向用户发送 E-mail
```
操作：查看服务是否开启
```bash
➜  /etc systemctl status atd
● atd.service - Job spooling tools
   Loaded: loaded (/usr/lib/systemd/system/atd.service; enabled; vendor preset: enabled)
   Active: active (running) since Wed 2019-08-14 19:32:11 CST; 10 months 28 days ago
 Main PID: 459 (atd)
   CGroup: /system.slice/atd.service
           └─459 /usr/sbin/atd -f

Warning: Journal has been rotated since unit was started. Log output is incomplete or unavailable.
➜  /etc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594562978609-b1015969-895e-4539-a540-7985bb903ab5.png#height=187&id=BoifC&originHeight=561&originWidth=2816&originalType=binary&ratio=1&size=553324&status=done&style=none&width=938.6666666666666)<br />几种设定任务的方法: 
<a name="tPoJ5"></a>
#### 创建一个任务(默认是今天,如果时间大于当前时间会创建在下一天 ）
```bash
➜  /etc at 21:00
at> ert
at> <EOT>
job 5 at Mon Jul 13 21:00:00 2020
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594564355652-7208d91a-a842-42ac-91d2-071251089195.png#height=169&id=ol5js&originHeight=507&originWidth=1786&originalType=binary&ratio=1&size=412465&status=done&style=none&width=595.3333333333334)
<a name="G0T85"></a>
#### 指定时间创建
```bash
➜  /etc at 21:00 2020-8-15
at> mkdir happy
at> <EOT>
job 6 at Sat Aug 15 21:00:00 2020
➜  /etc atq
2       Sun Jul 12 23:00:00 2020 a root
6       Sat Aug 15 21:00:00 2020 a root
5       Mon Jul 13 21:00:00 2020 a root
4       Sun Jul 12 23:00:00 2020 a root
➜  /etc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594564492956-6b35b028-a94b-4927-8673-5c750088fe12.png#height=188&id=bnN3D&originHeight=563&originWidth=1342&originalType=binary&ratio=1&size=309252&status=done&style=none&width=447.3333333333333)
<a name="3a2Pe"></a>
#### 从现在开始加时间
```bash
➜  /etc at now +3min
at> mkdir work
at> <EOT>
job 7 at Sun Jul 12 22:39:00 2020
➜  /etc atq
2       Sun Jul 12 23:00:00 2020 a root
6       Sat Aug 15 21:00:00 2020 a root
5       Mon Jul 13 21:00:00 2020 a root
4       Sun Jul 12 23:00:00 2020 a root
7       Sun Jul 12 22:39:00 2020 a root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594564622824-21790816-889e-4f43-b8fe-3b9172aeb90b.png#height=204&id=lm95v&originHeight=612&originWidth=1154&originalType=binary&ratio=1&size=297211&status=done&style=none&width=384.6666666666667)
<a name="DNGeH"></a>
#### 明天11:11 执行
```bash
➜  /etc at 11:11 tomorrow
at> mkdir morrow
at> <EOT>
job 8 at Mon Jul 13 11:11:00 2020
➜  /etc
➜  /etc atq
8       Mon Jul 13 11:11:00 2020 a root
2       Sun Jul 12 23:00:00 2020 a root
6       Sat Aug 15 21:00:00 2020 a root
5       Mon Jul 13 21:00:00 2020 a root
4       Sun Jul 12 23:00:00 2020 a root
7       Sun Jul 12 22:39:00 2020 a root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594564744815-37ea0e65-3579-414c-a15d-c69ee89277e0.png#height=238&id=kkxpT&originHeight=715&originWidth=1329&originalType=binary&ratio=1&size=384664&status=done&style=none&width=443)
<a name="KpPjz"></a>
#### 2天后的下午3.24执行
```bash
➜  /etc at 3:45 pm + 2days
at> mkdir others
at> <EOT>
job 9 at Tue Jul 14 15:45:00 2020
➜  /etc atq
8       Mon Jul 13 11:11:00 2020 a root
9       Tue Jul 14 15:45:00 2020 a root
2       Sun Jul 12 23:00:00 2020 a root
6       Sat Aug 15 21:00:00 2020 a root
5       Mon Jul 13 21:00:00 2020 a root
4       Sun Jul 12 23:00:00 2020 a root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594564907178-75d8e435-7f43-4eae-b11e-47110caae67b.png#height=223&id=mAoFL&originHeight=669&originWidth=1399&originalType=binary&ratio=1&size=375001&status=done&style=none&width=466.3333333333333)
<a name="uYO7o"></a>
#### 查看任务列表(显示还未执行的任务 ）
```java
➜  /etc atq
8       Mon Jul 13 11:11:00 2020 a root
9       Tue Jul 14 15:45:00 2020 a root
2       Sun Jul 12 23:00:00 2020 a root
6       Sat Aug 15 21:00:00 2020 a root
5       Mon Jul 13 21:00:00 2020 a root
4       Sun Jul 12 23:00:00 2020 a root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594565144147-739458bb-a05e-4e93-a9ee-3f4d71cbb309.png#height=150&id=lJCJo&originHeight=451&originWidth=1185&originalType=binary&ratio=1&size=221861&status=done&style=none&width=395)
<a name="TauVN"></a>
#### 查看一个单个的任务信息:(由于内容较多去掉部分信息)
```java
➜  /etc at -c 9
#!/bin/sh
# atrun uid=0 gid=0
# mail root 0
umask 22
USER=root; export USER
LOGNAME=root; export LOGNAME
HOME=/root; export HOME
PATH=/home/oracle/app/oracle/product/11.2.0/dbhome_2/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin:/opt/module/jdk1.8.0_161/bin:/opt/module/hadoop-2.7.7/bin:/opt/module/hadoop-2.7.7/sbin; export PATH
MAIL=/var/spool/mail/root; export MAIL
SHELL=/bin/zsh; export SHELL
SSH_CLIENT=223.64.72.123\ 24346\ 22; export SSH_CLIENT
SSH_CONNECTION=223.64.72.123\ 24346\ 172.16.252.76\ 22; export SSH_CONNECTION
SSH_TTY=/dev/pts/0; export SSH_TTY
XDG_SESSION_ID=57916; export XDG_SESSION_ID
XDG_RUNTIME_DIR=/run/user/0; export XDG_RUNTIME_DIR
SHLVL=1; export SHLVL
PWD=/etc; export PWD
OLDPWD=/; export OLDPWD
HISTCONTROL=ignoredups; export HISTCONTROL
HOSTNAME=fcant-hadoop001; export HOSTNAME
HISTSIZE=50000; export HISTSIZE
LS_COLORS=rs=0:di=38\;5\;27:ln=38\;5\;51:mh=44\;38\;5\;15:pi=40\;38\;5\;11:so=38\;5\;13:do=38\;5\;5:bd=48\;5\;232\;38\;5\;11:cd=48\;5\;232\;38\;5\;3:or=48\;5\;232\;38\;5\;9:mi=05\;48\;5\;232\;38\;5\;15:su=48\;5\;196\;38\;5\;15:sg=48\;5\;11\;38\;5\;16:ca=48\;5\;196\;38\;5\;226:tw=48\;5\;10\;38\;5\;16:ow=48\;5\;10\;38\;5\;21:st=48\;5\;21\;38\;5\;15:ex=38\;5\;34:\*.tar=38\;5\;9:\*.tgz=38\;5\;9:\*.arc=38\;5\;9:\*.arj=38\;5\;9:\*.taz=38\;5\;9:\*.lha=38\;5\;9:\*.lz4=38\;5\;9:\*.lzh=38\;5\;9:\*.lzma=38\;5\;9:\*.tlz=38\;5\;9:\*.txz=38\;5\;9:\*.tzo=38\;5\;9:\*.t7z=38\;5\;9:\*.zip=38\;5\;9:\*.z=38\;5\;9:\*.Z=38\;5\;9:\*.dz=38\;5\;9:\*.gz=38\;5\;9:\*.lrz=38\;5\;9:\*.lz=38\;5\;9:\*.lzo=38\;5\;9:\*.xz=38\;5\;9:\*.bz2=38\;5\;9:\*.bz=38\;5\;9:\*.tbz=38\;5\;9:\*.tbz2=38\;5\;9:\*.tz=38\;5\;9:\*.deb=38\;5\;9:\*.rpm=38\;5\;9:\*.jar=38\;5\;9:\*.war=38\;5\;9:\*.ear=38\;5\;9:\*.sar=38\;5\;9:\*.rar=38\;5\;9:\*.alz=38\;5\;9:\*.ace=38\;5\;9:\*.zoo=38\;5\;9:\*.cpio=38\;5\;9:\*.7z=38\;5\;9:\*.rz=38\;5\;9:\*.cab=38\;5\;9:\*.jpg=38\;5\;13:\*.jpeg=38\;5\;13:\*.gif=38\;5\;13:\*.bmp=38\;5\;13:\*.pbm=38\;5\;13:\*.pgm=38\;5\;13:\*.ppm=38\;5\;13:\*.tga=38\;5\;13:\*.xbm=38\;5\;13:\*.xpm=38\;5\;13:\*.tif=38\;5\;13:\*.tiff=38\;5\;13:\*.png=38\;5\;13:\*.svg=38\;5\;13:\*.svgz=38\;5\;13:\*.mng=38\;5\;13:\*.pcx=38\;5\;13:\*.mov=38\;5\;13:\*.mpg=38\;5\;13:\*.mpeg=38\;5\;13:\*.m2v=38\;5\;13:\*.mkv=38\;5\;13:\*.webm=38\;5\;13:\*.ogm=38\;5\;13:\*.mp4=38\;5\;13:\*.m4v=38\;5\;13:\*.mp4v=38\;5\;13:\*.vob=38\;5\;13:\*.qt=38\;5\;13:\*.nuv=38\;5\;13:\*.wmv=38\;5\;13:\*.asf=38\;5\;13:\*.rm=38\;5\;13:\*.rmvb=38\;5\;13:\*.flc=38\;5\;13:\*.avi=38\;5\;13:\*.fli=38\;5\;13:\*.flv=38\;5\;13:\*.gl=38\;5\;13:\*.dl=38\;5\;13:\*.xcf=38\;5\;13:\*.xwd=38\;5\;13:\*.yuv=38\;5\;13:\*.cgm=38\;5\;13:\*.emf=38\;5\;13:\*.axv=38\;5\;13:\*.anx=38\;5\;13:\*.ogv=38\;5\;13:\*.ogx=38\;5\;13:\*.aac=38\;5\;45:\*.au=38\;5\;45:\*.flac=38\;5\;45:\*.mid=38\;5\;45:\*.midi=38\;5\;45:\*.mka=38\;5\;45:\*.mp3=38\;5\;45:\*.mpc=38\;5\;45:\*.ogg=38\;5\;45:\*.ra=38\;5\;45:\*.wav=38\;5\;45:\*.axa=38\;5\;45:\*.oga=38\;5\;45:\*.spx=38\;5\;45:\*.xspf=38\;5\;45:; export LS_COLORS
LANG=en_US.UTF-8; export LANG
LESSOPEN=\|\|/usr/bin/lesspipe.sh\ %s; export LESSOPEN
JAVA_HOME=/opt/module/jdk1.8.0_161; export JAVA_HOME
JRE_HOME=/opt/module/jdk1.8.0_161/jre; export JRE_HOME
HADOOP_HOME=/opt/module/hadoop-2.7.7; export HADOOP_HOME
ORACLE_HOME=/home/oracle/app/oracle/product/11.2.0/dbhome_2; export ORACLE_HOME
ORACLE_SID=helowin; export ORACLE_SID
ZSH=/root/.oh-my-zsh; export ZSH
PAGER=less; export PAGER
LESS=-R; export LESS
LSCOLORS=Gxfxcxdxbxegedabagacad; export LSCOLORS
cd /etc || {
         echo 'Execution directory inaccessible' >&2
         exit 1
}
${SHELL:-/bin/sh} << 'marcinDELIMITER09e30bfc'
mkdir others

marcinDELIMITER09e30bfc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594565209339-28b0ca45-7cfa-4efa-b504-3dfb8c8ed3dd.png#height=512&id=nqd7l&originHeight=1535&originWidth=3299&originalType=binary&ratio=1&size=2094806&status=done&style=none&width=1099.6666666666667)<br />参数解释:<br />5 : 任务编号<br />Thu Nov 15 21:11:00 2018 : 时间<br />a : 队列<br />root : 执行用户
<a name="kyFIQ"></a>
### 删除任务
先查看一下有哪些任务，然后根据任务ID进行删除
```java
➜  /etc atq
8       Mon Jul 13 11:11:00 2020 a root
9       Tue Jul 14 15:45:00 2020 a root
2       Sun Jul 12 23:00:00 2020 a root
6       Sat Aug 15 21:00:00 2020 a root
5       Mon Jul 13 21:00:00 2020 a root
4       Sun Jul 12 23:00:00 2020 a root
➜  /etc atrm 9
➜  /etc atq
8       Mon Jul 13 11:11:00 2020 a root
2       Sun Jul 12 23:00:00 2020 a root
6       Sat Aug 15 21:00:00 2020 a root
5       Mon Jul 13 21:00:00 2020 a root
4       Sun Jul 12 23:00:00 2020 a root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594565367944-61ac1d8e-f32b-4588-8800-cc344617cffd.png#height=253&id=LdVCY&originHeight=758&originWidth=1560&originalType=binary&ratio=1&size=462414&status=done&style=none&width=520)
<a name="fxDdZ"></a>
## 时间表达式
前面提到过，at 命令使用的是我们日常生活中所使用的时间格式，非常方便：

- YYMMDDhhmm[.ss] （缩写年、月、日、小时、分钟[秒]）
- CCYYMMDDhhmm[.ss] （完整年、月、日、小时、分钟和[秒]）
- now
- midnight
- noon
- teatime`（下午4点）
- AM
- PM

时间和日期可以是绝对的，也可以添加一个加号 ( + ) 使它们相对于现在。在指定相对时间时，下面这些日常生活中所使用的词汇都可以使用：

- minutes
- hours
- days
- weeks
- months
- years

下面是一些 at 命令有效表达式的示例：
```bash
$ echo "rsync -av /home/tux me@myserver:/home/tux/" | at 3:30 AM tomorrow
$ echo "/opt/batch.sh ~/Pictures" | at 3:30 AM 08/01/2022
$ echo "echo hello" | at now + 3 days
```
<a name="tyZ2h"></a>
## 查看 at 任务队列
随时时间的推移，可以忘记在 at 命令队列时设置了多少个任务，如果想进行查看，可以使用 `atq` 命令：
```bash
$ atq
10 Thu Jul 29 12:19:00 2021 a tux
9  Tue Jul 27 03:30:00 2021 a tux
7  Tue Jul 27 00:00:00 2021 a tux
```
要从队列中删除任务，请使用 `atrm` 命令和任务编号。例如要删除任务 7 ：
```bash
$ atrm 7
$ atq
10 Thu Jul 29 12:19:00 2021 a tux
9  Tue Jul 27 03:30:00 2021 a tux
```
如果想要查看计划任务中的具体内容，就必须查看 `at spool` 。只有 root 用户才能查看`at spool`，因此必须使用 `sudo` 来查看 `spool` 或 `cat` 任务的内容。
