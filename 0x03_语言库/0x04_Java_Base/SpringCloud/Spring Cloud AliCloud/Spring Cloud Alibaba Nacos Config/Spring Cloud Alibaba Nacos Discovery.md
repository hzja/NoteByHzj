Spring Cloud Alibaba Nacos Discovery<br />Nacos 是一个 Alibaba 开源的、易于构建云原生应用的动态服务发现、配置管理和服务管理平台。<br />使用 Spring Cloud Alibaba Nacos Discovery，可基于 Spring Cloud 的编程模型快速接入 Nacos 服务注册功能。
<a name="DiRit"></a>
## 1、服务注册/发现: Nacos Discovery
服务发现是微服务架构体系中最关键的组件之一。如果尝试着用手动的方式来给每一个客户端来配置所有服务提供者的服务列表是一件非常困难的事，而且也不利于服务的动态扩缩容。Nacos Discovery 可以将服务自动注册到 Nacos 服务端并且能够动态感知和刷新某个服务实例的服务列表。除此之外，Nacos Discovery 也将服务实例自身的一些元数据信息-例如 host，port, 健康检查URL，主页等内容注册到 Nacos。Nacos 的获取和启动方式可以参考 Nacos 官网。
<a name="W4b5L"></a>
## 2、引入 Nacos Discovery 进行服务注册/发现
如果要在项目中使用 Nacos 来实现服务注册/发现，使用 group ID 为 `com.alibaba.cloud` 和 artifact ID 为 `spring-cloud-starter-alibaba-nacos-discovery` 的 starter。
```xml
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>
```
<a name="ae5J2"></a>
## 3、一个使用 Nacos Discovery 进行服务注册/发现并调用的例子
Nacos Discovery 适配了 Netflix Ribbon，可以使用 RestTemplate 或 OpenFeign 进行服务的调用。
<a name="HCpz6"></a>
### 3.1. Nacos Server 启动
<a name="FJZbH"></a>
#### 下载编译后压缩包方式
可以从 [最新稳定版本](https://github.com/alibaba/nacos/releases) 下载 nacos-server-$version.zip 包。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635577528002-e4975aa7-aae7-4a78-b082-afc66d703038.png#clientId=uc8243096-edc7-4&from=paste&id=u815deaf4&originHeight=391&originWidth=1361&originalType=binary&ratio=1&size=42160&status=done&style=shadow&taskId=u76a2eec6-1d57-4d6f-88ca-31c56dd3b98)
```bash
unzip nacos-server-$version.zip 
# 或者
tar -xvf nacos-server-$version.tar.gz   
cd nacos/bin
```
<a name="MAqkG"></a>
#### Linux/Unix/Mac启动
启动命令(standalone代表着单机模式运行，非集群模式):
```bash
sh startup.sh -m standalone
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635580187127-03778e0c-7c30-4998-ab4b-43358a4c2a26.png#clientId=ub7b93034-a76b-4&from=paste&height=220&id=u80890129&originHeight=660&originWidth=3323&originalType=binary&ratio=1&size=829749&status=done&style=none&taskId=ub98f6dbe-cf67-4f62-9a4e-4516d0e19fc&width=1107.6666666666667)<br />如果使用的是ubuntu系统，或者运行脚本报错提示[[符号找不到，可尝试如下运行：
```bash
bash startup.sh -m standalone
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635580195640-8beea540-328b-489d-8452-8ed55a09f9e8.png#clientId=ub7b93034-a76b-4&from=paste&height=587&id=u3344c63d&originHeight=1760&originWidth=3323&originalType=binary&ratio=1&size=2134945&status=done&style=none&taskId=u1ecee198-1be8-4266-b9df-d345859c173&width=1107.6666666666667)
<a name="QOycG"></a>
#### Windows启动
启动命令(standalone代表着单机模式运行，非集群模式):
```bash
startup.cmd -m standalone
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635579253575-53da78e8-a083-43d0-a021-f706313841fa.png#clientId=uc9dfac64-81cb-4&from=paste&height=587&id=u430781da&originHeight=1760&originWidth=3323&originalType=binary&ratio=1&size=2138264&status=done&style=none&taskId=u73c72088-da26-4899-aeae-0eb10b6e310&width=1107.6666666666667)
<a name="WiQtc"></a>
### 3.2. Nacos 服务端添加配置
Nacos Server 启动完毕后，访问控制台打印的管理页面的URL，默认的用户名和密码是nacos。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635579395783-a7abf84c-d66f-4dd9-97b5-e60a27c2a2e6.png#clientId=uc9714120-9e55-4&from=paste&height=645&id=ue41321bb&originHeight=1934&originWidth=3840&originalType=binary&ratio=1&size=219999&status=done&style=shadow&taskId=ued2e4c50-6571-4568-94b5-97ac9641067&width=1280)
<a name="Yp4Jb"></a>
### 3.3. Provider 应用
以下步骤展示了如何将一个服务注册到 Nacos。

- pom.xml的配置。一个完整的 pom.xml 配置如下所示：

pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>open.source.test</groupId>
    <artifactId>nacos-discovery-test</artifactId>
    <version>1.0-SNAPSHOT</version>
    <name>nacos-discovery-test</name>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>${spring.boot.version}</version>
        <relativePath/>
    </parent>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
        <java.version>1.8</java.version>
    </properties>
    <dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>org.springframework.cloud</groupId>
                <artifactId>spring-cloud-dependencies</artifactId>
                <version>${spring.cloud.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
            <dependency>
                <groupId>com.alibaba.cloud</groupId>
                <artifactId>spring-cloud-alibaba-dependencies</artifactId>
                <version>${spring.cloud.alibaba.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
        </dependencies>
    </dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-actuator</artifactId>
        </dependency>
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
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

- application.properties 配置。一些关于 Nacos 基本的配置也必须在 application.properties(也可以是application.yaml)配置，如下所示：

application.properties
```
server.port=8081
spring.application.name=nacos-provider
spring.cloud.nacos.discovery.server-addr=127.0.0.1:8848
management.endpoints.web.exposure.include=*
```
如果不想使用 Nacos 作为服务注册与发现，可以将 `spring.cloud.nacos.discovery` 设置为 `false`。

- 启动 Provider 示例。如下所示：
```java
@SpringBootApplication
@EnableDiscoveryClient
public class NacosProviderDemoApplication {
    public static void main(String[] args) {
        SpringApplication.run(NacosProviderDemoApplication.class, args);
    }
    @RestController
    public class EchoController {
        @GetMapping(value = "/echo/{string}")
        public String echo(@PathVariable String string) {
            return "Hello Nacos Discovery " + string;
        }
    }
}
```
这个时候就可以在 Nacos的控制台上看到注册上来的服务信息了。
<a name="DhbUh"></a>
### 3.4. Consumer 应用
Consumer 应用可能还没像启动一个 Provider 应用那么简单。因为在 Consumer 端需要去调用 Provider 端提供的REST 服务。例子中使用最原始的一种方式， 即显示的使用 LoadBalanceClient 和 RestTemplate 结合的方式来访问。pom.xml 和 application.properties 的配置可以参考 1.2 小结。启动一个 Consumer应用的示例代码如下所示：<br />通过带有负载均衡的RestTemplate 和 FeignClient 也是可以访问的。
```java
@SpringBootApplication
@EnableDiscoveryClient
public class NacosConsumerApp {
    @RestController
    public class NacosController{
        @Autowired
        private LoadBalancerClient loadBalancerClient;
        @Autowired
        private RestTemplate restTemplate;
        @Value("${spring.application.name}")
        private String appName;
        @GetMapping("/echo/app-name")
        public String echoAppName(){
            //使用 LoadBalanceClient 和 RestTemplate 结合的方式来访问
            ServiceInstance serviceInstance = loadBalancerClient.choose("nacos-provider");
            String url = String.format("http://%s:%s/echo/%s",serviceInstance.getHost(),serviceInstance.getPort(),appName);
            System.out.println("request url:"+url);
            return restTemplate.getForObject(url,String.class);
        }
    }
    //实例化 RestTemplate 实例
    @Bean
    public RestTemplate restTemplate(){
        return new RestTemplate();
    }
    public static void main(String[] args) {
        SpringApplication.run(NacosConsumerApp.class,args);
    }
}
```
这个例子中注入了一个 LoadBalancerClient 的实例，并且手动的实例化一个 RestTemplate，同时将 `spring.application.name` 的配置值 注入到应用中来， 目的是调用 Provider 提供的服务时，希望将当前配置的应用名给显示出来。
:::info
在启动 Consumer 应用之前请先将 Nacos 服务启动好。具体启动方式可参考 Nacos 官网。
:::
启动后，访问 Consumer 提供出来的 `http://ip:port/echo/app-name` 接口。这里测试启动的 port是 8082。访问结果如下所示：<br />访问地址：[http://127.0.0.1:8082/echo/app-name](http://127.0.0.1:8082/echo/app-name)<br />访问结果：Hello Nacos Discovery nacos-consumer
<a name="gvlW3"></a>
## 4、Nacos Discovery 对外暴露的 Endpoint
Nacos Discovery 内部提供了一个 Endpoint, 对应的 endpoint id 为 `nacos-discovery`。<br />Endpoint 暴露的 json 中包含了两种属性:

1. subscribe: 显示了当前服务有哪些服务订阅者
2. NacosDiscoveryProperties: 当前应用 Nacos 的基础配置信息

这是 Endpoint 暴露的 json 示例:
```json
{
  "subscribe": [
    {
      "jsonFromServer": "",
      "name": "nacos-provider",
      "clusters": "",
      "cacheMillis": 10000,
      "hosts": [
        {
          "instanceId": "30.5.124.156#8081#DEFAULT#nacos-provider",
          "ip": "30.5.124.156",
          "port": 8081,
          "weight": 1.0,
          "healthy": true,
          "enabled": true,
          "cluster": {
            "serviceName": null,
            "name": null,
            "healthChecker": {
              "type": "TCP"
            },
            "defaultPort": 80,
            "defaultCheckPort": 80,
            "useIPPort4Check": true,
            "metadata": {
            }
          },
          "service": null,
          "metadata": {
          }
        }
      ],
      "lastRefTime": 1541755293119,
      "checksum": "e5a699c9201f5328241c178e804657e11541755293119",
      "allIPs": false,
      "key": "nacos-provider",
      "valid": true
    }
  ],
  "NacosDiscoveryProperties": {
    "serverAddr": "127.0.0.1:8848",
    "endpoint": "",
    "namespace": "",
    "logName": "",
    "service": "nacos-provider",
    "weight": 1.0,
    "clusterName": "DEFAULT",
    "metadata": {
    },
    "registerEnabled": true,
    "ip": "30.5.124.201",
    "networkInterface": "",
    "port": 8082,
    "secure": false,
    "accessKey": "",
    "secretKey": ""
  }
}
```
<a name="ISAKH"></a>
## 5、关于 Nacos Discovery Starter 更多的配置项信息
更多关于 Nacos Discovery Starter 的配置项如下所示:

| 配置项 | Key | 默认值 | 说明 |
| --- | --- | --- | --- |
| 服务端地址 | `spring.cloud.nacos.discovery.server-addr` |  | Nacos Server 启动监听的ip地址和端口 |
| 服务名 | `spring.cloud.nacos.discovery.service` | `${spring.application.name}` | 注册的服务名 |
| 权重 | `spring.cloud.nacos.discovery.weight` | `1` | 取值范围 1 到 100，数值越大，权重越大 |
| 网卡名 | `spring.cloud.nacos.discovery.network-interface` |  | 当IP未配置时，注册的IP为此网卡所对应的IP地址，如果此项也未配置，则默认取第一块网卡的地址 |
| 注册的IP地址 | `spring.cloud.nacos.discovery.ip` |  | 优先级最高 |
| 注册的端口 | `spring.cloud.nacos.discovery.port` | `-1` | 默认情况下不用配置，会自动探测 |
| 命名空间 | `spring.cloud.nacos.discovery.namespace` |  | 常用场景之一是不同环境的注册的区分隔离，例如开发测试环境和生产环境的资源（如配置、服务）隔离等 |
| AccessKey | `spring.cloud.nacos.discovery.access-key` |  | 当要上阿里云时，阿里云上面的一个云账号名 |
| SecretKey | `spring.cloud.nacos.discovery.secret-key` |  | 当要上阿里云时，阿里云上面的一个云账号密码 |
| Metadata | `spring.cloud.nacos.discovery.metadata` |  | 使用Map格式配置，用户可以根据自己的需要自定义一些和服务相关的元数据信息 |
| 日志文件名 | `spring.cloud.nacos.discovery.log-name` |  |  |
| 集群 | `spring.cloud.nacos.discovery.cluster-name` | `DEFAULT` | Nacos集群名称 |
| 接入点 | `spring.cloud.nacos.discovery.endpoint` |  | 地域的某个服务的入口域名，通过此域名可以动态地拿到服务端地址 |
| 是否集成Ribbon | `ribbon.nacos.enabled` | `true` | 一般都设置成true即可 |
| 是否开启Nacos Watch | `spring.cloud.nacos.discovery.watch.enabled` | `true` | 可以设置成false来关闭 watch |

