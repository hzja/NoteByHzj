Linux <br />lsof是系统管理/安全的管理工具。将这个工具称之为lsof真实名副其实，因为它是指“列出打开文件（lists openfiles）”。而有一点要切记，在Unix中一切（包括网络套接口）都是文件。<br />有趣的是，lsof也是有着最多开关的Linux/Unix命令之一。它有那么多的开关，它有许多选项支持使用-和+前缀。
```bash
usage:  [-?abhlnNoOPRstUvV]  [+|-c c]  [+|-d s]  [+D D]  [+|-f[cgG]]
[-F [f]]  [-g [s]]  [-i [i]]  [+|-L [l]]  [+|-M]  [-o [o]]
[-p s]  [+|-r [t]]  [-S [t]]  [-T [t]]  [-u s]  [+|-w]  [-x [fl]]  [--]  [names]
```
lsof有着实在是令人惊讶的选项数量。可以使用它来获得系统上设备的信息，能通过它了解到指定的用户在指定的地点正在碰什么东西，或者甚至是一个进程正在使用什么文件或网络连接。<br />它的一些基本能力：<br />关键选项<br />理解一些关于lsof如何工作的关键性东西是很重要的。最重要的是，当给它传递选项时，默认行为是对结果进行“或”运算。因此，如果用`-i`来拉出一个端口列表，同时又用`-p`来拉出一个进程列表，那么默认情况下会获得两者的结果。<br />下面的一些其它东西需要牢记：

