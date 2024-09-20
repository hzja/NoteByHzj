JavaSpringBoot
<a name="zawd1"></a>
## 1、`@SpringBootApplication`
`@SpringBootApplication`是Spring Boot框架的核心注解之一，它用于标识一个主配置类，通常是项目的入口类。该注解可以将Spring Boot应用程序自动配置和启动的相关注解整合在一起，简化了Spring Boot应用的开发。<br />`@SpringBootApplication`解包含了以下三个注解的功能：

- `**@Configuration**`**：** 标注该类是一个配置类，可以使用`@Bean`注解来定义Bean。
- `**@EnableAutoConfiguration**`**：** 启用Spring Boot的自动配置机制，让Spring Boot自动根据项目的依赖和配置来决定应用程序的配置。
- `**@ComponentScan**`**：** 启用组件扫描，自动扫描并加载符合条件的组件，包括`@Controller`、`@Service`、`@Repository`等。

使用`@SpringBootApplication`注解，可以简化Spring Boot应用的配置，避免了繁琐的XML配置文件。通常情况下，只需要在项目的入类上添加`@SpringBootApplication`注解即可。<br />示例代码如下所示：
```java
@SpringBootApplication
public class Application {
    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
上述代码中，Application类使用`@SpringBootApplication`注解标注，并通过`SpringApplication.run`方法启动Spring Boot应用程序。<br />总结一下`@SpringBootApplication`的用法和作用：

- 用于标识Spring Boot应用程序的入口类。
- 整合了`@Configuration`、`@EnableAutoConfiguration`和`@ComponentScan`等注解的功能。
- 简化了Spring Boot应用的配置和启动过程。
- 可以通过`SpringApplication.run`方法启Spring Boot应用程序。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697636823320-c5ea1e31-d13a-4683-a78e-f4a2dac67f1a.jpeg)
<a name="Muycm"></a>
## 2、`@SpringBootConfiguration`
`SpringBootConfiguration`是Spring Boot框架中的一个注解，它用于标识一个类是Spring Boot的配置类。<br />在Spring Boot中，使用`@Configuration`注解来标识一个类是配置类，用于定义和配置Bean以及其他组件。而`SpringBootConfiguration`是`@Configuration`注解的派生注解，它在`@Configuration`注解的基础上添加了一些特定的功能。<br />`SpringBootConfiguration`的主要作用有以下几个方面：

- **标识配置类：** 使用`@SpringBootConfiguration`注解标识的类会被Spring Boot识别为配置类，Spring Boot会根据配置类的内容来进行相应的自动配置。
- **综合性注解：** `SpringBootConfiguration`是一个综合性注解，它包含了`@Configuration`、`@ComponentScan`和`@EnableAutoConfiguration`等注解的功能。因此，使用`@SpringBootConfiguration`注解可以简化配置类的编写，减少重复的注解。
- **自动配置：** 在Spring Boot中，使用`@SpringBootConfiguration`注解的类通常会与`@EnableAutoConfiguration`注解一起使用，实现自动配置的功能。`@EnableAutoConfiguration`注解用于启用Spring Boot的自动配置机制，根据项目的依赖和配置自动进行相关的配置。

使用`@SpringBootConfiguration`注解的示例代码如下：
```java
@SpringBootConfiguration
public class AppConfig {
    // 配置Bean和其他组件
}
```
上述代码中，`AppConfig`类使用`@SpringBootConfiguration`注解标识为配置类，通过在该类中定义Bean和其他组件的方式来进行配置。<br />总结一下`@SpringBootConfiguration`的用法和作用：

- 用于标识Spring Boot的配置类。
- 继承了`@Configuration`注解的功能，可以定义Bean和其他组件。
- 可以与`@EnableAutoConfiguration`注解一起使用，实现自动配置的功能。
- 简化了配置类的编写，减少了重复的注解。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697637367336-c277a23e-bdb3-4bd1-b5e1-ab12ca81bf0f.jpeg)
<a name="IIS8x"></a>
## 3、`@EnableAutoConfiguration`
`@EnableAutoConfiguration`是Spring Boot框架中的一个重要注解，它用于启用Spring Boot的自动配置机制。当使用`@EnableAutoConfiguration`注解时，Spring Boot会根据项目的依赖和配置来自动配置和加载相应的Bean。<br />`@EnableAutoConfiguration`注解的主要作用如下：

- **自动配置：** 通过`@EnableAutoConfiguration`注解，Spring Boot会根据classpath中的依赖和项目中的配置来自动完成一系列的自动配置工作。它会根据约定和条件，自动加载和配置Spring Boot所需的各种Bean。
- **简化配置：** 使用`@EnableAutoConfiguration`注解可以大大简化Spring Boot应用的配置工作。Spring Boot会根据项目的依赖自动完成大部分配置，省去了繁琐的XML配置和手动配置的过程。
- **条件装配：**`@EnableAutoConfiguration`注解会使用Spring Boot的条件注解机制来进行条件装配。它会根据项目的依赖和配置，判断是否满足条件来决定是否自动配置相关的Bean。

`@EnableAutoConfiguration`注解通常与`@SpringBootApplication`注解一起使用，`@SpringBootApplication`注解内部已经包含了`@EnableAutoConfiguration`注解。示例代码如下所示：
```java
@SpringBootApplication
public class Application {
    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
上述代码中，`@SpringBootApplication`注解标识了项目的入口类，内部已经包含了`@EnableAutoConfiguration`注解。<br />总结一下`@EnableAutoConfiguration`的用法和作用：

- 启用Spring Boot的自动配置机制。
- 自动根据项目的依赖和配置加载相应的Bean。
- 简化配置，省去大部分的XML配置和手动配置过程。
- 使用条件注解机制进行条件装配。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697638292408-22092499-26d4-4835-94a6-bb01cf6741b5.jpeg)
<a name="EF7nd"></a>
## 4、`@ComponentScan`
`@ComponentScan`是SpringBoot框架中的一个注解，用于启用组件扫描功能。它会自动扫描并加载符合条件的组件，包括`@Controller`、`@Service`、`@Repository`等。`@ComponentScan`注解的主要作用有以下几个方面：

