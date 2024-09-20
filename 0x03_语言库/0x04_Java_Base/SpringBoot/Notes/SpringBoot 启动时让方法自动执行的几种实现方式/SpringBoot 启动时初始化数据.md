Java SpringBoot
<a name="F1trI"></a>
## 1、前言
在使用 SpringBoot搭建项目的时候，有时候会碰到在项目启动时初始化一些操作的需求，针对这种需求 SpringBoot(Spring) 提供了以下几种方案:

- `ApplicationRunner` 与 `CommandLineRunner` 接口
- Spring Bean 初始化的InitializingBean, init-method 和 `PostConstruct`
- Spring 的事件机制
<a name="Wid1X"></a>
## 2、`ApplicationRunner`与`CommandLineRunner`
如果需要在`SpringApplication`启动时执行一些特殊的代码，可以实现`ApplicationRunner`或 `CommandLineRunner` 接口，这两个接口工作方式相同，都只提供单一的 `run()` 方法，而且该方法仅在`SpringApplication.run(…)`完成之前调用，更准确的说是在构造 SpringApplication 实例完成之后调用 `run()` 的时候，具体分析见后文，所以这里将他们分为一类。
<a name="w3Nns"></a>
### `ApplicationRunner`
构造一个类实现`ApplicationRunner`接口
```java
@Component
public class ApplicationRunnerTest implements ApplicationRunner {

    @Override
    public void run(ApplicationArguments args) throws Exception {
        System.out.println("ApplicationRunner");
    }
}
```
<a name="zpUiu"></a>
### `CommandLineRunner`
对于这两个接口而言，可以通过 `Order` 注解或者使用 Ordered 接口来指定调用顺序，`@Order()` 中的值越小，优先级越高
```java
@Component
@Order(1)
public class CommandLineRunnerTest implements CommandLineRunner {

    @Override
    public void run(String... args) throws Exception {
        System.out.println("CommandLineRunner...");
    }
}
```
当然也可以同时使用 `ApplicationRunner` 和 `CommandLineRunner`，默认情况下前者比后者先执行，但是这没有必要，使用一个就可以了。
<a name="VazlZ"></a>
### 两者的联系与区别
这两个接口都有 `run()` 方法，只不过它们的参数不一样，`CommandLineRunner`的参数是最原始的参数，没有进行任何处理，`ApplicationRunner` 的参数是`ApplicationArguments`，是对原始参数的进一步封装。<br />简要跟踪一下源码看 `ApplicationRunner(CommandLineRunner)` 是如何被调用的。<br />SpringBoot在启动的时候，都会构造一个 `SpringApplication` 实例，至于这个实例怎么构造的，这里不去探究了，有感兴趣的可以去看下源码。这里主要看`ApplicationRunner` 是如何被调用的，而它的调用就是在`SpringApplication`这个实例调用`run`方法中。
```java
@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
进入run方法
```java
public static ConfigurableApplicationContext run(Class<?> primarySource,
        String... args) {
    return run(new Class<?>[] { primarySource }, args);
}
```
执行 `SpringApplication` 的 `run` 方法
```java
public static ConfigurableApplicationContext run(Class<?>[] primarySources,
        String[] args) {
    return new SpringApplication(primarySources).run(args);
}
```
一路点击 `run()` 来，发现对 `ApplicationRunner` 的调用实际上在 `callRunners` 方法中<br />对于 `CommandLineRunner` 或者 `ApplicationRunner` 来说，需要注意的两点：

- 所有 `CommandLineRunner` / `ApplicationRunner` 的执行时点是在 SpringBoot 应用的 `ApplicationContext` 完全初始化开始工作之后，`callRunners()` 可以看出是 run 方法内部最后一个调用的方法(可以认为是`main`方法执行完成之前最后一步)
- 只要存在于当前 SpringBoot 应用的 `ApplicationContext` 中的任何`CommandLineRunner` / `ApplicationRunner`，都会被加载执行(不管是手动注册还是自动扫描去Ioc容器)
<a name="MWERd"></a>
## 3、Spring Bean初始化的InitializingBean,init-method和PostConstruct
<a name="IRAal"></a>
### `InitializingBean`接口
InitializingBean 接口为 bean 提供了初始化方法的方式，它只包括 `afterPropertiesSet()`方法。<br />在 spring 初始化 bean 的时候，如果bean实现了 InitializingBean 接口，在对象的所有属性被初始化后之后才会调用 `afterPropertiesSet()` 方法
```java
@Component
public class InitialingzingBeanTest implements InitializingBean {

    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("InitializingBean..");
    }
}
```
当然，可以看出Spring初始化bean肯定会在 `ApplicationRunner` 和 `CommandLineRunner` 接口调用之前。<br />当然有一点要注意的是，尽管使用 InitialingBean 接口可以实现初始化动作，但是官方并不建议使用 InitializingBean 接口，因为它将代码耦合在Spring代码中，官方的建议是在 bean 的配置文件指定 init-method 方法，或者在 `@Bean` 中设置 init-method 属性
<a name="XaTKM"></a>
### init-method和`@PostConstruct`
前面就说过官方文档上不建议使用 InitializingBean 接口，但是可以在 `<bean>` 元素的 init-method 属性指定 bean 初始化之后的操作方法，或者在指定方法上加上 `@PostConstruct` 注解来制定该方法在初始化之后调用
```java
@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }

    @PostConstruct
    public void init() {
        System.out.println("init...");
    }
}
```
更多关于Spring Bean的生命周期的内容，请参阅Spring相关书籍或博客Spring Bean的生命周期
<a name="Eds0e"></a>
## 4、Spring的事件机制
Spring 的事件机制实际上是设计模式中观察者模式的典型应用。<br />观察者模式定义了一个一对多的依赖关系，让一个或多个观察者对象监听一个主题对象。这样一来，当被观察者状态改变时，需要通知相应的观察者，使这些观察者能够自动更新<br />基础概念<br />Spring的事件驱动模型由三部分组成

- 事件: `ApplicationEvent`，继承自JDK的`EventObject`，所有事件都要继承它，也就是被观察者
- 事件发布者: `ApplicationEventPublisher`及`ApplicationEventMulticaster`接口，使用这个接口，就可以发布事件了
- 事件监听者: `ApplicationListener`，继承JDK的`EventListener`，所有监听者都继承它，也就是观察者，当然也可以使用注解 `@EventListener`，效果是一样的

事件<br />在Spring框架中，默认对`ApplicationEvent`事件提供了如下支持:

- `ContextStartedEvent`：`ApplicationContext`启动后触发的事件
- `ContextStoppedEvent`：`ApplicationContext`停止后触发的事件
- `ContextRefreshedEvent`：`ApplicationContext`初始化或刷新完成后触发的事件；（容器初始化完成后调用，所以可以利用这个事件做一些初始化操作）
- `ContextClosedEvent`：`ApplicationContext`关闭后触发的事件；（如web容器关闭时自动会触发Spring容器的关闭，如果是普通java应用，需要调用`ctx.registerShutdownHook();`注册虚拟机关闭时的钩子才行）
<a name="PrvMk"></a>
### 构造一个类继承`ApplicationEvent`
```java
public class TestEvent extends ApplicationEvent {

