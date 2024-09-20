JavaSpringBootPulsar<br />作为优秀的消息流平台，Pulsar 的使用越来越多，这篇文章讲解 Pulsar 的 Java 客户端。
<a name="wMYhR"></a>
## 部署 Pulsar
Pulsar 的部署方式主要有 3 种，本地安装二进制文件、docker 部署、在 Kubernetes 上部署。<br />本文采用 docker 部署一个单节点的 Pulsar 集群。实验环境是 2 核 CPU 和 4G 内存。<br />部署命令如下：
```bash
docker run -it -p 6650:6650  -p 8080:8080 --mount source=pulsardata,target=/pulsar/data --mount source=pulsarconf,target=/pulsar/conf apachepulsar/pulsar:2.9.1 bin/pulsar standalone
```
安装过程可能会出现下面的错误：
```
unknown flag: --mount
See 'docker run --help'.
```
这是因为 docker 版本低，不支持 mount 参数，把 docker 版本升级到 17.06 以上就可以了。<br />部署过程中可能会因为网络的原因失败，多试几次就可以成功了。如果看到下面的日志，就说明启动成功了。
```
2022-01-08T22:27:58,726+0000 [main] INFO  org.apache.pulsar.broker.PulsarService - messaging service is ready, bootstrap service port = 8080, broker url= pulsar://localhost:6650, cluster=standalone
```
本地单节点集群启动后，会创建一个 namespace，名字叫 public/default
<a name="CKSr8"></a>
## Pulsar 客户端
目前 Pulsar 支持多种语言的客户端，包括：<br />Java 客户端<br />Go 客户端<br />Python 客户端<br />C++ 客户端<br />Node.js 客户端<br />WebSocket 客户端<br />C# 客户端
<a name="nOpB0"></a>
## SpringBoot 配置
使用 SpringBoot 整合 Pulsar 客户端，首先引入 Pulsar 客户端依赖，代码如下：
```xml
<dependency>
  <groupId>org.apache.pulsar</groupId>
  <artifactId>pulsar-client</artifactId>
  <version>2.9.1</version>
</dependency>
```
然后在 properties 文件中添加配置:
```
# Pulsar 地址
pulsar.url=pulsar://192.168.59.155:6650
# topic
pulsar.topic=testTopic
# consumer group
pulsar.subscription=topicGroup
```
<a name="fRquQ"></a>
## 创建 Client
创建客户端非常简单，代码如下：
```java
client = PulsarClient.builder()
                .serviceUrl(url)
                .build();
```
上面的 url 就是 properties 文件中定义的 pulsar.url 。<br />创建 Client 时，即使集群没有启成功，程序也不会报错，因为这时还没有真正地去连接集群。
<a name="BhCjY"></a>
## 创建 Producer
```java
producer = client.newProducer()
                .topic(topic)
                .compressionType(CompressionType.LZ4)
                .sendTimeout(0, TimeUnit.SECONDS)
                .enableBatching(true)
                .batchingMaxPublishDelay(10, TimeUnit.MILLISECONDS)
                .batchingMaxMessages(1000)
                .maxPendingMessages(1000)
                .blockIfQueueFull(true)
                .roundRobinRouterBatchingPartitionSwitchFrequency(10)
                .batcherBuilder(BatcherBuilder.DEFAULT)
                .create();
```
创建 Producer，会真正的连接集群，这时如果集群有问题，就会报连接错误。<br />下面解释一下创建 Producer 的参数：<br />`**topic**`：Producer 要写入的 topic。<br />`**compressionType**`：压缩策略，目前支持 4 种策略 (NONE、LZ4、ZLIB、ZSTD)，从 Pulsar2.3 开始，只有 Consumer 的版本在 2.3 以上，这个策略才会生效。<br />`**sendTimeout**`：超时时间，如果 Producer 在超时时间为收到 ACK，会进行重新发送。<br />`**enableBatching**`：是否开启消息批量处理，这里默认 true，这个参数只有在异步发送 (sendAsync) 时才能生效，选择同步发送会失效。<br />`**batchingMaxPublishDelay**`：批量发送消息的时间段，这里定义的是 10ms，需要注意的是，设置了批量时间，就不会受消息数量的影响。**批量发送会把要发送的批量消息放在一个网络包里发送出去，减少网络 IO 次数，大大提高网卡的发送效率。**<br />`**batchingMaxMessages**`：批量发送消息的最大数量。<br />`**maxPendingMessages**`：等待从 broker 接收 ACK 的消息队列最大长度。如果这个队列满了，producer 所有的 sendAsync 和 send 都会失败，除非设置了 blockIfQueueFull 值是 true。<br />`**blockIfQueueFull**`：Producer 发送消息时会把消息先放入本地 Queue 缓存，如果缓存满了，就会阻塞消息发送。<br />`**roundRobinRouterBatchingPartition**`**-**`**SwitchFrequency**`：如果发送消息时没有指定 key，那默认采用 round robin 的方式发送消息，使用 round robin 的方式，切换 partition 的周期是 (frequency * batchingMaxPublishDelay)。
<a name="oAtrt"></a>
## 创建 Consumer
Pulsar 的消费模型如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688433687985-294c4bab-5ee8-4537-a1c2-1835af360c0b.png#averageHue=%23332f2f&clientId=u18a2840f-a957-4&from=paste&id=u4649b1d0&originHeight=471&originWidth=571&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u72e847dc-08e9-40c9-a0c0-fc01cce40e1&title=)<br />从图中可以看到，Consumer 要绑定一个 subscription 才能进行消费。
```java
consumer = client.newConsumer()
        .topic(topic)
        .subscriptionName(subscription)
        .subscriptionType(SubscriptionType.Shared)
        .subscriptionInitialPosition(SubscriptionInitialPosition.Earliest)
        .negativeAckRedeliveryDelay(60, TimeUnit.SECONDS)
        .receiverQueueSize(1000)
        .subscribe();
```
下面解释一下创建 Consumer 的参数：<br />`**topic**`：Consumer 要订阅的 topic。<br />`**subscriptionNam**e`：consumer 要关联的 subscription 名字。<br />`**subscriptionType**`：订阅类型，Pulsar 支持四种类型订阅：

