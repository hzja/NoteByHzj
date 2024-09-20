Java SpringBoot
<a name="PFWCx"></a>
### 1、`@Value`注解如何使用?
在项目中经常需要用到使用读取配置文件，进行属性值的注入很方便；<br />很多时候不想手动的去初始化配置，在Spring就提供了很强大的属性依赖配置注解`@Value`来实现，在实现bean创建后，自动实现属性的注入功能。接下来先从一个简单的例子来描述，如何使用`@Value`注解，能达到什么效果，到最后自己动手实现自己的属性注解。
<a name="Z009S"></a>
#### 1.1 准备
创建Maven项目，导入Spring相关的包。

- 配置一个注解类，能让Spring管理，AppConfig
```java
import org.springframework.context.annotation.ComponentScan;
import org.springframework.stereotype.Component;
@ComponentScan("com.mp.aop.annotation.day02.service")
public class MyConfig {
}
```

- 此处配置`@ComponentScan` 注解扫描指定的package及其子包；将其所有实现了`@bean` `@Component` 等注解的类统一交由Spring IOC来管理
- 准备程序入口，访问整体的spring bean
```java
import com.mp.aop.annotation.day02.config.MyConfig;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import java.util.stream.Stream;

public class MainManager {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MyConfig.class);

        // 已经完成了整个spring容器的初始化；这里先简单的获取下，我们容器负责管理的所有bean
        Stream.of(context.getBeanDefinitionNames()).forEach(s -> System.out.println(s));

    }
}
```

- 执行主入口程序，可以获取整个Spring管理系统，自定义获取bean及其操作。
```java
org.springframework.context.annotation.internalConfigurationAnnotationProcessor
org.springframework.context.annotation.internalAutowiredAnnotationProcessor
org.springframework.context.annotation.internalCommonAnnotationProcessor
org.springframework.context.event.internalEventListenerProcessor
org.springframework.context.event.internalEventListenerFactory
myConfig
```
可以看到，刚开始程序会创建6个bean，其中5个为Spring容器需要初始化（后面会详细介绍分析）；另外一个为自己添加的配置类。

- 添加一个带`@Value`注解的类 Person
```java
import com.mp.aop.annotation.MyAnnotation;
import lombok.Data;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.stereotype.Component;

/**
 * ClassName: Person
 * version    V1.0
 */
@Component
@Data
public class Person {
    static {
        System.out.println("this is person Static");
    }

    @Value("mp")
    String name;   

    @Value("10")
    int age;

    String addr;



    public Person(String name, int age,String addr) {
        this.name = name;
        this.age = age;
        this.addr = addr;
        System.out.println("constuctor ...");
    }

    public void setName(String name) {
        this.name = name;
        System.out.println("person.setName。。。。"+name);
    }

    public void setAge(int age) {
        this.age = age;
        System.out.println("person.setAge: "+ age);
    }

    public void setAddr(String addr) {
        this.addr = addr;
        System.out.println("person.setAddr: "+addr);
    }

    public Person() {
        System.out.println("constructor null");
    }

    public void p1(){
        System.out.println("this name:"+this.name+"  this age:"+this.age);
    }


    /*@Bean("p1")
    public Person getPerson(){
        System.out.println("自定义...");
        return new Person("zhuge",100,"wuhan");
    }*/
}
```

- 再次执行，main入口程序，可以看到，Spring初始化的bean会多了一个person
- 在main程序中，获取car这个bean，打印其信息
```java
// 获取car这个bean对象，打印其对象信息
System.out.println(context.getBean(Person.class));
```
打印结果：
```java
Person(name=mp, age=10, addr=null)
```
至此，结果显示在属性上的注解值成功的注入到bean对象
<a name="WmTqd"></a>
### 2、探究`@Value`注解的原理
在上面的使用例子中，成功的注入了属性值；那想一下，Spring到底是怎么创建带有属性的对象，并进行管理的呢？<br />先大胆猜测下，有以下几种可能

