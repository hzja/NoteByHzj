<a name="yqjZD"></a>
## xml 中 bean 定义详解
<a name="fCXCm"></a>
### bean概念回顾
再来回顾一下，被Spring管理的对象统称为bean，程序中需要用到很多对象，将这些对象让Spring去创建和管理，可以通过bean xml配置文件告诉Spring容器需要管理哪些bean，Spring创建和组装好这些bean对象；那么如何从Spring中获取想要的bean对象呢，需要给bean定义一个名称，Spring内部将这些名称和具体的bean对象进行绑定，然后Spring容器可以通过这个的名称找需要的对象，这个名称叫做bean的名称，在一个Spring容器中需要是唯一的。
<a name="tM3We"></a>
### bean xml配置文件格式
bean xml文件用于定义Spring容器需要管理的bean，常见的格式如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <import resource="引入其他bean xml配置文件" />
  <bean id="bean标识" class="玩转类型名称"/>
  <alias name="bean标识" alias="别名" />

</beans>
```
beans是根元素，下面可以包含任意数量的import、bean、alias元素，下面对每个元素进行详解。
<a name="RuIAy"></a>
### bean元素
用来定义一个bean对象。
<a name="bFn4a"></a>
#### 格式
```
<bean id="bean唯一标识" name="bean名称" class="完整类型名称" factory-bean="工厂bean名称" factory-method="工厂方法"/>
```
<a name="zVG78"></a>
#### bean名称
每个bean都有一个名称，叫做bean名称，bean名称在一个Spring容器中必须唯一，否则会报错，通过bean名称可以从Spring容器获取对应的bean对象。
<a name="FCkFx"></a>
#### bean别名
先来说一下什么是别名？<br />相当于人的外号一样，一个人可能有很多外号，当别人喊这个人的名称或者外号的时候，都可以找到这个人。那么bean也一样，也可以给bean起几个外号，这个外号在Spring中叫做bean的别名，Spring容器允许使用者通过名称或者别名获取对应的bean对象。
<a name="pHRrP"></a>
#### bean名称别名定义规则
名称和别名可以通过bean元素中的id和name来定义，具体定义规则如下：：

1. 当id存在的时候，不管name有没有，取id为bean的名称
2. 当id不存在，此时需要看name，name的值可以通过`,``;``或者空格`分割，最后会按照分隔符得到一个String数组，数组的第一个元素作为bean的名称，其他的作为bean的别名
3. 当id和name都存在的时候，id为bean名称，name用来定义多个别名
4. 当id和name都不指定的时候，bean名称自动生成，生成规则下面详细说明
<a name="lZvFA"></a>
#### 案例
下面演示一下bean名称和别名的各种写法。
```xml
<!-- 通过id定义bean名称：user1 -->
<bean id="user1" class="com.javacode2018.lesson001.demo2.UserModel"/>

<!-- 通过name定义bean名称：user2 -->
<bean name="user2" class="com.javacode2018.lesson001.demo2.UserModel"/>

<!-- id为名称，name为别名；bean名称：user3，1个别名：[user3_1] -->
<bean id="user3" name="user3_1" class="com.javacode2018.lesson001.demo2.UserModel"/>

<!-- bean名称：user4，多个别名：[user4_1,user4_2,user4_3,user4_4] -->
<bean id="user4" name="user4_1,user4_2;user4_3 user4_4" class="com.javacode2018.lesson001.demo2.UserModel"/>

<!-- bean名称：user5，别名：[user5_1,user5_2,user5_3,user5_4] -->
<bean name="user5,user5_1,user5_2;user5_3 user5_4" class="com.javacode2018.lesson001.demo2.UserModel"/>
```
来写个java来输出上面所有bean的名称和其别名，如下：
```java
package com.javacode2018.lesson001.demo2;

import org.springframework.beans.factory.BeanFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import java.util.Arrays;

public class Client {

