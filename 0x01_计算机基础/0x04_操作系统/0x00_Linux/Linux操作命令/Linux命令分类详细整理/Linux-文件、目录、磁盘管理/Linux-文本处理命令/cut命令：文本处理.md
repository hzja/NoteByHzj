<a name="M95AI"></a>
## 简介
**cut** 命令在Linux和Unix中的作用是从文件中的每一行中截取出一些部分，并输出到标准输出中。可以使用 **cut** 命令从一行字符串中于以字节，字符，字段（分隔符）等单位截取一部分内容出来。<br />Linux cut命令用于显示每行从开头算起 num1 到 num2 的文字。<br />cut的工作就是“剪”，具体的说就是在文件中负责剪切数据用的，cut命令从文件的每一行按照指定的分隔符进行分割，每行会被分成多个列，然后输出。
<a name="p5WCL"></a>
## 基本语法
```bash
cut [选项] filename
$ cut OPTION... [FILE]...
cut [-bn] [file]
cut [-c] [file]
cut [-df] [file]
```
<a name="pBFc6"></a>
## 选项
| **选项** | **功能** |
| --- | --- |
| -f | 列号，提取第几列 |
| -d | 分隔符，按照指定分隔符分割行，默认是制表符“\\t” |
| -c | 以字符为单位进行分割 |

`-f`：提取指定的字段，与-d一起使用，**cut** 命令使用 **Tab** 作为默认的分隔符。<br />`-d`：**Tab** 是默认的分隔符，使用这一选项可以指定自己的分隔符。<br />`-b`：以字节为单位进行分割。这些字节位置将忽略多字节字符边界，除非也指定了 -n 标志。提取指定的字节，也可以指定一个范围。<br />`-c`：以字符为单位进行分割，提取指定的字符，可以是以逗号分隔的数字的列表，也可以是以连字符分隔的数字的范围。<br />`-n` ：取消分割多字节字符。仅和 -b 标志一起使用。如果字符的最后一个字节落在由 `-b` 标志的 List 参数指示的<br />范围之内，该字符将被写出；否则，该字符将被排除<br />`–complement`：补充选中的部分，即反选。<br />`–output-delimiter`：修改输出时使用的分隔符。<br />`--only-delimited`：不输出不包含分隔符的列。
<a name="SCODG"></a>
## `-f` 选项
| **语法** | **说明** |
| --- | --- |
| -f N | 从第1 个开始数的第N 个字节、字符或域 |
| -f N- | 从第N 个开始到所在行结束的所有字符、字节或域 |
| -f N-M | 从第N 个开始到第M 个之间(包括第M 个)的所有字符、字节或域 |
| f -M | 从第1 个开始到第M 个之间(包括第M 个)的所有字符、字节或域 |
| f N,M,… | 取第N、第M个字节、字符或域 |

