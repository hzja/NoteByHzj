SpringCloud<br />先看一个最经典的业务场景，如开发一个电商网站，要实现支付订单的功能，流程如下：

1. 创建一个订单之后，如果用户立刻支付了这个订单，需要将订单状态更新为“已支付”
2. 扣减相应的商品库存
3. 通知仓储中心，进行发货
4. 给用户的这次购物增加相应的积分

![2021-05-29-22-51-11-864850.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622300066595-bd6ba91d-bbea-44c2-8d18-bef350d82ad7.png#clientId=u99e9257c-3b9a-4&from=ui&id=u31281721&originHeight=306&originWidth=758&originalType=binary&size=45691&status=done&style=shadow&taskId=u923ad726-cd15-4b56-87ab-97b102cbbca)<br />Spring Cloud架构的各个组件的原理分析<br />如上，微服务的应用场景和核心竞争力：

- 降低耦合：每一个微服务专注于单一功能，并通过定义良好的接口清晰表述服务边界。由于体积小、复杂度低，每个微服务可由一个小规模开发团队完全掌控，易于保持高可维护性和开发效率。
- 独立部署：由于微服务具备独立的运行进程，所以每个微服务也可以独立部署。当某个微服务发生变更时无需编译、部署整个应用。由微服务组成的应用相当于具备一系列可并行的发布流程，使得发布更加高效，同时降低对生产环境所造成的风险，最终缩短应用交付周期。
- 选型灵活：微服务架构下，技术选型是去中心化的。每个团队可以根据自身服务的需求和行业发展的现状，自由选择最适合的技术栈。由于每个微服务相对简单，故需要对技术栈进行升级时所面临的风险就较低，甚至完全重构一个微服务也是可行的。
- 容错机制：当某一组建发生故障时，在单一进程的传统架构下，故障很有可能在进程内扩散，形成应用全局性的不可用。在微服务架构下，故障会被隔离在单个服务中。若设计良好，其他服务可通过重试、平稳退化等机制实现应用层面的容错。
- 灵活扩展：单块架构应用也可以实现横向扩展，就是将整个应用完整的复制到不同的节点。当应用的不同组件在扩展需求上存在差异时，微服务架构便体现出其灵活性，因为每个服务可以根据实际需求独立进行扩展。

![2021-05-29-22-51-12-187630.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622300085257-a6e0f892-a0ae-4df4-be7d-752ec0caac61.png#clientId=u99e9257c-3b9a-4&from=ui&id=uc559acf7&originHeight=488&originWidth=1080&originalType=binary&size=90034&status=done&style=shadow&taskId=u1eea08ed-7a4e-40bd-8b7a-09b2678873f)<br />Spring Cloud架构的各个组件的原理分析<br />Dubbo对标Spring Cloud微服务：

- 背景分析：Dubbo，是阿里巴巴服务化治理的核心框架，并被广泛应用于中国各互联网公司；Spring Cloud是知名的Spring家族的产品。阿里巴巴是一个商业公司，虽然也开源了很多的顶级的项目，但从整体战略上来讲，仍然是服务于自身的业务为主。Spring专注于企业级开源框架的研发，不论是在中国还是在世界上使用都非常广泛，开发出通用、开源、稳健的开源框架就是他们的主业。
- 活跃度对比：Dubbo是一个非常优秀的服务治理框架，并且在服务治理、灰度发布、流量分发这方面做的比Spring Cloud还好，除过当当网在基础上增加了rest支持外，已有两年多的时间几乎都没有任何更新了。在使用过程中出现问题，提交到GitHub的Issue也少有回复。相反Spring Cloud自从发展到现在，仍然在不断的高速发展，从GitHub上提交代码的频度和发布版本的时间间隔就可以看出，现在Spring Cloud即将发布2.0版本，到了后期会更加完善和稳定。
- 平台架构：Dubbo框架只是专注于服务之间的治理，如果需要使用配置中心、分布式跟踪这些内容都需要自己去集成，这样无形中使用Dubbo的难度就会增加。Spring Cloud几乎考虑了服务治理的方方面面，更有Spring Boot这个大将的支持，开发起来非常的便利和简单。
- 技术前景：Dubbo在各中小公司也从中受益不少。经过了这么多年的发展，互联网行业也是涌现了更多先进的技术和理念，Dubbo有点可惜。Spring 推出Spring Boot/Cloud也是因为自身的很多原因。Spring最初推崇的轻量级框架，随着不断的发展也越来越庞大，随着集成项目越来越多，配置文件也越来越混乱，慢慢的背离最初的理念。随着这么多年的发展，微服务、分布式链路跟踪等更多新的技术理念的出现，Spring急需一款框架来改善以前的开发模式，因此才会出现Spring Boot/Cloud项目，现在访问Spring官网，会发现Spring Boot和Spring Cloud已经放到首页最重点突出的三个项目中的前两个，可见Spring对这两个框架的重视程度。Dubbo实现如下：

Spring Cloud实现思路：<br />![2021-05-29-22-51-12-393109.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622300106581-ce540650-488d-438c-9af5-79d83616b922.png#clientId=u99e9257c-3b9a-4&from=ui&id=u8020277d&originHeight=410&originWidth=1080&originalType=binary&size=102356&status=done&style=shadow&taskId=ub9ab0d19-e3cf-46fe-a409-13323e9573b)<br />Spring Cloud架构的各个组件的原理分析
<a name="uDBXj"></a>
## Eureka
Spring Cloud架构的各个组件的原理分析<br />原理：主管服务注册与发现，也就是微服务的名称注册到Eureka，就可以通过Eureka找到微服务，而不需要修改服务调用的配置文件。<br />分析：Spring Cloud封装了Netflix公司开发的Eureka模块来实现服务的注册与发现，采用的c-s的设计架构，Eureka Server作为服务注册功能的服务器，他是服务注册中心。而系统的其他微服务，使用Eureka的客户端连接到Eureka Server并维持心跳。这样系统的维护人员可以通过Eureka Server来监控系统中的各个微服务是否正常运行。Spring Cloud的一些其他模块（比如Zuul）就可以通过Eureka Server来发现系统其他的微服务，并执行相关逻辑。
<a name="nIZ9n"></a>
### Eureka Server
Eureka Server提供服务注册服务，各个节点启动后，会在Eureka Server中进行注册， 这样Eureka Server中的服务注册表中将会存储所有可用服务节点的信息，服务节点的信息可以在界面中直观的看到。
<a name="jeGAU"></a>
### Eureka Client
Eureka Client是一个Java客户端， 用于简化Eureka Server的交互，客户端同时也具备一个内置的、 使用轮询（round-robin）负载算法的负载均衡器。在应用启动后，将会向Eureka Server发送心跳（默认周期为30秒），以证明当前服务是可用状态。如果Eureka Server在一定的时间（默认90秒）未收到客户端的心跳，Eureka Server将会从服务注册表中把这个服务节点移除。
<a name="KpAZz"></a>
### Eureka Server的自我保护机制
如果在15分钟内超过85%的节点都没有正常的心跳，那么Eureka就认为客户端与注册中心出现了网络故障，此时会出现以下几种情况：

- Eureka不再从注册列表中移除因为长时间没收到心跳而应该过期的服务
- Eureka仍然能够接受新服务的注册和查询请求，但是不会被同步到其它节点上（即保证当前节点依然可用）
- 当网络稳定时，当前实例新的注册信息会被同步到其它节点中

因此， Eureka可以很好的应对因网络故障导致部分节点失去联系的情况，而不会像ZooKeeper那样使整个注册服务瘫痪。
<a name="Ipiip"></a>
### Eureka和ZooKeeper
著名的CAP理论指出，一个分布式系统不可能同时满足C（一致性）、A（可用性）和P（分区容错性）。由于分区容错性在是分布式系统中必须要保证的，因此只能在A和C之间进行权衡。
<a name="VxQ49"></a>
### ZooKeeper保证CP
当向注册中心查询服务列表时，可以容忍注册中心返回的是几分钟以前的注册信息，但不能接受服务直接down掉不可用。也就是说，服务注册功能对可用性的要求要高于一致性。但是ZooKeeper会出现这样一种情况，当Master节点因为网络故障与其他节点失去联系时，剩余节点会重新进行leader选举。问题在于，选举leader的时间太长，30 ~ 120s，且选举期间整个ZooKeeper集群都是不可用的，这就导致在选举期间注册服务瘫痪。在云部署的环境下，因网络问题使得ZooKeeper集群失去Master节点是较大概率会发生的事，虽然服务能够最终恢复，但是漫长的选举时间导致的注册长期不可用是不能容忍的。
<a name="Mj0nM"></a>
### Eureka保证AP
Eurek在设计时就优先保证可用性。Eureka各个节点都是平等的，几个节点挂掉不会影响正常节点的工作，剩余的节点依然可以提供注册和查询服务。而Eureka的客户端在向某个Eureka注册或时如果发现连接失败，则会自动切换至其它节点，只要有一台Eureka还在，就能保证注册服务可用（保证可用性），只不过查到的信息可能不是最新的（不保证强一致性）。<br />除此之外，Eureka还有一种自我保护机制，见上。
<a name="IgA50"></a>
### 总结
Eureka可以很好的应对因网络故障导致部分节点失去联系的情况，而不会像ZooKeeper那样使整个注册服务瘫痪。<br />Eureka作为单纯的服务注册中心来说要比ZooKeeper更加“专业”，因为注册服务更重要的是可用性，可以接受短期内达不到一致性的状况。<br />![2021-05-29-22-51-12-840242.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622300157776-3b59a6bf-2d07-47aa-8583-9aee343aeec7.png#clientId=u99e9257c-3b9a-4&from=ui&id=uf712f30f&originHeight=812&originWidth=1080&originalType=binary&size=302069&status=done&style=shadow&taskId=ue44cd680-d212-47a0-8a80-2369d90f1fe)<br />Spring Cloud架构的各个组件的原理分析
<a name="G1cfF"></a>
## Ribbon和Feign
Spring Cloud架构的各个组件的原理分析<br />在微服务架构中，业务都会被拆分成一个独立的服务，服务与服务的通讯是基于HTTP RESTful的。Spring Cloud有两种服务调用方式，一种是Ribbon+RestTemplate，另一种是Feign。
<a name="PDfMT"></a>
### 概念
基于Netflix Ribbon用过轮询策略实现的一套客户端负载均衡的工具。<br />客户端负载均衡：负载均衡Zuul网关将一个请求发送给某一个服务的应用的时候，如果一个服务启动了多个实例，就会通过Ribbon来通过一定的负载均衡策略来发送给某一一个服务实例。Spring Cloud中的Ribbon，客户端会有一个服务器地址列表，在发送请求前通过负载均衡算法（如简单轮询，随机连接等）选择一个服务器，然后进行访问。
<a name="MYpWx"></a>
### 负载均衡

- 负载均衡：用于将工作负载分布到多个服务器来提高网站、应用、数据库或其他服务的性能和可靠性。
- 使用负载均衡带来的好处很明显：当集群里的1台或者多台服务器down的时候，剩余的没有down的服务器可以保证服务的继续使用；将访问压力分配到各个服务器，不会由于某一高峰时刻导致系统cpu急剧上升。
- 负载均衡有好几种实现策略，常见的有：随机（Random），轮询（RoundRobin），一致性哈希（ConsistentHash），哈希（Hash），加权（Weighted）
- Ribbon的默认策略是轮询
<a name="mgINT"></a>
### RestTemplate
传统情况下在Java代码里访问RESTful服务，一般使用Apache的HttpClient，不过此种方法使用起来太过繁琐。Spring提供了一种简单便捷的模板类来进行操作，这就是RestTemplate。<br />Feign是一个声明式http客户端。使用Feign能让编写http客户端更加简单，它的使用方法是定义一个接口，然后在上面添加注解，避免了调用目标微服务时，需要不断的解析/封装json数据的繁琐。Spring Cloud中Feign默认集成了Ribbon，并和Eureka结合，默认实现了负载均衡的效果。
<a name="A9xY9"></a>
### Ribbon和Feign的区别
<a name="int5H"></a>
#### Feign目标使编写Java Http客户端变得更容易
在使用Ribbon+ RestTemplate时，Ribbon需要自己构建http请求，模拟http请求然后使用RestTemplate发送给其他服务，步骤相当繁琐。利用RestTemplate对http请求的封装处理，形成了-套模版化的调用方法。但是在实际开发中，由于对服务依赖的调用可能不止一处,往往一个接口会被多处调用，所以通常都会针对每个微服务自行封装一些客户端类来包装这些依赖服务的调用。所以，Feign在此基础上做了进一步封装，由他来定义和实现依赖服务接口的定义。<br />在Feign的实现下，只需创建一个接口并使用注解的方式来配置它（以前是Dao接口上面标注Mapper注解，现在是一个微服务接口上面标注一个Feign注解即可）， 即可完成对服务提供方的接口绑定，简化了使用Spring Cloud Ribbon时，自动封装服务调用客户端的开发量。
<a name="PFlL8"></a>
#### Feign集成了Ribbon
Ribbon通过轮询实现了客户端的负载均衡，而与Ribbon不同的是，Feign是一个声明式的Web服务客户端， 使得编写Web服务客户端变得非常容易，只需要创建一个接口， 然后在上面添加注解，像调用本地方法一样调用它就可以，而感觉不到是调用远程方法。SpringCloud中Feign默认集成了Ribbon，并和Eureka结合，默认实现了负载均衡的效果。
<a name="oEQ7B"></a>
### Ribbon和Nginx的区别
<a name="krZon"></a>
#### 服务器端负载均衡Nginx
Nginx是客户端所有请求统一交给Nginx，由Nginx进行实现负载均衡请求转发，属于服务器端负载均衡。既请求由Nginx服务器端进行转发。客户端负载均衡Ribbon，Ribbon是从Eureka注册中心服务器端上获取服务注册信息列表，缓存到本地，然后在本地实现轮询负载均衡策略。既在客户端实现负载均衡。
<a name="UlrBh"></a>
### 应用场景的区别
Nginx适合于服务器端实现负载均衡，比如：Tomcat，Ribbon适合与在微服务中RPC远程调用实现本地服务负载均衡，比如：Dubbo、Spring Cloud中都是采用本地负载均衡。<br />![2021-05-29-22-51-13-107531.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622300182872-01752cef-3f38-4aa5-8b38-3d4adc5ac749.jpeg#clientId=u99e9257c-3b9a-4&from=ui&id=ubfba5562&originHeight=1030&originWidth=1080&originalType=binary&size=85139&status=done&style=shadow&taskId=uf0859424-5997-40da-8311-d2cdb7cab1e)<br />Spring Cloud架构的各个组件的原理分析
<a name="nYGEe"></a>
## Zuul
Spring Cloud架构的各个组件的原理分析
<a name="BIPv5"></a>
### 应用场景
假如当前有十几个微服务服务，订单，商品，用户等等，显然是客户端不需要和每个服务逐一打交道，这就需要有一个统一入口，它就是服务网关。API网关所有的客户端请求通过这个网关访问后台的服务。他可以使用一定的路由配置来判断某一个URL由哪个服务来处理。并从Eureka获取注册的服务来转发请求。
<a name="IFoKi"></a>
### 核心功能
Zuul包含了对请求的路由和过滤两个最主要的功能，是各种服务的统一入口，同时还会用来提供监控、授权、安全、调度等等。<br />路由功能：负责将外部请求转发到具体的微服务实例上，是实现外部访问统一入口的基础。<br />过滤器功能：则负责对请求的处理过程进行干预，是实现请求校验、服务聚合等功能的基础。<br />Zuul和Eureka进行整合：将Zuul自身注册为Eureka服务治理下的应用，同时从Eureka中获得其他微服务的消息,也即以后的访问微服务都是通过Zuul跳转后获得。<br />注意：Zuul服务最终还是会注册进Eureka，提供代理+路由+过滤三大功能。
<a name="IMQrU"></a>
### 核心原理
Zuul的核心是一系列的filters，其作用可以类比Servlet框架的Filter，或者AOP。<br />过滤器之间没有直接进行通信,而是通过Request Context（上下文）进行数据传递。<br />Zuul的过滤器是由Groovy写成，这些过滤器文件被放在Zuul Server上的特定目录下面，Zuul会定期轮询这些目录，修改过的过滤器会动态的加载到Zuul Server中以便过滤请求使用。<br />Zuul负载均衡：Zuul拦截对应的API前缀请求做转发，转发到对应的serverId上，在Eureka服务上同一个serverId可以对应多个服务，也就是说用同一个服务节点不同的端口注册两个实例，但是serverId是一样Zuul做转发的时候会结合eureka-server起到负载均衡的效果。<br />过滤器的种类：

- PRE（前置）：这种过滤器在请求被路由之前调用。可利用这种过滤器实现鉴权、限流、参数校验调整等。
- ROUTING（路由）：这种过滤器将请求路由到微服务。这种过滤器用于构建发送给微服务的请求，并使用Apache HttpClient或Netfilx Ribbon请求微服务。
- POST（后置）：这种过滤器在路由到微服务以后执行。这种过滤器可用来为响应添加标准的HTTP Header、收集统计信息和指标、将响应从微服务发送给客户端、日志等。
- ERROR（错误）：在其他阶段发生错误时执行该过滤器。
<a name="geZfx"></a>
### Zuul和Nginx
Zuul虽然在性能上和Nginx没法比，但它也有它的优点。Zuul提供了认证鉴权，动态路由，监控，弹性，安全，负载均衡等边缘服务，在团队规模不大的情况下，没有专门负责路由开发时，使用Zuul当网关是一个快速上手的好方案。<br />Nginx和Zuul是可以配合使用的，发挥各自的优点，使用Nginx作为负载均衡实现高并发的请求转发，Zuul用作网关。
<a name="m5DvH"></a>
### Zuul和Ribbon实现负载均衡
Zuul支持Ribbon和Hystrix，也能够实现客户端的负载均衡。Feign不也是实现客户端的负载均衡和Hystrix的吗？既然Zuul已经能够实现了，那Feign还有必要吗？<br />可以这样理解：<br />Zuul是对外暴露的唯一接口相当于路由的是controller的请求，而Ribbonhe和Fegin路由了service的请求。<br />Zuul做最外层请求的负载均衡，而Ribbon和Fegin做的是系统内部各个微服务的service的调用的负载均衡。
<a name="DvMF4"></a>
## Hystrix
Spring Cloud架构的各个组件的原理分析
<a name="Z9jEo"></a>
### 介绍
Hystrix是一个用于处理分布式系统的延迟和容错的开源库,在分布式系统里,许多依赖不可避兔的会调用失败，比如超时、异常等，Hystrix能够保证在一个依赖出问题的情况下，不会导致整体服务失败，避免级联故障，以提高分布式系统的弹性。Hystrix的出现就是为了解决雪崩效应。
<a name="kR7Nr"></a>
### 服务雪崩
多个微服务之间调用的时候，假设微服务A调用微服务B和微服务C,微服务B和微服务C又调用其它的微服务，这就是所谓的“扇出”。如果扇出的链路上某个微服务的调用响应时间过长或者不可用，对微服务A的调用就会占用越来越多的系统资源，进而引起系统崩溃，所谓的”雪崩效应”。
<a name="u7Gmc"></a>
### 服务熔断
熔断机制是应对雪崩效应的一种微服务链路保护机制。<br />当删除链路的某个微服务不可用或者响应时间太长时，会进行服务的降级，进而熔断该节点微服务的调用，快速返回”错误的响应信息。当检测到该节点微服务调用响应正常后恢复调用链路。在SpringCloud框架里熔断机制通过Hystrix实现。Hystrix会监控微服务间调用的状况，当失败的调用到一定阈值,缺省是5秒内20次调用失败就会启动熔断机制。熔断机制的注解是`@HystrixCommand`。
<a name="Q02lU"></a>
### 服务降级
整体资源快不够了，忍痛将某些服务先关掉，待渡过难关，再开启回来。
<a name="ryyWq"></a>
### Hystrix监控和断路器
只需要在服务接口上添加Hystrix标签，就可以实现对这个接口的监控和断路器功能。<br />Hystrix Dashboard监控面板，提供了一个界面，可以监控各个服务上的服务调用所消耗的时间等。
<a name="kY5MI"></a>
### Hystrix Turbine监控聚合
使用Hystrix监控，需要打开每一个服务实例的监控信息来查看。而Turbine可以把所有的服务实例的监控信息聚合到一个地方统查看。这样就不需要挨个打开一个个的页面一个个查看。
<a name="DUdRD"></a>
### Zuul的安全机制
签名机制，为防止接口数据篡改和重复调用，增加接口参数校验机制，sig签名算法为MD5（appKey+appSecret+timestamp），appKey是分配给客户端的ID，appSecret是分配给客户端的密钥，timestamp为unix时间戳，请求的URL有效时间为15分钟。<br />Token机制，用户在登录之后会返回一个access_ token，客户端在访问需要登录之后才能访问的资源，需要在在Authorization头部使用Bearer模式新增token，如head(“Authorization”,” Bearer token”)。
<a name="cHcOD"></a>
### Hystrix的设计原则

- 资源隔离（线程池隔离和信号量隔离）机制：限制调用分布式服务的资源使用，某一个调用的服务出现问题不会影响其它服务调用。
- 限流机制：限流机制主要是提前对各个类型的请求设置最高的QPS阈值，若高于设置的阈值则对该请求直接返回，不再调用后续资源。
- 熔断机制：当失败率达到阀值自动触发降级（如因网络故障、超时造成的失败率真高），熔断器触发的快速失败会进行快速恢复。
- 降级机制：超时降级、资源不足时（线程或信号量）降级、运行异常降级等，降级后可以配合降级接口返回托底数据。
- 缓存支持：提供了请求缓存、请求合并实现。
- 通过近实时的统计/监控/报警功能，来提高故障发现的速度。
- 通过近实时的属性和配置热修改功能，来提高故障处理和恢复的速度。
<a name="bhCr3"></a>
## Config
Spring Cloud架构的各个组件的原理分析
<a name="KQHLb"></a>
### 介绍
Spring Cloud Config是一个解决分布式系统的配置管理方案。微服务意味着要将单体应用中的业务拆分成一个个子服务，每个服务的粒度相对较小，因此系统 中会出现大量的服务。由于每个服务都需要必要的配置信息才能运行，所以一套集中式的、 动态的配置管理设施是必不可少的。Spring Cloud提供了ConfigServer来解决这个问题，每一个微服务自 己带着一个application.yml 上百个配置文件的管理。
<a name="SXuO4"></a>
### 应用场景

- 不方便维护，多人同时对配置文件进行修改，冲突不断，很难维护
- 配置内容安全和权限，主要是针对线上的配置来说，一般不对开发公开，只有运维有权限所以需要将配置文件隔离，不放到项目代码里
- 更新配置项目需要重启，每次更新配置文件都需要重启项目，很耗时。使用了配置中心后，即可实现配置实时更新congfig Server和Config Client结合Spring Cloud Bus实现配置自动刷新。
<a name="JXnpC"></a>
### 原理

- 配置文件存储在远端Git（比如GitHub，Gitee等仓库），config-server从远端Git拉取配置文件，并保存到本地Git。
- 本地Git和config-server的交互是双向的，因为当远端Git无法访问时，会从本地Git获取配置文件。
- config-client（即各个微服务），从config-server拉取配置文件。
<a name="IwH3i"></a>
### 角色

- Config Server：提供配置文件的存储、以接口的形式将配置文件的内容提供出去。
- Config Client：通过接口获取数据、并依据此数据初始化自己的应用。
<a name="FK4dY"></a>
## 总结如下
![2021-05-29-22-51-13-616895.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622300207049-609f9d4a-0ba4-4b1f-8d90-d2d22314a968.png#clientId=u99e9257c-3b9a-4&from=ui&id=u1f26ef36&originHeight=1920&originWidth=1080&originalType=binary&size=378639&status=done&style=shadow&taskId=ub791fab7-6ee3-4e8a-8e30-628f36de2a6)
