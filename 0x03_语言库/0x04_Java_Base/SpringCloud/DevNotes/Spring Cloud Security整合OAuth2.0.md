Java SpringCloudSecurity OAuth2.0<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911841902-050fe162-2c62-479a-9f40-edc3dff0f71e.webp#clientId=u0d470160-2d8d-4&from=paste&id=ube720680&originHeight=1427&originWidth=865&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u63a15f3c-6679-4bd1-a6dc-2f563e3db2e&title=)
<a name="DT8sE"></a>
## 为什么需要OAuth2.0？
编码永远都是为了解决生产中的问题，想要理解为什么需要OAuth2，当然要从实际生活出发。<br />举个例子：小区的业主点了一份外卖，但是小区的门禁系统不给外卖人员进入，此时想要外卖员进入只能业主下来开门或者告知门禁的密码。<br />密码告知外卖员岂不是每次都能凭密码进入小区了，这明显造成了**安全隐患**。<br />**那么有没有一种方案：既能不泄露密码，也能让外卖小哥进入呢？**<br />于是此时就想到了一个授权机制，分为以下几个步骤：

1. 门禁系统中新增一个**授权按钮**，外卖小哥只需要点击授权按钮呼叫对应业主
2. 业主收到小哥的呼叫，知道小哥正在要求授权，于是做出了**应答授权**
3. 此时门禁系统弹出一个**密码**（类似于**access_token**），**有效期30分钟**，在30分钟内，小哥可以凭借这个密码进入小区。
4. 小哥输入密码进入小区

另外这个授权的密码不仅可以通过门禁，还可以通过楼下的门禁，这就非常类似于**网关**和**微服务**了。
<a name="XkelX"></a>
## 令牌和密码的区别？
上述例子中令牌和密码的作用是一样的，都可以进入小区，但是存在以下几点差异：

1. **时效不同**：令牌一般都是存在过期时间的，比如30分钟后失效，这个是无法修改的，除非重新申请授权；而密码一般都是永久的，除非主人去修改
2. **权限不同**：令牌的权限是有限的，比如上述例子中，小哥获取了令牌，能够打开小区的门禁、业主所在的楼下门禁，但是可能无法打开其它幢的门禁；
3. **令牌可以撤销**：业主可以撤销这个令牌的授权，一旦撤销了，这个令牌也就失效了，无法使用；但是密码一般不允许撤销。
<a name="cpeww"></a>
## 什么是OAuth2？
OAuth 是一个开放标准，该标准允许用户让第三方应用访问该用户在某一网站上存储的私密**资源**（如头像、照片、视频等），而在这个过程中无需将**用户名**和**密码**提供给**第三方应用**。实现这一功能是通过提供一个令牌（**token**），而不是用户名和密码来访问他们存放在特定**服务提供者**的数据。<br />采用令牌（token）的方式可以让用户灵活的对第三方应用授权或者收回权限。<br />OAuth2 是 OAuth 协议的下一版本，但不向下兼容 **OAuth 1.0**。<br />传统的 Web 开发登录认证一般都是基于 **session** 的，但是在**前后端分离**的架构中继续使用 session 就会有许多不便，因为移动端（Android、iOS、微信小程序等）要么不支持 **cookie**（微信小程序），要么使用非常不便，对于这些问题，使用 **OAuth2** 认证都能解决。<br />对于大家而言，在互联网应用中最常见的 OAuth2 应该就是各种第三方登录了，例如 **QQ 授权登录**、**微信授权登录**、**微博授权登录**、**GitHub 授权登录**等等。
<a name="i0IRs"></a>
## OAuth2.0的四种模式？
OAuth2.0协议一共支持 4 种不同的授权模式：

