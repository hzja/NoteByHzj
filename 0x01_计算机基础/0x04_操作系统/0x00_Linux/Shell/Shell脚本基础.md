Shell
<a name="Ft1qT"></a>
## shell脚本
<a name="Ab88z"></a>
### shell
shell是外壳的意思，就是操作系统的外壳。可以通过shell命令来操作和控制操作系统，比如Linux中的Shell命令就包括ls、cd、pwd等等。总结来说，Shell是一个命令解释器，它通过接受用户输入的Shell命令来启动、暂停、停止程序的运行或对计算机进行控制。<br />shell 是一个应用程序，它连接了用户和 Linux 内核，让用户能够更加高效、安全、低成本地使用 Linux 内核，这就是 Shell 的本质。<br />shell 本身并不是内核的一部分，它只是站在内核的基础上编写的一个应用程序。
<a name="Rt9ji"></a>
### shell脚本
shell脚本就是由Shell命令组成的执行文件，将一些命令整合到一个文件中，进行处理业务逻辑，脚本不用编译即可运行。它通过解释器解释运行，所以速度相对来说比较慢。<br />shell脚本中最重要的就是对shell命令的使用与组合，再使用shell脚本支持的一些语言特性，完成想要的功能。

---

<a name="Q41bA"></a>
### 注释
“`#`”开头的就是注释，被编译器忽略

- 单行注释：`#`
- 多行注释：:<<eof ...="" eof="" 或者="" :<<!="" !="" （:<<="" 标识多行注释开始，并指定一个标识符作为开始结束的标志）<="" li="" style="box-sizing: border-box;">
<a name="S4ZGb"></a>
### 脚本格式
脚本以`#!/bin/bash`开头(指定解析器)

---

<a name="81i67"></a>
## 变量
<a name="yk72Y"></a>
### 系统预定义变量
<a name="ysxY5"></a>
#### 常用系统变量
`$HOME`、`$PWD`、`$SHELL`、`$USER`等
<a name="oEBug"></a>
#### 查看系统变量的值
```bash
[root@test001 ~]# echo $HOME
/root
[root@test001 ~]# echo $USER
root
[root@test001 ~]# echo $SHELL
/bin/bash
[root@test001 ~]# echo $PWD
/root
```
<a name="QXYEj"></a>
#### 显示当前Shell中所有的变量：set
```bash
[root@test001 bash]# set
```
<a name="Y3ZPv"></a>
### 变量类型
运行shell时，会同时存在三种变量：

1. 局部变量：局部变量在脚本或命令中定义，仅在当前shell实例中有效，其他shell启动的程序不能访问局部变量。
2. 环境变量：所有的程序，包括shell启动的程序，都能访问环境变量，有些程序需要环境变量来保证其正常运行。必要的时候shell脚本也可以定义环境变量。
3. shell变量：shell变量是由shell程序设置的特殊变量。shell变量中有一部分是环境变量，有一部分是局部变量，这些变量保证了shell的正常运行
<a name="KBUnI"></a>
### 变量操作

- 创建普通变量：`name="test"` （=两边不可有空格）
- 创建只可函数体中使用的局部变量：`local name="test"` （使用local修饰的变量在函数体外无法访问，并且local只能在函数体内使用）
- 使用变量：`echo $name` 或者 `echo ${name}` （推荐使用大括号版）
- 变量重新赋值：`name="new_test"` （将原值覆盖）
- 只读变量：`name="only_read" -> readonly name` （使用readonly标识后的变量，不可被修改）
- 删除变量：`unset name;` （删除之后不可访问，删除不掉只读变量）
<a name="qhqsC"></a>
### 变量定义规则

- 变量名称可以有字母、数字和下划线组成，但是不能以数字开头，环境变量名建议大写
- =号两侧不能有空格
- 在bash中，变量默认类型都是字符串类型，无法直接进行数值运算
- 变量的值如果有空格，需要使用双引号或单引号括起来
<a name="YQWQD"></a>
### 字符串变量
<a name="5aNTV"></a>
#### 1）单引号

- 单引号变量 `var='test'` ，只能原样输出，变量无效
- 单引号中不能出现一个单独的单引号，转义也不可以
<a name="31d6376b"></a>
#### 2）双引号

- 双引号变量 `var="my name is ${name}"`，变量有效
- 可出现转义符
<a name="9ntPR"></a>
#### 3）拼接字符串

- 中间无任何+，之类的字符

创建str.sh，内容如下：
```bash
#!/bin/bash
name="fcant"
url="http://www.yueque.com"
str1=$name$url  #中间不能有空格
str2="$name $url"  #如果被双引号包围，那么中间可以有空格
str3=$name": "$url  #中间可以出现别的字符串
str4="$name: $url"  #这样写也可以
str5="${name}: ${url}/fcant"  #这个时候需要给变量名加上大括号
str6='${name}: ${url}/fcant' #单引号括包裹起来的不会被替换
echo $str1
echo $str2
echo $str3
echo $str4
echo $str5
echo $str6
```
运行输出
```bash
[root@test001 shells]# chmod +x str.sh 
[root@test001 shells]# ./str.sh 
fcanthttp://www.yueque.com
fcant http://www.yueque.com
fcant: http://www.yueque.com
fcant: http://www.yueque.com
fcant: http://www.yueque.com/fcant
${name}: ${url}/fcant
```
<a name="07OD7"></a>
#### 4）获取字符串长度

- 在`${}`中使用“`#`”获取长度
- `name="test";`
- `echo ${#name};` # 输出为4
<a name="c2bJk"></a>
#### 5）提取子字符串

