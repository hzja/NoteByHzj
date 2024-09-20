JavaSpringBoot
<a name="HNaF0"></a>
## `@Conditional`注解
`**@Conditional**`**注解是从Spring4.0才有的，可以用在任何类型或者方法上面，通过**`**@Conditional**`**注解可以配置一些条件判断，当所有条件都满足的时候，被**`**@Conditional**`**标注的目标才会被Spring容器处理。**<br />比如可以通过`@Conditional`来控制bean是否需要注册，控制被`@Configuration`标注的配置类是需要需要被解析等。<br />效果就像这段代码，相当于在Spring容器解析目标前面加了一个条件判断：
```java
if(@Conditional中配置的多个条件是否都匹配){
    //Spring继续处理被@Conditional注解标注的对象
}
```
`@Conditional`源码：
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Conditional {
    Class<? extends Condition>[] value();
}
```
这个注解只有一个value参数，`Condition`类型的数组，`Condition`是一个接口，表示一个条件判断，内部有个方法返回true或false，当所有`Condition`都成立的时候，`@Conditional`的结果才成立。<br />下面来看一下`Condition`接口。
<a name="mzxQN"></a>
## `Condition`接口
用来表示条件判断的接口，源码如下：
```java
@FunctionalInterface
public interface Condition {

    /**
     * 判断条件是否匹配
     * context:条件判断上下文
     */
    boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata);

}
```
是一个函数式接口，内部只有一个`matches`方法，用来判断条件是否成立的，2个参数：

- `context`：条件上下文，`ConditionContext`接口类型的，可以用来获取容器中的个人信息
- `metadata`：用来获取被`@Conditional`标注的对象上的所有注解信息
<a name="sWbXw"></a>
## `ConditionContext`接口
这个接口中提供了一些常用的方法，可以用来获取Spring容器中的各种信息，看一下源码：
```java
public interface ConditionContext {

    /**
     * 返回bean定义注册器，可以通过注册器获取bean定义的各种配置信息
     */
    BeanDefinitionRegistry getRegistry();

    /**
     * 返回ConfigurableListableBeanFactory类型的bean工厂，相当于一个ioc容器对象
     */
    @Nullable
    ConfigurableListableBeanFactory getBeanFactory();

    /**
     * 返回当前spring容器的环境配置信息对象
     */
    Environment getEnvironment();

    /**
     * 返回资源加载器
     */
    ResourceLoader getResourceLoader();

    /**
     * 返回类加载器
     */
    @Nullable
    ClassLoader getClassLoader();

}
```
<a name="xuBQr"></a>
## 比较关键性的问题：条件判断在什么时候执行？
Spring对配置类的处理主要分为2个阶段：
<a name="F3ZKB"></a>
### 配置类解析阶段
会得到一批配置类的信息，和一些需要注册的bean
<a name="iYOJe"></a>
### bean注册阶段
将配置类解析阶段得到的配置类和需要注册的bean注册到Spring容器中
<a name="A2lzn"></a>
### 看一下什么是配置类
类中有下面任意注解之一的就属于配置类：

1. 类上有`@Compontent`注解
2. 类上有`@Configuration`注解
3. 类上有`@CompontentScan`注解
4. 类上有`@Import`注解
5. 类上有`@ImportResource`注解
6. 类中有`@Bean`标注的方法

判断一个类是不是一个配置类，是否的是下面这个方法，有兴趣的可以看一下：<br />`org.springframework.context.annotation.ConfigurationClassUtils#isConfigurationCandidate`<br />Spring中处理这2个过程会循环进行，直到完成所有配置类的解析及所有bean的注册。
<a name="ZB6Ey"></a>
### Spring对配置类处理过程
源码位置：
```
org.springframework.context.annotation.ConfigurationClassPostProcessor#processConfigBeanDefinitions
```
整个过程大致的过程如下：

