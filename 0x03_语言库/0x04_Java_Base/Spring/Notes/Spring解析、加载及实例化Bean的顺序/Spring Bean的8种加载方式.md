JavaSpring<br />注意以下是 Spring 中 Bean 的加载方式，不是 SpringBoot，但其中的很多东西是相通的，尤其是 Bean 的注入方式、适用场景等，在本文中也有介绍的。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1667657415808-b759218a-5779-4330-9ad7-efb01d6b7501.jpeg#averageHue=%23faf4ee&clientId=u017cc8bb-8a9f-4&from=paste&id=u4bfa855e&originHeight=467&originWidth=796&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4dabe14b-6ad1-4314-aa09-2309269b949&title=)
<a name="EVUZI"></a>
## **1、xml +<bean>**
被配置的 bean 需要有无参数的构造函数
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
  <!-- xml方式声明自己开发的bean -->
  <bean id="user" class="cn.sticki.blog.pojo.domain.User" />
    <!-- xml方式声明第三方bean -->
    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource"/>
</beans>
```
<a name="TKeTV"></a>
## **2、**`**xml:context**`**+注解(**`**@Component**`**+4个**`**@Bean**`**)**

- 使用组件扫描，指定加载bean的位置，spring会自动扫描这个包下的文件。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="
       http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd">

    <!-- 组件扫描，指定加载bean的位置 -->
    <context:component-scan base-package="cn.sticki.bean,cn.sticki.config"/>
</beans>
```

- 然后在需要被加载的类名上添加`@Component`注解。也可以使用`@Controller`、`@Service`、`@Repository`定义bean。
```java
@Service
publice class UserServiceImpl implements UserService {
}
```

