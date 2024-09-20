Java 定时任务
<a name="ZxfvJ"></a>
## 一、`ScheduledThreadPool`
`ScheduledThreadPool`是JDK自带的类，可以用来替代`Timer`类实现定时任务。一个`Timer`只能执行一个任务，而一个`ScheduledThreadPool`却可以同时执行多个定时任务。用法很简单，直接看例子：
```java
public class ScheduledThreadPoolService {
    private Logger logger = LoggerFactory.getLogger(getClass().getSimpleName());

    // 参数代表可以同时执行的定时任务个数
    private ScheduledExecutorService service = Executors.newScheduledThreadPool(3);

    /**
     * schedule：延时2秒执行一次任务
     */
    public void task0() {
        service.schedule(() -> {
            logger.info("task0-start");
            sleep(2);
            logger.info("task0-end");
        }, 2, TimeUnit.SECONDS);
    }

    /**
     * scheduleAtFixedRate：2秒后，每间隔4秒执行一次任务
     * 注意，如果任务的执行时间（例如6秒）大于间隔时间，则会等待任务执行结束后直接开始下次任务
     */
    public void task1() {
        service.scheduleAtFixedRate(() -> {
            logger.info("task1-start");
            sleep(2);
            logger.info("task1-end");
        }, 2, 4, TimeUnit.SECONDS);
    }

    /**
     * scheduleWithFixedDelay：2秒后，每次延时4秒执行一次任务
     * 注意，这里是等待上次任务执行结束后，再延时固定时间后开始下次任务
     */
    public void task2() {
        service.scheduleWithFixedDelay(() -> {
            logger.info("task2-start");
            sleep(2);
            logger.info("task2-end");
        }, 2, 4, TimeUnit.SECONDS);
    }

    private void sleep(long time) {
        try {
            TimeUnit.SECONDS.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
和线程池的使用类似，也是通过`Executors`类来创建`ScheduledThreadPool`，常用的方法例子中都有说明，简单的定时任务使用`ScheduledThreadPool`实现就可以了，也不用引入其它依赖，还是挺方便的。
<a name="cmzbG"></a>
## 二、`@Scheduled`
`@Scheduled`是Spring框架的定时任务实现，相比JDK的`ScheduledThreadPool`功能更加强大。可以先创建一个Spring Boot项目，在启动类上添加`@EnableScheduling`注解，使`@Scheduled`生效，开启定时任务：
```java
@SpringBootApplication
@EnableScheduling
public class LearnSpringbootApplication {
    public static void main(String[] args) {
        SpringApplication.run(LearnSpringbootApplication.class, args);
    }
}
```
然后就是定义任务类，用`@Scheduled`配置具体的定时规则：
```java
@Service
public class ScheduleService {
    private Logger logger = LoggerFactory.getLogger(getClass().getSimpleName());

    /**
     * fixedRate：每间隔2秒执行一次任务
     * 注意，默认情况下定时任务是在同一线程同步执行的，如果任务的执行时间（例如6秒）大于间隔时间，则会等待任务执行结束后直接开始下次任务
     */
    @Scheduled(fixedRate = 2000)
    public void task0() {
        logger.info("task0-start");
        sleep(6);
        logger.info("task0-end");
    }

    /**
     * fixedDelay：每次延时2秒执行一次任务
     * 注意，这里是等待上次任务执行结束后，再延时固定时间后开始下次任务
     */
    @Scheduled(fixedDelay = 2000)
    public void task1() {
        logger.info("task1-start");
        sleep(6);
        logger.info("task1-end");
    }

    /**
     * initialDelay：首次任务启动的延时时间
     */
    @Scheduled(initialDelay = 2000, fixedDelay = 3000)
    public void task2() {
        logger.info("task2-start");
        sleep(6);
        logger.info("task2-end");
    }

