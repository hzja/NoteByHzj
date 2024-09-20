JavaSpringBoot日志
<a name="zhOsk"></a>
## 1、Java 日志概览
说到 Java 日志，很多初学者可能都比较懵，因为这里涉及到太多东西了：Apache Commons Logging、Slf4j、Log4j、Log4j2、Logback、Java Util Logging 等等，这些框架各自有什么作用？他们之间有什么区别？
<a name="sOflD"></a>
### 1.1 总体概览
下面这张图很好的展示了 Java 中的日志体系：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658109252978-5679516a-962d-4d44-b782-603d6fb88e05.png#clientId=u7a3ebe2a-80b4-4&from=paste&id=ub984249b&originHeight=232&originWidth=541&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc9e41e7a-214f-4bf6-b0e1-0d44f6ce1e7&title=)<br />可以看到，Java 中的日志框架主要分为两大类：**日志门面**和**日志实现**。<br />**日志门面**<br />日志门面定义了一组日志的接口规范，它并不提供底层具体的实现逻辑。Apache Commons Logging 和 Slf4j 就属于这一类。<br />**日志实现**<br />日志实现则是日志具体的实现，包括日志级别控制、日志打印格式、日志输出形式（输出到数据库、输出到文件、输出到控制台等）。Log4j、Log4j2、Logback 以及 Java Util Logging 则属于这一类。<br />将日志门面和日志实现分离其实是一种典型的门面模式，这种方式可以让具体业务在不同的日志实现框架之间自由切换，而不需要改动任何代码，开发者只需要掌握日志门面的 API 即可。<br />日志门面是不能单独使用的，它必须和一种具体的日志实现框架相结合使用。<br />那么日志框架是否可以单独使用呢？<br />技术上来说当然没问题，但是一般不会这样做，因为这样做可维护性很差，而且后期扩展不易。例如 A 开发了一个工具包使用 Log4j 打印日志，B 引用了这个工具包，但是 B 喜欢使用 Logback 打印日志，此时就会出现一个业务使用两个甚至多个日志框架，开发者也需要维护多个日志的配置文件。因此都是用日志门面打印日志。
<a name="CKeMC"></a>
### 1.2 日志级别
使用日志级别的好处在于，调整级别，就可以屏蔽掉很多调试相关的日志输出。不同的日志实现定义的日志级别不太一样，不过也都大同小异。
<a name="QDRil"></a>
#### Java Util Logging
Java Util Logging 定义了 7 个日志级别，从严重到普通依次是：

- SEVERE
- WARNING
- INFO
- CONFIG
- FINE
- FINER
- FINEST

因为默认级别是 INFO，因此 INFO 级别以下的日志，不会被打印出来。
<a name="zIL9B"></a>
#### Log4j
Log4j 定义了 8 个日志级别（除去 OFF 和 ALL，可以说分为 6 个级别），从严重到普通依次是：

- OFF：最高等级的，用于关闭所有日志记录。
- FATAL：重大错误，这种级别可以直接停止程序了。
- ERROR：打印错误和异常信息，如果不想输出太多的日志，可以使用这个级别。
- WARN：警告提示。
- INFO：用于生产环境中输出程序运行的一些重要信息，不能滥用。
- DEBUG：用于开发过程中打印一些运行信息。
- TRACE
- ALL 最低等级的，用于打开所有日志记录。
<a name="CKFuH"></a>
#### Logback
Logback 日志级别比较简单，从严重到普通依次是：

- ERROR
- WARN
- INFO
- DEBUG
- TRACE
<a name="iwgVs"></a>
### 1.3 综合对比
Java Util Logging 系统在 JVM 启动时读取配置文件并完成初始化，一旦应用程序开始运行，就无法修改配置。另外，这种日志实现配置也不太方便，只能在 JVM 启动时传递参数，像下面这样：<br />-Djava.util.logging.config.file=<config-file-name>。

