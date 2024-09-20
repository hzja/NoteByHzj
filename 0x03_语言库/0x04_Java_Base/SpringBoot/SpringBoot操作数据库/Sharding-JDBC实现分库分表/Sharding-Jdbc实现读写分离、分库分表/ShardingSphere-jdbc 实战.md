JavaSpringBoot<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415274452-9ee39551-08ee-4415-a91d-180c692b36e3.png#averageHue=%23f9f2ea&clientId=u2315dcf9-8a21-4&from=paste&id=u41e9e390&originHeight=519&originWidth=608&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8446b9dd-533f-4af4-9102-d02e8073612&title=)
<a name="Nz4BS"></a>
## 1、ShardingSphere 生态
Apache ShardingSphere 是一款分布式的数据库生态系统，它包含两大产品：

- ShardingSphere-Proxy
- ShardingSphere-JDBC
<a name="njdRD"></a>
### ▍一、ShardingSphere-Proxy
ShardingSphere-Proxy 被定位为透明化的数据库代理端，提供封装了数据库二进制协议的服务端版本，用于完成对异构语言的支持。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1678415274404-bfa45bb2-6268-4a17-8c4e-030ec12dae5b.jpeg#averageHue=%23f7f7f7&clientId=u2315dcf9-8a21-4&from=paste&id=u0834a480&originHeight=599&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uef391649-29ee-40bc-ac47-a254f07cb11&title=)<br />代理层介于应用程序与数据库间，每次请求都需要做一次转发，请求会存在额外的时延。<br />这种方式对于应用非常友好，应用基本零改动，和语言无关，可以通过连接共享减少连接数消耗。
<a name="D9uuG"></a>
### ▍二、ShardingSphere-JDBC
ShardingSphere-JDBC 是 ShardingSphere 的第一个产品，也是 ShardingSphere 的前身， 经常简称之为：sharding-jdbc 。<br />它定位为轻量级 Java 框架，在 Java 的 JDBC 层提供的额外服务。它使用客户端直连数据库，以 jar 包形式提供服务，无需额外部署和依赖，可理解为增强版的 JDBC 驱动，完全兼容 JDBC 和各种 ORM 框架。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1678415274441-f740c9e3-a9a1-409b-a3bb-614bac9960e6.jpeg#averageHue=%23accd95&clientId=u2315dcf9-8a21-4&from=paste&id=u10b1e1d3&originHeight=620&originWidth=693&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue9235b02-d372-47e5-8eee-15b5bb8e8dc&title=)

---

当在 Proxy 和 JDBC 两种模式选择时，可以参考下表对照：

| <br /> | JDBC | Proxy |
| --- | --- | --- |
| 数据库 | 任意 | MySQL/PostgreSQL |
| 连接消耗数 | 高 | 低 |
| 异构语言 | 仅Java | 任意 |
| 性能 | 损耗低 | 损耗略高 |
| 无中心化 | 是 | 否 |
| 静态入口 | 无 | 有 |

越来越多的公司都在生产环境使用了 sharding-jdbc ，最核心的原因就是：**简单**（原理简单，易于实现，方便运维）。
<a name="PS3Ib"></a>
## 2、基本原理
在后端开发中，JDBC 编程是最基本的操作。不管 ORM 框架是 Mybatis 还是 Hibernate ，亦或是 spring-jpa ，他们的底层实现是 JDBC 的模型。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415274444-310303e1-acdc-4c02-a030-567e3b9b73c5.png#averageHue=%23092c49&clientId=u2315dcf9-8a21-4&from=paste&id=ubf5b82b9&originHeight=379&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5e00a5b5-4612-4191-af23-5fbc2d6f699&title=)<br />**sharding-jdbc 的本质上就是实现 JDBC 的核心接口。**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415274449-222362c4-7a0d-42fc-bd1b-03a65cb57d33.png#averageHue=%23f7f6ee&clientId=u2315dcf9-8a21-4&from=paste&id=u97a44640&originHeight=609&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u064e65b6-067a-4560-9b70-17a118adc7f&title=)

