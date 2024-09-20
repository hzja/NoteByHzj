Java SpringBoot<br />![2021-06-03-22-40-39-571149.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622731474953-649343b9-0ac6-4f0c-87a8-59e2fe92b1e8.png#clientId=u339e8916-1fee-4&from=ui&id=ue7e712bb&originHeight=295&originWidth=724&originalType=binary&ratio=1&size=50720&status=done&style=shadow&taskId=ud80b35ff-fcac-4e39-bf2e-16626dfa09b)<br />数据库中的数据一直在变化，有时候希望能监听数据库数据的变化并根据变化做出一些反应，比如更新对应变化数据的缓存、增量同步到其它数据源、对数据进行检测和审计等等。而这种技术就叫变更数据捕获（Change Data Capture）。对于这种技术国内比较知名的框架Canal，非常好用！但是Canal有一个局限性就是只能用于MySQL的变更数据捕获。这里介绍另一种更加强大的分布式CDC框架Debezium。
<a name="Hagka"></a>
## Debezium
提起Debezium这个框架，相信大多数普通开发者都比较陌生，但是提及它所属的公司大家一定不会陌生。<br />![2021-06-03-22-40-39-641921.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622731465890-50497946-f6e7-46f4-84d5-f130a557624d.png#clientId=u339e8916-1fee-4&from=ui&id=ude486ff0&originHeight=332&originWidth=1080&originalType=binary&ratio=1&size=29543&status=done&style=shadow&taskId=u7ed8b2cb-f812-43cf-931a-c32fe2f01c7)<br />没错就是开源界最成功的红帽公司。Debezium是为捕获数据更改的流式处理框架，开源免费。Debezium近乎实时地监控数据库行级别(row-level)的数据变更，并针对变更可以做出反应。而且只有已提交的变更才是可见的，所以不用担心事务问题或者更改被回滚的问题。Debezium为所有的数据库更改事件提供了一个统一的模型，所以不用担心每种数据库系统的复杂性。Debezium提供了对MongoDB、MySQL、PostgreSQL、SQL Server、Oracle、DB2等数据库的支持。<br />另外借助于Kafka Connector可以开发出一个基于事件流的变更捕获平台，具有高容错率和极强的扩展性。<br />![2021-06-03-22-40-39-739426.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622731492278-476e404f-1b4c-4183-871f-a1b609f7a061.png#clientId=u339e8916-1fee-4&from=ui&id=u15b5f742&originHeight=245&originWidth=1080&originalType=binary&ratio=1&size=66681&status=done&style=shadow&taskId=u86b633ab-0d30-4c9d-b0a3-1a0536e6ada)<br />如图所示，部署了用于 MySQL 和 PostgresSQL 的 Debezium Kafka连接器以捕获对这两种类型数据库的更改事件，然后将这些更改通过下游的Kafka Connector将记录传输到其他系统或者数据库（例如 Elasticsearch、数据仓库、分析系统）或缓存。<br />另一种玩法就是将Debezium内置到应用程序中，来做一个类似消息总线的设施，将数据变更事件传递给订阅的下游系统中。<br />![2021-06-03-22-44-58-945697.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622731583539-d1237742-65a9-441d-be99-933bc3d6b663.png#clientId=u339e8916-1fee-4&from=ui&id=u1c89f9b1&originHeight=310&originWidth=1080&originalType=binary&ratio=1&size=62841&status=done&style=shadow&taskId=u7d27e68b-d39e-4d5a-9796-52c6254491b)<br />Debezium内置服务器架构<br />Debezium对数据的完整性和可用性也是做了不少的工作。Debezium用持久化的、有副本备份的日志来记录数据库数据变化的历史，因此，应用程序可以随时停止再重启，而不会错过它停止运行时发生的事件，保证了所有的事件都能被正确地、完全地处理掉。
<a name="nYwcC"></a>
## Spring Boot集成Debezium
理论介绍并不能直观感受到Debezium的能力，所以接下来将使用嵌入式Debezium引擎来演示一下。<br />![2021-06-03-22-44-59-022746.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633661125416-a3bb812c-6fe2-4151-943f-15d0a837651b.png#clientId=ue0996f8d-6901-4&from=ui&id=u79cd0ac1&originHeight=429&originWidth=1080&originalType=binary&ratio=1&size=1392711&status=done&style=shadow&taskId=u16823f49-4360-4748-9cc2-ec1e4f97bdc)<br />流程图<br />如上图所示，当变更MySQL数据库中的某行数据时，通过Debezium实时监听到binlog日志的变化触发捕获变更事件，然后获取到变更事件模型，并做出响应（消费）。接下来来搭建环境。
<a name="juOpY"></a>
### MySQL开启binlog日志
为了方便这里使用MySQL的Docker容器，对应的脚本为：
```bash
# 运行mysql容器 
docker run --name mysql-service -v d:/mysql/data:/var/lib/mysql -p 3306:3306 -e TZ=Asia/Shanghai -e MYSQL_ROOT_PASSWORD=123456 -d mysql:5.7 --character-set-server=utf8mb4 --collation-server=utf8mb4_unicode_ci --default-time_zone="+8:00"
# 设置binlog位置
docker exec mysql-service bash -c "echo 'log-bin=/var/lib/mysql/mysql-bin' >> /etc/mysql/mysql.conf.d/mysqld.cnf"
# 配置 mysql的server-id
docker exec mysql-service bash -c "echo 'server-id=123454' >> /etc/mysql/mysql.conf.d/mysqld.cnf"
```
上面的脚本运行了一个用户名为root、密码为123456并且将数据挂载到本地路径d:/mysql/data的MySQL容器，同时开启了binlog日志，并设置server-id为123454，这些信息后面配置会用。<br />请注意如果不使用`root`用户的话，需要保证用户具有`SELECT`, `RELOAD`, `SHOW DATABASES`, `REPLICATION SLAVE`, `REPLICATION CLIENT`五种权限。
<a name="mqPCC"></a>
### Spring Boot集成嵌入式Debezium
<a name="rYxKT"></a>
### Debezium依赖
Spring Boot的应用中加入下列依赖：
```xml
<dependency>
  <groupId>io.debezium</groupId>
  <artifactId>debezium-api</artifactId>
  <version>${debezium.version}</version>
</dependency>
<dependency>
  <groupId>io.debezium</groupId>
  <artifactId>debezium-embedded</artifactId>
  <version>${debezium.version}</version>
</dependency>
<dependency>
  <groupId>io.debezium</groupId>
  <artifactId>debezium-connector-mysql</artifactId>
  <version>${debezium.version}</version>
</dependency>
```
目前最新的版本号为1.5.2.Final。
<a name="RLixu"></a>
### 声明配置
然后声明需要的配置：
```java
/**
     * Debezium 配置.
     *
     * @return configuration
     */
@Bean
io.debezium.config.Configuration debeziumConfig() {
    return io.debezium.config.Configuration.create()
        //            连接器的Java类名称
        .with("connector.class", MySqlConnector.class.getName())
        //            偏移量持久化，用来容错 默认值
        .with("offset.storage", "org.apache.kafka.connect.storage.FileOffsetBackingStore")
        //                偏移量持久化文件路径 默认/tmp/offsets.dat  如果路径配置不正确可能导致无法存储偏移量 可能会导致重复消费变更
        //                如果连接器重新启动，它将使用最后记录的偏移量来知道它应该恢复读取源信息中的哪个位置。
        .with("offset.storage.file.filename", "C:/Users/n1/IdeaProjects/spring-boot-debezium/tmp/offsets.dat")
        //                捕获偏移量的周期
        .with("offset.flush.interval.ms", "6000")
        //               连接器的唯一名称
        .with("name", "mysql-connector")
        //                数据库的hostname
        .with("database.hostname", "localhost")
        //                端口
        .with("database.port", "3306")
        //                用户名
        .with("database.user", "root")
        //                密码
        .with("database.password", "123456")
        //                 包含的数据库列表
        .with("database.include.list", "etl")
        //                是否包含数据库表结构层面的变更，建议使用默认值true
        .with("include.schema.changes", "false")
        //                mysql.cnf 配置的 server-id
        .with("database.server.id", "123454")
        //                 MySQL 服务器或集群的逻辑名称
        .with("database.server.name", "customer-mysql-db-server")
        //                历史变更记录
        .with("database.history", "io.debezium.relational.history.FileDatabaseHistory")
        //                历史变更记录存储位置 
        .with("database.history.file.filename", "C:/Users/n1/IdeaProjects/spring-boot-debezium/tmp/dbhistory.dat")
        .build();
}
```
配置分为两部分：

- 一部分是Debezium Engine的配置属性，参见[Debezium Engine配置](https://debezium.io/documentation/reference/1.5/development/engine.html#engine-properties)。
- 一部分是Mysql Connector的配置属性，参见[MySQL Connector配置](https://debezium.io/documentation/reference/1.5/connectors/mysql.html#mysql-connector-properties)。
<a name="jSxzF"></a>
### 实例化Debezium Engine
应用程序需要为运行的MySQL Connector启动一个Debezium引擎，这个引擎会以异步线程的形式运行，它包装了整个MySQL Connector连接器的生命周期。声明一个引擎需要以下几步：

- 声明收到数据变更捕获信息的格式，提供了JSON、Avro、Protobuf、Connect、CloudEvents等格式。
- 加载上面定义的配置。
- 声明消费数据更改事件的函数方法。

声明的伪代码：
```java
DebeziumEngine<RecordChangeEvent<SourceRecord>> debeziumEngine = DebeziumEngine.create(ChangeEventFormat.of(Connect.class))
        .using(configuration.asProperties())
        .notifying(this::handlePayload)
        .build();
```
handlePayload方法为：
```java
private void handlePayload(List<RecordChangeEvent<SourceRecord>> recordChangeEvents, DebeziumEngine.RecordCommitter<RecordChangeEvent<SourceRecord>> recordCommitter) {
    recordChangeEvents.forEach(r -> {
        SourceRecord sourceRecord = r.record();
        Struct sourceRecordChangeValue = (Struct) sourceRecord.value();

        if (sourceRecordChangeValue != null) {
            // 判断操作的类型 过滤掉读 只处理增删改   这个其实可以在配置中设置
            Envelope.Operation operation = Envelope.Operation.forCode((String) sourceRecordChangeValue.get(OPERATION));

            if (operation != Envelope.Operation.READ) {
                String record = operation == Envelope.Operation.DELETE ? BEFORE : AFTER;
                // 获取增删改对应的结构体数据
                Struct struct = (Struct) sourceRecordChangeValue.get(record);
                // 将变更的行封装为Map
                Map<String, Object> payload = struct.schema().fields().stream()
                        .map(Field::name)
                        .filter(fieldName -> struct.get(fieldName) != null)
                        .map(fieldName -> Pair.of(fieldName, struct.get(fieldName)))
                        .collect(toMap(Pair::getKey, Pair::getValue));
                // 这里简单打印一下
                System.out.println("payload = " + payload);
            }
        }
    });
}
```
引擎的启动和关闭正好契合Spring Bean的生命周期：
```java
@Data
public class DebeziumServerBootstrap implements InitializingBean, SmartLifecycle {

    private final Executor executor = Executors.newSingleThreadExecutor();
    private DebeziumEngine<?> debeziumEngine;

    @Override
    public void start() {
        executor.execute(debeziumEngine);
    }

    @SneakyThrows
    @Override
    public void stop() {
        debeziumEngine.close();
    }

    @Override
    public boolean isRunning() {
        return false;
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        Assert.notNull(debeziumEngine, "debeziumEngine must not be null");
    }
}
```
<a name="Od4VV"></a>
## 启动
启动该Spring Boot项目，可以采用各种手段往数据库增删改数据，观察会有类似下面的打印：
```java
payload = {user_id=1123213, username=fcant.cn, age=11 , gender=0, enabled=1}
```
说明Debezium监听到了数据库的变更。
