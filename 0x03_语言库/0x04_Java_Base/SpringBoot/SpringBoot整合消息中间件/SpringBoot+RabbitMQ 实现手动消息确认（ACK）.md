JavaSpringBootRabbitMQ
<a name="qnwmJ"></a>
## 一、前言
如何确保消息成功被消费者消费？<br />说到这里，相信很多人会说使用ack，关闭队列自动删除啊什么的。主要是道理大家都懂，要实际的代码，网上找了半天，和设想的有很大差异，还是自己做研究总结吧。
<a name="qGjpm"></a>
## 二、准备
本次写案例，就按照最简单的方式，direct方式进行配置吧，实际流程如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800107-895a24f2-a831-4764-9c53-9e5730b1ab6e.png#clientId=u0b4ddc91-a78f-4&from=paste&id=u97e1d9c4&originHeight=175&originWidth=845&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2de70d28-39b4-48cb-96d8-436c618019a&title=)

- **消息转发器类型： direct直连方式。**
- **消息队列： 暂时采取公平分发方式。**
- **实现流程： 消息生产者生产的消息发送至队列中，由两个消费者获取并消费，消费完成后，清楚消息队列中的消息。**

所以接下来先写配置和demo。
<a name="HPt8E"></a>
### **2.1、依赖引入**
再一般的SpringBoot2.1.4项目中，添加一个pom依赖。
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-amqp</artifactId>
</dependency>
```
<a name="zWefu"></a>
### **2.2、连接yml的配置**
这边暂时只有一个RabbitMQ，所以连接操作，基本RabbitMQ的信息配置问题直接再yml中编写就可以了。
```yaml
spring:
  rabbitmq:
    host: 127.0.0.1
    port: 5672
    username: xiangjiao
    password: bunana
    virtual-host: /xiangjiao
    publisher-confirms: true   #开启发送确认
    publisher-returns: true  #开启发送失败回退
    
    #开启ack
    listener:
      direct:
        acknowledge-mode: manual
      simple:
        acknowledge-mode: manual #采取手动应答
        #concurrency: 1 # 指定最小的消费者数量
        #max-concurrency: 1 #指定最大的消费者数量
        retry:
          enabled: true # 是否支持重试
```
<a name="SGR8K"></a>
### **2.3、config注入配置**
根据图示<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800203-57c53d38-15a5-4f9d-ab41-038e346022cb.png#clientId=u0b4ddc91-a78f-4&from=paste&id=u830dc2f6&originHeight=175&originWidth=845&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue7447e8d-d1ca-48bb-b623-f5a7f213f97&title=)必须配置以下东西：

- **一个消息转发器，取名**`**directExchangeTx**`**。**
- **一个消息队列，取名**`**directQueueTx**`**，并将其绑定至指定的消息转发器上。**

所以配置文件需要这么写：
```java
import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.DirectExchange;
import org.springframework.amqp.core.Queue;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
* 直连交换机，发送指定队列信息，但这个队列后有两个消费者同时进行消费
*
*/
@Configuration
public class DirectExchangeTxQueueConfig {

    @Bean(name="getDirectExchangeTx")
    public DirectExchange getDirectExchangeTx(){
        return new DirectExchange("directExchangeTx", true, false);
    }

    @Bean(name="getQueueTx")
    public Queue getQueueTx(){
        return new Queue("directQueueTx", true, false, false);
    }

    @Bean
    public Binding getDirectExchangeQueueTx(
        @Qualifier(value="getDirectExchangeTx") DirectExchange getDirectExchangeTx,
        @Qualifier(value="getQueueTx") Queue getQueueTx){
        return BindingBuilder.bind(getQueueTx).to(getDirectExchangeTx).with("directQueueTxRoutingKey");
    }
}
```
<a name="qVjiW"></a>
### **2.4、消费者的配置**
有了队列和消息转发器，消息当然需要去消费啊，所以接下来配置消息消费者。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800189-f44dd9d7-90a6-4ad1-b291-b0a314069288.png#clientId=u0b4ddc91-a78f-4&from=paste&id=u1e2dac15&originHeight=175&originWidth=845&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6e1b42fa-c021-499f-a4e2-23a89f14c29&title=)从图中，可以看出，需要配置两个消息消费者，同时监听一个队列，所以配置类为：<br />消费者一：
```java
import java.io.IOException;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitHandler;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;
import com.rabbitmq.client.Channel;

