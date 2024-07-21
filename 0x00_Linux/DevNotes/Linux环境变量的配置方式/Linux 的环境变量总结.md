Linux<br />Linux 是一个多用户的操作系统。多用户意味着每个用户登录系统后，都有自己专用的运行环境。而这个环境是由一组变量所定义，这组变量被称为环境变量。用户可以对自己的环境变量进行修改以达到对环境的要求。
<a name="qD5Zp"></a>
## 设置环境变量的方法
<a name="eEYCa"></a>
### 对所有用户生效的永久性变量
这类变量对系统内的所有用户都生效，所有用户都可以使用这类变量。作用范围是整个系统。<br />此文件只在 root 下才能修改。
```bash
# vi /etc/profile
export CLASSPATH=./JAVA_HOME/lib:$JAVA_HOME/jre/lib
```
添加完成后新的环境变量不会立即生效，立即生效需要运行 `source /etc/profile` ，否则只能在下次重进此用户时才能生效。
<a name="ee09Z"></a>
### 对单一用户生效的永久性变量
在用户目录下的. bash_profile 文件中添加变量，该文件是隐藏文件，可使用 `ll -a` 查看：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1668781608566-ce301d9f-cf92-4b52-95b1-01fee5d4629f.jpeg#averageHue=%231c1e1c&clientId=u2a25a54c-ccd7-4&from=paste&id=ub7b29e2a&originHeight=126&originWidth=531&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2ad0e2ff-3a7c-4f21-86e6-7b269135886&title=)
```bash
$ whoami 
rethink

$ vi /home/rethink/.bash_profile
export CLASSPATH=./JAVA_HOME/lib:$JAVA_HOME/jre/lib 

$ source /home/rethink/.bash_profile
```
在上图中用红框框住了两个文件：.bashrc 和. bash_profile ，原则上讲设置此类环境变量时在这两个文件任意一个里面添加都是可以的，这两个文件的区别为：.bash_profile 是交互式 login 方式进入 bash shell 运行，.bashrc 是交互式 non-login 方式进入 bash shell 运行。<br />可以理解为. bash_profile 文件只会在用户登录的时候读取一次，而. bashrc 在每次打开终端进行一次新的会话时都会读取。
<a name="sdLEI"></a>
### 临时有效的环境变量 (只针对当前 shell 有效)
此类环境变量只对当前的 shell 有效。当退出登录或者关闭终端再重新打开时，这个环境变量就会消失。是临时的。<br />设置方法：命令行下直接使用 [export 变量名 = 变量值] 定义变量。
```bash
$ export 
$ echo $NAME
rethink
```
<a name="Hme65"></a>
### 设置环境变量常用命令

- echo 用于打印显示环境变量，如：`echo $NAME`；
- export 用于设置新的环境变量，如：`export NAME='rethink'`；
<a name="zKQQY"></a>
### 更新环境变量
更新环境变量直接重新赋值即可：NAME='test'   （注意：变量名前不需要加 $）；

- env 显示当前用户的变量；
- set 显示当前 shell 变量，shell 变量包含用户变量；
- unset 删除一个环境变量，如：`unset NAME`；
- readonly 设置环境变量只读，如：`readonly NAME`  ，只读变量 unset 无效。
<a name="VwRW8"></a>
## 常用环境变量
```bash
PATH
# echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
```
路径之间用冒号分隔，这些路径都是一些可以找到可执行程序的目录列表。当输入一个指令时，shell 会先检查命令是否是系统内部命令，如果不是则会再去检查此命令是否是一个应用程序，shell 会试着从 PATH 中寻找这些应用程序。<br />如果 shell 在这些路径目录里没有找到可执行文件，就会报错；若找到，系统将调用执行应用程序。通过设置 PATH ，可以运行程序或指令更加方便。<br />将某个目录路径加入到 PATH 中，可以这样写：
```bash
$ pwd
/root/docker/httpd
$ export PATH=$PATH:$PWD
$ echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin:/root/docker/httpd
`可以看到在PATH后面已经加上了当前所处目录的路径`
```
<a name="gAdi5"></a>
### HOME—用户的主工作目录
用户的主工作目录，即为用户登录到 Linux 系统中时的默认目录。
```bash
$ whoami
rethink
$ echo $HOME
/home/rethink
```
<a name="Tlo91"></a>
### HISTSIZE—保存历史命令的条数
输入的指令都会被系统保存下来，这个环境变量记录的就是保持指令的条数。一般为 1000。
```bash
$ echo $HISTSIZE
1000
$ HISTSIZE=1001
$ echo $HISTSIZE
1001
```
历史命令是被保存在内存中的，当退出或者登录 shell 时，会自动保存或读取可以通过 history 命令来查看。可以使用符号 ! 执行指定序号的历史命令。例如，要执行第 2 个历史命令，则输入! 2。
```bash
$ history 5
   59  ls
   60  who
   61  history | head -n 5
   62  who am i
   63  history 5
$ !62
who am i
root     pts/0        2018-04-04 11:00 (101.81.92.18)
```
<a name="N5al0"></a>
### LOGNAME—当前用户登录名
```bash
$ echo $LOGNAME
rethink
```
<a name="tGrQN"></a>
### HOSTNAME—主机名称
```bash
$ echo $HOSTNAME
JDu4e00u53f7
```
<a name="OrINk"></a>
### SHELL—当前用户使用的 shell 种类
```bash
$ echo $SHELL
/bin/bash
```
