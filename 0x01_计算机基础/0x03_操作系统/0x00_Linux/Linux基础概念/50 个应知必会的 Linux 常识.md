Linux
<a name="YyF5J"></a>
### 1、存放用户账号的文件在哪里?
```
/etc/passwd
```
<a name="HDpuN"></a>
### 2、如何删除一个非空的目录?
```bash
rm -rf 目录名
```
<a name="iClxk"></a>
### 3、查看当前的工作目录用什么命令?
```
pwd
```
<a name="oMorC"></a>
### 4、创建一个文件夹用什么命令?
```
mkdir
```
<a name="CKHC3"></a>
### 5、哪个 Linux 命令可以一次显示一页内容？上一页和下一页使用什么命令?
```
more

Space键：显示文本的下一屏内容,B键：显示上一屏内容
```
<a name="H6Vmj"></a>
### 6、显示一个文件的最后 10 行用什么命令
```
tail

tail -n 10 文件名
```
<a name="GcNOn"></a>
### 7、如何回到当前用户的家目录?
```
cd ~
```
<a name="bnSIR"></a>
### 8、如何统计一个文件的行数？
```
wc -l 文件名
```
<a name="ntswS"></a>
### 9、使用什么命令检测网络是否畅通?
```
ping
```
<a name="MtmDv"></a>
### 10、配置文件存放在哪个目录下?
```
/etc
```
<a name="YsEcO"></a>
### 11、有关于网卡的配置文件存放哪个目录下?
```
/etc/sysconfig/network-scripts
```
<a name="vnexL"></a>
### 12、强制递归删除一个文件夹用什么命令?
```
rm -rf 文件名
```
<a name="XJ2kF"></a>
### 13、如何复制一个文件，使用什么命令?
```
cp -R 文件名 新文件名
```
<a name="FxmJF"></a>
### 14、如何创建一个文件夹，使用什么命令?
```
mkdir 文件名
```
<a name="vAzRR"></a>
### 15、如何创建一个新文件，并且向文件内部写入数据 "清华大学"，并在当前的文件里面追加数据 "计算机科学与技术"
```bash
touch file

echo "清华大学" > hello

echo "计算机科学与技术" >> hello
```
<a name="yf59q"></a>
### 16、查看文件的内容用什么命令?
```
cat 文件名

more 文件名
```
<a name="lRpwu"></a>
### 17、如何查看当前的系统时间
```
date
```
<a name="nAUrk"></a>
### 18、重启用什么命令?
```
reboot
```
<a name="QiN5x"></a>
### 19、关机使用什么命令?
```
halt -pshutdown nowreboot -p
```
<a name="l7KiR"></a>
### 20、sbin 目录和 bin 目录有什么区别?
/bin 目录存在所有的系统指令 (linux 默认的自带指令)，这个目录中的所有指令都可以被所有的 Linux 用户正常的使用 (包括普通用户)，也可以被 root 使用。<br />/sbin 目录存放的指令是只有 root 才可以使用的指令，s 表示 super 的意思，因此 sbin 就是超级管理指令，普通用户不能执行 / sbin 当中的指令
<a name="JgY6Z"></a>
### 21、root 用户的家目录在哪里?
```
/root
```
<a name="Kh5BQ"></a>
### 22、在 linux 中 `cd ..` 是什么意思?
返回上一级目录
<a name="VQrGy"></a>
### 23、`2>` `1>` `2>&1` 是什么意思?
```
1 是标准输出
2 是错误输出

> 是输出重定向符号。

2>&1 是把错误输出导入（合并）到标准输出流中
```
<a name="HiSJG"></a>
### 24、管道符 `|` 有什么用处?
“|”是管道命令操作符，简称管道符。利用 Linux 所提供的管道符 “|” 将两个命令隔开，管道符左边命令的输出就会作为管道符右边命令的输入。
<a name="P2xe5"></a>
### 25、如何同时创建 100 个文件?
```
touch file{1…100}
```
<a name="cJ9ZV"></a>
### 26、在根目录下 找出 名字包含 bin 的文件夹
```
ll | grep bin
```
<a name="ho67X"></a>
### 27、vi 编辑器有几种模式?
vi 编辑器有 3 种基本工作模式，分别是命令模式、插入模式和可视化模式。
<a name="AZzJx"></a>
### 28、如何进入 vi 编辑器的插入模式
```
按下i
```
<a name="kxllF"></a>
### 29、如何进入 vi 编辑器的可视化模式
```
按下v
```
<a name="Spkuo"></a>
### 30、在 vi 编辑器中如何复制一行
```
按下y
```
<a name="Z5bSH"></a>
### 31、在 vi 编辑器中如何进行粘贴
```
按下p
```
<a name="GU8Ao"></a>
### 32、如何删除从 3 行到 15 行的所有数据
```
:3,15d
```
<a name="wrlmt"></a>
### 33、创建用户 zhangsan 用什么命令
```
useradd zhangsan
```
<a name="Ahwfi"></a>
### 34、给 zhangsan 用户设置密码为 123456?
```
passwd zhangsan123456
```
<a name="H13Zg"></a>
### 35、删除用户用什么命令?
```
userdel 用户名
```
<a name="GyLhv"></a>
### 36、切换用户用什么命令
```
su 用户名
```
<a name="eXtMp"></a>
### 37、zhangsan 用户的家目录在哪里?
```
/home/zhangsan
```
<a name="XIfLX"></a>
### 38、-rwxrwxrwx 是什么意思?
普通文件 当前用户可读可写可执行 当前用户组内用户可读可写可执行 其他用户可读可写可执行
<a name="pVcCU"></a>
### 39、drwx------ 是什么意思?
文件夹 当前用户可读可写可执行
<a name="fFT9p"></a>
### 40、如何修改一个文件夹的权限为 自己可读可写可执行 其他的都不允许
```bash
chmod 700
```
<a name="yT08F"></a>
### 41、创建组 super，将 zhangsan 加入到 super 组里面
```bash
groupadd super

usermod -g super zhangsan
```
<a name="IUS6B"></a>
### 42、使用 zhangsan 用户登录，在根目录下创建文件夹 test，在 test 文件夹下创建 test1.txt，修改这个文件夹的所有者为 root:root
```
chown root:root test1.txt
```
<a name="z7rca"></a>
### 43、查看命令的帮助用什么命令?
```
man
```
<a name="xmBKC"></a>
### 44、如何安装软件?
```
yum install 软件名
```
<a name="ivzLU"></a>
### 45、如何卸载软件?
```
yum remove 软件名
```
<a name="niUQy"></a>
### 46、wget 命令有什么用?
下载文件
<a name="C5Q8p"></a>
### 47、如何查看某个命令安装在哪里？
```
whereis 命令
```
<a name="loHvM"></a>
### 48、vi 编辑器保存并且退出用什么命令
```
:wq
```
<a name="iHUqP"></a>
### 49、vi 编辑器不保存并且退出用什么命令
```
:q!
```
<a name="PFoU0"></a>
### 50、如何查看当前的 ip 地址?
```bash
ifconfig
ip a
```
