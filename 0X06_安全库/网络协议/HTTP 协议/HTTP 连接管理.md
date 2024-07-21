网络 HTTP
<a name="b984U"></a>
## 搭载 HTTP 的 TCP
大家都知道，HTTP 这个应用层协议是以 TCP 为基础来传输数据的。想访问一个资源（资源在网络中就是一个 URL）时，需要先解析这个资源的 IP 地址和端口号，从而和这个 IP 和端口号所在的服务器建立 TCP 连接，然后 HTTP 客户端发起服务请求（GET）报文，服务器对服务器的请求报文做出响应，等到不需要交换报文时，客户端会关闭连接，下面的图很好的说明了这个过程。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628123126414-dd45b649-3041-44c8-ad19-eaeddd7ba93b.webp#clientId=u13d84b07-d46c-4&from=paste&id=u488c626e&originHeight=744&originWidth=784&originalType=url&ratio=1&status=done&style=shadow&taskId=uab53ceb3-1567-4082-8422-4bb8b1bc87a)<br />上面这幅图很好的说明了 HTTP 从建立连接开始 -> 发起请求报文 -> 关闭连接的全过程，但是上面这个过程还忽略了一个很重要的点，那就是TCP 建立连接的过程。<br />TCP 建立连接需要经过三次握手，交换三个报文，大家都对这个过程了然于胸了。<br />由于 HTTP 位于 TCP 的上层，所以 HTTP 的请求 -> 响应过程的时效性（性能）很大程度上取决于底层 TCP 的性能，只有在了解了 TCP 连接的性能之后，才可以更好的理解 HTTP 连接的性能，从而才能够实现高性能的 HTTP 应用程序。<br />通常把一次完整的请求 -> 相应过程称之为 HTTP 事务。<br />所以后面一般会写为 HTTP 事务，理解怎么回事就好。<br />接下来的重点要先从 TCP 的性能入手。
<a name="aQygv"></a>
### HTTP 时延损耗
再来回顾一下上面的 HTTP 事务的过程，有哪几个过程会造成 HTTP 事务时延呢？如下图所示<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628123126345-4c5f09a2-1bfd-4e91-b55f-4dca7ed026ce.webp#clientId=u13d84b07-d46c-4&from=paste&id=ua7ac3448&originHeight=325&originWidth=710&originalType=url&ratio=1&status=done&style=shadow&taskId=u32ca90db-8c58-4405-955d-ebbfa243653)<br />从图中可以看出，主要是有下面这几个因素影响 HTTP 事务的时延

1. 客户端会根据 URL 确定服务器的 IP 和端口号，这里主要是 DNS 把域名转换为 IP 地址的时延，DNS 会发起 DNS 查询，查询服务器的 IP 地址。
2. 第二个时延是 TCP 建立连接时会由客户端向服务器发送连接请求报文，并等待服务器回送响应报文的时延。每条新的 TCP 连接建立都会有建立时延。
3. 一旦连接建立后，客户端就会向服务器请求数据，这个时延主要是服务器从 TCP 连接中读取请求报文，并对请求进行处理的时延。
4. 服务器会向客户端传输响应报文的时延。
5. 最后一个时延是 TCP 连接关闭的时延。

