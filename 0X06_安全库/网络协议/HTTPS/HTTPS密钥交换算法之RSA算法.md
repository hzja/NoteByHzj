网络 HTTPS RSA<br />![2021-04-27-21-20-51-955099.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529929230-2c06dbd1-a1c4-482c-91e1-ccba19ffb44d.png#clientId=ue953bfbc-3a74-4&from=ui&id=ub72554a6&originHeight=657&originWidth=1080&originalType=binary&size=100364&status=done&style=none&taskId=u898a3f25-016e-487d-87e0-52e51bd59d7)
<a name="sYZvX"></a>
### TLS 握手过程
HTTP 由于是明文传输，所谓的明文，就是说客户端与服务端通信的信息都是肉眼可见的，随意使用一个抓包工具都可以截获通信的内容。<br />所以安全上存在以下三个风险：

- _窃听风险_，比如通信链路上可以获取通信内容，用户号容易没。
- _篡改风险_，比如强制植入垃圾广告，视觉污染，用户眼容易瞎。
- _冒充风险_，比如冒充淘宝网站，用户钱容易没。

HTTPS 在 HTTP 与 TCP 层之间加入了 TLS 协议，来解决上述的风险。<br />![2021-04-27-21-20-52-236468.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619529918650-3d43db47-5143-435e-afbf-b8dee1277232.jpeg#clientId=ue953bfbc-3a74-4&from=ui&id=u7dcf9192&originHeight=275&originWidth=596&originalType=binary&size=16242&status=done&style=shadow&taskId=ud217af6e-1330-45c8-81cc-d668796629c)<br />TLS 协议是如何解决 HTTP 的风险的呢？

- _信息加密_：HTTP 交互信息是被加密的，第三方就无法被窃取；
- _校验机制_：校验信息传输过程中是否有被第三方篡改过，如果被篡改过，则会有警告提示；
- _身份证书_：证明淘宝是真的淘宝网；

可见，有了 TLS 协议，能保证 HTTP 通信是安全的了，那么在进行 HTTP 通信前，需要先进行 TLS 握手。TLS 的握手过程，如下图：<br />![2021-04-27-21-20-52-756049.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529910152-d005951d-2c4c-4b5a-b802-d65d60fa5377.png#clientId=ue953bfbc-3a74-4&from=ui&id=u89e17bed&originHeight=1259&originWidth=1080&originalType=binary&size=282983&status=done&style=shadow&taskId=u78db3046-9d00-466e-918a-e61c9ed3a39)<br />上图简要概述来 TLS 的握手过程，其中每一个「框」都是一个记录（_record_），记录是 TLS 收发数据的基本单位，类似于 TCP 里的 segment。多个记录可以组合成一个 TCP 包发送，所以**通常经过「四个消息」就可以完成 TLS 握手，也就是需要 2个 RTT 的时延**，然后就可以在安全的通信环境里发送 HTTP 报文，实现 HTTPS 协议。<br />所以可以发现，HTTPS 是应用层协议，需要先完成 TCP 连接建立，然后走 TLS 握手过程后，才能建立通信安全的连接。<br />事实上，不同的密钥交换算法，TLS 的握手过程可能会有一些区别。<br />这里先简单介绍下密钥交换算法，因为考虑到性能的问题，所以双方在加密应用信息时使用的是对称加密密钥，而对称加密密钥是不能被泄漏的，为了保证对称加密密钥的安全性，所以使用非对称加密的方式来保护对称加密密钥的协商，这个工作就是密钥交换算法负责的。<br />接下来，以最简单的 RSA 密钥交换算法，来看看它的 TLS 握手过程。

---

<a name="wo9r0"></a>
### RSA 握手过程
传统的 TLS 握手基本都是使用 RSA 算法来实现密钥交换的，在将 TLS 证书部署服务端时，证书文件中包含一对公私钥，其中公钥会在 TLS 握手阶段传递给客户端，私钥则一直留在服务端，一定要确保私钥不能被窃取。<br />在 RSA 密钥协商算法中，客户端会生成随机密钥，并使用服务端的公钥加密后再传给服务端。根据非对称加密算法，公钥加密的消息仅能通过私钥解密，这样服务端解密后，双方就得到了相同的密钥，再用它加密应用消息。<br />用 Wireshark 工具抓了用 RSA 密钥交换的 TLS 握手过程，可以从下面看到，一共经历来四次握手：<br />![2021-04-27-21-20-53-453880.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529899401-a144102e-8b5a-4d70-800a-b61129b2139d.png#clientId=ue953bfbc-3a74-4&from=ui&id=ubcb211fc&originHeight=379&originWidth=1080&originalType=binary&size=254566&status=done&style=shadow&taskId=ue53cb0cd-fca7-4168-b20c-c88a873532f)<br />对应 Wireshark 的抓包，可以从下图很清晰地看到该过程：<br />![2021-04-27-21-20-53-891815.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529886646-a0c6a4b0-18e4-42b3-b7e4-6a4d22a17cbb.png#clientId=ue953bfbc-3a74-4&from=ui&id=u08151135&originHeight=1718&originWidth=1080&originalType=binary&size=412681&status=done&style=shadow&taskId=u2a30b940-683e-4c03-8f6b-0e1dc2eecf5)<br />那么，接下来针对每一个 TLS 握手做进一步的介绍。
<a name="8XLJq"></a>
#### TLS 第一次握手
客户端首先会发一个「**Client Hello**」消息，字面意思也能理解到，这是跟服务器「打招呼」。<br />![2021-04-27-21-20-54-379173.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529871509-6bfd683a-4676-4f00-a196-1f54008db4ce.png#clientId=ue953bfbc-3a74-4&from=ui&id=ub4ccbb67&originHeight=353&originWidth=734&originalType=binary&size=106289&status=done&style=shadow&taskId=uc7ee972d-1c84-42d5-8539-a2f80bef8c2)<br />消息里面有客户端使用的 TLS 版本号、支持的密码套件列表，以及生成的**随机数（_Client Random_）**，这个随机数会被服务端保留，它是生成对称加密密钥的材料之一。
<a name="ld5jJ"></a>
#### TLS 第二次握手
当服务端收到客户端的「Client Hello」消息后，会确认 TLS 版本号是否支持，和从密码套件列表中选择一个密码套件，以及生成**随机数（_Server Random_）**。<br />接着，返回「**Server Hello**」消息，消息里面有服务器确认的 TLS 版本号，也给出了随机数（Server Random），然后从客户端的密码套件列表选择了一个合适的密码套件。<br />![2021-04-27-21-20-54-623521.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529859597-ec9363c9-7afe-456c-92a8-99763bb2ce1a.png#clientId=ue953bfbc-3a74-4&from=ui&id=uc0d85924&originHeight=310&originWidth=682&originalType=binary&size=77708&status=done&style=shadow&taskId=u7da6e953-9f30-4f14-bd56-907540e3ec4)<br />可以看到，服务端选择的密码套件是 “Cipher Suite: TLS_RSA_WITH_AES_128_GCM_SHA256”。<br />这个密码套件看起来真让人头晕，好一大串，但是其实它是有固定格式和规范的。基本的形式是「**密钥交换算法 + 签名算法 + 对称加密算法 + 摘要算法**」， 一般 WITH 单词前面有两个单词，第一个单词是约定密钥交换的算法，第二个单词是约定证书的验证算法。比如刚才的密码套件的意思就是：

- 由于 WITH 单词只有一个 RSA，则说明握手时密钥交换算法和签名算法都是使用 RSA；
- 握手后的通信使用 AES 对称算法，密钥长度 128 位，分组模式是 GCM；
- 摘要算法 SHA384 用于消息认证和产生随机数；

就前面这两个客户端和服务端相互「打招呼」的过程，客户端和服务端就已确认了 TLS 版本和使用的密码套件，而且可能发现客户端和服务端都会各自生成一个随机数，并且还会把随机数传递给对方。<br />那这个随机数有啥用呢？其实这两个随机数是后续作为生成「会话密钥」的条件，所谓的会话密钥就是数据传输时，所使用的对称加密密钥。<br />然后，服务端为了证明自己的身份，会发送「**Server Certificate**」给客户端，这个消息里含有数字证书。<br />![2021-04-27-21-20-54-900908.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529844312-2fb9c6a4-8e8d-40ce-97a0-8ad57005fa0f.png#clientId=ue953bfbc-3a74-4&from=ui&id=ue89d0fdd&originHeight=202&originWidth=751&originalType=binary&size=40012&status=done&style=shadow&taskId=ueb5f41a9-08f5-4d32-a67c-13f0437b377)<br />随后，服务端发了「**Server Hello Done**」消息，目的是告诉客户端，已经把该给你的东西都给你了，本次打招呼完毕。<br />![2021-04-27-21-20-55-103808.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529828035-d0663cbc-5d72-41d7-900c-4dc4d4420d11.png#clientId=ue953bfbc-3a74-4&from=ui&id=u3109a7e0&originHeight=98&originWidth=594&originalType=binary&size=18032&status=done&style=shadow&taskId=u13524bda-f6f1-4552-825f-d3a93a8e7df)
<a name="jrJ63"></a>
#### 客户端验证证书
客户端拿到了服务端的数字证书后，要怎么校验该数字证书是真实有效的呢？
<a name="hJipC"></a>
##### 数字证书和 CA 机构
在说校验数字证书是否可信的过程前，先来看看数字证书是什么，一个数字证书通常包含了：

- 公钥；
- 持有者信息；
- 证书认证机构（CA）的信息；
- CA 对这份文件的数字签名及使用的算法；
- 证书有效期；
- 还有一些其他额外信息；

那数字证书的作用，是用来认证公钥持有者的身份，以防止第三方进行冒充。说简单些，证书就是用来告诉客户端，该服务端是否是合法的，因为只有证书合法，才代表服务端身份是可信的。<br />用证书来认证公钥持有者的身份（服务端的身份），那证书又是怎么来的？又该怎么认证证书呢？<br />为了让服务端的公钥被大家信任，服务端的证书都是由 CA （_Certificate Authority_，证书认证机构）签名的，CA 就是网络世界里的公安局、公证中心，具有极高的可信度，所以由它来给各个公钥签名，信任的一方签发的证书，那必然证书也是被信任的。<br />之所以要签名，是因为签名的作用可以避免中间人在获取证书时对证书内容的篡改。
<a name="N54m3"></a>
##### 数字证书签发和验证流程
如下图图所示，为数字证书签发和验证流程：<br />![2021-04-27-21-20-55-945247.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529807558-3e562094-a6bd-4ed8-96a0-dbc1caaa0242.png#clientId=ue953bfbc-3a74-4&from=ui&id=u347ed6a2&originHeight=598&originWidth=1080&originalType=binary&size=151065&status=done&style=shadow&taskId=u62979fd0-d1c9-4603-b64e-2b2e7fe94d5)<br />CA 签发证书的过程，如上图左边部分：

- 首先 CA 会把持有者的公钥、用途、颁发者、有效时间等信息打成一个包，然后对这些信息进行 Hash 计算，得到一个 Hash 值；
- 然后 CA 会使用自己的私钥将该 Hash 值加密，生成 Certificate Signature，也就是 CA 对证书做了签名；
- 最后将 Certificate Signature 添加在文件证书上，形成数字证书；

客户端校验服务端的数字证书的过程，如上图右边部分：

- 首先客户端会使用同样的 Hash 算法获取该证书的 Hash 值 H1；
- 通常浏览器和操作系统中集成了 CA 的公钥信息，浏览器收到证书后可以使用 CA 的公钥解密 Certificate Signature 内容，得到一个 Hash 值 H2 ；
- 最后比较 H1 和 H2，如果值相同，则为可信赖的证书，否则则认为证书不可信。
<a name="dVoZ6"></a>
##### 证书链
但事实上，证书的验证过程中还存在一个证书信任链的问题，因为向 CA 申请的证书一般不是根证书签发的，而是由中间证书签发的，比如百度的证书，从下图可以看到，证书的层级有三级：<br />![2021-04-27-21-20-56-646947.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529793131-60c22d89-50a5-4723-86e3-7b9ed05be2f5.png#clientId=ue953bfbc-3a74-4&from=ui&id=u69c53cc7&originHeight=217&originWidth=567&originalType=binary&size=39093&status=done&style=shadow&taskId=u6e1672df-8095-41dc-9cb6-4efde07e04d)<br />对于这种三级层级关系的证书的验证过程如下：

- 客户端收到 baidu.com 的证书后，发现这个证书的签发者不是根证书，就无法根据本地已有的根证书中的公钥去验证 baidu.com 证书是否可信。于是，客户端根据 baidu.com 证书中的签发者，找到该证书的颁发机构是 “GlobalSign Organization Validation CA - SHA256 - G2”，然后向 CA 请求该中间证书。
- 请求到证书后发现 “GlobalSign Organization Validation CA - SHA256 - G2” 证书是由 “GlobalSign Root CA” 签发的，由于 “GlobalSign Root CA” 没有再上级签发机构，说明它是根证书，也就是自签证书。应用软件会检查此证书有否已预载于根证书清单上，如果有，则可以利用根证书中的公钥去验证 “GlobalSign Organization Validation CA - SHA256 - G2” 证书，如果发现验证通过，就认为该中间证书是可信的。
- “GlobalSign Organization Validation CA - SHA256 - G2” 证书被信任后，可以使用 “GlobalSign Organization Validation CA - SHA256 - G2” 证书中的公钥去验证 baidu.com 证书的可信性，如果验证通过，就可以信任 baidu.com 证书。

在这四个步骤中，最开始客户端只信任根证书 GlobalSign Root CA 证书的，然后 “GlobalSign Root CA” 证书信任 “GlobalSign Organization Validation CA - SHA256 - G2” 证书，而 “GlobalSign Organization Validation CA - SHA256 - G2” 证书又信任 baidu.com 证书，于是客户端也信任 baidu.com 证书。<br />总括来说，由于用户信任 GlobalSign，所以由 GlobalSign 所担保的 baidu.com 可以被信任，另外由于用户信任操作系统或浏览器的软件商，所以由软件商预载了根证书的 GlobalSign 都可被信任。<br />![2021-04-27-21-20-56-859397.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529769391-b2e04b09-8624-4a71-be0d-2412ea408ddd.png#clientId=ue953bfbc-3a74-4&from=ui&id=uba12c8f8&originHeight=891&originWidth=707&originalType=binary&size=59296&status=done&style=shadow&taskId=ucb37adc4-1b0b-41ac-a5fe-97f62058503)<br />操作系统里一般都会内置一些根证书，比如电脑里内置的根证书有这么多：<br />![2021-04-27-21-20-57-273282.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529748960-25235ae0-3b73-42eb-97f8-263bb0a1b260.png#clientId=ue953bfbc-3a74-4&from=ui&id=u8fe2c91b&originHeight=534&originWidth=867&originalType=binary&size=178451&status=done&style=shadow&taskId=uf462fafd-5fd2-41ca-8296-79498b0d016)<br />这样的一层层地验证就构成了一条信任链路，整个证书信任链验证流程如下图所示：<br />![2021-04-27-21-20-57-567884.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529740998-a3d442e0-c70f-4a81-aba8-ebbae2309ad8.png#clientId=ue953bfbc-3a74-4&from=ui&id=u9e3f3fd4&originHeight=330&originWidth=1080&originalType=binary&size=58636&status=done&style=shadow&taskId=uddef4247-ba5d-405c-b2f8-b556fd3fb54)<br />最后一个问题，为什么需要证书链这么麻烦的流程？Root CA 为什么不直接颁发证书，而是要搞那么多中间层级呢？<br />这是为了确保根证书的绝对安全性，将根证书隔离地越严格越好，不然根证书如果失守了，那么整个信任链都会有问题。
<a name="UicWf"></a>
#### TLS 第三次握手
客户端验证完证书后，认为可信则继续往下走。接着，客户端就会生成一个新的**随机数 (_pre-master_)**，用服务器的 RSA 公钥加密该随机数，通过「**Change Cipher Key Exchange**」消息传给服务端。<br />![2021-04-27-21-20-57-770869.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529725659-5e2707dd-4812-4e41-85e2-77991068e513.png#clientId=ue953bfbc-3a74-4&from=ui&id=ua96eeeaf&originHeight=190&originWidth=836&originalType=binary&size=49129&status=done&style=shadow&taskId=uba48e232-8a6e-47dc-b5cf-6f3bf1494c5)<br />服务端收到后，用 RSA 私钥解密，得到客户端发来的随机数 (pre-master)。<br />至此，**客户端和服务端双方都共享了三个随机数，分别是 Client Random、Server Random、pre-master**。<br />于是，双方根据已经得到的三个随机数，生成**会话密钥（Master Secret）**，它是对称密钥，用于对后续的 HTTP 请求/响应的数据加解密。<br />生成完会话密钥后，然后客户端发一个「**Change Cipher Spec**」，告诉服务端开始使用加密方式发送消息。<br />![2021-04-27-21-20-57-962357.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529705976-eb20fd17-ca7a-4766-9b18-08f3118e5e8e.png#clientId=ue953bfbc-3a74-4&from=ui&id=u26daa5bf&originHeight=95&originWidth=630&originalType=binary&size=19242&status=done&style=shadow&taskId=u39b1eaad-980d-4337-b3fc-628c4a176fd)<br />然后，客户端再发一个「**Encrypted Handshake Message（Finishd）**」消息，把之前所有发送的数据做个摘要，再用会话密钥（master secret）加密一下，让服务器做个验证，验证加密通信是否可用和之前握手信息是否有被中途篡改过。<br />![2021-04-27-21-20-58-197726.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529699309-afc4f90c-771d-4562-a0f0-3850a151fcc8.png#clientId=ue953bfbc-3a74-4&from=ui&id=u1aeb147a&originHeight=103&originWidth=635&originalType=binary&size=20368&status=done&style=shadow&taskId=ua510a178-013e-4e30-8af7-2268feb14e8)<br />可以发现，「Change Cipher Spec」之前传输的 TLS 握手数据都是明文，之后都是对称密钥加密的密文。
<a name="UBI4x"></a>
#### TLS 第四次握手
服务器也是同样的操作，发「**Change Cipher Spec**」和「**Encrypted Handshake Message**」消息，如果双方都验证加密和解密没问题，那么握手正式完成。<br />最后，就用「会话密钥」加解密 HTTP 请求和响应了。

---

<a name="bYGPx"></a>
### RSA 算法的缺陷
**使用 RSA 密钥协商算法的最大问题是不支持前向保密**。因为客户端传递随机数（用于生成对称加密密钥的条件之一）给服务端时使用的是公钥加密的，服务端收到到后，会用私钥解密得到随机数。所以一旦服务端的私钥泄漏了，过去被第三方截获的所有 TLS 通讯密文都会被破解。<br />为了解决这一问题，于是就有了 DH 密钥协商算法，这里简单介绍它的工作流程。<br />![2021-04-27-21-20-58-509028.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1619529681389-bb41025e-91e1-408d-8902-baca869ee780.png#clientId=ue953bfbc-3a74-4&from=ui&id=uc3d92c0a&originHeight=828&originWidth=1052&originalType=binary&size=111359&status=done&style=shadow&taskId=ub4ee75e6-b4cc-4e62-85d0-12579c079d9)<br />客户端和服务端各自会生成随机数，并以此作为私钥，然后根据公开的 DH 计算公示算出各自的公钥，通过 TLS 握手双方交换各自的公钥，这样双方都有自己的私钥和对方的公钥，然后双方根据各自持有的材料算出一个随机数，这个随机数的值双方都是一样的，这就可以作为后续对称加密时使用的密钥。<br />DH 密钥交换过程中，**即使第三方截获了 TLS 握手阶段传递的公钥，在不知道的私钥的情况下，也是无法计算出密钥的，而且每一次对称加密密钥都是实时生成的，实现前向保密**。<br />但因为 DH 算法的计算效率问题，后面出现了 ECDHE 密钥协商算法，现在大多数网站使用的正是 ECDHE 密钥协商算法。