由于这些局限性，导致 Java Util Logging 并未广泛使用。<br />Log4j 虽然配置繁琐，但是一旦配置完成，使用起来就非常方便，只需要将相关的配置文件放到 classpath 下即可。在很多情况下，Log4j 的配置文件可以在不同的项目中反复使用。<br />Log4j 可以和 Apache Commons Logging 搭配使用，Apache Commons Logging 会自动搜索并使用 Log4j，如果没有找到 Log4j，再使用 Java Util Logging。<br />比 Log4j + Apache Commons Logging 组合更得人心的是 Slf4j + Logback 组合。<br />Logback 是 Slf4j 的原生实现框架，它也出自 Log4j 作者（Ceki Gülcü）之手，但是相比 Log4j，它拥有更多的优点、特性以及更强的性能。
<a name="ENonX"></a>
### 1.4 最佳实践

- 如果不想添加任何依赖，使用 Java Util Logging 或框架容器已经提供的日志接口。
- 如果比较在意性能，推荐：Slf4j + Logback。
- 如果项目中已经使用了 Log4j 且没有发现性能问题，推荐组合为：Slf4j + Log4j2。
<a name="fEfOc"></a>
## 2、Spring Boot 日志实现
Spring Boot 使用 Apache Commons Logging 作为内部的日志框架门面，它只是一个日志接口，在实际应用中需要为该接口来指定相应的日志实现。<br />Spring Boot 默认的日志实现是 Logback。这个很好查看：随便启动一个 Spring Boot 项目，从控制台找一行日志，例如下面这样：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658109253283-591e76a7-cf1e-488a-88ff-c9bea3935e44.png#clientId=u7a3ebe2a-80b4-4&from=paste&id=ucc8a1ad9&originHeight=158&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubdff6424-ddf3-48d7-b65e-e1dbcf5c9a5&title=)<br />考虑到最后的 prod 是一个可以变化的字符，在项目中全局搜索：The following profiles are active，结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658109253363-d8d1059b-8b4d-49c8-a82c-19bd7f09670d.png#clientId=u7a3ebe2a-80b4-4&from=paste&id=ube1f197f&originHeight=520&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2de222d8-1eba-4b10-9d42-8875af01abb&title=)<br />在日志输出的那一行 debug。然后再次启动项目，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658109253339-4bebb239-2a25-4866-b072-b58a4a454480.png#clientId=u7a3ebe2a-80b4-4&from=paste&id=u569a3d3e&originHeight=363&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6c031bc5-d186-44f3-a009-fa36a44cc76&title=)<br />此时就可以看到真正的日志实现是 Logback。<br />其他的诸如 Java Util Logging、Log4j 等框架，Spring Boot 也有很好的支持。<br />在 Spring Boot 项目中，只要添加了如下 web 依赖，日志依赖就自动添加进来了：
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
<a name="sgGGb"></a>
### 2.1 Spring Boot 日志配置
Spring Boot 的日志系统会自动根据 classpath 下的内容选择合适的日志配置，在这个过程中首选 Logback。<br />如果开发者需要修改日志级别，只需要在 application.properties 文件中通过 logging.level 前缀+包名 的形式进行配置即可，例如下面这样：
```
logging.level.org.springframework.web=debug
logging.level.org.hibernate=error
```
如果想将日志输出到文件，可以通过如下配置指定日志文件名：
```
logging.file.name=javaboy.log
```
`logging.file.name` 可以只指定日志文件名，也可以指定日志文件全路径，例如下面这样：
```
logging.file.name=/Users/sang/Documents/javaboy/javaboy.log
```
如果只是想重新定义输出日志文件的路径，也可以使用 `logging.file.path` 属性，如下：
```
logging.file.path=/Users/sang/Documents/javaboy
```
如果想对输出到文件中的日志进行精细化管理，还有如下一些属性可以配置：

- `logging.logback.rollingpolicy.file-name-pattern`：日志归档的文件名，日志文件达到一定大小之后，自动进行压缩归档。
- `logging.logback.rollingpolicy.clean-history-on-start`：是否在应用启动时进行归档管理。
- `logging.logback.rollingpolicy.max-file-size`：日志文件大小上限，达到该上限后，会自动压缩。
- `logging.logback.rollingpolicy.total-size-cap`：日志文件被删除之前，可以容纳的最大大小。
- `logging.logback.rollingpolicy.max-history`：日志文件保存的天数。

