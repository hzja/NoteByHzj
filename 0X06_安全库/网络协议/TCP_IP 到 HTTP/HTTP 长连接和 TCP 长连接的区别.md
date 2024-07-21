HTTPTCP<br />事实上，**这两个完全是两样不同东西**，实现的层面也不同：

- HTTP 的 Keep-Alive，是由**应用层（用户态）** 实现的，称为 HTTP 长连接；
- TCP 的 Keepalive，是由 **TCP 层（内核态）** 实现的，称为 TCP 保活机制；

接下来，分别说说它们。
<a name="wLlXG"></a>
## HTTP 的 Keep-Alive
HTTP 协议采用的是「请求-应答」的模式，也就是客户端发起了请求，服务端才会返回响应，一来一回这样子。<br />![请求-应答](https://cdn.nlark.com/yuque/0/2022/png/396745/1669960820628-580ed332-90dd-4f0a-b44d-e9716a68926b.png#averageHue=%23fcfbf8&clientId=u144509c2-c58a-4&from=paste&id=u9fc5b225&originHeight=467&originWidth=386&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u2e00f982-cd7c-43f7-b26f-a7342ecaa93&title=%E8%AF%B7%E6%B1%82-%E5%BA%94%E7%AD%94 "请求-应答")<br />由于 HTTP 是基于 TCP 传输协议实现的，客户端与服务端要进行 HTTP 通信前，需要先建立 TCP 连接，然后客户端发送 HTTP  请求，服务端收到后就返回响应，至此「请求-应答」的模式就完成了，随后就会释放 TCP 连接。<br />![一个 HTTP 请求](https://cdn.nlark.com/yuque/0/2022/png/396745/1669960820539-d3a28297-8cd6-46fc-a321-1f1ee2231979.png#averageHue=%23f9f2ec&clientId=u144509c2-c58a-4&from=paste&id=u0aaff6c2&originHeight=722&originWidth=386&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u34811148-e94e-4b80-9485-d2f52faae04&title=%E4%B8%80%E4%B8%AA%20HTTP%20%E8%AF%B7%E6%B1%82 "一个 HTTP 请求")<br />如果每次请求都要经历这样的过程：建立 TCP -> 请求资源 -> 响应资源 -> 释放连接，那么此方式就是 **HTTP 短连接**，如下图：<br />![HTTP 短连接](https://cdn.nlark.com/yuque/0/2022/png/396745/1669960820622-a8cab44c-5b19-4772-a2c1-04ab6496659d.png#averageHue=%23f8f0e8&clientId=u144509c2-c58a-4&from=paste&id=u5bb87c00&originHeight=1067&originWidth=386&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u93adf984-8816-4bf5-88cf-42f0d77e54d&title=HTTP%20%E7%9F%AD%E8%BF%9E%E6%8E%A5 "HTTP 短连接")<br />这样实在太累人了，一次连接只能请求一次资源。<br />能不能在第一个 HTTP 请求完后，先不断开 TCP 连接，让后续的 HTTP 请求继续使用此连接？<br />当然可以，HTTP 的 Keep-Alive 就是实现了这个功能，可以使用同一个 TCP 连接来发送和接收多个 HTTP 请求/应答，避免了连接建立和释放的开销，这个方法称为 **HTTP 长连接**。<br />![HTTP 长连接](https://cdn.nlark.com/yuque/0/2022/png/396745/1669960820611-731e85db-938d-4ef4-a768-05d0424da596.png#averageHue=%23faf6f1&clientId=u144509c2-c58a-4&from=paste&id=ud9145fe1&originHeight=1067&originWidth=386&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ud288b6d4-8382-459e-9a53-5893bc34c80&title=HTTP%20%E9%95%BF%E8%BF%9E%E6%8E%A5 "HTTP 长连接")<br />HTTP 长连接的特点是，只要任意一端没有明确提出断开连接，则保持 TCP 连接状态。<br />怎么才能使用 HTTP 的 Keep-Alive 功能？<br />在 HTTP 1.0 中默认是关闭的，如果浏览器要开启 Keep-Alive，它必须在请求的包头中添加：
```
Connection: Keep-Alive
```
然后当服务器收到请求，作出回应的时候，它也添加一个头在响应中：
```
Connection: Keep-Alive
```
这样做，连接就不会中断，而是保持连接。当客户端发送另一个请求时，它会使用同一个连接。这一直继续到客户端或服务器端提出断开连接。<br />**从 HTTP 1.1 开始， 就默认是开启了 Keep-Alive**，如果要关闭 Keep-Alive，需要在 HTTP 请求的包头里添加：
```
Connection:close
```
现在大多数浏览器都默认是使用 HTTP/1.1，所以 Keep-Alive 都是默认打开的。一旦客户端和服务端达成协议，那么长连接就建立好了。<br />HTTP 长连接不仅仅减少了 TCP 连接资源的开销，而且这给 **HTTP 流水线**技术提供了可实现的基础。<br />所谓的 HTTP 流水线，是**客户端可以先一次性发送多个请求，而在发送过程中不需先等待服务器的回应**，可以减少整体的响应时间。<br />举例来说，客户端需要请求两个资源。以前的做法是，在同一个 TCP 连接里面，先发送 A 请求，然后等待服务器做出回应，收到后再发出 B 请求。HTTP 流水线机制则允许客户端同时发出 A 请求和 B 请求。<br />![右边为 HTTP 流水线机制](https://cdn.nlark.com/yuque/0/2022/png/396745/1669960820612-80152929-1b51-4c81-b570-72374521765d.png#averageHue=%23fbf7f3&clientId=u144509c2-c58a-4&from=paste&id=ubb22d5f2&originHeight=1025&originWidth=926&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ua3fdeb94-cdc1-4530-b3e6-38257dfdd7d&title=%E5%8F%B3%E8%BE%B9%E4%B8%BA%20HTTP%20%E6%B5%81%E6%B0%B4%E7%BA%BF%E6%9C%BA%E5%88%B6 "右边为 HTTP 流水线机制")<br />但是**服务器还是按照顺序响应**，先回应 A 请求，完成后再回应 B 请求。<br />而且要等服务器响应完客户端第一批发送的请求后，客户端才能发出下一批的请求，也就说如果服务器响应的过程发生了阻塞，那么客户端就无法发出下一批的请求，此时就造成了「队头阻塞」的问题。<br />可能有的同学会问，如果使用了 HTTP 长连接，如果客户端完成一个 HTTP 请求后，就不再发起新的请求，此时这个 TCP 连接一直占用着不是挺浪费资源的吗？<br />对没错，所以为了避免资源浪费的情况，web 服务软件一般都会提供 keepalive_timeout 参数，用来指定 HTTP 长连接的超时时间。<br />比如设置了 HTTP 长连接的超时时间是 60 秒，web 服务软件就会**启动一个定时器**，如果客户端在完后一个 HTTP 请求后，在 60 秒内都没有再发起新的请求，**定时器的时间一到，就会触发回调函数来释放该连接。**<br />![HTTP 长连接超时](https://cdn.nlark.com/yuque/0/2022/png/396745/1669960821048-217bd2cd-75fa-4a55-b949-b10b8329e4fa.png#averageHue=%23f9f6f4&clientId=u144509c2-c58a-4&from=paste&id=u8842c00e&originHeight=947&originWidth=708&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uaedb0457-5606-4db4-b302-141f02f703b&title=HTTP%20%E9%95%BF%E8%BF%9E%E6%8E%A5%E8%B6%85%E6%97%B6 "HTTP 长连接超时")
<a name="cPsx3"></a>
## TCP 的 Keepalive
TCP 的 Keepalive 这东西其实就是 **TCP 的保活机制**：<br />如果两端的 TCP 连接一直没有数据交互，达到了触发 TCP 保活机制的条件，那么内核里的 TCP 协议栈就会发送探测报文。

- 如果对端程序是正常工作的。当 TCP 保活的探测报文发送给对端, 对端会正常响应，这样 **TCP 保活时间会被重置**，等待下一个 TCP 保活时间的到来。
- 如果对端主机崩溃，或对端由于其他原因导致报文不可达。当 TCP 保活的探测报文发送给对端后，石沉大海，没有响应，连续几次，达到保活探测次数后，**TCP 会报告该 TCP 连接已经死亡**。

所以，TCP 保活机制可以在双方没有数据交互的情况，通过探测报文，来确定对方的 TCP 连接是否存活，这个工作是在内核完成的。<br />![TCP 保活机制](https://cdn.nlark.com/yuque/0/2022/png/396745/1669960821289-246c3c00-eb20-4196-b045-08db80ec8e01.png#averageHue=%23fbfaf8&clientId=u144509c2-c58a-4&from=paste&id=uc21a3683&originHeight=810&originWidth=833&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u7aec9470-9795-44ac-8891-87ab6f4cdda&title=TCP%20%E4%BF%9D%E6%B4%BB%E6%9C%BA%E5%88%B6 "TCP 保活机制")<br />注意，应用程序若想使用 TCP 保活机制需要通过 socket 接口设置 SO_KEEPALIVE 选项才能够生效，如果没有设置，那么就无法使用 TCP 保活机制。
<a name="lHcXP"></a>
## 总结
HTTP 的 Keep-Alive 也叫 HTTP 长连接，该功能是由「应用程序」实现的，可以使得用同一个 TCP 连接来发送和接收多个 HTTP 请求/应答，减少了 HTTP 短连接带来的多次 TCP 连接建立和释放的开销。<br />TCP 的 Keepalive 也叫 TCP 保活机制，该功能是由「内核」实现的，当客户端和服务端长达一定时间没有进行数据交互时，内核为了确保该连接是否还有效，就会发送探测报文，来检测对方是否还在线，然后来决定是否要关闭该连接。
