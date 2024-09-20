Linux xargs<br />`xargs` 可以将一个命令的输出作为参数发送给另一个命令。<br />在 Linux 中，所有标准的应用程序都有与之关联的三个数据流。分别是标准输入流（stdin），标准输出流（stdout）和标准错误流（stderr）。这些流通过文本来运行，使用文本将输入（stdin）发送到命令，然后响应（stdout）将会以文本形式显示在终端窗口上。错误消息也以文本的形式显示在终端窗口上（stderr）。<br />Linux 和类 Unix 操作系统的一大功能是可以将一个命令的标准输出流传递到另一个命令的标准输入流。第一个命令不会管它的输出是否写到了终端窗口，第二个命令也不会管它的输入是否来自键盘。<br />虽然所有 Linux 命令都有三个标准流，但是并不是所有命令都接受另一个命令的标准输出作为它的标准输入流的输入。因此无法通过管道将输入传给这些命令。<br />`xargs`是一个使用标准数据流构建执行管道的命令。通过使用`xargs`命令可以使 `echo`，`rm` 和 `mkdir` 等命令接受标准输入作为它们的参数。
<a name="TOa99"></a>
### xargs命令
`xargs`接受管道输入，也可以接受来自文件的输入。`xargs` 使用该输入作为指定的命令的参数。如果没有给`xargs`指定特定的命令，则默认使用 `echo`。`xargs` 始终生成单行输出，即使输入的数据是多行的。<br />假如使用 `ls` 的 `-1`（每行列出一个文件）选项，则会得到一列文件名称：
```bash
$ ls -l ./*.pid
```
这一命令列出了当前目录中的 Shell 脚本文件。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275167555-82727493-1df8-4277-853a-bb7fef530dee.png#align=left&display=inline&height=325&originHeight=649&originWidth=3323&size=695217&status=done&style=none&width=1661.5)<br />如果将输出结果通过管道传递给 `xargs` ，会得到的效果
```bash
$ ls -l ./*.pid | xargs
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275209762-1029767c-a73f-4bb3-8b25-f1259b1bb912.png#align=left&display=inline&height=190&originHeight=380&originWidth=3323&size=405048&status=done&style=none&width=1661.5)<br />可以看出来，输出以一长串文本的形式写到了终端上。由此可见，`xargs` 可以将输出作为参数传递给其他命令。
<a name="6eWik"></a>
#### `xargs` 用三种用法

- 分隔数据，避免参数过长
- 传递参数，从而组合多个命令
- 处理不支持管道的命令
<a name="Rqtae"></a>
### Xargs命令参数
```
-0, --null
 如果输入的 stdin 含有特殊字符，例如反引号 `、反斜杠 \、空格等字符时，xargs 将它还原成一般字符。为默认选项
-a, --arg-file=FILE
 从指定的文件 FILE 中读取输入内容而不是从标准输入
-d, --delimiter=DEL
 指定 xargs 处理输入内容时的分隔符。xargs 处理输入内容默认是按空格和换行符作为分隔符，输出 arguments 时按空格分隔
-E EOF_STR
 EOF_STR 是 end of file string，表示输入的结束
-e, --eof[=EOF_STR]
 作用等同于 -E 选项，与 -E 选项不同时，该选项不符合 POSIX 标准且 EOF_STR 不是强制的。如果没有 EOF_STR 则表示输入没有结束符
-I REPLACE_STR
 将 xargs 输出的每一项参数单独赋值给后面的命令，参数需要用指定的替代字符串 REPLACE_STR 代替。REPLACE_STR 可以使用 {} $ @ 等符号，其主要作用是当 xargs command 后有多个参数时，调整参数位置。例如备份以 txt 为后缀的文件：find . -name "*.txt" | xargs -I {}  cp {} /tmp/{}.bak
-i, --replace[=REPLACE_STR]
 作用同 -I 选项，参数 REPLACE_STR 是可选的，缺省为 {}。建议使用 -I 选项，因为其符合 POSIX
-L MAX_LINES
 限定最大输入行数。隐含了 -x 选项
-l, --max-lines[=MAX_LINES]
 作用同 -L 选项，参数 MAX_LINES 是可选的，缺省为 1。建议使用 -L 选项，因为其符合 POSIX 标准
-n, --max-args=MAX_ARGS
 表示命令在执行的时候一次使用参数的最大个数
-o, --open-tty
 在执行命令之前，在子进程中重新打开stdin作为/dev/TTY。如果您希望xargs运行交互式应用程序，这是非常有用的
-P, --max-procs=MAX_PROCS
 每次运行最大进程；缺省值为 1。如果 MAX_PROCS 为 0，xargs 将一次运行尽可能多的进程。一般和 -n 或 -L 选项一起使用
-p, --interactive
 当每次执行一个 argument 的时候询问一次用户
--process-slot-var=NAME
 将指定的环境变量设置为每个正在运行的子进程中的唯一值。一旦子进程退出，将重用该值。例如，这可以用于初始负荷分配方案
-r, --no-run-if-empty
 当 xargs 的输入为空的时候则停止 xargs，不用再去执行后面的命令了。为默认选项
-s, --max-chars=MAX_CHARS
 命令行的最大字符数，指的是 xargs 后面那个命令的最大命令行字符数，包括命令、空格和换行符。每个参数单独传入 xargs 后面的命令
--show-limits
 显示操作系统对命令行长度的限制
-t， --verbose
 先打印命令到标准错误输出，然后再执行
-x, --exit
 配合 -s 使用，当命令行字符数大于 -s 指定的数值时，退出 xargs
--help
 显示帮助信息并退出
--version
 显示版本信息并退出
```
<a name="2WQYt"></a>
### 使用带有确认消息的xargs
<a name="GR7rU"></a>
#### 创建文件配合touch命令使用时
可以使用 `-p`（交互）选项来让 `xargs` 提示是否要进行下一步的操作。<br />如果通过 `xargs` 将一串文件名的字符串传递给 `touch` 命令，`touch` 将创建这些文件。
```bash
$ echo 'one two three' | xargs -p touch
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275400744-89b46af7-da5b-4978-9817-ab71319399e2.png#align=left&display=inline&height=162&originHeight=323&originWidth=3323&size=291299&status=done&style=none&width=1661.5)<br />终端上显示将要执行的命令，`xargs` 等待输入 `y` 或 `Y` ，`n` 或 `N` 并按 Enter 来响应。如果只按了 `Enter` ，则视为 `n`。只有当当输入 `y` 或 `Y` 时才执行该命令。<br />按下`y`和 Enter ，然后使用`ls`用来检查文件是否已经创建。
```bash
$ ls one two three
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275458329-5ccef871-aac2-42e9-935b-667319ddc9cc.png#align=left&display=inline&height=352&originHeight=704&originWidth=3323&size=777303&status=done&style=none&width=1661.5)
<a name="bX2YA"></a>
### Xargs-分割数据
通过命令列一下指定目录下面文件，下面的命令可以把 `~/local` 下面的所有一级目录的文件列清楚，非常方面逐层的查找文件
```bash
find ./local/ -maxdepth 1 | xargs ls -l
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598841621704-a8c9ff2e-8c26-4120-b966-c719a2fdd26e.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2077240&status=done&style=none&width=1107.6666666666667)
<a name="l2kVJ"></a>
### Xargs-传递参数
假设一个网站ID从1到100个用户，需要请求一下验证是否正确，可以通过下面的命令测试。
```bash
echo {1..100} | xargs -p -n1 -Ii curl https://api.example.com/i
```
其中 `-I` 后面的 `i` 是用来接收前面 1-100 的变量内容，赋值给后面的 `curl [https://api.example.com/i](https://api.example.com/i)` 的 `i`，其中 `-t` 参数不是这个例子的关键参数，这个参数是为了在运行命令的时候打印出来具体的命令，确定使用的对，相当于 debug。具体输出内容如下
```bash
$ echo {1..100} | xargs -p -n1 -Ii curl https://api.example.com/i
curl https://ap1.example.com/1?...
curl https://ap2.example.com/2?...
curl https://ap3.example.com/3?...
curl https://ap4.example.com/4?...
curl https://ap5.example.com/5?...
curl https://ap6.example.com/6?...
```
<a name="il6ww"></a>
### Xargs-处理不支持管道的命令
<a name="SyL88"></a>
#### 将xargs与多个命令一起使用-读取文本内容创建文件目录
可以用`-I`（初始参数）选项来将 `xargs` 和多个命令一起使用。这一选项定义了`替换字符串`。在命令行中的任何出现替换字符串的位置，都会插入提供给 `xargs` 的值 。<br />有点抽象，以一个实例来进行讲解。<br />先用 `tree` 命令查看当前目录中的子目录。该 `-d`（directory）选项使 `tree` 命令忽略文件，只输出目录。
```bash
$ tree -d
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275750123-16acc441-651e-4bcd-ba05-3eb495cdf4eb.png#align=left&display=inline&height=190&originHeight=380&originWidth=3323&size=349797&status=done&style=none&width=1661.5)<br />现在只有一个子目录 images 。<br />在 dir.txt 这个文件中，有一些想要创建的目录的名称。先用 `cat` 查看其中的内容。
```bash
$ cat dir.txt
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275800131-83516911-db3b-4835-b949-39981cb65951.png#align=left&display=inline&height=187&originHeight=374&originWidth=3323&size=343300&status=done&style=none&width=1661.5)<br />把这些内容作为输入数据传给 `xargs` ，执行以下的命令：
```bash
$ cat dirs.txt | xargs -I % sh -c 'echo %; mkdir %'
```
这条命令执行了以下操作：

- **cat dir.txt** ：将 directrories.txt 文件的内容（所有要创建的目录名称）传给 `xargs`。
- **xargs -I %** ：定义了替换字符串 `%`。
- **sh -c**：启动一个新的子shell。`-c`（commond）让 shell 读取命令。
- **'echo %; mkdir %'**：每个`％`都会被替换为 `xargs` 传过来的目录名称 。`echo`命令打印目录名称，`mkdir` 命令创建目录。

命令执行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275830065-a7be57a5-3327-47c9-b895-e1596126074a.png#align=left&display=inline&height=431&originHeight=862&originWidth=3323&size=892080&status=done&style=none&width=1661.5)<br />可以用 `tree` 验证已创建是否已创建了目录。
```bash
$ tree -d
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275834834-584e3a69-66b5-4735-af62-607314796b0b.png#align=left&display=inline&height=431&originHeight=862&originWidth=3323&size=892080&status=done&style=none&width=1661.5)
<a name="Jc98f"></a>
#### 配合wc命令使用xargs
可以使用 `xargs` 命令轻松地让 `wc` 命令计算多个文件中的单词数，字符数和行数
```bash
$ ls *.pid | xargs wc
```
执行结果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598275270289-35ca6600-75cb-4564-9e84-2a62b9278b07.png#align=left&display=inline&height=353&originHeight=705&originWidth=3323&size=728109&status=done&style=none&width=1661.5)<br />命令运行结果显示了每个文件的统计信息以及总数。<br />这条命令执行了以下操作：

- `ls`列出了所有的 .page 文件，并将该列表传给了`xargs`。
- `xargs` 将所有文件名传递给 `wc`。
- `wc` 将这些文件名作为命令行参数进行处理。
<a name="UEjkK"></a>
#### 将文件复制到多个位置
可以使用 `xargs` 命令来用一个命令将文件复制到多个位置。<br />首先，通过管道将两个目录的名称传给 `xargs` 。并且让 `xargs` 一次只将其中一个参数传递给正在使用的命令。<br />想要调用 `cp` 两次，每次各使用两个目录中的一个作为命令行参数，可以通过将 `xargs` 的 `-n`（max number）选项设置为 1 来实现。<br />这里还使用了`-v`（verbose 详细信息）选项，让 `cp` 反馈正在执行的操作。
```bash
$ echo ~/dir1/ ~/dir2/ | xargs -n 1 cp -v ./*.c
```
将文件复制到了两个目录，一次复制一个目录。`cp` 反馈了详细信息，看到进行了哪些操作。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598276267378-87f7cc9d-2415-43e2-a70c-6067370d448b.png#align=left&display=inline&height=875&originHeight=1750&originWidth=3323&size=1961270&status=done&style=none&width=1661.5)
<a name="zcSTG"></a>
#### 删除嵌套目录中的文件
如果文件名中包含空格或者其他特殊字符（例如换行符），`xargs` 将无法正确解释这些文件名。可以使用 `-0`（空终止符）选项来解决这一问题。此时， `xargs` 将使用 `null` 字符作为文件名最终的分隔符。<br />这里以 `find` 命令为例。`find` 有自己的选项来处理文件名中的空格和特殊字符，即 `-print0`（全名，空字符）选项。
```bash
$ find . -name "*.png" -type f -print0 | xargs -0 rm -v -rf "{}"
```
这一命令执行了以下操作：

- **find . -name “*.png”** ：`find` 将从当前目录中搜索名称和 *.png 相匹配的对象，`type -f` 指定了只搜索文件。
- **-print0**：名称将以空字符结尾，并且保留空格和特殊字符。
- **xargs -0**：`xargs` 也将考虑文件名以空值结尾，并且空格和特殊字符不会引起问题。
- **rm -v -rf "{}"**：`rm` 将反馈正在进行的操作（`-v`），递归进行操作（-r），不发送错误提示而直接删除文件（`-f`）。每个文件名替换 "{}"。

命令执行之后，将搜索了所有子目录，删除了其中匹配的文件。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598276708118-7c355f88-7004-40a7-ac14-b4f5f64eda58.png#align=left&display=inline&height=190&originHeight=380&originWidth=3323&size=372145&status=done&style=none&width=1661.5)
<a name="GIyp7"></a>
#### 删除嵌套目录
假设要删除一组嵌套的子目录，先用`tree`进行查看。
```bash
$ tree -d
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598276745931-393ad6ce-075d-442b-b97e-345fed7b226d.png#align=left&display=inline&height=406&originHeight=812&originWidth=3323&size=828698&status=done&style=none&width=1661.5)
```bash
$ find . -name "level_one" -type d -print0 | xargs -0 rm -v -rf "{}"
```
这条命令使用 find 在当前目录中递归搜索，搜索的目标是名为 level_one 的目录，然后将目录名通过`xargs`传递给 `rm` 。<br />这个命令和前面的命令之间的区别是，搜索的项目是最顶层目录的名称，而且`-type d`说明要查找的目录，而不是文件。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598276765928-b6e083a2-81db-40b4-b325-df32d7ee9899.png#align=left&display=inline&height=540&originHeight=1080&originWidth=3323&size=1174251&status=done&style=none&width=1661.5)<br />每个目录的名称都在删除时打印出来。可以用`tree`再查看效果：
```bash
$ tree -d
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598276780098-8e7c1b77-3a12-43ab-b854-7bc78f0bc21f.png#align=left&display=inline&height=782&originHeight=1563&originWidth=3323&size=1671689&status=done&style=none&width=1661.5)<br />所有嵌套的子目录已删除了。
<a name="142qN"></a>
#### 删除一种文件类型以外的所有文件
可以使用 `find` ，`xargs` 和 `rm` 删除所有类型的文件而只保留一种想要保留的类型的文件。这需要提供想要保留的文件类型。<br />`-not` 选项让 `find` 返回所有与搜索模式不匹配的文件名。此时再次使用 `xargs` 的 `-I` （初始参数）选项。这次定义的替换字符串为 `{}` 。这和之前使用的替换字符串 `%` 的效果是相同的。
```bash
$ find . -type f -not -name "*.sh" -print0 | xargs -0 -I {} rm -v {}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598276895501-cf306637-af08-4cfd-b6d2-2625378316f4.png#align=left&display=inline&height=648&originHeight=1296&originWidth=3323&size=1470091&status=done&style=none&width=1661.5)<br />命令执行之后，再通过 `ls` 来确认结果。可以看到，目录中只剩下了与 `*.sh` 相匹配的文件。
```bash
$ ls -l
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598276917990-4f4be45b-5eee-4474-b448-13f5e3582460.png#align=left&display=inline&height=890&originHeight=1780&originWidth=3323&size=2023934&status=done&style=none&width=1661.5)
<a name="6SqD6"></a>
#### 使用Xargs创建压缩文件
可以使用 `find` 命令来搜索文件，并通过 `xargs` 将文件名传给 `tar` 命令来创建压缩文件。<br />在当前目录中搜索 `* .sh` 文件。
```bash
$ find ./ -name "*.sh" -type f -print0 | xargs -0 tar -cvzf script_files.tar.gz
```
命令执行结果将列出了所有 .sh 文件，并创建了压缩文件。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598276980588-78a1e83b-887e-4339-9699-c33287713cd0.png#align=left&display=inline&height=571&originHeight=1142&originWidth=3323&size=1301592&status=done&style=none&width=1661.5)
<a name="BOKZD"></a>
#### 使用Xargs配合curl访问URL
有一个文件里面有 20个 URL，编写一个命令，分别请求这些URL。文件内容如下，文件名(urls.txt)。
```
https://www.baidu.com
https://www.google.com
https://www.alibaba.com
https://www.qq.com
https://www.mi.com
```
<a name="yNrpL"></a>
##### 循环来处理读取文本再执行curl进行访问
```bash
for i in `cat urls.txt`
do
curl $i
done
```
<a name="rjBJm"></a>
##### 通过xargs处理
```bash
cat urls.txt | xargs curl
```
<a name="b2oDR"></a>
#### 使用Xargs配合mkdir命令创建多文件目录
```bash
echo "one two three" | xargs mkdir
```
上面代码中，`mkdir` 会新建三个子目录，因为 `xargs` 将 `one` `two` `three` 分解成三个命令行参数，执行 `mkdir` `one` `two` `three`
<a name="lBnAs"></a>
#### 将 Shell 的特殊字符反引号还原为一般字符
```bash
echo '`0123`4 56789' | xargs -t echo
echo `0123`4 56789 
`0123`4 56789
```
如果直接进行如下操作，会报无法找到命令 01234 的错误，因为反引号在 Shell 中会将 01234 作为一个命令来执行，但是 01234 不是一个命令。`-t` 表示先打印命令，然后再执行。
```bash
echo `01234` 56789
-bash: 01234: command not found
56789
```
<a name="dc2vw"></a>
#### 设置 xargs 读入参数时的结束标识，以逗号结束
这里要注意结束标志必须要是单独的字段，即以空格或者换行符分隔的字段。
```bash
echo 01234 , 56789 | xargs -E ","
01234
```
<a name="bbe2aea7"></a>
#### 使用 rm、mv 等命令同时操作多个文件
使用 rm、mv 等命令同时操作多个文件时，有时会报 “argument list too long” 参数列表过长的错误，此时可以使用 xargs 来解决。xargs 将标准输入的字符串分隔后，作为参数传递给后面的命令。例如，给当前目录的所有文件添加后缀名。
```bash
ls | xargs -t -i mv {} {}.bak
# 选择符合条件的文件
ls | grep -E "201701|201702|201703" | xargs -I {} mv {} {}.bak
```
<a name="4mqtg"></a>
#### 设置命令行的最大字符数
参数默认一个一个单独传入命令中执行。
```bash
echo "01234 56789" | xargs -t -s 11
echo 01234 
01234
echo 56789 
56789
```
<a name="07dKL"></a>
#### 设置标准输入中每次多少行作为命令的参数
默认是将标准输入中所有行的归并到一行一次性传给命令执行。
```bash
echo -e "01234\n56789\n01234" | xargs -t -L 2 echo
echo 01234 56789 
01234 56789
echo 01234 
01234
```
<a name="DK7Wk"></a>
#### 将文件内容以空格分隔合并为一行输出
```bash
# 列出文件内容
$ cat test.txt
a b c d e
f g h i j 
k l m n o
# 多行输入合并为一行输出
$ cat test.txt | xargs
a b c d e f g h i j k l m n o
```
<a name="sOxTp"></a>
#### 与 ps、grep、awk 和 kill 结合，强制终止指定进程
```bash
ps -ef | grep spp | awk '{printf "%s ",$2}' | xargs kill -9
```
`ps \-ef|grep spp`用于查找包含 spp 的进程，`awk '{printf "%s ",$2,FNR}`将目标进程 ID 打印输出，`xargs kill \-9`则将目标进程 ID 作为参数传递给`kill \-9`用于杀死进程。
