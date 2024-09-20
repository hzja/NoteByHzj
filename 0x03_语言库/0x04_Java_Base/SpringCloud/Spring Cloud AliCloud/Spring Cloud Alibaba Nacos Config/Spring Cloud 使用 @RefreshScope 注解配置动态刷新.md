JavaSpringCloud
<a name="z8H8H"></a>
## 一、`@RefreshScope`动态刷新原理
在SpringIOC中，BeanScope（Bean的作用域）影响了Bean的管理方式。<br />Bean的作用域：

| **作用域** | **描述** |
| --- | --- |
| singleton（单例） | 每一个Spring IoC容器都拥有唯一的一个实例对象（默认作用域） |
| prototype（原型） | 一个Bean定义，任意多个对象 |
| request（请求） | 每一个HTTP请求都有自己的Bean实例（只在基于web的Spring ApplicationContext中可用） |
| session（会话） | 一个Bean的作用域为HTTPsession的生命周期（只有基于web的Spring ApplicationContext才能使用） |
| global session（全局会话） | 一个Bean的作用域为全局HTTPSession的生命周期。通常用于门户网站场景（只有基于web的Spring ApplicationContext才能使用） |

例如创建`Scope=singleton`的Bean时，IOC会保存实例在一个Map中，保证这个Bean在一个IOC上下文有且仅有一个实例。<br />SpringCloud新增了一个自定义的作用域：refresh（可以理解为“动态刷新”），同样用了一种独特的方式改变了Bean的管理方式，使得其可以通过外部化配置（.properties）的刷新，在应用不需要重启的情况下热加载新的外部化配置的值。<br />这个scope是如何做到热加载的呢？`@RefreshScope`主要做了以下动作：
<a name="YsbtL"></a>
##### 单独管理Bean生命周期
创建Bean的时候如果是`@RefreshScope`就缓存在一个专门管理的`ScopeMap`中，这样就可以管理Scope是Refresh的Bean的生命周期了（所以含`RefreshScope`的其实一共创建了两个bean）。
<a name="GCT4x"></a>
##### 重新创建Bean
外部化配置刷新之后，会触发一个动作，这个动作将上面的`ScopeMap`中的Bean清空，这样这些Bean就会重新被IOC容器创建一次，使用最新的外部化配置的值注入类中，达到热加载新值的效果。<br />spring cloud config或sprring cloud alibaba nacos作为配置中心，其实现原理就是通过`@RefreshScope` 来实现对象属性的的动态更新。<br />`@RefreshScope` 实现配置的动态刷新需要满足一下几点条件：

- `@Scope`注解
- `@RefreshScope`注解
- `RefreshScope`类
- `GenericScope`类
- `Scope`接口
- `ContextRefresher`类

`@RefreshScope` 能实现动态刷新全仰仗着`@Scope` 这个注解。
<a name="vmVxN"></a>
### 1、`@Scope`注解
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Scope {

    /**
     * Alias for {@link #scopeName}.
     *
     * @see #scopeName
     */
    @AliasFor("scopeName")
    String value() default "";

    /**
     * singleton 表示该bean是单例的。(默认)
     * prototype表示该bean是多例的，即每次使用该bean时都会新建一个对象。
     * request在一次http请求中，一个bean对应一个实例。
     * session在一个httpSession中，一个bean对应一个实例
     */
    @AliasFor("value")
    String scopeName() default "";

    /**
     * DEFAULT不使用代理。(默认)
     * NO不使用代理，等价于DEFAULT。
     * INTERFACES 使用基于接口的代理(jdk dynamic proxy)。
     * TARGET_CLASS 使用基于类的代理(cglib)。
     */
    ScopedProxyMode proxyMode() default ScopedProxyMode.DEFAULT;

}
```
`@Scope`有两个主要属性value 和 `proxyMode`，其中`proxyMode`就是`@RefreshScope` 实现的本质了。<br />`proxyMode`属性是一个`ScopedProxyMode`类型的枚举对象。
```java
public enum ScopedProxyMode {
    DEFAULT,
    NO,
    INTERFACES,// JDK 动态代理
    TARGET_CLASS;// CGLIB 动态代理

