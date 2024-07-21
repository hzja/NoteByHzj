Kubernetes<br />了解在 Kubernetes 内外，数据包是如何转发的，从原始的 Web 请求开始，到托管应用程序的容器。<br />在深入了解在 Kubernetes 集群中数据包如何流转的细节之前，先明确一下 Kubernetes 对网络的要求。<br />Kubernetes 网络模型定义了一组基本规则：

- 在不使用网络地址转换 (NAT) 的情况下，集群中的 Pod 能够与任意其他 Pod 进行通信。
- 在不使用网络地址转换 (NAT) 的情况下，在集群节点上运行的程序能与同一节点上的任何 Pod 进行通信。
- 每个 Pod 都有自己的 IP 地址（IP-per-Pod），并且任意其他 Pod 都可以通过相同的这个地址访问它。

这些要求，不会将具体实现限制在某种解决方案上。<br />相反，它们笼统地描述了集群网络的特性。<br />为了满足这些限制，必须解决以下挑战：

1. 如何确保同一个 Pod 中的容器行为就像它们在同一个主机上一样？
2. 集群中的 Pod 能否访问其他 Pod？
3. Pod 可以访问服务吗？服务是负载均衡的吗？
4. Pod 可以接收集群外部的流量吗？

在本文中，将重点关注前三点，从 Pod 内的网络，容器到容器的通信说起。
<a name="p7Jut"></a>
## **Linux 网络命名空间如何在 Pod 中工作**
来看一个运行应用的主容器和伴随一起的另一个容器。<br />在示例中，有一个带有 nginx 和 busybox 容器的 Pod:
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: multi-container-Pod
spec:
  containers:
    - name: container-1
      image: busybox
      command: ['/bin/sh', '-c', 'sleep 1d']
    - name: container-2
      image: nginx
```
部署时，会发生以下事情：

1. Pod 在节点上拥有独立的网络命名空间。
2. 分配一个 IP 地址给 Pod ，两个容器之间共享端口。
3. 两个容器共享相同的网络命名空间，并在本地彼此可见。

网络配置在后台迅速完成。<br />但是，退后一步，尝试理解为什么运行容器需要上述动作。<br />在 Linux 中，网络命名空间是独立的、隔离的逻辑空间。<br />可以将网络命名空间视为，将物理网络接口分割小块之后的独立部分。<br />每个部分都可以单独配置，并拥有自己的网络规则和资源。<br />这些包括防火墙规则、接口（虚拟的或物理的）、路由以及与网络相关的所有内容。

1. 物理网络接口持有根网络命名空间。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049543-46b31721-7ade-4f9f-a94d-01a35f3b3e9e.svg#clientId=u0310486e-436b-4&from=paste&height=291&id=udc99b0db&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u10be4daa-fb02-4ab8-8108-81dfac26a6f&title=&width=388)

2. 可以使用 Linux 网络命名空间来创建独立的网络。每个网络都是独立的，除非进行配置，默认不会与其他网络互通。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049612-e42c92c7-d276-4d9c-8884-0e95a85f3e63.svg#clientId=u0310486e-436b-4&from=paste&height=291&id=u8e2c443a&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u94abda90-2888-421b-acf9-6a7e677b1b3&title=&width=388)<br />但最终，还是需要物理接口处理所有真实的数据包，所有虚拟接口都是基于物理接口创建的。<br />网络命名空间可以通过 ip-netns 进行管理，使用 `ip netns list` 可以列出主机上的命名空间。<br />需要注意的是，创建的网络命名空间会出现在 /var/run/netns 下面，但 Docker 并没有遵循这一规则。<br />例如，这是 Kubernetes 节点的一些命名空间：
```bash
$ ip netns list

cni-0f226515-e28b-df13-9f16-dd79456825ac (id: 3)
cni-4e4dfaac-89a6-2034-6098-dd8b2ee51dcd (id: 4)
cni-7e94f0cc-9ee8-6a46-178a-55c73ce58f2e (id: 2)
cni-7619c818-5b66-5d45-91c1-1c516f559291 (id: 1)
cni-3004ec2c-9ac2-2928-b556-82c7fb37a4d8 (id: 0)
```
注意 cni- 前缀；这意味着命名空间是由 CNI 插件创建的。<br />当创建一个 Pod，Pod 被分配给一个节点后，CNI 将：

1. 分配 IP 地址。
2. 将容器连接到网络。

如果 Pod 包含多个容器，那么这些容器都将被放在同一个命名空间中。

1. 当创建 Pod 时，容器运行时会给容器创建一个网络命名空间。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049610-90fa728b-b12f-4145-83a8-d0aa4a394f34.svg#clientId=u0310486e-436b-4&from=paste&height=300&id=u8bdfacfd&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc5d3f5f5-848b-470a-95d4-bbfa1d61012&title=&width=400)<br />  2.然后 CNI 负责给 Pod 分配一个 IP 地址。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049566-914b2f7a-b85c-47c7-9f05-c435d13b4b15.svg#clientId=u0310486e-436b-4&from=paste&height=301&id=u97502955&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u11102714-fc9a-49df-b024-e25f4501499&title=&width=401)<br />  3. 最后 CNI 将容器连接到网络的其余部分。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049555-e83d15b3-5346-43c9-a975-bc191f334c07.svg#clientId=u0310486e-436b-4&from=paste&height=301&id=ue9b860a4&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3fbbd354-30c4-4e7d-b74f-70b69a4d037&title=&width=402)<br />那么，当列出节点上的容器的命名空间会发生什么呢？<br />可以通过 SSH 连接到 Kubernetes 节点并查看命名空间：
```bash
$ lsns -t net

        NS TYPE NPROCS   PID USER     NETNSID NSFS                           COMMAND
