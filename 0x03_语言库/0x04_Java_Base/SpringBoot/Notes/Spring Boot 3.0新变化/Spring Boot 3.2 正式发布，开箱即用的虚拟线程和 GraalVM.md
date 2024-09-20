Spring Boot 3.2 已经正式发布了，用 Java 21、GraalVM 和虚拟线程来尝试一下。<br />Spring Boot 3.2 支持：

- Java 21
- 虚拟线程
- 原生镜像（自 2022 年 11 月 Spring Boot 3.0 发布以来，Spring Boot 已在生产环境中支持 GraalVM 原生镜像）
<a name="OXGh7"></a>
### Java 21
2023 年 9 月 19 日发布的 Java 21，Spring Boot 3.2 已经做到完全支持了。<br />正如所声明的那样，Java 21 提供了数千项性能、稳定性和安全性改进，包括平台增强功能，可帮助开发人员提高生产力并推动整个组织的创新和增长。
<a name="gci4m"></a>
### 虚拟线程
更重要的更新之一是虚拟线程，这是 Project Loom 提供的功能。不打算深入细节，官方 JEP 提供了很好的解释：[https://openjdk.org/jeps/444](https://openjdk.org/jeps/444)
<a name="dbCux"></a>
### GraalVM 和本机镜像
GraalVM 是一种高性能 JDK，可以使用替代的即时 (JIT) 编译器来加快 Java 和基于 JVM 的应用程序的性能。<br />Native Image 是一种提前将 Java 代码编译为独立可执行文件（称为本机映像）的技术。该可执行文件包括应用程序类、其依赖项中的类、运行时库类以及来自 JDK 的静态链接本机代码。<br />它不在 Java VM 上运行，但包含来自不同运行时系统的必要组件，如内存管理、线程调度等。与 JVM 相比，生成的程序具有更快的启动时间和更低的运行时内存开销。
<a name="ZRzhd"></a>
### 尝鲜一下
从安装 Java 21.0.1 graal 开始，最简单的方法是使用SDKMAN 并将其指定机器的默认 Java 版本：

- sdk install java 21.0.1-graal
- sdk default java 21.0.1-graal

另一种安装方法是手动下载：[https://www.graalvm.org/downloads/](https://www.graalvm.org/downloads/)<br />使用Spring Initializr页面创建一个新的Spring Boot项目，使用 Spring Boot 3.2.0、Java 21、Gradle-Groovy以及Spring Web和GraalVM本地支持依赖项。<br />要在 Spring Boot 3.2 中启用虚拟线程，只需在 application.yml 或 application.properties 文件中设置一个属性：
```yaml
spring.threads.virtual.enabled：true
```
这个配置起到的作用：

- Tomcat 将使用虚拟线程来处理 HTTP 请求。这意味着处理 Web 请求的应用程序代码（例如控制器中的方法）将在虚拟线程上运行。
- 调用`@Async`方法时，Spring MVC 的异步请求处理和 Spring WebFlux 的阻塞执行支持现在将利用虚拟线程
- 标记有`@Scheduled`的方法将在虚拟线程上运行

因此，尝试使用这 3 个集成来实现虚拟线程。<br />此外，一些特定的集成将在虚拟线程上工作，例如 RabbitMQ/Kafka 监听器，以及 Spring Data Redis/Apache pulsar 相关的集成。但这些集成超出了本文的范围，有兴趣的可以参考 Spring Boot 3.2 官方示例。
<a name="rm78h"></a>
### 代码
<a name="br9KX"></a>
#### 1、对于 Tomcat 传入的 HTTP 请求，创建一个简单的控制器：
```java
@RestController
@RequestMapping("/test")
public class TestController {
    private static final Logger log = LoggerFactory.getLogger(TestController.class);

    @GetMapping
    public void test() {
        log.info("Rest controller method has been called {}", Thread.currentThread());
    }
}
```
<a name="D29mx"></a>
#### 2、异步任务
在应用程序启动时调用其“run”方法
```java
@Component
public class AsyncTaskExecutorService {
    private static final Logger log = LoggerFactory.getLogger(AsyncTaskExecutorService.class);

    @Async
    public void run() {
        log.info("Async task method has been called {}", Thread.currentThread());
    }
}
```
<a name="wuVD2"></a>
#### 3、Scheduled 定时任务
一个简单的方法，每 15 秒调用一次
```java
@Component
public class SchedulerService {
    private static final Logger log = LoggerFactory.getLogger(SchedulerService.class);

    @Scheduled(fixedDelayString = "15000")
    public void run() {
        log.info("Scheduled method has been called {}", Thread.currentThread());
    }
}
```
运行应用程序：
```bash
./gradlew bootRun
```
并调用端点
```bash
curl 'localhost:8085/test'
```
得到什么：
```java
Starting AppApplication using Java 21.0.1 with PID 38126
Started AppApplication in 1.131 seconds (process running for 1.491)
Async task method has been called VirtualThread[#52,task-1]/runnable@ForkJoinPool-1-worker-5
Scheduled method has been called VirtualThread[#46,scheduling-1]/runnable@ForkJoinPool-1-worker-1
Rest controller method has been called VirtualThread[#62,tomcat-handler-0]/runnable@ForkJoinPool-1-worker-1
Scheduled method has been called VirtualThread[#46,scheduling-1]/runnable@ForkJoinPool-1-worker-1
```
可以看到方法的日志链接到公共 ForkJoinPool 线程池。<br />根据JEP：预期行为：<br />JDK 的虚拟线程调度程序是一个工作窃取的 ForkJoinPool，它以 FIFO 模式运行。调度程序的并行度是可用于调度虚拟线程的平台线程的数量。<br />现在在 GraalVM 上运行它。<br />首先，需要构建一个 GraalVM 本机映像：（此命令可能需要几分钟）然后运行：（使用应用程序的名称而不是“app”）
```bash
./gradlew nativeCompile

./build/native/nativeComplie/app
```
它也可以工作，并且启动时间要快得多，这符合声明的“与 JVM 相比，生成的程序具有更快的启动时间和更低的运行时内存开销”。<br />在这里可以找到包含本文中使用的代码的存储库来源：<br />[https://github.com/egor-ponomarev/spring-boot3.2-with-graalvm-virtual-threads-example](https://github.com/egor-ponomarev/spring-boot3.2-with-graalvm-virtual-threads-example)
<a name="BJu8g"></a>
### 结论
Spring Boot 3.2 是一直在等待的东西！具有虚拟线程的本机映像允许我们编写能够提供与 Go 类似级别的性能和可扩展性的代码，从而保持 JVM 的强大生态系统。<br />但是，您必须考虑到并非所有库都已采用其代码来与虚拟线程正常工作（在大多数情况下，它正在用 ReentrantLock 替换“synchronize”块），您应该小心虚拟线程将使用的逻辑。
