Java Spring Bean
<a name="SWefe"></a>
## 前言
在庞大的java体系中，Spring有着举足轻重的地位，它给每位开发者带来了极大的便利和惊喜。都知道Spring是创建和管理bean的工厂，它提供了多种定义bean的方式，能够满足日常工作中的多种业务场景。<br />Spring中有哪些方式可以定义bean？<br />很多人会说出以下三种：<br />![2021-07-10-08-26-45-586062.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879199846-8d94b678-87ed-4ac7-a144-cce6f2e44b3b.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u5abe4ee7&originHeight=279&originWidth=1080&originalType=binary&ratio=1&size=53906&status=done&style=shadow&taskId=ud5fb5f5d-07b7-4973-b8ab-b0362c49918)<br />实际上Spring的功能远比想象中更强大。
<a name="W3ZJt"></a>
## 1、xml文件配置bean
先从xml配置bean开始，它是Spring最早支持的方式。后来，随着SpringBoot越来越受欢迎，该方法目前已经用得很少了，但还是有必要了解一下。
<a name="f9aPN"></a>
### 1.1 构造器
如果之前有在bean.xml文件中配置过bean的经历，那么对如下的配置肯定不会陌生：
```xml
<bean id="personService" class="com.sue.cache.service.test7.PersonService">
</bean>
```
这种方式是以前使用最多的方式，它默认使用了无参构造器创建bean。<br />当然还可以使用有参的构造器，通过`<constructor-arg>`标签来完成配置。
```xml
<bean id="personService" class="com.sue.cache.service.test7.PersonService">
   <constructor-arg index="0" value="susan"></constructor-arg>
   <constructor-arg index="1" ref="baseInfo"></constructor-arg>
</bean>
```
其中：

