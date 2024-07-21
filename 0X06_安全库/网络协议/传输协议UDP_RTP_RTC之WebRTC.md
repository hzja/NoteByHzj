UDP RTP RTP WebRTC<br />音视频时代，WebRTC在形形色色的产品和业务场景下均有落地。在熟悉如何在浏览器获取设备的音视频数据和WebRTC是如何将获取的音视频数据进行网络传输的同时，更要夯实一下网络传输协议相关的基础知识。
<a name="Vae1d"></a>
## 1、传输层协议：TCP vs. UDP
HTTP协议运行于TCP协议之上，是万维网的运转的基础。下图是一个TCP/IP通讯协议的4层结构图，在基于网际层的运输层，它提供了节点间的数据传送服务，其中最为人所熟知的**TCP协议（Transmission Control Protocol）** 和 **UDP协议（User Datagram Protocol）**。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839561394-69adbca9-9efd-409d-8351-0d21b2ea73b7.png#clientId=ucd30a984-0e4d-4&from=paste&id=uef307cb8&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u7fb9a10d-cf00-4a9b-a747-62e0495a36b)<br />两个协议本身涉及到内容非常多，但在实际选择使用中，不妨直接通过对比TCP和UDP，来去学习和理解它们。
<a name="txWQ2"></a>
### 1.1 TCP和UDP对比
总体上有以下三点不同：

- TCP是面向连接的，UDP是无连接的。
- TCP是面向字节流的，实际上是TCP把数据看成一连串无结构的字节流；UDP是面向报文的。
- TCP提供可靠的传输，也就是说TCP连接传输的数据不会丢失，没有重复，并且按顺序到达，UDP提供不可靠传输。
<a name="djd06"></a>
#### 1.1.1 UDP无连接，TCP面向连接
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839561501-9bcd40db-8dbd-4c02-b80c-f090a0f7c6a1.png#clientId=ucd30a984-0e4d-4&from=paste&id=uc8fccd36&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=ued93f894-8da1-4d4d-a7d3-d60dd41803f)<br />UDP在传输数据之前不需要建立连接，传输双方可以随时发送数据，因此UDP是无连接的。而TCP协议在传输数据之前三次握手建立连接，在结束后需要四次挥手释放连接，具体细节在此不做赘述。
<a name="vrOpt"></a>
#### 1.1.2 UDP面向报文，TCP面向字节流
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839561768-88456ffe-e0b6-4415-8e4c-52bff1872b7c.png#clientId=ucd30a984-0e4d-4&from=paste&id=u26a92116&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=uca9da6ac-a09f-4ab0-b5ff-127866b40ca)<br />对于UDP，发送接收方应用层只给UDP传输层发送或接收报文，而UDP除了传输外的处理只是对应用层报文添加或摘除UDP首部，保留了应用层报文，因此说UDP是面向报文。<br />对于TCP而言，TCP只将应用层交下来的数据当做一连串的无结构字节流，仅将他们存入缓存并根据策略构建TCP报文进行发送，而接受方TCP只提取数据载荷部分存入缓存，并同时将缓存字节流交给应用层。TCP不保证双方应用层的发送和接收数据具有对应大小关系。因此说它是面向字节流的，而它也是TCP实现流量控制和拥塞控制的基础。
<a name="wQglN"></a>
#### 1.1.3 UDP是不可靠连接，TCP是可靠连接
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839561553-f3e46c5b-5d43-459a-9630-2455f231656c.png#clientId=ucd30a984-0e4d-4&from=paste&id=uf7239af4&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u2d10d7c5-b05a-4141-beb7-46d70a9be29)<br />UDP在传输数据时，发送产生了丢包，发送方不做任何处理。接收方校验首部发现误码，同样也不做任何处理。因此说UDP向上提供的是无连接不可靠服务。<br />而TCP在传输数据时，如果发生了丢包或者接收方检查了误码（此时会接收方会丢弃），接收方不会回确认报文，则触发接收方超时重发。由此可见，TCP通过其策略确保其传输过程无论发生什么情况，则接收方就能正确收到该数据包，因此说TCP是向上提供面向连接的可靠服务。
<a name="My5zV"></a>
### 1.2 为什么选择UDP
既然TCP有这么多优点特性，那么为什么在实时音视频传输中使用UDP呢？<br />原因在于实时音视频对于延迟特别敏感，而基于TCP协议的做不到足够低。试想一下在丢包的情况下，TCP协议的超时重传机制中RTT是以2的指数的增长。如果7次重传任然失败，理论计算会达到2分钟！在延迟高的情况下，想做到正常的实时通讯显然是不可能的，此时TCP的可靠性反而成了累赘。<br />但实际情况是，通常实时音频视频数据在传输的少量数据包丢失，对接收者影响并不大。而UDP不属于连接型协议，认为它基本是管发不管收，因而具有资源消耗小，处理速度快的优点。<br />**因此UDP在实时性和效率性都很高，在实时音视频传输中通常会选用UDP协议作为传输层协议。**<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839562580-ea741ff7-09ff-4d74-89ef-0b30e669140b.png#clientId=ucd30a984-0e4d-4&from=paste&id=udf9ecad4&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=ue1239df7-90eb-4b45-8eea-8c493166ad8)<br />WebRTC也是如此，在信令控制方面采用了可靠的TCP，**但是音视频数据传输上，使用了UDP作为传输层协议**（如上图右上）。
<a name="asLIF"></a>
## 2、应用层协议：RTP and RTCP
实时音视频通讯只靠UDP够不够呢？答案显然是不够的！还需要基于UDP的应用层协议，来专门为音视频通讯做更多保障处理。
<a name="Mk9Qy"></a>
### 2.1. RTP协议
音视频中一个视频帧数据量需要多个包来传送，并在接收端组成对应帧，正确还原出视频信号。因此要做到至少两点：

