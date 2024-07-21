网络 STP<br />![2021-08-27-19-56-27-293234.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630065634316-da0d80bb-2630-46bf-85f9-1edf51ffa8da.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u4805931c&originHeight=1251&originWidth=892&originalType=binary&ratio=1&size=80215&status=done&style=shadow&taskId=udf910a24-5fbd-4443-a881-969dc141753)<br />上帝视角
<a name="weow0"></a>
### 网络环路
现在的生活已经离不开网络，如果断网，会抱怨这什么破网络，影响刷抖音、打游戏；如果公司断网，那老板估计会骂娘，因为会影响到公司正常运转，直接造成经济损失。网络通信中，通常是以一条链路能够正常工作为前提，如果链路断开或节点故障，那么互联的设备就无法正常通信了，这类网络问题叫做**单点故障**。没有备份的链路或节点，出现故障会直接断网。<br />![2021-08-27-19-56-27-468224.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630065851226-6d63d902-bb2d-4942-b699-8c486a4f9d53.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u7f049ca3&originHeight=262&originWidth=702&originalType=binary&ratio=1&size=11145&status=done&style=shadow&taskId=uec93b77b-1264-4009-a3db-b75c2019e4c)<br />单点故障<br />如果要提供 7×24 小时不间断的服务，那就需要在网络中提前部署冗余。避免出现单点故障，合理的做法是在网络中的关键设备和关键链路添加**冗余**。在冗余的网络环境中，任意一条链路发生故障断开，都不会影响网络，直接使用其它链路继续转发数据，解决单点故障的隐患。<br />![2021-08-27-19-56-27-641223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630065851259-3a19b5d9-674f-48e2-9569-aac27f5e8abc.png#clientId=u80d3c12b-e9ff-4&from=ui&id=tYDwz&originHeight=442&originWidth=702&originalType=binary&ratio=1&size=30617&status=done&style=shadow&taskId=u08e80903-1bfb-4e6a-a728-b7d9b1af639)<br />冗余链路<br />但同时也带来了另外的网络问题。这种组网会构成**二层环路**，会引发广播风暴、重复帧、MAC 地址漂移等问题，严重时会占满链路带宽，或打爆设备 CPU ，导致设备无法正常工作，最终造成网络瘫痪。当然，在实际的网络中，不少二层环路是由于人为的错误操作导致的，比如接错了网线。<br />![2021-08-27-19-56-27-819228.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630065851278-2cc47646-64b6-4d7e-99e9-fa71887f7d05.png#clientId=u80d3c12b-e9ff-4&from=ui&id=ajPgS&originHeight=442&originWidth=702&originalType=binary&ratio=1&size=19631&status=done&style=shadow&taskId=u83351ff1-40b6-437e-a64b-8460b600190)<br />二层环路<br />**举个栗子**：<br />大刘的主机想要与小美的主机进行通信，现在只知道小美主机的 IP 地址，不知道 MAC 地址。有 IP 地址，就可以通过 ARP 协议来获取小美主机的 MAC 地址。来看看有冗余的网络中数据交换的过程：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630065612395-f5ed5494-307f-441b-b9ca-33e37a6bf4b8.webp#clientId=u80d3c12b-e9ff-4&from=paste&id=ua9ee68a2&originHeight=583&originWidth=765&originalType=url&ratio=1&status=done&style=shadow&taskId=uc7ef281b-c5ef-44cc-ac5e-92d54fe6fc9)<br />冗余网络

1. 大刘主机向交换机 A 发送 **ARP 广播帧**，来解析小美主机的 MAC 地址；

![2021-08-27-19-56-28-346223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066081979-c2132b6c-0047-4c60-b370-e6fcbc73ede9.png#clientId=u80d3c12b-e9ff-4&from=ui&id=ud21d7611&originHeight=432&originWidth=232&originalType=binary&ratio=1&size=13837&status=done&style=shadow&taskId=u8f844d82-2a75-4f23-8709-2868e99a140)<br />主机发送广播帧

1. 交换机 A 收到广播帧后，查看自己的 MAC 地址表，没找到相应的表项，就向所有端口（除接收端口之外）**泛洪**这个广播帧。也就是向 G0/1 和 G0/2 两个端口泛洪广播帧；

![2021-08-27-19-56-28-680224.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066081979-40e4045e-11c3-48c8-893e-69e60733f6d5.png#clientId=u80d3c12b-e9ff-4&from=ui&id=TFzLe&originHeight=405&originWidth=692&originalType=binary&ratio=1&size=30579&status=done&style=shadow&taskId=u78931f5f-1fd4-4e03-84a9-ef8cadc3612)<br />广播帧泛洪

1. 交换机 B 和交换机 C 收到广播帧后，没有对应 MAC 地址表项，也将广播帧所有端口（除接收端口之外）**泛洪**出去；

![2021-08-27-19-56-28-938221.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066081982-b0b9cc19-9cd9-46f9-be76-b9e6f9c52807.png#clientId=u80d3c12b-e9ff-4&from=ui&id=HYZR0&originHeight=639&originWidth=712&originalType=binary&ratio=1&size=41240&status=done&style=shadow&taskId=u8b9d7fa2-97a2-494f-97df-434fabfbc0d)<br />广播帧泛洪

1. 小美主机终于收到了大刘发送的 ARP 广播帧，发现是查询自己的 MAC 地址后，小美主机将会通过**单播帧**返回自己的 MAC 地址；

![2021-08-27-19-56-29-053223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066114729-7ad5fba7-72e5-469b-9698-0759a3f2a622.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u8a7fbe76&originHeight=452&originWidth=229&originalType=binary&ratio=1&size=13999&status=done&style=shadow&taskId=u250c7206-2e57-4a81-955a-11164299f97)<br />主机发送单播帧

1. 这个过程看似正常，大刘主机发送的 ARP 广播帧顺利到达小美主机，小美主机也进行了响应，但是网络中广播帧的传输还没有结束。在第 3 步中，交换机 C 也把 ARP 广播帧泛洪到交换机 B 。这时交换机 B 就收到了两个相同的 ARP 广播帧，分别来自交换机 A 和交换机 C ，收到的广播帧都会泛洪出去。那么，小美主机也会收到两个相同的 ARP 广播帧，也就是**重复帧**。出现这种现象说明网络中存在不合理的冗余链路；

![2021-08-27-19-56-29-234254.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066114728-65a3c91e-ec68-493d-912a-c9e129feb341.png#clientId=u80d3c12b-e9ff-4&from=ui&id=UXLJT&originHeight=452&originWidth=371&originalType=binary&ratio=1&size=22215&status=done&style=shadow&taskId=u30b7cc57-20b8-4930-9131-4b29c7196a9)<br />重复帧

1. 接下来看下交换机 C ，交换机 C 收到从交换机 A 发过来的广播帧，同时交换机 C 的 MAC 地址表添加一条表项，记录大刘主机 MAC 地址和端口 G0/0 的映射关系。交换机 C 又从交换机 B 收到相同的广播帧，大刘主机 MAC 地址的映射端口从 G0/0 变成 G0/1 。从不同的端口收到相同的数据帧，导致 MAC 地址表项发生变化的现象，就叫做 **MAC 地址漂移**，这种现象说明网络中可能存在环路。这样一来，交换机 C 就无法确定大刘主机到底位于自己的哪个端口；

![2021-08-27-19-56-29-433225.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066114736-93911176-ecf2-48b4-b779-ebd36857725e.png#clientId=u80d3c12b-e9ff-4&from=ui&id=cdd0b&originHeight=261&originWidth=821&originalType=binary&ratio=1&size=19357&status=done&style=shadow&taskId=uf0aecd1f-d077-46a1-bc2e-9b3a52b05b1)<br />MAC地址漂移

1. 主机收到广播帧，会进行解封装，查看上层的 IP 地址是否是发送给自己的，再进行下一步处理。交换机（只指二层交换机）收到广播帧，会直接进行泛洪。大刘主机发出的广播帧，经过交换机 A 后，从交换机 A 的 G0/1 口泛洪的广播帧，交换机 B 收到后再从 G0/2 口进行泛洪，交换机 C 收到广播帧后，又从 G0/0 口泛洪出去，结果广播帧回到了交换机 A ，交换机 A 再从 G0/1 进行泛洪，最终这个广播帧会一直逆时针、永无止境的进行泛洪；同理，交换机 A 从 G0/2 口进行泛洪的广播帧，也会按顺时针、无休止的在三台交换机上进行泛洪。这种广播帧不停泛洪的现象，叫做**广播风暴**。广播风暴不仅会大量消耗网络设备的带宽和 CPU 使用率，也会影响到主机。主机收到一个广播帧后，会解封装上送网络层去处理，大量的广播帧泛洪，很可能导致主机瘫痪。

![2021-08-27-19-56-29-694226.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066216940-79aefb87-ca0c-49ba-a013-2c0291bf258c.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u203f9cd8&originHeight=872&originWidth=692&originalType=binary&ratio=1&size=58640&status=done&style=shadow&taskId=u1da17904-248a-48c2-8067-b624cbff3c9)<br />广播风暴<br />通过这个简单的演示，可以看到了冗余链路带来的风险。重复帧、MAC 地址漂移和广播风暴，都是由**_一个广播帧引起_**的，可是网络中不可避免出现广播帧，也不能因为二层环路问题而忽略冗余链路增加网络可靠性的好处。<br />那么如何在保证网络冗余的情况下，消除二层环路呢？实际上交换机的二层环路是一个典型问题，解决方案也有不少。其中的一个解决方案就是 **STP**（生成树协议），能够阻断冗余链路来消除可能存在的环路，并且在网络故障时激活被阻断的冗余备份链路，恢复网络的连通性，保障业务的不间断服务。<br />![2021-08-27-19-56-29-989224.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066216918-e4ee9276-ef16-4b46-9fe1-190adbfc2c08.png#clientId=u80d3c12b-e9ff-4&from=ui&id=paRvQ&originHeight=633&originWidth=765&originalType=binary&ratio=1&size=29274&status=done&style=shadow&taskId=u85d5f804-5edd-4372-be5a-c13dd666be2)<br />STP解决环路问题<br />当网络中部署了 STP 后，交换机之间会交互相关协议报文，并计算出一个无环路的网络拓扑。当网络存在环路时，STP 会将网络中的一个或多个接口进行阻塞，将环路网络结构修剪成无环路的树状网络结构。被阻塞的接口不再转发数据，这样二层环路问题便迎刃而解。STP 会持续监控网络拓扑状况，当网络拓扑发生变化时，STP 能够及时感知，并动态调整被阻塞接口，而无需人工干预。<br />![2021-08-27-19-56-30-162260.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066216926-ae62efad-eb30-45d8-aba0-d84b762e6882.png#clientId=u80d3c12b-e9ff-4&from=ui&id=iXZCG&originHeight=640&originWidth=765&originalType=binary&ratio=1&size=37864&status=done&style=shadow&taskId=u56c46e72-9a27-470b-bb5e-cc5721bf551)<br />STP修复网络故障
<a name="Mf5Ma"></a>
### STP 基本概念
**STP** 是用在局域网中消除数据链路层物理环路的协议，标准名称是 **802.1D** 。STP 带来的好处有：

- **消除环路**：STP 可以通过阻塞冗余端口，确保网络无环且连通；
- **链路备份**：当使用的链路因故障断开时，可以检测到这种情况，并自动开启阻塞状态的冗余端口，网络迅速恢复正常。

在了解 STP 的原理之前，先来看看几个专业术语：
<a name="yk0td"></a>
#### 1、桥 ID（ Bridge ID ，BID ）
在公司内使用资产编号标记不同的设备，在 STP 里使用不同的桥 ID 标识不同的交换机。每一台运行 STP 的交换机都有一个唯一的**桥 ID** 。桥 ID 一共 8 字节，包含 2 字节的桥优先级和 6 字节的桥 MAC 地址。**桥优先级**的值可以人为设定，默认值是 32768 。**桥 MAC 地址**通常是交换机默认 VLAN1 的 MAC 地址。<br />![2021-08-27-19-56-30-329243.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066261994-92b11137-4849-426f-8f90-e322bc3193fd.png#clientId=u80d3c12b-e9ff-4&from=ui&id=uf6f6babc&originHeight=202&originWidth=532&originalType=binary&ratio=1&size=12755&status=done&style=shadow&taskId=uf7f13ed7-abb4-4746-9ec5-2fd0afa436d)<br />桥ID
<a name="pw0hl"></a>
#### 2、根桥（ Root Bridge ，RB ）
STP 要在整个二层网络中计算出一棵无环的 “ 树 ” ，树形成了，网络中的无环拓扑也就形成了。其中最重要的就是树根，树根明确了，“ 树枝 ” 才能沿着网络拓扑进行延伸。STP 的**根桥**就是这棵树的树根。当 STP 启动后，第一件事就是在网络中选举出根桥。在一个二层网络中，根桥只有一个，其余设备都是非根桥。当网络的拓扑发生变化时，根桥也可能会发生变化。<br />![2021-08-27-19-56-30-561225.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066261990-e5eb2bd5-2a76-4d78-bfcc-de3a35472a5c.png#clientId=u80d3c12b-e9ff-4&from=ui&id=yVObc&originHeight=361&originWidth=741&originalType=binary&ratio=1&size=16129&status=done&style=shadow&taskId=u7059eb92-7fdc-4938-9f4a-3e9c412e7a0)<br />根桥ID<br />网络中桥 ID 最小的交换机将成为根桥。在比较桥 ID 大小时，首先比较的是桥优先级，桥优先级的值最小的交换机选为根桥；如果桥优先级相同，那么会比较 MAC 地址，MAC 地址最小的交换机选为根桥。<br />![2021-08-27-19-56-30-729229.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066261995-ee8211bb-aa19-42ec-a101-1d73ffa0d89a.png#clientId=u80d3c12b-e9ff-4&from=ui&id=XfWRt&originHeight=672&originWidth=622&originalType=binary&ratio=1&size=30902&status=done&style=shadow&taskId=u3def2fab-3b9a-4290-b5c9-0a63ecea44c)<br />根桥交换机
<a name="oAiky"></a>
#### 3、根路径开销（ Root Path Cost ，RPC ）
STP 交换机每一个端口都对应一个**开销值**，这个值表示数据通过端口发送时的开销，这个值与端口带宽有关，带宽越高，开销值越小。对于端口开销值的定义有不同的标准，通常设备默认使用 **IEEE 802.1t** 中定义的开销值，同时还支持其它标准，以便兼容不同厂家的设备。<br />![2021-08-27-19-56-30-897229.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066303552-1c100f62-f512-4a46-9f8a-c01cd9bcfb56.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u766b162f&originHeight=282&originWidth=602&originalType=binary&ratio=1&size=23875&status=done&style=shadow&taskId=u30e98a48-f9f7-43cf-8b23-db92dd6d871)<br />Cost值标准对比表<br />非根桥到达根桥可能有多条路径，每条路径都有一个总开销值，也就是**根路径开销**（ RPC ），这个值是通过这条路径所有出端口的开销值累加而来的。STP 不会计算入端口的开销，只在数据通过端口发出时，才计算这个端口的开销。对于根桥来说，根路径开销是 0 。<br />![2021-08-27-19-56-31-133224.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066303603-b3bd0496-91d9-4023-aed6-8c4a641ef6f8.png#clientId=u80d3c12b-e9ff-4&from=ui&id=x6AC7&originHeight=589&originWidth=622&originalType=binary&ratio=1&size=28100&status=done&style=shadow&taskId=u71b56ee7-79c0-4f62-90da-2c50521af97)<br />计算根路径开销
<a name="b98Iw"></a>
#### 4、端口 ID（ Port ID ，PID ）
运行 STP 的交换机使用**端口 ID** 标识每个端口，端口 ID 主要用于选举指定端口。端口 ID 长度为 16 比特，其中前 4 比特是**端口优先级**，后 12 比特是**端口编号**。在进行比较时，先比较端口优先级，优先级小的端口优先；在优先级相同时，再比较端口编号，编号小的端口优先。通常情况下，端口编号无法改变，可通过设置端口优先级来影响生成树的选路。<br />![2021-08-27-19-56-31-295222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066303611-5dcb9819-8422-4ec3-863a-6e532586021d.png#clientId=u80d3c12b-e9ff-4&from=ui&id=Ln32s&originHeight=201&originWidth=531&originalType=binary&ratio=1&size=10461&status=done&style=shadow&taskId=u8655409b-1ca0-40fd-9504-7f48d74decb)<br />端口ID
<a name="rCIfY"></a>
#### 5、BPDU
STP 协议使用 **BPDU** 报文进行交互，BPDU 包含与 STP 协议相关的所有信息，并且使用这些信息来完成生成树的计算。BPDU 是组播帧，地址为 0180-c200-0000 ，并由 STP 交换机产生、发送、接收、处理，终端主机不参与。BPDU 分为两种类型：

- **配置 BPDU**（ Configuration BPDU ）：在 STP 的初始化过程中，每台交换机都会产生并发送配置 BPDU 。在 STP 树形成后的稳定期，只有根桥才会周期性地发送配置 BPDU ；相应的，非根桥会从自己的根端口收到配置 BPDU ，并更新自己的配置 BPDU ，再从指定端口发送出去。这个过程看起来像根桥发出的配置 BPDU 逐跳的经过了其它交换机。

![2021-08-27-19-56-31-590222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066341045-701ffbee-4cc5-4b27-af05-f6d0ad673066.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u49ac9736&originHeight=190&originWidth=752&originalType=binary&ratio=1&size=20709&status=done&style=shadow&taskId=u02bbef5d-412e-4114-bcd2-30387b745c7)<br />Configuration BPDU 格式<br />配置 BPDU 的参数详情如下：<br />![2021-08-27-19-56-31-728222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066341062-0389742d-7f89-44d2-a054-35477262291c.png#clientId=u80d3c12b-e9ff-4&from=ui&id=SPWJ6&originHeight=601&originWidth=701&originalType=binary&ratio=1&size=26855&status=done&style=shadow&taskId=uc0dfcb78-2e8e-4752-8c22-605667407a8)<br />Configuration BPDU 字段表

- **拓扑变化通知 BPDU**（ Topology Change Notification BPDU ）：简称 TCN BPDU ，是非根桥通过根端口向根桥发送的。当非根桥检测到拓扑变化后，就会生成一个描述拓扑变化的 TCN BPDU ，并从自己的根端口发送出去。

![2021-08-27-19-56-31-888224.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066341066-aa025c09-506e-4543-8a5d-bb02e11bdaa5.png#clientId=u80d3c12b-e9ff-4&from=ui&id=LphyQ&originHeight=100&originWidth=361&originalType=binary&ratio=1&size=8644&status=done&style=shadow&taskId=ua9361814-1909-42a6-8bbf-03424e83389)<br />TCN BPDU 格式
<a name="kmM45"></a>
### STP 树的生成
STP 的基本原理是在一个有二层环路的网络中，交换机通过运行 STP ，自动生成一个没有环路的网络拓扑。这个无环网络拓扑也叫做 **STP 树**（ STP Tree ），树节点为某些交换机，树枝为某些链路。当网络拓扑发生变化时，STP 树也会自动地发生相应的改变。<br />![2021-08-27-19-56-32-051222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066421851-6bb93d4a-a864-468b-b90d-56cf360470a0.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u4cce3aed&originHeight=672&originWidth=832&originalType=binary&ratio=1&size=44992&status=done&style=shadow&taskId=u022bf607-593a-4f00-a082-bc66a03fa8c)<br />选举范围示意图<br />STP 树的生成过程是：**_首先选举根桥，然后确定根端口和指定端口，最后阻塞备用端口_**。既然是选举，就看下参选者和选举范围。<br />![2021-08-27-19-56-32-195231.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066421787-9fdf7470-96e4-4952-912e-86fc2d651fac.png#clientId=u80d3c12b-e9ff-4&from=ui&id=T6W05&originHeight=161&originWidth=441&originalType=binary&ratio=1&size=5662&status=done&style=none&taskId=u8e7b9f26-cded-4785-b38f-5ab858f24f6)<br />选举角色和选举范围<br />STP 是通过比较 BPDU 中的信息进行选举的，最终的结果是：

- 在整个 STP 网络中，唯一的一个根桥被选举出来；
- 对于所有的非根桥，选举出根端口和指定端口，负责转发数据；落选的端口就是备用端口，处于阻塞状态，不能转发数据。

![2021-08-27-19-56-32-443221.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066421842-7ba5d74f-6363-40d3-8a66-eb937b30ca85.png#clientId=u80d3c12b-e9ff-4&from=ui&id=VKL92&originHeight=562&originWidth=766&originalType=binary&ratio=1&size=35783&status=done&style=shadow&taskId=u4a474fed-2809-409c-a77d-4ebbae45d65)<br />交互BPDU
<a name="mlmpu"></a>
#### 1、选举根桥
根桥是 STP 树的根节点。要生成一棵 STP 树，首先要确定一个**根桥**。根桥是整个二层网络的逻辑中心，但不一定是物理中心。<br />在 STP 交换机刚连接到网络时，每台交换机都以自己为根桥，从所有启动的端口发送 BPDU ，宣告自己是根桥。收到 BPDU 的交换机会比较 BPDU 中的根桥 ID 与自己的根桥 ID ，选择根桥 ID 值小的配置成自己 BPDU 中的根桥 ID 。交换机不停地交互 BPDU ，同时对根桥 ID 进行比较，直至选出一台根桥 ID 最小的交换机作为根桥。根桥会在固定的时间间隔发送 BPDU ，其它设备对 BPDU 进行转发，从而保证网络拓扑的稳定。<br />![2021-08-27-19-56-32-729227.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066470872-8f73e1ec-6954-46ca-8019-85b30dc80856.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u87ebc4bf&originHeight=471&originWidth=821&originalType=binary&ratio=1&size=30660&status=done&style=shadow&taskId=ud50dba00-627c-4738-9e86-6e5a3186d1d)<br />选举根桥<br />根桥交换机往往会承担这个网络中最大流量的转发工作，希望性能最高的交换机当选根桥交换机。但是 STP 在选举时，并不会把交换机的性能列入考量。为了让网络流量更合理的转发，可以通过配置桥优先级的值来影响根桥的选举。<br />![2021-08-27-19-56-32-991223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066470939-88586586-58b3-4557-a0de-b676f0b45186.png#clientId=u80d3c12b-e9ff-4&from=ui&id=OiQVl&originHeight=471&originWidth=821&originalType=binary&ratio=1&size=29337&status=done&style=shadow&taskId=u9304e285-4fa4-421b-8a4e-c34cae66d7a)<br />根桥的选举结果<br />同时，根桥是可以抢占的，在 STP 完成网络收敛后，如果网络中接入一台新的交换机，且新增交换机的优先级比根桥交换机更优，那么新增交换机会成为网络中新的根桥。同时，STP 将会重新收敛、重新计算网络拓扑，这个过程会引发网络震荡，对流量的正常转发造成影响。
<a name="TtDEt"></a>
#### 2、选举根端口（ Root Port ，RP ）
根桥选举出来后，其它没有成为根桥的交换机称为非根桥。STP 会为每个非根桥选举一个**根接口**，也就是在交换机的所有端口中，选择距离根桥最近的一个端口，这就是根端口。<br />在 STP 树形成后的稳定期，根桥依然会周期性的向网络中发送 BPDU ，而非根桥的根端口会收到 BPDU ，并向指定端口发送出去。<br />那是如何选择根端口的呢？根桥周期性的发送 BPDU ，非根桥的所有端口都能收到 BPDU ，对比端口收到的 BPDU 中的参数值。<br />![2021-08-27-19-56-33-293226.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066470977-aa250902-9f87-4fa6-9e2d-1acd2594d7bb.png#clientId=u80d3c12b-e9ff-4&from=ui&id=QgxBQ&originHeight=536&originWidth=521&originalType=binary&ratio=1&size=40273&status=done&style=shadow&taskId=udfb958f4-c063-4c16-a8c4-c2886b60ca0)<br />选举根端口的流程

- 选择根路径开销（ RPC ）最小的端口；

![2021-08-27-19-56-33-568254.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066513105-3b0fdfd1-fe5e-4614-be98-3e2a746feda2.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u3a478f06&originHeight=325&originWidth=622&originalType=binary&ratio=1&size=24083&status=done&style=shadow&taskId=uce68380b-e4af-41b3-a782-9c3a87dd119)<br />选举根端口第一步

- 如果 RPC 相同，那就选对端桥 ID（ BID ）最小的端口；

![2021-08-27-19-56-33-794225.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066513205-201a01de-3e3c-486f-a2a8-ffb15917f40b.png#clientId=u80d3c12b-e9ff-4&from=ui&id=N90wU&originHeight=562&originWidth=791&originalType=binary&ratio=1&size=42484&status=done&style=shadow&taskId=ua65cb4c4-128f-4dda-b7d8-bb305833e9c)<br />选举根端口第二步

- 如果对端桥 ID 相同，那就选对端端口 ID（ PID ）最小的端口。

![2021-08-27-19-56-34-089222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066513318-e1898460-3f7d-45eb-8dae-0b7ea6c367f8.png#clientId=u80d3c12b-e9ff-4&from=ui&id=JGD9i&originHeight=562&originWidth=767&originalType=binary&ratio=1&size=44346&status=done&style=shadow&taskId=uc8256311-410b-4030-a8f8-7e2f9be3c5c)<br />选举根端口第三步<br />准确的说，选举根端口的目的是选举出 STP 网络中每台交换机上与根交换机通信效率最高的端口。
<a name="QfSHu"></a>
#### 3、选举指定端口（ Designated Port ，DP ）
根端口确保了交换机到根桥的路径是唯一的，也是最优的。网络中的每条链路与根桥之间的路径也要是唯一且最优的。当一条链路中有两条或两条以上的路径到达根桥，就必须确定出一个唯一的**指定端口**，防止出现二层环路。指定端口不但是这条链路内所有端口中到达根桥的最优接口，还会向链路内发送 BPDU 。<br />虽然选举指定端口的范围和根端口不同，但是选举的原则是一致的。指定端口的选举同样会按照以下过程进行：<br />![2021-08-27-19-56-34-496231.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066556844-71673062-a250-4f69-a8c4-64baf0c61909.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u3cd7086c&originHeight=489&originWidth=771&originalType=binary&ratio=1&size=49509&status=done&style=shadow&taskId=u5e46b12b-d4a5-4da8-bb22-ad64590df11)<br />选举指定端口的流程

- 选择根路径开销（ RPC ）最小的端口；
- 如果 RPC 相同，那就选对端桥 ID 最小的端口；
- 如果对端桥 ID 相同，那就选对端端口 ID 最小的端口。

![2021-08-27-19-56-34-722227.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066556837-dfed533c-ebef-4d56-901b-5ad5406db1d1.png#clientId=u80d3c12b-e9ff-4&from=ui&id=Eciuu&originHeight=346&originWidth=622&originalType=binary&ratio=1&size=32736&status=done&style=shadow&taskId=ud90229cf-2042-44d9-b18c-cd9eb89626d)<br />选举指定端口<br />如果有人不小心将同一台交换机上的两个端口用网线连接起来，网络中就会产生环路。选举指定端口就是为了预防这种错误连接导致环路的情况。当出现这种连接时，STP 会以端口 ID 较小的端口作为指定端口，从而打破环路。<br />为什么根桥交换机的所有端口都是指定端口呢？<br />因为根桥交换机端口的根路径开销都是 0 ，根据选择选举原则，成为这条链路的指定端口。
<a name="lSHer"></a>
#### 4、阻塞备用端口（ Alternate Port ，AP ）
在确定了根端口和指定端口后，交换机上剩下的非根端口和非指定端口都叫做**备用端口**。备用端口是打破环路的关键，STP 会对这些备用端口进行**逻辑阻塞**。逻辑阻塞，是指端口不会接收或发送任何数据，但是会监听 BPDU 。当网络的一些端口出现故障时，STP 会让备用端口开始转发数据，用来恢复网络的正常通信。<br />![2021-08-27-19-56-34-980222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066556833-a04017c2-0dbd-41d0-8dbd-d2f888d6d157.png#clientId=u80d3c12b-e9ff-4&from=ui&id=MWz62&originHeight=323&originWidth=623&originalType=binary&ratio=1&size=24546&status=done&style=shadow&taskId=u2bcf8888-7ace-46bb-a76e-50c40149a11)<br />三种端口的异同如下：<br />![2021-08-27-19-56-35-136223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066591960-b793991c-d056-4732-901c-c52ab847fb1b.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u4a54156b&originHeight=161&originWidth=621&originalType=binary&ratio=1&size=5362&status=done&style=none&taskId=u59bcbe48-b8ef-48a6-ac5c-51eebcf8d9e)<br />三种端口对比表<br />一旦备用端口被逻辑阻塞后，STP 树的生成过程就完成了。
<a name="tbpfo"></a>
### STP 端口状态机
STP 不但定义了 **_3 种端口角色：根端口、指定端口、备用端口_**，还讲定义了 **_5 种端口状态：禁用状态、阻塞状态、侦听状态、学习状态、转发状态_**。<br />![2021-08-27-19-56-35-395227.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066593142-0dbc3774-048c-4b88-8645-97ba459f7215.png#clientId=u80d3c12b-e9ff-4&from=ui&id=D6RKe&originHeight=342&originWidth=682&originalType=binary&ratio=1&size=20685&status=done&style=none&taskId=ue4a6c3ff-9ec9-42d7-8509-4fef1980c13)<br />STP接口状态表<br />STP 交换机的端口启动时，首先会从**禁用状态**自动进入到**阻塞状态**。在阻塞状态，端口只能接收和分析 BPDU ，不能发送 BPDU 。如果端口选为根端口或指定端口，则会进入**侦听状态**，这时端口可以接收并发送 BPDU ，这种状态会持续一个 **Forward Delay** 的时间，默认是 15 秒。15 秒后端口会进入到**学习状态**，并持续一个 **Forward Delay** 的时间。学习状态的端口可以接收和发送 BPDU ，同时开始进行 MAC 地址学习，为数据转发做好准备。最后，端口由学习状态进入到**转发状态**，就开始进行数据转发。在整个状态的迁移过程中，端口一旦关闭或发生链路故障，就会进入到**禁用状态**；如果端口不再是根端口或指定端口，那么端口状态会立刻退回到**阻塞状态**。<br />![2021-08-27-19-56-35-608221.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066595638-5c4c77f4-7838-4d09-bdd8-42e341b40f1d.png#clientId=u80d3c12b-e9ff-4&from=ui&id=MzxJn&originHeight=801&originWidth=230&originalType=binary&ratio=1&size=41370&status=done&style=shadow&taskId=u5e4727fa-ec74-44cb-8fb8-3fe04de4ad6)<br />STP端口状态机<br />在 STP 的生成过程中，为什么要有两个 Forward Delay 时间？<br />在**侦听状态**有一个 Forward Delay 的时间，是因为 BPDU 泛洪到全网需要一定的时间，STP 完成全网拓扑的计算同样需要时间，所以停留 15 秒让 STP 有充分的时间计算全网拓扑，避免网络中出现临时的环路。在**学习状态**又有一个 Forward Delay 的时间，是由于交换机的接口上未学习到任何 MAC 地址，需要时间学习 MAC 地址，避免网络中出现不必要的数据帧泛洪现象。停留 15 秒，为进入转发状态做好准备。
<a name="xm6iH"></a>
#### STP 时间参数
STP 定义了 3 个重要的时间参数。

- **Hello Time**（ Hello 时间 ）：STP 交换机发送配置 BPDU 的时间间隔，默认是 2 秒。如果要修改时间参数，那么必须在根桥上修改才有效。
- **Forward Delay**（转发延迟）：接口从侦听状态进入学习状态，或从学习状态进入转发状态的延迟时间，默认值是 15 秒。避免在 STP 树的生成过程中可能出现的临时环路，或短暂的数据帧泛洪现象，分别在侦听和学习的端口状态各停留一个转发延迟时间。对于 STP 而言，一个阻塞端口选举为根接口或指定接口后，进入转发状态至少需要经历 30 秒的时间。
- **Max Age**（最大生存时间）：BPDU 的最大生存时间，也称为 BPDU 的老化时间，Max Age 的值由根桥指定，默认值是 20 秒。如果端口在 20 秒内收到 BPDU ，最大生存时间会重新计时；如果端口一直没收到 BPDU ，那么 BPDU 将会老化，设备会重新在端口上选择最优 BPDU ，也就是重新进行根接口的选举。

由于时间参数的设计，一个 STP 接口从阻塞状态进入到转发状态，可能需要 30 ~ 50 秒的时间，这段时间内，网络无法正常使用。
<a name="vb4fs"></a>
#### 举例说明
![2021-08-27-19-56-35-802223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066649261-dd13360a-dba1-41ae-9d83-879c4a4382d6.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u68e65af3&originHeight=471&originWidth=821&originalType=binary&ratio=1&size=26503&status=done&style=shadow&taskId=u550bb6cf-bc46-4d41-a5c6-f88d485e820)<br />端口状态迁移示意图

1. 交换机 A 、B 、C 一起启动，各交换机的每个互联端口立即从**禁用**状态进入到**阻塞**状态。在阻塞状态的端口只能接收而不能发送 BPDU ，所以任何端口都收不到 BPDU 。在等待 **Max Age** 后，每台交换机都会认为自己是根桥，所有端口的角色都成为指定端口，并且端口的状态迁移为**侦听**状态。
2. 交换机的端口进入到**侦听**状态后，开始发送自己产生的配置 BPDU ，同时也收到其它交换机发送的配置 BPDU 。因为各个交换机发送 BPDU 的时间有一定的随机性，所以有可能交换机 B 和交换机 C 先选举根桥为交换机 B ，再收到交换机 A 的配置 BPDU ，最后选举出根桥为交换机 A 。因此，无论交换机开始的状态如何，也不管中间过程有多大差异，最终的结果总是确定且唯一的：桥 ID 最小的交换机会成为根桥。端口在侦听状态持续 Forward Delay 的时间后，开始进入学习状态。由于交换机 C 的 G0/2 端口已经成为备用端口，所以状态也会变成**阻塞**状态。
3. 各个端口（交换机 C 的 G0/2 端口除外）陆续进入**学习**状态后，会持续 Forward Delay 的时间。在这段时间里，交换机开始学习 MAC 地址和端口的映射关系，同时希望 STP 树在这个时间内能够完全收敛。
4. 之后各个端口（交换机 C 的 G0/2 端口除外）相继进入**转发**状态，开始进行数据帧的转发。
<a name="QN6Sd"></a>
### STP 的改进和代替技术
在实际应用中，STP 有很多不足之处，RSTP/MSTP 对 STP 进行了哪些改良？有哪些其它技术用来代替生成树技术？欲知具体细节，且看下篇文章。
<a name="xwgYb"></a>
### 实战演练
<a name="ztnUE"></a>
#### STP 基础配置
我们用 3 台交换机搭建一个网络环境，而且网络中存在二层环路。为了对网络进行破环，将在交换机上部署 STP 。
<a name="vWmQZ"></a>
##### 网络拓扑
![2021-08-27-19-56-36-004229.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066649322-9cceba66-6b3d-4a9f-8c65-8fdecb14c0f3.png#clientId=u80d3c12b-e9ff-4&from=ui&id=utaAn&originHeight=372&originWidth=586&originalType=binary&ratio=1&size=39177&status=done&style=shadow&taskId=u4143abd4-0ab4-46a4-a82c-9a19ab28603)<br />实验拓扑图
<a name="v3fNU"></a>
##### 实验要求

- 三台交换机（ SW ）配置 STP 功能，使用命令指定根桥交换机，使用命令改变端口状态。
<a name="kiXiC"></a>
##### 操作步骤

1. 三台 SW 按照网络拓扑图，对相应的端口进行连接。SW **默认**的生成树工作模式是 MSTP ，并且自动协商出了根桥、根端口和指定端口。交换机的桥优先级默认是 32768 ，MAC 地址最小的交换机成为网络中的根桥。通过命令查看，发现 SWB 选举为根桥。

![2021-08-27-19-56-36-770225.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066650398-a172958b-732e-4f95-aecc-a0dfa02ed1c3.png#clientId=u80d3c12b-e9ff-4&from=ui&id=o36Fb&originHeight=2763&originWidth=1080&originalType=binary&ratio=1&size=763548&status=done&style=none&taskId=uc570df39-a4db-48bf-9587-3e371caf577)<br />SW默认状态

1. 在三台交换机上分别配置工作模式从默认的 MSTP 改成 **STP** ，并且启动 STP 功能。stp mode 命令用来修改交换机的工作模式。stp enable 命令用在设备上激活生成树功能，默认处于激活状态，所以这条命令为可选。

![2021-08-27-19-56-37-248223.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066688473-8742d746-b87c-476b-9e94-8f59e8767223.png#clientId=u80d3c12b-e9ff-4&from=ui&id=uad2a2ecb&originHeight=344&originWidth=1080&originalType=binary&ratio=1&size=41658&status=done&style=none&taskId=uee7f1ee4-bbeb-4c03-bfd5-164df349444)

1. 在实际的网络部署中，我们通常会修改某台设备的**优先级**，确保它成为这个网络的根桥，从而保证 STP 的稳定性。假设 SW1 是高性能的核心交换机，SW2 和 SW3 是普通性能的接入交换机。可以将 SW1 设置成根桥。在 SW1 配置的 stp root primary 命令让它成为网络中的根桥，实际上这条命令是把交换机的优先级设置成最小值 0 ，而且这个优先级不能修改。因此在 SW1 使用替代命令 stp priority 0 也能实现相同效果。stp priority 命令可以修改设备的 STP 优先级，取值范围是 _0 ~ 61440_ ，而且要是 4096 的倍数，例如 0 、4096 、8192 等。

![2021-08-27-19-56-37-887258.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066688477-6a4a1259-7934-4dd4-836c-d671b12cf18b.png#clientId=u80d3c12b-e9ff-4&from=ui&id=g2ZZK&originHeight=171&originWidth=1080&originalType=binary&ratio=1&size=17997&status=done&style=none&taskId=ub550f520-1a8d-4872-be4f-92c433e2f61)<br />根桥配置

1. 在设备上使用 display stp 命令查看 **STP 状态**，SW1 上的输出如下。交换机的桥 ID 是 _0.4c1f-ccff-4a7d_ ，其中 0 为交换机的优先级值，这就是命令 stp root primary 的作用。_4c1f-ccff-4a7d_ 是交换机的 MAC 地址，且根桥的 MAC 地址也是 _4c1f-ccff-4a7d_ ，说明这台交换机就是根桥。

![2021-08-27-19-56-38-392254.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066688722-4c9c83d2-8234-416d-b897-3a028e13de92.png#clientId=u80d3c12b-e9ff-4&from=ui&id=jV0cv&originHeight=862&originWidth=1080&originalType=binary&ratio=1&size=233631&status=done&style=none&taskId=uaa96ad55-2b41-49d2-87bf-1a8b7e227dd)<br />查看STP状态

1. 还可以使用 display stp brief 命令查看接口的 **STP 状态**，在 SW1 上输出如下。由于 SW1 是根桥，所有端口都是指定状态，都处于转发状态。

![2021-08-27-19-56-38-892480.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066730082-bdc29def-d51c-4f74-a831-956cfd6a99ba.png#clientId=u80d3c12b-e9ff-4&from=ui&id=udecd587e&originHeight=258&originWidth=1080&originalType=binary&ratio=1&size=50507&status=done&style=none&taskId=u875c0309-9a46-44d0-8c07-4c320b8b9f7)<br />查看STP接口状态

1. SWC 的 G0/0/2 端口将会被阻塞，命令输出如下。为什么 G0/0/2 的端口状态是禁用，而不是阻塞。因为 STP 已经不再使用，交换机默认使用 MSTP ，即使将生成树的模式修改为 STP ，交换机的端口状态依然和 MSTP 保持一致。端口角色（ Role ）也是 MSTP 中的概念。

![2021-08-27-19-56-39-055483.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066730073-6590978f-e945-4e71-b25b-e1bf6af0c52c.png#clientId=u80d3c12b-e9ff-4&from=ui&id=C6saS&originHeight=258&originWidth=1080&originalType=binary&ratio=1&size=49764&status=done&style=none&taskId=ufb267882-21c8-47b9-9e2e-76a722c99e4)<br />查看SWC的STP接口状态

1. 因为 SWC 的 G0/0/1 端口是根端口，收到的 BPDU 比 G0/0/2 收到的更优，所以端口被阻塞。如果我们希望阻塞的不是 G0/0/2 ，而是 G0/0/1 端口，那就要让 G0/0/2 成为根端口，比如将 G0/0/1 端口的 **Cost 值调大**，让 SWC 的 G0/0/1 到达根桥的 RPC 比 G0/0/2的更大。SWC 增加配置如下：

![2021-08-27-19-56-39-206492.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066730054-a75e77ac-fcb6-4980-b9e9-d0763a38c9e1.png#clientId=u80d3c12b-e9ff-4&from=ui&id=O7dvj&originHeight=171&originWidth=1080&originalType=binary&ratio=1&size=29457&status=done&style=none&taskId=u5b118b96-350e-42e6-a565-2f1ce91209e)<br />修改STP端口Cost值

1. 再观察下 SWC 的**端口状态**，G0/0/2 端口成为根端口，G0/0/1 端口被阻塞。

![2021-08-27-19-56-39-609484.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066755783-a818d111-be3b-4b57-88be-78f81507914f.png#clientId=u80d3c12b-e9ff-4&from=ui&id=u780b7fdb&originHeight=258&originWidth=1080&originalType=binary&ratio=1&size=49424&status=done&style=none&taskId=u98417516-64d2-4444-97bc-08324d1e3bd)<br />查看SWC的端口状态
<a name="pGMjU"></a>
##### 功能验证
查看三台 STP 交换机的**端口状态**，是否符合预期结果。<br />![2021-08-27-19-56-40-116480.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066756087-4c071d9e-ee63-4aaa-8476-4b791b0fb4a1.png#clientId=u80d3c12b-e9ff-4&from=ui&id=to12J&originHeight=603&originWidth=1080&originalType=binary&ratio=1&size=145348&status=done&style=none&taskId=ucff5b670-2248-4e31-8f1a-4d06a99f4ec)<br />查看三台交换机的端口状态<br />还可以抓包验证 STP 交互的 **BPDU 报文**内容是否正常。<br />![2021-08-27-19-56-40-560482.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630066756097-39e5e67b-d6f4-491c-b3f0-dc049df2d24e.png#clientId=u80d3c12b-e9ff-4&from=ui&id=kIebS&originHeight=531&originWidth=1080&originalType=binary&ratio=1&size=159170&status=done&style=shadow&taskId=u80daa4f8-2955-4e15-970e-79facbef38e)
