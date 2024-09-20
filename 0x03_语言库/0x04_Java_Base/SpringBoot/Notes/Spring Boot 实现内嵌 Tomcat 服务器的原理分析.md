JavaSpringBoot<br />在Java 面试中聊到 Spring Boot 与 Spring 的区别时，几乎所有人都会提到 **Spring Boot 会通过默认内嵌 tomcat 服务器，可以实现 jar 包的快速启动的功能**。但是当被问到到：“Spring Boot 是如何实现 内嵌 tomcat 服务器的呢？” 回答出来的人却寥寥无几。所以，感觉这一点还是有必要在这里说明一下的。
<a name="e7kYM"></a>
## 一、Spring Boot 如何实现 tomcat 服务器的内嵌？
通常通过依赖 spring-boot-starter-web  这个 **stater** 来实现一个 SpringBoot  web 工程。而 spring-boot-starter-web 依赖，则包含了 **spring-boot-starter**、**web**、**webmvc** 以及 **tomcat** 等依赖：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter</artifactId>
  <version>2.1.6.RELEASE</version>
  <scope>compile</scope>
</dependency>

<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-json</artifactId>
  <version>2.1.6.RELEASE</version>
  <scope>compile</scope>
</dependency>

<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-tomcat</artifactId>
  <version>2.1.6.RELEASE</version>
  <scope>compile</scope>
</dependency>
<dependency>
  <groupId>org.hibernate.validator</groupId>
  <artifactId>hibernate-validator</artifactId>
  <version>6.0.17.Final</version>
  <scope>compile</scope>
</dependency>

<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-web</artifactId>
  <version>5.1.8.RELEASE</version>
  <scope>compile</scope>
</dependency>

<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-webmvc</artifactId>
  <version>5.1.8.RELEASE</version>
  <scope>compile</scope>
</dependency>
```
**web**、**webmvc**、**tomcat** 等依赖提供了 web 应用的运行环境，由于 spring-boot-starter 中会引入 spring-boot-autoconfigure，因此主要用来控制这些运行环境是否工作<br />Tips1<br />**如何查看 内嵌的 tomcat 的版本呢？**<br />很简单，进入 spring-boot-starter-tomcat 依赖，直接查看**tomcat-embed-core**依赖的版本即可，如这里项目中 **tomcat-embed-core** 的依赖为 **9.0.21** ，表示 **tomcat** 使用的版本为 **9.0.21** 版本<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715942662-3b662177-c4eb-40f1-a9f6-7c69af7ee1ec.png#clientId=uc53de10d-e0a4-4&from=paste&id=u2f399977&originHeight=148&originWidth=661&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u957a4aaa-19aa-4baf-a1af-fd1374c642b&title=)
<a name="tjPl3"></a>
## 二、替换为其他服务器（如Netty 服务器）
```xml
<!--引入启动器依赖 里面就有默认的tomcat -->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
  <!--排除tomcat-->
  <exclusions>
    <exclusion>
      <artifactId>spring-boot-starter-tomcat</artifactId>
      <groupId>org.springframework.boot</groupId>
    </exclusion>
  </exclusions>