- 默认：没有选项，lsof列出活跃进程的所有打开文件
- 组合：可以将选项组合到一起，如`-abc`，但要当心哪些选项需要参数
```bash
-a : 结果进行“与”运算（而不是“或”）
-l : 在输出显示用户ID而不是用户名
-h : 获得帮助
-t : 仅获取进程ID
-U : 获取UNIX套接口地址
-F : 格式化输出结果，用于其它命令。可以通过多种方式格式化，如-F pcfn（用于进程id、命令名、文件描述符、文件名，并以空终止）
```
<a name="Ve2DB"></a>
## 获取网络信息
<a name="wvR1K"></a>
### 使用`-i`显示所有连接
有些人喜欢用`netstat`来获取网络连接，结果以很直观的方式呈现，仅仅只需改变语法，就可以通过同样的命令来获取更多信息。<br />语法: `lsof -i[46] [protocol][@hostname|hostaddr][:service|port]`
```bash
# lsof -i

COMMAND  PID USER   FD   TYPE DEVICE SIZE NODE NAME
dhcpcd 6061 root 4u  IPv4  4510 UDP *:bootpc
sshd  7703 root 3u  IPv6  6499 TCP *:ssh  (LISTEN)
sshd  7892 root 3u  IPv6  6757 TCP 10.10.1.5:ssh->192.168.1.5:49901  (ESTABLISHED)
```
<a name="E2b2h"></a>
### 使用`-i 6`仅获取IPv6流量
```bash
# lsof -i 6
```
<a name="x0ezM"></a>
### 仅显示TCP连接（同理可获得UDP连接）
可以通过在`-i`后提供对应的协议来仅仅显示TCP或者UDP连接信息。
```bash
# lsof -iTCP

COMMAND  PID USER   FD   TYPE DEVICE SIZE NODE NAME
sshd  7703 root 3u  IPv6  6499 TCP *:ssh  (LISTEN)
sshd  7892 root 3u  IPv6  6757 TCP 10.10.1.5:ssh->192.168.1.5:49901  (ESTABLISHED)
```
<a name="KK7hO"></a>
### 使用`-i:port`来显示与指定端口相关的网络信息
或者，也可以通过端口搜索，这对于要找出什么阻止了另外一个应用绑定到指定端口实在是太棒了。
```bash
# lsof -i :22

COMMAND  PID USER   FD   TYPE DEVICE SIZE NODE NAME
sshd  7703 root 3u  IPv6  6499 TCP *:ssh  (LISTEN)
sshd  7892 root 3u  IPv6  6757 TCP 10.10.1.5:ssh->192.168.1.5:49901  (ESTABLISHED)
```
<a name="hls4E"></a>
### 使用`@host`来显示指定到指定主机的连接
这对于在检查是否开放连接到网络中或互联网上某个指定主机的连接时十分有用。
```bash
# lsof -i@172.16.12.5

sshd  7892 root 3u  IPv6  6757 TCP 10.10.1.5:ssh->172.16.12.5:49901  (ESTABLISHED)
```
<a name="HBao9"></a>
### 使用`@host:port`显示基于主机与端口的连接
也可以组合主机与端口的显示信息。
```bash
# lsof -i@172.16.12.5:22
sshd  7892 root 3u  IPv6  6757 TCP 10.10.1.5:ssh->172.16.12.5:49901  (ESTABLISHED)
```
<a name="SipC8"></a>
### 找出监听端口
<a name="UxcyB"></a>
#### 找出正等候连接的端口
```bash
# lsof -i -sTCP:LISTEN
```
<a name="qPSx6"></a>
#### 也可以`grep "LISTEN"`来完成该任务。
```bash
# lsof -i |  grep  -i LISTEN
iTunes 400 daniel 16u  IPv4  0x4575228  0t0 TCP *:daap (LISTEN)
```
<a name="tiGyz"></a>
### 找出已建立的连接
也可以显示任何已经连接的连接。
```bash
# lsof -i -sTCP:ESTABLISHED
```
也可以通过grep搜索"ESTABLISHED"来完成该任务。
```bash
# lsof -i |  grep  -i ESTABLISHED
firefox-b 169 daniel 49u  IPv4  0t0 TCP 1.2.3.3:1863->1.2.3.4:http (ESTABLISHED)
```
<a name="nuOsI"></a>
## 用户信息
可以获取各种用户的信息，以及它们在系统上正干着的事情，包括它们的网络活动、对文件的操作等。
<a name="tCBSu"></a>
### 使用`-u`显示指定用户打开了什么
```bash
#  lsof -u daniel

-- snipped --
Dock  155 daniel  txt REG 14,2  2798436  823208  /usr/lib/libicucore.A.dylib
Dock  155 daniel  txt REG 14,2  1580212  823126  /usr/lib/libobjc.A.dylib
Dock  155 daniel  txt REG 14,2  2934184  823498  /usr/lib/libstdc++.6.0.4.dylib
Dock  155 daniel  txt REG 14,2  132008  823505  /usr/lib/libgcc_s.1.dylib
Dock  155 daniel  txt REG 14,2  212160  823214  /usr/lib/libauto.dylib
-- snipped --
```
<a name="UXAin"></a>
### 使用`-u` user来显示除指定用户以外的其它所有用户所做的事情
```bash
# lsof -u ^daniel

-- snipped --
Dock  155 jim  txt REG 14,2  2798436  823208  /usr/lib/libicucore.A.dylib
Dock  155 jim  txt REG 14,2  1580212  823126  /usr/lib/libobjc.A.dylib
Dock  155 jim  txt REG 14,2  2934184  823498  /usr/lib/libstdc++.6.0.4.dylib
Dock  155 jim  txt REG 14,2  132008  823505  /usr/lib/libgcc_s.1.dylib
Dock  155 jim  txt REG 14,2  212160  823214  /usr/lib/libauto.dylib
-- snipped --
```
<a name="opQMH"></a>
### 杀死指定用户所做的一切事情
可以消灭指定用户运行的所有东西。
```bash
# kill -9 `lsof -t -u daniel`
```
<a name="dURca"></a>
## 命令和进程
可以查看指定程序或进程由什么启动，这通常会很有用，可以使用lsof通过名称或进程ID过滤来完成这个任务。下面列出了一些选项：
<a name="BfsxK"></a>
### 使用`-c`查看指定的命令正在使用的文件和网络连接
```bash
# lsof -c syslog-ng
COMMAND    PID USER   FD   TYPE     DEVICE    SIZE       NODE NAME
syslog-ng 7547 root  cwd    DIR 3,3  4096  2  /
syslog-ng 7547 root  rtd    DIR 3,3  4096  2  /
syslog-ng 7547 root  txt    REG 3,3  113524  1064970  /usr/sbin/syslog-ng
-- snipped --
```
<a name="gFaEL"></a>
### 使用`-p`查看指定进程ID已打开的内容
```bash
#  lsof -p 10075

-- snipped --
sshd  10068 root  mem    REG 3,3  34808  850407  /lib/libnss_files-2.4.so
sshd  10068 root  mem    REG 3,3  34924  850409  /lib/libnss_nis-2.4.so
sshd  10068 root  mem    REG 3,3  26596  850405  /lib/libnss_compat-2.4.so
sshd  10068 root  mem    REG 3,3  200152  509940  /usr/lib/libssl.so.0.9.7
sshd  10068 root  mem    REG 3,3  46216  510014  /usr/lib/liblber-2.3
sshd  10068 root  mem    REG 3,3  59868  850413  /lib/libresolv-2.4.so
sshd  10068 root  mem    REG 3,3  1197180  850396  /lib/libc-2.4.so
sshd  10068 root  mem    REG 3,3  22168  850398  /lib/libcrypt-2.4.so
sshd  10068 root  mem    REG 3,3  72784  850404  /lib/libnsl-2.4.so
sshd  10068 root  mem    REG 3,3  70632  850417  /lib/libz.so.1.2.3
sshd  10068 root  mem    REG 3,3  9992  850416  /lib/libutil-2.4.so
-- snipped --
```
<a name="WO2Qa"></a>
### `-t`选项只返回PID
```bash
# lsof -t -c Mail
350
```
<a name="ZNrUW"></a>
## 文件和目录
通过查看指定文件或目录，可以看到系统上所有正与其交互的资源——包括用户、进程等。
<a name="azVuX"></a>
### 显示与指定目录交互的所有一切
```bash
# lsof /var/log/messages/

COMMAND    PID USER   FD   TYPE DEVICE   SIZE   NODE NAME
syslog-ng 7547 root 4w REG 3,3  217309  834024  /var/log/messages
```
<a name="VbofS"></a>
### 显示与指定文件交互的所有一切
```bash
# lsof /home/daniel/firewall_whitelist.txt
```
<a name="yjqaN"></a>
## 高级用法
与tcpdump类似，当开始组合查询时，它就显示了它强大的功能。
<a name="sAhu1"></a>
### 显示daniel连接到1.1.1.1所做的一切
```bash
# lsof -u daniel -i @1.1.1.1
bkdr 1893 daniel 3u  IPv6  3456 TCP 10.10.1.10:1234->1.1.1.1:31337  (ESTABLISHED)
```
<a name="z29X6"></a>
### 同时使用`-t`和`-c`选项以给进程发送 HUP 信号
```bash
# kill -HUP `lsof -t -c sshd`
```
<a name="o3qwD"></a>
### `lsof +L1`显示所有打开的链接数小于1的文件
这通常（当不总是）表示某个攻击者正尝试通过删除文件入口来隐藏文件内容。
```bash
# lsof +L1
(hopefully nothing)
```
<a name="PwnKC"></a>
### 显示某个端口范围的打开的连接
```bash
lsof -i @fw.google.com:2150=2180
```
<a name="EDZDB"></a>
## 结尾
查看完整参考，运行`man lsof`命令或查看在线版本。<br />总结一下lsof指令的用法：<br />`lsof abc.txt` 显示开启文件abc.txt的进程<br />`lsof -i :22` 知道22端口现在运行什么程序<br />`lsof -c abc` 显示abc进程现在打开的文件<br />`lsof -g gid` 显示归属gid的进程情况<br />`lsof +d /usr/local/` 显示目录下被进程开启的文件<br />`lsof +D /usr/local/` 同上，但是会搜索目录下的目录，时间较长<br />`lsof -d 4` 显示使用fd为4的进程<br />`lsof -i` 用以显示符合条件的进程情况<br />`lsof -p 12` 看进程号为12的进程打开了哪些文件<br />`lsof +|-r [t]` 控制lsof不断重复执行，缺省是15s刷新<br />`-r`，lsof会永远不断的执行，直到收到中断信号<br />`+r`，lsof会一直执行，直到没有档案被显示
