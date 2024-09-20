JavaSpring<br />Spring scheduled 默认为单线程，最近就发现生产上有某个服务实例不按时执行 task 任务，最后排查出使用了`@Scheduled`，发现一个任务执行12个小时，导致其他任务都在排队等待，没有在规定时间去抢占分布式锁。
<a name="XEEVs"></a>
### `@Scheduled`使用
在 Spring Boot 中要想使用 `@Scheduled` 注解，先要在启动类上加上注解 `@EnableScheduling`
```java
@Configuration
@EnableScheduling 
public class ScheduleConfig {
}

@Component
public class ScheduleTask {

    private static final Logger LOGGER = LoggerFactory.getLogger(ScheduleTask.class);

    // 每秒触发一次
    @Scheduled(cron = "* * * * * ?")
    public void printLog() {
        LOGGER.warn("执行定时任务");
    }
}
```
事实上在 Spring Boot 管理的类上都是可以生效的，比如 `@Component` `@Configuration`注解都是可以生效的，只需要配置一次即可，建议配置到启动类或者单独的配置类，方便阅读和定位问题。
> **注意：**任务类需要添加注解如 `@Component`，将 Bean 交给 Spring 管理才可以执行定时任务 如果类被 `@Lazy` 修饰导致 Spring Boot 在启动的时候没有实例化，那定时任务就不会开始执行。

```java
@Lazy
@Component
public class ScheduleTask {

    private static final Logger LOGGER = LoggerFactory.getLogger(ScheduleTask.class);

    // 此时定时任务不会被执行
    @Scheduled(cron = "* * * * * ?")
    public void printLog() {
        LOGGER.warn("执行定时任务");
    }
}
```
<a name="HqUA3"></a>
### `@Scheduled` 注解参数

- `fixedRate`：隔指定时间调用一次，不管任务是否执行完；
- `fixedDelay`：任务执行完后隔指定时间再调用；
- `cron`：按表达式执行，可以通过工具网站来快速生成[https://cron.qqe2.com/](https://cron.qqe2.com/)

以上三个属性只能选择一个，详见`org.springframework.scheduling.annotation.Scheduled`类中的注释内容
```java
Annotation that marks a method to be scheduled.  Exactly one of the @link #cron}, {@link #fixedDelay},
or {@link #fixedRate} attributes must be pecified.
```
所以在 cron 表达式的定时任务未完成又到了执行时间，同样是不会继续执行的。具体可以参考<br />如果仍然需要执行可以添加`@Async` 注解，将方法设置为异步。<br />而 cron 表达式一般情况下也不会写死，通常都是将 cron 作为配置放到 yml 配置文件中，方便修改维护
```java
@Scheduled(cron = "${cron:* * * * * ?}")
public void printLog() {
    LOGGER.warn("执行定时任务");
}
```
```yaml
#yml文件中的配置内容，如果没有配置则会执行默认值 * * * * * ?
cron: 0/5 * * * * ?
```
动态修改配置文件，则可以使用 Apollo 实现
<a name="tJxIe"></a>
### 多线程定时任务的实现
`@Scheduled` 默认为单线程，导致任务阻塞，很多时候不能按时执行原因源码`ScheduledTaskRegistrar`（定时任务注册类）类中`ScheduleTasks`方法
```java
if (this.taskScheduler == null) {
    this.localExecutor = Executors.newSingleThreadScheduledExecutor();
    this.taskScheduler = new ConcurrentTaskScheduler(this.localExecutor);
}
```
多线程任务在 Spring Boot 2.0 及其之前的版本都需要实现 `SchedulingConfigurer` 接口，2.1 版本后提供了自动配置类 `TaskSchedulingAutoConfiguration`，实际操作可以根据 `TaskSchedulingProperties`的配置项添加所需配置到 yml 文件。<br />Spring Boot 2.0 以前的实现方式（多线程下，两个任务不用互相等待了，但是同一个任务还是需要等待的）
```java
@Configuration
@EnableScheduling
public class ScheduleConfig implements SchedulingConfigurer {

    @Bean("scheduledThreadPoolExecutor")
    public Executor scheduledThreadPoolExecutor() {
        ThreadPoolTaskScheduler scheduler = new ThreadPoolTaskScheduler();
        scheduler.setThreadNamePrefix("foxScheduling-");
        scheduler.setPoolSize(10);
        return scheduler;
    }

    @Override
    public void configureTasks(ScheduledTaskRegistrar scheduledTaskRegistrar) {
        scheduledTaskRegistrar.setScheduler(scheduledThreadPoolExecutor());
    }
}
```
Spring Boot 2.1以后版本只需要修改配置即可（多线程下，两个任务不用互相等待了，但是同一个任务还是需要等待的）
```yaml
spring:
  task:
 scheduling:
   pool:
  # 最大线程数，默认是 1
  size: 10
   # 线程名称前缀，默认是 scheduling-
   thread-name-prefix: foxScheduling-
```
还有一种建议使用的方式，同一任务也无需等待，自定义线程池，在定时任务上增加`@Async`注解，并在启动类上增加`@EnableAsync`注解
```java
@Configuration
public class ThreadPoolConfig {
    @Bean("mySyncExecutor")
    public Executor asyncServiceExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        //配置核心线程数
        executor.setCorePoolSize(5);
        //配置最大线程数
        executor.setMaxPoolSize(15);
        //配置队列大小
        executor.setQueueCapacity(200);
        //配置线程池中的线程的名称前缀
        executor.setThreadNamePrefix("async-executor-");

        // 设置拒绝策略：当pool已经达到max size的时候，如何处理新任务
        // CALLER_RUNS：不在新线程中执行任务，而是有调用者所在的线程来执行
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        //执行初始化
        executor.initialize();
        return executor;
    }

}

@Async("mySyncExecutor")
@Scheduled(cron = "${cron:* * * * * ?}")
public void printLog() {

}
```
<a name="QC5fe"></a>
### 通过配置开关定时任务
通过配置开关定时任务有几种思路：

- 定时任务内配置 这种方法基本也是在网上最容易查到的，通过注入配置文件中的 enable值来控制是否执行定时任务中的代码逻辑。
```java
@Value("${enable}")
private Boolean enable;

@Scheduled(cron = "${cron:* * * * * ?}")
public void printLog() {
    if (enable) {
        LOGGER.warn("执行定时任务");       
    }
}
```
当然这种方法很显然治标不治本，定时任务仍然在后台执行，只不过没有执行逻辑代码。

- 注解`@ConditionalOnProperty`这种方法也是最推荐使用的，不需要升级到 Spring Boot 2.1 不过同样需要在 yml 中增加 enable配置。

具体思路就是通过 `@ConditionalOnProperty`来控制类`ScheduleTask`是否生效，可以理解为如果 enable是 false 那么ScheduleTask 就不会交给 Spring 控制。
```java
@Component
// 此处增加了默认配置，如果不配置默认是开启定时任务的
@ConditionalOnProperty("${enable:true}")
public class ScheduleTask {

    private static final Logger LOGGER = LoggerFactory.getLogger(ScheduleTask.class);

    @Scheduled(cron = "${cron:* * * * * ?}")
    public void printLog() {
        LOGGER.warn("执行定时任务");
    }
}
```

- cron表达式2.1版本新增 在 Spring Boot 2.1 版本开始，cron 中增加了 `public static final String CRON_DISABLED = "-";`

当需要停止定时任务时可以通过修改配置文件中 cron 的值为 - 停止定时任务。<br />yml 文件中的配置内容
```yaml
cron: '-'
```
注意：yml 中 - 是特殊符号用来表示集合的元素，所以配置的时候需要加上单引号。
