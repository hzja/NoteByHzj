网络协议TCP/IPHTTP
<a name="CnfJk"></a>
## 1、TCP/IP
<a name="uqMIf"></a>
### 1.1  TCP/IP 定义
**TCP/IP** 协议族是一组协议的集合，也叫**互联网协议族**，计算机之间只有遵守这些规则，才能进行通信。**TCP** 和 **IP** 只是其中2个重要的协议，所以用 **TCP/IP** 来命名这个互联网协议族，实际上他大致包括四层协议。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921071-e09f17a2-c3e2-4b24-8d7e-204f923f097f.png#clientId=u38f601e0-1422-4&from=paste&id=ue56bf845&originHeight=313&originWidth=882&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua4b2b8a9-0e63-4578-8731-f32690b7df7&title=)
<a name="oacSP"></a>
### 1.2 TCP/IP  功能
上文说过 TCP/IP 宏观上分为四层，接下来说下四层的具体作用。
<a name="zxqXt"></a>
#### 1.2.1. 应用层
应用层 为用户直接提供不同的网络服务协议，比如 HTTP、Email、FTP 等，这些协议都是为了解决实际生活中不同的需求而产生的协议。用户大部分时间也是在此层操作跟组装数据，说白了就是socket 编程！至于具体的数据是如何进行网络传输的，是由下面的三层负责的。
<a name="pqvzF"></a>
#### 1.2.2. 传输层
传输层为应用层提供通信服务，属于面向通信部分的最高层，也是用户功能中的最底层。传输层为相互通信的应用进程提供了逻辑通信。主要包括 TCP 协议和 UDP 协议。

1. TCP 提供面向连接的数据流支持、可靠性、流量控制、多路复用等服务。
2. UDP 不提供复杂的控制机制。

**传输层的作用：**

1. 分段及封装应用层送来的数据。
2. 提供端到端的传输服务。
3. 在发送主机与接收主机之间构建逻辑通信。
<a name="qvNPc"></a>
#### 1.2.3. 网络层
网络层功能是实现数据包的**选路**和**转发**。广域网通常使用众多分级的路由器来连接分散的主机或局域网，因此，通信的两台主机一般是通过多个中间节点路由器连接的。网络层的任务就是选择这些中间节点，以确定两台主机之间的通信路径。同时对上层协议隐藏了网络拓扑连接的细节，使得在传输层和网络应用程序看来，通信的双方是直接相连的。<br />IP 协议即处于这一层，提供路由和寻址的功能，使两终端系统能够互连且决定最佳路径，並具有一定的拥塞控制和流量控制的能力。
<a name="dLInN"></a>
#### 1.2.4. 链路层
数据链路层实现了网卡接口的网络驱动程序，以处理数据在物理媒介上的传输。数据链路层两个常用的协议是ARP协议（Address Resolve Protocol，地址解析协议）和 RARP 协议（ReverseAddress Resolve Protocol，逆地址解析协议）。它们实现了 IP 地址和机器物理 MAC 地址之间的相互转换。
<a name="adtBb"></a>
#### 1.2.5 数据传输
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921018-795d0f23-d27e-4e12-9deb-9f3f1d431140.png#clientId=u38f601e0-1422-4&from=paste&id=uafe41ce6&originHeight=297&originWidth=618&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u245ca300-0b36-4190-99b2-e7ffc637929&title=)

