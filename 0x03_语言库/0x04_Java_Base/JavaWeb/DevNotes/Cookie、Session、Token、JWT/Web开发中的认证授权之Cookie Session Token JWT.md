JavaWeb Cookie Session Token JWT
<a name="ag0Xi"></a>
## HTTP 是一个无状态的协议
什么是无状态呢？就是说这一次请求和上一次请求是没有任何关系的，互不认识的，没有关联的。<br />记忆力不好的优点就是一个字「快」；缺点也很明显，需要借靠 cookie、session 、token等机制将客户端多次请求关联起来。<br />想象一下如果没有 cookie、session、token 这样的机制，在网站上每次点击都需要重新输入密码认证，这样槽糕的体验还愿意继续用吗？<br />在讲解cookie、session 、token前先简单讲解两个概念：认证、授权。
<a name="tGSMU"></a>
## 什么是认证？
认证（Authentication）简单来讲就是验证当前者的身份，证明你是你自己。<br />最常见的认证方式是通过用户名和密码，除此之外还有：

- 通过用户手机：手机短信、手机二维码扫描、手势密码
- 用户的电子邮箱
- 身份证号码
- 基于时间序列和用户相关的一次性口令
- 用户的生物学特征：指纹、语音、眼睛虹膜等

为了确认用户的身份，防止请求伪造，在安全要求高的场合，经常会使用组合认证（也叫多因素认证），也就是同时使用多个认证方式对用户的身份进行校验。
<a name="wLXWG"></a>
## 什么是授权？
授权（Authorization）简单来讲就是授予第三方访问用户资源的权限。<br />在互联网应用开发中，主要通过下面几种方式实现授权：

- 通过 session 机制，一个访问会话保持着用户的授权信息
- 通过 cookie 机制，一个网站的 cookie 保持着用户的授权信息
- 颁发授权令牌（token），一个合法有效的令牌中保持着用户的授权信息
<a name="l2JDB"></a>
## 什么是 cookie？
<a name="rtXrg"></a>
### cookie 的特点

