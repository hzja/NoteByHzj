JavaSpringBootCanalRabbitMQ

- 需求
- 步骤
- 环境搭建
- 整合SpringBoot Canal实现客户端
- Canal整合RabbitMQ
- SpringBoot整合RabbitMQ

---

<a name="rOBxw"></a>
## **需求**
在SpringBoot中采用一种与业务代码解耦合的方式，来实现数据的变更记录，记录的内容是新数据，如果是更新操作还得有旧数据内容。<br />经过调研发现，使用Canal来监听MySQL的binlog变化可以实现这个需求，可是在监听到变化后需要马上保存变更记录，除非再做一些逻辑处理，于是又结合了RabbitMQ来处理保存变更记录的操作。
<a name="lNOJ4"></a>
## **步骤**

- 启动MySQL环境，并开启binlog
- 启动Canal环境，为其创建一个MySQL账号，然后以Slave的形式连接MySQL
- Canal服务模式设为TCP，用Java编写客户端代码，监听MySQL的binlog修改
- Canal服务模式设为RabbitMQ，启动RabbitMQ环境，配置Canal和RabbitMQ的连接，用消息队列去接收binlog修改事件
<a name="nljJA"></a>
## **环境搭建**
环境搭建基于docker-compose：
```yaml
version: "3"  
services:  
    mysql:  
        network_mode: mynetwork  
        container_name: mymysql  
        ports:  
            - 3306:3306  
        restart: always  
        volumes:  
            - /etc/localtime:/etc/localtime  
            - /home/mycontainers/mymysql/data:/data  
            - /home/mycontainers/mymysql/mysql:/var/lib/mysql  
            - /home/mycontainers/mymysql/conf:/etc/mysql  
        environment:  
            - MYSQL_ROOT_PASSWORD=root  
        command:   
            --character-set-server=utf8mb4  
            --collation-server=utf8mb4_unicode_ci  
            --log-bin=/var/lib/mysql/mysql-bin  
            --server-id=1  
            --binlog-format=ROW  
            --expire_logs_days=7  
            --max_binlog_size=500M  
        image: mysql:5.7.20  
    rabbitmq:     
        container_name: myrabbit  
        ports:  
            - 15672:15672  
            - 5672:5672  
        restart: always  
        volumes:  
            - /etc/localtime:/etc/localtime  
            - /home/mycontainers/myrabbit/rabbitmq:/var/lib/rabbitmq  
        network_mode: mynetwork  
        environment:  
            - RABBITMQ_DEFAULT_USER=admin  
            - RABBITMQ_DEFAULT_PASS=123456  
        image: rabbitmq:3.8-management  
    canal-server:  
        container_name: canal-server  
        restart: always  
        ports:  
            - 11110:11110  
            - 11111:11111  
            - 11112:11112  
        volumes:  
            - /home/mycontainers/canal-server/conf/canal.properties:/home/admin/canal-server/conf/canal.properties  
            - /home/mycontainers/canal-server/conf/instance.properties:/home/admin/canal-server/conf/example/instance.properties  
            - /home/mycontainers/canal-server/logs:/home/admin/canal-server/logs  
        network_mode: mynetwork  
        depends_on:  
            - mysql  
            - rabbitmq  
            # - canal-admin  
        image: canal/canal-server:v1.1.5
```
需要修改下Canal环境的配置文件：canal.properties和instance.properties，映射Canal中的以下两个路径：

- /home/admin/canal-server/conf/canal.properties

配置文件中，canal.destinations意思是server上部署的instance列表，

- /home/admin/canal-server/conf/example/instance.properties

