JavaWeb JWT
<a name="IBDrJ"></a>
## JWT规范
从`jwt.io`的首页展示图中，可以看到描述：
> JSON Web Tokens are an open, industry standard RFC 7519 method for representing claims securely between two parties

从这段文字中可以提取到`JWT`的规范文件`RFC 7519`，里面有详细地介绍`JWT`的基本概念，`Claims`的含义、布局和算法实现等，下面逐个展开击破。
<a name="bfNoA"></a>
### JWT基本概念
`JWT`全称是`JSON Web Token`，如果从字面上理解感觉是基于`JSON`格式用于网络传输的令牌。实际上，`JWT`是一种紧凑的`Claims`声明格式，旨在用于空间受限的环境进行传输，常见的场景如`HTTP`授权请求头参数和`URI`查询参数。`JWT`会把`Claims`转换成`JSON`格式，而这个`JSON`内容将会应用为`JWS`结构的有效载荷或者应用为`JWE`结构的（加密处理后的）原始字符串，通过消息认证码（`Message Authentication Code`或者简称`MAC`）和/或者加密操作对`Claims`进行数字签名或者完整性保护。<br />这里有三个概念在其他规范文件中，简单提一下：

- `JWE`（规范文件`RFC 7516`）：`JSON Web Encryption`，表示基于`JSON`数据结构的加密内容，加密机制对任意八位字节序列进行加密、提供完整性保护和提高破解难度，`JWE`中的紧凑序列化布局如下
```
BASE64URL(UTF8(JWE Protected Header)) || '.' ||
BASE64URL(JWE Encrypted Key) || '.' ||
BASE64URL(JWE Initialization Vector) || '.' ||
BASE64URL(JWE Ciphertext) || '.' ||
BASE64URL(JWE Authentication Tag)
```

- `JWS`（规范文件`RFC 7515`）：`JSON Web Signature`，表示使用`JSON`数据结构和`BASE64URL`编码表示经过数字签名或消息认证码（`MAC`）认证的内容，数字签名或者`MAC`能够提供完整性保护，`JWS`中的紧凑序列化布局如下：
```
ASCII(BASE64URL(UTF8(JWS Protected Header)) || '.' || 
BASE64URL(JWS Payload)) || '.' ||
BASE64URL(JWS Signature)
```

- `JWA`（规范文件`RFC 7518`）：`JSON Web Algorithm`，`JSON Web`算法，数字签名或者`MAC`算法，应用于`JWS`的可用算法列表如下：

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1613908761395-130fe863-08fe-4668-b5e5-fe3c0bd5b1f0.webp#height=800&id=Jjke7&originHeight=800&originWidth=1036&originalType=binary&size=0&status=done&style=shadow&width=1036)<br />总的来说，`JWT`其实有两种实现，基于`JWE`实现的依赖于加解密算法、`BASE64URL`编码和身份认证等手段提高传输的`Claims`的被破解难度，而基于`JWS`的实现使用了`BASE64URL`编码和数字签名的方式对传输的`Claims`提供了完整性保护，也就是仅仅保证传输的`Claims`内容不被篡改，但是会暴露明文。**「目前主流的**`**JWT**`**框架中大部分都没有实现**`**JWE**`**，所以下文主要通过**`**JWS**`**的实现方式进行深入探讨」**。
<a name="tr13t"></a>
### JWT中的Claims
`Claim`有索赔、声称、要求或者权利要求的含义，但是任一个翻译都不怎么合乎语义，这里保留`Claim`关键字直接作为命名。`JWT`的核心作用就是保护`Claims`的完整性（或者数据加密），保证`JWT`传输的过程中`Claims`不被篡改（或者不被破解）。`Claims`在`JWT`原始内容中是一个`JSON`格式的字符串，其中单个`Claim`是`K-V`结构，作为`JsonNode`中的一个`field-value`，这里列出常用的规范中预定义好的`Claim`：

| 简称 | 全称 | 含义 |
| --- | --- | --- |
| iss | Issuer | 发行方 |
| sub | Subject | 主体 |
| aud | Audience | （接收）目标方 |
| exp | Expiration Time | 过期时间 |
| nbf | Not Before | 早于该定义的时间的``JWT不能被接受处理 |
| iat | Issued At | ``JWT发行时的时间戳 |
| jti | JWT ID | ``JWT的唯一标识 |

