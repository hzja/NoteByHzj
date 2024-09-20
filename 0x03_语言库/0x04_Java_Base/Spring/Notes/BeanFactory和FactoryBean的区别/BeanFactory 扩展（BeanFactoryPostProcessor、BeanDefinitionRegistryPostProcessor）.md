JavaSpring<br />**Spring中有2个非常重要的接口：**`**BeanFactoryPostProcessor**`**和**`**BeanDefinitionRegistryPostProcessor**`**，这2个接口面试中也会经常问到。**
<a name="HtbmS"></a>
## 先来看几个问题

1. `**BeanFactoryPostProcessor**`**是做什么的？**
2. `**BeanDefinitionRegistryPostProcessor**`**是干什么的？**
3. `**BeanFactoryPostProcessor**`**和**`**BeanDefinitionRegistryPostProcessor**`**有什么区别？**
4. **这几个接口的执行顺序是什么样的？**
<a name="piMln"></a>
## Spring容器中主要的4个阶段

- 阶段1：Bean注册阶段，此阶段会完成所有bean的注册
- 阶段2：`BeanFactory`后置处理阶段
- 阶段3：注册`BeanPostProcessor`
- 阶段4：bean创建阶段，此阶段完成所有单例bean的注册和装载操作。

本文介绍的2个接口主要和前2个阶段有关系，下面主要来看前2个阶段。
<a name="EKGys"></a>
## 阶段1：Bean注册阶段
<a name="lNW27"></a>
### 概述
Spring中所有bean的注册都会在此阶段完成，按照规范，所有bean的注册必须在此阶段进行，其他阶段不要再进行bean的注册。<br />这个阶段Spring提供1个接口：`BeanDefinitionRegistryPostProcessor`，Spring容器在这个阶段中会获取容器中所有类型为`BeanDefinitionRegistryPostProcessor`的bean，然后会调用他们的`postProcessBeanDefinitionRegistry`方法，源码如下，方法参数类型是`BeanDefinitionRegistry`，这个类型大家都比较熟悉，即bean定义注册器，内部提供了一些方法可以用来向容器中注册bean。
```java
public interface BeanDefinitionRegistryPostProcessor extends BeanFactoryPostProcessor {
    void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException;
}
```
这个接口还继承了`BeanFactoryPostProcessor`接口，这个大家先不用关心，一会阶段2中会介绍。<br />当容器中有多个`BeanDefinitionRegistryPostProcessor`的时候，可以通过下面任意一种方式来指定顺序

1. 实现`org.springframework.core.PriorityOrdered`接口
2. 实现`org.springframework.core.Ordered`接口

