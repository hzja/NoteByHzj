<a name="oRD7X"></a>
## 服务监控
Spring Boot Actuator是一个用于监控和管理Spring Boot应用的子项目，它提供了一组REST端点和命令行工具， 用于查看应用的运行状态、性能指标和健康状况等。Actuator还支持应用度量数据的导出，以及自定义端点和安全控制等功能。通过使用Spring Boot Actuator，开发人员可以更加方便地了解应用的运行状况，及时发现和解决问题。
<a name="P0TAj"></a>
## 概述
随着微服务架构的普及，Spring Boot 已经成为Java开发人员的首选框架。然而，随着应用的规模不断扩大， 如何有效地监控和管理这些应用成为一个重要的问题。Spring Boot Actuator的出现，为开发人员提供了一个解决方案。本文将详细介绍Spring Boot Actuator的功能、工作原理、使用场景以及应用示例，帮助读者更好地理解和掌握这一工具。
<a name="H8ykl"></a>
## 功能简介

- 应用度量数据的导出：Actuator 可以将应用的运行数据导出到各种不同的存储后端，例如 Prometheus、Datadog、New Relic 等。这样，开发人员可以方便地使用这些数据来监控应用的性能和健康状况。
- REST 端点：Actuator 提供了一组 REST 端点，用于查看应用的运行状态、健康状况、度量数据等信息。开发人员可以通过 HTTP 请求来获取这些数据，并使用各种工具进行可视化展示。
- 命令行工具：除了 REST 端点之外，Actuator 还提供了一些命令行工具，例如 spring-boot-cli 和 spring-boot-admin。这些工具可以让开发人员更方便地管理和监控应用。
- 自定义端点：Actuator 支持自定义端点的开发，让开发人员可以根据自己的需求来暴露自定义的监控数据。这样可以更灵活地监控和管理应用。
- 安全控制：Actuator 支持对监控端点的安全控制，例如限制访问权限、身份验证等。这样可以保护应用的敏感信息不被泄露。
<a name="Ha81A"></a>
## Spring-Actuator
<a name="TEXKZ"></a>
### 默认监控服务
| 服务端点 | 描述 |
| --- | --- |
| auditevents | 公开当前应用程序的审核事件信息。 |
| beans | 显示应用程序中所有Spring bean的完整列表。 |
| caches | 公开可用的缓存 |
| conditions | 显示在配置和自动配置类上评估的条件以及它们匹配或不匹配的原因。 |
| configprops | 显示所有`@ConfigurationProperties`的有序列表。 |
| env | 公开Spring的`ConfigurableEnvironment`中的属性 |
| flyway | 显示已应用的任何Flyway数据库迁移。 |
| health | 显示应用健康信息。 |
| httptrace | 显示HTTP跟踪信息（默认情况下，最后100个HTTP请求 – 响应交换）。 |
| info | 显示任意应用信息。 |
| integrationgraph | 显示Spring集成图。 |
| loggers | 显示和修改应用程序中日志记录器的配置。 |
| liquibase | 显示已应用的任何Liquibase数据库迁移。 |
| metrics | 显示当前应用程序的“指标”信息。 |
| mappings | 显示所有`@RequestMapping`路径的有序列表。 |
| scheduledtasks | 显示应用程序中的计划任务。 |
| sessions | 允许从Spring Session支持的会话存储中检索和删除用户会话。使用Spring Session对响应式Web应用程序的支持时不可用。 |
| shutdown | 允许应用程序正常关闭。 |

http://localhost:8080/actuator
<a name="P5FFR"></a>
### 依赖
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```
<a name="yG7Xt"></a>
### 配置
```yaml
management:
  endpoints:
    web:
      exposure:
#        [health, info]
        include: "*"
```
<a name="jdSMI"></a>
### 自定义监控
监控端点相关注解：

- `@Endpoint`：定义一个监控端点，同时支持HTTP和JMX两种方式。
- `@WebEndpoint`：定义一个监控端点，只支持HTTP方式。
- `@JmxEndpoint`：定义一个监控端点，只支持JMX方式。
- `@ReadOperation`：作用在方法上，可用来返回端点展示的信息（通过 Get 方法请求）。
- `@WriteOperation`：作用在方法上，可用来修改端点展示的信息（通过 Post 方法请求）。
- `@DeleteOperation`：作用在方法上，可用来删除对应端点信息（通过 Delete 方法请求）。
- `@Selector`：作用在参数上，用来定位一个端点的具体指标路由。

自定义一个端点服务：
```java
@Endpoint(id = "custom")
public class CustomEndpoint {
    /**
     * /actuator/custom
     */
    @ReadOperation
    public Map custom() {
        return new HashMap();
    }

