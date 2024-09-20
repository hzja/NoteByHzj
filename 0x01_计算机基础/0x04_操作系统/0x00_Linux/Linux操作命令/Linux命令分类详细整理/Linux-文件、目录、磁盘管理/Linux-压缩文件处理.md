Linux tar<br />Linux 下面的压缩命令主要有下面几个：

- zip，对应压缩文件格式：.zip
- gzip，对应压缩文件格式：.gz
- bzip2，对应压缩文件格式：.bz2
- xz，对应压缩文件格式：.xz

打包命令：

- tar，对应压缩文件格式：.tar
   - 如果和 gzip 一块使用：.tar.gz 或者 .tgz
   - 如果和 bzip2 一块使用：.tar.bz2
   - 如果和 xz 一块使用：.tar.xz

所谓打包，就是将一个文件、或者一堆文件打包成一个单一文件的过程。<br />这里还需要说明一个术语：压缩率。其计算公式是：（压缩后文件体积） / （压缩前文件体积）。<br />还有一点是：gzip bzip2 xz 这几个命令都只能对但一个文件做压缩，不能压缩一个目录，压缩目录需要通过 tar 配合这几个压缩命令来处理，或者使用 zip 命令<br />zip 命令能提供的功能和 tar 非常类似，更详细的使用方法使用 tldr zip & tldr unzip 来查看。
<a name="vi3Ui"></a>
## tar命令
<a name="B8FTf"></a>
### 命令简介
tar 命令用于打包、压缩与解压压缩包文件。<br />tar 命令常常用于打包、压缩某些文件或目录，也可以添加新文件到归档文件中。Tar 代表的是磁带存档，是一种归档的文件格式，早期用于将文件归档到磁带备份存储。现可以用于收集、分发、归档文件，还可以保留文件原有的属性，如：用户和组权限，访问和修改日期以及目录结构。<br />tar 命令的参数可以说是 Linux 上面最复杂且最不容易记忆的几个之一了。<br />先解释下 tar 的作用：tar 是打包命令，也就是将一系列文件，或者某个文件夹，打包成单一文件。如果配合压缩命令 gzip bzip2 xz （不包含 zip ），那么其可以完成 打包+压缩 的功能。<br />tar 执行完后，原有文件或者目录将保留。
<a name="satkh"></a>
### 语法格式
```
tar [OPTIONS] [FILE]
```
<a name="hgbOF"></a>
### 选项参数说明
```
-A  #新增文件到已经存在的归档文件
-B  #设置区块大小
-c  #建立新的归档文件
-C  #将压缩的文件解压到指定的目录下
-d  #记录文件的差异
-x  #解压或提取归档文件内容              
-t  #列出备份文件的内容
-z  #通过gzip命令来压缩/解压缩文件，文件名一般为 xx.tar.gz
-Z  #通过compress命令处理备份文件
-f  #指定备份文件
-v  #显示命令执行过程
-r  #添加新文件到已经压缩的文件中
-u  #添加改变了和现有的文件到已经存在的压缩文件
-j  #通过bzip2命令来压缩/解压缩文件，文件名一般为xx.tar.bz2
-v  #显示操作过程；
-k  #保留原有文件不覆盖
-m  #保留文件不被覆盖
-w  #确认压缩文件的正确性
-p  #保留原来的文件权限与属性
-P  #使用文件名的绝对路径，不删除文件名称前的“/”号
-N  #只将较指定日期更新的文件保存到备份文件中
--exclude=[范本样式]  #排除符合范本样式的文件
--remove-files       #归档/压缩之后删除源文件
```
<a name="nMzAU"></a>
### 压缩与解压
| 拓展名称 | 分类 |
| --- | --- |
| .zip/.rar | Windows系统压缩包 |
| .tar | 在Linux下的打包文件 |
| .gz | 在Linux下的压缩文件 |
| .tar.gz | 在Linux下的打包压缩文件 |

<a name="f0NgJ"></a>
#### 压缩文件
| tar | 参数 压缩包名 一个或多个文件{对一个或多个文件进行打包并压缩} |
| --- | --- |
| -c | 创建一个打包的文件 |
| -v | 显示打包的详细过程 |
| -z | 压缩文件 |
| -f<压缩文件名称> | 只当压缩包的文件名称，f后面跟着压缩的文件名，一般放在参数最后面 |