这里的/example是指instance即实例名，要和上面canal.properties内instance配置对应，canal会为实例创建对应的文件夹，一个Client对应一个实例<br />以下是需要准备的两个配置文件具体内容：
<a name="moBv3"></a>
### canal.properties
```
################################################  
########     common argument   ############  
################################################  
# tcp bind ip  
canal.ip =  
# register ip to zookeeper  
canal.register.ip =  
canal.port = 11111  
canal.metrics.pull.port = 11112  
# canal instance user/passwd  
# canal.user = canal  
# canal.passwd = E3619321C1A937C46A0D8BD1DAC39F93B27D4458  
  
# canal admin config  
# canal.admin.manager = canal-admin:8089  
  
# canal.admin.port = 11110  
# canal.admin.user = admin  
# canal.admin.passwd = 6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9  
  
# admin auto register 自动注册  
# canal.admin.register.auto = true  
# 集群名，单机则不写  
# canal.admin.register.cluster =  
# Canal Server 名字  
# canal.admin.register.name = canal-admin  
  
canal.zkServers =  
# flush data to zk  
canal.zookeeper.flush.period = 1000  
canal.withoutNetty = false  
# tcp, kafka, rocketMQ, rabbitMQ, pulsarMQ  
canal.serverMode = tcp  
# flush meta cursor/parse position to file  
canal.file.data.dir = ${canal.conf.dir}  
canal.file.flush.period = 1000  
# memory store RingBuffer size, should be Math.pow(2,n)  
canal.instance.memory.buffer.size = 16384  
# memory store RingBuffer used memory unit size , default 1kb  
canal.instance.memory.buffer.memunit = 1024   
# meory store gets mode used MEMSIZE or ITEMSIZE  
canal.instance.memory.batch.mode = MEMSIZE  
canal.instance.memory.rawEntry = true  
  
# detecing config  
canal.instance.detecting.enable = false  
#canal.instance.detecting.sql = insert into retl.xdual values(1,now()) on duplicate key update x=now()  
canal.instance.detecting.sql = select 1  
canal.instance.detecting.interval.time = 3  
canal.instance.detecting.retry.threshold = 3  
canal.instance.detecting.heartbeatHaEnable = false  
  
# support maximum transaction size, more than the size of the transaction will be cut into multiple transactions delivery  
canal.instance.transaction.size =  1024  
# mysql fallback connected to new master should fallback times  
canal.instance.fallbackIntervalInSeconds = 60  
  
# network config  
canal.instance.network.receiveBufferSize = 16384  
canal.instance.network.sendBufferSize = 16384  
canal.instance.network.soTimeout = 30  
  
# binlog filter config  
canal.instance.filter.druid.ddl = true  
canal.instance.filter.query.dcl = false  
canal.instance.filter.query.dml = false  
canal.instance.filter.query.ddl = false  
canal.instance.filter.table.error = false  
canal.instance.filter.rows = false  
canal.instance.filter.transaction.entry = false  
canal.instance.filter.dml.insert = false  
canal.instance.filter.dml.update = false  
canal.instance.filter.dml.delete = false  
  
# binlog format/image check  
canal.instance.binlog.format = ROW,STATEMENT,MIXED   
canal.instance.binlog.image = FULL,MINIMAL,NOBLOB  
  
# binlog ddl isolation  
canal.instance.get.ddl.isolation = false  
  
# parallel parser config  
canal.instance.parser.parallel = true  
# concurrent thread number, default 60% available processors, suggest not to exceed Runtime.getRuntime().availableProcessors()  
canal.instance.parser.parallelThreadSize = 16  
# disruptor ringbuffer size, must be power of 2  
canal.instance.parser.parallelBufferSize = 256  
  
# table meta tsdb info  
canal.instance.tsdb.enable = true  
canal.instance.tsdb.dir = ${canal.file.data.dir:../conf}/${canal.instance.destination:}  
canal.instance.tsdb.url = jdbc:h2:${canal.instance.tsdb.dir}/h2;CACHE_SIZE=1000;MODE=MYSQL;  
canal.instance.tsdb.dbUsername = canal  
canal.instance.tsdb.dbPassword = canal  
# dump snapshot interval, default 24 hour  
canal.instance.tsdb.snapshot.interval = 24  
# purge snapshot expire , default 360 hour(15 days)  
canal.instance.tsdb.snapshot.expire = 360  
  
################################################  
########     destinations    ############  
################################################  
canal.destinations = canal-exchange  
# conf root dir  
canal.conf.dir = ../conf  
# auto scan instance dir add/remove and start/stop instance  
canal.auto.scan = true  
canal.auto.scan.interval = 5  
# set this value to 'true' means that when binlog pos not found, skip to latest.  
# WARN: pls keep 'false' in production env, or if you know what you want.  
canal.auto.reset.latest.pos.mode = false  
  
canal.instance.tsdb.spring.xml = classpath:spring/tsdb/h2-tsdb.xml  
#canal.instance.tsdb.spring.xml = classpath:spring/tsdb/mysql-tsdb.xml  
  
canal.instance.global.mode = spring  
canal.instance.global.lazy = false  
canal.instance.global.manager.address = ${canal.admin.manager}  
#canal.instance.global.spring.xml = classpath:spring/memory-instance.xml  
canal.instance.global.spring.xml = classpath:spring/file-instance.xml  
#canal.instance.global.spring.xml = classpath:spring/default-instance.xml  
  
#################################################  
########         MQ Properties      ############  
#################################################  
# aliyun ak/sk , support rds/mq  
canal.aliyun.accessKey =  
canal.aliyun.secretKey =  
canal.aliyun.uid=  
  
canal.mq.flatMessage = true  
canal.mq.canalBatchSize = 50  
canal.mq.canalGetTimeout = 100  
# Set this value to "cloud", if you want open message trace feature in aliyun.  
canal.mq.accessChannel = local  
  
canal.mq.database.hash = true  
canal.mq.send.thread.size = 30  
canal.mq.build.thread.size = 8  
  
#################################################  
########         RabbitMQ       ############  
#################################################  
rabbitmq.host = myrabbit  
rabbitmq.virtual.host = /  
rabbitmq.exchange = canal-exchange  
rabbitmq.username = admin  
rabbitmq.password = RabbitMQ密码  
rabbitmq.deliveryMode =
```
此时`canal.serverMode = tcp`，即TCP直连，先开启这个服务，然后手写Java客户端代码去连接它，等下再改为RabbitMQ。<br />通过注释可以看到，canal支持的服务模式有：tcp, kafka, rocketMQ, rabbitMQ, pulsarMQ，即主流的消息队列都支持。
<a name="o6aZZ"></a>
### instance.properties
```
################################################  
# mysql serverId , v1.0.26+ will autoGen  
#canal.instance.mysql.slaveId=123  
  
# enable gtid use true/false  
canal.instance.gtidon=false  
  
# position info  
canal.instance.master.address=mymysql:3306  
canal.instance.master.journal.name=  
canal.instance.master.position=  
canal.instance.master.timestamp=  
canal.instance.master.gtid=  
  
# rds oss binlog  
canal.instance.rds.accesskey=  
canal.instance.rds.secretkey=  
canal.instance.rds.instanceId=  
  
# table meta tsdb info  
canal.instance.tsdb.enable=true  
#canal.instance.tsdb.url=jdbc:mysql://127.0.0.1:3306/canal_tsdb  
#canal.instance.tsdb.dbUsername=canal  
#canal.instance.tsdb.dbPassword=canal  
  
#canal.instance.standby.address =  
#canal.instance.standby.journal.name =  
#canal.instance.standby.position =  
#canal.instance.standby.timestamp =  
#canal.instance.standby.gtid=  
  
# username/password  
canal.instance.dbUsername=canal  
canal.instance.dbPassword=canal  
canal.instance.connectionCharset = UTF-8  
# enable druid Decrypt database password  
canal.instance.enableDruid=false  
#canal.instance.pwdPublicKey=MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBALK4BUxdDltRRE5/zXpVEVPUgunvscYFtEip3pmLlhrWpacX7y7GCMo2/JM6LeHmiiNdH1FWgGCpUfircSwlWKUCAwEAAQ==  
  
# table regex  
canal.instance.filter.regex=.*\..*  
# table black regex  
canal.instance.filter.black.regex=mysql\.slave_.*  
# table field filter(format: schema1.tableName1:field1/field2,schema2.tableName2:field1/field2)  
#canal.instance.filter.field=test1.t_product:id/subject/keywords,test2.t_company:id/name/contact/ch  
# table field black filter(format: schema1.tableName1:field1/field2,schema2.tableName2:field1/field2)  
#canal.instance.filter.black.field=test1.t_product:subject/product_image,test2.t_company:id/name/contact/ch  
  
# mq config  
canal.mq.topic=canal-routing-key  
# dynamic topic route by schema or table regex  
#canal.mq.dynamicTopic=mytest1.user,topic2:mytest2\..*,.*\..*  
canal.mq.partition=0
```
把这两个配置文件映射好，再次提醒，注意实例的路径名，默认是：/example/instance.properties
<a name="uhS4a"></a>
### 修改canal配置文件
需要修改这个实例配置文件，去连接MySQL，确保以下的配置正确：
```
canal.instance.master.address=mymysql:3306  
canal.instance.dbUsername=canal  
canal.instance.dbPassword=canal
```
mymysql是同为docker容器的MySQL环境，端口3306是指内部端口。<br />这里多说明一下，docker端口配置时假设为：13306:3306，那么容器对外的端口就是13306，内部是3306，在本示例中，MySQL和Canal都是容器环境，所以Canal连接MySQL需要满足以下条件：