4026531992 net     171     1 root  unassigned /run/docker/netns/default      /sbin/init noembed norestore
4026532286 net       2  4808 65535          0 /run/docker/netns/56c020051c3b /pause
4026532414 net       5  5489 65535          1 /run/docker/netns/7db647b9b187 /pause
```
lsns 是一个用于列出主机上所有可用命名空间的命令。<br />请记住，Linux 中有多种命名空间类型。<br />Nginx 容器在哪里？<br />那些 pause 容器是什么？
<a name="FjQlP"></a>
## **在 Pod 中，pause 容器创建了网络命名空间**
先列出节点上的所有命名空间，看看能否找到 Nginx 容器：
```bash
$ lsns
        NS TYPE   NPROCS   PID USER            COMMAND
# truncated output
4026532414 net         5  5489 65535           /pause
4026532513 mnt         1  5599 root            sleep 1d
4026532514 uts         1  5599 root            sleep 1d
4026532515 pid         1  5599 root            sleep 1d
4026532516 mnt         3  5777 root            nginx: master process nginx -g daemon off;
4026532517 uts         3  5777 root            nginx: master process nginx -g daemon off;
4026532518 pid         3  5777 root            nginx: master process nginx -g daemon off;
```
Nginx 容器在挂载 (mnt)、Unix time-sharing (uts) 和 PID (pid) 命名空间中，但不在网络命名空间 (net) 中。<br />不幸的是，lsns 只显示每个进程最小的 PID，可以根据这个进程 ID 进一步过滤。<br />使用以下命令，在所有命名空间中检索 Nginx 容器：
```bash
$ sudo lsns -p 5777

       NS TYPE   NPROCS   PID USER  COMMAND
4026531835 cgroup    178     1 root  /sbin/init noembed norestore
4026531837 user      178     1 root  /sbin/init noembed norestore
4026532411 ipc         5  5489 65535 /pause
4026532414 net         5  5489 65535 /pause
4026532516 mnt         3  5777 root  nginx: master process nginx -g daemon off;
4026532517 uts         3  5777 root  nginx: master process nginx -g daemon off;
4026532518 pid         3  5777 root  nginx: master process nginx -g daemon off;
```
pause 进程再次出现，它劫持了网络命名空间。<br />这是怎么回事？<br />_**集群中的每个 Pod 都有一个额外的隐藏容器在后台运行，称为 pause 容器。**_<br />列出在节点上运行的容器并获取 pause 容器：
```bash
$ docker ps | grep pause

fa9666c1d9c6   k8s.gcr.io/pause:3.4.1  "/pause"  k8s_POD_kube-dns-599484b884-sv2js…
44218e010aeb   k8s.gcr.io/pause:3.4.1  "/pause"  k8s_POD_blackbox-exporter-55c457d…
5fb4b5942c66   k8s.gcr.io/pause:3.4.1  "/pause"  k8s_POD_kube-dns-599484b884-cq99x…
8007db79dcf2   k8s.gcr.io/pause:3.4.1  "/pause"  k8s_POD_konnectivity-agent-84f87c…
```
可以看到，节点上的每一个 Pod 都会有一个对应的 pause 容器。<br />这个 pause 容器负责创建和维持网络命名空间。<br />底层容器运行时会完成网络命名空间的创建，通常是由 containerd 或 CRI-O 完成。<br />在部署 Pod 和创建容器之前，由运行时创建网络命名空间。<br />容器运行时会自动完成这些，不需要手工执行 `ip netns` 创建命名空间。<br />话题回到 pause 容器。<br />它包含非常少的代码，并且在部署后立即进入睡眠状态。<br />但是，它是必不可少的，并且在 Kubernetes 生态系统中起着至关重要的作用。

1. 创建 Pod 时，容器运行时会创建一个带有睡眠容器的网络命名空间。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049874-004f75de-55e4-4240-b087-99624d05a443.svg#clientId=u0310486e-436b-4&from=paste&height=311&id=u71c73fe1&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u96345174-8ad5-486e-be29-6b31e479f85&title=&width=415)<br />  2. Pod 中的其他容器都会加入由 pause 容器创建的网络名称空间。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049902-ef44c885-7148-43bb-a6d9-3643fda0d0e5.svg#clientId=u0310486e-436b-4&from=paste&height=311&id=u0ae61d97&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4f8c8842-45ab-459a-a73a-7cf2ae38646&title=&width=415)<br />  3. 此时，CNI 分配 IP 地址并将容器连接到网络。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049857-4780b7d6-b830-4a9f-a7f2-729a76d07d9e.svg#clientId=u0310486e-436b-4&from=paste&height=311&id=u9be242fc&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud39d7f19-53be-4a64-aafa-61dbdbfa68e&title=&width=415)<br />一个进入睡眠状态的容器有什么用？<br />为了理解它的用途，想象一个 Pod 有两个容器，就像前面的例子一样，但没有 pause 容器。<br />一旦容器启动，CNI 将会：

1. 使 busybox 容器加入之前的网络命名空间。
2. 分配 IP 地址。
3. 将容器连接到网络。

如果 Nginx 崩溃了怎么办？<br />CNI 将不得不再次执行所有步骤，并且两个容器的网络都将中断。<br />由于睡眠容器不太可能有任何错误，因此创建网络命名空间通常是一种更安全、更健壮的选择。<br />如果 Pod 中的一个容器崩溃了，剩下的仍然可以回复其他网络请求。
<a name="PqFdL"></a>
## **分配一个 IP 地址给 Pod**
前面提到 Pod 和两个容器将具有同一个 IP 地址。<br />那是怎样配置的呢？<br />在 Pod 网络命名空间内，创建了一个接口，并分配了一个 IP 地址。<br />验证一下。<br />首先，找到 Pod 的 IP 地址：
```bash
$ kubectl get Pod multi-container-Pod -o jsonpath={.status.PodIP}

