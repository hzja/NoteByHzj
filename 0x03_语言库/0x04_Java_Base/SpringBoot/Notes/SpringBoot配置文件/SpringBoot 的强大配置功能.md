- 前言
- 什么是`@ConfigurationProperties`和`@PropertySources`
- `@ConfigurationProperties`的使用
   - 步骤 1：创建 Java POJO 类
   - 步骤 2：配置类
   - 步骤 3：配置文件
   - 步骤 4：注入配置属性
- `@PropertySources`的使用
- `@PropertySources`与`@ConfigurationProperties`的区别
   - `@PropertySources` 注解的作用：
   - 与 `@ConfigurationProperties` 的关系：

---

<a name="D0cvc"></a>
## 1、前言
在当今的软件开发世界中，配置管理是至关重要的一部分。Spring框架提供了多种配置方式，其中`@ConfigurationProperties`和`@PropertySources`是强大的工具，可以轻松管理应用程序的配置信息。<br />本博客将深入探讨这两个关键注解，介绍它们的用途以及如何在Spring应用程序中使用它们。
<a name="uCE66"></a>
## 2、什么是`@ConfigurationProperties`和`@PropertySources`
`@ConfigurationProperties` 和 `@PropertySources` 都是与属性配置相关的注解，用于在 Spring 应用程序中管理和加载配置信息。
<a name="v8vBB"></a>
### `@ConfigurationProperties`

- `@ConfigurationProperties` 是 Spring Boot 中的一个注解，用于将配置文件中的属性值绑定到 Java Bean 上。通常，这个注解用于将外部属性文件中的属性值映射到应用程序的配置类中，以便在整个应用程序中方便地访问和使用这些属性。
- 这个注解通常与 `@Configuration` 或 `@Component` 一起使用，以将配置属性绑定到一个特定的 Java Bean。通过在类上使用 `@ConfigurationProperties`，可以将属性文件中的键与 Java Bean 的字段或属性进行映射。

示例：
```java
@Configuration
@ConfigurationProperties(prefix = "myapp")
public class MyAppProperties {
    private String name;
    private String version;
    // getters and setters
}
```
在上述示例中，`@ConfigurationProperties` 注解将以 myapp 前缀开头的属性映射到 MyAppProperties 类的字段中。
<a name="d9Jpr"></a>
### `@PropertySources`

- `@PropertySources` 是 Spring Framework 中的注解，用于指定多个属性源，以便在 Spring 应用程序中解析属性。每个属性源可以是一个属性文件，用于存储配置信息。与 `@PropertySource` 类似，`@PropertySources` 允许在多个地方定义属性源，以便将配置信息分散在不同的文件中。
- 这个注解通常与 `@Configuration` 一起使用，以将多个属性源引入 Spring 应用程序的环境中。

示例：
```java
@Configuration
@PropertySources({
    @PropertySource("classpath:application.properties"),
    @PropertySource("classpath:custom.properties")
})
public class AppConfig {
    // 这里可以注入和使用从属性文件加载的配置属性
}
```
在上述示例中，`@PropertySources` 注解指定了两个属性源，分别是 application.properties 和 custom.properties。
<a name="R4O8c"></a>
### 总结

