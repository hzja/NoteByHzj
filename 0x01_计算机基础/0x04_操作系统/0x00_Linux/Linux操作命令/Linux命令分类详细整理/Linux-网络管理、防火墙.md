Linux 网络 防火墙
<a name="2O0TZ"></a>
### 主机名管理-hostname
主机名称命令
```bash
# 查看主机名
$ hostname
# 修改主机名称 永久修改计算机名字，需要重启或使用bash刷新
$ hostnamectl set-hostname [args]
# 刷新
$ bash
```
<a name="QuhZW"></a>
### 网络服务管理-network
```bash
# 启动网络
$ systemctl start network
# 停止
$ systemctl stop network
# 重启
$ systemctl restart network
# 查看状态
$ systemctl status network
```
> 如果linux系统不能上网（内网和外网），需要检查网络服务

<a name="Sxevx"></a>
### 网卡激活管理
<a name="GUIJv"></a>
#### 关闭网卡激活实现步骤
```bash
# 找到网卡配置文件ens32
$ cd /etc/sysconfig/network-scripts/ # 网卡配置文件目录
# 查看目录文件
$ ls
# 修改配置文件
$ vim ifcfg-ens32
# 修改配置,关闭网卡
ONBOOT=no
# 重启网络服务
$ systemctl restart network
# 测试是否可以联网
$ ping www.mobaijun.com
# 修改配置/激活网卡 修片配置文件ifcfg-ens33,开启网卡
$ ONBOOT=yes
# 重启网络服务
$ systemctl restart network
```
<a name="lzqKd"></a>
#### 网卡配置工具
```bash
# 停止某个网卡
$ ifdown
# 开启某个网卡
$ ifup
# 多功能管理工具
$ ethtool
```
<a name="QPqDJ"></a>
### 配置静态ip

- IP配置类型
   - dhcp:动态IP
   - static:静态IP
- 配置静态IP步骤
   - 域名服务器，根据域名获取对应的服务器ip地址，这样才可以访问外网
   - 用于区别不同的网络，网关可以解决不同的网络通信，需要硬件支持路由器
   - 用于标识ip地址的网络号是多少位
   - IP地址由网络号和主机号组成
   - 给linux系统设置ip必须确定ip的地址、子网掩码、网关
   - IP地址(互联网协议地址):
   - 子网掩码
   - 网关
   - DNS

修改网卡配置
```bash
# 找到网卡配置文件ens32
$ cd /etc/sysconfig/network-scripts/ # 网卡配置文件目录
# 查看目录文件
$ ls
# 修改配置文件
$ vim ifcfg-ens32
# 修改配置,切换成静态模式
BOOTPROTO=static
# 最下方添加相关内容
IPADDR=192.168.56.135
GATEWAY=192.168.123.83
NETMASK=255.255.255.0
DNS1=114.114.114.114
# 重启网络服务
$ systemctl restart network
# 测试是否可以联网
$ ping www.mobaijun.com
```
<a name="0bEkU"></a>
### 克隆虚拟电脑

- 克隆步骤
   - 关闭当前虚拟机linux系统
   - 选择当前虚拟机/管理/克隆/
   - 修改克隆出来的静态ip
   - 修改计算机的名称
<a name="tiBF4"></a>
### 域名映射

- 本地域名映射文件位置
- Windows系统本地域名映射位置:
   - `c:\windows\System32\drivers\etc\hosts`
- Linux系统本地域名映射位置:
   - `/etc/hosts`
- 修改本地域名映射实现步骤
```bash
# 编辑本地etc/hosts域名映射文件
$ vim /etc/hosts
# 添加域名映射IP的配置
192.168.123.83 www.mobaijun.com
# 保存退出
$ wq
# 测试效果
$ ping www.mobaijun.com
```
<a name="Dqb29"></a>
### 防火墙管理
> 防火墙作用
> 是防止外界访问系统内部的程序，防火墙允许哪个软件访问外网，这个软件才可以进行网络通信

| 命令 | 简介 |
| --- | --- |
| systemctl status firewalld | 查看状态 |
| systemctl start firewalld | 启动防火墙 |
| systemctl stop firewalld | 关闭防火墙 |
| systemctl enable firewalld | 开机自启 |
| systemctl disable firewalld | 让防火墙开机不自启 |

<a name="xT47F"></a>
### 网络连接管理-netstat[`netstat`属于`net-tools`工具集]
<a name="1JZLH"></a>
#### 查询网络进程使用端口号
常用命令

