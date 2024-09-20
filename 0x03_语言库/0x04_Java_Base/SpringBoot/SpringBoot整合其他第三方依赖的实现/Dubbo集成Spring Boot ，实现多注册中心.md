Java SpringBoot Dubbo<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114004-6a6c6481-a622-4481-8f45-db0f8c5062e7.webp#clientId=u9c79b779-a240-4&from=paste&id=ua7af43d5&originHeight=648&originWidth=926&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc102fb34-ddd4-4abd-8793-c0016544971&title=)
<a name="DoIIQ"></a>
## 多注册中心
Dubbo目前差不多能支持市面上所有的注册中心：

- Consul
- Zookeeper
- Eureka
- Redis
- Nacos
- ....

其实，在实际开发中，Dubbo注册中心大部分都是使用Zookeeper和Nacos。<br />所以，本次分享就是使用Zookeeper和Nacos进行演示。
<a name="Ga7Pp"></a>
### Nacos准备
先从官网把Nacos下载到本地，然后在进行安装。<br />官网介绍：[https://nacos.io/zh-cn/docs/what-is-nacos.html](https://nacos.io/zh-cn/docs/what-is-nacos.html)<br />下载地址：[https://github.com/alibaba/nacos/releases/tag/1.4.0](https://github.com/alibaba/nacos/releases/tag/1.4.0)
<a name="DIO0X"></a>
##### 运行模式
Nacos Server 有两种运行模式：

- standalone 单机模式
- cluster 集群模式

这里采用的是standalone模式<br />如果不是部署Nacos集群，则将模式修改为单机模式<br />打开`bin/startup.cmd`文件，将MODE从cluster改为 standalone，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918113945-12fb3736-218c-4d16-a37f-d93cece4fd81.webp#clientId=u9c79b779-a240-4&from=paste&id=u8a767202&originHeight=482&originWidth=720&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u986d31c1-337b-46c9-88f1-1507d84eaaa&title=)
<a name="VO12E"></a>
##### 数据存储
把服务注册到Nacos上，服务相关信息需要保存。<br />保存在哪里呢？Nacos提供了两种方式：

- 本地内存（默认）
- 数据库

在Nacos的安装目录下/conf下面有个配置文件：application.properties，这个配置文件里就可以做相应的调整。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114051-31862ae4-1217-451c-b1e1-22967cfbaff9.webp#clientId=u9c79b779-a240-4&from=paste&id=u3242c16b&originHeight=476&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u859dabfb-0373-4a61-8c25-36398020331&title=)<br />这里是配置了数据库，默认是没有配置数据库的，另外还需要把数据库脚本执行，也在本目录下：nacos-mysql.sql<br />最后，把数据库连接信息改成自己数据库连接信息即可。<br />另外，想深入学习Nacos的，这个数据库脚本里基本上都是中文注释，帮助还是蛮大的。
<a name="jQJfH"></a>
##### 启动Nacos
Linux环境下：`sh bin/startup.sh`<br />Windows环境下：`cmd bin/startup.cmd`、或者直接双击`startup.cmd`即可。<br />基于Windows环境启动日志如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918113953-d7ae29d8-c79b-438f-affb-f2f108aac862.webp#clientId=u9c79b779-a240-4&from=paste&id=ub6dd6a96&originHeight=515&originWidth=645&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub3e4d005-b10e-4c34-8186-10048cf7cd5&title=)<br />访问：http://localhost:8848/nacos/index.html<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918113968-b6b1b2cb-f221-4a21-b403-6ae5e783ab1d.webp#clientId=u9c79b779-a240-4&from=paste&id=u283b1722&originHeight=714&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u183b8345-94e7-4c2b-aae3-3cc6ece2cc2&title=)<br />输入用户名/密码访问 nacos/nacos<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114501-a0172a24-24be-4eb2-94ba-15a1ca9d8a41.webp#clientId=u9c79b779-a240-4&from=paste&id=ud282d3e1&originHeight=331&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u865a64b7-a29d-414e-bf9f-34a9e57aa83&title=)<br />好了，说完Nacos的安装，暂时先告一段落，继续。
<a name="A1xWn"></a>
## 集成Spring Boot
整体项目如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114461-bf1b89dc-8c8a-41e0-9be6-e67a1989a9fa.webp#clientId=u9c79b779-a240-4&from=paste&id=u2a5d328a&originHeight=776&originWidth=471&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u42d6442f-0977-43aa-9440-56a7e643f43&title=)<br />父pom.xml内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.tian.dubbo</groupId>
    <artifactId>spring-boot-dubbo-demo</artifactId>
    <packaging>pom</packaging>
    <version>1.0-SNAPSHOT</version>
    <modules>
        <module>spring-boot-dubbo-demo-api</module>
        <module>spring-boot-dubbo-demo-provider</module>
        <module>spring-boot-dubbo-demo-consumer</module>
    </modules>

    <properties>
        <spring-boot.version>2.2.1.RELEASE</spring-boot.version>
    </properties>

    <dependencyManagement>
        <dependencies>
            <dependency>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-dependencies</artifactId>
                <version>${spring-boot.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
        </dependencies>
    </dependencyManagement>
</project>
```
<a name="IeVVU"></a>
### spring-boot-dubbo-demo-api
API项目中就简单的是一个接口类：
```java
package com.tian.dubbo.service;

public interface DemoService {
    String sayHello(String msg);
}
```
<a name="GSAqd"></a>
### spring-boot-dubbo-demo-provider
服务提供者项目中的代码解释如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <parent>
        <artifactId>spring-boot-dubbo-demo</artifactId>
        <groupId>com.tian.dubbo</groupId>
        <version>1.0-SNAPSHOT</version>
    </parent>
    <modelVersion>4.0.0</modelVersion>

    <artifactId>spring-boot-dubbo-demo-provider</artifactId>

    <properties>
        <dubbo.version>3.0.4</dubbo.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.apache.dubbo</groupId>
            <artifactId>dubbo-spring-boot-starter</artifactId>
            <version>3.0.2.1</version>
        </dependency>
        <dependency>
            <groupId>org.apache.dubbo</groupId>
            <artifactId>dubbo-dependencies-zookeeper</artifactId>
            <version>3.0.2.1</version>
            <type>pom</type>
            <exclusions>
                <exclusion>
                    <groupId>org.slf4j</groupId>
                    <artifactId>slf4j-log4j12</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>com.alibaba.nacos</groupId>
            <artifactId>nacos-client</artifactId>
            <version>2.0.2</version>
        </dependency>
        <dependency>
            <groupId>com.tian.dubbo</groupId>
            <artifactId>spring-boot-dubbo-demo-api</artifactId>
            <version>0.0.1-SNAPSHOT</version>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter</artifactId>
        </dependency>
    </dependencies>

</project>
```
集成了Zookeeper、Nacos以及相关starter。
<a name="kKrtl"></a>
#### application.properties配置项
application.properties配置内容：
```
## 随机端口
server.port=-1
spring.application.name=spring-boot-dubbo-demo-provider


dubbo.protocol.name=dubbo
dubbo.protocol.port=-1
## dubbo.registries.name.address
## name可以理解为注册中心名字
dubbo.registries.shanghai.address=zookeeper://127.0.0.1:2181
dubbo.registries.shanghai.timeout=10000 

dubbo.registries.beijing.address=nacos://127.0.0.1:8848
```
把Zookeeper和Nacos地址配上了。
<a name="uX5ov"></a>
#### DemoServiceImpl业务实现类
```java
package com.tian.dubbo.service;

import org.apache.dubbo.config.annotation.DubboService;

@DubboService
public class DemoServiceImpl implements DemoService {
    public String sayHello(String msg) {
        System.out.println("provider receive msg : " + msg);
        return "provider return " + msg;
    }
}
```
在Spring相关项目中，一个通常都是使用注解`@Service`表示这个是一个Service Bean，于是Dubbo也搞了一套，但此时使用的是注解`@DubboService`。<br />如果这个Service只想注册到一个注册中心，可以使用：
```java
@DubboService(registry = {"beijing"})
```
关于`@DubboService`注解，其中定义了一下内容：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114466-7f4b3c26-c1ab-4c5b-88de-2cb1b02a0ce8.webp#clientId=u9c79b779-a240-4&from=paste&id=uc7c25563&originHeight=738&originWidth=479&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u012c1fcf-64e3-4a2c-83b1-fd48b3e1f17&title=)<br />`@DubboService`

- version 服务版本号
- timeout 超时时间
- retries重试次数
- registry注册中心
- ...
<a name="QXtdy"></a>
#### ProviderMain项目启动类
```java
package com.tian.dubbo;

import org.apache.dubbo.config.spring.context.annotation.DubboComponentScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@DubboComponentScan(basePackages = "com.tian.dubbo.service")
@SpringBootApplication
public class ProviderMain {

    public static void main(String[] args) {
        SpringApplication.run(ProviderMain.class, args);
    }

}
```
上面，已经给给业务实现类加上了注解`@DubboService`，和Spring一样的，都是需要进行扫描`ComponentScan`。
<a name="MkiEq"></a>
#### 启动provider项目
项目启动好了后，回到Nacos注册中心：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114532-0aaa9d50-57d6-426a-bbbe-557475e4632f.webp#clientId=u9c79b779-a240-4&from=paste&id=u1b0a230b&originHeight=244&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u74e754e4-6062-4b0a-b9af-153f3b6ec81&title=)<br />证明项目已经注册到Nacos上了.<br />顺便去看看`providers:com.tian.dubbo.service.DemoService::`详情。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114499-b6e4417b-0bed-47ad-a152-3e8af2d72d30.webp#clientId=u9c79b779-a240-4&from=paste&id=u0c7c1251&originHeight=360&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u751a365e-aa42-40f9-b84c-63cd2f65447&title=)<br />再看看Zookeeper注册中心：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114787-372ba06f-4497-4624-b993-02374ae466fb.webp#clientId=u9c79b779-a240-4&from=paste&id=u6647683d&originHeight=361&originWidth=510&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u63a58bf6-c6a1-456a-a5f5-c2fb0fd05ba&title=)<br />从图中可以看出，服务已经成功注册上去了。<br />到这里，服务提供者已经把服务注册到注册中心上了。<br />再来看看消费者。
<a name="nQV28"></a>
### spring-boot-dubbo-demo-consumer
pom.xml和服务提供者差不多：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <parent>
        <artifactId>spring-boot-dubbo-demo</artifactId>
        <groupId>com.tian.dubbo</groupId>
        <version>1.0-SNAPSHOT</version>
    </parent>
    <modelVersion>4.0.0</modelVersion>

    <artifactId>spring-boot-dubbo-demo-consumer</artifactId>

    <dependencies>
        <dependency>
            <groupId>org.apache.dubbo</groupId>
            <artifactId>dubbo-spring-boot-starter</artifactId>
            <version>3.0.2.1</version>
        </dependency>
        <dependency>
            <groupId>org.apache.dubbo</groupId>
            <artifactId>dubbo-dependencies-zookeeper</artifactId>
            <version>3.0.2.1</version>
            <type>pom</type>
            <exclusions>
                <exclusion>
                    <groupId>org.slf4j</groupId>
                    <artifactId>slf4j-log4j12</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>com.alibaba.nacos</groupId>
            <artifactId>nacos-client</artifactId>
            <version>2.0.2</version>
        </dependency>
        <dependency>
            <groupId>com.tian.dubbo</groupId>
            <artifactId>spring-boot-dubbo-demo-api</artifactId>
            <version>0.0.1-SNAPSHOT</version>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
    </dependencies>
</project>
```
多了一个web的starter。
<a name="l30KH"></a>
#### application.properties配置文件
```
spring.application.name=spring-boot-dubbo-sample-consumer


dubbo.registries.shanghai.address=zookeeper://127.0.0.1:2181
dubbo.registries.shanghai.timeout=10000 

dubbo.registries.beijing.address=nacos://127.0.0.1:8848

server.port=20882
```
也配置两个注册中心。
<a name="ovXof"></a>
#### controller测试类
在消费者这边写一个Controller：
```java
package com.tian.dubbo.controller;


import com.tian.dubbo.service.DemoService;
import org.apache.dubbo.config.annotation.DubboReference;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class DemoController {

    @DubboReference(registry = {"shanghai","beijing"})
    private DemoService demoService;

    @GetMapping("/sayHello")
    public String sayHello(){
        return demoService.sayHello("tian");
    }

}
```
在Spring项目中，要注入一个Service通常都是使用注解`@Autowired`或者`@Service`，但在使用Dubbo服务注入的时候，使用的注解是`@DubboReference`。<br />并且，`@DubboService`类似的也可以指定注册中心是一个或者多个。<br />关于`@DubboReference`注解定义的方法：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1637918114898-55a206b5-fdd6-463e-8ff7-590e3b88e52b.webp#clientId=u9c79b779-a240-4&from=paste&id=ud06ccd97&originHeight=772&originWidth=424&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5d1ecd93-20fc-41b9-a911-eab5f04679d&title=)<br />`@DubboReference`<br />具体这些是用来干什么，其实可以从命名上基本就能猜到。比如：

- version就是用来做服务版本升级用
- check就是服务启动时是否需要检查服务是否可用
- url基本上是用于开发联调时候用的，绕过注册中心，直连服务
- timaout 超时时间设置
- registry注册中心
- ...

官网都是中文文档，所以这里就不在一一介绍了。
<a name="LOqoB"></a>
#### 启动类
```java
package com.tian.dubbo;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class ConsumerMain {
    public static void main(String[] args) {
        SpringApplication.run(ConsumerMain.class, args);
    }
}
```
启动类就没什么特别，和普通Spring项目启动一样。<br />下面，来验证一下。
<a name="G6Maq"></a>
#### 验证
访问：http://localhost:20882/sayHello<br />返回：provider return tian<br />证明消费者已经成功的调用到服务并返回了正确的信息。
<a name="osvrN"></a>
## 简单总结
关于Dubbo配置文件内容，只是简单的配置了注册中心信息。其实还可以配置默认注册中心、权重等信息，相关的配置建议看官网即可。<br />官方文档地址：[https://dubbo.apache.org/zh/docs/advanced/multi-registry/](https://dubbo.apache.org/zh/docs/advanced/multi-registry/)<br />Dubbo集成Spring Boot实现多注册中西做一个大致的总结，步骤不如下：

- Nacos和Zookeeper注册中心的搭建
- 创建一个api项目，打成jar包
- 创建服务提供者项目依赖api的jar包
- 引入相应的Nacos和Zookeeper依赖jar包
- 消费者和提供者都添加spring-boot-starter的依赖
- 消费者和提供者都添加dubbo-spring-boot-starter依赖
- 消费者和提供者都添加application.properties配置文件并配置项目配置项
- 提供者中业务实现类Service中需要添加注解`@DubboServie`
- 消费者需要注入Service，使用注解`@DubboReference`
