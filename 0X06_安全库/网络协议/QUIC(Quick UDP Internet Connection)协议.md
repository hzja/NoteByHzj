QUIC<br />QUIC(Quick UDP Internet Connection)是Google公司提出的基于UDP的高效可靠协议，他和HTTP一样同样是应用层协议。<br />为什么高效呢？是因为其基于无连接的UDP而不是基于TCP去实现的。<br />为什么可靠呢？因为其模仿TCP协议的可靠性，在应用层上做了可靠性的保证。
<a name="aVg4r"></a>
## QUIC的价值和意义
互联网已经发展了几十年了，但是一提到网络协议，传输层使用得最多的还是TCP协议，应用层使用得最多的是HTTP协议，当然HTTP底层也是使用得TCP协议。虽然互联网已经发展这么久了但是对于TCP来说发展依旧缓慢，要说最大的改进应该是Google 在 `ACM CoNEXT` 会议上发表的用于改善 Web 应用响应延时TCP Fast Open，通过修改 TCP 协议利用三次握手时进行数据交换，这个在Linux内核 3.7.1 以及更高版本可以支持。由于修改TCP协议必然会修改内核从而导致系统升级，这个推动的难度非常之大。<br />既然修改内核不行，那么Google就提出了在应用层协议上修改的办法，也就有了QUIC。
<a name="dXnJS"></a>
## QUIC的使用者
首先使用它的人肯定是谷歌，据说谷歌有50%的请求都是QUIC协议，微博也在全面使用QUIC协议，同时还有一些视频云服务比如七牛也在使用，在腾讯内部也有很多部门在大量使用QUIC，所以不需要担心这个协议使用的问题。
<a name="ixUIy"></a>
## QUIC分析
<a name="CEZMR"></a>
### 0RTT 建立链接
RTT((Round-Trip Time)顾名思义就是往返时延的意思，0RTT的话意思就是QUIC可以在第一次发送的时候就带上数据，熟悉TCP的同学应该知道，TCP会有一个三次握手那么实际上也就是会有1次RTT:<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600420205034-e031862c-b373-429e-a53f-44254c109ac8.png#align=left&display=inline&height=294&originHeight=883&originWidth=946&size=56365&status=done&style=shadow&width=315.3333333333333)<br />如果是HTTPS的话还会使用SSL/TLS的额外握手，就会有3次RTT：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600432079341-29f2bedd-bd83-4383-b15d-047fea3ff91c.png#align=left&display=inline&height=406&originHeight=1217&originWidth=1285&size=152247&status=done&style=shadow&width=428.3333333333333)<br />那么0RTT的建立链接QUIC是怎么做到的呢？这里得先说一下QUIC的0RTT并不是完全的0RTT,他同样需要1RTT去做一次秘钥协商，在QUIC中使用的是Diffie-Hellman密钥交换，该算法是一种建立密钥的方法，并非加密方法，但其产生的密钥可用于加密、密钥管理或任何其它的加密方式，这种密钥交换技术的目的在于使两个用户间能安全地交换密钥(KEY)以便用于今后的报文加密。DH算法用了离散对数的相关知识，这里就不扩展讲解，有兴趣的可以下来搜索这种算法。QUIC通过DH算法创建一个安全的连接后，客户端会缓存起来原始的连接信息等。在后续的过程中只要和同一个服务器建立链接都是直接发送数据，不需要再次协商秘钥，从而实现了后续的0RTT。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600432866546-e7e38185-d9e9-41db-8ae1-169f537d03a8.png#align=left&display=inline&height=426&originHeight=1279&originWidth=1503&size=190340&status=done&style=shadow&width=501)
<a name="sJ2Gn"></a>
### 更为出色拥塞控制
TCP的拥塞控制的算法特别多，比如基于丢包反馈的（Tahoe、Reno、New Reno、SACK）, 基于延时反馈的（Vegas、Westwood），其中的Reno也就是最为熟悉的，它分为四个阶段：慢启动，拥塞避免，快速重传，快速恢复。<br />而在QUIC中使用了更为优秀的机制来控制拥塞控制，它可以针对不同业务，不同网络制式，甚至不同的RTT，使用不同的拥塞控制算法。同时也会采用了packet pacing来探测网络带宽，来提升网络使用率。
<a name="9WSjQ"></a>
### 更好的重传机制
在重传的机制中有一个比较重要的名词，那就是RTO(Retransmission Timeout) 重传超时时间，一般这个数据会根据RTT去进行计算，那么有一个更精确的RTT肯定就可以有一个更好的RTO。<br />在TCP中重传的时候序列号不变，会导致RTT算得不准确，比如重传的时候不知道这次请求到底是和原始请求匹配还是和重试请求匹配，就会导致采样RTT不准确。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600433460250-06c7f0bf-adb0-4289-96f2-40a06e5912d6.png#align=left&display=inline&height=320&originHeight=960&originWidth=2494&size=161210&status=done&style=shadow&width=831.3333333333334)<br />在QUIC中序列号都是递增的，并且通过offset来确定在包中的真实位置，这样就可以得到更为准确的RTT。<br />在TCP中计算RTT的方法就是发出的时间和响应回来的时间相减，但是这样算出的时间不准确，在QUIC中会减去服务端Ack Delay的时间，这样的话就更为精准。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600433684082-7a99c9c8-adb7-4be5-b4cf-99b8f1261e9d.png#align=left&display=inline&height=315&originHeight=944&originWidth=1382&size=103373&status=done&style=shadow&width=460.6666666666667)<br />同样的在TCP中有个SACK选项，该选项打开时用于记录传输过程中一些没有被确认的数据的范围，便于后续定向重传多组丢失数据，而不是全部重传，所以更多的范围便于更多的选择重传，也意味着更少的重传包频率。但TCP最多支持3个SACK范围，而QUIC能支持255个。
<a name="mFuGE"></a>
### 没有队头阻塞的多路复用
熟悉HTTP2.0的同学应该知道在2.0中如果访问同一个服务器只会有一个TCP连接，所有的请求都会走这条连接：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600434042054-d38c1e92-3593-40a3-b68f-5b2251030cd3.png#align=left&display=inline&height=148&originHeight=444&originWidth=1666&size=64715&status=done&style=shadow&width=555.3333333333334)<br />而每个请求在Connection中叫做Stream,一个Connection中可以有多个Stream，这里有个问题是在TCP中的包是保证时序的，如果某个Stream丢了一个包，他同时也会影响其他的Stream，在更为严重的时候反而多路复用还不如HTTP1.1的多个链接。<br />而在QUIC中，因为底层是基于UDP,UDP不需要保证包的时序，只会在接收包的时候对包进行重组，所以不会存在这个问题。这也就是为什么Google提议在HTTP3中使用QUIC的原因。
<a name="lsFUh"></a>
### 更优秀的流量控制
上面说了QUIC是多路复用的，在QUIC中可以针对Stream和Connection都进行流量控制。<br />QUIC 的流量控制和 TCP 有点区别，TCP 为了保证可靠性，窗口左边沿向右滑动时的长度取决于已经确认的字节数。如果中间出现丢包，就算接收到了更大序号的 Segment，窗口也无法超过这个序列号。<br />但 QUIC 不同，就算此前有些 packet 没有接收到，它的滑动只取决于接收到的最大偏移字节数。<br />最重要的是可以进行动态配置，可以在内存不足或者上游处理性能出现问题时，通过流量控制来限制传输速率，保障服务可用性。
<a name="hCGM9"></a>
### 连接迁移
现在在手机上移动流量和wifi的切换是一个比较常见的事，每次切换ip地址都会发生变化，如果是TCP的话连接就会中断从而进行重新建立链接。<br />在QUIC不再以 IP 及端口四元组标识，而是以一个 64 位的随机数作为 ID 来标识，通过这样的方式可以进行连接重复利用，不会重新建立新的连接。
<a name="3bhAI"></a>
### 其他
在QUIC中还有更多的其他的特性，比如：

- 通过header stream保证流顺序
- 底层保证连接持久
- 源地址令牌防止地址欺骗
- 握手时压缩证书避免放大攻击这里就不一一介绍了
