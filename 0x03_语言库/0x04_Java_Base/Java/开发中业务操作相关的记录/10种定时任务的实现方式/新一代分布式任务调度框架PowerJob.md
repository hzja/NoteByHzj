Java
<a name="W4Kap"></a>
## 概述
PowerJob是新一代分布式任务调度与计算框架，支持CRON、API、固定频率、固定延迟等调度策略，提供工作流来编排任务解决依赖关系，能轻松完成作业的调度与繁杂任务的分布式计算。
<a name="gFlgg"></a>
## 为什么选择PowerJob？
当前市面上流行的作业调度框架有老牌的Quartz、基于Quartz的elastic-job和原先基于Quartz后面移除依赖的xxl-job，这里分别谈一些这些框架现存的缺点。<br />Quartz可以视为第一代任务调度框架，基本上是现有所有分布式调度框架的“祖宗”。由于历史原因，它不提供Web界面，只能通过API完成任务的配置，使用起来不够方便和灵活，同时它仅支持单机执行，无法有效利用整个集群的计算能力。<br />xxl-job可以视为第二代任务调度框架，在一定程度上解决了Quartz的不足，在过去几年中是个非常优秀的调度框架，不过放到今天来看，还是存在着一些不足的，具体如下：

- **数据库支持单一：** 仅支持MySQL，使用其他DB需要自己魔改代码
- **有限的分布式计算能力：** 仅支持静态分片，无法很好的完成复杂任务的计算
- **不支持工作流：** 无法配置各个任务之间的依赖关系，不适用于有DAG需求的场景

正所谓长江后浪推前浪，在如今这个数据量日益增长、业务越来越复杂的年代，急需一款更为强大的任务调度框架来解决上诉问题，而PowerJob因此应运而生。<br />PowerJob可以被认为是第三代任务调度框架，在任务调度的基础上，还额外提供了分布式计算和工作流功能，其主要特性如下：

- **使用简单：** 提供前端Web界面，允许开发者可视化地完成调度任务的管理（增、删、改、查）、任务运行状态监控和运行日志查看等功能。
- **定时策略完善：** 支持CRON表达式、固定频率、固定延迟和API四种定时调度策略。
- **执行模式丰富：** 支持单机、广播、Map、MapReduce四种执行模式，其中Map/MapReduce处理器能使开发者寥寥数行代码便获得集群分布式计算的能力。
- **DAG工作流支持：** 支持在线配置任务依赖关系，可视化得对任务进行编排，同时还支持上下游任务间的数据传递
- **执行器支持广泛：** 支持Spring Bean、内置/外置Java类、Shell、Python等处理器，应用范围广。
- **运维便捷：** 支持在线日志功能，执行器产生的日志可以在前端控制台页面实时显示，降低debug成本，极大地提高开发效率。
- **依赖精简：** 最小仅依赖关系型数据库（MySQL/PostgreSQL/Oracle/MS SQLServer…），同时支持所有Spring Data JPA所支持的关系型数据库。
- **高可用&高性能：** 调度服务器经过精心设计，一改其他调度框架基于数据库锁的策略，实现了无锁化调度。部署多个调度服务器可以同时实现高可用和性能的提升（支持无限的水平扩展）。
- **故障转移与恢复：** 任务执行失败后，可根据配置的重试策略完成重试，只要执行器集群有足够的计算节点，任务就能顺利完成。
<a name="CEGzF"></a>
## 同类产品对比
|  | QuartZ | xxl-job | SchedulerX 2.0 | PowerJob |
| --- | --- | --- | --- | --- |
| 定时类型 | CRON | CRON | CRON、固定频率、固定延迟、OpenAPI | **CRON、固定频率、固定延迟、OpenAPI** |
| 任务类型 | 内置Java | 内置Java、GLUE Java、Shell、Python等脚本 | 内置Java、外置Java（FatJar）、Shell、Python等脚本 | **内置Java、外置Java（容器）、Shell、Python等脚本** |
| 分布式任务 | 无 | 静态分片 | MapReduce动态分片 | **MapReduce动态分片** |
| 在线任务治理 | 不支持 | 支持 | 支持 | **支持** |
| 日志白屏化 | 不支持 | 支持 | 不支持 | **支持** |
| 调度方式及性能 | 基于数据库锁，有性能瓶颈 | 基于数据库锁，有性能瓶颈 | 不详 | **无锁化设计，性能强劲无上限** |
| 报警监控 | 无 | 邮件 | 短信 | **邮件，提供接口允许开发者扩展** |
| 系统依赖 | JDBC支持的关系型数据库（MySQL、Oracle…） | MySQL | 人民币（公测期间免费） | **任意Spring Data Jpa支持的关系型数据库（MySQL、Oracle…）** |
| DAG工作流 | 不支持 | 不支持 | 支持 | **支持** |

