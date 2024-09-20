JavaSpringBoot
<a name="hlJTX"></a>
## `@Scope`：指定bean的作用域
<a name="rTK5t"></a>
### 用法
`@Scope`用来配置bean的作用域，等效于bean xml中的bean元素中的scope属性。<br />看一下其源码：
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Scope {

    @AliasFor("scopeName")
    String value() default "";

    @AliasFor("value")
    String scopeName() default "";

    ScopedProxyMode proxyMode() default ScopedProxyMode.DEFAULT;

}
```
`@Scope`可以用在类上和方法上<br />参数：value和scopeName效果一样，用来指定bean作用域名称，如：singleton、prototype
<a name="L6CzB"></a>
### 常见2种用法

1. 和`@Compontent`一起使用在类上
2. 和`@Bean`一起标注在方法上
<a name="Rp734"></a>
### 案例1：和`@Compontent`一起使用在类上
```java
@Component
@Scope(ConfigurableBeanFactory.SCOPE_SINGLETON)//@1
public class ServiceA {
}
```
上面定义了一个bean，作用域为单例的。<br />@1：`ConfigurableBeanFactory`接口中定义了几个作用域相关的常量，可以直接拿来使用，如：
```java
String SCOPE_SINGLETON = "singleton";
String SCOPE_PROTOTYPE = "prototype";
```
<a name="zz5ji"></a>
### 案例2：和`@Bean`一起标注在方法上
`@Bean`标注在方法上，可以通过这个方法来向Spring容器中注册一个bean，在此方法上加上`@Scope`可以指定这个bean的作用域，如：
```java
@Configurable
public class MainConfig2 {
    @Bean
    @Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
    public ServiceA serviceA() {
        return new ServiceA();
    }
}
```
<a name="BOUhL"></a>
## `@DependsOn`：指定当前bean依赖的bean
<a name="rPuXn"></a>
### 用法
`@DependsOn`等效于bean xml中的bean元素中的depend-on属性。<br />Spring在创建bean的时候，如果bean之间没有依赖关系，那么Spring容器很难保证bean实例创建的顺序，如果想确保容器在创建某些bean之前，需要先创建好一些其他的bean，可以通过`@DependsOn`来实现，`**@DependsOn**`**可以指定当前bean依赖的bean，通过这个可以确保**`**@DependsOn**`**指定的bean在当前bean创建之前先创建好**<br />看一下其源码：
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface DependsOn {

    String[] value() default {};

}
```
可以用在任意类型和方法上。<br />value：string类型的数组，用来指定当前bean需要依赖的bean名称，可以确保当前容器在创建被`@DependsOn`标注的bean之前，先将value指定的多个bean先创建好。
<a name="fKSUk"></a>
### 常见2种用法

1. 和`@Compontent`一起使用在类上
2. 和`@Bean`一起标注在方法上
<a name="Zqymv"></a>
### 案例1：和`@Compontent`一起使用在类上
下面定义3个bean：service1、service2、service3；service1需要依赖于其他2个service，需要确保容器在创建service1之前需要先将其他2个bean先创建好。<br />看代码：
<a name="hI90x"></a>
#### Service2
```java
package com.javacode2018.lesson001.demo27.test3;

import org.springframework.stereotype.Component;

@Component
public class Service2 {
    public Service2() {
        System.out.println("create Service2");
    }
}
```
<a name="ppcix"></a>
#### Service3
```java
package com.javacode2018.lesson001.demo27.test3;

import org.springframework.stereotype.Component;

@Component
public class Service3 {
    public Service3() {
        System.out.println("create Service3");
    }
}
```
<a name="m9CbM"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo27.test3;

import org.springframework.context.annotation.DependsOn;
import org.springframework.stereotype.Component;

