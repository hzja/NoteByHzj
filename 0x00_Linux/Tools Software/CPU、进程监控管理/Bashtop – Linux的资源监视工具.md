Linux Bashtop <br />Bashtop是Linux中基于终端的资源监控实用程序。它是一个漂亮的命令行工具，可以直观地显示CPU、内存、正在运行的进程和带宽的统计数据。<br />使用Bashtop，还可以对进程进行排序，以及轻松地在各种排序选项之间切换。此外，可以将**SIGKILL**，**SIGTERM**和**SIGINT**发送到所需的进程。<br />Bashtop可以安装在Linux，macOS甚至FreeBSD上。
<a name="XE1yA"></a>
### 先决条件
要成功安装Bashtop，请确保系统中已准备好以下依赖项。<br />Bash 4.4或更高版本•Git•GNU Coreutils•GNU sed，awk，grep和ps命令行工具•Lm传感器–可选（用于收集CPU温度统计信息）<br />开始手动安装Bashtop。这应该适用于所有发行版。
<a name="GXJPf"></a>
### 手动安装
要手动安装Bashtop，克隆git仓库，并使用以下命令从源代码编译：
```bash
$ git clone https://github.com/aristocratos/bashtop.git
$ cd bashtop
$ sudo make install
```
要卸载Bashtop，运行：
```bash
$ sudo make uninstall
```
<a name="ddAsN"></a>
#### 在Ubuntu中安装Bashtop
在Ubuntu上安装Bashtop的方法有两种：使用snap或使用APT软件包管理器。<br />要使用snap安装，执行：
```bash
$ snap install bashtop
```
要使用APT软件包管理器进行安装，请首先添加如下所示的Bashtop PPA：
```bash
$ sudo add-apt-repository ppa:bashtop-monitor/bashtop
```
接下来，更新软件包列表，然后如图所示安装Bashtop。
```bash
$ sudo apt update$ sudo apt install bashtop
```
<a name="PEoPB"></a>
#### 在Debian中安装Bashtop
Bashtop在Debian的官方存储库中可用。要安装它，只需运行以下命令：
```bash
$ sudo apt install bashtop
```
另外，可以运行显示的命令。
```bash
$ git clone https://github.com/aristocratos/bashtop.git
$ cd bashtop/
$ cd DEB
$ sudo ./build
```
<a name="XM5yw"></a>
#### 在Fedora中安装Bashtop
要将Bashtop放入Fedora中，只需运行以下命令：
```
$ sudo dnf install bashtop
```
<a name="3wgoK"></a>
#### 在CentOS / RHEL 8中安装Bashtop
对于CentOS 8 / RHEL 8系统，需要首先启用EPEL存储库，然后再运行以下命令：
```bash
$ sudo yum install epel-release
$ sudo dnf install bashtop
```
<a name="I8pEJ"></a>
#### 在Arch Linux上安装Bashtop
Bashtop在AUR以bashtop-git的形式提供。要安装Bashtop，只需运行：
```bash
$ sudo pacman -S bashtop
```
<a name="Hhhit"></a>
### 使用Bashtop资源监视器
要启动Bashtop，只需在终端上运行以下命令:
```bash
$ bashtop
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597074041354-b55f3a7d-136a-450e-9c93-cce35a3ab270.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=1817931&status=done&style=none&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597073972537-12f7ace6-3aa0-460c-ae44-9695ea3eca4f.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2127699&status=done&style=none&width=1107.6666666666667)Bashtop – Linux资源监视工具
<a name="1dlwt"></a>
### Bashtop配置
Bashtop的配置文件位于`~/.config/bashtop/bashtop.cfg`位置。可以根据需要更改参数，以自定义终端上指标的外观和输出。<br />以下是默认配置的示例：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597074083559-a2b6124e-42d7-4cfd-8d0e-4d2a86f7028e.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2105942&status=done&style=none&width=1107.6666666666667)Bashtop配置<br />若要查看命令和快捷键，请按`Esc`键，然后使用向下箭头键选择`Help`选项。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597074152265-1630fd81-ed75-4feb-8525-88059b1ef60b.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2029420&status=done&style=none&width=1107.6666666666667)<br />Bashtop菜单<br />这将打印出下面的菜单和所有命令选项，如图所示。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597074180744-43e35d63-00b3-439f-9dc1-0c0f7bf10c59.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2065446&status=done&style=none&width=1107.6666666666667)Bashtop选项
<a name="tIXPr"></a>
### 结论
Bashtop提供了一种监视Linux系统资源的绝佳方式。但是，它比top和HTOP慢得多，而且有点占用资源。尽管如此，它提供了关于各种系统指标的重要信息。
