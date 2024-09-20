Java SpringBoot 日志<br />SpringBoot日志配置：<br />SpringBoot默认日志是打印再console中的，不会保存在文件中。项目上线肯定要保存日志用于分析的。
<a name="uGbZ9"></a>
### 一、使用xml配置日志保存（并不需要pom配置slf4j依赖，starter里面已经配置了依赖了）
<a name="pU4xy"></a>
#### 1、在项目的resources目录下创建一个【logback-spring.xml】日志配置文件
名称只要是一 logback开头就行，测试使用log.xml并不会生成日志。合法名称：logback.xml 、logback-spring.xml<br />备注：要配置logback-spring.xml，SpringBoot会默认加载此文件，为什么不配置logback.xml,因为logback.xml会先application.properties加载，而logback-spring.xml会后于application.properties加载，这样在application.properties文中设置日志文件名称和文件路径才能生效
<a name="WXXsk"></a>
#### 2、编写xml文件内容
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration  scan="true" scanPeriod="60 seconds" debug="false">
  <contextName>logback</contextName>
  <!--输出到控制台-->
  <appender name="console" class="ch.qos.logback.core.ConsoleAppender">
    <encoder>
      <!--<pattern>%d{HH:mm:ss} [%thread] %-5level %logger{36} - %msg%n</pattern>-->
      <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger - %msg%n</pattern>
      <!--            <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} -%5p ${PID:-} [%15.15t] %-30.30C{1.} : %m%n</pattern>-->
    </encoder>
  </appender>

  <!--按天生成日志-->
  <appender name="logFile"  class="ch.qos.logback.core.rolling.RollingFileAppender">
    <Prudent>true</Prudent> 
    <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
      <FileNamePattern>
        poslog/%d{yyyy-MM-dd}/%d{yyyy-MM-dd}.log
      </FileNamePattern>
      <maxHistory>7</maxHistory> 
    </rollingPolicy>
    <layout class="ch.qos.logback.classic.PatternLayout">
      <Pattern>
        %d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger - %msg%n
      </Pattern>
    </layout>
  </appender>

  <root level="INFO">
    <appender-ref ref="console" />
    <appender-ref ref="logFile" />
  </root>

</configuration>
```
上述配置的编码中，对应符号的含义如下

- %d{HH:mm:ss.SSS}——日志输出时间
- %thread——输出日志的进程名字，这在Web应用以及异步任务处理中很有用
- %-5level——日志级别，并且使用5个字符靠左对齐
- %logger ——日志输出者的名字
- %msg——日志消息
- %n——平台的换行符
<a name="fiQMp"></a>
#### 3、编写打印日志
```java
private Logger logger = LoggerFactory.getLogger(TestController.class);

