JavaSpringBoot<br />在项目中，为了保证数据的安全，常常会对传递的数据进行加密。常用的加密算法包括对称加密（AES）和非对称加密（RSA），这里使用码云上最简单的API加密项目进行整合实践：[https://gitee.com/isuperag/rsa-encrypt-body-spring-boot](https://gitee.com/isuperag/rsa-encrypt-body-spring-boot)
<a name="sc4Zr"></a>
## 项目介绍
该项目使用RSA加密方式对API接口返回的数据加密，让API数据更加安全。别人无法对提供的数据进行破解。Spring Boot接口加密，可以对返回值、参数值通过注解的方式自动加解密 。
<a name="NhdrA"></a>
## 什么是RSA加密
首先了解RSA加密<br />RSA加密是一种非对称加密。可以在不直接传递密钥的情况下，完成解密。这能够确保信息的安全性，避免了直接传递密钥所造成的被破解的风险。是由一对密钥来进行加解密的过程，分别称为公钥和私钥。两者之间有数学相关，该加密算法的原理就是对一极大整数做因数分解的困难性来保证安全性。通常个人保存私钥，公钥是公开的（可能同时多人持有）。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640429933-4b081b2e-eea4-4658-a54a-75d93ecbbc56.png#averageHue=%23eeecec&clientId=uba3705b7-ff37-4&from=paste&id=u92d462b2&originHeight=443&originWidth=822&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u46519056-2607-4353-8b7a-d03e1701d32&title=)<br />**举例子**<br />加密和签名都是为了安全性考虑，但略有不同。常有人问加密和签名是用私钥还是公钥？其实都是对加密和签名的作用有所混淆。简单的说，加密是为了防止信息被泄露，而签名是为了防止信息被篡改。这里举2个例子说明。<br />**「第一个场景：」**战场上，B要给A传递一条消息，内容为某一指令。<br />RSA的加密过程如下：

- A生成一对密钥（公钥和私钥），私钥不公开，A自己保留。公钥为公开的，任何人可以获取。
- A传递自己的公钥给B，B用A的公钥对消息进行加密。
- A接收到B加密的消息，利用A自己的私钥对消息进行解密。

在这个过程中，只有2次传递过程，第一次是A传递公钥给B，第二次是B传递加密消息给A，即使都被敌方截获，也没有危险性，因为只有A的私钥才能对消息进行解密，防止了消息内容的泄露。<br />**「第二个场景：」**A收到B发的消息后，需要进行回复“收到”。<br />RSA签名的过程如下：

- A生成一对密钥（公钥和私钥），私钥不公开，A自己保留。公钥为公开的，任何人可以获取。
- A用自己的私钥对消息加签，形成签名，并将加签的消息和消息本身一起传递给B。
- B收到消息后，在获取A的公钥进行验签，如果验签出来的内容与消息本身一致，证明消息是A回复的。

在这个过程中，只有2次传递过程，第一次是A传递加签的消息和消息本身给B，第二次是B获取A的公钥，即使都被敌方截获，也没有危险性，因为只有A的私钥才能对消息进行签名，即使知道了消息内容，也无法伪造带签名的回复给B，防止了消息内容的篡改。<br />但是，综合两个场景会发现，第一个场景虽然被截获的消息没有泄露，但是可以利用截获的公钥，将假指令进行加密，然后传递给A。第二个场景虽然截获的消息不能被篡改，但是消息的内容可以利用公钥验签来获得，并不能防止泄露。所以在实际应用中，要根据情况使用，也可以同时使用加密和签名，比如A和B都有一套自己的公钥和私钥，当A要给B发送消息时，先用B的公钥对消息加密，再对加密的消息使用A的私钥加签名，达到既不泄露也不被篡改，更能保证消息的安全性。
<a name="NVenN"></a>
## 加密实战
<a name="l9FWP"></a>
### 1、新建一个SpringBoot项目
springboot_api_encryption
<a name="IXMOC"></a>
### 2、引入Maven依赖
```xml
<dependency>
  <groupId>cn.shuibo</groupId>
  <artifactId>rsa-encrypt-body-spring-boot</artifactId>
  <version>1.0.1.RELEASE</version>
</dependency>
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640429782-e91af419-d8ab-4dc6-b5e1-f0c2f34453e7.png#averageHue=%23fcfbfa&clientId=uba3705b7-ff37-4&from=paste&id=Ipp5K&originHeight=450&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucb67d0ed-4737-44f2-9073-6611260f8e0&title=)
<a name="LqjRw"></a>
### 3、启动类Application中添加`@EnableSecurity`注解
```java
@SpringBootApplication
@EnableSecurity
public class DemoApplication {
    public static void main(String[] args) {
        SpringApplication.run(DemoApplication.class, args);
    }
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640429812-80bef9fc-136f-4e13-9ca4-0ce186ed4ef7.png#averageHue=%23f9f9f7&clientId=uba3705b7-ff37-4&from=paste&id=lCiXz&originHeight=440&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u64306369-cfd4-4b46-b1ce-68bcec0d280&title=)
<a name="DkrhK"></a>
### 4、在application.yml或者application.properties中添加RSA公钥及私钥
公钥私钥的生成文章后面会放出生成工具
```yaml
rsa:
  encrypt:
    open: false # 是否开启加密 true  or  false
    showLog: true # 是否打印加解密log true  or  false
    publicKey: # RSA公钥 软件生成
    privateKey: # RSA私钥 软件生成
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640429841-1101d77d-ca08-43e1-b503-0a6517fc4e64.png#averageHue=%23f8f7f6&clientId=uba3705b7-ff37-4&from=paste&id=rlbCH&originHeight=366&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u21d42817-d86a-4427-b6f7-320d6ffa747&title=)
<a name="O5YmO"></a>
### 5、对Controller 里面的API方法进行加密
创建一个实体类<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640429871-4fea3d60-f27f-42be-aff0-37d239d1be88.png#averageHue=%23fcfcfc&clientId=uba3705b7-ff37-4&from=paste&id=J7d79&originHeight=427&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5b31cc07-9310-4351-998a-732f1b4cbbf&title=)
```java
@Encrypt
@GetMapping("/encryption")
public TestBean encryption(){
    TestBean testBean = new TestBean();
    testBean.setName("试试");
    testBean.setAge(18);
    return testBean;
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640430449-1ca3eb12-30d7-41f5-9807-496b8715955f.png#averageHue=%23fbfbfa&clientId=uba3705b7-ff37-4&from=paste&id=ejFsa&originHeight=451&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf944fe26-c266-4495-b726-038984d6db4&title=)
<a name="aMtR2"></a>
### 6、对传过来的加密参数解密
其他Java端程序可以用注解，如果是Vue，请用RSA密钥解密
```java
@Decrypt
@PostMapping("/decryption")
public String Decryption(@RequestBody TestBean testBean){
    return testBean.toString();
}
```
<a name="sp7T9"></a>
### 7、启动项目
请求地址：http://localhost:8080/encryption<br />看到返回的数据未加密<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640430568-1ad974d0-4097-411c-8f03-dee58b19fa0c.png#averageHue=%23f7f7f6&clientId=uba3705b7-ff37-4&from=paste&id=ufca765af&originHeight=266&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub6eac9cf-7b32-4a5c-9b44-bd226f7cc5b&title=)
<a name="peEEs"></a>
### 8、修改
修改open为true 打开加密
```yaml
rsa:
  encrypt:
    open: true # 是否开启加密 true  or  false
    showLog: true # 是否打印加解密log true  or  false
    publicKey: # RSA公钥 软件生成
    privateKey: # RSA私钥 软件生成
```
<a name="CT1rv"></a>
### 9、再次重启项目
请求地址：http://localhost:8080/encryption<br />看到返回的数据已加密<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640430730-7a0a1c0d-cc93-4c96-870a-8411c574a45c.png#averageHue=%23f1f0ef&clientId=uba3705b7-ff37-4&from=paste&id=ub7d16a5a&originHeight=237&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucf9a8d18-f20c-4a58-b57e-020af00e4bf&title=)
<a name="LbFW8"></a>
### 10、加密日志
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640430890-4803a29c-e66a-424e-94c5-72ef9ab755e5.png#averageHue=%23f9f3f2&clientId=uba3705b7-ff37-4&from=paste&id=ucc5e3906&originHeight=230&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0fb8af52-7e40-4e40-892c-fa611eae561&title=)
<a name="dOhXL"></a>
## 解密实战
如果是其他SpringBoot项目，跟前面一样。这儿就当客户端是SpringBoot项目，其他的请使用RSA解密协议解密！<br />服务端有私密钥、公密钥，前端只需要公密钥就可以，在原来的SpringBoot基础上写一份解密方法
<a name="D4Rpw"></a>
### 1、前端js解密方法
```html
<script src="https://cdn.bootcdn.net/ajax/libs/jquery/3.5.1/jquery.js"></script>
<script src="https://cdn.bootcdn.net/ajax/libs/jsencrypt/3.0.0-rc.1/jsencrypt.js"></script>
```
<a name="EUpct"></a>
### 2、后台增加解密方法
```java
/**
 * 解密
 * @param user
 * @return
 */
@PostMapping("/decryption")
@Decrypt
@ResponseBody
public String Decryption(@RequestBody User user){
    System.out.println(user.toString());
    return user.toString();
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640431148-0fe32419-41c5-40c3-ae3f-d26b9693e574.png#averageHue=%23fbfbfa&clientId=uba3705b7-ff37-4&from=paste&id=DziDA&originHeight=515&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua45ad2f8-fe1e-4855-977c-add7cc05c8d&title=)
<a name="US2I5"></a>
### 3、js方法
```javascript
// 公钥
var PUBLIC_KEY = 'MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAobhGH4WMwMvJRUlTxWrCVIOQtsHijAxPJNvAWAgq80ADpFEWrpbcGB9cKqp6XHRH4k/CVtCUZ7jm9UKwhaeAm18sKtcwe+M8JFNX6FSHpgde0o8C9S/QpcmLxf4iN7nGZ7P3ZTvMdmKUcdRMsVQnsydG2Bj6gRxP2+kexEebTeODbdM7dHlkxAL0RxGWmX/ZOBzsoWZw2gKcC0vxwyIZBGHUdImG2T3nEA+VMfK2Yqv3uSYukmlKP+0mjfhrTtLFDuTV1VER9BfryBMvpQCxLO4pqgZnXPd+SOQcZHZ2OL0wqo5OX1+GPYx7TNxz5Qi76pK//T2mH7s6X/BuyT21HQIDAQAB';

/**
 * 加密方法
 * @returns {PromiseLike<ArrayBuffer>}
 * @constructor
 */
function RSA_encryption(jsonData) {
  var encrypt = new JSEncrypt();
  encrypt.setPublicKey('-----BEGIN PUBLIC KEY-----' + PUBLIC_KEY + '-----END PUBLIC KEY-----');
  var encrypted = encrypt.encrypt(JSON.stringify(jsonData));
  console.log('加密前数据:%o', str);
  console.log('加密后数据:%o', encrypted);
  return encrypted;
}


/**
 * 提交方法
 */
function tijiao() {
  var str = {
    "name":"1223334",
    "password":"asd",
    age:1
  };
  $.ajax({
    url: "/decryption",
    type : "POST",
    contentType: "application/json;charset=utf-8",
    data : RSA_encryption(str) ,
    success : function(data) {
      alert(data);
    }
  })
}
```
<a name="WwQkH"></a>
### 2、前端页面引入js以及方法
```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8">
    <title>Title</title>
  </head>
  <body>
    加密传后端，后端解密
    <button id="jiami" onclick="tijiao()">加密传后端</button>
    <script src="https://cdn.bootcdn.net/ajax/libs/jquery/3.5.1/jquery.js"></script>
    <script src="https://cdn.bootcdn.net/ajax/libs/jsencrypt/3.0.0-rc.1/jsencrypt.js"></script>
    <script>
      var PUBLIC_KEY = 'MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAobhGH4WMwMvJRUlTxWrCVIOQtsHijAxPJNvAWAgq80ADpFEWrpbcGB9cKqp6XHRH4k/CVtCUZ7jm9UKwhaeAm18sKtcwe+M8JFNX6FSHpgde0o8C9S/QpcmLxf4iN7nGZ7P3ZTvMdmKUcdRMsVQnsydG2Bj6gRxP2+kexEebTeODbdM7dHlkxAL0RxGWmX/ZOBzsoWZw2gKcC0vxwyIZBGHUdImG2T3nEA+VMfK2Yqv3uSYukmlKP+0mjfhrTtLFDuTV1VER9BfryBMvpQCxLO4pqgZnXPd+SOQcZHZ2OL0wqo5OX1+GPYx7TNxz5Qi76pK//T2mH7s6X/BuyT21HQIDAQAB';

      /**
       * 加密方法
       * @returns {PromiseLike<ArrayBuffer>}
       * @constructor
       */
      function RSA_encryption(jsonData) {
        var encrypt = new JSEncrypt();
        encrypt.setPublicKey('-----BEGIN PUBLIC KEY-----' + PUBLIC_KEY + '-----END PUBLIC KEY-----');
        var encrypted = encrypt.encrypt(JSON.stringify(jsonData));
        console.log('加密前数据:%o', jsonData);
        console.log('加密后数据:%o', encrypted);
        return encrypted;
      }

      /**
       * 提交方法
       */
      function tijiao() {
        var str = {
          "name":"1223334",
          "password":"asd",
          age:1
        };
        $.ajax({
          url: "/decryption",
          type : "POST",
          contentType: "application/json;charset=utf-8",
          data : RSA_encryption(str) ,
          success : function(data) {
            alert(data);
          }
        })
      }

    </script>
  </body>
</html>
```
<a name="TmiPA"></a>
### 3、启动访问
http://localhost:8080<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640431334-25187ad2-7e6d-488b-bd2f-f7aa53912ce8.png#averageHue=%23f9f6f4&clientId=uba3705b7-ff37-4&from=paste&id=u71d012ee&originHeight=583&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u30a0a8e2-5811-4c1d-bb69-a4059d5d4de&title=)
<a name="qh92l"></a>
### 4、后台解密日志
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1698640431510-4a1a3e8e-6eba-4490-b4d1-43a9593dac20.png#averageHue=%23f8f6f4&clientId=uba3705b7-ff37-4&from=paste&id=u626d8d99&originHeight=213&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9209d702-e588-4a17-a38c-2d49d182a72&title=)
<a name="HPhi6"></a>
## 总结
经过上面的接口加密解密操作。可以看出接口如果没有公钥、或者私钥别人根本无法解密！这样就对API接口起到了很好的保护作用，防止别人抓包！
<a name="iYltD"></a>
## 项目坑点
此项目的demo无法访问，难点就在前端如何加密回传到后台解密！<br />以下是注意事项：<br />1、注意ajax的 `contentType: "application/json;charset=utf-8"`
```javascript
$.ajax({
  url: "/decryption",
  type : "POST",
  contentType: "application/json;charset=utf-8",
  data : RSA_encryption(str) ,
  success : function(data) {
    alert(data);
  }
})
```
2、解密方法必须 `@RequestBody`
```java
@PostMapping("/decryption")
@Decrypt
@ResponseBody
public String Decryption(@RequestBody User user){
    System.out.println(user.toString());
    return user.toString();
}
```
