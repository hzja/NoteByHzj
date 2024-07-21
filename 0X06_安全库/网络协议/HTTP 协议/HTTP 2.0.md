网络 HTTP 2.0
<a name="kJN5d"></a>
## 最初的 HTTP
HTTP 刚刚诞生之初只用于 web 端的内容获取，一般就是用于页面访问，那个时候的页面内容还不如现在这样丰富，交互场景也不是很多，也没有庞大繁杂的 CSS、JS ，页面加载速度非常快。但是随着 web 2.0 的出现以及更多的内容被展示、更精美的排版、更多的用户交互场景一起出现，导致页面的内容越来越大，使得页面加载速度越来越慢。
<a name="i0QMj"></a>
### HTTP 的瓶颈
影响一个 HTTP 网络请求的因素主要有两个：**带宽和延迟**。<br />首先来说一下带宽，如果还停留在拨号上网阶段的话，带宽很容易出现瓶颈，因为单位时间内传输的数据量很小。但是现在随着光纤等通信技术的不断发展，10Mbps、100Mbps、甚至 1000 Mbps 进入了每个家庭，不用再担心带宽成为网络请求的瓶颈了。<br />那么剩下的就只剩下延迟了。<br />延迟主要有下面三个方面：

- 浏览器阻塞（HOL blocking）：浏览器会因为一些原因阻塞请求。浏览器对于同一个域名，同时只能有 4 个连接（这个根据浏览器内核不同可能会有所差异），超过浏览器最大连接数限制，后续请求就会被阻塞。
- DNS 查询（DNS Lookup）：浏览器需要知道目标服务器的 IP 才能建立连接。将域名解析为 IP 的这个系统就是 DNS。这个通常可以利用 DNS 缓存结果来达到减少这个时间的目的。
- 建立连接（Initial connection）：HTTP 是基于 TCP 协议的，浏览器最快也要在第三次握手时才能捎带 HTTP 请求报文，达到真正的建立连接，但是这些连接无法复用会导致每次请求都经历三次握手和慢启动。三次握手在高延迟的场景下影响较明显，慢启动则对文件类大请求影响较大。

HTTP 1.0 有一个被抱怨最多的是**连接无法复用**，当每次有新的请求时都会重新经历一次三次握手和四次挥手过程，并且连接的建立和释放需要耗费大量的服务器资源，在请求少的页面还尚能应对，不过随着请求的不断增多，HTTP 1.0 越来越难顶。<br />不过，HTTP 1.0 协议头里可以设置 Connection:Keep-Alive。在 header 里设置 Keep-Alive 可以在一定时间内复用连接，具体复用时间的长短可以由服务器控制，一般在 15s 左右。到 HTTP 1.1 之后 Connection 的默认值就是Keep-Alive，如果要关闭连接复用需要显式的设置 Connection:Close。<br />HTTP 还有一个被抱怨最多的问题就是它的**队头阻塞(head of blocking)**，队头阻塞问题会导致带宽无法充分利用，导致后续的请求被阻塞。<br />假如有五个请求被同时发出，如果第一个请求没有处理完成，就会导致后续的请求也无法得到处理，如下图所示<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635986681977-6ddffbce-19e9-4cad-bd64-c97e7aa13115.webp#clientId=u9375b2d6-27d7-4&from=paste&id=u772da752&originHeight=1017&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u09dcd8c3-845c-41a4-a91f-907f6701efe)<br />如果第一个请求没有被处理，那么 2 3 4 5 这四个请求会直接阻塞在客户端，等到请求 1 被处理完毕后，才能逐个发出。网络通畅的时候性能影响不大，不过一旦请求 1 因为某些原因没有抵达服务器，或者请求因为网络阻塞没有及时返回，影响的就是所有后续请求，导致后续请求无限阻塞下去，问题就变得比较严重了。<br />不过在 HTTP 1.1 中，也提出了**流水线(pipelining)**的设计，pipelining 就被用来解决队头阻塞的问题，如下图所示<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635986681987-5f14ffc8-20b2-43ea-8c22-8f97fb688bf9.webp#clientId=u9375b2d6-27d7-4&from=paste&id=udd06e095&originHeight=872&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u4421e964-8878-4e19-ba76-52230b48f2a)<br />虽然这种流水线的设计乍一看像是能够解决阻塞问题，因为右图中这三个请求没有等到响应到达后再进行发送，而是直接依次发送，但是实际上，并不是那么回事。<br />pipelining 并不是救世主，它也存在不少缺陷：

