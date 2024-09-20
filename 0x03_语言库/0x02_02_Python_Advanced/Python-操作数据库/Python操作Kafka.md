Python<br />Kafka是最初由Linkedin公司开发，是一个分布式、分区的、多副本的、多订阅者，基于zookeeper协调的分布式日志系统（也可以当做MQ系统），常见可以用于web/nginx日志、访问日志，消息服务等等。今天给大家带来用Python操作Kafka的知识介绍。
<a name="s7t2t"></a>
## Kafka的使用场景

- 日志收集：一个公司可以用Kafka可以收集各种服务的log，通过kafka以统一接口服务的方式开放给各种consumer，例如hadoop、Hbase、Solr等。
- 消息系统：解耦和生产者和消费者、缓存消息等。
- 用户活动跟踪：Kafka经常被用来记录web用户或者app用户的各种活动，如浏览网页、搜索、点击等活动，这些活动信息被各个服务器发布到kafka的topic中，然后订阅者通过订阅这些topic来做实时的监控分析，或者装载到hadoop、数据仓库中做离线分析和挖掘。
- 运营指标：Kafka也经常用来记录运营监控数据。包括收集各种分布式应用的数据，生产各种操作的集中反馈，比如报警和报告。
- 流式处理：比如spark streaming和storm。
<a name="j9GDY"></a>
## Kafka的基本术语概念

- Topic：一组消息数据的标记符；
- Producer：生产者，用于生产数据，可将生产后的消息送入指定的Topic；
- Consumer：消费者，获取数据，可消费指定的Topic；
- Group：消费者组，同一个group可以有多个消费者，一条消息在一个group中，只会被一个消费者获取；
- Partition：分区，为了保证kafka的吞吐量，一个Topic可以设置多个分区。同一分区只能被一个消费者订阅。
<a name="Cq5B3"></a>
## Python操作Kafka
安装kafka-python
```bash
pip install kafka-python
```
生产者和消费者的简易Demo
```python
from kafka import KafkaProducer, KafkaConsumer
from kafka.errors import kafka_errors
import traceback
import json


def producer_demo():
    # 假设生产的消息为键值对（不是一定要键值对），且序列化方式为json
    producer = KafkaProducer(
        bootstrap_servers=['localhost:9092'], 
        key_serializer=lambda k: json.dumps(k).encode(),
        value_serializer=lambda v: json.dumps(v).encode())
    # 发送三条消息
    for i in range(0, 3):
        future = producer.send(
            'kafka_demo',
            key='count_num',  # 同一个key值，会被送至同一个分区
            value=str(i),
            partition=1)  # 向分区1发送消息
        print("send {}".format(str(i)))
        try:
            future.get(timeout=10) # 监控是否发送成功           
        except kafka_errors:  # 发送失败抛出kafka_errors
            traceback.format_exc()


def consumer_demo():
    consumer = KafkaConsumer(
        'kafka_demo', 
        bootstrap_servers=':9092',
        group_id='test'
    )
    for message in consumer:
        print("receive, key: {}, value: {}".format(
            json.loads(message.key.decode()),
            json.loads(message.value.decode())
            )
        )
```
<a name="xo8Kb"></a>
### 重要参数详解

- group_id

高并发量，则需要有多个消费者协作，消费进度，则由group_id统一。例如消费者A与消费者B，在初始化时使用同一个group_id。在进行消费时，一条消息被消费者A消费后，在kafka中会被标记，这条消息不会再被B消费（前提是A消费后正确commit）。当一个新的group_id产生时，默认offset=latest。

- key_deserializer， value_deserializer

自动解析序列化

- auto_offset_reset

消费者启动的时刻，消息队列中或许已经有堆积的未消费消息，有时候需求是从上一次未消费的位置开始读（则该参数设置为earliest），有时候的需求为从当前时刻开始读之后产生的，之前产生的数据不再消费（则该参数设置为latest）。

- enable_auto_commit， auto_commit_interval_ms

是否自动commit，当前消费者消费完该数据后，需要commit，才可以将消费完的信息传回消息队列的控制中心。enable_auto_commit设置为True后，消费者将自动commit，并且两次commit的时间间隔为auto_commit_interval_ms。
```python
def consumer_demo():
    consumer = KafkaConsumer(
        'kafka_demo', 
        bootstrap_servers=':9092',
        group_id='test',
        enable_auto_commit=False
    )
    for message in consumer:
        print("receive, key: {}, value: {}".format(
            json.loads(message.key.decode()),
            json.loads(message.value.decode())
            )
        )
        consumer.commit()
```
<a name="raYGC"></a>
## 总结
Kafka作为最重要的消息中间件之一，使用频率很高，通过Python去操作Kafka除了解决业务问题，也可以方便编写一些测试小工具来对业务进行调试。
