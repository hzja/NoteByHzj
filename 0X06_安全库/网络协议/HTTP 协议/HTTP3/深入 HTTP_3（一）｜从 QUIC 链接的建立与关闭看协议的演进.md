网络协议 HTTP3 QUIC
<a name="ISQdf"></a>
## 1、引言
HTTP/3涉及到 QUIC、TLS、HTTP/2、QPACK 等标准文档，而这些标准文档各自又有大量的关联文档，所以学习起来并不是一个容易的事。<br />一个协议的性能优秀与否，除了本身的设计之外，也离不开大量的软硬件优化，架构落地，专项设计等工程实践经验。<br />“TCP 为什么要三次握手以及四次挥手？”这个问题，颇有点经典的不能再经典的味道，所以这里也将从 QUIC 链接的建立流程及关闭流程入手。
<a name="S0H1x"></a>
## 2、链接建立
<a name="CDBbS"></a>
### 2.1 重温 TCP 
_**“TCP 为什么要三次握手？”**_<br />在回答问题之前需要了解 TCP 的本质，TCP 协议被设计为一种面向连接的、可靠的、基于字节流的全双工传输层通信协议。<br />**“可靠”**意味着使用 TCP 协议传输数据时，如果 TCP 协议返回发送成功，那么数据一定已经成功的传输到了对端，为了保证数据的**“可靠”**传输，首先需要一个应答机制来确认对端已经收到了数据，而这就是熟悉的 ACK 机制。<br />**“流式”**则是一种使用上的抽象(_即收发端不用关注底层的传输，只需将数据当作持续不断的字节流去发送和读取就好了)_。**“流式”**的使用方式强依赖于数据的有序传输，为了这种使用上的抽象，需要一个机制来保证数据的有序，TCP 协议栈的设计则是给每个发送的字节标示其对应的 seq_(实际应用中 seq 是一个范围，但其真实效果就是做到了每个字节都被有序标示)_，接收端通过检视当前收到数据的 seq，并与自身记录的对端当前 seq 进行比对，以此确认数据的顺序。<br />**“全双工”**则意味着通信的一端的收发过程都是可靠且流式的，并且收和发是两个完全独立，互不干扰的两个行为。<br />可以看到，TCP 的这些特性，都是以 seq 和 ACK 字段作为载体来实现的，而所有 TCP 的交互流程，都是在为了上述特性服务，当然三次握手也不例外，再来看 TCP 的三次握手的示意图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358158783-1bd3af48-3598-410a-b41a-eaf1ad12608a.webp#clientId=u56061019-e653-4&from=paste&id=u1da65e4a&originHeight=602&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubea433da-59d7-4690-90da-06f3b8ad84a&title=)<br />为了保证通信双方都能确认对端数据的发送顺序，收发端都需要各自记录对端的当前 seq，并确认对端已经同步了自己的 seq 才可以实现，为了保证这个过程，起码需要 3 个 RTT。而实际的实现为了效率考虑，将 seq 和 ACK 放在了一个报文里，这也就形成了熟知的**三次握手**。<br />当然，三次握手不仅仅是同步了 seq，还可以用来验证客户端是一个正常的客户端，比如 TCP 可能会面临这些问题：<br />(1)有一些 TCP 的攻击请求，只发 SYN 请求，但不回数据，浪费 socket 资源；<br />(2)已失效的连接请求报文段突然又传送到了服务端，这些数据不再会有后续的响应，如何防止这样的请求浪费资源？<br />而这些问题只是三次握手顺手解决的问题，不是专门为了它们设计的三次握手。<br />可能已经发现了一个问题，如果约定好 Client 和 server 的 seq 都是从 0(或者某个大家都知道的固定值)开始，是不是就可以不用同步 seq 了呢？这样似乎也就不需要三次握手那么麻烦了？可以直接开始发送数据？<br />当然，协议的设计者肯定也想过这个方案，至于为什么没这么实现，来看看 TCP 面临什么样的问题。
<a name="yFpBG"></a>
### 2.2 TCP 面临的问题 
<a name="uzoiV"></a>
#### 2.2.1 seq 攻击
在上面提到，TCP 依赖 seq 和 ACK 来实现可靠，流式以及全双工的传输模式，而实际过程中却需要通过三次握手来同步双端的 seq，如果提前约定好通信双方初始 seq，其实是可以避免三次握手的，那么为什么没有这么做呢？答案是**安全问题**。<br />TCP 的数据是没有经过任何安全保护的，无论是其 header 还是 payload，对于一个攻击者而言，他可以在世界的任何角落，伪造一个合法 TCP 报文。<br />一个典型的例子就是攻击者可以伪造一个 reset 报文强制关闭一条 TCP 链接，而攻击成功的关键则是 TCP 字段里的 seq 及 ACK 字段，只要报文中这两项位于接收者的滑动窗口内，该报文就是合法的，而 TCP 握手采用随机 seq 的方式（不完全随机，而是随着时间流逝而线性增长，到了 2^32 尽头再回滚）来提升攻击者猜测 seq 的难度，以增加安全性。<br />为此，TCP 也不得不进行三次握手来同步各自的 seq。当然，这样的方式对于 off-path 的攻击者有一定效果，对于 on-path 的攻击者是完全无效的，一个 on-path 的攻击者仍然可以随意 reset 链接，甚至伪造报文，篡改用户的数据。<br />所以，虽然 TCP 为了安全做出过一些努力，但由于其本质上只是一个传输协议，安全并不是其原生的考量，在当前的网络环境中，TCP 会遇到大量的安全问题。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358158787-5a7cc359-ec55-40b7-9c44-ea390ca5e71d.webp#clientId=u56061019-e653-4&from=paste&id=u41c3f15a&originHeight=601&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3496a708-4ac1-436a-889e-a998b862178&title=)
<a name="AghBT"></a>
#### 2.2.2 不可避免的数据安全问题
相信 SSL/TLS/HTTPS 这一类的字眼大家都不陌生，整个 TLS(传输安全层)实际上解决的是 TCP 的 payload 安全问题，当然这也是最紧要的问题。<br />比如对一个用户而言，他可能能容忍一次转账失败，但他肯定无法容忍钱被转到攻击者手上去了。TLS 的出现，为用户提供了一种机制来保证中间人无法读取，篡改的 TCP 的 payload 数据，TLS 同时还提供了一套安全的身份认证体系，来防止攻击者冒充 Web 服务提供者。然而 TCP 的 header 这一层仍然是不在保护范围内的，对于一个 on/off-path 攻击者，仍然具备理论上随时关闭 TCP 链接的能力。
<a name="z2iLG"></a>
#### 2.2.3 为了安全引发的效率问题
在当前的网络环境中，安全通信已经成为了最基本的要求。熟悉 TLS 的同学都知道，TLS 也是需要握手和交互的，虽然 TLS 协议经过多年的实践和演进，已经设计并落地了大量的优化手段(如 TLS1.3、会话复用、PSK、0-RTT 等技术)，但由于 TLS 和 TCP 的分层设计，一个安全数据通道的建立实际上仍是一个相对繁琐的流程。以一次基于 TLS1.3 协议的数据安全通道新建流程为例，其详细交互如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358158696-42cd955f-27af-4fc3-868b-b9c49e56fb38.webp#clientId=u56061019-e653-4&from=paste&id=ua746487c&originHeight=853&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u736039db-1743-4001-b1f9-4247794b4e8&title=)<br />可以看到，在一个 Client 正式开始发送应用层数据之前，需要 3 个 RTT 的交互，这算是一个非常大的开销。而从流程上来看，TCP 握手和 TLS 的握手似乎比较相似，有融合在一起的可能。的确有相关的文献探讨过在 SYN 报文里融合 ClientHello 的可行性，不过由于以下原因，这部分的探索也慢慢不了了之。

