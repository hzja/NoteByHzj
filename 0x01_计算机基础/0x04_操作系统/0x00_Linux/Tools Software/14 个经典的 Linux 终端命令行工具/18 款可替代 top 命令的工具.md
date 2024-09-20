Linux<br />在 Linux 环境下 top 命令都不陌生，它以实时动态的方式查看系统的整体运行情况，综合了多方信息监测系统性能和运行信息的实用工具，通过 top 命令所提供的互动式界面，可以用热键来进行管理。<br />在介绍 top 命令的替代工具时，先来回顾下 top 的基本语法、常用选项、交互时的热键以及实例解释，从而加深对 top 的理解，同时也希望能进一步的运用在实际场景中。
<a name="ztOcS"></a>
## 一、top
<a name="O0t6C"></a>
### 1.1 top 语法
```bash
top (选项)
```
<a name="PmS03"></a>
### 1.2 top 选项

- -b：以批处理模式操作；
- -c：显示完整的治命令；
- -d：屏幕刷新间隔时间；
- -I：忽略失效过程；
- -s：保密模式；
- -S：累积模式；
- -i<时间>：设置间隔时间；
- -u<用户名>：指定用户名；
- -p<进程号>：指定进程；
- -n<次数>：循环显示的次数；
<a name="cazVh"></a>
### 1.3 top 交互时的热键

- h：显示帮助信息并给出简短的命令总结说明提示；
- k：终止一个进程；
- i：忽略闲置和僵死的进程；
- q：退出 top；
- r：重新安排一个进程的优先级别；
- S：切换到累计模式；
- s：改变两次刷新之间的延迟时间（单位为s），如果有小数，就换算成ms。输入0值则系统将不断刷新，默认值为：5s；
- f 或者 F：从当前显示中添加或者删除；
- o 或者 O：改变显示的顺序；
- l：切换显示平均负载和启动时间信息；
- m：切换显示内存信息；
- t：切换显示进程和 CPU 状态信息；
- c：切换显示命令名称和完整命令行；
- M：根据驻留内存大小进行排序；
- P：根据 CPU 使用百分比大小进行排序；
- T：根据时间或累计时间进行排序；
- w：将当前设置写入 ~/.toprc 文件中；
```bash
top 
- 09:48:47 up 1 day, 10:54,  2 users,  load average: 0.00, 0.02, 0.00
任务: 293 total,   2 running, 291 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.2 sy,  0.0 ni, 99.8 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
MiB Mem :   1945.1 total,    160.5 free,    849.7 used,    934.9 buff/cache
MiB Swap:    923.2 total,    921.4 free,      1.8 used.    904.4 avail Mem

PID USER      PR  NI    VIRT    RES    SHR    %CPU  %MEM     TIME+ COMMAND
6775 root      20   0   21752   4280   3424 R   0.3   0.2   0:05.58 top
```
<a name="yfkts"></a>
### 1.4 top 实例解释
<a name="jnJcY"></a>
#### 第一行：任务队列信息

- top - 09:48:47：显示当前系统时间
- 1 day：系统已经运行了 1 天
- 2 users：2 个用户当前登录
- load average: 0.00, 0.02, 0.00：系统负载，即任务队列的平均长度。三个数值分别为 1分钟、5分钟、15分钟前到现在的平均值。
<a name="nvhnK"></a>
#### 第二行：进程信息

- Tasks: 293 total：总进程数
- 2 running：正在运行的进程数
- 291 sleeping：睡眠的进程数
- 0 stopped：停止的进程数
- 0 zombie：僵尸进程数
<a name="fN4TK"></a>
#### 第三行：CPU 信息

- %Cpu(s):  0.0 us：用户空间占用 CPU 的百分比
- 0.2 sy：内核空间占用 CPU 的百分比
- 0.0 ni：用户进程空间内改变过优先级的进程占用 CPU 的百分比
- 99.8 id：空闲 CPU 的百分比
- 0.0 wa：等待输入输出 CPU 的时间百分比
- 0.0 hi：硬中断占用 CPU 的百分比
- 0.0 si：软中断占用 CPU 的百分比
- 0.0 st：用于有虚拟 CPU 的情况，用来指示被虚拟机偷掉的 CPU 时间
<a name="XgsdN"></a>
#### 第四 / 五行：内存信息

- MiB Mem :   1945.1 total：物理内存总量
- 160.5 free：空闲内存总量
- 849.7 used：使用的物理内存总量
- 934.9 buff/cache：用于内核缓存的内存量
- MiB Swap:    923.2 total：交换区总量
- 921.4 free：空闲交换区总量
- 1.8 used：使用的交换区总量
- 904.4 avail Mem：可用于进程下一次分配的物理内存数量
<a name="qYsXQ"></a>
#### 第六行：进程详细信息

- PID：进程 PID 号
- USER：用户
- PR：优先级
- NI：nice值，负值表示高优先级，正值表示低优先级
- VIRT：进程使用的虚拟内存总量，单位为 KB
- RES：进程使用的、未被换出的物理内存大小，单位为 KB
- SHR：共享内存大小，单位为 KB
- %CPU：上次更新到现在的CPU时间占用百分比
- %MEM：进程使用的物理内存百分比
- TIME+：进程使用的 CPU 时间总计，单位 1/100秒
- COMMAND：命令名 / 命令行

