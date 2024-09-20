Java Spring SpringBoot
<a name="KmOu2"></a>
## 什么是Spring
作为Java开发人员，大家都Spring都不陌生，简而言之，Spring框架为开发Java应用程序提供了全面的基础架构支持。它包含一些很好的功能，如依赖注入和开箱即用的模块，如：Spring JDBC 、Spring MVC 、Spring Security、 Spring AOP 、Spring ORM 、Spring Test，这些模块缩短应用程序的开发时间，提高了应用开发的效率例如，在Java Web开发的早期阶段，需要编写大量的代码来将记录插入到数据库中。但是通过使用Spring JDBC模块的JDBCTemplate，可以将操作简化为几行代码。
<a name="NxO1v"></a>
## 什么是Spring Boot
Spring Boot基本上是Spring框架的扩展，它消除了设置Spring应用程序所需的XML配置，为更快，更高效的开发生态系统铺平了道路。<br />Spring Boot中的一些特征：<br />1、 创建独立的Spring应用。<br />2、 嵌入式Tomcat、Jetty、 Undertow容器（无需部署war文件）。<br />3、 提供的starters 简化构建配置<br />4、 尽可能自动配置spring应用。<br />5、 提供生产指标,例如指标、健壮检查和外部化配置<br />6、 完全没有代码生成和XML配置要求
<a name="EpsyM"></a>
## 从配置分析
<a name="C4ubL"></a>
### Maven依赖
首先看一下使用Spring创建Web应用程序所需的最小依赖项
```xml
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-web</artifactId>
    <version>5.1.0.RELEASE</version>
</dependency>
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-webmvc</artifactId>
    <version>5.1.0.RELEASE</version>
</dependency>
```
与Spring不同，Spring Boot只需要一个依赖项来启动和运行Web应用程序：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <version>2.0.6.RELEASE</version>
</dependency>
```
在进行构建期间，所有其他依赖项将自动添加到项目中。<br />另一个很好的例子就是测试库。通常使用Spring Test，JUnit，Hamcrest和Mockito库。在Spring项目中，应该将所有这些库添加为依赖项。但是在Spring Boot中，只需要添加spring-boot-starter-test依赖项来自动包含这些库。<br />Spring Boot为不同的Spring模块提供了许多依赖项。一些最常用的是：<br />spring-boot-starter-data-jpaspring-boot-starter-securityspring-boot-starter-testspring-boot-starter-webspring-boot-starter-thymeleaf<br />有关starter的完整列表，请查看Spring文档。
<a name="u97bA"></a>
### MVC配置
来看一下Spring和Spring Boot创建JSP Web应用程序所需的配置。<br />Spring需要定义调度程序servlet，映射和其他支持配置。可以使用 web.xml 文件或`Initializer`类来完成此操作：
```java
public class MyWebAppInitializer implements WebApplicationInitializer {

    @Override
    public void onStartup(ServletContext container) {
        AnnotationConfigWebApplicationContext context = new AnnotationConfigWebApplicationContext();
        context.setConfigLocation("com.pingfangushi");
          container.addListener(new ContextLoaderListener(context));
          ServletRegistration.Dynamic dispatcher = container
          .addServlet("dispatcher", new DispatcherServlet(context));
        dispatcher.setLoadOnStartup(1);
        dispatcher.addMapping("/");
    }
}
```
还需要将`@EnableWebMvc`注释添加到`@Configuration`类，并定义一个视图解析器来解析从控制器返回的视图：
```java
@EnableWebMvc
@Configuration
public class ClientWebConfig implements WebMvcConfigurer { 
   @Bean
   public ViewResolver viewResolver() {
      InternalResourceViewResolver bean
        = new InternalResourceViewResolver();
      bean.setViewClass(JstlView.class);
      bean.setPrefix("/WEB-INF/view/");
      bean.setSuffix(".jsp");
      return bean;
   }
}
```
再来看SpringBoot一旦添加了Web启动程序，Spring Boot只需要在application配置文件中配置几个属性来完成如上操作：
```
spring.mvc.view.prefix=/WEB-INF/jsp/
spring.mvc.view.suffix=.jsp
```
上面的所有Spring配置都是通过一个名为auto-configuration的过程添加Boot web starter来自动包含的。<br />这意味着Spring Boot将查看应用程序中存在的依赖项，属性和bean，并根据这些依赖项，对属性和bean进行配置。当然，如果想要添加自己的自定义配置，那么Spring Boot自动配置将会退回。
<a name="xbjUr"></a>
### 配置模板引擎
现在来看下如何在Spring和Spring Boot中配置Thymeleaf模板引擎。<br />在Spring中，需要为视图解析器添加thymeleaf-spring5依赖项和一些配置：
```java
@Configuration
@EnableWebMvc
public class MvcWebConfig implements WebMvcConfigurer {

    @Autowired
    private ApplicationContext applicationContext;

    @Bean
    public SpringResourceTemplateResolver templateResolver() {
        SpringResourceTemplateResolver templateResolver = new SpringResourceTemplateResolver();
        templateResolver.setApplicationContext(applicationContext);
        templateResolver.setPrefix("/WEB-INF/views/");
        templateResolver.setSuffix(".html");
        return templateResolver;
    }

    @Bean
    public SpringTemplateEngine templateEngine() {
        SpringTemplateEngine templateEngine = new SpringTemplateEngine();
        templateEngine.setTemplateResolver(templateResolver());
        templateEngine.setEnableSpringELCompiler(true);
        return templateEngine;
    }

