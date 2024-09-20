JavaSpringBoot
<a name="J9uEX"></a>
## 一、何为自动装配
自动装配是 SpringBoot 的核心功能，主要是让开发者尽可能少的关注一些基础化的 Bean 的配置，实际上完成的工作是**如何自动将 Bean 装载到 Ioc 容器中**。<br />在 SpringBoot 中如果想要引入一个新的模块，例如项目中想使用 Redis 缓存，只需要做以下几步即可。<br />1、在 pom.xml 文件中引入 spring-boot-starter-data-redis 相关的 jar 包
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```
2、在 application.properties 文件中加入 Redis 相关的配置
```
spring.redis.host=127.0.0.1
spring.redis.port=6379
```
3、在代码中引用 Redis 缓存的操作类
```java
@Autowired
private RedisTemplate<String,String> redisTemplate;
```
为什么 RedisTemplate 可以被直接注入，它是什么时候加入到 Ioc 容器中的，这都是自动装配的功劳，一起来看一下。
<a name="QvIQv"></a>
## 二、自动装配过程
<a name="OIKvm"></a>
### 1、`@EnableAutoConfiguration`分析
SpringBoot 项目启动类上有 `@SpringBootApplication` 这样一个注解，它继承了 `@EnableAutoConfiguration`，主要作用是帮助 Springboot 应用把所有符合条件的配置类都加载到当前 SpringBoot 创建并使用的 Ioc 容器中。<br />这个注解主要由两部分组成

1. `**@AutoConfigurationPackage**`，指定 SpringBoot 扫描的包范围，这个范围下使用 `@Service` 、 `@Component` 等注解的 Bean 加入 Ioc 容器，默认值是启动类所在的包路径，默认指定启动类路径下的类加载到 Ioc 容器。
2. `**@Import(AutoConfigurationImportSelector.class)**`，将导入第三方提供的 Bean 配置类加载加载到 Ioc 容器。
```java
@AutoConfigurationPackage
@Import(AutoConfigurationImportSelector.class)
public @interface EnableAutoConfiguration {
    ...
}
```
<a name="LJBBf"></a>
#### 1.1 `@AutoConfigurationPackage`
`@AutoConfigurationPackage `指定 SpringBoot 扫描的包范围，主要逻辑在 `AutoConfigurationPackages#register` 方法中。<br />该方法有两个参数 registry 和 packageNames，在断点中发现 registry 实际上就是 `**DefaultListableBeanFactory**` 实例，packageNames 的值默认是启动类包所在的路径，在这里将 `@AutoConfigurationPackage` 指定的包路径添加到 `**DefaultListableBeanFactory**`，在后续Ioc容器扫描时将其加载进去。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698803029689-7d5b610e-4c26-4fbe-866f-826fd01218e6.jpeg#averageHue=%23333734&clientId=uecb0c72a-a339-4&from=paste&id=u67cfaa43&originHeight=576&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf2e09b1d-80cd-43f0-a21f-767b665fdcb&title=)
<a name="b6A0K"></a>
#### 1.2 `AutoConfigurationImportSelector.class`
`AutoConfigurationImportSelector` 主要是实现 `importSelector` 方法来实现基于动态 Bean 的加载功能，定位到 `importSelector` 方法看一下里面的逻辑。
```java
@Override
public String[] selectImports(AnnotationMetadata annotationMetadata) {
    if (!isEnabled(annotationMetadata)) {
        return NO_IMPORTS;
    }
    //1、从配置文件spring-autoconfigure-metadata.properties中加载自动装配候选规则
    AutoConfigurationMetadata autoConfigurationMetadata = AutoConfigurationMetadataLoader
    .loadMetadata(this.beanClassLoader);
    //2、获取@SpringBootApplication上配置的属性值
    AnnotationAttributes attributes = getAttributes(annotationMetadata);
    //3、使用SpringFactoriesLoader 加载classpath路径下META-INF\spring.factories中
    //通过key=org.springframework.boot.autoconfigure.EnableAutoConfiguration获取候选类
    List<String> configurations = getCandidateConfigurations(annotationMetadata,attributes);
    //4、去除重复值
    configurations = removeDuplicates(configurations);
    //5、获取exclude属性值，将exclude中的值排除掉
    Set<String> exclusions = getExclusions(annotationMetadata, attributes);
    checkExcludedClasses(configurations, exclusions);
    configurations.removeAll(exclusions);
    //6、检查候选配置类上的注解@ConditionalOnClass，如果要求的类不存在，则这个候选类会被过滤不被加载
    configurations = filter(configurations, autoConfigurationMetadata);
    fireAutoConfigurationImportEvents(configurations, exclusions);
    return StringUtils.toStringArray(configurations);
}
```
第一步和第三步逻辑中涉及到两个非常重要的文件 spring-autoconfigure-metadata.properties、spring.factories

