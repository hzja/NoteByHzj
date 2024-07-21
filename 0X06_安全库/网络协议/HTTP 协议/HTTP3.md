网络协议 HTTP3
<a name="AJedq"></a>
## HTTP/2 存在的问题
过HTTP/2因为底层使用的传输层协议仍然是TCP，所以他存在着TCP队头阻塞、TCP握手延时长以及协议僵化等问题。这导致HTTP/2虽然使用了多路复用、二进制分帧等技术，但是仍然存在着优化空间。
<a name="TRI4o"></a>
## QUIC协议
HTTP/2之所以"被弃用"，是因为他使用的传输层协议仍然是TCP，所以HTTP/3首要解决的问题就是绕开TCP。那么如果研发一种新的协议，同样还是会因为受到中间设备僵化的影响，导致无法被大规模应用。所以，研发人员们想到了一种基于UDP实现的方式。于是，Google是最先采用这种方式并付诸于实践的，他们在2013年推出了一种叫做QUIC的协议，全称是Quick UDP Internet Connections。从名字中可以看出来，这是一种完全基于UDP的协议。在设计之初，Google就希望使用这个协议来取代HTTPS/HTTP协议，使网页传输速度加快。2015年6月，QUIC的网络草案被正式提交至互联网工程任务组。2018 年 10 月，互联网工程任务组 HTTP 及 QUIC 工作小组正式将基于 QUIC 协议的 HTTP（英语：HTTP over QUIC）重命名为HTTP/3。**所以，现在所提到的HTTP/3，其实就是HTTP over QUIC，即基于QUIC协议实现的HTTP。**那么，想要了解HTTP/3的原理，只需要了解QUIC就可以了。QUIC协议有以下特点：

- 基于UDP的传输层协议：它使用UDP端口号来识别指定机器上的特定服务器。
- 可靠性：虽然UDP是不可靠传输协议，但是QUIC在UDP的基础上做了些改造，使得**他提供了和TCP类似的可靠性**。它提供了数据包重传、拥塞控制、调整传输节奏以及其他一些TCP中存在的特性。
- 实现了无序、并发字节流：**QUIC的单个数据流可以保证有序交付，但多个数据流之间可能乱序**，这意味着单个数据流的传输是按序的，但是多个数据流中接收方收到的顺序可能与发送方的发送顺序不同！
- 快速握手：**QUIC提供0-RTT和1-RTT的连接建立**
- 使用TLS 1.3传输层安全协议：与更早的TLS版本相比，TLS 1.3有着很多优点，但使用它的最主要原因是其握手所花费的往返次数更低，从而能降低协议的延迟。

那么，QUIC到底属于TCP/IP协议族中的那一层呢？QUIC是基于UDP实现的，并且是HTTP/3的所依赖的协议，那么，按照TCP/IP的分层来讲，他是属于传输层的，也就是和TCP、UDP属于同一层。<br />如果更加细化一点的话，因为QUIC不仅仅承担了传输层协议的职责，还具备了TLS的安全性相关能力，所以，可以通过下图来理解QUIC在HTTP/3的实现中所处的位置。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855309706-9b3e92ee-d711-47bf-ba56-a7c69a22c3e4.webp#averageHue=%23f8f8e5&height=553&id=EhBIQ&originHeight=553&originWidth=941&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=941)<br />分别展开分析一下QUIC协议。先来看下他是如何建立连接的。
<a name="6MV8k"></a>
### QUIC的连接建立
TCP这种可靠传输协议需要进行三次握手，也正是因为三次握手，所以需要额外消耗1.5 RTT，而如果再加上TLS的话，则需要消耗3-4个 RTT连接。那么，QUIC是如何建立连接的呢？如何减少RTT的呢？QUIC提出一种新的连接建立机制，基于这种连接接机制，实现了快速握手功能，一次QUIC连接建立可以实现使用 0-RTT 或者 1-RTT 来建立连接。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1617855309686-27e3c2be-13a0-4bff-ad89-6e1d19d5ea37.gif#averageHue=%23f9f9f9&height=381&id=gsLNn&originHeight=381&originWidth=600&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=600)<br />QUIC在握手过程中使用Diffie-Hellman算法来保证数据交互的安全性并合并了它的加密和握手过程来减小连接建立过程中的往返次数。
> Diffie–Hellman (以下简称DH)密钥交换是一个特殊的交换密钥的方法。它是密码学领域内最早付诸实践的密钥交换方法之一。DH可以让双方在完全缺乏对方(私有)信息的前提条件下通过不安全的信道达成一个共享的密钥。此密钥用于对后续信息交换进行对称加密。