- `index`表示下标，从0开始。
- `value`表示常量值
- `ref`表示引用另一个bean
<a name="wodfT"></a>
### 1.2 `setter`方法
除此之外，Spring还提供了另外一种思路：通过setter方法设置bean所需参数，这种方式耦合性相对较低，比有参构造器使用更为广泛。<br />先定义Person实体：
```java
@Data
public class Person {
    private String name;
    private int age;
}
```
它里面包含：成员变量name和age，getter/setter方法。<br />然后在bean.xml文件中配置bean时，加上`<property>`标签设置bean所需参数。
```xml
<bean id="person" class="com.sue.cache.service.test7.Person">
   <property name="name" value="susan"></constructor-arg>
   <property name="age" value="18"></constructor-arg>
</bean>
```
<a name="ejLZf"></a>
### 1.3 静态工厂
这种方式的关键是需要定义一个工厂类，它里面包含一个创建bean的静态方法。例如：
```java
public class SusanBeanFactory {
    public static Person createPerson(String name, int age) {
        return new Person(name, age);
    }
}
```
接下来定义Person类如下：
```java
@AllArgsConstructor
@NoArgsConstructor
@Data
public class Person {
    private String name;
    private int age;
}
```
它里面包含：成员变量name和age，getter/setter方法，无参构造器和全参构造器。<br />然后在bean.xml文件中配置bean时，通过`factory-method`参数指定静态工厂方法，同时通过`<constructor-arg>`设置相关参数。
```xml
<bean class="com.sue.cache.service.test7.SusanBeanFactory" factory-method="createPerson">
   <constructor-arg index="0" value="susan"></constructor-arg>
   <constructor-arg index="1" value="18"></constructor-arg>
</bean>
```
<a name="s9k90"></a>
### 1.4 实例工厂方法
这种方式也需要定义一个工厂类，但里面包含非静态的创建bean的方法。
```java
public class SusanBeanFactory {
    public Person createPerson(String name, int age) {
        return new Person(name, age);
    }
}
```
Person类跟上面一样，就不多说了。<br />然后bean.xml文件中配置bean时，需要先配置工厂bean。然后在配置实例bean时，通过factory-bean参数指定该工厂bean的引用。
```xml
<bean id="susanBeanFactory" class="com.sue.cache.service.test7.SusanBeanFactory">
</bean>
<bean factory-bean="susanBeanFactory" factory-method="createPerson">
   <constructor-arg index="0" value="susan"></constructor-arg>
   <constructor-arg index="1" value="18"></constructor-arg>
</bean>
```
<a name="JZ6E0"></a>
### 1.5 `FactoryBean`
上面的实例工厂方法每次都需要创建一个工厂类，不方面统一管理。这时可以使用`FactoryBean`接口。
```java
public class UserFactoryBean implements FactoryBean<User> {
    @Override
    public User getObject() throws Exception {
        return new User();
    }

    @Override
    public Class<?> getObjectType() {
        return User.class;
    }
}
```
在它的`getObject`方法中可以实现自己的逻辑创建对象，并且在`getObjectType`方法中可以定义对象的类型。<br />然后在bean.xml文件中配置bean时，只需像普通的bean一样配置即可。
```xml
<bean id="userFactoryBean" class="com.sue.async.service.UserFactoryBean">
</bean>
```
:::tips
注意：`getBean("userFactoryBean");`获取的是`getObject`方法中返回的对象。而`getBean("&userFactoryBean");`获取的才是真正的`UserFactoryBean`对象。
:::
通过上面五种方式，在bean.xml文件中把bean配置好之后，Spring就会自动扫描和解析相应的标签，并且创建和实例化bean，然后放入Spring容器中。<br />虽说基于xml文件的方式配置bean，简单而且非常灵活，比较适合一些小项目。但如果遇到比较复杂的项目，则需要配置大量的bean，而且bean之间的关系错综复杂，这样久而久之会导致xml文件迅速膨胀，非常不利于bean的管理。
<a name="U2EhF"></a>
## 2、`Component`注解
为了解决bean太多时，xml文件过大，从而导致膨胀不好维护的问题。在Spring2.5中开始支持：`@Component`、`@Repository`、`@Service`、`@Controller`等注解定义bean。<br />如果有看过这些注解的源码的话，就会惊奇得发现：其实后三种注解也是`@Component`。<br />![2021-07-10-08-26-45-679806.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879213452-ab3227fe-da1d-4d2b-9f47-cdd282c22ac4.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u680fa3ab&originHeight=524&originWidth=1080&originalType=binary&ratio=1&size=122997&status=done&style=none&taskId=u2d98840e-5926-478c-81d6-09c314caaca)![2021-07-10-08-26-45-794774.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879213518-ac76cf04-5794-454a-bda7-142d7cc1a416.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u5c2076c3&originHeight=508&originWidth=1080&originalType=binary&ratio=1&size=142468&status=done&style=none&taskId=u3d9037e0-6fb4-4dd7-9676-5a82812b885)![2021-07-10-08-26-45-922461.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879213542-7941b768-75be-409e-856c-4552a8a36a8e.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u95ba8c75&originHeight=532&originWidth=1080&originalType=binary&ratio=1&size=144048&status=done&style=none&taskId=u20b5c62d-07a5-4f32-8794-f1c883db608)<br />`@Component`系列注解的出现，带来了极大的便利。不需要像以前那样在bean.xml文件中配置bean了，现在只用在类上加`Component`、`Repository`、`Service`、`Controller`，这四种注解中的任意一种，就能轻松完成bean的定义。
```java
@Service
public class PersonService {
    public String get() {
        return "data";
    }
}
```
其实，这四种注解在功能上没有特别的区别，不过在业界有个不成文的约定：

- Controller 一般用在控制层
- Service 一般用在业务层
- Repository 一般用在数据层
- Component 一般用在公共组件上

不过，需要特别注意的是，通过这种`@Component`扫描注解的方式定义bean的前提是：需要先配置扫描路径。<br />目前常用的配置扫描路径的方式如下：

1. 在applicationContext.xml文件中使用`<context:component-scan>`标签。例如：
```xml
<context:component-scan base-package="com.sue.cache" />
```

2. 在SpringBoot的启动类上加上`@ComponentScan`注解，例如：
```java
@ComponentScan(basePackages = "com.sue.cache")
@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        new SpringApplicationBuilder(Application.class).web(WebApplicationType.SERVLET).run(args);
    }
}
```