其中最后一点的优化也是本文想要讨论的一个重点。
<a name="beROs"></a>
## HTTP 连接管理
试想一个问题，假设一个页面有五个资源（元素），每个资源都需要客户端打开一个 TCP 连接、获取资源、断开连接，而且每个连接都是串行打开的，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628123126376-9aa2496c-4adf-4659-839c-b6a96bc4d99c.webp#clientId=u13d84b07-d46c-4&from=paste&id=uce7e5bd0&originHeight=332&originWidth=749&originalType=url&ratio=1&status=done&style=shadow&taskId=ua0b79874-7fb9-4000-b349-7855c59f998)<br />串行的意思就是，这五个连接必须是有先后顺序，不会出现同时有两个以上的连接同时打开的情况。<br />上面五个资源就需要打开五条连接，资源少还好说，CPU 能够处理，如果页面资源达到上百或者更多的时候呢？每个资源还需要单独再打开一条连接吗？这样显然会急剧增加 CPU 的处理压力，造成大量的时延，显然是没有必要的。<br />串行还有一个缺点就是，有些浏览器在对象加载完毕之前是无法知道对象的尺寸（size）的，并且浏览器需要对象尺寸信息来将他们放在屏幕中合理的位置上，所以在加载了足够多的对象之前，屏幕是不会显示任何内容的，这就会造成，其实对象一直在加载，但是以为浏览器卡住了。<br />所以，有没有能够优化 HTTP 性能的方式呢？这个问题问得好，当然是有的。
<a name="I9FZl"></a>
### 并行连接
这是一种最常见的，也是最容易想到的一种连接方式，HTTP 允许客户端打开多条连接，并行执行多个 HTTP 事务，加入并行连接后，整个 HTTP 事务的请求过程是这样的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628123126388-da2ccc6a-6088-45c2-ae2f-0041e1fde6fd.webp#clientId=u13d84b07-d46c-4&from=paste&id=ud1c2093d&originHeight=776&originWidth=439&originalType=url&ratio=1&status=done&style=shadow&taskId=ue9fc45bf-0e95-4b43-8ad5-be02fcffc9d)<br />采用并行连接这种方式会克服单条连接的空载时间和带宽限制，因为每个事务都有连接，因此时延能够重叠起来，会提高页面的加载速度。<br />但是并行连接并不一定快，如果带宽不够的情况下，甚至页面响应速度还不如串行连接，因为在并行连接中，每个连接都会去竞争使用有效的带宽，每个对象都会以较慢的速度加载，有可能连接 1 加载了 95% ，连接 2 占用带宽加载了 80%，连接 3 ，连接 4 。。。。。。虽然每个对象都在加载，但是页面上却没有任何响应。<br />而且，打开大量连接会消耗很多内存资源，从而出现性能问题，上面讨论的就五个连接，这个还比较少，复杂的 web 页面有可能会有数十甚至数百个内嵌对象，也就是说，客户端可以打开数百个连接，而且有许多的客户端同时发出申请，这样很容易会成为性能瓶颈。<br />这样看来，并行连接并不一定"快"，实际上并行连接并没有加快页面的传输速度，并行连接也只是造成了一种假象，这是一切并行的通病。
<a name="Sv7VE"></a>
### 持久连接
Web 客户端通常会打开到同一个站点的连接，而且初始化了对某服务器请求的应用程序很可能会在不久的将来对这台服务器发起更多的请求，比如获取更多的图片。这种特性被称为站点局部性(site locality)。<br />因此，HTTP 1.1 以及 HTTP1.0 的允许 HTTP 在执行完一次事务之后将连接继续保持在打开状态，这个打开状态其实指的就是 TCP 的打开状态，以便于下一次的 HTTP 事务能够复用这条连接。<br />在一次 HTTP 事务结束之后仍旧保持打开状态的 TCP 连接被称为持久连接。<br />非持久连接会在每个事务结束之后关闭，相对的，持久连接会在每个事务结束之后继续保持打开状态。持久连接会在不同事务之间保持打开状态，直到客户端或者服务器决定将其关闭为止。<br />长连接也是有缺点的，如果单一客户端发起请求数量不是很频繁，但是连接的客户端却有很多的话，服务器早晚会有崩溃的时候。<br />持久连接一般有两种选型方式，一种是 HTTP 1.0 + keep-alive ；一种是 HTTP 1.1 + persistent。<br />HTTP 1.1 之前的版本默认连接都是非持久连接，如果想要在旧版本的 HTTP 上使用持久连接，需要指定 Connection 的值为 Keep-Alive。<br />HTTP 1.1 版本都是持久性连接，如果想要断开连接时，需要指定 Connection 的值为 close，这也是上面说的两种选型方式的版本因素。<br />下面是使用了持久连接之后的 HTTP 事务与使用串行 HTTP 事务连接的对比图<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628123126417-26dc5a19-58a0-4f83-9005-66d4fe7891e7.webp#clientId=u13d84b07-d46c-4&from=paste&id=u41263f22&originHeight=935&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u57187794-7d04-4bd1-a553-49237bdef2f)<br />这张图对比了 HTTP 事务在串行连接上和持久连接的时间损耗图，可以看到，HTTP 持久连接省去了连接打开 - 连接关闭的时间，所以在时间损耗上有所缩减。<br />在持久性连接中，还有一个非常有意思的地方，就是 Connection 选项，Connection 是一个通用选项，也就是客户端和服务端都具有的一个标头，下面是一个具有持久性连接的客户端和服务端的请求-响应图<br />![2021-08-05-08-24-44-580226.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628123213743-21b1e295-f627-42f6-9d8b-a7006545be10.jpeg#clientId=u13d84b07-d46c-4&from=ui&id=u820ec657&originHeight=329&originWidth=724&originalType=binary&ratio=1&size=26952&status=done&style=shadow&taskId=u5fc56837-0e71-43a5-b49b-b5916eb41c2)<br />从这张图可以看出，持久连接主要使用的就是 Connection 标头，这也就意味着，Connection 就是持久性连接的实现方式。所以下面主要讨论一下 Connection 这个大佬。
<a name="F5Rxa"></a>
#### Connection 标头
Connection 标头具有两种作用

