Linux
<a name="BAdlm"></a>
## 简介
Netstat 命令用于显示各种网络相关信息，如网络连接，路由表，接口状态 (Interface Statistics)，masquerade 连接，多播成员 (Multicast Memberships) 等等。
<a name="koZrR"></a>
## 输出信息含义
执行netstat后，其输出结果为
```
Active Internet connections (w/o servers)
Proto Recv-Q Send-Q Local Address Foreign Address State
tcp 0 2 210.34.6.89:telnet 210.34.6.96:2873 ESTABLISHED
tcp 296 0 210.34.6.89:1165 210.34.6.84:netbios-ssn ESTABLISHED
tcp 0 0 localhost.localdom:9001 localhost.localdom:1162 ESTABLISHED
tcp 0 0 localhost.localdom:1162 localhost.localdom:9001 ESTABLISHED
tcp 0 80 210.34.6.89:1161 210.34.6.10:netbios-ssn CLOSE

Active UNIX domain sockets (w/o servers)
Proto RefCnt Flags Type State I-Node Path
unix 1 [ ] STREAM CONNECTED 16178 @000000dd
unix 1 [ ] STREAM CONNECTED 16176 @000000dc
unix 9 [ ] DGRAM 5292 /dev/log
unix 1 [ ] STREAM CONNECTED 16182 @000000df
```
从整体上看，netstat的输出结果可以分为两个部分：<br />一个是Active Internet connections，称为有源TCP连接，其中"Recv-Q"和"Send-Q"指%0A的是接收队列和发送队列。这些数字一般都应该是0。如果不是则表示软件包正在队列中堆积。这种情况只能在非常少的情况见到。<br />另一个是Active UNIX domain sockets，称为有源Unix域套接口(和网络套接字一样，但是只能用于本机通信，性能可以提高一倍)。Proto显示连接使用的协议，RefCnt表示连接到本套接口上的进程号，Types显示套接口的类型，State显示套接口当前的状态，Path表示连接到套接口的其它进程使用的路径名。
<a name="kP7Jg"></a>
## 常见参数
`-a` (all)显示所有选项，默认不显示LISTEN相关<br />`-t` (tcp)仅显示tcp相关选项<br />`-u` (udp)仅显示udp相关选项<br />`-n` 拒绝显示别名，能显示数字的全部转化成数字。<br />`-l` 仅列出有在 Listen (监听) 的服務状态<br />`-p` 显示建立相关链接的程序名<br />`-r` 显示路由信息，路由表<br />`-e` 显示扩展信息，例如uid等<br />`-s` 按各个协议进行统计<br />`-c` 每隔一个固定时间，执行该netstat命令。
> 提示：LISTEN和LISTENING的状态只有用`-a`或者`-l`才能看到

