JavaSpringCloud StreamRabbitMQ
<a name="e9B6p"></a>
## 概述
SpringCloud Stream框架抽象出了三个最基础的概念来对各种消息中间件提供统一调用：

- Destination Binders：负责集成外部消息系统的组件。
- Destination Binding：由Binders创建的，负责沟通外部消息系统、消息发送者和消息消费者的桥梁。
- Message：消息发送者与消息消费者沟通的简单数据结构。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682738642600-85474d31-c1cd-4ad2-bce2-44d699b3980d.png#averageHue=%23f5f5f5&clientId=ub489f120-901b-4&from=paste&id=u51e6fb53&originHeight=665&originWidth=760&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf54152d0-5c04-431e-8a89-b4a7cdb6845&title=)
<a name="VT0aO"></a>
## 创建生产者项目
创建项目rabbitmq-stream-sender
<a name="FMxOE"></a>
### pom.xml添加依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.olive</groupId>
  <artifactId>rabbitmq-stream-sender</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.7.7</version>
    <relativePath />
  </parent>
  <dependencies>
    <!--rabbitMQ相关-->
    <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-stream</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-starter-stream-rabbit</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-stream-binder-rabbit</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
  </dependencies>
  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-compiler-plugin</artifactId>
        <configuration>
          <source>1.8</source>
          <target>1.8</target>
        </configuration>
      </plugin>
    </plugins>
  </build>
  <dependencyManagement>
    <dependencies>
      <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-dependencies</artifactId>
        <version>2021.0.6</version>
        <type>pom</type>
        <scope>import</scope>
      </dependency>
    </dependencies>
  </dependencyManagement>
</project>
```
<a name="w5xEz"></a>
### application.yml 配置
```yaml
spring:
  cloud:
    stream:
      bindings:
        output:
          destination: rabbitmqExchange
          content-type: text/plain
          group: stream
  rabbitmq:
    username: admin
    password: admin123
    port: 5672
    host: 127.0.0.1
    virtual-host: /

server:
  port: 8081
```
<a name="wEhrL"></a>
### 生产者
```java
package com.olive.service;

import org.springframework.cloud.stream.annotation.EnableBinding;
import org.springframework.cloud.stream.messaging.Source;
import org.springframework.stereotype.Component;

@Component
@EnableBinding(Source.class)
public class MessageProducer {

}
```
<a name="zOm3N"></a>
### 发送消息接口
```java
package com.olive.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.cloud.stream.messaging.Source;
import org.springframework.integration.support.MessageBuilder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class MessageController {

    @Autowired
    private Source source;

    @GetMapping("/api/send")
    public String send(String message) {
        MessageBuilder<String> messageBuilder = MessageBuilder.withPayload(message);
        source.output().send(messageBuilder.build());
        return "success" ;
    }
}
```
<a name="cYYsx"></a>
## 创建消费者者项目
创建项目rabbitmq-stream-revicer
<a name="TyW9x"></a>
### pom.xml添加依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.olive</groupId>
  <artifactId>rabbitmq-stream-revicer</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.7.7</version>
    <relativePath />
  </parent>
  <dependencies>
    <!--rabbitMQ相关-->
    <dependency>
      <groupId>org.springframework.cloud</groupId>
      <artifactId>spring-cloud-starter-stream-rabbit</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
  </dependencies>
  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-compiler-plugin</artifactId>
        <configuration>
          <source>1.8</source>
          <target>1.8</target>
        </configuration>
      </plugin>
    </plugins>
  </build>
  <dependencyManagement>
    <dependencies>
      <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-dependencies</artifactId>
        <version>2021.0.6</version>
        <type>pom</type>
        <scope>import</scope>
      </dependency>
    </dependencies>
  </dependencyManagement>
</project>
```
<a name="fqcSs"></a>
### application.yml 配置
```yaml
spring:
  cloud:
    stream:
      bindings:
        input:
          destination: rabbitmqExchange
          content-type: text/plain
          group: stream
  rabbitmq:
    username: admin
    password: admin123
    port: 5672
    host: 127.0.0.1
    virtual-host: /

server:
  port: 8082
```
<a name="f1Jri"></a>
### 消费者
```java
package com.olive.service;

import org.springframework.cloud.stream.annotation.EnableBinding;
import org.springframework.cloud.stream.annotation.StreamListener;
import org.springframework.cloud.stream.messaging.Sink;
import org.springframework.stereotype.Component;

@Component
@EnableBinding(Sink.class)
public class MessageConsumer {

    @StreamListener(Sink.INPUT)
    public void process(Object message) {
        System.out.println("received message: " + message);
    }

}
```
<a name="ehaLm"></a>
## 验证
分别启动rabbitmq-stream-sender和rabbitmq-stream-revicer项目<br />访问http://127.0.0.1:8081/api/send?message=hello world stream<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682738642561-24b07674-ff7e-4665-b73b-1658644c228a.png#averageHue=%23f8f7f5&clientId=ub489f120-901b-4&from=paste&id=uc9673403&originHeight=296&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub76f3759-6b18-40f9-b514-af3dd7afb93&title=)
