AWK 是一种处理文本文件的语言，是一个强大的文本分析工具。<br />之所以叫 AWK 是因为其取了三位创始人 Alfred Aho，Peter Weinberger, 和 Brian Kernighan 的 Family Name 的首字符。
<a name="sjUdT"></a>
### 语法
```bash
awk [选项参数] 'script' var=value file(s)
# 或
awk [选项参数] -f scriptfile var=value file(s)
```
<a name="WJjfW"></a>
### 参数

- -F fs or --field-separator fs<br />指定输入文件折分隔符，fs是一个字符串或者是一个正则表达式，如-F:。
- -v var=value or --asign var=value<br />赋值一个用户定义变量。
- -f scripfile or --file scriptfile<br />从脚本文件中读取awk命令。
- -mf nnn and -mr nnn<br />对nnn值设置内在限制，-mf选项限制分配给nnn的最大块数目；-mr选项限制记录的最大数目。这两个功能是Bell实验室版awk的扩展功能，在标准awk中不适用。
- -W compact or --compat, -W traditional or --traditional<br />在兼容模式下运行awk。所以gawk的行为和标准的awk完全一样，所有的awk扩展都被忽略。
- -W copyleft or --copyleft, -W copyright or --copyright<br />打印简短的版权信息。
- -W help or --help, -W usage or --usage<br />打印全部awk选项和每个选项的简短说明。
- -W lint or --lint<br />打印不能向传统unix平台移植的结构的警告。
- -W lint-old or --lint-old<br />打印关于不能向传统unix平台移植的结构的警告。
- -W posix<br />打开兼容模式。但有以下限制，不识别：/x、函数关键字、func、换码序列以及当fs是一个空格时，将新行作为一个域分隔符；操作符**和**=不能代替^和^=；fflush无效。
- -W re-interval or --re-inerval<br />允许间隔正则表达式的使用，参考(grep中的Posix字符类)，如括号表达式[[:alpha:]]。
- -W source program-text or --source program-text<br />使用program-text作为源代码，可与-f命令混用。
- -W version or --version<br />打印bug报告信息的版本。
<a name="0Q065"></a>
### 基础命令
```bash
awk '{print $4,$6}' f.txt
awk '{print NR,$0}' f.txt cpf.txt    
awk '{print FNR,$0}' f.txt cpf.txt
awk '{print FNR,FILENAME,$0}' f.txt cpf.txt
awk '{print FILENAME,"NR="NR,"FNR="FNR,"$"NF"="$NF}' f.txt cpf.txt
echo 1:2:3:4 | awk -F: '{print $1,$2,$3,$4}'
```
<a name="ObeqV"></a>
### 模式和动作
语法：`awk '{[pattern] action}' {filenames}`  
<a name="f504986f"></a>
#### 匹配
pattern 可以是

- 条件语句
- 正则

模式的两个特殊字段 BEGIN 和 END (不指定时匹配或打印行数)

- BEGIN：一般用来打印列名称。
- END：一般用来打印总结性质的字符。
<a name="c500cfab"></a>
#### 动作
action 在`{}`内指定，一般用来打印，也可以是一个代码段。
```bash
awk '/ldb/ {print}' f.txt   #匹配ldb
awk '!/ldb/ {print}' f.txt  #不匹配ldb
awk '/ldb/ && /LISTEN/ {print}' f.txt   #匹配ldb和LISTEN
awk '$5 ~ /ldb/ {print}' f.txt #第五列匹配ldb
```
<a name="vcw0S"></a>
### 复合表达式
<a name="qYFQa"></a>
#### `&&` `AND`
<a name="rn2k9"></a>
#### `||` `OR`

- `~`      包含
- `!~`      不包含
- `==`     等于，必须全部相等，精确比较
- `!=`      不等于，精确比较
- `&&`     逻辑与
- `||`       逻辑或
- `+`       匹配时表示1个或1个以上
<a name="dbz3d"></a>
### 内建变量
`NR`：`NR`表示从awk开始执行后，按照记录分隔符读取的数据次数，默认的记录分隔符为换行符，因此默认的就是读取的数据行数，`NR`可以理解为Number of Record的缩写。<br />`FNR`：在awk处理多个输入文件的时候，在处理完第一个文件后，NR并不会从1开始，而是继续累加，因此就出现了`FNR`，每当处理一个新文件的时候，FNR就从1开始计数，FNR可以理解为File Number of Record。NF: NF表示目前的记录被分割的字段的数目，NF可以理解为Number of Field。<br />`NF`：记录浏览域的个数，在记录被读后设置。<br />`NR`：已读的记录数。<br />`FS`：设置输入域分隔符<br />`A R G C`：命令行参数个数，支持命令行传入。<br />`RS`：控制记录分隔符<br />`FIlENAME`：awk当前读文件的名称