10.244.4.40
```
接下来，找到相关的网络命名空间。<br />由于网络命名空间是从物理接口创建的，需要先访问集群节点。<br />如果运行的是 minikube，使用 minikube ssh 访问节点。如果在云厂中运行，那么应该有某种方法可以通过 SSH 访问节点。<br />进入后，找到最新创建的命名网络命名空间：
```bash
$ ls -lt /var/run/netns

total 0
-r--r--r-- 1 root root 0 Sep 25 13:34 cni-0f226515-e28b-df13-9f16-dd79456825ac
-r--r--r-- 1 root root 0 Sep 24 09:39 cni-4e4dfaac-89a6-2034-6098-dd8b2ee51dcd
-r--r--r-- 1 root root 0 Sep 24 09:39 cni-7e94f0cc-9ee8-6a46-178a-55c73ce58f2e
-r--r--r-- 1 root root 0 Sep 24 09:39 cni-7619c818-5b66-5d45-91c1-1c516f559291
-r--r--r-- 1 root root 0 Sep 24 09:39 cni-3004ec2c-9ac2-2928-b556-82c7fb37a4d8
```
在示例中，就是 cni-0f226515-e28b-df13-9f16-dd79456825ac。然后，可以在该命名空间内运行 exec 命令：
```bash
$ ip netns exec cni-0f226515-e28b-df13-9f16-dd79456825ac ip a

# output truncated
3: eth0@if12: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1450 qdisc noqueue state UP group default
    link/ether 16:a4:f8:4f:56:77 brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 10.244.4.40/32 brd 10.244.4.40 scope global eth0
       valid_lft forever preferred_lft forever
    inet6 fe80::14a4:f8ff:fe4f:5677/64 scope link
       valid_lft forever preferred_lft forever
```
这个 IP 就是 Pod 的 IP 地址！通过查找 @if12 中的 12 找到网络接口
```bash
$ ip link | grep -A1 ^12

12: vethweplb3f36a0@if16: mtu 1376 qdisc noqueue master weave state UP mode DEFAULT group default
    link/ether 72:1c:73:d9:d9:f6 brd ff:ff:ff:ff:ff:ff link-netnsid 1
