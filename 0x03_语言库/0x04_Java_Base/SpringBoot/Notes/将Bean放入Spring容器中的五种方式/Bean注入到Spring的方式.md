JavaSpring
<a name="dmqHK"></a>
## 配置文件
配置文件的方式就是以外部化的配置方式来声明Spring Bean，在Spring容器启动时指定配置文件。配置文件方式现在用的不多了，但是为了文章的完整性和连续性，这里还是列出来了，知道的小伙伴可以自行跳过这节。<br />配置文件的类型Spring主要支持xml和properties两种类型。
<a name="E1omf"></a>
#### xml
在XmlBeanInjectionDemo.xml文件中声明一个class为类型为User的Bean
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns="http://www.springframework.org/schema/beans"
       xsi:schemaLocation="
       http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       ">

    <bean class="com.fcant.spring.bean.injection.User"/>

</beans>
```
User
```java
@Data
@ToString
public class User {

    private String username;

}
```
测试：
```java
public class XmlBeanInjectionDemo {

    public static void main(String[] args) {
        ClassPathXmlApplicationContext applicationContext = new ClassPathXmlApplicationContext("classpath:XmlBeanInjectionDemo.xml");
        applicationContext.refresh();
        User user = applicationContext.getBean(User.class);
        System.out.println(user);
    }

}
```
结果：
```
User(username=null)
```
可以看出成功将User注入到Spring中，由于没有设置username属性值，所以是null。
<a name="vlWWe"></a>
#### properties
除了xml，spring还支持properties配置文件声明Bean的方式。<br />如下，在PropertiesBeanInjectionDemo.properties文件中声明了class类型为User的Bean，并且设置User的username属性为fcant。
```java
user.(class) = com.fcant.spring.bean.injection.User
user.username = fcant
```
测试：
```java
public class PropertiesBeanInjectionDemo {

    public static void main(String[] args) {
        GenericApplicationContext applicationContext = new GenericApplicationContext();
        //创建一个PropertiesBeanDefinitionReader，可以从properties读取Bean的信息，将读到的Bean信息放到applicationContext中
        PropertiesBeanDefinitionReader propReader = new PropertiesBeanDefinitionReader(applicationContext);
        //创建一个properties文件对应的Resource对象
        Resource classPathResource = new ClassPathResource("PropertiesBeanInjectionDemo.properties");
        //加载配置文件
        propReader.loadBeanDefinitions(classPathResource);
        applicationContext.refresh();
        User user = applicationContext.getBean(User.class);
        System.out.println(user);
    }

}
```
结果：
```
User(username=fcant)
```
成功获取到User对象，并且username的属性为properties设置的fcant。<br />除了可以配置属性之外还支持其它的配置，如何配置可以查看`PropertiesBeanDefinitionReader`类上的注释。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424017256-f9679456-9720-424c-b488-fc2a6bf50a55.png#averageHue=%232b2b2b&clientId=ubbf5f4d1-93bd-4&from=paste&id=u932ca8c8&originHeight=820&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud85c7c1a-66aa-4615-8289-8481938d550&title=)
<a name="EUgv9"></a>
## 注解声明
上一节介绍了通过配置文件的方式来声明Bean，但是配置文件这种方式最大的缺点就是不方便，因为随着项目的不断扩大，可能会产生大量的配置文件。为了解决这个问题，Spring在2.x的版本中开始支持注解的方式来声明Bean。
<a name="xzuC1"></a>
#### `@Component` + `@ComponentScan`
这种方式其实就不用多说，在项目中自定义的业务类就是通过`@Component`及其派生注解(`@Service`、`@Controller`等)来注入到Spring容器中的。<br />在SpringBoot环境底下，一般情况下不需要主动调用`@ComponentScan`注解，因为`@SpringBootApplication`会调用`@ComponentScan`注解，扫描启动引导类(加了`@SpringBootApplication`注解的类)所在的包及其子包下所有加了`@Component`注解及其派生注解的类，注入到Spring容器中。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424017262-41c32f17-c122-43c0-8ab4-2642be7f4d8d.png#averageHue=%232f2b2a&clientId=ubbf5f4d1-93bd-4&from=paste&id=u5f905c2d&originHeight=442&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub473fd9d-8c03-4e78-84c7-2981c8de111&title=)
<a name="rf4fo"></a>
#### `@Bean`
虽然上面`@Component` + `@ComponentScan`的这种方式可以将Bean注入到Spring中，但是有个问题那就是对于第三方jar包来说，如果这个类没加`@Component`注解，那么`@ComponentScan`就扫不到，这样就无法注入到Spring容器中，所以Spring提供了一种`@Bean`的方式来声明Bean。<br />比如，在使用MybatisPlus的分页插件的时候，就可以按如下方式这么来声明。
```java
@Bean
public MybatisPlusInterceptor mybatisPlusInterceptor() {
    MybatisPlusInterceptor interceptor = new MybatisPlusInterceptor();
    interceptor.addInnerInterceptor(new PaginationInnerInterceptor(DbType.MYSQL));
    return interceptor;
}
```
此时就能将`MybatisPlusInterceptor`这个Bean注入到Spring容器中。
<a name="Rq6w7"></a>
#### `@Import`
`@Import`注解也可以用来将Bean注入到Spring容器中，`@Import`注解导入的类可以分为三种情况：

- 普通类
- 类实现了`ImportSelector`接口
- 类实现了`ImportBeanDefinitionRegistrar`接口
<a name="q6JOc"></a>
###### 普通类
普通类其实就很简单，就是将`@Import`导入的类注入到Spring容器中，这没什么好说的。
<a name="YeaQq"></a>
###### 类实现了`ImportSelector`接口
```java
public interface ImportSelector {

