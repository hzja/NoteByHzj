日志 Nacos
<a name="VsPyw"></a>
## 1、安装 Nacos 配置中心
配置中心 Nacos 的官网<br />官网：[https://nacos.io/zh-cn/docs/quick-start.html](https://nacos.io/zh-cn/docs/quick-start.html)<br />下载安装包<br />[https://github.com/alibaba/nacos/releases](https://github.com/alibaba/nacos/releases)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621062856416-cab371d1-7df0-47ce-8acb-ee29219548f2.png#clientId=ucbd40ccf-702d-4&from=paste&height=599&id=u0b1caf75&originHeight=1798&originWidth=3840&originalType=binary&size=395306&status=done&style=none&taskId=u60045ebb-92a5-4e85-8a48-36042a37b88&width=1280)<br />需要注意一下 Nacos 需要 64 位操作系统和 64 位的JDK，如果不是 64 位的启动时会出现以下异常。
```java
 at com.alipay.sofa.jraft.core.NodeImpl.init(NodeImpl.java:138)
        at com.alipay.sofa.jraft.RaftServiceFactory.createAndInitRaftNode(RaftServiceFactory.java:47)
        at com.alipay.sofa.jraft.RaftGroupService.start(RaftGroupService.java:129)
        at com.alibaba.nacos.core.distributed.raft.JRaftServer.createMultiRaftGroup(JRaftServer.java:268)
        at com.alibaba.nacos.core.distributed.raft.JRaftProtocol.addRequestProcessors(JRaftProtocol.java:163)
        at com.alibaba.nacos.naming.core.v2.service.impl.PersistentClientOperationServiceImpl.<init>(PersistentClientOperationServiceImpl.java:92)
        at sun.reflect.NativeConstructorAccessorImpl.newInstance0(Native Method)
        at sun.reflect.NativeConstructorAccessorImpl.newInstance(NativeConstructorAccessorImpl.java:62)
        at sun.reflect.DelegatingConstructorAccessorImpl.newInstance(DelegatingConstructorAccessorImpl.java:45)
        at java.lang.reflect.Constructor.newInstance(Constructor.java:423)
        at org.springframework.beans.BeanUtils.instantiateClass(BeanUtils.java:175)
        ... 57 common frames omitted
Caused by: java.lang.UnsupportedOperationException: Cannot determine JNI library name for ARCH='x86' OS='windows 10' name='rocksdb'
        at org.rocksdb.util.Environment.getJniLibraryName(Environment.java:88)
        at org.rocksdb.NativeLibraryLoader.<clinit>(NativeLibraryLoader.java:19)
        ... 74 common frames omitted
```
启动命令
```bash
startup.cmd -m standalone
```
因为启动脚本默认使用集群模式启动，为了不需要使用命令还，改动一下启动脚本，修改如下<br />![2021-05-15-15-11-20-524133.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621062703821-6985e5ab-2074-4116-8334-1c82f581a38d.png#clientId=ucbd40ccf-702d-4&from=ui&id=uc3e73ed4&originHeight=494&originWidth=924&originalType=binary&size=52852&status=done&style=shadow&taskId=u98f25d2a-d2aa-4afb-a8d3-2169db3e745)<br />启动成功，显示如下日志
```java
2021-05-14 22:00:34,140 INFO Initializing ExecutorService 'taskScheduler'
2021-05-14 22:00:34,178 INFO Exposing 16 endpoint(s) beneath base path '/actuator'
2021-05-14 22:00:34,465 INFO Tomcat started on port(s): 8848 (http) with context path '/nacos'
2021-05-14 22:00:34,473 INFO Nacos started successfully in stand alone mode. use embedded storage
```
访问 Nacos 配置中心，对应的账号和密码 nacos / nacos<br />[http://192.168.10.6:8848/nacos/index.html](http://192.168.10.6:8848/nacos/index.html)
<a name="JGevK"></a>
## 2、在配置中心创建配置文件 dynamics-log.json
内容如下
```json
{
    "logger": [
        {
            "name":"ROOT",
            "level":"debug"
        }
    ]
}
```
![2021-05-15-15-11-20-641818.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621062719293-70b0e4ab-6bdc-4881-a85d-01898c45ad55.png#clientId=ucbd40ccf-702d-4&from=ui&id=u6795538d&originHeight=467&originWidth=1080&originalType=binary&size=38937&status=done&style=shadow&taskId=u5008f2a4-baf2-4642-8a6a-f64d95fc9e6)
<a name="O5xwY"></a>
## 3、pom.xml 文件引入依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.olive</groupId>
    <artifactId>valid-demo</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <packaging>jar</packaging>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.1.8.RELEASE</version>
        <relativePath /> 
    </parent>
    <name>valid-demo</name>
    <url>http://maven.apache.org</url>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>com.alibaba.nacos</groupId>
            <artifactId>nacos-client</artifactId>
            <version>0.5.0</version>
        </dependency>

    </dependencies>
</project>
```
主要多引入 Nacos 客户端（nacos-client ）依赖
<a name="rdPod"></a>
## 4、编译监听器
要达到动态的效果，一般有 PUSH 和 PULL 两种方式。Nacos 内部提供了监听器，只要实现相关方法就可以对不同的 data-id  进行监听。大致代码如下：
```java
import java.util.concurrent.Executor;

import javax.annotation.PostConstruct;

import org.springframework.context.annotation.Configuration;

import com.alibaba.nacos.api.NacosFactory;
import com.alibaba.nacos.api.config.ConfigService;
import com.alibaba.nacos.api.config.listener.Listener;
import com.alibaba.nacos.api.exception.NacosException;

@Configuration
public class LoggerDynamicsConfig {

    private String serverAddr = "127.0.0.1:";

    private String dataId = "dynamics-log.json";

    private String group = "DEFAULT_GROUP";

    private long timeoutMs = 50000L;

    @PostConstruct
    public void init() {
        try {
            ConfigService configService = NacosFactory.createConfigService(serverAddr);
            String configInfo = configService.getConfig(dataId, group, timeoutMs);
            System.out.println("configInfo = " + configInfo);   
            configService.addListener(dataId, group, new Listener() {
                @Override
                public void receiveConfigInfo(String configInfo) {
                    System.out.println("recieve configInfo : " + configInfo);
                }

                @Override
                public Executor getExecutor() {
                    return null;
                }
            });
        } catch (NacosException e) {
            e.printStackTrace();
        }
    }
}
```
启动服务，可以看到控制台打印如下日志，说明可以获取到 Nacos 配置中心的配置项。
```java
2021-05-14 22:13:56.265 [main] INFO  c.alibaba.nacos.client.identify.CredentialWatcher - [] [] [] No credential found
configInfo = {
    "logger": [
        {
            "name":"ROOT",
            "level":"debug"
        }
    ]
}
recieve configInfo : {
    "logger": [
        {
            "name":"ROOT",
            "level":"debug"
        }
    ]
}
2021-05-14 22:13:56.563 [main] INFO  o.s.scheduling.concurrent.ThreadPoolTaskExecutor - Initializing ExecutorService 'applicationTaskExecutor'
```
动态在 Nacos 配置中心修改一下，可以看到控制台打印如下日志，说明服务是可以动态监听到配置的改变的。
```java
recieve configInfo : {
    "logger": [
        {
            "name":"ROOT",
            "level":"debug"
        }, {
            "name":"com.spring",
            "level":"info"
        }
    ]
}
```
<a name="pQub5"></a>
## 5、动态日志改造说明

- 在 `receiveConfigInfo()` 方法中，把拿到的配置 json 串构造成一个对象数组，比如定义为 `LoggerConfig[]`。
- 基于 [ [SpringBoot 动态设置 logback 日志的级别](https://www.yuque.com/fcant/java/iuo3nc) ] 改造，把 `LoggerController` 的 `printAllLogger()` 方法改成一个获取所有日志对象的方法，比如
```java
public List<Logger> getAllLogger(){
     //TODO

}
```

- 对配置动态变化构造的 `LoggerConfig[]` 数组与调用 `getAllLogger()` 获取的所有日志对象进行遍历比较，进行动态更新。
