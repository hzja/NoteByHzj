Java SpringCloud Sleuth<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158635654-3dadaae8-a9b5-4aa5-a378-cd89fa081a5b.webp#clientId=u6d702e88-fadc-4&from=paste&id=u0bdac4ac&originHeight=643&originWidth=966&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u02c6805f-0c4d-4f31-b933-36c3641f416&title=)
<a name="zyU6X"></a>
## 为什么需要链路追踪？
大型分布式微服务系统中，一个系统被拆分成N多个模块，这些模块负责不同的功能，组合成一套系统，最终可以提供丰富的功能。在这种分布式架构中，一次请求往往需要涉及到多个服务，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158165717-fd6fdfbd-40c5-4906-9dc5-e9ee8bbf75df.webp#clientId=u6d702e88-fadc-4&from=paste&id=ube264a3a&originHeight=680&originWidth=897&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6205e4d0-6905-48b5-a978-319608a06c6&title=)<br />服务之间的调用错综复杂，对于维护的成本成倍增加，势必存在以下几个问题：

- 服务之间的依赖与被依赖的关系如何能够清晰的看到？
- 出现异常时如何能够快速定位到异常服务？
- 出现性能瓶颈时如何能够迅速定位哪个服务影响的？

为了能够在分布式架构中快速定位问题，分布式链路追踪应运而生。将一次分布式请求还原成调用链路，进行日志记录，性能监控并将一次分布式请求的调用情况集中展示。比如各个服务节点上的耗时、请求具体到达哪台机器上、每个服务节点的请求状态等等。
<a name="GbfCp"></a>
## 常见的链路追踪技术有哪些？
市面上有很多链路追踪的项目，其中也不乏一些优秀的，如下：

- cat：由大众点评开源，基于Java开发的实时应用监控平台，包括实时应用监控，业务监控 。集成方案是通过代码埋点的方式来实现监控，比如：拦截器，过滤器等。对代码的侵入性很大，集成成本较高，风险较大。
- zipkin：由Twitter公司开源，开放源代码分布式的跟踪系统，用于收集服务的定时数据，以解决微服务架构中的延迟问题，包括：数据的收集、存储、查找和展现。该产品结合spring-cloud-sleuth使用较为简单， 集成很方便， 但是功能较简单。
- pinpoint：韩国人开源的基于字节码注入的调用链分析，以及应用监控分析工具。特点是支持多种插件，UI功能强大，接入端无代码侵入
- skywalking：SkyWalking是本土开源的基于字节码注入的调用链分析，以及应用监控分析工具。特点是支持多种插件，UI功能较强，接入端无代码侵入。目前已加入Apache孵化器。
- Sleuth：SpringCloud 提供的分布式系统中链路追踪解决方案。很可惜的是阿里系并没有链路追踪相关的开源项目，可以采用Spring Cloud Sleuth+Zipkin来做链路追踪的解决方案。
<a name="jcFwH"></a>
## Spring Cloud Sleuth是什么？
Spring Cloud Sleuth实现了一种分布式的服务链路跟踪解决方案，通过使用Sleuth可以快速定位某个服务的问题。简单来说，Sleuth相当于调用链监控工具的客户端，集成在各个微服务上，负责产生调用链监控数据。<br />Spring Cloud Sleuth只负责产生监控数据，通过日志的方式展示出来，并没有提供可视化的UI界面。<br />学习Sleuth之前必须了解它的几个概念：

- Span：基本的工作单元，相当于链表中的一个节点，通过一个唯一ID标记它的开始、具体过程和结束。可以通过其中存储的开始和结束的时间戳来统计服务调用的耗时。除此之外还可以获取事件的名称、请求信息等。
- Trace：一系列的Span串联形成的一个树状结构，当请求到达系统的入口时就会创建一个唯一ID（traceId），唯一标识一条链路。这个traceId始终在服务之间传递，直到请求的返回，那么就可以使用这个traceId将整个请求串联起来，形成一条完整的链路。
- Annotation：一些核心注解用来标注微服务调用之间的事件，重要的几个注解如下：
   - cs(Client Send)：客户端发出请求，开始一个请求的生命周期
   - sr（Server Received）：服务端接受请求并处理；sr-cs = 网络延迟 = 服务调用的时间
   - ss（Server Send）：服务端处理完毕准备发送到客户端；ss - sr = 服务器上的请求处理时间
   - cr（Client Reveived）：客户端接受到服务端的响应，请求结束；cr - sr = 请求的总时间
