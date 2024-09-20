Linux<br />因为网络传输的限制，导致很多时候，需要在 Linux 系统下进行大文件的切割。这样将一个大文件切割成为多个小文件，进行传输，传输完毕之后进行合并即可。
<a name="gCjPD"></a>
## 1、文件切割 - `split`
在 Linux 系统下使用 `split` 命令进行大文件切割很方便
<a name="clYDo"></a>
### [1] 命令语法
```bash
# -a: 指定输出文件名的后缀长度(默认为2个:aa,ab...)
# -d: 指定输出文件名的后缀用数字代替
# -l: 行数分割模式(指定每多少行切成一个小文件;默认行数是1000行)
# -b: 二进制分割模式(支持单位:k/m)
# -C: 文件大小分割模式(切割时尽量维持每行的完整性)
split [-a] [-d] [-l <行数>] [-b <字节>] [-C <字节>] [要切割的文件] [输出文件名]
```
<a name="HcIP2"></a>
### [2] 使用实例
```bash
# 行切割文件
$ split -l 300000 users.sql /data/users_

# 使用数字后缀
$ split -d -l 300000 users.sql /data/users_

# 按字节大小分割
$ split -d -b 100m users.sql /data/users_
```
<a name="RM7dp"></a>
### [3] 帮助信息
```bash
# 帮助信息
$ split --help
Usage: split [OPTION]... [FILE [PREFIX]]
Output pieces of FILE to PREFIXaa, PREFIXab, ...;
default size is 1000 lines, and default PREFIX is 'x'.

With no FILE, or when FILE is -, read standard input.

Mandatory arguments to long options are mandatory for short options too.
  -a, --suffix-length=N   generate suffixes of length N (default 2)            后缀名称的长度(默认为2)
      --additional-suffix=SUFFIX  append an additional SUFFIX to file names
  -b, --bytes=SIZE        put SIZE bytes per output file                       每个输出文件的字节大小
  -C, --line-bytes=SIZE   put at most SIZE bytes of records per output file    每个输出文件的最大字节大小
  -d                      use numeric suffixes starting at 0, not alphabetic   使用数字后缀代替字母后缀
      --numeric-suffixes[=FROM]  same as -d, but allow setting the start value
  -e, --elide-empty-files  do not generate empty output files with '-n'        不产生空的输出文件
      --filter=COMMAND    write to shell COMMAND; file name is $FILE           写入到shell命令行
  -l, --lines=NUMBER      put NUMBER lines/records per output file             设定每个输出文件的行数
  -n, --number=CHUNKS     generate CHUNKS output files; see explanation below  产生chunks文件
  -t, --separator=SEP     use SEP instead of newline as the record separator;  使用新字符分割
                            '\0' (zero) specifies the NUL character
  -u, --unbuffered        immediately copy input to output with '-n r/...'     无需缓存
      --verbose           print a diagnostic just before each                  显示分割进度
                            output file is opened
      --help     display this help and exit                                    显示帮助信息
      --version  output version information and exit                           显示版本信息

The SIZE argument is an integer and optional unit (example: 10K is 10*1024).
Units are K,M,G,T,P,E,Z,Y (powers of 1024) or KB,MB,... (powers of 1000).

CHUNKS may be:
  N       split into N files based on size of input
  K/N     output Kth of N to stdout
  l/N     split into N files without splitting lines/records
  l/K/N   output Kth of N to stdout without splitting lines/records
  r/N     like 'l' but use round robin distribution
  r/K/N   likewise but only output Kth of N to stdout

GNU coreutils online help: <http://www.gnu.org/software/coreutils/>
Full documentation at: <http://www.gnu.org/software/coreutils/split>
or available locally via: info '(coreutils) split invocation'
```
<a name="pTvqE"></a>
## 2、文件合并 - `cat`
在 Linux 系统下使用 `cat` 命令进行多个小文件的合并也很方便
<a name="F7cL3"></a>
### [1] 命令语法
```bash
# -n: 显示行号
# -e: 以$字符作为每行的结尾
# -t: 显示TAB字符(^I)
cat [-n] [-e] [-t] [输出文件名]
```
<a name="IPCU5"></a>
### [2] 使用实例
```bash
# 合并文件
$ cat /data/users_* > users.sql
```
<a name="F56P1"></a>
### [3] 帮助信息
```bash
# 帮助信息
$ cat --h
Usage: cat [OPTION]... [FILE]...
Concatenate FILE(s) to standard output.

With no FILE, or when FILE is -, read standard input.

  -A, --show-all           equivalent to -vET
  -b, --number-nonblank    number nonempty output lines, overrides -n
  -e                       equivalent to -vE
  -E, --show-ends          display $ at end of each line
  -n, --number             number all output lines
  -s, --squeeze-blank      suppress repeated empty output lines
  -t                       equivalent to -vT
  -T, --show-tabs          display TAB characters as ^I
  -u                       (ignored)
  -v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB
      --help     display this help and exit
      --version  output version information and exit

Examples:
  cat f - g  Output f's contents, then standard input, then g's contents.
  cat        Copy standard input to standard output.

GNU coreutils online help: <http://www.gnu.org/software/coreutils/>
Full documentation at: <http://www.gnu.org/software/coreutils/cat>
or available locally via: info '(coreutils) cat invocation'
```
