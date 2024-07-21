Linux which whereis locate grep find 
<a name="wkHtM"></a>
## 概览
```
which：查看执行文件的位置。
whereis：查看可执行文件位置和相关文件。
locate：配合数据库缓存，快速查看文件的位置。
grep：过滤匹配，他是一个文件搜索工具。
find：可以根据条件查看文件。
```
<a name="9k1SA"></a>
## which
**在 PATH 变量中定义的全部路径中查找可执行文件或脚本**。<br />`which` 命令有两个重要参数：

- `-all, -a` 默认情况下，`which` 命令会在匹配到第一个结果后结束运行，添加该参数可以让其搜索所有路径。
- `-read-alias, -i` 将输入视为别名搜索。Linux 系统中通常会使用 alias 设置诸多别名来简写命令，例如 Centos 中的 `ll` 实际是 `ls -l` ，而 `which` 是 `alias | /usr/bin/which --tty-only --read-alias --show-dot --show-tilde`。
```bash
# Centos
# 以绝对路径调用 which，这样就不会受到 Centos 默认的几个参数影响
# 返回结果说明找不到 ll 命令
$ /usr/bin/which ll
/usr/bin/which: no ll in (/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin)
# 直接输入 which 时实际效果为带有“默认参数”的
# 返回结果说明 ll 是 ls -l 的别名，
$ which ll
alias ll='ls -l --color=auto'
        /usr/bin/ls
```

- `which ll` 相当于 `alias | /usr/bin/which --tty-only --read-alias --show-dot --show-tilde ll`，返回结果第一行是 alias 输出的 `ll` 别名设置情况，第二行则是 `ls` 的实际位置。

`which` 的其他几个参数如下：

- `--tty-only`：尽在终端调用的情况下附带右侧添加的参数，其他情况下不接收右侧其他参数（此处的参数值 `--show-dot`、`--show-tilde` 此类，输入的待查询命令仍然会接收），通过这个命令可以保证 Shell 脚本中的 `which` 命令正确执行。
- `--show-dot`：输出以 "." 符号开头的目录。Linux 中 "`.`" 符号开头的目录是约定的隐藏文件夹，没有该参数时会忽略这些目录。
- `--show-tilde`：将用户家目录替换成 "~" 符号输出。Linux 中 "`~`" 符号是登录用户家目录的缩写，如果登录用户名为 cncsl，则 "~" 指 "/home/cncsl" 目录。当使用 root 账号登录时该参数无效。
```
--all, -a      显示所有的匹配路径
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594547032823-23910482-b71f-429e-9175-81824db237f1.png#averageHue=%233f3f3f&height=583&id=hbrWw&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2042525&status=done&style=none&title=&width=1107.6666666666667)<br />操作:
```bash
➜  / which pwd
/usr/bin/pwd
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594547081976-af600cad-3e61-42c4-9951-1c18988f7137.png#averageHue=%233b3b3b&height=61&id=jQL24&originHeight=184&originWidth=547&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42331&status=done&style=none&title=&width=182.33333333333334)
<a name="ojbFV"></a>
## whereis
**查找指定命令的可执行文件、源代码和手册的位置。**
```bash
$ whereis vim
vim: /usr/bin/vim /usr/share/vim /usr/share/man/man1/vim.1.gz
```
可以看出，`vim` 的可执行程序位于 `/usr/bin/vim`，手册位于 `/usr/share/vim` 和 `/share/man/man1/vim.1.gz` 目录。

- `-b`、`-m` 和 `-s` 分别用于指定仅查询可执行文件、手册和源代码。
- `-B`、`-M`和 `-S` 命令用于指定查询路径。
- `-u` 参数的描述直译为仅查询有异常情况的命令。所谓的异常情况是指，某个命令的相关类型文件不止恰好一份（一份都没有或多于一份）。例如：
   - `ls` 命令具有两份手册：
```bash
$ whereis -m -u ls
ls: /usr/share/man/man1/ls.1.gz /usr/share/man/man1p/ls.1p.gz
```

   - Linux 系统中有很多个与 python 相关的可执行文件：
```
$ whereis -b -u python
python: /usr/bin/python /usr/bin/python2.7 /usr/lib/python2.7 /usr/lib64/python
```

