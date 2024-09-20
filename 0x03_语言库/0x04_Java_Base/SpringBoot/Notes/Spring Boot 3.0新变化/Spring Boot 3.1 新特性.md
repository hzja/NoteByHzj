JavaSpringBoot
<a name="EIhyO"></a>
## Spring Boot 3.1 正式发布
Spring Boot 3.0 发布半年左右，Spring Boot 3.1 正式发布了。<br />同时发布更新的还有 2.7.x 版本，同时，2.6.x 版本线已经停止维护了。<br />2.7.x 这也是目前唯一正在维护的 2.x 版本线了，商业支持的版本也只有 2.5.x 了。
```xml
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>3.1.0</version>
    <relativePath/> <!-- lookup parent from repository -->
  </parent>
```
<a name="onpoK"></a>
## Spring Boot 3.0 重大新特性
Spring Boot 3.1 更新了不少内容，简单分享下几个重要的更新。
<a name="awy5A"></a>
### 亮点

- Spring Authorization Server 的自动配置
- 支持在开发时使用 Testcontainers 和 Docker Compose 管理外部服务
- 简化集成测试中 Testcontainers 的配置
- 支持 Mockito 单元测试
<a name="v8E6g"></a>
### 1、最低环境要求
Spring Boot 3.0 的发布，其最低要求 Java 17，并向上兼容支持 Java 19，Spring Boot 3.1 可以支持 Java 20 了，因为前段时间 Java 20 发布了。<br />**对 Java 开发环境的要求对比表：**

| Spring Boot | JDK | Spring | Maven | Gradle |
| --- | --- | --- | --- | --- |
| 3.1.0 | 17 ~ 20 | 6.0.9+ | 3.6.3+ | 7.5+,8.x |
| 3.0.0 | 17 ~ 19 | 6.0.2+ | 3.5+ | 7.5+ |
| 2.7.12 | 8 ~ 20 | 5.3.27+ | 3.5+ | 6.8.x, 6.9.x, 7.x, 8.x |