- `1:4` 从第2个开始 往后截取4个字符
- `::4` 从第一个字符开始 往后截取4个字符
- `name="this is my name";`
- `echo ${name:1:4}` #输出 `his `
- `echo ${name::4}` #输出 `this`
<a name="I6rIa"></a>
### `export`将变量提升为全局环境变量
子shell无法访问父shell中定义的变量，先来一段命令，打卡一个新的窗口，执行下面命令
```bash
[root@test001 ~]# A=tom
[root@test001 ~]# echo $A
tom
[root@test001 ~]# bash
[root@test001 ~]# echo $A

[root@test001 ~]#
```
在定义变量的shell中，使用export 变量名将变量提升为全局变量，这样内部的所有子shell都可以访问这个变量了，打开一个新的窗口，执行下面命令
```bash
[root@test001 ~]# A=tom
[root@test001 ~]# export A
[root@test001 ~]# bash
[root@test001 ~]# echo $A
tom
[root@test001 ~]# bash
[root@test001 ~]# echo $A
tom
[root@test001 ~]# ps -f
UID         PID   PPID  C STIME TTY          TIME CMD
root       3479   3475  0 16:43 pts/0    00:00:00 -bash
root       3531   3479  0 16:43 pts/0    00:00:00 bash
root       3571   3531  0 16:43 pts/0    00:00:00 bash
root       3831   3571  0 16:47 pts/0    00:00:00 ps -f
[root@test001 ~]# ps --forest
   PID TTY          TIME CMD
  3479 pts/0    00:00:00 bash
  3531 pts/0    00:00:00  \_ bash
  3571 pts/0    00:00:00      \_ bash
  3833 pts/0    00:00:00          \_ ps
```
<a name="wQkBL"></a>
### 数组
bash只支持一维数组，不支持多维数组

- 定义数组：`array_name=(li wang xiang zhang)` （小括号做边界、使用空格分离）
- 单独定义数组的元素：`arraypara[0]="w"; arraypara[3]="s"` （定义时下标不连续也可以）
- 赋值数组元素：`array_name[0]="zhao";`
- 获取数组元素：
   - `arrayname[0]="li"`
   - `arrayname[3]="zhang"`
   - `echo ${arrayname[0]}` # 输出"li"
   - `echo ${arrayname[1]}` # 输出" "
   - `echo ${arrayname[3]}` # 输出"zhang"
   - `echo ${arrayname[@]}` # 输出"li zhang" 输出数组所有元素，没有元素的下标省略
- 取得元素个数：`${#arrayname[@]}` 或者 `${#arrayname[*]}`
- 取得单个元素长度：`${#array_name[1]}`

---

<a name="pxZYl"></a>
## 参数传递
获取参数值：

- `$0` ：固定，代表执行的文件名
- `$1` ：代表传入的第1个参数
- `$n` ：代表传入的第n个参数，十以上的参数需要用大括号包含，如`${10}`）

`$#`：参数个数<br />`$*`：以一个单字符串显示所有向脚本传递的参数。这个变量代表命令行中所有的参数， $*把所有的参数看成一个整体，如"`$*`"用「"」括起来的情况、以"`$1` `$2` … `$n`"的形式输出所有参数<br />`$@`：与`$*`相同，这个变量也代表命令行中所有的参数， 不过`$@`把每个参数区分对待，但是使用时加引号，并在引号中返回每个参数。<br />`$*` 与 `$@` 区别

- 相同点：都是引用所有参数。
- 不同点：只有在双引号中体现出来。假设在脚本运行时写了三个参数 1、2、3，，则 `$*` 等价于 "1 2 3"（传递了一个参数），而 `$@` 等价于 "1" "2" "3"（传递了三个参数）。

当 `$*` 和 `$@` 不被双引号`" "`包围时，它们之间没有任何区别，都是将接收到的每个参数看做一份数据，彼此之间以空格来分隔。<br />但是当它们被双引号`" "`包含时，就会有区别了：

- "`$*`"会将所有的参数从整体上看做一份数据，而不是把每个参数都看做一份数据。
- "`$@`"仍然将每个参数都看作一份数据，彼此之间是独立的。

比如传递了 5 个参数，那么对于"`$*`"来说，这 5 个参数会合并到一起形成一份数据，它们之间是无法分割的；而对于"`$@`"来说，这 5 个参数是相互独立的，它们是 5 份数据。<br />如果使用 echo 直接输出"`$*`"和"`$@`"做对比，是看不出区别的；但如果使用 for 循环来逐个输出数据，立即就能看出区别来。<br />编写下面的代码，并保存为 test.sh：
```bash
#!/bin/bash

echo "print each param from \"\$*\""
for var in "$*"
do
    echo "$var"
done

echo "print each param from \"\$@\""
for var in "$@"
do
    echo "$var"
done
```
运行 test.sh，并附带参数
```bash
[mozhiyan@localhost demo]$ . ./test.sh a b c d
print each param from "$*"
a b c d
print each param from "$@"
a
b
c
d
```
从运行结果可以发现，对于"`$*`"，只循环了 1 次，因为它只有 1 分数据；对于"`$@`"，循环了 5 次，因为它有 5 份数据。<br />`$$`：脚本运行的当前进程号<br />`$!`：后台运行的最后一个进程的ID<br />`$?`：显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。

| **变量** | **含义** |
| --- | --- |
| `$0` | 当前脚本的文件名。 |
| `$n`（n≥1） | 传递给脚本或函数的参数。n 是一个数字，表示第几个参数。例如，第一个参数是 $1，第二个参数是 `$2`，若n大于10，则语法：`${n}` |
| `$#` | 传递给脚本或函数的参数个数 |
| `$*` | 传递给脚本或函数的所有参数。 |
| `$@` | 传递给脚本或函数的所有参数。当被双引号" "包含时，$@ 与 $* 稍有不同 |
| `$?` | 上个命令的退出状态，或函数的返回值 |
| `$$` | 当前 Shell 进程 ID。对于 Shell 脚本，就是这些脚本所在的进程 ID。 |
| `$!` | 后台运行的最后一个进程的ID |

案例：编写下面的代码，并保存为 test.sh：
```bash
#!/bin/bash

echo "Process ID: $$"
echo "File Name: $0"
echo "First Parameter : $1"
echo "Second Parameter : $2"
echo "All parameters 1: $@"
echo "All parameters 2: $*"
echo "Total: $#"
```
运行 test.sh，并附带参数：
```
[root@test001 shells]# ./test.sh shell linux
Process ID: 131045
File Name: ./test.sh
First Parameter : shell
Second Parameter : linux
All parameters 1: shell linux
All parameters 2: shell linux
Total: 2
```

---

