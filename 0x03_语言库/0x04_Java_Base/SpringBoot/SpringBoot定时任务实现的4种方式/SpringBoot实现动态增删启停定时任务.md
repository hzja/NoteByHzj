Java SpringBoot 定时任务<br />在SpringBoot项目中，可以通过`@EnableScheduling`注解和`@Scheduled`注解实现定时任务，也可以通过`SchedulingConfigurer`接口来实现定时任务。但是这两种方式不能动态添加、删除、启动、停止任务。<br />要实现动态增删启停定时任务功能，比较广泛的做法是集成Quartz框架。但是在满足项目需求的情况下，尽量少的依赖其它框架，避免项目过于臃肿和复杂。<br />查看spring-context这个jar包中org.springframework.scheduling.ScheduledTaskRegistrar这个类的源代码，发现可以通过改造这个类就能实现动态增删启停定时任务功能。<br />![定时任务列表页](https://cdn.nlark.com/yuque/0/2021/png/396745/1640348074717-34321080-51c5-405e-a6ed-4890d4fa8c6c.png#clientId=udab861ff-8c06-4&from=ui&id=ufb0100c8&originHeight=207&originWidth=963&originalType=binary&ratio=1&rotation=0&showTitle=true&size=84016&status=done&style=none&taskId=u8ebc0a78-1699-446c-ab6b-77b001cd8c3&title=%E5%AE%9A%E6%97%B6%E4%BB%BB%E5%8A%A1%E5%88%97%E8%A1%A8%E9%A1%B5 "定时任务列表页")<br />![定时任务执行日志](https://cdn.nlark.com/yuque/0/2021/png/396745/1640348074784-0085b779-1170-46aa-8caf-f6c6547f4d0b.png#clientId=udab861ff-8c06-4&from=ui&id=OWVcZ&originHeight=231&originWidth=884&originalType=binary&ratio=1&rotation=0&showTitle=true&size=359333&status=done&style=none&taskId=ud3ce6f75-92cc-4d8a-a659-b35833b04b0&title=%E5%AE%9A%E6%97%B6%E4%BB%BB%E5%8A%A1%E6%89%A7%E8%A1%8C%E6%97%A5%E5%BF%97 "定时任务执行日志")
<a name="eDvyB"></a>
### 添加执行定时任务的线程池配置类
```java
@Configuration  
public class SchedulingConfig {  
    @Bean  
    public TaskScheduler taskScheduler() {  
        ThreadPoolTaskScheduler taskScheduler = new ThreadPoolTaskScheduler();  
        // 定时任务执行线程池核心线程数  
        taskScheduler.setPoolSize(4);  
        taskScheduler.setRemoveOnCancelPolicy(true);  
        taskScheduler.setThreadNamePrefix("TaskSchedulerThreadPool-");  
        return taskScheduler;  
    }  
}  
```
<a name="Ci9Ov"></a>
### 添加`ScheduledFuture`的包装类。`ScheduledFuture`是`ScheduledExecutorService`定时任务线程池的执行结果。
```java
public final class ScheduledTask {  
  
    volatile ScheduledFuture<?> future;  
  
    /**  
     * 取消定时任务  
     */  
    public void cancel() {  
        ScheduledFuture<?> future = this.future;  
        if (future != null) {  
            future.cancel(true);  
        }  
    }  
}  
```
<a name="E16si"></a>
### 添加Runnable接口实现类，被定时任务线程池调用，用来执行指定bean里面的方法。
```java
public class SchedulingRunnable implements Runnable {  
  
    private static final Logger logger = LoggerFactory.getLogger(SchedulingRunnable.class);  
  
    private String beanName;  
  
    private String methodName;  
  
    private String params;  
  
    public SchedulingRunnable(String beanName, String methodName) {  
        this(beanName, methodName, null);  
    }  
  
    public SchedulingRunnable(String beanName, String methodName, String params) {  
        this.beanName = beanName;  
        this.methodName = methodName;  
        this.params = params;  
    }  
  
    @Override  
    public void run() {  
        logger.info("定时任务开始执行 - bean：{}，方法：{}，参数：{}", beanName, methodName, params);  
        long startTime = System.currentTimeMillis();  
  
        try {  
            Object target = SpringContextUtils.getBean(beanName);  
  
            Method method = null;  
            if (StringUtils.isNotEmpty(params)) {  
                method = target.getClass().getDeclaredMethod(methodName, String.class);  
            } else {  
                method = target.getClass().getDeclaredMethod(methodName);  
            }  
  
            ReflectionUtils.makeAccessible(method);  
            if (StringUtils.isNotEmpty(params)) {  
                method.invoke(target, params);  
            } else {  
                method.invoke(target);  
            }  
        } catch (Exception ex) {  
            logger.error(String.format("定时任务执行异常 - bean：%s，方法：%s，参数：%s ", beanName, methodName, params), ex);  
        }  
  
        long times = System.currentTimeMillis() - startTime;  
        logger.info("定时任务执行结束 - bean：{}，方法：{}，参数：{}，耗时：{} 毫秒", beanName, methodName, params, times);  
    }  
  
    @Override  
    public boolean equals(Object o) {  
        if (this == o) return true;  
        if (o == null || getClass() != o.getClass()) return false;  
        SchedulingRunnable that = (SchedulingRunnable) o;  
        if (params == null) {  
            return beanName.equals(that.beanName) &&  
                    methodName.equals(that.methodName) &&  
                    that.params == null;  
        }  
  
        return beanName.equals(that.beanName) &&  
                methodName.equals(that.methodName) &&  
                params.equals(that.params);  
    }  
  
    @Override  
    public int hashCode() {  
        if (params == null) {  
            return Objects.hash(beanName, methodName);  
        }  
  
        return Objects.hash(beanName, methodName, params);  
    }  
}  
```
<a name="ruu9n"></a>
### 添加定时任务注册类，用来增加、删除定时任务。
```java
@Component  
public class CronTaskRegistrar implements DisposableBean {  
  
    private final Map<Runnable, ScheduledTask> scheduledTasks = new ConcurrentHashMap<>(16);  
  
    @Autowired  
    private TaskScheduler taskScheduler;  
  
    public TaskScheduler getScheduler() {  
        return this.taskScheduler;  
    }  
  
    public void addCronTask(Runnable task, String cronExpression) {  
        addCronTask(new CronTask(task, cronExpression));  
    }  
  
    public void addCronTask(CronTask cronTask) {  
        if (cronTask != null) {  
            Runnable task = cronTask.getRunnable();  
            if (this.scheduledTasks.containsKey(task)) {  
                removeCronTask(task);  
            }  
  
            this.scheduledTasks.put(task, scheduleCronTask(cronTask));  
        }  
    }  
  
    public void removeCronTask(Runnable task) {  
        ScheduledTask scheduledTask = this.scheduledTasks.remove(task);  
        if (scheduledTask != null)  
            scheduledTask.cancel();  
    }  
  
    public ScheduledTask scheduleCronTask(CronTask cronTask) {  
        ScheduledTask scheduledTask = new ScheduledTask();  
        scheduledTask.future = this.taskScheduler.schedule(cronTask.getRunnable(), cronTask.getTrigger());  
  
        return scheduledTask;  
    }  
  
  
    @Override  
    public void destroy() {  
        for (ScheduledTask task : this.scheduledTasks.values()) {  
            task.cancel();  
        }  
  
        this.scheduledTasks.clear();  
    }  
}  
```
<a name="zHveg"></a>
### 添加定时任务示例类
```java
@Component("demoTask")  
public class DemoTask {  
    public void taskWithParams(String params) {  
        System.out.println("执行有参示例任务：" + params);  
    }  
  
    public void taskNoParams() {  
        System.out.println("执行无参示例任务");  
    }  
}  
```
<a name="xzhzS"></a>
### 定时任务数据库表设计
![定时任务数据库表设计](https://cdn.nlark.com/yuque/0/2021/png/396745/1640348113212-49025df2-5ff9-4a68-851b-488ef6c8991a.png#clientId=udab861ff-8c06-4&from=ui&id=uf8c9054e&originHeight=399&originWidth=657&originalType=binary&ratio=1&rotation=0&showTitle=true&size=52389&status=done&style=shadow&taskId=ucec45cc9-bb32-4026-a9e6-1a81a75d303&title=%E5%AE%9A%E6%97%B6%E4%BB%BB%E5%8A%A1%E6%95%B0%E6%8D%AE%E5%BA%93%E8%A1%A8%E8%AE%BE%E8%AE%A1 "定时任务数据库表设计")
<a name="DurA4"></a>
### 添加定时任务实体类
```java
public class SysJobPO {  
    /**  
     * 任务ID  
     */  
    private Integer jobId;  
    /**  
     * bean名称  
     */  
    private String beanName;  
    /**  
     * 方法名称  
     */  
    private String methodName;  
    /**  
     * 方法参数  
     */  
    private String methodParams;  
    /**  
     * cron表达式  
     */  
    private String cronExpression;  
    /**  
     * 状态（1正常 0暂停）  
     */  
    private Integer jobStatus;  
    /**  
     * 备注  
     */  
    private String remark;  
    /**  
     * 创建时间  
     */  
    private Date createTime;  
    /**  
     * 更新时间  
     */  
    private Date updateTime;  
  
    public Integer getJobId() {  
        return jobId;  
    }  
  
    public void setJobId(Integer jobId) {  
        this.jobId = jobId;  
    }  
  
    public String getBeanName() {  
        return beanName;  
    }  
  
    public void setBeanName(String beanName) {  
        this.beanName = beanName;  
    }  
  
    public String getMethodName() {  
        return methodName;  
    }  
  
    public void setMethodName(String methodName) {  
        this.methodName = methodName;  
    }  
  
    public String getMethodParams() {  
        return methodParams;  
    }  
  
    public void setMethodParams(String methodParams) {  
        this.methodParams = methodParams;  
    }  
  
    public String getCronExpression() {  
        return cronExpression;  
    }  
  
    public void setCronExpression(String cronExpression) {  
        this.cronExpression = cronExpression;  
    }  
  
    public Integer getJobStatus() {  
        return jobStatus;  
    }  
  
    public void setJobStatus(Integer jobStatus) {  
        this.jobStatus = jobStatus;  
    }  
  
    public String getRemark() {  
        return remark;  
    }  
  
    public void setRemark(String remark) {  
        this.remark = remark;  
    }  
  
    public Date getCreateTime() {  
        return createTime;  
    }  
  
    public void setCreateTime(Date createTime) {  
        this.createTime = createTime;  
    }  
  
    public Date getUpdateTime() {  
        return updateTime;  
    }  
  
    public void setUpdateTime(Date updateTime) {  
        this.updateTime = updateTime;  
    }  
  
}  
```
<a name="Y9wsI"></a>
### 新增定时任务
![新增定时任务](https://cdn.nlark.com/yuque/0/2021/png/396745/1640348139302-020d9163-d6b3-4ee7-a1fb-cc67d0e022b4.png#clientId=udab861ff-8c06-4&from=ui&id=ub2382dab&originHeight=478&originWidth=486&originalType=binary&ratio=1&rotation=0&showTitle=true&size=44452&status=done&style=shadow&taskId=u021d1827-cade-46fe-a49c-00ea4a814dd&title=%E6%96%B0%E5%A2%9E%E5%AE%9A%E6%97%B6%E4%BB%BB%E5%8A%A1 "新增定时任务")
```java
boolean success = sysJobRepository.addSysJob(sysJob);  
if (!success)  
    return OperationResUtils.fail("新增失败");  
else {  
    if (sysJob.getJobStatus().equals(SysJobStatus.NORMAL.ordinal())) {  
        SchedulingRunnable task = new SchedulingRunnable(sysJob.getBeanName(), sysJob.getMethodName(), sysJob.getMethodParams());  
        cronTaskRegistrar.addCronTask(task, sysJob.getCronExpression());  
    }  
}  
  
return OperationResUtils.success();  
```
<a name="n4uEp"></a>
### 修改定时任务，先移除原来的任务，再启动新任务
```java
boolean success = sysJobRepository.editSysJob(sysJob);  
if (!success)  
    return OperationResUtils.fail("编辑失败");  
else {  
    //先移除再添加  
    if (existedSysJob.getJobStatus().equals(SysJobStatus.NORMAL.ordinal())) {  
        SchedulingRunnable task = new SchedulingRunnable(existedSysJob.getBeanName(), existedSysJob.getMethodName(), existedSysJob.getMethodParams());  
        cronTaskRegistrar.removeCronTask(task);  
    }  
  
    if (sysJob.getJobStatus().equals(SysJobStatus.NORMAL.ordinal())) {  
        SchedulingRunnable task = new SchedulingRunnable(sysJob.getBeanName(), sysJob.getMethodName(), sysJob.getMethodParams());  
        cronTaskRegistrar.addCronTask(task, sysJob.getCronExpression());  
    }  
}  
  
return OperationResUtils.success();  
```
<a name="Adkwe"></a>
### 删除定时任务
```java
boolean success = sysJobRepository.deleteSysJobById(req.getJobId());  
if (!success)  
    return OperationResUtils.fail("删除失败");  
else{  
    if (existedSysJob.getJobStatus().equals(SysJobStatus.NORMAL.ordinal())) {  
        SchedulingRunnable task = new SchedulingRunnable(existedSysJob.getBeanName(), existedSysJob.getMethodName(), existedSysJob.getMethodParams());  
        cronTaskRegistrar.removeCronTask(task);  
    }  
}  
return OperationResUtils.success();  
```
<a name="JJ10Q"></a>
### 定时任务启动/停止状态切换
```java
if (existedSysJob.getJobStatus().equals(SysJobStatus.NORMAL.ordinal())) {  
    SchedulingRunnable task = new SchedulingRunnable(existedSysJob.getBeanName(), existedSysJob.getMethodName(), existedSysJob.getMethodParams());  
    cronTaskRegistrar.addCronTask(task, existedSysJob.getCronExpression());  
} else {  
    SchedulingRunnable task = new SchedulingRunnable(existedSysJob.getBeanName(), existedSysJob.getMethodName(), existedSysJob.getMethodParams());  
    cronTaskRegistrar.removeCronTask(task);  
}  
```
添加实现了`CommandLineRunner`接口的`SysJobRunner`类，当SpringBoot项目启动完成后，加载数据库里状态为正常的定时任务。
```java
@Service  
public class SysJobRunner implements CommandLineRunner {  
  
    private static final Logger logger = LoggerFactory.getLogger(SysJobRunner.class);  
  
    @Autowired  
    private ISysJobRepository sysJobRepository;  
  
    @Autowired  
    private CronTaskRegistrar cronTaskRegistrar;  
  
    @Override  
    public void run(String... args) {  
        // 初始加载数据库里状态为正常的定时任务  
        List<SysJobPO> jobList = sysJobRepository.getSysJobListByStatus(SysJobStatus.NORMAL.ordinal());  
        if (CollectionUtils.isNotEmpty(jobList)) {  
            for (SysJobPO job : jobList) {  
                SchedulingRunnable task = new SchedulingRunnable(job.getBeanName(), job.getMethodName(), job.getMethodParams());  
                cronTaskRegistrar.addCronTask(task, job.getCronExpression());  
            }  
  
            logger.info("定时任务已加载完毕...");  
        }  
    }  
}  
```
工具类`SpringContextUtils`，用来从Spring容器里获取bean
```java
@Component  
public class SpringContextUtils implements ApplicationContextAware {  
  
    private static ApplicationContext applicationContext;  
  
    @Override  
    public void setApplicationContext(ApplicationContext applicationContext)  
            throws BeansException {  
        SpringContextUtils.applicationContext = applicationContext;  
    }  
  
    public static Object getBean(String name) {  
        return applicationContext.getBean(name);  
    }  
  
    public static <T> T getBean(Class<T> requiredType) {  
        return applicationContext.getBean(requiredType);  
    }  
  
    public static <T> T getBean(String name, Class<T> requiredType) {  
        return applicationContext.getBean(name, requiredType);  
    }  
  
    public static boolean containsBean(String name) {  
        return applicationContext.containsBean(name);  
    }  
  
    public static boolean isSingleton(String name) {  
        return applicationContext.isSingleton(name);  
    }  
  
    public static Class<? extends Object> getType(String name) {  
        return applicationContext.getType(name);  
    }  
}
```
