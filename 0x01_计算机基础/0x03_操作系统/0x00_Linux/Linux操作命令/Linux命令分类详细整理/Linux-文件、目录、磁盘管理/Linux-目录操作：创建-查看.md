Linux mkdir rmdir pwd find mv cp cpio dd rm ls
<a name="YmCVO"></a>
# 目录操作
| **作用** | **改变目录/CD和目录名直接需要空格** |
| --- | --- |
| / | 切换到根目录 |
| . | 当前目录 |
| .. | 上一级目录 |
| ~ | 返回当前用户主目录 |
| - | 返回上一次目录 |

<a name="be749cba"></a>
## mkdir-创建目录
mkdir命令是“make directories”的缩写，用来创建目录。<br />注意：默认状态下，如果要创建的目录已经存在，则提示已存在，而不会继续创建目录。所以在创建目录时，应保证新建的目录与它所在目录下的文件没有重名。mkdir命令还可以同时创建多个目录。<br />语法格式 : mkdir [参数] [目录]<br />常用参数：

| -p | 递归创建多级目录 |
| --- | --- |
| -m | 建立目录的同时设置目录的权限 |
| -z | 设置安全上下文 |
| -v | 显示目录的创建过程 |


| mkdir 目录名称 | 创建一个目录 |
| --- | --- |
| mkdir -p | 创建多级目录 |

```bash
$ mkdir -p /父目录名称/子目录名称
```
> -p，如果父目录不存在会先创建父目录再创建子目录

<a name="cPYDt"></a>
## rmdir-删除空目录
rmdir命令作用是删除空的目录，英文全称：“removedirectory”。<br />注意：rmdir命令只能删除空目录。当要删除非空目录时，就要使用带有“-R”选项的rm命令。<br />rmdir命令的“-p”参数可以递归删除指定的多级目录，但是要求每个目录也必须是空目录。<br />语法格式 :  rmdir[参数] [目录名称]<br />常用参数：

| -p | 用递归的方式删除指定的目录路径中的所有父级目录，非空则报错 |
| --- | --- |
| -- -- ignore-fail-on-non-empty | 忽略由于删除非空目录时导致命令出错而产生的错误信息 |
| -v | 显示命令的详细执行过程 |
| -- -- help | 显示命令的帮助信息 |
| -- -- version | 显示命令的版本信息 |

<a name="2ktaG"></a>
## pwd-查看当前路径
pwd命令是“print workingdirectory”中每个单词的首字母缩写，其功能正如所示单词一样，为打印工作目录，即显示当前工作目录的绝对路径。<br />在实际工作中，经常会在不同目录之间进行切换，为了防止“迷路”，可以使用pwd命令快速查看当前所在的目录路径。<br />语法格式: pwd [参数]<br />常用参数：

| -L | 显示逻辑路径 |
| --- | --- |

