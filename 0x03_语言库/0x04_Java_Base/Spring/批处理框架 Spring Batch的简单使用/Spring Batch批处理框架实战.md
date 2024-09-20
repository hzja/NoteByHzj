Java Spring Spring Batch
<a name="hRo1g"></a>
## 项目搭建
新建一个 Spring Boot 项目，版本为 2.2.4.RELEASE，artifactId 为 spring-batch-start。<br />然后在 pom.xml 中引入 Spring Batch、MySQL 和 JDBC 依赖，引入后 pom.xml 内容如下所示：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.2.5.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>cc.mrbird</groupId>
    <artifactId>spring-batch-start</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>spring-batch-start</name>
    <description>Demo project for Spring Boot</description>

    <properties>
        <java.version>1.8</java.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-batch</artifactId>
        </dependency>

        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-jdbc</artifactId>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
</project>
```
在编写代码之前，先来简单了解下 Spring Batch 的组成：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633576384127-1d6a2d0f-b489-4eca-86f9-698329367b24.webp#clientId=u6cf63333-9b45-4&from=paste&id=ue37a18cc&originHeight=418&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u3920f224-5f65-4a1d-8757-f0c1f500ecd)

- Spring Batch 里最基本的单元就是任务 Job，一个 Job 由若干个步骤 Step 组成。
- 任务启动器 Job Launcher 负责运行 Job。
- 任务存储仓库 Job Repository 存储着 Job 的执行状态，参数和日志等信息。Job 处理任务又可以分为三大类：
   - 数据读取 Item Reader
   - 数据中间处理 Item Processor
   - 数据输出 Item Writer。

任务存储仓库可以是关系型数据库 MySQL，非关系型数据库 MongoDB 或者直接存储在内存中，本篇使用的是 MySQL 作为任务存储仓库。<br />新建一个名称为 springbatch 的 MySQL 数据库，然后导入 org.springframework.batch.core 目录下的 schema-mysql.sql 文件：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633576384021-b4dc545c-6be7-4e06-89e7-870c477810d2.webp#clientId=u6cf63333-9b45-4&from=paste&id=u28a49e50&originHeight=159&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u94c3336c-b892-4620-a8a8-ed6462e03c4)![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633576383941-67e03700-2645-4651-b95c-6fe9f1db94b3.webp#clientId=u6cf63333-9b45-4&from=paste&id=uf9949a9d&originHeight=512&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u2c987265-4b5b-48db-ab8f-7a03d8179b1)<br />导入后，库表如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633576383996-179b4cf9-80b9-4927-80d1-d18edc13863e.webp#clientId=u6cf63333-9b45-4&from=paste&id=u38ede029&originHeight=242&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u6a97c98c-c684-4855-a187-5bf991327ee)<br />然后在项目的配置文件 application.yml 里添加 MySQL 相关配置：
```yaml
spring:
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://127.0.0.1:3306/springbatch
    username: root
    password: 123456
```
接着在 Spring Boot 的入口类上添加 `@EnableBatchProcessing` 注解，表示开启 Spring Batch 批处理功能：
```java
@SpringBootApplication
@EnableBatchProcessing
public class SpringBatchStartApplication {
    public static void main(String[] args) {
        SpringApplication.run(SpringBatchStartApplication.class, args);
    }
}
```
至此，基本框架搭建好了，下面开始配置一个简单的任务。
<a name="OUzrl"></a>
## 编写第一个任务
在 cc.mrbird.batch 目录下新建 job 包，然后在该包下新建一个 FirstJobDemo 类，代码如下所示：
```java
@Component
public class FirstJobDemo {

    @Autowired
    private JobBuilderFactory jobBuilderFactory;
    @Autowired
    private StepBuilderFactory stepBuilderFactory;

    @Bean
    public Job firstJob() {
        return jobBuilderFactory.get("firstJob")
                .start(step())
                .build();
    }