<a name="zid2V"></a>
## 运算符`$(())`或`$[]`
<a name="Yj6Z4"></a>
### 语法
```bash
$((运算式))或$[运算式]
```
<a name="iyWRO"></a>
### 算数运算
`+` 、`-`、`*`、`\` ：乘号前必须加`\`进行转义才可以进行乘法运算<br />加法运算

- `val=`expr 2 + 2`` （使用linux命令`expr`进行辅助运算）
- `val=$[2+2]` （4个空格不是必要的，不同于条件判断）
- `val=$((2+2))`
<a name="M1ruj"></a>
## 条件判断
<a name="IDptA"></a>
### 语法
（1）`test condition`<br />（2）`[ condition ]`：注意`condition`前后要有空格<br />注意： 条件非空即为 true， `[ name ]`返回 true， `[ ]` 返回 false。
<a name="WXOcx"></a>
### 数字关系运算符
关系运算符只支持数字，不支持字符串，除非字符串的值是数字。下面假定变量 a 为 10，变量 b 为 20

- `-eq` ：检测两个数是否相等，相等返回 true。`[ $a -eq $b ]` 返回 false。
- `-ne`：检测两个数是否不相等，不相等返回 true。`[ $a -ne $b ]` 返回 true。
- `-gt`：检测左边的数是否大于右边的，如果是，则返回 true。`[ $a -gt $b ]` 返回 false。
- `-lt` ：检测左边的数是否小于右边的，如果是，则返回 true。`[ $a -lt $b ]` 返回 true。
- `-ge`：检测左边的数是否大于等于右边的，如果是，则返回 true。`[ $a -ge $b ]` 返回 false。
- `-le` ：检测左边的数是否小于等于右边的，如果是，则返回 true。`[ $a -le $b ]` 返回 true。
| **语法** | **说明** |
| --- | --- |
| `[ $a -eq $b]` | 判断a=b（equal） |
| `[ $a -ne $b]` | 判断a!=b（not equal） |
| `[ $a -lt $b]` | 判断a<b（less than） |
| `[ $a -le $b]` | 判断a<=b（less equal） |
| `[ $a -gt $b]` | 判断a>b（greater than） |
| `[ $a -ge $b]` | 判断a>=b（greater equal） |

<a name="mYHTc"></a>
### 字符串运算符
注意：字符串之间比较，用等号“=”判断相等，用“!=”判断不等，=和!=前后都要有空格<br />下表列出了常用的字符串运算符，假定变量 a 为 "abc"，变量 b 为 "efg"：

- `=` ：检测两个字符串是否相等，相等返回 true。`[ $a = $b ]` 返回 false。
- `!=` ：检测两个字符串是否相等，不相等返回 true。`[ $a != $b ]` 返回 true。
- `-z` ：检测字符串长度是否为0，为0返回 true。`[ -z $a ]` 返回 false。
- `-n` ：检测字符串长度是否为0，不为0返回 true。`[ -n "$a" ]` 返回 true。
- `$` ：检测字符串是否为空，不为空返回 true。`[ $a ]` 返回 true。
| **语法** | **说明** |
| --- | --- |
| `[ $a = $b]` | 把变量a和b都作为字符串，来判断这俩变量是否相等 |
| `[ $a != $b]` | 把变量a和b都作为字符串，来判断这俩变量是否不相等 |
| `[ $a = "ready" ]` | 判断变量a是否等于字符串“ready” |

<a name="B52AG"></a>
### 布尔运算符
下表列出了常用的布尔运算符，假定变量 a 为 10，变量 b 为 20：

- `!` ：非运算，表达式为 true 则返回 false，否则返回 true。`[ ! false ]` 返回 true。
- `-o` ：或运算，有一个表达式为 true 则返回 true。`[ $a -lt 20 -o $b -gt 100 ]` 返回 true。
- `-a` ：与运算，两个表达式都为 true 才返回 true。`[ $a -lt 20 -a $b -gt 100 ]` 返回 false。
<a name="IdVhY"></a>
### 逻辑运算符
以下介绍 Shell 的逻辑运算符，假定变量 a 为 10，变量 b 为 20:

- `&&` ：逻辑的 AND `[[ $a -lt 100 && $b -gt 100 ]]` 返回 false
- `||` ：逻辑的 OR `[[ $a -lt 100 || $b -gt 100 ]]` 返回 true
<a name="JAtZD"></a>
### 文件运算符

- `-b file` ：检测文件是否是块设备文件，如果是，则返回 true。`[ -b $file ]` 返回 false。
- `-c file` ：检测文件是否是字符设备文件，如果是，则返回 true。`[ -c $file ]` 返回 false。
- `-d file` ：检测文件是否是目录，如果是，则返回 true。`[ -d $file ]` 返回 false。
- `-f file` ：检测文件是否是普通文件（既不是目录，也不是设备文件），如果是，则返回 true。`[ -f $file ]` 返回 true。
- `-g file` ：检测文件是否设置了 SGID 位，如果是，则返回 true。`[ -g $file ]` 返回 false。
- `-k file` ：检测文件是否设置了粘着位(Sticky Bit)，如果是，则返回 true。`[ -k $file ]` 返回 false。
- `-p file` ：检测文件是否是有名管道，如果是，则返回 true。`[ -p $file ]` 返回 false。
- `-u file` ：检测文件是否设置了 SUID 位，如果是，则返回 true。`[ -u $file ]` 返回 false。
- `-r file` ：检测文件是否可读，如果是，则返回 true。`[ -r $file ]` 返回 true。
- `-w file` ：检测文件是否可写，如果是，则返回 true。`[ -w $file ]` 返回 true。
- `-x file` ：检测文件是否可执行，如果是，则返回 true。`[ -x $file ]` 返回 true。
- `-s file` ：检测文件是否为空（文件大小是否大于0），不为空返回 true。`[ -s $file ]` 返回 true。
- `-e file` ：检测文件（包括目录）是否存在，如果是，则返回 true。`[ -e $file ]` 返回 true。
| **语法** | **说明** |
| --- | --- |
| `[ -r 文件 ]` | 判断文件是否有可读权限 |
| `[ -w 文件 ]` | 判断文件是否有可写权限 |
| `[ -x 文件 ]` | 判断文件是否有可执行权限 |
| `[ -e 文件 ]` | 判断文件是否存在 |
| `[ -f 文件 ]` | 文件存在并且是一个常规的文件（file） |
| `[ -d 文件 ]` | 文件存在并且是一个目录（dirctory） |

<a name="PXgnS"></a>
### 使用`$?`获取条件判断的结果，0：表示true，1：表示false
shell中的条件判断返回值比较特殊，0：表示true，1：表示false
```bash
#下面判断2>1
[root@test001 ~]# [ 2 -gt 1 ]
#使用$?获取上面判断的结果，输出结果为0，表示true
[root@test001 ~]# echo $?
0
#判断2<1，结果为1，表示false
[root@test001 ~]# [ 2 -lt 1 ]
[root@test001 ~]# echo $?
1
```
<a name="xUkK5"></a>
### 案例
<a name="luMkf"></a>
#### （1）整数判断
```bash
#使用test语法判断10>9
[root@test001 ~]# test 10 -gt 9
# 打印出上面的判断结果，输出0，表示结果为真
[root@test001 ~]# echo $?
0
#也可以使用[]语法，下面判断2>1，输出结果为0，表示true
[root@test001 ~]# [ 2 -gt 1 ]
[root@test001 ~]# echo $?
0
#判断2<1，结果为1，表示false
[root@test001 ~]# [ 2 -lt 1 ]
[root@test001 ~]# echo $?
1
```
<a name="OnYrp"></a>
#### （2）文件权限判断
```bash
# 使用ll查看一下文件列表，当前root用户，对于1.txt有rw权限
[root@test001 ~]# ll
总用量 12
-rw-r--r--. 1 root root   99 5月  17 15:59 1.txt
# 判断当前用户对1.txt是否有r权限，结果为0，表示true
[root@test001 ~]# [ -r 1.txt ]
[root@test001 ~]# echo $?
0
# 判断当前用户对1.txt是否有x权限，结果为1，表示false
[root@test001 ~]# [ -x 1.txt ]
[root@test001 ~]# echo $?
1
```
<a name="S7ZOb"></a>
#### （3）根据文件类型判断
```bash
# 1.txt为普通文件，bash为目录
[root@test001 ~]# ll
总用量 12
-rw-r--r--. 1 root root   99 5月  17 15:59 1.txt
drwxr-xr-x. 2 root root  114 5月  20 16:56 bash
# 判断2.txt是否存在，返回1，表示false
[root@test001 ~]# [ -e 2.txt ]
[root@test001 ~]# echo $?
1
# 判断1.txt是否存在，返回0，表示true
[root@test001 ~]# [ -e 1.txt ]
[root@test001 ~]# echo $?
0
# 判断1.txt是否是普通文件，返回0，表示true
[root@test001 ~]# [ -f 1.txt ]
[root@test001 ~]# echo $?
0
# 判断1.txt是否是目录，返回1，表示false
[root@test001 ~]# [ -d 1.txt ]
[root@test001 ~]# echo $?
1
# 判断/root/bash是否是目录，返回0，表示true
[root@test001 ~]# [ -d /root/bash ]
[root@test001 ~]# echo $?
0
```
<a name="fcpUR"></a>
#### （4）多条件判断(`&&`、`||`)
多条件判断（&& 表示前一条命令执行成功时， 才执行后一条命令， || 表示上一条命令执行失败后， 才执行下一条命令）
<a name="szDSw"></a>
##### 案例1：多条件判断
```bash
# 3>2 && 2>1，返回0，表示ture
[root@test001 shells]# [ 3 -gt 2 ] && [ 2 -gt 1 ]
[root@test001 shells]# echo $?
0
# 3>2 && 2<1，返回1，表示false
[root@test001 shells]# [ 3 -gt 2 ] && [ 2 -lt 1 ]
[root@test001 shells]# echo $?
1
# 3<2 || 2>1，返回0，表示true
[root@test001 shells]# [ 3 -lt 2 ] || [ 2 -gt 1 ]
[root@test001 shells]# echo $?
0
```
<a name="cvjAb"></a>
##### 案例2：模仿3元运算符（`a?b:c`）
```bash
# 定义2个变量，a=10，b=9
[root@test001 ~]# a=10
[root@test001 ~]# b=9
# 若a>b，输出a>b为true，否则输出a>b为false，相当于实现了(a?b:c)3元运算符
[root@test001 ~]# [ $a -gt $b ] && echo "$a>$b为true" || echo "$a>$b为false"
10>9为true
[root@test001 ~]# a=1
[root@test001 ~]# b=2
[root@test001 ~]# [ $a -gt $b ] && echo "$a>$b为true" || echo "$a>$b为false"
1>2为false
```

---

<a name="HTych"></a>
## 执行相关
<a name="3f66024c"></a>
### 命令替换
命令替换与变量替换差不多，都是用来重组命令行的，先完成引号里的命令行，然后将其结果替换出来，再重组成新的命令行。执行命令：

1. ``ls /etc`` ：反引号 （所有的unix系统都支持）
2. `$(ls /etc)` ：`$+()` （部分unix系统不支持） 多个嵌套使用时，从内向外执行

`for file in `ls /etc`` 或 `for file in $(ls /etc)` 循环中使用 ``dirname \$0`` 获取脚本文件所在的目录 `path=$(cd `dirname $0`;pwd)` ：获取脚本当前所在目录，并且执行cd命令到达该目录，使用`pwd`获取路径并赋值到path变量
```bash
[root@test001 shells]# dt=`date -d '1 day ago' "+%Y-%m-%d"`
[root@test001 shells]# echo $dt
2022-05-20
[root@test001 shells]# echo date=`date -d '1 day ago' "+%Y-%m-%d"`
date=2022-05-20
[root@test001 shells]# echo date=$(date -d '1 day ago' "+%Y-%m-%d")
date=2022-05-20
```
<a name="uBhdE"></a>
### 算术运算

1. `$[ ]` : 加减乘除，不必添加空格
2. `$(( ))` ：加减乘除等，不必添加空格
<a name="YjvKB"></a>
### 逻辑判断

1. ：中括号旁边和运算符两边必须添加空格 （可以使用，不推荐）
2. `[[ ]]`：中括号旁边和运算符两边必须添加空格 （字符串验证时，推荐使用）
3. `(())` ：中括号旁边和运算符两边必须添加空格 （数字验证时，推荐使用）
4. `[[]]` 和 `(())` 分别是`[ ]`的针对数学比较表达式和字符串表达式的加强版。
5. 使用`[[ ... ]]`条件判断结构，而不是`[ ... ]`，能够防止脚本中的许多逻辑错误。比如，`&&`、`||`、`<`和`>` 操作符能够正常存在于`[[ ]]`条件判断结构中，但是如果出现在`[ ]`结构中的话，会报错。比如可以直接使用`if [[ $a != 1 && $a != 2 ]]`，如果不适用双括号，则为`if [ $a -ne 1] && [ $a != 2 ]`或者`if [ $a -ne 1 -a $a != 2 ]`。`[[ ]]`中增加模式匹配特效；`(( ))`不需要再将表达式里面的大小于符号转义，除了可以使用标准的数学运算符外，还增加了以下符号 
| 符号 | 描述 |
| --- | --- |
| `val++` | 后增 |
| `val--` | 后减 |
| `++val` | 先增 |
| `--val` | 先减 |
| `!` | 逻辑求反 |
| `~` | 位求反 |
| `**` | 幂运算 |
| `<<` | 左位移 |
| `>>` | 右位移 |
| `&` | 位布尔和 |
| `&#124;` | 位布尔减 |
| `&&` | 逻辑和 |
| `&#124;&#124;` | 逻辑或 |