1. spring.factories 主要记录了待自动装配的候选类，从下图断点中可以看到有 109 个。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698803029800-65b8a0e5-a599-434c-ae6f-d90ecf3d0ee4.jpeg#averageHue=%23313533&clientId=uecb0c72a-a339-4&from=paste&id=ue906afdd&originHeight=264&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u84cc929e-66b1-4a25-9a0d-dcdf0335a48&title=)

2. spring-autoconfigure-metadata.properties 中配置了系列 `ConditionalOnClass` 类和配置类之间的依赖，通过这个文件中配置的规则，来判断 spring.factories 哪些类真的需要加载。为什么会这样做，是因为很多的 `@Configuration` 其实是依托于其他的框架来加载的，如果当前的 classpath 环境下没有相关联的依赖，则意味着这些类没必要进行加载，断点的最后发现真正需要加载的只有 31 个。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698803029667-4c21d6da-ae7b-4b0b-9e36-9d06a53c8519.jpeg#averageHue=%23303331&clientId=uecb0c72a-a339-4&from=paste&id=uc7ad9946&originHeight=301&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8515ac8e-10bf-4fbe-bb2a-b3ec98016e7&title=)

3. 加载 spring.factories 文件，借助了 Spring 框架提供的一个工具类 SpringFactoriesLoader，它的实现和 java 中的 SPI 机制原理是一样的，它相对于 SPI 的改进点在于不会一次性加载所有的类，而是根据 key 进行加载。

SPI ，全称为 Service Provider Interface，是一种服务发现机制。它通过在 ClassPath 路径下的 META-INF/services 文件夹查找文件，自动加载文件里所定义的类。这一机制为很多框架扩展提供了可能，比如在 Dubbo、JDBC 中都使用到了 SPI 机制。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698803029691-a3ca2412-b7a2-4331-9828-3f43ca34663e.jpeg#averageHue=%23f8f8f8&clientId=uecb0c72a-a339-4&from=paste&id=udbea259f&originHeight=321&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue824a70f-100c-45db-b31b-9b3ca6f9b48&title=)
<a name="ojGof"></a>
### 2、`@Import`分析
在 `@EnableAutoConfiguration` 分析中，两种加载 Bean 到 Ioc 容器的方式，他们都是通过 `@Import` 引入，这里来分析一下 `@import` 是在哪里进行加载的。
<a name="DG80s"></a>
#### 2.1 `@Import`3种使用方式

1. 引入普通类，直接在 `@Import` 中引入即可。
```java
@Import(PersonConfig.class)
@Configuration
public class PersonConfiguration { 
}
```

2. 引入 `ImportSelector` 实现类（`AutoConfigurationImportSelector` 就是实现了这个接口），这个接口需要实现的方法是 `selectImports()`，它返回的是一个字符串数组，代表的是类名集合，这些类将会被加载到Ioc容器中。
```java
@Import(TestImportSelector.class)
@Configuration
public class ImportTestConfig {
}
```
```java
public class TestImportSelector implements ImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{"com.example.service.TestService"};
    }
}
```

