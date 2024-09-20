JavaWeb Cookie Session Token<br />![2021-09-21-22-39-11-035470.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235475896-167d2975-a258-4cda-8c5c-ab9cee75287f.png#clientId=u40b0da33-5f0c-4&from=ui&id=kKvLD&originHeight=428&originWidth=729&originalType=binary&ratio=1&size=32666&status=done&style=shadow&taskId=u1750acff-9290-4457-91ee-9a35b67e40a)
<a name="UW8fW"></a>
## 1、网站交互体验升级
每天都会使用浏览器来逛各种网站，来满足日常的工作生活需求。<br />![2021-09-21-22-39-11-381790.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235475915-1d84cc35-4344-470a-9a08-45af34113899.png#clientId=u40b0da33-5f0c-4&from=ui&id=uc760e3c0&originHeight=486&originWidth=532&originalType=binary&ratio=1&size=38211&status=done&style=shadow&taskId=ua56a8b87-010a-469c-bf69-06addd207f9)<br />现在的交互体验还是很丝滑的，但早期并非如此，而是一锤子买卖。
<a name="PJEua"></a>
### 1.1 无状态的http协议
无状态的http协议是什么鬼？<br />HTTP无状态协议，是指协议对于业务处理没有记忆能力，之前做了啥完全记不住，每次请求都是完全独立互不影响的，没有任何上下文信息。<br />缺少状态意味着如果后续处理需要前面的信息，则它必须重传关键信息，这样可能导致每次连接传送的数据量增大。<br />假如一直用这种原生无状态的http协议，每换一个页面可能就得重新登录一次，那还玩个球。<br />所以必须要解决http协议的无状态，提升网站的交互体验，否则星辰大海是去不了的。
<a name="cUI8d"></a>
### 1.2 解决之道
整个事情交互的双方只有客户端和服务端，所以必然要在这两个当事者身上下手。

- 客户端来买单<br />客户端每次请求时把自己必要的信息封装发送给服务端，服务端查收处理一下就行。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632235115363-0056d5bf-d501-4159-93ef-5a742e5c1249.webp#clientId=u40b0da33-5f0c-4&from=paste&id=uaebc1083&originHeight=326&originWidth=730&originalType=url&ratio=1&status=done&style=shadow&taskId=ue3cfaafd-3a67-455b-a6e4-c483fee3cc9)

- 服务端来买单<br />客户端第一次请求之后，服务端就开始做记录，然后客户端在后续请求中只需要将最基本最少的信息发过来就行，不需要太多信息了。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632235115788-4d8d9f0c-2460-48d7-b91f-08b3a9511756.webp#clientId=u40b0da33-5f0c-4&from=paste&id=u59342937&originHeight=433&originWidth=897&originalType=url&ratio=1&status=done&style=shadow&taskId=u2e3cbdb6-0f8a-4159-9f40-c7f27a88b2f)
<a name="DEMd0"></a>
## 2、Cookie方案
Cookie总是保存在客户端中，按在客户端中的存储位置，可分为内存Cookie和硬盘Cookie，内存Cookie由浏览器维护，保存在内存中，浏览器关闭后就消失了，其存在时间是短暂的，硬盘Cookie保存在硬盘里，有一个过期时间，除非用户手工清理或到了过期时间，硬盘Cookie不会被删除，其存在时间是长期的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632235115752-80a798d1-7a21-46a5-bc75-18ccf49c0586.webp#clientId=u40b0da33-5f0c-4&from=paste&id=u44f63dbb&originHeight=345&originWidth=1015&originalType=url&ratio=1&status=done&style=shadow&taskId=u4fb0561c-f5bd-4f17-9cce-692b318fb62)
<a name="f7OfZ"></a>
### 2.1 Cookie定义和作用
HTTP Cookie（也叫 Web Cookie 或浏览器 Cookie）是服务器发送到用户浏览器并保存在本地的一小块数据，它会在浏览器下次向同一服务器再发起请求时被携带并发送到服务器上。<br />通常Cookie用于告知服务端两个请求是否来自同一浏览器，如保持用户的登录状态。Cookie 使基于无状态的HTTP协议记录稳定的状态信息成为了可能。<br />Cookie 主要用于以下三个方面：

- 会话状态管理（如用户登录状态、购物车等其它需要记录的信息）
- 个性化设置（如用户自定义设置、主题等）
- 浏览器行为跟踪（如跟踪分析用户行为等）
<a name="VRtZn"></a>
### 2.2 服务端创建Cookie
当服务器收到 HTTP 请求时，服务器可以在响应头里面添加一个 Set-Cookie 选项。<br />浏览器收到响应后通常会保存下 Cookie，之后对该服务器每一次请求中都通过  Cookie 请求头部将 Cookie 信息发送给服务器。另外，Cookie 的过期时间、域、路径、有效期、适用站点都可以根据需要来指定。<br />![2021-09-21-22-39-15-613889.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235636708-9fe979a9-6221-463a-a8d7-0e6251c4636c.png#clientId=u40b0da33-5f0c-4&from=ui&id=u3024936a&originHeight=501&originWidth=580&originalType=binary&ratio=1&size=26874&status=done&style=shadow&taskId=u51a59fd1-b415-44cf-bce0-6533209a72e)
<a name="DWTLn"></a>
### 2.3 B/S的Cookie交互
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632235115986-663dbab8-57ad-4ef1-b2e6-b54af258627a.webp#clientId=u40b0da33-5f0c-4&from=paste&id=u8c42a896&originHeight=402&originWidth=603&originalType=url&ratio=1&status=done&style=none&taskId=u0d51bbcc-2ff0-45bc-a7ac-23be1f40025)<br />服务器使用 Set-Cookie 响应头部向用户浏览器发送 Cookie信息。<br />一个简单的 Cookie 可能像这样：<br />Set-Cookie: <cookie名>=<cookie值>

HTTP/1.0 200 OK<br />Content-type: text/html<br />Set-Cookie: yummy_cookie=choco<br />Set-Cookie: tasty_cookie=strawberry

客户端对该服务器发起的每一次新请求，浏览器都会将之前保存的Cookie信息通过 Cookie 请求头部再发送给服务器。<br />GET /sample_page.html HTTP/1.1<br />Host: www.example.org<br />Cookie: yummy_cookie=choco; tasty_cookie=strawberry<br />来访问下淘宝网，抓个包看看这个真实的过程：<br />![2021-09-21-22-39-16-550280.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235655488-3490ca68-ff20-483f-921e-5591aadc0b20.png#clientId=u40b0da33-5f0c-4&from=ui&id=u81a1b94a&originHeight=567&originWidth=1080&originalType=binary&ratio=1&size=244832&status=done&style=shadow&taskId=u5d375e50-15be-449e-8ce2-a4816aa4ee0)
<a name="hQOLC"></a>
### 2.4 存在的问题
Cookie 常用来标记用户或授权会话，被浏览器发出之后可能被劫持，被用于非法行为，可能导致授权用户的会话受到攻击，因此存在安全问题。<br />还有一种情况就是跨站请求伪造CSRF，简单来说 比如在登录银行网站的同时，登录了一个钓鱼网站，在钓鱼网站进行某些操作时可能会获取银行网站相关的Cookie信息，向银行网站发起转账等非法行为。<br />跨站请求伪造（英语：Cross-site request forgery），也被称为 one-click attack 或者 session riding，通常缩写为 CSRF 或者 XSRF， 是一种挟制用户在当前已登录的Web应用程序上执行非本意的操作的攻击方法。跟跨网站脚本（XSS）相比，XSS 利用的是用户对指定网站的信任，CSRF 利用的是网站对用户网页浏览器的信任。<br />跨站请求攻击，简单地说，是攻击者通过一些技术手段欺骗用户的浏览器去访问一个自己曾经认证过的网站并运行一些操作（如发邮件，发消息，甚至财产操作如转账和购买商品）。<br />由于浏览器曾经认证过，所以被访问的网站会认为是真正的用户操作而去运行。这利用了web中用户身份验证的一个漏洞：简单的身份验证只能保证请求发自某个用户的浏览器，却不能保证请求本身是用户自愿发出的。<br />不过这种情况有很多解决方法，特别对于银行这类金融性质的站点，用户的任何敏感操作都需要确认，并且敏感信息的 Cookie 只能拥有较短的生命周期。<br />同时Cookie有容量和数量的限制，每次都要发送很多信息带来额外的流量消耗、复杂的行为Cookie无法满足要求。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632235116262-847bbbeb-a2df-4287-897e-733983f6496a.webp#clientId=u40b0da33-5f0c-4&from=paste&id=ub3cb7d7b&originHeight=354&originWidth=492&originalType=url&ratio=1&status=done&style=shadow&taskId=ud1dfc801-7cc2-403a-8388-99c0a309d16)<br />特别注意：以上存在的问题只是Cookie被用于实现交互状态时存在的问题，但并不是说Cookie本身的问题。<br />试想一下：菜刀可以用来做菜，也可以被用来从事某些暴力行为，能说菜刀应该被废除吗？
<a name="gmHfh"></a>
## 3、Session方案
<a name="jZxEr"></a>
### 3.1 Session机制的概念
如果说Cookie是客户端行为，那么Session就是服务端行为。<br />![2021-09-21-22-39-17-366854.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235675467-005fe56b-13e6-4e24-960d-927faf464fac.png#clientId=u40b0da33-5f0c-4&from=ui&id=u35a8f9f5&originHeight=471&originWidth=690&originalType=binary&ratio=1&size=42852&status=done&style=shadow&taskId=u5c361ddd-abc6-4c98-b20e-48635589725)<br />Cookie机制在最初和服务端完成交互后，保持状态所需的信息都将存储在客户端，后续直接读取发送给服务端进行交互。<br />Session代表服务器与浏览器的一次会话过程，并且完全由服务端掌控，实现分配ID、会话信息存储、会话检索等功能。<br />Session机制将用户的所有活动信息、上下文信息、登录信息等都存储在服务端，只是生成一个唯一标识ID发送给客户端，后续的交互将没有重复的用户信息传输，取而代之的是唯一标识ID，暂且称之为Session-ID吧。
<a name="ZbKyZ"></a>
### 3.2 简单的交互流程

- 当客户端第一次请求session对象时候，服务器会为客户端创建一个session，并将通过特殊算法算出一个session的ID，用来标识该session对象。
- 当浏览器下次请求别的资源的时候，浏览器会将sessionID放置到请求头中，服务器接收到请求后解析得到sessionID，服务器找到该id的session来确定请求方的身份和一些上下文信息。
<a name="ECb4X"></a>
### 3.3 Session的实现方式
首先明确一点，Session和Cookie没有直接的关系，可以认为Cookie只是实现Session机制的一种方法途径而已，没有Cookie还可以用别的方法。<br />Session和Cookie的关系就像加班和加班费的关系，看似关系很密切，实际上没啥关系。<br />session的实现主要两种方式：cookie与url重写，而cookie是首选方式，因为各种现代浏览器都默认开通cookie功能，但是每种浏览器也都有允许cookie失效的设置，因此对于Session机制来说还需要一个备胎。<br />![2021-09-21-22-39-17-710348.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235675429-4d8a1b64-45e0-4ae0-aac5-b5b520a35476.png#clientId=u40b0da33-5f0c-4&from=ui&id=vzSzs&originHeight=257&originWidth=726&originalType=binary&ratio=1&size=27742&status=done&style=shadow&taskId=u689a546e-0022-4c5e-b788-386a1bf11fa)<br />将会话标识号以参数形式附加在超链接的URL地址后面的技术称为URL重写。<br />原始的URL：<br />http://taobao.com/getitem?name=baymax&action=buy<br />重写后的URL:<br />http://taobao.com/getitem?sessionid=1wui87htentg&?name=baymax&action=buy
<a name="t9EFF"></a>
### 3.4 存在的问题
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632235116463-39b6bf32-3f5b-4e60-8788-8e4c268b0431.webp#clientId=u40b0da33-5f0c-4&from=paste&id=u1eb4453b&originHeight=299&originWidth=575&originalType=url&ratio=1&status=done&style=shadow&taskId=ud25bddb4-20ce-4598-a07b-f9c8859bce2)<br />由于Session信息是存储在服务端的，因此如果用户量很大的场景，Session信息占用的空间就不容忽视。<br />对于大型网站必然是集群化&分布式的服务器配置，如果Session信息是存储在本地的，那么由于负载均衡的作用，原来请求机器A并且存储了Session信息，下一次请求可能到了机器B，此时机器B上并没有Session信息。<br />这种情况下要么在B机器重复创建造成浪费，要么引入高可用的Session集群方案，引入Session代理实现信息共享，要么实现定制化哈希到集群A，这样做其实就有些复杂了。<br />![2021-09-21-22-39-18-505407.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235705189-b6ebca78-cd9b-4e74-90d8-f55e35b124f0.png#clientId=u40b0da33-5f0c-4&from=ui&id=u90d6b5fa&originHeight=456&originWidth=924&originalType=binary&ratio=1&size=83735&status=done&style=shadow&taskId=ud7c0190b-386d-4271-bbe6-1a07004ca11)
<a name="BK8Xr"></a>
## 4、Token方案
Token是令牌的意思，由服务端生成并发放给客户端，是一种具有时效性的验证身份的手段。<br />Token避免了Session机制带来的海量信息存储问题，也避免了Cookie机制的一些安全性问题，在现代移动互联网场景、跨域访问等场景有广泛的用途。
<a name="yUuK8"></a>
### 4.1 简单的交互流程
![2021-09-21-22-39-18-884298.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235705193-f3678cde-bd87-48e3-ba60-2dcaf6931df5.png#clientId=u40b0da33-5f0c-4&from=ui&id=CAyYJ&originHeight=548&originWidth=1019&originalType=binary&ratio=1&size=58895&status=done&style=shadow&taskId=u89322c03-1635-47a4-a455-768cafb5b4d)

- 客户端将用户的账号和密码提交给服务器
- 服务器对其进行校验，通过则生成一个token值返回给客户端，作为后续的请求交互身份令牌
- 客户端拿到服务端返回的token值后，可将其保存在本地，以后每次请求服务器时都携带该token，提交给服务器进行身份校验
- 服务器接收到请求后，解析关键信息，再根据相同的加密算法、密钥、用户参数生成sign与客户端的sign进行对比，一致则通过，否则拒绝服务
- 验证通过之后，服务端就可以根据该Token中的uid获取对应的用户信息，进行业务请求的响应
<a name="IgLUW"></a>
### 4.2 Token的设计思想
以JSON Web Token（JWT）为例，Token主要由3部分组成：

- Header头部信息<br />记录了使用的加密算法信息
- Payload 净荷信息<br />记录了用户信息和过期时间等
- Signature 签名信息<br />根据header中的加密算法和payload中的用户信息以及密钥key来生成，是服务端验证服务端的重要依据

![2021-09-21-22-39-19-213549.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632235705188-d12e03ab-51fe-48ec-a4ca-6691c16f4a9d.png#clientId=u40b0da33-5f0c-4&from=ui&id=mFhpl&originHeight=440&originWidth=1007&originalType=binary&ratio=1&size=51317&status=done&style=shadow&taskId=u120db603-8664-47be-98b0-adb86736e4b)<br />header和payload的信息不做加密，只做一般的base64编码，服务端收到token后剥离出header和payload获取算法、用户、过期时间等信息，然后根据自己的加密密钥来生成sign，并与客户端传来的sign进行一致性对比，来确定客户端的身份合法性。<br />这样就实现了用CPU加解密的时间换取存储空间，同时服务端密钥的重要性就显而易见，一旦泄露整个机制就崩塌了，这个时候就需要考虑HTTPS了。
<a name="uKoU0"></a>
### 4.3 Token方案的特点

- Token可以跨站共享，实现单点登录
- Token机制无需太多存储空间，Token包含了用户的信息，只需在客户端存储状态信息即可，对于服务端的扩展性很好
- Token机制的安全性依赖于服务端加密算法和密钥的安全性
- Token机制也不是万金油
<a name="tG7S7"></a>
## 5、总结
Cookie、Session、Token这三者是不同发展阶段的产物，并且各有优缺点，三者也没有明显的对立关系，反而常常结伴出现，这也是容易被混淆的原因。<br />Cookie侧重于信息的存储，主要是客户端行为，Session和Token侧重于身份验证，主要是服务端行为。<br />三者方案在很多场景都还有生命力，了解场景才能选择合适的方案，没有银弹。
