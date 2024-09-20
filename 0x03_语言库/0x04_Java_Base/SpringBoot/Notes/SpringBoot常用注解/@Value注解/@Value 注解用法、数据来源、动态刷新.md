Java
<a name="o188h"></a>
## `@Value`的用法
系统中需要连接db，连接db有很多配置信息。<br />系统中需要发送邮件，发送邮件需要配置邮件服务器的信息。<br />还有其他的一些配置信息。<br />可以将这些配置信息统一放在一个配置文件中，上线的时候由运维统一修改。<br />那么系统中如何使用这些配置信息呢，Spring中提供了`@Value`注解来解决这个问题。<br />通常会将配置信息以key=value的形式存储在properties配置文件中。<br />通过`@Value("${配置文件中的key}")`来引用指定的key对应的value。
<a name="aFviu"></a>
### @`Value`使用步骤
<a name="tJcw0"></a>
#### 步骤一：使用`@PropertySource`注解引入配置文件
将`@PropertySource`放在类上面，如下
```java
@PropertySource({"配置文件路径1","配置文件路径2"...})
```
`@PropertySource`注解有个value属性，字符串数组类型，可以用来指定多个配置文件的路径。<br />如：
```java
@Component
@PropertySource({"classpath:com/javacode2018/lesson002/demo18/db.properties"})
public class DbConfig {
}
```
<a name="FdaJ5"></a>
#### 步骤二：使用`@Value`注解引用配置文件的值
通过`@Value`引用上面配置文件中的值：<br />语法
```java
@Value("${配置文件中的key:默认值}")
@Value("${配置文件中的key}")
```
如：
```java
@Value("${password:123}")
```
上面如果password不存在，将123作为值
```java
@Value("${password}")
```
上面如果password不存在，值为${password}<br />假如配置文件如下
```
jdbc.url=jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8
jdbc.username=javacode
jdbc.password=javacode
```
使用方式如下：
```java
@Value("${jdbc.url}")
private String url;

@Value("${jdbc.username}")
private String username;

@Value("${jdbc.password}")
private String password;
```
下面来看案例
<a name="exGmq"></a>
#### 案例
<a name="SbZ4l"></a>
##### 来个配置文件db.properties
```
jdbc.url=jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8
jdbc.username=javacode
jdbc.password=javacode
```
<a name="QEw1b"></a>
##### 来个配置类，使用`@PropertySource`引入上面的配置文件
```java
package com.javacode2018.lesson002.demo18.test1;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.PropertySource;

@Configurable
@ComponentScan
@PropertySource({"classpath:com/javacode2018/lesson002/demo18/db.properties"})
public class MainConfig1 {
}
```
<a name="hfWGl"></a>
##### 来个类，使用`@Value`来使用配置文件中的信息
```java
package com.javacode2018.lesson002.demo18.test1;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

@Component
public class DbConfig {

    @Value("${jdbc.url}")
    private String url;

    @Value("${jdbc.username}")
    private String username;

    @Value("${jdbc.password}")
    private String password;

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    @Override
    public String toString() {
        return "DbConfig{" +
        "url='" + url + '\'' +
        ", username='" + username + '\'' +
        ", password='" + password + '\'' +
        '}';
    }
}
```
上面重点在于注解`@Value`注解，注意`@Value`注解中的
<a name="Tcj7E"></a>
##### 来个测试用例
```java
package com.javacode2018.lesson002.demo18;

import com.javacode2018.lesson002.demo18.test1.DbConfig;
import com.javacode2018.lesson002.demo18.test1.MainConfig1;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class ValueTest {

    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig1.class);
        context.refresh();

        DbConfig dbConfig = context.getBean(DbConfig.class);
        System.out.println(dbConfig);
    }
}
```
<a name="r1bwn"></a>
##### 运行输出
```
DbConfig{url='jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8', username='javacode', password='javacode'}
```
上面用起来比较简单，很多用过的人看一眼就懂了，这也是第一个问题，多数人都是ok的，下面来看`@Value`中数据来源除了配置文件的方式，是否还有其他方式。
<a name="trU6i"></a>
## `@Value`数据来源
通常情况下我们@Value的数据来源于配置文件，不过，还可以用其他方式，比如我们可以将配置文件的内容放在数据库，这样修改起来更容易一些。<br />需要先了解一下`@Value`中数据来源于Spring的什么地方。<br />Spring中有个类`org.springframework.core.env.PropertySource`<br />可以将其理解为一个配置源，里面包含了key->value的配置信息，可以通过这个类中提供的方法获取key对应的value信息<br />内部有个方法：
```java
public abstract Object getProperty(String name);
```
通过name获取对应的配置信息。<br />系统有个比较重要的接口
```
org.springframework.core.env.Environment
```
用来表示环境配置信息，这个接口有几个方法比较重要
```java
String resolvePlaceholders(String text);
MutablePropertySources getPropertySources();
```
`resolvePlaceholders`用来解析`${text}`的，`@Value`注解最后就是调用这个方法来解析的。<br />`getPropertySources`返回`MutablePropertySources`对象，来看一下这个类
```java
public class MutablePropertySources implements PropertySources {

    private final List<PropertySource<?>> propertySourceList = new CopyOnWriteArrayList<>();

}
```
内部包含一个`propertySourceList`列表。<br />Spring容器中会有一个`Environment`对象，最后会调用这个对象的`resolvePlaceholders`方法解析`@Value`。<br />大家可以捋一下，最终解析`@Value`的过程：

