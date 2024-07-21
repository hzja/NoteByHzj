Linux <br />VnStat是针对Linux和BSD的基于控制台的网络流量监视工具设计。它将保留所选网络接口的网络流量日志。为了生成日志，vnStat使用内核提供的信息。<br />换句话说，它不会嗅探网络流量，并且可以确保系统资源的精简使用。要在Linux下使用此软件，至少需要内核系列的2.2版。<br />vnStat 2.6的最新版本已于2020年1月21日发布，其中包括以下几个功能和修补程序。
<a name="EiI1W"></a>
## 特征

- 即使重新启动系统，统计信息仍然可用
- 同时监视多个网络接口
- 多种输出选项
- 按小时，天，月，周对数据进行排序，或获取前10天
- 生成输出的png图形
- 配置“月”以跟进可能有的不同计费周期
- 非常轻巧–仅消耗系统资源的一小部分
- 低CPU使用率，无论产生多少流量
- 无需root就可以使用它
- 动态选择单位（KB，MB等）
- vnStati提供了一些新选项，例如：-nl / –nolegend（隐藏rx / tx图例） –altdate –使用备用日期/时间文本位置 –headertext –自定义图像标题中的文本。
- 可以生成图像格式的输出。
- vnStat.cgi的内容定位和图像背景颜色的可自定义选项。
- 将自动检测接口带宽。
- 使用JSON进行输出

在Linux系统下安装vnStat和vnStati工具来监视实时网络流量。
<a name="Q6QOt"></a>
## 安装vnStat和vnStati网络监控工具
<a name="dwfUn"></a>
### 1.要在Linux中安装vnStat，需要从其官方GitHub发布页面下载最新版本。
_下载地址：[https://github.com/vergoh/vnstat/releases](https://github.com/vergoh/vnstat/releases)_<br />另外，也可以使用以下wget命令下载最新的源tarball，如下所示。
```bash
# wget https://humdi.net/vnstat/vnstat-2.6.tar.gz
```
在CentOS中下载VnStat
<a name="qW6LR"></a>
### 2.下载完文件后，使用终端解压归档文件，然后导航到解压归档文件的位置，并使用以下命令进行编译。
:::warning
注意：源包附带了vnStat所需的源文件，包括守护进程(Vnstatd)和映像输出(Vnstati)。
:::
<a name="Ny6B4"></a>
#### 在CentOS / RHEL和Fedora中安装VnStat
```bash
# yum group install "Development Tools"
# yum install gd gd-devel sqlite-devel 
# tar -xvf vnstat-2.6.tar.gz
# cd vnstat-2.6/
# ./configure --prefix=/usr --sysconfdir=/etc
# make
# make install
```
<a name="lOgIk"></a>
#### 在Debian和Ubuntu中安装VnStat
```bash
$ sudo apt-get install build-essential gd gd-devel libsqlite3-dev
$ tar -xvf vnstat-2.6.tar.gz
$ cd vnstat-2.6/
$ sudo ./configure --prefix=/usr --sysconfdir=/etc
$ sudo make
$ sudo make install
```
在CentOS中从源代码安装VnStat
<a name="oj3Ad"></a>
### 3.安装完成后，需要复制Vnstat服务文件，如下所示
<a name="M9xpT"></a>
#### VnStat Systemd
```bash
# cp -v examples/systemd/vnstat.service /etc/systemd/system/
# systemctl enable vnstat
# systemctl start vnstat
```
<a name="7CX67"></a>
#### Vnstat Init.d
```bash
# cp -v examples/init.d/redhat/vnstat /etc/init.d/
# chkconfig vnstat on
# service vnstat start
```
<a name="tUMKX"></a>
### 4.现在验证二进制文件是否已安装到合适的位置，并且版本是否正确。
```bash
# vnstat
vnStat 2.6 by Teemu Toivola <tst at iki dot fi>
```
<a name="MTEM5"></a>
### 5.如果要自定义vnStat配置，可以打开位于以下位置的配置文件：
```bash
# vi /etc/vnstat.conf
```
<a name="pBAX6"></a>
## Vnstat的使用
<a name="nLG7O"></a>
### 1.需要一些时间来更新数据库中的统计信息。一旦在数据库中写入了足够的信息，就可以通过简单运行以下命令来查看网络统计信息：
```bash
# vnstat
```
<a name="JqahI"></a>
### 2.还可以使用-h选项查看每小时的统计数据：
```
# vnstat -h
```
<a name="BlCfw"></a>
### 3.对于每日统计信息，需要使用以下-d选项
```bash
# vnstat -d
```
<a name="64lzw"></a>
### 4.有关更多可用选项，可以使用--help：
```
# vnstat --help
```
<a name="GI3sg"></a>
### 5.为了使统计信息更易于阅读，可以使用vnStati工具为vnStat生成图像。
VnStati随vnStat一起自动安装，因此不需要进一步的安装操作。要为网络接口的网络使用情况生成摘要图像，可以运行
```bash
# vnstati -s -i eth0 -o ~/network-log.png
```
```
# vnstati -h -i eth0 -o ~/network-log.png
```
这两个工具vnStat/vnStati提供的信息可以及时进行网络监控、分析和故障排除。可以在该工具的手册页中找到有关vnStat可用选项的更多信息。