- 因为只有幂等的请求比如 GET、HEAD 才能使用 pipelining ，非幂等请求比如 POST 则不能使用，因为请求之间可能存在先后依赖关系。
- 其实队头阻塞问题并没有完全解决，因为服务器返回的响应还是要依次返回，也就是返回的请求时 FIFO - 先发先回。
- 绝大多数 HTTP 代理服务器不支持 pipelining。
- 和不支持 pipelining 的老服务器协商有问题。

正是因为有这么多的问题，各大浏览器厂商要么是根本就不支持 pipelining，要么就是默认关掉了 pipelining 机制，而且启用的条件十分苛刻。
<a name="XC7q1"></a>
## SPDY
虽然 HTTP1.0 和 HTTP 1.1 存在这么多问题，业界也是想出了各种优化手段，但是这些手段怎么说呢，都是治标不治本，直到 2020 年 Google 提出了 SPDY 的方案，大家才开始从正面看待和解决老版本 HTTP 协议本身的问题，这也直接加速了 HTTP 2.0 的诞生。<br />先来聊一下 SPDY 是什么，它都有哪些特点？
<a name="FjERh"></a>
### 认识 SPDY
SPDY 的目标在于解决 HTTP 的缺陷，即延迟和安全性。上面一直在讨论延迟，至于安全性，虽然上面没有具体聊，不过 HTTP 的明文传输确是个问题。如果以降低延迟为目标，应用层的 HTTP 和传输层的 TCP 都是都有调整的空间，不过 TCP 作为更底层协议存在已达数十年之久，其实现已深植全球的网络基础设施当中，如果要动必然伤经动骨，业界响应度必然不高，所以 SPDY 的手术刀对准的是 HTTP 。

- 降低延迟，客户端的单连接单请求，服务端的 FIFO 响应队列都是延迟的大头。
- HTTP 最初设计都是客户端发起请求，然后服务端进行响应，服务端无法主动发送内容到客户端。
- 压缩 HTTP header，HTTP 1.x 的 header 越来越膨胀，cookie 和 user agent 很容易让 header 的 size 增至1kb 大小甚至更多。而且由于 HTTP 的无状态特性，header 必须每次请求都重复携带，很浪费流量。

为了增加解决这些问题的可行性，聪明的 Google 一开始就避开了从传输层动手，而且打算利用开源社区的力量以提高扩散的力度，对于协议使用者来说，也只需要在请求的 header 里设置 user agent，然后在服务端做好支持即可，极大的降低了部署的难度。SPDY 的设计如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635986682031-1f260baf-8ae5-4654-a124-a9c9fbabfdbe.webp#clientId=u9375b2d6-27d7-4&from=paste&id=u423921ed&originHeight=799&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u5302ba26-ca73-4fb4-abdd-33f6811137b)<br />可以看到，SPDY 位于 HTTP 之下，SSL 之上，这样可以轻松的兼容老版本的 HTTP 协议，SPDY 的功能分为基础功能和高级功能两部分，基础功能是默认启用的，高级功能需要手动启用。
<a name="FeTYS"></a>
### SPDY 基础功能

- **多路复用(multiplexing)**，多路复用通过多个请求共用一个连接的方式，降低了 TCP 连接建立和释放的开销，同时提高了带宽的利用率。
- **请求优先级(request prioritization)**，多路复用带来的一个问题是，在共享连接的基础上会存在一些关键请求被阻塞，SPDY 允许给每个请求设置优先级，这样重要的请求就会优先得到响应。
- **header 压缩**，前面提到的 HTTP 1.x 的 header 很多时候都是重复而且多余的。选择合适的压缩算法可以减小包的大小和数量。SPDY 对 header 的压缩率可以达到 80% 以上。
<a name="xB1R2"></a>
### SPDY 高级功能

- 服务端推送，HTTP 只能由客户端发送，服务器只能被动发送响应。不过在开启服务端推送后，服务端通过 **X-Associated-Content** header 会告知服务器会有新的内容被推送过来，
- 服务端暗示，和服务端推送所不同的是，服务端暗示不会推送内容，只是告诉客户端有新的内容产生，，内容的下载还是需要客户端主动发起请求。服务端暗示通过 X-Subresources header 来通知，一般应用场景是客户端需要先查询服务端状态，然后再下载资源，可以节约一次查询请求。

