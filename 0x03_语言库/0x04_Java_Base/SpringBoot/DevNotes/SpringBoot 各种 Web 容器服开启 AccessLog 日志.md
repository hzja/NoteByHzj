JavaSpringBoot<br />SpringBoot 内置支持的 Web 容器有 Tomcat、Undertow、Jetty 和 Netty。默认情况下，这些 Web 服务的 AccessLog 日志是不开启的，而 AccessLog 日志对于做接口统计尤为重要。如果要开启这些日志，需要进行如下配置。
<a name="JdJ6h"></a>
## 1、Tomcat容器日志配置
SpringBoot 2.x 默认支持的是 Tomcat Web 容器，如果要开启 AccessLog 日志，只需要添加若干配置文件即可：
```yaml
server:
	tomcat:
		basedir: /var/tmp
		background-processor-delay: 30
		port-header: X-Forwarded-Port
		protocol-header: X-Forwarded-Proto
		protocol-header-https-value: https
		redirect-context-root: true
		remote-ip-header: X-Forwarded-For
		uri-encoding: UTF-8
		accesslog:
			enabled: true
			buffered: true
			directory: ./log
			file-date-format: .yyyy-MM-dd
			pattern: '%h %l %u %t "%r" %s %b "%{Referer}i" "%{User-Agent}i" %D ms'
			prefix: access_log
			rename-on-rotate: false
			request-attributes-enabled: false
			rotate: true
			suffix: .log
```
其中
```yaml
server:
	tomcat:
		basedir: /var/tmp
```
属性是必须要配置的，如果不配置该属性，日志也是不会输出的。配置完毕，AccessLog 日志就会在 /var/tmp/log 目录下生成，名称是 access_log.yyyy-MM-dd.log
<a name="EvjKx"></a>
## 2、Undertow容器日志配置
引入 Undertow 依赖包后，在配置文件添加如下配置。另外，注意需要排除 Tomcat 依赖包
```yaml
server:
	undertow:
		max-http-post-size: 10MB
		buffer-size: 1024KB
		io-threads: 8
		worker-threads: 64
		direct-buffers: true
		eager-filter-init: true
		accesslog:
			enabled: true
			pattern: common
			prefix: access_log.
			suffix: log
			dir: /var/undertow/logs
			rotate: true
```
配置完毕，AccessLog 日志会生成在 /var/undertow/logs 目录下。
<a name="iHtWb"></a>
## 3、Jetty容器日志配置
引入 Jetty 依赖包后，在配置文件添加如下配置。另外，注意需要排除 Tomcat 依赖包
```yaml
server:
	jetty:
		max-http-post-size: 4MB
		acceptors: 32
		selectors: 32
		accesslog:
			enabled: true
			filename: /var/jetty/logs/access_log.yyyy_MM_dd.log
			file-date-format: yyyy-MM-dd
			retention-period: 20
			append: true
			extended-format: true
			date-format: 'dd/MMM/yyyy:HH:mm:ss Z'
			locale: zh
			timeZone: GMT+8
			log-cookies: false
			log-server: true
			log-latency: true
```
配置完成后，AccessLog 日志文件会生成在 /var/jetty/logs 目录下。
<a name="W3KYi"></a>
## 4、Netty容器日志配置
这个容器比较特殊，说其是容器，倒不如说是一种协议。启用这个作为**“容器”**的话，需要引入reactor-netty.jar。这个版本管理也比较乱，注意与 netty 版本匹配！并且在reactor-netty早期版本(低于0.7.8版本)是不支持 AccessLog 日志的！（这里用的是0.7.13版本，当然也可以用更高版本，但是注意与netty版本要保持兼容！）首先加入reactor-netty依赖：
```xml
<dependency>
	<groupId>io.projectreactor.ipc</groupId>
	<artifactId>reactor-netty</artifactId>
	<version>0.7.13.RELEASE</version>
</dependency>
```
其次，需要开启netty日志支持，需要添加系统属性`reactor.netty.http.server.accessLogEnabled=true`。注意，这个属性不能通过SpringBoot配置添加，但可以使用下面方法：
```java
public static void main(String[] args) throws Exception {
	System.setProperty("reactor.netty.http.server.accessLogEnabled", "true");
	configureApplication(new SpringApplicationBuilder()).run(args);
}
```
最后，需要借助系统日志实现该日志的输出（这里用的是 logback），参照配置如下：
```xml
<!-- [Netty Appender] Access日志 -->
<appender name="ACCESS-APPENDER" class="ch.qos.logback.core.rolling.RollingFileAppender">
	<file>/var/netty/log/access_log.log</file>
	<rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
		<fileNamePattern>/var/netty/log/access/access_log.log.%d{yyyyMMdd}</fileNamePattern>
		<maxHistory>20</maxHistory>
	</rollingPolicy>
	<encoder charset="UTF-8">
		<pattern>%msg%n</pattern>
	</encoder>
</appender>
<appender name="ASYNC-APPENDER" class="ch.qos.logback.classic.AsyncAppender">
	<appender-ref ref="ACCESS-APPENDER" />
</appender>
<!-- ===== [Access Logger] ===== -->
<logger name="reactor.netty.http.server.AccessLog" additivity="false">
	<level value="INFO"/>
	<appender-ref ref="ASYNC-APPENDER"/>
</logger>
```
这里要注意几点：

- logger名称必须用`reactor.netty.http.server.AccessLog`；
- encoder的pattern无需自己定义，使用系统定义即可（`%msg%n`）；
- 生成日志存放路径/var/netty/log;

目前系统内置的支持的这几种 Web 容器的 AccessLog 日志配置就是这些，如果想使用自定义Web容器，那可以参照<br />`org.springframework.boot.autoconfigure.web.embedded.EmbeddedWebServerFactoryCustomizerAutoConfiguration`<br />该文件来配置自定义 Web 容器和其AccessLog 日志。
