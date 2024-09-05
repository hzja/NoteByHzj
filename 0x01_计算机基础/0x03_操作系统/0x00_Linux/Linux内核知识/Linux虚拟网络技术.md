Linux 虚拟网络
<a name="meDt6"></a>
## 背景
在Linux虚拟化技术中，网络层面，通常重要的三个技术分别是Network Namespace、veth pair、以及网桥或虚拟交换机技术。通过实验学习Linux网络虚拟化技术。<br />首先了解下Network Namespace，它是由Linux内核提供，是实现网络虚拟化的重要功能。通过创建多个隔离的网络空间，实现网络资源的隔离。<br />不同的Network Namespace的资源互相不可见，彼此之间无法通信。如下图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599895896203-6f641ca5-1e97-4244-a447-ecce2211ae2b.png#height=295&id=QQ9di&originHeight=886&originWidth=1057&originalType=binary&ratio=1&rotation=0&showTitle=false&size=65632&status=done&style=shadow&title=&width=352.3333333333333)<br />Network Namespace
<a name="roLs4"></a>
## `ip netns`命令
Network Namespace是Linux内核提供的功能，借助ip命令来完成各种操作。ip命令来自于iproute2安装包，一般系统默认安装，如果没有的话，可自行安装。<br />ip命令管理的功能很多，和Network Namespace有关的操作都在其子命令ip netns下进行的，可以通过`ip netns help`查询命令帮助信息
```bash
$ ip netns help
Usage: ip netns list
       ip netns add NAME
       ip netns set NAME NETNSID
       ip [-all] netns delete [NAME]
       ip netns identify [PID]
       ip netns pids NAME
       ip [-all] netns exec [NAME] cmd ...
       ip netns monitor
       ip netns list-id
```
<a name="IFwiP"></a>
### 创建Network Namespace
<a name="EQSE4"></a>
#### 1、通过 `ip netns add` 命令创建一个名为ns0的网络命名空间：
```bash
$ ip netns add ns0
```
<a name="T5tR5"></a>
#### 2、查询命名空间
```bash
$ ip netns list
ns0
```
<a name="vEFuY"></a>
#### 3、命名空间所在目录
```bash
$ ls /var/run/netns/
ns0
```
> 注意：新创建的 Network Namespace 会出现在/var/run/netns/目录下。如果需要管理其他不是 ip netns 创建的 network namespace，只要在这个目录下创建一个指向对应 network namespace 文件的链接即可。

