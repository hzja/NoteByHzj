Java SpringBoot 定时任务
<a name="muin2"></a>
## 定时任务实现的几种方式

1. Timer：这是java自带的java.util.Timer类，这个类允许调度一个`java.util.TimerTask`任务。使用这种方式可以让程序按照某一个频度执行，但不能在指定时间运行。一般用的较少。
2. ScheduledExecutorService：也jdk自带的一个类；是基于线程池设计的定时任务类,每个调度任务都会分配到线程池中的一个线程去执行,也就是说,任务是并发执行,互不影响。
3. Spring Task：Spring3.0以后自带的task，可以将它看成一个轻量级的Quartz，而且使用起来比Quartz简单许多。
4. Quartz：这是一个功能比较强大的的调度器，可以让程序在指定时间执行，也可以按照某一个频度执行，配置起来稍显复杂。
<a name="YeHKn"></a>
## 1、使用Timer实现定时任务
```java
public class TestTimer {
    public static void main(String[] args) {
        TimerTask timerTask = new TimerTask() {
            @Override
            public void run() {
                System.out.println("task  run:"+ new Date());
            }
        };
        Timer timer = new Timer();
        //安排指定的任务在指定的时间开始进行重复的固定延迟执行。这里是每3秒执行一次
        timer.schedule(timerTask,10,3000);
    }
}
```
<a name="XXNkc"></a>
## 2、使用ScheduledExecutorService实现定时任务
```java
public class TestScheduledExecutorService {
    public static void main(String[] args) {
        ScheduledExecutorService service = Executors.newSingleThreadScheduledExecutor();
        // 参数：1、任务体 2、首次执行的延时时间
        //      3、任务执行间隔 4、间隔时间单位
        service.scheduleAtFixedRate(()->System.out.println("task ScheduledExecutorService "+new Date()), 0, 3, TimeUnit.SECONDS);
    }
}
```
Scheduled只适合处理简单的计划任务，不能处理分布式计划任务。优势：是spring框架提供的计划任务，开发简单，执行效率比较高。且在计划任务数量太多的时候，可能出现阻塞，崩溃，延迟启动等问题。<br />Scheduled定时任务是spring3.0版本之后自带的一个定时任务。其所属Spring的资源包为：spring-context-support。所以需要使用Scheduled定时任务机制时，需要在工程中依赖对应资源，具体如下：
```xml
<dependency>
  <groupId>org.springframeworkgroupId>
  <artifactId>spring-context-supportartifactId>
<dependency>
```
如果在spring应用中需要启用Scheduled定时任务，则需要在启动类上增加注解`@EnableScheduling`，代表启用Scheduled定时任务机制。具体如下：
```java
@SpringBootApplication
@EnableScheduling
public class AppStarter {
    public static void main(String[] args) {
        SpringApplication.run(AppStarter.class, args);
    }
}
```
Scheduled定时任务的核心在于注解`@Scheduled`，这个注解的核心属性是cron，代表定时任务的触发计划表达式。这个表达式的格式为：
```java
@Scheduled(cron="seconds minutes hours day month week")
```
或
```java
@Scheduled(cron="seconds minutes hours day month week year")
```
推荐使用第一种表达式形式，因为在很多其他技术中都有不同的定时任务机制，其中用于设置触发计划的表达式都是第一种cron表达式。第二种表达式不能说是Spring Scheduled特有的，也是只有少数技术支持的。<br />cron表达式中，每个位置的约束如下：

| 位置 | 时间域名 | 允许值 | 允许的特殊字符 |
| --- | --- | --- | --- |
| 1 | 秒 | 0-59 | `,` `-` `*` `/` |
| 2 | 分钟 | 0-59 | `,` `-` `*` `/` |
| 3 | 小时 | 0-23 | `,` `-` `*` `/` |
| 4 | 日 | 1-31 | `,` `-` `*` `/` `L` `W` `C` |
| 5 | 月 | 1-12 | `,` `-` `*` `/` |
| 6 | 星期 | 1-7 | `,` `-` `*` `/` `?` `L` `C` `#` |
| 7 | 年（可选） | 1980-2099 | `,` `-` `*` `/` |

