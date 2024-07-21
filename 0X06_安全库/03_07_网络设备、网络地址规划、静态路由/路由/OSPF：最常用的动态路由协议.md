网络 路由协议
<a name="xwbMO"></a>
## RIP 缺陷
提及 OSFP 时，还得先从 RIP 说起。<br />RIP 以跳数来计算到达目的网络的最优路径，在实际应用时并不合适，以网络带宽和链路时延来衡量网络质量会更合理。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918030-9a9b0955-3f0a-45f1-ae4f-9ea6efd1c415.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u7d12f559&originHeight=424&originWidth=603&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4d13e1a8-3ed1-4e2a-ad3f-f9539c8668c&title=)<br />RIP 支持的最大跳数是 16 ，无法用于搭建大规模的网络。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918047-e6ffe872-9ff0-4317-8844-0f1d8fd95603.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u655a36cf&originHeight=532&originWidth=780&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf0c4b24f-8d23-46d4-96f6-a50963ac28a&title=)<br />RIP 的收敛速度慢，RIP 会对不可达路由的信息更新进行抑制，原路由失效，新路由要等到抑制时间结束后，才能更新。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348917988-c8a22d12-7c9a-4303-9bcf-b082387d7ba3.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u07f01b43&originHeight=535&originWidth=655&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1036640f-8148-4224-88e5-b92b9b4105b&title=)<br />RIP 的更新周期长，一个路由器突然离线，其它路由器要很长时间才能发现。<br />RIP 使用广播发送全部路由信息，网络规模越大，路由信息占用的网络资源也越大。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918002-36aa4f5b-fc2a-46f6-8676-a6306cb408be.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u81859d3c&originHeight=451&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5d4d576c-dba3-471a-87f8-f2e165ceef9&title=)<br />因此，RIP 路由并不适合大规模的网络，而 OSPF 协议解决了这些问题，得到了广泛的使用。
<a name="pKLWF"></a>
## OSPF 原理
OSPF ，是 Open Shortest Path First 的缩写，译为开放最短路径优先。OSPF 是基于链路状态（ Link State ）的自治系统内部路由协议，用来替代 RIP 协议，通用的是 OSPFv2 。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348917975-89a73d7d-8a1e-4abf-9374-0e7414504565.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u7bb5dcb2&originHeight=324&originWidth=577&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u77beaa37-57c3-4a06-addc-813d8072f6c&title=)<br />与距离矢量协议不同，链路状态协议使用最短路径优先算法（ Shortest Path First ，SPF ）计算和选择路由。这类路由协议关系网络链路或接口的状态，比如 up 、down 、IP 地址、掩码、带宽、利用率和时延等。每台路由器将已知的链路状态向其它路由器通告，让网络上每台路由器对网络结构有相同的了解。然后，路由器以此为依据，使用 SPF 算法计算和选择路由。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918594-c6c49f15-edc5-4d3b-8c73-2e27b0f1a44e.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u4c04e4aa&originHeight=272&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u47786a73-4b8b-42d3-a216-d5cd50d90e6&title=)<br />OSPF 协议使用组播发送协议包，节约资源，又减少对其它网络设备的干扰。<br />OSPF 将协议包封装在 IP 包中，协议号 89 。由于 IP 协议是无连接的，OSPF 定义了一些机制保证协议包安全可靠的传输。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918554-309621f9-c6ac-40c7-87c3-35334a9a2a5d.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u78e31b3c&originHeight=241&originWidth=321&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc77cfeb8-b696-4fe2-bc06-af4226005c7&title=)<br />总之，OSPF 协议比 RIP 有更大的扩展、快速收敛和安全可靠等特性，采用路由增量更新保证路由同步，减少对网络资源的浪费。
<a name="RzwTJ"></a>
### OSPF 协议简介
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918548-0e9750e4-ec64-41a8-b4ac-969c99383ed6.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=ud8ce579d&originHeight=651&originWidth=779&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u109fb70e-5339-4fa0-a041-5fb031ac79e&title=)
<a name="mA40K"></a>
#### OSPF 协议有四个主要过程：
<a name="r7Dv8"></a>
##### 寻找邻居
OSPF 协议启动后，先寻找网络中的邻居（ Neighbor ），也就是通过 Hello 报文确认可以双向通信。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918700-e8437283-fd0a-41e1-9f24-bb8cc9d6d646.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=uddb6e21f&originHeight=431&originWidth=601&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u097c0887-5de2-40cf-89bd-3722a78d3f2&title=)
<a name="FD7Rp"></a>
##### 建立邻接关系
一部分路由器形成邻居关系后，就开始进行建立邻接关系（ Adjacency ）。建立了邻居关系的路由器才能互相传递链路状态信息。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918624-e12b7dc7-630e-4d78-8694-5008e5c85544.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=ue9398b97&originHeight=431&originWidth=601&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7f1f54ad-8600-481d-b2a3-ebbc2648e82&title=)
<a name="EthDI"></a>
##### 链路状态信息同步
建立邻接关系的 OSPF 路由器在网络中交互 LSA（链路状态通告），最后形成包含网络完整链路状态信息的 LSDB（链路状态数据库）。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348918901-fad38f90-3a90-4646-b5fa-fe4d8334a149.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u522f814d&originHeight=831&originWidth=665&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9fb35109-17e0-4eda-bf10-8a6fe963981&title=)
<a name="kRB9F"></a>
##### 计算路由
LSDB 同步完成后，OSPF 区域内的每个路由器对网络结构有相同的认识，邻居路由器之间形成完全的邻接关系。然后，每台路由器根据 LSDB 的信息使用 SPF（最短路径优先）算法独立计算出路由。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919001-10b0656f-fcc9-4cc3-a86b-415221be1744.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=uafbe412d&originHeight=831&originWidth=665&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u96bf9a35-5f89-4ec6-9372-1da26a24df8&title=)
<a name="xJ1KY"></a>
### OSPF 协议过程
将 OSPF 的四个过程展开来讲，就是一个个邻居状态的切换，不同的邻居状态有不同的行为。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919144-6d0ee0ee-5821-4258-a133-fc7f9def4bbb.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u36f21804&originHeight=566&originWidth=522&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u870659b9-61ea-4387-8173-5e4035dde74&title=)
<a name="dA5Fh"></a>
#### Down（失效）
OSPF 邻居的初始状态，表示接口没有收到邻居发来的 Hello 报文。
<a name="bnZmV"></a>
#### Init（初始）
收到邻居发送的 Hello 报文，但是报文内没有自己的 Router-ID ，邻居状态就是 Init 。这个状态表示，直连链路上有一个 OSPF 路由器，但是还未确认双向通信。接下来，路由器会把对方的 Router-ID 添加到发送的 Hello 报文中。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919190-e4de23e8-6388-42c9-9c5b-4de95d8dbd03.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=ub209e634&originHeight=239&originWidth=539&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u443ce28b-03c1-4875-8fa9-d7dd522c429&title=)
<a name="BT3AV"></a>
##### 什么是 Router-ID ？
Router-ID 用于标识 OSPF 路由器，是一个 32 位的数值，跟 IPv4 地址格式一样。连续的 OSPF 路由器组成的网络叫做 OSPF 域，域内 Router-ID 必须唯一，也就是在同一个域内不允许出现两台相同 Router-ID 的路由器。Router-ID 可以手动设置，也可以自动生成，常见的做法是把设备的 Router-ID 指定为设备的 Loopback 接口的 IP 地址。
<a name="BQgDu"></a>
#### Attempt（尝试）
只在 NBMA 网络中出现。当路由器的 NBMA 接口启动后，邻居状态从 Down 切换到 Attempt 。这种状态下，路由器周期性的向邻居发送 Hello 报文，但是未收到邻居的有效 Hello 报文。当路由器收到邻居发送的没有自己 Router-ID 的 Hello 报文后，就将邻居状态切换到 Init 。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919346-eac544e4-8aec-4a33-be71-9a4d7ab70044.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=ue8958972&originHeight=259&originWidth=561&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua3d0b039-dc7a-4acc-83a8-8d4754f9837&title=)
<a name="Nku40"></a>
##### 什么是 NBMA ？
NBMA 类型的网络已经看不到了，它允许多台路由器接入，但是没有广播能力，无法使用组播或广播，只能配置成单播发送 OSPF 报文。帧中继、X.25 都是这类网络。
<a name="HKNRS"></a>
#### 2-Way（双向通信）
路由器收到邻居的 Hello 报文，报文里有自己的 Router-ID 时，状态切换成 2-Way ，表示两个路由器形成了可以双向通信的邻居关系。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919449-3e5eed3b-7afa-4ed0-98e2-7b4111763807.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=ua33fa7f7&originHeight=359&originWidth=539&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub9177cff-609e-48c4-a4b5-e5b2358478b&title=)
<a name="Jn1qf"></a>
#### 选举 DR 和 BDR
如果路由器是在一个 MA 网络，邻居状态在 2-Way 后，会进行 DR 和 BDR 选举。
<a name="D24VE"></a>
##### 什么是 MA 网络？
MA 网络，即多路访问网络，是在同一个共享介质中连接多个设备的网络。网络中的任意两台设备都能直接进行二层通信。MA 网络有两种，一种是 BMA 网络，即广播型多路访问网络，比如以太网，典型场景就是一台以太网交换机连接着多台路由器，如果有一个广播数据发出来，整个网络中的路由器都能收到。另一种是 NBMA 网络，即非广播型多路访问网络，比如帧中继，只是帧中继不支持广播。<br />在 MA 网络中，n 台路由器都两两建立邻接关系，那么就有 n(n-1)/2 个邻接关系，会消耗大量的路由器资源，增加网络中 LSA 的泛洪数量。为了优化邻接关系数量，减少不必要的协议流量，OSPF 会在每一个 MA 网络中选举一个 DR（指定路由器）和一个 BDR（备用指定路由器）。<br />既不是 DR 也不是 BDR 的路由器叫做 DROther ，MA网络中所有 DROther 只和 DR 及 BDR 建立 OSPF 邻接关系，BDR 也和 DR 建立邻接关系，DROther 之间只停留在 2-Way 状态。这样，就有 2(n-2)+1 个邻接关系，数量得到优化。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919416-77a0141b-9076-40da-9321-56de13dc01ec.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u8c85d846&originHeight=1155&originWidth=667&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4f831251-66ea-49e0-86f8-2a8b23950c8&title=)<br />DR 会侦听网络中的拓扑变化信息，并将变更信息通知给其它路由器。BDR 会监控 DR 状态，当 DR 发生故障时就接替它的工作。<br />DR 、BDR 的选举通过 Hello 报文实现，发生在 2-Way 状态之后。Hello 报文有路由器接口的 DR 优先级，取值范围是 0 ~ 255 ，默认值为 1 ，DR 优先级为 0 的接口没有 DR 和 BDR 的选举资格。<br />当接口激活 OSPF 后，它会查看网络中是否存在 DR ，如果有就使用已经存在的 DR ，也就是 DR 不可抢占，否则选择最高优先级的路由器成为 DR ，当优先级相等时，选择 Router-ID 最大的路由器成为 DR 。之后还会进行 BDR 的选举，选举过程与 DR 类似。<br />需要注意的是，DR 和 BDR 是一个接口级别的概念。某台路由器是 DR ，这种说法不准确，严谨的说法是：某台路由器的某个接口在这个 MA 网络中是 DR 。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919514-e174c2c1-6832-4a18-a112-637a00854d10.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u7069f5f6&originHeight=291&originWidth=789&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud8d28539-bee6-48a0-b9ae-065f5742e15&title=)<br />在一个 MA 网络中，DR 要确保网络中的所有路由器有相同的 LSDB ，也就是确保 LSDB 同步。DR 使用组播地址 224.0.0.5 向网络中发送 LSU 报文，所有 OSPF 路由器都会侦听这个组播地址，并同步自己的 LSDB 。而 DROther 感知到拓扑变化时，向 224.0.0.6 发送 LSU 报文通告这个变化，DR 和 BDR 会侦听这个组播地址。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919719-82559e79-fb71-48aa-ac4f-da758a969ecd.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u33af735f&originHeight=439&originWidth=601&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucc407272-160e-4150-9650-8c264bc5f20&title=)
<a name="EnP0f"></a>
#### ExSart（交换开始）
接来下，路由器会进入 ExStart 状态，发送空的 DD 报文，用于协商 Master/Slave ，Router-ID 最大的路由器成为 Master 路由器，DD 报文的序列号由 Master 路由器决定。协商 Master/Slave 的报文是空的、不携带 LSA 头部的 DD 报文，这时报文的 I 位被设置成 1 。
<a name="g7UYA"></a>
#### Exchange（交换）
接下来，路由器进入 Exchange 状态，向邻居发送描述自己 LSDB 的 DD 报文，DD 报文中包含 LSA 头部。DD 报文逐个发送，每个报文中都有 DD 序列号，DD 序列号由 Master 路由器决定，序列号在 DD 报文的交互过程中递增，确保交互过程的有序性和可靠性。
<a name="rULK7"></a>
##### 什么是 DD 报文？
DD 报文用于描述 LSDB ，携带 LSDB 中 LSA 的头部数据，而非完整的 LSA 内容。在路由器邻接关系的建立过程中，先用空的 DD 报文协商 Master/Slave ，然后用 DD 报文描述各自的 LSDB ，这种 DD 报文包含 LSDB 里的 LSA 头部。路由器可以使用多个 DD 报文来描述 LSDB ，为了保证 DD 报文传输的顺序和可靠，Master 路由器使用序列号字段递增的方式，主导整个 LSDB 描述过程。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919888-d5f83203-dc92-4717-a54e-4b0f2dbd1cb5.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u44c1c6c9&originHeight=759&originWidth=619&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc0756ca3-a0f1-413f-aa5f-249c5b87831&title=)
<a name="GnBca"></a>
#### Loading（加载）
接下来，路由器进入 Loading 状态，路由器向邻居发送 LSR 请求 LSA 的完整信息。邻居使用 LSU 进行回应，LSU 报文里有 LSA 的完整信息。在收到 LSU 报文后，路由器需要发送 LSAck 对 LSA 进行确认。
<a name="D1ilI"></a>
#### Full（完整）
当接口上需要请求的 LSA 列表为空时，表示路由器已经完成了和邻居的 LSDB 同步，没有再需要请求的 LSA 了，这时邻居的状态就是 Full 。
<a name="l9FTi"></a>
#### 路由计算
接下来，路由器开始计算路由。先评估一台路由器到另一台路由器需要的度量值。OSPF 协议是根据路由器的每一个接口的度量值决定最短路径的。一条路由的开销是指到达目的网络的路径上所有路由器接口的度量值总和。<br />度量值和接口带宽有关，路由器的接口度量值是根据公式 100/带宽（ Mbps ）计算出来的，它作为评估路由器之间网络资源的参考值。另外也可以通过命令手工指定路由器的度量值。
<a name="bwBQm"></a>
##### 什么是度量值？
每种路由协议对度量值的定义是不同的，OSPF 使用 Cost（开销）作为路由度量值，Cost 值越小，则路径（路由）越优。每一个激活 OSPF 的接口都有一个接口的 Cost 值，值等于 100/接口带宽 Mbit/s，计算结果取整数部分，当结果小于 1 时，值取 1 。这个值也可以人为修改，修改值会直接影响 Cost 值的计算，从而影响网络中 OSPF 路由的选择。<br />同步 OSPF 区域内每台路由器的 LSDB ，路由器通过交互 LSA 实现 LSDB 的同步。LSA 不但携带了网络连接状况信息，而且携带各接口的 Cost 信息。<br />由于一条 LSA 是对一台路由器或一个网段拓扑结构的描述，整个 LSDB 就形成了对整个网络的拓扑结构的描述。所有路由器得到一张完全相同的图。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919833-110a0bd3-c6d4-432d-ad3d-e55c0116cc48.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=u1c0c763d&originHeight=361&originWidth=755&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7f8b71c7-862f-4e3b-ad60-c2c6ddf23be&title=)<br />使用 SPF（最短路径优先算法）计算出路由。OSPF 路由器用 SPF 算法以自己为根节点，计算出一棵最短路径树。这棵树上，由根到各个节点的累计开销最小，也就是从根到各个节点的路径都是最优的，这样就获得了由根去往各个节点的路由。计算完成后，路由器将路由加入到 OSPF 路由表。当 SPF 算法发现有两条到达目的网络的路由的 Cost 值相同，会将这两条路由都加入到 OSPF 路由表中，形成等价路由。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640348919825-aeffb373-ccaf-46fb-85c7-e987afce2d3c.webp#clientId=u341a9cd3-9b3c-4&from=paste&id=ue86ee19b&originHeight=233&originWidth=761&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u89638264-fec9-4c08-a1c5-b47b39c5a22&title=)
