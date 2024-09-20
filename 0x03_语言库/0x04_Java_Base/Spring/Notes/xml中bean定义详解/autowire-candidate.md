JavaSpring
<a name="Ujdvy"></a>
## autowire-candidate做什么事情的？
当容器中某种类型的bean存在多个的时候，此时如果从容器中查找这种类型的bean的时候，会报下面这个异常：
```
org.springframework.beans.factory.NoUniqueBeanDefinitionException
```
**原因：当从容器中按照类型查找一个bean对象的时候，容器中却找到了多个匹配的bean，此时spring不知道如何选择了，处于懵逼状态，就会报这个异常。**<br />这种异常主要出现在2种场景中：
<a name="PNv3w"></a>
### 场景1：
从容器容器中查找符合指定类型的bean，对应`BeanFactory`下面的方法：
```java
<T> T getBean(Class<T> requiredType) throws BeansException;
```
<a name="ezqbc"></a>
### 场景2：
自动注入方式设置为byType的时候，如下：
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
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceA" class="com.javacode2018.lesson001.demo8.SetterBean$ServiceA"/>
  <bean id="serviceB" class="com.javacode2018.lesson001.demo8.SetterBean$ServiceB"/>

  <bean id="setterBean" class="com.javacode2018.lesson001.demo8.SetterBean" autowire="byType" />
</beans>
```
setterBean的`autowire`设置的是`byType`，即按setter方法的参数类型自动注入，SetterBean的setService的类型是IService，而IService类有2个实现类：ServiceA和ServiceB，而容器容器中刚好有这2个实现类的bean：serviceA和serviceB，所以上面代码会报错，不知道注入的时候选择那个对象注入。<br />**可以通过primary属性来指定一个主要的bean，当从容器中查找的时候，如果有多个候选的bean符合查找的类型，此时容器将返回**`**primary="true"**`**的bean对象。**<br />spring还有一种方法也可以解决这个问题，可以设置某个bean是否在自动注入的时候是否为作为候选bean，通过bean元素的autowire-candidate属性类配置，如下：
```xml
<bean id="serviceA" class="com.javacode2018.lesson001.demo8.SetterBean$ServiceA" autowire-candidate="false"/>
```
autowire-candidate：设置当前bean在被其他对象作为自动注入对象的时候，是否作为候选bean，默认值是true。<br />来举例说明一下，以上面的setter注入的案例先来说一下注入的过程：<br />**容器在创建setterBean的时候，发现其autowire为byType，即按类型自动注入，此时会在SetterBean类中查找所有setter方法列表，其中就包含了setService方法，setService方法参数类型是IService，然后就会去容器中按照IService类型查找所有符合条件的bean列表，此时容器中会返回满足IService这种类型并且**`**autowire-candidate="true"**`**的bean，刚才有说过bean元素的autowire-candidate的默认值是true，所以容器中符合条件的候选bean有2个：serviceA和serviceB，setService方法只需要一个满足条件的bean，此时会再去看这个列表中是否只有一个主要的bean（即bean元素的**`**primary="ture"**`**的bean），而bean元素的primary默认值都是false，所以没有primary为true的bean，此时spring容器懵了，不知道选哪个了，此时就报错了，抛出NoUniqueBeanDefinitionException异常**<br />从上面过程中可以看出将某个候选bean的primary置为true就可以解决问题了。<br />或者只保留一个bean的autowire-candidate为true，将其余的满足条件的bean的autowire-candidate置为false，此时也可以解决这个问题，下面使用autowire-candidate来解决上面问题看一下效果：<br />**SetterBean.java**
```java
package com.javacode2018.lesson001.demo9;

public class SetterBean {
    public interface IService {} //@1

    public static class ServiceA implements IService {} //@2

    public static class ServiceB implements IService {} //@3

    private IService service;

    public void setService(IService service) {
        this.service = service;
    }

