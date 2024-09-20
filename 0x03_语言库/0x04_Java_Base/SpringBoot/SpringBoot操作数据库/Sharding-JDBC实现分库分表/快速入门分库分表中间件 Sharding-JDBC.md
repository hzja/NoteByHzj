分库分表 Sharding-JDBC
<a name="l7q7Q"></a>
## 一、Sharding-JDBC 简介
Sharding-JDBC 最早是当当网内部使用的一款分库分表框架，到2017年的时候才开始对外开源，这几年在大量社区贡献者的不断迭代下，功能也逐渐完善，现已更名为 ShardingSphere，2020年4⽉16日正式成为 Apache 软件基金会的顶级项目。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688001417066-b1659f35-645e-4de5-b1a5-def3c01fbc19.png#averageHue=%23ebe4d4&clientId=u313dde3f-f88c-4&from=paste&id=u80278b55&originHeight=591&originWidth=597&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u306c5ae9-5899-475a-824e-2bfe4e3e25b&title=)<br />随着版本的不断更迭 ShardingSphere 的核心功能也变得多元化起来。从最开始 Sharding-JDBC 1.0 版本只有数据分片，到 Sharding-JDBC 2.0 版本开始支持数据库治理（注册中心、配置中心等等），再到 Sharding-JDBC 3.0版本又加分布式事务 （支持 Atomikos、Narayana、Bitronix、Seata），如今已经迭代到了 Sharding-JDBC 4.0 版本。<br />![2021-09-28-07-37-58-783813.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786428327-c692bf14-965f-4691-b622-1fb4aca99f41.png#averageHue=%23fef9f3&clientId=u3500ff80-a48a-4&from=ui&id=uabed1d20&originHeight=474&originWidth=542&originalType=binary&ratio=1&rotation=0&showTitle=false&size=32245&status=done&style=shadow&taskId=u6359678c-e4db-4c83-bcea-6236d320672&title=)<br />现在的 ShardingSphere 不单单是指某个框架而是一个生态圈，这个生态圈 Sharding-JDBC、Sharding-Proxy 和 Sharding-Sidecar 这三款开源的分布式数据库中间件解决方案所构成。<br />ShardingSphere 的前身就是 Sharding-JDBC，所以它是整个框架中最为经典、成熟的组件，先从 Sharding-JDBC 框架入手学习分库分表。
<a name="DobY0"></a>
## 二、核心概念
在开始 Sharding-JDBC分库分表具体实战之前，有必要先了解分库分表的一些核心概念。
<a name="zMMZj"></a>
### 分片
一般在提到分库分表的时候，大多是以水平切分模式（水平分库、分表）为基础来说的，数据分片将原本一张数据量较大的表 t_order 拆分生成数个表结构完全一致的小数据量表 t_order_0、t_order_1、···、t_order_n，每张表只存储原大表中的一部分数据，当执行一条SQL时会通过 分库策略、分片策略 将数据分散到不同的数据库、表内。<br />![2021-09-28-07-37-58-899801.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786428333-73e44930-36c6-4c57-8727-08624ed0e8ce.png#averageHue=%23fdf8f3&clientId=u3500ff80-a48a-4&from=ui&id=vJo0L&originHeight=459&originWidth=661&originalType=binary&ratio=1&rotation=0&showTitle=false&size=28310&status=done&style=shadow&taskId=ub2391e7d-84d5-496d-8cb5-89f17fa0fa5&title=)
<a name="ssIKl"></a>
### 数据节点
数据节点是分库分表中一个不可再分的最小数据单元（表），它由数据源名称和数据表组成，例如上图中 order_db_1.t_order_0、order_db_2.t_order_1 就表示一个数据节点。
<a name="u53HC"></a>
### 逻辑表
逻辑表是指一组具有相同逻辑和数据结构表的总称。比如将订单表t_order 拆分成 t_order_0 ···  t_order_9 等 10张表。此时会发现分库分表以后数据库中已不在有 t_order 这张表，取而代之的是 t_order_n，但在代码中写 SQL 依然按 t_order 来写。此时 t_order 就是这些拆分表的逻辑表。
<a name="lJVRl"></a>
### 真实表
真实表也就是上边提到的 t_order_n 数据库中真实存在的物理表。
<a name="QLNcs"></a>
### 分片键
用于分片的数据库字段。将 t_order 表分片以后，当执行一条SQL时，通过对字段 order_id 取模的方式来决定，这条数据该在哪个数据库中的哪个表中执行，此时 order_id 字段就是 t_order 表的分片健。<br />![2021-09-28-07-37-58-979802.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786428332-ab083321-8219-4103-8269-7780515415c0.png#averageHue=%23fdf9f6&clientId=u3500ff80-a48a-4&from=ui&id=pOOID&originHeight=303&originWidth=559&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14228&status=done&style=shadow&taskId=uce89ee88-4b25-4723-b96f-1c1f24cad0b&title=)<br />这样以来同一个订单的相关数据就会存在同一个数据库表中，大幅提升数据检索的性能，不仅如此 sharding-jdbc 还支持根据多个字段作为分片健进行分片。
<a name="kt1cx"></a>
### 分片算法
上边提到可以用分片健取模的规则分片，但这只是比较简单的一种，在实际开发中还希望用 >=、<=、>、<、BETWEEN 和 IN 等条件作为分片规则，自定义分片逻辑，这时就需要用到分片策略与分片算法。<br />从执行 SQL 的角度来看，分库分表可以看作是一种路由机制，把 SQL 语句路由到期望的数据库或数据表中并获取数据，分片算法可以理解成一种路由规则。<br />先捋一下它们之间的关系，分片策略只是抽象出的概念，它是由分片算法和分片健组合而成，分片算法做具体的数据分片逻辑。<br />分库、分表的分片策略配置是相对独立的，可以各自使用不同的策略与算法，每种策略中可以是多个分片算法的组合，每个分片算法可以对多个分片健做逻辑判断。<br />![分片算法和分片策略的关系](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786461891-81e2d38a-fd7b-4856-89b9-f935718bfbb4.png#averageHue=%23d6e5f9&clientId=u3500ff80-a48a-4&from=ui&id=u674b6df6&originHeight=171&originWidth=361&originalType=binary&ratio=1&rotation=0&showTitle=true&size=3149&status=done&style=shadow&taskId=u61bdeda1-9ac5-4ad8-8080-ea4850f6cb7&title=%E5%88%86%E7%89%87%E7%AE%97%E6%B3%95%E5%92%8C%E5%88%86%E7%89%87%E7%AD%96%E7%95%A5%E7%9A%84%E5%85%B3%E7%B3%BB "分片算法和分片策略的关系")<br />**注意**：sharding-jdbc 并没有直接提供分片算法的实现，需要开发者根据业务自行实现。<br />sharding-jdbc 提供了4种分片算法：
<a name="qBAWU"></a>
#### 1、精确分片算法
精确分片算法（`PreciseShardingAlgorithm`）用于单个字段作为分片键，SQL中有 = 与 IN 等条件的分片，需要在标准分片策略（`StandardShardingStrategy` ）下使用。
<a name="nJBpH"></a>
#### 2、范围分片算法
范围分片算法（`RangeShardingAlgorithm`）用于单个字段作为分片键，SQL中有 BETWEEN AND、>、<、>=、<=  等条件的分片，需要在标准分片策略（`StandardShardingStrategy` ）下使用。
<a name="Bbi95"></a>
#### 3、复合分片算法
复合分片算法（`ComplexKeysShardingAlgorithm`）用于多个字段作为分片键的分片操作，同时获取到多个分片健的值，根据多个字段处理业务逻辑。需要在复合分片策略（`ComplexShardingStrategy` ）下使用。
<a name="Rvr2k"></a>
#### 4、Hint分片算法
Hint分片算法（`HintShardingAlgorithm`）稍有不同，上边的算法中都是解析SQL 语句提取分片键，并设置分片策略进行分片。但有些时候并没有使用任何的分片键和分片策略，可还想将 SQL 路由到目标数据库和表，就需要通过手动干预指定SQL的目标数据库和表信息，这也叫强制路由。
<a name="yuKei"></a>
### 分片策略
上边讲分片算法的时候已经说过，分片策略是一种抽象的概念，实际分片操作的是由分片算法和分片健来完成的。
<a name="shCgF"></a>
#### 1、标准分片策略
标准分片策略适用于单分片键，此策略支持 `PreciseShardingAlgorithm` 和 `RangeShardingAlgorithm` 两个分片算法。<br />其中 `PreciseShardingAlgorithm` 是必选的，用于处理 = 和 IN 的分片。`RangeShardingAlgorithm` 是可选的，用于处理BETWEEN AND， >， <，>=，<= 条件分片，如果不配置`RangeShardingAlgorithm`，SQL中的条件等将按照全库路由处理。
<a name="WA6bT"></a>
#### 2、复合分片策略
复合分片策略，同样支持对 SQL语句中的 =，>， <， >=， <=，IN和 BETWEEN AND 的分片操作。不同的是它支持多分片键，具体分配片细节完全由应用开发者实现。
<a name="G2QZa"></a>
#### 3、行表达式分片策略
行表达式分片策略，支持对 SQL语句中的 = 和 IN 的分片操作，但只支持单分片键。这种策略通常用于简单的分片，不需要自定义分片算法，可以直接在配置文件中接着写规则。<br />t_order_$->{t_order_id % 4} 代表 t_order 对其字段 t_order_id取模，拆分成4张表，而表名分别是t_order_0 到 t_order_3。
<a name="pQHH1"></a>
#### 4、Hint分片策略
Hint分片策略，对应上边的Hint分片算法，通过指定分片健而非从 SQL中提取分片健的方式进行分片的策略。
<a name="h23uo"></a>
### 分布式主键
数据分片后，不同数据节点生成全局唯⼀主键是非常棘手的问题，同一个逻辑表（t_order）内的不同真实表（t_order_n）之间的自增键由于无法互相感知而产生重复主键。<br />尽管可通过设置自增主键 初始值 和 步长 的方式避免ID碰撞，但这样会使维护成本加大，乏完整性和可扩展性。如果后去需要增加分片表的数量，要逐一修改分片表的步长，运维成本非常高，所以不建议这种方式。<br />为了让上手更加简单，ApacheShardingSphere 内置了UUID、SNOWFLAKE 两种分布式主键生成器，默认使用雪花算法（snowflake）生成64bit的长整型数据。不仅如此它还抽离出分布式主键生成器的接口，方便实现自定义的自增主键生成算法。
<a name="hz8tT"></a>
### 广播表
广播表：存在于所有的分片数据源中的表，表结构和表中的数据在每个数据库中均完全一致。一般是为字典表或者配置表 t_config，某个表一旦被配置为广播表，只要修改某个数据库的广播表，所有数据源中广播表的数据都会跟着同步。
<a name="wilAl"></a>
### 绑定表
绑定表：那些分片规则一致的主表和子表。比如：t_order 订单表和 t_order_item 订单服务项目表，都是按 order_id 字段分片，因此两张表互为绑定表关系。<br />那绑定表存在的意义是啥呢？<br />通常在业务中都会使用 t_order 和 t_order_item 等表进行多表联合查询，但由于分库分表以后这些表被拆分成N多个子表。如果不配置绑定表关系，会出现笛卡尔积关联查询，将产生如下四条SQL。
```sql
SELECT * FROM t_order_0 o JOIN t_order_item_0 i ON o.order_id=i.order_id 
SELECT * FROM t_order_0 o JOIN t_order_item_1 i ON o.order_id=i.order_id 
SELECT * FROM t_order_1 o JOIN t_order_item_0 i ON o.order_id=i.order_id 
SELECT * FROM t_order_1 o JOIN t_order_item_1 i ON o.order_id=i.order_id 
```
![笛卡尔积查询](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786461903-6774764d-0b73-4384-ac3c-5a48528f803e.png#averageHue=%23f6efea&clientId=u3500ff80-a48a-4&from=ui&id=grPpj&originHeight=141&originWidth=290&originalType=binary&ratio=1&rotation=0&showTitle=true&size=8878&status=done&style=shadow&taskId=u33a7c758-d09a-4e20-acd8-7ee0dfb4f3f&title=%E7%AC%9B%E5%8D%A1%E5%B0%94%E7%A7%AF%E6%9F%A5%E8%AF%A2 "笛卡尔积查询")<br />而配置绑定表关系后再进行关联查询时，只要对应表分片规则一致产生的数据就会落到同一个库中，那么只需 t_order_0 和 t_order_item_0 表关联即可。
```sql
SELECT * FROM t_order_0 o JOIN t_order_item_0 i ON o.order_id=i.order_id 
SELECT * FROM t_order_1 o JOIN t_order_item_1 i ON o.order_id=i.order_id 
```
![绑定表关系](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786461917-fc76d0fc-22da-4015-883c-19b711c825e1.png#averageHue=%23f8f1eb&clientId=u3500ff80-a48a-4&from=ui&id=PE8QT&originHeight=141&originWidth=290&originalType=binary&ratio=1&rotation=0&showTitle=true&size=2945&status=done&style=shadow&taskId=uc09b6f42-a8cf-4ac4-91aa-b638386d426&title=%E7%BB%91%E5%AE%9A%E8%A1%A8%E5%85%B3%E7%B3%BB "绑定表关系")<br />**注意**：在关联查询时 t_order 它作为整个联合查询的主表。所有相关的路由计算都只使用主表的策略，t_order_item 表的分片相关的计算也会使用 t_order 的条件，所以要保证绑定表之间的分片键要完全相同。
<a name="yF2wO"></a>
## 三、和JDBC的猫腻
从名字上不难看出，Sharding-JDBC 和 JDBC有很大关系，JDBC 是一种 Java 语言访问关系型数据库的规范，其设计初衷就是要提供一套用于各种数据库的统一标准，不同厂家共同遵守这套标准，并提供各自的实现方案供应用程序调用。<br />![2021-09-28-07-37-59-378800.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786498935-8d9fae4e-c984-459b-b0af-5e16533cd51a.png#averageHue=%23fdf7f4&clientId=u3500ff80-a48a-4&from=ui&id=u235806f3&originHeight=464&originWidth=682&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20473&status=done&style=shadow&taskId=u843b49f2-b99f-45d4-9d7b-916594daa01&title=)<br />但其实对于开发人员而言，只关心如何调用 JDBC API 来访问数据库，只要正确使用 DataSource、Connection、Statement 、ResultSet 等 API 接口，直接操作数据库即可。所以如果想在 JDBC 层面实现数据分片就必须对现有的 API 进行功能拓展，而 Sharding-JDBC 正是基于这种思想，重写了 JDBC 规范并完全兼容了 JDBC 规范。<br />![JDBC流程](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786498941-5fb241b3-ebbf-4d00-9e13-b30c40e05a48.png#averageHue=%23fefbf8&clientId=u3500ff80-a48a-4&from=ui&id=JxusT&originHeight=415&originWidth=644&originalType=binary&ratio=1&rotation=0&showTitle=true&size=17193&status=done&style=shadow&taskId=u622c9800-cefb-4318-8cf7-8e571825bc3&title=JDBC%E6%B5%81%E7%A8%8B "JDBC流程")<br />对原有的 DataSource、Connection 等接口扩展成 `ShardingDataSource`、`ShardingConnection`，而对外暴露的分片操作接口与 JDBC 规范中所提供的接口完全一致，只要熟悉 JDBC 就可以轻松应用 Sharding-JDBC 来实现分库分表。<br />![2021-09-28-07-37-59-656799.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786498992-2345721b-cb7a-49e4-9f7f-96a7c5e9970b.png#averageHue=%23f8f6ef&clientId=u3500ff80-a48a-4&from=ui&id=nl1zR&originHeight=244&originWidth=621&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19431&status=done&style=shadow&taskId=u10fd65f6-de4b-414b-8987-62367ab5905&title=)<br />因此它适用于任何基于 JDBC 的 ORM 框架，如：JPA， Hibernate，Mybatis，Spring JDBC Template 或直接使用的 JDBC。完美兼容任何第三方的数据库连接池，如：DBCP， C3P0， BoneCP，Druid， HikariCP 等，几乎对主流关系型数据库都支持。<br />**那 Sharding-JDBC 又是如何拓展这些接口的呢**？想知道答案就的从源码入手了，下边以 JDBC API  中的 DataSource 为例看看它是如何被重写扩展的。<br />数据源 DataSource 接口的核心作用就是获取数据库连接对象 Connection，其内部提供了两个获取数据库连接的方法 ，并且继承了 CommonDataSource 和 Wrapper 两个接口。
```java
public interface DataSource  extends CommonDataSource, Wrapper {

  /**
   * <p>Attempts to establish a connection with the data source that
   * this {@code DataSource} object represents.
   * @return  a connection to the data source
   */
  Connection getConnection() throws SQLException;

  /**
   * <p>Attempts to establish a connection with the data source that
   * this {@code DataSource} object represents.
   * @param username the database user on whose behalf the connection is
   *  being made
   * @param password the user's password
   */
  Connection getConnection(String username, String password)
    throws SQLException;
}
```
其中 CommonDataSource 是定义数据源的根接口这很好理解，而 Wrapper 接口则是拓展 JDBC 分片功能的关键。<br />由于数据库厂商的不同，他们可能会各自提供一些超越标准 JDBC API 的扩展功能，但这些功能非 JDBC 标准并不能直接使用，而 Wrapper 接口的作用就是把一个由第三方供应商提供的、非 JDBC 标准的接口包装成标准接口，也就是适配器模式。<br />既然讲到了适配器模式就多啰嗦几句，也方便后边的理解。<br />适配器模式个种比较常用的设计模式，它的作用是将某个类的接口转换成客户端期望的另一个接口，使原本因接口不匹配（或者不兼容）而无法在一起工作的两个类能够在一起工作。比如用耳机听音乐，有个圆头的耳机，可手机插孔却是扁口的，如果想要使用耳机听音乐就必须借助一个转接头才可以，这个转接头就起到了适配作用。举个栗子：假如 Target 接口中有 `hello()` 和 `word()` 两个方法。
```java
public interface Target {

    void hello();

    void world();
}
```
可由于接口版本迭代`Target` 接口的 `word()` 方法可能会被废弃掉或不被支持，`Adaptee`  类的 `greet()`方法将代替`hello()` 方法。
```java
public class Adaptee {

    public void greet(){

    }
    public void world(){

    }
}
```
但此时旧版本仍然有大量 `word()` 方法被使用中，解决此事最好的办法就是创建一个适配器`Adapter`，这样就适配了 Target 类，解决了接口升级带来的兼容性问题。
```java
public class Adapter extends Adaptee implements Target {

    @Override
    public void world() {
        
    }

    @Override
    public void hello() {
        super.greet();
    }

    @Override
    public void greet() {
        
    }
}
```
而 Sharding-JDBC 提供的正是非 JDBC 标准的接口，所以它也提供了类似的实现方案，也使用到了 Wrapper 接口做数据分片功能的适配。除了 DataSource 之外，Connection、Statement、ResultSet 等核心对象也都继承了这个接口。<br />下面通过 `ShardingDataSource`  类源码简单看下实现过程，下图是继承关系流程图。<br />![ShardingDataSource实现流程](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786536717-183547d9-b447-4c65-a7ad-4bd471ea5936.png#averageHue=%23fef8f0&clientId=u3500ff80-a48a-4&from=ui&id=u66df753a&originHeight=484&originWidth=588&originalType=binary&ratio=1&rotation=0&showTitle=true&size=39136&status=done&style=shadow&taskId=u8d215ce1-8f7c-45a1-9714-b33ac6f0a9b&title=ShardingDataSource%E5%AE%9E%E7%8E%B0%E6%B5%81%E7%A8%8B "ShardingDataSource实现流程")<br />`ShardingDataSource`  类它在原 DataSource 基础上做了功能拓展，初始化时注册了分片SQL路由包装器、SQL重写上下文和结果集处理引擎，还对数据源类型做了校验，因为它要同时支持多个不同类型的数据源。到这好像也没看出如何适配，那接着向上看 `ShardingDataSource` 的继承类  `AbstractDataSourceAdapter `。
```java
@Getter
public class ShardingDataSource extends AbstractDataSourceAdapter {
    
    private final ShardingRuntimeContext runtimeContext;

    /**
     * 注册路由、SQl重写上下文、结果集处理引擎
     */
    static {
        NewInstanceServiceLoader.register(RouteDecorator.class);
        NewInstanceServiceLoader.register(SQLRewriteContextDecorator.class);
        NewInstanceServiceLoader.register(ResultProcessEngine.class);
    }

    /**
     * 初始化时校验数据源类型 并根据数据源 map、分片规则、数据库类型得到一个分片上下文，用来获取数据库连接
     */
    public ShardingDataSource(final Map<String, DataSource> dataSourceMap, final ShardingRule shardingRule, final Properties props) throws SQLException {
        super(dataSourceMap);
        checkDataSourceType(dataSourceMap);
        runtimeContext = new ShardingRuntimeContext(dataSourceMap, shardingRule, props, getDatabaseType());
    }

    private void checkDataSourceType(final Map<String, DataSource> dataSourceMap) {
        for (DataSource each : dataSourceMap.values()) {
            Preconditions.checkArgument(!(each instanceof MasterSlaveDataSource), "Initialized data sources can not be master-slave data sources.");
        }
    }

    /**
     * 数据库连接
     */
    @Override
    public final ShardingConnection getConnection() {
        return new ShardingConnection(getDataSourceMap(), runtimeContext, TransactionTypeHolder.get());
    }
}
```
`AbstractDataSourceAdapter` 抽象类内部主要获取不同类型的数据源对应的数据库连接对象，实现 `AutoCloseable` 接口是为在使用完资源后可以自动将这些资源关闭（调用 close方法），那再看看继承类 `AbstractUnsupportedOperationDataSource` 。
```java
@Getter
public abstract class AbstractDataSourceAdapter extends AbstractUnsupportedOperationDataSource implements AutoCloseable {
    
    private final Map<String, DataSource> dataSourceMap;
    
    private final DatabaseType databaseType;
    
    public AbstractDataSourceAdapter(final Map<String, DataSource> dataSourceMap) throws SQLException {
        this.dataSourceMap = dataSourceMap;
        databaseType = createDatabaseType();
    }
    
    public AbstractDataSourceAdapter(final DataSource dataSource) throws SQLException {
        dataSourceMap = new HashMap<>(1, 1);
        dataSourceMap.put("unique", dataSource);
        databaseType = createDatabaseType();
    }
    
    private DatabaseType createDatabaseType() throws SQLException {
        DatabaseType result = null;
        for (DataSource each : dataSourceMap.values()) {
            DatabaseType databaseType = createDatabaseType(each);
            Preconditions.checkState(null == result || result == databaseType, String.format("Database type inconsistent with '%s' and '%s'", result, databaseType));
            result = databaseType;
        }
        return result;
    }
    
    /**
     * 不同数据源类型获取数据库连接
     */
    private DatabaseType createDatabaseType(final DataSource dataSource) throws SQLException {
        if (dataSource instanceof AbstractDataSourceAdapter) {
            return ((AbstractDataSourceAdapter) dataSource).databaseType;
        }
        try (Connection connection = dataSource.getConnection()) {
            return DatabaseTypes.getDatabaseTypeByURL(connection.getMetaData().getURL());
        }
    }
    
    @Override
    public final Connection getConnection(final String username, final String password) throws SQLException {
        return getConnection();
    }
    
    @Override
    public final void close() throws Exception {
        close(dataSourceMap.keySet());
    }
}
```
`AbstractUnsupportedOperationDataSource` 实现DataSource 接口并继承了 `WrapperAdapter` 类，它内部并没有什么具体方法只起到桥接的作用，但看着是不是和前边讲适配器模式的例子方式有点相似。
```java
public abstract class AbstractUnsupportedOperationDataSource extends WrapperAdapter implements DataSource {
    
    @Override
    public final int getLoginTimeout() throws SQLException {
        throw new SQLFeatureNotSupportedException("unsupported getLoginTimeout()");
    }
    
    @Override
    public final void setLoginTimeout(final int seconds) throws SQLException {
        throw new SQLFeatureNotSupportedException("unsupported setLoginTimeout(int seconds)");
    }
}
```
`WrapperAdapter` 是一个包装器的适配类，实现了 JDBC 中的 `Wrapper` 接口，其中有两个核心方法 `recordMethodInvocation` 用于添加需要执行的方法和参数，而 `replayMethodsInvocation` 则将添加的这些方法和参数通过反射执行。仔细看不难发现两个方法中都用到了 `JdbcMethodInvocation`类。
```java
public abstract class WrapperAdapter implements Wrapper {
    
    private final Collection<JdbcMethodInvocation> jdbcMethodInvocations = new ArrayList<>();
 
    /**
     * 添加要执行的方法
     */
    @SneakyThrows
    public final void recordMethodInvocation(final Class<?> targetClass, final String methodName, final Class<?>[] argumentTypes, final Object[] arguments) {
        jdbcMethodInvocations.add(new JdbcMethodInvocation(targetClass.getMethod(methodName, argumentTypes), arguments));
    }
    
    /**
     * 通过反射执行 上边添加的方法
     */
    public final void replayMethodsInvocation(final Object target) {
        for (JdbcMethodInvocation each : jdbcMethodInvocations) {
            each.invoke(target);
        }
    }
}
```
`JdbcMethodInvocation` 类主要应用反射通过传入的 method 方法和 arguments 参数执行对应的方法，这样就可以通过 JDBC API 调用非 JDBC 方法了。
```java
@RequiredArgsConstructor
public class JdbcMethodInvocation {
    
    @Getter
    private final Method method;
    
    @Getter
    private final Object[] arguments;
    
    /**
     * Invoke JDBC method.
     * 
     * @param target target object
     */
    @SneakyThrows
    public void invoke(final Object target) {
        method.invoke(target, arguments);
    }
}
```
那 Sharding-JDBC 拓展 JDBC API 接口后，在新增的分片功能里又做了哪些事情呢？<br />一张表经过分库分表后被拆分成多个子表，并分散到不同的数据库中，在不修改原业务 SQL 的前提下，Sharding-JDBC 就必须对 SQL进行一些改造才能正常执行。<br />大致的执行流程：SQL 解析 -> 执行器优化 -> SQL 路由 -> SQL 改写 -> SQL 执行 -> 结果归并 六步组成，一起瞅瞅每个步骤做了点什么。<br />![2021-09-28-07-37-59-976802.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786576380-205738ed-caca-4d2e-8e85-fd1cdc3736ff.png#averageHue=%23fdf7f6&clientId=u3500ff80-a48a-4&from=ui&id=u95072c3e&originHeight=644&originWidth=580&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23664&status=done&style=shadow&taskId=u250e35bd-a182-4bac-a510-e18cb4513a4&title=)
<a name="kFHks"></a>
### SQL 解析
SQL解析过程分为词法解析和语法解析两步，比如下边这条查询用户订单的SQL，先用词法解析将SQL拆解成不可再分的原子单元。在根据不同数据库方言所提供的字典，将这些单元归类为关键字，表达式，变量或者操作符等类型。
```sql
SELECT order_no,price FROM t_order_ where user_id = 10086 and order_status > 0
```
接着语法解析会将拆分后的SQL转换为抽象语法树，通过对抽象语法树遍历，提炼出分片所需的上下文，上下文包含查询字段信息（Field）、表信息（Table）、查询条件（Condition）、排序信息（Order By）、分组信息（Group By）以及分页信息（Limit）等，并标记出 SQL中有可能需要改写的位置。<br />![抽象语法树](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786576384-c75b174f-6e5e-4a22-9251-18975aaf001d.png#averageHue=%23fdf4ec&clientId=u3500ff80-a48a-4&from=ui&id=t803o&originHeight=369&originWidth=718&originalType=binary&ratio=1&rotation=0&showTitle=true&size=22651&status=done&style=shadow&taskId=u4e0d093d-2ee9-43be-9048-2da20ef9744&title=%E6%8A%BD%E8%B1%A1%E8%AF%AD%E6%B3%95%E6%A0%91 "抽象语法树")
<a name="gMRQx"></a>
### 执行器优化
执行器优化对SQL分片条件进行优化，处理像关键字 OR这种影响性能的坏味道。
<a name="EQV2R"></a>
### SQL 路由
SQL 路由通过解析分片上下文，匹配到用户配置的分片策略，并生成路由路径。简单点理解就是可以根据配置的分片策略计算出 SQL该在哪个库的哪个表中执行，而SQL路由又根据有无分片健区分出 分片路由 和 广播路由。<br />![官方路由图谱](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786600628-af23d0b8-231f-4f50-832f-b234cd20d531.png#averageHue=%23f9f9f8&clientId=u3500ff80-a48a-4&from=ui&id=u5244d2c3&originHeight=775&originWidth=1046&originalType=binary&ratio=1&rotation=0&showTitle=true&size=121438&status=done&style=shadow&taskId=u06db3c7b-a75d-43ce-8de6-46ecd719ab4&title=%E5%AE%98%E6%96%B9%E8%B7%AF%E7%94%B1%E5%9B%BE%E8%B0%B1 "官方路由图谱")<br />有分片键的路由叫分片路由，细分为直接路由、标准路由和笛卡尔积路由这3种类型。
<a name="mQKPC"></a>
#### 标准路由
标准路由是最推荐也是最为常用的分片方式，它的适用范围是不包含关联查询或仅包含绑定表之间关联查询的SQL。<br />当 SQL分片健的运算符为 = 时，路由结果将落入单库（表），当分片运算符是BETWEEN 或IN 等范围时，路由结果则不⼀定落入唯一的库（表），因此一条逻辑SQL最终可能被拆分为多条用于执行的真实SQL。
```sql
SELECT * FROM t_order  where t_order_id in (1,2)
```
SQL路由处理后
```sql
SELECT * FROM t_order_0  where t_order_id in (1,2)
SELECT * FROM t_order_1  where t_order_id in (1,2)
```
<a name="Czmbm"></a>
#### 直接路由
直接路由是通过使用 HintAPI 直接将 SQL路由到指定至库表的一种分片方式，而且直接路由可以用于分片键不在SQL中的场景，还可以执行包括子查询、自定义函数等复杂情况的任意SQL。<br />比如根据 t_order_id 字段为条件查询订单，此时希望在不修改SQL的前提下，加上 user_id作为分片条件就可以使用直接路由。
<a name="Jh7iJ"></a>
#### 笛卡尔积路由
笛卡尔路由是由非绑定表之间的关联查询产生的，查询性能较低尽量避免走此路由模式。

---

无分片键的路由又叫做广播路由，可以划分为全库表路由、全库路由、 全实例路由、单播路由和阻断路由这 5种类型。
<a name="GYstr"></a>
#### 全库表路由
全库表路由针对的是数据库 DQL和 DML，以及 DDL等操作，当执行一条逻辑表 t_order SQL时，在所有分片库中对应的真实表 t_order_0 ···  t_order_n 内逐一执行。
<a name="Aorvp"></a>
#### 全库路由
全库路由主要是对数据库层面的操作，比如数据库 SET 类型的数据库管理命令，以及 TCL 这样的事务控制语句。<br />对逻辑库设置 autocommit 属性后，所有对应的真实库中都执行该命令。
```sql
SET autocommit=0;
```
<a name="Nissi"></a>
#### 全实例路由
全实例路由是针对数据库实例的 DCL 操作（设置或更改数据库用户或角色权限），比如：创建一个用户 order ，这个命令将在所有的真实库实例中执行，以此确保 order 用户可以正常访问每一个数据库实例。
```sql
CREATE USER order@127.0.0.1 identified BY 'Fcant';
```
<a name="BrcKa"></a>
#### 单播路由
单播路由用来获取某一真实表信息，比如获得表的描述信息：
```sql
DESCRIBE t_order; 
```
t_order 的真实表是 t_order_0 ···· t_order_n，他们的描述结构相完全同，只需在任意的真实表执行一次就可以。
<a name="I7JNY"></a>
#### 阻断路由
用来屏蔽SQL对数据库的操作，例如：
```sql
USE order_db;
```
这个命令不会在真实数据库中执行，因为 ShardingSphere 采用的是逻辑 Schema（数据库的组织和结构） 方式，所以无需将切换数据库的命令发送至真实数据库中。
<a name="AUyLp"></a>
### SQL 改写
将基于逻辑表开发的SQL改写成可以在真实数据库中可以正确执行的语句。比如查询 t_order 订单表，实际开发中 SQL是按逻辑表 t_order 写的。
```sql
SELECT * FROM t_order
```
但分库分表以后真实数据库中 t_order 表就不存在了，而是被拆分成多个子表 t_order_n 分散在不同的数据库内，还按原SQL执行显然是行不通的，这时需要将分表配置中的逻辑表名称改写为路由之后所获取的真实表名称。
```sql
SELECT * FROM t_order_n
```
<a name="CJM9v"></a>
### SQL执行
将路由和改写后的真实 SQL 安全且高效发送到底层数据源执行。但这个过程并不是简单的将 SQL 通过JDBC 直接发送至数据源执行，而是平衡数据源连接创建以及内存占用所产生的消耗，它会自动化的平衡资源控制与执行效率。
<a name="iIf82"></a>
### 结果归并
将从各个数据节点获取的多数据结果集，合并成一个大的结果集并正确的返回至请求客户端，称为结果归并。而SQL中的排序、分组、分页和聚合等语法，均是在归并后的结果集上进行操作的。
<a name="yZ4Bo"></a>
## 四、快速实践
下面结合 Springboot + mybatisplus 快速搭建一个分库分表案例。
<a name="dD8rL"></a>
### 1、准备工作
先做准备工作，创建两个数据库 ds-0、ds-1，两个库中分别建表 t_order_0、t_order_1、t_order_2 、t_order_item_0、t_order_item_1、t_order_item_2，t_config，方便后边验证广播表、绑定表的场景。<br />![2021-09-28-07-38-00-475046.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786600574-a3796154-ad8b-4793-b9a8-e15650b99e9c.png#averageHue=%23f9f7f7&clientId=u3500ff80-a48a-4&from=ui&id=xl2j0&originHeight=375&originWidth=700&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25795&status=done&style=shadow&taskId=uc422d720-dd68-4707-ac30-ca58d3550d4&title=)<br />表结构如下：<br />t_order_0 订单表
```sql
CREATE TABLE `t_order_0` (
  `order_id` bigint(200) NOT NULL,
  `order_no` varchar(100) DEFAULT NULL,
  `create_name` varchar(50) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`order_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
```
t_order_0 与 t_order_item_0  互为关联表
```sql
CREATE TABLE `t_order_item_0` (
  `item_id` bigint(100) NOT NULL,
  `order_no` varchar(200) NOT NULL,
  `item_name` varchar(50) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
```
广播表 t_config
```sql
CREATE TABLE `t_config` (
  `id` bigint(30) NOT NULL,
  `remark` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_modify_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
```
ShardingSphere 提供了4种分片配置方式：

- Java 代码配置
- Yaml 、properties 配置
- Spring 命名空间配置
- Spring Boot配置

为让代码看上去更简洁和直观，后边统一使用 properties 配置的方式，引入 shardingsphere 对应的 sharding-jdbc-spring-boot-starter 和 sharding-core-common 包，版本统一用的 4.0.0-RC1。
<a name="e8ig9"></a>
### 2、分片配置
```xml
<dependency>
  <groupId>org.apache.shardingsphere</groupId>
  <artifactId>sharding-jdbc-spring-boot-starter</artifactId>
  <version>4.0.0-RC1</version>
</dependency>

<dependency>
  <groupId>org.apache.shardingsphere</groupId>
  <artifactId>sharding-core-common</artifactId>
  <version>4.0.0-RC1</version>
</dependency>
```
准备工作做完（ mybatis 搭建就不赘述了），接下来逐一解读分片配置信息。<br />首先定义两个数据源 ds-0、ds-1，并分别加上数据源的基础信息。
```
# 定义两个全局数据源
spring.shardingsphere.datasource.names=ds-0,ds-1

# 配置数据源 ds-0
spring.shardingsphere.datasource.ds-0.type=com.alibaba.druid.pool.DruidDataSource
spring.shardingsphere.datasource.ds-0.driverClassName=com.mysql.jdbc.Driver
spring.shardingsphere.datasource.ds-0.url=jdbc:mysql://127.0.0.1:3306/ds-0?useUnicode=true&characterEncoding=utf8&tinyInt1isBit=false&useSSL=false&serverTimezone=GMT
spring.shardingsphere.datasource.ds-0.username=root
spring.shardingsphere.datasource.ds-0.password=root

# 配置数据源 ds-1
spring.shardingsphere.datasource.ds-1.type=com.alibaba.druid.pool.DruidDataSource
spring.shardingsphere.datasource.ds-1.driverClassName=com.mysql.jdbc.Driver
spring.shardingsphere.datasource.ds-1.url=jdbc:mysql://127.0.0.1:3306/ds-1?useUnicode=true&characterEncoding=utf8&tinyInt1isBit=false&useSSL=false&serverTimezone=GMT
spring.shardingsphere.datasource.ds-1.username=root
spring.shardingsphere.datasource.ds-1.password=root
```
配置完数据源接下来为表添加分库和分表策略，使用 sharding-jdbc 做分库分表需要为每一个表单独设置分片规则。
```
# 配置分片表 t_order
# 指定真实数据节点
spring.shardingsphere.sharding.tables.t_order.actual-data-nodes=ds-$->{0..1}.t_order_$->{0..2}
```
actual-data-nodes 属性指定分片的真实数据节点，`$`是一个占位符，{0..1}表示实际拆分的数据库表数量。<br />ds-$->{0..1}.t_order_$->{0..2}  表达式相当于 6个数据节点

- ds-0.t_order_0
- ds-0.t_order_1
- ds-0.t_order_2
- ds-1.t_order_0
- ds-1.t_order_1
- ds-1.t_order_2
```
### 分库策略
# 分库分片健
spring.shardingsphere.sharding.tables.t_order.database-strategy.inline.sharding-column=order_id
# 分库分片算法
spring.shardingsphere.sharding.tables.t_order.database-strategy.inline.algorithm-expression=ds-$->{order_id % 2}
```
为表设置分库策略，上边讲了 sharding-jdbc 它提供了四种分片策略，为快速搭建先以最简单的行内表达式分片策略来实现，在下一篇会介绍四种分片策略的详细用法和使用场景。<br />`database-strategy.inline.sharding-column`  属性中 database-strategy  为分库策略，inline  为具体的分片策略，sharding-column  代表分片健。<br />`database-strategy.inline.algorithm-expression` 是当前策略下具体的分片算法，ds-$->{order_id % 2} 表达式意思是 对 order_id字段进行取模分库，2 代表分片库的个数，不同的策略对应不同的算法，这里也可以是自定义的分片算法类。
```
# 分表策略
# 分表分片健
spring.shardingsphere.sharding.tables.t_order.table-strategy.inline.sharding-column=order_id
# 分表算法
spring.shardingsphere.sharding.tables.t_order.table-strategy.inline.algorithm-expression=t_order_$->{order_id % 3}
# 自增主键字段
spring.shardingsphere.sharding.tables.t_order.key-generator.column=order_id
# 自增主键ID 生成方案
spring.shardingsphere.sharding.tables.t_order.key-generator.type=SNOWFLAKE
```
分表策略 和 分库策略 的配置比较相似，不同的是分表可以通过 key-generator.column 和 key-generator.type 设置自增主键以及指定自增主键的生成方案，目前内置了SNOWFLAKE 和 UUID 两种方式，还能自定义的主键生成算法类，后续会详细的讲解。
```
# 绑定表关系
spring.shardingsphere.sharding.binding-tables= t_order,t_order_item
```
必须按相同分片健进行分片的表才能互为成绑定表，在联合查询时就能避免出现笛卡尔积查询。
```
# 配置广播表
spring.shardingsphere.sharding.broadcast-tables=t_config
```
广播表，开启 SQL解析日志，能清晰的看到 SQL分片解析的过程
```
# 是否开启 SQL解析日志
spring.shardingsphere.props.sql.show=true
```
<a name="emyXK"></a>
### 3、验证分片
分片配置完以后无需在修改业务代码了，直接执行业务逻辑的增、删、改、查即可，接下来验证一下分片的效果。<br />同时向 t_order、t_order_item 表插入 5条订单记录，并不给定主键 order_id ，item_id 字段值。
```java
public String insertOrder() {

   for (int i = 0; i < 4; i++) {
       TOrder order = new TOrder();
       order.setOrderNo("A000" + i);
       order.setCreateName("订单 " + i);
       order.setPrice(new BigDecimal("" + i));
       orderRepository.insert(order);

       TOrderItem orderItem = new TOrderItem();
       orderItem.setOrderId(order.getOrderId());
       orderItem.setOrderNo("A000" + i);
       orderItem.setItemName("服务项目" + i);
       orderItem.setPrice(new BigDecimal("" + i));
       orderItemRepository.insert(orderItem);
   }
   return "success";
}
```
看到订单记录被成功分散到了不同的库表中， order_id 字段也自动生成了主键ID，基础的分片功能就完成了。<br />![基础分片](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786600611-c36b0d7c-60c6-4891-8db2-42aa4d38370c.png#averageHue=%23f1eae8&clientId=u3500ff80-a48a-4&from=ui&id=Di6u2&originHeight=500&originWidth=603&originalType=binary&ratio=1&rotation=0&showTitle=true&size=41340&status=done&style=shadow&taskId=uf94a741d-ab24-4be8-80e4-e554587a036&title=%E5%9F%BA%E7%A1%80%E5%88%86%E7%89%87 "基础分片")<br />那向广播表 t_config 中插入一条数据会是什么效果呢？
```java
public String config() {

    TConfig tConfig = new TConfig();
    tConfig.setRemark("我是广播表");
    tConfig.setCreateTime(new Date());
    tConfig.setLastModifyTime(new Date());
    configRepository.insert(tConfig);
    return "success";
}
```
发现所有库中 t_config 表都执行了这条SQL，广播表和 MQ广播订阅的模式很相似，所有订阅的客户端都会收到同一条消息。<br />![广播表](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786641746-62e445b4-4969-47ca-a7b1-b01be82ad4de.png#averageHue=%23f4efed&clientId=u3500ff80-a48a-4&from=ui&id=u2d88e4ab&originHeight=339&originWidth=843&originalType=binary&ratio=1&rotation=0&showTitle=true&size=28834&status=done&style=shadow&taskId=u6c6cc1d6-72d6-4505-8596-81f351a121e&title=%E5%B9%BF%E6%92%AD%E8%A1%A8 "广播表")<br />简单SQL操作验证没问通，接下来在试试复杂一点的联合查询，前边已经把 t_order 、t_order_item 表设为绑定表，直接联表查询执行一下。<br />![关联查询](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786641797-3718c789-4987-4a9a-9971-b72662975132.png#averageHue=%23f3e8b9&clientId=u3500ff80-a48a-4&from=ui&id=zCaEG&originHeight=284&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=85732&status=done&style=shadow&taskId=u8ed3c1c8-2919-412c-b166-7cc7251e4bf&title=%E5%85%B3%E8%81%94%E6%9F%A5%E8%AF%A2 "关联查询")<br />通过控制台日志发现，逻辑表SQL 经过解析以后，只对  t_order_0 和 t_order_item_0 表进行了关联产生一条SQL。<br />![绑定表SQL](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786641876-76dd07b6-f778-45a6-85f5-42f3ca43a09a.png#averageHue=%23faf1ef&clientId=u3500ff80-a48a-4&from=ui&id=pQQA2&originHeight=213&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=132984&status=done&style=shadow&taskId=u3a6d55fe-bbc8-4dc6-9622-6d206ecf3c4&title=%E7%BB%91%E5%AE%9A%E8%A1%A8SQL "绑定表SQL")<br />那如果不互为绑定表又会是什么情况呢？去掉 `spring.shardingsphere.sharding.binding-tables`试一下。<br />发现控制台解析出了 3条真实表SQL，而去掉 order_id 作为查询条件再次执行后，结果解析出了 9条SQL，进行了笛卡尔积查询。所以相比之下绑定表的优点就不言而喻了。<br />![笛卡尔积查询](https://cdn.nlark.com/yuque/0/2021/png/396745/1632786641843-411b56b7-c98c-4f9d-a0b0-fc05f03c3d3e.png#averageHue=%23f8eeeb&clientId=u3500ff80-a48a-4&from=ui&id=l855B&originHeight=358&originWidth=1078&originalType=binary&ratio=1&rotation=0&showTitle=true&size=56567&status=done&style=shadow&taskId=ub7b84e90-d848-4aee-96c2-4355e25ba4b&title=%E7%AC%9B%E5%8D%A1%E5%B0%94%E7%A7%AF%E6%9F%A5%E8%AF%A2 "笛卡尔积查询")
<a name="jSwyu"></a>
## 五、总结
以上对分库分表中间件 sharding-jdbc 的基础概念做了简单梳理，快速的搭建了一个分库分表案例，但这只是实践分库分表的第一步，还有四种分片策略的具体用法和使用场景（必知必会），自定义分布式主键、分布式数据库事务、分布式服务治理，数据脱敏等。
