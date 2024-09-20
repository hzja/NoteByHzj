Java Spring XML
<a name="dnoBo"></a>
## 引言
自从SpringBoot时代的到来，去除了Spring的各种繁琐的XML配置，可以腾出双手以便于更加专注于业务逻辑的开发。刚用Spring的时候，每天被Spring的各种XMl配置文件折磨的不行，每引入一个新的框架，最担心的就是jar冲突、哪个配置文件又配的不对、配置文件没有起作用。所以每次搭建好一个项目就把配置文件记录下来， 方便下次在整合项目的时候直接copy复制就好。下面就以Spring整合dubbo的事例看下
```xml
<beans xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:dubbo="http://dubbo.apache.org/schema/dubbo"
       xmlns="http://www.springframework.org/schema/beans"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
       http://dubbo.apache.org/schema/dubbo http://dubbo.apache.org/schema/dubbo/dubbo.xsd">
    <dubbo:application name="demo-provider"/>
    <dubbo:registry address="zookeeper://127.0.0.1:2181"/>
    <dubbo:protocol name="dubbo" port="20890"/>
    <bean id="demoService" class="org.apache.dubbo.samples.basic.impl.DemoServiceImpl"/>
    <dubbo:service interface="org.apache.dubbo.samples.basic.api.DemoService" ref="demoService"/>
</beans>
```
上述代码中可以看到dubbo自定义了一套自己的标签，`dubbo:application`，`dubbo:registry`，`dubbo:protocol`，`dubbo:service`，心中是不是有点小疑问：这些标签在Spring项目启动的时候是如何被Spring管理的？是怎样被Spring来识别的？如果自己随便定义一个标签Spring是否能够识别？去Spring的官网就可以发现这其实就是Spring提供的 XML schema 的扩展支持。只要按照它的步骤来，就可以配置任何自定义的标签。XML schema 扩展机制是什么？<br />Spring 为基于 XML 构建的应用提供了一种扩展机制，用于定义和配置 Bean。它允许使用者编写自定义的 XML bean 解析器，并将解析器本身以及最终定义的 Bean 集成到 Spring IOC 容器中。<br />可以看看官网[https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#xml-custom 10.2. XML Schema Authoring](https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#xml-custom 10.2. XML Schema Authoring) 这个是主要介绍它的。
<a name="UKu77"></a>
## 如何实现一个自定义 XML 扩展
官网有介绍，要实现一个自定义的XML Schema 总共需要4步：

- 编写一个 XML schema 文件描述节点元素。
- 编写一个 NamespaceHandler 的实现类
- 编写一个或者多个 BeanDefinitionParser 的实现 (关键步骤).
- 注册上述的 schema 和 handler。

既然只要按照这四步来，那就照着这个文档来自己实现一个。
<a name="bkYD8"></a>
### Authoring the Schema
编写一个javajr.xsd 放入项目的resources/META-INF文件夹里面（这个也可以是其他路径）
```xml
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema"
            xmlns:beans="http://www.springframework.org/schema/beans"
            xmlns:tool="http://www.springframework.org/schema/tool"
            xmlns="https://www.javajr.cn/schema/javajr"
            targetNamespace="https://www.javajr.cn/schema/javajr">

    <xsd:import namespace="http://www.springframework.org/schema/beans"/>

    <xsd:element name="application">
        <xsd:complexType>
            <xsd:complexContent>
                <xsd:extension base="beans:identifiedType">
                    <xsd:attribute name="website" type="xsd:string" use="required"/>
                    <xsd:attribute name="weixin" type="xsd:string" use="required"/>
                </xsd:extension>
            </xsd:complexContent>
        </xsd:complexType>
    </xsd:element>
</xsd:schema>
```

- `targetNamespace="https://www.javajr.cn/schema/javajr"` 这里`targetNamespace`的地址后面有用到。
- 这里就定义了一个元素application 里面有两个属性分别为website和weixin。
<a name="G8EUr"></a>
### 编写一个 `NamespaceHandler`
```java
package org.spring.demo.schema;
import org.springframework.beans.factory.xml.NamespaceHandlerSupport;
public class MyNamespaceHandler extends NamespaceHandlerSupport {
    @Override
    public void init() {
     // 这个名字也不是随便取的，上面编写xsd的根节点元素的name， <xsd:element name="application">
        registerBeanDefinitionParser("application", new MyBeanDefinitionParser());
    }
}
```
这个`NamespaceHandler `就是将一个 XML 节点解析成 IOC 容器中的一个实体类。也就是说相当于在xml里面的配置的对象，通过Spring ioc 容器管理起来了
<a name="gpFxx"></a>
### 编写 `BeanDefinitionParser` 的实现类
```java
package org.spring.demo.schema;
import org.spring.demo.domain.JavajrDomain;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.xml.AbstractSingleBeanDefinitionParser;
import org.springframework.util.StringUtils;
import org.w3c.dom.Element;

public class MyBeanDefinitionParser extends AbstractSingleBeanDefinitionParser {

    @Override
    protected Class<?> getBeanClass(Element element) {
        return JavajrDomain.class;
    }
    @Override
    protected void doParse(Element element, BeanDefinitionBuilder bean) {
        // this however is an optional property
        String website = element.getAttribute("website");
        if (StringUtils.hasText(website)) {
            bean.addPropertyValue("website",website);
        }
        String weiXin = element.getAttribute("weixin");
        if (StringUtils.hasText(weiXin)) {
            bean.addPropertyValue("weixin",weiXin);
        }
    }
}
```
上面在这个实现类只是简单的做了一个赋值操作，如果需要有自己的逻辑业务也可以自行来实现。上面还有一个JavajrDomain这个实体类就不贴代码，就一个简单的javabean里面包含了两个属性weixin和website。
<a name="DB4Oe"></a>
### 注册schema组件
最后在resources/META-INF目录下添加两个配置文件(`spring.handler`和`spring.schema`):

- `resources/META-INF/spring.handlers`
```
https\://www.javajr.cn/schema/javajr=org.spring.demo.schema.MyNamespaceHandler
```

- `resources/META-IN/spring.schemas`
```
https\://www.javajr.cn/schema/javajr.xsd=META-INF/javajr.xsd
```
在这个地方的时候其实可以以版本号来进行命名，可以方便使用多个不同的版本，Spring-beans 就是。<br />![2021-06-04-22-58-20-583172.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622819358418-3ceab70f-b2af-4345-bd71-112c49565968.png#averageHue=%237b8887&clientId=u00325c02-75b1-4&from=ui&id=u651c3805&originHeight=808&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=357627&status=done&style=shadow&taskId=ufe28e7a4-277c-4dac-b7ee-6df47629d8f&title=)
<a name="YikU1"></a>
### 测试自定义schema

- 在resources 目录下新建一个applicationContext.xml文件

![2021-06-04-22-58-20-700366.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622819371864-f8fe5aba-33ce-4243-a611-1f274af13170.png#averageHue=%236d8e75&clientId=u00325c02-75b1-4&from=ui&id=ufb03dbb5&originHeight=549&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=137962&status=done&style=shadow&taskId=u437b0e72-e0fc-4390-a245-fc0bde48b66&title=)<br />这个文件就是使用下自己自定义的schema，这个文件需要注意的就是上面标红的这几行，一般如果有引入过第三方的框架，比如mq、或者dubbo等它们都有自定义的这些玩意。

- 编写一个启动类
```java
public static void main(String[] args) {
    ApplicationContext ctx = new ClassPathXmlApplicationContext("classpath:applicationContext.xml");
    JavajrDomain bean = ctx.getBean(JavajrDomain.class);
    System.out.println(bean.toString());
}

```
我们可以看到控制台输出
```java
JavajrDomain{weixin='javajr8', website='javajr.cn'}
```

- 到这里实现的了一个 XML schema ，只要照着官方文档操作就可以了。照着操作的过程可能有几个小细节需要注意下引入 XML schema 的时候需要注意下空格，或者一些特殊符号。
<a name="TZTKv"></a>
## Dubbo 中的 XML schema 扩展
在开始的时候有介绍dubbo 自定义的XML schema ，下面打开dubbo源码看看它是如何来实现的，看下面这个截图，也是按照那四步来的。<br />![2021-06-04-22-58-20-795114.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622819391083-05da5afb-9111-4227-919a-b46e603ffef7.png#averageHue=%23504c41&clientId=u00325c02-75b1-4&from=ui&id=u4e998bbc&originHeight=790&originWidth=735&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61591&status=done&style=shadow&taskId=u1c650199-d0f7-4aee-baf0-c3dd4e1e803&title=)
<a name="nJvlx"></a>
## SpringBoot的starter
现在有了SpringBoot 之后以前用这个 XML schema配置的框架，大多数都会有对应的starter来进行封装，starter的使用比起 XML schema的使用还是简单多了，开箱即用，无需编写很多的配置文件。
<a name="BTFSK"></a>
## 总结
虽然现在XML schema 扩展用的不多了，但是应该也还有比较老的项目在使用吧，如果还是比较老的项目，需要引入一个什么样的框架，至少需要知道需要怎么去引入。
