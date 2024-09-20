JavaSpring<br />本文详细解析了 Spring 的内置作用域，包括 Singleton、Prototype、Request、Session、Application 和 WebSocket 作用域，并通过实例讲解了它们在实际开发中的应用。特别是 Singleton 和 Prototype 作用域，深入讨论了它们的定义、用途以及如何处理相关的线程安全问题。通过阅读本文，读者可以更深入地理解 Spring 作用域，并在实际开发中更有效地使用
<a name="Rzejl"></a>
## **1、Spring 的内置作用域**
来看看 Spring 内置的作用域类型。在 5.x 版本中，Spring 内置了六种作用域：

- **singleton**：在 IOC 容器中，对应的 Bean 只有一个实例，所有对它的引用都指向同一个对象。这种作用域非常适合对于无状态的 Bean，比如工具类或服务类。
- **prototype**：每次请求都会创建一个新的 Bean 实例，适合对于需要维护状态的 Bean。
- **request**：在 Web 应用中，为每个 HTTP 请求创建一个 Bean 实例。适合在一个请求中需要维护状态的场景，如跟踪用户行为信息。
- **session**：在 Web 应用中，为每个 HTTP 会话创建一个 Bean 实例。适合需要在多个请求之间维护状态的场景，如用户会话。
- **application**：在整个 Web 应用期间，创建一个 Bean 实例。适合存储全局的配置数据等。
- **websocket**：在每个 WebSocket 会话中创建一个 Bean 实例。适合 WebSocket 通信场景。

需要重点学习两种作用域：singleton 和 prototype。在大多数情况下 singleton 和 prototype 这两种作用域已经足够满足需求。
<a name="uvqUt"></a>
## **2、singleton 作用域**
<a name="DAVmR"></a>
### **2.1 singleton 作用域的定义和用途**
Singleton 是 Spring 的默认作用域。在这个作用域中，Spring 容器只会创建一个实例，所有对该 bean 的请求都将返回这个唯一的实例。<br />例如，定义一个名为 Plaything 的类，并将其作为一个 bean：
```java
@Component
public class Plaything {
    public Plaything() {
        System.out.println("Plaything constructor run ...");
    }
}
```
在这个例子中，Plaything 是一个 singleton 作用域的 bean。无论在应用中的哪个地方请求这个 bean，Spring 都会返回同一个 Plaything 实例。<br />下面的例子展示了如何创建一个单实例的 Bean：
```java
package com.example.demo.bean;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Kid {
    private Plaything plaything;
    @Autowired
    public void setPlaything(Plaything plaything) {
        this.plaything = plaything;
    }
    public Plaything getPlaything() {
        return plaything;
    }
}
```
```java
package com.example.demo.bean;

import org.springframework.stereotype.Component;

@Component
public class Plaything {
    public Plaything() {
        System.out.println("Plaything constructor run ...");
    }
}
```
这里可以在 Plaything 类加上 `@Scope (BeanDefinition.SCOPE_SINGLETON)`，但是因为是默认作用域是 Singleton，所以没必要加。
```java
package com.example.demo.configuration;

import com.example.demo.bean.Kid;
import com.example.demo.bean.Plaything;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class BeanScopeConfiguration {
    @Bean
    public Kid kid1(Plaything plaything1) {
        Kid kid = new Kid();
        kid.setPlaything(plaything1);
        return kid;
    }
    @Bean
    public Kid kid2(Plaything plaything2) {
        Kid kid = new Kid();
        kid.setPlaything(plaything2);
        return kid;
    }
}
```
```java
package com.example.demo.application;

import com.example.demo.bean.Kid;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.ComponentScan;

@SpringBootApplication
@ComponentScan("com.example")
public class DemoApplication {
    public static void main(String[] args) {
        ApplicationContext context = new AnnotationConfigApplicationContext(DemoApplication.class);
        context.getBeansOfType(Kid.class).forEach((name, kid) -> {
            System.out.println(name + " : " + kid.getPlaything());
        });
    }
}
```
在 Spring IoC 容器的工作中，扫描过程只会创建 bean 的定义，真正的 bean 实例是在需要注入或者通过 getBean 方法获取时才会创建。这个过程被称为 bean 的初始化。<br />这里运行 `ctx.getBeansOfType (Kid.class).forEach ((name, kid)` -> `System.out.println (name + ":" + kid.getPlaything ()));` 时，Spring IoC 容器会查找所有的 Kid 类型的 bean 定义，然后为每一个找到的 bean 定义创建实例（如果这个 bean 定义还没有对应的实例），并注入相应的依赖。<br />运行结果：
```
kid : com.example.demo.bean.Plaything@295cf707
kid1 : com.example.demo.bean.Plaything@295cf707
kid2 : com.example.demo.bean.Plaything@295cf707
```
三个 Kid 的 Plaything bean 是相同的，说明默认情况下 Plaything 是一个单例 bean，整个 Spring 应用中只有一个 Plaything bean 被创建。<br />为什么会有 3 个 kid？

