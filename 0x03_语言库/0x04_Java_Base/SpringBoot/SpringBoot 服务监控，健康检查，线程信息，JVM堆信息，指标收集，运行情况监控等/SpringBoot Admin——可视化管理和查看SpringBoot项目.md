Java SpringBoot<br />Spring Boot Admin（SBA）是一个开源的社区项目，用于管理和监控 Spring Boot 应用程序。应用程序可以通过 http 的方式，或 Spring Cloud 服务发现机制注册到 SBA 中，然后就可以实现对 Spring Boot 项目的可视化管理和查看了。<br />Spring Boot Admin 可以监控 Spring Boot 单机或集群项目，它提供详细的健康 （Health）信息、内存信息、JVM 系统和环境属性、垃圾回收信息、日志设置和查看、定时任务查看、Spring Boot 缓存查看和管理等功能。接下来搭建和使用。<br />它最终的展示效果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755714671-5dee12ab-175c-432a-8cf9-af05ca2e71a9.png#clientId=ubf6d0044-28d3-4&from=paste&id=u9685aa1e&originHeight=869&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaa4f156e-a47a-4b20-9337-f1227281fd7&title=)
<a name="KTzei"></a>
## 1、搭建SBA监控端
首先需要创建一个 Spring Boot Admin 项目，用来监控和管理 Spring Boot 项目，搭建的方式和创建普通的 Spring Boot 项目类似，具体步骤如下。使用 Idea 创建一个 Spring Boot 项目：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755714778-c38aab4a-ecb6-4b18-b364-de13c291b5f4.png#clientId=ubf6d0044-28d3-4&from=paste&id=ue42d056d&originHeight=578&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u44516003-2106-41ec-bf6f-a37d6ea97fb&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755714636-79e257b7-8746-46dc-9b79-bc165a16bf7c.png#clientId=ubf6d0044-28d3-4&from=paste&id=u268e9db1&originHeight=578&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2945fdd1-a57d-4009-a4a5-111064297dd&title=)这里需要注意，需要添加 Spring Boot Admin（Server）服务端框架的支持，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755714660-4c20468f-4151-45b2-be0d-f2ceaca02b98.png#clientId=ubf6d0044-28d3-4&from=paste&id=u2bc43512&originHeight=665&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc7d030ee-318d-4d8c-a441-0e77f3d455a&title=)也就是创建的 Spring Boot 项目需要添加以下两个重要的框架支持：
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-web</artifactId>
</dependency>
<dependency>
	<groupId>de.codecentric</groupId>
	<artifactId>spring-boot-admin-starter-server</artifactId>
</dependency>
```
<a name="aglWc"></a>
### 1.1 开启SBA服务
创建完项目之后，需要在启动类上开启 SBA 服务：
```java
import de.codecentric.boot.admin.server.config.EnableAdminServer;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@EnableAdminServer // 添加此行代码
@SpringBootApplication 
public class SbaserverApplication {
    public static void main(String[] args) {
        SpringApplication.run(SbaserverApplication.class, args);
    }
}
```
<a name="MGtus"></a>
### 1.2 配置SBA端口号
在 application.properties 中配置一个项目的端口号就可以直接启动了，这里配置的端口号是 9001：
```
server.port=9001
```
PS：配置端口号的主要目的是为了不和其他 Spring Boot 项目冲突，如果 SBA 是单独部署此步骤可以忽略。<br />启动项目之后，就可以看到 SBA 的主页了，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755714620-6656eb7c-44e5-4944-b40b-3046dc283f21.png#clientId=ubf6d0044-28d3-4&from=paste&id=u99a7ea59&originHeight=776&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue8d616c5-0301-4c0f-9aee-839075daa39&title=)此时 SBA 中还没有添加任何需要监控的项目，接下来再创建一个 Spring Boot 项目，加入到 SBA 中来进行监控和管理吧。
<a name="kMy3t"></a>
## 2、创建一个普通SpringBoot项目
首先需要创建一个普通的 Spring Boot 项目。当创建好 Spring Boot 项目之后，需要在这个 Spring Boot 项目中需要添加 SBA 客户端框架的支持，也就是在 pom.xml 中配置如下内容：
```xml
<dependency>
	<groupId>de.codecentric</groupId>
	<artifactId>spring-boot-admin-starter-client</artifactId>
