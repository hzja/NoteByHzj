Java SpringBoot
<a name="msZ5H"></a>
### 方式一  注解`@PostConstruct`
```java
import com.example.javautilsproject.service.AutoMethodDemoService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
 
import javax.annotation.PostConstruct;
 
/**
 * springboot静态方法获取 bean 的三种方式(一)
 */
@Component
public class StaticMethodGetBean_1 {
 
    @Autowired
    private AutoMethodDemoService autoMethodDemoService;
 
    @Autowired
    private static AutoMethodDemoService staticAutoMethodDemoService;
 
    @PostConstruct
    public void init() {
        staticAutoMethodDemoService = autoMethodDemoService;
    }
 
    public static String getAuthorizer() {
        return staticAutoMethodDemoService.test();
    }
}
```
注解`@PostConstruct`说明<br />`PostConstruct` 注释用于在依赖关系注入完成之后需要执行的方法上，以执行任何初始化。此方法必须在将类放入服务之前调用。<br />支持依赖关系注入的所有类都必须支持此注释。即使类没有请求注入任何资源，用 `PostConstruct` 注释的方法也必须被调用。只有一个方法可以用此注释进行注释。<br />应用 `PostConstruct` 注释的方法必须遵守以下所有标准：

- 该方法不得有任何参数，除非是在 EJB 拦截器 (interceptor) 的情况下，根据 EJB 规范的定义，在这种情况下它将带有一个 InvocationContext 对象 ；
- 该方法的返回类型必须为 `void`；
- 该方法不得抛出已检查异常；
- 应用 `PostConstruct` 的方法可以是 `public`、`protected`、`package private` 或 `private`；
- 除了应用程序客户端之外，该方法不能是 static；
- 该方法可以是 `final`；
- 如果该方法抛出未检查异常，那么不得将类放入服务中，除非是能够处理异常并可从中恢复的 EJB。
<a name="mfoVu"></a>
### 方式二  启动类`ApplicationContext`
实现方式：在SpringBoot的启动类中，定义`static`变量`ApplicationContext`，利用容器的`getBean`方法获得依赖对象
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ConfigurableApplicationContext;

@SpringBootApplication
public class Application {
    public static ConfigurableApplicationContext ac;
    public static void main(String[] args) {
        ac = SpringApplication.run(Application.class, args);
    }
}
```
调用方式
```java
@RestController
public class TestController {
    /**
     * 方式二
     */
    @GetMapping("test2")
    public void method_2() {
        AutoMethodDemoService methodDemoService = Application.ac.getBean(AutoMethodDemoService.class);
        String test2 = methodDemoService.test2();
        System.out.println(test2);
    }
}
```
<a name="cD6kH"></a>
### 方式三 手动注入`ApplicationContext`
手动注入`ApplicationContext`
```java
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Component;
 
 
/**
 * springboot静态方法获取 bean 的三种方式(三)
 */
@Component
public class StaticMethodGetBean_3<T> implements ApplicationContextAware {
    private static ApplicationContext applicationContext;
    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        StaticMethodGetBean_3.applicationContext = applicationContext;
    }
 
    public static <T> T  getBean(Class<T> clazz) {
        return applicationContext != null?applicationContext.getBean(clazz):null;
    }
}
```
调用方式
```java
/**
 * 方式三
 */
@Test
public void method_3() {
    AutoMethodDemoService autoMethodDemoService = StaticMethodGetBean_3.getBean(AutoMethodDemoService.class);
    String test3 = autoMethodDemoService.test3();
    System.out.println(test3);
}
```