这些预定义的`Claim`并不要求强制使用，何时选用何种`Claim`完全由使用者决定，而为了使`JWT`更加紧凑，这些`Claim`都使用了简短的命名方式去定义。在不和内建的`Claim`冲突的前提下，使用者可以自定义新的公共`Claim`，如：

| 简称 | 全称 | 含义 |
| --- | --- | --- |
| cid | Customer ID | 客户ID |
| rid | Role ID | 角色ID |

一定要注意，在`JWS`实现中，`Claims`会作为`payload`部分进行`BASE64`编码，明文会直接暴露，敏感信息一般不应该设计为一个自定义`Claim`。
<a name="EeObZ"></a>
### JWT中的Header
在`JWT`规范文件中称这些`Header`为`JOSE Header`，`JOSE`的全称为`Javascript Object Signature Encryption`，也就是`Javascript`对象签名和加密框架，`JOSE Header`其实就是`Javascript`对象签名和加密的头部参数。**「下面列举一下**`**JWS**`**中常用的**`**Header**`**」**：

| 简称 | 全称 | 含义 |
| --- | --- | --- |
| alg | Algorithm | 用于保护``JWS的加解密算法 |
| jku | JWK Set URL | 一组``JSON编码的公共密钥的``URL，其中一个是用于对``JWS进行数字签名的密钥 |
| jwk | JSON Web Key | 用于对``JWS进行数字签名的密钥相对应的公共密钥 |
| kid | Key ID | 用于保护``JWS进的密钥 |
| x5u | X.509 URL | ``X.509相关 |
| x5c | X.509 Certificate Chain | ``X.509相关 |
| x5t | X.509 Certificate SHA-1 Thumbprin | ``X.509相关 |
| x5t#S256 | X.509 Certificate SHA-256 Thumbprint | ``X.509相关 |
| typ | Type | 类型，例如``JWT、``JWS或者``JWE等等 |
| cty | Content Type | 内容类型，决定``payload部分的``MediaType |

