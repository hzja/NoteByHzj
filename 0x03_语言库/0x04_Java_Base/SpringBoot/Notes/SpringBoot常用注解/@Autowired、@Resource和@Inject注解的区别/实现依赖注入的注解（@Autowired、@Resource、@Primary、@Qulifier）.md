JavaSpringBoot
<a name="YAKJT"></a>
## `@Autowired`：注入依赖对象
<a name="k5Uv6"></a>
### 作用
**实现依赖注入，Spring容器会对bean中所有字段、方法进行遍历，标注有**`**@Autowired**`**注解的，都会进行注入。**<br />看一下其定义：
```
@Target({ElementType.CONSTRUCTOR, ElementType.METHOD, ElementType.PARAMETER, ElementType.FIELD, ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Autowired {

    /**
     * Declares whether the annotated dependency is required.
     * <p>Defaults to {@code true}.
     */
    boolean required() default true;

}
```
可以用在构造器、方法、方法参数、字段、注解上。<br />参数：<br />`required`：标注的对象是否必须注入，可能这个对象在容器中不存在，如果为true的时候，找不到匹配的候选者就会报错，为false的时候，找不到也没关系 。
<a name="z7djv"></a>
### `@Autowired`查找候选者的过程
**查找过程有点复杂，看不懂的可以先跳过，先看后面案例，本文看完之后，可以回头再来看这个过程。**
<a name="uvYKi"></a>
#### `@Autowired`标注在字段上面：假定字段类型为一个自定义的普通的类型，候选者查找过程如下
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689671265282-745d6604-7b4d-4d9a-8ecd-0ac8dd68d9c4.png#averageHue=%23f6f6f6&clientId=u8044bf6c-801e-4&from=paste&id=u59c29574&originHeight=1838&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u481de639-b6f9-47c4-b438-4fabed02f93&title=)
<a name="s5nMe"></a>
#### `@Autowired`标注在方法上或者方法参数上面：假定参数类型为为一个自定义的普通的类型，候选者查找过程如下：
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689671265230-971a0299-c5a9-4f38-8f36-36122a98f03c.png#averageHue=%23f6f4f4&clientId=u8044bf6c-801e-4&from=paste&id=u2eae6165&originHeight=2255&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3542a21d-103f-4c06-988c-7c64995dc67&title=)<br />上图中深色的表示方法注入和字段注入查找过程的不同点。<br />上图中展示的是方法中只有一个参数的情况，如果有多个参数，就重复上面的过程，直到找到所有需要注入的参数。
<a name="uzPbV"></a>
#### 将指定类型的所有bean注入到`Collection`中
如果被注入的对象是`Collection`类型的，可以指定泛型的类型，然后会按照上面的方式查找所有满足泛型类型所有的bean
<a name="RKgJT"></a>
#### 将指定类型的所有bean注入到Map中
如果被注入的对象是Map类型的，可以指定泛型的类型，key通常为String类型，value为需要查找的bean的类型，然后会按照上面方式查找所有注入value类型的bean，将bean的name作为key，bean对象作为value，放在HashMap中，然后注入。
<a name="cWcG7"></a>
#### `@Autowired`查找候选者可以简化为下面这样
按类型找->通过限定符`@Qualifier`过滤->`@Primary`->`@Priority`->根据名称找（字段名称或者方法名称）<br />**概括为：先按类型找，然后按名称找**
<a name="bMBSP"></a>
### 案例1：`@Autowired`标注在构造器上，通过构造器注入依赖对象
<a name="gTLCa"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo26.test0;

import org.springframework.stereotype.Component;

@Component
public class Service1 {
}
```
<a name="aREMF"></a>
#### Service2
```java
package com.javacode2018.lesson001.demo26.test0;

import org.springframework.stereotype.Component;

@Component
public class Service2 {
    private Service1 service1;

    public Service2() { //@1
        System.out.println(this.getClass() + "无参构造器");
    }

    public Service2(Service1 service1) { //@2
        System.out.println(this.getClass() + "有参构造器");
        this.service1 = service1;
    }

    @Override
    public String toString() { //@2
        return "Service2{" +
        "service1=" + service1 +
        '}';
    }
}
```
Service2中依赖于Service1，有2个构造方法<br />@1：无参构造器<br />@2：有参构造器，可以通过这个传入依赖的Service1<br />@3：重写了`toString`方法，一会打印测试的时候方便查看
<a name="HYnDt"></a>
#### 来个总的配置文件
```java
package com.javacode2018.lesson001.demo26.test0;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan //@1
public class MainConfig0 {
}
```
@1：会自动扫描当前类所在的包，会将Service1和Service2注册到容器。
<a name="nxW3I"></a>
#### 来个测试用例
```java
package com.javacode2018.lesson001.demo26;

