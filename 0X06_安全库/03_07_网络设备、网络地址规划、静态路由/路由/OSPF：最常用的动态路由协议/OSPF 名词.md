网络<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927746506-72ba266c-baec-4271-a5e0-20b4cef516e1.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=ue9fef418&originHeight=1675&originWidth=939&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u75bb6a6f-9440-4241-b2f6-f4d5c6dac83&title=)
<a name="kVe80"></a>
### Router-ID
Router-ID 用于标识 OSPF 路由器，是一个 32 位的数值，跟 IPv4 地址格式一样，比如：192.168.100.1 。连续的 OSPF 路由器组成的网络叫做 OSPF 域，域内 Router-ID 必须唯一，也就是在同一个域内不允许出现两台相同 Router-ID 的路由器。Router-ID 可以手动设置，也可以自动生成，常见的做法是把设备的 Router-ID 指定为设备的 Loopback 接口的 IP 地址。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927746446-a1d70bde-d969-43cd-9d58-4d50d4356fe0.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u99fedb79&originHeight=103&originWidth=451&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u13105d80-9abe-4137-8826-94e782a0c77&title=)
<a name="Id8D7"></a>
#### Loopback 接口
Loopback 接口也就是本地回环接口，是一种软件的、逻辑的接口，不只网络设备支持 Loopback 接口，Windows 主机或 Linux 主机也支持。根据业务需求，在网络设备上创建 Loopback 接口，并配置 IP 地址。Loopback 接口非常稳定，除非手动进行关闭或删除，否则是永远不会失效的。正因如此，Loopback 接口常用于设备网管、网络测试、网络协议应用等。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927746427-3c843d2f-4097-48fe-bb68-eeac9a13eb8e.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=ua419df02&originHeight=315&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u779e5c91-c098-4884-875a-f49b6754182&title=)
<a name="nFvsW"></a>
### OSPF 三张表
OSPF 使用三种表格确保能正常运行。
<a name="qd3QG"></a>
#### 邻居表（ Peer Table ）
在 OSPF 交互 LSA 之前，两台直连路由器需要建立 OSPF 邻居关系。当一个接口激活 OSPF 后，就会周期性的发送 OSPF Hello 报文，同时侦听 Hello 报文从而发现直连链路上的邻居。在接口上发现邻居后，邻居的信息会写入路由器的 OSPF 邻居表，随后一个邻接关系的建立过程也开始了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927746492-e6b52856-54a8-4141-89f4-06aed6fd33f9.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u7a0ee9f0&originHeight=545&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc301a550-de6b-483a-9664-fe7032b2575&title=)
<a name="AwDZq"></a>
#### 链路状态数据库（ Link-State Database ，LSDB ）
OSPF 路由器在网络中泛洪的链路状态信息，叫做 LSA（ Link-State Advertisement ，链路状态通告）。路由器搜集 LSA 并添加到自己的 LSDB 中，路由器通过 LSDB 获取网络的完整信息。OSPF 定义了多种类型的 LSA ，这些 LSA 各有用途，最终目的是让路由器知道网络的拓扑结构以及网段信息，并计算出最短路径树，从而发现到达全网各个网段的路由。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927746543-884b4974-f9ed-45a9-8cde-b876c457487f.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=ue468f478&originHeight=488&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uce7d02bf-a5e3-4eb7-97f2-cf3e3f5dcbc&title=)
<a name="djPMB"></a>
#### OSPF 路由表（ Routing Table ）
OSPF 根据 LSDB 中的数据，运行 SPF 算法，得到一棵以自己为根、无环的最短路径树，基于这棵树，OSPF 能够发现到达网络中各个网段的最佳路径，从而得到路由信息，并添加到 OSPF 路由表中。当然，这些 OSPF 路由表中的路由最终是否被添加到全局路由器，还需要经过比较路由优先级等过程。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927746747-7638d40d-5f3f-4b36-b808-e411f8816244.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u3e683698&originHeight=574&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc5fdad7f-2f07-4b88-a300-cdf9b7b6a72&title=)
<a name="WU2hJ"></a>
### 度量值
每种路由协议对度量值的定义是不同的，OSPF 使用 Cost（开销）作为路由度量值，Cost 值越小，则路径（路由）越优。每一个激活 OSPF 的接口都有一个接口的 Cost 值，值等于 100/接口带宽 Mbit/s，计算结果取整数部分，当结果小于 1 时，值取 1 。这个值也可以人为修改，修改值会直接影响 Cost 值的计算，从而影响网络中 OSPF 路由的选择。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927746843-2b4e8c05-c8c7-4d84-8de6-be9ae5eb6963.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u734f5257&originHeight=431&originWidth=601&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ued530845-84fb-4eed-a741-76deb2d1bea&title=)
<a name="DCb3N"></a>
### 报文类型及格式
OSPF 协议的报文直接使用 IP 封装，在 IP 报文头部对应的协议号是 89 。通常 OSPF 的协议报文使用组播地址作为目的 IP 地址，有两个组播 IP 地址是 OSPF 专用。<br />224.0.0.5：这个组播 IP 地址是指所有的 OSPF 路由器。<br />224.0.0.6：这个组播 IP 地址是指所有的 OSPF DR 路由器。<br />OSPF 一共定义了五种报文，各有各的用途。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927747072-cb3d823b-2281-4e1f-be06-28131b4b80f3.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u1b5f1012&originHeight=321&originWidth=721&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u01968514-1926-4441-a850-d7e265fc15d&title=)<br />路由器的接口一旦激活 OSPF ，就会开始发送 Hello 报文。Hello 报文的一个重要功能就是发现直连链路上的 OSPF 邻居。发现邻居后，就开始邻接关系的建立。这个过程中，DD 报文用于发送 LSA 的头部摘要。通过 DD 报文的交互，路由器知道了对方所有的 LSA ，而 LSR 向对方请求完整的 LSA 。LSU 对 LSR 进行回应，或者主动更新 LSA ，LSU 包含完整的 LSA 数据。LSAck 保证 OSPF 更新机制的可靠性。此外，Hello 报文负责 OSPF 邻居关系的维护，两台直连路由器形成邻接关系后，双方仍然周期性的发送 Hello 报文，告知对方自己是在线状态。<br />所有 OSPF 报文是相同的头部，这个头部的长度是 24 字节。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927747113-0539253e-b829-4ece-be8e-484072d26b6b.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=ufa007623&originHeight=361&originWidth=561&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0c8545b1-a0f5-4d46-9f01-14747203976&title=)

