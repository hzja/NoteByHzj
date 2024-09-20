Java Spring SPringBoot
<a name="QuTwS"></a>
## 前言
在实际工作中总是需要在项目启动时做一些初始化的操作，比如初始化线程池、提前加载好加密证书.......<br />有哪些手段在Spring Boot 项目启动的时候做一些事情？方法有很多种，下面介绍几种常见的方法。
<a name="i3Q6T"></a>
## 1、监听容器刷新完成扩展点`ApplicationListener<ContextRefreshedEvent>`
`ApplicationContext`事件机制是观察者设计模式实现的，通过`ApplicationEvent`和`ApplicationListener`这两个接口实现`ApplicationContext`的事件机制。<br />Spring中一些内置的事件如下：

1. `ContextRefreshedEvent`：`ApplicationContext` 被初始化或刷新时，该事件被发布。这也可以在 `ConfigurableApplicationContext`接口中使用 `refresh()` 方法来发生。此处的初始化是指：所有的Bean被成功装载，后处理Bean被检测并激活，所有Singleton Bean 被预实例化，`ApplicationContext`容器已就绪可用。
2. `ContextStartedEvent`：当使用 `ConfigurableApplicationContext `（`ApplicationContext`子接口）接口中的 `start()` 方法启动 `ApplicationContext` 时，该事件被发布。可以调查数据库，或者可以在接受到这个事件后重启任何停止的应用程序。
3. `ContextStoppedEvent`：当使用 `ConfigurableApplicationContext` 接口中的 `stop()` 停止 `ApplicationContext` 时，发布这个事件。可以在接受到这个事件后做必要的清理的工作。
4. `ContextClosedEvent`：当使用 `ConfigurableApplicationContext` 接口中的 `close()` 方法关闭 `ApplicationContext` 时，该事件被发布。一个已关闭的上下文到达生命周期末端；它不能被刷新或重启。
5. `RequestHandledEvent`：这是一个 web-specific 事件，告诉所有 bean HTTP 请求已经被服务。只能应用于使用`DispatcherServlet`的Web应用。在使用Spring作为前端的MVC控制器时，当Spring处理用户请求结束后，系统会自动触发该事件。

好了，了解上面这些内置事件后，可以监听`ContextRefreshedEvent`在Spring Boot 启动时完成一些操作，代码如下：
```java
@Component
public class TestApplicationListener implements ApplicationListener<ContextRefreshedEvent>{
    @Override
    public void onApplicationEvent(ContextRefreshedEvent contextRefreshedEvent) {
        System.out.println(contextRefreshedEvent);
        System.out.println("TestApplicationListener............................");
    }
}
```
<a name="YeXIJ"></a>
### 高级玩法
可以自定事件完成一些特定的需求，比如：邮件发送成功之后，做一些业务处理。
<a name="EyjWi"></a>
#### 自定义`EmailEvent`
代码如下：
```java
public class EmailEvent extends ApplicationEvent{
   private String address;
   private String text;
   public EmailEvent(Object source, String address, String text){
   super(source);
      this.address = address;
      this.text = text;
   }
   public EmailEvent(Object source) {
     super(source);
   }
   //......address和text的setter、getter
}
```
<a name="j0Ia8"></a>
#### 自定义监听器
代码如下：
```java
public class EmailNotifier implements ApplicationListener{
    public void onApplicationEvent(ApplicationEvent event) {
        if (event instanceof EmailEvent) {
            EmailEvent emailEvent = (EmailEvent)event;
            System.out.println("邮件地址：" + emailEvent.getAddress());
            System.our.println("邮件内容：" + emailEvent.getText());
        } else {
            System.our.println("容器本身事件：" + event);
        }
    }
}
```
<a name="BOdNp"></a>
#### 发送邮件后，触发事件
代码如下：
```java
public class SpringTest {
    public static void main(String args[]){
        ApplicationContext context = new ClassPathXmlApplicationContext("bean.xml");
        //创建一个ApplicationEvent对象
        EmailEvent event = new EmailEvent("hello","abc@163.com","This is a test");
        //主动触发该事件
        context.publishEvent(event);
    }
}
```
<a name="dLhhh"></a>
## 2、SpringBoot的`CommandLineRunner`接口
当容器初始化完成之后会调用`CommandLineRunner`中的`run()`方法，同样能够达到容器启动之后完成一些事情。这种方式和`ApplicationListener`相比更加灵活，如下：

- 不同的`CommandLineRunner`实现可以通过`@Order()`指定执行顺序
- 可以接收从控制台输入的参数。