```
还可以验证 Nginx 容器是否监听了来自该命名空间内的 HTTP 流量：
```bash
$ ip netns exec cni-0f226515-e28b-df13-9f16-dd79456825ac netstat -lnp

Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN      692698/nginx: master
tcp6       0      0 :::80                   :::*                    LISTEN      692698/nginx: master
```
如果无法通过 SSH 访问集群中的工作节点，可以使用 `kubectl exec` 获取到 busybox 容器的 shell 并直接在内部使用 ip 和 netstat 命令。<br />刚刚介绍了容器之间的通信，再来看看如何建立 Pod 到 Pod 的通信吧。
<a name="r8jrH"></a>
## **查看集群中 Pod 到 Pod 的流量**
Pod 到 Pod 的通信有两种可能的情况：

1. Pod 流量的目的地是同一节点上的 Pod。
2. Pod 流量的目的地是在不同节点上的 Pod。

整个工作流依赖于虚拟接口对和网桥，下面先来了解一下这部分的内容。<br />为了让一个 Pod 与其他 Pod 通信，它必须先访问节点的根命名空间。<br />通过虚拟以太网对来实现 Pod 和根命名空间的连接。<br />这些虚拟接口设备（veth 中的 v）连接并充当两个命名空间之间的隧道。<br />使用此 veth 设备，将一端连接到 Pod 的命名空间，另一端连接到根命名空间。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049858-a6dcc913-878b-4ef3-aa2c-133ed898a7db.svg#clientId=u0310486e-436b-4&from=paste&height=324&id=u68d7e212&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uad4fbe33-d3e5-4239-a30f-54a9b21f835&title=&width=432)<br />CNI 可以执行这些操作，也可以手动执行：
```bash
$ ip link add veth1 netns Pod-namespace type veth peer veth2 netns root
```
现在 Pod 的命名空间有一个可以访问根命名空间的 隧道。<br />节点上，新建的每一个 Pod 都会设置这样的 veth 对。<br />一个是，创建接口对；另一个是为以太网设备分配地址并配置默认路由。<br />下面看看如何在 Pod 的命名空间中设置 veth1 接口：
```bash
$ ip netns exec cni-0f226515-e28b-df13-9f16-dd79456825ac ip addr add 10.244.4.40/24 dev veth1
$ ip netns exec cni-0f226515-e28b-df13-9f16-dd79456825ac ip link set veth1 up
$ ip netns exec cni-0f226515-e28b-df13-9f16-dd79456825ac ip route add default via 10.244.4.40
```
在节点上，创建另一个 veth2 对：
```bash
$ ip addr add 169.254.132.141/16 dev veth2
$ ip link set veth2 up
```
可以像前面一样检查现有的 veth 对。<br />在 Pod 的命名空间中，检索 eth0 接口的后缀。
```bash
$ ip netns exec cni-0f226515-e28b-df13-9f16-dd79456825ac ip link show type veth

3: eth0@if12: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1450 qdisc noqueue state UP mode DEFAULT group default
    link/ether 16:a4:f8:4f:56:77 brd ff:ff:ff:ff:ff:ff link-netnsid 0
```
在这种情况下，可以使用命令 grep -A1 ^12 查找（或滚动到目标所在处）：
```bash
$ ip link show type veth

# output truncated
12: cali97e50e215bd@if3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1450 qdisc noqueue state UP mode DEFAULT group default
    link/ether ee:ee:ee:ee:ee:ee brd ff:ff:ff:ff:ff:ff link-netns cni-0f226515-e28b-df13-9f16-dd79456825ac
