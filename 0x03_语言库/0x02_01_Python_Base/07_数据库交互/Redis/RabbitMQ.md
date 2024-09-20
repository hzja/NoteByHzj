# RabbitMQ

RabbitMQ 即一个消息队列，主要是用来实现应用程序的异步和解耦，同时也能起到消息缓冲，消息分发的作用。可维护多个队列，可实现消息的一对一和广播等方式发送

RabbitMQ是一个开源的AMQP实现，服务器端用Erlang语言编写，支持多种客户端。用于在分布式系统中存储转发消息，在易用性、扩展性、高可用性等方面表现不俗。



## 安装

~~~ shell
curl -s https://packagecloud.io/install/repositories/rabbitmq/erlang/script.rpm.sh | sudo bash
curl -s https://packagecloud.io/install/repositories/rabbitmq/rabbitmq-server/script.rpm.sh | sudo bash

yum -y install rabbitmq-server
# 启动
systemctl start rabbitmq-server
# 查看状态
rabbitmqctl status
# 启用插件
rabbitmq-plugins enable rabbitmq_management
# 创建用户
rabbitmqctl add_user admin admin
# 修改角色为管理员
rabbitmqctl set_permissions -p / admin "." "." ".*"
# 设置权限
rabbitmqctl set_user_tags admin administrator

# 得到所有队列及存在的数据条数
rabbitmqctl list_queues
~~~

![image6](D:\Note\python\数据库交互\图片\image6.png)



## RabbitMQ特点

RabbitMQ 是一个由 Erlang 语言开发的 AMQP 的开源实现。

AMQP ：Advanced Message Queue，高级消息队列协议。它是应用层协议的一个开放标准，为面向消息的中间件设计，基于此协议的客户端与消息中间件可传递消息，并不受产品、开发语言等条件的限制。

RabbitMQ 最初起源于金融系统，用于在分布式系统中存储转发消息，在易用性、扩展性、高可用性等方面表现不俗。具体特点包括：

1. 可靠性（Reliability）
   RabbitMQ 使用一些机制来保证可靠性，如持久化、传输确认、发布确认。
2. 灵活的路由（Flexible Routing）
   在消息进入队列之前，通过 Exchange 来路由消息的。对于典型的路由功能，RabbitMQ 已经提供了一些内置的 Exchange 来实现。针对更复杂的路由功能，可以将多个 Exchange 绑定在一起，也通过插件机制实现自己的 Exchange 。
3. 消息集群（Clustering）
   多个 RabbitMQ 服务器可以组成一个集群，形成一个逻辑 Broker 。
4. 高可用（Highly Available Queues）
   队列可以在集群中的机器上进行镜像，使得在部分节点出问题的情况下队列仍然可用。
5. 多种协议（Multi-protocol）
   RabbitMQ 支持多种消息队列协议，比如 STOMP、MQTT 等等。
6. 多语言客户端（Many Clients）
   RabbitMQ 几乎支持所有常用语言，比如 Java、.NET、Ruby 等等。
7. 管理界面（Management UI）
   RabbitMQ 提供了一个易用的用户界面，使得用户可以监控和管理消息 Broker 的许多方面。
8. 跟踪机制（Tracing）
   如果消息异常，RabbitMQ 提供了消息跟踪机制，使用者可以找出发生了什么。
9. 插件机制（Plugin System）
   RabbitMQ 提供了许多插件，来从多方面进行扩展，也可以编写自己的插件。



## RabbitMQ 持久化

MQ默认建立的是临时 queue 和 exchange，如果不声明持久化，一旦 rabbitmq 挂掉，queue、exchange 将会全部丢失。所以我们一般在创建 queue 或者 exchange 的时候会声明 持久化。

1. queue 声明持久化

~~~ python
# 声明消息队列，消息将在这个队列传递，如不存在，则创建。durable = True 代表消息队列持久化存储，False 非持久化存储
result = channel.queue_declare(queue = 'python-queue',durable = True)
~~~



2. exchange 声明持久化

