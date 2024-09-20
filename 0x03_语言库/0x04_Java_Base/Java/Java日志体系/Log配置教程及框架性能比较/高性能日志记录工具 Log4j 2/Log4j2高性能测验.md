Java 日志 Log4j2
<a name="Lvnsk"></a>
## 环境
jdk：1.7.0_79<br />cpu：i5-4570@3.20GHz 4核<br />eclipse：3.7<br />操作系统：win7
<a name="uFaGq"></a>
## 准备
<a name="AF5h5"></a>
### 1、log4j:1.7.21
```xml
<dependency>
        <groupId>org.slf4j</groupId>
    <artifactId>slf4j-log4j12</artifactId>
    <version>1.7.21</version>
</dependency>
```
<a name="laJU3"></a>
#### log4j.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE log4j:configuration SYSTEM "log4j.dtd">
<log4j:configuration xmlns:log4j='http://jakarta.apache.org/log4j/'>

    <appender name="myConsole" class="org.apache.log4j.ConsoleAppender">
        <layout class="org.apache.log4j.PatternLayout">
            <param name="ConversionPattern" value="[%d{dd HH:mm:ss,SSS\} %-5p] [%t] %c{2\} - %m%n" />
        </layout>

        <!--过滤器设置输出的级别 -->
        <filter class="org.apache.log4j.varia.LevelRangeFilter">
            <param name="levelMin" value="debug" />
            <param name="levelMax" value="warn" />
            <param name="AcceptOnMatch" value="true" />
        </filter>
    </appender>

    <appender name="myFile" class="org.apache.log4j.DailyRollingFileAppender">
        <param name="File" value="log4jTest.log" />
        <param name="Append" value="true" />
        <param name="DatePattern" value="'.'yyyy-MM-dd'.log'" />
        <layout class="org.apache.log4j.PatternLayout">
            <param name="ConversionPattern" value="[%t] - %m%n" />
        </layout>
    </appender>

    <appender name="async_file" class="org.apache.log4j.AsyncAppender">
        <param name="BufferSize" value="32" />
        <appender-ref ref="myFile" />
    </appender>

    <logger name="org.logTest" additivity="false">
        <level value="info" />
        <appender-ref ref="async_file" /> <!-- 同步:FILE 异步:async_file -->
    </logger>

</log4j:configuration>
```
<a name="ehi66"></a>
### 2、logback:1.1.7
```xml
<dependency>
    <groupId>ch.qos.logback</groupId>
    <artifactId>logback-classic</artifactId>
    <version>1.1.7</version>
