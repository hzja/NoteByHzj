<a name="onTeq"></a>
### 1、思维导图
![验证安全.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629447574005-683a95ea-10a8-4bd7-9671-bd4378b8b311.png#clientId=ua0b1d866-da99-4&from=paste&id=ubb1c4e0b&originHeight=506&originWidth=653&originalType=binary&ratio=1&size=30566&status=done&style=none&taskId=ub060fc82-cb13-440d-b844-95ce1ded865)<br />![逻辑漏洞.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629449127034-0c6062af-a516-48a0-bac1-faa624e2e40a.png#clientId=ua0b1d866-da99-4&from=paste&height=571&id=ue4981e02&originHeight=1142&originWidth=1228&originalType=binary&ratio=1&size=123754&status=done&style=none&taskId=u46b97d6b-e360-4830-b0ed-2599d326b64&width=614)
<a name="C8w1e"></a>
### 2、演示案例

- 验证码识别插件及工具操作演示-实例。
- 验证码绕过本地及远程验证-本地及实例
- Token客户端回显绕过登录爆破演示-
- 本地某URL下载接口ID值调用遍历测试-实例
- Callback自定义返回调用安全-漏洞测试-实例
- 补:上述在实战中如何做到漏洞发现-bp功能点


**验证码安全**

- 分类: 图片，手机或邮箱，语音，视频，操作等
- 原理: 验证生成或验证过程中的逻辑问题
- 危害: 账户权限泄漏，短信轰炸，遍历，任意用户操作等
- 漏洞: 客户端回显(已讲)，验证码复用，验证码爆破(已讲)，绕过等

**token 安全  **

- 基本上述同理，主要是验证中可存在绕过可继续后续测试 
- token 爆破，token 客户端回显等  

** 验证码识别插件工具使用  **

- captcha-killer
- Pkav_Http_Fuzz
- reCAPTCHA 等  

 **接口安全问题**

- 调用，遍历，未授权，篡改等 
- 调用案例：短信轰炸 
- 遍历案列：UID 等遍历 
- callback 回调 JSONP 
- 参数篡改：墨者靶场  

**水平越权**
```bash
未授权访问：
与越权漏洞区别
Jboss 未授权访问
Jenkins 未授权访问
ldap 未授权访问
Redis 未授权访问
elasticsearch 未授权访问
MenCache 未授权访问
Mongodb 未授权访问
Rsync 未授权访问
Zookeeper 未授权访问
Docker 未授权访问
```
<a name="aL4Tq"></a>
###  3、涉及资源：  
```bash
https://www.lanzous.com/i1z2s3e 或者 https://sec.kim/wp-content/uploads/2021/01/PKAV-HTTP-Fuzzer-1.5.6.zip

https://www.cnblogs.com/nul1/p/12071115.html

https://github.com/c0ny1/captcha-killer/releases/tag/0.1.2

https://github.com/bit4woo/reCAPTCHA/releases/tag/v1.0
```
<a name="cmtYS"></a>
### 4、工具操作演示
**①Pkav_Http_Fuzz工具**
```bash
https://www.lanzous.com/i1z2s3e 或者 https://sec.kim/wp-content/uploads/2021/01/PKAV-HTTP-Fuzzer-1.5.6.zip
```
[https://manage.yyxueche.com//panel/login.php](https://manage.yyxueche.com//panel/login.php)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629450762608-e92506d0-d015-4d9d-afc8-043b76b6a62f.png#clientId=ua0b1d866-da99-4&from=paste&id=ua1d99dd9&originHeight=842&originWidth=902&originalType=binary&ratio=1&size=183581&status=done&style=none&taskId=ud23c3ebd-cf2e-498a-b637-da79b06ada0)<br />获取到验证码地址：[https://manage.yyxueche.com/panel/verify_code_cn.php](https://manage.yyxueche.com/panel/verify_code_cn.php)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629451272769-ca23126d-7947-40ab-a4b1-c21fac630ad7.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=u762569f5&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=199778&status=done&style=none&taskId=ub26e2c80-e912-4e2b-9d53-05f931a5657&width=960)<br />优点:简单好用<br />缺点:不能配合burp使用

**captcha-killer**<br /> [https://github.com/c0ny1/captcha-killer/releases/tag/0.1.2](https://github.com/c0ny1/captcha-killer/releases/tag/0.1.2)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629452205020-0c876d7f-8e9a-4b35-988b-2e9a176f1d5f.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=ua40e2f98&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=124875&status=done&style=none&taskId=ua59822f0-ed6c-4daf-8599-3480c2b1f8e&width=960)<br />工具使用介绍：[https://www.cnblogs.com/nul1/p/12071115.html](https://www.cnblogs.com/nul1/p/12071115.html)<br />插件要配合百度AI文字识别使用。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629454120062-09d7724a-c847-47bc-99df-edbcfa1777e5.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=uf8c262de&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=149515&status=done&style=none&taskId=u0f0df9e6-7d8b-4764-b1f1-d1577f9a500&width=960)<br />由于穷后面的实验没得法继续做
<a name="IOuql"></a>
### 5、pikachu爆破
<a name="xEhrm"></a>
#### on server,
将数据包的用户名修改重复发送看回显的值是否发生变化。可以看出验证码存在复用的问题。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629455350540-15ce5f85-0be8-4119-95d2-8dca1ab7fecb.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=uf9b0569e&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=228672&status=done&style=none&taskId=u6860e09f-86e8-4568-a13d-d660116f8ad&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629455687902-50d97ba9-ed2e-418d-a400-3ff916b5fa27.png#clientId=ua0b1d866-da99-4&from=paste&height=358&id=u1ad3e337&originHeight=716&originWidth=1461&originalType=binary&ratio=1&size=122793&status=done&style=none&taskId=u67904057-55bb-4898-a947-78ddf50052c&width=730.5)<br />原因是没有销毁这个验证码。这个验证码存储在SESSION中。

<a name="P37U6"></a>
#### onclient
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629456175240-36cc7fd7-db54-4c17-b641-687b78105f44.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=uf8f955a4&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=192898&status=done&style=none&taskId=u798600c5-66a0-477e-a2fc-dab2e83acde&width=960)<br />在前端验证当中有的程序员会单独写一个js文件然后去加载``<script src="1.js"></script> 

**token**
```bash
1、Token的引入：Token是在客户端频繁向服务端请求数据，服务端频繁的去数据库查询用户名和密码并进行对比，判断用户名和密码正确与否，并作出相应提示，在这样的背景下，Token便应运而生。

2、Token的定义：Token是服务端生成的一串字符串，以作客户端进行请求的一个令牌，当第一次登录后，服务器生成一个Token便将此Token返回给客户端，以后客户端只需带上这个Token前来请求数据即可，无需再次带上用户名和密码。

3、使用Token的目的：Token的目的是为了减轻服务器的压力，减少频繁的查询数据库，使服务器更加健壮。

```
抓包发送到`intruder`模块当中<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629468567434-111964e6-ce58-4105-8999-2a437a2a28d9.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=u00169e7f&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=182358&status=done&style=none&taskId=u7ec72980-088d-4778-b8e3-d7f03016f57&width=960)<br />在表达式中输入`value='`然后点击`refetch response` 找到value后的token值复制，最后点击ok<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629468650978-b386ec49-3b3e-4185-a15a-c23957985af5.png#clientId=ua0b1d866-da99-4&from=paste&height=616&id=u0266f5f5&originHeight=877&originWidth=1079&originalType=binary&ratio=1&size=104470&status=done&style=none&taskId=u6bba96e6-c6da-4f1d-b13f-a0ec934bfed&width=757.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629468691036-743ab6b3-2909-42a6-ba17-6c77b583dbed.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=uc53b5ec8&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=101181&status=done&style=none&taskId=u88b6148c-3935-442e-9b27-4e4503d6d52&width=960)<br />设置线程数新版的burp和老版burp的差异，其实这里面还有很多的文章可以做，这里不展开叙述了。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629468820820-b9a334f0-e51c-426c-a955-6f7fa8e631a6.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=u066fc3e0&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=92263&status=done&style=none&taskId=ufeddd24f-f19a-4436-98ce-69f2b76da5f&width=960)<br />然后设置payloads<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629468923893-6d3f01a5-2ef3-48b1-b47c-7953e2a1ad34.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=u52b6fdb0&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=108288&status=done&style=none&taskId=ubc3e4b25-e4f4-4583-815b-afb13ae4933&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629468958865-88fbd084-5f1d-4310-a21e-3b4041da6239.png#clientId=ua0b1d866-da99-4&from=paste&height=521&id=u9d1496d4&originHeight=1042&originWidth=1920&originalType=binary&ratio=1&size=115219&status=done&style=none&taskId=ub53fe458-3309-4a1d-b39d-0ebe0831bee&width=960)<br />最后start，看到成功<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629468510248-e292fd53-306b-4c6c-a4c9-35e02d950e47.png#clientId=ua0b1d866-da99-4&from=paste&height=260&id=u3de240f3&originHeight=891&originWidth=1201&originalType=binary&ratio=1&size=103347&status=done&style=none&taskId=ub7bf04dd-174a-4f33-a185-7e606bc78e9&width=350)![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629469438126-a7e0aeec-8df7-48aa-951b-3832c31725b3.png#clientId=ua0b1d866-da99-4&from=paste&height=259&id=ua44384e9&originHeight=891&originWidth=1201&originalType=binary&ratio=1&size=74798&status=done&style=none&taskId=uf90bc1c0-71de-460c-8481-b88b25e9923&width=349.5)<br />总结：其实token的爆破还是非常麻烦的因为要爆破的东西非常多，如用户名，密码、token、要是字典比较大的话那么一时半会根本是爆破不完的。

由于之前对`Pitchfork`攻击类型不熟悉。在网上看了关于四种的攻击类型，作简要的陈述。

 <br />**一、Sniper（狙击手模式）**

针对单一密码，假设确定了两个位置A和B，然后密码包payload里有两个密码1、2，那么攻击模式如下：

| Attack No. | Position A | Position B |
| --- | --- | --- |
| 0 | 1 | null |
| 1 | 2 | null |
| 2 | null | 1 |
| 3 | null | 2 |

**二、Battering ram（攻城锤模式）**

于sniper模式不同的地方在于，同样情况下，攻击次数减半，每次两个位置用同样的密码，如表：

| Attack No. | Position A | Position B |
| --- | --- | --- |
| 0 | 1 | 1 |
| 1 | 2 | 2 |


---

**三、Pitchfork（叉子模式）**

跟前两种不同的地方在于，可以多组密码本payload，又于battering ram相同的地方在于，一一对应，现在添加包含3、4的密码本payload，暴力破解过程如表：

| Attack No. | Position A | Position B |
| --- | --- | --- |
| 0 | 1 | 3 |
| 1 | 2 | 4 |


---

**四、Cluster bomb（炸弹模式）**

跟叉子模式相似的是多个密码本对应多个位置，不同的是不再是一一对应，而是交叉组合，每一个密码本里的密码都对应于另一密码本所有密码，如表：

| Attack No. | Position A | Position B |
| --- | --- | --- |
| 0 | 1 | 3 |
| 1 | 2 | 3 |
| 2 | 1 | 4 |
| 3 | 2 | 4 |

<a name="kgwQY"></a>
## 接口回调安全问题
![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629470317750-5c54a99c-b59d-491c-b1ea-8069cf814fe0.png#clientId=ua0b1d866-da99-4&from=paste&id=u40fd8c2d&originHeight=316&originWidth=407&originalType=url&ratio=1&status=done&style=none&taskId=u48c59b48-7500-449a-9fbb-933a222d9a8)<br />callback容易产生跨站漏洞：inurl:callback<br />那么这些漏洞该怎么发现呢？可以使用bp爬虫
