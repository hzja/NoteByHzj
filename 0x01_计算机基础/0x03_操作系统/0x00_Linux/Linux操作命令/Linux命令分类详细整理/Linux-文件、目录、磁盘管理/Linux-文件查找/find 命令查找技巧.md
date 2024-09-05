Linux
<a name="peT81"></a>
## 前言
find命令是日常工作中比较常用的Linux命令。全面的掌握这个命令可以使很多操作达到事半功倍的效果。如果对find命令有以下这些疑惑，本文都能帮你解决：

1. find命令的格式是什么？
2. 参数中出现+或-号是什么意思？比如`find / -mtime +7`与`find / -mtime -7`什么区别？
3. `find /etc/ -name "passwd" -exec echo {} \;`和`find /etc/ -name "passwd" -exec echo {} +`有啥区别？
4. `-exec`参数为什么要以“`\;`”结尾，而不是只写“`;`”？
<a name="nbxcu"></a>
## 命令基础
find命令大家都比较熟悉，反倒想讲的有特色比较困难。那干脆怎么平淡怎么来好了。一般用的find命令格式很简单，一般分成三个部分：
```bash
find /etc -name 'passwd'
```
格式如上，第一段find命令。第二段，要搜索的路径。这一段目录可以写多个，如：
```bash
find /etc /var /usr -name 'passwd'
```
第三段，表达式。例子中用的是-name “passwd”这个表达式，指定条件为找到文件名是passwd的文件。对于find命令，最需要学习的是表达式这一段。表达式决定了要找的文件是什么属性的文件，还可以指定一些“动作”，比如将匹配某种条件的文件删除。所以，find命令的核心就是表达式（EXPRESSION）的指定方法。<br />find命令中的表达式有四种类型，分别是：

1. Tests：就是最常用的指定查找文件的条件。
2. Actions：对找到的文件可以做的操作。
3. Global options：全局属性用来限制一些查找的条件，比如常见的目录层次深度的限制。
4. Positional options：位置属性用来指定一些查找的位置条件。

