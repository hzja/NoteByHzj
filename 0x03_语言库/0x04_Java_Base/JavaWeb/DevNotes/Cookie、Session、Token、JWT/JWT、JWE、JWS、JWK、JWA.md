[**JOSE**](https://datatracker.ietf.org/wg/jose/documents/)全称**Javascript Object Signing and Encryption**，它和**JWT**有莫大的关系。
<a name="HbSGK"></a>
## JOSE简介
**JOSE**是一个**Javascript**对象签名和加密协议，目的是提供一种在各个通讯方之间安全传输声明（ **claims**，例如授权信息 ）的方法，它特意构建在**JSON**和**BASE64**之上，以便在 Web应用程序中轻松使用。目前该规范还在不断地发展，常用的包含以下几个由**RFC**文档定义的概念：<br />![JOSE规范集](https://cdn.nlark.com/yuque/0/2022/png/396745/1646180725563-edef76e8-da1a-4d54-90a5-1febdb23a6d3.png#clientId=u3946031e-0a8f-4&from=paste&id=ue1905bbe&originHeight=381&originWidth=839&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uf5d33334-a59e-4c79-ac35-dbb7cfa037c&title=JOSE%E8%A7%84%E8%8C%83%E9%9B%86 "JOSE规范集")<br />JWT就可以用JWS或JWE表示。
<a name="IMAAC"></a>
### JWS
**JSON Web签名**，基于**JSON**数据结构、使用数字签名技术或者消息认证码技术保护的内容（**MAC**）都可以称为**JWS**。该规范使用的密码算法和标识符在另一个规范**JWA**中定义。规则是比较多的参见[**RFC7515**](https://datatracker.ietf.org/doc/rfc7515/)，这里通过序列化来感受一下即可。
<a name="p6dMo"></a>
#### JWS 序列化
**JWS**的序列化分为**JWS Compact Serialization**和**JWS JSON Serialization**两种。
<a name="ViTeD"></a>
##### JWS Compact Serialization
该序列化表示为一种URL安全的、紧凑的字符串。格式为：
```
BASE64URL(UTF8(JWS Protected Header)) || '.' ||
BASE64URL(JWS Payload) || '.' ||
      BASE64URL(JWS Signature)
```
例如：
```
eyJhbGciOiJFUzI1NiJ9.eyJpc3MiOiJqb2UiLA0KICJleHAiOjEzMDA4MTkzODAsDQogImh0dHA6Ly9leGFtcGxlLmNvbS9pc19yb290Ijp0cnVlfQ.DtEhU3ljbEg8L38VWAfUAqOyKAM6-Xx-F4GawxaepmXFCgfTjDxw5djxLa8ISlSApmWQxfKTUJqPP3-Kg6NU1Q
```
**JWT**通常就是这种格式。
<a name="t3M73"></a>
##### JWS JSON Serialization
该序列化表示为一个**JSON**对象，有两种格式。一般格式为：
```json
{
  "payload":"<payload contents>",
  "signatures":[
   {"protected":"<integrity-protected header 1 contents>",
    "header":"<non-integrity-protected header 1 contents>",
    "signature":"<signature 1 contents>"},
   {"protected":"<integrity-protected header N contents>",
    "header":"<non-integrity-protected header N contents>",
    "signature":"<signature N contents>"}]
 }
```
平铺格式为：
```json
{
  "payload":"<payload contents>",
  "protected":"<integrity-protected header contents>",
  "header":"<non-integrity-protected header contents>",
  "signature":"<signature contents>"
}
```
举个一般格式的例子：
```json
{
  "payload":"eyJpc3MiOiJqb2UiLA0KICJleHAiOjEzMDA4MTkzODAsDQogImh0dHA6Ly9leGFtcGxlLmNvbS9pc19yb290Ijp0cnVlfQ",
  "signatures":[
    {
      "protected":"eyJhbGciOiJSUzI1NiJ9",
      "header":{
        "kid":"2010-12-29"
      },
    "signature":"cC4hiUPoj9Eetdgtv3hF80EGrhuB__dzERat0XF9g2VtQgr9PJbu3XOiZj5RZmh7AAuHIm4Bh-0Qc_lF5YKt_O8W2Fp5jujGbds9uJdbF9CUAr7t1dnZcAcQjbKBYNX4BAynRFdiuB--f_nZLgrnbyTyWzO75vRK5h6xBArLIARNPvkSjtQBMHlb1L07Qe7K0GarZRmB_eSN9383LcOLn6_dO--xi12jzDwusC-eOkHWEsqtFZESc6BfI7noOPqvhJ1phCnvWh6IeYI2w9QOYEUipUTI8np6LbgGY9Fs98rqVt5AXLIhWkWywlVmtVrBp0igcN_IoypGlUPQGe77Rw"},
    {
      "protected":"eyJhbGciOiJFUzI1NiJ9",
      "header":{
        "kid":"e9bc097a-ce51-4036-9562-d2ade882db0d"
      },
    "signature":"DtEhU3ljbEg8L38VWAfUAqOyKAM6-Xx-F4GawxaepmXFCgfTjDxw5djxLa8ISlSApmWQxfKTUJqPP3-Kg6NU1Q"}]
}
```
<a name="tyV4a"></a>
### JWE
**JWS** 仅仅是对声明(claims)作了签名，保证了其不被篡改，但是其 **payload(中段负载)** 信息是暴露的。也就是 **JWS** 仅仅能保证数据的完整性而不能保证数据不被泄露。它不适合传递敏感数据。**JWE** 的出现就是为了解决这个问题的。具体的可以看下图：<br />![JWE示意图](https://cdn.nlark.com/yuque/0/2022/png/396745/1646180725565-da07e847-7a77-423b-a5df-72ee43a8a9eb.png#clientId=u3946031e-0a8f-4&from=paste&id=u92107f77&originHeight=641&originWidth=899&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u5153c68b-97ee-40d9-9a70-9919604184e&title=JWE%E7%A4%BA%E6%84%8F%E5%9B%BE "JWE示意图")<br />从上面可以看出 **JWE** 的生成非常繁琐，作为 Token 可能比较消耗资源和耗时。用作安全的数据传输途径应该不错。举个例子：
```
eyJhbGciOiJSU0EtT0FFUCIsImVuYyI6IkEyNTZHQ00ifQ.
OKOawDo13gRp2ojaHV7LFpZcgV7T6DVZKTyKOMTYUmKoTCVJRgckCL9kiMT03JGe
ipsEdY3mx_etLbbWSrFr05kLzcSr4qKAq7YN7e9jwQRb23nfa6c9d-StnImGyFDb
Sv04uVuxIp5Zms1gNxKKK2Da14B8S4rzVRltdYwam_lDp5XnZAYpQdb76FdIKLaV
mqgfwX7XWRxv2322i-vDxRfqNzo_tETKzpVLzfiwQyeyPGLBIO56YJ7eObdv0je8
1860ppamavo35UgoRdbYaBcoh9QcfylQr66oc6vFWXRcZ_ZT2LawVCWTIy3brGPi
6UklfCpIMfIjf7iGdXKHzg.
48V1_ALb6US04U3b.
5eym8TW_c8SuK0ltJ3rpYIzOeDQz7TALvtu6UG9oMo4vpzs9tX_EFShS8iB7j6ji
SdiwkIr3ajwQzaBtQD_A.
XFBoMYUZodetZdvTiFvSkQ
```
一共有五个部分，被四个英文句号隔开。<br />其实**JWE**也有对应的**JSON**格式，同样具有**JWS**的两种序列化方式，参见[**RFC7516**](https://datatracker.ietf.org/doc/rfc7516/)。
<a name="KBNrp"></a>
### JWT和JWS、JWE的关系
以下是[**RFC7519**](https://datatracker.ietf.org/doc/rfc7519/)对**JWT**的说明：<br />![JWT的定义](https://cdn.nlark.com/yuque/0/2022/png/396745/1646180725606-2a1a15a8-e403-4293-969d-24e4e1e26335.png#clientId=u3946031e-0a8f-4&from=paste&id=ud40b5faa&originHeight=413&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u070b40f5-fdad-4bed-b8c0-458bfcef23d&title=JWT%E7%9A%84%E5%AE%9A%E4%B9%89 "JWT的定义")<br />从上面可以得出一些结论：

- **JWT**有特定的 claims，这些claims以JSON的形式组成Payload。
- **JWT**的结构可以是**JWS**或者**JWE**。
- **JWT**的序列化方式只能使用**Compact Serialization**，不能是**JSON Serialization**。

简而言之，**JWT**是包含了特定claims的**JWS**或者**JWE**字符串。常见的大部分都属于**JWS**。<br />另外，通常读作J、W、T，实际建议读作jot（角特），关于**JWT**的定义和规范请参阅[**RFC7519**](https://datatracker.ietf.org/doc/rfc7519/)。
<a name="Jvkop"></a>
### JWK
**JWK**是重点，这对后面学习资源服务器（**Resource Server**）非常重要。
<a name="HyF1F"></a>
#### 场景描述
**签名公私钥**这个大家都不陌生。**JWT**本身也要做使用私钥进行签名防止信息被篡改，公钥用来发给下游消费方来验证**JWT**的可靠性。通常情况下，公钥的配置方式为静态文件集成，这有一个弊端，当上游公私钥进行了改动，下游就无法动态进行公钥适配。这就是**JWK**要解决的问题，它对密码算法和标识符进行了规范设计，它紧凑的**JSON**数据结构非常方便在上下游之间传输。
<a name="U5F8f"></a>
#### JWK 格式
**JWK**是表示加密密钥的**JSON**对象。该对象包含的key名称必须是唯一的，在此基础上**JWK**可以包含一些自定义字段。下面是一个**P-256 EC（椭圆曲线离散密码）**密钥的**JWK**表示：
```json
{
  "kty":"EC",
  "crv":"P-256",
  "x":"f83OJ3D2xF1Bg8vub9tLe1gHMzV76e8Tus9uPHvRVEU",
  "y":"x_FEzRu9m36HLN_tue659LNpXW6pCyStikYjKIWI5a0",
  "kid":"Public key used in JWS spec Appendix A.3 example"
}
```
按照[**RFC7517**](https://datatracker.ietf.org/doc/rfc7517/)的定义，**JWK JSON**对象可能包含以下属性：<br />![JWK的定义属性](https://cdn.nlark.com/yuque/0/2022/png/396745/1646180725573-6e2ae7a5-74c0-45e7-926d-f5b4ecea86a5.png#clientId=u3946031e-0a8f-4&from=paste&id=ud1b61b87&originHeight=484&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ufc812b2c-6791-41ec-bc41-2f558056be6&title=JWK%E7%9A%84%E5%AE%9A%E4%B9%89%E5%B1%9E%E6%80%A7 "JWK的定义属性")<br />根据不同的算法**JWK**还可能包含其它的属性。
<a name="b9g3P"></a>
#### JWK Set
**JWK Set** 表示一组具有不同kid的**JWK**，这非常容易理解。它也是一个**JSON**对象，唯一的key就是keys。举个例子：
```json
{"keys":
 [
	 {
		 "kty":"EC",
		 "crv":"P-256",
		 "x":"MKBCTNIcKUSDii11ySs3526iDZ8AiTo7Tu6KPAqv7D4",
		 "y":"4Etl6SRW2YiLUrN5vfvVHuhp7x8PxltmWWlbbM4IFyM",
		 "use":"enc",
		 "kid":"1"},
	 
	 {
		 "kty":"RSA",
		 "n": "0vx7agoebGcQSuuPiLJXZptN9nndrQmbXEps2aiAFbWhM78LhWx4cbbfAAtVT86zwu1RK7aPFFxuhDR1L6tSoc_BJECPebWKRXjBZCiFV4n3oknjhMstn64tZ_2W-5JsGY4Hc5n9yBXArwl93lqt7_RN5w6Cf0h4QyQ5v-65YGjQR0_FDW2QvzqY368QQMicAtaSqzs8KJZgnYb9c7d0zgdAZHzu6qMQvRL5hajrn1n91CbOpbISD08qNLyrdkt-bFTWhAI4vMQFh6WeZu0fM4lFd2NcRwr3XPksINHaQ-G_xBniIqbw0Ls1jF44-csFCur-kEgU8awapJzKnqDKgw",
		 "e":"AQAB",
		 "alg":"RS256",
		 "kid":"2011-04-29"
	 }
 ]
}
```
**OAuth2**配置中的**JWK Set URL**就是输出**JWK Set**的端点。
<a name="R0P1n"></a>
### JWA
JWA规范规定了哪些算法可以作为JWS和JWE的密码算法。还规定了这些算法对应的**JWK**中的alg属性，以及特定算法在JWK包含的属性例如前面**EC**算法中的crv、x、y，这些属性并不是一成不变的，它们会根据算法的迭代进行调整。如果对**JWA**的细节感兴趣，请参阅[**RFC7518**](https://datatracker.ietf.org/doc/rfc7518/)。<br />可以通过[**JWK生成器**](https://mkjwk.org/)自行使用一些算法生成**JWK**观察不同算法之间的区别。
<a name="SwbYd"></a>
## 小结
对**JOSE**规范进行简单的介绍了解，对学习**OAuth2**和**OIDC**相关的知识非常有帮助。不要求深入但是一定要了解相关的知识。
<a name="PjF0e"></a>
## 参考资料
JOSE: [https://datatracker.ietf.org/wg/jose/documents/](https://datatracker.ietf.org/wg/jose/documents/)<br />RFC7515: [https://datatracker.ietf.org/doc/rfc7515/](https://datatracker.ietf.org/doc/rfc7515/)<br />RFC7516: [https://datatracker.ietf.org/doc/rfc7516/](https://datatracker.ietf.org/doc/rfc7516/)<br />RFC7519: [https://datatracker.ietf.org/doc/rfc7519/](https://datatracker.ietf.org/doc/rfc7519/)<br />RFC7517: [https://datatracker.ietf.org/doc/rfc7517/](https://datatracker.ietf.org/doc/rfc7517/)<br />RFC7518: [https://datatracker.ietf.org/doc/rfc7518/](https://datatracker.ietf.org/doc/rfc7518/)<br />JWK生成器: [https://mkjwk.org/](https://mkjwk.org/)
