网络 IS-IS<br />说起 IS-IS ，就不可避免要提到 OSPF 。它俩都是链路状态协议，但无法互相替代。IS-IS 是从 OSI 扩展到 TCP/IP 的，而 OSPF 又源于 IS-IS 。它们协议原理类似，但具体实现的机制不同，就有了不同的应用场景：

**OSPF** 路由类型多、区域类型多、开销规则合理、网络类型多，适合在园区网层次化部署。<br />**IS-IS** 算法快速、报文简洁、收敛快速、路由承载庞大，更适合运营商扁平化网络部署。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660057-e7e243d6-ea74-4070-9192-d1c2904b885b.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u0b06aa73&originHeight=1130&originWidth=575&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u749723d3-e440-4919-bc6e-44d3de9a4e2&title=)
<a name="Hxjqz"></a>
### 分层结构
链路状态协议的分层结构，可以减少 LSA 的数量，降低设备的性能消耗。OSPF 有骨干区域、常规区域、特殊区域，而 IS-IS 就简单些，只有**骨干网络**和**常规区域**。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660047-aa927005-1689-4d28-a4ad-3e92dd208a5e.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u903a622d&originHeight=351&originWidth=461&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u04495d2c-e853-406b-a6d6-eef80e5716e&title=)<br />ISIS 的骨干网络是由连续的 Level-2 和 Level-1-2 路由器组成。图中的 R1 、R2 、R3 和 R4 构成骨干网络。连续的 Level-1 和 Level-1-2 路由器构成的区域叫做 **Level-1 区域**。比如 Area 49.0001 和 Area 49.0002 。**Level-2 区域**，由连续的、同一个区域的 Level-2 和 Level-1-2 路由器构成。比如 Area 49.0003 和 Area 49.0004 。Level-1 区域可能有多个，骨干网络也可能覆盖多个 Level-2 区域。<br />IS-IS 的**区域**，不是按照接口划分的，而是按照设备划分的。配置 IS-IS 时，要指定设备的区域，一个设备可以同时属于多个区域。配完后，设备的所有接口都属于这个区域。比如 R1 和 R5 建立 Level-1 的邻居关系，R3 和 R4 建立 Level-2 的邻居关系。同时，区域的交界不在设备上，而是在**链路**上。比如 Area 49.0001 和 Area 49.0004 的交界就是 R1 和 R4 连接的链路上。<br />IS-IS 的每个 Level-1 区域必须和骨干网络直连，比如 Area 49.0001 就是通过 R1 连接到骨干网络。**Level-1-2 路由器**作为 Level-1 区域和骨干网络的桥梁，Level-1 区域内的路由通过 Level-2 LSP 通告给骨干网络。而 Level-1-2 路由器不会把骨干网络的路由通告给 Level-1 区域，而是下发默认路由给 Level-1 路由器。跟 OSPF 的 Totally NSSA 类似。Level-1 路由器只知道区域内的路由，区域外的网络，通过默认路由到达。
<a name="PRJes"></a>
### 路由器分类
运行 IS-IS 的路由器，根据 Level 的不同，可分为两类：**Level-1** 和 **Level-2** 。路由器可以是 Level-1 类型，或 Level-2 类型，还可以同时是 Level-1 和 Level-2 类型，即 Level-1-2 类型，实际上 Level-1-2 并不是一种单独的 IS-IS 路由器类型。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660035-a72f67d7-8a12-44d3-a91d-a1fb08cfed02.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u9b3ae419&originHeight=431&originWidth=472&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u395520f9-7c04-48d2-9045-9b8b8268c5d&title=)
<a name="uKTUK"></a>
#### Level-1 路由器
Level-1 路由器，比如图中的 R1 ，只能和同一区域内的其它 Level-1 或 Level-1-2 路由器建立 IS-IS 邻居关系，也叫做 Level-1 邻居关系。它无法与 Level-2 路由器建立邻居关系。<br />Level-1 路由器只维护 Level-1 LSDB ，根据链路状态信息计算区域内的网络拓扑及最优路由。Level-1 只能通过 Level-1-2 路由器接入骨干网络访问其它网络区域。
<a name="yjCpN"></a>
#### Level-2 路由器
Level-2 路由器，比如图中的 R4 、R5 、R6 、R7 ，可以看做 IS-IS 骨干网络的路由器，其实骨干网络是由连续的 Level-2 路由器及 Level-1-2 路由器组成的。<br />Level-2 路由器只能和 Level-1-2 或 Level-2 路由器建立邻居关系，也叫做 Level-2 邻居关系。Level-2 路由器只维护 Level-2 的 LSDB ，Level-2 路由器有整个 IS-IS 域的所有路由信息。
<a name="gL5r1"></a>
#### Level-1-2 路由器
Level-1-2 路由器，比如图中的 R2 、R3 ，同时是 Level-1 和 Level-2 级别的路由器，可以和同一区域的 Level-1 、Level-1-2 路由器建立 Level-1 邻居关系，也可以和 Level-2 路由器或 Level-1-2 路由器建立 Level-2 邻居关系。<br />Level-1-2 路由器同时维护 Level-1 的 LSDB 和 Level-2 的 LSDB ，分别计算 Level-1 路由和 Level-2 路由。通常，Level-1-2 路由器连接一个 Level-1 区域，也连着骨干网络，作为 Level-1 区域与其它区域通信的桥梁，下发的 Level-1 LSP 中设置 ATT 位，Level-1 路由器根据这条 LSP ，生成一条指向 Level-1-2 路由器的默认路由。<br />默认状态下，路由器的全局 Level 为 Level-1-2 。可通过命令修改设备类型。
<a name="rUobK"></a>
### 度量值
IS-IS 使用 **Cost** 作为路由度量值，也叫做开销、成本、代价。Cost 值越小，路由越优。每个接口都有 Cost 值，默认值为 10 ，与接口带宽不相关。某些情况下就会出现问题，比如设备选择 Cost 更优的低带宽路径，不选择 Cost 更劣的高带宽路径。这时，就要根据实际情况，修改接口 Cost 值。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660099-039a7f3c-f924-4f7a-941f-764716c91d16.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u0fb8db3d&originHeight=68&originWidth=522&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue875920c-eb96-4d96-84f2-f9cc1d313ba&title=)<br />IS-IS 路由的 Cost 等于源设备到目的网段所有出接口的 Cost 总和。R1 通过 IS-IS 获取到 3.3.3.0/24 的路由，Cost 值为 30 。<br />默认情况下，Cost 类型是 **Narrow** ，接口 Cost 长度是 6bit ，取值范围是 1~63 。路由 Cost 长度是 10bit ，最大值为 1023 。但在大型网络中，度量值范围太小，不能满足需求，就引入了 **Wide** 类型 Cost 。Wide 类型时，接口 Cost 是 24bit ，取值范围是 1～16777215 。同时，路由 Cost 值可达到 4261412864 。从而能够支持更大规模的网络，Cost 的路由控制也更灵活。<br />Wide 配置命令：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660065-da2967ce-e8ed-4062-adbb-bdafc7844f88.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u68068b41&originHeight=141&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u427e7cfa-f674-47e1-ae7a-65a14fbd833&title=)<br />路由器使用的 Cost 类型是 Narrow 时，只能接收和发送 Cost 类型是 Narrow 的路由。同理，Cost 类型改为 Wide 后，只能接收和发送 Cost 类型是 Wide 的路由。当然，为了兼容两种模式，就有了 Compatible 类型，可同时接收和发送 Narrow 类型和 Wide 类型的路由。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660396-bc22a6ac-abac-44b0-bbf7-4395a8e98c24.png#clientId=ue9b530e4-d7dc-4&from=paste&id=ucac9563f&originHeight=241&originWidth=391&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4975ae17-c056-41bc-ab7a-e4871782cdf&title=)<br />默认情况下，IS-IS Cost 类型为 Narrow 时， Cost 值为 10 。无论接口的带宽是多少，值都是 10 。简单的方法，就是手动修改接口 Cost 。既然路由都能动态生成，那么 Cost 还要手动修改，显然不合适。肯定有自动计算接口 Cost 的功能，使用 auto-cost enable 命令激活。接口的 Cost 值 = ( bandwidth-reference / 接口带宽值 ) × 10 。bandwidth-reference 值默认是 100 ，可通过命令修改。比如千兆接口的带宽是 1000Mbps ，结果是 1 ，即 Cost 值为 1 。当然，只有 Cost 类型为 Wide 或 Wide-compatible 时，自动计算才生效。如果 Cost 类型为 Narrow 、Narrow-compatible 或 Compatible 时，根据带宽和度量值的对应关系表设置 Cost 值。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660483-8c21de50-c962-4e74-8a30-4cbf96446628.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u14388784&originHeight=281&originWidth=321&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u70434396-a0b3-4d17-b604-0940cad726b&title=)
<a name="qFUK1"></a>
### 三张表
IS-IS 维护了三张重要的数据表，分别是**邻居表**、**LSDB** 、**IS-IS 路由表**。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660475-fdd0fafc-d7cd-4483-87b3-645b1befd220.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u54768c65&originHeight=191&originWidth=641&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufe9beaa2-4e25-4306-87b0-a130efef68d&title=)
<a name="Xes4J"></a>
#### 邻居表
相邻两台路由器要先建立邻居关系，才开始交互 LSP 。路由器在直连链路上发现的邻居加载到邻居表中。使用 display isis peer 命令查看邻接表，包含邻居的系统 ID 、状态、保活时间和类型等信息。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660486-0b7ac2fb-9a6b-4fe0-a1d4-068d48bfa07e.png#clientId=ue9b530e4-d7dc-4&from=paste&id=uc508e667&originHeight=418&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u593753bb-86dc-47fe-bd32-62fc0d3193f&title=)<br />R2 发现了两个邻居，GE0/0/0 接口发现了邻居 R1 ，邻居状态是 UP ，还有 28 秒保持时间，邻居类型是 Level-1 ，接口优先级是 64 ；GE0/0/1 接口发现了邻居 R3 ，邻居类型是 Level-2 。
<a name="fuz0v"></a>
#### LSDB
直连的两台路由器建立邻居关系后，开始交互 LSP 。路由器把自己生成的、网络中泛洪的 LSP 存储到 LSDB 中。用这些信息绘制网络拓扑、计算路由。使用 display isis lsdb 命令，查看路由器的 IS-IS LSDB 。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660599-d104e47b-46ee-44fb-8383-231025dc897b.png#clientId=ue9b530e4-d7dc-4&from=paste&id=uf072edc7&originHeight=658&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9d5f1a50-7c9e-4611-94e9-a628bf8b9c5&title=)<br />R1 是一台 Level-1 路由器，只维护 Level-1 LSDB ，LSDB 中有三个 LSP 。每个 LSP 使用 LSP ID 标识，LSP ID 由三部分组成：