- `$0`      表示整个当前行
- `$1`      每行第一个字段
- `NF`      字段数量变量
- `NR`      每行的记录号，多文件记录递增
- `FNR`     与NR类似，不过多文件记录不递增，每个文件都从1开始
- `\t`       制表符
- `\n`      换行符
- `FS`      BEGIN时定义分隔符
- `RS`     输入的记录分隔符， 默认为换行符(即文本是按一行一行输入)
- `/0-9+/`   两个或两个以上数字
- `/0-9*/`   一个或一个以上数字
- `OFS`     输出字段分隔符， 默认也是空格，可以改为其他的
- `ORS`    输出的记录分隔符，默认为换行符,即处理结果也是一行一行输出到屏幕
- `-F [:#/]`  定义了三个分隔符
<a name="d9di4"></a>
### awk脚本结构
```bash
awk ' BEGIN{ statements } statements2 END{ statements } '
```
<a name="Snwnu"></a>
### 工作方式

1. 执行begin中语句块；
2. 从文件或stdin中读入一行，然后执行statements
3. 重复这个过程，直到文件全部被读取完毕；
4. 执行end语句块；print 打印当前行使用不带参数的print时，会打印当前行;
```bash
echo -e "line1 line2"| awk 'BEGIN{print "start"} {print } END{ print "End" }'
```
<a name="Iz4kM"></a>
### printf 格式化输出
<a name="9vKSD"></a>
#### 语法
printf ([格式]，参数)<br />printf %x(格式) 具体参数 x代表具体格式

| **符号** | **说明** |
| --- | --- |
| - | 左对齐 |
| Width | 域的步长 |
| .prec | 最大字符串长度或小数点右边位数 |

格式转化符
<a name="17728239"></a>
#### 常用格式
| **符号** | **描述** |
| --- | --- |
| %c | ASCII |
| %d | 整数 |
| %o | 八进制 |
| %x | 十六进制数 |
| %f | 浮点数 |
| %e | 浮点数（科学记数法) |
| % s | 字符串 |
| %g | 决定使用浮点转化e/f |

<a name="msilQ"></a>
#### print 以逗号分割时，参数以空格定界;
```bash
echo | awk ' {var1 = "v1" ; var2 = "V2"; var3="v3";
print var1, var2 , var3; }'
$>v1 V2 v3
```
<a name="D5akM"></a>
### 使用-拼接符的方式（""作为拼接符）;
```bash
echo | awk ' {var1 = "v1" ; var2 = "V2"; var3="v3";
print var1"-"var2"-"var3; }'
$>v1-V2-v3
```
特殊变量：`NR` `NF` `$0` `$1` `$2`<br />`NR`：表示记录数量，在执行过程中对应当前行号；<br />`NF`：表示字段数量，在执行过程总对应当前行的字段数；<br />`$0`：这个变量包含执行过程中当前行的文本内容；<br />`$1`：第一个字段的文本内容；<br />`$2`：第二个字段的文本内容；
```bash
echo -e "line1 f2 f3 line2 line 3"| awk '{print NR":"$0"-"$1"-"$2}'
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1637372734850-6e545257-f08b-4e75-898e-c5c11f4aed62.png#averageHue=%23404040&clientId=udccd354b-5a18-4&from=paste&height=63&id=NjXiv&originHeight=189&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=259271&status=done&style=none&taskId=ue577bc8a-611e-443d-bac1-35da5f49d1e&title=&width=1107.6666666666667)
<a name="0p3Zi"></a>
### 打印每一行的第二和第三个字段
```bash
awk '{print $2, $3}' file
```
<a name="a8b0f063"></a>
### 统计文件的行数
```bash
awk ' END {print NR}' file
```
<a name="Apadn"></a>
### 累加每一行的第一个字段
```bash
echo -e "1 2 3 4 "| awk 'BEGIN{num = 0 ;
print "begin";} {sum += $1;} END {print "=="; print sum }'
```
<a name="0jwSf"></a>
### 传递外部变量
```bash
var=1000
echo | awk '{print vara}' vara=$var #  输入来自stdin
awk '{print vara}' vara=$var file # 输入来自文件
```
<a name="BeOAR"></a>
### 用样式对awk处理的行进行过滤
```bash
awk 'NR < 5'#行号小于5
awk 'NR==1,NR==4 {print}' file #行号等于1和4的打印出来
awk '/linux/'#包含linux文本的行（可以用正则表达式来指定，超级强大）
awk '!/linux/'#不包含linux文本的行
```
<a name="VaP6i"></a>
### 设置定界符使用-F来设置定界符（默认为空格）
```bash
awk -F: '{print $NF}' /etc/passwd
```
<a name="Tlf8q"></a>
### 读取命令输出使用getline，将外部shell命令的输出读入到变量cmdout中；
```bash
echo | awk '{"grep root /etc/passwd" | getline cmdout; print cmdout }'
```
<a name="hY9z1"></a>
### 在awk中使用循环
```bash
for(i=0;i<10;i++){print $i;}
for(i in array){print array[i];}
```
eg:
<a name="YT6Xr"></a>
### 以逆序的形式打印行：(tac命令的实现）
```bash
seq 9|
awk '{lifo[NR] = $0; lno=NR}
	END{ for(;lno>-1;lno--){print lifo[lno];}
}'
```
<a name="groqv"></a>
### awk实现head、tail命令
```bash
head:
  awk 'NR<=10{print}' filename