```bash
$ touch a.c       
$ tar -czvf test.tar.gz a.c   //压缩 a.c文件为test.tar.gz
a.c
```
<a name="oVzRd"></a>
#### 解压文件
| tar 参数 压缩文件名称 | 解压文件到当前目录 |
| --- | --- |
| -x | 解压指定的文件 |
| -v | 显示解压的详细过程 |
| -f<压缩文件> | 指定要解压的文件 |
| -C | 解压到指定的目录下 |

```bash
$ tar -xzvf test.tar.gz 
a.c
```
<a name="yk9E2"></a>
### 应用举例
```bash
tar -cf mingongge.tar *.jpg
#将所有.jpg的文件打包成一个名为mingongge.tar的文件
 
tar -rf mingongge.tar *.gif
#将所有.gif的文件增加到mingongge.tar的包里
 
tar -uf mingonggel.tar mingongge.gif
#更新mingongge.tar文件中的mingongge.gif文件
 
tar -tf mingongge.tar
#列出 all.tar 包中所有文件
tar -cfv mingongge.tar foo bar  
#将文件foo和bar打包成mingongge.tar文件包，也可以理解成：从这两个文件中去创建这个mingongge.tar文件
tar -tvf mingongge.tar         
#详细列出mingongge.tar中的所有文件
tar -xf mingongge.tar          
#从mingongge.tar提取所有文件
```
<a name="tuyD0"></a>
#### 列出压缩文件内容
```bash
$ tar -tzvf test.tar.gz 
-rw-r--r-- root/root     0 2010-05-24 16:51:59 a.c
```
<a name="lOVsG"></a>
#### 将文件全部打包成tar包
```bash
tar -cvf mingongg.tar mingongg.log       #仅打包，不压缩！
tar -zcvf mingongg.tar.gz mingongg.log   #打包后，以gzip方式压缩
tar -jcvf mingongg.tar.bz2 mingongg.log  #打包后，以bzip2方式压缩
```
<a name="ukPwn"></a>
#### 解压目录
```bash
tar -xvf portal-web-v2.0.0.tar --strip-components 1  -C 指定目录
#排除目录--strip-components
```
<a name="4UyMJ"></a>
#### 将 tar包解压缩
```bash
tar -zxvf /opt/soft/test/log.tar.gz
```
<a name="0aEO0"></a>
#### 打包或压缩文件时，排队指定的文件类型
```bash
tar -zcf mingongge.tar.gz /etc/ /var/ --exclude=*.txt
```
注意：如果在使用过程中遇到这类错误提示
```bash
tar: Removing leading `/` from member names
```
原因是tar默认为相对路径，使用绝对路径的话就回报这个错，可以使用`-P`（大写）参数解决这个问题。
<a name="wpDba"></a>
#### 只打包文件不压缩
打包单个或多个文件：
```bash
tar -cv -f target.tar a.txt b.txt c.txt d.txt
```
打包单个目录或者多个目录：
```bash
tar -cv -f target.tar dir1 dir2 dir3
```
同时打包文件和目录：
```bash
tar -cv -f target.tar dir1 dir2 a.txt b.txt c.txt
```
<a name="BYcqH"></a>
#### 打包同时用 gzip 压缩
```bash
tar -cvz -f target.tar.gz dir1 dir2 a.txt
```
<a name="tHx92"></a>
#### 打包同时使用 bzip2 压缩
```bash
tar -cvj -f target.tar.bz dir1 dir2 a.txt
```
<a name="eBSNc"></a>
#### 打包同时使用 xz 压缩
```bash
tar -cvJ -f target.tar.xz dir1 dir2 a.txt
```
<a name="Nma8a"></a>
#### 查看
```bash
tar -tvz -f target.tar.gz
```
或者
```bash
tar -tvJ -f target.tar.xz
```
<a name="LeYOh"></a>
#### 解包和解压缩
```bash
tar -xvz -f target.tar.gz
```
上述命令将会在当前目录下解压缩，也就是 target.tar.gz 里面的文件会解压缩到当前目录下。<br />如果要解压缩到指定目录怎么做呢？<br />使用 `-C` 参数：
```bash
tar -xvz -f target.tar.gz -C ~/test
```
上述命令，会将 target.tar.gz 里面的所有文件解压到 ~/test 目录下。<br />总结下：tar 命令可以将任意的文件和目录打包成一个单一文件，解压缩时，默认是将 tar ball 里面的所有文件倾倒在当前目录，如果需要倾倒到其它目录，使用 `-C` 参数。
<a name="g2FQ3"></a>
## gzip命令
<a name="HnBeM"></a>
### 命令简介
gzip 命令用来压缩文件，gzip，gunzip命令用于压缩或扩展 GNU GZIP 格式的文件。<br />如果压缩文件名对其文件系统而言太长，则gzip会将其截断。默认情况下，gzip 会将原始文件名和时间戳等信息保留在新产生的压缩文件中。<br />gzip 会就地压缩，也就是源文件会被压缩文件替代。
<a name="zWTQt"></a>
### 语法格式
```bash
gzip [ OPTIONS ]  [ name ]
gunzip [ OPTIONS ] [ name ]
```
<a name="4p2tv"></a>
### 选项说明
```bash
-a  #使用ASCII文字模式
-c  #将输出写入到标准输出，保持原始文件不变。
-d  #解压缩文件
-f  #强制压缩文件
-h  #显示在线帮助信息
-l  #列出压缩文件的相关信息
-L  #显示版本与版权信息
-n  #压缩文件时，不保留原来的文件名称及时间戳等属性信息
-N  #压缩文件时，保留原来的文件名称及时间戳属性信息
-q  #不显示警告信息
-r  #递归处理，将指定目录下的所有文件及子目录一并处理
-t  #测试压缩文件是否正确无误
-v  #显示命令执行过程信息
-V  #显示版本信息
-<压缩效率>  #压缩效率是一个介于1~9的数值，默认值为“6”，指定的值越高，压缩率就越高
```
<a name="a9f5af37"></a>
### 应用举例
把当前目录所有的文件压缩成.gz包
```bash
[root@centos7 testdir]# gzip *
[root@centos7 testdir]# ll
total 24
-rw-r--r-- 1 root root 59 Jan 16 12:15 cest.txt.gz
-rw-r--r-- 1 root root 57 Jan 16 12:12 cuttest.txt.gz
-rw-r--r-- 1 root root 81 Jan 16 09:55 mingongge1.txt.gz
-rw-r--r-- 1 root root 51 Jan 16 03:36 mingongge2.txt.gz
-rw-r--r-- 1 root root 87 Jan 16 09:59 mingongge.txt.md5.gz
-rw-r--r-- 1 root root 65 Jan 16 10:19 sort.cut.txt.gz
```
把当前目录的每个压缩的文件解压，并列出详细的信息
```bash
[root@centos7 testdir]# gzip -dv *
cest.txt.gz:  77.8% -- replaced with cest.txt
cuttest.txt.gz:  73.5% -- replaced with cuttest.txt
mingongge1.txt.gz:  61.3% -- replaced with mingongge1.txt
mingongge2.txt.gz:  25.0% -- replaced with mingongge2.txt
mingongge.txt.md5.gz:  -4.1% -- replaced with mingongge.txt.md5
sort.cut.txt.gz:  19.0% -- replaced with sort.cut.txt
[root@centos7 testdir]# ll
total 24
-rw-r--r-- 1 root root 144 Jan 16 12:15 cest.txt
-rw-r--r-- 1 root root 102 Jan 16 12:12 cuttest.txt
-rw-r--r-- 1 root root 124 Jan 16 09:55 mingongge1.txt
-rw-r--r-- 1 root root  24 Jan 16 03:36 mingongge2.txt
-rw-r--r-- 1 root root  49 Jan 16 09:59 mingongge.txt.md5
-rw-r--r-- 1 root root  42 Jan 16 10:19 sort.cut.txt
```
详细显当前目录每个压缩的文件的信息，但不将文件解压出来
```bash
[root@centos7 testdir]# gzip -l *
compressed        uncompressed  ratio uncompressed_name
59                 144  77.8% cest.txt
57                 102  73.5% cuttest.txt
81                 124  61.3% mingongge1.txt
51                  24  25.0% mingongge2.txt
87                  49  -4.1% mingongge.txt.md5
65                  42  19.0% sort.cut.txt
400                 485  23.9% (totals)
```
和tar命令的常用用法基本一样，都属于文件压缩与解压命令。
<a name="wOB3Q"></a>
#### 压缩
```bash
gzip <file_name>
```
执行完上述命令，会在当前目录下生成 file_name.gz 文件。<br />压缩命令有个比较有用的参数：

