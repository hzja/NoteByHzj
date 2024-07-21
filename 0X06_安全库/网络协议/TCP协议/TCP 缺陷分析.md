网络协议TCP<br />TCP 通过序列号、确认应答、超时重传、流量控制、拥塞控制等方式实现了可靠传输，看起来它很完美，事实真的是这样吗？TCP 就没什么缺陷吗？<br />TCP 协议有哪些缺陷？主要有四个方面：

- 升级 TCP 的工作很困难；
- TCP 建立连接的延迟；
- TCP 存在队头阻塞问题；
- 网络迁移需要重新建立 TCP 连接；

接下来，针对这四个方面详细说一下。
<a name="p6AIM"></a>
## 升级 TCP 的工作很困难
TCP 协议是诞生在 1973 年，至今 TCP 协议依然还在实现很多的新特性。<br />但是 TCP 协议是在内核中实现的，应用程序只能使用不能修改，如果要想升级 TCP 协议，那么只能升级内核。<br />而升级内核这个工作是很麻烦的事情，麻烦的事情不是说升级内核这个操作很麻烦，而是由于内核升级涉及到底层软件和运行库的更新，服务程序就需要回归测试是否兼容新的内核版本，所以服务器的内核升级比较保守和缓慢。<br />很多 TCP 协议的新特性，都是需要客户端和服务端同时支持才能生效的，比如  TCP Fast Open 这个特性，虽然在 2013 年就被提出了，但是由于因为很多 PC 端的系统升级滞后很严重，所以一些老旧的系统是无法支持这个特性的。<br />所以，即使 TCP 有比较好的特性更新，也很难快速推广，用户往往要几年或者十年才能体验到。
<a name="lL5rx"></a>
## TCP 建立连接的延迟
基于 TCP 实现的应用协议，都是需要先建立三次握手才能进行数据传输，比如 HTTP 1.0/1.1、HTTP/2、HTTPS。<br />现在大多数网站都是使用 HTTPS 的，这意味着在 TCP 三次握手之后，还需要经过 TLS 四次握手后，才能进行 HTTP 数据的传输，这在一定程序上增加了数据传输的延迟。<br />TCP 三次握手和 TLS 握手延迟，如图：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652668437861-574efeb1-1e9d-44b1-af64-ba444075f9c9.jpeg#clientId=u2fa52bb5-fea4-4&from=paste&id=u329b88a9&originHeight=693&originWidth=777&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua32697ed-61b1-4488-9160-e393b41a0ba&title=)<br />TCP 三次握手的延迟被 TCP Fast Open （快速打开）这个特性解决了，这个特性可以在「第二次建立连接」时减少 TCP 连接建立的时延。<br />![常规 HTTP 请求 与 Fast  Open HTTP 请求](https://cdn.nlark.com/yuque/0/2022/png/396745/1652668437845-a15160bf-09f7-43fe-a820-7f27d8a80c69.png#clientId=u2fa52bb5-fea4-4&from=paste&id=u87b2ddc4&originHeight=918&originWidth=1052&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u9434d10e-8568-4f40-9e77-5e09c73eb08&title=%E5%B8%B8%E8%A7%84%20HTTP%20%E8%AF%B7%E6%B1%82%20%E4%B8%8E%20Fast%20%20Open%20HTTP%20%E8%AF%B7%E6%B1%82 "常规 HTTP 请求 与 Fast  Open HTTP 请求")<br />过程如下：

- 在第一次建立连接的时候，服务端在第二次握手产生一个 Cookie （已加密）并通过 SYN、ACK 包一起发给客户端，于是客户端就会缓存这个 Cookie，所以第一次发起 HTTP Get 请求的时候，还是需要 2 个 RTT 的时延；
- 在下次请求的时候，客户端在 SYN 包带上 Cookie 发给服务端，就提前可以跳过三次握手的过程，因为 Cookie 中维护了一些信息，服务端可以从 Cookie 获取 TCP 相关的信息，这时发起的 HTTP GET 请求就只需要 1 个 RTT 的时延；

