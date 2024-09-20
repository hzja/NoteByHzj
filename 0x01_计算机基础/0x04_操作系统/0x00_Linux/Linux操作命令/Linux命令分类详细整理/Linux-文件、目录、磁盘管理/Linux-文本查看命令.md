Linux cat tac more less head tail sort sed uniq 
<a name="94nEW"></a>
## 全文本显示--`cat`
`cat`可能是常用的一个文本查看命令了，使用方法也很简单：
```bash
cat file  #全文本显示在终端
cat -n file #显示全文本，并显示行号
```
另外，`cat`也可用作合并文件：
```bash
cat file1 file2 >file3
```
该命令将file1 file2的内容合并写到file3中。
<a name="NbtIW"></a>
## 倒序显示全文本--`tac`
`tac`是`cat`倒过来的写法，`tac`以行为单位，倒序显示全文本内容。
```bash
tac file
```
<a name="gPOp9"></a>
## 分页显示文本--`more`
`cat`将整个文本内容输出到终端。那么也就带来一个问题，如果文本内容较多，前面的内容查看将十分不便。而`more`命令可以分页显示。
<a name="Yoexn"></a>
### 1、显示内容
```bash
more file
```
之后，就可以使用按键来查看文本。常用按键如下：
```bash
回车    #向下n行，默认为1行
空格    #向下滚动一屏
b      #向上滚动一屏
=      #输出当前行号
:f     #输出当前文件名和当前行号
q      #退出
```
<a name="sMxou"></a>
### 2、从指定行开始显示
```bash
more +10 file
```
该命令从第10行开始显示file的内容。
<a name="7174a36c"></a>
### 3、从匹配的字符串行开始显示
```bash
more +/string file
```
该命令从有string的行的前两行开始file的内容。
<a name="ATTn6"></a>
## 任意浏览搜索文本--`less`
`less`命令的基本功能和`more`没有太大差别，但是`**less**`**命令可以向前浏览文件，而**`**more**`**只能向后浏览文件**，同时`less`还拥有更多的搜索功能。<br />常见使用方法：
```bash
less file     #浏览file
less -N file  #浏览file，并且显示每行的行号
less -m file  #浏览file，并显示百分比
```
常用按键如下：
```bash
f        #向前滚动一屏
b        #向后滚动一屏
回车或j   #向前移动一行
k        #向后移动一行
G        #移动到最后一行
g        #移动到第一行
/string  #向下搜索string，n查看下一个，N查看上一个结果
？string #向上搜索string，n查看下一个，N查看上一个结果
q    #退出
```
相比`more`命令，`less`命令能够搜索匹配需要的字符串。<br />另外，`less`还能在多个文件间切换浏览：
```bash
less file1 file2 file3
:n     #切换到下一个文件
:p     #切换到上一个文件
:x     #切换到第一个文件
:d     #从当前列表移除文件
```
<a name="4WZqq"></a>
## 显示文本头部内容--`head`
`head`命令的作用就像它的名字一样，用于显示文件的开头部分文本。<br />常见用法如下：
```
head -n 100 file #显示file的前100行
head -n -100 file #显示file的除最后100行以外的内容。
```
<a name="mmaGa"></a>
## 显示文本尾部内容--`tail`
和`head`命令类似，只不过`tail`命令用于读取文本尾部部分内容：
```bash
tail -100 file  #显示file最后100行内容
tail -n +100 file  #从第100行开始显示file内容
```
`tail`还有一个比较实用的用法，用于实时文本更新内容。比如说，有一个日志文件正在写，并且实时在更新，就可以用命令：
```bash
tail -f logFile
```
对于更新的日志内容，会实时打印到终端上，方面查看实时日志。
<a name="kTmnB"></a>
## 指定顺序显示文本--`sort`
`sort`可用于对文本进行排序并显示，默认为字典升序。<br />Linux sort 命令用于将文本文件内容加以排序。<br />sort 可针对文本文件的内容，以行为单位来排序
<a name="TA8kx"></a>
### 语法
```bash
sort [-bcdfimMnr][-o<输出文件>][-t<分隔字符>][+<起始栏位>-<结束栏位>][--help][--verison][文件][-k field1[,field2]]
```
<a name="kIcaE"></a>
### 参数

- -b 忽略每行前面开始出的空格字符。
- -c 检查文件是否已经按照顺序排序。
- -d 排序时，处理英文字母、数字及空格字符外，忽略其他的字符。
- -f 排序时，将小写字母视为大写字母。
- -i 排序时，除了040至176之间的ASCII字符外，忽略其他的字符。
- -m 将几个排序好的文件进行合并。
- -M 将前面3个字母依照月份的缩写进行排序。
- -n 依照数值的大小排序。
- -u 意味着是唯一的(unique)，输出的结果是去完重了的。
- -o<输出文件> 将排序后的结果存入指定的文件。
- -r 以相反的顺序来排序。
- -t<分隔字符> 指定排序时所用的栏位分隔字符。
- +<起始栏位>-<结束栏位> 以指定的栏位来排序，范围由起始栏位到结束栏位的前一栏位。
- –help 显示帮助。
- –version 显示版本信息。
- [-k field1[,field2]] 按指定的列进行排序。

