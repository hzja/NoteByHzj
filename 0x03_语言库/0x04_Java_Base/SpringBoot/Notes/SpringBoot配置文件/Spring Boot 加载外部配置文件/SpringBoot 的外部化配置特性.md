JavaSpringBoot<br />作为 Java 程序员，相信大家都知道日常的 SpringBoot 项目会有一个配置文件 application.properties 文件。<br />里面会配置很多参数，例如服务的端口等，这些都只是默认值，在不改变配置文件里面内容的情况下，可以通过在部署的时候，传递一个相应的参数来替换默认的参数。<br />那么问题来了，你有想过为什么可以这样吗？为什么 SpringBoot 部署时传递的启动配置会生效，而配置文件中的配置就不生效了呢？或者说这两者的优先级是什么样子的呢？
<a name="edK26"></a>
## 外部化配置
要解释上面的问题，就需要知道 SpringBoot 到底支持哪些配置形式，以及这些配置方式的优先级是什么样子的，只有搞清楚了这个，才能真正的解决配置的优先级问题。<br />在 SpringBoot 的官方文档中可以看到这么一段描述<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273531783-6f76220b-72fa-4b8a-b840-0ddc9caf7451.png#averageHue=%23efefef&clientId=ue88b9f5b-888e-4&from=paste&id=u202d5575&originHeight=152&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue7fff721-e1cc-4d6d-8aac-0937288c67f&title=)<br />大概的意思就是：Spring Boot 提供了将配置文件外部化的功能，这样就可以在不同环境下使用相同的应用程序代码。可以使用 properties 文件、YAML 文件、环境变量以及命令行参数来外部化配置文件。<br />通过 `@Value` 注解，属性值可以直接注入到 beans 中，通过 Environment abstraction（环境映射）可以访问其他位置，或者使用 `@ConfigurationProperties` 绑定结构化对象。
<a name="v3wIA"></a>
## 有哪些外部配置
既然上面提到了 SpringBoot 提供了外部化配置，那么 SpringBoot 提供了哪些配置呢？依然是通过官方文档，可以看到有如下配置列表<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273531708-e722752a-5803-4800-ac09-3ddeaedb7222.png#averageHue=%23f3f2f2&clientId=ue88b9f5b-888e-4&from=paste&id=ua17c383d&originHeight=485&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueb280214-d288-46d4-b7bd-a47a889f188&title=)<br />从上图可以看到 SpringBoot 总共内置了 17 种外部化配置方法，而且这 17 种的优先级是从上到下依次优先的。这些方式中常用的有 4 命令行方法，9 Java 系统环境变量，10 操作系统环境变量，以及 12 到 15 到配置文件的形式。<br />通过上面的顺序就可以解释为什么通过命令行配置的参数会生效，而配置文件中的默认值就会忽略了，从而达到了覆盖配置的目的。
<a name="LjFxM"></a>
## `PropertySource`
上面的文档中也提到了，SpringBoot 主要是通过 `PropertySource` 机制来实现多样属性源的，SpringBoot 的 `PropertySource` 是一种机制，用于加载和解析配置属性，可以从多种来源获取这些属性，例如文件、系统环境变量、JVM 系统属性和命令行参数等。`PropertySource` 是 Spring 框架中的一个抽象接口，它定义了如何读取属性源的方法。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273531684-821737c7-86dd-4ff6-8f6c-45db20e392ea.png#averageHue=%23424037&clientId=ue88b9f5b-888e-4&from=paste&id=uf2edbe41&originHeight=368&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7df30d4a-bf2c-4431-8f2b-0bce9d3a9df&title=)<br />通过 SpringBoot 的代码，可以看到，`org.springframework.core.env.PropertySource` 是一个抽象类，实现在子类有很多，上面提到的命令行 `PropertySource` 是 `org.springframework.core.env.CommandLinePropertySource`。整体的类图如下，涵盖的内容还是很多的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273531711-b7859269-85e6-4a44-a7e3-17dd7b15e97c.png#averageHue=%232c2c2c&clientId=ue88b9f5b-888e-4&from=paste&id=u9d88d781&originHeight=377&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3c48676a-cdf5-49df-9d50-0e5a149eb36&title=)<br />另外在 SpringBoot 中，还可以使用 `@PropertySource` 注解来自定义指定要加载的属性文件。例如，可以在应用程序的主类上添加以下注解：
```java
@SpringBootApplication
@PropertySource("classpath:customer.properties")
public class CustomerProperties {
   // ...
}
```
这将告诉 SpringBoot 在 classpath 下查找名为 customer.properties 的文件，并将其加载为属性源。然后，可以使用 `@Value`注解将属性值注入到 bean 中，如下所示：
```java
@Service
public class MyService {
   @Value("${my.property}")
   private String myProperty;
   // ...
}
```
这里的 `${my.property}` 是从 customer.properties 文件中获取的属性值。如果找不到该属性，那么 SpringBoot 将使用默认值，这里因为是自定义的属性，是没有默认值的，就会报错，项目无法启动。<br />具体实现是，SpringBoot 在启动时会自动加载和解析所有的 `PropertySource`，包括默认的 `PropertySource` 和自定义的`PropertySource`。这些属性值被存储在 Spring 环境中，可以通过 Spring 的 Environment 对象访问。当属性被注入到 bean 中时， Spring 会查找 `Environment` 对象并尝试解析属性的值。<br />总之，SpringBoot 的 `PropertySource` 提供了一种简单的方法来加载和解析应用程序的配置属性，这些属性可以从多个来源获取。它通过将属性值存储在 Spring 环境中，使其易于在应用程序的不同部分中使用。
<a name="AVcTI"></a>
## 调试
为了验证上面说的命令行的参数配置要优先于配置文件，创建一个 SpringBoot 项目，并且在 application.properties 文件中配置一个参数 name=JavaGeekTech，而在 IDEA 启动窗口中配置 name=JAVA_JIKEJUSHU，分别如下所示<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273531742-27a19556-fe85-43b0-b49a-9b181375d0fa.png#averageHue=%23424140&clientId=ue88b9f5b-888e-4&from=paste&id=ud020d777&originHeight=323&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6727543f-cc0c-43c7-87d8-455f9231fe1&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273532358-caaab586-e892-4707-8f64-b44c9b5de67b.png#averageHue=%23303335&clientId=ue88b9f5b-888e-4&from=paste&id=u2947fd36&originHeight=518&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1b4b71e3-6d39-44b2-b764-d4d3992b3e6&title=)<br />在写一个简单的 HelloController 类，并且通过 `@Value` 注解注入 name 属性，接下来就需要调试看下，SpringBoot是如何将 name 属性赋值的。通过验证 name 会被赋值成 JAVA_JIKEJISHU 而不是 JavaGeekTech。
```java
package com.example.demo.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @Value("${name}")
    private String name;

    @GetMapping(value = "/hello")
    public String hello() {
        return helloService.sayHello(name);
    }

}
```
接着启动 debug，因为是基于 SpringBoot 的，属性的赋值是在创建 bean 的时候，从 `createBean`，到 `doCreateBean`，再到 `org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory#populateBean`，因为每个 bean 都会经过很多 `PostProcessor` 的处理，属性赋值的 `PostProcessor` 是 `org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor#postProcessProperties`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273532424-7172b153-0c0a-4ed2-b086-2be2b1aaffa5.png#averageHue=%233b3a39&clientId=ue88b9f5b-888e-4&from=paste&id=ubc35ea20&originHeight=377&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0941a065-c399-42d0-b8b3-671c96613fa&title=)<br />里面的 metadata.inject 会调用到 `org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor.AutowiredFieldElement#inject`，再到 `org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor.AutowiredFieldElement#resolveFieldValue`，<br />`org.springframework.beans.factory.support.DefaultListableBeanFactory#resolveDependency`，<br />`org.springframework.beans.factory.support.DefaultListableBeanFactory#doResolveDependency`，<br />`org.springframework.beans.factory.support.AbstractBeanFactory#resolveEmbeddedValue`，<br />`org.springframework.core.env.AbstractPropertyResolver#resolveRequiredPlaceholders`，<br />`org.springframework.core.env.PropertySourcesPropertyResolver#getPropertyAsRawString`，<br />`org.springframework.core.env.PropertySourcesPropertyResolver#getProperty(java.lang.String, java.lang.Class<T>, boolean)`<br />整体调用链还是挺长的，不过只要跟着思路，在配合断点，还是可以看看看出来的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273532605-2387317c-5940-45b2-9413-4068e34ea340.png#averageHue=%23423d3d&clientId=ue88b9f5b-888e-4&from=paste&id=uaf445b9e&originHeight=284&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u31c8918b-b59a-45ef-8e3e-92a34f2ce1d&title=)<br />在 `getProperty` 方法中，可以看到如下的逻辑，根据 key 获取到的 value 值为JAVA_JIKEJISHU。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273532560-31c6474a-8098-4d61-8bef-1d4956e7fa94.png#averageHue=%233a3938&clientId=ue88b9f5b-888e-4&from=paste&id=u51ca2783&originHeight=338&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u48983352-c911-4106-bffe-78a06292db0&title=)<br />继续跟踪 `getProperty` 方法，可以看到这个方法 `org.springframework.boot.context.properties.source.ConfigurationPropertySourcesPropertySource#findConfigurationProperty(org.springframework.boot.context.properties.source.ConfigurationPropertyName`)，<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273532508-22ee7378-1938-4959-8714-7a70e7379ea6.png#averageHue=%233f3e3e&clientId=ue88b9f5b-888e-4&from=paste&id=ueec161ca&originHeight=289&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u70e17c30-e18e-4ce2-a9f6-86824390d4d&title=)<br />其中的 `getSource()` 中就有配置的两个属性源的数据，如下所示<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679273532952-bf2ab1aa-7e02-4b84-9939-5a326ef936f2.png#averageHue=%23303335&clientId=ue88b9f5b-888e-4&from=paste&id=u0e0415d8&originHeight=604&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8066177f-3d82-4303-b3ec-b9116befb95&title=)<br />根据代码逻辑，也可以看到，在迭代的时候，如果找到了一个就直接返回了，所以得到的结果是JAVA_JIKEJISHU。
