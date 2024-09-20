<a name="Oz53d"></a>
## 1、相关链接
<a name="X8HbG"></a>
### A》官网地址
[http://forest.dtflyx.com/](http://forest.dtflyx.com/)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629944153907-616cb222-e071-4730-af7a-47cb2b9302de.png#clientId=ua365da1c-d46e-4&from=paste&height=492&id=uf3cce62a&originHeight=1476&originWidth=3798&originalType=binary&ratio=1&size=317671&status=done&style=none&taskId=u5b07bfa0-56bc-49f7-8bea-d3c22c985e6&width=1266)
<a name="ev4Nc"></a>
### B》Github仓库地址
[https://github.com/dromara/forest](https://github.com/dromara/forest)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629944262203-b09d59e0-9673-4b67-ac92-39e62eb53cf8.png#clientId=ua365da1c-d46e-4&from=paste&height=338&id=uc4e9a1b7&originHeight=1014&originWidth=2260&originalType=binary&ratio=1&size=171518&status=done&style=none&taskId=uf99561be-0a2a-4aeb-ab9b-c14365a40d6&width=753.3333333333334)
<a name="g954a"></a>
### C》Gitee仓库地址
[https://gitee.com/dt_flys/forest](https://gitee.com/dt_flys/forest)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629943830020-4ac728ae-b85f-46a7-ba3b-b95430aa6a59.png#clientId=u3c074127-4bc6-4&from=paste&height=340&id=u066f24c4&originHeight=1021&originWidth=1585&originalType=binary&ratio=1&size=145512&status=done&style=shadow&taskId=u0ad28029-4a16-428a-9c41-6deb35adfbc&width=528.3333333333334)
<a name="xeuiR"></a>
## 2、使用场景
对于调用不同公司接口的API，需要对应不同的HTTP请求，这些服务商都提供基于http的api。但是每家公司提供api具体细节差别很大。有的基于`RESTFUL`规范，有的基于传统的http规范；有的需要在`header`里放置签名，有的需要`SSL`的双向认证，有的只需要`SSL`的单向认证；有的以`JSON` 方式进行序列化，有的以`XML`方式进行序列化。类似于这样细节的差别太多了。<br />不同的公司API规范不一样，这很正常。但如果想要代码变得优雅。就必须解决一个痛点：<br />不同服务商API那么多的差异点，如何才能维护一套不涉及业务的公共http调用套件。最好通过配置或者简单的参数就能区分开来。进行方便的调用？<br />有很多优秀的大名鼎鼎的http开源框架可以实现任何形式的http调用，在多年的开发经验中都有使用过。比如apache的httpClient包，非常优秀的Okhttp，jersey client。<br />这些http开源框架的接口使用相对来说，都不太一样。不管选哪个，在这个场景里来说，都不希望在调用每个第三方的http api时写上一堆http调用代码。所以，在这个场景里，得对每种不同的http api进行封装。这样的代码才能更加优雅，业务代码和http调用逻辑耦合度更低。<br />Forest是一款优秀的开源http框架，能屏蔽不同细节http api所带来的所有差异。能通过简单的配置像调用rpc框架一样的去完成极为复杂的http调用。
<a name="Yck6G"></a>
## 3、Forest的特点
Forest 底层封装了2种不同的http框架：Apache httpClient和OKhttp。所以这个开源框架并没有对底层实现进行重复造轮子，而是在易用性上面下足了功夫。<br />Forest作为一款更加高层的http框架，其实并不需要写很多代码，大多数时候，仅通过一些配置就能完成http的本地化调用。而这个框架所能覆盖的面，却非常之广，满足绝大多数的http调用请求。

- 以`Httpclient`和`OkHttp`为后端框架
- 通过调用本地方法的方式去发送Http请求, 实现了业务逻辑与Http协议之间的解耦
- 相比Feign更轻量，不依赖`Spring Cloud`和任何注册中心
- 支持所有请求方法：`GET`, `HEAD`, `OPTIONS`, `TRACE`, `POST`, `DELETE`, `PUT`, `PATCH`
- 支持灵活的模板表达式
- 支持过滤器来过滤传入的数据
- 基于注解、配置化的方式定义`Http`请求
- 支持`Spring`和`Springboot`集成
- 实现`JSON`和`XML`的序列化和反序列化
- 支持JSON转换框架: `Fastjson`,`Jackson`, `Gson`
- 支持`JAXB`形式的`XML`转换
- 支持`SSL`的单向和双向加密
- 支持http连接池的设定
- 可以通过`OnSuccess`和`OnError`接口参数实现请求结果的回调
- 配置简单，一般只需要`@Request`一个注解就能完成绝大多数请求的定义
- 支持异步请求调用
<a name="PgM80"></a>
## 4、简单使用
Forest支持了Springboot的自动装配，所以只需要引入一个依赖就行
```xml
<dependency>
  <groupId>com.dtflys.forest</groupId>
  <artifactId>spring-boot-starter-forest</artifactId>
  <version>1.3.0</version>
</dependency>
```
定义自己的接口类
```java
public interface MyClient {

    @Request(url = "http://baidu.com")
    String simpleRequest();

    @Request(
            url = "http://ditu.amap.com/service/regeo",
            dataType = "json"
    )
    Map getLocation(@DataParam("longitude") String longitude, @DataParam("latitude") String latitude);
  
}
```
在启动类里配置代理接口类的扫描包
```java
@SpringBootApplication
@ForestScan(basePackages = "com.example.demo.forest")
public class DemoApplication {
    public static void main(String[] args) {
        SpringApplication.run(DemoApplication.class, args);
    }
}
```
这时候，就可以从Spring容器中注入代理接口，像调用本地方法一样去调用http的api了
```java
@Autowired
private MyClient myClient;

@Override
public void yourMethod throws Exception {
    Map result = myClient.getLocation("124.730329","31.463683");
    System.out.println(JSON.toJSONString(result,true));
}
```
日志打印，Forest打印了内部所用的http框架，和实际请求url和返回。当然日志可以通过配置去控制开关。<br />![2021-08-26-10-20-13-345834.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1629944425282-21b96a4b-1821-448c-ac9c-43fd616efe2f.jpeg#clientId=ua365da1c-d46e-4&from=ui&id=u2f493fe0&originHeight=497&originWidth=800&originalType=binary&ratio=1&size=29148&status=done&style=none&taskId=uc85ec282-beff-4d28-95c7-54f93104f9d)
<a name="EyqHW"></a>
## 5、两个很棒的功能
<a name="cpvwC"></a>
### 模板表达式和参数的映射绑定功能

---

模板表达式在使用的时候特别方便，举个栗子
```java
@Request(
    url = "${0}/send?un=${1}&pw=${2}&ph=${3}&ct=${4}",
    type = "get",
    dataType = "json"
)
public Map send(
    String base,
    String userName,
    String password,
    String phone,
    String content
);
```
上述是用序号下标进行取值，也可以通过名字进行取值：
```java
@Request(
    url = "${base}/send?un=${un}&pw=${pw}&ph=${3}&ct=${ct}",
    type = "get",
    dataType = "json"
)
public Map send(
    @DataVariable("base") String base,
    @DataVariable("un") String userName,
    @DataVariable("pw") String password,
    @DataVariable("ph") String phone,
    @DataVariable("ct") String content
);
```
甚至于可以这样简化写：
```java
@Request(
    url = "${base}/send",
    type = "get",
    dataType = "json"
)
public Map send(
    @DataVariable("base") String base,
    @DataParam("un") String userName,
    @DataParam("pw") String password,
    @DataParam("ph") String phone,
    @DataParam("ct") String content
);
```
以上三种写法是等价的<br />当然也可以把参数绑定到header和body里去，甚至于可以用一些表达式简单的把对象序列化成json或者xml：
```java
@Request(
    url = "${base}/pay",
    contentType = "application/json",
    type = "post",
    dataType = "json",
    headers = {"Authorization: ${1}"},
    data = "${json($0)}"
)
public PayResponse pay(PayRequest request, String auth);
```
当然数据绑定这块详情请参阅文档
<a name="YDJGA"></a>
### 对HTTPS的支持
以前用其他http框架处理https的时候，总觉得特别麻烦，尤其是双向证书。每次碰到问题也只能去baidu。然后根据别人的经验来修改自己的代码。<br />Forest对于这方面也想的很周到，底层完美封装了对https单双向证书的支持。也是只要通过简单的配置就能迅速完成。举个双向证书栗子：
```java
@Request(
    url = "${base}/pay",
    contentType = "application/json",
    type = "post",
    dataType = "json",
    keyStore = "pay-keystore",
    data = "${json($0)}"
)
public PayResponse pay(PayRequest request);
```
其中pay-keystore对应着application.yml里的ssl-key-stores
```yaml
forest:
  ...
  ssl-key-stores:
    - id: pay-keystore
      file: test.keystore
      keystore-pass: 123456
      cert-pass: 123456
      protocols: SSLv3
```
这样设置就ok了，剩下的就是本地代码形式的调用了。
<a name="EFwNO"></a>
## 6、最后
Forest有很多其他的功能设定，很多人一定会说，这不就是Feign吗？<br />在开发Spring Cloud项目的时候，也用过一段时间Feign，Forest的确在配置和用法上和Feign的设计很像，但Feign的角色更多是作为Spring Cloud生态里的一个成员。充当RPC通信的角色，其承担的不仅是http通讯，还要对注册中心下发的调用地址进行负载均衡。<br />而Forest这个开源项目其定位则是一个高阶的http工具，主打友好和易用性。从使用角度出发，Forest配置性更加简单直接。提供的很多功能也能解决很多人的痛点。
