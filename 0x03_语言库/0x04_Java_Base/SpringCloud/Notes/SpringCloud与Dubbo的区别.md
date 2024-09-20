Java SpringCloud Dubbo
<a name="l56Ed"></a>
## 1、SpringCloud与Dubbo的区别
两者都是现在主流的微服务框架，但却存在不少差异：

- 初始定位不同： SpringCloud定位为微服务架构下的一站式解决方案；Dubbo 是 SOA 时代的产物，它的关注点主要在于服务的调用和治理
- 生态环境不同： SpringCloud依托于Spring平台，具备更加完善的生态体系；而Dubbo一开始只是做RPC远程调用，生态相对匮乏，现在逐渐丰富起来。
- 调用方式： SpringCloud是采用Http协议做远程调用，接口一般是Rest风格，比较灵活；Dubbo是采用Dubbo协议，接口一般是Java的Service接口，格式固定。但调用时采用Netty的NIO方式，性能较好。
- 组件差异比较多，例如SpringCloud注册中心一般用Eureka，而Dubbo用的是Zookeeper

SpringCloud生态丰富，功能完善，更像是品牌机，Dubbo则相对灵活，可定制性强，更像是组装机。

- SpringCloud：Spring公司开源的微服务框架，SpirngCloud 定位为微服务架构下的一站式解决方案。
- Dubbo：阿里巴巴开源的RPC框架，Dubbo 是 SOA 时代的产物，它的关注点主要在于服务的调用，流量分发、流量监控和熔断