```
一般不加参数使用
-b         只搜索二进制文件
-B <目录>  定义二进制文件查找路径
-m         只搜索 man 手册
-M <目录>  定义 man 手册查找路径
-s         只搜索源代码
-S <目录>  定义源代码查找路径
```
操作:
```bash
➜  / whereis ls
ls: /usr/bin/ls /usr/share/man/man1/ls.1.gz
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594547142269-b2b4b79c-d06c-45f9-a777-e6f212b1b95e.png#averageHue=%23373737&height=61&id=EjpGv&originHeight=183&originWidth=1291&originalType=binary&ratio=1&rotation=0&showTitle=false&size=101167&status=done&style=none&title=&width=430.3333333333333)
<a name="3bOE3"></a>
## locate
**在文档和目录名称的数据库中查找指定文件。**<br />Linux 系统会定期自动扫描磁盘来维护一个记录磁盘数据的数据库，而 `locate` 命令使用的数据库是 _/var/lib/mlocate/mlocate.db_。
```bash
$ ls -hl /var/lib/mlocate/mlocate.db
-rw-r-----. 1 root slocate 2.7M Feb  4 03:42 /var/lib/mlocate/mlocate.db
```
可以看出当前 _mlocate.db_ 文件共记录了 2.7M 的数据。

- `--count, -c`：不输出具体的文件路径信息，仅输出查询到的数量。
- `--ignore-case, -i`：查询时忽略大小写
- `--limit, -l, -n LIMIT`：限定输出的文件数量为 LIMIT
- `--regexp,-r REGEXP`：使用 REGEXP 指定的正则表达式匹配。
```
# 统计有多少PNG格式的图像文件
$ locate -c png
# 统计有多少 readme 文件（根据编写者的习惯，readme 文件可能名为 README、ReadMe等）
$ locate -c -i readme
# 输出十个 .gz 归档文件的路径
$ locate -l 10 *.gz
# 查看 tomcat 2021年1月的日志
$ locate -r tomcat.2021-01-[0-3][0-9].log
```
由于 `locate` 命令是从数据库查找文件，新创建的文件可能由于未被记录到数据库中而无法查询到，这种时候需要使用 `updatedb` 命令手动更新数据库。<br />操作：locate和find命令功能差不多，但是搜索效率更高，因为locate查的是数据库而find查找的是目录文件。<br />数据库:
```bash
➜  ~ ls /var/lib/mlocate/mlocate.db
/var/lib/mlocate/mlocate.db
```
相关配置文件:
```bash
➜  ~ ls /etc/updatedb.conf
/etc/updatedb.conf
```
相关定时任务:
```bash
➜  ~ ls /etc/cron.daily/mlocate
/etc/cron.daily/mlocate
```
举例:
```bash
➜  ~ touch shafa
➜  ~ locate shafa
➜  ~ updatedb
➜  ~ locate shafa
/root/shafa
```
:::tips
注：如果当天新建的文件查找，需要手动updatedb。
:::
<a name="4hM9A"></a>
## grep
Linux grep 命令用于查找文件里符合条件的字符串。<br />grep 指令用于查找内容包含指定的范本样式的文件，如果发现某文件的内容符合所指定的范本样式，预设 grep 指令会把含有范本样式的那一列显示出来。若不指定任何文件名称，或是所给予的文件名为 -，则 grep 指令会从标准输入设备读取数据<br />命令：grep<br />语法：grep 参数  目标值  文件
```bash
grep [-abcEFGhHilLnqrsvVwxy][-A<显示行数>][-B<显示列数>][-C<显示列数>][-d<进行动作>][-e<范本样式>][-f<范本文件>][--help][范本样式][文件或目录...]
```
与 grep 相似的工具还有 `egrep`、`fgrep`，实用性并不强，其功能完全可以通过 grep 的扩展参数来实现。
<a name="tPDlQ"></a>
### 常用参数

- `-A`：除了匹配行，额外显示该行之**后**的N行
- `-B`：除了匹配行，额外显示该行之**前**的N行
- `-C`：除了匹配行，额外显示该行**前后**的N行
- `-c`：统计匹配的行数
- `-e`：**实现多个选项间的逻辑 or 关系**
- `-E`：**支持扩展的正则表达式**
- `-F`：相当于 fgrep
- `-i`：**忽略大小写**
- `-n`：显示匹配的行号
- `-o`：仅显示匹配到的字符串
- `-q`：安静模式，不输出任何信息，脚本中常用
- `-s`：不显示错误信息
- `-v`：**显示不被匹配到的行**
- `-w`：显示整个**单词**
- `--color`：以颜色突出显示匹配到的字符串
```
 -v       取反
 -i       忽略大小写
 -n       输出的同时打印行号
 ^*       以*开头
 *$       以*结尾
 ^$       空行