---

<a name="FcuwY"></a>
## 输出
<a name="dSnm1"></a>
### echo
仅用于字符串的输出，没有使用`printf`作为输出的移植性好，建议使用`printf`
<a name="D76IF"></a>
### printf
`printf` 不会像 echo 自动添加换行符，可以手动添加 `\n` 无大括号，直接以空格分隔

- 格式： `printf format-string[arguments...]` 其中（format-string: 格式控制字符串、arguments: 参数列表）
- 案例： `printf"%-10s %-8s %-4.2f\n"郭靖男66.1234`
- `%s` `%c` `%d` `%f` 都是格式替代符
   - d：Decimal 十进制整数 对应位置参数必须是十进制整数，否则报错!
   - s：String 字符串 对应位置参数必须是字符串或者字符型 否则报错
   - c：Char 字符 对应位置参数必须是字符串或者字符型 否则报错
   - f：Float 浮点 对应位置参数必须是数字型 否则报错
- `%-10s` ：指一个宽度为10个字符（-表示左对齐，没有则表示右对齐），任何字符都会被显示在10个字符宽的字符内，如果不足则自动以空格填充，超过也会将内容全部显示出来。
- `%-4.2f` ：指格式化为小数，宽度为4个字符，其中 `.2`指保留2位小数。
- 转义符：
   - `\a` ：警告字符，通常为ASCII的BEL字符
   - `\b` ：后退
   - `\c` ：抑制（不显示）输出结果中任何结尾的换行字符（只在%b格式指示符控制下的参数字符串中有效），而且，任何留在参数里的字符、任何接下来的参数以及任何留在格式字符串中的字符，都被忽略
   - `\f` ：换页（formfeed）
   - `\n` ：换行
   - `\r` ：回车（Carriage return）
   - `\t` ：水平制表符
   - `\v` ：垂直制表符
   - `\` ：一个字面上的反斜杠字符
   - `\ddd` ：表示1到3位数八进制值的字符。仅在格式字符串中有效
   - `\0ddd` ：表示1到3位的八进制值字符

---

<a name="Idshe"></a>
## 流程控制
和Java、PHP等语言不一样，sh的流程控制不可为空，即if或者else的大括号中无任何语句
<a name="OJCmd"></a>
### if else
<a name="M0u2Q"></a>
#### if
```bash
if condition
then
    command1
    command2
