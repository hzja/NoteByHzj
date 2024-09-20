Linux find grep xargs sort uniq tr cut paste wc sed awk<br />使用Shell处理文本时最常用的工具：find、grep、xargs、sort、uniq、tr、cut、paste、wc、sed、awk；
<a name="8DBOl"></a>
## find-文件查找
<a name="nu8j6"></a>
### 查找txt和pdf文件
```bash
find . ( -name "*.txt"-o -name "*.pdf") -print
```
<a name="rteHr"></a>
### 正则方式查找.txt和pdf
```bash
find . -regex  ".*(.txt|.pdf)$"
```
<a name="bwFkp"></a>
### -iregex：忽略大小写的正则3 否定参数查找所有非txt文本
```bash
find . ! -name "*.txt"-print
```
<a name="g0kql"></a>
### 指定搜索深度打印出当前目录的文件（深度为1）
```bash
find . -maxdepth 1-type f
```
<a name="zohXc"></a>
### 定制搜索按类型搜索
```bash
find . -type d -print  //只列出所有目录
-type f 文件 / l 符号链接
```
<a name="GNIMO"></a>
### 按时间搜索
```bash
-atime 访问时间 (单位是天，分钟单位则是-amin，以下类似）
-mtime 修改时间 （内容被修改）
-ctime 变化时间 （元数据或权限变化）
```
<a name="7RChm"></a>
#### 最近7天被访问过的所有文件
```bash
find . -atime 7-type f -print
```
<a name="HcTxz"></a>
#### 按大小搜索：w字 k M G寻找大于2k的文件
```bash
find . -type f -size +2k
```
<a name="7wJKr"></a>
### 按权限查找
```bash
find . -type f -perm 644-print //找具有可执行权限的所有文件
```
<a name="CHc6f"></a>
### 按用户查找
```bash
find . -type f -user weber -print// 找用户weber所拥有的文件
```
<a name="8r6YY"></a>
### 找到后的后续动作删除：删除当前目录下所有的swp文件
```bash
find . -type f -name "*.swp"-delete
```
<a name="xzf0v"></a>
### 执行动作（强大的exec）
```bash
find . -type f -user root -exec chown weber {} ; //将当前目录下的所有权变更为weber
```
注：{}是一个特殊的字符串，对于每一个匹配的文件，{}会被替换成相应的文件名；eg：将找到的文件全都copy到另一个目录
```bash
find . -type f -mtime +10-name "*.txt"-exec cp {} OLD ;
```
结合多个命令tips: 如果需要后续执行多个命令，可以将多个命令写成一个脚本。然后 -exec 调用时执行脚本即可；
```bash
-exec ./commands.sh {} ;
```
-print的定界符默认使用' '作为文件的定界符；<br />-print0 使用''作为文件的定界符，这样就可以搜索包含空格的文件；
<a name="NOOrP"></a>
## grep-文本搜索
语法：grep [option]... PATTERN [FILE]
```bash
grep match_patten file // 默认访问匹配行
```
常用参数:<br />-o 只输出匹配的文本行 VS -v 只输出没有匹配的文本行<br />-c 统计文件中包含文本的次数，符合条件的行数
```bash
grep -c "text" filename
```
-n 打印匹配的行号<br />-i 搜索时忽略大小写<br />-l 只打印文件名<br />-v 取反<br />^* 以*开头         <br />*$ 以*结尾 <br />^$ 空行 
<a name="BkzA5"></a>
### 在多级目录中对文本递归搜索
```bash
grep "class". -R -n
```
<a name="NA6ze"></a>
### 匹配多个模式
```bash
grep -e "class" -e "vitural" file
```
<a name="2f1e73ef"></a>
### grep输出以作为结尾符的文件名：（-z）
```bash
grep "test" file* -lZ| xargs -0 rm
```
<a name="Ruaci"></a>
### 多模式 Grep 命令
`grep` 命令支持三种正则表达式语法：**Basic**、**Extended** 和 **Perl-compatible** 。当没有指定正则表达式类型时，`grep` 命令将搜索模式默认为基本正则表达式。<br />要搜索多个匹配模式，可以使用 **OR** ( **alternation** ) 运算符。可以用 **OR** 运算符 **|**（ **pipe** ）指定不同的匹配项，这些匹配项可以是文本字符串，也可以是表达式集。值得注意的是，在所有正则表达式运算符中，这个运算符的优先级是最低的。<br />使用 `grep` 命令基本正则表达式搜索多个匹配模式的语法如下：
```bash
$ grep 'pattern1\|pattern2' filename
```
这里需要注意的是，始终要用单引号将正则表达式括起来，因为单引号内的内容原样输出，被单引号括起的内容不管是常量还是变量不会发生替换。<br />使用基本正则表达式时，元字符被解释为字面字符。要保留元字符的特殊含义，必须用反斜杠（ \ ）对它们进行转义。这就是为什么要转义 **OR** 运算符（**|**）。<br />要将模式解释为扩展正则表达式，请调用 `grep -E`（或 `--extended-regexp` ）选项。使用扩展正则表达式时，不需要对 **OR** 运算符 (**|**) 进行转义：
```bash
$ grep -E 'pattern1|pattern2' file
```
<a name="on79N"></a>
### Grep命令搜索多个字符串
通常认为，文字字符串是最基本的模式。<br />示例，搜索某用户日志错误文件中出现的所有 **fatal**、**error** 和 **critical** 字符串。语法如下：
```bash
$ grep 'fatal\|error\|critical' /var/log/nginx/error.log
```
还需要注意的是，如果要搜索的字符串包含空格，需要用双引号将其括起来。<br />下面是使用扩展正则表达式的同一个示例，它不需要转义字符:
```bash
$ grep -E 'fatal|error|critical' /var/log/nginx/error.log
```
默认情况下，`grep` 命令是区分大小写的。要在搜索时忽略大小写，请调用 `grep` 加 `-i` （或 `--ignore-case` ）选项，示例如下：
```bash
$ grep -i 'fatal|error|critical' /var/log/nginx/error.log
```
当只想搜索某个单词时，比如想搜索的是单词 `error` ，`grep` 命令会输出所有包含 `error` 字符串的行，即它除了会输出包含 `error` 单词的行，还会输出包含 `errorless` 或 `antiterrorists` 等非 `error` 单词的行，这样是极不方便的。<br />因此要仅返回指定字符串是整词的行，或者是由非单词字符括起来的行，可以使用 `grep` 加 `-w` （或 `--word-regexp` ）选项：
```bash
$ grep -w 'fatal|error|critical' /var/log/nginx/error.log
```
值得注意的是，单词字符包括有字母、数字字符（比如 a-z、a-Z 和 0-9 ）以及下划线（ _ ），所有其他字符都被视为非单词字符。
<a name="YWJfE"></a>
## xargs–过滤器、命令行参数转换
xargs命令是给其他命令传递参数的一个过滤器，也是组合多个命令的一个工具。全称是“extended arguments”，它擅长将标准输入数据转换成命令行参数，xargs能够处理管道或者 stdin 并将其转换成特定命令的命令参数。<br />xargs也可以将单行或多行文本输入转换为其他格式，例如多行变单行，单行变多行。xargs的默认命令是 echo，空格是默认定界符。这意味着通过管道传递给 xargs的输入将会包含换行和空白，不过通过 xargs 的处理，换行和空白将被空格取代。xargs是构建单行命令的重要组件之一。<br />语法格式：xargs [参数]<br />常用参数：