</dependency>
```
然后在 application.properties 文件中配置 SBA 服务器端地址，也就是咱们第一步创建 SBA 项目的地址，配置内容如下：
```
# 当前项目端口号
server.port=8080
# Spring Boot Admin 监控服务器端地址
spring.boot.admin.client.url=http://localhost:9001
```
其中“spring.boot.admin.client.url”为 SBA 监控地址。
<a name="z5nIh"></a>
## 3、SpringBootAdmin监控总览
配置完以上信息之后，此时查看 Spring Boot Admin 页面中就有被监控的 Spring Boot 项目了，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755714966-958cf086-b800-4784-8335-44f1df384f06.png#clientId=ubf6d0044-28d3-4&from=paste&id=u834f357d&originHeight=776&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9299f6d4-7d5d-4ef1-9a37-4f947c0dd5e&title=)也可以点击“应用墙”查看 Spring Boot Admin 中所有被监控的 Spring Boot 项目，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755715149-64f176a7-6876-4484-8b0b-a7f118b7ec8e.png#clientId=ubf6d0044-28d3-4&from=paste&id=u0b79044c&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc0565d9d-3d20-43db-a69a-d0ae37ddebe&title=)点击应用进入详情页面，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755715188-ae17c2d8-00de-4f55-9db4-12a3c5650da4.png#clientId=ubf6d0044-28d3-4&from=paste&id=u1563385f&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u09efa348-692b-4331-b19a-261a4c44d1a&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755715288-fb9310eb-2be6-414b-a5b0-49d85d8a542d.png#clientId=ubf6d0044-28d3-4&from=paste&id=u6edfcc5f&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u89a1e8e4-90ac-4820-ba4f-b64781e8e2c&title=)事件日志中包含 Spring Boot 各种状态的展示（UP 为正常、OFFLINE 为异常）和发生的时间，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755715504-05cb549c-c236-460a-a136-da721f8d432a.png#clientId=ubf6d0044-28d3-4&from=paste&id=udcb8991f&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u06a9f628-4011-491d-96e2-7eb37d8bc1e&title=)
<a name="BS44f"></a>
## 4、SpringBoot异常监控
当手动把被监控的 Spring Boot 项目停止之后，在 Spring Boot Admin 中就可以查看到一个应用已经被停掉了，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755715512-4814a211-028d-49ea-a3e6-3e1d40833760.png#clientId=ubf6d0044-28d3-4&from=paste&id=u7a0d3dc3&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4dc49c17-cd83-49b5-ae4d-aac7986dfa4&title=)也可以通过事件日志查看 Spring Boot 宕机的具体时间，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755715816-70fa2d4a-fd97-4019-b4c5-fe82c819362c.png#clientId=ubf6d0044-28d3-4&from=paste&id=u1c55acd7&originHeight=673&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u79e35c61-65e7-48c8-9d90-fb7dd89dc49&title=)
<a name="Y2RqW"></a>
## 5、配置查看更多监控项
通过上面的内容可以看出，监控的 Spring Boot 选项还是比较少的，怎么才能查看更多的监控项呢？要解决这个问题，需要在被监控的 Spring Boot 项目中添加 spring-boot-starter-actuator 框架的支持，并开启查看所有监控项的配置才行，最终展示效果如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755715964-dbbc92b5-5eb1-4bfc-8d71-f06cdf59f85a.png#clientId=ubf6d0044-28d3-4&from=paste&id=udbb533f3&originHeight=1232&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7b421c7f-1488-4355-9f39-85e68bb97a8&title=)接下来配置一下这些监控项。
<a name="s4VO0"></a>
### 5.1 添加actuator框架支持
在被监控的 Spring Boot 项目中添加 actuator 框架支持，也就是在 pom.xml 中添加以下配置：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```
手动点击 Maven 导入依赖包（如果开启了自动导入，此步骤可忽略）。
<a name="hITfe"></a>
### 5.2 配置开放所有监控项
在被监控的 Spring Boot 项目中添加以下配置：
```
# 开启监控所有项
management.endpoints.web.exposure.include=*
```
以上的配置是开放监控所有选项，配置完之后，重启此 Spring Boot 项目，然后再刷新 Spring Boot Admin 更多的监控项就展示出来了，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755716015-00a9b016-af06-45db-b462-e70240b2ee55.png#clientId=ubf6d0044-28d3-4&from=paste&id=u54a0f6e1&originHeight=889&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaf1fa5ae-0bb9-4ee9-8cc4-18c34e8f7b6&title=)
<a name="sb1Gj"></a>
### 5.3 监控项目预览
将 Spring Boot 的所有监控项都开启之后，通过 SBA 就可以查看以下内容了：