| netstat [args] |  | 显示网络状态 |
| --- | --- | --- |
| -n | number | 显示程序的IP地址和端口号 |
| -t | TCP | 只显示TCP协议链接的程序 |
| -l | listener | 显示监听中的Socket |
| -p | Programs | 显示程序的名称 |
| -u | UDP | 查询udp协议通信的程序 |

```bash
# 查看端口、udp、tcp、正在监听、显示程序名的所有程序
$ netstat -nutlp
```
<a name="67bbc2c5"></a>
#### 查看当前系统的连接
```bash
[root@iZuligp6e1dyzfZ /]# netstat -antp | awk '{a[$6]++}END{ for(x in a)print x,a[x]}'
LISTEN 16
ESTABLISHED 53
established) 1
Foreign 1
TIME_WAIT 48
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596432679344-506ac305-f323-487b-8618-37108bb4780f.png#height=143&id=h2mfL&originHeight=430&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=456093&status=done&style=none&title=&width=1107.6666666666667)
<a name="sr3fc"></a>
#### 监控TCP的连接数
```bash
➜  ~ netstat -n | grep tcp | grep 22 | wc -l
1
➜  ~ netstat -n | grep tcp | grep 22
tcp        0    124 172.16.252.76:22        222.93.84.161:51550     ESTABLISHED
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594712586902-033f6088-e7a7-4fe7-b7dc-4a887b516496.png#height=97&id=ojWWY&originHeight=290&originWidth=2222&originalType=binary&ratio=1&rotation=0&showTitle=false&size=231466&status=done&style=none&title=&width=740.6666666666666)
<a name="Hki6l"></a>
### 网络连接管理-ss[`ss`属于`iproute`]
<a name="SuM8G"></a>
#### 当前各种类型的连接统计
```bash
[root@iZuligp6e1dyzfZ /]# ss -s
Total: 206 (kernel 216)
TCP:   153 (estab 55, closed 80, orphaned 0, synrecv 0, timewait 51/0), ports 0

Transport Total     IP        IPv6
*         216       -         -
RAW       0         0         0
UDP       8         6         2
TCP       73        68        5
INET      81        74        7
FRAG      0         0         0
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596432695387-20cf3660-cadf-4075-b5c1-39453eb4b58d.png#height=244&id=QTv0t&originHeight=731&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=807438&status=done&style=none&title=&width=1107.6666666666667)
<a name="qFaoU"></a>
#### 查看系统正在监听的tcp连接
```bash
[root@iZuligp6e1dyzfZ /]# ss -atr
State      Recv-Q Send-Q                          Local Address:Port                              Peer Address:Port
LISTEN     0      128                                        :::mysql                                       :::*
LISTEN     0      128                                         *:6379                                         *:*
LISTEN     0      128                                        :::6379                                        :::*
LISTEN     0      128                                 localhost:26379                                        *:*
LISTEN     0      128                                         *:6380                                         *:*
LISTEN     0      128                                        :::6380                                        :::*
LISTEN     0      128                                 localhost:26380                                        *:*
LISTEN     0      128                                 localhost:26381                                        *:*
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596432709330-82b065a5-b83f-4ffc-9ccb-5daf3140fc9e.png#height=235&id=YpYq8&originHeight=706&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=801391&status=done&style=none&title=&width=1107.6666666666667)
<a name="tWtum"></a>
#### 查看系统中所有连接
```bash
[root@iZuligp6e1dyzfZ /]# ss -alt
State      Recv-Q Send-Q                          Local Address:Port                              Peer Address:Port
LISTEN     0      128                                        :::mysql                                       :::*
LISTEN     0      128                                         *:6379                                         *:*
LISTEN     0      128                                        :::6379                                        :::*
LISTEN     0      128                                 127.0.0.1:26379                                        *:*
LISTEN     0      128                                         *:6380                                         *:*
LISTEN     0      128                                        :::6380                                        :::*
LISTEN     0      128                                 127.0.0.1:26380                                        *:*
LISTEN     0      128                                 127.0.0.1:26381                                        *:*
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596432866593-3e1369ea-b483-436b-a961-d4d09ad2d32b.png#height=234&id=KrTrO&originHeight=702&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=794940&status=done&style=none&title=&width=1107.6666666666667)
<a name="VjEeE"></a>
#### 查看监听3306端口的进程 pid
```bash
[root@iZuligp6e1dyzfZ /]# ss -ltp | grep 3306
LISTEN     0      70                     :::33060                   :::*        users:(("mysqld",25073,22))
[root@iZuligp6e1dyzfZ /]# ss -ltp
State      Recv-Q Send-Q                          Local Address:Port                              Peer Address:Port
LISTEN     0      128                                        :::mysql                                       :::*        users:(("mysqld",25073,19))
LISTEN     0      128                                         *:6379                                         *:*        users:(("redis-server",31262,7))
LISTEN     0      128                                        :::6379                                        :::*        users:(("redis-server",31262,6))
LISTEN     0      128                                 127.0.0.1:26379                                        *:*        users:(("redis-server",30850,6))
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596432887018-5430ee66-7127-4ab3-b0ee-5c0e9154964c.png#height=253&id=rv35s&originHeight=758&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=872692&status=done&style=none&title=&width=1107.6666666666667)
<a name="uWLGe"></a>
#### 查看UDP、TCP、RAW、UNIX连接
```bash
# 查看TCP sockets，使用-ta选项
$ ss -ta
# 查看UDP sockets，使用-ua选项
$ ss -ua
# 查看RAW sockets，使用-wa选项
$ ss -wa
# 查看UNIX sockets，使用-xa选项
$ ss -xa
```
<a name="fpf2w"></a>
#### 查看和某个IP的所有连接
```bash
$ ss dst 10.66.224.130
$ ss dst 10.66.224.130:http
$ ss dst 10.66.224.130:smtp
$ ss dst 10.66.224.130:443
```
<a name="iKEbd"></a>
#### 显示所有的 HTTP 连接
```bash
$ ss dport = :http
```
<a name="RzrOb"></a>
#### 查看连接本机最多的前 10 个 IP 地址
```bash
[root@iZuligp6e1dyzfZ /]# netstat -antp | awk '{print $4}' | cut -d ':' -f1 | sort | uniq -c  | sort -n -k1 -r | head -n 10
     98 127.0.0.1
      6 0.0.0.0
      5
      3 172.16.252.76
      1 (servers
      1 Local
```
<a name="NwieH"></a>
#### 属性参数Recv-Q和Send-Q的说明
这两个值，在LISTEN和ESTAB状态分别代表不同意义。一般，正常的应用程序这两个值都应该为0（backlog除外）。数值越大，说明问题越严重。
<a name="5c1Rf"></a>
##### LISTEN 状态

