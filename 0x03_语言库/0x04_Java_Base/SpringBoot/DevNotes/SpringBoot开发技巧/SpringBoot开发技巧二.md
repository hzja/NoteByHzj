Java Spring
<a name="ld4Xj"></a>
### 一、`@Conditional`的强大之处
在项目开发中是否会遇到过这些问题：

1. 某个功能需要根据项目中有没有某个jar判断是否开启该功能。
2. 某个bean的实例化需要先判断另一个bean有没有实例化，再判断是否实例化自己。
3. 某个功能是否开启，在配置文件中有个参数可以对它进行控制。
<a name="su1Vc"></a>
#### `@ConditionalOnClass`
问题1可以用`@ConditionalOnClass`注解解决，代码如下：
```java
public class A {
}
public class B {
}
@ConditionalOnClass(B.class)
@Configuration
public class TestConfiguration {
    @Bean
    public A a() {
      return new A();
    }
}
```
如果项目中存在B类，则会实例化A类。如果不存在B类，则不会实例化A类。<br />有人可能会问：不是判断有没有某个jar吗？怎么现在判断某个类了？
> 直接判断有没有该jar下的某个关键类更简单。

这个注解有个升级版的应用场景：比如common工程中写了一个发消息的工具类mqTemplate，业务工程引用了common工程，只需再引入消息中间件，比如rocketmq的jar包，就能开启mqTemplate的功能。而如果有另一个业务工程，通用引用了common工程，如果不需要发消息的功能，不引入rocketmq的jar包即可。
<a name="F76Un"></a>
#### `@ConditionalOnBean`
问题2可以通过`@ConditionalOnBean`注解解决，代码如下：
```java
@Configuration
public class TestConfiguration {
    @Bean
    public B b() {
        return new B();
    }
    @ConditionalOnBean(name="b")
    @Bean
    public A a() {
      return new A();
    }
}
```
实例A只有在实例B存在时，才能实例化。
<a name="YNbd4"></a>
#### `@ConditionalOnProperty`
问题3可以通过`@ConditionalOnProperty`注解解决，代码如下：
```java
@ConditionalOnProperty(prefix = "demo",name="enable", havingValue = "true",matchIfMissing=true )
@Configuration
public class TestConfiguration {
    @Bean
    public A a() {
      return new A();
    }
}
```
在applicationContext.properties文件中配置参数：
```
demo.enable=false
```
各参数含义：

- prefix 表示参数名的前缀，这里是demo
- name 表示参数名
- havingValue 表示指定的值，参数中配置的值需要跟指定的值比较是否相等，相等才满足条件
- matchIfMissing 表示是否允许缺省配置。

这个功能可以作为开关，相比EnableXXX注解的开关更优雅，因为它可以通过参数配置是否开启，而EnableXXX注解的开关需要在代码中硬编码开启或关闭。
<a name="jAqVy"></a>
#### 其他的Conditional注解
当然，Spring用得比较多的Conditional注解还有：`ConditionalOnMissingClass`、`ConditionalOnMissingBean`、`ConditionalOnWebApplication`等。<br />下面用一张图整体认识一下`@Conditional`家族。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611842203707-44c3f92c-85df-4dc6-9b8d-02229121fa38.webp#align=left&display=inline&height=1116&originHeight=1116&originWidth=954&size=0&status=done&style=none&width=954)
<a name="js74H"></a>
#### 自定义Conditional
SpringBoot自带的Conditional系列已经可以满足绝大多数的需求了。但如果有比较特殊的场景，也可以自定义自定义Conditional。<br />第一步，自定义注解：
```java
@Conditional(MyCondition.class)
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE, ElementType.METHOD})
@Documented
public @interface MyConditionOnProperty {
    String name() default "";
    String havingValue() default "";
}
```
第二步，实现Condition接口：
```java
public class MyCondition implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        System.out.println("实现自定义逻辑");
        return false;
    }
}
```
第三步，使用`@MyConditionOnProperty`注解。<br />Conditional的奥秘就藏在`ConfigurationClassParser`类的`processConfigurationClass`方法中：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611842568896-5eb40060-07df-451a-a395-88612e3a0a89.png#align=left&display=inline&height=457&originHeight=1372&originWidth=3526&size=386532&status=done&style=shadow&width=1175.3333333333333)<br />这个方法逻辑不复杂：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1611842203627-10b981c1-b615-42ff-a22b-ac1d8c9da817.png#align=left&display=inline&height=763&originHeight=763&originWidth=1080&size=0&status=done&style=none&width=1080)

