SpringBoot2.* log4j2
<a name="khZkA"></a>
## 前言
Log4j2是 Log4j 的进化版本，并提供了许多 Logback 可用的改进，同时解决了 Logback 体系结构中的一些固有问题。而且日志处理中会用到kafka作为日志管道。而kafka客户端依赖与Logback的兼容不是很完美，可以选择排除依赖冲突或者使用Log4j2 。
<a name="Wc1w1"></a>
## 排除Logback依赖
Spring Boot 2.x默认使用Logback日志框架，要使用 Log4j2必须先排除 Logback。
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter</artifactId>
    <exclusions>
         <!--排除logback-->
        <exclusion>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-logging</artifactId>
        </exclusion>
    </exclusions>
</dependency>
```
<a name="vaOqM"></a>
## 引入Log4j2依赖
```xml
<!--log4j2 依赖-->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-log4j2</artifactId>
</dependency>
```
上面的 log4j2 已经适配了slf4j日志门面，所以代码无需替换，只需要替换具体的日志框架以及对应的配置文件。
<a name="Sr9q8"></a>
## 配置Log4j2
创建log4j2.xml文件，放在工程`resources`目录里。这样就可以不加任何配置。如果需要指定配置文件需要在Spring Boot 配置文件application.yml中指定 `logging.config` 属性。下面是一份比较详细的 log4j2 配置文件 ：
```xml
 <?xml version="1.0" encoding="UTF-8"?>
  <!--日志级别以及优先级排序: OFF > FATAL > ERROR > WARN > INFO > DEBUG > TRACE > ALL -->
  <!--Configuration后面的status，这个用于设置log4j2自身内部的信息输出，可以不设置，当设置成trace时，可以看到log4j2内部各种详细输出-->
  <!--monitorInterval：Log4j能够自动检测修改配置 文件和重新配置本身，设置间隔秒数-->
  <configuration status="WARN" monitorInterval="30">
      <!--先定义所有的appender-->
      <appenders>
      <!--这个输出控制台的配置-->
          <console name="Console" target="SYSTEM_OUT">
          <!--输出日志的格式-->
              <PatternLayout pattern="[%d{HH:mm:ss:SSS}] [%p] - %l - %m%n"/>
          </console>
      <!--文件会打印出所有信息，这个log每次运行程序会自动清空，由append属性决定，这个也挺有用的，适合临时测试用-->
      <File name="log" fileName="log/test.log" append="false">
         <PatternLayout pattern="%d{HH:mm:ss.SSS} %-5level %class{36} %L %M - %msg%xEx%n"/>
      </File>
      <!-- 这个会打印出所有的info及以下级别的信息，每次大小超过size，则这size大小的日志会自动存入按年份-月份建立的文件夹下面并进行压缩，作为存档-->
          <RollingFile name="RollingFileInfo" fileName="${sys:user.home}/logs/info.log"
                       filePattern="${sys:user.home}/logs/$${date:yyyy-MM}/info-%d{yyyy-MM-dd}-%i.log">
              <!--控制台只输出level及以上级别的信息（onMatch），其他的直接拒绝（onMismatch）-->
              <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="DENY"/>
              <PatternLayout pattern="[%d{HH:mm:ss:SSS}] [%p] - %l - %m%n"/>
              <Policies>
                  <TimeBasedTriggeringPolicy/>
                  <SizeBasedTriggeringPolicy size="100 MB"/>
              </Policies>
          </RollingFile>
          <RollingFile name="RollingFileWarn" fileName="${sys:user.home}/logs/warn.log"
                       filePattern="${sys:user.home}/logs/$${date:yyyy-MM}/warn-%d{yyyy-MM-dd}-%i.log">
              <ThresholdFilter level="warn" onMatch="ACCEPT" onMismatch="DENY"/>
              <PatternLayout pattern="[%d{HH:mm:ss:SSS}] [%p] - %l - %m%n"/>
              <Policies>
                  <TimeBasedTriggeringPolicy/>
                  <SizeBasedTriggeringPolicy size="100 MB"/>
              </Policies>
          <!-- DefaultRolloverStrategy属性如不设置，则默认为最多同一文件夹下7个文件，这里设置了20 -->
              <DefaultRolloverStrategy max="20"/>
          </RollingFile>
          <RollingFile name="RollingFileError" fileName="${sys:user.home}/logs/error.log"
                       filePattern="${sys:user.home}/logs/$${date:yyyy-MM}/error-%d{yyyy-MM-dd}-%i.log">
              <ThresholdFilter level="error" onMatch="ACCEPT" onMismatch="DENY"/>
              <PatternLayout pattern="[%d{HH:mm:ss:SSS}] [%p] - %l - %m%n"/>
              <Policies>
                  <TimeBasedTriggeringPolicy/>
                  <SizeBasedTriggeringPolicy size="100 MB"/>
              </Policies>
          </RollingFile>
      </appenders>
      <!--然后定义logger，只有定义了logger并引入的appender，appender才会生效-->
      <loggers>
          <!--过滤掉spring和mybatis的一些无用的DEBUG信息-->
          <logger name="org.springframework" level="INFO"/>
          <logger name="org.mybatis" level="INFO"/>
          <root level="all">
              <appender-ref ref="Console"/>
              <appender-ref ref="RollingFileInfo"/>
              <appender-ref ref="RollingFileWarn"/>
              <appender-ref ref="RollingFileError"/>
          </root>
      </loggers>
  </configuration>
```
基本上拿上面的配置根据自己的需要更改一下即可生效。Windows 下 `${sys:user.home}` 会将日志打印到用户目录下
<a name="XXTVH"></a>
## 调用Logger输出日志
下面的示例代码使用了`lombok`中的`@Slf4j` 注解可以很方便的使用 org.slf4j.Logger 对象。日常开发尽量使用Slf4j门面来处理日志，尽量避免使用具体的日志框架。
```java
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDate;
import java.util.Properties;

/**
 * @author dax
 * @since 2019/10/9 0:03
 */
@Slf4j
@RestController
@RequestMapping("/logging")
public class LogController {


    @GetMapping("/do")
    public String log() {

        log.info("log4j2 test date: {}  info: {}", LocalDate.now(), "Hello");

        return "log4j2";
    }

}
```
控制台输出 ：
```xml
[00:14:48:161] [INFO] - cn.fcant.spring.security.LogController.log(LogController.java:23) - log4j2 test date: 2019-10-09 info: Hello 
```
并存档。