- **系统 ID** ：6byte ，生成 LSP 的路由器的系统 ID 。
- **伪节点 ID** ：1byte 。字段值有 0 和非 0 两种情况。值为 0 时，表示普通 LSP 。值为非 0 时，由 DIS 分配，表示伪节点 LSP 。
- **分片号**：1byte ，如果 LSP 过大，会进行分片。通过不同的分片号，标识和区分不同的 LSP 分片。同一个 LSP 的不同分片，必须要有相同的系统 ID 和伪节点 ID 。

下面以 R1 生成的 LSP 为例，LSD ID 是 0000.0000.0001.00-00 。LSP ID 后面带星号 “ * ” ，表示是路由器自己生成的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660666-0deb0242-927a-4392-a0fa-f78b158bc8ec.png#clientId=ue9b530e4-d7dc-4&from=paste&id=uec652df3&originHeight=124&originWidth=367&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc61bd90e-ff64-4c1d-bda9-9638cc34682&title=)<br />LSDB 的 Seq Num 表示 LSP 的序列号，用来表示 LSP 的新旧。R1 也不会有 R3 的 LSP ，因为其它区域的 LSP 不会泛洪到 Level-1 区域内。<br />R2 是 Level-1-2 路由器，会同时维护 Level-1 LSDB 和 Level-2 LSDB 。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660740-4a2e471e-5a52-4914-b25d-322e1a71c272.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u687081ab&originHeight=1034&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uad9790c3-bf58-4e6a-b02e-7bcae8dd82d&title=)
<a name="HDaq9"></a>
#### IS-IS 路由表
路由器基于 LSDB ，运行路由算法，计算出最优路由。计算出的路由存放在 IS-IS 路由表中，使用 `display isis route` 命令查看 IS-IS 路由表，是否会加载到全局路由表中，取决于路由优先级等因素。<br />查看 R1 的 IS-IS 路由表：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660801-e0b55697-683f-48e3-b16b-85b4c39dd970.png#clientId=ue9b530e4-d7dc-4&from=paste&id=ue6a67c99&originHeight=633&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4448639e-abf0-4c13-a675-8c38151934e&title=)<br />查看 R2 的 IS-IS 路由表：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660820-a18e85ef-b8ca-4b35-b113-caa50e9f106c.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u233a1705&originHeight=996&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u83bea4d7-f87c-43f5-9f00-6f3aea284f2&title=)<br />R2 通过 IS-IS 学习到 Level-1 路由 1.1.1.0/24 和 Level-2 路由 3.3.3.0/24 。Level-1 路由根据 Level-1 LSDB 计算出的路由，Level-2 路由根据 Level-2 LSDB 计算出的路由。当到达目的网段，既有 Level-1 路由，又有 Level-2 路由时，优选 Level-1 路由，与路由 Cost 无关。<br />查看 R2 的全局路由表：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358660987-64d39814-439b-4afd-bd82-2e10d7e76a49.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u5e4c928b&originHeight=656&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u68241e08-b9ef-4b1c-b3a1-2fb153f5cbf&title=)
<a name="CeqXZ"></a>
### 协议报文
IS-IS 的协议报文直接采用数据链路层封装，对比 OSPF 少了 IP 层封装，IS-IS 报文的封装效率更高。IS-IS 报文直接封装在以太网数据帧中，使用几种 PDU ：
<a name="YMII2"></a>
#### IIH ( IS-IS Hello )
IIH PDU 用于建立和维护 IS-IS 的邻居关系。有三种 IIH PDU ：**Level-1 LAN IIH** 、**Level-2 LAN IIH** 和 **P2P IIH** 。其中 Level-1 LAN IIH 和 Level-2 LAN IIH 用于 Broadcast 类型的网络，Level-1 设备只发送和侦听 Level-1 LAN IIH ，Level-2 设备只发送和侦听 Level-2 LAN IIH ，Level-1-2 设备发送和侦听两种类型的 LAN IIH 。P2P IIH 用于 P2P 类型的网络中。
<a name="Lgfbx"></a>
#### LSP ( Link-State Packet )
ISIS 使用 LSP 存放链路状态信息，LSP 是一种独立的 PDU 。LSP 分为 **Level-1 LSP** 和 **Level-2 LSP** ，根据邻居关系类型，发送对应的 LSP 。比如 Level-1 邻居关系，交互 Level-1 LSP ，Level-1-2 邻居关系，Level-1 LSP 和 Level-2 LSP 都会交互。
<a name="uArcO"></a>
#### CSNP ( Complete Sequence Number PDU )
CSNP 用于 LSDB 同步，包含路由器 LSDB 所有 LSP 摘要。CSNP 也分为 Level-1 CSNP 和 Level-2 CSNP ，不同邻居关系交互不同类型的 CSNP 。<br />CSNP 包含设备的 LSDB 所有 LSP 摘要信息，一条 LSP 的摘要信息包括 LSP 的 LSP ID 、序列号、剩余生存时间、校验和，这四个信息是 LSP 头部的关键信息。
<a name="L7eY7"></a>
#### PSNP ( Partial Sequence Number PDU )
PSNP 只包含一部分 LSP 的摘要信息，用于请求 LSP 更新，也分为 Level-1 PSNP 和 Level-2 PSNP 。PSNP 还用于 P2P 网络中，对收到的 LSP 进行确认。
<a name="EaL4X"></a>
#### IS-IS PDU
IS-IS PDU 报文结构主要是三个部分：**通用的头部**、**PDU 特有的头部**、**可变长部分**。通用的头部，是指所有 IS-IS PDU 都有的、相同格式的头部。每种 PDU 都有自己特有的头部。可变长部分，使用三元组格式存储内容，也就是 **TLV** ，全称 Type-Length-Value ，也就是类型、长度、值的三元组。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661006-e3bb8b49-fab3-4f43-8c6b-9a191f205b18.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u67aa8b45&originHeight=41&originWidth=361&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc7b9ad76-b7bd-43cc-93d3-2c217a99fb4&title=)<br />TLV 的各元素描述如下：

