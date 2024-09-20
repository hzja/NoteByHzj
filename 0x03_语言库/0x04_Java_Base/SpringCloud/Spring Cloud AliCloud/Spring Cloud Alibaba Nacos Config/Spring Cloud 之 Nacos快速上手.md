JavaSpringCloud AlibabaNacos<br />Spring Cloud Alibaba 是阿里巴巴提供的一站式微服务开发解决方案，目前已被 Spring Cloud 官方收录。而 Nacos 作为 Spring Cloud Alibaba 的核心组件之一，提供了两个非常重要的功能：注册中心和配置中心，来了解和实现一下二者。
<a name="xE975"></a>
## 1、Nacos 简介
Nacos 致力于帮助开发者发现、配置和管理微服务。它提供了一组简单易用的特性集，帮助开发者快速实现动态服务发现、服务配置、服务元数据及流量管理。
<a name="Cmd83"></a>
### Nacos 特性介绍
Nacos 具有以下特性：

- 服务发现和服务健康监测：支持基于DNS和基于RPC的服务发现，支持对服务的实时的健康检查，阻止向不健康的主机或服务实例发送请求。
- 动态配置服务：动态配置服务可以让您以中心化、外部化和动态化的方式管理所有环境的应用配置和服务配置。
- 动态 DNS 服务：动态 DNS 服务支持权重路由，让您更容易地实现中间层负载均衡、更灵活的路由策略、流量控制以及数据中心内网的简单 DNS 解析服务。
- 服务及其元数据管理：支持从微服务平台建设的视角管理数据中心的所有服务及元数据。
<a name="gNcmK"></a>
## 2、注册中心实现
注册中心有两个重要的功能：服务注册和服务发现，它解决了微服务集群中，调用者和服务提供者连接管理和请求转发的功能，让程序的开发者无需过多的关注服务提供者的稳定性和健康程度以及调用地址，因为这些都可以依靠 Nacos 进行监测、管理和自动转发。<br />注册中心中有两个角色：一个是服务提供者 Provider，另一个是服务调用者 Consumer，接下来分别来创建二者。
<a name="gSf9L"></a>
### 2.1 创建服务提供者
<a name="aXIfg"></a>
#### 2.1.1 新建项目并添加依赖
新建一个 Spring Boot 项目，使用阿里云地址 http://start.aliyun.com 来创建 Spring Cloud Alibaba Nacos 项目，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970418069-1e39e63f-ffd4-4c38-aaca-9a62403e09c9.png#averageHue=%233d4143&clientId=u1690f7f4-2881-4&from=paste&id=u1d3bda36&originHeight=497&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u13e96807-f5a0-462f-a23f-0461519a0df&title=)点击 Next 下一步，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970417954-d5367b92-c2f3-4171-8459-cf201aae20a0.png#averageHue=%233e4144&clientId=u1690f7f4-2881-4&from=paste&id=udbeb96b6&originHeight=497&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8c23fd23-cd61-4859-852d-62737b23129&title=)增加 Nacos Service Discovery 框架支持，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970418004-cc6da250-3ad1-48ef-98d6-83acf2df9471.png#averageHue=%233d4246&clientId=u1690f7f4-2881-4&from=paste&id=ub6996fa8&originHeight=497&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u65124960-c885-4778-94cb-2a71946bc5b&title=)对应的依赖框架如下：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>
```
PS：添加 Spring Web（Spring MVC）框架主要是为了方便后面测试。
<a name="mQG5j"></a>
#### 2.1.2 配置 Nacos 连接信息
在配置文件 application.properties 中要填写 Nacos 的相关连接信息，具体信息如下：
```
# 应用名称（也是 Nacos 中的服务名）
spring.application.name=spring-cloud-nacos-producer
# 应用服务 WEB 访问端口
server.port=8082
# Nacos认证信息
spring.cloud.nacos.discovery.username=nacos
spring.cloud.nacos.discovery.password=nacos
# Nacos 服务发现与注册配置，其中子属性 server-addr 指定 Nacos 服务器主机和端口
spring.cloud.nacos.discovery.server-addr=127.0.0.1:8848
# 注册到 nacos 的指定 namespace，默认为 public
spring.cloud.nacos.discovery.namespace=public
```
<a name="Qd9MQ"></a>
#### 2.1.3 添加服务提供方法
新建一个控制器 Controller，添加一个 sayhi 方法，此方法可以使用 HTTP 协议进行访问，它是为后面的服务消费者提供的调用方法，具体实现如下：
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
@RestController
public class SpringCloudNacosProducerApplication {
    public static void main(String[] args) {
        SpringApplication.run(SpringCloudNacosProducerApplication.class, args);
    }
    @RequestMapping("/sayhi/{name}")
    public String sayHi(@PathVariable String name) {
        return "Hi Nacos Discovery " + name;
    }
}
```
编写完代码之后，运行项目就可以在 Nacos 的服务列表中看到它了，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970417913-3413fa44-c374-4e5b-aa7a-eccd13dbb02d.png#averageHue=%23d1cabd&clientId=u1690f7f4-2881-4&from=paste&id=u837c0796&originHeight=574&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4d7dbab8-fc6c-4f3f-9e8f-a2aa567013d&title=)经过以上步骤，服务提供者就创建好了，接下来创建一个服务消费者 Consumer。
<a name="xOgWx"></a>
### 2.2 创建服务消费者
<a name="Mxc4D"></a>
#### 2.2.1 新建项目并添加依赖
此步骤和上面的服务提供者类似，也是添加 nacos discovery 和 web 依赖，具体实现如下：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970419010-b19f5219-33e2-474a-b7a0-1c973a498f93.png#averageHue=%233d4043&clientId=u1690f7f4-2881-4&from=paste&id=u4d05050d&originHeight=572&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub9fc8a80-d456-472f-bd0a-a7fd6322e92&title=)![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970425587-4af09058-d22b-4d1b-a6ca-a32e66dd4889.png#averageHue=%233d4144&clientId=u1690f7f4-2881-4&from=paste&id=ua7d18fb6&originHeight=572&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9c5d3f8b-c644-489c-8124-e2aab5dc633&title=)
<a name="Q2n8E"></a>
#### 2.2.2 配置 Nacos 连接信息
```
# 应用名称
spring.application.name=springcloud-nacos-consumer
# 应用服务 WEB 访问端口
server.port=8082
# Nacos认证信息
spring.cloud.nacos.discovery.username=nacos
spring.cloud.nacos.discovery.password=nacos
# Nacos 服务发现与注册配置，其中子属性 server-addr 指定 Nacos 服务器主机和端口
spring.cloud.nacos.discovery.server-addr=127.0.0.1:8848
# 注册到 nacos 的指定 namespace，默认为 public
spring.cloud.nacos.discovery.namespace=public
```
<a name="xqEKm"></a>
#### 2.2.3 添加服务调用代码
服务消费者的实现有两个关键点，第一，先 new 一个 RestTemplate 对象，此对象是 Spring 框架提供用于进行 HTTP 请求的类，实现代码如下：
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;
import org.springframework.cloud.client.loadbalancer.LoadBalanced;
import org.springframework.context.annotation.Bean;
import org.springframework.web.client.RestTemplate;