- 处于同一网段（docker-compose.yml中的mynetwork）
- 访问内部端口（即3306，而非13306）

dbUsername和dbPassword为MySQL账号密码，为了开发方便可以使用root/root，但是仍建议自行创建用户并分配访问权限：
```bash
# 进入docker中的mysql容器  
docker exec -it mymysql bash  
# 进入mysql指令模式  
mysql -uroot -proot  
  
# 编写MySQL语句并执行  
> ...  
-- 选择mysql  
use mysql;  
-- 创建canal用户，账密：canal/canal  
create user 'canal'@'%' identified by 'canal';  
-- 分配权限，以及允许所有主机登录该用户  
grant SELECT, INSERT, UPDATE, DELETE, REPLICATION SLAVE, REPLICATION CLIENT on *.* to 'canal'@'%';  
  
-- 刷新一下使其生效  
flush privileges;  
  
-- 附带一个删除用户指令  
drop user 'canal'@'%';
```
用navicat或者shell去登录canal这个用户，可以访问即创建成功
<a name="bcd7a"></a>
## **整合SpringBoot Canal实现客户端**
Maven依赖：
```xml
<canal.version>1.1.5</canal.version>  

<!--canal-->  
<dependency>  
  <groupId>com.alibaba.otter</groupId>  
  <artifactId>canal.client</artifactId>  
  <version>${canal.version}</version>  
</dependency>  
<dependency>  
  <groupId>com.alibaba.otter</groupId>  
  <artifactId>canal.protocol</artifactId>  
  <version>${canal.version}</version>  
</dependency>
```
新增组件并启动：
```java
import com.alibaba.otter.canal.client.CanalConnector;  
import com.alibaba.otter.canal.client.CanalConnectors;  
import com.alibaba.otter.canal.protocol.CanalEntry;  
import com.alibaba.otter.canal.protocol.Message;  
import org.springframework.boot.CommandLineRunner;  
import org.springframework.stereotype.Component;  
  
import java.net.InetSocketAddress;  
import java.util.List;  
  
@Component  
public class CanalClient {  
  
    private final static int BATCH_SIZE = 1000;  
  
    public void run() {  
        // 创建链接  
        CanalConnector connector = CanalConnectors.newSingleConnector(new InetSocketAddress("localhost", 11111), "canal-exchange", "canal", "canal");  
        try {  
            //打开连接  
            connector.connect();  
            //订阅数据库表,全部表  
            connector.subscribe(".*\..*");  
            //回滚到未进行ack的地方，下次fetch的时候，可以从最后一个没有ack的地方开始拿  
            connector.rollback();  
            while (true) {  
                // 获取指定数量的数据  
                Message message = connector.getWithoutAck(BATCH_SIZE);  
                //获取批量ID  
                long batchId = message.getId();  
                //获取批量的数量  
                int size = message.getEntries().size();  
                //如果没有数据  
                if (batchId == -1 || size == 0) {  
                    try {  
                        //线程休眠2秒  
                        Thread.sleep(2000);  
                    } catch (InterruptedException e) {  
                        e.printStackTrace();  
                    }  
                } else {  
                    //如果有数据,处理数据  
                    printEntry(message.getEntries());  
                }  
                //进行 batch id 的确认。确认之后，小于等于此 batchId 的 Message 都会被确认。  
                connector.ack(batchId);  
            }  
        } catch (Exception e) {  
            e.printStackTrace();  
        } finally {  
            connector.disconnect();  
        }  
    }  
  
    /**  
     * 打印canal server解析binlog获得的实体类信息  
     */  
    private static void printEntry(List<CanalEntry.Entry> entrys) {  
        for (CanalEntry.Entry entry : entrys) {  
            if (entry.getEntryType() == CanalEntry.EntryType.TRANSACTIONBEGIN || entry.getEntryType() == CanalEntry.EntryType.TRANSACTIONEND) {  
                //开启/关闭事务的实体类型，跳过  
                continue;  
            }  
            //RowChange对象，包含了一行数据变化的所有特征  
            //比如isDdl 是否是ddl变更操作 sql 具体的ddl sql beforeColumns afterColumns 变更前后的数据字段等等  
            CanalEntry.RowChange rowChage;  
            try {  
                rowChage = CanalEntry.RowChange.parseFrom(entry.getStoreValue());  
            } catch (Exception e) {  
                throw new RuntimeException("ERROR # parser of eromanga-event has an error , data:" + entry.toString(), e);  
            }  
            //获取操作类型：insert/update/delete类型  
            CanalEntry.EventType eventType = rowChage.getEventType();  
            //打印Header信息  
            System.out.println(String.format("================》; binlog[%s:%s] , name[%s,%s] , eventType : %s",  
                    entry.getHeader().getLogfileName(), entry.getHeader().getLogfileOffset(),  
                    entry.getHeader().getSchemaName(), entry.getHeader().getTableName(),  
                    eventType));  
            //判断是否是DDL语句  
            if (rowChage.getIsDdl()) {  
                System.out.println("================》;isDdl: true,sql:" + rowChage.getSql());  
            }  
            //获取RowChange对象里的每一行数据，打印出来  
            for (CanalEntry.RowData rowData : rowChage.getRowDatasList()) {  
                //如果是删除语句  
                if (eventType == CanalEntry.EventType.DELETE) {  
                    printColumn(rowData.getBeforeColumnsList());  
                    //如果是新增语句  
                } else if (eventType == CanalEntry.EventType.INSERT) {  
                    printColumn(rowData.getAfterColumnsList());  
                    //如果是更新的语句  
                } else {  
                    //变更前的数据  
                    System.out.println("------->; before");  
                    printColumn(rowData.getBeforeColumnsList());  
                    //变更后的数据  
                    System.out.println("------->; after");  
                    printColumn(rowData.getAfterColumnsList());  
                }  
            }  
        }  
    }  
  
    private static void printColumn(List<CanalEntry.Column> columns) {  
        for (CanalEntry.Column column : columns) {  
            System.out.println(column.getName() + " : " + column.getValue() + "    update=" + column.getUpdated());  
        }  
    }  
}
```
启动类Application：
```java
@SpringBootApplication  
public class BaseApplication implements CommandLineRunner {  
    @Autowired  
    private CanalClient canalClient;  

    @Override  
    public void run(String... args) throws Exception {  
        canalClient.run();  
    }  
}
```
启动程序，此时新增或修改数据库中的数据，就能从客户端中监听到<br />不过建议监听的信息放到消息队列中，在空闲的时候去处理，所以直接配置Canal整合RabbitMQ更好。
<a name="tV0ZQ"></a>
## **Canal整合RabbitMQ**
修改canal.properties中的serverMode：
```
canal.serverMode = rabbitMQ
```
修改instance.properties中的topic：
```
canal.mq.topic=canal-routing-key
```
然后找到关于RabbitMQ的配置：
```
#################################################  
########         RabbitMQ       ############  
#################################################  
# 连接rabbit，写IP，因为同个网络下，所以可以写容器名  
rabbitmq.host = myrabbit  
rabbitmq.virtual.host = /  
# 交换器名称，等等我们要去手动创建  
rabbitmq.exchange = canal-exchange  
# 账密  
rabbitmq.username = admin  
rabbitmq.password = 123456  
# 暂不支持指定端口，使用的是默认的5762，好在在本示例中适用
```
重新启动容器，进入RabbitMQ管理页面创建exchange交换器和队列queue：

