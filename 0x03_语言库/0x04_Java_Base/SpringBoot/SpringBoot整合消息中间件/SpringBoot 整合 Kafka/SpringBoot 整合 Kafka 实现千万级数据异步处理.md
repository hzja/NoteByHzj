JavaSpringBootKafka
<a name="hDhCL"></a>
## 一、介绍
在集群环境中，kafka 可以通过设置分区数来加快数据的消费速度。<br />下面将结合生产环境的真实案例，以SpringBoot技术框架为基础，介绍 kafka 的使用以及如何实现数据高吞吐！
<a name="Yoj46"></a>
## 二、代码实践
最近，公司大数据团队每天凌晨会将客户的订单数据进行统计计算，然后把业绩数据推送给后端，以便销售人员每天能看到昨天的业绩数据，数据的体量大约在 1000 多万条，以下是对接的过程！
<a name="Rj18p"></a>
### 2.1、添加 kafka 依赖包
本次项目的SpringBoot版本为2.1.5.RELEASE，依赖的 kafka 的版本为2.2.6.RELEASE
```xml
<!--kafka-->
<dependency>
    <groupId>org.springframework.kafka</groupId>
    <artifactId>spring-kafka</artifactId>
    <version>2.2.6.RELEASE</version>
</dependency>
```
<a name="eUCT2"></a>
### 2.2、添加 kafka 配置变量
当添加完了依赖包之后，只需要在application.properties中添加 kafka 配置变量，基本上就可以正常使用了。
```
# 指定kafka server的地址，集群配多个，中间，逗号隔开
spring.kafka.bootstrap-servers=197.168.25.196:9092
#重试次数
spring.kafka.producer.retries=3
#批量发送的消息数量
spring.kafka.producer.batch-size=1000
#32MB的批处理缓冲区
spring.kafka.producer.buffer-memory=33554432
#默认消费者组
spring.kafka.consumer.group-id=crm-user-service
#最早未被消费的offset
spring.kafka.consumer.auto-offset-reset=earliest
#批量一次最大拉取数据量
spring.kafka.consumer.max-poll-records=4000
#是否自动提交
spring.kafka.consumer.enable-auto-commit=true
#自动提交时间间隔，单位ms
spring.kafka.consumer.auto-commit-interval=1000
```
<a name="BlpdW"></a>
### 2.3、创建一个消费者
```java
@Component
public class BigDataTopicListener {

    private static final Logger log = LoggerFactory.getLogger(BigDataTopicListener.class);

    /**
     * 监听kafka数据
     * @param consumerRecords
     * @param ack
     */
    @KafkaListener(topics = {"big_data_topic"})
    public void consumer(ConsumerRecord<?, ?> consumerRecord) {
        log.info("收到bigData推送的数据'{}'", consumerRecord.toString());
        //...
        //db.save(consumerRecord);//插入或者更新数据
    }

}
```
<a name="zayON"></a>
### 2.4、模拟对方推送数据测试
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class KafkaProducerTest {

    @Autowired
    private KafkaTemplate<String, String> kafkaTemplate;

    @Test
    public void testSend(){
        for (int i = 0; i < 5000; i++) {
            Map<String, Object> map = new LinkedHashMap<>();
            map.put("datekey", 20210610);
            map.put("userid", i);
            map.put("salaryAmount", i);
            //向kafka的big_data_topic主题推送数据
            kafkaTemplate.send("big_data_topic", JSONObject.toJSONString(map));
        }
    }
}
```
起初，通过这种单条数据消费方式，进行测试程序没太大毛病！<br />但是，当上到生产之后，发现一个很大的问题，就是消费 1000 万条数据，至少需要3个小时，结果导致数据看板一直没数据。<br />第二天痛定思痛，决定改成批量消费模型，怎么操作呢，请看下面！
<a name="tJ5c2"></a>
### 2.5、将 kafka 的消费模式改成批量消费
首先，创建一个`KafkaConfiguration`配置类，内容如下！
```java
@Configuration
public class KafkaConfiguration {

    @Value("${spring.kafka.bootstrap-servers}")
    private String bootstrapServers;

    @Value("${spring.kafka.producer.retries}")
    private Integer retries;

    @Value("${spring.kafka.producer.batch-size}")
    private Integer batchSize;

    @Value("${spring.kafka.producer.buffer-memory}")
    private Integer bufferMemory;

    @Value("${spring.kafka.consumer.group-id}")
    private String groupId;

    @Value("${spring.kafka.consumer.auto-offset-reset}")
    private String autoOffsetReset;

    @Value("${spring.kafka.consumer.max-poll-records}")
    private Integer maxPollRecords;

    @Value("${spring.kafka.consumer.batch.concurrency}")
    private Integer batchConcurrency;

    @Value("${spring.kafka.consumer.enable-auto-commit}")
    private Boolean autoCommit;

    @Value("${spring.kafka.consumer.auto-commit-interval}")
    private Integer autoCommitInterval;