<a name="PtB87"></a>
## Spring Cloud 如何整合Sleuth？
整合Spring Cloud Sleuth其实没什么的难的，在这之前需要准备以下三个服务：

- gateway-sleuth9031：作为网关服务
- sleuth-product9032：商品微服务
- sleuth-order9033：订单微服务

三个服务的调用关系如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158165451-a7b10a0d-9123-44dd-918b-bfe77c512911.webp#clientId=u6d702e88-fadc-4&from=paste&id=u2b6cdb3b&originHeight=439&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc06bbc6e-b143-4512-87e6-e9c7425f516&title=)<br />客户端请求网关发起查询订单的请求，网关路由给订单服务，订单服务获取订单详情并且调用商品服务获取商品详情。
<a name="QI2Fc"></a>
### 添加依赖
在父模块中添加sleuth依赖，如下：
```xml
<dependency>
   <groupId>org.springframework.cloud</groupId>
   <artifactId>spring-cloud-starter-sleuth</artifactId>
</dependency>
```
以上只是Spring Cloud Sleuth的依赖，还有Nacos，openFeign的依赖这里就不再详细说了。
<a name="q9ocW"></a>
### 调整日志级别
由于sleuth并没有UI界面，因此需要调整一下日志级别才能在控制台看到更加详细的链路信息。<br />在三个服务的配置文件中添加以下配置：
```yaml
## 设置openFeign和sleuth的日志级别为debug，方便查看日志信息
logging:
  level:
    org.springframework.cloud.openfeign: debug
    org.springframework.cloud.sleuth: debug
```
<a name="MWPqH"></a>
### 演示接口完善
以下接口只是为了演示造的数据，并没有整合DB。<br />sleuth-order9033查询订单详情的接口，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158165302-58539f9e-4d52-40b4-844b-7048f84546df.webp#clientId=u6d702e88-fadc-4&from=paste&id=u4dc65d08&originHeight=312&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u46927f2a-f464-4968-b61f-e4c14df1bed&title=)<br />sleuth-product9032的查询商品详情的接口，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158165447-2f9941b8-8440-414d-9e0b-d951ad90c08b.webp#clientId=u6d702e88-fadc-4&from=paste&id=uc2be7073&originHeight=211&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue04ae61a-ffa9-4b74-8bf4-2ab98c9f45f&title=)<br />gateway-sleuth9031网关路由配置如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158165404-3f367093-7644-4510-ada0-84ede8c320d6.webp#clientId=u6d702e88-fadc-4&from=paste&id=u2263c8e1&originHeight=367&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub52dc715-66a8-485a-acb8-bdc8ad8cda0&title=)
<a name="XAHUv"></a>
### 测试
启动上述三个服务，浏览器直接访问：http://localhost:9031/order/get/12<br />观察控制台日志输出，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158165941-923c60a9-c468-4d1c-bef9-3a85eb93037e.webp#clientId=u6d702e88-fadc-4&from=paste&id=ubbee5126&originHeight=172&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u49583217-e2c5-47ca-a795-0fcfb282393&title=)<br />日志格式中总共有四个参数，含义分别如下：

- 第一个：服务名称
- 第二个：traceId，唯一标识一条链路
- 第三个：spanId，链路中的基本工作单元id
- 第四个：表示是否将数据输出到其他服务，true则会把信息输出到其他可视化的服务上观察，这里并未整合zipkin，所以是false
<a name="R8rXF"></a>
## 什么是ZipKin？
Zipkin 是 Twitter 的一个开源项目，它基于Google Dapper实现，它致力于收集服务的定时数据，<br />以解决微服务架构中的延迟问题，包括数据的收集、存储、查找和展现。<br />ZipKin的基础架构如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158166460-23b6ac3e-fe48-4668-9e26-d0febad9de06.webp#clientId=u6d702e88-fadc-4&from=paste&id=u6def9e19&originHeight=484&originWidth=809&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub2b0009f-b864-452b-a33f-fb97f1701df&title=)<br />Zipkin共分为4个核心的组件，如下：