<a name="zmmKD"></a>
## 适用场景
有定时执行需求的业务场景：如每天凌晨全量同步数据、生成业务报表等。<br />有需要全部机器一同执行的业务场景：如使用广播执行模式清理集群日志。<br />有需要分布式处理的业务场景：比如需要更新一大批数据，单机执行耗时非常长，可以使用Map/MapReduce处理器完成任务的分发，调动整个集群加速计算。
<a name="W0x4k"></a>
## 整体架构
![2022-12-04-13-26-19.555656900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670131613963-342ab6d7-fbdc-442b-8304-1bd8827dbb8b.png#averageHue=%23a6f1bf&clientId=ucb4fd48b-a3c6-4&from=ui&id=u8a0b1dba&originHeight=898&originWidth=998&originalType=binary&ratio=1&rotation=0&showTitle=false&size=140473&status=done&style=none&taskId=uec07505b-8e41-4e7d-a684-65ac8923a9c&title=)
<a name="ZLDRR"></a>
## 快速开始
PowerJob由调度服务器（powerjob-server）和执行器(powerjob-worker)两部分组成，powerjob-server负责提供Web服务和完成任务的调度，powerjob-worker则负责执行用户所编写的任务代码，同时提供分布式计算能力。
<a name="BITB4"></a>
## 初始化项目
```bash
git clone https://github.com/KFCFans/PowerJob.git
```
导入 IDE，源码结构如下，需要启动调度服务器（powerjob-server），同时在samples工程中编写自己的处理器代码<br />![2022-12-04-13-26-19.721984500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670131613935-e8385a25-2316-4e05-a81b-f6616702b38a.png#averageHue=%238b8d8c&clientId=ucb4fd48b-a3c6-4&from=ui&id=ZAWQt&originHeight=670&originWidth=1316&originalType=binary&ratio=1&rotation=0&showTitle=false&size=135797&status=done&style=none&taskId=u471b3ec2-9101-4d4c-84b9-4f0d687d728&title=)
<a name="BBiJg"></a>
## 启动调度服务器
创建数据库 powerjob-daily<br />修改配置文件，配置文件的说明官方文档写的非常详细，此处不再赘述。需要修改的地方为数据库配置`spring.datasource.core.jdbc-url`、`spring.datasource.core.username`和`spring.datasource.core.password`，当然，有mongoDB的同学也可以修改`spring.data.mongodb.uri`以获取完全版体验。
```
oms.env=DAILY
logging.config=classpath:logback-dev.xml

####### 数据库配置 #######
spring.datasource.core.driver-class-name=com.mysql.cj.jdbc.Driver
spring.datasource.core.jdbc-url=jdbc:mysql://remotehost:3306/powerjob-daily?useUnicode=true&characterEncoding=UTF-8
spring.datasource.core.username=root
spring.datasource.core.password=No1Bug2Please3!
spring.datasource.core.hikari.maximum-pool-size=20
spring.datasource.core.hikari.minimum-idle=5

####### mongoDB配置，非核心依赖，可移除 #######
spring.data.mongodb.uri=mongodb://remotehost:27017/powerjob-daily

####### 邮件配置（启用邮件报警则需要） #######
spring.mail.host=smtp.163.com
spring.mail.username=zqq
spring.mail.password=qqz
spring.mail.properties.mail.smtp.auth=true
spring.mail.properties.mail.smtp.starttls.enable=true
spring.mail.properties.mail.smtp.starttls.required=true

####### 资源清理配置 #######
oms.log.retention.local=1
oms.log.retention.remote=1
oms.container.retention.local=1
oms.container.retention.remote=-1
oms.instanceinfo.retention=1

####### 缓存配置 #######
oms.instance.metadata.cache.size=1024
```
完成配置文件的修改后，可以直接通过启动类com.github.kfcfans.powerjob.server.OhMyApplication启动调度服务器，观察启动日志，查看是否启动成功～启动成功后，访问 http://127.0.0.1:7700/ ，如果能顺利出现Web界面，则说明调度服务器启动成功！<br />注册应用：点击主页应用注册按钮，填入 oms-test和控制台密码（用于进入控制台），注册示例应用（当然也可以注册其他的appName，只是别忘记在示例程序中同步修改～）<br />![2022-12-04-13-26-19.817996300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670131654909-34caedd6-3be7-4633-9021-62fd49e785bb.png#averageHue=%23051121&clientId=ucb4fd48b-a3c6-4&from=ui&id=uca229229&originHeight=896&originWidth=1905&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1117471&status=done&style=none&taskId=u26240d5c-7d3e-449c-ad2e-5db0219f8b3&title=)
<a name="k46fU"></a>
## 编写示例代码
进入示例工程（powerjob-worker-samples），修改配置文件连接powerjob-server并编写自己的处理器代码。<br />修改powerjob-worker-samples的启动配置类com.github.kfcfans.powerjob.samples.OhMySchedulerConfig，将AppName修改为刚刚在控制台注册的名称。
```java
@Configuration
public class OhMySchedulerConfig {
    @Bean
    public OhMyWorker initOMS() throws Exception {

        // 服务器HTTP地址（端口号为 server.port，而不是 ActorSystem port）
        List<String> serverAddress = Lists.newArrayList("127.0.0.1:7700");

        // 1. 创建配置文件
        OhMyConfig config = new OhMyConfig();
        config.setPort(27777);
        config.setAppName("oms-test");
        config.setServerAddress(serverAddress);
        // 如果没有大型 Map/MapReduce 的需求，建议使用内存来加速计算
        config.setStoreStrategy(StoreStrategy.MEMORY);

        // 2. 创建 Worker 对象，设置配置文件
        OhMyWorker ohMyWorker = new OhMyWorker();
        ohMyWorker.setConfig(config);
        return ohMyWorker;
    }
}
```
编写自己的处理器：随便找个地方新建类，继承想要使用的处理器（各个处理器的介绍可见官方文档，文档非常详细），这里为了简单演示，选择使用单机处理器BasicProcessor，以下是代码示例。
```java
@Slf4j
@Component
public class StandaloneProcessorDemo implements BasicProcessor {

    @Override
    public ProcessResult process(TaskContext context) throws Exception {

        OmsLogger omsLogger = context.getOmsLogger();
        omsLogger.info("StandaloneProcessorDemo start process,context is {}.", context);
        System.out.println("jobParams is " + context.getJobParams());
        
        return new ProcessResult(true, "process successfully~");
    }
}
```
启动示例程序，即直接运行主类com.github.kfcfans.powerjob.samples.SampleApplication，观察控制台输出信息，判断是否启动成功。
<a name="YYIkp"></a>
## 任务的配置与运行
调度服务器与示例工程都启动完毕后，再次前往Web页面（ http://127.0.0.1:7700/ ），进行任务的配置与运行。<br />在首页输入框输入配置的应用名称，成功操作后会正式进入前端管理界面。<br />![2022-12-04-13-26-20.147269700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670131654485-a3c7ff73-27ff-4f79-bd8c-c4bf84f305d2.png#averageHue=%23dfdada&clientId=ucb4fd48b-a3c6-4&from=ui&id=zWHZ6&originHeight=634&originWidth=1916&originalType=binary&ratio=1&rotation=0&showTitle=false&size=182308&status=done&style=none&taskId=uaf2b0b7e-d906-4829-bb4e-e739c8d3301&title=)<br />点击任务管理 -> 新建任务（右上角），开始创建任务。<br />![2022-12-04-13-26-20.231551000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670131687905-572aed05-2c2f-414d-805d-9c478f82f1c2.png#averageHue=%23fefdfd&clientId=ucb4fd48b-a3c6-4&from=ui&id=u0418cce4&originHeight=748&originWidth=937&originalType=binary&ratio=1&rotation=0&showTitle=false&size=116369&status=done&style=none&taskId=u170762c1-56dc-4276-88f9-2a323983835&title=)<br />完成任务创建后，即可在控制台看到刚才创建的任务，如果觉得等待调度太过于漫长，可以直接点击运行按钮，立即运行本任务。<br />![2022-12-04-13-26-20.254992400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670131688016-02503df8-c866-4603-affd-eec420a9a747.png#averageHue=%23e7e7e7&clientId=ucb4fd48b-a3c6-4&from=ui&id=NCD7a&originHeight=867&originWidth=1911&originalType=binary&ratio=1&rotation=0&showTitle=false&size=199388&status=done&style=none&taskId=ub4fa77ec-a9d8-4705-97a9-79d8c262c93&title=)<br />前往任务示例边栏，查看任务的运行状态和在线日志<br />![2022-12-04-13-26-20.333575800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670131688182-2a60cc04-0b85-4b2b-9072-df3d109b4773.png#averageHue=%23ebeaea&clientId=ucb4fd48b-a3c6-4&from=ui&id=BDBTE&originHeight=930&originWidth=1910&originalType=binary&ratio=1&rotation=0&showTitle=false&size=245252&status=done&style=none&taskId=ue266adec-abc7-4d6b-9706-d65198bce47&title=)<br />基础的教程到这里也就结束了～更多功能示例可见官方文档，工作流、MapReduce、容器等高级特性等你来探索！
<a name="kLexE"></a>
## 相关链接
**项目地址：**[https://github.com/KFCFans/PowerJob](https://github.com/KFCFans/PowerJob)<br />**官方文档：**[https://www.yuque.com/powerjob/guidence/ztn4i5](https://www.yuque.com/powerjob/guidence/ztn4i5)<br />**在线试用：**[https://www.yuque.com/powerjob/guidence/hnbskn](https://www.yuque.com/powerjob/guidence/hnbskn)