...
    commandN
fi
```
```bash
if [ 条件判断式 ];then
    程序
fi
```
```bash
if [ 条件判断式 ]
then
    程序
fi
```
<a name="lN5VD"></a>
#### if else
```bash
if else
if condition
then
    command1
    command2
...
    commandN
else
    command
fi
```
<a name="5f1565c2"></a>
#### if else-if else
```bash
if condition1
then
    command1
elif condition2
then
    command2
else
    commandN
fi
```
```bash
if [ 条件判断式 ]
then
    程序
elif [ 条件判断式 ]
then
    程序
else
    程序
fi
```
**注意事项：**

- [ 条件判断式 ]， 中括号和条件判断式之间必须有空格
- if 后要有空格
<a name="Rav70"></a>
### for
```bash
for var in item1 item2 ... itemN
do
    command1
    command2
...
    commandN
done
```
```bash
for 变量 in 值1 值2 值3…
do
    程序
done
```
```bash
for (( 初始值;循环控制条件;变量变化 ))
do
    程序
done
```
```bash
#!/bin/bash
sum=0;
for((i=1;i<=100;i++))
do
        sum=$[$sum+$i]
done
echo "结果："$sum
```
<a name="PmAaK"></a>
### while
<a name="uZiLN"></a>
#### while condition
```bash
while condition
do
    command
done
```
```bash
while [ 条件判断式 ]
do
    程序
done
```
<a name="wRMQH"></a>
#### while 无限循环
```bash
while:
do
    command
done
```
<a name="EvlM6"></a>
### until
until 循环执行一系列命令直至条件为 true 时停止。until 循环与 while 循环在处理方式上刚好相反。
```bash
until condition
do
    command
done
```
<a name="JP1nq"></a>
### case
Shell case语句为多选择语句。可以用`case`语句匹配一个值与一个模式，如果匹配成功，执行相匹配的命令。`case`需要一个`esac`（就是case反过来）作为结束标记，每个`case`分支用右圆括号，用两个分号表示`break`，其中“`;;`”不是跳出循环，是不在去匹配下面的模式 `case`语句格式如下：
```bash
case 值 in
模式1)
    command1
    command2
...
    commandN
;;
模式2）
    command1
    command2
...
    commandN