- `-v` : 可以显示压缩后的压缩率
```bash
gzip -v <file_name>
```
<a name="P2r85"></a>
#### 解压缩
```bash
gzip -d <file_name>.gz
```
或者
```bash
gunzip <file_name>.gz
```
会在当前目录下生成 <file_name> ，源压缩文件不在了。
<a name="LnA0P"></a>
## zip命令
<a name="c2c0d185"></a>
### 命令简介
zip 命令可以用来压缩、打包文件。<br />zip 命令和 gzip bzip2 xz 不一样，它压缩后不会覆盖原有的文件。
```bash
#Debian/Ubuntu安装
apt-get install zip
#CentOS安装
[root@centos7 testdir]# zip
-bash: zip: command not found
[root@centos7 testdir]# yum install zip -y
```
zip 命令也可以用来解压缩文件，zip也是一个常用的压缩、解压应用程序，文件经它压缩后会产生一个新以.zip为扩展名的压缩包文件。
<a name="u0GrG"></a>
### 语法格式
```bash
zip [ OPTIONS ] [ NAME ..]
```
<a name="4cIWN"></a>
### 选项说明
```bash
-A  #调整可执行的自动解压缩文件
-b<工作目录>  #指定压缩文件的存放目录
-c  #给每个被压缩的文件加上注释信息
-d  #从压缩文件内删除指定的文件，也可以使用--delete参数
-D  #压缩文件内不建立目录名称
-f  #更新既有文件，将其它文件一并加入到压缩文件中
-F  #修复已损坏的压缩文件
-g  #将指定文件压缩至已存在的压缩文件中，不建立新文件
-h  #打印帮助信息
-i<范本样式>  #只压缩匹配指定条件的文件
-m  #将指定文件压缩打包后直接删除原始文件
-o  #将压缩文件的属性信息更改成与压缩文件中的最新文件的属性一致
-q  #安静模式
-r  #递归处理
-t<日期时间>  #把压缩文件的日期设成指定的日期
-T  #检查备份文件内的每个文件是否正确无误
-u  #更新较新的文件到压缩文件内
-v  #打印命令执行过程信息或版本信息
-x<范本样式>  #压缩时排除符合条件的文件
-z  #给压缩文件加上注释信息
-<压缩效率>  #指定压缩效率（1~9数字）
```
<a name="biMum"></a>
### 应用举例
在当前目录下创建压缩文件（自动创建以.zip的文件）
```bash
[root@centos7 testdir]# zip mingongge *
  adding: cest.txt.gz (stored 0%)
  adding: cuttest.txt.gz (stored 0%)
  adding: dir/ (stored 0%)
  adding: file (stored 0%)
  adding: mingongge1.txt.gz (stored 0%)
  adding: mingongge2.txt.gz (stored 0%)
  adding: mingongge.txt.md5.gz (stored 0%)
  adding: sort.cut.txt.gz (stored 0%)
```
分割一个大文件
```bash
[root@centos7 ~]# ls -lh
total 22M
-rw-------.  1 root root 1.3K Aug 20 10:39 anaconda-ks.cfg
-rw-r--r--   1 root root  140 Jan 16 11:36 dos_test.txt
drwxr-xr-x   3 root root   39 Aug 30 03:48 goinception
-rw-r--r--   1 root root  13M Aug 30 03:42 goInception-linux-amd64-v1.2.3.tar.gz
drwxr-sr-x  11 root   40 4.0K Dec 24 22:35 httpd-2.4.46
-rw-r--r--   1 root root 9.0M Aug  5 07:32 httpd-2.4.46.tar.gz
-rw-r--r--   1 root root    0 Jan 16 11:32 mingongge.file
drwxr-xr-x   3 root root  192 Jan 16 16:19 testdir
-rw-r--r--   1 root root  140 Jan 16 11:32 test.txt
[root@centos7 ~]# zip -s 4M -r mingongge.zip httpd-2.4.46.tar.gz
  adding: httpd-2.4.46.tar.gz (deflated 0%)
[root@centos7 ~]# ls -lh
total 31M
-rw-------.  1 root root 1.3K Aug 20 10:39 anaconda-ks.cfg
-rw-r--r--   1 root root  140 Jan 16 11:36 dos_test.txt
drwxr-xr-x   3 root root   39 Aug 30 03:48 goinception
-rw-r--r--   1 root root  13M Aug 30 03:42 goInception-linux-amd64-v1.2.3.tar.gz
drwxr-sr-x  11 root   40 4.0K Dec 24 22:35 httpd-2.4.46
-rw-r--r--   1 root root 9.0M Aug  5 07:32 httpd-2.4.46.tar.gz
-rw-r--r--   1 root root    0 Jan 16 11:32 mingongge.file
-rw-r--r--   1 root root 4.0M Jan 16 16:24 mingongge.z01
-rw-r--r--   1 root root 4.0M Jan 16 16:24 mingongge.z02
-rw-r--r--   1 root root 943K Jan 16 16:24 mingongge.zip
drwxr-xr-x   3 root root  192 Jan 16 16:19 testdir
#从结果可以看出会拆分成三个文件即：4M大小的mingongge.z01、4M大小的mingongge.z02和一个943k的mingongge.zip
```
<a name="L4Hol"></a>
#### 压缩单个文件
```bash
zip <compresed>.zip <file_name>
```
或者加入 `-v` 参数，可以查看压缩率：
```bash
zip -v <compresed>.zip <file_name>
```
执行上述命令，将会在当前目录下新生成 <compresed>.zip 文件，原有的 <file_name> 将继续保留。
<a name="DOgKr"></a>
#### 压缩多个文件
```bash
zip <compresed>.zip a.txt b.txt c.txt
```
会将 a.txt, b.txt, c.txt 同时压缩到 compresed.zip 文件中。
<a name="psYbe"></a>
#### 压缩目录
```bash
zip -r dir.zip <dir_name>
```
<a name="hqaQg"></a>
#### 解压缩
```bash
unzip <compresed>.zip
```