- **Type（类型）**：1byte ，表示 TLV 的类型，不同 TLV 类型携带不同的信息。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661080-5a005a13-6e21-4249-ae22-3dfc046574da.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u1775e9a3&originHeight=401&originWidth=601&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uad12875a-ee43-45ea-bc07-540d3178704&title=)

- **Length（长度）**：1byte ，表示 Value 的长度。不同 TLV 类型，长度也不同。
- **Value（值）**：字节数的长度是 Length 的值，表示 TLV 的有效内容。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661188-18d594cb-a4a4-4b04-abde-8482ec0f6424.png#clientId=ue9b530e4-d7dc-4&from=paste&id=ua4047289&originHeight=641&originWidth=441&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7ec60e5d-a314-4d3f-8c4a-c25002b313e&title=)<br />每种 PDU 都有一定数量的 TLV ，TLV 非常灵活、方便扩展。不对协议做大改动的情况下，引入新的 TLV 就可以实现新功能的支持，这就是 TLV 的绝对优势。
<a name="enus4"></a>
### LSP
IS-IS 使用 LSP 来描述网络拓扑和网段信息，有两种 LSP ：Level-1 LSP 和 Level-2 LSP 。LSP 由 PDU 通用头部和 LSP 报文组成，LSP 报文包含 **LSP 特有头部**和 **TLV** 。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661237-40028ba4-8be4-443e-b533-603e93e95d76.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u0cc6232c&originHeight=721&originWidth=561&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub6308765-adcc-45b2-b665-8f98ae05f67&title=)<br />LSP 字段及说明：

