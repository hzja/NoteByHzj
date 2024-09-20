JavaSpringBoot<br />用了这么多年的 SpringBoot 那么你知道什么是 SpringBoot 的 web 类型推断吗？<br />估计很多小伙伴都不知道，毕竟平时开发做项目的时候做的都是普通的 web 项目并不需要什么特别的了解，不过抱着学习的心态，来看一下什么是 SpringBoot 的 web 类型推断。
<a name="tZETv"></a>
## SpringBoot 的 web 类型有哪些
既然是web 类型推断，那肯定要知道 SpringBoot 支持哪些类型，然后才能分析是怎样进行类型推断的。<br />根据官方的介绍 SpringBoot 的 web 类型有三种，分别是，NONE、SERVLET 和 REACTIVE，定义在枚举 WebApplicationType 中，这三种类型分别代表了三种含义：

1. NONE：不是一个 web 应用，不需要启动内置的 web 服务器；
2. SERVLET：基于 servlet 的 web 应用，需要启动一个内置的 servlet 服务器；
3. REACTIVE：一个 reactive 的 web 应用，需要启动一个内置的 reactive 服务器；
```java
public enum WebApplicationType {
    NONE,
    SERVLET,
    REACTIVE;
}
```
<a name="Vqvx8"></a>
## web 类型推断
上面提到了 SpringBoot 的三种 web 类型，接下来先通过代码验证一下，然后再分析一下 SpringBoot 是如何进行类型推断的。<br />首先通过在 [https://start.spring.io/](https://start.spring.io/) 快速的构建三种类型的项目，三种类型的项目配置除了依赖不一样之外，其他都一样，如下所示
<a name="tpiSm"></a>
### None web
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672032268028-5c361855-6168-4e09-9e31-1192923e4fd1.png#averageHue=%23f8f8f8&clientId=u6ef458aa-ce97-4&from=paste&height=713&id=uab4d8aa4&originHeight=1783&originWidth=3790&originalType=binary&ratio=1&rotation=0&showTitle=false&size=218478&status=done&style=shadow&taskId=u2bd3158a-fe6c-4568-9609-3b29a323b86&title=&width=1516)<br />下载后的项目文件 pom 中对应的依赖为
```xml
<dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter</artifactId>
</dependency>
```
<a name="oqkOG"></a>
### Servlet web
![Servlet](https://cdn.nlark.com/yuque/0/2022/png/396745/1672032319221-8bf628d7-d0b4-4e97-b3a6-eef2e15a72ff.png#averageHue=%23f7f7f7&clientId=u6ef458aa-ce97-4&from=paste&height=713&id=u1d2e06d1&originHeight=1783&originWidth=3781&originalType=binary&ratio=1&rotation=0&showTitle=true&size=251100&status=done&style=shadow&taskId=u73fb3dee-4775-41c2-987f-06ab393785a&title=Servlet&width=1512.4 "Servlet")<br />下载后的项目文件 pom 中对应的依赖为
```xml
<dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
<a name="Hk3FT"></a>
### Reactive web
![reactive](https://cdn.nlark.com/yuque/0/2022/png/396745/1672032374032-e2b6e47b-de53-41ed-90be-7b0b410f8ddd.png#averageHue=%23f7f7f7&clientId=u6ef458aa-ce97-4&from=paste&height=713&id=ub4ede88a&originHeight=1783&originWidth=3789&originalType=binary&ratio=1&rotation=0&showTitle=true&size=242389&status=done&style=shadow&taskId=u2b0ab4f7-36d1-4bf2-9a37-adbed5a5bfc&title=reactive&width=1515.6 "reactive")<br />下载后的项目文件 pom 中对应的依赖为
```xml
<dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-webflux</artifactId>
</dependency>
```
接下来依次启动三个项目看看有什么区别，
<a name="zEuOw"></a>
### 启动 None web
![none-web](https://cdn.nlark.com/yuque/0/2022/png/396745/1672032943370-f5de8662-4de0-4786-9e8d-239cf072f9fe.png#averageHue=%23f9f9f9&clientId=u6ef458aa-ce97-4&from=paste&height=412&id=u665853f2&originHeight=1031&originWidth=3763&originalType=binary&ratio=1&rotation=0&showTitle=true&size=254642&status=done&style=shadow&taskId=uee58e0f1-5b84-405d-97e8-69c23b0f8de&title=none-web&width=1505.2 "none-web")<br />通过启动日志可以看到，在 None web 类型下，应用启动运行后就自动关闭了，并没有启动内置的 web 服务器，也没有监听任何端口。接下来看看其他两种类型 web 的启动日志都是怎么样的。
<a name="Q9sqj"></a>
### 启动 Servlet web
![servelt-web](https://cdn.nlark.com/yuque/0/2022/png/396745/1672032984626-b01b666c-e040-4499-b60f-cad8334a4089.png#averageHue=%23f9f8f8&clientId=u6ef458aa-ce97-4&from=paste&height=426&id=ua3c3d510&originHeight=1066&originWidth=3727&originalType=binary&ratio=1&rotation=0&showTitle=true&size=274552&status=done&style=shadow&taskId=u197e686a-1f22-462c-9846-100ae00ac28&title=servelt-web&width=1490.8 "servelt-web")<br />通过启动日志可以看到这里启动了内置的 Tomcat Servlet 服务器，监听了 8080 端口，应用程序并不会像 None 类型一样，启动后就自动关闭。
<a name="Qcp4l"></a>
### 启动 Reactive web
![reactive-web](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033041188-e1ecdae1-37b0-4d94-8ddd-2e67af965696.png#averageHue=%23fbfafa&clientId=u6ef458aa-ce97-4&from=paste&height=410&id=uf085b747&originHeight=1024&originWidth=3359&originalType=binary&ratio=1&rotation=0&showTitle=true&size=161829&status=done&style=shadow&taskId=u3c151253-488d-4c31-a0a7-64379e349c2&title=reactive-web&width=1343.6 "reactive-web")<br />通过启动日志可以看到，这里启动了内置的 Netty 服务器，并监听在 8080 端口上（如果启动失败记得把上面 servlet web 关闭，不然端口会冲突）。<br />三种类型的服务都成功启动了，那么接下来的问题就是 SpringBoot 是如何判断出该使用哪种类型的呢？<br />这三个服务只有依赖不一样，很明显肯定和依赖有关系，接下来就来研究一下 SpringBoot 是如何实现的。
<a name="UFl4s"></a>
## SpringBoot Web 类型推断原理
在 main 方法中点击 run 方法，
```java
public static ConfigurableApplicationContext run(Class<?>[] primarySources, String[] args) {
    return new SpringApplication(primarySources).run(args);
}
```
在构造函数中可以看到其中有这么一行 `this.webApplicationType = WebApplicationType.deduceFromClasspath();`根据属性名称可以推断，web 类型就是根据 `WebApplicationType.deduceFromClasspath();` 这个静态方法来判断的。接下来看下这个方法的细节。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033179821-b4199977-ef6e-4266-a7da-01248e973008.png#averageHue=%23fbfbfa&clientId=u6ef458aa-ce97-4&from=paste&height=486&id=u392aed82&originHeight=1215&originWidth=3489&originalType=binary&ratio=1&rotation=0&showTitle=false&size=475916&status=done&style=shadow&taskId=ufba785f4-074e-4fc8-b5e4-68282ebc4e7&title=&width=1395.6)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033218600-236df570-be4c-45af-b083-4fe991334e0d.png#averageHue=%23fcfcfb&clientId=u6ef458aa-ce97-4&from=paste&height=678&id=u8981f8e6&originHeight=1695&originWidth=3616&originalType=binary&ratio=1&rotation=0&showTitle=false&size=476801&status=done&style=shadow&taskId=ub5751ba8-0646-4fa4-a490-d8aa5f7e434&title=&width=1446.4)<br />如上图所示，可以看到 SpringBoot 底层是通过 `ClassUtils.isPresent()` 方法来判断对应的 web 类型类是否存在来判断 web 类型的。<br />在前类路径下面如果当 `org.springframework.web.reactive.DispatcherHandler` 存在而且 `org.springframework.web.servlet.DispatcherServlet` 和 `org.glassfish.jersey.servlet.ServletContainer` 都不存在的时候说明当前应用 web 类型为 Reactive。<br />当 javax.servlet.Servlet 和 `org.springframework.web.context.ConfigurableWebApplicationContext` 任何一个不存在的时候，就说明当前应用是 None 类型非 web 应用。否则当前应用就为 Servlet 类型。<br />再看这个 `ClassUtils.isPresent()` 方法，可以发现底层是通过 className 在类路径上加载对应的类，如果存在则返回 true，如果不存在则返回 false。![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033255840-ca950974-af4e-4dcc-8363-e7b31c8f833f.png#averageHue=%23fdfcfa&clientId=u6ef458aa-ce97-4&from=paste&height=242&id=u09984973&originHeight=606&originWidth=3503&originalType=binary&ratio=1&rotation=0&showTitle=false&size=130736&status=done&style=shadow&taskId=u68fbde56-6193-4caa-9ea4-ae8c188c1db&title=&width=1401.2)<br />因此这也解释了为什么在 pom 文件中只要加入对应的依赖就可以直接得到相应的 web 类型了，因为当在 pom 中加入相应的依赖过后，类路径里面就存在了前面判断的对应的类，再通过 `ClassUtils.isPresent()` 就判断出来当前应用属于那种 web 类型了。
<a name="TiaAA"></a>
## 内置服务器是如何创建的
知道了 SpringBoot 是如何进行 web 类型推断的，那么接下来一个问题就是 SpringBoot 是如何根据 web 类型进行相应内置 web 服务器的启动的呢？这里以 Reactive web 为例进行调试追踪。<br />首先在 SpringApplication 的 run 方法 `createApplicationContext()` 下一行打断点，可以发现创建成功的 context 类型为 `AnnotationConfigReactiveWebServerApplicationContext ` 很明显在这一步的时候就已经根据类型推断得到了当前的应用 web 类型为 Reactive，并且根据 web 类型创建出了对应的 `ApplicationContext`。<br />![reactive-web](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033397917-eece77a1-be06-4f0d-8668-6d0ac0fd1f06.png#averageHue=%23f7f5f4&clientId=u6ef458aa-ce97-4&from=paste&height=563&id=u48bd0a21&originHeight=1407&originWidth=3830&originalType=binary&ratio=1&rotation=0&showTitle=true&size=614433&status=done&style=shadow&taskId=uefe48d46-054a-494d-9bfa-7b461ed9f2d&title=reactive-web&width=1532 "reactive-web")<br />紧接着进入 `org.springframework.boot.SpringApplication#refreshContext` 方法，最后可以进入到 `org.springframework.boot.web.reactive.context.ReactiveWebServerApplicationContext#refresh` 方法中，因为 `AnnotationConfigReactiveWebServerApplicationContext`   继承了 `ReactiveWebServerApplicationContext`。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033562864-45d53ac6-5f9d-4cf4-bf98-0f25f70aea2d.png#averageHue=%23fcfbfa&clientId=u6ef458aa-ce97-4&from=paste&height=141&id=u713489a5&originHeight=353&originWidth=2954&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62331&status=done&style=shadow&taskId=uee669990-5d51-49a8-8b5f-67e90d91468&title=&width=1181.6)<br />继续通过引用关系，可以找到 `org.springframework.boot.web.reactive.context.ReactiveWebServerApplicationContext#onRefresh` 方法，而在这个方法里面就会发现了如下代码，此处就会创建一个 webServer。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033642048-29bfd8de-a825-4cca-8f63-e9fcf9e7214a.png#averageHue=%23fdfdfd&clientId=u6ef458aa-ce97-4&from=paste&height=242&id=ubd1e6b67&originHeight=605&originWidth=3100&originalType=binary&ratio=1&rotation=0&showTitle=false&size=100770&status=done&style=shadow&taskId=u33bded63-71df-435d-8d3d-64c8eddb26a&title=&width=1240)<br />具体创建的方法在 `WebServerManager` 里面，跟着继续往下找可以找到 `createHttpServer()` 方法，在 `createHttpServer()` 方法中就创建了 HttpServer 并且绑定了默认的端口 8080。具体过程，如下几张接入所示，感兴趣的可以自行跟踪 debug，至此一个 Reactive 内置服务器就创建成功了，同样的 Servlet 服务器也是类似的。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033717612-bfa5046b-ab97-4fa2-a357-702a78113016.png#averageHue=%23faf9f7&clientId=u6ef458aa-ce97-4&from=paste&height=394&id=ua4125999&originHeight=984&originWidth=3566&originalType=binary&ratio=1&rotation=0&showTitle=false&size=337947&status=done&style=shadow&taskId=ub9984e37-0719-401f-93b9-63ebb860c98&title=&width=1426.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672033980965-9c4ebc81-961e-466a-9d03-124e693dc516.png#averageHue=%23faf9f6&clientId=u6ef458aa-ce97-4&from=paste&height=174&id=u60bbd78c&originHeight=436&originWidth=3506&originalType=binary&ratio=1&rotation=0&showTitle=false&size=135596&status=done&style=shadow&taskId=ua25d413c-b98a-4977-9599-ae6a6278028&title=&width=1402.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672034033604-cd0e54af-9571-4660-b3cd-081611334fdf.png#averageHue=%23fcfbdf&clientId=u6ef458aa-ce97-4&from=paste&height=305&id=uddd19614&originHeight=763&originWidth=2656&originalType=binary&ratio=1&rotation=0&showTitle=false&size=208961&status=done&style=shadow&taskId=ucb0d5052-2fe1-4fc1-a4f9-31e35432d29&title=&width=1062.4)![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672034058091-dd5ad486-be04-48d5-9f93-ea23adfa105f.png#averageHue=%23faf9f6&clientId=u6ef458aa-ce97-4&from=paste&height=193&id=ue66213c7&originHeight=482&originWidth=3481&originalType=binary&ratio=1&rotation=0&showTitle=false&size=123394&status=done&style=shadow&taskId=u3757447a-832c-474d-ace6-7de0c30466c&title=&width=1392.4)![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672034083169-c198a5a4-f33b-446b-a94f-431c42c2adb1.png#averageHue=%23fbfaf9&clientId=u6ef458aa-ce97-4&from=paste&height=494&id=u258abd4b&originHeight=1235&originWidth=3502&originalType=binary&ratio=1&rotation=0&showTitle=false&size=296654&status=done&style=shadow&taskId=ua15a5c69-a7de-494c-9fd0-b5677777e59&title=&width=1400.8)![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1672034130783-6687c374-ff5e-4621-b0c4-fa5bd7bf9f2c.png#averageHue=%23fcfbf7&clientId=u6ef458aa-ce97-4&from=paste&height=69&id=uba64f085&originHeight=173&originWidth=3213&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22678&status=done&style=shadow&taskId=uec19c902-dce1-4dcf-a47b-69c36399ae1&title=&width=1285.2)
