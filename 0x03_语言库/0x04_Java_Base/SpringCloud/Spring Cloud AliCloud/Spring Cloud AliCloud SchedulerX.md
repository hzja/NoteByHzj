Spring Cloud AliCloud SchedulerX
<a name="R8cWz"></a>
### 1、引入 SchedulerX 的依赖
如果要在项目中引入 SchedulerX，使用 group ID 为 `com.alibaba.cloud` 和 artifact ID 为 `spring-cloud-starter-alicloud-schedulerX` 的 starter。
```xml
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-starter-alicloud-schedulerX</artifactId>
</dependency>
```
<a name="d6zba"></a>
### 2、启动SchedulerX任务调度
当客户端引入了 Spring Cloud AliCloud SchedulerX Starter 以后，只需要进行一些简单的配置，就可以自动初始化SchedulerX的任务调度服务。<br />以下是一个简单的应用示例。
```java
@SpringBootApplication
public class ScxApplication {
    public static void main(String[] args) {
        SpringApplication.run(ScxApplication.class, args);
    }
}
```
在application.properties中，需要加上以下配置。
```
server.port=18033
# 其中cn-test是SchedulerX的测试区域
spring.cloud.alicloud.scx.group-id=***
spring.cloud.alicloud.edas.namespace=cn-test
```
在获取group-id之前，需要首先 注册阿里云账号 ，然后 开通EDAS服务 ，并开通分布式任务管理组件 。<br />其中group-id的获取，请参考下面。
:::info
在创建group的时候，要选择"测试"区域。
:::
<a name="yUN3h"></a>
### 3、编写一个简单任务
简单任务是最常用的任务类型，只需要实现 ScxSimpleJobProcessor 接口即可。<br />以下是一个简单的单机类型任务示例。
```java
public class SimpleTask implements ScxSimpleJobProcessor {
	@Override
	public ProcessResult process(ScxSimpleJobContext context) {
		System.out.println("-----------Hello world---------------");
		ProcessResult processResult = new ProcessResult(true);
		return processResult;
	}
}
```
<a name="uCGR1"></a>
### 4、 对任务进行调度
进入 SchedulerX任务列表页面，选择上方"测试"区域，点击右上角"新建Job"，创建一个Job，即如下所示。
```
Job分组：测试——***-*-*-****
Job处理接口：org.springframework.cloud.alibaba.cloud.examples.SimpleTask
类型：简单Job单机版
定时表达式：默认选项——0 * * * * ?
Job描述：无
自定义参数：无
```
以上任务类型选择了"简单Job单机版"，并且制定了Cron表达式为"0 * * * * ?"，这意味着，每过一分钟，任务将会被执行且只执行一次。<br />更多任务类型，请参考 SchedulerX官方文档。
<a name="ic8Gh"></a>
### 5、生产环境使用
以上使用的都是SchedulerX的"测试"区域，主要用于本地调试和测试。<br />在生产级别，除了上面的group-id和namespace以外，还需要一些额外的配置，如下所示。
```
server.port=18033
# 其中cn-test是SchedulerX的测试区域
spring.cloud.alicloud.scx.group-id=***
spring.cloud.alicloud.edas.namespace=***
# 当应用运行在EDAS上时，以下配置不需要手动配置。
spring.cloud.alicloud.access-key=***
spring.cloud.alicloud.secret-key=***
# 以下配置不是必须的，请参考SchedulerX文档
spring.cloud.alicloud.scx.domain-name=***
```
其中group-id与之前的获取方式一样，namespace则是从EDAS控制台左侧"命名空间"列表中获取命名空间ID。
:::info
group-id必须创建在namespace当中。
:::
access-key以及secret-key为阿里云账号的AK/SK信息，如果应用在EDAS上部署，则不需要填写这两项信息，否则请前往安全信息管理获取。<br />domain-name并不是必须的，具体请参考 SchedulerX官方文档。
