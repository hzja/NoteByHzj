JavaSpringBoot定时任务
<a name="BKQ3f"></a>
## 1、自定义任务调度
首先覆盖 `TaskSchedulingAutoConfiguration` 自动配置类里面的 `ThreadPoolTaskScheduler` Bean：
```java
/**
 * 自定义任务调度
 */
@Data
@Component
class CustomTaskScheduler extends ThreadPoolTaskScheduler {

    private Map<Object, ScheduledFuture<?>> scheduledTasks = new IdentityHashMap<>();

    @Override
    public ScheduledFuture<?> schedule(Runnable task, Trigger trigger) {
        ScheduledFuture<?> future = super.schedule(task, trigger);
        this.putScheduledTasks(task, future);
        return future;
    }

    @Override
    public ScheduledFuture<?> scheduleAtFixedRate(Runnable task, long period) {
        ScheduledFuture<?> future = super.scheduleAtFixedRate(task, period);
        this.putScheduledTasks(task, future);
        return future;
    }

    @Override
    public ScheduledFuture<?> scheduleAtFixedRate(Runnable task, Date startTime, long period) {
        ScheduledFuture<?> future = super.scheduleAtFixedRate(task, startTime, period);
        this.putScheduledTasks(task, future);
        return future;
    }

    private void putScheduledTasks(Runnable task, ScheduledFuture<?> future) {
        ScheduledMethodRunnable runnable = (ScheduledMethodRunnable) task;
        scheduledTasks.put(runnable.getTarget(), future);
    }
    
    // 重写所有 schedule* 方法...

}
```
重写所有 `schedule*` 方法...<br />因为要停止一个任务，就必须调用 `ScheduledFuture` -> `Future` 接口中的 cancel 方法。<br />所以，思路就是在任务执行的时候，把任务所在的实例 Bean 和任务启动后的 `ScheduledFuture` 维护到一个 Map 里面，然后需要停止的时候，从 Map 里面取出来，再进行 cancel 停止即可。
<a name="Oy3BO"></a>
## 2、按条件自动停止任务
新建一个每 3 秒执行一次的任务：
```java
/**
 * 按条件自动停止任务
 */
@Slf4j
@Component
public class AutoStopTask {

    @Autowired
    private CustomTaskScheduler customTaskScheduler;

    private int count;

    @Scheduled(cron = "*/3 * * * * *")
    public void printTask() {
        log.info("任务执行次数：{}", count + 1);

        count++;

        // 执行3次后自动停止
        if (count >= 3) {
            log.info("任务已执行指定次数，现在自动停止");
            boolean cancelled = customTaskScheduler.getScheduledTasks().get(this).cancel(true);
            
            // 停止后再次启动
            if (cancelled) {
                count = 0;
                ScheduledMethodRunnable runnable = new ScheduledMethodRunnable(this, ReflectionUtils.findMethod(this.getClass(), "printTask"));
                customTaskScheduler.schedule(runnable, new CronTrigger("*/3 * * * * *"));
            }
        }
    }

}
```
这里是统计执行，当执行次数超过 3 次时就自动停止。如果需要再次启动，上面也提供了参数代码。<br />需要注意的是，自定义调度里面绑定的是实例 Bean 和 Future 的关系，所以仅限 Bean 中的单个任务，如果一个 Bean 维护了多个任务，最后一个任务的启动就会覆盖之前的。<br />如果要维护 Bean 中的多个任务，自动停止该怎么做呢？<br />答案就是把任务的方法名和 Future 关联起来：
```java
scheduledMethodTasks.put(runnable.getMethod(), future);
```
取的的根据当前的方法名取就行了，这里是方法名，也可是类名＋方法名＋参数，防止重复。