- **PDU 长度（ PDU Length ）**：PDU 的总长度，单位是字节。
- **剩余生存时间（ Remaining Lifetime ）**：LSP 的剩余存活时间，单位是秒。
- **LSP 标识符（ LSP ID ）**：三部分组成：设备的系统 ID 、伪节点 ID 、分配编号，用于标识不同的 LSP 。
- **序列号（ Sequence Number ）**：LSP 的序列号，用于区分 LSP 的新旧。
- **校验和（ Checksum ）**：校验和。
- **P（ Partition Repair ）**：如果设备支持区域划分修复特性，那么生成的 LSP 中 P 值设为 1 。
- **ATT（ Attached bits ）**：关联位，Level-1-2 路由器，既连着 Level-1 区域，又连着 Level-2 区域，生成的 Level-1 LSP 中 ATT 值为 1 。
- **OL（ Overload bit ）**：过载位，通常 IS-IS 设备生成的 LSP 中 OL 值为 0 。值为 1 时，表示设备已经过载，只计算生成 LSP 设备的直连路由，不计算其它网段的路由。
- **IS 类型（ IS Type ）**：二进制值为 01 时，表示 Level-1 路由器。二进制值为 11 时，表示 Level-2 路由器。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661297-706d79a2-9ca7-46af-b613-b5bf6e25847f.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u8facbcff&originHeight=281&originWidth=412&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue10539c5-5840-4b18-b172-946041ea70d&title=)<br />使用 display isis lsdb 命令加上 verbose 关键字，可查看 LSP 的详细信息。R2 的输出如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661466-592ef14f-550d-49ad-b0a0-f3f570cf8eb6.png#clientId=ue9b530e4-d7dc-4&from=paste&id=udebd3e78&originHeight=1434&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6ddc8059-a585-46d4-a684-b3130e4d1ed&title=)
<a name="dSUpJ"></a>
### 网络类型
IS-IS 支持两种网络类型：**Broadcast**（广播）和 **P2P**（点对点）。IS-IS 会根据接口的数据链路层封装，决定接口的 IS-IS 网络类型。在以太网接口激活 IS-IS 时，接口类型为 Broadcast ，在广域网接口激活时，比如 PPP 或 HDLC 等，接口网络类型为 P2P 。
<a name="dmXxC"></a>
#### Broadcast 网络类型
在 Broadcast 网络中，会进行 DIS 选举，跟 OSPF 的 DR 类似。Broadcast 类型的接口上使用两种 IIH PDU ，分别是 **Level-1 LAN IIH** 和 **Level-2 LAN IIH** 。使用哪种 PDU ，取决于设备接口的 Level 。Level-1 LAN IIH 的目的 MAC 地址是组播地址 0180-c200-0014 ，Level-2 LAN IIH 的目的 MAC 地址是组播地址 0180-c200-0015 。<br />DIS 会周期性泛洪 CSNP ，确保网络中的 IS-IS 设备有一样的 LSDB 。CSNP 包含 DIS 所有 LSP 的摘要信息，使用 TLV 装载 LSP 摘要。同一个 Broadcast 网络中的其它 IS-IS 设备收到 CSNP 后，与自己的 LSDB 对比，如果一致，就忽略这条 CSNP ；如果缺少某些 LSP ，就向 DIS 发送 PSNP ，请求 LSP 的完整信息。DIS 就把对应的 LSP 发送给对方。收到 LSP 后更新到自己的 LSDB 中，无需向 DIS 进行确认。
<a name="ILZ74"></a>
#### P2P 网络类型
P2P 网络无需选举 DIS ，使用 **P2P IIH** 发现和维护 IS-IS 邻居关系。默认时，Hello 报文发送间隔是 10 秒。建立邻接关系后，开始交互 LSP 。收到 LSP 后，使用 PSNP 进行确认，告诉对方自己收到了 LSP 。如果一段时候后，对方没收到 PSNP ，就会对 LSP 进行重传。CSNP 只在邻居关系建立后，双方进行一次交互，后面不再发送。收到 CSNP 后，和自己的 LSDB 对比，相同则忽略这条 CSNP ，缺少某些 LSP ，就向邻居发送 PSNP 请求 LSP 的完整信息。
<a name="AHzAc"></a>
#### NBMA 网络类型
IS-IS 不支持 NBMA 网络类型。比如在 Frame-Relay 环境中，IS-IS 只能在接口上使用 P2P 网络类型，不支持修改成 Broadcast 。Frame-Relay 接口使用一条 PVC 连接一台设备，如果一个接口要连接多台设备，就需要使用子接口接入 NBMA 网络，当然也是 P2P 类型的子接口。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661393-a9df63c8-27db-4e10-83aa-2e84c2d99848.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u62abcc51&originHeight=278&originWidth=535&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5e790f63-396b-489b-9745-51731de048c&title=)
<a name="kRY3A"></a>
### DIS 和 伪节点
网络类型是 Broadcast 时，IS-IS 会选举 **DIS**（指定中间系统），DIS 在 LAN 中虚拟出一个**伪节点**（ Pseudonodes ），并生成**伪节点 LSP** 。<br />伪节点不是一台真实的物理设备，而是 DIS 生成的一台虚拟设备。LAN 内设备的 LSP 只需描述和伪节点的邻居关系即可，不需要描述和其它非伪节点的邻居关系。伪节点的 LSP 包括伪节点和 LAN 中所有设备的邻居关系，根据伪节点的 LSP 就能计算出 LAN 内的拓扑，DIS 生成伪节点的 LSP 。伪节点和伪节点的 LSP 让 LSP 的数量减少，当拓扑发生变化时，泛洪的 LSP 数量也减少了，设备的负担自然也减小了。<br />为了保证 LSDB 的同步，DIS 会在 LAN 内周期性泛洪 CSNP ，LAN 中设备收到后，会进行一致性检查，保证本地 LSDB 和 DIS 同步。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661523-b1537d42-8618-44c1-8d1e-df787d4ef15a.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u109290c9&originHeight=268&originWidth=591&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4037c247-9290-4119-ac84-437aeffa1d3&title=)<br />R1 、R2 、R3 、R4 连在一台交换机上，都是 Level-1 设备，属于同一个区域，就需要进行 DIS 选举。R4 和 R5 使用 P2P 链路互联，不需要选举 DIS ，也不存在伪节点。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661609-cf3640ce-81c0-4fb0-a630-3679c6092294.png#clientId=ue9b530e4-d7dc-4&from=paste&id=uf6175a8c&originHeight=351&originWidth=611&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u873e0e23-bc38-4000-a26d-c96ccd5ee1f&title=)<br />如果 R4 的 GE0/0/0 接口选举为 DIS ，那么 R4 会生成一个伪节点，并生成伪节点的 LSP 。图中有 R1 和 R4 的 LSP ，以及伪节点的 LSP 。<br />先查看 LSDB ，因为所有设备属于同一个区域，所以 LSDB 是相同的。R1 使用 display isis lsdb 命令查看：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661668-353f8a0c-1b24-4353-930f-72c7b63e494e.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u4efc969a&originHeight=726&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3cc9cc0b-6e10-4ffe-8836-9d0f64bb4c3&title=)<br />可以看到每台设备生成的 LSP ，R4 生成了两个 Level-1 LSP ，LSP ID 是 0000.0000.0004.01-00 的 LSP ，就是 R4 作为 DIS 生成的伪节点 LSP 。<br />查看 R1 生成的 LSP 详情：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661878-e626976d-f62f-40c0-b1f6-b4d0ae1ed2b4.png#clientId=ue9b530e4-d7dc-4&from=paste&id=u045ded08&originHeight=781&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7acdd2ce-0b94-4a8a-8eb9-3f3c98934c8&title=)<br />LSP 中的邻居是 0000.0000.0004.01 ，也就是伪节点，以及一个 IP 网段 10.1.1.0/24 。R2 和 R3 生成的 Level-1 LSP 也类似。实际上，每台路由器会和所有路由器建立 IS-IS 邻居关系，但是生成的 LSP 只描述自己和伪节点的邻居关系。<br />再看伪节点 LSP 的详情：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652358661892-414b10a9-ab24-452e-9254-81dcc8ec9aac.png#clientId=ue9b530e4-d7dc-4&from=paste&id=ud74a865a&originHeight=781&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u61ed4b3e-7937-411a-a085-9804e96060d&title=)<br />DIS 生成一个 Level-1 伪节点 LSP ，并描述了和 R1 、R2 、R3 、R4 的邻居关系。随着这条 LSP 的泛洪，区域内的其它设备就可以计算出 LAN 内的拓扑。<br />IS-IS 在 LAN 中**选举 DIS** 的顺序如下：

- 接口 DIS 优先级最高的设备成为 DIS 。DIS 优先级的值越大，优先级越高。
- 如果 DIS 优先级相同，那么接口 MAC 地址最大的设备称为 DIS 。

注意事项：

- 在一个 LAN 中部署 IS-IS ，LAN 中所有设备都会和 DIS 以及非 DIS 设备建立邻居关系。
- 在一个 LAN 中，Level-1 和 Level-2 的 DIS 独立选举，互不干扰。完全有可能出现一种情况：Level-1 DIS 和 Level-2 DIS 不是同一个设备。
- IS-IS 没有备份 DIS ，当 DIS 发生故障时，立即选举新的 DIS 。
- DIS 可抢占。比如一个已经选举出 DIS 的 LAN 中，新加一台优先级更高的设备，那么这台设备会抢占 DIS 的角色，成为新的 DIS 。
<a name="Sx8AG"></a>
### 结尾
看到这里，可以发现 IS-IS 的很多功能，OSPF 都有类似的。
