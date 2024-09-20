Linux touch cat more head tail cmp comm diff ln
<a name="9mexT"></a>
# 1、创建文件
<a name="touch"></a>
## touch-创建文件
touch命令有两个功能：一是创建新的空文件，二是改变已有文件的时间戳属性。<br />touch命令会根据当前的系统时间更新指定文件的访问时间和修改时间。如果文件不存在，将会创建新的空文件，除非指定了”-c”或”-h”选项。<br />注意：在修改文件的时间属性的时候，用户必须是文件的属主，或拥有写文件的访问权限。<br />语法格式：touch [参数] [文件]<br />常用参数：

| -a | 修改文件的读取时间记录 |
| --- | --- |
| -m | 修改文件的修改时间记录 |
| -r | 使用文件的时间记录，与 --file 的效果一样 |
| -c | 不创建新文件 |
| -t | [[CC]YY]MMDDhhmm[.ss]指定atime和mtime的时间戳 |

**创建一个或多个文件0字节大小**
```bash
$ touch filename1 filename2
```
<a name="DEWDe"></a>
# 2、查看文件
<a name="cat"></a>
## cat
用于链接文件并打印到标准输出设备上。这个文本输出命令可以用来查看文件内容，创建内容等。命令格式 : cat [选项]... [文件]...
<a name="Bs2zq"></a>
### cat -n 查看并显示行号
```bash
➜  /local ll
total 52K
drwxr-xr-x  2 root root  4.0K May 31 10:22 bin
-rw-r--r--  1 7161 31415  18K Oct  4  2018 COPYING
drwxr-xr-x  2 root root  4.0K May 31 10:22 docs
drwxr-xr-x  3 root root  4.0K May 31 10:21 include
drwxr-xr-x  5 root root  4.0K May 31 10:22 lib
drwxr-xr-x  4 root root  4.0K May 31 10:22 man
-rw-r--r--  1 7161 31415 2.5K Oct  4  2018 README
drwxr-xr-x 28 root root  4.0K May 31 10:22 share
drwxr-xr-x  2 root root  4.0K May 31 10:22 support-files
➜  /local cat -n README
     1  MySQL Server 5.7
     2
     3  This is a release of MySQL, a dual-license SQL database server.
     4  For the avoidance of doubt, this particular copy of the software
     5  is released under the version 2 of the GNU General Public License.
     6  MySQL is brought to you by Oracle.
     7
     8  Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved.
     9
    10  License information can be found in the COPYING file.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594542502975-1c6d0c1d-827d-4839-87ee-4c109092fa38.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2067037&status=done&style=none&width=1107.6666666666667)
<a name="HFGwp"></a>
### cat 创建一个空文件 b
```bash
➜  /local ll
total 52K
drwxr-xr-x  2 root root  4.0K May 31 10:22 bin
-rw-r--r--  1 7161 31415  18K Oct  4  2018 COPYING
drwxr-xr-x  2 root root  4.0K May 31 10:22 docs
drwxr-xr-x  3 root root  4.0K May 31 10:21 include
drwxr-xr-x  5 root root  4.0K May 31 10:22 lib
drwxr-xr-x  4 root root  4.0K May 31 10:22 man
-rw-r--r--  1 7161 31415 2.5K Oct  4  2018 README
drwxr-xr-x 28 root root  4.0K May 31 10:22 share
drwxr-xr-x  2 root root  4.0K May 31 10:22 support-files
➜  /local cat >b <<EOF
heredoc> 1
heredoc> exit
heredoc> wq
heredoc> <EOF
heredoc> >EOF
heredoc> EOF
➜  /local ll
total 56K
-rw-r--r--  1 root root    20 Jul 12 16:29 b
drwxr-xr-x  2 root root  4.0K May 31 10:22 bin
-rw-r--r--  1 7161 31415  18K Oct  4  2018 COPYING
drwxr-xr-x  2 root root  4.0K May 31 10:22 docs
drwxr-xr-x  3 root root  4.0K May 31 10:21 include
drwxr-xr-x  5 root root  4.0K May 31 10:22 lib
drwxr-xr-x  4 root root  4.0K May 31 10:22 man
-rw-r--r--  1 7161 31415 2.5K Oct  4  2018 README
drwxr-xr-x 28 root root  4.0K May 31 10:22 share
drwxr-xr-x  2 root root  4.0K May 31 10:22 support-files
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594542694997-817d7c90-4f00-4d5b-afc4-c87b4c9ec63a.png#align=left&display=inline&height=512&originHeight=1537&originWidth=1949&size=1160207&status=done&style=none&width=649.6666666666666)
<a name="qTwGa"></a>
### cat 清空文件内容/cat 写入内容(如果原来有内容将被覆盖)
```bash
➜  /local cat -n b
     1  1
     2  exit
     3  wq
     4  <EOF
     5  >EOF
➜  /local cat >b <<EOF
heredoc> EOF
➜  /local cat -n b
➜  /local
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594542780779-b778a9ab-a8d2-4264-a0d7-26fdcc96ed2a.png#align=left&display=inline&height=186&originHeight=557&originWidth=775&size=165347&status=done&style=none&width=258.3333333333333)
<a name="jZLG5"></a>
### cat 文件内容写入>目标文件
```bash
➜  /local cat a
I'm a File.EOF
➜  /local cat b
I'm b File
➜  /local cat a > b
➜  /local cat b
I'm a File.EOF
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594543353098-b3d44f36-1d0f-4155-a74a-a9d3dd36983d.png#align=left&display=inline&height=151&originHeight=452&originWidth=1304&size=224227&status=done&style=none&width=434.6666666666667)
<a name="XxNo8"></a>
### cat 文件内容追加至>>目标文件
```bash
➜  /local cat >b <<EOF
heredoc> I'm b File.
heredoc> EOF
➜  /local cat a
I'm a File.EOF
➜  /local cat a >> b
➜  /local cat b
I'm b File.
I'm a File.EOF
➜  /local cat a
I'm a File.EOF
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594543166395-877b8f4f-9621-479d-9ed3-6fc37723a667.png#align=left&display=inline&height=221&originHeight=664&originWidth=1462&size=366247&status=done&style=none&width=487.3333333333333)
<a name="TjX3a"></a>
## more-查看文件所有的内容,可以分屏显示
```bash
$ more filename1
```
<a name="lpOWQ"></a>
## head-默认查看文件前10行
```bash
$ head filename1
# 查看指定的前n行
$ head -n 行数 文件名
```
<a name="Qrvgu"></a>
## tail-默认查看文件后10行
```bash
$ tail filename1
# 查看指定的后n行
$ tail -n 行数 文件名

