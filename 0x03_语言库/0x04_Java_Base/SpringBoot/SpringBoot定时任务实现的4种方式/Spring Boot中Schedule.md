SpringBoot Schedule
<a name="0EPCy"></a>
## 1、依赖在 pom 包中的配置
`pom`包里面只需要引入`Spring Boot Starter`包即可！
```xml
<dependencies>
    <!--spring boot核心-->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
    </dependency>
    <!--spring boot 测试-->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-test</artifactId>
        <scope>test</scope>
    </dependency>
</dependencies>
```
<a name="E4zc9"></a>
## 2、启动类配置启用定时调度
在启动类上面加上`@EnableScheduling`即可开启定时
```java
@SpringBootApplication
@EnableScheduling
public class ScheduleApplication {
    public static void main(String[] args) {
        SpringApplication.run(ScheduleApplication.class, args);
    }
}
```
<a name="nGKC6"></a>
## 3、创建定时任务—`@Scheduled`参数详解
`Spring Scheduler`支持四种形式的任务调度！

- **fixedRate**：固定速率执行，例如每5秒执行一次
- **fixedDelay**：固定延迟执行，例如距离上一次调用成功后2秒执行
- **initialDelay**：初始延迟任务，例如任务开启过5秒后再执行，之后以固定频率或者间隔执行
- **cron**：使用 Cron 表达式执行定时任务
<a name="cYe3i"></a>
### 3.1 zone
时区，接收一个`java.util.TimeZone#ID`。`cron表达式`会基于该时区解析。默认是一个空字符串，即取服务器所在地的时区。比如一般使用的时区`Asia/Shanghai`。该字段一般留空。
<a name="sNOV8"></a>
### 3.2 `fixedRate`—固定速率执行
可以通过使用`fixedRate`参数以固定时间间隔来执行任务，示例如下：
```java
@Component
public class SchedulerTask {
    private static final Logger log = LoggerFactory.getLogger(SchedulerTask.class);
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    /**
     * fixedRate：固定速率执行。每5秒执行一次。
     */
    @Scheduled(fixedRate = 5000)
    public void runWithFixedRate() {
        log.info("Fixed Rate Task，Current Thread : {}，The time is now : {}", Thread.currentThread().getName(), dateFormat.format(new Date()));
    }
}
```
运行`ScheduleApplication`主程序，即可看到控制台输出效果：
```
Fixed Rate Task，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 11:46:00
Fixed Rate Task，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 11:46:10
...
```
<a name="daBu7"></a>
#### fixedRateString
与 `fixedRate` 意思相同，只是使用字符串的形式。唯一不同的是支持占位符。
<a name="4gggT"></a>
### 3.3 `fixedDelay`—固定延迟执行
可以通过使用`fixedDelay`参数来设置上一次任务调用完成与下一次任务调用开始之间的延迟时间，示例如下：
```java
@Component
public class SchedulerTask {
    private static final Logger log = LoggerFactory.getLogger(SchedulerTask.class);
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    /**
     * fixedDelay：固定延迟执行。距离上一次调用成功后2秒后再执行。
     */
    @Scheduled(fixedDelay = 2000)
    public void runWithFixedDelay() {
        log.info("Fixed Delay Task，Current Thread : {}，The time is now : {}", Thread.currentThread().getName(), dateFormat.format(new Date()));
    }
}
```
控制台输出效果：
```
Fixed Delay Task，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 11:46:00
Fixed Delay Task，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 11:46:02
...
```
<a name="icOQ6"></a>
#### fixedDelayString
与 `fixedDelay` 意思相同，只是使用字符串的形式。唯一不同的是支持占位符。如：
```java
@Scheduled(fixedDelayString = "5000") //上一次执行完毕时间点之后5秒再执行
```
占位符的使用(配置文件中有配置：`time.fixedDelay=5000`)：
```java
@Scheduled(fixedDelayString = "${time.fixedDelay}")
void testFixedDelayString() {
    System.out.println("Execute at " + System.currentTimeMillis());
}
```
<a name="aHn6A"></a>
### 3.4 初始延迟任务
可以通过使用`initialDelay`参数与`fixedRate`或者`fixedDelay`搭配使用来实现初始延迟任务调度。
```java
@Component
public class SchedulerTask {
    private static final Logger log = LoggerFactory.getLogger(SchedulerTask.class);
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    /**
     * initialDelay:初始延迟。任务的第一次执行将延迟5秒，然后将以5秒的固定间隔执行。
     */
    @Scheduled(initialDelay = 5000, fixedRate = 5000)
    public void reportCurrentTimeWithInitialDelay() {
        log.info("Fixed Rate Task with Initial Delay，Current Thread : {}，The time is now : {}", Thread.currentThread().getName(), dateFormat.format(new Date()));
    }
}
```
控制台输出效果：
```
Fixed Rate Task with Initial Delay，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 11:46:05
Fixed Rate Task with Initial Delay，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 11:46:10
...
```
<a name="initialDelayString"></a>
#### initialDelayString
与 `initialDelay` 意思相同，只是使用字符串的形式。唯一不同的是支持占位符。
<a name="jkpyF"></a>
### 3.5 使用 Cron 表达式
`Spring Scheduler`同样支持`Cron`表达式，如果以上简单参数都不能满足现有的需求，可以使用 cron 表达式来定时执行任务。<br />该参数接收一个`cron表达式`，`cron表达式`是一个字符串，字符串以5或6个空格隔开，分开共6或7个域，每一个域代表一个含义。
<a name="iUnd3"></a>
#### cron表达式语法
```
[秒] [分] [小时] [日] [月] [周] [年]
```
> 注：[年]不是必须的域，可以省略[年]，则一共6个域