这其中最重要的就是Tests和Actions，他们是find命令的核心。另外还有可以将多个表达式连接起来的操作符，他们可以表达多个表达式之间的逻辑关系和运算优先顺序，叫做Operators。<br />下面就来分类看一下这些个分类的功能。
<a name="MxlOa"></a>
## TESTS
find命令是通过文件属性查找文件的。所以，find表达式的tests都是文件的属性条件，比如文件的各种时间，文件权限等。很多参数中会出现指定一个数字n，一般会出现三种写法：<br />`+n`：表示大于n。<br />`-n`：表示小于n。<br />`n`：表示等于n。
<a name="kLgEk"></a>
### 根据时间查找
比较常用数字方式来指定的参数是针对时间的查找，比如`-mtime n`：查找文件修改时间，单位是天，就是n*24小时。举个例子说：
```bash
[root@zorrozou-pc0 zorro]# find / -mtime 7 -ls
```
为了方便看到结果，在这个命令中使用了-ls参数，具体细节后面会详细解释。再此只需要知道这个参数可以将符合条件的文件的相关属性显示出来即可。那么就可以通过这个命令看到查找到的文件的修改时间了。
```bash
[root@zorrozou-pc0 zorro]# find / -mtime 7 -ls|head
524295      4 drwxr-xr-x  12  root     root         4096 6月  8 13:43 /root/.config
524423      4 drwxr-xr-x   2  root     root         4096 6月  8 13:43 /root/.config/yelp
524299      4 drwxr-xr-x   2  root     root         4096 6月  8 13:23 /root/.config/dconf
524427      4 -rw-r--r--   1  root     root         3040 6月  8 13:23 /root/.config/dconf/user
...
```
可以发现，时间都集中在6月8号，而今天是：
```bash
[root@zorrozou-pc0 zorro]# date
2016年 06月 15日 星期三 14:30:09 CST
```
实际上，当在mtime后面指定的是7的时候，实际上是找到了距离现在7个24小时之前修改过的文件。如果再考究一下细节的话，可以使用这个命令再将找到的文件用时间排下顺序：
```bash
find / -mtime 7 -exec ls -tld {} \+
```
此命令用到了exec参数，后面会详细说明。可以发现，找到的文件实际上是集中在6月7日的14:30到6月8日的14:30这个范围内的。就是说，实际上，指定7天的意思是说，找到文件修改时间范围属于距离当前时间7个24小时到8个24小时之间的文件，这是不加任何+-符号的7的含义。如果是-mtime -7呢？
```bash
find / -mtime -7 -exec ls -tld {} \+
```
可以发现找到的文件是从现在开始到7个24小时范围内的文件。但是不包括7个24小时到8个24小时的时间范围。那么`-mtime +7`也应该好理解了。这就是find指定时间的含义。类似的参数还有：<br />-`ctime`：以天为单位通过change time查找文件。<br />`-atime`：以天为单位通过access time查找文件。<br />`-mmin`：以分钟为单位通过modify time查找文件。<br />`-amin`：以分钟为单位通过access time查找文件。<br />`-cmin`：以分钟单位通过change time查找文件。<br />这些参数都是指定一个时间数字n，数字的意义跟mtime完全一样，只是时间的单位和查找的时间不一样。<br />除了指定时间以外，find还可以通过对比某一个文件的相关时间找到符合条件的文件，比如-anewer file。
```bash
find /etc -anewer /etc/passwd
```
这样可以在/etc/目录下找到文件的access time比/etc/passwd的access time更新的所有文件。类似的参数还有：<br />`-cnewer`：比较文件的change time。<br />`-newer`：比较文件的modify time。<br />`-newer`还有一种特殊用法，可以用来做各种时间之间的比较。比如想找到文件修改时间比/etc/passwd文件的change time更新的文件：
```bash
find /etc/ -newermc /etc/passwd
```
这个用法的原型是：`find /etc/ -newerXY file`。其中Y表示的是跟后面file的什么时间比较，而X表示使用查找文件什么时间进行比较。`-newermc`就是拿文件的modify time时间跟file的change time进行比较。X和Y可以使用的字母为：<br />a：文件access time。<br />c：文件change time。<br />m：文件modify time。<br />在某些支持记录文件的创建时间的文件系统上，可以使用B来表示文件创建时间。ext系列文件系统并不支持记录这个时间。
<a name="OjnGQ"></a>
### 根据用户查找
`-uid n`：文件的所属用户uid为n。<br />`-user name`：文件的所属用户为name。<br />`-gid n`：文件的所属组gid为n。<br />`-group name`：所属组为name的文件。<br />`-nogroup`：没有所属组的文件。<br />`-nouser`：没有所属用户的文件。
<a name="tAKTJ"></a>
### 根据权限查找
`-executable`：文件可执行。<br />`-readable`：文件可读。<br />`-writable`：文件可写。<br />`-perm mode`：查找权限为mode的文件，mode的写法可以是数字，也可以是ugo=rwx的方式如：
```bash
[root@zorrozou-pc0 zorro]# find /etc/ -perm 644 -ls
```
这个写法跟：
```bash
[root@zorrozou-pc0 zorro]# find /etc/ -perm u=rw,g=r,o=r -ls
```
是等效的。<br />另外要注意，mode指定的是完全符合这个权限的文件，如：
```bash
[root@zorrozou-pc0 zorro]# find /etc/ -perm u=rw,g=r -ls
263562      4 -rw-r-----   1  root     brlapi         33 11月 13  2015 /etc/brlapi.key
```
没描述的权限就相当于指定了没有这个权限。<br />mode还可以使用/或-作为前缀进行描述。如果指定了-mode，就表示没指定的权限是忽略的，就是说，权限中只要包涵相关权限即可。如：
```bash
find /etc/ -perm 600 -ls
```
这是找到所有只有rw———-权限的文件，而-600就表示只要是包括了rw的其他位任意的文件。mode加/前缀表示的是，指定的权限只要某一位复合条件就可以，其他位跟-一样忽略，就是说-perm /600还可以找到r————或者-w———-这样权限的文件。老版本的/前缀是用+表示的，新版本的find意境不支持mode前加+前缀了。
<a name="JG96B"></a>
### 根据路径查找
`-name pattern`：文件名为pattern指定字符串的文件。注意如果pattern中包括*等特殊符号的时候，需要加””。<br />`-iname`：name的忽略大小写版本。<br />`-lname pattern`：查找符号连接文件名为pattern的文件。<br />`-ilname`：lname的忽略大小写版本。<br />`-path pattern`：根据完整路径查找文件名为pattern的文件，如：
```
[root@zorrozou-pc0 zorro]# find /etc -path '/e*d'| head
/etc/machine-id
/etc/profile.d
/etc/vnc/xstartup.old
/etc/vnc/config.d
/etc/vnc/updateid
/etc/.updated
```
`-ipath`：path的忽略大小写版本。<br />`-regex pattern`：用正则表达式匹配文件名。<br />`-iregex`：regex的忽略大小写版本。
<a name="Sc94Z"></a>
### 其他状态查找
`-empty`：文件为空而且是一个普通文件或者目录。<br />`-size n[cwbkMG]`：指定文件长度查找文件。单位选择位：

