Java SpringCloud GateWay
<a name="P9SP6"></a>
## 1、Gateway 简介
<a name="W7JhM"></a>
### GateWay是什么
Spring Cloud 全家桶中有个很重要的组件：网关。在 1.x 版本中使用的是 Zuul 网关，但是到了 2.x，由于Zuul的升级不断跳票，Spring Cloud 自己研发了一套网关组件：Spring Cloud Gateway。<br />Spring Cloud Gateway基于 Spring Boot 2.x，Spring WebFlux 和 Project Reactor 构建，使用了 Webflux 中的 reactor-netty 响应式编程组件，底层使用了 Netty 通讯框架。
<a name="fr8BA"></a>
### GateWay能做什么
反向代理、鉴权、流量控制、熔断、日志监控......
<a name="fqifY"></a>
### 网关在微服务架构中的位置
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250246942-ffa22b7b-ff8a-4a73-90c1-9c4de7e46a54.webp#clientId=uff77cd05-ef7e-4&from=paste&id=u3342825c&originHeight=570&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ub0907bb9-175b-4d45-90ed-df9d241801a)
<a name="pqIkA"></a>
## 2、Gateway 的三大概念
Route（路由）：路由是构建网关的基本模块，它由 ID、目标 URI、一系列的断言和过滤器组成，如果断言为 true 则匹配该路由<br />Predicate（断言）：参考的是 Java8 中的 java.util.function.Predicate。开发人员可以匹配 HTTP 请求中的所有内容（例如请求头或请求参数），如果请求与断言相匹配则进行路由<br />Filter（过滤）：指的是 Spring 框架中 GatewayFilter 的实例，使用过滤器，可以在请求被路由之前或之后对请求进行修改<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250247101-2e2ac01d-962a-4e3e-9061-3acf17ccf66c.webp#clientId=uff77cd05-ef7e-4&from=paste&id=u80586110&originHeight=508&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=uc6cd0c6d-5e03-4792-ac12-0527c4bf598)
<a name="Xr08T"></a>
## 3、工作流程
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250246969-0fd05332-efde-49f3-b892-1b57a0685b3b.webp#clientId=uff77cd05-ef7e-4&from=paste&id=u252a854d&originHeight=773&originWidth=604&originalType=url&ratio=3&status=done&style=shadow&taskId=u2500e05b-ad3c-44d2-bda0-658f194135e)
> Clients make requests to Spring Cloud Gateway. If the Gateway Handler Mapping determines that a request matches a route, it is sent to the Gateway Web Handler. This handler runs the request through a filter chain that is specific to the request. The reason the filters are divided by the dotted line is that filters can run logic both before and after the proxy request is sent. All “pre” filter logic is executed. Then the proxy request is made. After the proxy request is made, the “post” filter logic is run.

