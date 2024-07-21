Linux<br />如果要在Linux系统上监控网络，那么使用命令行工具是非常实用的，并且对于 Linux 用户来说，有着许许多多现成的工具可以使用，如：nethogs, ntopng, nload, iftop, iptraf, bmon, slurm, tcptrack, cbm, netwatch, collectl, trafshow, cacti, etherape, ipband, jnettop, netspeed 以及 speedometer。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1662438268403-f6c7064d-c54c-4838-8352-2cabebe9eca1.jpeg#clientId=ud7e07d4b-a37d-4&from=paste&id=u863d9951&originHeight=460&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u31bec0a6-9625-4dad-873a-b07d83b4df9&title=)<br />鉴于世上有着许多的 Linux 专家和开发者，显然还存在其他的网络监控工具，但在这里不打算将它们所有包括在内。<br />上面列出的工具都有着自己的独特之处，但归根结底，它们都做着监控网络流量的工作，只是通过各种不同的方法。例如 nethogs 可以被用来展示每个进程的带宽使用情况，以防想知道究竟是哪个应用在消耗了网络资源；iftop 可以被用来展示每个套接字连接的带宽使用情况，而像 nload 这类的工具可以得到有关整个带宽的信息。
<a name="pwhiC"></a>
### 1) nethogs
nethogs 是一个免费的工具，当要查找哪个 PID (注：即 process identifier，进程 ID) 给网络流量带来了麻烦时，它是非常方便的。它按每个进程来分组带宽，而不是像大多数的工具那样按照每个协议或每个子网来划分流量。它功能丰富，同时支持 IPv4 和 IPv6，若想在 Linux 主机上确定哪个程序正消耗着全部带宽，它是来做这件事的最佳的程序。<br />一个 Linux 用户可以使用 nethogs 来显示每个进程的 TCP 下载和上传速率，可以使用命令 nethogs eth0 来监控一个指定的设备，上面的 eth0 是想获取信息的设备的名称，还可以得到有关正在传输的数据的传输速率信息。<br />nethogs 是非常容易使用的，以至于总是在 Ubuntu 12.04 LTS 机器中使用它来监控网络带宽。<br />**例如要想使用混杂模式来嗅探，可以像下面展示的命令那样使用选项 **`**-p**`**：**
```bash
nethogs -p wlan0
```
假如想更多地了解 nethogs 并深入探索它，那么请毫不犹豫地阅读关于这个网络带宽监控工具的整个教程。<br />（LCTT 译注：关于 nethogs 的更多信息可以参考：[https://linux.cn/article-2808-1.html](https://linux.cn/article-2808-1.html) ）
<a name="gH38A"></a>
### 2) nload
nload 是一个控制台应用，可以被用来实时地监控网络流量和带宽使用情况，它还通过提供两个简单易懂的图表来对流量进行可视化。这个绝妙的网络监控工具还可以在监控过程中切换被监控的设备，而这可以通过按左右箭头来完成。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662438268244-b4a92b8a-7340-4252-b386-ee9229eb4e0e.png#clientId=ud7e07d4b-a37d-4&from=paste&id=u1639e28c&originHeight=409&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4906333f-802b-4d5c-b8fb-8f2cd90571d&title=)<br />正如在上面的截图中所看到的那样，由 nload 提供的图表是非常容易理解的。nload 提供了有用的信息，也展示了诸如被传输数据的总量和最小/最大网络速率等信息。<br />**而更酷的是只需要直接运行 nload 这个工具就行，这个命令是非常的短小且易记的：**
```bash
nload
```
（LCTT 译注：关于 nload 的更新信息可以参考：[https://linux.cn/article-5114-1.html](https://linux.cn/article-5114-1.html) ）
<a name="rL4a6"></a>
### 3) slurm
slurm 是另一个 Linux 网络负载监控工具，它以一个不错的 ASCII 图来显示结果，它还支持许多按键用以交互，例如 c 用来切换到经典模式， s 切换到分图模式， r 用来重绘屏幕， L 用来启用 TX/RX 灯（注：TX，发送流量；RX，接收流量） ，m 用来在经典分图模式和大图模式之间进行切换， q 退出 slurm。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662438268250-acffe57b-e7d2-45e7-9641-587a1e277fe2.png#clientId=ud7e07d4b-a37d-4&from=paste&id=u1c44fd13&originHeight=399&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ube6120e7-75c9-47b2-960e-0a20d13f5e7&title=)<br />在网络负载监控工具 slurm 中，还有许多其它的按键可用，可以很容易地使用下面的命令在 man 手册中学习它们。
```bash
man slurm
```
slurm 在 Ubuntu 和 Debian 的官方软件仓库中可以找到，所以使用这些发行版本的用户可以像下面展示的那样，使用 apt-get 安装命令来轻松地下载它：
```bash
sudo apt-get install slurm
```
<a name="ww3zU"></a>
### 4) iftop
如果想显示连接到网卡上的各个主机的带宽使用情况时，iftop 是一个非常有用的工具。根据 man 手册，iftop 在一个指定的接口或在它可以找到的第一个接口（假如没有任何特殊情况，它应该是一个对外的接口）上监听网络流量，并且展示出一个表格来显示当前的一对主机间的带宽使用情况。<br />**通过在虚拟终端中使用下面的命令，Ubuntu 和 Debian 用户可以在他们的机器中轻易地安装 iftop：**
```bash
sudo apt-get install iftop
```
可以使用下面的命令通过 yum 来安装 iftop：
```bash
yum -y install iftop
```
（LCTT 译注：关于 nload 的更多信息请参考：[https://linux.cn/article-1843-1.html](https://linux.cn/article-1843-1.html) ）
<a name="s2uE6"></a>
### 5) collectl
collectl 可以被用来收集描述当前系统状态的数据，并且它支持如下两种模式：

- 记录模式
- 回放模式

记录模式 允许从一个正在运行的系统中读取数据，然后将这些数据要么显示在终端中，要么写入一个或多个文件或一个套接字中。<br />回放模式<br />根据 man 手册，在这种模式下，数据从一个或多个由记录模式生成的数据文件中读取。<br />Ubuntu 和 Debian 用户可以在他们的机器上使用他们默认的包管理器来安装 `colletcl`。下面的命令将为他们做这个工作：
```bash
sudo apt-get install collectl
```
还可以使用下面的命令来安装 `collectl`， 因为对于这些发行版本（注：这里指的是用 yum 作为包管理器的发行版本），在它们官方的软件仓库中也含有 `collectl`：
```bash
yum install collectl
```
（LCTT 译注：关于 collectl 的更多信息请参考：[https://linux.cn/article-3154-1.html](https://linux.cn/article-3154-1.html) ）<br />来源：[http://linux.cn/article-5435-1.html](http://linux.cn/article-5435-1.html)
<a name="kl9e0"></a>
### 6) Netstat
Netstat 是一个用来监控传入和传出的网络数据包统计数据的接口统计数据命令行工具。它会显示 TCP 连接 (包括上传和下行)，路由表，及一系列的网络接口（网卡或者SDN接口）和网络协议统计数据。<br />Ubuntu 和 Debian 用户可以在他们的机器上使用默认的包管理器来安装 netstat。Netsta 软件被包括在 net-tools 软件包中，并可以在 shell 或虚拟终端中运行下面的命令来安装它：
```bash
sudo apt-get install net-tools
```
CentOS, Fedora, RHEL 用户可以在他们的机器上使用默认的包管理器来安装 netstat。Netstat 软件被包括在 net-tools 软件包中，并可以在 shell 或虚拟终端中运行下面的命令来安装它：
```bash
yum install net-tools
```
运行下面的命令使用 Netstat 来轻松地监控网络数据包统计数据：
```bash
netstat
```
更多的关于 netstat 的信息，可以简单地在 shell 或终端中键入 man netstat 来了解：
```bash
man netstat
```
（LCTT 译注：关于 netstat 的更多信息请参考：[https://linux.cn/article-2434-1.html](https://linux.cn/article-2434-1.html) ）
<a name="On40M"></a>
### 7) Netload
netload 命令只展示一个关于当前网络荷载和自从程序运行之后传输数据总的字节数目的简要报告，它没有更多的功能。它是 netdiag 软件的一部分。<br />可以在 fedora 中使用 yum 来安装 Netload，因为它在 fedora 的默认软件仓库中。但假如运行的是 CentOS 或 RHEL，则需要安装 rpmforge 软件仓库。
```bash
# yum install netdiag
```
Netload 是默认仓库中 netdiag 的一部分，可以轻易地使用下面的命令来利用 apt 包管理器安装 netdiag：
```bash
$ sudo apt-get install netdiag
```
为了运行 netload，需要确保选择了一个正在工作的网络接口的名称，如 eth0, eh1, wlan0, mon0等，然后在 shell 或虚拟终端中运行下面的命令：
```bash
$ netload wlan2
```
注意：请将上面的 wlan2 替换为想使用的网络接口名称，假如想通过扫描了解网络接口名称，可以在一个虚拟终端或 shell 中运行 ip link show 命令。
<a name="HZLNg"></a>
### 8) Nagios
Nagios 是一个领先且功能强大的开源监控系统，它使得网络或系统管理员可以在服务器的各种问题影响到服务器的主要事务之前，发现并解决这些问题。有了 Nagios 系统，管理员便可以在一个单一的窗口中监控远程的 Linux 、Windows 系统、交换机、路由器和打印机等。它会显示出重要的警告并指出在网络或服务器中是否出现某些故障，这可以间接地在问题发生前就着手执行补救行动。<br />Nagios 有一个 web 界面，其中有一个图形化的活动监视器。通过浏览网页 http://localhost/nagios/ 或 http://localhost/nagios3/ 便可以登录到这个 web 界面。假如在远程的机器上进行操作，请使用 IP 地址来替换 localhost，然后键入用户名和密码，便会看到如下图所展示的信息：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662438268253-f464ce11-468b-48f6-b61e-f780082d324c.png#clientId=ud7e07d4b-a37d-4&from=paste&id=uf9e6c257&originHeight=350&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1bf0f8e5-ec31-457e-a79d-5db8248006d&title=)<br />（LCTT 译注：关于 Nagios 的更多信息请参考：[https://linux.cn/article-2436-1.html](https://linux.cn/article-2436-1.html) ）
<a name="wmB52"></a>
### 9) EtherApe
EtherApe 是一个针对 Unix 的图形化网络监控工具，它仿照了 etherman 软件。它支持链路层、IP 和 TCP 等模式，并支持以太网, FDDI, 令牌环, ISDN, PPP, SLIP 及 WLAN 设备等接口，以及一些封装格式。主机和连接随着流量和协议而改变其尺寸和颜色。它可以过滤要展示的流量，并可从一个文件或运行的网络中读取数据包。<br />在 CentOS、Fedora、RHEL 等 Linux 发行版本中安装 etherape 是一件容易的事，因为在它们的官方软件仓库中就可以找到 etherape。可以像下面展示的命令那样使用 yum 包管理器来安装它：
```bash
yum install etherape
```
也可以使用下面的命令在 Ubuntu、Debian 及它们的衍生发行版本中使用 apt 包管理器来安装 EtherApe ：
```bash
sudo apt-get install etherape
```
在 EtherApe 安装到系统之后，需要像下面那样以 root 权限来运行 etherape：
```bash
sudo etherape
```
然后， etherape 的 图形用户界面 便会被执行。接着，在菜单上面的 捕捉 选项下，可以选择 模式(IP，链路层，TCP) 和 接口。一切设定完毕后，需要点击 开始 按钮。接着便会看到类似下面截图的东西：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662438268257-6d7c43eb-64e5-48d4-ab7e-3956e5e4c991.png#clientId=ud7e07d4b-a37d-4&from=paste&id=ucea4cf5f&originHeight=503&originWidth=636&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7c3d138a-7edd-4629-80a2-e3cb08e9f0e&title=)
<a name="hynWt"></a>
### 10) tcpflow
tcpflow 是一个命令行工具，它可以捕捉 TCP 连接(流)的部分传输数据，并以一种方便协议分析或除错的方式来存储数据。它重构了实际的数据流并将每个流存储在不同的文件中，以备日后的分析。它能识别 TCP 序列号并可以正确地重构数据流，不管是在重发还是乱序发送状态下。<br />通过 apt 包管理器在 Ubuntu 、Debian 系统中安装 tcpflow 是很容易的，因为默认情况下在官方软件仓库中可以找到它。
```bash
$ sudo apt-get install tcpflow
```
可以使用下面的命令通过 yum 包管理器在 Fedora 、CentOS 、RHEL 及它们的衍生发行版本中安装 tcpflow：
```bash
# yum install tcpflow
```
假如在软件仓库中没有找到它或不能通过 yum 包管理器来安装它，则需要像下面展示的那样从[http://pkgs.repoforge.org/tcpflow/](http://pkgs.repoforge.org/tcpflow/) 上手动安装它：<br />运行 64 位的 PC：
```bash
# yum install --nogpgcheck http://pkgs.repoforge.org/tcpflow/tcpflow-0.21-1.2.el6.rf.x86_64.rpm
```
运行 32 位的 PC：
```bash
# yum install --nogpgcheck http://pkgs.repoforge.org/tcpflow/tcpflow-0.21-1.2.el6.rf.i686.rpm
```
可以使用 tcpflow 来捕捉全部或部分 tcp 流量，并以一种简单的方式把它们写到一个可读的文件中。下面的命令就可以完成这个事情，但需要在一个空目录中运行下面的命令，因为它将创建诸如 x.x.x.x.y-a.a.a.a.z 格式的文件，运行之后，只需按 Ctrl-C 便可停止这个命令。
```bash
$ sudo tcpflow -i eth0 port 8000
```
注意：请将上面的 eth0 替换为想捕捉的网卡接口名称。<br />来源：[http://linux.cn/article-5435-1.html](http://linux.cn/article-5435-1.html)
<a name="QfOyq"></a>
### 11) IPTraf
IPTraf 是一个针对 Linux 平台的基于控制台的网络统计应用。它生成一系列的图形，如 TCP 连接的包/字节计数、接口信息和活动指示器、 TCP/UDP 流量故障以及局域网内设备的包/字节计数。<br />在默认的软件仓库中可以找到 IPTraf，所以可以使用下面的命令通过 apt 包管理器轻松地安装 IPTraf：
```bash
$ sudo apt-get install iptraf
```
可以使用下面的命令通过 yum 包管理器轻松地安装 IPTraf：
```bash
# yum install iptraf
```
需要以管理员权限来运行 IPTraf，并带有一个有效的网络接口名。这里网络接口名为 wlan2，所以使用 wlan2 来作为参数：
```bash
$ sudo iptraf wlan2
```
开始通常的网络接口统计，键入：
```bash
# iptraf -g
```
查看接口 eth0 的详细统计信息，使用：
```bash
# iptraf -d eth0
```
查看接口 eth0 的 TCP 和 UDP 监控信息，使用：
```bash
# iptraf -z eth0
```
查看接口 eth0 的包的大小和数目，使用：
```bash
# iptraf -z eth0
```
注意：请将上面的 eth0 替换为接口名称。可以通过运行`ip link show`命令来检查接口。<br />（LCTT 译注：关于 iptraf 的更多详细信息请参考：[https://linux.cn/article-5430-1.html](https://linux.cn/article-5430-1.html) ）
<a name="byT5G"></a>
### 12) Speedometer
Speedometer 是一个小巧且简单的工具，它只用来绘出一幅包含有通过某个给定端口的上行、下行流量的好看的图。<br />在默认的软件仓库中可以找到 Speedometer ，所以可以使用下面的命令通过 yum 包管理器轻松地安装 Speedometer：
```bash
# yum install speedometer
```
可以使用下面的命令通过 apt 包管理器轻松地安装 Speedometer：
```bash
$ sudo apt-get install speedometer
```
Speedometer 可以简单地通过在 shell 或虚拟终端中执行下面的命令来运行：
```bash
$ speedometer -r wlan2 -t wlan2
```
注：请将上面的 wlan2 替换为想要使用的网络接口名称。
<a name="A2t0y"></a>
### 13) Netwatch
Netwatch 是 netdiag 工具集里的一部分，它也显示当前主机和其他远程主机的连接情况，以及在每个连接中数据传输的速率。<br />可以使用 yum 在 fedora 中安装 Netwatch，因为它在 fedora 的默认软件仓库中。但若运行着 CentOS 或 RHEL ， 需要安装 rpmforge 软件仓库。
```bash
# yum install netwatch
```
Netwatch 是 netdiag 的一部分，可以在默认的软件仓库中找到，所以可以轻松地使用下面的命令来利用 apt 包管理器安装netdiag：
```bash
$ sudo apt-get install netdiag
```
为了运行 netwatch， 需要在虚拟终端或 shell 中执行下面的命令：
```bash
$ sudo netwatch -e wlan2 -nt
```
注意：请将上面的 wlan2 替换为想使用的网络接口名称，假如想通过扫描了解网络接口名称，可以在一个虚拟终端或 shell 中运行 `ip link show` 命令。
<a name="WA5Cq"></a>
### 14) Trafshow
Trafshow 同 netwatch 和 pktstat 一样，可以报告当前活动的连接里使用的协议和每个连接中数据传输的速率。它可以使用 pcap 类型的过滤器来筛选出特定的连接。<br />可以使用 yum 在 fedora 中安装 trafshow ，因为它在 fedora 的默认软件仓库中。但若正运行着 CentOS 或 RHEL ， 需要安装 rpmforge 软件仓库。
```bash
# yum install trafshow
```
Trafshow 在默认仓库中可以找到，所以可以轻松地使用下面的命令来利用 apt 包管理器安装它：
```bash
$ sudo apt-get install trafshow
```
为了使用 trafshow 来执行监控任务，需要在虚拟终端或 shell 中执行下面的命令：
```bash
$ sudo trafshow -i wlan2
```
为了专门监控 tcp 连接，如下面一样添加上 tcp 参数：
```bash
$ sudo trafshow -i wlan2 tcp
```
注意：请将上面的 wlan2 替换为想使用的网络接口名称，假如想通过扫描了解网络接口名称，可以在一个虚拟终端或 shell 中运行 `ip link show` 命令。
<a name="jTwBK"></a>
### 15) Vnstat
与大多数的其他工具相比，Vnstat 有一点不同。实际上它运行着一个后台服务或守护进程，并时刻记录着传输数据的大小。另外，它可以被用来生成一个网络使用历史记录的报告。<br />需要开启 EPEL 软件仓库，然后运行 yum 包管理器来安装 vnstat。
```bash
# yum install vnstat
```
Vnstat 在默认软件仓库中可以找到，所以可以使用下面的命令运行 apt 包管理器来安装它：
```bash
$ sudo apt-get install vnstat
```
不带有任何选项运行 vnstat 将简单地展示出从该守护进程运行后数据传输的总量。
```bash
$ vnstat
```
为了实时地监控带宽使用情况，使用 ‘`-l`’ 选项(live 模式)。然后它将以一种非常精确的方式来展示上行和下行数据所使用的带宽总量，但不会显示任何有关主机连接或进程的内部细节。
```bash
$ vnstat -l
```
完成了上面的步骤后，按 Ctrl-C 来停止，这将会得到如下类型的输出：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662438268571-d90a6f74-ea0b-40c2-93fc-bd63234c055b.png#clientId=ud7e07d4b-a37d-4&from=paste&id=u6bdf6800&originHeight=465&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u91390fb6-1623-4e90-a6da-4a6227bfb4f&title=)<br />来源：[http://linux.cn/article-5435-1.html](http://linux.cn/article-5435-1.html)
<a name="EObUr"></a>
### 16) tcptrack
tcptrack 可以展示 TCP 连接的状态，它在一个给定的网络端口上进行监听。tcptrack 监控它们的状态并展示出排序且不断更新的列表，包括来源/目标地址、带宽使用情况等信息，这与 top 命令的输出非常类似 。<br />鉴于 tcptrack 在软件仓库中，可以轻松地在 Debian、Ubuntu 系统中从软件仓库使用 apt 包管理器来安装 tcptrack。为此，需要在 shell 或虚拟终端中执行下面的命令：
```bash
$ sudo apt-get install tcptrack
```
可以通过 yum 在 fedora 中安装它，因为它在 fedora 的默认软件仓库中。但若运行着 CentOS 或 RHEL 系统，需要安装rpmforge 软件仓库。为此，需要运行下面的命令：
```bash
# wget http://apt.sw.be/redhat/el6/en/x86_64/rpmforge/RPMS/rpmforge-release-0.5.3-1.el6.rf.x86_64.rpm

# rpm -Uvh rpmforge-release*rpm

# yum install tcptrack
```
注：这里下载了 rpmforge-release 的当前最新版本，即 0.5.3-1，可以从 rpmforge 软件仓库中下载其最新版本，并请在上面的命令中替换为下载的版本。<br />tcptrack 需要以 root 权限或超级用户身份来运行。执行 tcptrack 时，需要带上要监视的网络接口 TCP 连接状况的接口名称。这里的接口名称为 wlan2，所以如下面这样使用：
```bash
sudo tcptrack -i wlan2
```
假如想监控特定的端口，则使用：
```bash
# tcptrack -i wlan2 port 80
```
请替换上面的 80 为想要监控的端口号。注意：请将上面的 wlan2 替换为想使用的网络接口名称，假如想通过扫描了解网络接口名称，可以在一个虚拟终端或 shell 中运行 `ip link show` 命令。
<a name="SldI7"></a>
### 17) CBM
CBM （ Color Bandwidth Meter） 可以展示出当前所有网络设备的流量使用情况。这个程序是如此的简单，以至于都可以从它的名称中看出其功能。CBM 的源代码和新版本可以在 [http://www.isotton.com/utils/cbm/](http://www.isotton.com/utils/cbm/) 上找到。<br />鉴于 CBM 已经包含在软件仓库中，可以简单地使用 apt 包管理器从 Debian、Ubuntu 的软件仓库中安装 CBM。为此，需要在一个 shell 窗口或虚拟终端中运行下面的命令：
```bash
$ sudo apt-get install cbm
```
只需使用下面展示的命令来在 shell 窗口或虚拟终端中运行 cbm：
```bash
$ cbm
```
<a name="AXqxw"></a>
### 18) bmon
Bmon （ Bandwidth Monitoring） ，是一个用于调试和实时监控带宽的工具。这个工具能够检索各种输入模块的统计数据。它提供了多种输出方式，包括一个基于 curses 库的界面，轻量级的HTML输出，以及 ASCII 输出格式。<br />bmon 可以在软件仓库中找到，所以可以通过使用 apt 包管理器来在 Debian、Ubuntu 中安装它。为此，需要在一个 shell 窗口或虚拟终端中运行下面的命令：
```bash
$ sudo apt-get install bmon
```
可以使用下面的命令来运行 bmon 以监视网络状态：
```bash
$ bmon
```
<a name="NfzPl"></a>
### 19) tcpdump
TCPDump 是一个用于网络监控和数据获取的工具。它可以节省很多的时间，并可用来调试网络或服务器的相关问题。它可以打印出在某个网络接口上与布尔表达式相匹配的数据包所包含的内容的一个描述。<br />tcpdump 可以在 Debian、Ubuntu 的默认软件仓库中找到，可以简单地以 sudo 权限使用 apt 包管理器来安装它。为此，需要在一个 shell 窗口或虚拟终端中运行下面的命令：
```bash
$ sudo apt -get install tcpdump
```
tcpdump 也可以在 Fedora、CentOS、RHEL 的软件仓库中找到。可以像下面一样通过 yum 包管理器来安装它：
```bash
# yum install tcpdump
```
tcpdump 需要以 root 权限或超级用户来运行，需要带上想要监控的 TCP 连接的网络接口名称来执行 tcpdump 。在这里有 wlan2 这个网络接口，所以可以像下面这样使用：
```bash
$ sudo tcpdump -i wlan2
```
假如只想监视一个特定的端口，则可以运行下面的命令。下面是一个针对 80 端口(网络服务器)的例子：
```bash
$ sudo tcpdump -i wlan2 'port 80'
```
<a name="gsvvl"></a>
### 20) ntopng
ntopng 是 ntop 的下一代版本。它是一个用于展示网络使用情况的网络探头，在一定程度上它与 top 针对进程所做的工作类似。ntopng 基于 libpcap 并且它以可移植的方式被重写，以达到可以在每一个 Unix 平台 、 MacOSX 以及 Win32 上运行的目的。<br />为了在 Debian，Ubuntu 系统上安装 ntopng，首先需要安装 编译 ntopng 所需的依赖软件包。可以通过在一个 shell 窗口或一个虚拟终端中运行下面的命令来安装它们：
```bash
$ sudo apt-get install libpcap-dev libglib2.0-dev libgeoip-dev redis-server wget libxml2-dev build-essential checkinstall
```
现在，需要像下面一样针对系统手动编译 ntopng ：
```bash
$ sudo wget http://sourceforge.net/projects/ntop/files/ntopng/ntopng-1.1_6932.tgz/download

$ sudo tar zxfv ntopng-1.1_6932.tgz

$ sudo cd ntopng-1.1_6932

$ sudo ./configure

$ sudo make

$ sudo make install
```
这样，在 Debian 或 Ubuntu 系统上应该已经安装上了编译的 ntopng 。<br />已经有了有关 ntopng 的使用方法的教程，它既可以在命令行也可以在 Web 界面中使用，可以前往这些教程来获得有关 ntopng 的知识。
<a name="xfn9l"></a>
### 结论
介绍了一些在 Linux 下的网络负载监控工具，这对于系统管理员甚至是新手来说，都是很有帮助的。在这篇文章中介绍的每一个工具都具有其特点，不同的选项等，但最终它们都可以来监控网络流量。