2. 直接在`SpringBootApplication`注解上加，它支持`ComponentScan`功能：
```java
@SpringBootApplication(scanBasePackages = "com.sue.cache")
public class Application {
    
    public static void main(String[] args) {
        new SpringApplicationBuilder(Application.class).web(WebApplicationType.SERVLET).run(args);
    }
}
```
当然，如果需要扫描的类跟SpringBoot的入口类，在同一级或者子级的包下面，无需指定`scanBasePackages`参数，Spring默认会从入口类的同一级或者子级的包去找。
```java
@SpringBootApplication
public class Application {
    
    public static void main(String[] args) {
        new SpringApplicationBuilder(Application.class).web(WebApplicationType.SERVLET).run(args);
    }
}
```
此外，除了上述四种`@Component`注解之外，SpringBoot还增加了`@RestController`注解，它是一种特殊的`@Controller`注解，所以也是`@Component`注解。<br />`@RestController`还支持`@ResponseBody`注解的功能，即将接口响应数据的格式自动转换成json。<br />![2021-07-10-08-26-46-032866.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879225881-f0f2fc94-05c7-422a-8e30-8b4d0621b367.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u5e47fd70&originHeight=597&originWidth=1080&originalType=binary&ratio=1&size=159486&status=done&style=none&taskId=u53804f0a-3a3b-46cf-9e5d-51d67d4c13e)<br />`@Component`是日常工作中最多的定义bean的方式。
<a name="VQCSE"></a>
## 3、JavaConfig
`@Component`系列注解虽说使用起来非常方便，但是bean的创建过程完全交给Spring容器来完成，没办法自己控制。<br />Spring从3.0以后，开始支持JavaConfig的方式定义bean。它可以看做Spring的配置文件，但并非真正的配置文件，需要通过编码java代码的方式创建bean。例如：
```java
@Configuration
public class MyConfiguration {

    @Bean
    public Person person() {
        return new Person();
    }
}
```
在JavaConfig类上加`@Configuration`注解，相当于配置了`<beans>`标签。而在方法上加`@Bean`注解，相当于配置了`<bean>`标签。<br />此外，SpringBoot还引入了一些列的`@Conditional`注解，用来控制bean的创建。
```java
@Configuration
public class MyConfiguration {

    @ConditionalOnClass(Country.class)
    @Bean
    public Person person() {
        return new Person();
    }
}
```
`@ConditionalOnClass`注解的功能是当项目中存在`Country`类时，才实例化Person类。换句话说就是，如果项目中不存在Country类，就不实例化Person类。<br />这个功能非常有用，相当于一个开关控制着Person类，只有满足一定条件才能实例化。<br />Spring中使用比较多的Conditional还有：

- `ConditionalOnBean`
- `ConditionalOnProperty`
- `ConditionalOnMissingClass`
- `ConditionalOnMissingBean`
- `ConditionalOnWebApplication`

