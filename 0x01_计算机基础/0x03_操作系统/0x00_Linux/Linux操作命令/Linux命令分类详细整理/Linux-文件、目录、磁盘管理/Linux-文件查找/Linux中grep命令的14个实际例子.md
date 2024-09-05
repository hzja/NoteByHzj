Linux
<a name="klJgL"></a>
### 搜索和查找文件
列出系统上安装了哪些python模块
```bash
rpm -qa | grep -i python
```
输出
```bash
python-kitchen-1.1.1-5.el7.noarch
python-IPy-0.75-6.el7.noarch
python-decorator-3.4.0-3.el7.noarch
python-syspurpose-1.24.48-1.el7.centos.x86_64
python2-pyasn1-0.1.9-7.el7.noarch
python-requestbuilder-0.7.1-1.el7.noarch
python-pycparser-2.14-1.el7.noarch
python-pyudev-0.15-9.el7.noarch
python2-pip-8.1.2-14.el7.noarch
python-configobj-4.7.2-7.el7.noarch
python-gobject-base-3.22.0-1.el7_4.1.x86_64
python-backports-1.0-8.el7.x86_64
python-progressbar-2.3-4.el7.noarch
python-perf-3.10.0-1160.31.1.el7.x86_64
python-2.7.5-90.el7.x86_64
python3-rpm-generators-6-2.el7.noarch
python2-wheel-0.29.0-2.el7.noarch
...
```
首先，跑了`dpkg –l`，其中列出了已安装*.deb系统上的软件包。其次，将该输出通过管道传输到grep –i python，它简单地说明grep并过滤掉并返回其中包含'python'的所有内容。"这`–i`选项是忽略大小写，因为grep区分大小写。使用`-i`选项是进入的好习惯，除非想确定更具体的搜索。
<a name="HF8F8"></a>
### 搜索包含关键词"error"的日志文件
```bash
grep "error" /var/log/syslog
```
<a name="xDOv3"></a>
### 搜索和过滤文件
这grep还可用于在单个文件或多个文件中搜索和过滤。来看看这个场景：<br />删除Nginx配置文件所有注释行
```bash
grep –v "#" /etc/nginx/nginx.conf
```
这`-v`选项告诉grep反转其输出，这意味着不是打印匹配的行，而是执行相反的操作并打印所有与表达式不匹配的行，在这种情况下，`#`注释行。
<a name="EaTb2"></a>
### 仅查找所有 .mp3 文件
多条件筛选
```bash
find . –name "*.mp3" | grep –i jaychou | grep –vi "七里香"
```
在这个例子中，使用 find 打印所有带有*.mp3 extension，管道它`grep -i`过滤并打印所有名称为 "的文件"jaychou" 然后另一个管道到`grep -vi`它过滤掉并且不打印所有带有字符串的文件名（在任何情况下）"remix"。
<a name="p3Ze4"></a>
### 搜索并输出匹配的行号
```bash
grep -n "pattern" file.txt
```
<a name="Wy3S1"></a>
### 显示搜索字符串前后的行数
另一个选项是`-A`和`-B`开关，显示匹配的行和搜索字符串之前或之后的行数。虽然手册页给出了更详细的解释，可以发现最容易记住的选项是`-A=after`， 和`-B=before`：
```bash
ifconfig | grep –A 4 eth0
ifconfig | grep  -B 2 UP
```
<a name="E2q1Y"></a>
### 打印匹配周围的行数
grep 的`-C` option 类似，但不是打印字符串之前或之后的行，而是打印任一方向的行：
```bash
ifconfig | grep –C 2 lo
```
<a name="q5Fdd"></a>
### 计算匹配次数
类似于管道grep字符串到字数（wc program) grep 的内置选项可以执行相同的操作：
```bash
ifconfig | grep –c inet6
```
<a name="qWNeA"></a>
### 按给定字符串搜索文件
这`-n`选项grep在编译错误期间调试文件时非常有用。它显示给定搜索字符串的文件中的行号：
```bash
grep –n "main" setup..py
```
<a name="clyVq"></a>
### 在所有目录中递归搜索字符串
搜索一个目录中所有文件中包含特定字符串的行。
```bash
grep -r "pattern" /path/to/directory
```
如果要在当前目录和所有子目录中搜索字符串，可以指定`-r`递归搜索的选项：
```bash
grep –r "function" *
```
<a name="iikVS"></a>
### 搜索整个模式
通过`-w` grep选项搜索字符串中的整个模式。例如，使用：
```bash
ifconfig | grep –w "RUNNING"
```
将打印出包含引号中的模式的行。另一方面，如果您尝试：
```bash
ifconfig | grep –w "RUN"
```
由于不是在搜索模式，而是整个单词，因此不会返回任何内容。
<a name="Yr6VM"></a>
### 在压缩文件中搜索字符串
zgrep 过滤查询压缩文件。类似于zcat，用于gzipped文件。它采用相同的选项grep并以相同的方式使用：
```bash
zgrep –i error /var/log/rumenz.gz
```
<a name="m2xdw"></a>
### 匹配文件中的正则表达式
这egrep是另一种代表 "Extended Global Regular Expression"。它识别额外的表达元字符，例如`at + ? |`和`().`<br />如果需要，egrep 对于搜索源文件和其他代码段非常有用。可以通过指定从常规 grep 调用它`-E`选项。
```bash
grep –E
```
<a name="dAiNh"></a>
### 搜索固定模式字符串
该命令用来查找与给定的字符串相匹配文件的行，fgrep命令查询速度比grep快，但是它不够灵活，不能用正则表达式。该命令等价于`grep -F`。<br />当前目录下所有以 .c字符串结尾的文件中搜索字符串strcpy：
```bash
fgrep strcpy *.c
```
