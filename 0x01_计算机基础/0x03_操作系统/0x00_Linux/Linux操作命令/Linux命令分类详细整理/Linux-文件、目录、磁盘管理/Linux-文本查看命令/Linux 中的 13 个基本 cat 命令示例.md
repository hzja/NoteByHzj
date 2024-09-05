Linux cat<br />cat (全称 concatenate) 命令是 Linux/类 Unix 操作系统中最常用的命令之一。cat 命令允许创建单个或多个文件、查看文件内容、连接文件和重定向终端或文件中的输出。<br />了解如何方便地使用 cat 命令及其在 Linux 中的示例。
<a name="Dxn3R"></a>
### Cat 命令的一般语法
```bash
$ cat [OPTION] [FILE]...
```
<a name="NAyQC"></a>
### 1、显示文件内容
下面的例子将显示 /etc/passwd 文件。
```bash
# cat /etc/passwd

root:x:0:0:root:/root:/bin/bash
bin:x:1:1:bin:/bin:/sbin/nologin
fcant:x:500:500::/home/fcant:/bin/bash
```
<a name="z5SrX"></a>
### 2、终端查看多个文件的内容
在下面的示例中，它将显示 Fcant 和 Fcant1 终端中的文件。
```bash
# cat Fcant Fcant1

Fcant
Fcant.com
```
<a name="JVXrQ"></a>
### 3、用`cat`命令创建文件
将创建一个名为 Fcant2 使用以下命令创建文件。
```bash
# cat > Fcant2
```
等待用户输入，键入所需文本，然后按 CTRL+D （按住 Ctrl key 并输入d） 退出。正文将写在Fcant2文件。可以使用以下命令查看文件的内容cat 命令。
```bash
# cat Fcant2

Fcant.com
```
<a name="VVOQQ"></a>
### 4、使用`more`和`less`参数的Cat命令
如果有大量的内容，屏幕滚动起来非常快，可以使用参数more和less
```bash
# cat song.txt | more
# cat song.txt | less
```
<a name="fcQuc"></a>
### 5、在文件中显示行号
随着 `-n` 选项可以看到一个文件的行号 song.txt 在输出端。
```bash
# cat -n song.txt
1 入门
2 小
3 入门
4 rumenz
5 rumenz.com
6 入门
7 你好 世界
```
<a name="Xv794"></a>
### 6、在文件末尾显示`$`
在下面，可以看到 `-e` 选项 `$` 显示在行尾，也显示在空格中 `$`如果段落之间有任何差距。此选项可用于将多行压缩为一行。
```bash
# cat -e rumenz

rumenz$
rumenz.com$
rumenz com$
hello world$
```
<a name="iAAyS"></a>
### 7、在文件中显示制表符分隔的行
在下面的输出中，可以看到TAB空间被' `^I` '字符填充。
```bash
# cat -T rumenz

rumenz
rumenz.com
rumenz^Icom
hello^Iworld
```
<a name="eNxYV"></a>
### 8、一次显示多个文件
在下面的例子中，有三个文件 rumenz, rumenz1, 和 rumenz2,并能够查看这些文件的内容，如上所示。需要将每个文件分开;。
```bash
# cat rumenz; cat rumenz1; cat rumenz2

This is a rumenz file
This is the rumenz1 file.
This is rumenz2 file.
```
<a name="fy7u5"></a>
### 9、使用标准输出和重定向操作符
可以将文件的标准输出重定向到一个新文件中，否则现有文件中带有 `>`（大于）符号。仔细，现有的内容rumenz1 将被内容覆盖 rumenz 文件。
```bash
# cat rumenz > rumenz1
```
<a name="dxCYy"></a>
### 10、使用重定向运算符附加标准输出
在现有文件中附加 `>>`（双大于）符号。这里的内容rumenz 文件将附加在文件的末尾 rumenz1 文件。
```bash
# cat rumenz >> rumenz1
```
<a name="qrikM"></a>
### 11、使用重定向运算符重定向标准输入
当将重定向与标准输入一起使用时`<`（小于符号），它使用文件名 rumenz2 作为命令的输入和输出将显示在终端中。
```bash
# cat < rumenz2

This is rumenz2 file.
```
<a name="JQf15"></a>
### 12、重定向包含在单个文件中的多个文件
这将创建一个名为 rumenz3 并且所有输出都将重定向到新创建的文件中。
```bash
# cat rumenz rumenz1 rumenz2 > rumenz3
```
<a name="k3dyk"></a>
### 13、在单个文件中对多个文件的内容进行排序
这将创建一个文件 rumenz4 和输出 cat 命令通过管道进行排序，结果将被重定向到新创建的文件。
```bash
# cat rumenz rumenz1 rumenz2 rumenz3 | sort > rumenz4
```
