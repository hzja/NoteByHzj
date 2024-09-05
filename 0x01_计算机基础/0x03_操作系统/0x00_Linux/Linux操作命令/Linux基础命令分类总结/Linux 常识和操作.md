Linux
<a name="W7JLD"></a>
### 1.存放用户账号的文件在哪里?
```bash
/etc/passwd
```
<a name="y9L6E"></a>
### 2.如何删除一个非空的目录?
```bash
rm -rf 目录名
```
<a name="h3znz"></a>
### 3.查看当前的工作目录用什么命令?
```bash
pwd
```
<a name="WT4U1"></a>
### 4.创建一个文件夹用什么命令?
```bash
mkdir
```
<a name="uteLC"></a>
### 5.哪个Linux命令可以一次显示一页内容？上一页和下一页使用什么命令?
```bash
more
```
Space键：显示文本的下一屏内容,B键：显示上一屏内容
<a name="T8UaT"></a>
### 6.显示一个文件的最后10行用什么命令
```bash
tail

tail -n 10 文件名
```
<a name="WOy5Y"></a>
### 7.如何回到当前用户的家目录?
```bash
cd ~
```
<a name="WfX1o"></a>
### 8.如何统计一个文件的行数？
```bash
wc -l 文件名
```
<a name="Sp1BE"></a>
### 9.使用什么命令检测网络是否畅通?
```bash
ping
```
<a name="KwRkj"></a>
### 10.配置文件存放在哪个目录下?
```bash
/etc
```
<a name="S3Jtr"></a>
### 11.有关于网卡的配置文件存放哪个目录下?
```bash
/etc/sysconfig/network-scripts
```
<a name="SGmpV"></a>
### 12.强制递归删除一个文件夹用什么命令?
```bash
rm -rf 文件名
```
<a name="oEIMy"></a>
### 13.如何复制一个文件,使用什么命令?
```bash
cp -R 文件名 新文件名
```
<a name="Pxiff"></a>
### 14.如何创建一个文件夹,使用什么命令?
```bash
mkdir 文件名
```
<a name="kdorf"></a>
### 15.如何创建一个新文件,并且向文件内部写入数据"清华大学",并在当前的文件里面追加数据"计算机科学与技术"
```bash
touch file

echo “清华大学” > hello

echo “计算机科学与技术” >> hello
```
<a name="H6hnY"></a>
### 16.查看文件的内容用什么命令?
```bash
cat 文件名

more 文件名
```
<a name="xzBqQ"></a>
### 17.如何查看当前的系统时间
```bash
date
```
<a name="ITmeR"></a>
### 18.重启用什么命令?
```bash
reboot
```
<a name="CIQk8"></a>
### 19.关机使用什么命令?
```bash
halt -pshutdown nowreboot -p
```
<a name="iKxvG"></a>
### 20.`sbin`目录和`bin`目录有什么区别?

- /bin目录存在所有的系统指令(linux默认的自带指令),这个目录中的所有指令都可以被所有的Linux用户正常的使用(包括普通用户),也可以被root使用。
- /sbin目录存放的指令是只有root才可以使用的指令,s表示super的意思,因此sbin就是超级管理指令,普通用户不能执行/sbin当中的指令
<a name="DMdyw"></a>
### 21.root用户的家目录在哪里?
```bash
/root
```
<a name="KdUmz"></a>
### 22.在linux中`cd …`是什么意思?
返回上一级目录
<a name="DDejT"></a>
### 23.`2> 1> 2>&1` 是什么意思?
1 是标准输出<br />2 是错误输出<br />`>` 是输出重定向符号。<br />`2>&1` 是把错误输出导入（合并）到标准输出流中
<a name="kYnPB"></a>
### 24.管道符|有什么用处?
“|”是管道命令操作符，简称管道符。利用Linux所提供的管道符“|”将两个命令隔开，管道符左边命令的输出就会作为管道符右边命令的输入。<br />25.如何同时创建100个文件?
```bash
touch file{1…100}
```
<a name="n9rDl"></a>
### 26.在根目录下 找出 名字包含bin的文件夹
```bash
ll | grep bin
```
<a name="deKZ8"></a>
### 27.vi编辑器有几种模式?
vi 编辑器有 3 种基本工作模式,分别是命令模式、插入模式和可视化模式。
<a name="HnOjb"></a>
### 28.如何进入vi编辑器的插入模式
按下i
<a name="ga0py"></a>
### 29.如何进入vi编辑器的可视化模式
按下v
<a name="OwuLz"></a>
### 30.在vi编辑器中如何复制一行
按下y
<a name="dGhPs"></a>
### 31.在vi编辑器中如何进行粘贴
按下p
<a name="qeGlT"></a>
### 32.如何删除从3行到15行的所有数据
`:3,15d`
<a name="sepgk"></a>
### 33.创建用户zhangsan用什么命令
```bash
useradd zhangsan
```
<a name="t5oOX"></a>
### 34.给zhangsan用户设置密码为123456?
```bash
passwd zhangsan123456
```
<a name="HhnBS"></a>
### 35.删除用户用什么命令?
```bash
userdel 用户名
```
<a name="LIP1x"></a>
### 36.切换用户用什么命令
```bash
su 用户名
```
<a name="nipg8"></a>
### 37.zhangsan用户的家目录在哪里?
```bash
/home/zhangsan
```
<a name="oyMzA"></a>
### 38.`-rwxrwxrwx` 是什么意思?
普通文件 当前用户可读可写可执行 当前用户组内用户可读可写可执行 其他用户可读可写可执行
<a name="zHZDj"></a>
### 39.`drwx------` 是什么意思?
文件夹 当前用户可读可写可执行
<a name="XmfX2"></a>
### 40.如何修改一个文件夹的权限为 自己可读可写可执行 其他的都不允许
```bash
chmod 700
```
<a name="VIwIB"></a>
### 41.创建组`super`，将zhangsan加入到`super`组里面
```bash
groupadd super

usermod -g super zhangsan
```
<a name="ehlxQ"></a>
### 42.使用zhangsan用户登录,在根目录下创建文件夹test，在test文件夹下创建test1.txt,修改这个文件夹的所有者为root:root
```bash
chown root:root test1.txt
```
<a name="GOxVv"></a>
### 43.查看命令的帮助用什么命令?
```bash
man
```
<a name="iJOXj"></a>
### 44.如何安装软件?
```bash
yum install 软件名
```
<a name="fJK0c"></a>
### 45.如何卸载软件?
```bash
yum remove 软件名
```
<a name="mJzvg"></a>
### 46.wget命令有什么用?
下载文件
<a name="X5w5B"></a>
### 47.如何查看某个命令安装在哪里？
```bash
whereis 命令
```
<a name="zUTbh"></a>
### 48.vi编辑器保存并且退出用什么命令
```bash
:wq
```
<a name="RuqTY"></a>
### 49.vi编辑器不保存并且退出用什么命令
```bash
:q!
```
<a name="sMv6M"></a>
### 50.如何查看当前的ip地址?
```bash
ifconfigip a
```
