Java性能测试 JMH Arthas
<a name="J50pc"></a>
## JMH简介
第一个问题就是，测试的方法是否标准。在编写代码时用的是死循环+前后“掐秒表”的方式。假如要再加个多线程的测试，还需要搞一个线程池，除了代码本身的逻辑还要关心测试的逻辑。可能会想，有没有一款工具能够从测试逻辑中彻底解放出来，只需要关心需要测试的代码逻辑。JMH就是这样一款Java的测试框架。下面是JMH的官方定义<br />JMH 是一个面向 Java 语言或者其他 Java 虚拟机语言的性能基准测试框架<br />这里面需要注意的是，JMH所测试的方法约简单越好，依赖越少越好，最适合的场景就是，测试两个集合put，get性能，例如ArrayList与LinkedList的对比等，这里需要测试的是批量打一批日志所需要的时间，也基本符合使用JMH的测试场景。下面是测试代码，bench框架代码以及主函数。

- 待测试方法
```java
public class LogBenchMarkWorker {

    private LogBenchMarkWorker() {}

    private static class LogBenchMarkWorkerInstance {
        private static final LogBenchMarkWorker instance = new LogBenchMarkWorker();
    }

    public static LogBenchMarkWorker getInstance() {
        return LogBenchMarkWorkerInstance.instance;
    }

    private static final String LOGGER_DEFAULT_NAME = "defaultLogger";
    private static final String LOGGER_INCLUDE_LOCATION = "includeLocationLogger";
    private static final Logger LOGGER = LoggerFactory.getLogger(LOGGER_DEFAULT_NAME);
    private static long BATCH_SIZE = 10000;

    public void logBatch() {
        for (int i = 0; i < BATCH_SIZE; i++) {
            LOGGER.info("msg is {}", i);
        }
    }
}
```
可以看到待测试方法非常简单，就是单批次一次性打印10000条日志的操作，所以并没有需要额外说明的部分。下面再来看benchmark部分。
```java
public class LogBenchMarkMain {

    @Benchmark
    @BenchmarkMode(Mode.AverageTime)
    @Fork(value = 1)
    @Threads(1)
    public void testLog1() {
        LogBenchMarkWorker.getInstance().logBatch();
    }

    @Benchmark
    @BenchmarkMode(Mode.AverageTime)
    @Fork(value = 1)
    @Threads(4)
    public void testLog4() {
        LogBenchMarkWorker.getInstance().logBatch();
    }

    @Benchmark
    @BenchmarkMode(Mode.AverageTime)
    @Fork(value = 1)
    @Threads(8)
    public void testLog8() {
        LogBenchMarkWorker.getInstance().logBatch();
    }

    @Benchmark
    @BenchmarkMode(Mode.AverageTime)
    @Fork(value = 1)
    @Threads(16)
    public void testLog16() {
        LogBenchMarkWorker.getInstance().logBatch();
    }
}
```
在这段代码中，就会发现有了一些JMH中特有的东西，下面进行简要介绍。<br />Benchmark注解：标识在某个具体方法上，表示这个方法将是一个被测试的最小方法，在JMH中成为一个OPS<br />BenmarkMode:测试类型，JMH提供了几种不同的Mode<br />    Throughput:整体吞吐量<br />    AverageTime:调用的平均时间，每次OPS执行的时间<br />    SampleTime:取样，给出不同比例的ops时间，例如99%的ops时间，99.99%的ops时间<br />fork：fork的次数，如果设置为2，JMH会fork出两个进程来测试<br />Threads：很容易理解，这个参数表示这个方法同时被多少个线程执行<br />在上面的代码中，定义了4个待测试的方法，方法的Fork，BenchmarkMode均为测试单次OPS的平均时间，但4个方法的线程数不同。<br />除了这几个参数，还有几个参数，会在main函数里面来讲，main代码如下所示
```java
public class Main {
    public static void main(String[] args) throws RunnerException {
        Options options = new OptionsBuilder()
                .include(LogBenchMarkMain.class.getSimpleName())
                .warmupIterations(5)
                .measurementIterations(5)
                .output("logs/BenchmarkCommon.log")
                .build();
        new Runner(options).run();
    }
}
```
可以看到，在main函数中，就是要设置JMH的基础配置，这里面的几个配置参数含义如下：<br />include:benchmark所在类的名字，可以使用正则表达<br />warmupIteration:预热的迭代次数，这里为什么要预热的原因是由于JIT的存在，随着代码的运行，会动态对代码的运行进行优化。因此在测试过程中需要先预热几轮，让代码运行稳定后再实际进行测试<br />measurementIterations:实际测试轮次<br />output:测试报告输出位置<br />分别用两种logger运行一下测试，查看性能测试报告对比<br />使用普通logger
```bash
LogBenchMarkMain.testLog1   avgt    5  0.006 ± 0.001   s/op
LogBenchMarkMain.testLog16  avgt    5  0.062 ± 0.026   s/op
LogBenchMarkMain.testLog4   avgt    5  0.006 ± 0.002   s/op
LogBenchMarkMain.testLog8   avgt    5  0.040 ± 0.004   s/op
```
使用了`INCLUDE_LOCATION`的logger
```bash
LogBenchMarkMain.testLog1   avgt    5  0.379 ± 0.007   s/op
LogBenchMarkMain.testLog16  avgt    5  1.784 ± 0.670   s/op
LogBenchMarkMain.testLog4   avgt    5  0.378 ± 0.003   s/op
LogBenchMarkMain.testLog8   avgt    5  0.776 ± 0.070   s/op
```
这里可以看到，性能差距立现。使用`INCLUDE_LOCATION`的性能要明显低于使用普通logger的性能。这时一定很好奇，并且问一句“到底慢在哪”！！
<a name="V8Iyz"></a>
## Arthas 在代码在运行时到底做了什么
Arthas是阿里开源的一款java调试神器，与greys类似，不过有着比greys更加强大的功能，例如，可以直接绘制java方法调用的火焰图等。这两个工具的原理都是使用了Java强大的字节码技术。
<a name="uKTyt"></a>
### 实际操作
talk is cheap, show me your code,既然是工具，直接进行实际操作。在本机运行一开始的程序，然后讲解arthas的使用方法。<br />首先通过jps找到程序的进程号，然后直接通过arthas给到的as.sh对运行的程序进行字节码增强，然后启动arthas，命令如下
```bash
./as.sh pid
```
![2021-10-06-17-30-52-593049.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513391559-acf45a33-b815-4aea-9bfe-e17629d7cbd4.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=ufd3b7e80&originHeight=705&originWidth=1080&originalType=binary&ratio=1&size=159349&status=done&style=none&taskId=u5e97be1e-e30b-4ca6-9341-3d0ed9c5e2e)<br />这个就是arthas的启动界面了，使用help命令查看工具所支持的功能<br />![2021-10-06-17-30-53-066058.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513391747-58c9e4bf-cbc2-4265-8e32-9b800a7fc2d5.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=QusCG&originHeight=840&originWidth=1080&originalType=binary&ratio=1&size=388630&status=done&style=none&taskId=u936cfc0e-9aa8-4e4c-86e4-88bc99fefa8)<br />可以看到，arthas支持查看当前jvm的状态，查看当前的线程状态，监控某些方法的调用时间，trace，profile生成火焰图等，功能一应俱全<br />这里也只将几个比较常用的命令，其他的命令如果大家感兴趣可以详见官网arthas官网。这篇文章主要介绍下面几个功能

