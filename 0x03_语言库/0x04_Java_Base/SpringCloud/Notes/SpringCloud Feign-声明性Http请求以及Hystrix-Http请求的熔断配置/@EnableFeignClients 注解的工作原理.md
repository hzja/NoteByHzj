Java SpringCloud @EnableFeignClients
<a name="Of8qc"></a>
## 1、概述
在`Spring cloud`应用中，当要使用`feign`客户端时，一般要做以下三件事情 :
<a name="d8caacb2"></a>
### 1.1 使用注解`@EnableFeignClients`启用`feign`客户端；
示例 :
```java
@SpringBootApplication
@EnableFeignClients
public class TestApplication {
    public static void main(String[] args) {
        SpringApplication.run(TestApplication.class, args);
    }
}
```
<a name="PIgrM"></a>
### 1.2 使用注解`@FeignClient` 定义`feign`客户端 ;
示例 : 该例子定义了一个`feign`客户端，将远程服务`http://test-service/test/echo`映射为一个本地`Java`方法调用。
```java
@FeignClient(name = "test-service", path = "/test")
public interface TestService {
    @RequestMapping(value = "/echo", method = RequestMethod.GET)
    TestModel echo(@RequestParam("parameter") String parameter);
}
```
<a name="suAXi"></a>
### 1.3 使用注解`@Autowired`使用上面所定义`feign`的客户端 ；
```java
@Autowired   
TestService testService;
public void run()
{
    // 这里的使用本地Java API的方式调用远程的Restful接口
    TestModel dto = testService.echo("Hello,你好!");
    log.info("echo : {}", dto);
 }
```
上面的三个步骤，前两个步骤可以理解为定义`feign`客户端,第三步是使用所定义的`feign`客户端。通过调试发现，上面第三步所注入的`testService`是一个代理对象，如下所示 :
```java
testService = {$Proxy66@5502} 
"HardCodedTarget(type=TestService, name=test-service, url=http://test-service/test)"
    h = {ReflectiveFeign$FeignInvocationHandler@6924} 
target = {Target$HardCodedTarget@6930} 
dispatch = {LinkedHashMap@6931}  size = 1
    0 = {LinkedHashMap$Entry@6948} 
"public abstract xxx.model.TestModel xxx.service.TestService.echo(java.lang.String)"
```
该对象会代理客户端完成远程服务方法的调用，那么，该代理对象是如何生成的 ？通过源代码分析来回答这些问题。
<a name="MCT7u"></a>
## 2、源代码解析
> 源代码版本 : spring-cloud-openfeign-core-2.1.0.RELEASE , Spring Cloud Greenwich.RELEASE

<a name="mQ6IF"></a>
### 2.1 注解`@EnableFeignClients`:扫描和注册`feign`客户端`bean`定义
注解`@EnableFeignClients`告诉框架扫描所有使用注解`@FeignClient`定义的`feign`客户端。它又通过注解`@Import`导入了类`FeignClientsRegistrar`( `feign`客户端注册器),如下所示:
```java
@EnableFeignClients 
 => @Import(FeignClientsRegistrar.class)
```
那么 `FeignClientsRegistrar` 又是做什么的呢 ？继续分析。
<a name="2rlTY"></a>
### 2.2 `FeignClientsRegistrar` : `feign`客户端注册器
`FeignClientsRegistrar`实现了接口 `ImportBeanDefinitionRegistrar`。而`ImportBeanDefinitionRegistrar`的设计目的，就是被某个实现类实现，配合使用`@Configuration`注解的使用者配置类，在配置类被处理时，用于额外注册一部分`bean`定义:
> 对于上面的例子，使用者配置类就是 `TestApplication`

