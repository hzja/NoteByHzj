鉴权
<a name="k1e0m"></a>
## 一、什么是JWT
Json web token (JWT)，是为了在网络应用环境间传递声明而执行的一种基于JSON的开放标准（(RFC 7519).<br />该token被设计为紧凑且安全的，特别适用于分布式站点的单点登录（SSO）场景。<br />JWT的声明一般被用来在身份提供者和服务提供者间传递被认证的用户身份信息，以便于从资源服务器获取资源，也可以增加一些额外的其它业务逻辑所必须的声明信息，该token也可直接被用于认证，也可被加密。<br />![2021-09-06-21-59-02-973561.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630936755564-016edd86-be9e-4005-8dd7-bdaa7f6c6947.png#clientId=u9bc53390-7891-4&from=ui&id=ue99cc399&originHeight=239&originWidth=751&originalType=binary&ratio=1&size=66243&status=done&style=shadow&taskId=u8cf61870-9798-4796-b40a-4df14d8ecf6)
<a name="VcCH8"></a>
## 二、JWT的组成
<a name="kxhXn"></a>
### 1、JWT生成编码后的样子
```
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9.UQmqAUhUrpDVV2ST7mZKyLTomVfg7sYkEjmdDI5XF8Q
```
<a name="szf99"></a>
### 2、JWT由三部分构成
第一部分称它为头部（header)，第二部分称其为载荷（payload，类似于飞机上承载的物品)，第三部分是签证（signature).
<a name="wfDhr"></a>
#### header
jwt的头部承载两部分信息：

- 声明类型，这里是jwt
- 声明加密的算法 通常直接使用 HMAC SHA256

完整的头部就像下面这样的JSON：
```json
{
  'typ': 'JWT',
  'alg': 'HS256'
}
```
然后将头部进行base64加密（该加密是可以对称解密的)，构成了第一部分
```
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9
```
<a name="bpKaZ"></a>
#### playload
载荷就是存放有效信息的地方。这个名字像是特指飞机上承载的货品，这些有效信息包含三个部分

- 标准中注册的声明
- 公共的声明
- 私有的声明

标准中注册的声明 (建议但不强制使用) ：

- iss: jwt签发者
- sub: jwt所面向的用户
- aud: 接收jwt的一方
- exp: jwt的过期时间，这个过期时间必须要大于签发时间
- nbf: 定义在什么时间之前，该jwt都是不可用的.
- iat: jwt的签发时间
- jti: jwt的唯一身份标识，主要用来作为一次性token，从而回避重放攻击。

公共的声明 ：<br />公共的声明可以添加任何的信息，一般添加用户的相关信息或其他业务需要的必要信息.但不建议添加敏感信息，因为该部分在客户端可解密.<br />私有的声明 ：<br />私有声明是提供者和消费者所共同定义的声明，一般不建议存放敏感信息，因为base64是对称解密的，意味着该部分信息可以归类为明文信息。<br />定义一个payload:
```json
{
  "sub": "1234567890",
  "name": "John Doe",
  "admin": true
}
```
然后将其进行base64加密，得到Jwt的第二部分
```
eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9
```
<a name="nCGB6"></a>
#### signature
jwt的第三部分是一个签证信息，这个签证信息由三部分组成：

- header (base64后的)
- payload (base64后的)
- secret

