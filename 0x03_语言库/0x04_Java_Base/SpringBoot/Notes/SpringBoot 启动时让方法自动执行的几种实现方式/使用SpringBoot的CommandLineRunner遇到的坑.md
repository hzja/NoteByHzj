Java SpringBoot CommandLineRunner
<a name="g14SO"></a>
### 使用场景
再应用程序开发过程中，往往需要在容器启动的时候执行一些操作。Spring Boot中提供了`CommandLineRunner`和`ApplicationRunner`两个接口来实现这样的需求。
<a name="pxdTK"></a>
### 两个接口的不同
参数不同，其他大体相同，可根据实际需求选择合适的接口使用。`CommandLineRunner`接口中`run`方法的参数为String数组，`ApplicationRunner`中`run`方法的参数为`ApplicationArguments`。
<a name="FsVDm"></a>
### 特殊的场景
在启动项目时，有时候所做的操作可能不是一次性的操作，有可能循环查询数据库，根据结果来处理不同的业务，亦或是监听消息队列……
<a name="aIB7T"></a>
### 遇到的坑
看下面一个例子，启动一个spring boot项目，正常启动情况下，项目启动后会打印启动时间。如下图所示
```
2018-07-16 01:48:22.378  INFO 9164 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Registering beans for JMX exposure on startup
2018-07-16 01:48:22.386  INFO 9164 --- [           main] o.s.c.support.DefaultLifecycleProcessor  : Starting beans in phase 2147483647
2018-07-16 01:48:22.386  INFO 9164 --- [           main] d.s.w.p.DocumentationPluginsBootstrapper : Context refreshed
2018-07-16 01:48:22.396  INFO 9164 --- [           main] d.s.w.p.DocumentationPluginsBootstrapper : Found 1 custom documentation plugin(s)
2018-07-16 01:48:22.417  INFO 9164 --- [           main] s.d.s.w.s.ApiListingReferenceScanner     : Scanning for api listing references
2018-07-16 01:48:22.546  INFO 9164 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8088 (http)
2018-07-16 01:48:22.555  INFO 9164 --- [           main] com.hello.word.WordParseApplication      : Started WordParseApplication in 3.811 seconds (JVM running for 4.486)
```
下面模拟一下启动项目时使用`CommandLineRunner`，有人说`CommandLineRunner`是项目启动完成后才调用的，看看现象。
```java
@Component
public class RunService  implements CommandLineRunner {

    public void run(String... strings){
        int i =0;
        while(true){
            i++;
            try {
                Thread.sleep(10000);
                System.out.println("过去了10秒钟……,i的值为："+i);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            if(i==4){ //第40秒时抛出一个异常
                throw new RuntimeException();
            }
            continue;
        }
    }
}
```
再次启动spring boot 项目，看看日志，直接报错，启动异常了。
```
2018-07-16 01:56:43.703  INFO 7424 --- [           main] o.s.c.support.DefaultLifecycleProcessor  : Starting beans in phase 2147483647
2018-07-16 01:56:43.703  INFO 7424 --- [           main] d.s.w.p.DocumentationPluginsBootstrapper : Context refreshed
2018-07-16 01:56:43.722  INFO 7424 --- [           main] d.s.w.p.DocumentationPluginsBootstrapper : Found 1 custom documentation plugin(s)
2018-07-16 01:56:43.750  INFO 7424 --- [           main] s.d.s.w.s.ApiListingReferenceScanner     : Scanning for api listing references
2018-07-16 01:56:43.885  INFO 7424 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8088 (http)
过去了10秒钟……,i的值为：1
过去了10秒钟……,i的值为：2
过去了10秒钟……,i的值为：3
过去了10秒钟……,i的值为：4
2018-07-16 01:57:23.939  INFO 7424 --- [           main] utoConfigurationReportLoggingInitializer : 
 
Error starting ApplicationContext. To display the auto-configuration report re-run your application with 'debug' enabled.
2018-07-16 01:57:23.973 ERROR 7424 --- [           main] o.s.boot.SpringApplication               : Application startup failed
 
java.lang.IllegalStateException: Failed to execute CommandLineRunner
 at org.springframework.boot.SpringApplication.callRunner(SpringApplication.java:735) [spring-boot-1.5.9.RELEASE.jar:1.5.9.RELEASE]
 at org.springframework.boot.SpringApplication.callRunners(SpringApplication.java:716) [spring-boot-1.5.9.RELEASE.jar:1.5.9.RELEASE]
 at org.springframework.boot.SpringApplication.afterRefresh(SpringApplication.java:703) [spring-boot-1.5.9.RELEASE.jar:1.5.9.RELEASE]
 at org.springframework.boot.SpringApplication.run(SpringApplication.java:304) [spring-boot-1.5.9.RELEASE.jar:1.5.9.RELEASE]
 at org.springframework.boot.SpringApplication.run(SpringApplication.java:1118) [spring-boot-1.5.9.RELEASE.jar:1.5.9.RELEASE]
 at org.springframework.boot.SpringApplication.run(SpringApplication.java:1107) [spring-boot-1.5.9.RELEASE.jar:1.5.9.RELEASE]
 at com.hello.word.WordParseApplication.main(WordParseApplication.java:15) [classes/:na]
Caused by: java.lang.RuntimeException: null
 at com.zhangwq.service.RunService.run(RunService.java:24) ~[classes/:na]
 at org.springframework.boot.SpringApplication.callRunner(SpringApplication.java:732) [spring-boot-1.5.9.RELEASE.jar:1.5.9.RELEASE]
 ... 6 common frames omitted
 
2018-07-16 01:57:23.975  INFO 7424 --- [           main] ationConfigEmbeddedWebApplicationContext : Closing org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@14a4e18: startup date [Mon Jul 16 01:56:39 CST 2018]; root of context hierarchy
2018-07-16 01:57:23.975  INFO 7424 --- [           main] o.s.c.support.DefaultLifecycleProcessor  : Stopping beans in phase 2147483647
2018-07-16 01:57:23.975  INFO 7424 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Unregistering JMX-exposed beans on shutdown
 
Process finished with exit code 1
```
说明启动`CommandLineRunner`的执行其实是整个应用启动的一部分，没有打印最后的启动时间，说明项目是在`CommandLineRunner`执行完成之后才启动完成的。此时`CommandLineRunner`的`run`方法执行的是一个循环，循环到第四次的时候，抛出异常，直接影响主程序的启动。
<a name="OW6hw"></a>
### 填坑
这样的问题该如何解决呢？这个操作影响了主线程，那么是否可以重新开启一个线程，让他单独去做想要做的操作呢。
```java
@Component
public class RunService  implements CommandLineRunner {
 
    public void run(String... strings){
        new Thread(){
            public void run() {
                int i = 0;
                while (true) {
                    i++;
                    try {
                        Thread.sleep(10000);
                        System.out.println("过去了10秒钟……,i的值为：" + i);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    if (i == 4) { //第40秒时抛出一个异常
                        throw new RuntimeException();
                    }
                    continue;
                }
            }
        }.start();
    }
}
```
再看看这次的日志是什么样的
```
2018-07-16 02:05:52.680  INFO 7148 --- [           main] o.s.c.support.DefaultLifecycleProcessor  : Starting beans in phase 2147483647
2018-07-16 02:05:52.680  INFO 7148 --- [           main] d.s.w.p.DocumentationPluginsBootstrapper : Context refreshed
2018-07-16 02:05:52.695  INFO 7148 --- [           main] d.s.w.p.DocumentationPluginsBootstrapper : Found 1 custom documentation plugin(s)
2018-07-16 02:05:52.717  INFO 7148 --- [           main] s.d.s.w.s.ApiListingReferenceScanner     : Scanning for api listing references
2018-07-16 02:05:52.815  INFO 7148 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8088 (http)
2018-07-16 02:05:52.819  INFO 7148 --- [           main] com.hello.word.WordParseApplication      : Started WordParseApplication in 3.406 seconds (JVM running for 4.063)
过去了10秒钟……,i的值为：1
过去了10秒钟……,i的值为：2
过去了10秒钟……,i的值为：3
过去了10秒钟……,i的值为：4
Exception in thread "Thread-10" java.lang.RuntimeException
 at com.zhangwq.service.RunService$1.run(RunService.java:26)
```
此时`CommandLineRunner`执行的操作和主线程是相互独立的，抛出异常并不会影响到主线程。程序打印了启动时间，并且`CommandLineRunner`中`run`方法报错后，应用程序并没有因为异常而终止。
