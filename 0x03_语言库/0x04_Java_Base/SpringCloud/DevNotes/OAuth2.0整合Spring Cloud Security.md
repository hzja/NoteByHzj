Java SpringCloud Spring Cloud Security OAuth2.0<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139188-1b50486b-7a2e-460b-90a8-054f424001bb.webp#clientId=u5cd47139-847d-4&from=paste&id=u13887b2a&originHeight=1427&originWidth=865&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u44633e63-5490-4850-ac22-02f8629fe72&title=)
<a name="wS94d"></a>
## 为什么需要OAuth2.0？
编码永远都是为了解决生产中的问题，想要理解为什么需要OAuth2，当然要从实际生活出发。<br />举个例子：小区的业主点了一份外卖，但是小区的门禁系统不给外卖人员进入，此时想要外卖员进入只能业主下来开门或者告知门禁的密码。<br />密码告知外卖员岂不是每次都能凭密码进入小区了，这明显造成了**安全隐患**。<br />**那么有没有一种方案：既能不泄露密码，也能让外卖小哥进入呢？**<br />于是此时就想到了一个授权机制，分为以下几个步骤：

1. 门禁系统中新增一个**授权按钮**，外卖小哥只需要点击授权按钮呼叫对应业主
2. 业主收到小哥的呼叫，知道小哥正在要求授权，于是做出了**应答授权**
3. 此时门禁系统弹出一个**密码**（类似于**access_token**），**有效期30分钟**，在30分钟内，小哥可以凭借这个密码进入小区。
4. 小哥输入密码进入小区

另外这个授权的密码不仅可以通过门禁，还可以通过楼下的门禁，这就非常类似于**网关**和**微服务**了。
<a name="G4N57"></a>
## 令牌和密码的区别？
上述例子中令牌和密码的作用是一样的，都可以进入小区，但是存在以下几点差异：

1. **时效不同**：令牌一般都是存在过期时间的，比如30分钟后失效，这个是无法修改的，除非重新申请授权；而密码一般都是永久的，除非主人去修改
2. **权限不同**：令牌的权限是有限的，比如上述例子中，小哥获取了令牌，能够打开小区的门禁、业主所在的楼下门禁，但是可能无法打开其它幢的门禁；
3. **令牌可以撤销**：业主可以撤销这个令牌的授权，一旦撤销了，这个令牌也就失效了，无法使用；但是密码一般不允许撤销。
<a name="ctSeh"></a>
## 什么是OAuth2？
OAuth 是一个开放标准，该标准允许用户让第三方应用访问该用户在某一网站上存储的私密**资源**（如头像、照片、视频等），而在这个过程中无需将**用户名**和**密码**提供给**第三方应用**。实现这一功能是通过提供一个令牌（**token**），而不是用户名和密码来访问他们存放在特定**服务提供者**的数据。<br />采用令牌（token）的方式可以让用户灵活的对第三方应用授权或者收回权限。<br />OAuth2 是 OAuth 协议的下一版本，但不向下兼容 **OAuth 1.0**。<br />传统的 Web 开发登录认证一般都是基于 **session** 的，但是在**前后端分离**的架构中继续使用 session 就会有许多不便，因为移动端（Android、iOS、微信小程序等）要么不支持 **cookie**（微信小程序），要么使用非常不便，对于这些问题，使用 **OAuth2** 认证都能解决。<br />对于大家而言，在互联网应用中最常见的 OAuth2 应该就是各种第三方登录了，例如 **QQ 授权登录**、**微信授权登录**、**微博授权登录**、**GitHub 授权登录**等等。
<a name="PCxua"></a>
## OAuth2.0的四种模式？
OAuth2.0协议一共支持 4 种不同的授权模式：

