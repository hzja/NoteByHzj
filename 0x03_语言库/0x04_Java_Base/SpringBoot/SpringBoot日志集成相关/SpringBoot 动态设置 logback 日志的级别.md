Java SpringBoot logback<br />SpringBoot 默认采用 Logback 日志作为记录日志的框架。在项目中，一般分为 dev 环境（开发环境）、test环境（测试环境）和 prd 环境（生产环境）。而一般开发环境和测试环境可以把日志级别设置成 info 级别，方便在联调与测试中解决遇到的bug，但是生产环境因为存在大量的业务，如果采用 info 级别的日志， 那么每一笔交易或者业务都打印大量的日志。这样的花需要大量的磁盘空间来保存日志。如果日志需要保存一年半载，或者有些业务场景，例如财务数据的日志需要保存更久。将给公司造成很大的财务浪费。今天分享一个小方案，动态改变日志的级别，方便在生产环境也能方便快捷的从 warn 或者 error 级别切换到 debug 或者 info 级别。
<a name="a7EG5"></a>
## 1、pom.xml 引入依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.olive</groupId>
    <artifactId>valid-demo</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <packaging>jar</packaging>

    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.8.RELEASE</version>
        <relativePath /> 
    </parent>
    <name>valid-demo</name>
    <url>http://maven.apache.org</url>

    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

    </dependencies>
</project>
```
<a name="d0yLv"></a>
## 2、logbak-spring.xml 配置
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!--
scan：当此属性设置为true时，配置文件如果发生改变，将会被重新加载，默认值为true。
scanPeriod：设置监测配置文件是否有修改的时间间隔，如果没有给出时间单位，默认单位是毫秒当scan为true时，此属性生效。默认的时间间隔为1分钟。
debug：当此属性设置为true时，将打印出logback内部日志信息，实时查看logback运行状态。默认值为false。
-->
<configuration scan="false" scanPeriod="60 seconds" debug="false">

    <!-- ch.qos.logback.core.ConsoleAppender 表示控制台输出 -->
    <appender name="stdout" class="ch.qos.logback.core.ConsoleAppender">
        <!--
            日志输出格式：
                %d表示日期时间，
                %thread表示线程名，
                %-5level：级别从左显示5个字符宽度
                %logger{50} 表示logger名字最长50个字符，否则按照句点分割。 
                %msg：日志消息，
                %n是换行符
        -->
        <layout class="ch.qos.logback.classic.PatternLayout">
            <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50} - %msg%n</pattern>
        </layout>
    </appender>

    <root level="info">
        <!-- 控制台输出日志-->
        <appender-ref ref="stdout" />
    </root>
</configuration>
```
为了演示，这里只配置了一个 rootLogger
<a name="qMEoN"></a>
## 3、编写 Controller
该 Controller 主要包含以下三个API<br />主要框架
```java
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.slf4j.impl.StaticLoggerBinder;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import ch.qos.logback.classic.Level;
import ch.qos.logback.classic.LoggerContext;

@RestController
public class LoggerController {

    private final static  Logger logger = LoggerFactory.getLogger(LoggerController.class);

     // TODO

}
```
<a name="H1LIr"></a>
### 打印各日志级别的日志
```java
@RequestMapping("/logger/print")
public Map loggerPrint(){
    Map result = new HashMap();
    result.put("code", 200);
    result.put("msg", "success");

    logger.debug("loggerPrint debug>>>>");
    logger.info("loggerPrint info>>>>");
    logger.warn("loggerPrint warn>>>>");
    logger.error("loggerPrint error>>>>");

    return result;
}
```