;;
esac
```
```bash
case $变量名 in
"值 1")
如果变量的值等于值 1， 则执行程序 1
;;
"值 2")
如果变量的值等于值 2， 则执行程序 2
;;
…省略其他分支…
*)
如果变量的值都不是以上的值， 则执行此程序
;;
esac
```
注意事项：

- case 行尾必须为单词“in” ， 每一个模式匹配必须以右括号“`)`” 结束
- 双分号“`;;`” 表示命令序列结束， 相当于 java 中的 break
- 最后的“`*)` ” 表示默认模式， 相当于 java 中的 default
<a name="EfGeX"></a>
### 跳出循环

1. `break` ：跳出总循环
2. `continue`：跳出当前循环，继续下一次循环
<a name="DJ8sx"></a>
## 函数
<a name="jqFFj"></a>
### 系统函数
<a name="uXwSA"></a>
#### `basename`：打印目录或者文件的基本名称
<a name="nYCM9"></a>
##### 1）语法
```bash
basename 名称 [后缀]
basename 选项... NAME...
```
<a name="HLiIv"></a>
##### 2）选项
| **选项** | **作用** |
| --- | --- |
| -a | 支持多个参数并将每个参数视为一个名称 |
| -s | 指定需要剔除的后缀 |

<a name="ZiWBo"></a>
##### 3）案例
```bash
[root@test001 shells]# basename /root/shells/while.sh 
while.sh
[root@test001 shells]# basename /root/shells/while.sh .sh
while
[root@test001 shells]# basename -s .sh -a case.sh while.sh 
case
while
```
<a name="ZRCpJ"></a>
#### `dirname`：获取文件的目录名称
<a name="YbMVc"></a>
##### 1）语法
```bash
dirname 文件名称
```
<a name="uqRlh"></a>
##### 2）案例
```bash
[root@test001 shells]# dirname /usr/bin/
/usr
[root@test001 shells]# dirname /usr/bin/1.sh
/usr/bin
[root@test001 shells]# dirname /usr/bin/1.sh /a/b/2.sh
/usr/bin
/a/b
[root@test001 shells]# dirname 1.sh
.
```
<a name="DWA9u"></a>
### 定义函数
可以带`function fun()` 定义，也可以直接`fun()` 定义，不带任何参数。
<a name="2d518350"></a>
#### 函数定义
```bash
[ function] funname()
{
    action;
		[return int;]
}
```

- 必须在调用函数地方之前， 先声明函数， shell 脚本是逐行运行。 不会像其它语言一样先编译
- 函数返回值， 只能通过`$?`系统变量获得， 可以显式加： return 返回， 如果不加， 将以最后一条命令运行结果， 作为返回值。 return 后跟数值 n(0-255)
<a name="BNv5p"></a>
#### 参数传递

- 调用函数: `fun_name 2 3 4`
- 函数中使用：和shell取用函数相同 `$n` `$#` `$*` `$?` 或者加上`{}`
```bash
funWithParam(){
    echo "第一个参数为 $1 !"
    echo "第二个参数为 $2 !"
    echo "第十个参数为 $10 !"
    echo "第十个参数为 ${10} !"
    echo "第十一个参数为 ${11} !"
    echo "参数总数有 $# 个!"
    echo "作为一个字符串输出所有参数 $* !"}
funWithParam 1 2 3 4 5 6 7 8 9 3 4 7 3
echo $?  \# 判断执行是否成功
```
创建一个sum函数，用来计算用户输入2个数的和。<br />创建fun.sh，内容如下
```bash
#!/bin/bash
function sum(){
    r=$[$1+$2]
    echo "$1+$2=$r"
}

read -p "请输入第1个参数:" a
read -p "请输入第2个参数:" b
sum $a $b
```
执行
```bash
[root@test001 shells]# chmod +x fun.sh 
[root@test001 shells]# ./fun.sh 
请输入第1个参数:2
请输入第2个参数:3
2+3=5
```
<a name="lqk9u"></a>
#### 函数返回值

- `return`字样可存在也可不存在
- `return`只能为 `return [0-255]`，此处的返回可作为函数执行的状态，通过`$?`获取的便是这个返回值
- 如果不加`return`， 则默认最后一条语句的执行状态所为函数执行状态的返回值，如果最后一条语句执行成功，则`$?`为0，否则不为0
<a name="YkEbR"></a>
#### 使用函数返回值
（Janusgraph图数据库官方启动服务脚本片段）

- `return`返回的数字，只是作为函数执行状态的返回值，也就是接下来`$?`获取的值
- 对于类似于下面的 `BIN=`abs_path``语句，获取的是函数体内所有的`echo`、`printf`输出组合成的一个字符串
```bash
abs_path() {
    SOURCE="${BASH_SOURCE[0]}"
while[ -h "$SOURCE"]; do
        DIR="$( cd -P "$( dirname "$SOURCE")" && pwd )"
        SOURCE="$(readlink "$SOURCE")"
[[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
    done
    echo "test"
    echo "$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    # 此函数的两个echo输出会组合成一个字符串作为下述BIN的值
}
BIN=`abs_path` # BIN赋值函数返回值，如果没有return，则函数中所有的echo、printf输出组合成一个字符串传入BIN
path=${BIN}/nodetool # 可直接使用
```
<a name="ZOhkP"></a>
## 输入输出重定向
一般情况下，每个 Unix/Linux 命令运行时都会打开三个文件：

- 标准输入文件(stdin)：stdin的文件描述符为0，Unix程序默认从stdin读取数据。
- 标准输出文件(stdout)：stdout 的文件描述符为1，Unix程序默认向stdout输出数据。
- 标准错误文件(stderr)：stderr的文件描述符为2，Unix程序会向stderr流中写入错误信息。

默认情况下，`command > file` 将 stdout 重定向到 file，`command < file` 将stdin 重定向到 file。如果希望执行某个命令，但又不希望在屏幕上显示输出结果，那么可以将输出重定向到 /dev/null：
<a name="DgJDD"></a>
### 输入重定向

1. `bash.sh < file` ：将脚本的输入重定向到file，由file提供参数
<a name="6b440ad9"></a>
### 输出重定向

1. `bash.sh > file` ：将脚本的输出数据重定向到file中，覆盖数据
2. `bash.sh >> file` ：将脚本的输出数据重定向到file中，追加数据
3. `command >> file 2>&1` ：将 stdout 和 stderr 合并后重定向到 file
<a name="0GPRk"></a>
## 读取外部输入
命令：`read arg` （脚本读取外部输入并赋值到变量上） 在shell脚本执行到上述命令时，停止脚本执行并等待外部输入，将外部输入赋值到arg变量上，继续执行脚本
<a name="bWY0I"></a>
### 语法
```bash
read 选项 变量名
```
等待用户输入值，将用户输入的值放在指定的变量中
<a name="d5GY8"></a>
### 选项
| **选项** | **作用** |
| --- | --- |
| -p | 指定读取时的提示符 |
| -t | 指定读取值时等待的时间（秒）如果-t不加表示一直等待 |