星号(`*`)：可用在所有字段中，表示对应时间域的每一个时刻，例如，*在分钟字段时，表示“每分钟”；<br />问号(`?`)：该字符只在日期和星期字段中使用，它通常指定为“无意义的值”，相当于占位符；<br />减号(`-`)：表达一个范围，如在小时字段中使用“10-12”，则表示从10到12点，即10,11,12；<br />逗号(`,`)：表达一个列表值，如在星期字段中使用“MON,WED,FRI”，则表示星期一，星期三和星期五；<br />斜杠(`/`)：x/y表达一个等步长序列，x为起始值，y为增量步长值。如在秒数字段中使用0/15，则表示为0,15,30和45秒，而5/15在分钟字段中表示5,20,35,50，你也可以使用*/y，它等同于0/y；<br />`L`：该字符只在日期和星期字段中使用，代表“Last”的意思，但它在两个字段中意思不同。L在日期字段中，表示这个月份的最后一天，如一月的31号，非闰年二月的28号；如果L用在星期中，则表示星期六，等同于7。但是，如果L出现在星期字段里，而且在前面有一个数值X，则表示“这个月的最后X天”，例如，6L表示该月的最后星期五；<br />`W`：该字符只能出现在日期字段里，是对前导日期的修饰，表示离该日期最近的工作日。例如15W表示离该月15号最近的工作日，如果该月15号是星期六，则匹配14号星期五；如果15日是星期日，则匹配16号星期一；如果15号是星期二，那结果就是15号星期二。但必须注意关联的匹配日期不能够跨月，如你指定1W，如果1号是星期六，结果匹配的是3号星期一，而非上个月最后的那天。W字符串只能指定单一日期，而不能指定日期范围；<br />`LW组合`：在日期字段可以组合使用LW，它的意思是当月的最后一个工作日；<br />`井号(#)`：该字符只能在星期字段中使用，表示当月某个工作日。如6#3表示当月的第三个星期五(6表示星期五，#3表示当前的第三个)，而4#5表示当月的第五个星期三，假设当月没有第五个星期三，忽略不触发；<br />`C`：该字符只在日期和星期字段中使用，代表“Calendar”的意思。它的意思是计划所关联的日期，如果日期没有被关联，则相当于日历中所有日期。例如5C在日期字段中就相当于日历5日以后的第一天。1C在星期字段中相当于星期日后的第一天。
:::info
**Cron表达式对特殊字符的大小写不敏感，对代表星期的缩写英文大小写也不敏感。**
:::
计划任务Scheduled是通过一个线程池实现的。是一个多线程的调度。SpringBoot会初始化一个线程池，线程池默认大小为1，专门用于执行计划任务。每个计划任务启动的时候，都从线程池中获取一个线程执行，如果发生异常，只是执行当前任务的线程发生异常，而不是计划任务调度线程发生异常。如果当前定时任务还未执行完成，当相同的定时任务又进入到执行周期时，不会触发新的定时任务。如：
```java
@Scheduled(cron="* * * * * ?")
public void test1(){
    Random r = new Random();
    /*int i = r.nextInt(100);
    if(i % 3 == 0){
        throw new RuntimeException("error");
    }*/
    System.out.println(Thread.currentThread().getName() + " cron=* * * * * ? --- " + new Date());
    try{
        Thread.sleep(2000);
    }catch(Exception e){
        e.printStackTrace();
    }
}
```
如结果所示(每次的线程名称一致，由于前一个定时任务未执行完成，因此造成后一个任务的推迟，而不是1秒执行一次，而是3秒)：
```java
pool-1-thread-1 cron=* * * * * ? --- Thu Sep 19 02:23:20 CST 2019
pool-1-thread-1 cron=* * * * * ? --- Thu Sep 19 02:23:23 CST 2019
pool-1-thread-1 cron=* * * * * ? --- Thu Sep 19 02:23:26 CST 2019
pool-1-thread-1 cron=* * * * * ? --- Thu Sep 19 02:23:29 CST 2019
```
<a name="IHjIX"></a>
## 3、使用Spring Task实现定时任务
<a name="j9kR9"></a>
### 简单的定时任务
在SpringBoot项目中，可以很优雅的使用注解来实现定时任务，首先创建项目，导入依赖：
```xml
<dependencies>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
  </dependency>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter</artifactId>
  </dependency>
  <dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <optional>true</optional>
  </dependency>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-test</artifactId>
    <scope>test</scope>
  </dependency>
</dependencies>
```
创建任务类
```java
@Slf4j
@Component
public class ScheduledService {
    @Scheduled(cron = "0/5 * * * * *")
    public void scheduled(){
        log.info("=====>>>>>使用cron  {}",System.currentTimeMillis());
    }
    
    @Scheduled(fixedRate = 5000)
    public void scheduled1() {
        log.info("=====>>>>>使用fixedRate{}", System.currentTimeMillis());
    }

    @Scheduled(fixedDelay = 5000)
    public void scheduled2() {
        log.info("=====>>>>>fixedDelay{}",System.currentTimeMillis());
    }
}
```
在主类上使用`@EnableScheduling`注解开启对定时任务的支持，然后启动项目<br />可以看到三个定时任务都已经执行，并且使同一个线程中串行执行，如果只有一个定时任务，这样做肯定没问题，当定时任务增多，如果一个任务卡死，会导致其他任务也无法执行。
<a name="izdgr"></a>
### 多线程执行
在传统的Spring项目中，可以在xml配置文件添加task的配置，而在SpringBoot项目中一般使用config配置类的方式添加配置，所以新建一个AsyncConfig类
```java
@Configuration
@EnableAsync
public class AsyncConfig {
     /*
    此处成员变量应该使用@Value从配置中读取
     */
    private int corePoolSize = 10;
    private int maxPoolSize = 200;
    private int queueCapacity = 10;

    @Bean
    public Executor taskExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(corePoolSize);
        executor.setMaxPoolSize(maxPoolSize);
        executor.setQueueCapacity(queueCapacity);
        executor.initialize();
        return executor;
    }
}
```
`@Configuration`：表明该类是一个配置类`@EnableAsync`：开启异步事件的支持 然后在定时任务的类或者方法上添加@Async 。最后重启项目，每一个任务都是在不同的线程中
<a name="Uj9QI"></a>
### 执行时间的配置
在上面的定时任务中，在方法上使用@Scheduled注解来设置任务的执行时间，并且使用三种属性配置方式：<br />fixedRate：定义一个按一定频率执行的定时任务<br />fixedDelay：定义一个按一定频率执行的定时任务，与上面不同的是，改属性可以配合initialDelay， 定义该任务延迟执行时间。<br />cron：通过表达式来配置任务执行时间
<a name="vrida"></a>
## 4、整合Quartz实现定时任务
Quartz是OpenSymphony开源组织在Job scheduling领域又一个开源项目，它可以与J2EE与J2SE应用程序相结合也可以单独使用。Quartz可以用来创建简单或为运行十个，百个，甚至是好几万个Jobs这样复杂的程序。<br />Quartz是一个完全由java编写的开源作业调度框架。尽管Quartz框架整合了许多额外功能， 但就其简易形式看，可以发现它十分易用！<br />在开发Quartz相关应用时，只要定义了Job（任务），Trigger（触发器）和Scheduler（调度器），即可实现一个定时调度能力。其中Scheduler是Quartz中的核心，Scheduler负责管理Quartz应用运行时环境，Scheduler不是靠自己完成所有的工作，是根据Trigger的触发标准，调用Job中的任务执行逻辑，来完成完整的定时任务调度。<br />Job - 定时任务内容是什么。<br />Trigger - 在什么时间上执行job。<br />Scheduler - 维护定时任务环境，并让触发器生效。
<a name="gYuN7"></a>
### 添加依赖
如果SpringBoot版本是2.0.0以后的，则在spring-boot-starter中已经包含了quart的依赖，则可以直接使用spring-boot-starter-quartz依赖
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-quartz</artifactId>
</dependency>
```
如果是1.5.9则要使用以下添加依赖：
```xml
<dependency>
  <groupId>org.quartz-scheduler</groupId>
  <artifactId>quartz</artifactId>
  <version>2.3.0</version>
