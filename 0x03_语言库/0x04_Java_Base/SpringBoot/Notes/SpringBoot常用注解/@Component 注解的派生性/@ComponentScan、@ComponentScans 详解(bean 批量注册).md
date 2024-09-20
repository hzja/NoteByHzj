JavaSpringBoot
<a name="ckEkl"></a>
## `@ComponentScan`
`@ComponentScan`用于批量注册bean。<br />这个注解会让Spring去扫描某些包及其子包中所有的类，然后将满足一定条件的类作为bean注册到Spring容器容器中。<br />具体需要扫描哪些包？以及这些包中的类满足什么条件时被注册到容器中，这些都可以通过这个注解中的参数动态配置。<br />先来看一下这个注解的定义：
```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
@Documented
@Repeatable(ComponentScans.class) //@1
public @interface ComponentScan {

    @AliasFor("basePackages")
    String[] value() default {};

    @AliasFor("value")
    String[] basePackages() default {};

    Class<?>[] basePackageClasses() default {};

    Class<? extends BeanNameGenerator> nameGenerator() default BeanNameGenerator.class;

    Class<? extends ScopeMetadataResolver> scopeResolver() default AnnotationScopeMetadataResolver.class;

    ScopedProxyMode scopedProxy() default ScopedProxyMode.DEFAULT;

    String resourcePattern() default "**/*.class";

    boolean useDefaultFilters() default true;

    Filter[] includeFilters() default {};

    Filter[] excludeFilters() default {};

    boolean lazyInit() default false;
}
```
定义上可以看出此注解可以用在任何类型上面，不过通常将其用在类上面。<br />常用参数：<br />`value`：指定需要扫描的包，如：com.javacode2018<br />`basePackages`：作用同value；value和`basePackages`不能同时存在设置，可二选一<br />`basePackageClasses`：指定一些类，Spring容器会扫描这些类所在的包及其子包中的类<br />`nameGenerator`：自定义bean名称生成器<br />`resourcePattern`：需要扫描包中的那些资源，默认是：`**/*.class`，即会扫描指定包中所有的class文件<br />`useDefaultFilters`：对扫描的类是否启用默认过滤器，默认为true<br />`includeFilters`：过滤器：用来配置被扫描出来的那些类会被作为组件注册到容器中<br />`excludeFilters`：过滤器，和`includeFilters`作用刚好相反，用来对扫描的类进行排除的，被排除的类不会被注册到容器中<br />`lazyInit`：是否延迟初始化被注册的bean<br />@1：`@Repeatable(ComponentScans.class)`，这个注解可以同时使用多个。<br />`**@ComponentScan**`**工作的过程：**

1. **Spring会扫描指定的包，且会递归下面子包，得到一批类的数组**
2. **然后这些类会经过上面的各种过滤器，最后剩下的类会被注册到容器中**

**所以玩这个注解，主要关注2个问题：**<br />**第一个：需要扫描哪些包？通过**`**value**`**、**`**backPackages**`**、**`**basePackageClasses**`**这3个参数来控制**<br />**第二：过滤器有哪些？通过**`**useDefaultFilters**`**、**`**includeFilters**`**、**`**excludeFilters**`**这3个参数来控制过滤器**<br />**这两个问题搞清楚了，就可以确定哪些类会被注册到容器中。**<br />**默认情况下，任何参数都不设置的情况下，此时，会将**`**@ComponentScan**`**修饰的类所在的包作为扫描包；默认情况下**`**useDefaultFilters**`**为true，这个为true的时候，Spring容器内部会使用默认过滤器，规则是：凡是类上有**`**@Repository**`**、**`**@Service**`**、**`**@Controller**`**、**`**@Component**`**这几个注解中的任何一个的，那么这个类就会被作为bean注册到Spring容器中，所以默认情况下，只需在类上加上这几个注解中的任何一个，这些类就会自动交给Spring容器来管理了。**
<a name="vL3Hl"></a>
## `@Component`、`@Repository`、`@Service`、`@Controller`
这几个注解都是Spring提供的。<br />先说一下`@Component`这个注解，看一下其定义：
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Indexed
public @interface Component {
    String value() default "";
}
```
从定义中可以看出，这个注解可以用在任何类型上面。<br />通常情况下将这个注解用在类上面，标注这个类为一个组件，默认情况下，被扫描的时候会被作为bean注册到容器中。<br />value参数：被注册为bean的时候，用来指定bean的名称，如果不指定，默认为类名首字母小写。如：类UserService对应的beanname为userService<br />再来看看`@Repository`源码如下：
```java
@Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Component
public @interface Repository {

