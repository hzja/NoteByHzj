Shell 
<a name="yYBSb"></a>
## Shell 简述简介
目前大多数GUN/Linux系统默认的shell环境均采用Bash。
<a name="YA1YB"></a>
### 用户分类
```bash
$ 表示的是普通用户
# 表示管理员用户root
```
<a name="9gZCd"></a>
### shebang

- shell脚本通常是以shebang起始的文本文件，如`#!/bin/bash`
- 在Unix的行话里，用sharp或hash来称呼`#`，用bang来成为`！`
- /bin/bash表示Bash解释器命令的位置，当然也可以使用其他解释器
- Linux内核会自动读取首行并注意到`#!/bin/bash`，运行/bin/bash来执行脚本
<a name="a3kvT"></a>
### 运行方式
将脚本作为bash的命令行参数
```bash
bash script.sh
```
<a name="fc8t5"></a>
### 授予脚本执行权限，变成为可执行文件

- 相对路径
```bash
chmod a+x script.sh
./script.sh
```

- 绝对路径
```bash
chmod a+x script.sh
/home/path/script.sh
```
<a name="iLB1H"></a>
## 命令终端打印
终端是交互的工具，用户可以通过它来与shell环境进行交互，打印是最为基础的任务了。
<a name="035Gj"></a>
### echo 命令
<a name="1ebbd644"></a>
#### 使用规则
在默认情况下，echo在每次调用后会添加一个换行符<br />使用不带引号的echo时，无法在所要显示的文本中使用分号`;`，因为分号被用作命令定界符<br />使用双引号的时候，遇到特殊字符的时候需要转义字符`\`进行转义，而在单引号中则不用<br />变量替换在单引号中无效，如`${username}`等
<a name="ATi4R"></a>
#### 转义字符
```bash
\a 发出警告声
\b 删除前一个字符
\c 最后不加上换行符号
\f 换行但光标仍旧停留在原来的位置
\n 换行且光标移至行首
\r 光标移至行首，但不换行
\t 插入tab
\v 与\f相同
\\ 插入\字符
\nnn 插入nnn（八进制）所代表的ASCII字符
```
<a name="fDHxU"></a>
#### 常用选项
可以使用直接输出命令结果<br />可以使用`-n`选项来忽略结尾的换行符<br />可以使用`-e`选项在echo中转义换行符
<a name="RpGtc"></a>
#### 打印彩色输出
文字闪动
```
0 关闭所有属性
1 设置高亮度(加粗)
2 淡化
3 斜体
4 下划线
5 闪烁
7 反显
8 消隐
```
文字的颜色
```
重置=0，黑色=30，红色=31，绿色=32，黄色=33，蓝色=34，洋红=35，青色=36，白色=37
```
背景颜色
```
重置=0，黑色=40，红色=41，绿色=42，黄色=43，蓝色=44，洋红=45，青色=46，白色=47
```
<a name="LXrsv"></a>
#### 实例演示
```bash
[root@localhost] $ echo -e "\e[1;31mWord is red \e[0m"
Word is red
[root@localhost] $ echo -e "\e[1;42mthe background is green \e[0m"
the background is green
[root@localhost] $ echo -e "\e[1;42;31mWord is red, the background is green \e[0m"
Word is red, the background is green
[root@localhost] $ echo -e "\e[5;37;31mMySQL Server Stop...\e[0m"
MySQL Server Stop...
[root@localhost] $ echo -e "\e[4;37;31mMySQL Server Stop...\e[0m"
MySQL Server Stop...
```
<a name="xxZhI"></a>
### printf命令使用规则
在默认情况下，printf并不像echo一样会自动添加换行符，必须手动添加
<a name="qmjSU"></a>
#### 格式替换符
```
%s 字符串
%c ASCII 字符
%d 十进制整数
%f 浮点数
```
<a name="E3EsF"></a>
#### 对齐格式
```
-左对齐
不指定，默认为右对齐
可以在对齐符号之后，添加数字，表示宽度
对于浮点数，可以使用其他参数对小数部分进行取舍，如%-4.2f
```
<a name="4f0mB"></a>
#### 实例演示
```
[root@localhost] $ printf "%-5s %-10s %-4.2f\n" 1 escape 100.123
1   escape   100.12
```
<a name="786Md"></a>
## 变量和环境变量
变量是任何一种编程语言都必不可少的组成部分，用于存放各类数据。

- 在 Bash 中，每一个变量的值都是字符串
- 无论给变量赋值时有没有使用引号，值都会是字符串的形式存储的
- 环境变量就是用来被 shell 环境和操作系统环境用来存储一些特殊值的变量
<a name="2O0jQ"></a>
### 查看环境变量
```bash
所有与终端相关
env命令
对于进程而已
cat /proc/$PID/environ
默认彼此之前由\0分割，可以使用tr命令进行转换
```
<a name="5dCMa"></a>
### 使用变量
<a name="c02c2f23"></a>
#### 定义变量
```bash
var=value
```
<a name="MT7Sn"></a>
#### 使用变量
```bash
echo $var
echo ${var}
```
<a name="fp68n"></a>
### 添加 PATH 变量路径
```bash
PATH变量通常定义在/etc/envitonment或/etc/profile或~/.bashrc中
export PATH="$PATH:/home/escape/bin"
```
<a name="GVbjc"></a>
### 变量的用途
<a name="3e8ff493"></a>
#### 获得字符串长度
```bash
length=$(#var)
```
<a name="WOOQC"></a>
#### 识别当前所使用的shell
```bash
echo $SHELL
echo $0
```
<a name="Gyc3g"></a>
#### 检查是否为超级用户
```bash
[ $UID -ne 0 ]
```
<a name="4RA32"></a>
#### 修改 Bash 提示符
```bash
echo $PS1
PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$'
可以使用类似\e[1;31的特定转义字符来设置彩色提示字符串
```
<a name="j3QUT"></a>
#### 使用函数添加环境变量
把下面的函数加入~/.bashrc中即可
```bash
prepend() { [ -d "$2" ] && eval $1=\"$2':'\$$1\" && export $1; }
```

- 判断第二个参数指定的目录是否存在
- 将第一个参数所指向的变量值设置成为第二个参数的值加上:$1的形式
- 设置为环境变量
<a name="JrDLV"></a>
### 使用方法
```bash
prepend PATH /opt/myapp/bin
```
<a name="JWBHs"></a>
#### 改进方案
```bash
prepend() { [ -d $2 ] && eval $1=\"$2\$\{$1+':'\$$1\}\" && export $1; }
#其中$1不为空的时候使用表达式':'\$$1
#$$1表示第一个参数的实际值
```
<a name="zEGYI"></a>
### 实例演示
```bash
[root@localhost] $ pgrep gedit
12501
[root@localhost] $ cat /proc/12501/environ
[root@localhost] $ cat /proc/12501/environ | tr '\0' '\n'
bash
[root@localhost] $ count=5
[root@localhost] $ fruit=apple
[root@localhost] $ echo "We hava $count ${fruit}(s)"
We have 5 apple(s)
[root@localhost] $ echo '$var' will print $var
$var will print $var
bash
[root@localhost] $ echo $PATH
/usr/local/var/pyenv/shims:/usr/local/bin
[root@localhost] $ export PATH="$PATH:/home/escape/bin"
[root@localhost] $ PATH="$PATH:/home/escape/bin"
[root@localhost] $ export PATH
[root@localhost] $ echo $PS1
${ret_status} %{$fg[cyan]%}%c%{$reset_color%} $(git_prompt_info)
```
<a name="K1KlG"></a>
## 进行数学运算
在Bash shell环境中，可以使用`let`、`(())`、`[]`执行基本的算术操作，使用`expr`和`bc`可以进行高级操作
<a name="hf18j"></a>
##### 基本算数
```bash
let
let result=num1+num2
let num++
let num--
let num+=5
let num-=5
$(())
result=$((num1+num2))
result=$(($num+5))
$[]
result=$[ num1+num2 ]
result=$[ $num+5 ]
```
<a name="xF97F"></a>
### 高级算数
```bash
expr
result=expr 3+4
result=$(expr $num+5)
bc
普通用法 echo "4 * 0.56" | bc
设定小数精度 echo "scale=2; 3/8" | bc
十进制转二进制 echo "obase=2; $num" | bc
二进制转十进制 echo "obase=10; ibase=2; $num" | bc
平方根 echo "sqrt(100)" | bc
计算平方 echo "10^10" | bc
```
<a name="4VL4h"></a>
## 脚本判断条件
写好 shell 脚本需要熟悉下面的判断条件和规则
<a name="SV9kJ"></a>
### 逻辑判断
```bash
! 逻辑非
-a 逻辑且
-o 逻辑或
```
<a name="79466dc2"></a>
### 语句判断
解释说明
```bash
[-e file] #如果 file 文件存在则为真
[-d file] #如果 file 存在且是一个目录则为真
[-f file] #如果 file 存在且是一个普通文件则为真
[-b file] #如果 file 存在且是一个块特殊文件则为真
[-s file] #如果 file 存在且大小不为 0 则为真
[-c file] #如果 file 存在且是一个字特殊文件则为真
[-h file] #如果 file 存在且是一个符号连接则为真
[-p file] #如果 file 存在且是一个名字管道则为真
[-r file] #如果 file 存在且是可读的则为真
[-w file] #如果 file 存在且是可写的则为真
[-x file] #如果 file 存在且是可执行的则为真
[-L file] #如果 file 存在且是一个符号连接则为真
[-S file] #如果 file 存在且是一个套接字则为真
[-O file] #如果 file 存在且属有效用户 ID 则为真
[-G file] #如果 file 存在且属有效用户组则为真
[-t FD]   #如果文件描述符 FD 打开且指向一个终端则为真
[-u file] #如果 file 存在且设置了 SUID(用 chmod u+s 设置)则为真
[-g file] #如果 file 存在且已经设置了 SGID(用 chmod g+s 设置)则为真
[-k file] #如果 file 存在且已经设置粘制位(用 chmod a+t 设置)则为真
```
<a name="zR5wV"></a>
### 条件表达式
解释说明
```
[string]     #string 的长度为非零 non-zero 则为真
[-n string]  #string 的长度为非零 non-zero 则为真
[-z string]   #string 的长度为零则为真
[sting1==string2] #如果 2 个字符串相同则为真
[string1!=string2] #如果 2 个字符串不相等则为真
[string1 –eq string2] #等于为真
[string1 -ne string2] #不等于为真
[string1 -le string2] #小于为真
[string1 –gt string2] #大于为真
[string1 –ge string2] #大于等于为真
[file1 –nt file2]     #如果 file1 比 file2 要新或者 file1 存在且 file2 不存在则为真
[file1 –ot file2]     #如果 file1 比 file2 要老或 file2 存在且 file1 不存在则为真
[file1 –ef file2]     #如果 file1 和 file2 指向相同的设备和节点号则为真
```