| -n | 多行输出 |
| --- | --- |
| -d | 自定义一个定界符 |
| -I | 指定一个替换字符串{} |
| -t | 打印出 xargs 执行的命令 |
| -p | 执行每一个命令时弹出确认 |

xargs 能够将输入数据转化为特定命令的命令行参数；这样，可以配合很多命令来组合使用。比如grep，比如find；将多行输出转化为单行输出
```bash
cat file.txt| xargs
```
是多行文本间的定界符将单行转化为多行输出
```bash
cat single.txt | xargs -n 3
```
-n：指定每行显示的字段数xargs参数说明<br />-d 定义定界符 （默认为空格 多行的定界符为 ）<br />-n 指定输出为多行<br />-I {} 指定替换字符串，这个字符串在xargs扩展时会被替换掉，用于待执行的命令需要多个参数时<br />eg：
```bash
cat file.txt | xargs -I {} ./command.sh -p {} -1
```
-0：指定为输入定界符<br />eg：统计程序行数
```bash
find source_dir/ -type f -name "*.cpp"-print0 |xargs -0 wc -l
```
<a name="HkY1B"></a>
### 其他示例
```bash
grep forest f.txt     #文件查找
grep forest f.txt cpf.txt #多文件查找
grep 'log' /home/admin -r -n #目录下查找所有符合关键字的文件
cat f.txt | grep -i shopbase    
grep 'shopbase' /home/admin -r -n --include *.{vm,java} #指定文件后缀
grep 'shopbase' /home/admin -r -n --exclude *.{vm,java} #反匹配
seq 10 | grep 5 -A 3    #上匹配
seq 10 | grep 5 -B 3    #下匹配
seq 10 | grep 5 -C 3    #上下匹配，平时用这个就妥了
cat f.txt | grep -c 'SHOPBASE'
```
<a name="Cym9A"></a>
## sort-排序
字段说明：<br />-n 按数字进行排序 VS -d 按字典序进行排序<br />-r 逆序排序<br />-k N 指定按第N列排序<br />eg：
```bash
sort -nrk 1 data.txt
sort -bd data // 忽略像空格之类的前导空白字符
```
<a name="c9d47c3f"></a>
## uniq-消除重复行
uniq命令全称是“unique”，中文释义是“独特的，唯一的”。该命令的作用是用来去除文本文件中连续的重复行，中间不能夹杂其他文本行。去除了重复的，保留的都是唯一的，也就是独特的，唯一的了。<br />应当注意的是，它和sort的区别，sort只要有重复行，它就去除，而uniq重复行必须要连续，也可以用它忽略文件中的重复行。<br />语法格式：uniq [参数] [文件]<br />常用参数：

