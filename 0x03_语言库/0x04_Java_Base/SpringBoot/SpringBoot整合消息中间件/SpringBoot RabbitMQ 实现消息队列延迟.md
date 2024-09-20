JavaSpringBootRabbitMQ
<a name="fFuZF"></a>
## 概述
要实现RabbitMQ的消息队列延迟功能，一般采用官方提供的 `rabbitmq_delayed_message_exchange`插件。但RabbitMQ版本必须是3.5.8以上才支持该插件，否则得用其**死信队列**功能。
<a name="WnBHE"></a>
## 安装RabbitMQ延迟插件
<a name="AlKcw"></a>
### 检查插件
使用`rabbitmq-plugins list`命令用于查看RabbitMQ安装的插件。
```bash
rabbitmq-plugins list
```
<a name="yEoYW"></a>
### 检查RabbitMQ插件安装情况
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682641372036-b470dbb2-c68d-42fc-a26c-8560ec067d4d.png#averageHue=%23191818&clientId=u2fd37859-337e-4&from=paste&id=u0c3a30a5&originHeight=660&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u321031c1-76a0-475c-a4cc-21a98885be6&title=)
<a name="rg6Xu"></a>
### 下载插件
如果没有安装插件，则直接访问官网进行下载<br />[https://www.rabbitmq.com/community-plugins.html](https://www.rabbitmq.com/community-plugins.html)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682641372116-e64b5443-357c-4401-970e-f45b8db9978d.png#averageHue=%23fcf8f6&clientId=u2fd37859-337e-4&from=paste&id=u81d393f0&originHeight=765&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6c8196e6-c980-4b36-9e7b-a6448fba397&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682641372095-62c4ba52-e712-4ad7-94f7-b4fbfad4c120.png#averageHue=%23fefdfd&clientId=u2fd37859-337e-4&from=paste&id=u7638ebe2&originHeight=458&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u74741adf-6eac-4040-81b1-7d4528f0186&title=)
<a name="URVxp"></a>
###  安装插件
下载后，将其拷贝到RabbitMQ安装目录的plugins目录；并进行解压，如：
```
E:\software\RabbitMQ Server\rabbitmq_server-3.11.13\plugins
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682641372027-38f4d29d-e1f7-494e-84e2-c76de7f044ce.png#averageHue=%23fcfcfb&clientId=u2fd37859-337e-4&from=paste&id=ua06063b5&originHeight=248&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u486a3f31-e9ae-406f-9de8-9382881224a&title=)<br />打开cmd命令行窗口，如果系统已经配置RabbitMQ环境变量，则直接执行以下的命令进行安装；否则需要进入到RabbitMQ安装目录的sbin目录。
```bash
rabbitmq-plugins enable rabbitmq_delayed_message_exchange
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682641371999-0b8bba27-f313-4302-b378-f09fe5b0706e.png#averageHue=%23161616&clientId=u2fd37859-337e-4&from=paste&id=u95958121&originHeight=278&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufa7be6ef-58aa-4f6e-9595-ea5a6ebfea3&title=)
<a name="RD7lJ"></a>
## 实现RabbitMQ消息队列延迟功能
<a name="PN7rj"></a>
###  pom.xml配置信息文件中，添加相关依赖文件
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.olive</groupId>
  <artifactId>rabbitmq-spring-demo</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.7.7</version>
    <relativePath />
  </parent>
  <dependencies>
    <!--rabbitmq-->
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-amqp</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-test</artifactId>
      <scope>test</scope>
    </dependency>

    <dependency>
      <groupId>org.eclipse.paho</groupId>
      <artifactId>org.eclipse.paho.client.mqttv3</artifactId>
      <version>1.2.5</version>
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
</project>
```
<a name="Brgna"></a>
### application.yml配置文件中配置RabbitMQ信息
```yaml
server:
  port: 8080
spring:
  #给项目来个名字
  application:
    name: rabbitmq-spring-demo
  #配置rabbitMq 服务器
  rabbitmq:
    host: 127.0.0.1
    port: 5672
    username: admin
    password: admin123
    #虚拟host。可以不设置,使用server默认host；不同虚拟路径下的队列是隔离的
    virtual-host: /
```
<a name="qxiRQ"></a>
### RabbitMQ配置类
```java
package com.olive.config;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.CustomExchange;
import org.springframework.amqp.core.Queue;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.util.HashMap;
import java.util.Map;

/**
 * RabbitMQ配置类
 **/
@Configuration
public class RabbitMqConfig {

    public static final String DELAY_EXCHANGE_NAME = "delayed_exchange";

    public static final String DELAY_QUEUE_NAME = "delay_queue_name";

    public static final String DELAY_ROUTING_KEY = "delay_routing_key";

    @Bean
    public CustomExchange delayExchange() {
        Map<String, Object> args = new HashMap<>();
        args.put("x-delayed-type", "direct");
        return new CustomExchange(DELAY_EXCHANGE_NAME, "x-delayed-message", true, false, args);
    }

    @Bean
    public Queue queue() {
        Queue queue = new Queue(DELAY_QUEUE_NAME, true);
        return queue;
    }

    @Bean
    public Binding binding(Queue queue, CustomExchange delayExchange) {
        return BindingBuilder.bind(queue).to(delayExchange).with(DELAY_ROUTING_KEY).noargs();
    }
}
```
<a name="D2vI4"></a>
### 发送消息
实现消息发送，设置消息延迟5s。
```java
package com.olive.service;

import java.text.SimpleDateFormat;
import java.util.Date;

import org.springframework.amqp.AmqpException;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessagePostProcessor;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.olive.config.RabbitMqConfig;

/**
 * 消息发送者
 **/
@Service
public class CustomMessageSender {

    @Autowired
    private RabbitTemplate rabbitTemplate;

    public void sendMsg(String msg) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        System.out.println("消息发送时间：" + sdf.format(new Date()));
        rabbitTemplate.convertAndSend(RabbitMqConfig.DELAY_EXCHANGE_NAME, 
                                      RabbitMqConfig.DELAY_ROUTING_KEY, 
                                      msg, 
                                      new MessagePostProcessor() {
                                          @Override
                                          public Message postProcessMessage(Message message) throws AmqpException {
                                              // 消息延迟5秒
                                              message.getMessageProperties().setHeader("x-delay", 5000);
                                              return message;
                                          }
                                      });
    }
}
```
<a name="j90jo"></a>
### 接收消息
```java
package com.olive.service;

import java.text.SimpleDateFormat;
import java.util.Date;

import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

import com.olive.config.RabbitMqConfig;

/**
 * 消息接收者
 **/
@Component
public class CustomMessageReceiver {

    @RabbitListener(queues = RabbitMqConfig.DELAY_QUEUE_NAME)
    public void receive(String msg) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        System.out.println(sdf.format(new Date()) + msg);
        System.out.println("Receiver：执行取消订单");
    }
}
```
<a name="V1cxx"></a>
### 测试验证
```java
package com.olive.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import com.olive.service.CustomMessageSender;

@RestController
public class DelayMessageController {

    @Autowired
    private CustomMessageSender customMessageSender;

    @GetMapping("/sendMessage")
    public String sendMessage() {
        // 发送消息
        customMessageSender.sendMsg("你已经支付超时，取消订单通知！");
        return "success";
    }

}
```
发送消息，访问http://127.0.0.1:8080/sendMessage<br />查看控制台打印的信息<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682641372611-56771100-c067-4d1b-bb41-400b698d5f44.png#averageHue=%23f9f4f3&clientId=u2fd37859-337e-4&from=paste&id=udff318ce&originHeight=211&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5729265e-48d9-44c2-b3c0-a0a9385a2ca&title=)
