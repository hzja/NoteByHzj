Java SpringBoot<br />基于SpringBoot 2.5.0-M2了解Spring中`Lifecycle`和`SmartLifecycle`的作用和区别，以及如何控制`SmartLifecycle`的优先级。<br />并了解SpringBoot中如何通过`SmartLifecycle`来启动/停止web容器.
<a name="R38ta"></a>
### `SmartLifecycle `& `Lifecycle`作用和区别
`SmartLifecycle`和`Lifecycle`作用<br />都是让开发者可以在所有的bean都创建完成(`getBean`) 之后执行自己的初始化工作，或者在退出时执行资源销毁工作。<br />`SmartLifecycle`和`Lifecycle`区别

- `SmartLifecycle`接口继承`Lifecycle`接口，同时继承了`org.springframework.context.Phased`接口用于控制多个`SmartLifecycle`实现之间的优先级。
- 在SpringBoot应用中，或在Spring应用中没有调用`AbstractApplicationContext#start`方法，如果一个Bean只是实现了`Lifecycle`接口的情况下:
   - 不会执行`Lifecycle`接口中的启动方法，包括`Lifecycle#isRunning`方法也不会被执行。
   - 但是在应用 退出时 会执行`Lifecycle#isRunning`方法判断该`Lifecycle`是否已经启动，如果返回true则调用`Lifecycle#stop()`停止方法。
