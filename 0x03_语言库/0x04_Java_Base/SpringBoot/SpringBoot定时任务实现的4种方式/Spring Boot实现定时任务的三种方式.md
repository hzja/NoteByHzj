Java SpringBoot<br />SpringBoot创建定时任务，目前主要有以下三种实现方式：

- **基于注解(**`**@Scheduled**`**)：** 基于注解`@Scheduled`默认为单线程，开启多个任务时，任务的执行时机会受上一个任务执行时间的影响；
- **基于接口（**`**SchedulingConfigurer**`**）：** 用于实现从数据库获取指定时间来动态执行定时任务；
- **基于注解设定多线程定时任务：** 基于注解设定多线程定时任务；
<a name="FCDB4"></a>
## 一、静态：基于注解
<a name="wGxKi"></a>
### 1、创建定时器
使用SpringBoot基于注解来创建定时任务比较简单，只需要如下代码即可。代码如下：
```java
@Configuration      //1.主要用于标记配置类，兼备Component的效果。
@EnableScheduling   // 2.开启定时任务
public class SaticScheduleTask {
    //3.添加定时任务
    @Scheduled(cron = "0/5 * * * * ?")
    //或直接指定时间间隔，例如：5秒
    //@Scheduled(fixedRate=5000)
    private void configureTasks() {
        System.err.println("执行静态定时任务时间: " + LocalDateTime.now());
    }
}
```
Cron表达式参数分别表示：

- 秒（0~59） 例如0/5表示每5秒
- 分（0~59）
- 时（0~23）
- 日（0~31）的某天，需计算
- 月（0~11）
- 周几（ 可填1-7 或 SUN/MON/TUE/WED/THU/FRI/SAT）