tail:
  awk '{buffer[NR%10] = $0;} END{for(i=0;i<11;i++){
  print buffer[i %10]} } ' filename
```
<a name="YoJUL"></a>
### 打印指定列awk方式实现
```bash
ls -lrt | awk '{print $6}'
```
<a name="45155228"></a>
### cut方式实现
```bash
ls -lrt | cut -f6
```
<a name="nxi5J"></a>
### 打印指定文本区域确定行号
```bash
seq 100| awk 'NR==4,NR==6{print}'
```
<a name="tMEZZ"></a>
### 确定文本打印处于startpattern 和endpattern之间的文本；
```bash
awk '/start_pattern/, /end_pattern/' filename
```
eg:
```bash
seq 100| awk '/13/,/15/'
cat /etc/passwd| awk '/mai.*mail/,/news.*news/'
```
<a name="Ny7Od"></a>
### awk常用内建函数
`sub(regex,replacement_str,string)`  将正则匹配到的第一处内容替换为replacement_str;<br />`length(s)`  返回s长度<br />`index(s,t)` 返回s中字符串t第一次出现的位置<br />`match (s,r)` s中是否包含r字符串<br />`split(s,a,fs)` 在fs上将s分成序列a<br />`gsub(r,s)` 用s代替r，范围全文本<br />`gsub(r,s,t)` 范围t中，s代替r<br />`substr(s,p)` 返回字符串s从第p个位置开始后面的部分（下标是从1 开始算的，大家可以自己试试）<br />`substr(s,p,n)` 返回字符串s从第p个位置开始后面n个字符串的部分
```bash
echo | awk '{"grep root /etc/passwd" | getline cmdout; print length(cmdout) }'
```
printf 类似c语言中的printf，对输出进行格式化<br />eg：
```bash
seq 10| awk '{printf "->%4s ", $1}'
```
<a name="49ec1"></a>
### 迭代文件中的行、单词和字符
<a name="dknVe"></a>
#### 1、迭代文件中的每一行
<a name="XkSIl"></a>
##### while 循环法
```bash
while read line;
do
echo $line;
done< file.txt
```
<a name="A0D5F"></a>
##### 改成子shell
```bash
cat file.txt | (while read line;do echo $line;done)
```
<a name="vUz2R"></a>
##### awk法
```bash
cat file.txt| awk '{print}'
```
<a name="n74kJ"></a>
#### 2、迭代一行中的每一个单词
```bash
for word in $line;
do
echo $word;
done
```
<a name="WNVbF"></a>
#### 3、迭代每一个字符
`${string:startpos:numof_chars}`：从字符串中提取一个字符；(bash文本切片）`${#word}`：返回变量word的长度
```bash
for((i=0;i<${#word};i++))
do
echo ${word:i:1);
done
```