- `**Exclusive**`：独占模式，同一个 Topic 只能有一个消费者，如果多个消费者，就会出错。
- `**Failover**`：灾备模式，同一个 Topic 可以有多个消费者，但是只能有一个消费者消费，其他消费者作为故障转移备用，如果当前消费者出了故障，就从备用消费者中选择一个进行消费。如下图：

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688433687662-59049f8f-fee1-4e76-9a49-bbef50944cfe.png#averageHue=%23fcfbfb&clientId=u18a2840f-a957-4&from=paste&id=ue540e1aa&originHeight=403&originWidth=831&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u20fb6e6b-44e4-4eb5-9bda-6821c061465&title=)

- `**Shared**`：共享模式，同一个 Topic 可以由多个消费者订阅和消费。消息通过 round robin 轮询机制分发给不同的消费者，并且每个消息仅会被分发给一个消费者。当消费者断开，如果发送给它消息没有被消费，这些消息会被重新分发给其它存活的消费者。如下图：

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688433687671-75c64c76-ca7a-4b30-b67a-101e571cc59f.png#averageHue=%23fdfbfb&clientId=u18a2840f-a957-4&from=paste&id=z2JXr&originHeight=437&originWidth=829&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue289406b-de8c-4c84-ae3d-52c107f975d&title=)

- `**Key_Shared**`：消息和消费者都会绑定一个key，消息只会发送给绑定同一个key的消费者。如果有新消费者建立连接或者有消费者断开连接，就需要更新一些消息的 key。跟 Shared 模式相比，Key_Shared 的好处是既可以让消费者并发地消费消息，又能保证同一Key下的消息顺序。如下图：

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688433687673-12ee421c-4712-4616-9c25-aa3caaa8ec4e.png#averageHue=%23fbfaf9&clientId=u18a2840f-a957-4&from=paste&id=uc4193600&originHeight=432&originWidth=821&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubb1e037d-6e49-4100-96f7-62b47ddddab&title=)<br />`**subscriptionInitialPosition**`：创建新的 subscription 时从哪里开始消费，有两个选项：

- `**Latest**`：从最新的消息开始消费
- `**Earliest**`：从最早的消息开始消费
- `**negativeAckRedeliveryDelay**`：消费失败后间隔多久 broker 重新发送。
- `**receiverQueueSize**`：在调用 receive 方法之前，最多能累积多少条消息。可以设置为 0，这样每次只从 broker 拉取一条消息。在 Shared 模式下，receiverQueueSize 设置为 0，可以防止批量消息多发给一个 Consumer 而导致其他 Consumer 空闲。