    String[] selectImports(AnnotationMetadata importingClassMetadata);

    @Nullable
    default Predicate<String> getExclusionFilter() {
        return null;
    }

}
```
当`@Import`导入的类实现了`ImportSelector`接口的时候，Spring就会调用`selectImports`方法的实现，获取一批类的全限定名，最终这些类就会被注册到Spring容器中。<br />比如如下代码中，`UserImportSelector`实现了`ImportSelector`，`selectImports`方法返回User的全限定名
```java
public class UserImportSelector implements ImportSelector {

    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        System.out.println("调用 UserImportSelector 的 selectImports 方法获取一批类限定名");
        return new String[]{"com.sanyou.spring.bean.injection.User"};
    }

}
```
当使用`@Import`注解导入`UserImportSelector`这个类的时候，其实最终就会把User注入到Spring容器中，如下测试
```java
@Import(UserImportSelector.class)
public class ImportSelectorDemo {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将 ImportSelectorDemo 注册到容器中
        applicationContext.register(ImportSelectorDemo.class);
        applicationContext.refresh();

        User user = applicationContext.getBean(User.class);
        System.out.println(user);
    }

}
```
运行结果
```
User(username=null)
```
对于类实现了`ImportBeanDefinitionRegistrar`接口的情况，这个后面说。<br />一般来说，`@Import`都是配合`@EnableXX`这类注解来使用的，比如常见的`@EnableScheduling`、`@EnableAsync`注解等，其实最终都是靠`@Import`来实现的。<br />![@EnableScheduling](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424017303-97eba3e6-ee83-44f5-9a58-fded5e55fb10.png#averageHue=%23332d2b&clientId=ubbf5f4d1-93bd-4&from=paste&id=uc611bb20&originHeight=390&originWidth=780&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u12ea69a3-2a76-4d2c-b58b-dd2464560d2&title=%40EnableScheduling "@EnableScheduling")<br />![@EnableAsync](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424017277-56fb4226-75fd-4f59-a1fa-d1de77fc119c.png#averageHue=%23322d2a&clientId=ubbf5f4d1-93bd-4&from=paste&id=u94231aac&originHeight=438&originWidth=946&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uce890210-ab67-4f1d-a656-faab9fcd218&title=%40EnableAsync "@EnableAsync")<br />讲完通过注解的方式来声明Bean之后，可以来思考一个问题，那就是既然注解方式这么简单，为什么Spring还写一堆代码来支持配置文件这种声明的方式？<br />其实答案很简单，跟Spring的发展历程有关。Spring在创建之初Java还不支持注解，所以只能通过配置文件的方式来声明Bean，在Java1.5版本开始支持注解之后，Spring才开始支持通过注解的方式来声明Bean。
<a name="gzcZZ"></a>
## 注册`BeanDefinition`
在说注册`BeanDefinition`之前，先来聊聊什么是`BeanDefinition`？<br />`BeanDefinition`是Spring Bean创建环节中很重要的一个东西，它封装了Bean创建过程中所需要的元信息。
```java
public interface BeanDefinition extends AttributeAccessor, BeanMetadataElement {
    //设置Bean className
    void setBeanClassName(@Nullable String beanClassName);

    //获取Bean className
    @Nullable
    String getBeanClassName();
    
    //设置是否是懒加载
    void setLazyInit(boolean lazyInit);

    //判断是否是懒加载
    boolean isLazyInit();
    