</dependency>
<!--引入Jetty-->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-jetty</artifactId>
</dependency>
```
<a name="op74h"></a>
## 三、配置加载原理解析
通过 Spring Boot 的自动配置原理，很容易定位到 **Servlet** 的自动配置类 `ServletWebServerFactoryAutoConfiguration`<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715942634-a64e4113-3c3e-4566-bde7-775f01c893a5.png#clientId=uc53de10d-e0a4-4&from=paste&id=u08dbd56b&originHeight=96&originWidth=1064&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5496701c-da81-47bf-a945-140a4aa0531&title=)<br />那么，可以围绕着 **web** 服务器的自动配置类 `ServletWebServerFactoryAutoConfiguration` 对于 **Spring Boot** 内嵌 **tomcat**  的原理进行研究<br />打开 `ServletWebServerFactoryAutoConfiguration` 类，它的源码如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715942782-957001c4-93c8-4c7f-b0c1-9e7be5a7c1b5.png#clientId=uc53de10d-e0a4-4&from=paste&id=ubdcced2b&originHeight=643&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u366a4128-9132-4d26-bf30-34eed7f6393&title=)<br />整体步骤，分为如下 4 个步骤：<br />1、根据条件开启服务器配置类，并引入相关自动配置类；<br />2、注册 `ServletWebServerFactoryCustomizer` 的 **Bean**，采用 `ServerProperties.class` 中的属性；<br />3、注册 `TomcatServletWebServerFactoryCustomizer` 类型的 **Bean**<br />4、定义静态内部类 `BeanPostProcessorsRegistrar`<br />具体来看看：<br />1、根据条件开启服务器配置类，并引入相关自动配置类<br />首先，从上往下，分别来看一下该配置类加载的步骤
<a name="Dc4X9"></a>
### 1）条件加载类 ServletRequest
```java
@ConditionalOnClass(ServletRequest.class)
```
表示 **classpath** 中需要存在 `ServletReques.class` ，而该类存在于 项目中引入的服务器依赖中。由于项目中直接使用了 **Spinrg boot** 默认内嵌的服务器：**tomcat** ，因此就存在于 **tomcat-embed-core** 的 jar 包中。也就是说当前需要存在某个服务器的依赖的情况下，才会加载该配置类<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715943091-7e4f66f8-868f-407a-b5de-15b46eee198d.png#clientId=uc53de10d-e0a4-4&from=paste&id=u909d2761&originHeight=793&originWidth=544&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u751ed838-9f44-44cc-a96b-17445892de7&title=)
<a name="QJ2ET"></a>
### 2）基于servlet
```java
@ConditionalOnWebApplication(type = Type.SERVLET)
```
表示该类的加载，仅基于 **servlet** 的 **Web** 应用程序
<a name="esNsV"></a>
### 3）加载属性
```java
@EnableConfigurationProperties(ServerProperties.class)
```
加载 `ServerProperties` 配置类中的属性，即 `server.*` 开头的配置类，如常常配置的 `server.port` 等配置项，就可以覆盖原有的默认配置<br />`ServerProperties` 配置类的内容如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715942697-79c7615e-34f4-45d0-b6ed-283b24b7024a.png#clientId=uc53de10d-e0a4-4&from=paste&id=udc1ebec6&originHeight=705&originWidth=890&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub59bbe9e-d187-4b54-b62f-e242f1ae4fe&title=)<br />那么，也就是说，使用 **Spring Boot** 进行 **web** 应用的开发，对于 **tomcat** 等服务器的默认配置修改，不需要在寻找之前的 server.xml 等配置文件，只需要通过 server 开头的配置项进行配置即可<br />如，配置 **tomcat** 的 **https** 协议，需要在 server.xml 文件中进行如下配置：
```xml
<!-- Define an SSL/TLS HTTP/1.1 Connector on port 8443
         This connector uses the NIO implementation. The default
         SSLImplementation will depend on the presence of the APR/native
         library and the useOpenSSL attribute of the
         AprLifecycleListener.
         Either JSSE or OpenSSL style configuration may be used regardless of
         the SSLImplementation selected. JSSE style configuration is used below.
-->
<Connector port="443"
    keystoreFile="/xxx/tomcat/cert/restlessman.cn.pfx"
    keystoreType="PKCS12"
    keystorePass=证书密码
    SSLProtocol="TLSv1+TLSv1.1+TLSv1.2"
    ciphers="TLS_RSA_WITH_AES_128_CBC_SHA,TLS_RSA_WITH_AES_256_CBC_SHA,TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA,TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,TLS_RSA_WITH_AES_128_CBC_SHA256,TLS_RSA_WITH_AES_256_CBC_SHA256"/>
