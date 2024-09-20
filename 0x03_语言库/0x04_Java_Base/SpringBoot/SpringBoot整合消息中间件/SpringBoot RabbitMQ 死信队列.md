JavaSpringBootRabbitMQ
<a name="kOEph"></a>
## 死信定义
无法被消费的消息，称为**死信**。<br />如果死信一直留在队列中，会导致一直被消费，却从不消费成功，专门有一个存放死信的队列，称为死信队列(DDX, dead-letter-exchange)。
<a name="tw4UH"></a>
###  死信队列
DLX，Dead Letter Exchange的缩写，又死信邮箱、死信交换机。其实DLX就是一个普通的交换机，和一般的交换机没有任何区别。当消息在一个队列中变成死信（dead message）时，通过这个交换机将死信发送到死信队列中（指定好相关参数，RabbitMQ会自动发送）。
<a name="ibcvW"></a>
### 死信的几种来源

- 消息TTL过期(time to live，存活时间，可以用在限时支付消息)
- 队列达到最大长度(队列满了，无法路由到该队列)
- 消息被拒绝(basic.reject/basic.nack)，并且`requeue = false`

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682555431938-f48dd118-159d-4af6-ae1a-1e23a758db31.png#averageHue=%23faf9f9&clientId=ue1178611-3f1d-4&from=paste&id=u6b725fb0&originHeight=548&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5f960dc1-cdb4-4c0f-b3a1-83fc7c88512&title=)
<a name="BSmJi"></a>
## 创建项目

- pom.xml配置如下
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

- application.yml
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

- 准备MQ的队列和环境场景
<a name="D3zdw"></a>
### 正常交换机

1. 正常队列(最长队列5)；正常消费者，拒绝消息
2. tt队列(过期时间60秒)；没有消费者
<a name="sKYvz"></a>
### 死信交换机

1. 死信队列
```java
package com.olive.config;

import java.util.HashMap;
import java.util.Map;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.DirectExchange;
import org.springframework.amqp.core.Queue;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class DeadConfig {

    /************ 正常配置 ******************/
    /**
      * 正常交换机，开启持久化
      */
    @Bean
    DirectExchange normalExchange() {
        return new DirectExchange("normalExchange", true, false);
    }

    @Bean
    public Queue normalQueue() {
        // durable: 是否持久化,默认是false,持久化队列：会被存储在磁盘上，当消息代理重启时仍然存在，暂存队列：当前连接有效
        // exclusive: 默认也是false，只能被当前创建的连接使用，而且当连接关闭后队列即被删除。此参考优先级高于durable
        // autoDelete: 是否自动删除，当没有生产者或者消费者使用此队列，该队列会自动删除。
        Map<String, Object> args = deadQueueArgs();
        // 队列设置最大长度
        args.put("x-max-length", 5);
        return new Queue("normalQueue", true, false, false, args);
    }

    @Bean
    public Queue ttlQueue() {
        Map<String, Object> args = deadQueueArgs();
        // 队列设置消息过期时间 60 秒
        args.put("x-message-ttl", 60 * 1000);
        return new Queue("ttlQueue", true, false, false, args);
    }

    @Bean
    Binding normalRouteBinding() {
        return BindingBuilder.bind(normalQueue())
        .to(normalExchange())
        .with("normalRouting");
    }

    @Bean
    Binding ttlRouteBinding() {
        return BindingBuilder.bind(ttlQueue())
        .to(normalExchange())
        .with("ttlRouting");
    }

    /**************** 死信配置 *****************/
    /**
     * 死信交换机
     */
    @Bean
    DirectExchange deadExchange() {
        return new DirectExchange("deadExchange", true, false);
    }

    /**
     * 死信队列
     */
    @Bean
    public Queue deadQueue() {
        return new Queue("deadQueue", true, false, false);
    }

    @Bean
    Binding deadRouteBinding() {
        return BindingBuilder.bind(deadQueue())
        .to(deadExchange())
        .with("deadRouting");
    }

    /**
     * 转发到 死信队列，配置参数
     */
    private Map<String, Object> deadQueueArgs() {
        Map<String, Object> map = new HashMap<>();
        // 绑定该队列到死信交换机
        map.put("x-dead-letter-exchange", "deadExchange");
        map.put("x-dead-letter-routing-key", "deadRouting");
        return map;
    }

}
```
arguments参数说明:<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682555432026-4cbcba29-8736-4a24-848a-d42eabdaa010.png#averageHue=%23f5f3f3&clientId=ue1178611-3f1d-4&from=paste&id=u7c89cd09&originHeight=648&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6474ec01-8709-455a-8ceb-d861ccfd366&title=)