- 如果一个Bean实现了`SmartLifecycle`接口，则会执行启动方法。先会被根据Phased接口优先级分组，封装在`LifecycleGroup`，然后循环调用`LifecycleGroup#start()`方法，`SmartLifecycle#isRunning`判断是否已经执行，返回false表示还未执行，则调用`SmartLifecycle#start()`执行。Phased返回值越小，优先级越高。
- `SmartLifecycle`中还有个`isAutoStartup`方法，如果返回false，在启动时也不会执行`start`方法，默认返回true
<a name="M590t"></a>
### 源码分析
`SmartLifecycle`和`Lifecycle`都是在`org.springframework.context.support.DefaultLifecycleProcessor`中被调用，`DefaultLifecycleProcessor#onRefresh`方法在执行`AbstractApplicationContext#finishRefresh`时会被调用，调用栈如下:
```java
startBeans:142, DefaultLifecycleProcessor (org.springframework.context.support)
onRefresh:123, DefaultLifecycleProcessor (org.springframework.context.support)
finishRefresh:934, AbstractApplicationContext (org.springframework.context.support)
refresh:585, AbstractApplicationContext (org.springframework.context.support)
refresh:144, ServletWebServerApplicationContext (org.springframework.boot.web.servlet.context)
refresh:755, SpringApplication (org.springframework.boot)
refreshContext:426, SpringApplication (org.springframework.boot)
run:326, SpringApplication (org.springframework.boot)
run:1299, SpringApplication (org.springframework.boot)
run:1288, SpringApplication (org.springframework.boot)
main:31, DemoApplication (com.example.demo)
```
`DefaultLifecycleProcessor#onRefresh`源码:
```java
@Override
public void onRefresh() {
    startBeans(true);  //autoStartupOnly = true
    this.running = true;
}
```
`DefaultLifecycleProcessor#startBeans`源码如下:<br />`autoStartupOnly`在`onRefresh`时传入的是true，表示只执行可以自动启动的bean，即为：`SmartLifecycle`的实现类，并且`SmartLifecycle#isAutoStartup`返回值必须为true。
```java
private void startBeans(boolean autoStartupOnly) {
    Map<String, Lifecycle> lifecycleBeans = getLifecycleBeans();
    Map<Integer, LifecycleGroup> phases = new TreeMap<>();

    lifecycleBeans.forEach((beanName, bean) -> {
        if (!autoStartupOnly || (bean instanceof SmartLifecycle && ((SmartLifecycle) bean).isAutoStartup())) {
            int phase = getPhase(bean);
            phases.computeIfAbsent(phase, p -> 
                                   new LifecycleGroup(phase, this.timeoutPerShutdownPhase, lifecycleBeans, autoStartupOnly)
                                  ).add(beanName, bean);
        }
    });
    if (!phases.isEmpty()) {
        phases.values().forEach(LifecycleGroup::start);
    }
}
```
而Spring `AbstractApplicationContext#doClose`退出时，无论是`SmartLifecycle`或`Lifecycle`都会执行`isRunning`方法，判断是否已经启动，返回true表示已经启动，则执行`SmartLifecycle`或`Lifecycle`的`stop`方法。源码见：`org.springframework.context.support.DefaultLifecycleProcessor#doStop`方法。<br />而执行`AbstractApplicationContext#doClose`一般是应用进程退出，通过jvm注册的钩子方法，或者应用程序编码调用。<br />`AbstractApplicationContext#registerShutdownHook`源码
```java
@Override
public void registerShutdownHook() {
    if (this.shutdownHook == null) {
        // No shutdown hook registered yet.
        this.shutdownHook = new Thread(SHUTDOWN_HOOK_THREAD_NAME) {
            @Override
            public void run() {
                synchronized (startupShutdownMonitor) {
                    doClose();
                }
            }
        };
        Runtime.getRuntime().addShutdownHook(this.shutdownHook);
    }
}
```
<a name="q48ky"></a>
### 自定义`LifecycleProcessor`处理`Lifecycle`
在源码分析中提到了`DefaultLifecycleProcessor`，其实现了`LifecycleProcessor`接口。然而自己也可以实现该接口，替换默认的`DefaultLifecycleProcessor`。SpringBoot中则是自己配置了`DefaultLifecycleProcessor`，可以按照同样的方式，覆盖默认的实现。例如可以让`Lifecycle`中的`start()`方法在`onRefresh()`时也能被执行。<br />`org.springframework.boot.autoconfigure.context.LifecycleAutoConfiguration`源码:
```java
/**
 * {@link EnableAutoConfiguration Auto-configuration} relating to the application
 * context's lifecycle.
 *
 * @author Andy Wilkinson
 * @since 2.3.0
 */
@Configuration(proxyBeanMethods = false)
@EnableConfigurationProperties(LifecycleProperties.class)
public class LifecycleAutoConfiguration {

    @Bean(name = AbstractApplicationContext.LIFECYCLE_PROCESSOR_BEAN_NAME)
    @ConditionalOnMissingBean(name = AbstractApplicationContext.LIFECYCLE_PROCESSOR_BEAN_NAME,
                              search = SearchStrategy.CURRENT)
    public DefaultLifecycleProcessor defaultLifecycleProcessor(LifecycleProperties properties) {
        DefaultLifecycleProcessor lifecycleProcessor = new DefaultLifecycleProcessor();
        lifecycleProcessor.setTimeoutPerShutdownPhase(properties.getTimeoutPerShutdownPhase().toMillis());
        return lifecycleProcessor;
    }
}
```
<a name="NnTDK"></a>
### SpringBoot中内嵌web容器启动时机
SpringBoot中就是通过实现`SmartLifecycle`来启动内嵌的web容器，实现类为`WebServerStartStopLifecycle`。<br />`ServletWebServerApplicationContext`在`onRefresh`方法中调用`createWebServer`，`createWebServer`方法中创建`org.springframework.boot.web.server.WebServer`实例，该对象则包含了控制web容器(tomcat、jetty)的启动与停止方法。
```java
@Override
protected void onRefresh() {
    super.onRefresh();
    try {
        createWebServer();
    }catch (Throwable ex) {
        throw new ApplicationContextException("Unable to start web server", ex);
    }
}
```
`ServletWebServerApplicationContext#createWebServer`源码:
```java
private void createWebServer() {
    WebServer webServer = this.webServer;
    ServletContext servletContext = getServletContext();
    if (webServer == null && servletContext == null) {
        StartupStep createWebServer = this.getApplicationStartup().start("spring.boot.webserver.create");
        ServletWebServerFactory factory = getWebServerFactory();
        createWebServer.tag("factory", factory.getClass().toString());
        this.webServer = factory.getWebServer(getSelfInitializer());
        createWebServer.end();
        getBeanFactory().registerSingleton("webServerGracefulShutdown",
                                           new WebServerGracefulShutdownLifecycle(this.webServer));
        getBeanFactory().registerSingleton("webServerStartStop",
                                           new WebServerStartStopLifecycle(this, this.webServer));
    }
    else if (servletContext != null) {
        try {
            getSelfInitializer().onStartup(servletContext);
        }
        catch (ServletException ex) {
            throw new ApplicationContextException("Cannot initialize servlet context", ex);
        }
    }
    initPropertySources();
}
```
`createWebServer`方法会将创建的webServer封装在`WebServerStartStopLifecycle`对象中，并注册到Spring容器中。<br />`org.springframework.boot.web.servlet.context.WebServerStartStopLifecycle`源码如下:
```java
class WebServerStartStopLifecycle implements SmartLifecycle {

  private final ServletWebServerApplicationContext applicationContext;
  private final WebServer webServer;
  private volatile boolean running;

  WebServerStartStopLifecycle(ServletWebServerApplicationContext applicationContext, WebServer webServer) {
    this.applicationContext = applicationContext;
    this.webServer = webServer;
  }

  @Override
  public void start() {
    this.webServer.start();
    this.running = true;
    this.applicationContext
      .publishEvent(new ServletWebServerInitializedEvent(this.webServer, this.applicationContext));
  }

  @Override
  public void stop() {  this.webServer.stop();   }

  @Override
  public boolean isRunning() { return this.running;  }

  @Override
  public int getPhase() {    return Integer.MAX_VALUE - 1;  }
}
```
`WebServerStartStopLifecycle`则实现了`SmartLifecycle`接口，当Spring回调到`SmartLifecycle`接口方法时则调用`this.webServer.start();`启动web容器，web容器启动完成之后会通过applicationContext发布`ServletWebServerInitializedEvent`事件，表示web容器启动成功，可以接收http请求。
<a name="jlTVN"></a>
### 和`SmartInitializingSingleton`区别
相同点：`SmartInitializingSingleton`和`Lifecycle`、`SmartLifecycle`都是在所有的单实例bean创建(`getBean`方法)之后执行。<br />不同点：

- `SmartInitializingSingleton`优先于`Lifecycle`、`SmartLifecycle`执行。
- `SmartInitializingSingleton`只有一个`afterSingletonsInstantiated`方法。而`Lifecycle`有`start`,`stop`,`isRunning`等方法。
- 多个`SmartInitializingSingleton`实现之间无法排序控制执行的顺序，而`SmartLifecycle`实现了Phased接口，可以通过`int getPhase()`控制执行循序。
- `SmartInitializingSingleton`之间可以通过`@DependsOn`来控制执行顺序，但这是由Spring中`@DependsOn`注解的作用及原理来实现的，并不是对`SmartInitializingSingleton`做了排序。