@DependsOn({"service2", "service3"}) //@1
@Component
public class Service1 {
    public Service1() {
        System.out.println("create Service1");
    }
}
```
@1：使用了`@DependsOn`，指定了2个bean：service2和service3，那么Spring容器在创建上面这个service1的时候会先将`@DependsOn`中指定的2个bean先创建好
<a name="jDggp"></a>
#### 来个配置类
```java
package com.javacode2018.lesson001.demo27.test3;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig3 {
}
```
<a name="W1a1y"></a>
#### 测试用例
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig3.class);
    System.out.println(context.getBean(Service1.class));
}
```
<a name="jLgn0"></a>
#### 运行输出
```
create Service2
create Service3
create Service1
com.javacode2018.lesson001.demo27.test3.Service1@9f116cc
```
从输出中可以看到，Spring容器在创建service1之前，先将service2和service3创建好了。
<a name="ou56U"></a>
### 案例2：和`@Bean`一起标注在方法上
下面通过配置文件的方式来创建bean，如下：
```java
package com.javacode2018.lesson001.demo27.test4;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.DependsOn;

@Configurable
public class MainConfig4 {

    @Bean
    @DependsOn({"service2", "service3"})//@1
    public Service1 service1() {
        return new Service1();
    }

    @Bean
    public Service2 service2() {
        return new Service2();
    }

    @Bean
    public Service3 service3() {
        return new Service3();
    }

}
```
上面是一个Spring的配置类，类中3个方法定义了3个bean<br />@1：这个地方使用了`@DependsOn`，表示service1这个bean创建之前，会先创建好service2和service3<br />来个测试用例
```java
@Test
public void test4() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig4.class);
    System.out.println(context.getBean(com.javacode2018.lesson001.demo27.test4.Service1.class));
}
```
运行输出
```
create Service2
create Service3
create Service1
com.javacode2018.lesson001.demo27.test4.Service1@6e20b53a
```
<a name="lXjAW"></a>
## `@ImportResource`：配置类中导入bean定义的配置文件
<a name="p1OKz"></a>
### 用法
有些项目，前期可能采用xml的方式配置bean，后期可能想采用Spring注解的方式来重构项目，但是有些老的模块可能还是xml的方式，Spring为了方便在注解方式中兼容老的xml的方式，提供了`@ImportResource`注解来引入bean定义的配置文件。<br />bean定义配置文件：目前主要介绍了xml的方式，还有一种properties文件的方式，以后会介绍，此时还是以引入bean xml来做说明。<br />看一下这个注解的定义：
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Documented
public @interface ImportResource {

    @AliasFor("locations")
    String[] value() default {};

    @AliasFor("value")
    String[] locations() default {};

    Class<? extends BeanDefinitionReader> reader() default BeanDefinitionReader.class;

}
```
**通常将其用在配置类上。**<br />有3个参数：

- value和locations效果一样，只能配置其中一个，是一个string类型的数组，用来指定需要导入的配置文件的路径。
- reader：用来指定bean定义的读取器，目前知道的配置bean的方式有xml文件的方式，注解的方式，其实还有其他的方式，比如properties文件的方式，如果用其他的方式，得告诉Spring具体要用那种解析器去解析这个bean配置文件，这个解析器就是`BeanDefinitionReader`。
<a name="MFbuj"></a>
### 资源文件路径的写法
通常项目是采用Maven来组织的，配置文件一般会放在resources目录，这个目录中的文件被编译之后会在target/classes目录中。<br />Spring中资源文件路径最常用的有2种写法：

1. **以**`**classpath:**`**开头**：检索目标为当前项目的classes目录
2. **以**`**classpath*:**`**开头**：检索目标为当前项目的classes目录，以及项目中所有jar包中的目录，如果确定jar不是检索目标，就不要用这种方式，由于需要扫描所有jar包，所以速度相对于第一种会慢一些

那再来说`classpath:`和`classpath*:`后面的部分，后面的部分是确定资源文件的位置地方，几种常见的如下：
<a name="KOUBG"></a>
#### 相对路径的方式
```
classpath:com/javacode2018/lesson001/demo27/test5/beans.xml
或者
classpath*:com/javacode2018/lesson001/demo27/test5/beans.xml
```
<a name="PrTWm"></a>
#### `/`：绝对路径的方式
```
classpath:/com/javacode2018/lesson001/demo27/test5/beans.xml
```
<a name="gx6zl"></a>
#### `*`：文件通配符的方式
```
classpath:/com/javacode2018/lesson001/demo27/test5/beans-*.xml
```
会匹配test5目录中所有以beans-开头的xml结尾的文件
<a name="FxlVJ"></a>
#### `*`：目录通配符的方式
```
classpath:/com/javacode2018/lesson001/demo27/*/beans-*.xml
```
会匹配demo27中所有子目录中所有以beans-开头的xml结尾的文件，注意这个地方只包含demo27的子目录，不包含子目录的子目录，不会进行递归
<a name="ThR97"></a>
#### `**`：递归任意子目录的方式
```
classpath:/com/javacode2018/**/beans-*.xml
```
`**`会递归当前目录以及下面任意级的子目录<br />继续回到`@ImportResource`上来，来看案例
<a name="g9qU4"></a>
### 案例代码
来2个类，这两个类分别用2个xml来定义bean
<a name="BBP3i"></a>
#### ServiceA
```java
package com.javacode2018.lesson001.demo27.test5;