- **版本**（ Version ）：OSPFv2 的值为 2 。
- **类型**（ Type ）：表示 OSPF 报文的类型。值与报文类型对应关系是：1 - Hello ；2 - DD ；3 - LSR ；4 - LSU ；5 - LSAck 。
- **报文长度**（ Packet Length ）：整个 OSPF 报文的长度，单位是字节。
- **路由器 ID**（ Router Identification ）：路由器的 OSPF Router-ID 。
- **区域 ID**（ Area Identification ）：表示所属的区域 ID ，是一个 4 字节的数值。
- **校验和**（ Checksum ）：用来校验报文有效性。
- **认证类型**（ Authentication Type ）：表示报文使用的认证类型。
- **认证数据**（ Authentication Data ）：用于报文认证的内容。
<a name="YCvhP"></a>
#### Hello 报文
Hello 报文用于发现直连链路上的邻居，以及维护邻居关系。Hello 报文携带邻居关系建立的各项参数，建立邻居关系的过程中，会检查这些参数，只有参数匹配，才能正确建立邻居关系。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927747266-340c0906-a4d5-4b4b-99de-058a87fc5996.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u065fdf12&originHeight=841&originWidth=561&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u60331f5d-2bde-4440-947a-b1af3f4b74f&title=)

- **网络掩码**（ Network Mask ）：这个字段表示接口的网络掩码。如果两台路由器是通过以太网接口连接，那么直连的两个接口必须配置相同的网络掩码。如果收到的 Hello 报文中“网络掩码”字段与自己接口的不同，就忽略这个 Hello 报文，不会建立邻居关系。
- **Hello 间隔**（ Hello Interval ）：接口周期性发送 Hello 报文的时间间隔，单位是秒。两台路由器要建立邻居关系，需要接口的 Hello Interval 相同，否则邻居关系无法建立。默认情况下，OSPF 路由器在 P2P 或 Broadcast 类型的接口上，Hello 间隔是 10 秒，在 NBMA 及 P2MP 类型的接口上，Hello 间隔是 30 秒。
- **选项**（ Options ）：这个字段一共 8 比特，每个比特位都表示路由器的某个特性。路由器通过设置相应的 Options 比特位来通告自己支持某种特性或拥有某种能力。
- **路由器优先级**（ Router Priority ）：路由器优先级，也叫做 DR 优先级，用于 DR 和 BDR 的选举。默认情况下，OSPF 接口的 DR 优先级是 1 ，这个值也可以通过命令进行修改。
- **路由器失效时间**（ Router Dead Interval ）：路由器等待对方发送 Hello 报文的时间，超过这个时间就认为是路由器已离线。路由器建立邻居关系，也需要双方接口的 Router Dead Interval 相同。默认情况下，Router Dead Interval 是 Hello Interval 的 4 倍。
- **指定路由器**（ Designated Router ）：网络中 DR 的接口 IP 地址。如果值为 0.0.0.0 ，表示没有 DR ，或 DR 还未选举出来。
- **备份指定路由器**（ Backup Designated Router ）：网络中 BDR 的接口 IP 地址。如果值为 0.0.0.0 ，表示没有 BDR ，或 BDR 还未选举出来。
- **邻居**（ Neighbor ）：表示邻居的 Router-ID ，是在直连链路上发现的有效邻居，如果发现多个邻居，就包含多个邻居字段。
<a name="ifbJf"></a>
#### DD 报文
DD 报文用于描述 LSDB ，这个报文携带的是 LSDB 中 LSA 的头部数据，并非完整的 LSA 内容。互为邻居的路由器使用空的 DD 报文来协商主/从（ Master/Slave ），空的 DD 报文不包含任何 LSA 头部信息。Router-ID 更大的路由器成为 Master 路由器。<br />Master/Slave 确定后，双方开始使用 DD 报文描述各自的 LSDB ，这时的 DD 报文包含 LSDB 里的 LSA 头部信息。路由器可以使用多个 DD 报文来描述 LSDB ，为了确保 DD 报文传输的有序和可靠，Master 路由器使用 “ DD Sequence Number（ DD 序列号）” 字段主导整个 LSDB 交互过程。比如：Master 路由器发送一个 DD 序列号是 100 的 DD 报文给 Slave 路由器，Slave 收到这个报文后，才发送自己的 DD 报文，而 DD 序列号也使用 100 。Master 路由器发送下一个 DD 报文（ DD 序列号是 101 ），Slave 路由器才会发送 DD 报文。这个过程一直持续，直到 LSDB 同步完成。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927747448-04b22386-a0c2-4bf2-84ec-ef1afd62690a.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u5a9fb802&originHeight=741&originWidth=561&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ude2d6f46-3872-4935-8b4a-781b011eb33&title=)