执行顺序：
```
PriorityOrdered.getOrder() asc,Ordered.getOrder() asc
```
下面通过案例来感受一下效果。
<a name="A9uoZ"></a>
### 案例1：简单实用
此案例演示`BeanDefinitionRegistryPostProcessor`的简单使用
<a name="JvGIr"></a>
#### 自定义一个`BeanDefinitionRegistryPostProcessor`
下面定义了一个类，需要实现`BeanDefinitionRegistryPostProcessor`接口，然后实现2个方法，大家重点关注`postProcessBeanDefinitionRegistry`这个方法，另外一个方法来自于`BeanFactoryPostProcessor`，一会后面在介绍这个方法，在`postProcessBeanDefinitionRegistry`方法中，定义了一个bean，然后通过registry将其注册到容器了，代码很简单
```java
package com.javacode2018.lesson003.demo3.test0;

@Component
public class MyBeanDefinitionRegistryPostProcessor implements BeanDefinitionRegistryPostProcessor {
    @Override
    public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
        //定义一个字符串类型的bean
        AbstractBeanDefinition userNameBdf = BeanDefinitionBuilder.
                genericBeanDefinition(String.class).
                addConstructorArgValue("路人").
                getBeanDefinition();
        //将userNameBdf注册到spring容器中
        registry.registerBeanDefinition("userName", userNameBdf);
    }

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {

    }
}
```
<a name="ViQHp"></a>
#### 同包中来个配置类
```java
package com.javacode2018.lesson003.demo3.test0;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig0 {
}
```
<a name="RnOKF"></a>
#### 测试用例
```java
package com.javacode2018.lesson003.demo3;

import com.javacode2018.lesson003.demo3.test0.MainConfig0;
import com.javacode2018.lesson003.demo3.test1.MainConfig1;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class BeanDefinitionRegistryPostProcessorTest {
    @Test
    public void test0() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig0.class);
        context.refresh();
        System.out.println(context.getBean("userName"));
    }
}
```
<a name="ozqJ0"></a>
#### 运行输出
```
路人
```
<a name="j1VJu"></a>
### 案例2：多个指定顺序
下面定义2个`BeanDefinitionRegistryPostProcessor`，都实现`Ordered`接口，第一个order的值为2，第二个order的值为1，来看一下具体执行的顺序。
<a name="CAf2r"></a>
#### 第一个
```java
package com.javacode2018.lesson003.demo3.test1;

@Component
public class BeanDefinitionRegistryPostProcessor1 implements BeanDefinitionRegistryPostProcessor, Ordered {
    @Override
    public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
        System.out.println(String.format("BeanDefinitionRegistryPostProcessor1{order=%d},注册name bean,", this.getOrder()));
        //定义一个bean
        AbstractBeanDefinition nameBdf = BeanDefinitionBuilder.
        genericBeanDefinition(String.class).
        addConstructorArgValue("java").
        getBeanDefinition();
        //将定义的bean注册到容器
        registry.registerBeanDefinition("name", nameBdf);
    }

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {

    }

    @Override
    public int getOrder() {
        return 2;
    }
}
```
<a name="x37Ml"></a>
#### 第二个
```java
package com.javacode2018.lesson003.demo3.test1;

@Component
public class BeanDefinitionRegistryPostProcessor2 implements BeanDefinitionRegistryPostProcessor, Ordered {
    @Override
    public void postProcessBeanDefinitionRegistry(BeanDefinitionRegistry registry) throws BeansException {
        System.out.println(String.format("BeanDefinitionRegistryPostProcessor2{order=%d},注册car bean,", this.getOrder()));
        //定义一个bean
        AbstractBeanDefinition nameBdf = BeanDefinitionBuilder.
        genericBeanDefinition(String.class).
        addConstructorArgValue("保时捷").
        getBeanDefinition();
        //将定义的bean注册到容器
        registry.registerBeanDefinition("car", nameBdf);
    }

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {

    }

    @Override
    public int getOrder() {
        return 1;
    }
}
```
上面2个类中的`postProcessBeanDefinitionRegistry`方法第一行都有输出，一个可以通过运行结果看到执行的顺序。
<a name="y3bAJ"></a>
#### 同包中添加配置类
```java
package com.javacode2018.lesson003.demo3.test1;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig1 {
}
```
<a name="AmQLn"></a>
#### 测试案例
```java
@Test
public void test1() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig1.class);
    context.refresh();
    context.getBeansOfType(String.class).forEach((beanName, bean) -> {
        System.out.println(String.format("%s->%s", beanName, bean));
    });
}
```
<a name="XRaTa"></a>
#### 运行输出
```
BeanDefinitionRegistryPostProcessor2{order=1},注册car bean,
BeanDefinitionRegistryPostProcessor1{order=2},注册name bean,
car->保时捷
name->java
```
<a name="Nk7TW"></a>
### 小结
`BeanDefinitionRegistryPostProcessor`有个非常重要的实现类：
```
org.springframework.context.annotation.ConfigurationClassPostProcessor
```
这个类可能有些人不熟悉，下面这些注解大家应该比较熟悉吧，这些注解都是在上面这个类中实现的，通过这些注解来实现bean的批量注册
```java
@Configuration
@ComponentScan
@Import
@ImportResource
@PropertySource
```
有兴趣的朋友可以去看一下`ConfigurationClassPostProcessor#postProcessBeanDefinitionRegistry`研究一下上面这些注解的解析过程，可以学到很多东西。
<a name="PQ3xT"></a>
## 阶段2：`BeanFactory`后置处理阶段
<a name="FkGnS"></a>
### 概述
到这个阶段的时候，Spring容器已经完成了所有bean的注册，这个阶段中可以对`BeanFactory`中的一些信息进行修改，比如修改阶段1中一些bean的定义信息，修改BeanFactory的一些配置等等，此阶段Spring也提供了一个接口来进行扩展：`BeanFactoryPostProcessor`，简称bfpp，接口中有个方法`postProcessBeanFactory`，Spring会获取容器中所有`BeanFactoryPostProcessor`类型的bean，然后调用他们的`postProcessBeanFactory`，来看一下这个接口的源码：
```java
@FunctionalInterface
public interface BeanFactoryPostProcessor {
    void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException;
}
```
当容器中有多个`BeanFactoryPostProcessor`的时候，可以通过下面任意一种方式来指定顺序