`@Scheduled`：除了支持灵活的参数表达式cron之外，还支持简单的延时操作，例如 fixedDelay ，fixedRate 填写相应的毫秒数即可。<br />建议：直接点击在线Cron表达式生成器生成参数比较方便：[https://www.matools.com/cron/](https://www.matools.com/cron/) 
<a name="asVIm"></a>
### 2、启动测试
启动应用，控制台打印信息如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648470676455-e3cc199e-96dc-4d40-a3e6-d650358e7987.png#clientId=uaeedcfda-4fa4-4&from=paste&id=ua795ee89&originHeight=183&originWidth=432&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue13fe1d2-dc45-4a97-b919-34b02d5660b&title=)<br />显然，使用`@Scheduled` 注解很方便，但缺点是当调整了执行周期的时候，需要重启应用才能生效，这多少有些不方便。为了达到实时生效的效果，可以使用接口来完成定时任务。
<a name="SFqIt"></a>
## 二、动态：基于接口
基于接口（SchedulingConfigurer）
<a name="DyKie"></a>
### 1、导入依赖包：
```xml
<parent>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter</artifactId>
	<version>2.0.4.RELEASE</version>
</parent>

<dependencies>
	<dependency><!--添加Web依赖 -->
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-web</artifactId>
	</dependency>
	<dependency><!--添加MySql依赖 -->
		<groupId>mysql</groupId>
		<artifactId>mysql-connector-java</artifactId>
	</dependency>
	<dependency><!--添加Mybatis依赖 配置mybatis的一些初始化的东西-->
		<groupId>org.mybatis.spring.boot</groupId>
		<artifactId>mybatis-spring-boot-starter</artifactId>
		<version>1.3.1</version>
	</dependency>
	<dependency><!-- 添加mybatis依赖 -->
		<groupId>org.mybatis</groupId>
		<artifactId>mybatis</artifactId>
		<version>3.4.5</version>
		<scope>compile</scope>
	</dependency>
</dependencies>
```
<a name="fcwjH"></a>
### 2、添加数据库记录：
开启本地数据库mysql，随便打开查询窗口，然后执行脚本内容，如下：
```sql
DROP DATABASE IF EXISTS `socks`;
CREATE DATABASE `socks`;
USE `SOCKS`;
DROP TABLE IF EXISTS `cron`;
CREATE TABLE `cron`  (
  `cron_id` varchar(30) NOT NULL PRIMARY KEY,
  `cron` varchar(30) NOT NULL  
);
INSERT INTO `cron` VALUES ('1', '0/5 * * * * ?');
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648470676486-ff9f749d-e327-49c4-b3ba-e0d31efdb774.png#clientId=uaeedcfda-4fa4-4&from=paste&id=u6dc692e6&originHeight=179&originWidth=476&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud2c6a759-2ae5-4150-a2c4-a3b5cab0a90&title=)<br />然后在项目中的application.yml 添加数据源：
```yaml
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/socks
    username: root
    password: 123456
```
<a name="MpF3M"></a>
### 3、创建定时器
数据库准备好数据后，开始编写定时任务，注意这里添加的是TriggerTask，目的是循环读取在数据库设置好的执行周期，以及执行相关定时任务的内容。<br />具体代码如下：
```java
@Configuration      //1.主要用于标记配置类，兼备Component的效果。
@EnableScheduling   // 2.开启定时任务
public class DynamicScheduleTask implements SchedulingConfigurer {
 
    @Mapper
    public interface CronMapper {
        @Select("select cron from cron limit 1")
        public String getCron();
    }
 
    @Autowired      //注入mapper
    @SuppressWarnings("all")
    CronMapper cronMapper;
 
    /**
     * 执行定时任务.
     */
    @Override
    public void configureTasks(ScheduledTaskRegistrar taskRegistrar) {
 
        taskRegistrar.addTriggerTask(
                //1.添加任务内容(Runnable)
                () -> System.out.println("执行动态定时任务: " + LocalDateTime.now().toLocalTime()),
                //2.设置执行周期(Trigger)
                triggerContext -> {
                    //2.1 从数据库获取执行周期
                    String cron = cronMapper.getCron();
                    //2.2 合法性校验.
                    if (StringUtils.isEmpty(cron)) {
                        // Omitted Code ..
                    }
                    //2.3 返回执行周期(Date)
                    return new CronTrigger(cron).nextExecutionTime(triggerContext);
                }
        );
    }
 
}
```
<a name="oFXgF"></a>
### 4、启动测试
启动应用后，查看控制台，打印时间是预期的每10秒一次：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648470676471-b2406bd3-3215-4f46-9121-3b63b90a2185.png#clientId=uaeedcfda-4fa4-4&from=paste&id=ud96cef75&originHeight=191&originWidth=311&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3682f610-69a1-4248-8d8a-a1c32b3e477&title=)<br />然后打开Navicat ，将执行周期修改为每6秒执行一次，如图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648470676500-fe461bc5-3466-421d-8cc2-d7292b9748d5.png#clientId=uaeedcfda-4fa4-4&from=paste&id=u1730d660&originHeight=180&originWidth=522&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u167f0617-4c8b-484c-a291-65f1d478c44&title=)<br />查看控制台，发现执行周期已经改变，并且不需要重启应用，十分方便。如图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648470676466-c6a09ed3-a822-4174-9a92-65420aadb915.png#clientId=uaeedcfda-4fa4-4&from=paste&id=u63d87223&originHeight=184&originWidth=347&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u508f08a6-f172-41f1-b805-8c2b29d9298&title=)<br />注意：如果在数据库修改时格式出现错误，则定时任务会停止，即使重新修改正确后，也只能重新启动项目才能恢复。
<a name="wg7qt"></a>
## 三、多线程定时任务
基于注解设定多线程定时任务
<a name="oTOLB"></a>
### 1、创建多线程定时任务
```java
//@Component注解用于对那些比较中立的类进行注释；
//相对与在持久层、业务层和控制层分别采用 @Repository、@Service 和 @Controller 对分层中的类进行注释
@Component
@EnableScheduling   // 1.开启定时任务
@EnableAsync        // 2.开启多线程
public class MultithreadScheduleTask {
    
    @Async
    @Scheduled(fixedDelay = 1000)  //间隔1秒
    public void first() throws InterruptedException {
        System.out.println("第一个定时任务开始 : " + LocalDateTime.now().toLocalTime() + "\r\n线程 : " + Thread.currentThread().getName());
        System.out.println();
        Thread.sleep(1000 * 10);
    }
    
    @Async
    @Scheduled(fixedDelay = 2000)
    public void second() {
        System.out.println("第二个定时任务开始 : " + LocalDateTime.now().toLocalTime() + "\r\n线程 : " + Thread.currentThread().getName());
        System.out.println();
    }
}
```
注：这里的`@Async`注解很关键
<a name="bQDkm"></a>
### 2、启动测试
启动应用后，查看控制台：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648470730428-ea8b91b3-ec8f-4022-9c5d-8cda7611a499.png#clientId=uaeedcfda-4fa4-4&from=paste&height=383&id=u3ed57935&originHeight=574&originWidth=722&originalType=binary&ratio=1&rotation=0&showTitle=false&size=145227&status=done&style=shadow&taskId=uf6af798f-b31e-4663-a217-4949b8c0b0f&title=&width=481.3333333333333)<br />从截图中可以看出，第一个和第二个定时任务互不影响；并且，由于开启了多线程，第一个任务的执行时间也不受其本身执行时间的限制，所以需要注意可能会出现重复操作导致数据异常。
