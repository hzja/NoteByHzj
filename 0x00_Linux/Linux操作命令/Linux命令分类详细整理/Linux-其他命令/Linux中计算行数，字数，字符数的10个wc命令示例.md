Linux<br />wc命令的功能为统计指定文件中的字节数、字数、行数，并将统计结果显示输出。
```bash
# wc [options] filenames
```
以下是该命令提供的选项和用法。
```
-c, --bytes
          输出目标文件中字节的计数结果

-m, --chars
          输出目标文件的中字符的计数结果

-l, --lines
          输出目标文件中 行 的计数结果

--files0-from=F
          从NUL-terminated指明的名字在文件F中的文件中读取，如果F是 - 则从标准输入读取

-L, --max-line-length
          输出目标文件中最大行的长度

-w, --words
          输出目标文件中单词的计数结果
```
测试文件
```bash
[root@rumenz ~]# cat rumenz.txt

Red Hat
CentOS
Fedora
Debian
Scientific Linux
OpenSuse
Ubuntu
Xubuntu
Linux Mint
Pearl Linux
Slackware
Mandriva
```
<a name="nCgNT"></a>
### 1、wc 命令的基本示例
这 wc 不传递任何参数的命令将显示 “的基本结果”rumenz.txt 文件。下面显示的三个数字是12(number of lines)，16(number of words） 和112(number of bytes) 的文件。
```bash
[root@rumenz ~]# wc rumenz.txt

12  16 112 rumenz.txt
```
<a name="OUlRd"></a>
### 2、计算行数
要计算文件中的换行数，请使用选项 `-l`，它打印给定文件中的行数。假设，以下命令将显示文件中的换行数。在输出中，第一个字段指定为计数，第二个字段是文件名。
```bash
[root@rumenz ~]# wc -l rumenz.txt

12 rumenz.txt
```
<a name="E9L1u"></a>
### 3、显示字数
使用 `-w`与wc 命令打印文件中的单词数。键入以下命令以计算文件中的字数。
```bash
[root@rumenz ~]# wc -w rumenz.txt

16 rumenz.txt
```
<a name="L0n3O"></a>
### 4、计算字节数和字符数
使用选项时`-c`和`-m`和wc 命令将打印总数number of bytes和characters分别在一个文件中。
```bash
[root@rumenz ~]# wc -c rumenz.txt

112 rumenz.txt
```
```bash
[root@rumenz ~]# wc -m rumenz.txt

112 rumenz.txt
```
<a name="aLAsa"></a>
### 5、统计最长行的长度
这 wc命令允许一个参数`-L`，可以用来打印最长行（number of characters) 的长度。所以有最长的字符行（Scientific Linux) 在一个文件中。
```bash
[root@rumenz ~]# wc -L rumenz.txt

16 rumenz.txt
```
<a name="wWPwE"></a>
### 6、检查更多 WC 选项
有关更多信息和帮助wc命令，简单地运行wc –help要么man wc 从命令行。
```bash
[root@rumenz ~]# wc --help

Usage: wc [OPTION]... [FILE]...
  or:  wc [OPTION]... --files0-from=F
Print newline, word, and byte counts for each FILE, and a total line if
more than one FILE is specified.  With no FILE, or when FILE is -,
read standard input.
  -c, --bytes            print the byte counts
  -m, --chars            print the character counts
  -l, --lines            print the newline counts
  -L, --max-line-length  print the length of the longest line
  -w, --words            print the word counts
      --help   display this help and exit
      --version   output version information and exit

Report wc bugs to bug-coreutils@gnu.org
GNU coreutils home page: <http://www.gnu.org/software/coreutils/>
General help using GNU software: <http://www.gnu.org/gethelp/>
For complete documentation, run: info coreutils 'wc invocation'
```
<a name="LJBLH"></a>
### 7、统计文件夹中文件个数
```bash
> ls -l | grep "^-" |wc -l
23
```
<a name="use2e"></a>
### 8、统计文件夹中目录个数
```bash
> ls -l | grep "^d" | wc -l
6
```
<a name="w7mn5"></a>
### 9、统计文件夹下文件个数，包括子文件
```bash
> ls -lR | grep "^-" |wc -l
13089
```
<a name="NYMdo"></a>
### 10、统计文件夹下目录个数，包括子目录
```bash
> ls -lR | grep "^d" | wc -l
1323
```