QUIC 连接的建立整体流程大致为：QUIC在握手过程中使用Diffie-Hellman算法协商初始密钥，初始密钥依赖于服务器存储的一组配置参数，该参数会周期性的更新。初始密钥协商成功后，服务器会提供一个临时随机数，双方根据这个数再生成会话密钥。客户端和服务器会使用新生的的密钥进行数据加解密。<br />以上过程主要分为两个步骤：初始握手（Initial handshake）、最终（与重复）握手（Final (and repeat) handshake），分别介绍下这两个过程。
<a name="zD5WC"></a>
### 初始握手（Initial handshake）
在连接开始建立时，客户端会向服务端发送一个打招呼信息，（inchoate client hello (CHLO)），因为是初次建立，所以，服务端会返回一个拒绝消息（REJ），表明握手未建立或者密钥已过期。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855309783-b7c8473a-daa4-4158-967b-ded0a84c0c48.webp#averageHue=%23fdfdfd&height=373&id=rPNXq&originHeight=373&originWidth=524&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=524)<br />但是，这个拒绝消息中还会包含更多的信息（配置参数），主要有：

- Server Config：一个服务器配置，包括服务器端的Diffie-Hellman算法的长期公钥（long term Diffie-Hellman public value）
- Certificate Chain：用来对服务器进行认证的信任链
- Signature of the Server Config：将Server Config使用信任链的叶子证书的public key加密后的签名
- Source-Address Token：一个经过身份验证的加密块，包含客户端公开可见的IP地址和服务器的时间戳。

在客户端接收到拒绝消息（REJ）之后，客户端会进行数据解析，签名验证等操作，之后会将必要的配置缓存下来。同时，在接收到REJ之后，客户端会为这次连接随机产生一对自己的短期密钥（ephemeral Diffie-Hellman private value） 和 短期公钥（ephemeral Diffie-Hellman public value）。<br />之后，客户端会将自己刚刚产生的短期公钥打包一个Complete CHLO的消息包中，发送给服务端。这个请求的目的是将自己的短期密钥传输给服务端，方便做前向保密，后面篇幅会详细介绍。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855309854-af264a09-69ae-4d60-9eff-770a9e36aab4.webp#averageHue=%23fdfdfd&height=372&id=S6dHL&originHeight=372&originWidth=588&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=588)<br />在发送了Complete CHLO消息给到服务器之后，为了减少RTT，客户端并不会等到服务器的响应，而是立刻会进行数据传输。<br />为了保证数据的安全性，客户端会自己的短期密钥和服务器返回的长期公钥进行运算，得到一个初始密钥（initial keys）。<br />有了这个初识密钥之后，客户端就可以用这个密钥，将想要传输的信息进行加密，然后把他们安全的传输给服务端了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855309884-cde38c6f-65a6-4cd7-9acd-d7e34e4cbf4f.webp#averageHue=%23fdfdfd&height=392&id=kE9WH&originHeight=392&originWidth=605&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=605)<br />另外一面，接收到Complete CHLO请求的服务器，解析请求之后，就同时拥有了客户端的短期公钥和自己保存的长期密钥。这样通过运算，服务端就能得到一份和客户端一模一样的初始密钥（initial keys）。<br />接下来他接收到客户端使用初始密钥加密的数据之后，就可以使用这个初始密钥进行解密了，并且可以将自己的响应再通过这个初始密钥进行加密后返回给客户端。**所以，从开始建立连接一直到数据传送，只消耗了初始连接连接建立的 1 RTT**
<a name="tDl3q"></a>
### 最终（与重复）握手
那么，之后的数据传输就可以使用初始密钥（initial keys）加密了吗？其实并不完全是，因为初始密钥毕竟是基于服务器的长期公钥产生的，而在公钥失效前，几乎多有的连接使用的都是同一把公钥，所以，这其实存在着一定的危险性。<br />所以，为了达到前向保密 (Forward Secrecy) 的安全性，客户端和服务端需要使用彼此的短期公钥和自己的短期密钥来进行运算。
> 在密码学中，前向保密（英语：Forward Secrecy，FS）是密码学中通讯协议的安全属性，指的是长期使用的主密钥泄漏不会导致过去的会话密钥泄漏。

