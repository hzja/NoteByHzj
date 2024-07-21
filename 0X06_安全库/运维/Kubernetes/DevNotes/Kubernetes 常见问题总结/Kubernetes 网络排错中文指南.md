Kubernetes<br />Kubernetes 集群中网络排查的思路，包含网络异常模型，常用工具，并且提出一些案例以供学习。

- **Pod 常见网络异常分类**
- **网络排查工具**
- **Pod 网络异常排查思路及流程模型**
- **CNI 网络异常排查步骤**
- **案例学习**
<a name="P4K3G"></a>
## Pod 网络异常
网络异常大概分为如下几类：

- **网络不可达，主要现象为 ping 不通，其可能原因为：**
   - **源端和目的端防火墙（iptables, selinux）限制**
   - **网络路由配置不正确**
   - **源端和目的端的系统负载过高，网络连接数满，网卡队列满**
   - **网络链路故障**
- **端口不可达：主要现象为可以 ping 通，但 telnet 端口不通，其可能原因为：**
   - **源端和目的端防火墙限制**
   - **源端和目的端的系统负载过高，网络连接数满，网卡队列满，端口耗尽**
   - **目的端应用未正常监听导致（应用未启动，或监听为 127.0.0.1 等）**
- **DNS 解析异常：主要现象为基础网络可以连通，访问域名报错无法解析，访问 IP 可以正常连通。其可能原因为**
   - **Pod 的 DNS 配置不正确**
   - **DNS 服务异常**
   - **pod 与 DNS 服务通讯异常**
- **大数据包丢包：主要现象为基础网络和端口均可以连通，小数据包收发无异常，大数据包丢包。可能原因为：**
   - **可使用 ping -s 指定数据包大小进行测试**
   - **数据包的大小超过了 docker、CNI 插件、或者宿主机网卡的 _MTU_ 值。**
- **CNI 异常：主要现象为 Node 可以通，但 Pod 无法访问集群地址，可能原因有：**
   - **kube-proxy 服务异常，没有生成 iptables 策略或者 ipvs 规则导致无法访问**
   - **CIDR 耗尽，无法为 Node 注入 PodCIDR 导致 CNI 插件异常**
   - **其他 CNI 插件问题**

