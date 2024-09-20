2022.8月，ShardingSphere当前最新版本是 5.1.2，相对于版本4变化挺大的，代码基本重构了，扩展了很多新功能，整个系统的扩展性更强了，不过核心原理没有变。<br />如果直接去看官方文档，快速上手还是有点难度的，本文基于 5.1.2 版，通过案例介绍其用法，引导大家如何使用，顺便会介绍如何去学习其他功能的用法。
<a name="UvRzH"></a>
## 5.1.2版Git地址
[https://github.com/apache/shardingsphere/tree/5.1.2](https://github.com/apache/shardingsphere/tree/5.1.2)
<a name="pjrnz"></a>
## 官方文档
Git首页有文档入口，如下，强烈建议阅读：[点击直达](https://shardingsphere.apache.org/document/current/cn/overview/)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697987484514-4e39309c-f773-47e2-bca9-d0a90c8d8593.png#averageHue=%23fcfcfb&clientId=u6f30bbd4-335f-4&from=paste&height=164&id=u766bb1be&originHeight=411&originWidth=1955&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=122703&status=done&style=none&taskId=u710ec75e-0caf-460c-9eff-b1e0efbf373&title=&width=782)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697987541660-e1006abb-4d8e-4c9f-8ada-8ed1c3918e90.png#averageHue=%23d6bc97&clientId=u6f30bbd4-335f-4&from=paste&height=418&id=uc5613eef&originHeight=1046&originWidth=2145&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=354924&status=done&style=none&taskId=u1fde3965-eee8-4750-9790-1c650ce8767&title=&width=858)
<a name="Zq1Gm"></a>
## 案例
<a name="IIIzz"></a>
### 1）需求
ds_shardingsphere5 库中有2个用户表：t_user_0，t_user_1，表中有2个字段（id：用户编号，name：姓名）<br />这2个表的分片规则：

- t_user_0：存放id为偶数的用户数据
- t_user_1：存放id为奇数的用户数据

下面来看具体实现
<a name="UhAxL"></a>
### 2）执行sql脚本
```sql
drop database if exists ds_shardingsphere5;
create database ds_shardingsphere5;
use ds_shardingsphere5;

drop table if exists t_user_0;
create table t_user_0(
  id bigint not null primary key,
  name varchar(128) not null
);

drop table if exists t_user_1;
create table t_user_1(
  id bigint not null primary key,
  name varchar(128) not null
);
```
<a name="b5vEl"></a>
### 3）项目中引入Maven配置
```xml
<dependency>
  <groupId>org.apache.shardingsphere</groupId>
  <artifactId>shardingsphere-jdbc-core</artifactId>
  <version>5.1.2</version>
</dependency>
```
<a name="mN2Y5"></a>
### 4）创建测试类
代码如下，有些地方可能大家看不懂，不过没关系，稍后会介绍。
```java
public class Test1 {

    static DataSource dataSource;

    //@BeforeAll 标注的方法会在  @Test 标注的方法被执行前执行一次
    @BeforeAll
    public static void init() throws SQLException {
        /**
         * 1、创建datasource
         */
        Map<String, DataSource> dataSourceMap = createDataSources();

        /**
         * 2、构建具体规则
         */
        Collection<RuleConfiguration> ruleConfigs = new ArrayList<>();

        /**
         * 2.1、创建分片规则
         */
        ShardingRuleConfiguration shardingRuleConfiguration = new ShardingRuleConfiguration();

        //2.2、创建t_user表的分片规则
        ShardingTableRuleConfiguration userShardingTableRuleConfiguration =
        new ShardingTableRuleConfiguration("t_user", //逻辑表
                                           "ds1.t_user_$->{0..1}"); //实际数据节点
        //2.3、配置t_user的分表规则，分片字段是：user_id,分片算法是：userShardingAlgorithm（这个算法名称是自定义的，后面会定义这个名称对应的具体算法）
        userShardingTableRuleConfiguration.setTableShardingStrategy(
            new StandardShardingStrategyConfiguration(
                "id",
                "userShardingAlgorithm"));
        //2.4、将t_user表的分片规则加入到shardingRuleConfiguration中
        shardingRuleConfiguration.getTables().add(userShardingTableRuleConfiguration);

        //2.5、定义分片
        Properties userShardingAlgorithmProperties = new Properties();
        userShardingAlgorithmProperties.put(
            "algorithm-expression",
            "t_user_$->{id%2}"
        );
        ShardingSphereAlgorithmConfiguration userShardingAlgorithm =
        new ShardingSphereAlgorithmConfiguration("INLINE",
                                                 userShardingAlgorithmProperties);

        //2.6、将定义好的 userShardingAlgorithm 算法加入到算法列表中（算法名称->算法）
        shardingRuleConfiguration.getShardingAlgorithms().
        put("userShardingAlgorithm", userShardingAlgorithm);

        //2.7、将分片规则加入规则列表
        ruleConfigs.add(shardingRuleConfiguration);

        /**
         * 3、建属性配置
         */
        Properties props = new Properties();
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), "true");

        /**
         * 4、创建数据源
         */
        dataSource = ShardingSphereDataSourceFactory.createDataSource(
            "shardingsphere-demo-db",
            null,
            dataSourceMap,
            ruleConfigs,
            props);
    }


    private static Map<String, DataSource> createDataSources() {
        Map<String, DataSource> dataSourceMap = new HashMap<>();
        // 配置第 1 个数据源
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/ds_shardingsphere5?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        dataSourceMap.put("ds1", dataSource1);
        return dataSourceMap;
    }

}
```
下面上测试案例，所有测试案例对应的方法都放到上面这个测试类中。
<a name="H3kgX"></a>
### 5）测试：插入
执行下面测试方法之前，先执行下上面的的sql脚本，清理下数据。<br />下面代码将插入2条数据，按照路由规则，会被路由到不同的用户表，这些信息都会包含在运行结果中，注意看运行结果
```java
@Test
public void m1() throws Exception {
    String sql = "insert into t_user values (?,?),(?,?)";
    try (Connection connection = dataSource.getConnection();) {
        connection.setAutoCommit(false);
        PreparedStatement ps = connection.prepareStatement(sql);

        int parameterIndex = 1;
        ps.setInt(parameterIndex++, 1);
        ps.setString(parameterIndex++, "user-1");
        ps.setInt(parameterIndex++, 2);
        ps.setString(parameterIndex++, "user-2");

        ps.executeUpdate();
        connection.commit();
    }
}
```
运行输出，如下，id为1的被路由到了t_user_1中，id为2的被路由到了t_user_0中
```
Logic SQL: insert into t_user values (?,?),(?,?)
Actual SQL: ds1 ::: insert into t_user_1 values (?, ?) ::: [1, user-1]
Actual SQL: ds1 ::: insert into t_user_0 values (?, ?) ::: [2, user-2]
```
<a name="VA5CF"></a>
### 6）测试：查询id为1的用户
```java
@Test
public void m2() throws Exception {
    String sql = "select id,name from t_user where id = 1";
    try (Connection connection = dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);) {
        ResultSet rs = ps.executeQuery();
        while (rs.next()) {
            long id = rs.getLong("id");
            String name = rs.getString("name");
            System.out.println(String.format("id:%s, name:%s", id, name));
        }
    }
}
```
运行输出，如下，id为1的被路由到了t_user_1表
```
Logic SQL: select id,name from t_user where id = 1
Actual SQL: ds1 ::: select id,name from t_user_1 where id = 1
id:1, name:user-1
```
<a name="ICYtZ"></a>
## 案例重点代码解释
<a name="rLDxX"></a>
### 1、init方法
大家估计对案例中的`init()`方法有很多疑问，这里把代码再贴出来，如下
```java
@BeforeAll
public static void init() throws SQLException {
    /**
     * 1、创建datasource
     */
    Map<String, DataSource> dataSourceMap = createDataSources();

    /**
     * 2、构建具体规则
     */
    Collection<RuleConfiguration> ruleConfigs = new ArrayList<>();

    /**
     * 2.1、创建分片规则
     */
    ShardingRuleConfiguration shardingRuleConfiguration = new ShardingRuleConfiguration();

    //2.2、创建t_user表的分片规则
    ShardingTableRuleConfiguration userShardingTableRuleConfiguration =
            new ShardingTableRuleConfiguration("t_user", //逻辑表
                    "ds1.t_user_$->{0..1}"); //实际数据节点
    //2.3、配置t_user的分表规则，分片字段是：user_id,分片算法是：userShardingAlgorithm（这个算法名称是自定义的，后面会定义这个名称对应的具体算法）
    userShardingTableRuleConfiguration.setTableShardingStrategy(
            new StandardShardingStrategyConfiguration(
                    "id",
                    "userShardingAlgorithm"));
    //2.4、将t_user表的分片规则加入到shardingRuleConfiguration中
    shardingRuleConfiguration.getTables().add(userShardingTableRuleConfiguration);

    //2.5、定义分片
    Properties userShardingAlgorithmProperties = new Properties();
    userShardingAlgorithmProperties.put(
            "algorithm-expression",
            "t_user_$->{id%2}"
    );
    ShardingSphereAlgorithmConfiguration userShardingAlgorithm =
            new ShardingSphereAlgorithmConfiguration("INLINE",
                    userShardingAlgorithmProperties);

    //2.6、将定义好的 userShardingAlgorithm 算法加入到算法列表中（算法名称->算法）
    shardingRuleConfiguration.getShardingAlgorithms().
            put("userShardingAlgorithm", userShardingAlgorithm);

    //2.7、将分片规则加入规则列表
    ruleConfigs.add(shardingRuleConfiguration);

    /**
     * 3、建属性配置
     */
    Properties props = new Properties();
    props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), "true");

    /**
     * 4、创建数据源
     */
    dataSource = ShardingSphereDataSourceFactory.createDataSource(
            "shardingsphere-demo-db",
            null,
            dataSourceMap,
            ruleConfigs,
            props);
}
```
<a name="SgwEE"></a>
### 2、通过分片算法名称关联分片算法
如下，配置用户表分片规则的时候，指定的算法名称为`userShardingAlgorithm`，这个名称可以随便定义，但是这个名称对应的具体算法需要在箭头指向的位置定义。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697987638975-d93507a0-9a08-47b5-97d1-4c2ca05197f9.png#averageHue=%23fcfbfa&clientId=u6f30bbd4-335f-4&from=paste&height=396&id=u1900af14&originHeight=991&originWidth=2164&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=728353&status=done&style=none&taskId=u2162eecd-c749-4ae5-8cb6-863c70b7acd&title=&width=865.6)
<a name="IZ7vJ"></a>
### 3、定义具体的分片算法
如下图，红框中用于定义用户表具体的分表算法<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697987839081-30381c58-47da-4bed-a651-44609864f31c.png#averageHue=%23fbfaf8&clientId=u6f30bbd4-335f-4&from=paste&height=373&id=u83e20228&originHeight=933&originWidth=2090&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=706252&status=done&style=none&taskId=u5fc64dda-35e6-4e14-900f-ebbf7e6e34e&title=&width=836)
<a name="GcL4j"></a>
#### 1）问题1：上图中具体用的是什么算法呢？
用的是`org.apache.shardingsphere.sharding.algorithm.sharding.inline.InlineShardingAlgorithm`这个算法类
<a name="jSBBP"></a>
#### 2）问题2：红圈中的 INLINE 字符串是什么意思？
INLINE 并不是乱写的，系统会通过INLINE找到其对应的算法类，即`InlineShardingAlgorithm`这算法。
<a name="Q8Q1S"></a>
#### 3）问题3：系统是如何通过INLINE找到`InlineShardingAlgorithm`的？
通过SPI机制找到的，SPI简单来说：通过SPI提供的方法，可以在程序中找到某个接口的所有实现类，并且会将这些实现类都进行实例化，返回返回给调用者。<br />SPI工作机制：

- 1、调用者给spi传入一个接口
- 2、SPI会在当前项目中以及所有的jar中找/META-INF/services/接口完整名称这些文件，这些文件中定义了这个接口具体的实现类列表。
- 3、读取这些文件，得到实现类列表，然后通过反射将这些类实例化，返回给调用者
<a name="DcW8z"></a>
#### 4）问题4：INLINE 这个名称是在哪里定义的呢？
这要说一下shardingsphere中的spi机制了，shardingsphere中有个spi接口：`org.apache.shardingsphere.spi.type.typed.TypedSPI`，源码如下
```java
public interface TypedSPI {

    /**
     * Get type.
     * 
     * @return type
     */
    default String getType() {
        return "";
    }

    /**
     * Get type aliases.
     *
     * @return type aliases
     */
    default Collection<String> getTypeAliases() {
        return Collections.emptyList();
    }
}
```
shardingsphere所有需要使用SPI功能的接口都必须继承上面这个接口，实现类需要实现2个方法了，而使用方可以通过type或者aliases（别名）找到具体的实现类了。<br />INLINE对应`org.apache.shardingsphere.sharding.algorithm.sharding.inline.InlineShardingAlgorithm`类，这个类实现了TypedSPI接口，uml图如下<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697987894208-f95aea10-87ed-4b14-91a8-6bbb793063fe.png#averageHue=%23f8f7f1&clientId=u6f30bbd4-335f-4&from=paste&height=442&id=u21d6b737&originHeight=1106&originWidth=1269&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=336099&status=done&style=none&taskId=ud7a0065b-26c1-4e30-a8e1-aa89ab59955&title=&width=507.6)<br />其type方法如下，清晰了吧
```java
@Override
public String getType() {
    return "INLINE";
}
```
上面的UML图中还可以看出：`InlineShardingAlgorithm` 还实现了`ShardingAlgorithm` 接口，而`InlineShardingAlgorithm`接口对应的spi文件在shardingsphere-sharding-core-5.1.2.jar/META/service/org.apache.shardingsphere.sharding.spi.ShardingAlgorithm中，如下：这里面列出了shardingsphere提供的所有常见的算法，建议都看一遍<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697987943077-bf6267e0-deed-495b-b830-54b2f49eb369.png#averageHue=%23f0ebcf&clientId=u6f30bbd4-335f-4&from=paste&height=332&id=u4c1f0e7a&originHeight=831&originWidth=2150&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1032922&status=done&style=none&taskId=uc87f0f9a-6bd9-4cb2-b08c-a35a621e4da&title=&width=860)<br />到这里大家都明了了。<br />若要使用其他算法，只需去看`org.apache.shardingsphere.sharding.spi.ShardingAlgorithm`接口实现类，以及其type方法返回值，就可以了，通过type的值便引用到具体的算法，大家自行发挥。
<a name="SGPHg"></a>
#### 5）问题5：SPI得到的实例对象的属性如何设置？
通过spi创建出来的对象，他们内部有很多属性，这些属性的值如何设置呢？<br />比如INLINE对应的`InlineShardingAlgorithm`类，源码如下，其内部有2个属性需要设置，这2个属性是根据init方法传入的Properties来设置的，外部需要传入一个Properteis对象，这个在哪定义的呢？继续向下看<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697987978111-09973267-c938-489a-be5c-8b6c3110cd4e.png#averageHue=%23fcfbfa&clientId=u6f30bbd4-335f-4&from=paste&height=447&id=ud19a19ae&originHeight=1117&originWidth=2155&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=761750&status=done&style=none&taskId=ud54c23f6-c7e6-4875-9657-c8312d64733&title=&width=862)<br />`algorithmExpression`属性是通过`getAlgorithmExpression(props)`实现的，源码如下
```java
private static final String ALGORITHM_EXPRESSION_KEY = "algorithm-expression";

private String getAlgorithmExpression(final Properties props) {
    String expression = props.getProperty(ALGORITHM_EXPRESSION_KEY);
    Preconditions.checkState(null != expression && !expression.isEmpty(), "Inline sharding algorithm expression cannot be null or empty.");
    return InlineExpressionParser.handlePlaceHolder(expression.trim());
}
```
重点在`props.getProperty(ALGORITHM_EXPRESSION_KEY)`这行，而`ALGORITHM_EXPRESSION_KEY`是个常量，其值是algorithm-expression，最终`algorithmExpression`的值来源于`props.getProperty("algorithm-expression")`，这部分信息和测试案例中的代码遥相呼应，如下，红框中的内容就是用来定义`InlineShardingAlgorithm`类中属性的值的，一目了然了吧。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697987995419-1cbd4873-5d54-4b6b-a205-91cadbd65616.png#averageHue=%23faf9f6&clientId=u6f30bbd4-335f-4&from=paste&height=253&id=u2cfcf23c&originHeight=633&originWidth=1725&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=435045&status=done&style=none&taskId=uc2591e75-dec1-40ce-b11a-64a77604ec0&title=&width=690)<br />shardingsphere中spi扩展的类，其属的值都是通过Properties来指定的。