</dependency>
<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-context-support</artifactId>
</dependency>
```
这里使用SpringBoot版本是2.0.0.BUILD-SNAPSHOT ，该版本开始集成了Quartz，所以事实现起来很方便。创建任务类TestQuartz，该类主要是继承了QuartzJobBean
```java
public class TestQuartz extends QuartzJobBean {
    /**
     * 执行定时任务
     * @param jobExecutionContext
     * @throws JobExecutionException
     */
    @Override
    protected void executeInternal(JobExecutionContext jobExecutionContext) throws JobExecutionException {
        System.out.println("quartz task "+new Date());
    }
}
```
<a name="zTlxr"></a>
#### 创建配置类QuartzConfig
```java
@Configuration
public class QuartzConfig {
    @Bean
    public JobDetail teatQuartzDetail(){
        return JobBuilder.newJob(TestQuartz.class).withIdentity("testQuartz").storeDurably().build();
    }

    @Bean
    public Trigger testQuartzTrigger(){
        SimpleScheduleBuilder scheduleBuilder = SimpleScheduleBuilder.simpleSchedule()
                .withIntervalInSeconds(10)  //设置时间周期单位秒
                .repeatForever();
        return TriggerBuilder.newTrigger().forJob(teatQuartzDetail())
                .withIdentity("testQuartz")
                .withSchedule(scheduleBuilder)
                .build();
    }
}
```
<a name="JyrYb"></a>
#### 启动项目即可看到任务的执行日志
<a name="7LeQJ"></a>
### Job任务调用业务需要重写JobFactory的实现
<a name="QDPQX"></a>
#### 启动器添加注解`@EnableScheduling`
```java
/**
 * @EnableScheduling 必要
 * 开启定时任务机制。
 */
