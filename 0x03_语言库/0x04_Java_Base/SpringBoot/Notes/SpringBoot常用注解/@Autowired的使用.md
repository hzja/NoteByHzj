Java Spring @Autowired<br />![2021-08-11-20-09-44-639023.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684471394-7b4ca3b3-a4d1-4506-8ef4-22edaf850da0.png#averageHue=%23f7f7f7&clientId=u782184a3-6165-4&from=ui&id=u8e2f88f2&originHeight=624&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69428&status=done&style=none&taskId=ubd85e71d-3f79-4946-b611-c1df1a5f9db&title=)
<a name="vBOmy"></a>
## 1、`@Autowired`的默认装配
在Spring中`@Autowired`注解，是用来自动装配对象的。通常在项目中是这样用的：
```java
package com.sue.cache.service;

import org.springframework.stereotype.Service;

@Service
public class TestService1 {
    public void test1() {
    }
}

package com.sue.cache.service;

import org.springframework.stereotype.Service;

@Service
public class TestService2 {

    @Autowired
    private TestService1 testService1;

    public void test2() {
    }
}
```
没错，这样是能够装配成功的，因为默认情况下Spring是按照类型装配的，也就是所说的byType方式。<br />此外，`@Autowired`注解的`required`参数默认是`true`，表示开启自动装配，有些时候不想使用自动装配功能，可以将该参数设置成false。
<a name="mpqXT"></a>
## 2、相同类型的对象不只一个时
上面`byType`方式主要针对相同类型的对象只有一个的情况，此时对象类型是唯一的，可以找到正确的对象。<br />但如果相同类型的对象不只一个时，会发生什么？<br />在项目的test目录下，建了一个同名的类TestService1：
```java
import org.springframework.stereotype.Service;

@Service
public class TestService1 {

    public void test1() {
    }
}
```
重新启动项目时：
```java
Caused by: org.springframework.context.annotation.ConflictingBeanDefinitionException: Annotation-specified bean name 'testService1' for bean class [com.sue.cache.service.test.TestService1] conflicts with existing, non-compatible bean definition of same name and class [com.sue.cache.service.TestService1]
```
结果报错了，报类类名称有冲突，直接导致项目启动不来。<br />注意，这种情况不是相同类型的对象在`Autowired`时有两个导致的，非常容易产生混淆。这种情况是因为Spring的`@Service`方法不允许出现相同的类名，因为Spring会将类名的第一个字母转换成小写，作为bean的名称，比如：testService1，而默认情况下bean名称必须是唯一的。<br />下面看看如何产生两个相同的类型bean：
```java
public class TestService1 {

    public void test1() {
    }
}

@Service
public class TestService2 {

    @Autowired
    private TestService1 testService1;

    public void test2() {
    }
}

@Configuration
public class TestConfig {

    @Bean("test1")
    public TestService1 test1() {
        return new TestService1();
    }

    @Bean("test2")
    public TestService1 test2() {
        return new TestService1();
    }
}
```
在TestConfig类中手动创建TestService1实例，并且去掉TestService1类上原有的`@Service`注解。<br />重新启动项目：<br />![2021-08-11-20-09-44-723025.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684452856-fc879af1-9c4c-4bd2-837a-2b8835c88984.png#averageHue=%23302f2f&clientId=u782184a3-6165-4&from=ui&id=u14815366&originHeight=188&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75775&status=done&style=none&taskId=ueac60507-f306-478e-9a3a-26c9760ddf9&title=)<br />果然报错了，提示testService1是单例的，却找到两个对象。<br />其实还有一个情况会产生两个相同的类型bean：
```java
public interface IUser {
    void say();
}

@Service
public class User1 implements IUser{
    @Override
    public void say() {
    }
}

@Service
public class User2 implements IUser{
    @Override
    public void say() {
    }
}

@Service
public class UserService {

    @Autowired
    private IUser user;
}
```
项目重新启动时：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1628683768991-8f917fba-15a0-4224-ad62-7d7106dd7f7d.png#averageHue=%23333232&clientId=u782184a3-6165-4&from=paste&id=ud1b038f7&originHeight=83&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u63a0c596-5f37-4d98-8c6c-e01d842af20&title=)报错了，提示跟上面一样，testService1是单例的，却找到两个对象。<br />第二种情况在实际的项目中出现得更多一些，后面的例子，主要针对第二种情况。
<a name="OJdei"></a>
## 3、`@Qualifier`和`@Primary`
显然在Spring中，按照`Autowired`默认的装配方式：byType，是无法解决上面的问题的，这时可以改用按名称装配：byName。<br />只需在代码上加上`@Qualifier`注解即可：
```java
@Service
public class UserService {

    @Autowired
    @Qualifier("user1")
    private IUser user;
}
```
只需这样调整之后，项目就能正常启动了。<br />`Qualifier`意思是合格者，一般跟`Autowired`配合使用，需要指定一个bean的名称，通过bean名称就能找到需要装配的bean。<br />除了上面的`@Qualifier`注解之外，还能使用`@Primary`注解解决上面的问题。在User1上面加上`@Primary注`解：
```java
@Primary
@Service
public class User1 implements IUser{
    @Override
    public void say() {
    }
}
```
去掉UserService上的`@Qualifier`注解：
```java
@Service
public class UserService {

    @Autowired
    private IUser user;
}
```
重新启动项目，一样能正常运行。<br />当使用自动配置的方式装配Bean时，如果这个Bean有多个候选者，假如其中一个候选者具有`@Primary`注解修饰，该候选者会被选中，作为自动配置的值。
<a name="hMdo2"></a>
## 4、`@Autowired`的使用范围
上面的实例中`@Autowired`注解，都是使用在成员变量上，但`@Autowired`的强大之处，远非如此。<br />先看看`@Autowired`注解的定义：<br />![2021-08-11-20-09-44-860026.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684409984-7dd9adaa-5f85-4412-ab2e-a741a775327c.png#averageHue=%232c2c2b&clientId=u782184a3-6165-4&from=ui&id=u33246d9d&originHeight=286&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=67802&status=done&style=none&taskId=ue14951aa-1c77-473b-8691-cb74c35cb15&title=)<br />从图中可以看出该注解能够使用在5种目标类型上，下面用一张图总结一下：<br />![2021-08-11-20-09-44-947027.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684422629-764f078e-7ad5-4b8c-997e-ddb9d2525116.png#averageHue=%23f7f7f7&clientId=u782184a3-6165-4&from=ui&id=ue2e8f224&originHeight=624&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69428&status=done&style=none&taskId=uc9c7ccd6-0969-4ed6-af72-d25519b0918&title=)<br />该注解平常使用最多的地方可能是在成员变量上。<br />接下来，重点看看在其他地方该怎么用？
<a name="B1l5R"></a>
### 4.1 成员变量
在成员变量上使用`Autowired`注解：
```java
@Service
public class UserService {

    @Autowired
    private IUser user;
}
```
这种方式可能是平时用得最多的。
<a name="J8lDd"></a>
### 4.2 构造器
在构造器上使用`Autowired`注解：
```java
@Service
public class UserService {

    private IUser user;

    @Autowired
    public UserService(IUser user) {
        this.user = user;
        System.out.println("user:" + user);
    }
}
```
注意，在构造器上加`Autowired`注解，实际上还是使用了`Autowired`装配方式，并非构造器装配。
<a name="eSJt2"></a>
### 4.3 方法
在普通方法上加`Autowired`注解：
```java
@Service
public class UserService {

    @Autowired
    public void test(IUser user) {
       user.say();
    }
}
```
Spring会在项目启动的过程中，自动调用一次加了`@Autowired`注解的方法，可以在该方法做一些初始化的工作。<br />也可以在setter方法上`Autowired`注解：
```java
@Service
public class UserService {

    private IUser user;

    @Autowired
    public void setUser(IUser user) {
        this.user = user;
    }
}
```
<a name="U41TD"></a>
### 4.4 参数
可以在构造器的入参上加`Autowired`注解：
```java
@Service
public class UserService {

    private IUser user;

    public UserService(@Autowired IUser user) {
        this.user = user;
        System.out.println("user:" + user);
    }
}
```
也可以在非静态方法的入参上加`Autowired`注解：
```java
@Service
public class UserService {

    public void test(@Autowired IUser user) {
       user.say();
    }
}
```
<a name="Q9ALA"></a>
### 4.5 注解
这种方式其实用得不多，我就不过多介绍了。
<a name="L9hXN"></a>
## 5、`@Autowired`的高端玩法
其实上面举的例子都是通过`@Autowired`自动装配单个实例，它也能自动装配多个实例，怎么回事呢？<br />将UserService方法调整一下，用一个List集合接收IUser类型的参数：
```java
@Service
public class UserService {

    @Autowired
    private List<IUser> userList;

    @Autowired
    private Set<IUser> userSet;

    @Autowired
    private Map<String, IUser> userMap;

    public void test() {
        System.out.println("userList:" + userList);
        System.out.println("userSet:" + userSet);
        System.out.println("userMap:" + userMap);
    }
}
```
增加一个controller：
```java
@RequestMapping("/u")
@RestController
public class UController {

    @Autowired
    private UserService userService;

    @RequestMapping("/test")
    public String test() {
        userService.test();
        return "success";
    }
}
```
调用该接口后：<br />![2021-08-11-20-09-45-004028.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684390556-c1c38241-8f88-402f-9e2a-8dc9562943aa.png#averageHue=%23383736&clientId=u782184a3-6165-4&from=ui&id=u2393b1f8&originHeight=87&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=76424&status=done&style=none&taskId=uabe6a561-58bf-4954-abd3-ecd880eb60d&title=)<br />从上图中看出：userList、userSet和userMap都打印出了两个元素，说明`@Autowired`会自动把相同类型的IUser对象收集到集合中。
<a name="eMV54"></a>
## 6、`@Autowired`一定能装配成功？
前面介绍了`@Autowired`注解这么多牛逼之处，其实有些情况下，即使使用了`@Autowired`装配的对象还是null，到底是什么原因呢？
<a name="iSUYU"></a>
### 6.1 没有加`@Service`注解
在类上面忘了加`@Controller`、`@Service`、`@Component`、`@Repository`等注解，Spring就无法完成自动装配的功能，例如：
```java
public class UserService {

    @Autowired
    private IUser user;

    public void test() {
        user.say();
    }
}
```
这种情况应该是最常见的错误了。
<a name="klFGH"></a>
### 6.2 注入`Filter`或`Listener`
web应用启动的顺序是：`listener`->`filter`->`servlet`。<br />![2021-08-11-20-09-45-067028.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684362139-101004ed-60a1-4432-8cf5-e68c89f8f04a.png#averageHue=%23f6f0f0&clientId=u782184a3-6165-4&from=ui&id=uc663fef5&originHeight=472&originWidth=334&originalType=binary&ratio=1&rotation=0&showTitle=false&size=34377&status=done&style=shadow&taskId=u680075be-ed9a-4a43-861f-26c2209583f&title=)<br />接下来看看这个案例：
```java
public class UserFilter implements Filter {

    @Autowired
    private IUser user;

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        user.say();
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {

    }

    @Override
    public void destroy() {
    }
}

@Configuration
public class FilterConfig {

    @Bean
    public FilterRegistrationBean filterRegistrationBean() {
        FilterRegistrationBean bean = new FilterRegistrationBean();
        bean.setFilter(new UserFilter());
        bean.addUrlPatterns("/*");
        return bean;
    }
}
```
程序启动会报错：<br />![2021-08-11-20-09-45-173031.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684344782-4f1e7b06-3786-438e-bdf2-284855d5e5a2.png#averageHue=%233f3837&clientId=u782184a3-6165-4&from=ui&id=ucce52118&originHeight=323&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=326000&status=done&style=none&taskId=u45ed2c61-671d-4524-83e0-d568a925852&title=)<br />tomcat无法正常启动。<br />什么原因呢？<br />众所周知，SpringMVC的启动是在`DisptachServlet`里面做的，而它是在`listener`和`filter`之后执行。如果想在`listener`和`filter`里面`@Autowired`某个bean，肯定是不行的，因为`filter`初始化的时候，此时bean还没有初始化，无法自动装配。<br />如果工作当中真的需要这样做，该如何解决这个问题呢？
```java
public class UserFilter  implements Filter {

    private IUser user;

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        ApplicationContext applicationContext = WebApplicationContextUtils.getWebApplicationContext(filterConfig.getServletContext());
        this.user = ((IUser)(applicationContext.getBean("user1")));
        user.say();
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {

    }

    @Override
    public void destroy() {

    }
}
```
答案是使用`WebApplicationContextUtils.getWebApplicationContext`获取当前的`ApplicationContext`，再通过它获取到bean实例。
<a name="Kmi7e"></a>
### 6.3 注解未被`@ComponentScan`扫描
通常情况下，`@Controller`、`@Service`、`@Component`、`@Repository`、`@Configuration`等注解，是需要通过`@ComponentScan`注解扫描，收集元数据的。<br />但是，如果没有加`@ComponentScan`注解，或者`@ComponentScan`注解扫描的路径不对，或者路径范围太小，会导致有些注解无法收集，到后面无法使用`@Autowired`完成自动装配的功能。<br />有个好消息是，在SpringBoot项目中，如果使用了`@SpringBootApplication`注解，它里面内置了`@ComponentScan`注解的功能。
<a name="vrazN"></a>
### 6.4 循环依赖问题
如果A依赖于B，B依赖于C，C又依赖于A，这样就形成了一个死循环。<br />![2021-08-11-20-09-45-249024.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684202620-50470354-c620-4429-b961-d5f25ecfb240.png#averageHue=%23eeeeee&clientId=u782184a3-6165-4&from=ui&id=u4077d6cd&originHeight=474&originWidth=636&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37761&status=done&style=shadow&taskId=ucad0378a-2eb4-41db-b266-a856590edf0&title=)<br />Spring的bean默认是单例的，如果单例bean使用`@Autowired`自动装配，大多数情况，能解决循环依赖问题。<br />但是如果bean是多例的，会出现循环依赖问题，导致bean自动装配不了。<br />还有有些情况下，如果创建了代理对象，即使bean是单例的，依然会出现循环依赖问题。
<a name="RYiIa"></a>
## 7、`@Autowired`和`@Resouce`的区别
`@Autowired`功能虽说非常强大，但是也有些不足之处。比如：比如它跟Spring强耦合了，如果换成了JFinal等其他框架，功能就会失效。而`@Resource`是JSR-250提供的，它是Java标准，绝大部分框架都支持。<br />除此之外，有些场景使用`@Autowired`无法满足的要求，改成`@Resource`却能解决问题。接下来，我们重点看看`@Autowired`和`@Resource`的区别。

- `@Autowired`默认按byType自动装配，而`@Resource`默认byName自动装配。
- `@Autowired`只包含一个参数：required，表示是否开启自动准入，默认是true。而`@Resource`包含七个参数，其中最重要的两个参数是：name 和 type。
- `@Autowired`如果要使用`byName`，需要使用`@Qualifier`一起配合。而`@Resource`如果指定了name，则用byName自动装配，如果指定了type，则用byType自动装配。
- `@Autowired`能够用在：构造器、方法、参数、成员变量和注解上，而`@Resource`能用在：类、成员变量和方法上。
- `@Autowired`是Spring定义的注解，而`@Resource`是JSR-250定义的注解。

此外，它们的装配顺序不同。
<a name="PCkwT"></a>
### `@Autowired`的装配顺序如下：
![2021-08-11-20-09-45-329025.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684049736-d520e90b-e760-481a-9c86-2365920ff928.png#averageHue=%23f9f7f7&clientId=u782184a3-6165-4&from=ui&id=u41bf29a5&originHeight=839&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=224908&status=done&style=shadow&taskId=ua4d10f25-5fd8-4e0d-8f0b-eecea74a669&title=)
<a name="jX0Mx"></a>
### `@Resource`的装配顺序如下：

1. 如果同时指定了name和type：

![2021-08-11-20-09-45-405029.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684049732-d6c7efc8-cda2-47da-9d3d-e85e1a9be5ff.png#averageHue=%23f8f7f5&clientId=u782184a3-6165-4&from=ui&id=g5s0F&originHeight=608&originWidth=760&originalType=binary&ratio=1&rotation=0&showTitle=false&size=72821&status=done&style=shadow&taskId=u3e1ea7df-a481-4a33-b59d-b160b36e73f&title=)

2. 如果指定了name：

![2021-08-11-20-09-45-462024.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684049712-7c239643-1d2e-4f68-b146-f75493ef5670.png#averageHue=%23f9f7f6&clientId=u782184a3-6165-4&from=ui&id=WBUXU&originHeight=594&originWidth=632&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57086&status=done&style=shadow&taskId=uc015aca3-743e-4d5e-bd8e-cb4d026d7cf&title=)

3. 如果指定了type：

![2021-08-11-20-09-45-530026.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684049713-feb5bc9f-db1f-45d5-8e16-8bc54bc27cbc.png#averageHue=%23f9f7f6&clientId=u782184a3-6165-4&from=ui&id=r96ke&originHeight=606&originWidth=708&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61334&status=done&style=shadow&taskId=ua92f3a9b-b701-4d18-a6a9-7c974a481c3&title=)

4. 如果既没有指定name，也没有指定type：

![2021-08-11-20-09-45-606023.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628684049731-1dea74bf-696d-42e7-b03c-87f49c8f369d.png#averageHue=%23f9f6f4&clientId=u782184a3-6165-4&from=ui&id=C2IBL&originHeight=720&originWidth=696&originalType=binary&ratio=1&rotation=0&showTitle=false&size=93576&status=done&style=shadow&taskId=uce31e5c9-d207-4f5a-b8c7-fe2bcbd3396&title=)
