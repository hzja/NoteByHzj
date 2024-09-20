Java SLF4J 日志<br />
<a name="3lwRy"></a>
### 1、SLF4J 是什么
SLF4J 是 Simple Logging Facade for Java 的缩写（for≈4），也就是简易的日志门面，以外观模式（Facade pattern，一种设计模式，为子系统中的一组接口提供一个统一的高层接口，使得子系统更容易使用）实现，支持 java.util.logging、Log4J 和 Logback。<br />SLF4J 的作者就是 Log4J 和 Logback 的作者。1996 年初，欧洲安全电子市场项目决定编写自己的跟踪 API，最后该 API 演变成了 Log4j，已经推出就备受宠爱。<br />2002 年 2 月，Sun 推出了自己的日志包 java.util.logging（可称 JUL），据说实现思想借鉴了 Log4j，毕竟此时的 Log4j 已经很成熟了。<br />2002 年 8 月，Apache 就推出了自己的日志包，也就是阿里巴巴开发手册上提到的 JCL（Jakarta Commons Logging）。JCL 的野心很大，它在 JUL 和 Log4j 的基础上提供了一个抽象层的接口，方便使用者在 JUL 和 Log4j 之间切换。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604467299603-65804cec-e7e7-4ccd-90ca-8ceb27e805c9.png#align=left&display=inline&height=500&originHeight=500&originWidth=890&size=0&status=done&style=shadow&width=890)<br />但 JCL 好像并不怎么招人喜欢，Ceki Gulcu 也觉得 JCL 不好，他在 2005 年写了一个名叫 SLF4J 的新项目，SLF4J 只有接口，没有实现，他在 SLF4J 和 JUL、Log4j、JCL 之间搭了三座桥：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604467299595-83327e09-4d37-41bb-9e96-b31da917ca1d.png#align=left&display=inline&height=615&originHeight=615&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="XkS8i"></a>
### 2、SLF4J 解决了什么痛点
春秋战国的时候，每个国家都有自己的货币，用别国的货币也不合适，对吧？那在发生贸易的时候就比较麻烦了，货币不统一，就没法直接交易，因为货币可能不等价。<br />那秦始皇统一六国后，就推出了新的货币政策，全国都用一种货币，那之前的问题就解决掉了。<br />同样的道理，日志系统有 JUL、JCL，Ceki Gulcu 自己又写了 2 种，Log4j 和 Logback，各有各的优缺点，再加上使用者千千万，这就导致不同的应用可能会用不同的日志系统。<br />假设正在开发一套系统，打算用 SLF4J 作为门面，Log4j 作为日志系统，在项目中使用了 A 框架，而 A 框架的门面是 JCL，日志系统是 JUL，那就相等于要维护两套日志系统<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604467299591-c414bb4f-3d04-4a3c-b2ff-e50036967b67.png#align=left&display=inline&height=754&originHeight=754&originWidth=692&size=0&status=done&style=shadow&width=692)<br />Ceki Gulcu 想到了这个问题，并且解决了！来看 SLF4J 官网给出的解决方案。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604467299645-f8ae69ac-1a12-434b-b0ce-d78bceb92201.png#align=left&display=inline&height=598&originHeight=598&originWidth=768&size=0&status=done&style=shadow&width=768)

- 使用 jcl-over-slf4j.jar 替换 commons-logging.jar
- 引入 jul-to-slf4j.jar

