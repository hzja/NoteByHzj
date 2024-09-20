JavaSpring<br />主要分享一些在工作中可能会用到的Spring依赖注入，依赖查找方面的技术点整理，非常实用。
<a name="q3GXJ"></a>
## **Spring依赖查找专题**
<a name="ziGPn"></a>
### 单一类型查找
常见用法如下所示：
```java
Object getBean(String name) throws BeansException;

<T> T getBean(Class<T> requiredType) throws BeansException;

<T> T getBean(String name, Class<T> requiredType) throws BeansException
```
<a name="rYRMM"></a>
### `ObjectProvider`
随着Spring版本的升高，也开始出现了延迟查找的功能。当实际需要用到某个bean的时候才将其从容器中进行初始化并且提取出来。
```java
<T> ObjectProvider<T> getBeanProvider(Class<T> requiredType);

<T> ObjectProvider<T> getBeanProvider(ResolvableType requiredType);
```
Spring会返回一个`ObjectProvider`，当查询的时候才会触发bean的创建。<br />延迟查找的好处在于，如果一个bean需要注入到spring容器中，但是不希望太过早地去进行初始化，那么可以思考使用`ObjectProvider`的方式来进行初始化。
<a name="gwqPY"></a>
### 集合类型查找
Bean的名称查询
```java
String[] getBeanNamesForType(@Nullable Class<?> type); 

String[] getBeanNamesForType(@Nullable Class<?> type, boolean includeNonSingletons, boolean allowEagerInit);
```
获取同类型Bean实例列表
```java
getBeansOfType(Class)
```
<a name="ewiPF"></a>
### 按照注解去查询
```java
String[] getBeanNamesForAnnotation(Class<? extends Annotation> annotationType);

Map<String, Object> getBeansWithAnnotation(Class<? extends Annotation> annotationType) throws BeansException;

<A extends Annotation> A findAnnotationOnBean(String beanName, Class<A> annotationType) throws NoSuchBeanDefinitionException;
```
使用依赖查找时候的一些小心得：<br />对于判断一个bean是否存在，可以采用判断其`beandefinition`是否存在，一般这样不会触发其中bean的初始化操作，例如：`getBeanNamesForType`。反观`getBeansOfType`可能回触发bean的初始化
<a name="qdycg"></a>
### 层次性的bean查找
可能大多数人在实际使用Spring容器的时候对于层次性的bean做计算并没有太多的实战尝试，这里举个例子：<br />例如说A容器中包含了Bean A，如果B容器继承了A容器，那么按道理来说也应该能够获得Bean A资源，这种设计可以减少Bean的额外存储。<br />如果理解了上边所说的这个案例之后，再来看看下边的这张图可能就会有更加深入的理解了。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1671949376699-0f0dd970-b339-46c7-b449-07582534e389.jpeg)<br />关于层次性的bean获取，这里给出一个小的demo供大家学习：
```java
package org.idea.spring.look.up.factory;

import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.HierarchicalBeanFactory;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

/**
 * 层次性的依赖查找 {@link org.springframework.beans.factory.HierarchicalBeanFactory}
 */
public class SpringHierarchicalLookUpDemo {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        applicationContext.register(SpringHierarchicalLookUpDemo.class);
        applicationContext.refresh();

        ParentIocContainer parentIocContainer = new ParentIocContainer();
        ApplicationContext parentApplicationContext = parentIocContainer.getAndStartApplicationContext();
        // ConfigurableListableBeanFactory -> ConfigurableBeanFactory -> HierarchicalBeanFactory
        ConfigurableListableBeanFactory configurableListableBeanFactory = applicationContext.getBeanFactory();
        System.out.println("此时的父类BeanFactory为：" + configurableListableBeanFactory.getParentBeanFactory());
        configurableListableBeanFactory.setParentBeanFactory(parentApplicationContext);
        System.out.println("此时的父类BeanFactory为：" + configurableListableBeanFactory.getParentBeanFactory());
        ParentIocContainer.ParentBean parentBean = (ParentIocContainer.ParentBean) configurableListableBeanFactory.getBean("parentBean");
        System.out.println(parentBean);
        isContainedBean(configurableListableBeanFactory, "parentBean");
        displayContainsBean(configurableListableBeanFactory, "parentBean");
    }


    /**
     * 这里是子类可以获取自己和父类层次内部的bean，如果是使用containsLocalBean方法的话就只能判断当前所在层次的容器上下文
     *
     * @param beanFactory
     * @param beanName
     */
    public static void isContainedBean(HierarchicalBeanFactory beanFactory, String beanName) {
        System.out.println("getBean is " + beanFactory.getBean(beanName));
        System.out.printf("contained is [%s] ,beanFactory is [%s],beanName is [%s]\n", beanFactory.containsLocalBean(beanName), beanFactory, beanName);
    }

    /**
     * 查找关于父类容器内部的bean
     *
     * @param beanFactory
     * @param beanName
     */
    private static void displayContainsBean(HierarchicalBeanFactory beanFactory, String beanName) {
        System.out.printf("contained is [%s] ,beanFactory is [%s],beanName is [%s]\n", isContainedBeanInHoldApplication(beanFactory, beanName), beanFactory, beanName);
    }

    /**
     * 使用递归判断 -- 自上到下判断父类容器是否含有bean
     *
     * @param hierarchicalBeanFactory
     * @param beanName
     * @return
     */
    public static boolean isContainedBeanInHoldApplication(HierarchicalBeanFactory hierarchicalBeanFactory, String beanName) {
        BeanFactory parentBeanFactory = hierarchicalBeanFactory.getParentBeanFactory();
        if (parentBeanFactory instanceof HierarchicalBeanFactory) {
            HierarchicalBeanFactory parentHierarchicalBeanFactory = HierarchicalBeanFactory.class.cast(parentBeanFactory);
            if (isContainedBeanInHoldApplication(parentHierarchicalBeanFactory, beanName)) {
                return true;
            }
        }
        return hierarchicalBeanFactory.containsBean(beanName);
    }
}
```
对应的父类容器案例：
```java
package org.idea.spring.look.up.factory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

/**
 * 父类ioc容器 这里面的ioc容器只包含有ParentBean这个类
 */
public class ParentIocContainer {

    public static AnnotationConfigApplicationContext applicationContext = null;
    class ParentBean {
        int id;
        public ParentBean(){
            System.out.println("this is no arg init");
        }
        @Override
        public String toString() {
            return "ParentBean{" +
                    "id=" + id +
                    '}';
        }
    }
    public ApplicationContext getAndStartApplicationContext(){
       applicationContext = new AnnotationConfigApplicationContext();
       applicationContext.register(ParentIocContainer.class);
       //需要支持无参构造函数
       applicationContext.registerBean("parentBean",ParentBean.class);
       applicationContext.refresh();
       return applicationContext;
    }
    public static void main(String[] args) {
        ParentIocContainer parentIocContainer = new ParentIocContainer();
        ApplicationContext applicationContext = parentIocContainer.getAndStartApplicationContext();
        String[] str = applicationContext.getBeanNamesForType(ParentBean.class);
        for (String beanName : str) {
            System.out.println(beanName);
        }
    }

}
```
从这段代码中可以看出，`HierarchicalBeanFactory`是一种常见的层次类`BeanFactory`，并且当需要判断一个bean是否存在某个容器上下文中的时候，不妨可以试试使用`BeanFacoty`自带的这个方法：
```java
org.springframework.beans.factory.HierarchicalBeanFactory#containsLocalBean
/**
 * Return whether the local bean factory contains a bean of the given name,
 * ignoring beans defined in ancestor contexts.
 * <p>This is an alternative to {@code containsBean}, ignoring a bean
 * of the given name from an ancestor bean factory.
 * @param name the name of the bean to query
 * @return whether a bean with the given name is defined in the local factory
 * @see BeanFactory#containsBean
 */
boolean containsLocalBean(String name);
```
<a name="ToZSg"></a>
## **Spring依赖注入专题**
<a name="x9XOK"></a>
### 依赖注入的几种模式
手动注入模式：