1. 实现`org.springframework.core.PriorityOrdered`接口
2. 实现`org.springframework.core.Ordered`接口

执行顺序：
```
PriorityOrdered.getOrder() asc,Ordered.getOrder() asc
```
下面通过案例来感受一下效果。
<a name="dKgre"></a>
### 案例
这个案例中演示，在`BeanFactoryPostProcessor`来修改bean中已经注册的bean定义的信息，给一个bean属性设置一个值。
<a name="kVYRb"></a>
#### 先来定义一个bean
```java
package com.javacode2018.lesson003.demo3.test2;

import org.springframework.stereotype.Component;

@Component
public class LessonModel {
    //课程名称
    private String name;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "LessonModel{" +
        "name='" + name + '\'' +
        '}';
    }
}
```
上面这个bean有个name字段，并没有设置值，下面在`BeanFactoryPostProcessor`来对其设置值。
<a name="kqZ9I"></a>
#### 自定义的`BeanFactoryPostProcessor`
下面代码中，先获取lessonModel这个bean的定义信息，然后给其name属性设置了一个值。
```java
package com.javacode2018.lesson003.demo3.test2;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.stereotype.Component;

@Component
public class MyBeanFactoryPostProcessor implements BeanFactoryPostProcessor {

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        System.out.println("准备修改lessonModel bean定义信息!");
        BeanDefinition beanDefinition = beanFactory.getBeanDefinition("lessonModel");
        beanDefinition.getPropertyValues().add("name", "spring!");
    }

}
```
<a name="zJRmO"></a>
#### 测试用例
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig2.class);
    context.refresh();

    System.out.println(context.getBean(LessonModel.class));
}
```
<a name="LAr3M"></a>
#### 运行输出
```
准备修改lessonModel bean定义信息!
LessonModel{name='spring!'}
```
结果中可以看出，通过`BeanFactoryPostProcessor`修改了容器中已经注册的bean定义信息。
<a name="tJvlR"></a>
### 这个接口的几个重要实现类
<a name="YZuXa"></a>
#### `PropertySourcesPlaceholderConfigurer`
这个接口做什么的，大家知道么？来看一段代码
```xml
<bean class="xxxxx">
  <property name="userName" value="${userName}"/>
  <property name="address" value="${address}"/>
</bean>
```
这个大家比较熟悉吧，Spring就是在`PropertySourcesPlaceholderConfigurer#postProcessBeanFactory`中来处理xml中属性中的${xxx}，会对这种格式的进行解析处理为真正的值。
<a name="cIma8"></a>
#### `CustomScopeConfigurer`
向容器中注册自定义的Scope对象，即注册自定义的作用域实现类。<br />这个用法比较简单，定义一个`CustomScopeConfigurer`的bean就可以了，然后通过这个类来注册自定义的bean。
<a name="JSuWR"></a>
#### `EventListenerMethodProcessor`
处理`@EventListener`注解的，即Spring中事件机制，还有一些实现类，这里就不介绍了。
<a name="U6IMm"></a>
### 使用注意
`BeanFactoryPostProcessor`接口的使用有一个需要注意的地方，在其`postProcessBeanFactory`方法中，强烈禁止去通过容器获取其他bean，此时会导致bean的提前初始化，会出现一些意想不到的问题，因为这个阶段中`BeanPostProcessor`还未准备好，本文开头4个阶段中有介绍，`BeanPostProcessor`是在第3个阶段中注册到Spring容器的，而`BeanPostProcessor`可以对bean的创建过程进行干预，比如Spring中的aop就是在`BeanPostProcessor`的一些子类中实现的，`@Autowired`也是在`BeanPostProcessor`的子类中处理的，此时如果去获取bean，此时bean不会被`BeanPostProcessor`处理，所以创建的bean可能是有问题的，还是通过一个案例给大家演示一下把，通透一些。
<a name="ycGOx"></a>
#### 来一个简单的类
```java
package com.javacode2018.lesson003.demo3.test3;

import org.springframework.beans.factory.annotation.Autowired;

public class UserModel {
    @Autowired
    private String name; //@1

    @Override
    public String toString() {
        return "UserModel{" +
        "name='" + name + '\'' +
        '}';
    }
}
```
@1：使用了`@Autowired`，会指定注入
<a name="eG6gK"></a>
### 来个配置类
配置类中定义了2个UserModel类型的bean：user1、user2<br />并且定义了一个String类型的bean：name，这个会注入到UserModel中的name属性中去。
```java
package com.javacode2018.lesson003.demo3.test3;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan
public class MainConfig3 {
    @Bean
    public UserModel user1() {
        return new UserModel();
    }

    @Bean
    public UserModel user2() {
        return new UserModel();
    }

    @Bean
    public String name() {
        return "Java";
    }
}
```
<a name="DcR3Y"></a>
#### 测试用例
输出容器中所有UserModel类型的bean
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig3.class);
    context.refresh();

    context.getBeansOfType(UserModel.class).forEach((beanName, bean) -> {
        System.out.println(String.format("%s->%s", beanName, bean));
    });
}
```
<a name="W4wbm"></a>
#### 运行输出
```
user1->UserModel{name='Java'}
user2->UserModel{name='Java'}
```
效果不用多解释，大家一看就懂，下面来重点。
<a name="sBe87"></a>
### 添加一个`BeanFactoryPostProcessor`
在`postProcessBeanFactory`方法中去获取一下user1这个bean
```java
package com.javacode2018.lesson003.demo3.test3;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.stereotype.Component;

