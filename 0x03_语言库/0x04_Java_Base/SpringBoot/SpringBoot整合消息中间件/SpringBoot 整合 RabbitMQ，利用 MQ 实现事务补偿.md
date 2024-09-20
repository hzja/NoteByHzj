SpringBoot RabbitMQ 事务补偿
<a name="XSE7m"></a>
### 项目整合流程
<a name="5YhGi"></a>
#### 创建一个 Maven工程，引入 amqp 包
```xml
<!--amqp 支持-->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-amqp</artifactId>
</dependency>
```
<a name="i7Rb5"></a>
#### 在全局文件中配置 rabbitMQ 服务信息
其中，`spring.rabbitmq.addresses`参数值为 rabbitmq 服务器地址
```
spring.rabbitmq.addresses=197.168.24.206:5672
spring.rabbitmq.username=guest
spring.rabbitmq.password=guest
spring.rabbitmq.virtual-host=/
```
<a name="nW6L3"></a>
#### Rabbitmq 配置类
```java
@Slf4j
@Configuration
public class RabbitConfig {

    /**
     * 初始化连接工厂
     * @param addresses
     * @param userName
     * @param password
     * @param vhost
     * @return
     */
    @Bean
    ConnectionFactory connectionFactory(@Value("${spring.rabbitmq.addresses}") String addresses,
                                        @Value("${spring.rabbitmq.username}") String userName,
                                        @Value("${spring.rabbitmq.password}") String password,
                                        @Value("${spring.rabbitmq.virtual-host}") String vhost) {
        CachingConnectionFactory connectionFactory = new CachingConnectionFactory();
        connectionFactory.setAddresses(addresses);
        connectionFactory.setUsername(userName);
        connectionFactory.setPassword(password);
        connectionFactory.setVirtualHost(vhost);
        return connectionFactory;
    }

    /**
     * 重新实例化 RabbitAdmin 操作类
     * @param connectionFactory
     * @return
     */
    @Bean
    public RabbitAdmin rabbitAdmin(ConnectionFactory connectionFactory){
        return new RabbitAdmin(connectionFactory);
    }

    /**
     * 重新实例化 RabbitTemplate 操作类
     * @param connectionFactory
     * @return
     */
    @Bean
    public RabbitTemplate rabbitTemplate(ConnectionFactory connectionFactory){
        RabbitTemplate rabbitTemplate=new RabbitTemplate(connectionFactory);
        //数据转换为json存入消息队列
        rabbitTemplate.setMessageConverter(new Jackson2JsonMessageConverter());
        return rabbitTemplate;
    }

    /**
     * 将 RabbitUtil 操作工具类加入IOC容器
     * @return
     */
    @Bean
    public RabbitUtil rabbitUtil(){
        return new RabbitUtil();
    }

}
```
<a name="XQE3U"></a>
#### RabbitUtil 工具类
```java
public class RabbitUtil {

    private static final Logger logger = LoggerFactory.getLogger(RabbitUtil.class);

    @Autowired
    private RabbitAdmin rabbitAdmin;

    @Autowired
    private RabbitTemplate rabbitTemplate;

    /**
     * 创建Exchange
     * @param exchangeName
     */
    public void addExchange(String exchangeType, String exchangeName){
        Exchange exchange = createExchange(exchangeType, exchangeName);
        rabbitAdmin.declareExchange(exchange);
    }

    /**
     * 删除一个Exchange
     * @param exchangeName
     */
    public boolean deleteExchange(String exchangeName){
        return rabbitAdmin.deleteExchange(exchangeName);
    }

    /**
     * 创建一个指定的Queue
     * @param queueName
     * @return queueName
     */
    public void addQueue(String queueName){
        Queue queue = createQueue(queueName);
        rabbitAdmin.declareQueue(queue);
    }

    /**
     * 删除一个queue
     * @return queueName
     * @param queueName
     */
    public boolean deleteQueue(String queueName){
        return rabbitAdmin.deleteQueue(queueName);
    }

    /**
     * 按照筛选条件，删除队列
     * @param queueName
     * @param unused 是否被使用
     * @param empty 内容是否为空
     */
    public void deleteQueue(String queueName, boolean unused, boolean empty){
        rabbitAdmin.deleteQueue(queueName,unused,empty);
    }

    /**
     * 清空某个队列中的消息，注意，清空的消息并没有被消费
     * @return queueName
     * @param queueName
     */
    public void purgeQueue(String queueName){
        rabbitAdmin.purgeQueue(queueName, false);
    }

    /**
     * 判断指定的队列是否存在
     * @param queueName
     * @return
     */
    public boolean existQueue(String queueName){
        return rabbitAdmin.getQueueProperties(queueName) == null ? false : true;
    }

    /**
     * 绑定一个队列到一个匹配型交换器使用一个routingKey
     * @param exchangeType
     * @param exchangeName
     * @param queueName
     * @param routingKey
     * @param isWhereAll
     * @param headers EADERS模式类型设置，其他模式类型传空
     */
    public void addBinding(String exchangeType, String exchangeName, String queueName, String routingKey, boolean isWhereAll, Map<String, Object> headers){
        Binding binding = bindingBuilder(exchangeType, exchangeName, queueName, routingKey, isWhereAll, headers);
        rabbitAdmin.declareBinding(binding);
    }

    /**
     * 声明绑定
     * @param binding
     */
    public void addBinding(Binding binding){
        rabbitAdmin.declareBinding(binding);
    }

    /**
     * 解除交换器与队列的绑定
     * @param exchangeType
     * @param exchangeName
     * @param queueName
     * @param routingKey
     * @param isWhereAll
     * @param headers
     */
    public void removeBinding(String exchangeType, String exchangeName, String queueName, String routingKey, boolean isWhereAll, Map<String, Object> headers){
        Binding binding = bindingBuilder(exchangeType, exchangeName, queueName, routingKey, isWhereAll, headers);
        removeBinding(binding);
    }

    /**
     * 解除交换器与队列的绑定
     * @param binding
     */
    public void removeBinding(Binding binding){
        rabbitAdmin.removeBinding(binding);
    }

    /**
     * 创建一个交换器、队列，并绑定队列
     * @param exchangeType
     * @param exchangeName
     * @param queueName
     * @param routingKey
     * @param isWhereAll
     * @param headers
     */
    public void andExchangeBindingQueue(String exchangeType, String exchangeName, String queueName, String routingKey, boolean isWhereAll, Map<String, Object> headers){
        //声明交换器
        addExchange(exchangeType, exchangeName);
        //声明队列
        addQueue(queueName);
        //声明绑定关系
        addBinding(exchangeType, exchangeName, queueName, routingKey, isWhereAll, headers);
    }

    /**
     * 发送消息
     * @param exchange
     * @param routingKey
     * @param object
     */
    public void convertAndSend(String exchange, String routingKey, final Object object){
        rabbitTemplate.convertAndSend(exchange, routingKey, object);
    }

    /**
     * 转换Message对象
     * @param messageType
     * @param msg
     * @return
     */
    public Message getMessage(String messageType, Object msg){
        MessageProperties messageProperties = new MessageProperties();
        messageProperties.setContentType(messageType);
        Message message = new Message(msg.toString().getBytes(),messageProperties);
        return message;
    }

    /**
     * 声明交换机
     * @param exchangeType
     * @param exchangeName
     * @return
     */
    private Exchange createExchange(String exchangeType, String exchangeName){
        if(ExchangeType.DIRECT.equals(exchangeType)){
            return new DirectExchange(exchangeName);
        }
        if(ExchangeType.TOPIC.equals(exchangeType)){
            return new TopicExchange(exchangeName);
        }
        if(ExchangeType.HEADERS.equals(exchangeType)){
            return new HeadersExchange(exchangeName);
        }
        if(ExchangeType.FANOUT.equals(exchangeType)){
            return new FanoutExchange(exchangeName);
        }
        return null;
    }

    /**
     * 声明绑定关系
     * @param exchangeType
     * @param exchangeName
     * @param queueName
     * @param routingKey
     * @param isWhereAll
     * @param headers
     * @return
     */
    private Binding bindingBuilder(String exchangeType, String exchangeName, String queueName, String routingKey, boolean isWhereAll, Map<String, Object> headers){
        if(ExchangeType.DIRECT.equals(exchangeType)){
            return BindingBuilder.bind(new Queue(queueName)).to(new DirectExchange(exchangeName)).with(routingKey);
        }
        if(ExchangeType.TOPIC.equals(exchangeType)){
            return BindingBuilder.bind(new Queue(queueName)).to(new TopicExchange(exchangeName)).with(routingKey);
        }
        if(ExchangeType.HEADERS.equals(exchangeType)){
            if(isWhereAll){
                return BindingBuilder.bind(new Queue(queueName)).to(new HeadersExchange(exchangeName)).whereAll(headers).match();
            }else{
                return BindingBuilder.bind(new Queue(queueName)).to(new HeadersExchange(exchangeName)).whereAny(headers).match();
            }
        }
        if(ExchangeType.FANOUT.equals(exchangeType)){
            return BindingBuilder.bind(new Queue(queueName)).to(new FanoutExchange(exchangeName));
        }
        return null;
    }

    /**
     * 声明队列
     * @param queueName
     * @return
     */
    private Queue createQueue(String queueName){
        return new Queue(queueName);
    }


    /**
     * 交换器类型
     */
    public final static class ExchangeType {

        /**
         * 直连交换机（全文匹配）
         */
        public final static String DIRECT = "DIRECT";

        /**
         * 通配符交换机（两种通配符：*只能匹配一个单词，#可以匹配零个或多个）
         */
        public final static String TOPIC = "TOPIC";

        /**
         * 头交换机（自定义键值对匹配，根据发送消息内容中的headers属性进行匹配）
         */
        public final static String HEADERS = "HEADERS";

        /**
         * 扇形（广播）交换机 （将消息转发到所有与该交互机绑定的队列上）
         */
        public final static String FANOUT = "FANOUT";
    }
}
```
<a name="3IY76"></a>
#### 队列监听类（静态）
```java
@Slf4j
@Configuration
public class DirectConsumeListener {

    /**
     * 监听指定队列，名称：mq.direct.1
     * @param message
     * @param channel
     * @throws IOException
     */
    @RabbitListener(queues = "mq.direct.1")
    public void consume(Message message, Channel channel) throws IOException {
        log.info("DirectConsumeListener，收到消息: {}", message.toString());
    }
}
```
如果需要监听指定的队列，只需要方法上加上`@RabbitListener(queues = "")`即可，同时填写对应的队列名称。
<a name="L2S3b"></a>
#### 队列监听类（动态）
如果动态监听队列，而不是通过写死在方法上，需要重新实例化一个`SimpleMessageListenerContainer`对象，这个对象就是监听容器。
```java
@Slf4j
@Configuration
public class DynamicConsumeListener {

    /**
     * 使用SimpleMessageListenerContainer实现动态监听
     * @param connectionFactory
     * @return
     */
    @Bean
    public SimpleMessageListenerContainer messageListenerContainer(ConnectionFactory connectionFactory){
        SimpleMessageListenerContainer container = new SimpleMessageListenerContainer(connectionFactory);
        container.setMessageListener((MessageListener) message -> {
            log.info("ConsumerMessageListen，收到消息: {}", message.toString());
        });
        return container;
    }
}
```
如果想向`SimpleMessageListenerContainer`添加监听队列或者移除队列，只需通过如下方式即可操作。
```java
@Slf4j
@RestController
@RequestMapping("/consumer")
public class ConsumerController {

    @Autowired
    private SimpleMessageListenerContainer container;

    @Autowired
    private RabbitUtil rabbitUtil;

    /**
     * 添加队列到监听器
     * @param consumerInfo
     */
    @PostMapping("addQueue")
    public void addQueue(@RequestBody ConsumerInfo consumerInfo) {
        boolean existQueue = rabbitUtil.existQueue(consumerInfo.getQueueName());
        if(!existQueue){
            throw new CommonExecption("当前队列不存在");
        }
        //消费mq消息的类
        container.addQueueNames(consumerInfo.getQueueName());
        //打印监听容器中正在监听到队列
        log.info("container-queue:{}", JsonUtils.toJson(container.getQueueNames()));
    }

    /**
     * 移除正在监听的队列
     * @param consumerInfo
     */
    @PostMapping("removeQueue")
    public void removeQueue(@RequestBody ConsumerInfo consumerInfo) {
        //消费mq消息的类
        container.removeQueueNames(consumerInfo.getQueueName());
        //打印监听容器中正在监听到队列
        log.info("container-queue:{}", JsonUtils.toJson(container.getQueueNames()));
    }

    /**
     * 查询监听容器中正在监听到队列
     */
    @PostMapping("queryListenerQueue")
    public void queryListenerQueue() {
        log.info("container-queue:{}", JsonUtils.toJson(container.getQueueNames()));
    }
}
```
<a name="Z4b3I"></a>
#### 发送消息到交换器
<a name="tKbjE"></a>
##### 先编写一个请求参数实体类
```java
@Data
public class ProduceInfo implements Serializable {

    private static final long serialVersionUID = 1l;

    /**
     * 交换器名称
     */
    private String exchangeName;

    /**
     * 路由键key
     */
    private String routingKey;

    /**
     * 消息内容
     */
    public String msg;
}
```
<a name="BnOvO"></a>
##### 编写接口api
```java
@RestController
@RequestMapping("/produce")
public class ProduceController {

    @Autowired
    private RabbitUtil rabbitUtil;

    /**
     * 发送消息到交换器
     * @param produceInfo
     */
    @PostMapping("sendMessage")
    public void sendMessage(@RequestBody ProduceInfo produceInfo) {
        rabbitUtil.convertAndSend(produceInfo.getExchangeName(), produceInfo.getRoutingKey(), produceInfo);
    }
}
```
当然，也可以直接使用`rabbitTemplate`操作类，来实现发送消息。
```java
rabbitTemplate.convertAndSend(exchange, routingKey, message);
```
**参数内容解释**：