    @AliasFor(annotation = Component.class)
    String value() default "";

}
```
Repository上面有`@Component`注解。<br />value参数上面有`@AliasFor(annotation = Component.class)`，设置value参数的时候，也相当于给`@Component`注解中的value设置值。<br />其他两个注解`@Service`、`@Controller`源码和`@Repository`源码类似。<br />这4个注解本质上是没有任何差别，都可以用在类上面，表示这个类被Spring容器扫描的时候，可以作为一个bean组件注册到Spring容器中。<br />Spring容器中对这4个注解的解析并没有进行区分，统一采用`@Component`注解的方式进行解析，所以这几个注解之间可以相互替换。<br />Spring提供这4个注解，是为了让系统更清晰，通常情况下，系统是分层结构的，多数系统一般分为controller层、service层、dao层。<br />`@Controller`通常用来标注controller层组件，`@Service`注解标注service层的组件，`@Repository`标注dao层的组件，这样可以让整个系统的结构更清晰，当看到这些注解的时候，会和清晰的知道属于哪个层，对于Spring来说，将这3个注解替换成`@Component`注解，对系统没有任何影响，产生的效果是一样的。<br />下面通过案例来感受`@ComponentScan`各种用法。
<a name="cfXwP"></a>
## 案例1：任何参数未设置
<a name="uVnMv"></a>
### UserController
```java
package com.javacode2018.lesson001.demo22.test1.controller;

import org.springframework.stereotype.Controller;

@Controller
public class UserController {
}
```
<a name="JGbma"></a>
### UserService
```java
package com.javacode2018.lesson001.demo22.test1.service;

import org.springframework.stereotype.Service;

@Service
public class UserService {
}
```
<a name="b7Rgx"></a>
### UserDao
```java
package com.javacode2018.lesson001.demo22.test1.dao;

import org.springframework.stereotype.Repository;

@Repository
public class UserDao {
}
```
<a name="Nyb68"></a>
### UserModel
```java
package com.javacode2018.lesson001.demo22.test1;

import org.springframework.stereotype.Component;

@Component
public class UserModel {
}
```
上面几个类中，分别使用了4种注解。
<a name="uWV7q"></a>
### `@CompontentScan`修饰的类
```java
package com.javacode2018.lesson001.demo22.test1;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class ScanBean1 {
}
```
<a name="OeJNR"></a>
### 测试用例
```java
package com.javacode2018.lesson001.demo22;