<a name="WMqF1"></a>
### 2、大量依赖升级
Spring Boot 3.1 最低支持的 Spring 框架也变成了 Spring 6.0.9+，除此之外，Spring Boot 管理的大量第三方的技术依赖也有大幅度的的升级，具体可以参考官方版本发布文档：<br />[https://github.com/spring-projects/spring-boot/releases/tag/v3.1.0](https://github.com/spring-projects/spring-boot/releases/tag/v3.1.0)<br />以上这些技术的升级对于 Spring Boot 应用的升级也是一头拦路虎。
<a name="bnxKM"></a>
### 3、移除 HttpClient 4
因为 Spring 6 删除了 RestTemplate 对 Apache HttpClient 4 的支持，取而代之的则是 Apache HttpClient 5。<br />而 Spring Boot 3.0 又同时包含了 HttpClient 4 和 HttpClient 5 两个依赖的管理，如果同时管理这两个版本，有的人可能会继续使用 HttpClient 4，应用程序在使用 RestTemplate 时可能会有潜在的错误，导致诊断困难。<br />所以，Spring Boot 3.1 移除了 HttpClient 4 的依赖管理，转而逼迫用户强制使用 HttpClient 5，大家有依赖 HttpClient 4 的升级时需要注意了。
<a name="sU98C"></a>
### 4、Servlet 和 Filter 注册失败时抛异常
大家知道，Spring Boot 注册 Servlet 和 Filter 时用的 `ServletRegistrationBean` 和 `FilterRegistrationBean` 这两个类，之前注册失败时是记录 warn 级别的日志，现在则是直接抛出 `IllegalStateException` 异常。<br />如果不想注册失败抛出异常，还是想像以前一样记录 warn 警告日志，可以调用这两个类的 `setIgnoreRegistrationFailure(true)` 方法，即忽略注册失败，使用示例如下：
```java
@Bean
public ServletRegistrationBean registerServlet() {
    ServletRegistrationBean servletRegistrationBean = new ServletRegistrationBean(new RegisterServlet(), "/registerServlet");
    servletRegistrationBean.addInitParameter("type", "blog");
    servletRegistrationBean.addInitParameter("name", "Java");
    servletRegistrationBean.setIgnoreRegistrationFailure(true);
    return servletRegistrationBean;
}
```
<a name="BM5m6"></a>
### 5、Spring Authorization Server 自动配置
Spring Boot 3.1 提供了一个 spring-boot-starter-oauth2-authorization-server 启动器，可以支持 Spring Authorization Server 的自动配置，轻松配置基于 Servlet 的 OAuth2 授权服务器。<br />更多参考官方文档：<br />[https://docs.spring.io/spring-boot/docs/3.1.0/reference/html/web.html#web.security.oauth2.authorization-server](https://docs.spring.io/spring-boot/docs/3.1.0/reference/html/web.html#web.security.oauth2.authorization-server)
<a name="NWu9U"></a>
### 6、Docker Compose 新模块
Docker Compose 是一个用于定义和运行多个 Docker 容器的工具，即官方容器编排工具，它可以轻松地定义多个容器之间的依赖关系、环境变量、网络设置等，并通过一个命令即可创建、启动和连接这些容器。<br />Spring Boot 3.1 提供了一个新模块：spring-boot-docker-compose，它支持与 Docker Compose 集成，当 Spring Boot 应用启动时，它会在当前工作目录中查找配置文件。<br />支持以下几种配置文件：

- compose.yaml
- compose.yml
- docker-compose.yaml
- docker-compose.yml

要使用非标准文件，请设置 spring.docker.compose.file 属性。<br />默认情况下，配置文件中声明的服务将使用 docker-compose up 启动，并将这些服务的连接详细信息 bean 添加到应用程序上下文中，以便可以在不进行任何进一步配置的情况下使用这些服务。当应用程序停止时，将使用 docker-compose down 关闭服务。可以使用 spring.docker.compose.lifecycle-management、spring.docker.compose.startup.command 和 spring.docker.compose.shutdown.command 配置属性自定义此生命周期管理及用于启动和关闭服务的命令。<br />更多参考官方文档：<br />[https://docs.spring.io/spring-boot/docs/3.1.0/reference/html/features.html#features.docker-compose](https://docs.spring.io/spring-boot/docs/3.1.0/reference/html/features.html#features.docker-compose)
<a name="IrMDJ"></a>
### 7、SSL 配置
Java KeyStore 和 PEM 编码证书等 SSL 信任相关的资料，现在可以使用 properties 属性进行配置，并且可以适用于各种类型的连接，例如嵌入式 Web 服务器、数据服务、RestTemplate 和 WebClient。<br />更多参考官方文档：<br />[https://docs.spring.io/spring-boot/docs/3.1.0/reference/html/features.html#features.ssl](https://docs.spring.io/spring-boot/docs/3.1.0/reference/html/features.html#features.ssl)
<a name="SkLx1"></a>
### 8、服务连接
Spring Boot 3.1 引入了一个新的服务连接概念，这是一个低级别的功能，旨在作为其他高级功能的构建块，这些高级功能可以通过定义 ConnectionDetails Bean 自动配置服务连接。<br />ConnectionDetails Bean 提供了建立、删除服务连接所需的详细信息，并且 Spring Boot 的自动配置也已经支持，当这些 Bean 可用时，它们将优先于任何与连接相关的配置参数，但是，与连接本身无关的配置参数（比如：控制连接池大小和行为的参数）仍将使用。
<a name="K7lf0"></a>
### 9、Git Commit ID Maven 插件版本属性
覆盖 io.github.git-commit-id:git-commit-id-maven-plugin 版本的属性已更新以匹配其 artifact 名称。适应此更改，在 pom.xml 文件中需要将 git-commit-id-plugin.version 替换为 git-commit-id-maven-plugin.version。
<a name="mqAd2"></a>
### 10、Spring Kafka 重试主题自动配置
如果使用了 Apache Kafka 的自动配置重试主题（spring.kafka.retry.topic.enabled: true），并使用具有最大延迟的指数回退，则所有最大延迟水平的重试现在都发送到相同的主题。以前，即使超过了最大延迟，每个重试都会使用一个单独的主题。
<a name="k9hUB"></a>
### 11、Testcontainers 的依赖管理
Spring Boot 的依赖管理现在包括 Testcontainers。如果需要，可以使用 testcontainers.version 属性覆盖由 Spring Boot 管理的版本。
<a name="vIacB"></a>
### 12、Hibernate 6.2
Spring Boot 3.1 升级到 Hibernate 6.2。请参阅 Hibernate 6.2 迁移指南，了解此更改可能对应用程序产生的影响。
<a name="cTP16"></a>
### 13、Jackson 2.15
Spring Boot 3.1 升级到 Jackson 2.15。请参阅 Jackson Wiki，了解此更改可能对应用程序产生的影响。<br />2.15 中显着的更改是引入了处理限制。要调整这些约束，请定义一个类似于以下内容的 `Jackson2ObjectMapperBuilderCustomizer`：
```java
@Bean
Jackson2ObjectMapperBuilderCustomizer customStreamReadConstraints() {
    return (builder) -> builder.postConfigurer((objectMapper) -> objectMapper.getFactory()
                                               .setStreamReadConstraints(StreamReadConstraints.builder().maxNestingDepth(2000).build()));
}
```
<a name="Mqcfu"></a>
### 14、升级 Mockito 5
Spring Boot 3.1 升级到 Mockito 5，具体是 5.3。请参阅 Mockito 发布说明，了解 5.x 系列中的显着更改。
<a name="Aq66F"></a>
### 15、Health Groups 默认验证健康状态
健康组成员的配置现在将在启动时进行验证。如果包含或排除了不存在的健康指标，则启动将失败。设置 management.endpoint.health.validate-group-membership 为 false 可以禁用此验证，恢复早期版本的行为。
<a name="sH5vz"></a>
### 16、服务连接
引入了新的服务连接概念，由 ConnectionDetails bean 在应用程序中表示。这些 bean 提供了建立与远程服务连接所需的必要详细信息，而 Spring Boot 的自动配置已更新为消耗 ConnectionDetails bean。当这些 bean 可用时，它们将优先于任何与连接相关的配置属性。控制连接池大小和行为等与连接本身无关的配置属性仍将使用。<br />这项低级功能旨在作为自动配置服务连接的其他更高级特性的构建块，通过定义 ConnectionDetails bean 来实现。。
<a name="SSvKI"></a>
### 17、Docker 镜像构建
<a name="GyFq6"></a>
#### 镜像创建日期和时间
spring-boot:build-image Maven 目标和 bootBuildImage Gradle 任务现在具有 createdDate 配置选项，可用于将生成图像元数据中的 Created 字段的值设置为用户指定的日期或 now 以使用当前日期和时间。有关详细信息，请参阅 Gradle 和 Maven 插件文档。
<a name="pH829"></a>
#### 镜像应用程序目录
spring-boot:build-image Maven 目标和 bootBuildImage Gradle 任务现在具有 applicationDirectory 配置选项，可用于设置在构建包消耗之前将应用程序内容上传到构建器映像中的位置。这也将是生成的映像中应用程序内容的位置。有关详细信息，请参阅 Gradle 和 Maven 插件文档。
<a name="NJPOa"></a>
## Spring Boot 最新支持版本
Spring Boot 的最新版本支持情况：

| 版本 | 发布时间 | 停止维护时间 | 停止商业支持 |
| --- | --- | --- | --- |
| 3.1.x | 2023-05-18 | 2024-05-18 | 2025-08-18 |
| 3.0.x | 2022-11-24 | 2023-11-24 | 2025-02-24 |
| 2.7.x | 2022-05-19 | 2023-11-18 | 2025-02-18 |
| 2.6.x | 2021-12-17 | 2022-11-24 | 2024-02-24 |
| 2.5.x | 2021-05-20 | 已停止 | 2023-08-24 |
| 2.4.x | 2020-11-12 | 已停止 | 2023-02-23 |
| 2.3.x | 2020-05-15 | 已停止 | 已停止 |
| 2.2.x | 2019-10-16 | 已停止 | 已停止 |
| 2.1.x | 2018-10-10 | 已停止 | 已停止 |
| 2.0.x | 2018-03-01 | 已停止 | 已停止 |
| 1.5.x | 2017-01-30 | 已停止 | 已停止 |

