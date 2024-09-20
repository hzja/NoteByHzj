Java
<a name="RobeH"></a>
## 区别
<a name="v09cZ"></a>
### 「`BeanFactory`和`FactoryBean`有哪些区别？」

1. `BeanFactory`是一个最基础的IOC容器，提供了依赖查找，依赖注入等基础的功能
2. `FactoryBean`是创建Bean的一种方式，帮助实现复杂Bean的创建

和`BeanFactory`相关的还有一个高频的面试题
<a name="cttUv"></a>
### 「ApplicationContext和BeanFactory有哪些区别？」

1. `BeanFactory`是一个最基础的IOC容器，提供了依赖查找，依赖注入等基础的功能
2. `ApplicationContext`继承了`BeanFactory`，在`BeanFactory`的基础上增加了企业级的功能，如AOP，资源管理（Resources）事件（Event），国际化（i18n），`Environment`抽象等
<a name="fEynJ"></a>
## 创建Bean的方式
常见的创建Bean的方式有如下四种

1. 通过构造器
2. 通过静态工厂方法
3. 通过Bean工厂方法
4. 通过`FactoryBean`
```java
@Data
@ToString
public class User {

    private Long id;
    private String name;

    public static User createUser() {
        User user = new User();
        user.setId(1L);
        user.setName("li");
        return user;
    }
}

public class UserFactory {

    public User createUser() {
        return User.createUser();
    }
}

public class UserFactoryBean implements FactoryBean {

    @Override
    public Object getObject() throws Exception {
        return User.createUser();
    }

    @Override
    public Class<?> getObjectType() {
        return User.class;
    }
}
```
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
                           https://www.springframework.org/schema/beans/spring-beans.xsd">

  <!-- 构造方法实例化 Bean -->
  <bean id="user-by-constructor" class="com.javashitang.domain.User">
    <property name="id" value="1"/>
    <property name="name" value="li"/>
  </bean>

  <!-- 静态方法实例化 Bean -->
  <bean id="user-by-static-method" class="com.javashitang.domain.User"
        factory-method="createUser"/>

  <bean id="userFactory" class="com.javashitang.factory.UserFactory"/>

  <!-- Bean工厂方法实例化 Bean -->
  <bean id="user-by-factory" factory-bean="userFactory" factory-method="createUser"/>

  <!-- FactoryBean实例化 Bean -->
  <bean id="user-by-factory-bean" class="com.javashitang.factory.UserFactoryBean"/>
</beans>
```
```java
public class BeanInstantiationDemo {

    public static void main(String[] args) {
        BeanFactory beanFactory = new ClassPathXmlApplicationContext("classpath:/bean-instantiation-context.xml");
        User user1 = beanFactory.getBean("user-by-constructor", User.class);
        User user2 = beanFactory.getBean("user-by-static-method", User.class);
        User user3 = beanFactory.getBean("user-by-factory", User.class);
        User user4 = beanFactory.getBean("user-by-factory-bean", User.class);
    }
}
```
<a name="t6CWn"></a>
## 实现原理
在分析源码之前，先明确2个概念<br />「`factoryBean`是配置到容器中的实现`FactoryBean`接口的Bean，而`subBean`是用`FactoryBean`创建出来的Bean」<br />在Spring容器启动的过程中，会实例化非延迟的单例Bean，即调用如下方法 `DefaultListableBeanFactory#preInstantiateSingletons`<br />![2021-05-13-13-28-33-170878.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620883983939-1e776d67-81ec-4a30-b97e-c2ed256421e2.png#clientId=u91887ba7-dec7-4&from=ui&id=u0701252b&originHeight=750&originWidth=1080&originalType=binary&size=195273&status=done&style=none&taskId=ufeeb05b1-aa38-4090-a6f3-b8cb7a466dd)<br />调用`FactoryBean#getObject`的链路如下图<br />![2021-05-13-13-28-33-297539.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620883991433-61d7b654-f326-4eaa-b83a-d2083b034f00.png#clientId=u91887ba7-dec7-4&from=ui&id=ufc6f4133&originHeight=436&originWidth=958&originalType=binary&size=108766&status=done&style=none&taskId=u88d7d3ba-16b9-4b22-816b-34ffac7540b)<br />通过分析`DefaultListableBeanFactory#preInstantiateSingletons`方法和`FactoryBean#getObject`的调用链路可以分析得到

1. 单例的`factoryBean`对象本身会在Spring容器启动时主动初始化。而`subBean`的初始化则是在第一次从缓存中获取`factoryBean`并且不为空才会触发
2. 如果`factoryBean`对象实现的接口是`SmartFactoryBean`且`isEagerInit`方法返回`true`，那么`subBean`对象也会在Spring容器启动的时候主动初始化
3. 如果`bean`注册的时候，`beanName`对应的`bean`实例是一个`factoryBean`，那么通过`getBean(beanName)`获取到的对象将会是`subBean`对象；如果要获取工厂对象`factoryBean`，需要使用`getBean("&" + beanName)`
4. 单例的`subBean`也会缓存在Spring容器中，具体的容器是`FactoryBeanRegistrySupport#factoryBeanObjectCache`，一个`Map<beanName, subBean实例>`

「建议大家看一下`DefaultListableBeanFactory#preInstantiateSingletons`方法和`FactoryBean#getObject`方法的调用链路，就能理解上面的流程了」
<a name="xXCoO"></a>
## 应用
目前在Dubbbo源码中看到了`FactoryBean`的应用<br />「服务导出：在Dubbo中，服务提供者会被包装成`ServiceBean`对象，当监听到`ContextRefreshedEvent`事件时开始服务导出」<br />「服务调用：服务调用方会被包装成`ReferenceBean`对象，`ReferenceBean`实现了`FactoryBean`接口和`InitializingBean`接口，创建`subBean`的逻辑在`ReferenceBean#getObject`方法中」<br />「Dubbo服务引入的时机有如下2种。」

1. 饿汉式：`init=true`，在`Bean`生命周期的初始化阶段会调用`InitializingBean#afterPropertiesSet`方法，而这个方法会调用`ReferenceBean#getObject`方法，完成`subBean`的创建，即`ReferenceBean`实例化时完成服务引入
2. 懒汉式：`init=false`，在`ReferenceBean`对应的服务被注入到其他类中时，此时会调用`AbstractApplicationContext#getBean`，获取`ReferenceBean`对象，因为`ReferenceBean`实现了`FactoryBean`接口，所以会调用`ReferenceBean#getObject`方法，完成`subBean`的创建，即完成服务引入
```java
public class ReferenceBean<T> extends ReferenceConfig<T> implements FactoryBean, ApplicationContextAware, InitializingBean, DisposableBean {

    @Override
    public Object getObject() {
        return get();
    }

    @Override
    @SuppressWarnings({"unchecked"})
    public void afterPropertiesSet() throws Exception {

        // 省略部分代码

        if (shouldInit()) {
            getObject();
        }
    }
    
}
```
