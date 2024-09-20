JavaSpringBoot
<a name="lR946"></a>
## `@Configuration`注解
<a name="g7hzR"></a>
### 用法
`@Configuration`这个注解可以加在类上，让这个类的功能等同于一个bean xml配置文件，如下：
```java
@Configuration
public class ConfigBean {

}
```
上面代码类似于下面的xml：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

</beans>
```
通过`AnnotationConfigApplicationContext`来加载`@Configuration`修饰的类，如下：
```java
AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ConfigBean.class);
```
此时ConfigBean类中没有任何内容，相当于一个空的xml配置文件，此时要在ConfigBean类中注册bean，那么就要用到`@Bean`注解了。
<a name="UOWv7"></a>
### 总结一下
`@Configuration`使用步骤：

1. 在类上使用`@Configuration`注解
2. 通过`AnnotationConfigApplicationContext`容器来加`@Configuration`注解修饰的类
<a name="b1F3E"></a>
## `@Bean`注解
<a name="ESnaX"></a>
### 用法
这个注解类似于bean xml配置文件中的bean元素，用来在Spring容器中注册一个bean。<br />`@Bean`注解用在方法上，表示通过方法来定义一个bean，默认将方法名称作为bean名称，将方法返回值作为bean对象，注册到Spring容器中。<br />如：
```java
@Bean
public User user1() {
    return new User();
}
```
`@Bean`注解还有很多属性，来看一下其源码：
```java
@Target({ElementType.METHOD, ElementType.ANNOTATION_TYPE}) //@1
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Bean {

    @AliasFor("name")
    String[] value() default {};

    @AliasFor("value")
    String[] name() default {};

    @Deprecated
    Autowire autowire() default Autowire.NO;

    boolean autowireCandidate() default true;

    String initMethod() default "";

    String destroyMethod() default AbstractBeanDefinition.INFER_METHOD;
}
```
@1：说明这个注解可以用在方法和注解类型上面。<br />每个参数含义：

1. value和name是一样的，设置的时候，这2个参数只能选一个，原因是`@AliasFor`导致的
2. `value`：字符串数组，第一个值作为bean的名称，其他值作为bean的别名
3. `autowire`：这个参数上面标注了`@Deprecated`，表示已经过期了，不建议使用了
4. `autowireCandidate`：是否作为其他对象注入时候的候选bean
5. `initMethod`：bean初始化的方法，这个和生命周期有关，以后详解
6. `destroyMethod`：bean销毁的方法，也是和生命周期相关的，以后详解
<a name="cDZ7Y"></a>
### 案例
<a name="gDNFa"></a>
#### User类
```java
package com.javacode2018.lesson001.demo20;

public class User {
}
```
<a name="gqwgW"></a>
#### Bean配置类：ConfigBean
```java
package com.javacode2018.lesson001.demo20;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class ConfigBean {

    //bean名称为方法默认值：user1
    @Bean
    public User user1() {
        return new User();
    }

    //bean名称通过value指定了：user2Bean
    @Bean("user2Bean")
    public User user2() {
        return new User();
    }

    //bean名称为：user3Bean，2个别名：[user3BeanAlias1,user3BeanAlias2]
    @Bean({"user3Bean", "user3BeanAlias1", "user3BeanAlias2"})
    public User user3() {
        return new User();
    }

}
```
上面通过`@Bean`注解定义了3个bean，比较简单
<a name="kfj50"></a>
#### 来个测试类
```java
package com.javacode2018.lesson001.demo20;

import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.util.Arrays;

