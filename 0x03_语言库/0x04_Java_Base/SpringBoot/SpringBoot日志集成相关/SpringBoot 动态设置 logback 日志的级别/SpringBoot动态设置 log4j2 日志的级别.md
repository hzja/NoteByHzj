SpringBoot log4j2
<a name="sbx7S"></a>
## 1、pom.xml 引入依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.olive</groupId>
    <artifactId>valid-log4j2</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <packaging>jar</packaging>

    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.8.RELEASE</version>
        <relativePath /> 
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
            <exclusions><!-- 去掉springboot默认配置 -->  
                <exclusion>  
                    <groupId>org.springframework.boot</groupId>  
                    <artifactId>spring-boot-starter-logging</artifactId>  
                </exclusion>  
            </exclusions>  
        </dependency> 

        <dependency> <!-- 引入log4j2依赖 -->  
            <groupId>org.springframework.boot</groupId>  
            <artifactId>spring-boot-starter-log4j2</artifactId>  
        </dependency> 
    </dependencies>
</project>
```
因为 SpringBoot 默认使用 logback 记录日志。所以要排除 logback 日志框架spring-boot-starter-logging，在引入 log4j2日志框架 spring-boot-starter-log4j2。
<a name="zPagX"></a>
## 2、Controller 控制器编写
<a name="Azuja"></a>
### Controller 基本框架
```java
import java.util.HashMap;
import java.util.Map;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.core.LoggerContext;
import org.apache.logging.log4j.core.config.Configuration;
import org.apache.logging.log4j.core.config.LoggerConfig;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.slf4j.impl.StaticLoggerBinder;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;


@RestController
public class LoggerController {

    private final static  Logger logger = LoggerFactory.getLogger(LoggerController.class);