- c：字节单位。
- b：块为单位，块大小为512字节，这个是默认单位。
- w：以words为单位，words表示两个字节。
- k：以1024字节为单位。
- M：以1048576字节为单位。
- G：以1073741824字节温单位。

n的数字指定也可以使用+-号作为前缀。意义跟时间类似，表示找到小于(-)指定长度的文件或者大于(+)指定长度的文件。<br />`-inum`：根据文件的inode编号查找。<br />`-links n`：根据文件连接数查找。<br />`-samefile name`：找到跟name指定的文件完全一样的文件，就是说两个文件是硬连接关系。<br />`-type c`：以文件类型查找文件：<br />c可以选择的类型为：

- b：块设备
- c：字符设备
- d：目录
- p：命名管道
- f：普通文件
- l：符号连接
- s：socket
<a name="ENza9"></a>
## ACTIONS
表达式中的actions类型参数主要是用来对找到的文件进行操作的参数。在上面的例子中，已经看到可以使用-ls参数对找到的文件进行长格式显示，这就是一个actions类型的参数。类似的参数还有。<br />`-fls file`：跟-ls功能一样，区别是将信息写入file指定的文件，而不是显示在屏幕上。<br />`-print`：将找到的文件显示在屏幕上，实际上默认find命令就会将文件打印出来显示。<br />`-print0`：-print参数会将每个文件用换行分割，而这个参数适用null分割。有时候在脚本编程时可能会用上。<br />`-fprint file`：-print参数的写入文件版本。将内容写到文件中，而不是显示在屏幕上。<br />`-fprint0 file`：-print0的写入文件版本。<br />`-delete`：可以将找到的文件直接删除。<br />`-printf`：格式化输出方式打印。如：
```bash
[root@zorrozou-pc0 zorro]# find /etc/ -name 'pass*' -printf '%p '
/etc/default/passwd /etc/pam.d/passwd /etc/passwd- /etc/passwd
```
显示文件名，并以空格分隔。`%p`代表文件名。其他信息可以参见man find。<br />`-prune`：如果复合条件的是一个目录，则不进入目录进行查找。例子：
```bash
[root@zorrozou-pc0 zorro]# mkdir /etc/passs
[root@zorrozou-pc0 zorro]# touch /etc/passs/passwd
[root@zorrozou-pc0 zorro]# find /etc/ -name 'pass*' -prune
/etc/passs
/etc/default/passwd
/etc/pam.d/passwd
/etc/passwd-
/etc/passwd
[root@zorrozou-pc0 zorro]# find /etc/ -name 'pass*'
/etc/passs
/etc/passs/passwd
/etc/default/passwd
/etc/pam.d/passwd
/etc/passwd-
/etc/passwd
```
先创建了一个/etc/passs的目录，然后在这个目录下创建了一个叫passwd的文件。之后先用带-prune的find看到，能显示出passs目录，但是目录中的passwd文件并没有显示，说明这个参数让find命令没有进入这个目录查找。而后一个不带-prune参数的find显示出了passs目录下的passwd。<br />`-quit`：找到符合条件的文件后立即退出。
<a name="u4Ikd"></a>
### find中执行命令
`-exec`<br />find命令的`exec`是一个非常好用的参数，当然其可能造成的破坏也可能非常大。在学习它之前，先要提醒大家，使用之前千万要确定自己在做什么。<br />这个参数的常见格式是：
```bash
-exec command ;
```
注意后面的分号。它是用来给find做标记用的。find在解析命令的时候，要区分给定的参数是要传给自己的还是要传给command命令的。所以find以分号作为要执行命令所有参数的结束标记。命令返回值为0则返回true。在exec参数指定的执行命令中，可以使用{}符号表示当前find找到的文件名。比如：
```bash
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd' -exec echo {} \;
/etc/default/passwd
/etc/pam.d/passwd
/etc/passwd
```
上面的命令表示，找到/etc/目录下文件名为passwd的文件，并echo其文件名。注意再使用分号的时候前面要加转移字符\，因为分号也是bash的特殊字符，所以bash会先解释它。前面加上\就可以让bash直接将其船体给find命令，这个分号由find解释，而不是bash。其实这个exec用的比较废话，毕竟find本身就会找到相关条件的文件并显示其文件名。但是试想如果将echo换成rm或者cp，是不是就有意义的多？比如：
```bash
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd' -exec rm {} \;
```
请不要执行这个命令！！<br />或者：
```bash
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd' -exec cp {} {}.bak \;
```
这个命令可以将符合条件的文件都加个.bak后缀备份一份。于是可以执行删除了：
```bash
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd.bak' 
/etc/default/passwd.bak
/etc/pam.d/passwd.bak
/etc/passwd.bak
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd.bak' -exec rm {} \;
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd.bak'
```
当然，删除前还是要确认清楚要删的文件一定是对的。
```bash
-execdir
```
execdir和exec有一些差别，主要是在执行指定的命令时，那个相关命令是在那个工作目录下执行的差别。exec是在find所指定的起始目录，而execdir是文件所在目录。对比一下就明白了：
```bash
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd' -exec echo {} \;
/etc/default/passwd
/etc/pam.d/passwd
/etc/passwd
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd' -execdir echo {} \;
./passwd
./passwd
./passwd
```
一个命令打印出来的路径都是/etc/开头，另一个显示的都是当前目录下的某某文件。<br />execdir的方式要比exec安全一些，因为这种执行方式避免了在解析文件名时所产生的竞争条件。<br />出了上述两种比较典型的执行命令的方法以外，find还对这两个参数提供了另一种形式的命令执行格式：