logger.info("name = " + name + " || views = " + views);// 使用
```
<a name="zz0hq"></a>
#### 4、启动测试
在当前文件夹下会创建一个【poslog/2020-10/22】的文件夹，里面会按天生成日志：【2020-10-22.log】，例如：
```java
2020-10-22 16:38 [main] INFO  org.springframework.boot.web.embedded.tomcat.TomcatWebServer - Tomcat initialized with port(s): 8088 (http)
2020-10-22 16:38 [main] INFO  org.apache.coyote.http11.Http11NioProtocol - Initializing ProtocolHandler ["http-nio-8088"]
2020-10-22 16:38 [main] INFO  org.apache.catalina.core.StandardService - Starting service [Tomcat]
2020-10-22 16:38 [main] INFO  org.apache.catalina.core.StandardEngine - Starting Servlet engine: [Apache Tomcat/9.0.38]
2020-10-22 16:38 [main] INFO  org.apache.catalina.core.AprLifecycleListener - Loaded Apache Tomcat Native library [1.2.23] using APR version [1.7.0].
2020-10-22 16:38 [main] INFO  org.apache.catalina.core.AprLifecycleListener - APR capabilities: IPv6 [true], sendfile [true], accept filters [false], random [true].
2020-10-22 16:38 [main] INFO  org.apache.catalina.core.AprLifecycleListener - APR/OpenSSL configuration: useAprConnector [false], useOpenSSL [true]
2020-10-22 16:38 [main] INFO  org.apache.catalina.core.AprLifecycleListener - OpenSSL successfully initialized [OpenSSL 1.1.1c  28 May 2019]
2020-10-22 16:38 [main] INFO  org.apache.catalina.core.ContainerBase.[Tomcat].[localhost].[/] - Initializing Spring embedded WebApplicationContext
2020-10-22 16:38 [main] INFO  org.springframework.boot.web.servlet.context.ServletWebServerApplicationContext - Root WebApplicationContext: initialization completed in 1422 ms
2020-10-22 16:38 [main] INFO  org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor - Initializing ExecutorService 'applicationTaskExecutor'
2020-10-22 16:38 [main] INFO  org.apache.coyote.http11.Http11NioProtocol - Starting ProtocolHandler ["http-nio-8088"]
2020-10-22 16:38 [main] INFO  org.springframework.boot.web.embedded.tomcat.TomcatWebServer - Tomcat started on port(s): 8088 (http) with context path ''
2020-10-22 16:38 [main] INFO  com.example.demo.DockerComposeTestApplication - Started DockerComposeTestApplication in 2.841 seconds (JVM running for 3.257)
```
备注：发现使用这个默认不用配置pom依赖，最新的spring-boot-starter-web中已经集成了。<br />总结：启动一个项目，直接将logback-spring.xml文件复制到resources目录下就可以实现日志文件记录。非常方便。
<a name="wVw68"></a>
### 二、进阶配置
分类logback.xml配置：<br />日志配置到d盘了
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
  <!-- 日志存放路径 -->
  <property name="log.path" value="d:/logback" />
  <!-- 日志输出格式 -->
  <property name="log.pattern" value="%d{HH:mm:ss.SSS} [%thread] %-5level %logger{20} - [%method,%line] - %msg%n" />

  <!-- 控制台输出 -->
  <appender name="console" class="ch.qos.logback.core.ConsoleAppender">
    <encoder>
      <pattern>${log.pattern}</pattern>
    </encoder>
  </appender>

  <!-- 系统日志输出 -->
  <appender name="file_info" class="ch.qos.logback.core.rolling.RollingFileAppender">
    <file>${log.path}/sys-info.log</file>
    <!-- 循环政策：基于时间创建日志文件 -->
    <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
      <!-- 日志文件名格式 -->
      <fileNamePattern>${log.path}/sys-info.%d{yyyy-MM-dd}.log</fileNamePattern>
      <!-- 日志最大的历史 60天 -->
      <maxHistory>60</maxHistory>
    </rollingPolicy>
    <encoder>
      <pattern>${log.pattern}</pattern>
    </encoder>
    <filter class="ch.qos.logback.classic.filter.LevelFilter">
      <!-- 过滤的级别 只会打印debug不会有info日志-->
      <!--            <level>DEBUG</level>-->
      <!-- 匹配时的操作：接收（记录） -->
      <onMatch>ACCEPT</onMatch>
      <!-- 不匹配时的操作：拒绝（不记录） -->
      <onMismatch>DENY</onMismatch>
    </filter>
  </appender>

  <appender name="file_error" class="ch.qos.logback.core.rolling.RollingFileAppender">
    <file>${log.path}/sys-error.log</file>
    <!-- 循环政策：基于时间创建日志文件 -->
    <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
      <!-- 日志文件名格式 -->
      <fileNamePattern>${log.path}/sys-error.%d{yyyy-MM-dd}.log</fileNamePattern>
      <!-- 日志最大的历史 60天 -->
      <maxHistory>60</maxHistory>
    </rollingPolicy>
    <encoder>
      <pattern>${log.pattern}</pattern>
    </encoder>
    <filter class="ch.qos.logback.classic.filter.LevelFilter">
      <!-- 过滤的级别 -->
      <level>ERROR</level>
      <!-- 匹配时的操作：接收（记录） -->
      <onMatch>ACCEPT</onMatch>
      <!-- 不匹配时的操作：拒绝（不记录） -->
      <onMismatch>DENY</onMismatch>
    </filter>
  </appender>

  <!-- 用户访问日志输出  -->
  <appender name="sys-user" class="ch.qos.logback.core.rolling.RollingFileAppender">
    <file>${log.path}/sys-user.log</file>
    <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
      <!-- 按天回滚 daily -->
      <fileNamePattern>${log.path}/sys-user.%d{yyyy-MM-dd}.log</fileNamePattern>
      <!-- 日志最大的历史 60天 -->
      <maxHistory>60</maxHistory>
    </rollingPolicy>
    <encoder>
      <pattern>${log.pattern}</pattern>
    </encoder>
  </appender>

  <!-- 系统模块日志级别控制  -->
  <logger name="com.example" level="debug" />
  <!-- Spring日志级别控制  -->
  <logger name="org.springframework" level="warn" />

  <root level="info">
    <appender-ref ref="console" />
  </root>

  <!--系统操作日志-->
  <root level="info">
    <appender-ref ref="file_info" />
    <appender-ref ref="file_error" />
  </root>

  <!--系统用户操作日志-->
  <logger name="sys-user" level="info">
    <appender-ref ref="sys-user"/>
  </logger>
</configuration> 
```
还可以在application.yml中配置
```yaml
# 日志配置
logging:
  level:
    com.example: info
    org.springframework: warn
```
配置`info`就只有`info`以上日志才会输出<br />如果配置 com.example: debug ，那么项目com.example包里面debug以上的日志也会输出。
