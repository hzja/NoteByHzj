性能监控系统<br />SkyWalking 是一个应用性能监控系统，特别为微服务、云原生和基于容器（Docker, Kubernetes, Mesos）体系结构而设计。除了应用指标监控以外，它还能对分布式调用链路进行追踪。类似功能的组件还有：Zipkin、Pinpoint、CAT等。<br />上几张图，看看效果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754045396-2512d532-7d47-4011-978c-4c2d1319e3f0.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u2c373e9c&originHeight=500&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u152cc317-96b1-4685-8355-1536f589a25&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754045411-f0549bb4-17ab-4f9e-8970-1a5a0daa731a.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u04237f50&originHeight=494&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u318f3716-13cb-442b-a344-2f22cd80df3&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754045392-913f5f5a-184c-4e98-a36e-37e1d67c1183.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u2e4238f6&originHeight=474&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6607b159-18d4-44a0-bb87-7637a93534b&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754045468-c4d8d546-6b2b-4050-903a-dab6de90940b.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u42f8ab19&originHeight=521&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6c90573c-ec03-4dde-a098-5cf6b3e0837&title=)
<a name="N7y8l"></a>
## 1、概念与架构
SkyWalking是一个开源监控平台，用于从服务和云原生基础设施收集、分析、聚合和可视化数据。SkyWalking提供了一种简单的方法来维护分布式系统的清晰视图，甚至可以跨云查看。它是一种现代APM，专门为云原生、基于容器的分布式系统设计。 <br />SkyWalking从三个维度对应用进行监视：service（服务）, service instance（实例）, endpoint（端点）<br />服务和实例就不多说了，端点是服务中的某个路径或者说URI
> SkyWalking allows users to understand the topology relationship between Services and Endpoints, to view the metrics of every Service/Service Instance/Endpoint and to set alarm rules.

