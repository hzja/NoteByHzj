Linux

- 一、cat命令：
- 二、more命令：
- 三、less命令：
- 四、head命令：
- 五、tail命令：
- 六、tac命令：
- 七、echo命令：
- 八、grep命令：
- 九、sed命令：
- 混合命令：
- 附加：

当日志存储文件很大时，就不能用 vi 直接去查看日志了，就需要Linux的一些内置命令去查看日志文件.<br />系统Log日志位置：<br />/var/log/message 系统启动后的信息和错误日志，是Red Hat Linux中最常用的日志之一<br />/var/log/secure 与安全相关的日志信息<br />/var/log/maillog 与邮件相关的日志信息<br />/var/log/cron 与定时任务相关的日志信息<br />/var/log/spooler 与UUCP和news设备相关的日志信息<br />/var/log/boot.log 守护进程启动和停止相关的日志消息
<a name="KhROu"></a>
## 一、cat命令
参数：<br />-n 或 --number 由 1 开始对所有输出的行数编号<br />-b 或 --number-nonblank 和 -n 相似，只不过对于空白行不编号<br />-s 或 --squeeze-blank 当遇到有连续两行以上的空白行，就代换为一行的空白行<br />-v 或 --show-nonprinting<br />-E --show-ends 在每行结束处显示 $<br />-e --等价于-vE<br />cat主要有三大功能：<br />1. `$ cat filename` 一次显示整个文件。<br />2. `$ cat > filename` 从键盘创建一个文件。（只能创建新文件,不能编辑已有文件）<br />3. `$ cat filename1 filename2 > filename` 将几个文件合并为一个文件（如果原本file文件中有内容，会被覆<br />   盖掉） 盖掉） <br />例：<br />把 file1 的内容加上行号后输入到 file2 这个文件里  
```bash
cat -n filename1 > filename2
```
把 file1 和 file2 的内容加上行号（空白行不加）之后将内容追加到 file3 里
```bash
cat -b filename1 filename2 >> filename3
```
把test.txt文件扔进垃圾箱，赋空值test.txt
```bash
cat /dev/null > /etc/test.txt 
```
注意：>意思是创建，>>是追加。千万不要弄混了。
<a name="XzhUp"></a>
## 二、more命令
more 命令是一个基于vi编辑器文本过滤器，它以全屏幕的方式按页显示文本文件的内容，支持vi中的关键字定位操作。<br />该命令一次显示一屏文本信息，满屏后停下来，以百分比的形式，以上下翻页，以上下行移动显示查看日志并且在屏幕<br />的底部给出一个提示信息，从开始至当前己显示的该文件的百分比：–More–（XX%）<br /> **按键   **说明**<br /> 按Space键：显示文本的下一屏内容。<br /> 按B键：显示上一屏内容。<br /> 按Enter键：只显示文本的下一行内容。<br /> 按斜线符：接着输入一个模式，可以在文本中寻找下一个相匹配的模式。<br /> 按H键：显示帮助屏，该屏上有相关的帮助信息。<br /> 按Q键：退出more命令
<a name="FiknP"></a>
## 三、less命令
less 命令查看日志，和more命令类似，只不过less支持上下键前后翻阅文件。
<a name="YR37x"></a>
## 四、head命令
参数：<br />-q 隐藏文件名<br />-v 显示文件名<br />-c 显示字节数<br />-n 显示的行数<br />从文本文件的头部开始查看，head 命令用于查看一个文本文件的开头部分。<br />例：<br />`head filename` 或 `head -n 10` 显示文本文件 file 的前十行内容，然后退出命令<br />`head -n 20 filename` 显示文本文件 file 的前二十行内容<br />`head -n -10 filename` 显示文本文件除了最后10行的其他所有文本文件信息
<a name="XOeyL"></a>
## 五、tail命令
tail 命令用于显示文本文件的末尾内容（默认10行，相当于增加参数 -n 10），并且实时不断有内容被打印出来，<br />  若想中断进程，使用命令 Ctrl-C<br />参数：
```bash
tail [ -f ] [ -c Number | -n Number | -m Number | -b Number | -k Number ] [ File ] 
```
参数解释：<br />-f 该参数用于监视File文件增长。<br />-c Number 从 Number 字节位置读取指定文件 <br />-n Number 从 Number 行位置读取指定文件。<br />-m Number 从 Number 多字节字符位置读取指定文件，比如文件假设包括中文字，假设指定-c参数，可能导致<br />   截断，但使用-m则会避免该问题。<br />-b Number 从 Number 表示的512字节块位置读取指定文件。<br />-k Number 从 Number 表示的1KB块位置读取指定文件。<br />File 指定操作的目标文件名称 <br />上述命令中，都涉及到number，假设不指定，默认显示10行。Number前面可使用正负号，表示该偏移从顶部还是从尾<br />  部开始计算。<br />tail 可运行文件一般在/usr/bin/以下。<br />`tail -f filename` 监视filename文件的尾部内容（默认10行，相当于增加参数 -n 10）<br />`tail -100f filename` 监视filename文件的尾部内容（默认从底部往前100行，相当于增加参数 -n 100）<br />`tail -n 20 filename` 显示filename最后20行<br />`tail -r -n 10 filename` 逆序显示filename最后10行
<a name="A0OwU"></a>
## 六、tac命令
tac (反向查看日志，会打开整个文件，倒序显示，不常用)<br />tac 是将 cat 反写过来，所以他的功能就跟 cat 相反。<br />cat 是由第一行到最后一行连续显示在屏幕上，而 tac 则是由最后一行到第一行反向在萤幕上显示出来
<a name="UUPvS"></a>
## 七、echo命令
echo 命令用来在标准输出上显示一段字符<br />echo [ -n ] 字符串其中选项n表示输出文字后不换行；字符串能加引号，也能不加引号<br />`echo "the echo command test!"`<br />`echo "the echo command test!">filename` 输出内容到文件<br />用 echo 命令输出加引号的字符串时，将字符串原样输出<br />用 echo 命令输出不加引号的字符串时，将字符串中的各个单词作为字符串输出，各字符串之间用一个空格分割
<a name="iYH3u"></a>
## 八、grep命令
grep 同时满足多个关键字和满足任意关键字，是一种强大的文本搜索工具，它能使用正则表达式搜索文本，并把匹 <br />  配的行打印出来。grep全称是Global Regular Expression Print，表示全局正则表达式版本，显示完自动退<br />  出命令<br />grep [options]  <br />参数:  <br />[options]参数：<br />-c：只输出匹配行的计数<br />-I：不区分大 小写(只适用于单字符)<br />-h：查询多文件时不显示文件名<br />-l：查询多文件时只输出包含匹配字符的文件名<br />-n：显示匹配行及 行号<br />-s：不显示不存在或无匹配文本的错误信息<br />-v：显示不包含匹配文本的所有行<br />-A: 显示匹配行及前面多少行, 如: -A3, 则表示显示匹配行及前3行<br />-B: 显示匹配行及后面多少行, 如: -B3, 则表示显示匹配行及后3行<br />-C: 显示匹配行前后多少行, 如: -C3, 则表示显示批量行前后3行<br />pattern正则表达式主要参数：<br />：忽略正则表达式中特殊字符的原有含义<br />^：匹配正则表达式的开始行<br />$: 匹配正则表达式的结束行<br /><：从匹配正则表达 式的行开始<br />>：到匹配正则表达式的行结束<br />[ ]：单个字符，如[A]即A符合要求 <br />[ - ]：范围，如[A-Z]，即A、B、C一直到Z都符合要求 <br />。：所有的单个字符<br />- ：有字符，长度可以为0<br />例<br />`grep -n "word" filename` 查看文件包含条件的日志，全部显示出来（单引号或者双引号都可以，不区分）<br />`grep -E "word1|word2|word3" filename` 满足任意条件（word1、word2和word3之一）将匹配的内容全部打<br />  印出来<br />`grep word1 filename | grep word2 |grep word3` 必须同时满足三个条件（word1、word2和word3）才匹配<br />  多管道，多次筛选<br />使用正则表达式 -E 选项<br />`grep -E "[1-9]+" 或 egrep "[1-9]+"`<br />`grep -A100 'word' filename` 显示匹配行往后100行<br />`grep -B100 'word' filename` 显示匹配行往前100行<br />`grep -C100 'word' filename` 显示匹配行往前往后100行
<a name="mnxUA"></a>
## 九、sed命令
sed 本身是一个管道命令，主要是以行为单位进行处理，可以将数据行进行替换、删除、新增、选取等特定工作<br />参数<br />-n∶使用安静(silent)模式。在一般 sed 的用法中，所有来自 STDIN的资料一般都会被列出到萤幕上。但如果加<br />  上 -n 参数后，则只有经过sed 特殊处理的那一行(或者动作)才会被列出来。<br />-p ∶列印，亦即将某个选择的资料印出。通常 p 会与参数 sed -n 一起运作～<br />-s ∶取代，可以直接进行取代的工作哩！通常这个 s 的动作可以搭配正规表示法！例如 1,20s/old/new/g<br />例<br />`sed -n '5,10p' filename` 只查看文件的第5行到第10行<br />`sed -n '/2019-01-04 21:30:00/,/2019-01-04 22:30:30/p' filename` 只查看文件包含时间段的区间内容
<a name="SlA2A"></a>
## 混合命令
`tail -n +92`表示查询92行之后的日志<br />`tail filename -n 300 -f` 查看底部即最新300条日志记录，并实时刷新<br />`tail -f filename | grep -E 'word1|word2|word3'` 实时打印出匹配规则的文件内容（注意或符号前后最好<br />  不要有空格）<br />`cat -n filename |grep "地形" | more` 得到关键日志的行号<br />`cat -n filename |tail -n +92|head -n 20`<br />`grep 'nick' | tail filename -C 10` 查看字符‘nick’前后10条日志记录, 大写C<br />`head -n 20` 则表示在前面的查询结果里再查前20条记录
<a name="L2BXQ"></a>
## 附加：
vi filename 查看或编辑文件<br />查找文件内容关键字方法：<br />先 执行命令>：`vi filename`<br />然后输入>:   /查找字符串 <br />按n查找下一个<br />例<br />如查找nohup.out日志文件的error关键字：<br />执行命令：`vi  nohup.out`<br />输入以下回车：/error<br />按n查找下一个<br />将实时日志打印到指定文件：<br />例<br />将实时日志打印到文件newlog.log内，方便查找<br />执行命令：`tail  -f  nohup.out   >newlog.log`<br />备注：newlog.log文件可以不存在，命令执行时会自动新建
