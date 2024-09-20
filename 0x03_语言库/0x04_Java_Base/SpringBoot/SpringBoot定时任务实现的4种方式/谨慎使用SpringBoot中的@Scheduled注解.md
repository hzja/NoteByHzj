Java SpringBoot @Scheduled<br />使用`@Scheduled`注解做定时任务需求需要格外小心，避免踩入不必要的坑。<br />比如，有一个需求：一是每隔5s做一次业务处理，另一个则是每隔10s做相应的业务处理，在Springboot项目中，代码如下：
```java
@EnableScheduling
@Component
public class ScheduleTask {

    @Scheduled(cron = "0/5 * * * * ?")
    public void taskA() {
        System.out.println("执行了ScheduleTask类中的taskA方法");
    }

    @Scheduled(cron = "0/10 * * * * ?")
    public void taskB() {
        System.out.println("执行了ScheduleTask类中的taskB方法");
    }
}
```
`@Component`：是将`ScheduleTask`类注入到Spring容器中。<br />`@Scheduled`：表示这个方法是个定时任务<br />`@EnableScheduling`：开启定时任务<br />cron表达式：是一个字符串，字符串以5或6个空格隔开，分开共6或7个域，每一个域代表一个含义，分别为 `[秒] [分] [小时] [日] [月] [周] [年]`<br />如果对cron表达式不太了解，可以在 [https://cron.qqe2.com/](https://cron.qqe2.com/)网站按照自己的需求生成相应的cron表达式。
<a name="Mzy7T"></a>
## 产生的问题
<a name="bdOc1"></a>
### 1、定时器的任务默认是按照顺序执行的
创建定时器执行任务目的是为了让它多线程执行任务，但是后来才发现，`@Scheduled`注解的方法默认是按照顺序执行的，这会导致当一个任务挂死的情况下，其它任务都在等待，无法执行。<br />那么这是为什么呢？<br />首先说明一下`@Scheduled`注解加载的过程，以及它是如何执行的。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622907845622-00dad8ce-bf99-4e92-b538-22db93032ba1.png#clientId=ubad24891-1b54-4&from=paste&id=ud5ba6349&originHeight=1718&originWidth=672&originalType=url&ratio=3&status=done&style=shadow&taskId=u258b2bae-f997-4e6a-98de-61818f2414a)<br />解析@Scheduled注解

1.  `ScheduledAnnotationBeanPostProcessor`类处理器解析带有`@Scheduled`注解的方法

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622907845593-37cea0bf-9f28-486d-8a0e-3186fdf82c66.webp#clientId=ubad24891-1b54-4&from=paste&id=u2f4c51f1&originHeight=455&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=ud22e1772-9f7b-4677-934e-3d5ffcda61b)

2.  `processScheduled`方法处理`@Scheduled`注解后面的参数，并将其添加到任务列表中

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622907845788-54647e54-7611-4283-b810-bb63bae8326d.webp#clientId=ubad24891-1b54-4&from=paste&id=u5ff1386c&originHeight=435&originWidth=894&originalType=url&ratio=3&status=done&style=none&taskId=uad578dfa-5a55-434e-8177-649484a66eb)

3. 执行任务。`ScheduledTaskRegistrar`类为Spring容器的定时任务注册中心。Spring容器通过线程处理注册的定时任务

首先，调用`scheduleCronTask`初始化定时任务。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622907845633-71935615-47b2-49f0-84f6-bcaf9731f905.webp#clientId=ubad24891-1b54-4&from=paste&id=u35e6b9a0&originHeight=446&originWidth=1000&originalType=url&ratio=3&status=done&style=none&taskId=u368537ce-cee9-4c74-97f2-5c3ae0f4b5a)<br />然后，在`ThreadPoolTaskShcedule`类中，会对线程池进行初始化，线程池的核心线程数量为1，<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622907845609-0ddd7a6f-2e24-4edf-9433-dc3bded55139.webp#clientId=ubad24891-1b54-4&from=paste&id=u895169e0&originHeight=440&originWidth=1072&originalType=url&ratio=3&status=done&style=none&taskId=u826ddb98-04fb-4356-8249-73b0052b43c)<br />阻塞队列为`DelayedWorkQueue`。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622907846274-1231ccbc-5c93-43b1-92ea-5054973f8207.png#clientId=ubad24891-1b54-4&from=paste&id=ubecb5536&originHeight=178&originWidth=769&originalType=url&ratio=3&status=done&style=none&taskId=ue17f2b1b-b4de-4fa7-88e9-5d9e5005832)<br />因此，原因就找到了，当有多个方法使用`@Scheduled`注解时，就会创建多个定时任务到任务列表中，当其中一个任务没执行完时，其它任务在阻塞队列当中等待，因此，所有的任务都是按照顺序执行的，只不过由于任务执行的速度相当快，感觉任务都是多线程执行的。<br />下面举例来验证一下，将上述的某个定时任务添加睡眠时间，观察另一个定时任务是否输出。
```java
@Slf4j
@EnableScheduling
@Component
public class ScheduleTask {
    private static final ThreadLocal<Integer> threadLocalA = new ThreadLocal<>();

    @Scheduled(cron = "0/2 * * * * ?")
    public void taskA() {
        try {
            log.info("执行了ScheduleTask类中的taskA方法");
            Thread.sleep(TimeUnit.SECONDS.toMillis(10));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Scheduled(cron = "0/1 * * * * ?")
    public void taskB() {
        int num = threadLocalA.get() == null ? 0 : threadLocalA.get();
        log.info("taskB方法执行次数：{}", ++num);
        threadLocalA.set(num);
    }
}
```
输出结果：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622907846425-5b5f9035-5215-49b8-8442-dae8bd1b5240.png#clientId=ubad24891-1b54-4&from=paste&id=u0d84c2cd&originHeight=177&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u78237a2f-d8db-493f-a260-2600332f48c)<br />那么如何解决顺序执行呢？答案是配置定时任务线程池：
```java
@Configuration
public class ScheduleConfig implements SchedulingConfigurer {

    @Override
    public void configureTasks(ScheduledTaskRegistrar taskRegistrar) {
        taskRegistrar.setScheduler(getExecutor());
    }

    @Bean
    public Executor getExecutor(){
        return new ScheduledThreadPoolExecutor(5);
    }
}
```
再次启动观察输出结果：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622907847426-5b07cd55-fa9a-407e-abe4-fbea41f1be76.png#clientId=ubad24891-1b54-4&from=paste&id=u79642c93&originHeight=270&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u0f43528b-393a-4dbc-b666-b428d271bcf)<br />从输出结果可以看到，即使testA休眠，但是testB仍然正常执行，并且其还复用了其它线程，导致执行次数发生了变化。

---

<a name="eMnRA"></a>
### 2、当系统时间发生改变时，`@Scheduled`注解失效
另外一种情况就是在配置完线程池之后，当你手动修改服务器时间时，目前做的测试就是服务器时间调前，则会导致注解失效，而服务器时间调后，则不会影响注解的作用。<br />那么原因是什么呢？<br />在查询资料后得出：
> JVM启动之后会记录当前系统时间，然后JVM根据CPU ticks自己来算时间，此时获取的是定时任务的基准时间。如果此时将系统时间进行了修改，当Spring将之前获取的基准时间与当下获取的系统时间进行比对不一致，就会造成Spring内部定时任务失效。因为此时系统时间发生变化了，不会触发定时任务。

那么这时候怎么解决呢？<br />1. 重启项目。这在生产环境中肯定是不允许的，所以Pass<br />2. 无奈之举，改方案。怎么改呢？就是不适用`@Scheduled`注解，改成<br />`ScheduledThreadPoolExecutor`进行替代。<br />举例说明：下面是项目中所写的部分定时任务<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622907846467-d1dffcfe-ee3e-4610-8384-f25a69fa1f3f.png#clientId=ubad24891-1b54-4&from=paste&id=u25f3f324&originHeight=738&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u63ca6eb2-a24c-4739-acba-7cce525465e)<br />`ScheduledThreadPoolExecutor` 执行流程：

1. 当调用`scheduleAtFixedRate()`方法或者`scheduleWithFixedDelay()`方法时，会向`ScheduledThreadPoolExecutor`的`DelayQueue`添加一个实现了`RunnableScheduledFuture`接口的`ScheduleFutureTask`。
2. 线程池中的线程从`DelayQueue`中获取`ScheduleFutureTask`，然后执行任务。

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622907846645-01130702-175f-4348-a172-b955bca75142.png#clientId=ubad24891-1b54-4&from=paste&id=u89c0d6bd&originHeight=749&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u7c76f26b-b052-4c64-a2b7-b34ef5b7f71)<br />方法说明：
```java
public ScheduledFuture scheduleAtFixedRate(Runnable command,long initialDelay,long period,TimeUnit unit)
```
`scheduleAtFixedRate`方法的作用是预定在初始的延迟结束后，周期性地执行给定的任务，周期长度为`period`，其中`initialDelay`为初始延迟。
```java
public ScheduledFuture scheduleWithFixedDelay(Runnable command,long initialDelay,long delay,TimeUnit unit);
```
`scheduleWithFixedDelay`方法的作用是预定在初始的延迟结束后周期性地执行给定任务，在一次调用完成和下一次调用开始之间有长度为`delay`的延迟，其中`initialDelay`为初始延迟。

