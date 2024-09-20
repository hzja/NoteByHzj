Java OpenFeign
<a name="yTRdX"></a>
## 1、前言
介绍一款服务调用的组件：OpenFeign，同样是一款超越先辈（Ribbon、Feign）的狠角色。<br />目录如下：<br />![2021-10-12-12-25-23-916375.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012738083-22fe2177-cefe-4e5d-9416-5216636d506e.png#clientId=u09417671-babf-4&from=ui&id=u2fcf0d19&originHeight=460&originWidth=1015&originalType=binary&ratio=1&size=59366&status=done&style=shadow&taskId=u638a2dad-ffed-4f8b-924b-b04e30714f8)
<a name="D5VgR"></a>
## 2、Feign是什么？
Feign也是一个狠角色，Feign旨在使得Java Http客户端变得更容易。<br />Feign集成了Ribbon、RestTemplate实现了负载均衡的执行Http调用，只不过对原有的方式（Ribbon+RestTemplate）进行了封装，开发者不必手动使用RestTemplate调服务，而是定义一个接口，在这个接口中标注一个注解即可完成服务调用，这样更加符合面向接口编程的宗旨，简化了开发。<br />![2021-10-12-12-25-24-219063.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012738066-08ce89af-66ee-4540-a578-86ab3aee5241.png#clientId=u09417671-babf-4&from=ui&id=gZAfE&originHeight=323&originWidth=266&originalType=binary&ratio=1&size=5319&status=done&style=shadow&taskId=u055f210c-be96-4310-9ab4-dc70924207e)<br />但遗憾的是Feign现在停止迭代了，当然现在也是有不少企业在用。
<a name="MbukF"></a>
## 3、openFeign是什么？
前面介绍过停止迭代的Feign，简单点来说：OpenFeign是springcloud在Feign的基础上支持了SpringMVC的注解，如`@RequestMapping`等等。OpenFeign的`@FeignClient`可以解析SpringMVC的`@RequestMapping`注解下的接口，并通过动态代理的方式产生实现类，实现类中做负载均衡并调用其他服务。<br />官网地址：[https://docs.spring.io/spring-cloud-openfeign/docs/2.2.10.BUILD-SNAPSHOT/reference/html](https://docs.spring.io/spring-cloud-openfeign/docs/2.2.10.BUILD-SNAPSHOT/reference/html)
<a name="e8jEn"></a>
## 4、Feign和openFeign有什么区别？
| Feign | openFiegn |
| --- | --- |
| Feign是SpringCloud组件中一个轻量级RESTful的HTTP服务客户端，Feign内置了Ribbon，用来做客户端负载均衡，去调用服务注册中心的服务。Feign的使用方式是：使用Feign的注解定义接口，调用这个接口，就可以调用服务注册中心的服务 | OpenFeign 是SpringCloud在Feign的基础上支持了SpringMVC的注解，如`@RequestMapping`等。OpenFeign 的`@FeignClient`可以解析SpringMVC的`@RequestMapping`注解下的接口，并通过动态代理的方式产生实现类，实现类中做负载均衡并调用其他服务。 |

<a name="mJmCJ"></a>
## 5、环境准备
注册中心直接使用Nacos作为注册和配置中心。<br />项目结构如下图：<br />![2021-10-12-12-25-24-378348.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012738066-9b6f0657-9dd9-48e6-8f1d-b0dfd2b9cb2d.png#clientId=u09417671-babf-4&from=ui&id=DIFDH&originHeight=314&originWidth=733&originalType=binary&ratio=1&size=15722&status=done&style=shadow&taskId=u632e986e-399a-4e8a-a284-8620140e8b9)<br />注册中心使用Nacos，创建个微服务，分别为服务提供者Produce，服务消费者Consumer。
<a name="Xi9M1"></a>
## 6、创建服务提供者
既然是微服务之间的相互调用，那么一定会有服务提供者了，创建openFeign-provider9005，注册进入Nacos中，配置如下：
```yaml
server:
  port: 9005
spring:
  application:
    ## 指定服务名称，在nacos中的名字
    name: openFeign-provider
  cloud:
    nacos:
      discovery:
        # nacos的服务地址，nacos-server中IP地址:端口号
        server-addr: 127.0.0.1:8848
management:
  endpoints:
    web:
      exposure:
        ## yml文件中存在特殊字符，必须用单引号包含，否则启动报错
        include: '*'
```
注意：此处的`spring.application.name`指定的名称将会在openFeign接口调用中使用。
<a name="ewzP6"></a>
## 7、创建服务消费者
新建一个模块openFeign-consumer9006作为消费者服务，步骤如下。
<a name="XptIR"></a>
### 1、添加依赖
除了Nacos的注册中心的依赖，还要添加openFeign的依赖，如下：
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-openfeign</artifactId>
</dependency>
```
<a name="R8Ttz"></a>
### 2、添加注解`@EnableFeignClients`开启openFeign功能
老套路了，在Spring boot 主启动类上添加一个注解`@EnableFeignClients`，开启openFeign功能，如下：
```java
@SpringBootApplication
@EnableDiscoveryClient
@EnableFeignClients
public class OpenFeignConsumer9006Application{
    public static void main(String[] args) {
        SpringApplication.run(OpenFeignConsumer9006Application.class, args);
    }
}
```
<a name="c5qMZ"></a>
### 3、新建openFeign接口
新建一个openFeign接口，使用`@FeignClient`注解标注，如下：
```java
@FeignClient(value = "openFeign-provider")
public interface OpenFeignService {
}
```
注意：该注解`@FeignClient`中的`value`属性指定了服务提供者在nacos注册中心的服务名。
<a name="DzOtE"></a>
### 4、新建一个Controller调试
新建一个controller用来调试接口，直接调用openFeign的接口，如下：
```java
@RestController
@RequestMapping("/openfeign")
public class OpenFeignController {
    
}
```
好了，至此一个openFeign的微服务就搭建好了，并未实现具体的功能，下面一点点实现。
<a name="vtbLu"></a>
## 8、openFeign如何传参？
开发中接口传参的方式有很多，但是在openFeign中的传参是有一定规则的，下面详细介绍。
<a name="fQgwa"></a>
### 1、传递JSON数据
这个也是接口开发中常用的传参规则，在Spring Boot 中通过`@RequestBody`标识入参。<br />provider接口中JSON传参方法如下：
```java
@RestController
@RequestMapping("/openfeign/provider")
public class OpenFeignProviderController {
    @PostMapping("/order2")
    public Order createOrder2(@RequestBody Order order){
        return order;
    }
}
```
consumer中openFeign接口中传参代码如下：
```java
@FeignClient(value = "openFeign-provider")
public interface OpenFeignService {
    /**
     * 参数默认是@RequestBody标注的，这里的@RequestBody可以不填
     * 方法名称任意
     */
    @PostMapping("/openfeign/provider/order2")
    Order createOrder2(@RequestBody Order order);
}
```
注意：openFeign默认的传参方式就是JSON传参（`@RequestBody`），因此定义接口的时候可以不用`@RequestBody`注解标注，不过为了规范，一般都填上。
<a name="KPxHt"></a>
### 2、POJO表单传参
这种传参方式也是比较常用，参数使用POJO对象接收。<br />provider服务提供者代码如下：
```java
@RestController
@RequestMapping("/openfeign/provider")
public class OpenFeignProviderController {
    @PostMapping("/order1")
    public Order createOrder1(Order order){
        return order;
    }
}
```
consumer消费者openFeign代码如下：
```java
@FeignClient(value = "openFeign-provider")
public interface OpenFeignService {
    /**
     * 参数默认是@RequestBody标注的，如果通过POJO表单传参的，使用@SpringQueryMap标注
     */
    @PostMapping("/openfeign/provider/order1")
    Order createOrder1(@SpringQueryMap Order order);
}
```
网上很多人疑惑POJO表单方式如何传参，官方文档明确给出了解决方案，如下：<br />![2021-10-12-12-25-24-570938.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012797269-275fb77b-27d1-4ffe-a764-3bb2db53c8a5.png#clientId=u09417671-babf-4&from=ui&id=u01121f16&originHeight=761&originWidth=1047&originalType=binary&ratio=1&size=70060&status=done&style=shadow&taskId=u98e7a9f3-fdf7-4817-9595-d53cc5898b7)<br />openFeign提供了一个注解`@SpringQueryMap`完美解决POJO表单传参。
<a name="rD8f0"></a>
### 3、URL中携带参数
此种方式针对restful方式中的GET请求，也是比较常用请求方式。<br />provider服务提供者代码如下：
```java
@RestController
@RequestMapping("/openfeign/provider")
public class OpenFeignProviderController {

    @GetMapping("/test/{id}")
    public String test(@PathVariable("id")Integer id){
        return "accept one msg id="+id;
}
```
consumer消费者openFeign接口如下：
```java
@FeignClient(value = "openFeign-provider")
public interface OpenFeignService {

    @GetMapping("/openfeign/provider/test/{id}")
    String get(@PathVariable("id")Integer id);
}
```
使用注解`@PathVariable`接收url中的占位符，这种方式很好理解。
<a name="gvuEx"></a>
### 4、普通表单参数
此种方式传参不建议使用，但是也有很多开发在用。<br />provider服务提供者代码如下：
```java
@RestController
@RequestMapping("/openfeign/provider")
public class OpenFeignProviderController {
    @PostMapping("/test2")
    public String test2(String id,String name){
        return MessageFormat.format("accept on msg id={0}，name={1}",id,name);
    }
}
```
consumer消费者openFeign接口传参如下：
```java
@FeignClient(value = "openFeign-provider")
public interface OpenFeignService {
    /**
     * 必须要@RequestParam注解标注，且value属性必须填上参数名
     * 方法参数名可以任意，但是@RequestParam注解中的value属性必须和provider中的参数名相同
     */
    @PostMapping("/openfeign/provider/test2")
    String test(@RequestParam("id") String arg1,@RequestParam("name") String arg2);
}
```
<a name="UZypU"></a>
### 5、总结
传参的方式有很多，比如文件传参....这里只是列举了四种常见得传参方式。
<a name="DZLVU"></a>
## 9、超时如何处理？
想要理解超时处理，先看一个例子：将provider服务接口睡眠3秒钟，接口如下：
```java
@PostMapping("/test2")
public String test2(String id,String name) throws InterruptedException {
        Thread.sleep(3000);
        return MessageFormat.format("accept on msg id={0}，name={1}",id,name);
}
```
此时，调用consumer的openFeign接口返回结果如下图：<br />![2021-10-12-12-25-25-141000.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012797520-cf6d54fe-4b3e-42af-aad1-6d76e7c9e104.png#clientId=u09417671-babf-4&from=ui&id=PMaIz&originHeight=665&originWidth=1080&originalType=binary&ratio=1&size=228110&status=done&style=shadow&taskId=ue19444a7-f6d3-4f15-bf73-cc530563c30)<br />很明显的看出程序异常了，返回了接口调用超时。what？why？...........<br />openFeign其实是有默认的超时时间的，默认分别是连接超时时间10秒、读超时时间60秒，源码在`feign.Request.Options#Options()`这个方法中，如下图：<br />![2021-10-12-12-25-25-446453.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012822308-fbd50123-ab6b-407e-a903-8cada6f135ea.png#clientId=u09417671-babf-4&from=ui&id=uc655685e&originHeight=299&originWidth=994&originalType=binary&ratio=1&size=136206&status=done&style=none&taskId=u2d662c49-d8f6-4de7-bd0d-460e337837d)<br />那么问题来了：为什么只设置了睡眠3秒就报超时呢？<br />其实openFeign集成了Ribbon，Ribbon的默认超时连接时间、读超时时间都是是1秒，源码在`org.springframework.cloud.openfeign.ribbon.FeignLoadBalancer#execute()`方法中，如下图：<br />![2021-10-12-12-25-25-653885.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012822356-1de7e085-ba1e-419e-bc49-32fd73d1a61a.png#clientId=u09417671-babf-4&from=ui&id=IgkbG&originHeight=410&originWidth=948&originalType=binary&ratio=1&size=194393&status=done&style=none&taskId=u57dca94a-34a5-49ea-9b50-f595e9bf7a0)<br />源码大致意思：如果openFeign没有设置对应得超时时间，那么将会采用Ribbon的默认超时时间。<br />理解了超时设置的原理，由之产生两种方案也是很明了了，如下：

- 设置openFeign的超时时间
- 设置Ribbon的超时时间
<a name="oj4iU"></a>
### 1、设置Ribbon的超时时间（不推荐）
设置很简单，在配置文件中添加如下设置：
```yaml
ribbon:
  # 值的是建立链接所用的时间，适用于网络状况正常的情况下， 两端链接所用的时间
  ReadTimeout: 5000
  # 指的是建立链接后从服务器读取可用资源所用的时间
  ConectTimeout: 5000
```
<a name="a0OM9"></a>
### 2、设置openFeign的超时时间（推荐）
openFeign设置超时时间非常简单，只需要在配置文件中配置，如下：
```yaml
feign:
  client:
    config:
      ## default 设置的全局超时时间，指定服务名称可以设置单个服务的超时时间
      default:
        connectTimeout: 5000
        readTimeout: 5000
```
default设置的是全局超时时间，对所有的openFeign接口服务都生效<br />但是正常的业务逻辑中可能涉及到多个openFeign接口的调用，如下图：<br />![2021-10-12-12-25-25-837171.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012838262-b97cab29-48e9-49fc-9990-0b588567e195.png#clientId=u09417671-babf-4&from=ui&id=u7d23ac80&originHeight=381&originWidth=501&originalType=binary&ratio=1&size=18983&status=done&style=shadow&taskId=u0608a591-8b90-4659-b933-c4a9c64b412)<br />上图中的伪代码如下：
```java
public T invoke(){
    //1. 调用serviceA
    serviceA();
    
    //2. 调用serviceA
    serviceB();
    
    //3. 调用serviceA
    serviceC();
}
```
那么上面配置的全局超时时间能不能通过呢？很显然是serviceA、serviceB能够成功调用，但是serviceC并不能成功执行，肯定报超时。<br />此时可以给serviceC这个服务单独配置一个超时时间，配置如下：
```yaml
feign:
  client:
    config:
      ## default 设置的全局超时时间，指定服务名称可以设置单个服务的超时时间
      default:
        connectTimeout: 5000
        readTimeout: 5000
      ## 为serviceC这个服务单独配置超时时间
      serviceC:
        connectTimeout: 30000
        readTimeout: 30000
```
注意：单个配置的超时时间将会覆盖全局配置。
<a name="VoyGD"></a>
## 10、如何开启日志增强？
openFeign虽然提供了日志增强功能，但是默认是不显示任何日志的，不过开发者在调试阶段可以自己配置日志的级别。<br />openFeign的日志级别如下：

- `NONE`：默认的，不显示任何日志;
- `BASIC`：仅记录请求方法、URL、响应状态码及执行时间;
- `HEADERS`：除了BASIC中定义的信息之外，还有请求和响应的头信息;
- `FULL`：除了HEADERS中定义的信息之外，还有请求和响应的正文及元数据。

配置起来也很简单，步骤如下：
<a name="JNrFG"></a>
### 1、配置类中配置日志级别
需要自定义一个配置类，在其中设置日志级别，如下：<br />![2021-10-12-12-25-26-256438.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012838418-a946536c-fd06-4efc-ab2f-a23084da5268.png#clientId=u09417671-babf-4&from=ui&id=eMdPQ&originHeight=430&originWidth=718&originalType=binary&ratio=1&size=129702&status=done&style=none&taskId=u941cde3c-7be1-4394-a4c4-669de52cda9)<br />注意：这里的logger是feign包里的。
<a name="iqa3i"></a>
### 2、yaml文件中设置接口日志级别
只需要在配置文件中调整指定包或者openFeign的接口日志级别，如下：
```yaml
logging:
  level:
    cn.fcant.service: debug
```
这里的cn.fcant.service是openFeign接口所在的包名，当然也可以配置一个特定的openFeign接口。
<a name="t6KOP"></a>
### 3、演示效果
上述步骤将日志设置成了FULL，此时发出请求，日志效果如下图：<br />![2021-10-12-12-25-27-029016.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012863421-99e8b16f-13f2-4127-b426-9065b691d0f0.png#clientId=u09417671-babf-4&from=ui&id=u491fa36c&originHeight=362&originWidth=1080&originalType=binary&ratio=1&size=352897&status=done&style=none&taskId=uef6b6016-8d3a-4705-88ee-7bcf9cd6a75)<br />日志中详细的打印出了请求头、请求体的内容。
<a name="gXS9M"></a>
## 11、如何替换默认的httpclient？
Feign在默认情况下使用的是JDK原生的URLConnection发送HTTP请求，没有连接池，但是对每个地址会保持一个长连接，即利用HTTP的persistence connection。<br />在生产环境中，通常不使用默认的http client，通常有如下两种选择：

- 使用`ApacheHttpClient`
- 使用`OkHttp`

至于哪个更好，其实各有千秋，比较倾向于`ApacheHttpClient`，毕竟老牌子了，稳定性不在话下。<br />那么如何替换掉呢？其实很简单，下面演示使用`ApacheHttpClient`替换。
<a name="YhxZJ"></a>
### 1、添加`ApacheHttpClient`依赖
在openFeign接口服务的pom文件添加如下依赖：
```xml
<!--     使用Apache HttpClient替换Feign原生httpclient-->
<dependency>
  <groupId>org.apache.httpcomponents</groupId>
  <artifactId>httpclient</artifactId>
</dependency>

<dependency>
  <groupId>io.github.openfeign</groupId>
  <artifactId>feign-httpclient</artifactId>
</dependency>
```
为什么要添加上面的依赖呢？从源码中不难看出，请看`org.springframework.cloud.openfeign.FeignAutoConfiguration.HttpClientFeignConfiguration`这个类，代码如下：<br />![2021-10-12-12-25-27-400271.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012863441-3786beb9-857c-4260-810b-b362ef268cf0.png#clientId=u09417671-babf-4&from=ui&id=vRvc8&originHeight=570&originWidth=1080&originalType=binary&ratio=1&size=346609&status=done&style=none&taskId=u5578d3a3-d94a-4ee6-bf6e-8d5bb594257)<br />上述红色框中的生成条件，其中的`@ConditionalOnClass(ApacheHttpClient.class)`，必须要有`ApacheHttpClient`这个类才会生效，并且`feign.httpclient.enabled`这个配置要设置为`true`。
<a name="SpLY7"></a>
### 2、配置文件中开启
在配置文件中要配置开启，代码如下：
```yaml
feign:
  client:
    httpclient:
      # 开启 Http Client
      enabled: true
```
<a name="EhpOt"></a>
### 3、如何验证已经替换成功？
其实很简单，在`feign.SynchronousMethodHandler#executeAndDecode()`这个方法中可以清楚的看出调用哪个client，如下图：<br />![2021-10-12-12-25-27-758285.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012863463-3c5685ca-490d-47e8-ab41-6ba00425e317.png#clientId=u09417671-babf-4&from=ui&id=mwSKt&originHeight=550&originWidth=1080&originalType=binary&ratio=1&size=293331&status=done&style=none&taskId=u4b91aa21-cd3d-4e23-ba48-85bb8182873)<br />上图中可以看到最终调用的是ApacheHttpClient。
<a name="KUFFn"></a>
### 4、总结
上述步骤仅仅演示一种替换方案，剩下的一种不再演示了，原理相同。
<a name="CQnSW"></a>
## 12、如何通讯优化？
在讲如何优化之前先来看一下GZIP 压缩算法，概念如下：<br />gzip是一种数据格式，采用用deflate算法压缩数据；gzip是一种流行的数据压缩算法，应用十分广泛，尤其是在Linux平台。<br />当GZIP压缩到一个纯文本数据时，效果是非常明显的，大约可以减少70％以上的数据大小。<br />网络数据经过压缩后实际上降低了网络传输的字节数，最明显的好处就是可以加快网页加载的速度。网页加载速度加快的好处不言而喻，除了节省流量，改善用户的浏览体验外，另一个潜在的好处是GZIP与搜索引擎的抓取工具有着更好的关系。例如 Google就可以通过直接读取GZIP文件来比普通手工抓取更快地检索网页。<br />GZIP压缩传输的原理如下图：<br />![2021-10-12-12-25-28-062018.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012890768-a7bc70f0-844d-46ff-a0df-3e0fa4c666a6.png#clientId=u09417671-babf-4&from=ui&id=uc42dd2ef&originHeight=108&originWidth=554&originalType=binary&ratio=1&size=3184&status=done&style=shadow&taskId=u7d7ac25b-6650-4ca1-a072-3495660a757)<br />按照上图拆解出的步骤如下：

- 客户端向服务器请求头中带有：`Accept-Encoding:gzip,deflate` 字段，向服务器表示，客户端支持的压缩格式（gzip或者deflate)，如果不发送该消息头，服务器是不会压缩的。
- 服务端在收到请求之后，如果发现请求头中含有`Accept-Encoding`字段，并且支持该类型的压缩，就对响应报文压缩之后返回给客户端，并且携带`Content-Encoding:gzip`消息头，表示响应报文是根据该格式压缩过的。
- 客户端接收到响应之后，先判断是否有Content-Encoding消息头，如果有，按该格式解压报文。否则按正常报文处理。

openFeign支持请求/响应开启GZIP压缩，整体的流程如下图：<br />![2021-10-12-12-25-28-215022.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012890754-4c48c9e1-6fb5-4768-9a6b-50a2a9a6febf.png#clientId=u09417671-babf-4&from=ui&id=zUTPu&originHeight=115&originWidth=554&originalType=binary&ratio=1&size=8660&status=done&style=shadow&taskId=ue8bde2c2-0242-4ab9-8d3b-c829de0187c)<br />上图中涉及到GZIP传输的只有两块，分别是Application client -> Application Service、 Application Service->Application client。<br />注意：openFeign支持的GZIP仅仅是在openFeign接口的请求和响应，即是openFeign消费者调用服务提供者的接口。<br />openFeign开启GZIP步骤也是很简单，只需要在配置文件中开启如下配置：
```yaml
feign:
  ## 开启压缩
  compression:
    request:
      enabled: true
      ## 开启压缩的阈值，单位字节，默认2048，即是2k，这里为了演示效果设置成10字节
      min-request-size: 10
      mime-types: text/xml,application/xml,application/json
    response:
      enabled: true
```
上述配置完成之后，发出请求，可以清楚看到请求头中已经携带了GZIP压缩，如下图：<br />![2021-10-12-12-25-28-827018.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012891033-413ce8e4-f45d-489e-ad31-29db02798254.png#clientId=u09417671-babf-4&from=ui&id=Ei2q8&originHeight=393&originWidth=1080&originalType=binary&ratio=1&size=299137&status=done&style=none&taskId=u15b48194-6793-4595-b534-7cd9fe2707d)
<a name="pIlSW"></a>
## 13、如何熔断降级？
常见的熔断降级框架有Hystrix、Sentinel，openFeign默认支持的就是Hystrix，这个在官方文档上就有体现。<br />但是阿里的Sentinel无论是功能特性、简单易上手等各方面都完全秒杀Hystrix，因此就使用openFeign+Sentinel进行整合实现服务降级。
<a name="rHsdl"></a>
### 1、添加Sentinel依赖
在openFeign-consumer9006消费者的pom文件添加sentinel依赖（由于使用了聚合模块，不指定版本号），如下：
```xml
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-sentinel</artifactId>
</dependency>
```
<a name="GxvJT"></a>
### 2、配置文件中开启sentinel熔断降级
要想openFeign使用sentinel的降级功能，还需要在配置文件中开启，添加如下配置：
```yaml
feign:
  sentinel:
    enabled: true
```
<a name="gB0Hb"></a>
### 3、添加降级回调类
这个类一定要和openFeign接口实现同一个类，如下图：<br />![2021-10-12-12-25-29-330051.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012916139-e18077ee-701b-4419-b630-0ce9f9cd704e.png#clientId=u09417671-babf-4&from=ui&id=u0ac286ec&originHeight=198&originWidth=1080&originalType=binary&ratio=1&size=126467&status=done&style=none&taskId=u281971d2-c58c-4cee-b493-90e24f536c8)<br />`OpenFeignFallbackService`这个是降级回调的类，一旦`OpenFeignService`中对应得接口出现了异常则会调用这个类中对应得方法进行降级处理。
<a name="g1aiY"></a>
### 4、添加fallback属性
在`@FeignClient`中添加`fallback`属性，属性值是降级回调的类，如下：
```java
@FeignClient(value = "openFeign-provider",fallback = OpenFeignFallbackService.class)
public interface OpenFeignService {}
```
<a name="QpVGL"></a>
### 5、演示
经过如上4个步骤，openFeign的熔断降级已经设置完成了，此时演示下效果。<br />通过postman调用http://localhost:9006/openfeign/order3这个接口，正常逻辑返回如下图：<br />![2021-10-12-12-25-29-687021.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012916103-b18c97bc-b6ca-41f0-9d7e-64a218bac869.png#clientId=u09417671-babf-4&from=ui&id=DamKY&originHeight=550&originWidth=1080&originalType=binary&ratio=1&size=79552&status=done&style=shadow&taskId=u06b6ad5d-e756-46b2-ab2b-f4ef8aba4c5)<br />现在手动造个异常，在服务提供的接口中抛出异常，如下图：<br />![2021-10-12-12-25-30-069017.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012916093-e983eaaf-1fbe-413a-8bfc-7ee6efc238bd.png#clientId=u09417671-babf-4&from=ui&id=fFYoV&originHeight=225&originWidth=929&originalType=binary&ratio=1&size=91618&status=done&style=none&taskId=uc3e62690-1126-4c91-8f96-1bd42310bf5)<br />此时重新调用http://localhost:9006/openfeign/order3，返回如下图：<br />![2021-10-12-12-25-30-582051.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634012934989-42011875-d873-4665-b043-dd5ea27f4ac9.png#clientId=u09417671-babf-4&from=ui&id=ua2f466b3&originHeight=500&originWidth=1080&originalType=binary&ratio=1&size=71237&status=done&style=shadow&taskId=u2c786ccd-b6d0-45b3-85dc-7947299733d)<br />可以很清楚的看到服务已经成功降级调用。