3. 引用 `ImportBeanDefinitionRegistrar` 实现类(`AutoConfigurationPackages.Registrar` 就是实现类这个接口)，这个接口需要实现的方法是 `registerBeanDefinitions()`，它有两个入参，第一个参数 `AnnotationMetadata`代表当前类的注解信息；第二个参数 registry 代表的是 `**DefaultListableBeanFactory**` 实例，因为参数 `**DefaultListableBeanFactory**` 代表的是 Ioc 容器，如果想注入 Bean，可以直接对该类进行操作。
```java
@Import(TestImportBeanDefinitorSelector.class)
@Configuration
public class ImportBeanDefinitionTestConfig {
}
```
```java
public class TestImportBeanDefinitorSelector implements ImportBeanDefinitionRegistrar {
    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata,
                                        BeanDefinitionRegistry registry) {
        AbstractBeanDefinition beanDefinition = BeanDefinitionBuilder
        .rootBeanDefinition(Person.class)
        .getBeanDefinition();
        registry.registerBeanDefinition("person", beanDefinition);
    }
}
```
<a name="Sgkq5"></a>
#### 2.2 `@Import`实现原理

1. `@Import` 主要由 `**ConfigurationClassPostProcessor**` 后置处理器加载实现，`**ConfigurationClassPostProcessor**` 则实现了 `BeanDefinitionRegistryPostProcessor` 接口，在 `invokeBeanFactoryPostProcessors()` 方法中进行处理，该方法实例化并调用所有的 BeanFactory 后置处理器。
```java
@Override
public void refresh() throws BeansException, IllegalStateException {
    //....省略n行代码
    //1.beanFactory后置处理逻辑，在这个方法里加载ConfigurationClassPostProcessor
    invokeBeanFactoryPostProcessors(beanFactory);
    //2.注册bean后置处理逻辑
    registerBeanPostProcessors(beanFactory);
    //...省略n行代码
    //3.实例化非懒加载的bean，并加入到Ioc容器中
    finishBeanFactoryInitialization(beanFactory);
    //....省略n行代码
}
```

2. `ConfigurationClassPostProcessor` 实现了方法 `postProcessBeanDefinitionRegistry()`，在这个方法中跟踪代码到 `ConfigurationClassParser.parse()`，所有配置类的解析逻辑都在 `parse()` 方法中进行处理。
3. 在 `ConfigurationClassParser.parseparse()` 继续往下跟踪会到 `doProcessConfigurationClass()` 方法，在该方法中会有一些常用配置注解的解析，例如 `@Component`、`@ComponentScan`、`@Bean`、`@Configuration`、`@Import` 等。
```java
@Nullable
protected final SourceClass doProcessConfigurationClass(ConfigurationClass configClass, SourceClass sourceClass)
throws IOException {
    //...省略n行代码
    //加载@Import注解，递归解析，获取导入的配置类
    processImports(configClass, sourceClass, getImports(sourceClass), true);
    //...省略n行代码
}
```

