Linux Shell
<a name="0pzIn"></a>
## 一、条件选择、判断
<a name="FENkm"></a>
### 1、条件选择`if`
<a name="12c7d6bc"></a>
#### （1）用法格式
```bash
if 判断条件 1 ; then
  条件为真的分支代码
elif 判断条件 2 ; then
  条件为真的分支代码
elif 判断条件 3 ; then
  条件为真的分支代码
else
  以上条件都为假的分支代码
fi
```
逐条件进行判断，第一次遇为“真”条件时，执行其分支，而后结束整个if。
<a name="oy4bA"></a>
#### （2）经典案例
```bash
#判断年纪
#!/bin/bash
read -p "Please input your age: " age
if [[ $age =~ [^0-9] ]] ;then
    echo "please input a int"
    exit 10
elif [ $age -ge 150 ];then
    echo "your age is wrong"
    exit 20
elif [ $age -gt 18 ];then
    echo "good good work,day day up"
else
    echo "good good study,day day up"
fi
```
**分析**：请输入年纪，先判断输入的是否含有除数字以外的字符，有，就报错；没有，继续判断是否小于150，是否大于18。
```bash
#判断分数
#!/bin/bash
read -p "Please input your score: " score
if [[ $score =~ [^0-9] ]] ;then
    echo "please input a int"
    exit 10
elif [ $score -gt 100 ];then
    echo "Your score is wrong"
    exit 20
elif [ $score -ge 85 ];then
    echo "Your score is very good"
elif [ $score -ge 60 ];then
    echo "Your score is soso"
else
    echo "You are loser"
fi
```
**分析**：请输入成绩，先判断输入的是否含有除数字以外的字符，有，就报错；没有，继续判断是否大于100，是否大于85，是否大于60。
<a name="Tlbmk"></a>
### 2、条件判断 `case`
<a name="7FQ6r"></a>
#### （1）用法格式
```bash
case $name in;
PART1)
  cmd
  ;;
PART2)
  cmd
  ;;
*)
  cmd
  ;;
esac
```
:::danger
注意：`case` 支持glob 风格的通配符：<br /> `*` ：任意长度任意字符<br />  `?` ：任意单个字符<br />  `[]` ：指定范围内的任意单个字符<br /> `a|b` ：a 或b
:::
<a name="AVskA"></a>
#### （2）案例
```bash
#判断yes or no
#!/bin/bash
read -p "Please input yes or no: " anw
case $anw in
[yY][eE][sS]|[yY])
    echo yes
    ;;
[nN][oO]|[nN])
    echo no
    ;;
*)
    echo false
    ;;
esac
```
**分析**：请输入yes or no，回答Y/y、yes各种大小写组合为yes；回答N/n、No各种大小写组合为no。
<a name="BYaFN"></a>
## 二、四个循环
<a name="wKGPx"></a>
### 1、`for`
<a name="wfuvt"></a>
#### （1）用法格式
```bash
# 方式一
for name in 列表 ;do
  循环体
done

# 方式二
for (( exp1; exp2; exp3 )) ;do
  cmd
done
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1614515725726-2cd03b26-c94b-4a66-8b23-758453f0eb4d.png#height=300&id=Jb0c4&originHeight=300&originWidth=594&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=594)<br />exp1只执行一次，相当于在for里嵌了while<br />执行机制：

- 依次将列表中的元素赋值给“变量名”; 每次赋值后即执行一次循环体; 直到列表中的元素耗尽，循环结束
- 列表的表示方法，可以glob 通配符，如{1..10} 、*.sh ；也可以变量引用，如：`seq 1 $name`
<a name="YJUcz"></a>
#### （2）案例
```bash
#求出（1+2+...+n）的总和
sum=0
read -p "Please input a positive integer: " num
if [[ $num =~ [^0-9] ]] ;then
    echo "input error"
elif [[ $num -eq 0 ]] ;then
    echo "input error"
else
    for i in `seq 1 $num` ;do
        sum=$[$sum+$i]
    done
    echo $sum
fi
unset zhi
```
**分析**：sum初始值为0，请输入一个数，先判断输入的是否含有除数字以外的字符，有，就报错；没有判断是否为0，不为0进入for循环，i的范围为1~输入的数，每次的循环为sum=sum+i，循环结束，最后输出sum的值。
```bash
#求出（1+2+...+100）的总和
for (( i=1,num=0;i<=100;i++ ));do
        [ $[i%2] -eq 1 ] && let sum+=i
