日志 Log4j 2<br />`Log4j 2`，顾名思义，它就是 Log4j 的升级版。<br />Log4j、SLF4J、Logback 都出自——Ceki Gulcu，但 Log4j 2 却是例外，它是 Apache 基金会的产品。<br />SLF4J 和 Logback 作为 Log4j 的替代品，在很多方面都做了必要的改进。
<a name="RVVgb"></a>
### 1、Log4j 2 的优势
1）在多线程场景下，Log4j 2 的吞吐量比 Logback 高出了 10 倍，延迟降低了几个数量级。来着Log4j 2 官方声明。<br />Log4j 2 的异步 Logger 使用的是无锁数据结构，而 Logback 和 Log4j 的异步 Logger 使用的是 ArrayBlockingQueue。对于阻塞队列，多线程应用程序在尝试使日志事件入队时通常会遇到锁争用。<br />下图说明了多线程方案中无锁数据结构对吞吐量的影响。Log4j 2 随着线程数量的扩展而更好地扩展：具有更多线程的应用程序可以记录更多的日志。其他日志记录库由于存在锁竞争的关系，在记录更多线程时，总吞吐量保持恒定或下降。这意味着使用其他日志记录库，每个单独的线程将能够减少日志记录。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604464119552-a2393a14-9b8d-41d6-8fcd-7a4cc953df2d.png#averageHue=%23f3f1f1&height=398&id=g6DZQ&originHeight=398&originWidth=757&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=757)<br />性能方面是 Log4j 2 的最大亮点，至于其他方面的一些优势，比如说下面这些，可以忽略不计，文字有多短就代表它有多不重要。<br />2）Log4j 2 可以减少垃圾收集器的压力。<br />3）支持 Lambda 表达式。<br />4）支持自动重载配置。
<a name="Y5DKr"></a>
### 2、Log4j 2 使用示例
**第一步**，在 pom.xml 文件中添加 Log4j 2 的依赖：
```xml
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-api</artifactId>
    <version>2.5</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-core</artifactId>
    <version>2.5</version>
</dependency>
```
（这个 artifactId 还是 log4j，没有体现出来 2，而在 version 中体现）<br />**第二步**，最简单的测试用例：
```java
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
public class Demo {
    private static final Logger logger = LogManager.getLogger(Demo.class);
    public static void main(String[] args) {
        logger.debug("log4j2");
    }
}
```
运行 Demo 类，可以在控制台看到以下信息：
```
ERROR StatusLogger No log4j2 configuration file found. Using default configuration: logging only errors to the console.
```
Log4j 2 没有在控制台打印“ log4j2”，提示没有为它指定配置文件。<br />这对于新手来说，很不友好，因为新手在遇到这种情况的时候，往往不知所措。日志里面虽然体现了 ERROR，但代码并没有编译出错或者运行出错。<br />通过源码，可以看得到，Log4j 2 会去寻找 4 种类型的配置文件，后缀分别是 properties、yaml、json 和 xml。前缀是 log4j2-test 或者 log4j2。<br />得到这个提示后，就可以进行第三步了。<br />**第三步**，在 resource 目录下增加 log4j2-test.xml 文件（方便和 Logback 做对比），内容如下所示：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration>
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
        </Console>
    </Appenders>
    <Loggers>
        <Root level="DEBUG">
            <AppenderRef ref="Console"/>
        </Root>
    </Loggers>
</Configuration>
```
Log4j 2 的配置文件格式和 Logback 有点相似，基本的结构为 `< Configuration>` 元素，包含 0 或多个 `< Appenders>` 元素，其后跟 0 或多个 `< Loggers>` 元素，里面再跟最多只能存在一个的 `< Root>` 元素。
<a name="hP8Ba"></a>
#### 1）配置 appender
**配置 appender**也就是配置日志的输出目的地。<br />有 Console，典型的控制台配置信息上面也看到了，简单解释一下里面 pattern 的格式：

- `%d{HH:mm:ss.SSS}` 表示输出到毫秒的时间
- `%t` 输出当前线程名称
- `%-5level` 输出日志级别，-5 表示左对齐并且固定输出 5 个字符，如果不足在右边补空格
- `%logger` 输出 logger 名称，最多 36 个字符
- `%msg` 日志文本
- `%n` 换行

顺带补充一下其他常用的占位符：

- `%F` 输出所在的类文件名，如 Demo.java
- `%L` 输出行号
- `%M` 输出所在方法名
- `%l`  输出语句所在的行数, 包括类名、方法名、文件名、行数
- `%p` 输出日志级别
- `%c` 输出包名，如果后面跟有 `{length.}` 参数，比如说 `%c{1.}`，它将输出报名的第一个字符，如 `com.itwanger` 的实际报名将只输出 `c.i`

再次运行 Demo 类，就可以在控制台看到打印的日志信息了：
```
10:14:04.657 [main] DEBUG com.itwanger.Demo - log4j2
```
<a name="nFamj"></a>
#### 2）配置 Loggers
指定 Root 的日志级别，并且指定具体启用哪一个 `Appenders`。
<a name="KiT6t"></a>
#### 3）自动重载配置
Logback 支持自动重载配置，Log4j 2 也支持，那想要启用这个功能也非常简单，只需要在 `Configuration` 元素上添加 `monitorInterval` 属性即可。
```xml
<Configuration monitorInterval="30">
...
</Configuration>
```
注意值要设置成非零，上例中的意思是至少 30 秒后检查配置文件中的更改。最小间隔为 5 秒。
<a name="HRCSK"></a>
### 3、Async 示例
除了 Console，还有 Async，可以配合文件的方式来异步写入，典型的配置信息如下所示：
```xml
<Configuration>
  <Appenders>
    <File name="DebugFile" fileName="debug.log">
      <PatternLayout>
        <Pattern>%d %p %c [%t] %m%n</Pattern>
      </PatternLayout>
    </File>
    <Async name="Async">
      <AppenderRef ref="DebugFile"/>
    </Async>
  </Appenders>
  <Loggers>
    <Root level="debug">
      <AppenderRef ref="Async"/>
    </Root>
  </Loggers>