日志文件归档这块，小伙伴们感兴趣可以自己试下，可以首先将 max-file-size 属性调小，这样方便看到效果：
```
logging.logback.rollingpolicy.max-file-size=1MB
```
然后添加如下接口：
```java
@RestController
public class HelloController {
	private static final Logger logger = getLogger(HelloController.class);
	@GetMapping("/hello")
	public void hello() {
		for (int i = 0; i < 100000; i++) {
			logger.info("hello javaboy");
		}
	}
}
```
访问该接口，可以看到最终生成的日志文件被自动压缩了：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658109253277-fc0d3deb-212d-43b5-bc1e-252654d0eca3.png#clientId=u7a3ebe2a-80b4-4&from=paste&id=ue64f594e&originHeight=266&originWidth=426&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8ca910cd-4ff3-4a0f-99f6-4ce9988d580&title=)<br />application.properties 中还可以配置日志分组。<br />日志分组能够把相关的 logger 放到一个组统一管理。<br />例如可以定义一个 tomcat 组：
```
logging.group.tomcat=org.apache.catalina,org.apache.coyote, org.apache.tomcat
```
然后统一管理 tomcat 组中的所有 logger：
```
logging.level.tomcat=TRACE
```
Spring Boot 中还预定义了两个日志分组 web 和 sql，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658109253619-0543e81a-a8da-4a16-bcce-44340fe1d6cb.png#clientId=u7a3ebe2a-80b4-4&from=paste&id=u0dedb4ef&originHeight=191&originWidth=961&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u721c47f3-57f7-4d2e-adbc-43b41e94936&title=)<br />不过在 application.properties 中只能实现对日志一些非常简单的配置，如果想实现更加细粒度的日志配置，那就需要使用日志实现的原生配置，例如 Logback 的 classpath:logback.xml，Log4j 的 classpath:log4j.xml 等。如果这些日志配置文件存在于 classpath 下，那么默认情况下，Spring Boot 就会自动加载这些配置文件。
<a name="dPBjD"></a>
### 2.2 Logback 配置
<a name="S1E2H"></a>
#### 2.2.1 基本配置
默认的 Logback 配置文件名有两种：

- logback.xml：这种配置文件会直接被日志框架加载。
- logback-spring.xml：这种配置文件不会被日志框架直接加载，而是由 Spring Boot 去解析日志配置，可以使用 Spring Boot 的高级 Profile 功能。

Spring Boot 中为 Logback 提供了四个默认的配置文件，位置在 org/springframework/boot/logging/logback/，分别是：

- defaults.xml：提供了公共的日志配置，日志输出规则等。
- console-appender.xml：使用 CONSOLE_LOG_PATTERN 添加一个ConsoleAppender。
- file-appender.xml：添加一个 RollingFileAppender。
- base.xml：为了兼容旧版 Spring Boot 而提供的。

如果需要自定义 logback.xml 文件，可以在自定义时使用这些默认的配置文件，也可以不使用。一个典型的 logback.xml 文件如下（resources/logback.xml）：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <include resource="org/springframework/boot/logging/logback/defaults.xml"/>
    <include resource="org/springframework/boot/logging/logback/console-appender.xml" />
    <root level="INFO">
        <appender-ref ref="CONSOLE" />
    </root>
    <logger name="org.springframework.web" level="DEBUG"/>