最常见的两个`Header`就是`alg`和`typ`，例如：
```json
{
  "alg": "HS256",
  "typ": "JWT"
}
```
<a name="9vx0v"></a>
### JWT的布局
主要介绍`JWS`的布局，前面已经提到过，`JWS`的**「紧凑布局」**如下：
```
ASCII(BASE64URL(UTF8(JWS Protected Header)) || '.' || 
BASE64URL(JWS Payload)) || '.' ||
BASE64URL(JWS Signature)
```
其实还有**「非紧凑布局」**，会通过一个`JSON`结构完整地展示`Header`参数、`Claims`和分组签名：
```json
{
    "payload":"<payload contents>",
    "signatures":[
    {"protected":"<integrity-protected header 1 contents>",
    "header":<non-integrity-protected header 1 contents>,
    "signature":"<signature 1 contents>"},
    ...
    {"protected":"<integrity-protected header N contents>",
    "header":<non-integrity-protected header N contents>,
    "signature":"<signature N contents>"}]
}
```
非紧凑布局还有一个扁平化的表示形式：
```json
{
    "payload":"<payload contents>",
    "protected":"<integrity-protected header contents>",
    "header":<non-integrity-protected header contents>,
    "signature":"<signature contents>"
}
```
其中`Header`参数部分可以参看上一小节，而签名部分可以参看下一小节，剩下简单提一下`payload`部分，`payload`（有效载荷）其实就是完整的`Claims`，假设`Claims`的`JSON`形式是：
```json
{
   "iss": "throwx",
   "jid": 1
}
```
那么扁平化非紧凑格式下的`payload`节点就是：
```json
{  
   ......
   "payload": {
      "iss": "throwx",
      "jid": 1
   }
   ......
}
```
<a name="zzYaC"></a>
### JWS签名算法
`JWS`签名生成依赖于散列或者加解密算法，可以使用的算法见前面，例如`HS256`，具体是`HMAC SHA-256`，也就是通过散列算法`SHA-256`对于编码后的`Header`和`Claims`字符串进行一次散列计算，签名生成的伪代码如下：
```
## 不进行编码
HMACSHA256(
  base64UrlEncode(header) + "." +
  base64UrlEncode(payload),
  256 bit secret key
)
## 进行编码
base64UrlEncode(
    HMACSHA256(
       base64UrlEncode(header) + "." +
       base64UrlEncode(payload)
       [256 bit secret key])
)
```
其他算法的操作基本相似，生成好的签名直接加上一个前置的`.`拼接在`base64UrlEncode(header).base64UrlEncode(payload)`之后就生成完整的`JWS`。
<a name="07zNv"></a>
## JWT的生成、解析和校验
前面已经分析过`JWT`的一些基本概念、布局和签名算法，这里根据前面的理论进行`JWT`的生成、解析和校验操作。先引入`common-codec`库简化一些编码和加解密操作，引入一个主流的`JSON`框架做序列化和反序列化：
```xml
<dependency>
    <groupId>commons-codec</groupId>
    <artifactId>commons-codec</artifactId>
    <version>1.15</version>
</dependency>
<dependency>
    <groupId>com.fasterxml.jackson.core</groupId>
    <artifactId>jackson-databind</artifactId>
    <version>2.11.0</version>
</dependency>
```
为了简单起见，`Header`参数写死为：
```json
{
  "alg": "HS256",
  "typ": "JWT"
}
```
使用的签名算法是`HMAC SHA-256`，输入的加密密钥长度必须为`256 bit`（如果单纯用英文和数字组成的字符，要`32`个字符），这里为了简单起见，用`00000000111111112222222233333333`作为`KEY`。定义`Claims`部分如下：
```json
{
  "iss": "throwx",
  "jid": 10087,
  "exp": 1613227468168     # 20210213    
}
```
生成`JWT`的代码如下：
```java
@Slf4j
public class JsonWebToken {
    private static final String KEY = "00000000111111112222222233333333";
    private static final String DOT = ".";
    private static final Map<String, String> HEADERS = new HashMap<>(8);
    private static final ObjectMapper OBJECT_MAPPER = new ObjectMapper();
    static {
        HEADERS.put("alg", "HS256");
        HEADERS.put("typ", "JWT");
    }
    String generateHeaderPart() throws JsonProcessingException {
        byte[] headerBytes = OBJECT_MAPPER.writeValueAsBytes(HEADERS);
        String headerPart = new String(Base64.encodeBase64(headerBytes,false ,true), StandardCharsets.US_ASCII);
        log.info("生成的Header部分为:{}", headerPart);
        return headerPart;
    }
    String generatePayloadPart(Map<String, Object> claims) throws JsonProcessingException {
        byte[] payloadBytes = OBJECT_MAPPER.writeValueAsBytes(claims);
        String payloadPart = new String(Base64.encodeBase64(payloadBytes,false ,true), StandardCharsets.UTF_8);
        log.info("生成的Payload部分为:{}", payloadPart);
        return payloadPart;
    }
    String generateSignaturePart(String headerPart, String payloadPart) {
        String content = headerPart + DOT + payloadPart;
        Mac mac = HmacUtils.getInitializedMac(HmacAlgorithms.HMAC_SHA_256, KEY.getBytes(StandardCharsets.UTF_8));
        byte[] output = mac.doFinal(content.getBytes(StandardCharsets.UTF_8));
        String signaturePart = new String(Base64.encodeBase64(output, false ,true), StandardCharsets.UTF_8);
        log.info("生成的Signature部分为:{}", signaturePart);
        return signaturePart;
    }
    public String generate(Map<String, Object> claims) throws Exception {
        String headerPart = generateHeaderPart();
        String payloadPart = generatePayloadPart(claims);
        String signaturePart = generateSignaturePart(headerPart, payloadPart);
        String jws = headerPart + DOT + payloadPart + DOT + signaturePart;
        log.info("生成的JWT为:{}", jws);
        return jws;
    }
    public static void main(String[] args) throws Exception {
        Map<String, Object> claims = new HashMap<>(8);
        claims.put("iss", "throwx");
        claims.put("jid", 10087L);
        claims.put("exp", 1613227468168L);
        JsonWebToken jsonWebToken = new JsonWebToken();
        System.out.println("自行生成的JWT:" + jsonWebToken.generate(claims));
    }
}
```
执行输出日志如下：
```
23:37:48.743 [main] INFO club.throwable.jwt.JsonWebToken - 生成的Header部分为:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9
23:37:48.747 [main] INFO club.throwable.jwt.JsonWebToken - 生成的Payload部分为:eyJpc3MiOiJ0aHJvd3giLCJqaWQiOjEwMDg3LCJleHAiOjE2MTMyMjc0NjgxNjh9
23:37:48.748 [main] INFO club.throwable.jwt.JsonWebToken - 生成的Signature部分为:7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs
23:37:48.749 [main] INFO club.throwable.jwt.JsonWebToken - 生成的JWT为:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJ0aHJvd3giLCJqaWQiOjEwMDg3LCJleHAiOjE2MTMyMjc0NjgxNjh9.7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs
自行生成的JWT:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJ0aHJvd3giLCJqaWQiOjEwMDg3LCJleHAiOjE2MTMyMjc0NjgxNjh9.7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs
```
可以在`jwt.io`上验证一下：<br />![image.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1613908765930-fae20df5-0a9c-4df3-8283-a957021037d4.gif#height=1&id=rQ4GQ&originHeight=1&originWidth=1&originalType=binary&size=70&status=done&style=none&width=1)![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1613909311418-867c5a2f-0fe0-4f04-9972-46013bf3c0db.png#height=516&id=VgZhn&originHeight=1547&originWidth=3103&originalType=binary&size=270678&status=done&style=shadow&width=1034.3333333333333)<br />解析`JWT`的过程是构造`JWT`的逆向过程，首先基于点号`.`分三段，然后分别进行`BASE64`解码，然后得到三部分的明文，头部参数和有效载荷需要做一次`JSON`反序列化即可还原各个部分的`JSON`结构：
```java
public Map<Part, PartContent> parse(String jwt) throws Exception {
    System.out.println("当前解析的JWT:" + jwt);
    Map<Part, PartContent> result = new HashMap<>(8);
    // 这里暂且认为所有的输入JWT的格式都是合法的
    StringTokenizer tokenizer = new StringTokenizer(jwt, DOT);
    String[] jwtParts = new String[3];
    int idx = 0;
    while (tokenizer.hasMoreElements()) {
        jwtParts[idx] = tokenizer.nextToken();
        idx++;
    }
    String headerPart = jwtParts[0];
    PartContent headerContent = new PartContent();
    headerContent.setRawContent(headerPart);
    headerContent.setPart(Part.HEADER);
    headerPart = new String(Base64.decodeBase64(headerPart), StandardCharsets.UTF_8);
    headerContent.setPairs(OBJECT_MAPPER.readValue(headerPart, new TypeReference<Map<String, Object>>() {
    }));
    result.put(Part.HEADER, headerContent);
    String payloadPart = jwtParts[1];
    PartContent payloadContent = new PartContent();
    payloadContent.setRawContent(payloadPart);
    payloadContent.setPart(Part.PAYLOAD);
    payloadPart = new String(Base64.decodeBase64(payloadPart), StandardCharsets.UTF_8);
    payloadContent.setPairs(OBJECT_MAPPER.readValue(payloadPart, new TypeReference<Map<String, Object>>() {
    }));
    result.put(Part.PAYLOAD, payloadContent);
    String signaturePart = jwtParts[2];
    PartContent signatureContent = new PartContent();
    signatureContent.setRawContent(signaturePart);
    signatureContent.setPart(Part.SIGNATURE);
    result.put(Part.SIGNATURE, signatureContent);
    return result;
}
enum Part {
    HEADER,
    PAYLOAD,
    SIGNATURE
}
@Data
public static class PartContent {
    private Part part;
    private String rawContent;
    private Map<String, Object> pairs;
}
```
这里尝试用之前生产的`JWT`进行解析：
```java
public static void main(String[] args) throws Exception {
    JsonWebToken jsonWebToken = new JsonWebToken();
    String jwt = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJ0aHJvd3giLCJqaWQiOjEwMDg3LCJleHAiOjE2MTMyMjc0NjgxNjh9.7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs";
    Map<Part, PartContent> parseResult = jsonWebToken.parse(jwt);
    System.out.printf("解析结果如下:\nHEADER:%s\nPAYLOAD:%s\nSIGNATURE:%s%n",
            parseResult.get(Part.HEADER),
            parseResult.get(Part.PAYLOAD),
            parseResult.get(Part.SIGNATURE)
    );
}
```
解析结果如下：
```
当前解析的JWT:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJ0aHJvd3giLCJqaWQiOjEwMDg3LCJleHAiOjE2MTMyMjc0NjgxNjh9.7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs
解析结果如下:
HEADER:PartContent(part=HEADER, rawContent=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9, pairs={typ=JWT, alg=HS256})
PAYLOAD:PartContent(part=PAYLOAD, rawContent=eyJpc3MiOiJ0aHJvd3giLCJqaWQiOjEwMDg3LCJleHAiOjE2MTMyMjc0NjgxNjh9, pairs={iss=throwx, jid=10087, exp=1613227468168})
SIGNATURE:PartContent(part=SIGNATURE, rawContent=7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs, pairs=null)
```
验证`JWT`建立在解析`JWT`完成的基础之上，需要对解析出来的头部参数和有效载做一次`MAC`签名，与解析出来的签名做校对。另外，可以自定义校验具体的`Claim`项，如过期时间和发行者等。一般校验失败会针对不同的情况定制不同的运行时异常便于区分场景，这里为了方便统一抛出`IllegalStateException`：
```java
public void verify(String jwt) throws Exception {
    System.out.println("当前校验的JWT:" + jwt);
    Map<Part, PartContent> parseResult = parse(jwt);
    PartContent headerContent = parseResult.get(Part.HEADER);
    PartContent payloadContent = parseResult.get(Part.PAYLOAD);
    PartContent signatureContent = parseResult.get(Part.SIGNATURE);
    String signature = generateSignaturePart(headerContent.getRawContent(), payloadContent.getRawContent());
    if (!Objects.equals(signature, signatureContent.getRawContent())) {
        throw new IllegalStateException("签名校验异常");
    }
    String iss = payloadContent.getPairs().get("iss").toString();
    // iss校验
    if (!Objects.equals(iss, "throwx")) {
        throw new IllegalStateException("ISS校验异常");
    }
    long exp = Long.parseLong(payloadContent.getPairs().get("exp").toString());
    // exp校验,有效期14天
    if (System.currentTimeMillis() - exp > 24 * 3600 * 1000 * 14) {
        throw new IllegalStateException("exp校验异常,JWT已经过期");
    }
    // 省略其他校验项
    System.out.println("JWT校验通过");
}
```
类似地，用上面生成过的`JWT`进行验证，结果如下：
```
当前校验的JWT:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJ0aHJvd3giLCJqaWQiOjEwMDg3LCJleHAiOjE2MTMyMjc0NjgxNjh9.7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs
当前解析的JWT:eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJ0aHJvd3giLCJqaWQiOjEwMDg3LCJleHAiOjE2MTMyMjc0NjgxNjh9.7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs
23:33:00.174 [main] INFO club.throwable.jwt.JsonWebToken - 生成的Signature部分为:7skduDGxV-BP2p_CXyr3Na7WBvENNl--Pm4HQ8cJuEs
JWT校验通过
```
**「上面的代码存在硬编码问题，只是为了用最简单的**`**JWS**`**实现方式重新实现了**`**JWT**`**的生成、解析和校验过程」**，算法也使用了复杂程度和安全性极低的`HS256`，所以在生产中并不推荐花大量时间去实现`JWS`，可以选用现成的`JWT`类库，如`auth0`和`jjwt`。
<a name="UHhFp"></a>
## JWT的使用场景和实战
`JWT`本质是一个令牌，更多场景下是作为会话`ID`（`session_id`）使用，作用是`'维持会话的粘性'`和携带认证信息（如果用`JWT`术语，应该是安全地传递`Claims`）。很久以前使用的一种`Session ID`解决方案是由服务端生成和持久化`Session ID`，返回的`Session ID`需要写入用户的`Cookie`，然后用户每次请求必须携带`Cookie`，`Session ID`会映射用户的一些认证信息，这一切都是由服务端管理，一个很常见的例子就是`Tomcat`容器中出现的`J(ava)SESSIONID`。与之前的方案不同，`JWT`是一种无状态的令牌，它并不需要由服务端保存，携带的数据或者会话的数据都不需要持久化，使用`JWT`只需要关注`Claims`的完整性和合法性即可，生成`JWT`时候所有有效数据已经通过编码存储在`JWT`字符串中。正因`JWT`是无状态的，一旦颁发后得到`JWT`的客户端都可以通过它与服务端交互，`JWT`一旦泄露有可能造成严重安全问题，因此实践的时候一般需要做几点：

- `JWT`需要设置有效期，也就是`exp`这个`Claim`必须启用和校验
- `JWT`需要建立黑名单，一般使用`jti`这个`Claim`即可，技术上可以使用布隆过滤器加数据库的组合（数量少的情况下简单操作甚至可以用`Redis`的`SET`数据类型）
- `JWS`的签名算法尽可能使用安全性高的算法，如`RSXXX`
- `Claims`尽可能不要写入敏感信息
- 高风险场景如支付操作等不能仅仅依赖`JWT`认证，需要进行短信、指纹等二次认证
> PS：不少开发者所在的项目会把JWT持久化，其实这违背了JWT的设计理念，把JWT当成传统的会话ID使用了

`JWT`一般用于认证场景，搭配`API`网关使用效果甚佳。多数情况下，`API`网关会存在一些通用不需要认证的接口，其他则是需要认证`JWT`合法性并且提取`JWT`中的消息载荷内容进行调用，针对这个场景：

- 对于控制器入口可以提供一个自定义注解标识特定接口需要进行`JWT`认证，这个场景在`Spring Cloud Gateway`中需要自定义实现一个`JWT`认证的`WebFilter`
- 对于单纯的路由和转发可以提供一个`URI`白名单集合，命中白名单则不需要进行`JWT`认证，这个场景在`Spring Cloud Gateway`中需要自定义实现一个`JWT`认证的`GlobalFilter`

下面就`Spring Cloud Gateway`和`jjwt`，贴一些骨干代码，限于篇幅不进行细节展开。引入依赖：
```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>Hoxton.SR10</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
<dependencies>
    <dependency>
        <groupId>io.jsonwebtoken</groupId>
        <artifactId>jjwt-api</artifactId>
        <version>0.11.2</version>
    </dependency>
    <dependency>
        <groupId>io.jsonwebtoken</groupId>
        <artifactId>jjwt-impl</artifactId>
        <version>0.11.2</version>
        <scope>runtime</scope>
    </dependency>
    <dependency>
        <groupId>io.jsonwebtoken</groupId>
        <artifactId>jjwt-jackson</artifactId>
        <version>0.11.2</version>
        <scope>runtime</scope>
    </dependency>
    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <version>1.18.18</version>
        <scope>provided</scope>
    </dependency>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-gateway</artifactId>
    </dependency>
</dependencies>
```
然后编写`JwtSpi`和对应的实现`HMAC256JwtSpiImpl`：
```java
@Data
public class CreateJwtDto {
    private Long customerId;
    private String customerName;
    private String customerPhone;
}

@Data
public class JwtCacheContent {
    private Long customerId;
    private String customerName;
    private String customerPhone;
}

@Data
public class VerifyJwtResultDto {
    private Boolean valid;
    private Throwable throwable;
    private long jwtId;
    private JwtCacheContent content;
}

public interface JwtSpi {
    /**
     * 生成JWT
     *
     * @param dto dto
     * @return String
     */
    String generate(CreateJwtDto dto);
    /**
     * 校验JWT
     *
     * @param jwt jwt
     * @return VerifyJwtResultDto
     */
    VerifyJwtResultDto verify(String jwt);
    /**
     * 把JWT添加到封禁名单中
     *
     * @param jwtId jwtId
     */
    void blockJwt(long jwtId);
    /**
     * 判断JWT是否在封禁名单中
     *
     * @param jwtId jwtId
     * @return boolean
     */
    boolean isInBlockList(long jwtId);
}

@Component
public class HMAC256JwtSpiImpl implements JwtSpi, InitializingBean, EnvironmentAware {
    private SecretKey secretKey;
    private Environment environment;
    private int minSeed;
    private String issuer;
    private int seed;
    private Random random;
    @Override
    public void afterPropertiesSet() throws Exception {
        String secretKey = Objects.requireNonNull(environment.getProperty("jwt.hmac.secretKey"));
        this.minSeed = Objects.requireNonNull(environment.getProperty("jwt.exp.seed.min", Integer.class));
        int maxSeed = Objects.requireNonNull(environment.getProperty("jwt.exp.seed.max", Integer.class));
        this.issuer = Objects.requireNonNull(environment.getProperty("jwt.issuer"));
        this.random = new Random();
        this.seed = (maxSeed - minSeed);
        this.secretKey = new SecretKeySpec(secretKey.getBytes(), "HmacSHA256");
    }
    @Override
    public void setEnvironment(Environment environment) {
        this.environment = environment;
    }
    @Override
    public String generate(CreateJwtDto dto) {
        long duration = this.random.nextInt(this.seed) + minSeed;
        Map<String, Object> claims = new HashMap<>(8);
        claims.put("iss", issuer);
        // 这里的jti最好用类似雪花算法之类的序列算法生成,确保唯一性
        claims.put("jti", dto.getCustomerId());
        claims.put("uid", dto.getCustomerId());
        claims.put("exp", TimeUnit.NANOSECONDS.toMillis(System.nanoTime()) + duration);
        String jwt = Jwts.builder()
                .setHeaderParam("typ", "JWT")
                .signWith(this.secretKey, SignatureAlgorithm.HS256)
                .addClaims(claims)
                .compact();
        // 这里需要缓存uid->JwtCacheContent的信息
        JwtCacheContent content = new JwtCacheContent();
        // redis.set(KEY[uid],toJson(content),expSeconds);
        return jwt;
    }
    @Override
    public VerifyJwtResultDto verify(String jwt) {
        JwtParser parser = Jwts.parserBuilder()
                .requireIssuer(this.issuer)
                .setSigningKey(this.secretKey)
                .build();
        VerifyJwtResultDto resultDto = new VerifyJwtResultDto();
        try {
            Jws<Claims> parseResult = parser.parseClaimsJws(jwt);
            Claims claims = parseResult.getBody();
            long jti = Long.parseLong(claims.getId());
            if (isInBlockList(jti)) {
                throw new IllegalArgumentException(String.format("jti is in block list,[i:%d]", jti));
            }
            long uid = claims.get("uid", Long.class);
            // JwtCacheContent content = JSON.parse(redis.get(KEY[uid]),JwtCacheContent.class);
            // resultDto.setContent(content);
            resultDto.setValid(Boolean.TRUE);
        } catch (Exception e) {
            resultDto.setValid(Boolean.FALSE);
            resultDto.setThrowable(e);
        }
        return resultDto;
    }
    @Override
    public void blockJwt(long jwtId) {
    }
    @Override
    public boolean isInBlockList(long jwtId) {
        return false;
    }
}
```
然后是`JwtGlobalFilter`和`JwtWebFilter`的非完全实现：
```java
@Component
public class JwtGlobalFilter implements GlobalFilter, Ordered, EnvironmentAware {
    private final AntPathMatcher pathMatcher = new AntPathMatcher();
    private List<String> accessUriList;
    @Autowired
    private JwtSpi jwtSpi;
    private static final String JSON_WEB_TOKEN_KEY = "X-TOKEN";
    private static final String UID_KEY = "X-UID";
    private static final String JWT_ID_KEY = "X-JTI";
    @Override
    public void setEnvironment(Environment environment) {
        accessUriList = Arrays.asList(Objects.requireNonNull(environment.getProperty("jwt.access.uris"))
                .split(","));
    }
    @Override
    public Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain) {
        ServerHttpRequest request = exchange.getRequest();
        // OPTIONS 请求直接放行
        HttpMethod method = request.getMethod();
        if (Objects.nonNull(method) && Objects.equals(method, HttpMethod.OPTIONS)) {
            return chain.filter(exchange);
        }
        // 获取请求路径
        String requestPath = request.getPath().value();
        // 命中请求路径白名单
        boolean matchWhiteRequestPathList = Optional.ofNullable(accessUriList)
                .map(paths -> paths.stream().anyMatch(path -> pathMatcher.match(path, requestPath)))
                .orElse(false);
        if (matchWhiteRequestPathList) {
            return chain.filter(exchange);
        }
        HttpHeaders headers = request.getHeaders();
        String token = headers.getFirst(JSON_WEB_TOKEN_KEY);
        if (!StringUtils.hasLength(token)) {
            throw new BusinessException(BusinessErrorCode.TOKEN_ERROR.getCode(), "token is null");
        }
        VerifyJwtResultDto resultDto = jwtSpi.verify(token);
        if (Objects.equals(resultDto.getValid(), Boolean.FALSE)) {
            throw new BusinessException(BusinessErrorCode.TOKEN_ERROR.getCode(), resultDto.getThrowable());
        }
        headers.set(JWT_ID_KEY, String.valueOf(resultDto.getJwtId()));
        headers.set(UID_KEY, String.valueOf(resultDto.getContent().getCustomerId()));
        return chain.filter(exchange);
    }
    @Override
    public int getOrder() {
        return 1;
    }
}

@Component
public class JwtWebFilter implements WebFilter {
    @Autowired
    private RequestMappingHandlerMapping requestMappingHandlerMapping;
    @Autowired
    private JwtSpi jwtSpi;
    private static final String JSON_WEB_TOKEN_KEY = "X-TOKEN";
    private static final String UID_KEY = "X-UID";
    private static final String JWT_ID_KEY = "X-JTI";
    @Override
    public Mono<Void> filter(ServerWebExchange exchange, WebFilterChain chain) {
        // OPTIONS 请求直接放行
        HttpMethod method = exchange.getRequest().getMethod();
        if (Objects.nonNull(method) && Objects.equals(method, HttpMethod.OPTIONS)) {
            return chain.filter(exchange);
        }
        HandlerMethod handlerMethod = requestMappingHandlerMapping.getHandlerInternal(exchange).block();
        if (Objects.isNull(handlerMethod)) {
            return chain.filter(exchange);
        }
        RequireJWT typeAnnotation = handlerMethod.getBeanType().getAnnotation(RequireJWT.class);
        RequireJWT methodAnnotation = handlerMethod.getMethod().getAnnotation(RequireJWT.class);
        if (Objects.isNull(typeAnnotation) && Objects.isNull(methodAnnotation)) {
            return chain.filter(exchange);
        }
        HttpHeaders headers = exchange.getRequest().getHeaders();
        String token = headers.getFirst(JSON_WEB_TOKEN_KEY);
        if (!StringUtils.hasLength(token)) {
            throw new BusinessException(BusinessErrorCode.TOKEN_ERROR.getCode(), "token is null");
        }
        VerifyJwtResultDto resultDto = jwtSpi.verify(token);
        if (Objects.equals(resultDto.getValid(), Boolean.FALSE)) {
            throw new BusinessException(BusinessErrorCode.TOKEN_ERROR.getCode(), resultDto.getThrowable());
        }
        headers.set(JWT_ID_KEY, String.valueOf(resultDto.getJwtId()));
        headers.set(UID_KEY, String.valueOf(resultDto.getContent().getCustomerId()));
        return chain.filter(exchange);
    }
}
```
最后是一些配置属性：
```
jwt.hmac.secretKey='00000000111111112222222233333333'
jwt.exp.seed.min=360000
jwt.exp.seed.max=8640000
jwt.issuer='throwx'
jwt.access.uris=/index,/actuator/*
```
<a name="4zLrf"></a>
## 使用JWT曾经遇到的坑
`API`网关使用了`JWT`应用于认证场景，算法上使用了安全性稍高的`RS256`，使用`RSA`算法进行签名生成。项目上线初期，`JWT`的过期时间都固定设置为`7`天，生产日志发现该`API`网关周期性发生"假死"现象，具体表现为：

- `Nginx`自检周期性出现自检接口调用超时，提示部分或者全部`API`网关节点宕机
- `API`网关所在机器的`CPU`周期性飙高，在用户访问量低的时候表现平稳
- 通过`ELK`进行日志排查，发现故障出现时段有`JWT`集中性过期和重新生成的日志痕迹

排查结果表明`JWT`集中过期和重新生成时候使用`RSA`算法进行签名是`CPU`密集型操作，同时重新生成大量`JWT`会导致服务所在机器的`CPU`超负载工作。**「初步的解决方案是」**：

- `JWT`生成的时候，过期时间添加一个随机数，例如`360000(1小时的毫秒数) ~ 8640000(24小时的毫秒数)`之间取一个随机值添加到当前时间戳加`7`天得到`exp`值

这个方法，对于一些老用户营销场景（老用户长时间没有登录，他们客户端缓存的`JWT`一般都已经过期）没有效果。有时候运营会通过营销活动唤醒老用户，大量老用户重新登录有可能出现爆发性大批量重新生成`JWT`的情况，对于这个场景提出两个解决思路：

- 首次生成`JWT`时候，考虑延长过期时间，但是时间越长，风险越大
- 提升`API`网关所在机器的硬件配置，特别是`CPU`配置，现在很多云厂商都有弹性扩容方案，可以很好应对这类突发流量场景
<a name="21tUL"></a>
## 小结
主流的`JWT`方案是`JWS`，此方案是只编码和签名，不加密，务必注意这一点，`JWS`方案是无状态并且不安全的，关键操作应该做多重认证，也要做好黑名单机制防止`JWT`泄漏后造成安全性问题。`JWT`不存储在服务端，这既是它的优势，同时也是它的劣势。很多软件架构都无法做到尽善尽美，这个时候只能权衡利弊。
