JavaSpring<br />Spring 6 是一个重要的版本，距离 Spring 5 发布有四年多了。一起来快速了解 Spring 6发行版中的那些令人兴奋的特性。
<a name="jrH2B"></a>
### JDK 17+ 和 Jakarta EE 9+ 基线

- 现在基于Java 17 源代码级别的整个框架代码库。
- Servlet、JPA 等从 javax 迁移到 Jakarta 命名空间。
- 运行时与 Jakarta EE 9 以及 Jakarta EE 10 API 的兼容性。
- 与最新的Web服务器兼容：Tomcat 10.1、Jetty 11、Undertow 2。
- 早期兼容虚拟线程（从 JDK 19 开始预览）。
<a name="t4AQT"></a>
### 一般核心修订

- 升级到 ASM 9.4 和 Kotlin 1.7。
- 完整的 CGLIB fork，支持捕获 CGLIB 生成的类。
- 全面的向 AOT（Ahead-Of-Time Processing，提前处理）转型。
- 对 GraalVM 原生映像的一流支持。
<a name="RBRAe"></a>
### 核心容器

- 默认情况下，无需 java.beans.Introspector 来确定基本 bean 属性。
- 在 `GenericApplicationContext (refreshForAotProcessing)` 中的支持 AOT 处理。
- 基于预解析构造函数和工厂方法的 Bean 定义转换。
- 支持 AOP 代理和配置类的早期代理类确定。
- `PathMatchingResourcePatternResolver` 使用 NIO 和模块路径 API 进行扫描，分别支持 GraalVM 本机映像和 Java 模块路径中的类路径扫描。
- `DefaultFormattingConversionService` 支持基于 ISO 的默认 java.time 类型解析。
<a name="IGQXH"></a>
### 数据访问和事务

- 支持预定 JPA 托管类型（用于包含在 AOT 处理中）。
- JPA 支持 Hibernate ORM 6.1（保持与 Hibernate ORM 5.6 的兼容性）。
- 升级到 R2DBC 1.0（包括 R2DBC 事务定义）。
- 删除 JCA CCI 支持。
<a name="OZjx2"></a>
### Spring 消息传递

- 基于 @RSocketExchange 服务接口的 RSocket 接口客户端。
- 基于 Netty 5 Alpha 的 Reactor Netty 2 的早期支持。
- 支持 Jakarta WebSocket 2.1 及其标准 WebSocket 协议升级机制。
<a name="hFZT9"></a>
### 通用 Web 修订

- 基于 `@HttpExchange` 服务接口的HTTP接口客户端。
- 支持 RFC 7807 问题详细信息.
- 统一 HTTP 状态码处理。
- 支持 Jackson 2.14。
- 与 Servlet 6.0 对齐（同时保留与 Servlet 5.0 的运行时兼容性）。
<a name="oXde8"></a>
### Spring MVC

- 默认情况下使用的 PathPatternParser（能够选择进入 PathMatcher）。
- 删除过时的 Tiles 和 FreeMarker JSP 支持。
<a name="FhHdc"></a>
### Spring WebFlux

- 新的 PartEvent API 用于流式传输多部分表单上传(两者都在客户端和服务器)。
- 新的 `ResponseEntityExceptionHandler` 用于自定义 WebFlux 异常并呈现 RFC 7807 错误响应。
- 非流媒体类型的 Flux 返回值（写入前不再收集到 List）。
- 基于 Netty 5 Alpha 的 Reactor Netty 2 的早期支持。
- JDK HttpClient 与 WebClient 集成。
<a name="ExIg1"></a>
### 可观察性

- Micrometer Observation 直接可观察性在 Spring 框架中的部分应用。spring-web 模块现在需要 io.micrometer:micrometer-observation:1.10+ 作为编译依赖项。
- RestTemplate 和 WebClient 被检测为生成 HTTP 客户端请求观察。
- Spring MVC 可以使用新的 `org.springframework.web.filter.ServerHttpObservationFilter` 检测 HTTP 服务器观察。
- Spring WebFlux 可以使用新的 `org.springframework.web.filter.reactive.ServerHttpObservationFilter` 检测 HTTP 服务器观察。
- 对于 Flux 和 Mono 的 Micrometer Context Propagation 集成，从控制器方法返回值。
<a name="uaO1g"></a>
### 测试

- 支持在 JVM 上或 GraalVM 本机映像中测试 AOT 处理的应用程序上下文。
- 集成 HtmlUnit 2.64 + 请求参数处理。
- Servlet 模拟（`MockHttpServletRequest`、`MockHttpSession`）现在基于 Servlet API 6.0。
<a name="weyYJ"></a>
### 参考引用

- [https://spring.io/blog/2022/11/16/spring-framework-6-0-goes-ga](https://spring.io/blog/2022/11/16/spring-framework-6-0-goes-ga)