1. 通常会通过`new AnnotationConfigApplicationContext()`传入多个配置类来启动Spring容器
2. Spring对传入的多个配置类进行解析
3. 配置类解析阶段：这个过程就是处理配置类上面6中注解的过程，此过程中又会发现很多新的配置类，比如`@Import`导入的一批新的类刚好也符合配置类，而被`@CompontentScan`扫描到的一些类刚好也是配置类；此时会对这些新产生的配置类进行同样的过程解析
4. bean注册阶段：配置类解析后，会得到一批配置类和一批需要注册的bean，此时Spring容器会将这批配置类作为bean注册到Spring容器，同样也会将这批需要注册的bean注册到Spring容器
5. 经过上面第3个阶段之后，Spring容器中会注册很多新的bean，这些新的bean中可能又有很多新的配置类
6. Spring从容器中将所有bean拿出来，遍历一下，会过滤得到一批未处理的新的配置类，继续交给第3步进行处理
7. step3到step6，这个过程会经历很多次，直到完成所有配置类的解析和bean的注册

从上面过程中可以了解到：

1. 可以在配置类上面加上`@Conditional`注解，来控制是否需要解析这个配置类，配置类如果不被解析，那么这个配置上面6种注解的解析都会被跳过
2. 可以在被注册的bean上面加上`@Conditional`注解，来控制这个bean是否需要注册到Spring容器中
3. 如果配置类不会被注册到容器，那么这个配置类解析所产生的所有新的配置类及所产生的所有新的bean都不会被注册到容器

一个配置类被Spring处理有2个阶段：配置类解析阶段、bean注册阶段（将配置类作为bean被注册到Spring容器)。<br />如果将`Condition`接口的实现类作为配置类上`@Conditional`中，那么这个条件会对两个阶段都有效，此时通过`Condition`是无法精细的控制某个阶段的，如果想控制某个阶段，比如可以让他解析，但是不能让他注册，此时就就需要用到另外一个接口了：`ConfigurationCondition`
<a name="AXAEY"></a>
## `ConfigurationCondition`接口
看一下这个接口的源码：
```java
public interface ConfigurationCondition extends Condition {

    /**
     * 条件判断的阶段，是在解析配置类的时候过滤还是在创建bean的时候过滤
     */
    ConfigurationPhase getConfigurationPhase();


    /**
     * 表示阶段的枚举：2个值
     */
    enum ConfigurationPhase {

        /**
         * 配置类解析阶段，如果条件为false，配置类将不会被解析
         */
        PARSE_CONFIGURATION,

        /**
         * bean注册阶段，如果为false，bean将不会被注册
         */
        REGISTER_BEAN
    }

}
```
`ConfigurationCondition`接口相对于`Condition`接口多了一个`getConfigurationPhase`方法，用来指定条件判断的阶段，是在解析配置类的时候过滤还是在创建bean的时候过滤。
<a name="vY2Vg"></a>
## `@Conditional`使用的3步骤

