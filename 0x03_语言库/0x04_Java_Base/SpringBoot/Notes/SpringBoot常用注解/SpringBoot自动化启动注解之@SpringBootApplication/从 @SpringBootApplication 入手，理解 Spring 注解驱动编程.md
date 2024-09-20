JavaSpringBoot
<a name="Yy6F5"></a>
## 前言
几乎所有的 Spring Boot 应用都会在启动类上添加 `@SpringBootApplication`，可以说它是 Spring Boot 应用最核心的注解了。Spring Boot 基于 Spring Framework，`@SpringBootApplication` 也不例外，在理解 `@SpringBootApplication` 的基础上将Spring注解的内容进行串联，以便达到融会贯通的效果。
<a name="IONuq"></a>
## 理解 `@SpringBootApplication` 语义
即便初学者不理解 `@SpringBootApplication`，通常来说也会把这个注解添加到启动类，然后将启动类放在根包下面。
```java
@SpringBootApplication
public class SpringBootDemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(SpringBootDemoApplication.class, args);
    }

}
```
可以说上面的代码，就是 Spring Boot 应用的一个模板，所有的 Spring Boot 应用都有类似的代码。<br />关于 `@SpringBootApplication` 的注解语义，Spring 官网在 “3.6 Using the @SpringBootApplication Annotation” 描述如下：<br />许多 Spring Boot 开发者喜欢在他们的应用中使用自动装配、组件扫描以及在配置类上定义额外的配置。单个 `@SpringBootApplication` 注解已经用来启动这三个特性，即：

- `**@EnableAutoConfiguration**`**：启用 Spring Boot 自动装配机制。**
- `**@ComponentScan**`**：启用应用中包下面的 **`**@Component**`** 扫描功能。**
- `**@Configuration**`**：允许在上下文中注册 bean 或导入额外的配置类。**

也就是说 `@SpringBootApplication` 相当于三个注解的组合，来看下这个注解的定义。
```java
/**
 * @since 1.4.0
 */
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@SpringBootConfiguration
@EnableAutoConfiguration
@ComponentScan(excludeFilters = { @Filter(type = FilterType.CUSTOM, classes = TypeExcludeFilter.class),
  @Filter(type = FilterType.CUSTOM, classes = AutoConfigurationExcludeFilter.class) })
public @interface SpringBootApplication {
    ...省略属性
}
```
可以看到 `@SpringBootApplication` 被 `@EnableAutoConfiguration`、`@ComponentScan` 及 `@SpringBootConfiguration` 同时标注。<br />那么官网描述的 `@Configuration` 在哪呢？根据直觉看下 `@SpringBootConfiguration`。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Configuration
public @interface SpringBootConfiguration {
    ... 省略属性
}
```
可以看到 `@SpringBootConfiguration` 被 `@Configuration` 标注，因此 Spring 官网提到 `@Configuration` 注解。<br />不过 `@SpringBootApplication` 中的 `@ComponentScan` 注解并没有使用默认的属性配置，而是指定了 `excludeFilters` 属性值用来过滤扫描的组件，`TypeExcludeFilter` 根据用户自定义的 `TypeExcludeFilter` bean 过滤组件，`AutoConfigurationExcludeFilter` 用于跳过自动装配的配置类。<br />事实上，`@SpringBootApplication` 这个注解也不是在最初的版本就有的，而是在 Spring Boot 1.4 版本添加到 Spring Boot 框架中的，在此之前只能使用组合 `@SpringBootApplication` 注解的三个注解，它们的语义是保持一致的，如下所示。
```java
@Configuration
@ComponentScan
@EnableAutoConfiguration
//@SpringBootApplication
public class SpringBootDemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(SpringBootDemoApplication.class, args);
    }

}
```
由于 `@Configuration`、`@ComponentScan`、`@EnableAutoConfiguration` 这三个注解使用较为频繁 Spring 官方才将其组合到一个注解中。
<a name="rkCoK"></a>
## Spring 注解编程模型
注解作为元数据，只有被读取到才能发挥其作用，那么 Spring Boot 如何读取 `@SpringBootApplication` 注解的呢？<br />将 `@SpringBootApplication` 注解标注的 class 传入 `SpringApplication.run` 方法的参数后，Spring Boot 便会将这个 class 注册为 bean，之后 Spring 便会对这个 bean 进一步的处理，例如根据 `@ComponentScan` 进行组件扫描，根据 `@EnableAutoConfiguration` 进行自动装配。<br />这也意味着不必一定要把 `@SpringBootApplication` 添加到启动类，假如想把 `@SpringBootApplication` 添加到其他配置类，可以如下更改。
```java
@SpringBootApplication
public class Config {
}

public class SpringBootDemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(Config.class, args);
    }

}
```
事实上，`@SpringBootApplication` 只是将多个注解标注到自身，Spring Boot 也没有对这个注解做额外的处理，甚至可以自定义一个注解替代 `@SpringBootApplication`，示例如下。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@Configuration
@EnableAutoConfiguration
@ComponentScan
public @interface CustomSpringBootApplication {
}

@CustomSpringBootApplication
public class SpringBootDemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(SpringBootDemoApplication.class, args);
    }

}
```
上述自定义的 `@CustomSpringBootApplication` 注解可以达到类似 `@SpringBootApplication` 注解的效果，这得益于 Spring 的注解编程模型，其中一些概念如下。
<a name="q8RHa"></a>
### 1、注解与元注解
标注在注解上的注解称为元注解，例如 `@ComponentScan` 注解标注在了 `@SpringBootApplication` 注解上，`@ComponentScan` 是 `@SpringBootApplication` 注解的元注解，同时称 `@SpringBootApplication` 被 `@ComponentScan` 注解元标注。
<a name="m7dYX"></a>
### 2、组合注解
被一个或多个注解标注的注解被称为组合注解，如 `@SpringBootApplication` 注解，组合注解的目的在于组合多个注解具备的功能。<br />例如 `@SpringBootApplication` 自动具备组件扫描、配置类、启动自动装配的功能。Spring 中的注解允许多层次 “继承”，例如 `@Configuration` 标注了 `@SpringBootConfiguration`，`@SpringBootConfiguration` 又标注了 `@SpringBootApplication`，因此 `@SpringBootApplication` 标注的类成为一个配置类。
<a name="ZLHl2"></a>
### 3、属性别名与属性重写
属性别名用于将一个属性指定为另一个属性的别名。使用 `@AliasFor` 注解进行标注。例如 Spring MVC 中的 `@RequestMapping` 注解。
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Mapping
public @interface RequestMapping {