- 写XML的方式注入
- 通过注解的方式注入
- 通过API的方式去注入
<a name="WTQ88"></a>
### 自动注入的模式：
使用`Autowiring`的模式进行注入<br />常见的注入案例：
<a name="jcdxA"></a>
#### xml方式注入
例如通过xml配置文件对bean的属性进行注入：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        https://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="malePerson" class="org.idea.spring.ioc.bean.Person" >
        <property name="id" value="1"></property>
        <property name="name" value="idea"></property>
    </bean>

    <bean class="org.idea.spring.dependency.inject.setter.PersonHolder">
        <property name="person" ref="malePerson"></property>
    </bean>
</beans>
```
对应的Person对象
```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class Person {

    Integer id;
    String name;
}
```
<a name="h5Imn"></a>
#### 注解的方式注入
例如加入一个`@Bean`的注解进行注入容器
```java
@Bean
public PersonHolder personHolder(Person person){
    return new PersonHolder(person);
}
```
<a name="hw57C"></a>
#### Spring容器内部的api注入
核心是利用了`BeanDefinitionBuilder`进行一个`beanDefinition`的构建，然后将这个`beanDefinition`给注入到Spring容器当中，上下文在启动之后会将之前准备好的`BeanDefinition`机械能初始化创建。
```java
import org.idea.spring.ioc.bean.Person;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Bean;