    private void sleep(long time) {
        try {
            TimeUnit.SECONDS.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
`fixedRate`、`fixedDelay`、`initialDelay`属性的时间单位是毫秒，用法也比较简单，可参考代码的注释。<br />这里重点看使用`@Scheduled`的`cron`属性定义定时任务的时间表达式，它的功能更加丰富。先看一个简单的例子：
```java
@Service
public class ScheduleService {
    ...................
    /**
     * 每天晚上23:59执行一次
     */
    @Scheduled(cron = "0 59 23 * * ?")
    public void task3() {
        logger.info("task3-start");
        sleep(6);
        logger.info("task3-end");
    }
    ...................
}
```
接下来具体看cron属性对应时间表达式的定义规则：<br />1、按顺序依次是：秒、分、时、日、月、周，中间用空格间隔<br />2、月、周可以用数字或英文单词的前三个字母表示<br />3、日和周可能会冲突，因此两个可以有一个配置为`?`<br />4、常用通配符的含义：

- 表示任意值，例如在秒字段上设置，表示每秒都触发
- ?表示不指定值，只能出现在日或者周的位置，用于处理日和周可能存在的冲突，例如2020年8月15是周六，如果又在周的位置上指定为周一，那就会产生冲突到导致定时任务失效。如果不关心日或者周的时候，也可以将其设置为`?`
- -表示时间区间，例如在秒上设置1-3，表示第1、2、3秒都会触发
- /表示时间间隔，例如在秒上设置2/4，表示从第2秒开始每间隔4秒触发一次
- ,表示列举多个值，例如MON,WED,FRI表示周一、周三、周五触发

5、几个表达式的例子：

- `0 0 9 * * ?`：每天早上9点触发
- `0 0/30 * * * ?`：每30分钟触发一次
- `0 30 18 ？* MON-FRI`：每周一到周五的18:30分触发
- `0 10 12 1,15 * ?`：每月1号、15号的12:10触发
- `0 0/10 7-8 1,15 * ?`：每月1号、15号早上7点到8点每10分钟触发一次

6、注意，不同版本的Spring对`cron`属性的支持有所差异，例如较新的版本中cron属性只支持6个字段，不包含年份；同时`#`、`L`、`W`通配符也不支持。<br />7、注意，`@Scheduled`默认会使用同一线程同步的去执行所有定时任务，要打破这一点，可以在Spring Boot启动类上配置`@EnableAsync`注解来开启异步线程可用，然后在定时任务方法上配置`@Async`注解，则会使用异步线程去执行定时任务。
<a name="fZzUz"></a>
## 三、Quartz
除了上边的方式之外，还有更好的选择，那就是Quartz，在Spring Boot中集成Quartz需要先添加如下Maven依赖：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-quartz</artifactId>
    <version>2.3.2.RELEASE</version>
</dependency>
```
和`@Scheduled`一样，也需要在启动类添加`@EnableScheduling`注解来开启定时任务：
```java
@SpringBootApplication
@EnableScheduling
public class LearnSpringbootApplication {
    public static void main(String[] args) {
        SpringApplication.run(LearnSpringbootApplication.class, args);
    }
}
```
这样准备工作就完成了。接下来先了解几个重要的概念：

- `Job`：定义具体要执行的任务
- `JobDetail`：配置要执行任务的描述信息，即如何去定位要执行的Job，每次执行任务时，都会根据`JobDetail`创建一个Job对象，避免任务并发执行时访问同一个Job对象产生问题
- `Trigger`：触发器，配置任务执行的时间规则，需要和一个`JobDetail`关联起来
- `Scheduler`：调度器，它维护了一个`JobDetail`和`Trigger`的注册表，当任务关联的触发器到达预定的时间，调度器会去执行任务

接下来通过Quartz实现定时任务，可以按照上边几个重要概念的顺序来完成。
<a name="OAojm"></a>
### 1、定义Job
定义Job有两种方式，<br />第一种是直接定义任务类，并注册到Spring IoC容器中：
```java
@Service
public class QuartzJob {
    private Logger logger = LoggerFactory.getLogger(getClass().getSimpleName());

    public void hello(){
        logger.info("job0-start");
        Utils.sleep(TimeUnit.SECONDS, 4);
        logger.info("job0-end");
    }
}
```
第二种是继承`QuartzJobBean`，重写`executeInternal`方法，这种方式可以接受`JobDetail`传递的参数`：
```java
public class QuartzJob2 extends QuartzJobBean {
    private Logger logger = LoggerFactory.getLogger(getClass().getSimpleName());

    @Override
    protected void executeInternal(JobExecutionContext jobExecutionContext) throws JobExecutionException {
        logger.info("job1-start");
        Utils.sleep(TimeUnit.SECONDS, 4);
        // 获取参数
        JobDataMap jobDataMap = jobExecutionContext.getMergedJobDataMap();
        String date = jobDataMap.getString("date");
        logger.info("参数：" + date);
        logger.info("job1-end");
    }
}
```
这样就把`JobDetail`和之前定义的`QuartzJob`关联起来了。
<a name="KEgHu"></a>
### 2、配置`JobDetail`
`JobDetail`可以使用`MethodInvokingJobDetailFactoryBean`或者`JobDetailFactoryBean`配置，配置工作需要在一个Spring配置类中完成，可以定义一个`QuartzConfig`配置类，首先看`MethodInvokingJobDetailFactoryBean`的使用：
```java
@Configuration
public class QuartzConfig {
    @Bean
    public MethodInvokingJobDetailFactoryBean methodInvokingJobDetailFactoryBean() {
        MethodInvokingJobDetailFactoryBean bean = new MethodInvokingJobDetailFactoryBean();
        // 指定任务类在IoC容器中的Bean名称
        bean.setTargetBeanName("quartzJob");
        // 指定要执行的方法名称
        bean.setTargetMethod("hello");
        return bean;
    }
}
```
这样就把`JobDetail`和之前`QuartzJob`所定义的任务关联起来了，接下来看`JobDetailFactoryBean`：
```java
@Configuration
public class QuartzConfig {
    @Bean
    public JobDetailFactoryBean jobDetailFactoryBean() {
        JobDetailFactoryBean bean = new JobDetailFactoryBean();
        // 指定任务类名称
        bean.setJobClass(QuartzJob2.class);
        // 准备参数
        JobDataMap jobDataMap = new JobDataMap();
        jobDataMap.put("date", "2020-8-16");
        // 传递参数
        bean.setJobDataMap(jobDataMap);
        return bean;
    }
}
```
这样就把`JobDetail`和之前定义的`QuartzJob2`关联起来了，同时传递了参数。
<a name="jpJMO"></a>
### 3、配置`Trigger`
`Trigger`同样定义在`QuartzConfig`配置类里，常用的`Trigger`有`SimpleTrigger`、`CronTrigger`等，它们分别可以通过`SimpleTriggerFactoryBean`、`CronTriggerFactoryBean`来完成配置，先用`SimpleTriggerFactoryBean`配置的触发器关联`MethodInvokingJobDetailFactoryBean`配置的`JobDetail`：
```java
@Configuration
public class QuartzConfig {
    @Bean
    public SimpleTriggerFactoryBean simpleTriggerFactoryBean() {
        SimpleTriggerFactoryBean bean = new SimpleTriggerFactoryBean();
        bean.setRepeatCount(10);
        bean.setRepeatInterval(2000);
        // 关联JobDetail
bean.setJobDetail(methodInvokingJobDetailFactoryBean().getObject());
        return bean;
    }
}
```
`SimpleTriggerFactoryBean`的用法比较简单<br />再用`CronTriggerFactoryBean`配置的触发器关联`JobDetailFactoryBean`配置的`JobDetail`：
```java
@Configuration
public class QuartzConfig {
    @Bean
    public CronTriggerFactoryBean cronTriggerFactoryBean() {
        CronTriggerFactoryBean bean = new CronTriggerFactoryBean();
        bean.setCronExpression("0/2 * * * * ? 2020");
        // 关联JobDetail
        bean.setJobDetail(jobDetailFactoryBean().getObject());
        return bean;
    }
}
```
`CronTriggerFactoryBean`可以实现类似Spring中`@Scheduled`的`cron`表达式的功能，同时支持了年份的配置。
<a name="WY03n"></a>
### 4、配置`Scheduler`
最后一步就是通过`SchedulerFactoryBean`来配置`Scheduler`，来注册`Trigger`
```java
@Configuration
public class QuartzConfig {
    @Bean
    public SchedulerFactoryBean schedulerFactoryBean() {
        SchedulerFactoryBean bean = new SchedulerFactoryBean();
        // 注册两个Trigger
        bean.setTriggers(simpleTriggerFactoryBean().getObject(), cronTriggerFactoryBean().getObject());
        return bean;
    }
}
```
Quartz会使用异步线程去执行定时任务，不会出现像`@Scheduled`中定时任务在同一线程同步执行的情况。<br />到这里Spring Boot集成Quartz整个流程就完成了，运行项目就可以看到定时任务按照预期的效果执行。