| -c | 打印每行在文本中重复出现的次数 |
| --- | --- |
| -d | 只显示有重复的纪录，每个重复纪录只出现一次 |
| -u | 只显示没有重复的纪录 |

<a name="Fl926"></a>
### 消除重复行
```bash
sort unsort.txt | uniq
```
<a name="VRBG7"></a>
### 统计各行在文件中出现的次数
```bash
sort unsort.txt | uniq -c
```
<a name="c4627a8c"></a>
### 找出重复行
```bash
sort unsort.txt | uniq -d
```
可指定每行中需要比较的重复内容：-s 开始位置 -w 比较字符数
<a name="DpBRk"></a>
## tr-字符转换
tr的英文全称是“ transform ”，即转换的意思。该命令的作用是一种可将字符进行替换、压缩、删除，他可以将一组字符转换成另一组字符。tr他只能从标准输入中读取数据，因此，tr要么将输入文件重定向到标准输入，要么从管道读入数据。<br />注意：tr类似于sed命令，但是比sed简单，所以tr能实现的功能，sed都能实现。<br />语法格式：tr [参数] [字符串1] [字符串2]<br />常用参数：

| -c | 选定字符串1中字符集的补集，即反选字符串1的补集 |
| --- | --- |
| -d | 删除字符串1中出现的所有字符 |
| -s | 删除所有重复出现的字符序列，只保留一个 |