done
echo sum=$sum
 分析：i=1,num=0；当i<=100，进入循环，若i÷2取余=1，则sum=sum+i，i=i+1。
```
<a name="7CpXM"></a>
### 2、`while`
<a name="1qQIS"></a>
#### （1）用法格式
```
while 循环控制条件 ;do
  循环
done
```
循环控制条件；进入循环之前，先做一次判断；每一次循环之后会再次做判断；条件为“`true`” ，则执行一次循环；直到条件测试状态为“`false`” 终止循环
<a name="QRQGw"></a>
#### （2）特殊用法（遍历文件的每一行）
```bash
while read line; do控制变量初始化
  循环体
done < /PATH/FROM/SOMEFILE
或cat /PATH/FROM/SOMEFILE | while read line; do
  循环体
done
```
依次读取/PATH/FROM/SOMEFILE文件中的每一行，且将行赋值给变量line
<a name="RyoWj"></a>
#### （3）案例
```bash
#100以内所有正奇数之和
sum=0
i=1
while [ $i -le 100 ] ;do
if [ $[$i%2] -ne 0 ];then
    let sum+=i
    let i++
else
    let i++
fi
done
echo "sum is $sum"
```
**分析**：sum初始值为0，i的初始值为1；请输入一个数，先判断输入的是否含有除数字以外的字符，有，就报错；没有当i<100时，进入循环，判断 i÷2取余 是否不为0，不为0时为奇数，sum=sum+i，i+1，为0，i+1；循环结束，最后输出sum的值。
<a name="wAkgn"></a>
### 3、`until` 循环
<a name="bLRzY"></a>
#### （1）用法
```bash
unitl 循环条件 ;do
  循环
done
```
进入条件：循环条件为true ；退出条件：循环条件为false；刚好和while相反，所以不常用，用while就行。
<a name="T6GDZ"></a>
#### （2）案例
```bash
#监控xiaoming用户，登录就杀死
until pgrep -u xiaoming &> /dev/null ;do
        sleep 0.5
done
pkill -9 -u xiaoming
```
**分析**：每隔0.5秒扫描，直到发现xiaoming用户登录，杀死这个进程，退出脚本，用于监控用户登录。
<a name="GNfHb"></a>
### 4、`select` 循环与菜单
<a name="dbh9Q"></a>
#### （1）用法
```bash
select variable in list
do
  循环体命令
done
```
① select 循环主要用于创建菜单，按数字顺序排列的示菜单项将显示在标准错误上，并显示PS3 提示符，等待用户输入<br />② 用户输入菜单列表中的某个数字，执行相应的命令<br />③ 用户输入被保存在内置变量 REPLY 中<br />④ select 是个无限循环，因此要记住用 break 命令退出循环，或用 exit 按 命令终止脚本。也可以按 ctrl+c退出循环<br />⑤ select 和 经常和 case 联合使用<br />⑥ 与for循环类似，可以省略 in list， 此时使用位置参量
<a name="c17dd1b4"></a>
#### （2）案例
```bash
#生成菜单，并显示选中的价钱
PS3="Please choose the menu: "
select menu in mifan huimian jiaozi babaozhou quit
do
        case $REPLY in
        1|4)
                echo "the price is 15"
                ;;
        2|3)
                echo "the price is 20"
                ;;
        5)
                break
                ;;
        *)
                echo "no the option"
        esac
done
```
**分析**：`PS3`是`select`的提示符，自动生成菜单，选择5`break`退出循环。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614517805167-237be761-2621-45c8-916c-7297065bad3c.png#height=234&id=CliZ2&originHeight=703&originWidth=1382&originalType=binary&ratio=1&rotation=0&showTitle=false&size=314078&status=done&style=none&title=&width=460.6666666666667)
<a name="jQdvA"></a>
## 三、循环里的一些用法
<a name="5KPZ8"></a>
### 1、循环控制语句
<a name="c2aea68c"></a>
#### （1）语法
`continue [N]`：提前结束第N层的本轮循环，而直接进入下一轮判断；最内层为第1层 `break [N]`：提前结束第N层循环，最内侧为第1层
```bash
# 例：
while CONDTITON1; do
  CMD1
if CONDITION2; then
  continue / break
fi
  CMD2
done
```
<a name="Mivok"></a>
#### （2）案例
```bash
# 求（1+3+...+49+53+...+100）的和
#!/bin/bash
sum=0
for i in {1..100} ;do
        [ $i -eq 51 ] && continue
        [ $[$i%2] -eq 1 ] && { let sum+=i;let i++; }