    //判断是否是单例
    boolean isSingleton();

}
```
如上代码是`BeanDefinition`接口的部分方法，从这方法的定义名称可以看出，一个Bean所创建过程中所需要的一些信息都可以从`BeanDefinition`中获取，比如这个Bean的class类型，这个Bean是否是懒加载，这个Bean是否是单例的等等，因为有了这些信息，Spring才知道要创建一个什么样的Bean。<br />有了`BeanDefinition`这个概念之后，再来看一下配置文件和注解声明这些方式往Spring容器注入Bean的原理。<br />![Bean注入到Spring原理](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424017316-578412fb-94ec-4809-b8fc-7c05cf841f20.png#averageHue=%233b3632&clientId=ubbf5f4d1-93bd-4&from=paste&id=u20583a1b&originHeight=271&originWidth=811&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uf68e491f-445a-4de3-a41f-48dd8f5a913&title=Bean%E6%B3%A8%E5%85%A5%E5%88%B0Spring%E5%8E%9F%E7%90%86 "Bean注入到Spring原理")<br />如图为Bean注入到Spring大致原理图，整个过程大致分为以下几个步骤

- 通过`BeanDefinitionReader`组件读取配置文件或者注解的信息，为每一个Bean生成一个`BeanDefinition`
- `BeanDefinition`生成之后，添加到`BeanDefinitionRegistry`中，`BeanDefinitionRegistry`就是用来保存`BeanDefinition`
- 当需要创建Bean对象时，会从`BeanDefinitionRegistry`中拿出需要创建的Bean对应的`BeanDefinition`，根据`BeanDefinition`的信息来生成Bean
- 当生成的Bean是单例的时候，Spring会将Bean保存到`SingletonBeanRegistry`中，也就是平时说的三级缓存中的第一级缓存中，以免重复创建，需要使用的时候直接从`SingletonBeanRegistry`中查找

好了，通过以上分析可以知道，配置文件和注解声明的方式其实都是声明Bean的一种方式，最终都会转换成`BeanDefinition`，Spring是基于`BeanDefinition`的信息来创建Bean。<br />既然Spring最终是基于`BeanDefinition`的信息来创建Bean，那么是不是可以跳过配置文件和注解声明的方式，直接通过手动创建和注册`BeanDefinition`的方式实现往Spring容器中注入呢？<br />答案是可以的。<br />前面说过，`BeanDefinition`最终会被注册到`BeanDefinitionRegistry`中，那么如何拿到`BeanDefinitionRegistry`呢？主要有以下两种方式：

- `ImportBeanDefinitionRegistrar`
- `BeanDefinitionRegistryPostProcessor`
<a name="L6BQv"></a>
#### `ImportBeanDefinitionRegistrar`
上面在说@Import的时候，关于导入的类实现了`ImportBeanDefinitionRegistrar`接口的情况没有说，主要是因为在这里说比较合适
```java
public interface ImportBeanDefinitionRegistrar {

   default void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry,BeanNameGenerator importBeanNameGenerator) {
       registerBeanDefinitions(importingClassMetadata, registry);
   }

   default void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
   }

}
```
`ImportBeanDefinitionRegistrar`中有两个方法，方法的参数就是`BeanDefinitionRegistry`。当`@Import`导入的类实现了`ImportBeanDefinitionRegistrar`接口之后，Spring就会调用`registerBeanDefinitions`方法，传入`BeanDefinitionRegistry`。<br />来个Demo<br />`UserImportBeanDefinitionRegistrar`实现`ImportBeanDefinitionRegistrar`
```java
public class UserImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {

    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry, BeanNameGenerator importBeanNameGenerator) {
        //构建一个 BeanDefinition , Bean的类型为 User
        AbstractBeanDefinition beanDefinition = BeanDefinitionBuilder.rootBeanDefinition(User.class)
                //设置User这个Bean的属性username的值为java
                .addPropertyValue("username", "java")
                .getBeanDefinition();
        //把User的BeanDefinition注入到BeanDefinitionRegistry中
        registry.registerBeanDefinition("user", beanDefinition);
    }

}
```
测试类
```java
@Import(UserImportBeanDefinitionRegistrar.class)
public class UserImportBeanDefinitionRegistrarDemo {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        applicationContext.register(UserImportBeanDefinitionRegistrarDemo.class);
        applicationContext.refresh();
        User user = applicationContext.getBean(User.class);
        System.out.println(user);
    }

}
```
结果
```
User(username=java)
```
从结果可以看出，成功将User注入到了Spring容器中。<br />上面的例子中有行代码
```java
applicationContext.register(UserImportBeanDefinitionRegistrarDemo.class);
```
这行代码的意思就是把`UserImportBeanDefinitionRegistrarDemo`这个Bean注册到Spring容器中，所以这里其实也算一种将Bean注入到Spring的方式，原理也跟上面一样，会为`UserImportBeanDefinitionRegistrarDemo`生成一个`BeanDefinition`注册到Spring容器中。
<a name="YXIPR"></a>
#### `BeanDefinitionRegistryPostProcessor`
除了`ImportBeanDefinitionRegistrar`可以拿到`BeanDefinitionRegistry`之外，还可以通过`BeanDefinitionRegistryPostProcessor`拿到`BeanDefinitionRegistry`<br />![BeanDefinitionRegistryPostProcessor](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424017866-c46ce672-e170-4c56-8b04-e9ed6300af44.png#averageHue=%23312c2b&clientId=ubbf5f4d1-93bd-4&from=paste&id=u98697024&originHeight=327&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u2022a503-353d-4529-a2d6-37e7bf8aaf8&title=BeanDefinitionRegistryPostProcessor "BeanDefinitionRegistryPostProcessor")<br />这种方式就不演示了。<br />手动注册`BeanDefinition`这种方式还是比较常见的。就比如说`OpenFeign`在启用过程中，会为每个标注了`@FeignClient`注解的接口创建一个`BeanDefinition`，然后再往Spring中的注册的，如下是`OpenFeign`注册`FeignClient`的部分代码
```java
class FeignClientsRegistrar implements ImportBeanDefinitionRegistrar, ResourceLoaderAware, EnvironmentAware {