```
<a name="GdOBx"></a>
### 全部参数

- -a 或 --text：不要忽略二进制的数据。
- -A<显示行数> 或 --after-context=<显示行数>：除了显示符合范本样式的那一列之外，并显示该行之后的内容。
- -b 或 --byte-offset：在显示符合样式的那一行之前，标示出该行第一个字符的编号。
- -B<显示行数> 或 --before-context=<显示行数>：除了显示符合样式的那一行之外，并显示该行之前的内容。
- -c 或 --count：计算符合样式的列数。
- -C<显示行数> 或 --context=<显示行数>或-<显示行数>：除了显示符合样式的那一行之外，并显示该行之前后的内容。
- -d <动作> 或 --directories=<动作>：当指定要查找的是目录而非文件时，必须使用这项参数，否则grep指令将回报信息并停止动作。
- -e<范本样式> 或 --regexp=<范本样式>：指定字符串做为查找文件内容的样式。
- -E 或 --extended-regexp：将样式为延伸的正则表达式来使用。
- -f<规则文件> 或 --file=<规则文件>：指定规则文件，其内容含有一个或多个规则样式，让grep查找符合规则条件的文件内容，格式为每行一个规则样式。
- -F 或 --fixed-regexp：将样式视为固定字符串的列表。
- -G 或 --basic-regexp：将样式视为普通的表示法来使用。
- -h 或 --no-filename：在显示符合样式的那一行之前，不标示该行所属的文件名称。
- -H 或 --with-filename：在显示符合样式的那一行之前，表示该行所属的文件名称。
- -i 或 --ignore-case：忽略字符大小写的差别。
- -l 或 --file-with-matches：列出文件内容符合指定的样式的文件名称。
- -L 或 --files-without-match：列出文件内容不符合指定的样式的文件名称。
- -n 或 --line-number：在显示符合样式的那一行之前，标示出该行的列数编号。
- -o 或 --only-matching：只显示匹配PATTERN 部分。
- -q 或 --quiet或–silent：不显示任何信息。
- -r 或 --recursive：此参数的效果和指定"-d recurse"参数相同。
- -s 或 --no-messages：不显示错误信息。
- -v 或 --invert-match：显示不包含匹配文本的所有行。
- -V 或 --version：显示版本信息。
- -w 或 --word-regexp：只显示全字符合的列。
- -x --line-regexp：只显示全列符合的列。
- -y：此参数的效果和指定"-i"参数相同。
<a name="0VXBT"></a>
### 操作
<a name="UYzg3"></a>
#### 查 /etc/passwd有root的行
```bash
➜  / grep root /etc/passwd
root:x:0:0:root:/root:/bin/zsh
operator:x:11:0:operator:/root:/sbin/nologin
dockerroot:x:996:993:Docker User:/var/lib/docker:/sbin/nologin
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594560577077-c97df3e8-0885-484e-84f4-044395a23146.png#averageHue=%233b3b3b&height=99&id=qZfTx&originHeight=296&originWidth=1780&originalType=binary&ratio=1&rotation=0&showTitle=false&size=211837&status=done&style=none&title=&width=593.3333333333334)
<a name="EYq3N"></a>
#### 查 /etc/passwd 没有root的行(内容较多，仅展示几行)
```bash
➜  / grep -v root /etc/passwd
bin:x:1:1:bin:/bin:/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
adm:x:3:4:adm:/var/adm:/sbin/nologin
lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
sync:x:5:0:sync:/sbin:/bin/sync
shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown
halt:x:7:0:halt:/sbin:/sbin/halt
mail:x:8:12:mail:/var/spool/mail:/sbin/nologin
games:x:12:100:games:/usr/games:/sbin/nologin
ftp:x:14:50:FTP User:/var/ftp:/sbin/nologin
nobody:x:99:99:Nobody:/:/sbin/nologin
systemd-bus-proxy:x:999:998:systemd Bus Proxy:/:/sbin/nologin
systemd-network:x:192:192:systemd Network Management:/:/sbin/nologin
dbus:x:81:81:System message bus:/:/sbin/nologin
polkitd:x:998:997:User for polkitd:/:/sbin/nologin
tss:x:59:59:Account used by the trousers package to sandbox the tcsd daemon:/dev/null:/sbin/nologin
sshd:x:74:74:Privilege-separated SSH:/var/empty/sshd:/sbin/nologin
postfix:x:89:89::/var/spool/postfix:/sbin/nologin
chrony:x:997:995::/var/lib/chrony:/sbin/nologin
ntp:x:38:38::/etc/ntp:/sbin/nologin
nscd:x:28:28:NSCD Daemon:/:/sbin/nologin
tcpdump:x:72:72::/:/sbin/nologin
mysql:x:995:1000::/home/mysql:/bin/bash
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594560675105-6926aae8-41ab-4cc1-b567-9882fc5b2d35.png#averageHue=%23383838&height=458&id=oTQxR&originHeight=1374&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1757366&status=done&style=none&title=&width=1107.6666666666667)
<a name="89Lqf"></a>
#### 查 /etc/passwd 有root的行并显示行号
```bash
➜  / grep -n root /etc/passwd
1:root:x:0:0:root:/root:/bin/zsh
10:operator:x:11:0:operator:/root:/sbin/nologin
25:dockerroot:x:996:993:Docker User:/var/lib/docker:/sbin/nologin
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594560772010-f012131a-f354-4a9f-9a17-b205748e08c9.png#averageHue=%23363636&height=96&id=udNy0&originHeight=288&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=374625&status=done&style=none&title=&width=1107.6666666666667)
```bash
grep forest f.txt     #文件查找
grep forest f.txt cpf.txt #多文件查找
grep 'log' /home/admin -r -n #目录下查找所有符合关键字的文件
cat f.txt | grep -i shopbase    
grep 'shopbase' /home/admin -r -n --include *.{vm,java} #指定文件后缀
grep 'shopbase' /home/admin -r -n --exclude *.{vm,java} #反匹配
seq 10 | grep 5 -A 3    #上匹配
seq 10 | grep 5 -B 3    #下匹配
seq 10 | grep 5 -C 3    #上下匹配，平时用这个就妥了
cat f.txt | grep -c 'SHOPBASE'
```
<a name="keq33"></a>
#### 把包含 syslog 的行过滤出来
```bash
$ grep "syslog" g.txt
syslog:x:104:108::/home/syslog:/bin/false

# 可以忽略大小写
$ grep -i "SYSLOG" g.txt
syslog:x:104:108::/home/syslog:/bin/false
```
<a name="ZylIv"></a>
#### 把以 ntp 开头的行过滤出来
```bash
$ grep "^ntp" g.txt
ntp:x:108:114::/home/ntp:/bin/false

# 把false结尾的行过滤出来
$ grep "false$" g.txt
syslog:x:104:108::ntp.syslog:/bin/false
ntp:x:108:114::/home/ntp:/bin/false
```
<a name="ybKM3"></a>
#### 把匹配 ntp 的行以及下边的两行过滤出来
```bash
$ grep -A2 "syslog" g.txt
syslog:x:104:108::ntp.syslog:/bin/false
ntp:x:108:114::/home/ntp:/bin/false
sshd:x:109:65534::sshd:/usr/sbin/nologin
```
<a name="yh1Pm"></a>
#### 把包含 syslog 及上边的一行过滤出来
```bash
$ grep -B1 "syslog" g.txt
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
syslog:x:104:108::ntp.syslog:/bin/false
```
<a name="cNBWm"></a>
#### 把包含 syslog 以及上、下一行内容过滤出来
```bash
$ grep -C1 "syslog" g.txt
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
syslog:x:104:108::ntp.syslog:/bin/false
ntp:x:108:114::/home/ntp:/bin/false
```
<a name="1Q055"></a>
#### 过滤某个关键词，并输出行号
```bash
$ grep -n "ntp" g.txt
3:syslog:x:104:108::ntp.syslog:/bin/false
4:ntp:x:108:114::/home/ntp:/bin/false
```
<a name="zoMPv"></a>
#### 过滤不包含某关键词，并输出行号
```bash
$ grep -n -v "sbin" g.txt
1:0root:x:0:0:root:/root:/bin/zsh
3:syslog:x:104:108::ntp.syslog:/fnlas
4:ntp:x:108:114::/home/ntp:/bin/false
```
<a name="0lWRn"></a>
#### 删除掉空行
```bash
$ grep -v "^$" g.txt
# "^$"表示空行
```
<a name="k3BSU"></a>
#### 过滤包含 root 或 syslog 的行
```bash
# 用"-e"实现多选项间逻辑or关系
$ grep -e "root" -e "syslog" g.txt
0root:x:0:0:root:/root:/bin/zsh
syslog:x:104:108::ntp.syslog:/bin/false

# 等效于
$ grep -E "root|syslog" g.txt
```
<a name="i2hy4"></a>
#### 查看当前目录中包含某关键词的所有文件（这个很有用）
```bash
# -r 表示在当前目录递归查询
$ grep -r "font" .
./README.md: --pdf-mono-font-size 12
./default.css:  font-size: 120%;
./default.css:  font-weight: bold;
./ samples/foo/genmobi.sh:  --default-font 12

# 一般会用下边的命令，会更加有效(显示行号,且以单词严格匹配)
$ grep -rnw "font" .

# 在递归的过程中排除某些目录
$ grep -rnw --exclude-dir={.git,svn} "mail"
```
<a name="RHn7f"></a>
#### grep命令的其他操作
`grep -H 'spring' *.xml` 查找所以有的包含spring的xml文件<br />`grep 'test' d*` 显示所有以d开头的文件中包含test的行。<br />`grep 'test' aa bb cc` 显示在aa，bb，cc文件中匹配test的行。<br />`grep '[a-z]\{5\}' aa` 显示所有包含每个字符串至少有5个连续小写字符的字符串的行。
<a name="mgagQ"></a>
### 实例
<a name="jO7zA"></a>
#### 1、查询匹配字符的文件
```bash
cat /etc/passwd | grep root
root:x:0:0:root:/root:/bin/bash
operator:x:11:0:operator:/root:/sbin/nologin
```
<a name="KO2TK"></a>
#### 2、查询匹配字符前一行
```bash
cat /etc/passwd | grep root -B 1
root:x:0:0:root:/root:/bin/bash
--
mail:x:8:12:mail:/var/spool/mail:/sbin/nologin
operator:x:11:0:operator:/root:/sbin/nologin
```
<a name="uJNo1"></a>
#### 3、查询匹配字符后一行
```bash
cat /etc/passwd | grep root -A 1
root:x:0:0:root:/root:/bin/bash
bin:x:1:1:bin:/bin:/sbin/nologin
--
operator:x:11:0:operator:/root:/sbin/nologin
games:x:12:100:games:/usr/games:/sbin/nologin
```
<a name="XHS0k"></a>
#### 4、查询匹配字符前后一行
```bash
cat /etc/passwd | grep root -C 1
root:x:0:0:root:/root:/bin/bash
bin:x:1:1:bin:/bin:/sbin/nologin
--
mail:x:8:12:mail:/var/spool/mail:/sbin/nologin
operator:x:11:0:operator:/root:/sbin/nologin
games:x:12:100:games:/usr/games:/sbin/nologin
```
<a name="O92uj"></a>
#### 5、查询匹配以op开头的行
```bash
cat /etc/passwd | grep ^op
operator:x:11:0:operator:/root:/sbin/nologin
```
<a name="efevb"></a>
#### 6、查询匹配以bash结尾的行
```bash
cat /etc/passwd | grep bash$
root:x:0:0:root:/root:/bin/bash
devops:x:1000:1000::/home/devops:/bin/bash
```
<a name="DvsDE"></a>
## find
**在一个目录层级中查找文件。**<br />Linux find 命令用来在指定目录下查找文件。任何位于参数之前的字符串都将被视为欲查找的目录名。如果使用该命令时，不设置任何参数，则 find 命令将在当前目录下查找子目录与文件。并且将查找到的子目录和文件全部进行显示。<br />`find` 命令功能强大，可根据多种条件查询文件，随后进行自定义的操作，格式如下：
```bash
find [path...] [expression]
```

