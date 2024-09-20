Java SpringBoot<br />在编写Spring Boot应用中经常会遇到这样的场景，比如：需要定时地发送一些短信、邮件之类的操作，也可能会定时地检查和监控一些标志、参数等。
<a name="zWCLM"></a>
### 创建定时任务
在Spring Boot中编写定时任务是非常简单的事，下面通过实例介绍如何在Spring Boot中创建定时任务，实现每过5秒输出一下当前时间。

- 在Spring Boot的主类中加入`@EnableScheduling`注解，启用定时任务的配置
```java
@SpringBootApplication
@EnableScheduling
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }

}
```

- 创建定时任务实现类
```java
@Component
public class ScheduledTasks {

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");

    @Scheduled(fixedRate = 5000)
    public void reportCurrentTime() {
        log.info("现在时间：" + dateFormat.format(new Date()));
    }

}
```

- 运行程序，控制台中可以看到类似如下输出，定时任务开始正常运作了。
```java
2021-07-13 14:56:56.413  INFO 34836 --- [           main] c.d.chapter71.Chapter71Application       : Started Chapter71Application in 1.457 seconds (JVM running for 1.835)
2021-07-13 14:57:01.411  INFO 34836 --- [   scheduling-1] com.didispace.chapter71.ScheduledTasks   : 现在时间：14:57:01
2021-07-13 14:57:06.412  INFO 34836 --- [   scheduling-1] com.didispace.chapter71.ScheduledTasks   : 现在时间：14:57:06
2021-07-13 14:57:11.413  INFO 34836 --- [   scheduling-1] com.didispace.chapter71.ScheduledTasks   : 现在时间：14:57:11
2021-07-13 14:57:16.413  INFO 34836 --- [   scheduling-1] com.didispace.chapter71.ScheduledTasks   : 现在时间：14:57:16
```
<a name="OB5Cn"></a>
### `@Scheduled`详解
在上面的入门例子中，使用了`@Scheduled(fixedRate = 5000)`注解来定义每过5秒执行的任务。对于`@Scheduled`的使用，从源码里看看有哪些配置：
```java
@Target({ElementType.METHOD, ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Repeatable(Schedules.class)
public @interface Scheduled {

    String CRON_DISABLED = ScheduledTaskRegistrar.CRON_DISABLED;

    String cron() default "";

    String zone() default "";

    long fixedDelay() default -1;

    String fixedDelayString() default "";

    long fixedRate() default -1;

    String fixedRateString() default "";

    long initialDelay() default -1;

    String initialDelayString() default "";

}
```
这些具体配置信息的含义如下：

- `cron`：通过`cron`表达式来配置执行规则
- `zone`：cron表达式解析时使用的时区
- `fixedDelay`：上一次执行结束到下一次执行开始的间隔时间（单位：ms）
- `fixedDelayString`：上一次任务执行结束到下一次执行开始的间隔时间，使用`java.time.Duration#parse`解析
- `fixedRate`：以固定间隔执行任务，即上一次任务执行开始到下一次执行开始的间隔时间（单位：ms），若在调度任务执行时，上一次任务还未执行完毕，会加入worker队列，等待上一次执行完成后立即执行下一次任务
- `fixedRateString`：与`fixedRate`逻辑一致,只是使用`java.time.Duration#parse`解析
- `initialDelay`：首次任务执行的延迟时间
- `initialDelayString`：首次任务执行的延迟时间,使用`java.time.Duration#parse`解析