~~~ python
# 声明exchange，由exchange指定消息在哪个队列传递，如不存在，则创建.durable = True 代表exchange持久化存储，False 非持久化存储
channel.exchange_declare(exchange = 'python-exchange', durable = True)
~~~

注意：如果已存在一个非持久化的 queue 或 exchange ，执行上述代码会报错，因为当前状态不能更改 queue 或 exchange 存储属性，需要删除重建。如果 queue 和 exchange 中一个声明了持久化，另一个没有声明持久化，则不允许绑定。



3. 消息持久化

虽然 exchange 和 queue 都申明了持久化，但如果消息只存在内存里，rabbitmq 重启后，内存里的东西还是会丢失。所以必须声明消息也是持久化，从内存转存到硬盘。

~~~ python
# 向队列插入数值 routing_key是队列名。delivery_mode = 2 声明消息在队列中持久化，delivery_mod = 1 消息非持久化
channel.basic_publish(exchange = 'python-exchange', routing_key = 'python-routing_key', body = message, properties=pika.BasicProperties(delivery_mode = 2))
~~~



4. acknowledgement 消息不丢失

消费者（consumer）调用callback函数时，会存在处理消息失败的风险，如果处理失败，则消息丢失。但是也可以选择消费者处理失败时，将消息回退给 rabbitmq ，重新再被消费者消费，这个时候需要设置确认标识。

~~~ python
# no_ack 设置成 False，在调用callback函数时，未收到确认标识，消息会重回队列。True，无论调用callback成功与否，消息都被消费掉
channel.basic_consume(on_message_callback = callback, queue = 'python-queue', no_ack = False)
~~~



## RabbitMQ发布与订阅

rabbitmq 的发布与订阅要借助交换机（Exchange）的原理实现：

![1236854-20190312103044651-543462222](D:\Note\python\数据库交互\图片\1236854-20190312103044651-543462222.png)

Exchange 一共有三种工作模式：**fanout, direct, topicd**

**安装Pika**

~~~ python 
pip install pika
~~~



### 模式一：fanout

这种模式下，传递到 exchange 的消息将会**转发到所有**与其绑定的 queue 上。很像子网广播，每台子网内的主机都获得了一份复制的消息。fanout 类型转发消息是最快的。

- 不需要指定 routing_key ，即使指定了也是无效。
- 需要提前将 exchange 和 queue 绑定，一个 exchange 可以绑定多个 queue，一个queue可以绑定多个exchange。
- 需要先启动**订阅者**，此模式下的队列是 consumer 随机生成的，**发布者**仅仅发布消息到 exchange ，由 exchange 转发消息至 queue。



**发布者：**

~~~ python
import json
import pika
import datetime


# 生成消息入口处
def get_message():
    for i in range(100):  # 生成10条消息
        message = json.dumps(
            {'id': "10000%s" % i, "amount": 100 * i, "name": "tony", "createtime": str(datetime.datetime.now())})
        producter(message)


def producter(message):  # 消息生产者
    exchange_name = "rt_default_y"  # 主题
    routing_key = '' # 路由键

    # 建立连接与rabbitmq 服务的连接，虚拟队列需要指定参数 virtual_host，如果是默认的可以不填（默认为/)，也可以自己创建一个
    userx = pika.PlainCredentials("admin", "admin")  # 账号密码
    conn = pika.BlockingConnection(pika.ConnectionParameters("192.168.131.128", 5672, '/', credentials=userx))

    # 创建一个 AMQP 信道（Channel）,建造一个大邮箱，隶属于这家邮局的邮箱
    channel = conn.channel()

    # 声明消息队列rt_autoidc_queue，消息将在这个队列传递，如不存在，则创建, exchange_type表示使用那种工作模式，durable = True代表exchange持久化存储，False 非持久化存储
    channel.exchange_declare(exchange=exchange_name, exchange_type='fanout', durable=True)

    # 向队列插入数值routing_key为空，body 就是放入的消息内容，exchange指定消息在哪个队列传递，这里是空的exchange但仍然能够发送消息到队列中，因为我们使用的是我们定义的空字符串“”exchange（默认的exchange）
    channel.basic_publish(exchange=exchange_name,  # 确定发布主题为
                          routing_key=routing_key,  # 路由键
                          body=message,  # 发送的数据
                          properties=pika.BasicProperties(delivery_mode=2)) # delivery_mode = 2 声明消息在队列中持久化，delivery_mod = 1 消息非持久化

    # 关闭连接
    conn.close()