@SpringBootApplication
@EnableScheduling
public class AppStarter {
    public static void main(String[] args) {
        SpringApplication.run(AppStarter.class, args);
    }
}
```
定义JOB任务以及JOB任务调用的模拟业务对象：
```java
public class SpringBootQuartzJobDemo implements Job {
    // 用于模拟任务中的业务对象。也可能是数据访问对象，或其他类型的对象。
    @Autowired
    private CommonsUtil4Quartz commonsUtil4Quartz;

    @Override
    public void execute(JobExecutionContext context) throws JobExecutionException {
        System.out.println("SpringBootQuartzJobDemo : " + new Date());
        this.commonsUtil4Quartz.testMethod();
    }
}
@Component
public class CommonsUtil4Quartz {
    public void testMethod(){
        System.out.println("CommonsUtil4Quartz testMethod run...");
    }
}
```
<a name="gD7ga"></a>
#### 创建Trigger以及JobDetail对象，并用Schedule配置定时任务
```java
/**
 * 初始化类
 * Quartz环境初始化。
 *
 */
@Configuration
public class QuartzConfiguration {
    /**
     * 创建Job对象。在Spring环境中，创建一个类型的对象的时候，很多情况下，都是通过FactoryBean来间接创建的。
     * 如果有多个Job对象，定义多次方法。
     *
     * 在JobDetailFactoryBean类型中，用于创建JobDetail对象的方法，其底层使用的逻辑是：Class.newInstance()
     * 也就是说，JobDetail对象不是通过Spring容器管理的。
     * 因为Spring容器不管理JobDetail对象，那么Job中需要自动装配的属性，就无法实现自动状态。如上JOB的第10行会报空指针异常。
     *
     * 解决方案是：将JobDetail加入到Spring容器中，让Spring容器管理JobDetail对象。
     * 需要重写Factory相关代码。实现Spring容器管理JobDetail。
     * @return
     */
    @Bean
    public JobDetailFactoryBean initJobDetailFactoryBean(){
        JobDetailFactoryBean factoryBean =
            new JobDetailFactoryBean();
        // 提供job类型。
        factoryBean.setJobClass(SpringBootQuartzJobDemo.class);
        return factoryBean;
    }

