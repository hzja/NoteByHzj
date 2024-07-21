Linux Shell 
<a name="OKCki"></a>
## 1、Shell中的特殊变量
<a name="u5dku"></a>
### 1.1 `$n` 命令-获取脚本命令的参数
基本语法：`$n`<br />n为数字，`$0`代表该脚本的名称，`$1`-`$9`代表第一到第九个参数，10以上的参数用大括号表示`${10}`<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094284-31f3671e-3107-4234-92ae-4e21303d9167.png#averageHue=%23010101&height=369&id=mFp49&originHeight=883&originWidth=1464&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=612)
<a name="TMU01"></a>
### 1.2 `$#` 命令-统计输入参数的个数
基本语法：`$#`<br />获取所有输入参数的个数，常用于循环<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094266-4326afd3-9d82-4d5c-bbe5-9e127a7afd6a.png#averageHue=%23010100&height=446&id=E1xvt&originHeight=1051&originWidth=1443&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=613)
<a name="AaqFd"></a>
### 1.3 `$*` 、 `$@` 命令-获取输入的所有参数
基本语法： `$*` 、 `$@` <br />`$*` ：*变量代表命令行中所有参数，并把所有参数当作一个整体看待<br />`$@` ：@代表命令行所有参数， `$@` 将所有参数区分对待<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094297-897cc165-7a2f-496d-b531-9c600131939e.png#averageHue=%23000000&height=335&id=VoodE&originHeight=796&originWidth=1420&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=597)
<a name="jY0BB"></a>
### 1.4 `$?` 命令-最后一次执行的命令的返回状态
基本语法：`$?`<br />`$?`：最后一次执行的命令的返回状态，变量值为0，则命令执行正确，变量值为非0，则命令有误。<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094248-df3bc5e1-0de6-4f63-835a-7573a1e7ddd4.png#averageHue=%23020101&height=294&id=BFE0E&originHeight=638&originWidth=1362&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=628)
<a name="AFqcO"></a>
### 1.5Shell脚本的定义
脚本以 `#!/bin/bash` 开头
```bash
#!/bin/bash
```
<a name="FUKAu"></a>
#### Shell脚本中将执行的命令返回给变量，使用反引号将执行的命令引用
```shell
a = `pwd`
```
<a name="xoBRF"></a>
### 1.6常用系统变量
<a name="udx5Z"></a>
#### 1.6.1查看系统变量的值
```bash
$HOME，$PWD，$SHELL，$USER
```
<a name="DH2EA"></a>
#### 1.6.2显示当前Shell的所有变量
```bash
root@Fcscanf:~# set
```
<a name="fNH3N"></a>
## 2、自定义变量
<a name="AaNCu"></a>
### 2.1基本语法

1. 定义变量：`变量 = 值`
2. 撤销变量：`unset 变量`
3. 声明静态变量：`readonly` 变量，注意：不能`unset`
<a name="eljaR"></a>
### 2.2变量的定义规则

1. 变量名称可以由字母，数字和下画线组成，但是不能以数字开头，环境变量名建议大写
2. 等号两侧不能有空格
3. 在Bash中，变量默认的都是字符串类型，无法进行数值运算
4. 变量的值如果有空格，需要使用引号括起来
<a name="hkoeP"></a>
### 2.3操作实例
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094313-b009c6f9-72d9-4059-b18c-76049b8b4492.png#averageHue=%23080707&height=339&id=viaP5&originHeight=950&originWidth=1235&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=441)
<a name="uWHEN"></a>
### 2.4变量定义为全局变量供其他shell程序使用
**全局变量定义：**`**export 变量名**`<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094275-01585d31-2253-45b6-a413-9d653edb6c60.png#averageHue=%23010101&height=493&id=dpuzg&originHeight=1350&originWidth=1253&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=458)
<a name="El3h2"></a>
## 3、Shell运算符
<a name="LH4Ac"></a>
### 3.1基本语法

1. “`$((运算式))`”或“`$[运算式]`”
2. `expr +，-，\*，/，%：加，减，乘，除，取余`（expr运算符间要有空格）

