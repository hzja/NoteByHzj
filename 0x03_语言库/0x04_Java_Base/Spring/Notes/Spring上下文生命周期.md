JavaSpring
<a name="GGSii"></a>
## 1、什么是Spring应用上下文？
接口`org.springframework.context.ApplicationContext`表示Spring上下文，下面2个实现类
```
org.springframework.context.support.ClassPathXmlApplicationContext
org.springframework.context.annotation.AnnotationConfigApplicationContext
```
<a name="weHNJ"></a>
## 2、应用上文生命周期(14个阶段)
1、创建Spring应用上下文<br />2、上下文启动准备阶段<br />3、`BeanFactory`创建阶段<br />4、`BeanFactory`准备阶段<br />5、`BeanFactory`后置处理阶段<br />6、`BeanFactory`注册`BeanPostProcessor`阶段<br />7、初始化内建Bean：`MessageSource`<br />8、初始化内建Bean：Spring事件广播器<br />9、Spring应用上下文刷新阶段<br />10、Spring事件监听器注册阶段<br />11、单例bean实例化阶段<br />12、`BeanFactory`初始化完成阶段<br />13、Spring应用上下文启动完成阶段<br />14、Spring应用上下文关闭阶段
<a name="uJ7ah"></a>
## 3、Spring应用上下文的使用
看下这段代码，是不是很熟悉，这就是Spring上下文最常见的用法，稍后以这段代码为例，结合Spring源码，来细说每个阶段的细节。
```java
@Configuration
@ComponentScan
public class MainConfig {
    public static void main(String[] args) {
        //1.创建spring上下文
        AnnotationConfigApplicationContext configApplicationContext = new AnnotationConfigApplicationContext();
        //2.上下文中注册bean
        configApplicationContext.register(MainConfig.class);
        //3.刷新spring上下文，内部会启动spring上下文
        configApplicationContext.refresh();
        //4.关闭spring上下文
        System.out.println("stop ok!");
    }
}
```
<a name="QZDBq"></a>
## 4、阶段1：创建Spring应用上下文
对应这段代码
```java
AnnotationConfigApplicationContext configApplicationContext = new AnnotationConfigApplicationContext();
```
看一下其类图，这里主要列出了`AnnotationConfigApplicationContext` 2个父类<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683103292300-30e1cc4f-01b3-447a-8bd6-6d5e1f4f12d5.png#averageHue=%23fbfaf3&clientId=u969703ae-8345-4&from=paste&id=uc8597a2e&originHeight=294&originWidth=504&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec8f42c7-5e86-4339-974d-33fc1380eda&title=)<br />当调用子类的构造器的时候，默认会自动先调用父类的构造器，先来看一下`GenericApplicationContext`构造器源码，如下，将`beanFactory`创建好了。
```java
public GenericApplicationContext() {
    this.beanFactory = new DefaultListableBeanFactory();
}
```
再来看看`AnnotationConfigApplicationContext`构造器源码，如下：
```java
public AnnotationConfigApplicationContext() {
    //创建AnnotatedBeanDefinitionReader：用来读取及注册通过注解方式定义的bean
    this.reader = new AnnotatedBeanDefinitionReader(this); //@1
    //创建ClassPathBeanDefinitionScanner：bean定义扫描器，可以扫描包中的类，对满足条件的类，会将其注册到spring容器中
    this.scanner = new ClassPathBeanDefinitionScanner(this);
}
```
@1：`new AnnotatedBeanDefinitionReader(this)`，进入这个方法内部，最终会走到下面这个方法，非常关键的一个方法，会向Spring容器中注册5个关键的bean，这几个bean都是非常很重要的。
```java
// org.springframework.context.annotation.AnnotationConfigUtils#registerAnnotationConfigProcessors(org.springframework.beans.factory.support.BeanDefinitionRegistry, java.lang.Object)

public static Set<BeanDefinitionHolder> registerAnnotationConfigProcessors(
    BeanDefinitionRegistry registry, @Nullable Object source) {

    //1、注册ConfigurationClassPostProcessor，这是个非常关键的类，实现了BeanDefinitionRegistryPostProcessor接口
    // ConfigurationClassPostProcessor这个类主要做的事情：负责所有bean的注册,如果想看bean注册源码的，可以在其postProcessBeanDefinitionRegistry方法中设置断点
    if (!registry.containsBeanDefinition(CONFIGURATION_ANNOTATION_PROCESSOR_BEAN_NAME)) {
        RootBeanDefinition def = new RootBeanDefinition(ConfigurationClassPostProcessor.class);
        def.setSource(source);
        beanDefs.add(registerPostProcessor(registry, def, CONFIGURATION_ANNOTATION_PROCESSOR_BEAN_NAME));
    }

    //2、注册AutowiredAnnotationBeanPostProcessor：负责处理@Autowire注解
    if (!registry.containsBeanDefinition(AUTOWIRED_ANNOTATION_PROCESSOR_BEAN_NAME)) {
        RootBeanDefinition def = new RootBeanDefinition(AutowiredAnnotationBeanPostProcessor.class);
        def.setSource(source);
        beanDefs.add(registerPostProcessor(registry, def, AUTOWIRED_ANNOTATION_PROCESSOR_BEAN_NAME));
    }

    //3、注册CommonAnnotationBeanPostProcessor：负责处理@Resource注解
    if (jsr250Present && !registry.containsBeanDefinition(COMMON_ANNOTATION_PROCESSOR_BEAN_NAME)) {
        RootBeanDefinition def = new RootBeanDefinition(CommonAnnotationBeanPostProcessor.class);
        def.setSource(source);
        beanDefs.add(registerPostProcessor(registry, def, COMMON_ANNOTATION_PROCESSOR_BEAN_NAME));
    }

    //4、注册EventListenerMethodProcessor：负责处理@EventListener标注的方法，即事件处理器
    if (!registry.containsBeanDefinition(EVENT_LISTENER_PROCESSOR_BEAN_NAME)) {
        RootBeanDefinition def = new RootBeanDefinition(EventListenerMethodProcessor.class);
        def.setSource(source);
        beanDefs.add(registerPostProcessor(registry, def, EVENT_LISTENER_PROCESSOR_BEAN_NAME));
    }

    //5、注册DefaultEventListenerFactory：负责将@EventListener标注的方法包装为ApplicationListener对象
    if (!registry.containsBeanDefinition(EVENT_LISTENER_FACTORY_BEAN_NAME)) {
        RootBeanDefinition def = new RootBeanDefinition(DefaultEventListenerFactory.class);
        def.setSource(source);
        beanDefs.add(registerPostProcessor(registry, def, EVENT_LISTENER_FACTORY_BEAN_NAME));
    }

    return beanDefs;
}
```
再来捋一下上面这段代码，主要向Spring容器中注册了5个关键的bean<br />**1、**`**ConfigurationClassPostProcessor**`：这是个非常关键的类，建议去看一下他的源码，基本上自定义的bean都是通过这个类注册的，下面这些注解都是在这个类中处理的
```
@Configuration
@Component
@PropertySource
@PropertySources
@ComponentScan
@ComponentScans
@Import
@ImportResource
@Bean
```
**2、**`**AutowiredAnnotationBeanPostProcessor**`：负责处理`@Autowire`注解<br />**3、注册**`**CommonAnnotationBeanPostProcessor**`：负责处理`@Resource`注解<br />**4、注册**`**EventListenerMethodProcessor**`：负责处理`@EventListener`标注的方法，即事件处理器<br />**5、注册**`**DefaultEventListenerFactory**`：负责将`@EventListener`标注的方法包装为`ApplicationListener`对象
<a name="QeeCf"></a>
## 4、阶段2~阶段13
阶段2到阶段13，这中间的12个阶段都位于`refresh`方法中，所以`refresh`方法非常很重要，需要多花时间研究这个方法。<br />`refresh`方法源码如下：
```java
// org.springframework.context.support.AbstractApplicationContext#refresh

@Override
public void refresh() throws BeansException, IllegalStateException {
    // 阶段2：Spring应用上下文启动准备阶段
    prepareRefresh();

    // 阶段3：BeanFactory创建阶段
    ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();

    // 阶段4：BeanFactory准备阶段
    prepareBeanFactory(beanFactory);

    try {
        // 阶段5：BeanFactory后置处理阶段
        postProcessBeanFactory(beanFactory);

        // 阶段6：BeanFactory注册BeanPostProcessor阶段
        invokeBeanFactoryPostProcessors(beanFactory);

        // 阶段7：注册BeanPostProcessor
        registerBeanPostProcessors(beanFactory);

        // 阶段8：初始化内建Bean：MessageSource
        initMessageSource();

        // 阶段9：初始化内建Bean：Spring事件广播器
        initApplicationEventMulticaster();

        // 阶段10：Spring应用上下文刷新阶段，由子类实现
        onRefresh();

        // 阶段11：Spring事件监听器注册阶段
        registerListeners();

        // 阶段12：实例化所有剩余的（非lazy init）单例。
        finishBeanFactoryInitialization(beanFactory);

        // 阶段13：刷新完成阶段
        finishRefresh();
    }
}
```
<a name="FzR1y"></a>
### 阶段2：Spring应用上下文启动准备阶段
```java
protected void prepareRefresh() {
    // 标记启动时间
    this.startupDate = System.currentTimeMillis();
    // 是否关闭：false
    this.closed.set(false);
    // 是否启动：true
    this.active.set(true);

    // 初始化PropertySource，留个子类去实现的，可以在这个方法中扩展属性配置信息，丢到this.environment中
    initPropertySources();

    // 验证环境配置中是否包含必须的配置参数信息，比如希望上下文启动中，this.environment中必须要有某些属性的配置，才可以启动，那么可以在这个里面验证
    getEnvironment().validateRequiredProperties();

    // earlyApplicationListeners用来存放早期的事件监听器
    if (this.earlyApplicationListeners == null) {
        this.earlyApplicationListeners = new LinkedHashSet<>(this.applicationListeners);
    }
    else {
        // Reset local application listeners to pre-refresh state.
        this.applicationListeners.clear();
        this.applicationListeners.addAll(this.earlyApplicationListeners);
    }

    // earlyApplicationEvents用来存放早期的事件
    this.earlyApplicationEvents = new LinkedHashSet<>();
}
```
**这里说一下什么是早期事件？什么是早期的事件监听器呢？**<br />当使用spring上下文发布事件的时候，如下代码
```java
applicationContext.publishEvent(事件对象)
```
其内部最终会用到`AbstractApplicationContext`下面这个属性来发布事件，但是可能此时`applicationEventMulticaster`还没有创建好，是空对象，所以此时是无法广播事件的，那么此时发布的时间就是早期的时间，就会被放在`this.earlyApplicationEvents`中暂时存放着，当时间广播器`applicationEventMulticaster`创建好了之后，才会会将早期的事件广播出去
```java
private ApplicationEventMulticaster applicationEventMulticaster;
```
同理，当`applicationEventMulticaster`还未准备好的时候，调用下面下面代码向Spring上下文中添加事件监听器的时候，这时放进去的监听器就是早期的事件监听器。
```java
applicationContext.addApplicationListener(事件监听器对象)
```
说了这么多，理解起来很简单，就是当事件广播器`applicationEventMulticaster`还未准备好的时候，此时向上下文中添加的事件就是早期的事件，会被放到`this.earlyApplicationEvents`中，此时这个事件暂时没办法广播。
<a name="n8OFO"></a>
### 阶段3：`BeanFactory`创建阶段
这个阶段负责将`BeanFactory`创建好，返回给Spring应用上下文
```java
ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();
```
`obtainFreshBeanFactory`源码：
```java
protected ConfigurableListableBeanFactory obtainFreshBeanFactory() {
    //刷新BeanFactory，由子类实现
    refreshBeanFactory();
    //返回spring上下文中创建好的BeanFacotry
    return getBeanFactory();
}
```
<a name="wl63Q"></a>
### 阶段4：`BeanFactory`准备阶段
```java
prepareBeanFactory(beanFactory);
```
源码如下
```java
protected void prepareBeanFactory(ConfigurableListableBeanFactory beanFactory) {
    // 设置类加载器
    beanFactory.setBeanClassLoader(getClassLoader());
    // 设置spel表达式解析器
    beanFactory.setBeanExpressionResolver(new StandardBeanExpressionResolver(beanFactory.getBeanClassLoader()));
    // 设置属性编辑器注册器
    beanFactory.addPropertyEditorRegistrar(new ResourceEditorRegistrar(this, getEnvironment()));

    // @1：添加一个BeanPostProcessor：ApplicationContextAwareProcessor，当你的bean实现了spring中xxxAware这样的接口，那么bean创建的过程中，将由ApplicationContextAwareProcessor来回调这些接口，将对象注入进去
    beanFactory.addBeanPostProcessor(new ApplicationContextAwareProcessor(this));
    // @2：自动注入的时候，下面这些接口定义的方法，将会被跳过自动注入，为什么？因为这部分接口将由ApplicationContextAwareProcessor来回调注入
    beanFactory.ignoreDependencyInterface(EnvironmentAware.class);
    beanFactory.ignoreDependencyInterface(EmbeddedValueResolverAware.class);
    beanFactory.ignoreDependencyInterface(ResourceLoaderAware.class);
    beanFactory.ignoreDependencyInterface(ApplicationEventPublisherAware.class);
    beanFactory.ignoreDependencyInterface(MessageSourceAware.class);
    beanFactory.ignoreDependencyInterface(ApplicationContextAware.class);

    // @3:注册依赖注入的时候查找的对象，比如你的bean中想用ResourceLoader，那么可以写：@Autowire private ResourceLoader resourceLoader，那么spring容器会将spring容器上下文注入进去
    beanFactory.registerResolvableDependency(BeanFactory.class, beanFactory);
    beanFactory.registerResolvableDependency(ResourceLoader.class, this);
    beanFactory.registerResolvableDependency(ApplicationEventPublisher.class, this);
    beanFactory.registerResolvableDependency(ApplicationContext.class, this);

    // @4:注入一个beanpostprocessor：ApplicationListenerDetector
    beanFactory.addBeanPostProcessor(new ApplicationListenerDetector(this));

    // @5:将Environment注册到spring容器，对应的bena名称是environment
    if (!beanFactory.containsLocalBean(ENVIRONMENT_BEAN_NAME)) {
        beanFactory.registerSingleton(ENVIRONMENT_BEAN_NAME, getEnvironment());
    }
    // @6:将系统属性注册到spring容器，对应的bean名称是systemProperties
    if (!beanFactory.containsLocalBean(SYSTEM_PROPERTIES_BEAN_NAME)) {
        beanFactory.registerSingleton(SYSTEM_PROPERTIES_BEAN_NAME, getEnvironment().getSystemProperties());
    }
    // @7:将系统环境变量配置信息注入到spring容器，对应的bean名称是systemEnvironment
    if (!beanFactory.containsLocalBean(SYSTEM_ENVIRONMENT_BEAN_NAME)) {
        beanFactory.registerSingleton(SYSTEM_ENVIRONMENT_BEAN_NAME, getEnvironment().getSystemEnvironment());
    }
}
```
<a name="XhbIC"></a>
#### **先来看看@1的代码**
```java
beanFactory.addBeanPostProcessor(new ApplicationContextAwareProcessor(this));
```
向Spring容器中注入了一个`ApplicationContextAwareProcessor`，这是一个`BeanPostProcessor`（Bean处理器），这个接口中定义了很多方法，会在bean创建的不同阶段被调用，常用来扩展bean的创建过程。<br />在回到`ApplicationContextAwareProcessor`这个接口上来，看一下其的源码，大家就知道是干什么的，如下，当bean实现了`Aware`这样的接口的时候，接口中的方法会被回调，用来在自定义的bean中注入Spring上下文中的一些对象，比如在bean中用到`Environment`，那么只需实现`EnvironmentAware`接口，那么`Environment`会被自动注入进去
```java
if (bean instanceof EnvironmentAware) {
    ((EnvironmentAware) bean).setEnvironment(this.applicationContext.getEnvironment());
}
if (bean instanceof EmbeddedValueResolverAware) {
    ((EmbeddedValueResolverAware) bean).setEmbeddedValueResolver(this.embeddedValueResolver);
}
if (bean instanceof ResourceLoaderAware) {
    ((ResourceLoaderAware) bean).setResourceLoader(this.applicationContext);
}
if (bean instanceof ApplicationEventPublisherAware) {
    ((ApplicationEventPublisherAware) bean).setApplicationEventPublisher(this.applicationContext);
}
if (bean instanceof MessageSourceAware) {
    ((MessageSourceAware) bean).setMessageSource(this.applicationContext);
}
if (bean instanceof ApplicationContextAware) {
    ((ApplicationContextAware) bean).setApplicationContext(this.applicationContext);
}
```
<a name="gY3Fr"></a>
#### 再来看看@3的代码
```java
beanFactory.registerResolvableDependency(BeanFactory.class, beanFactory);
beanFactory.registerResolvableDependency(ResourceLoader.class, this);
beanFactory.registerResolvableDependency(ApplicationEventPublisher.class, this);
beanFactory.registerResolvableDependency(ApplicationContext.class, this);
```
这个用来向Spring容器中添加依赖查找的对象，上面代码的第1行是添加了`BeanFactory`，当bean中想用到`BeanFactory`的时候，只需要按照下面这么写，那么就会被自动注入，即使因为上面第1行代码将`BeanFactory`添加到依赖查找列表中了
```java
@Autowire
private BeanFactory beanFactory;
```
<a name="MP8sC"></a>
#### 再来看看@4的代码
```java
beanFactory.addBeanPostProcessor(new ApplicationListenerDetector(this));
```
`ApplicationListenerDetector`也是一个`BeanPostProcessor`，这个类是干嘛的？处理自定义的事件监听器的。<br />当bean实现了`ApplicationListener`接口，是一个事件监听器的时候，那么这个bean创建的过程中将会被`ApplicationListenerDetector`处理，会将这个bean添加到Spring上下文容器的事件监听器列表中。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683103292274-2d814da3-c0f0-4961-9738-9af0a98d3407.png#averageHue=%23f8f7f3&clientId=u969703ae-8345-4&from=paste&id=udbe82b73&originHeight=320&originWidth=1078&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc87f9888-2a64-4f8d-8225-2ed7b909cbf&title=)
<a name="GAQgv"></a>
#### 再来看看@5的代码
```java
if (!beanFactory.containsLocalBean(ENVIRONMENT_BEAN_NAME)) {
    beanFactory.registerSingleton(ENVIRONMENT_BEAN_NAME, getEnvironment());
}
```
将`Environment`作为单例注册到Spring容器单例列表中，对应的bena名称是environment，当bean中需要用到`Environment`的时候，可以使用下面的写法，此时Spring容器创建bean的过程中，就会从单例的bean列表中找到`Environment`，将其注入到下面的属性中。
```java
@Autowire
private Environment environment
```
还可以通过environment名称从Spring容器中查找到`Environment`对象，如下：
```java
applicationContext.getBean("environment", Environment.class)
```
<a name="qkV3E"></a>
#### 再来看看@6、@7代码
这个同@5的代码，这里说一下
```java
getEnvironment().getSystemProperties() -- 对应 --> System.getProperties()
getEnvironment().getSystemEnvironment() -- 对应 --> System.getenv()
```
这里说一下`System.getProperties()`、`System.getenv()`这两个是干嘛的。<br />下面这个命令大家都用过吧，大家主要看一下-D，这个可以设置一些启动参数，`-D`后面跟的这些参数，会被放在`System.getProperties()`中了。
```bash
java -jar -D参数=值
```
`System.getenv()`用来获取环境变量信息的。
<a name="zUlgv"></a>
### 阶段5：`BeanFactory`后置处理阶段
```java
postProcessBeanFactory(beanFactory);
```
这个方法留给子类实现的，此时`beanFactory`已经创建好了，但是容器中的bean还没有被实例化，子类可以实现这个方法，可以对`BeanFactory`做一些特殊的配置，比如可以添加一些自定义`BeanPostProcessor`等等，主要是留给子类去扩展的。
<a name="Dzh0R"></a>
### 阶段6：`BeanFactory`注册`BeanPostProcessor`阶段
```java
invokeBeanFactoryPostProcessors(beanFactory);
```
调用`BeanFactoryPostProcessor`，`BeanFactoryPostProcessor`是干什么的？<br />这个阶段主要就是从Spring容器中找到`BeanFactoryPostProcessor`接口的所有实现类，然后调用，完成所有bean注册的功能，注意是bean注册，即将bean的定义信息转换为`BeanDefinition`对象，然后注册到Spring容器中，此时bean还未被实例化，下面继续。<br />大家再回头看一下阶段1，阶段1在Spring容器中注册了一个非常关键的bean：`ConfigurationClassPostProcessor`，代码如下
```java
if (!registry.containsBeanDefinition(CONFIGURATION_ANNOTATION_PROCESSOR_BEAN_NAME)) {
    RootBeanDefinition def = new RootBeanDefinition(ConfigurationClassPostProcessor.class);
    def.setSource(source);
    beanDefs.add(registerPostProcessor(registry, def, CONFIGURATION_ANNOTATION_PROCESSOR_BEAN_NAME));
}
```
`ConfigurationClassPostProcessor`就实现了`BeanFactoryPostProcessor`接口，所以`ConfigurationClassPostProcessor`会在阶段6中被调用，下面这些注解都是在这个类中处理的，所以想研究下面这些注解原理的，直接看去看`ConfigurationClassPostProcessor`的源码，非常关键重要的一个类，研究Spring源码，此类必看
```java
@Configuration
@Component
@PropertySource
@PropertySources
@ComponentScan
@ComponentScans
@Import
@ImportResource
@Bean
```
**通常，阶段6执行完毕之后，所有自定义的bean都已经被注册到Spring容器中了，被转换为**`**BeanDefinition**`**丢到**`**BeanFactory**`**中了，此时bean还未被实例化。**
<a name="xq8b2"></a>
### 阶段7：注册`BeanPostProcessor`
```java
registerBeanPostProcessors(beanFactory);
```
注册`BeanPostProcessor`，这个阶段会遍历Spring容器bean定义列表，把所有实现了`BeanPostProcessor`接口的bean撸出来，然后将他们添加到Spring容器的`BeanPostProcessor`列表中。<br />`BeanPostProcessor`是bean后置处理器，其内部提供了很多方法，用来对bean创建的过程进行扩展的。
<a name="uWV4q"></a>
### 阶段8：初始化内建Bean：`MessageSource`
```java
initMessageSource();
```
`MessageSource` 是用来处理国际化的，这个阶段会将`MessageSource`创建好，如果想扩展或者实现国家化的，可以看看这个方法的源码。
<a name="zAKWh"></a>
### 阶段9：初始化内建Bean：Spring事件广播器
```java
initApplicationEventMulticaster();
```
`ApplicationEventMulticaster`是事件广播器，用来广播事件的，这个阶段会将`ApplicationEventMulticaster`创建好，如果想自定义事件广播器的，可以看看这个方法的源码。
<a name="BREqf"></a>
### 阶段10：Spring应用上下文刷新阶段，由子类实现
```java
onRefresh();
```
用来初始化其他特殊的bean，由子类去实现。
<a name="QPswX"></a>
### 阶段11：Spring事件监听器注册阶段
```java
registerListeners();
```
注册事件监听器到事件广播器中，看一下其源码，如下：
```java
protected void registerListeners() {
    // @1:先注册静态指定的侦听器,即将spring上下文中添加的时间监听器，添加到时间广播器（ApplicationEventMulticaster）中
    for (ApplicationListener<?> listener : getApplicationListeners()) {
        getApplicationEventMulticaster().addApplicationListener(listener);
    }

    // @2:将spring容器中定义的事件监听器，添加到时间广播器（ApplicationEventMulticaster）中
    String[] listenerBeanNames = getBeanNamesForType(ApplicationListener.class, true, false);
    for (String listenerBeanName : listenerBeanNames) {
        getApplicationEventMulticaster().addApplicationListenerBean(listenerBeanName);
    }

    // @3:此时事件广播器准备好了，所以此时发布早期的事件（早期的事件由于事件广播器还未被创建，所以先被放在了earlyApplicationEvents中，而此时广播器创建好了，所以将早期的时间发布一下）
    Set<ApplicationEvent> earlyEventsToProcess = this.earlyApplicationEvents;
    this.earlyApplicationEvents = null;
    if (earlyEventsToProcess != null) {
        for (ApplicationEvent earlyEvent : earlyEventsToProcess) {
            getApplicationEventMulticaster().multicastEvent(earlyEvent);
        }
    }
}
```
**这里主要说一下@1**，将上下文中的事件监听器添加到事件广播器中，看看下面源码就懂了
```java
// org.springframework.context.support.AbstractApplicationContext

Set<ApplicationListener<?>> applicationListeners = new LinkedHashSet<>();

public Collection<ApplicationListener<?>> getApplicationListeners() {
    return this.applicationListeners;
}

@Override
public void addApplicationListener(ApplicationListener<?> listener) {
    Assert.notNull(listener, "ApplicationListener must not be null");
    if (this.applicationEventMulticaster != null) {
        this.applicationEventMulticaster.addApplicationListener(listener);
    }
    this.applicationListeners.add(listener);
}
```
再来看看@3，广播早期的事件，早期的时候，由于事件广播器`this.applicationEventMulticaster`还是空，所以事件被放在了`this.earlyApplicationEvents`这个集合中并没有广播，等到这个时候才广播早期的事件，所以用事件的时候，大家需要注意，如果在阶段11之前，调用了下面方法发布事件，那么可能此时看不到事件产生的效果
```
applicationContext.publishEvent(事件对象)
```
所以如果bean实现了下面这些接口，不建议再实现`org.springframework.context.ApplicationListener`接口，否则会让你莫名其妙的感觉。
```
org.springframework.beans.factory.config.BeanFactoryPostProcessor
org.springframework.beans.factory.config.BeanPostProcessor
```
<a name="ipqDw"></a>
### 阶段12：实例化所有剩余的（非lazy init）单例
```java
finishBeanFactoryInitialization(beanFactory);
```
**这个方法中将实例化所有单例的bean（不包需要延迟实例化的bean）**，来看看其源码：
```java
protected void finishBeanFactoryInitialization(ConfigurableListableBeanFactory beanFactory) {
    // 添加${}表达式解析器
    if (!beanFactory.hasEmbeddedValueResolver()) {
        beanFactory.addEmbeddedValueResolver(strVal -> getEnvironment().resolvePlaceholders(strVal));
    }

    // 冻结所有bean定义，表示已注册的bean定义不会被进一步修改或后处理。这允许工厂主动缓存bean定义元数据。
    beanFactory.freezeConfiguration();

    // @1：实例化所有单例bean（不包含需延迟实例化的bean），通常scope=singleton的bean都会在下面这个方法中完成初始化。
    beanFactory.preInstantiateSingletons();
}
```
重点看@1的代码，这里会调用`beanFactory`的`preInstantiateSingletons()`方法，进去看源码，源码位于`DefaultListableBeanFactory`这个类中，如下：
```java
// org.springframework.beans.factory.support.DefaultListableBeanFactory#preInstantiateSingletons

public void preInstantiateSingletons() throws BeansException {
    // beanDefinitionNames表示当前BeanFactory中定义的bean名称列表，
    List<String> beanNames = new ArrayList<>(this.beanDefinitionNames);

    // @1：循环实例化bean
    for (String beanName : beanNames) {
        //这里根据beanName来来实例化bean，代码省略了。。。。
    }

    // @2：变量bean，若bean实现了SmartInitializingSingleton接口，将调用其afterSingletonsInstantiated()方法
    for (String beanName : beanNames) {
        Object singletonInstance = getSingleton(beanName);
        if (singletonInstance instanceof SmartInitializingSingleton) {
            final SmartInitializingSingleton smartSingleton = (SmartInitializingSingleton) singletonInstance;
            smartSingleton.afterSingletonsInstantiated();
        }
    }
}
```
上面这个方法主要做了2个事情：<br />1、完成所有单例bean的实例化：对应上面的代码@1，循环遍历beanNames列表，完成所有单例bean的实例化工作，这个循环完成之后，所有单例bean已经实例化完毕了，被放在Spring容器缓存起来了。<br />2、回调`SmartInitializingSingleton`接口：对应上面的代码@2，此时所有的单例bean已经实例化好了，此时遍历所有单例bean，若bean实现了`SmartInitializingSingleton`接口，这个接口中有个`afterSingletonsInstantiated`方法，此时会回调这个方法，若想在所有单例bean创建完毕之后，做一些事情，可以实现这个接口。
<a name="ezEAE"></a>
### 阶段13：刷新完成阶段
```java
finishRefresh();
```
进去看看其源码
```java
protected void finishRefresh() {
    // @1：清理一些资源缓存
    clearResourceCaches();

    // @2：为此上下文初始化生命周期处理器
    initLifecycleProcessor();

    // @3：首先将刷新传播到生命周期处理器
    getLifecycleProcessor().onRefresh();

    // @4：发布ContextRefreshedEvent事件
    publishEvent(new ContextRefreshedEvent(this));
}
```
<a name="kL6Pp"></a>
#### 先来看@2的代码
```java
initLifecycleProcessor();
```
源码如下
```java
protected void initLifecycleProcessor() {
    
    ConfigurableListableBeanFactory beanFactory = getBeanFactory();
    //spring容器中是否有名称为"lifecycleProcessor"的bean
    if (beanFactory.containsLocalBean("lifecycleProcessor")) {
        //从spring容器中找到LifecycleProcessor，赋给this.lifecycleProcessor
        this.lifecycleProcessor =
            beanFactory.getBean("lifecycleProcessor", LifecycleProcessor.class);
    }
    else {
        //如果spring容器中没有自定义lifecycleProcessor，那么创建一个默认的DefaultLifecycleProcessor
        DefaultLifecycleProcessor defaultProcessor = new DefaultLifecycleProcessor();
        defaultProcessor.setBeanFactory(beanFactory);
        //设置当前spring应用上下文的生命周期处理器
        this.lifecycleProcessor = defaultProcessor;
        //将其注册到spring容器中
        beanFactory.registerSingleton("lifecycleProcessor", this.lifecycleProcessor);
    }
}
```
<a name="NFZD0"></a>
#### 再来看@3的代码
```java
getLifecycleProcessor().onRefresh();
```
调用`LifecycleProcessor`的`onRefresh`方法， 这里要先说一下`org.springframework.context.LifecycleProcessor`这个接口，生命周期处理器，接口中定义了2个方法，而`onClose`方法会在上下文关闭中会被调用，稍后会看到
```java
public interface LifecycleProcessor extends Lifecycle {

    /**
     * 上下文刷新通知
     */
    void onRefresh();

    /**
     * 上下文关闭通知
     */
    void onClose();

}
```
先来看看`onRefresh`方法，这个接口有个默认实现`org.springframework.context.support.DefaultLifecycleProcessor`，所以默认情况下，就看`DefaultLifecycleProcessor`中的`onRefresh`源码，如下
```java
public void onRefresh() {
    startBeans(true);
    this.running = true;
}
```
进入`startBeans(true)`内部看看，如下，代码看起来可能有点绕，实际上很简单，就是从容器中找到所有实现`org.springframework.context.Lifecycle`接口的bean，然后调用他们的`start`方法。
```java
private void startBeans(boolean autoStartupOnly) {
    Map<String, Lifecycle> lifecycleBeans = getLifecycleBeans();
    Map<Integer, LifecycleGroup> phases = new HashMap<>();
    lifecycleBeans.forEach((beanName, bean) -> {
        if (!autoStartupOnly || (bean instanceof SmartLifecycle && ((SmartLifecycle) bean).isAutoStartup())) {
            int phase = getPhase(bean);
            LifecycleGroup group = phases.get(phase);
            if (group == null) {
                group = new LifecycleGroup(phase, this.timeoutPerShutdownPhase, lifecycleBeans, autoStartupOnly);
                phases.put(phase, group);
            }
            group.add(beanName, bean);
        }
    });
    if (!phases.isEmpty()) {
        List<Integer> keys = new ArrayList<>(phases.keySet());
        Collections.sort(keys);
        for (Integer key : keys) {
            phases.get(key).start();
        }
    }
}
```
<a name="RUG6z"></a>
#### 再来看@4的代码
```java
publishEvent(new ContextRefreshedEvent(this));
```
发布`ContextRefreshedEvent`事件，想在这个阶段做点事情的，可以监听这个事件。
<a name="louIT"></a>
#### 小结下
这个阶段主要干了3个事情：<br />1、初始化当前上下文中的生命周期处理器`LifecycleProcessor`。<br />2、调用`LifecycleProcessor.onRefresh()`方法，若没有自定义`LifecycleProcessor`，那么会走`DefaultLifecycleProcessor`，其内部会调用Spring容器中所有实现`Lifecycle`接口的bean的`start`方法。<br />3、发布`ContextRefreshedEvent`事件。
<a name="qOLFC"></a>
## 4、阶段14：Spring应用上下文关闭阶段
```java
applicationContext.close()
```
最终会进入到`doClouse()`方法，代码如下
```java
// org.springframework.context.support.AbstractApplicationContext#doClose

protected void doClose() {
    // 判断是不是需要关闭（active为tue的时候，才能关闭，并用cas确保并发情况下只能有一个执行成功）
    if (this.active.get() && this.closed.compareAndSet(false, true)) {

        //@1：发布关闭事件ContextClosedEvent
        publishEvent(new ContextClosedEvent(this));

        // @2：调用生命周期处理器的onClose方法
        if (this.lifecycleProcessor != null) {
            this.lifecycleProcessor.onClose();
        }

        // @3：销毁上下文的BeanFactory中所有缓存的单例
        destroyBeans();

        // @4：关闭BeanFactory本身
        closeBeanFactory();

        // @5：就给子类去扩展的
        onClose();

        // @6：恢复事件监听器列表至刷新之前的状态，即将早期的事件监听器还原
        if (this.earlyApplicationListeners != null) {
            this.applicationListeners.clear();
            this.applicationListeners.addAll(this.earlyApplicationListeners);
        }

        // @7：标记活动状态为：false
        this.active.set(false);
    }
}
```
<a name="F59Ci"></a>
### 先来看@1的代码
```java
publishEvent(new ContextClosedEvent(this));
```
发布`ContextClosedEvent`事件。
<a name="VLboK"></a>
### 再来看@2的代码
```java
if (this.lifecycleProcessor != null) {
    this.lifecycleProcessor.onClose();
}
```
调用生命周期处理器的`onClose`方法，这里直接看`DefaultLifecycleProcessor`的`onClose()`，看看其源码，如下
```java
public void onClose() {
    stopBeans();
    //将running设置为false
    this.running = false;
}
```
`stopBeans()`源码如下，一大片，简单点理解：就是从容器中找到所有实现`org.springframework.context.Lifecycle`接口的bean，然后调用他们的`close()`方法。
```java
private void stopBeans() {
    Map<String, Lifecycle> lifecycleBeans = getLifecycleBeans();
    Map<Integer, LifecycleGroup> phases = new HashMap<>();
    lifecycleBeans.forEach((beanName, bean) -> {
        int shutdownPhase = getPhase(bean);
        LifecycleGroup group = phases.get(shutdownPhase);
        if (group == null) {
            group = new LifecycleGroup(shutdownPhase, this.timeoutPerShutdownPhase, lifecycleBeans, false);
            phases.put(shutdownPhase, group);
        }
        group.add(beanName, bean);
    });
    if (!phases.isEmpty()) {
        List<Integer> keys = new ArrayList<>(phases.keySet());
        keys.sort(Collections.reverseOrder());
        for (Integer key : keys) {
            phases.get(key).stop();
        }
    }
}
```
<a name="ucDlU"></a>
### 再来看@3的代码
```java
destroyBeans();
```
销毁上下文的`BeanFactory`中所有缓存的单例bean，主要干2件事情：<br />**1、调用bean的销毁方法**<br />比如bean实现了`org.springframework.beans.factory.DisposableBean`接口，此时会被调用。还有bean中有些方法标注了`@PreDestroy`注解的，此时也会被调用。<br />**2、将bean的信息从**`**BeanFactory**`**中清理掉**
<a name="CblcJ"></a>
### 再来看@4的代码
```java
closeBeanFactory();
```
源码如下，主要就是想当前Spring应用上下文中的`beanFactory`属性还原。
```java
// org.springframework.context.support.AbstractRefreshableApplicationContext#closeBeanFactory

@Override
protected final void closeBeanFactory() {
    synchronized (this.beanFactoryMonitor) {
        if (this.beanFactory != null) {
            this.beanFactory.setSerializationId(null);
            this.beanFactory = null;
        }
    }
}
```
<a name="axQET"></a>
## 5、总结
本文详细介绍了Spring应用上下文的生命周期，非常重要，内容和细节稍微比较多。