    @Override
    public String toString() {
        return "SetterBean{" +
        "service=" + service +
        '}';
    }
}
```
**autowireCandidateBean.xml**
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceA" class="com.javacode2018.lesson001.demo9.SetterBean$ServiceA" autowire-candidate="false"/>
  <bean id="serviceB" class="com.javacode2018.lesson001.demo9.SetterBean$ServiceB"/>

  <bean id="setterBean" class="com.javacode2018.lesson001.demo9.SetterBean" autowire="byType" />
</beans>
```
上面将serviceA的autowire-candidate置为false了，serviceA在被其他bean自动按照类型注入的时候，将不再放入候选名单中<br />**测试用例**
```java
package com.javacode2018.lesson001.demo9;

import com.javacode2018.lesson001.demo5.IocUtils;
import com.javacode2018.lesson001.demo8.NormalBean;
import com.javacode2018.lesson001.demo8.PrimaryBean;
import org.junit.Test;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import java.util.Arrays;
import java.util.Map;

/**
 * bean元素的autowire-candidate可以设置当前bean是否作为其他bean自动注入的候选bean
 */
public class AutowireCandidateTest {

    @Test
    public void setterBean() {
        String beanXml = "classpath:/com/javacode2018/lesson001/demo9/autowireCandidateBean.xml";
        ClassPathXmlApplicationContext context = IocUtils.context(beanXml);
        System.out.println(context.getBean(SetterBean.class)); //@1

        SetterBean.IService service = context.getBean(SetterBean.IService.class); //@2
        System.out.println(service);
    }

}
```
@1：查找容器中`SetterBean`类型的bean对象<br />@2：查找容器中`SetterBean.IService`接口类型的bean，实际上面容器中serviceA和serviceB都是这种类型的<br />下面运行一下，看看输出：
```
SetterBean{service=com.javacode2018.lesson001.demo9.SetterBean$ServiceB@29176cc1}
com.javacode2018.lesson001.demo9.SetterBean$ServiceB@29176cc1
```
注意一下输出，2行输出中都是ServiceB，因为serviceB的autowire-candidate是默认值true，自动注入的时候作为候选bean，而serviceA的autowire-candidate是false，自动注入的时候不作为候选bean，所以上面输出的都是serviceB。
<a name="eDkPO"></a>
## `autowire-candidates`属性解析源码
beans元素是xml中定义bean的根元素，beans元素有个`default-autowire-candidates`属性，用于定义哪些bean可以作为候选者，`default-autowire-candidates`的值是个通配符如：
```
default-autowire-candidates="*Service"
```
再来说一下bean元素的autowire-candidate属性，这个属性有3个可选值：

- default：这个是默认值，autowire-candidate如果不设置，其值就是default
- true：作为候选者
- false：不作为候选者

spring中由beans元素的`default-autowire-candidates`和bean元素的`autowire-candidate`来决定最终bean元素`autowire-candidate`的值，来看一下bean元素`autowire-candidates`的解析源码：
```
源码位置：org.springframework.beans.factory.xml.BeanDefinitionParserDelegate#parseBeanDefinitionAttributes
```
主要代码如下：
```java
//获取bean元素的autowire-candidate元素，autowire-candidate如果不设置，其值就是default
String autowireCandidate = ele.getAttribute(AUTOWIRE_CANDIDATE_ATTRIBUTE);
//判断bean元素的autowire-candidate元素是否等于"default"或者是否等于""
if (isDefaultValue(autowireCandidate)) { 
    //获取beans元素default-autowire-candidates属性值
    String candidatePattern = this.defaults.getAutowireCandidates();
    //判断获取beans元素default-autowire-candidates属性值是否为空，default-autowire-candidates默认值就是null
    if (candidatePattern != null) {
        //判断bean的名称是否和default-autowire-candidates的值匹配，如果匹配就将bean的autowireCandidate置为true，否则置为false
        String[] patterns = StringUtils.commaDelimitedListToStringArray(candidatePattern);
        bd.setAutowireCandidate(PatternMatchUtils.simpleMatch(patterns, beanName));
    }
}else {
    //判断bean的autowire-candidate的值是否等于"true"
    bd.setAutowireCandidate(TRUE_VALUE.equals(autowireCandidate));
}
```
如果上面判断都没有进去，`autowireCandidate`属性默认值就是true，这个在下面定义的：
```
org.springframework.beans.factory.support.AbstractBeanDefinition#autowireCandidate

private boolean autowireCandidate = true;
```
**所有的bean元素最后都会被解析为spring中的org.springframework.beans.factory.config.BeanDefinition对象**。