done
echo sum=$sum
```
**分析**：做1+2+...+100的循环，当i=51时，跳过这次循环，但是继续整个循环，结果为：sum=2449
```bash
# 求（1+3+...+49）的和
#!/bin/bash
sum=0
for i in {1..100} ;do
        [ $i -eq 51 ] && break
        [ $[$i%2] -eq 1 ] && { let sum+=i;let i++; }
done
echo sum=$sum
```
**分析**：做1+2+...+100的循环，当i=51时，跳出整个循环，结果为：sum=625
<a name="WCK7c"></a>
### 2、循环控制`shift`命令
<a name="DLkBV"></a>
#### （1）作用
用于将参数列表list左移指定次数，最左端的那个参数就从列表中删除，其后边的参数继续进入循环
<a name="YJIIH"></a>
#### （2）案例
```bash
# 创建指定的多个用户
#!/bin/bash
if [ $# -eq 0 ] ;then
        echo "Please input a arg(eg:`basename $0` arg1)"
        exit 1
else
        while [ -n "$1" ];do
                useradd $1 &> /dev/null
                shift
        done
fi
```
**分析**：如果没有输入参数（参数的总数为0），提示错误并退出；反之，进入循环；若第一个参数不为空字符，则创建以第一个参数为名的用户，并移除第一个参数，将紧跟的参数左移作为第一个参数，直到没有第一个参数，退出。
```bash
# 打印直角三角形的字符
#!/bin/bash
while (( $# > 0 ))
do
        echo "$*"
        shift
done
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614517768085-b5efdce0-cfee-405a-a9e2-02bf997739c1.png#height=181&id=GP0nB&originHeight=544&originWidth=1308&originalType=binary&ratio=1&rotation=0&showTitle=false&size=227739&status=done&style=none&title=&width=436)
<a name="Q6WPD"></a>
### 3、返回值结果
```bash
true 永远返回成功结果
: null command ,什么也不干，返回成功结果
false 永远返回错误结果
```
创建无限循环
```bash
while true ;do
  循环体
done
```
<a name="HQbl4"></a>
### 4、循环中可并行执行，使脚本运行更快
<a name="jBQpE"></a>
#### （1）用法
```bash
for name in 列表 ;do
  {
  循环体
  }&
done
wait
```
<a name="At5W2"></a>
#### （2）实例
```bash
#搜寻自己指定ip（子网掩码为24的）的网段中，UP的ip地址
read -p "Please input network (eg:192.168.0.0): " net
echo $net |egrep -o "\<(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\>"
[ $? -eq 0 ] || ( echo "input error";exit 10 )
IP=`echo $net |egrep -o "^([0-9]{1,3}\.){3}"`
for i in {1..254};do
        {
        ping -c 1 -w 1 $IP$i &> /dev/null && \
        echo "$IP$i is up" 
        }&
done
wait
```
**分析**：请输入一个IP地址例192.168.37.234，如果格式不是0.0.0.0 则报错退出；正确则进入循环，IP变量的值为192.168.37.  i的范围为1-254，并行ping 192.168.37.1-154，ping通就输出此IP为UP。直到循环结束。
<a name="iVcE5"></a>
## 四、信号捕获trap
<a name="zCoPd"></a>
### 1、用法格式
`trap ' 触发指令'` 信号，自定义进程收到系统发出的指定信号后，将执行触发指令，而不会执行原操作<br />`trap ''` 信号，忽略信号的操作<br />`trap '-'` 信号，恢复原信号的操作<br />`trap -p`，列出自定义信号操作<br />信号可以3种表达方法：信号的数字2、全名`SIGINT`、缩写`INT`
<a name="pDAXC"></a>
### 2、常用信号
1) `SIGHUP` ：无须关闭进程而让其重读配置文件<br />2) `SIGINT` ：中止正在运行的进程；相当于Ctrl+c<br />3) `SIGQUIT` ：相当于Ctrl+\<br />9) `SIGKILL` ：强制杀死正在运行的进程<br />15) `SIGTERM` ：终止正在运行的进程（默认为15）<br />18) `SIGCONT` ：继续运行<br />19) `SIGSTOP` ：后台休眠<br />9 信号，强制杀死，捕获不住
<a name="ww7G7"></a>
### 3、案例
```bash
# 打印0-9，ctrl+c不能终止
#!/bin/bash
trap 'echo press ctrl+c' 2
for ((i=0;i<10;i++));do
        sleep 1
        echo $i
done
```
**分析**：i=0，当i<10，每休眠1秒，i+1，捕获2信号，并执行echo press ctrl+c<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614517530835-ddc60e61-c253-498a-83ac-6a90d9b52de7.png#height=251&id=OBfXm&originHeight=753&originWidth=1220&originalType=binary&ratio=1&rotation=0&showTitle=false&size=303008&status=done&style=none&title=&width=406.6666666666667)
```bash
# 打印0-3，ctrl+c不能终止，3之后恢复，能终止
#!/bin/bash
trap '' 2
trap -p
for ((i=0;i<3;i++));do
        sleep 1
        echo $i
done
trap '-' SIGINT
for ((i=3;i<10;i++));do
        sleep 1
        echo $i
done
```
**分析**：i=0，当i<3，每休眠1秒，i+1，捕获2信号；i>3时，解除捕获2信号。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614517629923-0cc559ef-f2a5-406e-9c7f-3c00e7847638.png#height=161&id=CXygb&originHeight=482&originWidth=1226&originalType=binary&ratio=1&rotation=0&showTitle=false&size=197165&status=done&style=none&title=&width=408.6666666666667)
<a name="9Z2kr"></a>
## 五、脚本小知识
<a name="pVl0Q"></a>
### 1、生成随机字符 `cat /dev/urandom`
```bash
#生成8个随机大小写字母或数字 
cat /dev/urandom |tr -dc [:alnum:] |head -c 8
```
<a name="XZ7dp"></a>
### 2、生成随机数 `echo $RANDOM`
```
确定范围 echo $[RANDOM%7] 随机7个数（0-6）
echo $[$[RANDOM%7]+31] 随机7个数（31-37）
```
<a name="47MyI"></a>
### 3、`echo`打印颜色字
```bash
echo -e "\033[31malong\033[0m" # 显示红色along
echo -e "\033[1;31malong\033[0m" # 高亮显示红色along
echo -e "\033[41malong\033[0m" # 显示背景色为红色的along
echo -e "\033[31;5malong\033[0m" # 显示闪烁的红色along
color=$[$[RANDOM%7]+31]
echo -ne "\033[1;${color};5m*\033[0m" # 显示闪烁的随机色along
```
<a name="skWDf"></a>
## 六、几个有意思的小脚本
<a name="kfLZm"></a>
### 1、9x9乘法表
```bash
#!/bin/bash
for a in {1..9};do
        for b in `seq 1 $a`;do
                let c=$a*$b ;echo -e "${a}x${b}=$c\t\c"
        done
        echo    
done
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614517038963-52fed78e-dc44-469b-8252-9118a286e264.png#height=216&id=ExQms&originHeight=649&originWidth=1945&originalType=binary&ratio=1&rotation=0&showTitle=false&size=403218&status=done&style=none&title=&width=648.3333333333334)
<a name="OXAjO"></a>
### 2、彩色等腰三角形
```bash
#!/bin/bash
read -p "Please input a num: " num
if [[ $num =~ [^0-9] ]];then
        echo "input error" 