@Component
@RabbitListener(queues="directQueueTx")
public class Consumer1 {
    @RabbitHandler
    public void process(String msg,Channel channel, Message message) throws IOException {
        //拿到消息延迟消费
        try {
            Thread.sleep(1000*1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }


        try {
            /**
            * 确认一条消息：<br>
            * channel.basicAck(deliveryTag, false); <br>
            * deliveryTag:该消息的index <br>
            * multiple：是否批量.true:将一次性ack所有小于deliveryTag的消息 <br>
            */
            channel.basicAck(message.getMessageProperties().getDeliveryTag(), false);
            System.out.println("get msg1 success msg = "+msg);

        } catch (Exception e) {
            //消费者处理出了问题，需要告诉队列信息消费失败
            /**
            * 拒绝确认消息:<br>
            * channel.basicNack(long deliveryTag, boolean multiple, boolean requeue) ; <br>
            * deliveryTag:该消息的index<br>
            * multiple：是否批量.true:将一次性拒绝所有小于deliveryTag的消息。<br>
            * requeue：被拒绝的是否重新入队列 <br>
            */
            channel.basicNack(message.getMessageProperties().getDeliveryTag(),
                              false, true);
            System.err.println("get msg1 failed msg = "+msg);

            /**
            * 拒绝一条消息：<br>
            * channel.basicReject(long deliveryTag, boolean requeue);<br>
            * deliveryTag:该消息的index<br>
            * requeue：被拒绝的是否重新入队列 
            */
            //channel.basicReject(message.getMessageProperties().getDeliveryTag(), true);
        }
    }
}
```
消息消费者二：
```java
import java.io.IOException;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitHandler;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;
import com.rabbitmq.client.Channel;

@Component
@RabbitListener(queues="directQueueTx")
public class Consumer2 {
    @RabbitHandler
    public void process(String msg,Channel channel, Message message) throws IOException {
        //拿到消息延迟消费
        try {
            Thread.sleep(1000*3);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }


        try {
            channel.basicAck(message.getMessageProperties().getDeliveryTag(), false);
            System.out.println("get msg2 success msg = "+msg);

        } catch (Exception e) {
            //消费者处理出了问题，需要告诉队列信息消费失败
            channel.basicNack(message.getMessageProperties().getDeliveryTag(),
                              false, true);
            System.err.println("get msg2 failed msg = "+msg);
        }
    }
}
```
两个消费者之间唯一的区别在于两者获取消息后，延迟时间不一致。
<a name="R4bav"></a>
### **2.5、消息生产者**
有了消息消费者，需要有一个方式提供消息并将消息推送到消息队列中。
```java
public interface IMessageServcie {
	public void sendMessage(String exchange,String routingKey,Object msg);
}
```
```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.connection.CorrelationData;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.amqp.rabbit.core.RabbitTemplate.ConfirmCallback;
import org.springframework.amqp.rabbit.core.RabbitTemplate.ReturnCallback;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import cn.linkpower.service.IMessageServcie;

@Component
public class MessageServiceImpl implements IMessageServcie,ConfirmCallback,ReturnCallback {

    private static Logger log = LoggerFactory.getLogger(MessageServiceImpl.class);

    @Autowired
    private RabbitTemplate rabbitTemplate;

    @Override
    public void sendMessage(String exchange,String routingKey,Object msg) {
        //消息发送失败返回到队列中, yml需要配置 publisher-returns: true
        rabbitTemplate.setMandatory(true);
        //消息消费者确认收到消息后，手动ack回执
        rabbitTemplate.setConfirmCallback(this);
        rabbitTemplate.setReturnCallback(this);
        //发送消息
        rabbitTemplate.convertAndSend(exchange,routingKey,msg);
    }

    @Override
    public void returnedMessage(Message message, int replyCode, String replyText, String exchange, String routingKey) {
        log.info("---- returnedMessage ----replyCode="+replyCode+" replyText="+replyText+" ");
    }