SkyWalking允许用户了解服务和端点之间的拓扑关系，查看每个服务/服务实例/端点的度量，并设置警报规则。
<a name="KOkNs"></a>
### 1.1.  架构
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754217813-350c2f5b-aea6-4be7-88d5-a77205e24659.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u4d2d97ae&originHeight=410&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u04180d18-5fc2-4d92-a0c3-a7cdcdf3678&title=)<br />SkyWalking逻辑上分为四个部分：Probes（探针）, Platform backend（平台后端）, Storage（存储）, UI<br />这个结构就很清晰了，探针就是Agent负责采集数据并上报给服务端，服务端对数据进行处理和存储，UI负责展示<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754046204-e29c80e9-5f3a-459b-bc02-5834fa7acae8.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=ue37f23f7&originHeight=626&originWidth=943&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1a89b5d8-f2d2-416d-8b43-49e19cd095c&title=)
<a name="aGJ2b"></a>
## 2、下载与安装
SkyWalking有两中版本，ES版本和非ES版。如果决定采用ElasticSearch作为存储，那么就下载es版本。 <br />[https://skywalking.apache.org/downloads/](https://skywalking.apache.org/downloads/)<br />[https://archive.apache.org/dist/skywalking/](https://archive.apache.org/dist/skywalking/)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638891011881-7bd7e3b7-36ee-4d57-853d-5424ebcc43db.png#clientId=u0ea24311-11c7-4&from=paste&height=522&id=ud26dc735&originHeight=1043&originWidth=1681&originalType=binary&ratio=1&rotation=0&showTitle=false&size=242452&status=done&style=shadow&taskId=u64429bf6-722e-4449-8c2a-ace5459d4cd&title=&width=840.5)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754240570-40a4d330-4b16-4baf-8f8b-799cf74fe78a.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u7e95fd86&originHeight=406&originWidth=1015&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uefaedfd6-c717-4099-98ed-961eb07b6d9&title=)<br />agent目录将来要拷贝到各服务所在机器上用作探针<br />bin目录是服务启动脚本<br />config目录是配置文件<br />oap-libs目录是oap服务运行所需的jar包<br />webapp目录是web服务运行所需的jar包<br />接下来，要选择存储了，支持的存储有：

- H2
- ElasticSearch 6, 7
- MySQL
- TiDB
- InfluxDB

作为监控系统，首先排除H2和MySQL，这里推荐InfluxDB，它本身就是时序数据库，非常适合这种场景<br />但是InfluxDB不是很熟悉，所以这里先用ElasticSearch7<br />[https://github.com/apache/skywalking/blob/master/docs/en/setup/backend/backend-storage.md](https://github.com/apache/skywalking/blob/master/docs/en/setup/backend/backend-storage.md)
<a name="ZasrM"></a>
### 2.1.  安装ElasticSearch
[https://www.elastic.co/guide/en/elasticsearch/reference/7.10/targz.html](https://www.elastic.co/guide/en/elasticsearch/reference/7.10/targz.html) 
```bash
# 启动
./bin/elasticsearch -d -p pid
# 停止
pkill -F pid
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754251859-03978bfa-68d7-45fd-839b-f452a96a60ff.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=uaad9e9f7&originHeight=117&originWidth=981&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u97438cac-fd74-44c8-b51c-bc6e30b00f7&title=)<br />ElasticSearch7.x需要Java 11以上的版本，但是如果设置了环境变量`JAVA_HOME`的话，它会用环境变量设置的Java版本<br />通常，启动过程中会报以下三个错误：
```bash
[1]: max file descriptors [4096] for elasticsearch process is too low, increase to at least [65535]
[2]: max virtual memory areas vm.max_map_count [65530] is too low, increase to at least [262144]
[3]: the default discovery settings are unsuitable for production use; at least one of [discovery.seed_hosts, discovery.seed_providers, cluster.initial_master_nodes] must be configured
```
解决方法：<br />在 /etc/security/limits.conf 文件中追加以下内容：
```
* soft nofile 65536
* hard nofile 65536
* soft nproc  4096
* hard nproc  4096
```
可通过以下四个命令查看修改结果：
```
ulimit -Hn
ulimit -Sn
ulimit -Hu
ulimit -Su
```
修改 /etc/sysctl.conf 文件，追加以下内容：
```
vm.max_map_count=262144
```
修改es配置文件 elasticsearch.yml 取消注释，保留一个节点
```
cluster.initial_master_nodes: ["node-1"]
```
为了能够ip:port方式访问，还需修改网络配置
```
network.host: 0.0.0.0
```
修改完是这样的：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754046034-0973050d-c7a6-4ccb-a1bf-c748a66f5293.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u0cb0c865&originHeight=478&originWidth=884&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud1c6a9ad-3d58-4989-9814-14eaba908ff&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754301080-2e62e5ad-769a-4ec9-a455-ddc2f396ae6d.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u4faef75d&originHeight=389&originWidth=521&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u29f7e617-5a7b-403e-af22-f23cbe51ac7&title=)<br />至此，ElasticSearch算是启动成功了<br />一个节点还不够，这里用三个节点搭建一个集群<br />192.168.100.14  config/elasticsearch.yml
```
cluster.name: my-monitor
node.name: node-1
network.host: 192.168.100.14
http.port: 9200
discovery.seed_hosts: ["192.168.100.14:9300", "192.168.100.15:9300", "192.168.100.19:9300"]
cluster.initial_master_nodes: ["node-1"]
```
192.168.100.15  config/elasticsearch.yml
```
cluster.name: my-monitor
node.name: node-2
network.host: 192.168.100.15
http.port: 9200
discovery.seed_hosts: ["192.168.100.14:9300", "192.168.100.15:9300", "192.168.100.19:9300"]
cluster.initial_master_nodes: ["node-1"]
```
192.168.100.19  config/elasticsearch.yml
```
cluster.name: my-monitor
node.name: node-3
network.host: 192.168.100.19
http.port: 9200
discovery.seed_hosts: ["192.168.100.14:9300", "192.168.100.15:9300", "192.168.100.19:9300"]
cluster.initial_master_nodes: ["node-1"]
```
同时，建议修改三个节点config/jvm.options
```
-Xms2g
-Xmx2g
```
依次启动三个节点
```bash
pkill -F pid
./bin/elasticsearch -d -p pid
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754345824-c232e0ac-0bac-479a-889b-fb6f1e8377cd.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u07d35a50&originHeight=121&originWidth=726&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u385929ff-6425-452d-9f3f-9127bf95169&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754046557-251363fe-973d-42fa-b300-e50794f9691e.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u84ea5a21&originHeight=294&originWidth=630&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uab432514-926e-47ce-820f-2288f54dd6f&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754046602-00f6b544-94d4-4e4b-b220-b5abb0708b02.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=ue9a0a2aa&originHeight=383&originWidth=690&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7c2eb10b-18f1-4459-be56-fe86a46b23c&title=)<br />接下来，修改skywalking下config/application.yml 中配置es地址即可
```yaml
storage:
  selector: ${SW_STORAGE:elasticsearch7}
  elasticsearch7:
    nameSpace: ${SW_NAMESPACE:""}
    clusterNodes: ${SW_STORAGE_ES_CLUSTER_NODES:192.168.100.14:9200,192.168.100.15:9200,192.168.100.19:9200}
```
<a name="NL3XZ"></a>
### 2.2.  安装Agent
[https://github.com/apache/skywalking/blob/v8.2.0/docs/en/setup/service-agent/java-agent/README.md](https://github.com/apache/skywalking/blob/v8.2.0/docs/en/setup/service-agent/java-agent/README.md)<br />将agent目录拷贝至各服务所在的机器上
```bash
scp -r ./agent chengjs@192.168.100.12:~/
```
这里，将它拷贝至各个服务目录下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754046689-c80c0851-8237-4ec8-a181-0618372468d0.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=ufa743570&originHeight=152&originWidth=949&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1b591f4f-0d80-407c-929e-e3d4c6b7d19&title=)<br />plugins是探针用到各种插件，SkyWalking插件都是即插即用的，可以把optional-plugins中的插件放到plugins中 <br />修改 agent/config/agent.config 配置文件，也可以通过命令行参数指定<br />主要是配置服务名称和后端服务地址
```bash
agent.service_name=${SW_AGENT_NAME:user-center}
collector.backend_service=${SW_AGENT_COLLECTOR_BACKEND_SERVICES:192.168.100.17:11800}
```
当然，也可以通过环境变量或系统属性的方式来设置，例如：
```bash
export SW_AGENT_COLLECTOR_BACKEND_SERVICES=127.0.0.1:11800
```
最后，在服务启动的时候用命令行参数 `-javaagent` 来指定探针
```bash
java -javaagent:/path/to/skywalking-agent/skywalking-agent.jar -jar yourApp.jar
```
例如：
```bash
java -javaagent:./agent/skywalking-agent.jar -Dspring.profiles.active=dev -Xms512m -Xmx1024m -jar demo-0.0.1-SNAPSHOT.jar
```
<a name="v5Nut"></a>
## 3、启动服务
修改 webapp/webapp.yml 文件，更改端口号及后端服务地址
```yaml
server:
  port: 9000

collector:
  path: /graphql
  ribbon:
    ReadTimeout: 10000
    # Point to all backend's restHost:restPort, split by ,
    listOfServers: 127.0.0.1:12800
```
启动服务
```bash
bin/startup.sh
```
或者分别依次启动
```bash
bin/oapService.sh
bin/webappService.sh
```
查看logs目录下的日志文件，看是否启动成功<br />浏览器访问 http://127.0.0.1:9000
<a name="FQrSc"></a>
## 4、告警
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754046816-212ab29b-eb79-4449-b4cf-8c3725f1044b.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u3b3ebdd3&originHeight=160&originWidth=865&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u40332784-38fd-4d25-a6cf-1ab5160b8df&title=)<br />编辑 alarm-settings.yml 设置告警规则和通知<br />[https://github.com/apache/skywalking/blob/v8.2.0/docs/en/setup/backend/backend-alarm.md](https://github.com/apache/skywalking/blob/v8.2.0/docs/en/setup/backend/backend-alarm.md)<br />重点说下告警通知<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754047162-873cadcc-1fbc-4a68-9f36-6b9b17a300de.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u6966c37c&originHeight=65&originWidth=546&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2df3b2f0-3270-4d6d-a0aa-73dd7230e10&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754046916-f0ef0761-a89c-4cd8-b0d2-f6c97824abbf.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=u2fb93463&originHeight=318&originWidth=1018&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u94736d39-94e7-4991-9791-c6de7e413c4&title=)<br />为了使用钉钉机器人通知，接下来，新建一个项目
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.4.0</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>com.wt.monitor</groupId>
    <artifactId>skywalking-alarm</artifactId>
    <version>1.0.0-SNAPSHOT</version>
    <name>skywalking-alarm</name>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <dependency>
            <groupId>com.aliyun</groupId>
            <artifactId>alibaba-dingtalk-service-sdk</artifactId>
            <version>1.0.1</version>
        </dependency>

        <dependency>
            <groupId>commons-codec</groupId>
            <artifactId>commons-codec</artifactId>
            <version>1.15</version>
        </dependency>

        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>fastjson</artifactId>
            <version>1.2.75</version>
        </dependency>

        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>

</project>
```
<a name="TKDi5"></a>
### 可选依赖（不建议引入）
```xml
<dependency>
    <groupId>org.apache.skywalking</groupId>
    <artifactId>server-core</artifactId>
    <version>8.2.0</version>
</dependency>
```
<a name="f04sa"></a>
### 定义告警消息实体类
```java
package com.wt.monitor.skywalking.alarm.domain;

import lombok.Data;

import java.io.Serializable;

@Data
public class AlarmMessageDTO implements Serializable {

    private int scopeId;

    private String scope;

    /**
     * Target scope entity name
     */
    private String name;

    private String id0;

    private String id1;

    private String ruleName;

    /**
     * Alarm text message
     */
    private String alarmMessage;

    /**
     * Alarm time measured in milliseconds
     */
    private long startTime;

}
```
<a name="rHibj"></a>
### 发送钉钉机器人消息
```java
package com.wt.monitor.skywalking.alarm.service;

import com.dingtalk.api.DefaultDingTalkClient;
import com.dingtalk.api.DingTalkClient;
import com.dingtalk.api.request.OapiRobotSendRequest;
import com.taobao.api.ApiException;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.codec.binary.Base64;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;

@Slf4j
@Service
public class DingTalkAlarmService {

    @Value("${dingtalk.webhook}")
    private String webhook;
    @Value("${dingtalk.secret}")
    private String secret;

    public void sendMessage(String content) {
        try {
            Long timestamp = System.currentTimeMillis();
            String stringToSign = timestamp + "\n" + secret;
            Mac mac = Mac.getInstance("HmacSHA256");
            mac.init(new SecretKeySpec(secret.getBytes("UTF-8"), "HmacSHA256"));
            byte[] signData = mac.doFinal(stringToSign.getBytes("UTF-8"));
            String sign = URLEncoder.encode(new String(Base64.encodeBase64(signData)),"UTF-8");

            String serverUrl = webhook + "&timestamp=" + timestamp + "&sign=" + sign;
            DingTalkClient client = new DefaultDingTalkClient(serverUrl);
            OapiRobotSendRequest request = new OapiRobotSendRequest();
            request.setMsgtype("text");
            OapiRobotSendRequest.Text text = new OapiRobotSendRequest.Text();
            text.setContent(content);
            request.setText(text);

            client.execute(request);
        } catch (ApiException e) {
            e.printStackTrace();
            log.error(e.getMessage(), e);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
            log.error(e.getMessage(), e);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            log.error(e.getMessage(), e);
        } catch (InvalidKeyException e) {
            e.printStackTrace();
            log.error(e.getMessage(), e);
        }
    }
}
```
<a name="QML35"></a>
### AlarmController.java
```java
package com.wt.monitor.skywalking.alarm.controller;

import com.alibaba.fastjson.JSON;
import com.wt.monitor.skywalking.alarm.domain.AlarmMessageDTO;
import com.wt.monitor.skywalking.alarm.service.DingTalkAlarmService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.text.MessageFormat;
import java.util.List;

@Slf4j
@RestController
@RequestMapping("/skywalking")
public class AlarmController {

    @Autowired
    private DingTalkAlarmService dingTalkAlarmService;

    @PostMapping("/alarm")
    public void alarm(@RequestBody List<AlarmMessageDTO> alarmMessageDTOList) {
       log.info("收到告警信息: {}", JSON.toJSONString(alarmMessageDTOList));
       if (null != alarmMessageDTOList) {
           alarmMessageDTOList.forEach(e->dingTalkAlarmService.sendMessage(MessageFormat.format("-----来自SkyWalking的告警-----\n【名称】: {0}\n【消息】: {1}\n", e.getName(), e.getAlarmMessage())));
       }
    }
}
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635754047042-7891b047-216c-4e89-b710-310b7c928e07.webp#clientId=ufb9101a9-fdc0-4&from=paste&id=ua73d3ce2&originHeight=363&originWidth=740&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u517b824a-a7e3-404e-97d5-71493b5bf1d&title=)
