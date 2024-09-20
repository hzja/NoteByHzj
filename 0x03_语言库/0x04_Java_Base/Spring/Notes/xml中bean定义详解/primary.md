JavaSpring
<a name="q95WJ"></a>
## 存在的问题以及解决方案
直接上案例，通过案例来看技术是如何使用的：
```java
package com.javacode2018.lesson001.demo8;

public class NormalBean {
    public interface IService{} //@1
    public static class ServiceA implements IService{} //@2
    public static class ServiceB implements IService{} //@3
}
```
上面代码很简单，@1：定义了一个接口IService，@2和@3创建了两个类都实现了IService接口。<br />下面通过Spring来定义ServiceA和ServiceB两个bean，配置文件（normalBean.xml）如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceA" class="com.javacode2018.lesson001.demo8.NormalBean$ServiceA"/>

  <bean id="serviceB" class="com.javacode2018.lesson001.demo8.NormalBean$ServiceB"/>

</beans>
```
来个测试用例来从Spring容器中获取上面定义的bean对象，如下：
```java
package com.javacode2018.lesson001.demo8;

import com.javacode2018.lesson001.demo5.IocUtils;
import org.junit.Test;
import org.springframework.context.support.ClassPathXmlApplicationContext;

/**
 * bean元素的primary属性可以解决什么问题？
 */
public class PrimaryTest {
    @Test
    public void normalBean() {
        String beanXml = "classpath:/com/javacode2018/lesson001/demo8/normalBean.xml";
        ClassPathXmlApplicationContext context = IocUtils.context(beanXml);
        //下面通过spring容器的T getBean(Class<T> requiredType)方法获取容器中对应的bean
        NormalBean.IService service = context.getBean(NormalBean.IService.class); //@1
        System.out.println(service);
    }
}
```
注意@1的代码，从Spring容器中在容器中查找`NormalBean.IService.class`类型的bean对象，来运行一下看看效果，部分输出如下：
```
org.springframework.beans.factory.NoUniqueBeanDefinitionException: No qualifying bean of type 'com.javacode2018.lesson001.demo8.NormalBean$IService' available: expected single matching bean but found 2: serviceA,serviceB

    at org.springframework.beans.factory.support.DefaultListableBeanFactory.resolveNamedBean(DefaultListableBeanFactory.java:1180)
    at org.springframework.beans.factory.support.DefaultListableBeanFactory.resolveBean(DefaultListableBeanFactory.java:416)
    at org.springframework.beans.factory.support.DefaultListableBeanFactory.getBean(DefaultListableBeanFactory.java:349)
    at org.springframework.beans.factory.support.DefaultListableBeanFactory.getBean(DefaultListableBeanFactory.java:342)
    at org.springframework.context.support.AbstractApplicationContext.getBean(AbstractApplicationContext.java:1126)
```
发生异常了，错误中有一段提示比较重要，如下：
```
org.springframework.beans.factory.NoUniqueBeanDefinitionException: No qualifying bean of type 'com.javacode2018.lesson001.demo8.NormalBean$IService' available: expected single matching bean but found 2: serviceA,serviceB
```
这个详细说出了错误原因：Spring容器中定义了2个bean，分别是serviceA和serviceB，这两个bean对象都实现了IService接口，而用例中想从容器中获取IService接口对应的bean，此时容器中有2个候选者（serviceA和serviceB）满足需求，此时Spring容器不知道如何选择，到底是返回serviceA呢还是返回serviceB呢？Spring容器也懵逼了，所以报错了。<br />再来看一个通过setter方法注入的案例：
```java
package com.javacode2018.lesson001.demo8;

public class SetterBean {
    public interface IService{} //@1
    public static class ServiceA implements IService{} //@2
    public static class ServiceB implements IService{} //@3

    private IService service;