下面自定义一个实现类，代码如下：
```java
@Component
@Slf4j
public class CustomCommandLineRunner implements CommandLineRunner {

    /**
     * @param args 接收控制台传入的参数
     */
    @Override
    public void run(String... args) throws Exception {
        log.debug("从控制台接收参数>>>>"+ Arrays.asList(args));
    }
}
```
运行这个jar，命令如下：
```bash
java -jar demo.jar aaa bbb ccc
```
以上命令中传入了三个参数，分别是aaa、bbb、ccc，这三个参数将会被`run()`方法接收到。如下图：<br />![2021-08-31-14-24-10-358411.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630392545483-50c032da-5b19-4d41-8510-fe7e0f0b43c3.png#clientId=u72addc77-5d66-4&from=ui&id=u68ea0869&originHeight=404&originWidth=1080&originalType=binary&ratio=1&size=73076&status=done&style=shadow&taskId=u58841e03-369a-453b-969e-4e9543e2d76)
<a name="yJ8KC"></a>
### 源码分析
Spring Boot 加载上下文的入口在`org.springframework.context.ConfigurableApplicationContext()`这个方法中，如下图：<br />![2021-08-31-14-24-10-566406.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630392545539-94948a96-c4b3-47a3-802e-40fa1085be1c.png#clientId=u72addc77-5d66-4&from=ui&id=jmFWM&originHeight=809&originWidth=1080&originalType=binary&ratio=1&size=266748&status=done&style=none&taskId=uc6d7e3f1-83db-49a4-8930-fd51d4a35de)<br />调用`CommandLineRunner`在`callRunners(context, applicationArguments);`这个方法中执行，源码如下图：<br />![2021-08-31-14-24-10-698403.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630392545510-7621d397-fe53-485d-9c46-30e3a5329011.png#clientId=u72addc77-5d66-4&from=ui&id=KruvE&originHeight=376&originWidth=1080&originalType=binary&ratio=1&size=105622&status=done&style=none&taskId=u89b29dad-3082-45a8-b78e-42382b630c0)
<a name="jenJ3"></a>
## 3、SpringBoot的`ApplicationRunner`接口
`ApplicationRunner`和`CommandLineRunner`都是Spring Boot 提供的，相对于`CommandLineRunner`来说对于控制台传入的参数封装更好一些，可以通过键值对来获取指定的参数，比如`--version=2.1.0`。<br />此时运行这个jar命令如下：
```bash
java -jar demo.jar --version=2.1.0 aaa bbb ccc
```
以上命令传入了四个参数，一个键值对`version=2.1.0`，另外三个是分别是aaa、bbb、ccc。<br />同样可以通过`@Order()`指定优先级，如下代码：
```java
@Component
@Slf4j
public class CustomApplicationRunner implements ApplicationRunner {
    @Override
    public void run(ApplicationArguments args) throws Exception {
        log.debug("控制台接收的参数：{},{},{}",args.getOptionNames(),args.getNonOptionArgs(),args.getSourceArgs());
    }
}
```
通过以上命令运行，结果如下图：<br />![2021-08-31-14-24-11-141404.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630392565029-67a2244f-e255-4ede-b14a-ebbd48fa8fc5.png#clientId=u72addc77-5d66-4&from=ui&id=u9b12a1b7&originHeight=531&originWidth=1080&originalType=binary&ratio=1&size=241647&status=done&style=none&taskId=u97988621-1a90-474d-ac94-9b8b63fa330)
<a name="PnfaE"></a>
### 源码分析
和`CommandLineRunner`一样，同样在`callRunners()`这个方法中执行，源码如下图：<br />![2021-08-31-14-24-11-270407.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630392564987-49b3a15c-13a3-4c7b-9537-3112508766ff.png#clientId=u72addc77-5d66-4&from=ui&id=pxS8M&originHeight=377&originWidth=1080&originalType=binary&ratio=1&size=100316&status=done&style=none&taskId=u970b919f-a4d6-4608-81f1-d701700f738)
<a name="q63xA"></a>
## 4、`@PostConstruct`注解
前三种针对的是容器的初始化完成之后做的一些事情，`@PostConstruct`这个注解是针对`Bean`的初始化完成之后做一些事情，比如注册一些监听器...<br />`@PostConstruct`注解一般放在Bean的方法上，一旦Bean初始化完成之后，将会调用这个方法，代码如下：
```java
@Component
@Slf4j
public class SimpleExampleBean {

    @PostConstruct
    public void init(){
        log.debug("Bean初始化完成，调用...........");
    }
}
```
<a name="aZvi0"></a>
## 5、`@Bean`注解中指定初始化方法
这种方式和`@PostConstruct`比较类似，同样是指定一个方法在Bean初始化完成之后调用。<br />新建一个Bean，代码如下：
```java
@Slf4j
public class SimpleExampleBean {

    public void init(){
        log.debug("Bean初始化完成，调用...........");
    }
}
```
在配置类中通过`@Bean`实例化这个Bean，不过`@Bean`中的`initMethod`这个属性需要指定初始化之后需要执行的方法，如下：
```java
@Bean(initMethod = "init")
public SimpleExampleBean simpleExampleBean(){
    return new SimpleExampleBean();
}
```
<a name="RHb4P"></a>
## 6、`InitializingBean`接口
`InitializingBean`的用法基本上与`@PostConstruct`一致，只不过相应的`Bean`需要实现`afterPropertiesSet`方法，代码如下：
```java
@Slf4j
@Component
public class SimpleExampleBean implements InitializingBean {

    @Override
    public void afterPropertiesSet()  {
        log.debug("Bean初始化完成，调用...........");
    }
}
```