    @AliasFor("path")
    String[] value() default {};

    @AliasFor("value")
    String[] path() default {};
}
```
path 和 value 属性上都标注了 `@AliasFor` 注解，它们互为对方的别名，使用时指定其中一个属性值即可，如果使用指定多个产生歧义则会报错。<br />由于注解不支持继承，用户无法直接指定元注解中的属性值，Spring 注解编程模型允许注解中的属性重写元注解中的属性。注解中的属性名称与元注解中的属性名称一致即可重写元注解属性，如果不一致还可以使用 `@AliasFor` 注解标注在注解属性上用来指定重写哪个元注解的哪个属性。例如对于 `@SpringBootApplication` 注解。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@SpringBootConfiguration
@EnableAutoConfiguration
@ComponentScan(excludeFilters = { @Filter(type = FilterType.CUSTOM, classes = TypeExcludeFilter.class),
                                 @Filter(type = FilterType.CUSTOM, classes = AutoConfigurationExcludeFilter.class) })
public @interface SpringBootApplication {

    @AliasFor(annotation = ComponentScan.class, attribute = "basePackages")
    String[] scanBasePackages() default {};

}
```
`@SpringBootApplication` 注解的 `scanBasePackages` 属性就重写了元注解 `@ComponentScan` 的 basePackages 属性。
<a name="q4Cbc"></a>
## Spring 组件扫描
`@SpringBootApplication` 注解上标注了 `@ComponentScan` 注解，因此 `@SpringBootApplication` 可用于扫描组件，那么扫描组件的动作发生在什么时候呢？Spring 又是如何进行组件扫描的？<br />要理解 Spring 的组件扫描，还得看 Spring 的本质与历史。Spring 本质上只是一个管理 bean 的容器，并提供了依赖注入与依赖获取的能力，因此需要定义哪些 bean 交给 Spring 容器进行管理。<br />Spring 诞生初期，Java 注解还未诞生，Spring 采用业界广泛使用的 xml 作为配置文件。Spring 1.2 版本，第一个注解 `@Transactional` 诞生。到了 2.5 版本，Spring 才开始支持在 xml 配置 `<context:component-scan>` 对类路径下进行扫描，并将标注了 `@Component` 的类注册为 bean。到了 Spring 3.0 版本， `@Configuration` 注解诞生，此时只能配合 `@Import`、`@ImporResource` 或者`@Bean` 注解有限的配置一些 bean。直到 Spring 3.1 版本，`@ComponentScan` 注解才诞生，并提供了基于注解的 `ApplicationContext` 实现 `AnnotationConfigApplicationContext`。<br />回到 Spring 对类路径下的组件扫描，流程大概如下，当使用 `<context:component-scan>` 或直接使用基于注解的 `ApplicationContext` 对类路径扫描后，Spring 会查找符合条件的组件类，并将满足条件的这些类注册为 Spring bean，包括满足 `@Conditional` 及 `@ComponentScan#excludeFilter` 条件。然后使用 `ConfigurationClassPostProcessor` 在应用上下文的生命周期 bean 实例化前的阶段对配置类进一步的处理，配置类包括标注或元标注了 `@Component`、`@ComponentScan`、`@Import` 或 `@ImportResource` 注解的组件类，或存在 `@Bean` 注解的方法的组件类。<br />Spring Boot 将 `@SpringBootApplication` 标注的类注册为 bean 之后，`ConfigurationClassPostProcessor` 就会对这个类进一步处理了，当发现存在注解或元注解 `@ComponentScan` 就会进一步处理，扫描并注册 bean。
<a name="PDVxO"></a>
## Spring Boot 自动装配
前文提到使用 `@SpringBootApplication` 注解后会自动开启自动装配功能，正是因为 `@SpringBootApplication` 被 `@EnableAutoConfiguration` 标注。<br />`@EnableAutoConfiguration` 使用了 `Enable*` 编程模型，主要利用 `@Import` 及 `@Conditional` 根据某些条件自动化装配一些 bean。<br />自动装配可以理解为当某些条件满足后 Spring 会自动注册一些 bean，而不需要手动进行配置。例如，当添加 spring-boot-starter-web 依赖后，Spring Boot 发现类路径下存在 Tomcat，就会自动注入一些 Web 相关的 bean，从而简化用户的配置。<br />如果 Spring Boot 未提供自动装配功能，则只能通过 `@Enable*` 编程模型手动的将某些功能模块的 bean 进行注册，Spring Boot 将一些与业务无关的 bean 的装配工作简化到 `@EnableAutoConfiguration` 注解后可谓是大大降低了 Spring Boot 的上手门槛。<br />作为 Spring Boot 核心特性的自动装配，这里先简单理解。
<a name="HZYyG"></a>
## 总结
在介绍 `@SpringBootApplication` 的基础上，介绍了有关这个注解更底层的一些内容，包括注解编程模型、组件扫描、自动装配。
