Java SpringBoot 
<a name="tlAU0"></a>
## 一、什么是自动配置
<a name="Yon3o"></a>
### 1.1 引入redisTemplate
只要在pom.xml文件中引入spring-boot-starter-data-redis-xxx.jar包，然后只要在配置文件中配置redis连接，如：
```java
spring.redis.database = 0
spring.redis.timeout = 10000
spring.redis.host = 10.72.16.9
spring.redis.port = 6379
spring.redis.pattern = 1
```
就可以在service方法中直接注入`StringRedisTemplate`对象的实例，可以直接使用了。有没有想过这是为什么？
```java
@Autowired
private StringRedisTemplate stringRedisTemplate;
```
<a name="FUcYK"></a>
### 1.2  引入`transactionTemplate`
在项目中只要引入spring-boot-starter-xxx.jar，事务就自动生效了，并且可以直接在service方法中直接注入`TransactionTemplate`，用它开发编程式事务代码。是不是很神奇？这又是为什么？
<a name="e0ONo"></a>
### 1.3  使用`@ConfigurationProperties`
使用`@ConfigurationProperties`可以把指定路径下的属性，直接注入到实体对象中，看看下面这个例子：
```java
@Data
@Component
@ConfigurationProperties("jump.threadpool")
public class ThreadPoolProperties {

    private int corePoolSize;
    private int maxPoolSize;
    private int keepAliveSeconds;
    private int queueCapacity;
}
```
只要`application.properties`这样配置，就可以自动注入到上面的实体中
```java
jump.threadpool.corePoolSize=8
jump.threadpool.maxPoolSize=16
jump.threadpool.keepAliveSeconds=10
jump.threadpool.queueCapacity=100
```
没错，这三个例子都是SpringBoot自动配置在起作用，分为两种情况：bean的自动配置 和 属性的自动配置。
<a name="ARQOd"></a>
## 二、工作原理
<a name="bCZXA"></a>
### 2.1 bean的自动配置
Spring Boot的启动类上有一个`@SpringBootApplication`注解，这个注解是Spring Boot项目必不可少的注解。<br />先看看`@SpringBootApplication`注解<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623487994950-775ac2f8-f120-422f-91c9-c79400b03bc7.png#clientId=u015df49d-d832-4&from=paste&id=u4010b6cb&originHeight=317&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=ufbf15869-6402-45d2-9b12-507317d0c77)<br />它上面定义了另外一个注解：`@EnableAutoConfiguration`<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623487993954-8f507612-3392-4ec1-9985-728622d455ca.webp#clientId=u015df49d-d832-4&from=paste&id=u06067f35&originHeight=286&originWidth=804&originalType=url&ratio=3&status=done&style=none&taskId=u71091ce1-71a5-401d-9e1e-9cac7ef89be)<br />该注解的关键功能由`@Import`提供，其导入的`AutoConfigurationImportSelector`的`selectImports()`方法通过`SpringFactoriesLoader.loadFactoryNames()`扫描所有具有`META-INF/spring.factories`的jar包下面key是`EnableAutoConfiguration`全名的，所有自动配置类。<br />看看SpringBoot的spring-boot-autoconfigure-xxx.jar<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623487994048-732f8978-92e8-4a59-ab7b-a320c5cbfe9d.png#clientId=u015df49d-d832-4&from=paste&id=ua328f10a&originHeight=784&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u64d9cd3e-aa2d-4f74-86d1-05793871311)<br />该jar包里面就有`META-INF/spring.factories`文件。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623487993944-94600f0c-61c8-4bd9-bde9-add6aa5a33e6.png#clientId=u015df49d-d832-4&from=paste&id=ud7757898&originHeight=512&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=ua06ca9d3-d725-47ef-9ace-1080a8bc510)<br />这个`spring.factories`文件是一组一组的`key=value`的形式，其中一个key是`EnableAutoConfiguration`类的全类名，而它的value是一个`xxxxAutoConfiguration`的类名的列表，这些类名以逗号分隔。<br />`@EnableAutoConfiguration`注解通过`@SpringBootApplication`被间接的标记在了Spring Boot的启动类上。在`SpringApplication.run(...)`的内部就会执行`selectImports()`方法，找到所有`JavaConfig`自动配置类的全限定名对应的`class`，然后将所有自动配置类加载到Spring容器中。<br />`SpringApplication.run(...)`方法怎么调到`selectImports()`方法的<br />加载过程大概是这样的：<br />`SpringApplication.run(...)`方法  》 <br />`AbstractApplicationContext.refresh()`方法  》 <br />`invokeBeanFactoryPostProcessors(...)`方法  》 <br />`PostProcessorRegistrationDelegate.invokeBeanFactoryPostProcessors(...)` 方法  》<br />`ConfigurationClassPostProcessor.postProcessBeanDefinitionRegistry(..)`方法  》<br />`AutoConfigurationImportSelector.selectImports`<br />该方法会找到自动配置的类，并给打了`@Bean`注解的方法创建对象。<br />`postProcessBeanDefinitionRegistry`方法是最核心的方法，它负责解析`@Configuration`、`@Import`、`@ImportSource`、`@Component`、`@ComponentScan`、`@Bean`等，完成bean的自动配置功能。<br />回到刚刚第二个例子`TransactionTemplate`为什么可以直接引用？<br />是因为在spring-boot-autoconfigure-xxx.jar的`spring.factories`配置文件中，`EnableAutoConfiguration`全类名下配置了`TransactionAutoConfiguration`全类名，SpringBoot在启动的时候会加载这个类。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623487994255-a4b5fcfb-d342-425a-a8ed-2b72f4691094.webp#clientId=u015df49d-d832-4&from=paste&id=u7312f49d&originHeight=420&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u867820b6-b701-4d89-a2f2-419b89520c2)<br />而`TransactionAutoConfiguration`类是一个配置类，它里面创建`TransactionTemplate`类的实例。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623487994967-0f036921-a8e0-402d-9099-0a0ea1ac0763.png#clientId=u015df49d-d832-4&from=paste&id=u6f43a884&originHeight=715&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u0af95a54-9085-443f-a420-ca76d850a17)<br />这样在其他地方就可以直接注入`TransactionTemplate`类的实例。
<a name="f7T0a"></a>
### 2.2 属性的自动配置
属性的自动配置是通过`ConfigurationPropertiesBindingPostProcessor`类的`postProcessBeforeInitialization`方法完成，
```java
public Object postProcessBeforeInitialization(Object bean, String beanName)
    throws BeansException {
    ConfigurationProperties annotation = getAnnotation(bean, beanName,
                                                       ConfigurationProperties.class);
    if (annotation != null) {
        bind(bean, beanName, annotation);
    }
    return bean;
}
```
它会解析`@ConfigurationProperties`注解上的属性，将配置文件中对应key的值绑定到属性上。
<a name="hWnOR"></a>
## 三、自动配置的生效条件
每个`xxxxAutoConfiguration`类上都可以定义一些生效条件，这些条件基本都是从`@Conditional`派生出来的。<br />常用的条件如下：<br />`@ConditionalOnBean`：当容器里有指定的bean时生效<br />`@ConditionalOnMissingBean`：当容器里不存在指定bean时生效<br />`@ConditionalOnClass`：当类路径下有指定类时生效<br />`@ConditionalOnMissingClass`：当类路径下不存在指定类时生效<br />`@ConditionalOnProperty`：指定的属性是否有指定的值，比如`@ConditionalOnProperties(prefix=”xxx.xxx”, value=”enable”, matchIfMissing=true)`，代表当xxx.xxx为`enable`时条件的布尔值为`true`，如果没有设置的情况下也为`true`。<br />举个比较常用的例子看看`TransactionAutoConfiguration`，是如何使用条件的<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623487994925-ae62d06a-05bd-411d-8821-ebe9ac6253fa.png#clientId=u015df49d-d832-4&from=paste&id=ud9ae931c&originHeight=451&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u6efdd1cd-c5e7-4133-86e8-65f7fe84a2e)<br />可以看到，条件用的是：`@ConditionalOnClass`，表示`TransactionAutoConfiguration`类只有在`PlatformTransactionManager`类存在时才会生效。