    private void registerFeignClient(BeanDefinitionRegistry registry, AnnotationMetadata annotationMetadata, Map<String, Object> attributes) {
        //构建BeanDefinition，class类型为FeignClientFactoryBean
        BeanDefinitionBuilder definition = BeanDefinitionBuilder.genericBeanDefinition(FeignClientFactoryBean.class);
        String alias = contextId + "FeignClient";
        AbstractBeanDefinition beanDefinition = definition.getBeanDefinition();
        BeanDefinitionHolder holder = new BeanDefinitionHolder(beanDefinition, className, new String[] { alias });
        //注册BeanDefinition
        BeanDefinitionReaderUtils.registerBeanDefinition(holder, registry);
    }
}
```
<a name="yBMVo"></a>
## 注册创建完成的Bean
上一节说可以跳过配置文件或者是注解，直接通过注册`BeanDefinition`以达到将Bean注入到Spring中的目的。<br />既然已经可以跳过配置文件或者是注解，那么可不可以更激进一步，跳过注册`BeanDefinition`这一步，直接往Spring中注册一个已经创建好的Bean呢？<br />答案依然是可以的。<br />因为上面在提到当创建的Bean是单例的时候，会将这个创建完成的Bean保存到`SingletonBeanRegistry`中，需要用到直接从`SingletonBeanRegistry`中查找。既然最终是从`SingletonBeanRegistry`中查找的Bean，那么直接注入一个创建好的Bean有什么不可以呢？<br />既然可以，那么如何拿到`SingletonBeanRegistry`呢？<br />其实拿到`SingletonBeanRegistry`的方法其实很多，因为`ConfigurableListableBeanFactory`就继承了`SingletonBeanRegistry`接口，所以只要能拿到`ConfigurableListableBeanFactory`就相当于拿到了`SingletonBeanRegistry`。<br />![ConfigurableListableBeanFactory类图](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424018288-878d598e-c663-4d6c-a51a-b553e10ce12e.png#averageHue=%23312f2d&clientId=ubbf5f4d1-93bd-4&from=paste&id=u8464c782&originHeight=430&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u2b8371f7-dc02-43e7-8d43-348f0de1e4a&title=ConfigurableListableBeanFactory%E7%B1%BB%E5%9B%BE "ConfigurableListableBeanFactory类图")<br />而`ConfigurableListableBeanFactory`可以通过`BeanFactoryPostProcessor`来获取<br />![BeanFactoryPostProcessor](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424018288-6c10e6d2-d828-46bc-a091-d64ce411cc3f.png#averageHue=%232d2c2b&clientId=ubbf5f4d1-93bd-4&from=paste&id=u8b0700ed&originHeight=346&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u4409ee24-d264-4e86-acc2-4bc3e43fdae&title=BeanFactoryPostProcessor "BeanFactoryPostProcessor")<br />来个Demo<br />`RegisterUserBeanFactoryPostProcessor`实现`BeanFactoryPostProcessor`， 往Spring容器中添加一个手动创建的User对象
```java
public class RegisterUserBeanFactoryPostProcessor implements BeanFactoryPostProcessor {

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        //创建一个User对象
        User user = new User();
        user.setUsername("java");
        //将这个User对象注入到Spring容器中
        beanFactory.registerSingleton("user", user);
    }

}
```
测试
```java
public class RegisterUserDemo {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        applicationContext.register(RegisterUserBeanFactoryPostProcessor.class);
        applicationContext.refresh();
        User user = applicationContext.getBean(User.class);
        System.out.println(user);
    }

}
```
结果
```
User(username=java)
```
从结果还是可以看出，成功从Spring容器中获取到了User对象。<br />这种直接将创建好的Bean注入到Spring容器中在Spring框架内部使用的还是比较多的，Spring的一些内建的Bean就是通过这个方式注入到Spring中的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424018699-0d54f557-3807-4de7-b33f-ddf0182aff67.png#averageHue=%232e2c2b&clientId=ubbf5f4d1-93bd-4&from=paste&id=ue085ba68&originHeight=663&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u41616ca2-6cb1-4ab8-8a97-fe79b99905d&title=)<br />如上图，在SpringBoot项目启动的过程中会往Spring容器中添加两个创建好的Bean，如果程序需要使用到这些Bean，就可以通过依赖注入的方式获取到。<br />虽然基于这种方式可以将Bean注入到Spring容器，但是这种方式注入的Bean是不经过Bean的生命周期的，也就是说这个Bean中诸如`@Autowired`等注解和Bean生命周期相关的回调都不会生效的，注入到Spring时Bean是什么样就是什么样，Spring不做处理，仅仅只是做一个保存作用。
<a name="mMyJG"></a>
## FactoryBean
FactoryBean是一种特殊的Bean的类型，通过`FactoryBean`也可以将Bean注入到Spring容器中。<br />![FactoryBean](https://cdn.nlark.com/yuque/0/2023/png/396745/1676424018805-dcdcc925-5266-4d84-86e8-37c924a41df8.png#averageHue=%2330302f&clientId=ubbf5f4d1-93bd-4&from=paste&id=u0d7b0a42&originHeight=524&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u5f7b26e9-173b-485c-ad93-ec3550fbd90&title=FactoryBean "FactoryBean")<br />当通过配置文件、注解声明或者是注册`BeanDenifition`的方式，往Spring容器中注入了一个class类型为`FactoryBean`类型的Bean时候，其实真正注入的Bean类型为getObjectType方法返回的类型，并且Bean的对象是通过getObject方法返回的。<br />来个Demo<br />`UserFactoryBean`实现了`FactoryBean`，`getObjectType`返回了User类型，所以这个`UserFactoryBean`会往Spring容器中注入User这个Bean，并且User对象是通过`getObject()`方法的实现返回的。
```java
public class UserFactoryBean implements FactoryBean<User> {
    @Override
    public User getObject() throws Exception {
        User user = new User();
        user.setUsername("java");
        return user;
    }