    /**
     *  生产者配置信息
     */
    @Bean
    public Map<String, Object> producerConfigs() {
        Map<String, Object> props = new HashMap<>();
        props.put(ProducerConfig.ACKS_CONFIG, "0");
        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
        props.put(ProducerConfig.RETRIES_CONFIG, retries);
        props.put(ProducerConfig.BATCH_SIZE_CONFIG, batchSize);
        props.put(ProducerConfig.LINGER_MS_CONFIG, 1);
        props.put(ProducerConfig.BUFFER_MEMORY_CONFIG, bufferMemory);
        props.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        props.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        return props;
    }

    /**
     *  生产者工厂
     */
    @Bean
    public ProducerFactory<String, String> producerFactory() {
        return new DefaultKafkaProducerFactory<>(producerConfigs());
    }

    /**
     *  生产者模板
     */
    @Bean
    public KafkaTemplate<String, String> kafkaTemplate() {
        return new KafkaTemplate<>(producerFactory());
    }


    /**
     *  消费者配置信息
     */
    @Bean
    public Map<String, Object> consumerConfigs() {
        Map<String, Object> props = new HashMap<>();
        props.put(ConsumerConfig.GROUP_ID_CONFIG, groupId);
        props.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, autoOffsetReset);
        props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
        props.put(ConsumerConfig.MAX_POLL_RECORDS_CONFIG, maxPollRecords);
        props.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, autoCommit);
        props.put(ConsumerConfig.SESSION_TIMEOUT_MS_CONFIG, 30000);
        props.put(ConsumerConfig.REQUEST_TIMEOUT_MS_CONFIG, 30000);
        props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class);
        props.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class);
        return props;
    }

    /**
     *  消费者批量工厂
     */
    @Bean
    public KafkaListenerContainerFactory<?> batchFactory() {
        ConcurrentKafkaListenerContainerFactory<Integer, String> factory = new ConcurrentKafkaListenerContainerFactory<>();
        factory.setConsumerFactory(new DefaultKafkaConsumerFactory<>(consumerConfigs()));
        //设置并发量，小于或等于Topic的分区数
        factory.setConcurrency(batchConcurrency);
        factory.getContainerProperties().setPollTimeout(1500);
        factory.getContainerProperties().setAckMode(ContainerProperties.AckMode.MANUAL_IMMEDIATE);
        //设置为批量消费，每个批次数量在Kafka配置参数中设置ConsumerConfig.MAX_POLL_RECORDS_CONFIG
        factory.setBatchListener(true);
        return factory;
    }

}
```
同时，新增一个`spring.kafka.consumer.batch.concurrency`变量，用来设置并发数，通过这个参数可以指定几个线程来实现消费。<br />在application.properties配置文件中，添加如下变量
```
#批消费并发量，小于或等于Topic的分区数
spring.kafka.consumer.batch.concurrency = 3

#设置每次批量拉取的最大数量为4000
spring.kafka.consumer.max-poll-records=4000

#设置自动提交改成false
spring.kafka.consumer.enable-auto-commit=false
```
最后，将单个消费方法改成批量消费方法模式
```java
@Component
public class BigDataTopicListener {

    private static final Logger log = LoggerFactory.getLogger(BigDataTopicListener.class);

    /**
     * 监听kafka数据（批量消费）
     * @param consumerRecords
     * @param ack
     */
    @KafkaListener(topics = {"big_data_topic"}, containerFactory = "batchFactory")
    public void batchConsumer(List<ConsumerRecord<?, ?>> consumerRecords, Acknowledgment ack) {
        long start = System.currentTimeMillis();

        //...
        //db.batchSave(consumerRecords);//批量插入或者批量更新数据

        //手动提交
        ack.acknowledge();
        log.info("收到bigData推送的数据，拉取数据量：{}，消费时间：{}ms", consumerRecords.size(), (System.currentTimeMillis() - start));
    }

}
```
此时，消费性能大大的提升，数据处理的非常快，500万条数据，最多 30 分钟就全部消费完毕了。<br />本例中的消费微服务，生产环境部署了3台服务器，同时big_data_topic主题的分区数为3，因此并发数设置为3比较合适。<br />随着推送的数据量不断增加，如果觉得消费速度还不够，可以重新设置每次批量拉取的最大数量，活着横向扩展微服务的集群实例数量和 topic 的分区数，以此来加快数据的消费速度。<br />但是，如果在单台机器中，每次批量拉取的最大数量过大，大对象也会很大，会造成频繁的 gc 告警！<br />因此，在实际的使用过程中，每次批量拉取的最大数量并不是越大越好，根据当前服务器的硬件配置，调节到合适的阀值，才是最优的选择！
<a name="M6OU2"></a>
## 三、小结
本文主要以SpringBoot技术框架为背景，结合实际业务需求，采用 kafka 进行数据消费，实现数据量的高吞吐。