例如有一段文本test.txt内容如下：
```bash
$ vim
count
fail
help
help
dead
apple
```
<a name="oKrDC"></a>
### 1、升序显示文本
使用命令：
```bash
$ sort test.txt
apple
count
dead
fail
help
help
vim
```
文本内容将以升序显示。
<a name="qou7I"></a>
### 2、`-r`—降序显示
相关参数`-r`：
```bash
$ sort -r test.txt
vim
help
help
fail
dead
count
apple
```
<a name="fSgqE"></a>
### 3、`-u`—去掉重复的行
可以观察到，前面的`help`有两行，如果不想看到重复的行，可以使用参数`-u`，例如：
```bash
$ sort -u test.txt
apple
count
dead
fail
help
vim
```
可以看到help行不再重复显示。
<a name="Aj2Kv"></a>
### 4、按照数字排序
如果按照字典排序，10将会在2的前面，因此需要按照数字大小排序：
```bash
sort -n file
```
<a name="HXNCq"></a>
### 实例
1、直接排序
```bash
cat << EOF | sort
test 30  
Hello 95  
Linux 85
EOF
Hello 95  
Linux 85
test 30
```
2、使用 -k 参数设置对第二列的值进行重排
```bash
cat << EOF | sort -k 2
test 30  
Hello 95  
Linux 85
EOF
test 30  
Linux 85
Hello 95
```
3、使用 `-r` 进行倒序排序
```bash
cat << EOF | sort -r
test 30  99
Hello 95  76
Linux 85 35
EOF
test 30  99
Linux 85 35
Hello 95  76
```
<a name="CUNbR"></a>
## 过滤显示文本--`sed`
`sed`是一个流编辑器，功能非常强大，但本文只介绍文本查看相关功能。
<a name="AQarS"></a>
### 1、显示匹配关键字行
有时候查看日志，可能只需要查看包含某些关键字的日志行：
```bash
sed -n "/string/p" logFile
```
上面的命令表示打印包含string的行。
<a name="bzPvH"></a>
### 2、打印指定行
```
sed -n "1,5p" logFile #打印第1到5行
sed -n '3,5{=;p}' logFile #打印3到5行，并且打印行号
sed -n "10p" logFIle  #打印第10行
```
<a name="Y9RZN"></a>
## 去重显示文本--`uniq`
uniq 命令用于检查及删除文本文件中重复出现的行列，一般与 sort 命令结合使用。<br />uniq 可检查文本文件中重复出现的行列
<a name="oK394"></a>
### 语法
```
uniq [-cdu][-f<栏位>][-s<字符位置>][-w<字符位置>][--help][--version][输入文件][输出文件]
```
<a name="eO3IA"></a>
### 参数

- -c或–count 在每列旁边显示该行重复出现的次数。
- -d或–repeated 仅显示重复出现的行列。
- -f<栏位>或–skip-fields=<栏位> 忽略比较指定的栏位。
- -s<字符位置>或–skip-chars=<字符位置> 忽略比较指定的字符。
- -u或–unique 仅显示出一次的行列。
- -w<字符位置>或–check-chars=<字符位置> 指定要比较的字符。
- –help 显示帮助。
- –version 显示版本信息。
- [输入文件] 指定已排序好的文本文件。如果不指定此项，则从标准读取数据；
- [输出文件] 指定输出的文件。如果不指定此选项，则将内容显示到标准输出设备（显示终端）。
<a name="LrU6a"></a>
### 实例
常见用法如下：
```bash
uniq file  #去除重复的行
uniq -c file #去除重复的行，并显示重复次数
uniq -d file #只显示重复的行
uniq -u file #只显示出现一次的行
uniq -i file #忽略大小写，去除重复的行
uniqe -w 10 file #认为前10个字符相同，即为重复
```
```bash
# 删除重复行
cat << EOF | uniq -c
test 30  
test 30  
test 30  
Hello 95  
Hello 95  
Hello 95  
Hello 95  
Linux 85  
Linux 85
EOF
      3 test 30  
      4 Hello 95  
      1 Linux 85  
      1 Linux 85
# 排序删除
cat << EOF | sort |uniq -c
test 30  
test 30  
test 30  
Hello 95  
Hello 95  
Hello 95  
Hello 95  
Linux 85  
Linux 85
EOF
      4 Hello 95  
      1 Linux 85
      1 Linux 85  
      3 test 30 
# 找出重复行
cat << EOF |uniq -d
test 30  
test 30  
test 30  
Hello 95  
Hello 95  
Hello 95  
Hello 95  
Linux 85  
Linux 85
Linux 85
EOF
test 30  
Hello 95  
Linux 85
```
<a name="Wpkt4"></a>
## 文本编辑查看--`vi`
查看文件也很简单：
```bash
vi file
```