```java
public interface ImportBeanDefinitionRegistrar {
   /**
    * Register bean definitions as necessary based on the given annotation metadata of
    * the importing @Configuration class.
    * 根据使用者配置类的注解元数据注册bean定义
    * @param importingClassMetadata 使用者配置类的注解元数据
    * @param registry 当前bean定义注册表，一般指当前Spring应用上下文对象，当前Spring容器
    */
   public void registerBeanDefinitions(
     AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry);
}
```
<a name="le6N2"></a>
### 2.3 `registerBeanDefinitions` – 注册`feign`客户端配置和`feign`客户端
方法`FeignClientsRegistrar#registerBeanDefinitions`实现如下:
```java
@Override
public void registerBeanDefinitions(AnnotationMetadata metadata,BeanDefinitionRegistry registry) {
    // 注册缺省配置到容器 registry
    registerDefaultConfiguration(metadata, registry);
    // 注册所发现的各个 feign 客户端到到容器 registry
    registerFeignClients(metadata, registry);
}
```
<a name="jF6j2"></a>
### 2.4 `registerDefaultConfiguration`– 注册`feign`客户端缺省配置
```java
// 注册feign客户端的缺省配置，缺省配置信息来自注解元数据的属性 defaultConfiguration    
private void registerDefaultConfiguration(AnnotationMetadata metadata,BeanDefinitionRegistry registry) {
    // 获取注解@EnableFeignClients的注解属性     
    Map<String, Object> defaultAttrs = metadata
        .getAnnotationAttributes(EnableFeignClients.class.getName(), true);
    if (defaultAttrs != null && defaultAttrs.containsKey("defaultConfiguration")) {
        String name;
        // 下面是对所注册的缺省配置的的命名，格式如下 :
        // default.xxx.TestApplication
        if (metadata.hasEnclosingClass()) {
            //  针对注解元数据metadata对应一个内部类或者方法返回的方法本地类的情形
            name = "default." + metadata.getEnclosingClassName();
        }
        else {        
            // name 举例 : default.xxx.TestApplication
            // 这里 xxx.TestApplication 是注解@EnableFeignClients所在配置类的长名称   
            name = "default." + metadata.getClassName();
        }
        // 各种信息准备就绪，现在执行注册
        registerClientConfiguration(registry, name,
                                    defaultAttrs.get("defaultConfiguration"));
    }
}
```
`#registerDefaultConfiguration`方法最终注册客户端缺省配置的动作交给方法`#registerClientConfiguration`执行。
<a name="aeiuF"></a>
### 2.5 `registerClientConfiguration` – 注册`feign`客户端配置
```java
// 将指定feign客户端配置configuration作为一个bean定义注册到容器：
// bean 定义对象类型 : GenericBeanDefinition
// bean class : FeignClientSpecification    
// bean name : default.xxx.TestApplication.FeignClientSpecification (缺省配置)
// bean name : test-service.FeignClientSpecification (针对某个feign client 的配置)
private void registerClientConfiguration(BeanDefinitionRegistry registry, Object name,
                                         Object configuration) {
    BeanDefinitionBuilder builder = BeanDefinitionBuilder
        .genericBeanDefinition(FeignClientSpecification.class);
    // 设置构造函数参数  
    builder.addConstructorArgValue(name);
    builder.addConstructorArgValue(configuration);
    // 从bean定义构建器构造bean定义并注册到容器
    registry.registerBeanDefinition(
        name + "." + FeignClientSpecification.class.getSimpleName(),
        builder.getBeanDefinition());
}
```
`#registerClientConfiguration`方法用于注册一个`feign`客户端配置`bean`,可以用于注册针对所有`feign`客户端的缺省配置的注册，也可以用于针对每个`feign`客户端的专有配置的注册。<br />针对所有`feign`客户端的缺省配置的`bean`名称类似于 : `default.xxx.TestApplication.FeignClientSpecification`，<br />针对某个名称为`test-service`的`feign`客户端的配置的`bean`名称类似于:`test-service.FeignClientSpecification`。
<a name="i5i20"></a>
### 2.6 `registerFeignClients` – 注册各个`feign`客户端及其配置
```java
// 参数 metadata : 注解@EnableFeignClients所在配置类的注解元数据
public void registerFeignClients(AnnotationMetadata metadata,BeanDefinitionRegistry registry) {
    // 定义一个基于classpath的组件扫描器，它会根据指定的扫描位置和@EnableFeignClients注解属性   
    // 找出开发人员定义的所有feign客户端，也就是那些使用了注解@FeignClient的所有接口定义
    ClassPathScanningCandidateComponentProvider scanner = getScanner();
    scanner.setResourceLoader(this.resourceLoader);
    Set<String> basePackages;
    // attrs 用于表示注解@EnableFeignClients所在配置类的注解元数据中注解@EnableFeignClients
    // 的部分
    Map<String, Object> attrs = metadata
        .getAnnotationAttributes(EnableFeignClients.class.getName());
    AnnotationTypeFilter annotationTypeFilter = new AnnotationTypeFilter(
        FeignClient.class);
    final Class<?>[] clients = attrs == null ? null
        : (Class<?>[]) attrs.get("clients");
    if (clients == null || clients.length == 0) {
        // @EnableFeignClients 中没有指定 clients 属性的情况
        scanner.addIncludeFilter(annotationTypeFilter);
        basePackages = getBasePackages(metadata);
    }
    else {
        // @EnableFeignClients 中指定了 clients 属性的情况
        final Set<String> clientClasses = new HashSet<>();
        basePackages = new HashSet<>();
        for (Class<?> clazz : clients) {
            basePackages.add(ClassUtils.getPackageName(clazz));
            clientClasses.add(clazz.getCanonicalName());
        }
        AbstractClassTestingTypeFilter filter = new AbstractClassTestingTypeFilter() {
            @Override
            protected boolean match(ClassMetadata metadata) {
                String cleaned = metadata.getClassName().replaceAll("\\$", ".");
                return clientClasses.contains(cleaned);
            }
        };
        scanner.addIncludeFilter(
            new AllTypeFilter(Arrays.asList(filter, annotationTypeFilter)));
    }
    // 使用 scanner 扫描每一个 basePackage, 获取其中的 feign 客户端定义， 
    // 也就是 @FeignClient 定义的那些接口
    for (String basePackage : basePackages) {
        Set<BeanDefinition> candidateComponents = scanner
            .findCandidateComponents(basePackage);
        for (BeanDefinition candidateComponent : candidateComponents) {
            if (candidateComponent instanceof AnnotatedBeanDefinition) {
                // verify annotated class is an interface
                AnnotatedBeanDefinition beanDefinition = (AnnotatedBeanDefinition) candidateComponent;
                AnnotationMetadata annotationMetadata = beanDefinition.getMetadata();
                Assert.isTrue(annotationMetadata.isInterface(),
                              "@FeignClient can only be specified on an interface");
                // 获取所定义的feign客户端接口上的注解@FeignClient属性
                Map<String, Object> attributes = annotationMetadata
                    .getAnnotationAttributes(
                    FeignClient.class.getCanonicalName());
                String name = getClientName(attributes);
                // 将所定义的feign客户端上的配置属性作为一个bean注册到容器   
                // 此方法的逻辑我们上面已经分析过
                registerClientConfiguration(registry, name,
                                            attributes.get("configuration"));
                // 将所定义的feign客户端作为一个bean注册到容器：
                // bean 定义类型 : GenericBeanDefinition
                //  bean class : FeignClientFactoryBean
                //  autowire 模式 : 根据类型绑定
                // @FeignClient注解中的url,path,fallback等属性会设置为bean定义的属性
                registerFeignClient(registry, annotationMetadata, attributes);
            }
        }
    }
}   
// 辅助工具类，从@EnableFeignClients注解属性中获取basePackages属性:
// 参考以下@EnableFeignClients注解属性 :
// 1. value
// 2. basePackages
// 3. basePackageClasses
// 4. 配置类所在的包
// 参数 importingClassMetadata : 使用注解@EnableFeignClients的配置类的元数据
protected Set<String> getBasePackages(AnnotationMetadata importingClassMetadata) {
    // 注解@EnableFeignClients的属性
    Map<String, Object> attributes = importingClassMetadata
        .getAnnotationAttributes(EnableFeignClients.class.getCanonicalName());
    Set<String> basePackages = new HashSet<>();
    for (String pkg : (String[]) attributes.get("value")) {
        if (StringUtils.hasText(pkg)) {
            basePackages.add(pkg);
        }
    }
    for (String pkg : (String[]) attributes.get("basePackages")) {
        if (StringUtils.hasText(pkg)) {
            basePackages.add(pkg);
        }
    }
    for (Class<?> clazz : (Class[]) attributes.get("basePackageClasses")) {
        basePackages.add(ClassUtils.getPackageName(clazz));
    }
    if (basePackages.isEmpty()) {
        basePackages.add(
            ClassUtils.getPackageName(importingClassMetadata.getClassName()));
    }
    return basePackages;
}
```
`#registerFeignClients` 最终注册`feign`客户端配置的动作交给`#registerClientConfiguration`完成，而注册`feign`客户端的动作交给`#registerFeignClient`方法完成。
<a name="naw8T"></a>
### 2.7 `registerFeignClient` – 注册一个`feign`客户端
```java
// 将所定义的feign客户端作为一个bean注册到容器：
// bean 定义类型 : GenericBeanDefinition
//  bean class : FeignClientFactoryBean -- 这是一个工厂bean，而不是最终bean实例的class
//  autowire 模式 : 根据类型绑定
// @FeignClient注解中的url,path,fallback等属性会设置为bean定义的属性
// 参数 registry : Spring 容器
// 参数 annotationMetadata : @FeignClient所注解的接口上的注解元数据
// 参数 attributes : @FeignClient 注解属性信息
private void registerFeignClient(BeanDefinitionRegistry registry,
                                 AnnotationMetadata annotationMetadata, Map<String, Object> attributes) {
    String className = annotationMetadata.getClassName();
    BeanDefinitionBuilder definition = BeanDefinitionBuilder
        .genericBeanDefinition(FeignClientFactoryBean.class);
    validate(attributes);
    definition.addPropertyValue("url", getUrl(attributes));
    definition.addPropertyValue("path", getPath(attributes));
    String name = getName(attributes);
    definition.addPropertyValue("name", name);
    definition.addPropertyValue("type", className);
    definition.addPropertyValue("decode404", attributes.get("decode404"));
    definition.addPropertyValue("fallback", attributes.get("fallback"));
    definition.addPropertyValue("fallbackFactory", attributes.get("fallbackFactory"));
    definition.setAutowireMode(AbstractBeanDefinition.AUTOWIRE_BY_TYPE);
    String alias = name + "FeignClient";
    AbstractBeanDefinition beanDefinition = definition.getBeanDefinition();
    boolean primary = (Boolean)attributes.get("primary"); // has a default, won't be null
    beanDefinition.setPrimary(primary);
    String qualifier = getQualifier(attributes);
    if (StringUtils.hasText(qualifier)) {
        alias = qualifier;
    }
    BeanDefinitionHolder holder = new BeanDefinitionHolder(beanDefinition, className,
                                                           new String[] { alias });
    BeanDefinitionReaderUtils.registerBeanDefinition(holder, registry);
}
```
从上面的代码分析可知，`FeignClientsRegistrar`的主要作用如下 :