| 接口 | 实现类 |
| --- | --- |
| DataSource | ShardingDataSource |
| Connection | ShardingConnection |
| Statement | ShardingStatement |
| PreparedStatement | ShardingPreparedStatement |
| ResultSet | ShardingResultSet |

虽然理解了 sharding-jdbc 的本质，但是真正实现起来还有非常多的细节，下图展示了 Prxoy 和 JDBC 两种模式的核心流程。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415274868-bf762e8b-2b0f-4ad7-8250-2caaf1c95dd9.png#averageHue=%2387a44c&clientId=u2315dcf9-8a21-4&from=paste&id=uf8f520b7&originHeight=499&originWidth=491&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc0bca8d1-c784-43ca-8cbf-3bdd540b207&title=)
<a name="MxjO9"></a>
### 1、SQL 解析
分为词法解析和语法解析。先通过词法解析器将 SQL 拆分为一个个不可再分的单词。再使用语法解析器对 SQL 进行理解，并最终提炼出解析上下文。<br />解析上下文包括表、选择项、排序项、分组项、聚合函数、分页信息、查询条件以及可能需要修改的占位符的标记。
<a name="YRkOf"></a>
### 2、执行器优化
合并和优化分片条件，如 OR 等。
<a name="eDklF"></a>
### 3、SQL 路由
根据解析上下文匹配用户配置的分片策略，并生成路由路径。目前支持分片路由和广播路由。
<a name="yCof3"></a>
### 4、SQL 改写
将 SQL 改写为在真实数据库中可以正确执行的语句。SQL 改写分为正确性改写和优化改写。
<a name="JwQgN"></a>
### 5、SQL 执行
通过多线程执行器异步执行。
<a name="mqhhM"></a>
### 6、结果归并
将多个执行结果集归并以便于通过统一的 JDBC 接口输出。结果归并包括**流式归并**、**内存归并**和使用装饰者模式的**追加归并**这几种方式。
<a name="SZ0Lv"></a>
## 3、实战案例
武汉一家 O2O 公司订单服务做过分库分表架构设计 ，当企业用户创建一条采购订单 ， 会生成如下记录：

- 订单基础表**t_ent_order** ：单条记录
- 订单详情表**t_ent_order_detail** ：单条记录
- 订单明细表**t_ent_order_item：N** 条记录

订单数据采用了如下的分库分表策略：

- 订单基础表按照 ent_id (企业用户编号) 分库 ，订单详情表保持一致；
- 订单明细表按照 ent_id (企业用户编号) 分库，同时也要按照 ent_id (企业编号) 分表。

首先创建 4 个库，分别是：ds_0、ds_1、ds_2、ds_3 。<br />这四个分库，每个分库都包含 订单基础表 ， 订单详情表 ，订单明细表 。但是因为明细表需要分表，所以包含多张表。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415274973-cd23697b-b103-4435-8755-0bb981113379.png#averageHue=%23a7d69b&clientId=u2315dcf9-8a21-4&from=paste&id=u4f583001&originHeight=705&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub381bd64-b3a8-4f84-82e1-387939c229f&title=)<br />然后 springboot 项目中配置依赖 ：
```xml
<dependency>
    <groupId>org.apache.shardingsphere</groupId>
    <artifactId>sharding-jdbc-spring-boot-starter</artifactId>
    <version>4.1.1</version>
</dependency>
```
配置文件中配置如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415274970-34ad1a7d-2b93-4825-a391-0daaa01c1211.png#averageHue=%23072845&clientId=u2315dcf9-8a21-4&from=paste&id=uef50bdd4&originHeight=1619&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub3fe59b8-32e3-4ef6-818e-8779d22b31c&title=)

