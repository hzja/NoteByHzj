JavaSpringCloudGateway
<a name="SwlSA"></a>
## 为什么需要微服务网关
![网关流量](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642671999-e2f675c1-b265-42eb-89f6-e6ebe082d413.png#averageHue=%23f5f0e9&clientId=u5e9baa42-6a22-4&from=paste&id=u92096591&originHeight=538&originWidth=990&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u102cf4c0-e7b3-4388-a88c-4b1ac603922&title=%E7%BD%91%E5%85%B3%E6%B5%81%E9%87%8F "网关流量")<br />从功能角度来看，微服务网关通常用来统一提供认证授权、限流、熔断、协议转换等功能。<br />从使用场景上来看

- 南北向流量，需要流量网关和微服务网关配合使用，主要是为了区分外部流量和微服务流量，将内部的微服务能力，以统一的 HTTP 接入点对外提供服务
- 东西向流量，在一些业务量比较大的系统中，可能会按照业务域隔离出一系列的微服务，在同一业务域内的微服务通信走的是服务发现机制，而跨业务域访问，则建议借助于微服务网关。
<a name="K11AZ"></a>
## 微服务网关核心功能
微服务架构、微服务/API 网关这些关键词发展至今，早已不是什么新鲜的概念，技术选型者也从出于好奇心关注一个技术，转移到了更加关注这个技术的本质。市场上各类网关产品的功能也逐渐趋于同质化，基本可以用同一张图来概括：<br />![网关核心功能](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642671998-de8cb9bb-472f-4cd8-aea1-baa0c05cd73b.png#averageHue=%23705748&clientId=u5e9baa42-6a22-4&from=paste&id=ufdb9d9dc&originHeight=373&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc963a2f5-b88b-4f4c-bc16-5344f69020d&title=%E7%BD%91%E5%85%B3%E6%A0%B8%E5%BF%83%E5%8A%9F%E8%83%BD "网关核心功能")
<a name="d0Kv3"></a>
## 网关选型对比
企业在选择使用一款网关产品时，通常会有两个选择，一是基于某一款开源产品做二次开发，二是选择某一款商业化产品开箱即用，无论如何，都应当从稳定性、安全、性能、业务兼容性等方面去进行选型。<br />早期 SpringCloud 社区出现过 Zuul 这种产品，时至今日搜索微服务网关的资料，大概率都会出现它的身影，仅其通信模型是同步的线程模型这一条，就不足以支撑其成为企业级的网关产品选型，会主要对比 SpringCloud Gateway、阿里云 CSB 2.0、Nginx、Kong、Envoy。<br />![选型对比](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642672022-d34ba241-3be3-43fe-b843-597ad2a98978.png#averageHue=%23ced1d2&clientId=u5e9baa42-6a22-4&from=paste&id=u5f3bc7fc&originHeight=433&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ubcf6faea-ac69-40ce-832c-f2e2f96e424&title=%E9%80%89%E5%9E%8B%E5%AF%B9%E6%AF%94 "选型对比")<br />严谨来说，这几个网关并不适合对比，因为他们都有其各自适用的场景，表格仅供参考。<br />SpringCloud Gateway 的优势在于其可以很好地跟 Spring 社区和 SpringCloud 微服务体系打通，这一点跟 Java 语言流行的原因如出一辙，所以如果一个企业的语言体系是 Java 技术栈，并且基于 SpringBoot/ SpringCloud 开发微服务，选型 SpringCloud Gateway 作为微服务网关，会有着得天独厚的优势。<br />SpringCloud Gateway 选型的优势：

- SpringCloud Gateway 有很多开箱即用的功能，且扩展点多
- 适合 Java 技术栈
- Spring/SpringCloud 社区生态好
- 适合跟 SpringBoot/ SpringCloud 微服务生态集成
<a name="GyJEX"></a>
## SpringCloud Gateway 介绍
如果你之前没有了解过 SpringCloud Gateway，也不用担心，下面一小部分篇幅会介绍 SpringCloud Gateway 基本用法。这是一段非常基础的 SpringCloud Gateway 路由配置示例
```yaml
spring:
  cloud:
    gateway:
      routes:
        - id: aliyun
          uri: https://www.aliyun.com
          predicates:
            - Host=*.aliyun.com
        - id: httpbin
          uri: http://httpbin.org
          predicates:
            - Path=/httpbin/**
          filters:
            - StripPrefix=1
        - id: sca-provider
          uri: lb://sca-provider
          predicates:
            - Path=/sca/**
          filters:
            - StripPrefix=1
    nacos:
      discovery:
        server-addr: mse-xxxxx-p.nacos-ans.mse.aliyuncs.com:8848
```
该示例介绍了微服务网关常见的几种路由配置示例

- Host 路由匹配
- 前缀 Path 路由匹配
- 前缀 Path 路由匹配 & 服务发现

SpringCloud Gateway 支持丰富的路由匹配逻辑，以应对各种类型的业务诉求：<br />![SCG断言](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642671942-dc59fc0f-aec8-444b-884f-3b233c371270.png#averageHue=%23464747&clientId=u5e9baa42-6a22-4&from=paste&id=u0789e71b&originHeight=343&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub739932c-870a-45b0-b0d5-47c190c8afb&title=SCG%E6%96%AD%E8%A8%80 "SCG断言")<br />其中 Path、Header、Method 这几种断言最为常用。<br />针对于网关请求路径、参数和后端服务请求路径、参数不一致的场景，SpringCloud Gateway 也提供了诸多开箱即用的 GatewayFilter，以实现对请求和响应的定制。<br />![SCG插件](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642671991-5cd3f0e4-5a84-46f0-8fc6-b0e3e3950fc5.png#averageHue=%23fdfefd&clientId=u5e9baa42-6a22-4&from=paste&id=uef7ea39c&originHeight=362&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u17d3ad31-8f76-4570-b940-4f188fc4dc6&title=SCG%E6%8F%92%E4%BB%B6 "SCG插件")<br />SpringCloud Gateway 的 user guide 介绍到此为止，如果想要了解 develop guide，建议参考 SpringCloud Gateway 的官方文档。
<a name="NQstF"></a>
## 开源特性 vs 企业级特性需求
众所周知，开源产品直接投入企业级生产使用一般是会面临一些挑战的，毕竟场景不同。以扩展性为例，开源产品大多讲究扩展点丰富，以应对开源用户千奇百怪的需求，而企业级产品场景更为单一，性能和稳定性是第一考虑因素，当二者发生 trade off 时，则需要一些取舍了。<br />![企业级特性](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642672656-021ebaae-4bda-4811-8b27-ac035797f6b4.png#averageHue=%23f2cbc0&clientId=u5e9baa42-6a22-4&from=paste&id=u6b0caa73&originHeight=481&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u8c15abdc-7e8a-48c9-8273-01d9ccf7651&title=%E4%BC%81%E4%B8%9A%E7%BA%A7%E7%89%B9%E6%80%A7 "企业级特性")<br />开源 SpringCloud Gateway 没有开箱即用地支持一些重要的企业级特性，如果选型 SpringCloud Gateway 构建生产级别可用的微服务网关，建议是需要补足以上这些能力。下面会花较多的篇幅介绍在开源基础上做的一些企业级改造，希望能够抛砖引玉。
<a name="k8hJZ"></a>
### 白屏化管控
![白屏化](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642672828-cd48d5ab-7884-4cdc-963c-d46701840398.png#averageHue=%23fcfcfb&clientId=u5e9baa42-6a22-4&from=paste&id=uc542e638&originHeight=334&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uf6b6341a-766b-468b-8f9e-99e6d643524&title=%E7%99%BD%E5%B1%8F%E5%8C%96 "白屏化")<br />表面看来，SpringCloud Gateway 并没有配套一个管理控制台，深层次一点来看，是 SpringCloud Gateway 还停留在一个开发框架层面，不是那么的产品化，同时它的领域模型也不是划分的那么清晰，说的好听点，这说明 SpringCloud Gateway 有充足的改造空间。<br />改造原则有两点，一是完全兼容开源的规则及模型，不破坏底层规则的语义，这样可以跟随社区的节奏一起演进，将来也有机会贡献给社区，二是区分研发态的领域模型和用户态的产品模型，抽象出了路由、服务、来源、消费者、策略、插件等领域对象，这算不上什么创新，实际上网关领域的这些模型早已有了一些约定俗成的规范。<br />白屏化管控的背后，也意味着一切配置：路由配置、服务配置、策略配置...都是动态的，并且配置的变更都会实时生效。
<a name="jS6DA"></a>
### 配置方案重构
上文提到了配置实时生效这一改造，有人可能会有疑问，开源不是已经支持将路由配置存储在 Nacos 中了吗？对的，开源支持两种配置方式，一是将路由配置在 application.yaml 中，这样最简单，但对于路由配置的 curd 都需要重启进程，非常繁琐，二是将配置托管到 Nacos 这样的配置中心组件中，实现分布式配置，能够动态刷新，但这还不足以支持企业级需求，将配置存储在单个 dataId 中这种开源方案有以下痛点：

1. 配置推送慢：配置量大，网络传输慢，万级别配置推送耗时 5 分钟
2. 爆炸半径大：不支持配置拆分，错误配置影响解析流程，导致网关路由整体不可用
3. 配置规模：单个 value 有 10M大小限制，仅支持千级别路由

配置拆分势在必行，但其中困难也很多，例如动态监听的管理，稳定性的保障流程尤为复杂，额外提供的视图层与实际配置中心数据一致性保障等等。方案参考下图：<br />![配置方案重构](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642672969-c14c040f-2576-4c13-9780-985ab16506f8.png#averageHue=%236b6b6b&clientId=u5e9baa42-6a22-4&from=paste&id=u58f7f80d&originHeight=529&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud33af789-97b6-45a3-bbf5-d76de268a10&title=%E9%85%8D%E7%BD%AE%E6%96%B9%E6%A1%88%E9%87%8D%E6%9E%84 "配置方案重构")<br />图中还有一个细节，也是优先选择 Nacos 作为配置中心的原因，nacos-client 的 snapshot 机制可以保证在管控以及配置中心组件都不可用时，即使网关 broker 重启了，依旧保证路由不丢失，保证自身可用性。<br />经过这套方案的改造，获得显著的优化效果：

- 推送时间优化：1w 配置 5 分钟 -> 30 秒
- 配置量上限提升：1000 -> 10w
- 确保了配置推送的最终一致性
<a name="cIC1j"></a>
### 协议转换 x 服务发现
这两个企业级改造放到一起说，在实现上这两个模块也耦合的比较紧密。<br />协议转换：就以 Java 微服务体系而言，后端服务很有可能会出现 Dubbo 框架或者 GRPC 框架，甚至有些老的业务还会使用 WebService 这类框架，大多数时候说的网关都是只对接 HTTP 这一类通信协议，这限制了后端服务只能是 SpringBoot 或者 SpringCloud 框架，网关支持后端不同协议类型的能力，称之为协议转换。<br />服务发现：微服务框架离不开服务发现，一般常见的注册中心包括 Nacos、Eureka 等，例如开源 SpringCloud Gateway 便支持对接 Nacos/Eureka 两类注册中心。<br />这类开源特性的痛点是：

1. SpringCloud Gateway 仅支持 HTTP2HTTP，不支持 HTTP2DUBBO，HTTP2GRPC，HTTP2WEBSERVICE
2. SpringCloud Gateway 仅支持单一注册中心的静态配置

一些常见的企业级诉求：

1. 存在不同类型的微服务架构：SpringCloud、Dubbo、GRPC
2. 网关支持跨环境访问，需要连接多个注册中心或者多个命名空间

针对这些痛点和诉求，分享一些改造时遇到的难点以及经验<br />![服务发现x协议转换扩展](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642673099-a1a0ff7a-8785-4831-81f3-f0ae6e302ca6.png#averageHue=%23363636&clientId=u5e9baa42-6a22-4&from=paste&id=u888fa974&originHeight=314&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ufae651f9-0dba-4bcb-bda0-a884bbd5165&title=%E6%9C%8D%E5%8A%A1%E5%8F%91%E7%8E%B0x%E5%8D%8F%E8%AE%AE%E8%BD%AC%E6%8D%A2%E6%89%A9%E5%B1%95 "服务发现x协议转换扩展")<br />在支持不同协议时，对应的服务框架可能已经有了对应的 remoting 层和 discovery 层，我们的选择是仅引入该协议的 remoting 二方包解决协议转换问题，对于 discovery 层，应当自行封装，避免使用对应协议的 discovery 层这个误区，因为回归到网关领域，服务发现和协议转换是对等的模块，抽象 ServiceDiscoveryFIlter 负责服务发现，ProtocolTransferFilter 则负责点对点的协议通信。<br />在服务发现层，为了适配不同注册中心的模型（推和拉），提供了两个实现 PullServiceRegistry、PushServiceRegistry，这些改造是独立于 spring-cloud-loadbalancer 模块实现的，开源的默认实现存在诸多的限制，例如仅支持拉模型 + 缓存服务列表的方案，实际上推模型能够为网关的服务发现提供更高的实时性。<br />基本流程：服务发现 serviceName -> n x IP，负载均衡 IP n ->1，协议转换 IP 点对点通信<br />这样一套扩展机制可以在有新的协议类型、注册中心、负载均衡算法需要对接时实现快速扩展。
<a name="NWxcD"></a>
### 限流熔断
如果仔细阅读过 SpringCloud Gateway 的文档，你会发现，开源对限流熔断的支持是非常有限的，它强依赖一个 Redis 做集群限流，且限流方案是自己实现的，可能会更加信赖 Sentinel 提供的解决方案。事实上，开源 Sentinel 也对 SpringCloud Gateway 提供了一部分开箱即用的能力，使用层面完全没问题，主要是欠缺了一部分可观测性的能力。<br />![限流熔断](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642673456-5f004501-de78-4613-bd12-4553e5b1f918.png#averageHue=%23f1d9ca&clientId=u5e9baa42-6a22-4&from=paste&id=ub0041bcd&originHeight=587&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u709df1e6-cfa5-44cd-972e-a90846d93df&title=%E9%99%90%E6%B5%81%E7%86%94%E6%96%AD "限流熔断")<br />在改造中，尤为注意要使用高版本的 Sentinel，即按比例阈值这套模型实现的限流方案，集成 Sentinel 之后，按照网关的通用场景提供了两类限流模型：基于慢调用比例的限流熔断和基于响应码比例的限流熔断。借助于 Sentinel 的能力，可惜实现渐进式的恢复。
<a name="wBbFg"></a>
### 可观测体系建设
可观测性体系的建设，可以说是很多开源产品距离企业级使用的距离，SpringCloud Gateway 亦是如此。<br />![可观测性](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642673659-904c64ab-2dcf-4014-8884-290bf9221ff0.png#averageHue=%23f0edde&clientId=u5e9baa42-6a22-4&from=paste&id=ud2ac831a&originHeight=538&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u129634ea-9196-4aa2-9e5d-cebe9a1d69d&title=%E5%8F%AF%E8%A7%82%E6%B5%8B%E6%80%A7 "可观测性")<br />网关通常会需要记录三类可观测性指标。

- Metrics：如上图所示，记录请求数、QPS、响应码、P99、P999 等指标
- Trace：网关链路能够串联后续微服务体系链路，实现全链路监控
- Logging：按类别打印网关日志，常见的日志分类如 accessLog、requestLog、remotingLog 等

开源 SpringCloud Gateway 集成了 micrometer-registry-prometheus，提供了一个开箱即用的大盘：[https://docs.spring.io/spring-cloud-gateway/docs/3.1.8/reference/html/gateway-grafana-dashboard.json](https://docs.spring.io/spring-cloud-gateway/docs/3.1.8/reference/html/gateway-grafana-dashboard.json)，需要更加丰富维度的指标则需要自行埋点。<br />Trace 方案推荐对接 opentelemetry。<br />Logging 方案则是 SpringCloud Gateway 开源欠缺的，在实际生产中至少应该打印 accessLog 记录请求信息，按需开启 requestLog 记录请求的 payload 信息和响应体信息，以及与后端服务连接的日志，用于排查一些连接问题。日志采集方案的实践是将 accessLog 输出到标准输出中，方便在 K8s 架构下配置采集，或者采用日志 agent 的方案进行文件采集。
<a name="dmrpU"></a>
## 性能优化
除了功能层面的优化与新增，网关的性能也是使用者尤为关注的点。在前文中，并没有把 SpringCloud Gateway 归为一个性能特别高的网关分类中，主要是基于实践，发现其有不少优化空间。下面的章节会分享一些基于 SpringCloud Gateway 进行的性能优化。<br />网关优化道阻且长，为了验证优化效果，建设性能基线不可避免，需要面向 benchmark 进行优化。<br />一些常用的优化技巧在网关中也同样适用，例如：缓存、懒加载、预分配、算法复杂度优化、CPU 友好操作，减少线程切换。<br />![性能基线](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642673683-094f4bf2-fda5-4b0d-8210-0e3d8838bf63.png#averageHue=%23fcfcfc&clientId=u5e9baa42-6a22-4&from=paste&id=u128b9645&originHeight=570&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u048e2280-b24e-4a02-a968-43660b050c3&title=%E6%80%A7%E8%83%BD%E5%9F%BA%E7%BA%BF "性能基线")
<a name="SgwqZ"></a>
### 火焰图
通过火焰图观测性能可以从宏观角度分析大的性能损耗点<br />![火焰图](https://cdn.nlark.com/yuque/0/2023/png/396745/1689642673872-4dccd88d-515b-4306-b4c9-84b671d4370c.png#averageHue=%23c6edb9&clientId=u5e9baa42-6a22-4&from=paste&id=ScKHp&originHeight=536&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u292527a2-6b03-436b-a021-8b9993011fb&title=%E7%81%AB%E7%84%B0%E5%9B%BE "火焰图")<br />一个理想的网关火焰图应当是大部分的时间片占用花费在 IO 上，即图中的 netty 相关的损耗，除此之外占用了 CPU 的类，都需要重点关注。通过火焰图，也定位到了相当多的性能损耗点，并针对进行了优化。
<a name="fPCQQ"></a>
### GlobalFilter 排序优化
SpringCloud Gateway 中通过 `GlobalFilter`、`GatewayFilter` 对请求进行过滤，在 `FilteringWebHandler` 中可以看到这段逻辑
```java
public Mono<Void> handle(ServerWebExchange exchange) {
    Route route = exchange.getRequiredAttribute(GATEWAY_ROUTE_ATTR);
    List<GatewayFilter> gatewayFilters = route.getFilters();
    
    List<GatewayFilter> combined = new ArrayList<>(this.globalFilters);
    combined.addAll(gatewayFilters);
    // TODO: needed or cached?
    AnnotationAwareOrderComparator.sort(combined);
    
    return new DefaultGatewayFilterChain(combined).filter(exchange);
}
```
开源实现在每次请求级别都会重新组装出一个 `FilterChain`，并进行排序，内存分配和排序会占用 CPU，无疑会导致性能下降，通过注释可以看到 `Contributor` 自己也意识到了这里的性能问题，但一直没有修复。<br />一个可行的优化手段是在路由或者策略变更时，触发 FilterChain 的更新，这样请求时 FilterChain 就没必要重新构造了。而观测到这一性能问题，正是通过了火焰图中的 `FilteringWebHandler.handle` 的占用。
<a name="v2kJe"></a>
### 路由增量推送
之前的企业级特性章节中，介绍了配置中心改造的方案，其中提及了开源方案爆炸半径大的问题，可以从下面的代码中，窥见一斑：
```
public class RouteDefinitionRouteLocator implements RouteLocator {

 @Override
 public Flux<Route> getRoutes() {
  Flux<Route> routes = this.routeDefinitionLocator.getRouteDefinitions().map(this::convertToRoute);

  if (!gatewayProperties.isFailOnRouteDefinitionError()) {
   // instead of letting error bubble up, continue
   routes = routes.onErrorContinue((error, obj) -> {
    if (logger.isWarnEnabled()) {
     logger.warn("RouteDefinition id " + ((RouteDefinition) obj).getId()
       + " will be ignored. Definition has invalid configs, " + error.getMessage());
    }
   });
  }

  return routes.map(route -> {
   return route;
  });
 }
```
可以见得，SpringCloud Gateway 认为路由配置是一个整体，任意路由的变更，就会导致整个 Route 序列重新构建。并且在默认情况下，如果其中一个路由配置出错了，会导致整个网关路由不可用，除非 `isFailOnRouteDefinitionError` 被关闭。<br />改造方案是使用 Map 结构进行改造，配合路由配置的增量推送，实现 Route 的单点更新。
```java
public class DynamicRouteRepository implements Ordered, RouteLocator, ApplicationEventPublisherAware, RouteDefinitionWriter {

    private RouteConverter routeConverter;

    static class RouteKey implements Ordered {
        private String id;
        private int order;
        ...
    }

    static final Map<RouteKey, Route> ORDERED_ROUTE = new TreeMap<>((o1, o2) -> {
        int order1 = o1.order;
        int order2 = o2.order;
        if (order1 != order2) {
            return Integer.compare(order1, order2);
        }
        return o1.id.compareTo(o2.id);
    });

    private static final Map<String, Integer> ORDER = new HashMap<>();

    public Route getRouteById(String id) {
        return ORDERED_ROUTE.get(new RouteKey(id, ORDER.getOrDefault(id, 0)));
    }
    ...
}
```
<a name="aENKq"></a>
### 路由内存优化
这个优化来自于一次生产问题的排查，起初并没有意识到该问题。问题表现为路由数量非常大时，内存占用的消耗超过了预期，经过 dump 发现，同一份路由的配置内容竟然以 3 种形式常驻于内存中

- Nacos 配置中心自身的 Cache
- SpringCloud Gateway 路由定义 RouteDefinition 的占用
- SpringCloud Gateway 真实路由 Route 的占用

Nacos 的占用在预期之类，但 RouteDefinition 其实仅仅是一个中间变量，如果流程合理，其实是没必要常驻内存的，经过优化，去除了一份占用，增加了支持路由的数量。
<a name="WCSlK"></a>
### 内存泄漏优化
该问题通用来自于生产实践，SpringCloud Gateway 底层依赖 netty 进行 IO 通信，熟悉 netty 的人应当知道其有一个读写缓冲的设计，如果通信内容较小，一般会命中 chunked buffer，而通信内容较大时，例如文件上传，则会触发内存的新分配，而 SpringCloud Gateway 在对接 netty 时存在逻辑缺陷，会导致新分配的池化内存无法完全回收，导致堆外内存泄漏。并且这块堆外内存时 netty 使用 unsafe 自行分配的，通过常规的 JVM 工具还无法观测，非常隐蔽。<br />处于改造成本考量，最终选择的方案是增加一行启动参数 `-Dio.netty.allocator.type=unpooled`，使得请求未命中 chunked buffer 时，分配的临时内存不进行池化，规避内存性能问题。<br />可能有人会有疑问，`-Dio.netty.allocator.type=unpooled`会不会导致性能下降，这个担心完毕没有必要，首先只有大报文才会触发该内存的分配，而网关的最佳实践应该是不允许文件上传这类需求，加上该参数只是为了应对非主流场景的一个兜底行为。
<a name="BLdMo"></a>
### 预构建 URI
该热点问题由 `org.springframework.cloud.client.loadbalancer.LoadBalancerUriTools` 贡献，SpringCloud Gateway 引用了 spring-cloud-loadbalancer 解决服务发现和负载均衡的问题，
```java
private static URI doReconstructURI(ServiceInstance serviceInstance, URI original) {
    String host = serviceInstance.getHost();
    String scheme = (String)Optional.ofNullable(serviceInstance.getScheme()).orElse(computeScheme(original, serviceInstance));
    int port = computePort(serviceInstance.getPort(), scheme);
    if (Objects.equals(host, original.getHost()) && port == original.getPort() && Objects.equals(scheme, original.getScheme())) {
        return original;
    } else {
        boolean encoded = containsEncodedParts(original);
        return UriComponentsBuilder.fromUri(original).scheme(scheme).host(host).port(port).build(encoded).toUri();
    }
}
```
注意最后一行构建，实际是针对不可变对象的一次变更，从而进行了一次深拷贝，重新重构了一个 URI，这样的行为同样发生在调用级别，不要小看这类行为，它会严重占用 CPU。<br />优化方案便是，对于不可变部分的构造，提前到路由推送时构建，对于可变的调用级别的参数，支持修改。这一点跟路由增量推送的优化是一个道理。<br />Spring 体系出于契约考虑，大量使用了不可变变量传递契约信息，但某些扩展点中，又的确希望对其进行变更，不得已进行了深拷贝，从而造成了性能下降，企业级应用需要在其中寻找到一个平衡点。
<a name="du8EK"></a>
### 对象缓存
尽量避免调用链路中出现 new 关键字，它会加大 CPU 的开销，从而影响 IO，可以使用 ThreadLocal 或者对象池化技术进行对象复用。<br />如果 new 关键词仅出现在初始化，配置推送等异步场景，通常是一次性的行为，则出于代码可读性的考虑，不做太多要求。
<a name="oRQea"></a>
## 总结
今天的分享简单介绍了一些主流的网关的对比，并重点介绍了 SpringCloud Gateway 适用的场景。并分析了 SpringCloud Gateway 如果在企业中投入生产使用，需要新增&改造的一些能力，最后针对一些常见的性能优化场景，介绍了一些优化方案。这些经验完全来源 CSB 2.0 微服务网关基于 SpringCloud Gateway 改造的实践，CSB 2.0 是一款适用于私有化输出的网关产品。
