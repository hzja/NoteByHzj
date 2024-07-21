Linuxnetstat
<a name="bHB9B"></a>
## 简介
Linux命令Netstat是一个非常实用的网络监测工具，它可以用来监测网络连接状态、查找端口占用情况、分析网络通信状况等等。在服务器管理和网络故障排查中经常用到。了解一下这个神器。
<a name="bLECw"></a>
## 安装方式
对于不同版本的Linux系统，安装Netstat的方式也可能不同。下面来介绍一些常见Linux版本的安装方式。
<a name="lwV3C"></a>
### Debian/Ubuntu
在Debian或Ubuntu上安装Netstat非常简单，只需打开终端，然后使用下面的命令安装即可：
```bash
sudo apt-get install net-tools
```
如果系统中没有安装apt，需要先执行以下命令：
```bash
sudo apt-get update
```
<a name="gwH0s"></a>
### CentOS/RHEL/Fedora
对于CentOS、RHEL和Fedora系统，需要使用yum命令来安装Netstat。只需打开终端，然后使用下面的命令安装即可：
```bash
sudo yum install net-tools
```
如果系统中没有yum，需要执行以下命令：
```bash
sudo yum update
```
<a name="TJTkR"></a>
### Arch Linux
如果使用的是Arch Linux，那么可以使用pacman命令安装Netstat。打开终端，使用下面的命令安装：
```bash
sudo pacman -S net-tools
```
<a name="Rm0Po"></a>
### Debian 9 及以后版本
在Debian 9及以后的版本中，Netstat不再默认安装，需要使用ss代替。可以使用以下命令安装ss：
```bash
sudo apt-get install -y iproute2
```
安装完成后，使用以下命令进行网络连接监测：
```bash
ss -a
```
在不同版本的Linux系统上安装Netstat可能存在差异，但以上介绍的方法通常可以适用于大多数系统。根据你的具体系统版本，选择相应的安装方式即可。
<a name="DLcwS"></a>
## 基本用法
首先，来看一下Netstat的基本用法，打开终端并输入以下命令：
```bash
netstat -a
```
或者
```bash
netstat -an
```
这样就可以查看所有正在监听的端口，以及服务器的网络连接情况。其中，`-a`选项表示显示所有端口，而`-n`选项表示使用数字形式显示网络地址和端口号，这样看起来更加直观。<br />例如，这里展示的是一台Linux服务器上的所有网络连接情况：
```bash
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN     
tcp        0     52 172.16.28.111:22        114.114.114.114:63552   ESTABLISHED
udp        0      0 172.16.28.111:123       0.0.0.0:*                          
udp        0      0 127.0.0.1:123           0.0.0.0:*                          
udp        0      0 0.0.0.0:123             0.0.0.0:*                          
udp        0      0 0.0.0.0:5353            0.0.0.0:*
```
从上到下逐一分析一下：

- 第一列：协议类型，如tcp、udp等；
- 第二列：接收队列中的数据大小；
- 第三列：发送队列中的数据大小；
- 第四列：本地地址和端口号；
- 第五列：远程地址和端口号；
- 第六列：网络连接状态（如LISTEN表示监听状态，ESTABLISHED表示已经建立连接）。

除了以上基本用法，Netstat还有很多其他常用选项，下面来简单介绍一下。
<a name="JpiDk"></a>
### `netstat -t`
此命令将只显示TCP连接。可以将其视为-a的子集。
<a name="fURiZ"></a>
### `netstat -u`
此命令将只显示UDP连接。需要注意的是，当你运行此命令时，将会看到许多UDP连接。这是正常的，因为UDP并不要求在两端之间建立虚拟电路。
<a name="ZeDVj"></a>
### `netstat -l`
此命令将只显示正在监听的套接字。
<a name="EbmoY"></a>
### `netstat -p`
此命令显示正在监听的程序。使用此命令将显示Netstat与上面两种命令相比的一个独特的输出。当你使用此命令时，将看到所有程序的Socket接口，程序使用的端口，包括正在监听套接字的程序。<br />除了以上常用选项，那么还有些选择呢？下面列举几个比较常用的命令选项，以供参考。
<a name="MpGz1"></a>
### `netstat -s`
此命令将显示网络各种协议被使用的统计信息。
<a name="euXRd"></a>
### `netstat -r`
此命令将显示Linux内核的路由表。此命令的输出也可以解释为Linux从网络获得数据包时，如何将数据包路由到目标。
<a name="AOz91"></a>
### `netstat -c`
此命令将持续不断地运行，直到以CTRL-C停止。在每行输出中，在Netstat的实时视图下，将显示已建立的连接，以及连接正在进行的状态。<br />**更多命令详见netstat -help**
<a name="RM74m"></a>
## 常用的使用场景
以下是一些常见的Netstat应用场景及相应命令：
<a name="xTFgv"></a>
### 监测所有TCP连接
命令：`netstat -at`<br />此命令可以查看所有TCP连接信息，包括本地地址、远程地址、连接状态等。
<a name="sB2o3"></a>
### 监测处于LISTEN状态的TCP连接
命令：`netstat -at | grep LISTEN`<br />此命令可以查看所有正在监听的TCP连接信息，包括本地地址、远程地址、连接状态等。
<a name="enRyR"></a>
### 监测某个端口的使用情况
命令：`netstat -an | grep :端口号`<br />此命令可用于查找某个端口的占用情况，其中“端口号”是要查询的端口号。
<a name="H0lT2"></a>
### 监测TCP和UDP连接
命令：`netstat -au`<br />此命令可以查看所有TCP和UDP连接信息，包括本地地址、远程地址、连接状态等。
<a name="CHQC9"></a>
### 显示与每个进程相关联的端口
命令：`netstat -nap`<br />此命令可以查看所有与进程相关联的端口信息，包括占用端口的进程名称、PID等信息。
<a name="zFLTh"></a>
### 监测网络流量
命令：`netstat -s`<br />此命令可以查看网络接口的统计信息，包括传输的数据包数量、字节数、错误数量、丢失数量等。
<a name="S5nsR"></a>
## 总结
总结来说，Netstat是一个非常实用的网络监测工具，可以在服务器管理和网络故障排查等方面使用。通过学习并灵活运用Netstat，可以更好地了解Linux中的网络连接，以及排除网络故障的原因。