if __name__ == '__main__':
    try:
        get_message()
    except KeyboardInterrupt:
        print("Quit")
~~~



![image](D:\Note\python\数据库交互\图片\image7.png)



**订阅者：**

~~~ python
import pika


def rt_receive():
    exchange_name = "rt_default_y"  # 主题
    routing_key = '' # 路由键

    # 建立连接
    userx = pika.PlainCredentials("admin", "admin")  # 账号密码
    conn = pika.BlockingConnection(pika.ConnectionParameters("192.168.131.128", 5672, '/', credentials=userx))

    # 创建一个信道
    channel = conn.channel()
    # 创建临时队列,队列名传空字符，consumer关闭后，队列自动删除
    result = channel.queue_declare('', exclusive=True)

    # 声明exchange的类型，由exchange指定消息在哪个队列传递，如不存在，则创建，exchange_type表示使用那种工作模式，durable = True代表exchange持久化存储，False 非持久化存储
    channel.exchange_declare(exchange=exchange_name, exchange_type='fanout', passive=False, durable=True, auto_delete=False)

    # 消费者绑定queue，确定订阅主题为rt_autoidc_queue
    channel.queue_bind(queue=result.method.queue, exchange=exchange_name, routing_key=routing_key)

    # 收到指定消息的回调设置，auto_ack设置成False，在调用callback函数时，未收到确认标识，消息会重回队列。设置成True，无论调用callback成功与否，消息都被消费掉
    channel.basic_consume(queue=result.method.queue,  # 队列名
                          on_message_callback=callback,  # 收到消息的回调函数
                          auto_ack=False)
    # 开始循环监听
    channel.start_consuming()


# 回调函数
def callback(ch, method, properties, body):
    print(ch, method, properties, body)
    # 是否消费队列，可以添加判断条件
    # if models_info == True:
    # ch.basic_ack(delivery_tag=method.delivery_tag)


if __name__ == '__main__':
    try:
        rt_receive()
    except KeyboardInterrupt:
        print("Quit")
~~~



返回结果：

~~~ python
<BlockingChannel impl=<Channel number=1 OPEN conn=<SelectConnection OPEN transport=<pika.adapters.utils.io_services_utils._AsyncPlaintextTransport object at 0x00000192E30F3388> params=<ConnectionParameters host=192.168.131.128 port=5672 virtual_host=/ ssl=False>>>> <Basic.Deliver(['consumer_tag=ctag1.52d63d66ec2c41a5ba91ee2ee3192b56', 'delivery_tag=1', 'exchange=rt_default_y', 'redelivered=False', 'routing_key='])> <BasicProperties(['delivery_mode=2'])> b'{"id": "100000", "amount": 0, "name": "tony", "createtime": "2021-04-27 22:04:48.859094"}'
<BlockingChannel impl=<Channel number=1 OPEN conn=<SelectConnection OPEN transport=<pika.adapters.utils.io_services_utils._AsyncPlaintextTransport object at 0x00000192E30F3388> params=<ConnectionParameters host=192.168.131.128 port=5672 virtual_host=/ ssl=False>>>> <Basic.Deliver(['consumer_tag=ctag1.52d63d66ec2c41a5ba91ee2ee3192b56', 'delivery_tag=2', 'exchange=rt_default_y', 'redelivered=False', 'routing_key='])> <BasicProperties(['delivery_mode=2'])> b'{"id": "100001", "amount": 100, "name": "tony", "createtime": "2021-04-27 22:04:48.884085"}'
<BlockingChannel impl=<Channel number=1 OPEN conn=<SelectConnection OPEN transport=<pika.adapters.utils.io_services_utils._AsyncPlaintextTransport object at 0x00000192E30F3388> params=<ConnectionParameters host=192.168.131.128 port=5672 virtual_host=/ ssl=False>>>> <Basic.Deliver(['consumer_tag=ctag1.52d63d66ec2c41a5ba91ee2ee3192b56', 'delivery_tag=3', 'exchange=rt_default_y', 'redelivered=False', 'routing_key='])> <BasicProperties(['delivery_mode=2'])> b'{"id": "100002", "amount": 200, "name": "tony", "createtime": "2021-04-27 22:04:48.898071"}'