@SpringBootApplication
@EnableDiscoveryClient
public class SpringcloudNacosConsumerApplication {
    public static void main(String[] args) {
        SpringApplication.run(SpringcloudNacosConsumerApplication.class, args);
    }
    @LoadBalanced
    @Bean
    public RestTemplate restTemplate() {
        // 用于进行 HTTP 请求的对象
        return new RestTemplate();
    }
}
```
第二，新建一个控制器，注入 RestTemplate 对象，并调用 Nacos 中的服务提供者接口，实现代码如下：
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

@RestController
public class TestController {
    @Autowired
    private RestTemplate restTemplate;

    @RequestMapping("/hi")
    public String hi(String name) {
        // 调用生产者 sayhi 方法，并返回结果
        return restTemplate.getForObject("http://spring-cloud-nacos-producer/sayhi/" + name,
                String.class);
    }
}
```
其中 spring-cloud-nacos-producer 是服务提供者的 ID，而 sayhi 是服务提供者提供的接口地址。<br />通过以上配置，服务消费者也创建好了，启动项目，执行结果如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970424461-19f2ef5c-de69-4c43-b4bb-f18116d1b5c4.png#averageHue=%23d9cfbf&clientId=u1690f7f4-2881-4&from=paste&id=u2986ae3a&originHeight=167&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uece8b5a2-a23d-4976-910f-f8f29f98de2&title=)从上述结果可以看出，服务消费者通过 Nacos 已经成功调用到服务提供者了，这就是 Nacos 中注册中心的基本使用。
<a name="tIO5v"></a>
## 3、配置中心实现
配置中心的作用是将本地配置文件云端话，所谓的云端也就是 Nacos 的服务器端，这样既能保证配置文件中的敏感数据不会暴露，同时又提供了实时的修改、查看、回滚和动态刷新配置文件的功能，非常实用。<br />但是需要注意的是**配置中心和注册中心的依赖包是不同的**，注册中心的依赖包是 nacos discovery，而配置中心的依赖包是 nacos config，它的具体如下。
<a name="NIdzX"></a>
### 3.1 新建项目并添加依赖
创建一个 Spring Boot 项目，添加 nacos config 和 web 依赖包，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970424641-507b7131-3b01-4a45-a141-9d7f76bf5877.png#averageHue=%233d4145&clientId=u1690f7f4-2881-4&from=paste&id=u6e9c61b5&originHeight=574&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uce491b43-e43b-4781-a27f-723e9edb7e6&title=)<br />PS：添加 web 依赖主要是为了方便后面测试。<br />对应的配置依赖信息如下：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
</dependency>
```
<a name="DSFK7"></a>
### 3.2 配置 Nacos Config 信息
在应用的 /src/main/resources/ 目录下，创建引导配置文件 bootstrap.yml（或 bootstrap.properties），添加以下 Nacos Config 配置：
```yaml
spring:
  application:
    name: nacosconfig # 项目名称和 nacos DataId 相匹配
  cloud:
    nacos:
      config:
        server-addr: 127.0.0.1:8848 # nacos 地址和端口
        file-extension: yaml # 获取配置文件的格式：yaml
        username: nacos # nacos 认证用户名
        password: nacos # nacos 认证密码