- 配置数据源，上面配置数据源是：ds0、ds1、ds2、ds3 ；
- 配置打印日志，也就是：sql.show ，在测试环境建议打开 ，便于调试；
- 配置哪些表需要分库分表 ，在 shardingsphere.datasource.sharding.tables 节点下面配置：

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415275134-0e3f762b-7352-4137-8f0c-32aa18063111.png#averageHue=%23f9f2f2&clientId=u2315dcf9-8a21-4&from=paste&id=u49d6bb28&originHeight=218&originWidth=805&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue33d6f53-9fb3-46d6-8f3d-faa6d404fc1&title=)<br />上图中看到配置分片规则包含如下两点：
<a name="YAce7"></a>
### 1、真实节点
对于应用来讲，查询的**逻辑表**是：t_ent_order_item 。<br />它们在数据库中的真实形态是：t_ent_order_item_0 到  t_ent_order_item_7。<br />真实数据节点是指数据分片的最小单元，由数据源名称和数据表组成。<br />订单明细表的真实节点是：ds$->{0..3}.t_ent_order_item_$->{0..7} 。
<a name="ppD6k"></a>
### 2、分库分表算法
配置分库策略和分表策略，每种策略都需要配置**分片字段**（ sharding-columns ）和**分片算法**。
<a name="Kxp5Q"></a>
## 4、基因法 & 自定义复合分片算法
分片算法和阿里开源的数据库中间件 cobar 路由算法非常类似的。<br />假设现在需要将订单表平均拆分到4个分库 shard0 ，shard1 ，shard2 ，shard3 。<br />首先将 [0-1023] 平均分为4个区段：[0-255]，[256-511]，[512-767]，[768-1023]，然后对字符串（或子串，由用户自定义）做 hash， hash 结果对 1024 取模，最终得出的结果 **slot** 落入哪个区段，便路由到哪个分库。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415275086-f68e5bd8-dde1-4487-a915-def82e1d7fc6.png#averageHue=%23f8f5f5&clientId=u2315dcf9-8a21-4&from=paste&id=u741a086a&originHeight=591&originWidth=929&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf7f8d68-873c-4080-8c13-87fa3302203&title=)<br />看起来分片算法很简单，但需要按照订单 ID 查询订单信息时依然需要路由四个分片，效率不高，那么如何优化呢 ？<br />答案是：**基因法** & **自定义复合分片算法**。<br />基因法是指在订单 ID 中携带企业用户编号信息，可以在创建订单 **order_id** 时使用雪花算法，然后将 **slot** 的值保存在 **10位工作机器 ID** 里。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415275394-ca496502-d159-4dfd-8648-af9e70f4f712.png#averageHue=%23faeee6&clientId=u2315dcf9-8a21-4&from=paste&id=u70c562d8&originHeight=195&originWidth=911&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u51e1de47-350f-4687-902e-629c54e9674&title=)<br />通过订单 order_id   可以反查出  **slot，**就可以定位该用户的订单数据存储在哪个分片里。
```java
Integer getWorkerId(Long orderId) {
    Long workerId = (orderId >> 12) & 0x03ff;
	return workerId.intValue();
}
```
下图展示了订单 ID 使用雪花算法的生成过程，生成的编号会携带企业用户 ID 信息。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415275741-109ad796-b325-4920-bcc5-9a25340d00ce.png#averageHue=%23f8f8f8&clientId=u2315dcf9-8a21-4&from=paste&id=ufbf7178d&originHeight=425&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4505d122-7725-4997-b81d-9637c4acd05&title=)<br />解决了分布式 ID 问题，接下来的一个问题：sharding-jdbc 可否支持按照订单 ID ，企业用户 ID 两个字段来决定分片路由吗？<br />答案是：**自定义复合分片算法**。只需要实现 **ComplexKeysShardingAlgorithm** 类即可。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415275972-722ab8fe-c498-4dae-a9d2-563e00126789.png#averageHue=%23f6f0f0&clientId=u2315dcf9-8a21-4&from=paste&id=ue00b6d90&originHeight=640&originWidth=991&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf431f32b-2747-4090-b424-fc2ec216dd5&title=)<br />复合分片的算法流程非常简单：<br />1.分片键中有主键值，则直接通过主键解析出路由分片；<br />2.分片键中不存在主键值 ，则按照其他分片字段值解析出路由分片。
<a name="xvl58"></a>
## 5、扩容方案
既然做了分库分表，如何实现**平滑扩容**也是一个非常有趣的话题。<br />在数据同步之前，需要梳理迁移范围。<br />1.**业务唯一主键**；<br />在进行数据同步前，需要先梳理所有表的唯一业务 ID，只有确定了唯一业务 ID 才能实现数据的同步操作。<br />需要注意的是：业务中是否有使用数据库自增 ID 做为业务 ID 使用的，如果有需要业务先进行改造 。另外确保每个表是否都有唯一索引，一旦表中没有唯一索引，就会在数据同步过程中造成数据重复的风险，所以先将没有唯一索引的表根据业务场景增加唯一索引（有可能是联合唯一索引）。<br />2.**迁移哪些表，迁移后的分库分表规则**；<br />分表规则不同决定着 rehash 和数据校验的不同。需逐个表梳理是用户ID纬度分表还是非用户ID纬度分表、是否只分库不分表、是否不分库不分表等等。<br />接下来，**进入数据同步环节**。<br />整体方案见下图，数据同步基于 binlog ，独立的中间服务做同步，对业务代码无侵入。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415275958-0ff36ffe-5d87-497c-827d-8691e7bd59b9.png#averageHue=%23faf5ef&clientId=u2315dcf9-8a21-4&from=paste&id=u251a3b5a&originHeight=321&originWidth=642&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3883f412-ecb1-4881-888a-fd88a55c6f8&title=)<br />首先需要做**历史数据全量同步**：也就是将旧库迁移到新库。<br />单独一个服务，使用**游标**的方式从旧库分片 select 语句，经过 rehash 后批量插入 （batch insert）到新库，需要配置jdbc 连接串参数 `rewriteBatchedStatements=true` 才能使批处理操作生效。<br />因为历史数据也会存在不断的更新，如果先开启历史数据全量同步，则刚同步完成的数据有可能不是最新的。<br />所以会先开启增量数据单向同步（从旧库到新库），此时只是开启积压 kafka 消息并不会真正消费；然后在开始历史数据全量同步，当历史全量数据同步完成后，在开启消费 kafka 消息进行增量数据同步（提高全量同步效率减少积压也是关键的一环），这样来保证迁移数据过程中的数据一致。<br />增量数据同步考虑到灰度切流稳定性、容灾 和可回滚能力 ，采用实时双向同步方案，切流过程中一旦新库出现稳定性问题或者新库出现数据一致问题，可快速回滚切回旧库，保证数据库的稳定和数据可靠。<br />增量数据实时同步的大体思路 ：
<a name="zTu10"></a>
### 1、过滤循环消息
需要过滤掉循环同步的 binlog 消息 ;
<a name="eM3k5"></a>
### 2、数据合并
同一条记录的多条操作只保留最后一条。为了提高性能，数据同步组件接到 kafka 消息后不会立刻进行数据流转，而是先存到本地阻塞队列，然后由本地定时任务每X秒将本地队列中的N条数据进行数据流转操作。此时N条数据有可能是对同一张表同一条记录的操作，所以此处只需要保留最后一条（类似于 redis aof 重写）;
<a name="KuTjH"></a>
### 3、update 转 insert
数据合并时，如果数据中有 insert + update 只保留最后一条 update ，会执行失败，所以此处需要将 update 转为 insert 语句 ;
<a name="TH0Bk"></a>
### 4、按新表合并
将最终要提交的 N 条数据，按照新表进行拆分合并，这样可以直接按照新表纬度进行数据库批量操作，提高插入效率。
<a name="blUBa"></a>
## 6、总结
sharding-jdbc 的本质是实现 JDBC 的核心接口，架构相对简单。<br />实战过程中，需要配置数据源信息，逻辑表对应的真实节点和分库分表策略（**分片字段**和**分片算法**）<br />实现分布式主键直接路由到对应分片，则需要使用**基因法 & 自定义复合分片算法** 。<br />平滑扩容的核心是**全量同步**和**实时双向同步**，工程上有不少细节。

---

实战代码地址：[https://github.com/makemyownlife/shardingsphere-jdbc-demo](https://github.com/makemyownlife/shardingsphere-jdbc-demo)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678415275982-9077334d-8c9d-4958-b007-3d4611701019.png#averageHue=%23d0b15f&clientId=u2315dcf9-8a21-4&from=paste&id=u7c0122af&originHeight=352&originWidth=897&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud66ea90b-08a0-4a8f-9c1e-6bb3150e913&title=)
