JavaSpringBoot
<a name="xkjeV"></a>
## **前言**
从 `SpringApplication.run(EasyPaasAdminApplication.class, args);`入口，逐渐向下看下执行流程。
<a name="P843E"></a>
## 1、执行逻辑梳理
一般SpringBoot 应用的启动入口都是如下这种固定的写法<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1694147708818-19389cda-f72c-42fc-96f4-3ed5167f4767.png#averageHue=%23fdfcfc&clientId=u09245b17-4fa3-4&from=paste&height=230&id=QC4Sf&originHeight=575&originWidth=2137&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=99875&status=done&style=none&taskId=ubd1aabe7-9ec6-4399-8d85-1dac8ea9427&title=&width=854.8)<br />也可以是这样
```java
public static void main(String[] args) {
    SpringApplication application = new SpringApplication(MyApplication.class);
    // ... customize application settings here
    application.run(args)
}
```
但总之，都是使用SpringApplication 调用静态方法<br />此方法的注释<br />Static helper that can be used to run a SpringApplication from the specified source using default settings.
```java
public static ConfigurableApplicationContext run(Class<?> primarySource, String... args) {
    return run(new Class<?>[] { primarySource }, args);
}
```
跟过来就到这，可以看到注释运行Spring应用程序，创建并刷新一个新的`ApplicationContext`。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163713346-adff0948-3a1a-4b01-aaae-258a38c1a240.png#averageHue=%232e2d2c&clientId=u6e528e3c-a44e-4&from=paste&id=u8cc75147&originHeight=109&originWidth=679&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u59695b79-c75b-40bd-9d20-c79b10a396e&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163713342-4269d386-66e3-4dae-ad42-bc075836189e.png#averageHue=%232d2c2c&clientId=u6e528e3c-a44e-4&from=paste&id=u99e20859&originHeight=860&originWidth=729&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubc89bac7-4f8f-46e0-ac4e-dfbb30eb649&title=)<br />跟代码到这儿其实对于SpringBoot 的基本启动流程已经知道了。但是要解答什么时候启动的Tomcat 还需要继续分析。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163728709-3a15e8ec-e72d-48d3-b96e-a4be4adfcbcb.png#averageHue=%23565f4e&clientId=u6e528e3c-a44e-4&from=paste&id=u0f3bc7ec&originHeight=162&originWidth=731&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1e5b039e-17e3-48d7-a99b-f49c744ed13&title=)<br />到这儿就可以继续下去，发现Spring Boot 启动WebServer。此处的WebServer就不展开了，可以点击去就三个方法start ，stop，getPort。可以看出来Spring 在设计接口的时候还是很严谨和精简。<br />核心脉络是梳理SpringBoot 启动过程，并且回答Tomcat 是如何被启动的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163738204-206d25ec-9b40-416f-b192-3ca2c080dce9.png#averageHue=%23393632&clientId=u6e528e3c-a44e-4&from=paste&id=u6b126f79&originHeight=788&originWidth=1466&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec65a28e-36e7-4181-b946-e5106c52fb8&title=)<br />可以看到WebServer 的实现目前内置的有5种。其实Spring Boot 还有一个特性叫做 自动装配。<br />这就是为什么5个实现，最后启动的是Tomcat。此处也不做展开。后面我专门搞一个解析SpringBoot 自动装配的文章。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163747565-7b38feb4-f3b1-4c0e-ba37-3086cd6a6107.png#averageHue=%23575043&clientId=u6e528e3c-a44e-4&from=paste&id=ubb8895ad&originHeight=321&originWidth=632&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2b4acd14-76a9-493e-b99a-4f4f05ad4e8&title=)<br />看一下内部start 的TomcatWebServer的内部实现。了解过Tomcat 源码的同学看到这儿就基本明白了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163756513-98decdd8-da97-4c41-85c6-c1bfdbcc973a.png#averageHue=%233b3733&clientId=u6e528e3c-a44e-4&from=paste&id=ua7a5fb39&originHeight=778&originWidth=1455&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5f3d1877-a2a7-4c9e-b508-d0308105cb2&title=)<br />好源码跟进过程到此结束，整理和总结一下。<br />通过扫一遍源码大概可以总结出来如下三个阶段<br />准备阶段、应用上下文创建阶段、刷新上下文阶段。