    public void setService(IService service) {
        this.service = service;
    }
}
```
下面通过xml来定义SetterBean，并且使用setter方式将IService注入到SetterBean中，配置如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceA" class="com.javacode2018.lesson001.demo8.SetterBean$ServiceA"/>
  <bean id="serviceB" class="com.javacode2018.lesson001.demo8.SetterBean$ServiceA"/>

  <bean id="setterBean" class="com.javacode2018.lesson001.demo8.SetterBean" autowire="byType" />
</beans>
```
注意上面setterBean的定义，`autowire="byType"`采用了按照类型自动注入的方式，容器启动的时候，会自动取调用SetterBean的setService方法，在容器中查找和这个方法参数类型匹配的bean，将查找的bean通过setService方法注入进去。<br />来个测试用例，PrimaryTest中加个方法：
```java
@Test
public void setterBean() {
    String beanXml = "classpath:/com/javacode2018/lesson001/demo8/setterBean.xml";
    ClassPathXmlApplicationContext context = IocUtils.context(beanXml);
}
```
运行输出：
```
org.springframework.beans.factory.UnsatisfiedDependencyException: Error creating bean with name 'setterBean' defined in class path resource [com/javacode2018/lesson001/demo8/setterBean.xml]: Unsatisfied dependency expressed through bean property 'service'; nested exception is org.springframework.beans.factory.NoUniqueBeanDefinitionException: No qualifying bean of type 'com.javacode2018.lesson001.demo8.SetterBean$IService' available: expected single matching bean but found 2: serviceA,serviceB

    at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.autowireByType(AbstractAutowireCapableBeanFactory.java:1526)
    at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.populateBean(AbstractAutowireCapableBeanFactory.java:1406)
```
错误中重点信息：
```
org.springframework.beans.factory.NoUniqueBeanDefinitionException: No qualifying bean of type 'com.javacode2018.lesson001.demo8.SetterBean$IService' available: expected single matching bean but found 2: serviceA,serviceB
```
容器中去找IService接口对应的bean，期望有一个匹配的，实际上却找到了2个匹配的，不知道如何选择，报错了。<br />上面2个案例报的异常都是下面这个异常：
```
org.springframework.beans.factory.NoUniqueBeanDefinitionException
```
**当希望从容器中获取到一个bean对象的时候，容器中却找到了多个匹配的bean，此时Spring不知道如何选择了，处于懵逼状态，就会报这个异常。**<br />**Spring中可以通过bean元素的primary属性来解决这个问题，可以通过这个属性来指定当前bean为主要候选者，当容器查询一个bean的时候，如果容器中有多个候选者匹配的时候，此时Spring会返回主要的候选者。**<br />下面使用primary来解决上面案例的问题：
```java
package com.javacode2018.lesson001.demo8;

public class PrimaryBean {
    public interface IService{} //@1
    public static class ServiceA implements IService{} //@2
    public static class ServiceB implements IService{} //@3

    private IService service;

    public void setService(IService service) {
        this.service = service;
    }

    @Override
    public String toString() {
        return "PrimaryBean{" +
        "service=" + service +
        '}';
    }
}
```
Spring配置文件如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceA" class="com.javacode2018.lesson001.demo8.PrimaryBean$ServiceA" primary="true"/>
  <bean id="serviceB" class="com.javacode2018.lesson001.demo8.PrimaryBean$ServiceA"/>

  <bean id="setterBean" class="com.javacode2018.lesson001.demo8.PrimaryBean" autowire="byType" />
</beans>
```
上面配置中将serviceA的primary属性置为true了，将其置为主要候选者，容器中如果查找bean的时候，如果有多个匹配的，就以他为主。<br />来个测试用例：
```java
@Test
public void primaryBean() {
    String beanXml = "classpath:/com/javacode2018/lesson001/demo8/primaryBean.xml";
    ClassPathXmlApplicationContext context = IocUtils.context(beanXml);
    PrimaryBean.IService service = context.getBean(PrimaryBean.IService.class); //@1
    System.out.println(service);
    PrimaryBean primaryBean = context.getBean(PrimaryBean.class); //@2
    System.out.println(primaryBean);
}
```
@1：从容器中查找IService类型匹配的bean，这个接口有2个实现类（ServiceA和Service2），这类在容器中都定义了，但是serviceA为主要的bean，所以这行代码会返回serviceA<br />@2：从容器中查找PrimaryBean类型的bean，容器中有一个，这个bean按照byType默认注入IService接口匹配的bean，注入的时候如果候选者有多个，以primary="true"的bean为主来注入，所以此处会注入service2<br />来运行一下，看看和分析的是否一致，运行输出：
```
com.javacode2018.lesson001.demo8.PrimaryBean$ServiceA@7b227d8d
PrimaryBean{service=com.javacode2018.lesson001.demo8.PrimaryBean$ServiceA@7b227d8d}
```
上面输出确实都是ServiceA，和分析的一致。<br />还有当候选者中如果有多个bean都将primary置为true，此时Spring还是会懵逼的，也会报错，不知道如何选择了。
<a name="xfgIi"></a>
## 总结
**当从容器中查找一个bean的时候，如果容器中出现多个Bean候选者时，可以通过**`**primary="true"**`**将当前bean置为首选者，那么查找的时候就会返回主要的候选者，否则将抛出异常。**
