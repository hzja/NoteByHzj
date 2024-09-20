JavaSpringBoot<br />Spring对于每个Java后端程序员来说肯定不陌生，日常开发和面试必备的。本文就来盘点Spring/SpringBoot常见的扩展点，同时也来看看常见的开源框架是如何基于这些扩展点跟Spring/SpringBoot整合的。
<a name="JpkYi"></a>
## `FactoryBean`
提起`FactoryBean`，就有一道“著名”的面试题“说一说`FactoryBean`和`BeanFactory`的区别”。其实这两者除了名字有点像，没有半毛钱关系。<br />`BeanFactory`是Bean的工厂，可以生成想要的Bean，而FactoryBean就是一种Bean的类型。当往容器中注入class类型为`FactoryBean`的类型的时候，最终生成的Bean是用过`FactoryBean`的`getObject`获取的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883774899-22950008-13c3-4592-9b14-78da6bab7fd2.png#averageHue=%23f5f3ef&clientId=ud7d136fd-1fc1-4&from=paste&id=uf1cc14de&originHeight=252&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u99f4d615-1288-45a0-94da-fef54561c6f&title=)
<a name="cYAqA"></a>
### 来个`FactoryBean`的Demo
定义一个`UserFactoryBean`，实现`FactoryBean`接口，`getObject`方法返回一个User对象
```java
public class UserFactoryBean implements FactoryBean<User> {

    @Override
    public User getObject() throws Exception {
        User user = new User();
        System.out.println("调用 UserFactoryBean 的 getObject 方法生成 Bean:" + user);
        return user;
    }

    @Override
    public Class<?> getObjectType() {
        // 这个 FactoryBean 返回的Bean的类型
        return User.class;
    }

}
```
测试类：
```java
public class Application {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将 UserFactoryBean 注册到容器中
        applicationContext.register(UserFactoryBean.class);
        applicationContext.refresh();

        System.out.println("获取到的Bean为" + applicationContext.getBean(User.class));
    }

}
```
结果：
```java
调用 UserFactoryBean 的 getObject 方法生成 Bean:com.sanyou.spring.extension.User@396e2f39
获取到的Bean为com.sanyou.spring.extension.User@396e2f39
```
从结果可以看出，明明注册到Spring容器的是`UserFactoryBean`，但是却能从容器中获取到User类型的Bean，User这个Bean就是通过`UserFactoryBean`的`getObject`方法返回的。
<a name="heAcn"></a>
### `FactoryBean`在开源框架中的使用
<a name="SV2OQ"></a>
#### 1、 在Mybatis中的使用
Mybatis在整合Spring的时候，就是通过`FactoryBean`来实现的，这也就是为什么在Spring的Bean中可以注入Mybatis的`Mapper`接口的动态代理对象的原因。<br />代码如下，省略了不重要的代码。
```java
public class MapperFactoryBean<T> extends SqlSessionDaoSupport implements FactoryBean<T> {

	// mapper的接口类型
	private Class<T> mapperInterface;

	@Override
	public T getObject() throws Exception {
		// 通过SqlSession获取接口的动态搭理对象
		return getSqlSession().getMapper(this.mapperInterface);
	}

	@Override
	public Class<T> getObjectType() {
		return this.mapperInterface;
	}

}
```
`getObject`方法的实现就是返回通过`SqlSession`获取到的`Mapper`接口的动态代理对象。<br />而`@MapperScan`注解的作用就是将每个接口对应的`MapperFactoryBean`注册到Spring容器的。
<a name="GDHrH"></a>
#### 2、在`OpenFeign`中的使用
`FeignClient`接口的动态代理也是通过`FactoryBean`注入到Spring中的。
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
`getObject`方法是调用`getTarget`方法来返回的动态代理。<br />`@EnableFeignClients`注解的作用就是将每个接口对应的`FeignClientFactoryBean`注入到Spring容器的。<br />**一般来说，**`**FactoryBean**`** 比较适合那种复杂Bean的构建，在其他框架整合Spring的时候用的比较多。**
<a name="OhDnZ"></a>
## `@Import`注解
`@Import`注解在项目中可能不常见，但是下面这两个注解肯定常见。
```java
@Import({SchedulingConfiguration.class})
public @interface EnableScheduling {
}

@Import({AsyncConfigurationSelector.class})
public @interface EnableAsync {
    //忽略
}
```
`@EnableScheduling`和`@EnableAsync`两个注解，一个是开启定时任务，一个是开启异步执行。通过这两个注解可以看出，他们都使用了`@Import`注解，所以真正起作用的是`@Import`注解。并且在很多情况下，`@EnbaleXXX`这种格式的注解，都是通过`@Import`注解起作用的，代表开启了某个功能。
<a name="qq5DB"></a>
### `@Import`注解导入的配置类的分类
`@Import`注解导入的配置类可以分为三种情况：
<a name="jnTvB"></a>
#### 第一种：配置类实现了 ImportSelector 接口
```java
public interface ImportSelector {

   String[] selectImports(AnnotationMetadata importingClassMetadata);

   @Nullable
   default Predicate<String> getExclusionFilter() {
      return null;
   }

}
```
当配置类实现了 `ImportSelector` 接口的时候，就会调用 selectImports 方法的实现，获取一批类的全限定名，最终这些类就会被注册到Spring容器中。<br />`UserImportSelector`实现了`ImportSelector`，selectImports方法返回User的全限定名，代表吧User这个类注册容器中
```java
public class UserImportSelector implements ImportSelector {

    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        System.out.println("调用 UserImportSelector 的 selectImports 方法获取一批类限定名");
        return new String[]{"com.sanyou.spring.extension.User"};
    }

}
```
测试：
```java
// @Import 注解导入 UserImportSelector
@Import(UserImportSelector.class)
public class Application {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将 Application 注册到容器中
        applicationContext.register(Application.class);
        applicationContext.refresh();

        System.out.println("获取到的Bean为" + applicationContext.getBean(User.class));
    }

}
```
结果：
```java
调用 UserImportSelector 的 selectImports 方法获取一批类限定名
获取到的Bean为com.sanyou.spring.extension.User@282003e1
```
所以可以看出，的确成功往容器中注入了User这个Bean
<a name="vhRly"></a>
#### 第二种：配置类实现了 `ImportBeanDefinitionRegistrar` 接口
```java
public interface ImportBeanDefinitionRegistrar {

   default void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry,BeanNameGenerator importBeanNameGenerator) {
       registerBeanDefinitions(importingClassMetadata, registry);
   }

   default void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
   }

}
```
当配置类实现了 `ImportBeanDefinitionRegistrar` 接口，就可以自定义往容器中注册想注入的Bean。这个接口相比与 `ImportSelector` 接口的主要区别就是，`ImportSelector`接口是返回一个类，不能对这个类进行任何操作，但是 `ImportBeanDefinitionRegistrar` 是可以自己注入 `BeanDefinition`，可以添加属性之类的。<br />来个demo：<br />实现`ImportBeanDefinitionRegistrar`接口
```java
public class UserImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {

    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry, BeanNameGenerator importBeanNameGenerator) {
        //构建一个 BeanDefinition , Bean的类型为 User
        AbstractBeanDefinition beanDefinition = BeanDefinitionBuilder.rootBeanDefinition(User.class)
                // 设置 User 这个Bean的属性username的值为Tom
                .addPropertyValue("username", "Tom")
                .getBeanDefinition();

        System.out.println("往Spring容器中注入User");
        //把 User 这个Bean的定义注册到容器中
        registry.registerBeanDefinition("user", beanDefinition);
    }

}
```
测试：
```java
// 导入 UserImportBeanDefinitionRegistrar
@Import(UserImportBeanDefinitionRegistrar.class)
public class Application {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将 Application 注册到容器中
        applicationContext.register(Application.class);
        applicationContext.refresh();

        User user = applicationContext.getBean(User.class);
        System.out.println("获取到的Bean为" + user + "，属性username值为：" + user.getUsername());
    }

}
```
结果：
```java
往Spring容器中注入User
获取到的Bean为com.sanyou.spring.extension.User@6385cb26，属性username值为：Tom
```
<a name="gQgib"></a>
#### 第三种：配置类什么接口都没实现
这种就不演示了，就是一个普普通通的类。
<a name="gUDpS"></a>
### 总结
![@Import注解作用示意图](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883774885-cf02428f-11d5-439a-bbd0-054702ba4899.png#averageHue=%23f5f4f2&clientId=ud7d136fd-1fc1-4&from=paste&id=u5e5d1e7f&originHeight=488&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubf1c3277-186e-4abc-93d0-1e099f4c3d2&title=%40Import%E6%B3%A8%E8%A7%A3%E4%BD%9C%E7%94%A8%E7%A4%BA%E6%84%8F%E5%9B%BE "@Import注解作用示意图")<br />其实不论是什么样的配置类，主要的作用就是往Spring容器中注册Bean，只不过注入的方式不同罢了。<br />这种方式有什么好处呢？<br />`ImportSelector`和`ImportBeanDefinitionRegistrar`的方法是有入参的，也就是注解的一些属性的封装，所以就可以根据注解的属性的配置，来决定应该返回样的配置类或者是应该往容器中注入什么样的类型的Bean，可以看一下 `@EnableAsync` 的实现，看看是如何根据`@EnableAsync`注解的属性来决定往容器中注入什么样的Bean。<br />`**@Import**`**的核心作用就是导入配置类，并且还可以根据配合（比如**`**@EnableXXX**`**）使用的注解的属性来决定应该往Spring中注入什么样的Bean。**
<a name="wGQKs"></a>
## Bean的生命周期
第一节说的`FactoryBean`是一种特殊的Bean的类型，`@Import`注解是往Spring容器中注册Bean。其实不论是`@Import`注解，还是`@Component`、`@Bean`等注解，又或是xml配置，甚至是demo中的register方法，其实主要都是做了一件事，那就是往Spring容器去注册Bean。<br />![Bean注册示意图](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883774863-698a786e-991e-4e36-a53f-16c9a88fa841.png#averageHue=%23f5f2ee&clientId=ud7d136fd-1fc1-4&from=paste&id=u0fc73753&originHeight=511&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ub64e2a0b-3fd3-4318-ba42-4ed0421b8fc&title=Bean%E6%B3%A8%E5%86%8C%E7%A4%BA%E6%84%8F%E5%9B%BE "Bean注册示意图")<br />为什么需要去注册Bean？<br />当然是为了让Spring知道要生成Bean，并且需要按照要求来生成Bean，比如说，要`@Autowired`一个对象，那么在创建Bean的过程中，就得给`@Autowired`一个对象，这就是一个IOC的过程。所以这就涉及了Bean的创建，销毁的过程，也就是面试常问的Bean的生命周期。<br />本节来着重看一下，一个Bean在创建的过程中，有哪些常见的操作Spring在Bean的创建过程中完成，并且操作的顺序是什么样的。<br />话不多说，直接测试，基于结果来分析。
<a name="FoiVT"></a>
### Bean生命周期的回调
<a name="kljP9"></a>
#### 先来测试
<a name="UXcVn"></a>
##### 创建`LifeCycle`类
创建了一个`LifeCycle`，实现了 `InitializingBean`、`ApplicationContextAware`、`DisposableBean`接口，加了`@PostConstruct`、`@PreDestroy`注解，注入了一个User对象。
```java
public class LifeCycle implements InitializingBean, ApplicationContextAware, DisposableBean {

    @Autowired
    private User user;

    public LifeCycle() {
        System.out.println("LifeCycle对象被创建了");
    }

    /**
     * 实现的 Aware 回调接口
     *
     * @param applicationContext
     * @throws BeansException
     */
    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        System.out.println("Aware接口起作用，setApplicationContext被调用了，此时user=" + user);
    }

    @PostConstruct
    public void postConstruct() {
        System.out.println("@PostConstruct注解起作用，postConstruct方法被调用了");
    }

    /**
     * 实现 InitializingBean 接口
     *
     * @throws Exception
     */
    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("InitializingBean接口起作用，afterPropertiesSet方法被调用了");
    }

    /**
     * 通过 {@link Bean#initMethod()}来指定
     *
     * @throws Exception
     */
    public void initMethod() throws Exception {
        System.out.println("@Bean#initMethod()起作用，initMethod方法被调用了");
    }

    @PreDestroy
    public void preDestroy() throws Exception {
        System.out.println("@PreDestroy注解起作用，preDestroy方法被调用了");
    }

    /**
     * 通过 {@link Bean#destroyMethod()}来指定
     *
     * @throws Exception
     */
    public void destroyMethod() throws Exception {
        System.out.println("@Bean#destroyMethod()起作用，destroyMethod方法被调用了");
    }

    /**
     * 实现 DisposableBean 注解
     *
     * @throws Exception
     */
    @Override
    public void destroy() throws Exception {
        System.out.println("DisposableBean接口起作用，destroy方法被调用了");
    }

}
```
<a name="JXWLN"></a>
##### 声明`LifeCycle`
通过`@Bean`声明了`LifeCycle`，并且`initMethod`和`destroyMethod`属性分别指定到了`LifeCycle`类的`initMethod`方法和`destroyMethod`方法
```java
@Bean(initMethod = "initMethod", destroyMethod = "destroyMethod")
public LifeCycle lifeCycle() {
    return new LifeCycle();
}
```
<a name="Y6sfK"></a>
##### 测试
```java
public class Application {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将 LifeCycle 注册到容器中
        applicationContext.register(Application.class);
        applicationContext.refresh();

        // 关闭上下文，触发销毁操作
        applicationContext.close();
    }

    @Bean(initMethod = "initMethod", destroyMethod = "destroyMethod")
    public LifeCycle lifeCycle() {
        return new LifeCycle();
    }

    @Bean
    public User user() {
        return new User();
    }

}
```
执行结果：
```java
LifeCycle对象被创建了
Aware接口起作用，setApplicationContext被调用了，此时user=com.sanyou.spring.extension.User@57d5872c
@PostConstruct注解起作用，postConstruct方法被调用了
InitializingBean接口起作用，afterPropertiesSet方法被调用了
@Bean#initMethod()起作用，initMethod方法被调用了
@PreDestroy注解起作用，preDestroy方法被调用了
DisposableBean接口起作用，destroy方法被调用了
@Bean#destroyMethod()起作用，destroyMethod方法被调用了
```
<a name="q3lz4"></a>
#### 分析结果
通过测试的结果可以看出，Bean在创建和销毁的过程当实现了某些接口或者加了某些注解，Spring就会回调实现的接口或者执行的方法。<br />同时，在执行`setApplicationContext`的时候，能打印出User对象，说明User已经被注入了，说明注入发生在`setApplicationContext`之前。<br />这里画张图总结一下Bean创建和销毁过程中调用的顺序。<br />![回调顺序](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883774836-a2f1f1ba-6a27-438d-a784-c6f2482d25f7.png#averageHue=%23ede4e5&clientId=ud7d136fd-1fc1-4&from=paste&id=u4e745eb9&originHeight=910&originWidth=992&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u25626263-9042-4c9a-bfe7-f9394ce4ea0&title=%E5%9B%9E%E8%B0%83%E9%A1%BA%E5%BA%8F "回调顺序")<br />红色部分发生在Bean的创建过程，灰色部分发生在Bean销毁的过程中，在容器关闭的时候，就会销毁Bean。<br />这里说一下图中的`Aware`接口指的是什么。其余的其实没什么好说的，就是按照这种方式配置，Spring会调用对应的方法而已。<br />`Aware`接口是指以`Aware`结尾的一些Spring提供的接口，当Bean实现了这些接口的话，在创建过程中会回调对应的set方法，并传入响应的对象。<br />这里列举几个`Aware`接口以及它们的作用

| **接口** | **作用** |
| --- | --- |
| `ApplicationContextAware` | 注入`ApplicationContext` |
| `ApplicationEventPublisherAware` | 注入`ApplicationEventPublisher`事件发布器 |
| `BeanFactoryAware` | 注入`BeanFactory` |
| `BeanNameAware` | 注入Bean的名称 |

有了这些回调，比如说Bean想拿到`ApplicationContext`，不仅可以通过`@Autowired`注入，还可以通过实现`ApplicationContextAware`接口拿到。<br />通过上面的例子知道了比如说`@PostConstruct`注解、`@Autowired`注解、`@PreDestroy`注解的作用，但是它们是如何在不同的阶段实现的呢？接着往下看。
<a name="ivQLe"></a>
## BeanPostProcessor
`BeanPostProcessor`，中文名 Bean的后置处理器，在Bean创建的过程中起作用。<br />`BeanPostProcessor`是Bean在创建过程中一个非常重要的扩展点，因为每个Bean在创建的各个阶段，都会回调`BeanPostProcessor`及其子接口的方法，传入正在创建的Bean对象，这样如果想对Bean创建过程中某个阶段进行自定义扩展，那么就可以自定义`BeanPostProcessor`来完成。<br />说得简单点，`BeanPostProcessor`就是在Bean创建过程中留的口子，通过这个口子可以对正在创建的Bean进行扩展。只不过Bean创建的阶段比较多，然后`BeanPostProcessor`接口以及他的子接口`InstantiationAwareBeanPostProcessor`、`DestructionAwareBeanPostProcessor`就提供了很多方法，可以使得在不同的阶段都可以拿到正在创建的Bean进行扩展。
<a name="rAQaD"></a>
### 来个Demo
现在需要实现一个这样的需求，如果Bean的类型是User，那么就设置这个对象的username属性为 ”Tom“。<br />那么就可以这么写：
```java
public class UserBeanPostProcessor implements BeanPostProcessor {

    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        if (bean instanceof User) {
            //如果当前的Bean的类型是 User ，就把这个对象 username 的属性赋值为 Tom
            ((User) bean).setUsername("Tom");
        }

        return bean;
    }

}
```
测试：
```java
public class Application {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将 UserBeanPostProcessor 和  User 注册到容器中
        applicationContext.register(UserBeanPostProcessor.class);
        applicationContext.register(User.class);
        applicationContext.refresh();

        User user = applicationContext.getBean(User.class);
        System.out.println("获取到的Bean为" + user + "，属性username值为：" + user.getUsername());
    }

}
```
测试结果：
```java
获取到的Bean为com.sanyou.spring.extension.User@21a947fe，属性username值为：Tom
```
从结果可以看出，每个生成的Bean在执行到某个阶段的时候，都会回调`UserBeanPostProcessor`，然后`UserBeanPostProcessor`就会判断当前创建的Bean的类型，如果是User类型，那么就会将username的属性设置为 ”Tom“。
<a name="F41Xp"></a>
### Spring内置的`BeanPostProcessor`
这里列举了常见的一些`BeanPostProcessor`的实现以及它们的作用

| **BeanPostProcessor** | **作用** |
| --- | --- |
| `AutowiredAnnotationBeanPostProcessor` | 处理`@Autowired`、`@Value`注解 |
| `CommonAnnotationBeanPostProcessor` | 处理`@Resource`、`@PostConstruct`、`@PreDestroy`注解 |
| `AnnotationAwareAspectJAutoProxyCreator` | 处理一些注解或者是AOP切面的动态代理 |
| `ApplicationContextAwareProcessor` | 处理`Aware`接口注入的 |
| `AsyncAnnotationBeanPostProcessor` | 处理`@Async`注解 |
| `ScheduledAnnotationBeanPostProcessor` | 处理`@Scheduled`注解 |

通过列举的这些`BeanPostProcessor`的实现可以看出，Spring Bean的很多注解的处理都是依靠`BeanPostProcessor`及其子类的实现来完成的，这也回答了上一小节的疑问，处理`@Autowired`、`@PostConstruct`、`@PreDestroy`注解是如何起作用的，其实就是通过`BeanPostProcessor`，在Bean的不同阶段来调用对应的方法起作用的。
<a name="XWywd"></a>
### `BeanPostProcessor`在Dubbo中的使用
在Dubbo中可以通过`@DubboReference`(`@Reference`)来引用生产者提供的接口，这个注解的处理也是依靠`ReferenceAnnotationBeanPostProcessor`，也就是 `BeanPostProcessor` 的扩展来实现的。
```java
public class ReferenceAnnotationBeanPostProcessor 
       extends AbstractAnnotationBeanPostProcessor 
       implements ApplicationContextAware, BeanFactoryPostProcessor {
        // 忽略
}
```
当Bean在创建的某一阶段，走到了`ReferenceAnnotationBeanPostProcessor`这个类，就会根据反射找出这个类有没有`@DubboReference`(`@Reference`)注解，有的话就构建一个动态搭理注入就可以了。<br />`**BeanPostProcessor**`**在Spring Bean的扩展中扮演着重要的角色，是Spring Bean生命周期中很重要的一部分。正是因为有了**`**BeanPostProcessor**`**，就可以在Bean创建过程中的任意一个阶段扩展自己想要的东西。**
<a name="o16f4"></a>
## `BeanFactoryPostProcessor`
通过上面一节知道 `BeanPostProcessor` 是对Bean的处理，那么`BeanFactoryPostProcessor`很容易就猜到是对`BeanFactory`，也就是Spring容器的处理。<br />举个例子，如果想禁止循环依赖，那么就可以这么写。
```java
public class MyBeanFactoryPostProcessor implements BeanFactoryPostProcessor {

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        // 禁止循环依赖
        ((DefaultListableBeanFactory) beanFactory).setAllowCircularReferences(false);
    }

}
```
后面只需要将注入到Spring容器中就会生效。<br />`**BeanFactoryPostProcessor**`**是可以对Spring容器做处理的，方法的入参就是Spring的容器，通过这个接口，就对容器进行为所欲为的操作。**
<a name="YNlH1"></a>
## Spring SPI机制
SPI全称为 (Service Provider Interface)，是一种动态替换发现的机制，一种解耦非常优秀的思想，SPI可以很灵活的让接口和实现分离， 让api提供者只提供接口，第三方来实现，然后可以使用配置文件的方式来实现替换或者扩展，在框架中比较常见，提高框架的可扩展性。<br />JDK有内置的SPI机制的实现`ServiceLoader`，Dubbo也有自己的SPI机制的实现`ExtensionLoader`，但是这里都不讲。。<br />这里着重看一下Spring的SPI机制的实现`SpringFactoriesLoader`。
<a name="PsXgK"></a>
### `SpringFactoriesLoader`
Spring的SPI机制规定，配置文件必须在classpath路径下的META-INF文件夹内，文件名必须为spring.factories，文件内容为键值对，一个键可以有多个值，只需要用逗号分割就行，同时键值都需要是类的全限定名。但是键和值可以没有任何关系，当然想有也可以有。
<a name="yCcgA"></a>
### show me the code
这里自定义一个类，`MyEnableAutoConfiguration`作为键，值就是User
```java
public class MyEnableAutoConfiguration {
}
```
spring.factories文件
```
com.sanyou.spring.extension.spi.MyEnableAutoConfiguration=com.sanyou.spring.extension.User
```
然后放在META-INF底下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883775240-f62353ae-034f-4846-a72f-75c1711463b7.png#averageHue=%23f6f6f5&clientId=ud7d136fd-1fc1-4&from=paste&id=u9d72b795&originHeight=148&originWidth=518&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c19c0a6-dfdd-4f20-b37c-efe37ec9c37&title=)<br />测试：
```java
public class Application {
	
	public static void main(String[] args) {
		List<String> classNames = SpringFactoriesLoader.loadFactoryNames(MyEnableAutoConfiguration.class, MyEnableAutoConfiguration.class.getClassLoader());
		classNames.forEach(System.out::println);
	}
	
}
```
结果：
```java
com.sanyou.spring.extension.User
```
可以看出，通过`SpringFactoriesLoader`的确可以从spring.factories文件中拿到`MyEnableAutoConfiguration`键对应的值。<br />到这可能说会，这SPI机制也没啥用啊。的确，这个例子比较简单，拿到就是遍历，但是在Spring中，如果Spring在加载类的话使用SPI机制，那就可以扩展，接着往下看。
<a name="h3vxZ"></a>
## SpringBoot启动扩展点
SpringBoot项目在启动的过程中有很多扩展点，这里就来盘点一下几个常见的扩展点。
<a name="ktaf8"></a>
### 1、自动装配
说到SpringBoot的扩展点，第一时间肯定想到的就是自动装配机制，面试贼喜欢问，但是其实就是一个很简单的东西。当项目启动的时候，会去从所有的spring.factories文件中读取`@EnableAutoConfiguration`键对应的值，拿到配置类，然后根据一些条件判断，决定哪些配置可以使用，哪些不能使用。<br />spring.factories文件？键值？不错，自动装配说白了就是SPI机制的一种运用场景。<br />`@EnableAutoConfiguration`注解：
```java
@Import(AutoConfigurationImportSelector.class)
public @interface EnableAutoConfiguration {
    //忽略
}
```
这个注解也是使用`@Import`注解，而且配置类还实现了`ImportSelector`接口，跟前面也都对上了。在SpringBoot中，`@EnableAutoConfiguration`是通过`@SpringBootApplication`来使用的。<br />在`AutoConfigurationImportSelector`中还有这样一段代码![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883775232-d689d618-3cd9-41c7-ac9e-03ae72c59c44.png#averageHue=%23332d2b&clientId=ud7d136fd-1fc1-4&from=paste&id=uccf9df62&originHeight=311&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6240e9a1-2622-4dc6-9802-888447403e4&title=)<br />所以，这段代码也明显地可以看出，自动装配也是基于SPI机制实现的。<br />那么想实现自动装配怎么办呢？很简单，只需两步。<br />第一步，写个配置类：
```java
@Configuration
public class UserAutoConfiguration {

    @Bean
    public UserFactoryBean userFactoryBean() {
        return new UserFactoryBean();
    }

}
```
这里为了跟前面的知识有关联，配置了一个`UserFactoryBean`。<br />第二步，往spring.factories文件配置一下
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
com.sanyou.spring.extension.springbootextension.UserAutoConfiguration
```
到这就已经实现了自动装配的扩展。<br />接下来进行测试：
```java
@SpringBootApplication
public class Application {
	
	public static void main(String[] args) {
		ConfigurableApplicationContext applicationContext = SpringApplication.run(Application.class);
		
		User user = applicationContext.getBean(User.class);
		
		System.out.println("获取到的Bean为" + user);
	}
	
}
```
运行结果：
```java
调用 UserFactoryBean 的 getObject 方法生成 Bean:com.sanyou.spring.extension.User@3406472c
获取到的Bean为com.sanyou.spring.extension.User@3406472c
```
从运行结果可以看出，自动装配起了作用，并且虽然往容器中注入的Bean的class类型为`UserFactoryBean`，但是最终会调用`UserFactoryBean`的`getObject`的实现获取到User对象。<br />自动装配机制是SpringBoot的一个很重要的扩展点，很多框架在整合SpringBoot的时候，也都通过自动装配来的，实现项目启动，框架就自动启动的，这里举个Mybatis整合SpringBoot。<br />![Mybatis整合SpringBoot的spring.factories文件](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883775304-21685deb-3e61-4972-a393-d14480b1232c.png#averageHue=%23312f2d&clientId=ud7d136fd-1fc1-4&from=paste&id=u6655cfa6&originHeight=241&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubd3ed88c-2b47-4b3a-b7e4-699cee6e2f4&title=Mybatis%E6%95%B4%E5%90%88SpringBoot%E7%9A%84spring.factories%E6%96%87%E4%BB%B6 "Mybatis整合SpringBoot的spring.factories文件")
<a name="MLCh5"></a>
### 2、`PropertySourceLoader`
<a name="uOafH"></a>
#### `PropertySourceLoader`，这是干啥的呢？
都知道，在SpringBoot环境下，外部化的配置文件支持properties和yaml两种格式。但是，现在不想使用properties和yaml格式的文件，想使用json格式的配置文件，怎么办？<br />当然是基于该小节讲的`PropertySourceLoader`来实现的。
```java
public interface PropertySourceLoader {

   //可以支持哪种文件格式的解析
   String[] getFileExtensions();

   // 解析配置文件，读出内容，封装成一个PropertySource<?>结合返回回去
   List<PropertySource<?>> load(String name, Resource resource) throws IOException;

}
```
对于`PropertySourceLoader`的实现，SpringBoot两个实现<br />`PropertiesPropertySourceLoader`：可以解析properties或者xml结尾的配置文件![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883775433-c2eca147-016c-499d-ad85-cceae93ce757.png#averageHue=%232d2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=ub12bd5f6&originHeight=731&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8861a2bb-95a5-4347-9369-372dec1eed6&title=)<br />`YamlPropertySourceLoader`：解析以yml或者yaml结尾的配置文件![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883775578-3b3f5d98-501c-4966-ac11-2cfb577e9268.png#averageHue=%232c2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u5fdf08cf&originHeight=715&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue1f590b6-194e-4f1e-aa37-f7251b15fe3&title=)<br />所以可以看出，要想实现json格式的支持，只需要自己实现可以用来解析json格式的配置文件的`PropertySourceLoader`就可以了。<br />动手来一个。
<a name="nxdM6"></a>
#### 实现可以读取json格式的配置文件
实现这个功能，只需要两步就可以了。
<a name="TaWA5"></a>
##### 第一步：自定义一个`PropertySourceLoader`
`JsonPropertySourceLoader`，实现`PropertySourceLoader`接口
```java
public class JsonPropertySourceLoader implements PropertySourceLoader {

    @Override
    public String[] getFileExtensions() {
        //这个方法表明这个类支持解析以json结尾的配置文件
        return new String[]{"json"};
    }

    @Override
    public List<PropertySource<?>> load(String name, Resource resource) throws IOException {

        ReadableByteChannel readableByteChannel = resource.readableChannel();

        ByteBuffer byteBuffer = ByteBuffer.allocate((int) resource.contentLength());

        //将文件内容读到 ByteBuffer 中
        readableByteChannel.read(byteBuffer);
        //将读出来的字节转换成字符串
        String content = new String(byteBuffer.array());
        // 将字符串转换成 JSONObject
        JSONObject jsonObject = JSON.parseObject(content);

        Map<String, Object> map = new HashMap<>(jsonObject.size());
        //将 json 的键值对读出来，放入到 map 中
        for (String key : jsonObject.keySet()) {
            map.put(key, jsonObject.getString(key));
        }

        return Collections.singletonList(new MapPropertySource("jsonPropertySource", map));
    }

}
```
<a name="NkaUF"></a>
##### 第二步：配置`PropertySourceLoader`
`JsonPropertySourceLoader`已经有了，那么怎么用呢？当然是SPI机制了，SpringBoot对于配置文件的处理，就是依靠SPI机制，这也是能扩展的重要原因。<br />![SPI机制加载PropertySourceLoader实现](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883775585-b87c6913-243f-4837-a586-6ace44a95337.png#averageHue=%23322c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=ufaac8a49&originHeight=258&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u8fbb970f-3d08-4152-af10-5005fdb1e61&title=SPI%E6%9C%BA%E5%88%B6%E5%8A%A0%E8%BD%BDPropertySourceLoader%E5%AE%9E%E7%8E%B0 "SPI机制加载PropertySourceLoader实现")<br />![spring.factories文件配置PropertySourceLoader](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883775754-a2774012-9d58-4207-8fff-5af91add1403.png#averageHue=%23362f2d&clientId=ud7d136fd-1fc1-4&from=paste&id=ua3bd4fa8&originHeight=378&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubcd8dc85-0203-4a42-b866-9e047a123f4&title=spring.factories%E6%96%87%E4%BB%B6%E9%85%8D%E7%BD%AEPropertySourceLoader "spring.factories文件配置PropertySourceLoader")<br />SpringBoot会先通过SPI机制加载所有`PropertySourceLoader`，然后遍历每个`PropertySourceLoader`，判断当前遍历的`PropertySourceLoader`，通过`getFileExtensions`获取到当前`PropertySourceLoader`能够支持哪些配置文件格式的解析，让后跟当前需要解析的文件格式进行匹配，如果能匹配上，那么就会使用当前遍历的`PropertySourceLoader`来解析配置文件。<br />`PropertySourceLoader`其实就属于策略接口，配置文件的解析就是策略模式的运用。<br />所以，只需要按照这种格式，在spring.factories文件中配置一下就行了。
```
org.springframework.boot.env.PropertySourceLoader=\
com.sanyou.spring.extension.springbootextension.propertysourceloader.JsonPropertySourceLoader
```
到此，其实就扩展完了，接下来就来测试一下。
<a name="Ffxeh"></a>
#### 测试
先创建一个application.json的配置文件
```json
{
	"sample.username": "Tom"
}
```
改造User
```java
public class User {
	// 注入配置文件的属性
	@Value("${sample.username:}")
	private String username;
}
```
启动项目
```java
@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        ConfigurableApplicationContext applicationContext = SpringApplication.run(Application.class);

        User user = applicationContext.getBean(User.class);

        System.out.println("获取到的Bean为" + user + "，属性username值为：" + user.getUsername());
    }


    @Bean
    public User user() {
        return new User();
    }

}
```
运行结果：
```java
获取到的Bean为com.sanyou.spring.extension.User@481ba2cf，属性username值为：Tom
```
成功将json配置文件的属性注入到User对象中。<br />至此，SpringBoot就支持了以json为结尾的配置文件格式。
<a name="ajANz"></a>
#### Nacos对于`PropertySourceLoader`的实现
如果项目正在用Nacos作为配置中心，那么刚刚好，Nacos已经实现json配置文件格式的解析。<br />![Nacos对于PropertySourceLoader的实现](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883775978-66a6238d-27a9-417d-a7b7-f50104f22c76.png#averageHue=%23342e2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u3cb7fdc4&originHeight=321&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u224e59f2-283e-4364-bf88-013c3e9adfd&title=Nacos%E5%AF%B9%E4%BA%8EPropertySourceLoader%E7%9A%84%E5%AE%9E%E7%8E%B0 "Nacos对于PropertySourceLoader的实现")<br />Nacos不仅实现了json格式的解析，也实现了关于xml格式的配置文件的解析，并且优先级会比SpringBoot默认的xml格式文件解析的优先级高。至于Nacos为啥需要实现PropertySourceLoader？其实很简单，因为Nacos作为配置中心，不仅支持properties和yaml格式的文件，还支持json格式的配置文件，那么客户端拿到这些配置就需要解析，SpringBoot已经支持了properties和yaml格式的文件的解析，那么Nacos只需要实现SpringBoot不支持的就可以了。
<a name="CJQAQ"></a>
### 3、`ApplicationContextInitializer`
`ApplicationContextInitializer`也是SpringBoot启动过程的一个扩展点。<br />![ApplicationContextInitializer](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776129-f4d2a993-6a78-4039-9138-119af3fb7237.png#averageHue=%232c2b2b&clientId=ud7d136fd-1fc1-4&from=paste&id=uc031cca1&originHeight=234&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0c33a094-9dab-4f85-bd92-df2e8f0e190&title=ApplicationContextInitializer "ApplicationContextInitializer")<br />在SpringBoot启动过程，会回调这个类的实现`initialize`方法，传入`ConfigurableApplicationContext`。<br />那怎么用呢？<br />依然是SPI。<br />![SPI加载ApplicationContextInitializer](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776155-96a18ac6-a82b-47fc-bb7c-e01a11bd58e1.png#averageHue=%23322d2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u054d5160&originHeight=301&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3f52b88d-9a1f-4a58-8428-c47b09356f4&title=SPI%E5%8A%A0%E8%BD%BDApplicationContextInitializer "SPI加载ApplicationContextInitializer")<br />然后遍历所有的实现，依次调用<br />![调用initialize](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776225-5cc7ac85-2492-48bf-ac1d-5f3f5e4a0f1d.png#averageHue=%232d2c2c&clientId=ud7d136fd-1fc1-4&from=paste&id=u0238d5e2&originHeight=281&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucc28e98e-f59a-4fc9-a4fb-17bf79f18ee&title=%E8%B0%83%E7%94%A8initialize "调用initialize")<br />这里就不演示了，实现接口，按照如下这种配置就行了<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776174-3b5b9f1b-251e-4154-b55d-45d5764a1134.png#averageHue=%232d2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u5913ebe5&originHeight=256&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u49480d28-d1ca-4c7b-bef0-1d11dc9d1c1&title=)<br />但是这里需要注意的是，此时传入的`ConfigurableApplicationContext`并没有调用过`refresh`方法，也就是里面是没有Bean对象的，一般这个接口是用来配置`ConfigurableApplicationContext`，而不是用来获取Bean的。
<a name="pRfMa"></a>
### 4、`EnvironmentPostProcessor`
`EnvironmentPostProcessor`在SpringBoot启动过程中，也会调用，也是通过SPI机制来加载扩展的。<br />![EnvironmentPostProcessor](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776484-937d2606-b76f-48b6-9e64-6f8b0dfaee81.png#averageHue=%232c2b2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u895f6480&originHeight=268&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=udf3b0235-63d8-4ee4-a262-4865ee806fa&title=EnvironmentPostProcessor "EnvironmentPostProcessor")<br />`EnvironmentPostProcessor`是用来处理`ConfigurableEnvironment`的，也就是一些配置信息，SpringBoot所有的配置都是存在这个对象的。<br />说这个类的主要原因，主要不是说扩展，而是他的一个实现类很关键。<br />![ConfigFileApplicationListener](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776642-4b291bc5-8744-4cd4-b94e-0842e6aa69b0.png#averageHue=%232e2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u1395af43&originHeight=411&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u824dabd7-69c2-4d76-a0b6-7eea71a25d2&title=ConfigFileApplicationListener "ConfigFileApplicationListener")<br />这个类的作用就是用来处理外部化配置文件的，也就是这个类是用来处理配置文件的，通过前面提到的`PropertySourceLoader`解析配置文件，放到`ConfigurableEnvironment`里面。
<a name="w2yeh"></a>
### 5、`ApplicationRunner`和`CommandLineRunner`
`ApplicationRunner`和`CommandLineRunner`都是在SpringBoot成功启动之后会调用，可以拿到启动时的参数。<br />那怎么扩展呢？<br />当然又是SPI了。<br />这两个其实不是通过SPI机制来扩展，而是直接从容器中获取的，这又是为啥呢？<br />因为调用`ApplicationRunner`和`CommandLineRunner`时，SpringBoot已经启动成功了，Spring容器都准备好了，需要什么Bean直接从容器中查找多方便。<br />而前面说的几个需要SPI机制的扩展点，是因为在SpringBoot启动的时候，Spring容器还没有启动好，也就是无法从Spring容器获取到这些扩展的对象，为了兼顾扩展性，所以就通过SPI机制来实现获取到实现类。<br />![刷新上下文和调用Runner](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776636-d968db24-d6bd-4271-9d9f-6290a816e026.png#averageHue=%232e2b2b&clientId=ud7d136fd-1fc1-4&from=paste&id=uee823541&originHeight=463&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u307d6581-0ea5-4fbc-8d2f-c6b6f818fe7&title=%E5%88%B7%E6%96%B0%E4%B8%8A%E4%B8%8B%E6%96%87%E5%92%8C%E8%B0%83%E7%94%A8Runner "刷新上下文和调用Runner")<br />![加载和调用Runner](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776733-d5026bcd-acc2-4df7-bc49-7076b30cdd57.png#averageHue=%23322e2d&clientId=ud7d136fd-1fc1-4&from=paste&id=u0f274e04&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u67251f40-3ee8-4c2a-8cb5-26af4dba4cf&title=%E5%8A%A0%E8%BD%BD%E5%92%8C%E8%B0%83%E7%94%A8Runner "加载和调用Runner")<br />所以要想扩展这个点，只需要实现接口，添加到Spring容器就可以了。
<a name="hCoKP"></a>
## Spring Event 事件
Event 事件可以说是一种观察者模式的实现，主要是用来解耦合的。当发生了某件事，只要发布一个事件，对这个事件的监听者（观察者）就可以对事件进行响应或者处理。<br />举个例子来说，假设发生了火灾，可能需要打119、救人，那么就可以基于事件的模型来实现，只需要打119、救人监听火灾的发生就行了，当发生了火灾，通知这些打119、救人去触发相应的逻辑操作。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776893-9afc37ab-77a3-4ffd-8b9a-bdcdc8651a33.png#averageHue=%23f6f5f0&clientId=ud7d136fd-1fc1-4&from=paste&id=u9823ae21&originHeight=450&originWidth=808&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9abd9978-67dc-4379-b86e-6282ca86754&title=)
<a name="NGbdK"></a>
### 什么是Spring Event 事件
那么是什么是Spring Event 事件，就是Spring实现了这种事件模型，只需要基于Spring提供的API进行扩展，就可以完成事件的发布订阅<br />Spring提供的事件api:
<a name="o97le"></a>
#### `ApplicationEvent`
![ApplicationEvent](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883776951-c9b12a4b-3c62-4d31-bf35-bd46d913eb89.png#averageHue=%232d2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u6edd0227&originHeight=924&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ufe51c87b-6b17-4e23-99a8-ccec4df9a43&title=ApplicationEvent "ApplicationEvent")<br />事件的父类，所有具体的事件都得继承这个类，构造方法的参数是这个事件携带的参数，监听器就可以通过这个参数来进行一些业务操作。
<a name="DFYNc"></a>
#### `ApplicationListener`
![ApplicationListener](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777079-0d1cd990-066d-46c2-aaf1-15a3e6575679.png#averageHue=%232c2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u960df926&originHeight=282&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ub73d3675-4a3c-46f0-a822-12ef0a2cb90&title=ApplicationListener "ApplicationListener")<br />事件监听的接口，泛型是子类需要监听的事件类型，子类需要实现`onApplicationEvent`，参数就是事件类型，`onApplicationEvent`方法的实现就代表了对事件的处理，当事件发生时，Spring会回调`onApplicationEvent`方法的实现，传入发布的事件。
<a name="bXpNu"></a>
#### `ApplicationEventPublisher`
![ApplicationEventPublisher](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777126-4482e7a2-2a08-4985-b91c-295980fdad52.png#averageHue=%232c2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u9b5511ed&originHeight=871&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u18f6a662-bbcb-4a37-807d-3b6ce6336e0&title=ApplicationEventPublisher "ApplicationEventPublisher")<br />事件发布器，通过`publishEvent`方法就可以发布一个事件，然后就可以触发监听这个事件的监听器的回调。<br />`ApplicationContext`实现了`ApplicationEventPublisher`接口，所以通过`ApplicationContext`就可以发布事件。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777255-a740e600-69a5-4e70-8567-86b274df5166.png#averageHue=%232e2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u8d7fa52a&originHeight=241&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u98af00c7-f748-4d3e-9f04-70acfbc9f3e&title=)<br />那怎么才能拿到`ApplicationContext`呢？<br />前面Bean生命周期那节说过，可以通过`ApplicationContextAware`接口拿到，甚至可以通过实现`ApplicationEventPublisherAware`直接获取到`ApplicationEventPublisher`，其实获取到的`ApplicationEventPublisher`也就是`ApplicationContext`，因为是`ApplicationContext`实现了`ApplicationEventPublisher`。
<a name="ev6HV"></a>
#### 话不多说，上代码
就以上面的火灾为例
<a name="xoDO3"></a>
##### 第一步：创建一个火灾事件类
火灾事件类继承`ApplicationEvent`
```java
// 火灾事件
public class FireEvent extends ApplicationEvent {

    public FireEvent(String source) {
        super(source);
    }

}
```
<a name="YUjq1"></a>
##### 第二步：创建火灾事件的监听器
打119的火灾事件的监听器：
```java
public class Call119FireEventListener implements ApplicationListener<FireEvent> {

    @Override
    public void onApplicationEvent(FireEvent event) {
        System.out.println("打119");
    }

}
```
救人的火灾事件的监听器：
```java
public class SavePersonFireEventListener implements ApplicationListener<FireEvent> {

    @Override
    public void onApplicationEvent(FireEvent event) {
        System.out.println("救人");
    }

}
```
事件和对应的监听都有了，接下来进行测试：
```java
public class Application {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        //将 事件监听器 注册到容器中
        applicationContext.register(Call119FireEventListener.class);
        applicationContext.register(SavePersonFireEventListener.class);
        applicationContext.refresh();

        // 发布着火的事件，触发监听
        applicationContext.publishEvent(new FireEvent("着火了"));
    }

}
```
将两个事件注册到Spring容器中，然后发布FireEvent事件<br />运行结果：
```java
打119
救人
```
控制台打印出了结果，触发了监听。<br />如果现在需要对火灾进行救火，那么只需要去监听FireEvent，实现救火的逻辑，注入到Spring容器中，就可以了，其余的代码根本不用动。
<a name="WZVT7"></a>
### Spring内置的事件
Spring内置的事件很多，这里罗列几个

| **事件类型** | **触发时机** |
| --- | --- |
| `ContextRefreshedEvent` | 在调用`ConfigurableApplicationContext`接口中的`refresh()`方法时触发 |
| `ContextStartedEvent` | 在调用`ConfigurableApplicationContext`的`start()`方法时触发 |
| `ContextStoppedEvent` | 在调用`ConfigurableApplicationContext`的`stop()`方法时触发 |
| `ContextClosedEvent` | 当`ApplicationContext`被关闭时触发该事件，也就是调用`close()`方法触发 |

在Spring容器启动的过程中，Spring会发布这些事件，如果需要这Spring容器启动的某个时刻进行什么操作，只需要监听对应的事件即可。
<a name="o0epb"></a>
### Spring事件的传播
Spring事件的传播是什么意思呢？<br />都知道，在Spring中有子父容器的概念，而Spring事件的传播就是指当通过子容器发布一个事件之后，不仅可以触发在这个子容器的事件监听器，还可以触发在父容器的这个事件的监听器。
<a name="EBoIN"></a>
#### 上代码
```java
public class EventPropagateApplication {
	
	public static void main(String[] args) {
		
		// 创建一个父容器
		AnnotationConfigApplicationContext parentApplicationContext = new AnnotationConfigApplicationContext();
		//将 打119监听器 注册到父容器中
		parentApplicationContext.register(Call119FireEventListener.class);
		parentApplicationContext.refresh();
		
		// 创建一个子容器
		AnnotationConfigApplicationContext childApplicationContext = new AnnotationConfigApplicationContext();
		//将 救人监听器 注册到子容器中
		childApplicationContext.register(SavePersonFireEventListener.class);
		childApplicationContext.refresh();
		
		// 设置一下父容器
		childApplicationContext.setParent(parentApplicationContext);
		
		// 通过子容器发布着火的事件，触发监听
		childApplicationContext.publishEvent(new FireEvent("着火了"));
		
	}
	
}
```
创建了两个容器，父容器注册了打119的监听器，子容器注册了救人的监听器，然后将子父容器通过setParent关联起来，最后通过子容器，发布了着火的事件。<br />运行结果：
```java
救人
打119
```
从打印的日志，的确可以看出，虽然是子容器发布了着火的事件，但是父容器的监听器也成功监听了着火事件。
<a name="G6D1T"></a>
#### 源码验证
![事件传播源码](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777362-fb054d04-89ea-494a-a192-2ba89e65778d.png#averageHue=%23312d2c&clientId=ud7d136fd-1fc1-4&from=paste&id=u077eb90f&originHeight=492&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u0d1e6eb7-fe7b-436e-a6ab-52877c9f3fa&title=%E4%BA%8B%E4%BB%B6%E4%BC%A0%E6%92%AD%E6%BA%90%E7%A0%81 "事件传播源码")<br />从这段源码可以看出，如果父容器不为空，就会通过父容器再发布一次事件。
<a name="v4v4f"></a>
#### 传播特性的一个坑
前面说过，在Spring容器启动的过程，会发布很多事件，如果需要有相应的扩展，可以监听这些事件。但是，在SpringCloud环境下，这些Spring发布的事件的监听器可能会执行很多次。为什么会执行很多次呢？其实就是跟传播特性有关。<br />在SpringCloud的环境下，为了使像FeignClient和RibbonClient这些不同的服务的配置相互隔离，会创建很多的子容器，而这些子容器都有一个公共的父容器，那就是SpringBoot项目启动时创建的容器，事件的监听器都在这个容器中。而这些为了配置隔离创建的子容器，在容器启动的过程中，也会发布诸如ContextRefreshedEvent等这样的事件，如果监听了这些事件，那么由于传播特性的关系，这个事件的监听器就会触发多次。<br />如何解决这个坑呢？<br />可以进行判断这些监听器有没有执行过，比如加一个判断的标志；或者是监听类似的事件，比如ApplicationStartedEvent事件，这种事件是在SpringBoot启动中发布的事件，而子容器不是SpringBoot，所以不会多次发这种事件，也就会只执行一次。
<a name="mbqUk"></a>
### Spring事件的运用举例
<a name="qxipF"></a>
#### 1、在Mybatis中的使用
又来以Mybatis举例了。。Mybatis的`SqlSessionFactoryBean`监听了`ApplicationEvent`，然后判断如果是`ContextRefreshedEvent`就进行相应的处理，这个类还实现了`FactoryBean`接口。。
```java
public class SqlSessionFactoryBean
    implements FactoryBean<SqlSessionFactory>, InitializingBean, ApplicationListener<ApplicationEvent> {
    
    @Override
    public void onApplicationEvent(ApplicationEvent event) {
        if (failFast && event instanceof ContextRefreshedEvent) {
        // fail-fast -> check all statements are completed
        this.sqlSessionFactory.getConfiguration().getMappedStatementNames();
        }
    }
    
}
```
说实话，这监听代码写的不太好，监听了`ApplicationEvent`，那么所有的事件都会回调这个类的`onApplicationEvent`方法，但是`onApplicationEvent`方法实现又是当`ApplicationEvent`是`ContextRefreshedEvent`类型才会往下走，那为什么不直接监听`ContextRefreshedEvent`呢？
<a name="dwVdV"></a>
#### 2、在SpringCloud的运用
在SpringCloud的中，当项目启动的时候，会自动往注册中心进行注册，那么是如何实现的呢？当然也是基于事件来的。当web服务器启动完成之后，就发布ServletWebServerInitializedEvent事件。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777373-cf077fbf-f461-4077-91a8-8d43cef06366.png#averageHue=%232c2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=ued3f6663&originHeight=260&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub921dd12-3e32-4c81-b514-f43cb047b7e&title=)<br />然后不同的注册中心的实现都只需要监听这个事件，就知道web服务器已经创建好了，那么就可以往注册中心注册服务实例了。如果服务没往注册中心，看看是不是web环境，因为只有web环境才会发这个事件。<br />SpringCloud提供了一个抽象类 `AbstractAutoServiceRegistration`，实现了对`WebServerInitializedEvent`（`ServletWebServerInitializedEvent`的父类）事件的监听<br />![AbstractAutoServiceRegistration](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777626-93037b74-143b-4b58-9b54-c3c2734e0300.png#averageHue=%232f2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u48e59bf9&originHeight=507&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u1897fc38-7929-4b83-bbaa-023d8e3cb24&title=AbstractAutoServiceRegistration "AbstractAutoServiceRegistration")<br />一般不同的注册中心都会去继承这个类，监听项目启动，实现往注册中心服务端进行注册。<br />![Nacos对于AbstractAutoServiceRegistration的继承](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777654-434b1933-eeb8-4d3a-8ed3-b8f13542ecb0.png#averageHue=%232f2c2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u88f362b1&originHeight=524&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uf8de4ceb-ac19-4d60-80c6-ff216073a19&title=Nacos%E5%AF%B9%E4%BA%8EAbstractAutoServiceRegistration%E7%9A%84%E7%BB%A7%E6%89%BF "Nacos对于AbstractAutoServiceRegistration的继承")<br />**Spring Event事件在Spring内部中运用很多，是解耦合的利器。在实际项目中，既可以监听Spring/Boot内置的一些事件，进行相应的扩展，也可以基于这套模型在业务中自定义事件和相应的监听器，减少业务代码的耦合。**
<a name="jGfME"></a>
## 命名空间
最后来讲一个可能没有留意，但是很神奇的扩展点--命名空间。起初知道这个扩展点的时候，都惊呆了，这玩意也能扩展？真的不得不佩服Spring设计的可扩展性。<br />回忆一下啥是命名空间？<br />先看一段配置
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">http://www.w3.org/2001/XMLSchema-instance"
	xmlns="http://www.springframework.org/schema/beans"
	xmlns:context="http://www.springframework.org/schema/context"
	xsi:schemaLocation="
	http://www.springframework.org/schema/beans
	http://www.springframework.org/schema/beans/spring-beans.xsd
	http://www.springframework.org/schema/context 
	http://www.springframework.org/schema/beans/spring-context.xsd
	">
	
	<context:component-scan base-package="com.sanyou.spring.extension"/>
	
</beans>
```
这一段xml配置想必都很熟悉，其中， context 标签就代表了一个命名空间。<br />也就说，这个标签是可以扩展的。
<a name="o1rwa"></a>
### 来自定义一个扩展
接下来自定义命名空间 sanyou，总共分为3步。
<a name="gx5Pa"></a>
#### 第一步：定义一个xsd文件
如下：
```xml
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!-- xmlns 和 targetNamespace 需要定义，结尾为sanyou，前面都一样的-->
<xsd:schema xmlns="http://sanyou.com/schema/sanyou"
            xmlns:xsd="http://www.w3.org/2001/XMLSchema"
            targetNamespace="http://sanyou.com/schema/sanyou">

    <xsd:import namespace="http://www.w3.org/XML/1998/namespace"/>

    <xsd:complexType name="Bean">
        <xsd:attribute name="class" type="xsd:string" use="required"/>
    </xsd:complexType>

    <!--  sanyou 便签的子标签，类型是Bean ，就会找到上面的complexType=Bean类型，然后处理属性  -->
    <xsd:element name="mybean" type="Bean"/>
</xsd:schema>
```
这个xsd文件来指明sanyou这个命名空间下有哪些标签和属性。这里只指定了一个标签 mybean，mybean标签里面有个class的属性，然后这个标签的目的就是将class属性指定的Bean的类型，注入到Spring容器中，作用跟spring的标签的作用是一样的。<br />xsd文件没有需要放的固定的位置，这里放到 META-INF 目录下
<a name="qaxqF"></a>
#### 第二步：解析这个命名空间
解析命名空间很简单，Spring都有配套的东西--`NamespaceHandler`接口，只要实现这个接口就行了。但一般不直接实现 `NamespaceHandler` 接口，可以继承 `NamespaceHandlerSupport` 类，这个类实现了 `NamespaceHandler` 接口。
```java
public class SanYouNameSpaceHandler extends NamespaceHandlerSupport {
	
	@Override
	public void init() {
		//注册解析 mybean 标签的解析器
		registerBeanDefinitionParser("mybean", new SanYouBeanDefinitionParser());
	}
	
	private static class SanYouBeanDefinitionParser extends AbstractSingleBeanDefinitionParser {
		@Override
		protected boolean shouldGenerateId() {
			return true;
		}
		
		@Override
		protected String getBeanClassName(Element element) {
			return element.getAttribute("class");
		}
		
	}
}
```
`SanYouNameSpaceHandler`的作用就是将sanyou命名空间中的mybean这个标签读出来，拿到class的属性，然后将这个class属性指定的class类型注入到Spring容器中，至于注册这个环节的代码，都交给了`SanYouBeanDefinitionParser`的父类来做了。
<a name="A6P8K"></a>
#### 第三步：创建并配置spring.handlers和spring.schemas文件
先创建spring.handlers和spring.schemas文件<br />spring.handlers文件内容
```
http\://sanyou.com/schema/sanyou=com.sanyou.spring.extension.namespace.SanYouNameSpaceHandler
```
通过spring.handlers配置文件，就知道sanyou命名空间应该找`SanYouNameSpaceHandler`进行解析<br />spring.schemas文内容
```
http\://sanyou.com/schema/sanyou.xsd=META-INF/sanyou.xsd
```
spring.schemas配置xsd文件的路径<br />文件都有了，只需要放到classpath下的META-INF文件夹就行了。<br />![xsd、spring.handlers、spring.schema文件](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777716-24ff9256-bcdc-40dd-a7bc-b187605ef83e.png#averageHue=%233e4143&clientId=ud7d136fd-1fc1-4&from=paste&id=u9d43df23&originHeight=340&originWidth=634&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u93abe477-aec8-4f1d-969f-9a25b0c0cb1&title=xsd%E3%80%81spring.handlers%E3%80%81spring.schema%E6%96%87%E4%BB%B6 "xsd、spring.handlers、spring.schema文件")<br />到这里，就完成了扩展，接下来进行测试
<a name="U7sLX"></a>
#### 测试
先构建一个applicationContext.xml文件，放到resources目录下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns="http://www.springframework.org/schema/beans"
       xmlns:sanyou="http://sanyou.com/schema/sanyou"
       xsi:schemaLocation="
       http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://sanyou.com/schema/sanyou
       http://sanyou.com/schema/sanyou.xsd
       ">

    <!--使用 sanyou 标签，配置一个 User Bean-->
    <sanyou:mybean class="com.sanyou.spring.extension.User"/>

</beans>
```
再写个测试类
```java
public class Application {

    public static void main(String[] args) {

        ClassPathXmlApplicationContext applicationContext = new ClassPathXmlApplicationContext("applicationContext.xml");
        applicationContext.refresh();

        User user = applicationContext.getBean(User.class);

        System.out.println(user);
    }

}
```
运行结果：
```java
com.sanyou.spring.extension.User@27fe3806
```
成功获取到User这个对象，说明自定义标签生效了。
<a name="z5af9"></a>
### Spring内置命名空间的扩展
![NameSpaceHandler的spring实现](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883777777-da9f174e-cf09-46a1-a6a3-482503f5244c.png#averageHue=%23656a5e&clientId=ud7d136fd-1fc1-4&from=paste&id=ufc44286e&originHeight=628&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u244e5063-4518-42b4-a6cd-475112f2705&title=NameSpaceHandler%E7%9A%84spring%E5%AE%9E%E7%8E%B0 "NameSpaceHandler的spring实现")<br />通过`NameSpaceHandler`接口的这些实现类的命名就可以看出来有哪些扩展和这些扩展的作用，比如有处理aop的，有处理mvc的等等之类的。
<a name="hzmPC"></a>
### 开源框架对命名空间的扩展
<a name="dTnof"></a>
#### 1、Mybatis的扩展
![Mybatis的NameSpaceHandler实现](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883778003-f8732661-a3f0-4adf-8906-7fddbdfca2cd.png#averageHue=%232c2b2b&clientId=ud7d136fd-1fc1-4&from=paste&id=u4fed203f&originHeight=306&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u989aceb8-7850-4365-9fd2-a22ff4cff80&title=Mybatis%E7%9A%84NameSpaceHandler%E5%AE%9E%E7%8E%B0 "Mybatis的NameSpaceHandler实现")<br />这个就是来扫描指定路径的mapper接口的，处理 scan 标签，跟`@MapperScan`注解的作用是一样的。
<a name="V4qJf"></a>
#### 2、dubbo的扩展
使用dubbo可能写过如下的配置
```xml
<dubbo:registry address="zookeeper://192.168.10.119:2181" />
```
这个dubbo命名空间肯定就是扩展的Spring的，也有对应的dubbo实现的`NameSpaceHandler`。<br />![DubboNamespaceHandler](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883778113-2edc38fe-e2ac-42f6-a225-6e080a2bc4dd.png#averageHue=%232d2c2c&clientId=ud7d136fd-1fc1-4&from=paste&id=uc522f600&originHeight=521&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u8fe021ed-5f47-439b-b5d6-e4e72ad0f38&title=DubboNamespaceHandler "DubboNamespaceHandler")<br />不得不说，dubbo解析的标签可真的多，不过功能也是真的多。
<a name="Xk6iw"></a>
## 总结
这里画两张图来总结一下本文讲了Spring的哪些扩展点。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658886207158-4377a78f-8e02-47c2-884a-5340bf960b4d.jpeg)<br />整体<br />![SpringBoot启动扩展点](https://cdn.nlark.com/yuque/0/2022/png/396745/1658883778116-7ef2269c-b792-41d5-b43b-ba9588d813e5.png#averageHue=%23f7f7f6&clientId=ud7d136fd-1fc1-4&from=paste&id=u67321c35&originHeight=448&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u24cc93c6-4c5b-45c1-89c5-e5757279c60&title=SpringBoot%E5%90%AF%E5%8A%A8%E6%89%A9%E5%B1%95%E7%82%B9 "SpringBoot启动扩展点")<br />通过学习Spring的这些扩展点，既可以应对日常的开发，还可以更好地看懂Spring的源码。