server:
  port: 9001 # 项目启动端口
```
<a name="hE2qS"></a>
### 3.3 编写代码读取配置文件
新建控制器，使用 `@Value` 注解读取配置信息，实现代码如下：
```java
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class TestController {
    // 从 nacos 中读取配置项 config.info
    @Value("${config.info}")
    private String configInfo;

    @GetMapping("/getconfig")
    public String getConfigInfo(){
        return configInfo;
    }
}
```
<a name="vd01Q"></a>
### 3.4 Nacos 控制台添加配置信息
在 Nacos 控制台创建并设置配置文件，执行步骤如下所示。首先，在配置列表中点击“添加”按钮，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970424490-36ffb742-598c-49d0-9a03-5edc8ee0e288.png#averageHue=%23d4cec0&clientId=u1690f7f4-2881-4&from=paste&id=u1fc16127&originHeight=414&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua29a0c35-c3ef-423b-b20f-b1141f53ab1&title=)进入配置页面，新建 YAML 或 Properties 配置文件，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970424570-85ee30a0-3529-44f0-a978-8c6beed40a17.png#averageHue=%23a79586&clientId=u1690f7f4-2881-4&from=paste&id=ue692f69a&originHeight=476&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8bbac5f8-eaaa-4028-8c41-d21d96697fd&title=)以上关键参数的含义和规则说明如下。
<a name="DIcye"></a>
#### 3.4.1 Data ID
Data ID 的拼接格式如下：
```
${prefix} - ${spring.profiles.active} . ${file-extension}
```
其中

- prefix 默认为 `spring.application.name` 的值，也可以通过配置项 `spring.cloud.nacos.config.prefix` 来配置。
- `spring.profiles.active` 即为当前环境对应的 profile，**当 active profile 为空时，对应的连接符 - 也将不存在，dataId 的拼接格式变成 **`**${prefix}.${file-extension}**`
- file-extension 为配置内容的数据格式，可以通过配置项 `spring.cloud.nacos.config.file-extension` 来配置。目前只支持 properties 类型。
<a name="wNiud"></a>
#### 3.4.2 Group
Group 分组选项，主要是用来隔离不同的配置项目的，它的默认值为 `DEFAULT_GROUP`，可以通过 `spring.cloud.nacos.config.group` 配置。<br />配置好相应的内容之后，点击底部的“发布”按钮即可，添加成功之后会自动返回配置列表，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970424921-52f9d445-b31f-4dee-893d-2a3cfc122b93.png#averageHue=%23dcdad0&clientId=u1690f7f4-2881-4&from=paste&id=u3b63b524&originHeight=400&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u30972e17-e8c9-4cd3-ada8-bf48a647c3b&title=)经过以上步骤，Nacos 配置中心的功能就实现完了，接下来启动项目，程序的执行结果如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678970424985-def1843b-f50d-4aeb-a1a3-96ca9dd3c663.png#averageHue=%23e8d9aa&clientId=u1690f7f4-2881-4&from=paste&id=uf6617a35&originHeight=186&originWidth=1007&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucb5f3535-2f2a-45f7-aa70-51785b2704d&title=)
<a name="YLtmm"></a>
### 3.5 动态刷新功能
**动态刷新功能是指，在 Nacos 配置中心修改了配置文件，在不重启项目的前提下，可以实时读取到最新的配置内置。**<br />Nacos 默认会为所有获取数据成功的 Nacos 的配置项添加了监听功能，在监听到服务端配置发生变化时会实时触发 `org.springframework.cloud.context.refresh.ContextRefresher` 的 refresh 方法。<br />但**如果需要对 Bean 进行动态刷新**，需要参照 Spring 和 Spring Cloud 规范，**推荐给类添加 **`**@RefreshScope**`** 或 **`**@ConfigurationProperties**`** 注解**，就可以实现配置中心的动态刷新功能了。
<a name="PqUCf"></a>
## 小结
Nacos 作为 Spring Cloud Alibaba 的核心组件之一，提供了两个非常重要的功能：注册中心和配置中心。注册中心有两个重要的功能：服务注册和服务发现，它解决了微服务集群中，调用者和服务提供者连接管理和请求转发的功能，保证了服务调用者能够稳定的调用到健康的服务。而注册中心的本质是将项目中的本地配置文件云端化，解决了配置文件的安全性与统一性的问题，并且提供了配置文件历史版本回滚和配置文件动态刷新的功能。
<a name="opriv"></a>
## 参考文档
Nacos 官方文档：[https://nacos.io/zh-cn/docs/what-is-nacos.html](https://nacos.io/zh-cn/docs/what-is-nacos.html)