    /**
     * /actuator/custom/{name}?value={value}
     */
    @ReadOperation
    public Map name(@Selector String name, @Nullable String value) {
        return new HashMap();
    }
}
```
<a name="boIct"></a>
## Spring-Admin
Spring-Actuator主要实现数据的采集，以及提供REST API以及JMX的访问渠道，那么数据具体如何友好地显示出来？这时需要对应的UI，其中spring-boot-admin就是这样一款工具。<br />http://localhost:8080/applications
<a name="wvZu9"></a>
### 服务端
```xml
<dependency>
  <groupId>de.codecentric</groupId>
  <artifactId>spring-boot-admin-starter-server</artifactId>
</dependency>
```
```java
@EnableAdminServer
public class Application{   }
```
<a name="Z7Gv6"></a>
### 客户端
```xml
<dependency>
  <groupId>de.codecentric</groupId>
  <artifactId>spring-boot-admin-starter-client</artifactId>
  <version>2.6.2</version>
</dependency>
```
客户端配置
```yaml
spring:
  boot:
    admin:
      client:
        url: http://localhost:8080
```
<a name="LaOhX"></a>
## Prometheus + Grafana
上面说到，Actuator除了采集指标，提供访问API外，还提供了“应用度量数据的导出”的功能，这样就能将采集到的指标输出到指定的存储服务或终端以便进一步分析。其中Prometheus就是这样一个应用。

- Prometheus 时序数据库，用于存储数据，提供并提供查询，它存储了计算机系统在各个时间点上的监控数据
- Grafana 仪表盘，提供监控指标可视化界面。
<a name="nasiC"></a>
### 依赖
```xml
<dependency>
  <groupId>io.micrometer</groupId>
  <artifactId>micrometer-registry-prometheus</artifactId>
</dependency>
```
<a name="wignr"></a>
### 配置
```yaml
management:
  endpoints:
    web:
      exposure:
        include: "*"
  metrics:
    export:
      prometheus:
        enabled: true
  prometheus:
    enabled: true
```
<a name="qSrGq"></a>
### prometheus配置
```yaml
scrape_configs:
  - job_name: 'spring-boot-actuator'
    metrics_path: '/actuator/prometheus'
    static_configs:
      - targets: ['localhost:8080'] # 使用你的Spring Boot应用程序的实际主机和端口替换
```
<a name="YEtXN"></a>
### 启动
```bash
prometheus.exe --config.file=prometheus.yml

grafana-server.exe
```
<a name="ZQ84U"></a>
## 问题
<a name="WTZn4"></a>
### 服务端点
由于项目使用spring-boot版本为_2.3.7.RELEASE_，而spring-boot-admin-starter-server版本设置设置为2.7.x版本时，UI相关配置一直无法加载，通过源码可以看到<br />在2.6.x版本中对应spring-boot-admin-server-ui存在META-IN\spring.factories文件
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
  de.codecentric.boot.admin.server.ui.config.AdminServerUiAutoConfiguration
```
而在2.7.x版本中，spring.factories删除了且改为了 META-INF\spring\org.springframework.boot.autoconfigure.AutoConfiguration.imports
```
de.codecentric.boot.admin.server.ui.config.AdminServerUiAutoConfiguration
```
因此如果需要使用2.7.x版本的spring-boot-admin，记得把spring-boot升级到2.7.x
<a name="I3YHY"></a>
### 参数名称
参数名称被解析为arg0，导致请求匹配失败。通过下面的配置保证编译后的文件通过反射获取的参数名称不变
```xml
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-compiler-plugin</artifactId>
  <version>3.11.0</version>
  <configuration>
    <debug>false</debug>
    <!-- 防止方法参数名解析为arg0...  -->
    <compilerArgs>
      <arg>-parameters</arg>
    </compilerArgs>
  </configuration>
</plugin>
```
如果使用Idea，可以在应用启动后，Actuator功能面板的Mappings中看到服务地址的变化
<a name="rPkIw"></a>
## 结束语
服务监控是为了更好地了解服务运行状况，及时发现服务可能出现的问题，并在出现故障时能够有效地定位问题产生的原因。更大层面解决系统运行过程中的维护 成本。关于监控相关的应用还有一些，比如SkyWalking、Zipkin、Elastic APM等等。