```bash
[root@iZuligp6e1dyzfZ ~]# pwd
/root
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596077301843-8176ff73-7259-4654-a9f1-088f94b361ff.png#averageHue=%234d4d4d&height=213&id=DVVaP&originHeight=639&originWidth=1824&originalType=binary&ratio=1&rotation=0&showTitle=false&size=362717&status=done&style=none&title=&width=608)
<a name="MN8cV"></a>
## find-搜索查找
| find | [目录名称] [-name '查询字符串'] |
| --- | --- |
| 无参数 | 搜索当前目录和它的子目录所有文件和目录 |
| 目录名称 | 搜索指定目录下和他的子目录所有的文件和目录 |
| -name '查询字符串' | 指定具体查询字符串 |
| <br /> | *匹配多个字符串 |
| <br /> | ?匹配一个字符 |

<a name="c8ce4b36"></a>
## mv-重命名、移动
mv命令是“move”单词的缩写，其功能大致和英文含义一样，可以移动文件或对其改名。<br />这是一个使用频率超高的文件管理命令，需要特别留意它与复制的区别：mv与cp的结果不同。mv命令好像文件“搬家”，文件名称发生改变，但个数并未增加。而cp命令是对文件进行复制操作，文件个数是有增加的。<br />语法格式：mv [参数]<br />常用参数：<br />-i：交互式<br />-f：强制<br />-b：目标存在，覆盖前先备份

| -i | 若存在同名文件，则向用户询问是否覆盖 |
| --- | --- |
| -f | 覆盖已有文件时，不进行任何提示 |
| -b | 当文件存在时，覆盖前为其创建一个备份 |
| -u | 当源文件比目标文件新，或者目标文件不存在时，才执行移动此操作 |


| move=mv |
| --- |
| mv 旧名称 新名称 |
| mv既可以改文件也可以改目录 |


| mv 源目录 目标目录 |
| --- |
| mv将源目录移动到目标目录下 |

<a name="5F8Rd"></a>
## cp-复制
| cp [参数] 源文件或目录 目标目录 将源文件或整个目录复制到目标目录下 |
| --- |
| -r recursion连同子目录下所有文件全部复制 |

cp命令可以理解为英文单词copy的缩写，其功能为复制文件或目录。<br />cp命令可以将多个文件复制到一个具体的文件名或一个已经存在的目录下，也可以同时复制多个文件到一个指定的目录中。<br />语法格式：cp [参数] [文件]<br />常用参数：<br />-i：覆盖前提<br />-n：不覆盖，注意两者顺序<br />-r，-R：递归复制目录及内部的所有内容<br />-a：归档，相当于-dR –preserv=all<br />-d:--no-dereference—preserv=links不复制原文件，只复制链接名<br />-preserv[=ATTR_LIST]<br />mode:权限<br />ownership：属主属组<br />-p:等同—preserv=mode,ownership,timestamp<br />-v:--verbose<br />-f:--force<br />-u:--update只复制源比目标更新文件或者目标不存在的文件<br />-b：目标存在，覆盖前先备份<br />--backup=numbered目标存在，覆盖前先备份加数字后缀

| -f | 若目标文件已存在，则会直接覆盖原文件 |
| --- | --- |
| -i | 若目标文件已存在，则会询问是否覆盖 |
| -p | 保留源文件或目录的所有属性、等同—preserv=mode,ownership,timestamp |
| -r/-R | 递归复制文件和目录 |
| -a | 归档，相当于-dR –preserv=all |

<a name="1bzAJ"></a>
## cpio–复制文件到归档包
cpio命令全称为“CoPyIn and Out”。主要是用来建立或者还原备份档的工具程序，cpio命令可以复制文件到归档包中，或者从归档包中复制文件。<br />语法格式: cpio [参数]<br />常用参数：

| -0 | 接受新增列控制字符，通常配合find指令的“-print0”参数使用 |
| --- | --- |
| -a | 重新设置文件的存取时间 |
| -A | 附加到已存在的备份文档中，且这个备份文档必须存放在磁盘上，而不能放置于磁带机里 |
| -B | 将输入/输出的区块大小改成5210Bytes |
| -c | 使用旧ASCII备份格式 |

<a name="JSRkj"></a>
## dd–拷贝文件及转换
全称为“Disk Dump”。dd命令用指定大小的块拷贝一个文件，并在拷贝的同时进行指定的转换。<br />注意：指定数字的地方若以下列字符结尾则乘以相应的数字：b=512；c=1；k=1024；w=2<br />语法格式：dd [参数]<br />常用参数：

| -v | 显示版本信息 |
| --- | --- |
| -h | 显示帮助信息 |

<a name="ntR7c"></a>
## rm-删除移除文件或目录
rm是常用的命令，全称为“Remove”。该命令的功能为删除一个目录中的一个或多个文件或目录，它也可以将某个目录及其下的所有文件及子目录均删除。对于链接文件，只是删除了链接，原有文件均保持不变。rm也是一个危险的命令，使用的时候要特别当心，尤其对于新手，否则整个系统就会毁在这个命令（比如在/（根目录）下执行rm * -rf）。所以，在执行rm之前最好先确认一下在哪个目录，到底要删除什么东西，操作时保持高度清醒的头脑。<br />语法格式：rm [参数] [文件]<br />常用参数：

| -f | 忽略不存在的文件，不会出现警告信息 |
| --- | --- |
| -i | 删除前会询问用户是否操作 |
| -r/R | 递归删除 |
| -v | 显示指令的详细执行过程 |


> 注意：千万不要rm -rf /*  会导致删除所有的文件或目录，导致系统崩溃

- 查看帮助文档
   - 命令  --help  查看英文文档手册，使用q退出
   - man 命令     查看中文文档手册【推荐方式】，使用q退出
<a name="9aOoT"></a>
## 查看目录文件-ls
命令格式:  `ls [OPTION] [FILE]`
```bash
➜  / ls
bin  boot  dev  etc  home  lib  lib64  local  media  mnt  my  opt  proc  root  run  sbin  srv  sys  tmp  usr  var
➜  /
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594540691716-48454813-153f-4ad3-9dae-2d4bdaff0e59.png#averageHue=%23747474&height=74&id=it9EE&originHeight=223&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=178724&status=done&style=none&title=&width=1107.6666666666667)
<a name="ed37251f"></a>
### 加[option]-l 它展示了权限，属主，属组，大小，时间，文件名称
```bash
➜  / ls -l
total 60
lrwxrwxrwx.  1 root root     7 Aug 18  2017 bin -> usr/bin
dr-xr-xr-x.  4 root root  4096 Sep 12  2017 boot
drwxr-xr-x  19 root root  2960 Aug 14  2019 dev
drwxr-xr-x. 86 root root 12288 May 31 15:40 etc
drwxr-xr-x.  3 root root  4096 Nov 17  2019 home
lrwxrwxrwx.  1 root root     7 Aug 18  2017 lib -> usr/lib
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594540747656-0558a63e-6a7a-49bd-9907-f3587214b524.png#averageHue=%23404040&height=450&id=Qcexe&originHeight=1349&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1635966&status=done&style=none&title=&width=1107.6666666666667)
<a name="GmIf9"></a>
### ls -a (all)   linux  '.'（点）开头的文件默认是隐藏的，-a可以查询所有的文件。
```bash
➜  / ls -a
.   .autorelabel  boot  etc   lib    local  mnt  opt   root  sbin  sys  usr
..  bin           dev   home  lib64  media  my   proc  run   srv   tmp  var
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594540980244-9e904ce7-3967-41af-b134-c5010ff342e6.png#averageHue=%232e2e2e&height=115&id=vsnWy&originHeight=346&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=443923&status=done&style=none&title=&width=1107.6666666666667)
<a name="oWPqB"></a>
### ls -F 以"/"结尾的方式展示文件夹。
```bash
➜  / ls -F
bin@   dev/  home/  lib64@  media/  my/   proc/  run/   srv/  tmp/  var/
boot/  etc/  lib@   local/  mnt/    opt/  root/  sbin@  sys/  usr/
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541072296-5537fd20-bf0a-40e7-b5df-054227c05c4a.png#averageHue=%232e2e2e&height=57&id=tZ1kU&originHeight=171&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=220363&status=done&style=none&title=&width=1107.6666666666667)
<a name="df0ee3bb"></a>
### ls -r 倒叙展示，默认的顺序是(a,b,c,d)正序这样展示
```bash
➜  / ls -r
var  usr  tmp  sys  srv  sbin  run  root  proc  opt  my  mnt  media  local  lib64  lib  home  etc  dev  boot  bin
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541239223-f2ce8ebe-3f49-4eef-b45b-4c5393c6b045.png#averageHue=%232f2f2f&height=153&id=usKSA&originHeight=458&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=547055&status=done&style=none&title=&width=1107.6666666666667)
<a name="352ece36"></a>
### ls -R ()递归展示子目录
```bash
➜  / cd tmp
➜  /tmp ls
systemd-private-76b13ef273274b3ea1f911a228dabaf4-ntpd.service-lh3szC
➜  /tmp ls -R
.:
systemd-private-76b13ef273274b3ea1f911a228dabaf4-ntpd.service-lh3szC

./systemd-private-76b13ef273274b3ea1f911a228dabaf4-ntpd.service-lh3szC:
tmp

./systemd-private-76b13ef273274b3ea1f911a228dabaf4-ntpd.service-lh3szC/tmp:
➜  /tmp
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541380077-a6787078-3055-40a2-bbc3-24148a2ed205.png#averageHue=%23323232&height=259&id=U6fEm&originHeight=777&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=950183&status=done&style=none&title=&width=1107.6666666666667)
<a name="ada91e1f"></a>
### ls -lS 按照文件大小降序显示
```bash
➜  / ls -lS
total 60
drwxr-xr-x. 86 root root 12288 May 31 15:40 etc
dr-xr-xr-x.  4 root root  4096 Sep 12  2017 boot
drwxr-xr-x.  3 root root  4096 Nov 17  2019 home
drwxr-xr-x   9 root root  4096 May 31 10:22 local
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541456225-3e86f9f1-9653-49de-b20c-33df78e18dad.png#averageHue=%23363636&height=441&id=bwTcT&originHeight=1322&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1673910&status=done&style=none&title=&width=1107.6666666666667)
<a name="afde46c1"></a>
### ls -g 不输出所有者(属主)信息
```bash
➜  / ls -lS
total 60
drwxr-xr-x. 86 root root 12288 May 31 15:40 etc
dr-xr-xr-x.  4 root root  4096 Sep 12  2017 boot
drwxr-xr-x.  3 root root  4096 Nov 17  2019 home
drwxr-xr-x   9 root root  4096 May 31 10:22 local
dr-xr-xr-x  13 root root     0 Aug 15  2019 sys
➜  / ls -g
total 60
lrwxrwxrwx.  1 root     7 Aug 18  2017 bin -> usr/bin
dr-xr-xr-x.  4 root  4096 Sep 12  2017 boot
drwxr-xr-x  19 root  2960 Aug 14  2019 dev
drwxr-xr-x. 86 root 12288 May 31 15:40 etc
drwxr-xr-x.  3 root  4096 Nov 17  2019 home
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541578204-f6f1428e-16d9-404e-832e-537a29850464.png#averageHue=%233b3b3b&height=637&id=eeDIt&originHeight=1912&originWidth=1980&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1470901&status=done&style=none&title=&width=660)
<a name="R04CZ"></a>
### ls -lG 隐藏所有组（属组）信息
```bash
➜  / ls -lS
total 60
drwxr-xr-x. 86 root root 12288 May 31 15:40 etc
dr-xr-xr-x.  4 root root  4096 Sep 12  2017 boot
drwxr-xr-x.  3 root root  4096 Nov 17  2019 home
drwxr-xr-x   9 root root  4096 May 31 10:22 local
drwxr-xr-x.  2 root root  4096 Nov  5  2016 media
➜  / ls -lG
total 60
lrwxrwxrwx.  1 root     7 Aug 18  2017 bin -> usr/bin
dr-xr-xr-x.  4 root  4096 Sep 12  2017 boot
drwxr-xr-x  19 root  2960 Aug 14  2019 dev
drwxr-xr-x. 86 root 12288 May 31 15:40 etc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541647064-5238409e-a73f-4bbf-bc71-37e4f461e1c8.png#averageHue=%23444444&height=583&id=x5A15&originHeight=1750&originWidth=1678&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1084401&status=done&style=none&title=&width=559.3333333333334)
<a name="eOuBU"></a>
### ls -li 显示文件的索引号
```bash
➜  / ls -li
total 60
     17 lrwxrwxrwx.  1 root root     7 Aug 18  2017 bin -> usr/bin
 131074 dr-xr-xr-x.  4 root root  4096 Sep 12  2017 boot
      3 drwxr-xr-x  19 root root  2960 Aug 14  2019 dev
1179649 drwxr-xr-x. 86 root root 12288 May 31 15:40 etc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541727609-4b4f4cd2-2020-47a8-a2c5-637c31483b62.png#averageHue=%23373737&height=441&id=hUmNm&originHeight=1322&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1645665&status=done&style=none&title=&width=1107.6666666666667)
<a name="THJVf"></a>
### ls -lt 按照修改时间排序（倒序）依次创建文件 a b
```bash
➜  / ls -lt
total 60
dr-xr-x---. 10 root root  4096 Jul 12 16:16 root
drwxrwxrwt.  8 root root  4096 Jul 12 09:03 tmp
drwxr-xr-x. 86 root root 12288 May 31 15:40 etc
drwxr-xr-x  25 root root   800 May 31 15:38 run
drwxr-xr-x   9 root root  4096 May 31 10:22 local
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541824023-459a0720-9d60-4bea-a6ba-7ca35ecb7fc3.png#averageHue=%233b3b3b&height=439&id=H2GIb&originHeight=1317&originWidth=2039&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1017722&status=done&style=none&title=&width=679.6666666666666)
<a name="bc2371ce"></a>
### ls --version
```bash
➜  / ls --version
ls (GNU coreutils) 8.22
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by Richard M. Stallman and David MacKenzie.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594541913017-dd8a81e6-5294-40b8-9891-58d09ffc2cf2.png#averageHue=%23393939&height=222&id=glAUU&originHeight=667&originWidth=2180&originalType=binary&ratio=1&rotation=0&showTitle=false&size=526149&status=done&style=none&title=&width=726.6666666666666)