</Configuration>
```
对比 Logback 的配置文件来看，Log4j 2 真的复杂了一些，不太好用，把这个 Async 加入到 Appenders：
```xml
<Configuration>
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
        </Console>
        <File name="DebugFile" fileName="debug.log">
            <PatternLayout>
                <Pattern>%d %p %c [%t] %m%n</Pattern>
            </PatternLayout>
        </File>
        <Async name="Async">
            <AppenderRef ref="DebugFile"/>
        </Async>
    </Appenders>
    <Loggers>
        <Root level="DEBUG">
            <AppenderRef ref="Console"/>
            <AppenderRef ref="Async"/>
        </Root>
    </Loggers>
</Configuration>
```
再次运行 Demo 类，可以在项目根路径下看到一个 debug.log 文件，内容如下所示：
```
2020-10-30 09:35:49,705 DEBUG com.itwanger.Demo [main] log4j2
```
<a name="72Z3R"></a>
### 4、RollingFile 示例
RollingFile 会根据 Triggering（触发）策略和 Rollover（过渡）策略来进行日志文件滚动。如果没有配置 Rollover，则使用 `DefaultRolloverStrategy` 来作为 RollingFile 的默认配置。<br />触发策略包含有，基于 cron 表达式（源于希腊语，时间的意思，用来配置定期执行任务的时间格式）的 `CronTriggeringPolicy`；基于文件大小的 `SizeBasedTriggeringPolicy`；基于时间的 `TimeBasedTriggeringPolicy`。<br />过渡策略包含有，默认的过渡策略 `DefaultRolloverStrategy`，直接写入的 `DirectWriteRolloverStrategy`。一般情况下，采用默认的过渡策略即可，它已经足够强大。<br />第一个基于 `SizeBasedTriggeringPolicy` 和 `TimeBasedTriggeringPolicy` 策略，以及缺省 `DefaultRolloverStrategy` 策略的配置示例：
```xml
<Configuration>
  <Appenders>
    <RollingFile name="RollingFile" fileName="rolling.log"
                 filePattern="rolling-%d{yyyy-MM-dd}-%i.log">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <Policies>
        <SizeBasedTriggeringPolicy size="1 KB"/>
      </Policies>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="debug">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```
为了验证文件的滚动策略，调整一下 Demo 类，让它多打印点日志：
```java
for (int i = 1;i < 20; i++) {
    logger.debug("Hello World");
}
```
再次运行 Demo 类，可以看到根目录下多了 3 个日志文件：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604464119412-651bb5de-93e7-4cb0-aba0-52d4b5917f8b.webp#averageHue=%23fdfefb&height=136&id=kxb5s&originHeight=136&originWidth=366&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=366)<br />结合日志文件名，再来看 RollingFile 的配置，就很容易理解了。<br />1）fileName 用来指定文件名。<br />2）filePattern 用来指定文件名的模式，它取决于过渡策略。<br />由于配置文件中没有显式指定过渡策略，因此 RollingFile 会启用默认的 `DefaultRolloverStrategy`。<br />先来看一下 `DefaultRolloverStrategy` 的属性：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604464119377-f6288f7d-4445-4dee-971b-be28309950c5.png#averageHue=%23f5f3ea&height=772&id=i5DNW&originHeight=772&originWidth=1020&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1020)<br />再来看 filePattern 的值 `rolling-%d{yyyy-MM-dd}-%i.log`，其中 `%d{yyyy-MM-dd}` 很好理解，就是年月日；其中 `%i` 是什么意思呢？<br />第一个日志文件名为 rolling.log（最近的日志放在这个里面），第二个文件名除去日期为 rolling-1.log，第二个文件名除去日期为 rolling-2.log，根据这些信息，就能猜到其中的规律。<br />其实和 `DefaultRolloverStrategy` 中的 max 属性有关，目前使用的默认值，也就是 7，那就当 rolling-8.log 要生成的时候，删除 rolling-1.log。可以调整 Demo 中的日志输出量来进行验证。<br />3）`SizeBasedTriggeringPolicy`，基于日志文件大小的时间策略，大小以字节为单位，后缀可以是 KB，MB 或 GB，例如 20 MB。<br />再来看一个日志文件压缩的示例，来看配置：
```xml
<RollingFile name="RollingFileGZ" fileName="gz/rolling.log"
             filePattern="gz/%d{yyyy-MM-dd-HH}-%i.rolling.gz">
    <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
    </PatternLayout>
    <Policies>
        <SizeBasedTriggeringPolicy size="1 KB"/>
    </Policies>
</RollingFile>
```

- fileName 的属性值中包含了一个目录 gz，也就是说日志文件都将放在这个目录下。
- filePattern 的属性值中增加了一个 gz 的后缀，这就表明日志文件要进行压缩了，还可以是 zip 格式。

运行 Demo 后，可以在 gz 目录下看到以下文件：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604464119389-790ee0ee-6437-4e7f-90ca-081369e1ec05.png#averageHue=%23f0e8e5&height=246&id=xrBrn&originHeight=246&originWidth=460&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=460)