- Recv-Q：代表建立的连接还有多少没有被accept，比如Nginx接受新连接变的很慢
- Send-Q：代表listen backlog值
<a name="sDfKZ"></a>
##### ESTAB 状态

- Recv-Q：内核中的数据还有多少(bytes)没有被应用程序读取，发生了一定程度的阻塞
- Send-Q：代表内核中发送队列里还有多少(bytes)数据没有收到ack，对端的接收处理能力不强
<a name="C7PQD"></a>
### 网络流量和带宽监控工具—`iftop`
`iftop`是Linux系统上的网络流量和带宽监控工具，可用于查看（监控）实时的网络流量、网络TCP/IP连接等等，也是非常有用的！如果未安装则需要进行自动安装。
```bash
yum install iftop
```
可以看到命令执行后效果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1610374454406-cce683c1-71b7-4798-9915-2f7c8a491a14.png#height=583&id=ngVUQ&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2006881&status=done&style=none&title=&width=1107.6666666666667)<br />`iftop`命令常用的可带参数如下：

- `-i` 指定网卡，如：`iftop -i eth0`
- `-B` 以bytes为单位显示，如：`iftop -B`
- `-n` host信息显示IP，如：`iftop -n`
- `-N` 端口信息显示端口号，如: `iftop -N`
- `-F` 指定网段，如`iftop -F 10.10.1.0/24`
- `-h` 帮助信息