- `@Value`直接调用带有属性的构造函数，创建对象并交由Spring来统一管理；
- 分两步，先调用空构造器创建一个属性为空的对象，再通过set方法来设置属性值；

到底是哪一种，通过程序来验证…<br />在`new AnnotationConfigApplicationContext(MyConfig.class)`打上断点，debug模式走起…
```java
public AnnotationConfigApplicationContext(Class<?>... annotatedClasses) {
  this();  // 构造器，初始化AnnotatedBeanDefinitionReader和  ClassPathBeanDefinitionScanner并准备好5个sping系统需要的BeanDefinition.
  register(annotatedClasses); // 注册我们的配置类AppConfig 对应的BeanDefinition，此时我们的beanFactory中有6个beanDefinitionName了
  refresh(); // 重点流程
}
```
这里发现，Spring在管理bean的时候，不是直接就创建对象扔进对象池，而是先创建起对应的`BeanDefinition`，这个还不是真正的bean，还未创建对象。<br />接着进入`refresh()`
```java
synchronized (this.startupShutdownMonitor) {
    // Prepare this context for refreshing.
    prepareRefresh();

    // Tell the subclass to refresh the internal bean factory.
    ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();

    // Prepare the bean factory for use in this context.
    prepareBeanFactory(beanFactory);

    try {
        // Allows post-processing of the bean factory in context subclasses.
        postProcessBeanFactory(beanFactory);

        // Invoke factory processors registered as beans in the context.
        invokeBeanFactoryPostProcessors(beanFactory);  // @1 说明

        // Register bean processors that intercept bean creation.
        registerBeanPostProcessors(beanFactory);  

        // Initialize message source for this context.
        initMessageSource();

        // Initialize event multicaster for this context.
        initApplicationEventMulticaster();

        // Initialize other special beans in specific context subclasses.
        onRefresh();

        // Check for listener beans and register them.
        registerListeners();

        // Instantiate all remaining (non-lazy-init) singletons.
        finishBeanFactoryInitialization(beanFactory);   // @2  说明

        // Last step: publish corresponding event.
        finishRefresh();  // 
    }
```
@1说明：

- 会创建好Spring需要的5个bean
- 根据`@ComponentScan`扫描的包路径，`ClassPathBeanDefinitionScanner` 去target根据文件系统找到对应的class文件，并根据是否有注解来判断，决定是否要加入到`beanDefinitionNames` 列表中，为后面初始化做准备。
- 所以，这里`beanDefinitionNames` 列表扫描会新增1个person -bean

@2 说明<br />实例化通过包扫描的bean， 实例化person对象，并完成属性注入，重点来了…

- 继续断点，`finishBeanFactoryInitialization(beanFactory);`
- -> 经过一系列判断后，调用`beanFactory.preInstantiateSingletons();`进行实例化。
- -> 遍历`BeanDefinitionNames`来判断是否要创建bean，针对上面已经创建过的5个bean，见到单例池中存在，则不再创建，这里所有的bean都是单例。
- 增加断点条件`beanName.equals("person")`
- -> `getBean(beanName);` 继续调用

`Object sharedInstance = getSingleton(beanName);` 这里是否单例池缓存中取对象，由于之前没有创建，所以这里为null，流程继续往下走<br />`mbd.isSingleton()` 为默认单例，开始创建实例

- 关键的步骤：RootBeanDefinition
- `resolveBeanClass(mbd, beanName)`根据beanName获取class
- `instanceWrapper = createBeanInstance(beanName, mbd, args);`这里调用空构造器，创建一个空对象

接下来，哪里实现了属性的注入呢？来，继续往下走…