那么整个 Pod 网络异常分类可以如下图所示：<br />![Pod network trouble hirarchy](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267204-612d007e-0921-4ac7-8dc0-ba680493f1bb.png#clientId=uedc1184e-97af-4&from=paste&id=uc0e58ccd&originHeight=225&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u4d757ff9-aafa-4ad8-ad13-6d60eb9c532&title=Pod%20network%20trouble%20hirarchy "Pod network trouble hirarchy")<br />总结一下，Pod 最常见的网络故障有，网络不可达（ping 不通）；端口不可达（telnet 不通）；DNS 解析异常（域名不通）与大数据包丢失（大包不通）。
<a name="oHf2S"></a>
## 常用网络排查工具
在了解到常见的网络异常后，在排查时就需要使用到一些网络工具才可以很有效的定位到网络故障原因，下面会介绍一些网络排查工具。
<a name="xJVmC"></a>
### **tcpdump**
tcpdump 网络嗅探器，将强大和简单结合到一个单一的命令行界面中，能够将网络中的报文抓取，输出到屏幕或者记录到文件中。<br />**各系统下的安装**

- **Ubuntu/Debian: tcpdump  ；**`**apt-get install -y tcpdump**`
- **Centos/Fedora: tcpdump ；**`**yum install -y tcpdump**`
- **Apline：tcpdump ；**`**apk add tcpdump --no-cache**`

查看指定接口上的所有通讯<br />语法

| **参数** | **说明** |
| --- | --- |
| -i [interface] | 

 |
| -w [flle] | 第一个 n 表示将地址解析为数字格式而不是主机名，第二个 N 表示将端口解析为数字格式而不是服务名 |
| -n | 不显示 IP 地址 |
| -X | hex and ASCII |
| -A | ASCII（实际上是以人类可读懂的包进行显示） |
| -XX | 

 |
| -v | 详细信息 |
| -r | 读取文件而不是实时抓包 |
| 

 | **关键字** |
| **type** | host（主机名，域名，IP 地址）, net, port, portrange |
| **direction** | src, dst, src or dst , src and ds |
| **protocol** | ether, ip，arp, tcp, udp, wlan |

<a name="VCeZ8"></a>
#### 捕获所有网络接口
```bash
tcpdump -D
```
<a name="uET7R"></a>
#### 按 IP 查找流量
最常见的查询之一 host，可以看到来往于 1.1.1.1 的流量。
```bash
tcpdump host 1.1.1.1
```
<a name="Hswq7"></a>
#### 按源 / 目的 地址过滤
如果只想查看来自 / 向某方向流量，可以使用 src 和 dst。
```bash
tcpdump src|dst 1.1.1.1
```
<a name="GHZPF"></a>
#### 通过网络查找数据包
使用 net 选项，来要查找出 / 入某个网络或子网的数据包。
```bash
tcpdump net 1.2.3.0/24
```
<a name="pyn3d"></a>
#### 使用十六进制输出数据包内容
hex 可以以 16 进制输出包的内容
```bash
tcpdump -c 1 -X icmp
```
<a name="L1sHm"></a>
#### 查看特定端口的流量
使用 port 选项来查找特定的端口流量。
```bash
tcpdump port 3389
tcpdump src port 1025
```
<a name="T0pwZ"></a>
#### 查找端口范围的流量
```bash
tcpdump portrange 21-23
```
<a name="UFzqA"></a>
#### 过滤包的大小
如果需要查找特定大小的数据包，可以使用以下选项。可以使用 `less`，`greater`。
```bash
tcpdump less 32
tcpdump greater 64
tcpdump <= 128
```
<a name="AmDb1"></a>
#### 捕获流量输出为文件
`-w`  可以将数据包捕获保存到一个文件中以便将来进行分析。这些文件称为 PCAP（PEE-cap）文件，它们可以由不同的工具处理，包括 Wireshark 。
```bash
tcpdump port 80 -w capture_file
```
<a name="dUbE2"></a>
#### 组合条件
tcpdump 也可以结合逻辑运算符进行组合条件查询

- **AND **`**_and_**`** or **`**&&**`
- **OR **`**_or_**`** or **`**||**`
- **EXCEPT**`**_not_**`** or **`**!**`
```bash
tcpdump -i eth0 -nn host 220.181.57.216 and 10.0.0.1  # 主机之间的通讯
tcpdump -i eth0 -nn host 220.181.57.216 or 10.0.0.1
# 获取10.0.0.1与 10.0.0.9或 10.0.0.1 与10.0.0.3之间的通讯
tcpdump -i eth0 -nn host 10.0.0.1 and \(10.0.0.9 or 10.0.0.3\)
```
<a name="oRfzD"></a>
#### 原始输出
并显示人类可读的内容进行输出包（不包含内容）。
```bash
tcpdump -ttnnvvS -i eth0
tcpdump -ttnnvvS -i eth0
```
<a name="T8Vib"></a>
#### IP 到端口
查找从某个 IP 到端口任何主机的某个端口所有流量。
```bash
tcpdump -nnvvS src 10.5.2.3 and dst port 3389
```
<a name="ze4LD"></a>
#### 去除特定流量
可以将指定的流量排除，如这显示所有到 192.168.0.2 的 非 ICMP 的流量。
```bash
tcpdump dst 192.168.0.2 and src net and not icmp
```
来自非指定端口的流量，如，显示来自不是 SSH 流量的主机的所有流量。
```bash
tcpdump -vv src mars and not dst port 22
```
<a name="QAXKn"></a>
#### 选项分组
在构建复杂查询时，必须使用单引号 `'`。单引号用于忽略特殊符号 `()` ，以便于使用其他表达式（如 host, port, net 等）进行分组。
```bash
tcpdump 'src 10.0.2.4 and (dst port 3389 or 22)'
```
<a name="huePw"></a>
#### 过滤 TCP 标记位
TCP RST<br />The filters below find these various packets because tcp[13] looks at offset 13 in the TCP header, the number represents the location within  the byte, and the !=0 means that the flag in question is set to 1, i.e.  it’s on.
```bash
tcpdump 'tcp[13] & 4!=0'
tcpdump 'tcp[tcpflags] == tcp-rst'
```
TCP SYN
```bash
tcpdump 'tcp[13] & 2!=0'
tcpdump 'tcp[tcpflags] == tcp-syn'
```
同时忽略 SYN 和 ACK 标志的数据包
```bash
tcpdump 'tcp[13]=18'
```
TCP URG
```bash
tcpdump 'tcp[13] & 32!=0'
tcpdump 'tcp[tcpflags] == tcp-urg'
```
TCP ACK
```bash
tcpdump 'tcp[13] & 16!=0'
tcpdump 'tcp[tcpflags] == tcp-ack'
```
TCP PSH
```bash
tcpdump 'tcp[13] & 8!=0'
tcpdump 'tcp[tcpflags] == tcp-push'
```
TCP FIN
```bash
tcpdump 'tcp[13] & 1!=0'
tcpdump 'tcp[tcpflags] == tcp-fin'
```
<a name="vVkBr"></a>
#### 查找 http 包
查找 user-agent 信息
```bash
tcpdump -vvAls0 | grep 'User-Agent:'
```
查找只是 GET 请求的流量
```bash
tcpdump -vvAls0 | grep 'GET'
```
查找 http 客户端 IP
```bash
tcpdump -vvAls0 | grep 'Host:'
```
查询客户端 cookie
```bash
tcpdump -vvAls0 | grep 'Set-Cookie|Host:|Cookie:'
```
<a name="BIgq2"></a>
#### 查找 DNS 流量
```bash
tcpdump -vvAs0 port 53
```
<a name="y1Zy3"></a>
#### 查找对应流量的明文密码
```bash
tcpdump port http or port ftp or port smtp or port imap or port pop3 or port telnet -lA | egrep -i -B5 'pass=|pwd=|log=|login=|user=|username=|pw=|passw=|passwd= |password=|pass:|user:|username:|password:|login:|pass |user '
```
<a name="uO9Jo"></a>
#### wireshark 追踪流
wireshare 追踪流可以很好的了解出在一次交互过程中都发生了那些问题。<br />wireshare 选中包，右键选择 “追踪流“ 如果该包是允许的协议是可以打开该选项的
<a name="BLvTZ"></a>
#### 关于抓包节点和抓包设备
如何抓取有用的包，以及如何找到对应的接口，有以下建议<br />**抓包节点**：<br />通常情况下会在源端和目的端两端同时抓包，观察数据包是否从源端正常发出，目的端是否接收到数据包并给源端回包，以及源端是否正常接收到回包。如果有丢包现象，则沿网络链路上各节点抓包排查。例如，A 节点经过 c 节点到 B 节点，先在 AB 两端同时抓包，如果 B 节点未收到 A 节点的包，则在 c 节点同时抓包。<br />**抓包设备**：<br />对于 Kubernetes 集群中的 Pod，由于容器内不便于抓包，通常视情况在 Pod 数据包经过的 veth 设备，_docker0_ 网桥，_CNI_ 插件设备（如 cni0，flannel.1 etc..）及 Pod 所在节点的网卡设备上指定 Pod IP 进行抓包。选取的设备根据怀疑导致网络问题的原因而定，比如范围由大缩小，从源端逐渐靠近目的端，比如怀疑是 _CNI_ 插件导致，则在 _CNI_ 插件设备上抓包。从 pod 发出的包逐一经过 veth 设备，_cni0_ 设备，_flannel0_，宿主机网卡，到达对端，抓包时可按顺序逐一抓包，定位问题节点。<br />需要注意在不同设备上抓包时指定的源目 IP 地址需要转换，如抓取某 Pod 时，ping _{host}_ 的包，在 _veth_ 和 _cni0_ 上可以指定 Pod IP 抓包，而在宿主机网卡上如果仍然指定 Pod IP 会发现抓不到包，因为此时 Pod IP 已被转换为宿主机网卡 IP。<br />下图是一个使用 _VxLAN_ 模式的 _flannel_ 的跨界点通讯的网络模型，在抓包时需要注意对应的网络接口<br />![VxLAN in kubernetes](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267437-da7de957-f3ca-4427-be4a-32a955619dc6.png#clientId=uedc1184e-97af-4&from=paste&id=ue3bda694&originHeight=539&originWidth=938&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uc3796afc-2bae-4f67-a628-913822696f0&title=VxLAN%20in%20kubernetes "VxLAN in kubernetes")
<a name="isULH"></a>
### **nsenter**
nsenter 是一款可以进入进程的名称空间中。例如，如果一个容器以非 root 用户身份运行，而使用 docker exec 进入其中后，但该容器没有安装 sudo 或未 netstat ，并且您想查看其当前的网络属性，如开放端口，这种场景下将如何做到这一点？_**nsenter**_ 就是用来解决这个问题的。<br />**nsenter** (_namespace enter_) 可以在容器的宿主机上使用 _nsenter_ 命令进入容器的命名空间，以容器视角使用宿主机上的相应网络命令进行操作。当然需要拥有 _root_ 权限<br />**各系统下的安装** [**2\][1]**

- **Ubuntu/Debian: util-linux  ；**`**apt-get install -y util-linux**`
- **Centos/Fedora: util-linux ；**`**yum install -y util-linux**`
- **Apline：util-linux ；**`**apk add util-linux --no-cache**`

_nsenter_ 的 c 使用语法为，`nsenter -t pid -n <commond>`，`-t` 接 进程 ID 号，`-n` 表示进入名称空间内，`<commond>` 为执行的命令。<br />实例：如有一个 Pod 进程 ID 为 30858，进入该 Pod 名称空间内执行 `ifconfig` ，如下列所示
```bash
$ ps -ef|grep tail
root      17636  62887  0 20:19 pts/2    00:00:00 grep --color=auto tail
root      30858  30838  0 15:55 ?        00:00:01 tail -f

$ nsenter -t 30858 -n ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1480
        inet 192.168.1.213  netmask 255.255.255.0  broadcast 192.168.1.255
        ether 5e:d5:98:af:dc:6b  txqueuelen 0  (Ethernet)
        RX packets 92  bytes 9100 (8.8 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 92  bytes 8422 (8.2 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 5  bytes 448 (448.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 5  bytes 448 (448.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

net1: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.1.0.201  netmask 255.255.255.0  broadcast 10.1.0.255
        ether b2:79:f9:dd:2a:10  txqueuelen 0  (Ethernet)
        RX packets 228  bytes 21272 (20.7 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 216  bytes 20272 (19.7 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```
<a name="Q4XsB"></a>
#### 如何定位 Pod 名称空间
首先需要确定 Pod 所在的节点名称
```bash
$ kubectl get pods -owide |awk '{print $1,$7}'
NAME NODE
netbox-85865d5556-hfg6v master-machine
netbox-85865d5556-vlgr4 node01
```
如果 Pod 不在当前节点还需要用 IP 登录则还需要查看 IP（可选）
```bash
$ kubectl get pods -owide |awk '{print $1,$6,$7}'
NAME IP NODE
netbox-85865d5556-hfg6v 192.168.1.213 master-machine
netbox-85865d5556-vlgr4 192.168.0.4 node01
```
接下来，登录节点，获取容器 lD，如下列所示，每个 pod 默认有一个 _pause_ 容器，其他为用户 yaml 文件中定义的容器，理论上所有容器共享相同的网络命名空间，排查时可任选一个容器。
```bash
$ docker ps |grep netbox-85865d5556-hfg6v
6f8c58377aae   f78dd05f11ff                                                    "tail -f"                45 hours ago   Up 45 hours             k8s_netbox_netbox-85865d5556-hfg6v_default_4a8e2da8-05d1-4c81-97a7-3d76343a323a_0
b9c732ee457e   registry.cn-hangzhou.aliyuncs.com/google_containers/pause:3.1   "/pause"                 45 hours ago   Up 45 hours             k8s_POD_netbox-85865d5556-hfg6v_default_4a8e2da8-05d1-4c81-97a7-3d76343a323a_0
```
接下来获得获取容器在节点系统中对应的进程号，如下所示
```bash
$ docker inspect --format "{{ .State.Pid }}" 6f8c58377aae
30858
```
最后就可以通过 _nsenter_ 进入容器网络空间执行命令了
<a name="fzfZD"></a>
### **paping**
**paping** 命令可对目标地址指定端口以 TCP 协议进行连续 ping，通过这种特性可以弥补 _ping_ ICMP 协议，以及 _nmap_ , _telnet_ 只能进行一次操作的的不足；通常情况下会用于测试端口连通性和丢包率<br />paping download：**paping[2]**<br />_paping_ 还需要安装以下依赖，这取决于安装的 _paping_ 版本

- **RedHat/CentOS：**`**yum install -y libstdc++.i686 glibc.i686**`
- **Ubuntu/Debian：最小化安装无需依赖**
```bash
$ paping -h
paping v1.5.5 - Copyright (c) 2011 Mike Lovell

Syntax: paping [options] destination

Options:
 -?, --help     display usage
 -p, --port N   set TCP port N (required)
     --nocolor  Disable color output
 -t, --timeout  timeout in milliseconds (default 1000)
 -c, --count N  set number of checks to N
```
<a name="Zyp7Z"></a>
### **mtr**
**mtr** 是一个跨平台的网络诊断工具，将 **traceroute** 和 **ping** 的功能结合到一个工具。与 _traceroute_ 不同的是 _mtr_ 显示的信息比起 _traceroute_ 更加丰富：通过 _mtr_ 可以确定网络的条数，并且可以同时打印响应百分比以及网络中各跳跃点的响应时间。<br />**各系统下的安装** [**2\][3]**

- **Ubuntu/Debian: mtr  ；**`**apt-get install -y mtr**`
- **Centos/Fedora: mtr ；**`**yum install -y mtr**`
- **Apline：mtr ；**`**apk add mtr --no-cache**`
<a name="hidnB"></a>
#### 简单的使用示例
最简单的示例，就是后接域名或 IP，这将跟踪整个路由
```bash
$ mtr google.com

Start: Thu Jun 28 12:10:13 2018
HOST: TecMint                     Loss%   Snt   Last   Avg  Best  Wrst StDev
  1.|-- 192.168.0.1                0.0%     5    0.3   0.3   0.3   0.4   0.0
  2.|-- 5.5.5.211                  0.0%     5    0.7   0.9   0.7   1.3   0.0
  3.|-- 209.snat-111-91-120.hns.n 80.0%     5    7.1   7.1   7.1   7.1   0.0
  4.|-- 72.14.194.226              0.0%     5    1.9   2.9   1.9   4.4   1.1
  5.|-- 108.170.248.161            0.0%     5    2.9   3.5   2.0   4.3   0.7
  6.|-- 216.239.62.237             0.0%     5    3.0   6.2   2.9  18.3   6.7
  7.|-- bom05s12-in-f14.1e100.net  0.0%     5    2.1   2.4   2.0   3.8   0.5
```
-n 强制 _mtr_ 打印 IP 地址而不是主机名
```bash
$ mtr -n google.com

Start: Thu Jun 28 12:12:58 2018
HOST: TecMint                     Loss%   Snt   Last   Avg  Best  Wrst StDev
  1.|-- 192.168.0.1                0.0%     5    0.3   0.3   0.3   0.4   0.0
  2.|-- 5.5.5.211                  0.0%     5    0.9   0.9   0.8   1.1   0.0
  3.|-- ???                       100.0     5    0.0   0.0   0.0   0.0   0.0
  4.|-- 72.14.194.226              0.0%     5    2.0   2.0   1.9   2.0   0.0
  5.|-- 108.170.248.161            0.0%     5    2.3   2.3   2.2   2.4   0.0
  6.|-- 216.239.62.237             0.0%     5    3.0   3.2   3.0   3.3   0.0
  7.|-- 172.217.160.174            0.0%     5    3.7   3.6   2.0   5.3   1.4
```
`-b` 同时显示 IP 地址与主机名
```bash
$ mtr -b google.com

Start: Thu Jun 28 12:14:36 2018
HOST: TecMint                     Loss%   Snt   Last   Avg  Best  Wrst StDev
  1.|-- 192.168.0.1                0.0%     5    0.3   0.3   0.3   0.4   0.0
  2.|-- 5.5.5.211                  0.0%     5    0.7   0.8   0.6   1.0   0.0
  3.|-- 209.snat-111-91-120.hns.n  0.0%     5    1.4   1.6   1.3   2.1   0.0
  4.|-- 72.14.194.226              0.0%     5    1.8   2.1   1.8   2.6   0.0
  5.|-- 108.170.248.209            0.0%     5    2.0   1.9   1.8   2.0   0.0
  6.|-- 216.239.56.115             0.0%     5    2.4   2.7   2.4   2.9   0.0
  7.|-- bom07s15-in-f14.1e100.net  0.0%     5    3.7   2.2   1.7   3.7   0.9
```
`-c` 跟一个具体的值，这将限制 _mtr_ ping 的次数，到达次数后会退出
```bash
$ mtr -c5 google.com
```
如果需要指定次数，并且在退出后保存这些数据，使用 -r flag
```bash
$ mtr -r -c 5 google.com >  1
$ cat 1
Start: Sun Aug 21 22:06:49 2022
HOST: xxxxx.xxxxx.xxxx.xxxx Loss%   Snt   Last   Avg  Best  Wrst StDev
  1.|-- gateway                    0.0%     5    0.6 146.8   0.6 420.2 191.4
  2.|-- 212.xx.21.241              0.0%     5    0.4   1.0   0.4   2.3   0.5
  3.|-- 188.xxx.106.124            0.0%     5    0.7   1.1   0.7   2.1   0.5
  4.|-- ???                       100.0     5    0.0   0.0   0.0   0.0   0.0
  5.|-- 72.14.209.89               0.0%     5   43.2  43.3  43.1  43.3   0.0
  6.|-- 108.xxx.250.33             0.0%     5   43.2  43.1  43.1  43.2   0.0
  7.|-- 108.xxx.250.34             0.0%     5   43.7  43.6  43.5  43.7   0.0
  8.|-- 142.xxx.238.82             0.0%     5   60.6  60.9  60.6  61.2   0.0
  9.|-- 142.xxx.238.64             0.0%     5   59.7  67.5  59.3  89.8  13.2
 10.|-- 142.xxx.37.81              0.0%     5   62.7  62.9  62.6  63.5   0.0
 11.|-- 142.xxx.229.85             0.0%     5   61.0  60.9  60.7  61.3   0.0
 12.|-- xx-in-f14.1e100.net  0.0%     5   59.0  58.9  58.9  59.0   0.0
```
默认使用的是 ICMP 协议 -i ，可以指定 -u,  -t 使用其他协议
```bash
mtr --tcp google.com
```
`-m` 指定最大的跳数
```bash
mtr -m 35 216.58.223.78
```
`-s` 指定包的大小
<a name="YCRiu"></a>
#### mtr 输出的数据
| **colum** | **describe** |
| --- | --- |
| last | 最近一次的探测延迟值 |
| avg | 探测延迟的平均值 |
| best | 探测延迟的最小值 |
| wrst | 探测延迟的最大值 |
| stdev | 标准偏差。越大说明相应节点越不稳定 |

<a name="x8JF7"></a>
#### 丢包判断
任一节点的 Loss%（丢包率）如果不为零，则说明这一跳网络可能存在问题。导致相应节点丢包的原因通常有两种。

- **运营商基于安全或性能需求，人为限制了节点的 ICMP 发送速率，导致丢包。**
- **节点确实存在异常，导致丢包。可以结合异常节点及其后续节点的丢包情况，来判定丢包原因。**

Notes:

- **如果随后节点均没有丢包，则通常说明异常节点丢包是由于运营商策略限制所致。可以忽略相关丢包。**
- **如果随后节点也出现丢包，则通常说明节点确实存在网络异常，导致丢包。对于这种情况，如果异常节点及其后续节点连续出现丢包，而且各节点的丢包率不同，则通常以最后几跳的丢包率为准。如链路测试在第 5、6、7 跳均出现了丢包。最终丢包情况以第 7 跳作为参考。**
<a name="BxMM1"></a>
#### 延迟判断
由于链路抖动或其它因素的影响，节点的 _Best_ 和 _Worst_ 值可能相差很大。而 _Avg_（平均值）统计了自链路测试以来所有探测的平均值，所以能更好的反应出相应节点的网络质量。而 _StDev_（标准偏差值）越高，则说明数据包在相应节点的延时值越不相同（越离散）。所以标准偏差值可用于协助判断 _Avg_ 是否真实反应了相应节点的网络质量。例如，如果标准偏差很大，说明数据包的延迟是不确定的。可能某些数据包延迟很小（例如：25ms），而另一些延迟却很大（例如：350ms），但最终得到的平均延迟反而可能是正常的。所以此时 _Avg_ 并不能很好的反应出实际的网络质量情况。<br />这就需要结合如下情况进行判断：

- **如果 _StDev_ 很高，则同步观察相应节点的 _Best_ 和 _wrst_，来判断相应节点是否存在异常。**
- **如果 _StDev_ 不高，则通过 Avg 来判断相应节点是否存在异常。**
<a name="ou26S"></a>
## Pod 网络排查流程
Pod 网络异常时排查思路，可以按照下图所示<br />![Pod network troubleshooting idea](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267286-0f121225-8124-441b-b7f5-34258530939d.png#clientId=uedc1184e-97af-4&from=paste&id=u5a50dc2b&originHeight=687&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u87098e14-3ec3-42c3-aa52-142e0a73418&title=Pod%20network%20troubleshooting%20idea "Pod network troubleshooting idea")
<a name="odAtD"></a>
## 案例学习
<a name="XaoIb"></a>
### **扩容节点访问 service 地址不通**
**测试环境 k8s 节点扩容后无法访问集群 clusterlP 类型的 registry 服务**<br />环境信息：

| **IP** | **Hostname** | **role** |
| --- | --- | --- |
| 10.153.204.15 | yq01-aip-aikefu12 | worknode 节点（本次扩容的问题节点） |
| 10.153.203.14 | yq01-aip-aikefu31 | master 节点 |
| 10.61.187.42 | yq01-aip-aikefu2746f8e9 | master 节点 |
| 10.61.187.48 | yq01-aip-aikefu30b61e25 | master 节点（本次 registry 服务 pod 所在 节点） |

- **cni 插件：flannel vxlan**
- **kube-proxy 工作模式为 iptables**
- **registry 服务**
   - **单实例部署在 10.61.187.48:5000**
   - **Pod IP：10.233.65.46，**
   - **Cluster IP：10.233.0.100**

现象：

- **所有节点之间的 pod 通信正常**
- **任意节点和 Pod curl registry 的 Pod 的 _IP:5000_ 均可以连通**
- **新扩容节点 10.153.204.15 curl registry 服务的 Cluster lP 10.233.0.100:5000 不通，其他节点 curl 均可以连通**

分析思路：

- **根据现象 1 可以初步判断 _CNI_ 插件无异常**
- **根据现象 2 可以判断 _registry_ 的 _Pod_ 无异常**
- **根据现象 3 可以判断 _registry_ 的 _service_ 异常的可能性不大，可能是新扩容节点访问 _registry_ 的 _service_ 存在异常**

怀疑方向：

- **问题节点的 kube-proxy 存在异常**
- **问题节点的 iptables 规则存在异常**
- **问题节点到 service 的网络层面存在异常**

排查过程：

- **排查问题节点的 **`**kube-proxy**`
- **执行 **`**kubectl get pod -owide -nkube-system l grep kube-proxy**`** 查看 _kube-proxy_ Pod 的状态，问题节点上的 _kube-proxy_ Pod 为 _running_ 状态**
- **执行 **`**kubecti logs <nodename> <kube-proxy pod name> -nkube-system**`** 查看问题节点 _kube-proxy_ 的 Pod 日志，没有异常报错**
- **在问题节点操作系统上执行 **`**iptables -S -t nat**`** 查看 **`**iptables**`** 规则**

排查过程：<br />确认存在到 _registry_ 服务的 Cluster lP _10.233.0.100_ 的 _KUBE-SERVICES_ 链，跳转至 _KUBE-SVC-_* 链做负载均衡，再跳转至 _KUBE-SEP-_* 链通过 _DNAT_ 替换为服务后端 Pod 的 IP 10.233.65.46。因此判断 iptables 规则无异常执行 route-n 查看问题节点存在访问 10.233.65.46 所在网段的路由，如图所示<br />![10.233.65.46 路由](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267193-049365c8-7761-49dd-a296-996f3b62b281.png#clientId=uedc1184e-97af-4&from=paste&id=u434dc3a9&originHeight=253&originWidth=661&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua7a8fab8-1289-48fd-a49d-bfd8a3b86b3&title=10.233.65.46%20%E8%B7%AF%E7%94%B1 "10.233.65.46 路由")<br />查看对端的回程路由<br />![回程路由](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267290-bf039613-284b-4dd2-ba06-d4f91ee57344.png#clientId=uedc1184e-97af-4&from=paste&id=uc52cd9a3&originHeight=271&originWidth=789&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0f5793e5-6e26-48b7-a01a-b66fcfd710a&title=%E5%9B%9E%E7%A8%8B%E8%B7%AF%E7%94%B1 "回程路由")<br />以上排查证明问题原因不是 _cni_ 插件或者 _kube-proxy_ 异常导致，因此需要在访问链路上抓包，判断问题原因、问题节点执行 curl 10.233.0.100:5000，在问题节点和后端 pod 所在节点的 flannel.1 上同时抓包发包节点一直在重传，Cluster lP 已 _DNAT_ 转换为后端 Pod IP，如图所示<br />![抓包过程，发送端](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267792-81c50cf5-ab83-4310-b373-9e2b5696b705.png#clientId=uedc1184e-97af-4&from=paste&id=ub667532c&originHeight=99&originWidth=814&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u7b8c8cf0-ca3c-4151-8596-c82ed25b2c4&title=%E6%8A%93%E5%8C%85%E8%BF%87%E7%A8%8B%EF%BC%8C%E5%8F%91%E9%80%81%E7%AB%AF "抓包过程，发送端")<br />后端 Pod（ _registry_ 服务）所在节点的 _flannel.1_ 上未抓到任何数据包，如图所示<br />![抓包过程，服务端](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267660-784b2135-7ee9-46e4-aa0d-a404680dc63c.png#clientId=uedc1184e-97af-4&from=paste&id=uab9f8e8e&originHeight=79&originWidth=727&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ud20bdd1c-55d4-4528-b02a-2bc8c0c4dff&title=%E6%8A%93%E5%8C%85%E8%BF%87%E7%A8%8B%EF%BC%8C%E6%9C%8D%E5%8A%A1%E7%AB%AF "抓包过程，服务端")<br />请求 _service_ 的 _ClusterlP_ 时，在两端物理机网卡抓包，发包端如图所示，封装的源端节点 IP 是 10.153.204.15，但一直在重传<br />![包传送过程，发送端](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267767-1b9be09b-25fe-4eff-868f-612567e51ce3.png#clientId=uedc1184e-97af-4&from=paste&id=ucc70c9eb&originHeight=151&originWidth=808&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucf023ecd-5202-4402-a3db-2baa2cd5977&title=%E5%8C%85%E4%BC%A0%E9%80%81%E8%BF%87%E7%A8%8B%EF%BC%8C%E5%8F%91%E9%80%81%E7%AB%AF "包传送过程，发送端")<br />收包端收到了包，但未回包，如图所示<br />![包传送过程，服务端](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267784-92576fb9-1363-4902-b599-1176fbbcb3b6.png#clientId=uedc1184e-97af-4&from=paste&id=ud063cfc9&originHeight=135&originWidth=760&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u6a2ab279-284d-4e17-8e74-294b8e60f3f&title=%E5%8C%85%E4%BC%A0%E9%80%81%E8%BF%87%E7%A8%8B%EF%BC%8C%E6%9C%8D%E5%8A%A1%E7%AB%AF "包传送过程，服务端")<br />由此可以知道，NAT 的动作已经完成，而只是后端 Pod（ _registry_ 服务）没有回包，接下来在问题节点执行 curl10.233.65.46:5000，在问题节点和后端（ _registry_ 服务）Pod 所在节点的 _flannel.1_ 上同时抓包，两节点收发正常，发包如图所示<br />![正常包发送端](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088267843-bf47841f-5424-4070-851f-eca74b126304.png#clientId=uedc1184e-97af-4&from=paste&id=ue79f903a&originHeight=169&originWidth=866&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3bda448c-38af-46a9-b6e5-b9ce28efc96&title=%E6%AD%A3%E5%B8%B8%E5%8C%85%E5%8F%91%E9%80%81%E7%AB%AF "正常包发送端")<br />![正常包接收端](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088268069-9b294441-90a2-4c6c-acb2-467711ac648d.png#clientId=uedc1184e-97af-4&from=paste&id=u64be4483&originHeight=171&originWidth=867&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u33d0cd06-57e7-48ea-9d51-ed502a75b4c&title=%E6%AD%A3%E5%B8%B8%E5%8C%85%E6%8E%A5%E6%94%B6%E7%AB%AF "正常包接收端")<br />接下来在两端物理机网卡接口抓包，因为数据包通过物理机网卡会进行 _vxlan_ 封装，需要抓 _vxlan_ 设备的 8472 端口，发包端如图所示<br />发现网络链路连通，但封装的 IP 不对，封装的源端节点 IP 是 10.153.204.228，但是存在问题节点的 IP 是 10.153.204.15<br />![问题节点物理机网卡接口抓包](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088268361-b53adf83-b402-487c-bcf1-de7723ee277f.png#clientId=uedc1184e-97af-4&from=paste&id=ue98094c3&originHeight=266&originWidth=811&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u33575cc5-1c82-48f0-a2c3-8f54332e31c&title=%E9%97%AE%E9%A2%98%E8%8A%82%E7%82%B9%E7%89%A9%E7%90%86%E6%9C%BA%E7%BD%91%E5%8D%A1%E6%8E%A5%E5%8F%A3%E6%8A%93%E5%8C%85 "问题节点物理机网卡接口抓包")<br />后端 Pod 所在节点的物理网卡上抓包，注意需要过滤其他正常节点的请求包，如图所示；发现收到的数据包，源地址是 10.153.204.228，但是问题节点的 IP 是 10.153.204.15。<br />![对端节点物理机网卡接口抓包](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088268528-510df08c-c6ec-4119-ac3a-39420311b7bb.png#clientId=uedc1184e-97af-4&from=paste&id=udb759210&originHeight=272&originWidth=854&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u32f30a8a-ac18-4a41-bab2-9fa0365e25f&title=%E5%AF%B9%E7%AB%AF%E8%8A%82%E7%82%B9%E7%89%A9%E7%90%86%E6%9C%BA%E7%BD%91%E5%8D%A1%E6%8E%A5%E5%8F%A3%E6%8A%93%E5%8C%85 "对端节点物理机网卡接口抓包")<br />此时问题以及清楚了，是一个 Pod 存在两个 IP，导致发包和回包时无法通过隧道设备找到对端的接口，所以发可以收到，但不能回。<br />问题节点执行 `ip addr`，发现网卡 _enp26s0f0_ 上配置了两个 IP，如图所示<br />![问题节点 IP](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088268611-5026a060-f7d6-4977-becf-49f88aac5334.png#clientId=uedc1184e-97af-4&from=paste&id=u9fd9527b&originHeight=145&originWidth=532&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u76c898dd-bc77-491b-8cd1-0e2868338a0&title=%E9%97%AE%E9%A2%98%E8%8A%82%E7%82%B9%20IP "问题节点 IP")<br />进一步查看网卡配置文件，发现网卡既配置了静态 IP，又配置了 dhcp 动态获取 IP。如图所示<br />![问题节点网卡配置](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088268633-830f665f-d9e3-4cff-8dab-64f557a37df7.png#clientId=uedc1184e-97af-4&from=paste&id=ud8f63f93&originHeight=352&originWidth=574&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uefc36291-ec6d-452b-8760-503ca17d274&title=%E9%97%AE%E9%A2%98%E8%8A%82%E7%82%B9%E7%BD%91%E5%8D%A1%E9%85%8D%E7%BD%AE "问题节点网卡配置")<br />最终定位原因为问题节点既配置了 dhcp 获取 IP，又配置了静态 IP，导致 IP 冲突，引发网络异常<br />解决方法：修改网卡配置文件 /etc/sysconfig/network-scripts/ifcfg-enp26s0f0 里 BOOTPROTO="dhcp"为 BOOTPROTO="none"；重启 _docker_ 和 _kubelet_ 问题解决。
<a name="J89mC"></a>
### **集群外云主机调用集群内应用超时**
问题现象：Kubernetes 集群外云主机以 http post 方式访问 Kubernetes 集群应用接口超时<br />环境信息：Kubernetes 集群：calicoIP-IP 模式，应用接口以 nodeport 方式对外提供服务<br />客户端：Kubernetes 集群之外的云主机<br />排查过程：

- **在云主机 telnet 应用接口地址和端口，可以连通，证明网络连通正常，如图所示**
- **云主机上调用接口不通，在云主机和 Pod 所在 Kubernetes 节点同时抓包，使用 wireshark 分析数据包**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088268775-27470124-2779-48b7-8610-d534b1f3e686.png#clientId=uedc1184e-97af-4&from=paste&id=u693d6bff&originHeight=269&originWidth=410&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8d453e2c-ab8f-47b2-81dd-a99605ff427&title=)<br />通过抓包结果分析结果为 TCP 链接建立没有问题，但是在传输大数据的时候会一直重传 **1514** 大小的第一个数据包直至超时。怀疑是链路两端 MTU 大小不一致导致（现象：某一个固定大小的包一直超时的情况）。如图所示，1514 大小的包一直在重传。<br />报文 1-3 TCP 三次握手正常<br />报文 1 info 中 MSS 字段可以看到 MSS 协商为 1460，MTU=1460+20bytes（IP 包头）+20bytes（TCP 包头）=1500<br />报文 7 k8s 主机确认了包 4 的数据包，但是后续再没有对数据的 ACK<br />报文 21-29 可以看到云主机一直在发送后面的数据，但是没有收到 k8s 节点的 ACK，结合 pod 未收到任何报文，表明是 k8s 节点和 POD 通信出现了问题。<br />![wireshark 分析](https://cdn.nlark.com/yuque/0/2022/png/396745/1662088269032-45d5ff39-4055-4e17-9504-f81df8177e97.png#clientId=uedc1184e-97af-4&from=paste&id=ud450b848&originHeight=302&originWidth=952&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua570e9a5-7732-47be-a352-3664eded39b&title=wireshark%20%E5%88%86%E6%9E%90 "wireshark 分析")<br />在云主机上使用 `ping -s` 指定数据包大小，发现超过 1400 大小的数据包无法正常发送。结合以上情况，定位是云主机网卡配置的 MTU 是 1500，_tunl0_ 配置的 MTU 是 1440，导致大数据包无法发送至 _tunl0_ ，因此 Pod 没有收到报文，接口调用失败。<br />解决方法：修改云主机网卡 MTU 值为 1440，或者修改 calico 的 MTU 值为 1500，保持链路两端 MTU 值一致。
<a name="q3IfI"></a>
### **集群 pod 访问对象存储超时**
环境信息：公有云环境，Kubernetes 集群节点和对象存储在同一私有网络下，网络链路无防火墙限制 k8s 集群开启了节点自动弹缩（CA）和 Pod 自动弹缩（HPA），通过域名访问对象存储，Pod 使用集群 DNS 服务，集群 DNS 服务配置了用户自建上游 DNS 服务器<br />排查过程：

- **使用 nsenter 工具进入 pod 容器网络命名空间测试，ping 对象存储域名不通，报错 unknown server name，ping 对象存储 lP 可以连通。**
- **telnet 对象存储 80/443 端口可以连通。**
- **paping 对象存储 80/443 端口无丢包。**
- **为了验证 Pod 创建好以后的初始阶段网络连通性，将以上测试动作写入 dockerfile，重新生成容器镜像并创 pod，测试结果一致。**

通过上述步骤，判断 Pod 网络连通性无异常，超时原因为域名解析失败，怀疑问题如下：

- **集群 DNS 服务存在异常**
- **上游 DNS 服务存在异常**
- **集群 DNS 服务与上游 DNS 通讯异常**
- **pod 访问集群 DNS 服务异常**

根据上述方向排查，集群 DNS 服务状态正常，无报错。测试 Pod 分别使用集群 DNS 服务和上游 DNS 服务解析域名，前者解析失败，后者解析成功。至此，证明上游 DNS 服务正常，并且集群 DNS 服务日志中没有与上游 DNS 通讯超时的报错。定位到的问题：Pod 访问集群 DNS 服务超时<br />此时发现，出现问题的 Pod 集中在新弹出的 Kubernetes 节点上。这些节点的 kube-proxy Pod 状态全部为 _pending_，没有正常调度到节点上。因此导致该节点上其他 Pod 无法访问包括 dns 在内的所有 Kubernetes service。<br />再进一步排查发现 kube-proxy Pod 没有配置 priorityclass 为最高优先级，导致节点资源紧张时为了将高优先级的应用 Pod 调度到该节点，将原本已运行在该节点的 kube-proxy 驱逐。<br />解决方法：将 kube-proxy 设置 priorityclass 值为 system-node-critical 最高优先级，同时建议应用 Pod 配置就绪探针，测试可以正常连通对象存储域名后再分配任务。