<a name="Nl9IH"></a>
### 通用用法
```bash
echo 12345| tr '0-9''9876543210'//加解密转换，替换对应字符
cat text| tr '    '' '//制表符转空格
```
<a name="skaf5"></a>
### tr删除字符
```bash
cat file | tr -d '0-9'// 删除所有数字
```
<a name="pEXW8"></a>
### -c 求补集
```bash
cat file | tr -c '0-9'//获取文件中所有数字
cat file | tr -d -c '0-9 '//删除非数字数据
```
<a name="Em3G9"></a>
### tr压缩字符tr -s 压缩文本中出现的重复字符；最常用于压缩多余的空格
```bash
cat file | tr -s ' '
```
<a name="aHvMi"></a>
### 字符类tr中可用各种字符类
alnum：字母和数字<br />alpha：字母<br />digit：数字<br />space：空白字符<br />lower：小写<br />upper：大写<br />cntrl：控制（非可打印）字符<br />print：可打印字符<br />使用方法：
```bash
tr [:class:] [:class:]
```
eg: 
```bash
tr '[:lower:]''[:upper:]'
```
<a name="oP3Ba"></a>
## paste-按列拼接文本
<a name="5eKAy"></a>
### 将两个文本按列拼接到一起
```bash
> cat file1
1
2
> cat file2
colin
book
> paste file1 file2
1 colin
2 book
```
<a name="eiV89"></a>
### 默认的定界符是制表符，可以用-d指明定界符
```bash
paste file1 file2 -d ","
1,colin
2,book
```
<a name="51ff6c18"></a>
## wc-统计行和字符的工具
wc命令全称为“Word Count”，是统计指定文件中的字节数、字数、行数，并将统计结果显示输出。利用wc指令可以计算文件的Byte数、字数或是列数，若不指定文件名称，或是所给予的文件名为“-”，则wc指令会从标准输入设备读取数据。wc同时也给出所指定文件的总统计数。<br />语法格式：wc [参数] [文件]<br />常用参数：

| -w | 统计字数，或--words：只显示字数。一个字被定义为由空白、跳格或换行字符分隔的字符串 |
| --- | --- |
| -c | 统计字节数，或--bytes或--chars：只显示Bytes数 |
| -l | 统计行数，或--lines：只显示列数 |
| -m | 统计字符数 |


```bash
wc -l file // 统计行数
wc -w file // 统计单词数
wc -c file // 统计字符数
```
<a name="XZRaY"></a>
## gawk–模式扫描与处理语言
gawk全称是为“GNUAWK”，是Unix中原始awk程序的GNU版本，强大之处在于可以写脚本来读取文本行的数据，然后处理并显示数据。<br />语法格式：gawk [参数]<br />常用参数：

| -f | 从文件程序文件读取AWK程序源，而不是从第一个命令行参数。可以使用多个-f（或--file）选项 |
| --- | --- |
| -F | 指定描绘一行中数据字段的文件分隔符 |
| -v | 定义gawk程序中使用的变量和默认值 |
| -mr | 指定数据文件中的最大记录大小 |

<a name="wqd0H"></a>
## fmt–编排文本文件
这个命令是单词“format”的缩写。fmt命令会从指定的文件里读取内容，将其依照指定格式重新编排后，输出到标准输出设备。若指定的文件名为”-“，则fmt指令会从标准输入设备读取数据。<br />语法格式：fmt [参数] [文件]<br />常用参数：

| -c | 每段前两列缩排 |
| --- | --- |
| -s | 只拆开字数超出每列字符数的列，但不合并字数不足每列字符数的列 |
| -t | 每列前两列缩排，但第1列和第2列的缩排格式不同 |
| -u | 每个字符之间都以一个空格字符间隔，每个句子之间则两个空格字符分隔 |
| -w | 设置每列的最大字符数 |

<a name="7MAFB"></a>
## ed–文本编辑
ed是英文单词“editor”的缩写，是Linux中功能最简单的文本编辑程序，一次仅能编辑一行而非全屏幕方式的操作。<br />ed命令并不是一个常用的命令，一般使用比较多的是vi 指令。但ed文本编辑器对于编辑大文件或对于在shell脚本程序中进行文本编辑很有用。<br />语法格式：ed [参数]<br />常用参数：

| -G | 提供回兼容的功能 |
| --- | --- |
| -p | 指定ed在command mode的提示字符 |
| -s | 不执行开启文件时的检查功能 |
| --help  | 显示帮助信息 |
| --version  | 显示版本信息 |