为了模拟这个过程，来建一个使用 JCL 的项目。<br />第一步，在 pom.xml 文件中引入 commons-logging.jar：
```xml
<dependency>
    <groupId>commons-logging</groupId>
    <artifactId>commons-logging</artifactId>
    <version>1.2</version>
</dependency>
```
第二步，新建测试类：
```java
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
/**
 * @author Fcant
 */
public class Demo {
    private static Log logger = LogFactory.getLog(Demo.class);
    public static void main(String[] args) {
        logger.info("jcl");
    }
}
```
该类会通过 LogFactory 获取一个 Log 对象，并且使用 info() 方法打印一行日志。<br />调试这段代码的过程中可以发现，Log 的实现有四种：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604467299639-dc058c0f-833b-479d-8db5-a4ab90cdd072.png#align=left&display=inline&height=254&originHeight=254&originWidth=1072&size=0&status=done&style=shadow&width=1072)<br />如果没有绑定 Log4j 的话，就会默认选择 Jdk14Logger——它返回的 Logger 对象，正是 java.util.logging.Logger，也就是 JUL。<br />因此，就可以在控制台看到以下信息：
```
10月 21, 2020 3:13:30 下午 com.itwanger.Demo main
信息: jcl
```
怎么把使用 JCL 的项目改造成使用 SLF4J 的呢？<br />第三步，使用 jcl-over-slf4j.jar 替换 commons-logging.jar，并加入 jul-to-slf4j.jar、slf4j-log4j12.jar（会自动引入 slf4j-api.jar 和 log4j.jar）：
```xml
<dependency>
    <groupId>org.slf4j</groupId>
    <artifactId>jcl-over-slf4j</artifactId>
    <version>1.7.25</version>
</dependency>
<dependency>
    <groupId>org.slf4j</groupId>
    <artifactId>jul-to-slf4j</artifactId>
    <version>1.7.29</version>
</dependency>
<dependency>
    <groupId>org.slf4j</groupId>
    <artifactId>slf4j-log4j12</artifactId>
    <version>1.7.25</version>
</dependency>
```
第四步，在 resources 目录下创建 log4j.properties 文件，内容如下所示：
```
### 设置###
log4j.rootLogger = debug,stdout,D
### 输出信息到控制台 ###
log4j.appender.stdout = org.apache.log4j.ConsoleAppender
log4j.appender.stdout.Target = System.out
log4j.appender.stdout.layout = org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern = [%-5p] %d{yyyy-MM-dd HH:mm:ss,SSS} method:%l%n%m%n
### 输出DEBUG 级别以上的日志到=debug.log ###
log4j.appender.D = org.apache.log4j.DailyRollingFileAppender
log4j.appender.D.File = debug.log
log4j.appender.D.Append = true
log4j.appender.D.Threshold = DEBUG 
log4j.appender.D.layout = org.apache.log4j.PatternLayout
log4j.appender.D.layout.ConversionPattern = %d{yyyy-MM-dd HH:mm:ss}  [ %t:%r ] - [ %p ]  %m%n
```
再次运行 Demo 类，可以发现 target 目录下会生成一个名叫 debug.log 的文件，内容如下所示：
```
2020-10-21 15:32:06  [ main:0 ] - [ INFO ]  jcl
```
并且可以在控制台看到以下信息：
```
[INFO ] 2020-10-21 15:32:06,192 method:com.itwanger.Demo.main(Demo.java:12)
jcl
```
仔细对比一下，就可以发现，这次输出的格式和之前不一样，这就是因为 Log4j 和 JUL 的日志格式不同导致的。<br />另外有没有发现？并没有改动测试类 Demo，它里面使用的仍然是 JCL 获取 Log 的方式：
```java
private static Log logger = LogFactory.getLog(Demo.class);
```
但输出的格式已经切换到 Log4j 了！<br />SLF4J 除了提供这种解决方案，绑定 Log4j 替换 JUL 和 JCL；还提供了绑定 Logback 替换 JUL、JCL、Log4j 的方案：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604467299671-bd8b1ccd-e0fd-4cbc-b10a-16d8a32f3364.png#align=left&display=inline&height=508&originHeight=508&originWidth=788&size=0&status=done&style=shadow&width=788)<br />还有绑定 JUL 替换 JCL 和 Log4j 的方案：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604467299698-a4676ec4-11a9-4da9-a431-1119cfe5fe80.png#align=left&display=inline&height=586&originHeight=586&originWidth=640&size=0&status=done&style=shadow&width=640)
<a name="Ulwmz"></a>
### 3、SLF4J 比 Log4J 的优势
SLF4J 除了解决掉以上的痛点，帮助应用程序独立于任何特定的日志系统，还有一个非常厉害的功能，那就是 SLF4J 在打印日志的时候使用了占位符 {}，它有点类似于 String 类的 format() 方法（使用 %s 等填充参数），但更加便捷，这在很大程度上提高了程序的性能。<br />众所周知，字符串是不可变的，字符串拼接会创建很多不必要的字符串对象，极大的消耗了内存空间。但 Log4J 在打印带参数的日志时，只能使用字符串拼接的方式：
```java
String name = "Fcant";
int age = 18;
logger.debug(name + "，年纪：" + age + "，Hello World");
```
非常笨重，但加入了 SLF4J 后，这个问题迎刃而解。来看一下在 Log4j 项目中加入 SLF4J 的详细的步骤。<br />第一步，把 log4j 的依赖替换为 slf4j-log4j12（Maven 会自动引入 slf4j-api.jar 和 log4j.jar）：
```xml
<dependency>
    <groupId>org.slf4j</groupId>
    <artifactId>slf4j-log4j12</artifactId>
    <version>1.7.25</version>
</dependency>
```
第二步，在 resources 目录下创建 log4j.properties 文件：
```
### 设置###
log4j.rootLogger = debug,stdout,D,E
### 输出信息到控制台 ###
log4j.appender.stdout = org.apache.log4j.ConsoleAppender
log4j.appender.stdout.Target = System.out
log4j.appender.stdout.layout = org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern = [%-5p] %d{yyyy-MM-dd HH:mm:ss,SSS} method:%l%n%m%n
### 输出DEBUG 级别以上的日志到=debug.log ###
log4j.appender.D = org.apache.log4j.DailyRollingFileAppender
log4j.appender.D.File = debug.log
log4j.appender.D.Append = true
log4j.appender.D.Threshold = DEBUG 
log4j.appender.D.layout = org.apache.log4j.PatternLayout
log4j.appender.D.layout.ConversionPattern = %d{yyyy-MM-dd HH:mm:ss}  [ %t:%r ] - [ %p ]  %m%n
### 输出ERROR 级别以上的日志到=error.log ###
log4j.appender.E = org.apache.log4j.DailyRollingFileAppender
log4j.appender.E.File =error.log 
log4j.appender.E.Append = true
log4j.appender.E.Threshold = ERROR 
log4j.appender.E.layout = org.apache.log4j.PatternLayout
log4j.appender.E.layout.ConversionPattern = %d{yyyy-MM-dd HH:mm:ss}  [ %t:%r ] - [ %p ]  %m%n
```
第三步，新建测试类：
```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
/**
 * @author Fcant
 */
public class Log4jSLF4JDemo {
    private static final Logger logger = LoggerFactory.getLogger(Log4jSLF4JDemo.class);
    public static void main(String[] args) {
        logger.debug("{}，Hello","Fcant");
    }
}
```
使用占位符要比“+”操作符方便的多。并且此时不再需要 isDebugEnabled() 先进行判断，debug() 方法会在字符串拼接之前执行。<br />如果只是 Log4J 的话，会先进行字符串拼接，再执行 debug() 方法，来看示例代码：
```java
String name = "Fcant";
int age = 18;
logger.debug(name + "，年纪：" + age + "，Hi");
```
在调试这段代码的时候，可以发现如果日志系统的级别不是 DEBUG，就会多执行了字符串拼接的操作，白白浪费了性能。
:::tips
注意：阿里巴巴开发手册上还有一条「强制」级别的规约：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604467299721-7d40bc34-dc72-4e72-aa79-9b7b9e24f1c2.png#align=left&display=inline&height=344&originHeight=344&originWidth=1080&size=0&status=done&style=shadow&width=1080)
:::
这是因为如果参数是基本数据类型的话，会先进行自动装箱（Integer.valueOf()）。测试代码如下所示：
```java
logger.debug("Fcant，{}岁", 18);
```
通过反编译工具就可以看得到：
```java
logger.debug("\u6C89\u9ED8\u738B\u4E8C\uFF0C{}\u5C81", Integer.valueOf(18));
```
如果参数需要调用其他方法的话，debug() 方法会随后调用。<br />也就是说，如果不 isDebugEnabled() 的话，在不是 DEBUG 级别的情况下，会多执行自动装箱和调用其他方法的操作——程序的性能就下降了！
<a name="3UEnG"></a>
### 4、总结
1）在使用日志系统的时候，一定要使用 SLF4J 作为门面担当。<br />2）SLF4J 可以统一日志系统，作为上层的抽象接口，不需要关注底层的日志实现，可以是 Log4j，也可以是 Logback，或者 JUL、JCL。<br />3）SLF4J 在打印日志的时候可以使用占位符，既提高了程序性能（临时字符串少了，垃圾回收的工作量就小），又让代码变得美观统一。