import com.javacode2018.lesson001.demo22.test1.ScanBean1;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class ComponentScanTest {

    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ScanBean1.class);
        for (String beanName : context.getBeanDefinitionNames()) {
            System.out.println(beanName + "->" + context.getBean(beanName));
        }
    }
}
```
@1：使用`AnnotationConfigApplicationContext`作为ioc容器，将ScanBean作为参数传入。<br />默认会扫描ScanBean类所在的包中的所有类，类上有`@Component`、`@Repository`、`@Service`、`@Controller`任何一个注解的都会被注册到容器中
<a name="VHfBH"></a>
### 运行输出
部分输出如下：
```
userModel->com.javacode2018.lesson001.demo22.test1.UserModel@595b007d
userController->com.javacode2018.lesson001.demo22.test1.controller.UserController@72d1ad2e
userDao->com.javacode2018.lesson001.demo22.test1.dao.UserDao@2d7275fc
userService->com.javacode2018.lesson001.demo22.test1.service.UserService@399f45b1
```
注意最后4行这几个bean，都被注册成功了。
<a name="x1ZWB"></a>
## 案例2：指定需要扫描的包
指定需要扫描哪些包，可以通过value或者basePackage来配置，二者选其一，都配置运行会报错，下面通过value来配置。
<a name="hAivR"></a>
### ScanBean2
```java
package com.javacode2018.lesson001.demo22.test2;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan({
    "com.javacode2018.lesson001.demo22.test1.controller",
    "com.javacode2018.lesson001.demo22.test1.service"
})
public class ScanBean2 {
}
```
上面指定了2需要扫描的包，这两个包中有2个类。
<a name="Bl0qA"></a>
### 测试用例
ComponentScanTest中新增个方法
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ScanBean2.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(beanName + "->" + context.getBean(beanName));
    }
}
```
<a name="BWWwe"></a>
### 运行输出
截取了关键几行如下：
```
userController->com.javacode2018.lesson001.demo22.test1.controller.UserController@dd8ba08
userService->com.javacode2018.lesson001.demo22.test1.service.UserService@245b4bdc
```
可以看出只有controller包和service包中的2个类被注册为bean了。
<a name="jWj6H"></a>
### 注意
**指定包名的方式扫描存在的一个隐患，若包被重名了，会导致扫描会失效，一般情况下使用**`**basePackageClasses**`**的方式来指定需要扫描的包，这个参数可以指定一些类型，默认会扫描这些类所在的包及其子包中所有的类，这种方式可以有效避免这种问题。**<br />下面来看一下`basePackageClasses`的方式。
<a name="qDa7L"></a>
## 案例：`basePackageClasses`指定扫描范围
可以在需要扫描的包中定义一个标记的接口或者类，他们的唯一的作用是作为`basePackageClasses`的值，其他没有任何用途。
<a name="Zl7wN"></a>
### 下面定义这样一个接口
```java
package com.javacode2018.lesson001.demo22.test6.beans;

public interface ScanClass {
}
```
<a name="AmvTR"></a>
### 再来定义2个类，用`@Component`注解标记
```java
package com.javacode2018.lesson001.demo22.test6.beans;

import org.springframework.stereotype.Component;

@Component
public class Service1 {
}
```
```java
package com.javacode2018.lesson001.demo22.test6.beans;

import org.springframework.stereotype.Component;

@Component
public class Service2 {
}
```
<a name="OsC7U"></a>
### 来一个`@CompontentScan`标记的类
```java
package com.javacode2018.lesson001.demo22.test6;

import com.javacode2018.lesson001.demo22.test6.beans.ScanClass;
import org.springframework.context.annotation.ComponentScan;

@ComponentScan(basePackageClasses = ScanClass.class)
public class ScanBean6 {
}
```
<a name="Bemy8"></a>
### 测试用例
ComponentScanTest中新增个方法
```java
@Test
public void test6() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ScanBean6.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(beanName + "->" + context.getBean(beanName));
    }
}
```
<a name="qTBeq"></a>
### 运行输出
```
service1->com.javacode2018.lesson001.demo22.test6.beans.Service1@79924b
service2->com.javacode2018.lesson001.demo22.test6.beans.Service2@7b9a4292
```
<a name="tUC32"></a>
## `includeFilters`的使用
<a name="l5aT6"></a>
### 用法
再来看一下`includeFilters`这个参数的定义：
```java
Filter[] includeFilters() default {};
```
是一个`Filter`类型的数组，**多个Filter之间为或者关系，即满足任意一个就可以了**，看一下`Filter`的代码：
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({})
@interface Filter {

    FilterType type() default FilterType.ANNOTATION;

    @AliasFor("classes")
    Class<?>[] value() default {};

    @AliasFor("value")
    Class<?>[] classes() default {};

    String[] pattern() default {};

}
```
可以看出Filter也是一个注解，参数：<br />`**type**`**：过滤器的类型，是个枚举类型，5种类型**

1. `ANNOTATION`：通过注解的方式来筛选候选者，即判断候选者是否有指定的注解
2. `ASSIGNABLE_TYPE`：通过指定的类型来筛选候选者，即判断候选者是否是指定的类型
3. `ASPECTJ`：ASPECTJ表达式方式，即判断候选者是否匹配ASPECTJ表达式
4. `REGEX`：正则表达式方式，即判断候选者的完整名称是否和正则表达式匹配
5. `CUSTOM`：用户自定义过滤器来筛选候选者，对候选者的筛选交给用户自己来判断

`**value**`**：和参数classes效果一样，二选一**<br />`**classes**`**：3种情况如下**

1. 当`type=FilterType.ANNOTATION`时，通过classes参数可以指定一些注解，用来判断被扫描的类上是否有classes参数指定的注解
2. 当`type=FilterType.ASSIGNABLE_TYPE`时，通过classes参数可以指定一些类型，用来判断被扫描的类是否是classes参数指定的类型
3. 当`type=FilterType.CUSTOM`时，表示这个过滤器是用户自定义的，classes参数就是用来指定用户自定义的过滤器，自定义的过滤器需要实现`org.springframework.core.type.filter.TypeFilter`接口

`**pattern**`**：2种情况如下**

1. 当`type=FilterType.ASPECTJ`时，通过pattern来指定需要匹配的ASPECTJ表达式的值
2. 当`type=FilterType.REGEX`时，通过pattern来自正则表达式的值
<a name="Vfz4M"></a>
### 案例：扫描包含注解的类
<a name="vHCZn"></a>
#### 需求
自定义一个注解，让标注有这些注解的类自动注册到容器中
<a name="HILdS"></a>
#### 代码实现
下面的代码都在com.javacode2018.lesson001.demo22.test3包中。
<a name="oBx1N"></a>
##### 定义一个注解
```java
package com.javacode2018.lesson001.demo22.test3;