    public static void main(String[] args) {
        //1.bean配置文件位置
        String beanXml = "classpath:/com/javacode2018/lesson001/demo2/beans.xml";

        //2.创建ClassPathXmlApplicationContext容器，给容器指定需要加载的bean配置文件
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext(beanXml);

        for (String beanName : Arrays.asList("user1", "user2", "user3", "user4", "user5")) {
            //获取bean的别名
            String[] aliases = context.getAliases(beanName);
            System.out.println(String.format("beanName:%s,别名:[%s]", beanName, String.join(",", aliases)));
        }

        System.out.println("spring容器中所有bean如下：");

        //getBeanDefinitionNames用于获取容器中所有bean的名称
        for (String beanName : context.getBeanDefinitionNames()) {
            //获取bean的别名
            String[] aliases = context.getAliases(beanName);
            System.out.println(String.format("beanName:%s,别名:[%s]", beanName, String.join(",", aliases)));
        }

    }
}
```
上面有2个新的方法：<br />`getAliases`：通过bean名称获取这个bean的所有别名<br />`getBeanDefinitionNames`：返回Spring容器中定义的所有bean的名称<br />运行输出：
```
beanName:user1,别名:[]
beanName:user2,别名:[]
beanName:user3,别名:[user3_1]
beanName:user4,别名:[user4_1,user4_4,user4_2,user4_3]
beanName:user5,别名:[user5_3,user5_4,user5_1,user5_2]
spring容器中所有bean如下：
beanName:user1,别名:[]
beanName:user2,别名:[]
beanName:user3,别名:[user3_1]
beanName:user4,别名:[user4_1,user4_4,user4_2,user4_3]
beanName:user5,别名:[user5_3,user5_4,user5_1,user5_2]
```
<a name="VU80W"></a>
#### id和name都未指定
当id和name都未指定的时候，bean的名称和别名又是什么呢？此时由Spring自动生成，bean名称为：
```
bean的class的完整类名#编号
```
上面的编号是从0开始的，同种类型的没有指定名称的依次递增。<br />如：<br />下面定义了2个UserModel和2个String类型的bean，这些bean都没有指定id和name。
```xml
<bean class="com.javacode2018.lesson001.demo2.UserModel" />
<bean class="com.javacode2018.lesson001.demo2.UserModel" />

<bean class="java.lang.String" />
<bean class="java.lang.String" />
```
再次运行一下上面Client类的main方法，输出：
```
beanName:user1,别名:[]
beanName:user2,别名:[]
beanName:user3,别名:[user3_1]
beanName:user4,别名:[user4_1,user4_4,user4_2,user4_3]
beanName:user5,别名:[user5_3,user5_4,user5_1,user5_2]
spring容器中所有bean如下：
beanName:user1,别名:[]
beanName:user2,别名:[]
beanName:user3,别名:[user3_1]
beanName:user4,别名:[user4_1,user4_4,user4_2,user4_3]
beanName:user5,别名:[user5_3,user5_4,user5_1,user5_2]
beanName:com.javacode2018.lesson001.demo2.UserModel#0,别名:[com.javacode2018.lesson001.demo2.UserModel]
beanName:com.javacode2018.lesson001.demo2.UserModel#1,别名:[]
beanName:java.lang.String#0,别名:[java.lang.String]
beanName:java.lang.String#1,别名:[]
```
注意看一下上面最后4行的输出，bean名称和别名都是自动生成的，未指定id和name的bean对象，第一个会有别名，别名为完整的类名。bean名称为完整类名#编号。
<a name="BY6QA"></a>
### alias元素
alias元素也可以用来给某个bean定义别名，语法：
```xml
<alias name="需要定义别名的bean" alias="别名" />
```
如：
```xml
<bean id="user6" class="com.javacode2018.lesson001.demo2.UserModel" />
<alias name="user6" alias="user6_1" />
<alias name="user6" alias="user6_2" />
```
上面第一行通过bean元素定义了一个名称为user6的UserModel对象，后面2行给user6这个bean定义了2个别名，分别是user6_1和user6_2。<br />运行Client，会输出一行：
```
beanName:user6,别名:[user6_2,user6_1]
```
<a name="R2Ban"></a>
### import元素
当系统比较大的时候，会分成很多模块，每个模块会对应一个bean xml文件，可以在一个总的bean xml中对其他bean xml进行汇总，相当于把多个bean xml的内容合并到一个里面了，可以通过import元素引入其他bean配置文件。<br />语法：
```xml
<import resource="其他配置文件的位置" />
```
如：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <import resource="user.xml" />
  <import resource="order.xml" />

</beans>
```
<a name="lkeOd"></a>
## bean scope