下面用一张图整体认识一下`@Conditional`家族：<br />![2021-07-10-08-26-46-204781.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879241050-729f3ca2-9c79-4fa1-a8fb-af0bd2da5866.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u25c713a5&originHeight=1116&originWidth=954&originalType=binary&ratio=1&size=321765&status=done&style=shadow&taskId=u74263018-de4b-4135-b962-cb68e74e294)<br />有了这些功能，终于可以告别麻烦的xml时代了。
<a name="oBFUw"></a>
## 4、`Import`注解
通过前面介绍的`@Configuration`和`@Bean`相结合的方式，可以通过代码定义bean。但这种方式有一定的局限性，它只能创建该类中定义的bean实例，不能创建其他类的bean实例，如果想创建其他类的bean实例该怎么办呢？<br />这时可以使用`@Import`注解导入。
<a name="IWKAu"></a>
### 4.1 普通类
Spring4.2之后`@Import`注解可以实例化普通类的bean实例。例如：<br />先定义了Role类：
```java
@Data
public class Role {
    private Long id;
    private String name;
}
```
接下来使用`@Import`注解导入Role类：
```java
@Import(Role.class)
@Configuration
public class MyConfig {
}
```
然后在调用的地方通过`@Autowired`注解注入所需的bean。
```java
@RequestMapping("/")
@RestController
public class TestController {

    @Autowired
    private Role role;

    @GetMapping("/test")
    public String test() {
        System.out.println(role);
        return "test";
    }
}
```
没有在任何地方定义过Role的bean，但Spring却能自动创建该类的bean实例，这是为什么呢？<br />这正是`@Import`注解的强大之处。<br />此时，有些朋友可能会问：`@Import`注解能定义单个类的bean，但如果有多个类需要定义bean该怎么办呢？这个需求`@Import`注解也支持。
```java
@Import({Role.class, User.class})
@Configuration
public class MyConfig {
}
```
甚至，如果想偷懒，不想写这种MyConfig类，springboot也欢迎。
```java
@Import({Role.class, User.class})
@SpringBootApplication(exclude = {DataSourceAutoConfiguration.class,
        DataSourceTransactionManagerAutoConfiguration.class})
public class Application {

    public static void main(String[] args) {
        new SpringApplicationBuilder(Application.class).web(WebApplicationType.SERVLET).run(args);
    }
}
```
可以将`@Import`加到SpringBoot的启动类上。<br />这样也能生效？<br />SpringBoot的启动类一般都会加`@SpringBootApplication`注解，该注解上加了`@SpringBootConfiguration`注解。<br />![2021-07-10-08-26-46-371894.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879254601-6cfef04f-f357-4d90-8097-da2017650e48.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=ud0af9641&originHeight=796&originWidth=1080&originalType=binary&ratio=1&size=229784&status=done&style=none&taskId=uc1971e7d-f640-40c5-af02-5a75cf31aa6)<br />而`@SpringBootConfiguration`注解，上面又加了`@Configuration`注解<br />![2021-07-10-08-26-46-479028.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879263761-f68c9ac5-261a-41b1-b9e3-175c72714fa1.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=uc9612d59&originHeight=288&originWidth=658&originalType=binary&ratio=1&size=33276&status=done&style=none&taskId=uf3d349b4-50b5-4418-b13d-04b918da8ba)<br />所以，SpringBoot启动类本身带有`@Configuration`注解的功能。
<a name="ydxTG"></a>
### 4.2 `Configuration`类
上面介绍了`@Import`注解导入普通类的方法，它同时也支持导入`Configuration`类。<br />先定义一个`Configuration`类：
```java
@Configuration
public class MyConfig2 {

    @Bean
    public User user() {
        return  new User();
    }

    @Bean
    public Role role() {
        return new Role();
    }
}
```
然后在另外一个`Configuration`类中引入前面的`Configuration`类：
```java
@Import({MyConfig2.class})
@Configuration
public class MyConfig {
}
```
这种方式，如果MyConfig2类已经在Spring指定的扫描目录或者子目录下，则MyConfig类会显得有点多余。因为MyConfig2类本身就是一个配置类，它里面就能定义bean。<br />但如果MyConfig2类不在指定的Spring扫描目录或者子目录下，则通过MyConfig类的导入功能，也能把MyConfig2类识别成配置类。这就有点厉害了喔。<br />其实下面还有更高端的玩法。<br />swagger作为一个优秀的文档生成框架，在Spring项目中越来越受欢迎。接下来，以swagger2为例，介绍一下它是如何导入相关类的。<br />众所周知，引入swagger相关jar包之后，只需要在SpringBoot的启动类上加上`@EnableSwagger2`注解，就能开启swagger的功能。<br />其中`@EnableSwagger2`注解中导入了`Swagger2DocumentationConfiguration`类。<br />![2021-07-10-08-26-46-567094.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879280579-8d1ce641-7bbb-48ca-94c8-3f79468440a8.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=ud832a9ba&originHeight=244&originWidth=954&originalType=binary&ratio=1&size=48439&status=done&style=none&taskId=uc66e6812-1f75-494e-a4a9-df894a66fbf)<br />该类是一个Configuration类，它又导入了另外两个类：

- `SpringfoxWebMvcConfiguration`
- `SwaggerCommonConfiguration`

