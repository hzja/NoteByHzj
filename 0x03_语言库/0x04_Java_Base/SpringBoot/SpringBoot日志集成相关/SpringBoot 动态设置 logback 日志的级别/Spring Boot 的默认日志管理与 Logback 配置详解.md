JavaSpringBootLogback<br />Spring Boot在所有内部日志中使用Commons Logging，但是对底层日志的实现是开放的。在Spring Boot生态中，为Java Util Logging 、Log4J2 和Logback 这些常见的日志框架都提供了自动化配置组件，每种Logger都可以通过配置在控制台或者文件中输出日志内容。默认情况下，当使用各种Starter的时候，会使用Logback来实现日志管理。
<a name="Dcej1"></a>
## 如何记日志
在介绍写日志的方式有很多，这里就不对各种方式做罗列了，只讲DD用得最多的方式！<br />首先，在代码层面，不纠结到底用默认的Logback还是Log4j，而是直接用：**Slf4j**。<br />为什么不用管具体用Logback还是Log4j就可以去写代码呢？这个就是使用Slf4j好处！那么什么是Slf4j？英文全称：Simple Logging Facade for Java，即：简单日志门面，它并不是一个具体的日志解决方案，实际工作的还是Logback或Log4j这样的日志框架。Slf4j是23种设计模式中门面模式的典型应用案例，通过Slf4j这样一个门面的抽象，在写代码的记录日志的时候，只依赖这个抽象的操作，而具体实现会在Slf4j门面调用的时候委托给具体的实现日志记录的框架。<br />比如：下面的就是一个简单的使用Slf4j记录日志例子：
```java
@Slf4j
@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);

        log.error("Hello World");
        log.warn("Hello World");
        log.info("Hello World");
        log.debug("Hello World");
        log.trace("Hello World");
    }

}
```
注意：这里通过在pom.xml中引入了Lombok，然后使用`@Slf4j`声明引入Slf4j的log日志记录对象，之后就可以轻松的用它来日志了。而这个日志具体是如何写到控制台或者文件的，则有Spring Boot项目中引入了什么具体的日志框架决定，默认情况下就是Logback。
<a name="bb5Je"></a>
## 日志元素
启动任意一个Spring Boot项目，都可以在控制台看到很多日志信息，比如下面这样的一条日志：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668417040399-c35406b0-5950-46c6-b0ff-f44766446dec.png#averageHue=%23342e2d&clientId=u8d0e846b-865a-4&from=paste&id=uc01aecd9&originHeight=79&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue9e34cf1-02bb-47bb-b17e-735eea7266b&title=)<br />日志的输出内容中一共有7种元素，具体如下：

1. 时间日期：精确到毫秒
2. 日志级别：ERROR, WARN, INFO, DEBUG or TRACE
3. 进程ID
4. 分隔符：--- 标识实际日志的开始
5. 线程名：方括号括起来（可能会截断控制台输出）
6. Logger名：通常使用源代码的类名
7. 日志内容
<a name="dcDHT"></a>
## 日志输出
在Spring Boot应用中，日志会默认会输出到控制台中，默认的输出日志级别包含：ERROR、WARN和INFO，可以帮上面写的Hello World例子跑起来，就可以验证这样的默认设定：
```
2021-12-28 17:37:25.578  INFO 65136 --- [           main] com.didispace.chapter81.Application      : Started Application in 2.695 seconds (JVM running for 3.957)
2021-12-28 17:37:25.579 ERROR 65136 --- [           main] com.didispace.chapter81.Application      : Hello World
2021-12-28 17:37:25.579  WARN 65136 --- [           main] com.didispace.chapter81.Application      : Hello World
2021-12-28 17:37:25.579  INFO 65136 --- [           main] com.didispace.chapter81.Application      : Hello World
```
<a name="hmspW"></a>
### 开启DEBUG日志
可以通过两种方式切换至DEBUG级别：<br />**第一种**：在运行命令后加入`--debug`标志，如：`$ java -jar myapp.jar --debug`<br />**第二种**：在配置文件application.properties中配置`debug=true`<br />这里开启的DEBUG日志，仅影响核心Logger，包含嵌入式容器、hibernate、spring等这些框架层面的会输出更多内容，但是自己应用的日志并不会输出为DEBUG级别，从下面的截图中就可以看到，自己编写的debug级别的Hello World并没有输出。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668417040307-5e6446b0-8e36-450e-8e95-f1ee4a984fcb.png#averageHue=%23313131&clientId=u8d0e846b-865a-4&from=paste&id=ue232064f&originHeight=126&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud6e7c7df-42c4-472a-b0dc-ab70dd62d2a&title=)
<a name="gssm4"></a>
## 日志配置
下面介绍一些常用的日志配置，以更好的管理好日志内容。
<a name="BvObq"></a>
### 多彩输出
如果终端支持ANSI，设置彩色输出会让日志更具可读性。通过在application.properties中设置`spring.output.ansi.enabled`参数来支持，该参数有三个选项：

