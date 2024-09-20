Java 日志 Log4j2<br />Logback 算是JAVA 里一个老牌的日志框架，从06年开始第一个版本，迭代至今也十几年了。不过logback最近一个稳定版本还停留在 2017 年，好几年都没有更新；logback的兄弟 slf4j 最近一个稳定版也是2017年。<br />而且 logback的异步性能实在拉跨，功能简陋，配置又繁琐，远不及Apache 的新一代日志框架 - Log4j2<br />目前来看，Log4j2 就是王者，其他日志框架都不是对手
<a name="L6llv"></a>
## Log4j2简介
Apache Log4j 2是 Log4j(1) 的升级版，比它的祖先 Log4j 1.x 有了很大的改进，和logback对比有很大的改进。除了内部设计的调整外，主要有以下几点的大升级：

- 更简化的配置
- 更强大的参数格式化
- 最夸张的异步性能

Log4j 2中，分为API(log4j-api）和实现(log4j-core)两个模块。API 和slf4j 是一个类型，属于日志抽象/门面，而实现部分，才是Log4j 2的核心。

- org.apache.logging.log4j » log4j-api
- org.apache.logging.log4j » log4j-core
<a name="EjSC9"></a>
## 最牛逼的性能
<a name="S2nwB"></a>
### 最强的异步性能
这个特性，算是Log4j2最强之处了。log4j2 在目前JAVA中的日志框架里，异步日志的性能是最高的，没有之一。<br />先来看一下，几种日志框架benchmark对比结果（log4j2官方测试结果）：<br />![2021-05-17-20-08-45-115917.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621254289864-8d659fe6-67bc-4d48-be02-fa28547bbe2b.png#averageHue=%23f3f1f1&clientId=uc07639fe-5da8-4&from=ui&id=ue6ca29b3&originHeight=398&originWidth=757&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80244&status=done&style=shadow&taskId=u1c9d6a41-1df1-40dd-b2a9-ad9859339a1&title=)<br />从图上可以看出，log4j2的异步（全异步，非混合模式）下的性能，远超log4j1和logback，简直吊打。压力越大的情况下，吞吐上的差距就越大。在64线程测试下，log4j2的吞吐达到了180w+/s，而logback/log4j1只有不到20w，相差近十倍
<a name="SuQ3F"></a>
### 零GC（Garbage-free）
从2.6版本开始（2016年），log4j2 默认就以零GC模式运行了。什么叫零GC呢？就是不会由于log4j2而导致GC。<br />log4j2 中各种Message对象，字符串数组，字节数组等全部复用，不重复创建，大大减少了无用对象的创建，从而做到“零GC”。
<a name="iC8Vr"></a>
### 更高性能 I/O 写入的支持
log4j 还提供了一个`MemoryMappedFileAppender`，I/O 部分使用`MemoryMappedFile`来实现，可以得到极高的I/O性能。不过在使用`MemoryMappedFileAppender`之前，得确定足够了解`MemoryMappedFile`的相关知识，否则不要轻易使用。
<a name="BR5b3"></a>
## 更强大的参数格式化
API模块和slf4j相比，提供了更丰富的参数格式化功能。
<a name="REWL5"></a>
### 使用`{}`占位符格式化参数
在slf4j里，可以用`{}`的方式来实现“format”的功能（参数会直接`toString`替换占位符），像下面这样：
```java
logger.debug("Logging in user {} with birthday {}", user.getName(), user.getBirthdayCalendar());
```
<a name="X9mZo"></a>
### 使用`String.format`的形式格式化参数
log4j2 中除了支持`{}`的参数占位符，还支持`String.format`的形式：
```java
public static Logger logger = LogManager.getFormatterLogger("Foo");

logger.debug("Logging in user %s with birthday %s", user.getName(), user.getBirthdayCalendar());
logger.debug("Logging in user %1$s with birthday %2$tm %2$te,%2$tY", user.getName(), user.getBirthdayCalendar());
logger.debug("Integer.MAX_VALUE = %,d", Integer.MAX_VALUE);
logger.debug("Long.MAX_VALUE = %,d", Long.MAX_VALUE);
```
:::tips
注意，如果想使用`String.format`的形式，需要使用`LogManager.getFormatterLogger`而不是`LogManager.getLogger`
:::
<a name="a5w5R"></a>
### 使用`logger.printf`格式化参数
log4j2 的 Logger接口中，还有一个`printf`方法，无需创建`LogManager.getFormatterLogger`，就可以使用`String.format`的形式
```java
logger.printf(Level.INFO, "Logging in user %1$s with birthday %2$tm %2$te,%2$tY", user.getName(), user.getBirthdayCalendar());

logger.debug("Opening connection to {}...", someDataSource);
```
<a name="IWBXm"></a>
### “惰性”打日志（lazy logging）
这个功能虽然小，但非常实用。<br />在某些业务流程里，为了留根或追溯问题，需要完整的打印入参，一般是把入参给用JSON/XML序列化后用debug级别打印：
```java
logger.debug("入参报文：{}",JSON.toJSONString(policyDTO));
```
如果需要追溯问题时，会将系统的日志级别调到debug/trace，这样就可以打印。但是这里有个问题，虽然在info级别下debug不会输出内容，但`JSON.toJSONString()`这个序列化的代码一定会执行，严重影响正常流程下的执行效率。<br />期望的结果是`info`级别下，连序列化都不执行。这里可以通过`isDebugEnable`来判断当前配置下debug级别是否可以输出：
```java
if(logger.isDebugEnabled()){
    logger.debug("入参报文：{}",JSON.toJSONString(policyDTO));
}
```
这样虽然可以避免不必要的序列化，但每个地方都这么写还是有点难受的，一行变成了三行。<br />log4j2 的 logger 对象，提供了一系列`lambda`的支持，通过这些接口可以实现“惰性”打日志：
```java
void debug(String message, Supplier<?>... paramSuppliers);
void info(String message, Supplier<?>... paramSuppliers);
void trace(String message, Supplier<?>... paramSuppliers);
void error(String message, Supplier<?>... paramSuppliers);

//等同于下面的先判断，后打印
logger.debug("入参报文：{}",() -> JSON.toJSONString(policyDTO));

if(logger.isDebugEnabled()){
    logger.debug("入参报文：{}",JSON.toJSONString(policyDTO));
}
```
这种 Supplier + Lambda 的形式，等同于上面的先判断 `isDebugEnable` 然后打印，三行的代码变成了一行。
<a name="WAArW"></a>
## 更简化的配置
Log4j 2 同时支持XML/JSON/YML/Properties 四种形式的配置文件，不过最主流的还是XML的方式，最直观。<br />来看一下logback和log4j2的配置文件对比，同样功能的配置下：
<a name="Bxr17"></a>
### logback.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
  <appender name = "File" class= "ch.qos.logback.core.rolling.RollingFileAppender">
    <file>logs/app.log</file>
    <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
      <fileNamePattern>logs/archives/app-%d{yyyy-MM-dd}.log.gz</fileNamePattern>
      <!--一天内大于size就单独分隔-->
      <maxFileSize>1 GB</maxFileSize>
    </rollingPolicy>
  </appender>
  <root level="info">
    <appender-ref ref="File"/>
  </root>
</configuration>
```
<a name="VtCTM"></a>
### log4j2.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration xmlns:xi="http://www.w3.org/2001/XInclude"
  status="warn" name="XInclude">
  <Appenders>
    <RollingFile name="File" fileName="logs/app.log" filePattern="logs/archives/app-%d{yyyy-MM-dd}-%i.log.gz">
      <PatternLayout pattern="%d{yyyy-MM-dd HH:mm:ss.SSS} %5p [%t] %-40.40c{1.} : %m%n"/>
      <Policies>
        <TimeBasedTriggeringPolicy />
        <!--一天内大于size就单独分隔-->
        <SizeBasedTriggeringPolicy size="1 GB"/>
      </Policies>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="INFO">
      <AppenderRef ref="File"/>
    </Root>
  </Loggers>
</Configuration>
```
在log4j2中，appender的配置从使用 Appender 实现名即标签名的形式，语法上更简洁一些：
```xml
<RollingFile name="File">
  <!-- 等同于logback中的 -->
<appender name = "File" class= "ch.qos.logback.core.rolling.RollingFileAppender">
```
<a name="AJS4C"></a>
## 与其他日志抽象/门面适配
log4j2 由于拆分为 API 和 实现两部分，所以可能也需要和其他日志框架进行适配。<br />![2021-05-17-20-08-45-296980.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621254256359-b1f38579-a716-42c9-8066-e0c8a4dedd79.png#averageHue=%23f7f5f5&clientId=uc07639fe-5da8-4&from=ui&id=uaa7d6798&originHeight=694&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=207011&status=done&style=shadow&taskId=u3a649bc2-adb1-4eaf-bc70-3acb43d0fbb&title=)
<a name="Crlx6"></a>
## 其他的特点

1. 异步队列使用高性能队列 - LMAX Disruptor
2. Appender丰富，有JMS/JPA/KAFKA/Http/MONGODB/CouchDB/Socket/Script等各种`Appender`的支持
3. 支持自定义日志级别
4. ……
<a name="w0WvL"></a>
## 基本用法
终于介绍完了Log4j2的强大，现在来介绍下Log4j2的基本使用。
<a name="tCzr3"></a>
### 引用log4j2的Maven依赖
log4j-api在log4j-core中已经有依赖了，直接依赖core即可
```xml
<dependency>
  <groupId>org.apache.logging.log4j</groupId>
  <artifactId>log4j-core</artifactId>
  <version>2.14.1</version>
</dependency>
```
> 注意，引用log4j2时，需要注意项目中是否有多套日志框架共存/冲突，需要适配的问题。细节请参考上面的与其他日志抽象/门面适配

<a name="phkWv"></a>
### 配置文件示例
首先是配置文件，默认的配置文件路径为：classpath:log4j2.xml（推荐使用xml）
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration xmlns:xi="http://www.w3.org/2001/XInclude"
  status="warn" name="XInclude">
  <Properties>
    <Property name="PATTERN" value="%d{yyyy-MM-dd HH:mm:ss.SSS} %5p [%t] %-40.40c{1.} : %m%n"/>
  </Properties>
  <Appenders>
    <!-- 输出到控制台，仅在本地开发环境使用 -->
    <Console name="Console" target="SYSTEM_OUT">
      <PatternLayout pattern="${PATTERN}"/>
    </Console>
    <!--输出到日志文件，滚动分割日志文件，自动打包gz-->
    <RollingFile name="File" fileName="logs/app.log" filePattern="logs/archives/app-%d{yyyy-MM-dd}-%i.log.gz">
      <PatternLayout pattern="${PATTERN}"/>
      <Policies>
        <!--默认一天一个文件-->
        <TimeBasedTriggeringPolicy />
        <!--一天内大于size就单独分隔-->
        <SizeBasedTriggeringPolicy size="1 GB"/>
      </Policies>
    </RollingFile>
  </Appenders>
  <Loggers>
    <!-- 添加自定义logger，一般用于区分包名的日志，不同包名不同的级别/appender -->
    <!-- additivity 意思是，调用完当前appender，是否继续调用parent logger appender，默认true-->
    <Logger name="your logger/package name" level="debug" additivity="false"/>
    <!--默认的Root Logger 级别-->
    <Root level="INFO">
      <!--这里需要区分下环境（配合maven profile之类的）-->
      <!-- 开发环境使用Console Appender，生产环境使用File Appender -->
      <AppenderRef ref="Console"/>
      <AppenderRef ref="File"/>
    </Root>
  </Loggers>
</Configuration>
```
<a name="DeEL0"></a>
### XML配置文件语法
```xml
<?xml version="1.0" encoding="UTF-8"?>;
<Configuration>
  <Properties>
    <Property name="name1">value</property>
      <Property name="name2" value="value2"/>
    </Properties>
  <filter  ... />
    <Appenders>
      <appender ... >
        <filter  ... />
        </appender>
      ...
    </Appenders>
    <Loggers>
      <Logger name="name1">
        <filter  ... />
        </Logger>
      ...
      <Root level="level">
        <AppenderRef ref="name"/>
      </Root>
    </Loggers>
</Configuration>
```
<a name="pVVIH"></a>
### Log4j2配置文件中使用自带的变量
在 log4j2.xml 中配置写入日志的文件名上加上当前主机的 IP 地址，可以使用 `${os.local-hostname}` 变量来获取本地主机名，并使用 `${sys:ip}` 变量来获取当前主机的 IP 地址。<br />以下是一个示例配置，将日志文件命名为 ${os.local-hostname}-${sys:ip}.log：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN">
  <Appenders>
    <File name="File" fileName="${os.local-hostname}-${sys:ip}.log">
      <PatternLayout pattern="%d %p %c{1.} [%t] %m%n"/>
    </File>
  </Appenders>
  <Loggers>
    <Root level="info">
      <AppenderRef ref="File"/>
    </Root>
  </Loggers>
</Configuration>
```
在log4j2.xml文件中，`${sys:serverInstance}`是一个系统属性变量，用于获取当前运行的服务器实例的名称或标识符。它通常用于在日志文件中标识日志消息来自哪个服务器实例。<br />通过使用`${sys:serverInstance}`变量，可以根据不同的服务器实例动态生成不同的日志文件名或配置不同的日志输出方式。这样，可以轻松地跟踪和区分不同服务器实例的日志信息。<br />例如，假设有一个应用程序部署在多个不同的服务器实例上，每个实例都有不同的IP地址和端口号。可以在log4j2.xml文件中使用`${sys:serverInstance}`变量来创建不同的日志文件名，以便每个服务器实例写入自己的日志文件。<br />以下是一个示例配置，将日志文件命名为${sys:serverInstance}.log：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN">
  <Appenders>
    <File name="File" fileName="${sys:serverInstance}.log">
      <PatternLayout pattern="%d %p %c{1.} [%t] %m%n"/>
    </File>
  </Appenders>
  <Loggers>
    <Root level="info">
      <AppenderRef ref="File"/>
    </Root>
  </Loggers>
</Configuration>
```
通过这样的配置，每个服务器实例将生成一个以其实例名命名的日志文件，例如"server1.log"、"server2.log"等。这样，可以轻松地跟踪和区分不同服务器实例的日志信息。<br />log4j2.xml中支持的变量

- **sys**：读取系统属性，其格式为`${sys:some.property}`或`${sys:some.property:-default_value}`，其中some.property为属性的key，default_value为默认值。
- **env**：读取系统环境变量。
- **main**：在log4j2.xml获取值。

除此之外，Log4j 2.x 还支持其他类型的变量，如PropertyLookup、JNDI、LDAP等<br />log4j2.xml中支持的变量的key在`Interpolator`的源码中。<br />`Interpolator`内部以`Map<String,StrLookup>`的方式，封装了很多`StrLookuo`对象，key则对应参数占位符prefix:key中的prefix。同时，`Interpolator`内部还保存着一个没有prefix的`StrLookup`实例，被称作默认查找器，它的键值对数据来自于log4j2.xml配置文件中的`<Properties>`元素的配置。当参数占位符{prefix:key}中的prefix。同时，`Interpolator`内部还保存着一个没有prefix的`StrLookup`实例，被称作默认查找器，它的键值对数据来自于log4j2.xml配置文件中的`<Properties>`元素的配置。当参数占位符prefix:key中的prefix。同时，`Interpolator`内部还保存着一个没有prefix的`StrLookup`实例，被称作默认查找器，它的键值对数据来自于log4j2.xml配置文件中的`<Properties>`元素的配置。当参数占位符{prefix:key}带有prefix前缀时，`Interpolator`会从指定prefix对应的`StrLookup`实例中进行key查询。当参数占位符${key}没有prefix时，`Interpolator`则会从默认查找器中进行查询。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692805445109-2aa71fce-cc4d-422f-8e56-254bba8a142d.png#averageHue=%23f8f7ee&clientId=u8d9b2f16-9686-4&from=paste&height=319&id=ubda75147&originHeight=797&originWidth=3752&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=440768&status=done&style=shadow&taskId=u1a06aed1-f7d2-4430-a0bb-4943e3a6f98&title=&width=1500.8)
<a name="jTr3a"></a>
### 创建Logger
直接使用log4j2的api：
```java
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

Logger logger = LogManager.getLogger(Log4j2Test.class);
logger.error(...);
logger.warn(...);
logger.info(...);
logger.debug(...);
logger.trace(...);
```
如果是配合slf4j使用也是可以的，只需要按照前面说的，提前做好适配，然后使用slf4j的api即可。不过如果是新系统的话，建议直接上log4j2的api吧，可以享受所有log4j2的功能，使用slf4j之类的api时，上面说的参数格式化之类的功能就无法使用了。
<a name="LS1uS"></a>
### 全异步配置（重要！！）
推荐配置log4j2 全异步（all async），在启动脚本中增加一个系统变量的配置：
```bash
-Dlog4j2.contextSelector=org.apache.logging.log4j.core.async.AsyncLoggerContextSelector
```
<a name="NzVD8"></a>
## 总结
Log4j2 如今性能最强，功能最强，而且持续更新维护。还在等什么？是时候替换logback/log4j1了！