    private Step step() {
        return stepBuilderFactory.get("step")
                .tasklet((contribution, chunkContext) -> {
                    System.out.println("执行步骤....");
                    return RepeatStatus.FINISHED;
                }).build();
    }
}
```
上面代码中，注入了`JobBuilderFactory`任务创建工厂和`StepBuilderFactory`步骤创建工厂，分别用于创建任务 Job 和步骤 Step。`JobBuilderFactory`的`get`方法用于创建一个指定名称的任务，`start`方法指定任务的开始步骤，步骤通过`StepBuilderFactory`构建。<br />步骤 Step 由若干个小任务 Tasklet 组成，所以通过`tasklet`方法创建。`tasklet`方法接收一个`Tasklet`类型参数，`Tasklet`是一个函数是接口，源码如下：
```java
public interface Tasklet {
    @Nullable
    RepeatStatus execute(StepContribution contribution, ChunkContext chunkContext) throws Exception;
}
```
所以可以使用 lambda 表达式创建一个匿名实现：
```java
(contribution, chunkContext) -> {
    System.out.println("执行步骤....");
    return RepeatStatus.FINISHED;
}
```
该匿名实现必须返回一个明确的执行状态，这里返回`RepeatStatus.FINISHED`表示该小任务执行成功，正常结束。<br />此外，需要注意的是，配置的任务 Job 必须注册到 Spring IOC 容器中，并且任务的名称和步骤的名称组成唯一。比如上面的例子，任务名称为 firstJob，步骤的名称为 step，如果存在别的任务和步骤组合也叫这个名称的话，则会执行失败。<br />启动项目，控制台打印日志如下：
```java
...
2020-03-06 11:01:11.785  INFO 17324 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=firstJob]] launched with the following parameters: [{}]
2020-03-06 11:01:11.846  INFO 17324 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step]
执行步骤....
2020-03-06 11:01:11.886  INFO 17324 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step] executed in 40ms
2020-03-06 11:01:11.909  INFO 17324 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=firstJob]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 101ms
```
可以看到，任务成功执行了，数据库的库表也将记录相关运行日志。<br />重新启动项目，控制台并不会再次打印出任务执行日志，因为 Job 名称和 Step 名称组成唯一，执行完的不可重复的任务，不会再次执行。
<a name="BlRAm"></a>
## 多步骤任务
一个复杂的任务一般包含多个步骤，下面举个多步骤任务的例子。在 job 包下新建MultiStepJobDemo类：
```java
@Component
public class MultiStepJobDemo {

    @Autowired
    private JobBuilderFactory jobBuilderFactory;
    @Autowired
    private StepBuilderFactory stepBuilderFactory;

    @Bean
    public Job multiStepJob() {
        return jobBuilderFactory.get("multiStepJob")
                .start(step1())
                .next(step2())
                .next(step3())
                .build();
    }

    private Step step1() {
        return stepBuilderFactory.get("step1")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤一操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Step step2() {
        return stepBuilderFactory.get("step2")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤二操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Step step3() {
        return stepBuilderFactory.get("step3")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤三操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }
}
```
上面代码中，通过`step1()`、`step2()`和`step3()`三个方法创建了三个步骤。Job 里要使用这些步骤，只需要通过`JobBuilderFactory`的`start`方法指定第一个步骤，然后通过`next`方法不断地指定下一个步骤即可。<br />启动项目，控制台打印日志如下：
```java
2020-03-06 13:52:52.188  INFO 18472 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=multiStepJob]] launched with the following parameters: [{}]
2020-03-06 13:52:52.222  INFO 18472 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step1]
执行步骤一操作。。。
2020-03-06 13:52:52.251  INFO 18472 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step1] executed in 29ms
2020-03-06 13:52:52.292  INFO 18472 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step2]
执行步骤二操作。。。
2020-03-06 13:52:52.323  INFO 18472 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step2] executed in 30ms
2020-03-06 13:52:52.375  INFO 18472 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step3]
执行步骤三操作。。。
2020-03-06 13:52:52.405  INFO 18472 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step3] executed in 29ms
2020-03-06 13:52:52.428  INFO 18472 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=multiStepJob]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 231ms
```
三个步骤依次执行成功。<br />多个步骤在执行过程中也可以通过上一个步骤的执行状态来决定是否执行下一个步骤，修改上面的代码：
```java
@Component
public class MultiStepJobDemo {

    @Autowired
    private JobBuilderFactory jobBuilderFactory;
    @Autowired
    private StepBuilderFactory stepBuilderFactory;

    @Bean
    public Job multiStepJob() {
        return jobBuilderFactory.get("multiStepJob2")
                .start(step1())
                .on(ExitStatus.COMPLETED.getExitCode()).to(step2())
                .from(step2())
                .on(ExitStatus.COMPLETED.getExitCode()).to(step3())
                .from(step3()).end()
                .build();
    }

