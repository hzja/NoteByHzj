JavaLogback
<a name="wOkBR"></a>
## 前言
组内许多服务既有同步接口也有异步脚本，接口和脚本的日志都打印在同一个日志文件中，日志繁杂给排查问题带来不少的阻碍。<br />为了解决这个问题，同事提了个按照接口分类日志文件的技术需求，也就是一个同步接口对应一个日志文件，从而将日志区分开。<br />目前组内所有服务都是使用 logback 日志框架，对这个需求产生了一定的兴趣，查找资料了解到了 logback 日志过滤器，因此有了本文，有兴趣的话也可以去官方文档：[https://logback.qos.ch/manual/filters.html](https://logback.qos.ch/manual/filters.html)
<a name="zkeKq"></a>
## 1、Logback 过滤器的分类
logback 提供两种类型的过滤器, 一种是常规过滤器(regular filters) ，另一种是全局过滤器(turbo filter)。常规过滤器与 appender 绑定， 全局过滤器与 logger context 绑定，二者的区别就是全局过滤器过滤所有 logging request ，而常规过滤器只过滤某个 appender 的 logging request
<a name="xHmX3"></a>
## 2、常规过滤器
在 logback-classic 中常规过滤器继承 `ch.qos.logback.core.filter.Filter` 抽象类，该抽象类的 `decide()`抽象方法接收一个 ILoggingEvent 参数，返回一个FilterReply枚举值。枚举值标明了过滤器对当前日志事件的过滤情况，具体处理如下表

| **枚举值** | **含义** |
| --- | --- |
| `FilterReply.ACCEPT`（通过） | 立即处理日志事件，跳过其余过滤器的调用 |
| `FilterReply.DENY`（拒绝） | 立即删除日志事件，不调用处理链下游的过滤器 |
| `FilterReply.NEUTRAL`（中立） | 调用处理链中的下一个过滤器，如果没有其他过滤器，则正常处理日志记录事件 |

<a name="Ks37x"></a>
### 2.1 日志级别过滤器 LevelFilter
LevelFilter 的过滤是基于日志事件的级别，如果日志级别等于配置的 level，则过滤器通过，否则拒绝，其代码实现如下。另外`ThresholdFilter`也是基于日志等级门槛过滤的，只不过其逻辑是当日志级别大于等于配置等级才能通过过滤器，此处不再赘述<br />需注意源码中涉及的成员变量 `this.level`、`this.onMatch`以及`this.onMismatch`都是在过滤器初始化时根据配置的值自动注入的，代码中只要提供其 set 方法即可
```java
public class LevelFilter extends AbstractMatcherFilter<ILoggingEvent> {
    Level level;

    public LevelFilter() {
    }

    public FilterReply decide(ILoggingEvent event) {
        if (!this.isStarted()) {
            return FilterReply.NEUTRAL;
        } else {
            return event.getLevel().equals(this.level) ? this.onMatch : this.onMismatch;
        }
    }

    public void setLevel(Level level) {
        this.level = level;
    }

    public void start() {
        if (this.level != null) {
            super.start();
        }

    }
}
```
以下为`LevelFilter` 配置示例，该配置需要关注的点如下：

- `**<appender>**`**标签配置指定的 **`**RollingFileAppender**`** 为滚动文件追加器，其滚动策略由 **`**<rollingPolicy>**`**标签配置，指定为 **`**TimeBasedRollingPolicy**`
- **常规过滤器需与 **`**appender**`** 绑定，过滤器元素**`**<filter>**`**需要在 **`**<appender>**`**标签之间。配置**`**LevelFilter**`** 的**`**<filter>**`**标签中的 3 个子标签配置了需要注入 **`**this.level**`**、**`**this.onMatch**`**以及**`**this.onMismatch**`** 属性的值，实现自定义过滤器时标签名可以自定义，只要与属性名相对应即可**
- `**<appender-ref>**`** 标签启用指定的 **`**appender**`
```xml
<configuration>
    <appender name="FILE_ALL" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <level>INFO</level>
            <onMatch>ACCEPT</onMatch>
            <onMismatch>DENY</onMismatch>
        </filter>
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <maxHistory>90</maxHistory>
            <fileNamePattern>${BASE_PATH_ERROR}/%d{yyyy-MM-dd}.%i.log.gz</fileNamePattern>
            <timeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
                <maxFileSize>200MB</maxFileSize>
            </timeBasedFileNamingAndTriggeringPolicy>
        </rollingPolicy>
        <encoder>
            <pattern> %d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{30} - %msg%n</pattern>
        </encoder>
    </appender>
  
    <root level="INFO">
      <appender-ref ref="FILE_ALL" />
    </root>
  
</configuration>
```
<a name="rQQP8"></a>
### 2.2 日志评估过滤器 `EvaluatorFilter`
`EvaluatorFilter` 是一个抽象类，它包含了一个 `EventEvaluator` 对象，这个对象是日志评估过滤器实现的关键。它会封装配置在`<expression>`标签中的过滤逻辑，在处理日志事件时负责过滤条件的判断<br />`EventEvaluator`主要有两个实现类，分别是 `GEventEvaluator` 和 `JaninoEventEvaluator`。`GEventEvaluator` 接收 Groovy 语言的条件表达式作为判断条件，`JaninoEventEvaluator` 接收一个 java 的判断表达式作为判断条件，logback 默认使用的是 `JaninoEventEvaluator`，其依赖于 Janino 相关库
<a name="gdH2G"></a>
### `JaninoEventEvaluator` 的使用
使用 `JaninoEventEvaluator`评估器时 logback 会自动导出日志事件对象的属性到 `evaluation` 表达式中，所以可以直接使用以下属性

| **属性名** | **数据类型** | **含义** |
| --- | --- | --- |
| event | ILoggingEvent | 原始的 `ILoggingEvent` 对象，可以通过该对象获取各项属性，例如 `event.getMessage()` 相当于以下的 message |
| message | String | logging request 的原始信息，例如代码`I.info("Hi {}", name)`打印日志时 message 的值就是 “Hi {}” |
| formattedMessage | String | 格式化后的完整信息，例如`String name="Nathan";I.info("hello {}", name);`，则 formattedMessage 的值就是"hello Nathan" |
| logger | String | logger 的名称，一般为该日志所在类的全类名 |
| loggerContext | LoggerContextVO | logging event 所属的 LoggerContext 对象 |
| level | int | logging event 的等级，可以直接使用 level > INFO 的方式判断日志等级 |
| timeStamp | long | logging event 产生的时间 |
| marker | Marker | logging request 的 Marker 标签，需要注意 marker可以为空，所以使用前需要判断 Marker 是否为空，避免空指针异常 |
| mdc | Map | logging event 的 MDC 值，可以通过 `mdc.get("key")` 的方式获取指定 key 的 value，但是需要注意，获取的是object对象，需要按需求强制转换类型，例如：`((String) mdc.get("key")).contains("value")` |
| throwable | java.lang.Throwable | logging event 的异常信息，如果没有异常关则值为 null。throwable 不支持序列化，所以在远程日志服务器中该值为 null |
| throwableProxy | IThrowableProxy | logging event 异常的代理。如果没有异常，则值为null，但它支持序列化 |

`JaninoEventEvaluator` 的使用配置如下所示，`<expression>`标签包裹的表达式即为日志过滤的判断逻辑。根据其源代码实现来看，如果未检测到表达式中的 return 字符串将直接在表达式首尾添上 return 和 分号
```xml
<configuration>
  <appender name="FILE_ALL" class="ch.qos.logback.core.rolling.RollingFileAppender">
      <filter class="ch.qos.logback.core.filter.EvaluatorFilter">     
        <evaluator> <!-- defaults to type ch.qos.logback.classic.boolex.JaninoEventEvaluator -->
          <expression>return message.contains("nathan");</expression>
        </evaluator>
        <OnMismatch>NEUTRAL</OnMismatch>
        <OnMatch>ACCEPT</OnMatch>
      </filter>
      <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
          <maxHistory>90</maxHistory>
          <fileNamePattern>${BASE_PATH_ERROR}/%d{yyyy-MM-dd}.%i.log.gz</fileNamePattern>
          <timeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
              <maxFileSize>200MB</maxFileSize>
          </timeBasedFileNamingAndTriggeringPolicy>
      </rollingPolicy>
      <encoder>
          <pattern> %d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{30} - %msg%n</pattern>
      </encoder>
  </appender>

  <root level="INFO">
    <appender-ref ref="FILE_ALL" />
  </root>

</configuration>
```
<a name="u00vL"></a>
## 3、全局过滤器
在 logback-classic 中全局过滤器都继承自抽象类 `ch.qos.logback.classic.turbo.TurboFilter` ，其实 Trubo Filter 与 Regular Filter 只有两点主要的不同：

- **TurboFilter 对象与 logging context 绑定，因此它会处理所有的 logging request，而不是单独的appender，过滤范围更广**
- **TurboFilter 过滤器在 **`**LoggingEvent**`** 对象创建之前就已经被调用，所以并不需要日志事件来过滤logging request，因此会有更好的性能表现**

logback 提供了一些常用的 `TurboFilter`：

- `**MDCFilter**`**：通过 MDC 过滤，主要校验 MDC 中指定 key-value 是否存在**
- `**DynamicThresholdFilter**`**：通过 MDC 或 level 过滤**
- `**MarkerFilter**`**：通过 marker 标签过滤**

以下为全局过滤器使用的配置示例
```xml
<configuration>
    <turboFilter class="ch.qos.logback.classic.turbo.DynamicThresholdFilter">
     <Key>username</Key>
     <DefaultThreshold>DEBUG</DefaultThreshold>
     <MDCValueLevelPair>
       <value>nathan</value>
       <level>DEBUG</level>
     </MDCValueLevelPair>
   </turboFilter>
  
   <turboFilter class="ch.qos.logback.classic.turbo.MDCFilter">
     <MDCKey>username</MDCKey>
     <Value>nathan</Value>
     <OnMatch>ACCEPT</OnMatch>
   </turboFilter>
        
  <turboFilter class="ch.qos.logback.classic.turbo.MarkerFilter">
    <Marker>nathan</Marker>
    <OnMatch>DENY</OnMatch>
  </turboFilter>

 <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <encoder>
      <pattern>%date [%thread] %-5level %logger - %msg%n</pattern>
    </encoder>
  </appender>

  <root level="INFO">
    <appender-ref ref="STDOUT"/>
  </root>  
  
</configuration>
```
<a name="e9IfK"></a>
## 4、日志分类归档方案
按照接口的名称分类日志，将接口方法执行期间的所有日志输出到指定到接口文件中，其需要解决的主要问题有两个：

- **怎么分辨哪些日志是哪个接口被调用过程中打印的?**
- **接口的日志怎么输出到指定到文件？**
<a name="tCR9O"></a>
### 问题1方案
对于问题1，首先想到的就是对特定的日志打上特定的标记，可以参考文章 Slf4j 中的 MDC ，借助 MDC 把接口名称以一个 key 存储下来，则可据此区分不同的接口的日志
<a name="mFvNQ"></a>
### 问题2方案
对于问题2，本文介绍的 logback 日志过滤器便起到了作用。可以为每个接口重新配置一个 appender，其关键点如下：

- **使用 **`**EvaluatorFilter**`** 过滤器，配置**`**<expression>**`**标签表达式通过 MDC 获取指定 key 的 value 是否等于目标值**
- **配置日志的滚动策略中 **`**<fileNamePattern>**`** 日志文件名标签为指定的格式**
- **配置 **`**<appender-ref>**`** 标签启用指定的 appender**
```xml
<configuration>
  <appender name="FILE_METHOD" class="ch.qos.logback.core.rolling.RollingFileAppender">
    <filter class="ch.qos.logback.core.filter.EvaluatorFilter">     
      <evaluator> <!-- defaults to type ch.qos.logback.classic.boolex.JaninoEventEvaluator -->
        <expression>return ((String)mdc.get("key")).equals("method");</expression>
      </evaluator>
      <OnMismatch>NEUTRAL</OnMismatch>
      <OnMatch>ACCEPT</OnMatch>
    </filter>
    <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
      <maxHistory>90</maxHistory>
      <fileNamePattern>${BASE_PATH_ERROR}/%d{yyyy-MM-dd}.%i.log.method.gz</fileNamePattern>
      <timeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
        <maxFileSize>200MB</maxFileSize>
      </timeBasedFileNamingAndTriggeringPolicy>
    </rollingPolicy>
    <encoder>
      <pattern> %d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{30} - %msg%n</pattern>
    </encoder>
  </appender>

  <root level="INFO">
    <appender-ref ref="FILE_METHOD" />
  </root>
```