<a name="cBWyz"></a>
## 案例
<a name="nJO2B"></a>
### （1）准备数据，创建cut.txt，内容如下
分隔符是;，相当于每行3列数据
```
java;100;90
linux;120;110
php;90;80
c;150;140
c++;200;190
```
<a name="DVHZs"></a>
### （2）按;分隔符切割cut.txt，输出第1列
```bash
[root@test001 shells]# cut -d ";" -f 1 cut.txt 
java
linux
php
c
c++
```
<a name="Kj9sW"></a>
### （3）使用`-f`选项取多个列
```bash
[root@test001 shells]# cut -d ";" -f -2 cut.txt 
java;100
linux;120
php;90
c;150
c++;200
[root@test001 shells]# cut -d ";" -f 1,3 cut.txt 
java;90
linux;110
php;80
c;140
c++;190
[root@test001 shells]# cut -d ";" -f 2- cut.txt 
100;90
120;110
90;80
150;140
200;190
```
<a name="uJ2Ut"></a>
##  实例
<a name="LHGk9"></a>
### 截取文件的第2列和第4列
```bash
cut -f2,4 filename
```
<a name="N8T0J"></a>
### 去文件除第3列的所有列
```bash
cut -f3 --complement filename
```
-d 指定定界符：
```bash
cat -f2 -d";" filename
```
cut 取的范围<br />N- 第N个字段到结尾<br />-M 第1个字段为M<br />N-M N到M个字段<br />cut 取的单位<br />-b 以字节为单位<br />-c 以字符为单位<br />-f 以字段为单位（使用定界符）<br />eg:
```bash
cut -c1-5 file //打印第一到5个字符
cut -c-2 file  //打印前2个字符
```
以如下的名为 `context.txt` 的文本文件和 `/etc/passwd` 文件来为例来进行说明。
```bash
$ cat content.txt 
Ubuntu Linux
Microsoft Windows
OsX El Capitan
Unix
FreeBSD
```
<a name="af7c1b81"></a>
### 指定分隔符
最常用的选项是 `-d` 和 `-f` 的组合，这会根据 `-d` 指定的分隔符和 `-f` 列出的字段来提取内容。<br />例如在这个例子中只打印出 /etc/passwd 文件每一行的第一个字段，用的分隔符是 `:`
```bash
$ cut -d':' -f1 /etc/passwd
root
bin
daemon
adm
lp
sync
shutdown
halt
mail
operator
games
alvin
liangxu
...
```
在下面这个例子中用空格作为分隔符打印 content.txt 文件的第一个字段
```bash
$ cut -d " " -f 1 content.txt 
Ubuntu
Microsoft
OsX
Unix
FreeBSD
```
在下面这个例子中提取了多个字段。这里使用冒号（:）分隔符从文件 /etc/passwd 中包含字符串 /bin/bash 的行提取第一和第六个字段。
```bash
$ grep "/bin/bash" /etc/passwd | cut -d':' -f1,6
root:/root
alvin:/home/alvin
```
要显示字段的某个范围，可以指定开始和结束的字段，中间用连字符（-）连接，如下所示：
```bash
$ grep "/bin/bash" /etc/passwd | cut -d':' -f1-4,6,7
root:x:0:0:/root:/bin/bash
alvin:x:1000:1000:/home/alvin:/bin/bash
```
<a name="f471be25"></a>
### 补全选择的输出
要补全选择输出的字段（即反选），使用 `--complement` 选项。这一选项输出所有的字段，除了指定的字段。<br />在下面这个例子中输出 /etc/passwd 文件中包含 /bin/bash 的行中除了第二个字段以外的所有字段：
```bash
$ grep "/bin/bash" /etc/passwd | cut -d':' --complement -f2
root:0:0:root:/root:/bin/bash
```
<a name="v7fSL"></a>
### 指定输出的分隔符
使用 `--output-delimiter` 可以指定输出的分隔符。输入的分隔符由 `-d` 来指定，而输出分隔符和输入分隔符默认是一样的。<br />以下面的例子来测试不指定输出分隔符时的输出；
```bash
$  cut -d: -f1,7  /etc/passwd |  sort |  uniq -u
_apt:/usr/sbin/nologin
backup:/usr/sbin/nologin
bin:/usr/sbin/nologin
daemon:/usr/sbin/nologin
dnsmasq:/usr/sbin/nologin
games:/usr/sbin/nologin
gnats:/usr/sbin/nologin
irc:/usr/sbin/nologin
landscape:/usr/sbin/nologin
list:/usr/sbin/nologin
lp:/usr/sbin/nologin
lxd:/bin/false
```
现在加上`--output-delimiter`选项，将输出分隔符指定为空格：
```bash
$  cut -d: -f1,7 --output-delimiter ' ' /etc/passwd |  sort |  uniq -u
_apt /usr/sbin/nologin
backup /usr/sbin/nologin
bin /usr/sbin/nologin
daemon /usr/sbin/nologin
dnsmasq /usr/sbin/nologin
games /usr/sbin/nologin
gnats /usr/sbin/nologin
irc /usr/sbin/nologin
landscape /usr/sbin/nologin
list /usr/sbin/nologin
lp /usr/sbin/nologin
lxd /bin/false
```
用分隔符让每一行打印一个字段。<br />将 `--output-delimiter` 指定为 `$'\n'` 表换行。<br />输出结果为：
```bash
$ grep root /etc/passwd | cut -d':' -f1,6,7 --output-delimiter=$'\n'
root
/root
/bin/bash
operator
/root
/sbin/nologin
```
<a name="e4cbeeb3"></a>
### 以字符的方式提取内容
`-c`选项可以用来根据字符位置进行提取，注意空格和`Tab`也以字符来处理。<br />以字符定位
```bash
who|cut -c 3
t
```
打印 context.txt 文件每一行的第一个字符，如下：
```bash
$ cut -c 1 content.txt
U
M
O
U
F
```
下面显示了 context.txt 文件每一行的第一至七个字符；
```bash
$ cut -c 1-7 content.txt
Ubuntu
Microso
OsX El
Unix
FreeBSD
```
下面提取第二个到最后一个字符：
```bash
$ cut -c2- content.txt
buntu Linux
icrosoft Windows
sX El Capitannix
reeBSD
```
提取第一到第四个字符：
```bash
cut -c-4 content.txt
Ubun
Micr
OsX
Unix
Free
```
<a name="7b89a087"></a>
### 根据字节提取
使用`-b`选项通过指定字节的位置来选择一行的某一部分，使用逗号分隔每个指定位置，或用连字符 `-` 指定一个范围。<br />下面这个例子提取 content.txt 文件每一行的第一，二，三个字节：
```bash
$ cut -b 1,2,3 content.txt 
Ubu
Mic
OsX
Uni
Fre
```
用如下命令列出一个范围；
```bash
$ cut -b 1-3,5-7 content.txt 
Ubutu 
Micoso
OsXEl 
Uni
FreBSD
```
以字节定位
```bash
who|cut -b 3
o
```
<a name="jiZWc"></a>
### 实用案例
**cut** 是一个实用的命令，常常和其他Linux或Unix命令结合使用 。<br />例如如果想提取 ps 命令中的 USER，PID和COMMAND：
```bash
ps -L u n | tr -s " " | cut -d " " -f 2,3,14-
USER PID COMMAND0 
676 /sbin/agetty -o -p -- \u --keep-baud 115200,38400,9600 ttyS0 vt220
0 681 /sbin/agetty -o -p -- \u --noclear tty1 linux
0 23174 -bash
0 26737 ps -L u n
0 26738 tr -s
0 26739 cut -d -f 2,3,14-
```
提取内存的 total，used和free值，并保存到一个文件中。
```bash
$ free -m | tr -s ' ' | sed '/^Mem/!d' | cut -d" " -f2-4 >> memory.txt
$ cat memory.txt
985 86 234
```
**cut** 命令可以和很多其他Linux或Unix命令通过管道连接。可以通过管道传递一个或多个过滤器进行额外的文本处理。<br />**cut** 命令的局限性之一是它不支持指定多个字符作为分隔符。多个空格会被计算为多个字段分隔符，因此必须在 **cut** 命令前使用 **tr** 命令才能获得需要的输出。