    private Step step1() {
        return stepBuilderFactory.get("step1")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤一操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Step step2() {
        return stepBuilderFactory.get("step2")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤二操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Step step3() {
        return stepBuilderFactory.get("step3")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤三操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }
}
```
`multiStepJob()`方法的含义是：multiStepJob2 任务先执行 step1，当 step1 状态为完成时，接着执行 step2，当 step2 的状态为完成时，接着执行 step3。`ExitStatus.COMPLETED`常量表示任务顺利执行完毕，正常退出，该类还包含以下几种退出状态：
```java
public class ExitStatus implements Serializable, Comparable<ExitStatus> {

    /**
     * Convenient constant value representing unknown state - assumed not
     * continuable.
     */
    public static final ExitStatus UNKNOWN = new ExitStatus("UNKNOWN");

    /**
     * Convenient constant value representing continuable state where processing
     * is still taking place, so no further action is required. Used for
     * asynchronous execution scenarios where the processing is happening in
     * another thread or process and the caller is not required to wait for the
     * result.
     */
    public static final ExitStatus EXECUTING = new ExitStatus("EXECUTING");

    /**
     * Convenient constant value representing finished processing.
     */
    public static final ExitStatus COMPLETED = new ExitStatus("COMPLETED");

    /**
     * Convenient constant value representing job that did no processing (e.g.
     * because it was already complete).
     */
    public static final ExitStatus NOOP = new ExitStatus("NOOP");

    /**
     * Convenient constant value representing finished processing with an error.
     */
    public static final ExitStatus FAILED = new ExitStatus("FAILED");

    /**
     * Convenient constant value representing finished processing with
     * interrupted status.
     */
    public static final ExitStatus STOPPED = new ExitStatus("STOPPED");

    ...
}
```
启动项目，控制台日志打印如下：
```java
2020-03-06 14:21:49.384  INFO 18745 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [FlowJob: [name=multiStepJob2]] launched with the following parameters: [{}]
2020-03-06 14:21:49.427  INFO 18745 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step1]
执行步骤一操作。。。
2020-03-06 14:21:49.456  INFO 18745 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step1] executed in 29ms
2020-03-06 14:21:49.501  INFO 18745 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step2]
执行步骤二操作。。。
2020-03-06 14:21:49.527  INFO 18745 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step2] executed in 26ms
2020-03-06 14:21:49.576  INFO 18745 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step3]
执行步骤三操作。。。
2020-03-06 14:21:49.604  INFO 18745 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step3] executed in 28ms
2020-03-06 14:21:49.629  INFO 18745 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [FlowJob: [name=multiStepJob2]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 238ms
```
<a name="iXfTM"></a>
## Flow 的用法
Flow 的作用就是可以将多个步骤 Step 组合在一起然后再组装到任务 Job 中。举个 Flow 的例子，在 job 包下新建FlowJobDemo类：
```java
@Component
public class FlowJobDemo {

    @Autowired
    private JobBuilderFactory jobBuilderFactory;
    @Autowired
    private StepBuilderFactory stepBuilderFactory;

    @Bean
    public Job flowJob() {
        return jobBuilderFactory.get("flowJob")
                .start(flow())
                .next(step3())
                .end()
                .build();
    }

    private Step step1() {
        return stepBuilderFactory.get("step1")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤一操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Step step2() {
        return stepBuilderFactory.get("step2")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤二操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Step step3() {
        return stepBuilderFactory.get("step3")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤三操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    // 创建一个flow对象，包含若干个step
    private Flow flow() {
        return new FlowBuilder<Flow>("flow")
                .start(step1())
                .next(step2())
                .build();
    }
}
```
上面代码中，通过`FlowBuilder`将 step1 和 step2 组合在一起，创建了一个名为 flow 的 Flow，然后再将其赋给任务 Job。使用 Flow 和 Step 构建 Job 的区别是，Job 流程中包含 Flow 类型的时候需要在`build()`方法前调用`end()`方法。<br />启动程序，控制台日志打印如下：
```java
2020-03-06 14:36:42.621  INFO 18865 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [FlowJob: [name=flowJob]] launched with the following parameters: [{}]
2020-03-06 14:36:42.667  INFO 18865 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step1]
执行步骤一操作。。。
2020-03-06 14:36:42.697  INFO 18865 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step1] executed in 30ms
2020-03-06 14:36:42.744  INFO 18865 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step2]
执行步骤二操作。。。
2020-03-06 14:36:42.771  INFO 18865 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step2] executed in 27ms
2020-03-06 14:36:42.824  INFO 18865 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step3]
执行步骤三操作。。。
2020-03-06 14:36:42.850  INFO 18865 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step3] executed in 25ms
2020-03-06 14:36:42.874  INFO 18865 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [FlowJob: [name=flowJob]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 245ms
```
<a name="ObUSd"></a>
## 并行执行
任务中的步骤除了可以串行执行（一个接着一个执行）外，还可以并行执行，并行执行在特定的业务需求下可以提供任务执行效率。<br />将任务并行化只需两个简单步骤：

1. 将步骤 Step 转换为 Flow；
2. 任务 Job 中指定并行 Flow。

举个例子，在 job 包下新建SplitJobDemo类：
```java
@Component
public class SplitJobDemo {

    @Autowired
    private JobBuilderFactory jobBuilderFactory;
    @Autowired
    private StepBuilderFactory stepBuilderFactory;

    @Bean
    public Job splitJob() {
        return jobBuilderFactory.get("splitJob")
                .start(flow1())
                .split(new SimpleAsyncTaskExecutor()).add(flow2())
                .end()
                .build();

    }

    private Step step1() {
        return stepBuilderFactory.get("step1")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤一操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Step step2() {
        return stepBuilderFactory.get("step2")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤二操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Step step3() {
        return stepBuilderFactory.get("step3")
                .tasklet((stepContribution, chunkContext) -> {
                    System.out.println("执行步骤三操作。。。");
                    return RepeatStatus.FINISHED;
                }).build();
    }

    private Flow flow1() {
        return new FlowBuilder<Flow>("flow1")
                .start(step1())
                .next(step2())
                .build();
    }

    private Flow flow2() {
        return new FlowBuilder<Flow>("flow2")
                .start(step3())
                .build();
    }
}
```
上面例子中，创建了两个 Flow：flow1（包含 step1 和 step2）和 flow2（包含 step3）。然后通过`JobBuilderFactory`的`split`方法，指定一个异步执行器，将 flow1 和 flow2 异步执行（也就是并行）。<br />启动项目，控制台日志打印如下：
```java
2020-03-06 15:25:43.602  INFO 19449 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [FlowJob: [name=splitJob]] launched with the following parameters: [{}]
2020-03-06 15:25:43.643  INFO 19449 --- [cTaskExecutor-1] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step3]
2020-03-06 15:25:43.650  INFO 19449 --- [cTaskExecutor-2] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step1]
执行步骤三操作。。。
执行步骤一操作。。。
2020-03-06 15:25:43.673  INFO 19449 --- [cTaskExecutor-2] o.s.batch.core.step.AbstractStep         : Step: [step1] executed in 23ms
2020-03-06 15:25:43.674  INFO 19449 --- [cTaskExecutor-1] o.s.batch.core.step.AbstractStep         : Step: [step3] executed in 31ms
2020-03-06 15:25:43.714  INFO 19449 --- [cTaskExecutor-2] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step2]
执行步骤二操作。。。
2020-03-06 15:25:43.738  INFO 19449 --- [cTaskExecutor-2] o.s.batch.core.step.AbstractStep         : Step: [step2] executed in 24ms
2020-03-06 15:25:43.758  INFO 19449 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [FlowJob: [name=splitJob]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 146ms
```
可以看到 step3 并没有在 step2 后才执行，说明步骤已经是并行化的（开启并行化后，并行的步骤执行顺序并不能 100%确定，因为线程调度具有不确定性）。
<a name="kgzWU"></a>
## 任务决策器
决策器的作用就是可以指定程序在不同的情况下运行不同的任务流程，比如今天是周末，则让任务执行 step1 和 step2，如果是工作日，则之心 step1 和 step3。<br />使用决策器前，需要自定义一个决策器的实现。在 cc.mrbird.batch 包下新建 decider 包，然后创建`MyDecider`类，实现`JobExecutionDecider`接口：
```java
@Component
public class MyDecider implements JobExecutionDecider {
    @Override
    public FlowExecutionStatus decide(JobExecution jobExecution, StepExecution stepExecution) {
        LocalDate now = LocalDate.now();
        DayOfWeek dayOfWeek = now.getDayOfWeek();

        if (dayOfWeek == DayOfWeek.SATURDAY || dayOfWeek == DayOfWeek.SUNDAY) {
            return new FlowExecutionStatus("weekend");
        } else {
            return new FlowExecutionStatus("workingDay");
        }
    }
}
```
`MyDecider`实现`JobExecutionDecider`接口的`decide`方法，该方法返回`FlowExecutionStatus`。上面的逻辑是：判断今天是否是周末，如果是，返回`FlowExecutionStatus("weekend")`状态，否则返回`FlowExecutionStatus("workingDay")`状态。<br />下面演示如何在任务 Job 里使用决策器。在 job 包下新建`DeciderJobDemo`：
```java
@Component
public class DeciderJobDemo {

    @Autowired
    private JobBuilderFactory jobBuilderFactory;
    @Autowired
    private StepBuilderFactory stepBuilderFactory;
    @Autowired
    private MyDecider myDecider;

    @Bean
    public Job deciderJob() {
        return jobBuilderFactory.get("deciderJob")
            .start(step1())
            .next(myDecider)
            .from(myDecider).on("weekend").to(step2())
            .from(myDecider).on("workingDay").to(step3())
            .from(step3()).on("*").to(step4())
            .end()
            .build();
    }

    private Step step1() {
        return stepBuilderFactory.get("step1")
            .tasklet((stepContribution, chunkContext) -> {
                System.out.println("执行步骤一操作。。。");
                return RepeatStatus.FINISHED;
            }).build();
    }

    private Step step2() {
        return stepBuilderFactory.get("step2")
            .tasklet((stepContribution, chunkContext) -> {
                System.out.println("执行步骤二操作。。。");
                return RepeatStatus.FINISHED;
            }).build();
    }

    private Step step3() {
        return stepBuilderFactory.get("step3")
            .tasklet((stepContribution, chunkContext) -> {
                System.out.println("执行步骤三操作。。。");
                return RepeatStatus.FINISHED;
            }).build();
    }


    private Step step4() {
        return stepBuilderFactory.get("step4")
            .tasklet((stepContribution, chunkContext) -> {
                System.out.println("执行步骤四操作。。。");
                return RepeatStatus.FINISHED;
            }).build();
    }
}
```
上面代码中，注入了自定义决策器`MyDecider`，然后在`jobDecider()`方法里使用了该决策器：
```java
@Bean
public Job deciderJob() {
    return jobBuilderFactory.get("deciderJob")
        .start(step1())
        .next(myDecider)
        .from(myDecider).on("weekend").to(step2())
        .from(myDecider).on("workingDay").to(step3())
        .from(step3()).on("*").to(step4())
        .end()
        .build();
}
```
这段代码的含义是：任务 deciderJob 首先执行 step1，然后指定自定义决策器，如果决策器返回 weekend，那么执行 step2，如果决策器返回 workingDay，那么执行 step3。如果执行了 step3，那么无论 step3 的结果是什么，都将执行 step4。<br />启动项目，控制台输出如下所示：
```java
2020-03-06 16:09:10.541  INFO 19873 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [FlowJob: [name=deciderJob]] launched with the following parameters: [{}]
2020-03-06 16:09:10.609  INFO 19873 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step1]
执行步骤一操作。。。
2020-03-06 16:09:10.641  INFO 19873 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step1] executed in 32ms
2020-03-06 16:09:10.692  INFO 19873 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step3]
执行步骤三操作。。。
2020-03-06 16:09:10.723  INFO 19873 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step3] executed in 31ms
2020-03-06 16:09:10.769  INFO 19873 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [step4]
执行步骤四操作。。。
2020-03-06 16:09:10.797  INFO 19873 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [step4] executed in 27ms
2020-03-06 16:09:10.818  INFO 19873 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [FlowJob: [name=deciderJob]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 256ms
```
因为今天是 2020 年 03 月 06 日星期五，是工作日，所以任务执行了 step1、step3 和 step4。
<a name="bj7RG"></a>
## 任务嵌套
任务 Job 除了可以由 Step 或者 Flow 构成外，还可以将多个任务 Job 转换为特殊的 Step，然后再赋给另一个任务 Job，这就是任务的嵌套。<br />举个例子，在 job 包下新建NestedJobDemo类：
```java
@Component
public class NestedJobDemo {

    @Autowired
    private JobBuilderFactory jobBuilderFactory;
    @Autowired
    private StepBuilderFactory stepBuilderFactory;
    @Autowired
    private JobLauncher jobLauncher;
    @Autowired
    private JobRepository jobRepository;
    @Autowired
    private PlatformTransactionManager platformTransactionManager;

    // 父任务
    @Bean
    public Job parentJob() {
        return jobBuilderFactory.get("parentJob")
            .start(childJobOneStep())
            .next(childJobTwoStep())
            .build();
    }


    // 将任务转换为特殊的步骤
    private Step childJobOneStep() {
        return new JobStepBuilder(new StepBuilder("childJobOneStep"))
            .job(childJobOne())
            .launcher(jobLauncher)
            .repository(jobRepository)
            .transactionManager(platformTransactionManager)
            .build();
    }

    // 将任务转换为特殊的步骤
    private Step childJobTwoStep() {
        return new JobStepBuilder(new StepBuilder("childJobTwoStep"))
            .job(childJobTwo())
            .launcher(jobLauncher)
            .repository(jobRepository)
            .transactionManager(platformTransactionManager)
            .build();
    }

    // 子任务一
    private Job childJobOne() {
        return jobBuilderFactory.get("childJobOne")
            .start(
            stepBuilderFactory.get("childJobOneStep")
            .tasklet((stepContribution, chunkContext) -> {
                System.out.println("子任务一执行步骤。。。");
                return RepeatStatus.FINISHED;
            }).build()
        ).build();
    }

    // 子任务二
    private Job childJobTwo() {
        return jobBuilderFactory.get("childJobTwo")
            .start(
            stepBuilderFactory.get("childJobTwoStep")
            .tasklet((stepContribution, chunkContext) -> {
                System.out.println("子任务二执行步骤。。。");
                return RepeatStatus.FINISHED;
            }).build()
        ).build();
    }
}
```
上面代码中，通过`childJobOne()`和`childJobTwo()`方法创建了两个任务 Job，这里没什么好说的，前面都介绍过。关键在于`childJobOneStep()`方法和`childJobTwoStep()`方法。在`childJobOneStep()`方法中，通过`JobStepBuilder`构建了一个名称为`childJobOneStep`的 Step，顾名思义，它是一个任务型 Step 的构造工厂，可以将任务转换为“特殊”的步骤。在构建过程中，还需要传入任务执行器 `JobLauncher`、任务仓库 `JobRepository` 和事务管理器 `PlatformTransactionManager`。<br />将任务转换为特殊的步骤后，将其赋给父任务 `parentJob` 即可，流程和前面介绍的一致。<br />配置好后，启动项目，控制台输出如下所示：
```java
2020-03-06 16:58:39.771  INFO 21588 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=parentJob]] launched with the following parameters: [{}]
2020-03-06 16:58:39.812  INFO 21588 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [childJobOneStep]
2020-03-06 16:58:39.866  INFO 21588 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=childJobOne]] launched with the following parameters: [{}]
2020-03-06 16:58:39.908  INFO 21588 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [childJobOneStep]
子任务一执行步骤。。。
2020-03-06 16:58:39.940  INFO 21588 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [childJobOneStep] executed in 32ms
2020-03-06 16:58:39.960  INFO 21588 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=childJobOne]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 86ms
2020-03-06 16:58:39.983  INFO 21588 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [childJobOneStep] executed in 171ms
2020-03-06 16:58:40.019  INFO 21588 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [childJobTwoStep]
2020-03-06 16:58:40.067  INFO 21588 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=childJobTwo]] launched with the following parameters: [{}]
2020-03-06 16:58:40.102  INFO 21588 --- [           main] o.s.batch.core.job.SimpleStepHandler     : Executing step: [childJobTwoStep]
子任务二执行步骤。。。
2020-03-06 16:58:40.130  INFO 21588 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [childJobTwoStep] executed in 28ms
2020-03-06 16:58:40.152  INFO 21588 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=childJobTwo]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 75ms
2020-03-06 16:58:40.157  INFO 21588 --- [           main] o.s.batch.core.step.AbstractStep         : Step: [childJobTwoStep] executed in 138ms
2020-03-06 16:58:40.177  INFO 21588 --- [           main] o.s.b.c.l.support.SimpleJobLauncher      : Job: [SimpleJob: [name=parentJob]] completed with the following parameters: [{}] and the following status: [COMPLETED] in 398ms
```
<a name="JVq6c"></a>
### 参考资料
Spring Batch 官方文档: [https://docs.spring.io/spring-batch/docs/4.3.x/reference/html/spring-batch-intro.html#spring-batch-intro](https://docs.spring.io/spring-batch/docs/4.3.x/reference/html/spring-batch-intro.html#spring-batch-intro)