- `populateBean(beanName, mbd, instanceWrapper);`根据 `instanceof InstantiationAwareBeanPostProcessor` 判断结果，dp为`AutowiredAnnotationBeanPostProcessor` 类， --debug发现这个`postProcessor`的服务会处理`@Autowired` 和`@Value`来个注解
- `PropertyValues pvsToUse = ibp.postProcessProperties(pvs, bw.getWrappedInstance(), beanName);`会做属性注入，继续debug进去。
- 可以看到想要的代码了，`postProcessProperties(PropertyValues pvs, Object bean, String beanName)`注入属性值，实际调用方法。`metadata.inject(bean, beanName, pvs)` ，继续debug深入。
- 获取InjectedElement 集合，可以检查到两个注解属性，name age, 遍历属性字段，根据Field类做属性注入。从注解value中获取值
- `field.set(bean, value)` 至此，完成了 属性的注入
- debug信息查看person：`Person(name=mp, age=10, addr=null)`好了，完成梳理。
<a name="oSpCc"></a>
### 3、自定义实现`@MyValue`注解
<a name="dSwqd"></a>
#### 3.1 创建`@MyValue`注解类
```java
import java.lang.annotation.*;

@Target(ElementType.FIELD)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface MyValue {
    String value() default "";
}
```
<a name="gvbaw"></a>
#### 3.2 创建使用`@MyValue`注解的属性的类
```java
import com.mp.aop.annotation.day02.config.MyValue;
import lombok.Data;
import org.springframework.stereotype.Component;

/**
 * ClassName: Car
 * version    V1.0
 */
@Component
@Data
public class Car {

    @MyValue(value = "布加迪")
    String name;

    @MyValue(value = "6666w")
    String price;

    static {
        System.out.println("static car...");
    }

}
```
<a name="mcsrM"></a>
#### 3.3 自定义后置处理器 `MyPostProcessor`
这边采用的策略是，在完成spring bean构建之后，通过`beanFactory`获取对应的bean，做字段的依赖注入
```java
import com.mp.aop.annotation.day02.config.MyValue;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.SmartInitializingSingleton;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;

import java.util.stream.Stream;

/**
 * ClassName: ServicePostProcess
 * Function:  TODO
 * version    V1.0
 */
@Component
public class MyPostProcess implements SmartInitializingSingleton,BeanFactoryPostProcessor {
    BeanFactory beanFactory;
    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        System.out.println("this is factory...");
        this.beanFactory = beanFactory;

    }

    @Override
    public void afterSingletonsInstantiated() {
        System.out.println("process afterSingletons...");

        // 处理@MyValue注解
        Car car = beanFactory.getBean(Car.class);
        Stream.of(Car.class.getDeclaredFields())
                .filter(field -> field.getAnnotation(MyValue.class) != null)
                .forEach(field -> {
                    MyValue myValue = field.getAnnotation(MyValue.class);
                    try {
                        field.set(car,myValue.value());  // 属性注入.
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    }
                    // 设置属性
//                    System.out.println(field.getName());

                   // 
                    //1. 方法2，反射找到对应的set方法做属性的设置
                    /*String fieldName = field.getName();
                    Class<?> type = field.getType();
                    String setMethodName = "set".concat(fieldName.substring(0,1).toUpperCase()).concat(fieldName.substring(1));
                    try {
                        Method method = Car.class.getMethod(setMethodName, type);
                        method.invoke(car,myValue.value());
                    } catch (Exception e) {
                        e.printStackTrace();
                    }*/

                });

        System.out.println("car: "+ car);

    }

}
```
打印结果：
```
Car(name=布加迪, price=6666w)
```
完成自定义属性注解。<br />这里需要重点说如一下，自定义后置处理器，实现`SmartInitializingSingleton`，`BeanFactoryPostProcessor` 接口，需要重写`postProcessBeanFactory()` 和 `afterSingletonsInstantiated()` 方法

- `postProcessBeanFactory()`方法块，在Spring初始化bean之前就会调用执行；
- `afterSingletonsInstantiated()`方法会在完成bean的创建及管理之后执行。