- 新建exchange，命名为：canal-exchange
- 新建queue，命名为：canal-queue
- 绑定exchange和queue，routing-key设置为：canal-routing-key，这里对应上面instance.properties的canal.mq.topic

顺带一提，上面这段可以忽略，因为在SpringBoot的RabbitMQ配置中，会自动创建交换器exchange和队列queue，不过手动创建的话，可以在忽略SpringBoot的基础上，直接在RabbitMQ的管理页面上看到修改记录的消息。
<a name="lA0Do"></a>
## **SpringBoot整合RabbitMQ**
依赖：
```xml
<amqp.version>2.3.4.RELEASE</amqp.version>  

<!--消息队列-->  
<dependency>  
  <groupId>org.springframework.boot</groupId>  
  <artifactId>spring-boot-starter-amqp</artifactId>  
  <version>${amqp.version}</version>  
</dependency>
```
**application.yml** ：
```yaml
spring:  
  rabbitmq:  
    #    host: myserverhost  
    host: 192.168.0.108  
    port: 5672  
    username: admin  
    password: RabbitMQ密码  
    # 消息确认配置项  
    # 确认消息已发送到交换机(Exchange)  
    publisher-confirm-type: correlated  
    # 确认消息已发送到队列(Queue)  
    publisher-returns: true
```
RabbitMQ配置类：
```java
@Configuration  
public class RabbitConfig {  
    @Bean  
    public RabbitTemplate rabbitTemplate(ConnectionFactory connectionFactory) {  
        RabbitTemplate template = new RabbitTemplate();  
        template.setConnectionFactory(connectionFactory);  
        template.setMessageConverter(new Jackson2JsonMessageConverter());  

        return template;  
    }  

    /**  
     * template.setMessageConverter(new Jackson2JsonMessageConverter());  
     * 这段和上面这行代码解决RabbitListener循环报错的问题  
     */  
    @Bean  
    public SimpleRabbitListenerContainerFactory rabbitListenerContainerFactory(ConnectionFactory connectionFactory) {  
        SimpleRabbitListenerContainerFactory factory = new SimpleRabbitListenerContainerFactory();  
        factory.setConnectionFactory(connectionFactory);  
        factory.setMessageConverter(new Jackson2JsonMessageConverter());  
        return factory;  
    }  
}
```
Canal消息生产者：
```java
public static final String CanalQueue = "canal-queue";  
public static final String CanalExchange = "canal-exchange";  
public static final String CanalRouting = "canal-routing-key";  

/**  
 * Canal消息提供者，canal-server生产的消息通过RabbitMQ消息队列发送  
 */  
@Configuration  
public class CanalProvider {  
    /**  
     * 队列  
     */  
    @Bean  
    public Queue canalQueue() {  
        /**  
         * durable:是否持久化，默认false，持久化队列：会被存储在磁盘上，当消息代理重启时仍然存在；暂存队列：当前连接有效  
         * exclusive:默认为false，只能被当前创建的连接使用，而且当连接关闭后队列即被删除。此参考优先级高于durable  
         * autoDelete:是否自动删除，当没有生产者或者消费者使用此队列，该队列会自动删除  
         */  
        return new Queue(RabbitConstant.CanalQueue, true);  
    }  

    /**  
     * 交换机，这里使用直连交换机  
     */  
    @Bean  
    DirectExchange canalExchange() {  
        return new DirectExchange(RabbitConstant.CanalExchange, true, false);  
    }  

    /**  
     * 绑定交换机和队列，并设置匹配键  
     */  
    @Bean  
    Binding bindingCanal() {  
        return BindingBuilder.bind(canalQueue()).to(canalExchange()).with(RabbitConstant.CanalRouting);  
    }  
}
```
Canal消息消费者：
```java
/**  
 * Canal消息消费者  
 */  
@Component  
@RabbitListener(queues = RabbitConstant.CanalQueue)  
public class CanalComsumer {  
    private final SysBackupService sysBackupService;  

    public CanalComsumer(SysBackupService sysBackupService) {  
        this.sysBackupService = sysBackupService;  
    }  

    @RabbitHandler  
    public void process(Map<String, Object> msg) {  
        System.out.println("收到canal消息：" + msg);  
        boolean isDdl = (boolean) msg.get("isDdl");  

        // 不处理DDL事件  
        if (isDdl) {  
            return;  
        }  

        // TiCDC的id，应该具有唯一性，先保存再说  
        int tid = (int) msg.get("id");  
        // TiCDC生成该消息的时间戳，13位毫秒级  
        long ts = (long) msg.get("ts");  
        // 数据库  
        String database = (String) msg.get("database");  
        // 表  
        String table = (String) msg.get("table");  
        // 类型：INSERT/UPDATE/DELETE  
        String type = (String) msg.get("type");  
        // 每一列的数据值  
        List<?> data = (List<?>) msg.get("data");  
        // 仅当type为UPDATE时才有值，记录每一列的名字和UPDATE之前的数据值  
        List<?> old = (List<?>) msg.get("old");  

        // 跳过sys_backup，防止无限循环  
        if ("sys_backup".equalsIgnoreCase(table)) {  
            return;  
        }  

        // 只处理指定类型  
        if (!"INSERT".equalsIgnoreCase(type)  
            && !"UPDATE".equalsIgnoreCase(type)  
            && !"DELETE".equalsIgnoreCase(type)) {  
            return;  
        }  
    }  
}
```
测试一下，修改MySQL中的一条消息，Canal就会发送信息到RabbitMQ，就能从监听的RabbitMQ队列中得到该条消息。