- `@ConfigurationProperties` 主要用于将外部属性映射到 Java Bean，适用于 Spring Boot 应用程序。
- `@PropertySources` 主要用于指定多个属性源，以在 Spring 应用程序中加载配置属性，适用于标准的 Spring 应用程序。
<a name="RIKIf"></a>
## 3、`@ConfigurationProperties`的使用
在 Spring Boot 应用程序中，可以使用 `@ConfigurationProperties` 注解来绑定配置属性，并将其注入到 Spring Bean 中。以下是如何执行这些步骤的详细说明：
<a name="RAyjL"></a>
### 步骤 1：创建 Java POJO 类
首先，创建一个普通的 Java 类（POJO），该类的属性将映射到配置属性。这个类通常被用于存储和操作配置属性的值。确保为属性提供适当的 getter 和 setter 方法。
```java
@ConfigurationProperties(prefix = "myapp")
public class MyAppProperties {
    private String name;
    private String version;

    // 省略构造函数、getter 和 setter 方法
}
```
在上面的示例中，`@ConfigurationProperties` 注解指定了一个前缀 "myapp"，这意味着配置文件中的属性键应以 "myapp" 开头。例如，myapp.name 和 myapp.version。
<a name="QLWNC"></a>
### 步骤 2：配置类
接下来，创建一个配置类，用于加载配置属性并将它们绑定到 Java POJO 类。通常，这个配置类应该使用 `@Configuration` 注解进行标记。
```java
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.annotation.Configuration;

@Configuration
@EnableConfigurationProperties(MyAppProperties.class)
public class AppConfig {
}
```
`@EnableConfigurationProperties` 注解用于启用对 MyAppProperties 类的配置属性绑定。
<a name="LWSCj"></a>
### 步骤 3：配置文件
在 application.properties 或 application.yml 文件中，添加与配置属性相关的属性键和值。确保它们以与 `@ConfigurationProperties` 注解中指定的前缀匹配的方式进行命名。
```
myapp.name=My Spring Boot App
myapp.version=1.0
```
或者，使用 YAML 格式：
```yaml
myapp:
  name: My Spring Boot App
  version: 1.0
```
<a name="hxakA"></a>
### 步骤 4：注入配置属性
最后，在 Spring Bean 中注入 MyAppProperties 类，以访问配置属性的值。可以通过构造函数注入或使用 `@Autowired` 注解进行注入。
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class MyService {
    private final MyAppProperties myAppProperties;

    @Autowired
    public MyService(MyAppProperties myAppProperties) {
        this.myAppProperties = myAppProperties;
    }

    public void doSomething() {
        String appName = myAppProperties.getName();
        String appVersion = myAppProperties.getVersion();

        // 使用配置属性进行操作
        System.out.println("App Name: " + appName);
        System.out.println("App Version: " + appVersion);
    }
}
```
现在， MyService Bean 可以访问通过 `@ConfigurationProperties` 注解绑定的配置属性。<br />通过上述步骤，可以成功地在 Spring Boot 应用程序中使用 `@ConfigurationProperties` 注解将配置属性绑定到 Java POJO 类，并将其注入到 Spring Bean 中，以便在应用程序中使用这些属性的值。这种方式使配置属性的管理和使用变得非常方便。
<a name="hR3wl"></a>
## 4、`@PropertySources`的使用
在 Spring Boot 中，通常使用 `@PropertySource` 注解和 application.properties（或 application.yml）文件来管理应用程序的配置属性。虽然 `@PropertySources` 注解在标准 Spring 应用程序中更常见，但在 Spring Boot 中，通常使用 `@PropertySource` 和默认的 application.properties 或 application.yml 更为常见。<br />不过，可以提供一个示例，演示如何在 Spring Boot 中使用 `@PropertySources` 注解来指定多个属性源。<br />以下是一个示例：

1. 创建一个 Spring Boot 项目。
2. 创建一个配置类，例如 AppConfig.java，并使用 `@PropertySources` 注解来指定多个属性源。
```java
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;
import org.springframework.context.annotation.PropertySources;

@Configuration
@PropertySources({
    @PropertySource("classpath:default.properties"), // 默认属性源
    @PropertySource("classpath:custom.properties")  // 自定义属性源
})
public class AppConfig {
}
```
在上述示例中，在 AppConfig 类上使用 `@PropertySources` 注解，并指定了两个属性源，分别是 default.properties 和 custom.properties。这两个属性源位于类路径下，可以包含不同的配置属性。

3. 创建属性文件 default.properties 和 custom.properties 并放置在 src/main/resources 目录下。示例 default.properties 文件内容如下：
```
app.name=My Spring Boot App (Default)
app.version=1.0 (Default)
```
示例 custom.properties 文件内容如下：
```
app.name=My Custom Spring Boot App
```

4. 创建一个服务类，例如 MyService.java，并使用 `@Value` 注解来注入属性。
```java
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

