Linux
<a name="WnjEO"></a>
## awk是什么
一个强大的文本分析工具，把文件逐行的读入，以空格为默认分隔符将每行切片，切开的部分再进行分析处理。
<a name="yeMeF"></a>
## 基本用法
```bash
awk [选项] '[/pattern1/]{action1} [/pattern2/]{action2}...' filename
```

- []包含的表示可选的
- pattern：表示awk在数据中查找的内容，正则匹配模式
- action：在找到匹配的内容时执行的一系列命令
<a name="BZ0yr"></a>
### 选项说明
| **选项** | **功能** |
| --- | --- |
| -F | 指定分隔符 |
| -v | 赋值一个用户定义的变量 |

<a name="NSJUu"></a>
### 案例
（1）准备数据，创建awk.txt，内容如下
```
java;100;90
linux;120;110
php;90;80
c;150;140
c++;200;190
java;100;200
```
（2）下面匹配awk.txt中以java开头的行，按照;分割为3列，将每行第3列输出
```bash
[root@test001 shells]# awk -F ';' '/^java/{print $3}' awk.txt 
90
200
```
（3）下面匹配awk.txt中以java开头的行，按照`;`分割为3列，并输出第1列和第3列，之间以-》隔开
```bash
[root@test001 shells]# awk -F ';' '/^java/{print $1"-》"$2}' awk.txt
java-》100
java-》100
```
（4）下面匹配awk.txt中以java开头的行，按照;分割为3列，并输出第1列和第3列，之间以-》隔开，并在所有结果输出之前添加一行“start”，结束后添加一行“end”<br />注意： BEGIN 在所有数据读取行之前执行； END 在所有数据执行之后执行。
```bash
[root@test001 shells]# awk -F ';' 'BEGIN{print "start"} /^java/{print $1"-》"$2} END{print "end"}' awk.txt
start
java-》100
java-》100
end
```
（4）对awk.txt按照;分割，输出第1列和第2列，中间用->隔开，将第2列添加1<br />下面用-v来定义一个变量i=1，在{action}中可以直接使用这个变量
```bash
[root@test001 shells]# awk -v i=1 -F ';' '{print $1"->"$2+i}' awk.txt
java->101
linux->121
php->91
c->151
c++->201
java->101
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1693066864873-43510e1d-6cf4-4e0e-877f-06f4d21a2150.png#averageHue=%23050403&clientId=u1a4d4a31-fa7d-4&from=paste&height=164&id=u28b0ee84&originHeight=411&originWidth=2084&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=310709&status=done&style=none&taskId=u864ebcc7-d2e0-4fb1-84d7-ad622c1d4d8&title=&width=833.6)
<a name="qZxFD"></a>
## awk内置变量
| **变量** | **说明** |
| --- | --- |
| FILENAME | 文件名 |
| NR | 已读的记录数（行号） |
| NF | 浏览记录的域的个数（切割后， 列的个数） |

<a name="myVWK"></a>
### 案例：内置变量用法
<a name="wKhV1"></a>
#### （1）统计passwd文件名，每行的行号，每行的列数
```bash
[root@test001 shells]# awk -F ":" '{print "filename:"FILENAME",linenum:"NR",col:"NF}' /etc/passwd
filename:/etc/passwd,linenum:1,col:7
filename:/etc/passwd,linenum:2,col:7
filename:/etc/passwd,linenum:3,col:7
filename:/etc/passwd,linenum:4,col:7
```
<a name="Exq5e"></a>
#### （2）查询ifconfig命令输出结果中的空行所在的行号
```bash
[root@test001 shells]# ifconfig | awk '/^$/{print NR}'
9
18
26
```
<a name="Y38SW"></a>
### 案例：找出指定的进程，并kill
找出bash相关的所有进程，并kill，下面一步步演示过程。
```bash
[root@test001 shells]# ps -ef | grep " bash$"
root      32198  29899  0 5月22 pts/2   00:00:01 bash
root     127354 127346  0 5月22 pts/1   00:00:00 bash
root     127491 127354  0 5月22 pts/1   00:00:00 bash
[root@test001 shells]# ps -ef | grep " bash$" | awk '{print $2}'
32198
127354
127491
[root@test001 shells]# ps -ef | grep " bash$" | awk '{print $2}' | xargs 
32198 127354 127491
[root@test001 shells]# ps -ef | grep " bash$" | awk '{print $2}' | xargs kill -9
[root@test001 shells]# ps -ef | grep " bash$" | awk '{print $2}' | xargs
```
上面整个过程，用一行命令即可操作，如下，可以将这条命令改写一下，用来结束一些匹配的进程
```bash
[root@test001 shells]# ps -ef | grep " bash$" | awk '{print $2}' | xargs kill -9
```
**这里用到了一个xargs命令，这个命令是干嘛的呢？**<br />管道实现的是将前面的stdout作为后面的stdin，但是有些命令不接受管道的传递方式，最常见的就是ls命令。有些时候命令希望管道传递的是参数，但是直接用管道有时无法传递到命令的参数位，这时候需要xargs，xargs实现的是将管道传输过来的stdin进行处理然后传递到命令的参数位上。**也就是说xargs完成了两个行为：处理管道传输过来的stdin；将处理后的传递到正确的位置上。**<br />可以试试运行下面的几条命令，应该能很好理解xargs的作用了:
```bash
[root@test001 shells]# echo "/etc/inittab" | cat
/etc/inittab
[root@test001 shells]# echo "/etc/inittab" | xargs cat
# inittab is no longer used when using systemd.
#
# ADDING CONFIGURATION HERE WILL HAVE NO EFFECT ON YOUR SYSTEM.
#
# Ctrl-Alt-Delete is handled by /usr/lib/systemd/system/ctrl-alt-del.target
#
# systemd uses 'targets' instead of runlevels. By default, there are two main targets:
#
# multi-user.target: analogous to runlevel 3
# graphical.target: analogous to runlevel 5
#
# To view current default target, run:
# systemctl get-default
#
# To set a default target, run:
# systemctl set-default TARGET.target
#
```
<a name="cEO9k"></a>
## 案例汇总
<a name="ylebQ"></a>
### 打印文件的第一列
```bash
awk '{print $1}' rumenz.txt
```
<a name="OO5pF"></a>
### 打印文件的前两列
```bash
awk '{print $1,$2}' rumenz.txt
```
<a name="cyBmx"></a>
### 打印文件的最后一列
```bash
awk '{print $NF}' rumenz.txt
```
<a name="oCbCY"></a>
### 计算列的总和
```bash
awk '{sum+=$3} END {print sum}' file.txt
```
<a name="VwQZM"></a>
### 打印文件的总行数
```bash
awk 'END{print NR}' rumenz.txt
```
<a name="Tvskj"></a>
### 打印文件的第一行
```bash
awk 'NR==1{print}' rumenz.txt
```
NR是指awk正在处理的记录位于文件中的位置（行号）
<a name="j4V5r"></a>
### 打印文件的第3行第2列
```bash
sed -n '3,1p' rumenz.txt | awk '{print $2}'
```
<a name="PkhAB"></a>
### 删除空行
```bash
awk 'NF' rumenz.txt
```
<a name="ecs8W"></a>
### 打印奇数行
```bash
awk 'b=!b' rumenz.txt
```
<a name="p7YCW"></a>
### 使用自定义分隔符
```bash
awk -F',' '{print $1}' file.csv
```
<a name="Hn363"></a>
### 打印文件按#分割后,行长度为3的所有行
```bash
awk -F '#' 'if(NF==3){print}' rumenz.txt
```
NF是指awk正在处理的记录包含几个域（字段），这与域分隔符有关，默认为空
<a name="ds7b3"></a>
### 统计Linux系统中每个用户所用的shell
```bash
cat /etc/passwd | awk -F ":" '{print $1" : "$7}'
```
<a name="tDo2v"></a>
### 用awk统计Linux系统中所有的用户数
```bash
cat /etc/passwd | awk '{count++}END{ print count}'
```
<a name="VeIIl"></a>
### 统计某个文件夹下文件所占的字节数
```bash
ls -l | awk 'BEGIN{size=0}{size=size+$5}END{print size}'
```
<a name="u3bkt"></a>
### 统计某个文件夹下文件所占的字节数,按M显示
```bash
ls -l | awk 'BEGIN{size=0}{size=size+$5}END{print size}'
```
<a name="JGiYy"></a>
### netstat结合awk统计TCP连接数
```bash
netstat -tunlp | awk '/^tcp/{++a[$6]}END{for(i in a) print i,a[i]}'
```
<a name="XDvs7"></a>
### 过滤空行
```bash
awk '/^[^$]/ {print $0}' rumenz.txt
```
<a name="hMSbN"></a>
### 列运算
```bash
cat 1.txt
1
2
3
```
<a name="TK68T"></a>
#### 求和
```bash
cat 1.txt | awk '{a+=$1}END{print a}'
```
<a name="B5UGT"></a>
#### 求平均值
```bash
cat 1.txt | awk '{a+=$1}END{print a/NR}'
```
<a name="f8YUF"></a>
#### 求列的最大值
```bash
cat 1.txt | awk 'BEGIN{a=0}{if($1>a) a=$1 fi}END{print a}'
```