import java.lang.annotation.*;

@Documented
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface MyBean {
}
```
<a name="EhUWZ"></a>
##### 创建一个类，使用这个注解标注
```java
package com.javacode2018.lesson001.demo22.test3;

@MyBean
public class Service1 {
}
```
<a name="RrBlm"></a>
##### 再来一个类，使用Spring中的`@Compontent`标注
```java
package com.javacode2018.lesson001.demo22.test3;

import org.springframework.stereotype.Component;

@Component
public class Service2 {
}
```
<a name="iZlro"></a>
##### 再来一个类，使用`@CompontentScan`标注
```java
package com.javacode2018.lesson001.demo22.test3;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.FilterType;

@ComponentScan(includeFilters = {
    @ComponentScan.Filter(type = FilterType.ANNOTATION, classes = MyBean.class)
})
public class ScanBean3 {
}
```
上面指定了Filter的type为注解的类型，只要类上面有`@MyBean`注解的，都会被作为bean注册到容器中。
<a name="KFtSA"></a>
##### 测试用例
ComponentScanTest中新增个测试用例
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ScanBean3.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(beanName + "->" + context.getBean(beanName));
    }
}
```
<a name="J66Bn"></a>
##### 运行输出，截取了主要的几行
```
service1->com.javacode2018.lesson001.demo22.test3.Service1@6b81ce95
service2->com.javacode2018.lesson001.demo22.test3.Service2@2a798d51
```
**Service1上标注了**`**@MyBean**`**注解，被注册到容器了，但是Service2上没有标注**`**@MyBean**`**，怎么也被注册到容器了？**<br />**原因：Service2上标注了**`**@Compontent**`**注解，而**`**@CompontentScan**`**注解中的**`**useDefaultFilters**`**默认是true，表示也会启用默认的过滤器，而默认的过滤器会将标注有**`**@Component**`**、**`**@Repository**`**、**`**@Service**`**、**`**@Controller**`**这几个注解的类也注册到容器中**<br />如果只想将标注有`@MyBean`注解的bean注册到容器，需要将默认过滤器关闭，即：`useDefaultFilters=false`，修改一下ScanBean3的代码如下：
```java
@ComponentScan(
    useDefaultFilters = false, //不启用默认过滤器
    includeFilters = {
        @ComponentScan.Filter(type = FilterType.ANNOTATION, classes = MyBean.class)
    })
public class ScanBean3 {
}
```
再次运行test3输出：
```
service1->com.javacode2018.lesson001.demo22.test3.Service1@294425a7
```
<a name="Y76XP"></a>
#### 扩展：自定义注解支持定义bean名称
上面的自定义的`@MyBean`注解，是无法指定bean的名称的，可以对这个注解做一下改造，加个value参数来指定bean的名称，如下：
```java
@Documented
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Component //@1
public @interface MyBean {
    @AliasFor(annotation = Component.class) //@2
    String value() default ""; //@3
}
```
重点在于@1和@2这2个地方的代码，通过上面的参数可以间接给`@Component`注解中的value设置值。<br />这块用到了`@AliasFor`注解。<br />修改一下Service1的代码：
```java
@MyBean("service1Bean")
public class Service1 {
}
```
运行test3用例输出：
```
service1Bean->com.javacode2018.lesson001.demo22.test3.Service1@222545dc
```
此时bean名称就变成了service1Bean。
<a name="KibQO"></a>
### 案例：包含指定类型的类
下面的代码都位于com.javacode2018.lesson001.demo22.test4包中。
<a name="kTGAP"></a>
#### 来个接口
```java
package com.javacode2018.lesson001.demo22.test4;

public interface IService {
}
```
让Spring来进行扫描，类型满足IService的都将其注册到容器中。
<a name="iQCwB"></a>
#### 来2个实现类
```java
package com.javacode2018.lesson001.demo22.test4;

public class Service1 implements IService {
}
```
```java
package com.javacode2018.lesson001.demo22.test4;

public class Service2 implements IService {
}
```
<a name="OissX"></a>
#### 来一个`@CompontentScan`标注的类
```java
package com.javacode2018.lesson001.demo22.test4;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.FilterType;

@ComponentScan(
    useDefaultFilters = false, //不启用默认过滤器
    includeFilters = {
        @ComponentScan.Filter(type = FilterType.ASSIGNABLE_TYPE, classes = IService.class) //@1
    })
public class ScanBean4 {
}
```
@1：被扫描的类满足`IService.class.isAssignableFrom(被扫描的类)`条件的都会被注册到Spring容器中
<a name="FhOus"></a>
#### 来个测试用例
ComponentScanTest中新增个测试用例
```java
@Test
public void test4() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ScanBean4.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(beanName + "->" + context.getBean(beanName));
    }
}
```
<a name="Qd4Ti"></a>
#### 运行输出
```
service1->com.javacode2018.lesson001.demo22.test4.Service1@6379eb
service2->com.javacode2018.lesson001.demo22.test4.Service2@294425a7
```
<a name="tTzBa"></a>
## 自定义Filter
<a name="LRqVU"></a>
### 用法
有时候需要用到自定义的过滤器，使用自定义过滤器的步骤：

