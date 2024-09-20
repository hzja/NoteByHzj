JavaSpringKafka
<a name="F8xXB"></a>
## **消息监听容器**
<a name="ghoRE"></a>
### 1、`KafkaMessageListenerContainer`
由spring提供用于监听以及拉取消息，并将这些消息按指定格式转换后交给由`@KafkaListener`注解的方法处理，相当于一个消费者；<br />看看其整体代码结构：<br />![2022-06-09-09-03-51-525023.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1654736924122-9d3154e2-58b8-461d-8db3-ba38ed0cf7d6.png#clientId=u5d0902c0-d9d2-4&from=ui&id=u865b2704&originHeight=1105&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3587178&status=done&style=none&taskId=u9862583c-f442-4b17-8308-91e2e79fc5d&title=)

- 可以发现其入口方法为`doStart()`，往上追溯到实现了`SmartLifecycle`接口，很明显，由spring管理其start和stop操作；
- `ListenerConsumer`，内部真正拉取消息消费的是这个结构，其 实现了Runable接口，简言之，它就是一个后台线程轮训拉取并处理消息
- 在`doStart`方法中会创建`ListenerConsumer`并交给线程池处理
- 以上步骤就开启了消息监听过程
<a name="ObPIh"></a>
### `KafkaMessageListenerContainer#doStart`
```java
protected void doStart() {
	if (isRunning()) {
		return;
	}
	if (this.clientIdSuffix == null) { // stand-alone container
		checkTopics();
	}
	ContainerProperties containerProperties = getContainerProperties();
	checkAckMode(containerProperties);
	......
	// 创建ListenerConsumer消费者并放入到线程池中执行
	this.listenerConsumer = new ListenerConsumer(listener, listenerType);
	setRunning(true);
	this.startLatch = new CountDownLatch(1);
	this.listenerConsumerFuture = containerProperties
		.getConsumerTaskExecutor()
		.submitListenable(this.listenerConsumer);
	......
}
```
<a name="PohCZ"></a>
### `KafkaMessageListenerContainer.ListenerConsumer#run`
```java
public void run() { // NOSONAR complexity
	.......
	Throwable exitThrowable = null;
	while (isRunning()) {
		try {
			// 拉去消息并处理消息
			pollAndInvoke();
		}
		catch (@SuppressWarnings(UNUSED) WakeupException e) {
			......
		}
		......
	}
	wrapUp(exitThrowable);
}
```
<a name="HiUsX"></a>
## 2、`ConcurrentMessageListenerContainer`
并发消息监听，相当于创建消费者；其底层逻辑仍然是通过`KafkaMessageListenerContainer`实现处理；从实现上看就是在`KafkaMessageListenerContainer`上做了层包装，有多少的`concurrency`就创建多个`KafkaMessageListenerContainer`，也就是concurrency个消费者<br />![2022-06-09-09-03-51-634004.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1654736833370-013a6e7b-b9e9-4df0-aae5-fa11a0bc93b6.png#clientId=u5d0902c0-d9d2-4&from=ui&id=ud1b6e406&originHeight=588&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1908866&status=done&style=none&taskId=u1c3f0184-2075-413d-abfc-0342b620d64&title=)
<a name="QGNCd"></a>
### `ConcurrentMessageListenerContainer#doStart`
```java
protected void doStart() {
	if (!isRunning()) {
		checkTopics();
		......
		setRunning(true);
		for (int i = 0; i < this.concurrency; i++) {
			KafkaMessageListenerContainer<K, V> container =
				constructContainer(containerProperties, topicPartitions, i);
			String beanName = getBeanName();
			container.setBeanName((beanName != null ? beanName : "consumer") + "-" + i);

			......

				if (isPaused()) {
					container.pause();
				}
			// 这里调用KafkaMessageListenerContainer启动相关监听方法
			container.start();
			this.containers.add(container);
		}
	}
}
```
<a name="guFE3"></a>
### `@KafkaListener`底层监听原理
上面已经介绍了`KafkaMessageListenerContainer`的作用是拉取并处理消息，但还缺少关键的一步，即 如何将业务逻辑与`KafkaMessageListenerContainer`的处理逻辑联系起来？<br />那么这个桥梁就是`@KafkaListener`注解

- `KafkaListenerAnnotationBeanPostProcessor`，从后缀`BeanPostProcessor`就可以知道这是Spring IOC初始化bean相关的操作，当然这里也是；此类会扫描带`@KafkaListener`注解的类或者方法，通过 `KafkaListenerContainerFactory`工厂创建对应的`KafkaMessageListenerContainer`，并调用start方法启动监听，也就是这样打通了这条路…
<a name="sCoZW"></a>
## Spring Boot 自动加载kafka相关配置
<a name="Pzr5g"></a>
### 1、`KafkaAutoConfiguration`

- 自动生成kafka相关配置，比如当缺少这些bean的时候KafkaTemplate、ProducerListener、ConsumerFactory、ProducerFactory等，默认创建bean实例
<a name="c0Baw"></a>
### 2、`KafkaAnnotationDrivenConfiguration`

- 主要是针对于spring-kafka提供的注解背后的相关操作，比如 `@KafkaListener`;
- 在开启了`@EnableKafka`注解后，spring会扫描到此配置并创建缺少的bean实例，比如当配置的工厂beanName不是`kafkaListenerContainerFactory`的时候，就会默认创建一个beanName为`kafkaListenerContainerFactory`的实例，这也是为什么在springboot中不用定义consumer的相关配置也可以通过`@KafkaListener`正常的处理消息
<a name="NDUce"></a>
### 生产配置
<a name="TV4EQ"></a>
#### 1、单条消息处理
```java
@Configuration
@EnableKafka
public class KafkaConfig {

    @Bean
    KafkaListenerContainerFactory<ConcurrentMessageListenerContainer<Integer, String>>
                        kafkaListenerContainerFactory() {
        ConcurrentKafkaListenerContainerFactory<Integer, String> factory =
                                new ConcurrentKafkaListenerContainerFactory<>();
        factory.setConsumerFactory(consumerFactory());
        factory.setConcurrency(3);
        factory.getContainerProperties().setPollTimeout(3000);
        return factory;
    }
    
    @Bean
    public ConsumerFactory<Integer, String> consumerFactory() {
        return new DefaultKafkaConsumerFactory<>(consumerConfigs());
    }
    
    @Bean
    public Map<String, Object> consumerConfigs() {
        Map<String, Object> props = new HashMap<>();
        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, embeddedKafka.getBrokersAsString());
        ...
        return props;
    }
}

@KafkaListener(id = "myListener", topics = "myTopic",
        autoStartup = "${listen.auto.start:true}", concurrency = "${listen.concurrency:3}")
public void listen(String data) {
    ...
}
```
<a name="Esk6D"></a>
#### 2、批量处理
```java
@Configuration
@EnableKafka
public class KafkaConfig {

    @Bean
public KafkaListenerContainerFactory<?, ?> batchFactory() {
    ConcurrentKafkaListenerContainerFactory<Integer, String> factory =
            new ConcurrentKafkaListenerContainerFactory<>();
    factory.setConsumerFactory(consumerFactory());
    factory.setBatchListener(true);  // <<<<<<<<<<<<<<<<<<<<<<<<<
    return factory;
}

    @Bean
    public ConsumerFactory<Integer, String> consumerFactory() {
        return new DefaultKafkaConsumerFactory<>(consumerConfigs());
    }
    
    @Bean
    public Map<String, Object> consumerConfigs() {
        Map<String, Object> props = new HashMap<>();
        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, embeddedKafka.getBrokersAsString());
        ...
        return props;
    }
}

@KafkaListener(id = "list", topics = "myTopic", containerFactory = "batchFactory")
public void listen(List<String> list) {
    ...
}
```
<a name="yCKeu"></a>
#### 3、同一个消费组支持单条和批量处理
场景：<br />生产上最初都采用单条消费模式，随着量的积累，部分topic常常出现消息积压，最开始通过新增消费者实例和分区来提升消费端的能力；一段时间后又开始出现消息积压，由此便从代码层面通过批量消费来提升消费能力。
<a name="a4MUP"></a>
#### 4、只对部分topic做批量消费处理
简单的说就是需要配置批量消费和单条记录消费(从单条消费逐步向批量消费演进)

- 假设最开始就是配置的单条消息处理的相关配置，原配置基本不变
- 然后新配置 批量消息监听`KafkaListenerContainerFactory`
```java
@Configuration
@EnableKafka
public class KafkaConfig {

    @Bean(name = [batchListenerContainerFactory])
public KafkaListenerContainerFactory<?, ?> batchFactory() {
    ConcurrentKafkaListenerContainerFactory<Integer, String> factory =
            new ConcurrentKafkaListenerContainerFactory<>();
    factory.setConsumerFactory(consumerFactory());
    // 开启批量处理
    factory.setBatchListener(true); 
    return factory;
}

    @Bean(name = [batchConsumerFactory])
    public ConsumerFactory<Integer, String> consumerFactory() {
        return new DefaultKafkaConsumerFactory<>(consumerConfigs());
    }
    
    @Bean(name = [batchConsumerConfig])
    public Map<String, Object> consumerConfigs() {
        Map<String, Object> props = new HashMap<>();
        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, embeddedKafka.getBrokersAsString());
        ...
        return props;
    }
}
```
注意点：

- 如果自定义的`ContainerFactory`其beanName不是`kafkaListenerContainerFactory`，spring会通过`KafkaAnnotationDrivenConfiguration`创建新的bean实例，所以需要注意的是最终的`@KafkaListener`会使用到哪个`ContainerFactory`
- 单条或在批量处理的`ContainerFactory`可以共存，默认会使用beanName为`kafkaListenerContainerFactory`的bean实例，因此可以为batch container Factory实例指定不同的beanName，并在`@KafkaListener`使用的时候指定`containerFactory`即可
<a name="wIO5f"></a>
## 总结

- spring为了将kafka融入其生态，方便在spring大环境下使用kafka，开发了spring-kafa这一模块，本质上是为了帮助开发者更好的以spring的方式使用kafka
- `@KafkaListener`就是这么一个工具，在同一个项目中既可以有单条的消息处理，也可以配置多条的消息处理，稍微改变下配置即可实现，很是方便
- 当然，`@KafkaListener`单条或者多条消息处理仍然是spring自行封装处理，与kafka-client客户端的拉取机制无关；比如一次性拉取50条消息，对于单条处理来说就是循环50次处理，而多条消息处理则可以一次性处理50条；本质上来说这套逻辑都是spring处理的，并不是说单条消费就是通过kafka-client一次只拉取一条消息
- 在使用过程中需要注意spring自动的创建的一些bean实例，当然也可以覆盖其自动创建的实例以满足特定的需求场景

调试及相关源码版本：

- org.springframework.boot:2.3.3.RELEASE
- spring-kafka:2.5.4.RELEASE