![2021-07-10-08-26-46-669079.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879290443-6225af75-68be-433c-9bd9-895cf04ab53d.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u931c05fb&originHeight=627&originWidth=1080&originalType=binary&ratio=1&size=209057&status=done&style=none&taskId=ua63d3346-15ae-49de-9663-f0965a99c92)<br />`SpringfoxWebMvcConfiguration`类又会导入新的`Configuration`类，并且通过`@ComponentScan`注解扫描了一些其他的路径。<br />![2021-07-10-08-26-46-848353.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879301660-96e86101-7eca-4eba-bad3-31f0ade5f917.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=ub102851e&originHeight=888&originWidth=1080&originalType=binary&ratio=1&size=279412&status=done&style=none&taskId=uf8106d29-ad3b-4229-8750-8b306503c88)<br />`SwaggerCommonConfiguration`同样也通过`@ComponentScan`注解扫描了一些额外的路径。<br />![2021-07-10-08-26-46-956354.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879311282-1ecf0d7e-e600-45d2-bf1c-11a86a732ba2.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=uc5b37168&originHeight=320&originWidth=686&originalType=binary&ratio=1&size=41530&status=done&style=none&taskId=uc3e62e2b-9086-41e8-9e0d-44147f829af)<br />如此一来，通过一个简单的`@EnableSwagger2`注解，就能轻松的导入swagger所需的一系列bean，并且拥有swagger的功能。
<a name="ku7eG"></a>
### 4.3 `ImportSelector`
上面提到的`Configuration`类，它的功能非常强大。但怎么说呢，它不太适合加复杂的判断条件，根据某些条件定义这些bean，根据另外的条件定义那些bean。<br />那么，这种需求该怎么实现呢？<br />这时就可以使用`ImportSelector`接口了。<br />首先定义一个类实现`ImportSelector`接口：
```java
public class DataImportSelector implements ImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{"com.sue.async.service.User", "com.sue.async.service.Role"};
    }
}
```
重写`selectImports`方法，在该方法中指定需要定义bean的类名，注意要包含完整路径，而非相对路径。<br />然后在MyConfig类上`@Import`导入这个类即可：
```java
@Import({DataImportSelector.class})
@Configuration
public class MyConfig {
}
```
不过，这个注解还有更厉害的用途。<br />`@EnableAutoConfiguration`注解中导入了`AutoConfigurationImportSelector`类，并且里面包含系统参数名称：`spring.boot.enableautoconfiguration`。<br />![2021-07-10-08-26-47-048352.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879340175-c8aeccc2-2dba-4151-9f00-9dd0a3b3606f.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=ud0c03e5b&originHeight=771&originWidth=1080&originalType=binary&ratio=1&size=187066&status=done&style=none&taskId=u52a5c82c-c28c-49ee-9640-c92f2508b0b)<br />`AutoConfigurationImportSelector`类实现了`ImportSelector`接口。<br />![2021-07-10-08-26-47-225586.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879486857-df1c9e77-4d20-45fe-90ec-6e1ef3a7c39a.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u9f29fc3a&originHeight=232&originWidth=1080&originalType=binary&ratio=1&size=34338&status=done&style=none&taskId=u73eb0d15-405a-43e8-8bf4-39ef72fffd8)<br />并且重写了`selectImports`方法，该方法会根据某些注解去找所有需要创建bean的类名，然后返回这些类名。其中在查找这些类名之前，先调用`isEnabled`方法，判断是否需要继续查找。<br />![2021-07-10-08-26-47-417881.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879377088-eefc2afc-b389-42e1-8e67-d4aad0efb4b3.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u77860f07&originHeight=558&originWidth=1080&originalType=binary&ratio=1&size=259914&status=done&style=none&taskId=u495d6810-6015-42d1-bf38-f517c5866bb)<br />该方法会根据`ENABLED_OVERRIDE_PROPERTY`的值来作为判断条件。<br />![2021-07-10-08-26-47-526196.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879501811-3e7ee6e8-bf5b-4d21-b66e-bdcbbcae4cc5.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u9b21e91f&originHeight=294&originWidth=1080&originalType=binary&ratio=1&size=86243&status=done&style=none&taskId=ub6a6ce4a-e45e-4799-99be-0e23a7b6429)<br />而这个值就是`spring.boot.enableautoconfiguration`。<br />换句话说，这里能根据系统参数控制bean是否需要被实例化。<br />实现`ImportSelector`接口的好处主要有以下两点：