1. 先判断有没有使用`@Conditional`注解，如果没有直接返回false
2. 收集`condition`到集合中
3. 按`order`排序该集合
4. 遍历该集合，循环调用`condition`的`matchs`方法。
<a name="d1MTi"></a>
### 二. `@Import`
有时需要在某个配置类中引入另外一些类，被引入的类也加到Spring容器中。这时可以使用`@Import`注解完成这个功能。<br />如果看过它的源码会发现，引入的类支持三种不同类型。<br />但是最好将普通类和`@Configuration`注解的配置类分开讲解，所以列了四种不同类型：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611842203637-b326c0d9-7870-455f-95c3-54d75666cede.webp#align=left&display=inline&height=326&originHeight=326&originWidth=1080&size=0&status=done&style=none&width=1080)
<a name="oBDLB"></a>
#### 普通类
这种引入方式是最简单的，被引入的类会被实例化bean对象。
```java
public class A {
}
@Import(A.class)
@Configuration
public class TestConfiguration {
}
```
通过`@Import`注解引入A类，Spring就能自动实例化A对象，然后在需要使用的地方通过`@Autowired`注解注入即可：
```java
@Autowired
private A a;
```
不用加`@Bean`注解也能实例化bean。
<a name="qtQZ0"></a>
#### `@Configuration`注解的配置类
这种引入方式是最复杂的，因为`@Configuration`注解还支持多种组合注解，比如：

- `@Import`
- `@ImportResource`
- `@PropertySource`等。
```java
public class A {
}
public class B {
}
@Import(B.class)
@Configuration
public class AConfiguration {
    @Bean
    public A a() {
        return new A();
    }
}
@Import(AConfiguration.class)
@Configuration
public class TestConfiguration {
}
```
通过`@Import`注解引入`@Configuration`注解的配置类，会把该配置类相关`@Import`、`@ImportResource`、`@PropertySource`等注解引入的类进行递归，一次性全部引入。
<a name="mc7Qc"></a>
#### 实现`ImportSelector`接口的类
这种引入方式需要实现`ImportSelector`接口：
```java
public class AImportSelector implements ImportSelector {
private static final String CLASS_NAME = "com.sue.cache.service.test13.A";
    
 public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{CLASS_NAME};
    }
}
@Import(AImportSelector.class)
@Configuration
public class TestConfiguration {
}
```
这种方式的好处是`selectImports`方法返回的是数组，意味着可以同时引入多个类，还是非常方便的。
<a name="iiRJa"></a>
#### 实现`ImportBeanDefinitionRegistrar`接口的类
这种引入方式需要实现`ImportBeanDefinitionRegistrar`接口：
```java
public class AImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {
    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
        RootBeanDefinition rootBeanDefinition = new RootBeanDefinition(A.class);
        registry.registerBeanDefinition("a", rootBeanDefinition);
    }
}
@Import(AImportBeanDefinitionRegistrar.class)
@Configuration
public class TestConfiguration {
}
```
这种方式是最灵活的，能在`registerBeanDefinitions`方法中获取到`BeanDefinitionRegistry`容器注册对象，可以手动控制`BeanDefinition`的创建和注册。<br />当然`@import`注解非常人性化，还支持同时引入多种不同类型的类。
```java
@Import({B.class,AImportBeanDefinitionRegistrar.class})
@Configuration
public class TestConfiguration {
}
```
这四种引入类的方式各有千秋，总结如下：

1. 普通类，用于创建没有特殊要求的bean实例。
2. `@Configuration`注解的配置类，用于层层嵌套引入的场景。
3. 实现`ImportSelector`接口的类，用于一次性引入多个类的场景，或者可以根据不同的配置决定引入不同类的场景。
4. 实现`ImportBeanDefinitionRegistrar`接口的类，主要用于可以手动控制`BeanDefinition`的创建和注册的场景，它的方法中可以获取`BeanDefinitionRegistry`注册容器对象。

