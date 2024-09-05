Linux Shell
<a name="Rujrt"></a>
## 1、为何叫做 Shell？
计算机的运作离不开硬件，但却无法直接操作硬件，硬件的驱动只能通过一种称之为“操作系统（OS，Opertating System）的软件来管控。Linux 严格来说是一个操作系统（OS）。<br />使用者没有办法直接操作 Kernel，而是通过 Kernel 的“外壳”程序，也就是所谓的 Shell 来与 Kernel 沟通。Shell 是一个使用者与系统的交互界面（Interface），只能通过命令行（Command line）来使用系统来完成工作。因此 Shell 最简单的定义就是：命令解译器（Command Interpreter）

- 将使用者的命令翻译给内核处理；
- 同时，将内核处理结果翻译给使用者。

不同的 OS 使用不同的 Kernel；同一个 kernel 之上，也可以使用不同的 Shell，常见的 Shell 有sh、bash、csh、ksh 等
<a name="HF8In"></a>
## 2、Shell prompt（PS1）与 Carriage Return（CR）的关系？
成功登录一个 Shell 终端后，光标左边部分称之为提示符 Prompt，通常一般用户使用`$`，管理员用户使用`#`

- Shell Prompt：可以输入命令了，键入命令后，直到读进 CR（Carriage Return）字符为止
- Carriage Return：可以执行命令了

若从技术的细节来看，Shell 会依据 IFS（Internal Field Seperator）将 Command line 所输入的文字拆解为”字段”（word/field）。然后再针对特殊字符（meta）先作处理，最后重组整行command line。
<a name="c0dQV"></a>
## 3、echo 知多少?
echo 将 argument送到标准输出（stdout），通常显示在屏幕<br />stdin 标准输入<br />stdout 标准输出<br />stderr 标准错误输出
```bash
echo -n  # 取消换行符
echo -e  # 启用反斜杠转译
```
<a name="J9xJK"></a>
## 4、双引号`""`与单引号`''`有什么区别?

- hard quote：`''`（单引号），关闭所有引用
- soft quote：`""`（双引号），保留$引用
<a name="NxPqa"></a>
## 5、var=value? export 前后差别在哪？

- 变量定义：`name=value`，等号左右两边不能使用分隔符。
- 变量替换：`echo ${name}`
- `export`变量：`export name=value`，使变量成为环境变量
```bash
# 本地变量
A=B
# 取消变量
unset A
# 环境变量export A=B
```
<a name="spEQg"></a>
## 6、`exec` 跟 `source` 差别在哪？
环境变量只能从父进程到子进程单向传递。换句话说：在子进程中环境如何变更，均不会影响父进程的环境。<br />当执行一个shell script时，其实是先产生一个sub-shell的子进程， 然后sub-shell再去产生命令行的子进程。
```bash
# 创建子shell执行脚本
./1.sh
# 当前shell执行
source 1.sh
# 当前shell执行后退出
exec 1.sh
```
<a name="Y8mv5"></a>
## 7、`( )` 与 `{ }` 差别在哪？
`( )`将 command group 置于 sub-shell 执行<br />`{ }`则是在同一个shell内完成
<a name="whU0m"></a>
## 8、`$(())` 与 `$()` 还有 `${}` 差别在哪？
```bash
# 假设定义了一个变量为：
file=/dir1/dir2/dir3/my.file.txt
# 可以用 ${ } 分别替换获得不同的值：

# 1. shell字符串的非贪婪(最小匹配)左删除
${file#*/} # 拿掉第一条 / 及其左边的字符串：dir1/dir2/dir3/my.file.txt
# 2. shell字符串的贪婪(最大匹配)左删除
${file##*/} # 拿掉最后一条 / 及其左边的字符串：my.file.txt
${file##*.} # 拿掉最后一个 . 及其左边的字符串：txt
# 3. shell字符串的非贪婪(最小匹配)右删除：
${file%/*} # 拿掉最后条 / 及其右边的字符串：/dir1/dir2/dir3
${file%.*} # 拿掉最后一个 . 及其右边的字符串：/dir1/dir2/dir3/my.file
# 4. shell字符串的贪婪(最大匹配)右删除：
${file%%/*} # 拿掉第一条 / 及其右边的字符串：(空值)
${file%%.*} # 拿掉第一个 . 及其右边的字符串：/dir1/dir2/dir3/my
```
记忆的方法为：<br />`#` 是去掉左边(在键盘上 `#` 在 `$` 之左边)<br />`%` 是去掉右边(在键盘上 `%` 在 `$` 之右边)<br />单一符号是最小匹配﹔两个符号是最大匹配。
```bash
# 5. shell字符串取子串：
${file:0:5}：提取最左边的 5 个字节：/dir1
${file:5:5}：提取第 5 个字节右边的连续 5 个字节：/dir2

# 6. shell字符串变量值的替换：
${file/dir/path}：将第一个 dir 提换为 path：/path1/dir2/dir3/my.file.txt
${file//dir/path}：将全部 dir 提换为 path：/path1/path2/path3/my.file.txt

# 7. ${}还可针对变量的不同状态(没设定、空值、非空值)进行赋值：
${file-my.file.txt} ：假如 $file 没有设定，则使用 my.file.txt 作传回值。(空值及非空值时不作处理) 
${file:-my.file.txt} ：假如 $file 没有设定或为空值，则使用 my.file.txt 作传回值。(非空值时不作处理)
${file+my.file.txt} ：假如 $file 设为空值或非空值，均使用 my.file.txt 作传回值。(没设定时不作处理)
${file:+my.file.txt} ：若 $file 为非空值，则使用 my.file.txt 作传回值。(没设定及空值时不作处理)
${file=my.file.txt} ：若 $file 没设定，则使用 my.file.txt 作传回值，同时将 $file 赋值为 my.file.txt 。(空值及非空值时不作处理)
${file:=my.file.txt} ：若 $file 没设定或为空值，则使用 my.file.txt 作传回值，同时将 $file 赋值为 my.file.txt 。(非空值时不作处理)
${file?my.file.txt} ：若 $file 没设定，则将 my.file.txt 输出至 STDERR。(空值及非空值时不作处理)
${file:?my.file.txt} ：若 $file 没设定或为空值，则将 my.file.txt 输出至 STDERR。(非空值时不作处理)
```
tips:<br />以上的理解在于，一定要分清楚 `unset` 与 null 及 non-null 这三种赋值状态.<br />一般而言，`:` 与 null 有关，若不带 `:` 的话，null 不受影响，若带 `:` 则连 null 也受影响。
```bash
# 8. 计算shell字符串变量的长度：${#var}
${#var} 可计算出变量值的长度：
${#file} 可得到 27 ，因为 /dir1/dir2/dir3/my.file.txt 刚好是 27 个字节...

# 9. bash数组(array)的处理方法
数组:
A=(a b c d)
引用数组:
${A[@]}
${A[*]}
访问数组成员
${A[0]}
计算数组长度
${#A[@]}
${#A[*]}
数组重新赋值
A[2]=xyz

# 10.$(( ))是用来做整数运算的 
a=5;b=7;c=2;
echo $(( a + b * c))
```
<a name="qWsOU"></a>
## 9、`$@` 与 `$*` 区别在哪？

