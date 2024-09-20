SpringBoot CommandLineRunner
<a name="Fll92"></a>
## 1、`CommandLineRunner`
<a name="maRp5"></a>
### SpringBoot中`CommandLineRunner`的作用
平常开发中有可能需要实现在项目启动后执行的功能，SpringBoot提供的一种简单的实现方案就是添加一个model并实现`CommandLineRunner`接口，实现功能的代码放在实现的`run`方法中。也就是项目一启动之后，就立即需要执行的动作。只需要在项目里面简单的配置，就可以实现这个功能。
<a name="5Ogba"></a>
#### 简单例子
```java
import org.springframework.boot.CommandLineRunner;
import org.springframework.stereotype.Component;
@Component
public class MyStartupRunner implements CommandLineRunner {
    @Override
    public void run(String... args) throws Exception {
        System.out.println("项目已经启动");
    }
}
```
<a name="49ea3a1a"></a>
### 多个类实现`CommandLineRunner`接口执行顺序的保证
<a name="98309b80"></a>
#### 通过实现`Ordered`接口实现控制执行顺序
```java
import lombok.extern.slf4j.Slf4j;
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.Ordered;
import org.springframework.stereotype.Component;

/**
 * 优先级最高
 * 该类期望在springboot 启动后第一顺位执行
 * @since 12:57
 **/
@Slf4j
@Component
public class HighOrderCommandLineRunner implements CommandLineRunner, Ordered {
    @Override
    public void run(String... args) throws Exception {
        for (String arg : args) {
            log.info("arg = " + arg);
        }
        log.info("i am highOrderRunner");
    }

    @Override
    public int getOrder() {
        return Integer.MIN_VALUE+1;
    }
}
```
```java
import lombok.extern.slf4j.Slf4j;
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.Ordered;
import org.springframework.stereotype.Component;

/**
 * 优先级低于{@code HighOrderCommandLineRunner}
 * @since 12:59
 **/
@Slf4j
@Component
public class LowOrderCommandLineRunner implements CommandLineRunner, Ordered {

    @Override
    public void run(String... args) throws Exception {
        log.info("i am lowOrderRunner");
    }

    @Override
    public int getOrder() {
        return Integer.MIN_VALUE+1;
    }
}
```
启动Spring Boot程序后，控制台按照预定的顺序打印出了结果：
```
2020-05-30 23:11:03.685  INFO 11976 --- [           main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat started on port(s): 8080 (http) with context path ''
2020-05-30 23:11:03.701  INFO 11976 --- [           main] c.f.Application  : Started SpringBootApplication in 4.272 seconds (JVM running for 6.316)
2020-05-30 23:11:03.706  INFO 11976 --- [           main] c.f.HighOrderCommandLineRunner   : i am highOrderRunner
2020-05-30 23:11:03.706  INFO 11976 --- [           main] c.f.LowOrderCommandLineRunner   : i am lowOrderRunner
```
<a name="1OjZI"></a>
#### 通过`@Order`注解实现控制执行顺序
SpringBoot在项目启动后会遍历所有实现`CommandLineRunner`的实体类并执行`run`方法，如果需要按照一定的顺序去执行，那么就需要在实体类上使用一个`@Order`注解（或者实现`Order`接口）来表明顺序
```java
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;
@Component
@Order(value=2)
public class MyStartupRunner1 implements CommandLineRunner {
    @Override
    public void run(String... args) throws Exception {
        System.out.println("执行2");
    }
}
```
```java
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;
@Component
@Order(value=1)
public class MyStartupRunner2 implements CommandLineRunner {
    @Override
    public void run(String... args) throws Exception {
        System.out.println("执行1");
    }
}
```
控制台显示
```
执行1
执行2
```
> 根据控制台结果可判断，`@Order` 注解的执行优先级是按value值从小到大顺序。

