Linux
<a name="fJ2bL"></a>
## 1、Linux 下的流量控制原理
通过对包的排队，可以控制数据包的发送方式。这种控制，称之为数据整形，shape the data，包括对数据的以下操作:

- **增加延时**
- **丢包**
- **重新排列**
- **重复、损坏**
- **速率控制**

在 qdisc-class-filter 结构下，对流量进行控制需要进行三个步骤:

- **创建 qdisc 队列**

上面提到 Linux 是通过包的排队进行流量的控制，那么首先得有一个队列。

- **创建 class 分类**

class 实际上，就是划分流量策略分类。比如划分两档流量限速 10MBps、20MBbs。

- **创建 filter 过滤**

虽然创建了 class 分类，但是并没有将任何的 IP、Port 绑定到 class 上，此时并不会有控制作用。还需要创建 filter 将指定的 IP、Port 绑定到 class 上，才能使流量控制 class 生效于资源。<br />TC 是 Linux 下提供的流量控制工具，也是 Cilium/eBPF 等网络组件的核心基础设施之一。
<a name="PbJwG"></a>
## 2、限制指定 IP、Port 对本机的访问速度
<a name="DwrSV"></a>
### 2.1 查看网卡
```bash
ifconfig

eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 1.1.1.1  netmask 255.255.254.0  broadcast 1.1.1.1
        inet6 1::1:1:1:1  prefixlen 64  scopeid 0x20<link>
        ether 1:1:1:1:1:1  txqueuelen 1000  (Ethernet)
        RX packets 2980910  bytes 2662352343 (2.4 GiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 1475969  bytes 122254809 (116.5 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```
<a name="C5jy9"></a>
### 2.2 配置 qdisc-class-filter

- **创建 qdisc 根队列**
```bash
tc qdisc add dev eth0 root handle 1: htb default 1
```

- **创建第一级 class 绑定所有带宽资源**

注意这里的单位是 6 MBps，也就是 48 Mbps。
```bash
tc class add dev eth0 parent 1:0 classid 1:1 htb rate 6MBps burst 15k
```

- **创建子分类 class**

可以创建多个子分类，对资源的流量进行精细化管理。
```bash
tc class add dev eth0 parent 1:1 classid 1:10 htb rate 6MBps ceil 10MBps burst 15k
```
这里 ceil 设置的是上限，正常情况下限速为 6MBps，但网络空闲时，可以达到 10 MBps。

- **创建过滤器 filter，限制 IP**
```bash
tc filter add dev eth0 protocol ip parent 1:0 prio 1 u32 match ip dst 1.2.3.3 flowid 1:10
```
这里对 1.2.3.4 进行限制带宽为 1:10，也就是 6MBps。当然，也可以直接给网段 1.2.0.0/16 加 class 策略。
<a name="iDE2g"></a>
### 2.3 查看并清理配置

- **查看 class 配置**
```bash
tc class show dev eth0

class htb 1:10 parent 1:1 leaf 10: prio 0 rate 48Mbit ceil 80Mbit burst 15Kb cburst 1600b 
class htb 1:1 root rate 48Mbit ceil 48Mbit burst 15Kb cburst 1590b
```

- **查看 filter 配置**
```bash
tc filter show dev eth0

filter parent 1: protocol ip pref 1 u32 chain 0 
filter parent 1: protocol ip pref 1 u32 chain 0 fh 800: ht divisor 1 
filter parent 1: protocol ip pref 1 u32 chain 0 fh 800::800 order 2048 key ht 800 bkt 0 flowid 1:10 not_in_hw 
  match 01020303/ffffffff at 16
```

- **清理全部配置**
```bash
tc qdisc del dev eth0 root
```
<a name="TzoDD"></a>
## 3、限制本机对指定 IP、Port 的访问速度
由于排队规则主要是基于出口方向，不能对入口方向的流量（Ingress）进行限制。因此，需要将流量重定向到 ifb 设备上，再对 ifb 的出口流量（Egress）进行限制，以最终达到控制的目的。
<a name="hehKG"></a>
### 3.1 启用虚拟网卡

- **将在 ifb 设备**
```bash
modprobe ifb numifbs=1
```

- **启用 ifb0 虚拟设备**
```bash
ip link set dev ifb0 up
```
<a name="OSrAq"></a>
### 3.2 配置 qdisc-class-filter

- **添加 qdisc**
```bash
tc qdisc add dev eth0 handle ffff: ingress
```

- **重定向网卡流量到 ifb0**
```bash
tc filter add dev eth0 parent ffff: protocol ip u32 match u32 0 0 action mirred egress redirect dev ifb0
```

- **添加 class 和 filter**
```bash
tc qdisc add dev ifb0 root handle 1: htb default 10
tc class add dev ifb0 parent 1:0 classid 1:1 htb rate 6Mbps
tc class add dev ifb0 parent 1:1 classid 1:10 htb rate 6Mbps
tc filter add dev ifb0 parent 1:0 protocol ip prio 16 u32 match ip dst 1.2.3.4  flowid 1:10
```
<a name="MTGGl"></a>
### 3.3 查看并清理配置

- **下面是限速本机对指定 IP 访问的监控图**

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683159768893-05fad31e-b34f-4a53-bd1e-eabf316b6866.png#averageHue=%2315171a&clientId=u50efb1ec-7ee7-4&from=paste&id=u229d2c17&originHeight=324&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5f13f55e-4afb-4b4a-8e39-088c8848195&title=)<br />进入的流量被限制在 6 MBps 以下，而出去的流量不被限制。

- **查看 class 配置**
```bash
tc class show dev ifb0

class htb 1:10 parent 1:1 prio 0 rate 48Mbit ceil 48Mbit burst 1590b cburst 1590b 
class htb 1:1 root rate 48Mbit ceil 48Mbit burst 1590b cburst 1590b 
```

- **查看 filter 配置**
```bash
tc filter show dev ifb0

filter parent 1: protocol ip pref 16 u32 chain 0 
filter parent 1: protocol ip pref 16 u32 chain 0 fh 800: ht divisor 1 
filter parent 1: protocol ip pref 16 u32 chain 0 fh 800::800 order 2048 key ht 800 bkt 0 flowid 1:10 not_in_hw 
  match 01020304/ffffffff at 16
```

- **清理全部配置**
```bash
tc qdisc del dev eth0 ingress
tc qdisc del dev ifb0 root
modprobe -r ifb
```
<a name="v9BFw"></a>
## 4、参考

- [https://arthurchiao.art/blog/lartc-qdisc-zh/](https://arthurchiao.art/blog/lartc-qdisc-zh/)
- [https://serverfault.com/questions/350023/tc-ingress-policing-and-ifb-mirroring](https://serverfault.com/questions/350023/tc-ingress-policing-and-ifb-mirroring)