4. `processImports()` 中主要实现类了 `@Import` 接口的 3 种不同的加载方式
```java
private void processImports(ConfigurationClass configClass, 
                            SourceClass currentSourceClass,
                            Collection<SourceClass> importCandidates, 
                            boolean checkForCircularImports) {
    //...省略n行代码
    if (candidate.isAssignable(ImportSelector.class)) {
        //1.实现了ImportSelector接口的类在@Import中引用逻辑
        Class<?> candidateClass = candidate.loadClass();
        ImportSelector selector = BeanUtils.instantiateClass(
            candidateClass,ImportSelector.class);
        ParserStrategyUtils.invokeAwareMethods(
        selector, this.environment, this.resourceLoader, this.registry);
        if (this.deferredImportSelectors != null && 
            selector instanceof DeferredImportSelector) {
            this.deferredImportSelectors.add(
                new DeferredImportSelectorHolder(configClass, (DeferredImportSelector) selector));
        } else {
            String[] importClassNames = selector.selectImports(currentSourceClass.getMetadata());
            Collection<SourceClass> importSourceClasses = asSourceClasses(importClassNames);
            processImports(configClass, currentSourceClass, importSourceClasses, false);
        }
    } else if (candidate.isAssignable(ImportBeanDefinitionRegistrar.class)) {
        //2.实现了ImportBeanDefinitionRegistrar接口的类在@Import中引用逻辑
        Class<?> candidateClass = candidate.loadClass();
        ImportBeanDefinitionRegistrar registrar =BeanUtils.instantiateClass(
            candidateClass, ImportBeanDefinitionRegistrar.class);
        ParserStrategyUtils.invokeAwareMethods(
        registrar, this.environment, this.resourceLoader, this.registry);
        configClass.addImportBeanDefinitionRegistrar(
            registrar, currentSourceClass.getMetadata());
    } else {
        //3.普通类直接在@Import中引用逻辑
        this.importStack.registerImport(
            currentSourceClass.getMetadata(), candidate.getMetadata().getClassName());
        processConfigurationClass(candidate.asConfigClass(configClass));
    }
    //...省略n行代码
}
```
总结一下就是如下的方法链调用
```
refresh()=>invokeBeanFactoryPostProcessors()=>postProcessBeanDefinitionRegistry()=>parse()=>
doProcessConfigurationClass()=>processImports()
```
<a name="kq94n"></a>
### 3、启动类何时加入到 Ioc 容器
前面分析了自动装配的主要逻辑，那么 SpringBoot 启动类又是如何加入到Ioc容器中的呢？
<a name="jULgh"></a>
#### 3.1 `prepareContext()` 中的 `load()` 方法

1. 从 SpringBoot 启动类的 run() 方法开始，跟踪代码到 `SpringApplication.run()` 方法，这里是 SpringBoot 启动的核心逻辑。
2. 在 `SpringApplication.run()` 方法中有一个 `prepareContext()` 方法，进入这个方法里面，会发现有一个 `load()` 方法，这里就是加载启动类的地方，它会将启动类注入到 Ioc 容器中。
```java
private void prepareContext(ConfigurableApplicationContext context,
                            ConfigurableEnvironment environment, SpringApplicationRunListeners listeners,
                            ApplicationArguments applicationArguments, Banner printedBanner) {
    //...省略n行代码
    //加载启动类，将启动类注入到Ioc容器中
    load(context, sources.toArray(new Object[0]));
    //...省略n行代码
}
```

3. 在断点中可以看到，SpringBoot 启动类注入到了 `annotatedReader` 中（`AnnotatedBeanDefinitionReader` 基于注解的 `beanDefinition` 解析器），在这里将启动类加入到了 Ioc 容器。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698803029696-578a79c4-55a1-4151-b685-25b7654b3496.jpeg#averageHue=%233a322a&clientId=uecb0c72a-a339-4&from=paste&id=u6fc57d24&originHeight=362&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7d9d4296-d4bf-485c-93af-08c6947f3bc&title=)<br />总结一下就是如下的方法链调用
```
run()=>prepareContext()()=>load()=>parse()=>register()
```
<a name="vSddR"></a>
### 4、自动装配整体流程
基于以上3块的分析可以得到如下一个关于自动装配的流程图<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698803030048-307c1660-176a-4d94-95aa-78653a452bc0.jpeg#averageHue=%23eff3e1&clientId=uecb0c72a-a339-4&from=paste&id=ua667bd4d&originHeight=967&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue9b3aa72-73fd-4df3-b6ae-54a60d08146&title=)
<a name="HOpCZ"></a>
## 三、总结
学习源码的过程中如果不了解源码的整体思路，直接看代码会迷失在源码的海洋中。要了解代码的整体脉络，以总-分-总的方式去学习，学会舍弃部分无关的代码，才能高效的阅读和学习源码，从中汲取到代码的精华所在，提升自己的编程能力。