| 序号 | 说明 | 必填 | 允许填写的值 | 允许的通配符 |
| --- | --- | --- | --- | --- |
| 1 | 秒 | 是 | 0-59 | , - * / |
| 2 | 分 | 是 | 0-59 | , - * / |
| 3 | 时 | 是 | 0-23 | , - * / |
| 4 | 日 | 是 | 1-31 | , - * ? / L W |
| 5 | 月 | 是 | 1-12 / JAN-DEC | , - * / |
| 6 | 周 | 是 | 1-7 or SUN-SAT | , - * ? / L # |
| 7 | 年 | 否 | 1970-2099 | , - * / |

<a name="0Qmlg"></a>
###### 通配符说明

- `*` 表示所有值。例如：在分的字段上设置 *，表示每一分钟都会触发。
- `?` 表示不指定值。使用的场景为不需要关心当前设置这个字段的值。例如：要在每月的10号触发一个操作，但不关心是周几，所以需要周位置的那个字段设置为“?” 具体设置为 0 0 0 10 * ?
- `-` 表示区间。例如 在小时上设置 “10-12”，表示 10,11,12点都会触发。
- `,` 表示指定多个值，例如在周字段上设置 “MON,WED,FRI” 表示周一，周三和周五触发
- `/` 用于递增触发。如在秒上面设置“5/15” 表示从5秒开始，每增15秒触发(5,20,35,50)。在月字段上设置‘1/3’所示每月1号开始，每隔三天触发一次。
- `L` 表示最后的意思。在日字段设置上，表示当月的最后一天(依据当前月份，如果是二月还会依据是否是润年[leap])，在周字段上表示星期六，相当于“7”或”SAT”。如果在“L”前加上数字，则表示该数据的最后一个。例如在周字段上设置“6L”这样的格式，则表示“本月最后一个星期五”
- `W` 表示离指定日期的最近那个工作日(周一至周五)。例如在日字段上置“15W”，表示离每月15号最近的那个工作日触发。如果15号正好是周六，则找最近的周五(14号)触发，如果15号是周未，则找最近的下周一(16号)触发.如果15号正好在工作日(周一至周五)，则就在该天触发。如果指定格式为 “1W”，它则表示每月1号往后最近的工作日触发。如果1号正是周六，则将在3号下周一触发。(注“W”前只能设置具体的数字，不允许区间“-”)。
- `#` 序号(表示每月的第几个周几)，例如在周字段上设置“6#3”表示在每月的第三个周六。注意如果指定“#5”，正好第五周没有周六，则不会触发该配置(用在母亲节和父亲节再合适不过了) ；小提示：‘L’和 ‘W’可以一组合使用。如果在日字段上设置“LW”，则表示在本月的最后一个工作日触发；周字段的设置，若使用英文字母是不区分大小写的，即MON与mon相同。
<a name="q3pHv"></a>
###### 示例