<a name="G8tWk"></a>
### 案例
需求：提示7秒内，读取控制台输入的字符串，然后输出。<br />创建read.sh，内容如下
```bash
#!/bin/bash
read -p "请输入你喜欢的技术：" -t 7 a
echo "你喜欢的技术是："$a
```
测试效果
```bash
[root@test001 shells]# chmod +x read.sh
[root@test001 shells]# ./read.sh 
请输入你喜欢的技术：你喜欢的技术是：
[root@test001 shells]# ./read.sh
请输入你喜欢的技术：linux
你喜欢的技术是：linux
```
<a name="vheZh"></a>
## 文件引用
引用其他的文件之后，可以使用其变量、函数等等，相当于将引用的文件包含进了当前文件 两种方式：

1. `. filepath\filename`
2. `source filepath\filename`
<a name="9cdf419e"></a>
## 颜色标识
```bash
printf  "\033[32m SUCCESS: yay \033[0m\n";
printf  "\033[33m WARNING: hmm \033[0m\n";
printf  "\033[31m ERROR: fubar \033[0m\n";
```
输出结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1608788242832-7d318ca7-a532-4c22-9f78-cd9fea8c75fd.png#averageHue=%235d5d5c&height=107&id=hqGsP&originHeight=321&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=305637&status=done&style=none&title=&width=1107.6666666666667)
<a name="fXZ8N"></a>
## 长句换行
在shell中为避免一个语句过长，可以使用“`\`”进行换行 使用“`\`”换行，在脚本执行过程中还是当做一行一个语句执行，不同于enter直接换行<br />注意：` \` 前添加一个空格 。`\` 后无空格直接换行。
```bash
/mysql/bin/mysql \
-h test_host  -P 000 \
-u test_user -ptest_password ;
```
<a name="93e95e6a"></a>
## shell操作MySQL
下面案例为登录MySQL，并选择操作数据库，之后进行导入数据
```bash
/mysql/mysql/bin/mysql \
-h test_host  -P 000 \
-u test_user -ptest_password \
-e"use test_database; source data_faile; "# -e 代表执行sql语句
```
-u 用户名 -p 用户密码 -h 服务器ip地址 -D 连接的数据库 -N 不输出列信息 -B 使用tab键 代替 分隔符 -e 执行的SQL语句
<a name="tGNd4"></a>
## 退出脚本
命令：`exit`<br />在退出脚本时使用不同的错误码，这样可以根据错误码来判断发生了什么错误。<br />在绝大多数 shell 脚本中，`exit 0` 表示执行成功，`exit 1` 表示发生错误。对错误与错误码进行一对一的映射，这样有助于脚本调试。<br />命令：`set -e`或者`set +e` `set -e`表示从当前位置开始，如果出现任何错误都将触发exit。相反，`set +e`表示不管出现任何错误继续执行脚本。<br />如果脚本是有状态的（每个后续步骤都依赖前一个步骤），那么请使用`set -e`，在脚本出现错误时立即退出脚本。如果要求所有命令都要执行完（很少会这样），那么就使用`set +e`。
<a name="kxgFj"></a>
## shell脚本调试
检查是否有语法错误 `-n`：`bash-n script_name.sh`使用下面的命令来执行并调试 Shell 脚本 `-x`：`bash-x script_name.sh`<br />调试countoddnumber.sh 程序案例：
```bash
#!/usr/bin.env bash
# 用于计算数组中奇数的和
sum=0
for num in1234;do
    re=${num}%2
if(( ${re} == 1));then
        sum=$[${sum}+${num}]
