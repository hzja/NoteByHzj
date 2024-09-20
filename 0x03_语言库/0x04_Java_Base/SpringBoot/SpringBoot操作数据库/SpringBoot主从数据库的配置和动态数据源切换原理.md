Java 主从数据库<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634906162871-3c680e70-d44f-486c-9056-da3b00e376dc.webp#clientId=u7c8cac53-f126-4&from=paste&id=ua3560894&originHeight=1250&originWidth=568&originalType=url&ratio=1&status=done&style=shadow&taskId=u2bc53e6e-930b-41ba-8e96-a5e9c61f228)
<a name="hBaEV"></a>
### 第一步：配置多数据源
首先，在 SpringBoot 中配置两个数据源，其中第二个数据源是`ro-datasource`：
```yaml
spring:
  datasource:
    jdbc-url: jdbc:mysql://localhost/test
    username: rw
    password: rw_password
    driver-class-name: com.mysql.jdbc.Driver
    hikari:
      pool-name: HikariCP
      auto-commit: false
      ...
  ro-datasource:
    jdbc-url: jdbc:mysql://localhost/test
    username: ro
    password: ro_password
    driver-class-name: com.mysql.jdbc.Driver
    hikari:
      pool-name: HikariCP
      auto-commit: false
      ...
```
在开发环境下，没有必要配置主从数据库。只需要给数据库设置两个用户，一个`rw`具有读写权限，一个`ro`只有 `SELECT` 权限，这样就模拟了生产环境下对主从数据库的读写分离。<br />在 SpringBoot 的配置代码中，初始化两个数据源：
```java
@SpringBootApplication
public class MySpringBootApplication {
    /**
     * Master data source.
     */
    @Bean("masterDataSource")
    @ConfigurationProperties(prefix = "spring.datasource")
    DataSource masterDataSource() {
       logger.info("create master datasource...");
        return DataSourceBuilder.create().build();
    }

    /**
     * Slave (read only) data source.
     */
    @Bean("slaveDataSource")
    @ConfigurationProperties(prefix = "spring.ro-datasource")
    DataSource slaveDataSource() {
        logger.info("create slave datasource...");
        return DataSourceBuilder.create().build();
    }

    ...
}
```
<a name="Nw6uD"></a>
### 第二步：编写 `RoutingDataSource`
然后，用 Spring 内置的 `RoutingDataSource`，把两个真实的数据源代理为一个动态数据源：
```java
public class RoutingDataSource extends AbstractRoutingDataSource {

    @Override
    protected Object determineCurrentLookupKey() {
        return "masterDataSource";
    }
}


对这个RoutingDataSource，需要在 SpringBoot 中配置好并设置为主数据源：
@SpringBootApplication
public class MySpringBootApplication {
    @Bean
    @Primary
    DataSource primaryDataSource(
            @Autowired @Qualifier("masterDataSource") DataSource masterDataSource,
            @Autowired @Qualifier("slaveDataSource") DataSource slaveDataSource
    ) {
        logger.info("create routing datasource...");
        Map<Object, Object> map = new HashMap<>();
        map.put("masterDataSource", masterDataSource);
        map.put("slaveDataSource", slaveDataSource);
        RoutingDataSource routing = new RoutingDataSource();
        routing.setTargetDataSources(map);
        routing.setDefaultTargetDataSource(masterDataSource);
        return routing;
    }
    ...
}
```
现在，`RoutingDataSource` 配置好了，但是，路由的选择是写死的，即永远返回`"masterDataSource"`，<br />现在问题来了：如何存储动态选择的 key 以及在哪设置 key？<br />在 Servlet 的线程模型中，使用 ThreadLocal 存储 key 最合适，因此，编写一个 `RoutingDataSourceContext`，来设置并动态存储 key：
```java
public class RoutingDataSourceContext implements AutoCloseable {

    // holds data source key in thread local:
    static final ThreadLocal<String> threadLocalDataSourceKey = new ThreadLocal<>();

    public static String getDataSourceRoutingKey() {
        String key = threadLocalDataSourceKey.get();
        return key == null ? "masterDataSource" : key;
    }

    public RoutingDataSourceContext(String key) {
        threadLocalDataSourceKey.set(key);
    }

    public void close() {
        threadLocalDataSourceKey.remove();
    }
}
```
然后，修改 `RoutingDataSource`，获取 key 的代码如下：
```java
public class RoutingDataSource extends AbstractRoutingDataSource {
    protected Object determineCurrentLookupKey() {
        return RoutingDataSourceContext.getDataSourceRoutingKey();
    }
}
```
这样，在某个地方，例如一个 Controller 的方法内部，就可以动态设置 DataSource 的 Key：
```java
@Controller
public class MyController {
    @Get("/")
    public String index() {
        String key = "slaveDataSource";
        try (RoutingDataSourceContext ctx = new RoutingDataSourceContext(key)) {
            // TODO:
            return "html... www.liaoxuefeng.com";
        }
    }
}
```
到此为止，已经成功实现了数据库的动态路由访问。<br />这个方法是可行的，但是，需要读从数据库的地方，就需要加上一大段`try (RoutingDataSourceContext ctx = ...) {}`代码，使用起来十分不便。有没有方法可以简化呢？<br />有！<br />仔细想想，Spring 提供的声明式事务管理，就只需要一个`@Transactional()`注解，放在某个 Java 方法上，这个方法就自动具有了事务。<br />也可以编写一个类似的`@RoutingWith("slaveDataSource")`注解，放到某个 Controller 的方法上，这个方法内部就自动选择了对应的数据源。代码看起来应该像这样：
```java
@Controller
public class MyController {
    @Get("/")
    @RoutingWith("slaveDataSource")
    public String index() {
        return "html... www.liaoxuefeng.com";
    }
}
```
这样，完全不修改应用程序的逻辑，只在必要的地方加上注解，自动实现动态数据源切换，这个方法是最简单的。<br />想要在应用程序中少写代码，就得多做一点底层工作：必须使用类似 Spring 实现声明式事务的机制，即用 AOP 实现动态数据源切换。<br />实现这个功能也非常简单，编写一个`RoutingAspect`，利用 AspectJ 实现一个`Around`拦截：
```java
@Aspect
@Component
public class RoutingAspect {
    @Around("@annotation(routingWith)")
    public Object routingWithDataSource(ProceedingJoinPoint joinPoint, RoutingWith routingWith) throws Throwable {
        String key = routingWith.value();
        try (RoutingDataSourceContext ctx = new RoutingDataSourceContext(key)) {
            return joinPoint.proceed();
        }
    }
}
```
注意方法的第二个参数`RoutingWith`是 Spring 传入的注解实例，根据注解的`value()`获取配置的 key。编译前需要添加一个 Maven 依赖：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-aop</artifactId>
</dependency>
```
到此为止，就实现了用注解动态选择数据源的功能。最后一步重构是用字符串常量替换散落在各处的`"masterDataSource"`和`"slaveDataSource"`。
<a name="NGACU"></a>
### 使用限制
受 Servlet 线程模型的局限，动态数据源不能在一个请求内设定后再修改，也就是`@RoutingWith`不能嵌套。此外，`@RoutingWith`和`@Transactional`混用时，要设定 AOP 的优先级。<br />本文代码需要 SpringBoot 支持，JDK 1.8 编译并打开`-parameters`编译参数。