    @Override
    public Class<?> getObjectType() {
        return User.class;
    }
}
```
测试
```java
public class UserFactoryBeanDemo {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将UserFactoryBean注入到Spring容器中
        applicationContext.register(UserFactoryBean.class);
        applicationContext.refresh();
        User user = applicationContext.getBean(User.class);
        System.out.println(user);
    }

}
```
结果
```
User(username=java)
```
成功通过`UserFactoryBean`将User这个Bean注入到Spring容器中了。<br />FactoryBean这中注入的方式使用也是非常多的，就拿上面举例的`OpenFeign`来说，`OpenFeign`为每个`FeignClient`的接口创建的`BeanDefinition`的Bean的class类型`FeignClientFactoryBean`就是`FactoryBean`的实现。
```java
class FeignClientFactoryBean implements FactoryBean<Object>, InitializingBean, ApplicationContextAware {
    
    // FeignClient接口类型
    private Class<?> type;
    
    @Override
    public Object getObject() throws Exception {
       return getTarget();
    }
    
    @Override
    public Class<?> getObjectType() {
       return type;
    }
}
```
`getObject()`方法就会返回接口的动态代理的对象，并且这个代理对象是由Feign创建的，这也就实现了Feign和Spring的整合。
<a name="ZHvER"></a>
## 总结
通过以上分析可以看出，将Bean注入到Spring容器中大致可以分为5类：

- 配置文件
- 注解声明
- 注册`BeanDefinition`
- 注册创建完成的Bean
- `FactoryBean`

以上几种注入的方式，在日常业务开发中，基本上都是使用注解声明的方式注入Spring中的；在第三方框架在和Spring整合时，注册`BeanDefinition`和`FactoryBean`这些注入方式也会使用的比较多；至于配置文件和注册创建完成的Bean的方式，有但是不多。
