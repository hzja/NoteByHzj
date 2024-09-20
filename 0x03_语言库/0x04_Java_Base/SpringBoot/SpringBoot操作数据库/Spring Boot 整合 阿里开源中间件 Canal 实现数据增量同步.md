Java SpringBoot Canal**<br />canal**译意为水道/管道/沟渠，主要用途是基于 **MySQL** 数据库**增量日志**解析，提供增量数据订阅和消费。<br />从这句话理解到了什么？<br />基于MySQL，并且通过MySQL日志进行的增量解析，这也就意味着对原有的业务代码完全是无侵入性的。<br />工作原理：解析MySQL的binlog日志，提供增量数据。<br />基于日志增量订阅和消费的业务包括

- 数据库镜像
- 数据库实时备份
- 索引构建和实时维护(拆分异构索引、倒排索引等)
- 业务 cache 刷新
- 带业务逻辑的增量数据处理

当前的 canal 支持源端 MySQL 版本包括 5.1.x , 5.5.x , 5.6.x , 5.7.x , 8.0.x。<br />官方文档：[https://github.com/alibaba/canal](https://github.com/alibaba/canal)
<a name="EPaam"></a>
## Canal数据如何传输？
先来一张官方图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640569484358-a30734fe-18ff-4122-878d-bd07f1a0bbb9.webp#clientId=udf69b0d0-8884-4&from=paste&id=uca589534&originHeight=569&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50920614-2440-42cb-b365-8d91989fbd1&title=)<br />Canal分为服务端和客户端，这也是阿里常用的套路，比如前面讲到的注册中心**Nacos**：

- **服务端**：负责解析MySQL的binlog日志，传递增量数据给客户端或者消息中间件
- **客户端**：负责解析服务端传过来的数据，然后定制自己的业务处理。

目前为止支持的消息中间件很全面了，比如**Kafka**、**RocketMQ**，**RabbitMQ**。
<a name="tzlv4"></a>
## 数据同步还有其他中间件吗？
有，当然有，还有一些开源的中间件也是相当不错的，比如**Bifrost**。<br />常见的几款中间件的区别如下：

|  | Canal | Debezium | DataX | Databus | Flinkx | Bifrost |
| --- | --- | --- | --- | --- | --- | --- |
| 实时同步 | 支持 | 支持 | 不支持 | 支持 | 支持 | 支持 |
| 增量同步 | 支持 | 支持 | 不支持 | 支持 | 支持 | 支持 |
| 写业务逻辑 | 自己写保存变更数据的代码 | 自己写保存变更数据的代码 | 不用写 | 自己写保存变更数据的代码 | 自己写保存变更数据的代码 | 不用写 |
| 支持MySQL | 支持 | 支持 | 支持 | 支持 | 支持 | 支持 |
| 活跃度 | 高 | 高 | 高 | 不高 | 一般 | 可以 |

<a name="zcw7q"></a>
## Canal服务端安装
服务端需要下载压缩包，下载地址：[https://github.com/alibaba/canal/releases](https://github.com/alibaba/canal/releases)<br />目前最新的是**v1.1.5**，点击下载：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640592600303-d0d60ce0-118a-49de-aaef-46d62f1e2057.png#clientId=u1ce7cfc9-d5b3-4&from=paste&height=273&id=u4f8ab8ad&originHeight=818&originWidth=2280&originalType=binary&ratio=1&rotation=0&showTitle=false&size=99244&status=done&style=none&taskId=u76bcb407-ab1b-45d4-82e5-3ee745ebbb2&title=&width=760)<br />下载完成解压，目录如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640592751894-b2a7e5e7-61f8-431e-a255-34b6c95dc405.png#clientId=u1ce7cfc9-d5b3-4&from=paste&height=201&id=u147c491b&originHeight=602&originWidth=1768&originalType=binary&ratio=1&rotation=0&showTitle=false&size=105739&status=done&style=shadow&taskId=ue02333b3-da09-4ea5-902f-54ffc370e26&title=&width=589.3333333333334)<br />本文使用**Canal+RabbitMQ**进行数据的同步，因此下面步骤完全按照这个base进行。
<a name="BMNkv"></a>
### 1、打开MySQL的binlog日志
修改MySQL的日志文件，my.cnf 配置如下：
```
[mysqld]
log-bin=mysql-bin # 开启 binlog
binlog-format=ROW # 选择 ROW 模式
server_id=1 # 配置 MySQL replaction 需要定义，不要和 canal 的 slaveId 重复
```
<a name="bv3B4"></a>
### 2、设置MySQL的配置
需要设置服务端配置文件中的MySQL配置，这样Canal才能知道需要监听哪个库、哪个表的日志文件。<br />一个 Server 可以配置多个实例监听 ，Canal 功能默认自带的有个 example 实例，这里用 example 实例 。如果增加实例，复制 example 文件夹内容到同级目录下，然后在 canal.properties 指定添加实例的名称。<br />**修改canal.deployer-1.1.5\conf\example\instance.properties配置文件**
```
# url
canal.instance.master.address=127.0.0.1:3306
# username/password
canal.instance.dbUsername=root
canal.instance.dbPassword=root
# 监听的数据库
canal.instance.defaultDatabaseName=test

# 监听的表，可以指定，多个用逗号分割，这里正则是监听所有
canal.instance.filter.regex=.*\\..*
```
<a name="V9zXU"></a>
### 3、设置RabbitMQ的配置
服务端默认的传输方式是**tcp**，需要在配置文件中设置**MQ**的相关信息。<br />这里需要修改两处配置文件，如下；
<a name="QV43y"></a>
#### 1、canal.deployer-1.1.5\conf\canal.properties
这个配置文件主要是设置MQ相关的配置，比如URL，用户名、密码...
```
# 传输方式：tcp, kafka, rocketMQ, rabbitMQ
canal.serverMode = rabbitMQ
##################################################
######### 		    RabbitMQ	     #############
##################################################
rabbitmq.host = 127.0.0.1
rabbitmq.virtual.host =/
# exchange
rabbitmq.exchange =canal.exchange
# 用户名、密码
rabbitmq.username =guest
rabbitmq.password =guest
## 是否持久化
rabbitmq.deliveryMode = 2
```
<a name="FSfIM"></a>
#### 2、canal.deployer-1.1.5\conf\example\instance.properties
这个文件设置MQ的路由KEY，这样才能路由到指定的队列中，如下：
```
canal.mq.topic=canal.routing.key
```
<a name="HrP1K"></a>
### 4、RabbitMQ新建exchange和Queue
在RabbitMQ中需要新建一个**canal.exchange**（必须和配置中的相同）的exchange和一个名称为 **canal.queue**（名称随意）的队列。<br />其中绑定的路由KEY为：**canal.routing.key**（必须和配置中的相同），如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640569484589-3220b30f-2705-44ef-92a6-86dea973b7b2.webp#clientId=udf69b0d0-8884-4&from=paste&id=ue6d38fdc&originHeight=242&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u22947195-aea1-4238-9c9d-7cc3977b3b7&title=)
<a name="zp7zS"></a>
### 5、启动服务端
点击bin目录下的脚本，windows直接双击**startup.bat**，启动成功如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1640592799246-c2e1a27d-ac3a-445d-94c0-9e1a6a8aa991.png#clientId=u1ce7cfc9-d5b3-4&from=paste&height=587&id=u608e8940&originHeight=1760&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1467672&status=done&style=none&taskId=u419b33a3-a823-420f-87bb-3f5ae8c2750&title=&width=1107.6666666666667)
<a name="IHEYj"></a>
### 6、测试
在本地数据库**test**中的**oauth_client_details**插入一条数据，如下：
```sql
INSERT INTO `oauth_client_details` VALUES ('myjszl', 'res1', '$2a$10$F1tQdeb0SEMdtjlO8X/0wO6Gqybu6vPC/Xg8OmP9/TL1i4beXdK9W', 'all', 'password,refresh_token,authorization_code,client_credentials,implicit', 'http://www.baidu.com', NULL, 1000, 1000, NULL, 'false');
```
此时查看MQ中的**canal.queue**已经有了数据，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640569485019-4a0aaa8c-28c0-459c-9a1c-ff4255cef917.webp#clientId=udf69b0d0-8884-4&from=paste&id=u0457a209&originHeight=164&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7e2c0a84-62f1-40e6-9de4-4c34a2f6502&title=)<br />其实就是一串JSON数据，这个JSON如下：
```json
{
  "data": [{
    "client_id": "myjszl",
    "resource_ids": "res1",
    "client_secret": "$2a$10$F1tQdeb0SEMdtjlO8X/0wO6Gqybu6vPC/Xg8OmP9/TL1i4beXdK9W",
    "scope": "all",
    "authorized_grant_types": "password,refresh_token,authorization_code,client_credentials,implicit",
    "web_server_redirect_uri": "http://www.baidu.com",
    "authorities": null,
    "access_token_validity": "1000",
    "refresh_token_validity": "1000",
    "additional_information": null,
    "autoapprove": "false"
  }],
  "database": "test",
  "es": 1640337532000,
  "id": 7,
  "isDdl": false,
  "mysqlType": {
    "client_id": "varchar(48)",
    "resource_ids": "varchar(256)",
    "client_secret": "varchar(256)",
    "scope": "varchar(256)",
    "authorized_grant_types": "varchar(256)",
    "web_server_redirect_uri": "varchar(256)",
    "authorities": "varchar(256)",
    "access_token_validity": "int(11)",
    "refresh_token_validity": "int(11)",
    "additional_information": "varchar(4096)",
    "autoapprove": "varchar(256)"
  },
  "old": null,
  "pkNames": ["client_id"],
  "sql": "",
  "sqlType": {
    "client_id": 12,
    "resource_ids": 12,
    "client_secret": 12,
    "scope": 12,
    "authorized_grant_types": 12,
    "web_server_redirect_uri": 12,
    "authorities": 12,
    "access_token_validity": 4,
    "refresh_token_validity": 4,
    "additional_information": 12,
    "autoapprove": 12
  },
  "table": "oauth_client_details",
  "ts": 1640337532520,
  "type": "INSERT"
}
```
每个字段的意思已经很清楚了，有表名称、方法、参数、参数类型、参数值.....<br />**客户端要做的就是监听MQ获取JSON数据，然后将其解析出来，处理自己的业务逻辑。**
<a name="beW5T"></a>
## Canal客户端搭建
客户端很简单实现，要做的就是消费Canal服务端传递过来的消息，监听**canal.queue**这个队列。
<a name="RzmeS"></a>
### 1、创建消息实体类
MQ传递过来的是JSON数据，当然要创建个实体类接收数据，如下：
```java
/**
 * Canal消息接收实体类
 */
@NoArgsConstructor
@Data
public class CanalMessage<T> {
    @JsonProperty("type")
    private String type;

    @JsonProperty("table")
    private String table;

    @JsonProperty("data")
    private List<T> data;

    @JsonProperty("database")
    private String database;

    @JsonProperty("es")
    private Long es;

    @JsonProperty("id")
    private Integer id;

    @JsonProperty("isDdl")
    private Boolean isDdl;

    @JsonProperty("old")
    private List<T> old;

    @JsonProperty("pkNames")
    private List<String> pkNames;

    @JsonProperty("sql")
    private String sql;

    @JsonProperty("ts")
    private Long ts;
}
```
<a name="RQsK9"></a>
### 2、MQ消息监听业务
接下来就是监听队列，一旦有Canal服务端有数据推送能够及时的消费。<br />代码很简单，只是给出个接收的案例，具体的业务逻辑可以根据业务实现，如下：
```java
import cn.hutool.json.JSONUtil;
import cn.myjszl.middle.ware.canal.mq.rabbit.model.CanalMessage;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.amqp.rabbit.annotation.Exchange;
import org.springframework.amqp.rabbit.annotation.Queue;
import org.springframework.amqp.rabbit.annotation.QueueBinding;
import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

/**
 * 监听MQ获取Canal增量的数据消息
 */
@Component
@Slf4j
@RequiredArgsConstructor
public class CanalRabbitMQListener {

    @RabbitListener(bindings = {
            @QueueBinding(
                    value = @Queue(value = "canal.queue", durable = "true"),
                    exchange = @Exchange(value = "canal.exchange"),
                    key = "canal.routing.key"
            )
    })
    public void handleDataChange(String message) {
        //将message转换为CanalMessage
        CanalMessage canalMessage = JSONUtil.toBean(message, CanalMessage.class);
        String tableName = canalMessage.getTable();
        log.info("Canal 监听 {} 发生变化；明细：{}", tableName, message);
        //TODO 业务逻辑自己完善...............
    }
}
```
<a name="t7QjL"></a>
### 3、测试
下面向表中插入数据，看下接收的消息是什么样的，SQL如下：
```sql
INSERT INTO `oauth_client_details`
VALUES
 ( 'myjszl', 'res1', '$2a$10$F1tQdeb0SEMdtjlO8X/0wO6Gqybu6vPC/Xg8OmP9/TL1i4beXdK9W', 'all', 'password,refresh_token,authorization_code,client_credentials,implicit', 'http://www.baidu.com', NULL, 1000, 1000, NULL, 'false' );
```
客户端转换后的消息如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1640569485009-99b92751-f87d-4eb0-95d8-db55a3003213.webp#clientId=udf69b0d0-8884-4&from=paste&id=ufa948a0f&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u741987b8-0a94-471e-9bd2-188a8d539ab&title=)<br />上图可以看出所有的数据都已经成功接收到，只需要根据数据完善自己的业务逻辑即可。
