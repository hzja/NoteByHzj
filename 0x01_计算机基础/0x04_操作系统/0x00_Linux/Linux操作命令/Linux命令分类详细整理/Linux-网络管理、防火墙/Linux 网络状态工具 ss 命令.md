Linux 网络状态工具 ss命令<br />ss命令用于显示socket状态。他可以显示PACKET sockets, TCP sockets, UDP sockets, DCCP sockets, RAW sockets, Unix domain sockets等等统计。它比其他工具展示等多tcp和state信息. 它是一个非常实用、快速、有效的跟踪IP连接和sockets的新工具，SS命令可以提供如下信息：

- 所有的TCP sockets<br />
- 所有的UDP sockets<br />
- 所有ssh/ftp/ttp/https持久连接<br />
- 所有连接到Xserver的本地进程<br />
- 使用state（例如：connected, synchronized, SYN-RECV, SYN-SENT,TIME-WAIT）、地址、端口过滤<br />
- 所有的state FIN-WAIT-1 tcpsocket连接以及更多<br />

很多流行的Linux发行版都支持ss以及很多监控工具使用ss命令，熟悉这个工具有助于您更好的发现与解决系统性能问题。建议使用ss命令替代netstat部分命令，例如`netsat -ant/lnt`等.
<a name="MBA9y"></a>
### ss和netstat命令统计服务器并发连接数对比
```bash
netstat
$ time netstat -ant | grep EST | wc -l
3100

real 0m12.960s
user 0m0.334s
sys 0m12.561s

$ time ss -o state established | wc -l
3204

real 0m0.030s
user 0m0.005s
sys 0m0.026s
```
结果很明显ss统计并发连接数效率完胜netstat。
<a name="R1A1g"></a>
### 常用的ss命令
```bash
ss -l 显示本地打开的所有端口
ss -pl 显示每个进程具体打开的socket
ss -t -a 显示所有tcp socket
ss -u -a 显示所有的UDP Socekt
ss -o state established '( dport = :smtp or sport = :smtp )' 显示所有已建立的SMTP连接
ss -o state established '( dport = :http or sport = :http )' 显示所有已建立的HTTP连接
ss -x src /tmp/.X11-unix/* 找出所有连接X服务器的进程
ss -s 列出当前socket详细信息:
```
<a name="Vu7RB"></a>
### ss命令的使用示例
<a name="yeyrk"></a>
#### 显示sockets简要信息，列出当前已经连接，关闭，等待的tcp连接
```bash
$ ss -s
Total: 3519 (kernel 3691)
TCP: 26557 (estab 3163, closed 23182, orphaned 194, synrecv 0, timewait 23182/0), ports 1452

Transport Total IP IPv6
* 3691 - -
RAW 2 2 0
UDP 10 7 3
TCP 3375 3368 7
INET 3387 3377 10
FRAG 0 0 0
```
<a name="paHi3"></a>
#### 列出当前监听端口
```bash
$ ss -lRecv-Q Send-Q Local Address:Port Peer Address:Port
0 10 :::5989 :::*
0 5 *:rsync *:*
0 128 :::sunrpc :::*
0 128 *:sunrpc *:*
0 511 *:http *:*
0 128 :::ssh :::*
0 128 *:ssh *:*
0 128 :::35766 :::*
0 128 127.0.0.1:ipp *:*
0 128 ::1:ipp :::*
0 100 ::1:smtp :::*
0 100 127.0.0.1:smtp *:*
0 511 *:https *:*
0 100 :::1311 :::*
0 5 *:5666 *:*
0 128 *:3044 *:*
```
<a name="sp6zZ"></a>
#### ss列出每个进程名及其监听的端口
```bash
$ ss -pl
```
<a name="XuKXe"></a>
#### ss列所有的tcp sockets
```bash
$ ss -t -a
```
<a name="7h1Fz"></a>
#### ss列出所有udp sockets
```bash
$ ss -u -a
```
<a name="WzArM"></a>
#### ss列出所有http连接中的连接
```bash
$ ss -o state established '( dport = :http or sport = :http )'
```
以上包含对外提供的80，以及访问外部的80，用以上命令完美的替代netstat获取http并发连接数，监控中常用到。
<a name="yBW3o"></a>
#### ss列出本地哪个进程连接到x server
```bash
$ ss -x src /tmp/.X11-unix/*
```
<a name="3WNQY"></a>
#### ss列出处在FIN-WAIT-1状态的http、https连接
```bash
$ ss -o state fin-wait-1 '( sport = :http or sport = :https )'
```
<a name="71a2693c"></a>
### ss常用的state状态
```bash
established
syn-sent
syn-recv
fin-wait-1
fin-wait-2
time-wait
closed
close-wait
last-ack
listen
closing
all : All of the above states
connected : All the states except for listen and closed
synchronized : All the connected states except for syn-sent
bucket : Show states, which are maintained as minisockets, i.e. time-wait and syn-recv.
big : Opposite to bucket state.
```
<a name="MjOE4"></a>
### ss使用IP地址筛选
```bash
ss src ADDRESS_PATTERN
src：表示来源
ADDRESS_PATTERN：表示地址规则
如下：
ss src 120.33.31.1 # 列出来之20.33.31.1的连接

＃　列出来至120.33.31.1,80端口的连接
ss src 120.33.31.1:http
ss src 120.33.31.1:8
```
<a name="d028fa59"></a>
### ss使用端口筛选
```bash
ss dport OP PORT
OP:是运算符
PORT：表示端口
dport：表示过滤目标端口、相反的有sport
```
<a name="pWyuv"></a>
### OP运算符如下
```bash
<= or le : 小于等于 >= or ge : 大于等于
== or eq : 等于
!= or ne : 不等于端口
< or lt : 小于这个端口 > or gt : 大于端口
```
<a name="vra2u"></a>
#### OP实例
```bash
ss sport = :http 也可以是 ss sport = :80
ss dport = :http
ss dport \> :1024
ss sport \> :1024
ss sport \< :32000
ss sport eq :22
ss dport != :22
ss state connected sport = :http
ss \( sport = :http or sport = :https \)
ss -o state fin-wait-1 \( sport = :http or sport = :https \) dst 192.168.1/24
```
<a name="VlVJI"></a>
### ss命令帮助
```bash
$ ss -h
Usage: ss [ OPTIONS ]
       ss [ OPTIONS ] [ FILTER ]
   -h, --help          this message
   -V, --version       output version information
   -n, --numeric       don't resolve service names
   -r, --resolve       resolve host names
   -a, --all           display all sockets
   -l, --listening     display listening sockets
   -o, --options       show timer information
   -e, --extended      show detailed socket information
   -m, --memory        show socket memory usage
   -p, --processes     show process using socket
   -i, --info          show internal TCP information
   -s, --summary       show socket usage summary
   -b, --bpf           show bpf filter socket information
   -E, --events        continually display sockets as they are destroyed
   -Z, --context       display process SELinux security contexts
   -z, --contexts      display process and socket SELinux security contexts
   -N, --net           switch to the specified network namespace name

   -4, --ipv4          display only IP version 4 sockets
   -6, --ipv6          display only IP version 6 sockets
   -0, --packet        display PACKET sockets
   -t, --tcp           display only TCP sockets
   -S, --sctp          display only SCTP sockets
   -u, --udp           display only UDP sockets
   -d, --dccp          display only DCCP sockets
   -w, --raw           display only RAW sockets
   -x, --unix          display only Unix domain sockets
       --vsock         display only vsock sockets
   -f, --family=FAMILY display sockets of type FAMILY
       FAMILY := {inet|inet6|link|unix|netlink|vsock|help}

   -K, --kill          forcibly close sockets, display what was closed
   -H, --no-header     Suppress header line

   -A, --query=QUERY, --socket=QUERY
       QUERY := {all|inet|tcp|udp|raw|unix|unix_dgram|unix_stream|unix_seqpacket|packet|netlink|vsock_stream|vsock_dgram}[,QUERY]

   -D, --diag=FILE     Dump raw information about TCP sockets to FILE
   -F, --filter=FILE   read filter information from FILE
       FILTER := [ state STATE-FILTER ] [ EXPRESSION ]
       STATE-FILTER := {all|connected|synchronized|bucket|big|TCP-STATES}
         TCP-STATES := {established|syn-sent|syn-recv|fin-wait-{1,2}|time-wait|closed|close-wait|last-ack|listen|closing}
          connected := {established|syn-sent|syn-recv|fin-wait-{1,2}|time-wait|close-wait|last-ack|closing}
       synchronized := {established|syn-recv|fin-wait-{1,2}|time-wait|close-wait|last-ack|closing}
             bucket := {syn-recv|time-wait}
                big := {established|syn-sent|fin-wait-{1,2}|closed|close-wait|last-ack|listen|closing}
```
<a name="9l9DA"></a>
### ss比netstat快的原因
netstat是遍历/proc下面每个PID目录，ss直接读/proc/net下面的统计信息。所以ss执行的时候消耗资源以及消耗的时间都比netstat少很多