1. 注册缺省`feign`客户端配置`bean`定义；<br />
2. 对于每个`@FeignClient`注解的`feign`客户端定义 :<br />
   1. 注册一个针对该`feign`客户端的配置`bean`定义；<br />
   2. 注册该`feign`客户端`bean`定义,指定生成`bean`实例采用工厂类`FeignClientFactoryBean`;<br />

而且，上述功能实现在类方法`FeignClientsRegistrar#registerBeanDefinitions`中，这是接口`ImportBeanDefinitionRegistrar`所定义的方法。该方法会在`@EnableFeignClients`注解被处理时执行。具体的执行时调用栈如下所示：
```java
AbstractApplicationContext#invokeBeanFactoryPostProcessors
    => PostProcessorRegistrationDelegate#invokeBeanFactoryPostProcessors
    => foreach BeanDefinitionRegistryPostProcessor : #postProcessBeanDefinitionRegistry
    => ConfigurationClassPostProcessor#postProcessBeanDefinitionRegistry
    => #processConfigBeanDefinitions
    => ConfigurationClassBeanDefinitionReader#loadBeanDefinitions
    => foreach ConfigurationClass : #loadBeanDefinitionsForConfigurationClass
    => #loadBeanDefinitionsFromRegistrars
    => foreach ImportBeanDefinitionRegistrar : #registerBeanDefinitions
    => FeignClientsRegistrar#registerBeanDefinitions
```
<a name="qkv2x"></a>
### 2.8 `FeignClientFactoryBean`生成`feign`客户端代理对象
基于上面的分析，可以得知，开发人员所定义的`feign`客户端和相关配置会以`bean`定义的形式注册到`bean`容器中，这样当使用`@Autowired`注入一个`feign`客户端时，容器会使用工厂类`FeignClientFactoryBean`为其生成一个实例。下面来看其具体工作过程。
<a name="FxwiR"></a>
#### `FeignClientFactoryBean#getObject`生成`feign`客户端代理对象
```java
// 该方法由接口FactoryBean约定
@Override
public Object getObject() throws Exception {
    return getTarget();
}
<T> T getTarget() {
    //  从应用上下文中获取创建 feign 客户端的上下文对象 FeignContext
    // FeignContext 针对每个feign客户端定义会生成一个不同的 AnnotationConfigApplicationContext，
    // 这些应用上下文的parent都设置为当前应用的主应用上下文
    // 参考 : FeignAutoConfiguration
    FeignContext context = applicationContext.getBean(FeignContext.class);
    // 为目标feign客户端对象构建一个 builder,该builder最终生成的目标feign客户端是一个
    // 动态代理，使用 InvocationHandler ：ReflectiveFeign$FeignInvocationHandler
    Feign.Builder builder = feign(context);
    if (!StringUtils.hasText(this.url)) {
        // @FeignClient 属性 url 属性没有指定的情况         
        // 根据属性 name , path 拼装一个 url，
        // 这种通常是需要在多个服务节点之间进行负载均衡的情况
        if (!this.name.startsWith("http")) {
            url = "http://" + this.name;
        }
        else {
            url = this.name;
        }
        // 方法cleanPath()加工属性path，使其以/开头，不以/结尾
        url += cleanPath();
        // 这里形成的url格式类似 :  http://test-service/test
        // 其中 test-service 是服务名，不是服务所在节点的IP，主机名或者域名

        // 函数 loadBalance 做如下动作 :
        // 1. 将builder和一个LoadBalancerFeignClient bean实例关联起来
        // 2. 使用一个HystrixTargeter将builder和一个 HardCodedTarget bean实例关联起来
        // 这里 HardCodedTarget 表示对应 url 为 http://test-service/test 的远程服务(可能
        // 包含多个服务方法)
        // 3. 生成最终的feign client 实例 : ReflectiveFeign$FeignInvocationHandler 的动态代理对象，
        // 使用 InvocationHandler ：ReflectiveFeign$FeignInvocationHandler。
        // 每个远程服务方法会对应到一个@FeignClient注解的接口方法上(依据方法上的注解进行匹配)
        return (T) loadBalance(builder, context, new HardCodedTarget<>(this.type,
                                                                       this.name, url));
    }

    // @FeignClient 属性 url 属性被指定的情况 
    // 这种通常是明确指出了服务节点的url的情况，实际上不需要负载均衡
    if (StringUtils.hasText(this.url) && !this.url.startsWith("http")) {
        this.url = "http://" + this.url;
    }
    String url = this.url + cleanPath();
    // 将builder和一个LoadBalancerFeignClient bean实例关联起来
    Client client = getOptional(context, Client.class);
    if (client != null) {
        if (client instanceof LoadBalancerFeignClient) {
            // not load balancing because we have a url,
            // but ribbon is on the classpath, so unwrap
            // 因为指定了明确的服务节点url，所以这里不需要负载均衡，
            // 所以这里尽管client是LoadBalancerFeignClient，所以
            // 实际上可以获取其所代理的对象作为最终的client，
            // 相当于去掉了LoadBalancerFeignClient这层的代理功能
            client = ((LoadBalancerFeignClient)client).getDelegate();
        }
        builder.client(client);
    }
    // 使用一个HystrixTargeter将builder和一个 HardCodedTarget bean实例关联起来
    Targeter targeter = get(context, Targeter.class);
    // 生成最终的feign client 实例 : ReflectiveFeign$FeignInvocationHandler 的动态代理对象，
    // 使用 InvocationHandler ：ReflectiveFeign$FeignInvocationHandler。
    // 每个远程服务方法会对应到 一个@FeignClient注解的接口方法上(依据方法上的注解进行匹配)        
    return (T) targeter.target(this, builder, context, new HardCodedTarget<>(
        this.type, this.name, url));
}
```
<a name="uPPuB"></a>
#### 方法`FeignClientFactoryBean#feign` – 创建`feign`客户端构建器
```java
protected Feign.Builder feign(FeignContext context) {
    FeignLoggerFactory loggerFactory = get(context, FeignLoggerFactory.class);
    Logger logger = loggerFactory.create(this.type);
    // 从上下文获取一个 Feign.Builder 上,
    // 并从上下文获得 Encoder, Decoder, Contract 设置到该 builder 上
    Feign.Builder builder = get(context, Feign.Builder.class)
        // required values
        .logger(logger)
        .encoder(get(context, Encoder.class))
        .decoder(get(context, Decoder.class))
        .contract(get(context, Contract.class));

    // 对 builder 进行其他属性设置
    configureFeign(context, builder);
    return builder;
}
```
<a name="fojeR"></a>
#### 方法`FeignClientFactoryBean#loadBalance` – 生成具备负载均衡能力的`feign`客户端
为`feign`客户端构建器绑定负载均衡客户端,绑定目标服务端点，并生成最终的`feign`客户端实例。
```java
// 对builder设置负载均衡客户端，绑定到目标服务端点，构建最终的feign客户端对象
protected <T> T loadBalance(Feign.Builder builder, FeignContext context,
                            HardCodedTarget<T> target) {
    // 从上下文context获取一个Client，缺省是 LoadBalancerFeignClient  
    Client client = getOptional(context, Client.class);
    if (client != null) {
        // 将client设置到builder上
        builder.client(client);
        // 从上下文中获取一个 targeter,缺省是一个 HystrixTargeter
        Targeter targeter = get(context, Targeter.class);
        // 上面获取得到的 targeter 会根据 builder 的类型决定如何将 target
        // 绑定到 builder 并设置有关的其他属性和功能,然后生成最终的feign客户端对象
        return targeter.target(this, builder, context, target);
    }
    throw new IllegalStateException(
        "No Feign Client for loadBalancing defined. Did you forget to include " +
        "spring-cloud-starter-netflix-ribbon?");
}
```
从上面分析可以看出，缺省情况下，所使用的`feign`客户端构建器类为`Feign.Builder`,并且`Targeter`是一个`HystrixTargeter`。`HystrixTargeter#target`方法的参数`builder`为`Feign.Builder`时，会直接调用该`builder`的`target`方法,如下所示 ：
```java
class HystrixTargeter implements Targeter {
    @Override
    public <T> T target(FeignClientFactoryBean factory, Feign.Builder feign, FeignContext context,
                        Target.HardCodedTarget<T> target) {
        if (!(feign instanceof feign.hystrix.HystrixFeign.Builder)) {
            return feign.target(target);
        }
        // ... 省略其他代码   
    }
}
```
接下来再来看`Feign.Builder#target`是如何工作的:
```java
// 执行构建并且创建相应的feign客户端实例 
public <T> T target(Target<T> target) {
    return build().newInstance(target);
}

// 构建过程，最终根据各种配置生成一个 ReflectiveFeign 对象    
public Feign build() {
    SynchronousMethodHandler.Factory synchronousMethodHandlerFactory =
        new SynchronousMethodHandler.Factory(client, retryer, requestInterceptors, logger,
                                             logLevel, decode404, closeAfterDecode, propagationPolicy);
    ParseHandlersByName handlersByName =
        new ParseHandlersByName(contract, options, encoder, decoder, queryMapEncoder,
                                errorDecoder, synchronousMethodHandlerFactory);
    return new ReflectiveFeign(handlersByName, invocationHandlerFactory, queryMapEncoder);
}
```
然后再看`ReflectiveFeign#newInstance`方法:
```java
// 创建最终的feign客户端实例 : 一个 ReflectiveFeign$FeignInvocationHandler 的动态代理对象
@Override
public <T> T newInstance(Target<T> target) {
    Map<String, MethodHandler> nameToHandler = targetToHandlersByName.apply(target);
    Map<Method, MethodHandler> methodToHandler = new LinkedHashMap<Method, MethodHandler>();
    List<DefaultMethodHandler> defaultMethodHandlers = new LinkedList<DefaultMethodHandler>();
    for (Method method : target.type().getMethods()) {
        if (method.getDeclaringClass() == Object.class) {
            continue;
        } else if (Util.isDefault(method)) {
            // 对于每个缺省方法，使用 DefaultMethodHandler 
            DefaultMethodHandler handler = new DefaultMethodHandler(method);
            defaultMethodHandlers.add(handler);
            methodToHandler.put(method, handler);
        } else {
            // 对于每个对应服务功能端点的方法，缺省使用nameToHandler获取的MethodHandler，缺省是
            // SynchronousMethodHandler
            methodToHandler.put(method, nameToHandler.get(Feign.configKey(target.type(), method)));
        }
    }
    // 创建feign客户端实例 ReflectiveFeign$FeignInvocationHandler,
    // 该对象包含了上面所创建的methodToHandler，用于对应各个开发者定义的@FeignClient接口方法
    InvocationHandler handler = factory.create(target, methodToHandler);
    // 创建feign客户端实例的动态代理对象
    T proxy = (T) Proxy.newProxyInstance(target.type().getClassLoader(),
                                         new Class<?>[] {target.type()}, handler);
    // 将缺省方法处理器绑定到feign客户端实例的动态代理对象上
    for (DefaultMethodHandler defaultMethodHandler : defaultMethodHandlers) {
        defaultMethodHandler.bindTo(proxy);
    }
    return proxy;
}
```
从上面的分析不难看出，为什么最终注入的`testService`最终是一个`ReflectiveFeign$FeignInvocationHandler`动态代理实例了。
<a name="cporz"></a>
## 3、总结
从上面的分析可以看出，当使用注解`@EnableFeignClients` 时，相当于启用了`feign`客户端定义的扫描和注册机制，从而可以发现开发人员通过注解`@FeignClient`定义的`feign`客户端，并最终作为`bean`定义注册到容器中。<br />而通过`@Autowired`自动装配注解，这些`feign`客户端会以`ReflectiveFeign$FeignInvocationHandler`动态代理的形式被注入到使用方。该`feign`客户端包含了对每个接口方法的处理器`MethodHandler`,接口缺省方法对应`DefaultMethodHandler`,服务功能端点方法对应`SynchronousMethodHandler`。