1. 检测出错顺序，并保持采样和播放之间的同步关系。
2. 需要在接收端检测出分组的丢失。

而UDP并没有这个能力，所以音视频传输中，并不直接使用UDP，而是需要RTP作为实时音视频中的应用层协议。<br />**RTP全名Real-timeTransportProtocol(实时传输协议)**，主用于实时传输数据。那么RTP协议提供哪些能力？<br />包括以下四点：

1. 实时数据的端到端传输。
2. 序号（用于丢包和重排序检）
3. 时间戳（时间同步校对和分发监控）
4. 载荷的定义类型（用于说明数据的编码格式）

但不包括：

1. 及时发送
2. 质量保证
3. 送达（可能丢）
4. 时序（到达顺序）

简单看下[**RTP协议规范**](https://tools.ietf.org/html/rfc35f50#)<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839565168-5972dde9-9ec1-4a51-b7fb-7530ae778036.png#clientId=ucd30a984-0e4d-4&from=paste&id=u025d6335&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=uf4a5d7bd-1266-41ce-a2bb-5fd91800af5)<br />RTP报文由两部分组成：报头和有效载荷。<br />以下为RTP协议头的解释，前12字节是固定的，CSRC可以有多个或者0个。

- **V**：RTP协议的版本号，占2位，当前协议版本号为2。
- **P**：填充标志，占1位，如果P=1，则在该报文的尾部填充一个或多个额外的八位组，它们不是有效载荷的一部分。
- **X**：扩展标志，占1位，如果X=1，则在RTP报头后跟有一个扩展报头。
- **CC**：CSRC计数器，占4位，指示CSRC标识符个数。
- **M**：标志，占1位，不同的有效载荷有不同的含义，**对于视频，标记一帧的结束；对于音频，标记会话的开始**。
- **PT（payload type）**：**有效荷载类型，占7位，用于记录RTP报文中有效载荷的类型/Codec**，在流媒体中大部分是用来区分音频流和视频流，便于接收（receiver）找出相应的 decoder 解码出來。
- **序列号（sequence number）**：占16位，**用于标识发送者所发送的RTP报文的序列号，每发送一个报文，序列号增1**。 这个字段当下层的承载协议用UDP的时候，网络状况不好的时候可以用来检查丢包。当出现网络抖动的情况可以用来对数据进行重新排序。序列号的初始值是随机的，同时音频包和视频包的sequence是分别计数的。
- **时戳（timestamp）**：占32位，必须使用90kHZ时钟频率（程序中的90000）。**时戳反映了该RTP报文的第一个八位组的采样时刻。接受者使用时戳来计算延迟和延迟抖动，并进行同步控制。可以根据RTP包的时间戳来获得数据包的时序。**
- **同步源（SSRC）标识符**：占32位，用于标识同步信源。同步信源是指产生媒体流的信源，他通过RTP报头中的一个32为数字SSRC标识符来标识，而不依赖网络地址，接收者将根据SSRC标识符来区分不同的信源，进行RTP报文的分组。
- **贡献源（CSRC）标识符**：每个CSRC标识符占32位，可以有0~15个CSRC。每个CSRC标识了包含在RTP报文有效载荷中的所有提供信源。
<a name="uJC2X"></a>
### 2.2. RTCP协议
前面提到RTP协议整体上还是比较简单粗暴的，其本身并没有提供按时发送机制或其它服务质量（QoS）保证。因此RTP还需要有一套配套协议为其服务质量提供保证，则就是**RTCP协议（全名Real-timeControlProtocol）。**<br />RTP标准定义了两个子协议，RTP和RTCP。<br />举个例子，在传输音视频时的丢包，乱序，抖动，这些WebRTC在底层都有对应的处理策略。但是如何将这些传输时 **“网络质量信息”** 实时告诉对方，就是RTCP它的作用。相对于RTP来说，RTCP所占的带宽非常小，通常只有5%。<br />接下来简单看下RTCP协议规范：首先RTCP报文有多种类型：

1. **发送报告SR (Sender Report)**: 当前活动发送者发送、接收统计。PT=200
2. **接受者报告RR (Reciver Report)**：接收报告，非活动发送者接收统计。PT=201
3. 源描述报告SDES(Source Description)：源描述项，包括CNAME PT=202
4. BYB报告：参与者结束对话 PT=203
5. APP报告：应用自定义 PT=204
6. jitter报告IJ PT=195
7. 传输反馈RTPFB PT=205
8. Playload反馈PSFB PT=206 . . . . . 这里可以关注两个比较重要的的报文：SR 和 RR，通过他们让收发两端知道网络质量情况。

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839565997-0cfede27-af04-4f42-b3a3-41bba3bb4b01.png#clientId=ucd30a984-0e4d-4&from=paste&id=uc5a8c927&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u68f4ce83-a1e3-45cf-a27f-5df703227df)<br />以上为SR的协议规范：

- Header 部分用于标识该报文的类型，比如是 SR（200） 还是 RR（201）。
- Sender info 部分用于指明作为发送方，到底发了多少包。
- Report block 部分指明发送方作为接收方时，它从各个 SSRC 接收包的情况。

通过报告以上信息，各端知道网络传输反馈数据后，就可以根据其做传输策略的调整了。当然协议本身的内容并不只有上面的简单一小段，实际还涉及各项反馈数据的计算方法，这里篇幅有限不展开细讲。
<a name="pm1Cq"></a>
### 2.3. RTP会话流程小结
当了解为什么选择UDP协议、以及RTP/RTCP协议做了些什么事情之后，简单总结在传输协议层面上的整个流程：<br />当应用建立一个RTP会话时，应用程序将确定一对目的传输地址。目的传输地址由一个网络地址和一对端口组成，有两个端口：一个给RTP包，一个给RTCP包。RTP数据发向偶数的UDP端口，而对应的控制信号RTCP数据发向相邻的奇数UDP端口（偶数的UDP端口＋1），这样就构成一个UDP端口对。大致流程如下：

1. RTP协议从上层接收流媒体信息码流，封装成RTP数据包；
2. RTCP从上层接收控制信息，封装成RTCP控制包。
3. RTP将RTP 数据包发往UDP端口对中偶数端口；RTCP将RTCP控制包发往UDP端口对中的接收端口。
<a name="GezPR"></a>
### 2.4. 快速上手Wireshark抓包RTP及RTCP
纸上得来终觉浅,绝知此事要躬行。接下来通过实际播放WebRTC流媒体，并通过抓包来还原RTP包和RTCP报文的真面目。<br />Wireshark是一个强大的网络数据包分析软件，可以详细的展示网络数据包的交换过程，是监控网络请求定位网络问题的利器。这个强大的抓包工具，涉及非常多功能，由于这里不是Wireshark教程，更详细的功能课自行搜索挖掘，这里只会讲个大致流程：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839567720-a172a421-bda2-47a9-91c2-350ba435f8c0.png#clientId=ucd30a984-0e4d-4&from=paste&id=u4237e7ee&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=ub3fcf79d-b25b-4b4d-b546-6f98c588121)

1. 下载并安装Wireshark（非常简单）。
2. 浏览器打开腾讯课堂，挑选一个免费且正在直播的课程，一般情况下采用WebRTC播放。(另起tab打开WebRTC调试工具 这里会展示页面WebRTC播放实时流媒体数据的网络情况。)

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839566346-f56f6029-4659-4320-beaf-3ea33b83dd27.png#clientId=ucd30a984-0e4d-4&from=paste&id=ub300cdbc&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u91831086-6944-4f23-8545-dcdf754f13d)

3. 打开Wireshark，需要选择要捕获机器上网卡的网络包。当机器上有多块网卡的时候，需要选择一个网卡接口，这里选择Wifi，并点击左上角蓝色按钮开始抓包。

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839566370-a7d4c141-41b1-459f-9ea9-6ee7bd505276.png#clientId=ucd30a984-0e4d-4&from=paste&id=u80950430&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=uaca67a91-8d83-4f89-a54e-050cadc57ea)

4. 一旦启动抓包，这里会瞬间展示抓到的各种协议的大量数据包（下图展示wireshark每个区域的功能），其中在①过滤栏中输入UDP进行过滤，然后就会在

②数据包列表中只展示出udp的数据包，并会解析出部分协议的数据包，并在Protocol列清楚的看到它们的协议。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839566623-481b9cd5-9ca4-47ba-986a-e38dc7fd7244.png#clientId=ucd30a984-0e4d-4&from=paste&id=u0c7bb1e3&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=ubf2b698d-a63a-4a6d-996a-8160a7dba8d)

5. 由于WireShark不会直接识别RTP的UDP数据包，需要右键UDP包解析为（decode as）RTP包。

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839567176-4ed84ae5-33ec-4644-8b85-4b63c3084020.png#clientId=ucd30a984-0e4d-4&from=paste&id=u6a9bf956&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=uef4a3d36-ab72-4ba0-8549-8689cf8bc96)

6. 此时wireshark能够识别RTP协议数据包，在分层协议可以看到清晰的结构，从上往下依次是：IP = > UDP => RTP 点开RTP协议那一层，展开可以看到，和上面RTP报文协议的一致：标明了版本号、填充标记...等等内容。

其中应该着重关注的部分内容： **Playload type（PT值）**：代表负载的类型，其中这里122对照WebRTC的SDP可以确认是H264视频负载类型数据。<br />**时间戳**：记录的是采样时刻为6120，还要根据采样率进行换算。<br />**SSRC**: 同步源（SSRC）标识符为0x0202c729。以上这些都是RTP头部，最后playload才是承载的媒体数据。<br />RTP的特点不仅仅支持承载在UDP上，有利于低延迟音视频数据的传输，它允许通过其它协议接收端和发送端协商音视频数据的封装和编解码格式，playload type字段比较灵活支持的音视频数据类型非常多的，具体可以参考：RTP payload formats<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839567992-0fe22e8b-3927-4b0a-ae41-659cd7432784.png#clientId=ucd30a984-0e4d-4&from=paste&id=u6e9cb58a&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u7c9199d0-1423-4a5b-a155-5cf94a8ca22)<br />再具体看看RTP包的音视频帧：<br />其中下面seq=21到seq=24的多个数据包，每个单独为一个音频帧，所以时间戳不同。而红色框seq=96到seq=102的多个数据包组成，组成PT=122的一个视频帧，所以这几个报的时间戳也是相同的。这是因为一个视频帧包含数据量较大，需要分开多个包发送。而音频帧较小，则单独一个包发送，从它们的包length大小就能看出视频包比音频包要大的多。另外seq=102的数据包，mark字段为true表示为一个视频帧的最后一个数据包，通过结合seq可以知道音视频数据的接收是否有乱序或者是丢包。

7. 通过上方‘工具栏’=>‘电话’=>‘RTP‘打开信息面板，可以看到当前有一条音频RTP流，和一条视频RTP流。左边分析出表示了流的源地址端口和目的地址和端口。右边为RTP相关内容，展示了RTP流的SSRC、载荷类型、丢包情况等等。

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839567909-65a36e39-a696-47a5-8532-612fcef84b2d.png#clientId=ucd30a984-0e4d-4&from=paste&id=u0b113191&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=udcce45ba-501b-40c6-9995-76819f04946)

8. 最后再简单看RTCP，在过滤栏中输入rtcp进行过滤，可以看到Sender Report 和 Receive Report。他们的PT（packcet type）分别为200和201，报告的SSRC为0x02029dfc，以及详细的发送包和接收的情况。详细的内容解析可以结合RTCP规范协议去进一步学习了解。

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618839568159-c25eac32-f1af-458f-bc97-049e0e7ce9fe.png#clientId=ucd30a984-0e4d-4&from=paste&id=u76dcca62&originHeight=140&originWidth=140&originalType=url&status=done&style=shadow&taskId=u12b04a37-da1a-43de-aa37-d7940970734)
<a name="vP5Mm"></a>
## 3、总结
不少人觉得一名开发者在学习使用WebRTC时，能够快速上手实践和业务落地就足够了，再去了解这些传输协议有必要吗？但常常即便已经清楚如何使用它，不代表能发挥出它本身最大优势。运用一项技术所达到的上限，往往取决于对它的底层理解有多深入。<br />这里简单介绍为什么实时音视频选择UDP作为传输层协议，以及简单介绍WebRTC所涉及协议中比较重要的两个协议RTP/RTCP，像WebRTC技术涉及与融合多方面种技术（音视频处理，传输、安全加密等等）。