1. **组件扫描：** `@ComponentScan`注解会扫描指定的包及其子包下的所有类，寻找带有`@Component`及其派生注解的类，并将其注册为Spring的Bean。这样，在其他地方就可以通过自动注入的方式将这些组件使用起来。
2. **简化配置：** 使用`@ComponentScan`注解可以大大简化SpringBoot应用的配置工作。不再需要手动配置每个组件的Bean定义，而是可以通过自动扫描的方式自动加载所需的组件。
3. **指定扫描路径：** `@ComponentScan`注解可以通过`basePackages`或value属性指定要扫描的包路径。如果不指定，默认会扫描启动类所在的包及其子包下的所有类。
4. **排除特定组件：** `@ComponentScan`注解还可以通过`excludeFilters`属性排除指定的组件，不进行扫描和加载。可以使用`FilterType`枚举类型来指定排除的条件，如按照类、注解等进行排除。

示例代码如下所示：
```java
@SpringBootApplication
@ComponentScan(basePackages = "com.example")
public class Application {
    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
上述代码中，`@ComponentScan`注解标识了项目的入口类，并通过`basePackages`属性指定要扫描的包路径为"com.example"。这样，SpringBoot会扫描该包及其子包下的所有类，并注册为Spring的Bean。<br />总结一下`@ComponentScan`的用法和作用：

1. 启用组件扫描功能。
2. 自动扫描并加载符合条件的组件，包括`@Controller`、`@Service`、`@Repository`等。
3. 通过`basePackages`属性指定要扫描的包路径。
4. 可以通过`excludeFilters`属性排除指定的组件。