- exchange：表示交换器名称
- routingKey：表示路由键key
- message：表示消息
<a name="pIJzg"></a>
#### 交换器、队列维护操作
如果想通过接口对 rabbitMQ 中的交换器、队列以及绑定关系进行维护，通过如下方式接口操作，即可实现！

- 先写一个请求参数实体类
```java
@Data
public class QueueConfig implements Serializable{

    private static final long serialVersionUID = 1l;

    /**
     * 交换器类型
     */
    private String exchangeType;

    /**
     * 交换器名称
     */
    private String exchangeName;

    /**
     * 队列名称
     */
    private String queueName;

    /**
     * 路由键key
     */
    private String routingKey;
}
```

- 编写接口api
```java
/**
 * rabbitMQ管理操作控制层
 */
@RestController
@RequestMapping("/config")
public class RabbitController {


    @Autowired
    private RabbitUtil rabbitUtil;

    /**
     * 创建交换器
     * @param config
     */
    @PostMapping("addExchange")
    public void addExchange(@RequestBody QueueConfig config) {
        rabbitUtil.addExchange(config.getExchangeType(), config.getExchangeName());
    }

    /**
     * 删除交换器
     * @param config
     */
    @PostMapping("deleteExchange")
    public void deleteExchange(@RequestBody QueueConfig config) {
        rabbitUtil.deleteExchange(config.getExchangeName());
    }

    /**
     * 添加队列
     * @param config
     */
    @PostMapping("addQueue")
    public void addQueue(@RequestBody QueueConfig config) {
        rabbitUtil.addQueue(config.getQueueName());
    }

    /**
     * 删除队列
     * @param config
     */
    @PostMapping("deleteQueue")
    public void deleteQueue(@RequestBody QueueConfig config) {
        rabbitUtil.deleteQueue(config.getQueueName());
    }

    /**
     * 清空队列数据
     * @param config
     */
    @PostMapping("purgeQueue")
    public void purgeQueue(@RequestBody QueueConfig config) {
        rabbitUtil.purgeQueue(config.getQueueName());
    }

    /**
     * 添加绑定
     * @param config
     */
    @PostMapping("addBinding")
    public void addBinding(@RequestBody QueueConfig config) {
        rabbitUtil.addBinding(config.getExchangeType(), config.getExchangeName(), config.getQueueName(), config.getRoutingKey(), false, null);
    }

    /**
     * 解除绑定
     * @param config
     */
    @PostMapping("removeBinding")
    public void removeBinding(@RequestBody QueueConfig config) {
        rabbitUtil.removeBinding(config.getExchangeType(), config.getExchangeName(), config.getQueueName(), config.getRoutingKey(), false, null);
    }

    /**
     * 创建头部类型的交换器
     * 判断条件是所有的键值对都匹配成功才发送到队列
     * @param config
     */
    @PostMapping("andExchangeBindingQueueOfHeaderAll")
    public void andExchangeBindingQueueOfHeaderAll(@RequestBody QueueConfig config) {
        HashMap<String, Object> header = new HashMap<>();
        header.put("queue", "queue");
        header.put("bindType", "whereAll");
        rabbitUtil.andExchangeBindingQueue(RabbitUtil.ExchangeType.HEADERS, config.getExchangeName(), config.getQueueName(), null, true, header);
    }

    /**
     * 创建头部类型的交换器
     * 判断条件是只要有一个键值对匹配成功就发送到队列
     * @param config
     */
    @PostMapping("andExchangeBindingQueueOfHeaderAny")
    public void andExchangeBindingQueueOfHeaderAny(@RequestBody QueueConfig config) {
        HashMap<String, Object> header = new HashMap<>();
        header.put("queue", "queue");
        header.put("bindType", "whereAny");
        rabbitUtil.andExchangeBindingQueue(RabbitUtil.ExchangeType.HEADERS, config.getExchangeName(), config.getQueueName(), null, false, header);
    }
}
```
:::info
RabbitMQ 管理器基本的 crud 全部开发完成
:::
<a name="Oc6U8"></a>
### 利用 MQ 实现事务补偿
以常见的订单系统为例，用户点击【下单】按钮之后的业务逻辑可能包括：**支付订单、扣减库存、生成相应单据、发红包、发短信通知等等**。<br />在业务发展初期这些逻辑可能放在一起同步执行，随着业务的发展订单量增长，需要提升系统服务的性能，这时可以将一些不需要立即生效的操作拆分出来异步执行，比如发放红包、发短信通知等。这种场景下就可以用 MQ ，在下单的主流程（比如扣减库存、生成相应单据）完成之后发送一条消息到 MQ 让主流程快速完结，而由另外的单独线程拉取 MQ 的消息（或者由 MQ 推送消息），当发现 MQ 中有发红包或发短信之类的消息时，执行相应的业务逻辑。<br />这种是利用 MQ 实现业务解耦，其它的场景包括最终一致性、广播、错峰流控等等。<br />利用 MQ 实现业务解耦的过程其实也很简单。<br />当主流程结束之后，将消息推送到发红包、发短信交换器中即可
<a name="YV7Aa"></a>
#### 监听下订单操作
```java
@Service
public class OrderService {

    @Autowired
    private RabbitUtil rabbitUtil;

    /**
     * 创建订单
     * @param order
     */
    @Transactional
    public void createOrder(Order order){
        //1、创建订单
        //2、调用库存接口，减库存
        //3、向客户发放红包
        rabbitUtil.convertAndSend("exchange.send.bonus", null, order);
        //4、发短信通知
        rabbitUtil.convertAndSend("exchange.sms.message", null, order);
    }
}
```
<a name="9Cn1l"></a>
#### 监听发红包操作
```java
/**
 * 监听发红包
 * @param message
 * @param channel
 * @throws IOException
 */
@RabbitListener(queues = "exchange.send.bonus")
public void consume(Message message, Channel channel) throws IOException {
    String msgJson = new String(message.getBody(),"UTF-8");
    log.info("收到消息: {}", message.toString());

    //调用发红包接口
}
```
<a name="fpieN"></a>
#### 监听发短信操作
```java
/**
 * 监听发短信
 * @param message
 * @param channel
 * @throws IOException
 */
@RabbitListener(queues = "exchange.sms.message")
public void consume(Message message, Channel channel) throws IOException {
    String msgJson = new String(message.getBody(),"UTF-8");
    log.info("收到消息: {}", message.toString());

    //调用发短信接口
}
```
:::danger
既然 MQ 这么好用，那么是不是完全可以将以前的业务也按照整个模型进行拆分呢？<br />答案显然不是！<br />当引入 MQ 之后业务的确是解耦了，但是当 MQ 一旦挂了，所有的服务基本都挂了，是不是很可怕！
:::