1. 将`@Value`注解的value参数值作为`Environment.resolvePlaceholders`方法参数进行解析
2. `Environment`内部会访问`MutablePropertySources`来解析
3. `MutablePropertySources`内部有多个`PropertySource`，此时会遍历`PropertySource`列表，调用`PropertySource.getProperty`方法来解析key对应的值

通过上面过程，如果想改变`@Value`数据的来源，只需要将配置信息包装为`PropertySource`对象，丢到`Environment`中的`MutablePropertySources`内部就可以了。<br />下面就按照这个思路来一个。<br />来个邮件配置信息类，内部使用`@Value`注入邮件配置信息
```java
package com.javacode2018.lesson002.demo18.test2;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

/**
 * 邮件配置信息
 */
@Component
public class MailConfig {

    @Value("${mail.host}")
    private String host;

    @Value("${mail.username}")
    private String username;

    @Value("${mail.password}")
    private String password;

    public String getHost() {
        return host;
    }

    public void setHost(String host) {
        this.host = host;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    @Override
    public String toString() {
        return "MailConfig{" +
        "host='" + host + '\'' +
        ", username='" + username + '\'' +
        ", password='" + password + '\'' +
        '}';
    }
}
```
再来个类DbUtil，`getMailInfoFromDb`方法模拟从db中获取邮件配置信息，存放在map中
```java
package com.javacode2018.lesson002.demo18.test2;

import java.util.HashMap;
import java.util.Map;

public class DbUtil {
    /**
     * 模拟从db中获取邮件配置信息
     *
     * @return
     */
    public static Map<String, Object> getMailInfoFromDb() {
        Map<String, Object> result = new HashMap<>();
        result.put("mail.host", "smtp.qq.com");
        result.put("mail.username", "Tom");
        result.put("mail.password", "123");
        return result;
    }
}
```
来个Spring配置类
```java
package com.javacode2018.lesson002.demo18.test2;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan
public class MainConfig2 {
}
```
下面是重点代码
```java
@Test
public void test2() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();

    /*下面这段是关键 start*/
    //模拟从db中获取配置信息
    Map<String, Object> mailInfoFromDb = DbUtil.getMailInfoFromDb();
    //将其丢在MapPropertySource中（MapPropertySource类是spring提供的一个类，是PropertySource的子类）
    MapPropertySource mailPropertySource = new MapPropertySource("mail", mailInfoFromDb);
    //将mailPropertySource丢在Environment中的PropertySource列表的第一个中，让优先级最高
    context.getEnvironment().getPropertySources().addFirst(mailPropertySource);
    /*上面这段是关键 end*/

    context.register(MainConfig2.class);
    context.refresh();
    MailConfig mailConfig = context.getBean(MailConfig.class);
    System.out.println(mailConfig);
}
```
注释比较详细，就不详细解释了。<br />直接运行，看效果
```
MailConfig{host='smtp.qq.com', username='Tom', password='123'}
```
有没有感觉很爽，此时可以随意修改`DbUtil.getMailInfoFromDb`，具体数据是从db中来，来时从redis或者其他介质中来，任由大家发挥。<br />上面重点是下面这段代码，大家需要理解
```java
/*下面这段是关键 start*/
//模拟从db中获取配置信息
Map<String, Object> mailInfoFromDb = DbUtil.getMailInfoFromDb();
//将其丢在MapPropertySource中（MapPropertySource类是spring提供的一个类，是PropertySource的子类）
MapPropertySource mailPropertySource = new MapPropertySource("mail", mailInfoFromDb);
//将mailPropertySource丢在Environment中的PropertySource列表的第一个中，让优先级最高
context.getEnvironment().getPropertySources().addFirst(mailPropertySource);
/*上面这段是关键 end*/
```
继续看下一个问题<br />**如果将配置信息放在db中，可能会通过一个界面来修改这些配置信息，然后保存之后，希望系统在不重启的情况下，让这些值在Spring容器中立即生效。**<br />`@Value`动态刷新的问题的问题，SpringBoot中使用`@RefreshScope`实现了。
<a name="vP0Bj"></a>
## 实现`@Value`动态刷新
<a name="Dv9Gr"></a>
### 先了解一个知识点
这块需要先讲一个知识点，用到的不是太多，所以很多人估计不太了解，但是非常重要的一个点，来看一下。<br />这个知识点是自定义bean作用域。<br />bean作用域中有个地方没有讲，来看一下`@Scope`这个注解的源码，有个参数是：
```java
ScopedProxyMode proxyMode() default ScopedProxyMode.DEFAULT;
```
这个参数的值是个`ScopedProxyMode`类型的枚举，值有下面4中
```java
public enum ScopedProxyMode {
    DEFAULT,
    NO,
    INTERFACES,
    TARGET_CLASS;
}
```
前面3个，不讲了，直接讲最后一个值是干什么的。<br />当@Scope中proxyMode为TARGET_CLASS的时候，会给当前创建的bean通过cglib生成一个代理对象，通过这个代理对象来访问目标bean对象。<br />理解起来比较晦涩，还是来看代码吧，容易理解一些，来个自定义的Scope案例。<br />**自定义一个bean作用域的注解**
```java
package com.javacode2018.lesson002.demo18.test3;

import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;

import java.lang.annotation.*;

@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Scope(BeanMyScope.SCOPE_MY) //@1
public @interface MyScope {
    /**
     * @see Scope#proxyMode()
     */
    ScopedProxyMode proxyMode() default ScopedProxyMode.TARGET_CLASS;//@2
}
```
@1：使用了`@Scope`注解，value为引用了一个常量，值为my，一会下面可以看到。<br />@2：注意这个地方，参数名称也是`proxyMode`，类型也是`ScopedProxyMode`，而`@Scope`注解中有个和这个同样类型的参数，Spring容器解析的时候，会将这个参数的值赋给`@MyScope`注解上面的`@Scope`注解的`proxyMode`参数，所以此处我们设置`proxyMode`值，最后的效果就是直接改变了`@Scope`中`proxyMode`参数的值。此处默认值取的是`ScopedProxyMode.TARGET_CLASS`<br />`**@MyScope**`**注解对应的Scope实现如下**
```java
package com.javacode2018.lesson002.demo18.test3;

import org.springframework.beans.factory.ObjectFactory;
import org.springframework.beans.factory.config.Scope;
import org.springframework.lang.Nullable;

/**
 * @see MyScope 作用域的实现
 */
public class BeanMyScope implements Scope {

    public static final String SCOPE_MY = "my"; //@1

    @Override
    public Object get(String name, ObjectFactory<?> objectFactory) { 
        System.out.println("BeanMyScope >>>>>>>>> get:" + name); //@2
        return objectFactory.getObject(); //@3
    }

    @Nullable
    @Override
    public Object remove(String name) {
        return null;
    }

    @Override
    public void registerDestructionCallback(String name, Runnable callback) {

    }

    @Nullable
    @Override
    public Object resolveContextualObject(String key) {
        return null;
    }

    @Nullable
    @Override
    public String getConversationId() {
        return null;
    }
}
```
@1：定义了一个常量，作为作用域的值<br />@2：这个get方法是关键，自定义作用域会自动调用这个get方法来创建bean对象，这个地方输出了一行日志，为了一会方便看效果<br />@3：通过`objectFactory.getObject()`获取bean实例返回。<br />**下面来创建个类，作用域为上面自定义的作用域**
```java
package com.javacode2018.lesson002.demo18.test3;

import org.springframework.stereotype.Component;

import java.util.UUID;

@Component
@MyScope //@1 
public class User {

    private String username;

    public User() { 
        System.out.println("---------创建User对象" + this); //@2
        this.username = UUID.randomUUID().toString(); //@3
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

}
```
@1：使用了自定义的作用域`@MyScope`<br />@2：构造函数中输出一行日志<br />@3：给username赋值，通过uuid随机生成了一个<br />**来个Spring配置类，加载上面**`**@Compontent**`**标注的组件**
```java
package com.javacode2018.lesson002.demo18.test3;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@ComponentScan
@Configuration
public class MainConfig3 {
}
```
**下面重点来了，测试用例**
```java
@Test
public void test3() throws InterruptedException {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    //将自定义作用域注册到spring容器中
    context.getBeanFactory().registerScope(BeanMyScope.SCOPE_MY, new BeanMyScope());//@1
    context.register(MainConfig3.class);
    context.refresh();

    System.out.println("从容器中获取User对象");
    User user = context.getBean(User.class); //@2
    System.out.println("user对象的class为：" + user.getClass()); //@3

    System.out.println("多次调用user的getUsername感受一下效果\n");
    for (int i = 1; i <= 3; i++) {
        System.out.println(String.format("********\n第%d次开始调用getUsername", i));
        System.out.println(user.getUsername());
        System.out.println(String.format("第%d次调用getUsername结束\n********\n", i));
    }
}
```
@1：将自定义作用域注册到Spring容器中<br />@2：从容器中获取User对应的bean<br />@3：输出这个bean对应的class，一会认真看一下，这个类型是不是User类型的<br />代码后面又搞了3次循环，调用user的getUsername方法，并且方法前后分别输出了一行日志。<br />**见证奇迹的时候到了，运行输出**
```
从容器中获取User对象
user对象的class为：class com.javacode2018.lesson002.demo18.test3.User$$EnhancerBySpringCGLIB$$80233127
多次调用user的getUsername感受一下效果

********
第1次开始调用getUsername
BeanMyScope >>>>>>>>> get:scopedTarget.user
---------创建User对象com.javacode2018.lesson002.demo18.test3.User@6a370f4
7b41aa80-7569-4072-9d40-ec9bfb92f438
第1次调用getUsername结束
********

********
第2次开始调用getUsername
BeanMyScope >>>>>>>>> get:scopedTarget.user
---------创建User对象com.javacode2018.lesson002.demo18.test3.User@1613674b
01d67154-95f6-44bb-93ab-05a34abdf51f
第2次调用getUsername结束
********

********
第3次开始调用getUsername
BeanMyScope >>>>>>>>> get:scopedTarget.user
---------创建User对象com.javacode2018.lesson002.demo18.test3.User@27ff5d15
76d0e86f-8331-4303-aac7-4acce0b258b8
第3次调用getUsername结束
********
```
从输出的前2行可以看出：