翻译：客户端向 Spring Cloud Gateway 发出请求。如果网关处理程序映射确定请求与路由匹配，则将其发送到网关 Web 处理程序。该处理程序通过特定于请求的过滤器链来运行请求。 筛选器由虚线分隔的原因是，筛选器可以在发送代理请求之前和之后运行逻辑。所有 “前置“ 过滤器逻辑均被执行，然后发出代理请求，发出代理请求后，将运行“ 后置 ”过滤器逻辑。<br />总结：路由转发 + 执行过滤器链
<a name="p5Ysb"></a>
## 4、两种配置方式
<a name="uvFSv"></a>
### 配置文件方式
以访问「百度新闻网」为例，添加如下配置
```yaml
server:
  port: 9527
spring:
  application:
    name: cloud-gateway9527
  cloud:
    gateway:
      routes:
        - id: news                        # 路由id
          uri: http://news.baidu.com    # 真实调用地址
          predicates:
            - Path=/guonei                # 断言，符合规则进行路由
```
浏览器虽然输入 localhost:9527/guonei，却会转发到指定的地址<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250246854-9145db85-7a60-43df-be8a-8c899421e13b.webp#clientId=uff77cd05-ef7e-4&from=paste&id=uc74d3c9e&originHeight=466&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u4277bfbc-2729-4206-a310-76a1394bff8)
<a name="TeNGZ"></a>
### 编码方式
新增配置文件
```java
@Configuration
public class GatewayConfig {
    @Bean
    public RouteLocator routes(RouteLocatorBuilder builder) {
        return builder.routes()
                .route("news2", r -> r.path("/guoji").uri("http://news.baidu.com"))
                .build();
    }
}
```
效果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250247135-92224ca8-7e47-4f99-b582-cdbcc957b88e.webp#clientId=uff77cd05-ef7e-4&from=paste&id=ud7fe2b94&originHeight=435&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ub1b34285-c9a7-451c-a36a-1a625a8a3c3)
<a name="fiRwL"></a>
## 5、动态路由
开启后，默认情况下 Gateway 会根据注册中心注册的服务列表，以注册中心上微服务名为路径创建动态路由进行转发，从而实现动态路由的功能
```yaml
spring:
  cloud:
    gateway:
      discovery:
        locator:
          enabled: true #开启从注册中心动态创建路由的功能，利用微服务名进行路由
      routes:
        - id: payment_routh1
          #uri: http://localhost:8001     #静态，写死了地址，只能调用一个服务
          uri: lb://CLOUD-PAYMENT-SERVICE #动态，lb://微服务名
          predicates:
            - Path=/payment/get/**
        - id: payment_routh2
          #uri: http://localhost:8001
          uri: lb://CLOUD-PAYMENT-SERVICE
          predicates:
            - Path=/payment/lb/**
```
<a name="GvyPH"></a>
## 6、Predicate 的使用
<a name="tCLj7"></a>
### 时间相关配置
After：在指定时间之后进行路由<br />Before：在指定时间之前进行路由<br />Between：在指定时间之间进行路由
```yaml
predicates:
    - Path=/payment/lb/**
    #- After=2020-04-25T16:30:58.215+08:00[Asia/Shanghai]
    #- Before=2020-04-25T16:40:58.215+08:00[Asia/Shanghai]
    - Between=2020-04-25T16:35:58.215+08:00[Asia/Shanghai],2020-04-25T16:40:58.215+08:00[Asia/Shanghai]
```
上述配置的时间格式可以通过以下代码得到
```java
@Test
public void test(){
    ZonedDateTime now = ZonedDateTime.now();
    System.out.println(now);
}
```
<a name="y0Ipq"></a>
### 请求相关配置
<a name="HUXlM"></a>
#### Cookie
配置说明：【Cookie=cookie名, cookie值的正则表达式规则】
```yaml
predicates:
  - Path=/payment/lb/**
  - Cookie=id, [0-9
```
使用 curl 工具模拟携带 cookie 发送请求<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250248248-f5bed25e-2119-4b7c-a10e-ebe917d7943d.webp#clientId=uff77cd05-ef7e-4&from=paste&id=ud31e5744&originHeight=229&originWidth=990&originalType=url&ratio=3&status=done&style=none&taskId=u673fda60-ec18-4f70-9004-8df422f6c73)
<a name="nmMhT"></a>
#### Header
配置说明：【Header=header名, header值的正则表达式规则】
```yaml
predicates:
  - Path=/payment/lb/**
  - Header=h, [a-h]
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250248008-58b9be56-c2e4-4937-a30a-f0927e01f2ef.webp#clientId=uff77cd05-ef7e-4&from=paste&id=u802335ff&originHeight=151&originWidth=673&originalType=url&ratio=3&status=done&style=none&taskId=u0cea4d95-2f08-42e2-bcf5-378f12f1da3)
<a name="v2SwS"></a>
#### Host
配置说明：【Host=主机名（可配置多个，也可以使用通配符）】
```yaml
predicates:
  - Path=/payment/lb/**
  - Host=**.a.com,**.b.cn
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250247970-356ffbbf-dc8f-4d46-8a87-c91b86cc99e5.webp#clientId=uff77cd05-ef7e-4&from=paste&id=u2ba8f476&originHeight=146&originWidth=776&originalType=url&ratio=3&status=done&style=none&taskId=uaa1fae88-be93-4460-b2b5-23a4c1ad399)
<a name="qSmDW"></a>
#### Method
配置说明：【Method=请求类型】
```yaml
predicates:
  - Path=/payment/lb/**
  - Method=GET
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623250247923-2830eae4-3561-480e-94ff-64d77ff73bac.png#clientId=uff77cd05-ef7e-4&from=paste&id=u654baa9e&originHeight=99&originWidth=602&originalType=url&ratio=3&status=done&style=none&taskId=ueef1bc2d-869f-43eb-bbfe-8323c74d8df)
<a name="Lg6S0"></a>
#### Path
配置说明：【Path=请求路径】
```yaml
predicates:
  - Path=/payment/lb/**
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623250247801-2d04dc0f-235d-4a02-85dc-721c46667b38.png#clientId=uff77cd05-ef7e-4&from=paste&id=u3eeb3517&originHeight=99&originWidth=602&originalType=url&ratio=3&status=done&style=none&taskId=u1ac181f8-727e-4a42-85bd-9971b534034)
<a name="UtSTr"></a>
#### Query
配置说明：【Query=参数名，参数值】
```yaml
predicates:
  - Path=/payment/lb/**
  - Query=name, zhangsan
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250248252-166af19e-925e-473a-bcb9-7618cfa8e023.webp#clientId=uff77cd05-ef7e-4&from=paste&id=u5c14ed5b&originHeight=94&originWidth=695&originalType=url&ratio=3&status=done&style=none&taskId=ue7adff3d-a123-4e76-94fe-54c73152748)<br />详见：官网
<a name="LIZW9"></a>
## 7、Filter 的使用

- 生命周期：pre、post
- 种类：`GatewayFilter`、`GlobalFilter`

GatewayFilter 在官方文档有几十种！详细配置可参考 官网，这里主要介绍自定义全局过滤器。
```java
@Component
@Slf4j
public class MyGlobalFilter implements GlobalFilter, Ordered {
    @Override
    public Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain) {
        String username = exchange.getRequest().getQueryParams().getFirst("username");
        //用户名为空时，给出错误响应
        if (username == null) {
            log.info("用户名为空，非法登录");
            exchange.getResponse().setStatusCode(HttpStatus.NOT_ACCEPTABLE);
            return exchange.getResponse().setComplete();
        }
        return chain.filter(exchange);
    }

    @Override
    public int getOrder() {
        return 0;
    }
}
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250248569-38068e90-a03f-447f-9c7e-a25b7d96b12d.webp#clientId=uff77cd05-ef7e-4&from=paste&id=ua5430cc6&originHeight=223&originWidth=887&originalType=url&ratio=3&status=done&style=shadow&taskId=u4289b3b4-8bbb-4020-ae7e-f5c887e9fbb)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623250248703-88ccffa8-0a9b-4be1-acfb-cb2e7c4368cb.webp#clientId=uff77cd05-ef7e-4&from=paste&id=u8a4e4a9c&originHeight=435&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ud81fa358-1ce1-4b44-8b76-43225233767)
