HTTPS
<a name="SQmNf"></a>
## 1、 HTTP 协议
在谈论 HTTPS 协议之前，先来回顾一下 HTTP 协议的概念。
<a name="QxU8o"></a>
### 1.1 HTTP 协议介绍
HTTP 协议是一种基于文本的传输协议，它位于 OSI 网络模型中的应用层。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901895889-d98772b8-a6dc-4253-8c76-5aec3448ea23.png#averageHue=%239dca69&clientId=u3624ef54-2859-4&from=paste&id=u53423397&originHeight=525&originWidth=732&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u75915ea3-77b2-4623-a05f-43e0c03d9bc&title=)<br />HTTP 协议是通过客户端和服务器的请求应答来进行通讯，目前协议由之前的 RFC 2616 拆分成立六个单独的协议说明（RFC 7230、RFC 7231、RFC 7232、RFC 7233、RFC 7234、RFC 7235），通讯报文如下：

- 请求
```
POST http://www.baidu.com HTTP/1.1
Host: www.baidu.com
Connection: keep-alive
Content-Length: 7
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36

wd=HTTP
```

- 响应
```
HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Encoding: gzip
Content-Type: text/html;charset=utf-8
Date: Thu, 14 Feb 2019 07:23:49 GMT
Transfer-Encoding: chunked

<html>...</html>
```
<a name="Tv4ca"></a>
### 1.2 HTTP 中间人攻击
HTTP 协议使用起来确实非常的方便，但是它存在一个致命的缺点：不安全。<br />HTTP 协议中的报文都是以明文的方式进行传输，不做任何加密，这样会导致什么问题呢？下面来举个例子：

1. 小明在 JAVA 贴吧发帖，内容为我爱JAVA：

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901895830-34c840ef-4ba7-43c4-924e-0ecfe868d58c.png#averageHue=%23f6f6f6&clientId=u3624ef54-2859-4&from=paste&id=uaaeddfe3&originHeight=294&originWidth=500&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8a0d0e2e-d94b-48e7-a5d8-b642884f4eb&title=)

1. 被中间人进行攻击，内容修改为我爱PHP

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901895858-29e81d18-c13b-4011-a3c1-a024a3be9c2f.png#averageHue=%23f6f6f6&clientId=u3624ef54-2859-4&from=paste&id=u63377495&originHeight=296&originWidth=732&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u92992802-b461-4f8b-b260-1abf4d218ec&title=)

1. 小明被群嘲(手动狗头)

可以看到在 HTTP 传输过程中，中间人能看到并且修改 HTTP 通讯中所有的请求和响应内容，所以使用 HTTP 是非常的不安全的。
<a name="HNQqM"></a>
### 1.3 防止中间人攻击
这个时候可能就有人想到了，既然内容是明文那使用对称加密的方式将报文加密这样中间人不就看不到明文了吗，于是如下改造：

1. 双方约定加密方式

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901895863-7f0a4dec-a500-4802-bbfd-39f316ab9b0e.png#averageHue=%23f6f6f6&clientId=u3624ef54-2859-4&from=paste&id=ue423786f&originHeight=212&originWidth=553&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec436dd5-6848-4830-8b84-44c80dbc433&title=)

