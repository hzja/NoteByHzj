Java 微服务 Skywalking<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1645516942635-8396375a-617b-4410-89d1-38cb1d124e6e.jpeg)
<a name="k3kNC"></a>
## Skywalking简介
skywalking是一个优秀的**国产开源框架**，**2015**年由个人吴晟（华为开发者）开源 ， **2017**年加入Apache孵化器。短短两年就被Apache收入麾下，实力可见一斑。<br />skywalking支持dubbo，SpringCloud，SpringBoot集成，**代码无侵入，通信方式采用GRPC，性能较好，实现方式是java探针，支持告警，支持JVM监控，支持全局调用统计**等等，功能较完善。
<a name="i31LY"></a>
## Skywalking和Spring Cloud Sleuth+ZipKin选型
Skywalking相比于zipkin还是有很大的优势的，如下：

- skywalking采用字节码增强的技术实现代码无侵入，zipKin代码侵入性比较高
- skywalking功能比较丰富，报表统计，UI界面更加人性化

**建议**：如果是新的架构，建议优先选择skywalking。
<a name="PIFU4"></a>
## Skywalking架构
skywalking和zipkin一样，也分为服务端和客户端，服务端负责收集日志数据并且展示，架构如下：<br />![sky.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645516785149-5579c7cb-8979-4b19-908b-9d8a626355cb.png#clientId=u8e6c4625-0580-4&from=ui&id=u5c8c7d03&originHeight=850&originWidth=1900&originalType=binary&ratio=1&rotation=0&showTitle=false&size=91836&status=done&style=shadow&taskId=u7c9d1b3f-5ed2-4ed6-baf0-e37ec85866c&title=)<br />上述架构图中主要分为四个部分，如下：

- **上面的Agent**：负责收集日志数据，并且传递给中间的OAP服务器
- **中间的OAP**：负责接收 Agent 发送的 Tracing 和Metric的数据信息，然后进行分析(Analysis Core) ，存储到外部存储器( Storage )，最终提供查询( Query )功能。
- **左面的UI**：负责提供web控制台，查看链路，查看各种指标，性能等等。
- **右面Storage**：负责数据的存储，支持多种存储类型。

看了架构图之后，思路很清晰了，Agent负责收集日志传输数据，通过GRPC的方式传递给OAP进行分析并且存储到数据库中，最终通过UI界面将分析的统计报表、服务依赖、拓扑关系图展示出来。
<a name="LxKs4"></a>
## 服务端搭建
skywalking同样是通过jar包方式启动，需要下载jar包，地址：[https://skywalking.apache.org/downloads/](https://skywalking.apache.org/downloads/)
<a name="yomsN"></a>
### 1、下载安装包
选择**V8.7.0**这个版本，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645516447672-35fadb0a-6d91-44f2-ac1b-7108323d1536.png#clientId=uf2773b4c-5095-4&from=paste&height=745&id=ucb14a428&originHeight=1863&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=479925&status=done&style=shadow&taskId=u1ecf0d3d-edda-4913-8a0c-ae904df55a3&title=&width=1536)<br />可以按照自己的需要选择其他版本<br />[apache-skywalking-apm-bin-es7.zip](https://www.yuque.com/attachments/yuque/0/2022/zip/396745/1645585346707-7617062a-107b-4841-8e0a-ce3e24b8e5b2.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2022%2Fzip%2F396745%2F1645585346707-7617062a-107b-4841-8e0a-ce3e24b8e5b2.zip%22%2C%22name%22%3A%22apache-skywalking-apm-bin-es7.zip%22%2C%22size%22%3A180673905%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22status%22%3A%22done%22%2C%22taskId%22%3A%22u921055ae-81d3-4314-ade6-1b60157320e%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22u024db1a2%22%2C%22card%22%3A%22file%22%7D)【**如果官网下载比较慢可以使用此安装包**】<br />解压之后完整目录如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639140492760-142545ef-8750-49c5-b16a-006ed65cbdf8.png#clientId=uf95ed468-a24c-4&from=paste&id=u957cf996&originHeight=1110&originWidth=1768&originalType=binary&ratio=1&rotation=0&showTitle=false&size=103987&status=done&style=shadow&taskId=ud2245a60-1775-403b-a46c-71f8e47bc99&title=)<br />重要的目录结构分析如下：

- **agent**：客户端需要指定的目录，其中有一个jar，就是负责和客户端整合收集日志
- **bin**：服务端启动的脚本
- **config**：一些配置文件的目录
- **logs**：oap服务的日志目录
- **oap-libs**：oap所需的依赖目录
- **webapp**：UI服务的目录
<a name="aJXrQ"></a>
### 2、配置修改
启动之前需要对配置文件做一些修改，修改如下：
<a name="DF4Zo"></a>
#### 1、/config/application.yml
这个是oap服务的配置文件，需要修改注册中心为nacos，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639140634535-c733c259-1961-45cc-abab-114a1d62b690.png#clientId=uf95ed468-a24c-4&from=paste&height=399&id=u323864ca&originHeight=1198&originWidth=3337&originalType=binary&ratio=1&rotation=0&showTitle=false&size=203900&status=done&style=shadow&taskId=ubf67e25b-2138-4058-8fe0-1df02015ead&title=&width=1112.3333333333333)<br />**配置①**：修改默认注册中心选择nacos，这样就不用在启动参数中指定了。<br />**配置②**：修改nacos的相关配置，由于这里是本地的，则不用改动，根据自己情况修改。
<a name="lNg47"></a>
#### 2、webapp/webapp.yml
这个是UI服务的配置文件，其中有一个**server.port**配置，是UI服务的端口，默认**8080**，将其改成**8888**，避免端口冲突，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639140682768-78652cac-94cc-4a86-91cb-89789e0f0ee4.png#clientId=uf95ed468-a24c-4&from=paste&height=56&id=uf3acc36f&originHeight=169&originWidth=1034&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10590&status=done&style=shadow&taskId=uc6adb86b-cada-4bc1-94ff-81e87a97e59&title=&width=344.6666666666667)
<a name="cxCrf"></a>
### 3、启动服务
启动命令在/bin目录下，这里需要启动两个服务，如下：

- **oap服务**：对应的启动脚本oapService.bat，Linux下对应的后缀是sh
- **UI服务**：对应的启动脚本webappService.bat，Linux下对应的后缀是sh

当然还有一个**startup.bat**启动文件，可以直接启动上述两个服务，可以直接使用这个脚本，直接双击，将会弹出两个窗口则表示启动成功，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645597712516-f3811f24-0520-4296-a42d-6cd451cfe65d.png#clientId=ua85166fa-7db4-4&from=paste&height=499&id=u2e66e3a2&originHeight=1247&originWidth=2014&originalType=binary&ratio=1&rotation=0&showTitle=false&size=44972&status=done&style=none&taskId=u808d836e-6c23-45b1-a29a-60281e13b97&title=&width=805.6)<br />此时直接访问：http://localhost:8888/，直接进入UI端，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803136652-2ded3a9c-4916-49f4-b77c-c03e49885965.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u8e97f691&originHeight=504&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf9ce1cc4-e718-4885-9e9c-b465632799b&title=)
<a name="Cv1vR"></a>
## 客户端搭建
客户端也就是单个微服务，由于Skywalking采用字节码增强技术，因此对于微服务无代码侵入，只要是普通的微服务即可，不需要引入什么依赖。<br />三个服务，如下：

- **skywalking-product1001**：商品微服务
- **skywalking-order1002**：订单微服务
- **skywalking-gateway1003**：网关微服务

案例源码结构目录如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803136583-90be890a-3cdb-4cd7-a0ae-887923464f7b.webp#clientId=u24dbefbd-acc4-4&from=paste&id=uf7aafa2f&originHeight=521&originWidth=371&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue9f65093-7dc0-4483-82c7-6b8db64f9fd&title=)<br />想要传输数据必须借助skywalking提供的agent，只需要在启动参数指定即可，命令如下：
```bash
-javaagent:E:\springcloud\apache-skywalking-apm-es7-8.7.0\apache-skywalking-apm-bin-es7\agent\skywalking-agent.jar
-Dskywalking.agent.service_name=skywalking-product-service
-Dskywalking.collector.backend_service=127.0.0.1:11800
```
上述命令解析如下：

- `**-javaagent**`：指定skywalking中的agent中的skywalking-agent.jar的路径
- `**-Dskywalking.agent.service_name**`：指定在skywalking中的服务名称，一般是微服务的`spring.application.name`
- `**-Dskywalking.collector.backend_service**`：指定oap服务绑定的地址，由于这里是本地，并且oap服务默认的端口是11800，因此只需要配置为127.0.0.1:11800

上述参数可以在命令行通过`java -jar xxx`指定，在IDEA中操作如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645605437437-72680f24-a280-49f5-82e2-756fabad17e5.png#clientId=u684eb548-4bcd-4&from=paste&height=670&id=u7fc495d5&originHeight=1676&originWidth=2770&originalType=binary&ratio=1&rotation=0&showTitle=false&size=228969&status=done&style=shadow&taskId=u3a467dd5-8592-47a3-a93b-4989d71da85&title=&width=1108)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645605467998-1d87e9fd-e587-4654-9e54-0e7d092cc195.png#clientId=u684eb548-4bcd-4&from=paste&height=670&id=u27267b3f&originHeight=1676&originWidth=2770&originalType=binary&ratio=1&rotation=0&showTitle=false&size=193206&status=done&style=none&taskId=u0f2f8568-c99f-4543-a627-a1017a4d15e&title=&width=1108)<br />上述三个微服务都需要配置skywalking的启动配置，配置成功后正常启动即可。
:::tips
注意：agent的jar包路径不能包含中文，不能有空格，否则运行不成功。
:::
成功启动后，直接通过网关访问：http://localhost:1003/order/get/12。<br />此时查看skywalking的UI端，可以看到服务已经监控成功了，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645605493534-351603e5-f7ba-4b99-b35a-14f6f4d28f5b.png#clientId=u684eb548-4bcd-4&from=paste&height=713&id=u6527c9f9&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=355240&status=done&style=shadow&taskId=u3c335179-fd41-4993-8cbe-bd1025e2ae6&title=&width=1536)<br />服务之前的依赖关系也是可以很清楚的看到，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803137089-aab0e556-9cd0-4e09-8c75-e1cf8d895b14.webp#clientId=u24dbefbd-acc4-4&from=paste&id=ubf1b8389&originHeight=489&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ued070bea-28c6-4fe6-a68e-f6b0be46c45&title=)<br />请求链路的信息也是能够很清楚的看到，比如请求的url，执行时间、调用的服务，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803137147-56658456-c544-4a23-b723-3f4c486fb40a.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u3382ef0c&originHeight=504&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc822e06a-6314-4124-b048-2dad60d96ad&title=)<br />比zipkin的功能更加丰富。
<a name="bQsHK"></a>
## 数据持久化
可以发现只要服务端重启之后，这些链路追踪数据将会丢失了，因为skywalking默认持久化的方式是存储在内存中。<br />当然这里也是可以通过插拔方式的替换掉存储中间件，企业中往往是使用**ES**存储，这里使用**MySQL**的方式存储，ES方式后面再介绍。
<a name="tE6VJ"></a>
### 1、修改配置文件
修改config/application.yml文件中的存储方式，总共需要修改两处地方。

- 修改默认的存储方式为mysql，如下图：

![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639141415811-4695b0ab-4bfb-46a7-bb48-efa8553b4b2f.png#clientId=u176d5979-c82b-4&from=paste&height=42&id=ua2f4b47c&originHeight=127&originWidth=1458&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17052&status=done&style=none&taskId=ue4be46ae-96ee-4c8e-846f-38cfbb822c7&title=&width=486)

- 修改Mysql相关的信息，比如用户名、密码等，如下图：

![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639141450890-8c972bb2-1beb-4fbe-a628-1e92ba9a3f90.png#clientId=u176d5979-c82b-4&from=paste&height=239&id=u99b86b64&originHeight=717&originWidth=3261&originalType=binary&ratio=1&rotation=0&showTitle=false&size=198829&status=done&style=shadow&taskId=u05ca678d-1323-4b86-8d96-30b3dd43fd4&title=&width=1087)
<a name="MaJDN"></a>
### 2、添加MySQL的jdbc依赖
默认的oap中是没有jdbc驱动依赖，因此需要手动添加一下，只需要将驱动的jar放在**oap-libs**文件夹中，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803137561-e46ff181-69d9-47d2-a9c7-74f831d8e36a.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u9dba6dfd&originHeight=49&originWidth=759&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5e6b31ef-5bc0-46f2-ad51-460ad7ac4e8&title=)<br />好了，已经配置完成，启动服务端，在skywalking这个数据库中将会自动创建表，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803137620-35eafcb1-51c7-460f-96fb-9b846e8ac621.webp#clientId=u24dbefbd-acc4-4&from=paste&id=ua695bda8&originHeight=572&originWidth=987&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2366e7f7-40d9-4555-afe2-c8cb36ba8f4&title=)
<a name="KmYhk"></a>
## 日志监控
在skywalking的UI端有一个日志的模块，用于收集客户端的日志，默认是没有数据的，那么需要如何将日志数据传输到skywalking中呢？<br />日志框架的种类很多，比较出名的有log4j，logback，log4j2，就以**logback**为例子介绍一下如何配置，官方文档如下：

- log4j：[https://skywalking.apache.org/docs/skywalking-java/v8.8.0/en/setup/service-agent/java-agent/application-toolkit-log4j-1.x/](https://skywalking.apache.org/docs/skywalking-java/v8.8.0/en/setup/service-agent/java-agent/application-toolkit-log4j-1.x/)
- log4j2：[https://skywalking.apache.org/docs/skywalking-java/v8.8.0/en/setup/service-agent/java-agent/application-toolkit-log4j-2.x/](https://skywalking.apache.org/docs/skywalking-java/v8.8.0/en/setup/service-agent/java-agent/application-toolkit-log4j-2.x/)
- logback：[https://skywalking.apache.org/docs/skywalking-java/v8.8.0/en/setup/service-agent/java-agent/application-toolkit-logback-1.x/](https://skywalking.apache.org/docs/skywalking-java/v8.8.0/en/setup/service-agent/java-agent/application-toolkit-logback-1.x/)
<a name="l6nPQ"></a>
### 1、添加依赖
根据官方文档，需要先添加依赖，如下：
```xml
<dependency>
   <groupId>org.apache.skywalking</groupId>
   <artifactId>apm-toolkit-logback-1.x</artifactId>
   <version>${project.release.version}</version>
</dependency>
```
<a name="pb6e2"></a>
### 2、添加配置文件
新建一个logback-spring.xml放在resource目录下，配置如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803137754-3e5c8b15-ff69-4d7c-99ba-3e055e9cbcba.webp#clientId=u24dbefbd-acc4-4&from=paste&id=uf8081293&originHeight=696&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u17440572-2885-4ba0-b21f-28e532fc695&title=)<br />以上配置全部都是拷贝官方文档的。<br />配置完成之后，启动服务，访问：http://localhost:1003/order/get/12。<br />控制台打印的日志如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803137673-e4ed8827-5b11-47b5-a1ec-5fdd4ef83ba4.webp#clientId=u24dbefbd-acc4-4&from=paste&id=uaed501c7&originHeight=221&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u463a4e42-bb2f-4575-b7e8-7939e4afa78&title=)<br />可以看到已经打印出了traceId。<br />skywalking中的日志模块输出的日志如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803137884-23640d57-c69e-4814-9be3-aef8f15f717e.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u3fad0ce5&originHeight=504&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b916cd2-e97d-4a98-90ed-71d98ad0512&title=)<br />日志已经传输到了skywalking中..............<br />**注意**：如果**agent**和**oap**服务不在同一台服务器上，需要在/agent/config/agent.config配置文件末尾添加如下配置：
```
plugin.toolkit.log.grpc.reporter.server_host=${SW_GRPC_LOG_SERVER_HOST:10.10.10.1}
plugin.toolkit.log.grpc.reporter.server_port=${SW_GRPC_LOG_SERVER_PORT:11800}
plugin.toolkit.log.grpc.reporter.max_message_size=${SW_GRPC_LOG_MAX_MESSAGE_SIZE:10485760}
plugin.toolkit.log.grpc.reporter.upstream_timeout=${SW_GRPC_LOG_GRPC_UPSTREAM_TIMEOUT:30}
```
配置分析如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138185-ccd2bfcc-49b0-4fcd-9fed-6c4bbba81e3d.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u6404ad93&originHeight=110&originWidth=875&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3f68ed2f-285a-4e5b-96bd-09d72838962&title=)
<a name="O9lSi"></a>
## 性能剖析
skywalking在性能剖析方面真的是非常强大，提供到基于堆栈的分析结果，能够让运维人员一眼定位到问题。<br />新建一个/order/list接口，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138186-16c03e5b-ccbd-42f1-9554-e684e5450e67.webp#clientId=u24dbefbd-acc4-4&from=paste&id=ucb3fba8d&originHeight=185&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uffb1fc95-9bb2-4e1c-81d7-bde384cc53f&title=)<br />上述代码中休眠了2秒，看看如何在skywalking中定位这个问题。<br />在性能剖析模块->新建任务->选择服务、填写端点、监控时间，操作如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138377-49e34ebf-29b8-49b7-8ca3-be9626684df7.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u4f9d3176&originHeight=510&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc77ce9d4-2914-49a6-a3de-7f5808403b4&title=)<br />上图中选择了最大采样数为5，则直接访问5次：http://localhost:1003/order/list，然后选择这个任务将会出现监控到的数据，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138354-af2d0f37-5a41-4187-b21c-366c8d34b907.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u6a112db7&originHeight=478&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u865460d4-1d9d-4823-a5e1-8949e8ad228&title=)<br />上图中可以看到{GET}/order/list这个接口上耗费了2秒以上，因此选择这个接口点击分析，可以看到详细的堆栈信息，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138644-56010d2a-02d2-483c-a55c-43e5a67d0e80.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u6c13703a&originHeight=520&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1ce31b7c-0217-4537-b9bc-0fcf6f51e95&title=)<br />如何定位到睡眠2秒钟的那一行代码呢？直接往下翻，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138489-bba715ff-ce69-4d40-914f-279a75d614b5.webp#clientId=u24dbefbd-acc4-4&from=paste&id=ufbd0a82e&originHeight=675&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2e8c61d2-c899-41ae-9652-c7118c7f5d8&title=)<br />是不是很清楚了，在OrderController这个接口线程睡眠了两秒........
<a name="Ak7hc"></a>
## 监控告警
对于服务的异常信息，比如接口有较长延迟，skywalking也做出了告警功能，如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138410-41efc4de-7c74-4f5c-82cf-269a4f08970f.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u75478df8&originHeight=290&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3cc101d0-275d-4793-9c97-c9a3cc3ec5f&title=)<br />skywalking中有一些默认的告警规则，如下：

- 最近3分钟内服务的平均响应时间超过1秒
- 最近2分钟服务成功率低于80%
- 最近3分钟90%服务响应时间超过1秒
- 最近2分钟内服务实例的平均响应时间超过1秒

当然除了以上四种，随着Skywalking不断迭代也会新增其他规则，这些规则的配置在config/alarm-settings.yml配置文件中，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639141504634-8d4418ac-d8f9-4812-ac5e-b5bff2cd6a02.png#clientId=u176d5979-c82b-4&from=paste&height=575&id=u3eaa60e8&originHeight=1726&originWidth=3292&originalType=binary&ratio=1&rotation=0&showTitle=false&size=236178&status=done&style=shadow&taskId=u28e499b2-29df-4b58-b47d-46ed467e188&title=&width=1097.3333333333333)<br />每个规则都由相同的属性组成，这些属性的含义如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138647-90f06d41-3065-4dab-866a-d8281810a008.webp#clientId=u24dbefbd-acc4-4&from=paste&id=u9cd4b9ad&originHeight=557&originWidth=919&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4ac6cded-1527-43bd-9d12-ef13fd57f88&title=)<br />如果想要调整默认的规则，比如监控返回的信息，监控的参数等等，只需要改动上述配置文件中的参数即可。<br />当然除了以上默认的几种规则，skywalking还适配了一些钩子（**webhooks**）。其实就是相当于一个回调，一旦触发了上述规则告警，skywalking则会调用配置的webhook，这样开发者就可以定制一些处理方法，比如发送**邮件**、**微信**、**钉钉**通知运维人员处理。<br />当然这个钩子也是有些规则的，如下：

- POST请求
- **application/json** 接收数据
- 接收的参数必须是AlarmMessage中指定的参数。

**注意**：AlarmMessage这个类随着skywalking版本的迭代可能出现不同，一定要到对应版本源码中去找到这个类，拷贝其中的属性。这个类在源码的路径：org.apache.skywalking.oap.server.core.alarm，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639141673793-aaba1271-b7ec-4340-bff7-b9aea0303dea.png#clientId=u176d5979-c82b-4&from=paste&height=594&id=u27d6c894&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=359073&status=done&style=none&taskId=uaec126d1-96de-4637-947a-ad9b9f0d862&title=&width=1280)<br />新建一个告警模块：**skywalking-alarm1004**，其中利用webhook定义一个接口，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803138889-42ddbb73-0a6d-4f72-82b0-1b3c0e2aa858.webp#clientId=u24dbefbd-acc4-4&from=paste&height=544&id=u6c97b3aa&originHeight=544&originWidth=827&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u895a4f32-0168-4778-929c-776f880235e&title=&width=827)<br />接口定制完成后，只需要在config/alarm-settings.yml配置文件中添加这个钩子，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639141712323-1ca7ed38-1608-4116-a98d-5c3eb2e362f3.png#clientId=u176d5979-c82b-4&from=paste&height=63&id=uf6d236dc&originHeight=188&originWidth=1354&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22867&status=done&style=shadow&taskId=uc4ed3cff-1c0a-4b6e-a642-eb24db49831&title=&width=451.3333333333333)<br />好了，这就已经配置完成了，测试也很简单，还是调用上面案例中的睡眠两秒的接口：http://localhost:1003/order/list，多调用几次，则会触发告警，控制台打印日志如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637803139194-e129b14e-70dd-4513-ba58-d50b65deee94.webp#clientId=u24dbefbd-acc4-4&from=paste&id=ue9a10604&originHeight=595&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u61829868-c338-4b51-b527-fba87be1943&title=)