- `-exec command {} +`
- `-execdir command {} +`

还是先用例子来看一下这个格式和以分号结束的方式的差别：
```bash
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd' -exec echo {} \;
/etc/default/passwd
/etc/pam.d/passwd
/etc/passwd
[root@zorrozou-pc0 find]# find /etc/ -name 'passwd' -exec echo {} \+
/etc/default/passwd /etc/pam.d/passwd /etc/passwd
```
光这样看可能还不是很明显，可以这样在描述一遍他们的执行过程：
```
echo /etc/default/passwd
echo /etc/pam.d/passwd
echo /etc/passwd
```
和
```bash
echo /etc/default/passwd /etc/pam.d/passwd /etc/passwd
```
其实就是说，对于command {} ;格式来说，每找到一个文件就执行一遍相关命令，而command {} +格式的意思是说，先执行find，找到所有符合条件的文件之后，将每个文件作为命令的一个参数传给命令执行，exec指定的命令实际上只被执行了一次。这样用的限制也是不言而喻的：{}只能出现一次。
```bash
[root@zorrozou-pc0 find]# find /etc -mtime -7 -type f -exec cp -t /tmp/back/ {} \+
```
上面这个命令将符合条件的文件全部cp到了/tmp/back目录中，当然如果文件有重名的情况下，会被覆盖掉。从这个命令中学习一下`{} +`格式的使用注意事项，它不能写成：
```bash
find /etc -mtime -7 -type f -exec cp {} /tmp/back/ \+
```
所以只能使用`-t`参数改变cp命令的参数顺序来指定相关的动作。<br />无论如何，直接使用exec和execdir是很危险的，因为他们会直接对找到的文件调用相关命令，并且没有任何确认。所以不得不在进行相关操作前再三确认，以防止误操作。当然，find命令也给了更安全的exec参数，它们就是：

