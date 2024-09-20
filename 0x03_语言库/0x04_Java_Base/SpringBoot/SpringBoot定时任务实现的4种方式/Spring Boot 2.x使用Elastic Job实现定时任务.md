Java SpringBoot Elastic Job<br />使用Spring Boot自带的`@Scheduled`注解实现定时任务当在集群环境下的时候，如果任务的执行或操作依赖一些共享资源的话，就会存在竞争关系。如果不引入分布式锁等机制来做调度的话，就可能出现预料之外的执行结果。所以，`@Scheduled`注解更偏向于使用在单实例自身维护相关的一些定时任务上会更为合理一些，比如：定时清理服务实例某个目录下的文件、定时上传本实例的一些统计数据等。<br />那么，在实际实现业务逻辑的时候，没有更好的定时任务方案呢？来看一个老牌的分布式定时任务框架，在Spring Boot下的使用案例。
<a name="sIFN9"></a>
## Elastic Job
Elastic Job的前生是当当开源的一款分布式任务调度框架，而目前已经加入到了Apache基金会。<br />该项目下有两个分支：ElasticJob-Lite和ElasticJob-Cloud。ElasticJob-Lite是一个轻量级的任务管理方案，本文接下来的案例就用这个来实现。而 ElasticJob-Cloud则相对重一些，因为它使用容器来管理任务和隔离资源。<br />更多关于ElasticJob的介绍，也可以点击这里直达官方网站([https://shardingsphere.apache.org/elasticjob/](https://shardingsphere.apache.org/elasticjob/))了解更多信息。
<a name="moQxS"></a>
## 操作实例
<a name="BeiiX"></a>
### 第一步：创建一个最基础的Spring Boot项目
<a name="bUCA6"></a>
### 第二步：pom.xml中添加elasticjob-lite的starter
```xml
<dependencies>
    <dependency>
        <groupId>org.apache.shardingsphere.elasticjob</groupId>
        <artifactId>elasticjob-lite-spring-boot-starter</artifactId>
        <version>3.0.0</version>
    </dependency>

    // ...
</dependencies>
```
<a name="sOJ8J"></a>
### 第三步：创建一个简单任务
```java
@Slf4j
@Service
public class MySimpleJob implements SimpleJob {

    @Override
    public void execute(ShardingContext context) {
        log.info("MySimpleJob start : didispace.com {}", System.currentTimeMillis());
    }

}
```
<a name="IhIWM"></a>
### 第四步：编辑配置文件
```
elasticjob.reg-center.server-lists=localhost:2181
elasticjob.reg-center.namespace=didispace

elasticjob.jobs.my-simple-job.elastic-job-class=com.didispace.chapter72.MySimpleJob
elasticjob.jobs.my-simple-job.cron=0/5 * * * * ?
elasticjob.jobs.my-simple-job.sharding-total-count=1
```
这里主要有两个部分：<br />第一部分：`elasticjob.reg-center`开头的，主要配置elastic job的注册中心和namespace<br />第二部分：任务配置，以`elasticjob.jobs`开头，这里的`my-simple-job`是任务的名称，根据喜好命名即可，但不要重复。任务的下的配置`elastic-job-class`是任务的实现类，`cron`是执行规则表达式，`sharding-total-count`是任务分片的总数。可以通过这个参数来把任务切分，实现并行处理。这里先设置为1，后面讲分片的使用。
<a name="DW73E"></a>
## 运行与测试
完成了上面所有操作时候，可以尝试运行一下上面应用，因为这里需要用到ZooKeeper来协调分布式环境下的任务调度。所以，需要先在本地安装ZooKeeper，然后启动它。注意：上面`elasticjob.reg-center.server-lists`配置，根据实际使用的ZooKeeper地址和端口做相应修改。<br />在启动上述Spring Boot应用之后，可以看到如下日志输出：
```
2021-07-20 15:33:39.541  INFO 56365 --- [           main] org.quartz.impl.StdSchedulerFactory      : Quartz scheduler 'my-simple-job' initialized from an externally provided properties instance.
2021-07-20 15:33:39.541  INFO 56365 --- [           main] org.quartz.impl.StdSchedulerFactory      : Quartz scheduler version: 2.3.2
2021-07-20 15:33:39.551  INFO 56365 --- [           main] org.apache.curator.utils.Compatibility   : Using org.apache.zookeeper.server.quorum.MultipleAddresses
2021-07-20 15:33:40.067  INFO 56365 --- [           main] c.d.chapter72.Chapter72Application       : Started Chapter72Application in 3.25 seconds (JVM running for 4.965)
2021-07-20 15:33:40.069  INFO 56365 --- [           main] .s.b.j.ScheduleJobBootstrapStartupRunner : Starting ElasticJob Bootstrap.
2021-07-20 15:33:40.078  INFO 56365 --- [           main] org.quartz.core.QuartzScheduler          : Scheduler my-simple-job_$_NON_CLUSTERED started.
2021-07-20 15:33:40.078  INFO 56365 --- [           main] .s.b.j.ScheduleJobBootstrapStartupRunner : ElasticJob Bootstrap started.
2021-07-20 15:33:45.157  INFO 56365 --- [le-job_Worker-1] com.didispace.chapter72.MySimpleJob      : MySimpleJob start : didispace.com 1626766425157
2021-07-20 15:33:50.010  INFO 56365 --- [le-job_Worker-1] com.didispace.chapter72.MySimpleJob      : MySimpleJob start : didispace.com 1626766430010
2021-07-20 15:33:55.013  INFO 56365 --- [le-job_Worker-1] com.didispace.chapter72.MySimpleJob      : MySimpleJob start : didispace.com 1626766435013
```
既然是分布式任务调度，那么再启动一个（注意，在同一台机器启动的时候，会端口冲突，可以在启动命令中加入`-Dserver.port=8081`来区分端口），在第二个启动的服务日志也打印了类似的内容
```
2021-07-20 15:34:06.430  INFO 56371 --- [           main] org.quartz.impl.StdSchedulerFactory      : Quartz scheduler 'my-simple-job' initialized from an externally provided properties instance.
2021-07-20 15:34:06.430  INFO 56371 --- [           main] org.quartz.impl.StdSchedulerFactory      : Quartz scheduler version: 2.3.2
2021-07-20 15:34:06.436  INFO 56371 --- [           main] org.apache.curator.utils.Compatibility   : Using org.apache.zookeeper.server.quorum.MultipleAddresses
2021-07-20 15:34:06.786  INFO 56371 --- [           main] c.d.chapter72.Chapter72Application       : Started Chapter72Application in 1.446 seconds (JVM running for 1.884)
2021-07-20 15:34:06.787  INFO 56371 --- [           main] .s.b.j.ScheduleJobBootstrapStartupRunner : Starting ElasticJob Bootstrap.
2021-07-20 15:34:06.792  INFO 56371 --- [           main] org.quartz.core.QuartzScheduler          : Scheduler my-simple-job_$_NON_CLUSTERED started.
2021-07-20 15:34:06.792  INFO 56371 --- [           main] .s.b.j.ScheduleJobBootstrapStartupRunner : ElasticJob Bootstrap started.
2021-07-20 15:34:10.182  INFO 56371 --- [le-job_Worker-1] com.didispace.chapter72.MySimpleJob      : MySimpleJob start : didispace.com 1626766450182
2021-07-20 15:34:15.010  INFO 56371 --- [le-job_Worker-1] com.didispace.chapter72.MySimpleJob      : MySimpleJob start : didispace.com 1626766455010
2021-07-20 15:34:20.013  INFO 56371 --- [le-job_Worker-1] com.didispace.chapter72.MySimpleJob      : MySimpleJob start : didispace.com 1626766460013
```
此时，在回头看看之前第一个启动的应用，日志输出停止了。由于设置了分片总数为1，所以这个任务启动之后，只会有一个实例接管执行。这样就避免了多个进行同时重复的执行相同逻辑而产生问题的情况。同时，这样也支持了任务执行的高可用。比如：可以尝试把第二个启动的应用（正在打印日志的）终止掉。可以发现，第一个启动的应用（之前已经停止输出日志）继续开始打印任务日志了。<br />在整个实现过程中，并没有自己手工的去编写任何的分布式锁等代码去实现任务调度逻辑，只需要关注任务逻辑本身，然后通过配置分片的方式来控制任务的分割，就可以轻松的实现分布式集群环境下的定时任务管理了。是不是在复杂场景下，这种方式实现起来要比`@Scheduled`更方便呢？
<a name="l9NIx"></a>
## 使用Elastic Job的namespace配置，防止任务名称的冲突
<a name="A9CLw"></a>
### 报错
```java
Caused by: org.apache.shardingsphere.elasticjob.infra.exception.JobConfigurationException: Job conflict with register center. The job 'my-simple-job' in register center's class is 'com.didispace.chapter72.MySimpleJob', your job class is 'com.didispace.chapter74.MySimpleJob'
 at org.apache.shardingsphere.elasticjob.lite.internal.config.ConfigurationService.checkConflictJob(ConfigurationService.java:86) ~[elasticjob-lite-core-3.0.0.jar:3.0.0]
 at org.apache.shardingsphere.elasticjob.lite.internal.config.ConfigurationService.setUpJobConfiguration(ConfigurationService.java:70) ~[elasticjob-lite-core-3.0.0.jar:3.0.0]
 at org.apache.shardingsphere.elasticjob.lite.internal.setup.SetUpFacade.setUpJobConfiguration(SetUpFacade.java:66) ~[elasticjob-lite-core-3.0.0.jar:3.0.0]
 at org.apache.shardingsphere.elasticjob.lite.internal.schedule.JobScheduler.<init>(JobScheduler.java:84) ~[elasticjob-lite-core-3.0.0.jar:3.0.0]
 at org.apache.shardingsphere.elasticjob.lite.api.bootstrap.impl.ScheduleJobBootstrap.<init>(ScheduleJobBootstrap.java:36) ~[elasticjob-lite-core-3.0.0.jar:3.0.0]
 at org.apache.shardingsphere.elasticjob.lite.spring.boot.job.ElasticJobBootstrapConfiguration.registerClassedJob(ElasticJobBootstrapConfiguration.java:101) ~[elasticjob-lite-spring-boot-starter-3.0.0.jar:3.0.0]
 at org.apache.shardingsphere.elasticjob.lite.spring.boot.job.ElasticJobBootstrapConfiguration.constructJobBootstraps(ElasticJobBootstrapConfiguration.java:84) ~[elasticjob-lite-spring-boot-starter-3.0.0.jar:3.0.0]
 at org.apache.shardingsphere.elasticjob.lite.spring.boot.job.ElasticJobBootstrapConfiguration.createJobBootstrapBeans(ElasticJobBootstrapConfiguration.java:57) ~[elasticjob-lite-spring-boot-starter-3.0.0.jar:3.0.0]
 at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_151]
 at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_151]
 at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_151]
 at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_151]
 at org.springframework.beans.factory.annotation.InitDestroyAnnotationBeanPostProcessor$LifecycleElement.invoke(InitDestroyAnnotationBeanPostProcessor.java:389) ~[spring-beans-5.3.8.jar:5.3.8]
 at org.springframework.beans.factory.annotation.InitDestroyAnnotationBeanPostProcessor$LifecycleMetadata.invokeInitMethods(InitDestroyAnnotationBeanPostProcessor.java:333) ~[spring-beans-5.3.8.jar:5.3.8]
 at org.springframework.beans.factory.annotation.InitDestroyAnnotationBeanPostProcessor.postProcessBeforeInitialization(InitDestroyAnnotationBeanPostProcessor.java:157) ~[spring-beans-5.3.8.jar:5.3.8]
 ... 17 common frames omitted
```
根据错误消息Job conflict with register center. The job 'my-simple-job' in register center's，初步判断是ZooKeeper中存储的任务配置出现冲突：任务名一样，但实现类不同。<br />如果在一个大一些的团队做开发的时候，只要存在多系统的话，那么定时任务的重名其实是很有可能发生。比如：很多应用都可能存在一些定时清理某些资源的任务，就很可能起一样的名字，然后注册到同一个ZooKeeper，最后出现冲突。那么有什么好办法来解决这个问题吗？
<a name="V7Lr6"></a>
### 方法一：任务创建的统一管理
最原始的处理方法，就是集中的管理任务创建流程，比如：可以开一个Wiki页面，所有任务在这个页面上登记，每个人登记的时候，可以查一下想起的名字是否已经存在。如果存在了就再想一个名字，并做好登记。<br />这种方法很简单，也很好理解。但存在的问题是，当任务非常非常多的时候，这个页面内容就很大，维护起来也是非常麻烦的。
<a name="pkqPl"></a>
### 方法二：巧用Elastic Job的namespace属性来隔离任务名称
回忆一下之前第一篇写定时任务的时候，关于注册中心的配置是不是有下面两项：
```
elasticjob.reg-center.server-lists=localhost:2181
elasticjob.reg-center.namespace=didispace
```
第一个`elasticjob.reg-center.server-lists`不多说，就是ZooKeeper的访问地址。这里要重点讲的就是第二个参数`elasticjob.reg-center.namespace`。<br />其实在ZooKeeper中注册任务的时候，真正冲突的并不纯粹是因为任务名称，而是namespace + 任务名称，全部一样，才会出现问题。所以，是不是引入把每个应用创建的任务都设定一个独立的namespace，那么是不是就隔离了呢？<br />继续思考一下，每个应用是不是肯定有一些属性是肯定不一样的呢？最后，给出了下面这样的建议：
```
spring.application.name=chapter74

elasticjob.reg-center.server-lists=localhost:2181
elasticjob.reg-center.namespace=${spring.application.name}
```
即：将定时任务服务的`elasticjob.reg-center.namespace`设置为当前Spring Boot应用的名称一致`spring.application.name`。<br />通常，在规划各个Spring Boot应用的时候，都会做好唯一性的规划，这样未来注册到Eureka、Nacos等注册中心的时候，也可以保证唯一。利用好这个唯一参数，也可以方便把各个应用的定时任务也都隔离出来。
<a name="I7G4b"></a>
## 使用Elastic Job的分片配置
然而，还有一类问题是在做定时任务时候容易出现的，就是任务执行速度时间过长；同时，为了实现定时任务的高可用，还启动了很多任务实例，但每个任务执行时候就一个实例在跑，资源利用率不高。<br />所以，接下来就来继续介绍，使用Elastic Job的分片配置，来为任务执行加加速，资源利用抬抬高的目标！
<a name="YvJk4"></a>
### 操作实践
<a name="pYGsg"></a>
#### 第一步：创建一个分片执行的任务
```java
@Slf4j
@Service
public class MyShardingJob implements SimpleJob {

    @Override
    public void execute(ShardingContext context) {
        switch (context.getShardingItem()) {
            case 0:
                log.info("分片1：执行任务");
                break;
            case 1:
                log.info("分片2：执行任务");
                break;
            case 2:
                log.info("分片3：执行任务");
                break;
        }
    }

}
```
这里通过`switch`来判断当前任务上下文的sharding-item值来执行不同的分片任务。sharding-item的值取决于后面将要配置的分片总数，但注意是从0开始计数的。这里仅采用了日志打印的方式，来展示分片效果，真正实现业务逻辑的时候，一定记得根据分片数量对执行任务也要做分片操作的设计。比如：可以根据批量任务的id求模的方式来区分不同分片处理不同的数据，以避免又重复执行而出现问题。
<a name="EiOfj"></a>
#### 第二步：在配置文件中，设置配置任务的实现类、执行表达式、以及将要重要测试的分片总数参数
```
elasticjob.jobs.my-sharding-job.elastic-job-class=com.didispace.chapter73.MyShardingJob
elasticjob.jobs.my-sharding-job.cron=0/5 * * * * ?
elasticjob.jobs.my-sharding-job.sharding-total-count=3
```
这里设置为3，所以任务会被分为3个分片，每个分片对应第一步中一个switch的分支。
<a name="ehE0N"></a>
### 运行与测试
<a name="jRIQZ"></a>
#### 单实例运行
在完成了上面的代码之后，尝试启动上面实现的第一个实例。<br />此时，可以看到，每间隔5秒，这个实例会打印这样的日志：
```
2021-07-21 17:42:00.122  INFO 63478 --- [           main] .s.b.j.ScheduleJobBootstrapStartupRunner : Starting ElasticJob Bootstrap.
2021-07-21 17:42:00.126  INFO 63478 --- [           main] org.quartz.core.QuartzScheduler          : Scheduler my-sharding-job_$_NON_CLUSTERED started.
2021-07-21 17:42:00.126  INFO 63478 --- [           main] .s.b.j.ScheduleJobBootstrapStartupRunner : ElasticJob Bootstrap started.
2021-07-21 17:42:05.254  INFO 63478 --- [-sharding-job-1] com.didispace.chapter73.MyShardingJob    : 分片1：执行任务
2021-07-21 17:42:05.254  INFO 63478 --- [-sharding-job-3] com.didispace.chapter73.MyShardingJob    : 分片3：执行任务
2021-07-21 17:42:05.254  INFO 63478 --- [-sharding-job-2] com.didispace.chapter73.MyShardingJob    : 分片2：执行任务
2021-07-21 17:42:10.011  INFO 63478 --- [-sharding-job-4] com.didispace.chapter73.MyShardingJob    : 分片1：执行任务
2021-07-21 17:42:10.011  INFO 63478 --- [-sharding-job-5] com.didispace.chapter73.MyShardingJob    : 分片2：执行任务
2021-07-21 17:42:10.011  INFO 63478 --- [-sharding-job-6] com.didispace.chapter73.MyShardingJob    : 分片3：执行任务
```
每次任务都被拆分成了3个分片任务，就如上文中所说的，每个分片对应一个`switch`的分支。由于当前情况下，只启动了一个实例，所以3个分片任务都被分配到了这个唯一的实例上。
<a name="Dt08j"></a>
#### 双实例运行
接下来，再启动一个实例（注意使用-Dserver.port来改变不同的端口，不然本地会启动不成功）。此时，两个实例的日志出现了变化：<br />实例1的日志：
```java
2021-07-21 17:44:50.190  INFO 63478 --- [ng-job_Worker-1] com.didispace.chapter73.MyShardingJob    : 分片2：执行任务
2021-07-21 17:44:55.007  INFO 63478 --- [ng-job_Worker-1] com.didispace.chapter73.MyShardingJob    : 分片2：执行任务
2021-07-21 17:45:00.010  INFO 63478 --- [ng-job_Worker-1] com.didispace.chapter73.MyShardingJob    : 分片2：执行任务
```
实例2的日志：
```java
2021-07-21 17:44:50.272  INFO 63484 --- [-sharding-job-1] com.didispace.chapter73.MyShardingJob    : 分片1：执行任务
2021-07-21 17:44:50.273  INFO 63484 --- [-sharding-job-2] com.didispace.chapter73.MyShardingJob    : 分片3：执行任务
2021-07-21 17:44:55.009  INFO 63484 --- [-sharding-job-3] com.didispace.chapter73.MyShardingJob    : 分片1：执行任务
2021-07-21 17:44:55.009  INFO 63484 --- [-sharding-job-4] com.didispace.chapter73.MyShardingJob    : 分片3：执行任务
```
随着实例数量的增加，可以看到分片的分配发生了变化。这也就意味着，当一个任务开始执行的时候，两个任务执行实例都被利用了起来，这样任务执行和资源利用的效率就可以得到优化。