- 打印日志对象 Logger
```java
@RequestMapping("/logger/printAllLogger")
public Map printAllLogger(){
    Map result = new HashMap();
    result.put("code", 200);
    result.put("msg", "success");

    LoggerContext loggerContext = (LoggerContext) LoggerFactory.getILoggerFactory();
    List<ch.qos.logback.classic.Logger> loggers = loggerContext.getLoggerList();
    Iterator<ch.qos.logback.classic.Logger> iter = loggers.iterator();
    System.out.println("printAllLogger begin>>>>>>>>");
    while(iter.hasNext()){
        System.out.println(iter.next().getName());
    }
    System.out.println("printAllLogger end>>>>>>>>");
    return result;
}
```
该方法打印所有存在的日志对象 Logger
<a name="zRPI8"></a>
### 设置 ROOT 日志对象的日志级别
```java
@RequestMapping("/logger/level")
public Map loggerLevelChange(String level){
    Map result = new HashMap();
    result.put("code", 200);
    result.put("msg", "success");

    String loggerFactoryClassStr = StaticLoggerBinder.getSingleton().getLoggerFactoryClassStr();
    System.out.println("loggerFactoryClassStr>>>>" + loggerFactoryClassStr);

    LoggerContext loggerContext = (LoggerContext) LoggerFactory.getILoggerFactory();
    ch.qos.logback.classic.Logger logger = loggerContext.getLogger("ROOT");
    switch (level) {
    case "debug":
        logger.setLevel(Level.DEBUG);
        break;
    case "info":
        logger.setLevel(Level.INFO);
        break;
    case "warn":
        logger.setLevel(Level.WARN);
        break;
    case "error":
        logger.setLevel(Level.ERROR);
        break;
    default:
        break;
    }
    return result;
}
```
<a name="i4bK4"></a>
## 4、编写引导类与测试
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class Application 
{
    public static void main( String[] args )
    {
        SpringApplication.run(Application.class, args);
    }
}
```
启动服务
<a name="X9qbi"></a>
### 打印日志对象 Logger
![2021-05-13-09-09-20-357154.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620869197002-e377403a-6f3e-4b8d-b3b6-13c4a8b038a7.png#clientId=u89b1f52f-8a2b-4&from=ui&id=u17d6b43d&originHeight=378&originWidth=1080&originalType=binary&size=38374&status=done&style=shadow&taskId=u2241a7a0-d852-4acb-abca-80e779fdc32)<br />可以在控制台看到如下日志
```
printAllLogger begin>>>>>>>>
ROOT
com.olive.Application
com.olive.controller.LoggerController
// 省略一万条记录
printAllLogger end>>>>>>>>
```
<a name="wCeOd"></a>
### 打印各日志级别的日志
目前 logback-spring.xml 配置的 ROOT 的日志级别是 info 级别<br />![2021-05-13-09-09-20-590152.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620869214674-67938ebe-125c-45a7-a7d6-e34e21bd7277.png#clientId=u89b1f52f-8a2b-4&from=ui&id=u3eb822ff&originHeight=366&originWidth=1080&originalType=binary&size=36958&status=done&style=shadow&taskId=u51baa85a-4766-4f5e-b816-c2b218f2b47)<br />可以看到控制台只打印 info 级别及 info 级别以上的日志
```sql
2021-05-12 22:10:27.959 [http-nio-8080-exec-3] INFO  com.olive.controller.LoggerController - loggerPrint info>>>>
2021-05-12 22:10:27.959 [http-nio-8080-exec-3] WARN  com.olive.controller.LoggerController - loggerPrint warn>>>>
2021-05-12 22:10:27.960 [http-nio-8080-exec-3] ERROR com.olive.controller.LoggerController - loggerPrint error>>>>
```
<a name="f85FM"></a>
### 设置 ROOT 日志对象的日志级别
![2021-05-13-09-09-20-796006.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620869228032-98fb6770-26aa-485e-9ea8-0b53a25c20dd.png#clientId=u89b1f52f-8a2b-4&from=ui&id=uc833ebb2&originHeight=363&originWidth=1080&originalType=binary&size=37957&status=done&style=shadow&taskId=u078c9a07-8da0-425f-a8f2-47a085cebba)<br />调用完以上接口后，再次调用 http://127.0.0.1:8080/logger/print 接口。可以看到控制台<br />![2021-05-13-09-09-21-308044.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620869236134-4f916bc6-133c-40ac-b8a6-57b5fdb9d8dd.png#clientId=u89b1f52f-8a2b-4&from=ui&id=ud73f938d&originHeight=420&originWidth=1080&originalType=binary&size=402985&status=done&style=shadow&taskId=uafc335d6-3fb1-492d-bee2-2ec6c86a6f5)<br />打印了大量 debug 级别及 debug 级别以上的日志