- `-ok`
- `-okdir`

它们的作用跟exec和execdir一样，区别只是在做任何操作之前，会让用户确认是不是ok？如：
```bash
[root@zorrozou-pc0 find]# find /etc -mtime -7 -type f -ok cp -t /tmp/back/ {} \;
< cp ... /etc/bluetooth/main.conf > ?
```
于是，每一次cp都要确认是不是要这么做。只要输入的是y或者以y开头的任何字符串，都是确认。其他的字符串是否认。另外，这两个参数不支持{} +的格式。
<a name="NhBiC"></a>
## OPERATORS
find的操作符（OPERATORS）实际上是用来连接多个表达式和确定其逻辑关系用的。如：
```bash
[root@zorrozou-pc0 zorro]# find /etc -name 'pass*' -type f
/etc/passs/passwd
/etc/default/passwd
/etc/pam.d/passwd
/etc/passwd-
/etc/passwd
```
这个find命令中使用了两个表达式，他们之间没有任何分隔，这是实际上表达的含义是，找到两个条件都符合的文件。实际上就是表达式的逻辑与关系，这跟`-a`参数连接或者`-and`参数一样：
```bash
[root@zorrozou-pc0 zorro]# find /etc -name 'pass*' -a -type f
/etc/passs/passwd
/etc/default/passwd
/etc/pam.d/passwd
/etc/passwd-
/etc/passwd
[root@zorrozou-pc0 zorro]# find /etc -name 'pass*' -and -type f
/etc/passs/passwd
/etc/default/passwd
/etc/pam.d/passwd
/etc/passwd-
/etc/passwd
```
除了逻辑与关系以外，还有逻辑或关系：
```bash
[root@zorrozou-pc0 zorro]# find /etc -name 'pass*' -o -type f

[root@zorrozou-pc0 zorro]# find /etc -name 'pass*' -or -type f
```
表示两个条件只要符合其中一个都可以。<br />在条件表达式前面加`!`表示对表达式取非。同样的也可以用-not参数。另外如果表达式很多，可以使用( expr )确定优先级，如：
```java
find / \( -name 'passwd' -a -type f \) -o \( -name 'shadow' -a -type f \)
```
这里表示的是：`-name "passwd" -a -type f`和`-name "shadow" -a -type f`是或关系。
<a name="GFocp"></a>
## 最后
find中还可能常用的其他参数比如：<br />`-depth`：制定了这个参数后，遇到目录先进入目录操作目录中的文件，最后再操作目录本身。<br />`-maxdepth`：目录最大深度限制。<br />`-mindepth`：目录最小深度限制。<br />还有一些其他相关参数大家可以在man find中自行补充。
