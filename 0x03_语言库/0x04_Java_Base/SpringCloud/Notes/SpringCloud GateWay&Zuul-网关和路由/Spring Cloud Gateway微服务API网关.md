JavaSpringCloudGateway
<a name="Q2h5w"></a>
## 1、网关介绍
如果没有网关，难道不行吗？功能上是可以的，直接调用提供的接口就可以了。那为什么还需要网关？<br />因为网关的作用不仅仅是转发请求而已。可以试想一下，如果需要做一个请求认证功能，可以接入到 API 服务中。但是倘若后续又有服务需要接入，又需要重复接入。这样不仅代码要重复编写，而且后期也不利于维护。<br />由于接入网关后，网关将转发请求。所以在这一层做请求认证，天然合适。这样这需要编写一次代码，在这一层过滤完毕，再转发给下面的 API。<br />所以 API 网关的通常作用是完成一些通用的功能，如请求认证，请求记录，请求限流，黑白名单判断等。<br />API网关是一个服务器，是系统的唯一入口。<br />API网关方式的核心要点是，所有的客户端和消费端都通过统一的网关接入微服务，在网关层处理所有的非业务功能。通常，网关提供REST/HTTP的访问API。
<a name="k6g2B"></a>
## 2、Spring Cloud Gateway介绍
Spring Cloud Gateway是Spring Cloud的新一代API网关，基于WebFlux框架实现，它旨在为微服务架构提供一种简单而有效的统一的API路由管理方式。<br />Spring Cloud Gateway作为Spring Cloud生态系统中的网关，目标是替代Netflix ZUUL，具有更好的性能、更强的扩展性、以及更丰富的功能特性，其不仅提供统一的路由方式，并且基于Filter链的方式提供了网关基本的功能，例如：安全，监控/埋点，限流等。
<a name="yCHIC"></a>
## 3、Spring Cloud Gateway的特性

