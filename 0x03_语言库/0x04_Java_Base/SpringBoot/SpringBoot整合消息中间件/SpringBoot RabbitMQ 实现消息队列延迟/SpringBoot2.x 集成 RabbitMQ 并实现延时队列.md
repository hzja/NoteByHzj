RabbitMQ SpringBoot<br />消息队列中间件是分布式系统中重要的组件，主要解决应用耦合，异步消息，流量削锋等问题。<br />实现高性能，高可用，可伸缩和最终一致性架构。**「RabbitMQ」**是实现了高级消息队列协议（AMQP）的开源消息，具有较高的系统吞吐量、可靠性、消息持久化、免费等优点，在软件项目中具有非常广泛的应用。
<a name="31ecc0e6"></a>
## 项目代码实现
<a name="nrunL"></a>
### 依赖配置
<a name="lDnfz"></a>
#### Maven依赖
```xml
<parent>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-parent</artifactId>
	<version>2.0.1.RELEASE</version>
	<relativePath/> 
</parent>
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-amqp</artifactId>
</dependency>
```
<a name="BmhKK"></a>
#### 配置文件
```
spring.rabbitmq.host=192.168.202.128
spring.rabbitmq.port=5672
spring.rabbitmq.username=guest
spring.rabbitmq.password=guest
```
<a name="6GHnV"></a>
### 组件设计与实现
<a name="70pqu"></a>
#### Exchange(交换机）
定义交换机名称、类型、持久化、延时交换机名称等属性。
```java
public interface IRabbitMqExchange {

    /**
     * Exchange(交换机) 的名称
     * */
    String exchangeName();

    /**
     * exchange类型 DIRECT("direct"), FANOUT("fanout"), TOPIC("topic"), HEADERS("headers")
     * */
    default String type(){return "topic";}

    /**
     * 是否持久化
     */
    default boolean durable(){return true;}

    /**
     * 当所有队列在完成使用此exchange时，是否删除
     */
    default boolean autoDelete(){return false;}

    /**
     * 是否允许直接binding
     * 如果是true的话 则不允许直接binding到此 exchange
     */
    default boolean internal(){ return false;}

    /**
     * 其他的一些参数设置
     */
    default Map<String, Object> arguments(){ return null; }

    /**
     * 延时 Exchange
     * */
    default String delayExchangeName() {return "delay."+exchangeName();}

}
```
<a name="4c80562e"></a>
#### 路由(Routing)
```java
public interface IRabbitMqRouting {
    /**
     * rabbitmq路由key
     * */
    String routingKey();

}
```
<a name="xrTrb"></a>
#### 队列(Queue)
定义队列名称、持久化、延时队列名称等属性
```java
public interface IRabbitMqQueue {
    /**
     * Queue(队列)名称
     */
    String queueName();

    /**
     * 是否持久化
     * */
    default boolean durable() {return true;}

    /**
     * 排他性
     * */
    default boolean exclusive(){return false;}

    /**
     * 是否自动删除
     * */
    default boolean autoDelete(){return false;}

    /**
     * 其他属性设置
     * */
    default Map<String, Object> arguments() { return null; }

    /**
     * 默认的延时队列名称
     * */
    default String delayQueueName(){return "delay."+this.queueName();}

}
```
<a name="hZERt"></a>
#### 绑定关系(Binding)
定义了 交换机(Exchange)-路由(Routing)-消息队列(Queue)的绑定关系,以及定义是否支持延时消息。
```java
public interface IRabbitMqBinding {
    /**
     * 需要绑定的exchange(交换机)
     * */
    IRabbitMqExchange exchange();

    /**
     * 需要绑定的routing(路由)
     * */
    IRabbitMqRouting routing();

    /**
     * 需要绑定的queue(队列)
     * */
    IRabbitMqQueue queue();

    /**
     * 消息队列是否允许延时
     * */
    boolean allowDelay();
}
```
<a name="B1OvP"></a>
#### 默认注册器
实现了交换机、消息队列、绑定关系的注册。如果绑定关系中定义支持延迟消息，则额外注册一个延时交换机和死信队列，以实现延时消息推送的功能。
```java
public class DefaultRabbitMqRegister implements IRabbitMqRegister, SmartLifecycle {

    ConnectionFactory connectionFactory;

    Channel channel;

    public DefaultRabbitMqRegister() {
    }

    public DefaultRabbitMqRegister(ConnectionFactory connectionFactory) {
        this.connectionFactory = connectionFactory;
    }

    @PostConstruct
    public void init() {
        channel = connectionFactory.createConnection().createChannel(false);
    }

    @Override
    public void registerExchange(IRabbitMqExchange... exchanges) throws IOException {
        for (IRabbitMqExchange exchange : exchanges) {
            channel.exchangeDeclare(exchange.exchangeName(), exchange.type(), exchange.durable(), exchange.autoDelete(), exchange.internal(), exchange.arguments());
        }
    }

    @Override
    public void registerQueue(IRabbitMqQueue... queues) throws IOException {
        for (IRabbitMqQueue queue : queues) {
            channel.queueDeclare(queue.queueName(), queue.durable(), queue.exclusive(), queue.autoDelete(), queue.arguments());
        }
    }

    @Override
    public void registerBinding(IRabbitMqBinding... bindings) throws IOException {
        for (IRabbitMqBinding binding : bindings) {
            channel.queueBind(binding.queue().queueName(), binding.exchange().exchangeName(), binding.routing().routingKey());
            if (binding.allowDelay()) {
                registerDelayBinding(binding);
            }
        }
    }

    /**
     * 创建一个内部的 死信队列 用来实现 延时队列
     */
    private void registerDelayBinding(IRabbitMqBinding binding) throws IOException {
        IRabbitMqExchange exchange = binding.exchange();
        // 注册一个延时的消息交换机
        channel.exchangeDeclare(exchange.delayExchangeName(), exchange.type(), exchange.durable(), exchange.autoDelete(), exchange.internal(), exchange.arguments());
        // 注册一个死信队列  设置消息超时后，将消息转发到原来的Router队列
        IRabbitMqQueue queue = binding.queue();
        Map<String, Object> arguments = queue.arguments();
        if (arguments == null) {
            arguments = new HashMap<>(4);
        }
        arguments.put("x-dead-letter-exchange", binding.exchange().exchangeName());
        arguments.put("x-dead-letter-routing-key", binding.routing().routingKey());
        channel.queueDeclare(queue.delayQueueName(), queue.durable(), queue.exclusive(), queue.autoDelete(), arguments);
        // 将交换机和队列绑定
        channel.queueBind(queue.delayQueueName(), exchange.delayExchangeName(), binding.routing().routingKey());
    }

    private List<MessageListenerContainer> listenerContainers = new LinkedList<>();

    @Override
    public void listenerQueue(IRabbitMqListener listener, IRabbitMqQueue... queues) {
        String[] queueNames = new String[queues.length];
        for (int idx = 0; idx < queues.length; idx++) {
            queueNames[idx] = queues[idx].queueName();
        }
        SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory);
        // 配置手动确认
        container.setAcknowledgeMode(AcknowledgeMode.MANUAL);
        container.setQueueNames(queueNames);
        container.setMessageListener(listener);
        listenerContainers.add(container);
    }

    @Override
    public void start() {
        for (MessageListenerContainer container : listenerContainers) {
            container.start();
        }
    }

    @Override
    public void stop() {
    }

    @Override
    public boolean isRunning() {
        return false;
    }

    @Override
    public boolean isAutoStartup() {
        return true;
    }

    @Override
    public void stop(Runnable runnable) {
    }

    @Override
    public int getPhase() {
        return 9999;
    }
}
```
<a name="C5XxN"></a>
#### 消息监听器
```java
public interface IRabbitMqListener {
    /**
     * 处理rabbitMq的消息
     * */
    boolean handleMessage(Object obj);

}
```
抽象实现类(具体的消费者继承该抽象类，重写`handleMessage()`方法，实现消费逻辑)
```java
public abstract class AbstractMessageListener implements ChannelAwareMessageListener, IRabbitMqListener {

    private Logger logger = LoggerFactory.getLogger(AbstractMessageListener.class);

    private MessageConverter messageConverter = new Jackson2JsonMessageConverter();

    @Override
    public void onMessage(Message message, Channel channel) throws Exception {
        long tag = message.getMessageProperties().getDeliveryTag();
        try {
            Object obj = messageConverter.fromMessage(message);
            boolean handleResult = handleMessage(obj);
            if (handleResult) {
                channel.basicAck(tag, false);
            } else {
                logger.error("消息处理失败 message: {}", message);
                channel.basicNack(tag, false, false);
            }
        } catch (Exception e) {
            channel.basicNack(tag, false, false);
            logger.error("消息处理异常 message: " + message + " " + e.getMessage(), e);
        }
    }

}
```
<a name="Jnszf"></a>
#### 消息发送服务类
实现发送消息、发送延时消息等功能
```java
public class RabbitMqServiceImpl implements IRabbitMqService, RabbitTemplate.ConfirmCallback, RabbitTemplate.ReturnCallback {

    private Logger logger = LoggerFactory.getLogger(RabbitMqServiceImpl.class);

    @Autowired
    protected RabbitTemplate rabbitTemplate;

    @PostConstruct
    public void init() {
        rabbitTemplate.setConfirmCallback(this);
        rabbitTemplate.setReturnCallback(this);
        rabbitTemplate.setMessageConverter(new Jackson2JsonMessageConverter());
    }

    @Override
    public void send(IRabbitMqExchange exchange, IRabbitMqRouting routing, Object msg) {
        CorrelationData correlationId = new CorrelationData(UUID.randomUUID().toString());
        rabbitTemplate.convertAndSend(exchange.exchangeName(), routing.routingKey(), msg, correlationId);
    }

    @Override
    public void send(IRabbitMqExchange exchange, IRabbitMqRouting routing, Object msg, long delay) {
        CorrelationData correlationId = new CorrelationData(UUID.randomUUID().toString());
        if (delay > 0) {
            MessagePostProcessor processor = (Message message) -> {
                message.getMessageProperties().setExpiration(delay + "");
                return message;
            };
            rabbitTemplate.convertAndSend(exchange.delayExchangeName(), routing.routingKey(), msg, processor, correlationId);
        } else {
            rabbitTemplate.convertAndSend(exchange.exchangeName(), routing.routingKey(), msg, correlationId);
        }
    }
    /**
     * 消息发送的回调
     *
     * @param correlationId 消息Id
     * @param ack           是否成功的标示
     * @param cause         错误原因
     */
    @Override
    public void confirm(CorrelationData correlationId, boolean ack, String cause) {
        if (ack) {
            logger.info("消息发送成功 correlationId: {} cause: {}", correlationId, cause);
        } else {
            logger.error("消息发送失败 correlationId: {} cause: {}", correlationId, cause);
        }
    }

    @Override
    public void returnedMessage(Message message, int replyCode, String replyText, String exchange, String routingKey) {
        logger.info("returnedMessage message: {} replyCode: {} exchange: {} routingKey: {}", message, replyCode, exchange, routingKey);
    }

}
```
<a name="aSlaq"></a>
### 实战
<a name="RJnIE"></a>
#### 使用枚举定义消息队列配置
定义测试Exchange：mq.exchange.test
```java
/**
 * RabbitMq Exchange(交换机)定义
 * */
public enum RabbitMqExchange implements IRabbitMqExchange {

    MQ_EXCHANGE_TEST("mq.exchange.test") ;

    private String exchangeName;

    @Override
    public String exchangeName() {
        return this.exchangeName;
    }

    RabbitMqExchange(String exchangeName){
        this.exchangeName = exchangeName;
    }
}
```
定义测试Queue：mq.queue.test
```java
public enum RabbitMqQueue implements IRabbitMqQueue {
    MQ_QUEUE_TEST("mq.queue.test");

    private String queueName;

    @Override
    public String queueName() {
        return this.queueName;
    }

    RabbitMqQueue(String queueName){
        this.queueName = queueName;
    }

}
```
定义测试Routing：mq.routing.test
```java
/**
 * RabbitMq routing（路由定义）
 * */
public enum RabbitMqRouting implements IRabbitMqRouting {
    MQ_ROUTING_TEST("mq.routing.test");

    private String routingKey;

    @Override
    public String routingKey() {
        return this.routingKey;
    }

    RabbitMqRouting(String routingKey){
        this.routingKey = routingKey;
    }
}
```
定义绑定关系：
```java
/**
 * RabbitMq Exchange(交换机) Routing(路由) Queue(队列) 的绑定关系
 * */
public enum RabbitMqBinding implements IRabbitMqBinding {

    MQ_BINDING_TEST(RabbitMqExchange.MQ_EXCHANGE_TEST,RabbitMqRouting.MQ_ROUTING_TEST,RabbitMqQueue.MQ_QUEUE_TEST,true);

    /**
     * exchange(交换机)
     */
    IRabbitMqExchange exchange;
    /**
     * routing(路由)
     */
    IRabbitMqRouting routing;
    /**
     * queue(队列)
     */
    IRabbitMqQueue queue;
    /**
     * 是否允许延时
     */
    boolean allowDelay = false;

    RabbitMqBinding(IRabbitMqExchange exchange,IRabbitMqRouting routing,IRabbitMqQueue queue){
        this.exchange = exchange;
        this.routing = routing;
        this.queue = queue;
    }

    RabbitMqBinding(IRabbitMqExchange exchange,IRabbitMqRouting routing,IRabbitMqQueue queue,boolean allowDelay){
        this.exchange = exchange;
        this.routing = routing;
        this.queue = queue;
        this.allowDelay = allowDelay;
    }

    @Override
    public IRabbitMqExchange exchange() {
        return this.exchange;
    }

    @Override
    public IRabbitMqRouting routing() {
        return this.routing;
    }

    @Override
    public IRabbitMqQueue queue() {
        return this.queue;
    }

    @Override
    public boolean allowDelay() {
        return this.allowDelay;
    }
}
```
测试消费者类
```java
public class TestConsumer extends AbstractMessageListener {

    Logger logger = LoggerFactory.getLogger(TestConsumer.class);
    @Override
    public boolean handleMessage(Object obj) {
        logger.info("rabbitmq消费者开始消费，消息内容：" +obj.toString());
        return true;
    }
}
```
启动项目<br />登录rabbitmq控制台，已经自动创建了 交换机和延迟交换机，消息队列和死信队列<br />测试发送消息
```java
@Test
public void testSendMq(){
     logger.info("生产者发送消息到mq");
     rabbitMqService.send(RabbitMqExchange.MQ_EXCHANGE_TEST, RabbitMqRouting.MQ_ROUTING_TEST,"测试发送消息");
}
```
测试发送延时消息（60秒）
```java
 @Test
public void testSendDelayMq(){
    logger.info("生产者发送延迟消息到mq");
    rabbitMqService.send(RabbitMqExchange.MQ_EXCHANGE_TEST, RabbitMqRouting.MQ_ROUTING_TEST,"测试发送延时消息60s",60*1000);
}
```
