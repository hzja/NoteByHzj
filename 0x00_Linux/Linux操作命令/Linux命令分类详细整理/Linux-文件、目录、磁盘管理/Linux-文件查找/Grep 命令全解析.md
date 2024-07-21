Linux<br />grep 命令是最常用的 UNIX 命令之一。通过使用 grep 命令，可以搜索包含特定单词或特定模式的文件。grep 命令提供了很多有用的选项，可用于增强 grep 命令的功能。通过使用 `grep -v` 可以列出所有不包含单词的文件，排除与模式匹配的文件，`grep -c` 将打印文件中匹配模式的计数等。<br />作为一个强大的工具，分析大量数据或日志文件，可以节省大量时间。但许多人使用 grep 只是为了在文件中查找单词，并没有使用 grep 的所有强大命令行选项和正则表达式功能。<br />**这里分享 11 个 Linux 中的 grep 命令示例。文末附网络排查工具大全。**
<a name="jyeN6"></a>
### **1、搜索时忽略某些单词**
查找相关词并排除不相关词。大多数时候在日志文件中查找异常和错误，有时知道某些可以忽略的异常，所以使用 `grep -v` 选项来排除这些异常
```bash
grep Exception logfile.txt | grep -v ERROR
```
这个 grep 命令示例将在 logfile.txt 中搜索单词Exception并打印它们，但是由于已经将第一个 grep 命令输出到第二个 grep 命令，该命令将排除与单词ERROR匹配的所有行。<br />再来看另一个示例，有一个包含三行的文件，如下所示：
```bash
$ cat example.txt
UNIX operating system
UNIX and Linux operating system
Linux operation system
```
要搜索文件 example.txt 中包含单词 UNIX 但同时不包含单词 Linux 的所有行。
```bash
$ grep UNIX example.txt
UNIX operating system
UNIX and Linux operating system
```
要排除所有包含 Linux 的行，将在此输出中应用另一个 grep 命令，并带有选项 `-v` 以排除匹配的单词，如下面的 grep 命令所示：
```bash
$ grep UNIX example.txt | grep -v Linux
UNIX operating system
```
<a name="Dq9Ju"></a>
### **2、计算文件中某个单词的出现次数**
如果要计算日志文件中的特定单词，可以使用 `grep -c` 选项来计算单词。下面的命令示例将打印单词Error 在 logfile.txt 中出现的次数。
```bash
grep -c "Error" logfile.txt
```
<a name="gwjeP"></a>
### **3、打印匹配单词前后的行**
有时不仅对匹配行感兴趣，而且对匹配行上下文感兴趣，这对于查看任何错误或异常之前发生的情况特别有用。`grep --context` 选项允许在匹配模式周围打印行。<br />下面是 UNIX 中的 grep 命令示例，将在 logfile.txt 中匹配单词“successful”的行周围打印 6 行
```bash
$ grep --context=6 successful logfile.txt
```
还可以使用命令行选项“C”而不是“--context ”
```bash
$ grep -C 2 'hello' *
```
在每个匹配行周围打印两行上下文。
<a name="KLSAA"></a>
### **4、使用 egrep 和正则表达式搜索模式**
egrep 是扩展的 grep，它比 grep 命令更强大，并且允许更多的正则表达式，比如可以使用`|` 通过仅执行一个命令来搜索错误或异常的选项。
```bash
$ egrep 'Error|Exception' logfile.txt
```
<a name="YaGcm"></a>
### **5、不区分大小写的搜索**
如果要进行不区分大小写的搜索，可以使用 `-i` 选项。`grep -i` 命令将查找任何出现的 Error、error、ERROR：
```bash
$ grep -i Error logfile
```
<a name="rnwqC"></a>
### **6、使用 zgrep 命令在 gzip 文件中搜索模式**
zgrep 用于执行与 grep 相同的操作，但用于 .gz 文件。通过压缩旧文件来减小大小，然后想在这些文件上查找某些内容。<br />下面的 grep 命令将打印所有带有Error的压缩文件。
```bash
$ zgrep -i Error *.gz
```
<a name="LL6KZ"></a>
### **7、目录中递归搜索**
使用 `grep` 命令进行递归搜索，可以使用：`-R` 命令行选项，如下所示，在当前目录和所有子目录中搜索指定的目录或文件。
```bash
$ grep -R store *
```
<a name="Ij9vQ"></a>
### **8、显示包含给定单词的文件名**
另一个有用的 grep 命令行选项是 `grep -l`，它只显示与给定模式匹配的文件名。下面的命令只会显示有ERROR的文件名：
```bash
grep -l ERROR *.log
```
`grep -l 'main' *.java` 将列出当前目录中内容包含 `main` 的所有 Java 文件名称。
<a name="XKKB4"></a>
### **9、显示行号**
想查看匹配行的行号，可以使用下面的选项`grep -n`命令将显示出现 Error 的行。
```bash
$ grep -n ERROR log file.
```
<a name="Vx7Fr"></a>
### **10、搜索文件中的整个单词**
可以使用 grep -w 命令来查找整个单词而不是仅查找模式，如下示例将仅打印 logfile.txt 中包含完整单词 ERROR 的行。
```bash
$ grep -w ERROR logfile.txt
```
UNIX 中的上述 `grep` 命令仅搜索作为整个单词的 'ERROR' 实例；它不匹配 SysERROR。<br />请使用 `\<' 和 `\>' 来匹配单词的开头和结尾。例如：
```bash
$ grep 'ERROR>' *
```
仅搜索以“ERROR”结尾的单词，因此它匹配单词“SysERROR”。
<a name="r8crR"></a>
### **11、颜色模式**
grep 命令可以显示匹配的颜色模式，这对于突出显示匹配部分非常有用：
```bash
$ grep Exception today.log --color
```
可以在 bash_profile 文件中创建`alias grep='grep --color'`以避免每次都键入 `--color`。