1. **授权码模式**：常见的第三方平台登录功能基本都是使用这种模式。
2. **简化模式**：简化模式是不需要客户端服务器参与，直接在浏览器中向授权服务器申请令牌（token），一般如果网站是纯静态页面则可以采用这种方式。
3. **密码模式**：密码模式是用户把用户名密码直接告诉客户端，客户端使用说这些信息向授权服务器申请令牌（token）。这需要用户对客户端高度信任，例如客户端应用和服务提供商就是同一家公司，自己做前后端分离登录就可以采用这种模式。
4. **客户端模式**：客户端模式是指客户端使用自己的名义而不是用户的名义向服务提供者申请授权，严格来说，客户端模式并不能算作 OAuth 协议要解决的问题的一种解决方案，但是，对于开发者而言，在一些前后端分离应用或者为移动端提供的认证授权服务器上使用这种模式还是非常方便的。
<a name="jICru"></a>
### 1、授权码模式
这种方式是最常用的流程，**安全性也最高**，它适用于那些有后端的 Web 应用。授权码通过前端传送，令牌则是储存在后端，而且所有与资源服务器的通信都在后端完成。这样的前后端分离，可以避免令牌泄漏。<br />令牌获取的流程如下：<br />![授权码模式](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911841896-de7191f8-9a3f-45fb-b499-7a5fb30031e0.webp#clientId=u0d470160-2d8d-4&from=paste&id=ud1e4d2bd&originHeight=370&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uab5ef3e8-e490-4d17-8f30-45cb232dd5e&title=%E6%8E%88%E6%9D%83%E7%A0%81%E6%A8%A1%E5%BC%8F "授权码模式")<br />上图中涉及到两个角色，分别是客户端、认证中心，客户端负责拿令牌，认证中心负责发放令牌。<br />但是不是所有客户端都有权限请求令牌的，需要事先在认证中心申请，比如微信并不是所有网站都能直接接入，而是要去微信后台开通这个权限。<br />至少要提前向认证中心申请的几个参数如下：

1. **client_id**：客户端唯一id，认证中心颁发的唯一标识
2. **client_secret**：客户端的秘钥，相当于密码
3. **scope**：客户端的权限
4. **redirect_uri**：授权码模式使用的跳转uri，需要事先告知认证中心。
<a name="EBGvH"></a>
#### 1、请求授权码
客户端需要向认证中心拿到授权码，比如第三方登录使用微信，扫一扫登录那一步就是向微信的认证中心获取授权码。<br />请求的url如下：
```http
/oauth/authorize?client_id=&response_type=code&scope=&redirect_uri=
```
上述这个url中携带的几个参数如下：

- **client_id**：客户端的id，这个由认证中心分配，并不是所有的客户端都能随意接入认证中心
- **response_type**：固定值为**code**，表示要求返回授权码。
- **scope**：表示要求的授权范围，客户端的权限
- **redirect_uri**：跳转的uri，认证中心同意或者拒绝授权跳转的地址，如果同意会在uri后面携带一个code=xxx，这就是授权码
<a name="pJOkU"></a>
#### 2、返回授权码
第1步请求之后，认证中心会要求登录、是否同意授权，用户同意授权之后直接跳转到redirect_uri（这个需要事先在认证中心申请配置），授权码会携带在这个地址后面，如下：
```http
http://xxxx?code=NMoj5y
```
上述链接中的NMoj5y就是授权码了。
<a name="jlmcZ"></a>
#### 3、请求令牌
客户端拿到授权码之后，直接携带授权码发送请求给认证中心获取令牌，请求的url如下：
```http
/oauth/token?
 client_id=&
 client_secret=&
 grant_type=authorization_code&
 code=NMoj5y&
 redirect_uri=
```
相同的参数同上，不同参数解析如下：

- **grant_type**：授权类型，授权码固定的值为**authorization_code**
- **code**：这个就是上一步获取的授权码
<a name="uVXsV"></a>
#### 4、返回令牌
认证中心收到令牌请求之后，通过之后，会返回一段JSON数据，其中包含了令牌**access_token**，如下：
```json
{    
  "access_token":"ACCESS_TOKEN",
  "token_type":"bearer",
  "expires_in":2592000,
  "refresh_token":"REFRESH_TOKEN",
  "scope":"read",
  "uid":100101
}
```
**access_token**则是颁发的令牌，refresh_token是刷新令牌，一旦令牌失效则携带这个令牌进行刷新。
<a name="ny34F"></a>
### 2、简化模式
这种模式不常用，主要针对那些无后台的系统，直接通过web跳转授权，流程如下图：<br />![简化模式](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911841866-3cc61e48-de4e-49d0-8b06-b8dc1fb855cc.webp#clientId=u0d470160-2d8d-4&from=paste&id=ua4b6dbae&originHeight=370&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ue359855c-6faf-4f3b-9e18-a539266303d&title=%E7%AE%80%E5%8C%96%E6%A8%A1%E5%BC%8F "简化模式")<br />这种方式把令牌直接传给前端，是很不安全的。因此，只能用于一些安全要求不高的场景，并且令牌的有效期必须非常短，通常就是会话期间（**session**）有效，浏览器关掉，令牌就失效了。
<a name="EThVj"></a>
#### 1、请求令牌
客户端直接请求令牌，请求的url如下：
```http
/oauth/authorize?
  response_type=token&
  client_id=CLIENT_ID&
  redirect_uri=CALLBACK_URL&
  scope=
```
这个url正是授权码模式中获取授权码的url，各个参数解析如下：

- client_id：客户端的唯一Id
- response_type：简化模式的固定值为**token**
- scope：客户端的权限
- redirect_uri：跳转的uri，这里后面携带的直接是**令牌**，不是授权码了。
<a name="T9Asw"></a>
#### 2、返回令牌
认证中心认证通过后，会跳转到**redirect_uri**，并且后面携带着令牌，链接如下：
```http
https://xxxx#token=NPmdj5
```
#token=NPmdj5这一段后面携带的就是认证中心携带的，令牌为**NPmdj5**。
<a name="z0Mhl"></a>
### 3、密码模式
密码模式也很简单，直接通过**用户名**、**密码**获取令牌，流程如下：<br />![密码模式](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911842153-21f5b0c9-de63-474f-96dc-3961ddbb7570.webp#clientId=u0d470160-2d8d-4&from=paste&id=u85cbc30c&originHeight=370&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u653d30f3-a3e4-42ce-a980-fa200ab0c9e&title=%E5%AF%86%E7%A0%81%E6%A8%A1%E5%BC%8F "密码模式")
<a name="jN1nr"></a>
#### 1、请求令牌
认证中心要求客户端输入用户名、密码，认证成功则颁发令牌，请求的url如下：
```http
/oauth/token?
  grant_type=password&
  username=&
  password=&
  client_id=&
  client_secret=
```
参数解析如下：

- **grant_type**：授权类型，密码模式固定值为password
- **username**：用户名
- **password**：密码
- **client_id**：客户端id
- **client_secret**：客户端的秘钥
<a name="rplFu"></a>
#### 2、返回令牌
上述认证通过，直接返回JSON数据，不需要跳转，如下：
```json
{    
  "access_token":"ACCESS_TOKEN",
  "token_type":"bearer",
  "expires_in":2592000,
  "refresh_token":"REFRESH_TOKEN",
  "scope":"read",
  "uid":100101
}
```
**access_token**则是颁发的令牌，refresh_token是刷新令牌，一旦令牌失效则携带这个令牌进行刷新。
<a name="ME6GO"></a>
### 4、客户端模式
适用于没有前端的命令行应用，即在命令行下请求令牌。<br />**这种方式给出的令牌，是针对第三方应用的，而不是针对用户的，即有可能多个用户共享同一个令牌。**<br />流程如下：<br />![客户端模式](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911841825-6cfed11f-d418-4348-8187-f9888897326a.webp#clientId=u0d470160-2d8d-4&from=paste&id=uceb3c310&originHeight=370&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u5abb3f27-55b1-4ca5-84c6-b0749687217&title=%E5%AE%A2%E6%88%B7%E7%AB%AF%E6%A8%A1%E5%BC%8F "客户端模式")
<a name="lslpT"></a>
#### 1、请求令牌
请求的url为如下：
```http
/oauth/token?
grant_type=client_credentials&
client_id=&
client_secret=
```
参数解析如下：

- **grant_type**：授权类型，客户端模式固定值为**client_credentials**
- **client_id**：客户端id
- **client_secret**：客户端秘钥
<a name="JD3TR"></a>
#### 2、返回令牌
认证成功后直接返回令牌，格式为JSON数据，如下：
```json
{
    "access_token": "ACCESS_TOKEN",
    "token_type": "bearer",
    "expires_in": 7200,
    "scope": "all"
}
```
<a name="IUiH9"></a>
## OAuth2.0的认证中心搭建
为了方便测试OAuth2的四种授权模式，这里为了方便测试，简单搭建一个认证中心，后续会逐渐完善。
<a name="PabYJ"></a>
### 1、案例架构
这里使用的是**Spring Boot + Spring Cloud Alibaba** 作为基础搭建，新建一个oauth2-auth-server-in-memory模块作为认证中心，目录如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911842902-587b8e11-e6e8-429f-9db0-3ffafcd2366b.webp#clientId=u0d470160-2d8d-4&from=paste&id=u8fa003ad&originHeight=402&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a604cb0-da69-4cd4-90a5-14f230f5a6b&title=)
<a name="tTWcg"></a>
### 2、添加依赖
Spring Boot 和 Spring Cloud 的相关依赖这里就不再说了，直接上Spring Security和OAuth2的依赖，如下：
```xml
<!--spring security的依赖-->
<dependency>
   <groupId>org.springframework.boot</groupId>
 <artifactId>spring-boot-starter-security</artifactId>
</dependency>

<!--OAuth2的依赖-->
<dependency>
 <groupId>org.springframework.cloud</groupId>
 <artifactId>spring-cloud-starter-oauth2</artifactId>
</dependency>
```
<a name="tGMj4"></a>
### 3、Spring Security安全配置
这里主要涉及到Spring Security的配置。<br />SecurityConfig这个配置类中主要设置有4块内容，如下：
<a name="Aj4Z4"></a>
#### 1、加密方式
采用`**BCryptPasswordEncoder**`加密，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911842973-51cb85ec-b4e5-422e-8723-f5d22884e59c.webp#clientId=u0d470160-2d8d-4&from=paste&id=u674d2cf9&originHeight=189&originWidth=419&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3dcdf6fe-9148-4cdd-b94b-bd5ecb95d05&title=)
<a name="xVt0I"></a>
#### 2、配置用户
这里为了方便测试，直接将用户信息存储在内存中，**后续完善**，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911842899-53a44711-d8df-4e39-879c-c32d6b8e7dd1.webp#clientId=u0d470160-2d8d-4&from=paste&id=u1eb2f3a4&originHeight=329&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u25ceaa35-4153-4ad9-94fb-bb8e7cd51ed&title=)<br />上述代码配置了两个用户，如下：

- 用户名admin，密码123，角色admin
- 用户名user，密码123，角色user
<a name="v3OlT"></a>
#### 3、注入认证管理器`AuthenticationManager`
`AuthenticationManager`在**密码授权模式**下会用到，这里提前注入，如果用的不是密码模式，可以不注入，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911843058-11196b65-0e46-4294-a871-4eb4cbb85f1a.webp#clientId=u0d470160-2d8d-4&from=paste&id=u695ab116&originHeight=229&originWidth=765&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub777e406-89dc-4080-946e-cffa06cd88c&title=)
<a name="YUOOP"></a>
#### 4、配置安全拦截策略
由于需要验证授权码模式，因此开启表单提交模式，所有url都需要认证，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911843442-0f034a7e-908e-4b9d-a97f-fd8ed89d0bbf.webp#clientId=u0d470160-2d8d-4&from=paste&id=ud58861e8&originHeight=333&originWidth=920&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf225fa64-0a21-444f-91dc-541fcd8c316&title=)
<a name="w5VpK"></a>
### 4、令牌存储策略配置
令牌支持多种方式存储，比如内存方式、**Redis**、**JWT**，比较常用的两种则是Redis、JWT。<br />这里暂时使用内存存储的方式，一旦服务器重启令牌将会失效。<br />代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911843535-ac327c04-5628-4803-9803-a9839b35bbd5.webp#clientId=u0d470160-2d8d-4&from=paste&id=uecf1fb35&originHeight=410&originWidth=779&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf9a0e186-1159-412e-bf95-11bc9c09c1c&title=)
<a name="HUXcL"></a>
### 5、OAuth2.0的配置类
不是所有配置类都可以作为**OAuth2**.0认证中心的配置类，需要满足以下两点：

1. 继承`**AuthorizationServerConfigurerAdapter**`
2. 标注 `**@EnableAuthorizationServer**` 注解

代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911843281-7c883fb8-12c3-40a3-b402-110d33d5ac6f.webp#clientId=u0d470160-2d8d-4&from=paste&id=u2ff599a2&originHeight=243&originWidth=808&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud2a4fe46-224b-4c6b-b8b1-7e44ad1e1b3&title=)<br />`**AuthorizationServerConfigurerAdapter**`需要实现的三个方法如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911843607-a74c4987-3fef-43fd-846d-2411a52f53c2.webp#clientId=u0d470160-2d8d-4&from=paste&id=u8721c6dc&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u289ddb44-b3a9-4c9d-87ca-b174e6078f6&title=)<br />下面便是围绕这三个方法进行OAuth2的详细配置。
<a name="N2YO8"></a>
### 6、客户端配置
在介绍OAuth2.0 协议的时候介绍到，并不是所有的客户端都有权限向认证中心申请令牌的，首先认证中心要知道你是谁，你有什么资格？<br />因此一些必要的配置是要认证中心分配给你的，比如**客户端唯一Id**、**秘钥**、**权限**。<br />客户端配置的存储也支持多种方式，比如**内存**、**数据库**，对应的接口为：`**org.springframework.security.oauth2.provider.ClientDetailsService**`，接口如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911843942-2ec1b409-1786-4534-b42b-bfcdb1e0601e.webp#clientId=u0d470160-2d8d-4&from=paste&id=u506e019d&originHeight=279&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u07c30898-10c3-4a60-a3e2-345a918d44a&title=)<br />同样这里为了方便测试，依然是加载在内存中，**后续完善**，完整的配置如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844009-25b912b7-3e50-417d-b8d4-a1ee7ce7598d.webp#clientId=u0d470160-2d8d-4&from=paste&id=u857f2a38&originHeight=471&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5d9c5c83-59ab-431d-b690-8281fe07c79&title=)<br />几个重要参数说一下，如下：

- `.withClient("myjszl")`：指定客户端唯一ID为myjszl
- `.secret()`：指定秘钥，使用加密算法加密了，秘钥为123
- `.resourceIds("res1")`：给客户端分配的资源权限，对应的是资源服务，比如订单这个微服务就可以看成一个资源，作为客户端肯定不是所有资源都能访问。
- `authorizedGrantTypes()`：定义认证中心支持的授权类型，总共支持五种
   - 授权码模式：**authorization_code**
   - 密码模式：**password**
   - 客户端模式：**client_credentials**
   - 简化模式：**implicit**
   - 令牌刷新：**refresh_token**，这并不是**OAuth2**的模式，定义这个表示认证中心支持令牌刷新
- `scopes()`：定义客户端的权限，这里只是一个标识，资源服务可以根据这个权限进行鉴权。
- `autoApprove`：是否需要授权，设置为true则不需要用户点击确认授权直接返回授权码
- `redirectUris`：跳转的uri
<a name="ngXMi"></a>
### 7、授权码服务配置
使用授权码模式必须配置一个授权码服务，用来颁布和删除授权码，当然授权码也支持多种方式存储，比如内存，数据库，这里暂时使用内存方式存储，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911843861-83afe491-ba85-4499-96fc-df39b64c3181.webp#clientId=u0d470160-2d8d-4&from=paste&id=ufdbb281c&originHeight=271&originWidth=914&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u52f1abc7-278c-45b2-b82b-309ea9ab9f7&title=)
<a name="L4r5Y"></a>
### 8、令牌服务的配置
除了令牌的存储策略需要配置，还需要配置令牌的服务`AuthorizationServerTokenServices`用来创建、获取、刷新令牌，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911843833-29388882-5a6e-4ed9-9758-95c85bd6140b.webp#clientId=u0d470160-2d8d-4&from=paste&id=uaa49bf89&originHeight=539&originWidth=977&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua17228fa-7f04-497e-b91e-c6005ae9578&title=)
<a name="KxCUe"></a>
### 9、令牌访问端点的配置
目前这里仅仅配置了四个，分别如下：

- 配置了授权码模式所需要的服务，`**AuthorizationCodeServices**`
- 配置了密码模式所需要的`**AuthenticationManager**`
- 配置了令牌管理服务，`**AuthorizationServerTokenServices**`
- 配置`/oauth/token`申请令牌的uri只允许POST提交。

详细代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844147-bfbe19c6-d4b7-45c1-a655-ba443df37e7e.webp#clientId=u0d470160-2d8d-4&from=paste&id=u8f77d328&originHeight=452&originWidth=972&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u86a1d619-e2bd-42bd-91e7-9d84888ded2&title=)<br />spring Security框架默认的访问端点有如下6个：

- **/oauth/authorize**：获取授权码的端点
- **/oauth/token**：获取令牌端点。
- **/oauth/confifirm_access**：用户确认授权提交端点。
- **/oauth/error**：授权服务错误信息端点。
- **/oauth/check_token**：用于资源服务访问的令牌解析端点。
- **/oauth/token_key**：提供公有密匙的端点，如果使用JWT令牌的话。

当然如果业务要求需要改变这些默认的端点的url，也是可以修改的，`AuthorizationServerEndpointsConfigurer`有一个方法，如下：
```java
public AuthorizationServerEndpointsConfigurer pathMapping(String defaultPath, String customPath)
```
**第一个参数**：需要替换的默认端点url<br />**第二个参数**：自定义的端点url
<a name="LE3NM"></a>
### 10、令牌访问安全约束配置
主要对一些端点的权限进行配置，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844254-5fec6f2a-908b-42db-854e-f4334bcb8b23.webp#clientId=u0d470160-2d8d-4&from=paste&id=u5bd50bd0&originHeight=413&originWidth=842&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u73771568-dd99-4d76-8a60-4a6e615146a&title=)
<a name="GRTZ0"></a>
## OAuth2.0的资源服务搭建
客户端申请令牌的目的就是为了访问资源，当然这个资源也是分权限的，一个令牌不是所有资源都能访问的。<br />在认证中心搭建的第6步配置客户端详情的时候，一行代码`.resourceIds("res1")`则指定了能够访问的资源，可以配置多个，这里的**res1**则是唯一对应一个资源。
<a name="wFb14"></a>
### 1、案例架构
使用**Spring Boot + Spring Cloud Alibaba** 作为基础搭建，新建一个oauth2-auth-resource-in-memory模块作为认证中心，目录如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844704-1c0e75ba-779e-49eb-baad-c71a4b36de88.webp#clientId=u0d470160-2d8d-4&from=paste&id=u3c76f1e8&originHeight=333&originWidth=853&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u734b6bcc-9512-4791-b78d-fade5b47593&title=)
<a name="fHMxR"></a>
### 2、OAuth2.0的配置类
作为资源服务的配置类必须满足两个条件，如下：

- 标注注解`@EnableResourceServer`
- 继承`ResourceServerConfigurerAdapter`

代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844398-a7e6de31-820e-4bb0-85f6-6bd8a1387e23.webp#clientId=u0d470160-2d8d-4&from=paste&id=ubd15e571&originHeight=327&originWidth=1061&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1ec267f8-a6d8-447a-9c67-96ca5feb493&title=)
<a name="JocT9"></a>
### 3、令牌校验服务配置
由于认证中心使用的令牌存储策略是在内存中的，因此服务端必须远程调用认证中心的校验令牌端点**/oauth/check_token**进行校验。<br />代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844521-a0d11e61-393c-4fb3-a2e9-39c76864c9a4.webp#clientId=u0d470160-2d8d-4&from=paste&id=u496ea7b6&originHeight=486&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u79fc4574-8481-4ee7-a661-7baa26456d2&title=)
:::danger
注意：远程校验令牌存在性能问题，但是后续使用JWT令牌则本地即可进行校验，不必远程校验了。
:::
<a name="r0c0b"></a>
### 4、配置客户端唯一id和令牌校验服务
上文说到客户端有一个唯一标识，因此需要配置上，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844588-2dea24e4-3fa7-49f4-b0e9-d9e745dd13f2.webp#clientId=u0d470160-2d8d-4&from=paste&id=u45a66866&originHeight=322&originWidth=880&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1220bb0a-4d2f-4119-9c8b-01a0bf64a9b&title=)
<a name="KPhws"></a>
### 5、配置security的安全机制
上文在认证中心的第6步配置客户端详情那里，有一行代码`.scopes("all")`则是指定了客户端的权限，资源服务可以根据这个scope进行url的拦截。<br />拦截方式如下：
```java
.access("#oauth2.hasScope('')")
```
详细配置代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844908-caaf2959-8fcd-404c-b321-b3c75c1ef7f3.webp#clientId=u0d470160-2d8d-4&from=paste&id=u2a282d48&originHeight=304&originWidth=1011&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub424f3c9-90dc-4c20-8a30-e1e051561d0&title=)<br />这里配置了所有路径都需要**all**的权限。
<a name="mmf98"></a>
### 6、新建测试接口
新建了两个接口，如下：

- /hello：认证成功都可以访问
- /admin：只有具备**ROLE_admin**角色的用户才可以访问

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911844795-06c455b0-9326-4f8c-9e7d-149302af885b.webp#clientId=u0d470160-2d8d-4&from=paste&id=u286f4d0e&originHeight=593&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua6f806ea-42e8-4dec-8458-e3921c78e47&title=)
<a name="kuQ89"></a>
## OAuth2.0的四种模式测试
下面结合认证中心、资源服务对OAuth2.0的四种服务进行测试。<br />启动上述搭建的认证中心和资源服务，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845300-2afd2a8b-d3da-42d1-995c-d080501c4906.webp#clientId=u0d470160-2d8d-4&from=paste&id=u5f388a6f&originHeight=134&originWidth=543&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1c68430a-9568-4a45-a173-9a6fb536a90&title=)
<a name="QssgS"></a>
### 授权码模式
<a name="FOed8"></a>
#### 1、获取授权码
请求的url如下：
```http
http://localhost:2003/auth-server/oauth/authorize?client_id=myjszl&response_type=code&scope=all&redirect_uri=http://www.baidu.com
```
浏览器访问，security需要登录，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845448-62465d3f-27e6-4747-a265-0ed789913eed.webp#clientId=u0d470160-2d8d-4&from=paste&id=udf119ca6&originHeight=339&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u46281aed-beb2-4fe1-b0a6-97c24ddacdf&title=)<br />输入用户名**user**，密码**123**，成功登录。<br />此时来到了**确认授权**的页面，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845519-d68e43d0-2b19-494b-8664-38c8012b3df2.webp#clientId=u0d470160-2d8d-4&from=paste&id=ueed1841e&originHeight=227&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udc0d362f-0963-4c04-b667-e20c80afde5&title=)<br />选择Apporove、确认授权，成功跳转到了百度页面，并且携带了授权码，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845217-8752a77a-b289-49f6-b7ca-aa3f20a31230.webp#clientId=u0d470160-2d8d-4&from=paste&id=u639b03e9&originHeight=73&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8f243276-6350-42cf-adb0-492e22dc6cf&title=)<br />这里的**6yV2bF**就是获取到的授权码。
<a name="rGNLG"></a>
#### 2、获取token
```http
http://localhost:2003/auth-server/oauth/token?code=jvMH5U&client_id=myjszl&client_secret=123&redirect_uri=http://www.baidu.com&grant_type=authorization_code
```
**注意**：**/oauth/token**获取token的接口请求允许的方式要配置在授权服务器中，比如配置**POST**方式，代码如下：
```java
.allowedTokenEndpointRequestMethods(HttpMethod.POST)
```
POSTMAN请求如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845369-c95d5e2b-25bf-4400-a365-0a7ed5d02ec3.webp#clientId=u0d470160-2d8d-4&from=paste&id=ub680b0fe&originHeight=549&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u82f8d953-0e81-4735-899c-a17106759e3&title=)
<a name="GuizV"></a>
#### 3、访问资源服务
拿着令牌访问资源服务的**/hello**接口，请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845826-4906cd39-6e9f-4b97-bc03-cfb2ee52252e.webp#clientId=u0d470160-2d8d-4&from=paste&id=u6dbac915&originHeight=387&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u55ca42f8-807d-43e3-b2f9-a804b213c48&title=)<br />请求头需要添加**Authorization**，并且值为**Bearer+" "+access_token**的形式。<br />**注意**：Bearer后面一定要跟一个空格。
<a name="emkAj"></a>
### 密码模式
密码模式比较简单，不用先获取授权码，直接使用**用户名**、**密码**获取token。<br />POSTMAN请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845940-90cb1e06-9fa6-4657-bfb2-9fb89093915b.webp#clientId=u0d470160-2d8d-4&from=paste&id=u626e7353&originHeight=537&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2f0a84d3-3606-4d82-a17d-e5ef085358c&title=)<br />访问资源可以自行拿着获取到的令牌尝试下.....
<a name="NhfWb"></a>
### 简化模式
简化模式就很简单了，拿着客户端id就可以获取token，请求的url如下：
```http
http://localhost:2003/auth-server/oauth/authorize?response_type=token&client_id=myjszl&redirect_uri=http://www.baidu.com&scope=all
```
这个过程和获取授权码一样，需要**登录**，**同意授权**<br />最终跳转到百度，链接后面直接携带了令牌，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845826-5ab86c89-7b2b-40a1-8cfd-e29d7ec0a3df.webp#clientId=u0d470160-2d8d-4&from=paste&id=u0bb1684c&originHeight=178&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u24476fdf-7b50-4cde-856c-5d5829334e2&title=)<br />上图中的**0d5ecf06-b255-4272-b0fa-8e51dde2ce3e**则是获取的令牌。<br />访问资源可以自己尝试下..........
<a name="DV8kc"></a>
### 客户端模式
请求的url如下：
```http
http://localhost:2003/auth-server/oauth/token?client_id=myjszl&client_secret=123&grant_type=client_credentials
```
POSTMAN请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845941-2a94a607-fb5c-4298-a9e2-3c2c43f87ff8.webp#clientId=u0d470160-2d8d-4&from=paste&id=u183a4cb6&originHeight=552&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0e6aaa00-14e5-428e-861f-446f58fa1e7&title=)<br />访问资源可以自行尝试下..........
<a name="mskZ8"></a>
## OAuth2.0 其他端点的测试
Spring Security OAuth2.0还提供了其他的端点，下面来逐一测试一下。
<a name="NB9vu"></a>
### 1、刷新令牌
OAuth2.0提供了令牌刷新机制，一旦**access_token**过期，客户端可以拿着**refresh_token**去请求认证中心进行令牌的续期。<br />请求的url如下：
```http
http://localhost:2003/auth-server/oauth/token?client_id=myjszl&client_secret=123&grant_type=refresh_token&refresh_token=
```
POSTMAN请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911845993-be3fdc05-735e-444d-865f-8072a4eda971.webp#clientId=u0d470160-2d8d-4&from=paste&id=uaa9a135b&originHeight=620&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub8386340-106c-4602-a3a1-19bd975f8b8&title=)
<a name="lH3EI"></a>
### 2、校验令牌
OAuth2.0还提供了校验令牌的端点，请求的url如下：
```http
http://localhost:2003/auth-server/oauth/check_token?toke=
```
POSTMAN请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640911846622-63448e7e-1b06-473e-b5bf-f003672f6a17.webp#clientId=u0d470160-2d8d-4&from=paste&id=u0573c175&originHeight=605&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc5bbdc3d-6b66-47e6-86c4-43e611ce1d3&title=)
<a name="ZCV8W"></a>
## 总结
本文介绍了OAuth2.0协议原理、四种授权模式，并且搭建了认证授权中心、资源服务进行了四种模式的测试。