- **接口最大传输单元**（ Interface Maximum Transmission Unit ）：接口的 MTU 。默认情况下，接口发送的 DD 报文中，无论接口实际的 MTU 值是多少，值都为 0 。
- **选项**（ Options ）：路由器支持的 OSPF 可选项。
- **I 位**（ Initial Bit ）：初始化位，协商 Master/Slave 路由器时，值为 1 ，Master/Slave 选举完成后，值为 0 。
- **M 位**（ More Bit ）：如果值为 1 ，表示后续还有 DD 报文；如果值为 0 ，表示这是最后一个 DD 报文。
- **MS 位**（ Master Bit ）：Master 路由器发送的 DD 报文中，值为 1 ，Slave 路由器则值为 0 。
- **DD 序列号**（ DD Sequence Number ）：DD 报文的序列号，在 DD 报 文交互过程中，逐次加 1 ，确保传输的有序和可靠。DD 序列号必须由 Master 路由器决定，而 Slave 路由器只能使用 Master 路由器发送的 DD 序列号来发送自己的 DD 报文。
- **LSA 头部**（ LSA Header ）：当路由器使用 DD 报文描述自己的 LSDB 时，LSA 头部信息就在这里。一个 DD 报文可能包含一条或多条 LSA 头部信息。
<a name="ExMYb"></a>
#### LSR 报文
在与 OSPF 邻居交换 DD 报文后，路由器就知道了邻居的 LSDB 摘要，向邻居发送 LSR 报文请求所需 LSA 的完整数据。LSR 报文的链路状态类型（ Link-State Type ）、链路状态 ID（ Link-State ID ）、通告路由器（ Advertising Router ）三个字段表示路由器请求的 LSA 。如果请求多个 LSA ，那么 LSR 可能包含多个三元组。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927747676-3ade3c82-b61f-4b84-87e9-dd0ed7208a5a.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u80fa301f&originHeight=601&originWidth=561&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0011e822-8775-48e1-91d7-6426e12d5fe&title=)

