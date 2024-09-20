JavaSpringBoot
<a name="TQFw5"></a>
## `@Import`出现的背景
到目前，批量定义bean的方式有2种：

1. `@Configuration`结合`@Bean`注解的方式
2. `@CompontentScan`扫描包的方式

下面来看几个问题。
<a name="U8Tcv"></a>
### 问题1
如果需要注册的类是在第三方的jar中，那么如果想注册这些bean有2种方式：

1. 通过`@Bean`标注方法的方式，一个个来注册
2. `@CompontentScan`的方式：默认的`@CompontentScan`是无能为力的，默认情况下只会注册`@Compontent`标注的类，此时只能自定义`@CompontentScan`中的过滤器来实现了

这2种方式都不是太好，每次有变化，调整的代码都比较多。
<a name="xwCqY"></a>
### 问题2
通常项目中有很多子模块，可能每个模块都是独立开发的，最后通过jar的方式引进来，每个模块中都有各自的`@Configuration`、`@Bean`标注的类，或者使用`@CompontentScan`标注的类，**被**`**@Configuration**`**、**`**@Bean**`**、**`**@CompontentScan**`**标注的类，统称为bean配置类，配置类可以用来注册bean**，此时如果只想使用其中几个模块的配置类，怎么办？<br />`@Import`可以很好的解决这2个问题，下面来看`@Import`怎么玩的。
<a name="KAn1E"></a>
## `@Import`使用
先看Spring对它的注释，总结下来作用就是和xml配置的 `<import />`标签作用一样，允许通过它引入`@Configuration`标注的类 ， 引入`ImportSelector`接口和`ImportBeanDefinitionRegistrar`接口的实现，也包括 `@Component`注解的普通类。<br />**总的来说：**`**@Import**`**可以用来批量导入需要注册的各种类，如普通的类、配置类，完后完成普通类和配置类中所有bean的注册。**<br />`@Import`的源码：
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Import {

    /**
     * {@link Configuration @Configuration}, {@link ImportSelector},
     * {@link ImportBeanDefinitionRegistrar}, or regular component classes to import.
     */
    Class<?>[] value();

}
```
`@Import`可以使用在任何类型上，通常情况下，类和注解上用的比较多。<br />value：一个Class数组，设置需要导入的类，可以是`@Configuration`标注的列，可以是`ImportSelector`接口或者`ImportBeanDefinitionRegistrar`接口类型的，或者需要导入的普通组件类。
<a name="h6qVc"></a>
### 使用步骤

1. 将`@Import`标注在类上，设置value参数
2. 将`@Import`标注的类作为`AnnotationConfigApplicationContext`构造参数创建`AnnotationConfigApplicationContext`对象
3. 使用`AnnotationConfigApplicationContext`对象
<a name="hjQ9l"></a>
### `@Import`的value常见的有5种用法

1. **value为普通的类**
2. **value为**`**@Configuration**`**标注的类**
3. **value为**`**@CompontentScan**`**标注的类**
4. **value为**`**ImportBeanDefinitionRegistrar**`**接口类型**
5. **value为**`**ImportSelector**`**接口类型**
6. **value为**`**DeferredImportSelector**`**接口类型**

下面通过案例来一个个详细介绍。
<a name="VnIx1"></a>
## value为普通的类
来2个类
<a name="KtLLq"></a>
### Service1
```java
package com.javacode2018.lesson001.demo24.test1;

public class Service1 {
}
```
<a name="bbNnx"></a>
### Service2
```java
package com.javacode2018.lesson001.demo24.test1;

public class Service2 {
}
```
<a name="AImAM"></a>
### 总配置类：使用`@Import`标注
```java
package com.javacode2018.lesson001.demo24.test1;

import org.springframework.context.annotation.Import;

@Import({Service1.class, Service2.class})
public class MainConfig1 {
}
```
`@Import`中导入了2个普通的类：Service1、Service2，这两个类会被自动注册到容器中
<a name="gM9q5"></a>
### 测试用例
```java
package com.javacode2018.lesson001.demo24;