    /**
     * 管理Trigger对象
     * CronTrigger - 就是Trigger的一个实现类型。其中用于定义周期时间的是CronSchedulerBuilder
     * 实际上，CronTrigger是用于管理一个Cron表达式的类型。
     * @param jobDetailFactoryBean - 上一个方法初始化的JobDetailFactoryBean
     * @return
     */
    @Bean(name="cronTriggerFactoryBean1")
    public CronTriggerFactoryBean initCronTriggerFactoryBean(
    ){
        CronTriggerFactoryBean factoryBean =
            new CronTriggerFactoryBean();
        JobDetailFactoryBean jobDetailFactoryBean = this.initJobDetailFactoryBean();
        factoryBean.setJobDetail(jobDetailFactoryBean.getObject());
        factoryBean.setCronExpression("0/3 * * * * ?");
        return factoryBean;
    }

    /**
     * 初始化Scheduler
     * @param cronTriggerFactoryBean - 上一个方法初始化的CronTriggerFactoryBean
     * @return
     */
    @Bean
    public SchedulerFactoryBean initSchedulerFactoryBean(
        CustomJobFactory customJobFactory,
        CronTriggerFactoryBean[] cronTriggerFactoryBean){
        SchedulerFactoryBean factoryBean =
            new SchedulerFactoryBean();
        CronTrigger[] triggers = new CronTrigger[cronTriggerFactoryBean.length];
        for(int i = 0; i < cronTriggerFactoryBean.length; i++){
            triggers[i] = cronTriggerFactoryBean[i].getObject();
        }
        // 注册触发器，一个Scheduler可以注册若干触发器。
        factoryBean.setTriggers(triggers);
        // 为Scheduler设置JobDetail的工厂。可以覆盖掉SpringBoot提供的默认工厂，保证JobDetail中的自动装配有效。
        factoryBean.setJobFactory(customJobFactory);
        return factoryBean;
    }
}
```
<a name="iknQd"></a>
#### 重写JobFactory
```java
/**
 * 重写的工厂对象。
 */
@Component
public class CustomJobFactory extends AdaptableJobFactory {
    /**
     * AutowireCapableBeanFactory : 简单理解为Spring容器，是Spring容器Context的一个Bean对象管理工程。
     * 可以实现自动装配逻辑，和对象创建逻辑。
     * 是SpringIoC容器的一个重要组成部件。
     */
    @Autowired
    private AutowireCapableBeanFactory autowireCapableBeanFactory;

    @Override
    protected Object createJobInstance(TriggerFiredBundle bundle) throws Exception {
        // 通过父类型中的方法，创建JobDetail对象。
        Object obj = super.createJobInstance(bundle);
        // 将JobDetail对象加入到Spring容器中，让Spring容器管理，并实现自动装配逻辑。
        this.autowireCapableBeanFactory.autowireBean(obj);
        return obj;
    }
}
```
<a name="Zvf7e"></a>
### 分布式quartz配置
<a name="8a9b7a5f"></a>
#### 1、资源依赖配置
由于分布式的原因，Quartz中提供分布式处理的jar包以及数据库及连接相关的依赖。
```xml
<dependency>
	<groupId>org.springframework<groupId>
	<artifactId>spring-context-support<artifactId>
<dependency>

<dependency>
	<groupId>org.quartz-scheduler<groupId>
	<artifactId>quartz<artifactId>
	<version>2.2.1<version>
	<exclusions>
		<exclusion>
			<artifactId>slf4j-api<artifactId>
			<groupId>org.slf4j<groupId>
		<exclusion>
	<exclusions>
<dependency>

<dependency>
	<groupId>org.quartz-scheduler<groupId>
	<artifactId>quartz-jobs<artifactId>
	<version>2.2.1<version>