    @Override
    public void confirm(CorrelationData correlationData, boolean ack, String cause) {
        log.info("---- confirm ----ack="+ack+"  cause="+String.valueOf(cause));
        log.info("correlationData -->"+correlationData.toString());
        if(ack){
            log.info("---- confirm ----ack==true  cause="+cause);
        }else{
            log.info("---- confirm ----ack==false  cause="+cause);
        }
    }

}
```
除了定义好了消息发送的工具服务接口外，还需要一个类，实现请求时产生消息，所以写一个controller。
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import cn.linkpower.service.IMessageServcie;

@Controller
public class SendMessageTx {

    @Autowired
    private IMessageServcie messageServiceImpl;

    @RequestMapping("/sendMoreMsgTx")
    @ResponseBody
    public String sendMoreMsgTx(){
        //发送10条消息
        for (int i = 0; i < 10; i++) {
            String msg = "msg"+i;
            System.out.println("发送消息  msg："+msg);
            messageServiceImpl.sendMessage("directExchangeTx", "directQueueTxRoutingKey", msg);
            //每两秒发送一次
            try {
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        return "send ok";
    }
}
```
运行SpringBoot项目，访问指定的url，是可以观察到消息产生和消费的。<br />有些人会问，写到这里就够了吗，这和没写一样啊，都是教如何配置，如何生产消息，如何消费消息。<br />所以接下来的才是重点了，一起研究一个事，当配置的消费者二出现消费消息时，出问题了，如何能够保证像之前那样，消费者一处理剩下的消息？
<a name="dp5Ux"></a>
## 三、ack配置和测试
<a name="Nt7GT"></a>
### **3.1、模拟消费者二出问题**
发送的消息格式都是 msg1、msg2、…<br />所以，不妨这么想，当消费者二拿到的消息msg后面的数字大于3，表示不要了。
```java
import java.io.IOException;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitHandler;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

import com.rabbitmq.client.Channel;

@Component
@RabbitListener(queues="directQueueTx")
public class Consumer2 {
    @RabbitHandler
    public void process(String msg,Channel channel, Message message) throws IOException {
        //拿到消息延迟消费
        try {
            Thread.sleep(1000*3);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }



        try {
            if(!isNull(msg)){
                String numstr = msg.substring(3);
                Integer num = Integer.parseInt(numstr);
                if(num >= 3){
                    channel.basicNack(message.getMessageProperties().getDeliveryTag(),
                                      false, true);
                    System.out.println("get msg2 basicNack msg = "+msg);
                }else{
                    channel.basicAck(message.getMessageProperties().getDeliveryTag(), false);
                    System.out.println("get msg2 basicAck msg = "+msg);
                }
            }
        } catch (Exception e) {
            //消费者处理出了问题，需要告诉队列信息消费失败
            channel.basicNack(message.getMessageProperties().getDeliveryTag(),
                              false, true);
            System.err.println("get msg2 failed msg = "+msg);
        }
    }

    public static boolean isNull(Object obj){
        return obj == null || obj == ""||obj == "null";
    }
}
```
再次请求接口，统计日志信息打印发现：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661777975948-1d653ed5-0af7-4f5c-8267-b5cb43924eb6.png#clientId=u0b4ddc91-a78f-4&from=paste&height=472&id=uf326fc3b&originHeight=1181&originWidth=856&originalType=binary&ratio=1&rotation=0&showTitle=false&size=530232&status=done&style=shadow&taskId=u980c3e54-8cfa-446e-8a7d-40b3f99ca93&title=&width=342.4)<br />可以发现：当对消息者二进行限制大于等于3时，不接受消息队列传递来的消息时，消息队列会随机重发那条消息，直至消息发送至完好的消费者一时，才会把消息消费掉。
<a name="ZanEz"></a>
## 四、分析几个回执方法
<a name="xy79B"></a>
### **4.1、确认消息**
```java
channel.basicAck(long deliveryTag, boolean multiple);
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800212-501a200c-500c-4866-be42-726c6370f6e8.png#clientId=u0b4ddc91-a78f-4&from=paste&id=u60e316b5&originHeight=117&originWidth=708&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4922578f-f64c-409f-8d8d-b8f76ad63a8&title=)<br />一般使用下列方式：
```java
channel.basicAck(message.getMessageProperties().getDeliveryTag(),  false);
```
<a name="z2QlD"></a>
### **4.2、拒绝消息**
```java
channel.basicNack(long deliveryTag, boolean multiple, boolean requeue);
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800410-abc7383d-fcf3-4500-a41f-804a390d4004.png#clientId=u0b4ddc91-a78f-4&from=paste&id=udd898e80&originHeight=159&originWidth=685&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u523af78e-a716-4506-9f53-07a34ce9449&title=)<br />接下来还是修改消费者二，将这个方法最后个参数更改为false，看现象是什么？
```java
import java.io.IOException;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.annotation.RabbitHandler;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;
import com.rabbitmq.client.Channel;

@Component
@RabbitListener(queues="directQueueTx")
public class Consumer2 {
    @RabbitHandler
    public void process(String msg,Channel channel, Message message) throws IOException {
        //拿到消息延迟消费
        try {
            Thread.sleep(1000*3);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }



        try {
            if(!isNull(msg)){
                String numstr = msg.substring(3);
                Integer num = Integer.parseInt(numstr);
                if(num >= 3){
                    channel.basicNack(message.getMessageProperties().getDeliveryTag(),
                                      false, false);
                    System.out.println("get msg2 basicNack msg = "+msg);
                }else{
                    channel.basicAck(message.getMessageProperties().getDeliveryTag(), false);
                    System.out.println("get msg2 basicAck msg = "+msg);
                }
            }
        } catch (Exception e) {
            //消费者处理出了问题，需要告诉队列信息消费失败
            channel.basicNack(message.getMessageProperties().getDeliveryTag(),
                              false, true);
            System.err.println("get msg2 failed msg = "+msg);
        }
    }

    public static boolean isNull(Object obj){
        return obj == null || obj == ""||obj == "null";
    }
}
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800566-cae40ecb-5d69-41a7-b110-f1de1236d4c6.png#clientId=u0b4ddc91-a78f-4&from=paste&id=u00667df5&originHeight=150&originWidth=745&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue535df31-fe57-4ecb-90c2-02b03b23159&title=)重启项目，重新请求测试接口。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800692-cefa189e-dd21-4a19-8a0e-5cff27050a31.png#clientId=u0b4ddc91-a78f-4&from=paste&id=u90c5024b&originHeight=373&originWidth=290&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u38a27d60-cb9e-4db5-b12d-e080906f5f7&title=)<br />可以发现，当出现设置参数为false时，也就是如下所示的设置时：
```java
channel.basicNack(message.getMessageProperties().getDeliveryTag(), false, false);
```
如果此时消费者二出了问题，这条消息不会重新回归队列中重新发送，会丢失这条数据。<br />并且再消息队列中不会保存。
<a name="l23al"></a>
### **4.3、拒绝消息**
```java
channel.basicReject(long deliveryTag, boolean requeue);
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800575-e5b24140-40ba-4f09-9ff3-7c42157b3e34.png#clientId=u0b4ddc91-a78f-4&from=paste&id=u56423575&originHeight=118&originWidth=534&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud26e6bf1-6b02-4c23-89e8-91e0193fb5f&title=)<br />这个和上面的`channel.basicNack`又有什么不同呢？还是修改消费者二实验下。<br />![2022-08-29-20-55-59.469610600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661777920362-9403b88e-cea4-43c6-8e85-b744897a78f7.png#clientId=u0b4ddc91-a78f-4&from=ui&id=u82eadae1&originHeight=261&originWidth=877&originalType=binary&ratio=1&rotation=0&showTitle=false&size=688137&status=done&style=shadow&taskId=u41931d7f-d2d2-4e41-bfae-60d15afbf84&title=)<br />请求测试接口，查看日志信息。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661776800897-a2901f38-2125-44c2-b163-28d71ddc492f.png#clientId=u0b4ddc91-a78f-4&from=paste&id=u892b6885&originHeight=514&originWidth=289&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0802a89d-1e9d-4d22-b55a-87c456b0925&title=)<br />可以发现，此时的日志信息配置
```java
channel.basicReject(message.getMessageProperties().getDeliveryTag(), true);
```
和
```java
channel.basicNack(message.getMessageProperties().getDeliveryTag(), false, true);
```
实现的效果是一样的，都是将信息拒绝接收，由于设置的requeue为true，所以都会将拒绝的消息重新入队列中，重新进行消息分配并消费。
<a name="f6A6Z"></a>
## 五、总结
总结了相关的配置，三个确认（或回执）信息的方法，并区别了他们的各项属性，也知道了当消息再一个消费者中处理失败了，如何不丢失消息重新进行消息的分配消费问题。<br />但是这个只是队列和消费者之间的消息确认机制，使用手动ACK方式确保消息队列中的消息都能在消费者中成功消费。那么，消息转发器和消息队列之间呢？消息生产者和消息转发器之间呢？<br />当然，差点忘了一个小问题。<br />思考一个问题，如果消息队列对应的消费者只有一个，并且那个消费者炸了，会出现什么问题呢？？