import com.javacode2018.lesson001.demo24.test1.MainConfig1;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class ImportTest {
    @Test
    public void test1() {
        //1.通过AnnotationConfigApplicationContext创建spring容器，参数为@Import标注的类
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig1.class);
        //2.输出容器中定义的所有bean信息
        for (String beanName : context.getBeanDefinitionNames()) {
            System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
        }
    }
}
```
<a name="VVQXR"></a>
### 运行输出
部分输出如下：
```
com.javacode2018.lesson001.demo24.test1.Service1->com.javacode2018.lesson001.demo24.test1.Service1@7e0b85f9
com.javacode2018.lesson001.demo24.test1.Service2->com.javacode2018.lesson001.demo24.test1.Service2@63355449
```
<a name="zaNov"></a>
### 结果分析
从输出中可以看出：

1. Service1和Service2成功注册到容器了。
2. 通过`@Import`导入的2个类，bean名称为完整的类名

也可以指定被导入类的bean名称，使用`@Compontent`注解就可以了，如下：
```java
@Component("service1")
public class Service1 {
}
```
再次运行test1输出：
```
service1->com.javacode2018.lesson001.demo24.test1.Service1@45efd90f
```
<a name="TVnQU"></a>
### 总结一下
**按模块的方式进行导入，需要哪个导入哪个，不需要的时候，直接修改一下总的配置类，调整一下**`**@Import**`**就可以了，非常方便。**
<a name="ZIhK3"></a>
## value为`@Configuration`标注的配置类
项目比较大的情况下，会按照模块独立开发，每个模块在maven中就表现为一个个的构建，然后通过坐标的方式进行引入需要的模块。<br />假如项目中有2个模块，2个模块都有各自的配置类，如下
<a name="Eohel"></a>
### 模块1配置类
```java
package com.javacode2018.lesson001.demo24.test2;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * 模块1配置类
 */
@Configuration
public class ConfigModule1 {
    @Bean
    public String module1() {
        return "我是模块1配置类！";
    }
}
```
<a name="xFykI"></a>
### 模块2配置类
```java
package com.javacode2018.lesson001.demo24.test2;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * 模块2配置类
 */
@Configuration
public class ConfigModule2 {
    @Bean
    public String module2() {
        return "我是模块2配置类！";
    }
}
```
<a name="vl0un"></a>
### 总配置类：通过`@Import`导入2个模块的配置类
```java
package com.javacode2018.lesson001.demo24.test2;

import org.springframework.context.annotation.Import;

/**
 * 通过Import来汇总多个@Configuration标注的配置类
 */
@Import({ConfigModule1.class, ConfigModule2.class}) //@1
public class MainConfig2 {
}
```
@1导入了2个模块中的模块配置类，可以按需导入。
<a name="RyxC9"></a>
### 测试用例
ImportTest中新增个方法
```java
@Test
public void test2() {
    //1.通过AnnotationConfigApplicationContext创建spring容器，参数为@Import标注的类
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig2.class);
    //2.输出容器中定义的所有bean信息
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="PgL0J"></a>
### 运行输出
```
mainConfig2->com.javacode2018.lesson001.demo24.test2.MainConfig2@ba2f4ec
com.javacode2018.lesson001.demo24.test2.ConfigModule1->com.javacode2018.lesson001.demo24.test2.ConfigModule1$$EnhancerBySpringCGLIB$$700e65cd@1c1bbc4e
module1->我是模块1配置类！
com.javacode2018.lesson001.demo24.test2.ConfigModule2->com.javacode2018.lesson001.demo24.test2.ConfigModule2$$EnhancerBySpringCGLIB$$a87108ee@55fe41ea
module2->我是模块2配置类！
```
<a name="xngz9"></a>
## value为`@CompontentScan`标注的类
项目中分多个模块，每个模块有各自独立的包，在每个模块所在的包中配置一个`@CompontentScan`类，然后通过`@Import`来导入需要启用的模块。
<a name="XxXwS"></a>
### 定义模块1
2个组件和一个组件扫描类，模块1所有类所在的包为：
```
com.javacode2018.lesson001.demo24.test3.module1
```
<a name="B4jPK"></a>
#### 组件1：Module1Service1
```java
package com.javacode2018.lesson001.demo24.test3.module1;

import org.springframework.stereotype.Component;

@Component
public class Module1Service1 {
}
```
<a name="gZk4L"></a>
#### 组件2：Module1Service2
```java
package com.javacode2018.lesson001.demo24.test3.module1;

import org.springframework.stereotype.Component;

@Component
public class Module1Service2 {
}
```
<a name="U1xGy"></a>
#### 组件扫描类：CompontentScanModule1
负责扫描当前模块中的组件
```java
package com.javacode2018.lesson001.demo24.test3.module1;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.stereotype.Component;

/**
 * 模块1的主键扫描
 */
@ComponentScan
public class CompontentScanModule1 {
}
```
<a name="NgBSq"></a>
### 同样的方式定义模块2
2个组件和一个组件扫描类，模块1所有类所在的包为：
```
com.javacode2018.lesson001.demo24.test3.module2
```
<a name="Q0H57"></a>
#### 组件1：Module2Service1
```java
package com.javacode2018.lesson001.demo24.test3.module2;

import org.springframework.stereotype.Component;

@Component
public class Module2Service1 {
}
```
<a name="RLEz7"></a>
#### 组件2：Module2Service2
```java
package com.javacode2018.lesson001.demo24.test3.module2;

import org.springframework.stereotype.Component;

@Component
public class Module2Service2 {
}
```
<a name="Ofvu8"></a>
#### 组件扫描类：CompontentScanModule1
负责扫描当前模块中的组件
```java
package com.javacode2018.lesson001.demo24.test3.module2;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.stereotype.Component;

/**
 * 模块2的组件扫描
 */
@ComponentScan
public class CompontentScanModule2 {
}
```
<a name="hltMe"></a>
### 总配置类：通过`@Import`导入每个模块中的组件扫描类
```java
package com.javacode2018.lesson001.demo24.test3;

import com.javacode2018.lesson001.demo24.test3.module1.CompontentScanModule1;
import com.javacode2018.lesson001.demo24.test3.module2.CompontentScanModule2;
import org.springframework.context.annotation.Import;

/**
 * 通过@Import导入多个@CompontentScan标注的配置类
 */
@Import({CompontentScanModule1.class, CompontentScanModule2.class}) //@1
public class MainConfig3 {
}
```
@1导入了2个模块中的组件扫描类，可以按需导入。
<a name="ArwIW"></a>
### 测试用例
ImportTest中新增个方法
```java
@Test
public void test3() {
    //1.通过AnnotationConfigApplicationContext创建spring容器，参数为@Import标注的类
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig3.class);
    //2.输出容器中定义的所有bean信息
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="umFuF"></a>
### 运行输出
部分输出如下：
```
module1Service1->com.javacode2018.lesson001.demo24.test3.module1.Module1Service1@5b239d7d
module1Service2->com.javacode2018.lesson001.demo24.test3.module1.Module1Service2@6572421
module2Service1->com.javacode2018.lesson001.demo24.test3.module2.Module2Service1@6b81ce95
module2Service2->com.javacode2018.lesson001.demo24.test3.module2.Module2Service2@2a798d51
```
两个模块中通过`@Compontent`定义的4个bean都输出了。<br />如果只想注册模块1中的bean，只需要修改一下`@Import`，去掉CompontentScanModule2，如下：
```java
@Import({CompontentScanModule1.class})
```
再次运行输出：
```
module1Service1->com.javacode2018.lesson001.demo24.test3.module1.Module1Service1@6379eb
module1Service2->com.javacode2018.lesson001.demo24.test3.module1.Module1Service2@294425a7
```
此时模块2的bean就没有了。
<a name="VCMZ9"></a>
## 先来了解一下相关的几个接口
<a name="l8gk4"></a>
### `ImportBeanDefinitionRegistrar`接口
**这个接口提供了通过Spring容器api的方式直接向容器中注册bean**。<br />接口的完整名称：
```java
org.springframework.context.annotation.ImportBeanDefinitionRegistrar
```
源码：
```java
public interface ImportBeanDefinitionRegistrar {