那么现在问题是，客户端的短期密钥已经发送给服务端，而服务端只把自己的长期密钥给了客户端，并没有给到自己的短期密钥。<br />所以，服务端在收到Complete CHLO之后，会给到服务器一个server hello(SHLO)消息，这个消息会使用初始密钥（initial keys）进行加密。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855309972-2670f41c-a8c4-4a6e-912c-0935f4240e4e.webp#height=501&id=UNydN&originHeight=501&originWidth=542&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=542)<br />这个CHLO消息包中，会包含一个服务端重新生成的短期公钥。<br />这样客户端和服务端就都有了对方的短期公钥（ephemeral Diffie-Hellman public value）。这样，客户端和服务端都可以基于自己的短期密钥和对方的短期公钥做运算，产生一个仅限于本次连接使用的前向保密密钥 (Forward-Secure Key)，后续的请求发送，都基于这个密钥进行加解密就可以了。这样，双方就完成了最终的密钥交换、连接的握手并且建立了QUIC连接。<br />当下一次要重新创建连接的时候，客户端会从缓存中取出自己之前缓存下来的服务器的长期公钥，并重新创建一个短期密钥，重新生成一个初始密钥，再使用这个初始密钥对想要传输的数据进行加密，向服务器发送一个Complete CHLO 请求即可。这样就达到了0 RTT的数据传输。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855310032-648a5169-ab68-41b8-aa70-42084890b1dd.webp#height=487&id=lhqqg&originHeight=487&originWidth=650&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=650)<br />**所以，如果是有缓存的长期公钥，那么数据传输就会直接进行，准备时间是0 RTT**<br />**以上，通过使用Diffie-Hellman算法协商密钥，并且对加密和握手过程进行合并，大大减小连接过程的RTT ，使得基于QUIC的连接建立可以少到1 RTT甚至0 RTT。**以下，是Google官网上面的一张关于QUIC连接建立的流程图，可以帮助大家理解这个过程。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855310144-34468a80-153d-48ec-8a08-3ddb14dd0589.webp#height=739&id=MnFdD&originHeight=739&originWidth=452&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=452)<br />另外，通过以上关于握手建立的过程，可以知道，QUIC在整个过程中通过加解密的方式很好的保证了安全性。
<a name="qsUJB"></a>
## 多路复用
基于TCP的协议实现的HTTP有一个最大的问题那就是队头阻塞问题，那么，在这方面，QUIC是如何解决这个问题的呢？<br />TCP传输过程中会把数据拆分为一个个按照顺序排列的数据包，这些数据包通过网络传输到了接收端，接收端再按照顺序将这些数据包组合成原始数据，这样就完成了数据传输。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855310179-63e39bb8-d31a-43c1-8dc5-567fc0d58a91.webp#height=437&id=nlZn6&originHeight=437&originWidth=817&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=817)<br />但是如果其中的某一个数据包没有按照顺序到达，接收端会一直保持连接等待数据包返回，这时候就会阻塞后续请求。这就发生了TCP队头阻塞。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855310156-70346011-ee0d-4a45-8fea-c2c3c524a75a.webp#height=453&id=QxtLC&originHeight=453&originWidth=888&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=888)类似于HTTP/2，**QUIC在同一物理连接上可以有多个独立的逻辑数据流，这些数据流并行在同一个连接上传输，且多个数据流之间间的传输没有时序性要求，也不会互相影响。**
> 数据流（Streams）在QUIC中提供了一个轻量级、有序的字节流的抽象化