- Collector：收集器组件，它主要用于处理从外部系统发送过来的跟踪信息，将这些信息转换为Zipkin内部处理的 Span 格式，以支持后续的存储、分析、展示等功能。
- Storage：存储组件，它主要对处理收集器接收到的跟踪信息，默认会将这些信息存储在内存中，也可以修改此存储策略，通过使用其他存储组件将跟踪信息存储到数据库中
- RESTful API：API 组件，它主要用来提供外部访问接口。比如给客户端展示跟踪信息，或是外接系统访问以实现监控等。
- UI：基于API组件实现的上层应用。通过UI组件用户可以方便而有直观地查询和分析跟踪信息

zipkin分为服务端和客户端，服务端主要用来收集跟踪数据并且展示，客户端主要功能是发送给服务端，微服务的应用也就是客户端，这样一旦发生调用，就会触发监听器将sleuth日志数据传输给服务端。
<a name="EJsqN"></a>
## zipkin服务端如何搭建？
首先需要下载服务端的jar包，地址：[https://search.maven.org/artifact/io.zipkin/zipkin-server/2.23.4/jar](https://search.maven.org/artifact/io.zipkin/zipkin-server/2.23.4/jar)<br />下载完成将会得到一个jar包，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158165930-577539af-f394-4384-a7bd-9a5f1550c2d9.webp#clientId=u6d702e88-fadc-4&from=paste&id=ucc12786e&originHeight=149&originWidth=823&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8f714509-bd57-41dc-926e-fcbced53d7f&title=)<br />直接启动这个jar，命令如下：
```bash
java -jar zipkin-server-2.23.4-exec.jar
```
出现以下界面表示启动完成：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158166225-351d1d6a-c3b0-43f5-803d-7365a13180ae.webp#clientId=u6d702e88-fadc-4&from=paste&id=ua6da8aa0&originHeight=535&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud549c556-e1d0-4a0a-91df-36ecf9e5a2c&title=)<br />此时可以访问zipkin的UI界面，地址：http://localhost:9411，界面如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158166365-1d15b5b7-ee1d-4912-8005-50931bcf01c7.webp#clientId=u6d702e88-fadc-4&from=paste&id=u655695ec&originHeight=374&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8f6eaeb1-e38d-4af7-85d3-349982fb4d2&title=)<br />以上是通过下载jar的方式搭建服务端，当然也有其他方式安装，比如docker。
<a name="LyGNF"></a>
## zipKin客户端如何搭建？
服务端只是跟踪数据的收集和展示，客户端才是生成和传输数据的一端，下面详细介绍一下如何搭建一个客户端。<br />还是上述例子的三个微服务，直接添加zipkin的依赖，如下：
```xml
<!--链路追踪 zipkin依赖，其中包含Sleuth的依赖-->
<dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-starter-zipkin</artifactId>
</dependency>
```
注意：由于spring-cloud-starter-zipkin中已经包含了Spring Cloud Sleuth依赖，因此只需要引入上述一个依赖即可。<br />配置文件需要配置一下zipkin服务端的地址，配置如下：
```yaml
spring:
  cloud:
  sleuth:
    sampler:
      # 日志数据采样百分比，默认0.1(10%)，这里为了测试设置成了100%，生产环境只需要0.1即可
      probability: 1.0
  zipkin:
      #zipkin server的请求地址
    base-url: http://127.0.0.1:9411
      #让nacos把它当成一个URL，而不要当做服务名
    discovery-client-enabled: false
```
上述配置完成后启动服务即可，此时访问：http://localhost:9031/order/get/12<br />调用接口之后，再次访问zipkin的UI界面，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158166447-b2d22a90-45bf-4ff4-b41c-9500fcaa6337.webp#clientId=u6d702e88-fadc-4&from=paste&id=u4666448e&originHeight=408&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3993ab91-1ac8-44a2-8b4c-01c86168b32&title=)<br />可以看到刚才调用的接口已经被监控到了，点击SHOW进入详情查看，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158166475-41f97875-cee6-415e-bfab-9ec2f2c2f451.webp#clientId=u6d702e88-fadc-4&from=paste&id=u004f4960&originHeight=443&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u69b03827-d9ef-428c-865e-be289a2a475&title=)<br />可以看到左边展示了一条完整的链路，包括服务名称、耗时，右边展示服务调用的相关信息，包括开始、结束时间、请求url，请求方式.....<br />除了调用链路的相关信息，还可以清楚看到每个服务的依赖如下图，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158166913-1adaec5b-8adf-4289-a458-786f64309ffc.webp#clientId=u6d702e88-fadc-4&from=paste&id=u621ba02c&originHeight=378&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf51cdb21-c415-4ded-8539-85c16bf8c61&title=)
<a name="l3IaZ"></a>
## ZipKin的数据传输方式如何切换？
zipkin默认的传输方式是HTTP，但是这里存在一个问题，一旦传输过程中客户端和服务端断掉了，那么这条跟踪日志信息将会丢失。<br />当然zipkin还支持MQ方式的传输，支持消息中间件有如下几种：

- ActiveMQ
- RabbitMQ
- Kafka

使用MQ方式传输不仅能够保证消息丢失的问题，还能提高传输效率，生产中推荐MQ传输方式。<br />那么问题来了，如何切换呢？<br />其实方式很简单，下面以RabbitMQ为例介绍一下。
<a name="RS6ln"></a>
### 1、服务端连接RabbitMQ
运行服务端并且连接RabbitMQ，命令如下：
```bash
java -jar zipkin-server-2.23.4-exec.jar --zipkin.collector.rabbitmq.addresses=localhost --zipkin.collector.rabbitmq.username=guest --zipkin.collector.rabbitmq.password=guest
```
命令分析如下：

- zipkin.collector.rabbitmq.addresses：MQ地址
- zipkin.collector.rabbitmq.username：用户名
- zipkin.collector.rabbitmq.password：密码
<a name="gIwKJ"></a>
### 2、客户端添加RabbitMQ
既然使用MQ传输，肯定是要添加对应的依赖和配置了，添加RabbitMQ依赖如下：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-amqp</artifactId>
</dependency>
```
配置MQ的地址、用户名、密码，配置如下：
```yaml
spring:
  rabbitmq:
    addresses: 127.0.0.1
    username: guest
    password: guest
```
<a name="AAkqY"></a>
### 3、配置文件中传输方式切换
`spring.cloud.zipkin.sender.type`这个配置就是用来切换传输方式的，取值为`rabbit`则表示使用rabbitMQ进行数据传输。<br />配置如下：
```yaml
spring:
  cloud:
  zipkin:
    sender:
     ## 使用rabbitMQ进行数据传输
      type: rabbit
```
注意：使用MQ传输，则`spring.cloud.zipkin.sender.base-url`可以去掉。<br />完整的配置如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158167110-6eaa380d-b024-415d-8078-cadb86ac60ac.webp#clientId=u6d702e88-fadc-4&from=paste&id=u9eff9a35&originHeight=660&originWidth=861&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u949f6775-bfc0-4689-9c1b-467e0e17d2b&title=)
<a name="iCBIP"></a>
### 4、测试
既然使用MQ传输，那么不启动服务端也是能够成功传输的，浏览器访问：http://localhost:9031/order/get/12<br />此时发现服务并没有报异常，在看RabbitMQ中已经有数据传输过来了，存在zipkin这个队列中，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158166849-9c0a96c3-fbb6-4ad7-9520-d0cbc405f6ec.webp#clientId=u6d702e88-fadc-4&from=paste&id=ud92e506e&originHeight=398&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc7464a26-ea61-4015-9645-6ccf96bcd3d&title=)<br />可以看到有消息未被消费，点进去可以看到消息内容就是Trace、Span相关信息。<br />好了，启动服务端，命令如下：
```bash
java -jar zipkin-server-2.23.4-exec.jar --zipkin.collector.rabbitmq.addresses=localhost --zipkin.collector.rabbitmq.username=guest --zipkin.collector.rabbitmq.password=guest
```
服务端启动后发现zipkin队列中的消息瞬间被消费了，查看zipkin的UI界面发现已经生成了链路信息，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158166918-b7bc10f3-bcb7-45db-80f7-886c707662e1.webp#clientId=u6d702e88-fadc-4&from=paste&id=u0d6d2634&originHeight=322&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1e95d1a1-d870-4397-ac05-f30bd977bc1&title=)
<a name="YmZ0D"></a>
## zipkin如何持久化？
zipkin的信息默认是存储在内存中，服务端一旦重启信息将会丢失，但是zipkin提供了可插拔式的存储。<br />zipkin支持以下四种存储方式：

- 内存：服务重启将会失效，不推荐
- MySQL：数据量越大性能较低
- Elasticsearch：主流的解决方案，推荐使用
- Cassandra：技术太牛批，用的人少，自己选择，不过官方推荐

这里以MySQL为例介绍一下zipkin如何持久化。
<a name="sp5gk"></a>
### 1、创建数据库
zipkin服务端的MySQL建表SQL：
```sql
CREATE TABLE IF NOT EXISTS zipkin_spans (
  `trace_id_high` BIGINT NOT NULL DEFAULT 0 COMMENT 'If non zero, this means the trace uses 128 bit traceIds instead of 64 bit',
  `trace_id` BIGINT NOT NULL,
  `id` BIGINT NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `remote_service_name` VARCHAR(255),
  `parent_id` BIGINT,
  `debug` BIT(1),
  `start_ts` BIGINT COMMENT 'Span.timestamp(): epoch micros used for endTs query and to implement TTL',
  `duration` BIGINT COMMENT 'Span.duration(): micros used for minDuration and maxDuration query',
  PRIMARY KEY (`trace_id_high`, `trace_id`, `id`)
) ENGINE=InnoDB ROW_FORMAT=COMPRESSED CHARACTER SET=utf8 COLLATE utf8_general_ci;

ALTER TABLE zipkin_spans ADD INDEX(`trace_id_high`, `trace_id`) COMMENT 'for getTracesByIds';
ALTER TABLE zipkin_spans ADD INDEX(`name`) COMMENT 'for getTraces and getSpanNames';
ALTER TABLE zipkin_spans ADD INDEX(`remote_service_name`) COMMENT 'for getTraces and getRemoteServiceNames';
ALTER TABLE zipkin_spans ADD INDEX(`start_ts`) COMMENT 'for getTraces ordering and range';

CREATE TABLE IF NOT EXISTS zipkin_annotations (
  `trace_id_high` BIGINT NOT NULL DEFAULT 0 COMMENT 'If non zero, this means the trace uses 128 bit traceIds instead of 64 bit',
  `trace_id` BIGINT NOT NULL COMMENT 'coincides with zipkin_spans.trace_id',
  `span_id` BIGINT NOT NULL COMMENT 'coincides with zipkin_spans.id',
  `a_key` VARCHAR(255) NOT NULL COMMENT 'BinaryAnnotation.key or Annotation.value if type == -1',
  `a_value` BLOB COMMENT 'BinaryAnnotation.value(), which must be smaller than 64KB',
  `a_type` INT NOT NULL COMMENT 'BinaryAnnotation.type() or -1 if Annotation',
  `a_timestamp` BIGINT COMMENT 'Used to implement TTL; Annotation.timestamp or zipkin_spans.timestamp',
  `endpoint_ipv4` INT COMMENT 'Null when Binary/Annotation.endpoint is null',
  `endpoint_ipv6` BINARY(16) COMMENT 'Null when Binary/Annotation.endpoint is null, or no IPv6 address',
  `endpoint_port` SMALLINT COMMENT 'Null when Binary/Annotation.endpoint is null',
  `endpoint_service_name` VARCHAR(255) COMMENT 'Null when Binary/Annotation.endpoint is null'
) ENGINE=InnoDB ROW_FORMAT=COMPRESSED CHARACTER SET=utf8 COLLATE utf8_general_ci;

ALTER TABLE zipkin_annotations ADD UNIQUE KEY(`trace_id_high`, `trace_id`, `span_id`, `a_key`, `a_timestamp`) COMMENT 'Ignore insert on duplicate';
ALTER TABLE zipkin_annotations ADD INDEX(`trace_id_high`, `trace_id`, `span_id`) COMMENT 'for joining with zipkin_spans';
ALTER TABLE zipkin_annotations ADD INDEX(`trace_id_high`, `trace_id`) COMMENT 'for getTraces/ByIds';
ALTER TABLE zipkin_annotations ADD INDEX(`endpoint_service_name`) COMMENT 'for getTraces and getServiceNames';
ALTER TABLE zipkin_annotations ADD INDEX(`a_type`) COMMENT 'for getTraces and autocomplete values';
ALTER TABLE zipkin_annotations ADD INDEX(`a_key`) COMMENT 'for getTraces and autocomplete values';
ALTER TABLE zipkin_annotations ADD INDEX(`trace_id`, `span_id`, `a_key`) COMMENT 'for dependencies job';

CREATE TABLE IF NOT EXISTS zipkin_dependencies (
  `day` DATE NOT NULL,
  `parent` VARCHAR(255) NOT NULL,
  `child` VARCHAR(255) NOT NULL,
  `call_count` BIGINT,
  `error_count` BIGINT,
  PRIMARY KEY (`day`, `parent`, `child`)
) ENGINE=InnoDB ROW_FORMAT=COMPRESSED CHARACTER SET=utf8 COLLATE utf8_general_ci;
```
创建的数据库：zipkin（名称任意），导入建表SQL，新建的数据库表如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640158167162-7c56e295-d005-4d16-866e-ffbb90c84f27.webp#clientId=u6d702e88-fadc-4&from=paste&id=ud0de7186&originHeight=119&originWidth=919&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5c55c93d-6fe3-4de5-baff-dd8cda98215&title=)
<a name="rV7BO"></a>
### 2、服务端配置MySQL
服务端配置很简单，运行如下命令：
```bash
java -jar zipkin-server-2.23.4-exec.jar --STORAGE_TYPE=mysql --MYSQL_HOST=127.0.0.1 --MYSQL_TCP_PORT=3306 --MYSQL_DB=zipkin --MYSQL_USER=root --MYSQL_PASS=Nov2014
```
上述命令参数分析如下：

- `STORAGE_TYPE`：指定存储的方式，默认内存形式
- `MYSQL_HOST`：MySQL的ip地址，默认localhost
- `MYSQL_TCP_PORT`：MySQL的端口号，默认端口3306
- `MYSQL_DB`：MySQL中的数据库名称，默认是zipkin
- `MYSQL_USER`：用户名
- `MYSQL_PASS`：密码

这些配置都在源码的/zipkin-server/src/main/resources/zipkin-server-shared.yml这个配置文件中，比如上述MySQL的相关配置，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640158402002-6fb16eaa-4ceb-4254-b029-c04a6c8d403d.png#clientId=u6d702e88-fadc-4&from=paste&height=594&id=ub2d797c5&originHeight=1783&originWidth=2676&originalType=binary&ratio=1&rotation=0&showTitle=false&size=515187&status=done&style=none&taskId=u85f34fe4-5834-4354-82b9-a3834fa8c67&title=&width=892)<br />zipkin服务端的所有配置项都在这里。<br />github地址：[https://github.com/openzipkin/zipkin/blob/master/zipkin-server/src/main/resources/zipkin-server-shared.yml](https://github.com/openzipkin/zipkin/blob/master/zipkin-server/src/main/resources/zipkin-server-shared.yml)<br />那么采用rabbitMQ传输方式、MySQL持久化方式，完整的命令如下：
```bash
java -jar zipkin-server-2.23.4-exec.jar --STORAGE_TYPE=mysql --MYSQL_HOST=127.0.0.1 --MYSQL_TCP_PORT=3306 --MYSQL_DB=zipkin --MYSQL_USER=root --MYSQL_PASS=Nov2014 --zipkin.collector.rabbitmq.addresses=localhost --zipkin.collector.rabbitmq.username=guest --zipkin.collector.rabbitmq.password=guest
```
持久化是服务端做的事，和客户端无关。
<a name="qhOze"></a>
## 总结

- Spring Cloud Sleuth 作为链路追踪的一种组件，只提供了日志采集，日志打印的功能，并没有可视化的UI界面
- zipkin提供了强大的日志追踪分析、可视化、服务依赖分析等相关功能，结合Spring Cloud Sleuth作为一种主流的解决方案
- zipkin生产环境建议切换的MQ传输模式，这样做有两个优点
   - 防止数据丢失
   - MQ异步解耦，性能提升很大
- zipkin默认是内存的形式存储，MySQL虽然也是一种方式，但是随着数据量越大，性能越差，因此生产环境建议采用Elasticsearch，下一篇文章介绍。