    default void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry,
                                         BeanNameGenerator importBeanNameGenerator) {

        registerBeanDefinitions(importingClassMetadata, registry);
    }

    default void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
    }

}
```
2个默认方法，都可以用来调用Spring容器api来注册bean。
<a name="OvEfp"></a>
#### 2个方法中主要有3个参数
<a name="zC1yS"></a>
##### `importingClassMetadata`
`AnnotationMetadata`类型的，通过这个可以获取被`@Import`注解标注的类所有注解的信息。
<a name="U1E3F"></a>
##### `registry`
`BeanDefinitionRegistry`类型，是一个接口，内部提供了注册bean的各种方法。
<a name="LbuIv"></a>
##### `importBeanNameGenerator`
`BeanNameGenerator`类型，是一个接口，内部有一个方法，用来生成bean的名称。<br />关于`BeanDefinitionRegistry`和`BeanNameGenerator`接口在来细说一下。
<a name="wj9CY"></a>
### `BeanDefinitionRegistry`接口：bean定义注册器
bean定义注册器，提供了bean注册的各种方法，来看一下源码：
```java
public interface BeanDefinitionRegistry extends AliasRegistry {

    /**
     * 注册一个新的bean定义
     * beanName：bean的名称
     * beanDefinition：bean定义信息
     */
    void registerBeanDefinition(String beanName, BeanDefinition beanDefinition)
    throws BeanDefinitionStoreException;

    /**
     * 通过bean名称移除已注册的bean
     * beanName：bean名称
     */
    void removeBeanDefinition(String beanName) throws NoSuchBeanDefinitionException;

    /**
     * 通过名称获取bean的定义信息
     * beanName：bean名称
     */
    BeanDefinition getBeanDefinition(String beanName) throws NoSuchBeanDefinitionException;

    /**
     * 查看beanName是否注册过
     */
    boolean containsBeanDefinition(String beanName);

    /**
     * 获取已经定义（注册）的bean名称列表
     */
    String[] getBeanDefinitionNames();

    /**
     * 返回注册器中已注册的bean数量
     */
    int getBeanDefinitionCount();