~~~

![image8](D:\Note\python\数据库交互\图片\image8.png)



### 模式二：direct

这种工作模式的原理是 消息发送至 exchange，exchange 根据 **路由键（routing_key）**转发到相对应的 queue 上。

-  可以使用默认 exchange =' ' ，也可以自定义 exchange
- 这种模式下不需要将 exchange 和 任何进行绑定，当然绑定也是可以的。可以将 exchange 和 queue ，routing_key 和 queue 进行绑定
- 传递或接受消息时 需要 **指定 routing_key**
- 需要先启动 **订阅者**，此模式下的队列是 consumer 随机生成的，**发布者** 仅仅发布消息到 exchange ，由 exchange 转发消息至 queue。

**发布者：**

如果生成多个的话，实现效果是轮询发送，一个一个循环发送数据，如同“皇帝轮流做…”

~~~ python
import json
import pika
import datetime


# 生成消息入口处
def get_message():
    for i in range(10):  # 生成10条消息
        message = json.dumps(
            {'id': "10000%s" % i, "amount": 100 * i, "name": "tony", "createtime": str(datetime.datetime.now())})
        producter(message)


def producter(message):  # 消息生产者
    exchange_name = "rt_default_x"  # 主题
    routing_key = 'rt_autoidc_queue' # 路由键

    # 建立连接与rabbitmq 服务的连接，虚拟队列需要指定参数 virtual_host，如果是默认的可以不填（默认为/)，也可以自己创建一个
    userx = pika.PlainCredentials("admin", "admin")
    conn = pika.BlockingConnection(pika.ConnectionParameters("192.168.131.128", 5672, '/', credentials=userx))

    # 创建一个 AMQP 信道（Channel）,建造一个大邮箱，隶属于这家邮局的邮箱
    channel = conn.channel()

    # 声明消息队列rt_autoidc_queue，消息将在这个队列传递，如不存在，则创建, exchange_type表示使用那种工作模式，durable = True代表exchange持久化存储，False 非持久化存储
    channel.exchange_declare(exchange=exchange_name, exchange_type='direct', durable=True)

    # 向队列插入数值routing_key的队列名为rt_autoidc_queue，body 就是放入的消息内容，exchange指定消息在哪个队列传递，这里是空的exchange但仍然能够发送消息到队列中，因为我们使用的是我们定义的空字符串“”exchange（默认的exchange）
    channel.basic_publish(exchange=exchange_name,  # 确定发布主题为
                          routing_key=routing_key,  # 路由键
                          body=message,  # 发送的数据
                          properties=pika.BasicProperties(delivery_mode=2)) # delivery_mode = 2 声明消息在队列中持久化，delivery_mod = 1 消息非持久化

    # 关闭连接
    conn.close()


if __name__ == '__main__':
    try:
        get_message()
    except KeyboardInterrupt:
        print("Quit")
~~~

**订阅者：**

~~~ python
import pika