在`ConfigurationClassParser`类的`processImports`方法中可以看到这三种方式的处理逻辑：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611844598455-2a1572fc-c55d-4d57-9b76-9f963d7b1abd.png#align=left&display=inline&height=516&originHeight=1549&originWidth=3652&size=361306&status=done&style=shadow&width=1217.3333333333333)<br />最后的else方法其实包含了：普通类和`@Configuration`注解的配置类两种不同的处理逻辑。
<a name="spZW0"></a>
### 三、`@ConfigurationProperties`赋值
在项目中使用配置参数是非常常见的场景，比如在配置线程池的时候，需要在`applicationContext.propeties`文件中定义如下配置：
```
thread.pool.corePoolSize=5
thread.pool.maxPoolSize=10
thread.pool.queueCapacity=200
thread.pool.keepAliveSeconds=30
```
方法一：通过`@Value`注解读取这些配置。
```java
public class ThreadPoolConfig {
    @Value("${thread.pool.corePoolSize:5}")
    private int corePoolSize;
    @Value("${thread.pool.maxPoolSize:10}")
    private int maxPoolSize;
    @Value("${thread.pool.queueCapacity:200}")
    private int queueCapacity;
    @Value("${thread.pool.keepAliveSeconds:30}")
    private int keepAliveSeconds;
    @Value("${thread.pool.threadNamePrefix:ASYNC_}")
    private String threadNamePrefix;
    @Bean
    public Executor threadPoolExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(corePoolSize);
        executor.setMaxPoolSize(maxPoolSize);
        executor.setQueueCapacity(queueCapacity);
        executor.setKeepAliveSeconds(keepAliveSeconds);
        executor.setThreadNamePrefix(threadNamePrefix);
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        executor.initialize();
        return executor;
    }
}
```
这种方式使用起来非常简单，但建议在使用时都加上`:`，因为`:`后面跟的是默认值，比如：`@Value("${thread.pool.corePoolSize:5}")`，定义的默认核心线程数是5。
> 假如有这样的场景：business工程下定义了这个ThreadPoolConfig类，api工程引用了business工程，同时job工程也引用了business工程，而ThreadPoolConfig类只想在api工程中使用。这时，如果不配置默认值，job工程启动的时候可能会报错。

如果参数少还好，多的话，需要给每一个参数都加上`@Value`注解，是不是有点麻烦？<br />此外，还有一个问题，`@Value`注解定义的参数看起来有点分散，不容易辨别哪些参数是一组的。<br />这时，`@ConfigurationProperties`就派上用场了，它是SpringBoot中新加的注解。<br />第一步，先定义`ThreadPoolProperties`类
```java
@Data
@Component
@ConfigurationProperties("thread.pool")
public class ThreadPoolProperties {
    private int corePoolSize;
    private int maxPoolSize;
    private int queueCapacity;
    private int keepAliveSeconds;
    private String threadNamePrefix;
}
```
第二步，使用`ThreadPoolProperties`类
```java
@Configuration
public class ThreadPoolConfig {
    @Autowired
    private ThreadPoolProperties threadPoolProperties;
    @Bean
    public Executor threadPoolExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(threadPoolProperties.getCorePoolSize());
        executor.setMaxPoolSize(threadPoolProperties.getMaxPoolSize());
        executor.setQueueCapacity(threadPoolProperties.getQueueCapacity());
        executor.setKeepAliveSeconds(threadPoolProperties.getKeepAliveSeconds());
        executor.setThreadNamePrefix(threadPoolProperties.getThreadNamePrefix());
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        executor.initialize();
        return executor;
    }
}
```
使用`@ConfigurationProperties`注解，可以将`thread.pool`开头的参数直接赋值到`ThreadPoolProperties`类的同名参数中，这样省去了像`@Value`注解那样一个个手动去对应的过程。<br />这种方式显然要方便很多，只需编写xxxProperties类，Spring会自动装配参数。此外，不同系列的参数可以定义不同的xxxProperties类，也便于管理，推荐优先使用这种方式。<br />它的底层是通过：`ConfigurationPropertiesBindingPostProcessor`类实现的，该类实现了`BeanPostProcessor`接口，在`postProcessBeforeInitialization`方法中解析`@ConfigurationProperties`注解，并且绑定数据到相应的对象上。<br />绑定是通过`Binder`类的`bindObject`方法完成的：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611844391517-3eb5e9c0-5b46-47a8-a594-51c5ed321f84.png#align=left&display=inline&height=445&originHeight=1336&originWidth=3525&size=374412&status=done&style=shadow&width=1175)<br />以上这段代码会递归绑定数据，主要考虑了三种情况：