1. 5种bean的sope及使用注意点
2. 自定义作用域的实现

应用中，有时候需要一个对象在整个应用中只有一个，有些对象希望每次使用的时候都重新创建一个，Spring对这种需求也提供了支持，在Spring中这个叫做bean的作用域，xml中定义bean的时候，可以通过scope属性指定bean的作用域，如：
```xml
<bean id="" class="" scope="作用域" />
```
Spring容器中scope常见的有5种，下面分别来介绍一下。
<a name="zpHgG"></a>
### singleton
当scope的值设置为singleton的时候，整个Spring容器中只会存在一个bean实例，通过容器多次查找bean的时候（调用BeanFactory的getBean方法或者bean之间注入依赖的bean对象的时候），返回的都是同一个bean对象，singleton是scope的默认值，所以Spring容器中默认创建的bean对象是单例的，通常Spring容器在启动的时候，会将scope为singleton的bean创建好放在容器中（有个特殊的情况，当bean的lazy被设置为true的时候，表示懒加载，那么使用的时候才会创建），用的时候直接返回。
<a name="U1nW0"></a>
#### 案例
<a name="tEAPq"></a>
##### bean xml配置
```xml
<!-- 单例bean，scope设置为singleton -->
<bean id="singletonBean" class="com.javacode2018.lesson001.demo4.BeanScopeModel" scope="singleton">
  <constructor-arg index="0" value="singleton"/>
</bean>
```
<a name="olbx8"></a>
##### BeanScopeModel代码
```java
package com.javacode2018.lesson001.demo4;

public class BeanScopeModel {
    public BeanScopeModel(String beanScope) {
        System.out.println(String.format("create BeanScopeModel,{sope=%s},{this=%s}", beanScope, this));
    }
}
```
上面构造方法中输出了一段文字，一会可以根据输出来看一下这个bean什么时候创建的，是从容器中获取bean的时候创建的还是容器启动的时候创建的。
<a name="HCTR5"></a>
##### 测试用例
```java
package com.javacode2018.lesson001.demo4;

import org.junit.Before;
import org.junit.Test;
import org.springframework.context.support.ClassPathXmlApplicationContext;

/**
 * <p>
 * bean作用域
 */
public class ScopeTest {

    ClassPathXmlApplicationContext context;

    @Before
    public void before() {
        System.out.println("spring容器准备启动.....");
        //1.bean配置文件位置
        String beanXml = "classpath:/com/javacode2018/lesson001/demo4/beans.xml";
        //2.创建ClassPathXmlApplicationContext容器，给容器指定需要加载的bean配置文件
        this.context = new ClassPathXmlApplicationContext(beanXml);
        System.out.println("spring容器启动完毕！");
    }

    /**
     * 单例bean
     */
    @Test
    public void singletonBean() {
        System.out.println("---------单例bean，每次获取的bean实例都一样---------");
        System.out.println(context.getBean("singletonBean"));
        System.out.println(context.getBean("singletonBean"));
        System.out.println(context.getBean("singletonBean"));
    }

}
```
上面代码中`before`方法上面有`@Before`注解，这个是junit提供的功能，这个方法会在所有`@Test`标注的方法之前之前运行，before方法中对容器进行初始化，并且在容器初始化前后输出了一段文字。<br />上面代码中，`singletonBean`方法中，3次获取singletonBean对应的bean。
<a name="LTTge"></a>
##### 运行测试用例
```
spring容器准备启动.....
create BeanScopeModel,{sope=singleton},{this=com.javacode2018.lesson001.demo4.BeanScopeModel@e874448}
spring容器启动完毕！
---------单例bean，每次获取的bean实例都一样---------
com.javacode2018.lesson001.demo4.BeanScopeModel@e874448
com.javacode2018.lesson001.demo4.BeanScopeModel@e874448
com.javacode2018.lesson001.demo4.BeanScopeModel@e874448
```
<a name="owleB"></a>
##### 结论
从输出中得到2个结论