1. 利用 TCP/IP 协议族进行网络通信时，会通过分层顺序与对方进行通信。发送端从应用层往下走，接收端则从链路层往上走。
2. 发送端在层与层之间传输数据时，每经过一层时必定会被打上一个该层所属的首部信息。反之，接受端在层与层传输数据时，每经过一层时会把对应的首部消去。
3. 这种把数据信息包装起来的做法成为封装。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921115-3f36e67b-176c-4097-bc98-a385b1754184.png#clientId=u38f601e0-1422-4&from=paste&id=uc8ce0974&originHeight=191&originWidth=573&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u86433c79-db46-46a5-acd5-b5d34ed4566&title=)<br />但是需注意一点， **IP** 层是有 **Maximum Transmission Unit** 最大传输单元 **MTU** 限制的，同理一次数据传输时 **TCP** 层是有 **Maximum Segment Size** 最大报文段长度 **MSS** 限制的，<br />以太网的MTU是1500，基本IP首部长度为20，TCP首部是20，所以MSS的值最大可达1460(MSS不包括协议首部，只包含应用数据)。<br />所以一个大的应用层信息传输时候可能会被切分若干块然后逐个传输。接收方收到每个包的应用层数据再组装成应用层数据，然后一个请求才算接收完成，这也是 **Content-Length** 字段存在的意义。<br />![数据分包发送](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921054-a6d3677b-b105-4a90-923a-f5dabe5c431d.png#clientId=u38f601e0-1422-4&from=paste&id=u3e4c6104&originHeight=233&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ud8859b7c-72e7-4e09-96aa-29ed52b52c3&title=%E6%95%B0%E6%8D%AE%E5%88%86%E5%8C%85%E5%8F%91%E9%80%81 "数据分包发送")
<a name="Jafuv"></a>
### 1.3 OSI 跟 TCP/IP
**OSI**

- OSI又称 **开放式系统互联通信参考模型** ，是由国际标准化组织提出的一种概念模型，一个试图使各种计算机在世界范围内互连为网络的标准框架，它注重通信协议必要的功能是什么。

**TCP/IP**

- 现实生活中真正的网络传输通讯协议，注重在计算机上实现协议应该开发哪种程序。

**OSI 跟 TCP/IP 区别**

1. OSI 引入了服务、接口、协议、分层的概念，TCP/IP 借鉴了 OSI 的这些概念建立 TCP/IP 模型。
2. OSI 先有模型后有协议，先有标准后进行实践。
3. TCP/IP 先有协议和应用再提出了模型，且是参照的OSI模型。
4. OSI 是一种理论下的模型，而 TCP/IP 已被广泛使用，成为网络互联事实上的标准。