- 和 Upgrade 一起使用进行协议升级
- 管理持久连接
<a name="hPseV"></a>
##### 和 Upgrade 一起使用进行协议升级
HTTP 提供了一种特殊的机制，这一机制允许将一个已建立的连接升级成新的协议，一般写法如下
```
GET /index.html HTTP/1.1
Host: www.example.com
Connection: upgrade
Upgrade: example/1, foo/2
```
HTTP/2 明确禁止使用此机制，这个机制只属于HTTP/1.1<br />也就是说，客户端发起 Connection:upgrade 就表明这是一个连接升级的请求，如果服务器决定升级这次连接，就会返回一个 101 Switching Protocols 响应状态码，和一个要切换到的协议的头部字段 Upgrade。如果服务器没有（或者不能）升级这次连接，它会忽略客户端发送的 Upgrade 头部字段，返回一个常规的响应：例如返回 200。
<a name="Y8NU0"></a>
##### 管理持久连接
上面说持久连接有两种方式，一种是 HTTP 1.0 + Keep-Alive ；一种是 HTTP 1.1 + persistent。
```
Connection: Keep-Alive
Keep-Alive: timeout=10,max=500
```
在 HTTP 1.0 + Keep-Alive 这种方式下，客户端可以通过包含 Connection：Keep-Alive 首部请求将一条连接保持在打开状态。<br />这里需要注意⚠️一点：Keep-Alive 首部只是将请求保持在活跃状态，发出 Keep-Alive 请求之后，客户端和服务器不一定会同意进行 Keep-Alive 会话。它们可以在任何时刻关闭空闲的 Keep-Alive 连接，并且客户端和服务器可以限制 Keep-Alive 连接所处理事务的数量。<br />Keep-Alive 这个标头有下面几种选项：

- `timeout`：这个参数估计了服务器希望将连接保持在活跃状态的时间。
- `max`：这个参数是跟在 timeout 参数后面的，它表示的是服务器还能够为多少个事务打开持久连接。

Keep-Alive 这个首部是可选的，但是只有在提供 Connection：Keep-Alive 时才能使用它。<br />Keep-Alive 的使用有一定限制，下面就来讨论一下 Keep-Alive 的使用限制问题。
<a name="avMNC"></a>
#### Keep-Alive 使用限制和规则