</dependency>
```
<a name="jpBgc"></a>
#### logback.xml
```xml
<configuration>
    <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
        <!-- encoder 默认配置为PatternLayoutEncoder -->
        <encoder>
            <pattern>%d{HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n
            </pattern>
        </encoder>
    </appender>

    <appender name="FILE" class="ch.qos.logback.core.FileAppender">
        <file>testFile.log</file>
        <append>true</append>
        <encoder>
            <pattern>[%t] - %m%n
            </pattern>
        </encoder>
    </appender>

    <!-- 异步输出 -->
    <appender name="ASYNC" class="ch.qos.logback.classic.AsyncAppender">
        <discardingThreshold>0</discardingThreshold>
        <appender-ref ref="FILE" />
    </appender>

    <logger name="org.logTest" level="INFO"
        additivity="false">
        <appender-ref ref="ASYNC" />  <!-- 同步:FILE 异步:ASYNC-->
    </logger>

    <root level="ERROR">
        <appender-ref ref="STDOUT" />
    </root>
</configuration>
```
<a name="PKefn"></a>
### 3、log4j2:2.6.2
```xml
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-core</artifactId>
    <version>2.6.2</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-slf4j-impl</artifactId>
    <version>2.6.2</version>
</dependency>
<dependency>
    <groupId>com.lmax</groupId>
    <artifactId>disruptor</artifactId>
    <version>3.3.4</version>
</dependency>
```
<a name="rZ9Ul"></a>
#### log4j2.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!--设置log4j2的自身log级别为warn -->
<configuration status="warn">

    <appenders>
         <console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="[%d{HH:mm:ss:SSS}] [%p] - %l - %m%n" />
        </console>

        <RollingFile name="RollingFileInfo" fileName="info.log"
            filePattern="${sys:user.home}/logs/hpaasvc/$${date:yyyy-MM}/info-%d{yyyy-MM-dd}-%i.log">
            <Filters>
                <ThresholdFilter level="INFO" />
                <ThresholdFilter level="WARN" onMatch="DENY"
                    onMismatch="NEUTRAL" />
            </Filters>
            <PatternLayout pattern="[%t] - %m%n" />
            <Policies>
                <TimeBasedTriggeringPolicy />
                <SizeBasedTriggeringPolicy size="100 MB" />
            </Policies>
        </RollingFile>

        <RandomAccessFile name="RandomAccessFile" fileName="asyncWithLocation.log"
            immediateFlush="false" append="true">
            <PatternLayout>
                <Pattern>[%t] - %m%n</Pattern>
            </PatternLayout>
        </RandomAccessFile>

    </appenders>

    <loggers>
        <!-- <AsyncLogger name="asynLogger" level="trace"
            includeLocation="true">
            <AppenderRef ref="RandomAccessFile" />
        </AsyncLogger> -->
        <Root level="info" includeLocation="true">
            <AppenderRef ref="RollingFileInfo" />
        </Root>
    </loggers>

</configuration>
```
<a name="zNTDa"></a>
## 测试
准备50条线程同时记录1000000条数据，然后统计时间，详细代码如下：
```java
import java.util.concurrent.CountDownLatch;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class App {
    private static Logger log = LoggerFactory.getLogger(App.class);

    public static void main(String[] args) throws InterruptedException {
        int messageSize = 1000000;
        int threadSize = 50;
        final int everySize = messageSize / threadSize;

        final CountDownLatch cdl = new CountDownLatch(threadSize);
        long startTime = System.currentTimeMillis();
        for (int ts = 0; ts < threadSize; ts++) {
            new Thread(new Runnable() {

                @Override
                public void run() {
                    for (int es = 0; es < everySize; es++) {
                        log.info("======info");
                    }
                    cdl.countDown();
                }
            }).start();
        }

        cdl.await();
        long endTime = System.currentTimeMillis();
        System.out.println("log4j1:messageSize = " + messageSize
                + ",threadSize = " + threadSize + ",costTime = "
                + (endTime - startTime) + "ms");
    }
}
```
log4j1和logback的同步和异步分别修改为对应的`appender`就行了<br />log4j2的异步方式提供了2中模式：
<a name="dXBcP"></a>
### 1、全局开启
设置`Log4jContextSelector`系统属性为： 
```java
org.apache.logging.log4j.core.async.AsyncLoggerContextSelector
System.setProperty("Log4jContextSelector", "org.apache.logging.log4j.core.async.AsyncLoggerContextSelector");
```
<a name="xuM6Z"></a>
### 2、混合同步异步模式
不需要设置`Log4jContextSelector`，但是需要使用`AsyncLogger`标签<br />更多详细参考官方文档：[http://logging.apache.org/log4j/2.x/manual/async.html#AllAsync](http://logging.apache.org/log4j/2.x/manual/async.html#AllAsync)<br />结果<br />分别测试完以后统计成表格如下：<br />![2021-09-09-16-40-35-815318.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631176877992-333cce45-e628-4867-891f-3308b819fa69.png#averageHue=%23edecec&clientId=u254114ab-811e-4&from=ui&id=u66b61350&originHeight=243&originWidth=895&originalType=binary&ratio=1&rotation=0&showTitle=false&size=653825&status=done&style=none&taskId=u8e36af2c-6a83-4141-852f-cef6c080404&title=)<br />log4j2的异步模式表现了绝对的性能优势，优势主要得益于Disruptor框架的使用
> LMAX Disruptor technology. Asynchronous Loggers internally use the Disruptor, a lock-free inter-thread communication library, instead of queues, resulting in higher throughput and lower latency.

一个无锁的线程间通信库代替了原来的队列。
