Linux pgrep pstree lsof 
<a name="Ol2Jd"></a>
# pgrep-列举进程ID
grep命令是进程相关的。不过，这个命令主要是用来列举进程ID的。下面列举root用户相关的进行ID
```bash
➜  ~ pgrep -u root
1
2
3
5
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594651803706-0f7143ba-d49a-4993-9f7b-bfe20ea2ffcf.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=1959915&status=done&style=none&width=1107.6666666666667)<br />这个命令相当于
```bash
ps -ef | egrep '^hchen' | awk '{print $2}'
```
<a name="pwJ3R"></a>
# pstree**-**以树形的方式列出进程
以树形的方式列出进程
```bash
➜  ~ pstree
systemd─┬─2*[agetty]
        ├─atd
        ├─auditd───{auditd}
        ├─crond
        ├─dbus-daemon
        ├─dhclient
        ├─docker-containe─┬─sh─┬─tail
        │                 │    └─tnslsnr───2*[{tnslsnr}]
        │                 └─8*[{docker-containe}]
        ├─dockerd-current─┬─docker-containe───8*[{docker-containe}]
        │                 ├─3*[docker-proxy───4*[{docker-proxy}]]
        │                 └─10*[{dockerd-current}]
        ├─lvmetad
        ├─ntpd
        ├─polkitd───5*[{polkitd}]
        ├─reportor───2*[{reportor}]
        ├─rsyslogd───2*[{rsyslogd}]
        ├─sshd───sshd───zsh───pstree
        ├─systemd-journal
        ├─systemd-logind
        ├─systemd-udevd
        └─tuned───4*[{tuned}]
```
<a name="FRmRO"></a>
# lsof-列出打开了的文件
列出打开了的文件
```bash
➜  ~ lsof | grep TCP
httpd       548    apache    4u     IPv6   14300967    TCP *:http (LISTEN)
httpd       548    apache    6u     IPv6   14300972    TCP *:https (LISTEN)
httpd       561    apache    4u     IPv6   14300967    TCP *:http (LISTEN)
httpd       561    apache    6u     IPv6   14300972    TCP *:https (LISTEN)
sshd       1764      root    3u     IPv6       4993    TCP *:ssh (LISTEN)
tcpserver  8965      root    3u     IPv4  153795500    TCP *:pop3 (LISTEN)
mysqld    10202     mysql   10u     IPv4   73819697    TCP *:mysql (LISTEN)
sshd      10735      root    3u     IPv6  160731956    TCP 210.51.0.232:ssh->123.117.239.68:31810 (ESTABLISHED)
sshd      10767     hchen    3u     IPv6  160731956    TCP 210.51.0.232:ssh->123.117.239.68:31810 (ESTABLISHED)
vsftpd    11095      root    3u     IPv4  152157957    TCP *:ftp (LISTEN)
```
<a name="PPSGR"></a>
# xmlwf-检查一个XML文档是否所有的tag都是正常
这个命令可以检查一下一个XML文档是否是所有的tag都是正常的。
```bash
➜  ~ curl 'https://coolshell.cn/?feed=rss2' > cocre.xml
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100 64882    0 64882    0     0  86455      0 --:--:-- --:--:-- --:--:-- 2073k
➜  ~ xmlwf cocre.xml
➜  ~ perl -i -pe 's@<link>@<br>@g' cocre.xml
➜  ~ xmlwf cocre.xml
cocre.xml:13:23: mismatched tag
```
<a name="XXF2p"></a>
# col-把man文件转成纯文本文件
这个命令可以把man文件转成纯文本文件
```bash
➜  ~ PAGER=cat
➜  ~ man less | col -b > less.txt
```
<a name="IlHMb"></a>
# ldd-查看可执行文件的动态链接库
这个命令可以知道一个可执行文件所使用了动态链接库
```bash
➜  ~ ldd /usr/bin/java
        linux-gate.so.1 =>  (0x00cd9000)
        libgij.so.7rh => /usr/lib/libgij.so.7rh (0x00ed3000)
        libgcj.so.7rh => /usr/lib/libgcj.so.7rh (0x00ed6000)
        libpthread.so.0 => /lib/i686/nosegneg/libpthread.so.0 (0x00110000)
        librt.so.1 => /lib/i686/nosegneg/librt.so.1 (0x009c8000)
        libdl.so.2 => /lib/libdl.so.2 (0x008b5000)
        libz.so.1 => /usr/lib/libz.so.1 (0x00bee000)
        libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00aa7000)
        libc.so.6 => /lib/i686/nosegneg/libc.so.6 (0x0022f000)
        libm.so.6 => /lib/i686/nosegneg/libm.so.6 (0x00127000)
        /lib/ld-linux.so.2 (0x00214000)