</configuration>
```
可以通过 include 引入 Spring Boot 已经提供的配置文件，也可以自定义。
<a name="IxTmJ"></a>
#### 2.2.2 输出到文件
如果想禁止控制台的日志输出，转而将日志内容输出到一个文件，可以自定义一个 logback-spring.xml 文件，并引入前面所说的 file-appender.xml 文件。<br />像下面这样（resources/logback-spring.xml）：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <include resource="org/springframework/boot/logging/logback/defaults.xml" />
    <property name="LOG_FILE" value="${LOG_FILE:-${LOG_PATH:-${LOG_TEMP:-${java.io.tmpdir:-/tmp}}/}spring.log}"/>
    <include resource="org/springframework/boot/logging/logback/file-appender.xml" />
    <root level="INFO">
        <appender-ref ref="FILE" />
    </root>
</configuration>
```
<a name="zJ6E4"></a>
### 2.3 Log4j 配置
如果 classpath 下存在 Log4j2 的依赖，Spring Boot 会自动进行配置。<br />默认情况下 classpath 下当然不存在 Log4j2 的依赖，如果想使用 Log4j2，可以排除已有的 Logback，然后再引入 Log4j2，如下：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <exclusions>
        <exclusion>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-logging</artifactId>
        </exclusion>
    </exclusions>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-log4j2</artifactId>
</dependency>
```
Log4j2 的配置就比较容易了，在 reources 目录下新建 log4j2.xml 文件，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration status="warn">
	<properties>
		<Property name="app_name">logging</Property>
		<Property name="log_path">logs/${app_name}</Property>
	</properties>
	<appenders>
		<console name="Console" target="SYSTEM_OUT">
			<PatternLayout pattern="[%d][%t][%p][%l] %m%n" />
		</console>
		<RollingFile name="RollingFileInfo" fileName="${log_path}/info.log"
								 filePattern="${log_path}/$${date:yyyy-MM}/info-%d{yyyy-MM-dd}-%i.log.gz">
			<Filters>
				<ThresholdFilter level="INFO" />
				<ThresholdFilter level="WARN" onMatch="DENY"
												 onMismatch="NEUTRAL" />
			</Filters>
			<PatternLayout pattern="[%d][%t][%p][%c:%L] %m%n" />
			<Policies>
				<TimeBasedTriggeringPolicy interval="1" modulate="true" />
				<SizeBasedTriggeringPolicy size="2 MB" />
			</Policies>
			<DefaultRolloverStrategy compressionLevel="0" max="10"/>
		</RollingFile>
		<RollingFile name="RollingFileWarn" fileName="${log_path}/warn.log"
								 filePattern="${log_path}/$${date:yyyy-MM}/warn-%d{yyyy-MM-dd}-%i.log.gz">
			<Filters>
				<ThresholdFilter level="WARN" />
				<ThresholdFilter level="ERROR" onMatch="DENY"
												 onMismatch="NEUTRAL" />
			</Filters>
			<PatternLayout pattern="[%d][%t][%p][%c:%L] %m%n" />
			<Policies>
				<TimeBasedTriggeringPolicy interval="1" modulate="true" />
				<SizeBasedTriggeringPolicy size="2 MB" />
			</Policies>
			<DefaultRolloverStrategy compressionLevel="0" max="10"/>
		</RollingFile>
		
		<RollingFile name="RollingFileError" fileName="${log_path}/error.log"
								 filePattern="${log_path}/$${date:yyyy-MM}/error-%d{yyyy-MM-dd}-%i.log.gz">
			<ThresholdFilter level="ERROR" />
			<PatternLayout pattern="[%d][%t][%p][%c:%L] %m%n" />
			<Policies>
				<TimeBasedTriggeringPolicy interval="1" modulate="true" />
				<SizeBasedTriggeringPolicy size="2 MB" />
			</Policies>
			<DefaultRolloverStrategy compressionLevel="0" max="10"/>
		</RollingFile>
	</appenders>
	<loggers>
		<root level="info">
			<appender-ref ref="Console" />
			<appender-ref ref="RollingFileInfo" />
			<appender-ref ref="RollingFileWarn" />
			<appender-ref ref="RollingFileError" />
		</root>
	</loggers>
</configuration>
```
首先在 properties 节点中指定了应用名称以及日志文件位置。<br />然后通过几个不同的 RollingFile 对不同级别的日志分别处理，不同级别的日志将输出到不同的文件，并按照各自的命名方式进行压缩。
