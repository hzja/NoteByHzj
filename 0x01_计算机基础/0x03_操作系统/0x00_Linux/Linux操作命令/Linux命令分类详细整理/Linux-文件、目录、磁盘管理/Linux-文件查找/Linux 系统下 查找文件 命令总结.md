Linux
<a name="vZoVV"></a>
## 查命令绝对路径：
`which`用于查找并显示给定命令的绝对路径，环境变量中PATH参数也可以被查出来。
```bash
[root@localhost ~]# which bash
/usr/bin/bash

[root@localhost ~]# which ls
alias ls='ls --color=auto'
    /usr/bin/ls
```
<a name="tv8Gw"></a>
## 寻找特定文件：
`whereis`命令用来定位指令的二进制程序、源代码文件和man手册页等相关文件的路径，该命令只能用于程序名的搜索
```bash
[root@localhost ~]# whereis --help

语法格式：[ whereis [选项] 文件名 ]

        -b              #只找二进制文件
        -m              #只找man文档
        -s              #只找源代码
```
使用 `whereis -b` 命令找二进制文件，与帮助手册。
```bash
[root@localhost ~]# whereis -b ifconfig
ifconfig: /usr/sbin/ifconfig

[root@localhost ~]# whereis -m ifconfig
ifconfig: /usr/share/man/man8/ifconfig.8.gz
```
<a name="HWZWs"></a>
## 缓存查找文件：
locate 搜索一个数据库/var/lib/mlocatedb，这个数据库中含有本地所有文件信息，Linux系统自动创建这个数据库，并且每天自动更新一次，所以使用locate命令查不到最新变动过的文件，为了避免这种情况，可以在使用locate之前，先使用updatedb命令，手动更新数据库，updatedb命令会根据/etc/updatedb.conf来更新文件。
```bash
[root@localhost ~]# yum install -y mlocate
[root@localhost ~]# locate --help

语法格式：[ locate [选项] 文件名 ]

        -d 目录        #指定数据库所在的目录
        -i             #忽略大小写差异
        -r             #后面接正则表达式
```
使用 locate 命令查询一个文件.
```bash
[root@localhost ~]# updatedb 
[root@localhost ~]# locate /etc/passwd
/etc/passwd
/etc/passwd-
```
<a name="yeAhA"></a>
## 遍历文件查找：
find 命令可以说是最重要的查找命令了，该命令参数较多。
```bash
[root@localhost ~]# find --help

语法格式：[ find [目录] [属性] 文件名 ]

        -name         #按文件名查找
        -size         #根据大小查找
        -user         #根据属主查找
        -perm         #根据权限查找
        -type         #根据类型查找
        -time         #按时间查找
        -inum         #根据i节点查询
        -exec         #查找后执行命令
```
<a name="ugRwE"></a>
### `-name` 按文件名查找
常用查询通配符
```bash
\*     #匹配任意一个或多个字符
？     #匹配任意一个字符
[]     #指定范围,外侧加引号
```
查找/var/目录下，以.log结尾的文件。
```bash
[root@localhost ~]# find /var/ -name "*.log"
/var/log/tuned/tuned.log
/var/log/audit/audit.log
/var/log/anaconda/X.log
/var/log/anaconda/program.log
....省略....
```
查找/root/目录下，以[1-3之间]，结尾是.txt的文件
```bash
[root@localhost ~]# ls
1.txt  2.txt  3.txt  Catalog  File

[root@localhost ~]# find /root/ -name "[1-3].txt"
/root/1.txt
/root/2.txt
/root/3.txt
```
查找/etc/目录下，开头是6个任意字符的文件
```bash
[root@localhost ~]# find /etc/ -name "??????"
/etc/grub.d
/etc/grub.d/README
/etc/shells
/etc/init.d
....省略....
```
<a name="Wk7KX"></a>
### `-size` 根据大小查找
单位是 block 数据块  一块是512字节<br />1M -> 1024k -> 2048 块  (1块是0.5k 也就是512字节)<br />100M -> 102400k -> 204800块<br />查找/etc/目录下，小于10k的文件
```bash
root@localhost ~]# find /etc/ -size -10k
/etc/crypttab
/etc/.pwd.lock
/etc/environment
....省略....
```
查找/etc/目录下，大于1M的文件
```bash
[root@localhost ~]# find /etc/ -size +1M   #查询大于1M的文件
/etc/udev/hwdb.bin
/etc/selinux/targeted/active/policy.kern
/etc/selinux/targeted/contexts/files/file_contexts.bin
/etc/selinux/targeted/policy/policy.31
....省略....

#注意：+-号如果没有,是精确到这么大,通常都会带上+或-号表示一个范围.
```
<a name="vlF0A"></a>
### `-user` 根据属主与权限查找
在/root目录中查找属于wang用户的文件
```bash
[root@localhost ~]# find /root/ -user wang
/root/1.txt
/root/2.txt
/root/3.txt
#注意：系统中要存在该用户,否则会报错误.
```
查找/boot/目录中权限是644的文件
```bash
[root@localhost ~]# find /boot/ -perm 0644
/boot/grub2/device.map
/boot/grub2/i386-pc/gcry_rmd160.mod
/boot/grub2/i386-pc/acpi.mod
/boot/grub2/i386-pc/gcry_rsa.mod
....省略....
```
<a name="WYdMF"></a>
### `-type` 根据类型查找
```bash
-type f 二进制文件（普通文件）
-type l 软链接文件
-type d 目录
```
查找/usr/bin/目录下，类型是二进制文件.
```bash
[root@localhost ~]# find /usr/bin/ -type f
/usr/bin/cp
/usr/bin/gzip
/usr/bin/alias
/usr/bin/csplit
/usr/bin/bash
....省略....
```
<a name="ui1zA"></a>
### `-time` 按时间查找
```bash
按天数   ctime  atime  mtime
按分钟   cmin   amin     mmin

  c  change   #表示属性被修改过：所有者、所属组、权限
  a  access   #被访问过(被查看过)
  m  modify   #表示内容被修改过
```
查找/etc/目录下，在120分钟以内，内容被修改过的文件
```bash
[root@localhost ~]# find /etc/ -mmin -120
/etc/
/etc/resolv.conf
/etc/group-
/etc/gshadow-
/etc/group
/etc/gshadow
....省略....
```
查找/etc/目录下，在7天之前，属性被修改过的文件
```bash
[root@localhost ~]# find /etc/ -ctime +7
/etc/resolv.conf
/etc/group-
/etc/gshadow-
....省略....
```
<a name="wXS3O"></a>
### `-inum` 根据i节点查询
有一些文件的硬链接数量很多，有相同的i节点，查找其中一个文件的i节点号，一次性删除。
```bash
[root@localhost ~]# find ./ -inum 1024 -exec rm{} \;   #删除相同I节点的数据
```
<a name="pIffD"></a>
### `-and or` 逻辑连接符
```bash
-a    （and 逻辑与）     
-o    （or  逻辑或)
```
在/etc/目录下，查找大于1k，并且小于10k的文件
```bash
[root@localhost ~]# find /etc/ -size +1k -a -size -10k
/etc/
/etc/grub.d/00_header
/etc/grub.d/20_ppc_terminfo
/etc/grub.d/00_tuned
/etc/rc.d/init.d/README
/etc/rc.d/init.d/netconsole
/etc/rc.d/init.d/network
/etc/pam.d
....省略....
```
<a name="aPkmE"></a>
### `-exec` 命令执行连接符
```bash
[查询格式] find  ...  -exec 命令 {}  \;

{}        #表示find查询的结果集
\         #是转义符,不使用命令别名,直接使用命令本身
;         #分号是表示语句的结束.

#注意：固定格式,只能这样写.注意中间的空格.
-----------------------------------------------------------------
说明： 转义符的作用是什么？

在linux中有一个别名机制，如rm删除文件，执行的却是rm -i(用which rm 可以查看命令别名)，
使用rm删除文件前会提示，就是因为rm -i这个参数。如果想使用命令原意，可以在加\转义，
如：\rm test.txt   则不会提示，直接删除
```
查找/var/log/目录下名字以.log结尾的文件，找到后执行 `ls -l` 显示详细信息。
```bash
[root@localhost ~]# find /var/log/ *.log -exec ls -l {} \;
total 1176
drwxr-xr-x. 2 root   root      204 Sep 18 09:12 anaconda
drwx------. 2 root   root       23 Sep 18 09:12 audit
-rw-------. 1 root   root    53001 Sep 19 00:57 boot.log
-rw-------. 1 root   utmp      384 Sep 18 09:22 btmp
drwxr-xr-x. 2 chrony chrony      6 Apr 12 13:37 chrony
-rw-------. 1 root   root     3523 Sep 19 01:01 cron
-rw-r--r--  1 root   root   119414 Sep 19 00:57 dmesg
-rw-r--r--  1 root   root   119599 Sep 18 23:35 dmesg.old
-rw-r--r--. 1 root   root     1320 Sep 19 00:23 firewalld
-rw-r--r--. 1 root   root      193 Sep 18 09:05 grubby_prune_debug
....
```
查找/etc/目录下名字以"init*"开头的文件，找到后，只列出文件，过滤掉目录，并执行 `ls -l` 显示详细信息.
```bash
[root@localhost ~]# find /etc/ -name "init*" -a -type f -exec ls -l {} \;

-rw-r--r--. 1 root root 511 Apr 11 01:09 /etc/inittab
-rw-r--r--. 1 root root 798 Apr 11 01:09 /etc/sysconfig/init
-rwxr-xr-x. 1 root root 5419 Jan  2  2018 /etc/sysconfig/network-scripts/init.ipv6-global
-rw-r--r--. 1 root root 30 Apr 11 14:12 /etc/selinux/targeted/contexts/initrc_context
```
查找/tmp/下，的yum.log文件，找到后直接删除。
```bash
[root@localhost tmp]# find /tmp/ -name yum.log -exec rm {} \;
[root@localhost tmp]#
```
查找根下，找关于lyshark用户的所有文件，找到后直接删除。
```bash
[root@localhost ~]# find / -user lyshark -exec rm -r {} \;

find: ‘/proc/1465/task/1465/fd/6’: No such file or directory
find: ‘/proc/1465/task/1465/fdinfo/6’: No such file or directory
find: ‘/proc/1465/fd/5’: No such file or directory
find: ‘/proc/1465/fdinfo/5’: No such file or directory
find: ‘/root/Catalog’: No such file or directory
find: ‘/home/lyshark’: No such file or directory
#rm -r 连带目录一起删除.报错原因：-exec 不适合大量传输,速率慢导致.
```
在根下，查找lyshark用户的文件，找到后删除，删除前会提示是否删除。
```bash
[root@localhost ~]# find / -user lyshark -ok rm -r {} \;
find: ‘/proc/1777/task/1777/fd/6’: No such file or directory
find: ‘/proc/1777/task/1777/fdinfo/6’: No such file or directory

< rm ... /root/LyShark > ? y
# -ok的使用和-exec是一样的,区别是-ok,执行时会提示你是否进行下一步操作.
```