- 基于Spring Framework 5， Project Reactor和Spring Boot 2.0
- 动态路由：能够匹配任何请求属性
- 可以对路由指定 Predicate 和 Filter
- 集成Hystrix断路器
- 集成Spring Cloud DiscoveryClient 服务发现功能
- 易于编写的Predicate和Filter
- 请求限流
- 支持路径重写
<a name="DidWO"></a>
## 4、Spring Cloud Gateway的三大核心概念
**路由(Route)：** 路由是网关最基础的部分，路由信息由一个ID，一个目标URI，一组断言和过滤器组成。路由断言Predicate用于匹配请求，过滤器Filter用于修改请求和响应。如果断言为true，则说明请求URI和配置匹配，则执行路由。
```yaml
spring:
  cloud:
    gateway:
      # 定义多个路由
      routes:
      # 一个路由route的id
      - id: path_route
        # 该路由转发的目标URI
        uri: https://example.org
        # 路由条件集合
        predicates:
        - Path=/test/**
        # 过滤器集合
        filters:
        - AddRequestHeader=X-Request-Id, 1024
        - AddRequestParameter=color, red
```
**断言(Predicate)：** 参考Java8中的断言Predicate，用于实现请求匹配逻辑，例如匹配路径、请求头、请求参数等。请求与断言匹配则执行该路由。<br />**过滤器(Filter)：** 指的是Spring框架中GatewayFilter的实例，使用过滤器，可以在请求被路由前后对请求进行修改。
<a name="r12hs"></a>
## 5、Gateway工作流程
客户端向Spring Cloud Gateway发出请求，然后在Gateway Handler Mapping中找到与请求相匹配的路由，将其发送到Gateway Web Handler。Handler再通过指定的过滤器链来对请求进行过滤处理，最后发送到实际的服务执行业务逻辑，然后返回。<br />![335fd5e7a20cb172b86cff94aadb2a17[1].png](https://cdn.nlark.com/yuque/0/2023/png/396745/1690770598205-da44c083-f7b4-4ef1-b9e0-43c1775d3aff.png#averageHue=%23f6f6f6&clientId=u9c79400d-51d7-4&from=ui&id=u4403f16e&originHeight=547&originWidth=411&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=33911&status=done&style=none&taskId=u3c3f263c-9214-4523-8bda-775d8ded9c0&title=)<br />过滤器链被虚线分隔，是因为过滤器既可以在转发请求前拦截请求，也可以在请求处理之后对响应进行拦截处理。
<a name="VzTNt"></a>
## 6、Gateway核心配置
依赖
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-gateway</artifactId>
</dependency>
```
启动类
```java
@SpringBootApplication
@EnableEurekaClient
public class GatewayApplication {

    public static void main(String[] args) {
        SpringApplication.run(GatewayApplication.class, args);
    }

}
```
application.yml
```yaml
spring: 
  application:
    name: cloud-gateway 
  cloud:
    gateway:
      routes:
      # 路由的ID，没有固定规则但要求唯一，建议配合服务名
      - id: config_route
       # 匹配后提供服务的路由地址
        uri: http://ityouknow.com
        # 断言，路径相匹配的条件
        predicates:
        - Path=/routeconfig/rest/**
      - id: header_route
        uri: http://ityouknow.com
        predicates:
        - Header=X-Request-Id, \d+
```
<a name="iS5F0"></a>
## 7、动态路由
网关接收外部请求，按照一定的规则，将请求转发给其他服务或者应用。如果站在服务调用的角度，网关就扮演着服务消费者的角色，此时，如果再来看看服务调用的目标URI配置，就会很自然的发现一个问题，服务提供者调用的地址是写死的，即网关没有动态的发现服务，这就涉及到了服务的自动发现问题，以及发现服务后，所涉及到的服务调用的负载均衡的问题。<br />可以通过Nacos或者Eureka注册中心动态发现服务，通过Ribbon进行服务调用的负载均衡。同样，Gateway也可以整合Nacos或者Eureka，Ribbon从而实现动态路由的功能。<br />想要使用动态路由的功能，首先要整合注册中心，这里以Nacos为例
```xml
<!--SpringCloud ailibaba nacos -->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>
```
```yaml
spring:
  application:
    name: cloud-gateway
  cloud:
    nacos:
      discovery:
        server-addr: localhost:8848
    gateway:
      routes:
        #路由的ID，没有固定规则但要求唯一，建议配合服务名
        - id: config_route
  #匹配后提供服务的路由地址, 这里lb之后，跟的是要调用的服务名称
          uri: lb://nacos-provider-8002
  # 断言，路径相匹配的条件
          predicates:
            - Path=/routeconfig/rest/**
```
此时，当id为config_route的路由规则匹配某个请求后，在调用该请求对应的服务时，就会从nacos注册中心自动发现服务，并在服务调用的时候实现负载均衡。
<a name="aeXOU"></a>
## 8、Predicate
在Gateway中，有一些的内置Predicate Factory，有了这些Pridicate Factory，在运行时，Gateway会 自动根据需要创建其对应的Pridicate对象测试路由条件。
<a name="FBbus"></a>
### Path 路由断言 Factory
根据请求路径匹配的路由条件工厂
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: path_route
        uri: https://example.org
        predicates:
        # 如果可以匹配的PathPattern有多个，则每个路径模式以，分开
        - Path=/red/{segment},/blue/{segment}
```
<a name="eRywe"></a>
### After 路由断言 Factory
在指定日期时间之后发生的请求都将被匹配
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: after_route
        uri: https://example.org
        predicates:
        - After=2017-01-20T17:42:47.789-07:00[America/Denver]
```
<a name="oxn4u"></a>
### Cookie 路由断言 Factory
Cookie 路由断言 Factory有两个参数，cookie名称和正则表达式。请求包含此cookie名称且正则表达式为真的将会被匹配。
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: cookie_route
        uri: https://example.org
        predicates:
        - Cookie=chocolate, ch.p
```
<a name="IZPdZ"></a>
### Header 路由断言 Factory
Header 路由断言 Factory有两个参数，header名称和正则表达式。请求包含此header名称且正则表达式为真的将会被匹配。
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: header_route
        uri: https://example.org
        predicates:
        - Header=X-Request-Id, \d+
```
<a name="YfOiN"></a>
### Host 路由断言 Factory
Host 路由断言 Factory包括一个参数：host name列表。使用Ant路径匹配规则， `.` 作为分隔符。
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: host_route
        uri: https://example.org
        predicates:
        - Host=**.somehost.org,**.anotherhost.org
```
<a name="eDgd4"></a>
### Method 路由断言 Factory
Method 路由断言 Factory只包含一个参数：需要匹配的HTTP请求方式
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: method_route
        uri: https://example.org
        predicates:
        - Method=GET
```
<a name="Sy4cS"></a>
### 自定义Predicate
可以自定义Predicate来实现复杂的路由匹配规则：
```java
// 实现自定义 Predicate 工厂
// 通过HostRoutePredicateFactory创建Predicate进行路由判断
@Component
public class MyHostRoutePredicateFactory extends AbstractRoutePredicateFactory<MyHostRoutePredicateFactory.Config> {

    public MyHostRoutePredicateFactory() {
        // Config 类作为 Predicate 的配置参数类
        super(Config.class);
    }

    public static class Config {
        // 路由匹配规则
        private String hostName;

        public String getHostName() {
            return hostName;
        }

        public void setHostName(String hostName) {
            this.hostName = hostName; 
        }
    }

    // 生成一个 Predicate 实例
    @Override
    public Predicate<ServerWebExchange> apply(Config config) {
        // 实现匹配逻辑
        return exchange -> {
            // 根据config实现匹配判断

            String host = exchange.getRequest().getURI().getHost();
            // 匹配配置中的域名
            return host.equals(config.getHostName());
        };
    } 

}

// 使用
RouteLocator locator = new RouteLocatorBuilder(router)
.routes()
.route("test_route", r -> r.path("/test")
       .filters(f -> f.filter(new MyHostRoutePredicateFactory.Config("www.test.com")))
       .uri("http://localhost:8080"))
.build();
```
<a name="NeUQQ"></a>
## 9、自定义`Filter`
可以通过实现`GatewayFilter`和`Ordered`接口自定义Filter来实现请求处理逻辑：
```java
@Component
public class TokenFilter implements GatewayFilter, Ordered {

    @Override
    public Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain) {
        //请求处理逻辑
        log.info("请求路径:"+ exchange.getRequest().getPath());

        ServerHttpRequest request = exchange.getRequest();
        MultiValueMap<String, HttpCookie> cookies = request.getCookies();
        List<HttpCookie> tokens = cookies.get("access_token");
        if (tokens == null || tokens.size() == 0) {
            throw new RuntimeException("少了cookie！");
        }

        return chain.filter(exchange);
    }

    @Override
    public int getOrder() {
        return 0; 
    }

}
```
<a name="VzXji"></a>
## 10、默认过滤器
Spring Cloud Gateway内置了多种过滤器，例如：

- AddRequestHeader GatewayFilter：在请求头中添加参数
- PrefixPath GatewayFilter：请求路径前缀
- Hystrix GatewayFilter：断路器
- RateLimit GatewayFilter：限流
- Retry GatewayFilter：重试