- **链路状态类型**（ Link-State Type ）：表示 LSA 类型。OSPF 有多种 LSA 类型，每种 LSA 描述 OSPF 网络的某个部分，使用不同的类型编号。常见的 LSA 类型值和 LSA 名称是：1 - Router LSA ，2 - Network LSA ，3 - Network Summary LSA ，4 - ASBR Summary LSA ，5 - AS External LSA 。
- **链路状态标识**（ Link-State ID ）：LSA 的标识。不同的 LSA 类型，字段的定义不同。
- **通告路由器**（ Advertising Router ）：生成这条 LSA 的路由器的 Router-ID 。
<a name="F7IbW"></a>
#### LSU 报文
路由器收到邻居发送的 LSR 后，会使用 LSU 报文进行回应，在 LSU 报文中包含请求 LSA 的完整信息，一个 LSU 报文可以包含多个 LSA 。另外，当路由器感知到网络发生变化时，也会触发 LSU 报文的泛洪，及时把网络变化通告给其它路由器。在 BMA 网络中，非 DR 、BDR 路由器向组播地址 224.0.0.6 发送 LSU 报文，而 DR 和 BDR 会侦听这个组播地址，DR 在接收 LSU 报文后向 224.0.0.5 发送 LSU 报文，从而将更新信息泛洪到整个 OSPF 区域，所有的 OSPF 路由器都会侦听 224.0.0.5 这个组播地址。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927747736-fd4d0376-7d26-48fe-8c7b-08bd69101910.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u45862c5e&originHeight=601&originWidth=561&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8d502895-478d-4221-814e-30ca3d70e7d&title=)
<a name="rdOYt"></a>
#### LSAck 报文
当一台路由器收到邻居发送的 LSU 报文时，为了确认 LSA 已经送达，需要对报文中的 LSA 进行确认，就是回复一个 LSAck 报文。LSAck 报文包含路由器确认的 LSA 头部信息。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927747816-49c379fc-ef2e-456d-8e25-7d03f3d45381.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=ufa34991d&originHeight=541&originWidth=561&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubfb1bbcd-dbdd-4227-b1d3-d38b4eb3d85&title=)
<a name="h5OUZ"></a>
### 邻接关系
OSPF 有两个概念：邻居关系和邻接关系。假如两台路由器通过网线直连，在双方互联的接口上激活 OSPF ，路由器接口开始发送和侦听 Hello 报文，通过 Hello 报文发现彼此，并确认双向通信后，就形成了邻居关系。<br />之后，两台路由器会开始交互空的 DD 报文协商 Master/Slave ，再交互包含 LSA 头部信息的 DD 报文，以便同步自己的 LSDB ，然后通过 LSR 和 LSU 报文交互双方的 LSA 。当两者的 LSDB 同步完成后，两台路由器形成对网络拓扑的一致认知，并开始独立计算路由。这时，两台路由器形成了邻接关系。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927747923-cbc49779-6a03-4ad4-b0de-d543609367ce.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=udf8f50c0&originHeight=566&originWidth=522&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue0076351-4b65-4fbc-95ff-56cb9f9e0af&title=)
<a name="sY8RZ"></a>
### 网络类型
OSPF 的许多功能或特性都是基于接口实现的，当一个接口激活 OSPF 后，这个接口会维护很多 OSPF 变量，比如：接入的区域 ID 、接口 Cost 值、DR 优先级、邻居列表、认证类型等，其中接口的网络类型（ Network-Type ）是非常重要的一个变量。OSPF 接口的网络类型跟接口的数据链路层封装有关，在不同网络类型的接口上，OSPF 的操作有所不同。
<a name="pIoLr"></a>
#### 1、点对点类型（ Point-to-Point ，P2P ）
P2P 网络是在一条链路上只能连接两台路由器的环境。典型的例子就是 PPP 链路，当两台路由器通过 PPP 链路直连时，接口的封装协议就是 PPP ，接口激活 OSPF 后，网络类型就是 P2P 。OSPF 在 P2P 网络类型中，接口以组播方式发送协议报文，组播地址是 224.0.0.5 ，报文类型包括 Hello 报文、DD 报文、LSR 报文、LSU 报文和 LSAck 报文。默认情况下，P2P 类型接口的 Hello 报文发送间隔是 10 秒。P2P 类型的网络中，不会选举 DR 和 BDR 。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927748121-c8f52ead-2952-4ee8-ae67-4539c0507e42.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u52345a70&originHeight=92&originWidth=451&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4ce0bbe5-bec4-444b-b3cd-e70be616819&title=)
<a name="h8xyX"></a>
#### 2、广播型多路访问类型（ Broadcast Multi-Access ，BMA ）
BMA 网络中可以多台路由器接入，任意两台路由器之间都能进行二层通信，一台路由器发送出去的广播数据，其它所有路由器都能收到，是一个支持广播的网络环境。以太网就是典型的 BMA 网络。当多台路由器接入到 BMA 网络时，比如多台路由器连接在同一台二层交换机上，这些路由器的接口激活 OSPF 就会开始发送组播的 Hello 报文，从而发现网络中的其它路由器。BMA 网络中，会选举 DR 和 BDR ，所有非 DR 、BDR 路由器仅与 DR 和 BDR 建立邻接关系。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927748355-ca849f0b-f9c7-4340-af22-9a5716b2580c.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u5ebfaf11&originHeight=431&originWidth=601&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2d60ae9f-96dd-4808-a258-b6cdaa22ba4&title=)<br />OSPF 在 BMA 网络中，接口以组播方式发送 Hello 报文、LSU 报文以及 LSAck 报文，单播方式发送 DD 报文及 LSR 报文。当路由器需要向 DR 和 BDR 发送 OSPF 报文时，使用 224.0.0.6 这个组播地址作为目的 IP 地址；当需要向所有的 OSPF 路由器发送报文时，使用 224.0.0.5 。默认情况下，广播类型接口的 Hello 报文发送间隔是 10 秒。
<a name="vmhSS"></a>
#### 3、非广播型多路访问类型（ Non-Broadcast Multi-Access ，NBMA ）
NBMA 网络也允许多台路由器接入，但是不具备广播能力，这时组播发送的 Hello 报文在 NBMA 网络中可能会有问题。为了让 OSPF 路由器之间能够顺利发现彼此，并正确建立邻接关系，还需要手动配置，比如使用单播方式发送 OSPF 报文等。帧中继、X.25 就是 NBMA 网络，不过现在已经几乎看不到这类型网络了。NBMA 网络中，也会进行 DR 和 BDR 选举。默认情况下，NBMA 类型接口的 Hello 报文发送间隔是 30 秒。
<a name="w5F8d"></a>
#### 4、点对多点类型（ Point-to-Multipoint ，P2MP ）
P2MP 网络中，路由器接口的数据链路层封装不会自动设置，必须手动指定。P2MP 类似将多条 P2P 链路的一头进行捆绑的网络。在 P2MP 网络中无需选举 DR 、BDR 。OSPF 在 P2MP 类型的接口上以组播方式发送 Hello 报文，以单播方式发送其它报文。默认情况下，Hello 报文发送间隔是 30 秒。<br />了解了这么多的网络类型，即使两个路由器的直连接口的网络类型不同，也能建立 OSPF 邻接关系，但是 OSPF 路由计算容易出现问题，因为网络类型会影响 LSA 对接口的描述，关系到路由器对网络拓扑的理解和路由计算。因此，OSPF 邻接的路由器，互联接口的网络类型必须一致。<br />即使在以太网中只有两台路由器，OSPF 也会选举 DR 和 BDR ，实际上没必要且浪费时间，因为从逻辑上看是点对点的连接，选举 DR 和 BDR 实在是画蛇添足。因此，为了提高 OSPF 的效率，加快邻接关系的建立过程，可以把互联接口的网络类型修改为 P2P 。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927748644-ddc84720-5166-4b2e-840d-4df33db984d3.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=ue6144acb&originHeight=312&originWidth=556&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7e8f01f4-2466-4150-9dd3-4460b3db103&title=)
<a name="uEiJK"></a>
### DR 和 BDR
多路访问（ MA ）网络是指在同一个共享介质中连接着多个设备的网络，在这个网络中，任意两台设备之间都能够进行二层通信。MA 网络有两种：一种是 BMA 网络，另一种是 NBMA 网络。典型的 BMA 网络是一台交换机连接多台路由器，如果有一个广播数据发出来，那么整个网络中的路由器都会收到。<br />在 MA 网络中，n 台路由器都两两建立邻接关系，那么就有 n(n-1)/2 个邻接关系，会消耗大量的路由器资源，增加网络中 LSA 的泛洪数量。为了优化邻接关系数量，减少不必要的协议流量，OSPF 会在每一个 MA 网络中选举一个 DR（指定路由器）和一个 BDR（备用指定路由器）。<br />既不是 DR 也不是 BDR 的路由器叫做 DROther ，MA网络中所有 DROther 只和 DR 及 BDR 建立 OSPF 邻接关系，BDR 也和 DR 建立邻接关系，DROther 之间只停留在 2-Way 状态。这样，只有 2(n-2)+1 个邻接关系，数量得到优化。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927748562-cc8772cc-3631-450c-a9f2-cf4211d7344d.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u6defe3de&originHeight=1155&originWidth=667&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u31f03309-a3af-49e8-a0ff-ae1d0e83cea&title=)<br />DR 在 LSDB 同步方面有关键性的作用，会侦听网络中的拓扑变化信息，并将变更信息通知给其它路由器。DR 会生成一种 Type-2 LSA ，这个 LSA 包含个 MA 网络中所有 OSPF 路由器的 Router-ID ，也包括 DR 自己的。BDR 会监控 DR 状态，当 DR 发生故障时就接替它的工作。<br />DR 、BDR 的选举通过 Hello 报文实现，发生在 2-Way 状态之后。Hello 报文有路由器接口的 DR 优先级，取值范围是 0 ~ 255 ，默认值为 1 ，DR 优先级为 0 的接口没有 DR 和 BDR 的选举资格。当接口激活 OSPF 后，它会查看网络中是否存在 DR ，如果有就使用已经存在的 DR ，也就是 DR 不可抢占，否则选择最高优先级的路由器成为 DR ，当优先级相等时，选择 Router-ID 最大的路由器成为 DR 。之后还会进行 BDR 的选举，选举过程与 DR 类似。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927748854-6c62f364-25bd-4b34-bec7-57296985bba6.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u59a21e29&originHeight=291&originWidth=779&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u719e6547-9150-4400-9377-99cb5ebb09a&title=)<br />需要注意的是，DR 和 BDR 是一个接口级别的概念。某台路由器是 DR ，这种说法不准确，严谨的说法是：某台路由器的某个接口在这个 MA 网络中是 DR 。在一个 MA 网络中，DR 确保接入到网络中的所有 OSPF 路由器拥有相同的 LSDB ，也就是确保这些 LSDB 的同步。DR 使用组播地址 224.0.0.5 向网络中发送 LSU 报文，所有的 OSPF 路由器会侦听这个目的地址，并与 DR 同步 LSDB 。而 DROther 感知到拓扑变化时，向 224.0.0.6 发送 LSU 报文通告这个变化，DR 和 BDR 会侦听这个组播地址。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927748948-63e46c88-b930-4ab8-a0b6-77170d5baa50.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u9c76fdd7&originHeight=488&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue5727f79-6e0c-4ab6-8ec1-0daf4f7178e&title=)
<a name="q6ax1"></a>
### 区域和多区域
连续的 OSPF 路由器组成的网络叫做 OSPF 域（ Domain ），为了保证每台路由器都能正常的计算路由，就要求域内所有的路由器同步 LSDB ，才能达到对整个 OSPF 网络的一致认知。当网络规模越来越大时，每台路由器维护的 LSDB 变得臃肿，计算庞大的 LSDB 需要消耗更多的设备资源，加重设备的负担。另外，网络拓扑的变化，引起所有域内的路由器重新计算，而域内路由无法进行汇总，每台路由器需要维护的路由表也越来越大，又是一个不能忽略的资源消耗。<br />因此，OSPF 引入了区域（ Area ）的概念。在一个大规模的网络中，会把 OSPF 域分成多个区域。某些 LSA 的泛洪只在单个区域内部，同一个区域内的路由器维护一套相同的 LSDB ，对区域内的网络有一致的认知。每个区域独立进行 SPF 计算，区域内的拓扑结构对区域外是不可见的，而且区域内部的拓扑变化通知被限制在区域内，避免对区域外部造成影响。如果一台路由器的多个接口分别接入多个不同的区域，那么它会为每个区域分别维护一套 LSDB 。多区域的设计极大程度的限制了 LSA 的泛洪，有效的把拓扑变化的影响控制在区域内，另外在区域边界路由器上可以进行路由汇总，减少网络中的路由条目数量。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927749262-7e5b034d-d0be-44fa-b1e0-9ce72d2357be.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u5a24fcee&originHeight=334&originWidth=389&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8d11283c-142a-47c1-9c9a-51baf2481be&title=)<br />OSPF 的每一个区域都由一个编号，不同的编号表示不同的区域，这个区域编号也叫做区域 ID（ Area-ID ）。区域 ID 是一个 32 位二进制数，与 IPv4 地址的格式一样，比如 Area 0.0.0.1 ，为了方便起见，也会用十进制数表示，Area 0.0.0.1 简化成 Area1 ，Area 0.0.0.255 简化成 Area255 ，Area 0.0.1.0 简化成 Area256 。<br />一个 OSPF 域中，允许存在多个区域，其中有一个中心区域，也就是骨干区域 Area0（或 Area 0.0.0.0 ）。OSPF 要求域内的所有非骨干区域（区域 ID 不是 0 的区域）必须与 Area0 相连。如果一个域内有多个区域，那么有且只有一个 Area0 ，Area0 负责在区域之间发布区域间的路由。因此，所有的 ABR（ Area Border Router ，区域边界路由器）至少有一个接口属于 Area0 ，所以 Area0 包含所有的 ABR 。有点类似星型结构，骨干区域在中间，每个非骨干区域是分支。<br />任何一个非骨干区域都必须与 Area0 相连，当网络中某个区域没有与 Area0 相连时，这个区域的路由计算就会出问题。OSPF 的区域间路由都由 Area0 中转，任何两个非骨干区域之间是不能直接交互路由的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927749539-4e58a28a-b378-469e-959a-9d2527663362.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=u86ad655b&originHeight=124&originWidth=626&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubc9d80c3-5cf3-4178-8517-44c66531959&title=)<br />解决方法是修改 OSPF 的网络设计，与 Area0 直接相连。如果不能改或改动成本大等问题，可以考虑使用 OSPF 虚链路（ Virtual Link ）。Virtual Link 是一种逻辑的链路，不是一条真实的链路。通过搭建一条 Virtual Link ，可以把原来没有与骨干区域直连的区域给连接起来。<br />另一个可能的问题是，骨干区域不连续或被分隔开。非骨干区域交互区域路由时，容易引发路由环路。因此，OSPF 要求 ABR 只能将自己直连的区域内部路由通告给 Area0 ，而不能将自己到达其它区域的域间路由通告给 Area0 。另外，ABR 可以将自己直连区域的内部路由和到达其它区域的域间路由通告给非骨干区域。这样就能规避网络规划不合理导致的路由环路。解决问题最好的办法是修改 OSPF 的规划，当然建立 Virtual Link 也可以临时解决这个问题。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927749732-bb85adea-526f-428c-b0ae-e87c30e4317c.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=ue534b5af&originHeight=124&originWidth=626&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufbc193ab-a004-47e5-be50-a608555adb5&title=)<br />实际部署中，Virtual Link 并不是一种常规的技术，而是一种临时方案，合理的 OSPF 网络规划依然是一个最佳的选择。
<a name="iGbCX"></a>
### OSPF 路由器角色
在 OSPF 中，有多种路由器角色，在 OSPF 网络中都发挥着不同的作用。实际上，OSPF 不仅在路由器上使用，许多交换机、防火墙，甚至 Linux 主机都能实现。这里说的 OSPF 路由器，实际上是以路由器为代表。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647927749723-e78bf2fc-ff3c-452c-bb65-b7927f544aa3.png#clientId=u8dfe1fd7-47ea-4&from=paste&id=uf17dfc51&originHeight=471&originWidth=331&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua928f405-cb91-49aa-9961-43560f8240e&title=)

- 内部路由器（ Internal Router ，IR ）：所有接口都在同一个 OSPF 区域内的路由器。图中 R1 、R4 、R5 是 IR 。
- 区域边界路由器（ Area Border Router ，ABR ）：接入多个区域的路由器，并非所有接入多个区域的路由器都是 ABR 。它至少有一个接口在 Area0 中，同时还有其它接口在其它区域中。ABR 负责在区域之间传递路由信息，因此必须连接到 Area0 ，同时连接着其它区域。图中 R2 、R3 是 ABR 。
- 骨干路由器（ Backbone Router ，BR ）：至少有一个接口接入 Area0 的路由器，那它就是一台骨干路由器，另外 ABR 也是骨干路由器。图中 R1 、R2 、R3 、R6 是 BR 。
- AS 边界路由器（ AS Boundary Router ，ASBR ）：工作在 OSPF 自治系统（ Autonomous System ，AS ）边界的路由器。ASBR 将 OSPF 域外的路由引入到本域，外部路由在整个 OSPF 域内传递。并不是同时运行多种路由协议的路由器就一定是 ASBR ，ASBR 一定是将外部路由重分发到 OSPF ，或者执行了路由重分发操作的路由器。图中 R6 是 ASBR 。
