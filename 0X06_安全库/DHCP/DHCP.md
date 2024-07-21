网络 DHCP<br />![2021-05-17-21-17-08-599240.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257605428-1dc90ab1-f615-4829-9cdf-22b0d48db4ad.png#clientId=u95b07839-3711-4&from=ui&id=u8ee71be1&originHeight=1118&originWidth=602&originalType=binary&size=63776&status=done&style=shadow&taskId=u4b608d87-da5f-462f-b524-699b49b80b3)
<a name="hKjUG"></a>
## DHCP 出现
电脑或手机需要**IP 地址**才能上网。大刘有两台电脑和两台手机，小美有一台笔记本电脑、一台平板电脑和两台手机，老王、阿丽、敏敏也有几台终端设备。如果为每台设备**手动配置**IP 地址，那会非常繁琐，一点儿也不方便。特别是手机、笔记本电脑、平板电脑等设备，每移动到一个新的地方，接入不同的网络，都要重新设置 IP 地址，实在是**太麻烦**了。<br />![2021-05-17-21-17-08-734841.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257616008-79c600b8-4db9-4252-a9b9-94a2c21f7b44.png#clientId=u95b07839-3711-4&from=ui&id=u3d6b9ce0&originHeight=601&originWidth=556&originalType=binary&size=27523&status=done&style=shadow&taskId=uf8f30d6a-e9e1-47f6-92e3-ff5a44e940f)<br />手动配置<br />于是就有了**DHCP**协议，会**自动配置**设备的网络参数，包括 IP 地址、子网掩码、网关地址、DNS 服务器等，替代手动配置。还能统一 IP 地址分配，方便网络管理。<br />![2021-05-17-21-17-08-911920.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257624555-78786240-d16b-44a4-9b9a-61d92e35c03c.png#clientId=u95b07839-3711-4&from=ui&id=u8896b60b&originHeight=133&originWidth=521&originalType=binary&size=15628&status=done&style=shadow&taskId=ub4fb3bf2-cb4c-44a0-9417-7e63e1b2fa3)<br />DHCP动态获取
<a name="NIVhv"></a>
## DHCP 简介
**DHCP**协议是从**BOOTP**协议发展而来。但 BOOTP 运行在相对静态的环境中，每台设备配置专门的 BOOTP 参数文件，该文件会在相当长的时间内保持不变。DHCP 从以下两方面对 BOOTP 进行了扩展：

- DHCP 允许设备**动态地获取**IP 地址，而不是静态指定每台主机地址。
- DHCP 能够分配其它的配置参数，客户端仅用一个消息就获取它所需要的**所有配置信息**。

![2021-05-17-21-17-09-019628.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257633532-6f04ed8a-19dc-4c5f-b658-1f16eebf8b7f.png#clientId=u95b07839-3711-4&from=ui&id=u7a608fbc&originHeight=201&originWidth=330&originalType=binary&size=12702&status=done&style=shadow&taskId=u74de1ce7-b271-4fbf-a474-2fb79b027e9)<br />动态分配<br />大刘他们的设备使用 DHCP 功能后，只要连接到网络，就可以进行 TCP/IP 通信。对于**路由器**和**交换机**，通常是手动配置 IP 地址等参数。<br />DHCP 是一种**Client/Server 模式**的网络协议，由 DHCP Client 向 DHCP Server 提出配置申请，DHCP Server 返回为 DHCP Client 分配的配置信息。这里的 Client 和 Server 是**应用程序**，可以运行在电脑、服务器、路由器等设备上。<br />![2021-05-17-21-17-09-126343.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257648646-084c0e14-3cb6-429b-8d87-1ccdc69c8023.png#clientId=u95b07839-3711-4&from=ui&id=u060d9fdf&originHeight=138&originWidth=501&originalType=binary&size=12205&status=done&style=shadow&taskId=ubc48d8a8-64e7-4fc8-a4e3-34b5e84fc01)<br />DHCP应用程序<br />**举个栗子**：<br />大刘的电脑开机后，自动运行 DHCP Client ，DHCP Client 主动向其它设备上的 DHCP Server 提出请求，DHCP Server 根据预先配置的策略，返回相应 IP 配置信息，DHCP Client 使用获得的 IP 配置信息与其它设备进行通信。<br />![2021-05-17-21-17-09-273947.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257657489-76391ee2-4e4d-42a3-afa5-648ae7bb2f2a.png#clientId=u95b07839-3711-4&from=ui&id=udf81d270&originHeight=412&originWidth=507&originalType=binary&size=24351&status=done&style=shadow&taskId=ucdcfd337-8de8-4cc8-ad49-2995f8d3dad)<br />举个栗子
<a name="rbE5i"></a>
## DHCP 分配机制
DHCP 提供了两种地址分配机制，可以根据网络需求为不同的 Client 选择不同的分配策略。

- **动态分配机制**：通过 DHCP 为 Client 分配一个有**使用期限**的 IP 地址。如果 Client 没有及时续约，到达使用期限后，这个地址可能会被其它 Client 使用。绝大多数 Client 使用的都是这种动态分配的地址。
- **静态分配机制**：通过 DHCP 为特定的 Client 分配**固定**的 IP 地址。固定 IP 地址可以**永久使用**， Client 通常是打印机、服务器等设备。

![2021-05-17-21-17-10-443273.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257668612-4dfda1a1-ed41-4e9b-a4a2-d3bb74ace726.png#clientId=u95b07839-3711-4&from=ui&id=u87778258&originHeight=561&originWidth=422&originalType=binary&size=25513&status=done&style=shadow&taskId=u6da78989-e8b8-492e-97dd-3066a470aff)<br />动态和静态分配<br />在实际情况中，发现 DHCP Client 重启后，也能获得相同的 IP 地址。DHCP Server 为 DHCP Client**分配 IP 地址**时，采用如下的**顺序**：

1. DHCP Server 中与 DHCP Client 的 MAC 地址**静态绑定**的 IP 地址；
2. DHCP Client**曾经使用过**的 IP 地址；
3. 最先找到的可用 IP 地址。

如果没找到可用的 IP 地址，就依次查询**超过租期**、**发生冲突**的 IP 地址，如果找到就进行分配，否则**报错**处理。<br />![2021-05-17-21-17-10-563930.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257683057-f1aad47a-4e09-4549-8f85-0d0e1b7d6ec2.png#clientId=u95b07839-3711-4&from=ui&id=u1eb598f1&originHeight=541&originWidth=181&originalType=binary&size=13845&status=done&style=shadow&taskId=u550f0f79-3afa-4858-a8ea-202c50f501a)<br />IP地址分配顺序
<a name="sYXo6"></a>
## DHCP 系统组成
DHCP 系统由**DHCP Server**（ DHCP 服务器）、**DHCP Client**（ DHCP 客户端）、**DHCP Relay**（ DHCP 中继）等组成。<br />![2021-05-17-21-17-10-696575.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257693154-62fdf928-ee57-46e6-9d2c-78878f8fbcdc.png#clientId=u95b07839-3711-4&from=ui&id=u844ff9aa&originHeight=452&originWidth=656&originalType=binary&size=20322&status=done&style=shadow&taskId=ud71e923e-d1b7-4c86-a8fa-902ea7cc847)<br />DHCP系统组成

- **DHCP Server**DHCP Server 提供**网络参数**给 DHCP Client ，通常是一台提供 DHCP 服务功能的服务器或网络设备（路由器或三层交换机）。比如：家里用的**无线路由器**。

![2021-05-17-21-17-10-805285.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257705719-31b3abc7-d93b-4fa8-84a0-4d98ba44901e.png#clientId=u95b07839-3711-4&from=ui&id=u3f26f136&originHeight=87&originWidth=102&originalType=binary&size=2942&status=done&style=shadow&taskId=uc00612cc-2df7-43fe-bf1d-f9460d540c3)<br />DHCP Server

- **DHCP Client**DHCP Client 通过 DHCP Server**获取网络参数**，通常是一台主机或网络设备。比如：大刘的**电脑**、小美的**手机**。

![2021-05-17-21-17-11-041652.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257716115-dfa22731-b9d6-4b9e-8c04-0264f31db0f7.png#clientId=u95b07839-3711-4&from=ui&id=uee8e2b1f&originHeight=252&originWidth=202&originalType=binary&size=4810&status=done&style=shadow&taskId=u89571266-ab3a-4e81-bd5c-2758bc863f0)<br />DHCP Client

- **DHCP Relay**通常情况下，DHCP 采用广播方式实现报文交互，DHCP 服务仅限在本地网段使用。如果需要**跨网段**实现 DHCP ，那么使用**DHCP Relay**技术实现。在 DHCP Server 和 DHCP Client 之间转发跨网段 DHCP 报文的设备，通常是**三层网络设备**。

![2021-05-17-21-17-11-144383.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257728859-e05d44ea-df66-43a9-95dc-b5b91ac15287.png#clientId=u95b07839-3711-4&from=ui&id=u45c2a9e9&originHeight=142&originWidth=492&originalType=binary&size=11628&status=done&style=shadow&taskId=u9704636e-4075-4995-848b-a618b5d16b4)<br />DHCP Relay
<a name="AtJga"></a>
## DHCP 基本流程
DHCP 协议报文采用**UDP**方式封装，DHCP Server 侦听的端口号是**67**，DHCP Client 的端口号是**68**。DHCP 设备通过发送和接收 UDP 67 和 UPD 68 端口的报文进行协议交互。DHCP 的基本工作流程分为**4**个阶段，即**发现阶段**、**提供阶段**、**请求阶段**、**确认阶段**。假设大刘的 PC 是一台**新电脑**，下面将描述 PC**第一次**是如何通过 DHCP 获取 IP 地址的。<br />![2021-05-17-21-17-11-293979.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257741709-3ed03998-4a87-42e3-bde0-6ffdfa752e90.png#clientId=u95b07839-3711-4&from=ui&id=ubc397bfc&originHeight=552&originWidth=601&originalType=binary&size=47283&status=done&style=shadow&taskId=u5843e86b-0a08-46b9-85e3-2296aca9980)<br />PC首次获取地址的流程<br />为方便描述，**DHCP Server**简称**小 S**，**DHCP Client**简称**小 C**。
<a name="Qd6oB"></a>
### 1、发现阶段
**小 C**在本地网段中**广播**一个**DHCP Discover**报文，目的寻找能够分配 IP 地址的**小 S**。<br />![2021-05-17-21-17-11-414166.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257755855-048615a7-c583-485c-a4d5-ff57441a9240.png#clientId=u95b07839-3711-4&from=ui&id=u53cf441f&originHeight=417&originWidth=711&originalType=binary&size=37047&status=done&style=shadow&taskId=ua06e1c92-e2f8-4053-9505-ad4a310fd3c)<br />Discover报文
<a name="yaf6w"></a>
### 2、提供阶段
本地网段的**小 S**收到 DHCP Discover 报文后，回应**DHCP Offer**报文。DHCP Offer 报文包含了可用**IP 地址**和其它**网络参数**。<br />![2021-05-17-21-17-11-591692.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257765681-6531ebca-4bc1-4784-a394-35b6707e4cf3.png#clientId=u95b07839-3711-4&from=ui&id=u8e8a09aa&originHeight=417&originWidth=711&originalType=binary&size=37433&status=done&style=shadow&taskId=u28464513-e06e-4703-9fef-93750b36153)<br />Offer报文<br />小 C 通过**对比**Discover 报文和 Offer 报文中的**xid 字段**是否相同，来判断 Offer 报文是不是发给自己的。
<a name="TOKgh"></a>
### 3、请求阶段
小 C 会收到 小 S 发送的 DHCP Offer 报文。如果有**多个 小 S**，那么每个 小 S 都会回应 DHCP Offer 报文。通常 小 C 会选择**最先收到**的 Offer 报文，并广播**DHCP Request**报文来表明哪个 小 S 被选择，其余 小 S 就凉凉了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1621257588427-a0c7ece2-fc15-4744-9501-7d35d5e65bb7.webp#clientId=u95b07839-3711-4&from=paste&height=47&id=u62534355&originHeight=140&originWidth=140&originalType=url&status=done&style=none&taskId=ub580ee93-a575-4a51-bafd-170d423edca&width=46.666666666666664)<br />![2021-05-17-21-17-11-787168.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257777672-00eb30e6-769d-49d6-a2e0-49b51f2de931.png#clientId=u95b07839-3711-4&from=ui&id=u8fdd02d3&originHeight=417&originWidth=711&originalType=binary&size=37362&status=done&style=shadow&taskId=u0675f636-3775-417a-81ce-e2179fce645)<br />Request报文<br />如果 小 C 在一定时间后**一直没收到**DHCP Offer 报文，那么它就会重新发送 DHCP Discover 报文。
<a name="T9ZZv"></a>
### 4、确认阶段
**小 S**收到 DHCP Request 广播报文后，发送**DHCP Ack**报文作为回应，其中包含 小 C 的网络参数。DHCP Ack 报文和之前 DHCP Offer 报文的**参数**不能有**冲突**，否则 小 S 会回应一个**DHCP Nak**报文。<br />![2021-05-17-21-17-11-942752.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257787903-8692ebdf-fb69-46b8-88bc-c6716cd0b000.png#clientId=u95b07839-3711-4&from=ui&id=u72b74026&originHeight=417&originWidth=711&originalType=binary&size=37943&status=done&style=shadow&taskId=ubd1e7718-5d29-43dd-9345-6722646eb68)<br />Ack报文<br />当 小 C 收到 DHCP Ack 报文后，会发送**免费 ARP**报文进行探测，目的地址为获得的 IP 地址，如果探测此地址没有被使用，那么 小 C 就会使用这个地址，并完成配置。
<a name="jITgH"></a>
## DHCP 租期
从 DHCP 协议上看，**小 S**才有 IP 地址的**所有权**，而**小 C**只有 IP 地址的**使用权**。小 S 每次给 小 C 分配一个 IP 地址时，会约定一个 IP 地址的**租期**，通常是 24 小时。在租期内，小 C 才能使用相应的 IP 地址。当租期**到期后**，小 C 将**不能继续使用**这个 IP 地址。当然了，在租期还没到期的时候，小 C 是可以**申请续租**这个 IP 地址的。<br />**T1**时刻是租期到**一半**的时候，**T2**时刻是租期到**87.5%**的时候。在 T1 时刻 小 C 会**单播**一个**DHCP Request**报文给 小 S ，**请求续租**IP 地址。如果 小 C 收到了**DHCP Ack**回应报文，则说明**续租成功**。<br />如果直到**T2**时刻，小 C 都未收到 DHCP Ack 回应报文，那么会**广播**发送一个 DHCP Request 报文，继续请求续租 IP 地址。如果 小 C 收到了 DHCP Ack 回应报文，则说明续租成功。<br />如果直到**租期到期**， 小 C 都未收到 DHCP Ack 回应报文，那么必须**停止使用**原来的 IP 地址。小 C 将从发现阶段开始，**重新来申请**一个 IP 地址。<br />![2021-05-17-21-17-12-075381.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257801998-b85fd457-7eed-48c9-b1c4-1cd2f3e5daf1.png#clientId=u95b07839-3711-4&from=ui&id=u911a399a&originHeight=472&originWidth=581&originalType=binary&size=27998&status=done&style=shadow&taskId=u9401b462-fef0-4a00-8f16-48fd286fd4c)<br />续租流程
<a name="nLcoB"></a>
## DHCP Relay
动态获取 IP 地址的过程中，使用**广播**方式发生报文，因此 DHCP 只适用于 小 C 和 小 S 在**同一个子网**内的情况。如果为每个网段配置一个 小 S ，这显然太浪费了。<br />实际上还有**DHCP Relay**这种角色。小 C 通过 DHCP Relay 实现**跨网段**与 小 S 通信，获取 IP 地址。这样，多个子网上的 小 C 可以使用同一个 小 S ，既节省成本，又方便集中管理。<br />![2021-05-17-21-17-12-251908.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257825421-6a5ba7a9-f7b2-4375-8797-a1d075775e97.png#clientId=u95b07839-3711-4&from=ui&id=u1fa858e6&originHeight=102&originWidth=586&originalType=binary&size=11131&status=done&style=shadow&taskId=u8f96db77-fabd-4402-8f87-533c0caeec3)<br />DHCP Relay<br />DHCP Relay 的**工作原理**如下：

1. 小 C 发送 DHCP Discover 或 DHCP Request**广播报文**，具有 DHCP Relay 功能的网络设备收到后，根据配置将报文**单播**给指定的 小 S ；
2. 小 S 进行 IP 地址的分配，**单播**发送给 DHCP Relay ，DHCP Relay 再将配置信息**广播**给 小 C ，完成对 小 C 的动态配置。

![2021-05-17-21-17-12-373581.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257835085-536a41bf-0d16-4b15-8368-8b3c5fdc46b5.png#clientId=u95b07839-3711-4&from=ui&id=u95fba4e4&originHeight=595&originWidth=746&originalType=binary&size=69083&status=done&style=shadow&taskId=ueab357e6-847f-4005-9848-b1255e6b6fa)<br />Relay工作原理
<a name="SCsCx"></a>
## DHCP 协议报文
前面的内容有提到 DHCP 的一些**报文类型**，现在讲讲 DHCP 主要的**8 种**报文类型。常见的 5 种报文类型有：**DHCP Discover**、**DHCP Offer**、**DHCP Request**、**DHCP Ack**和**DHCP Release**，用得少的 3 种报文类型有：**DHCP Nak**、**DHCP Decline**和**DHCP Inform**。

- **DHCP Discover 报文**它是 DHCP Client 首次接入网络，DHCP 交互过程的**第一个报文**，用来寻找 DHCP Server的请求报文。
- **DHCP Offer 报文**它是 DHCP Server 用来回应  DHCP Discover 报文的，并携带**网络参数**，包括：IP 地址、子网掩码、默认网关、DNS 服务器等。
- **DHCP Request 报文**它是 DHCP Client 发送的报文，有三种使用场景：
- 根据策略**选择相应的 DHCP Server**，并回应 DHCP Offer 报文；
- DHCP Client 非首次接入网络，直接发送 DHCP Request 报文来**申请之前使用过**的 IP 地址等参数；
- 当 IP 地址的租约到期后，发送 DHCP Request 进行**租期更新**。
- **DHCP Ack 报文**它是 DHCP Server 对 DHCP Request 报文的回应报文，进行**最终确认**。DHCP Client 收到这个报文后，才能获得 IP 地址和相应网络参数。
- **DHCP Nak 报文**它也是 DHCP Server 对 DHCP Request 报文的**回应报文**，当 DHCP Request 报文中的各个参数都正确时，回应**DHCP Ack 报文**，否则回应**DHCP Nak 报文**，告诉 DHCP Client 禁止使用获得的 IP 地址。
- **DHCP Decline 报文**当 DHCP Client 收到 DHCP Ack 报文后，还会发送**免费 ARP**报文，确认申请的 IP 地址是否已经在网络上使用了。如果 IP 地址已经被其它 Client 使用，那么 DHCP Client 发送 DHCP Decline 报文，**拒绝**分配的 IP 地址，并重新向 DHCP Server 申请地址。
- **DHCP Release 报文**当 DHCP Client 想要**释放获得的 IP 地址**时，向 DHCP Server 发送 DHCP Release 报文，DHCP Server 收到报文后，可将这个 IP 地址分配给其它的 Client 。
- **DHCP Inform 报文**DHCP Client 通过手动方式获得 IP 地址后，还想向 DHCP Server**获取更多网络参数**时，比如：默认网关地址、DNS 服务器地址，DHCP Client 就向 DHCP Server 发送 DHCP Inform 报文进行申请。
<a name="Rt84A"></a>
## DHCP 状态机
如果把**功能各异**的 8 种报文串起来，就是整个 DHCP**协议交互**流程。前面讲的 4 种阶段（发现、提供、请求、确认）不能完全展现出来，这就需要使用 DHCP 协议的**状态机**。状态指出**下一步**使用的报文类型，**状态转换**是通过报文的接收、发送或超时。下面是 Client 的状态机。<br />![2021-05-17-21-17-12-594039.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257847492-654ac2df-a708-4b09-8377-4bd8d9cc7429.png#clientId=u95b07839-3711-4&from=ui&id=u297d9f0a&originHeight=709&originWidth=629&originalType=binary&size=71502&status=done&style=shadow&taskId=udf2f0c8e-5c97-4389-b290-cea63a02c5b)<br />Client状态机<br />Client 从**INIT**状态开始，广播 DHCP Discover 报文。在**选择**状态时，它收到 DHCP Offer 报文，并决定使用哪个地址和 Server 。做出选择后，通过 DHCP Request 报文进入**请求**状态。如果分配的地址和曾使用过的地址不一致，那么回应 DHCP Nak 报文进行拒绝，并返回**INIT**状态；如果分配的地址已经被占用，那么回应 DHCP Decline 报文进行拒绝，也返回到**INIT**状态。通常是收到一个需要的地址，回应 DHCP Ack 报文，获得租期超时值 T1 和 T2 ，并进入**绑定**状态，这个时候就可以使用这个地址直到租期到期。当 T1 到期时，进入**更新**状态并进行续租申请。如果续租成功，那么可以收到 DHCP Ack 报文，并返回到**绑定**状态；如果续租不成功，那么在 T2 到期时，再次进行续租申请。如果租期最终到期，那么 Client 将禁止使用所租用的地址，并返回到**INIT**状态。
<a name="h1Vwt"></a>
## DHCP 网络实战
DHCP 协议是为解决网络问题而生，现在就来模拟实际环境，**动手操作**，验证下 DHCP 的功能。
<a name="JKEgH"></a>
### DHCP Server 示例
<a name="n76bl"></a>
#### 网络拓扑
![2021-05-17-21-17-12-695763.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257858293-1cdeb05d-5a81-4162-93d7-9339fae46dce.png#clientId=u95b07839-3711-4&from=ui&id=ubacf03c1&originHeight=205&originWidth=507&originalType=binary&size=12352&status=done&style=shadow&taskId=uc26d8ff2-541d-45b1-ad38-c57a8fe1411)<br />网络拓扑图
<a name="SCusG"></a>
#### 实验要求

- RT（路由器）配置**DHCP Server**，PC**动态获取**IP 地址等网络参数
<a name="bLHxp"></a>
#### 操作步骤
配置思路：

1. 在 RT 上开启 DHCP 功能，
2. 创建一个地址池，
3. 配置地址池的相关参数，
4. 在 RT 的接口下引用地址池，实现 DHCP Server 功能。

配置命令：<br />使用 DHCP 功能之前，先要开启 DHCP 功能。系统视图下，使用命令`**dhcp enable**`启动 DHCP 功能。<br />动态分配 IP 地址，就需要有多个可分配的 IP 地址，使用`**ip pool **_ip-pool-name_`命令来创建全局地址池，`_ip-pool-name_`表示地址池的名称。<br />在全局地址池视图下，通过命令`**network **_ip-address _[ **mask**{ _mask _| _mask-length _} ]` 配置可分配的 IP 地址段。`**mask **{ _mask _| _mask-length _}` 表示子网掩码，通常根据设备数量来确定掩码长度。<br />有了 IP 地址和子网掩码，再加上默认网关，终端设备就能网络互通了。在全局地址池视图下，使用命令`**gateway-list **_ip-address_`配置网关 IP 地址。<br />网络互通，还不能正常访问网站，需要配置 DNS 服务器，用于域名解析。在全局地址池视图下，使用`**dns-list **_ip-address_`配置 DNS 服务器的 IP 地址。<br />地址池配置中，常用的可选命令如下，可根据需求进行选择性设置。<br />`**lease **{ **day **_day _[ **hour **_hour _[ **minute **_minute _] ]  | **unlimited **}` ：配置 IP 地址的租期，默认租期是 1 天。<br />`**excluded-ip-address **_start-ip-address _[_end-ip-address_]` ：在可分配的地址池中，设置不分配的 IP 地址。比如：地址池是 192.168.100.0/24 ，可设置 192.168.100.1 - 192.168.100.10 不参与地址分配。<br />`**static-bindip-address **_ip-address _**mac-address **_mac-address_`：为 Client 固定分配一个 IP 地址。<br />配置完地址池，还需要进行引用，DHCP Server 功能就能正常使用了。在 RT 的接口下配置引用，命令为`**dhcp select global**`。<br />![2021-05-17-21-17-13-091704.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257870295-9ddd1351-f519-4e22-a88f-5524571f3af4.png#clientId=u95b07839-3711-4&from=ui&id=ub639cd68&originHeight=1047&originWidth=1080&originalType=binary&size=214296&status=done&style=none&taskId=u4adeab91-6e26-42c2-87f6-dca1f0305e2)<br />配置
<a name="o9lwQ"></a>
#### 功能验证
配置完成后，理论上是 DHCP 功能正常工作了，实际情况的话，可通过命令`**display ip pool name **_pool-name _**used**`，查看地址池的配置情况，和地址分配情况。<br />在**RT**上查看 DHCP Server 地址分配状态。<br />![2021-05-17-21-17-13-645733.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257883257-9329dd7e-2737-4ae1-a9be-1d2ee4cee818.png#clientId=u95b07839-3711-4&from=ui&id=ub41d99c4&originHeight=894&originWidth=1080&originalType=binary&size=131775&status=done&style=none&taskId=u1e28e5e0-4c39-4d98-8520-9d7af96514a)<br />查看RT的DHCP Server状态<br />同时也在**PC**上查看动态获取地址情况，进行双向验证。<br />![2021-05-17-21-17-14-247125.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257890270-54ace96a-9e29-4e6b-a5ed-1ba70b68b6d6.png#clientId=u95b07839-3711-4&from=ui&id=u08da56b8&originHeight=533&originWidth=781&originalType=binary&size=24381&status=done&style=none&taskId=u480c07f7-5a53-409a-9d84-3403f319e8e)<br />查看PC的IP配置<br />抓包还可以看到 DHCP**报文交互**的详细过程，同时也是**检验理论知识**是否正确。<br />![2021-05-17-21-17-14-078575.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257897669-260ed5b3-5bf6-482e-b09e-43b1bd6fcc8c.png#clientId=u95b07839-3711-4&from=ui&id=ucbbee6b8&originHeight=171&originWidth=977&originalType=binary&size=7080&status=done&style=none&taskId=uaaa589c8-8280-426d-830f-e82b939ea3a)<br />抓包验证<br />这里发现抓包看到的是 DHCP Offer**单播**报文，而前面介绍的时候，DHCP Offer 是**广播**报文。其实 DHCP Offer 报文有可能是单播，也有可能是广播。DHCP 在报文的标志字段有一个**广播位**，如果 Client 支持接收 Offer 单播报文，那么 Client 就会将发送报文中的广播位设为 0 ，否则为 1 。
<a name="FMJuU"></a>
### DHCP Relay 示例
<a name="rW31m"></a>
#### 网络拓扑
![2021-05-17-21-17-14-160358.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257905056-3c267174-f500-48f7-b3bb-4fa38333bc91.png#clientId=u95b07839-3711-4&from=ui&id=ud60cf066&originHeight=176&originWidth=851&originalType=binary&size=13027&status=done&style=shadow&taskId=u7235a29a-559d-43d6-b6c9-144e46f7f15)<br />网络拓扑图
<a name="V20L7"></a>
#### 实验要求

- DHCP Client 和 DHCP Server 在**不同网段**，DHCP Client 通过**DHCP Relay**获取到 IP 地址等网络参数。
<a name="uTdKz"></a>
#### 操作步骤

1. **PC**（ DHCP Client ）开启 DHCP 功能；

![2021-05-17-21-17-14-247125.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257927070-cd290242-cc0d-48a9-9468-d530287aff4a.png#clientId=u95b07839-3711-4&from=ui&id=u97d17382&originHeight=533&originWidth=781&originalType=binary&size=24381&status=done&style=none&taskId=ucb01953c-f4f4-4ba8-aee8-a1871a6a9b7)<br />PC配置

1. **RT1**（ DHCP Relay ）使用**dhcp select relay**命令开启 DHCP Relay 功能，在 G0/0/1 口下使用**dhcp relay server-ip**_ip-address_命令，配置 DHCP Server 的 IP 地址；

![2021-05-17-21-17-14-448588.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257934796-ebcf7e24-e5d5-4df1-9e10-d7565f73d763.png#clientId=u95b07839-3711-4&from=ui&id=u7077e1d3&originHeight=1077&originWidth=1080&originalType=binary&size=272683&status=done&style=none&taskId=u3b68ee66-72a2-464b-83a8-add19479437)<br />RT1配置

1. **RT2**（DHCP Server）开启 DHCP 功能，创建地址池并配置相关参数，在接口下引用地址池，实现 DHCP Server 功能。

![2021-05-17-21-17-14-678968.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257943763-197b6866-aa1a-4c41-a777-6ac0047b833e.png#clientId=u95b07839-3711-4&from=ui&id=ud89128c7&originHeight=1321&originWidth=1080&originalType=binary&size=254952&status=done&style=none&taskId=uc9204935-34a8-454f-8e42-f07f28f28db)<br />RT2配置
<a name="PDCEB"></a>
#### 功能验证
PC 端能获取到 IP 配置，并且获取的配置正确。<br />![2021-05-17-21-17-14-786680.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257957482-e3ba7806-b6e6-4cb4-8e6c-35377bde48a4.png#clientId=u95b07839-3711-4&from=ui&id=ud4767142&originHeight=305&originWidth=627&originalType=binary&size=24285&status=done&style=none&taskId=u80b5650c-7090-4fe1-b8eb-07da0fe5431)<br />PC端验证<br />在 RT1 的 G0/0/1 抓包，查看**DHCP Client**和 DHCP Relay 的报文交互过程。<br />![2021-05-17-21-17-14-861994.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257966193-4a12b43a-fa8a-43ea-9cfd-1b7682ed9198.png#clientId=u95b07839-3711-4&from=ui&id=u72fb29a9&originHeight=171&originWidth=992&originalType=binary&size=7356&status=done&style=shadow&taskId=u38223768-aa69-40d6-b4b4-42596ca1cb0)<br />RT1的G0/0/1抓包<br />在 RT1 的 G0/0/0 抓包，查看**DHCP Server**和 DHCP Relay 的报文交互过程。<br />![2021-05-17-21-17-14-962252.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257976827-61affbee-a12d-4481-8822-5823b13efd1c.png#clientId=u95b07839-3711-4&from=ui&id=u91fe22e0&originHeight=151&originWidth=1000&originalType=binary&size=7017&status=done&style=none&taskId=u209fdc5f-5dc2-4053-a8f4-e9d3c6d09ac)<br />RT1的G0/0/0抓包
<a name="uULK9"></a>
## DHCP 报文格式
如果想要更深入的了解 DHCP 协议，那就要看它的报文格式。DHCP 设备通过识别报文内容，实现协议功能。<br />![2021-05-17-21-17-15-102875.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621257987253-ce19ca1e-8d85-4c40-b3bd-d7bdbf87f1ed.png#clientId=u95b07839-3711-4&from=ui&id=uc4229f2c&originHeight=1022&originWidth=738&originalType=binary&size=45266&status=done&style=shadow&taskId=u1032eef9-1dba-49fb-a7f3-fcbd04cfc7d)<br />DHCP报文格式

- **op**（操作类型）：表示报文的格式。当值为 1 时，表示客户端的请求报文；当值为 2 时，表示服务器的响应报文。
- **htype**（硬件类型）：不同的硬件类型取不同的值，最常见的以太网，值是 1 。
- **hlen**（硬件地址长度）：表示硬件地址长度，以太网的值是 6 ，也就是 MAC 地址的长度。
- **hops**（跳数）：DHCP 报文经过的 DHCP 中继的数量。
- **xid**（交互 ID ）：DHCP 客户端取的随机值，收到 DHCP 服务器的响应报文时，查看 xid 值是否相同，来判断报文是否是发送给自己的。
- **secs**（客户端启动秒数）：记录 IP 地址的使用时间。
- **flags**（标志）：广播响应标志位，当值为 0 时，表示服务器以单播形式发送响应报文；当值为 1 时，服务器以广播形式发送响应报文。
- **ciaddr**（客户端 IP 地址）：客户端的 IP 地址，可以是分配的地址，也可以是正在使用的地址，还可以是的 0.0.0.0 。0.0.0.0 是客户端初始状态没有地址的时候，仅用于临时通信，不是有效的地址。
- **yiaddr**（你的 IP 地址）：当服务器发送响应报文时，将分配给客户端的 IP 地址填入这个字段。
- **siaddr**（服务器 IP 地址）：用来标识服务器的 IP 地址。
- **giaddr**（中继设备 IP 地址）：表示 DHCP 中继的 IP 地址，服务器通过识别这个字段来判断出客户端的网段地址，从而选择合适的地址池，为客户端分配该网段的 IP 地址。
- **chaddr**（客户端硬件地址）：用来标识客户端的硬件地址，当客户端发送广播发现报文时，这个字段就是自己的硬件地址。
- **sname**（服务器名）：可选项，DHCP 服务器填写这个字段。
- **file**（引导文件名）：可选项，DHCP 服务器填写这个字段。
- **options**（可选项）：可选项，DHCP 客户端获取网络参数，DHCP 服务器提供网络参数，都是使用的这个字段。内容有很多，例如：租期、子网掩码、默认网关地址、DNS 服务器地址等。

拿着 DHCP**报文格式**，就可以看懂抓包获取的**报文内容**。<br />DHCP Discover 报文<br />![2021-05-17-21-17-15-196619.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621258006111-e58b34b8-0d99-4a4c-9fd8-25c4e14cf7d0.png#clientId=u95b07839-3711-4&from=ui&id=u4a32ed1d&originHeight=583&originWidth=977&originalType=binary&size=18055&status=done&style=shadow&taskId=uf3150d3b-f183-4883-9920-6bf1216cd25)<br />Discover报文明细<br />DHCP Offer 报文<br />![2021-05-17-21-17-15-313307.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621258016857-8f4595ee-fc50-4f9b-846d-0ae481675e9e.png#clientId=u95b07839-3711-4&from=ui&id=u9dd629c3&originHeight=578&originWidth=981&originalType=binary&size=18818&status=done&style=shadow&taskId=ufdba045c-d592-456e-a4fa-1ed43f4f3e1)<br />Offer报文明细<br />DHCP Request 报文<br />![2021-05-17-21-17-15-408473.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621258028698-9db07509-2d0f-469b-b9c8-17666abb71c1.png#clientId=u95b07839-3711-4&from=ui&id=ubabf0ec7&originHeight=580&originWidth=972&originalType=binary&size=18684&status=done&style=shadow&taskId=u140dbb01-cc97-4ec4-9caa-933f46a9727)<br />Request报文明细<br />DHCP Ack 报文<br />![2021-05-17-21-17-15-502222.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621258040871-15106bd4-9fb9-4992-9418-9bd140a661e6.png#clientId=u95b07839-3711-4&from=ui&id=uf58bdc60&originHeight=579&originWidth=981&originalType=binary&size=18983&status=done&style=none&taskId=u5bb1c909-b6ac-4c40-ad26-fe1986c5e62)<br />Ack报文明细