    // TODO 
}
```
<a name="NcIEc"></a>
### 打印各级别日志
```java
@RequestMapping("/logger/print")
public Map loggerPrint(){
    Map result = new HashMap();
    result.put("code", 200);
    result.put("msg", "success");

    logger.debug("loggerPrint debug>>>>");
    logger.info("loggerPrint info>>>>");
    logger.warn("loggerPrint warn>>>>");
    logger.error("loggerPrint error>>>>");

    return result;
}
```
<a name="IIzGc"></a>
### 打印所有日志对象
```java
@RequestMapping("/logger/printAllLogger")
public Map printAllLogger(){
    Map result = new HashMap();
    result.put("code", 200);
    result.put("msg", "success");

    LoggerContext ctx = (LoggerContext) LogManager.getContext(false);

    Configuration config = ctx.getConfiguration();
    Map<String, LoggerConfig> loggerConfigs = config.getLoggers();
    System.out.println("printAllLogger begin>>>>>>>>");
    loggerConfigs.forEach((name, loggerConfig) -> {
        System.out.println("name = " + name);
        System.out.println("loggerConfig = " + loggerConfig);
    });
    System.out.println("printAllLogger end>>>>>>>>");
    return result;
}
```
<a name="zW8Qm"></a>
### 动态设置日志对象的日志级别
```java
@RequestMapping("/logger/level")
public Map loggerLevelChange(String level){
    Map result = new HashMap();
    result.put("code", 200);
    result.put("msg", "success");

    String loggerFactoryClassStr = StaticLoggerBinder.getSingleton().getLoggerFactoryClassStr();
    System.out.println("loggerFactoryClassStr>>>>" + loggerFactoryClassStr);    
    LoggerContext ctx = (LoggerContext) LogManager.getContext(false);
    Configuration config = ctx.getConfiguration();
    LoggerConfig loggerConfig = config.getLoggerConfig(LogManager.ROOT_LOGGER_NAME);

    switch (level) {
    case "debug":
        loggerConfig.setLevel(Level.DEBUG);
        break;
    case "info":
        loggerConfig.setLevel(Level.INFO);
        break;
    case "warn":
        loggerConfig.setLevel(Level.WARN);
        break;
    case "error":
        loggerConfig.setLevel(Level.ERROR);
        break;
    default:
        break;
    }
    ctx.updateLoggers();
    return result;
}
```
<a name="vWLR4"></a>
## 3、log4j2-spring.xml 配置
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!--Configuration后面的status，这个用于设置log4j2自身内部的信息输出，可以不设置，当设置成trace时，你会看到log4j2内部各种详细输出-->
<!--monitorInterval：Log4j能够自动检测修改配置 文件和重新配置本身，设置间隔秒数-->
<configuration monitorInterval="5">
  <!--日志级别以及优先级排序: OFF > FATAL > ERROR > WARN > INFO > DEBUG > TRACE > ALL -->

  <!--变量配置-->
  <Properties>
    <!-- 格式化输出：%date表示日期，%thread表示线程名，%-5level：级别从左显示5个字符宽度 %msg：日志消息，%n是换行符-->
    <!-- %logger{36} 表示 Logger 名字最长36个字符 -->
    <property name="LOG_PATTERN" value="%date{HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n" />
  </Properties>

  <appenders>
    <console name="ConsoleOut" target="SYSTEM_OUT">
      <!--输出日志的格式-->
      <PatternLayout pattern="${LOG_PATTERN}"/>
      <!--控制台只输出level及其以上级别的信息（onMatch），其他的直接拒绝（onMismatch）-->
      <!--    <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="DENY"/> -->
    </console>
  </appenders>

  <loggers>
    <root level="debug">
      <appender-ref ref="ConsoleOut"/>
    </root>
  </loggers>

</configuration>
```
这里只配置了打印到控制台的 ROOT logger。
<a name="ympMk"></a>
## 4、编写引导类
```java
@SpringBootApplication
public class Application 
{
    public static void main( String[] args )
    {
        SpringApplication.run(Application.class, args);
    }
}
```
启动服务
<a name="loeap"></a>
### 打印各级别日志
![2021-05-14-21-52-22-481535.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621000656574-59cdd672-93a1-4a98-ade0-3661b636e8be.png#clientId=uffd9e588-2314-4&from=ui&id=u88da5113&originHeight=363&originWidth=1080&originalType=binary&size=37366&status=done&style=shadow&taskId=u00143e5b-1359-4732-a213-dc9156150df)<br />因为 log4j2-spring.xml 配置的级别是 debug，所以可以在控制台看到如下打印
```java
21:08:52.298 [http-nio-8080-exec-1] DEBUG com.olive.controller.LoggerController - loggerPrint debug>>>>
21:08:52.298 [http-nio-8080-exec-1] INFO  com.olive.controller.LoggerController - loggerPrint info>>>>
21:08:52.298 [http-nio-8080-exec-1] WARN  com.olive.controller.LoggerController - loggerPrint warn>>>>
21:08:52.298 [http-nio-8080-exec-1] ERROR com.olive.controller.LoggerController - loggerPrint error>>>>
```
<a name="hsSsh"></a>
### 打印所有日志对象
![2021-05-14-21-52-24-788720.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621000674677-8e23ecec-2deb-4d8f-9dfb-5ce9b182bcd4.png#clientId=uffd9e588-2314-4&from=ui&id=uec4377d4&originHeight=357&originWidth=1080&originalType=binary&size=37330&status=done&style=shadow&taskId=uf471e5f8-5520-495f-9d78-78a0ce2e6e2)<br />在控制台只看见打印了 ROOT logger 对象。
```java
printAllLogger begin>>>>>>>>
name = 
loggerConfig = root
printAllLogger end>>>>>>>>
```
<a name="jzjJH"></a>
### 动态修改日志的级别
![2021-05-14-21-52-25-099810.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621000687079-e6f5519e-963e-402d-a819-1c0876b409ee.png#clientId=uffd9e588-2314-4&from=ui&id=u729e2750&originHeight=331&originWidth=1080&originalType=binary&size=37328&status=done&style=shadow&taskId=u6048b6c4-9acc-4a4b-8de1-a4638ebde30)<br />再次调用 http://127.0.0.1:8080/logger/print 接口打印日志，在控制台看到不在打印 debug 级别日志
```java
21:19:25.756 [http-nio-8080-exec-6] INFO  com.olive.controller.LoggerController - loggerPrint info>>>>
21:19:25.756 [http-nio-8080-exec-6] WARN  com.olive.controller.LoggerController - loggerPrint warn>>>>
21:19:25.756 [http-nio-8080-exec-6] ERROR com.olive.controller.LoggerController - loggerPrint error>>>>
```
<a name="u8tte"></a>
### 修改 log4j2-spring.xml 配置
```xml
<loggers>
  <root level="debug">
    <appender-ref ref="Console"/>
  </root>

  <logger name="org.springframework" level="info" additivity="true">
  </logger>

  <logger name="org.hibernate" level="warn" additivity="true">
  </logger>
</loggers>
```
重启服务，再次调用 http://127.0.0.1:8080/logger/printAllLogger 接口打印日志对象
```java
printAllLogger begin>>>>>>>>
name = 
loggerConfig = root
name = org.springframework
loggerConfig = org.springframework
name = org.hibernate
loggerConfig = org.hibernate
printAllLogger end>>>>>>>>
```