1. 自定义一个类，实现`Condition`或`ConfigurationCondition`接口，实现`matches`方法
2. 在目标对象上使用`@Conditional`注解，并指定value的指为自定义的`Condition`类型
3. 启动Spring容器加载资源，此时`@Conditional`就会起作用了
<a name="bQfxK"></a>
## 案例1：阻止配置类的处理
在配置类上面使用`@Conditional`，这个注解的value指定的`Condition`当有一个为false的时候，Spring就会跳过处理这个配置类。<br />自定义一个`Condition`类：
```java
package com.javacode2018.lesson001.demo25.test3;

import org.springframework.context.annotation.Condition;
import org.springframework.context.annotation.ConditionContext;
import org.springframework.core.type.AnnotatedTypeMetadata;

public class MyCondition1 implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        return false;
    }
}
```
`matches`方法内部可以随意发挥，此处为了演示效果就直接返回false。<br />来个配置类，在配置类上面使用上面这个条件，此时会让配置类失效，如下：
```java
package com.javacode2018.lesson001.demo25.test3;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Conditional;
import org.springframework.context.annotation.Configuration;

@Conditional(MyCondition1.class) //@1
@Configuration
public class MainConfig3 {
    @Bean
    public String name() { //@1
        return "Java";
    }
}
```
@1：使用了自定义的条件类<br />@2：通过`@Bean`标注这name这个方法，如果这个配置类成功解析，会将name方法的返回值作为bean注册到Spring容器<br />来个测试类，启动Spring容器加载MainConfig3配置类，如下：
```java
package com.javacode2018.lesson001.demo25;

import com.javacode2018.lesson001.demo25.test3.MainConfig3;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.util.Map;

public class ConditionTest {

    @Test
    public void test3() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig3.class);
        Map<String, String> serviceMap = context.getBeansOfType(String.class);
        serviceMap.forEach((beanName, bean) -> {
            System.out.println(String.format("%s->%s", beanName, bean));
        });
    }
}
```
test3中，从容器中获取String类型的bean，运行test3没有任何输出。<br />可以将MainConfig3上面的`@Conditional`去掉，再次运行输出：
```
name->Java
```
<a name="PjhLH"></a>
## 案例2：阻止bean的注册
来个配置类，如下：
```java
package com.javacode2018.lesson001.demo25.test4;

import com.javacode2018.lesson001.demo25.test3.MyCondition1;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Conditional;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MainConfig4 {
    @Conditional(MyCondition1.class) //@1
    @Bean
    public String name() {
        return "Java";
    }

    @Bean
    public String address() {
        return "上海市";
    }
}
```
上面2个方法上面使用了`@Bean`注解来定义了2个bean，name方法上面使用了`@Conditional`注解，这个条件会在name这个bean注册到容器之前会进行判断，当条件为true的时候，name这个bean才会被注册到容器。<br />ConditionTest中新增个测试用例来加载上面这个配置类，从容器中获取String类型所有bean输出，代码如下：
```java
@Test
public void test4() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig4.class);
    Map<String, String> serviceMap = context.getBeansOfType(String.class);
    serviceMap.forEach((beanName, bean) -> {
        System.out.println(String.format("%s->%s", beanName, bean));
    });
}
```
运行输出：
```
address->上海市
```
可以看到容器中只有一个address被注册了，而name这个bean没有被注册。
<a name="nUMRV"></a>
## 案例3：bean不存在的时候才注册
<a name="RDCrV"></a>
### 需求
IService接口有两个实现类Service1和Service1，这两个类会放在2个配置类中通过`@Bean`的方式来注册到容器，此时想加个限制，只允许有一个IService类型的bean被注册到容器。<br />可以在`@Bean`标注的2个方法上面加上条件限制，当容器中不存在IService类型的bean时，才将这个方法定义的bean注册到容器，下面来看代码实现。
<a name="j1lBQ"></a>
### 代码实现
<a name="Sj9J2"></a>
#### 条件判断类：`OnMissingBeanCondition`
```java
package com.javacode2018.lesson001.demo25.test1;

import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.context.annotation.Condition;
import org.springframework.context.annotation.ConditionContext;
import org.springframework.context.annotation.ConfigurationCondition;
import org.springframework.core.type.AnnotatedTypeMetadata;

import java.util.Map;

public class OnMissingBeanCondition implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        //获取bean工厂
        ConfigurableListableBeanFactory beanFactory = context.getBeanFactory();
        //从容器中获取IService类型bean
        Map<String, IService> serviceMap = beanFactory.getBeansOfType(IService.class);
        //判断serviceMap是否为空
        return serviceMap.isEmpty();
    }

}
```
上面`matches`方法中会看容器中是否存在IService类型的bean，不存在的时候返回true
<a name="jmmEf"></a>
#### IService接口
```java
package com.javacode2018.lesson001.demo25.test1;

public interface IService {
}
```
<a name="s1DC2"></a>
#### 接口有2个实现类
<a name="wa1Mg"></a>
##### Service1
```java
package com.javacode2018.lesson001.demo25.test1;

public class Service1 implements IService {
}
```
<a name="hPC0s"></a>
##### Service2
```java
package com.javacode2018.lesson001.demo25.test1;

public class Service2 implements IService {
}
```
<a name="UVSW7"></a>
#### 来一个配置类负责注册Service1到容器
```java
package com.javacode2018.lesson001.demo25.test1;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Conditional;
import org.springframework.context.annotation.Configuration;

@Configuration
public class BeanConfig1 {
    @Conditional(OnMissingBeanCondition.class) //@1
    @Bean
    public IService service1() {
        return new Service1();
    }
}
```
@1：方法之前使用了条件判断
<a name="D4KxX"></a>
#### 再来一个配置类负责注册Service2到容器
```java
package com.javacode2018.lesson001.demo25.test1;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Conditional;
import org.springframework.context.annotation.Configuration;

@Configuration
public class BeanConfig2 {
    @Conditional(OnMissingBeanCondition.class)//@1
    @Bean
    public IService service2() {
        return new Service2();
    }
}
```
@1：方法之前使用了条件判断
<a name="m5AEE"></a>
#### 来一个总的配置类，导入另外2个配置类
```java
package com.javacode2018.lesson001.demo25.test1;

import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Import;

@Configuration
@Import({BeanConfig1.class,BeanConfig2.class}) //@1
public class MainConfig1 {
}
```
@1：通过`@Import`将其他2个配置类导入
<a name="Qxgn0"></a>
#### 来个测试用例
ConditionTest新增一个方法，方法中从容器中获取IService类型的bean，然后输出：
```java
@Test
public void test1() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig1.class);
    Map<String, IService> serviceMap = context.getBeansOfType(IService.class);
    serviceMap.forEach((beanName, bean) -> {
        System.out.println(String.format("%s->%s", beanName, bean));
    });
}
```
运行输出：
```
service1->com.javacode2018.lesson001.demo25.test1.Service1@2cd76f31
```
可以看出容器中只有一个IService类型的bean。<br />可以将`@Bean`标注的2个方法上面的`@Conditional`去掉，再运行会输出：
```
service1->com.javacode2018.lesson001.demo25.test1.Service1@49438269
service2->com.javacode2018.lesson001.demo25.test1.Service2@ba2f4ec
```
此时没有条件限制，2个Service都会注册到容器。
<a name="sZbUB"></a>
## 案例4：根据环境选择配置类
平常做项目的时候，有开发环境、测试环境、线上环境，每个环境中有些信息是不一样的，比如数据库的配置信息，下面来模拟不同环境中使用不同的配置类来注册不同的bean。
<a name="F9gJH"></a>
### 自定义一个条件的注解
```java
package com.javacode2018.lesson001.demo25.test2;

import org.springframework.context.annotation.Conditional;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Conditional(EnvCondition.class) //@1
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface EnvConditional {
    //环境(测试环境、开发环境、生产环境)
    enum Env { //@2
    TEST, DEV, PROD
}

//环境
Env value() default Env.DEV; //@3
}
```
@1：注意这个注解比较特别，这个注解上面使用到了`@Conditional`注解，这个地方使用到了一个自定义`Conditione`类：`EnvCondition`<br />@2：枚举，表示环境，定义了3个环境<br />@3：这个参数用指定环境<br />上面这个注解一会会用在不同环境的配置类上面
<a name="B7zQV"></a>
### 下面来3个配置类
让3个配置类分别在不同环境中生效，会在这些配置类上面使用上面自定义的`@EnvConditional`注解来做条件限定。<br />每个配置类中通过`@Bean`来定义一个名称为name的bean，一会通过输出这个bean来判断哪个配置类生效了。<br />下面来看3个配置类的代码
<a name="Le7mj"></a>
#### 测试环境配置类
```java
package com.javacode2018.lesson001.demo25.test2;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
@EnvConditional(EnvConditional.Env.TEST)//@1
public class TestBeanConfig {
    @Bean
    public String name() {
        return "我是测试环境!";
    }
}
```
@1指定的测试环境
<a name="o3wHS"></a>
#### 开发环境配置类
```java
package com.javacode2018.lesson001.demo25.test2;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
@EnvConditional(EnvConditional.Env.DEV) //@1
public class DevBeanConfig {
    @Bean
    public String name() {
        return "我是开发环境!";
    }
}
```
@1：指定的开发环境
<a name="EfHIO"></a>
#### 生产环境配置类
```java
package com.javacode2018.lesson001.demo25.test2;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
@EnvConditional(EnvConditional.Env.PROD) //@1
public class ProdBeanConfig {
    @Bean
    public String name() {
        return "我是生产环境!";
    }
}
```
@1：指定的生产环境
<a name="EWaq3"></a>
### 下面来看一下条件类：`EnvCondition`
条件类会解析配置类上面`@EnvConditional`注解，得到环境信息。<br />然后和目前的环境对比，决定返回true还是false，如下：
```java
package com.javacode2018.lesson001.demo25.test2;

import org.springframework.context.annotation.Condition;
import org.springframework.context.annotation.ConditionContext;
import org.springframework.core.type.AnnotatedTypeMetadata;

public class EnvCondition implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        //当前需要使用的环境
        EnvConditional.Env curEnv = EnvConditional.Env.DEV; //@1
        //获取使用条件的类上的EnvCondition注解中对应的环境
        EnvConditional.Env env = (EnvConditional.Env) metadata.getAllAnnotationAttributes(EnvConditional.class.getName()).get("value").get(0);
        return env.equals(curEnv);
    }

}
```
@1：这个用来指定当前使用的环境，此处假定当前使用的是开发环境，这个以后可以任意发挥，比如将这些放到配置文件中，此处方便演示效果。
<a name="AojOU"></a>
### 来个测试用例
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig2.class);
    System.out.println(context.getBean("name"));
}
```
<a name="dmjPC"></a>
### 运行输出
```
我是开发环境!
```
可以看到开发环境生效了。<br />修改一下`EnvCondition`的代码，切换到生产环境：
```java
EnvConditional.Env curEnv = EnvConditional.Env.PROD;
```
再次运行test2方法输出：
```
我是生产环境!
```
生产环境配置类生效了。
<a name="S5PBb"></a>
## 案例5：`Condition`指定优先级
<a name="lKGHY"></a>
### 多个`Condition`按顺序执行
`@Condtional`中value指定多个`Condtion`的时候，默认情况下会按顺序执行，还是通过代码来看一下效果。<br />下面代码中定义了3个`Condition`，每个`Condition`的`matches`方法中会输出当前类名，然后在配置类上面同时使用这3个`Condition`：
```java
package com.javacode2018.lesson001.demo25.test5;

