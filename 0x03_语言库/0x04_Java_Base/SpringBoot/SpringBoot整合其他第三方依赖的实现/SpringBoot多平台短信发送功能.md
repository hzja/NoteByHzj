<a name="oBWw6"></a>
## 背景
开发的软件少不了短信这个功能，比如登录需要短信，通知需要短信，支付需要短信等等。<br />但是市面上短信渠道却是众多，比如腾讯云短信、阿里云短信等等，让人无从下手，应接不暇。<br />这时候多么希望有一个项目能够集成市面上所有的第三方短信功能，只需引入一个Jar包按需调用即可。<br />这款开源的项目：SMS4J -- 让发送短信变的更简单。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690777072536-eb86ef1c-8659-44b2-b8f0-8f012d49f540.png#averageHue=%23fefefe&clientId=ue30f86a5-6a6e-4&from=paste&id=ua6733938&originHeight=410&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u870d05be-cd08-4f5b-a3a9-5b6e890b514&title=)
<a name="TnH0q"></a>
## 介绍
SMS4J ，让发送短信变的更简单。<br />根据官网介绍，目前SMS4J 集成了多款短信通道，如：**腾讯云短信、阿里云短信、网易云短信、合一短信**等等。<br />并且目前SMS4J 拥有众多优秀的特点：

- **开箱即用：极简单的配置，方便的使用，三分钟即可完成发送短信的功能**
- **简单优雅：统一各个厂商的发送方式和功能，省去学习不同短信厂商的差异化方法，做到极简使用**
- **切换灵活：只需要通过配置文件即可立即切换不同的服务商，省去查阅文档和ctrl c v，发送短信，有手就行**
- **功能丰富：对于短信中常见的限制和规则，不需要编写额外的功能方法和模块，只需要开启配置即可，后续还将集成更多功能方便使用。**
<a name="ArGwG"></a>
## 快速使用
SMS4J 使用非常简单，支持多种环境集成，下面我们使用SpringBoot环境集成，具体文档可以参考如下地址。<br />官网地址：[https://wind.kim/](https://wind.kim/)
<a name="JSKrQ"></a>
### 1、引入依赖
```xml
<dependency>
  <groupId>org.dromara.sms4j</groupId>
  <artifactId>sms4j-spring-boot-starter</artifactId>
  <version> version </version>
</dependency>
```
<a name="q2bPn"></a>
### 2、配置配置文件
自 V2.0.0版本开始，sms4j开始支持多厂商共用，可以同时配置多个厂商使用，以下仅为yml形式的演示
```yaml
sms:
   alibaba:
      #阿里云的accessKey
      accessKeyId: 您的accessKey
      #阿里云的accessKeySecret
      accessKeySecret: 您的accessKeySecret
      #短信签名
      signature: 测试签名
      #模板ID 用于发送固定模板短信使用
      templateId: SMS_215125134
      #模板变量 上述模板的变量
      templateName: code
      #请求地址 默认为dysmsapi.aliyuncs.com 如无特殊改变可以不用设置
      requestUrl: dysmsapi.aliyuncs.com
   huawei:
      #华为短信appKey
      appKey: 5N6fvXXXX920HaWhVXXXXXX7fYa
      #华为短信appSecret
      app-secret: Wujt7EYzZTBXXXXXXEhSP6XXXX
      #短信签名
      signature: 华为短信测试
      #通道号
      sender: 8823040504797
      #模板ID 如果使用自定义模板发送方法可不设定
      template-id: acXXXXXXXXc274b2a8263479b954c1ab5
      #华为回调地址，如不需要可不设置或为空
      statusCallBack:
      #华为分配的app请求地址
      url: https://XXXXX.cn-north-4.XXXXXXXX.com:443
```
<a name="wFNC9"></a>
### 3、创建测试controller
为应对多厂商的不同实现自 V1.1.0版本开始不再使用springBoot注入对象的形式进行实现类的获取，改为工厂模式获取单例的实现对象
```java
@RestController
@RequestMapping("/test/")
public class DemoController {

    // 测试发送固定模板短信
    @RequestMapping("/")
    public void doLogin(String username, String password) {
        //阿里云向此手机号发送短信
        SmsFactory.createSmsBlend(SupplierType.ALIBABA).sendMessage("18888888888","123456");
        //华为短信向此手机号发送短信
        SmsFactory.createSmsBlend(SupplierType.HUAWEI).sendMessage("16666666666","000000");
    }
}
```
<a name="HNVIv"></a>
## 总结
总的来说SMS4J 功能丰富，基本集成了市面上所有的短信渠道，并且支持多环境渠道部署简单明了，如果有自定义渠道，也可以联系作者，让你的使用无后顾之忧<br />最后在贴上地址：<br />使用文档：[https://wind.kim/](https://wind.kim/)<br />gitee代码：[https://gitee.com/dromara/sms4j](https://gitee.com/dromara/sms4j)
