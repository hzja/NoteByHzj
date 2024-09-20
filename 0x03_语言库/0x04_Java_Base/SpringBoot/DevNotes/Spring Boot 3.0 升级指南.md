JavaSpringBoot<br />Spring Boot 3.0 现已正式发布，它包含了 12 个月以来 151 个开发者的 5700 多次代码提交。这是自 4.5 年前发布 2.0 以来，Spring Boot 的第一次重大修订。<br />它也是第一个支持 Spring Framework 6.0 和 GraalVM 的 Spring Boot GA 版本，同时也是第一个基于Java 17的Spring Boot版本，以Jakarta EE 9为基准并支持Jakarta EE 10。同时借助于GraalVM支持了AOT和Native Image。
<a name="CmBa1"></a>
## 3.0升级指南
<a name="z4JhA"></a>
### 升级JDK
首先要升级JDK到JDK17，如果要使用Native Image特性，要使用GraalVM 22.3以上和 Native Build Tools Plugin 0.9.17 或更高的版本。
<a name="dC11i"></a>
### 平滑升级
这里不建议直接从低于Spring Boot 2.7的版本直接升级到Spring Boot 3.0。不然新特性和API变更太多，就需要修改大量的配置，升级路径会过于陡峭。建议通过小版本号分阶段升级到2.4、2.6、2.7，最后再到3.0。
<a name="yEuvY"></a>
### 依赖评估
Spring Boot3.0下依赖组件的版本要求也不尽相同，比如Spring Boot Kafka Starter可能对Kafka有什么要求，这要求你进行充分评估。Spring Cloud体系应该在对应的Spring Boot 3.0体系发布后进行升级。
<a name="ZxxqE"></a>
### 升级到Spring Boot 3
一旦上面的工作准备完毕，就可以开始尝试升级到Spring Boot 3.0了。
<a name="aGt1W"></a>
#### 配置属性兼容
在Spring Boot 3.0中，一些配置属性被重新命名/删除了，开发人员需要相应地更新他们的application.properties或者application.yml。为了帮助你做到这一点，Spring Boot 3.0提供了一个spring-boot-properties-migrator模块。一旦作为依赖关系添加到你的项目中，它不仅会在启动时分析你的应用程序的环境并打印诊断结果，而且还会在运行时为你临时迁移属性。把下面这个加入pom.xml以帮助你升级兼容配置属性：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-properties-migrator</artifactId>
  <scope>runtime</scope>
</dependency>
```
完成迁移之后，务必要移除spring-boot-properties-migrator依赖。
<a name="um7R1"></a>
#### Jakarta EE
由于Java EE已经变更为Jakarta EE，包名以 javax开头的需要相应地变更为jakarta。这里建议通过**Intellij IDEA 2021.2**提供的内置功能批量处理即可：<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1669364351615-ef90f6cb-133e-4e42-be2b-e7a5e3ad59e7.gif#averageHue=%23414248&clientId=uc36c3b1c-2096-4&from=paste&id=uc62b520c&originHeight=432&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4e61d470-c3d8-4087-bf8c-9bbb527cf4f&title=)<br />当然也可以手动去变更包名。
<a name="LxItT"></a>
#### 升级的关键变更
Spring Boot 3.0有一些关键改变，这些改变将与大多数应用程序有关。所以开发者要仔细阅读这些变更。
<a name="A0X5T"></a>
##### 图片Banner不再支持
现在Spring Boot 3.0自定义Banner只支持文本类型（banner.txt），不再支持图片类型。
<a name="K594M"></a>
##### 日志日期格式变更
Logback和Log4j2的日志消息的日期和时间部分的默认格式已经改变，以符合**ISO-8601**标准。新的默认格式`yyyy-MM-dd'T'HH:mm:ss.SSSXXX`使用`T`来分隔日期和时间，而不是空格字符，并在最后添加时区偏移。`LOG_DATEFORMAT_PATTERN`环境变量或`logging.pattern.dateformat`属性可以用来恢复以前的默认值yyyy-MM-dd HH:mm:ss.SSS。
<a name="WMAAu"></a>
##### `ConstructingBinding`注解
配置属性类注解`@ConfigurationProperties`默认情况下不再需要使用`@ConstructorBinding`来标记构造，你应该从配置类上移除它，除非该配置类多个构造函数来明确配置属性绑定。
<a name="QTh3C"></a>
##### `YamlJsonParser`被移除
因为`SnakeYAML`的JSON解析与其他解析器的实现不一致。如果直接使用`YamlJsonParser`，请迁移到其他`JsonParser`实现中。
<a name="VUgtv"></a>
##### 自动配置文件变更
Spring Boot 2.7引入了一个新的文件：
```
META-INF/spring/org.springframework.boot.autoconfigure.AutoConfiguration.imports
```
用于注册自动配置，同时保持与 `spring.factories` 中注册的向后兼容。Spring Boot 3.0发布后，`spring.factories`已经被移除，只能通过`imports`文件来注册自动配置。
<a name="NEioY"></a>
##### Web应用变更
<a name="kSHOD"></a>
###### 路径匹配
现在Spring MVC和Spring Webflux 的路径匹配规则已经做了调整，默认情况下尾部斜杠`/`的匹配机制将和以前不同：

- 3.0以前/foo/bar等同于/foo/bar/
- 3.0以后/foo/bar**不同**于/foo/bar/
<a name="s0fsT"></a>
###### 弃用配置 `server.max-http-header-size`
`server.max-http-header-size`已经被弃用，现在用`server.max-http-request-header-size`替代 ，并且现在该属性只能来配置请求头的大小，不再限制某些Web容器的响应头大小，如果需要限制响应头大小请通过自定义`WebServerFactoryCustomizer`来实现。
<a name="DiHyi"></a>
###### 优雅停机阶段变更
优雅停机由`SmartLifecycle`实现，在SmartLifecycle.DEFAULT_PHASE - 2048阶段开始，Web服务器在SmartLifecycle.DEFAULT_PHASE - 1024阶段停止。任何参与优雅关机的SmartLifecycle实现都应该被相应地更新。
<a name="kMhiU"></a>
###### Jetty
由于Jetty目前还不支持Servlet 6.0，使用Jetty的开发者需要通过降低**Jakarta Servlet**的版本为5.0。
<a name="QCgba"></a>
##### 数据访问相关的变更
`spring.data`前缀已被保留给Spring Data项目，如果你使用了Spring Data项目的子项目，application.yaml中相关的配置属性就需要响应的变更。例如 `spring.redis`现在需要修改为`spring.data.redis`。<br />数据访问相关的组件现在都使用了比较新的版本：

- **Flyway**更新到 **9.0**
- **Hibernate** 更新到 **6.1**
- **R2DBC** 更新到 **1.0**
<a name="l0g8X"></a>
###### Mongodb
另外 Flapdoodle 嵌入式 MongoDB 的自动配置被移除，开发者有需要的话需要自行引入。
<a name="bLyZr"></a>
###### Elasticsearch
Elasticsearch的Rest客户端采用了新的API实现，旧的自动配置和Spring Data Elasticsearch中旧的客户端API已经被移除。
<a name="HRZU4"></a>
##### Spring Security
<a name="SD7iF"></a>
##### 其它变更
可观测性、度量跟踪、Maven、Gradle也都有不同程度的变更，有需要可以查看官方的Spring-Boot-3.0-Migration-Guide 。
