Java SpringBoot
<a name="Sejf7"></a>
## 通过`SpringApplication`开始引导启动
`SpringApplication`类是用来执行Spring框架启动的引导类。有两种方式可以进行启动引导：

1. 通过静态方法 `SpringApplication.run`启动。
2. 先创建 `SpringApplication`实例，在调用的实例方法 `run`进行启动。

无论是以上哪种方式，最终都是通过创建`SpringApplication`实例，在调用`run()`启动。
<a name="uOfC5"></a>
## `new SpringApplication`——创建引导启动的实例
在创建`SpringApplication`实例的时候，会根据用户输入和工程环境做一些基础配置，供之后引导启动中使用。

- 设置`ResourceLoader`和`PrimarySources`
- 从类中加载`initializer`和`listener`放在集合
- 设置是否为Web环境(先确认用户是否指定，未指定则根据工程目录下是否有servlet相关环境)
- 从工程环境中决定主入口的类
<a name="qCcbx"></a>
## `run()`——开始引导启动
<a name="GYRA0"></a>
### `new StopWatch()`——创建计时器
`StopWatch`是`springframework.util`中提供的一个工具类，在启动过程中使用`StopWatch`是为了记录启动花费的时间。
<a name="zwkkB"></a>
### `configureHeadlessProperty()`——配置Headless模式
Headless模式是在环境缺少显示器等设备情况下的一种配置，和启动流程并无太多关系，不做介绍。
<a name="AgIPG"></a>
### `SpringApplicationRunListener.start()`——获取监听器,启动监听
监听器可以用来监听`SpringApplication`启动过程中的各个阶段。默认的监听器是`EventPublishRunListener`，用户也可以通过实现`SpringApplicationRunListener`接口，实现应用程序对`SpringApplication`启动过程的监听。<br />在 resources/META-INF 下建立 spring.factories 文件，文件中添加 key=value 形式，其中 key 为 `SpringApplicationRunListener`的全路径名，value 为应用程序对该接口的实现类(类需要一个参数类型为 `SpringApplication `和 String 数组的构造函数，用于通过反射创建实例)。
<a name="aaTeB"></a>
### `prepareEnvironment()`——准备环境，创建`ConfigurableEnvironment`对象
在这一步，`SpringApplication`会创建Spring启动所需的环境，这个环境主要由`ConfigurableEnviroment`对象表示。首先，该对象确认了程序是否需要设置Web环境，其次，该对象还确定了程序所需要的参数和读取的配置文件等信息。<br />此步骤会回调`SpringApplicationRunListener`的`environmentPrepared()`方法，通知监听器环境已经准备好。
<a name="XHVS7"></a>
### `printBanner()`——打印横幅
这一步骤其实和启动并没有太大关系，只是会向控制台或是日志中输出Spring的Logo和版本信息。
<a name="BQIvJ"></a>
### `createApplicationContext()`——创建应用程序上下文并加载Bean
在准备好环境之后，接下来要做的就是创建应用程序上下文`ApplicationContext`对象。<br />`ApplicationContext`是Spring IoC的核心组件，它不仅是程序所需Bean的容器，还提供了国际化，事件发布等功能。<br />在创建应用程序上下文的时候，首先会根据之前配置决定上下文的具体类型（`AnnotationConfigApplicationContext`或是`AnnotationConfigServletWebServerApplicationContext`）。再通过反射实例化到对象。
<a name="csrmK"></a>
### `prepareContext()`——准备`ApplicationContext`
虽然已经得到了`ApplicationContext`对象，但此时的对象还只是一个空白对象，需要准备和处理后，`ApplicationContext`才能被使用。<br />在准备过程中主要做了做了几件事：为`ApplicationContext`设置之前准备好的`Environment`对象。<br />通过对`ApplicationContext`后置处理或是`BeanDefinitionLoader`等方式往容器中添加一些初始的Bean。<br />应用默认的初始化器初始化应用程序上下文(责任链模式的应用，多个初始化器形成一个List，应用程序需要被每个初始化器应用一次，每个初始化器有自己的职责)。<br />准备过程中`ApplicationRunListener`发出两个消息，分别是`contextPrepared`和`contextLoaded`。
<a name="LajLg"></a>
### `refreshContext()`——刷新上下文
在应用程序上下文准备好后，可以通过刷新应用程序上下文发现Bean并加载到容器中。`refreshContext()`会调用`ApplicationContext.refresh()`方法。<br />`AbstractApplicationContext`中定义了`refresh()`方法的基本框架(模板模式的应用)。
<a name="ZZcGg"></a>
#### `prepareRefresh()`——准备刷新
准备刷新的阶段做了初始化和校验的工作。比如初始化启动时间，初始化PropertySources(在`AbstractApplicationContext`中只是一个空方法，留给子类根据需要实现)，以及校验环境中是否已经有必要的参数。
<a name="xV0Qr"></a>
#### `prepareBeanFactory()`——准备`BeanFactory`
`BeanFactory`是 Spring 框架中容器的底层实现，所有的 Bean 都存放在`BeanFactory`中，虽然`ApplicationContext`也实现了`BeanFactory`接口，但是在其内部还是将获取 Bean 的相关操作委托给内部的`DefaultListableBeanFactory`变量，只是`ApplicationContext`帮用户屏蔽了底层的操作，同时提供出一些更符合外部用户使用的接口。<br />对BeanFactory的准备主要是：添加一些必要组件，比如类加载器，表达式解析器，属性编辑器注册表等。<br />以及一些后置处理器，比如`ApplicationContextAwareProcessor`(`xxxAware`的接口就是通过后置处理器在Bean创建的时候，通过后置处理器设置的)。此外还有一些特殊的Bean，`environment`,`systemProperties`和`systemEnvirnoment`。
<a name="UBjc7"></a>
#### `postProcessBeanFactory()`——后置处理`BeanFactory`
对于非`WebServlet`环境的`ApplicationContext`而言这个方法是个空方法，但是Web环境下的`ApplicationContext`会通过这个方法定制一些后处理动作，比如添加`WebApplicationContextServletAwareProcessor`后置处理器，添加在web环境中可能使用的Scope（`session`和`request`）。
<a name="RfqhC"></a>
#### `invokeBeanFactoryPostProcessors()`——实例化并调用`BeanFactoryPostProcessor`
`BeanFactoryPostProcessor`是一种特殊的后置处理器，其操作的对象是针对BeanFactory。<br />此时主要有三个后置处理器，分别是:`SharedMetadataReaderFactoryContextInitializer$CachingMetadataReaderFactoryPostProcessor`,`ConfigurationWarningsApplicationContextInitializer$ConfigurationWarningsPostProcessor`和`ConfigFileApplicationListener$PropertySourceOrderingPostProcessor`。这三个类名字虽然很长，但是其实是因为内部类的关系，而且看名字也能发现类是怎么来的(外部类是xxxInitializer的就说明是初始化器设置的)。<br />其中第一个类和启动流程有关，因为它会向容器注册`ConfigurationClassPostProcessor`。<br />如果`BeanFactoryPostProcessor`同时又是`BeanDefinitionRegistryPostProcessor`，则先进行针对`BeanDefinition`注册表的后置处理，目的是为了发现Bean。<br />在最初的三个`BeanFactoryProcessor`后置处理完成后，会从容器中获取`BeanDefinitionRegistryPostProcessor`类型的后置处理器（这里主要会得到刚才加载的`ConfigurationClassPostProcessor`实例）。再调用这些`BeanDefinitionRegistry`的后置处理器，继续向发现并向容器中注册新的Bean。<br />这里主要是通过`@Configuration`注解作为入口发现Bean，如果发现的Bean中又存在新的`@Configuration`Bean，则以此Bean为入口再进行发现，直到所有的Bean都被发现。<br />在针对`BeanDefinition`注册表的后置处理完成（发现Bean的过程）中，如果找到了`BeanFactoryPostProcessor`(包括最初的三个`BeanFatoryProcessor`)，会进行针对BeanFactory的后置处理过程（之前只是进行针对注册表的后置处理，二者的目的还是有区别的）。<br />注意 Bean的发现过程只是向BeanDefinition注册表注册BeanDefinition的过程，并没有针对发现的Bean进行实例化（少部分需要用到的Bean会进行实例化，比如这部分会对`BeanDefinitionRegistryPostProcessor`类型的Bean实例化）。
<a name="fglxF"></a>
#### `registerBeanPostProcessors()`——注册Bean后置处理器
上一步是针对`BeanFactory`和`BeanDefinitionRegistry`的后置处理器，这一步从BeanFactory中获取针对普通Bean的后置处理器`BeanFactoryPostProcessor`放到专门的容器`beanPostProcessors`中。
<a name="DWXYv"></a>
#### `initMessageSource()`——初始化`MessageSource`
`MessageSource`是拥有特殊功能的Bean，用来处理国际化相关内容。
<a name="wD8ls"></a>
#### `initApplicationEventMulticaster()`——初始化`ApplicationEventMulticaster`
`ApplicationEventMulticaster`是`ApplicationEvent`广播器，可以通过这个对象向容器中添加移除`Listener`，也可以通过这个对象发布事件（观察者模式的应用）。
<a name="aoHf3"></a>
#### `onRefresh()`——刷新应用程序
发现了所有的Bean，并且需要实例化的Bean也都被创建好了之后，Spring接下去要做的是创建`ThemeSource`（和主题相关的组件），以及创建Webserver(如果是Web环境的话)。
<a name="YcNnk"></a>
#### `registerListeners()`——注册监听器
这一步会将初始化得到的`ApplicationListener`方法和容器中获得`ApplicationListener`一起注册到`ApplicationEventMulticaster`中，并且如果存在需要早起发布的事件，则发布事件。
<a name="eYaoL"></a>
#### `finishBeanFactoryInitialzation()`——初始化容器中的Bean
经过之前的步骤，现在容器中必要的组件都已经准备好了，并且所有需要容器管理的Bean也都已经被发现注册成BeanDefinition注册表中。<br />对于Scope是Singleton的Bean而言，此时已经具备了实例化Bean的条件，因此在这一步中，Spring会对所有Singleton且非lazy-init的Bean进行实例化。<br />主要做法就是获取容器中所有为`singletion`且非`lazyInit`的`BeanDefinition`，然后通过getBean创建出Bean的实例，保存在容器内部。<br />有一种特殊的情况是针对`FactoryBean`，`FactoryBean`是一种用来创建Bean的特殊Bean，在得到`FactoryBean`的Bean之后，还需要判断是否要创建`FactoryBean`负责创建的Bean。<br />Bean的实例化过程`getBean()`在以后的文章中再介绍。
<a name="xubIT"></a>
#### `finishRefresh()`——完成刷新
在这步主要是一些资源清理以及注册`LifeCycleProcessor`。`LifeCycleProcessor`可以用来在 Spring 生命周期的`refresh`和`close`时触发回调。并且发布Refresh的消息。
<a name="TaT2t"></a>
### `afterRefresh()`——留给子类的钩子函数
在Application完成刷新后，`SpringApplication`给子类留了`afterRefresh()`的方法作为回调。
<a name="C0q9H"></a>
### 启动完成
启动完成后，`stopWatch`会记录下本次启动消费的时间。<br />然后向`ApplicationRunListener`发布`started`事件，说明已经启动就绪。
<a name="FVlEq"></a>
### 准备运行
启动完成后，正式运行前，SpringApplication还会执行用户定义的`ApplicationRunner`和`CommandLineRunner`两个接口中定义的`run()`方法。<br />在执行完成后，向`ApplicationRunListener`发布`runing`的消息。至此，启动流程结束。
<a name="uurwn"></a>
## 总结
本文旨在对SpringBoot启动流程各个步骤做一次梳理（本文的段落标题就是启动的各个步骤，不同等级的标题也含有方法前后调用的关系），并没有对每行代码做深入分析。如果感兴趣可以对照流程自己分析一遍。另外，在贴一份整理的不错的流程图帮助大家加深印象。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1653639237594-96be0991-57e7-4809-b73d-d5dc198f7d49.jpeg)<br />![Fcant_2022-05-27_16-02-43.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653638578702-40cdc6f2-2072-4271-9912-f428a7b0dc22.png#clientId=u94c1e730-6331-4&from=ui&id=u1459a377&originHeight=1633&originWidth=3548&originalType=binary&ratio=1&rotation=0&showTitle=false&size=395234&status=done&style=none&taskId=u8d49f692-658f-4200-8ac4-7afafeec77d&title=)