```
也可以使用 ip -n cni-0f226515-e28b-df13-9f16-dd79456825ac link show type veth.命令<br />注意 3: eth0@if12和12: cali97e50e215bd@if3 接口上的符号。<br />从 Pod 命名空间，该 eth0 接口连接到根命名空间的 12 号接口，因此是 @if12.<br />在 veth 对的另一端，根命名空间连接到 Pod 命名空间的 3 号接口。<br />接下来是连接 veth 对两端的桥接器。
<a name="vf184"></a>
## **Pod 网络命名空间连接到以太网桥**
网桥会汇聚位于根命名空间中的每一个虚拟接口。这个网桥允许虚拟 pair 之间的流量，也允许穿过公共根命名空间的流量。<br />补充一下相关原理。<br />以太网桥位于 OSI 网络模型 的第 2 层。<br />可以将网桥视为接受来自不同命名空间和接口的连接的虚拟交换机。<br />以太网桥可以连接节点上的多个可用网络。<br />因此，可以使用网桥连接两个接口，即 Pod 命名空间的 veth 连接到同一节点上另一个 Pod 的 veth。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453049926-873d4df0-b638-4fb5-832d-722fb37af99c.svg#clientId=u0310486e-436b-4&from=paste&height=344&id=uc297dbf1&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua41918fb-13e8-48d4-bff1-bbd67e72c10&title=&width=459)<br />接下来，继续看网桥和 veth 对的用途。
<a name="Vjvp2"></a>
## **跟踪在同一节点上 Pod 到 Pod 的流量**
假设同一个节点上有两个 Pod，Pod-A 向 Pod-B 发送消息。

1. 由于访问目标不在同一个命名空间，Pod-A 将数据包发送到其默认接口 eth0。这个接口与 veth 对的一端绑定，作为隧道。这样，数据包会被转发到节点上的根命名空间。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050189-48dac777-085b-4f43-b93d-eddd3bd8c12c.svg#clientId=u0310486e-436b-4&from=paste&height=307&id=u4e6966b3&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucc6c8ab5-05f8-4727-b4f8-9321aaad324&title=&width=409)<br />  2. 以太网网桥作为一个虚拟交换机，需要目标 Pod-B 的 MAC 地址才能工作。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050209-c1343bb3-a44b-4ea2-bd93-4c05c3244d10.svg#clientId=u0310486e-436b-4&from=paste&height=308&id=u6c52f7c9&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u861d9584-e9b3-426f-a974-6fde0b087bf&title=&width=410)<br />  3. ARP 协议会解决这个问题。当帧到达网桥时，会向所有连接的设备发送 ARP 广播。网桥广播询问持有 Pod-B 的 IP 地址<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050161-452f0629-3b68-4510-9041-f536a35c6260.svg#clientId=u0310486e-436b-4&from=paste&height=308&id=ue56438c7&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5ef6cb69-3710-47b8-8bdf-fefb4c8344b&title=&width=411)<br />  4. 此时会收到一个带有 Pod-B IP 的 MAC 地址应答，这条消息会被存储在桥接 ARP 缓存(查找表)中。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050251-dd71b24c-75b2-4361-bf5c-bfff9a933278.svg#clientId=u0310486e-436b-4&from=paste&height=310&id=u54dbce79&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub5ffe8a3-3f8d-45c1-940e-75defd04c86&title=&width=413)<br />  5. IP 地址和 MAC 地址的映射关系存储之后，网桥就在表中查找，并将数据包转发到正确的端点。数据包到达根命名空间内 Pod-B 的 veth 之后，很快又到达 Pod-B 命名空间内的 eth0 接口。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050287-d8e10fee-b8f5-4e2c-9664-c571898ba5f4.svg#clientId=u0310486e-436b-4&from=paste&height=311&id=uf848c07a&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u473e9af6-25fd-4f63-98e7-4286246af82&title=&width=414)<br />至此，Pod-A 和 Pod-B 之间的通信就成功了。
<a name="GGCzW"></a>
## **跟踪不同节点上的 Pod 到 Pod 通信**
对于跨节点 Pod 之间的通信，会经过额外的通信跳跃。

1. 前几个步骤保持不变，直到数据包到达根命名空间并需要发送到 Pod-B。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050498-2e0cadf7-877f-4f5a-a059-25c7aa25d720.svg#clientId=u0310486e-436b-4&from=paste&height=308&id=u204923ca&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueef3afe7-9f78-41c4-a6f6-fa124740e8f&title=&width=411)<br />  2. 当目的 IP 不在本地网络中时，报文被转发到节点的默认网关。节点的出口网关或默认网关，通常位于节点与网络相连的物理接口 eth0 上。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050460-71aa7d97-6e2d-4ab6-be35-3bc2bfedbbeb.svg#clientId=u0310486e-436b-4&from=paste&height=311&id=ua1925e1d&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u79c7aa06-9231-4705-95cc-825b2d78cd0&title=&width=414)<br />此时 不会发生 ARP 解析，因为源 IP 和目标 IP 不在同一个网段中。<br />网段的检查是使用按位运算完成的。<br />当目的 IP 不在当前网络段时，数据包被转发到节点的默认网关。
<a name="tkm0z"></a>
## **按位运算的工作原理**
在确定数据包的转发位置时，源节点必须执行位运算<br />这也称为与操作。<br />复习一下，按位与运算的规则：
```bash
0 AND 0 = 0
0 AND 1 = 0
1 AND 0 = 0
1 AND 1 = 1
```
除了 1 与 1 以外的都是 false。<br />如果源节点的 IP 为 192.168.1.1，子网掩码为 /24，目标 IP 为 172.16.1.1/16，则按位与运算将得知它们位于不同的网段上。<br />这意味着目标 IP 与数据包的源不在同一个网络上，数据包将通过默认网关转发。<br />数学时间。<br />必须从二进制的 32 位地址开始进行 AND 操作。<br />先找出源 IP 网络和目标 IP 网段。

| Type | Binary | Converted |
| --- | --- | --- |
| Src. IP Address | 11000000.10101000.00000001.00000001 | 192.168.1.1 |
| Src. Subnet Mask | 11111111.11111111.11111111.00000000 | 255.255.255.0(/24) |
| Src. Network | 11000000.10101000.00000001.00000000 | 192.168.1.0 |
| <br /> | <br /> | <br /> |
| Dst. IP Address | 10101100.00010000.00000001.00000001 | 172.16.1.1 |
| Dst. Subnet Mask | 11111111.11111111.00000000.00000000 | 255.255.0.0(/16) |
| Dst. Network | 10101100.00010000.00000000.00000000 | 172.16.0.0 |

按位运算之后，需要将目标 IP 与数据包源节点的子网进行比较。

| Type | Binary | Converted |
| --- | --- | --- |
| Dst. IP Address | 10101100.00010000.00000001.00000001 | 172.16.1.1 |
| Src. Subnet Mask | 11111111.11111111.11111111.00000000 | 255.255.255.0(/24) |
| Network Result | 10101100.00010000.00000001.00000000 | 172.16.1.0 |

运算的结果是 172.16.1.0，不等于 192.168.1.0（源节点的网络）。说明源 IP 地址和目标 IP 地址不在同一个网络上。<br />如果目标 IP 是 192.168.1.2，即与发送 IP 在同一子网中，则 AND 操作将得到节点的本地网络。

| Type | Binary | Converted |
| --- | --- | --- |
| Dst. IP Address | 11000000.10101000.00000001.00000010 | 192.168.1.2 |
| Src. Subnet Mask | 11111111.11111111.11111111.00000000 | 255.255.255.0(/24) |
| Network | 11000000.10101000.00000001.00000000 | 192.168.1.0 |

进行逐位比较后，ARP 通过查找表查找默认网关的 MAC 地址。<br />如果有条目，将立即转发数据包。<br />否则，先进行广播以找到网关的 MAC 地址。

1. 现在，数据包路由到另一个节点的默认接口，称为 Node-B。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050515-02889a99-97ca-4c31-a0fe-0f6c8560d6b2.svg#clientId=u0310486e-436b-4&from=paste&height=305&id=ub4c03d2c&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u23eca1cc-8863-4903-b1d8-36d66ac9dc0&title=&width=407)

1. 以相反的顺序。现在，数据包位于 Node-B 的根命名空间，并到达网桥，这里会进行 ARP 解析。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050608-be638212-e743-49ca-97df-31df0b7ec77f.svg#clientId=u0310486e-436b-4&from=paste&height=307&id=u87ca4659&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u97e4c498-8b2e-46fb-90b5-c463fb96455&title=&width=409)

1. 路由系统将返回与 Pod-B 相连的接口的 MAC 地址。

![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050607-563dcf9c-3baf-4d20-9113-be0612d6ac96.svg#clientId=u0310486e-436b-4&from=paste&height=306&id=u240401e2&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u92cffc0e-7bcb-4d20-bc33-52d049d8d3e&title=&width=408)<br />  4. 网桥通过 Pod-B 的 veth 设备转发帧，并到达 Pod-B 的命名空间。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050784-4a0455ea-44c3-4cc4-82bb-dd1a022919cd.svg#clientId=u0310486e-436b-4&from=paste&height=319&id=uecf68f58&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud30890f8-8615-43d6-aa8c-8d8c2c245ce&title=&width=425)<br />至此，应该已经熟悉了 Pod 之间的流量是如何流转的。下面花点时间来看看 CNI 如何管理上诉内容。
<a name="KSz6q"></a>
## **容器网络接口 - CNI**
容器网络接口（CNI）主要关注的是当前节点中的网络。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050724-977b84c6-e7a4-4c37-9cf9-744c69e72f60.svg#clientId=u0310486e-436b-4&from=paste&height=351&id=u7c05bd5d&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c6cf34d-0519-4da2-a1be-5611734a27f&title=&width=468)<br />可以将 CNI 看作为解决 Kubernetes 网络需求，而遵循的一组规则。<br />有这些 CNI 实现可供使用：

- Calico
- Cillium
- Flannel
- Weave Net
- 其他网络插件

他们都遵循相同的 CNI 标准。<br />如果没有 CNI，需要人工完成如下操作：

- 创建接口。
- 创建 veth 对。
- 设置网络命名空间。
- 设置静态路由。
- 配置以太网桥。
- 分配 IP 地址。
- 创建 NAT 规则。
- 还有其他大量事情。

这还不包括，在删除或重启 Pod 时，需要进行类似的全部操作。<br />CNI 必须支持四种不同的操作：

- ADD - 向网络添加一个容器。
- DEL - 从网络中删除一个容器。
- CHECK - 如果容器的网络出现问题，则返回错误。
- VERSION - 显示插件的版本。

一起看下，CNI 是如何工作的。<br />当 Pod 被分配到特定节点时，Kubelet 自身不会初始化网络。<br />相反，Kubelet 将这个任务交给 CNI。<br />_但是，Kubelet 以 JSON 格式指定配置并发送至 CNI 插件。_<br />可以进入节点上的 /etc/cni/net.d 文件夹，使用以下命令查看当前的 CNI 配置文件：
```bash
$ cat 10-calico.conflist