import org.springframework.context.annotation.Condition;
import org.springframework.context.annotation.ConditionContext;
import org.springframework.context.annotation.Conditional;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.type.AnnotatedTypeMetadata;

class Condition1 implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        System.out.println(this.getClass().getName());
        return true;
    }
}

class Condition2 implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        System.out.println(this.getClass().getName());
        return true;
    }
}

class Condition3 implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        System.out.println(this.getClass().getName());
        return true;
    }
}

@Configuration
@Conditional({Condition1.class, Condition2.class, Condition3.class})
public class MainConfig5 {
}
```
来个测试用例
```java
@Test
public void test5() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig5.class);
}
```
运行输出：
```
com.javacode2018.lesson001.demo25.test5.Condition1
com.javacode2018.lesson001.demo25.test5.Condition2
com.javacode2018.lesson001.demo25.test5.Condition3
com.javacode2018.lesson001.demo25.test5.Condition1
com.javacode2018.lesson001.demo25.test5.Condition2
com.javacode2018.lesson001.demo25.test5.Condition3
com.javacode2018.lesson001.demo25.test5.Condition1
com.javacode2018.lesson001.demo25.test5.Condition2
com.javacode2018.lesson001.demo25.test5.Condition3
```
上面有多行输出，是因为Spring解析整个配置类的过程中，有好几个地方都会执行条件判断。<br />只用关注前3行，可以看出输出的属性和`@Conditional`中value值的顺序是一样的。
<a name="YoI1J"></a>
### 指定`Condition`的顺序
自定义的`Condition`可以实现`PriorityOrdered`接口或者继承`Ordered`接口，或者使用`@Order`注解，通过这些来指定这些`Condition`的优先级。<br />**排序规则：先按**`**PriorityOrdered**`**排序，然后按照order的值进行排序；也就是：**`**PriorityOrdered**`** asc,order值 asc**<br />下面这几个都可以指定order的值<br />接口：`org.springframework.core.Ordered`，有个`getOrder`方法用来返回int类型的值<br />接口：`org.springframework.core.PriorityOrdered`，继承了`Ordered`接口，所以也有`getOrder`方法<br />注解：`org.springframework.core.annotation.Order`，有个int类型的value参数指定Order的大小<br />看案例代码：
```java
package com.javacode2018.lesson001.demo25.test6;