else
        for i in `seq 1 $num` ;do
                xing=$[2*$i-1]
                for j in `seq 1 $[$num-$i]`;do
                        echo -ne " "
                done
                for k in `seq 1 $xing`;do
                        color=$[$[RANDOM%7]+31]
                        echo -ne "\033[1;${color};5m*\033[0m"
                done
                echo
        done
fi
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614517016021-8dbb0ee4-521d-4323-9255-2887ff554e2a.png#height=162&id=XOdUj&originHeight=486&originWidth=1225&originalType=binary&ratio=1&rotation=0&showTitle=false&size=206469&status=done&style=none&title=&width=408.3333333333333)
<a name="gzqW3"></a>
### 3、国际象棋棋盘
```bash
#!/bin/bash
red="\033[1;41m  \033[0m"
yellow="\033[1;43m  \033[0m"
for i in {1..8};do
        if [ $[i%2] -eq 0 ];then
                for i in {1..4};do
                        echo -e -n "$red$yellow";
                done
                echo
        else
                for i in {1..4};do
                        echo -e -n "$yellow$red";
                done
                echo 
        fi
done
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614517731817-a342afe4-06c7-4dbb-9732-799d0ff74cae.png#height=201&id=qaKG4&originHeight=602&originWidth=1320&originalType=binary&ratio=1&rotation=0&showTitle=false&size=187036&status=done&style=none&title=&width=440)