<dependency>

<dependency>
	<groupId>org.springframework<groupId>
	<artifactId>spring-tx<artifactId>
<dependency>

<dependency>
	<groupId>org.springframework.boot<groupId>
	<artifactId>spring-boot-starter-web<artifactId>
<dependency>

<dependency>
	<groupId>org.springframework.boot<groupId>
	<artifactId>spring-boot-starter-data-jpa<artifactId>
<dependency>

<dependency>
	<groupId>mysql<groupId>
	<artifactId>mysql-connector-java<artifactId>
<dependency>

<dependency>
	<groupId>com.alibaba<groupId>
	<artifactId>druid<artifactId>
	<version>1.0.9<version>
<dependency>
```
<a name="HtBQo"></a>
#### 2、数据库相关配置
```
spring.datasource.url=jdbc:mysql://localhost:3306/quartz?autoReconnect=true&useUnicode=true&characterEncoding=utf-8
spring.datasource.username=root
spring.datasource.password=root
spring.datasource.driver-class-name=com.mysql.jdbc.Driver
spring.datasource.type=com.alibaba.druid.pool.DruidDataSource
spring.datasource.initialSize=5
spring.datasource.minIdle=5
spring.datasource.maxActive=20
spring.datasource.maxWait=600000
spring.datasource.timeBetweenEvictionRunsMillis=600000
spring.datasource.minEvictableIdleTimeMillis=300000
spring.datasource.validationQuery=SELECT 1
spring.datasource.testWhileIdle=true
spring.datasource.testOnBorrow=false
spring.datasource.testOnReturn=false
server.port=8080
```
<a name="UySpY"></a>
#### 3、Quartz配置信息
为Quartz提供数据库配置信息，如数据库，表格的前缀之类的。
```
# 是否使用properties作为数据存储
org.quartz.jobStore.useProperties=false
# 数据库中的表格命名前缀
org.quartz.jobStore.tablePrefix = QRTZ_
# 是否是一个集群，是不是分布式的任务
org.quartz.jobStore.isClustered = true
# 集群检查周期，单位毫秒。可以自定义缩短时间。当某一个节点宕机的时候，其他节点等待多久后开始执行任务。
org.quartz.jobStore.clusterCheckinInterval = 5000
# 单位毫秒， 集群中的节点退出后，再次检查进入的时间间隔。
org.quartz.jobStore.misfireThreshold = 60000
# 事务隔离级别
org.quartz.jobStore.txIsolationLevelReadCommitted = true
# 存储的事务管理类型
org.quartz.jobStore.class = org.quartz.impl.jdbcjobstore.JobStoreTX
# 使用的Delegate类型
org.quartz.jobStore.driverDelegateClass = org.quartz.impl.jdbcjobstore.StdJDBCDelegate
# 集群的命名，一个集群要有相同的命名。
org.quartz.scheduler.instanceName = ClusterQuartz
# 节点的命名，可以自定义。AUTO代表自动生成。
org.quartz.scheduler.instanceId= AUTO
# rmi远程协议是否发布
org.quartz.scheduler.rmi.export = false
# rmi远程协议代理是否创建
org.quartz.scheduler.rmi.proxy = false
# 是否使用用户控制的事务环境触发执行job。
org.quartz.scheduler.wrapJobExecutionInUserTransaction = false
```
<a name="ghtpS"></a>
#### 4、初始化数据库
建表语句可以在官方网站中查找（Quartz-lib中），使用tables-mysql.sql建表。
<a name="oX62j"></a>
#### 5、定义JOB类
启动器和普通quartz无差异，但是JOB自身定义有些许差异：
```java
/**
 * 使用Spring提供的Quartz相关Job类型实现Job的定义。
 * 父类型QuartzJobBean中，提供了分布式环境中任务的配置定义。
 * 保证分布式环境中的任务是有效的。
 */