    @Override
    public void configureViewResolvers(ViewResolverRegistry registry) {
        ThymeleafViewResolver resolver = new ThymeleafViewResolver();
        resolver.setTemplateEngine(templateEngine());
        registry.viewResolver(resolver);
    }
}
```
SpringBoot1X只需要spring-boot-starter-thymeleaf的依赖项来启用Web应用程序中的Thymeleaf支持。 但是由于Thymeleaf3.0中的新功能，必须将thymeleaf-layout-dialect 添加为SpringBoot2XWeb应用程序中的依赖项。配置好依赖，就可以将模板添加到src/main/resources/templates文件夹中，SpringBoot将自动显示它们。
<a name="NYH2S"></a>
### Spring Security 配置
为简单起见，使用框架默认的HTTP Basic身份验证。首先看一下使用Spring启用`Security`所需的依赖关系和配置。<br />Spring首先需要依赖 spring-security-web和spring-security-config 模块。接下来， 需要添加一个扩展`WebSecurityConfigurerAdapter`的类，并使用`@EnableWebSecurity`注解：
```java
@Configuration
@EnableWebSecurity
public class CustomWebSecurityConfigurerAdapter extends WebSecurityConfigurerAdapter {

    @Autowired
    public void configureGlobal(AuthenticationManagerBuilder auth) throws Exception {
        auth.inMemoryAuthentication()
          .withUser("admin")
            .password(passwordEncoder()
            .encode("password"))
          .authorities("ROLE_ADMIN");
    }

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http.authorizeRequests()
          .anyRequest().authenticated()
          .and()
          .httpBasic();
    }

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }
}
```
这里使用`inMemoryAuthentication`来设置身份验证。同样，Spring Boot也需要这些依赖项才能使其工作。但是只需要定义spring-boot-starter-security的依赖关系，因为这会自动将所有相关的依赖项添加到类路径中。<br />Spring Boot中的安全配置与上面的相同 。
<a name="ZC9zX"></a>
## 应用程序启动引导配置
Spring和Spring Boot中应用程序引导的基本区别在于servlet。Spring使用web.xml 或`SpringServletContainerInitializer`作为其引导入口点。Spring Boot仅使用Servlet 3功能来引导应用程序，下面详细来了解下
<a name="NlEF9"></a>
### Spring 引导配置
Spring支持传统的web.xml引导方式以及最新的Servlet 3+方法。<br />配置web.xml方法启动的步骤<br />Servlet容器（服务器）读取web.xml<br />web.xml中定义的`DispatcherServlet`由容器实例化<br />`DispatcherServlet`通过读取`WEB-INF / {servletName} -servlet.xml`来创建`WebApplicationContext`。最后，`DispatcherServlet`注册在应用程序上下文中定义的bean<br />使用Servlet 3+方法的Spring启动步骤<br />容器搜索实现`ServletContainerInitializer`的类并执行`SpringServletContainerInitializer`找到实现所有类`WebApplicationInitializer````WebApplicationInitializer`创建具有XML或上下文`@Configuration`类`WebApplicationInitializer`创建`DispatcherServlet`与先前创建的上下文。
<a name="bAdDw"></a>
### SpringBoot 引导配置
Spring Boot应用程序的入口点是使用`@SpringBootApplication`注释的类
```java
@SpringBootApplication
public class Application {
    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
默认情况下，Spring Boot使用嵌入式容器来运行应用程序。在这种情况下，Spring Boot使用`public static void main`入口点来启动嵌入式Web服务器。此外，它还负责将`Servlet`，`Filter`和`ServletContextInitializer` bean从应用程序上下文绑定到嵌入式servlet容器。Spring Boot的另一个特性是它会自动扫描同一个包中的所有类或Main类的子包中的组件。<br />Spring Boot提供了将其部署到外部容器的方式。只需要扩展`SpringBootServletInitializer`即可：
```java
/**
 * War部署
 */
public class ServletInitializer extends SpringBootServletInitializer {

    @Override
    protected SpringApplicationBuilder configure(SpringApplicationBuilder application) {
        return application.sources(Application.class);
    }

    @Override
    public void onStartup(ServletContext servletContext) throws ServletException {
        super.onStartup(servletContext);
        servletContext.addListener(new HttpSessionEventPublisher());
    }
}
```
这里外部`servlet`容器查找在war包下的META-INF文件夹下MANIFEST.MF文件中定义的Main-class，`SpringBootServletInitializer`将负责绑定`Servlet`，`Filter`和`ServletContextInitializer`。
<a name="oioem"></a>
## 打包和部署
最后看看如何打包和部署应用程序。这两个框架都支持Maven和Gradle等通用包管理技术。但是在部署方面，这些框架差异很大。例如，Spring Boot Maven插件在Maven中提供Spring Boot支持。它还允许打包可执行jar或war包并就地运行应用程序。<br />在部署环境中Spring Boot 对比Spring的一些优点包括：<br />1、提供嵌入式容器支持<br />2、使用命令_`java -jar`_独立运行jar<br />3、在外部容器中部署时，可以选择排除依赖关系以避免潜在的jar冲突<br />4、部署时灵活指定配置文件的选项<br />5、用于集成测试的随机端口生成
<a name="SBoHC"></a>
## 结论
简而言之，可以说Spring Boot只是Spring本身的扩展，使开发，测试和部署更加方便。