1. **授权码模式**：常见的第三方平台登录功能基本都是使用这种模式。
2. **简化模式**：简化模式是不需要客户端服务器参与，直接在浏览器中向授权服务器申请令牌（token），一般如果网站是纯静态页面则可以采用这种方式。
3. **密码模式**：密码模式是用户把用户名密码直接告诉客户端，客户端使用说这些信息向授权服务器申请令牌（token）。这需要用户对客户端高度信任，例如客户端应用和服务提供商就是同一家公司，自己做前后端分离登录就可以采用这种模式。
4. **客户端模式**：客户端模式是指客户端使用自己的名义而不是用户的名义向服务提供者申请授权，严格来说，客户端模式并不能算作 OAuth 协议要解决的问题的一种解决方案，但是，对于开发者而言，在一些前后端分离应用或者为移动端提供的认证授权服务器上使用这种模式还是非常方便的。
<a name="jJyxB"></a>
### 1、授权码模式
这种方式是最常用的流程，**安全性也最高**，它适用于那些有后端的 Web 应用。授权码通过前端传送，令牌则是储存在后端，而且所有与资源服务器的通信都在后端完成。这样的前后端分离，可以避免令牌泄漏。<br />令牌获取的流程如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139065-8aa5e4a9-d76f-4758-86b9-2d32737a17e5.webp#clientId=u5cd47139-847d-4&from=paste&id=u69b1b35c&originHeight=370&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u106fb531-238d-4f2d-b4d4-9492fb7f5b7&title=)<br />授权码模式<br />上图中涉及到两个角色，分别是客户端、认证中心，客户端负责拿令牌，认证中心负责发放令牌。<br />但是不是所有客户端都有权限请求令牌的，需要事先在认证中心申请，比如微信并不是所有网站都能直接接入，而是要去微信后台开通这个权限。<br />至少要提前向认证中心申请的几个参数如下：

1. **client_id**：客户端唯一id，认证中心颁发的唯一标识
2. **client_secret**：客户端的秘钥，相当于密码
3. **scope**：客户端的权限
4. **redirect_uri**：授权码模式使用的跳转uri，需要事先告知认证中心。

**1、请求授权码**<br />客户端需要向认证中心拿到授权码，比如第三方登录使用微信，扫一扫登录那一步就是向微信的认证中心获取授权码。<br />请求的url如下：<br />/oauth/authorize?client_id=&response_type=code&scope=&redirect_uri=

上述这个url中携带的几个参数如下：

- **client_id**：客户端的id，这个由认证中心分配，并不是所有的客户端都能随意接入认证中心
- **response_type**：固定值为**code**，表示要求返回授权码。
- **scope**：表示要求的授权范围，客户端的权限
- **redirect_uri**：跳转的uri，认证中心同意或者拒绝授权跳转的地址，如果同意会在uri后面携带一个code=xxx，这就是授权码