<a name="EryqK"></a>
#### `@Order` 作用
项目启动之后，要执行的动作是比较的多，那么到底先执行哪个，那么就可以利用这个注解限定优先级。
:::danger
`Ordered`接口并不能被 `@Order`注解所代替。
:::
<a name="pCwnJ"></a>
## 2、ApplicationRunner
在Spring Boot 1.3.0又引入了一个和`CommandLineRunner`功能一样的接口`ApplicationRunner`。`CommandLineRunner`接收可变参数`String... args`，而`ApplicationRunner` 接收一个封装好的对象参数`ApplicationArguments`。除此之外它们功能完全一样，甚至连方法名都一样。声明一个`ApplicationRunner`并让它优先级最低:
```java
import lombok.extern.slf4j.Slf4j;
import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.ApplicationRunner;
import org.springframework.core.Ordered;
import org.springframework.stereotype.Component;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
/**
 * 优先级最低
 **/
@Slf4j
@Component
public class DefaultApplicationRunner implements ApplicationRunner, Ordered {
    @Override
    public void run(ApplicationArguments args) throws Exception {
        log.info("i am applicationRunner");
        Set<String> optionNames = args.getOptionNames();
        log.info("optionNames = " + optionNames);
        String[] sourceArgs = args.getSourceArgs();
        log.info("sourceArgs = " + Arrays.toString(sourceArgs));
        List<String> nonOptionArgs = args.getNonOptionArgs();
        log.info("nonOptionArgs = " + nonOptionArgs);
        List<String> optionValues = args.getOptionValues("foo");
        log.info("optionValues = " + optionValues);
    }
    @Override
    public int getOrder() {
        return Integer.MIN_VALUE+2;
    }
}
```
按照顺序打印了三个类的执行结果：
```
2020-06-01 13:02:39.420  INFO 19032 --- [           main] c.f.MybatisResultmapApplication  : Started MybatisResultmapApplication in 1.801 seconds (JVM running for 2.266)
2020-06-01 13:02:39.423  INFO 19032 --- [           main] c.f.HighOrderCommandLineRunner   : i am highOrderRunner
2020-06-01 13:02:39.423  INFO 19032 --- [           main] c.f.LowOrderCommandLineRunner    : i am lowOrderRunner
2020-06-01 13:02:39.423  INFO 19032 --- [           main] c.f.DefaultApplicationRunner     : i am applicationRunner
2020-06-01 13:02:39.423  INFO 19032 --- [           main] c.f.DefaultApplicationRunner     : optionNames = []
2020-06-01 13:02:39.423  INFO 19032 --- [           main] c.f.DefaultApplicationRunner     : sourceArgs = []
2020-06-01 13:02:39.423  INFO 19032 --- [           main] c.f.DefaultApplicationRunner     : nonOptionArgs = []
2020-06-01 13:02:39.423  INFO 19032 --- [           main] c.f.DefaultApplicationRunner     : optionValues = null
```
> `Ordered`接口并不能被 `@Order`注解所代替。

<a name="SibQi"></a>
## 3、传递参数
Spring Boot应用启动时是可以接受参数的，换句话说也就是`Spring Boot`的`main`方法是可以接受参数的。这些参数通过命令行 `java -jar yourapp.jar` 来传递。`CommandLineRunner`会原封不动照单全收这些接口，这些参数也可以封装到`ApplicationArguments`对象中供`ApplicationRunner`调用。看一下`ApplicationArguments`的相关方法：

- `getSourceArgs()` 被传递给应用程序的原始参数，返回这些参数的字符串数组。
- `getOptionNames()` 获取选项名称的`Set`字符串集合。如 `--spring.profiles.active=dev --debug` 将返回`["spring.profiles.active","debug"]` 。
- `getOptionValues(String name)` 通过名称来获取该名称对应的选项值。如`--foo=bar --foo=baz` 将返回`["bar","baz"]`。
- `containsOption(String name)` 用来判断是否包含某个选项的名称。
- `getNonOptionArgs()` 用来获取所有的无选项参数。