两者的生态对比：<br />![2021-09-16-10-41-58-871941.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631760133715-ad7a6a6c-3be9-47ce-8960-22af2ab3f779.png#clientId=u5ac33db1-08a6-4&from=ui&id=uf8a999b0&originHeight=341&originWidth=771&originalType=binary&ratio=1&size=12066&status=done&style=none&taskId=u5d2df5c9-7014-496d-aecb-5643fe7180b)<br />Spring Cloud 的功能很明显比 Dubbo 更加强大，涵盖面更广，而且作为 Spring 的旗舰项目，它也能够与 Spring Framework、Spring Boot、Spring Data、Spring Batch 等其他 Spring 项目完美融合，这些对于微服务而言是至关重要的。<br />使用 Dubbo 构建的微服务架构就像组装电脑，各环节选择自由度很高，但是最终结果很有可能因为一条内存质量不行就点不亮了，总是让人不怎么放心，但是如果使用者是一名高手，那这些都不是问题。<br />而 Spring Cloud 就像品牌机，在 Spring Source 的整合下，做了大量的兼容性测试，保证了机器拥有更高的稳定性，但是如果要在使用非原装组件外的东西，就需要对其基础原理有足够的了解。
<a name="XK4in"></a>
## 2、dubbo和Feign远程调用的差异
Feign是SpringCloud中的远程调用方式，基于成熟Http协议，所有接口都采用Rest风格。因此接口规范更统一，而且只要符合规范，实现接口的微服务可以采用任意语言或技术开发。但受限于http协议本身的特点，请求和响应格式臃肿，其通信效率相对会差一些。<br />Dubbo框架默认采用Dubbo自定义通信协议，与Http协议一样底层都是TCP通信。但是Dubbo协议自定义了Java数据序列化和反序列化方式、数据传输格式，因此Dubbo在数据传输性能上会比Http协议要好一些。<br />不过这种性能差异除非是达极高的并发量级，否则无需过多考虑。<br />相关资料：<br />Dubbo采用自定义的Dubbo协议实现远程通信，是一种典型的RPC调用方案，而SpringCloud中使用的Feign是基于Rest风格的调用方式。
<a name="YqxEW"></a>
#### 1）Rest风格
REST是一种架构风格，指的是一组架构约束条件和原则。满足这些约束条件和原则的应用程序或设计就是 RESTful。<br />Rest的风格可以完全通过HTTP协议实现，使用 HTTP 协议处理数据通信。REST架构对资源的操作包括获取、创建、修改和删除资源的操作正好对应HTTP协议提供的GET、POST、PUT和DELETE方法。<br />因此请求和想要过程只要遵循http协议即可，更加灵活<br />SpringCloud中的Feign就是Rest风格的调用方式。
<a name="rhycw"></a>
#### 2）RPC
Remote Procedure Call，远程过程调用，就是像调用本地方法一样调用远程方法。<br />RPC一般要确定下面几件事情：

- 数据传输方式： 多数RPC框架选择TCP作为传输协议，性能比较好。
- 数据传输内容： 请求方需要告知需要调用的函数的名称、参数、等信息。
- 序列化方式： 客户端和服务端交互时将参数或结果转化为字节流在网络中传输，那么数据转化为字节流的或者将字节流转换成能读取的固定格式时就需要进行序列化和反序列化

因为有序列化和反序列化的需求，因此对数据传输格式有严格要求，不如Http灵活<br />Dubbo协议就是RPC的典型代表。<br />看看Dubbo协议和Feign的调用区别：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631760106634-f5060814-10f2-419a-8081-878b6de98861.webp#clientId=u5ac33db1-08a6-4&from=paste&id=u1ee27851&originHeight=155&originWidth=770&originalType=url&ratio=1&status=done&style=none&taskId=u7a1d3324-bfb7-4a34-9dfc-28ddfa5856f)
<a name="gO0S1"></a>
## 3、Eureka和Zookeeper注册中心的区别
SpringCloud和Dubbo都支持多种注册中心，不过目前主流来看SpringCloud用Eureka较多，Dubbo则以Zookeeper为主。两者存在较大的差异：

- 从集群设计来看： Eureka集群各节点平等，没有主从关系，因此可能出现数据不一致情况；ZK为了满足一致性，必须包含主从关系，一主多从。集群无主时，不对外提供服务
- CAP原则来看： Eureka满足AP原则，为了保证整个服务可用性，牺牲了集群数据的一致性；而Zookeeper满足CP原则，为了保证各节点数据一致性，牺牲了整个服务的可用性。
- 服务拉取方式来看： Eureka采用的是服务主动拉取策略，消费者按照固定频率（默认30秒）去Eureka拉取服务并缓存在本地；ZK中的消费者首次启动到ZK订阅自己需要的服务信息，并缓存在本地。然后监听服务列表变化，以后服务变更ZK会推送给消费者。
<a name="wQZAy"></a>
## 4、SpringCloud中的常用组件有哪些？
Spring Cloud的子项目很多，比较常见的都是Netflix开源的组件：<br />Spring Cloud Config<br />集中配置管理工具，分布式系统中统一的外部配置管理，默认使用Git来存储配置，可以支持客户端配置的刷新及加密、解密操作。<br />Spring Cloud Netflix<br />Netflix OSS 开源组件集成，包括Eureka、Hystrix、Ribbon、Feign、Zuul等核心组件。

- Eureka：服务治理组件，包括服务端的注册中心和客户端的服务发现机制；
- Ribbon：负载均衡的服务调用组件，具有多种负载均衡调用策略；
- Hystrix：服务容错组件，实现了断路器模式，为依赖服务的出错和延迟提供了容错能力；
- Feign：基于Ribbon和Hystrix的声明式服务调用组件；
- Zuul：API网关组件，对请求提供路由及过滤功能。

Spring Cloud Bus<br />用于传播集群状态变化的消息总线，使用轻量级消息代理链接分布式系统中的节点，可以用来动态刷新集群中的服务配置。<br />Spring Cloud Consul<br />基于Hashicorp Consul的服务治理组件。<br />Spring Cloud Security<br />安全工具包，对Zuul代理中的负载均衡OAuth2客户端及登录认证进行支持。<br />Spring Cloud Sleuth<br />Spring Cloud应用程序的分布式请求链路跟踪，支持使用Zipkin、HTrace和基于日志（例如ELK）的跟踪。<br />Spring Cloud Stream<br />轻量级事件驱动微服务框架，可以使用简单的声明式模型来发送及接收消息，主要实现为Apache Kafka及RabbitMQ。<br />Spring Cloud Task<br />用于快速构建短暂、有限数据处理任务的微服务框架，用于向应用中添加功能性和非功能性的特性。<br />Spring Cloud Zookeeper<br />基于Apache Zookeeper的服务治理组件。<br />Spring Cloud Gateway<br />API网关组件，对请求提供路由及过滤功能。<br />Spring Cloud OpenFeign<br />基于Ribbon和Hystrix的声明式服务调用组件，可以动态创建基于Spring MVC注解的接口实现用于服务调用，在Spring Cloud 2.0中已经取代Feign成为了一等公民。
<a name="peRVU"></a>
## 5、微服务调用关系复杂，如何做监控和错误排查？
企业中对于微服务监控有一套东西，叫做APM。比如：SpringCloudSeluth+Zipkin，Pinpoint、Skywalking，可以实现性能监控、链路跟踪（精确到某个代码，某条sql）、CPU运行情况，链路运行耗时。<br />当然， 还可以借助于分布式日志管理系统。把项目运行的日志收集，形成统计报表，放入elasticsearch，便于搜索查看。比如：ELK技术栈、GrayLog
<a name="Uikxv"></a>
## 6、Hystix的作用是什么？
Hystix是Netflix开源的一个延迟和容错库，用于隔离访问远程服务、第三方库，防止出现级联失败。比较常用的手段就是线程隔离和服务熔断。