public class ConfigurationTest {
    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ConfigBean.class);//@1
        for (String beanName : context.getBeanDefinitionNames()) {
            //别名
            String[] aliases = context.getAliases(beanName);
            System.out.println(String.format("bean名称:%s,别名:%s,bean对象:%s",
                                             beanName,
                                             Arrays.asList(aliases),
                                             context.getBean(beanName)));
        }
    }
}
```
@1：通过`AnnotationConfigApplicationContext`来加载配置类`ConfigBean`，会将配置类中所有的bean注册到Spring容器中<br />for循环中输出了bean名称、别名、bean对象
<a name="PtjpR"></a>
#### 运行test1方法输出
```
bean名称:org.springframework.context.annotation.internalConfigurationAnnotationProcessor,别名:[],bean对象:org.springframework.context.annotation.ConfigurationClassPostProcessor@3bd82cf5
bean名称:org.springframework.context.annotation.internalAutowiredAnnotationProcessor,别名:[],bean对象:org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor@544fa968
bean名称:org.springframework.context.annotation.internalCommonAnnotationProcessor,别名:[],bean对象:org.springframework.context.annotation.CommonAnnotationBeanPostProcessor@247bddad
bean名称:org.springframework.context.event.internalEventListenerProcessor,别名:[],bean对象:org.springframework.context.event.EventListenerMethodProcessor@d35dea7
bean名称:org.springframework.context.event.internalEventListenerFactory,别名:[],bean对象:org.springframework.context.event.DefaultEventListenerFactory@7770f470
bean名称:configBean,别名:[],bean对象:com.javacode2018.lesson001.demo20.ConfigBean$$EnhancerBySpringCGLIB$$dde45976@5e5d171f
bean名称:user1,别名:[],bean对象:com.javacode2018.lesson001.demo20.User@24313fcc
bean名称:user2Bean,别名:[],bean对象:com.javacode2018.lesson001.demo20.User@7d20d0b
bean名称:user3Bean,别名:[user3BeanAlias2, user3BeanAlias1],bean对象:com.javacode2018.lesson001.demo20.User@77f1baf5
```
上面的输出，主要关注与最后4行，前面的可以先忽略。<br />从输出中可以看出，有个名称为configBean的bean，正是ConfigBean这个类型，可以得出，被`@Configuration`修饰的类，也被注册到Spring容器中了<br />最后3行输出就是几个User的bean对象了。<br />**上面的用法应该很多人都比较熟悉，下面的属于重点了。**
<a name="uU9uL"></a>
## 去掉`@Configuration`会怎样？
来看一下没有`@Configuration`的时候，什么效果。
<a name="Xag08"></a>
#### 新建一个ConfigBean1类
内容和ConfigBean类一样，只是将`@Configuration`注解去掉了，如下：
```java
public class ConfigBean1 {

    //bean名称为方法默认值：user1
    @Bean
    public User user1() {
        return new User();
    }

    //bean名称通过value指定了：user2Bean
    @Bean("user2Bean")
    public User user2() {
        return new User();
    }