- 前3行的输出可以看出，`BeanScopeModel`的构造方法是在容器启动过程中调用的，说明这个bean实例在容器启动过程中就创建好了，放在容器中缓存着
- 最后3行输出的是一样的，说明返回的是同一个bean对象
<a name="TqFoF"></a>
#### 单例bean使用注意
**单例bean是整个应用共享的，所以需要考虑到线程安全问题，之前在玩SpringMVC的时候，SpringMVC中controller默认是单例的，有些开发者在controller中创建了一些变量，那么这些变量实际上就变成共享的了，controller可能会被很多线程同时访问，这些线程并发去修改controller中的共享变量，可能会出现数据错乱的问题；所以使用的时候需要特别注意。**
<a name="a3f1n"></a>
### prototype
如果scope被设置为prototype类型的了，表示这个bean是多例的，通过容器每次获取的bean都是不同的实例，每次获取都会重新创建一个bean实例对象。
<a name="YV5jL"></a>
#### 案例
<a name="U6X5s"></a>
##### bean xml配置
```xml
<!-- 多例bean，scope设置为prototype-->
<bean id="prototypeBean" class="com.javacode2018.lesson001.demo4.BeanScopeModel" scope="prototype">
  <constructor-arg index="0" value="prototype"/>
</bean>
```
<a name="XgiJb"></a>
##### 新增一个测试用例
ScopeTest中新增一个方法
```java
/**
 * 多例bean
 */
@Test
public void prototypeBean() {
    System.out.println("---------单例bean，每次获取的bean实例都一样---------");
    System.out.println(context.getBean("prototypeBean"));
    System.out.println(context.getBean("prototypeBean"));
    System.out.println(context.getBean("prototypeBean"));
}
```
<a name="BuFBp"></a>
##### 运行测试用例
```
spring容器准备启动.....
spring容器启动完毕！
---------单例bean，每次获取的bean实例都一样---------
create BeanScopeModel,{sope=prototype},{this=com.javacode2018.lesson001.demo4.BeanScopeModel@289d1c02}
com.javacode2018.lesson001.demo4.BeanScopeModel@289d1c02
create BeanScopeModel,{sope=prototype},{this=com.javacode2018.lesson001.demo4.BeanScopeModel@22eeefeb}
com.javacode2018.lesson001.demo4.BeanScopeModel@22eeefeb
create BeanScopeModel,{sope=prototype},{this=com.javacode2018.lesson001.demo4.BeanScopeModel@17d0685f}
com.javacode2018.lesson001.demo4.BeanScopeModel@17d0685f
```
<a name="vNieT"></a>
##### 结论
输出中可以看出，容器启动过程中并没有去创建BeanScopeModel对象，3次获取prototypeBean得到的都是不同的实例，每次获取的时候才会去调用构造方法创建bean实例。
<a name="xrpRt"></a>
#### 多例bean使用注意
**多例bean每次获取的时候都会重新创建，如果这个bean比较复杂，创建时间比较长，会影响系统的性能，这个地方需要注意。**<br />**下面要介绍的3个：request、session、application都是在SpringWeb容器环境中才会有的。**
<a name="Z9ZL6"></a>
### request
当一个bean的作用域为request，表示在一次http请求中，一个bean对应一个实例；对每个http请求都会创建一个bean实例，request结束的时候，这个bean也就结束了，request作用域用在Spring容器的web环境中，这个以后讲SpringMVC的时候会说，Spring中有个web容器接口`WebApplicationContext`，这个里面对request作用域提供了支持，配置方式：
```xml
<bean id="" class="" scope="request" />
```
<a name="XTq95"></a>
### session
这个和request类似，也是用在web环境中，session级别共享的bean，每个会话会对应一个bean实例，不同的session对应不同的bean实例。
```xml
<bean id="" class="" scope="session" />
```
<a name="Fuvg7"></a>
### application
全局web应用级别的作用于，也是在web环境中使用的，一个web应用程序对应一个bean实例，通常情况下和singleton效果类似的，不过也有不一样的地方，singleton是每个Spring容器中只有一个bean实例，一般程序只有一个Spring容器，但是，一个应用程序中可以创建多个Spring容器，不同的容器中可以存在同名的bean，但是sope=aplication的时候，不管应用中有多少个Spring容器，这个应用中同名的bean只有一个。
```xml
<bean id="" class="" scope="application" />
```
<a name="cVPyf"></a>
### 自定义scope
有时候，Spring内置的几种sope都无法满足需求的时候，可以自定义bean的作用域。
<a name="kjGHA"></a>
#### 自定义Scope 3步骤
<a name="SPTTY"></a>
##### 第1步：实现Scope接口
来看一下这个接口定义
```java
package org.springframework.beans.factory.config;

import org.springframework.beans.factory.ObjectFactory;
import org.springframework.lang.Nullable;

public interface Scope {

    /**
    * 返回当前作用域中name对应的bean对象
    * name：需要检索的bean的名称
    * objectFactory：如果name对应的bean在当前作用域中没有找到，那么可以调用这个ObjectFactory来创建这个对象
    **/
    Object get(String name, ObjectFactory<?> objectFactory);

    /**
     * 将name对应的bean从当前作用域中移除
     **/
    @Nullable
    Object remove(String name);

    /**
     * 用于注册销毁回调，如果想要销毁相应的对象,则由Spring容器注册相应的销毁回调，而由自定义作用域选择是不是要销毁相应的对象
     */
    void registerDestructionCallback(String name, Runnable callback);

    /**
     * 用于解析相应的上下文数据，比如request作用域将返回request中的属性。
     */
    @Nullable
    Object resolveContextualObject(String key);

    /**
     * 作用域的会话标识，比如session作用域将是sessionId
     */
    @Nullable
    String getConversationId();

}
```
<a name="CLWvr"></a>
##### 第2步：将自定义的scope注册到容器
需要调用`org.springframework.beans.factory.config.ConfigurableBeanFactory#registerScope`的方法，看一下这个方法的声明
```java
/**
* 向容器中注册自定义的Scope
*scopeName：作用域名称
* scope：作用域对象
**/
void registerScope(String scopeName, Scope scope);
```
<a name="wdWlM"></a>
##### 第3步：使用自定义的作用域
定义bean的时候，指定bean的scope属性为自定义的作用域名称。
<a name="FSVQu"></a>
#### 案例
<a name="oYRbA"></a>
##### 需求
下面来实现一个线程级别的bean作用域，同一个线程中同名的bean是同一个实例，不同的线程中的bean是不同的实例。
<a name="eUiCW"></a>
##### 实现分析
需求中要求bean在线程中是贡献的，所以可以通过`ThreadLocal`来实现，`ThreadLocal`可以实现线程中数据的共享。下面来上代码。
<a name="meB05"></a>
##### ThreadScope
```java
package com.javacode2018.lesson001.demo4;

import org.springframework.beans.factory.ObjectFactory;
import org.springframework.beans.factory.config.Scope;
import org.springframework.lang.Nullable;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

/**
 * 自定义本地线程级别的bean作用域，不同的线程中对应的bean实例是不同的，同一个线程中同名的bean是同一个实例
 */
public class ThreadScope implements Scope {

    public static final String THREAD_SCOPE = "thread";//@1

    private ThreadLocal<Map<String, Object>> beanMap = new ThreadLocal() {
        @Override
        protected Object initialValue() {
            return new HashMap<>();
        }
    };

    @Override
    public Object get(String name, ObjectFactory<?> objectFactory) {
        Object bean = beanMap.get().get(name);
        if (Objects.isNull(bean)) {
            bean = objectFactory.getObject();
            beanMap.get().put(name, bean);
        }
        return bean;
    }

    @Nullable
    @Override
    public Object remove(String name) {
        return this.beanMap.get().remove(name);
    }

    @Override
    public void registerDestructionCallback(String name, Runnable callback) {
        //bean作用域范围结束的时候调用的方法，用于bean清理
        System.out.println(name);
    }

    @Nullable
    @Override
    public Object resolveContextualObject(String key) {
        return null;
    }

    @Nullable
    @Override
    public String getConversationId() {
        return Thread.currentThread().getName();
    }
}
```
@1：定义了作用域的名称为一个常量thread，可以在定义bean的时候给scope使用
<a name="k6liX"></a>
##### BeanScopeModel
```java
package com.javacode2018.lesson001.demo4;

public class BeanScopeModel {
    public BeanScopeModel(String beanScope) {
        System.out.println(String.format("线程:%s,create BeanScopeModel,{sope=%s},{this=%s}", Thread.currentThread(), beanScope, this));
    }
}
```
上面的构造方法中会输出当前线程的信息，到时候可以看到创建bean的线程。
<a name="UVBnm"></a>
##### bean配置文件
beans-thread.xml内容
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <!-- 自定义scope的bean -->
  <bean id="threadBean" class="com.javacode2018.lesson001.demo4.BeanScopeModel" scope="thread">
    <constructor-arg index="0" value="thread"/>
  </bean>