/**
 * 通过api的方式进行注入实现
 */
public class ApiDependencyInjectDemo {

    @Bean
    public Person myPerson(){
        return new Person(1,"idea");
    }

    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        applicationContext.register(ApiDependencyInjectDemo.class);
        BeanDefinition personBeanDefinition = createUserBeanDefinition();
        applicationContext.registerBeanDefinition("personHolder",personBeanDefinition);
        applicationContext.refresh();
        PersonHolder personHolder = applicationContext.getBean(PersonHolder.class);
        System.out.println(personHolder.getPerson());
        applicationContext.close();
    }

    private static BeanDefinition createUserBeanDefinition() {
        BeanDefinitionBuilder beanDefinitionBuilder = BeanDefinitionBuilder.genericBeanDefinition(PersonHolder.class);
        //注意这里的add方法有多种类型，需要注意下细节点
        beanDefinitionBuilder.addPropertyReference("person","myPerson");
        return beanDefinitionBuilder.getBeanDefinition();
    }
}
```
<a name="mR6xn"></a>
### `@Resource`,`@Autowire`,`@Qualifier`模式注入
这两类的注入在实际使用中频率比较高：

- `@Resource`注解的注入会根据后边的字段类型识别进行注入
- `@Autowire`则会根据bean的类型进行注入

这里有一段案例：
```java
public class AnnotationDependencyInjectDemo {
    @Autowired
    private PersonHolder personHolder2;
    @Resource
    private PersonHolder personHolder;
    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        applicationContext.register(AnnotationDependencyInjectDemo.class);
        applicationContext.refresh();
        AnnotationDependencyInjectDemo annotationDependencyInjectDemo = applicationContext.getBean(AnnotationDependencyInjectDemo.class);
        System.out.println(annotationDependencyInjectDemo.personHolder);
        System.out.println(annotationDependencyInjectDemo.personHolder2);
        //这里面的两个bean都是同一个，因为bean的作用域是一致相同的
        System.out.println(annotationDependencyInjectDemo.personHolder == annotationDependencyInjectDemo.personHolder2);
        applicationContext.close();
    }
    @Bean
    public PersonHolder personHolder(){
        return new PersonHolder(new Person(1,"idea"));
    }
}
```
如果出现了一个接口对应多个实现类，但是代码中使用的是`@Autowire`的方式进行依赖注入，此时可以通过新增`@Qualifier`注解的方式来实现依赖注入的效果。<br />单纯从实际使用来说，`@Resource` = `@Autowire` + `@Qualifier`
<a name="RM62z"></a>
## **依赖注入和依赖查找**
在常见的业务开发当中，可能会使用到比较多的依赖注入注解，但是在一些基础组件的开发中，个人感觉使用依赖查找往往更加具有灵活性。

| 名称类型 | 代码侵入性 | 具体示例 | 可读性 | 实现难度 |
| --- | --- | --- | --- | --- |
| 依赖注入 | 低 | Autowire Resource | 良好 | 低 |
| 依赖查找 | 高 | ApplicationContext | 一般 | 高 |

<a name="M6Tb1"></a>
## **Spring内部的Bean有哪几种**
<a name="aVIBB"></a>
### 自定义的bean
例如业务系统中常见的XXXXDao,XXXXService
<a name="ppGNi"></a>
### Spring容器中初始化构建好的Bean
例如Spring容器中的`Enviorment`对象
<a name="DKd23"></a>
### Spring容器内部的一些基础服务对象
例如Spring容器内部的`BeanFactory`对象，这类Bean通常是无法通过`getBean`接口去直接获取的。
<a name="BUC5k"></a>
### Spring中的`BeanDefinition`对象
上边有提及过到Spring内部提供了相关的Api供开发者进行灵活的依赖注入。但是当深入到具体细节进行分析之后，其实是可以发现`BeanDefinition`对象也是有不同类型差异的。<br />**用户自定义的**`**BeanDeinition**`**对象**例如这样一段代码：
```java
public class ApiDependencyInjectDemo {
    @Bean
    public Person myPerson(){
        return new Person(1,"idea");
    }
    public static void main(String[] args) {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        applicationContext.register(ApiDependencyInjectDemo.class);
        BeanDefinition personBeanDefinition = createUserBeanDefinition();
        applicationContext.registerBeanDefinition("personHolder",personBeanDefinition);
        applicationContext.refresh();
        PersonHolder personHolder = applicationContext.getBean(PersonHolder.class);
        System.out.println(personHolder.getPerson());
        applicationContext.close();
    }
    private static BeanDefinition createUserBeanDefinition() {
        //spring官方比较推荐的一种注入方式
        BeanDefinitionBuilder beanDefinitionBuilder = BeanDefinitionBuilder.genericBeanDefinition(PersonHolder.class);
        //注意这里的add方法有多种类型，需要注意下细节点
        beanDefinitionBuilder.addConstructorArgReference("myPerson");
        return beanDefinitionBuilder.getBeanDefinition();
    }
}
```
**框架内部初始化定义好的**`**BeanDefinition**`**对象**<br />具体体现在Spring容器进行初始化的时候，内部的`refresh`函数中有个`prepareBeanFactory`<br />点进去这个函数，对它的源代码进行深入解读之后会发现，其实内部已经注册了一系列的`BeanDefinition`对象<br />**在容器初始化之后是否还能注册Bean？**<br />其实是可以的，下边通过使用`BeanDefiniation`的案例来一起分析一下：
```java
package org.idea.spring.bean.source;