import org.springframework.context.annotation.Condition;
import org.springframework.context.annotation.ConditionContext;
import org.springframework.context.annotation.Conditional;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.Ordered;
import org.springframework.core.PriorityOrdered;
import org.springframework.core.annotation.Order;
import org.springframework.core.type.AnnotatedTypeMetadata;

@Order(1) //@1
class Condition1 implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        System.out.println(this.getClass().getName());
        return true;
    }
}

class Condition2 implements Condition, Ordered { //@2
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        System.out.println(this.getClass().getName());
        return true;
    }

    @Override
    public int getOrder() { //@3
        return 0;
    }
}

class Condition3 implements Condition, PriorityOrdered { //@4
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        System.out.println(this.getClass().getName());
        return true;
    }

    @Override
    public int getOrder() {
        return 1000;
    }
}

@Configuration
@Conditional({Condition1.class, Condition2.class, Condition3.class})//@5
public class MainConfig6 {
}
```
@1：Condition1通过`@Order`指定顺序，值为1<br />@2：Condition2通过实现了`Ordered`接口来指定顺序，@3：`getOrder`方法返回1<br />@4：Condition3实现了`PriorityOrdered`接口，实现这个接口需要重写getOrder方法，返回1000<br />@5：Condtion顺序为1、2、3<br />根据排序的规则，`PriorityOrdered`的会排在前面，然后会再按照order升序，最后可以顺序是：
```
Condtion3->Condtion2->Condtion1
```
来个测试用例看看效果是不是分析的这样：
```java
@Test
public void test6() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig6.class);
}
```
运行test6，部分输出如下：
```
com.javacode2018.lesson001.demo25.test6.Condition3
com.javacode2018.lesson001.demo25.test6.Condition2
com.javacode2018.lesson001.demo25.test6.Condition1
```
结果和分析的一致。
<a name="mBvFQ"></a>
## 案例6：`ConfigurationCondition`使用
`ConfigurationCondition`使用的比较少，很多地方对这个基本上也不会去介绍，`Condition`接口基本上可以满足99%的需求了，但是SpringBoot中却大量用到了`ConfigurationCondition`这个接口。<br />`ConfigurationCondition`通过解释比较难理解，来个案例感受一下：
<a name="nrMI3"></a>
### 来一个普通的类：Service
```java
package com.javacode2018.lesson001.demo25.test7;

