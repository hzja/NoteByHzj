Java SpringBoot<br />事件及监听并不是 SpringBoot 的新功能，Spring 框架早已提供了完善的事件监听机制，在 Spring 框架中实现事件监听的流程如下：

1. 自定义事件，继承 `org.springframework.context.ApplicationEvent` 抽象类
2. 定义事件监听器，实现 `org.springframework.context.ApplicationListener` 接口
3. 在 Spring 容器中发布事件
<a name="oCdtd"></a>
## 实现自定义事件及监听
<a name="6568bcba"></a>
### 定义事件
```java
//自定义事件
public class ApplicationEventTest extends ApplicationEvent {

    public ApplicationEventTest(Object source) {
        super(source);
    }

    /**
     * 事件处理事项
     * @param msg
     */
    public void printMsg(String msg)
    {
        System.out.println("监听到事件："+ApplicationEventTest.class);
    }
}
```
<a name="4c15a849"></a>
### 定义监听器
```java
//自定义事件监听器
//@Component
public class ApplicationListenerTest implements ApplicationListener<ApplicationEventTest> {

    @Override
    public void onApplicationEvent(ApplicationEventTest event) {

        event.printMsg(null);
    }
}
```
<a name="7d55679e"></a>
### 在Spring容器中发布事件
```java
public static void main(String[] args) {

   SpringApplication application = new SpringApplication(SpringbootdemoApplication.class);
   //需要把监听器加入到spring容器中
   application.addListeners(new ApplicationListenerTest());
   Set<ApplicationListener<?>> listeners = application.getListeners();
   ConfigurableApplicationContext context =  application.run(args);
   //发布事件
   context.publishEvent(new ApplicationEventTest(new Object()));

   context.close();
}
```
上面的示例是在 SpringBoot 应用中简单的测试一下。<br />实际开发中实现监听还有其他的方式，在 Spring 框架中提供了两种事件监听的方式：

1. 编程式：通过实现 `ApplicationListener` 接口来监听指定类型的事件
2. 注解式：通过在方法上加 `@EventListener` 注解的方式监听指定参数类型的事件，写该类需要托管到 Spring 容器中

在 SpringBoot 应用中还可以通过配置的方式实现监听：<br />3. 通过 `application.properties` 中配置 `context.listener.classes` 属性指定监听器<br />下面分别分析一下这三种监听方式
<a name="DmK4M"></a>
## 编程式实现监听
实现 `ApplicationListenser` 接口：
```java
@Component
public class ApplicationListenerTest implements ApplicationListener<ApplicationEventTest> {

    @Override
    public void onApplicationEvent(ApplicationEventTest event) {

        event.printMsg(null);
    }
}
```
控制台输出测试：
```java
public static void main(String[] args) {

   SpringApplication application = new SpringApplication(SpringbootdemoApplication.class);
   //需要把监听器加入到spring容器中
   //application.addListeners(new ApplicationListenerTest());
   //Set<ApplicationListener<?>> listeners = application.getListeners();

   ConfigurableApplicationContext context =  application.run(args);
   //发布事件
   context.publishEvent(new ApplicationEventTest(new Object()));
}
```
那么跟踪一下源码，看一下事件是如何发布出去的，又是如何被监听到的呢？<br />`AbstractApplicationContext.java` 中截取部分代码
```java
protected void publishEvent(Object event, @Nullable ResolvableType eventType) {
    Assert.notNull(event, "Event must not be null");
    if (logger.isTraceEnabled()) {
        logger.trace("Publishing event in " + getDisplayName() + ": " + event);
    }

    // Decorate event as an ApplicationEvent if necessary
    /将object转成ApplicationEvent
        ApplicationEvent applicationEvent;
    if (event instanceof ApplicationEvent) {
        applicationEvent = (ApplicationEvent) event;
    }
    else {
        applicationEvent = new PayloadApplicationEvent<>(this, event);
        if (eventType == null) {
            eventType = ((PayloadApplicationEvent) applicationEvent).getResolvableType();
        }
    }

    // Multicast right now if possible - or lazily once the multicaster is initialized
    if (this.earlyApplicationEvents != null) {
        this.earlyApplicationEvents.add(applicationEvent);
    }
    else {
        // SimpleApplicationEventMulticaster 获取事件发布器，发布事件
        getApplicationEventMulticaster().multicastEvent(applicationEvent, eventType);
    }

    // Publish event via parent context as well...
    if (this.parent != null) {
        if (this.parent instanceof AbstractApplicationContext) {
            ((AbstractApplicationContext) this.parent).publishEvent(event, eventType);
        }
        else {
            this.parent.publishEvent(event);
        }
    }
}
```
查看一下 `ApplicationContext` 类结构图可以发现：应用上下文 `AbstractApplicationContext` 实际还是通过继承 `ApplicationEventPublisher` 接口，实现了其中的事件发布的方法，使得 Spring 应用上下文有了发布事件的功能，在 `AbstractApplicationContext` 内部通过 `SimpleApplicationEventMulticaster` 事件发布类，将具体事件 `ApplicationEvent` 发布出去。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624353828066-6643d669-d1ac-4e2b-85fc-e462f3dac833.png#align=left&display=inline&height=332&id=u36e29352&originHeight=332&originWidth=1080&status=done&style=none&width=1080)<br />那么事件发布出去后又是如何被监听到的呢？下面看一下具 Spring 中负责处理事件发布类 `SimpleApplicationEventMulticaster` 中 `multicastEvent` 方法具体实现过程<br />`SimpleApplicationEventMulticaster.java` 部分代码，实际尝试将当前事件逐个广播到指定类型的监听器中（listeners 已经根据当前事件类型过滤了）
```java
@Override
public void multicastEvent(final ApplicationEvent event, @Nullable ResolvableType eventType) {
   ResolvableType type = (eventType != null ? eventType : resolveDefaultEventType(event));
    // getApplicationListeners(event, type) 筛选监听器，在context.publish(ApplicationEvent event)中已经将事件传入，getApplicationListeners中将可以根据这个event类型从Spring容器中检索出符合条件的监听器

   for (final ApplicationListener<?> listener : getApplicationListeners(event, type)) {
      Executor executor = getTaskExecutor();
      if (executor != null) {
         executor.execute(() -> invokeListener(listener, event));
      }
      else {
    //尝试逐个向监听器广播
         invokeListener(listener, event);
      }
   }
}
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624353829032-5f95a8fd-f67e-4c61-b85e-a672c208076e.png#align=left&display=inline&height=263&id=ua7c04608&originHeight=263&originWidth=1080&status=done&style=none&width=1080)
<a name="dNSwb"></a>
## `@EventListener` 注解方式实现
定义注解方法
```java
@Component
public class MyEventHandleTest {