def rt_receive():
    exchange_name = "rt_default_x"  # 主题
    routing_key = 'rt_autoidc_queue' # 路由键

    # 建立连接
    userx = pika.PlainCredentials("admin", "admin")  # 账号密码
    conn = pika.BlockingConnection(pika.ConnectionParameters("192.168.131.128", 5672, '/', credentials=userx))

    # 创建一个信道
    channel = conn.channel()
    # 创建临时队列，队列名传空字符，consumer关闭后，队列自动删除
    result = channel.queue_declare('', exclusive=True)

    # 声明exchange的类型，由exchange指定消息在哪个队列传递，如不存在，则创建，exchange_type表示使用那种工作模式，durable = True代表exchange持久化存储，False 非持久化存储
    channel.exchange_declare(exchange=exchange_name, exchange_type='direct', passive=False, durable=True, auto_delete=False)

    # 消费者绑定queue，确定订阅主题为rt_autoidc_queue
    channel.queue_bind(queue=result.method.queue, exchange=exchange_name, routing_key=routing_key)

    # 收到指定消息的回调设置，auto_ack设置成False，在调用callback函数时，未收到确认标识，消息会重回队列。设置成True，无论调用callback成功与否，消息都被消费掉
    channel.basic_consume(queue=result.method.queue,  # 队列名
                          on_message_callback=callback,  # 收到消息的回调函数
                          auto_ack=False)
    # 开始循环监听
    channel.start_consuming()


# 回调函数
def callback(ch, method, properties, body):
    print(ch, method, properties, body)
    # 是否消费队列，可以添加判断条件
    # if models_info == True:
    #     ch.basic_ack(delivery_tag=method.delivery_tag)


if __name__ == '__main__':
    try:
        rt_receive()
    except KeyboardInterrupt:
        print("Quit")
~~~

**订阅者：**

~~~ python
import pika


def rt_receive():
    exchange_name = "rt_default_x"  # 主题
    routing_key = 'rt_autoidc_queue' # 路由键

    # 建立连接
    userx = pika.PlainCredentials("admin", "admin")  # 账号密码
    conn = pika.BlockingConnection(pika.ConnectionParameters("192.168.131.128", 5672, '/', credentials=userx))

    # 创建一个信道
    channel = conn.channel()
    # 创建临时队列，队列名传空字符，consumer关闭后，队列自动删除
    result = channel.queue_declare('', exclusive=True)

    # 声明exchange的类型，由exchange指定消息在哪个队列传递，如不存在，则创建，exchange_type表示使用那种工作模式，durable = True代表exchange持久化存储，False 非持久化存储
    channel.exchange_declare(exchange=exchange_name, exchange_type='direct', passive=False, durable=True, auto_delete=False)

    # 消费者绑定queue，确定订阅主题为rt_autoidc_queue
    channel.queue_bind(queue=result.method.queue, exchange=exchange_name, routing_key=routing_key)

    # 收到指定消息的回调设置，auto_ack设置成False，在调用callback函数时，未收到确认标识，消息会重回队列。设置成True，无论调用callback成功与否，消息都被消费掉
    channel.basic_consume(queue=result.method.queue,  # 队列名
                          on_message_callback=callback,  # 收到消息的回调函数
                          auto_ack=False)
    # 开始循环监听
    channel.start_consuming()


# 回调函数
def callback(ch, method, properties, body):
    print(ch, method, properties, body)
    # 是否消费队列，可以添加判断条件
    # if models_info == True:
    #     ch.basic_ack(delivery_tag=method.delivery_tag)


if __name__ == '__main__':
    try:
        rt_receive()
    except KeyboardInterrupt:
        print("Quit")
~~~

**返回数据**