进入`iftop`视图画面后还有很多操作命令可用，具体指令可以随用随help，不过要注意区分大小写。
<a name="EmtDL"></a>
### 网络流量管理
<a name="F3B1z"></a>
#### 查看流量-`sar -n DEV 1`
sar是linux上功能最全的监控软件。sar 工具比较强大，既能收集系统CPU、硬盘、动态数据，也能显示动态显示，更能查看二进制数据文件；sar 的应用比较多，而且也比较复杂，数据更为精确。使用`sar -n DEV 1`即可每秒刷新一次网络流量。<br />参数说明：<br />`-A`  显示所有历史数据，通过读取/var/log/sar目录下的所有文件，并把它们分门别类的显示出来；<br />`-b`  通过设备的I/O中断读取设置的吞吐率；<br />`-B`  报告内存或虚拟内存交换统计；<br />`-c`  报告每秒创建的进程数；<br />`-d`  报告物理块设备（存储设备）的写入、读取之类的信息，如果直观一点，可以和p参数共同使用，-dp<br />`-f`  从一个二进制的数据文件中读取内容，比如 sar -f filename<br />`-i`  interval 指定数据收集的时间，时间单位是秒；<br />`-n`  分析网络设备状态的统计，后面可以接的参数有 DEV、EDEV、NFS、NFSD、SOCK等。比如-n DEV<br />`-o`  把统计信息写入一个文件，比如 `-o filename` ；<br />`-P`  报告每个处理器应用统计，用于多处理器机器，并且启用SMP内核才有效；<br />`-p`  显示友好设备名字，以方便查看，也可以和`-d`和`-n` 参数结合使用，比如 `-dp` 或`-np`<br />`-r`  内存和交换区占用统计；<br />`-R` <br />`-t`  这个选项对从文件读取数据有用，如果没有这个参数，会以本地时间为标准 读出；<br />`-u`  报告CPU利用率的参数；<br />`-v`  报告inode,文件或其它内核表的资源占用信息；<br />`-w`  报告系统交换活动的信息； 每少交换数据的个数；<br />`-W`  报告系统交换活动吞吐信息；<br />`-x`  用于监视进程的，在其后要指定进程的PID值；<br />`-X`  用于监视进程的，但指定的应该是一个子进程ID
```bash
[root@iZuligp6e1dyzfZ /]# sar -n DEV 1
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      08/03/2020      _x86_64_        (1 CPU)

01:56:32 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
01:56:33 PM      eth0    300.00    250.00     17.58    125.49      0.00      0.00      0.00
01:56:33 PM        lo   9150.00   9150.00  34707.62  34707.62      0.00      0.00      0.00

01:56:33 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
01:56:34 PM      eth0     50.00     50.00      2.93     34.38      0.00      0.00      0.00
01:56:34 PM        lo   5650.00   5650.00   1039.31   1039.31      0.00      0.00      0.00
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596434228013-fe56e8ce-b44f-4156-9ebf-280e1f053db7.png#height=223&id=IVAqq&originHeight=670&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=775636&status=done&style=none&title=&width=1107.6666666666667)<br />参数释义：<br />IFACE：设备名；<br />rxpck/s:每秒收到的包；<br />rxbyt/s：每秒收到的所有包的数量 ；<br />txbyt/s：每秒发送的所有包的数量 ；<br />txbyt/s：每秒发送的所有包的大小；<br />rxcmp/s：每秒收到数的据压缩包的数量；<br />txcmp/s :每秒传输的数据压缩包的数据；<br />rxmcst/s: 每秒收到的多播的包数量；
<a name="VCg6I"></a>
#### 查看TCP指标汇总- `sar -n TCP,ETCP 1`
```bash
[root@iZuligp6e1dyzfZ /]# sar -n TCP,ETCP 1
Linux 3.10.0-514.26.2.el7.x86_64 (iZuligp6e1dyzfZ)      08/03/2020      _x86_64_        (1 CPU)

02:51:53 PM  active/s passive/s    iseg/s    oseg/s
02:51:54 PM     33.33      0.00   3116.67   3216.67

02:51:53 PM  atmptf/s  estres/s retrans/s isegerr/s   orsts/s
02:51:54 PM      0.00      0.00      0.00      0.00      0.00

02:51:54 PM  active/s passive/s    iseg/s    oseg/s
02:51:55 PM     50.00     50.00   8200.00   8200.00

02:51:54 PM  atmptf/s  estres/s retrans/s isegerr/s   orsts/s
02:51:55 PM      0.00    100.00     50.00      0.00     50.00
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596437588094-95fcbb77-fd10-4bde-858e-583d3c7cfb2e.png#height=297&id=XDn0g&originHeight=890&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1026981&status=done&style=none&title=&width=1107.6666666666667)

- active/s：每秒本地发起 TCP 连接数（例如，通过 `connect()`）。
- passive/s：每秒远程发起的 TCP 连接数（例如，通过 `accept()`）。
- retrans/s：每秒重传 TCP 次数。