- cookie 最开始被设计出来是为了弥补HTTP在状态管理上的不足。
- cookie 存储在客户端：cookie 是服务器发送到用户浏览器并保存在本地的一小块数据，它会在浏览器下次向同一服务器再发起请求时被携带并发送到服务器上。
- cookie 是不可跨域的：每个 cookie 都会绑定单一的域名（包括子域），无法在别的域名下获取使用。
<a name="BAz4p"></a>
### 设置 cookie
服务器向客户端发送 Cookie 是通过 HTTP 响应报文实现的，在 Set-Cookie 中设置需要向客户端发送的cookie，cookie格式如下：
```
Set-Cookie: value[; expires=date][; domain=domain][; path=path][; secure]
```
<a name="uGQ74"></a>
### cookie 工作流程
![2021-09-27-07-40-27-606405.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632699693503-2518595a-6a2d-40c1-bff8-ec013fe23eba.png#clientId=u6702ee6e-7763-4&from=ui&id=u8feda9b3&originHeight=436&originWidth=1080&originalType=binary&ratio=1&size=73078&status=done&taskId=u25de1f68-4266-4fdc-a31b-74bf0b6cc54)<br />（1）浏览器向服务器发送请求；<br />（2）服务器响应请求，向浏览器设置 cookie；<br />（3）浏览器将 cookie 存在本地，下一次请求带上该 cookie；<br />（4）服务器响应请求。
<a name="RQVtw"></a>
### cookie 常见属性
<a name="wLS07"></a>
#### （1）name=value
键值对，设置 Cookie 的名称及相对应的值。
<a name="ldHSv"></a>
#### （2）domain
指定 cookie 所属域名，默认是当前域名。如果 cookie 的 domain 设置为 taobao.com，那么 item.taobao.com, order.taobao.com 都是可以共享 cookie 的， 但是访问 tmall.com 就不能共享 cookie 了，这就涉及跨域访问的问题，跨域问题如何解决，这里不展开，有兴趣可以自行搜索。
<a name="uPauG"></a>
#### （3）path
指定 cookie 在哪个路径（路由）下生效，默认是 '/'。如果设置为 /abc，则只有 /abc 下的路由可以访问到该 cookie，如：/abc/read。
<a name="TOUkO"></a>
#### （4）expires
指定 cookie 的过期时间（GMT时间格式），到达该时间点后该 cookie 就会自动失效。
<a name="BK7Ee"></a>
#### （5）max-age
HTTP 1.1中定义的，优先级高于 expires 字段。<br />max-age 表示 cookie 有效期，单位秒。如果为正数，则该 cookie 在 max-age 秒后失效；如果为负数，该 cookie 为临时 cookie ，关闭浏览器即失效，浏览器也不会以任何形式保存该 cookie ；如果为 0，表示删除该 cookie 。默认为 -1。
<a name="TEowY"></a>
#### （6）HttpOnly
如果给某个 cookie 设置了 httpOnly 属性，则无法通过 JS 脚本读写该 cookie 的信息。
<a name="Lwqm1"></a>
#### （7）secure
该 cookie 是否仅被使用安全协议传输，默认为false。当 secure 值为 true 时，cookie 在 HTTP 中是无效的。
<a name="lc0fM"></a>
## 什么是 session?
session 翻译过来就是『会话』。用户打开一个浏览器, 点击多个超链接, 访问服务器多个web资源, 然后关闭浏览器, 整个过程称之为一个会话。
<a name="fvEhc"></a>
### （1）session 的特点

- session 是另一种记录服务器和客户端会话状态的机制；
- session 存储在服务器端，一般是文件中，也可以存在数据库或缓存中。
- session 一般基于 cookie 实现。session 中包含敏感信息存储在服务器端，通常将 sessionId 存储在客户端的 cookie 中，客户端每次请求携带 sessionId 即可识别用户。
<a name="PtYQl"></a>
### （2）session 工作流程
![2021-09-27-07-40-27-817458.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632699693501-117278b9-2c39-4423-88ad-284f2857cc6a.png#clientId=u6702ee6e-7763-4&from=ui&id=u9Y5t&originHeight=578&originWidth=1080&originalType=binary&ratio=1&size=67407&status=done&style=none&taskId=u968dacc5-f0c7-47df-86d1-39ef09e5e31)<br />（1）用户第一次请求，提交用户名密码等信息进行登录认证，服务器根据用户提交的信息进行鉴权，鉴权成功后创建 session 对象，并将 sessionId 塞入 cookie 中，浏览器收到响应信息将 cookie 存入本地；<br />（2）用户第二次请求，以查看订单信息为例，浏览器自动将当前域名下的 cookie 信息发送给服务端，服务端解析 cookie，获取到 sessionId 后再查找对应的 session 对象，如果 session 对象存在说明用户已经登录，继续下一步操作。<br />从上面的流程可知，sessionId 是 cookie 和 session 中间的一道桥梁。<br />需要注意：如果客户端禁用了 cookie，还可以通过 url 重写等方法传递 sessionId。
<a name="ESa8G"></a>
## cookie 和 session 的区别

- 存储方式：cookie 数据存放在客户的浏览器上，session 数据放在服务器上；
- 安全性：cookie 是本地存储，不是很安全，别人可以分析存放在本地的 cookie 并进行欺骗；
- 存储大小：很多浏览器限制单个 cookie 保存的数据不能超过4K，一个站点最多保存20个cookie，session 没有类似的限制；
- 生存周期：cookie 可设置为长时间保持，Session 一般失效时间较短，一般客户端关闭 session 就会失效。
<a name="Tu0HI"></a>
## 什么是 token ？
<a name="mmOMn"></a>
### （1）token 的组成
token 是验证用户身份的凭证，通常叫它：令牌。<br />最简单的token组成: uid(用户唯一的身份标识)、time(当前时间的时间戳)、sign(签名，以哈希算法压缩成一定长的十六进制字符串）<br />![2021-09-27-07-40-27-938420.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632699693497-caa622a0-96b2-4039-ace1-79c95304f68c.png#clientId=u6702ee6e-7763-4&from=ui&id=E9yT5&originHeight=810&originWidth=716&originalType=binary&ratio=1&size=31199&status=done&style=none&taskId=ubf03307f-ce37-41b5-a205-86b5aec190c)
<a name="StQXe"></a>
### （2）token 特点

- 无状态、可扩展
- 支持移动端设备
- 支持跨程序调用
- 安全
<a name="AW9qE"></a>
### （3）token 工作流程
![2021-09-27-07-40-28-081403.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632699743257-768a1768-4ec5-4b79-9359-c395879aea36.png#clientId=u6702ee6e-7763-4&from=ui&id=ub740a69e&originHeight=523&originWidth=1080&originalType=binary&ratio=1&size=67067&status=done&style=none&taskId=u39acd85a-5c8c-4edc-a7ee-78d2a5d02c3)![2021-09-27-07-40-28-209449.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632699743313-3a370286-1170-41d7-a799-a84dd530c358.png#clientId=u6702ee6e-7763-4&from=ui&id=u8f1d737e&originHeight=644&originWidth=1080&originalType=binary&ratio=1&size=99561&status=done&style=none&taskId=uec27f17d-559c-48a2-89e4-ff5fd420da2)<br />第一步：客户端使用用户名密码或者扫码等形式请求登录；<br />第二步：服务端收到请求后进行鉴权，鉴权成功后服务端会生成一个 token 并发送给客户端，客户端收到 token 以后，会把它存储起来，比如放在 cookie 里或者 localStorage 里；<br />第三步：客户端下一次向服务端请求资源的时候需要带着存储的 token；<br />第四步：服务端收到请求，然后去验证客户端请求里面带着的 token ，如果验证成功，就向客户端返回请求的数据。<br />需要注意：

- 客户端请求时可以将 token 放到 HTTP 的 Header 里；
- 基于 token 的用户认证是一种服务端无状态的认证方式，服务端不用存放 token 数据。
- 用解析 token 的计算时间换取 session 的存储空间，从而减轻服务器的压力，减少频繁的查询数据库
<a name="JhgaF"></a>
## 什么是 JWT？
<a name="cfkmn"></a>
### （1）JWT 是标准化的 token
从本质上讲 JWT 也是一种 token，只不过 JWT 是被大家广泛接受的标准。<br />JWT 即：Json web token (JWT), 是为了在网络应用环境间传递声明而执行的一种基于 JSON 的开放标准(RFC 7519)。<br />JWT的声明一般被用来在身份提供者和服务提供者间传递被认证的用户身份信息，以便于从资源服务器获取资源，也可以增加一些额外的其它业务逻辑所必须的声明信息。
<a name="D80h7"></a>
### （2）JWT 的组成
![2021-09-27-07-40-28-131442.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632699743153-4afca885-0877-4ff4-a656-38ed338a5341.png#clientId=u6702ee6e-7763-4&from=ui&id=n3R10&originHeight=176&originWidth=708&originalType=binary&ratio=1&size=18015&status=done&style=none&taskId=u8171c2b8-789f-4888-bd22-d88ec012e09)<br />JWT 共有三部分组成：

- 第一部分称它为头部（header)
- 第二部分称其为载荷（payload, 类似于飞机上承载的物品)
- 第三部分是签证（signature)

在[https://jwt.io/](https://jwt.io/)网站上可以解析一个已知的 JWT：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632699593470-22523896-076a-4349-bbca-e8a1fb47ee91.webp#clientId=u6702ee6e-7763-4&from=paste&id=u5664c927&originHeight=644&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=uf8cce3f6-130d-4d46-89a6-e3655ec85a4)

- header

jwt的头部承载两部分信息：声明类型，这里是jwt；声明加密的算法,通常直接使用 HMAC SHA256；

- playload

载荷就是存放有效信息的地方。主要包含三个部分：标准中注册的声明；公共的声明；私有的声明。

- signature

jwt 的第三部分是一个签名信息，这个签证信息由三部分组成：header (base64后的)；payload (base64后的)；secret。
<a name="oFpSp"></a>
### （3）JWT 的特点

- 不在 jwt 的 payload 部分存放敏感信息，因为该部分是客户端可解密的部分。
- 保护好 secret 私钥，该私钥非常重要。
- 如果可以，请使用 https 协议。
<a name="wMd19"></a>
## 总结
在分布式微服务技术日趋流行的今天，大型网站的设计都尽量避免使用 session 实现 HTTP 状态化。<br />session简单粗暴，在服务端维护会话信息，在客户端保存session id，服务端能够轻易地把会话控制在自己的手中，但服务集群化产生了session共享的负担；<br />jwt（token）只在客户端保存会话信息，服务端通过密钥校验会话，（相比session）拿时间换空间，卸下了服务端集群共享会话信息的负担，同时也加大了服务端控制会话的难度。