可以通过下面的命令运行一个 Spring Boot应用 Jar
```bash
java -jar yourapp.jar --foo=bar --foo=baz --dev.name=fcant java fcantcn
```
或者在IDEA开发工具中打开Spring Boot应用`main`方法的配置项，进行命令行参数的配置，其他IDE工具同理。<br />运行Spring Boot应用，将会打印出：
```
2020-06-01 15:04:31.490  INFO 13208 --- [           main] c.f.HighOrderCommandLineRunner   : arg = --foo=bar
2020-06-01 15:04:31.490  INFO 13208 --- [           main] c.f.HighOrderCommandLineRunner   : arg = --foo=baz
2020-06-01 15:04:31.490  INFO 13208 --- [           main] c.f.HighOrderCommandLineRunner   : arg = --dev.name=fcant
2020-06-01 15:04:31.490  INFO 13208 --- [           main] c.f.HighOrderCommandLineRunner   : arg = java
2020-06-01 15:04:31.490  INFO 13208 --- [           main] c.f.HighOrderCommandLineRunner   : arg = fcantcn
2020-06-01 15:04:31.491  INFO 13208 --- [           main] c.f.HighOrderCommandLineRunner   : i am highOrderRunner
2020-06-01 15:04:31.491  INFO 13208 --- [           main] c.f.LowOrderCommandLineRunner    : i am lowOrderRunner
2020-06-01 15:04:31.491  INFO 13208 --- [           main] c.f.DefaultApplicationRunner     : i am applicationRunner
2020-06-01 15:04:31.491  INFO 13208 --- [           main] c.f.DefaultApplicationRunner     : optionNames = [dev.name, foo]
2020-06-01 15:04:31.491  INFO 13208 --- [           main] c.f.DefaultApplicationRunner     : sourceArgs = [--foo=bar, --foo=baz, --dev.name=fcant, java, fcantcn]
2020-06-01 15:04:31.491  INFO 13208 --- [           main] c.f.DefaultApplicationRunner     : nonOptionArgs = [java, fcantcn]
2020-06-01 15:04:31.491  INFO 13208 --- [           main] c.f.DefaultApplicationRunner     : optionValues = [bar, baz]
```
然后就可以根据实际需要动态地执行一些逻辑。
<a name="x1Agd"></a>
## 4、源码跟踪
通过源码理解一下底层实现。
<a name="rKR2A"></a>
#### `run()`方法
跟进`run`方法后，一路F6直达以下方法
```java
public ConfigurableApplicationContext run(String... args) {
   StopWatch stopWatch = new StopWatch();
   //设置线程启动计时器
   stopWatch.start();
   ConfigurableApplicationContext context = null;
   Collection<SpringBootExceptionReporter> exceptionReporters = new ArrayList<>();
   //配置系统属性：默认缺失外部显示屏等允许启动
   configureHeadlessProperty();
   //获取并启动事件监听器，如果项目中没有其他监听器，则默认只有EventPublishingRunListener
   SpringApplicationRunListeners listeners = getRunListeners(args);
   //将事件广播给listeners
   listeners.starting();
   try {
       //对于实现ApplicationRunner接口，用户设置ApplicationArguments参数进行封装
      ApplicationArguments applicationArguments = new DefaultApplicationArguments(
            args);
      //配置运行环境：例如激活应用***.yml配置文件      
      ConfigurableEnvironment environment = prepareEnvironment(listeners,
            applicationArguments);
      configureIgnoreBeanInfo(environment);
      //加载配置的banner(gif,txt...)，即控制台图样
      Banner printedBanner = printBanner(environment);
      //创建上下文对象，并实例化
      context = createApplicationContext();
      exceptionReporters = getSpringFactoriesInstances(
            SpringBootExceptionReporter.class,
            new Class[] { ConfigurableApplicationContext.class }, context);
      //配置SPring容器      
      prepareContext(context, environment, listeners, applicationArguments,
            printedBanner);
      //刷新Spring上下文，创建bean过程中      
      refreshContext(context);
      //空方法，子类实现
      afterRefresh(context, applicationArguments);
      //停止计时器：计算线程启动共用时间
      stopWatch.stop();
      if (this.logStartupInfo) {
         new StartupInfoLogger(this.mainApplicationClass)
               .logStarted(getApplicationLog(), stopWatch);
      }
      //停止事件监听器
      listeners.started(context);
      //开始加载资源
      callRunners(context, applicationArguments);
   }
   catch (Throwable ex) {
      handleRunFailure(context, listeners, exceptionReporters, ex);
      throw new IllegalStateException(ex);
   }
   listeners.running(context);
   return context;
}
```
主要是熟悉SpringBoot的`CommandLineRunner`接口实现原理。因此上面SpringBoot启动过程方法不做过多介绍。直接进入`CallRunners()`方法内部。
<a name="nxoAc"></a>
#### `callRunners`方法
```java
private void callRunners(ApplicationContext context, ApplicationArguments args) {
    //将实现ApplicationRunner和CommandLineRunner接口的类，存储到集合中
   List<Object> runners = new ArrayList<>();
   runners.addAll(context.getBeansOfType(ApplicationRunner.class).values());
   runners.addAll(context.getBeansOfType(CommandLineRunner.class).values());
   //按照加载先后顺序排序
   AnnotationAwareOrderComparator.sort(runners);
   for (Object runner : new LinkedHashSet<>(runners)) {
      if (runner instanceof ApplicationRunner) {
         callRunner((ApplicationRunner) runner, args);
      }
      if (runner instanceof CommandLineRunner) {
         callRunner((CommandLineRunner) runner, args);
      }
   }
}
```
```java
private void callRunner(CommandLineRunner runner, ApplicationArguments args) {
   try {
       //调用各个实现类中的逻辑实现
      (runner).run(args.getSourceArgs());
   }
   catch (Exception ex) {
      throw new IllegalStateException("Failed to execute CommandLineRunner", ex);
   }
}
```
到此结束，再跟进`run()`方法，就可以看到资源加载逻辑。