Consumer 接收消息有四种方式：同步单条、同步批量、异步单条和异步批量，代码如下：
```java
Message message = consumer.receive()
CompletableFuture<Message> message = consumer.receiveAsync();
Messages message = consumer.batchReceive();
CompletableFuture<Messages> message = consumer.batchReceiveAsync();
```
对于批量接收，也可以设置批量接收的策略，代码如下：
```java
consumer = client.newConsumer()
    .topic(topic)
    .subscriptionName(subscription)
        .batchReceivePolicy(BatchReceivePolicy.builder()
        .maxNumMessages(100)
        .maxNumBytes(1024 * 1024)
        .timeout(200, TimeUnit.MILLISECONDS)
        .build())
    .subscribe();
```
代码中的参数说明如下：<br />`**maxNumMessages**`：批量接收的最大消息数量。<br />`**maxNumBytes**`：批量接收消息的大小，这里是 1MB。
<a name="EJJNa"></a>
## 测试
首先编写 Producer 发送消息的代码，如下：
```java
public void sendMsg(String key, String data) {
    CompletableFuture<MessageId> future = producer.newMessage()
    .key(key)
    .value(data.getBytes()).sendAsync();
    future.handle((v, ex) -> {
        if (ex == null) {
            logger.info("发送消息成功, key:{}, msg: {}", key, data);
        } else {
            logger.error("发送消息失败, key:{}, msg: {}", key, data);
        }
        return null;
    });
    future.join();
    logger.info("发送消息完成, key:{}, msg: {}", key, data);
}
```
然后编写一个 Consumer 消费消息的代码，如下：
```java
public void start() throws Exception{
    while (true) {
        Message message = consumer.receive();
        String key = message.getKey();
        String data = new String(message.getData());
        String topic = message.getTopicName();
        if (StringUtils.isNotEmpty(data)) {
            try{
                logger.info("收到消息, topic:{}, key:{}, data:{}", topic, key, data);
            }catch(Exception e){
                logger.error("接收消息异常，topic:{}, key:{}, data:{}", topic, key, data, e);
            }
        }
        consumer.acknowledge(message);
    }
}
```
最后编写一个 Controller 类，调用 Producer 发送消息，代码如下：
```java
@RequestMapping("/send")
@ResponseBody
public String send(@RequestParam String key, @RequestParam String data) {
    logger.info("收到消息发送请求, key:{}, value:{}", key, data);
    pulsarProducer.sendMsg(key, data);
    return "success";
}
```
调用 Producer 发送一条消息，key=key1，data=data1，具体操作为在浏览器中输入下面的 url 后回车：<br />http://192.168.157.1:8083/pulsar/send?key=key1&data=data1<br />可以看到控制台输出下面日志：
```
2022-01-08 22:42:33,199 [pulsar-client-io-6-1] [INFO] boot.pulsar.PulsarProducer - 发送消息成功, key:key1, msg: data1
2022-01-08 22:42:33,200 [http-nio-8083-exec-1] [INFO] boot.pulsar.PulsarProducer - 发送消息完成, key:key1, msg: data1
2022-01-08 22:42:33,232 [Thread-22] [INFO] boot.pulsar.PulsarConsumer - 收到消息, topic:persistent://public/default/testTopic, key:key1, data:data1
2022-01-08 22:43:14,498 [pulsar-timer-5-1] [INFO] org.apache.pulsar.client.impl.ConsumerStatsRecorderImpl - [testTopic] [topicGroup] [7def6] Prefetched messages: 0 --- Consume throughput received: 0.02 msgs/s --- 0.00 Mbit/s --- Ack sent rate: 0.02 ack/s --- Failed messages: 0 --- batch messages: 0 ---Failed acks: 0
2022-01-08 22:43:14,961 [pulsar-timer-9-1] [INFO] org.apache.pulsar.client.impl.ProducerStatsRecorderImpl - [testTopic] [standalone-9-0] Pending messages: 0 --- Publish throughput: 0.02 msg/s --- 0.00 Mbit/s --- Latency: med: 69.000 ms - 95pct: 69.000 ms - 99pct: 69.000 ms - 99.9pct: 69.000 ms - max: 69.000 ms --- Ack received rate: 0.02 ack/s --- Failed messages: 0
```
从日志中看到，这里使用的 namespace 就是创建集群时生成的public/default。
<a name="xQrOY"></a>
## 总结
从 SpringBoot 整合 Java 客户端使用来看，Pulsar 的 api 是非常友好的，使用起来方便简洁。Consumer 的使用需要考虑多一些，需要考虑到批量、异步以及订阅类型。