- `bindAggregate` 绑定集合类
- `bindBean` 绑定对象
- `bindProperty` 绑定参数 前面两种情况最终也会调用到`bindProperty`方法。

**「此外，友情提醒一下：」**<br />使用`@ConfigurationProperties`注解有些场景有问题，比如：在apollo中修改了某个参数，正常情况可以动态更新到`@ConfigurationProperties`注解定义的xxxProperties类的对象中，但是如果出现比较复杂的对象，比如：
```java
private Map<String, Map<String,String>>  urls;
```
可能动态更新不了。这时候该怎么办呢？<br />答案是使用`ApolloConfigChangeListener`监听器自己处理：
```java
@ConditionalOnClass(com.ctrip.framework.apollo.spring.annotation.EnableApolloConfig.class)
public class ApolloConfigurationAutoRefresh implements ApplicationContextAware {
   private ApplicationContext applicationContext;
   
   @Override
   public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
   }
   
    @ApolloConfigChangeListener
    private void onChange(ConfigChangeEvent changeEvent{
        refreshConfig(changeEvent.changedKeys());
    }
    private void refreshConfig(Set<String> changedKeys){
       System.out.println("将变更的参数更新到相应的对象中");
    }
}
```
<a name="2MMBa"></a>
### 四、Spring事务避坑
Spring中的事务功能主要分为：`声明式事务`和`编程式事务`。
<a name="p7td2"></a>
#### 声明式事务
大多数情况下，在开发过程中使用更多的可能是`声明式事务`，即使用`@Transactional`注解定义的事务，因为它用起来更简单，方便。<br />只需在需要执行的事务方法上，加上`@Transactional`注解就能自动开启事务：
```java
@Service
public class UserService {
    @Autowired
    private UserMapper userMapper;
    
    @Transactional
    public void add(UserModel userModel) {
        userMapper.insertUser(userModel);
    }
}
```
这种声明式事务之所以能生效，是因为它的底层使用了AOP，创建了代理对象，调用`TransactionInterceptor`拦截器实现事务的功能。
> Spring事务有个特别的地方：它获取的数据库连接放在`ThreadLocal`中的，也就是说同一个线程中从始至终都能获取同一个数据库连接，可以保证同一个线程中多次数据库操作在同一个事务中执行。