1. Auto expire：队列在被自动删除之前可以使用多长时间(毫秒)。(x-expires参数)
2. Message TTL：发布到队列的消息在被丢弃之前可以存在多长时间(毫秒)。(x-message-ttl参数)
3. Overflow behaviour：设置队列溢出行为。这决定了当达到队列的最大长度时消息会发生什么。有效值为drop-head（删除queue头部的消息）、reject-publish（最近发来的消息将被丢弃）或reject-publish-dlx（拒绝发送消息到死信交换器）。仲裁队列类型只支持drop-head和拒绝-发布。(x-overflow参数)
4. Single active consumer：如果设置，确保每次只从队列中使用一个使用者，并在活动使用者被取消或死x-dead-letter-exchange亡的情况下故障转移到另一个注册使用者。(x-single-active-consumer参数)
5. Dead letter exchange：一个可选的死信交换机，如果消息被拒绝或过期，将重新发布到死信交换机。(x-dead-letter-exchange参数)
6. Dead letter routing key：当消息是死信时使用的可选替换路由键。如果未设置此值，则将使用消息的原始路由键。(x-dead-letter-routing-key参数)
7. Max length：一个队列在开始从头中丢弃消息之前可以包含多少(准备好的)消息。(x-max-length参数)
8. Max length bytes：队列在开始从头部丢弃消息之前所能包含的就绪消息的总正文大小。(x-max-length-bytes参数)
9. Leader locator：将队列设置为主位置模式，确定在节点集群上声明时队列主机所在的规则。(x-queue-leader-locator参数)
<a name="b60x0"></a>
## 队列达到的最大长度
消息没有消费者；调用6次正常队列的消息生产方法；消息数量超过队列长度。
```java
package com.olive.controller;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import org.springframework.amqp.rabbit.connection.CorrelationData;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class TestController {

    @Autowired
    private RabbitTemplate rabbitTemplate;

    /**
     * 正常消息队列，队列最大长度5
     */
    @GetMapping("/normalQueue")
    public String normalQueue() {
        Map<String, Object> map = new HashMap<>();
        map.put("messageId", String.valueOf(UUID.randomUUID()));
        map.put("data", System.currentTimeMillis() + ", 正常队列消息，最大长度5");
        rabbitTemplate.convertAndSend("normalExchange", "normalRouting", map, new CorrelationData());
        return "success";
    }
}
```
访问6次，发送6条消息
```
http://127.0.0.1:8080/normalQueue
```
从RabbitMQ管理后台查看结果:<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682555431944-025a4e9d-4948-4e76-ba04-d4a0b22cb47c.png#averageHue=%23f7f2f1&clientId=ue1178611-3f1d-4&from=paste&id=u6650f83a&originHeight=605&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8526df35-3e10-4a6a-a328-54975fb04c1&title=)
<a name="MxLFV"></a>
## 消息TTL过期
消息的TTL指的是消息存活时间，可以通过设置消息TTL或者队列的TTL来实现。

- 消息的TTL：对于设置了过期时间属性(expiration)的消息，消息如果在过期时间内没被消费，会过期。
- 队列的TTL：对于设置了过期时间属性(x-message-ttl)的队列，所有路由到这个队列的消息，都会设置上这个过期时间。

使用者两种配置都可以，一般都用在定时任务，限时支付这种场景。
```java
/**
 * 消息 TTL, time to live
 */
@GetMapping("/ttlToDead")
public String ttlToDead() {
    Map<String, Object> map = new HashMap<>();
    map.put("messageId", String.valueOf(UUID.randomUUID()));
    map.put("data", System.currentTimeMillis() + ", ttl队列消息");
    rabbitTemplate.convertAndSend("normalExchange", "ttlRouting", map, new CorrelationData());
    return "success";
}
```
访问6次，发送6条消息
```
http://127.0.0.1:8080/ttlToDead
```
从RabbitMQ管理后台查看结果，发送消息后<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682555431945-12e0c52f-5a11-4bf1-86a5-6ddf9dabe308.png#averageHue=%23f6f3f2&clientId=ue1178611-3f1d-4&from=paste&id=ud2d7c6d6&originHeight=597&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5f876b84-4b85-499c-a3cd-da8ab580e68&title=)<br />从RabbitMQ管理后台查看结果，等待消息过期后<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682555431854-8c24b9c6-a149-46f0-bb6e-114f1968feb4.png#averageHue=%23f8f5f4&clientId=ue1178611-3f1d-4&from=paste&id=uf530b297&originHeight=566&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u93898b18-fba7-48fa-a1b0-762b1f23fd5&title=)<br />建议在项目中尽量使用**消息TTL**，不使**用队列TTL**
<a name="o1RYQ"></a>
## 拒绝消息
正常队列消费后拒绝消息，并且不进行重新入队
```java
package com.olive.config;

import java.io.IOException;
import java.util.Map;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitHandler;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

import com.rabbitmq.client.Channel;

@Component
@RabbitListener(queues = "normalQueue")
public class NormalConsumer {

    @RabbitHandler
    public void process(Map<String, Object> message, Channel channel, Message mqMsg) 
    throws IOException {
        System.out.println("收到消息，并拒绝重新入队:" + message.toString());
        channel.basicReject(mqMsg.getMessageProperties().getDeliveryTag(), false);
    }

}
```
从RabbitMQ管理后台查看结果<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682555432529-e6774847-e0fd-47d0-8e96-f2a7c4808fda.png#averageHue=%23f7f3f2&clientId=ue1178611-3f1d-4&from=paste&id=u25115965&originHeight=664&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud412c2bf-d325-4de4-b35f-2df75588c63&title=)
<a name="DRxbn"></a>
## 死信队列消息消费
```java
package com.olive.config;

import java.io.IOException;
import java.util.Map;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitHandler;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

import com.rabbitmq.client.Channel;

@Component
@RabbitListener(queues = "deadQueue")
public class DeadConsumer {

    @RabbitHandler
    public void process(Map<String, Object> message, Channel channel, Message mqMsg) 
    throws IOException {
        System.out.println("死信队列收到消息:" + message.toString());
        channel.basicAck(mqMsg.getMessageProperties().getDeliveryTag(), false);
    }
}
```
从RabbitMQ管理后台查看结果，死信队列消息被完全消费<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682555432600-b0d61a60-a799-42d9-b33e-b6f7c8c6498e.png#averageHue=%23f7f4f4&clientId=ue1178611-3f1d-4&from=paste&id=u043cfa58&originHeight=621&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u03f4766c-d916-4d2c-abab-bb849dbb2b8&title=)
