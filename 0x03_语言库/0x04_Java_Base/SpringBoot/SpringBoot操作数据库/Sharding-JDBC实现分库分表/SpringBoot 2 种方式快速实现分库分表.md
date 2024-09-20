JavaSpringBoot
<a name="E39G5"></a>
## 一、什么是 ShardingSphere？
shardingsphere 是一款开源的分布式关系型数据库中间件，为 Apache 的顶级项目。其前身是 sharding-jdbc 和 sharding-proxy 的两个独立项目，后来在 2018 年合并成了一个项目，并正式更名为 ShardingSphere。<br />其中 sharding-jdbc 为整个生态中最为经典和成熟的框架，最早接触分库分表的人应该都知道它，是学习分库分表的最佳入门工具。<br />如今的 ShardingSphere 已经不再是单纯代指某个框架，而是一个完整的技术生态圈，由三款开源的分布式数据库中间件 sharding-jdbc、sharding-proxy 和 sharding-sidecar 所构成。前两者问世较早，功能较为成熟，是目前广泛应用的两个分布式数据库中间件，将重点介绍它们的特点和使用方法。
<a name="vlnmM"></a>
## 二、为什么选 ShardingSphere？
为了回答这个问题，整理了市面上常见的分库分表工具，包括 ShardingSphere、Cobar、Mycat、TDDL、MySQL Fabric 等，并从多个角度对它们进行了简单的比较。
<a name="oQMv0"></a>
### Cobar
Cobar 是阿里巴巴开源的一款基于MySQL的分布式数据库中间件，提供了分库分表、读写分离和事务管理等功能。它采用轮询算法和哈希算法来进行数据分片，支持分布式分表，但是不支持单库分多表。<br />它以 Proxy 方式提供服务，在阿里内部被广泛使用已开源，配置比较容易，无需依赖其他东西，只需要有Java环境即可。兼容市面上几乎所有的 ORM 框架，仅支持 MySQL 数据库，且事务支持方面比较麻烦。
<a name="VtPpe"></a>
### MyCAT
Mycat 是社区爱好者在阿里 Cobar 基础上进行二次开发的，也是一款比较经典的分库分表工具。它以 Proxy 方式提供服务，支持分库分表、读写分离、SQL路由、数据分片等功能。<br />兼容市面上几乎所有的 ORM 框架，包括 Hibernate、MyBatis和 JPA等都兼容，不过，美中不足的是它仅支持 MySQL数据库，目前社区的活跃度相对较低。
<a name="FDjOq"></a>
### TDDL
TDDL 是阿里巴巴集团开源的一款分库分表解决方案，可以自动将SQL路由到相应的库表上。它采用了垂直切分和水平切分两种方式来进行分表分库，并且支持多数据源和读写分离功能。<br />TDDL 是基于 Java 开发的，支持 MySQL、Oracle 和 SQL Server 数据库，并且可以与市面上 Hibernate、MyBatis等 ORM 框架集成。<br />不过，TDDL仅支持一些阿里巴巴内部的工具和框架的集成，对于外部公司来说可能相对有些局限性。同时，其文档和社区活跃度相比 ShardingSphere 来说稍显不足。
<a name="GKDWr"></a>
### Mysql Fabric
MySQL Fabric是 MySQL 官方提供的一款分库分表解决方案，同时也支持 MySQL其他功能，如高可用、负载均衡等。它采用了管理节点和代理节点的架构，其中管理节点负责实时管理分片信息，代理节点则负责接收并处理客户端的读写请求。<br />它仅支持 MySQL 数据库，并且可以与市面上 Hibernate、MyBatis 等 ORM 框架集成。MySQL Fabric 的文档相对来说比较简略，而且由于是官方提供的解决方案，其社区活跃度也相对较低。
<a name="RopR6"></a>
### ShardingSphere
ShardingSphere 成员中的 sharding-jdbc 以 JAR 包的形式下提供分库分表、读写分离、分布式事务等功能，但仅支持 Java 应用，在应用扩展上存在局限性。<br />因此，ShardingSphere 推出了独立的中间件 sharding-proxy，它基于 MySQL协议实现了透明的分片和多数据源功能，支持各种语言和框架的应用程序使用，对接的应用程序几乎无需更改代码，分库分表配置可在代理服务中进行管理。<br />除了支持 MySQL，ShardingSphere还可以支持 PostgreSQL、SQLServer、Oracle等多种主流数据库，并且可以很好地与 Hibernate、MyBatis、JPA等 ORM 框架集成。重要的是，ShardingSphere的开源社区非常活跃。<br />如果在使用中出现问题，用户可以在 GitHub 上提交PR并得到快速响应和解决，这为用户提供了足够的安全感。
<a name="PblM9"></a>
### 产品比较
通过对上述的 5 个分库分表工具进行比较，不难发现，就整体性能、功能丰富度以及社区支持等方面来看，ShardingSphere 在众多产品中优势还是比较突出的。下边用各个产品的主要指标整理了一个表格，看着更加直观一点。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288658324-1702ba12-99bd-4480-b1df-6691e63b229c.png#averageHue=%23ecebeb&clientId=u8712b1b7-88f2-4&from=paste&id=u0f3b5c03&originHeight=491&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufa8cb6e6-cc9f-4bd7-bc50-336e992fd8a&title=)
<a name="li6xc"></a>
## 三、ShardingSphere 成员
ShardingSphere 的主要组成成员为sharding-jdbc、sharding-proxy，它们是实现分库分表的两种不同模式：
<a name="NACp6"></a>
### sharding-jdbc
它是一款轻量级Java框架，提供了基于 JDBC 的分库分表功能，为客户端直连模式。使用sharding-jdbc，开发者可以通过简单的配置实现数据的分片，同时无需修改原有的SQL语句。支持多种分片策略和算法，并且可以与各种主流的ORM框架无缝集成。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288658334-3bf39ece-08cb-4d65-8aed-451bd11bf4bb.png#averageHue=%23fcf9f7&clientId=u8712b1b7-88f2-4&from=paste&id=uca2f5377&originHeight=468&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7a6ef8cb-7b0a-4564-8caa-86f25bb89d1&title=)
<a name="vXTdT"></a>
### sharding-proxy
它是基于 MySQL 协议的代理服务，提供了透明的分库分表功能。使用 sharding-proxy 开发者可以将分片逻辑从应用程序中解耦出来，无需修改应用代码就能实现分片功能，还支持多数据源和读写分离等高级特性，并且可以作为独立的服务运行。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288658372-03d3b753-0c69-4ad5-a960-887a87bfcc93.png#averageHue=%23fdfcfb&clientId=u8712b1b7-88f2-4&from=paste&id=uab39d954&originHeight=540&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9ac0af16-7a7c-4002-aa87-99c3d6645a5&title=)
<a name="pZSOi"></a>
## 四、快速实现
先使用sharding-jdbc来快速实现分库分表。相比于 sharding-proxy，sharding-jdbc 适用于简单的应用场景，不需要额外的环境搭建等。下边主要基于 SpringBoot 的两种方式来实现分库分表，一种是通过YML配置方式，另一种则是通过纯Java编码方式（不可并存）。<br />ShardingSphere 官网地址：[https://shardingsphere.apache.org/](https://shardingsphere.apache.org/)
<a name="xFXUp"></a>
### 准备工作
在开始实现之前，需要对数据库和表的拆分规则进行明确。以对t_order表进行分库分表拆分为例，具体地，将 t_order 表拆分到两个数据库中，分别为db1和db2，每个数据库又将该表拆分为三张表，分别为t_order_1、t_order_2和t_order_3。
```
db0
├── t_order_0
├── t_order_1
└── t_order_2
db1
├── t_order_0
├── t_order_1
└── t_order_2
```
<a name="GXbDS"></a>
### JAR包引入
引入必要的 JAR 包，其中最重要的是shardingsphere-jdbc-core-spring-boot-starter和mysql-connector-java这两个。为了保证功能的全面性和兼容性，以及避免因低版本包导致的不必要错误和调试工作，选择的包版本都较高。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288658306-269b1631-8186-429f-93de-02e2a2cad397.png#averageHue=%23f5f5f5&clientId=u8712b1b7-88f2-4&from=paste&id=u4ddb80ef&originHeight=398&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u40d3023c-0d9d-4a91-9552-4dccf603666&title=)<br />shardingsphere-jdbc-core-spring-boot-starter 是 ShardingSphere 框架的核心组件，提供了对 JDBC 的分库分表支持；而 mysql-connector-java 则是 MySQL JDBC 驱动程序的实现，用于连接MySQL数据库。除此之外，使用了JPA作为持久化工具还引入了相应的依赖包。
```xml
<!-- jpa持久化工具 -->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-jpa</artifactId>
  <version>2.7.6</version>
</dependency>
<!-- 必须引入的包 mysql -->
<dependency>
  <groupId>mysql</groupId>
  <artifactId>mysql-connector-java</artifactId>
  <version>8.0.31</version>
</dependency>
<!-- 必须引入的包 ShardingSphere -->
<dependency>
  <groupId>org.apache.shardingsphere</groupId>
  <artifactId>shardingsphere-jdbc-core-spring-boot-starter</artifactId>
  <version>5.2.0</version>
</dependency>
```
<a name="skust"></a>
### YML配置
比较推荐使用YML配置方式来实现 sharding-jdbc 分库分表的，使用YML配置方式不仅可以让分库分表的实现更加简单、高效、可维护，也更符合 SpringBoot的开发规范。<br />在 src/main/resources/application.yml 路径文件下添加以下完整的配置，即可实现对t_order表的分库分表，接下来拆解看看每个配置模块都做了些什么。
```yaml
spring:
  shardingsphere:
    # 数据源配置
    datasource:
      # 数据源名称，多数据源以逗号分隔
      names: db0,db1
      db0:
        type: com.zaxxer.hikari.HikariDataSource
        driver-class-name: com.mysql.cj.jdbc.Driver
        jdbc-url: jdbc:mysql://127.0.0.1:3306/shardingsphere-db1?useUnicode=true&characterEncoding=utf-8&useSSL=false&serverTimezone=Asia/Shanghai&allowPublicKeyRetrieval=true
        username: root
        password: 123456
      db1:
        type: com.zaxxer.hikari.HikariDataSource
        driver-class-name: com.mysql.cj.jdbc.Driver
        jdbc-url: jdbc:mysql://127.0.0.1:3306/shardingsphere-db0?useUnicode=true&characterEncoding=utf-8&useSSL=false&serverTimezone=Asia/Shanghai&allowPublicKeyRetrieval=true
        username: root
        password: 123456
    # 分片规则配置
    rules:
      sharding:
        # 分片算法配置
        sharding-algorithms:
          database-inline:
            # 分片算法类型
            type: INLINE
            props:
              # 分片算法的行表达式（算法自行定义，此处为方便演示效果）
              algorithm-expression: db$->{order_id > 4?1:0}
          table-inline:
            # 分片算法类型
            type: INLINE
            props:
              # 分片算法的行表达式
              algorithm-expression: t_order_$->{order_id % 4}
        tables:
          # 逻辑表名称
          t_order:
            # 行表达式标识符可以使用 ${...} 或 $->{...}，但前者与 Spring 本身的属性文件占位符冲突，因此在 Spring 环境中使用行表达式标识符建议使用 $->{...}
            actual-data-nodes: db${0..1}.t_order_${0..3}
            # 分库策略
            database-strategy:
              standard:
                # 分片列名称
                sharding-column: order_id
                # 分片算法名称
                sharding-algorithm-name: database-inline
            # 分表策略
            table-strategy:
              standard:
                # 分片列名称
                sharding-column: order_id
                # 分片算法名称
                sharding-algorithm-name: table-inline
    # 属性配置
    props:
      # 展示修改以后的sql语句
      sql-show: true
```
以下是 shardingsphere 多数据源信息的配置，其中的 names 表示需要连接的数据库别名列表，每添加一个数据库名就需要新增一份对应的数据库连接配置。
```yaml
spring:
  shardingsphere:
    # 数据源配置
    datasource:
      # 数据源名称，多数据源以逗号分隔
      names: db0,db1
      db0:
        type: com.zaxxer.hikari.HikariDataSource
        driver-class-name: com.mysql.cj.jdbc.Driver
        jdbc-url: jdbc:mysql://127.0.0.1:3306/shardingsphere-db1?useUnicode=true&characterEncoding=utf-8&useSSL=false&serverTimezone=Asia/Shanghai&allowPublicKeyRetrieval=true
        username: root
        password: 123456
      db1:
        type: com.zaxxer.hikari.HikariDataSource
        driver-class-name: com.mysql.cj.jdbc.Driver
        jdbc-url: jdbc:mysql://127.0.0.1:3306/shardingsphere-db0?useUnicode=true&characterEncoding=utf-8&useSSL=false&serverTimezone=Asia/Shanghai&allowPublicKeyRetrieval=true
        username: root
        password: 123456
```
rules节点下为分片规则的配置，sharding-algorithms 节点为自定义的分片算法模块，分片算法可以在后边配置表的分片规则时被引用，其中：

- database-inline：自定义的分片算法名称；
- type：该分片算法的类型，这里先以 inline 为例，后续会有详细章节介绍；
- props：指定该分片算法的具体内容，其中 algorithm-expression 是该分片算法的表达式，即根据分片键值计算出要访问的真实数据库名或表名，。

db$->{order_id % 2} 这种为 Groovy 语言表达式，表示对分片键 order_id 进行取模，根据取模结果计算出db0、db1，分表的表达式同理。
```yaml
spring:
  shardingsphere:
    # 规则配置
    rules:
      sharding:
        # 分片算法配置
        sharding-algorithms:
          database-inline:
            # 分片算法类型
            type: INLINE
            props:
              # 分片算法的行表达式（算法自行定义，此处为方便演示效果）
              algorithm-expression: db$->{order_id % 2}
          table-inline:
            # 分片算法类型
            type: INLINE
            props:
              # 分片算法的行表达式
              algorithm-expression: t_order_$->{order_id % 3}
```
tables节点定义了逻辑表名t_order的分库分表规则。actual-data-nodes 用于设置物理数据节点的数量。<br />db${0..1}.t_order_${0..3} 表达式意思此逻辑表在不同数据库实例中的分布情况，如果只想单纯的分库或者分表，可以调整表达式，分库db${0..1}、分表t_order_${0..3}。
```
db0
├── t_order_0
├── t_order_1
└── t_order_2
db1
├── t_order_0
├── t_order_1
└── t_order_2
```
```yaml
spring:
  shardingsphere:
    # 规则配置
    rules:
      sharding:
        tables:
          # 逻辑表名称
          t_order:
            # 行表达式标识符可以使用 ${...} 或 $->{...}，但前者与 Spring 本身的属性文件占位符冲突，因此在 Spring 环境中使用行表达式标识符建议使用 $->{...}
            actual-data-nodes: db${0..1}.t_order_${0..3}
            # 分库策略
            database-strategy:
              standard:
                # 分片列名称
                sharding-column: order_id
                # 分片算法名称
                sharding-algorithm-name: database-inline
            # 分表策略
            table-strategy:
              standard:
                # 分片列名称
                sharding-column: order_id
                # 分片算法名称
                sharding-algorithm-name: table-inline
```
database-strategy 和 table-strategy分别设置了分库和分表策略；<br />sharding-column表示根据表的哪个列（分片键）进行计算分片路由到哪个库、表中；<br />sharding-algorithm-name 表示使用哪种分片算法对分片键进行运算处理，这里可以引用刚才自定义的分片算法名称使用。<br />props节点用于设置其他的属性配置，比如：`sql-show`表示是否在控制台输出解析改造后真实执行的 SQL语句以便进行调试。
```yaml
spring:
  shardingsphere:
    # 属性配置
    props:
      # 展示修改以后的sql语句
      sql-show: true
```
跑个单测在向数据库中插入 10 条数据时，发现数据已经相对均匀地插入到了各个分片中。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288682742-c649fbc6-62fc-4531-b417-30059375310e.png#averageHue=%23f6f5f3&clientId=u8712b1b7-88f2-4&from=paste&id=u9e2b26db&originHeight=927&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub1312255-8134-490b-9e3e-1aee49bb878&title=)
<a name="TzXNT"></a>
### JAVA 编码
如果不想通过 yml 配置文件实现自动装配，也可以使用 ShardingSphere 的 API 实现相同的功能。使用 API 完成分片规则和数据源的配置，优势在于更加灵活、可定制性强的特点，方便进行二次开发和扩展。<br />下边是纯JAVA编码方式实现分库分表的完整代码。
```java
@Configuration
public class ShardingConfiguration {

    /**
     * 配置分片数据源
     */
    @Bean
    public DataSource getShardingDataSource() throws SQLException {
        Map<String, DataSource> dataSourceMap = new HashMap<>();
        dataSourceMap.put("db0", dataSource1());
        dataSourceMap.put("db1", dataSource2());

        // 分片rules规则配置
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();
        shardingRuleConfig.setShardingAlgorithms(getShardingAlgorithms());

        // 配置 t_order 表分片规则
        ShardingTableRuleConfiguration orderTableRuleConfig = new ShardingTableRuleConfiguration("t_order", "db${0..1}.t_order_${0..2}");
        orderTableRuleConfig.setTableShardingStrategy(new StandardShardingStrategyConfiguration("order_id", "table-inline"));
        orderTableRuleConfig.setDatabaseShardingStrategy(new StandardShardingStrategyConfiguration("order_id", "database-inline"));
        shardingRuleConfig.getTables().add(orderTableRuleConfig);

        // 是否在控制台输出解析改造后真实执行的 SQL
        Properties properties = new Properties();
        properties.setProperty("sql-show", "true");
        // 创建 ShardingSphere 数据源
        return ShardingSphereDataSourceFactory.createDataSource(dataSourceMap, Collections.singleton(shardingRuleConfig), properties);
    }

    /**
     * 配置数据源1
     */
    public DataSource dataSource1() {
        HikariDataSource dataSource = new HikariDataSource();
        dataSource.setDriverClassName("com.mysql.cj.jdbc.Driver");
        dataSource.setJdbcUrl("jdbc:mysql://127.0.0.1:3306/shardingsphere-db1?useUnicode=true&characterEncoding=utf-8&useSSL=false&serverTimezone=Asia/Shanghai&allowPublicKeyRetrieval=true");
        dataSource.setUsername("root");
        dataSource.setPassword("123456");
        return dataSource;
    }

    /**
     * 配置数据源2
     */
    public DataSource dataSource2() {
        HikariDataSource dataSource = new HikariDataSource();
        dataSource.setDriverClassName("com.mysql.cj.jdbc.Driver");
        dataSource.setJdbcUrl("jdbc:mysql://127.0.0.1:3306/shardingsphere-db0?useUnicode=true&characterEncoding=utf-8&useSSL=false&serverTimezone=Asia/Shanghai&allowPublicKeyRetrieval=true");
        dataSource.setUsername("root");
        dataSource.setPassword("123456");
        return dataSource;
    }

    /**
     * 配置分片算法
     */
    private Map<String, AlgorithmConfiguration> getShardingAlgorithms() {
        Map<String, AlgorithmConfiguration> shardingAlgorithms = new LinkedHashMap<>();

        // 自定义分库算法
        Properties databaseAlgorithms = new Properties();
        databaseAlgorithms.setProperty("algorithm-expression", "db$->{order_id % 2}");
        shardingAlgorithms.put("database-inline", new AlgorithmConfiguration("INLINE", databaseAlgorithms));

        // 自定义分表算法
        Properties tableAlgorithms = new Properties();
        tableAlgorithms.setProperty("algorithm-expression", "t_order_$->{order_id % 3}");
        shardingAlgorithms.put("table-inline", new AlgorithmConfiguration("INLINE", tableAlgorithms));

                               return shardingAlgorithms;
                               }
                               }
```
ShardingSphere 的分片核心配置类 `ShardingRuleConfiguration`，它主要用来加载分片规则、分片算法、主键生成规则、绑定表、广播表等核心配置。将相关的配置信息 set到配置类，并通过`createDataSource`创建并覆盖 DataSource，最后注入Bean。<br />使用Java编码方式只是将 ShardingSphere 预知的加载配置逻辑自己手动实现了一遍，两种实现方式比较下来，还是推荐使用YML配置方式来实现 ShardingSphere的分库分表功能，相比于Java编码，YML配置更加直观和易于理解，开发者可以更加专注于业务逻辑的实现，而不需要过多关注底层技术细节。
```java
@Getter
@Setter
public final class ShardingRuleConfiguration implements DatabaseRuleConfiguration, DistributedRuleConfiguration {

    // 分表配置配置
    private Collection<ShardingTableRuleConfiguration> tables = new LinkedList<>();
    // 自动分片规则配置
    private Collection<ShardingAutoTableRuleConfiguration> autoTables = new LinkedList<>();
    // 绑定表配置
    private Collection<String> bindingTableGroups = new LinkedList<>();
    // 广播表配置
    private Collection<String> broadcastTables = new LinkedList<>();
    // 默认的分库策略配置
    private ShardingStrategyConfiguration defaultDatabaseShardingStrategy;
    // 默认的分表策略配置
    private ShardingStrategyConfiguration defaultTableShardingStrategy;
    // 主键生成策略配置
    private KeyGenerateStrategyConfiguration defaultKeyGenerateStrategy;

    private ShardingAuditStrategyConfiguration defaultAuditStrategy;
    // 默认的分片键
    private String defaultShardingColumn;
    // 自定义的分片算法
    private Map<String, AlgorithmConfiguration> shardingAlgorithms = new LinkedHashMap<>();
    // 主键生成算法
    private Map<String, AlgorithmConfiguration> keyGenerators = new LinkedHashMap<>();

    private Map<String, AlgorithmConfiguration> auditors = new LinkedHashMap<>();
}
```
经过查看控制台打印的真实 SQL日志，发现在使用 ShardingSphere 进行数据插入时，其内部实现会先根据分片键 order_id 查询记录是否存在。如果记录不存在，则执行插入操作；如果记录已存在，则进行更新操作。看似只会执行10条插入SQL，但实际上需要执行20条SQL语句，多少会对数据库的性能产生一定的影响。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288682704-b54b4542-0c04-466e-93e5-d58458c2a438.png#averageHue=%233c3b3b&clientId=u8712b1b7-88f2-4&from=paste&id=ue2547fd2&originHeight=457&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u12064b3d-4f2c-4b54-9140-9a9754d24c5&title=)<br />功能挺简单的，但由于**不同版本的 ShardingSphere 的 API 变化较大**，网上类似的资料太不靠谱，最后还是扒了扒看了源码完成的。
<a name="GwCru"></a>
## 默认数据源
可能有些小伙伴会有疑问，对于已经设置了分片规则的t_order表可以正常操作数据，如果t_user表没有配置分库分表规则，那么在执行插入操作时会发生什么呢？<br />仔细看了下官方的技术文档，其实已经回答了这个问题，如果只有部分数据库分库分表，是否需要将不分库分表的表也配置在分片规则中？官方回答：**不需要**。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288682708-1ad32475-a0f2-444b-8d2e-6db7af1c77ac.png#averageHue=%23fefdfd&clientId=u8712b1b7-88f2-4&from=paste&id=u3998fc92&originHeight=163&originWidth=757&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u01f6b99b-78f1-4d61-88f5-8060ac65b66&title=)<br />创建一张t_user表，并且不对其进行任何分片规则的配置。在印象中没有通过设置 default-data-source-name 默认的数据源，操作未分片的表应该会报错的！<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288682739-67440364-5e4d-42d8-812f-5196b401da58.png#averageHue=%23806131&clientId=u8712b1b7-88f2-4&from=paste&id=ue51cc0f6&originHeight=414&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udc5c71f5-aa89-413d-b6f9-52c2c40d821&title=)<br />向t_user尝试插入一条数据，结果居然成功了？翻了翻库表发现数据只被插在了 db1 库里，说明没有走广播路由。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688288683141-09f6ffaf-f71d-4b46-ba40-cafe2c5e897a.png#averageHue=%23deeadd&clientId=u8712b1b7-88f2-4&from=paste&id=ue4e3c66e&originHeight=478&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0d6f7b01-4ac6-487b-8ba8-e6f1d748641&title=)<br />shardingsphere-jdbc 5.x版本移除了原本的默认数据源配置，自动使用了默认数据源的规则，为验证多增加了数据源，尝试性的调整了db2、db0、db1的顺序，再次插入数据，这回记录被插在了 db2 库，反复试验初步得出结论。<br />**未分片的表默认会使用第一个数据源作为默认数据源，也就是 datasource.names 第一个。**
```yaml
spring:
  shardingsphere:
    # 数据源配置
    datasource:
      # 数据源名称，多数据源以逗号分隔
      names: db2 , db1 , db0
```
<a name="r47hH"></a>
## 总结
本期对 shardingsphere 做了简单的介绍，并使用 yml 和 Java编码的方式快速实现了分库分表功能。