```
<a name="xbI2b"></a>
### 应用场景
常用于监听远程服务器项目写入日志记录，监听日志文件的写入内容
```bash
➜  ~ tail --help
Usage: tail [OPTION]... [FILE]...
Print the last 10 lines of each FILE to standard output.
With more than one FILE, precede each with a header giving the file name.
With no FILE, or when FILE is -, read standard input.

Mandatory arguments to long options are mandatory for short options too.
  -c, --bytes=K            output the last K bytes; or use -c +K to output
                             bytes starting with the Kth of each file
  -f, --follow[={name|descriptor}]
                           output appended data as the file grows;实时输出文件的追加内容
                             an absent option argument means 'descriptor'
  -F                       same as --follow=name --retry
  -n, --lines=K            output the last K lines, instead of the last 10;输出最后K行，而不是最后10行
                             or use -n +K to output starting with the Kth
      --max-unchanged-stats=N
                           with --follow=name, reopen a FILE which has not
                             changed size after N (default 5) iterations
                             to see if it has been unlinked or renamed
                             (this is the usual case of rotated log files);
                             with inotify, this option is rarely useful
      --pid=PID            with -f, terminate after process ID, PID dies
  -q, --quiet, --silent    never output headers giving file names
      --retry              keep trying to open a file if it is inaccessible
  -s, --sleep-interval=N   with -f, sleep for approximately N seconds
                             (default 1.0) between iterations;
                             with inotify and --pid=P, check process P at
                             least once every N seconds
  -v, --verbose            always output headers giving file names
      --help     display this help and exit
      --version  output version information and exit

If the first character of K (the number of bytes or lines) is a '+',
print beginning with the Kth item from the start of each file, otherwise,
print the last K items in the file.  K may have a multiplier suffix:
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,
GB 1000*1000*1000, G 1024*1024*1024, and so on for T, P, E, Z, Y.

With --follow (-f), tail defaults to following the file descriptor, which
means that even if a tail'ed file is renamed, tail will continue to track
its end.  This default behavior is not desirable when you really want to
track the actual name of the file, not the file descriptor (e.g., log
rotation).  Use --follow=name in that case.  That causes tail to track the
named file in a way that accommodates renaming, removal and creation.