@PersistJobDataAfterExecution // 当job执行结束，持久化job信息到数据库
@DisallowConcurrentExecution // 保证job的唯一性（单例）
public class SpringBootQuartzJobDemo extends QuartzJobBean {
    @Override
    protected void executeInternal(JobExecutionContext context) throws JobExecutionException {
        System.out.println("SpringBootQuartzJobDemo : " + new Date());
    }
}
```
<a name="Pjcbh"></a>
#### 6、QuartzConfiguration类型定义
```java
@Configuration
public class QuartzConfiguration {
    @Autowired
    private DataSource dataSource;
    /**
     * 创建调度器， 可以省略的。
     * @return
     * @throws Exception
     */
    @Bean
    public Scheduler scheduler() throws Exception {
        Scheduler scheduler = schedulerFactoryBean().getScheduler();
        scheduler.start();
        return scheduler;
    }
    /**
     * 创建调度器工厂bean对象。
     * @return
     * @throws IOException
     */
    @Bean
    public SchedulerFactoryBean schedulerFactoryBean() throws IOException {
        SchedulerFactoryBean factory = new SchedulerFactoryBean();
        factory.setSchedulerName("Cluster_Scheduler");
        factory.setDataSource(dataSource);
        factory.setApplicationContextSchedulerContextKey("applicationContext");
        // 设置调度器中的线程池。
        factory.setTaskExecutor(schedulerThreadPool());
        // 设置触发器
        factory.setTriggers(trigger().getObject());
        // 设置quartz的配置信息
        factory.setQuartzProperties(quartzProperties());
        return factory;
    }
    /**
     * 读取quartz.properties配置文件的方法。
     * @return
     * @throws IOException
     */
    @Bean
    public Properties quartzProperties() throws IOException {
        PropertiesFactoryBean propertiesFactoryBean = new PropertiesFactoryBean();
        propertiesFactoryBean.setLocation(new ClassPathResource("/quartz.properties"));
        // 在quartz.properties中的属性被读取并注入后再初始化对象
        propertiesFactoryBean.afterPropertiesSet();
        return propertiesFactoryBean.getObject();
    }
    /**
     * 创建Job对象的方法。
     * @return
     */
    @Bean
    public JobDetailFactoryBean job() {
        JobDetailFactoryBean jobDetailFactoryBean = new JobDetailFactoryBean();
        jobDetailFactoryBean.setJobClass(SpringBootQuartzJobDemo.class);
        // 是否持久化job内容
        jobDetailFactoryBean.setDurability(true);
        // 设置是否多次请求尝试任务。
        jobDetailFactoryBean.setRequestsRecovery(true);
        return jobDetailFactoryBean;
    }
    /**
     * 创建trigger factory bean对象。
     * @return
     */
    @Bean
    public CronTriggerFactoryBean trigger() {
        CronTriggerFactoryBean cronTriggerFactoryBean = new CronTriggerFactoryBean();
        cronTriggerFactoryBean.setJobDetail(job().getObject());
        cronTriggerFactoryBean.setCronExpression("0/2 * * * * ?");
        return cronTriggerFactoryBean;
    }
    /**
     * 创建一个调度器的线程池。
     * @return
     */
    @Bean
    public Executor schedulerThreadPool() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(15);
        executor.setMaxPoolSize(25);
        executor.setQueueCapacity(100);
        return executor;
    }
}
```
若JOB任务有定义调用业务等内容，也需要重写JobFactory。
<a name="O7Vzt"></a>
## cron表达式详解
一个cron表达式有至少6个（也可能7个）有空格分隔的时间元素。按顺序依次为：<br />秒（0~59）<br />分钟（0~59）<br />小时（0~23）<br />天（0~31）<br />月（0~11）<br />星期（1~7 1=SUN 或 SUN，MON，TUE，WED，THU，FRI，SAT）<br />年份（1970－2099）<br />其中每个元素可以是一个值(如6),一个连续区间(9-12),一个间隔时间(8-18/4)(/表示每隔4小时),一个列表(1,3,5),通配符。由于”月份中的日期”和”星期中的日期”这两个元素互斥的,必须要对其中一个设置。配置实例：<br />`/5 * ?`：每隔5秒执行一次<br />`0 /1 ?`：每隔1分钟执行一次<br />`0 0 10,14,16 ?`：每天上午10点，下午2点，4点<br />`0 0/30 9-17 ?`：朝九晚五工作时间内每半小时<br />`0 0 12 ? * WED`：表示每个星期三中午12点<br />`0 0 12 ?`：每天中午12点触发<br />`0 15 10 ?`：每天上午10:15触发<br />`0 15 10 ?`：每天上午10:15触发<br />`0 15 10 ? *`：每天上午10:15触发<br />`0 15 10 ? 2005`：2005年的每天上午10:15触发<br />`0 14 * ?`：在每天下午2点到下午2:59期间的每1分钟触发<br />`0 0/5 14 ?`：在每天下午2点到下午2:55期间的每5分钟触发<br />`0 0/5 14,18 ?`：在每天下午2点到2:55期间和下午6点到6:55期间的每5分钟触发<br />`0 0-5 14 ?`：在每天下午2点到下午2:05期间的每1分钟触发<br />`0 10,44 14 ? 3 WED`：每年三月的星期三的下午2:10和2:44触发<br />`0 15 10 ? * MON-FRI`：周一至周五的上午10:15触发<br />`0 15 10 15 * ?`：每月15日上午10:15触发<br />`0 15 10 L * ?`：每月最后一日的上午10:15触发<br />`0 15 10 ? * 6L`：每月的最后一个星期五上午10:15触发<br />`0 15 10 ? * 6L 2002-2005`：2002年至2005年的每月的最后一个星期五上午10:15触发<br />`0 15 10 ? * 6#3`：每月的第三个星期五上午10:15触发<br />有些子表达式能包含一些范围或列表，
> 例如：子表达式（天（星期））可以为 “MON-FRI”，“MON，WED，FRI”，“MON-WED,SAT”

