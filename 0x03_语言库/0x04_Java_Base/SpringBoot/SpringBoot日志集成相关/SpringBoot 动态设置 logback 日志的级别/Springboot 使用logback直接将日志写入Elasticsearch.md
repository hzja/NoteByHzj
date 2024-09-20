SpringBoot logback Elasticsearch 日志
<a name="uopjU"></a>
#### 1、引入Elasticsearch依赖
```xml
<dependency>
  <groupId>com.internetitem</groupId>
  <artifactId>logback-elasticsearch-appender</artifactId>
  <version>1.6</version>
</dependency>
```
<a name="vCyoL"></a>
#### 2、添加logback-spring.xml到resource目录的根目录下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <property name="LOGS" value="./logs"/>
    <appender name="Console"
              class="ch.qos.logback.core.ConsoleAppender">
        <layout class="ch.qos.logback.classic.PatternLayout">
            <Pattern>
                %black(%d{ISO8601}) %highlight(%-5level) [%blue(%t)] %yellow(%C{1.}): %msg%n%throwable
            </Pattern>
        </layout>
    </appender>
    <appender name="RollingFile"
              class="ch.qos.logback.core.rolling.RollingFileAppender">
        <file>${LOGS}/spring-boot-logger.log</file>
        <encoder
                class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
            <Pattern>%d %p %C{1.} [%t] %m%n</Pattern>
        </encoder>
        <rollingPolicy
                class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!-- rollover daily and when the file reaches 10 MegaBytes -->
            <fileNamePattern>${LOGS}/archived/spring-boot-logger-%d{yyyy-MM-dd}.%i.log
            </fileNamePattern>
            <timeBasedFileNamingAndTriggeringPolicy
                    class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
                <maxFileSize>10MB</maxFileSize>
            </timeBasedFileNamingAndTriggeringPolicy>
        </rollingPolicy>
    </appender>
    <appender name="ELASTIC" class="com.internetitem.logback.elasticsearch.ElasticsearchAppender">
        <url>ES连接字符串</url>
        <index>linkanyway-logs-%date{yyyy-MM-dd}</index>
        <type>tester</type>
        <loggerName>linkanyway-logger</loggerName> <!-- optional -->
        <errorLoggerName>linkanyway-error-logger</errorLoggerName> <!-- optional -->
        <connectTimeout>30000</connectTimeout> <!-- optional (in ms, default 30000) -->
        <errorsToStderr>false</errorsToStderr> <!-- optional (default false) -->
        <includeCallerData>false</includeCallerData> <!-- optional (default false) -->
        <logsToStderr>false</logsToStderr> <!-- optional (default false) -->
        <maxQueueSize>104857600</maxQueueSize> <!-- optional (default 104857600) -->
        <maxRetries>3</maxRetries> <!-- optional (default 3) -->
        <readTimeout>30000</readTimeout> <!-- optional (in ms, default 30000) -->
        <sleepTime>250</sleepTime> <!-- optional (in ms, default 250) -->
        <rawJsonMessage>false</rawJsonMessage> <!-- optional (default false) -->
        <includeMdc>false</includeMdc> <!-- optional (default false) -->
        <maxMessageSize>100</maxMessageSize> <!-- optional (default -1 -->
        <authentication class="com.internetitem.logback.elasticsearch.config.BasicAuthentication"/> <!-- optional -->
        <properties>
            <property>
                <name>host</name>
                <value>${HOSTNAME}</value>
                <allowEmpty>false</allowEmpty>
            </property>
            <property>
                <name>severity</name>
                <value>%level</value>
            </property>
            <property>
                <name>thread</name>
                <value>%thread</value>
            </property>
            <property>
                <name>stacktrace</name>
                <value>%ex</value>
            </property>
            <property>
                <name>logger</name>
                <value>%logger</value>
            </property>
        </properties>
        <headers>
            <header>
                <name>Content-Type</name>
                <value>application/json</value>
            </header>
        </headers>
    </appender>
    <!-- LOG everything at INFO level -->
    <root level="info">
        <appender-ref ref="RollingFile"/>
        <appender-ref ref="Console"/>
      <!--所有的日志都会通过ELASTIC为名的appender计入es-->
        <appender-ref ref="ELASTIC"/>
    </root>
    <!-- LOG "com.baeldung*" at TRACE level -->
    <logger name="com.baeldung" level="trace" additivity="false">
        <appender-ref ref="RollingFile"/>
        <appender-ref ref="ELASTIC"/>
    </logger>
   
    <logger name="es-logger" level="INFO" additivity="false">
        <appender-ref ref="ELASTIC"/>
    </logger>
</configuration>
```
<a name="xg36G"></a>
#### 3、在application.yml中设定日志level
```yaml
logging:
  level:
    root: info
```
<a name="AeXl4"></a>
#### 4、在root部分不适用es记录日志而在需要的代码中用如下方式记录日志
```java
package com.linkanyway.portal.controllers;
import com.linkanyway.sdk.exception.ApiResult;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
/**
 * @author Fcant
 * @version 1.0
 * @name EsLoggerController
 * @description TODO
 * @date 2020/12/13 13:14
 */
@RestController
@RequestMapping("log")
public class EsLoggerController {
    private Logger logger = LoggerFactory.getLogger("es-logger");
    @GetMapping("log")
    public ApiResult<String> log()
    {
        logger.warn ("eslogger");
        return ApiResult.success ("done!");
    }
}
```