1. **Kid**：这个是通过在 Kid 类上标注的 `@Component` 注解自动创建的。Spring 在扫描时发现这个注解，就会自动在 IOC 容器中注册这个 bean。这个 Bean 的名字默认是将类名的首字母小写 kid。
2. **kid1**：在 `BeanScopeConfiguration` 中定义，通过 kid1 (Plaything plaything1) 方法创建，并且注入了 plaything1。
3. **kid2**：在 `BeanScopeConfiguration` 中定义，通过 kid2 (Plaything plaything2) 方法创建，并且注入了 plaything2。
<a name="ci2l0"></a>
### **2.2 singleton 作用域线程安全问题**
需要注意的是，虽然 singleton Bean 只会有一个实例，但 Spring 并不会解决其线程安全问题，开发者需要根据实际场景自行处理。<br />通过一个代码示例来说明在多线程环境中出现 singleton Bean 的线程安全问题。<br />首先创建一个名为 Counter 的 singleton Bean，这个 Bean 有一个 count 变量，提供 `increment` 方法来增加 count 的值：
```java
package com.example.demo.bean;

import org.springframework.stereotype.Component;

@Component
public class Counter {
    private int count = 0;
    public int increment() {
        return ++count;
    }
}
```
然后创建一个名为 CounterService 的 singleton Bean，这个 Bean 依赖于 Counter，在 `increaseCount` 方法中，调用 `counter.increment` 方法：
```java
package com.example.demo.service;

import com.example.demo.bean.Counter;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class CounterService {
    @Autowired
    private final Counter counter;
    public void increaseCount() {
        counter.increment();
    }
}
```
在多线程环境中调用 `counterService.increaseCount` 方法时，就可能出现线程安全问题。因为 `counter.increment` 方法并非线程安全，多个线程同时调用此方法可能会导致 count 值出现预期外的结果。<br />要解决这个问题，需要使 `counter.increment` 方法线程安全。<br />这里可以使用原子变量，在 `Counter` 类中，可以使用 `AtomicInteger` 来代替 int 类型的 count，因为 `AtomicInteger` 类中的方法是线程安全的，且其性能通常优于 `synchronized` 关键字。
```java
package com.example.demo.bean;

import org.springframework.stereotype.Component;
import java.util.concurrent.atomic.AtomicInteger;

@Component
public class Counter {
    private AtomicInteger count = new AtomicInteger(0);
    public int increment() {
        return count.incrementAndGet();
    }
}
```
尽管优化后已经使 Counter 类线程安全，但在设计 Bean 时，应该尽可能地减少可变状态。这是因为可变状态使得并发编程变得复杂，而无状态的 Bean 通常更容易理解和测试。<br />**什么是无状态的 Bean 呢？** 如果一个 Bean 不持有任何状态信息，也就是说，同样的输入总是会得到同样的输出，那么这个 Bean 就是无状态的。反之，则是有状态的 Bean。<br />欢迎关注公众号：SpringForAll社区（spring4all.com），专注分享关于Spring的一切！回复“加群”还可加入Spring技术交流群！
<a name="QgRG5"></a>
## **3、prototype 作用域**
<a name="KzGV2"></a>
### **3.1 prototype 作用域的定义和用途**
在 prototype 作用域中，Spring 容器会为每个请求创建一个新的 bean 实例。<br />例如，定义一个名为 Plaything 的类，并将其作用域设置为 prototype：
```java
package com.example.demo.bean;

import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

@Component
@Scope(BeanDefinition.SCOPE_PROTOTYPE)
public class Plaything {
    public Plaything() {
        System.out.println("Plaything constructor run ...");
    }
}
```
在这个例子中，Plaything 是一个 prototype 作用域的 bean。每次请求这个 bean，Spring 都会创建一个新的 Plaything 实例。<br />只需要修改上面的 Plaything 类，其他的类不用动。<br />打印结果：
```
kid : com.example.demo.bean.Plaything@7a3793c7
kid1 : com.example.demo.bean.Plaything@42b3b079
kid2 : com.example.demo.bean.Plaything@651aed93
```
这个 `@Scope (BeanDefinition.SCOPE_PROTOTYPE)` 可以写成 `@Scope ("prototype")`，按照规范，还是利用已有的常量比较好。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1688695838545-1f1f9011-170d-4601-a50b-4a321c6c41d3.png#averageHue=%23fbfbfb&clientId=ubfa6aed9-ef45-4&from=paste&height=636&id=u06856fdf&originHeight=1589&originWidth=2839&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=323187&status=done&style=none&taskId=u8a227f72-4967-47d1-ae92-52126054ff8&title=&width=1135.6)
<a name="kYO3f"></a>
### **3.2 prototype 作用域在开发中的例子**
在 Excel 多线程上传的时候用到过这个，当时是 EasyExcel 框架，给一部分关键代码展示一下如何在 Spring 中使用 prototype 作用域来处理多线程环境下的任务（实际业务会更复杂），大家可以对比，如果用 prototype 作用域和使用 new 对象的形式在实际开发中有什么区别。<br />**使用 prototype 作用域的例子**
```java
@Resource
private ApplicationContext context;

@PostMapping("/user/upload")
public ResultModel upload(@RequestParam("multipartFile") MultipartFile multipartFile) {
    ......
    ExecutorService es = new ThreadPoolExceutor(10, 16, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<>(2000), new ThreadPoolExecutor.CallerRunPolicy());
    ......
    EasyExcel.read(multipartFile.getInputStream(), UserDataUploadVO.class, 
    new PageReadListener<UserDataUploadVO>(dataList ->{
        ......
        // 多线程处理上传excel数据
        Future<?> future = es.submit(context.getBean(AsyncUploadHandler.class, user, dataList, errorCount));
        ......
    })).sheet().doRead();
    ......
}
```
**AsyncUploadHandler.java**
```java
@Component
@Scope(BeanDefinition.SCOPE_PROTOTYPE)
public class AsyncUploadHandler implements Runnable {
    private User user;

    private List<UserDataUploadVO> dataList;

    private AtomicInteger errorCount;

    @Resource
    private RedisService redisService;

    ......

    @Resource
    private CompanyManagementMapper companyManagementMapper;
    public AsyncUploadHandler(user, List<UserDataUploadVO> dataList, AtomicInteger errorCount) {
        this.user = user;
        this.dataList = dataList;
        this.errorCount = errorCount;
    }
    @Override
    public void run() {
        ......
    }

    ......
}
```
`AsyncUploadHandler` 类是一个 prototype 作用域的 bean，它被用来处理上传的 Excel 数据。由于并发上传的每个任务可能需要处理不同的数据，并且可能需要在不同的用户上下文中执行，因此每个任务都需要有自己的 `AsyncUploadHandler` bean。这就是为什么需要将 `AsyncUploadHandler` 定义为 prototype 作用域的原因。<br />由于 `AsyncUploadHandler` 是由 Spring 管理的，可以直接使用 `@Resource` 注解来注入其他的 bean，例如 `RedisService` 和 `CompanyManagementMapper`。<br />把 `AsyncUploadHandler` 交给 Spring 容器管理，里面依赖的容器对象可以直接用 `@Resource` 注解注入。如果采用 new 出来的对象，那么这些对象只能从外面注入好了再传入进去。<br />**不使用 **`**prototype**`** 作用域改用 new 对象的例子**
```java
@PostMapping("/user/upload")
public ResultModel upload(@RequestParam("multipartFile") MultipartFile multipartFile) {
    ......
    ExecutorService es = new ThreadPoolExceutor(10, 16, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<>(2000), new ThreadPoolExecutor.CallerRunPolicy());
    ......
    EasyExcel.read(multipartFile.getInputStream(), UserDataUploadVO.class, 
    new PageReadListener<UserDataUploadVO>(dataList ->{
        ......
        // 多线程处理上传excel数据
        Future<?> future = es.submit(new AsyncUploadHandler(user, dataList, errorCount, redisService, companyManagementMapper));
        ......
    })).sheet().doRead();
    ......
}
```
**AsyncUploadHandler.java**
```java
public class AsyncUploadHandler implements Runnable {
    private User user;

    private List<UserDataUploadVO> dataList;

    private AtomicInteger errorCount;

    private RedisService redisService;

    private CompanyManagementMapper companyManagementMapper;

    ......
    public AsyncUploadHandler(user, List<UserDataUploadVO> dataList, AtomicInteger errorCount, 
                              RedisService redisService, CompanyManagementMapper companyManagementMapper) {
        this.user = user;
        this.dataList = dataList;
        this.errorCount = errorCount;
        this.redisService = redisService;
        this.companyManagementMapper = companyManagementMapper;
    }
    @Override
    public void run() {
        ......
    }

    ......
}
```
如果直接新建 `AsyncUploadHandler` 对象，则需要手动传入所有的依赖，这会使代码变得更复杂更难以管理，而且还需要手动管理 `AsyncUploadHandler` 的生命周期。
<a name="FlG16"></a>
## **4、request 作用域（了解）**
request 作用域：Bean 在一个 HTTP 请求内有效。当请求开始时，Spring 容器会为每个新的 HTTP 请求创建一个新的 Bean 实例，这个 Bean 在当前 HTTP 请求内是有效的，请求结束后，Bean 就会被销毁。如果在同一个请求中多次获取该 Bean，就会得到同一个实例，但是在不同的请求中获取的实例将会不同。
```java
@Component
@Scope(value = WebApplicationContext.SCOPE_REQUEST, proxyMode = ScopedProxyMode.TARGET_CLASS)
public class RequestScopedBean {
    // 在一次Http请求内共享的数据
    private String requestData;
    public void setRequestData(String requestData) {
        this.requestData = requestData;
    }
    public String getRequestData() {
        return this.requestData;
    }
}
```
上述 Bean 在一个 HTTP 请求的生命周期内是一个单例，每个新的 HTTP 请求都会创建一个新的 Bean 实例。
<a name="LDAcz"></a>
## **5、session 作用域（了解）**
session 作用域：Bean 是在同一个 HTTP 会话（Session）中是单例的。也就是说，从用户登录开始，到用户退出登录（或者 Session 超时）结束，这个过程中，不管用户进行了多少次 HTTP 请求，只要是在同一个会话中，都会使用同一个 Bean 实例。
```java
@Component
@Scope(value = WebApplicationContext.SCOPE_SESSION, proxyMode = ScopedProxyMode.TARGET_CLASS)
public class SessionScopedBean {
    // 在一个Http会话内共享的数据
    private String sessionData;
    public void setSessionData(String sessionData) {
        this.sessionData = sessionData;
    }
    public String getSessionData() {
        return this.sessionData;
    }
}
```
这样的设计对于存储和管理会话级别的数据非常有用，例如用户的登录信息、购物车信息等。因为它们是在同一个会话中保持一致的，所以使用 session 作用域的 Bean 可以很好地解决这个问题。<br />但是实际开发中没人这么干，会话 id 都会存在数据库，根据会话 id 就能在各种表中获取数据，避免频繁查库也是把关键信息序列化后存在 Redis。
<a name="rpMNR"></a>
## **6、application 作用域（了解）**
application 作用域：在整个 Web 应用的生命周期内，Spring 容器只会创建一个 Bean 实例。这个 Bean 在 Web 应用的生命周期内都是有效的，当 Web 应用停止后，Bean 就会被销毁。
```java
@Component
@Scope(value = WebApplicationContext.SCOPE_APPLICATION, proxyMode = ScopedProxyMode.TARGET_CLASS)
public class ApplicationScopedBean {
    // 在整个Web应用的生命周期内共享的数据
    private String applicationData;
    public void setApplicationData(String applicationData) {
        this.applicationData = applicationData;
    }
    public String getApplicationData() {
        return this.applicationData;
    }
}
```
如果在一个 application 作用域的 Bean 上调用 setter 方法，那么这个变更将对所有用户和会话可见。后续对这个 Bean 的所有调用（包括 getter 和 setter）都将影响到同一个 Bean 实例，后面的调用会覆盖前面的状态。
<a name="OdHcv"></a>
## **7、websocket 作用域（了解）**
websocket 作用域：Bean 在每一个新的 WebSocket 会话中都会被创建一次，就像 session 作用域的 Bean 在每一个 HTTP 会话中都会被创建一次一样。这个 Bean 在整个 WebSocket 会话内都是有效的，当 WebSocket 会话结束后，Bean 就会被销毁。
```java
@Component
@Scope(value = "websocket", proxyMode = ScopedProxyMode.TARGET_CLASS)
public class WebSocketScopedBean {
    // 在一个WebSocket会话内共享的数据
    private String socketData;
    public void setSocketData(String socketData) {
        this.socketData = socketData;
    }
    public String getSocketData() {
        return this.socketData;
    }
}
```
上述 Bean 在一个 WebSocket 会话的生命周期内是一个单例，每个新的 WebSocket 会话都会创建一个新的 Bean 实例。<br />这个作用域需要 Spring Websocket 模块支持，并且应用需要配置为使用 websocket。
