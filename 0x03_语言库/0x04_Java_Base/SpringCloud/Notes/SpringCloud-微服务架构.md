Java SpringCloud
<a name="B5yXL"></a>
## 1、SpringCloud-微服务组件
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575784378295-25538b90-74b2-4de1-adb1-115b06692bd9.png#height=648&id=bUX32&originHeight=648&originWidth=720&originalType=binary&ratio=1&rotation=0&showTitle=false&size=30882&status=done&style=shadow&title=&width=720)
<a name="YkqSv"></a>
## 2、SpringCloud常用组件
• 服务治理：Spring Cloud Eureka<br />• 负载均衡：Spring Cloud Ribbon<br />• 熔断限流：Spring Cloud Hystrix<br />• 服务调用：Spring Cloud Feign<br />• 网关服务：Spring Cloud Zuul<br />• 配置中心：Spring Cloud Config<br />• 消息总线：Spring Cloud Bus<br />• 消息驱动：Spring Cloud Stream<br />• 服务追踪：Spring Cloud Sleuth
<a name="OPCGF"></a>
## 3、什么是微服务
<a name="dGDxH"></a>
### 3.1 架构演进
架构的发展历程是从单体式架构，到分布式架构，到SOA架构，再到微服务架构。<br />图1：架构演进<br />![20200810104800618.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371479573-4be9191c-c608-43d9-a705-53b327a13d1f.png#clientId=ub80f9e46-17c7-4&from=ui&id=u0c019781&originHeight=383&originWidth=853&originalType=binary&ratio=1&rotation=0&showTitle=false&size=51827&status=done&style=shadow&taskId=u2c58edeb-d04d-4eff-8f17-d0adac53c8f&title=)

- 单体架构：未做任何拆分的Java Web程序

图2：单体架构示意图<br />![20200810095010767.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371298004-1db8672f-797d-4296-83c2-05c961d4d186.png#clientId=ub80f9e46-17c7-4&from=ui&id=oM3ZS&originHeight=528&originWidth=418&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19389&status=done&style=shadow&taskId=uaf0c17f6-a8b3-4d30-befc-77dae1de6ba&title=)

- 分布式架构:按照业务垂直划分，每个业务都是单体架构，通过API互相调用。

图3：分布式架构示意图<br />![20200810100445816.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371298029-815ea629-01e8-459e-a5e3-c8cf643b0f89.png#clientId=ub80f9e46-17c7-4&from=ui&id=sinJo&originHeight=393&originWidth=600&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42931&status=done&style=shadow&taskId=u4999257b-4c56-422f-8a8e-03b94369e77&title=)

- SOA架构：SOA是一种面向服务的架构。其应用程序的不同组件通过网络上的通信协议向其它组件提供服务或消费服务，所以也是分布式架构的一种。

图4：SOA架构示意图<br />![20200810101312890.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371298161-7e3bd992-25a0-47af-aa46-93d3373de5b7.png#clientId=ub80f9e46-17c7-4&from=ui&id=n89Zl&originHeight=423&originWidth=720&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40070&status=done&style=none&taskId=u402b53ba-f075-4aff-a005-70741546626&title=)
<a name="Y9JWw"></a>
### 3.2 微服务架构
微服务架构在某种程度上是SOA架构的进一步的发展。<br />微服务目前并没有比较官方的定义。微服务 Microservices 之父，马丁.福勒，对微服务大概的概述如下：<br />就目前而言，对于微服务业界并没有一个统一的、标准的定义（While there is no precise definition of this architectural style ) 。<br />但通常在其而言，微服务架构是一种架构模式或者说是一种架构风格，它提倡将单一应用程序划分成一组小的服务，每个服务运行独立的自己的进程中，服务之间互相协调、互相配合，为用户提供最终价值。<br />服务之间采用轻量级的通信机制互相沟通（通常是基于 HTTP 的 RESTful API ) 。每个服务都围绕着具体业务进行构建，并且能够被独立地部署到生产环境、类生产环境等。<br />另外，应尽量避免统一的、集中式的服务管理机制，对具体的一个服务而言，应根据业务上下文，选择合适的语言、工具对其进行构建，可以有一个非常轻量级的集中式管理来协调这些服务。可以使用不同的语言来编写服务，也可以使用不同的数据存储。<br />图5：微服务定义思维导图<br />![20200810102436348.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371298431-18364079-bf6c-403a-80b6-4fe60f5f942b.png#clientId=ub80f9e46-17c7-4&from=ui&id=xlb2L&originHeight=563&originWidth=722&originalType=binary&ratio=1&rotation=0&showTitle=false&size=34345&status=done&style=shadow&taskId=u0dd2ba37-830f-45ce-a41e-afc165d8b48&title=)<br />图6：微服务架构示意图<br />![640.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619012829730-12f3187c-76f5-4e84-b872-c99546c17bac.jpeg#clientId=u7c5cac41-04d5-4&from=ui&id=lkKUl&originHeight=735&originWidth=720&originalType=binary&ratio=1&rotation=0&showTitle=false&size=163317&status=done&style=shadow&taskId=ubfbbeb91-7c9e-4aaf-9424-88f72406e4a&title=)
<a name="C533p"></a>
### 3.3 微服务解决方案
目前最流行的两种微服务解决方案是Spring Cloud和Dubbo。
<a name="pL0zr"></a>
## 4、SpringCloud概览
<a name="A97Hd"></a>
### 4.1 什么是SpringCloud
Spring Cloud 作为 Java 言的微服务框架，它依赖于 Spring Boot ，有快速开发、持续交付和容易部署等特点。Spring Cloud 的组件非常多，涉及微服务的方方面面，井在开源社区 Spring、Netflix Pivotal 两大公司的推动下越来越完善。<br />Spring Cloud是一系列组件的有机集合。<br />图7：SpringCloud技术体系<br />![20200810105241576.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371348277-98016923-c077-48c1-989c-fd51e418510f.png#clientId=ub80f9e46-17c7-4&from=ui&id=u095daceb&originHeight=469&originWidth=963&originalType=binary&ratio=1&rotation=0&showTitle=false&size=340818&status=done&style=shadow&taskId=u52310c0d-9e85-4d15-bc75-95aaf5178cf&title=)<br />图8：SpringCloud技术体系思维导图<br />![20200810112743519.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371345499-047d66ed-76e6-472f-b900-5c89c1311b96.png#clientId=ub80f9e46-17c7-4&from=ui&id=bs0YZ&originHeight=683&originWidth=1247&originalType=binary&ratio=1&rotation=0&showTitle=false&size=116404&status=done&style=shadow&taskId=u8f13a6e0-663c-4f21-940f-f6468c8e57b&title=)
<a name="eo7RF"></a>
### 4.2 SpringCloud主要组件
<a name="JoUah"></a>
#### 1、Eureka
Netflix Eureka 是由 Netflix 开源的一款基于 REST 的服务发现组件，包括 Eureka Server 及 Eureka Client。<br />![20200810120101878.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371345565-16e16765-c3a0-4a6b-b107-9dc2bb612b56.png#clientId=ub80f9e46-17c7-4&from=ui&id=ZbFb0&originHeight=818&originWidth=1256&originalType=binary&ratio=1&rotation=0&showTitle=false&size=115841&status=done&style=shadow&taskId=uf62e41be-655d-4d7f-8677-b1f4d17f8ac&title=)
<a name="D8B8O"></a>
#### 2、Ribbon
Ribbon Netflix 公司开源的一个负载均衡的组件。<br />![20200810174308572.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371345566-9fa15c56-710d-4bc0-83a0-2c7af0f37409.png#clientId=ub80f9e46-17c7-4&from=ui&id=UW0PX&originHeight=755&originWidth=1081&originalType=binary&ratio=1&rotation=0&showTitle=false&size=96731&status=done&style=none&taskId=u3d4e13f9-b6b4-46d4-b8a4-22418d57836&title=)
<a name="mBxqG"></a>
#### 3、Feign
Feign是是一个声明式的Web Service客户端。<br />![20200810182853299.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371412725-be0f8d1d-0f6b-4c2b-ad90-50a7a4ce4491.png#clientId=ub80f9e46-17c7-4&from=ui&id=u9033bf4b&originHeight=823&originWidth=1007&originalType=binary&ratio=1&rotation=0&showTitle=false&size=110732&status=done&style=none&taskId=u38379efd-ab63-4e70-acc2-e174ab4487a&title=)
<a name="df22s"></a>
#### 4、Hystrix
Hystrix是Netstflix 公司开源的一个项目，它提供了熔断器功能，能够阻止分布式系统中出现联动故障。<br />![20200810205126528.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371412901-a9d46275-bfd4-4e57-aa99-94c5e1923ed1.png#clientId=ub80f9e46-17c7-4&from=ui&id=nhOjj&originHeight=1020&originWidth=1113&originalType=binary&ratio=1&rotation=0&showTitle=false&size=156349&status=done&style=none&taskId=udcf9c506-51ca-4d1b-a8e0-ccef6a52c4f&title=)
<a name="kbi2i"></a>
#### 5、Zuul
Zuul 是由 Netflix 孵化的一个致力于“网关 “解决方案的开源组件。<br />![20200810211514805.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371387192-040ca3fa-2bd4-47b8-983e-9ca1ec1b9fae.png#clientId=ub80f9e46-17c7-4&from=ui&id=R3qKH&originHeight=696&originWidth=761&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61697&status=done&style=none&taskId=u6edf2143-213f-44b2-8671-626a20aa780&title=)
<a name="vP4QQ"></a>
#### 6、Gateway
Spring Cloud Gateway 是 Spring 官方基于 Spring 5.0、 Spring Boot 2.0 和 Project Reactor 等技术开发的网关， Spring Cloud Gateway 旨在为微服务架构提供简单、 有效且统一的 API 路由管理方式。<br />![20200810212808923.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371412357-54f19eda-c77d-41aa-8ff7-bf645b2159a6.png#clientId=ub80f9e46-17c7-4&from=ui&id=QfUuY&originHeight=551&originWidth=1000&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68257&status=done&style=none&taskId=u5649df87-624b-4d8c-a406-048bfc884d4&title=)
<a name="dSHlb"></a>
#### 7、Config
Spring Cloud 中提供了分布式配置中 Spring Cloud Config ，为外部配置提供了客户端和服务器端的支持。<br />![20200810214123442.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371423485-2d8a784b-4fc4-412a-b9c3-93d668a8a1cf.png#clientId=ub80f9e46-17c7-4&from=ui&id=J0ygN&originHeight=471&originWidth=806&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41234&status=done&style=none&taskId=ua2a6a788-e0a3-4c24-a359-273945ca7b7&title=)
<a name="sSlqX"></a>
#### 8、Bus
使用 Spring Cloud Bus, 可以非常容易地搭建起消息总线。<br />![20200810223142844.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371423514-cce81794-95f1-4769-8c90-2147f36ef8ce.png#clientId=ub80f9e46-17c7-4&from=ui&id=IRnrZ&originHeight=963&originWidth=896&originalType=binary&ratio=1&rotation=0&showTitle=false&size=102491&status=done&style=none&taskId=u10dcf383-061d-4ea5-bd25-f0178cd2f5c&title=)
<a name="pyZ2v"></a>
#### 9、OAuth2
Sprin Cloud 构建的微服务系统中可以使用 Spring Cloud OAuth2 来保护微服务系统。<br />![20200811205313656.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371423524-8c4bf40d-78cb-4c74-b482-d616f3053783.png#clientId=ub80f9e46-17c7-4&from=ui&id=PF94b&originHeight=653&originWidth=1073&originalType=binary&ratio=1&rotation=0&showTitle=false&size=76165&status=done&style=none&taskId=u66b4b463-d793-47f3-bd93-95b6a8aee86&title=)
<a name="J5fOG"></a>
#### 10、Sleuth
Spring Cloud Sleuth是Spring Cloud 个组件，它的主要功能是在分布式系统中提供服务链路追踪的解决方案。<br />![20200811212137615.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371423528-ed4a2378-eeb5-45ff-be64-c0f51e0fb7f1.png#clientId=ub80f9e46-17c7-4&from=ui&id=iHYY0&originHeight=651&originWidth=820&originalType=binary&ratio=1&rotation=0&showTitle=false&size=66719&status=done&style=none&taskId=ucc85b4da-6068-4ca3-a80f-04a84c64623&title=)<br />这里对架构的演进及Spring Cloud 构建微服务的基本组件进行了概览。<br />![20200811213243542.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638371423738-e2999df8-1ec9-4d91-a474-1bec9863a875.png#clientId=ub80f9e46-17c7-4&from=ui&id=u0693e21a&originHeight=3368&originWidth=2776&originalType=binary&ratio=1&rotation=0&showTitle=false&size=997588&status=done&style=shadow&taskId=u56c2cd0a-e6e1-43f7-b101-5bf59eab3ef&title=)