```
而使用了 **Spring Boot** 之后，只需要将配置项配置在 **application.properties** 或者 **application.yml** 中即可
```
# HTTPS相关配置
server.port=443
server.ssl.key-store=classpath:*.pfx
server.ssl.key-store-type=PKCS12
server.ssl.key-store-password=证书密码
server.ssl.enabled-protocols=TLSv1,TLSv1.1,TLSv1.2
server.ssl.ciphers =TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA,TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384,TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA,TLS_ECDHE_RSA_WITH_RC4_128_SHA,TLS_RSA_WITH_AES_128_CBC_SHA256,TLS_RSA_WITH_AES_128_CBC_SHA,TLS_RSA_WITH_AES_256_CBC_SHA256,TLS_RSA_WITH_AES_256_CBC_SHA,SSL_RSA_WITH_RC4_128_SHA
```
温馨提示：关于 **tomcat** 的配置在实际项目中使用比较频繁。所以，一定要了解这一点，可以避免很多弯路。
<a name="FdsMb"></a>
### 4）引入几个类
```java
@Import({ ServletWebServerFactoryAutoConfiguration.BeanPostProcessorsRegistrar.class,
         ServletWebServerFactoryConfiguration.EmbeddedTomcat.class,
         ServletWebServerFactoryConfiguration.EmbeddedJetty.class,
         ServletWebServerFactoryConfiguration.EmbeddedUndertow.class })
```
分别导入了并引入了 `ServletWebServerFactoryAutoConfiguration` 本身的静态内部类 `BeanPostProcessorsRegistrar.class`，和 **Spring Boot** 所支持的三种嵌入式容器的自动配置类：`EmbeddedTomcat.class`、`EmbeddedJetty.class` 和 `EmbeddedUndertow.class`，他们均属于 `ServletWebServerFactoryConfiguration` 的静态内部类<br />关于 `BeanPostProcessorsRegistrar.class` 会在第4 节中 再看，目前先来看下 `ServletWebServerFactoryConfiguration` 类都干了些什么<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715943347-c18309dc-b940-443c-a03b-975d3b51f4ea.png#clientId=uc53de10d-e0a4-4&from=paste&id=u3561d8ef&originHeight=661&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u450c980f-626b-4d61-a09e-a4cc6217a4b&title=)<br />可以看到，当前直接使用的是 **Spring Boot** 的默认内嵌的 服务器 **tomcat** ，并未引入其他两个内嵌服务器：Jetty 和 Undertow 的依赖，因此他们相应的类便显示为红色。也就是说，他们并不存在。那么，这两个类 `EmbeddedJetty.class` 和 `EmbeddedUndertow.class` 将不会被声明<br />**2、注册 **`**ServletWebServerFactoryCustomizer**`** 的 Bean，采用 **`**ServerProperties.class**`** 中的属性**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715943481-d3a4d59a-6e40-49a6-b14c-6ad3b8a3ae59.png#clientId=uc53de10d-e0a4-4&from=paste&id=ufdef8751&originHeight=663&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8713bcbb-4d6a-42e1-8939-a1d90b53e0e&title=)<br />这个类很简单，主要是通过 `customize` 方法，将 `serverProperties` 配置值绑定给 `ConfigurableServletWebServerFactory` 对象实例上<br />**3、注册 **`**TomcatServletWebServerFactoryCustomizer**`** 类型的 Bean**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715943420-d827d567-aa08-4ef4-83d9-3dd8e35f2e12.png#clientId=uc53de10d-e0a4-4&from=paste&id=ua7207770&originHeight=666&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u36cea79a-c5c2-4989-95e1-7b49dd04dbf&title=)<br />该类则是通过 `costomize` 方法来处理 Tomcat 配置中的值<br />**4、定义静态内部类 **`**BeanPostProcessorsRegistrar**`<br />来看下 这个 `BeanPostProcessorsRegistrar`
```java
public static class BeanPostProcessorsRegistrar implements ImportBeanDefinitionRegistrar, BeanFactoryAware {

    private ConfigurableListableBeanFactory beanFactory;

