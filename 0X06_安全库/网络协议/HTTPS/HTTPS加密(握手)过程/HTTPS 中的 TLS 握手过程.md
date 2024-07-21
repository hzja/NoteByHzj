网络 HTTPS TLS<br />TLSv1.2 握手过程基本都是需要四次，也就是需要经过 2-RTT 才能完成握手，然后才能发送请求，而 TLSv1.3 只需要 1-RTT 就能完成 TLS 握手，如下图。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1627982054171-98e06fca-bf03-4b88-a836-ab6b449f1bef.webp#clientId=u12339891-ded3-4&from=paste&id=u5618fed1&originHeight=685&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=udcc4ed8d-11b8-4c0e-99ba-9e1cd031ed7)<br />一般情况下，不管 TLS 握手次数如何，都得先经过 TCP 三次握手后才能进行，因为 HTTPS 都是基于 TCP 传输协议实现的，得先建立完可靠的 TCP 连接才能做 TLS 握手的事情。<br />这个场景是可能发生的，但是需要在特定的条件下才可能发生，如果没有说任何前提条件，说这句话就是在耍流氓。<br />那到底什么条件下，这个场景才能发生呢？需要下面这两个条件同时满足才可以：

- 客户端和服务端都开启了 TCP Fast Open 功能，且 TLS 版本是 1.3；
- 客户端和服务端已经完成过一次通信。