以上就是针对 top 命令的回顾。进程详细信息里还有很多信息，就不一一介绍了，在日常学习和工作中有需要用到的解释说明，可参阅相关资料进行了解。<br />不过 top 已经满足在学习以及工作中排查相关问题的基本条件了。接下来介绍一些针对 top 命令的替代工具。
<a name="qVf9Z"></a>
## 二、bashtop
[bashtop](https://github.com/aristocratos/bashtop)基于 Shell 语言编写，是用于展示当前 Linux 操作系统的处理器、内存、硬盘、网络和进程等各项资源的使用情况与状态，可在 Fedora、CentOS 8、RHEL 8、Ubuntu、Debian、FreeBSD、OSX 等多种操作系统中安装。
<a name="Ph3vC"></a>
### 2.1 bashtop 的特征

- 易使用，具有游戏灵感的菜单系统；
- 快速且“大部分”响应式 UI，带有 UP、DOWN 键进程选择；
- 显示所选进程的详细统计信息的功能；
- 显示当前磁盘的读写速度；
- 过滤流程的能力；
- 在排序选项之间轻松切换；
- 向选定的进程发送 SIGTERM、SIGKILL、SIGINT；
- 可更改所有配置文件选项的 UI 菜单；
- 网络使用情况的自动缩放图；
- 如有新版本可用，则在菜单中进行显示；
- Linux 环境下可切换的多种数据采集方式；
<a name="XkhlM"></a>
### 2.2 bashtop 的安装
<a name="AH4OV"></a>
#### CentOS 8 安装：
```bash
# dnf config-manager --set-enabled PowerTools 
# dnf install epel-release 
# dnf install bashtop
```
<a name="FfSx0"></a>
#### RHEL 8 安装：
```bash
ARCH= $( /bin/arch ) 
subscription-manager repos --enable " codeready-builder-for-rhel-8- ${ARCH} -rpms " 
dnf install epel-release 
dnf install bashtop
```
<a name="U6Vlr"></a>
#### Ubuntu 安装：
```bash
# sudo add-apt-repository ppa:bashtop-monitor/bashtop 
# sudo apt update 
# sudo apt install bashtop
```
安装非常简单，这里用的是 Ubuntu，所以用 `apt install` 直接安装即可，如果用的是其他操作系统，可参考：[https://github.com/aristocratos/bashtop#installation](https://github.com/aristocratos/bashtop#installation) 章节进行安装。
<a name="x5ApW"></a>
### 2.3 bashtop 的使用
安装完毕后，直接执行命令bashtop即可。
```bash
# bashtop
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643942718505-7f8de7a8-3b95-49c3-b4f3-9fa16a997ff9.gif#averageHue=%23000000&clientId=uefd68e2c-cd31-4&from=paste&id=ucdb3818f&originHeight=433&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u95f08bc5-b4fd-4eb9-bb70-6abd82afcca&title=)
<a name="S8Yuv"></a>
## 三、bpytop
[bpytop](https://github.com/aristocratos/bpytop)是 bashtop 的延续，基于 Python 语言编写，主要用于展示当前 Linux 操作系统的处理器、内存、磁盘、网络和进程的使用情况和统计信息的资源监视器，可在 Mac OSX、Arch Linux、Debian、FreeBSD、Fedora、CentOS 8、Ubuntu 等多种操作系统中安装。
<a name="xMvOC"></a>
### 3.1 bpytop 的特征

- 易使用且具有游戏灵感的菜单系统；
- 完全支持鼠标，所有带有突出显示键的按钮都是可点击的，并且鼠标滚动可以在进程列表和菜单框中工作；
- 快速且响应迅速的 UI，带有 UP、DOWN 键进程选择；
- 显示所选进程的详细统计信息的功能；
- 能够过滤进程，可以输入多个过滤器；
- 在排序选项之间轻松切换；
- 向选定的进程发送 SIGTERM、SIGKILL、SIGINT；
- 用于更改所有配置文件选项的 UI 菜单；
- 网络使用情况的自动缩放图；
- 如有新版本可用，则在菜单中显示消息；
- 显示当前磁盘的读写速度；
<a name="zhx0I"></a>
### 3.2 bpytop 的安装
<a name="NNpUQ"></a>
#### Mac OSX 安装：
```bash
# brew install bpytop
```
<a name="manhg"></a>
#### Fedora / CentOS 8 安装：
```bash
# sudo dnf install bpytop
```
<a name="zTGgU"></a>
#### Debian / Ubuntu 安装：
```bash
# sudo apt install bpytop
```
安装非常简单，这里用的是 Ubuntu，所以用 `apt install` 直接安装即可，如果用的是其他操作系统，可参考：[https://github.com/aristocratos/bpytop#installation](https://github.com/aristocratos/bpytop#installation) 章节进行安装。
<a name="HKQpX"></a>
#### 3.3 bpytop 的使用
安装完毕后，直接执行命令`bpytop`即可。
```bash
$ bpytop
```
![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1643942718550-ea37b599-c023-4f47-b42e-8c44dfc939ec.gif#averageHue=%23000000&clientId=uefd68e2c-cd31-4&from=paste&id=udada33ed&originHeight=433&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua3ab1b6c-c50b-4d9b-b735-d440b7b65cd&title=)<br />更多 bpytop 命令行选项
```bash
usage: bpytop [-h] [-b BOXES] [-lc] [-v] [--debug]

optional arguments:
  -h, --help            show this help message and exit
  -b BOXES, --boxes BOXES
                        which boxes to show at start, example: -b "cpu mem net proc"
  -lc, --low-color      disable truecolor, converts 24-bit colors to 256-color
  -v, --version         show version info and exit
  --debug               start with loglevel set to DEBUG overriding value set in config
```
<a name="iPAdJ"></a>
## 四、btop
[btop](https://github.com/aristocratos/btop)基于 C++ 语言编写，主要用于展示当前 Linux 操作系统的处理器、内存、磁盘、网络和进程的使用情况和统计信息的资源监视器。
<a name="rHZTR"></a>
### 4.1 btop 的特征

- 易使用，具有游戏灵感的菜单系统；
- 支持鼠标所有带有突出显示键的按钮都是可点击的，并且鼠标滚动在进程列表和菜单框中工作；
- 快速且响应迅速的 UI，带有 UP、DOWN 键进程选择；
- 显示所选进程的详细统计信息的功能；
- 过滤流程的能力；
- 在排序选项之间轻松切换；
- 进程的树视图；
- 向选定的进程发送任何信号；
- 用于更改所有配置文件选项的 UI 菜单；
- 网络使用情况的自动缩放图；
- 显示磁盘的 IO 活动和速度；
- 电池电量计；
- 图表的可选符号；
<a name="kQrPd"></a>
### 4.2 btop 的安装
```bash
# snap install btop

# 下载压缩包
wget https://github.com/aristocratos/btop/releases/download/v1.2.13/btop-x86_64-linux-musl.tbz
# 下载解压工具
yum install bzip2 -y
# 解压
bunzip2 btop-x86_64-linux-musl.tbz
tar xf btop-x86_64-linux-musl.tar
# 进入解压后的文件夹，进行安装
cd btop
# 指定安装的目录
make install PREFIX=/opt/btop
# 运行
/opt/btop/bin/btop
```
如果是 macOS 系统的话，则直接执行 `brew install btop` 进行安装就好。<br />安装非常简单，这里用的是 Ubuntu，所以用 `snap install` 直接安装即可，如果用的是其他操作系统或通过其他方式进行安装，可参考：[https://github.com/aristocratos/btop#installation](https://github.com/aristocratos/btop#installation) 章节进行安装。
<a name="WdHq1"></a>
### 4.3 btop 的使用
安装完毕后，直接执行命令`btop`即可。
```bash
# btop
```
运行 btop 过后，有很多快捷键可以使用，所有这些标红的字母或者数字都可以输入，还可以用鼠标进行上下选择，回车确定等等。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700616559897-0405c99c-6b1a-4fe1-944b-614a01b2f0d6.png#averageHue=%23191919&clientId=u3438a759-a5fb-4&from=paste&id=u12cab08a&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u466a6501-3496-4cc1-8831-cda9fa7bfcb&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942718568-1b2879bb-9704-4677-ab37-939882e25edc.webp#averageHue=%23172217&clientId=uefd68e2c-cd31-4&from=paste&id=uf36b4b8e&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud78a643b-1fc2-4ab1-93b0-90f8765e58a&title=)<br />更多 bpytop 命令行选项
```bash
usage: btop [-h] [-v] [-/+t] [-p <id>] [--utf-force] [--debug]

optional arguments:
  -h, --help            show this help message and exit
  -v, --version         show version info and exit
  -lc, --low-color      disable truecolor, converts 24-bit colors to 256-color
  -t, --tty_on          force (ON) tty mode, max 16 colors and tty friendly graph symbols
  +t, --tty_off         force (OFF) tty mode
  -p, --preset <id>     start with preset, integer value between 0-9
  --utf-force           force start even if no UTF-8 locale was detected
  --debug               start in DEBUG mode: shows microsecond timer for information collect
                        and screen draw functions and sets loglevel to DEBUG
```
以上三款开源 top 替代工具可以说是 Jakob P. Liljenberg 的三剑客。
<a name="FKY4J"></a>
## 五、bottom
[bottom](https://github.com/ClementTsang/bottom)是用于终端的可定制跨平台图形进程 / 系统监视器，支持 Linux、macOS 和 Windows。
<a name="ml9KX"></a>
### 5.1 bottom 的特征

- 随着时间变化的 CPU 使用率、平均水平和每个核心水平；
- 随着时间变化的 RAM 和交换使用情况；
- 一段时间内的网络 I/O 使用情况；
- 支持放大或缩小显示的当前时间间隔；
- 支持显示磁盘容量、使用情况、温度传感器、电池使用情况的信息；
- 支持显示、排序、搜索有关流程信息的小部件（CPU、内存、网络、进程、磁盘、温度、电池）；
- 支持使用命令行标志或配置文件控制的可定制行为（自定义和预建的颜色主题、更改某些小部件的默认行为、更改小部件的布局、过滤掉磁盘和温度小部件中的条目）；
- 支持键、鼠标绑定相关快捷键；
<a name="gwPDj"></a>
### 5.2 bottom 的安装
可以在 Arch Linux、Debian/Ubuntu、Fedora/CentOS 多种平台或以多种方式都可进行安装。
```bash
# curl -LO https://github.com/ClementTsang/bottom/releases/download/0.6.6/bottom_0.6.6_amd64.deb 
# sudo dpkg -i bottom_0.6.6_amd64.deb
```
<a name="wojJB"></a>
### 5.3 bottom 的使用
安装完毕后，执行命令`btm`即可
```bash
# btm
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942718567-be1cb8b5-1587-4343-886c-549bc9e8f651.webp#averageHue=%230a0d0c&clientId=uefd68e2c-cd31-4&from=paste&id=u3d5d8177&originHeight=407&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u117d51e2-b17c-4107-a5bc-01b5d3591d0&title=)
<a name="bOxOD"></a>
## 六、glances
[Glances](https://github.com/nicolargo/glances)是基于 Python 语言编写的一款跨平台的监控工具，旨在通过 curses 或基于 Web 的界面呈现大量系统监控信息，该信息可根据用户界面的大小动态调整，是 GNU/Linux、BSD、Mac OS 和 Windows 操作系统的 top/htop 替代品。<br />它可以在客户端/服务器模式下工作，远程监控可以通过终端、Web 界面或 API（XML-RPC 和 RESTful）完成，统计数据也可以导出到文件或外部时间/值数据库。<br />除了列出所有进程及其 CPU 和内存使用情况之外，它还可以显示有关系统的其他信息，比如：

- 网络及磁盘使用情况
- 文件系统已使用的空间和总空间
- 来自不同传感器（例如电池）的数据
- 以及最近消耗过多资源的进程列表
<a name="eX99H"></a>
### 6.1 glances 的安装
```bash
# apt install glances
```
<a name="o8OdW"></a>
### 6.2 glances 的使用
独立模式只需执行：
```bash
# glances
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942718495-5c7292d0-f5ba-40ff-b468-a08ef262a5f9.webp#averageHue=%23172216&clientId=uefd68e2c-cd31-4&from=paste&id=ua11931f2&originHeight=407&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u48e0d0ec-88ea-43a6-b5e5-4f5f3fef14a&title=)<br />Web 服务器模式可执行：
```bash
# glances -w
```
使用 Web 界面监控本地机器并启动 RESTful 服务器，从http://0.0.0.0:61208/开始浏览 Web 服务器。<br />客户端 / 服务器模式可执行：
```bash
# glances -s
```
在服务器端并运行：
```bash
# glances -c <ip_server>
```
更多 glances 使用、可选参数选项以及使用案例可执行：`glances -h`查看相关帮助信息。
<a name="Du0bv"></a>
## 七、gotop
[gotop](https://github.com/xxxserxxx/gotop)是基于 Go 语言编写，是一个基于终端的图形活动监视器，可在 Linux、FreeBSD 和 macOS 上运行。
<a name="eKYRo"></a>
### 7.1 gotop 的安装
```bash
# snap install gotop
```
<a name="KUcBq"></a>
### 7.2 gotop 的使用
安装完毕后，执行命令`gotop`即可，更多热键可参考 GitHub 存储库的用法部分。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942719202-8bca05a2-0744-4fb0-89a0-4a2f145d76e3.webp#averageHue=%23144214&clientId=uefd68e2c-cd31-4&from=paste&id=u967c1aae&originHeight=407&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u45b0ab99-b122-4d46-8ee3-1f30183e167&title=)
<a name="DcP8s"></a>
## 八、gtop
[gtop](https://github.com/aksakalli/gtop)是基于 JavaScript 语言编写的一款终端系统监控仪表板。
<a name="h5RRq"></a>
### 8.1 gtop 的安装
```bash
# apt install npm
# npm install gtop -g
```
<a name="nXRie"></a>
### 8.2 gtop 的使用
安装完毕后，执行命令`gtop`即可，要停止 gtop 可使用`q`键或者`Ctrl+c`。
```bash
# gtop
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942719323-912d70e8-7b6c-4f8b-bb38-aa24c7d27cc5.webp#averageHue=%23071913&clientId=uefd68e2c-cd31-4&from=paste&id=uec473fa9&originHeight=407&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueb390b31-fca7-4e17-bf64-3bf11654c6c&title=)
<a name="p8wBr"></a>
## 九、htop
[htop](https://github.com/htop-dev/htop)可以说是 top 的绝佳替代品，它是用 C 写的，是一个跨平台的交互式的进程监控工具，具有更好的视觉效果，一目了然更容易理解当前系统的状况，允许垂直和水平滚动进程列表以查看它们的完整命令行和相关信息，如内存和 CPU 消耗。还显示了系统范围的信息，例如平均负载或交换使用情况。<br />显示的信息可通过图形设置进行配置，并且可以交互排序和过滤，与进程相关的任务（例如终止和重新处理）可以在不输入其 PID 的情况下进行完成。<br />安装也很简单，只需执行命令：`apt install htop`即可完成。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942719321-e59136cb-2479-4e9a-a6d3-3797080cdabc.webp#averageHue=%230a4c3b&clientId=uefd68e2c-cd31-4&from=paste&id=u64026706&originHeight=407&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue948c905-892d-449a-a8df-0f71d37d78f&title=)
<a name="YmUY3"></a>
## 十、nvtop
[Nvtop](https://github.com/Syllo/nvtop)：NVidia TOP，一个用于 NVIDIA GPU 的 (h)top 类似任务监视器，它可以处理多个 GPU 并以 htop 熟悉的方式打印有关它们的信息。
<a name="h4TXj"></a>
### Ubuntu disco (19.04) / Debian buster (stable)
```bash
# sudo apt install nvtop
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942719644-4815d968-5a41-4829-a300-cb90136c7db9.webp#averageHue=%23265f5c&clientId=uefd68e2c-cd31-4&from=paste&id=ud67e0a9b&originHeight=417&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u79bad1d4-140b-4679-925f-bef76be7a32&title=)<br />更多 nvtop 命令行选项
```bash
# nvtop -h
nvtop version 1.1.0
Available options:
  -d --delay        : Select the refresh rate (1 == 0.1s)
  -v --version      : Print the version and exit
  -s --gpu-select   : Colon separated list of GPU IDs to monitor
  -i --gpu-ignore   : Colon separated list of GPU IDs to ignore
  -p --no-plot      : Disable bar plot
  -r --reverse-abs  : Reverse abscissa: plot the recent data left and older on the right
  -C --no-color     : No colors
  -N --no-cache     : Always query the system for user names and command line information
  -f --freedom-unit : Use fahrenheit
  -E --encode-hide  : Set encode/decode auto hide time in seconds (default 30s, negative = always on screen)
  -h --help         : Print help and exit
```
<a name="paDy5"></a>
## 十一、vtop
[vtop](https://github.com/MrRio/vtop)是一款命令行的图形活动监视器，它是使用 drawille 绘制带有 Unicode 盲文字符的 CPU 和内存图表，通过可视化的方式进行展示，还将具有相同名称的进程分组在一起，安装也很简单，只需执行命令`npm install -g vtop`即可完成。<br />运行只需执行命令`vtop`即可。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942719734-fd106d4e-f3d3-4071-a709-230868263550.webp#averageHue=%23081212&clientId=uefd68e2c-cd31-4&from=paste&id=u99a37385&originHeight=407&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue480358e-e590-45a5-bbd9-1bf9db4e094&title=)

- dd：杀死该组中的所有进程
- 按向下箭头或`j`键向下移动
- 按向上箭头或`k`键向上移动进程列表
- 按`g`键转到进程列表的顶部
- 按`G`键移动到列表的末尾
- 按`c`键可按 CPU 进行排序
- 按`m`键可按内存进行排序
<a name="k0heR"></a>
## 十二、zenith
[zenith](https://github.com/bvaisvil/zenith)是基于 Rust 语言编写的一款具有可缩放的图表、CPU、GPU、网络和磁盘使用的终端图形。
<a name="K6Xnq"></a>
### 12.1 zenith 的特征

- 可选的 CPU、内存、网络和磁盘使用情况图表
- 支持浏览磁盘可用空间、NIC IP 地址、CPU 频率
- 支持显示 CPU、内存和磁盘的用户
- 电池百分比、充电或放电时间、已用电量
- 类似于 top 的可过滤进程表，包括每个进程的磁盘使用情况
- 更改流程优先级
- 可缩放图表视图（支持及时回滚）
- 使用信号管理流程
- 运行之间保存的性能数据
- NVIDIA GPU 的 GPU 利用率指标（带有--features nvidia），包括每个进程的 GPU 使用率
- 磁盘可用空间图表
<a name="AAVrF"></a>
### 12.2 zenith 的安装
zenith 通过 deb 软件包安装的，不过最新的 64 位 deb 软件包需要基于 Debian >= 9 或 Ubuntu >= 16.04 的发行版才可安装。
```bash
# curl -LO https://github.com/bvaisvil/zenith/releases/download/0.12.0/zenith_0.12.0-1_amd64.deb
# dpkg -i zenith_0.12.0-1_amd64.deb
```
<a name="Hphee"></a>
### 12.3 zenith 的使用
安装 zenith 完毕后，不带任何参数运行 zenith 将以 CPU、磁盘和网络的默认可视化和 2000 毫秒（2 秒）的刷新率启动。
```bash
# zenith
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643942719919-66e3ca14-b0aa-4b22-9fc2-2e82e1dd50dd.webp#averageHue=%23202d2b&clientId=uefd68e2c-cd31-4&from=paste&id=u152762fe&originHeight=407&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc837687f-a246-47b2-ae9a-86297e20508&title=)<br />更多 zenith 命令行选项
```bash
Usage: zenith [OPTIONS]

Optional arguments:
  --disable-history          Disables history when flag is present (default: false)
  -h, --help
  -V, --version
  -c, --cpu-height INT       Min Percent Height of CPU/Memory visualization. (default: 17)
  --db STRING                Database to use, if any.
  -d, --disk-height INT      Min Percent Height of Disk visualization. (default: 17)
  -n, --net-height INT       Min Percent Height of Network visualization. (default: 17)
  -p, --process-height INT   Min Percent Height of Process Table. (default: 32)
  -r, --refresh-rate INT     Refresh rate in milliseconds. (default: 2000)
  -g, --graphics-height INT  Min Percent Height of Graphics Card visualization. (default: 17)
```
<a name="pe7px"></a>
## 十三、atop
[atop](https://www.atoptool.nl/index.php)是基于 C 语言编写的一款适用于 Linux 操作系统的 ASCII 全屏性能监视器，它可以将所有的进程（即使进程在间隔期间已完成的），系统和进程活动的每日日志记录以进行长期分析，通过突出显示过载的系统资源使用颜色等。<br />同时，它还会定期显示与 CPU、内存、交换、磁盘（包括 LVM）和网络层相关的系统级活动，并且对于每个进程和线程，例如：CPU 利用率、内存增长、磁盘利用率、优先级、用户名、状态和退出代码。<br />除此之外，atop 还可结合内核模块 netatop，可显示每个进程和线程的网络活动。<br />安装 atop：
```bash
# apt install atop
```
下载 netatop 源码包并解压，需要以 root 身份进行执行以下步骤安装 netatop 内核模块和 netatopd 守护程序。
```bash
# curl -LO https://www.atoptool.nl/download/netatop-3.1.tar.gz
# tar -zxf netatop-3.1.tar.gz
# apt install zlib1g-dev
# cd netatop-3.1
# make
# make install
```
加载模块并启动守护进程：
```bash
# systemctl start netatop
```
加载模块并在启动后自动启动守护进程：
```bash
# systemctl enable netatop
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644280877443-233dae51-67ea-45d4-a3ef-c6ec7b06f158.webp#averageHue=%233a3c37&clientId=u0d7acc3f-71b4-4&from=paste&id=u2cb67e97&originHeight=406&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1d373d95-b176-42b2-a444-790ce70ec29&title=)
<a name="nOwGL"></a>
## 十四、ctop
[ctop](https://github.com/bcicen/ctop)是基于 Go 语言编写的一款为提供对多个容器的实时指标的展示，使用 ctop 可以快速查看容器 CPU、Memory、Network、IO 等指标的实时情况。
```bash
# sudo wget https://github.com/bcicen/ctop/releases/download/0.7.6/ctop-0.7.6-linux-amd64 -O /usr/local/bin/ctop
# sudo chmod +x /usr/local/bin/ctop
```
安装完毕后，若执行`ctop`命令提示弹窗`dial unix /var/run/docker.sock`报错的话，原因是没有这个 docker 文件，则需要使用官方安装脚本自动安装，可执行：`curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun`来进行安装 docker，安装完 docker 后，可以在 /var/run/ 目录下检查下 docker.sock 文件是否已经存在，则可以执行 ctop。
<a name="LXak1"></a>
### ctop 参数选项

- -a：只显示活动的容器
- -f：设置初始过滤字符串
- -h：显示帮助对话框
- -i：反转默认颜色
- -r：反向容器排序顺序
- -s：选择容器排序字段
- -v：输出版本信息并退出

![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1644280877443-6c49fcbc-d70c-4e90-acbf-4fa8f50d329e.gif#averageHue=%23333133&clientId=u0d7acc3f-71b4-4&from=paste&id=udd06496e&originHeight=374&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue12d744f-013c-4591-af8f-7acc2b7dd95&title=)
<a name="KKXhV"></a>
## 十五、iftop
[iftop](http://www.ex-parrot.com/~pdw/iftop/)是对网络使用的作用与 top 对 CPU 使用的作用相同，它可以侦听命名接口上的网络流量，并按主机对显示当前带宽使用情况表。
```bash
# apt install iftop
```
<a name="ETshu"></a>
#### iftop 参数选项

- -h：显示帮助信息
- -n：不做主机名查询
- -N：不将端口号转换为服务
- -p：在复杂模式下运行（显示同一网段的其他主机之间的流量)
- -b：不显示条形图
- -B：以字节为单位显示带宽
- -i interface：指定监听接口
- -f filter code：使用过滤码选择要计数的数据包（默认不计算，但只计算 IP 报文）
- -F net/mask：显示输入 / 输出 IPv4 网络流量
- -G net6/mask6：显示输入 / 输出 IPv6 网络流量
- -l：display 和 count link-local IPv6 流量（默认为：off）
- -P：显示端口和主机
- -m limit：设置带宽上限
- -c config file：指定一个替代的配置文件
- -t：使用不带 ncurses 的文本接口

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644280877425-7d2681c0-e51e-42de-ba1c-3e03282ea8ef.webp#averageHue=%23413f3b&clientId=u0d7acc3f-71b4-4&from=paste&id=u45d1fe66&originHeight=414&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1f948613-26b3-4895-9447-a28a5aeb670&title=)
<a name="lUZhV"></a>
## 十六、iotop
[iotop](https://github.com/Tomas-M/iotop)是可以识别在机器上使用大量输入 / 输出请求的进程，类似于 top 的实用程序，但不是显示最消耗 CPU 的内容信息，而是按进程的 IO 使用情况列出相关进程。<br />受到 Guillaume Chazarain 的 iotop Python 脚本的启发，由 Vyacheslav Trushkin 用 C 重写并由 Boian Bonev 改进，完全无需 Python 即可运行。<br />iotop 可以在 Debian、Ubuntu、Fedora、CentOS 7/8、RHEL 7/8、其他 Linux 发行版的操作系统上运行。
```bash
# apt install iotop
```
<a name="iX61I"></a>
### iotop 参数选项

- --version：显示程序的版本号并退出
- -h, --help：显示帮助信息并退出
- -o, --only：仅显示实际执行 I/O 操作的进程或线程
- -b, --batch：非交互式模式
- -n NUM, --iter=NUM：结束前的迭代次数 [无限]
- -d SEC, --delay=SEC：迭代间隔时间 [1秒]
- -p PID, --pid=PID：监视的进程 / 线程 [全部]
- -u USER, --user=USER：监控的用户 [全部]
- -P, --processes：只显示进程，而不是所有线程
- -a, --accumulated：显示累计 I/O，而不是带宽
- -k, --kilobytes：使用千字节为单位
- -t, --time：在每行上添加一个时间戳
- -q, --quiet：禁止一些标题行
- --no-help：禁止列出快捷方式
<a name="bQVAg"></a>
### iotop 热键

- any：任何其他键强制刷新
- q：退出
- i：改变 I/O 优先级
- o：仅显示活跃的进程
- r：反转排序顺序
- 方向左右键：改变排序列

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644280877449-48f12436-393d-43b8-88fb-9f8d34b78d50.webp#averageHue=%2313202c&clientId=u0d7acc3f-71b4-4&from=paste&id=ucc946fd9&originHeight=654&originWidth=763&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8f2e7e76-8065-48b1-bfde-5c666fff0bb&title=)
<a name="vva6A"></a>
## 十七、nmon
[nmon](http://nmon.sourceforge.net/pmwiki.php)是用于 AIX 和 Linux 操作系统的计算机性能系统监控工具，以交互的方式精简格式在终端上每两秒更新一次的频率显示本地系统统计信息（CPU、内存、网络、磁盘、文件系统、NFS、进程、资源等），以记录方式将统计数据保存到逗号分隔的 (CSV) 数据文件中，以供绘图和分析帮助理解计算机资源使用、调整选项以及相关瓶颈。<br />如果您在运行 nmon 时指定`-F`、`-f`、`-X`、`-x`和`-Z`参数选项中的任何一个，则 nmon 将处于记录模式，若不添加任何参数选项 nmon 将处于交互模式。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644280877616-9d243c47-fce6-4ccf-9d39-c2524c027ea7.webp#averageHue=%23131a14&clientId=u0d7acc3f-71b4-4&from=paste&id=ubdc86ef2&originHeight=521&originWidth=872&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7d3128bb-254b-4a82-b195-dfef3c3bf55&title=)
```bash
# apt install nmon
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644280877968-8d34efea-5156-4c0d-9f43-663faff87c63.webp#averageHue=%23060406&clientId=u0d7acc3f-71b4-4&from=paste&id=u4d22546b&originHeight=380&originWidth=655&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue8e8dff5-8c97-4d15-b5b0-ffafb8f8fdf&title=)<br />更多帮助信息可执行`nmon -h`查看 nmon 的帮助手册。
<a name="uvqml"></a>
## 十八、slabtop
[slabtop](https://wangchujiang.com/linux-command/c/slabtop.html)是以实时的方式显示内核“slab”缓冲区的细节信息。
<a name="RN1Qj"></a>
### 用法：
```bash
slabtop [选项]
```
<a name="nBHtG"></a>
### 选项：

- -d n--delay=n：每 n 秒更新一次显示的信息，默认是每 3 秒
- -o --once：显示一次后退出
- -s S --sort=S：指定排序标准(如下)进行排序
- -h--help：显示帮助信息
- -V--version：显示程序版本
<a name="u7FuE"></a>
### 排序标准：

- a：按活动对象的数量排序
- b：按每个 slab 的对象排序
- c：按缓存大小排序
- l：按 slabs 排序
- v：按活跃 slabs 数量排序
- n：按名称排序
- o：按对象数量排序
- p：按每个 slab 的页数排序
- s：按对象大小排序
- u：按缓存利用率排序
<a name="Wup5T"></a>
### 显示内核 slab 缓存区信息：
```bash
 Active / Total Objects (% used)    : 567933 / 601056 (94.5%)
 Active / Total Slabs (% used)      : 22815 / 22815 (100.0%)
 Active / Total Caches (% used)     : 106 / 175 (60.6%)
 Active / Total Size (% used)       : 160092.21K / 167455.54K (95.6%)
 Minimum / Average / Maximum Object : 0.01K / 0.28K / 8.00K

  OBJS ACTIVE  USE OBJ SIZE  SLABS OBJ/SLAB CACHE SIZE NAME                   
 74382  70996  95%    0.19K   3542       21     14168K dentry
 70005  60472  86%    0.10K   1795       39      7180K buffer_head
 55648  55324  99%    0.50K   3478       16     27824K kmalloc-512
 53376  53376 100%    0.12K   1668       32      6672K kernfs_node_cache
 40192  39937  99%    0.03K    314      128      1256K kmalloc-32
 36231  35951  99%    0.59K   2787       13     22296K inode_cache
 34880  34617  99%    0.20K   1744       20      6976K vm_area_struct
 21951  21711  98%    1.15K    813       27     26016K ext4_inode_cache
 18368  15434  84%    0.06K    287       64      1148K anon_vma_chain
 16170  13296  82%    0.57K   1155       14      9240K radix_tree_node
 15360  15360 100%    0.02K     60      256       240K kmalloc-16
 14848  14848 100%    0.01K     29      512       116K kmalloc-8
 10944  10944 100%    0.06K    171       64       684K kmalloc-64
 10540   8896  84%    0.02K     62      170       248K lsm_file_cache
 10396   8866  85%    0.09K    226       46       904K anon_vma
  9984   6569  65%    0.06K    156       64       624K vmap_area
  9520   9520 100%    0.05K    112       85       448K ftrace_event_field
  9184   7952  86%    0.25K    574       16      2296K filp
  8580   8455  98%    0.66K    715       12      5720K proc_inode_cache
  7956   7755  97%    0.04K     78      102       312K ext4_extent_status
  7014   6173  88%    0.19K    334       21      1336K kmalloc-192
  6720   6720 100%    0.07K    120       56       480K Acpi-Operand
  5696   5436  95%    0.12K    178       32       712K kmalloc-128
  4324   4324 100%    0.09K     94       46       376K trace_event_file
```
<a name="HEw9l"></a>
## 十九、virt-top
[virt-top](https://linux.die.net/man/1/virt-top)是一个类似于 top 的实用程序，主要用于在 Linux 操作系统中显示虚拟化域的统计信息以及 CPU、内存、虚拟磁盘、虚拟网络、虚拟机管理程序等，它需要使用 libvirt 才能够显示不同虚拟化系统的统计信息。<br />virt-top 支持在 CentOS、RHEL、Debian、Ubuntu、Fedora、OpenSUSE 等操作系统上进行安装。
<a name="OEV4W"></a>
### CentOS、RHEL 上安装 Virt-top：
```bash
# yum install virt-top
```
<a name="ivSbU"></a>
### Debian、Ubuntu 上安装 Virt-top：
```bash
# apt install virt-top
```
<a name="MdVjW"></a>
### Fedora 上安装 Virt-top：
```bash
# dnf install virt-top
```
<a name="WnKR0"></a>
### OpenSUSE 上安装 Virt-top：
```bash
# zypper install virt-top
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644280877853-354f0a6c-af0f-4ba2-9d29-722bd26399cd.webp#averageHue=%23296f0a&clientId=u0d7acc3f-71b4-4&from=paste&id=u8355c241&originHeight=248&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u11930364-5619-4bb6-b6cb-7914fb7b1f2&title=)<br />更多 virt-top 参数选项的运用可参考 virt-top man 手册。
<a name="QS10H"></a>
## References
[1] bashtop：[https://github.com/aristocratos/bashtop](https://github.com/aristocratos/bashtop)<br />[2] bpytop：[https://github.com/aristocratos/bpytop](https://github.com/aristocratos/bpytop)<br />[3] btop：[https://github.com/aristocratos/btop](https://github.com/aristocratos/btop)<br />[4] bottom：[https://github.com/ClementTsang/bottom](https://github.com/ClementTsang/bottom)<br />[5] glances：[https://github.com/nicolargo/glances](https://github.com/nicolargo/glances)<br />[6] gotop：[https://github.com/xxxserxxx/gotop](https://github.com/xxxserxxx/gotop)<br />[7] gtop：[https://github.com/aksakalli/gtop](https://github.com/aksakalli/gtop)<br />[8] htop：[https://github.com/htop-dev/htop](https://github.com/htop-dev/htop)<br />[9] nvtop：[https://github.com/Syllo/nvtop](https://github.com/Syllo/nvtop)<br />[10] vtop：[https://github.com/MrRio/vtop](https://github.com/MrRio/vtop)<br />[11] zenith：[https://github.com/bvaisvil/zenith](https://github.com/bvaisvil/zenith)<br />[12] atop：[https://www.atoptool.nl/index.php](https://www.atoptool.nl/index.php)<br />[13] ctop：[https://github.com/bcicen/ctop](https://github.com/bcicen/ctop)<br />[14] iftop：[http://www.ex-parrot.com/~pdw/iftop/](http://www.ex-parrot.com/~pdw/iftop/)<br />[15] iotop：[https://github.com/Tomas-M/iotop](https://github.com/Tomas-M/iotop)<br />[16] nmon：[http://nmon.sourceforge.net/pmwiki.php](http://nmon.sourceforge.net/pmwiki.php)<br />nmon：[https://en.wikipedia.org/wiki/Nmon](https://en.wikipedia.org/wiki/Nmon)<br />nmon：[https://www.ibm.com/docs/en/aix/7.1?topic=n-nmon-command](https://www.ibm.com/docs/en/aix/7.1?topic=n-nmon-command)<br />[17] slabtop：[https://wangchujiang.com/linux-command/c/slabtop.html](https://wangchujiang.com/linux-command/c/slabtop.html)<br />[18] virt-top：[https://linux.die.net/man/1/virt-top](https://linux.die.net/man/1/virt-top)<br />virt-top：[https://www.mankier.com/1/virt-top](https://www.mankier.com/1/virt-top)<br />virt-top：[https://ostechnix.com/display-virtualization-systems-stats-with-virt-top-in-linux/](https://ostechnix.com/display-virtualization-systems-stats-with-virt-top-in-linux/)<br />libvirt：[https://libvirt.org/](https://libvirt.org/)