- 启动时间、累计运行时间；
- 进程和线程数量和占用的 CPU 资源；
- 垃圾回收详情信息，回收次数和花费时间；
- JVM 线程转储、内存转储详情和对应的文件下载；
- 可以查看和配置 Spring Boot 项目中的日志级别；
- 查看 Spring Boot 项目性能监控；
- 查看 Spring Boot 运行环境信息；
- 查看 Spring Boot 所有类信息；
- 查看 Spring Boot 中的定时任务；
- 查看和管理 Spring Boot 项目中的所有缓存。

以下是几个重要页面的截图，一起来看。
<a name="U0kcC"></a>
#### 5.3.1 查看运行环境
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755716034-9f42d392-d673-4e5c-a830-218d84614106.png#clientId=ubf6d0044-28d3-4&from=paste&id=u900a70e1&originHeight=889&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u881990a1-cd08-40dd-ae67-b6f4798fae7&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755716529-70ae9a15-f67b-40e5-be2c-0bffa8faea7e.png#clientId=ubf6d0044-28d3-4&from=paste&id=u4e9fd369&originHeight=889&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9d982aac-a0e3-48d4-981f-132765cbda5&title=)
<a name="Db1kt"></a>
#### 5.3.2 查看定时任务
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755716716-83c817f8-8e5e-4fdd-b051-0feb9ce3fdfa.png#clientId=ubf6d0044-28d3-4&from=paste&id=u7907f0bb&originHeight=889&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u14101911-cdc7-471e-8a9e-c1e1be8b44f&title=)
<a name="g9wVp"></a>
#### 5.3.3 项目日志级别配置
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755717011-ba736094-76c1-4445-9afc-314d9f2dbd03.png#clientId=ubf6d0044-28d3-4&from=paste&id=u8d06dd98&originHeight=889&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufed3b81c-67c0-4ccc-9701-90d84a91d18&title=)可以通过 Spring Boot Admin 来动态的配置项目中的日志级别。
<a name="kxUm5"></a>
#### 5.3.4 JVM线程和内存查看
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755717484-8cfa57ab-9901-44c3-a096-9f6a68da5483.png#clientId=ubf6d0044-28d3-4&from=paste&id=ue49517e6&originHeight=889&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1a0166a2-132d-4b4f-b9a1-453ace1d0a6&title=)
<a name="FKnEn"></a>
#### 5.3.5 查看SpringBoot所有缓存
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755717456-dcc58a28-7a7b-4de3-8d92-544ed5c65849.png#clientId=ubf6d0044-28d3-4&from=paste&id=ue2f7a49f&originHeight=889&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u734016b6-dcad-452f-9572-27bc2e55866&title=)当然还可以对这些缓存进行删除操作。
<a name="LUhNW"></a>
## 6、查看项目实时日志
想要查看监控项目中的日志信息，有一个前提条件，前提条件是被监控的 Spring Boot 项目，必须配置了日志的保存路径或者日志保存文件名，只有配置这两项中的任意一项，Spring Boot 项目才会将日志保存到磁盘上，这样才能通过 SBA 查看到，配置的是日志路径，在 Spring Boot 的 application.properties 配置文件中添加以下配置：
```
# 设置日志保存路径
logging.file.path=C:\\work\\log
```
设置完成之后，重启 Spring Boot 项目，然后刷新 SBA 页面，最终展示效果如下：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1647755717799-6aab6f61-2afd-4190-a88d-9d4ccceeef22.png#clientId=ubf6d0044-28d3-4&from=paste&id=uede5a513&originHeight=889&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud9f0e372-fd77-4f35-bef0-dc522e4b3a8&title=)此时就可以查看实时的日志信息了，当然也可以随时下载日志，如果需要的话。
<a name="QXxyd"></a>
## 总结
Spring Boot Admin（SBA）是一个社区开源项目，用于管理和监视 Spring Boot 应用程序，它提供详细的健康 （Health）信息、内存信息、JVM 系统和环境属性、垃圾回收信息、日志设置和查看、定时任务查看、Spring Boot 缓存查看和管理等功能。<br />需要创建一个 SBA 服务器端用来监控一个或多个 Spring Boot 项目，被监控的 Spring Boot 项目要添加 SBA Client 框架的支持，且添加 actuator 框架和相应的配置，就可以实现对 Spring Boot 项目的完美监控了。

