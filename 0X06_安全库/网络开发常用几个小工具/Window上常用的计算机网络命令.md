Window 网络<br />![2021-07-11-13-39-16-528612.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625982065386-ad203cac-d8ac-4f90-9b18-c8a6326b2174.png#clientId=u7f278a19-bab2-4&from=ui&id=ue6b731e4&originHeight=467&originWidth=458&originalType=binary&ratio=1&size=13070&status=done&style=none&taskId=ua5be34e2-2ee4-4f6b-9b5c-6ca8e14bfed)
<a name="QQ5dM"></a>
## 1、ping 命令
ping 是个使用频率极高的实用程序，主要用于确定网络的连通性。这对确定网络是否正确连接，以及网络连接的状况十分有用。简单的说，ping 就是一个测试程序，如果 ping 运行正确，大体上就可以排除网络访问层、网卡、Modem 的输入输出线路、电缆和路由器等存在的故障，从而缩小问题的范围。ping 能够以毫秒为单位显示发送请求到返回应答之间的时间量。如果应答时间短，表示数据报不必通过太多的路由器或网络，连接速度比较快。ping 还能显示 TTL（Time To Live，生存时间）值，通过 TTL 值可以推算数据包通过了多少个路由器。命令格式

- ping 主机名
- ping 域名
- ping IP 地址

使用 ping 命令检查到 IP 地址 210.43.16.17 的计算机的连通性，该例为连接正常。共发送了四个测试数据包，正确接收到四个数据包。
<a name="hBhrr"></a>
### ping 命令的常用参数选项
`**ping IP -t**`：连续对 IP 地址执行 ping 命令，直到被用户以 Ctrl+C 中断。<br />`**ping IP -l 2000**`：指定 ping 命令中的特定数据长度（此处为 2000 字节），而不是缺省的 32 字节。<br />`**ping IP -n 20**`：执行特定次数（此处是 20）的 ping 命令。
<a name="d97eJ"></a>
### ping 命令的基本应用
一般情况下，用户可以通过使用一系列 ping 命令来查找问题出在什么地方，或检验网络运行的情况。下面就给出一个典型的检测次序及对应的可能故障：
<a name="mQlBe"></a>
#### ① `ping 127.0.0.1`
如果测试成功，表明网卡、TCP/IP 协议的安装、IP 地址、子网掩码的设置正常。如果测试不成功，就表示 TCP/IP 的安装或设置存在有问题。
<a name="t4Si9"></a>
#### ② `ping 本机 IP `
地址如果测试不成功，则表示本地配置或安装存在问题，应当对网络设备和通讯介质进行测试、检查并排除。
<a name="bJteE"></a>
#### ③ `ping 局域网内其他 IP`
如果测试成功，表明本地网络中的网卡和载体运行正确。但如果收到 0 个回送应答，那么表示子网掩码不正确或网卡配置错误或电缆系统有问题。
<a name="c1000ec8"></a>
#### ④ `ping 网关 IP`
这个命令如果应答正确，表示局域网中的网关路由器正在运行并能够做出应答。
<a name="i9tII"></a>
#### ⑤ `ping 远程 IP`
如果收到正确应答，表示成功的使用了缺省网关。对于拨号上网用户则表示能够成功的访问 Internet（但不排除 ISP 的DNS会有问题）。
<a name="aWcLb"></a>
#### ⑥ `ping localhost`
local host 是系统的网络保留名，它是 127.0.0.1 的别名，每台计算机都应该能够将该名字转换成该地址。否则，则表示主机文件（/windows/host）中存在问题。
<a name="fa0aG"></a>
#### ⑦ `ping www.yahoo.com`（一个著名网站域名）
对此域名执行 Ping 命令，计算机必须先将域名转换成 IP 地址，通常是通过 DNS 服务器。如果这里出现故障，则表示本机 DNS 服务器的 IP 地址配置不正确，或它所访问的 DNS 服务器有故障如果上面所列出的所有 ping 命令都能正常运行，那么计算机进行本地和远程通信基本上就没有问题了。但是，这些命令的成功并不表示所有的网络配置都没有问题，例如，某些子网掩码错误就可能无法用这些方法检测到。
:::danger
注意：随着防火墙功能在网络中的广泛使用，当ping 其他主机或其他主机 ping 你的主机时，而显示主机不可达的时候，不要草率地下结论。最好与对某台 “设置良好” 主机的 ping 结果进行对比。
:::
<a name="5vyVZ"></a>
## 2、`ipconfig` 命令
ipconfig 实用程序可用于显示当前的 TCP/IP 配置的设置值。这些信息一般用来检验人工配置的 TCP/IP 设置是否正确。而且，如果计算机和所在的局域网使用了动态主机配置协议 DHCP，使用 ipconfig 命令可以了解到计算机是否成功地租用到了一个 IP 地址，如果已经租用到，则可以了解它目前得到的是什么地址，包括 IP 地址、子网掩码和缺省网关等网络配置信息。
<a name="DCQyp"></a>
### `ipconfig`最常用的选项
<a name="78620dd9"></a>
#### (1) `ipconfig`
当使用不带任何参数选项 ipconfig 命令时，显示每个已经配置了的接口的 IP 地址、子网掩码和缺省网关值。
<a name="b69e6872"></a>
#### (2) `ipconfig /all`
当使用 all 选项时，ipconfig 能为 DNS 和 WINS 服务器显示它已配置且所有使用的附加信息，并且能够显示内置于本地网卡中的物理地址（mac）。如果 IP 地址是从 DHCP 服务器租用的，ipconfig 将显示 DHCP 服务器分配的 IP 地址和租用地址预计失效的日期。图为运行 ipconfig /all 命令的结果窗口。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599721393161-5045baab-7bcb-41c0-afe4-38db65934780.png#height=583&id=xp68C&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2035147&status=done&style=none&width=1107.6666666666667)
<a name="TTFEV"></a>
#### (3) `ipconfig /release` 和 `ipconfig /renew`
这两个附加选项，只能在向 DHCP 服务器租用 IP 地址的计算机使用。如果输入 ipconfig /release，那么所有接口的租用 IP 地址便重新交付给 DHCP 服务器（归还 IP 地址）。如果用户输入 ipconfig /renew，那么本地计算机便设法与 DHCP 服务器取得联系，并租用一个 IP 地址。大多数情况下网卡将被重新赋予和以前所赋予的相同的 IP 地址。
<a name="orKb8"></a>
## 3、 arp 命令（地址转换协议）
ARP 是 TCP/IP 协议族中的一个重要协议，用于确定对应 IP 地址的网卡物理地址。使用 arp 命令，能够查看本地计算机或另一台计算机的 ARP 高速缓存中的当前内容。此外，使用 arp 命令可以人工方式设置静态的网卡物理地址 / IP 地址对，使用这种方式可以为缺省网关和本地服务器等常用主机进行本地静态配置，这有助于减少网络上的信息量。按照缺省设置，ARP 高速缓存中的项目是动态的，每当向指定地点发送数据并且此时高速缓存中不存在当前项目时，ARP 便会自动添加该项目。
<a name="FJgYW"></a>
### arp常用命令选项
<a name="jFQ6a"></a>
#### ① `arp –a`
用于查看高速缓存中的所有项目。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599721840283-e2364587-93d8-49c0-bcb2-351d0c6210f3.png#height=416&id=Go1SR&originHeight=1249&originWidth=3323&originalType=binary&ratio=1&size=1498576&status=done&style=none&width=1107.6666666666667)
<a name="kvGLa"></a>
#### ② `arp -a IP`
如果有多个网卡，那么使用 arp -a 加上接口的 IP 地址，就可以只显示与该接口相关的 ARP 缓存项目。
<a name="wqHWE"></a>
#### ③ `arp -s IP 物理地址`
向 ARP 高速缓存中人工输入一个静态项目。该项目在计算机引导过程中将保持有效状态，或者在出现错误时，人工配置的物理地址将自动更新该项目。
<a name="lyGVc"></a>
#### ④ `arp -d IP`
使用本命令能够人工删除一个静态项目。
<a name="iYmlJ"></a>
## 4、`traceroute` 命令
掌握使用 traceroute 命令测量路由情况的技能，即用来显示数据包到达目的主机所经过的路径。traceroute 命令的基本用法是，在命令提示符后键入 “tracert host_name” 或 “tracert ip_address”，其中，tracert 是 traceroute 在 Windows 操作系统上的称呼。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599725566242-5ad50244-abea-4792-b826-3b6a057659a2.png#height=415&id=EibSA&originHeight=1244&originWidth=3323&originalType=binary&ratio=1&size=1465784&status=done&style=none&width=1107.6666666666667)<br />输出有 5 列：<br />第一列是描述路径的第 n 跳的数值，即沿着该路径的路由器序号；<br />第二列是第一次往返时延；<br />第三列是第二次往返时延；<br />第四列是第三次往返时延；<br />第五列是路由器的名字及其输入端口的 IP 地址。<br />如果源从任何给定的路由器接收到的报文少于 3 条（由于网络中的分组丢失），traceroute 在该路由器号码后面放一个星号，并报告到达那台路由器的少于 3 次的往返时间。此外，tracert 命令还可以用来查看网络在连接站点时经过的步骤或采取哪种路线，如果是网络出现故障，就可以通过这条命令查看出现问题的位置。<br />思考【测试大型网络的路由】：<br />（1）多尝试几次 “ping www.sina.com.cn” 操作，比较得到的新浪网的 IP 地址。如果两次 ping 得到的 IP 地址不同，试考虑其中的原因（如考虑到负载均衡）。然后，针对这些不同的 IP 地址，执行 “tracert ip_address” 命令，观察分析输出的结果是否有差异。<br />（2）对于大型网络中的某站点进行 traceroute 测试，记录测试结果。观察其中是否出现第 n 跳的时延小于第 n-1 跳的时延情况。试分析其中原因（提示：可分别考虑时延的各个构成成分在总时延中所起的作用）。<br />（3）在一天的不同时段内，用 traceroute 程序多次测试从固定主机到远程固定 IP 地址的主机的路由。试分析比较测量数据，观察该路由是否有变化？如果有变化，该变化频繁吗？
<a name="5a956664"></a>
## 5、route 命令
大多数主机一般都是驻留在只连接一台路由器的网段上。由于只有一台路由器，因此不存在选择使用哪一台路由器将数据包发送到远程计算机上去的问题，该路由器的 IP 地址可作为该网段上所有计算机的缺省网关。但是，当网络上拥有两个或多个路由器时，用户就不一定想只依赖缺省网关了。实际上可能想让某些远程 IP 地址通过某个特定的路由器来传递，而其他的远程 IP 则通过另一个路由器来传递。在这种情况下，用户需要相应的路由信息，这些信息储存在路由表中，每个主机和每个路由器都配有自己独一无二的路由表。大多数路由器使用专门的路由协议来交换和动态更新路由器之间的路由表。但在有些情况下，必须人工将项目添加到路由器和主机上的路由表中。route 命令就是用来显示、人工添加和修改路由表项目的。该命令可使用如下选项：
<a name="MzpYW"></a>
### （1）`route print`
本命令用于显示路由表中的当前项目，在单个路由器网段上的输出结果如图所示。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722051304-54ba121c-fc78-4c3b-97f1-9085ecb8a670.png#height=583&id=H4Kd0&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2087294&status=done&style=none&width=1107.6666666666667)
<a name="q9mtK"></a>
### （2）`route add`
使用本命令，可以将路由项目添加给路由表。例如，如果要设定一个到目的网络 209.99.32.33 的路由，其间要经过 5 个路由器网段，首先要经过本地网络上的一个路由器 IP 为 202.96.123.5，子网掩码为 255.255.255.224，那么用户应该输入以下命令：`route add 209.99.32.33 mask 255.255.255.224 202.96.123.5 metric 5`
<a name="6HJ6j"></a>
### （3）`route change`
可以使用本命令来修改数据的传输路由，不过，用户不能使用本命令来改变数据的目的地。下面这个例子将上例路由改变采用一条包含 3 个网段的路径：route add 209.99.32.33 mask 255.255.255.224 202.96.123.250 metric 3
<a name="9bd9973c"></a>
### （4）`route delete`
 使用本命令可以从路由表中删除路由。例如：route delete 209.99.32.33
<a name="cnPpJ"></a>
## 6、`nslookup` 命令
命令 nslookup 的功能是查询任何一台机器的 IP 地址和其对应的域名。它通常需要一台域名服务器来提供域名。如果用户已经设置好域名服务器，就可以用这个命令查看不同主机的 IP 地址对应的域名。<br />（1）在本地机上使用 nslookup 命令查看本机的 IP 及域名服务器地址。直接键入命令，系统返回本机的服务器名称（带域名的全称）和 IP 地址，并进入以 “>” 为提示符的操作命令行状态；键入 “？” 可查询详细命令参数；若要退出，需键入 exit。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722146266-9649ffe4-d2f2-440c-bdcd-4e548ab19636.png#height=144&id=CjG77&originHeight=433&originWidth=3323&originalType=binary&ratio=1&size=453987&status=done&style=none&width=1107.6666666666667)<br />（2）查看 www.haut.edu.cn 的 IP。在提示符后输入要查询的 IP 地址或域名并回车即可。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722162647-b7e1a82a-2ec2-43d2-aeba-f3894064126b.png#height=222&id=gZmjH&originHeight=667&originWidth=3323&originalType=binary&ratio=1&size=734323&status=done&style=none&width=1107.6666666666667)
<a name="Zcs5T"></a>
## 7、nbtstat 命令
使用 nbtstat 命令可以查看计算机上网络配置的一些信息。使用这条命令还可以查找出别人计算机上一些私人信息。如果想查看自己计算机上的网络信息，可以运行 nbtstat -n，可以得到所在的工作组，计算机名以及网卡地址等等；想查看网络上其他的电脑情况，就，运行 nbtstat -a...，此处的...用 IP 地址代替就会返回得到那台主机上的一些信息。
<a name="2d348b45"></a>
## 8、 netstat 命令
学习使用 netstat 命令，以了解网络当前的状态。netstat 命令能够显示活动的 TCP 连接、计算机侦听的端口、以太网统计信息、IP 路由表、IPv4 统计信息（对于 IP、ICMP、TCP 和 UDP 协议）以及 IPv6 统计信息（对于 IPv6、ICMPv6、通过 IPv6 的 TCP 以及 UDP 协议）。使用时如果不带参数，netstat 显示活动的 TCP 连接。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722669073-aa5b50a9-5fb1-4a48-833b-0238466638a9.png#height=583&id=Ofapg&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2102334&status=done&style=none&width=1107.6666666666667)
<a name="66236d91"></a>
### netstat 的一些常用选项
<a name="KpZX1"></a>
#### ① `netstat –a`
`-a` 选项显示所有的有效连接信息列表，包括已建立的连接（ESTABLISHED），也包括监听连接请求（LISTENING）的那些连接。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722648083-4c54b0d0-eac1-49d6-b97d-0f8ffb7add24.png#height=583&id=FCxAT&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2090475&status=done&style=none&width=1107.6666666666667)
<a name="GBNJw"></a>
#### ② `netstat –n`
以点分十进制的形式列出 IP 地址，而不是象征性的主机名和网络名。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722701133-f1d4d5fe-c21c-4dd3-83c3-4490db8dfc0d.png#height=583&id=Y3TST&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2094538&status=done&style=none&width=1107.6666666666667)
<a name="Kw4bZ"></a>
#### ③ `netstat -e`
`-e` 选项用于显示关于以太网的统计数据。它列出的项目包括传送的数据包的总字节数、错误数、删除数、数据包的数量和广播的数量。这些统计数据既有发送的数据包数量，也有接收的数据包数量。使用这个选项可以统计一些基本的网络流量。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722731203-05eb6266-7daf-4e04-923a-bc5d488ddf1e.png#height=251&id=dbITI&originHeight=753&originWidth=3323&originalType=binary&ratio=1&size=839338&status=done&style=none&width=1107.6666666666667)
<a name="2wmH9"></a>
#### ④ `netstat -r`
`-r` 选项可以显示关于路由表的信息，类似于 route print 命令时看到的信息。除了显示有效路由外，还显示当前有效的连接。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722745408-b6a54b42-fcf9-4a08-afcd-989064bb9480.png#height=583&id=vCrCt&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2086607&status=done&style=none&width=1107.6666666666667)<br />上图显示的是一个路由表，其中：Network Destination 表示目的网络，0.0.0.0 表示不明网络，这是设置默认网关后系统自动产生的；127.0.0.0 表示本机网络地址，用于测试；224.0.0.0 表示组播地址；255.255.255.255 表示限制广播地址；Netmask 表示网络掩码，Gateway 表示网关，Interface 表示接口地址，Metric 表示路由跳数。
<a name="QUHB2"></a>
#### ⑤ `netstat -s`
`-s` 选项能够按照各个协议分别显示其统计数据。这样就可以看到当前计算机在网络上存在哪些连接，以及数据包发送和接收的详细情况等等。如果应用程序（如 Web 浏览器）运行速度比较慢，或者不能显示 Web 页之类的数据，那么可以用本选项来查看一下所显示的信息。仔细查看统计数据的各行，找到出错的关键字，进而确定问题所在。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599722766311-b4ed6e07-3ed8-4be8-91a8-63579e776c73.png#height=583&id=eQCQD&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&size=2058954&status=done&style=none&width=1107.6666666666667)
<a name="0ccd13bd"></a>
## 9、net 命令
了解 Net 服务的功能，学会使用 Net 服务命令解决有关网络问题。在命令行键入 net help command，可以在命令行获得 net 命令的语法帮助。例如，要得到关于 `net accounts` 命令的帮助信息，可键入 “net help accounts”。所有 net 命令都可以使用 `/y` 和 `/n` 命令行选项。例如，`net stop server` 命令用于提示用户确认停止所有依赖的服务器服务，`net stop server/y` 表示确认停止并关闭服务器服务。下表列出了基本的 NET 命令及它们的作用：

| **命令** | **例子** | **作用** |
| --- | --- | --- |
| `NET ACCOUNTS` | `NET ACCOUNTS` | 查阅当前账号设置 |
| `NET CONFIG` | `NET CONFIG SERVER` | 查阅本网络配置信息统计 |
| `NET GROUP` | `NET GROUP` | 查阅域组（在域控制器上） |
| `NET PRINT` | `NET PRINT\\\\printserver\\printer1` | 查阅或修改打印机映射 |
| `NET SEND` | `NET SEND server1 "test message"` | 向别的计算机发送消息或广播消息 |
| `NET SHARE` | `NET SHARE` | 查阅本地计算机上共享文件 |
| `NET START` | `NET START Messenger` | 启动服务 |
| `NET STATISTICS` | `NET STATISTICS SERVER` | 查阅网络流量统计值 |
| `NET STOP` | `NET STOP Messenger` | 停止服务 |
| `NET USE` | `NET USE x:\\\\server1\\admin` | 将网络共享文件映射到一个驱动器字母 |
| `NET USER` | `NET USER` | 查阅本地用户账号 |
| `NET VIEW` | `NET VIEW` | 查阅网络上可用计算机 |

NET 命令可以在一个地方提供所有信息，并可以把结果重定向到打印机或一个标准的文本文件中。许多服务所使用的网络命令都以 net 开头，这些 net 命令有一些公用属性。要看到所有可用的 net 命令的列表，可以在命令提示符窗口键入 `net/?` 得到。