<a name="kyzDS"></a>
### 操作Network Namespace
对于每个 Network Namespace 来说，它会有自己独立的网卡、路由表、ARP 表、iptables 等和网络相关的资源。ip命令提供了ip netns exec子命令可以在对应的 Network Namespace 中执行命令。
<a name="jXlZ6"></a>
#### 1、查看网络命名空间 ns0 的网卡信息
```bash
$ ip netns exec ns0 ip addr
1: lo: <LOOPBACK> mtu 65536 qdisc noop state DOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
```
每个 namespace 在创建时会自动创建一个回环接口lo，默认不启用。它的作用和Linux系统中默认看到的lo一样，都是为了实现loopback通信，如果希望lo口能工作，可以通过下面的步骤2启用它。
<a name="X5ke1"></a>
#### 2、启用lo回环网卡：
```bash
$ ip netns exec ns0 ip link set lo up
```
再次检查回环网卡状态：
```bash
$ ip netns exec ns0 ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
```
会发现此时回环口卡处于UP状态，并且系统分配127.0.0.1/8的ip地址。
<a name="jvynr"></a>
#### 3、在 ns0 中打开一个shell终端
```bash
$ ip netns exec ns0 /bin/bash
$ ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
$ exit
exit
```
通过执行ip netns exec ns0 /bin/bash进入ns0的shell终端，后面所有的命令都在这个Network Namespace中执行，好处是不用每次执行命令时都要带上ip netns exec ，缺点是无法清楚知道自己当前所在的shell，容易混淆。<br />可以采用下面的方法解决：
```bash
$ ip netns exec ns0 /bin/bash --rcfile <(echo "PS1=\"ns0> \"")
ns0>
```
<a name="vjT1M"></a>
### Network Namespace 之间的通信
默认情况下，network namespace 是不能和主机网络，或者其他 network namespace 通信的。<br />可以使用 Linux 提供的veth pair来完成通信，veth pair可以理解为使用网线连接好的两个接口，把两个端口放到两个namespace中，那么这两个namespace就能打通。<br />接下来通过实验进行验证：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599896456364-a6ad7579-a58d-4e4c-bf7c-43afef8385c3.png#height=204&id=HAnjY&originHeight=612&originWidth=1024&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55013&status=done&style=shadow&title=&width=341.3333333333333)<br />实验拓扑图
<a name="uU8T4"></a>
#### 1、创建veth pair
```bash
[root@iZuligp6e1dyzfZ ~ 15:41]# ip link add type veth
[root@iZuligp6e1dyzfZ ~ 15:42]# ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000
    link/ether 00:16:3e:0a:ce:22 brd ff:ff:ff:ff:ff:ff
3: docker0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP mode DEFAULT group default
    link/ether 02:42:29:46:fd:24 brd ff:ff:ff:ff:ff:ff
15: vethde9f880@if14: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP mode DEFAULT group default
    link/ether 4a:30:bf:3c:4d:9c brd ff:ff:ff:ff:ff:ff link-netnsid 1
17: veth8c80a5d@if16: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP mode DEFAULT group default
    link/ether 4e:d3:6d:57:87:c5 brd ff:ff:ff:ff:ff:ff link-netnsid 2
18: veth0@veth1: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether fa:a3:34:cc:87:62 brd ff:ff:ff:ff:ff:ff
19: veth1@veth0: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether be:bf:2b:4f:34:81 brd ff:ff:ff:ff:ff:ff
```
可以看到，此时系统中新增了一对veth pair：veth0和veth1，需要记住的是veth pair无法单独存在，删除其中一个，另一个也会自动消失。<br />如果需要指定veth pair两个端点的名称，可以使用下面的命令：
```bash
[root@iZuligp6e1dyzfZ ~ 15:42]# ip link add veth001 type veth peer name veth002
[root@iZuligp6e1dyzfZ ~ 15:42]# ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000
    link/ether 00:16:3e:0a:ce:22 brd ff:ff:ff:ff:ff:ff
3: docker0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP mode DEFAULT group default
    link/ether 02:42:29:46:fd:24 brd ff:ff:ff:ff:ff:ff
15: vethde9f880@if14: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP mode DEFAULT group default
    link/ether 4a:30:bf:3c:4d:9c brd ff:ff:ff:ff:ff:ff link-netnsid 1
17: veth8c80a5d@if16: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP mode DEFAULT group default
    link/ether 4e:d3:6d:57:87:c5 brd ff:ff:ff:ff:ff:ff link-netnsid 2
18: veth0@veth1: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether fa:a3:34:cc:87:62 brd ff:ff:ff:ff:ff:ff
19: veth1@veth0: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether be:bf:2b:4f:34:81 brd ff:ff:ff:ff:ff:ff
20: veth002@veth001: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether ea:d5:f2:02:7f:a3 brd ff:ff:ff:ff:ff:ff
21: veth001@veth002: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 22:e7:10:7a:d7:93 brd ff:ff:ff:ff:ff:ff
```
<a name="rb5eG"></a>
#### 2、创建Network Namespace
已创建了一个名为ns0的Network Namespace，下面再创建一个名称为ns1的网络命名空间。
```bash
[root@iZuligp6e1dyzfZ ~ 15:42]# ip netns add ns1
[root@iZuligp6e1dyzfZ ~ 15:43]# ip netns list
ns1
ns0
```
<a name="acSYN"></a>
#### 3、把veth pair分别加入到这两个namespace中
将veth0加入到ns0，将veth1加入到ns1，如下所示：
```bash
[root@iZuligp6e1dyzfZ ~ 15:43]# ip link set veth0 netns ns0
[root@iZuligp6e1dyzfZ ~ 15:43]# ip link set veth1 netns ns1
```
<a name="kEagV"></a>
#### 4、分别为这对veth pair配置上ip地址，并启用
<a name="F1voQ"></a>
##### 1）为veth0配置IP，并启用该虚拟网卡
```bash
[root@iZuligp6e1dyzfZ ~ 15:43]# ip netns exec ns0 ip addr add 192.168.1.1/24 dev veth0
[root@iZuligp6e1dyzfZ ~ 15:44]# ip netns exec ns0 ip link set veth0 up
```
<a name="d8tPj"></a>
##### 2）为veth1配置IP，并启用该虚拟网卡
```bash
[root@iZuligp6e1dyzfZ ~ 15:44]# ip netns exec ns1 ip addr add 192.168.1.2/24 dev veth1
[root@iZuligp6e1dyzfZ ~ 15:44]# ip netns exec ns1 ip link set veth1 up
```
<a name="dy7RQ"></a>
#### 5、查看这对veth pair的状态
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599896828116-1863938a-f56d-4a2f-b0d0-674b1472a436.png#height=434&id=HR82s&originHeight=1301&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1615480&status=done&style=none&title=&width=1107.6666666666667)
<a name="teyql"></a>
#### 6、验证两个Network Namespace之间的互通
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599896895123-27494cac-021a-48b1-9848-97db33454ed4.png#height=324&id=BQY2q&originHeight=972&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1191789&status=done&style=none&title=&width=1107.6666666666667)<br />可以看到，veth pair成功实现了两个不同Network Namespace之间的网络交互。
<a name="Yjo3f"></a>
### 网桥
虽然veth pair可以实现两个 Network Namespace 之间的通信，但 veth pair 有一个明显的缺陷，就是只能实现两个网络接口之间的通信。如果多个network namespace需要进行通信，则需要借助bridge。<br />下面通过实验来进行讲解：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599897245465-1f119939-52f4-4ffc-af66-e047ef10e6db.png#height=299&id=Hul4E&originHeight=896&originWidth=1683&originalType=binary&ratio=1&rotation=0&showTitle=false&size=117793&status=done&style=shadow&title=&width=561)<br />实验拓扑图
<a name="Gn5mp"></a>
#### 0、还原网络环境
为方便接下来的实验，把刚刚创建的Network Namespace及veth pair删除，保证纯净的网络环境。
```bash
[root@iZuligp6e1dyzfZ / 15:47]# ip netns delete ns0
[root@iZuligp6e1dyzfZ / 15:54]# ip netns delete ns1
```
<a name="xmFKj"></a>
#### 1、创建3个Network Namespace
```bash
[root@iZuligp6e1dyzfZ / 15:55]# ip netns add ns0
[root@iZuligp6e1dyzfZ / 15:55]# ip netns add ns1
[root@iZuligp6e1dyzfZ / 15:55]# ip netns add ns2
```
<a name="OR7Co"></a>
#### 2、创建3对veth pair
```bash
[root@iZuligp6e1dyzfZ / 15:55]# ip link add type veth
[root@iZuligp6e1dyzfZ / 15:55]# ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000
    link/ether 00:16:3e:0a:ce:22 brd ff:ff:ff:ff:ff:ff
3: docker0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP mode DEFAULT group default
    link/ether 02:42:29:46:fd:24 brd ff:ff:ff:ff:ff:ff
15: vethde9f880@if14: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP mode DEFAULT group default
    link/ether 4a:30:bf:3c:4d:9c brd ff:ff:ff:ff:ff:ff link-netnsid 1
17: veth8c80a5d@if16: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master docker0 state UP mode DEFAULT group default
    link/ether 4e:d3:6d:57:87:c5 brd ff:ff:ff:ff:ff:ff link-netnsid 2
20: veth002@veth001: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether ea:d5:f2:02:7f:a3 brd ff:ff:ff:ff:ff:ff
21: veth001@veth002: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 22:e7:10:7a:d7:93 brd ff:ff:ff:ff:ff:ff
22: veth0@veth1: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 6a:2e:e3:2d:d7:fb brd ff:ff:ff:ff:ff:ff
23: veth1@veth0: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/ether 02:7b:cd:88:93:4d brd ff:ff:ff:ff:ff:ff
```
<a name="tsHhH"></a>
#### 3、创建网桥
```bash
// 创建名为 docker0 的网桥
[root@iZuligp6e1dyzfZ / 15:55]# ip link add docker0 type bridge
//启动 docker0 网桥
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev docker0 up
//为docker0网桥配置IP
[root@iZuligp6e1dyzfZ / 15:55]# ifconfig docker0 172.17.0.1/16
```
此时可以通过ifconfig命令查看：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599897402184-92d6b95a-26a7-4d93-9e68-c5b3cbcdc1f4.png#height=190&id=olxTH&originHeight=571&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=647885&status=done&style=none&title=&width=1107.6666666666667)<br />docker0
<a name="DPWCV"></a>
#### 4、绑定网口
Network Namespace、veth pair、bridge 都创建完毕，下面通过命令将每对veth pair的一端绑定在network namespace，另一端绑定在docker0网桥上，用于实现网络互通
<a name="czck6"></a>
##### 1）配置第一个网络命名空间 ns0
```bash
// 将veth1添加进ns0
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth1 netns ns0
//将veth1重命名为eth0
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns0 ip link set dev veth1 name eth0
//为ns0中的eth0配置ip
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns0 ip addr add 172.17.0.101/16 dev eth0
// 启动ns0中的eth0网卡
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns0 ip link set dev eth0 up
// 将veth0添加加网桥docker0
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth0 master docker0
// 启动veth0网卡
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth0 up
```
<a name="NeKCd"></a>
##### 2）配置第二个网络命名空间 ns1
```bash
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth3 netns ns1
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns1 ip link set dev veth3 name eth0
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns1 ip addr add 172.17.0.102/16 dev eth0
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns1 ip link set dev eth0 up
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth1 master docker0
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth3 up
```
<a name="uU0m9"></a>
##### 2）配置第三个网络命名空间 ns2
```bash
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth5 netns ns2
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns2 ip link set dev veth5 name eth0
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns2 ip addr add 172.17.0.103/16 dev eth0
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns1 ip link set dev eth0 up
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth5 master docker0
[root@iZuligp6e1dyzfZ / 15:55]# ip link set dev veth5 up
```
<a name="CxCYH"></a>
#### 5、查看绑定端口
```bash
$ brctl show
```
> 和网桥有关的操作还可以使用brctl，这个命令来自 bridge-utils安装包。这里使用brctl show来查询网桥docker0下绑定的网卡。

<a name="sR8Uc"></a>
#### 6、验证多个namespace之间的通信
```bash
// 进入ns0 bash终端
[root@iZuligp6e1dyzfZ / 15:55]# ip netns exec ns0 /bin/bash --rcfile <(echo "PS1=\"ns0> \"")
// ping 网桥docker0
ns0> ping -c 1 172.17.0.1
PING 172.17.0.1 (172.17.0.1) 56(84) bytes of data.
64 bytes from 172.17.0.1: icmp_seq=1 ttl=64 time=0.033 ms
...
// ping ns1上的eth0网卡
ns0> ping -c 1 172.17.0.102
PING 172.17.0.102 (172.17.0.102) 56(84) bytes of data.
64 bytes from 172.17.0.102: icmp_seq=1 ttl=64 time=0.049 ms
...
// ping ns2上的eth0网卡
ns0> ping -c 1 172.17.0.103
PING 172.17.0.103 (172.17.0.103) 56(84) bytes of data.
64 bytes from 172.17.0.103: icmp_seq=1 ttl=64 time=0.038 ms
...
```