{
  "name": "k8s-Pod-network",
  "cniVersion": "0.3.1",
  "plugins": [
    {
      "type": "calico",
      "datastore_type": "kubernetes",
      "mtu": 0,
      "nodename_file_optional": false,
      "log_level": "Info",
      "log_file_path": "/var/log/calico/cni/cni.log",
      "ipam": { "type": "calico-ipam", "assign_ipv4" : "true", "assign_ipv6" : "false"},
      "container_settings": {
          "allow_ip_forwarding": false
      },
      "policy": {
          "type": "k8s"
      },
      "kubernetes": {
          "k8s_api_root":"https://10.96.0.1:443",
          "kubeconfig": "/etc/cni/net.d/calico-kubeconfig"
      }
    },
    {
      "type": "bandwidth",
      "capabilities": {"bandwidth": true}
    },
    {"type": "portmap", "snat": true, "capabilities": {"portMappings": true}}
  ]
}
```
每个 CNI 插件都会使用不同类型的网络配置。<br />例如，Calico 使用基于 BGP 的三层网络连接 Pod<br />Cilium 从三层到七层使用的是基于 eBPF 的 overlay 网络<br />与 Calico 一样，Cilium 也支持通过配置网络策略来限制流量。<br />那么应该使用哪一个呢？主要有两类 CNI。<br />在第一类中，使用基本网络设置（也称为平面网络），从集群的 IP 池为 Pod 分配 IP 地址的 CNI。<br />这种方式可能很快耗尽 IP 地址，而成为负担。<br />相反，另一类是使用 overlay 网络。<br />简单来说，overlay 网络是主（底层）网络之上的重建网络。<br />overlay 网络通过封装来自底层网络的数据包工作，这些数据包被发送到另一个节点上的 Pod。<br />overlay 网络的一种流行技术是 VXLAN，它可以在 L3 网络上建立 L2 域的隧道。<br />那么哪个更好呢？<br />没有单一的答案，这取决于需求。<br />是否正在构建具有数万个节点的大型集群？<br />也许 overlay 网络更好。<br />是否在意更简单的配置和审查网络流量，而不会愿意在复杂网络中丢失这种能力？<br />扁平网络更适合。<br />现在讨论完了 CNI，接着来看看 Pod 到服务的通信是如何连接的。
<a name="ySppU"></a>
## **检查 Pod 到 Service 的流量**
由于 Pod 在 Kubernetes 中是动态的，分配给 Pod 的 IP 地址不是静态的。<br />Pod 的 IP 是短暂的，每次创建或删除 Pod 时都会发生变化。<br />Kubernetes 中的 Service 解决了这个问题，为连接一组 Pod 提供了可靠的机制。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050841-7e20399e-db70-4dc8-925d-970ab2b87d91.svg#clientId=u0310486e-436b-4&from=paste&height=379&id=u4bd64db8&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5c06a30b-a802-470c-bdd5-75e5f748d73&title=&width=505)<br />默认情况下，在 Kubernetes 中创建 Service 时，被分配一个虚拟 IP。<br />在 Service 中，可以使用选择器将 Service 与目标 Pod 相关联。<br />当删除或添加一个 Pod 时会发生什么呢？<br />Service 的虚拟 IP 保持静态不变。<br />但流量可以再无需干预的情况下，到达新创建的 Pod。<br />换句话说，Kubernetes 中的 Service 类似于负载均衡器。<br />但它们是如何工作的？
<a name="MpJe8"></a>
## **使用 Netfilter 和 Iptables 拦截和重写流量**
Kubernetes 中的 Service 是基于 Linux 内核中的两个组件构建的：

1. 网络过滤器
2. iptables

Netfilter 是一个可以配置数据包过滤、创建 NAT 、端口转发规则以及管理网络中流量的框架<br />此外，它可以屏蔽和禁止未经同意的访问。<br />另一方面，iptables 是一个用户态程序，可以用来配置 Linux 内核防火墙的 IP 数据包过滤规则。<br />iptables 是作为不同的 Netfilter 模块实现的。<br />可以使用 iptables CLI 即时修改过滤规则，并将它们插入 netfilters 挂载点。<br />过滤器配置在不同的表中，其中包含用于处理网络流量数据包的链。<br />不同的协议使用不同的内核模块和程序。<br />当提到 iptables 时，通常指的是 IPv4。对于 IPv6 ，终端工具是 ip6tables。<br />iptables 有五种链，每一种链都直接映射到 Netfilter 的钩子上。<br />从 iptables 的角度来看，它们是：

- PRE_ROUTING
- INPUT
- FORWARD
- OUTPUT
- POST_ROUTING

它们对应地映射到 Netfilter 钩子：

- NF_IP_PRE_ROUTING
- NF_IP_LOCAL_IN
- NF_IP_FORWARD
- NF_IP_LOCAL_OUT
- NF_IP_POST_ROUTING

当一个数据包到达时，根据它所处的阶段，将 “触发” 一个 Netfilter 钩子。这个钩子会执行特定的 iptables 过滤规则。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050926-ec2f32c9-e173-4cfb-9295-e0e9d26dd66f.svg#clientId=u0310486e-436b-4&from=paste&height=333&id=u2881cd50&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u04d6c68a-d71b-42bc-93b0-698fa9b5f54&title=&width=444)<br />哎呀！看起来很复杂！<br />不过没什么好担心的。<br />这就是使用 Kubernetes 的原因，以上所有内容都是通过使用 Service 抽象出来的，并且一个简单的 YAML 定义可以自动设置这些规则。<br />如果有兴趣查看 iptables 规则，可以连接到节点并运行：
```bash
$ iptables-save
```
还可以使用这个工具来可视化节点上的 iptables 链。<br />这是来自 GKE 节点上的可视化 iptables 链的示例图：<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050912-b72fbd82-5586-4255-a7f8-4462046ce80c.svg#clientId=u0310486e-436b-4&from=paste&height=322&id=u113a7cc6&originHeight=124&originWidth=300&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3dcdec86-8418-42d5-b392-ecc2a4ea759&title=&width=778)<br />注意，这里可能配置了几百条规则，想想一下自己动手怎么配置！<br />至此，已经了解了，相同节点上的 Pod 和不同节点上 Pod 之间是如何通信的。<br />在 Pod 与 Service 的通信中，链路的前半部分是一样的。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453050978-dbd5bc8f-12f5-4322-8c9d-41099f21808e.svg#clientId=u0310486e-436b-4&from=paste&height=336&id=ueb769894&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u21d091c6-ac64-44e0-94f0-e0851197b39&title=&width=448)<br />当请求从 Pod-A 走向 Pod-B 时，由于 Pod-B 在 Service 的 “后面”，在传输的过程中，会有一些不一样。<br />原始的请求，在 Pod-A 命名空间的 eth0 接口发出。<br />接着，请求通过 veth到达根名称空间的网桥。<br />一旦到达网桥，数据包就会立即通过默认网关转发。<br />与 Pod-to-Pod 部分一样，主机进行按位比较。由于服务的虚拟 IP 不是节点 CIDR 的一部分，因此数据包将立即通过默认网关转发。<br />如果默认网关的 MAC 地址尚未出现在查找表中，则会进行 ARP 解析找出默认网关的 MAC 地址。<br />现在神奇的事情发生了。<br />在数据包通过节点的路由之前，Netfilter 的 NF_IP_PRE_ROUTING 挂钩被触发，并执行 iptables 规则。这个规则会修改 Pod-A 数据包的目标 IP 地址 DNAT。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453051068-4e6b02e5-e575-4cc5-9c0b-27aec8e904fe.svg#clientId=u0310486e-436b-4&from=paste&height=335&id=ub594f23c&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub2f637c7-edcf-4614-bd68-5f1c9131b79&title=&width=447)<br />前面服务的虚拟 IP 地址被重写为 Pod-B 的 IP 地址。<br />接下来，数据包路由过程与 Pod 到 Pod 的通信一样。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453051234-28c2f712-5774-4b9c-b31b-4d24ad8a8a2f.svg#clientId=u0310486e-436b-4&from=paste&height=337&id=u0d354b7b&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucdf8232b-4aeb-4af0-be19-0bd8a600828&title=&width=449)<br />数据包重写后，通信是 Pod 到 Pod。<br />然而，在所有这些通信中，使用了一个第三方的功能。<br />此功能称为 conntrack 或链路跟踪。<br />当 Pod-B 发回响应时，conntrack 会将数据包与链路相关联，并跟踪其来源。<br />NAT 严重依赖于 conntrack。<br />如果没有链路跟踪，将不知道将包含响应的数据包发回何处。<br />使用 conntrack 时，数据包的返回路径很容易设置为相同的源或目标 NAT 更改。<br />通信的另一部分与现在的链路相反。<br />Pod-B 接收并处理了请求，现在将数据发送回 Pod-A。<br />现在会发生什么呢？
<a name="Twlkb"></a>
## 检查来自服务的响应
Pod-B 发送响应，将其 IP 地址设置为源地址，并将 Pod-A 的 IP 地址设置为目标地址。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453051277-1e8b66b7-87cd-4957-b05b-356fa3f77001.svg#clientId=u0310486e-436b-4&from=paste&height=293&id=u8aac8b33&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u06877baa-6460-4e53-a755-10744318077&title=&width=391)<br />当数据包到达 Pod-A 所在节点的接口时，会发生另一个 NAT。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453051270-b110aadc-05c9-453e-90e0-95bc3b862012.svg#clientId=u0310486e-436b-4&from=paste&height=298&id=uf5a03d03&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5fa8c2bc-4e2c-455f-8382-d4c030988a6&title=&width=397)<br />这时，conntrack 开始工作，修改源 IP 地址，iptables 规则执行 SNAT，并将 Pod-B 的源 IP 地址修改为原始服务的虚拟 IP。<br />![](https://cdn.nlark.com/yuque/0/2022/svg/396745/1666453051300-7e2a3b90-4ec6-4f0f-8685-6217bc431243.svg#clientId=u0310486e-436b-4&from=paste&height=301&id=ueeeda438&originHeight=150&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u14588050-91ef-4304-9ed6-64fb3ea083b&title=&width=401)<br />对于 Pod-A 来说，响应是来自于 Service 而不是 Pod-B。<br />其余的都是一样的。一旦 SNAT 完成，数据包就会到达根命名空间中的网桥，并通过 veth 对转发到 Pod-A。
<a name="RegM7"></a>
## 总 结
一起回顾下本文相关要点

- 容器如何在本地或 Pod 内通信。
- 在相同节点和不同节点上的 Pod 如何通信。
- Pod-to-Service - Pod 如何将流量发送到 Kubernetes 中服务后面的 Pod 时。
- 什么是命名空间、veth、iptables、chains、conntrack、Netfilter、CNI、overlay 网络，以及 Kubernetes 网络工具箱中所需的一切。

原文链接：[https://learnk8s.io/kubernetes-network-packets](https://learnk8s.io/kubernetes-network-packets)