1. **准备阶段**：Spring Boot 会加载应用程序的初始设置，并创建 Spring Boot 上下文。这个阶段的核心源码是 SpringApplication 类的 `run()` 方法，它会调用 Spring Boot 的各个初始化器进行初始化和准备工作。
2. **应用上下文创建阶段** : Spring Boot 会创建应用程序的上下文，包括各种配置信息、Bean 的加载和初始化等。这个阶段的核心源码是 Spring Boot 自动配置机制，通过扫描 classpath 中的配置文件，自动加载和配置各种组件和 Bean。
3. **刷新上下文阶段**：Spring Boot 会执行各种启动任务，包括创建 Web 服务器、加载应用程序的配置、初始化各种组件等。这个阶段的核心源码是 Spring Boot 的刷新机制，它会调用各种初始化器和监听器，执行各种启动任务。其中启动Tomcat 就是在这个环节进行。
<a name="YnUPt"></a>
## **2、核心源码解析**
既然上面已经基本上总结除了，Spring Boot的启动脉络。也梳理出了一些核心源码。那么对启动过程的核心源码解析一下。
<a name="ML9IN"></a>
### 2.1. 准备阶段
在准备阶段中，Spring Boot 会加载应用程序的初始设置，并创建 Spring Boot 上下文。这个阶段的核心源码是 SpringApplication 类的 `run()` 方法，它会调用 Spring Boot 的各个初始化器进行初始化和准备工作。
```java
public ConfigurableApplicationContext run(String... args) {
    // 启动计时器
    StopWatch stopWatch = new StopWatch();
    stopWatch.start();

    // 定义应用程序上下文和异常报告器列表
    ConfigurableApplicationContext context = null;
    Collection<SpringBootExceptionReporter> exceptionReporters = new ArrayList<>();

    // 配置 Headless 属性
    configureHeadlessProperty();

    // 获取 Spring Boot 启动监听器
    SpringApplicationRunListeners listeners = getRunListeners(args);
    // 执行启动监听器的 starting 方法
    listeners.starting();

    try {
        // 解析命令行参数
        ApplicationArguments applicationArguments = new DefaultApplicationArguments(args);
        // 准备应用程序环境
        ConfigurableEnvironment environment = prepareEnvironment(listeners, applicationArguments);
        // 配置忽略 BeanInfo
        configureIgnoreBeanInfo(environment);
        // 打印 Banner
        Banner printedBanner = printBanner(environment);
        // 创建应用程序上下文
        context = createApplicationContext();
        // 获取异常报告器,关于异常报告，我下次专门讲一下SpringBoot 的异常收集器。
        exceptionReporters = getSpringFactoriesInstances(SpringBootExceptionReporter.class, new Class[]{ConfigurableApplicationContext.class}, context);
        // 准备应用程序上下文
        prepareContext(context, environment, listeners, applicationArguments, printedBanner);
        // 刷新应用程序上下文
        refreshContext(context);
        // 刷新后操作
        afterRefresh(context, applicationArguments);
        // 停止计时器
        stopWatch.stop();
        // 记录启动日志
        if (this.logStartupInfo) {
            new StartupInfoLogger(this.mainApplicationClass).logStarted(getApplicationLog(), stopWatch);
        }
        // 执行启动监听器的 started 方法
        listeners.started(context);
        // 执行 Runner
        callRunners(context, applicationArguments);
    } catch (Throwable ex) {
        // 处理启动失败
        handleRunFailure(context, ex, exceptionReporters, listeners);
        throw new IllegalStateException(ex);
    }

    try {
        // 执行启动监听器的 running 方法
        listeners.running(context);
    } catch (Throwable ex) {
        // 处理启动失败
        handleRunFailure(context, ex, exceptionReporters, null);
        throw new IllegalStateException(ex);
    }

    // 返回应用程序上下文
    return context;
}
```
在 `run()` 方法中，Spring Boot 首先会创建一个 StopWatch 对象，用于记录整个启动过程的耗时。然后，Spring Boot 会调用 `getRunListeners(args)` 方法获取 Spring Boot 的各个启动监听器，并调用`starting()` 方法通知这些监听器启动过程已经开始。接着调用 `prepareEnvironment(listeners, applicationArguments)` 方法创建应用程序的环境变量。<br />这个方法会根据用户的配置和默认设置创建一个 `ConfigurableEnvironment`对象，并将其传给后面的 `createApplicationContext()` 方法。`printBanner(environment)` 方法打印启动界面的 Banner，调用 `refreshContext(context)`方法刷新上下文。这个方法会启动上下文，执行各种启动任务，包括创建 Web 服务器、加载应用程序的配置、初始化各种组件等。具体的启动任务会在刷新上下文阶段中进行。
<a name="pe7lq"></a>
### 2.2. 应用上下文创建阶段
在应用上下文创建阶段中，Spring Boot 会创建应用程序的上下文，包括各种配置信息、Bean 的加载和初始化等。这个阶段的核心源码是 Spring Boot 自动配置机制，通过扫描 classpath 中的配置文件，自动加载和配置各种组件和 Bean。
```java
protected ConfigurableApplicationContext createApplicationContext() {
    Class<?> contextClass = this.applicationContextClass;
    if (contextClass == null) {
        try {
            switch (this.webApplicationType) {
                case SERVLET:
                    contextClass = Class.forName(DEFAULT_SERVLET_WEB_CONTEXT_CLASS);
                    break;
                case REACTIVE:
                    contextClass = Class.forName(DEFAULT_REACTIVE_WEB_CONTEXT_CLASS);
                    break;
                default:
                    contextClass = Class.forName(DEFAULT_CONTEXT_CLASS);
            }
        }
        catch (ClassNotFoundException ex) {
            throw new IllegalStateException(
                "Unable to create a default ApplicationContext, " +
                "please specify an ApplicationContextClass", ex);
        }
    }
    return (ConfigurableApplicationContext) BeanUtils.instantiateClass(contextClass);
}
```
在 `createApplicationContext()` 方法中，Spring Boot 首先会判断应用程序的类型，如果是 Web 应用程序，则会创建一个 `WebApplicationContext`；否则，会创建一个普通的 `ApplicationContext`。调用 `BeanUtils.instantiateClass(contextClass)` 方法创建应用程序的上下文。这个方法会根据上面的逻辑创建一个相应的 `ApplicationContext`。调用 `load()` 方法加载应用程序的配置。<br />这个方法会扫描 classpath 中的各种配置文件，例如 application.properties、application.yml、META-INF/spring.factories 等，自动配置各种组件和 Bean。调用 `postProcessApplicationContext()` 方法对应用程序的上下文进行后处理。这个方法会调用各种初始化器和监听器，执行各种初始化任务。
<a name="qbj0X"></a>
### 2.3. 刷新上下文阶段
在刷新上下文阶段中，Spring Boot 会执行各种启动任务，包括创建 Web 服务器（刚才跟源码的时候也看到了，如上我的截图）、加载应用程序的配置、初始化各种组件等。这个阶段的核心源码是 Spring Boot 的刷新机制，它会调用各种初始化器和监听器，执行各种启动任务。
```java
protected void refreshContext(ConfigurableApplicationContext applicationContext) {
    refresh(applicationContext);
    if (this.registerShutdownHook) {
        try {
            applicationContext.registerShutdownHook();
        }
        catch (AccessControlException ex) {
            // Not allowed in some environments.
        }
    }
}
```
在 `refreshContext()` 方法中调用 `refresh(applicationContext)` 方法刷新上下文。这个方法是 `ApplicationContext` 接口的核心方法，会启动上下文，执行各种启动任务。调用 `registerShutdownHook() `方法注册应用程序的关闭钩子。这个方法会在应用程序关闭时自动执行，清理资源、关闭线程等,所以利用此特性在服务关闭的时候清理一些资源。并向外部发送告警通知。<br />在 `refresh(applicationContext)` 方法中，Spring Boot 会执行上下文的各种启动任务，包括创建 Web 服务器、加载应用程序的配置、初始化各种组件等。具体的启动任务会调用各种初始化器和监听器，例如：
```java
for (ApplicationContextInitializer<?> initializer : getInitializers()) {
    initializer.initialize(applicationContext);
}
```
另外，Spring Boot 还会调用各种监听器，不做赘述，例如：
```java
for (ApplicationListener<?> listener : getApplicationListeners()) {
    if (listener instanceof SmartApplicationListener) {
        SmartApplicationListener smartListener = (SmartApplicationListener) listener;
        if (smartListener.supportsEventType(eventType)
            && smartListener.supportsSourceType(sourceType)) {
            invokeListener(smartListener, event);
        }
    }
    else if (supportsEvent(listener, eventType)) {
        invokeListener(listener, event);
    }
}
```
基本上就是这些了。<br />关于`SpringApplication`的官方文档讲的比较简单，大家可供参考。地址如下：

- [https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.spring-application](https://docs.spring.io/spring-boot/docs/current/reference/html/features.html#features.spring-application)

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163774776-de3c80cf-b02d-42cd-a66c-b1a990adfc70.png#averageHue=%23fafdf5&clientId=u6e528e3c-a44e-4&from=paste&id=ub91e36f3&originHeight=762&originWidth=1576&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5dcc1a2-12ee-4db1-8a3d-3b3e92afcc1&title=)