</beans>
```
注意上面的scope是自定义的，值为thread
<a name="nZ4wk"></a>
##### 测试用例
```java
package com.javacode2018.lesson001.demo4;

import org.springframework.context.support.ClassPathXmlApplicationContext;

import java.util.concurrent.TimeUnit;

/**
 * <p>
 * 自定义scope
 */
public class ThreadScopeTest {
    public static void main(String[] args) throws InterruptedException {
        String beanXml = "classpath:/com/javacode2018/lesson001/demo4/beans-thread.xml";
        //手动创建容器
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext();
        //设置配置文件位置
        context.setConfigLocation(beanXml);
        //启动容器
        context.refresh();
        //向容器中注册自定义的scope
        context.getBeanFactory().registerScope(ThreadScope.THREAD_SCOPE, new ThreadScope());//@1

        //使用容器获取bean
        for (int i = 0; i < 2; i++) { //@2
            new Thread(() -> {
                System.out.println(Thread.currentThread() + "," + context.getBean("threadBean"));
                System.out.println(Thread.currentThread() + "," + context.getBean("threadBean"));
            }).start();
            TimeUnit.SECONDS.sleep(1);
        }
    }
}
```
注意上面代码，重点在@1，这个地方向容器中注册了自定义的ThreadScope。<br />@2：创建了2个线程，然后在每个线程中去获取同样的bean 2次，然后输出，来看一下效果。
<a name="ZkLPm"></a>
##### 运行输出
```
线程:Thread[Thread-1,5,main],create BeanScopeModel,{sope=thread},{this=com.javacode2018.lesson001.demo4.BeanScopeModel@4049d530}
Thread[Thread-1,5,main],com.javacode2018.lesson001.demo4.BeanScopeModel@4049d530
Thread[Thread-1,5,main],com.javacode2018.lesson001.demo4.BeanScopeModel@4049d530
线程:Thread[Thread-2,5,main],create BeanScopeModel,{sope=thread},{this=com.javacode2018.lesson001.demo4.BeanScopeModel@87a76da}
Thread[Thread-2,5,main],com.javacode2018.lesson001.demo4.BeanScopeModel@87a76da
Thread[Thread-2,5,main],com.javacode2018.lesson001.demo4.BeanScopeModel@87a76da
```
从输出中可以看到，bean在同样的线程中获取到的是同一个bean的实例，不同的线程中bean的实例是不同的。
<a name="QJ1uS"></a>
### 总结

1. Spring容器自带的有2种作用域，分别是`singleton`和`prototype`；还有3种分别是spring web容器环境中才支持的`request`、`session`、`application`
2. `singleton`是Spring容器默认的作用域，一个Spring容器中同名的bean实例只有一个，多次获取得到的是同一个bean；单例的bean需要考虑线程安全问题
3. prototype是多例的，每次从容器中获取同名的bean，都会重新创建一个；多例bean使用的时候需要考虑创建bean对性能的影响
4. 一个应用中可以有多个Spring容器
5. 自定义scope 3个步骤，实现Scope接口，将实现类注册到Spring容器，使用自定义的sope
