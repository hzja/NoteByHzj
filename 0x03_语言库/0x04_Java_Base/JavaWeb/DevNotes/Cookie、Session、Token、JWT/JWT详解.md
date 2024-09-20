JavaWeb JWT<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905252003-47dafcd7-c089-4ed3-ba8c-978daaafe6af.webp#averageHue=%23f4fbfc&clientId=ub262e4e2-553c-4&from=paste&id=uc3ed5fe7&originHeight=864&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u02578928-8869-4704-8ddb-647441359a0&title=)
<a name="gNhBk"></a>
## 前言
最近在开发一个统一认证服务，涉及到 OIDC 协议，其中授权码模式所颁发的 id_token 使用的是 JWT ( JSON Web Token ) ，因为这次使用的库的默认签名算法和以往不同，所以特地去翻阅了 JWT 的 RFC 文档（ [RFC 7519](https://datatracker.ietf.org/doc/rfc7519/) ），一番阅读后发现原来对 JWT 的认知只停留在表面，还有更多深层的内容是不知道的。
<a name="gDh10"></a>
## 1）最常使用的 JWT
每次提到无状态的 JWT 时相信都会看到另一种基于 Session 的用户认证方案介绍，这里也不例外，Session 的认证流程通常会像这样：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905251994-cc3f7add-91bf-41cc-873e-6177a6090083.webp#averageHue=%23cad0d0&clientId=ub262e4e2-553c-4&from=paste&id=u2211ea3a&originHeight=732&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u12821b9d-59fe-44e6-8883-1808020dfed&title=)<br />这种方案有一些缺点：

- 需要从内存或数据库里存取 session 数据
- 扩展性差，对于分布式应用，需要实现 session 数据共享

JWT 正好可以解决这些问题：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905251954-c17d9698-5f7c-41bb-a481-42575b86b1ac.webp#averageHue=%23eef2f2&clientId=ub262e4e2-553c-4&from=paste&id=u483dc214&originHeight=716&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1cecb5bd-7d4f-4c63-a303-b904137fa1f&title=)<br />JWT 的魔法很简单，将需要使用到的用户数据等信息放入 JWT 里面，每次请求都会携带上，只要保证密钥不泄露，JWT 就无法伪造。<br />一个简单的 JWT 示例如下：<br />eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOiIyMDIxLTEwLTI0IDAwOjAwOjAwIiwibmFtZSI6InRvZ2V0dG95b3UifQ.XdF46NflSUjnt-adAc6rNZEXI1OD6nxtwGuhz9qkxUA<br />[jwt.io](https://jwt.io/) 这个网站相信没有人不知道了，把上面的 JWT 复制粘贴到网站中<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905252032-13e084dd-91da-4a34-8377-ab52426757ed.webp#averageHue=%237cd6f0&clientId=ub262e4e2-553c-4&from=paste&id=u26d316f1&originHeight=688&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u622753c2-6c24-4cba-9e74-879d60d3269&title=)<br />可以看出 JWT 以不同颜色区分，两个小数点隔开，分为了三部分：<br />① Header（头部）：JSON 对象，描述 JWT 的元数据。其中 alg 属性表示签名的算法（algorithm），默认是 HMAC SHA256（写成 HS256）；typ 属性表示这个令牌（token）的类型（type），统一写为 JWT<br />② Payload（载荷）：JSON 对象，存放实际需要传递的数据，支持自定义字段<br />③ Signature（签名）：这部分就是 JWT 防篡改的精髓，其值是对前两部分 base64UrlEncode 后使用指定算法签名生成，以默认 HS256 为例，指定一个密钥（secret），就会按照如下公式生成：
```
HMACSHA256(
 base64UrlEncode(header) + "." + base64UrlEncode(payload),
 secret,
)
```
到这里，大多数人对 JWT 的认知应该是停留在此了，日常使用也已经足够，想更深入了解 JWT 的话，那就得知道 JOSE 。
<a name="wiTrC"></a>
## 2）JOSE 规范
什么是 JOSE ，它和 JWT 之间又有什么关系呢。<br />JOSE 全称 JSON Object Signing and Encryption ( [RFC 7165](https://www.rfc-editor.org/rfc/rfc7165.html) , [RFC 7520](https://www.rfc-editor.org/rfc/rfc7520.html) )，它定义了一系列的标准，用来规范网络传输过程中使用 JSON 的方式，上面一直说的 JWT 其实是 JOSE 体系之一。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905251942-c38df198-2771-4dff-adb8-1daaacc26bec.webp#averageHue=%23ebefc7&clientId=ub262e4e2-553c-4&from=paste&id=ucbba7a5d&originHeight=714&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0e8796b0-5b65-4f39-9afa-62ed382e232&title=)<br />其中 JWT 又可分为 JWS 和 JWE 两种不同的实现，大部分日常所使用的，所说的 JWT 其实应该属于 JWS 。 为什么这么说，请看下文。
<a name="TNXug"></a>
## 3）JWA 和 JWS 以及 JWK
JWA 的全称是 JSON Web Algorithms ( [RFC 7518](https://www.rfc-editor.org/rfc/rfc7518.html) ) ，字如其名， JOSE 体系中涉及到的所有算法就是它来定义的，比如通用算法有 Base64-URL 和 SHA，签名算法有 HMAC，RSA 和 Elliptic Curve（EC 椭圆曲线）。上面的 JWT 例子中第一部分 Header 有个 alg 属性，其值是 HS256 ，也就是 HMAC + SHA256 算法。<br />说了那么多，好像都没有正式介绍过 JWS 。JWS 的全称是 JSON Web Signature ( [RFC 7515](https://www.rfc-editor.org/rfc/rfc7515.html) ) ，它的核心就是签名，保证数据未被篡改，而检查签名的过程就叫做验证。更通俗的理解，就是对应前面提到的 JWT 的第三部分 Signature ，所以才会说日常所使用的 JWT 都是 JWS 。<br />通常在客户端-服务端模式中，JWS 使用 JWA 提供的 HS256 算法加上一个密钥即可，这种方式严格依赖密钥，但在分布式场景，可能多个服务都需要验证 JWT ，若要在每个服务里面都保存密钥，那么安全性将会大打折扣，要知道，密钥一旦泄露，任何人都可以随意伪造 JWT 。<br />解决办法就是使用非对称加密算法 RSA ，RSA 有两把钥匙，一把公钥，一把私钥，可以使用私钥签发（签名分发） JWT ，使用公钥验证 JWT ，公钥是所有人都可以获取到的。这样一来，就只有认证服务保存着私钥，进行签发，其他服务只能验证。<br />如下是一个使用 RS256 ( RSA + SHA256 ) 算法生成的 JWT ：
```
eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6IjF6aXlIVG15M184MDRDOU1jUENHVERmYWJCNThBNENlZG9Wa3VweXdVeU0ifQ.eyJqdGkiOiIzWUJ5eWZ2TDB4b01QNXdwTXVsZ0wiLCJzdWIiOiI2MDE5NDI5NjgwMWRjN2JjMmExYjI3MzUiLCJpYXQiOjE2MTI0NDQ4NzEsImV4cCI6MTYxMzY1NDQ3MSwic2NvcGUiOiJvcGVuaWQgZW1haWwgbWVzc2FnZSIsImlzcyI6Imh0dHBzOi8vc3RlYW0tdGFsay5hdXRoaW5nLmNuL29pZGMiLCJhdWQiOiI2MDE5M2M2MTBmOTExN2U3Y2IwNDkxNTkifQ.cYyZ6buwAjp7DzrYQEhvz5rvUBhkv_s8xzuv2JHgzYx0jbqqsWrA_-gufLTFGmNkZkZwPnF6ktjvPHFT-1iJfWGRruOOMV9QKPhk0S5L2eedtbKJU6XIEkl3F9KbOFwYM53v3E7_VC8RBj5IKqEY0qd4mW36C9VbS695wZlvMYnmXhIopYsd5c83i39fLBF8vEBZE1Rq6tqTQTbHAasR2eUz1LnOqxNp2NNkV2dzlcNIksSDbEGjTNkWceeTWBRtFMi_o9EWaHExdm5574jQ-ei5zE4L7x-zfp9iAe8neuAgTsqXOa6RJswhyn53cW4DwWg_g26lHJZXQvv_RHZRlQ
```
把它复制到 [jwt.io](https://jwt.io/) 上面看看<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905252630-79086244-eaf9-407c-9c27-a5e3ce38c9cc.webp#averageHue=%23cee3d4&clientId=ub262e4e2-553c-4&from=paste&id=u6d4b3536&originHeight=946&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u52981c3a-a397-4a1f-a9e8-0c20a0931cd&title=)<br />注意绿色框选中的地方，里面是一段 JSON ，把它删掉，看看输入框的提示信息<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905252852-d046abd5-dae5-4ac7-b889-f598bc4a999f.webp#averageHue=%23edf6f5&clientId=ub262e4e2-553c-4&from=paste&id=u5c2fad15&originHeight=1131&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0aab5de7-791a-4787-b296-633cc8400d6&title=)<br />这里提示了，里面是填写公钥格式（通常为 PEM）或者 JWK （说过 RSA 算法是使用私钥签发 JWT，公钥进行验证），刚刚删掉的是一段 JSON，所以必然不是公钥格式，那是 JWK 吗？<br />当然是，JWK 的全称是 JSON Web Key ( [RFC 7517](https://www.rfc-editor.org/rfc/rfc7517.html) ) ，它就是一个 JSON ，JWK 就是用 JSON 来表示密钥（JSON 字段因密钥类型而异）。例如刚才删除的 JWK ：
```json
{
  "e": "AQAB",
  "kty": "RSA",
  "n": "wVKQLBUqOBiay2dkn9TlbfuaF40_edIKUmdLq6OlvzEMrP4IDzdOk50TMO0nfjJ6v5830_5x0vRg5bzZQeKpHniR0sw7qyoSI6n2eSkSnFt7P-N8gv2KWnwzVs_h9FDdeLOeVOU8k_qzkph3_tmBV7ZZG-4_DEvgvat6ifEC-WzzYqofsIrTiTT7ZFxTqid1q6zrrsmyU2DQH3WdgFiOJVVlN2D0BuZu5X7pGZup_RcWzt_9T6tQsGeU1juSuuUk_9_FVDXNNCTObfKCTKXqjW95ZgAI_xVrMeQC5nXlMh6VEaXfO83oy1j36wUoVUrUnkANhp-dnjTdvJgwN82dGQ"
}
```
其中 kty 字段是必须的，代表密钥类型，支持 EC 椭圆曲线密钥，RSA 密钥和 oct 对称密钥。<br />JWK 和 公钥格式 Pem 是可以互相转换的：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905252778-911ef48f-57a5-4ea3-88cb-7bf79dea63ef.webp#averageHue=%23d6d9d6&clientId=ub262e4e2-553c-4&from=paste&id=u2d1e86db&originHeight=797&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8666c9fb-952c-400d-8ceb-2a244f9b4ad&title=)<br />现在已经知道，验证这个 JWT 是需要公钥或 JWK 的，会不会好奇 jwt.io 这个网站是怎么知道 JWK 的呢，为什么一粘贴，就自动将 JWK 填充进去了。<br />原理其实很简单，而且已经是一种大家都遵循的规范了，就是将 JWK 放在 iss/.well-known/jwks.json 下，其中 iss 就是 Payload 里面的 iss 。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905252780-0aeb4bc3-425e-4b48-b60f-425b56050f95.webp#averageHue=%23e5e7f4&clientId=ub262e4e2-553c-4&from=paste&id=u6727986c&originHeight=531&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u73e88daa-108d-4fb5-8969-eddfb8086c4&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905252739-58b9a359-8c8f-420e-9651-777368b22b77.webp#averageHue=%2366666d&clientId=ub262e4e2-553c-4&from=paste&id=u38222c82&originHeight=361&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufbe6a434-fe35-48b1-aeac-8a0d3943ec0&title=)<br />当在 jwt.io 粘贴下 JWT 的瞬间，jwt.io 会先解析 Header ，判断出 JWT 使用的算法（JWA），接着解析出 Payload 的信息，由于这里是 RS256 算法， 所以还会去请求 Payload 里的 iss 下的 .well-known/jwks.json得到 JWK ，从而完成 JWS 的验证。
<a name="WSUz3"></a>
## 4）另一种 JWT 的实现 ：JWE
经过 Signature 签名后的 JWT 就是指的 JWS ，而 JWS 仅仅是对前两部分签名，保证无法篡改，但是其 Payload（载荷） 信息是暴露的（只是作了 base64UrlEncode 处理）。因此，使用 JWS 方式的 Payload 是不适合传递敏感数据的，JWT 的另一种实现 JWE 就是来解决这个问题的。<br />JWE 全称是 JSON Web Encryption ( [RFC 7516](https://www.rfc-editor.org/rfc/rfc7516.html) ) ，JWS 的 Payload 是 Base64Url 的明文，而 JWE 的数据则是经过加密的。它可以使 JWT 更加安全。<br />JWE 提供了两种方案：共享密钥方案和公钥/私钥方案。共享密钥方案的工作原理是让各方都知道一个密钥，大家都可以签名验证，这和 JWS 是一致的。而公钥/私钥方案的工作方式就不同了，在 JWS 中私钥对令牌进行签名，持有公钥的各方只能验证这些令牌；但在 JWE 中，持有私钥的一方是唯一可以解密令牌的一方，公钥持有者可以引入或交换新数据然后重新加密，因此，当使用公钥/私钥方案时，JWS 和 JWE 是互补的。<br />想要理解这一点的更简单的方法是从生产者和消费者的角度进行思考。生产者对数据进行签名或加密，消费者可以对其进行验证或解密。对于 JWS ，私钥对 JWT 进行签名，公钥用于验证，也就是生产者持有私钥，消费者持有公钥，数据流动只能从私钥持有者到公钥持有者。相比之下，对于 JWE ，公钥是用于加密数据，而私钥用来解密，在这种情况下，数据流动只能从公钥持有者到私钥持有者。如下图所示（来源 [JWT Handbook](https://auth0.com/resources/ebooks/jwt-handbook) ）：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905253006-9dd66a52-7a4b-4d65-8b04-2c0812e0bb53.webp#averageHue=%23f9f9f9&clientId=ub262e4e2-553c-4&from=paste&id=u73a39d2c&originHeight=991&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufaccaf19-b08d-4012-8b9b-7be3c5303e3&title=)<br />相比于 JWS 的三个部分，JWE 有五个部分组成（四个小数点隔开）。一个 JWE 示例如下：
```
eyJhbGciOiJSU0ExXzUiLCJlbmMiOiJBMTI4Q0JDLUhTMjU2In0.
UGhIOguC7IuEvf_NPVaXsGMoLOmwvc1GyqlIKOK1nN94nHPoltGRhWhw7Zx0-kFm1NJn8LE9XShH59_
i8J0PH5ZZyNfGy2xGdULU7sHNF6Gp2vPLgNZ__deLKxGHZ7PcHALUzoOegEI-8E66jX2E4zyJKxYxzZIItRzC5hlRirb6Y5Cl_p-ko3YvkkysZIFNPccxRU7qve1WYPxqbb2Yw8kZqa2rMWI5ng8Otv
zlV7elprCbuPhcCdZ6XDP0_F8rkXds2vE4X-ncOIM8hAYHHi29NX0mcKiRaD0-D-ljQTPcFPgwCp6X-nZZd9OHBv-B3oWh2TbqmScqXMR4gp_A.
AxY8DCtDaGlsbGljb3RoZQ.
KDlTtXchhZTGufMYmOYGS4HffxPSUrfmqCHXaI9wOGY.
9hH0vgRfYgPnAHOd8stkvw
```

- Protected Header (受保护的头部) ：类似于 JWS 的 Header ，标识加密算法和类型。
- Encrypted Key (加密密钥) ：用于加密密文和其他加密数据的密钥。
- Initialization Vector (初始化向量) ：一些加密算法需要额外的（通常是随机的）数据。
- Encrypted Data (Ciphertext) (加密的数据) ：被加密的数据。
- Authentication Tag (认证标签) ：算法产生的附加数据，可用于验证密文内容不被篡改。

这五个部分的生成，也就是 JWE 的加密过程可以分为 7 个步骤：

1. 根据 Header alg 的声明，生成一定大小的随机数
2. 根据密钥管理方式确定 Content Encryption Key ( CEK )
3. 根据密钥管理方式确定 JWE Encrypted Key
4. 计算所选算法所需大小的 Initialization Vector (IV)。如果不需要，可以跳过
5. 如果 Header 声明了 zip ，则压缩明文
6. 使用 CEK、IV 和 Additional Authenticated Data ( AAD，额外认证数据 ) ，通过 Header enc 声明的算法来加密内容，结果为 Ciphertext 和 Authentication Tag
7. 最后按照以下算法构造出 Token：
```java
base64(header) + '.' +
base64(encryptedKey) + '.' + // Steps 2 and 3
base64(initializationVector) + '.' + // Step 4
base64(ciphertext) + '.' + // Step 6
base64(authenticationTag) // Step 6
```
JWE 相比 JWS 更加安全可靠，但是不够轻量，有点复杂。
<a name="pqebM"></a>
## 5）安全性考虑
不管怎样，JWT 多多少少还是存在一些安全性隐患的，下面是平时开发过程的一些建议：

1. 始终执行算法验证签名算法的验证固定在后端，不以 JWT 里的算法为标准。假设每次验证 JWT ，验证算法都靠读取 Header 里面的 alg 属性来判断的话，攻击者只要签发一个 "alg: none" 的 JWT ，就可以绕过验证了。
2. 选择合适的算法具体场景选择合适的算法，例如分布式场景下，建议选择 RS256 。
3. HMAC 算法的密钥安全除了需要保证密钥不被泄露之外，密钥的强度也应该重视，防止遭到字典攻击。
4. 避免敏感信息保存在 JWT 中JWS 方式下的 JWT 的 Payload 信息是公开的，不能将敏感信息保存在这里，如有需要，请使用 JWE 。
5. JWT 的有效时间尽量足够短JWT 过期时间建议设置足够短，过期后重新使用 refresh_token 刷新获取新的 token 。
<a name="OG6D6"></a>
## 6）总结
今天为大家讲了一些 JWT 不为人知的秘密，总结一下涉及到的知识点：

- JOSE：规范网络传输过程中使用 JSON 的一系列标准
- JWT：以 JSON 编码并由 JWS 或 JWE 安全传递的表示形式
- JWS：签名和验证 Token
- JWE：加密和解密 Token
- JWA：定义 JOSE 体系中涉及到的所有算法
- JWK：用 JSON 来表示密钥

最后，再次附上 JOSE 的体系图，相关的 RFC 均备注在图上了：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635905253059-e823471c-f481-469a-97bd-ff18b86619d1.webp#averageHue=%23ebefc7&clientId=ub262e4e2-553c-4&from=paste&id=ua5da1f18&originHeight=714&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uee9ff16e-fc8c-44ef-b4aa-1fcfedd44b1&title=)
<a name="hILfF"></a>
### 参考资料
RFC 7519：[https://datatracker.ietf.org/doc/rfc7519/](https://datatracker.ietf.org/doc/rfc7519/)<br />jwt.io：[https://jwt.io/](https://jwt.io/)<br />RFC 7165：[https://www.rfc-editor.org/rfc/rfc7165.html](https://www.rfc-editor.org/rfc/rfc7165.html)<br />RFC 7520：[https://www.rfc-editor.org/rfc/rfc7520.html](https://www.rfc-editor.org/rfc/rfc7520.html)<br />RFC 7518：[https://www.rfc-editor.org/rfc/rfc7518.html](https://www.rfc-editor.org/rfc/rfc7518.html)<br />RFC 7515：[https://www.rfc-editor.org/rfc/rfc7515.html](https://www.rfc-editor.org/rfc/rfc7515.html)<br />RFC 7517：[https://www.rfc-editor.org/rfc/rfc7517.html](https://www.rfc-editor.org/rfc/rfc7517.html)<br />RFC 7516：[https://www.rfc-editor.org/rfc/rfc7516.html](https://www.rfc-editor.org/rfc/rfc7516.html)<br />JWT Handbook：[https://auth0.com/resources/ebooks/jwt-handbook](https://auth0.com/resources/ebooks/jwt-handbook)