active 和 passive 的连接数往往对于描述一个粗略衡量服务器负载是非常有用的：新接受的连接数（passive），下行连接数（active）。可以理解为 active 连接是对外的，而 passive 连接是对内的，虽然严格来说并不完全正确（例如，一个 localhost 到 localhost 的连接）。<br />重传是出现一个网络和服务器问题的一个征兆。其可能是由于一个不可靠的网络（例如，公网）造成的，或许也有可能是由于服务器过载并丢包。上面的例子显示了每秒只有一个新的 TCP 连接。
<a name="Bealt"></a>
#### 动态的监听网络流量传输- `watch cat /proc/net/dev`
```bash
[root@iZuligp6e1dyzfZ /]# watch cat /proc/net/dev

[1]+  Stopped                 watch cat /proc/net/dev
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596434295358-8a88b14a-35b1-4e1f-9de8-74bef7a168b3.png#height=218&id=a85pV&originHeight=654&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=725762&status=done&style=none&title=&width=1107.6666666666667)
<a name="wThGj"></a>
#### 查看占流量最大的IP-iftop
<a name="kVDgt"></a>
##### iftop的安装
```bash
[root@iZuligp6e1dyzfZ /]# yum install iftop
```
<a name="J1E7L"></a>
##### iftop的运行使用
```bash
[root@iZuligp6e1dyzfZ /]# iftop
interface: eth0
IP address is: 172.16.252.76
MAC address is: 00:16:3e:0a:ce:22
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596434699930-656d6db3-51a5-4506-a97f-3e72957d3182.png#height=583&id=k8XwP&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2128743&status=done&style=none&title=&width=1107.6666666666667)
<a name="YZiwB"></a>
### 网络抓包-tcpdump
<a name="Vylem"></a>
#### 官方文档
[https://hackertarget.com/tcpdump-examples/](https://hackertarget.com/tcpdump-examples/)<br />当需要判断是否有流量，或者调试一个难缠的 netty 应用问题，则可以通过抓包的方式去进行进一步的判断。在 Linux 上，可以通过 tcpdump 命令抓取数据，然后使用Wireshark 进行分析。
```bash
[root@iZuligp6e1dyzfZ /]# tcpdump -i eth0 -nn -s0 -v port 80
tcpdump: listening on eth0, link-type EN10MB (Ethernet), capture size 65535 bytes
14:08:55.970475 IP (tos 0x0, ttl 64, id 38382, offset 0, flags [DF], proto TCP (6), length 1338)
    172.16.252.76.39824 > 100.100.30.26.80: Flags [P.], cksum 0x3008 (incorrect -> 0xa0b9), seq 107914755:107916053, ack 2384084475, win 1427, length 1298
14:08:55.996061 IP (tos 0x0, ttl 52, id 9103, offset 0, flags [DF], proto TCP (6), length 40)
    100.100.30.26.80 > 172.16.252.76.39824: Flags [.], cksum 0x65ef (correct), ack 1298, win 1424, length 0
^C
2 packets captured
2 packets received by filter
0 packets dropped by kernel
```

- `-i` 指定网卡进行抓包
- `-n` 和ss一样，表示不解析域名
- `-nn` 两个n表示端口也是数字，否则解析成服务名
- `-s` 设置抓包长度，0表示不限制
- `-v` 抓包时显示详细输出，`-vv`、`-vvv`依次更加详细
<a name="b2b22756"></a>
#### 1）加入`-A`选项将打印 ascii ，`-X`打印 hex 码。
```
tcpdump -A -s0 port 80
```
<a name="3P5Lf"></a>
#### 2）抓取特定 IP 的相关包
```
tcpdump -i eth0 host 10.10.1.1tcpdump -i eth0 dst 10.10.1.20
```
<a name="0ee9a45b"></a>
#### 3）`-w`参数将抓取的包写入到某个文件中
```
tcpdump -i eth0 -s0 -w test.pcap
```
<a name="4Iy8R"></a>
#### 4）tcpdump支持表达式，还有更加复杂的例子，比如抓取系统中的get,post请求（非https)
```
tcpdump -s 0 -v -n -l | egrep -i "POST /|GET /|Host:"
```
<a name="M9vY4"></a>
### 连接状态
TCP/IP介绍，socket大概包含10个连接状态。平常工作中遇到的，除了针对SYN的拒绝服务攻击，如果有异常，大概率是TIME_WAIT和CLOSE_WAIT的问题。TIME_WAIT一般通过优化内核参数能够解决；CLOSE_WAIT一般是由于程序编写不合理造成的。
<a name="CdY6L"></a>
#### TIME_WAIT
TIME_WAIT 是主动关闭连接的一方保持的状态，像 nginx、爬虫服务器，经常发生大量处于time_wait状态的连接。TCP 一般在主动关闭连接后，会等待 2MS，然后彻底关闭连接。由于 HTTP 使用了 TCP 协议，所以在这些频繁开关连接的服务器上，就积压了非常多的 TIME_WAIT 状态连接。某些系统通过 dmesg 可以看到以下信息。
```
__ratelimit: 2170 callbacks suppressed
TCP: time wait bucket table overflow
TCP: time wait bucket table overflow
TCP: time wait bucket table overflow
TCP: time wait bucket table overflow
```
通过`ss -s`命令查看，可以看到timewait已经有2w个了。
```bash
$ ss -s
Total: 174 (kernel 199)
TCP:   20047 (estab 32, closed 20000, orphaned 4, synrecv 0, timewait 20000/0), ports 10785
```
sysctl 命令可以设置这些参数，如果想要重启生效的话，加入/etc/sysctl.conf文件中。
```
# 修改阈值
net.ipv4.tcp_max_tw_buckets = 50000
# 表示开启TCP连接中TIME-WAIT sockets的快速回收
net.ipv4.tcp_tw_reuse = 1
#启用timewait 快速回收。这个一定要开启，默认是关闭的。
net.ipv4.tcp_tw_recycle= 1   
# 修改系統默认的TIMEOUT时间,默认是60
snet.ipv4.tcp_fin_timeout = 10
```
测试参数的话，可以使用 sysctl -w net.ipv4.tcp_tw_reuse = 1 这样的命令。如果是写入进文件的，则使用sysctl -p生效。
<a name="7Cvcx"></a>
#### CLOSE_WAIT
CLOSE_WAIT一般是由于对端主动关闭，而我方没有正确处理的原因引起的。就是程序写的有问题，属于危害比较大的一种。
<a name="i8vJ7"></a>
### 网络传输文件
```bash
# 断点续传下载文件
$ wget -c $url
# 下载整站
$ wget -r -p -np -k $url
# 发送网络连接（常用)
$ curl -XGET $url
# 传输文件
$ scp
$ sftp
# 数据镜像备份
$ rsync
```
<a name="D8JAm"></a>
### 网络检测工具
```bash
# 连通性检测
$ ping google.com
# 到对端路由检测
$ tracepath google.com
# 域名检测
$ dig google.com
$ nslookup google.com
# 网络扫描工具
$ nmap
# 压力测试
$ iperf
# 全方位监控工具（好东西)
$ nmon
```
<a name="9qTaU"></a>
#### ping命令 – 测试主机间网络连通性
全称“Packet Internet Grouper”。ping命令主要用来测试主机之间网络的连通性，也可以用于执行ping指令会使用ICMP传输协议，发出要求回应的信息，若远端主机的网络功能没有问题，就会回应该信息，因而得知该主机运作正常。<br />不过值得注意的是：Linux系统下的ping命令与Windows系统下的ping命令稍有不同。Windows下运行ping命令一般会发出4个请求就结束运行该命令；而Linux下不会自动终止，此时需要按CTR+C终止或者使用-c参数为ping命令指定发送的请求数目。<br />语法格式：`ping [参数] [目标主机]`<br />常用参数：

| -d | 使用Socket的SO_DEBUG功能 |
| --- | --- |
| -c | 指定发送报文的次数 |
| -i | 指定收发信息的间隔时间 |
| -I | 使用指定的网络接口送出数据包 |

<a name="QCDiK"></a>
### 压力测试
```bash
wrk
ab
webbench
http_load
```
<a name="FNe6l"></a>
### 其他网络工具
```bash
# 远程登录
$ telnet
$ ssh
$ nc
# 防火墙
$ iptables -L
```
<a name="7giMJ"></a>
#### telnet–远端登入
telnet命令全称为“teminaLover Network”，执行telnet指令开启终端机阶段作业，并登入远端主机。telnet命令可以从这台路由器远程登陆到远端开启了telnet服务的设备，包括路由器、交换机、linux服务器等，并且配置当前路由器的telnet服务。<br />语法格式：telnet [参数]<br />常用参数：

| -8 | 允许使用8位字符资料，包括输入与输出 |
| --- | --- |
| -a | 尝试自动登入远端系统 |
| -b | 使用别名指定远端主机名称 |
| -c | 不读取用户专属目录里的.telnetrc文件 |