    /**
     * 确定给定的bean名称或者别名是否已在此注册表中使用
     * beanName：可以是bean名称或者bean的别名
     */
    boolean isBeanNameInUse(String beanName);

}
```
基本上所有bean工厂都实现了这个接口，让bean工厂拥有bean注册的各种能力。<br />上面用到的`AnnotationConfigApplicationContext`类也实现了这个接口。
<a name="b78va"></a>
### `BeanNameGenerator`接口：bean名称生成器
bean名称生成器，这个接口只有一个方法，用来生成bean的名称：
```java
public interface BeanNameGenerator {
    String generateBeanName(BeanDefinition definition, BeanDefinitionRegistry registry);
}
```
Spring内置了3个实现
<a name="T9FXl"></a>
#### `DefaultBeanNameGenerator`
默认bean名称生成器，xml中bean未指定名称的时候，默认就会使用这个生成器，默认为：完整的类名#bean编号
<a name="aqhaB"></a>
#### `AnnotationBeanNameGenerator`
注解方式的bean名称生成器，比如通过`@Component(bean名称)`的方式指定bean名称，如果没有通过注解方式指定名称，默认会将完整的类名作为bean名称。
<a name="TbcN6"></a>
#### `FullyQualifiedAnnotationBeanNameGenerator`
将完整的类名作为bean的名称
<a name="Hco6K"></a>
### `BeanDefinition`接口：bean定义信息
用来表示bean定义信息的接口，向容器中注册bean之前，会通过xml或者其他方式定义bean的各种配置信息，bean的所有配置信息都会被转换为一个`BeanDefinition`对象，然后通过容器中`BeanDefinitionRegistry`接口中的方法，将`BeanDefinition`注册到Spring容器中，完成bean的注册操作。<br />这个接口有很多实现类，有兴趣的可以去看看源码，`BeanDefinition`的各种用法。
<a name="N230l"></a>
## value为`ImportBeanDefinitionRegistrar`接口类型
<a name="xf2rG"></a>
### 用法（4个步骤）

1. 定义`ImportBeanDefinitionRegistrar`接口实现类，在`registerBeanDefinitions`方法中使用`registry`来注册bean
2. 使用`@Import`来导入步骤1中定义的类
3. 使用步骤2中`@Import`标注的类作为`AnnotationConfigApplicationContext`构造参数创建Spring容器
4. 使用`AnnotationConfigApplicationContext`操作bean
<a name="jfSFi"></a>
### 案例
来2个普通的类。
<a name="V9yxO"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo24.test4;

public class Service1 {
}
```
<a name="xVR72"></a>
#### Service2
这个类中需要注入Service1
```java
package com.javacode2018.lesson001.demo24.test4;

public class Service2 {
    private Service1 service1;

    public Service1 getService1() {
        return service1;
    }

    public void setService1(Service1 service1) {
        this.service1 = service1;
    }

    @Override
    public String toString() {
        return "Service2{" +
        "service1=" + service1 +
        '}';
    }
}
```
来个类实现`ImportBeanDefinitionRegistrar`接口，然后在里面实现上面2个类的注册，如下：
<a name="thLq0"></a>
#### `MyImportBeanDefinitionRegistrar`
```java
package com.javacode2018.lesson001.demo24.test4;

import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.BeanDefinitionRegistry;
import org.springframework.context.annotation.ImportBeanDefinitionRegistrar;
import org.springframework.core.type.AnnotationMetadata;


public class MyImportBeanDefinitionRegistrar implements ImportBeanDefinitionRegistrar {
    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
        //定义一个bean：Service1
        BeanDefinition service1BeanDinition = BeanDefinitionBuilder.genericBeanDefinition(Service1.class).getBeanDefinition();
        //注册bean
        registry.registerBeanDefinition("service1", service1BeanDinition);

        //定义一个bean：Service2，通过addPropertyReference注入service1
        BeanDefinition service2BeanDinition = BeanDefinitionBuilder.genericBeanDefinition(Service2.class).
        addPropertyReference("service1", "service1").
        getBeanDefinition();
        //注册bean
        registry.registerBeanDefinition("service2", service2BeanDinition);
    }
}
```
注意上面的`registerBeanDefinitions`方法，内部注册了2个bean，Service1和Service2。<br />上面使用了`BeanDefinitionBuilder`这个类，这个是BeanDefinition的构造器，内部提供了很多静态方法方便构建`BeanDefinition`对象。<br />上面定义的2个bean，和下面xml方式效果一样：
```xml
<bean id="service1" class="com.javacode2018.lesson001.demo24.test4.Service1" />
<bean id="service2" class="com.javacode2018.lesson001.demo24.test4.Service2">
  <property name="service1" ref="service1"/>
</bean>
```
<a name="cuZSB"></a>
#### 来个测试用例
ImportTest中新增个方法
```java
@Test
public void test4() {
    //1.通过AnnotationConfigApplicationContext创建spring容器，参数为@Import标注的类
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig4.class);
    //2.输出容器中定义的所有bean信息
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="oM3RI"></a>
#### 运行输出
```
service1->com.javacode2018.lesson001.demo24.test4.Service1@62150f9e
service2->Service2{service1=com.javacode2018.lesson001.demo24.test4.Service1@62150f9e}
```
<a name="zKnor"></a>
## value为`ImportSelector`接口类型
先来看一下`ImportSelector`接口
<a name="wMOnP"></a>
### `ImportSelector`接口
导入选择器，看一下源码：
```java
public interface ImportSelector {