@Service
public class MyService {

    @Value("${app.name}")
    private String appName;

    @Value("${app.version}")
    private String appVersion;

    public String getAppInfo() {
        return "App Name: " + appName + ", App Version: " + appVersion;
    }
}
```
在上述示例中，使用 `@Value` 注解来注入属性 app.name 和 app.version，这些属性的值将从属性源中获取。

5. 创建一个控制器类，例如 MyController.java，并在其中使用 MyService 来获取属性值并返回给客户端。
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class MyController {

    private final MyService myService;

    @Autowired
    public MyController(MyService myService) {
        this.myService = myService;
    }

    @GetMapping("/app-info")
    public String getAppInfo() {
        return myService.getAppInfo();
    }
}
```

6. 启动应用程序，并访问 /app-info 路径，将看到根据不同属性源获取的应用程序信息。

在这个示例中，使用了 `@PropertySources` 注解来指定多个属性源，并在服务类中使用 `@Value` 注解来注入属性。这使得应用程序能够根据属性源的不同而获取不同的属性值。<br />不过需要注意，在 Spring Boot 中，通常更常见的做法是使用默认的 application.properties（或 application.yml）文件以及 `@ConfigurationProperties` 注解来管理配置属性。
<a name="GUPZp"></a>
## 5、`@PropertySources`与@ConfigurationProperties的区别
`@PropertySources` 注解是 Spring 框架中用于指定多个属性源的注解。它的主要作用是允许在 Spring 应用程序中定义多个属性源，以便在不同位置或不同配置文件中存储和管理配置属性。这有助于将配置信息分散在多个文件中，使应用程序更加模块化和可维护。<br />下面详细介绍 `@PropertySources` 注解的作用以及与 `@ConfigurationProperties` 的关系：
<a name="xSz10"></a>
### `@PropertySources` 注解的作用：
**指定多个属性源：** `@PropertySources` 允许在一个配置类中指定多个属性源，每个属性源对应一个属性文件。这些属性源可以包含应用程序的配置信息，如数据库连接参数、日志级别、服务端口等。<br />**模块化配置：** 通过将配置信息分散在多个属性文件中，可以将应用程序的配置模块化，使每个模块负责自己的配置。这使得代码更易于维护和理解，特别是在大型应用程序中。<br />**环境适应性：** 可以为不同的环境（如开发、测试、生产）或不同的配置需求定义不同的属性源。这使得应用程序能够在不同环境中使用不同的配置，而不必修改代码。<br />**属性源优先级：** 如果存在多个属性源，Spring 将按照它们在 `@PropertySources` 注解中的顺序来解析属性。这意味着后面的属性源可以覆盖前面的属性源中的属性值。
<a name="mIH9x"></a>
### 与 `@ConfigurationProperties` 的关系：

- `@PropertySources` 和 `@ConfigurationProperties` 都用于处理配置属性，但它们的关注点和使用方式不同。
- `@PropertySources` 主要用于定义属性源，将配置信息存储在不同的属性文件中，并在不同环境或模块之间共享配置。它不直接与 Java Bean 绑定。
- `@ConfigurationProperties` 主要用于将配置属性绑定到 Java Bean 上。它通常与 `@Configuration` 或 `@Component` 注解一起使用，以将外部属性文件中的属性值映射到一个特定的 Java Bean。
- 通常情况下，会使用 `@PropertySources` 来定义属性源，然后使用 `@ConfigurationProperties` 来将属性源中的属性值绑定到 Java Bean，以便在整个应用程序中使用这些属性。这两个注解可以协同工作，但它们的功能不同，各自有其用途。

综上所述，`@PropertySources` 注解用于管理多个属性源，而 `@ConfigurationProperties` 注解用于将配置属性绑定到 Java Bean。它们可以一起使用，以实现更灵活和模块化的配置管理。
