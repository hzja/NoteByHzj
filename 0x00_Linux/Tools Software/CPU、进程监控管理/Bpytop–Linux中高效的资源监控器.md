Linux 资源监控 Bpytop
<a name="tdPkD"></a>
### 准备工作
在开始安装Bpytop之前，请确保系统满足以下要求:

- Python3
- Psutil模块。安装方式`python3 -m pip install psutil`
<a name="6bs0t"></a>
### 安装Bpytop
可以通过两种方式安装Bpytop：从github下载源码包进行编译安装和从各个发行版系统的仓库中安装。
<a name="14LpJ"></a>
#### 源码安装
首先需要从github克隆：
```bash
[root@localhost ~]# git clone https://github.com/aristocratos/bpytop.git
Cloning into 'bpytop'...
remote: Enumerating objects: 121, done.
remote: Counting objects: 100% (121/121), done.
remote: Compressing objects: 100% (72/72), done.
remote: Total 501 (delta 73), reused 95 (delta 48), pack-reused 380
Receiving objects: 100% (501/501), 765.44 KiB | 81.00 KiB/s, done.
Resolving deltas: 100% (298/298), done.
```
克隆完成之后，进入bpytop目录，并编译安装：
```bash
[root@localhost ~]# python3 -m pip install psutil
[root@localhost ~]# cd bpytop/
[root@localhost bpytop]# make install
```
编译安装的时候，需要安装psutil模块，没有安装的话，bpytop不能启动。<br />psutil模块也可以从发行版仓库中安装，例如ubuntu和Centos：
```bash
# Centos8中安装python3-psutil
[root@localhost ~]# yum -y install python3-psutil
# Ubuntu 18.04中安装python3-psutil
fcant@ubuntu-18-04:~$ sudo apt -y install python3-psutil
```
安装完成。
<a name="3I62c"></a>
#### 在各个发行版中使用包管理器安装
除了从源码进行编译外，还可以按以下方式安装Bpytop：<br />**Ubuntu中使用snap包管理器安装**
```bash
bob@ubuntu-18-04:~$ sudo snap install bpytop
bpytop 1.0.22 from James Tigert (kz6fittycent) installed
```
**Centos8中使用dnf/yum包管理器安装**
```bash
[root@localhost ~]# dnf -y install bpytop
```
**FreeBSD中使用pkg包管理器安装**
```bash
$ sudo pkg install bpytop
```
<a name="meq91"></a>
### 启动Bpytop
只需要在终端中输入`bpytop`就可运行：
```bash
$ bpytop
```
Centos8文本模式终端中启动Bpytop<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602430661792-001b2caa-ec68-43e6-9956-8e5083c3b5eb.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2214580&status=done&style=none&width=1107.6666666666667)<br />如果想要修改主题等设置，可以按一下`ESC` 键，然后选择 `OPTIONS`，然后会出现一些选项，可供修改。使用键盘上下左右按键来修改。<br />如果需要提供帮助，可以按一下`h键`或者`ESC` 键 ，选择 `HELP` 选项。可以打开帮助页面。
<a name="Eud9C"></a>
### 退出Bpytop
按一下`q键`就可以退出。或者`ctrl + c`快捷键退出。