- `"$@"`则可得到 “p1” “p2 p3” “p4” 这三个不同的词段
- `"$*"`则可得到 “p1 p2 p3 p4” 这一整串单一的词段
<a name="Y04Sg"></a>
## 10、`&&` 与 `||` 差别在哪？
<a name="zOvkQ"></a>
### 1、test命令有两种形式

- test expression
- [ expression ]
<a name="M7Ez2"></a>
### 2、bash的test目前支持三种测试对象

- string：字符串
- integer：整数
- file：文件
<a name="SBMUT"></a>
### 3、当 expression 为真是返回 0（true） ，否则返回 非0（false）

- `command1 && command2`  command2 只有在 command1 的RV为0（True）的条件下执行。
- `command1 || command2`  command2只有在command1的RV为非0(False)的条件下执行。
<a name="IpJQS"></a>
### 4、先替换变量再比较
```bash
A=123[ -n "$A" ] && ([ "$A" -lt 100 ] || echo "too big")unset A
```
<a name="Ixczw"></a>
## 11、`>` 与 `<` 差别在哪？
0: Standard Input（STDIN）<br />1: Standard Output （STDOUT）<br />2: Standard Error Output（STDERR）<br />可用 `<` 来改变读进的数据信道（stdin），使之从指定的档案读进。<br />可用 `>` 来改变送出的数据信道（stdout, stderr），使之输出到指定的档案。
```bash
ls my.file no.such.file 1> file.out 2>file.err
# 2>&1 就是将stderr并进stdout做输出
ls my.file no.such.file 1> file.out 2>&1
# /dev/null 空
ls my.file no.such.file >/dev/null 2>&1

cat < file > file
# 在 IO Redirection 中，stdout 与 stderr 的管道会先准备好，才会从 stdin 读进资料。
# 也就是说，在上例中，> file 会先将 file 清空，然后才读进 < file ， 
# 但这时候档案已经被清空了，因此就变成读不进任何数据了
```
<a name="PBSXB"></a>
## 12、要if还是case呢？
```bash
# if
echo -n "Do you want to continue?(Yes/No):"
read YN
if [ "$YN"=Y -o "$YN"=y -o "$YN"="Yes" -o "$YN"="yes" -o "$YN"="YES"];then
echo "continue"
else
exit 0
fi

# case
echo -n "Do you want to continue?(Yes/No):"
read YN
case "$YN" in
[Yy]|[Yy][Ee][Ss])
echo "continue"
;;
*)
exit 0
esac
```
<a name="qmY60"></a>
## 13、for what? `while`与`until`差别在哪？
```bash
# for
for ((i=1;i<=10;i++))
do
echo "num is $i"
done

# while
num=1
while [ "$num" -le 10 ]; do
echo "num is $num"
num=$(($num + 1))
done

# until
num=1
until [ "$num" -gt 10 ]; do
echo "num is $num"
num=$(($nu + 1))
done
```

- break 是结束 loop
- return 是结束 function
- exit 是结束 script/shell
