在微服务系统中，经常会有用到任务调度的场景。比如每天定时同步数据、定时生成业务报表、定期清理日志等。推荐一个分布式调度框架，轻松完成任务调度工作！
<a name="mQONh"></a>
## PowerJob 简介
PowerJob是全新一代分布式调度与计算框架，能轻松完成任务的调度与繁杂任务的分布式计算。<br />主要特性：

- 使用简单：提供前端Web界面，允许开发者可视化完成调度任务的管理及查看任务运行状态和日志。
- 定时策略完善：支持CRON表达式、固定频率、固定延迟和API四种定时调度策略。
- 执行模式丰富：支持单机、广播、Map、MapReduce四种执行模式。
- 依赖精简：最小仅依赖关系型数据库（MySQL等），扩展依赖为MongoDB（用于存储庞大的在线日志）。
<a name="mSbdA"></a>
## 为什么要有调度中心
一般情况下会使用QuartZ或Spring Task这些框架在应用中实现定时任务来进行任务调度，但是在微服务架构下，如果很多应用都充斥着这种任务调度代码就显得有些不合适。合理的方案应该是这样的，任务的执行方法存在于应用中，有一个调度中心，调度中心负责调度这些方法，只需在调度中心配置好任务即可，PowerJob正是这样一个分布式调度框架。
<a name="BUC3m"></a>
## 安装准备
由于PowerJob的调度中心（powerjob-server）需要使用MySQL存储数据，使用MongoDB存储日志，所以先安装并启动这两个服务。

- 在Docker容器中启动MySQL服务；
```bash
docker run -p 3306:3306 --name mysql \
-v /mydata/mysql/log:/var/log/mysql \
-v /mydata/mysql/data:/var/lib/mysql \
-v /mydata/mysql/conf:/etc/mysql \
-e MYSQL_ROOT_PASSWORD=root  \
-d mysql:5.7
```

- 在MySQL中创建PowerJob需要的数据库powerjob-product；
```sql
CREATE DATABASE IF NOT EXISTS `powerjob-product` DEFAULT CHARSET utf8mb4
```

- 在Docker容器中启动MongoDB服务。
```bash
docker run -p 27017:27017 --name mongo \
-v /mydata/mongo/db:/data/db \
-d mongo:4.2.5
```
<a name="Wr7wc"></a>
## 安装调度中心
在Docker环境下安装PowerJob的调度中心非常简单，分分钟搞定！

- 下载镜像powerjob-server的Docker镜像：
```bash
docker pull tjqq/powerjob-server:latest
```

- 在Docker容器中运行powerjob-server服务：
```bash
docker run -p 7700:7700 -p 10086:10086 --name powerjob-server \
--link mysql:db \
--link mongo:mongo \
-e TZ="Asia/Shanghai" \
-e JVMOPTIONS="" \
-e PARAMS="--spring.profiles.active=product --spring.datasource.core.jdbc-url=jdbc:mysql://db:3306/powerjob-product?useUnicode=true&characterEncoding=UTF-8 --spring.datasource.core.username=root --spring.datasource.core.password=root --spring.data.mongodb.uri=mongodb://mongo:27017/powerjob-product" \
-v ~/docker/powerjob-server:/mydata/powerjob/powerjob-server \
-v ~/.m2:/mydata/powerjob/.m2 \
-d tjqq/powerjob-server:latest
```

- 运行成功后即可访问powerjob-server的Web界面，注意Linux防火墙需要开放7700和10086两个端口，访问地址：http://192.168.3.101:7700/

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697770594292-d32caf68-2ec4-436c-bfdd-57bfe6190f9b.png#averageHue=%230b2147&clientId=ue85991e4-e3c7-4&from=paste&id=u7901b88a&originHeight=673&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u44da7195-d946-4448-836d-580c37565fd&title=)
<a name="WK7yU"></a>
## 在应用中初始化执行器
安装完调度中心后，需要在SpringBoot应用中初始化PowerJob的执行器（powerjob-worker）。

- 首先在pom.xml中添加powerjob-worker的相关依赖：
```xml
<dependency>
  <groupId>com.github.kfcfans</groupId>
  <artifactId>powerjob-worker-spring-boot-starter</artifactId>
  <version>3.2.3</version>
</dependency>
```