1. TLS 本身也是基于有序传输设计的协议，融合在 TCP 中需要做大量的重新设计；
2. 出于安全的考虑，TCP 的 SYN 报文被设计为不能携带数据，如果要携带 ClientHello，则需要对协议栈做大量改动，而由于 TCP 是一个内核协议栈，改动和迭代是一个痛苦且难以落地的过程；
3. 新的协议难以和传统 TCP 兼容，大面积使用的可能性也很低。
<a name="wsYf0"></a>
#### 2.2.4 TCP 的设计问题
出于 TCP 设计的历史背景，当时的网络情况并没有现在这么复杂，整个网络的瓶颈在于带宽，所以整个 TCP 的字段设计非常精简，然而造成的效果就是将控制通道和数据通道被耦合的设计在了一起，在某些场景下就会形成问题。<br />比如：<br />**seq 的二义性问题**：设想这样的一个场景，发送端发送了一个 TCP 报文，由于通信的中间设备发生了阻塞，导致该报文被延迟转发了，发送端迟迟未收到 ACK，便重新发送了一个 TCP 报文，在新的 TCP 报文达到接收端时，被延迟转发的报文也达到了接收端，接收端只会响应一个 ACK。而客户端收到 ACK 时，并不清楚这个 ACK 是对延迟转发的报文的 ACK，还是新的报文的 ACK，带来的影响也就是 RTT 的估计会不准确，从而影响拥塞控制算法的行为，降低网络效率。<br />**难用的 TCP keepalive**：比如 TCP 连接中的另一方因为停电突然断网，并不知道连接断开，此时发送数据失败会进行重传，由于重传包的优先级要高于 keepalive 的数据包，因此 keepalive 的数据包无法发送出去。只有在长时间的重传失败之后才能判断此连接断开了。<br />**队头阻塞问题**：严格来说这并不算 TCP 自身的问题，因为 TCP 本身是一个面向链接的协议，它保证了一个链接上的数据可靠传输，也算完成了任务。然而随着互联网的普及，人们利用网络传输的数据越来越多，如果将所有数据都放在一个 TCP 链接上传输，其中某一个数据发生丢包，后面的数据的传输都会被 block 住，严重影响效率。当然，使用多个 TCP 链接传输数据是一种解决方案，但多个链接又会带来新的开销问题及链接管理问题。<br />了解了 TCP 的这些问题，就能从 QUIC 的一系列复杂的机制中抽丝剥茧，看清 QUIC 本身设计的源头思路。
<a name="R4wZA"></a>
### 2.3 QUIC 的建联设计 
和 TCP 一样，QUIC 的首要目标也是提供一个**可靠、有序的流式传输协议**。不仅如此，QUIC 还要保证**原生的数据安全**以及**传输的高效**。<br />可以说，QUIC 就是在以一种更简洁高效的机制去对标 TCP+TLS。当然，和 TCP+TLS 一样，QUIC 建联流程的本质都是在为上述特性服务，由于 QUIC 是基于 UDP 重新设计的协议，便也就没那么多的历史包袱，先来整理下对这个新的协议的诉求：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358158822-d4b74d3f-359e-40bc-82da-a1274fda6ba4.webp#clientId=u56061019-e653-4&from=paste&id=ufb33b268&originHeight=537&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u266196fa-1eb4-48f5-9bf0-1218f52aa5f&title=)<br />整理好需求之后，再来看看 QUIC 实现的效果。<br />先来看一个 QUIC 链接的建立流程，一次 QUIC 链接建立的粗略示意图如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358158834-311620f7-84ef-43d4-b741-24b4f9962087.webp#clientId=u56061019-e653-4&from=paste&id=ua4ebc971&originHeight=520&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubaf8bcf2-4f9a-4c2b-bec8-86b55beee24&title=)<br />可以看到，QUIC 相比于 TCP+TLS，只需要 1.5 个 RTT 就能完成建联，大大提升了效率。熟悉 TLS 的同学可能会发现 QUIC 的建联流程似乎跟 TLS 握手没有太大区别，TLS 本身又是一个强依赖于数据有序可靠传输的协议，然而 QUIC 又依赖 TLS 去达成有序且可靠的能力，这似乎成为了一个鸡和蛋的问题，**那么 QUIC 是如何解决这个问题的呢**？<br />需要更深一步去看看 QUIC 建联的流程，粗略示意图仅仅只能粗略感受下 QUIC 相比于 TCP+TLS 流程的高效，来进一步看看更精细化的 QUIC 建联流程：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358159216-78489c96-ad76-45d2-aa71-3cd381049904.webp#clientId=u56061019-e653-4&from=paste&id=ud337a53e&originHeight=1036&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8dc4dc82-84fe-48a8-80b1-6ac7a79146d&title=)<br />这里的图显得有些繁琐，抛去 TLS 握手的细节，整个流程实际上还是和 TCP 一样是一个请求-响应的模式，然而相比于 TCP+TLS，还看到了一些不一样的地方：