- 反编译代码
- 监控某个方法的调用
- 查看某个方法的调用和返回值
- trace某个方法
<a name="McqyZ"></a>
### 监控方法调用
这个主要的命令为monitor，根据官网的介绍，常用的使用方法为
```bash
monitor -c duration className methodName
```
其中`duration`代表每隔几秒展示一次统计结果，即单次的统计周期，className就是类的全限定名，methodname就是方法的名字，这里面查看的方法是`Logger`类的`info`方法，分别对使用两种不同logger的info方法。这里面的类是org.slf4j.Logger，方法时info，监控语句为
```bash
monitor -c 5 org.slf4j.Logger info
```
监控结果如下

- 使用普通appender

![2021-10-06-17-30-53-354043.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513391548-c74b376c-44d0-4fda-af73-fbf69f335065.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=auWxu&originHeight=627&originWidth=1080&originalType=binary&ratio=1&size=117028&status=done&style=none&taskId=u16964530-c689-4f1d-9201-f3ed57c4d7b)

- 使用include appender

![2021-10-06-17-30-53-836043.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513417366-da613064-d408-498a-9b04-f6842828f611.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=u3305b5ef&originHeight=434&originWidth=1080&originalType=binary&ratio=1&size=81272&status=done&style=none&taskId=ua10948a0-64ab-46f4-aa29-a60a1df5801)<br />可以看到，使用include appeder的打印日志方法要比普通的appender高出了3倍，这就不禁让人有了疑问，究竟这两个方法各个步骤耗时如何呢。下面就介绍第二条命令，trace方法。
<a name="coaaP"></a>
### `trace`命令 & `jad`命令
这两个程序的log4j2配置文件如下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!--status：日志等级   monitorInterval：更新配置文件的时间间隔，单位秒-->
<configuration status="warn" monitorInterval="30">
    <!--定义所有的appender -->
    <appenders>
        <!--这个输出控制台的配置 -->
        <Console name="console" target="SYSTEM_OUT">
            <!--控制台只输出level及以上级别的信息（onMatch），其他的直接拒绝（onMismatch） -->
            <ThresholdFilter level="warn" onMatch="ACCEPT" onMismatch="DENY"/>
            <!--日志打印格式 -->
            <PatternLayout pattern="[%d{HH:mm:ss.SSS}] [%-5p] %l - %m%n"/>
        </Console>


        <Async name="AsyncDefault" blocking="false" includeLocation="false">
            <AppenderRef ref="fileAppender"/>
        </Async>

        <Async name="AsyncIncludeLocation" blocking="false" includeLocation="true">
            <AppenderRef ref="fileAppender"/>
        </Async>

        <!--文件会打印出所有信息，这个log每次运行程序会自动清空，由append属性决定，这个也挺有用的，适合临时测试用 -->
        <!--append为TRUE表示消息增加到指定文件中，false表示消息覆盖指定的文件内容，默认值是true -->
        <File name="fileAppender" fileName="log/test.log" append="false">
            <PatternLayout pattern="[%d{HH:mm:ss.SSS}] [%-5p] %l - %m%n"/>
        </File>

        <!--添加过滤器ThresholdFilter,可以有选择的输出某个级别以上的类别  onMatch="ACCEPT" onMismatch="DENY"意思是匹配就接受,否则直接拒绝  -->
        <File name="ERROR" fileName="logs/error.log">
            <ThresholdFilter level="error" onMatch="ACCEPT" onMismatch="DENY"/>
            <PatternLayout pattern="[%d{yyyy.MM.dd 'at' HH:mm:ss z}] [%-5p] %l - %m%n"/>
        </File>

        <!--这个会打印出所有的信息，每次大小超过size，则这size大小的日志会自动存入按年份-月份建立的文件夹下面并进行压缩，作为存档 -->
        <RollingFile name="rollingFile" fileName="logs/app.log"
                     filePattern="logs/$${date:yyyy-MM}/web-%d{yyyy-MM-dd}.log.gz">
            <PatternLayout pattern="[%d{yyyy-MM-dd HH:mm:ss}] [%-5p] %l - %m%n"/>
            <Policies>
                <TimeBasedTriggeringPolicy modulate="true" interval="1"/>
            </Policies>
            <DefaultRolloverStrategy>
                <Delete basePath="logs" maxDepth="2">
                    <IfFileName glob="*/*.log.gz" />
                    <IfLastModified age="7d" />
                </Delete>
            </DefaultRolloverStrategy>
        </RollingFile>
    </appenders>

    <!--然后定义logger，只有定义了logger并引入的appender，appender才会生效 -->
    <loggers>
        <!--过滤掉spring和mybatis的一些无用的DEBUG信息-->
        <logger name="defaultLogger" additivity="false">
            <appender-ref ref="AsyncDefault"></appender-ref>
        </logger>

        <logger name="includeLocationLogger" additivity="false">
            <appender-ref ref="AsyncIncludeLocation"></appender-ref>
        </logger>

        <!--建立一个默认的root的logger -->
        <root level="INFO">

        </root>
    </loggers>