"*"字符代表所有可能的值<br />"/"字符用来指定数值的增量
> 例如：在子表达式（分钟）里的“0/15”表示从第0分钟开始，每15分钟
> 在子表达式（分钟）里的“3/20”表示从第3分钟开始，每20分钟（它和“3，23，43”）的含义一样

"？"字符仅被用于天（月）和天（星期）两个子表达式，表示不指定值<br />当2个子表达式其中之一被指定了值以后，为了避免冲突，需要将另一个子表达式的值设为"？"<br />"L"字符仅被用于天（月）和天（星期）两个子表达式，它是单词"last"的缩写<br />如果在"L"前有具体的内容，它就具有其他的含义了。
> 例如：“6L”表示这个月的倒数第６天
> 注意：在使用“L”参数时，不要指定列表或范围，因为这会导致问题

W 字符代表着平日(Mon-Fri)，并且仅能用于日域中。它用来指定离指定日的最近的一个平日。大部分的商业处理都是基于工作周的，所以 W 字符可能是非常重要的。
> 例如，日域中的 15W 意味着 “离该月15号的最近一个平日。” 假如15号是星期六，那么 trigger 会在14号(星期五)触发，因为星期四比星期一离15号更近。

C：代表“Calendar”的意思。它的意思是计划所关联的日期，如果日期没有被关联，则相当于日历中所有日期。
> 例如5C在日期字段中就相当于日历5日以后的第一天。1C在星期字段中相当于星期日后的第一天。

| 字段 | 允许值 | 允许的特殊字符 |
| --- | --- | --- |
| 秒 | 0~59 | , - * / |
| 分 | 0~59 | , - * / |
| 小时 | 0~23 | , - * / |
| 日期 | 1-31 | , - * ? / L W C |
| 月份 | 1~12或者JAN~DEC | , - * / |
| 星期 | 1~7或者SUN~SAT | , - * ? / L C # |
| 年（可选） | 留空，1970~2099 | , - * / |

在线cron表达式生成：[http://qqe2.com/cron/index](http://qqe2.com/cron/index)
:::tips
**Cron表达式对特殊字符的大小写不敏感，对代表星期的缩写英文大小写也不敏感。**
:::
