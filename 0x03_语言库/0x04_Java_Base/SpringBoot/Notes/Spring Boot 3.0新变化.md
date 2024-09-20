JavaSpringBoot
<a name="VjktW"></a>
## Java 17
Java 17将是继Java 8以来最重要的LTS版本，是Java社区八年努力的成果。包含了很多重要的改进，Java 17也是目前性能最强的LTS版本。目前所有的Spring Boot 2.x 版本都能很好地适配Java 17，可以无需等待，先着手JDK的升级调试，并尝试一些新的特性和API。
<a name="FbuuT"></a>
## 尽快升级到Spring Boot 2.7
Spring Boot 2.7基本已经是Spring Boot 2.x最后一个大版本了，Spring Boot 2.5已经停止OSS支持，不再进行维护，Spring Boot 2.6也将在Spring Boot 3.0发布后停止维护，迭代的速度越来越快了。尽早升级到2.7才能更好迁移到3.0，这里建议不要跳版本升级，比如不要直接从2.4跳到2.7，尽量按照2.4、2.5、2.6、2.7这样的步骤升级，跨度太大反而不利于平滑升级。
<a name="kUams"></a>
## 移除过时代码
每个Spring Boot版本都会或多或少有一些被标记为`@Deprecated`的代码，Spring Boot 3.0 将完全移除在 2.x 过时的代码，当然2.x早期的过时代码也可能在最新的2.x被移除。尽量不要使用过时代码，一般过时代码上都注释有过时的原因或者替代的API。
<a name="fpQgC"></a>
## 配置文件机制的更改
在Spring Boot 2.4中，配置文件 application.properties 和 application.yaml 的加载机制被改变，目的是为了简化外部配置的加载方式，使之更加合理，带来的是不向下兼容。为了平滑升级，Spring 提供了一个配置项来兼容旧机制：
```yaml
spring:
  config:
    use-legacy-processing: true
```
而这个机制将在3.0时移除，必须使用符合新机制的配置方式，如果存在这几种方式就需要注意了。
<a name="XJXFI"></a>
### 多文档Yaml
如果在yaml配置文件中使用了间隔符`---`来声明多文档的话，得知道现在按文档声明的顺序来注册声明的配置属性；而在 Spring Boot 2.3 及更早版本中，基于配置文件激活顺序。举个例子：
```yaml
---
spring:
  profiles: 
    active: dev
  application:
    name: dev-app
server:
  port: 8081      
---
spring:
  profiles:
    active: prod
  application:
    name: prod-app
server:
  port: 8080     
```
这个配置文件在Spring Boot 2.3以及更早的版本中会根据`spring.profiles.active`来决定加载的环境。但是从2.4开始后面的属性会覆盖前面的属性。
<a name="NffhL"></a>
### 外部配置总是覆盖jar内的配置
如果配置文件在jar之外，并且该配置文件适用于特定的环境，例如application-dev.yaml。2.4以下的版本中，在jar外面的application.yaml不会覆盖jar中的application-<profile名称>.yaml文件，从2.4开始外部文件将总是覆盖jar内的配置文件。需要检查是否存在这种情况。
<a name="NjVnC"></a>
### 激活配置文件
如果使用`spring.profiles`属性来激活环境配置，那么现在就应该迁移到`spring.config.activate.on-profile`。<br />旧玩法：
```yaml
spring:
  profiles: "prod"
secret: "production-password"
```
新玩法：
```yaml
spring:
  config:
    activate:
      on-profile: "prod"
secret: "production-password"
```
这真是折腾人。`spring.profiles.active`仍然可以用来激活特定的环境，例如命令行：
```bash
$ java -jar myapp.jar --spring.profiles.active=prod
```
也可以在 application.properties 或 application.yaml 中使用`spring.profiles.active`，从2.4开始`spring.profiles.active`不能在特定环境中使用，也就是说`application-<profile>.yaml`中不能使用，`---`间隔的多文档中也不能使用它。一句话，不能再通过`spring.profiles.active`来合并一个包含了spring.config.activate.on-profile属性的配置文件。<br />同样的机制， `spring.profiles.include`属性只能在非特定配置文件中使用，下面的配置是无效的：
```yaml
# 无效配置
spring:
  config:
    activate:
      on-profile: "prod"
  profiles:
    include: "metrics"
```
更多的要点请参考官方配置文件[**Spring boot配置迁移指南**](https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-Config-Data-Migration-Guide)。
<a name="VQHEy"></a>
## 性能更高的路径解析方式
从Spring Boot 2.6 开始，路径解析默认使用`PathPatternParser`，取代了之前的Ant风格匹配`AntPathMatcher`，升级时很多人的**Swagger**出问题了，通过`spring.mvc.pathmatch.matching-strategy`解决了这个问题。虽然在Spring Boot 3.0中`AntPathMatcher`会继续生效，但是`PathPatternParser`成为官方推荐，因为它的性能更高。
<a name="ufOzf"></a>
## 兼容问题
首先是Jakarta EE 9的兼容问题，确保第三方依赖库和代码都兼容Jakarta EE 9。另外也要检查Spring框架正在使用的第三方依赖jar是否有计划兼容Spring 6。
<a name="EtMDd"></a>
## 参考资料
Spring boot配置迁移指南: [https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-Config-Data-Migration-Guide](https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-Config-Data-Migration-Guide)