- NEVER：禁用ANSI-colored输出
- DETECT：会检查终端是否支持ANSI，是的话就采用彩色输出（默认项）
- ALWAYS：总是使用ANSI-colored格式输出，若终端不支持的时候，会有很多干扰信息，不推荐使用

注意：Spring Boot 1.x的时候，默认值为NEVER，2.x之后默认为DETECT，所以看上面的截图，默认就已经带有颜色了。所以如果是Spring Boot 2.x版本用户的话，这个基本就不用去修改了。
<a name="NxOlI"></a>
### 文件输出
Spring Boot默认配置只会输出到控制台，并不会记录到文件中，但是通常生产环境使用时都需要以文件方式记录。<br />若要增加文件输出，需要在配置文件application.properties配置几个参数，比如这样：
```
logging.file.name=run.log
logging.file.path=./
```

- `logging.file.name`：设置文件名
- `logging.file.path`：设置文件路径

注意：这里跟1.x版本有区别，1.x的时候分别对应的参数为logging.file和logging.path。
<a name="Ul9Kn"></a>
### 文件滚动
一直把日志输出在一个文件里显然是不合适的，任何一个日志框架都会为此准备日志文件的滚动配置。由于本篇将默认配置，所以就是Logback的配置，具体有这几个：

- `logging.logback.rollingpolicy.file-name-pattern`：用于创建日志档案的文件名模式。
- `logging.logback.rollingpolicy.clean-history-on-start`：应用程序启动时是否对进行日志归档清理，默认为false，不清理
- `logging.logback.rollingpolicy.max-history`：要保留的最大归档日志文件数量，默认为7个
- `logging.logback.rollingpolicy.max-file-size`：归档前日志文件的最大尺寸，默认为10MB
- `logging.logback.rollingpolicy.total-size-cap`：日志档案在被删除前的最大容量，默认为0B
<a name="BTvXv"></a>
### 级别控制
如果要对各个Logger做一些简单的输出级别控制，那么只需要在application.properties中进行配置就能完成。<br />配置格式：`logging.level.*=LEVEL`

- `logging.level`：日志级别控制前缀，`*`为包名或Logger名
- LEVEL：选项TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF

举例：

- `logging.level.com.didispace=DEBUG`：com.didispace包下所有class以DEBUG级别输出
- `logging.level.root=WARN`：root日志以WARN级别输出

做了这样的配置之后，可以再执行下上面的程序，原本debug级别的Hello World就可以被成功输出了。<br />![logging.level.com.didispace=DEBUG](https://cdn.nlark.com/yuque/0/2022/png/396745/1668417040228-7805fc99-94cd-4f34-a728-b5ceb00083ac.png#averageHue=%23333232&clientId=u8d0e846b-865a-4&from=paste&id=u8837ff64&originHeight=104&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u0dd80ee3-41d1-4bcf-a95a-c6d07a2b9db&title=logging.level.com.didispace%3DDEBUG "logging.level.com.didispace=DEBUG")
<a name="FhBsk"></a>
### 自定义日志配置
由于日志服务一般都在ApplicationContext创建前就初始化了，它并不是必须通过Spring的配置文件控制。因此通过系统属性和传统的Spring Boot外部配置文件依然可以很好的支持日志控制和管理。<br />根据不同的日志系统，可以按如下规则组织配置文件名，就能被正确加载：

- Logback：logback-spring.xml, logback-spring.groovy, logback.xml, logback.groovy
- Log4j2：log4j2-spring.xml, log4j2.xml
- JDK (Java Util Logging)：logging.properties

**Spring Boot官方推荐优先使用带有-spring的文件名作为日志配置（如使用logback-spring.xml，而不是logback.xml）**
<a name="LXfNi"></a>
### 自定义输出格式
在Spring Boot中可以通过在application.properties配置如下参数控制输出格式：

- logging.pattern.console：定义输出到控制台的样式（不支持JDK Logger）
- logging.pattern.file：定义输出到文件的样式（不支持JDK Logger）