- 使用`@Bean`定义第三方bean，并将所在类定位为配置类或Bean
```java
@Configuration  // 或使用@Component
public class DBConfig {
    @Bean
    public DruidDataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        return ds;
    } 
}
```
<a name="BXPkJ"></a>
## **3、配置类+扫描+注解(**`**@Component**`**+4个**`**@Bean**`**)**
使用 `AnnotationConfigApplicationContext(SpringConfig.class);` 来获取 `ApplicationContext`
```java
public class AppObject {
    public static void main() {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        SpringConfig config = ctx.getBean("Config", SpringConfig.class);
        // 两次获取的是同一个对象
        config.user();
        config.user();
    }
}
```
和上面的第二种有点类似，就是包的扫描方式有所改变。
```java
@ComponentScan({"cn.sticki.bean","cn.sticki.config"})
public class SpringConfig {
    @Bean
    public DogFactoryBean dog(){
        return new DogFactoryBean();
    }
}
```
<a name="oklQ2"></a>
### **3.1**`**FactoryBean**`**接口**
初始化实现`FactoryBean`接口的类，实现对bean加载到容器之前的批处理操作。
```java
public class DogFactoryBean implements FactoryBean<Dog> {
    @Override
    public Dog getObject() throws Exception {
        return new Dog();
    }
    @Override
    public Class<?> getObjectType() {
        return Dog.class;
    }
}
```
在下面的bean中，显示的表示为配置`DogFactoryBean`，但实际上配置的是 Dog 。
```java
@Component
public class SpringConfig {
    @Bean
    public DogFactoryBean dog(){
        return new DogFactoryBean();
    }
}
```
<a name="Zoatg"></a>
### **3.2**`**@ImportResource**`**注解**
用于加载配置类并加载配置文件（系统迁移）
```java
@Configuration
@ComponentScan("cn.sticki.bean")
@ImportResource("applicationContext.xml")
public class SpringConfig {
}
```
<a name="o63ds"></a>
### **3.3**`**proxyBeanMethods**`**属性**
使用 `proxyBeanMethods = true` 可以保障调用此类中的方法得到的对象是从容器中获取的，而不是重新创建的，但要求必须是通过此类调用方法获得的bean。
```java
@Configuration(proxyBeanMethods = true)
public class SpringConfig {
    @Bean
    public User user() {
        System.out.println("user init...");
        return new User();
    }
}
```
<a name="WqEE3"></a>
## **4、**`**@Import**`**导入bean的类**
使用`@Import`注解导入要注入的bean对应的字节码
```java
@Import(User.class)
public class SpringConfig {
}
```
而被导入的bean无需使用注解声明为bean
```java
public class User{
}
```
这种形式可以有效的降低源代码与spring技术的耦合度（无侵入），在spring技术底层及诸多框架的整合中大量使用。<br />使用这种方法可以加在配置类，且也可以加在配置类当中的bean。
<a name="ql9AW"></a>
## **5、**`**AnnotationConfigApplicationContext**`**调用**`**register**`**方法**
在容器初始化完毕后使用容器对象手动注入bean
```java
public class App {
    public static void main() {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        ctx.register(User.class);
        // 打印容器中当前的所有bean
        String[] names = ctx.getBeanDefinitionNames();
        for (String name: names) {
            System.out.println(name);
        }
    }
}
```
必须在容器初始化之后才能使用这种方法。如果重复加载同一个bean，后面加载的会覆盖前面加载的。
<a name="yfq5u"></a>
## **6、**`**@Import**`**导入**`**ImportSelector**`**接口**
导入实现了`ImportSelector`接口的类，实现对导入源的编程式处理
```java
public class MyImportSelector implements ImportSelector {
    public String selectImports(AnnotationMetadata metadata) {
        // 使用metadata可以获取到导入该类的元类的大量属性，通过对这些属性进行判断，可以达到动态注入bean的效果
        boolean flag = metadata.hasAnnotation("org.springframework.context.annotation.Import");
        if(flag) {
            return new String[]{"cn.sticki.pojo.User"};
        }
        return new String[]{"cn.sticki.pojo.Dog"};
    }
}
```
调用处：
```java
@Import(MyImportSelector.class)
public class SpringConfig {
}
```
<a name="MEKHh"></a>
## **7、**`**@Import**`**导入**`**ImportBeanDefinitionRegistrar**`**接口**
导入实现了`ImportBeanDefinitionRegistrar`接口的类，通过`BeanDefinition`的注册器注册实名bean，实现对容器中bean的绑定，例如对现有bean的覆盖，进而达成不修改源代码的情况下更换实现的效果。
```java
public class MyImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {
    public String registrarBeanDefinitions(AnnotationMetadata metadata, BeanDefinitionRegistry registry) {
        // 使用元数据去做判定，然后再决定要注入哪些bean
        BeanDefinition beanDefinition = BeanDefinitionBuilder.rootBeanDefinition(User.class).getBeanDefinition();
        registry.registerBeanDefinition("user",beanDefinition);
    }
}
```
调用处和上面第六种方式差不多。
<a name="vGws8"></a>
## **8、**`**@Import**`**导入**`**BeanDefinitionRegistryPostProcessor**`**接口**
导入实现了`BeanDefinitionRegistryPostProcessor`接口的类，通过`BeanDefinition`的注册器注册实名bean，实现对容器中bean的最终裁定。其在`@Import`中的加载顺序为最后一个加载，可以用来做bean覆盖的最终裁定。
```java
public class MyPostProcessor implements BeanDefinitionRegistryPostProcessor {
    @Override
    public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
        // 注意这里注册的是BookServiceImpl4
        BeanDefinition beanDefinition = BeanDefinitionBuilder.rootBeanDefinition(BookServiceImpl4.class).getBeanDefinition();
        registry.registerBeanDefinition("bookService",beanDefinition);
    }
    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
    }
}
```
调用处：
```java
// 按照先后顺序加载，但 MyPostProcessor.class 最后才加载
@Import(BookServiceImpl1.class,MyPostProcessor.class, BookServiceImpl.class, MyImportSelector.class)
public class SpringConfig {
}
```
<a name="kfebw"></a>
## **后记**
这八种加载方式几乎可以满足需要的全部场景了，但一般场景下，用的其实也就只有那两三种，真正掌握这八种加载方式的朋友，肯定是大佬了🧐🧐。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1667657415814-c3e3a8ed-e94b-4b6f-838b-ab958e5ea14d.jpeg#averageHue=%23f7f0e8&clientId=u017cc8bb-8a9f-4&from=paste&id=ue77ae86d&originHeight=985&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1f2ca26c-00bf-45a3-8fb2-071a4ff4fef&title=)
