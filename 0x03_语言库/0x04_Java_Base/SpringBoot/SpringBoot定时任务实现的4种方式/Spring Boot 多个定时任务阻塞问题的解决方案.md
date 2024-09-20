Java SpringBoot 定时任务
<a name="UZaSn"></a>
## 为什么Spring Boot 定时任务是单线程的？
想要解释为什么，一定要从源码入手，直接从`@EnableScheduling`这个注解入手，找到了这个`ScheduledTaskRegistrar`类，其中有一段代码如下：
```java
protected void scheduleTasks() {
    if (this.taskScheduler == null) {
        this.localExecutor = Executors.newSingleThreadScheduledExecutor();
        this.taskScheduler = new ConcurrentTaskScheduler(this.localExecutor);
    }
}
```
如果`taskScheduler`为null，则创建单线程的线程池：`Executors.newSingleThreadScheduledExecutor()`。
<a name="ki7D1"></a>
## 多线程定时任务如何配置？
下面介绍三种方案配置多线程下的定时任务。
<a name="ZdEKR"></a>
### 1、重写`SchedulingConfigurer#configureTasks()`
直接实现`SchedulingConfigurer`这个接口，设置`taskScheduler`，代码如下：
```java
@Configuration
public class ScheduleConfig implements SchedulingConfigurer {
    @Override
    public void configureTasks(ScheduledTaskRegistrar taskRegistrar) {
        //设定一个长度10的定时任务线程池
        taskRegistrar.setScheduler(Executors.newScheduledThreadPool(10));
    }
}
```
<a name="WiVuf"></a>
### 2、通过配置开启
Spring Boot quartz 已经提供了一个配置用来配置线程池的大小，如下；
```
spring.task.scheduling.pool.size=10
```
只需要在配置文件中添加如上的配置即可生效！
<a name="Y9ATW"></a>
### 3、结合`@Async`
`@Async`这个注解都用过，用来开启异步任务的，使用`@Async`这个注解之前一定是要先配置线程池的，配置如下：
```java
@Bean
public ThreadPoolTaskExecutor taskExecutor() {
    ThreadPoolTaskExecutor poolTaskExecutor = new ThreadPoolTaskExecutor();
    poolTaskExecutor.setCorePoolSize(4);
    poolTaskExecutor.setMaxPoolSize(6);
    // 设置线程活跃时间（秒）
    poolTaskExecutor.setKeepAliveSeconds(120);
    // 设置队列容量
    poolTaskExecutor.setQueueCapacity(40);
    poolTaskExecutor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
    // 等待所有任务结束后再关闭线程池
    poolTaskExecutor.setWaitForTasksToCompleteOnShutdown(true);
    return poolTaskExecutor;
}
```
然后在`@Scheduled`方法上标注`@Async`这个注解即可实现多线程定时任务，代码如下：
```java
@Async
@Scheduled(cron = "0/2 * * * * ? ")
public void test2() {
    System.out.println("..................执行test2.................");
}
```