~~~ python
<BlockingChannel impl=<Channel number=1 OPEN conn=<SelectConnection OPEN transport=<pika.adapters.utils.io_services_utils._AsyncPlaintextTransport object at 0x0000024125AB3588> params=<ConnectionParameters host=192.168.131.128 port=5672 virtual_host=/ ssl=False>>>> <Basic.Deliver(['consumer_tag=ctag1.b5cd70883d414facb51a748df4a8824c', 'delivery_tag=21', 'exchange=rt_default_x', 'redelivered=False', 'routing_key=rt_autoidc_queue'])> <BasicProperties(['delivery_mode=2'])> b'{"id": "100000", "amount": 0, "name": "tony", "createtime": "2021-04-27 21:59:08.161943"}'
<BlockingChannel impl=<Channel number=1 OPEN conn=<SelectConnection OPEN transport=<pika.adapters.utils.io_services_utils._AsyncPlaintextTransport object at 0x0000024125AB3588> params=<ConnectionParameters host=192.168.131.128 port=5672 virtual_host=/ ssl=False>>>> <Basic.Deliver(['consumer_tag=ctag1.b5cd70883d414facb51a748df4a8824c', 'delivery_tag=22', 'exchange=rt_default_x', 'redelivered=False', 'routing_key=rt_autoidc_queue'])> <BasicProperties(['delivery_mode=2'])> b'{"id": "100001", "amount": 100, "name": "tony", "createtime": "2021-04-27 21:59:08.199919"}'
<BlockingChannel impl=<Channel number=1 OPEN conn=<SelectConnection OPEN transport=<pika.adapters.utils.io_services_utils._AsyncPlaintextTransport object at 0x0000024125AB3588> params=<ConnectionParameters host=192.168.131.128 port=5672 virtual_host=/ ssl=False>>>> <Basic.Deliver(['consumer_tag=ctag1.b5cd70883d414facb51a748df4a8824c', 'delivery_tag=23', 'exchange=rt_default_x', 'redelivered=False', 'routing_key=rt_autoidc_queue'])> <BasicProperties(['delivery_mode=2'])> b'{"id": "100002", "amount": 200, "name": "tony", "createtime": "2021-04-27 21:59:08.211914"}'
~~~



### 模式三：topicd

这种模式和第二种模式差不多，exchange 也是通过 路由键 routing_key 来转发消息到指定的 queue 。 不同点是 **routing_key 使用正则表达式支持模糊匹配**，但匹配规则又与常规的正则表达式不同，比如“#”是匹配全部，“*”是匹配一个词。

举例：routing_key =“#orderid#”，意思是将消息转发至所有 routing_key 包含 “orderid” 字符的队列中。代码和模式二 类似，就不贴出来了。



# 死信消息

死信队列用于处理无法被正常消费的消息，即死信消息。当一条消息初次消费失败，消息队列RocketMQ版会自动进行消息重试，达到最大重试次数后，若消费依然失败，则表明消费者在正常情况下无法正确地消费该消息。此时，消息队列RocketMQ版不会立刻将消息丢弃，而是将其发送到该消费者对应的特殊队列中。

在消息队列RocketMQ版中，这种正常情况下无法被消费的消息称为**死信消息**（Dead-Letter Message），存储死信消息的特殊队列称为**死信队列**（Dead-Letter Queue）。



# 高可用集群

**原理上有三种实现方式**

1. 要么消息共享（共用同一块磁盘）
2. 要么消息同步（镜像集群）
3. 要么元数据共享（普通集群）



## 普通集群

就是在多个联通的服务器上安装不同的RabbitMQ的服务，这些服务器上的RabbitMQ服务组成一个个节点，通过RabbitMQ内部提供的命令或者配置来构建集群，形成了RabbitMQ的普通集群模式

![image9](D:\Note\python\数据库交互\图片\image9.png)

- 当用户向服务注册一个队列，该队列会随机保存到某一个服务节点上，然后将对应的元数据同步到各个不同的服务节点上，每个RabbitMQ都保存有相同的元数据
- 用户只需要链接到任意一个服务节点中，就可以监听消费到对应队列上的消息数据
- 但是RabbitMQ的实际数据却不是保存在每个RabbitMQ的服务节点中，这就意味着用户可能联系的是节点C，但是C上并没有对应的实际数据，也就是说节点C并不能提供消息供用户来消费，节点C会根据元数据，向服务节点B（该服务节点上有实际数据可供消费）请求实际数据，然后提供给用户进行消费

缺点：

1. 请求可能会在RabbitMQ内部服务节点之间进行频繁的进行数据交互，这样的交互比较耗费资源
2. 当其中一个RabbitMQ的服务节点宕机了，那么该节点上的实际数据就会丢失，用户再次请求时，就会请求不到数据，系统的功能就会出现异常



## 镜像集群

为了解决上面普通模式的两个显著的缺点，RabbitMQ官方提供另外一种集群模式：镜像集群模式

![image10](D:\Note\python\数据库交互\图片\image10.png)

镜像集群模式和普通集群模式大体是一样的，不一样的是：