1. 把某个功能的相关类，可以放到一起，方面管理和维护。
2. 重写`selectImports`方法时，能够根据条件判断某些类是否需要被实例化，或者某个条件实例化这些bean，其他的条件实例化那些bean等。能够非常灵活的定制化bean的实例化。
<a name="j83lo"></a>
### 4.4 `ImportBeanDefinitionRegistrar`
通过上面的这种方式，确实能够非常灵活的自定义bean。<br />但它的自定义能力，还是有限的，它没法自定义bean的名称和作用域等属性。<br />有需求，就有解决方案。<br />接下来，一起看看`ImportBeanDefinitionRegistrar`接口的神奇之处。<br />先定义`CustomImportSelector`类实现`ImportBeanDefinitionRegistrar`接口：
```java
public class CustomImportSelector implements ImportBeanDefinitionRegistrar {

    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
        RootBeanDefinition roleBeanDefinition = new RootBeanDefinition(Role.class);
        registry.registerBeanDefinition("role", roleBeanDefinition);

        RootBeanDefinition userBeanDefinition = new RootBeanDefinition(User.class);
        userBeanDefinition.setScope(ConfigurableBeanFactory.SCOPE_PROTOTYPE);
        registry.registerBeanDefinition("user", userBeanDefinition);
    }
}
```
重写`registerBeanDefinitions`方法，在该方法中可以获取`BeanDefinitionRegistry`对象，通过它去注册bean。不过在注册bean之前，先要创建`BeanDefinition`对象，它里面可以自定义bean的名称、作用域等很多参数。<br />然后在MyConfig类上导入上面的类：
```java
@Import({CustomImportSelector.class})
@Configuration
public class MyConfig {
}
```
所熟悉的fegin功能，就是使用`ImportBeanDefinitionRegistrar`接口实现的：<br />![2021-07-10-08-26-47-668779.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879522872-7c69d5b1-ac39-41e2-b417-1b746b09b6cc.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u2d76941d&originHeight=572&originWidth=1080&originalType=binary&ratio=1&size=163160&status=done&style=none&taskId=u857ff5a0-dcff-4dbb-b25b-27c99def3fe)
<a name="Ufuzl"></a>
## 5、`PostProcessor`
除此之外，Spring还提供了专门注册bean的接口：`BeanDefinitionRegistryPostProcessor`。<br />该接口的方法`postProcessBeanDefinitionRegistry`上有这样一段描述：<br />![2021-07-10-08-26-47-820042.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879532252-57defc26-2dbe-4e00-8891-c96fb07f1e05.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=ubba33cb1&originHeight=374&originWidth=1080&originalType=binary&ratio=1&size=172489&status=done&style=none&taskId=u32a77c9e-4f9c-431b-b610-a26938a371f)<br />修改应用程序上下文的内部bean定义注册表标准初始化。所有常规bean定义都将被加载，但是还没有bean被实例化。这允许进一步添加在下一个后处理阶段开始之前定义bean。<br />如果用这个接口来定义bean，要做的事情就变得非常简单了。只需定义一个类实现`BeanDefinitionRegistryPostProcessor`接口。
```java
@Component
public class MyRegistryPostProcessor implements BeanDefinitionRegistryPostProcessor {
    @Override
    public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
        RootBeanDefinition roleBeanDefinition = new RootBeanDefinition(Role.class);
        registry.registerBeanDefinition("role", roleBeanDefinition);

        RootBeanDefinition userBeanDefinition = new RootBeanDefinition(User.class);
        userBeanDefinition.setScope(ConfigurableBeanFactory.SCOPE_PROTOTYPE);
        registry.registerBeanDefinition("user", userBeanDefinition);
    }

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
    }
}
```
重写`postProcessBeanDefinitionRegistry`方法，在该方法中能够获取`BeanDefinitionRegistry`对象，它负责bean的注册工作。<br />里面还多了一个`postProcessBeanFactory`方法，没有做任何实现。<br />这个方法其实是它的父接口：`BeanFactoryPostProcessor`里的方法。<br />![2021-07-10-08-26-47-913796.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879567067-fefc7120-1a3c-41f4-9ef4-73a19b68e93b.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u0fa2164a&originHeight=398&originWidth=1080&originalType=binary&ratio=1&size=151336&status=done&style=none&taskId=u6b789c34-e3d5-490a-9411-697cb4db0f6)<br />在应用程序上下文的标准bean工厂之后修改其内部bean工厂初始化。所有bean定义都已加载，但没有bean将被实例化。这允许重写或添加属性甚至可以初始化bean。
```java
@Component
public class MyPostProcessor implements BeanFactoryPostProcessor {

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        DefaultListableBeanFactory registry = (DefaultListableBeanFactory)beanFactory;
        RootBeanDefinition roleBeanDefinition = new RootBeanDefinition(Role.class);
        registry.registerBeanDefinition("role", roleBeanDefinition);

        RootBeanDefinition userBeanDefinition = new RootBeanDefinition(User.class);
        userBeanDefinition.setScope(ConfigurableBeanFactory.SCOPE_PROTOTYPE);
        registry.registerBeanDefinition("user", userBeanDefinition);
    }
}
```
既然这两个接口都能注册bean，那么他们有什么区别？