1. 图中多了"init packet"、"handshake packet"、"short-header packet"的概念；
2. 图中多了 pkt_number的概念以及stream+offset的概念；
3. pkt_number 的下标变化似乎有些奇怪。

而这些不同机制就是 QUIC 实现相比于 TCP 来说更高效的点，来逐一分析。
<a name="Ivu9s"></a>
#### 2.3.1 pkt_number 的设计
pkt_number 从流程图看起来，和 TCP 的 seq 字段比较类似，然而实际上还是有不少差别，可以说，pkt_number 的设计就是为了解决前面提到的 TCP 的问题的，来看看 pkt_number 的设计：
<a name="x1dGE"></a>
##### 1. 从 0 开始的下标
前面提到过，如果 TCP 的 seq 是一个从 0 开始的字段，那么其实不需要握手，就可以开始数据的有序发送，所以解决 TLS 和有序可靠传输这个鸡和蛋问题的方案非常简单。即 pkt_number 从 0 开始计数，便可直接保证 TLS 数据的有序。
<a name="Aa0lE"></a>
##### 2. 加密 pkt_number 以保障安全
当然 pkt_number 从 0 开始技术便也就遇上了和 TCP一样的安全问题，解决方案也很简单，就是用为 pkt_number 加密，pkt_number 加密后，中间人便无法获取到加密的 pkt_number 的 key，便也无法获取到真实的 pkt_number，也就无法通过观测 pkt_number 来预测后续的数据发送。而这里又引申出了另一个问题，TLS 需要握手完成后才能得到中间人无法获取的 key，而 pkt_number 又在 TLS 握手之前又存在，这看起来又是一个鸡和蛋的问题，至于其解决方案，这里先卖一个关子，留到后面 QUIC-TLS 的专题文章再讲。
<a name="HvtHi"></a>
##### 3. 细粒度的 pkt_number space 的设计
TLS 严格来说并不是一个状态严格递进的协议，每进入一个新的状态，还是有可能会收到上一个状态的数据，这么说有点抽象。<br />举个例子，TLS1.3 引入了一个 0-RTT 的技术，该技术允许 Client 在通过 ClientHello 发起 TLS 请求时，同时发送一些应用层数。当然期望这个应用层数据的过程相对于握手过程来说是异步且互不干扰的，而如果他们都是用同一个 pkt_number 来标示，那么应用层数据的丢包势必会导致对握手过程的影响，所以，QUIC 针对握手的状态设计了三种不同的 pkt_number space：<br />(1) init<br />(2) Handshake<br />(3) Application Data<br />分别对应：<br />(1) TLS 握手中的明文数据传输，即图中的 init packet；<br />(2) TLS 中通过 traffic secret 加密的握手数据传输，即图中 handshake packet；<br />(3)握手完成后的应用层数据传输及 0-RTT 数据传输，即图中的 short header packet 以及图中暂未画出的 0-RTT packet。<br />三种不同的 space 保证了三个流程的丢包检测互不影响。关于这部分在系列文章后续(关于 QUIC 丢包检测)还会再次深入剖析。
<a name="MvE94"></a>
##### 4. 永远自增的 pkt_number
这里的永远自增指的是 pkt_number 的明文随每个 QUIC packet 发送，都会自增 1。pkt_number 的自增解决的是二义性问题，接收端收到 pkt_number 对应的 ACK 之后，可以明确的知道到底是重传的报文被 ACK 了，还是新的报文被 ACK 了，这样 RTT 的估计及丢包检测，就可以更加精细，不过仅仅只靠自增的 pkt_number 是无法保证数据的有序的，再来看看 QUIC 提供了什么样的机制保证数据的有序。
<a name="grBMW"></a>
#### 2.3.2 基于 stream 的有序传输
QUIC 是基于 UDP 实现的协议，而 UDP 是不可靠的面向报文的协议，这和 TCP 基于 IP 层的实现并没有什么本质上的不同，都是：<br />(1) 底层只负责尽力而为的，以 packet 为单位的传输;<br />(2) 上层协议实现更关键的特性，如可靠，有序，安全等。<br />从前面知道 TCP 的设计导致了链接维度的队头阻塞问题，而仅仅依靠 pkt_number 也无法实现数据的有序，所以 QUIC 必须要一种更细粒度的机制来解决这些问题：
<a name="vLgNy"></a>
##### 1. 流既是一种抽象，也是一种单位
TCP 队头阻塞的根因来自于其一条链接只有一个发送流，流上任意一个 packet 的阻塞都会导致其他数据的失败。当然解决方案也不复杂，只需要在一条 QUIC 链接上抽象出多个流来即可，整体的思路如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358159336-1b7b380f-87f9-4aba-8eaf-ba9901cfce52.webp#clientId=u56061019-e653-4&from=paste&id=ua0f51d6e&originHeight=406&originWidth=880&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u57cf2a88-6eaf-4ae5-b2fa-9337c016460&title=)<br />只要能保证各个 stream 的发送独立，那么实际上就避免了 QUIC 链接本身的队头阻塞，即一个流阻塞也可以在其他流上发送数据。<br />有了单链接多流的抽象，再来看 QUIC 的传输有序性设计，实际上 QUIC 在 stream 层面之上，还有更细粒度的单位，称作 frame。一个承载数据的 frame 携带有一个 offset 字段，表明自己相对于初始数据的偏移量。而初始偏移量为 0，这个思路等价于 pkt_number 等于 0，即不需要握手即可开始数据的发送。熟悉 HTTP/2、GRPC 的同学应该比较清楚这个 offset 字段的设计，和流式数据的传输是一样的。
<a name="hiwGJ"></a>
##### 2. 一个 TLS 握手也是一个流
虽然 TLS 数据并没有一个固定的 stream 来标示，但其可以被看作为一个特定的 stream，或者说是所有其他 stream 能建立起来的初始 stream，因为它其实也是基于 offset 字段和固定的 frame 来承载的，这也就是 TLS 数据有序的保障所在。
<a name="EsOcB"></a>
##### 3. 基于 frame 的控制
有了 frame 这一层的抽象之后，当然可以做更多的事情。除了承载实际数据之外，也可以承载一些控制数据，QUIC 的设计汲取了 TCP 的经验教训，对于 keepalive、ACK、stream 的操作等行为，都设置了专门的控制 frame，这也就实现了数据和控制的完全解耦，同时保证了基于 stream 的细粒度控制，让 QUIC 成为更精细化的传输层协议。<br />讲到这里，其实可以看到从 QUIC 建联流程的探讨中，已经明确了 QUIC 设计的目标，正如文中一直在强调的概念：<br />“无论是建联还是什么流程，都是在为实现 QUIC 的特性而服务”。<br />现在对 QUIC 的特性及实现已经有了一些认知，来小结一下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358159199-c438dff6-1503-4a2c-9465-a46ccc795566.webp#clientId=u56061019-e653-4&from=paste&id=u7126b63b&originHeight=604&originWidth=1072&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc548c629-4ee2-45e7-890a-49d2554d99b&title=)<br />此时，再来看看 QUIC 的建联过程中的一些设计，就不会再被其复杂的流程所困扰，更能直击它的本质，因为这些设计是在 QUIC 建联大框架确认下来之后，一些细枝末节的点，而这些点往往又会在 RFC 中占据不小的篇幅。<br />举个例子，比如针对 QUIC 的放大攻击及其处理方式：放大攻击的原理为，TLS 握手过程中 ClientHello 数据很少。但 server 可能响应很多数据，这就可能形成放大攻击，比如 attacker A 发起大量 ClientHello，但把自己的 src ip 修改为 Client B，这样 attacker A 就成倍的放大了自己的流量，以攻击 Client B，其解决方案也很简单，QUIC 要求每个 Client 的首包都 padding 到一定的长度，并且在服务端提供了 address validation 机制，同时在握手完成之前，限制服务端响应的数据大小。<br />RFC9000 中花了一章节来介绍这个机制，但其本质来说只是针对 QUIC 当前握手流程的问题的修补，而不是为了设计这个机制再去设计了握手流程。
<a name="qmACL"></a>
## 3、链接关闭
<a name="uzRz4"></a>
### 3.1 从 TCP 看 QUIC 链接的优雅关闭 
链接关闭是一个简单的诉求，可以简单梳理为两个目标：

