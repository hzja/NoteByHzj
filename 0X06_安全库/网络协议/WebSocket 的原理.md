<a name="JNZ9H"></a>
## 一、概述
由于 WebSocket 的协议的内容非常多，本文只会取其冰山一角进行简单阐述，不会铺开详细说。
<a name="m0Fkj"></a>
## 二、什么是 WebSocket
<a name="L3aeq"></a>
### 2.1 WebSocket 产生的背景
在 WebSocket 协议出现以前，创建一个和服务端进双通道通信的 web 应用，需要依赖HTTP协议，进行不停的轮询，这会导致一些问题：

- 服务端被迫维持来自每个客户端的大量不同的连接
- 大量的轮询请求会造成高开销，比如会带上多余的header，造成了无用的数据传输。

所以，为了解决这些问题，WebSocket 协议应运而生。
<a name="xTqB2"></a>
### 2.2 WebSocket 的定义
WebSocket 是一种在单个TCP连接上进行全双工通信的协议。WebSocket 使得客户端和服务器之间的数据交换变得更加简单，允许服务端主动向客户端推送数据。<br />在 WebSocket API 中，浏览器和服务器只需要完成一次握手，两者之间就直接可以创建持久性的连接， 并进行双向数据传输。（维基百科）
<a name="oU9R8"></a>
## 三、WebSocket 的基础帧结构分析
下图是参考 [RFC6455](https://datatracker.ietf.org/doc/html/rfc6455#section-5.2) 5.2章节的websocket 基础帧的数据结构图，接下里重点解析下数据结构图。<br />[![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1703046467626-9616c473-56f4-446b-b29f-f78a63d932b0.png#averageHue=%23fefefd&clientId=u192c7a9d-e73b-4&from=paste&height=380&id=u1bbeeae7&originHeight=949&originWidth=1710&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=119394&status=done&style=none&taskId=u011ca2f5-0d9c-47ae-95c2-50e0e96999a&title=&width=684)](https://datatracker.ietf.org/doc/html/rfc6455#section-5.2)

- FIN：占用1 bit，表示这是消息的最后一个片段。第一个片段也有可能是最后一个片段。
- RSV1，RSV2，RSV3：每个1 bit

必须设置为0，除非扩展了非0值含义的扩展。如果收到了一个非0值但是没有扩展任何非0值的含义，接收终端必须断开WebSocket连接。<br />Opcode：4 bit，操作码，如果收到一个未知的操作码，接收终端必须断开WebSocket连接。

- %x0 表示一个持续帧
- %x1 表示一个文本帧
- %x2 表示一个二进制帧
- %x3-7 预留给以后的非控制帧
- %x8 表示一个连接关闭包
- %x9 表示一个ping包
- %xA 表示一个pong包
- %xB-F 预留给以后的控制帧

Mask：1 bit，mask标志位，定义“有效负载数据”是否添加掩码。如果设置为1，那么掩码的键值存在于Masking-Key中。<br />Payload length：7 bits, 7+16 bits, or 7+64 bits，以字节为单位的“有效负载数据”长度。<br />Masking-Key：0 or 4 bytes，所有从客户端发往服务端的数据帧都已经与一个包含在这一帧中的32 bit的掩码进行过了运算。如果mask标志位（1 bit）为1，那么这个字段存在，如果标志位为0，那么这个字段不存在。<br />备注：载荷数据的长度，不包括mask key的长度。<br />Payload data：有效负载数据
<a name="L6kTa"></a>
### 为什么需要掩码？
为了安全，但并不是为了防止数据泄密，而是为了防止早期版本的协议中存在的代理缓存污染攻击（proxy cache poisoning attacks）等问题。
<a name="avi2X"></a>
## 四、 抓包分析
<a name="J7uE0"></a>
### 4.1 DEMO展示及分析
写了一个[DEMO](https://github.com/kinglaw1204/websocket/tree/master)用来抓包分析 websocket，源代码会放在文章末尾的链接。DEMO效果如下：<br />![DM_20231220110905_002.JPG](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1703041945498-7abee499-e944-4e04-b929-76fdc1a4f52d.jpeg#averageHue=%23fbfbfb&clientId=ubc26d167-63ff-4&from=ui&id=u416e128a&originHeight=344&originWidth=1694&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=7672&status=done&style=none&taskId=u42f30d61-760f-4680-bf09-aeafa530784&title=)<br />页面提供连接与断开功能，输入自己的名字发送，服务端返回Hello,名字！功能很简单，先看看页面的请求和响应。<br />请求：<br />![DM_20231220110905_003.JPG](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1703041921930-61de5d2d-ea13-4a68-b306-cf48064f70f0.jpeg#averageHue=%23f6f5f5&clientId=ubc26d167-63ff-4&from=ui&id=u302924f1&originHeight=383&originWidth=944&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=24628&status=done&style=none&taskId=u95cd31d0-ae51-4513-9087-8dd346a2b7f&title=)<br />响应：<br />![DM_20231220110905_004.JPG](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1703041921973-197728e6-f81f-4489-981d-8b42e41050a3.jpeg#averageHue=%23f9f8f8&clientId=ubc26d167-63ff-4&from=ui&id=uRXLX&originHeight=766&originWidth=1898&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=55594&status=done&style=none&taskId=u97393f7c-878d-4a14-ad1a-5a34e93a382&title=)<br />这里的请求与响应就是反应了 WebSocket 的一次握手，根据上图可以简单抽象一下 WebSocket 的请求和响应格式：客户端握手请求格式：
```
GET /chat HTTP/1.1
       Host: server.example.com
       Upgrade: websocket
       Connection: Upgrade
       Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
       Origin: http://example.com
       Sec-WebSocket-Protocol: chat, superchat
       Sec-WebSocket-Version: 13
```
服务端握手响应：
```
HTTP/1.1 101 Switching Protocols
        Upgrade: websocket
        Connection: Upgrade
        Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
        Sec-WebSocket-Protocol: chat
```
重点说明下结果请求字段：

- Upgrade：表示HTTP协议升级为webSocket
- connection：Upgrade 请求升级。
- Sec-WebSocket-Key：用于服务端进行标识认证，生成全局唯一id,GUID。
- Sec-WebSocket-Version：版本
- Sec-WebSocket-Protocol：请求服务端使用指定的子协议。如果指定了这个字段，服务器需要包含相同的字段，并且从子协议的之中选择一个值作为建立连接的响应。
- Sec-WebSocket-Extensions：WebSocket的扩展。
- Sec-WebSocket-Accept：s3pPLMBiTxaQ9kYGzzhZRbK+xOo= 生成的全局唯一id，GUID。
<a name="KaxB2"></a>
### 4.2 抓包
在DEMO中的操作流程如下：

- 连接WebSocket
- 发送“LUOZHOU”
- 断开连接

用 Wireshark 抓包如下：<br />![DM_20231220110905_005.JPG](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1703041896934-e38fae73-caad-4756-a2b8-87c18d1d1a9b.jpeg#averageHue=%23dcf6c1&clientId=ubc26d167-63ff-4&from=paste&height=436&id=u0268fd55&originHeight=1091&originWidth=1847&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=257402&status=done&style=none&taskId=u441a22bb-040a-4fa8-8898-30a2a8bb100&title=&width=738.8)<br />结合浏览器截图和抓包截图，发现在真正开启 websocket 之前，浏览器会有两次http请求，分别是：
```
A请求 GET /gs-guide-websocket/info?t=1551252237372 HTTP/1.1

B请求 GET /gs-guide-websocket/690/pdsz5x1q/websocket HTTP/1.1
```
根据 RFC6455 协议规定 WebSocket 只需要一次握手就可以完成，所以只需要分析第二次的http 握手请求，A请求应该是使用的框架层面自己实现。<br />根据截图可以知道，B请求对应的响应是序号 192 的数据，返回码是101，根据 HTTP 返回码可以知道，服务器已经理解了客户端的请求，并将通过Upgrade 消息头通知客户端采用不同的协议来完成这个请求。在发送完这个响应最后的空行后，服务器将会切换到在 Upgrade 消息头中定义的那些协议，也就是升级为 WebSocket 协议。所以接着193的包已经变成了 WebSocket 协议了。到这里，WebSocket 的握手连接就已经完成了。<br />接下来分析下发送消息的流程，这里大家肯定会疑惑，就发送了一条消息，为啥会有这么多 WebSocket 的包呢？其实这里多余的包是框架层面进行发送的，比如要进行订阅与发布的注册等等操作。所以真正使我们操作的包就只有断开连接的相关包和发送“LUOZHOU”的包<br />![DM_20231220110905_006.JPG](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1703041885345-1f51f62d-6fe9-432d-b4bc-97ce727664c5.jpeg#averageHue=%23e0f4c8&clientId=ubc26d167-63ff-4&from=paste&height=347&id=u10fcc4e8&originHeight=717&originWidth=1584&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=84528&status=done&style=none&taskId=uf3c376e2-502a-4078-b2dc-bd08db9fadb&title=&width=767.6000366210938)<br />根据上图可以发现 序号229的包是一个文本类型的包，opcode:1，然后采用了掩码处理，同时是最后一个处理包。仔细发现所有客户端发送服务端的包都会有[MASKED]标记，服务端返回的没有，这就说明了从客户端向服务端发送数据时，需要对数据进行掩码操作；从服务端向客户端发送数据时，不需要对数据进行掩码操作。
<a name="akW11"></a>
## 五、总结

- WebSocket 是为了在 web 应用上进行双通道通信而产生的协议，相比于轮询HTTP请求的方式，WebSocket 有节省服务器资源，效率高等优点。
- WebSocket 中的掩码是为了防止早期版本中存在中间缓存污染攻击等问题而设置的，客户端向服务端发送数据需要掩码，服务端向客户端发送数据不需要掩码。
- WebSocket 中 Sec-WebSocket-Key 的生成算法是拼接服务端和客户端生成的字符串，进行SHA1哈希算法，再用base64编码。
- WebSocket 协议握手是依靠 HTTP 协议的，依靠于 HTTP 响应101进行协议升级转换。