QUIC的单个数据流可以保证有序交付，但多个数据流之间可能乱序。这意味着单个数据流的传输是按序的，但是多个数据流中接收方收到的顺序可能与发送方的发送顺序不同！<br />也就是说同一个连接上面的多个数据流之间没有任何依赖（不要求按照顺序到达），即使某一个数据包没有达到，也只会影响自己这个数据流，并不会影响到到其他的数据流。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617855310244-a9c14326-1018-4dba-a22d-1afc1844e8e9.webp#height=381&id=zRdQi&originHeight=381&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="3gGrV"></a>
## 连接迁移
对于TCP连接的识别，需要通过服务器和客户端过双方的ip和端口四个参数进行的。在网络切换的场景中，比如手机切换网络，那么自身的ip就会发生变化。这就导致之前的TCP连接就会失效，就需要重新建立。这种场景对于移动端设备普及的今天来说，还是比较频繁的。所以，在这一点上，QUIC进行了优化。**QUIC协议使用特有的UUID来标记每一次连接，在网络环境发生变化的时候，只要UUID不变，就能不需要握手，继续传输数据。**
<a name="jOXIB"></a>
## 可靠性
TCP之所以被称之为可靠链接，不仅仅是因为他有三次握手和四次关闭的过程，还因为他做了很多诸如**流量控制、数据重传、拥塞控制**等可靠性保证。这也是为什么一直以来都是以TCP作为HTTP实现的重要协议的原因。那么，QUIC想要取代TCP，就需要在这方面也做出努力，毕竟UDP自身是不具备这些能力的。TCP拥塞控制是TCP避免网络拥塞的算法，是互联网上主要的一个拥塞控制措施。经典的算法实现有很多，诸如TCP Tahoe 和 Reno、TCP Vegas、TCP Hybla、TCP New Reno、TCP Westwood和Westwood+以及TCP BIC 和 CUBIC等等。QUIC协议同样实现了**拥塞控制**。不依赖于特定的拥塞控制算法，并且提供了一个可插拔的接口，允许用户实验。默认使用了 TCP 协议的 Cubic 拥塞控制算法。关于**流量控制**，QUIC提供了基于stream和connection两种级别的流量控制，既需要对单个 Stream 进行控制，又需要针对所有 Stream 进行总体控制。QUIC的连接级流控，用以限制 QUIC 接收端愿意分配给连接的总缓冲区，避免服务器为某个客户端分配任意大的缓存。连接级流控与流级流控的过程基本相同，但转发数据和接收数据的偏移限制是所有流中的总和。
<a name="YYCEi"></a>
## 弊端
以上介绍了很多QUIC的相比较于TCP的优点，可以说这种协议相比较于TCP确实要优秀一些。因为他是基于UDP的，并没有改变UDP协议本身，只是做了一些增强，虽然可以避开中间设备僵化的问题，但是，在推广上面也不是完全没有问题的。首先，**很多企业、运营商和组织对53端口（DNS）以外的UDP流量会进行拦截或者限流**，因为这些流量近来常被滥用于攻击。特别是一些**现有的UDP协议和实现易受放大攻击（amplification attack）威胁，攻击者可以控制无辜的主机向受害者投放发送大量的流量。**所以，基于UDP的QUIC协议的传输可能会受到屏蔽。另外，因为UDP一直以来定位都是不可靠连接，所以有很多中间设备对于他的支持和优化程度并不高，所以，出现丢包的可能性还是比较高的。
<a name="eSifw"></a>
## 总结
下表是总结的HTTP/2和HTTP/3的异同点。

| 特性 | HTTP/2 | HTTP/3 |
| --- | --- | --- |
| 传输层协议 | TCP | 基于UDP的QUIC |
| 默认加密 | 否 | 是 |
| 独立的数据流 | 否 | 是 |
| 队头阻塞 | 存在TCP队头阻塞 | 无 |
| 报头压缩 | HPACK | QPACK |
| 握手时延 | TCP+TLS 的 1-3 RTT | 0-1 RTT |
| 连接迁移 | 无 | 有 |
| 服务器推送 | 有 | 有 |
| 多路复用 | 有 | 有 |
| 流量控制 | 有 | 有 |
| 数据重传 | 有 | 有 |
| 拥塞控制 | 有 | 有 |