    private String message;

    /**
     * Create a new ApplicationEvent.
     * @param source the object on which the event initially occurred (never {@code null})
     */
    public TestEvent(Object source) {
        super(source);
    }

    public void getMessage() {
        System.out.println(message);
    }

    public void setMessage(String message) {
        this.message = message;
    }

}
```
<a name="e2j86"></a>
#### 创建事件监听者
有两种方法可以创建监听者，一种是直接实现`ApplicationListener`的接口，一种是使用注解 `@EventListener`，注解是添加在监听方法上的，下面的例子是直接实现的接口
```java
@Component

public class ApplicationListenerTest implements ApplicationListener<TestEvent> {

    @Override
    public void onApplicationEvent(TestEvent testEvent){
        testEvent.getMessage();
    }

}
```
<a name="GLXDP"></a>
#### 事件发布
对于事件发布，代表者是`ApplicationEventPublisher`和`ApplicationEventMulticaster`,<br />`ApplicationContext`接口继承了`ApplicationEventPublisher`，并在`AbstractApplicationContext`实现了具体代码，实际执行是委托给`ApplicationEventMulticaster`（可以认为是多播）<br />下面是一个事件发布者的测试实例:
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class EventTest {

    @Autowired
    private ApplicationContext applicationContext;

    @Test
    public void publishTest() {
        TestEvent testEvent = new TestEvent("");
        testEvent.setMessage("hello world");
        applicationContext.publishEvent(testEvent);
    }
}
```
```
//output:
hello world
```
<a name="U606H"></a>
#### 利用`ContextRefreshedEvent`事件进行初始化操作
前面做了这么多铺垫，下面进入今天的主题，利用Spring的事件机制进行初始化一些操作，实际上就是前面提到了，利用`ContextRefreshedEvent`事件进行初始化，该事件是`ApplicationContext`初始化完成后调用的事件，所以可以利用这个事件，对应实现一个监听器，在其`onApplicationEvent()`方法里初始化操作
```java
@Component
public class ApplicationListenerTest implements ApplicationListener<ContextRefreshedEvent> {

    @Override
    public void onApplicationEvent(ContextRefreshedEvent event) {
        System.out.println("我被调用了..");
    }

}
```
注意：在传统的基于XML配置的Spring项目中会存在二次调用的问题，即调用两次该方法，原因是在传统的Spring MVC项目中，系统存在两个容器，一个root容器，一个project-servlet.xml对应的子容器，在初始化这两个容器的时候都会调用该方法一次，所以有二次调用的问题，而对于基于SpringBoot的项目不存在这个问题
<a name="PIlTq"></a>
## 小结
以上简要总结了在SpringBoot启动时进行初始化操作的几个方案，这几种方式都可以满足需求，针对具体场景使用对应的方案。但是，`CommandLineRunner`或者`ApplicationRunner`不是Spring框架原有的东西，它俩属于SpringBoot应用特定的回调扩展接口，所以很容易进行扩展，在一些微服务应用中使用也较广泛。