1. 使用 AES 加密报文

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901895995-7c3fb312-3fa7-49be-98a8-35a3666d393d.png#averageHue=%23f7f7f7&clientId=u3624ef54-2859-4&from=paste&id=uabb4b3cc&originHeight=216&originWidth=538&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2d987424-b687-449f-ab42-b0f1ee1701b&title=)<br />这样看似中间人获取不到明文信息了，但其实在通讯过程中还是会以明文的方式暴露加密方式和秘钥，如果第一次通信被拦截到了，那么秘钥就会泄露给中间人，中间人仍然可以解密后续的通信：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901896209-88d30bed-8b48-461b-9e71-16eeff5ba09a.png#averageHue=%23f5f5f5&clientId=u3624ef54-2859-4&from=paste&id=u8b792134&originHeight=193&originWidth=732&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u886febda-f84f-4426-804b-6bc395704cc&title=)<br />那么对于这种情况，肯定就会考虑能不能将秘钥进行加密不让中间人看到呢？答案是有的，采用非对称加密，可以通过 RSA 算法来实现。<br />在约定加密方式的时候由服务器生成一对公私钥，服务器将公钥返回给客户端，客户端本地生成一串秘钥(AES_KEY)用于对称加密，并通过服务器发送的公钥进行加密得到(AES_KEY_SECRET)，之后返回给服务端，服务端通过私钥将客户端发送的AES_KEY_SECRET进行解密得到AEK_KEY，最后客户端和服务器通过AEK_KEY进行报文的加密通讯，改造如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901896234-7a5115de-26f9-4457-b5b6-beda628f1289.png#averageHue=%23f7f7f7&clientId=u3624ef54-2859-4&from=paste&id=u2e2f95c6&originHeight=628&originWidth=732&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubec27f16-5549-4d02-bb61-b385b682972&title=)<br />可以看到这种情况下中间人是窃取不到用于AES加密的秘钥，所以对于后续的通讯是肯定无法进行解密了，那么这样做就是绝对安全了吗？<br />所谓道高一尺魔高一丈，中间人为了对应这种加密方法又想出了一个新的破解方案，既然拿不到AES_KEY，那就把自己模拟成一个客户端和服务器端的结合体，在用户->中间人的过程中中间人模拟服务器的行为，这样可以拿到用户请求的明文，在中间人->服务器的过程中中间人模拟客户端行为，这样可以拿到服务器响应的明文，以此来进行中间人攻击：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901896244-41e65c01-9469-4ec8-80cb-f57889df3c60.png#averageHue=%23f6f6f6&clientId=u3624ef54-2859-4&from=paste&id=u489f4336&originHeight=535&originWidth=732&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uefc679f9-0721-40ab-9913-58e71394958&title=)<br />这一次通信再次被中间人截获，中间人自己也伪造了一对公私钥，并将公钥发送给用户以此来窃取客户端生成的AES_KEY，在拿到AES_KEY之后就能轻松的进行解密了。<br />中间人这样为所欲为，就没有办法制裁下吗，当然有啊，接下来看看 HTTPS 是怎么解决通讯安全问题的。
<a name="Basm6"></a>
## 2、HTTPS 协议
<a name="sOsK4"></a>
### 2.1 HTTPS 简介
HTTPS 其实是SSL+HTTP的简称，当然现在SSL基本已经被TLS取代了，不过接下来还是统一以SSL作为简称，SSL协议其实不止是应用在HTTP协议上，还在应用在各种应用层协议上，例如：FTP、WebSocket。<br />其实SSL协议大致就和上一节非对称加密的性质一样，握手的过程中主要也是为了交换秘钥，然后再通讯过程中使用对称加密进行通讯，大概流程如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901896448-48193125-bc44-4a98-a6f5-3b1c438e412a.png#averageHue=%23f6f6f6&clientId=u3624ef54-2859-4&from=paste&id=ud35e5973&originHeight=444&originWidth=732&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u753ecd84-f856-40ad-ac17-bbb6bc9628d&title=)<br />这里只是画了个示意图，其实真正的 SSL 握手会比这个复杂的多，但是性质还是差不多，而且这里需要关注的重点在于 HTTPS 是如何防止中间人攻击的。<br />通过上图可以观察到，服务器是通过 SSL 证书来传递公钥，客户端会对 SSL 证书进行验证，其中证书认证体系就是确保SSL安全的关键，接下来就来讲解下CA 认证体系，看看它是如何防止中间人攻击的。
<a name="SdwRk"></a>
### 2.2 CA 认证体系
上一节看到客户端需要对服务器返回的 SSL 证书进行校验，那么客户端是如何校验服务器 SSL 证书的安全性呢。

- 权威认证机构

在 CA 认证体系中，所有的证书都是由权威机构来颁发，而权威机构的 CA 证书都是已经在操作系统中内置的，把这些证书称之为CA根证书：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901896386-7bf66520-c366-4230-a624-1a5735cae3e9.png#averageHue=%23f8f6f4&clientId=u3624ef54-2859-4&from=paste&id=u8c3e6c98&originHeight=464&originWidth=732&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2de1e6d0-2baa-4658-b5d9-7d49752a506&title=)

- 签发证书

应用服务器如果想要使用 SSL 的话，需要通过权威认证机构来签发CA证书，将服务器生成的公钥和站点相关信息发送给CA签发机构，再由CA签发机构通过服务器发送的相关信息用CA签发机构进行加签，由此得到应用服务器的证书，证书会对应的生成证书内容的签名，并将该签名使用CA签发机构的私钥进行加密得到证书指纹，并且与上级证书生成关系链。这里把百度的证书下载下来看看：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901896631-2e1f062f-376d-4dba-a8fb-103f36ef0bcd.png#averageHue=%23fbfafa&clientId=u3624ef54-2859-4&from=paste&id=u35016293&originHeight=572&originWidth=454&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue77c8246-61c3-45bd-9b6f-b2667b4860d&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901896711-89e407c1-9ba8-46ad-9189-dd6019ed3391.png#averageHue=%23f6f5f4&clientId=u3624ef54-2859-4&from=paste&id=u17de5b1d&originHeight=658&originWidth=475&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf6629414-a609-4638-b220-84f69edf489&title=)<br />可以看到百度是受信于GlobalSign G2，同样的GlobalSign G2是受信于GlobalSign R1，当客户端(浏览器)做证书校验时，会一级一级的向上做检查，直到最后的根证书，如果没有问题说明服务器证书是可以被信任的。

- 如何验证服务器证书

那么客户端(浏览器)又是如何对服务器证书做校验的呢，首先会通过层级关系找到上级证书，通过上级证书里的公钥来对服务器的证书指纹进行解密得到签名(sign1)，再通过签名算法算出服务器证书的签名(sign2)，通过对比sign1和sign2，如果相等就说明证书是没有被篡改也不是伪造的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676901896730-807df647-a554-4b9a-b346-d885be0682e0.png#averageHue=%23ececec&clientId=u3624ef54-2859-4&from=paste&id=ued52f40d&originHeight=404&originWidth=688&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u73d560d0-a554-4cad-b8d3-eab596a819b&title=)<br />这里有趣的是，证书校验用的 RSA 是通过私钥加密证书签名，公钥解密来巧妙的验证证书有效性。<br />这样通过证书的认证体系，就可以避免了中间人窃取AES_KEY从而发起拦截和修改 HTTP 通讯的报文。
<a name="BNPeY"></a>
## 总结
首先先通过对 HTTP 中间人攻击的来了解到 HTTP 为什么是不安全的，然后再从安全攻防的技术演变一直到 HTTPS 的原理概括，希望能让大家对 HTTPS 有个更深刻的了解。
