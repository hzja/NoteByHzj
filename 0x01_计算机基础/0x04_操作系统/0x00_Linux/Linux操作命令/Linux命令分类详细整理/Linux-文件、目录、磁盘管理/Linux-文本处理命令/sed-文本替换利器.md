<a name="WlHvN"></a>
### 参数
```bash
-e<script>或--expression=<script> 以选项中指定的script来处理输入的文本文件。
-f<script文件>或--file=<script文件> 以选项中指定的script文件来处理输入的文本文件。
-h或--help 显示帮助。
-n或--quiet或--silent 仅显示script处理后的结果。
-V或--version 显示版本信息。
```
```bash
d  删除选择的行    
s  查找    
y  替换
i  当前行前面插入一行
a  当前行后面插入一行
p  打印行       
q  退出     

替换符:

数字：替换第几处    
g：全局替换    
\1：子串匹配标记，前面搜索可以用元字符集\(..\)
&：保留搜索刀的字符用来替换其他字符
```
<a name="ybwPK"></a>
### 动作

- a：新增， a 的后面可以接字串，而这些字串会在新的一行出现(目前的下一行)～
- c：取代， c 的后面可以接字串，这些字串可以取代 n1,n2 之间的行！
- d：删除，因为是删除啊，所以 d 后面通常不接任何东东；
- i：插入， i 的后面可以接字串，而这些字串会在新的一行出现(目前的上一行)；
- p：打印，亦即将某个选择的数据印出。通常 p 会与参数 sed -n 一起运行～
- s：取代，可以直接进行取代的工作哩！通常这个 s 的动作可以搭配正规表示法！例如 1,20s/old/new/g 就是啦
<a name="LHsgw"></a>
### sed元字符
```
^ # 匹配行开始，如：/^sed/匹配所有以sed开头的行。
$ # 匹配行结束，如：/sed$/匹配所有以sed结尾的行。
. # 匹配一个非换行符的任意字符，如：/s.d/匹配s后接一个任意字符，最后是d。
* # 匹配0个或多个字符，如：/*sed/匹配所有模板是一个或多个空格后紧跟sed的行。
[] # 匹配一个指定范围内的字符，如/[sS]ed/匹配sed和Sed。  
[^] # 匹配一个不在指定范围内的字符，如：/[^A-RT-Z]ed/匹配不包含A-R和T-Z的一个字母开头，紧跟ed的行。
\(..\) # 匹配子串，保存匹配的字符，如s/\(love\)able/\1rs，loveable被替换成lovers。
& # 保存搜索字符用来替换其他字符，如s/love/ **&** /，love这成 **love** 。
\< # 匹配单词的开始，如:/\<love/匹配包含以love开头的单词的行。
\> # 匹配单词的结束，如/love\>/匹配包含以love结尾的单词的行。
x\{m\} # 重复字符x，m次，如：/0\{5\}/匹配包含5个0的行。
x\{m,\} # 重复字符x，至少m次，如：/0\{5,\}/匹配至少有5个0的行。
x\{m,n\} # 重复字符x，至少m次，不多于n次，如：/0\{5,10\}/匹配5~10个0的行。
```
<a name="u7QGr"></a>
### 向文本中添加一行
```bash
cat << EOF > testfile | sed -e 4a\newLine testfile
HELLO LINUX!  
Linux is a free unix-type opterating system.  
This is a linux testfile!  
Linux test 
Google
Taobao
Runoob
Tesetfile
Wiki
EOF
HELLO LINUX!  
Linux is a free unix-type opterating system.  
This is a linux testfile!  
Linux test 
newLine
Google
Taobao
Runoob
Tesetfile
Wiki
```
<a name="PKGq8"></a>
### 首处替换
```bash
seg 's/text/replace_text/' file   //替换每一行的第一处匹配的text
```
<a name="EMJgB"></a>
### 全局替换
```bash
seg 's/text/replace_text/g' file
```
<a name="uu10r"></a>
### 打印指定行
```bash
sed -n '5p' file.txt
```
<a name="PzzPY"></a>
### 删除特定行
如果想删除文件中的特定行，可以使用以下命令：
```bash
sed 'nd' filename
```
在该命令中，n是要删除的行号，filename是文件名。例如，在删除文件中的第3行之前，运行以下命令：
```bash
sed '3d' filename
```
如果要删除从第3行到第5行的内容，可以使用下面的命令：
```bash
sed '3,5d' filename
```
<a name="pyXhX"></a>
### 在原文件中进行修改
默认替换后，输出替换后的内容，如果需要直接替换原文件，使用`-i`<br />到目前为止，只是在输出中看到了删除后的内容，而没有在原文件中进行修改。要在原文件中进行修改，可以使用`-i`选项：
```bash
sed -i 'nd' filename
```
例如，要删除文件中的第3行并在原文件上进行修改，可以运行以下命令：
```bash
sed -i '3d' filename
```
```bash
sed -i 's/text/repalce_text/g' file
```
请记住，在使用`-i`选项时，请务必小心，因为会直接修改原文件。
<a name="m4LiD"></a>
### 移除空白行
```bash
sed '/^$/d' file
```
在该命令中，`^$`表示一个空行。`/pattern/d`会删除匹配pattern的行。
<a name="yF5V3"></a>
### 删除包含特定模式的行
如果想删除文件中包含特定模式的行，可以使用以下命令：
```bash
sed '/pattern/d' filename
```
在该命令中，pattern是要匹配的模式。/pattern/d会删除包含pattern的行。
<a name="N14wA"></a>
### 执行多个编辑操作
```bash
sed -e 's/one/two/g' -e 's/three/four/g' file.txt
```
<a name="xSfnH"></a>
### 变量转换已匹配的字符串通过标记&来引用
```bash
echo this is en example | seg 's/w+/[&]/g'
$>[this]  [is] [en] [example]
```
<a name="ilK1I"></a>
### 子串匹配标记第一个匹配的括号内容使用标记 来引用
```bash
sed 's/hello([0-9])//'
```
<a name="n0rdG"></a>
### 双引号求值sed通常用单引号来引用；也可使用双引号，使用双引号后，双引号会对表达式求值
```bash
sed 's/$var/HLLOE/'
```
<a name="ef743c0b"></a>
### 当使用双引号时，可以在sed样式和替换字符串中指定变量
eg:
```bash
p=patten
r=replaced
echo "line con a patten"| sed "s/$p/$r/g"
$>line con a replaced
```
<a name="UegG4"></a>
### 其它示例字符串插入字符：将文本中每行内容（PEKSHA） 转换为 PEK/SHA
```bash
sed 's/^.{3}/&//g' file
```
这就是使用sed命令删除文件中行的基本方法。sed具有更多高级用法和功能，可以进一步扩展处理文本的能力。通过查看sed的文档，可以深入了解其更多功能和选项。