TCP Fast Open 这个特性是不错，但是它需要服务端和客户端的操作系统同时支持才能体验到，而 TCP Fast Open 是在 2013 年提出的，所以市面上依然有很多老式的操作系统不支持，而升级操作系统是很麻烦的事情，因此  TCP Fast Open 很难被普及开来。<br />还有一点，针对 HTTPS 来说，TLS 是在应用层实现的握手，而 TCP 是在内核实现的握手，这两个握手过程是无法结合在一起的，总是得先完成 TCP 握手，才能进行 TLS 握手。<br />也正是 TCP 是在内核实现的，所以 TLS 是无法对 TCP 头部加密的，这意味着 TCP 的序列号都是明文传输，所以就存安全的问题。<br />一个典型的例子就是攻击者伪造一个的 RST 报文强制关闭一条 TCP 连接，而攻击成功的关键则是 TCP 字段里的序列号位于接收方的滑动窗口内，该报文就是合法的。<br />为此 TCP 也不得不进行三次握手来同步各自的序列号，而且初始化序列号时是采用随机的方式（不完全随机，而是随着时间流逝而线性增长，到了 2^32 尽头再回滚）来提升攻击者猜测序列号的难度，以增加安全性。<br />但是这种方式只能避免攻击者预测出合法的 RST 报文，而无法避免攻击者截获客户端的报文，然后中途伪造出合法 RST 报文的攻击的方式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652668437776-ee7b7a0e-5f84-4e66-993e-cf2b8a2d1ddb.png#clientId=u2fa52bb5-fea4-4&from=paste&id=uc256dea6&originHeight=601&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8bfa4b44-46d6-4655-bae2-faa17436e03&title=)<br />大胆想一下，如果 TCP 的序列号也能被加密，或许真的不需要三次握手了，客户端和服务端的初始序列号都从 0 开始，也就不用做同步序列号的工作了，但是要实现这个要改造整个协议栈，太过于麻烦，即使实现出来了，很多老的网络设备未必能兼容。
<a name="OSEaW"></a>
## TCP 存在队头阻塞问题
TCP 是字节流协议，**TCP 层必须保证收到的字节数据是完整且有序的**，如果序列号较低的 TCP 段在网络传输中丢失了，即使序列号较高的 TCP 段已经被接收了，应用层也无法从内核中读取到这部分数据。如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652668437857-b0d263e2-b1f9-4f07-a6bf-a7d4c49a689c.jpeg#clientId=u2fa52bb5-fea4-4&from=paste&id=u315ef479&originHeight=502&originWidth=521&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u833c006b-9d04-4d12-806d-e232d375fc5&title=)<br />图中发送方发送了很多个 packet，每个 packet 都有自己的序号，你可以认为是 TCP 的序列号，其中 packet #3 在网络中丢失了，即使 packet #4-6 被接收方收到后，由于内核中的 TCP 数据不是连续的，于是接收方的应用层就无法从内核中读取到，只有等到 packet #3 重传后，接收方的应用层才可以从内核中读取到数据。<br />这就是 TCP 队头阻塞问题，但这也不能怪 TCP ，因为只有这样做才能保证数据的有序性。<br />HTTP/2 多个请求是跑在一个 TCP 连接中的，那么当 TCP 丢包时，整个 TCP 都要等待重传，那么就会阻塞该 TCP 连接中的所有请求，所以 HTTP/2 队头阻塞问题就是因为 TCP 协议导致的。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652668437918-6199b7d5-6b90-4a0c-81d5-ec77de038368.jpeg#clientId=u2fa52bb5-fea4-4&from=paste&id=u27cea84b&originHeight=377&originWidth=1011&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueda73269-4e0f-4dbe-b53f-564e13f6bc4&title=)
<a name="oeQJx"></a>
## 网络迁移需要重新建立 TCP 连接
基于 TCP 传输协议的 HTTP 协议，由于是通过四元组（源 IP、源端口、目的 IP、目的端口）确定一条 TCP 连接。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652668438194-104fb27c-c1e0-47f3-b7ff-8b38a7331a7c.png#clientId=u2fa52bb5-fea4-4&from=paste&id=u2f2b7d9f&originHeight=228&originWidth=821&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u46ec5910-678d-4edf-97a2-c42fab8a4f8&title=)<br />那么**当移动设备的网络从 4G 切换到 WIFI 时，意味着 IP 地址变化了，那么就必须要断开连接，然后重新建立 TCP 连接**。<br />而建立连接的过程包含 TCP 三次握手和 TLS 四次握手的时延，以及 TCP 慢启动的减速过程，给用户的感觉就是网络突然卡顿了一下，因此连接的迁移成本是很高的。
<a name="MWbma"></a>
## 结尾
<a name="gCF0M"></a>
### 如何基于 UDP 协议实现可靠传输？
很多同学第一反应就会说把 TCP 可靠传输的特性（序列号、确认应答、超时重传、流量控制、拥塞控制）在应用层实现一遍。<br />实现的思路确实这样没错，但是有没有想过，**既然 TCP 天然支持可靠传输，为什么还需要基于 UDP 实现可靠传输呢？这不是重复造轮子吗？**<br />所以，要先弄清楚 TCP 协议有哪些痛点？而这些痛点是否可以在基于 UDP 协议实现的可靠传输协议中得到改进？<br />现在市面上已经有基于 UDP 实现的可靠传输协议的成熟方案了，那就是 QUIC 协议，**QUIC 协议把这里说的 TCP 的缺点都给解决了**，而且已经应用在了 HTTP/3。