import com.javacode2018.lesson001.demo26.test0.MainConfig0;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class InjectTest {

    @Test
    public void test0() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig0.class);
        for (String beanName : context.getBeanDefinitionNames()) {
            System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
        }
    }

}
```
main方法中启动容器，加载MainConfig0配置类，然后输出容器中所有的bean
<a name="QuakN"></a>
#### 运行部分输出
```
class com.javacode2018.lesson001.demo26.test0.Service2无参构造器
service1->com.javacode2018.lesson001.demo26.test0.Service1@4a94ee4
service2->Service2{service1=null}
```
输出中可以看出调用了Service2的无参构造器，service2中的service1为null
<a name="c5alU"></a>
#### 通过`@Autowired`指定注入的构造器
在Service2有参有参构造器上面加上`@Autowired`注解，如下：
```java
public Service2(Service1 service1) {
    System.out.println(this.getClass() + "有参构造器");
    this.service1 = service1;
}
```
<a name="QGTes"></a>
#### 再次运行test0()
```
class com.javacode2018.lesson001.demo26.test0.Service2有参构造器
service1->com.javacode2018.lesson001.demo26.test0.Service1@4ec4f3a0
service2->Service2{service1=com.javacode2018.lesson001.demo26.test0.Service1@4ec4f3a0}
```
Service2有参构造器被调用了，service2中的service1有值了。
<a name="WAgbW"></a>
### 案例2：`@Autowired`标注在方法上，通过方法注入依赖的对象
<a name="PM8xm"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo26.test1;

import org.springframework.stereotype.Component;

@Component
public class Service1 {
}
```
<a name="xcC5P"></a>
#### Service2
```java
package com.javacode2018.lesson001.demo26.test1;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Service2 {
    private Service1 service1;

    @Autowired
    public void injectService1(Service1 service1) { //@1
        System.out.println(this.getClass().getName() + ".injectService1()");
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
@1：方法上标注了`@Autowired`，Spring容器会调用这个方法，从容器中查找Service1类型的bean，然后注入。
<a name="azZWI"></a>
#### 来个总的配置文件
```java
package com.javacode2018.lesson001.demo26.test1;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig1 {
}
```
<a name="zLkM3"></a>
#### 来个测试用例
InjectTest中加个方法
```java
@Test
public void test1() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig1.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="vR9xR"></a>
#### 运行输出
```
com.javacode2018.lesson001.demo26.test1.Service2.injectService1()
service1->com.javacode2018.lesson001.demo26.test1.Service1@9597028
service2->Service2{service1=com.javacode2018.lesson001.demo26.test1.Service1@9597028}
```
通过injectService1方法成功注入service1
<a name="Uefqr"></a>
### 案例3：`@Autowired`标注在setter方法上，通过setter方法注入
上面2种通过构造器，和通过普通的一个方法注入，不是很常见，可以将`@Autowired`标注在set方法上面，来注入指定的对象
<a name="uDvB1"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo26.test2;

import org.springframework.stereotype.Component;