- 生产者向任一服务节点注册队列，该队列相关信息会同步到其他节点上
- 任一消费者向任一节点请求消费，可以直接获取到消费的消息，因为每个节点上都有相同的实际数据
- 任一节点宕机，不影响消息在其他节点上进行消费

缺点：

1. 性能开销非常大，因为要同步消息到对应的节点，这个会造成网络之间的数据量的频繁交互，对于网络带宽的消耗和压力都是比较重的
2. 没有扩展可言，rabbitMQ是集群，不是分布式的，所以当某个Queue负载过重，我们并不能通过新增节点来缓解压力，因为所以节点上的数据都是相同的，这样就没办法进行扩展了



# 如何把MQ嵌入项目

## 透过模型看 MQ 的应用场景

目前，MQ 的应用场景非常多，大家能倒背如流的是：系统解耦、异步通信和流量削峰。除此之外，还有延迟通知、最终一致性保证、顺序消息、流式处理等等。

那到底是先有消息模型，还是先有应用场景呢？答案肯定是：先有应用场景（也就是先有问题），再有消息模型，因为消息模型只是解决方案的抽象而已。

MQ 经过 30 多年的发展，能从最原始的队列模型发展到今天百花齐放的各种消息中间件（平台级的解决方案），我觉得万变不离其宗，还是得益于：消息模型的适配性很广。

我们试着重新理解下消息队列的模型。它其实解决的是：生产者和消费者的通信问题。那它对比 RPC 有什么联系和区别呢？

![v2-f9793a436f5983c7bac2c9cc7d7abfdb_720w](D:\Note\python\数据库交互\图片\v2-f9793a436f5983c7bac2c9cc7d7abfdb_720w.jpg)

通过对比，能很明显地看出两点差异：

1. 引入 MQ 后，由之前的一次 RPC 变成了现在的两次 RPC，而且生产者只跟队列耦合，它根本无需知道消费者的存在。
2. 多了一个中间节点「队列」进行消息转储，相当于将同步变成了异步。

再返过来思考 MQ 的所有应用场景，就不难理解 MQ 为什么适用了？因为这些应用场景无外乎都利用了上面两个特性。

举一个实际例子，比如说电商业务中最常见的「订单支付」场景：在订单支付成功后，需要更新订单状态、更新用户积分、通知商家有新订单、更新推荐系统中的用户画像等等。

![v2-b0b81fcf533970cd71d23a85ec266e5b_720w](D:\Note\python\数据库交互\图片\v2-b0b81fcf533970cd71d23a85ec266e5b_720w.jpg)

引入 MQ 后，订单支付现在只需要关注它最重要的流程：更新订单状态即可。其他不重要的事情全部交给 MQ 来通知。这便是 MQ 解决的最核心的问题：系统解耦。

改造前订单系统依赖 3 个外部系统，改造后仅仅依赖 MQ，而且后续业务再扩展（比如：营销系统打算针对支付用户奖励优惠券），也不涉及订单系统的修改，从而保证了核心流程的稳定性，降低了维护成本。

这个改造还带来了另外一个好处：因为 MQ 的引入，更新用户积分、通知商家、更新用户画像这些步骤全部变成了异步执行，能减少订单支付的整体耗时，提升订单系统的吞吐量。这便是 MQ 的另一个典型应用场景：异步通信。

除此以外，由于队列能转储消息，对于超出系统承载能力的场景，可以用 MQ 作为 “漏斗” 进行限流保护，即所谓的流量削峰。我们还可以利用队列本身的顺序性，来满足消息必须按顺序投递的场景；利用队列 + 定时任务来实现消息的延时消费 ……

MQ 其他的应用场景基本类似，都能回归到消息模型的特性上，找到它适用的原因，这里就不一一分析了。总之，就是建议大家多从复杂多变的实践场景再回归到理论层面进行思考和抽象，这样能吃得更透。



## 写一个适用于生产环境的 MQ

### 1、先把握这个问题的关键点

假如我们还是只考虑最基础的功能：发消息、存消息、消费消息（支持发布-订阅模式）。

那在生产环境中，这些基础功能将面临哪些挑战呢？我们能很快想到下面这些：