介绍完宏观的TCP/IP 协议簇后，接下来**从上到下**进入网络的世界吧。
<a name="aNAbL"></a>
## 2、应用层 HTTP
<a name="Vb8T1"></a>
### 2.1  HTTP 简单了解
<a name="rKiEI"></a>
#### 2.1.1 HTTP 定义
**HyperText Transfer Protocol**，又名 超文本传输协议。HTTP 是对计算机世界里任意两点之间传输文字、图片、音视频等超文本数据的约定和规范。<br />![HTTP](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921471-1b884b20-f0fc-457c-a62d-30d23e79a735.png#clientId=u38f601e0-1422-4&from=paste&id=u68982a83&originHeight=113&originWidth=509&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucda720e3-50b7-461a-9087-ade2e4baeb0&title=HTTP "HTTP")
<a name="ndVre"></a>
#### 2.1.2 URI、 URN 、URL
**URI**：Uniform Resource Identifier 统一资源标志符，表示的是web上每一种可用的资源，URI只是一种**概念**，怎样实现无所谓，重点在于标识一个资源。<br />**URN** ：Universal Resource Name 统一资源名称，通过特定命名空间中的唯一名称或ID来标识资源。<br />**URL**：Universal Resource Locator 统一资源定位符，URL 其实是 URI 的一种子集，不仅标识了一个资源还告诉了如何访问它，一个标准的URL必须包括：protocol、host、port、path。<br />![URL模板](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921524-55a31147-98cf-4be9-8f06-694eccbdfb7d.png#clientId=u38f601e0-1422-4&from=paste&id=ua9d49fc6&originHeight=69&originWidth=480&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucfd27a98-d1f7-4426-b6f6-11f8aae801c&title=URL%E6%A8%A1%E6%9D%BF "URL模板")

1. **protocol**：通讯双方采用什么协议交流，HTTP、ftp、file等
2. **IP**：服务器的真实IP地址。
3. **Port**：服务资源在IP机器上暴露的端口。
4. **path**：资源在服务器上的存放路径，一般就是文件或者访问目录。
5. **query**：可选配置，用&分割，参数以KV方式存储。

**举例三者关系**：

1. 你想去找一个人，此处人就是一种资源 URI。
2. 如果用身份证号 + 名字去找就是URN，身份证号 + 名字只标识了人这个资源，但无法确认资源的地址。
3. 如果用地址：XX省XX市XX区XX单元XX房间的住户 就是URL，不仅标识人这个资源，而且定位了其地址
<a name="nq1ce"></a>
### 2.2 HTTP 报文格式
请求 和 响应 报文都由 起始行、头部、空行、实体四个部分组成，只不过 起始行 稍有不同。
<a name="sgyu4"></a>
#### 2.2.1 请求
![请求报文格式](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921679-514b24ef-e23d-46c3-bce8-bc98587fb663.png#clientId=u38f601e0-1422-4&from=paste&id=u9eaab571&originHeight=281&originWidth=486&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u1e3ec6b3-aa5e-497d-8cab-db03ee79610&title=%E8%AF%B7%E6%B1%82%E6%8A%A5%E6%96%87%E6%A0%BC%E5%BC%8F "请求报文格式")
<a name="e58R8"></a>
##### 2.2.1.1  请求行
请求行又包含3个部分：**请求方法、URL、协议版本**。它们之间用空格分开，请求行最后以一个回车符 + 一个换行符结尾。<br />**请求方法**：表明想对目标资源进行何种操作，HTTP1.1 定义了下表中列出的 8 种请求方法，其中最常用的是 GET 和 POST。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921745-483d3dbc-8e4a-4363-bed1-10f53202957d.png#clientId=u38f601e0-1422-4&from=paste&id=u857bb8dd&originHeight=377&originWidth=730&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc42295a6-b64d-4d4e-83c3-6f11b5521b7&title=)<br />**URL**：指定就是本次访问的目标地址。<br />**协议版本**：指定了客户端当前支持的 HTTP 版本，HTTP 目前通用的有 1.1、 2.0、3.0 三个版本，如果请求方指定了 1.1，应答方收到之后也会使用 HTTP 1.1 协议进行回复。
<a name="aop4J"></a>
##### 2.2.1.2 请求头
请求头部 用来告知服务器该请求和客户端本身的一些额外信息，每个请求头都是一个**键值对**，键和值之间用英文冒号隔开。每个请求头单独形成一行，它们的末尾都是一个回车符和换行符。在所有的请求头中，只有 **Host** 是必需的，其它请求头都是可选的，列举一些常见请求头：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921864-86a328ce-179d-4bbf-808d-dad5a33c531e.png#clientId=u38f601e0-1422-4&from=paste&id=uc5f810de&originHeight=472&originWidth=1007&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u45b19cd6-c927-4f69-949b-2c50be9e427&title=)
<a name="hf5TC"></a>
##### 2.2.1.3 空行
只包含一个回车符和一个换行符，不包含其它任何内容。这个空行用于标记请求头部已结束，它是必须要有的。
<a name="qwxPn"></a>
##### 2.2.1.4 请求体
一般就是用户自定义的 信息体了，在消息头中可以通过 Content-Type 指定类型。
<a name="ECEGi"></a>
##### 2.2.1.5 请求样例
![请求样例](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921918-e69c8399-993c-45bb-ae69-39daf025f7d3.png#clientId=u38f601e0-1422-4&from=paste&id=uf8eedc82&originHeight=413&originWidth=791&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua9762001-f656-48cd-bcc1-0dad4bd1249&title=%E8%AF%B7%E6%B1%82%E6%A0%B7%E4%BE%8B "请求样例")
<a name="b0IhT"></a>
#### 2.2.2 响应
![响应报文格式](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295921932-f7ab8e23-7324-41bb-a5ca-21998ac3c4c5.png#clientId=u38f601e0-1422-4&from=paste&id=u3dbe5024&originHeight=285&originWidth=485&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u6bbee8d4-a187-42f8-a8d1-f92a77a79b2&title=%E5%93%8D%E5%BA%94%E6%8A%A5%E6%96%87%E6%A0%BC%E5%BC%8F "响应报文格式")
<a name="Dkigz"></a>
##### 2.2.2.1  响应行
指定返回信息对应的 HTTP 版本、响应信息状态码、简单原因。
<a name="r54rn"></a>
##### 2.2.2.2  响应头
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922058-9a7c7fdd-2873-4994-9d01-a0d64953f889.png#clientId=u38f601e0-1422-4&from=paste&id=u20bae3c6&originHeight=235&originWidth=1001&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u69086e02-1ae2-4032-ad1c-08acff9530c&title=)<br />至于空行跟消息体几乎跟跟请求类似，而消息体类型是由 **Content-Type** 指定的。
<a name="BRZXT"></a>
##### 2.2.2.3 响应样例
![响应样例](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922279-84f5cb3a-5f9f-4134-82af-c089f12e0fc9.png#clientId=u38f601e0-1422-4&from=paste&id=u2a9196e6&originHeight=329&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uaa13b01a-3f35-4335-9eb4-05bce83142e&title=%E5%93%8D%E5%BA%94%E6%A0%B7%E4%BE%8B "响应样例")
<a name="zdG2D"></a>
### 2.3 HTTP 头字段
HTTP 协议规定了非常多的头字段，可以实现各种各样的功能，但基本上可以分为以下四类：

1. **通用字段**：在请求头和响应头里都可以出现。
2. **请求字段**：仅能出现在请求头里，进一步说明请求信息或者额外的附加条件。
3. **响应字段**：仅能出现在响应头里，补充说明响应报文的信息。
4. **实体字段**：它实际上属于通用字段，但专门描述 body 的额外信息。

通过对 HTTP 头字段的设置，HTTP 提供了如下几个重要功能：

1. **内容协商**：客户端跟服务端就响应的资源内容约定好，比如语言、字符集、编码方式、压缩类型。
2. **缓存管理** : 针对资源特性可进行资源是否缓存到客户端，注意 max-age、no-cache、no-store、must-revalidate 之间区别。
3. **实体类型**：通过解析 Content-Type 来获得请求跟响应的 MIME 类型。
4. **连接管理**：通过读取配置参数实现长短连接。
<a name="hBSIy"></a>
### 2.4 HTTPS 跟 HTTP
HTTP 是明文传输的，存在如下几个风险：

1. **窃听风险**：信息保密性，比如通信链路上可以获取通信内容。
2. **篡改风险**：信息完整性，比如强制入垃圾广告。
3. **冒充风险**：身份识别，比如杂牌网址冒充淘宝等购物网站。
<a name="XtVvX"></a>
#### 2.4.1 SSL/TLS 概述
![SSL/TLS](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922293-861ff560-5814-42fa-b2ec-82ab302fcb80.png#clientId=u38f601e0-1422-4&from=paste&id=ub50887f7&originHeight=435&originWidth=478&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uc1f8b91b-f0d0-40d7-890c-3b7cbcf90c8&title=SSL%2FTLS "SSL/TLS")<br />为了保证安全性 HTTPS 应运而生，HTTPS 在 HTTP 与 TCP 层之间加入了 **SSL/TLS** 加密协议，可以解决上述三个问题。

1. 通过混合加密实现了信息的机密性。
2. 通过摘要算法的方式来实现完整性，它能够为数据生成独一无二的序列号。
3. 将服务器公钥放入到数字证书中，解决了冒充的风险。

这里需注意一般 HTTP 默认是 80 端口，而 HTTPS 默认 443 端口。
<a name="ADTxi"></a>
#### 2.4.2 加密算法
加密算法 分为 对称加密 跟 非对称加密。

1. **对称加密**：加密解密使用一个密钥，运算速度快，密钥必须保密，无法做到安全的密钥交换。常见加密算法有 AES、DES、RC4、BlowFish 等。
2. **非对称加密**：使用**公钥**和**私钥**两个秘钥，公钥可以任意分发而私钥保密，解决了密钥交换问题但速度慢。私钥到公钥的推导过程是单向的，可保证私钥的安全性。常见加密算法有 RSA、 DSA、Diffie-Hellman等。

HTTPS 采用的是 对称加密  +  非对称加密  = 混合加密方式：

1. 在通信建立前采用**非对称加密**的方式交换秘钥，后续就不再使用非对称加密。
2. 在通信过程中全部使用**对称加密**的会话秘钥的方式加密明文数据。
<a name="X4e8e"></a>
#### 2.4.3 摘要算法
摘要算法的主要特征是加密过程不需要密钥，并且经过加密的数据无法被解密，目前可以被解密逆向的只有CRC32算法，只有输入相同的明文数据经过相同的消息摘要算法才能得到相同的密文。<br />消息摘要算法主要应用在数字签名领域，作为对明文的摘要算法。著名的摘要算法有RSA公司的**MD5**算法和 **SHA-1** 算法及其大量的变体。<br />![校验完整性](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922391-5ce9beb2-24fe-469d-b545-10c26bf67ba6.png#clientId=u38f601e0-1422-4&from=paste&id=u7e124ed9&originHeight=190&originWidth=943&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u64b68ef5-318b-4c4d-87b2-ada9c938949&title=%E6%A0%A1%E9%AA%8C%E5%AE%8C%E6%95%B4%E6%80%A7 "校验完整性")

1. 客户端将明文数据通过指定的摘要算法生成摘要。
2. 明文数据 + 摘要算法 经过公钥加密后传输。
3. 服务器收到信息后用私钥解密信息得到明文 + 摘要。
4. 服务器通过相同的摘要算法对明文生成摘要。
5. 对比客户端跟服务器生成的两个摘要是否一样，以此检测数据是否完整。
<a name="RGpiB"></a>
#### 2.4.4 CA 证书
非对称加密时，客户端保存公钥，如何确保公钥的准确性是个难题，如果有人窃取服务器公钥搞事情，那么整个数据传输过程中客户端跟服务器是感知不到第三方存在，但信息却早就泄露了！<br />![非对称加密信息泄露](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922651-a25ae493-4efd-4914-a3ae-c630971d8712.png#clientId=u38f601e0-1422-4&from=paste&id=ud48ea1b7&originHeight=404&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3bb9f174-a45f-4e88-8ce5-9d6aa2dc962&title=%E9%9D%9E%E5%AF%B9%E7%A7%B0%E5%8A%A0%E5%AF%86%E4%BF%A1%E6%81%AF%E6%B3%84%E9%9C%B2 "非对称加密信息泄露")<br />问题的关键就是**如何保证客户端收到的是服务器的公钥**！此时 **数字证书**就出现了，它就是基于上上面所说的私钥加密数据，公钥解密来验证其身份。<br />![CA确保公钥正确传输](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922532-fbc68190-fcdf-4bc6-96e6-71d852c1460f.png#clientId=u38f601e0-1422-4&from=paste&id=udcd66d3a&originHeight=362&originWidth=912&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u8bc0ed57-8190-4316-a38f-b510688185a&title=CA%E7%A1%AE%E4%BF%9D%E5%85%AC%E9%92%A5%E6%AD%A3%E7%A1%AE%E4%BC%A0%E8%BE%93 "CA确保公钥正确传输")

1. CA 是权威的证书签发机构，全球就那么几个公司比较权威，该机构用RSA生成一对公私钥。
2. 服务器公钥内容 + 签发者ID + 证书签发给谁Subject + 有效期 + 其他信息 =  明文内容P
3. 明文内容 P 经过Hash算法生成 H1，用 CA 的私钥对 H1 进行 RSA 加密获得 S。
4. P + S  = 数字证书。
5. 客户端得到数字证书后，用同样Hash算法对 P 进行 Hash计算得到 H2。
6. 用 CA 公钥解密 S 得到了一个 H3。
7. 比较 H2 跟 H3 是否一样，一样说明这个证书OK。不一样说明 P 被修改了或者证书不是CA签发的。
8. 一样就可以正确拿出服务器公钥了，搞定！
<a name="XTdoO"></a>
#### 2.4.5 SSL/TLS 建立流程
先进行 TCP 的三次握手，然后准备加密通信，开始加密通信之前，客户端和服务器首先必须建立连接和交换参数，这个过程叫做握手 HandShake，也就是前面一直说的SSL/TLS模块，那么它的主要工作流程是啥呢，可以认为是 ClientHello、ServerHello、Finish。<br />![SSL/TLS 建立流程](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922718-37fb376e-a752-4caf-b819-3546148099cd.png#clientId=u38f601e0-1422-4&from=paste&id=u7f896604&originHeight=773&originWidth=760&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ufe063086-9166-4f51-976c-a4608372404&title=SSL%2FTLS%20%E5%BB%BA%E7%AB%8B%E6%B5%81%E7%A8%8B "SSL/TLS 建立流程")

1. 客户端请求

客户端向服务器发起加密通信请求 : 客户端给出SSL/TLS协议版本号 + 一个客户端生成的随机数Random1 + 客户端支持的加密方法。

1. 服务端请求

服务器端确认SSL/TLS版本是否支持，确认使用的加密算法，生成随机数Random2 (用来生成会话秘钥)，生成服务器**数字证书**。

1. 客户端证书验证
2. 客户端通过CA公钥确认服务器数字证书真实性，取出服务器公钥。
3. 客户端生成一个随机数Random3，用服务器公钥加密生成 PreMaster Key然后发送给 服务器，再发送个约定的**加密算法**。
4. 服务器用私钥解密 PreMaster Key得到 Random3。至此服务器跟客户端都用相同的加密算法加密Random1 + Random2 + Random3 = 会话秘钥 Session Key，以后通信就用这个了加密通信。
5. 客户端将前面的握手消息生成摘要再用协商好的秘钥加密，这是客户端发出的第一条加密消息。服务端接收后会用秘钥解密，能解出来说明前面协商出来的秘钥是一致的。
6. 服务器最后回应
7. 服务端收到Random3 + 最终加密算法 最终定下  会话秘钥 Session Key。
8. 服务端向客户端告知加密算法改变，后面会用Session Key 加密信息。
9. 服务端也会将握手过程的消息生成摘要再用秘钥加密，这是服务端发出的第一条加密消息。客户端接收后会用秘钥解密，能解出来说明协商的秘钥是一致的。
10. 正常发送数据
- 至此，双方已安全地协商出了同一份秘钥， SSL/TLS 的握手阶段全部结束。所有的应用层数据都会用这个秘钥加密后再通过 TCP 进行可靠传输。
<a name="fm5kL"></a>
### 2.4 HTTP 发展史
目前 HTTP 版本分为 HTTP/1.1、HTTP/2、HTTP/3 三个版本，主流用的是前面俩。<br />![HTTP版本对比](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922728-8a757651-76f6-4a18-96d8-268c4e976481.png#clientId=u38f601e0-1422-4&from=paste&id=uccc43598&originHeight=512&originWidth=971&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0526f555-572d-4e2c-9768-af9214b73e4&title=HTTP%E7%89%88%E6%9C%AC%E5%AF%B9%E6%AF%94 "HTTP版本对比")
<a name="AM3kC"></a>
#### 2.4.1  HTTP/1.1
HTTP/1.1 相比于老版本优缺点如下：<br />**优点**：

1. TCP 开始使用长连接替代短连接来避免不必要的性能开销。
2. 比如发送ABC时B的发送没必要必须等待A发送完才开始发送B。

**缺点**：

1. 请求/响应头未经压缩就发送，只能压缩Body部分。
2. 来回发送冗余的配置信息。
3. 会引发头部阻塞。
4. FIFO模式，没有优先级概念。
5. 只能客户端请求，服务器响应。
<a name="bR5oU"></a>
#### 2.4.1  HTTP/2
HTTP/2 协议是基于 HTTPS 的，做了向下兼容同时还有如下优化。

1. **头部压缩**：引入 HPACK 算法，在客户端和服务器同时维护一张头信息表，所有字段都会存入这个表中，头部来回重复信息不再发原值直接发索引号就好。
2. **二进制传输**：新版本采用对计算机更友好的二进制模式传输，数据按帧传输。
3. **流式优先级传输**：按Stream区分不同的请求响应数据包，每个Stream都有独立编号。并且还可以指定优先级。
4. **多路复用**：一个连接里多个流可以同时收发请求-应答数据帧，每个流中数据包按序传输组装，每个流都是独立的，所以谁先处理好请求，谁就可以先将响应通过连接发送给对方。
5. **服务器推送**：服务器端会主动 推送可能用到的JS、CSS 等 static 变量。

**缺点**：

1. **阻塞问题**：HTTP/2 的分帧传输是在应用层进行的，最终数据要经过 TCP 传输，而 TCP 是可靠性连接，有丢包重传功能。如果有包丢失会导致所有的 HTTP 请求在等待被丢的包被重传回来。
<a name="jOVzB"></a>
#### 2.4.1  HTTP/3
HTTP/3 把 TCP 协议改成了UDP，因为 UDP 是不管顺序、不管丢包的， 同时 Google 在 UDP 的基础上也加了 TCP 的连接管理、拥塞窗口、流量控制等机制，这套协议称之为 **QUIC** 协议。整体来说 HTTP/3 优化点如下：

1. QUIC 独有一套机制来保证传输的可靠性的。当某个流发生丢包时，只会阻塞这个流，其他流不会受到影响。
2. TLS 算法也由1.2升级到了1.3，头部压缩算法升级为 QPack。
3. HTTP/3 之前通信要先三次 TCP 握手 + TLS 三次加密交互。QUIC 底层将6步合并成了3步。
4. QUIC 是一个在 UDP 之上的 TCP + TLS + HTTP/2 的多路复用的协议。
<a name="aDZMP"></a>
### 2.5  HTTP 特性

1. **灵活扩展**

HTTP 牛逼之处在于他只是规定了 header +  body 的基本框架，里面具体填写啥用户可自定义，同时它的底层都是插拔式的组件，比如 SSL/TLS 的添加，二进制帧传送，UDP替换TCP等等。

1. **可靠传输**

不管是 TCP 还是 QUIC 都保证了 数据传输的可靠性。

1. **请求-应答模式**

HTTP 是 基于-请求 应答模型实现数据传输的。

1. **无状态**

HTTP 的每一个请求-应答都是无状态的，所以每次收发报文都是完全独立的，如果要实现一些连锁反应需要用到 Session 跟 Cookie 机制。

1. **应用层协议**

HTTP 只是一个在应用层规定好的传输协议而已，它的底层用的是 **TCP** 协议传输数据。
<a name="I36ui"></a>
### 2.6  HTTP 常见状态码
常见的 HTTP 状态码 有五种类型。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660295922800-92ac6c65-8a68-4a32-88a8-03e8b34d159e.png#clientId=u38f601e0-1422-4&from=paste&id=u5c556702&originHeight=237&originWidth=731&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua2a18d0d-722a-4815-9370-7ccecff1c9d&title=)
<a name="vUcSy"></a>
## 3、附录
大致讲解了TCP/IP协议的应用层跟传输层，网络层下篇见，看个更详细版本的 **TCP/IP** 协议。<br />![TCP/IP协议](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660295922934-c33230c2-c975-4986-856f-7743427a306f.jpeg#clientId=u38f601e0-1422-4&from=paste&id=uf4aae33f&originHeight=1530&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u48433b84-733d-4240-b821-4d3a60c5638&title=TCP%2FIP%E5%8D%8F%E8%AE%AE "TCP/IP协议")