这个部分需要base64加密后的header和base64加密后的payload使用.连接组成的字符串(头部在前)，然后通过header中声明的加密方式进行加盐secret组合加密，然后就构成了jwt的第三部分。
```
UQmqAUhUrpDVV2ST7mZKyLTomVfg7sYkEjmdDI5XF8Q
```
密钥secret是保存在服务端的，服务端会根据这个密钥进行生成token和验证，所以需要保护好。<br />![2021-09-06-21-59-03-074569.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630936786643-4cf20f2a-5e60-4b95-8110-85aef6ca71dc.png#clientId=u9bc53390-7891-4&from=ui&id=udbbc4862&originHeight=285&originWidth=812&originalType=binary&ratio=1&size=162984&status=done&style=shadow&taskId=u7ef528b5-e50c-4a07-9a3d-a83224a9b2b)
<a name="o079b"></a>
### 3、签名的目的
最后一步签名的过程，实际上是对头部以及载荷内容进行签名。一般而言，加密算法对于不同的输入产生的输出总是不一样的。对于两个不同的输入，产生同样的输出的概率极其地小。所以，就把“不一样的输入产生不一样的输出”当做必然事件来看待吧。<br />所以，如果有人对头部以及载荷的内容解码之后进行修改，再进行编码的话，那么新的头部和载荷的签名和之前的签名就将是不一样的。而且，如果不知道服务器加密的时候用的密钥的话，得出来的签名也一定会是不一样的。<br />![2021-09-06-21-59-03-232546.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630936786636-febc0566-7603-41c6-ae13-98a391b68b0a.png#clientId=u9bc53390-7891-4&from=ui&id=W8oPK&originHeight=265&originWidth=834&originalType=binary&ratio=1&size=133750&status=done&style=shadow&taskId=uc0c0c76c-af80-4704-a715-b1f9c2858b7)<br />服务器应用在接受到JWT后，会首先对头部和载荷的内容用同一算法再次签名。那么服务器应用是怎么知道用的是哪一种算法呢？别忘了，在JWT的头部中已经用alg字段指明了加密算法了。<br />如果服务器应用对头部和载荷再次以同样方法签名之后发现，自己计算出来的签名和接受到的签名不一样，那么就说明这个Token的内容被别人动过的，应该拒绝这个Token，返回一个HTTP 401 Unauthorized响应。<br />注意：在JWT中，不应该在载荷里面加入任何敏感的数据，比如用户的密码。
<a name="wwIbK"></a>
### 4、如何应用
一般是在请求头里加入Authorization，并加上Bearer标注：
```javascript
fetch('api/user/1', {
  headers: {
    'Authorization': 'Bearer ' + token
  }
})
```
服务端会验证token，如果验证通过就会返回相应的资源。
<a name="zP9Pc"></a>
### 5、安全相关

- 不应该在jwt的payload部分存放敏感信息，因为该部分是客户端可解密的部分。
- 保护好secret私钥，该私钥非常重要。
- 如果可以，请使用https协议
<a name="oK3Ac"></a>
### 6、对Token认证的五点认识

- 一个Token就是一些信息的集合；
- 在Token中包含足够多的信息，以便在后续请求中减少查询数据库的几率；
- 服务端需要对cookie和HTTP Authrorization Header进行Token信息的检查；
- 基于上一点，可以用一套token认证代码来面对浏览器类客户端和非浏览器类客户端；
- 因为token是被签名的，所以可以认为一个可以解码认证通过的token是由系统发放的，其中带的信息是合法有效的；
<a name="SNlAI"></a>
## 三、传统的session认证
http协议本身是一种无状态的协议，而这就意味着如果用户向应用提供了用户名和密码来进行用户认证，那么下一次请求时，用户还要再一次进行用户认证才行，因为根据http协议，并不能知道是哪个用户发出的请求，所以为了让应用能识别是哪个用户发出的请求，只能在服务器存储一份用户登录的信息，这份登录信息会在响应时传递给浏览器，告诉其保存为cookie,以便下次请求时发送给应用，这样应用就能识别请求来自哪个用户了，这就是传统的基于session认证。<br />但是这种基于session的认证使应用本身很难得到扩展，随着不同客户端用户的增加，独立的服务器已无法承载更多的用户，而这时候基于session认证应用的问题就会暴露出来。
<a name="fBGyB"></a>
### 基于session认证所显露的问题
Session: 每个用户经过应用认证之后，应用都要在服务端做一次记录，以方便用户下次请求的鉴别，通常而言session都是保存在内存中，而随着认证用户的增多，服务端的开销会明显增大。<br />扩展性: 用户认证之后，服务端做认证记录，如果认证的记录被保存在内存中的话，这意味着用户下次请求还必须要请求在这台服务器上，这样才能拿到授权的资源，这样在分布式的应用上，相应的限制了负载均衡器的能力。这也意味着限制了应用的扩展能力。<br />CSRF: 因为是基于cookie来进行用户识别的，cookie如果被截获，用户就会很容易受到跨站请求伪造的攻击。
<a name="G7R5g"></a>
### 基于token的鉴权机制
基于token的鉴权机制类似于http协议也是无状态的，它不需要在服务端去保留用户的认证信息或者会话信息。这就意味着基于token认证机制的应用不需要去考虑用户在哪一台服务器登录了，这就为应用的扩展提供了便利。<br />流程上是这样的：

- 用户使用用户名密码来请求服务器
- 服务器进行验证用户的信息
- 服务器通过验证发送给用户一个token
- 客户端存储token，并在每次请求时附送上这个token值
- 服务端验证token值，并返回数据

这个token必须要在每次请求时传递给服务端，它应该保存在请求头里， 另外，服务端要支持CORS(跨来源资源共享)策略，一般在服务端这么做就可以了 Access-Control-Allow-Origin:*。<br />四、token的优点

- 支持跨域访问: Cookie是不允许垮域访问的，这一点对Token机制是不存在的，前提是传输的用户认证信息通过HTTP头传输。
- 无状态(也称：服务端可扩展行):Token机制在服务端不需要存储session信息，因为Token 自身包含了所有登录用户的信息，只需要在客户端的cookie或本地介质存储状态信息。
- 更适用CDN: 可以通过内容分发网络请求服务端的所有资料（如：javascript，HTML，图片等），而服务端只要提供API即可。
- 去耦: 不需要绑定到一个特定的身份验证方案。Token可以在任何地方生成，只要在API被调用的时候，可以进行Token生成调用即可。
- 更适用于移动应用: 当客户端是一个原生平台（iOS，Android，Windows 8等）时，Cookie是不被支持的（需要通过Cookie容器进行处理），这时采用Token认证机制就会简单得多。
- CSRF:因为不再依赖于Cookie，所以就不需要考虑对CSRF（跨站请求伪造）的防范。
- 性能: 一次网络往返时间（通过数据库查询session信息）总比做一次HMACSHA256计算 的Token验证和解析要费时得多。
- 不需要为登录页面做特殊处理: 如果使用Protractor 做功能测试的时候，不再需要为登录页面做特殊处理。
- 基于标准化:API可以采用标准化的 JSON Web Token (JWT). 这个标准已经存在多个后端库（.NET, Ruby, Java,Python, PHP）和多家公司的支持（如：Firebase,Google, Microsoft）。
- 因为json的通用性，所以JWT是可以进行跨语言支持的，像JAVA,JavaScript,NodeJS,PHP等很多语言都可以使用。
- 因为有了payload部分，所以JWT可以在自身存储一些其他业务逻辑所必要的非敏感信息。
- 便于传输，jwt的构成非常简单，字节占用很小，所以它是非常便于传输的。
- 它不需要在服务端保存会话信息，所以它易于应用的扩展。
<a name="FMaGt"></a>
## 五、JWT的JAVA实现
Java中对JWT的支持可以考虑使用JJWT开源库；JJWT实现了JWT, JWS, JWE 和 JWA RFC规范；<br />下面将简单举例说明其使用：
<a name="Q3Ywi"></a>
### 1、生成Token码
```java
import javax.crypto.spec.SecretKeySpec;
import javax.xml.bind.DatatypeConverter;
import java.security.Key;
import io.jsonwebtoken.*;
import java.util.Date;    

//Sample method to construct a JWT

private String createJWT(String id, String issuer, String subject, long ttlMillis) {

    //The JWT signature algorithm we will be using to sign the token
    SignatureAlgorithm signatureAlgorithm = SignatureAlgorithm.HS256;

    long nowMillis = System.currentTimeMillis();
    Date now = new Date(nowMillis);

    //We will sign our JWT with our ApiKey secret
    byte[] apiKeySecretBytes = DatatypeConverter.parseBase64Binary(apiKey.getSecret());
    Key signingKey = new SecretKeySpec(apiKeySecretBytes, signatureAlgorithm.getJcaName());

    //Let's set the JWT Claims
    JwtBuilder builder = Jwts.builder().setId(id)
        .setIssuedAt(now)
        .setSubject(subject)
        .setIssuer(issuer)
        .signWith(signatureAlgorithm, signingKey);

    //if it has been specified, let's add the expiration
    if (ttlMillis >= 0) {
        long expMillis = nowMillis + ttlMillis;
        Date exp = new Date(expMillis);
        builder.setExpiration(exp);
    }

    //Builds the JWT and serializes it to a compact, URL-safe string
    return builder.compact();
}
```
<a name="p2ckX"></a>
### 2、解码和验证Token码
```java
import javax.xml.bind.DatatypeConverter;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.Claims;

//Sample method to validate and read the JWT
private void parseJWT(String jwt) {
    //This line will throw an exception if it is not a signed JWS (as expected)
    Claims claims = Jwts.parser()        
        .setSigningKey(DatatypeConverter.parseBase64Binary(apiKey.getSecret()))
        .parseClaimsJws(jwt).getBody();
    System.out.println("ID: " + claims.getId());
    System.out.println("Subject: " + claims.getSubject());
    System.out.println("Issuer: " + claims.getIssuer());
    System.out.println("Expiration: " + claims.getExpiration());
}
```