</configuration>
```
都是用了一个`AsyncAppender`套用了一个`FileAppender`。查看`fileAppender`，发现二者相同完全没区别，只有asyncAppender中的一个选项有区别，这就是`includeLocation`，一个是false，另一个是true。至于这个参数的含义，暂时不讨论，现在知道问题可能出在`AsyncAppender`里面，但是该如何验证呢。trace命令就有了大用场。<br />trace命令的基本用法与monitor类似，其中主要的一个参数是-n则是代表trace多少次的意思
```bash
trace -n trace_times className methodName
```
任何一个appender，最核心的方法就是他的append方法。所以分别trace两个程序的`append`方法。
```bash
trace -n 5 org.apache.logging.log4j.core.appender.AsyncAppender append
```
trace结果如下

- 使用普通appender

![2021-10-06-17-30-54-158060.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513417528-37dcb323-a55e-45a4-ba16-212c47b6829f.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=TRdz6&originHeight=692&originWidth=873&originalType=binary&ratio=1&size=192270&status=done&style=none&taskId=u941896bd-25e2-4600-ba62-6307f16d7b4)

- 使用include appender

![2021-10-06-17-30-54-410050.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513417548-b1f11795-7f05-4f00-833d-79e9eb27064b.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=dXscB&originHeight=723&originWidth=895&originalType=binary&ratio=1&size=197353&status=done&style=none&taskId=u217db5cf-583f-426f-b23c-137795995ec)<br />立刻可以发现，两个trace的热点方法不一样，在使用include的`appender`中，耗时最长的方法时org.apache.logging.log4j.core.impl.Log4jLogEvent类中的`createMemento`方法，那么怎么才能知道这个方法到底做了啥呢，那么就看下一个常用命令jad，这个命令能够反编译出对应方法的代码。这里jad一下上面说的那个createMemento方法，命令很简单
```bash
jad org.apache.logging.log4j.core.impl.Log4jLogEvent createMemento
```
结果如下<br />![2021-10-06-17-30-54-534041.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513462773-b4c69d7c-02c8-4547-b978-2045090e301a.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=ucf58c802&originHeight=53&originWidth=608&originalType=binary&ratio=1&size=9966&status=done&style=none&taskId=u7af9688c-6259-4309-b160-479227ac4ce)<br />可以发现，这个方法中有个`includeLocation`参数，这个和看到的两个appender的唯一不同的配置相吻合，此时应该有这个猜想，会不会就是这个参数导致的呢？为了验证这个猜想，引出下一个命令，watch
<a name="SSMFr"></a>
### watch命令
watch命令能够观察到某个特定方法的入参，返回值等信息，使用这个命令查看一下这个`createMemento`方法的入参，如果两个程序的入参不同，那基本可以断定是这个原因引起命令如下
```bash
watch org.apache.logging.log4j.core.impl.Log4jLogEvent createMemento "params" -x 2 -n 5 -b -f
```
这里面的参数含义如下<br />-x 参数展开层次<br />-n 执行次数<br />-b 查看方法调用前状态<br />-f 方法调用后<br />其中的param代表查看方法的调用参数列表，还有其他的监控项详见官网官网<br />最终watch结果如下

- 使用普通`logger`

![2021-10-06-17-30-54-653042.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513462815-415a14b7-6223-45b0-ab92-512f4be14271.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=L0rCk&originHeight=882&originWidth=964&originalType=binary&ratio=1&size=58709&status=done&style=none&taskId=u700f7340-3283-4cf7-903a-7ec2c4ab259)

- 使用`include`

![2021-10-06-17-30-54-925047.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513462823-2ff68886-0a74-4ad9-87d7-4ef611aa2ed6.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=SUcKe&originHeight=641&originWidth=1080&originalType=binary&ratio=1&size=84831&status=done&style=none&taskId=u1ddfbfd9-e587-4932-af8f-613edf5625c)<br />果不其然，这两个参数果然是一个true一个false，简单看下这个参数是如何传进来的，jad一下`AsyncAppender`的`append`方法<br />![2021-10-06-17-30-55-217048.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513495920-54c7a6e6-2278-403a-bed9-76768402c65f.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=u98440c7a&originHeight=435&originWidth=1080&originalType=binary&ratio=1&size=165065&status=done&style=none&taskId=u73d738c6-998d-4d28-95cc-304488e15a2)<br />可以发现这个`includeLocation`正是`appender`的一个属性，也就是xml中配置的那个属性。查看官网的相关分析，看到这个参数会使log的性能下降5–10倍<br />![2021-10-06-17-30-55-452042.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513495744-1258ea92-4265-4d37-b556-46e11b5e9c94.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=kCpuo&originHeight=78&originWidth=1080&originalType=binary&ratio=1&size=16928&status=done&style=shadow&taskId=u6b40390c-42d5-4632-8401-f65fee42813)<br />不过为了一探究竟，还是静态跟了一下这段代码<br />这个`includeLocation`会在event的`createMemento`中被用到，在序列化生成对象时会创建一个`LogEventProxy`，代码如下
```java
public LogEventProxy(final LogEvent event, final boolean includeLocation) {
    this.loggerFQCN = event.getLoggerFqcn();
    this.marker = event.getMarker();
    this.level = event.getLevel();
    this.loggerName = event.getLoggerName();

    final Message msg = event.getMessage();
    this.message = msg instanceof ReusableMessage
            ? memento((ReusableMessage) msg)
            : msg;
    this.timeMillis = event.getTimeMillis();
    this.thrown = event.getThrown();
    this.thrownProxy = event.getThrownProxy();
    this.contextData = memento(event.getContextData());
    this.contextStack = event.getContextStack();
    this.source = includeLocation ? event.getSource() : null;
    this.threadId = event.getThreadId();
    this.threadName = event.getThreadName();
    this.threadPriority = event.getThreadPriority();
    this.isLocationRequired = includeLocation;
    this.isEndOfBatch = event.isEndOfBatch();
    this.nanoTime = event.getNanoTime();
}
```
如果`includeLocation`为`true`，那么就会调用`getSource`函数，跟进去查看，代码如下
```java
public StackTraceElement getSource() {
    if (source != null) {
        return source;
    }
    if (loggerFqcn == null || !includeLocation) {
        return null;
    }
    source = Log4jLogEvent.calcLocation(loggerFqcn);
    return source;
}
public static StackTraceElement calcLocation(final String fqcnOfLogger) {
    if (fqcnOfLogger == null) {
        return null;
    }
    // LOG4J2-1029 new Throwable().getStackTrace is faster than Thread.currentThread().getStackTrace().
    final StackTraceElement[] stackTrace = new Throwable().getStackTrace();
    StackTraceElement last = null;
    for (int i = stackTrace.length - 1; i > 0; i--) {
        final String className = stackTrace[i].getClassName();
        if (fqcnOfLogger.equals(className)) {
            return last;
        }
        last = stackTrace[i];
    }
    return null;
}
```
可以看到他会从整个的调用栈中去寻找调用这个方法的代码行，其性能可想而知。用arthas监控一下，验证一下。<br />首先`trace crateMemento`方法
```bash
trace -n 5 org.apache.logging.log4j.core.impl.Log4jLogEvent createMemento
```
![2021-10-06-17-30-55-641063.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513495889-a5784dce-f143-497b-adb5-110222231afe.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=T9BHo&originHeight=107&originWidth=1080&originalType=binary&ratio=1&size=78408&status=done&style=none&taskId=u95db9b68-667c-46c8-8633-e0ae261dfee)<br />发现热点方法时org.apache.logging.log4j.core.impl.Log4jLogEvent的`serialize()`，继续trace下去
```bash
trace -n 5 org.apache.logging.log4j.core.impl.Log4jLogEvent serialize
```
![2021-10-06-17-30-55-864143.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513529429-aa16c6aa-8f3a-49f2-a9cd-d0f0afb2eccc.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=u5c0ee76f&originHeight=74&originWidth=1080&originalType=binary&ratio=1&size=56443&status=done&style=none&taskId=ube5f3168-95ab-4482-84fe-793de0610aa)<br />看到热点是`org.apache.logging.log4j.core.impl.Log4jLogEvent:LogEventProxy`的构造方法，继续trace
```bash
trace -n 5 org.apache.logging.log4j.core.impl.Log4jLogEvent$LogEventProxy <init>
```
![2021-10-06-17-30-56-355025.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513529608-358b0e37-8059-4136-945b-1306958febc0.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=H0lBW&originHeight=440&originWidth=1080&originalType=binary&ratio=1&size=344130&status=done&style=none&taskId=ud106a1cf-55d5-4a25-b377-208ecd49dba)<br />发现是`getSource`方法，继续
```bash
trace -n 5 trace -n 5 org.apache.logging.log4j.core.LogEvent getSource
```
![2021-10-06-17-30-56-744067.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633513529479-9a3f154a-74b4-4b55-b3df-95885c732d49.png#clientId=ue8dbe3d1-1f99-4&from=ui&id=e4z16&originHeight=90&originWidth=1080&originalType=binary&ratio=1&size=60174&status=done&style=none&taskId=u1d60e01e-1dd9-4bfd-b83a-3278fe5033e)<br />热点终于定位到了，是org.apache.logging.log4j.core.impl.Log4jLogEvent的`calcLocation`函数，和静态跟踪的代码一样。