import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.idea.spring.bean.beandefinitionbuilder.User;

public class AddBeanAfterRefreshDemo {


    public static void main(String[] args) {
        AnnotationConfigApplicationContext annotationConfigApplicationContext = new AnnotationConfigApplicationContext();
        annotationConfigApplicationContext.register(AddBeanAfterRefreshDemo.class);
        annotationConfigApplicationContext.refresh();
        try {
            User user0 = annotationConfigApplicationContext.getBean(User.class);
            System.out.println("user0 is " + user0);
        } catch (Exception b) {
            b.printStackTrace();
        }
        System.out.println("启动后手动注入bean对象");

        BeanDefinitionBuilder beanDefinitionBuilder = BeanDefinitionBuilder.genericBeanDefinition(User.class);
        beanDefinitionBuilder.setScope("prototype");
        beanDefinitionBuilder
                .addPropertyValue("id", 2)
                .addPropertyValue("name", "idea");
        annotationConfigApplicationContext.registerBeanDefinition("user", beanDefinitionBuilder.getBeanDefinition());
        //1)
        User user1 = (User) annotationConfigApplicationContext.getBean("user");
        BeanDefinition beanDefinition = annotationConfigApplicationContext.getBeanDefinition("user");
        System.out.println(beanDefinition.getScope());
        System.out.println(user1);
        annotationConfigApplicationContext.close();
    }
}
```
注意，这里面如果将1)部分的代码调整为：
```java
User user1 = (User) annotationConfigApplicationContext.getBean(User.class);
```
则不会实现容器启动后注册了对应的bean，但是依然查询不到预期对象的效果。这是因为在Spring容器底层会有一个Map专门记录不同的`beanClass`类型对应不同的`beanName`集合，从而导致第二次查询的时候走了第一次查询时候的缓存。代码位于：
```java
org.springframework.beans.factory.support.DefaultListableBeanFactory
/** Map of singleton and non-singleton bean names, keyed by dependency type. */
  private final Map<Class<?>, String[]> allBeanNamesByType = new ConcurrentHashMap<>(64);
```
