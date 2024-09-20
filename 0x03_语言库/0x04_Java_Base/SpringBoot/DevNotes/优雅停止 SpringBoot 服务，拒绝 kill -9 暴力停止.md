SpringBoot<br />在使用 SpringBoot 的时候，都要涉及到服务的停止和启动，当停止服务的时候，很多时候大家都是kill -9 直接把程序进程杀掉，这样程序不会执行优雅的关闭。而且一些没有执行完的程序就会直接退出。<br />很多时候都需要安全的将服务停止，也就是把没有处理完的工作继续处理完成。比如停止一些依赖的服务，输出一些日志，发一些信号给其他的应用系统，这个在保证系统的高可用是非常有必要的。看一下几种停止 SpringBoot 的方法。
<a name="l1uXw"></a>
## 第一种：SpringBoot 提供的actuator的功能
它可以执行`shutdown`, `health`, `info`等，默认情况下，actuator的`shutdown`是`disable`的，需要打开它。首先引入acturator的Maven依赖。
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```
然后将shutdown节点打开，也将/actuator/shutdown暴露web访问也设置上，除了shutdown之外还有health, info的web访问都打开的话将`management.endpoints.web.exposure.include=*`就可以。将如下配置设置到application.properties里边。设置一下服务的端口号为3333。
```
server.port=3333
management.endpoint.shutdown.enabled=true
management.endpoints.web.exposure.include=shutdown
```
接下来，创建一个SpringBoot工程，然后设置一个bean对象，配置上PreDestroy方法。这样在停止的时候会打印语句。bean的整个生命周期分为创建、初始化、销毁，当最后关闭的时候会执行销毁操作。在销毁的方法中执行一条输出日志。
```java
package com.fcant.springboot.shutdowndemo.bean;
import javax.annotation.PreDestroy;
public class TerminateBean {
    @PreDestroy
    public void preDestroy() {
        System.out.println("TerminalBean is destroyed");
    }
}
```
做一个configuration，然后提供一个获取bean的方法，这样该bean对象会被初始化。
```java
package com.fcant.springboot.shutdowndemo.config;
import com.fcant.springboot.shutdowndemo.bean.TerminateBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
@Configuration
public class ShutDownConfig {
    @Bean
    public TerminateBean getTerminateBean() {
        return new TerminateBean();
    }
}
```
在启动类里边输出一个启动日志，当工程启动的时候，会看到启动的输出，接下来执行停止命令。
```bash
curl -X POST http://localhost:3333/actuator/shutdown
```
以下日志可以输出启动时的日志打印和停止时的日志打印，同时程序已经停止。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618365239939-ac6af6f6-d43b-48a9-8398-c3384e5d3d81.png#height=130&id=T5q8m&originHeight=130&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)
<a name="vsl9S"></a>
## 第二种：获取程序启动时候的context，然后关闭主程序启动时的context
这样程序在关闭的时候也会调用PreDestroy注解。如下方法在程序启动十秒后进行关闭。
```java
/* method 2: use ctx.close to shutdown all application context */
ConfigurableApplicationContext ctx = SpringApplication.run(ShutdowndemoApplication.class, args);
try {
    TimeUnit.SECONDS.sleep(10);
} catch (InterruptedException e) {
    e.printStackTrace();
}
ctx.close();
```
<a name="Yfjw7"></a>
## 第三种
在SpringBoot启动的时候将进程号写入一个app.pid文件，生成的路径是可以指定的，可以通过命令 `cat /Users/huangqingshi/app.id | xargs kill` 命令直接停止服务，这个时候bean对象的PreDestroy方法也会调用的。这种方法使用的比较普遍。写一个start.sh用于启动SpringBoot程序，然后写一个停止程序将服务停止。
```java
/* method 3 : generate a pid in a specified path, while use command to shutdown pid :
            'cat /Users/huangqingshi/app.pid | xargs kill' */
SpringApplication application = new SpringApplication(ShutdowndemoApplication.class);
application.addListeners(new ApplicationPidFileWriter("/Users/huangqingshi/app.pid"));
application.run();
```
<a name="oy6Ev"></a>
## 第四种
通过调用一个`SpringApplication.exit()`方法也可以退出程序，同时将生成一个退出码，这个退出码可以传递给所有的context。这个就是一个JVM的钩子，通过调用这个方法的话会把所有PreDestroy的方法执行并停止，并且传递给具体的退出码给所有Context。通过调用`System.exit(exitCode)`可以将这个错误码也传给JVM。程序执行完后最后会输出：Process finished with exit code 0，给JVM一个SIGNAL。
```java
/* method 4: exit this application using static method */
ConfigurableApplicationContext ctx = SpringApplication.run(ShutdowndemoApplication.class, args);
exitApplication(ctx);
```
```java
public static void exitApplication(ConfigurableApplicationContext context) {
    int exitCode = SpringApplication.exit(context, (ExitCodeGenerator) () -> 0);
    System.exit(exitCode);
}
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1618365239740-f30d5f74-e8b2-4e3b-9947-d5736e81d76b.png#height=157&id=SIqVw&originHeight=157&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)
<a name="FuLbB"></a>
## 第五种
自己写一个Controller，然后将自己写好的Controller获取到程序的context，然后调用自己配置的Controller方法退出程序。通过调用自己写的/shutDownContext方法关闭程序：<br />`curl -X POST http://localhost:3333/shutDownContext`。
```java
package com.fcant.springboot.shutdowndemo.controller;
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;
@RestController
public class ShutDownController implements ApplicationContextAware {
    private ApplicationContext context;
    @PostMapping("/shutDownContext")
    public String shutDownContext() {
        ConfigurableApplicationContext ctx = (ConfigurableApplicationContext) context;
        ctx.close();
        return "context is shutdown";
    }
    @GetMapping("/")
    public String getIndex() {
        return "OK";
    }
    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        context = applicationContext;
    }
}
```
SpringBoot的优雅关闭方法也都实现好了，暴力停止直接`kill -9` 相应的PID即可。