1. 设置`@Filter`中type的类型为：`FilterType.CUSTOM`
2. 自定义过滤器类，需要实现接口：`org.springframework.core.type.filter.TypeFilter`
3. 设置`@Filter`中的classses为自定义的过滤器类型

来看一下`TypeFilter`这个接口的定义：
```java
@FunctionalInterface
public interface TypeFilter {
    boolean match(MetadataReader metadataReader, MetadataReaderFactory metadataReaderFactory)
    throws IOException;
}
```
是一个函数式接口，包含一个match方法，方法返回boolean类型，有2个参数，都是接口类型的，下面介绍一下这2个接口。
<a name="DmC8N"></a>
#### `MetadataReader`接口
**类元数据读取器，可以读取一个类上的任意信息，如类上面的注解信息、类的磁盘路径信息、类的class对象的各种信息，Spring进行了封装，提供了各种方便使用的方法。**<br />看一下这个接口的定义：
```java
public interface MetadataReader {

    /**
     * 返回类文件的资源引用
     */
    Resource getResource();

    /**
     * 返回一个ClassMetadata对象，可以通过这个读想获取类的一些元数据信息，如类的class对象、是否是接口、是否有注解、是否是抽象类、父类名称、接口名称、内部包含的之类列表等等，可以去看一下源码
     */
    ClassMetadata getClassMetadata();

    /**
     * 获取类上所有的注解信息
     */
    AnnotationMetadata getAnnotationMetadata();

}
```
<a name="qM95a"></a>
#### `MetadataReaderFactory`接口
**类元数据读取器工厂，可以通过这个类获取任意一个类的**`**MetadataReader**`**对象。**<br />源码：
```java
public interface MetadataReaderFactory {

    /**
     * 返回给定类名的MetadataReader对象
     */
    MetadataReader getMetadataReader(String className) throws IOException;

    /**
     * 返回指定资源的MetadataReader对象
     */
    MetadataReader getMetadataReader(Resource resource) throws IOException;

}
```
<a name="U5eV8"></a>
### 自定义`Filter`案例
<a name="wdlPD"></a>
#### 需求
来个自定义的Filter，判断被扫描的类如果是IService接口类型的，就让其注册到容器中。
<a name="Rnrgi"></a>
#### 代码实现
来个自定义的`TypeFilter`类：
```java
package com.javacode2018.lesson001.demo22.test5;

import com.javacode2018.lesson001.demo22.test4.IService;
import org.springframework.core.type.ClassMetadata;
import org.springframework.core.type.classreading.MetadataReader;
import org.springframework.core.type.classreading.MetadataReaderFactory;
import org.springframework.core.type.filter.TypeFilter;

import java.io.IOException;

public class MyFilter implements TypeFilter {
    /**
     * @param metadataReader
     * @param metadataReaderFactory
     * @return
     * @throws IOException
     */
    @Override
    public boolean match(MetadataReader metadataReader, MetadataReaderFactory metadataReaderFactory) throws IOException {
        Class curClass = null;
        try {
            //当前被扫描的类
            curClass = Class.forName(metadataReader.getClassMetadata().getClassName());
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        //判断curClass是否是IService类型
        boolean result = IService.class.isAssignableFrom(curClass);
        return result;
    }
}
```
<a name="jKvF8"></a>
#### 来一个`@CompontentScan`标注的类
```java
package com.javacode2018.lesson001.demo22.test5;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.FilterType;

@ComponentScan(
    basePackages = {"com.javacode2018.lesson001.demo22.test4"},
    useDefaultFilters = false, //不启用默认过滤器
    includeFilters = {
        @ComponentScan.Filter(type = FilterType.CUSTOM, classes = MyFilter.class) //@1
    })
public class ScanBean5 {
}
```
@1：type为`FilterType.CUSTOM`，表示Filter是用户自定义的，classes为自定义的过滤器
<a name="wTeUc"></a>
#### 再来个测试用例
ComponentScanTest中新增个测试用例
```java
@Test
public void test5() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(ScanBean5.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(beanName + "->" + context.getBean(beanName));
    }
}
```
<a name="qMZlU"></a>
#### 运行输出
```
service1->com.javacode2018.lesson001.demo22.test4.Service1@4cc451f2
service2->com.javacode2018.lesson001.demo22.test4.Service2@6379eb
```
<a name="nrU3W"></a>
## `excludeFilters`
**配置排除的过滤器，满足这些过滤器的类不会被注册到容器中，用法上面和**`**includeFilters**`**用一样，这个就不演示了，可以自己玩玩**
<a name="FLw84"></a>
## `@ComponentScan`重复使用
从这个注解的定义上可以看出这个注解可以同时使用多个，如：
```java
@ComponentScan(basePackageClasses = ScanClass.class)
@ComponentScan(
    useDefaultFilters = false, //不启用默认过滤器
    includeFilters = {
        @ComponentScan.Filter(type = FilterType.ASSIGNABLE_TYPE, classes = IService.class)
    })
public class ScanBean7 {
}
```
还有一种写法，使用`@ComponentScans`的方式：
```java
@ComponentScans({
    @ComponentScan(basePackageClasses = ScanClass.class),
    @ComponentScan(
        useDefaultFilters = false, //不启用默认过滤器
        includeFilters = {
            @ComponentScan.Filter(type = FilterType.ASSIGNABLE_TYPE, classes = IService.class)
        })})
public class ScanBean7 {
}
```
<a name="y70PD"></a>
## Spring中这块的源码
`**@CompontentScan**`**注解是被下面这个类处理的**
```
org.springframework.context.annotation.ConfigurationClassPostProcessor
```
**这个类非常非常关键，主要用户bean的注册，前面介绍的**`**@Configuration**`**，**`**@Bean**`**注解也是被这个类处理的。**<br />还有下面这些注解：
```java
@PropertySource
@Import
@ImportResource
@Compontent
```
**以上这些注解都是被**`**ConfigurationClassPostProcessor**`**这个类处理的，内部会递归处理这些注解，完成bean的注册。**<br />**以**`**@CompontentScan**`**来说一下过程，第一次扫描之后会得到一批需要注册的类，然后会对这些需要注册的类进行遍历，判断是否有上面任意一个注解，如果有，会将这个类交给**`**ConfigurationClassPostProcessor**`**继续处理，直到递归完成所有bean的注册。**<br />**想成为高手，这个类是必看的。**
<a name="tVpAK"></a>
## 总结

1. `**@ComponentScan**`**用于批量注册bean，Spring会按照这个注解的配置，递归扫描指定包中的所有类，将满足条件的类批量注册到Spring容器中**
2. **可以通过value、**`**basePackages**`**、**`**basePackageClasses**`** 这几个参数来配置包的扫描范围**
3. **可以通过**`**useDefaultFilters**`**、**`**includeFilters**`**、**`**excludeFilters**`**这几个参数来配置类的过滤器，被过滤器处理之后剩下的类会被注册到容器中**
4. **指定包名的方式配置扫描范围存在隐患，包名被重命名之后，会导致扫描实现，所以一般在需要扫描的包中可以创建一个标记的接口或者类，作为**`**basePackageClasses**`**的值，通过这个来控制包的扫描范围**
5. `**@CompontScan**`**注解会被**`**ConfigurationClassPostProcessor**`**类递归处理，最终得到所有需要注册的类。**
