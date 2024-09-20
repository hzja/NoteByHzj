JavaSpring
<a name="af1Ll"></a>
### 先来看一个案例
<a name="yXeZz"></a>
#### ServiceA.java
```java
package com.javacode2018.lesson001.demo12;

public class ServiceA {
}
```
<a name="KNfkc"></a>
#### ServiceB.java
```java
package com.javacode2018.lesson001.demo12;

public class ServiceB {
    private String name;
    private ServiceA serviceA;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public ServiceA getServiceA() {
        return serviceA;
    }

    public void setServiceA(ServiceA serviceA) {
        this.serviceA = serviceA;
    }

    @Override
    public String toString() {
        return "ServiceB{" +
        "name='" + name + '\'' +
        ", serviceA=" + serviceA +
        '}';
    }
}
```
上面类中有2个属性，下面再创建一个ServiceC类，和ServiceB中的内容一样。
<a name="S7DsV"></a>
#### ServiceC.java
```java
package com.javacode2018.lesson001.demo12;

public class ServiceC {
    private String name;
    private ServiceA serviceA;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public ServiceA getServiceA() {
        return serviceA;
    }

    public void setServiceA(ServiceA serviceA) {
        this.serviceA = serviceA;
    }

    @Override
    public String toString() {
        return "ServiceC{" +
        "name='" + name + '\'' +
        ", serviceA=" + serviceA +
        '}';
    }
}
```
下面使用Spring来创建上面3个类对应的bean。
<a name="U6X5Y"></a>
#### beanExtend.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceA" class="com.javacode2018.lesson001.demo12.ServiceA"/>

  <bean id="serviceB" class="com.javacode2018.lesson001.demo12.ServiceB">
    <property name="name" value="Java"/>
    <property name="serviceA" ref="serviceA"/>
  </bean>

  <bean id="serviceC" class="com.javacode2018.lesson001.demo12.ServiceB">
    <property name="name" value="Java"/>
    <property name="serviceA" ref="serviceA"/>
  </bean>

</beans>
```
创建测试用例。
<a name="g2sig"></a>
#### BeanExtendTest.java
```java
package com.javacode2018.lesson001.demo12;

import org.junit.Test;
import org.springframework.context.support.ClassPathXmlApplicationContext;

/**
 * bean定义继承案例
 */
public class BeanExtendTest {
    @Test
    public void normalBean() {
        String beanXml = "classpath:/com/javacode2018/lesson001/demo12/normalBean.xml";
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext(beanXml);
        System.out.println("serviceB:" + context.getBean(ServiceB.class));
        System.out.println("serviceC:" + context.getBean(ServiceC.class));
    }
}
```
<a name="l2xfb"></a>
#### 运行输出
```
serviceB:ServiceB{name='Java', serviceA=com.javacode2018.lesson001.demo12.ServiceA@222114ba}
serviceC:ServiceC{name='Java', serviceA=com.javacode2018.lesson001.demo12.ServiceA@222114ba}
```
<a name="uughZ"></a>
### 通过继承优化代码
再回头去看一下上面xml中，serviceB和serviceC两个bean的定义如下：
```xml
<bean id="serviceB" class="com.javacode2018.lesson001.demo12.ServiceB">
  <property name="name" value="Java"/>
  <property name="serviceA" ref="serviceA"/>
</bean>

<bean id="serviceC" class="com.javacode2018.lesson001.demo12.ServiceC">
  <property name="name" value="Java"/>
  <property name="serviceA" ref="serviceA"/>
</bean>
```
这2个bean需要注入的属性的值是一样的，都需要注入name和serviceA两个属性，并且2个属性的值也是一样的，可以将上面的公共的代码抽取出来，通过Spring中继承的方式来做到代码重用。<br />可以将上面xml调整一下，来新建一个extendBean.xml，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceA" class="com.javacode2018.lesson001.demo12.ServiceA"/>

  <bean id="baseService" abstract="true">
    <property name="name" value="Java"/>
    <property name="serviceA" ref="serviceA"/>
  </bean>

  <bean id="serviceB" class="com.javacode2018.lesson001.demo12.ServiceB" parent="baseService"/>

  <bean id="serviceC" class="com.javacode2018.lesson001.demo12.ServiceC" parent="baseService"/>

</beans>
```
**上面多了一个baseService的bean，这个bean没有指定class对象，但是多了一个**`**abstract="true"**`**的属性，表示这个bean是抽象的，**`**abstract**`**为**`**true**`**的bean在Spring容器中不会被创建，只是会将其当做bean定义的模板，而serviceB和serviceC的定义中多了一个属性parent，用来指定当前bean的父bean名称，此处是baseService，此时serviceB和serviceC会继承baseService中定义的配置信息。**<br />来个测试用例看一下效果:
```java
@Test
public void extendBean() {
    String beanXml = "classpath:/com/javacode2018/lesson001/demo12/extendBean.xml";
    ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext(beanXml);
    System.out.println("serviceB:" + context.getBean(ServiceB.class));
    System.out.println("serviceC:" + context.getBean(ServiceC.class));
}
```
运行输出：
```
serviceB:ServiceB{name='Java', serviceA=com.javacode2018.lesson001.demo12.ServiceA@222114ba}
serviceC:ServiceC{name='Java', serviceA=com.javacode2018.lesson001.demo12.ServiceA@222114ba}
```
输出和上面案例的输出基本一致。<br />但是这次bean xml中bean的定义简化了很多，将公共的bean配置提取出来了，通过parent属性来配置需要继承的bean。<br />**子bean中也可以重新定义父bean中已经定义好的配置，这样子配置会覆盖父bean中的配置信息**，将extendBean.xml中serviceC的定义改一下：
```xml
<bean id="serviceC" class="com.javacode2018.lesson001.demo12.ServiceC" parent="baseService">
  <property name="name" value="spring"/>
</bean>
```
运行extendBean输出：
```
serviceB:ServiceB{name='Java', serviceA=com.javacode2018.lesson001.demo12.ServiceA@222114ba}
serviceC:ServiceC{name='spring', serviceA=com.javacode2018.lesson001.demo12.ServiceA@222114ba}
```
从输出中可以看出serviceC中的name对父bean中name的值进行了覆盖。<br />再来从容器中获取一下baseService，如下：
```java
System.out.println(context.getBean("baseService"));
```
运行输出：
```
org.springframework.beans.factory.BeanIsAbstractException: Error creating bean with name 'baseService': Bean definition is abstract

    at org.springframework.beans.factory.support.AbstractBeanFactory.checkMergedBeanDefinition(AbstractBeanFactory.java:1412)
```
会报`BeanIsAbstractException`异常，因为baseService是抽象的，不能够创建这个bean实例。
<a name="EShEn"></a>
### 总结

1. **bean元素的**`**abstract**`**属性为true的时候可以定义某个bean为一个抽象的bean，相当于定义了一个bean模板，Spring容器并不会创建这个bean，从容器中查找abstract为true的bean的时候，会报错**`**BeanIsAbstractException**`**异常**
2. **bean元素的parent属性可以指定当前bean的父bean，子bean可以继承父bean中配置信息，也可以自定义配置信息，这样可以覆盖父bean中的配置**