自动 SPDY 出现后，页面加载时间相比于 HTTP 减少了 64%，而且各大浏览器厂商在 SPDY 诞生之后的 1 年多时间里也都陆续支持了 SPDY。但是，SPDY 的生存时间却没有人们想象中的那么长，SPDY 从 2012 年诞生到 2016 年停止维护，如果 HTTP 2.0 没有诞生，相信 Google 可能会收到更多的真实反馈和数据，但是 SPDY 在这段时间里也完成了自己的使命。
<a name="xbR9x"></a>
## 初探 HTTP 2.0
HTTP 2.0 也被写作 HTTP/2 ，它是超文本传输协议的 2.0 版本，因为 SPDY 的流行让 IETF 看到了优化后的效果，以及可以通过修改协议层来优化 HTTP，所以 IETF 开始决定正式考虑制定 HTTP 2.0 的计划，而且，SPDY的部分设计人员也被邀请参与了 HTTP 2.0 的设计。<br />HTTP2.0 在设计之初就与 SPDY 的设计目的和出发点不同，SPDY 更像是 Google 自家的一个产品，相当于自家的一个玩具，怎么玩儿都行，而 HTTP 2.0 在设计之初就是为了普适性的这个目的，所以，一开始任何的设计都会关系到以后的维护问题，如果有什么瑕疵或者不足的地方可能会影响巨大，所以考虑的问题角度要非常严谨和慎重。<br />HTTP 2.0 在设计之初就有一些重要的前提：

- 客户端向服务器发送请求的这种基本模型不会改变。
- 原有的协议头不会改变，使用 http:// 和 https:// 的服务和应用不会做任何修改，不会有 http2://。
- 使用 HTTP 1.x 的客户端和服务器可以平滑升级到 HTTP 2.0 上。
- 不识别 HTTP 2.0 的代理服务器可以将请求降级到 HTTP 1.x。