正常情况下是没有问题的，但是如果使用不当，事务会失效，主要原因如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611842203716-1ce3a641-935d-42a4-b0ef-f571e0e1e927.webp#align=left&display=inline&height=577&originHeight=577&originWidth=1080&size=0&status=done&style=none&width=1080)<br />除了上述列举的问题之外，由于`@Transactional`注解最小粒度是要被定义在方法上，如果有多层的事务方法调用，可能会造成大事务问题。<br />所以，建议在实际工作中少用`@Transactional`注解开启事务。
<a name="PbfUG"></a>
#### 编程式事务
一般情况下编程式事务可以通过`TransactionTemplate`类开启事务功能。SpringBoot已经默认实例化好这个对象了，能直接在项目中使用。
```java
@Service
public class UserService {
   @Autowired
   private TransactionTemplate transactionTemplate;
   
   ...
   
   public void save(final User user) {
         transactionTemplate.execute((status) => {
            doSameThing...
            return Boolean.TRUE;
         })
   }
}
```
使用`TransactionTemplate`的编程式事务能避免很多事务失效的问题，但是对大事务问题，不一定能够解决，只是说相对于使用`@Transactional`注解要好些。
<a name="VFlp4"></a>
### 五、跨域问题的解决方案
关于跨域问题，前后端的解决方案还是挺多的，Spring的解决方案，目前有三种：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611842203660-c754062c-b146-4ba6-9c15-b044a678d7b2.webp#align=left&display=inline&height=292&originHeight=292&originWidth=1080&size=0&status=done&style=none&width=1080)
<a name="H8nWd"></a>
#### 1. 使用@CrossOrigin注解
```java
@RequestMapping("/user")
@RestController
public class UserController {
    @CrossOrigin(origins = "http://localhost:8016")
    @RequestMapping("/getUser")
    public String getUser(@RequestParam("name") String name) {
        System.out.println("name:" + name);
        return "success";
    }
}
```
该方案需要在跨域访问的接口上加`@CrossOrigin`注解，访问规则可以通过注解中的参数控制，控制粒度更细。如果需要跨域访问的接口数量较少，可以使用该方案。
<a name="wmedH"></a>
#### 2. 增加全局配置
```java
@Configuration
public class WebConfig implements WebMvcConfigurer {
    @Override
    public void addCorsMappings(CorsRegistry registry) {
        registry.addMapping("/**")
                .allowedOrigins("*")
                .allowedMethods("GET", "POST")
                .allowCredentials(true)
                .maxAge(3600)
                .allowedHeaders("*");
    }
}
```
该方案需要实现`WebMvcConfigurer`接口，重写`addCorsMappings`方法，在该方法中定义跨域访问的规则。这是一个全局的配置，可以应用于所有接口。
<a name="cFmsH"></a>
#### 3. 自定义过滤器
```java
@WebFilter("corsFilter")
@Configuration
public class CorsFilter implements Filter {
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        HttpServletResponse httpServletResponse = (HttpServletResponse) response;
        httpServletResponse.setHeader("Access-Control-Allow-Origin", "*");
        httpServletResponse.setHeader("Access-Control-Allow-Methods", "POST, GET");
        httpServletResponse.setHeader("Access-Control-Max-Age", "3600");
        httpServletResponse.setHeader("Access-Control-Allow-Headers", "x-requested-with");
        chain.doFilter(request, response);
    }
    @Override
    public void destroy() {
    }
}
```
该方案通过在请求的`header`中增加`Access-Control-Allow-Origin`等参数解决跨域问题。<br />使用`@CrossOrigin`注解 和 实现`WebMvcConfigurer`接口的方案，Spring在底层最终都会调用到`DefaultCorsProcessor`类的`handleInternal`方法：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611844221635-2e3f9101-3e3f-4daf-86e7-d5e15b77d987.png#align=left&display=inline&height=510&originHeight=1530&originWidth=2658&size=360466&status=done&style=shadow&width=886)<br />最终三种方案殊途同归，都会往`header`中添加跨域需要参数，只是实现形式不一样而已。
<a name="5l1yH"></a>
### 六、自定义starter
以前在没有使用`starter`时，在项目中需要引入新功能，步骤一般是这样的：

- 在maven仓库找该功能所需jar包
- 在maven仓库找该jar所依赖的其他jar包
- 配置新功能所需参数

以上这种方式会带来三个问题：

1. 如果依赖包较多，找起来很麻烦，容易找错，而且要花很多时间。
2. 各依赖包之间可能会存在版本兼容性问题，项目引入这些jar包后，可能没法正常启动。
3. 如果有些参数没有配好，启动服务也会报错，没有默认配置。

**「为了解决这些问题，springboot的`starter`机制应运而生」**。<br />starter机制带来这些好处：

1. 它能启动相应的默认配置。
2. 它能够管理所需依赖，摆脱了需要到处找依赖 和 兼容性问题的困扰。
3. 自动发现机制，将spring.factories文件中配置的类，自动注入到spring容器中。
4. 遵循“约定大于配置”的理念。

在业务工程中只需引入starter包，就能使用它的功能。<br />下面用一张图，总结starter的几个要素：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1611842203756-d57beaa6-f334-4542-b76f-b976a7bac926.png#align=left&display=inline&height=800&originHeight=800&originWidth=768&size=0&status=done&style=none&width=768)<br />自定义starter。<br />第一步，创建id-generate-starter工程：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611842203746-a9550143-2ca1-4193-a410-aa969e798702.webp#align=left&display=inline&height=453&originHeight=453&originWidth=1080&size=0&status=done&style=none&width=1080)其中的pom.xml配置如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <version>1.3.1</version>
    <groupId>com.sue</groupId>
    <artifactId>id-generate-spring-boot-starter</artifactId>
    <name>id-generate-spring-boot-starter</name>
    <dependencies>
        <dependency>
            <groupId>com.sue</groupId>
            <artifactId>id-generate-spring-boot-autoconfigure</artifactId>
            <version>1.3.1</version>
        </dependency>
    </dependencies>
