网络 NAT<br />![2021-04-28-23-30-35-397559.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624059252-414b277c-771b-46f7-955a-fa629d0f5afb.png#clientId=uaf4a90b3-8125-4&from=ui&id=u403dd415&originHeight=508&originWidth=549&originalType=binary&size=27845&status=done&style=shadow&taskId=u6dd12a99-f2c8-4078-9e51-90d0443f7e9)
<a name="alObu"></a>
## 初识 NAT
IP 地址分为公网地址和私有地址。公网地址有 IANA 统一分配，用于连接互联网；私有地址可以自由分配，用于私有网络内部通信。<br />![2021-04-28-23-30-35-506277.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624049126-f1f2ec9a-ee8f-47e6-ab4b-98ca3a06f0f7.png#clientId=uaf4a90b3-8125-4&from=ui&id=u7b3c7e8a&originHeight=282&originWidth=772&originalType=binary&size=33816&status=done&style=none&taskId=u3bb50400-fc39-4966-8aef-17ee4d62468)<br />私网和公网<br />随着互联网用户的快速增长，2019 年 11 月 25 日全球的公网 IPv4 地址已耗尽。在 IPv4 地址耗尽前，使用 NAT（ Network Address Translation ）技术解决 IPv4 地址不够用的问题，并持续至今。<br />NAT 技术的是将私有地址转换成公网地址，使私有网络中的主机可以通过少量公网地址访问互联网。<br />但 NAT只是一种过渡技术，从根本上解决问题，是采用支持更大地址空间的下一代 IP 技术，即 IPv6 协议，它提供了几乎用不完的地址空间。<br />![2021-04-28-23-30-35-606008.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624071740-654c01f2-8bab-45f6-9fdb-faddd7ae1948.png#clientId=uaf4a90b3-8125-4&from=ui&id=u3f859090&originHeight=281&originWidth=772&originalType=binary&size=38828&status=done&style=shadow&taskId=u71930167-2ebe-45a4-9d2f-026fb181871)<br />NAT技术
<a name="ODf5b"></a>
## NAT 技术
IP 地址中预留了 3 个私有地址网段，在私有网络内，可以任意使用。<br />![2021-04-28-23-30-35-706732.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624081159-d3e19be6-d0a8-443d-a2e6-8ec743988cd0.png#clientId=uaf4a90b3-8125-4&from=ui&id=u140afd8a&originHeight=321&originWidth=561&originalType=binary&size=29811&status=done&style=none&taskId=u26307ab5-6d53-4fef-b15a-afefe5a133c)<br />私有地址范围<br />其余的 IP 地址可以在互联网上使用，由 IANA 统一管理，称为公网地址。<br />![2021-04-28-23-30-35-812451.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624091107-28f06daf-c101-42de-975d-a62883466643.png#clientId=uaf4a90b3-8125-4&from=ui&id=uc0cb4617&originHeight=321&originWidth=561&originalType=binary&size=46200&status=done&style=none&taskId=u845db497-8fdb-47f7-a72d-42822eb5555)<br />公网地址范围<br />NAT 解决了 IPv4 地址不够用的问题，另外 NAT 屏蔽了私网用户真实地址，提高了私网用户的安全性。<br />典型的 NAT 组网模型，网络通常是被划分为私网和公网两部分，各自使用独立的地址空间。私网使用私有地址 10.0.0.0/24 ，而公网使用公网地址。为了让主机 A 和 B 访问互联网上的服务器 Server ，需要在网络边界部署一台 NAT 设备用于执行地址转换。NAT 设备通常是路由器或防火墙。<br />![2021-04-28-23-30-35-917173.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624099186-075324dd-bf99-4fda-8341-162265850034.png#clientId=uaf4a90b3-8125-4&from=ui&id=ucdd6c99a&originHeight=281&originWidth=772&originalType=binary&size=29561&status=done&style=shadow&taskId=u35245d5a-58b1-43af-822c-43850a82478)<br />典型NAT组网
<a name="q8Rpo"></a>
### 基本 NAT
基本 NAT 是最简单的一种地址转换方式，它只对数据包的 IP 层参数进行转换，它可分为静态 NAT 和动态 NAT 。<br />静态 NAT 是公网 IP 地址和私有 IP 地址有一对一的关系，一个公网 IP 地址对应一个私有 IP 地址，建立和维护一张静态地址映射表。<br />动态 NAT 是公网 IP 地址和私有 IP 地址有一对多的关系，同一个公网 IP 地址分配给不同的私网用户使用，使用时间必须错开。它包含一个公有 IP 地址池和一张动态地址映射表。<br />举个动态 NAT 栗子<br />私网主机 A（ 10.0.0.1 ）需要访问公网的服务器 Server（ 61.144.249.229 ），在路由器 RT 上配置 NAT ，地址池为 219.134.180.11 ~ 219.134.180.20 ，地址转换过程如下：<br />![2021-04-28-23-30-36-014908.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624110162-4174422d-51e6-4b8f-a027-6b3befc1960e.png#clientId=uaf4a90b3-8125-4&from=ui&id=ueccbe5cf&originHeight=329&originWidth=771&originalType=binary&size=40730&status=done&style=shadow&taskId=u394ede90-f265-416a-987d-33d6d32551a)<br />基本NAT

1. A 向 Server 发送报文，网关是 10.0.0.254 ，源地址是 10.0.0.1 ，目的地址是 61.144.249.229 。

![2021-04-28-23-30-36-118632.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624136662-816c46b2-4750-46f5-a743-c2ace0871c3b.png#clientId=uaf4a90b3-8125-4&from=ui&id=u4739bf47&originHeight=181&originWidth=512&originalType=binary&size=12019&status=done&style=shadow&taskId=ua7ff88f5-f0de-4d52-9f3f-a46f1ea7532)<br />A发包

2. RT 收到 IP 报文后，查找路由表，将 IP 报文转发至出接口，由于出接口上配置了 NAT ，因此 RT 需要将源地址 10.0.0.1 转换为公网地址。

![2021-04-28-23-30-36-217367.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624150597-cd55a8df-2cfe-4f9a-acc9-a41d82a43413.png#clientId=uaf4a90b3-8125-4&from=ui&id=uaa58eadd&originHeight=374&originWidth=442&originalType=binary&size=21869&status=done&style=shadow&taskId=u02145128-f0fe-4346-84bf-49874e6bdef)<br />RT收包

3. RT 从地址池中查找第一个可用的公网地址 219.134.180.11 ，用这个地址替换数据包的源地址，转换后的数据包源地址为 219.134.180.11 ，目的地址不变。同时 RT 在自己的 NAT 表中添加一个表项，记录私有地址 10.0.0.1 到 公网地址 219.134.180.11 的映射。RT 再将报文转发给目的地址 61.144.249.229 。

![2021-04-28-23-30-36-326076.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624179673-fe53a6d7-2e27-457c-8c34-4b851d1c4626.png#clientId=uaf4a90b3-8125-4&from=ui&id=uf3e4e57b&originHeight=411&originWidth=774&originalType=binary&size=31963&status=done&style=shadow&taskId=ucc9b5edf-1f38-4462-814c-51a8cee5220)<br />NAT转换

4. Server 收到报文后做相应处理。
5. Server 发送回应报文，报文的源地址是 61.144.249.229 ，目的地址是 219.134.180.11 。

![2021-04-28-23-30-36-425810.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624189618-bca0ee9e-2501-4dfe-88eb-9da34b3d50f8.png#clientId=uaf4a90b3-8125-4&from=ui&id=SSb7n&originHeight=421&originWidth=774&originalType=binary&size=33946&status=done&style=shadow&taskId=udcf06af3-b0d9-4b86-89a2-8c876601306)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/gif/396745/1619624017462-0471e715-2304-4dc2-91db-86f0e1faecf4.gif#clientId=uaf4a90b3-8125-4&from=paste&height=0&id=u0a75f686&originHeight=1&originWidth=1&originalType=url&size=70&status=done&style=none&taskId=ue143be12-6a55-4a4b-9eb6-bb005d384a8&width=0.3333333333333333)Server发回应包

6. RT 收到报文，发现报文的目的地址 219.134.180.11 在 NAT 地址池内，于是检查 NAT 表，找到对应表项后，使用私有地址 10.0.0.1 替换公网地址 219.134.180.11，转换后的报文源地址不变，目的地址为 10.0.0.1 。RT 在将报文转发给 A 。

![2021-04-28-23-30-36-527538.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624199966-ab19c101-5d6d-4b46-afa5-2b58f3bcb49b.png#clientId=uaf4a90b3-8125-4&from=ui&id=p8fqY&originHeight=411&originWidth=679&originalType=binary&size=25302&status=done&style=shadow&taskId=uc492ee92-6979-43e2-9710-bb0b3fcabd7)<br />NAT转换

7. A 收到报文，地址转换过程结束。

![2021-04-28-23-30-36-626273.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624390939-f9076c50-9ed7-400a-9609-da5e4a1c10fa.png#clientId=uaf4a90b3-8125-4&from=ui&id=u40839587&originHeight=228&originWidth=573&originalType=binary&size=12538&status=done&style=shadow&taskId=u7e644f81-3d63-446e-994e-cf86ebaa3d7)<br />A收包<br />如果 B 也要访问 Server ，则 RT 会从地址池中分配另一个可用公网地址 219.134.180.12 ，并在 NAT 表中添加一个相应的表项，记录 B 的私有地址 10.0.0.2 到公网地址 219.134.180.12 的映射关系。<br />![2021-04-28-23-30-36-730994.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624407005-d18ff435-2bbe-4fd9-b8fc-13e88eaf3a36.png#clientId=uaf4a90b3-8125-4&from=ui&id=u9be50341&originHeight=441&originWidth=735&originalType=binary&size=37336&status=done&style=shadow&taskId=uc1039928-7da2-4b92-aa16-d419c7ea73e)<br />B的NAT转换
<a name="Juw4Q"></a>
### NAPT
在基础 NAT 中，私有地址和公网地址存在一对一地址转换的对应关系，即一个公网地址同时只能分配给一个私有地址。它只解决了公网和私网的通信问题，并没有解决公网地址不足的问题。<br />![2021-04-28-23-30-36-830726.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624421350-d479a9d5-9c71-45ee-945b-32ad76abb857.png#clientId=uaf4a90b3-8125-4&from=ui&id=ud95105fe&originHeight=334&originWidth=301&originalType=binary&size=15688&status=done&style=shadow&taskId=u73c6c23a-a4cb-405b-be63-a24899de6d3)<br />NAT表<br />NAPT（ Network Address Port Translation ）对数据包的 IP 地址、协议类型、传输层端口号同时进行转换，可以明显提高公网 IP 地址的利用率。<br />![2021-04-28-23-30-36-941430.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624433629-b90724c2-cee1-4db2-ae5e-75e64a9ea388.png#clientId=uaf4a90b3-8125-4&from=ui&id=uc2cdeb01&originHeight=334&originWidth=551&originalType=binary&size=20217&status=done&style=shadow&taskId=udffaaf64-1f1c-4072-be0a-ef4bbb2be7c)<br />NAPT的NAT表<br />举个栗子<br />私网主机 A（ 10.0.0.1 ）需要访问公网的服务器 Server 的 WWW 服务（ 61.144.249.229 ），在路由器 RT 上配置 NAPT ，地址池为 219.134.180.11 ~ 219.134.180.20 ，地址转换过程如下：

1. A 向 Server 发送报文，网关是 RT（ 10.0.0.254 ），源地址和端口是 10.0.0.1:1024 ，目的地址和端口是 61.144.249.229:80 。

![2021-04-28-23-30-37-037175.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624508584-d9fbdd19-6bf4-4287-a8f6-45111e8e9d04.png#clientId=uaf4a90b3-8125-4&from=ui&id=u5963de2f&originHeight=221&originWidth=512&originalType=binary&size=15084&status=done&style=shadow&taskId=u5b7593e0-3256-4e5d-8a1c-402ce22cfd2)<br />A发包

2. RT 收到 IP 报文后，查找路由表，将 IP 报文转发至出接口，由于出接口上配置了 NAPT ，因此 RT 需要将源地址 10.0.0.1:1024 转换为公网地址和端口。
3. RT 从地址池中查找第一个可用的公网地址 219.134.180.11 ，用这个地址替换数据包的源地址，并查找这个公网地址的一个可用端口，例如 2001 ，用这个端口替换源端口。转换后的数据包源地址为 219.134.180.11:2001 ，目的地址和端口不变。同时 RT 在自己的 NAT 表中添加一个表项，记录私有地址 10.0.0.1:1024 到 公网地址 219.134.180.11:2001 的映射。RT 再将报文转发给目的地址 61.144.249.229 。

![2021-04-28-23-30-37-141894.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624545692-ea2e73e8-4813-46c2-94e4-8f098912a1db.png#clientId=uaf4a90b3-8125-4&from=ui&id=u52228d19&originHeight=491&originWidth=551&originalType=binary&size=28471&status=done&style=shadow&taskId=ue74a8f52-2122-44ba-a692-350d81eb772)<br />NAPT转换

4. Server 收到报文后做相应处理。
5. Server 发送回应报文，报文的源地址是 61.144.249.229:80 ，目的地址是 219.134.180.11:2001 。

![2021-04-28-23-30-37-240631.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624624909-a72dfd67-3b0e-4a00-abe1-2b74a3b4a58e.png#clientId=uaf4a90b3-8125-4&from=ui&id=u79e2c606&originHeight=290&originWidth=354&originalType=binary&size=19040&status=done&style=shadow&taskId=u8ed85dbf-f38f-4b22-898a-ae8c717ab66)<br />Server发回应包

6. RT 收到报文，发现报文的目的地址在 NAT 地址池内，于是检查 NAT 表，找到对应表项后，使用私有地址和端口 10.0.0.1:1024 替换公网地址 219.134.180.11:2001，转换后的报文源地址和端口不变，目的地址和端口为 10.0.0.1:1024 。RT 再将报文转发给 A 。

![2021-04-28-23-30-37-345350.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624634267-5c2b3eae-2b89-48d6-8b9f-d31f60d11dd9.png#clientId=uaf4a90b3-8125-4&from=ui&id=u3a2beb74&originHeight=491&originWidth=551&originalType=binary&size=28815&status=done&style=shadow&taskId=ue3cb3119-16a2-4f8a-af14-d684dbed1ae)<br />NAPT转换

7. A 收到报文，地址转换过程结束。

如果 B 也要访问 Server ，则 RT 会从地址池中分配同一个公网地址 219.134.180.11 ，但分配另一个端口 3001 ，并在 NAT 表中添加一个相应的表项，记录 B 的私有地址 10.0.0.2:1024 到公网地址 219.134.180.12:3001 的映射关系。<br />![2021-04-28-23-30-37-448076.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624653687-6eea87de-4062-4c78-8683-1725eb8c9be0.png#clientId=uaf4a90b3-8125-4&from=ui&id=ua29969d7&originHeight=502&originWidth=735&originalType=binary&size=43053&status=done&style=shadow&taskId=u81fa95a8-4333-4703-934d-dcc1ad611f8)<br />B的NAPT转换
<a name="Jc7OM"></a>
### Easy IP
在标准的 NAPT 配置中需要创建公网地址池，也就是必须先知道公网 IP 地址的范围。而在拨号接入的上网方式中，公网 IP 地址是有运营商动态分配的，无法事先确定，标准的 NAPT 无法做地址转换。要解决这个问题，就要使用 Easy IP 。<br />Easy IP 又称为基于接口的地址转换。在地址转换时，Easy IP 的工作原理与 NAPT 相同，对数据包的 IP 地址、协议类型、传输层端口号同时进行转换。但 Easy IP 直接使用公网接口的 IP 地址作为转换后的源地址。Easy IP 适用于拨号接入互联网，动态获取公网 IP 地址的场合。<br />![2021-04-28-23-30-37-564764.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624688641-d55b4f7f-6266-4702-8890-e5e9d53681ec.png#clientId=uaf4a90b3-8125-4&from=ui&id=udb4a058f&originHeight=281&originWidth=771&originalType=binary&size=30708&status=done&style=shadow&taskId=u038b34ab-c2c6-410e-bb0a-f2337627692)<br />Easy IP<br />Easy IP 无需配置地址池，只需要配置一个 ACL（访问控制列表），用来指定需要进行 NAT 转换的私有 IP 地址范围。
<a name="v6AOC"></a>
### NAT Server
从基本 NAT 和 NAPT 的工作原理可知，NAT 表项由私网主机主动向公网主机发起访问而生成，公网主机无法主动向私网主机发起连接。因此 NAT 隐藏了内部网络结构，具有屏蔽主机的作用。但是在实际应用中，内网网络可能需要对外提供服务，例如 Web 服务，常规的 NAT 就无法满足需求了。<br />为了满足公网用户访问私网内部服务器的需求，需要使用 NAT Server 功能，将私网地址和端口静态映射成公网地址和端口，供公网用户访问。<br />![2021-04-28-23-30-37-671478.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624699360-35b018ff-9fa2-46ad-bf77-53adf3872358.png#clientId=uaf4a90b3-8125-4&from=ui&id=ub88a6998&originHeight=471&originWidth=771&originalType=binary&size=40296&status=done&style=shadow&taskId=u82466e0c-836c-4006-97c2-9bf1882b37d)<br />NAT Server<br />举个栗子<br />A 的私网地址为 10.0.0.1 ，端口 8080 提供 Web 服务，在对公网提供 Web 服务时，要求端口号为 80 。在 NAT 设备上启动 NAT Server 功能，将私网 IP 地址和端口 10.0.0.1:8080 映射成公网 IP 地址和端口 219.134.180.11:80 ，这样公网主机 C 就可以通过 219.134.180.11:80 访问 A 的 Web 服务。
<a name="y2Xqz"></a>
### NAT ALG
基本 NAT 和 NAPT 只能识别并修改 IP 报文中的 IP 地址和端口号信息，无法修改报文内携带的信息，因此对于一些 IP 报文内携带网络信息的协议，例如 FTP 、DNS 、SIP 、H.323 等，是无法正确转换的。<br />ALG 能够识别应用层协议内的网络信息，在转换 IP 地址和端口号时，也会对应用层数据中的网络信息进行正确的转换。<br />举个栗子：ALG 处理 FTP 的 Active 模式<br />FTP 是一种基于 TCP 的协议，用于在客户端和服务器间传输文件。FTP 协议工作时建立 2 个通道：Control 通道和 Data 通道。Control 用于传输 FTP 控制信息，Data 通道用于传输文件数据。<br />私网 A（ 10.0.0.1 ）访问公网 Server（ 61.144.249.229 ）的 FTP 服务，在 RT 上配置 NAPT，地址池为 219.134.180.11 ~ 219.134.180.20 ，地址转换过程如下：<br />![2021-04-28-23-30-37-840027.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624712093-2a3944fe-542f-41d2-9ff0-414f7e53cf45.png#clientId=uaf4a90b3-8125-4&from=ui&id=u563b2c72&originHeight=281&originWidth=791&originalType=binary&size=31492&status=done&style=shadow&taskId=uca430e03-8480-4dde-9177-5915df6b841)<br />NAT ALG

1. A 发送到 Server 的 FTP Control 通道建立请求，报文源地址和端口为 10.0.0.1:1024 ，目的地址和端口为 61.144.249.229:21 ，携带数据是 “ IP = 10.0.0.1 port=5001 ”，即告诉 Server 自己使用 TCP 端口 5001 传输 Data。

![2021-04-28-23-30-37-948740.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624748212-e5d8e7c9-6f8f-4bf1-bf8b-7f903a5ccea7.png#clientId=uaf4a90b3-8125-4&from=ui&id=u0839b1b1&originHeight=271&originWidth=512&originalType=binary&size=17397&status=done&style=shadow&taskId=u7f3003f9-2337-4e12-bfc9-71120545cd0)<br />A发包

2. RT 收到报文，建立 10.0.0.1:1024 到 219.134.180.11:2001 的映射关系，转换源 IP 地址和 TCP 端口。根据目的端口 21 ，RT 识别出这是一个 FTP 报文，因此还要检查应用层数据，发现原始数据为 “ IP = 10.0.0.1 port=5001 ”，于是为 Data 通道 10.0.0.1:5001 建立第二个映射关系：10.0.0.1:5001 到 219.134.180.11:2002 ，转换后的报文源地址和端口为 219.134.180.11:2001 ，目的地址和端口不变，携带数据为 “ IP = 219.134.180.11 port=2002 ”。

![2021-04-28-23-30-38-052614.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624763592-5e83ef7d-dc46-4887-b1ed-ef56bcd73e28.png#clientId=uaf4a90b3-8125-4&from=ui&id=u3cd1810f&originHeight=541&originWidth=551&originalType=binary&size=35715&status=done&style=shadow&taskId=uecdeabd9-1b64-42b2-a624-b1bacdaceda)<br />NAT ALG转换

3. Server 收到报文，向 A 回应 command okay 报文，FTP Control 通道建立成功。同时 Server 根据应用层数据确定 A 的 Data 通道网络参数为 219.134.180.11:2002 。
4. A 需要从 FTP 服务器下载文件，于是发起文件请求报文。Server 收到请求后，发起 Data 通道建立请求，IP 报文的源地址和端口为 61.144.249.229:20 ，目的地址和端口为 219.134.180.11:2002，并携带 FTP 数据。

![2021-04-28-23-30-38-170263.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624779643-7708e4f2-392e-4517-bc66-6dbe8ae175c5.png#clientId=uaf4a90b3-8125-4&from=ui&id=uc78a8cba&originHeight=511&originWidth=723&originalType=binary&size=46565&status=done&style=shadow&taskId=u2ac88058-2a45-4ecb-886d-d9bf31a8eec)<br />Data通道
<a name="iHXt2"></a>
## NAT 实战
<a name="ZzWo8"></a>
### 基本 NAT 实验
<a name="bn2Uq"></a>
#### 实验拓扑图
![2021-04-28-23-30-38-349783.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624796171-57f7b2f0-3985-4078-b7e5-941b93c1b7cd.png#clientId=uaf4a90b3-8125-4&from=ui&id=u7dc0b886&originHeight=212&originWidth=805&originalType=binary&size=15935&status=done&style=shadow&taskId=ua776e492-8062-4ba1-9476-97b0ecfd7ba)<br />拓扑图
<a name="cjGcv"></a>
#### 实验要求

- ENSP 模拟器
- PC 通过公网地址访问互联网
<a name="mq8xz"></a>
#### 实验步骤

1. 根据接口 IP 地址表，配置各个设备的接口地址。

![2021-04-28-23-30-38-463479.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624823877-b3449464-30df-4684-80e1-59e8e93f3d3b.png#clientId=uaf4a90b3-8125-4&from=ui&id=u615a1b9c&originHeight=261&originWidth=671&originalType=binary&size=25859&status=done&style=shadow&taskId=u6e1c078e-bf2a-448e-a21f-ebaf15d7a60)<br />IP地址表<br />![2021-04-28-23-30-38-572197.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624833928-cc33a693-f8be-4e36-85e9-eccd2478ff9e.png#clientId=uaf4a90b3-8125-4&from=ui&id=u9886d7c0&originHeight=316&originWidth=346&originalType=binary&size=13058&status=done&style=shadow&taskId=u20f7f66e-8023-4963-8bb2-d7fdfe44ad3)<br />PC配置<br />![2021-04-28-23-30-38-761682.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624842615-f3b28062-9241-4592-8e94-d01c7f8e356a.png#clientId=uaf4a90b3-8125-4&from=ui&id=ua6e03912&originHeight=834&originWidth=1080&originalType=binary&size=196918&status=done&style=none&taskId=u9423fcc1-7853-46fa-8911-f0291e8156e)<br />RT配置<br />![2021-04-28-23-30-38-886389.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624856042-2879cc99-7330-43b3-9908-71c62cf46881.png#clientId=uaf4a90b3-8125-4&from=ui&id=u1c2eb2f4&originHeight=742&originWidth=1080&originalType=binary&size=168161&status=done&style=none&taskId=u5fc068ad-c04a-4967-bff1-0ff80874dc6)<br />ISP配置

1. 在 RT 上配置 NAT 配置。

![2021-04-28-23-30-39-095932.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624870881-b35fbab3-1130-4ae2-bb68-41090a110a69.png#clientId=uaf4a90b3-8125-4&from=ui&id=u039d8b84&originHeight=620&originWidth=1080&originalType=binary&size=107011&status=done&style=none&taskId=u950188b5-0202-4f8d-8c66-7ec605c7314)<br />NAT配置<br />配置基本 NAT 只需要一条命令：把私有 IP 地址转换成公网 IP 地址，在接口视图下配置 `nat static globalglobal-addressinsidehost-address` 命令。默认路由是网关路由器上的常见配置。使用 `display nat static` 命令查看 RT 上的静态 NAT 配置。

1. 在 PC 上验证联网功能。

![2021-04-28-23-30-39-193668.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619624998799-b0eb6260-9a4a-4c87-b46c-f2c55fce4716.png#clientId=uaf4a90b3-8125-4&from=ui&id=u5488ea4e&originHeight=365&originWidth=560&originalType=binary&size=30577&status=done&style=shadow&taskId=ud2951f05-f0e4-4f96-9b73-c60635171ec)<br />PC验证结果

1. 抓包查看 NAT 转换效果。分别抓包 RT 的内网口 G0/0/0 和外网口 G0/0/1 的报文，看出发送的 Echo Request 报文和接收的 Echo Reply 报文都有进行 NAT 转换。

![2021-04-28-23-30-39-315342.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625009362-d910d6b7-8dbe-43d9-ab67-42cc386c2213.png#clientId=uaf4a90b3-8125-4&from=ui&id=u8909ef81&originHeight=609&originWidth=800&originalType=binary&size=55555&status=done&style=shadow&taskId=u434d0c9e-bed0-4b5a-bd09-89eaac6d1b5)<br />RT内网口抓包<br />![2021-04-28-23-30-39-485243.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625019465-3282224e-3c0f-453c-bb5b-279c0d95c659.png#clientId=uaf4a90b3-8125-4&from=ui&id=u4f9a5a47&originHeight=594&originWidth=802&originalType=binary&size=55059&status=done&style=shadow&taskId=uf880a7d3-54aa-4d7b-a4ec-1cd5b1256ef)<br />RT外网口抓包
<a name="HZx9r"></a>
### NAPT 实验
<a name="D9dnB"></a>
#### 实验拓扑图
![2021-04-28-23-30-39-616891.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625038204-4fe021d5-b22b-48bc-9313-9b607245e38e.png#clientId=uaf4a90b3-8125-4&from=ui&id=ufdbfe226&originHeight=480&originWidth=811&originalType=binary&size=33756&status=done&style=shadow&taskId=u99d43ce0-3359-4bd3-96ac-fd368b3f77f)<br />拓扑图
<a name="Efcvv"></a>
#### 实验要求

- RT 使用 NAPT 功能
- ISP 分配 4 个可用的公网地址：202.0.0.3 ~ 202.0.0.6
- VLAN 10 的用户使用两个公网地址
- VLAN 20 的用户使用另外两个公网地址
<a name="GkkO0"></a>
#### 实验步骤

1. 根据接口 IP 地址表，配置各个设备的接口地址。配置命令可参考上一个实验步骤 1 。

![2021-04-28-23-30-39-716622.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625064045-5ec02655-bbab-439a-9bcc-4e25e25036bd.png#clientId=uaf4a90b3-8125-4&from=ui&id=u5d591ff3&originHeight=341&originWidth=671&originalType=binary&size=33163&status=done&style=shadow&taskId=u5d94cf51-8395-414b-a4b4-2dfb3b819aa)<br />IP地址表

1. 在 RT 上配置 NAPT 配置。

![2021-04-28-23-30-39-914094.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625076149-3a7f90cd-9e41-465d-98b8-e1198838e3b7.png#clientId=uaf4a90b3-8125-4&from=ui&id=u1735ac75&originHeight=971&originWidth=1080&originalType=binary&size=231632&status=done&style=none&taskId=uacc3ffe8-c4f8-4e62-b310-b22097060a3)<br />NAPT配置<br />在 NAPT 的配置中，使用基本 ACL 来指定私有 IP 地址范围。ACL 2010 指定 VLAN 10 的 IP 地址空间，ACL 2020 指定 VLAN 20 的 IP 地址空间。使用 `nat address-groupgroup-index start-address end-address` 命令指定公网 IP 地址范围，分别指定了两个 NAT 地址组，编号分别选择了 1 和 2 。在外网接口上，使用 `nat outboundacl-numberaddress-groupgroup-index` ，绑定 NAT 转换关系。<br />使用 `display nat address-group` 命令查看 RT 上的 NAT 地址组配置。命令 `display nat outbound` 查看出方向 NAT 的转换关系。

1. 分别在 PC10 和 PC 20 上验证上网功能。

![2021-04-28-23-30-40-008848.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625091665-709a20bd-9506-4c21-9730-b9deb034a1f9.png#clientId=uaf4a90b3-8125-4&from=ui&id=u8c7f8f63&originHeight=365&originWidth=556&originalType=binary&size=31743&status=done&style=none&taskId=u7bac2336-e0ef-44d6-b333-98b7b8d2aac)<br />PC10验证结果<br />![2021-04-28-23-30-40-109816.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625102124-22d038ac-0deb-48b1-9ad8-f2528efe2fc9.png#clientId=uaf4a90b3-8125-4&from=ui&id=u74f1dc35&originHeight=362&originWidth=556&originalType=binary&size=31792&status=done&style=none&taskId=u685de380-d52a-440f-87aa-e36c9c183b7)<br />PC20验证结果

1. 抓包查看 NAT 转换效果。分别抓包 RT 的内网口 G0/0/1 和外网口 G0/0/0 的报文，查看 VLAN 10 的用户出发送的 Echo Request 报文和接收的 Echo Reply 报文都有进行 NAT 转换。

![2021-04-28-23-30-40-239712.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625130767-e0e83ebe-7787-444b-b092-b1e42e8a4baf.png#clientId=uaf4a90b3-8125-4&from=ui&id=u34fe9d0c&originHeight=609&originWidth=803&originalType=binary&size=56683&status=done&style=shadow&taskId=ubcd0232d-d7b8-4ebe-88e0-1016a1b8a9c)<br />RT内网口VLAN10抓包<br />![2021-04-28-23-30-40-405343.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619625139692-03aabe80-8339-4251-8e05-de6ff4cab03b.png#clientId=uaf4a90b3-8125-4&from=ui&id=u7c78d120&originHeight=605&originWidth=805&originalType=binary&size=56153&status=done&style=shadow&taskId=uf218804f-000b-40a0-989c-d1d140cadb8)<br />RT外网口抓包
<a name="IVWQN"></a>
## 其它常用 NAT 命令
NAT Server 是在接口视图下配置，命令格式为：`nat server protocol { tcp | udp } globalglobal-address global-portinsidehost-address host-port` 。<br />检查 NAT Server 配置信息命令：`display nat server` 。<br />检查 NAT 会话命令：`display nat session all` 。<br />启动 NAT ALG 功能命令：`nat algallenable` 。<br />查看 NAT ALG 功能命令：`display nat alg` 。
