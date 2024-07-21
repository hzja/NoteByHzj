Kubernetes<br />容器的本质就是一个特殊的进程，特殊在为其创建了 NameSpace 隔离运行环境，并用 Cgroups 为其控制资源开销。<br />借助这两个底层技术，可以成功实现应用容器化，但如何让多个容器在网络环境不互相干扰的情况下还能互相通信，让容器可以访问外部网络，让外部网络可以访问特定容器等等的这些容器的**网络问题**还得再利用一些 **Linux 网络虚拟化技术**。
<a name="wWv56"></a>
## 容器网络隔离：NameSpace
让多个容器的网络环境不会互相干扰，可以延续之前 NameSpace 的知识点。<br />在前面介绍的 8 种 NameSpace 中，有一个 Network NameSpace ，可以借助这个来给容器配置独立的网络视图。<br />先看宿主机所处的 Default Network NameSpace ：
```bash
[root@host ~]# readlink /proc/$$/ns/net
net:[4026531956]
[root@host ~]#
```
以 net:[4026531956] 为例，net 代表了 NameSpace 的类型，4026531956 则是 NameSpace 的 inode number。<br />经过之前的文章介绍，已经深知容器的本质就是进程，本文也不再浪费笔墨，接下来的操作将直接透过上层直达本质，**借助 Linux 底层提供的能力来探索和还原容器网络的实现**。（后续本文创建出的进程请直接理解为容器）<br />首先通过 ip netns 工具创建两个网络命名空间 netns1 和 netns2 ：
```bash
[root@host ~]# ip netns add netns1
[root@host ~]# ip netns add netns2
[root@host ~]# ip netns list
netns2
netns1
[root@host ~]#
```
在这两个网络命名空间之上分别创建两个 bash ~~进程~~容器，其中 container1 ：
```bash
[root@host ~]# ip netns exec netns1 /bin/bash --rcfile <(echo "PS1=\"container1> \"")
container1> readlink /proc/$$/ns/net
net:[4026532165]
container1> ip link # 查看网络设备列表
1: lo: <LOOPBACK> mtu 65536 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
container1> route -n # 查看路由表
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
container1> iptables -L # 查看 iptables 规则
Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
container1>
```
同样的，container2 ：
```bash
[root@host ~]# ip netns exec netns2 /bin/bash --rcfile <(echo "PS1=\"container2> \"")
container2> readlink /proc/$$/ns/net
net:[4026532219]
container2> ip link # 查看网络设备列表
1: lo: <LOOPBACK> mtu 65536 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
container2> route -n # 查看路由表
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
container2> iptables -L # 查看 iptables 规则
Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination
container2>
```
可以看出，由于 Network NameSpace 隔离的作用，不同的容器（ container1 和 container2 ）拥有自己独立的网络协议栈，包括网络设备、路由表、ARP 表、iptables 规则、socket 等，所有的容器都会以为自己运行在独立的网络环境中。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244577358-8ec3a03c-130b-43d1-b814-4ec4c327090a.png#averageHue=%23e2d9d8&clientId=ud3c90f48-4bb0-4&from=paste&id=ua94f5e8a&originHeight=784&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u79c6ceb2-0418-42f7-8eb7-67e875f0a60&title=)<br />现在准备一个简单的 go web 服务，并分别在 container1 和 container2 的后台运行：
```go
package main

import (
    "fmt"
    "net/http"
    "os"
)

func main() {
    name := os.Args[1]
    http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
        fmt.Println("req")
        w.Write([]byte(name + "\n"))
    })
    fmt.Println(name, "listen :8080")
    panic(http.ListenAndServe(":8080", nil))
}
```
container1 ：
```bash
container1> go run main.go container1 > container1.log &
[1] 2866
container1> tail container1.log
container1 listen :8080
container1>
```
container2 ：
```bash
container2> go run main.go container2 > container2.log &
[1] 2955
container2> tail container2.log
container2 listen :8080
container2>
```
当前在同一主机下，即使 container1 和 container2 都监听着 8080 端口，也并不会发生端口冲突。<br />测试一下刚才所启动的服务的可用性，以 container1 为例：
```bash
container1> curl localhost:8080
curl: (7) Failed to connect to ::1: Network is unreachable
container1>
```
此时访问不通是因为根本还没有启用任何网络设备，包括基本的 lo 回路设备。直接将其启用即可：
```bash
container1> ifconfig
container1> ifup lo
container1> ifconfig
lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

container1> curl localhost:8080
container1
container1>
```
container2 同理。
<a name="MmVff"></a>
## 容器点对点通信：Veth
当前两个容器处于不同的 Network NameSpace 中，它们的网络环境是互相隔离的，你不认识我，我也不认识你，它们之间自然也无法进行网络通信。<br />在进入解决办法之前，回顾下，在现实世界中，如果两台计算机需要互相进行网络通信。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244577333-97b7b99d-9d1b-4074-86a9-5d3e4e958a2e.png#averageHue=%23dcdcdc&clientId=ud3c90f48-4bb0-4&from=paste&id=ube6578c0&originHeight=435&originWidth=951&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u95d0e362-1a6f-40aa-8fe0-03bc009c92f&title=)<br />只需要很简单地直接使用一根网线把这两台计算机的网口给连接起来，就可以了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244577353-7e3970d1-8565-422d-ae44-6bb8eb46ae11.png#averageHue=%23d7d6d6&clientId=ud3c90f48-4bb0-4&from=paste&id=ud387779c&originHeight=399&originWidth=905&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud447ace1-7cad-44fe-9957-39353b27ce9&title=)<br />那么回到容器层面呢，抽象一点，容器能不能也有一个类似的网口可以用来“插一根网线”呢？<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244577375-63e92020-7340-4012-8bb9-b7e21ae0ba20.png#averageHue=%23f6f6f6&clientId=ud3c90f48-4bb0-4&from=paste&id=ucb3a6fa6&originHeight=466&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2faee986-4560-478d-b8e2-4ca8f2427f9&title=)<br />答案当然是可以的，在 Linux 网络虚拟化技术中就提供了一种这样的用软件来模拟硬件网卡的方式： Veth（Virtual Ethernet devices）。<br />和一根网线有两端一样，Veth 也是成对出现的，所以也被称为 **veth pair** 。假设 veth1 和 veth2 是一对设备，那么从 veth1 进入的数据包将会在 veth2 收到，反过来也一样。所以只要将一对 Veth 分别放入两个 Network Namespace 中，这两个 Network Namespace 就会像连接了网线一样，可以互相通信了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244577323-4721b128-eb49-43c7-8679-15d0a1c39bda.png#averageHue=%23f9f4f4&clientId=ud3c90f48-4bb0-4&from=paste&id=ue5e23539&originHeight=407&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5d9b0e30-23b8-4359-926c-b6d0106bba9&title=)<br />开始实践，首先查看宿主机上已有的网络设备列表：
```bash
[root@host ~]# ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether 52:54:00:26:eb:d4 brd ff:ff:ff:ff:ff:ff
[root@host ~]#
```
创建一个 veth pair （包含 veth1 和 veth2 两个虚拟网络设备）：
```bash
[root@host ~]# ip link add veth1 type veth peer name veth2
[root@host ~]# ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether 52:54:00:26:eb:d4 brd ff:ff:ff:ff:ff:ff
3: veth2@veth1: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 6a:01:c8:fa:9e:6e brd ff:ff:ff:ff:ff:ff
4: veth1@veth2: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 42:7e:de:c6:89:ff brd ff:ff:ff:ff:ff:ff
[root@host ~]#
```
直接将一端的虚拟网卡 veth1 放入 netns1 命名空间，另一端的 veth2 放入 netns2 命名空间，这样就相当于使用网线将两个命名空间连接起来了：
```bash
[root@host ~]# ip link set veth1 netns netns1
[root@host ~]# ip link set veth2 netns netns2
[root@host ~]#
```
连接起来后就可以在 container1 和 container2 容器中查看到各自对应的网络设备：
```bash
container1> ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
4: veth1@if3: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 42:7e:de:c6:89:ff brd ff:ff:ff:ff:ff:ff link-netnsid 1
container1>
```
```bash
container2> ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
3: veth2@if4: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 6a:01:c8:fa:9e:6e brd ff:ff:ff:ff:ff:ff link-netnsid 0
container2>
```
分别为这两个网卡设置 IP 地址，使其位于同一个子网 172.17.0.0/24 中，然后启用网卡：
```bash
container1> ip addr add 172.17.0.101/24 dev veth1
container1> ip link set dev veth1 up
container1> ifconfig
lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 10  bytes 942 (942.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 10  bytes 942 (942.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

veth1: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        inet 172.17.0.101  netmask 255.255.255.0  broadcast 0.0.0.0
        ether 42:7e:de:c6:89:ff  txqueuelen 1000  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

container1>
```
```bash
container2> ip addr add 172.17.0.102/24 dev veth2
container2> ip link set dev veth2 up
container2> ifconfig
lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 10  bytes 942 (942.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 10  bytes 942 (942.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

veth2: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.17.0.102  netmask 255.255.255.0  broadcast 0.0.0.0
        inet6 fe80::6801:c8ff:fefa:9e6e  prefixlen 64  scopeid 0x20<link>
        ether 6a:01:c8:fa:9e:6e  txqueuelen 1000  (Ethernet)
        RX packets 6  bytes 516 (516.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 6  bytes 516 (516.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

container2>
```
测试 container1 和 container2 容器互相访问对方的服务：
```bash
container1> curl 172.17.0.102:8080
container2
container1>
```
```bash
container2> curl 172.17.0.101:8080
container1
container2>
```
到这里，只需依靠 Veth ，就得到了一个点对点的二层网络拓扑，容器点对点通信的问题也得以成功解决。<br />![Veth 工作原理](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244600317-75e07819-41d2-4bea-afcc-9cd275843f57.png#averageHue=%23f8f1f0&clientId=ud3c90f48-4bb0-4&from=paste&id=u58e9710f&originHeight=398&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud913f9f8-ae2e-430c-8b4a-908cad620f4&title=Veth%20%E5%B7%A5%E4%BD%9C%E5%8E%9F%E7%90%86 "Veth 工作原理")
<a name="JaerZ"></a>
## 容器间互相通信：Bridge
在现实世界中，不可能只有两台计算机，当有第三台、第四台，乃至无数台计算机加入到网络环境中的时候，不可能有这么多网口可以彼此两两互相连接。为了解决这个问题，便发明了二层交换机（或网桥）。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244600329-396b39a4-016a-4c1f-bd49-b245a42515d3.png#averageHue=%23dedddd&clientId=ud3c90f48-4bb0-4&from=paste&id=u8d0b24b0&originHeight=697&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue0eb1d37-9392-44cc-9e58-175ac9b2677&title=)<br />对于容器也是如此，如果有 3 个或以上的 Namespace 需要接入同一个二层网络，就不能简单的只使用 Veth 了。不过和使用 Veth 作为虚拟网卡的方式一样，贴心的 Linux 同样提供了网桥（交换机）的虚拟实现方式：Bridge 。<br />在 netns1 和 netns2 命名空间的基础上再创建一个 netns3 命名空间：
```bash
[root@host ~]# ip netns add netns3
[root@host ~]# ip netns list
netns3
netns2 (id: 1)
netns1 (id: 0)
[root@host ~]#
```
重复之前的操作，创建 container3 容器：
```bash
[root@host ~]# ip netns exec netns3 /bin/bash --rcfile <(echo "PS1=\"container3> \"")
container3> readlink /proc/$$/ns/net
net:[4026532277]
container3> go run main.go container3 > container3.log &
[1] 4270
container3> tail container3.log
container3 listen :8080
container3> ifup lo
container3> ifconfig
lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

container3> curl localhost:8080
container3
container3>
```
在为这三个容器互相通网之前，先把之前给 container1 和 container2 容器之间连接的“网线”（veth1 和 veth2 ）拔了（只需在其中一个容器中操作）：
```bash
container1> ip link delete veth1
container1> ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
container1>
```
现在三个容器之间就谁也不认识谁了。<br />回归实践，创建一个 Bridge ，并将其启用：
```bash
[root@host ~]# ip link add br0 type bridge
[root@host ~]# ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether 52:54:00:26:eb:d4 brd ff:ff:ff:ff:ff:ff
5: br0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 9e:ac:12:15:98:64 brd ff:ff:ff:ff:ff:ff
[root@host ~]# ip link set dev br0 up
[root@host ~]# ifconfig
br0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet6 fe80::9cac:12ff:fe15:9864  prefixlen 64  scopeid 0x20<link>
        ether 9e:ac:12:15:98:64  txqueuelen 1000  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 6  bytes 516 (516.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.0.12.15  netmask 255.255.252.0  broadcast 10.0.15.255
        inet6 fe80::5054:ff:fe26:ebd4  prefixlen 64  scopeid 0x20<link>
        ether 52:54:00:26:eb:d4  txqueuelen 1000  (Ethernet)
        RX packets 114601  bytes 160971385 (153.5 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 18824  bytes 2035143 (1.9 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 15  bytes 2000 (1.9 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 15  bytes 2000 (1.9 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

[root@host ~]#
```
准备三条“网线”（三对 veth）：
```bash
[root@host ~]# ip link add veth1 type veth peer name veth1-br
[root@host ~]# ip link add veth2 type veth peer name veth2-br
[root@host ~]# ip link add veth3 type veth peer name veth3-br
[root@host ~]#
```
将 veth1 插入 netns1 、veth1-br 插入 br0 、 veth2 插入 netns2 、veth2-br 插入 br0 、 veth3 插入 netns3 、veth3-br 插入 br0 （记得启用 veth*-br ）：
```bash
[root@host ~]# ip link set dev veth1 netns netns1
[root@host ~]# ip link set dev veth2 netns netns2
[root@host ~]# ip link set dev veth3 netns netns3
[root@host ~]# ip link set dev veth1-br master br0
[root@host ~]# ip link set dev veth2-br master br0
[root@host ~]# ip link set dev veth3-br master br0
[root@host ~]# ip link set dev veth1-br up
[root@host ~]# ip link set dev veth2-br up
[root@host ~]# ip link set dev veth3-br up
[root@host ~]#
```
分别在三个容器中，为各自的网卡设置 IP 地址，并使其位于同一个子网 172.17.0.0/24 中，设置完后同样需要进行启用操作：
```bash
container1> ip addr add 172.17.0.101/24 dev veth1
container1> ip link set dev veth1 up
container1>
```
```bash
container2> ip addr add 172.17.0.102/24 dev veth2
container2> ip link set dev veth2 up
container2>
```
```bash
container3> ip addr add 172.17.0.103/24 dev veth3
container3> ip link set dev veth3 up
container3>
```
测试 container1 、 container2 、container3 容器互相访问对方的服务：
```bash
container1> curl 172.17.0.102:8080
container2
container1> curl 172.17.0.103:8080
container3
container1>
```
```bash
container2> curl 172.17.0.101:8080
container1
container2> curl 172.17.0.103:8080
container3
container2>
```
```bash
container3> curl 172.17.0.101:8080
container1
container3> curl 172.17.0.102:8080
container2
container3>
```
至此，在 Veth 的基础上，引入 Bridge 功能，将多个 Namespace 连接到了同一个二层网络中，容器间互相通信的问题得以成功解决。<br />![Veth+Bridge 工作原理](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244600301-aedd63f9-8642-45fd-b331-3d60ffe2a2b6.png#averageHue=%23e5e3e3&clientId=ud3c90f48-4bb0-4&from=paste&id=u87d3f160&originHeight=539&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u0f05a0fc-19ef-4413-bd2a-0d97dfa1e2e&title=Veth%2BBridge%20%E5%B7%A5%E4%BD%9C%E5%8E%9F%E7%90%86 "Veth+Bridge 工作原理")
<a name="fufSI"></a>
## 容器与外部网络通信：route 和 iptables
目前为止，实验都是处于同一子网中。但实际的应用场景，更多的是需要容器可以与外部进行互通。<br />在现实世界中，二层交换机只能解决同一子网内的数据流向，对于不同子网，就需要使用三层路由器（或网关）来转发。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244600342-52daae9c-5543-4093-8a88-4de090d15040.png#averageHue=%23d9d9d9&clientId=ud3c90f48-4bb0-4&from=paste&id=u1b716695&originHeight=870&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueb729332-33db-45ec-ba3b-9e767231063&title=)<br />不过和之前 Linux 提供了交换机的虚拟化实现 Bridge 不同，Linux 并没有提供一个虚拟的路由器设备。因为 Linux 其自身就已经具备了路由器的功能，可以直接用来充当路由器，更准确地说，在 Linux 中，**一个 Network Namespace 就可以承担一个路由器的功能**。<br />在 Linux Network Namespace 中，路由功能的定义其实很简单：**直接通过定义路由表规则就可以决定将请求的数据包流向到指定的网络设备上。**<br />路由的规则都定义在了路由表，对于路由表，最常用的是 local 和 main ，当然也可以另外配置其他表，其中 local 的优先级要高于 main ，平时访问本机（localhost）的请求，都会直接在 local 表中找到规则，不会再经过 main 。<br />可以查看所有的路由表：
```bash
[root@host ~]# cat /etc/iproute2/rt_tables
#
# reserved values
#
255     local
254     main
253     default
0       unspec
#
# local
#
#1      inr.ruhep
[root@host ~]#
```
如果要查看指定的路由表中的规则可以使用 `ip route list table 表名` ：
```bash
[root@host ~]# ip route list table local
broadcast 10.0.12.0 dev eth0 proto kernel scope link src 10.0.12.15
local 10.0.12.15 dev eth0 proto kernel scope host src 10.0.12.15
broadcast 10.0.15.255 dev eth0 proto kernel scope link src 10.0.12.15
broadcast 127.0.0.0 dev lo proto kernel scope link src 127.0.0.1
local 127.0.0.0/8 dev lo proto kernel scope host src 127.0.0.1
local 127.0.0.1 dev lo proto kernel scope host src 127.0.0.1
broadcast 127.255.255.255 dev lo proto kernel scope link src 127.0.0.1
[root@host ~]# ip route list table main
default via 10.0.12.1 dev eth0
10.0.12.0/22 dev eth0 proto kernel scope link src 10.0.12.15
169.254.0.0/16 dev eth0 scope link metric 1002
[root@host ~]# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         10.0.12.1       0.0.0.0         UG    0      0        0 eth0
10.0.12.0       0.0.0.0         255.255.252.0   U     0      0        0 eth0
169.254.0.0     0.0.0.0         255.255.0.0     U     1002   0        0 eth0
[root@host ~]#
```
平时使用的 `route -n` 命令实际就是在查看 main 路由表。
<a name="hq6Tc"></a>
### 容器和宿主机互通
现在有 container1 、 container2 和 container3 三个容器，其 IP 分别为：172.17.0.101 、172.17.0.102 和 172.17.0.103 ，它们都处于同一子网中。<br />查看宿主机的 IP ，这里是 10.0.12.15 ：
```bash
[root@host ~]# ifconfig
...
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.0.12.15  netmask 255.255.252.0  broadcast 10.0.15.255
        inet6 fe80::5054:ff:fe26:ebd4  prefixlen 64  scopeid 0x20<link>
        ether 52:54:00:26:eb:d4  txqueuelen 1000  (Ethernet)
        RX packets 119923  bytes 161411733 (153.9 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 24106  bytes 2884317 (2.7 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
...
[root@host ~]#
```
查看容器（以 container1 为例）的路由规则：
```bash
container1> route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
172.17.0.0      0.0.0.0         255.255.255.0   U     0      0        0 veth1
container1>
```
可以发现，当前容器的路由表中只有一条规则：当访问 172.17.0.0/24 子网内的 IP 时，例如 172.17.0.102 ，数据包直接转发到 veth1 设备。<br />根据 Veth 的性质，对应 veth1 的另一端 veth1-br 设备就会接收到数据包，又由于 veth1-br 设备是连接在 br0 二层交换机上的，所以 veth2-br 设备也会接收到该数据包，最后就到达了对应的 veth2 目标设备。<br />现在的目的就是要让容器访问除了 172.17.0.0/24 子网内的 IP 的其它 IP 时（比如宿主机 IP 10.0.12.15），也可以把数据包转发出去，也就是需要给容器增加路由规则。<br />回想之前的 Bridge ，从网络角度上看，是把它当作一个二层交换机，所以不需要为它设置 IP 地址，但现在从宿主机的角度上看，br0 同时也是 Default Network Namespace 里面的一张网卡，可以直接为这张网卡设置 IP 后，来充当三层路由器（网关），参与主机的路由转发。<br />在宿主机上给 br0 设备设置 IP 地址为 172.17.0.1 （同样位于子网 172.17.0.0/24 内）：
```bash
[root@host ~]# ip addr add local 172.17.0.1/24 dev br0
[root@host ~]# ifconfig
br0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.17.0.1  netmask 255.255.255.0  broadcast 0.0.0.0
        inet6 fe80::9cac:12ff:fe15:9864  prefixlen 64  scopeid 0x20<link>
        ether 16:bd:7d:ca:53:bf  txqueuelen 1000  (Ethernet)
        RX packets 27  bytes 1716 (1.6 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 8  bytes 656 (656.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
......
```
此时，宿主机会自动增加一条 Destination 为 172.17.0.0 的路由规则：
```bash
[root@host ~]# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         10.0.12.1       0.0.0.0         UG    0      0        0 eth0
10.0.12.0       0.0.0.0         255.255.252.0   U     0      0        0 eth0
169.254.0.0     0.0.0.0         255.255.0.0     U     1002   0        0 eth0
172.17.0.0      0.0.0.0         255.255.255.0   U     0      0        0 br0
[root@host ~]#
```
按照这条路由规则（访问目标主机为 172.17.0.0/24 的数据包会转发到 br0 设备），现在的宿主机就可以直接访问容器了：
```bash
[root@host ~]# ping 172.17.0.101
PING 172.17.0.101 (172.17.0.101) 56(84) bytes of data.
64 bytes from 172.17.0.101: icmp_seq=1 ttl=64 time=0.025 ms
64 bytes from 172.17.0.101: icmp_seq=2 ttl=64 time=0.037 ms
64 bytes from 172.17.0.101: icmp_seq=3 ttl=64 time=0.031 ms
^C
--- 172.17.0.101 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 1999ms
rtt min/avg/max/mdev = 0.025/0.031/0.037/0.005 ms
[root@host ~]# curl 172.17.0.101:8080
container1
[root@host ~]#
```
现在宿主机可以访问容器了，回到容器访问宿主机的问题上，给容器增加路由转发规则，使默认所有路由（0.0.0.0 ，除了 172.17.0.0/24 子网内的 IP ）都需要透过作用于三层的 br0 网关（172.17.0.1）来转递封包：
```bash
container1> ip route add default via 172.17.0.1
container1> route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         172.17.0.1      0.0.0.0         UG    0      0        0 veth1
172.17.0.0      0.0.0.0         255.255.255.0   U     0      0        0 veth1
container1>
```
这样一来，当容器访问宿主机 IP 时，数据包就会直接转发到 Default Network Namespace 的 br0 网卡上，宿主机发现收到的 IP 数据包是属于自己的，就会对其进行处理。<br />也就是说现在容器也可以访问宿主机了：
```bash
container1> ping 10.0.12.15
PING 10.0.12.15 (10.0.12.15) 56(84) bytes of data.
64 bytes from 10.0.12.15: icmp_seq=1 ttl=64 time=0.022 ms
64 bytes from 10.0.12.15: icmp_seq=2 ttl=64 time=0.031 ms
64 bytes from 10.0.12.15: icmp_seq=3 ttl=64 time=0.032 ms
^C
--- 10.0.12.15 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 1999ms
rtt min/avg/max/mdev = 0.022/0.028/0.032/0.006 ms
container1>
```
<a name="r6HJM"></a>
### 容器访问其它主机（外网）
上面给容器配置了路由规则，使其访问宿主机 IP 时，数据包就会转发到 Default Network Namespace 的 br0 网卡上，宿主机发现收到的 IP 数据包是属于自己的，就会对其进行处理。<br />但在 Linux 中，如果发现收到的 IP 数据包并不是属于自己的，是会将其直接丢弃的。这里准备了一个和宿主机（10.0.12.15）同一子网的另一台 IP 地址为 10.0.12.11 的主机 host2 来测试：
```bash
[root@host2 ~]# ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.0.12.11  netmask 255.255.252.0  broadcast 10.0.15.255
        inet6 fe80::5054:ff:fe78:a238  prefixlen 64  scopeid 0x20<link>
        ether 52:54:00:78:a2:38  txqueuelen 1000  (Ethernet)
        RX packets 6103  bytes 2710292 (2.5 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 4662  bytes 640093 (625.0 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 10  bytes 1360 (1.3 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 10  bytes 1360 (1.3 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

[root@host2 ~]#
```
在 container1 内 ping host2 ，以及访问外网，和预期一致，无法访问：
```bash
container1> ping 10.0.12.11
PING 10.0.12.11 (10.0.12.11) 56(84) bytes of data.
^C
--- 10.0.12.11 ping statistics ---
16 packets transmitted, 0 received, 100% packet loss, time 14999ms

container1> curl baidu.com
curl: (6) Could not resolve host: baidu.com; Unknown error
container1>
```
如果想要在容器中可以访问其他主机，或者外网，就不能让 Linux 丢弃不属于自己的数据包，而是继续转发出去。要做到这一点，只需要打开 Linux 的 IP Forward 功能：
```bash
[root@host ~]# cat /proc/sys/net/ipv4/ip_forward # 0 代表关闭，1 代表开启
0
[root@host ~]# vi /etc/sysctl.d/30-ipforward.conf
net.ipv4.ip_forward=1
net.ipv6.conf.default.forwarding=1
net.ipv6.conf.all.forwarding=1
[root@host ~]# sysctl -p /etc/sysctl.d/30-ipforward.conf
net.ipv4.ip_forward = 1
net.ipv6.conf.default.forwarding = 1
net.ipv6.conf.all.forwarding = 1
[root@host ~]# cat /proc/sys/net/ipv4/ip_forward # 0 代表关闭，1 代表开启
1
[root@host ~]#
```
但是，这里还是会有一个问题，当容器访问其他主机或者外网时（veth1->br0），Linux 也转发了出去（br0->eth0），但其他主机或者外网在响应了容器的请求时，它并不认识配置的 172.17.0.0/24 这个网段，还是无法成功处理请求。所以还需要使用 NAT（ Network Address Translation ）技术：把 IP 数据报文头中的 IP 地址和端口转换成另一个 IP 地址和端口。
<a name="ZTIlD"></a>
#### NAT 技术也被用于 IPv4 地址枯竭问题
在这里需要修改的是源 IP 地址，所以是 SNAT （Source NAT），把 IP 转换成宿主机出口网卡的 IP 。<br />在 Linux 中，可以通过 **iptables** 中的 MASQUERADE 策略来实现这个 SNAT ：
```bash
[root@host ~]# iptables -t nat -A POSTROUTING -s 172.17.0.0/24 ! -o br0 -j MASQUERADE
[root@host ~]# iptables -t nat -nL
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination

Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination
MASQUERADE  all  --  172.17.0.0/24        0.0.0.0/0
[root@host ~]#
```
这条规则的意思是将不是从网卡 br0 发出的且源地址为 172.17.0.0/24 的数据包（即容器中发出的数据包）做 SNAT 。<br />此时，容器就可以访问其它主机（外网）了：
```bash
container1> ping 10.0.12.11
PING 10.0.12.11 (10.0.12.11) 56(84) bytes of data.
64 bytes from 10.0.12.11: icmp_seq=1 ttl=63 time=0.231 ms
64 bytes from 10.0.12.11: icmp_seq=2 ttl=63 time=0.216 ms
64 bytes from 10.0.12.11: icmp_seq=3 ttl=63 time=0.206 ms
^C
--- 10.0.12.11 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 1999ms
rtt min/avg/max/mdev = 0.206/0.217/0.231/0.019 ms
container1> curl baidu.com
<html>
<meta http-equiv="refresh" content="0;url=http://www.baidu.com/">
</html>
container1>
```
<a name="vEH2Z"></a>
### 外部访问容器（容器端口映射）
容器与外部网络通信，已经解决了容器和宿主机间的通信，容器访问其他主机（外网），还剩最后一个，外部来访问容器。<br />在 Docker 中，要让外部可以访问容器服务，会做一次容器端口映射，如：
```bash
[root@host ~]# docker run -p 8000:8080 xxx
```
通过 `-p` 参数就可以将容器内的 8080 端口映射到宿主机的 8000 端口上，这样外部就可以通过访问宿主机 IP + 8000 端口访问到容器服务了。<br />这一技术实现其实同样是使用到了 NAT 技术，只不过和上面的 SNAT 不同的是，这里需要修改的是目的 IP 地址，即 DNAT （Destination NAT），把宿主机上 8000 端口的流量请求转发到容器中的地址 172.17.0.101:8080 中。<br />在 Linux 中，可以通过 **iptables** 中的 DNAT 策略来实现这个 DNAT ：
```bash
[root@host ~]# iptables -t nat -A PREROUTING  ! -i br0 -p tcp -m tcp --dport 8000 -j DNAT --to-destination 172.17.0.101:8080
[root@host ~]# iptables -t nat -nL
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination
DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0            tcp dpt:8000 to:172.17.0.101:8080

Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination
MASQUERADE  all  --  172.17.0.0/24        0.0.0.0/0
[root@host ~]#
```
这条规则的意思是将访问宿主机 8000 端口的请求转发到 172.17.0.101 的 8080 端口上，也就是说现在可以在 host2 主机（10.0.12.11）上通过访问 10.0.12.15:8000 来访问实际的 container1 服务了：
```bash
[root@host2 ~]# curl 10.0.12.15:8000
container1
[root@host2 ~]#
```
到这里，就已经实现了一个和 Docker 默认网络模式一样的拓扑关系：<br />![容器网络拓扑](https://cdn.nlark.com/yuque/0/2023/png/396745/1680244618612-93113beb-00fd-4bfe-8fb1-a1910d411b7a.png#averageHue=%23e1e0e0&clientId=ud3c90f48-4bb0-4&from=paste&id=u1868c46d&originHeight=700&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub98a4e39-6a32-4b9a-b11f-ddcd87e6c32&title=%E5%AE%B9%E5%99%A8%E7%BD%91%E7%BB%9C%E6%8B%93%E6%89%91 "容器网络拓扑")
<a name="MY54l"></a>
## 总结
Docker 容器网络用到的技术就是上面的 Veth+Bridge+route+iptables ，只不过是换成了 Go 语言来实现这些配置。<br />只有当了解了这些底层技术，平时在处理容器网络问题时，才能够更加得心应手。<br />本文所介绍的仅仅是 Docker 本身默认的网络模型，而后面的 CNCF 的 CNI 容器网络接口（Kubernetes 的网络模型）， Service Mesh + CNI 的层次化 SDN 将会越来越复杂。