</project>
```
第二步，创建id-generate-spring-boot-autoconfigure工程：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611842203764-02da5cb4-c74d-492a-8f52-8bb14708469a.webp#align=left&display=inline&height=664&originHeight=664&originWidth=1080&size=0&status=done&style=none&width=1080)该项目当中包含：

- pom.xml
- spring.factories
- IdGenerateAutoConfiguration
- IdGenerateService
- IdProperties pom.xml配置如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.0.4.RELEASE</version>
    </parent>
    <modelVersion>4.0.0</modelVersion>
    <version>1.3.1</version>
    <groupId>com.sue</groupId>
    <artifactId>id-generate-spring-boot-autoconfigure</artifactId>
    <name>id-generate-spring-boot-autoconfigure</name>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-autoconfigure</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-configuration-processor</artifactId>
            <optional>true</optional>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <source>1.8</source>
                    <target>1.8</target>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```
spring.factories配置如下：
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=com.sue.IdGenerateAutoConfiguration
```
IdGenerateAutoConfiguration类：
```java
@ConditionalOnClass(IdProperties.class)
@EnableConfigurationProperties(IdProperties.class)
@Configuration
public class IdGenerateAutoConfiguration {
    @Autowired
    private IdProperties properties;
    @Bean
    public IdGenerateService idGenerateService() {
        return new IdGenerateService(properties.getWorkId());
    }
}
```
IdGenerateService类：
```java
public class IdGenerateService {
    private Long workId;
    public IdGenerateService(Long workId) {
        this.workId = workId;
    }
    public Long generate() {
        return new Random().nextInt(100) + this.workId;
    }
}
```
IdProperties类：
```java
@ConfigurationProperties(prefix = IdProperties.PREFIX)
public class IdProperties {
    public static final String PREFIX = "sue";
    private Long workId;
    public Long getWorkId() {
        return workId;
    }
    public void setWorkId(Long workId) {
        this.workId = workId;
    }
}
```
这样在业务项目中引入相关依赖:
```xml
<dependency>
      <groupId>com.sue</groupId>
      <artifactId>id-generate-spring-boot-starter</artifactId>
      <version>1.3.1</version>
</dependency>
```
就能使用注入使用IdGenerateService的功能了
```java
@Autowired
private IdGenerateService idGenerateService;
```
<a name="qmuDD"></a>
### 七、项目启动时的附加功能
有时候需要在项目启动时定制化一些附加功能，比如：加载一些系统参数、完成初始化、预热本地缓存等，该怎么办呢？<br />SpringBoot提供了：

- CommandLineRunner
- ApplicationRunner

这两个接口可以实现以上需求。<br />它们的用法还是挺简单的，以`ApplicationRunner`接口为例：
```java
@Component
public class TestRunner implements ApplicationRunner {
    @Autowired
    private LoadDataService loadDataService;
    public void run(ApplicationArguments args) throws Exception {
        loadDataService.load();
    }
    
}
```
实现`ApplicationRunner`接口，重写`run`方法，在该方法中实现自己定制化需求。<br />如果项目中有多个类实现了`ApplicationRunner`接口，他们的执行顺序要怎么指定呢？<br />答案是使用`@Order(n)`注解，n的值越小越先执行。当然也可以通过`@Priority`注解指定顺序。<br />SpringBoot项目启动时主要流程是这样的：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611842203852-67cb107e-25e7-4b92-bc65-261956201332.webp#align=left&display=inline&height=870&originHeight=870&originWidth=372&size=0&status=done&style=shadow&width=372)<br />在`SpringApplication`类的`callRunners`方法中，可以看到这两个接口的具体调用：![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611843835287-4a11a75e-b386-451a-a7ab-00c7bb1eb12a.png#align=left&display=inline&height=351&originHeight=1053&originWidth=2741&size=272967&status=done&style=shadow&width=913.6666666666666)<br />这两个接口的区别

- `CommandLineRunner`接口中run方法的参数为String数组
- `ApplicationRunner`中`run`方法的参数为`ApplicationArguments`，该参数包含了String数组参数和一些可选参数。