客户端在和服务器确定是使用 HTTP1.x 还是 HTTP 2.0 之前，需要先确定对方是否支持 HTTP 2.0，所以这里必须要先进行协商，也就是客户端询问服务器，这样一来一回就多了一个 RTT 的延迟。对 HTTP 1.x 的修改就是为了降低延迟，现在又多了一个 RTT，这样显然是无法接受的。Google 制定 SPDY 协议的时候也遇到了这个问题，他们采取的做法是强制协商在 SSL 层完成，还因此制定了一个 TLS 的拓展，叫做 **NPN(Next Protocol Negotiation)**。虽然 HTTP 2.0 也采用了相同的方式，不过经过讨论后，最终 HTTP 2.0 没有强制要走 SSL 层，HTTP 2.0 没有使用 NPN，却制定了一个 TLS 的拓展叫做 **ALPN(Application Layer Protocol Negotiation)**，现在，SPDY 也打算迁移到 ALPN 了。
<a name="tsNCf"></a>
## HTTP 2.0 的主要变化
HTTP 2.0 自从设计到诞生以来，发生了很多变化，不过对于开发人员和厂商来说，影响比较大的就几点：
<a name="T9VUn"></a>
### 二进制格式
HTTP 1.x 的诞生使用的是明文协议，它的格式主要由三部分构成：请求行(request line) 、请求头(header) 和报文体(body)，要识别这三部分必须要做协议解析，而协议解析是基于文本的，基于文本的解析存在多样性的缺陷，而二进制格式只能识别 0 和 1 ，比较固定，基于这种考量，HTTP 2.0 决定采用二进制格式，实现方便而且健壮性强。<br />下面这幅图很好的诠释了 HTTP1.x 和 HTTP 2.0 使用的不同报文格式。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635986682027-eff8c05d-9110-48d1-bb82-ac84f5e7b7d1.webp#clientId=u9375b2d6-27d7-4&from=paste&id=ue70b4247&originHeight=485&originWidth=747&originalType=url&ratio=1&status=done&style=shadow&taskId=udcb9bd7b-0022-4e71-b4e0-dbd54ca0ea4)<br />在 HTTP 2.0 报文中，length 定义了整个 _frame_ 的开始到结束，type 定了 frame 的类型，一种有十种，flags 定义了一些重要的参数，stream id 用作流控制，剩下的 payload 就是 request 的正文。<br />虽然 HTTP 2.0 报文格式看上去和 HTTP 1.x 的完全不同，但是实际上 HTTP 2.0 并没有改变 HTTP 1.x 的语义，它只是在 HTTP 1.x 的基础上封装了一层，如下图所示<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635986682144-7cc2df42-8910-4145-b17e-49aa7ee85bee.webp#clientId=u9375b2d6-27d7-4&from=paste&id=u114b771e&originHeight=547&originWidth=845&originalType=url&ratio=1&status=done&style=shadow&taskId=u7595e88b-5367-4516-9dac-15426850225)<br />从上图可以看到，HTTP 1.x 中的请求行、请求头被 HTTP 2.0 封装成为了 _HEADERS Frame_，而 HTTP 1.x 中的报文体被 HTTP 2.0 封装成为了 _Data Frame_。调试的时候浏览器会把 HTTP 2.0 的 frame 自动还原成 HTTP 1.x的格式。
<a name="hGg7N"></a>
### 连接共享
上面聊到，HTTP 1.x 并没有真正意义上的解决连接复用问题，所以 HTTP 2.0 要解决的一大难题就是**连接共享(MultiPlexing)**，连接共享意味着客户端与服务器之间也只需要一个连接即可，这样即使来自很多流的数据包也能够混合在一起通过同样连接传输，再根据不同帧首部的 stream id 标识符重新连接将不同的数据流进行组装。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635986682416-c0db5267-9031-4a67-912b-21c0f48836e4.webp#clientId=u9375b2d6-27d7-4&from=paste&id=u3f5199a0&originHeight=196&originWidth=804&originalType=url&ratio=1&status=done&style=shadow&taskId=u018f8b5a-602a-4eaa-9b9f-09f30af5c1b)<br />什么是 stream？<br />stream 是连接中的一个虚拟信道，可以承载双向消息传输。每个流有唯一整数标识符。为了防止两端 streaam id 冲突，客户端发起的流具有奇数 id，服务器端发起的流具有偶数 id。<br />上面提到 HTTP 1.x 没有真正解决连接共享还有一个主要的因素就是**无法对不同的请求设置优先级**，这样会导致关键请求被阻塞。而 HTTP 2.0 可以对不同的 stream 设置不同的优先级，stream 之间也可以设置依赖，依赖和优先级都可以动态调整，这样就会解决关键请求被阻塞的问题。
<a name="XiXIQ"></a>
### 头部压缩
上面还聊到了 HTTP1.x 中的 header 由于 cookie 和 user agent 不存在记忆性，这样导致每次都要带着这些头重新发送请求，HTTP 2.0 使用 _encoder_ 来减少传输的 header 大小，通信双方会各自缓存一份 header 字段表，这样能够避免重复传输 header ，也能够减小传输的大小。HTTP 2.0 采用的是 _HPACK_ 压缩算法。<br />这种压缩算法的主要思想可以参考官方文档 https://httpwg.org/specs/rfc7541.html
<a name="pJ8ja"></a>
### 服务端推送
**服务端推送(Server Push)** 上面也已经聊过，HTTP 2.0 能够以推的方式将客户端的内容预先发送出去，正因为没有发起请求，建立连接等操作，所以静态资源通过服务端推送的方式可以极大地提升速度。服务端推送还有一个更大的优势：**缓存**，缓存也能够在不同页面之间共享缓存资源。<br />需要注意下面几个点：<br />1、推送遵循同源策略；<br />2、这种服务端的推送是基于客户端的请求响应来确定的。<br />当服务端需要主动推送某个资源时，便会发送一个 Frame Type 为 PUSH_PROMISE 的 frame ，里面带了 PUSH 需要新建的 stream id。意思是告诉客户端：接下来要用这个 id 向你发送东西，客户端准备好接着。客户端解析 frame 时，发现它是一个 PUSH_PROMISE 类型，便会准备接收服务端要推送的流。
<a name="Gn6ex"></a>
## HTTP 2.0 的缺陷
HTTP 2.0 最惊艳的莫过于多路复用了，虽然多路复用有种种好处，但是大家可以想一下，多路复用虽然好，但是它是建立在 TCP 连接的基础上，在连接频繁的情况下，是不是会对 TCP 连接造成压力，这个角度来讲，TCP 很容易成为性能瓶颈。<br />还有一点，使用 HTTP 2.0 会增加一次 TLS 握手过程，增加 RTT，这个上面也说到了。<br />在 HTTP 2.0 中，多个请求是在同一个 TCP 管道中，这样当 HTTP 2.0 出现丢包时，整个 TCP 都要开始等待重传，那么就会阻塞该 TCP。连接中的所有请求。
