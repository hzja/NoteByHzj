JavaSpringBootRabbitMQ
<a name="vi9VD"></a>
### 前言
死信：无法被消费的消息，称为死信。<br />如果死信一直留在队列中，会导致一直被消费，却从不消费成功。<br />所以专门开辟了一个来存放死信的队列，叫死信队列（DLX，dead-letter-exchange）。
<a name="UfOtG"></a>
### 死信队列
DLX，Dead Letter Exchange的缩写，又死信邮箱、死信交换机。其实DLX就是一个普通的交换机，和一般的交换机没有任何区别。当消息在一个队列中变成死信（dead message）时，通过这个交换机将死信发送到死信队列中（指定好相关参数，RabbitMQ会自动发送）。
<a name="prNXd"></a>
### 死信的几种来源

- 消息 TTL 过期（time to live，存活时间，可以用在限时支付消息）
- 队列达到最大长度（队列满了，无法路由到该队列）
- 消息被拒绝（ basic.reject / basic.nack ），并且 `requeue = false`

![d7c802d5da6009b9d8faec6830c22a72[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653211940021-37a6ebf8-ac26-4ba1-8ba5-5303a3e94615.png#averageHue=%232d2b2b&clientId=u9a0e79a3-0778-4&from=ui&id=u41f758bc&originHeight=1166&originWidth=2403&originalType=binary&ratio=1&rotation=0&showTitle=false&size=167423&status=done&style=shadow&taskId=u31359d8c-4b42-4099-ad9a-8c1e6f4839a&title=)
<a name="jhvIr"></a>
### 环境准备配置
准备 MQ 的队列和环境：

- 正常交换机
   - 正常队列（最长队列 5） ---- 正常消费者，拒绝消息
   - ttl 队列（过期时间 60 秒） ---- 没有消费者
- 死信交换机
   - 死信队列

主要配置文件如下：
```java
@Configuration
public class DeadConfig {

    /* 正常配置 **********************************************************************************************************/

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
        return BindingBuilder.bind(normalQueue()).to(normalExchange()).with("normalRouting");
    }

    @Bean
    Binding ttlRouteBinding() {
        return BindingBuilder.bind(ttlQueue()).to(normalExchange()).with("ttlRouting");
    }

    /* 死信配置 **********************************************************************************************************/

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
        return BindingBuilder.bind(deadQueue()).to(deadExchange()).with("deadRouting");
    }

    /**
     * 转发到 死信队列，配置参数
     */
    private Map<String, Object> deadQueueArgs() {
        Map<String, Object> map = new HashMap<>();
        // 绑定该队列到私信交换机
        map.put("x-dead-letter-exchange", "deadExchange");
        map.put("x-dead-letter-routing-key", "deadRouting");
        return map;
    }

}
```
arguments 具体参数如下：

| 参数名 | 作用 |
| --- | --- |
| x-message-ttl | 发送到队列的消息在丢弃之前可以存活多长时间（毫秒）。 |
| x-max-length | 队列最大长度 |
| x-expires | 队列在被自动删除（毫秒）之前可以使用多长时间。 |
| x-max-length | 队列在开始从头部删除之前可以包含多少就绪消息。 |
| x-max-length-bytes | 队列在开始从头部删除之前可以包含的就绪消息的总体大小。 |
| x-dead-letter-exchange | 设置队列溢出行为。这决定了在达到队列的最大长度时消息会发生什么。<br />有效值为drop-head或reject-publish。交换的可选名称，如果消息被拒绝或过期，将重新发布这些名称。 |
| x-dead-letter-routing-key | 可选的替换路由密钥，用于在消息以字母为单位时使用。如果未设置，将使用消息的原始路由密钥。 |
| x-max-priority | 队列支持的最大优先级数;如果未设置，队列将不支持消息优先级。 |
| x-queue-mode | 将队列设置为延迟模式，在磁盘上保留尽可能多的消息以减少内存使用;如果未设置，队列将保留内存缓存以尽快传递消息。 |
| x-queue-master-locator | 将队列设置为主位置模式，确定在节点集群上声明时队列主机所在的规则。 |
| x-overflow | 队列达到最大长度时，可选模式包括： drop-head, reject-publish 和 reject-publish-dlx。 |

<a name="Xi8eA"></a>
### 队列达到最大长度
首先测试最简单的，没有消费者。<br />调用6次正常队列的生产方法。
```java
/**
 * 正常消息队列，队列最大长度5
 */
@GetMapping("/normalQueue")
public String normalQueue() {

	Map<String, Object> map = new HashMap<>();
	map.put("messageId", String.valueOf(UUID.randomUUID()));
	map.put("data", System.currentTimeMillis() + ", 正常队列消息，最大长度 5");

	rabbitTemplate.convertAndSend("normalExchange", "normalRouting", map, new CorrelationData());
	return JSONObject.toJSONString(map);
}
```
MQ 结果如下：<br />![69318be10383d3321adbf3dc0a981053[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653212028562-2a19e569-4ac1-4f6c-ac2e-d7101a92e62d.png#averageHue=%23f2f1f0&clientId=u9a0e79a3-0778-4&from=ui&id=uae9d7bbe&originHeight=168&originWidth=882&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18007&status=done&style=shadow&taskId=u5314079f-e411-432d-8690-fba12d1dc9d&title=)
<a name="AQDxZ"></a>
### 消息 TTL 过期
消息的TTL 指的是消息的存活时间，可以通过设置消息的TTL或者队列的TTL来实现。

- 消息的TTL ：对于设置了过期时间属性(expiration)的消息，消息如果在过期时间内没被消费，会过期
- 队列的TTL ：对于设置了过期时间属性(x-message-ttl)的队列，所有路由到这个队列的消息，都会设置上这个过期时间

两种配置都行，一般都用在定时任务，限时支付这种地方。
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
	return JSONObject.toJSONString(map);
}
```
发送后：<br />![91dbcb60fb2676289c374177f716e5c4[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653212043284-18f71a18-23cf-4e97-8e8a-23ddd07b40db.png#averageHue=%23f2f1f0&clientId=u9a0e79a3-0778-4&from=ui&id=ude8cf65e&originHeight=171&originWidth=910&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18867&status=done&style=shadow&taskId=u24cbb3b0-a5b7-4114-87b0-7fcf977ae8d&title=)<br />等待过期后：<br />![37054943b9c7327c13094be823202da3[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653212058558-c91f8c08-1ed6-4a43-b893-376d732c3a94.png#averageHue=%23f1f0ef&clientId=u9a0e79a3-0778-4&from=ui&id=u3af40478&originHeight=159&originWidth=870&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18332&status=done&style=shadow&taskId=u1e0040de-d369-4c74-a8f3-13bc44f8874&title=)<br />Demo 中只是为了方便，代码中尽量使用 消息TTL，不要用 队列TTL
<a name="rSwgE"></a>
### 拒绝消息
正常队列消费后拒绝消息，并且不进行重新入队：
```java
@Component
@RabbitListener(queues = "normalQueue")
public class NormalConsumer {
	@RabbitHandler
	public void process(Map<String, Object> message, Channel channel, Message mqMsg) throws IOException {
		System.out.println("收到消息，并拒绝重新入队 : " + message.toString());
		channel.basicReject(mqMsg.getMessageProperties().getDeliveryTag(), false);
	}
}
```
MQ 控制台：<br />![8fd167b840ba1e2db833d467b3c46f1c[1].png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653212096275-246f4d2d-eda4-4d40-a2f6-19bd8ae80b92.png#averageHue=%23f1f0ef&clientId=u9a0e79a3-0778-4&from=ui&id=u8866bd8b&originHeight=167&originWidth=865&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18755&status=done&style=shadow&taskId=u57e3b214-ff01-44ef-9f52-12ee124127c&title=)<br />死信队列消费：
```java
@Component
@RabbitListener(queues = "deadQueue")
public class DeadConsumer {
	@RabbitHandler
	public void process(Map<String, Object> message, Channel channel, Message mqMsg) throws IOException {
		System.out.println("死信队列收到消息 : " + message.toString());
		channel.basicAck(mqMsg.getMessageProperties().getDeliveryTag(), false);
	}
}
```
消息顺序和实验一致：
```java
死信队列收到消息 : {data=1631534291765, 正常队列消息，最大长度 5, messageId=bce3888b-da38-4299-ac88-d22cbe164739}
死信队列收到消息 : {data=1631535222745, ttl队列消息, messageId=a4617445-5aab-4fac-aec7-5709ea699598}
死信队列收到消息 : {data=1631534503765, 正常队列消息，最大长度 5, messageId=b65ecaab-5ce7-4597-a32c-c90b67ec46da}
死信队列收到消息 : {data=1631534511468, 正常队列消息，最大长度 5, messageId=d63d2a4c-e7d3-4f00-a6ca-78e2d62d1d92}
死信队列收到消息 : {data=1631534585087, 正常队列消息，最大长度 5, messageId=eed0c349-415b-43dc-aa79-c683122a1289}
死信队列收到消息 : {data=1631534588311, 正常队列消息，最大长度 5, messageId=7a7bd152-f2fa-4a74-b9e6-943ac7cbb3d4}
死信队列收到消息 : {data=1631534608504, 正常队列消息，最大长度 5, messageId=9de512a1-4ca4-4060-9096-27aba01c1687}
```
