Java SpringBoot<br />在 Spring Boot开发过程中，经常会看到使用 `@EnableXXX`来激活某一个功能性的模块，通过类注解激活后就能使用所激活的配置带来的功能。<br />来探究一下这个 `@EnableXXX`做了哪些工作，或者应该怎么通过自定义的方式开发自己的功能模块。
<a name="wl5EK"></a>
## 演示环境

- IntelliJ IDEA 2020.2.1 (Community Edition)
- Maven 3.5.4
- Spring Boot 2.1.1.RELEASE
<a name="upf8Q"></a>
## 走进源码
在 SpringBoot中 `@Enable`的实现方式用两种。<br />一种是注解驱动的方式，以 `@EnableWebMvc`为例进行探究；<br />另外一种是接口编程的方式，以 `@EnableCaching`为例进行探究。
<a name="T5qvU"></a>
### 1、注解驱动方式（`@EnableWebMvc`）
在 Spring Boot项目中，可以使用 `@EnableWebMvc`注解用来激活 Spring MVC相关的配置，接下来进入源码一探究竟。
```java
@Retention(RetentionPolicy.RUNTIME)  
@Target(ElementType.TYPE)  
@Documented  
@Import(DelegatingWebMvcConfiguration.class)  
public @interface EnableWebMvc {  
}
```
通过观察上面的源码，就可以大胆的猜测，其实使用 `@EnableWebMvc`注解的作用就是导入 `DelegatingWebMvcConfiguration.class`这个类，接下来就进入这个类看看。
```java
@Configuration  
public class DelegatingWebMvcConfiguration extends WebMvcConfigurationSupport {  
  
    ........  
    @Override  
    protected void addInterceptors(InterceptorRegistry registry) {  
        this.configurers.addInterceptors(registry);  
    }  
  
    @Override  
    protected void addResourceHandlers(ResourceHandlerRegistry registry) {  
        this.configurers.addResourceHandlers(registry);  
    }  
    ........  
}
```
进入到这个类中发现了这个 `@Configuration`注解，到这儿好像明白了写什么。首先这个 `DelegatingWebMvcConfiguration`类继承了 `WebMvcConfigurationSupport`类，重写了里面的关于 WebMvc的相关配置，然后作为一个配置类加载到 Spring容器中。至此来实现启动（激活）WebMvc模块。
<a name="Gop58"></a>
### 2、接口编程的方式（`@EnableCaching`）
在 Spring Boot项目中，可以使用 `@EnableCaching`注解用来激活缓存相关的配置，接着进入源码看看到底做了什么。
```java
@Target(ElementType.TYPE)  
@Retention(RetentionPolicy.RUNTIME)  
@Documented  
@Import(CachingConfigurationSelector.class)  
public @interface EnableCaching {  
  
    boolean proxyTargetClass() default false;  
    AdviceMode mode() default AdviceMode.PROXY;  
    int order() default Ordered.LOWEST_PRECEDENCE;  
  
}
```
这里 `@EnableCaching`同样是使用 `@Import`导入了一个配置类，而它导入的是 `CachingConfigurationSelector`，接着进入这个类看一看。
```java
public class CachingConfigurationSelector extends AdviceModeImportSelector<EnableCaching> {  
  
    .....  
    
    @Override  
    public String[] selectImports(AdviceMode adviceMode) {  
        switch (adviceMode) {  
            case PROXY:  
                return getProxyImports();  
            case ASPECTJ:  
                return getAspectJImports();  
            default:  
                return null;  
        }  
    }  
    .....  
}
```
发现其实这个类没有被注解标注，但是它继承了 `AdviceModeImportSelector<enablecaching>`，而这个类又继承了 `ImportSelector`，并且可以看看 `ImportSelector`的代码：
```java
public interface ImportSelector {  
  
    /**  
     * Select and return the names of which class(es) should be imported based on  
     * the {@link AnnotationMetadata} of the importing @{@link Configuration} class.  
     */  
    String[] selectImports(AnnotationMetadata importingClassMetadata);  
  
}
```
这个类中只用一个方法，那就是 `selectImports`。也就是说当重写了这个方法之后，可以在方法中添加自己的逻辑判断，来决定最后导入哪些配置类。这样就可以实现灵活的加载配置。这个方法的返回值 `String[]`里面存放的是所有复合条件的配置类的全路径信息。
<a name="HxQYr"></a>
## 自定义实现
通过上面的分析已经完成了对 `@EnableXXX`套路的了解，接下来自己动手实现下。首先需要准备一个 Spirng Boot的项目，这里已经准备好了。
<a name="r4DAq"></a>
### 1、注解驱动方式的自定义实现
根据分析源码的步骤首先需要准备一个配置类。接下来在 configuration包下，创建一个 `HelloConfiguration`。代码如下：
```java
/**  
 * Hello模块的配置  
 */  
@Configuration  
public class HelloConfiguration {  
  
    @Bean  
    public String hello() { // method name is bean name  
        System.out.println("Bean : hello is loading.");  
        return "hello word !";  
    }  
  
}
```
这里被 `@Bean`标注的方法，方法名会作为 bean对象的名称。而且当该 bean被加载的时候还会在控制台输出一段话 Bean : hello is loading.。<br />配置类已经准备好了，接下来在 `annotation`包中编写激活配置的注解。
```java
/**  
 * 激活Hello模块配置  
 */  
@Retention(RetentionPolicy.RUNTIME)  
@Target(ElementType.TYPE)  
@Documented  
@Import(HelloConfiguration.class) // 模块装配：注解驱动实现  
public @interface EnableHello {  
}
```
最后在 bootstrap包中编写验证 `@EnableHello`模块的启动类。
```java
/**  
 * 验证自定义 {@link EnableHello} 模块化装配   
 */  
@EnableHello  
public class EnableHelloBootstrap {  
  
    public static void main(String[] args) {  
        ConfigurableApplicationContext context = new SpringApplicationBuilder(EnableHelloBootstrap.class)  
                .web(WebApplicationType.NONE)  
                .run(args);  
  
        String helloBean = context.getBean("hello", String.class);  
        System.out.println("hello Bean: " + helloBean);  
        context.close();  
    }  
}
```
在启动类上标注了 `@EnableHello`来激活 Hello模块，并且在 Spring Boot项目启动后，获取到了应用的上下文 `ConfigurableApplicationContext`。然后根据注入的 bean的名字 hello来获取 bean，接着打印 bean的内容，最后关闭上下文。<br />启动程序后，可以在控制台中看到这样两行输出：
```java
Bean : hello is loading.  
hello Bean: hello word !  
```
这刚好就是配置的内容，到这儿就完成了基于注解驱动方式激活（启动）配置。
<a name="LMUQu"></a>
### 2、接口编程方式自定义实现
根据读过源码后，首先最重要的是有一个继承 `ImportSelector`的实现类。在 `annotation`包中创建一个 `HelloImportSelector`配置类。
```java
/**  
 * Hello {@link ImportSelector} 的实现  
 */  
public class HelloImportSelector implements ImportSelector {  
  
    private final String key = "jerome";  
  
    @Override  
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {  
  
        if ("jerome".equals(key)) {  
            return new String[]{HelloJeromeConfiguration.class.getName()};  
        }  
  
        return new String[]{HelloConfiguration.class.getName()};  
    }  
}
```
这里为了体现出 selectImports方法的作用，在本类中添加一个 key字段。当 key的值为 jerome的时候去加载 `HelloJeromeConfiguration`这个配置类。首先看一个这个配置类的内容。
```java
/**  
 * Hello模块的配置   
 */  
@Configuration  
public class HelloJeromeConfiguration {  
  
    @Bean  
    public String hello() { // method name is bean name  
        System.out.println("Bean : hello is loading.");  
        return "hello jerome !";  
    }  
  
}
```
加载这个配置类后 bean的名字还是 hello但是内容变成了 hello jerome !。接着对 `@EnableHello`注解进行改造改造成 `@EnableHellos`。
```java
/**  
* 激活Hellos模块配置   
*/  
@Retention(RetentionPolicy.RUNTIME)  
@Target(ElementType.TYPE)  
@Documented  
@Import(HelloImportSelector.class) // 模块装配：接口驱动实现 优点可选bean  
public @interface EnableHellos {  
}  
```
这里使用了 `HelloImportSelector`来实现模块装配。<br />这里 `@EnableHellos`可能不符合命名规范，这里只做演示使用。<br />最后在 bootstrap包中编写验证 `@EnableHello`模块的启动类。
```java
/**  
 * 验证自定义 {@link EnableHellos} 模块化装配   
 */  
@EnableHellos  
public class EnableHellosBootstrap {  
  
    public static void main(String[] args) {  
        ConfigurableApplicationContext context = new SpringApplicationBuilder(EnableHellosBootstrap.class)  
                .web(WebApplicationType.NONE)  
                .run(args);  
  
        String helloBean = context.getBean("hello", String.class);  
        System.out.println("hello Bean: " + helloBean);  
        context.close();  
    }  
}
```
这里和基于注解驱动方式激活（启动）配置的验证方法一样，就是直接拷贝的，主要修改了 `@EnableHellos`这个激活注解。<br />启动程序后，可以在控制台中看到这样两行输出：
```java
Bean : hello is loading.  
hello Bean: hello jerome !  
```
这刚好就是配置的内容，到这儿就完成了基于接口编程方式激活（启动）配置。
<a name="p9sBQ"></a>
## 总结
通过上面了解了 `@EnableXXX`的两种实现方式。其中基于注解驱动方式激活（启动）配置的方式是相对方便的一个实现，需要激活什么就直接导入该配置类即可；而基于接口编程方式激活（启动）配置的方式相对更加灵活，可以根据自定义的规则来选择激活哪些配置类，不激活哪些配置类，这是一种比较灵活的方式，方便对其进行扩展。<br />总之这两种方式都是手动激活的方式，换言之需要在启动类上添加 `@Enable***`的注解，来手动激活配置，这种方式在某些场景并不是很方便。
