Java Token<br />JWT Token的 payload 部分是一个json串，是要传递数据的一组声明，这些声明被JWT标准称为claims。<br />JWT标准里面定义的标准claim包括：

- `iss(Issuser)`：JWT的签发主体；
- `sub(Subject)`：JWT的所有者；
- `aud(Audience)`：JWT的接收对象；
- `exp(Expiration time)`：JWT的过期时间；
- `nbf(Not Before)`：JWT的生效开始时间；
- `iat(Issued at)`：JWT的签发时间；
- `jti(JWT ID)`：是JWT的唯一标识。

除了以上标准声明以外，还可以自定义声明。以 com.auth0 为例，下面代码片段实现了生成一个带有过期时间的Token。
```java
String token = JWT.create()
    .withIssuer(ISSUER)
    .withIssuedAt(new Date(currentTime))// 签发时间
    .withExpiresAt(new Date(currentTime + EXPIRES_IN * 1000 * 60))// 过期时间戳
    .withClaim("username", username)//自定义参数
    .sign(Algorithm.HMAC256(user.getPassword()));
```
其中：

- `withIssuer()` 设置签发主体；
- `withIssuedAt()` 设置签发时间；
- `withExpiresAt()` 设置过期时间戳，过期的时长为 EXPIRES_IN （单位秒）；
- `withClaim()` 设置自定义参数。

JWT设置了过期时间以后，一定超过，那么接口就不能访问了，需要用户重新登录获取Token。如果经常需要用户重新登录，显然这种体验不是太好，因此很多应用会采用Token过期后自动续期的方案，只有特定条件下才会让用户重新登录。
<a name="AiAmb"></a>
## Token过期的续期方案
解决Token过期的续期问题可以有很多种不同的方案，这里举一些比较有代表性的例子。首先看一个单Token方案，这个方案除了可以实现Token续期以外，还可以实现某些条件下的强制重新登录。
<a name="uxaoi"></a>
### 单Token方案
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651903072652-3b5ab774-4b13-4681-8f8e-586cd3cc4823.png#averageHue=%23faf6f4&clientId=uc59da83c-b68d-4&from=paste&id=uf665fac3&originHeight=447&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0c85ec91-ec29-4896-b2d5-33bd4439d71&title=)

- 将 token 过期时间设置为15分钟；
- 前端发起请求，后端验证 token 是否过期；如果过期，前端发起刷新token请求，后端为前端返回一个新的token；
- 前端用新的token发起请求，请求成功；
- 如果要实现每隔72小时，必须重新登录，后端需要记录每次用户的登录时间；用户每次请求时，检查用户最后一次登录日期，如超过72小时，则拒绝刷新token的请求，请求失败，跳转到登录页面。

另外后端还可以记录刷新token的次数，比如最多刷新50次，如果达到50次，则不再允许刷新，需要用户重新授权。<br />上面介绍的单token方案原理比较简单。下面再看一个双Token方案。
<a name="hMC9M"></a>
### 双Token方案

- 登录成功以后，后端返回 access_token 和 refresh_token，客户端缓存此两种Token;
- 使用 access_token 请求接口资源，成功则调用成功；如果Token超时，客户端携带 refresh_token 调用Token刷新接口获取新的 access_token;
- 后端接受刷新Token的请求后，检查 refresh_token 是否过期。如果过期，拒绝刷新，客户端收到该状态后，跳转到登录页；如果未过期，生成新的 access_token 返回给客户端。
- 客户端携带新的 access_token 重新调用上面的资源接口。
- 客户端退出登录或修改密码后，注销旧的Token，使 access_token 和 refresh_token 失效，同时清空客户端的 access_token 和 refresh_toke。

微信网页授权是通过OAuth2.0机制实现的，也使用了双Token方案。
<a name="ztyvS"></a>
### 微信网页授权方案

- 用户在第三方应用的网页上完成微信授权以后，第三方应用可以获得 code（授权码）。code的超时时间为10分钟，一个code只能成功换取一次access_token即失效。
- 第三方应用通过code获取网页授权凭证access_token和刷新凭证 refresh_token。
- access_token是调用授权关系接口的调用凭证，由于access_token有效期（2个小时）较短，当access_token超时后，可以使用refresh_token进行刷新。
- refresh_token拥有较长的有效期（30天），当refresh_token失效的后，需要用户重新授权。

后端实现Token过期还可以利用Redis来存储Token，设置redis的键值对的过期时间。如果发现redis中不存在Token的记录，说明Token已经过期了。