fi
done
echo ${sum}
```

1. 首先检查有无语法错误： `bash-n count_odd_number.sh`
2. 没有输出，说明没有错误，开始实际调试： `bash-x count_odd_number.sh`
3. 调试结果如下：
```bash
+ sum=0
+ for num in1234
+ re=1%2
+ ((  1%2== 1))
+ sum=1
+ for num in1234
+ re=2%2
+ ((  2%2== 1))
+ for num in1234
+ re=3%2
+ ((  3%2== 1))
+ sum=4
+ for num in1234
+ re=4%2
+ ((  4%2== 1))
+ echo 4
4
```
其中的输出显示了程序执行的每一步，通过观察程序执行的步骤是否满足预期从而达到调试的效果 带有 + 表示的是 Shell 调试器的输出，不带 + 表示程序的输出。
<a name="jw9mL"></a>
## 脚本的执行方式
执行shell脚本文件有很多方式，下面来看下。
<a name="RdabW"></a>
### 第1种：采用bash或sh+脚本的绝对路径或相对路径（不用赋予脚本+x权限）
<a name="wMdMF"></a>
#### （1）sh+脚本的相对路径
```bash
[root@test001 shells]# pwd
/root/shells
[root@test001 shells]# sh ./helloworld.sh 
helloworld
```
<a name="Hb6Ky"></a>
#### （2）sh+绝对路径
```bash
[root@test001 shells]# sh /root/shells/helloworld.sh 
helloworld
```
<a name="LzaqJ"></a>
#### （3）bash+脚本的相对路径
```bash
[root@test001 shells]# bash ./helloworld.sh 
helloworld
```
<a name="TXHue"></a>
#### （4）bash+脚本的绝对路径
```bash
[root@test001 shells]# bash /root/shells/helloworld.sh 
helloworld
```
<a name="Uz13z"></a>
### 第2种： 采用输入脚本的绝对路径或相对路径执行脚本（必须具有可执行权限+x）
<a name="meQ1D"></a>
#### ①首先要赋予 helloworld.sh 脚本的+x 权限
```bash
[root@test001 shells]# ./helloworld.sh
-bash: ./helloworld.sh: 权限不够
[root@test001 shells]# chmod +x helloworld.sh
```
<a name="eDopP"></a>
#### ②执行脚本
```bash
[root@test001 shells]# ./helloworld.sh 
helloworld
[root@test001 shells]# /root/shells/helloworld.sh 
helloworld
```
注意：第一种执行方法，本质是bash解析器执行脚本，所以脚本本身不需要执行权限；第二种执行方法，本质是脚本需要自己执行，所以需要执行权限。
<a name="lGJwA"></a>
### 第3种：在脚本的路径前加上“`.`”或者`source`
```bash
[root@test001 shells]# . helloworld.sh 
helloworld
[root@test001 shells]# source helloworld.sh 
helloworld
```
<a name="lt5HM"></a>
### 执行shell脚本的几种方式的区别
刚刚上面介绍了执行shell脚本有3种方式

- 第1种：`bash 脚本文件`或者`sh 脚本文件`
- 第2中：采用输入脚本的绝对路径或相对路径执行脚本（必须具有可执行权限+x）
- 第3中：`. 脚本文件`或`source 脚本文件`

前2种方式都是在当前 shell 中打开一个子 shell 来执行脚本内容， 当脚本内容结束， 则子 shell 关闭， 回到父 shell 中。<br />第3种， 也就是使用在脚本路径前加“.” 或者 source 的方式， 可以使脚本内容在当前 shell 里执行， 而无需打开子 shell。<br />子shell中无法访问父shell中定义的变量，如果需要访问，则需在父shell中，通过`export 变量名`将变量提升为全局环境变量，这样内层的shell才可以访问到外层shell中的环境变量，这也是为什么我们每次要修改完/etc/profile 文件以后， 需要 source 一下的原因。
<a name="gRm0R"></a>
## 父子shell
当使用ssh登录Linux以后，出来了一个黑屏界面，就开启了一个shell进程<br />可以使用`ps -f`查看到到当前的shell进程，如下，使用是bash<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693061373089-6b8c8731-6c9d-4610-a752-8dc7734741e7.png#averageHue=%233e3e3e&clientId=u44bae9da-c056-4&from=paste&height=114&id=u0f82a3e1&originHeight=285&originWidth=1643&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=177001&status=done&style=none&taskId=u2405a31b-3fc3-4616-a5d5-7058c115427&title=&width=657.2)<br />shell可以一层层嵌套，在当前shell中可以使用bash命令开启一个子shell，子shell中还可以继续执行bash命令再次打开一个子shell，多层级父子shell
```
[root@test001 ~]# ps -f
UID         PID   PPID  C STIME TTY          TIME CMD
root        507    503  0 15:59 pts/0    00:00:00 -bash
root       1608    507  0 16:14 pts/0    00:00:00 ps -f
[root@test001 ~]# bash
[root@test001 ~]# ps -f
UID         PID   PPID  C STIME TTY          TIME CMD
root        507    503  0 15:59 pts/0    00:00:00 -bash
root       1611    507  0 16:15 pts/0    00:00:00 bash
root       1647   1611  0 16:15 pts/0    00:00:00 ps -f
[root@test001 ~]# bash
[root@test001 ~]# ps -f
UID         PID   PPID  C STIME TTY          TIME CMD
root        507    503  0 15:59 pts/0    00:00:00 -bash
root       1611    507  0 16:15 pts/0    00:00:00 bash
root       1654   1611  0 16:15 pts/0    00:00:00 bash
root       1687   1654  0 16:15 pts/0    00:00:00 ps -f
```
使用exit可以一层层从里面的shell中向外退出，现在有3层shell，如果需要退出登录，则需要执行3次exit，如下
```
[root@test001 ~]# exit
exit
[root@test001 ~]# ps -f
UID         PID   PPID  C STIME TTY          TIME CMD
root        507    503  0 15:59 pts/0    00:00:00 -bash
root       1611    507  0 16:15 pts/0    00:00:00 bash
root       2205   1611  0 16:24 pts/0    00:00:00 ps -f
[root@test001 ~]# exit 
exit
[root@test001 ~]# ps -f
UID         PID   PPID  C STIME TTY          TIME CMD
root        507    503  0 15:59 pts/0    00:00:00 -bash
root       2223    507  0 16:24 pts/0    00:00:00 ps -f
[root@test001 ~]# exit
登出

Connection closed by foreign host.

Disconnected from remote host(CentOS7_001) at 16:25:15.

Type `help' to learn how to use Xshell prompt.
```
<a name="9ZTmo"></a>
## 案例
（ElasticSearch）官方启动服务的脚本
```bash
#!/usr/bin/env bash
# CONTROLLING STARTUP:
#
# This script relies on a few environment variables to determine startup
# behavior, those variables are:
#
#   ES_PATH_CONF -- Path to config directory
#   ES_JAVA_OPTS -- External Java Opts on top of the defaults set
#
# Optionally, exact memory values can be set using the `ES_JAVA_OPTS`. Note that
# the Xms and Xmx lines in the JVM options file must be commented out. Example
# values are "512m", and "10g".
#
#   ES_JAVA_OPTS="-Xms8g -Xmx8g" ./bin/elasticsearch
source "`dirname "$0"`"/elasticsearch-env
parse_jvm_options() {
if[ -f "$1"]; then
    echo "`grep "^-" "$1" | tr '\n' ' '`"
fi
}
ES_JVM_OPTIONS="$ES_PATH_CONF"/jvm.options
ES_JAVA_OPTS="`parse_jvm_options "$ES_JVM_OPTIONS"` $ES_JAVA_OPTS"
# manual parsing to find out, if process should be detached
if! echo $* | grep -E '(^-d |-d$| -d |--daemonize$|--daemonize )'> /dev/null; then
exec \
"$JAVA" \
    $ES_JAVA_OPTS \
-Des.path.home="$ES_HOME" \
-Des.path.conf="$ES_PATH_CONF" \
-cp "$ES_CLASSPATH" \
    org.elasticsearch.bootstrap.Elasticsearch \
"$@"
else
exec \
"$JAVA" \
    $ES_JAVA_OPTS \
-Des.path.home="$ES_HOME" \
-Des.path.conf="$ES_PATH_CONF" \
-cp "$ES_CLASSPATH" \
    org.elasticsearch.bootstrap.Elasticsearch \
"$@" \
<&- &
  retval=$?
  pid=$!
[ $retval -eq 0] || exit $retval
if[ ! -z "$ES_STARTUP_SLEEP_TIME"]; then
    sleep $ES_STARTUP_SLEEP_TIME
fi
if! ps -p $pid > /dev/null; then
exit1
fi
exit0
fi
exit $?
```

