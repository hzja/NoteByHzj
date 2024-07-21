网络协议 HTTP3<br />事实上，HTTP/3 现在还没正式推出，不过自 2017 年起， HTTP/3 已经更新到 34 个草案了，基本的特性已经确定下来了，对于包格式可能后续会有变化。<br />所以，这次 HTTP/3 介绍不会涉及到包格式，只说它的特性。<br />![2021-05-04-10-40-47-095810.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620096299174-0c348566-0c92-4459-9ac7-31c3d8791be4.png#clientId=u7f784eef-cdc9-4&from=ui&id=uf028bb16&originHeight=589&originWidth=926&originalType=binary&size=66815&status=done&style=shadow&taskId=u1667ea85-42a6-4ca2-aef2-bf144399637)

---

<a name="UVuFk"></a>
### 美中不足的 HTTP/2
HTTP/2 通过头部压缩、二进制编码、多路复用、服务器推送等新特性大幅度提升了 HTTP/1.1 的性能，而美中不足的是 HTTP/2 协议是基于 TCP 实现的，于是存在的缺陷有三个。

- 队头阻塞；
- TCP 与 TLS 的握手时延迟；
- 网络迁移需要重新连接；
<a name="qO5lg"></a>
#### 队头阻塞
HTTP/2 多个请求是跑在一个 TCP 连接中的，那么当 TCP 丢包时，整个 TCP 都要等待重传，那么就会阻塞该 TCP 连接中的所有请求。<br />因为 TCP 是字节流协议，TCP 层必须保证收到的字节数据是完整且有序的，如果序列号较低的 TCP 段在网络传输中丢失了，即使序列号较高的 TCP 段已经被接收了，应用层也无法从内核中读取到这部分数据，从 HTTP 视角看，就是请求被阻塞了。<br />举个例子，如下图：<br />![2021-05-04-10-40-47-329186.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620096311979-7f326d0c-f76d-4b40-8674-af8ac3b8dbd7.jpeg#clientId=u7f784eef-cdc9-4&from=ui&id=uca93b07a&originHeight=502&originWidth=521&originalType=binary&size=30912&status=done&style=shadow&taskId=u74123e3f-54fc-4df5-99ff-0ed3398f939)<br />图中发送方发送了很多个 packet，每个 packet 都有自己的序号，可以认为是 TCP 的序列号，其中 packet 3 在网络中丢失了，即使 packet 4-6 被接收方收到后，由于内核中的 TCP 数据不是连续的，于是接收方的应用层就无法从内核中读取到，只有等到 packet 3 重传后，接收方的应用层才可以从内核中读取到数据，这就是 HTTP/2 的队头阻塞问题，是在 TCP 层面发生的。
<a name="AGyoa"></a>
#### TCP 与 TLS 的握手时延迟
发起 HTTP 请求时，需要经过 TCP 三次握手和 TLS 四次握手（TLS 1.2）的过程，因此共需要 3 个 RTT 的时延才能发出请求数据。<br />![2021-05-04-10-40-47-608439.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620096323657-faac1656-fbcd-4988-8483-490a941628a1.jpeg#clientId=u7f784eef-cdc9-4&from=ui&id=u5fa9aded&originHeight=693&originWidth=777&originalType=binary&size=45783&status=done&style=shadow&taskId=u73d38408-893c-49e5-9303-59361113c69)<br />另外， TCP 由于具有「拥塞控制」的特性，所以刚建立连接的 TCP 会有个「慢启动」的过程，它会对 TCP 连接产生"减速"效果。
<a name="y4wMD"></a>
#### 网络迁移需要重新连接
一个 TCP 连接是由四元组（源 IP 地址，源端口，目标 IP 地址，目标端口）确定的，这意味着如果 IP 地址或者端口变动了，就会导致需要 TCP 与 TLS 重新握手，这不利于移动设备切换网络的场景，比如 4G 网络环境切换成 WIFI。<br />这些问题都是 TCP 协议固有的问题，无论应用层的 HTTP/2 在怎么设计都无法逃脱。<br />要解决这个问题，就必须把传输层协议替换成 UDP，这个大胆的决定，HTTP/3 做了！<br />![2021-05-04-10-40-47-812892.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620096335869-2d554ebc-e087-4274-a66d-03fca52f7f8e.jpeg#clientId=u7f784eef-cdc9-4&from=ui&id=uf497cf83&originHeight=327&originWidth=700&originalType=binary&size=25103&status=done&style=shadow&taskId=u21015835-506c-471e-bebd-36382a6a526)

---

<a name="EZ2aR"></a>
### QUIC 协议的特点
UDP 是一个简单、不可靠的传输协议，而且是 UDP 包之间是无序的，也没有依赖关系。<br />而且，UDP 是不需要连接的，也就不需要握手和挥手的过程，所以天然的就比 TCP 快。<br />当然，HTTP/3 不仅仅只是简单将传输协议替换成了 UDP，还基于 UDP 协议在「应用层」实现了 QUIC 协议，它具有类似 TCP 的连接管理、拥塞窗口、流量控制的网络特性，相当于将不可靠传输的 UDP 协议变成“可靠”的了，所以不用担心数据包丢失的问题。<br />QUIC 协议的优点有很多，这里举例几个，比如：

- 无队头阻塞；
- 更快的连接建立；
- 连接迁移；
<a name="LFV7W"></a>
#### 无队头阻塞
QUIC 协议也有类似 HTTP/2 Stream 与多路复用的概念，也是可以在同一条连接上并发传输多个 Stream，Stream 可以认为就是一条 HTTP 请求。<br />由于 QUIC 使用的传输协议是 UDP，UDP 不关心数据包的顺序，如果数据包丢失，UDP 也不关心。不过，QUIC 协议会保证数据包的可靠性，每个数据包都有一个序号唯一标识。<br />如果 QUIC 连接中的某个流中的一个数据包丢失了，只会阻塞该流，其他流不会受影响。这与 HTTP/2 不同，HTTP/2 只要某个流中的数据包丢失了，其他流也会因此受影响。<br />![2021-05-04-10-40-48-053249.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620096347543-b1aee71e-2e8c-4603-bc94-f7b0e895f34b.png#clientId=u7f784eef-cdc9-4&from=ui&id=u78600d29&originHeight=504&originWidth=700&originalType=binary&size=169476&status=done&style=shadow&taskId=uf5afcd58-bb17-4eec-aeeb-22606e17cef)<br />所以，QUIC 连接上的多个 Stream 之间并没有依赖，都是独立的，某个流发生丢包了，只会影响该流，其他流不受影响，消除了 HTTP/2 的队头阻塞问题。
<a name="PJBqT"></a>
#### 更快的连接建立
对于 HTTP/1 和 HTTP/2 协议，TCP 和 TLS 是分层的，分别属于内核实现的传输层、openssl 库实现的表示层，因此它们难以合并在一起，需要分批次来握手，先 TCP 握手，再 TLS 握手。<br />HTTP/3 在传输数据前虽然需要 QUIC 协议握手，这个握手过程只需要 1 RTT，握手的目的是为确认双方的「连接 ID」，连接迁移就是基于连接 ID 实现的。<br />但是 HTTP/3 的 QUIC 协议并不是与 TLS 分层，而是QUIC 内部包含了 TLS，它在自己的帧会携带 TLS 里的“记录”，再加上 QUIC 使用的是 TLS1.3，因此仅需 1 个 RTT 就可以「同时」完成建立连接与密钥协商，甚至在第二次连接的时候，应用数据包可以和 QUIC 握手信息（连接信息 + TLS 信息）一起发送，达到 0-RTT 的效果。<br />如下图右边部分，HTTP/3 当会话恢复时，有效负载数据与第一个数据包一起发送，可以做到 0-RTT：<br />![2021-05-04-10-40-48-431239.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1620096358378-12801915-c568-44b0-9e99-6ec15335e903.gif#clientId=u7f784eef-cdc9-4&from=ui&id=ue4b8912a&originHeight=381&originWidth=600&originalType=binary&size=447645&status=done&style=shadow&taskId=ud93285d6-7a0e-444b-a1ad-a8f41f3c299)
<a name="C747A"></a>
#### 连接迁移
在前面提到，基于 TCP 传输协议的 HTTP 协议，由于是通过四元组（源 IP、源端口、目的 IP、目的端口）确定一条 TCP 连接。<br />那么当移动设备的网络从 4G 切换到 WIFI 时，意味着 IP 地址变化了，就必须要断开连接，然后重新建立连接，而建立连接的过程包含 TCP 三次握手和 TLS 四次握手的时延，以及 TCP 慢启动的减速过程，给用户的感觉就是网络突然卡顿了一下，因此连接的迁移成本是很高的。<br />而 QUIC 协议没有用四元组的方式来“绑定”连接，而是通过连接 ID来标记通信的两个端点，客户端和服务器可以各自选择一组 ID 来标记自己。<br />因此，即使移动设备的网络变化后，导致 IP 地址变化了，只要仍保有上下文信息（比如连接 ID、TLS 密钥等），就可以“无缝”地复用原连接，消除重连的成本，没有丝毫卡顿感，达到了连接迁移的功能。

---

<a name="q5w4Z"></a>
### HTTP/3 协议
了解完 QUIC 协议的特点后，再来看看 HTTP/3 协议在 HTTP 这一层做了什么变化。<br />HTTP/3 同 HTTP/2 一样采用二进制帧的结构，不同的地方在于 HTTP/2 的二进制帧里需要定义 Stream，而 HTTP/3 自身不需要再定义 Stream，直接使用 QUIC 里的 Stream，于是 HTTP/3 的帧的结构也变简单了。<br />![2021-05-04-10-40-48-648658.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620096371663-9c7837eb-657a-45e7-854b-85075327444d.png#clientId=u7f784eef-cdc9-4&from=ui&id=u3740e8f4&originHeight=369&originWidth=1080&originalType=binary&size=81839&status=done&style=shadow&taskId=ub637db0d-8b53-4895-b750-2c7fd021b7b)<br />从上图可以看到，HTTP/3 帧头只有两个字段：类型和长度。<br />根据帧类型的不同，大体上分为数据帧和控制帧两大类，HEADERS 帧（HTTP 头部）和 DATA 帧（HTTP 包体）属于数据帧。<br />HTTP/3 在头部压缩算法这一方便也做了升级，升级成了 QPACK。与 HTTP/2 中的 HPACK 编码方式相似，HTTP/3 中的 QPACK 也采用了静态表、动态表及 Huffman 编码。<br />对于静态表的变化，HTTP/2 中的 HPACK 的静态表只有 61 项，而 HTTP/3 中的 QPACK 的静态表扩大到 91 项。<br />HTTP/2 和 HTTP/3 的 Huffman 编码并没有多大不同，但是动态表编解码方式不同。<br />所谓的动态表，在首次请求-响应后，双方会将未包含在静态表中的 Header 项更新各自的动态表，接着后续传输时仅用 1 个数字表示，然后对方可以根据这 1 个数字从动态表查到对应的数据，就不必每次都传输长长的数据，大大提升了编码效率。<br />可以看到，动态表是具有时序性的，如果首次出现的请求发生了丢包，后续的收到请求，对方就无法解码出 HPACK 头部，因为对方还没建立好动态表，因此后续的请求解码会阻塞到首次请求中丢失的数据包重传过来。<br />HTTP/3 的 QPACK 解决了这一问题，那它是如何解决的呢？<br />QUIC 会有两个特殊的单向流，所谓的单项流只有一端可以发送消息，双向则指两端都可以发送消息，传输 HTTP 消息时用的是双向流，这两个单向流的用法：

- 一个叫QPACK Encoder Stream， 用于将一个字典（key-value）传递给对方，比如面对不属于静态表的 HTTP 请求头部，客户端可以通过这个 Stream 发送字典；
- 一个叫QPACK Decoder Stream，用于响应对方，告诉它刚发的字典已经更新到自己的本地动态表了，后续就可以使用这个字典来编码了。

这两个特殊的单向流是用来同步双方的动态表，编码方收到解码方更新确认的通知后，才使用动态表编码 HTTP 头部。

---

<a name="xJfm6"></a>
### 总结
HTTP/2 虽然具有多个流并发传输的能力，但是传输层是 TCP 协议，于是存在以下缺陷：

- 队头阻塞，HTTP/2 多个请求跑在一个 TCP 连接中，如果序列号较低的 TCP 段在网络传输中丢失了，即使序列号较高的 TCP 段已经被接收了，应用层也无法从内核中读取到这部分数据，从 HTTP 视角看，就是多个请求被阻塞了；
- TCP 和 TLS 握手时延，TCL 三次握手和 TLS 四次握手，共有 3-RTT 的时延；
- 连接迁移需要重新连接，移动设备从 4G 网络环境切换到 WIFI 时，由于 TCP 是基于四元组来确认一条 TCP 连接的，那么网络环境变化后，就会导致 IP 地址或端口变化，于是 TCP 只能断开连接，然后再重新建立连接，切换网络环境的成本高；

HTTP/3 就将传输层从 TCP 替换成了 UDP，并在 UDP 协议上开发了 QUIC 协议，来保证数据的可靠传输。<br />QUIC 协议的特点：

- 无队头阻塞，QUIC 连接上的多个 Stream 之间并没有依赖，都是独立的，也不会有底层协议限制，某个流发生丢包了，只会影响该流，其他流不受影响；
- 建立连接速度快，因为 QUIC 内部包含 TLS1.3，因此仅需 1 个 RTT 就可以「同时」完成建立连接与 TLS 密钥协商，甚至在第二次连接的时候，应用数据包可以和 QUIC 握手信息（连接信息 + TLS 信息）一起发送，达到 0-RTT 的效果。
- 连接迁移，QUIC 协议没有用四元组的方式来“绑定”连接，而是通过连接 ID 来标记通信的两个端点，客户端和服务器可以各自选择一组 ID 来标记自己，因此即使移动设备的网络变化后，导致 IP 地址变化了，只要仍保有上下文信息（比如连接 ID、TLS 密钥等），就可以“无缝”地复用原连接，消除重连的成本；

另外 HTTP/3 的 QPACK 通过两个特殊的单向流来同步双方的动态表，解决了 HTTP/2 的 HPACK 队头阻塞问题。<br />不过，由于 QUIC 使用的是 UDP 传输协议，UDP 属于“二等公民”，大部分路由器在网络繁忙的时候，会丢掉 UDP包，把“空间”让给 TCP 包，所以 QUIC 的推广之路应该没那么简单。