    @Override
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
       if (beanFactory instanceof ConfigurableListableBeanFactory) {
          this.beanFactory = (ConfigurableListableBeanFactory) beanFactory;
       }
    }

    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata,
          BeanDefinitionRegistry registry) {
       if (this.beanFactory == null) {
          return;
       }
       registerSyntheticBeanIfMissing(registry, "webServerFactoryCustomizerBeanPostProcessor",
             WebServerFactoryCustomizerBeanPostProcessor.class);
       registerSyntheticBeanIfMissing(registry, "errorPageRegistrarBeanPostProcessor",
             ErrorPageRegistrarBeanPostProcessor.class);
    }


}
```
注册了两个 **bean**，一个 `WebServerFactoryCustomizerBeanPostProcessor`，一个 `errorPageRegistrarBeanPostProcessor`<br />这两个都实现了类 `BeanPostProcessor` 接口，属于 **bean** 的后置处理器，作用是在 **bean** 初始化前后加一些自己的逻辑处理。<br />`WebServerFactoryCustomizerBeanPostProcessor`：获得所有后置处理器，并执行处理器的具体方法<br />`errorPageRegistrarBeanPostProcessor`：获得所有异常处理的后置处理器，并执行其方法<br />那么，加载完这些必要的 **bean** 以及配置项之后，**Spring Boot** 是如何启动 **tomcat**服务器 的呢？
<a name="eHx37"></a>
## 四、启动过程
实际上，**SpringBoot** 在项目启动的时候，通过 SpringApplication 的 `run()` 方法，启动 **tomcat**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715943576-0383fa20-be05-4729-8d07-8b4166e655fa.png#clientId=uc53de10d-e0a4-4&from=paste&id=u35d8c8c8&originHeight=204&originWidth=798&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u07415256-ed1a-4848-8604-b20604e4994&title=)<br />接下来，一起来探究一下 **tomcat** 在 **Spring Boot** 中的启动过程是怎样的<br />1 进入 `SpringApplication` 的 `run()` 方法，可以发现 **Spring Boot** 是通过 `refreshContext(context)` 方法，完成服务器的刷新动作
```java
/**
* Run the Spring application, creating and refreshing a new
* {@link ApplicationContext}.
* @param args the application arguments (usually passed from a Java main method)
* @return a running {@link ApplicationContext}
*/
public ConfigurableApplicationContext run(String... args) {
    StopWatch stopWatch = new StopWatch();
    stopWatch.start();
    ConfigurableApplicationContext context = null;
    Collection<SpringBootExceptionReporter> exceptionReporters = new ArrayList<>();
    configureHeadlessProperty();
    SpringApplicationRunListeners listeners = getRunListeners(args);
    listeners.starting();
    try {
        ApplicationArguments applicationArguments = new DefaultApplicationArguments(args);
        ConfigurableEnvironment environment = prepareEnvironment(listeners, applicationArguments);
        configureIgnoreBeanInfo(environment);
        //准备 banner 打印器，就是在 Spring Boot 启动时，打印在 console 控制台上的 ASCII 艺术字体
        Banner printedBanner = printBanner(environment);
        // 创建 Spring 容器
        context = createApplicationContext();
        // 获得异常报告器数组
        exceptionReporters = getSpringFactoriesInstances(SpringBootExceptionReporter.class,
                                                         new Class[] { ConfigurableApplicationContext.class }, context);
        // Spring 容器的前置处理
        // 是在下一步：容器刷新动作的之前的准备动作。包含：将启动类注入到容器，为后续开启自动化配置奠定基础
        prepareContext(context, environment, listeners, applicationArguments, printedBanner);
        // 刷新容器
        refreshContext(context);
        // Srping 容器的后置处理
        afterRefresh(context, applicationArguments);
        stopWatch.stop();
        if (this.logStartupInfo) {
            new StartupInfoLogger(this.mainApplicationClass).logStarted(getApplicationLog(), stopWatch);
        }
        listeners.started(context);
        callRunners(context, applicationArguments);
    }
    catch (Throwable ex) {
        handleRunFailure(context, ex, exceptionReporters, listeners);
        throw new IllegalStateException(ex);
    }

    try {
        listeners.running(context);
    }
    catch (Throwable ex) {
        handleRunFailure(context, ex, exceptionReporters, null);
        throw new IllegalStateException(ex);
    }
    return context;
}
```
继续向下跟踪，进入到 `AbstractApplicationContext` 中的 `onRefresh()`<br />2 查看 `onRefresh()`  所有的实现方法，其中有一个 `ServletWebServerApplicationContext`，则是当前 **Web** 容器的实现<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715943691-fbe78630-b412-434f-aaca-67daf0457913.png#clientId=uc53de10d-e0a4-4&from=paste&id=uc493d0b8&originHeight=338&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u350bf9cb-c465-459a-a7b4-f4896c3be8b&title=)<br />3 进入 `ServletWebServerApplicationContext` 类，其通过 `createWebServer()`方法，执行具体逻辑<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715944682-df05c1f1-3820-4e35-8ac5-31dd54262c98.png#clientId=uc53de10d-e0a4-4&from=paste&id=u0420a106&originHeight=267&originWidth=865&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udff4f829-7a30-4e1b-9514-7fa9f2bd566&title=)<br />4 进入 `createWebServer()` 分别获得 `ServletWebServerFactory` 对象，从而调用 `getWebServer` 创建 `WebServer` 对象<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715944289-ac483183-5374-4c4b-8b09-61896da98a36.png#clientId=uc53de10d-e0a4-4&from=paste&id=u783aa949&originHeight=406&originWidth=1021&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2bb57afc-4d65-4ab8-85a1-355f82e4fa1&title=)<br />5 进入 `getWebServer()`方法，发现其主要是对 **tomcat** 的创建，并进行基本的配置<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715944331-7ef33d68-0041-4ac3-a9d0-cfaa41680eb2.png#clientId=uc53de10d-e0a4-4&from=paste&id=u14a9c333&originHeight=605&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3a2aae98-3203-442e-8b6a-91f67592c53&title=)<br />逻辑点又落到了 `getTomcatWebServer(Tomcat tomcat)` 方法身上<br />6 继续进入 `getTomcatWebServer(Tomcat tomcat)` 方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715944376-5e013716-4594-4992-b350-8beea2f48a29.png#clientId=uc53de10d-e0a4-4&from=paste&id=uce248624&originHeight=131&originWidth=956&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub369958a-232d-4e23-b648-89ffc71f48e&title=)<br />发现该方法是通过 `TomcatWebServer` 类的构造器，创建一个 TomcatWebServer 实例<br />7 进入 `TomcatWebServer` 类的构造器方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715944521-635b61cb-cfcb-4263-804a-480f52534ff9.png#clientId=uc53de10d-e0a4-4&from=paste&id=u2a372faa&originHeight=181&originWidth=1070&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub1a44333-10d6-480f-be57-2fe3560ec0f&title=)<br />这个构造方法，则是采用 `initialize()` 方法进行具体的创建动作<br />8 继续进入 `initialize()` 方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660715944880-77ae3405-3f69-4cde-b4c5-99b857fb38d7.png#clientId=uc53de10d-e0a4-4&from=paste&id=u10dad022&originHeight=504&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf62d8a98-60fe-4e2d-aa65-41317eb1b4a&title=)<br />继续跟踪该方法，发现该方法通过 `tomcat.start()` 方法，完成 **tomcat** 服务器的启动动作<br />如果感兴趣，可以自己跑一个 Spring Boot web 项目，代码实时跟踪一下看看具体的细节，方便更深入的理解~
<a name="tGs0V"></a>
## 总结
分别通过 **Spring Boot** 内嵌 **tomcat** 服务器和**将 tomcat 服务器替换为其他服务器的方式**、**Spring Boot 对于 tomcat 服务器的加载步骤**以及 **Spring Boot 启动 tomcat 的过程**简单介绍。相信对于 **Spring Boot** 为什么就可以这样轻易地启动一个 **web** 项目，有了一定的了解了~<br />可以发现，**Spring Boot** 对于 **tomcat** 服务器的加载与启动过程，核心还在于 **Spring Boot** 的**自动配置特性**。项目启动的过程中，通过扫描 `classpath` 下的 `META-INF/spring.factories`，实例化工厂对象 `TomcatServletWebServerFactory`，完成 **tomcat** 服务器所需要的相关类的创建与配置。在调用启动类的 `run()` 方法时，完成 **Tomcat** 对象的创建，环境设置和启动，从而实现 **Tomcat** 服务器的自动化处理。