    /**
     * 返回需要导入的类名的数组，可以是任何普通类，配置类（@Configuration、@Bean、@CompontentScan等标注的类）
     * @importingClassMetadata：用来获取被@Import标注的类上面所有的注解信息
     */
    String[] selectImports(AnnotationMetadata importingClassMetadata);

}
```
<a name="DicOU"></a>
### 用法（4个步骤）

1. 定义`ImportSelector`接口实现类，在`selectImports`返回需要导入的类的名称数组
2. 使用`@Import`来导入步骤1中定义的类
3. 使用步骤2中`@Import`标注的类作为`AnnotationConfigApplicationContext`构造参数创建Spring容器
4. 使用`AnnotationConfigApplicationContext`操作bean
<a name="A5Qq0"></a>
### 案例
<a name="K3iQx"></a>
#### 来个普通类：Service1
```java
package com.javacode2018.lesson001.demo24.test5;

public class Service1 {
}
```
<a name="ZD24s"></a>
#### 来个`@Configuration`标注的配置类：Module1Config
```java
package com.javacode2018.lesson001.demo24.test5;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Module1Config {
    @Bean
    public String name() {
        return "java";
    }

    @Bean
    public String address() {
        return "上海市";
    }
}
```
上面定义了两个string类型的bean：name和address
<a name="rhsSg"></a>
#### 下面自定义一个`ImportSelector`，然后返回上面2个类的名称
```java
package com.javacode2018.lesson001.demo24.test5;

import org.springframework.context.annotation.ImportSelector;
import org.springframework.core.type.AnnotationMetadata;

public class MyImportSelector implements ImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{
            Service1.class.getName(),
            Module1Config.class.getName()
        };
    }
}
```
<a name="Y8txv"></a>
#### 来个`@Import`标注的类，导入MyImportSelector
```java
package com.javacode2018.lesson001.demo24.test5;

import com.javacode2018.lesson001.demo24.test4.MyImportBeanDefinitionRegistrar;
import org.springframework.context.annotation.Import;

/**
 * 通过@Import导入MyImportSelector接口实现类
 */
@Import({MyImportSelector.class})
public class MainConfig5 {
}
```
<a name="JdfX2"></a>
#### 新增测试用例
ImportTest中新增个方法
```java
@Test
public void test5() {
    //1.通过AnnotationConfigApplicationContext创建spring容器，参数为@Import标注的类
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig5.class);
    //2.输出容器中定义的所有bean信息
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="y34vu"></a>
#### 运行输出
部分输出如下：
```
com.javacode2018.lesson001.demo24.test5.Service1->com.javacode2018.lesson001.demo24.test5.Service1@45b4c3a9
name->java
address->上海市
```
输出中可以看到Service1以及Module1Config中定义的2个bean都有了。
<a name="F77TH"></a>
## 来一个牛逼的案例
<a name="rcYEe"></a>
### 需求
凡是类名中包含service的，调用他们内部任何方法，希望调用之后能够输出这些方法的耗时。
<a name="HoMPT"></a>
### 实现分析
此处就可以通过代理来实现，bean实例创建的过程中，可以给这些bean生成一个代理，在代理中统计方法的耗时，这里面有2点：