public class ServiceA {
}
```
<a name="rIDAG"></a>
#### ServiceB
```java
package com.javacode2018.lesson001.demo27.test5;

public class ServiceB {
}
```
<a name="i9kew"></a>
#### beans1.xml来定义serviceA这个bean，如下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceA" class="com.javacode2018.lesson001.demo27.test5.ServiceA"/>

</beans>
```
<a name="y2Cb2"></a>
#### beans2.xml来定义serviceB这个bean，如下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="serviceB" class="com.javacode2018.lesson001.demo27.test5.ServiceB"/>

</beans>
```
<a name="xvFbN"></a>
#### 下面来个配置类，来引入上面2个配置文件
```java
package com.javacode2018.lesson001.demo27.test5;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.context.annotation.ImportResource;

@Configurable
@ImportResource("classpath:/com/javacode2018/lesson001/demo27/test5/beans*.xml")
public class MainConfig5 {
}
```
这个类上使用了`@Configurable`表示这是个配置类<br />并且使用了`@ImportResource`注解来导入上面2个配置文件
<a name="ruvWK"></a>
#### 来个测试用例加载上面这个配置类
```java
@Test
public void test5() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig5.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
上面会输出MainConfig5配置类中所有定义的bean
<a name="RXeUH"></a>
#### 运行输出
```
mainConfig5->com.javacode2018.lesson001.demo27.test5.MainConfig5@4ec4f3a0
serviceA->com.javacode2018.lesson001.demo27.test5.ServiceA@223191a6
serviceB->com.javacode2018.lesson001.demo27.test5.ServiceB@49139829
```
从输出中可以看出2个xml中定义的bean也被注册了
<a name="nteI5"></a>
## `@Lazy`：延迟初始化
<a name="JNxis"></a>
### 用法
`@Lazy`等效于bean xml中bean元素的lazy-init属性，可以实现bean的延迟初始化。<br />**所谓延迟初始化：就是使用到的时候才会去进行初始化。**<br />来看一下其定义：
```java
@Target({ElementType.TYPE, ElementType.METHOD, ElementType.CONSTRUCTOR, ElementType.PARAMETER, ElementType.FIELD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Lazy {

    boolean value() default true;

}
```
可以用在任意类型、方法、构造器、参数、字段上面<br />参数：<br />value：boolean类型，用来配置是否应发生延迟初始化，默认为true。
<a name="TJtGZ"></a>
### 常用3种方式

1. 和`@Compontent`一起标注在类上，可以是这个类延迟初始化
2. 和`@Configuration`一起标注在配置类中，可以让当前配置类中通过`@Bean`注册的bean延迟初始化
3. 和`@Bean`一起使用，可以使当前bean延迟初始化

来看一下这3种方式案例代码。
<a name="IqU9z"></a>
### 案例1：和`@Compontent`一起使用
<a name="EEvVZ"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo27.test6;