- 在 HTTP/1.0 中，Keep-Alive 并不是默认使用的，客户端必须发送一个 Connection：Keep-Alive 请求首部来激活 Keep-Alive 连接。
- 通过检测响应中是否含有 Connection：Keep-Alive 首部字段，客户端可以判断服务器是否在发出响应之后关闭连接。
- 代理和网管必须执行 Connection 首部规则，它们必须在将报文转发出去或者将缓存之前，删除 Connection 首部中的首部字段和 Connection 首部自身，因为 Connection 是一个 Hop-by-Hop 首部，这个首部说的是只对单次转发有效，会因为转发给缓存/代理服务器而失效。
- 严格来说，不应该与无法确定是否支持 Connection 首部的代理服务器建立 Keep-Alive 连接，以防止出现哑代理问题，哑代理问题下面会说。
<a name="FudAV"></a>
#### Keep-Alive 和哑代理问题
这里先解释一下什么是代理服务器，然后再说哑代理问题。<br />什么是代理服务器？<br />代理服务器就是代替客户端去获取网络信息的一种媒介，通俗一点就是网络信息的中转站。<br />为什么需要代理服务器？<br />最广泛的一种用处是需要使用代理服务器来替我们访问一些客户端无法直接访问的网站。除此之外，代理服务器还有很多功能，比如缓存功能，可以降低费用，节省带宽；对信息的实时监控和过滤，代理服务器相对于目标服务器（最终获取信息的服务器）来说，也是一个客户端，它能够获取服务器提供的信息，代理服务器相对于客户端来说，它是一个服务器，由它来决定提供哪些信息给客户端，以此来达到监控和过滤的功能。<br />哑代理问题出现就出现在代理服务器上，再细致一点就是出现在不能识别 Connection 首部的代理服务器，而且不知道在发出请求之后会删除 Connection 首部的代理服务器。<br />假设一个 Web 客户端正在通过一个哑代理服务器与 Web 服务器进行对话，如下图所示<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628123126980-1e94a692-3bf9-47df-a2b2-74184a553d06.webp#clientId=u13d84b07-d46c-4&from=paste&id=u073faec1&originHeight=618&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u3787ff5f-a3f2-4358-a21b-a2c00e43d07)<br />来解释一下上面这幅图

- 首先，Web 客户端向代理发送了一条报文，其中包含了 Connection: Keep-Alive 首部，希望在这次 HTTP 事务之后继续保持活跃状态，然后客户端等待响应，以确定对方是否允许持久连接。
- 哑代理（这里先界定为哑代理是不妥的，往往先看做的事，再给这件事定性，现在这个服务器还没做出哑代理行为呢，就给他定性了）收到了这条 HTTP 请求，但它不理解 Connection 首部，它也不知道 Keep-Alive 是什么意思，因此只是沿着转发链路将报文发送给服务器，但 Connection 首部是个 Hop-by-Hop 首部，只适用于单条链路传输，所以这个代理服务器不应该再将其发送给服务器了，但是它还是发送了，后面就会发生一些难顶的事情。
- 经过转发的 HTTP 请求到达服务器后，会误以为对方希望保持 Keep-Alive 持久连接，经过评估后，服务器作出响应，它同意进行 Keep-Alive 对话，所以它回送了一个 Connection：Keep-Alive 响应并到达了哑代理服务器。
- 哑代理服务器会直接将响应发送给客户端，客户端收到响应后，就知道服务器可以使用持久连接。然而，此时客户端和服务器都知道要使用 Keep-Alive 持久连接，但是哑代理服务器却对 Keep-Alive 一无所知。
- 由于代理对 Keep-Alive 一无所知，所以会收到的所有数据都会发送给客户端，然后等待服务器关闭连接，但是代理服务器却认为应该保持打开状态，所以不会去关闭连接。这样，哑代理服务器就一直挂在那里等待连接的关闭。
- 等到客户端发送下一个 HTTP 事务后，哑代理会直接忽视新的 HTTP 事务，因为它并不认为一条连接上还会有其他请求的到来，所以会直接忽略新的请求。

这就是 Keep-Alive 的哑代理。<br />那么如何解决这个问题呢？用 Proxy-Connection
<a name="q2yl1"></a>
#### Proxy-Connection 解决哑代理
网景公司提出了一种使用 Proxy-Connection 标头的办法，首先浏览器会向代理发送 Proxy-Connection 扩展首部，而不是官方支持的 Connection 首部。如果代理服务器是哑代理的话，它会直接将 Proxy-Connection 发送给服务器，而服务器收到 Proxy-Connection 的话，就会忽略这个首部，这样不会带来任何问题。如果是一个聪明的代理服务器，在收到 Proxy-Connection 的时候，就会直接将 Connection 替换掉 Proxy-Connection ，再发送给服务器。
<a name="pdliG"></a>
#### HTTP/1.1 持久连接
HTTP/1.1 逐渐停止了对 Keep-Alive 连接的支持，用一种名为 persistent connection 的改进型设计取代了 Keep-Alive ，这种改进型设计也是持久连接，不过比 HTTP/1.0 的工作机制更优。<br />与 HTTP/1.0 的 Keep-Alive 连接不同，HTTP/1.1 在默认情况下使用的就是持久连接。除非特别指明，否则 HTTP/1.1 会假定所有连接都是持久连接。如果想要在事务结束后关闭连接的话，就需要在报文中显示添加一个 Connection：close 首部。这是与以前的 HTTP 协议版本很重要的区别。<br />使用 persistent connection 也会有一些限制和规则：