public class Service {
}
```
<a name="OICOD"></a>
### 来一个配置类，通过配置类注册上面这个Service
```java
package com.javacode2018.lesson001.demo25.test7;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class BeanConfig1 {
    @Bean
    public Service service() {
        return new Service();
    }
}
```
<a name="YqRff"></a>
### 再来一个配置类：BeanConfig2
```java
package com.javacode2018.lesson001.demo25.test7;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class BeanConfig2 {
    @Bean
    public String name() {
        return "Java";
    }
}
```
<a name="xF4mZ"></a>
### 来一个总的配置类
```java
package com.javacode2018.lesson001.demo25.test7;

import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Import;

@Configuration
@Import({BeanConfig1.class, BeanConfig2.class})
public class MainConfig7 {
}
```
上面通过`@Import`引入了另外2个配置类
<a name="tUBf7"></a>
### 来个测试用例加载MainConfig7配置类
```java
@Test
public void test7() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig7.class);
    context.getBeansOfType(String.class).forEach((beanName, bean) -> {
        System.out.println(String.format("%s->%s", beanName, bean));
    });
}
```
上面从容器中获取String类型的bean，然后输出。
<a name="GDjY6"></a>
### 运行输出
```
name->Java
```
<a name="h0Xej"></a>
### 现在有个需求
当容器中有Service这种类型的bean的时候，BeanConfig2才生效。<br />很简单吧，加个Condition就行了，内部判断容器中是否有Service类型的bean，继续
<a name="mMlii"></a>
### 来个自定义的Condition
```java
package com.javacode2018.lesson001.demo25.test7;