    /**
     * 参数为Object类型时，所有事件都会监听到
     * 参数为指定类型事件时，该参数类型事件或者其子事件（子类）都可以接收到
     */
    @EventListener
    public void event(ApplicationEventTest event){

        event.printMsg(null);
    }

}
```
实现过程分析：<br />`@EventListener` 注解主要通过 `EventListenerMethodProcessor` 扫描出所有带有 `@EventListener` 注解的方法，然后动态构造事件监听器，并将监听器托管到 Spring 应用上文中。
```java
protected void processBean(
      final List<EventListenerFactory> factories, final String beanName, final Class<?> targetType) {

   if (!this.nonAnnotatedClasses.contains(targetType)) {
      Map<Method, EventListener> annotatedMethods = null;
      try {
        //查找含有@EventListener注解的所有方法
         annotatedMethods = MethodIntrospector.selectMethods(targetType,
               (MethodIntrospector.MetadataLookup<EventListener>) method ->
                     AnnotatedElementUtils.findMergedAnnotation(method, EventListener.class));
      }
      catch (Throwable ex) {
         // An unresolvable type in a method signature, probably from a lazy bean - let's ignore it.
         if (logger.isDebugEnabled()) {
            logger.debug("Could not resolve methods for bean with name '" + beanName + "'", ex);
         }
      }
      if (CollectionUtils.isEmpty(annotatedMethods)) {
         this.nonAnnotatedClasses.add(targetType);
         if (logger.isTraceEnabled()) {
            logger.trace("No @EventListener annotations found on bean class: " + targetType.getName());
         }
      }
      else {
         // Non-empty set of methods
         ConfigurableApplicationContext context = getApplicationContext();
    //遍历含有@EventListener注解的方法
         for (Method method : annotatedMethods.keySet()) {
            for (EventListenerFactory factory : factories) {
               if (factory.supportsMethod(method)) {
                  Method methodToUse = AopUtils.selectInvocableMethod(method, context.getType(beanName));
    　　　　　　//动态构造相对应的事件监听器
                  ApplicationListener<?> applicationListener =
                        factory.createApplicationListener(beanName, targetType, methodToUse);
                  if (applicationListener instanceof ApplicationListenerMethodAdapter) {
                     ((ApplicationListenerMethodAdapter) applicationListener).init(context, this.evaluator);
                  }
    　　　　　　//将监听器添加的Spring应用上下文中托管
                  context.addApplicationListener(applicationListener);
                  break;
               }
            }
         }
         if (logger.isDebugEnabled()) {
            logger.debug(annotatedMethods.size() + " @EventListener methods processed on bean '" +
                  beanName + "': " + annotatedMethods);
         }
      }
   }
}
```
<a name="jEJ7q"></a>
## 在 application.properties 中配置 context.listener.classes
添加如下配置：
```java
context.listener.classes=com.sl.springbootdemo.Listeners.ApplicationListenerTest
```
查看一下 DelegatingApplicationListener 类中实现逻辑：
```java
public class DelegatingApplicationListener
    implements ApplicationListener<ApplicationEvent>, Ordered {

    private static final String PROPERTY_NAME = "context.listener.classes";

    private int order = 0;
    //Spring framework提供的负责处理发布事件的类，前面说的Spring应用上下文中也是通过这个类发布事件的
    private SimpleApplicationEventMulticaster multicaster;

    @Override
    public void onApplicationEvent(ApplicationEvent event) {
        if (event instanceof ApplicationEnvironmentPreparedEvent) {
            // getListeners内部实现读取context.listener.classes配置的监听器
            List<ApplicationListener<ApplicationEvent>> delegates = getListeners(
                ((ApplicationEnvironmentPreparedEvent) event).getEnvironment());
            if (delegates.isEmpty()) {
                return;
            }
            this.multicaster = new SimpleApplicationEventMulticaster();
            for (ApplicationListener<ApplicationEvent> listener : delegates) {
                this.multicaster.addApplicationListener(listener);
            }
        }
        //发布事件
        if (this.multicaster != null) {
            this.multicaster.multicastEvent(event);
        }
    }
}
```
`Spring-boot-{version}.jar` 包中提供一个类 `DelegatingApplicationListener`，该类的作用是从 `application.properties` 中读取配置 `context.listener.classes`，并将事件广播给这些配置的监听器。通过前面一章对 SpringBoot 启动流程分析，已经了解到 SpringBoot 启动时会从 `META-INF/spring.factories` 中读取 key 为 `org.springframework.context.ApplicationListener` 的所有监听器。`DelegatingApplicationListener` 的功能可以让不需要创建 `META-INF/spring.factories`，直接在 `application.properties` 中配置即可。