1. 调用`context.getBean(User.class)`从容器中获取bean的时候，此时并没有调用User的构造函数去创建User对象
2. 第二行输出的类型可以看出，`getBean`返回的user对象是一个cglib代理对象。

**后面的日志输出可以看出，每次调用user.getUsername方法的时候，内部自动调用了**`**BeanMyScope#get**`** 方法和 User的构造函数。**<br />**通过上面的案例可以看出，当自定义的Scope中**`**proxyMode=ScopedProxyMode.TARGET_CLASS**`**的时候，会给这个bean创建一个代理对象，调用代理对象的任何方法，都会调用这个自定义的作用域实现类（上面的**`**BeanMyScope**`**）中get方法来重新来获取这个bean对象。**
<a name="kqpwW"></a>
### 动态刷新`@Value`具体实现
那么我们可以利用上面讲解的这种特性来实现`@Value`的动态刷新，可以实现一个自定义的Scope，这个自定义的Scope支持@Value注解自动刷新，需要使用`@Value`注解自动刷新的类上面可以标注这个自定义的注解，当配置修改的时候，调用这些bean的任意方法的时候，就让Spring重启初始化一下这个bean，这个思路就可以实现了，下面我们来写代码。
<a name="eQVD3"></a>
#### 先来自定义一个Scope：`RefreshScope`
```java
package com.javacode2018.lesson002.demo18.test4;

import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;

import java.lang.annotation.*;

@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Scope(BeanRefreshScope.SCOPE_REFRESH)
@Documented
public @interface RefreshScope {
    ScopedProxyMode proxyMode() default ScopedProxyMode.TARGET_CLASS; //@1
}
```
要求标注`@RefreshScope`注解的类支持动态刷新`@Value`的配置<br />@1：这个地方是个关键，使用的是`ScopedProxyMode.TARGET_CLASS`
<a name="QGvlp"></a>
#### 这个自定义Scope对应的解析类
下面类中有几个无关的方法去掉了，可以忽略
```java
package com.javacode2018.lesson002.demo18.test4;


import org.springframework.beans.factory.ObjectFactory;
import org.springframework.beans.factory.config.Scope;
import org.springframework.lang.Nullable;

import java.util.concurrent.ConcurrentHashMap;

public class BeanRefreshScope implements Scope {

    public static final String SCOPE_REFRESH = "refresh";

    private static final BeanRefreshScope INSTANCE = new BeanRefreshScope();

    //来个map用来缓存bean
    private ConcurrentHashMap<String, Object> beanMap = new ConcurrentHashMap<>(); //@1

    private BeanRefreshScope() {
    }

    public static BeanRefreshScope getInstance() {
        return INSTANCE;
    }

    /**
     * 清理当前
     */
    public static void clean() {
        INSTANCE.beanMap.clear();
    }

    @Override
    public Object get(String name, ObjectFactory<?> objectFactory) {
        Object bean = beanMap.get(name);
        if (bean == null) {
            bean = objectFactory.getObject();
            beanMap.put(name, bean);
        }
        return bean;
    }

}
```
上面的get方法会先从`beanMap`中获取，获取不到会调用`objectFactory`的`getObject`让Spring创建bean的实例，然后丢到`beanMap`中<br />上面的clean方法用来清理`beanMap`中当前已缓存的所有bean
<a name="pXKpg"></a>
#### 来个邮件配置类，使用`@Value`注解注入配置，这个bean作用域为自定义的`@RefreshScope`
```java
package com.javacode2018.lesson002.demo18.test4;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

/**
 * 邮件配置信息
 */
@Component
@RefreshScope //@1
public class MailConfig {

    @Value("${mail.username}") //@2
    private String username;

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    @Override
    public String toString() {
        return "MailConfig{" +
        "username='" + username + '\'' +
        '}';
    }
}
```
@1：使用了自定义的作用域`@RefreshScope`<br />@2：通过`@Value`注入mail.username对一个的值<br />重写了`toString`方法，一会测试时候可以看效果。
<a name="WsjSN"></a>
#### 再来个普通的bean，内部会注入MailConfig
```java
package com.javacode2018.lesson002.demo18.test4;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class MailService {
    @Autowired
    private MailConfig mailConfig;

    @Override
    public String toString() {
        return "MailService{" +
                "mailConfig=" + mailConfig +
                '}';
    }
}
```
代码比较简单，重写了toString方法，一会测试时候可以看效果。
<a name="EiUZO"></a>
#### 来个类，用来从db中获取邮件配置信息
```java
package com.javacode2018.lesson002.demo18.test4;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

public class DbUtil {
    /**
     * 模拟从db中获取邮件配置信息
     *
     * @return
     */
    public static Map<String, Object> getMailInfoFromDb() {
        Map<String, Object> result = new HashMap<>();
        result.put("mail.username", UUID.randomUUID().toString());
        return result;
    }
}
```
<a name="n9NBj"></a>
#### 来个Spring配置类，扫描加载上面的组件
```java
package com.javacode2018.lesson002.demo18.test4;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@Configuration
@ComponentScan
public class MainConfig4 {
}
```
<a name="YtKUf"></a>
#### 来个工具类
内部有2个方法，如下：
```java
package com.javacode2018.lesson002.demo18.test4;

import org.springframework.context.support.AbstractApplicationContext;
import org.springframework.core.env.MapPropertySource;

import java.util.Map;

public class RefreshConfigUtil {
    /**
     * 模拟改变数据库中都配置信息
     */
    public static void updateDbConfig(AbstractApplicationContext context) {
        //更新context中的mailPropertySource配置信息
        refreshMailPropertySource(context);

        //清空BeanRefreshScope中所有bean的缓存
        BeanRefreshScope.getInstance().clean();
    }

    public static void refreshMailPropertySource(AbstractApplicationContext context) {
        Map<String, Object> mailInfoFromDb = DbUtil.getMailInfoFromDb();
        //将其丢在MapPropertySource中（MapPropertySource类是spring提供的一个类，是PropertySource的子类）
        MapPropertySource mailPropertySource = new MapPropertySource("mail", mailInfoFromDb);
        context.getEnvironment().getPropertySources().addFirst(mailPropertySource);
    }

}
```
`updateDbConfig`方法模拟修改db中配置的时候需要调用的方法，方法中2行代码，第一行代码调用`refreshMailPropertySource`方法修改容器中邮件的配置信息<br />`BeanRefreshScope.getInstance().clean()`用来清除`BeanRefreshScope`中所有已经缓存的bean，那么调用bean的任意方法的时候，会重新出发Spring容器来创建bean，Spring容器重新创建bean的时候，会重新解析`@Value`的信息，此时容器中的邮件配置信息是新的，所以`@Value`注入的信息也是新的。<br />来个测试用例
```java
@Test
public void test4() throws InterruptedException {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.getBeanFactory().registerScope(BeanRefreshScope.SCOPE_REFRESH, BeanRefreshScope.getInstance());
    context.register(MainConfig4.class);
    //刷新mail的配置到Environment
    RefreshConfigUtil.refreshMailPropertySource(context);
    context.refresh();

    MailService mailService = context.getBean(MailService.class);
    System.out.println("配置未更新的情况下,输出3次");
    for (int i = 0; i < 3; i++) { //@1
        System.out.println(mailService);
        TimeUnit.MILLISECONDS.sleep(200);
    }

    System.out.println("模拟3次更新配置效果");
    for (int i = 0; i < 3; i++) { //@2
        RefreshConfigUtil.updateDbConfig(context); //@3
        System.out.println(mailService);
        TimeUnit.MILLISECONDS.sleep(200);
    }
}
```
@1：循环3次，输出`mailService`的信息<br />@2：循环3次，内部先通过@3来模拟更新db中配置信息，然后在输出`mailService`信息
<a name="NIaO6"></a>
#### 见证奇迹的时刻，来看效果
```
配置未更新的情况下,输出3次
MailService{mailConfig=MailConfig{username='df321543-8ca7-4563-993a-bd64cbf50d53'}}
MailService{mailConfig=MailConfig{username='df321543-8ca7-4563-993a-bd64cbf50d53'}}
MailService{mailConfig=MailConfig{username='df321543-8ca7-4563-993a-bd64cbf50d53'}}
模拟3次更新配置效果
MailService{mailConfig=MailConfig{username='6bab8cea-9f4f-497d-a23a-92f15d0d6e34'}}
MailService{mailConfig=MailConfig{username='581bf395-f6b8-4b87-84e6-83d3c7342ca2'}}
MailService{mailConfig=MailConfig{username='db337f54-20b0-4726-9e55-328530af6999'}}
```
上面MailService输出了6次，前3次username的值都是一样的，后面3次username的值不一样了，说明修改配置起效了。
<a name="L8m4e"></a>
### 小结
**动态**`**@Value**`**实现的关键是**`**@Scope**`**中**`**proxyMode**`**参数，值为**`**ScopedProxyMode.DEFAULT**`**，会生成一个代理，通过这个代理来实现**`**@Value**`**动态刷新的效果，这个地方是关键。**<br />**有兴趣的可以去看一下SpringBoot中的**`**@RefreshScope**`**注解源码，和上面自定义的**`**@RefreshScope**`**类似，实现原理类似的。**
