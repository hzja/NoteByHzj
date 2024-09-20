Java SpringBoot InfluxDB
<a name="HGDwx"></a>
## InfluxDB简介
什么是时序数据库？全称为时间序列数据库。时间序列数据库主要用于指处理带时间标签（按照时间的顺序变化，即时间序列化）的数据，带时间标签的数据也称为时间序列数据。时间序列数据主要由电力行业、化工行业等各类型实时监测、检查与分析设备所采集、产生的数据，这些工业数据的典型特点是：产生频率快（每一个监测点一秒钟内可产生多条数据）、严重依赖于采集时间（每一条数据均要求对应唯一的时间）、测点多信息量大（常规的实时监测系统均有成千上万的监测点，监测点每秒钟都产生数据，每天产生几十GB的数据量）。虽然关系型数据库也可以存储基于时间序列的数据，但由于存储结构上的劣势，使得这些数据无法高效的实现高频存储和查询统计，因此就诞生了一种专门针对时间序列来做存储和优化的数据库，以满足更高的效率要求。<br />InfluxDB就是目前比较流行的开源时序数据库（官网地址：[https://www.influxdata.com/](https://www.influxdata.com/)），比较常见的使用场景就是一些与时间相关的高频的数据记录和统计需要，比如：监控数据的存储和查询。<br />在进行下面的动手环节之前，先了解一下InfluxDB中的几个重要名词：

- database：数据库
- measurement：类似于关系数据库中的table（表）
- points：类似于关系数据库中的row（一行数据）

其中，一个Point由三个部分组成：

- time：时间戳
- fields：记录的值
- tags：索引的属性
<a name="J49XO"></a>
## 动手试试
在了解了什么是时序数据库以及InfluxDB一些基础概念之后，下面通过一个简单的定时上报监控数据的小案例，进一步理解InfluxDB的基础配置、数据组织和写入操作！
<a name="uqmWc"></a>
### 第一步：创建一个基础的Spring Boot项目
<a name="t2oQ5"></a>
### 第二步：在pom.xml中引入influx的官方SDK
```xml
<dependency>
    <groupId>org.influxdb</groupId>
    <artifactId>influxdb-java</artifactId>
</dependency>
```
注意：这里因为Spring Boot 2.x版本的parent中有维护InfluxDB的SDK版本，所以不需要手工指明版本信息。如果使用的Spring Boot版本比较老，那么可能会缺少version信息，就需要手工写了。
<a name="cUjUx"></a>
### 第三步：配置要连接的influxdb信息
```
spring.influx.url=http://localhost:8086
spring.influx.user=admin
spring.influx.password=
```
三个属性分别代表：连接地址、用户名、密码。到这一步，基础配置就完成了。<br />注意：虽然没有spring data的支持，但spring boot 2.x版本中也实现了InfluxDB的自动化配置，所以只需要写好配置信息，就可以使用了。具体配置属性可以查看源码：org.springframework.boot.autoconfigure.influx.InfluxDbProperties。
<a name="Dvzhl"></a>
### 第四步：创建定时任务，模拟上报数据，并写入InfluxDB
```java
@Service
@AllArgsConstructor
@Slf4j
public class Monitor {

    private InfluxDB influxDB;

    @Scheduled(fixedRate = 5000)
    public void writeQPS() {
        // 模拟要上报的统计数据
        int count = (int) (Math.random() * 100);

        Point point = Point.measurement("ApiQPS")     // ApiQPS表
                .tag("url", "/hello")  // url字段
                .addField("count", count)        // 统计数据
                .time(System.currentTimeMillis(), TimeUnit.MILLISECONDS)  // 时间
                .build();

        // 往test库写数据
        influxDB.write("test", "autogen", point);

        log.info("上报统计数据：" + count);
    }

}
```
<a name="SyhTC"></a>
## 测试验证
<a name="FwWZo"></a>
### 第一步：启动InfluxDB，并通过命令行准备好要使用的数据库，主要涉及的命令如下；

- 进入InfluxDB：
```sql
$ influx
```

- 查询当前存在的数据库：
```sql
> show databases
```

- 创建数据库（注意数据库名称与上面Java代码中write的第一个参数一致）：
```sql
> create database "test"
```
<a name="IbYbo"></a>
### 第二步：启动Spring Boot应用，在定时任务的作用下，会看到类似下面的日志：
```xml
2021-08-03 01:52:47.732  INFO 94110 --- [           main] c.d.chapter63.Chapter63Application       : Started Chapter63Application in 2.326 seconds (JVM running for 3.027)
2021-08-03 01:52:47.764  INFO 94110 --- [   scheduling-1] com.didispace.chapter63.Monitor          : 上报统计数据：25
2021-08-03 01:52:52.736  INFO 94110 --- [   scheduling-1] com.didispace.chapter63.Monitor          : 上报统计数据：30
2021-08-03 01:52:57.737  INFO 94110 --- [   scheduling-1] com.didispace.chapter63.Monitor          : 上报统计数据：38
2021-08-03 01:53:02.739  INFO 94110 --- [   scheduling-1] com.didispace.chapter63.Monitor          : 上报统计数据：51
2021-08-03 01:53:07.739  INFO 94110 --- [   scheduling-1] com.didispace.chapter63.Monitor          : 上报统计数据：31
```
<a name="iNTS0"></a>
### 第三步：通过命令，查看一下InfluxDB中是否已经存在这些数据
```sql
> select * from ApiQPS order by time desc;

name: ApiQPS
time                count url
----                ----- ---
1627926787730000000 31    /hello
1627926782730000000 51    /hello
1627926777729000000 38    /hello
1627926772727000000 30    /hello
1627926767728000000 25    /hello
```
可以看到，已经存在与日志中一样的数据了。
