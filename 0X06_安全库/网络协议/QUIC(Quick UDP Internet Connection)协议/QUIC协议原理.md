网络协议 QUIC协议<br />QUIC，又名HTTP3，是近年来诞生的非常厉害的传输协议，它利用UDP解决了当前基于TCP协议的HTTP的许多问题，提升了在弱网环境下的网络通信体验。
<a name="EYx26"></a>
## 1、QUIC协议
<a name="rMzDZ"></a>
### 1.1 什么是QUIC？
QUIC(Quick UDP Internet Connection)是谷歌推出的一套基于UDP的传输协议，它实现了TCP + HTTPS + HTTP/2的功能，目的是保证可靠性的同时降低网络延迟。因为UDP是一个简单传输协议，基于UDP可以摆脱TCP传输确认、重传慢启动等因素，建立安全连接只需要一的个往返时间，它还实现了HTTP/2多路复用、头部压缩等功能。<br />众所周知UDP比TCP传输速度快，TCP是可靠协议，但是代价是双方确认数据而衍生的一系列消耗。其次TCP是系统内核实现的，如果升级TCP协议，就得让用户升级系统，这个的门槛比较高，而QUIC在UDP基础上由客户端自由发挥，只要有服务器能对接就可以。<br />![2021-04-30-23-17-37-960822.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619796389674-b13124a3-cd29-4626-ae28-d491cd18bdc7.jpeg#align=left&display=inline&height=300&id=ucc5816db&originHeight=300&originWidth=708&size=29783&status=done&style=shadow&width=708)<br />图1-1 HTTP与QUIC
<a name="CpYAt"></a>
### 1.2 HTTP协议发展
<a name="xQ5sF"></a>
#### 1.2.1 HTTP历史进程
◎HTTP 0.9（1991年）只支持get方法不支持请求头；<br />◎HTTP 1.0（1996年）基本成型，支持请求头、富文本、状态码、缓存、连接无法复用；<br />◎HTTP 1.1（1999年）支持连接复用、分块发送、断点续传；<br />◎HTTP 2.0（2015年）二进制分帧传输、多路复用、头部压缩、服务器推送等；<br />◎HTTP 3.0（2018年）QUIC 于2013年实现、2018年正式更名为HTTP3；
<a name="COB1Y"></a>
#### 1.2.2 HTTP1.和HTTP1.1
◎队头阻塞：下个请求必须在前一个请求返回后才能发出，导致带宽无法被充分利用，后续请求被阻塞（HTTP 1.1 尝试使用流水线（Pipelining）技术，但先天 FIFO（先进先出）机制导致当前请求的执行依赖于上一个请求执行的完成，容易引起队头阻塞，并没有从根本上解决问题）；<br />◎协议开销大：header里携带的内容过大，且不能压缩，增加了传输的成本；<br />◎单向请求：只能单向请求，客户端请求什么，服务器返回什么；<br />HTTP 1.0 和 HTTP 1.1 的区别：

| HTTP 1.0 | HTTP1.1 |
| --- | --- |
| 仅支持保持短暂的TCP连接（连接无法复用） | 默认支持长连接（请求可复用TCP连接） |
| 不支持断点续传 | 支持断点续传（通过在 Header 设置参数） |
| 前一个请求响应到达之后下一个请求才能发送，存在队头阻塞 | 优化了缓存控制策略 |
| ------ | 管道化，可以一次发送多个请求，但是响应仍是顺序返回，仍然无法解决队头阻塞的问题 |
| ------ | 新增错误状态码通知 |
| ------ | 请求消息和响应消息都支持Host头域 |

<a name="Owneo"></a>
#### 1.2.3 HTTP2
解决 HTTP1 的一些问题，但是解决不了底层 TCP 协议层面上的队头阻塞问题。<br />1.二进制传输：二进制格式传输数据解析起来比文本更高效；<br />2.多路复用：重新定义底层 http 语义映射，允许同一个连接上使用请求和响应双向数据流。同一域名只需占用一个 TCP 连接，通过数据流（Stream）以帧为基本协议单位，避免了因频繁创建连接产生的延迟，减少了内存消耗，提升了使用性能，并行请求，且慢的请求或先发送的请求不会阻塞其他请求的返回；<br />3.Header压缩：减少请求中的冗余数据，降低开销；<br />4.服务端可以主动推送：提前给客户端推送必要的资源，这样就可以相对减少一点延迟时间；<br />5.流优先级：数据传输优先级可控，使网站可以实现更灵活和强大的页面控制；<br />6.可重置：能在不中断 TCP 连接的情况下停止数据的发送；<br />缺点：HTTP 2中，多个请求在一个TCP管道中的，出现了丢包时，HTTP 2的表现反倒不如HTTP 1.1了。因为 TCP 为了保证可靠传输，有个特别的“丢包重传”机制，丢失的包必须要等待重新传输确认，HTTP 2出现丢包时，整个 TCP 都要开始等待重传，那么就会阻塞该TCP连接中的所有请求。而对于 HTTP 1.1 来说，可以开启多个 TCP 连接，出现这种情况反到只会影响其中一个连接，剩余的 TCP 连接还可以正常传输数据。
<a name="xfVqI"></a>
#### 1.2.4 HTTP3 —— HTTP Over QUIC 
HTTP 是建立在 TCP 协议之上，所有 HTTP 协议的瓶颈及其优化技巧都是基于 TCP 协议本身的特性，HTTP2 虽然实现了多路复用，底层 TCP 协议层面上的问题并没有解决（HTTP 2.0 同一域名下只需要使用一个 TCP 连接。但是如果这个连接出现了丢包，会导致整个 TCP 都要开始等待重传，后面的所有数据都被阻塞了），而 HTTP3 的 QUIC 就是为解决 HTTP2 的 TCP 问题而生。
<a name="fBB3S"></a>
## 2、QUIC的关键特性
关于 QUIC 的原理，相关介绍的文章很多，这里再列举一下 QUIC 的重要特性。这些特性是 QUIC 得以被广泛应用的关键。不同业务也可以根据业务特点利用 QUIC 的特性去做一些优化。同时，这些特性也是 QUIC 服务的切入点。
<a name="fpx4Q"></a>
### 2.1 连接迁移
<a name="IkDsp"></a>
#### 2.1.1 TCP的连接重连之痛
一条 TCP 连接是由四元组标识的（源 IP，源端口，目的 IP，目的端口）。什么叫连接迁移呢？就是当其中任何一个元素发生变化时，这条连接依然维持着，能够保持业务逻辑不中断。当然这里面主要关注的是客户端的变化，因为客户端不可控并且网络环境经常发生变化，而服务端的 IP 和端口一般都是固定的。<br />比如大家使用手机在 WIFI 和 4G 移动网络切换时，客户端的 IP 肯定会发生变化，需要重新建立和服务端的 TCP 连接。<br />又比如大家使用公共 NAT 出口时，有些连接竞争时需要重新绑定端口，导致客户端的端口发生变化，同样需要重新建立 TCP 连接。<br />所以从 TCP 连接的角度来讲，这个问题是无解的。
<a name="XydvI"></a>
#### 2.1.2 基于UDP的QUIC连接迁移实现
当用户的地址发生变化时，如 WIFI 切换到 4G 场景，基于 TCP 的 HTTP 协议无法保持连接的存活。QUIC 基于连接 ID 唯一识别连接。当源地址发生改变时，QUIC 仍然可以保证连接存活和数据正常收发。<br />那 QUIC 是如何做到连接迁移呢？很简单，QUIC是基于UDP协议的，任何一条 QUIC 连接不再以 IP 及端口四元组标识，而是以一个 64 位的随机数作为 ID 来标识，这样就算 IP 或者端口发生变化时，只要 ID 不变，这条连接依然维持着，上层业务逻辑感知不到变化，不会中断，也就不需要重连。<br />由于这个 ID 是客户端随机产生的，并且长度有 64 位，所以冲突概率非常低。<br />![2021-04-30-23-17-38-388003.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619796461363-a24fd4cd-a075-41f0-a39a-dd7a611d5b1e.jpeg#align=left&display=inline&height=522&id=ued46b9ca&originHeight=522&originWidth=720&size=32891&status=done&style=shadow&width=720)<br />图2-1 TCP 和 QUIC 在 Wi-Fi 和 cellular 切换时，唯一标识的不同情况
<a name="reQyt"></a>
### 2.2 低连接延时
<a name="dPnml"></a>
#### 2.2.1 TLS的连接时延问题
以一次简单的浏览器访问为例，在地址栏中输入https://www.abc.com，实际会产生以下动作：<br />◎DNS递归查询www.abc.com，获取地址解析的对应IP；<br />◎TCP握手，TCP三次握手需要需要1个RTT；<br />◎TLS握手，以目前应用最广泛的TLS 1.2而言，需要2个RTT。对于非首次建连，可以选择启用会话重用，则可缩小握手时间到1个RTT；<br />◎HTTP业务数据交互，假设abc.com的数据在一次交互就能取回来。那么业务数据的交互需要1个RTT；经过上面的过程分析可知，要完成一次简短的HTTPS业务数据交互，需要经历：新连接 4RTT + DNS；会话重用 3RTT + DNS。<br />所以，对于数据量小的请求而言，单一次的请求握手就占用了大量的时间，对于用户体验的影响非常大。同时，在用户网络不佳的情况下，RTT延时会变得较高，极其影响用户体验。<br />下图对比了TLS各版本与场景下的延时对比：<br />![2021-04-30-23-17-38-776966.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619796479079-cb076810-e8ce-4c9a-be08-929eec1c68be.png#align=left&display=inline&height=755&id=uc86a64b9&originHeight=755&originWidth=835&size=67176&status=done&style=shadow&width=835)<br />图2-2 TLS各个版本握手时延<br />从对比可以看到，即使用上了TLS 1.3，精简了握手过程，最快能做到0-RTT握手(首次是1-RTT)；但是对用户感知而言, 还要加上1RTT的TCP握手开销。<br />Google有提出Fastopen的方案来使得TCP非首次握手就能附带用户数据，但是由于TCP实现僵化，无法升级应用，相关RFC到现今都是experimental状态。这种分层设计带来的延时,有没有办法进一步降低呢? QUIC通过合并加密与连接管理解决了这个问题，来看看其是如何实现真正意义上的0-RTT的握手, 让与server进行第一个数据包的交互就能带上用户数据。
<a name="YH8GE"></a>
#### 2.2.2 真0-RTT的QUIC握手
QUIC 由于基于 UDP，无需 TCP 连接，在最好情况下，短连接下 QUIC 可以做到 0RTT 开启数据传输。而基于 TCP 的 HTTPS，即使在最好的 TLS1.3 的 early data 下仍然需要 1RTT 开启数据传输。而对于目前线上常见的 TLS1.2 完全握手的情况，则需要 3RTT 开启数据传输。对于 RTT 敏感的业务，QUIC 可以有效的降低连接建立延迟。<br />究其原因一方面是TCP和TLS分层设计导致的：分层的设计需要每个逻辑层次分别建立自己的连接状态。另一方面是TLS的握手阶段复杂的密钥协商机制导致的。要降低建连耗时，需要从这两方面着手。<br />QUIC具体握手过程如下：<br />1.客户端判断本地是否已有服务器的全部配置参数（证书配置信息），如果有则直接跳转到(5)，否则继续 ；<br />2.客户端向服务器发送inchoate client hello(CHLO)消息，请求服务器传输配置参数；<br />3.服务器收到CHLO，回复rejection(REJ)消息，其中包含服务器的部分配置参数；<br />4.客户端收到REJ，提取并存储服务器配置参数，跳回到(1) ；<br />5.客户端向服务器发送full client hello消息，开始正式握手，消息中包括客户端选择的公开数。此时客户端根据获取的服务器配置参数和自己选择的公开数，可以计算出初始密钥K1；<br />6.服务器收到full client hello，如果不同意连接就回复REJ，同(3)；如果同意连接，根据客户端的公开数计算出初始密钥K1，回复server hello(SHLO)消息，SHLO用初始密钥K1加密，并且其中包含服务器选择的一个临时公开数；<br />7.客户端收到服务器的回复，如果是REJ则情况同(4)；如果是SHLO，则尝试用初始密钥K1解密，提取出临时公开数；<br />8.客户端和服务器根据临时公开数和初始密钥K1，各自基于SHA-256算法推导出会话密钥K2；<br />9.双方更换为使用会话密钥K2通信，初始密钥K1此时已无用，QUIC握手过程完毕。之后会话密钥K2更新的流程与以上过程类似，只是数据包中的某些字段略有不同。<br />![2021-04-30-23-17-39-187455.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619796501593-ba8940f5-9fe8-4359-b53b-6cd9e2b97ff2.png#align=left&display=inline&height=586&id=u1b4050d7&originHeight=586&originWidth=752&size=135946&status=done&style=shadow&width=752)<br />图2-3 QUIC 0-RTT 握手
<a name="vWSzo"></a>
### 2.3 可自定义的拥塞控制
Quic使用可插拔的拥塞控制，相较于TCP，它能提供更丰富的拥塞控制信息。比如对于每一个包，不管是原始包还是重传包，都带有一个新的序列号(seq)，这使得Quic能够区分ACK是重传包还是原始包，从而避免了TCP重传模糊的问题。Quic同时还带有收到数据包与发出ACK之间的时延信息。这些信息能够帮助更精确的计算RTT。此外，Quic的ACK Frame 支持256个NACK 区间，相比于TCP的SACK(Selective Acknowledgment)更弹性化，更丰富的信息会让client和server 哪些包已经被对方收到。<br />QUIC 的传输控制不再依赖内核的拥塞控制算法，而是实现在应用层上，这意味着根据不同的业务场景，实现和配置不同的拥塞控制算法以及参数。GOOGLE 提出的 BBR 拥塞控制算法与 CUBIC 是思路完全不一样的算法，在弱网和一定丢包场景，BBR 比 CUBIC 更不敏感，性能也更好。在 QUIC 下可以根据业务随意指定拥塞控制算法和参数，甚至同一个业务的不同连接也可以使用不同的拥塞控制算法。<br />![2021-04-30-23-17-39-477196.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1619796523326-2e1452a9-a083-4e1e-bf28-f8c563c146b1.gif#align=left&display=inline&height=320&id=uc43af5ec&originHeight=320&originWidth=640&size=62414&status=done&style=shadow&width=640)<br />图2-4 BBR拥塞弱网下算法效果对比
<a name="Ty6BL"></a>
### 2.4 无队头阻塞
<a name="uK6tg"></a>
#### 2.4.1 TCP的队头阻塞问题
虽然 HTTP2 实现了多路复用，但是因为其基于面向字节流的 TCP，因此一旦丢包，将会影响多路复用下的所有请求流。QUIC 基于 UDP，在设计上就解决了队头阻塞问题。<br />TCP 队头阻塞的主要原因是数据包超时确认或丢失阻塞了当前窗口向右滑动，最容易想到的解决队头阻塞的方案是不让超时确认或丢失的数据包将当前窗口阻塞在原地。QUIC也正是采用上述方案来解决TCP 队头阻塞问题的。<br />TCP 为了保证可靠性，使用了基于字节序号的 Sequence Number 及 Ack 来确认消息的有序到达。<br />![2021-04-30-23-17-39-711255.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619796549844-cf862912-1cf7-4034-89ff-fe10ec04e48b.jpeg#align=left&display=inline&height=300&id=uc1b41f06&originHeight=300&originWidth=805&size=26343&status=done&style=shadow&width=805)<br />图2-5 HTTP2队头阻塞<br />如上图，应用层可以顺利读取stream1中的内容，但由于stream2中的第三个segment发生了丢包，TCP 为了保证数据的可靠性，需要发送端重传第 3 个 segment 才能通知应用层读取接下去的数据。所以即使stream3 stream4的内容已顺利抵达，应用层仍然无法读取，只能等待stream2中丢失的包进行重传。<br />在弱网环境下，HTTP2的队头阻塞问题在用户体验上极为糟糕。
<a name="UmtKS"></a>
#### 2.4.2 QUIC的无队头阻塞解决方案
QUIC 同样是一个可靠的协议，它使用 Packet Number 代替了 TCP 的 Sequence Number，并且每个 Packet Number 都严格递增，也就是说就算 Packet N 丢失了，重传的 Packet N 的 Packet Number 已经不是 N，而是一个比 N 大的值，比如Packet N+M。<br />QUIC 使用的Packet Number 单调递增的设计，可以让数据包不再像TCP 那样必须有序确认，QUIC 支持乱序确认，当数据包Packet N 丢失后，只要有新的已接收数据包确认，当前窗口就会继续向右滑动。待发送端获知数据包Packet N 丢失后，会将需要重传的数据包放到待发送队列，重新编号比如数据包Packet N+M 后重新发送给接收端，对重传数据包的处理跟发送新的数据包类似，这样就不会因为丢包重传将当前窗口阻塞在原地，从而解决了队头阻塞问题。那么，既然重传数据包的Packet N+M 与丢失数据包的Packet N 编号并不一致，怎么确定这两个数据包的内容一样呢？<br />QUIC使用Stream ID 来标识当前数据流属于哪个资源请求，这同时也是数据包多路复用传输到接收端后能正常组装的依据。重传的数据包Packet N+M 和丢失的数据包Packet N 单靠Stream ID 的比对一致仍然不能判断两个数据包内容一致，还需要再新增一个字段Stream Offset，标识当前数据包在当前Stream ID 中的字节偏移量。<br />有了Stream Offset 字段信息，属于同一个Stream ID 的数据包也可以乱序传输了（HTTP/2 中仅靠Stream ID 标识，要求同属于一个Stream ID 的数据帧必须有序传输），通过两个数据包的Stream ID 与 Stream Offset 都一致，就说明这两个数据包的内容一致。<br />![2021-04-30-23-17-40-707041.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619796572681-a38a6fda-4c6c-4ed5-9239-f157a029b811.jpeg#align=left&display=inline&height=300&id=uf91abfa6&originHeight=300&originWidth=733&size=22465&status=done&style=shadow&width=733)<br />图2-6 QUIC无队头阻塞
<a name="s6zw5"></a>
## 3、QUIC协议组成
QUIC 的 packet 除了个别报文比如 PUBLIC_RESET 和 CHLO，所有报文头部都是经过认证的，报文 Body 都是经过加密的。这样只要对 QUIC 报文任何修改，接收端都能够及时发现，有效地降低了安全风险。<br />如图3-1所示，红色部分是 Stream Frame 的报文头部，有认证。绿色部分是报文内容，全部经过加密。<br />![2021-04-30-23-17-41-259226.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619796589857-8dd0db5e-b803-4369-a75b-ecef57b62c23.png#align=left&display=inline&height=392&id=u88f4b2d7&originHeight=392&originWidth=732&size=135444&status=done&style=shadow&width=732)<br />图3-1 QUIC的协议组成<br />◎Flags：用于表示Connection ID长度、Packet Number长度等信息；<br />◎Connection ID：客户端随机选择的最大长度为64位的无符号整数。但是，长度可以协商；<br />◎QUIC Version：QUIC协议的版本号，32位的可选字段。如果Public Flag & FLAG_VERSION != 0，这个字段必填。客户端设置Public Flag中的Bit0为1，并且填写期望的版本号。如果客户端期望的版本号服务端不支持，服务端设置Public Flag中的Bit0为1，并且在该字段中列出服务端支持的协议版本（0或者多个），并且该字段后不能有任何报文；<br />◎Packet Number：长度取决于Public Flag中Bit4及Bit5两位的值，最大长度6字节。发送端在每个普通报文中设置Packet Number。发送端发送的第一个包的序列号是1，随后的数据包中的序列号的都大于前一个包中的序列号；<br />◎Stream ID：用于标识当前数据流属于哪个资源请求；<br />◎Offset：标识当前数据包在当前Stream ID 中的字节偏移量；<br />QUIC报文的大小需要满足路径MTU的大小以避免被分片。当前QUIC在IPV6下的最大报文长度为1350，IPV4下的最大报文长度为1370。
<a name="DlJBB"></a>
## 4、总结
QUIC具有众多优点，它融合了UDP协议的速度、性能与TCP的安全与可靠，同时也解决了HTTP1、HTTP1.1、HTTP2中引入的一些缺点，大大优化了互联网传输体验。