GNU coreutils online help: <http://www.gnu.org/software/coreutils/>
For complete documentation, run: info coreutils 'tail invocation'
```
<a name="YXhxk"></a>
#### 最常用的 `tail -f` 
```bash
tail -300f shopbase.log #倒数300行并进入实时监听文件写入模式
```
<a name="less"></a>
## less-查看文件所有内容
-n 显示行号<br />PageUp 向前翻页<br />PageDown 向后翻页<br />q:退出
```bash
$ less filename1
```
<a name="Gd9aA"></a>
## rev–颠倒过来显示文件内容
使用rev命令可以把每一行字符的顺序颠倒过来显示文件内容。全称为“REVerse”。<br />语法格式：rev [文件]
<a name="yGzS4"></a>
## pr–将文本文件转换成适合打印的格式
pr命令英文全称为pre，用来将文本文件转换成适合打印的格式，它可以把较大的文件分割成多个页面进行打印，并为每个页面添加标题。<br />语法格式：pr [参数]<br />常用参数：

| -h | 为页指定标题 |
| --- | --- |

<a name="ugQgi"></a>
# 3、文件比较
<a name="EULft"></a>
## cmp-比较两个文件是否有差异
cmp命令有用于比较两个任意类型的文件，是英文单词“compare”的缩写。若两个文件相同则默认不输出任何信息，若两个文件不同，则会输出第一个不同之处的字符和列数编号，结果输出到标准输出。<br />若不指定任何文件名称或是所给予的文件名为”-“，则cmp命令会从标准输入设备读取数据。<br />语法格式：cmp [参数] [文件] [文件]<br />常用参数：

| --help | 显示帮助 |
| --- | --- |
| -v/--version | 显示版本信息 |
| -s/--quit/--silent | 不显示错误信息 |
| -l/--verbose | 标示出所有不一样的地方 |
| -i/--ignore-initial | 指定一个数目 |

<a name="XVsnh"></a>
## comm – 比较两个已排过序的文件
是英文单词“common”的缩写。comm命令会一列列地比较两个已排序文件的差异，并将其结果显示出来，如果没有指定任何参数，则会把结果分成3行显示：第1行仅是在第1个文件中出现过的列，第2行是仅在第2个文件中出现过的列，第3行则是在第1与第2个文件里都出现过的列。若给予的文件名称为”-“，则comm指令会从标准输入设备读取数据。<br />语法格式：[参数] [文件1][文件2]

| -1  | 不显示只在第1个文件里出现过的列 |
| --- | --- |
| -2 | 不显示只在第2个文件里出现过的列 |
| -3 | 不显示只在第1和第2个文件里出现过的列 |
| –help | 在线帮助 |
| –version | 显示版本信息 |

<a name="bocub"></a>
## diff – 比较文件的差异
diff命令是单词“Difference”的缩写。diff以逐行的方式，比较文本文件的异同处。如果指定要比较目录，则diff会比较目录中相同文件名的文件，但不会比较其中子目录 。<br />语法格式：diff [参数] [目录]<br />常用参数：

| -a | diff预设只会逐行比较文本文件 |
| --- | --- |
| -b | 不检查空格字符的不同 |
| -W | 在使用-y参数时，指定栏宽 |
| -x | 不比较选项中所指定的文件或目录 |

<a name="2yD6c"></a>
# 4、可执行文件创建链接
<a name="htdX9"></a>
## ln–为文件创建链接
ln命令是linux系统中一个非常重要命令，英文全称是“link”，即链接的意思，它的功能是为某一个文件在另外一个位置建立一个同步的链接。一种是hard link，又称为硬链接；另一种是symbolic link，又称为符号链接。<br />通俗一点理解，可以把硬链接当成源文件的副本，他和源文件一样的大小，但是事实上却不占任何空间。符号链接可以理解为类似windows一样的快捷方式。
<a name="Kvlve"></a>
### 语法格式
ln [参数] [源文件或目录] [目标文件或目录]
<a name="SWRGX"></a>
### 常用参数
| -b | 为每个已存在的目标文件创建备份文件 |
| --- | --- |
| -d | 此选项允许“root”用户建立目录的硬链接 |
| -f | 强制创建链接，即使目标文件已经存在 |
| -n | 把指向目录的符号链接视为一个普通文件 |

<a name="NZbK0"></a>
### 符号链接 （软链接）

- 符号链接以路径的形式存在，类似于Windows操作系统中的快捷方式。
- 符号链接可以跨文件系统 ，硬链接不可以。
- 符号链接可以对一个不存在的文件名进行链接，硬链接不可以。
- 符号链接可以对目录进行链接，硬链接不可以。
- 一个符号链接指向另一个文件
- IS-l的显示链接的名称和引用的文件
- 一个符号链接的内容是它引用文件的名称
- 可以对目标进行
- 可以跨分区
- 指向的是另一个文件的路径；其大小为指向的路径字符串的长度，不增加或减少目标文件inode的引用计数；

语法：<br />In-s filename [linkname]
<a name="0xuEx"></a>
### 硬链接

- 硬链接以文件副本的形式存在，但不占用实际空间。
- 硬链接不允许给目录创建硬链接。
- 硬链接只有在同一个文件系统中才能创建。
- 创建硬链接会增加额外的记录项以引用文件
- 对应于同一文件系统上一个物理文件
- 每个目录引用相同的inode号
- 创建时链接数递增
- 删除文件时：
- rm命令递减计数的链接
- 文件要存在，至少有一个链接数
- 当链接数为零时，该文件被删除
- 不能跨越驱动器或分区

语法格式：<br />In filename [linkname]**
<a name="4H4TB"></a>
### **硬链接和软链接的区别：**
**硬链接**：对一个文件起多个名字<br />**软链接**：原始文件一般路径用相对路径，相对路径一定相对于软链接文件的路径<br />（1） 本质：硬链接是一个文件多个名字，而软链接一个文件对应多个快捷方式。<br />（2） 跨分区：硬链接不可以跨分区，软链接可以跨分区。<br />（3） 链接数：硬链接会增加链接数，软链接不增加。<br />（4） 节点编号：硬链接相同，软链接不相同。<br />（5） 删掉原始文件是否会影响到链接文件？硬链接各个文件时平级的关系，该访问还能访问，而软链接是一种依赖关系，删除原始文件后链接会失效。<br />（6） 目录：硬链接不支持目录，软链接支持目录。