import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.context.annotation.Condition;
import org.springframework.context.annotation.ConditionContext;
import org.springframework.core.type.AnnotatedTypeMetadata;

public class MyCondition1 implements Condition {
    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        //获取spring容器
        ConfigurableListableBeanFactory beanFactory = context.getBeanFactory();
        //判断容器中是否存在Service类型的bean
        boolean existsService = !beanFactory.getBeansOfType(Service.class).isEmpty();
        return existsService;
    }
}
```
上面代码很简单，判断容器中是否有IService类型的bean。
<a name="VbTpo"></a>
### BeanConfig2上使用`Condition`条件判断
```java
@Configuration
@Conditional(MyCondition1.class)
public class BeanConfig2 {
    @Bean
    public String name() {
        return "Java";
    }
}
```
<a name="Nb0W9"></a>
### 再次运行test7输出
无任何输出
<a name="ZkcXb"></a>
### 为什么？
在前面说过，配置类的处理会依次经过2个阶段：配置类解析阶段和bean注册阶段，Condition接口类型的条件会对这两个阶段都有效，解析阶段的时候，容器中是还没有Service这个bean的，配置类中通过@Bean注解定义的bean在bean注册阶段才会被注册到Spring容器，所以BeanConfig2在解析阶段去容器中是看不到Service这个bean的，所以就被拒绝了。<br />**此时需要用到**`**ConfigurationCondition**`**了，让条件判断在bean注册阶段才起效。**
<a name="o0Q28"></a>
### 自定义一个`ConfigurationCondition`类
```java
package com.javacode2018.lesson001.demo25.test7;

import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.context.annotation.ConditionContext;
import org.springframework.context.annotation.ConfigurationCondition;
import org.springframework.core.type.AnnotatedTypeMetadata;

public class MyConfigurationCondition1 implements ConfigurationCondition {
    @Override
    public ConfigurationPhase getConfigurationPhase() {
        return ConfigurationPhase.REGISTER_BEAN; //@1
    }

    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        //获取spring容器
        ConfigurableListableBeanFactory beanFactory = context.getBeanFactory();
        //判断容器中是否存在Service类型的bean
        boolean existsService = !beanFactory.getBeansOfType(Service.class).isEmpty();
        return existsService;
    }
}
```
@1：指定条件在bean注册阶段，这个条件才有效<br />matches方法中的内容直接复制过来，判断规则不变。
<a name="LJOtl"></a>
### 修改BeanConfig2的类容
```
将
@Conditional(MyCondition1.class)
替换为
@Conditional(MyConfigurationCondition1.class)
```
<a name="HFoo8"></a>
### 再次运行test7输出
```
name->Java
```
此时name这个bean被输出了。<br />可以再试试将BeanConfig1中service方法上面的`@Bean`去掉，此时Service就不会被注册到容器，再运行一下test7，会发现没有输出了，此时BeanConfig2会失效。<br />**判断bean存不存在的问题，通常会使用**`**ConfigurationCondition**`**这个接口，阶段为：REGISTER_BEAN，这样可以确保条件判断是在bean注册阶段执行的。**<br />对SpringBoot比较熟悉的，它里面有很多`@Conditionxxx`这样的注解，可以去看一下这些注解，很多都实现了`ConfigurationCondition`接口。
<a name="BpDxI"></a>
## Spring中这块的源码<br /><br />
`**@Conditional**`**注解是被下面这个类处理的**
```
org.springframework.context.annotation.ConfigurationClassPostProcessor
```
**又是这个类，说了很多次了，非常重要的一个类，大家下去了多撸一下这个类的源码，这样理解起来更顺畅一些。**
<a name="mybY9"></a>
## 总结

1. `@Conditional`注解可以标注在Spring需要处理的对象上（配置类、`@Bean`方法），相当于加了个条件判断，通过判断的结果，让Spring觉得是否要继续处理被这个注解标注的对象
2. Spring处理配置类大致有2个过程：解析配置类、注册bean，这两个过程中都可以使用`@Conditional`来进行控制Spring是否需要处理这个过程
3. `Condition`默认会对2个过程都有效
4. `ConfigurationCondition`控制得更细一些，可以控制到具体那个阶段使用条件判断