    //bean名称为：user3Bean，2个别名：[user3BeanAlias1,user3BeanAlias2]
    @Bean({"user3Bean", "user3BeanAlias1", "user3BeanAlias2"})
    public User user3() {
        return new User();
    }

}
```
<a name="vEJOp"></a>
#### 来个测试用例test2
代码类似于test1，给Spring容器传递ConfigBean1
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ConfigBean1.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        //别名
        String[] aliases = context.getAliases(beanName);
        System.out.println(String.format("bean名称:%s,别名:%s,bean对象:%s",
                                         beanName,
                                         Arrays.asList(aliases),
                                         context.getBean(beanName)));
    }
}
```
<a name="z7sTO"></a>
#### 运行输出
```
bean名称:org.springframework.context.annotation.internalConfigurationAnnotationProcessor,别名:[],bean对象:org.springframework.context.annotation.ConfigurationClassPostProcessor@333291e3
bean名称:org.springframework.context.annotation.internalAutowiredAnnotationProcessor,别名:[],bean对象:org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor@479d31f3
bean名称:org.springframework.context.annotation.internalCommonAnnotationProcessor,别名:[],bean对象:org.springframework.context.annotation.CommonAnnotationBeanPostProcessor@40ef3420
bean名称:org.springframework.context.event.internalEventListenerProcessor,别名:[],bean对象:org.springframework.context.event.EventListenerMethodProcessor@498d318c
bean名称:org.springframework.context.event.internalEventListenerFactory,别名:[],bean对象:org.springframework.context.event.DefaultEventListenerFactory@6e171cd7
bean名称:configBean1,别名:[],bean对象:com.javacode2018.lesson001.demo20.ConfigBean1@402bba4f
bean名称:user1,别名:[],bean对象:com.javacode2018.lesson001.demo20.User@795cd85e
bean名称:user2Bean,别名:[],bean对象:com.javacode2018.lesson001.demo20.User@59fd97a8
bean名称:user3Bean,别名:[user3BeanAlias2, user3BeanAlias1],bean对象:com.javacode2018.lesson001.demo20.User@f5ac9e4
```
<a name="trpzH"></a>
#### 分析结果
将2个输出的最后4行拿来对比一下：
<a name="cawxc"></a>
##### 有`@Configuration`注解的
```
bean名称:configBean,别名:[],bean对象:com.javacode2018.lesson001.demo20.ConfigBean$$EnhancerBySpringCGLIB$$dde45976@5e5d171f
bean名称:user1,别名:[],bean对象:com.javacode2018.lesson001.demo20.User@24313fcc
bean名称:user2Bean,别名:[],bean对象:com.javacode2018.lesson001.demo20.User@7d20d0b
bean名称:user3Bean,别名:[user3BeanAlias2, user3BeanAlias1],bean对象:com.javacode2018.lesson001.demo20.User@77f1baf5
```
<a name="YO5np"></a>
##### 没有`@Configuration`注解的
```
bean名称:configBean1,别名:[],bean对象:com.javacode2018.lesson001.demo20.ConfigBean1@402bba4f
bean名称:user1,别名:[],bean对象:com.javacode2018.lesson001.demo20.User@795cd85e
bean名称:user2Bean,别名:[],bean对象:com.javacode2018.lesson001.demo20.User@59fd97a8
bean名称:user3Bean,别名:[user3BeanAlias2, user3BeanAlias1],bean对象:com.javacode2018.lesson001.demo20.User@f5ac9e4
```
<a name="iQOU8"></a>
##### 对比得出

1. 对比最后3行，可以看出：**有没有**`**@Configuration**`**注解，**`**@Bean**`**都会起效，都会将**`**@Bean**`**修饰的方法作为bean注册到容器中**
2. 两个内容的第一行有点不一样，被`@Configuration`修饰的bean最后输出的时候带有`EnhancerBySpringCGLIB`的字样，而没有`@Configuration`注解的bean没有Cglib的字样；有`EnhancerBySpringCGLIB`字样的说明这个bean被cglib处理过的，变成了一个代理对象。

