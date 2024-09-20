Java SpringCloud Sentinel<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636172534022-71025f5e-2a98-423f-9189-363377848089.webp#clientId=uf41c23f8-84a8-4&from=paste&id=u988e2c09&originHeight=234&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u225dbdd4-3156-438d-a7d4-c2a5b6a6ffb)
<a name="QOSim"></a>
## 网关如何限流？
Spring Cloud Gateway本身自带的限流实现，过滤器是`RequestRateLimiterGatewayFilterFactory`，不过这种上不了台面的就不再介绍了，有兴趣的可以实现下。<br />从1.6.0版本开始，Sentinel提供了SpringCloud Gateway的适配模块，可以提供两种资源维度的限流：

- **route维度**：即在配置文件中配置的路由条目，资源名为对应的`routeId`，这种属于粗粒度的限流，一般是对某个微服务进行限流。
- **自定义API维度**：用户可以利用Sentinel提供的API来自定义一些API分组，这种属于细粒度的限流，针对某一类的uri进行匹配限流，可以跨多个微服务。

sentinel官方文档：[https://github.com/alibaba/Sentinel/wiki/%E7%BD%91%E5%85%B3%E9%99%90%E6%B5%81](https://github.com/alibaba/Sentinel/wiki/%E7%BD%91%E5%85%B3%E9%99%90%E6%B5%81)<br />Spring Cloud Gateway集成Sentinel实现很简单。
<a name="EN4tm"></a>
### 新建项目
新建一个gateway-sentinel模块，添加如下依赖：
```xml
<!--nacos注册中心-->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>

<!--spring cloud gateway-->
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-gateway</artifactId>
</dependency>

<!--    spring cloud gateway整合sentinel的依赖-->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-alibaba-sentinel-gateway</artifactId>
</dependency>

<!--    sentinel的依赖-->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-sentinel</artifactId>
</dependency>
```
:::danger
注意：这依然是一个网关服务，不要添加WEB的依赖
:::
<a name="dIgqA"></a>
### 配置文件
配置文件中主要指定以下三种配置：

- nacos的地址
- sentinel控制台的地址
- 网关路由的配置

配置如下：
```yaml
spring:
  cloud:
    ## 整合sentinel，配置sentinel控制台的地址
    sentinel:
      transport:
        ## 指定控制台的地址，默认端口8080
        dashboard: localhost:8080
    nacos:
      ## 注册中心配置
      discovery:
        # nacos的服务地址，nacos-server中IP地址:端口号
        server-addr: 127.0.0.1:8848
    gateway:
      ## 路由
      routes:
        ## id只要唯一即可，名称任意
        - id: gateway-provider
          uri: lb://gateway-provider
          ## 配置断言
          predicates:
            ## Path Route Predicate Factory断言，满足/gateway/provider/**这个请求路径的都会被路由到http://localhost:9024这个uri中
            - Path=/gateway/provider/**
```
上述配置中设置了一个路由gateway-provider，只要请求路径满足/gateway/provider/**都会被路由到gateway-provider这个服务中。
<a name="L4VK7"></a>
### 限流配置
经过上述两个步骤其实已经整合好了Sentinel，此时访问一下接口：http://localhost:9026/gateway/provider/port<br />然后在sentinel控制台可以看到已经被监控了，监控的路由是gateway-provider，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636172534249-d5b41cf0-c91a-402c-aaeb-582cf33636ca.webp#clientId=uf41c23f8-84a8-4&from=paste&id=u5704f358&originHeight=366&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ud23acdab-69ba-4307-a805-3feaaa170cf)<br />此时可以为其新增一个route维度的限流，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636172534345-ac523b1a-7eb5-4340-88d6-32dcc927094f.webp#clientId=uf41c23f8-84a8-4&from=paste&id=u62a2cd70&originHeight=619&originWidth=881&originalType=url&ratio=1&status=done&style=none&taskId=ud4dc04c8-121e-4574-9075-a3e7aa06a86)<br />上图中对gateway-provider这个路由做出了限流，QPS阈值为1。<br />此时快速访问：http://localhost:9026/gateway/provider/port，看到已经被限流了，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636172534352-fd885d42-98ea-401a-b6b8-58fc12dcd423.webp#clientId=uf41c23f8-84a8-4&from=paste&id=u9e8a66f9&originHeight=505&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u9fe3aa8b-0648-4759-9bcb-da805448d4f)<br />以上route维度的限流已经配置成功，小伙伴可以自己照着上述步骤尝试一下。<br />API分组限流也很简单，首先需要定义一个分组，**API管理-> 新增API分组**，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636172534309-bd543242-40a5-41cd-b751-c0ffc6fb63c7.webp#clientId=uf41c23f8-84a8-4&from=paste&id=u2999e06b&originHeight=358&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u1b82afe7-08df-41f3-b143-0e6621fd7a6)<br />匹配模式选择了精确匹配（还有前缀匹配，正则匹配），因此只有这个uri：http://xxxx/gateway/provider/port会被限流。<br />第二步需要对这个分组添加流控规则，**流控规则->新增网关流控**，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636172534624-28bdb917-40c6-441c-b067-a1d7caa50cef.webp#clientId=uf41c23f8-84a8-4&from=paste&id=u8f2a704b&originHeight=450&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u1fdd84af-1e73-4131-8b50-a794c528eb0)<br />API名称那里选择对应的分组即可，新增之后，限流规则就生效了。
<a name="je1Qw"></a>
## 如何自定义限流异常信息？
从上面的演示中可以看到默认的异常返回信息是："Block........."，这种肯定是客户端不能接受的，因此需要定制自己的异常返回信息。<br />下面介绍两种不同的方式定制异常返回信息，开发中自己选择其中一种。
<a name="rzxpm"></a>
### 直接配置文件中定制
开发者可以直接在配置文件中直接修改返回信息，配置如下：
```yaml
spring:
  cloud:
    ## 整合sentinel，配置sentinel控制台的地址
    sentinel:
      #配置限流之后，响应内容
      scg:
        fallback:
          ## 两种模式，一种是response返回文字提示信息，
          ## 一种是redirect，重定向跳转，需要同时配置redirect(跳转的uri)
          mode: response
          ## 响应的状态
          response-status: 200
          ## 响应体
          response-body: '{"code": 200,"message": "请求失败，稍后重试！"}'
```
上述配置中mode配置的是response，一旦被限流了，将会返回JSON串。
```json
{
    "code": 200,
    "message": "请求失败，稍后重试！"
}
```
**重定向**的配置如下：
```yaml
spring:
  cloud:
    ## 整合sentinel，配置sentinel控制台的地址
    sentinel:
      #配置限流之后，响应内容
      scg:
        fallback:
          ## 两种模式，一种是response返回文字提示信息，一种是redirect，重定向跳转，需要同时配置redirect(跳转的uri)
          mode: redirect
          ## 跳转的URL
          redirect: http://www.baidu.com
```
一旦被限流，将会直接跳转到：http://www.baidu.com
<a name="NIISk"></a>
### 编码定制
这种就不太灵活了，通过硬编码的方式，完整代码如下：
```java
@Configuration
public class GatewayConfig {
    /**
     * 自定义限流处理器
     */
    @PostConstruct
    public void initBlockHandlers() {
        BlockRequestHandler blockHandler = (serverWebExchange, throwable) -> {
            Map map = new HashMap();
            map.put("code",200);
            map.put("message","请求失败，稍后重试！");
            return ServerResponse.status(HttpStatus.OK)
                    .contentType(MediaType.APPLICATION_JSON_UTF8)
                    .body(BodyInserters.fromObject(map));
        };
        GatewayCallbackManager.setBlockHandler(blockHandler);
    }
}
```
两种方式介绍完了，根据业务需求自己选择适合的方式，当然第一种更受人喜欢，理由：**约定>配置>编码**。
<a name="O9iCN"></a>
## 网关限流了，服务就安全了吗？
很多人认为只要网关层面做了限流，躲在身后的服务就可以高枕无忧了，是不是也有这种想法？<br />很显然这种想法是错误的，复杂的微服务架构一个独立服务不仅仅被一方调用，往往是多方调用，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636172534906-eec3d7c7-82b3-45fa-a594-e03d0943cc7a.webp#clientId=uf41c23f8-84a8-4&from=paste&id=u92e43e71&originHeight=453&originWidth=766&originalType=url&ratio=1&status=done&style=shadow&taskId=u3ce53cb9-e140-4e5e-b84d-4bcfd736694)<br />商品服务不仅仅被网关层调用，还被内部订单服务调用，这时候仅仅在网关层限流，那么商品服务还安全吗？<br />一旦大量的请求订单服务，比如大促秒杀，商品服务不做限流会被瞬间击垮。<br />因此需要根据公司业务场景对自己负责的服务也要进行限流兜底，最常见的方案：**网关层集群限流+内部服务的单机限流兜底**，这样才能保证不被流量冲垮。
