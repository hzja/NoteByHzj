Java Spring Spring Security OAuth2.0<br />Spring Security 5.5发布了，主要涉及OAuth2.0和SAML2.0两个协议。其中最大的亮点是支持了OAuth2.0的另一种授权模式jwt-bearer。这个模式可能对大家比较陌生，它并不是刚出的规范，这是2015年5月起草的RFC7523的一部分，如今正式实装到Spring Security中。
<a name="xo9Fu"></a>
## JWT Bearer  授权模式
通常出现在各大技术社区的OAuth2.0有四大授权模式：

- 授权码模式`grant_type=authorization_code`。
- 隐藏模式`response_type=token`。
- 密码模式`grant_type=password`。
- 凭据模式`grant_type=client_credentials`。

其实这几种模式中都会用到Bearer Token，甚至Token直接选用JWT技术。那么它作为一种授权模式是如何定义的呢？
<a name="DYfa6"></a>
### JWT Bearer  授权
首先jwt-bearer认证请求也要携带grant_type参数来表明使用的授权模式：
```http
grant_type=urn:ietf:params:oauth:grant-type:jwt-bearer
```
这个`grant_type`有点长！还要携带`assertion`参数，这个参数对应的值只能是一个`JWT`，另外也可以携带（可选）`scope`参数以表明请求的作用域。根据上面的描述，一个`jwt-bearer`类型的授权模式大致是这样的：
```http
POST /token/oauth2 HTTP/1.1
Host: fcant.cn
Content-Type: application/x-www-form-urlencoded

grant_type=urn%3Aietf%3Aparams%3Aoauth%3Agrant-type%3Ajwt-bearer
&assertion=eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.
eyJzdWIiOiJhbGwiLCJhdWQiOiJGZWxvcmRjbiIsInJv.
ER2U4CAt1xYxXBmnVQsrirkMwPwxwjWxjs
```
其实也就是说用户如果要请求授权要先有一个JWT，是可以被授权服务器信任的第三方JWT凭据，凭据校验通过用户就可以得到相应的授权去访问特定的资源。
<a name="eHhSC"></a>
### 客户端身份认证
RFC7523还规定JWT Bearer还可以用于客户端身份验证。客户端携带一个`client_assertion_type`参数：
```http
client_assertion_type=urn：ietf：params：oauth：client-assertion-type：jwt-bearer
```
同Beaerer JWT授权类似，还要携带一个`client_assertion`参数，这个参数同样要带一个JWT。请求实例如下：
```http
POST /token/oauth2 HTTP/1.1
Host: fcant.cn
Content-Type: application/x-www-form-urlencoded

grant_type=authorization_code&
code=n0esc3NRze7LTCu7iYzS6a5acc3f0ogp4&
client_assertion_type=urn%3Aietf%3Aparams%3Aoauth%3A
client-assertion-type%3Ajwt-bearer&
client_assertion=eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.
eyJzdWIiOiJhbGwiLCJhdWQiOiJGZWxvcmRjbiIsInJv.
ER2U4CAt1xYxXBmnVQsrirkMwPwxwjWxjs
```
<a name="Eaf0D"></a>
### 场景
那么场景是什么？根据RFC723的描述，该模式用于当客户端希望利用一个现有的、可信任的、使用JWT语义表达的关系来获取Access Token，它不需要在授权服务器上直接进行用户批准（User Approval）步骤。<br />另外jwt-bearer也被定义用于客户端身份验证机制，来判断客户端的身份是否合规。客户端使用JWT进行身份认证和客户端使用JWT进行授权是分离的行为。当然这两种行为可以组合使用，也可以分离使用。客户端使用JWT进行身份验证仅是客户端向令牌端点进行身份验证一种替代方法。