- 如果这个 .zip 文件是从多个单一文件通过 zip 命令压缩来的，那么执行 unzip 将会倾倒里面所有的文件到当前文件夹；
- 如果这个 .zip 文件是从某个目录通过 zip -r 压缩来的，那么执行 unzip 将会在当前目录下生成一个 compresed 名字的目录，目录里面的所有文件不会被倾倒在当前目录。
<a name="ImNXb"></a>
## unzip命令
<a name="aNVmR"></a>
### 命令简介
unzip 命令用于解压由zip命令压缩的压缩包文件。
```bash
#Debian/Ubuntu安装
apt-get install unzip
#CentOS安装
[root@centos7 ~]# unzip 
-bash: unzip: command not found
[root@centos7 ~]# yum install unzip -y
```
<a name="ynw7A"></a>
### 语法格式
```
unzip [ OPTIONS ] file[.zip] [file(s) ...]
```
<a name="Koouu"></a>
### 选项说明
```
-c  #将解压缩的结果输出，并对字符做适当的转换
-f  #更新现有的文件
-l  #列出压缩文件内所包含的文件
-p  #将解压缩的结果显示到屏幕上，但不执行任何的转换
-t  #检查压缩文件是否正确；
-u  #除了更新现有的文件外，也会将压缩文件中的其他文件解压缩到目录中
-v  #显示详细的信息
-z  #仅显示压缩文件的备注信息
-a  #对文本文件进行必要的字符转换
-b  #不对文本文件进行字符转换
-C  #压缩文件名称区分大小写
-j  #不处理压缩文件中原有的目录路径
-L  #将压缩文件中的全部文件名改为小写
-M  #将输出结果再交给more程序处理
-n  #解压缩时不覆盖原有的文件
-o  #unzip执行后覆盖原有的文件，不提示
-P<密码>  #使用zip的密码选项
-q  #不显示任何命令执行过程信息
-s  #将文件名中的空白字符转换为底线字符
-d<目录>  #将解压缩后存至指定的目录下
-x<文件>  #指定不要处理.zip压缩文件中的哪些文件
-Z  #unzip-Z相当于执行zipinfo命令
```
<a name="NDbs2"></a>
### 应用举例
解压一个文件
```bash
[root@centos7 testdir]# unzip mingongge.zip
```
查看一个压缩文件但不解压
```bash
[root@centos7 testdir]# unzip -v mingongge.zip 
Archive:  mingongge.zip
 Length   Method    Size  Cmpr    Date    Time   CRC-32   Name
--------  ------  ------- ---- ---------- ----- --------  ----
      59  Stored       59   0% 01-16-2021 12:15 b32621da  cest.txt.gz
      57  Stored       57   0% 01-16-2021 12:12 cbda1ce8  cuttest.txt.gz
       0  Stored        0   0% 01-16-2021 16:18 00000000  dir/
       0  Stored        0   0% 01-16-2021 16:18 00000000  file
      81  Stored       81   0% 01-16-2021 09:55 da9f2476  mingongge1.txt.gz
      51  Stored       51   0% 01-16-2021 03:36 8fdf382e  mingongge2.txt.gz
      87  Stored       87   0% 01-16-2021 09:59 982ab7bb  mingongge.txt.md5.gz
      65  Stored       65   0% 01-16-2021 10:19 17350869  sort.cut.txt.gz
--------          -------  ---                            -------
     400              400   0%                            8 files
```
指定解压后的文件存放目录
```bash
[root@centos7 testdir]# unzip -n mingongge.zip -d /tmp/
Archive:  mingongge.zip
 extracting: /tmp/cest.txt.gz        
 extracting: /tmp/cuttest.txt.gz     
   creating: /tmp/dir/
 extracting: /tmp/file               
 extracting: /tmp/mingongge1.txt.gz  
 extracting: /tmp/mingongge2.txt.gz  
 extracting: /tmp/mingongge.txt.md5.gz  
 extracting: /tmp/sort.cut.txt.gz    
[root@centos7 testdir]# ll /tmp/
total 24
-rw-r--r-- 1 root root 59 Jan 16 12:15 cest.txt.gz
-rw-r--r-- 1 root root 57 Jan 16 12:12 cuttest.txt.gz
drwxr-xr-x 2 root root  6 Jan 16 16:18 dir
-rw-r--r-- 1 root root  0 Jan 16 16:18 file
-rw-r--r-- 1 root root 81 Jan 16 09:55 mingongge1.txt.gz
-rw-r--r-- 1 root root 51 Jan 16 03:36 mingongge2.txt.gz
-rw-r--r-- 1 root root 87 Jan 16 09:59 mingongge.txt.md5.gz
-rw-r--r-- 1 root root 65 Jan 16 10:19 sort.cut.txt.gz
```
<a name="x0KNx"></a>
## bzip2 命令
默认情况下，bzip2 命令也是就地压缩，压缩后源文件将会被替代。<br />使用方法和 gzip 基本一样，但是能提供更好的压缩率，当然，付出的代价就是压缩时间更长。
<a name="db2Ml"></a>
### 压缩
```bash
bzip2 <file_name>
```
或者引入 `-v` 参数查看压缩率：
```bash
bzip2 -v <file_name>
```
将在当前目录下生成 <file_name>.bz2 文件。
<a name="LMJi1"></a>
### 解压缩
```bash
bzip2 -d <file_name>.bz2
```
或者
```bash
bunzip2 <file_name>.bz2
```
<a name="YK6Sd"></a>
## `xz` 命令
默认情况下，`xz` 命令也是就地压缩，原始文件将被覆盖。<br />使用方法和前面提到的 gzip bzip2 类似，参数也几乎是一样的。
<a name="V5a4f"></a>
### 压缩
```bash
xz <file_name>
```
或者加上 `-v` 参数查看压缩率：
```bash
xz -v <file_name>
```
压缩完后，会在当前目录下生成 `<file_name>.xz` 文件。<br />这时可以使用 `-l` 参数，来查看压缩前后的文件对比信息：
```bash
xz -l <file_name>.xz
```
举个例子如下：
```bash
Fcant@mbp:~/Downloads$ xz -l ppt模板.pptx.xz
Strms  Blocks   Compressed Uncompressed  Ratio  Check   Filename
    1       1    124.7 KiB    135.5 KiB  0.920  CRC64   ppt模板.pptx.xz
Fcant@mbp:~/Downloads$
```
<a name="jVnvL"></a>
### 解压缩
```bash
xz -d <file_name>.xz
```
注意，在 `xz` 命令这里，没有 `xunz` 这种命令。