- 首先，发送了 Connection: close 请求后，客户端就无法在这条连接上发送更多的请求。这同时也可以说，如果客户端不想发送其他请求，就可以使用 Connection：close 关闭连接。
- HTTP/1.1 的代理必须能够分别管理客户端和服务器的持久连接 ，每个持久连接都只适用于单次传输。
- 客户端对任何服务器或者代理最好只维护两条持久连接，以防止服务器过载。
- 只有实体部分的长度和相应的 Content-Length保持一致时，或者使用分块传输编码的方式时，连接才能保持长久。
<a name="Pqf0P"></a>
### 管道化连接
HTTP/1.1 允许在持久连接上使用请求管道。这是相对于 Keep-Alive 连接的又一个性能优化。管道就是一个承载 HTTP 请求的载体，可以将多个 HTTP 请求放入管道，这样能够降低网络的环回时间，提升性能。下图是使用串行连接、并行连接、管道化连接的示意图：<br />![2021-08-05-08-24-44-937279.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628123328712-faffb879-9a46-42f8-a6e3-a392ebc7bc99.jpeg#clientId=u13d84b07-d46c-4&from=ui&id=u6c7ee119&originHeight=1161&originWidth=1080&originalType=binary&ratio=1&size=198153&status=done&style=shadow&taskId=u62463615-39df-4e72-aa83-103c0f45890)<br />使用管道化的连接也有几处限制：

- 如果 HTTP 客户端无法确认连接是持久的，就不应该使用管道。
- 必须按照与请求的相同顺序回送 HTTP 响应，因为 HTTP 没有序号这个概念，所以一旦响应失序，就没办法将其与请求匹配起来了。
- HTTP 客户端必须做好连接会在任何时刻关闭的准备，还要准备好重发所有未完成的管道化请求。
<a name="aZQtt"></a>
## HTTP 关闭连接
所有 HTTP 客户端、服务器或者代理都可以在任意时刻关闭一条 HTTP 传输连接。通常情况下会在一次响应后关闭连接，但是保不准也会在 HTTP 事务的过程中出现。<br />但是，服务器无法确定在关闭的那一刻，客户端有没有数据要发送，如果出现这种情况，客户端就会在进行数据传输的过程中发生了写入错误。<br />即使在不出错的情况下，连接也可以在任意时刻关闭。如果在事务传输的过程中出现了连接关闭情况，就需要重新打开连接进行重试。如果是单条连接还好说，如果是管道化连接，就比较糟糕，因为管道化连接会把大量的连接丢在管道中，此时如果服务器关闭，就会造成大量的连接未响应，需要重新调度。<br />如果一个 HTTP 事务不管执行一次还是执行 n 次，它得到的结果始终是一样的，那么就认为这个事务是幂等的，一般 GET、HEAD、PUT、DELETE、TRACE 和 OPTIONS方法都认为是幂等的。客户端不应该以管道化的方式发送任何非幂等请求，比如 POST，否则就会造成不确定的后果。<br />由于 HTTP 使用 TCP 作为传输层的协议，所以 HTTP 关闭连接其实还是 TCP 关闭连接的过程。<br />HTTP 关闭连接一共分为三种情况：完全关闭、半关闭和正常关闭。<br />应用程序可以关闭 TCP 输入和输出信道中的任何一个，或者将二者同时关闭。调用套接字 close() 方法会讲输入和输出同时关闭，这就被称为完全关闭。还可以调用套接字的 shutdown 方法单独关闭输入或者输出信道，这被称为半关闭。HTTP 规范建议当客户端和服务器突然需要关闭连接的时候，应该正常关闭，但是它没有说如何去做。