@Component
public class Service1 {
}
```
<a name="HOHa8"></a>
#### Service2
```java
package com.javacode2018.lesson001.demo26.test2;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Service2 {

    private Service1 service1;

    @Autowired
    public void setService1(Service1 service1) { //@1
        System.out.println(this.getClass().getName() + ".setService1方法");
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
@1：标准的set方法，方法上使用了 `@Autowired`，会通过这个方法注入Service1类型的bean对象。
<a name="YuCS3"></a>
#### 来个总的配置文件
```java
package com.javacode2018.lesson001.demo26.test2;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig2 {
}
```
<a name="dVhmK"></a>
#### 来个测试用例
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig2.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="BvEwO"></a>
#### 运行输出
```
com.javacode2018.lesson001.demo26.test2.Service2.setService1方法
service1->com.javacode2018.lesson001.demo26.test2.Service1@6069db50
service2->Service2{service1=com.javacode2018.lesson001.demo26.test2.Service1@6069db50}
```
<a name="bDHI3"></a>
### 案例4：`@Autowired`标注在方法参数上
<a name="FCNjk"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo26.test3;

import org.springframework.stereotype.Component;

@Component
public class Service1 {
}
```
<a name="vE9Iq"></a>
#### Service2
```java
package com.javacode2018.lesson001.demo26.test3;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Service2 {

    private Service1 service1;

    @Autowired
    public void injectService1(Service1 service1, String name) { //@1
        System.out.println(String.format("%s.injectService1(),{service1=%s,name=%s}", this.getClass().getName(), service1, name));
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
@1：方法上标注了`@Autowired`，表示会将这个方法作为注入方法，这个方法有2个参数，Spring查找这2个参数对应的bean，然后注入。<br />第一个参数对应的bean是存在的，第二个是一个String类型的，并没有定义String类型bean，一会看看效果
<a name="cLnQX"></a>
#### 来个总的配置文件
```java
package com.javacode2018.lesson001.demo26.test3;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig3 {
}
```
<a name="Q4LPN"></a>
#### 来个测试用例
```java
@Test
public void test3() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig3.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="djDUg"></a>
#### 运行输出
```
org.springframework.beans.factory.UnsatisfiedDependencyException: Error creating bean with name 'service2': Unsatisfied dependency expressed through method 'injectService1' parameter 1; nested exception is org.springframework.beans.factory.NoSuchBeanDefinitionException: No qualifying bean of type 'java.lang.String' available: expected at least 1 bean which qualifies as autowire candidate. Dependency annotations: {}
```
报错了，从错误信息中可以看出，通过injectService1方法注入的时候，第二个参数为String类型，Spring从容器中没有找到String类型的候选bean，所以报错了。
<a name="DJJXE"></a>
#### 可以这么做
**多个参数的时候，方法上面的**`**@Autowired**`**默认对方法中所有参数起效，如果想对某个参数进行特定的配置，可以在参数上加上**`**@Autowired**`**，这个配置会覆盖方法上面的**`**@Autowired**`**配置。**<br />在第二个参数上面加上`@Autowired`，设置`required`为false：表示这个bean不是强制注入的，能找到就注入，找不到就注入一个null对象，调整一下代码，如下：
```java
@Autowired
public void injectService1(Service1 service1, @Autowired(required = false) String name) { //@1
    System.out.println(String.format("%s.injectService1(),{service1=%s,name=%s}", this.getClass().getName(), service1, name));
    this.service1 = service1;
}
```
此时方法的第一个参数被方法上面的`@Autowired`约束<br />第二个参数受`@Autowired(required = false)`约束
<a name="e1XnV"></a>
#### 再次运行输出
```
com.javacode2018.lesson001.demo26.test3.Service2.injectService1(),{service1=com.javacode2018.lesson001.demo26.test3.Service1@59309333,name=null}
service1->com.javacode2018.lesson001.demo26.test3.Service1@59309333
service2->Service2{service1=com.javacode2018.lesson001.demo26.test3.Service1@59309333}
```
注入成功了，service1有值，name为null
<a name="o0IVu"></a>
### 案例5：`@Autowired`用在字段上
<a name="ZSOAr"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo26.test4;

import org.springframework.stereotype.Component;

@Component
public class Service1 {
}
```
<a name="cwM8U"></a>
#### Service2
```java
package com.javacode2018.lesson001.demo26.test4;

import org.springframework.stereotype.Component;

@Component
public class Service2 {
}
```
<a name="Lzopw"></a>
#### Service3
```java
package com.javacode2018.lesson001.demo26.test4;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Service3 {

    @Autowired
    private Service1 service1;//@1

    @Autowired
    private Service2 service2;//@2

    @Override
    public String toString() {
        return "Service3{" +
        "service1=" + service1 +
        ", service2=" + service2 +
        '}';
    }
}
```
@1和@2：定义了2个字段，上面都标注了`@Autowired`，Spring会去容器中按照类型查找这2种类型的bean，然后设置给这2个属性。
<a name="HYjim"></a>
#### 来个总的配置文件
```java
package com.javacode2018.lesson001.demo26.test4;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig4 {
}
```
<a name="EFG6K"></a>
#### 来个测试用例
```java
@Test
public void test4() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig4.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="ZxgzO"></a>
#### 运行输出
```
service1->com.javacode2018.lesson001.demo26.test4.Service1@7e07db1f
service2->com.javacode2018.lesson001.demo26.test4.Service2@1189dd52
service3->Service3{service1=com.javacode2018.lesson001.demo26.test4.Service1@7e07db1f, service2=com.javacode2018.lesson001.demo26.test4.Service2@1189dd52}
```
service3中标注`@Autowired`的2个属性都有值了，都被注入成功了。
<a name="baE15"></a>
### 案例6：`@Autowired`标注字段，多个候选者的时候，按字段名称注入
<a name="Occfq"></a>
#### IService接口
```java
package com.javacode2018.lesson001.demo26.test5;

public interface IService {
}
```
<a name="bUOq4"></a>
#### 接口来2个实现
2个实现类上都标注了`@Component`注解，都会被注册到容器中。
<a name="gg1eq"></a>
##### Service0
```java
package com.javacode2018.lesson001.demo26.test5;

import org.springframework.stereotype.Component;

@Component
public class Service0 implements IService {
}
```
<a name="U0jh4"></a>
##### Service1
```java
package com.javacode2018.lesson001.demo26.test5;

import org.springframework.stereotype.Component;

@Component
public class Service1 implements IService {
}
```
<a name="tpOx4"></a>
#### 来个Service2
```java
package com.javacode2018.lesson001.demo26.test5;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Service2 {
    @Autowired
    private IService service1; //@1

    @Override
    public String toString() {
        return "Service2{" +
        "service1=" + service1 +
        '}';
    }
}
```
@1：标注了`@Autowired`注解，需要注入类型为IService类型的bean，满足这种类型的有2个：service0和service1<br />按照上面介绍的候选者查找过程，最后会注入和字段名称一样的bean，即：service1
<a name="grt0A"></a>
#### 来个总的配置类，负责扫描当前包中的组件
```java
package com.javacode2018.lesson001.demo26.test5;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig5 {
}
```
<a name="F5LRC"></a>
#### 来个测试用例
```java
@Test
public void test5() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig5.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="jomnp"></a>
#### 运行输出
```
service0->com.javacode2018.lesson001.demo26.test5.Service0@36902638
service1->com.javacode2018.lesson001.demo26.test5.Service1@223d2c72
service2->Service2{service1=com.javacode2018.lesson001.demo26.test5.Service1@223d2c72}
```
注意最后一行，service2中的service1被注入了bean：service1
<a name="Yp06V"></a>
### 案例7：将指定类型的所有bean，注入到Collection、Map中
<a name="ZtYDG"></a>
#### 注入到Collection中
**被注入的类型为Collection类型或者Collection子接口类型，注意必须是接口类型**，如：
```
Collection<IService>
List<IService>
Set<IService>
```
**会在容器中找到所有IService类型的bean，放到这个集合中**。
<a name="aWgdy"></a>
#### 注入到Map中
**被注入的类型为Map类型或者Map子接口类型，注意必须是接口类型**，如：
```java
Map<String,IService>
```
**会在容器中找到所有IService类型的bean，放到这个Map中，key为bean的名称，value为bean对象**。<br />来看案例代码。
<a name="WjHsl"></a>
#### 来个接口
```java
package com.javacode2018.lesson001.demo26.test6;

public interface IService {
}
```
<a name="fvcF3"></a>
#### 来2个实现类，标注`@Component`注解
<a name="s6mOE"></a>
##### Service0
```java
package com.javacode2018.lesson001.demo26.test6;

import org.springframework.stereotype.Component;

@Component
public class Service0 implements IService {
}
```
<a name="kQ4L2"></a>
##### Service1
```java
package com.javacode2018.lesson001.demo26.test6;

import org.springframework.stereotype.Component;

@Component
public class Service1 implements IService {
}
```
<a name="pKzTc"></a>
#### 再来个类Service2
```java
package com.javacode2018.lesson001.demo26.test6;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.util.List;
import java.util.Map;

@Component
public class Service2 {

    @Autowired
    private List<IService> services;

    @Autowired
    private Map<String, IService> serviceMap;

    @Override
    public String toString() {
        return "Service2{\n" +
        "services=" + services +
        ", \n serviceMap=" + serviceMap +
        '}';
    }
}
```
@1：注入IService类型的所有bean<br />@2：注入一个map
<a name="Pluqk"></a>
#### 来个总的配置类
```java
package com.javacode2018.lesson001.demo26.test6;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig6 {
}
```
<a name="QEgmP"></a>
#### 来个测试用例
```java
@Test
public void test6() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig6.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="laKn1"></a>
#### 运行输出
```
service0->com.javacode2018.lesson001.demo26.test6.Service0@1189dd52
service1->com.javacode2018.lesson001.demo26.test6.Service1@36bc55de
service2->Service2{
services=[com.javacode2018.lesson001.demo26.test6.Service0@1189dd52, com.javacode2018.lesson001.demo26.test6.Service1@36bc55de], 
 serviceMap={service0=com.javacode2018.lesson001.demo26.test6.Service0@1189dd52, service1=com.javacode2018.lesson001.demo26.test6.Service1@36bc55de}}
```
注意看一下上面services和serviceMap的值。
<a name="gwL7W"></a>
### `@Autowired`源码
Spring使用下面这个类处理`@Autowired`注解
```
org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor
```
<a name="tT8Vg"></a>
## `@Resource`：注意依赖对象
<a name="DpwZL"></a>
### 作用
**和**`**@Autowired**`**注解类似，也是用来注入依赖的对象的，spring容器会对bean中所有字段、方法进行遍历，标注有**`**@Resource**`**注解的，都会进行注入。**<br />看一下这个注解定义：
```java
javax.annotation.Resource

@Target({TYPE, FIELD, METHOD})
@Retention(RUNTIME)
public @interface Resource {
    String name() default "";
    ..其他不常用的参数省略
}
```
这个注解是javax中定义的，并不是Spring中定义的注解。<br />从定义上可以见，这个注解可以用在任何类型上面、字段、方法上面。<br />注意点：<br />**用在方法上的时候，方法参数只能有一个。**
<a name="XADh0"></a>
### `@Resource`查找候选者的过程
**查找过程有点复杂，看不懂的可以先跳过，先看后面案例，本文看完之后，可以回头再来看这个过程。**
<a name="bX9fe"></a>
#### `@Resource`标注在字段上面：假定字段类型为一个自定义的普通的类型，候选者查找过程如下
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689671294781-bc3856a0-c362-4a3e-bc63-873ec1d60913.png#averageHue=%23f4f1f1&clientId=u8044bf6c-801e-4&from=paste&id=uc431e5ce&originHeight=931&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uac0e5e83-098f-40fa-adb3-5cc3677f3dc&title=)
<a name="uHqvg"></a>
#### `@Autowired`标注在方法上或者方法参数上面：假定参数类型为为一个自定义的普通的类型，候选者查找过程如下：
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689671294709-99afa2c0-e954-4011-a2c4-251402ad99af.png#averageHue=%23f6f4f4&clientId=u8044bf6c-801e-4&from=paste&id=ue55e51e6&originHeight=1558&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9d06e253-5a04-4f0e-8ca6-7df7c0c1e9e&title=)
<a name="OYxKi"></a>
#### 将指定类型的所有bean注入到Collection中
如果被注入的对象是Collection类型的，可以指定泛型的类型，然后会按照上面的方式查找所有满足泛型类型所有的bean
<a name="kJ5xs"></a>
#### 将指定类型的所有bean注入到Map中
如果被注入的对象是Map类型的，可以指定泛型的类型，key通常为String类型，value为需要查找的bean的类型，然后会按照上面方式查找所有注入value类型的bean，将bean的name作为key，bean对象作为value，放在HashMap中，然后注入。
<a name="KtCKH"></a>
#### `@Resource`查找候选者可以简化为
先按Resource的name值作为bean名称找->按名称（字段名称、方法名称、set属性名称）找->按类型找->通过限定符`@Qualifier`过滤->`@Primary`->`@Priority`->根据名称找（字段名称或者方法参数名称）<br />**概括为：先按名称找，然后按类型找**
<a name="y5RkR"></a>
### 案例1：将`@Resource`标注在字段上
<a name="tjN5i"></a>
#### IService接口
```java
package com.javacode2018.lesson001.demo26.test7;

public interface IService {
}
```
<a name="NqxKe"></a>
#### 2个实现类
<a name="BOIUt"></a>
##### Service0
```java
package com.javacode2018.lesson001.demo26.test7;

import org.springframework.stereotype.Component;

@Component
public class Service0 implements IService {
}
```
`@Component`标注的bean名称默认为service0
<a name="onA8S"></a>
##### Service1
```java
package com.javacode2018.lesson001.demo26.test7;

import org.springframework.stereotype.Component;

@Component
public class Service1 implements IService {
}
```
`@Component`标注的bean名称默认为service1<br />再来一个类
```java
package com.javacode2018.lesson001.demo26.test7;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.annotation.Resource;
import java.util.List;
import java.util.Map;

@Component
public class Service2 {

    @Resource
    private IService service1;//@1

    @Override
    public String toString() {
        return "Service2{" +
        "service1=" + service1 +
        '}';
    }
}
```
@1：字段名称为service1，按照字段名称查找bean，会找到Service1
<a name="KHiUs"></a>
#### 来个配置类
```java
package com.javacode2018.lesson001.demo26.test7;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig7 {
}
```
<a name="H8rNu"></a>
#### 测试用例
```java
@Test
public void test7() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig7.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="OlBUS"></a>
#### 运行输出
```
service0->com.javacode2018.lesson001.demo26.test7.Service0@222545dc
service1->com.javacode2018.lesson001.demo26.test7.Service1@5c5eefef
service2->Service2{service1=com.javacode2018.lesson001.demo26.test7.Service1@5c5eefef}
```
最后一行可以看出注入了service1
<a name="Pgzjt"></a>
#### 如果将Service2中的代码调整一下
```java
@Resource
private IService service0;
```
此时会注入service0这个bean<br />同样`@Resource`可以用在方法上，也可以将所有类型的bean注入到Collection、Map中，这里就不演示了，重点了解一下候选者查找的过程，使用上就比较简单了，`@Resource`的其他案例，大家可以自己写写练练。<br />下面来说另外几个注解，也是比较重要的。
<a name="euQTY"></a>
### `@Resource`源码
Spring使用下面这个类处理`@Resource`注解
```
org.springframework.context.annotation.CommonAnnotationBeanPostProcessor
```
<a name="zqmIf"></a>
## `@Qualifier`：限定符
<a name="RJr7c"></a>
### 作用
这个单词的意思是：限定符。<br />**可以在依赖注入查找候选者的过程中对候选者进行过滤。**<br />看一下其定义：
```java
@Target({ElementType.FIELD, ElementType.METHOD, ElementType.PARAMETER, ElementType.TYPE, ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
public @interface Qualifier {

    String value() default "";

}
```
可以用在字段、方法、参数、任意类型、注解上面<br />有一个参数value<br />还是来看案例，通过案例理解更容易。
<a name="Yo0ja"></a>
### 案例1：用在类上
用在类上，可以理解为给通过`@Qulifier`给这个bean打了一个标签。
<a name="Y9ETx"></a>
#### 先来一个接口
```java
package com.javacode2018.lesson001.demo26.test8;

public interface IService {
}
```
<a name="WDwcE"></a>
#### 来3个实现类
**前2个**`**@Qulifier**`**的value为tag1，第3个实现类为tag2**
<a name="ZTvRt"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo26.test8;

import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

@Component
@Qualifier("tag1") //@1
public class Service1 implements IService {
}
```
@1：tag1<br />Service2
```java
package com.javacode2018.lesson001.demo26.test8;

import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

@Component
@Qualifier("tag1")
public class Service2 implements IService {
}
```
@1：tag1
<a name="bJ4nl"></a>
#### Service3
```java
package com.javacode2018.lesson001.demo26.test8;

import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

@Component
@Qualifier("tag2")//@1
public class Service3 implements IService {
}
```
@1：tag2
<a name="Gumpt"></a>
#### 来一个类，来注入上面几个bean
```java
package com.javacode2018.lesson001.demo26.test8;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import java.util.Map;

@Component
public class InjectService {
    @Autowired
    @Qualifier("tag1") //@1
    private Map<String, IService> serviceMap1;

    @Autowired
    @Qualifier("tag2") //@2
    private Map<String, IService> serviceMap2;

    @Override
    public String toString() {
        return "InjectService{" +
        "serviceMap1=" + serviceMap1 +
        ", serviceMap2=" + serviceMap2 +
        '}';
    }
}
```
@1：限定符的值为tag1，此时会将类上限定符为tag1的所有bean注入进来<br />@2：限定符的值为tag2，此时会将类上限定符为tag2的所有bean注入进来
<a name="FjGL2"></a>
#### 来个配置类
```java
package com.javacode2018.lesson001.demo26.test8;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig8 {
}
```
<a name="MojJ4"></a>
#### 测试用例
```java
@Test
public void test8() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig8.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
运行输出
```
injectService->InjectService{serviceMap1={service1=com.javacode2018.lesson001.demo26.test8.Service1@9597028, service2=com.javacode2018.lesson001.demo26.test8.Service2@6069db50}, serviceMap2={service3=com.javacode2018.lesson001.demo26.test8.Service3@4efbca5a}}
service1->com.javacode2018.lesson001.demo26.test8.Service1@9597028
service2->com.javacode2018.lesson001.demo26.test8.Service2@6069db50
service3->com.javacode2018.lesson001.demo26.test8.Service3@4efbca5a
```
注意第一行的输出，看一下serviceMap1和serviceMap2的值。<br />serviceMap1注入了`@Qulifier`的value为tag1的所有IService类型的bean<br />serviceMap1注入了`@Qulifier`的value为tag2的所有IService类型的bean<br />实现了bean分组的效果。
<a name="AY71z"></a>
### 案例2：`@Autowired`结合`@Qulifier`指定注入的bean
被注入的类型有多个的时候，可以使用`@Qulifier`来指定需要注入那个bean，将`@Qulifier`的value设置为需要注入bean的名称<br />看案例代码
<a name="PxMiE"></a>
#### 来个接口
```java
package com.javacode2018.lesson001.demo26.test9;

public interface IService {
}
```
<a name="GD9Id"></a>
#### 有2个实现类
2个实现类上面没有使用`@Qulifier`注解了
<a name="S7VZ1"></a>
##### Service1
```java
package com.javacode2018.lesson001.demo26.test9;

import org.springframework.stereotype.Component;

@Component
public class Service1 implements IService {
}
```
<a name="p88Gz"></a>
##### Service2
```java
package com.javacode2018.lesson001.demo26.test9;

import org.springframework.stereotype.Component;

@Component
public class Service2 implements IService {
}
```
可以知道上面2个bean的名称分别为：service1、service2
<a name="glATG"></a>
#### 来个类，注入IService类型的bean
```java
package com.javacode2018.lesson001.demo26.test9;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

@Component
public class InjectService {
    @Autowired
    @Qualifier("service2") //@1
    private IService service;

    @Override
    public String toString() {
        return "InjectService{" +
        "service=" + service +
        '}';
    }
}
```
**@1：这里限定符的值为service2，容器中IService类型的bean有2个[service1和service2]，当类上没有标注**`**@Qualifier**`**注解的时候，可以理解为：bean的名称就是限定符的值，所以@1这里会匹配到service2**
<a name="iN8tm"></a>
#### 来个配置类
```java
package com.javacode2018.lesson001.demo26.test9;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.stereotype.Component;

@ComponentScan
public class MainConfig9 {
}
```
<a name="MgSw6"></a>
#### 来个测试用例
```java
@Test
public void test9() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig9.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="xUIrN"></a>
#### 运行输出
```
injectService->InjectService{service=com.javacode2018.lesson001.demo26.test9.Service2@223d2c72}
service1->com.javacode2018.lesson001.demo26.test9.Service1@8f4ea7c
service2->com.javacode2018.lesson001.demo26.test9.Service2@223d2c72
```
从第一行可以看出注入了service1
<a name="eVbPy"></a>
### 案例3：用在方法参数上
<a name="fzlxk"></a>
#### 代码
```java
package com.javacode2018.lesson001.demo26.test10;


import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

@Component
public class InjectService {

    private IService s1;
    private IService s2;

    @Autowired
    public void injectBean(@Qualifier("service2") IService s1, @Qualifier("service1") IService s2) { //@1
        this.s1 = s1;
        this.s2 = s2;
    }

    @Override
    public String toString() {
        return "InjectService{" +
        "s1=" + s1 +
        ", s2=" + s2 +
        '}';
    }
}
```
@1：方法上标注了`@Autowired`注解，说明会被注入依赖，2个参数上分别使用了限定符来指定具体需要注入哪个bean
<a name="Jgc4K"></a>
#### 测试用例
```java
@Test
public void test10() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig10.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="LTsDO"></a>
#### 运行输出
```java
injectService->InjectService{s1=com.javacode2018.lesson001.demo26.test10.Service2@55183b20, s2=com.javacode2018.lesson001.demo26.test10.Service1@4f83df68}
service1->com.javacode2018.lesson001.demo26.test10.Service1@4f83df68
service2->com.javacode2018.lesson001.demo26.test10.Service2@55183b20
```
第一行中的<br />s1：service2<br />s2：service1
<a name="Jh3vj"></a>
### 案例4：用在setter方法上
不管是用在setter方法还是普通方法上面，都是一样的效果
<a name="WciSV"></a>
#### 代码
```java
package com.javacode2018.lesson001.demo26.test11;


import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

@Component
public class InjectService {

    private IService s1;
    private IService s2;

    @Autowired
    @Qualifier("service2")
    public void setS1(IService s1) {
        this.s1 = s1;
    }

    @Autowired
    @Qualifier("service2")
    public void setS2(IService s2) {
        this.s2 = s2;
    }

    @Override
    public String toString() {
        return "InjectService{" +
        "s1=" + s1 +
        ", s2=" + s2 +
        '}';
    }
}
```
上面2个setter方法上都有`@Autowired`注解，并且结合了`@Qulifier`注解来限定需要注入哪个bean
<a name="AKfTl"></a>
#### 测试用例
```java
@Test
public void test11() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig11.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
运行输出
```
injectService->InjectService{s1=com.javacode2018.lesson001.demo26.test11.Service2@35e2d654, s2=com.javacode2018.lesson001.demo26.test11.Service2@35e2d654}
service1->com.javacode2018.lesson001.demo26.test11.Service1@1bd4fdd
service2->com.javacode2018.lesson001.demo26.test11.Service2@35e2d654
```
输出中可以看出：s1为service2，s2为service1
<a name="t4BIl"></a>
## `@Primary`：设置为主要候选者
注入依赖的过程中，当有多个候选者的时候，可以指定哪个候选者为主要的候选者。<br />看一下其定义
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Primary {

}
```
可以用在类上或者方法上面。<br />通常定义bean常见的有2种方式：<br />方式1：在类上标注`@Component`注解，此时可以配合`@Primary`，标注这个bean为主要候选者<br />方式2：在配置文件中使用`@Bean`注解标注方法，来注册bean，可以在`@Bean`标注的方法上加上`@Primary`，标注这个bean为主要候选bean。<br />看案例。
<a name="abVw0"></a>
### 案例1：用在类上
<a name="D2YZM"></a>
#### 来个接口
```java
package com.javacode2018.lesson001.demo26.test12;

public interface IService {
}
```
<a name="n50ga"></a>
#### 2个实现类
<a name="tTAfw"></a>
##### Service1
```java
package com.javacode2018.lesson001.demo26.test12;

import org.springframework.stereotype.Component;

@Component
public class Service1 implements IService {
}
```
<a name="JZNrE"></a>
##### Service2
```java
package com.javacode2018.lesson001.demo26.test12;

import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Component;

@Component
@Primary
public class Service2 implements IService {
}
```
Service2上面使用了`@Primary`，表示这是个主要的候选者
<a name="BbUne"></a>
#### 再来个类，注入IService类型的bean
```java
package com.javacode2018.lesson001.demo26.test12;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class InjectService {

    @Autowired
    private IService service1; //@1

    @Override
    public String toString() {
        return "InjectService{" +
        "service1=" + service1 +
        '}';
    }
}
```
@1：容器中IService类型的bean有2个，但是service2为主要的候选者，所以此处会注入service2
<a name="IYh5W"></a>
#### 总的配置类
```java
package com.javacode2018.lesson001.demo26.test12;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig12 {
}
```
<a name="URf6L"></a>
#### 测试用例
```java
@Test
public void test12() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig12.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="MOMBn"></a>
#### 运行输出
```
injectService->InjectService{service1=com.javacode2018.lesson001.demo26.test12.Service2@49ec71f8}
service1->com.javacode2018.lesson001.demo26.test12.Service1@1d2adfbe
service2->com.javacode2018.lesson001.demo26.test12.Service2@49ec71f8
```
<a name="bpDkk"></a>
### 案例2：用在方法上，结合`@Bean`使用
<a name="VVaGy"></a>
#### 来个接口
```java
package com.javacode2018.lesson001.demo26.test13;

public interface IService {
}
```
<a name="GY9v2"></a>
#### 2个实现类
<a name="XJuLb"></a>
##### Service1
```java
package com.javacode2018.lesson001.demo26.test13;

public class Service1 implements IService {
}
```
<a name="JllXh"></a>
#### Service2
```java
package com.javacode2018.lesson001.demo26.test13;

public class Service2 implements IService {
}
```
<a name="pYj4E"></a>
#### InjectService
```java
package com.javacode2018.lesson001.demo26.test13;

import org.springframework.beans.factory.annotation.Autowired;

public class InjectService {

    @Autowired
    private IService service1;//@1

    @Override
    public String toString() {
        return "InjectService{" +
        "service1=" + service1 +
        '}';
    }
}
```
使用了`@Autowired`，需要注入
<a name="En5u6"></a>
#### 来个配置类，通过`@Bean`定义上面3个类型的bean
```java
package com.javacode2018.lesson001.demo26.test13;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;

@Configuration
public class MainConfig13 {

    @Bean
    public IService service1() {
        return new Service1();
    }

    @Bean
    @Primary //@1
    public IService service2() {
        return new Service2();
    }

    @Bean
    public InjectService injectService() {
        return new InjectService();
    }
}
```
上面是一个配置类，定义了3个bean<br />@1：这个bean被标注为主要的候选者
<a name="WWTez"></a>
#### 来个测试用例
```java
@Test
public void test13() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig13.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="tuk0w"></a>
#### 运行输出
```
service1->com.javacode2018.lesson001.demo26.test13.Service1@6913c1fb
service2->com.javacode2018.lesson001.demo26.test13.Service2@66d18979
injectService->InjectService{service1=com.javacode2018.lesson001.demo26.test13.Service2@66d18979}
```
注意最后一行，service1注入的是service2这个bean
<a name="GkvA0"></a>
## `@Bean`定义bean时注入依赖的几种方式
<a name="RlPOW"></a>
### 常见3种方式

1. **硬编码方式**
2. `**@Autowired**`**、**`**@Resource**`**的方式**
3. `**@Bean**`**标注的方法参数的方式**
<a name="tky5z"></a>
### 方式1：硬编码方式
来3个类
<a name="GYaOc"></a>
#### Service1
```java
package com.javacode2018.lesson001.demo26.test14;

public class Service1 {
}
```
<a name="hJMpY"></a>
#### Service2
```java
package com.javacode2018.lesson001.demo26.test14;

public class Service2 {
}
```
<a name="FZXxu"></a>
#### Service3
```java
package com.javacode2018.lesson001.demo26.test14;

public class Service3 {
    private Service1 service1;
    private Service2 service2;

    public Service1 getService1() {
        return service1;
    }

    public void setService1(Service1 service1) {
        this.service1 = service1;
    }

    public Service2 getService2() {
        return service2;
    }

    public void setService2(Service2 service2) {
        this.service2 = service2;
    }

    @Override
    public String toString() {
        return "Service3{" +
        "service1=" + service1 +
        ", service2=" + service2 +
        '}';
    }
}
```
上面类中会用到service1和service2，提供了对应的setter方法，一会通过setter方法注入依赖对象
<a name="ExFQK"></a>
#### 来个配置类，通过`@Bean`的方式创建上面对象
```java
package com.javacode2018.lesson001.demo26.test14;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MainConfig14 {
    @Bean
    public Service1 service1() {
        return new Service1();
    }

    @Bean
    public Service2 service2() {
        return new Service2();
    }

    @Bean
    public Service3 service3() {
        Service3 service3 = new Service3(); //@0
        service3.setService1(this.service1()); //@1
        service3.setService2(this.service2()); //@2
        return service3;
    }
}
```
上面代码中通过`@Bean`定义了3个bean<br />Service3中需要用到Service1和Service2，注意@1和@2直接调用当前方法获取另外2个bean，注入到service3中
<a name="Iv182"></a>
#### 测试用例
```java
@Test
public void test14() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig14.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
<a name="yBfKV"></a>
#### 运行输出
```
service1->com.javacode2018.lesson001.demo26.test14.Service1@41a2befb
service2->com.javacode2018.lesson001.demo26.test14.Service2@6c40365c
service3->Service3{service1=com.javacode2018.lesson001.demo26.test14.Service1@41a2befb, service2=com.javacode2018.lesson001.demo26.test14.Service2@6c40365c}
```
<a name="TLVte"></a>
### 方式2：`@Autowired`、`@Resource`的方式
这种方式就不讲了直接在需要注入的对象上面加上这2个注解的任意一个就行了，可以参考文章前面的部分。
<a name="VfIrx"></a>
### 方式3：`@Bean`标注的方法使用参数来进行注入
```java
package com.javacode2018.lesson001.demo26.test15;

import com.javacode2018.lesson001.demo26.test14.Service1;
import com.javacode2018.lesson001.demo26.test14.Service2;
import com.javacode2018.lesson001.demo26.test14.Service3;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MainConfig15 {
    @Bean
    public Service1 service1() {
        return new Service1();
    }

    @Bean
    public Service2 service2() {
        return new Service2();
    }

    @Bean
    public Service3 service3(Service1 s1, Service2 s2) { //@0
        Service3 service3 = new Service3();
        service3.setService1(s1); //@1
        service3.setService2(s2); //@2
        return service3;
    }
}
```
@0：这个地方是关键，方法上标注了`@Bean`，并且方法中是有参数的，Spring调用这个方法创建bean的时候，会将参数中的两个参数注入进来。<br />注入对象的查找逻辑可以参考上面`@Autowired`标注方法时查找候选者的逻辑。<br />来个测试用例
```java
@Test
public void test15() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig15.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
运行输出
```
service1->com.javacode2018.lesson001.demo26.test14.Service1@4009e306
service2->com.javacode2018.lesson001.demo26.test14.Service2@43c1b556
service3->Service3{service1=com.javacode2018.lesson001.demo26.test14.Service1@4009e306, service2=com.javacode2018.lesson001.demo26.test14.Service2@43c1b556}
```
同样注入成功了。
<a name="uYMOO"></a>
### 其他
<a name="OFPRn"></a>
#### `@Bean`标注的方法参数上使用`@Autowired`注解
```java
@Bean
public Service3 service3_0(Service1 s1, @Autowired(required = false) Service2 s2) { //@0
    Service3 service3 = new Service3();
    service3.setService1(s1); //@1
    service3.setService2(s2); //@2
    return service3;
}
```
@0：方法由2个参数，第二个参数上标注了`@Autowired(required = false)`，说明第二个参数候选者不是必须的，找不到会注入一个null对象；第一个参数候选者是必须的，找不到会抛出异常
<a name="BqAH0"></a>
#### `@Bean`结合`@Qualifier`
```java
package com.javacode2018.lesson001.demo26.test17;

import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.util.Map;

@Configuration
public class MainConfig17 {

    @Bean
    @Qualifier("tag1") //@1
    public Service1 service1() {
        return new Service1();
    }

    @Bean
    @Qualifier("tag1") //@2
    public Service2 service2() {
        return new Service2();
    }

    @Bean
    @Qualifier("tag2") //@3
    public Service3 service3() {
        return new Service3();
    }

    @Bean
    public InjectService injectService(@Qualifier("tag1") Map<String, IService> map1) { //@4
        InjectService injectService = new InjectService();
        injectService.setServiceMap1(map1);
        return injectService;
    }
}
```
Service1,Service2,Service3都实现了IService接口<br />@1,@2,@3这3个方法上面使用了`@Bean`注解，用来定义3个bean，这3个方法上还是用了`@Qualifier`注解，用来给这些bean定义标签，service1()方法类似于下面的写法：
```java
@Compontent
@Qualifier("tag1")
public class Service1 implements IService{
}
```
再回到MainConfig17中的@4：参数中需要注入`Map<String, IService>`，会查找IService类型的bean，容器中有3个，但是这个参数前面加上了`@Qualifier`限定符，值为tag1，所以会通过这个过滤，最后满足的候选者为：[service1,service]<br />对应测试用例
```java
@Test
public void test17() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig17.class);
    for (String beanName : context.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, context.getBean(beanName)));
    }
}
```
运行输出
```
service1->com.javacode2018.lesson001.demo26.test17.Service1@1190200a
service2->com.javacode2018.lesson001.demo26.test17.Service2@6a2f6f80
service3->com.javacode2018.lesson001.demo26.test17.Service3@45b4c3a9
injectService->InjectService{serviceMap1={service1=com.javacode2018.lesson001.demo26.test17.Service1@1190200a, service2=com.javacode2018.lesson001.demo26.test17.Service2@6a2f6f80}, serviceMap2=null}
```
注意最后一行serviceMap1，注入了service1和service2
<a name="SCOEs"></a>
## 泛型注入
<a name="Ew6Y3"></a>
### 先来2个普通的类
<a name="hO2oa"></a>
#### UserModel
```java
package com.javacode2018.lesson001.demo26.test18;

public class UserModel {
}
```
<a name="KR2BZ"></a>
#### OrderModel
```java
package com.javacode2018.lesson001.demo26.test18;

public class OrderModel {
}
```
记住上面2个普通的类UserModel和OrderModel，一会下面会用到。
<a name="zxTjq"></a>
### 来个泛型接口
```java
package com.javacode2018.lesson001.demo26.test18;

public interface IDao<T> {
}
```
上面是个泛型类，类名后面后尖括号
<a name="O7Udp"></a>
### 来2个实现类
两个实现类都会标注`@Compontent`，交给Spring容器管理
<a name="PVNj7"></a>
#### UserDao
```java
package com.javacode2018.lesson001.demo26.test18;

import org.springframework.stereotype.Component;

@Component
public class UserDao implements IDao<UserModel> { //@1
}
```
@1：指定了IDao后面泛型的类型为UserModel
<a name="EM1P7"></a>
#### OrderDao
```java
package com.javacode2018.lesson001.demo26.test18;

import org.springframework.stereotype.Component;

@Component
public class OrderDao implements IDao<OrderModel> {//@1
}
```
@1：指定了IDao后面泛型的类型为OrderModel
<a name="MCws6"></a>
### 在来个泛型类型
```java
package com.javacode2018.lesson001.demo26.test18;

import org.springframework.beans.factory.annotation.Autowired;

public class BaseService<T> {
    @Autowired
    private IDao<T> dao; //@1

    public IDao<T> getDao() {
        return dao;
    }

    public void setDao(IDao<T> dao) {
        this.dao = dao;
    }
}
```
BaseService同样是个泛型类<br />@1：这个地方要注意了，上面使用了`@Autowired`，来注入IDao对象
<a name="uS7RY"></a>
### BaseService来2个子类
两个子类都会标注`@Compontent`，交给Spring容器管理
<a name="GmY1n"></a>
#### UserService
```java
package com.javacode2018.lesson001.demo26.test18;

import org.springframework.stereotype.Component;

@Component
public class UserService extends BaseService<UserModel> {//@1
}
```
@1：指定了BaseService后面泛型的类型为UserModel
<a name="sQiQh"></a>
#### OrderService
```java
package com.javacode2018.lesson001.demo26.test18;

import org.springframework.stereotype.Component;

@Component
public class OrderService extends BaseService<OrderModel> {//@1
}
```
@1：指定了BaseService后面泛型的类型为`OrderModel`<br />`**UserService**`**和**`**OrderService**`**继承了**`**BaseService**`**，所以一会**`**BaseService**`**中的dao属性会被注入，一会关注一下dao这个属性的值，会是什么样的**
<a name="aE402"></a>
### 来个总的配置类
```java
package com.javacode2018.lesson001.demo26.test18;

import org.springframework.context.annotation.ComponentScan;

@ComponentScan
public class MainConfig18 {
}
```
上面有`@CompontentScan`注解，会自动扫描当前包中的所有类，并进行自动注入
<a name="J1ozA"></a>
### 来个测试用例
```java
@Test
public void test18() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig18.class);
    System.out.println(context.getBean(UserService.class).getDao());
    System.out.println(context.getBean(OrderService.class).getDao());
}
```
上面代码中会将两个service中的dao输出，来看一下效果
<a name="m8ap6"></a>
### 运行输出
```
com.javacode2018.lesson001.demo26.test18.UserDao@6adbc9d
com.javacode2018.lesson001.demo26.test18.OrderDao@4550bb58
```
结果就是重点了，dao属性并没有指定具体需要注入那个bean，此时是根据尖括号中的泛型类型来匹配的，这个功能也是相当厉害的。
<a name="vdmVk"></a>
## 总结

1. 需要掌握`@Autowired`注解和`@Resource`注解中候选者查找的过程
2. `@Autowired`：先通过类型找，然后通过名称找
3. `@Resource`：先通过名称找，然后通过类型找
4. `@Autowired`和`@Resource`，建议开发中使用`@Autowired`来实现依赖注入，Spring的注解用起来更名正言顺一些
5. `@Qulifier`：限定符，可以用在类上；也可以用在依赖注入的地方，可以对候选者的查找进行过滤
6. `@Primary`：多个候选者的时候，可以标注某个候选者为主要的候选者
7. `@Bean`中注入依赖的3种方式需要掌握
8. 掌握泛型注入的使用
