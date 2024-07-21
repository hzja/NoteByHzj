Kubernetes
<a name="AVK5l"></a>
## Overview
本文将探讨 Kubernetes 中的网络模型，以及对各种网络模型进行分析。
<a name="WCU0G"></a>
## Underlay Network Model
<a name="HAQ9J"></a>
### 什么是 Underlay Network
底层网络 Underlay Network 顾名思义是指网络设备基础设施，如交换机，路由器，DWDM 使用网络介质将其链接成的物理网络拓扑，负责网络之间的数据包传输。<br />![Underlay network topology](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506211-9146b212-8e46-4317-b260-62ebe70396c5.png#averageHue=%23fbfaf8&clientId=u1783b5c5-5885-4&from=paste&id=u6cca9720&originHeight=185&originWidth=453&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ud9e1d9e8-2c43-406f-9fa6-0385f59b592&title=Underlay%20network%20topology "Underlay network topology")<br />underlay network 可以是二层，也可以是三层；二层的典型例子是以太网 Ethernet，三层是的典型例子是互联网 Internet。<br />而工作于二层的技术是 vlan，工作在三层的技术是由 OSPF, BGP 等协议组成。
<a name="GCxGH"></a>
### k8s 中的 underlay network
在 kubernetes 中，underlay network 中比较典型的例子是通过将宿主机作为路由器设备，Pod 的网络则通过学习路由条目从而实现跨节点通讯。<br />![underlay network topology in kubernetes](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506210-704c0d35-ba24-4a56-a7fc-f72f27b3938b.png#averageHue=%23f6e7e3&clientId=u1783b5c5-5885-4&from=paste&id=uf20508fc&originHeight=494&originWidth=938&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u42f69077-ca82-4f70-896b-8ccea700a86&title=underlay%20network%20topology%20in%20kubernetes "underlay network topology in kubernetes")<br />这种模型下典型的有 flannel 的 host-gw 模式与 calico BGP 模式。
<a name="FlqAy"></a>
#### flannel host-gw
flannel host-gw 模式中每个 Node 需要在同一个二层网络中，并将 Node 作为一个路由器，跨节点通讯将通过路由表方式进行，这样方式下将网络模拟成一个underlay network。<br />![layer2 ethernet topology](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506202-fbb81544-d6de-4ac5-8591-87ca5431b8e6.png#averageHue=%23fefefb&clientId=u1783b5c5-5885-4&from=paste&id=u827b1feb&originHeight=224&originWidth=290&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u549f7045-7e2a-4dd6-9dab-0fba01ef6ee&title=layer2%20ethernet%20topology "layer2 ethernet topology")<br />Notes：因为是通过路由方式，集群的 cidr 至少要配置 16，因为这样可以保证，跨节点的 Node 作为一层网络，同节点的 Pod 作为一个网络。如果不是这种用情况，路由表处于相同的网络中，会存在网络不可达
<a name="ijsOk"></a>
#### Calico BGP
BGP（Border Gateway Protocol）是去中心化自治路由协议。它是通过维护 IP 路由表或前缀表来实现 AS （Autonomous System）之间的可访问性，属于向量路由协议。<br />![BGP network topology](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506161-3fdcb722-22ee-4d8b-93d0-15304c75a502.png#averageHue=%23eaeaea&clientId=u1783b5c5-5885-4&from=paste&id=uaff376a0&originHeight=203&originWidth=482&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u4ab802b6-e6d3-4a00-bfae-8c54a713e8d&title=BGP%20network%20topology "BGP network topology")<br />与 flannel 不同的是，Calico 提供了的 BGP 网络解决方案，在网络模型上，Calico 与 Flannel host-gw 是近似的，但在软件架构的实现上，flannel 使用 flanneld 进程来维护路由信息；而 Calico 是包含多个守护进程的，其中 Brid 进程是一个 BGP 客户端与路由反射器(Router Reflector)，BGP 客户端负责从 Felix 中获取路由并分发到其他 BGP Peer，而反射器在 BGP 中起了优化的作用。<br />在同一个 IBGP 中，BGP 客户端仅需要和一个 RR 相连，这样减少了AS内部维护的大量的 BGP 连接。通常情况下，RR 是真实的路由设备，而 Bird 作为 BGP 客户端工作。<br />![Calico Network Architecture](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506239-e8e06f6e-679c-4cb5-9573-747b6ab937ca.png#averageHue=%23dee8d9&clientId=u1783b5c5-5885-4&from=paste&id=u9d2dc734&originHeight=325&originWidth=624&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u2627f271-b60f-41eb-8b23-f1f9467c532&title=Calico%20Network%20Architecture "Calico Network Architecture")
<a name="iZhPp"></a>
### IPVLAN & MACVLAN
IPVLAN 和 MACVLAN 是一种网卡虚拟化技术，两者之间的区别为， IPVLAN 允许一个物理网卡拥有多个 IP 地址，并且所有的虚拟接口用同一个 MAC 地址；而 MACVLAN 则是相反的，其允许同一个网卡拥有多个 MAC 地址，而虚拟出的网卡可以没有 IP 地址。<br />因为是网卡虚拟化技术，而不是网络虚拟化技术，本质上来说属于 Overlay network，这种方式在虚拟化环境中与 Overlay network 相比最大的特点就是可以将 Pod 的网络拉平到 Node 网络同级，从而提供更高的性能、低延迟的网络接口。本质上来说其网络模型属于下图中第二个。<br />![Virtual networking modes: bridging, multiplexing and SR-IOV](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506515-eaa74efc-c013-4ddd-b81e-cddaebe7fde5.png#averageHue=%23ebebeb&clientId=u1783b5c5-5885-4&from=paste&id=ubdbf8c2b&originHeight=380&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u5b652894-662e-4589-98e5-1cb8c231b45&title=Virtual%20networking%20modes%3A%20bridging%2C%20multiplexing%20and%20SR-IOV "Virtual networking modes: bridging, multiplexing and SR-IOV")

- 虚拟网桥：创建一个虚拟网卡对(veth pair)，一头在容器内，一头在宿主机的 root namespaces 内。这样一来容器内发出的数据包可以通过网桥直接进入宿主机网络栈，而发往容器的数据包也可以经过网桥进入容器。
- 多路复用：使用一个中间网络设备，暴露多个虚拟网卡接口，容器网卡都可以介入这个中间设备，并通过 MAC/IP 地址来区分 packet 应该发往哪个容器设备。
- 硬件交换，为每个 Pod 分配一个虚拟网卡，这样一来，Pod 与 Pod 之间的连接关系就会变得非常清晰，因为近乎物理机之间的通信基础。如今大多数网卡都支持 SR-IOV 功能，该功能将单一的物理网卡虚拟成多个 VF 接口，每个 VF 接口都有单独的虚拟 PCIe 通道，这些虚拟的 PCIe 通道共用物理网卡的 PCIe 通道。

在 kubernetes 中 IPVLAN 这种网络模型下典型的 CNI 有，multus 与 danm。
<a name="BhLKO"></a>
#### multus
multus 是 intel 开源的 CNI 方案，是由传统的 cni 与 multus，并且提供了 SR-IOV CNI 插件使 K8s pod 能够连接到 SR-IOV VF 。这是使用了 IPVLAN/MACVLAN 的功能。<br />当创建新的 Pod 后，SR-IOV 插件开始工作。配置 VF 将被移动到新的 CNI 名称空间。该插件根据 CNI 配置文件中的 “name” 选项设置接口名称。最后将 VF 状态设置为 UP。<br />下图是一个 Multus 和 SR-IOV CNI 插件的网络环境，具有三个接口的 pod。

- eth0 是 flannel 网络插件，也是作为 Pod 的默认网络
- VF 是主机的物理端口 ens2f0 的实例化。这是英特尔 X710-DA4 上的一个端口。在 Pod 端的 VF 接口名称为 south0 。
- 这个 VF 使用了 DPDK 驱动程序，此 VF 是从主机的物理端口 ens2f1 实例化出的。这个是英特尔 ® X710-DA4 上另外一个端口。Pod 内的 VF 接口名称为 north0。该接口绑定到 DPDK 驱动程序 vfio-pci 。

![Mutus networking Architecture overlay and SR-IOV](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506559-76f1f1ad-5f21-4723-8e9c-929ba60f3cf2.png#averageHue=%23aabc88&clientId=u1783b5c5-5885-4&from=paste&id=u0f76355d&originHeight=550&originWidth=722&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ufa86c5bc-1ef9-41ee-a17b-e323c4f7aea&title=Mutus%20networking%20Architecture%20overlay%20and%20SR-IOV "Mutus networking Architecture overlay and SR-IOV")<br />**Notes：术语**

- NIC：network interface card，网卡
- SR-IOV：single root I/O virtualization，硬件实现的功能，允许各虚拟机间共享 PCIe 设备。
- VF：Virtual Function，基于 PF，与 PF 或者其他 VF 共享一个物理资源。
- PF：PCIe Physical Function，拥有完全控制 PCIe 资源的能力
- DPDK：Data Plane Development Kit

于此同时，也可以将主机接口直接移动到 Pod 的网络名称空间，当然这个接口是必须存在，并且不能是与默认网络使用同一个接口。这种情况下，在普通网卡的环境中，就直接将 Pod 网络与 Node 网络处于同一个平面内了。<br />![Mutus networking Architecture overlay and ipvlan](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506494-c36ddb8f-94bb-4220-8a3f-87f2da7dad43.png#averageHue=%23d1e5cf&clientId=u1783b5c5-5885-4&from=paste&id=uccc6584c&originHeight=401&originWidth=748&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ub8274818-d4dc-4c69-9758-04c1ec608a4&title=Mutus%20networking%20Architecture%20overlay%20and%20ipvlan "Mutus networking Architecture overlay and ipvlan")
<a name="d7jme"></a>
#### danm
DANM 是诺基亚开源的 CNI 项目，目的是将电信级网络引入 kubernetes 中，与 multus 相同的是，也提供了 SR-IOV/DPDK 的硬件技术，并且支持 IPVLAN.
<a name="rrgAe"></a>
## Overlay Network Model
<a name="dELCY"></a>
### 什么是 Overlay
叠加网络是使用网络虚拟化技术，在 underlay 网络上构建出的虚拟逻辑网络，而无需对物理网络架构进行更改。本质上来说，overlay network 使用的是一种或多种隧道协议 (tunneling)，通过将数据包封装，实现一个网络到另一个网络中的传输，具体来说隧道协议关注的是数据包（帧）。<br />![overlay network topology](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506600-4753bb39-ec93-4a34-9374-627b8aea24b0.png#averageHue=%23dcefd6&clientId=u1783b5c5-5885-4&from=paste&id=u9b121aac&originHeight=484&originWidth=850&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u6f36de75-1e40-45b7-b646-6c58f5fbf4b&title=overlay%20network%20topology "overlay network topology")
<a name="UcJXb"></a>
### 常见的网络隧道技术

- 通用路由封装 ( Generic Routing Encapsulation ) 用于将来自 IPv4/IPv6 的数据包封装为另一个协议的数据包中，通常工作与 L3 网络层中。
- VxLAN (Virtual Extensible LAN)，是一个简单的隧道协议，本质上是将 L2 的以太网帧封装为 L4 中 UDP 数据包的方法，使用 **4789** 作为默认端口。VxLAN 也是 VLAN 的扩展，对于 4096（ 位 VLAN ID） 扩展为 1600 万（ 位 VN·ID ）个逻辑网络。

这种工作在 overlay 模型下典型的有 flannel 与 calico 中的的 VxLAN, IPIP 模式。
<a name="bhoXG"></a>
### IPIP
IP in IP 也是一种隧道协议，与 VxLAN 类似的是，IPIP 的实现也是通过 Linux 内核功能进行的封装。IPIP 需要内核模块 ipip.ko 使用命令查看内核是否加载 IPIP 模块lsmod | grep ipip ；使用命令modprobe ipip 加载。<br />![A simple IPIP network workflow](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506754-7e420777-8589-4990-a035-78539470e4b7.png#averageHue=%23eeedec&clientId=u1783b5c5-5885-4&from=paste&id=ub5c33186&originHeight=355&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u2ceb95e7-5840-449d-a13d-16127b94db2&title=A%20simple%20IPIP%20network%20workflow "A simple IPIP network workflow")<br />Kubernetes 中 IPIP 与 VxLAN 类似，也是通过网络隧道技术实现的。与 VxLAN 差别就是，VxLAN 本质上是一个 UDP 包，而 IPIP 则是将包封装在本身的报文包上。<br />![IPIP in kubernetes](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506776-e194df8d-ba00-4971-b226-ea9164ac7107.png#averageHue=%23f2d9d2&clientId=u1783b5c5-5885-4&from=paste&id=u985bab4f&originHeight=539&originWidth=938&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u9a944297-3a51-4cf1-b592-ba469c78b33&title=IPIP%20in%20kubernetes "IPIP in kubernetes")<br />![IPIP packet with wireshark unpack](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374506958-85281636-ca47-469f-b3d1-738f3135583e.png#averageHue=%23f9f7f6&clientId=u1783b5c5-5885-4&from=paste&id=u335edc30&originHeight=572&originWidth=823&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u8edc8e90-e58e-494a-8db8-05c70743f79&title=IPIP%20packet%20with%20wireshark%20unpack "IPIP packet with wireshark unpack")<br />Notes：公有云可能不允许 IPIP 流量，例如 Azure
<a name="b3pwA"></a>
### VxLAN
kubernetes 中不管是 flannel 还是 calico VxLAN 的实现都是使用 Linux 内核功能进行的封装，Linux 对 vxlan 协议的支持时间并不久，2012 年 Stephen Hemminger 才把相关的工作合并到 kernel 中，并最终出现在 kernel 3.7.0 版本。为了稳定性和很多的功能，你可以会看到某些软件推荐在 3.9.0 或者 3.10.0 以后版本的 kernel 上使用 VxLAN。<br />![A simple VxLAN network topology](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374507065-5387a54b-74db-4f6d-ae8d-b3cae9be8ab5.png#averageHue=%23fcfcfc&clientId=u1783b5c5-5885-4&from=paste&id=u8d90feeb&originHeight=208&originWidth=576&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ub6878ef6-f1e0-4dec-abd6-97cb6c4393a&title=A%20simple%20VxLAN%20network%20topology "A simple VxLAN network topology")<br />在 kubernetes 中 vxlan 网络，例如 flannel，守护进程会根据 kubernetes 的 Node 而维护 VxLAN，名称为 flannel.1 这是 VNID，并维护这个网络的路由，当发生跨节点的流量时，本地会维护对端 VxLAN 设备的 MAC 地址，通过这个地址可以知道发送的目的端，这样就可以封包发送到对端，收到包的对端 VxLAN 设备 flannel.1 解包后得到真实的目的地址。<br />查看 Forwarding database 列表
```bash
$ bridge fdb
26:5e:87:90:91:fc dev flannel.1 dst 10.0.0.3 self permanent
```
![VxLAN in kubernetes](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374507080-c502158f-3806-4269-8dbe-0bf132cc3469.png#averageHue=%23f2d9d2&clientId=u1783b5c5-5885-4&from=paste&id=u3ec7878c&originHeight=539&originWidth=938&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=udd3dfb60-9fd8-43f7-ab75-a256a61927e&title=VxLAN%20in%20kubernetes "VxLAN in kubernetes")<br />![VxLAN packet with wireshark unpack](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374507162-1a93f10a-b1c5-4760-b5ba-58de52f0f9b4.png#averageHue=%23e8e5e2&clientId=u1783b5c5-5885-4&from=paste&id=u599b1ea9&originHeight=160&originWidth=834&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u824ad44b-438e-49d5-b87a-aea5b285e77&title=VxLAN%20packet%20with%20wireshark%20unpack "VxLAN packet with wireshark unpack")<br />Notes：VxLAN 使用的 4789 端口，wireshark 应该是根据端口进行分析协议的，而 flannel 在 linux 中默认端口是 8472，此时抓包仅能看到是一个 UDP 包。<br />通过上述的架构可以看出，隧道实际上是一个抽象的概念，并不是建立的真实的两端的隧道，而是通过将数据包封装成另一个数据包，通过物理设备传输后，经由相同的设备（网络隧道）进行解包实现网络的叠加。
<a name="EQKDD"></a>
### weave vxlan
weave 也是使用了 VxLAN 技术完成的包的封装，这个技术在 weave 中称之为 fastdp (fast data path)，与 calico 和 flannel 中用到的技术不同的，这里使用的是 Linux 内核中的 openvswitch datapath module，并且 weave 对网络流量进行了加密。<br />![weave fastdp network topology](https://cdn.nlark.com/yuque/0/2022/png/396745/1672374507244-92c76d76-6722-46ec-bbf8-cbf6cf18ffcd.png#averageHue=%23e7e4e9&clientId=u1783b5c5-5885-4&from=paste&id=u25692859&originHeight=454&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u3597ec3a-2e8e-49be-b5c0-b0caa522d27&title=weave%20fastdp%20network%20topology "weave fastdp network topology")<br />Notes：fastdp 工作在 Linux 内核版本 3.12 及更高版本，如果低于此版本的例如 CentOS7，weave 将工作在用户空间，weave 中称之为 sleeve mode
<a name="Ypjmo"></a>
## Reference
[https://github.com/flannel-io/flannel/blob/master/Documentation/backends.md#host-gw](https://github.com/flannel-io/flannel/blob/master/Documentation/backends.md#host-gw)<br />[https://projectcalico.docs.tigera.io/networking/bgp](https://projectcalico.docs.tigera.io/networking/bgp)<br />[https://www.weave.works/docs/net/latest/concepts/router-encapsulation/](https://www.weave.works/docs/net/latest/concepts/router-encapsulation/)<br />[https://github.com/k8snetworkplumbingwg/sriov-network-device-plugin](https://github.com/k8snetworkplumbingwg/sriov-network-device-plugin)<br />[https://github.com/nokia/danm](https://github.com/nokia/danm)