- 每隔5秒执行一次：`*/5 * * * * ?`
- 每隔1分钟执行一次：`0 */1 * * * ?`
- 每天23点执行一次：`0 0 23 * * ?`
- 每天凌晨1点执行一次：`0 0 1 * * ?`
- 每月1号凌晨1点执行一次：`0 0 1 1 * ?`
- 每月最后一天23点执行一次：`0 0 23 L * ?`
- 每周星期天凌晨1点实行一次：`0 0 1 ? * L`
- 在26分、29分、33分执行一次：`0 26,29,33 * * * ?`
- 每天的0点、13点、18点、21点都执行一次：`0 0 0,13,18,21 * * ?`
<a name="1uknY"></a>
##### cron表达式使用占位符
另外，`cron`属性接收的cron表达式支持占位符。eg：<br />配置文件：
```yaml
time:
  cron: */5 * * * * *
  interval: 5
```
每5秒执行一次：
```java
@Scheduled(cron="${time.cron}")
void testPlaceholder1() {
    System.out.println("Execute at " + System.currentTimeMillis());
}
@Scheduled(cron="*/${time.interval} * * * * *")
void testPlaceholder2() {
    System.out.println("Execute at " + System.currentTimeMillis());
}
```
更多关于`cron`表达式的具体用法，可以点击参考这里：[https://cron.qqe2.com/](https://cron.qqe2.com/)
```java
@Component
public class SchedulerTask {
    private static final Logger log = LoggerFactory.getLogger(SchedulerTask.class);
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    /**
     * cron：使用Cron表达式。每6秒中执行一次
     */
    @Scheduled(cron = "*/6 * * * * ?")
    public void reportCurrentTimeWithCronExpression() {
        log.info("Cron Expression，Current Thread : {}，The time is now : {}", Thread.currentThread().getName(), dateFormat.format(new Date()));
    }
}
```
控制台输出效果：
```
Cron Expression，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 11:46:06
Cron Expression，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 11:46:12
...
```
<a name="vES4L"></a>
### 3.5 异步执行定时任务
在下面的示例中，创建了一个每隔2秒执行一次的定时任务，在任务里面大概需要花费 3 秒钟，猜猜执行结果如何？
```java
@Component
public class AsyncScheduledTask {
    private static final Logger log = LoggerFactory.getLogger(AsyncScheduledTask.class);
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    @Scheduled(fixedRate = 2000)
    public void runWithFixedDelay() {
        try {
            TimeUnit.SECONDS.sleep(3);
            log.info("Fixed Delay Task, Current Thread : {} : The time is now {}", Thread.currentThread().getName(), dateFormat.format(new Date()));
        } catch (InterruptedException e) {
            log.error("错误信息",e);
        }
    }
}
```
控制台输入结果：
```
Fixed Delay Task, Current Thread : scheduling-1 : The time is now 2020-12-15 17:55:26
Fixed Delay Task, Current Thread : scheduling-1 : The time is now 2020-12-15 17:55:31
Fixed Delay Task, Current Thread : scheduling-1 : The time is now 2020-12-15 17:55:36
Fixed Delay Task, Current Thread : scheduling-1 : The time is now 2020-12-15 17:55:41
...
```
很清晰的看到，任务调度频率变成了每隔5秒调度一次！<br />这是为啥呢？<br />从`Current Thread : scheduling-1`输出结果可以很看到，任务执行都是同一个线程！默认的情况下，`@Scheduled`任务都在 Spring 创建的大小为 1 的默认线程池中执行！<br />更直观的结果是，任务都是串行执行！<br />下面，将其改成异步线程来执行，看看效果如何？
```java
@Component
@EnableAsync
public class AsyncScheduledTask {
    private static final Logger log = LoggerFactory.getLogger(AsyncScheduledTask.class);
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    @Async
    @Scheduled(fixedDelay = 2000)
    public void runWithFixedDelay() {
        try {
            TimeUnit.SECONDS.sleep(3);
            log.info("Fixed Delay Task, Current Thread : {} : The time is now {}", Thread.currentThread().getName(), dateFormat.format(new Date()));
        } catch (InterruptedException e) {
            log.error("错误信息",e);
        }
    }
}
```
控制台输出结果：
```
Fixed Delay Task, Current Thread : SimpleAsyncTaskExecutor-1 : The time is now 2020-12-15 18:55:26
Fixed Delay Task, Current Thread : SimpleAsyncTaskExecutor-2 : The time is now 2020-12-15 18:55:28
Fixed Delay Task, Current Thread : SimpleAsyncTaskExecutor-3 : The time is now 2020-12-15 18:55:30
...
```
任务的执行频率不受方法内的时间影响，以并行方式执行！
<a name="aZD9B"></a>
### 3.6 自定义任务线程池
虽然默认的情况下，`@Scheduled`任务都在 Spring 创建的大小为 1 的默认线程池中执行，但是也可以自定义线程池，只需要实现`SchedulingConfigurer`类即可！<br />自定义线程池示例如下：
```java
@Configuration
public class SchedulerConfig implements SchedulingConfigurer {
    @Override
    public void configureTasks(ScheduledTaskRegistrar scheduledTaskRegistrar) {
        ThreadPoolTaskScheduler threadPoolTaskScheduler = new ThreadPoolTaskScheduler();
        //线程池大小为10
        threadPoolTaskScheduler.setPoolSize(10);
        //设置线程名称前缀
        threadPoolTaskScheduler.setThreadNamePrefix("scheduled-thread-");
        //设置线程池关闭的时候等待所有任务都完成再继续销毁其他的Bean
        threadPoolTaskScheduler.setWaitForTasksToCompleteOnShutdown(true);
        //设置线程池中任务的等待时间，如果超过这个时候还没有销毁就强制销毁，以确保应用最后能够被关闭，而不是阻塞住
        threadPoolTaskScheduler.setAwaitTerminationSeconds(60);
        //这里采用了CallerRunsPolicy策略，当线程池没有处理能力的时候，该策略会直接在 execute 方法的调用线程中运行被拒绝的任务；如果执行程序已关闭，则会丢弃该任务
        threadPoolTaskScheduler.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        threadPoolTaskScheduler.initialize();
        scheduledTaskRegistrar.setTaskScheduler(threadPoolTaskScheduler);
    }
}
```
启动服务，看看`cron`任务示例调度效果：
```
Cron Expression，Current Thread : scheduled-thread-1，The time is now : 2020-12-15 20:46:00
Cron Expression，Current Thread : scheduled-thread-2，The time is now : 2020-12-15 20:46:06
Cron Expression，Current Thread : scheduled-thread-3，The time is now : 2020-12-15 20:46:12
Cron Expression，Current Thread : scheduled-thread-4，The time is now : 2020-12-15 20:46:18
....
```
当前线程名称已经被改成自定义`scheduled-thread`的前缀！