1. **高并发场景下，如何保证收发消息的性能？**
2. **如何保证消息服务的高可用和高可靠？**
3. **如何保证服务是可以水平任意扩展的？**
4. **如何保证消息存储也是水平可扩展的？**
5. **各种元数据（比如集群中的各个节点、主题、消费关系等）如何管理，需不需要考虑数据的一致性？**

可见，高并发场景下的三高问题在你设计一个 MQ 时都会遇到，「如何满足高性能、高可靠等非功能性需求」才是这个问题的关键所在。



### 2、整体设计思路

先来看下整体架构，会涉及三类角色：

![v2-a70067dabc94c9f2e42348e1cc728c3f_720w](D:\Note\python\数据库交互\图片\v2-a70067dabc94c9f2e42348e1cc728c3f_720w.jpg)

基于上图，我们可以很快明确出 3 类角色的作用，分别如下：

1. Broker（服务端）：MQ 中最核心的部分，是 MQ 的服务端，核心逻辑几乎全在这里，它为生产者和消费者提供 RPC 接口，负责消息的存储、备份和删除，以及消费关系的维护等。
2. Producer（生产者）：MQ 的客户端之一，调用 Broker 提供的 RPC 接口发送消息。
3. Consumer（消费者）：MQ 的另外一个客户端，调用 Broker 提供的 RPC 接口接收消息，同时完成消费确认。



### 3、技术难点

#### 难点1：RPC 通信

解决的是 Broker 与 Producer 以及 Consumer 之间的通信问题。如果不重复造轮子，直接利用成熟的 RPC 框架实现即可，这样不需要考虑服务注册与发现、负载均衡、通信协议、序列化方式等一系列问题了。

当然，你也可以基于 Netty 来做底层通信，用 Zookeeper、Euraka 等来做注册中心，然后自定义一套新的通信协议（类似 Kafka），也可以基于 AMQP 这种标准化的 MQ 协议来做实现（类似 RabbitMQ）。对比直接用 RPC 框架，这种方案的定制化能力和优化空间更大。



#### 难点2：高可用设计

高可用主要涉及两方面：Broker 服务的高可用、存储方案的高可用。可以拆开讨论。

Broker 服务的高可用，只需要保证 Broker 可水平扩展进行集群部署即可，进一步通过服务自动注册与发现、负载均衡、超时重试机制、发送和消费消息时的 ack 机制来保证。

存储方案的高可用有两个思路：

1. 参考 Kafka 的分区 + 多副本模式，但是需要考虑分布式场景下数据复制和一致性方案（类似 Zab、Raft等协议），并实现自动故障转移；
2. 还可以用主流的 DB、分布式文件系统、带持久化能力的 KV 系统，它们都有自己的高可用方案。



#### 难点3：存储设计

消息的存储方案是 MQ 的核心部分，可靠性保证已经在高可用设计中谈过了，可靠性要求不高的话直接用内存或者分布式缓存也可以。这里重点说一下存储的高性能如何保证？这个问题的决定因素在于存储结构的设计。

目前主流的方案是：追加写日志文件（数据部分） + 索引文件的方式（很多主流的开源 MQ 都是这种方式），索引设计上可以考虑稠密索引或者稀疏索引，查找消息可以利用跳转表、二份查找等，还可以通过操作系统的页缓存、零拷贝等技术来提升磁盘文件的读写性能。

如果不追求很高的性能，也可以考虑现成的分布式文件系统、KV 存储或者数据库方案。



#### 难点4：消费关系管理

为了支持发布-订阅的广播模式，Broker 需要知道每个主题都有哪些 Consumer 订阅了，基于这个关系进行消息投递。由于 Broker 是集群部署的，所以消费关系通常维护在公共存储上，可以基于 Zookeeper、Apollo 等配置中心来管理以及进行变更通知。



#### 难点5：高性能设计

存储的高性能前面已经谈过了，当然还可以从其他方面进一步优化性能。比如 Reactor 网络 IO 模型、业务线程池的设计、生产端的批量发送、Broker 端的异步刷盘、消费端的批量拉取等等。