@Component
public class MyBeanFactoryPostProcessor implements BeanFactoryPostProcessor {

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        beanFactory.getBean("user1");
    }

}
```
<a name="nZ6IZ"></a>
#### 再次运行输出
```
user1->UserModel{name='null'}
user2->UserModel{name='Java'}
```
注意，user1中的name变成null了，什么情况？<br />是因为`@Autowired`注解是在`AutowiredAnnotationBeanPostProcessor`中解析的，Spring容器调用`BeanFactoryPostProcessor#postProcessBeanFactory`的使用，此时Spring容器中还没有`AutowiredAnnotationBeanPostProcessor`，所以此时去获取user1这个bean的时候，`@Autowired`并不会被处理，所以name是null。
<a name="Ck6aX"></a>
## 源码
<a name="ZV7Mc"></a>
### 4个阶段的源码
4个阶段的源码为位于下面这个方法中
```
org.springframework.context.support.AbstractApplicationContext#refresh
```
这个方法中截取部分代码如下：
```java
// 对应阶段1和阶段2：调用上下文中注册为bean的工厂处理器，即调用本文介绍的2个接口中的方法
invokeBeanFactoryPostProcessors(beanFactory);

// 对应阶段3：注册拦截bean创建的bean处理器，即注册BeanPostProcessor
registerBeanPostProcessors(beanFactory);

// 对应阶段3：实例化所有剩余的（非延迟初始化）单例。
finishBeanFactoryInitialization(beanFactory);
```
阶段1和阶段2的源码位于下面方法中，代码比较简单，强烈建议大家去看一下，几分钟就可以看懂了。
```
org.springframework.context.support.PostProcessorRegistrationDelegate#invokeBeanFactoryPostProcessors(org.springframework.beans.factory.config.ConfigurableListableBeanFactory, java.util.List<org.springframework.beans.factory.config.BeanFactoryPostProcessor>)
```
<a name="v0k38"></a>
## 总结

1. **注意Spring的4个阶段：bean定义阶段、**`**BeanFactory**`**后置处理阶段、**`**BeanPostProcessor**`**注册阶段、单例bean创建组装阶段**
2. `**BeanDefinitionRegistryPostProcessor**`**会在第一个阶段被调用，用来实现bean的注册操作，这个阶段会完成所有bean的注册**
3. `**BeanFactoryPostProcessor**`**会在第2个阶段被调用，到这个阶段时候，bean此时已经完成了所有bean的注册操作，这个阶段中可以对**`**BeanFactory**`**中的一些信息进行修改，比如修改阶段1中一些bean的定义信息，修改**`**BeanFactory**`**的一些配置等等**
4. **阶段2的时候，2个禁止操作：禁止注册bean、禁止从容器中获取bean**
5. **本文介绍的2个接口的实现类可以通过**`**PriorityOrdered**`**接口或者**`**Ordered**`**接口来指定顺序**