1. 创建一个代理类，通过代理来间接访问需要统计耗时的bean对象
2. 拦截bean的创建，给bean实例生成代理生成代理
<a name="SAvqe"></a>
### 具体实现
<a name="UXygr"></a>
#### 先来两个Service类
<a name="SI6E3"></a>
##### Service1
```java
package com.javacode2018.lesson001.demo24.test6;

import org.springframework.stereotype.Component;

@Component
public class Service1 {
    public void m1() {
        System.out.println(this.getClass() + ".m1()");
    }
}
```
<a name="jYMgL"></a>
##### Service2
```java
package com.javacode2018.lesson001.demo24.test6;

import org.springframework.stereotype.Component;

@Component
public class Service2 {
    public void m1() {
        System.out.println(this.getClass() + ".m1()");
    }
}
```
<a name="gBRgS"></a>
#### 创建统计耗时的代理类
下面使用cglib来实现一个代理类，如下：
```java
package com.javacode2018.lesson001.demo24.test6;


import org.springframework.cglib.proxy.Enhancer;
import org.springframework.cglib.proxy.MethodInterceptor;
import org.springframework.cglib.proxy.MethodProxy;

import java.lang.reflect.Method;

public class CostTimeProxy implements MethodInterceptor {
    //目标对象
    private Object target;

    public CostTimeProxy(Object target) {
        this.target = target;
    }

    @Override
    public Object intercept(Object o, Method method, Object[] objects, MethodProxy methodProxy) throws Throwable {
        long starTime = System.nanoTime();
        //调用被代理对象（即target）的方法，获取结果
        Object result = method.invoke(target, objects); //@1
        long endTime = System.nanoTime();
        System.out.println(method + "，耗时(纳秒)：" + (endTime - starTime));
        return result;
    }

    /**
     * 创建任意类的代理对象
     *
     * @param target
     * @param <T>
     * @return
     */
    public static <T> T createProxy(T target) {
        CostTimeProxy costTimeProxy = new CostTimeProxy(target);
        Enhancer enhancer = new Enhancer();
        enhancer.setCallback(costTimeProxy);
        enhancer.setSuperclass(target.getClass());
        return (T) enhancer.create();
    }
}
```
`createProxy`方法可以用来给某个对象生成代理对象
<a name="l0qfQ"></a>
#### 拦截bean实例的创建，返回代理对象
这里需要用到Spring中的一个接口：
```java
org.springframework.beans.factory.config.BeanPostProcessor

public interface BeanPostProcessor {

    /**
     * bean初始化之后会调用的方法
     */
    @Nullable
    default Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
        return bean;
    }

    /**
     * bean初始化之后会调用的方法
     */
    @Nullable
    default Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        return bean;
    }

}
```
这个接口是bean处理器，内部有2个方法，分别在bean初始化前后会进行调用，这里只需要知道bean初始化之后会调用`postProcessAfterInitialization`方法就行，这个方法中会给bean创建一个代理对象。<br />下面创建一个`BeanPostProcessor`实现类：
```java
package com.javacode2018.lesson001.demo24.test6;

import com.javacode2018.lesson001.demo23.test4.CostTimeProxy;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.lang.Nullable;

public class MethodCostTimeProxyBeanPostProcessor implements BeanPostProcessor {
    @Nullable
    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        if (bean.getClass().getName().toLowerCase().contains("service")) {
            return CostTimeProxy.createProxy(bean); //@1
        } else {
            return bean;
        }
    }
}
```
@1：使用上面创建代理类来给当前bean对象创建一个代理<br />需要将`MethodCostTimeProxyBeanPostProcessor`注册到容器中才会起作用，下面通过`@Import`结合`ImportSelector`的方式来导入这个类，将其注册到容器中。
<a name="SHL74"></a>
#### `MethodCostTimeImportSelector`
```java
package com.javacode2018.lesson001.demo24.test6;


import com.javacode2018.lesson001.demo23.test4.MethodCostTimeProxyBeanPostProcessor;
import org.springframework.context.annotation.ImportSelector;
import org.springframework.core.type.AnnotationMetadata;

public class MethodCostTimeImportSelector implements ImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{MethodCostTimeProxyBeanPostProcessor.class.getName()};
    }
}
```
<a name="YBDKO"></a>
#### 来一个`@Import`来导入`MethodCostTimeImportSelector`
下面使用注解的方式，在注解上使用`@Import`，如下：
```java
package com.javacode2018.lesson001.demo24.test6;


import com.javacode2018.lesson001.demo23.test4.MethodCostTimeImportSelector;
import org.springframework.context.annotation.Import;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Import(MethodCostTimeImportSelector.class)
public @interface EnableMethodCostTime {
}
```
<a name="OcI8W"></a>
#### 来一个总的配置类
```java
package com.javacode2018.lesson001.demo24.test6;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
@EnableMethodCostTime //@1
public class MainConfig6 {
}
```
上面使用了`@CompontentScan`注解，此时会将Servce1和Service2这两个类注册到容器中。<br />@1：此处使用了`@EnableMethodCostTime`注解，而`@EnableMethodCostTime`注解上使用了`@Import(MethodCostTimeImportSelector.class)`，此时`MethodCostTimeImportSelector`类中的`MethodCostTimeProxyBeanPostProcessor`会被注册到容器，会拦截bean的创建，创建耗时代理对象。
<a name="KS2cy"></a>
#### 来个测试用例
ImportTest中新增个方法
```java
@Test
public void test6() {
    //1.通过AnnotationConfigApplicationContext创建spring容器，参数为@Import标注的类
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig6.class);
    Service1 service1 = context.getBean(Service1.class);
    Service2 service2 = context.getBean(Service2.class);
    service1.m1();
    service2.m1();
}
```
上面会调用service1和service2的方法
<a name="c98Ms"></a>
#### 运行输出
```
class com.javacode2018.lesson001.demo24.test6.Service1.m1()
public void com.javacode2018.lesson001.demo24.test6.Service1.m1()，耗时(纳秒)：74200
class com.javacode2018.lesson001.demo24.test6.Service2.m1()
public void com.javacode2018.lesson001.demo24.test6.Service2.m1()，耗时(纳秒)：33800
```
**太牛逼了，需求实现了。**<br />**如果不想开启方法耗时统计，只需要将MainConfig6上的**`**@EnableMethodCostTime**`**去掉就可以了，用起来是不是特别爽。**<br />**Spring****中有很多类似的注解，以**`**@EnableXXX**`**开头的注解，基本上都是通过上面这种方式实现的，如：**
```java
@EnableAspectJAutoProxy
@EnableCaching
@EnableAsync
```
继续向下看，还有一个更牛逼的接口`DeferredImportSelector`。
<a name="m5rme"></a>
## `DeferredImportSelector`接口
**SpringBoot中的核心功能**`**@EnableAutoConfiguration**`**就是靠**`**DeferredImportSelector**`**来实现的。**<br />`DeferredImportSelector`是`ImportSelector`的子接口，既然是`ImportSelector`的子接口，所以也可以通过`@Import`进行导入，这个接口和`ImportSelector`不同地方有两点：