1. 用户可以主动优雅关闭链接，并能通知对方，释放资源。
2. 当通信一端无法建立的链接时，有一个通知对方的机制。

然而诉求很简单，TCP 的实现却很不简单，来看看这个 TCP 链接关闭流程状态机的转移图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358159375-48c10439-efcf-4399-8ed4-77c8add3dd59.webp#clientId=u56061019-e653-4&from=paste&id=ude7c843e&originHeight=616&originWidth=780&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u080111d7-8e7c-46e3-8ab1-ea84010c25f&title=)<br />这个流程看起来就够复杂了，牵扯出来的问题就更不少，比如经典面试题：<br />“为什么需要 TIME_WAIT？”<br />“TIME_WAIT 的连接数过多需要怎么处理？”<br />“tcp_tw_reuse 和 tcp_tw_recycle 等<br />内核参数的作用和区别？”<br />而这一切问题的根因都来源于 TCP 的链接和流的绑定，或者说是控制信令和数据通道的耦合。<br />不禁要提出一个灵魂拷问**“我们需要的是全双工的数据传输模式，但我们真的需要在链接维度做这个事情吗？”**这么说似乎有一些抽象，还是以 TCP 的 TIME_WAIT 设计为例子来说明，来自底向上的看看 TCP 的问题：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358159418-28e8ebce-80fa-4fd5-b86a-276169773238.webp#clientId=u56061019-e653-4&from=paste&id=u9f0bea4d&originHeight=764&originWidth=1066&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubc30252b-6787-460e-8fde-8599e0ffd8d&title=)<br />再回到问题上，如果将流和链接区分开，在链接维度保证流的控制指令可靠传输，链接本身实现一个简单的单工关闭过程，即通信一端主动关闭链接，则整个链接关闭，是否一切就简单起来了呢？<br />当然这就是 QUIC 的解法，有了这一层思路之后，来整理下 QUIC 链接关闭的诉求：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358159571-35114e98-f21a-41a2-a7df-a1c19c484556.webp#clientId=u56061019-e653-4&from=paste&id=udef56074&originHeight=386&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uafd896f4-9156-46fe-85d8-b1c5af30cb9&title=)<br />抛开流的关闭流程设计，在链接维度就可以得到一个清爽的状态机：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358159654-14af50d8-cb6b-4601-a3d2-68ea2ec4c78d.webp#clientId=u56061019-e653-4&from=paste&id=u6c3306bd&originHeight=788&originWidth=876&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8b29028b-c0f9-4406-8804-176add1ec00&title=)<br />可以看到，得益于单工的关闭模式，在整个 QUIC 链接关闭的流程里，关闭指令只有一个，即图中的 CONNECTION_CLOSE，而关闭的状态也只有两个，即图中的 closing 和 draing。先来看看两种状态下终端的行为：<br />**- closing 状态**：当用户主动关闭链接时，即进入该状态，该状态下，终端收到所有的应用层数据都将只会回复 CONNECTION_CLOSE<br />**- draining 状态**：当终端收到 CONNECTION_CLOSE 时，即进入该状态，该状态下终端不再回复任何数据<br />更简单的是，CONNECTION_CLOSE 是一个不需要被 ACK 的指令，也就意味着不需要重传。因为从链接维度而言，只需要保证最后能成功关闭的链接，并且新的链接不被老的关闭指令影响即可，这种简单的 CONNECTION_CLOSE 指令就能实现所有的诉求
<a name="Z2S5I"></a>
### 3.2 更安全的 reset 的方式 
当然，链接关闭也分为多种情况，和 TCP 一样，除了上一节提到的 QUIC 一端主动关闭链接的模式，QUIC 也需要提供无法回复响应时的，直接 reset 对端链接的能力。<br />而 QUIC reset 对端链接的方式相比于 TCP 来说更加安全，该机制被称作 stateless reset，这并不是一个十分复杂的机制。在 QUIC 链接建立好之后，QUIC 双方会同步一个 token，而后续的链接关闭将通过校验这个 token 来来判断该对端是否有权限来 reset 这个链接，这种机制从根本上规避了前面提到的 TCP 被恶意 reset 的这种攻击模式，整个流程如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640358159922-c23d08e3-4b22-4249-8b27-3282cfbecd48.webp#clientId=u56061019-e653-4&from=paste&id=u95c85425&originHeight=768&originWidth=804&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2f18cea4-a44d-4efc-aefb-29009d4b3e6&title=)<br />当然，stateless reset 的方案并不是银弹，安全的代价是更窄的使用范围。因为为了保障安全，token 必须通过安全的数据通道进行传输_(在 QUIC 中被限定为 NEW_TOKEN frame)_，并且接收端需要维持一个记录这个 token 的状态，也只有通过这个状态才可以保证 token 的有效性。<br />因此 stateless reset 被限定为 QUIC 链接关闭的最后手段，并且也只能在只能使用在客户端和服务端均处于一个相对正常的情况下正常工作，比如这样的情况 stateless reset 就不适用，服务端并没有监听在 443 端口，但客户端发送数据到 443 端口，而这种情况在 TCP 协议栈下是可以 RST 掉的。
<a name="psD9N"></a>
### 3.3 工程考量的超时断链 
keepalive 机制本身并没有什么花样，都是一个计时器加探测报文即可搞定，而 QUIC 得益于链接和数据流的拆分，关闭链接变成了一个非常简单的事情，keepalive 也就变得更简单易用，QUIC 在链接维度上提供了名为 PING frame 的控制指令，用于主动探测保活。<br />更简单的是，QUIC 在超时后关闭链接的方式是 silently close，即不通知对端，本机直接释放掉链接所有的资源。silently close 的好处是资源可以立即得到释放，特别是对于 QUIC 这种单链接上既要维护 TLS 状态，也要维护流状态的协议来说，有很大的收益。<br />但其劣势在于后续如果有之前链接数据到来，则只能通过 stateless reset 的方式通知对端关闭链接，stateless reset 的关闭相对来说 CONNECTION_CLOSE 开销更大。因此这部分可以说完全是一个 tradeoff，而这部分的设计方案的最终敲定，更多来自于大量工程实践的经验与结果。
<a name="dSIus"></a>
## 4、从 QUIC 链接的建立与关闭看协议的演进
从 TCP 到 QUIC，虽然只是网络协议技术的演进，但也可以管中窥豹地看一下整个网络发展的趋势。链接的建立与关闭只是对 QUIC 协议的切入点，正如文中一直在强调的部分，无论是建联还是什么流程，都是在为实现 QUIC 的特性而服务，这里除了在详细分析 QUIC 的链接建立的关闭流程之外，更是在总结这些特性的由来及设计思路。<br />通读全文，可以看到，一个现代化的网络协议已经绕不开安全这个诉求了，可以说**“安全是一切的基础，效率则是永恒的追求”。**<br />而 QUIC 首先从收敛分层协议的思路出发，统一了安全和可靠两种交互诉求，这似乎也在提示我们，**未来协议的发展，似乎也不必再完全遵从 OSI 的模型。**分层是为了各个组件更良好分工协作，而收敛则是极致的性能追求，TCP+TLS 可以收敛到 QUIC，那么就如华为提出的 NEW IP 技术一样，如果把智能路由等技术结合起来，所有三层及以上网络协议也未尝不能收敛到一个全新的 IPSEC 协议中去。<br />当然这些都来的太远了，QUIC 本身是一个非常接地气的协议，在其以开源为主导的标准形成过程中，吸收了大量工程经验，使其不至于有太多理想化的特性，且可扩展性非常强，未来这样的工作模式，也将是一种主流的模式。