    private ScopedProxyMode() {
    }
}
```
当`proxyMode`属性的值为`ScopedProxyMode.TARGET_CLASS`时，会给当前创建的bean 生成一个代理对象，会通过代理对象来访问，每次访问都会创建一个新的对象。
<a name="gOfWT"></a>
### 2、`@RefreshScope`注解
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Scope("refresh")
@Documented
public @interface RefreshScope {
    /**
     * @see Scope#proxyMode()
     */
    ScopedProxyMode proxyMode() default ScopedProxyMode.TARGET_CLASS;

}
```
它使用就是 `@Scope` ，一个`scopeName="refresh"`的`@Scope`。<br />`proxyMode`值为`ScopedProxyMode.TARGET_CLASS`，通过CGLIB动态代理的方式生成Bean。<br />使用 `@RefreshScope` 注解的 bean，不仅会生成一个`beanName`的bean，默认情况下同时会生成 `scopedTarget.beanName`的 bean。<br />![2022-10-08-20-53-43.939507900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1665234354354-0a5e7af2-f274-41a6-b96b-cc83b4f228d8.png#clientId=ud8c598ee-823e-4&errorMessage=unknown%20error&from=ui&id=uc87497ea&originHeight=125&originWidth=812&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13249&status=error&style=none&taskId=u09092d54-fb0b-4030-a31f-84d16649978&title=)![2022-10-08-20-53-44.040246700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1665234354370-23b3c9f8-a722-4855-ba68-6c0de4d39495.png#clientId=ud8c598ee-823e-4&errorMessage=unknown%20error&from=ui&id=u790fd371&originHeight=120&originWidth=890&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13674&status=error&style=none&taskId=u6d869a6a-0095-4571-81c2-86343791078&title=)<br />`@RefreshScope`不能单独使用，需要和其他其他bean注解结合使用，如：`@Controller`、`@Service`、`@Component`、`@Repository`等。
<a name="FjWks"></a>
### 3、`Scope`接口
```java
public interface Scope {

    /**
     * Return the object with the given name from the underlying scope,
     * {@link org.springframework.beans.factory.ObjectFactory#getObject() creating it}
     * if not found in the underlying storage mechanism.
     * <p>This is the central operation of a Scope, and the only operation
     * that is absolutely required.
     *
     * @param name          the name of the object to retrieve
     * @param objectFactory the {@link ObjectFactory} to use to create the scoped
     *                      object if it is not present in the underlying storage mechanism
     * @return the desired object (never {@code null})
     * @throws IllegalStateException if the underlying scope is not currently active
     */
    Object get(String name, ObjectFactory<?> objectFactory);

    @Nullable
    Object remove(String name);

    void registerDestructionCallback(String name, Runnable callback);

    @Nullable
    Object resolveContextualObject(String key);

    @Nullable
    String getConversationId();
}
```
```java
Object get(String name, ObjectFactory<?> objectFactory)
```
这个方法来创建一个新的bean ，也就是说，`@RefreshScope` 在调用刷新的时候会使用此方法来创建新的对象，这样就可以通过spring 的装配机制将属性重新注入了，也就实现了所谓的动态刷新。
```java
RefreshScope extends GenericScope, GenericScope implements Scope
```
`GenericScope` 实现了 Scope 最重要的 `get(String name, ObjectFactory<?> objectFactory)` 方法，在`GenericScope` 里面 包装了一个内部类 `BeanLifecycleWrapperCache` 来对加了 `@RefreshScope` 从而创建的对象进行缓存，使其在不刷新时获取的都是同一个对象。（这里可以把 `BeanLifecycleWrapperCache` 想象成为一个大Map 缓存了所有`@RefreshScope` 标注的对象）<br />知道了对象是缓存的，所以在进行动态刷新的时候，只需要清除缓存，重新创建就好了。
```java
// ContextRefresher 外面使用它来进行方法调用 ============================== 我是分割线

public synchronized Set<String> refresh() {
    Set<String> keys = refreshEnvironment();
    this.scope.refreshAll();
    return keys;
}

// RefreshScope 内部代码  ============================== 我是分割线

@ManagedOperation(description = "Dispose of the current instance of all beans in this scope and force a refresh on next method execution.")
public void refreshAll() {
    super.destroy();
    this.context.publishEvent(new RefreshScopeRefreshedEvent());
}


// GenericScope 里的方法 ============================== 我是分割线

//进行对象获取，如果没有就创建并放入缓存
@Override
public Object get(String name, ObjectFactory<?> objectFactory) {
    BeanLifecycleWrapper value = this.cache.put(name,
            new BeanLifecycleWrapper(name, objectFactory));
    locks.putIfAbsent(name, new ReentrantReadWriteLock());
    try {
        return value.getBean();
    } catch (RuntimeException e) {
        this.errors.put(name, e);
        throw e;
    }
}

// 初始化Bean
public Object getBean() {
    if (this.bean == null) {
        String var1 = this.name;
        synchronized (this.name) {
            if (this.bean == null) {
                this.bean = this.objectFactory.getObject();
            }
        }
    }
    return this.bean;
}

//进行缓存的数据清理
@Override
public void destroy() {
    List<Throwable> errors = new ArrayList<Throwable>();
    Collection<BeanLifecycleWrapper> wrappers = this.cache.clear();
    for (BeanLifecycleWrapper wrapper : wrappers) {
        try {
            Lock lock = locks.get(wrapper.getName()).writeLock();
            lock.lock();
            try {
                wrapper.destroy();
            } finally {
                lock.unlock();
            }
        } catch (RuntimeException e) {
            errors.add(e);
        }
    }
    if (!errors.isEmpty()) {
        throw wrapIfNecessary(errors.get(0));
    }
    this.errors.clear();
}
```
通过观看源代码得知，截取了三个片段所得之，`ContextRefresher` 就是外层调用方法用的。<br />`GenericScope`类中有一个成员变量`BeanLifecycleWrapperCache`，用于缓存所有已经生成的Bean，在调用get方法时尝试从缓存加载，如果没有的话就生成一个新对象放入缓存，并通过初始化getBean其对应的Bean。<br />`destroy` 方法负责再刷新时缓存的清理工作。清空缓存后，下次访问对象时就会重新创建新的对象并放入缓存了。<br />所以在重新创建新的对象时，也就获取了最新的配置，也就达到了配置刷新的目的。
<a name="nW0qW"></a>
### 4、`@RefreshScope`实现流程

- 需要动态刷新的类标注`@RefreshScope` 注解。
- `@RefreshScope` 注解标注了`@Scope` 注解，并默认了`ScopedProxyMode.TARGET_CLASS;` 属性，此属性的功能就是再创建一个代理，在每次调用的时候都用它来调用GenericScope get 方法来获取对象。
- 如属性发生变更
   - 调用 `ContextRefresher refresh()` -->> `RefreshScope refreshAll()` 进行缓存清理方法调用；
   - 发送刷新事件通知，`GenericScope` 真正的清理方法`destroy()` 实现清理缓存。
- 在下一次使用对象的时候，会调用`GenericScope get(String name, ObjectFactory<?> objectFactory)` 方法创建一个新的对象，并存入缓存中，此时新对象因为Spring 的装配机制就是新的属性了。
<a name="ZbjGH"></a>
### 5、`@RefreshScope`原理总结

1. SpringCloud程序的存在一个自动装配的类，这个类默认情况下会自动初始化一个`RefreshScope`实例，该实例是`GenericScope`的子类，然后注册到容器中。（RefreshAutoConfiguration.java）
2. 当容器启动的时候，`GenericScope`会自己把自己注册到scope中（`ConfigurableBeanFactory#registerScope`）（`GenericScope`）
3. 然后当自定义的Bean（被`@RefreshScope`修饰）注册的时候，会被容器读取到其作用域为refresh。(`AnnotatedBeanDefinitionReader#doRegisterBean`)

通过上面三步，一个带有`@RefreshScope`的自定义Bean就被注册到容器中来，其作用域为refresh。

4. 当后续进行以来查找的时候，会绕过`Singleton`和`Prototype`分支，进入最后一个分支，通过调用Scope接口的`get()`获取到该refresh作用域的实例。（`AbstractBeanFactory.doGetBean`）
<a name="mqQVR"></a>
## 二、`@RefreshScope`注意事项
<a name="Gja4d"></a>
### 1、`@RefreshScope`使用注意事项

- `@RefreshScope`作用的类，不能是`final`类，否则启动时会报错。
- `@RefreshScope`不能单独使用，需要和其他其他bean注解结合使用，如：`@Controller`、`@Service`、`@Component`、`@Repository`、`@Configuration`等。
- `@RefreshScope` 最好不要修饰在 `@Scheduled`、`listener`、`Timmer`等类中，因为配置的刷新会导致原来的对象被清除，需要重新使用对象才能出发生成新对象（但因为对象没了，又没法重新使用对象，死循环）
<a name="FouqL"></a>
### 2、`@RefreshScope`动态刷新失效
考虑使用的bean是否是`@RefreshScope`生成的那个`scopedTarget.beanName`的 bean<br />SpringBoot某些低版本貌似有问题，在Controller类上使用不会生效（网上有这么说的，没具体研究）

- **解决方法1**：注解上加属性`@RefreshScope(proxyMode = ScopedProxyMode.DEFAULT)`
- **解决方法2**：直接使用其他类单独封装配置参数，使用`@RefreshScope`+`@Value`方式
- **解决方法3**：直接使用`@ConfigurationProperties`
<a name="WaqnS"></a>
### 3、不使用`@RefreshScope`也能实现动态刷新
直接使用`@ConfigurationProperties`，并不需要加`@RefreshScope`就能实现动态更新。<br />`@ConfigurationProperties`实现动态刷新的原理：<br />`@ConfigurationProperties`有`ConfigurationPropertiesRebinder`这个监听器，监听着`EnvironmentChangeEvent`事件。当发生`EnvironmentChange`事件后，会重新构造原来的加了`@ConfigurationProperties`注解的Bean对象。这个是Spring Cloud的默认实现。
<a name="TDvoY"></a>
### 4、静态变量利用`@RefreshScope`动态刷新的坑
```java
@RefreshScope
@Component
public class TestConfig {
    public static int url;

    @Value("${pesticide.url}")
    public void setUrl(int url) {
        TestConfig.url = url;
    }

    public void getUrl() {
    }
}
```
```java
@RestController
@RequestMapping("test")
public class TestController {
    @Autowired
    private TestConfig testConfig;

    @GetMapping("testConfig")
    public int testConfig(){
        System.out.println("TestConfig:"+ TestConfig.url);
        testConfig.getUrl();
        System.out.println("TestConfig:"+ TestConfig.url);
        return TestConfig.url;
    }
}
```
<a name="hHnIk"></a>
#### 1.url初始配置的值为1
请求接口日志：
```
TestConfig:1
TestConfig:1
```
<a name="WEeXP"></a>
#### 2.修改url配置的值为2，动态刷新成功
请求接口日志：
```
TestConfig:1
TestConfig:2
```
这里就出现了问题，不调用`@RefreshScope`生产的代理对象testConfig的方法前（注意，该方法内无代码），取到的值还是为1；调了之后，取到的值为2，后续再次请求接口，取到的值都为2。
```
TestConfig:2
TestConfig:2
TestConfig:2
TestConfig:2
```
个人大胆猜想原因：参考上面`@RefreshScope` 实现流程可知，在第2步骤动态刷新成功时，此时仅仅是再创建类一个代理对象，并清除了实际对象的缓存；当再次通过代理对象来使用，才会触发创建一个新的实例对象，此时才会更新url的值。所以使用静态变量来是实现动态刷新时，一点要注意：使用对象才能出发创建新的实际对象，更新静态变量的值。<br />Spring Cloud的参考文档指出：
> `@RefreshScope`在`@Configuration`类上工作，但可能导致令人惊讶的行为：例如，这并不意味着该类中定义的所有`@Beans`本身都是`@RefreshScope`。具体来说，依赖于这些bean的任何东西都不能依赖于刷新启动时对其进行更新，除非它本身在`@RefreshScope`中从刷新的`@Configuration`重新初始化（在刷新中将其重建并重新注入其依赖项，此时它们将被刷新）。

<a name="zpfUK"></a>
## 三、使用`@RefreshScope`的bean问题
这里之所以要会讨论使用`@RefreshScope`的bean问题，由上面上面所讲可以总结得到：

- 使用 `@RefreshScope` 注解的 bean，不仅会生成一个名为beanName的bean，默认情况下同时会生成名为`scopedTarget.beanName`的bean
- 使用 `@RefreshScope` 注解的会生成一个代理对象，通过这个代理对象来调用名为`scopedTarget.beanName`的 bean
- 刷新操作会导致原来的名为`scopedTarget.beanName`的bean被清除，再次使用会新生成新的名为`scopedTarget.beanName`的bean，但原来的代理对象不会变动

下面举例说明：<br />**nacos配置**
```yaml
test:
  value: 1
```
**配置类获取配置值**
```java
@Data
@Component
@RefreshScope
public class TestConfig {

    @Value("${test.value}")
    private String value;
}
```
**测试接口**
```java
@RestController
public class TestController {

    @Autowired
    private TestConfig testConfig;

    @RequestMapping("test11")
    public void test11() {
        // 代理对象
        System.out.println("@Autowired bean==========" + testConfig.getClass().getName());

        // 代理对象
        TestConfig bean = SpringUtils.getBean(TestConfig.class);
        System.out.println("Class bean==========" + bean.getClass().getName());

        // 代理对象
        Object bean1 = SpringUtils.getBean("testConfig");
        System.out.println("name(testConfig) bean==========" + bean1.getClass().getName());

        // 原类对象
        Object bean2 = SpringUtils.getBean("scopedTarget.testConfig");
        System.out.println("name(scopedTarget.testConfig) bean==========" + bean2.getClass().getName());

        System.out.println("================================================================================");

    }
}
```
**测试**<br />![2022-10-08-20-53-44.064309400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1665234586180-2a299d6c-7cf3-44f1-8ffa-955b122fe34b.png#clientId=ud8c598ee-823e-4&errorMessage=unknown%20error&from=ui&id=u250d4f90&originHeight=145&originWidth=802&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20917&status=error&style=none&taskId=u9ec426d9-15da-4fef-b3f3-43ae5006ed7&title=)<br />`@Autowired`注入的是代理对象

- 通过Class得到的是代理对象
- 通过名为`beanName`的得到的是代理对象
- 通过名为`scopedTarget.beanName`的得到的是由`@RefreshScope`生成的那个原类对象

**修改配置的值，测试**
```yaml
test:
  value: 2
```
![2022-10-08-20-53-44.196394600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1665234586202-2af561d8-5b5c-4f28-bea3-6a175789ef76.png#clientId=ud8c598ee-823e-4&errorMessage=unknown%20error&from=ui&id=iXLuG&originHeight=126&originWidth=730&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20014&status=error&style=none&taskId=ud0905849-e8ed-4328-aa06-777b10bed45&title=)<br />动态刷新后，代理对象没有变化，由`@RefreshScope`生成的那个原类对象被清除后重新生成了一个新的原类对象
<a name="pYMob"></a>
### 小结：

- `@Autowired`方式注入的是代理对象
- `beanName`的得到的是代理对象
- `scopedTarget.beanName`的得到的`@RefreshScope`生成的那个原类对象
- 代理对象不会随着配置刷新而更新
- `@RefreshScope`生成的那个原类对象会随着配置的刷新而更新（属性时清除原来的，使用时才生成新的）
<a name="efuC5"></a>
## 四、其它配置刷新方式
这种方法必须有 spring-boot-starter-actuator 这个starter才行。
```http
POST http://localhost:7031/refresh
```
refresh的底层原理详见：org.springframework.cloud.context.refresh.ContextRefresher#refresh<br />SpringCloud2.0以后，没有/refresh手动调用的刷新配置地址。
<a name="VR505"></a>
### SpringCloud2.0前
加入依赖
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```
在类上，变量上打上`@RefreshScope`的注解<br />在启动的时候，都会看到
```
RequestMappingHandlerMapping : Mapped "{/refresh,methods=[post]}"
```
也就是SpringCloud暴露了一个接口 /refresh 来刷新配置，但是SpringCloud 2.0.0以后，有了改变。
<a name="BQz9w"></a>
### SpringCloud 2.0后
需要在bootstrap.yml里面加上需要暴露出来的地址
```yaml
management:
  endpoints:
    web:
      exposure:
        include: refresh,health
```
现在的地址也不是/refresh了，而是/actuator/refresh