那具体怎么做到的呢？先了解些 TCP Fast Open 功能和 TLSv1.3 的特性。
<a name="NU1lc"></a>
### TCP Fast Open
先来了解下什么是 TCP Fast Open？<br />常规的情况下，如果要使用 TCP 传输协议进行通信，则客户端和服务端通信之前，先要经过 TCP 三次握手后，建立完可靠的 TCP 连接后，客户端才能将数据发送给服务端。<br />其中，TCP 的第一次和第二次握手是不能够携带数据的，而 TCP 的第三次握手是可以携带数据的，因为这时候客户端的 TCP 连接状态已经是 ESTABLISHED，表明客户端这一方已经完成了 TCP 连接建立。<br />![2021-08-03-17-15-35-252941.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627982179917-42a283cc-9de7-41b7-a58c-d778d8c5c7d0.png#clientId=u12339891-ded3-4&from=ui&id=u44639765&originHeight=330&originWidth=600&originalType=binary&ratio=1&size=34619&status=done&style=shadow&taskId=ue4512d6c-e10b-4026-912f-e9f2b05f6cf)<br />就算客户端携带数据的第三次握手在网络中丢失了，客户端在一定时间内没有收到服务端对该数据的应答报文，就会触发超时重传机制，然后客户端重传该携带数据的第三次握手的报文，直到重传次数达到系统的阈值，客户端就会销毁该 TCP 连接。<br />说完常规的 TCP 连接后，再来看看 TCP Fast Open。<br />TCP Fast Open 是为了绕过 TCP 三次握手发送数据，在 Linux 3.7 内核版本之后，提供了 TCP Fast Open 功能，这个功能可以减少 TCP 连接建立的时延。<br />要使用 TCP Fast Open 功能，客户端和服务端都要同时支持才会生效。<br />不过，开启了 TCP Fast Open 功能，想要绕过 TCP 三次握手发送数据，得建立第二次以后的通信过程。<br />在客户端首次建立连接时的过程，如下图：<br />![2021-08-03-17-15-35-364945.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627982162253-fe9c6207-bda3-403c-a94a-6eefc24799db.png#clientId=u12339891-ded3-4&from=ui&id=uf1c2c020&originHeight=310&originWidth=600&originalType=binary&ratio=1&size=36815&status=done&style=shadow&taskId=u72758cea-b215-48e8-bc92-2a61f756f18)<br />具体介绍：

- 客户端发送 SYN 报文，该报文包含 Fast Open 选项，且该选项的 Cookie 为空，这表明客户端请求 Fast Open Cookie；
- 支持 TCP Fast Open 的服务器生成 Cookie，并将其置于 SYN-ACK 报文中的 Fast Open 选项以发回客户端；
- 客户端收到 SYN-ACK 后，本地缓存 Fast Open 选项中的 Cookie。

所以，第一次客户端和服务端通信的时候，还是需要正常的三次握手流程。随后，客户端就有了 Cookie 这个东西，它可以用来向服务器 TCP 证明先前与客户端 IP 地址的三向握手已成功完成。<br />对于客户端与服务端的后续通信，客户端可以在第一次握手的时候携带应用数据，从而达到绕过三次握手发送数据的效果，整个过程如下图：<br />![2021-08-03-17-15-35-470971.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627982153747-cc557e7d-c23f-4f7c-bb9f-61d69fb88f9c.png#clientId=u12339891-ded3-4&from=ui&id=ubb95ad98&originHeight=365&originWidth=600&originalType=binary&ratio=1&size=41775&status=done&style=shadow&taskId=u2c2bdc65-db8d-4a42-b849-cad5e8959f4)<br />详细介绍下这个过程：

- 客户端发送 SYN 报文，该报文可以携带「应用数据」以及此前记录的 Cookie；
- 支持 TCP Fast Open 的服务器会对收到 Cookie 进行校验：如果 Cookie 有效，服务器将在 SYN-ACK 报文中对 SYN 和「数据」进行确认，服务器随后将「应用数据」递送给对应的应用程序；如果 Cookie 无效，服务器将丢弃 SYN 报文中包含的「应用数据」，且其随后发出的 SYN-ACK 报文将只确认 SYN 的对应序列号；
- 如果服务器接受了 SYN 报文中的「应用数据」，服务器可在握手完成之前发送「响应数据」，这就减少了握手带来的 1 个 RTT 的时间消耗；
- 客户端将发送 ACK 确认服务器发回的 SYN 以及「应用数据」，但如果客户端在初始的 SYN 报文中发送的「应用数据」没有被确认，则客户端将重新发送「应用数据」；
- 此后的 TCP 连接的数据传输过程和非 TCP Fast Open 的正常情况一致。

所以，如果客户端和服务端同时支持 TCP Fast Open 功能，那么在完成首次通信过程后，后续客户端与服务端 的通信则可以绕过三次握手发送数据，这就减少了握手带来的 1 个 RTT 的时间消耗。
<a name="GlFVT"></a>
### TLSv1.3
说完 TCP Fast Open，再来看看 TLSv1.3。<br />在最开始的时候，也提到 TLSv1.3 握手过程只需 1-RTT 的时间，它到整个握手过程，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1627982053733-1ebfe462-1082-464a-8aab-dd68bd77bb58.png#clientId=u12339891-ded3-4&from=paste&id=ue929be68&originHeight=570&originWidth=585&originalType=url&ratio=1&status=done&style=none&taskId=ubd32a452-36c0-40d2-a809-5e490a3f367)<br />TCP 连接的第三次握手是可以携带数据的，如果客户端在第三次握手发送了 TLSv1.3 第一次握手数据，是不是就表示「HTTPS 中的 TLS 握手过程可以同时进行三次握手」？。<br />不是的，因为服务端只有在收到客户端的 TCP 的第三次握手后，才能和客户端进行后续 TLSv1.3 握手。<br />TLSv1.3 还有个更厉害到地方在于会话恢复机制，在重连 TLvS1.3 只需要 0-RTT，用“pre_shared_key”和“early_data”扩展，在 TCP 连接后立即就建立安全连接发送加密消息，过程如下图：<br />![2021-08-03-17-15-35-688943.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627982144894-104ed33f-4cf7-4033-ba9a-76211633b1f4.png#clientId=u12339891-ded3-4&from=ui&id=uea5287d1&originHeight=557&originWidth=1080&originalType=binary&ratio=1&size=61114&status=done&style=shadow&taskId=u3b3cd15c-6177-4d27-8306-d0d472e6ef7)
<a name="Ow5Gp"></a>
### TCP Fast Open + TLSv1.3
在前面可以知道，客户端和服务端同时支持 TCP Fast Open 功能的情况下，在第二次以后到通信过程中，客户端可以绕过三次握手直接发送数据，而且服务端也不需要等收到第三次握手后才发送数据。<br />如果 HTTPS 的 TLS 版本是 1.3，那么 TLS 过程只需要 1-RTT。<br />因此如果「TCP Fast Open + TLSv1.3」情况下，在第二次以后的通信过程中，TLS 和 TCP 的握手过程是可以同时进行的。<br />如果基于 TCP Fast Open 场景下的 TLSv1.3 0-RTT 会话恢复过程，不仅 TLS 和 TCP 的握手过程是可以同时进行的，而且 HTTP 请求也可以在这期间内一同完成。
<a name="m4eDE"></a>
### 总结
「HTTPS 是先进行 TCP 三次握手，再进行 TLSv1.2 四次握手」，这句话没问题。<br />「HTTPS 中的 TLS 握手过程可以同时进行三次握手」，这个场景是可能存在到，但是在没有说任何前提条件，需要下面这两个条件同时满足才可以：

- 客户端和服务端都开启了 TCP Fast Open 功能，且 TLS 版本是 1.3；
- 客户端和服务端已经完成过一次通信；

下面这个 TLSv1.2 的 基于 RSA 算法的四次握手过程：<br />![2021-08-03-17-15-35-023941.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627982247381-44f2dd9b-d93b-46f0-b2a2-d991b226d1ba.png#clientId=u12339891-ded3-4&from=ui&id=ue30ad37e&originHeight=406&originWidth=1080&originalType=binary&ratio=1&size=257501&status=done&style=shadow&taskId=u605323ad-e3e3-4aba-b97e-12e9a1a141a)<br />先三次握手，再进行 TLS 四次握手。