**目前为止还是看不出二者本质上的区别，继续向下看。**
<a name="K8J2o"></a>
## `@Configuration`加不加到底区别在哪？
通常情况下，bean之间是有依赖关系的，来创建个有依赖关系的bean，通过这个案例就可以看出根本的区别了。
<a name="LtmrS"></a>
### 再来一个加`@Configuration`的案例
定义2个类。
<a name="PieiP"></a>
#### ServiceA
```java
package com.javacode2018.lesson001.demo20;

public class ServiceA {
}
```
<a name="wr1bk"></a>
#### ServiceB
```java
package com.javacode2018.lesson001.demo20;

public class ServiceB {
    private ServiceA serviceA;

    public ServiceB(ServiceA serviceA) {
        this.serviceA = serviceA;
    }

    @Override
    public String toString() {
        return "ServiceB{" +
        "serviceA=" + serviceA +
        '}';
    }
}
```
上面定义了2个类，ServiceB依赖于ServiceA，ServiceB通过构造器注入ServiceA。<br />来个`@Configuration`类管理上面对象。
<a name="AEfis"></a>
#### ConfigBean2
```java
package com.javacode2018.lesson001.demo20;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class ConfigBean2 {

    @Bean
    public ServiceA serviceA() {
        System.out.println("调用serviceA()方法"); //@0
        return new ServiceA();
    }

    @Bean
    ServiceB serviceB1() {
        System.out.println("调用serviceB1()方法");
        ServiceA serviceA = this.serviceA(); //@1
        return new ServiceB(serviceA);
    }

    @Bean
    ServiceB serviceB2() {
        System.out.println("调用serviceB2()方法");
        ServiceA serviceA = this.serviceA(); //@2
        return new ServiceB(serviceA);
    }

}
```
上面通过`@Bean`注解，向容器中注册了3个bean<br />注意@1和@2，通过`this.serviceA()`获取需要注入的ServiceA对象。<br />上面每个方法第一行都输出了一行日志。<br />**重点关注一下@0这行日志会输出几次，大家先思考一下1次还是3次？**
<a name="KvYOa"></a>
#### 测试用例
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ConfigBean2.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        //别名
        String[] aliases = context.getAliases(beanName);
        System.out.println(String.format("bean名称:%s,别名:%s,bean对象:%s",
                                         beanName,
                                         Arrays.asList(aliases),
                                         context.getBean(beanName)));
    }
}
```
<a name="hTu8w"></a>
#### 运行输出
截取了几行输出如下：
```
调用serviceA()方法
调用serviceB1()方法
调用serviceB2()方法
bean名称:configBean2,别名:[],bean对象:com.javacode2018.lesson001.demo20.ConfigBean2$$EnhancerBySpringCGLIB$$ffa0178@77f1baf5
bean名称:serviceA,别名:[],bean对象:com.javacode2018.lesson001.demo20.ServiceA@41a2befb
bean名称:serviceB1,别名:[],bean对象:ServiceB{serviceA=com.javacode2018.lesson001.demo20.ServiceA@41a2befb}
bean名称:serviceB2,别名:[],bean对象:ServiceB{serviceA=com.javacode2018.lesson001.demo20.ServiceA@41a2befb}
```
<a name="GHCGG"></a>
#### 分析结果
从输出中可以看出

1. **前三行可以看出，被**`**@Bean**`**修饰的方法都只被调用了一次，这个很关键**
2. **最后三行中可以看出都是同一个ServiceA对象，都是`ServiceA@41a2befb`这个实例**
<a name="cQJHn"></a>
#### 这是为什么？
**被**`**@Configuration**`**修饰的类，Spring容器中会通过cglib给这个类创建一个代理，代理会拦截所有被**`**@Bean**`**修饰的方法，默认情况（bean为单例）下确保这些方法只被调用一次，从而确保这些bean是同一个bean，即单例的。**
<a name="Du8Df"></a>
### 不加的案例
再来看看将ConfigBean2上的的`@Configuration`去掉，效果如何，代码就不写了，直接上输出结果：
```
调用serviceA()方法
调用serviceB1()方法
调用serviceA()方法
调用serviceB2()方法
调用serviceA()方法
bean名称:configBean2,别名:[],bean对象:com.javacode2018.lesson001.demo20.ConfigBean2@6e171cd7
bean名称:serviceA,别名:[],bean对象:com.javacode2018.lesson001.demo20.ServiceA@402bba4f
bean名称:serviceB1,别名:[],bean对象:ServiceB{serviceA=com.javacode2018.lesson001.demo20.ServiceA@795cd85e}
bean名称:serviceB2,别名:[],bean对象:ServiceB{serviceA=com.javacode2018.lesson001.demo20.ServiceA@59fd97a8}
```
<a name="q4KtZ"></a>
#### 结果分析

1. `**serviceA()**`**方法被调用了3次**
2. **configBean2这个bean没有代理效果了**
3. **最后3行可以看出，几个ServiceA对象都是不一样的**
<a name="TAjIM"></a>
## Spring这块的源码
Spring中用下面这个类处理`@Configuration`这个注解：
```
org.springframework.context.annotation.ConfigurationClassPostProcessor
```
这里面重点关注这几个方法：
```java
public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) 
public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory)
public void enhanceConfigurationClasses(ConfigurableListableBeanFactory beanFactory)
```
最后一个方法会创建cglib代理，大家可以设置断点进去看看，有问题欢迎交流。
<a name="RubA6"></a>
## 总结

1. `**@Configuration**`**注解修饰的类，会被Spring通过cglib做增强处理，通过cglib会生成一个代理对象，代理会拦截所有被**`**@Bean**`**注解修饰的方法，可以确保一些bean是单例的**
2. **不管**`**@Bean**`**所在的类上是否有**`**@Configuration**`**注解，都可以将**`**@Bean**`**修饰的方法作为一个bean注册到Spring容器中**