- `find` 会实际的扫描磁盘，所以速度会明显小于前三个。

命令：find<br />语法：find 路径  参数 输出<br />查找路径：指定具体目标路径；默认为当前目录<br />查找条件：指定的查找标准，可以文件名、大小、类型、权限等标准进行；默认找出指定路径下的所有文件<br />处理动作：对符合条件的文件做操作，默认输出至屏幕
```bash
find path -option [-print] [-exec -ok command ]{}\;
```
<a name="ejSNi"></a>
### 参数

- -mount, -xdev：只检查和指定目录在同一个文件系统下的文件，避免列出其它文件系统中的文件
- -amin n：在过去 n 分钟内被读取过
- -anewer file：比文件 file 更晚被读取过的文件
- -atime n：在过去 n 天内被读取过的文件
- -cmin n：在过去 n 分钟内被修改过
- -cnewer file：比文件 file 更新的文件
- -ctime n：在过去 n 天内创建的文件
- -mtime n：在过去 n 天内修改过的文件
- -empty：空的文件-gid n or -group name：gid 是 n 或是 group 名称是 name
- -ipath p, -path p：路径名称符合 p 的文件，ipath 会忽略大小写
- -name name, -iname name：文件名称符合 name 的文件。iname 会忽略大小写
- -size n：文件大小 是 n 单位，b 代表 512 位元组的区块，c 表示字元数，k 表示 kilo bytes，w 是二个位元组。
- -type c：文件类型是 c 的文件。<br />d：目录<br />c：字型装置文件<br />b：区块装置文件<br />p：具名贮列<br />f：一般文件<br />l：符号连结
<a name="XvHbR"></a>
### 查找条件
<a name="Ijh0t"></a>
#### 指搜索层级
-maxdepth level 最大搜索目录深度，指定目录为第1级<br />-mindepth level 最小搜索目录深度
<a name="u5dau"></a>
#### 先处理目录内的文件，再处理目录
-depth
<a name="mJoc2"></a>
#### 根据文件名和inode查找
-name“文件名称”：支持使用glob<br />*,?,[],[^]<br />-iname“文件名称”：不区分字母大小写<br />-inum n 按inode号查找<br />-samefile name 相同inode号的文件<br />-links n 链接数为n的文件<br />-regex “PATTERN”：以PATTERN匹配整个文件路径，而非文件名称
<a name="PW0kB"></a>
#### 根据属主、属组查找
-user USERNAME:查找属主为指定用户（UID）的文件<br />-group GRPNAME:查找属组为指定组（GID）的文件<br />-uid UserID：查找属主为指定的UID号文件<br />-gid GroupID:查找属组为指定的GID号的文件<br />-nouser:查找没有属主的文件<br />-nogroup:查找没有属组的文件
<a name="BvYIR"></a>
#### 根据文件类型查找
type TYPE：<br />f：普通文件<br />d：目录文件<br />l：符号链接文件<br />s：套接字文件<br />b：块设备文件<br />c：字符设备文件<br />p：管道文件
<a name="va4it"></a>
#### 空文件或目录
-empty<br />Find/app -type d -empty
<a name="KAd64"></a>
#### 组合条件
与：-a<br />或：-o<br />非：-not,!
<a name="svF3Q"></a>
### 常用参数
```
路径: 例如用 . 来表示当前目录，用 / 来表示系统根目录。
-print：显示的时候”\n”做为定界符, 换行
-print0：与xargs配套使用，以“\0”做为定界符
```
find常用命令选项:
```
-name   按照文件名查找文件。“名称”
-perm   按照文件权限来查找文件。666 777 等
-depth  在查找文件时，首先查找当前目录中的文件，然后再在其子目录中查找
-user   按照文件属主来查找文件
-atime  -ctime (单位是天)
-mmin -cmin  -amin(单位是分钟)
-size n [c] 查找文件长度为n块的文件，带有c时表示文件长度以字节计
-follow  如果find命令遇到符号链接文件，就跟踪至链接所指向的文件。
```
<a name="uHZra"></a>
### 操作
<a name="oN3zg"></a>
#### 根据文件名称查找
`find / -name filename.txt` 根据名称查找/目录下的filename.txt文件。<br />`find . -name "*.xml"` 递归查找所有的xml文件
<a name="q3O36"></a>
#### 查看当前目录下文件名为.conf的文件
```bash
➜  /etc find *.conf
asound.conf
chrony.conf
dnsmasq.conf
dracut.conf
e2fsck.conf
GeoIP.conf
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594561728436-eafad781-9df5-4238-842b-007aab99d0ef.png#averageHue=%23393939&height=583&id=qx7aP&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2001754&status=done&style=none&title=&width=1107.6666666666667)
<a name="JkLIn"></a>
#### 查看当前目录下yum.开头的文件,输出结果换行显示(默认)
```bash
➜  /etc find yum.* -print
yum.conf
yum.repos.d
yum.repos.d/CentOS-Base.repo
yum.repos.d/epel.repo
➜  /etc find yum.*
yum.conf
yum.repos.d
yum.repos.d/CentOS-Base.repo
yum.repos.d/epel.repo
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594561833395-cb37a227-c261-4ebf-80c2-397844dd81f2.png#averageHue=%233b3b3b&height=205&id=QJ2sf&originHeight=616&originWidth=988&originalType=binary&ratio=1&rotation=0&showTitle=false&size=240096&status=done&style=none&title=&width=329.3333333333333)
<a name="zTl8u"></a>
#### 查看当前目录下yum.开头的文件,输出结果不换行显示
```bash
➜  /etc find yum.* -print
yum.conf
yum.repos.d
yum.repos.d/CentOS-Base.repo
yum.repos.d/epel.repo
➜  /etc find yum.*
yum.conf
yum.repos.d
yum.repos.d/CentOS-Base.repo
yum.repos.d/epel.repo
➜  /etc find yum.* -print0
yum.confyum.repos.dyum.repos.d/CentOS-Base.repoyum.repos.d/epel.repo#
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594561987457-2ae57e73-fb9a-469d-9900-b3da0805ba97.png#averageHue=%23363636&height=240&id=F7bLY&originHeight=719&originWidth=1969&originalType=binary&ratio=1&rotation=0&showTitle=false&size=529797&status=done&style=none&title=&width=656.3333333333334)<br />对查找的内容执行相应命令<br />默认执行动作：-print<br />常用动作:
```
-exec  这个参数后可以跟自定义shell命令
```
操作:
<a name="CBvu2"></a>
#### 查询yum.开头的文件并用ls查询其更多属性
```bash
➜  /etc find yum.* -exec ls -l {} \;
-rw-r--r--. 1 root root 970 Nov 15  2016 yum.conf
total 8
-rw-r--r-- 1 root root 675 Jul 18  2019 CentOS-Base.repo
-rw-r--r-- 1 root root 230 Jul 18  2019 epel.repo
-rw-r--r-- 1 root root 675 Jul 18  2019 yum.repos.d/CentOS-Base.repo
-rw-r--r-- 1 root root 230 Jul 18  2019 yum.repos.d/epel.repo
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594562138424-ed08c436-3642-4cf4-a930-2c486b093100.png#averageHue=%233c3c3c&height=150&id=AKV9j&originHeight=451&originWidth=2088&originalType=binary&ratio=1&rotation=0&showTitle=false&size=344491&status=done&style=none&title=&width=696)
<a name="uHIr6"></a>
#### 查询fcscanf文件并变成.bak结尾的文件
```bash
➜  /etc find fcscanf
fcscanf
➜  /etc find fcscanf -exec mv {} {}.bak \;
➜  /etc find *.bak
fcscanf.bak
nsswitch.conf.bak
➜  /etc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594562435773-9a1c8869-9ae1-427c-b865-c150da797e12.png#averageHue=%23373737&height=168&id=FJ1Ys&originHeight=505&originWidth=1359&originalType=binary&ratio=1&rotation=0&showTitle=false&size=244460&status=done&style=none&title=&width=453)
<a name="dOb4P"></a>
#### 递归查找所有文件内容中包含hello world的xml文件
`find . -name "*.xml" | xargs grep "hello world"`
<a name="N0we4"></a>
#### 删除文件大小为零的文件
`find ./ -size 0 | xargs rm -f &`
<a name="sBhVS"></a>
### 逻辑查询
```
-a  并且
-o  或者
+   高于
-   低于
```
<a name="0f6Hz"></a>
#### 查看 .sh或者.q结尾的文件。
```
➜  test find . -name "*.sh" -o -name "*.q"
./book.q
./stop.sh
./start.sh
```
<a name="30Sb0"></a>
#### 查看.sh结尾并且s开头的文件。
```
➜  test find . -name "*.sh" -a -name "s*"
./stop.sh
./start.sh
```
<a name="gOMvL"></a>
#### 在文件中插入内容后查看。
```
➜  test ll
总用量 12K
-rw-r--r-- 1 root root  9 11月  4 15:10 book.q
-rw-r--r-- 1 root root 13 11月  4 15:10 start.sh
-rw-r--r-- 1 root root  4 11月  4 15:10 stop.sh
-rw-r--r-- 1 root root  0 11月  4 14:56 xq1.txt.bak
-rw-r--r-- 1 root root  0 11月  4 14:56 xq2.txt.bak
-rw-r--r-- 1 root root  0 11月  4 14:56 xq.txt.bak
```
<a name="Fc8pK"></a>
#### 查看/etc 大于40k小于50k的文件
```bash
➜  test find /etc/ -size +40k -a -size  -50k
/etc/selinux/targeted/active/modules/100/sysadm/hll
/etc/selinux/targeted/contexts/files/file_contexts.homedirs.bin
```
<a name="jB2id"></a>
### 按类型和时间查询
```bash
sudo -u admin find /home/admin /tmp /usr -name \*.log(多个目录去找)
find . -iname \*.txt(大小写都匹配)
find . -type d(当前目录下的所有子目录)
find /usr -type l(当前目录下所有的符号链接)
find /usr -type l -name "z*" -ls(符号链接的详细信息 eg:inode,目录)
find /home/admin -size +250000k(超过250000k的文件，当然+改成-就是小于了)
find /home/admin f -perm 777 -exec ls -l {} \; (按照权限查询文件)
find /home/admin -atime -1  1天内访问过的文件
find /home/admin -ctime -1  1天内状态改变过的文件    
find /home/admin -mtime -1  1天内修改过的文件
find /home/admin -amin -1  1分钟内访问过的文件
find /home/admin -cmin -1  1分钟内状态改变过的文件    
find /home/admin -mmin -1  1分钟内修改过的文件
```

- 查询当前目录下所有的 markdown 文档：
```
$ find . -name "*.log"
```

- 查询用户视频文件夹中大于 100M 的文件：
```bash
$ find ~/Videos/ -size +100M
```

- 查询用户音乐文件夹中过去七天访问过的文件：
```bash
$ find ~/Music/ -atime -7
```

- 查询系统中、三个月之前创建的、一个月之内没有访问过、大于 30M 的日志文件，并删除：
```bash
find / -ctime +90 -atime +30 -size +1M -name "*.log" -delete
```
<a name="ZYtqt"></a>
### 实例
<a name="muKEm"></a>
#### 1、查询当前目录及其子目录中所有以"xxx"结尾的文件
```bash
find . -name "*.log"
```
<a name="SwHee"></a>
#### 2、将当前目录及其子目录中的所有文件列出
```bash
find . -type -f
```
<a name="xoWl7"></a>
#### 3、将当前目录及其子目录下所有最近 20 天内更新过的文件列出
```bash
find . -ctime 20
```
<a name="sDlxX"></a>
#### 4、将当前目录及其子目录下所有最近 20 天内访问过的文件列出
```bash
find . -atime 20
```
<a name="H0c7M"></a>
#### 5、查找 /var/log 目录中更改时间在 7 日以前的普通文件，并在删除之前询问它们
```bash
find /var/log -type f -mtime +7 -ok rm {} \;
```
<a name="HjZGA"></a>
#### 6、查找当前目录中文件属主具有读、写权限，并且文件所属组的用户和其他用户具有读权限的文件
```bash
find . -type f -perm 644 -exec ls -l {} \;
```
<a name="qBJyV"></a>
#### 7、查找超过10MB的所有.mp3文件，并使用一个命令删除它们
```bash
find . -type f -name "*.mp3" -size +10m -exec rm -rf {} \;
```
<a name="AGkq8"></a>
#### 8、找到大于50MB且小于100MB的所有文件
```bash
find . -type f -size +50m -size -100m
```