**2、返回授权码**<br />第1步请求之后，认证中心会要求登录、是否同意授权，用户同意授权之后直接跳转到redirect_uri（这个需要事先在认证中心申请配置），授权码会携带在这个地址后面，如下：<br />[http://xxxx?code=NMoj5y](http://xxxx?code=NMoj5y)<br />上述链接中的NMoj5y就是授权码了。<br />**3、请求令牌**<br />客户端拿到授权码之后，直接携带授权码发送请求给认证中心获取令牌，请求的url如下：<br />/oauth/token?<br /> client_id=&<br /> client_secret=&<br /> grant_type=authorization_code&<br /> code=NMoj5y&<br /> redirect_uri=

相同的参数同上，不同参数解析如下：

- **grant_type**：授权类型，授权码固定的值为**authorization_code**
- **code**：这个就是上一步获取的授权码

**4、返回令牌**<br />认证中心收到令牌请求之后，通过之后，会返回一段JSON数据，其中包含了令牌**access_token**，如下：
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
<a name="HztfJ"></a>
### 2、简化模式
这种模式不常用，主要针对那些无后台的系统，直接通过web跳转授权，流程如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407138911-77da52a4-3f3b-4417-a7b9-121f1952ad81.webp#clientId=u5cd47139-847d-4&from=paste&id=ube05c1ca&originHeight=370&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf8db5312-7077-493b-8085-bf5c87516ce&title=)<br />简化模式<br />这种方式把令牌直接传给前端，是很不安全的。因此，只能用于一些安全要求不高的场景，并且令牌的有效期必须非常短，通常就是会话期间（**session**）有效，浏览器关掉，令牌就失效了。<br />**1、请求令牌**<br />客户端直接请求令牌，请求的url如下：<br />/oauth/authorize?<br />  response_type=token&<br />  client_id=CLIENT_ID&<br />  redirect_uri=CALLBACK_URL&<br />  scope=<br />这个url正是授权码模式中获取授权码的url，各个参数解析如下：

- client_id：客户端的唯一Id
- response_type：简化模式的固定值为**token**
- scope：客户端的权限
- redirect_uri：跳转的uri，这里后面携带的直接是**令牌**，不是授权码了。

**2、返回令牌**<br />认证中心认证通过后，会跳转到**redirect_uri**，并且后面携带着令牌，链接如下：<br />[https://xxxx#token=NPmdj5](https://xxxx#token=NPmdj5)<br />`#token=NPmdj5`这一段后面携带的就是认证中心携带的，令牌为**NPmdj5**。
<a name="W4PNI"></a>
### 3、密码模式
密码模式也很简单，直接通过**用户名**、**密码**获取令牌，流程如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139109-10f4c569-0114-4045-be87-126e2f22d2f3.webp#clientId=u5cd47139-847d-4&from=paste&id=u7cfa2b91&originHeight=370&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua86cbcc3-8375-4e25-83bf-9848a344e7e&title=)<br />密码模式<br />**1、请求令牌**<br />认证中心要求客户端输入用户名、密码，认证成功则颁发令牌，请求的url如下：<br />/oauth/token?<br />  grant_type=password&<br />  username=&<br />  password=&<br />  client_id=&<br />  client_secret=<br />参数解析如下：

- `**grant_type**`：授权类型，密码模式固定值为password
- `**username**`：用户名
- `**password**`：密码
- `**client_id**`：客户端id
- `**client_secret**`：客户端的秘钥

**2、返回令牌**<br />上述认证通过，直接返回JSON数据，不需要跳转，如下：
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
**access_token**则是颁发的令牌，`refresh_token`是刷新令牌，一旦令牌失效则携带这个令牌进行刷新。
<a name="h8uod"></a>
### 4、客户端模式
适用于没有前端的命令行应用，即在命令行下请求令牌。<br />**这种方式给出的令牌，是针对第三方应用的，而不是针对用户的，即有可能多个用户共享同一个令牌。**<br />流程如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139131-4f60b50c-603d-4496-aafc-691aaa2b38ac.webp#clientId=u5cd47139-847d-4&from=paste&id=u1ef9ed9b&originHeight=370&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u75c0e7f7-191d-40f7-968c-bc8ce1a1a3c&title=)<br />客户端模式<br />**1、请求令牌**<br />请求的url为如下：<br />/oauth/token?<br />grant_type=client_credentials&<br />client_id=&<br />client_secret=<br />参数解析如下：

- `**grant_type**`：授权类型，客户端模式固定值为`**client_credentials**`
- `**client_id**`：客户端id
- `**client_secret**`：客户端秘钥

**2、返回令牌**<br />认证成功后直接返回令牌，格式为JSON数据，如下：
```json
{
    "access_token": "ACCESS_TOKEN",
    "token_type": "bearer",
    "expires_in": 7200,
    "scope": "all"
}
```
<a name="TIggs"></a>
## OAuth2.0的认证中心搭建
为了方便测试OAuth2的四种授权模式，这里为了方便测试，简单搭建一个认证中心，后续会逐渐完善。
<a name="faXi9"></a>
### 1、案例架构
新建一个oauth2-auth-server-in-memory模块作为认证中心，目录如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139772-14a78c75-aba4-41ef-8aa9-614d87ea383b.webp#clientId=u5cd47139-847d-4&from=paste&id=ue50a2348&originHeight=402&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u24054784-b775-4b18-87cf-7fed35e0b6d&title=)
<a name="aA90J"></a>
### 2、添加依赖
直接上Spring Security和OAuth2的依赖，如下：
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
<a name="xKPeK"></a>
### 3、Spring Security安全配置
SecurityConfig这个配置类中主要设置有4块内容，如下：<br />**1、加密方式**<br />采用`**BCryptPasswordEncoder**`加密，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139553-bfbecc29-8900-4546-b47b-95940d158fe9.webp#clientId=u5cd47139-847d-4&from=paste&id=uec95eb6c&originHeight=189&originWidth=419&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubb1e86c2-aa96-46f5-89d6-bb49703aa73&title=)<br />**2、配置用户**<br />这里为了方便测试，直接将用户信息存储在内存中，**后续完善**，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139912-41b7a05c-7420-46ec-bcf5-991740cb8dfa.webp#clientId=u5cd47139-847d-4&from=paste&id=u6bba8b88&originHeight=329&originWidth=838&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u86d135e2-5ff8-43e5-a784-6479c736f64&title=)<br />上述代码配置了两个用户，如下：

- 用户名admin，密码123，角色admin
- 用户名user，密码123，角色user

**3、注入认证管理器**`**AuthenticationManager**`<br />`AuthenticationManager`在**密码授权模式**下会用到，这里提前注入，如果用的不是密码模式，可以不注入，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139783-02901fc4-9e6f-4b5e-831d-b3103f88cff8.webp#clientId=u5cd47139-847d-4&from=paste&id=ufeb788cd&originHeight=229&originWidth=765&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udbe1b84e-c7ed-4713-8420-b5dd4850cc7&title=)<br />**4、配置安全拦截策略**<br />由于需要验证授权码模式，因此开启表单提交模式，所有url都需要认证，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407139990-7bd3d323-4e97-4dd4-973b-803bfb4eb2ca.webp#clientId=u5cd47139-847d-4&from=paste&id=u13993548&originHeight=333&originWidth=920&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7f4819e0-11f6-4609-9a90-9fcbb8e6b63&title=)
<a name="tXg4z"></a>
### 4、令牌存储策略配置
令牌支持多种方式存储，比如内存方式、**Redis**、**JWT**，比较常用的两种则是Redis、JWT。<br />这里暂时使用内存存储的方式，一旦服务器重启令牌将会失效。<br />代码如下：<br />![2021-12-02-09-17-51-910328.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638408071970-a1b59913-1669-4a9b-aeb2-344937ff8990.png#clientId=u5cd47139-847d-4&from=ui&id=u28e29db2&originHeight=410&originWidth=779&originalType=binary&ratio=1&rotation=0&showTitle=false&size=960213&status=done&style=none&taskId=u58757449-6cc5-4ad4-b188-131fbf9feb9&title=)
<a name="v29rd"></a>
### 5、OAuth2.0的配置类
不是所有配置类都可以作为**OAuth2**.0认证中心的配置类，需要满足以下两点：

1. 继承`**AuthorizationServerConfigurerAdapter**`
2. 标注 `**@EnableAuthorizationServer**` 注解

代码如下：<br />![2021-12-02-09-17-52-051273.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638408041094-7ebd20df-e299-43de-ad8c-a08f54c77fc9.png#clientId=u5cd47139-847d-4&from=ui&id=u5ea6a485&originHeight=243&originWidth=808&originalType=binary&ratio=1&rotation=0&showTitle=false&size=590301&status=done&style=none&taskId=uac45b0eb-f837-454d-b2fc-e0b0281dad7&title=)<br />`**AuthorizationServerConfigurerAdapter**`需要实现的三个方法如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407140530-90834212-dd7c-466b-928c-716f0b702ea7.webp#clientId=u5cd47139-847d-4&from=paste&id=u9dda75dc&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7cdb00d1-9e03-47e5-b2aa-ee50aa59803&title=)<br />下面便是围绕这三个方法进行OAuth2的详细配置。
<a name="scPbm"></a>
### 6、客户端配置
在介绍OAuth2.0 协议的时候介绍到，并不是所有的客户端都有权限向认证中心申请令牌的，首先认证中心要知道你是谁，你有什么资格？<br />因此一些必要的配置是要认证中心分配的，比如**客户端唯一Id**、**秘钥**、**权限**。<br />客户端配置的存储也支持多种方式，比如**内存**、**数据库**，对应的接口为：**org.springframework.security.oauth2.provider.ClientDetailsService**，接口如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407140639-63518736-b844-4d37-ad7b-c0a468767890.webp#clientId=u5cd47139-847d-4&from=paste&id=u239a0a81&originHeight=279&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaf0edeea-9dd5-4ca3-9594-1a8744a5aad&title=)<br />同样这里为了方便测试，依然是加载在内存中，**后续完善**，完整的配置如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407140712-a4c5b466-4293-48d9-9026-2c39e0edb1c5.webp#clientId=u5cd47139-847d-4&from=paste&id=ufb815962&originHeight=471&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf9b364c6-14a8-48b9-ad41-f6a50af5d26&title=)<br />几个重要参数说一下，如下：

- `.withClient("myjszl")`：指定客户端唯一ID为myjszl
- `.secret()`：指定秘钥，使用加密算法加密了，秘钥为123
- `.resourceIds("res1")`：给客户端分配的资源权限，对应的是资源服务，比如订单这个微服务就可以看成一个资源，作为客户端肯定不是所有资源都能访问。
- `authorizedGrantTypes()`：定义认证中心支持的授权类型，总共支持五种
   - 授权码模式：`**authorization_code**`
   - 密码模式：`**password**`
   - 客户端模式：`**client_credentials**`
   - 简化模式：`**implicit**`
   - 令牌刷新：`**refresh_token**`，这并不是**OAuth2**的模式，定义这个表示认证中心支持令牌刷新
- `scopes()`：定义客户端的权限，这里只是一个标识，资源服务可以根据这个权限进行鉴权。
- `autoApprove`：是否需要授权，设置为false则不需要用户点击确认授权直接返回授权码
- `redirectUris`：跳转的uri
<a name="oViNv"></a>
### 7、授权码服务配置
使用授权码模式必须配置一个授权码服务，用来颁布和删除授权码，当然授权码也支持多种方式存储，比如内存，数据库，这里暂时使用内存方式存储，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407140691-88597b25-d9b4-4592-8ae3-b7407d182347.webp#clientId=u5cd47139-847d-4&from=paste&id=u5690f86a&originHeight=271&originWidth=914&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4197438d-d2bd-4e26-b857-1f9dcf22b0a&title=)
<a name="WY1ox"></a>
### 8、令牌服务的配置
除了令牌的存储策略需要配置，还需要配置令牌的服务`AuthorizationServerTokenServices`用来创建、获取、刷新令牌，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407140914-bd9df153-0a70-4a3b-8006-48ac994cd7b8.webp#clientId=u5cd47139-847d-4&from=paste&id=u0f284815&originHeight=539&originWidth=977&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0728a732-009b-43a8-9095-6b2cd374b07&title=)
<a name="cPcB4"></a>
### 9、令牌访问端点的配置
目前这里仅仅配置了四个，分别如下：

- 配置了授权码模式所需要的服务，`**AuthorizationCodeServices**`
- 配置了密码模式所需要的`**AuthenticationManager**`
- 配置了令牌管理服务，`**AuthorizationServerTokenServices**`
- 配置`/oauth/token`申请令牌的uri只允许POST提交。

详细代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407141257-20bce4e9-ad85-420f-99be-32e8fcf6a8d6.webp#clientId=u5cd47139-847d-4&from=paste&id=uea315e39&originHeight=452&originWidth=972&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u88bd6870-95d4-46e1-b818-944daa463b8&title=)<br />spring Security框架默认的访问端点有如下6个：

- **/oauth/authorize**：获取授权码的端点
- **/oauth/token**：获取令牌端点。
- **/oauth/confifirm_access**：用户确认授权提交端点。
- **/oauth/error**：授权服务错误信息端点。
- **/oauth/check_token**：用于资源服务访问的令牌解析端点。
- **/oauth/token_key**：提供公有密匙的端点，如果使用JWT令牌的话。

当然如果业务要求需要改变这些默认的端点的url，也是可以修改的，`AuthorizationServerEndpointsConfigurer`有一个方法，如下：
```bash
public AuthorizationServerEndpointsConfigurer pathMapping(String defaultPath, String customPath)
```
**第一个参数**：需要替换的默认端点url<br />**第二个参数**：自定义的端点url
<a name="xeDEc"></a>
### 10、令牌访问安全约束配置
主要对一些端点的权限进行配置，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407141046-3bf263ca-0a2c-4562-852a-15d5ea055b96.webp#clientId=u5cd47139-847d-4&from=paste&id=uc4edf5db&originHeight=413&originWidth=842&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc0fae876-1c47-403c-8b1f-e54949f5f07&title=)
<a name="ivCpB"></a>
## OAuth2.0的资源服务搭建
客户端申请令牌的目的就是为了访问资源，当然这个资源也是分权限的，一个令牌不是所有资源都能访问的。<br />在认证中心搭建的第6步配置客户端详情的时候，一行代码.resourceIds("res1")则指定了能够访问的资源，可以配置多个，这里的**res1**则是唯一对应一个资源。
<a name="w5OCD"></a>
### 1、案例架构
新建一个oauth2-auth-resource-in-memory模块作为认证中心，目录如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407141296-071f8c80-44ae-48a0-9295-e119696ba8ff.webp#clientId=u5cd47139-847d-4&from=paste&id=ua7f5bd14&originHeight=333&originWidth=853&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue42d30a4-bee3-4aff-bd15-501af8fda96&title=)
<a name="O8ZQE"></a>
### 2、OAuth2.0的配置类
作为资源服务的配置类必须满足两个条件，如下：

- 标注注解`@EnableResourceServer`
- 继承`ResourceServerConfigurerAdapter`

代码如下：<br />![2021-12-02-09-17-53-750542.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638407979274-f1597cb5-0f98-4bb1-854c-b9ccb323708f.png#clientId=u5cd47139-847d-4&from=ui&id=uc0cefec8&originHeight=327&originWidth=1061&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1042931&status=done&style=none&taskId=u8601764b-47c7-45b3-b01d-b20a1a6508e&title=)
<a name="o7DwI"></a>
### 3、令牌校验服务配置
由于认证中心使用的令牌存储策略是在内存中的，因此服务端必须远程调用认证中心的校验令牌端点**/oauth/check_token**进行校验。<br />代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407141491-cfdc768e-828b-42d9-87df-5a09faa796e5.webp#clientId=u5cd47139-847d-4&from=paste&id=ua37e72de&originHeight=486&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a2340de-f08d-4f5f-ae61-bc09d2223fb&title=)<br />**注意**：远程校验令牌存在性能问题，但是后续使用JWT令牌则本地即可进行校验，不必远程校验了。
<a name="bHeoH"></a>
### 4、配置客户端唯一id和令牌校验服务
上文说到客户端有一个唯一标识，因此需要配置上，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407141659-cd99b368-d07d-4414-b59f-7255cb44f5f3.webp#clientId=u5cd47139-847d-4&from=paste&id=u357d4913&originHeight=322&originWidth=880&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uff68987d-932d-43de-a7c4-a8de6557990&title=)
<a name="lpzHU"></a>
### 5、配置security的安全机制
上文在认证中心的第6步配置客户端详情那里，有一行代码`.scopes("all")`则是指定了客户端的权限，资源服务可以根据这个scope进行url的拦截。<br />拦截方式如下：
```java
.access("#oauth2.hasScope('')")
```
详细配置代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407141978-e5446961-17c8-45f5-8912-ea1cfabce1db.webp#clientId=u5cd47139-847d-4&from=paste&id=uc1b60bad&originHeight=304&originWidth=1011&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u247397f1-9a68-415b-b526-434c8b9d24c&title=)<br />这里配置了所有路径都需要**all**的权限。
<a name="GeYuo"></a>
### 6、新建测试接口
新建了两个接口，如下：

- /hello：认证成功都可以访问
- /admin：只有具备**ROLE_admin**角色的用户才可以访问

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142015-d334dfb7-3fdb-4336-af94-fda959efcb0f.webp#clientId=u5cd47139-847d-4&from=paste&id=uf8ab37c6&originHeight=593&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub38f9434-2d3d-4f77-8e5e-3e5f4f2d265&title=)
<a name="aR8MS"></a>
## OAuth2.0的四种模式测试
下面结合认证中心、资源服务对OAuth2.0的四种服务进行测试。<br />启动上述搭建的认证中心和资源服务，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142238-21282786-b3e7-4841-ad41-d63d65e1a410.webp#clientId=u5cd47139-847d-4&from=paste&id=u97a6d86a&originHeight=134&originWidth=543&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9ec35c21-053a-43b9-bf69-97441db2296&title=)
<a name="v9Etd"></a>
### 授权码模式
<a name="mBLhg"></a>
#### 1、获取授权码
请求的url如下：<br />[http://localhost:2003/auth-server/oauth/authorize?client_id=myjszl&response_type=code&scope=all&redirect_uri=http://www.baidu.com](http://localhost:2003/auth-server/oauth/authorize?client_id=myjszl&response_type=code&scope=all&redirect_uri=http://www.baidu.com)<br />浏览器访问，security需要登录，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142192-dc375d90-cf04-40c8-9d39-816fa6f9a5ea.webp#clientId=u5cd47139-847d-4&from=paste&id=uc3a79ef0&originHeight=339&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc96612f8-c600-4808-bbc1-07acbde917c&title=)<br />输入用户名**user**，密码**123**，成功登录。<br />此时来到了**确认授权**的页面，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142171-ec2d0a99-9ccb-4bd6-a462-c5cc7480d0bc.webp#clientId=u5cd47139-847d-4&from=paste&id=ufcb0930f&originHeight=227&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua1f8654c-889c-4373-9bf6-dbf0b56a02b&title=)<br />选择Apporove、确认授权，成功跳转到了百度页面，并且携带了授权码，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142565-8a08ec81-28e9-4ffb-a0eb-09f8800c7619.webp#clientId=u5cd47139-847d-4&from=paste&id=u7e5a507d&originHeight=73&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1dfcb749-8a81-4fb8-a149-92b37421113&title=)<br />这里的**6yV2bF**就是获取到的授权码。
<a name="tGCzI"></a>
#### 2、获取token
[http://localhost:2003/auth-server/oauth/token?code=jvMH5U&client_id=myjszl&client_secret=123&redirect_uri=http://www.baidu.com&grant_type=authorization_code](http://localhost:2003/auth-server/oauth/token?code=jvMH5U&client_id=myjszl&client_secret=123&redirect_uri=http://www.baidu.com&grant_type=authorization_code)<br />**注意**：**/oauth/token**获取token的接口请求允许的方式要配置在授权服务器中，比如配置**POST**方式，代码如下：
```java
.allowedTokenEndpointRequestMethods(HttpMethod.POST)
```
POSTMAN请求如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142740-14e8310c-7388-4094-ae61-420ebd925024.webp#clientId=u5cd47139-847d-4&from=paste&id=uf46dbe8a&originHeight=549&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u236179a0-5f5b-4da8-91a6-5f69f62786d&title=)
<a name="dOnJM"></a>
#### 3、访问资源服务
拿着令牌访问资源服务的**/hello**接口，请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142883-20364a5f-4d4d-4b8d-a29e-65d7cf843d2f.webp#clientId=u5cd47139-847d-4&from=paste&id=u7244d0d3&originHeight=387&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2de5a34a-8edd-493d-8c23-537cb749899&title=)<br />请求头需要添加**Authorization**，并且值为**Bearer+" "+access_token**的形式。<br />**注意**：Bearer后面一定要跟一个空格。
<a name="hItfg"></a>
### 密码模式
密码模式比较简单，不用先获取授权码，直接使用**用户名**、**密码**获取token。<br />POSTMAN请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142564-909e14e0-5155-4802-bb0d-03a3337b4cef.webp#clientId=u5cd47139-847d-4&from=paste&id=u55197615&originHeight=537&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u351ec30c-c758-45af-ab64-1b06240cc79&title=)<br />PS：访问资源自己拿着获取到的令牌尝试下.....
<a name="nq8mo"></a>
### 简化模式
简化模式就很简单了，拿着客户端id就可以获取token，请求的url如下：<br />[http://localhost:2003/auth-server/oauth/authorize?response_type=token&client_id=myjszl&redirect_uri=http://www.baidu.com&scope=all](http://localhost:2003/auth-server/oauth/authorize?response_type=token&client_id=myjszl&redirect_uri=http://www.baidu.com&scope=all)<br />这个过程和获取授权码一样，需要**登录**，**同意授权**<br />最终跳转到百度，链接后面直接携带了令牌，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142727-543acb84-3edd-4975-abee-0190bd0dd252.webp#clientId=u5cd47139-847d-4&from=paste&id=uc8cf4a1c&originHeight=178&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u60893937-9500-491d-b616-e93ba69eb86&title=)<br />上图中的**0d5ecf06-b255-4272-b0fa-8e51dde2ce3e**则是获取的令牌。<br />PS：访问资源自己尝试下..........
<a name="wmZQR"></a>
### 客户端模式
请求的url如下：<br />[http://localhost:2003/auth-server/oauth/token?client_id=myjszl&client_secret=123&grant_type=client_credentials](http://localhost:2003/auth-server/oauth/token?client_id=myjszl&client_secret=123&grant_type=client_credentials)<br />POSTMAN请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142998-36c554b3-dd3f-40b9-aa92-aa209a6b19a6.webp#clientId=u5cd47139-847d-4&from=paste&id=u9b077b03&originHeight=552&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u74212c1c-5204-40f5-857b-d1f9c90ab92&title=)<br />PS：访问资源自己尝试下..........
<a name="SuFVS"></a>
## OAuth2.0 其他端点的测试
Spring Security OAuth2.0还提供了其他的端点，下面来逐一测试一下。
<a name="aWyfq"></a>
### 1、刷新令牌
OAuth2.0提供了令牌刷新机制，一旦**access_token**过期，客户端可以拿着**refresh_token**去请求认证中心进行令牌的续期。<br />请求的url如下：<br />[http://localhost:2003/auth-server/oauth/token?client_id=myjszl&client_secret=123&grant_type=refresh_token&refresh_token=](http://localhost:2003/auth-server/oauth/token?client_id=myjszl&client_secret=123&grant_type=refresh_token&refresh_token=)<br />POSTMAN请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407142875-12d1b32b-1d43-4ec5-aaab-cb829ad4419a.webp#clientId=u5cd47139-847d-4&from=paste&id=u5c258df2&originHeight=620&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u669f501f-be37-4db2-87eb-ba007b4347a&title=)
<a name="c00wr"></a>
### 2、校验令牌
OAuth2.0还提供了校验令牌的端点，请求的url如下：<br />[http://localhost:2003/auth-server/oauth/check_token?toke=](http://localhost:2003/auth-server/oauth/check_token?toke=)<br />POSTMAN请求如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638407143012-696f193f-6f41-4bb9-bd98-c114a1024022.webp#clientId=u5cd47139-847d-4&from=paste&id=u5e1d67d9&originHeight=605&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u37f1c09e-02e0-435c-a0bb-1322d8c4fde&title=)
<a name="Vk2Yu"></a>
## 总结
本文介绍了OAuth2.0协议原理、四种授权模式，并且搭建了认证授权中心、资源服务进行了四种模式的测试。