<a name="ML9lP"></a>
## egrep–在文件内查找指定的字符串
egrep的全称是“ExtendedGREP”，egrep命令作用是在文件内查找指定的字符串。<br />egrep执行效果与grep -E相似，使用的语法及参数可参照grep指令，与grep的不同点在于解读字符串的方法。<br />egrep命令为 grep 的扩充版本，改良了许多传统 grep 不能或不便的操作. 比方说:<br />grep之下不支持 ? 与 + 这两种modifier，但egrep可以。<br />grep不支持 a|b 或 (abc|xyz) 这类”或一”比对，但egrep可以。<br />grep在处理 {n,m} 时，需用 \{ 与 \} 处理，但egrep则不需要。<br />语法格式：egrep [参数] [文件]<br />常用参数：

| -i | 搜索时，忽略大小写 |
| --- | --- |
| -c | 只输出匹配行的数量 |
| -l | 只列出符合匹配的文件名，不列出具体的匹配行 |
| -n | 列出所有的匹配行，显示行号 |

<a name="7KqDJ"></a>
## fgrep–为文件搜索文字字符串
全称为“Fixed GREP”。fgrep命令的作用是用来搜索file参数指定的输入文件（缺省为标准输入）中的匹配模式的行。fgrep 命令特别搜索Pattern参数，它们是固定的字符串。如果在File 参数中指定一个以上的文件fgrep命令将显示包含匹配行的文件。<br />fgrep命令 作用与grep和egrep命令不同，因为它搜索字符串而不是搜索匹配表达式的模式。fgrep命令使用快速的压缩算法。$, *, [, |, (, )和\等字符串被fgrep命令按字面意思解释。这些字符并不解释为正则表达式。<br />如果没有指定文件，fgrep命令假定标准输入。一般，找到的每行都复制到标准输出中去。如果不止一个输入文件，则在找到的每行前打印文件名。<br />fgrep有以下相关属性:

- fgrep 命令和带     -F 标志的 grep命令是一样的但出错和用法消息不同-s 标志功能也不同。
- 每行限制在 2048 个字节。
- 段落（-p 标志下）目前限制在5000个字符的长度。
- 不要在特定的文件中运行 grep 命令，因为会产生不可预料的结果。
- 输入行不能包含空字符。
- 输入文件应该以换行字符结尾。
- 虽然可以同时指定很多标志，但某些标志会覆盖其余的标志。例如，如果同时指定 -l 和 -n ，只有文件名写入到标准输出中去。

语法格式: fgrep[参数]<br />常用参数：

| -b | 在找到的每行之前添加行所在的块编号 |
| --- | --- |
| -c | 仅显示匹配行的计数 |
| -e | 指定模式 |
| -f | 指定包含字符串的文件 |

<a name="Q0wSR"></a>
## emacs–功能强大的文本编辑器
emacs全称是“EditorMACroS”，这个命令是由GNU组织的创始人Richard Stallman开发的一个功能强大的全屏文本编辑器，它支持多种编程语言，具有很多优良的特性。有众多的系统管理员和软件开发者使用emacs。<br />语法格式：emacs [参数]<br />常用参数:

| -q | 启动emacs编辑器，而不加载初始化文件 |
| --- | --- |
| -u | 启动emacs编辑器时，加载指定用户的初始化文件 |
| -l | 加载指定的lisp代码文件 |
| -batch | 以批处理模式运行emacs编辑器 |

<a name="1a6bbc61"></a>
## joe–纯文本编辑器
joe命令用于编辑文本文件。全称是“Joe's Own Editor”，joe是一个功能强大的全屏幕文本编辑程序。操作的复杂度要比Pico高一点，但是功能较为齐全。joe一次可开启多个文件，每个文件各放在一个编辑区内，并可在文件之间执行剪贴的动作。<br />语法格式：joe [参数]<br />常用参数：

| -columns | 设置栏数 |
| --- | --- |
| -help | 执行程序时一并显示帮助 |
| -lines | 设置行数 |
| -marking | 在选取区块时，反白区块会随着光标移动 |


