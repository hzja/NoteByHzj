Sentinel 对监控数据的做法是定时落盘在客户端，然后 Sentinel 提供接口去拉取日志文件。所以 Sentinel 在监控数据上理论上是最少存储 1 天以上的数据；然而作为控制台展示，则仅在内存中聚合 5 分钟以内的统计数据，不进行持久化。<br />官方鼓励大家对 Dashboard 进行改造实现指标信息的持久化，并从其它的存储中（如 RDBMS、时序数据库等）拉取的监控信息，包括实时的和历史的数据。<br />基于此，结合公司内部的需求，自行改造并实现了监控指标的持久化。把一些实现过程分享给大家！<br />Sentinel 是阿里巴巴开源的流量治理平台，提供了 流量控制、熔断降级、系统负载保护、黑白名单访问控制 等功能。在实际的生产需求中，笔者进行了部分扩展：

1. 流控规则持久化：适配 Apollo、Nacos、Zookeeper
2. 监控数据持久化：适配 InfluxDB、Kafka、Elasticsearch
3. 监控面板优化：新增时间控件，允许在任意时刻内查询监控数据。

开源地址：[https://github.com/shiyindaxiaojie/Sentinel](https://github.com/shiyindaxiaojie/Sentinel)
<a name="nCnTO"></a>
## 演示图例
<a name="tYs1D"></a>
### 改造前
![DM_20231113165805_001.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1699865940831-6c122f6d-1b36-4990-8059-5a14f110de2a.png#averageHue=%23f9f9f8&clientId=ucf3a24ce-bd9d-4&from=ui&id=uf37b66e7&originHeight=951&originWidth=1920&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=205999&status=done&style=none&taskId=ub66124cf-faab-4f50-8193-3c6ffe9334a&title=)
<a name="XSyzg"></a>
### 改造后
![DM_20231113165805_002.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1699865952860-7a870098-d8d6-4a7f-9a2e-6d862bbb6fc9.png#averageHue=%23f9f8f8&clientId=ucf3a24ce-bd9d-4&from=ui&id=u680bb23e&originHeight=1080&originWidth=1920&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=117445&status=done&style=none&taskId=u9e98de63-2c67-4eb0-954a-b189de1cdce&title=)<br />快捷时间选择。<br />![DM_20231113165805_003.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1699865965691-6ec4f05a-eee8-47c1-927d-782f73c99279.png#averageHue=%23f9f8f7&clientId=ucf3a24ce-bd9d-4&from=ui&id=ufe3d252e&originHeight=1080&originWidth=1920&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=126181&status=done&style=none&taskId=uefd5a57d-82ae-439d-b358-189b3bfa302&title=)<br />自定义时间选择。<br />![DM_20231113165805_004.PNG](https://cdn.nlark.com/yuque/0/2023/png/396745/1699865975910-54664706-fcb8-41bf-bfb8-0fc87bf2f491.png#averageHue=%23f9f8f8&clientId=ucf3a24ce-bd9d-4&from=ui&id=uccbd1840&originHeight=1080&originWidth=1920&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=126101&status=done&style=none&taskId=ubbb16d89-3228-477f-94d8-ca31856fdb7&title=)
<a name="nxJom"></a>
## 如何构建
本项目默认使用 Maven 来构建，最快的使用方式是把代码导入的开发工具中。或在项目的根目录执行 `mvn install -T 4C` 完成本项目的构建。
<a name="a2ldv"></a>
## 如何启动
<a name="fZhe1"></a>
### IDEA 启动
本项目默认不依赖外部组件，可以直接启动运行。

1. 在项目目录下运行 `mvn install`（如果不想运行测试，可以加上 `-DskipTests` 参数）。
2. 进入 sentinel-dashboard 目录，执行 `mvn spring-boot:run` 或者启动 SentinelApplication 类。运行成功的话，可以看到 Spring Boot 启动成功的界面。

在实际的生产需求，Sentinel 保存的规则和监控是需要持久化落盘的，因此，可以在 sentinel-dashboard/src/main/resources/application.properties 接入外部组件。

- 规则存储类型：memory（默认）、nacos（推荐）、apollo、zookeeper
```
# 规则存储类型，可选项：memory（默认）、nacos（推荐）、apollo、zookeeper
sentinel.rule.type=nacos
# Nacos 存储规则，如果您设置了 sentinel.metrics.type=nacos，需要调整相关配置
sentinel.rule.nacos.server-addr=localhost:8848
sentinel.rule.nacos.namespace=demo
sentinel.rule.nacos.group-id=sentinel
sentinel.rule.nacos.username=nacos
sentinel.rule.nacos.password=nacos
# Apollo 存储规则，如果您设置了 sentinel.metrics.type=apollo，需要调整相关配置
sentinel.rule.apollo.portal-url=http://localhost:10034
sentinel.rule.apollo.token=
sentinel.rule.apollo.env=
# Zookeeper 存储规则，如果您设置了 sentinel.metrics.type=zookeeper，需要调整相关配置
sentinel.rule.zookeeper.connect-string=localhost:2181
sentinel.rule.zookeeper.root-path=/sentinel_rule
```

- 监控存储类型：memory（默认）、influxdb（推荐）、elasticsearch
```
# 监控存储类型，可选项：memory（默认）、influxdb（推荐）、elasticsearch
sentinel.metrics.type=memory
# InfluxDB 存储监控数据，如果您设置了 sentinel.metrics.type=influxdb，需要调整相关配置
influx.url=http://localhost:8086/
influx.token=UfgaW37A93PkncmJum25G7M2QkBg6xqqjGthh-o-UIVIynC_-Q7RFWlTtEpMqhGLCuAsX64k3Isc2uN33YgElw==
influx.org=sentinel
influx.bucket=sentinel
influx.log-level=NONE
influx.read-timeout=10s
influx.write-timeout=10s
influx.connect-timeout=10s
# Elasticsearch 存储监控数据，如果您设置了 sentinel.metrics.type=elasticsearch，需要调整相关配置
sentinel.metrics.elasticsearch.index-name=sentinel_metric
spring.elasticsearch.rest.uris=http://localhost:9200
spring.elasticsearch.rest.connection-timeout=3000
spring.elasticsearch.rest.read-timeout=5000
spring.elasticsearch.rest.username=
spring.elasticsearch.rest.password=
# 监控数据存储缓冲设置，降低底层存储组件写入压力。可选项：none（默认不启用）、kafka（推荐）
sentinel.metrics.sender.type=none
# Kafka 存储监控数据，如果您设置了 sentinel.metrics.sender.type=kafka，需要调整相关配置
sentinel.metrics.sender.kafka.topic=sentinel_metric
spring.kafka.producer.bootstrap-servers=localhost:9092
spring.kafka.producer.batch-size=4096
spring.kafka.producer.buffer-memory=40960
spring.kafka.producer.key-serializer=org.apache.kafka.common.serialization.StringSerializer
spring.kafka.producer.value-serializer=org.apache.kafka.common.serialization.StringSerializer
```
<a name="wS7ww"></a>
### 镜像启动
本项目已发布到 Docker Hub [https://hub.docker.com/repository/docker/shiyindaxiaojie/sentinel-dashboard](https://hub.docker.com/repository/docker/shiyindaxiaojie/sentinel-dashboard)，请执行参考命令运行。
```bash
docker run -p 8090:8090 --name=sentinel-dashboard -d shiyindaxiaojie/sentinel-dashboard
```
<a name="PVors"></a>
## 如何部署
<a name="ozEGz"></a>
### FatJar 部署
执行 `mvn clean package` 打包成一个 fat jar，参考如下命令启动编译后的控制台。
```bash
java -Dserver.port=8080 \
-Dsentinel.rule.nacos.server-addr=localhost:8848 \
-Dsentinel.rule.nacos.namespace=demo \
-Dsentinel.rule.nacos.group-id=sentinel \
-Dsentinel.metrics.type=influxdb \
-Dinflux.url=http://localhost:8086 \
-Dinflux.token=XXXXXX \
-Dinflux.org=sentinel \
-Dinflux.bucket=sentinel \
-jar target/sentinel-dashboard.jar
```
<a name="ApJxw"></a>
### Docker 部署
本项目使用了 Spring Boot 的镜像分层特性优化了镜像的构建效率，请确保正确安装了 Docker 工具，然后执行以下命令。
```bash
docker build -f docker/Dockerfile -t sentinel-dashboard:{tag} .
```
<a name="nFxBc"></a>
### Helm 部署
以应用为中心，建议使用 Helm 统一管理所需部署的 K8s 资源描述文件，请参考以下命令完成应用的安装和卸载。
```bash
helm install sentinel-dashboard ./helm # 部署资源
helm uninstall sentinel-dashboard # 卸载资源
```
<a name="iu485"></a>
## 如何接入
为了减少客户端集成的工作，可以使用 eden-architect 框架，只需要两步就可以完成 Sentinel 的集成。

1. 引入 Sentinel 依赖
```xml
<dependency>
  <groupId>io.github.shiyindaxiaojie</groupId>
  <artifactId>eden-sentinel-spring-cloud-starter</artifactId>
</dependency>
```

1. 开启 Sentinel 配置
```yaml
spring:
  cloud:
    sentinel: # 流量治理组件
      enabled: false # 默认关闭，请按需开启
      http-method-specify: true # 兼容 RESTful
      eager: true # 立刻刷新到 Dashboard
      transport:
        dashboard: localhost:8090
      datasource:
        flow:
          nacos:
            server-addr: ${spring.cloud.nacos.config.server-addr}
            namespace: ${spring.cloud.nacos.config.namespace}
            groupId: sentinel
            dataId: ${spring.application.name}-flow-rule
            rule-type: flow
            data-type: json
```
提供两种不同应用架构的示例，里面有集成 Sentinel 的示例。

- 面向领域模型的 **COLA 架构**，代码实例可以查看 [eden-demo-cola](https://github.com/shiyindaxiaojie/eden-demo-cola)
- 面向数据模型的 **分层架构**，代码实例请查看 [eden-demo-layer](https://github.com/shiyindaxiaojie/eden-demo-layer)
