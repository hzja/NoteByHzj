JavaSpringBoot<br />通过`@ImportResource`实现xml配置的装载 `@ImportResource`：导入Spring的配置文件，让配置文件里面的内容生效；通过初识springboot的实践，所有的bean装载全部通过java配置实现，那么一直以来习惯的xml配置是否就没有了用武之地呢，答案是否定的，下面就通过实践验证说明。

- 1、通过之前的实践，在启动类APP中定义了
```java
@ComponentScan(basePackages={"com.shf.SpringBoot1","com.shf.springboot.*"})
```
扫描相关的注解配置类，故本次案例中的bean放置在其他包下，从而通过xml文件注册；

- 2、首先定义一个service层bean：
```java
package com.shf.springboot2.service;
import org.springframework.stereotype.Service;

@Service
public class HelloService1 {
    public void method1(){
        System.out.println("class:HelloService1__method:method1");
    }
}
```

- 3、添加xml配置文件applicationContext.xml：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:context="http://www.springframework.org/schema/context"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans.xsd
  http://www.springframework.org/schema/context
  http://www.springframework.org/schema/context/spring-context.xsd" default-autowire="byName" default-lazy-init="false" >
  <description>Spring Configuration</description>
  <!-- 开启注解模式 -->
  <context:annotation-config />
  <!-- 使用Annotation自动注册Bean -->
  <context:component-scan base-package="com.shf.springboot2.service">
  </context:component-scan>
</beans>
```

- 4、下面就是新增一个关键的配置类`XmlConfiguration`：
```java
package com.shf.springboot.config;

import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.ImportResource;
@Configuration
@ImportResource(locations={"classpath:applicationContext.xml"})
public class XmlConfiguration {

}
```
该配置类此时必须在启动类中扫描到。

- 5、在控制其中注入2中定义的service对应的bean，并调用其方法：
```java
@RestController
public class HelloWorldController {
    @Autowired
    ServerConfig serverConfig;
    @Autowired
    ServerConfig2 serverConfig2;
    @Autowired
    HelloService1 helloService1;
    @RequestMapping("/")
    public String helloWorld() {
        System.out.println("server.properties服务器端口："+serverConfig.getPort());
        System.out.println("application.properties服务器端口："+serverConfig2.getPort());
        helloService1.method1();
        return "Hello World!";
    }
}
```

- 6、启动程序，验证结果，是否能够正常的打印service中的内容。

![](https://upload-images.jianshu.io/upload_images/4055666-e2ebf663867c30c7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240#id=xJdcG&originHeight=60&originWidth=318&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=)<br />以上正常装载bean，正常注入bean，正常使用bean实例。本案例主要使用到了一个新的注解： `**@ImportResource**`**：通过locations属性加载对应的xml配置文件，同时需要配合**`**@Configuration**`**注解一起使用，定义为配置类。**