![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094322-d3d09c10-6f45-47ce-bce9-07bdbdb8ab5b.png#averageHue=%230d0d0c&height=238&id=u3y7v&originHeight=679&originWidth=1568&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=550)
<a name="vnzED"></a>
## 4、条件判断
<a name="wqte2"></a>
### 4.1基本语法
`[condition] （注意condition前后要有空格）`<br />条件非空即为true<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094336-76af3986-6202-45d9-92e3-6dce3e173859.png#averageHue=%23020101&height=92&id=kGKvr&originHeight=277&originWidth=1621&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=540)
<a name="jgm2H"></a>
### 4.2常用判断条件
<a name="SLKBn"></a>
#### 4.2.1两个整数之间比较

1. `=` 字符串比较
2. `-lt` 小于（less than）
3. `-le` 小于等于（less equal）
4. `-eq` 等于 （equal）
5. `-gt` 大于 （greater than）
6. `-ge` 大于等于 （greater equal）
7. `-ne` 不等于 （Not equal）
<a name="XO8xu"></a>
#### 4.2.2按照文件权限进行比较

1. `-r` 读的权限read
2. `-w` 写的权限write
3. `-x` 执行的权限execute
<a name="OSYkQ"></a>
#### 4.2.3按照文件类型进行判断

1. `-f` 文件存在并且是一个常规文件file
2. `-e` 文件存在existence
3. `-d` 文件存在并且是一个目录directory

![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094352-2a4b4654-fafa-4824-89f9-b68ed54446e9.png#averageHue=%23050505&height=226&id=jVEIt&originHeight=507&originWidth=1214&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=542)
<a name="nrsJ3"></a>
## 5、流程控制
<a name="l49JY"></a>
### 5.1 if判断
<a name="AoJ4p"></a>
#### 5.1.1基本语法
```shell
if [ 条件判断式 ];then
程序
fi
或者
if [ 条件判断式 ]
  then
  程序
fi
```
<a name="OjPLS"></a>
#### 5.1.2注意事项

1. [ 条件判断式 ]，中括号和条件判断式之间必须有空格
2. if后要有空格
<a name="owLse"></a>
#### 5.1.3操作实例
```shell
#!/bin/bash
if [ $1 -eq 1 ]
then
		echo "hello world"
elif [ $1 -eq 2 ]
then
		echo "hello world 2"
fi
```
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094322-f848eb1b-c892-47a5-8724-08cbf8a6c3e9.png#averageHue=%23000000&height=210&id=OBAIj&originHeight=446&originWidth=966&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=455)
<a name="aBM5l"></a>
### 5.2 case语句
<a name="oNzGZ"></a>
#### 5.2.1基本语法
```shell
case $ 变量名 in
"值1")
  程序-如果变量值为值1，执行此语句
  ;;             //相当于其他语言的break
"值2")
  程序-如果变量值为值2，执行此语句
  ;;
*)               //相当于其他语言的default
  程序-找不到以上值执行此语句
  ;;
  esac
```
<a name="Es9RD"></a>
#### 5.2.2注意事项

1. `case`行尾必须为单词“`in`”，每一个模式必须以右括号“`)`”结束
2. 双分号“`;;`”表示命令序列结束，相当于Java中的`break`
3. 最后的"`*)`"表示默认模式，相当于Java中的`default`
<a name="K0OnC"></a>
#### 5.2.3操作实例
```shell
#!/bin/bash
case $1 in
    1)
    		echo "1 toch"
   			;;
    2)
    		echo "2 toch"
    		;;
    *)
    echo "more toch"
    		;;
esac
```
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094364-7a3245d6-7c25-46e4-a78d-aee19af3c314.png#averageHue=%23000000&height=491&id=ChPlU&originHeight=1203&originWidth=1002&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=409)
<a name="aLEKY"></a>
### 5.3 for循环
<a name="uIsbp"></a>
#### 5.3.1基本语法
```shell
for (( 初始值;循环控制条件;变量变化 ))
  do
    程序
  done
或将输入的值赋给变量进行循环输出
for 变量 in 值1 值2 值3 ...
  do
    程序
  done
```
<a name="uxSJB"></a>
#### 5.3.2操作实例
<a name="hJKN7"></a>
##### A、定义值范围的循环
```shell
#!/bin/bash
s=0
for((i=1;i<=100;i++))
do
		s=$[$s+$i]
done
echo $
```
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094388-86e37f04-a3de-4759-8f9a-2d088522e6fc.png#averageHue=%23000000&height=310&id=tPaj6&originHeight=798&originWidth=964&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=374)
<a name="rHjtI"></a>
##### B、赋值输出示例
```shell
#!/bin/bash
for i in $*
do
		echo "fcc is $i"
done
```
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094350-c6c3a553-2ea7-47f9-8d37-953ec35c7f53.png#averageHue=%23000000&height=363&id=iHLk8&originHeight=1008&originWidth=1306&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=470)
```shell
#!/bin/bash
for i in $*
do
		echo "fcc is $i"
done
for j in $@
do
		echo "fcc is $j"
done
```
整体与分开的参数传递
```shell
#!/bin/bash
for i in "$*"
do
		echo "fcc is $i"
done
for j in "$@"
do
		echo "fcc is $j"
done
```
<a name="a3mQA"></a>
### 5.4 while循环
<a name="fmC2p"></a>
#### 5.3.1基本语法
```shell
while [ 条件判断式 ]
do
  程序
done
```
<a name="IeYEx"></a>
#### 5.3.2注意事项

1. `while`后面要用空格隔开
2. "`[]`"里的条件判断式要用空格隔开
<a name="NZ2ry"></a>
#### 5.3.3操作实例
```shell
#!/bin/bash
s=0
i=1
while [ $i -le 100 ]
do
    s=$[$s + $i]
    i=$[$i + 1]
done
echo $
```
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094380-349c78c9-627d-4589-ba13-43f07106d5bd.png#averageHue=%23000000&height=381&id=MBprp&originHeight=906&originWidth=1044&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=439)
<a name="mVfh7"></a>
## 6、read读取控制台输入
<a name="sfaMW"></a>
### 6.1基本语法
基本语法：`read(选项)(参数)`<br />选项：<br />-p：指定读取值时的提示符<br />-t：指定读取值时的等待时间（秒）<br />参数：<br />变量：指定读取值时的变量名
<a name="PE0DZ"></a>
### 6.2操作示例
```shell
#!/bin/bash
read -t 7 -p "Enter your name in 7 seconds" NAME
echo $NAME
```
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094383-145d2172-2733-432c-a14b-9c7c0dc3088d.png#averageHue=%23000000&height=317&id=ntPvL&originHeight=731&originWidth=1237&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=537)
<a name="uyixQ"></a>
## 7、函数
<a name="DYCd3"></a>
### 7.1系统函数
<a name="0aGZJ"></a>
#### 7.1.2 basename
基本语法：`basename [string/pathname] [suffix]` <br />basename命令会删除所有前缀包括最后一个('/')字符，然后将字符串显示出来<br />选项：<br />suffix为后缀，如果suffix被指定了，basename会将pathname或string中的suffix去掉<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094367-2ea26432-13dc-441b-a851-1feab61932c0.png#averageHue=%23000000&height=118&id=QOPWR&originHeight=283&originWidth=1378&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=575)
<a name="LgQ2R"></a>
#### 7.1.3 dirname
基本语法：`dirname 文件绝对路径`<br />从给定的包含绝对路径的文件名中去除文件名（非目录的部分），然后返回剩下的路径（目录的部分）<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094436-a05b8c77-7e39-4f8a-bf8d-3ff2417c4957.png#averageHue=%23000000&height=77&id=o2w4q&originHeight=155&originWidth=1225&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=608)
<a name="yL7B4"></a>
### 7.2自定义函数
<a name="YkGfS"></a>
#### 7.2.1基本语法
```shell
[ function ] function[()]
{
  Action;
  [return int;]
}
funname
```
<a name="NlP6y"></a>
#### 7.2.2注意事项

1. 函数必须在调用之前声明
2. 函数返回值只能通过`$?`系统变量获得，可以显示加：`return`，如果不加，将以最后一条命令运行结果作为返回值。return后跟数值n（0-255）
<a name="dsWkr"></a>
#### 7.2.3操作案例
计算两个输入参数的和
```shell
#!/bin/bash
function sum()
{
    s=0
    s=$[ $1 + $2 ]
    echo "$s"
}
read -p "Please input the number1:" n1;
read -p "Please input the number2:" n2;
sum $n1 $n2
```
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094388-ebe9cb8c-4654-45ae-afa4-e06c544e8146.png#averageHue=%23010101&height=379&id=nuABt&originHeight=1000&originWidth=1054&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=399)
<a name="v3Xfs"></a>
## 8、Shell工具
<a name="5Xaos"></a>
### 8.1 cut-剪切数据并输出

1. 基本语法
   1. `cut [选项参数] filename`：默认分隔符是制表符
2. 选项参数说明
   1. -f ：列号，提取第几列
   2. -d：分隔符，按照指定分隔符分隔列

![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094442-6b4026fc-2c3b-4c87-bddd-5b3a235fbe52.png#averageHue=%23000000&height=606&id=u83R0&originHeight=1500&originWidth=1288&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=520)<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094412-d12c4bf5-6f49-4f53-a55a-37d794ed8a96.png#averageHue=%23000000&height=291&id=Ro1gL&originHeight=753&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=742)
<a name="mDsSN"></a>
### 8.2 sed-流编辑器

1. 基本语法
   1. `sed [参数列表] 'command' filename`
2. 选项参数说明
   1. -e：直接在指令列模式进行sed的动作编辑
3. 命令功能描述
   1. a：新增，a的后面可以接字串，在下一行出现
   2. d：删除
   3. s：查找并替换
- 在输出流插入

![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094430-cddc16a7-c7e1-4f50-a26a-e91367691d60.png#averageHue=%23030303&height=292&id=usNEG&originHeight=803&originWidth=1234&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=449)

- 在输出流删除

![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094421-1a86451f-a5e6-4b82-970f-04c39034fab6.png#averageHue=%23030302&height=277&id=v3OF4&originHeight=729&originWidth=1170&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=445)

- 在输出流替换内容，g=global表示全部替换

![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094450-e468343b-621b-4c3b-a2b7-222d4e15feb1.png#averageHue=%23040404&height=185&id=EIjGK&originHeight=454&originWidth=1230&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=501)

- 将第二行删除并替换

![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094484-a72c2e8b-3963-4ee2-804e-83933aefd269.png#averageHue=%23020101&height=135&id=riwAM&originHeight=368&originWidth=1519&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=557)
<a name="2A7bo"></a>
### 8.3 awk-文本分析工具
文本分析工具，把文件逐行读取，以空格为默认分隔符将每行切片，切开的部分进行分析处理。
<a name="0nUcp"></a>
#### 8.3.1基本语法

1. 基本语法：`awk [选项参数] 'pattern1{action1} pattern2{action2} ...' filename`
   1. pattern：表示awk在数据中查找的内容，就是匹配模式
   2. action：在找到匹配内容时所执行的命令
2. 选项参数说明
   1. -F：指定输入文件折分隔符
   2. -v：赋值一个用户定义变量
3. 内置变量
   1. FILENAME：文件名
   2. NR：已读的记录数
   3. NF：浏览记录的域的个数（切割后，列的个数）
<a name="8EWCz"></a>
#### 8.3.2操作实例
```bash
root@Fcscanf:/home/fc# awk -F: '/^root/{print $7}' passwd
/bin/bash
root@Fcscanf:/home/fc# awk -F: '/^root/{print $1","$7}' passwd
root,/bin/bash
root@Fcscanf:/home/fc# awk -F: 'BEGIN{print "user,shell"} {print $1","$7} END{print "dahai,bin/sh"}' passwd
user,shell
root,/bin/bash
daemon,/usr/sbin/nologin
bin,/usr/sbin/nologin
sys,/usr/sbin/nologin
sync,/bin/sync
games,/usr/sbin/nologin
man,/usr/sbin/nologin
lp,/usr/sbin/nologin
mail,/usr/sbin/nologin
news,/usr/sbin/nologin
uucp,/usr/sbin/nologin
proxy,/usr/sbin/nologin
www-data,/usr/sbin/nologin
backup,/usr/sbin/nologin
list,/usr/sbin/nologin
irc,/usr/sbin/nologin
gnats,/usr/sbin/nologin
nobody,/usr/sbin/nologin
systemd-network,/usr/sbin/nologin
systemd-resolve,/usr/sbin/nologin
syslog,/usr/sbin/nologin
messagebus,/usr/sbin/nologin
_apt,/usr/sbin/nologin
lxd,/bin/false
uuidd,/usr/sbin/nologin
dnsmasq,/usr/sbin/nologin
landscape,/usr/sbin/nologin
sshd,/usr/sbin/nologin
pollinate,/bin/false
dahai,bin/sh
root@Fcscanf:/home/fc# awk -F: -v i=1 '{print $3+$i}' passwd
0
1
2
3
4
5
6
7
8
9
```
![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094487-e68ab4ad-31a4-46e1-8cf4-c0e66951a33c.png#averageHue=%230a0909&height=209&id=jjpV2&originHeight=209&originWidth=1617&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1617)<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094503-ce716356-3f25-4e28-9391-8221fd10d6cc.png#averageHue=%23000000&height=1165&id=rP5Ek&originHeight=1165&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1920)<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094510-9b23070c-8e00-4cd6-8a20-75e3568fb400.png#averageHue=%23000000&height=199&id=R0469&originHeight=554&originWidth=1568&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=563)<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094427-b9adb167-9cf5-4215-88ac-d4c230a43e23.png#averageHue=%23000000&height=812&id=i9O8c&originHeight=812&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1920)<br />![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094524-cee76000-a499-4bcd-9b7b-65e5d2ea6013.png#averageHue=%23000000&height=657&id=OCUSh&originHeight=657&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1920)
<a name="0pQNQ"></a>
### 8.4 `sort`-将文件排序输出
<a name="lf1kT"></a>
#### 8.4.1基本语法

1. 基本语法：`sort(选项)(参数)`
   1. -n：按照数值的大小排序
   2. -r：以相反的顺序来排序
   3. -t：设置排序时所用的分隔字符
   4. -k：指定需要排序的列
<a name="8Br7d"></a>
#### 8.4.2操作示例
```bash
root@Fcscanf:/home/fc# touch sort.sh
root@Fcscanf:/home/fc# vim sort.sh
root@Fcscanf:/home/fc# cat sort.sh
bb:40:5.4
bd:20:4.2
xz:50:2.3
cls:10:3.5
ss:30:1.6
root@Fcscanf:/home/fc# sort -t : -nrk 2 sort.sh
xz:50:2.3
bb:40:5.4
ss:30:1.6
bd:20:4.2
cls:10:3.5
root@Fcscanf:/home/fc#
```

![](https://cdn.nlark.com/yuque/0/2019/png/396745/1564285094590-fad6dc99-4801-4e41-b224-34c0e8f6057a.png#averageHue=%23030303&height=312&id=OweOv&originHeight=755&originWidth=1231&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=509)
<a name="HRKrS"></a>
## 9、企业面试题
<a name="zRtaW"></a>
### 11.1京东

1. 使用Linux命令查询file 1中空行所在的行号

`awk '/^/{print NR}' sed.txt`

1. 有文件chengji.txt内容如下：

张三 40<br />李四 50<br />王五 60<br />使用Linux命令计算第二列的和并输出<br />`cat chengji.txt | awk -F " " '{sum+=$2} END{print sum}'`
<a name="T0WnI"></a>
### 11.2搜狐&和讯网

1. Shell脚本如何检查一个脚本是否存在，如果不存在该如何处理？
```shell
#!/bin/bash
if [ -f file.txt ]
then
  echo "文件存在！"
else
  echo "文件不存在！"
fi
```
<a name="nZDXs"></a>
### 11.3新浪

1. 用shell写一个脚本，对文本中无序的一列数字排序
```bash
sort -n test.txt | awk '{a+$0;print$0}END{print "SUM="a}'
```
<a name="RJfSR"></a>
### 11.4金和网络

1. 使用shell脚本写出查找当前文件夹/home下的所有文件内容中包含有字符“shen”的文件的名称
```bash
grep -r "shen" /home | cut -d ":" -f 1
```