```
<a name="EK6xd"></a>
# mkfifo-有名称的管道创建
这个是一个创建有名称管道的系统调用或命令。平时，命令行上使用竖线“|”把命令串起来是使用无命管道。而用mkfifo则使用的是有名管道
```bash
➜  ~ mkfifo /tmp/hchenpipe

➜  ~ ls -l /tmp
prw-rw-r-- 1 hchen  hchen  0 05-10 18:58 hchenpipe
```
在一个shell中运行如下命令，这个命令不会返回，除非有人从这个有名管道中把信息读走。
```bash
➜  ~ ls -al > /tmp/hchenpipe
```
在另一个命令窗口中读取这个管道中的信息：（其会导致上一个命令返回）
```bash
➜  ~ head /tmp/hchenpipe
drwx------ 8 hchen hchen    4096 05-10 18:27 .
drwxr-xr-x 7 root  root     4096 03-05 00:06 ..
drwxr-xr-x 3 hchen hchen    4096 03-01 18:13 backup
-rw------- 1 hchen hchen     721 05-05 22:12 .bash_history
-rw-r--r-- 1 hchen hchen      24 02-28 22:20 .bash_logout
-rw-r--r-- 1 hchen hchen     176 02-28 22:20 .bash_profile
-rw-r--r-- 1 hchen hchen     124 02-28 22:20 .bashrc
-rw-r--r-- 1 root  root    14002 03-07 00:29 index.htm
-rw-r--r-- 1 hchen hchen   31465 03-01 23:48 index.php
```
<a name="DqBqd"></a>
# nl-带行号的cat命令
nl命令其它和cat命令很像，只不过它会打上行号
```bash
➜  ~ nl stdio.h | head -n 10
     1  /* Define ISO C stdio on top of C++ iostreams.
     2     Copyright (C) 1991,1994-2004,2005,2006 Free Software Foundation, Inc.
     3     This file is part of the GNU C Library.

     4     The GNU C Library is free software; you can redistribute it and/or
     5     modify it under the terms of the GNU Lesser General Public
     6     License as published by the Free Software Foundation; either
     7     version 2.1 of the License, or (at your option) any later version.

     8     The GNU C Library is distributed in the hope that it will be useful,
```
<a name="6F1ya"></a>
# split-文件分割
如果有一个很大的文件，就可以通过split命令把其分割成一些小的文件
```bash
➜  ~ ls -l largefile.tar.gz
-rw-r--r-- 1 hchen hchen 436774774 04-17 02:00 largefile.tar.gz

➜  ~ split -b 50m largefile.tar.gz LF_

➜  ~ ls -l LF_*
-rw-r--r-- 1 hchen hchen 52428800 05-10 18:34 LF_aa
-rw-r--r-- 1 hchen hchen 52428800 05-10 18:34 LF_ab
-rw-r--r-- 1 hchen hchen 52428800 05-10 18:34 LF_ac
-rw-r--r-- 1 hchen hchen 52428800 05-10 18:34 LF_ad
-rw-r--r-- 1 hchen hchen 52428800 05-10 18:34 LF_ae
-rw-r--r-- 1 hchen hchen 52428800 05-10 18:35 LF_af
-rw-r--r-- 1 hchen hchen 52428800 05-10 18:35 LF_ag
-rw-r--r-- 1 hchen hchen 52428800 05-10 18:35 LF_ah
-rw-r--r-- 1 hchen hchen 17344374 05-10 18:35 LF_ai
```
文件合并只需要使用简单的合并
```bash
➜  ~ cat LF_* >largefile.tar.gz
```
<a name="uQ7QQ"></a>
# bc-高精度运算
这个命令主要是做一个精度比较高的数学运算的。比如开平方根等。下面利用bc命令写的一个脚本（文件名：sqrt）
```bash
#!/bin/bash

if [ $
then
    echo 'Usage: sqrt number'
    exit 1
else
    echo -e "sqrt($1)\nquit\n" | bc -q -i
fi
```
可以使用这个脚本进行平方根运算
```bash
➜  ~ ./sqrt 36
6
➜  ~ ./sqrt 2.0000
1.4142
➜  ~ ./sqrt 10.0000
3.1622
```
<a name="b77nr"></a>
# wall–输出信息到当前连接的所有访问端
全称为“write all”。wall命令用于向系统当前所有打开的终端上输出信息。通过wall命令可将信息发送给每位同意接收公众信息的终端机用户，若不给予其信息内容，则wall命令会从标准输入设备读取数据，然后再把所得到的数据传送给所有终端机用户。<br />语法格式：wall [参数]<br />常用参数：

| messge | 消息：指定广播消息 |
| --- | --- |

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597067092169-64d8f947-6a8e-455f-a9a8-389283113af2.png#align=left&display=inline&height=278&originHeight=833&originWidth=3323&size=957034&status=done&style=none&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597067116274-66c550a2-59df-4f21-8b78-01ff0d6b6026.png#align=left&display=inline&height=234&originHeight=702&originWidth=3323&size=787820&status=done&style=none&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597067171296-ad74783e-1e42-4ddf-91c8-8172530232aa.png#align=left&display=inline&height=237&originHeight=711&originWidth=3323&size=802646&status=done&style=none&width=1107.6666666666667)
<a name="wsRal"></a>
# dmesg–显示开机信息
dmesg命令全称为“diagnostic message”。被用于检查和控制内核的环形缓冲区。kernel会将开机信息存储在ring buffer中。若是开机时来不及查看信息，可利用dmesg来查看。开机信息保存在/var/log/dmesg文件里。<br />语法格式：dmesg [参数]<br />常用参数：

| -c | 显示信息后，清除ring buffer中的内容 |
| --- | --- |
| -s <缓冲区大小> | 预设置为8196，刚好等于ring buffer的大小 |
| -n | 设置记录信息的层级 |

<a name="4330fa71"></a>
# uname–显示系统信息
uname命令的英文全称即“Unix name”。<br />用于显示系统相关信息，比如主机名、内核版本号、硬件架构等。<br />如果未指定任何选项，其效果相当于执行”uname -s”命令，即显示系统内核的名字。<br />语法格式：uname [参数]<br />常用参数：

| -a | 显示系统所有相关信息 |
| --- | --- |
| -m | 显示计算机硬件架构 |
| -n | 显示主机名称 |
| -r | 显示内核发行版本号 |

<a name="bIOKy"></a>
# umount–卸载文件系统
umount是“unmount”的缩写，译为“不挂在。所以它的”的作用是卸载已安装的文件系统、目录或文件。<br />使用umount命令可以卸载文件系统。利用设备名或挂载点都能umount文件系统，不过最好还是通过挂载点卸载，一面使用绑定挂在（一个设备，多个挂载点）时产生混乱。<br />语法格式：umount [参数]<br />常用参数：

| -a | 卸载/etc/mtab中记录的所有文件系统 |
| --- | --- |
| -h | 显示帮助 |
| -n | 卸载时不要将信息存入/etc/mtab文件中 |
| -r | 若无法成功卸载，则尝试以只读的方式重新挂入文件系统 |

<a name="4DMDa"></a>
# umask–设定权限掩码
全称是“User's MASK”。umask命令可用来设定权限掩码。权限掩码是由3个八进制的数字所组成，将现有的存取权限减掉权限掩码后，即可产生建立文件时预设的权限。<br />语法格式：umask [参数] [权限掩码]<br />常用参数：

| -S | 以文字的方式来表示权限掩码 |
| --- | --- |
| -p | 输出的权限掩码可直接作为指令来执行 |

<a name="8UCQY"></a>
# ulimit–控制shell程序的资源
其全称是“User's LIMIT”。Linux对于每个用户，系统限制其最大进程数。为提高性能，可以根据设备资源情况，设置各linux用户的最大进程数，可以用ulimit来显示当前的各种用户进程限制。<br />ulimit为shell内建命令，可用来控制shell执行程序的资源。<br />语法格式：ulimit [参数]<br />常用参数：

| -a | 显示目前资源限制的设定 |
| --- | --- |
| -d | 程序数据节区的最大值，单位为KB |
| -t | 指定CPU使用时间的上限，单位为秒 |
| -u | 用户最多可开启的程序数目 |
| -v | 指定可使用的虚拟内存上限,单位为KB |

<a name="yIVCc"></a>
# tty–打印输出连接到标准输入的终端文件名
tty是 Teletype（电传打字机）的缩写，电传打字机是最早出现的一种终端设备，现在通常使用 TTY 来统称各种类型的终端设备，如键盘 、打印机 、显示器等。<br />pty（虚拟终端): 如果远程telnet到主机或使用xterm时,也需要一个终端交互，这就是虚拟终端pty(pseudo-tty) 。<br />pts/ptmx(pts/ptmx结合使用，进而实现pty): pts(pseudo-terminalslave)是pty的实现方法，与ptmx(pseudo-terminalmaster)配合使用实现pty。<br />语法格式：tty [参数]<br />常用参数：

| -s/--silent/--quiet | 无打印输出结果，只是返回退出状态码 |
| --- | --- |
| --help | 打印输出tty命令的帮助信息并退出 |
| --version | 打印输出当前tty命令版本信息 |

<a name="5pKUx"></a>
# tee–读取标准输入的数据
tee指令会从标准输入设备读取数据，将其内容输出到标准输出设备，同时保存成文件 。<br />语法格式：tee [参数] [文件]<br />常用参数：

| -a | 附加到既有文件的后面，而非覆盖它 |
| --- | --- |
| -i | 忽略中断信号 |
| — help  | 查看帮助信息 |
| — version | 显示版本信息 |

<a name="bKSyy"></a>
# stty–修改终端命令行的相关设置
全称为“Set TTY”，stty命令用于检查和修改当前注册的终端的通信参数。UNIX系统为键盘的输入和终端的输出提供了重要的控制手段，可以通过stty命令对特定终端或通信线路设置选项。stty还可用于控制一些具有特殊控制功能的键。比如系统可能已经使用CTRL-D作为文件结束符，但你可能喜欢使用CTRL-C。这时可以使用stty命令改变这些默认设置。<br />语法格式：stty [参数]<br />常用参数：

| -a | 以容易阅读的方式打印当前的所有配置 |
| --- | --- |
| -g | 以stty可读方式打印当前的所有配置 |

<a name="57YBP"></a>
# seq–打印数字序列
全称“SEQuence”，seq命令用于以指定增量从首数开始打印数字到尾数，即产生从某个数到另外一个数之间的所有整数，并且可以对整数的格式、宽度、分割符号进行控制。<br />语法格式：seq [参数]<br />常用参数：

| -f | 格式 |
| --- | --- |
| -s | 字符串 |
| -w | 在列前添加0 使得宽度相同 |

<a name="n1vr6"></a>
# rsh–远端登入的shell
rsh命令的英文全称为“remote shell”，该命令提供的用户环境，也就是shell，以便指令能够在指定的远端主机上执行。<br />语法格式：rsh [参数]<br />常用参数：

| -d | 使用Socket层级的排错功能 |
| --- | --- |
| -l | 指定要登入远端主机的用户名称 |
| -n | 把输入的指令号向代号为/dev/null的特殊外围设备 |

<a name="817d29c2"></a>
# rlogin–远端登入
rlogin命令用于远端登入。执行rlogin指令开启终端机阶段操作，并登入远端主机。<br />语法格式：rlogin [参数]<br />常用参数：

| -E | 忽略escape字符 |
| --- | --- |
| -8  | 只识别8位字的字符 |
| -L | 允许rlogin会话运行在litout模式 |
| -ec | 设置escape字符为c |

<a name="wsyTG"></a>
# insmod–载入模块
insmod命令用于将给定的模块加载到内核中。全称是“install module”。<br />Linux有许多功能是通过模块的方式，在需要时才载入kernel。如此可使kernel较为精简，进而提高效率，以及保有较大的弹性。这类可载入的模块，通常是设备驱动程序。<br />语法格式：insmod [参数]<br />常用参数：

| -f | 不检查目前kernel版本与模块编译时的kernel版本是否一致，强制将模块载入 |
| --- | --- |
| -k | 将模块设置为自动卸除 |
| -m | 输出模块的载入信息 |
| -o | 指定模块的名称，可使用模块文件的文件名 |

<a name="dlBzx"></a>
# lsmod–显示已加载模块状态
lsmod命令全称“List modules”，用于显示已经加载到内核中的模块的状态信息。<br />执行lsmod命令后会列出所有已载入系统的模块。linux操作系统的核心具有模块化的特性，因此在编译核心时，务须把全部的功能都放入核心。可以将这些功能编译成独立的模块，待需要时再分别载入。<br />lsmod命令支持内核版本为2.5.48以上的linux系统，比此版本老的内核使用命令lsmod.old<br />语法格式：lsmod
<a name="GaMzP"></a>
# rmmod–删除模块
rmmod即“remove module”的简称,rmmod命令用于删除模块。执行rmmod命令，可删除不需要的模块。Linux操作系统的核心具有模块化的特性，因此在编译核心时，不需要吧全部功能都放入核心，可以将这些功能编译成一个个单独的模块，待有需要时再分别载入它们。<br />语法格式：rmmod [参数] [模块名称]<br />常用参数：

| -a | 删除所有目前不需要的模块 |
| --- | --- |
| -s | 把信息输出至syslog常驻服务，而非终端机界面 |
| -v | 显示指令执行的详细信息 |
| -f | 强制移除模块，使用此选项比较危险 |
| -w | 等待着，知道模块能够被除时再移除模块 |

<a name="sUWcW"></a>
# pushd–添加目录到目录堆栈顶部
全称为“PUSH Directory”。pushd命令用于更改新目录并（或）压栈，或者把栈里的某个目录推到栈顶。<br />语法格式: pushd[参数]<br />常用参数:

| +N | 把正数第N个放到栈顶 |
| --- | --- |
| -N | 把倒数第N个放到栈顶 |
| -n | 不切换，只压栈 |
| dir | 要推送的目录 |

<a name="rMgj2"></a>
# popd–从shell目录堆栈中删除记录
全称“POP Directory”用于从Shell目录堆栈中删除记录，如果不加任何选项，popd命令先删除Shell目录堆栈栈顶的记录，再把当前目录切换到新的Shell目录堆栈顶的目录。<br />语法格式：popd [参数]<br />常用参数：

| -n | 将右起第n个目录删除（n从0开始计数） |
| --- | --- |
| +n | 将左起第n个目录删除（n从0开始计数） |

<a name="dWF6A"></a>
# pico–编辑文字文件
全称为“PIne's message COmposition editor”。pico是个简单易用、以显示导向为主的文字编辑程序，它伴随着处理电子邮件和新闻组的程序pine而来。<br />语法格式：pico [参数] [文件]<br />常用参数：

| -k | 预设pico在使用剪下命令时，会把光标所在的列的内容全部删除 |
| --- | --- |
| -m | 开启鼠标支持的功能，您可用鼠标点选命令列表 |
| -v | 启动阅读模式，用户只能观看，无法编辑文件的内容 |
| -w | 关闭自动换行，通过这个参数可以编辑内容很长的列 |
| -z | 让pico可被Ctrl+z中断，暂存在后台作业里 |

<a name="Cntjd"></a>
# perl–实用报表提取语言
全称"Pratical Extraction and Report Language"或"Pathologically Eclectic Rubbish Lister"。使用perl命令可以扫描任意文本文件，从这些文本文件中提取信息优化的语言，根据这些信息打印报告。<br />语法格式：perl [参数] [程序文件]<br />常用参数：

| -c | 只进行语法检查 |
| --- | --- |
| -e | 可以让Perl程序在命令行上运行 |
| -l | 启用行结束处理，指定行结束 |

<a name="YzTIU"></a>
# od–输出文件内容
全称为“Octal Dump”。od命令会读取所给予的文件的内容，并将其内容以八进制字码呈现出来。<br />语法格式：od [参数]<br />常用参数：

| -a  | 此参数的效果和同时指定”-ta”参数相同 |
| --- | --- |
| -A | 选择要以何种基数计算字码 |
| -b | 此参数的效果和同时指定”-toC”参数相同 |
| -c  | 此参数的效果和同时指定”-tC”参数相同 |

<a name="26ec384e"></a>
# nm–显示二进制目标文件的符号表
nm是names的缩写， nm命令主要是用来列出某些文件中的符号（说白了就是一些函数和全局变量等）。一般来说， 搞linux开发的人， 才会用到nm命令， 非开发的人， 应该用不到。<br />语法格式: nm [参数]<br />常用选项：

| -A | 每个符号前显示文件名 |
| --- | --- |
| -D | 显示动态符号 |
| -g | 仅显示外部符号 |
| -r | 反序显示符号表 |

<a name="WqTUY"></a>
# nano– 字符终端文本编辑器
nano全称“Nano's ANOther editor”，是一个字符终端的文本编辑器，有点像DOS下的editor程序。它比vi/vim要简单得多。某些Linux发行版的默认编辑器就是nano。<br />nano命令可以打开指定文件进行编辑，默认情况下它会自动断行，即在一行中输入过长的内容时自动拆分成几行，但用这种方式来处理某些文件可能会带来问题，比如Linux系统的配置文件，自动断行就会使本来只能写在一行上的内容折断成多行了，有可能造成系统不灵了。因此，如果想避免这种情况出现，需要加上-w选项吧。<br />语法格式：nano [参数] [文件]<br />常用参数：

| -A | 启用智能HOME键 |
| --- | --- |
| -B | 储存既有文件的备份 |
| -D | 用粗体替代颜色反转 |

<a name="dbabb808"></a>
# nl-添加行号
nl命令是一个很好用的编号过滤工具。全称是“Number of Lines”。该命令可以读取 File 参数（缺省情况下标准输入），计算输入中的行号，将计算过的行号写入标准输出。<br />语法格式：nl [参数] [文件]<br />常用参数：

| -b | 指定行号指定的方式 |
| --- | --- |
| -n | 列出行号表示的方式 |
| -w | 行号栏位的占用的位数 |
| -p | 在逻辑定界符处不重新开始计算。 |

<a name="7td2f"></a>
# mkfs–在特定分区上建立文件系统
全称为“MaKe FileSystem”在磁盘分区上创建ext2、ext3、ext4、ms-dos、vfat文件系统，默认情况下会创建ext2。mkfs用于在设备上构建Linux文件系统，通常是硬盘分区。文件要么是设备名称(例如/dev/hda1，/dev/sdb2)，要么是包含文件系统的常规文件。成功返回0，失败返回1。<br />实际上，mkfs只是Linux下可用的各种文件系统构建器(mkfs.fstype)的前端，在可能/sbin、/sbin/fs、/sbin/fs.d、/etc/fs、/etc/fs等多个目录中搜索特定于文件系统的生成器(编译时定义了精确的列表，但至少包含/sbin和/sbin/fs)，最后在PATH环境变量中列出的目录中搜索。<br />语法格式：mkfs [参数]<br />常用参数：

| device | 预备检查的硬盘分区，例如：/dev/sda1 |
| --- | --- |
| -V | 详细显示模式 |
| -t | 给定档案系统的型式，Linux 的预设值为 ext2 |
| -c | 在制做档案系统前，检查该partition 是否有坏轨 |

<a name="DnQYW"></a>
# mc–菜单式文件管理程序
mc命令全称“Midnight Commander”用于提供一个菜单式的文件管理程序，执行mc之后，将会看到菜单式的文件管理程序，共分成４个部分。<br />语法格式：mc [参数] [文件]<br />常用参数：

| -a 　 | 当mc程序画线时不用绘图字符画线 |
| --- | --- |
| -b 　 | 使用单色模式显示 |
| -c 　 | 使用彩色模式显示 |
| -d 　 | 不使用鼠标 |
| -f 　 | 显示mc函数库所在的目录 |

<a name="X0Mvd"></a>
# lpr–将文件放入打印队列等待打印
lpr命令全称“Line PRint”用于将文件发送给指定打印机进行打印，如果不指定目标打印机，则使用默认打印机。<br />语法格式：lpr [参数]<br />常用参数：

| -E | 与打印服务器连接时强制使用加密 |
| --- | --- |
| -H | 指定可选的打印服务器 |
| -C | 指定打印任务的名称 |
| -P | 指定接受打印任务的目标打印机 |
| -U | 指定可选的用户名 |

<a name="u77sl"></a>
# ldd–打印程序依赖的共享库
全称为“List dynamic dependencies”，ldd命令用于打印程序或者库文件所依赖的共享库列表。<br />语法格式: ldd [参数] [文件]<br />常用参数：

| -v | 详细信息模式，打印所有相关信息 |
| --- | --- |
| -u | 打印未使用的直接依赖 |
| -d | 执行重定位和报告任何丢失的对象 |
| -r | 执行数据对象和函数的重定位，并且报告任何丢失的对象和函数 |
| -- -help | 显示帮助信息 |

<a name="cd285b94"></a>
# lilo–安装核心载入
lilo命令用于安装核心载入，开机管理程序， 全称“LInux Loader”。lilo是个Linux系统核心载入程序，同时具备管理开机的功能。单独执行lilo指令，它会读取/etc/lilo.conf配置文件，然后根据其内容安装lilo。虽然 LILO 仍在不断地发展，但 LILO 工作原理的基本概念保持不变。<br />语法格式: lilo [参数]<br />常用参数：

| -b | 指定安装lilo之处的外围设备代号 |
| --- | --- |
| -c | 使用紧致映射模式 |
| -C | 指定lilo的配置文件 |
| -d | 设置开机延迟时间 |
| -D | 指定开机后预设启动的操作系统，或系统核心识别标签 |

<a name="YVCuq"></a>
# fsck–检查并修复Linux文件系统
fsck命令的英文全称是“filesystem check”，即检查文件系统的意思，常用于检查并修复Linux文件系统的一些错误信息，操作文件系统需要先备份重要数据，以防丢失。<br />Linux fsck命令用于检查并修复Linux文件系统，可以同时检查一个或多个 Linux 文件系统；若系统掉电或磁盘发生问题，可利用fsck命令对文件系统进行检查。<br />语法格式：fsck [参数] [文件系统]<br />常用参数：

| -a | 自动修复文件系统，不询问任何问题 |
| --- | --- |
| -A | 依照/etc/fstab配置文件的内容，检查文件内所列的全部文件系统 |
| -N | 不执行指令，仅列出实际执行会进行的动作 |
| -P | 当搭配”-A”参数使用时，则会同时检查所有的文件系统 |

<a name="fB6Ij"></a>
# grub–多重引导程序grub的命令行shell工具
grub命令全称是“GRand Unified Bootloader”是多重引导程序grub的命令行shell工具。<br />语法格式：grub [参数]<br />常用参数：

| -- -batch | 打开批处理模式 |
| --- | --- |
| -- -boot-drive=<驱动器> | 指定stage2的引导驱动器 |
| -- -config-file<配置文件> | 指定stage2的配置文件 |
| -- -device-map=<文件> | 指定设备的映射文件 |

<a name="3sVRX"></a>
# exec–调用并执行指定的命令
exec命令用于调用并执行指定的命令。全称为“EXECute”。exec命令通常用在shell脚本程序中，可以调用其他的命令。如果在当前终端中使用命令，则当指定的命令执行完毕后会立即退出终端。<br />语法格式：exec [参数]<br />常用参数：

| -c | 在空环境中执行指定的命令 |
| --- | --- |

<a name="dLGWu"></a>
# elm–纯文本邮件客户端程序
elm命令是ELectronic Mail单词的缩写，是一个E-mail客户端管理程序，它提供了纯文本交互式全屏幕界面。elm命令中不但提供了方便的键盘功能，还有一个在屏幕下方的命令帮助。<br />elm命令在运行时使用小键盘的箭头来选择要处理的邮件，直接按回车键阅读邮件。<br />语法格式:  elm [参数]<br />常用参数：

| -s<邮件主题> | 指定新邮件的邮件主题 |
| --- | --- |
| -f<目录> | 开启程序时，读取指定的目录 |
| -h | 显示帮助 |
| -i<文件名> | 将文件内容插入送出的邮件中 |