<a name="sTk0Q"></a>
## 实用命令实例
<a name="csuZz"></a>
### 1、列出所有端口 (包括监听和未监听的)
列出所有端口 `netstat -a`
```bash
# netstat -a | more
 Active Internet connections (servers and established)
 Proto Recv-Q Send-Q Local Address           Foreign Address         State
 tcp        0      0 localhost:30037         *:*                     LISTEN
 udp        0      0 *:bootpc                *:*
 
Active UNIX domain sockets (servers and established)
 Proto RefCnt Flags       Type       State         I-Node   Path
 unix  2      [ ACC ]     STREAM     LISTENING     6135     /tmp/.X11-unix/X0
 unix  2      [ ACC ]     STREAM     LISTENING     5140     /var/run/acpid.socket
```
列出所有 tcp 端口 `netstat -at`
```bash
# netstat -at
 Active Internet connections (servers and established)
 Proto Recv-Q Send-Q Local Address           Foreign Address         State
 tcp        0      0 localhost:30037         *:*                     LISTEN
 tcp        0      0 localhost:ipp           *:*                     LISTEN
 tcp        0      0 *:smtp                  *:*                     LISTEN
 tcp6       0      0 localhost:ipp           [::]:*                  LISTEN
```
列出所有 udp 端口 `netstat -au`
```bash
# netstat -au
 Active Internet connections (servers and established)
 Proto Recv-Q Send-Q Local Address           Foreign Address         State
 udp        0      0 *:bootpc                *:*
 udp        0      0 *:49119                 *:*
 udp        0      0 *:mdns                  *:*
```
<a name="f47pM"></a>
### 2、列出所有处于监听状态的 Sockets
只显示监听端口 `netstat -l`
```bash
# netstat -l
 Active Internet connections (only servers)
 Proto Recv-Q Send-Q Local Address           Foreign Address         State
 tcp        0      0 localhost:ipp           *:*                     LISTEN
 tcp6       0      0 localhost:ipp           [::]:*                  LISTEN
 udp        0      0 *:49119                 *:*
```
只列出所有监听 tcp 端口 `netstat -lt`
```bash
# netstat -lt
 Active Internet connections (only servers)
 Proto Recv-Q Send-Q Local Address           Foreign Address         State
 tcp        0      0 localhost:30037         *:*                     LISTEN
 tcp        0      0 *:smtp                  *:*                     LISTEN
 tcp6       0      0 localhost:ipp           [::]:*                  LISTEN
```
只列出所有监听 udp 端口 `netstat -lu`
```bash
# netstat -lu
 Active Internet connections (only servers)
 Proto Recv-Q Send-Q Local Address           Foreign Address         State
 udp        0      0 *:49119                 *:*
 udp        0      0 *:mdns                  *:*
```
只列出所有监听 UNIX 端口 `netstat -lx`
```bash
# netstat -lx
 Active UNIX domain sockets (only servers)
 Proto RefCnt Flags       Type       State         I-Node   Path
 unix  2      [ ACC ]     STREAM     LISTENING     6294     private/maildrop
 unix  2      [ ACC ]     STREAM     LISTENING     6203     public/cleanup
 unix  2      [ ACC ]     STREAM     LISTENING     6302     private/ifmail
 unix  2      [ ACC ]     STREAM     LISTENING     6306     private/bsmtp
```
<a name="coSaH"></a>
### 3、显示每个协议的统计信息
显示所有端口的统计信息 `netstat -s`
```bash
# netstat -s
 Ip:
 11150 total packets received
 1 with invalid addresses
 0 forwarded
 0 incoming packets discarded
 11149 incoming packets delivered
 11635 requests sent out
 Icmp:
 0 ICMP messages received
 0 input ICMP message failed.
 Tcp:
 582 active connections openings
 2 failed connection attempts
 25 connection resets received
 Udp:
 1183 packets received
 4 packets to unknown port received.
 .....
```
显示 TCP 或 UDP 端口的统计信息 `netstat -st` 或 `-su`
```bash
# netstat -st 
# netstat -su
```
<a name="QNvmU"></a>
### 4、在 netstat 输出中显示 PID 和进程名称 `netstat -p`
`netstat -p` 可以与其它开关一起使用，就可以添加 “PID/进程名称” 到 netstat 输出中，这样 debugging 的时候可以很方便的发现特定端口运行的程序。
```bash
# netstat -pt
 Active Internet connections (w/o servers)
 Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
 tcp        1      0 ramesh-laptop.loc:47212 192.168.185.75:www        CLOSE_WAIT  2109/firefox
 tcp        0      0 ramesh-laptop.loc:52750 lax:www ESTABLISHED 2109/firefox
```
<a name="JSY8g"></a>
### 5、在 netstat 输出中不显示主机，端口和用户名 (host, port or user)
当不想让主机，端口和用户名显示，使用 `netstat -n`。将会使用数字代替那些名称。<br />同样可以加速输出，因为不用进行比对查询。
```bash
# netstat -an
```
如果只是不想让这三个名称中的一个被显示，使用以下命令
```bash
# netsat -a --numeric-ports
# netsat -a --numeric-hosts
# netsat -a --numeric-users
```
<a name="ZOayh"></a>
### 6、持续输出 netstat 信息
netstat 将每隔一秒输出网络信息。
```bash
# netstat -c
 Active Internet connections (w/o servers)
 Proto Recv-Q Send-Q Local Address           Foreign Address         State
 tcp        0      0 ramesh-laptop.loc:36130 101-101-181-225.ama:www ESTABLISHED
 tcp        1      1 ramesh-laptop.loc:52564 101.11.169.230:www      CLOSING
 tcp        0      0 ramesh-laptop.loc:43758 server-101-101-43-2:www ESTABLISHED
 tcp        1      1 ramesh-laptop.loc:42367 101.101.34.101:www      CLOSING
 ^C
```
<a name="IqaZI"></a>
### 7、显示系统不支持的地址族 (Address Families)
```bash
netstat --verbose
```
在输出的末尾，会有如下的信息
```
netstat: no support for `AF IPX' on this system.
netstat: no support for `AF AX25' on this system.
netstat: no support for `AF X25' on this system.
netstat: no support for `AF NETROM' on this system.
```
<a name="ECoqI"></a>
### 8、显示核心路由信息 `netstat -r`
```bash
# netstat -r
 Kernel IP routing table
 Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
 192.168.1.0     *               255.255.255.0   U         0 0          0 eth2
 link-local      *               255.255.0.0     U         0 0          0 eth2
 default         192.168.1.1     0.0.0.0         UG        0 0          0 eth2
```
注意：使用 `netstat -rn` 显示数字格式，不查询主机名称。
<a name="i52NO"></a>
### 9、找出程序运行的端口
并不是所有的进程都能找到，没有权限的会不显示，使用 root 权限查看所有的信息。
```bash
# netstat -ap | grep ssh
 tcp        1      0 dev-db:ssh           101.174.100.22:39213        CLOSE_WAIT  -
 tcp        1      0 dev-db:ssh           101.174.100.22:57643        CLOSE_WAIT  -
```
找出运行在指定端口的进程
```bash
# netstat -an | grep ':80'
```
<a name="SEYbx"></a>
### 10、显示网络接口列表
```bash
# netstat -i
 Kernel Interface table
 Iface   MTU Met   RX-OK RX-ERR RX-DRP RX-OVR    TX-OK TX-ERR TX-DRP TX-OVR Flg
 eth0       1500 0         0      0      0 0             0      0      0      0 BMU
 eth2       1500 0     26196      0      0 0         26883      6      0      0 BMRU
 lo        16436 0         4      0      0 0             4      0      0      0 LRU
```
显示详细信息，像是 ifconfig 使用 `netstat -ie`：
```bash
# netstat -ie
 Kernel Interface table
 eth0      Link encap:Ethernet  HWaddr 00:10:40:11:11:11
 UP BROADCAST MULTICAST  MTU:1500  Metric:1
 RX packets:0 errors:0 dropped:0 overruns:0 frame:0
 TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
 collisions:0 txqueuelen:1000
 RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
 Memory:f6ae0000-f6b00000
```
<a name="CZJXY"></a>
### 11、IP和TCP分析
查看连接某服务端口最多的的IP地址
```bash
wss8848@ubuntu:~$ netstat -nat | grep "192.168.1.15:22" |awk '{print $5}'|awk -F: '{print $1}'|sort|uniq -c|sort -nr|head -20
18 221.136.168.36
3 154.74.45.242
2 78.173.31.236
2 62.183.207.98
2 192.168.1.14
2 182.48.111.215
2 124.193.219.34
2 119.145.41.2
2 114.255.41.30
1 75.102.11.99
```
TCP各种状态列表
```bash
wss8848@ubuntu:~$ netstat -nat |awk '{print $6}'
established)
Foreign
LISTEN
TIME_WAIT
ESTABLISHED
TIME_WAIT
SYN_SENT
```
先把状态全都取出来，然后使用`uniq -c`统计，之后再进行排序。
```bash
wss8848@ubuntu:~$ netstat -nat |awk '{print $6}'|sort|uniq -c
143 ESTABLISHED
1 FIN_WAIT1
1 Foreign
1 LAST_ACK
36 LISTEN
6 SYN_SENT
113 TIME_WAIT
1 established)
```
最后的命令如下：
```bash
netstat -nat |awk '{print $6}'|sort|uniq -c|sort -rn
```
分析access.log获得访问前10位的ip地址
```bash
awk '{print $1}' access.log |sort|uniq -c|sort
```