1. 延迟导入
2. 指定导入的类的处理顺序
<a name="sRA1H"></a>
### 延迟导入
比如`@Import`的value包含了多个普通类、多个`@Configuration`标注的配置类、多个`ImportSelector`接口的实现类，多个`ImportBeanDefinitionRegistrar`接口的实现类，还有`DeferredImportSelector`接口实现类，此时Spring处理这些被导入的类的时候，**会将**`**DeferredImportSelector**`**类型的放在最后处理，会先处理其他被导入的类，其他类会按照value所在的前后顺序进行处理**。<br />那么是可以做很多事情的，比如可以在`DeferredImportSelector`导入的类中判断一下容器中是否已经注册了某个bean，如果没有注册过，那么再来注册。<br />`@Conditional`这个注解可以按条件来注册bean，比如可以判断某个bean不存在的时候才进行注册，某个类存在的时候才进行注册等等各种条件判断，通过`@Conditional`来结合`DeferredImportSelector`可以做很多事情。
<a name="WZmaw"></a>
### 来个延迟导入的案例
来3个配置类，每个配置类中都通过`@Bean`定一个string类型的bean，内部输出一句文字。
<a name="bXQ4P"></a>
#### Configuration1
```java
package com.javacode2018.lesson001.demo24.test7;


import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Configuration1 {
    @Bean
    public String name1() {
        System.out.println("name1");
        return "name1";
    }
}
```
<a name="N5vPU"></a>
#### Configuration2
```java
package com.javacode2018.lesson001.demo24.test7;


import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Configuration2 {
    @Bean
    public String name2() {
        System.out.println("name2");
        return "name2";
    }
}
```
<a name="VYP83"></a>
#### Configuration3
```java
package com.javacode2018.lesson001.demo24.test7;


import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Configuration3 {
    @Bean
    public String name3() {
        System.out.println("name3");
        return "name3";
    }
}
```
<a name="BuZlf"></a>
#### 来一个`ImportSelector`实现类，导入Configuration1
```java
package com.javacode2018.lesson001.demo24.test7;

import org.springframework.context.annotation.ImportSelector;
import org.springframework.core.type.AnnotationMetadata;

public class ImportSelector1 implements ImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{
            Configuration1.class.getName()
        };
    }
}
```
<a name="knZSM"></a>
#### 来一个`DeferredImportSelector`实现类，导入Configuration2
```java
package com.javacode2018.lesson001.demo24.test7;

import org.springframework.context.annotation.DeferredImportSelector;
import org.springframework.core.type.AnnotationMetadata;

public class DeferredImportSelector1 implements DeferredImportSelector {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{Configuration2.class.getName()};
    }
}
```
<a name="U3gok"></a>
#### 来一个总的配置类
```java
package com.javacode2018.lesson001.demo24.test7;

import org.springframework.context.annotation.Import;

@Import({
    DeferredImportSelector1.class,
    Configuration3.class,
    ImportSelector1.class,
})
public class MainConfig7 {
}
```
注意上面的`@Import`中被导入类的顺序：<br />DeferredImportSelector1->Configuration3->ImportSelector1<br />下面来个测试用例，看一下3个配置文件中@Bean标注的方法被执行的先后顺序。
<a name="XiRuG"></a>
#### 测试用例
ImportTest中新增个方法
```java
@Test
public void test7() {
    //1.通过AnnotationConfigApplicationContext创建spring容器，参数为@Import标注的类
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig7.class);
}
```
<a name="h251I"></a>
#### 运行输出
```
name3
name1
name2
```
**输出的结果结合一下**`**@Import**`**中被导入的3个类的顺序，可以看出DeferredImportSelector1是被最后处理的，其他2个是按照value中所在的先后顺序处理的。**
<a name="HS3aS"></a>
### 指定导入的类的处理顺序
当`@Import`中有多个`DeferredImportSelector`接口的实现类时候，可以指定他们的顺序，指定顺序常见2种方式
<a name="AfVwO"></a>
#### 实现`Ordered`接口的方式
```java
org.springframework.core.Ordered

public interface Ordered {

    int HIGHEST_PRECEDENCE = Integer.MIN_VALUE;

    int LOWEST_PRECEDENCE = Integer.MAX_VALUE;

    int getOrder();

}
```
value的值越小，优先级越高。
<a name="qqgPv"></a>
#### 实现Order注解的方式
```java
org.springframework.core.annotation.Order

@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE, ElementType.METHOD, ElementType.FIELD})
@Documented
public @interface Order {

    int value() default Ordered.LOWEST_PRECEDENCE;

}
```
value的值越小，优先级越高。<br />下面来个案例感受一下。
<a name="sT51Q"></a>
### 来个指定导入类处理顺序的案例
来2个配置类，内部都有一个`@Bean`标注的方法，用来注册一个bean，方法内部输出一行文字
<a name="Krm5j"></a>
#### Configuration1
```java
package com.javacode2018.lesson001.demo24.test8;


import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Configuration1 {
    @Bean
    public String name1() {
        System.out.println("name1");
        return "name1";
    }
}
```
<a name="AMsWj"></a>
#### Configuration2
```java
package com.javacode2018.lesson001.demo24.test8;


import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Configuration2 {
    @Bean
    public String name2() {
        System.out.println("name2");
        return "name2";
    }
}
```
来2个`DeferredImportSelector`实现类，分别来导入上面2个配置文件，顺便通过Ordered接口指定一下顺序
<a name="ElZ5Y"></a>
#### DeferredImportSelector1
```java
package com.javacode2018.lesson001.demo24.test8;

import org.springframework.context.annotation.DeferredImportSelector;
import org.springframework.core.Ordered;
import org.springframework.core.type.AnnotationMetadata;

public class DeferredImportSelector1 implements DeferredImportSelector, Ordered {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{Configuration1.class.getName()};
    }

    @Override
    public int getOrder() {
        return 2;
    }
}
```
<a name="KcH2o"></a>
#### DeferredImportSelector2
```java
package com.javacode2018.lesson001.demo24.test8;

import com.javacode2018.lesson001.demo24.test7.Configuration2;
import org.springframework.context.annotation.DeferredImportSelector;
import org.springframework.core.Ordered;
import org.springframework.core.type.AnnotationMetadata;

public class DeferredImportSelector2 implements DeferredImportSelector, Ordered {
    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[]{Configuration2.class.getName()};
    }

    @Override
    public int getOrder() {
        return 1;
    }
}
```
**DeferredImportSelector1的order为2，DeferredImportSelector2的order为1，order值越小优先级越高。**<br />来个总的配置类，引入上面两个`ImportSelector`
<a name="Psa5f"></a>
#### MainConfig8
```java
package com.javacode2018.lesson001.demo24.test8;


import org.springframework.context.annotation.Import;

@Import({
    DeferredImportSelector1.class,
    DeferredImportSelector2.class,
})
public class MainConfig8 {
}
```
<a name="nMwFu"></a>
#### 测试用例
ImportTest中新增个方法
```java
@Test
public void test8() {
    //1.通过AnnotationConfigApplicationContext创建spring容器，参数为@Import标注的类
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig8.class);
}
```
<a name="Z7Gd7"></a>
#### 运行输出
```
name2
name1
```
**结果配合order的值，按照order从小到大来处理，可以看出DeferredImportSelector2先被处理的。**
<a name="d0dHs"></a>
## Spring中这块的源码
`**@Import**`**注解是被下面这个类处理的**
```
org.springframework.context.annotation.ConfigurationClassPostProcessor
```
`@Configuration`、`@Bean`、`@CompontentScan`、`@CompontentScan`都是被这个类处理的，这个类是高手必经之路，建议花点时间研究研究。
<a name="zVSiH"></a>
## 总结

1. `**@Import**`**可以用来批量导入任何普通的组件、配置类，将这些类中定义的所有bean注册到容器中**
2. `**@Import**`**常见的5种用法需要掌握**
3. **掌握**`**ImportSelector**`**、**`**ImportBeanDefinitionRegistrar**`**、**`**DeferredImportSelector**`**的用法**
4. `**DeferredImportSelector**`**接口可以实现延迟导入、按序导入的功能**
5. **Spring中很多以**`**@Enable**`**开头的都是使用**`**@Import**`**集合**`**ImportSelector**`**方式实现的**
6. `**BeanDefinitionRegistry**`**接口：bean定义注册器，这个需要掌握常见的方法**