import org.springframework.context.annotation.Lazy;
import org.springframework.stereotype.Component;

@Component
@Lazy //@1
public class Service1 {
    public Service1() {
        System.out.println("创建Service1");
    }
}
```
@1：使用到了`@Lazy`，默认值为true，表示会被延迟初始化，在容器启动过程中不会被初始化，当从容器中查找这个bean的时候才会被初始化。
<a name="ZiEar"></a>
#### 配置类
```java
package com.javacode2018.lesson001.demo27.test6;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig6 {
}
```
<a name="Y2asc"></a>
#### 测试用例
```java
@Test
public void test6() {
    System.out.println("准备启动spring容器");
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig6.class);
    System.out.println("spring容器启动完毕");
    System.out.println(context.getBean(com.javacode2018.lesson001.demo27.test6.Service1.class));
}
```
运行输出
```
准备启动spring容器
spring容器启动完毕
创建Service1
com.javacode2018.lesson001.demo27.test6.Service1@4fb61f4a
```
可以看出service1这个bean在Spring容器启动过程中并没有被创建，而是在调用`getBean`进行查找的时候才进行创建的，此时起到了延迟创建的效果。
<a name="JY3gr"></a>
### 案例2：和`@Configuration`一起使用加在配置类上
`@Lazy`和`@Configuration`一起使用，此时配置类中所有通过`@Bean`方式注册的bean都会被延迟初始化，不过也可以在`@Bean`标注的方法上使用`@Lazy`来覆盖配置类上的`@Lazy`配置，看下面代码：
<a name="OQZNU"></a>
#### 配置类MainConfig7
```java
package com.javacode2018.lesson001.demo27.test7;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Lazy;

@Lazy //@1
@Configurable
public class MainConfig7 {

    @Bean
    public String name() {
        System.out.println("create bean:name");
        return "Java";
    }

    @Bean
    public String address() {
        System.out.println("create bean:address");
        return "上海市";
    }

    @Bean
    @Lazy(false) //@2
    public Integer age() {
        System.out.println("create bean:age");
        return 30;
    }
}
```
@1：配置类上使用了`@Lazy`，此时会对当前类中所有`@Bean`标注的方法生效<br />@2：这个方法上面使用到了`@Lazy(false)`，此时age这个bean不会被延迟初始化。其他2个bean会被延迟初始化。
<a name="Y4X72"></a>
#### 测试用例
```java
@Test
public void test7() {
    System.out.println("准备启动spring容器");
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig7.class);
    System.out.println("spring容器启动完毕");

    for (String beanName : Arrays.asList("name", "age", "address")) {
        System.out.println("----------");
        System.out.println("getBean:" + beanName + ",start");
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
        System.out.println("getBean:" + beanName + ",end");
    }
}
```
上面会输出配置类中定义的3个bean的信息。
<a name="Mhjea"></a>
#### 运行输出
```
准备启动spring容器
create bean:age
spring容器启动完毕
----------
getBean:name,start
create bean:name
name->Java
getBean:name,end
----------
getBean:age,start
age->30
getBean:age,end
----------
getBean:address,start
create bean:address
address->上海市
getBean:address,end
```
输出中可以看到age是在容器启动过程中创建的，其他2个是在通过`getBean`查找的时候才创建的。
<a name="RZFxT"></a>
## 总结
本文介绍的几个注解也算是比较常用的

1. `@Scope`：用来定义bean 的作用域；2种用法：第1种：标注在类上；第2种：和`@Bean`一起标注在方法上
2. `@DependsOn`：用来指定当前bean依赖的bean，可以确保在创建当前bean之前，先将依赖的bean创建好；2种用法：第1种：标注在类上；第2种：和`@Bean`一起标注在方法上
3. `@ImportResource`：标注在配置类上，用来引入bean定义的配置文件
4. `@Lazy`：让bean延迟初始化；常见3种用法：第1种：标注在类上；第2种：标注在配置类上，会对配置类中所有的`@Bean`标注的方法有效；第3种：和`@Bean`一起标注在方法上