- `BeanDefinitionRegistryPostProcessor` 更侧重于bean的注册
- `BeanFactoryPostProcessor` 更侧重于对已经注册的bean的属性进行修改，虽然也可以注册bean。

此时，有些朋友可能会问：既然拿到`BeanDefinitionRegistry`对象就能注册bean，那通过`BeanFactoryAware`的方式是不是也能注册bean呢？<br />从下面这张图能够看出`DefaultListableBeanFactory`就实现了`BeanDefinitionRegistry`接口。<br />![2021-07-10-08-26-48-007543.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879578204-a1e77593-a1d8-4048-a21f-dc648fc393b7.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=ud5e803a9&originHeight=482&originWidth=1080&originalType=binary&ratio=1&size=77819&status=done&style=none&taskId=u9b72768f-070d-4f59-87ca-8cf61ca62e8)<br />这样一来，如果能够获取`DefaultListableBeanFactory`对象的实例，然后调用它的注册方法，不就可以注册bean了？<br />说时迟那时快，定义一个类实现`BeanFactoryAware`接口：
```java
@Component
public class BeanFactoryRegistry implements BeanFactoryAware {
    @Override
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
        DefaultListableBeanFactory registry = (DefaultListableBeanFactory) beanFactory;
        RootBeanDefinition rootBeanDefinition = new RootBeanDefinition(User.class);
        registry.registerBeanDefinition("user", rootBeanDefinition);

        RootBeanDefinition userBeanDefinition = new RootBeanDefinition(User.class);
        userBeanDefinition.setScope(ConfigurableBeanFactory.SCOPE_PROTOTYPE);
        registry.registerBeanDefinition("user", userBeanDefinition);
    }
}
```
重写`setBeanFactory`方法，在该方法中能够获取`BeanFactory`对象，它能够强制转换成`DefaultListableBeanFactory`对象，然后通过该对象的实例注册bean。<br />发现竟然报错了：<br />![2021-07-10-08-26-48-109989.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879592111-85413895-1547-4cda-aed1-0d05872e9522.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=u7735b599&originHeight=263&originWidth=1080&originalType=binary&ratio=1&size=36098&status=done&style=none&taskId=ua575fb5c-8f0a-4158-b372-9ce2054f19f)<br />为什么会报错？<br />Spring中bean的创建过程顺序大致如下：<br />![2021-07-10-08-26-48-220900.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625879601643-f2bd4569-749f-4e03-ba6f-bee188171c8e.png#clientId=ubd2f3e3b-1ee6-4&from=ui&id=ua73bba3d&originHeight=838&originWidth=356&originalType=binary&ratio=1&size=107187&status=done&style=shadow&taskId=u88e168ad-e8ad-4890-956b-a2dc40c4740)<br />`BeanFactoryAware`接口是在bean创建成功，并且完成依赖注入之后，在真正初始化之前才被调用的。在这个时候去注册bean意义不大，因为这个接口是获取bean的，并不建议去注册bean，会引发很多问题。<br />此外，`ApplicationContextRegistry`和`ApplicationListener`接口也有类似的问题，可以用他们获取bean，但不建议用它们注册bean。
