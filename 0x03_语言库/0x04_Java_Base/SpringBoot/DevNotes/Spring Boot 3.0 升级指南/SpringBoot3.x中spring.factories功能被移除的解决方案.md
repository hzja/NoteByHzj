JavaSpringBoot
<a name="JkLt4"></a>
## 背景
项目组在搭建一个全新项目的时候选用了SpringBoot3.x，项目中应用了很多SpringBoot2.x时代相关的第三方组件例如baomidou出品的mybatis-plus、dynamic-datasource等。在配置好相关依赖、最小启动类和配置之后，发现项目无法启动。于是根据启动上下文日志和按行DEBUG找到原因并且在等待组件升级兼容之前进行临时性解决。
<a name="udaxW"></a>
## 关于spring.factories
spring.factories其实是SpringBoot提供的SPI机制，底层实现是基于SpringFactoriesLoader检索ClassLoader中所有jar（包括ClassPath下的所有模块）引入的META-INF/spring.factories文件，基于文件中的接口（或者注解）加载对应的实现类并且注册到IOC容器。这种方式对于`@ComponentScan`不能扫描到的并且想自动注册到IOC容器的使用场景十分合适，基本上绝大多数第三方组件甚至部分spring-projects中编写的组件都是使用这种方案。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672639844921-b34f038b-7b30-44ff-8684-df5c993311e6.png#averageHue=%23738876&clientId=udccb03c7-08d6-4&from=paste&id=ua1ade014&originHeight=687&originWidth=674&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u329225f4-e582-4e81-9554-2a772378bc0&title=)<br />spring.factories文件的格式大致如下：
```
# Initializers
org.springframework.context.ApplicationContextInitializer=\
org.springframework.boot.autoconfigure.SharedMetadataReaderFactoryContextInitializer,\
org.springframework.boot.autoconfigure.logging.ConditionEvaluationReportLoggingListener

# Application Listeners
org.springframework.context.ApplicationListener=\
org.springframework.boot.autoconfigure.BackgroundPreinitializer

# Environment Post Processors
org.springframework.boot.env.EnvironmentPostProcessor=\
org.springframework.boot.autoconfigure.integration.IntegrationPropertiesEnvironmentPostProcessor

# Auto Configuration Import Listeners
org.springframework.boot.autoconfigure.AutoConfigurationImportListener=\
org.springframework.boot.autoconfigure.condition.ConditionEvaluationReportAutoConfigurationImportListener

# Auto Configuration Import Filters
org.springframework.boot.autoconfigure.AutoConfigurationImportFilter=\
org.springframework.boot.autoconfigure.condition.OnBeanCondition,\
org.springframework.boot.autoconfigure.condition.OnClassCondition,\
org.springframework.boot.autoconfigure.condition.OnWebApplicationCondition

org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
com.baomidou.mybatisplus.autoconfigure.MybatisPlusLanguageDriverAutoConfiguration,\
com.baomidou.mybatisplus.autoconfigure.MybatisPlusAutoConfiguration
```
通用格式是：接口（或者注解）全类名=\接口实现类（或者使用了该注解的类）全类名-1,\接口实现类（或者使用了该注解的类）全类名-2,\...接口实现类（或者使用了该注解的类）全类名-n。spring.factories中最常用的注解是`org.springframework.boot.autoconfigure.EnableAutoConfiguration`，通过配置此注解对应的实现了，底层会由`AutoConfigurationImportSelector`对响应的目标类进行加载和自动注册。通过阅读Spring Boot 3.0 Migration Guide得知，spring.factories功能在Spring Boot 2.7已经废弃，并且会在Spring Boot 3.0移除。
<a name="GHEec"></a>
## spring.factories被移除后的替代方案
Spring Boot 2.x升级到Spring Boot 3.0其实是一个"破坏性"升级，目前来看相对较大的影响是：

- 必须使用JDK17
- Jakarta EE的引入，导致很多旧的类包名称改变
- 部分类被彻底移除
- spring-data模块的所有配置属性必须使用spring.data前缀，例如`spring.redis.host`必须更变为`spring.data.redis.host`
- spring.factories功能在Spring Boot 2.7已经废弃，在Spring Boot 3.0彻底移除（见下图）

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672639844923-2c08fc5c-3431-46a8-9e6f-b3b282ce9770.png#averageHue=%2329313a&clientId=udccb03c7-08d6-4&from=paste&id=ubb5346e0&originHeight=127&originWidth=912&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u631efdda-709a-4d7a-960f-9b663e7c536&title=)<br />替代方案比较简单，就是在类路径下创建`META-INF/spring/org.springframework.boot.autoconfigure.AutoConfiguration.imports`文件，文件的内容是：「每个实现类的全类名单独一行」。例如对于使用了（低版本还没适配Spring Boot 3.0）mybatis-plus、dynamic-datasource组件的场景，可以在项目某个模块的resources目录下建立`META-INF/spring/org.springframework.boot.autoconfigure.AutoConfiguration.imports`文件，输入以下内容：
```
com.baomidou.dynamic.datasource.spring.boot.autoconfigure.DynamicDataSourceAutoConfiguration
com.baomidou.mybatisplus.autoconfigure.MybatisPlusLanguageDriverAutoConfiguration
com.baomidou.mybatisplus.autoconfigure.MybatisPlusAutoConfiguration
```
对于某些社区热度比较高的组件近期可以密切关注其基于Spring Boot 3.0适配的版本发布，例如mybatis-spring、dubbo等：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672639844960-92b770ff-069a-49d2-858a-cef64c4568bf.png#averageHue=%23262d37&clientId=udccb03c7-08d6-4&from=paste&id=u3921dbfc&originHeight=801&originWidth=699&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u361cf720-21a0-4213-a472-3c5e35c2300&title=)<br />这里还没详细分析`META-INF/spring/org.springframework.boot.autoconfigure.AutoConfiguration.imports`的源码实现，从描述和文件名来看，大致看出它在使用上跟原来的spring.factories文件中编写`org.springframework.boot.autoconfigure.EnableAutoConfiguration`是相同的
<a name="snCHb"></a>
## 小结
Spring Boot 3.0的升级门槛比较高。「目前来看spring.factories功能的移除是本次版本升级的最大影响因素，有可能导致大部分第三方编写过自动注册板块的组件全部失效」。当然，JDK17也是一个比较高的门槛，对于大部分有历史包袱的项目如果决定升级需要极大的容器。建议先观望和关注团队用到的技术栈或者框架都适配Spring Boot 3.0后再进行版本升级。
