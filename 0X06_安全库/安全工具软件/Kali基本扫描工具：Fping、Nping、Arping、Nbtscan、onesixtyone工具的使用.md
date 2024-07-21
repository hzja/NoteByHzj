Kali Fping Nping Arping Nbtscan onesixtyone
<a name="XU68i"></a>
## Hijacker下载
[https://github.com/chrisk44/Hijacker/releases](https://github.com/chrisk44/Hijacker/releases)
<a name="FuCKP"></a>
## Aircrack-ng/Airodump-ng下载
[https://www.aircrack-ng.org/](https://www.aircrack-ng.org/)
<a name="xmU60"></a>
## MDK3下载
[https://www.kali.org/tools/mdk3/](https://www.kali.org/tools/mdk3/)
<a name="CMyFJ"></a>
## Reaver下载
[https://www.kali.org/tools/reaver/](https://www.kali.org/tools/reaver/)
<a name="WDUSR"></a>
## Fping工具的使用
<a name="whGG7"></a>
### fping简介
fping是一个小型命令行工具，用于向网络主机发送ICMP回应请求，类似于`ping`，但在`ping`多个主机时性能要高得多。<br />fping完全不同于`ping`，因为可以在命令行上定义任意数量的主机，或者指定包含要`ping`的IP地址或主机列表的文件。<br />与ping要等待某一主机连接超时或发回反馈信息不同，fping给一个主机发送完数据包后，马上给下一个主机发送数据包，实现多主机同时`ping`。如果某一主机`ping`通，则此主机将被打上标记，并从等待列表中移除，如果没ping通，说明主机无法到达，主机仍然留在等待列表中，等待后续操作。
<a name="GGnrP"></a>
### fping用法
常见参数：
```bash
-4, --ipv4 只ping IPv4地址
-6, --ipv6 只ping IPv6地址
-b, --size 要发送的字节数(默认56)
-f, --file 读取指定文件中的目标地址
-a, --alive 显示可以ping通的目标
-A, --addr  将目标以IP地址的形式显示
-c, --count ping每个目标的次数，默认为1
-e, --elapsed  显示返回数据包所花费时间
-g, --generate 通过指定开始和结束地址来生成目标列表
-l, --loop 循环发送ping
-s, --stats 显示最终状态
-m, --all ping目标主机的多个网口
-q, --quiet  安静模式（不显示目标或ping的结果）
-r, --retry  当ping失败时，重试次数
-u, --unreach  显示不可达的目标
-v, --version  显示版本号
```
<a name="HbZUb"></a>
### 常见用法示例
<a name="Dbg7n"></a>
#### fping多个ip地址：
以下命令将同时显示多个IP地址，它将显示状态为活动或无法访问。
```bash
root@kali:~# fping  192.168.50.1 192.168.50.2 192.168.50.3 
192.168.50.1 is alive
192.168.50.2 is alive
ICMP Host Unreachable from 192.168.50.128 for ICMP Echo sent to 192.168.50.3
ICMP Host Unreachable from 192.168.50.128 for ICMP Echo sent to 192.168.50.3
ICMP Host Unreachable from 192.168.50.128 for ICMP Echo sent to 192.168.50.3
ICMP Host Unreachable from 192.168.50.128 for ICMP Echo sent to 192.168.50.3
192.168.50.3 is unreachable
```
<a name="qu6Du"></a>
#### fping IP地址范围：
通过-g可以指定IP地址范围，–s统计结果。使用以下输出，将响应请求发送到IP地址范围并获得想要的回复。退出后还显示累积结果。
```bash
root@kali:~# fping -s -g  192.168.50.1 192.168.50.5
192.168.50.1 is alive
192.168.50.2 is alive
192.168.50.3 is unreachable
192.168.50.4 is unreachable
192.168.50.5 is unreachable

       5 targets
       2 alive
       3 unreachable
       0 unknown addresses

       3 timeouts (waiting for response)
      14 ICMP Echos sent
       2 ICMP Echo Replies received
      12 other ICMP received

 0.26 ms (min round trip time)
 0.46 ms (avg round trip time)
 0.66 ms (max round trip time)
        4.120 sec (elapsed real time)
```
<a name="smPEU"></a>
#### 从文件中读取目标列表：
通过在文件中指定多个IP地址，读取文件去ping。
```bash
root@kali:~# cat addlist  
192.168.50.1
192.168.50.2
192.168.50.3
202.100.1.23
23.123.12.32
root@kali:~# fping < addlist 
192.168.50.1 is alive
192.168.50.2 is alive
192.168.50.3 is unreachable
202.100.1.23 is unreachable
23.123.12.32 is unreachable
```
<a name="J4vmo"></a>
#### fping扫描网络存活主机，并保存为文件：
```bash
root@kali:~# fping -g 192.168.50.0/24 -a -q > activeHost
root@kali:~# cat activeHost 
192.168.50.1
192.168.50.2
192.168.50.128
```
<a name="psNbK"></a>
## Nping工具的使用
<a name="CGJ9G"></a>
### 简介
Nping允许用户发送多种协议（TCP、UDP、ICMP和ARP协议）的数据包。可以调整协议头中的字段，例如可以设置TCP和UDP的源端口和目的端口。
<a name="cHbhO"></a>
### 主要功能

- 发送ICMP echo请求
- 对网络进行压力测试
- ARP毒化攻击
- DoS攻击
- 支持多种探测模式
- 可以探测多个主机的多个端口
<a name="CjSRN"></a>
### Nping用法
nping常用参数：<br />用法：
```bash
nping [Probe mode][Options] {target specification}
```
Probe mode（探测模式）
```bash
--tcp-connect 无特权的tcp连接探测模式；
--tcp TCP探测模式
--udp --icmp --arp
-tr,--traceroute 路由跟踪模式（仅能和tcp、udp、icmp模式一起使用）
-p, --dest-port 目标端口
-g, --source-port 源端口
--seq 设置序列号
--flags 设置tcp标识（ack,psh,rst,syn,fin）
--ack 设置ack数
-S，--source-ip 设置源IP地址
--dest-ip 目的IP地址
-c 设置次数
-e，--interface 接口
-H，--hide-sent 不显示发送的包
-N,--no-capture 不抓获回复包
-v 增加冗余等级
-q 减少冗余登记
```
<a name="RaloL"></a>
### 使用示例
nping使用端口进行网络扫描：
```bash
nping -c 1 -p 22 192.168.50.128-129 --tcp
```
向目标主机的22端口发送一次TCP数据包，用于查看主机是否开启SSH服务。
```bash
root@kali:~# nping -c 1 -p 22 192.168.50.128-129 --tcp

Starting Nping 0.7.80 ( https://nmap.org/nping ) at 2019-09-22 21:52 CST
SENT (0.0421s) TCP 192.168.50.128:27998 > 192.168.50.128:22 S ttl=64 id=60983 iplen=40  seq=4120708549 win=1480 
RCVD (0.0423s) TCP 192.168.50.128:22 > 192.168.50.128:27998 SA ttl=64 id=0 iplen=44  seq=1487346769 win=65495 <mss 65495>
SENT (1.0425s) TCP 192.168.50.128:27998 > 192.168.50.129:22 S ttl=64 id=60983 iplen=40  seq=4120708549 win=1480 

Statistics for host 192.168.50.128:
 |  Probes Sent: 1 | Rcvd: 1 | Lost: 0  (0.00%)
 |_ Max rtt: 0.101ms | Min rtt: 0.101ms | Avg rtt: 0.101ms
Statistics for host 192.168.50.129:
 |  Probes Sent: 1 | Rcvd: 0 | Lost: 1  (100.00%)
 |_ Max rtt: N/A | Min rtt: N/A | Avg rtt: N/A
Raw packets sent: 2 (80B) | Rcvd: 1 (44B) | Lost: 1 (50.00%)
Nping done: 2 IP addresses pinged in 2.08 seconds

nping echo mode:
echo mode用于测试防火墙与NAT规则比较实用。
Kali01客户端：
nping --echo-client "public" echo.nmap.org --tcp -p1-1024 --flags ack
Kali02服务端：
nping --echo-server "public" -e wlan0 -vvv 
```
<a name="LzHSD"></a>
## Arping工具的使用
<a name="hpNli"></a>
### 简介
arping实现了简单的二层发现，即基于物理层和链路层的发现。由于没有ip层的参与，所以是不可路由的。优点是速度快，准确性高；缺点是不可路由，无法发现局域网以外的主机。<br />arping，用来向局域网内的其它主机发送ARP请求的指令，它可以用来测试局域网内的某个IP是否已被使用。<br />arping命令无法一次性实现多个ip的扫描，但是可以配合shell脚本实现整个局域网的扫描。<br />arping主要干的活就是查看ip的MAC地址及IP占用的问题。
<a name="PVjnJ"></a>
### 常用参数
```bash
-c count：发送指定数量的arp包后，即停止退出
-d：这个比较重要，当局域网有IP占用的时候，可以指定这个参数，当有相同的IP的不同MAC地址reply的时候，arping会退出，退出码为1
-r：输出的时候只打印MAC，写脚本的时候用得到，不用自己对结果awk了
-R：输出的时候只打印IP，和上面一样
-q：表示不打印输出，写脚本不想打印输出的时候，应该用的到
-w deadline：指定两个ping直接的时间间隔，单位为毫秒，默认为1秒
```
<a name="hk7dL"></a>
### 常用示例
查看某个IP的MAC地址，并指定发送包的数量：
```bash
root@kali:~# arping -c 1 192.168.50.1
ARPING 192.168.50.1
60 bytes from 00:50:56:c0:00:08 (192.168.50.1): index=0 time=393.107 usec

--- 192.168.50.1 statistics ---
1 packets transmitted, 1 packets received,   0% unanswered (0 extra)
rtt min/avg/max/std-dev = 0.393/0.393/0.393/0.000 ms
```
指定特定网卡发送请求包：
```bash
root@kali:~# arping -c 1 -i eth0 192.168.50.2 
ARPING 192.168.50.2
60 bytes from 00:50:56:ec:dd:dc (192.168.50.2): index=0 time=314.876 usec

--- 192.168.50.2 statistics ---
1 packets transmitted, 1 packets received,   0% unanswered (0 extra)
rtt min/avg/max/std-dev = 0.315/0.315/0.315/0.000 ms
```
<a name="GL65P"></a>
## Nbtscan工具的使用
<a name="AG987"></a>
### 简介
扫描一个IP地址范围的NetBIOS信息，它将提供一个关于IP地址，NetBIOS信息，计算机名，服务器可用性，登录用户名和MAC地址的报告。<br />如果需要使用NetBIOS协议访问目标主机的NetBIOS服务，就要掌握目标主机的NetBIOS名称。主要针对Windows主机。<br />这个工具的运行流程：

- 遍历输入的IP范围，以广播MAC地址发送ARP查询
- 一旦接受到ARP回复，遍历记录相应的IP与MAC地址，同时向对方发送NBNS消息查询对方的主机信息。
- 打印出每条信息。
<a name="BjMYK"></a>
### NBNS简介
网络基本输入/输出系统 (NetBIOS) 名称服务器 (NBNS) 协议是 TCP/IP 上的 NetBIOS (NetBT) 协议族的一部分，它在基于 NetBIOS 名称访问的网络上提供主机名和地址映射方法。
<a name="hoJYw"></a>
## onesixtyone工具的使用
<a name="Y7RUb"></a>
### 简介
onesixtyone是一款高速的SNMP扫描程序，用来挖掘设备上的SNMP Community字串。它的扫描速度非常快。<br />如果可以获取到设备的SNMP字串，就可以发起对应的攻击或者更改设备的SNMP设定。<br />通过SNMP服务，渗透测试人员可以获取大量的设备和系统信息。在这些信息中，系统信息最为关键，如操作系统版本、内核版本等。Kali Linux提供一个简易SNMP扫描器onesixtyone。该工具可以批量获取目标的系统信息。同时，该工具还支持SNMP社区名枚举功能。安全人员可以很轻松获取多台主机的系统信息，完成基本的信息收集工作。<br />examples：
```bash
onesixtyone 192.168.4.0/24 public
onesixtyone -c dict.txt -i hosts -o my.log -w 100
```