- 之后在application.yml配置文件中添加powerjob-worker相关配置，注意powerjob.worker.app-name这个配置；
```yaml
powerjob:
  worker:
    akka-port: 27777 # akka 工作端口
    app-name: powerjob-sample # 接入应用名称，用于分组隔离
    server-address: 192.168.3.101:7700 # 调度服务器地址
    store-strategy: disk # 持久化方式
```

- 添加一个单机处理器`StandaloneProcessor`，只需继承`BasicProcessor`接口并实现process方法即可；
```java
@Slf4j
@Component
public class StandaloneProcessor implements BasicProcessor {

    @Override
    public ProcessResult process(TaskContext context){
        //OmsLogger可以直接将日志上报到powerjob-server
        OmsLogger omsLogger = context.getOmsLogger();
        omsLogger.info("StandaloneProcessor start process,context is {}.", context);
        log.info("jobParams is {}", context.getJobParams());
        return new ProcessResult(true, "Process success!");
    }
}
```

- 打包上传好镜像后，在Docker容器中运行SpringBoot应用服务，注意配置好时区要和调度中心一致。
```bash
docker run -p 8080:8080 --name mall-tiny-powerjob \
--link mysql:db \
-v /etc/localtime:/etc/localtime \
-v /mydata/app/mall-tiny-powerjob/logs:/var/logs \
-e TZ="Asia/Shanghai" \
-d mall-tiny/mall-tiny-powerjob:1.0-SNAPSHOT
```
<a name="Vs0ur"></a>
## 任务的配置与执行
有了执行器和调度中心，只需在调度中心中配置好任务即可实现任务调度。

- 首先需要在调度中心注册好应用（集成执行器的），应用名称为application.yml中的powerjob.worker.app-name属性，这里使用的是powerjob-sample:123456；

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697770594384-04aa40bf-981a-4ddb-822f-202ad8be5b88.png#averageHue=%23071326&clientId=ue85991e4-e3c7-4&from=paste&id=u5d9b9bc4&originHeight=674&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua31a7171-e1f5-4185-a000-60188ab5ec6&title=)

- 之后可以在首页看见一台机器信息；

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697770594336-2a3dba0e-b136-4aaf-9a93-88dd8b23dd6d.png#averageHue=%23e9e8e7&clientId=ue85991e4-e3c7-4&from=paste&id=uba004374&originHeight=674&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u265f6045-3ff3-4d98-a66c-3f1a90f0b83&title=)

- 之后在任务管理功能中添加一个任务，这里用CRON方式设置每20秒执行执行器中的处理方法；

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697770594312-a5a23c42-2354-4923-9bc8-f3d810f3be15.png#averageHue=%23adacab&clientId=ue85991e4-e3c7-4&from=paste&id=u8940d83d&originHeight=673&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5d3fc5c3-318e-44ab-a239-90fabe809ce&title=)

- 在任务列表中点击运行开始执行任务；

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697770594360-efc04e64-d508-498f-b17e-655dc05f435a.png#averageHue=%23eaeaea&clientId=ue85991e4-e3c7-4&from=paste&id=u3f756310&originHeight=674&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u83051147-a544-4b97-ba9d-0058d1a058b&title=)

- 在任务列表中点击更多->运行记录可以查看任务的运行日志；

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697770594671-9967b225-b345-4810-9aea-c72c6b0e1acc.png#averageHue=%23e8e7e5&clientId=ue85991e4-e3c7-4&from=paste&id=u01790b69&originHeight=674&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u406f4507-6e49-49f4-992b-ac6e28ee70d&title=)

- 点击日志可以查看处理器中上报的日志，jobParams为之前创建任务时设置的参数；

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697770595179-19a68ce2-4651-4bee-9d2b-f6e6ad93ef45.png#averageHue=%23908f8e&clientId=ue85991e4-e3c7-4&from=paste&id=uef925b14&originHeight=676&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubfe623e3-d7a1-4fef-b280-1fa2ad5ded1&title=)

- 点击详情可以查看此次触发任务的结果，即在ProcessResult中返回的信息。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697770595170-34f90969-d147-4aa7-82bb-49915f3f5ce3.png#averageHue=%23a9a8a8&clientId=ue85991e4-e3c7-4&from=paste&id=u51b63bd0&originHeight=676&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u49f0920e-759b-489b-b985-623bd643e6d&title=)
