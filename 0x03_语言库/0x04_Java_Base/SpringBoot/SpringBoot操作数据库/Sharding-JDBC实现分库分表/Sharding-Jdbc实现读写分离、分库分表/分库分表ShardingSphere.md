<a name="VEEWd"></a>
## Java api案例
<a name="NkvEC"></a>
### 准备工作
1）创建一个SpringBoot应用<br />2）引入shardingsphere的Maven配置
```xml
<dependency>
  <groupId>org.apache.shardingsphere</groupId>
  <artifactId>sharding-jdbc-core</artifactId>
  <version>4.1.1</version>
</dependency>
```
3）完整的Maven配置如下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.7.1</version>
    <relativePath/> <!-- lookup parent from repository -->
  </parent>
  <groupId>com.itsoku</groupId>
  <artifactId>sj-demo1</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <name>sj-demo1</name>
  <description>Demo project for Spring Boot</description>
  <properties>
    <java.version>1.8</java.version>
  </properties>
  <dependencies>
    <dependency>
      <groupId>org.mybatis.spring.boot</groupId>
      <artifactId>mybatis-spring-boot-starter</artifactId>
      <version>2.2.2</version>
    </dependency>
    <dependency>
      <groupId>mysql</groupId>
      <artifactId>mysql-connector-java</artifactId>
    </dependency>
    <dependency>
      <groupId>org.projectlombok</groupId>
      <artifactId>lombok</artifactId>
      <optional>true</optional>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-test</artifactId>
      <scope>test</scope>
    </dependency>
    <dependency>
      <groupId>org.apache.shardingsphere</groupId>
      <artifactId>sharding-jdbc-core</artifactId>
      <version>4.1.1</version>
    </dependency>
  </dependencies>

  <build>
    <plugins>
      <plugin>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-maven-plugin</artifactId>
        <configuration>
          <excludes>
            <exclude>
              <groupId>org.projectlombok</groupId>
              <artifactId>lombok</artifactId>
            </exclude>
          </excludes>
        </configuration>
      </plugin>
    </plugins>
  </build>

</project>
```
<a name="FHSPB"></a>
### 案例1：单库多表
<a name="vk7xd"></a>
#### 需求
一个库中有2个订单表，按照订单id取模，将数据路由到指定的表。
<a name="eBiOu"></a>
#### SQL脚本
```sql
drop database if exists sj_ds0;
create database sj_ds0;
use sj_ds0;
drop table if exists t_order_0;
create table t_order_0(
  order_id bigint not null primary key,
  user_id bigint not null,
  price bigint not null
);

drop table if exists t_order_1;
create table t_order_1(
  order_id bigint not null primary key,
  user_id bigint not null,
  price bigint not null
);

drop table if exists t_user;
create table t_user(
  id bigint not null primary key auto_increment,
  name varchar(128) not null
);
```
<a name="a82G0"></a>
#### Java代码
```java
import com.zaxxer.hikari.HikariDataSource;
import org.apache.shardingsphere.api.config.sharding.ShardingRuleConfiguration;
import org.apache.shardingsphere.api.config.sharding.TableRuleConfiguration;
import org.apache.shardingsphere.api.config.sharding.strategy.InlineShardingStrategyConfiguration;
import org.apache.shardingsphere.shardingjdbc.api.ShardingDataSourceFactory;
import org.apache.shardingsphere.underlying.common.config.properties.ConfigurationPropertyKey;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public class Demo1 {
    public static void main(String[] args) throws SQLException {
        /**
         * 1、配置真实数据源
         */
        Map<String, DataSource> dataSourceMap = new HashMap<>();
        dataSourceMap.put("ds0", dataSource1());

        /**
         * 2.配置表的规则
         */
        TableRuleConfiguration orderTableRuleConfig = new TableRuleConfiguration("t_order", "ds0.t_order_$->{0..1}");
        // 指定表的分片策略（分片字段+分片算法）
        orderTableRuleConfig.setTableShardingStrategyConfig(new InlineShardingStrategyConfiguration("order_id", "t_order_$->{order_id % 2}"));

        /**
         * 3、分片规则
         */
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();
        //将表的分片规则加入到分片规则列表
        shardingRuleConfig.getTableRuleConfigs().add(orderTableRuleConfig);

        /**
         * 4、配置一些属性
         */
        Properties props = new Properties();
        //输出sql
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), true);

        /**
         * 5、创建数据源
         */
        DataSource dataSource = ShardingDataSourceFactory.
        createDataSource(dataSourceMap, shardingRuleConfig, props);

        /**
         * 6、获取连接，执行sql
         */
        Connection connection = dataSource.getConnection();
        connection.setAutoCommit(false);

        /**
         * 测试向t_order表插入8条数据，8条数据会分散到2个表
         */
        PreparedStatement ps = connection.prepareStatement("insert into t_order (order_id,user_id,price) values (?,?,?)");
        for (long i = 1; i <= 8; i++) {
            int j = 1;
            ps.setLong(j++, i);
            ps.setLong(j++, i);
            ps.setLong(j, 100 * i);
            System.out.println(ps.executeUpdate());
        }

        connection.commit();
        ps.close();
        connection.close();
    }


    private static DataSource dataSource1() {
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/sj_ds0?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        return dataSource1;
    }

}
```
<a name="X435n"></a>
#### 运行输出
```
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds0 ::: insert into t_order_1 (order_id,user_id,price) values (?, ?, ?) ::: [1, 1, 100]
1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds0 ::: insert into t_order_0 (order_id,user_id,price) values (?, ?, ?) ::: [2, 2, 200]
1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds0 ::: insert into t_order_1 (order_id,user_id,price) values (?, ?, ?) ::: [3, 3, 300]
1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds0 ::: insert into t_order_0 (order_id,user_id,price) values (?, ?, ?) ::: [4, 4, 400]
1
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697970052081-3c0d9c20-1889-49d2-ae8a-8f573196a569.png#averageHue=%23cae5cd&clientId=u523cdd65-a277-4&from=paste&height=284&id=u63f4a4fb&originHeight=711&originWidth=2146&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=497313&status=done&style=none&taskId=u7dc76a7a-1ae1-47ae-b43c-56264c281cd&title=&width=858.4)
<a name="ERpLm"></a>
### 案例2：多库多表
<a name="XgTIG"></a>
#### 需求
2个库：sj_ds0、sj_ds1<br />2个库中都包含2个表：t_order_0，t_order_1<br />根据 user_id%2 路由库，根据 order_id%2路由表。
<a name="jXneE"></a>
#### 执行SQL
```sql
drop database if exists sj_ds0;
create database sj_ds0;
use sj_ds0;
drop table if exists t_order_0;
create table t_order_0(
  order_id bigint not null primary key,
  user_id bigint not null,
  price bigint not null
);

drop table if exists t_order_1;
create table t_order_1(
  order_id bigint not null primary key,
  user_id bigint not null,
  price bigint not null
);

drop database if exists sj_ds1;
create database sj_ds1;
use sj_ds1;
drop table if exists t_order_0;
create table t_order_0(
  order_id bigint not null primary key,
  user_id bigint not null,
  price bigint not null
);

drop table if exists t_order_1;
create table t_order_1(
  order_id bigint not null primary key,
  user_id bigint not null,
  price bigint not null
);
```
<a name="YbaOn"></a>
#### Java代码
```java
import com.zaxxer.hikari.HikariDataSource;
import org.apache.shardingsphere.api.config.sharding.ShardingRuleConfiguration;
import org.apache.shardingsphere.api.config.sharding.TableRuleConfiguration;
import org.apache.shardingsphere.api.config.sharding.strategy.InlineShardingStrategyConfiguration;
import org.apache.shardingsphere.shardingjdbc.api.ShardingDataSourceFactory;
import org.apache.shardingsphere.underlying.common.config.properties.ConfigurationPropertyKey;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public class Demo2 {
    public static void main(String[] args) throws SQLException {
        // 配置真实数据源
        Map<String, DataSource> dataSourceMap = new HashMap<>();
        dataSourceMap.put("ds0", dataSource1());
        dataSourceMap.put("ds1", dataSource2());

        /**
         * 2.配置表的规则
         */
        TableRuleConfiguration orderTableRuleConfig = new TableRuleConfiguration("t_order", "ds$->{0..1}.t_order_$->{0..1}");
        // 指定db的分片策略（分片字段+分片算法）
        orderTableRuleConfig.setDatabaseShardingStrategyConfig(new InlineShardingStrategyConfiguration("user_id", "ds$->{user_id % 2}"));
        // 指定表的分片策略（分片字段+分片算法）
        orderTableRuleConfig.setTableShardingStrategyConfig(new InlineShardingStrategyConfiguration("order_id", "t_order_$->{order_id % 2}"));

        /**
         * 3、分片规则
         */
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();
        //将表的分片规则加入到分片规则列表
        shardingRuleConfig.getTableRuleConfigs().add(orderTableRuleConfig);

        /**
         * 4、配置一些属性
         */
        Properties props = new Properties();
        //输出sql
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), true);

        /**
         * 5、创建数据源
         */
        DataSource dataSource = ShardingDataSourceFactory.
                createDataSource(dataSourceMap, shardingRuleConfig, props);

        /**
         * 6、获取连接，执行sql
         */
        Connection connection = dataSource.getConnection();
        connection.setAutoCommit(false);
        PreparedStatement ps = connection.prepareStatement("insert into t_order (order_id,user_id,price) values (?,?,?)");
        // 插入4条数据测试,每个表会落入1条数据
        for (long user_id = 1; user_id <= 2; user_id++) {
            for (long order_id = 1; order_id <= 2; order_id++) {
                int j = 1;
                ps.setLong(j++, order_id);
                ps.setLong(j++, user_id);
                ps.setLong(j, 100);
                System.out.println(ps.executeUpdate());
            }
        }

        connection.commit();
        ps.close();
        connection.close();
    }


    private static DataSource dataSource1() {
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/sj_ds0?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        return dataSource1;
    }

    private static DataSource dataSource2() {
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/sj_ds1?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        return dataSource1;
    }
}
```
<a name="M3pwL"></a>
#### 3行关键代码
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697959763210-28bb1f9e-3eb3-4be1-9b60-f4b47c5ceadf.png#averageHue=%23fcfbfa&clientId=udd55f3a3-ef66-4&from=paste&height=136&id=ub536a611&originHeight=340&originWidth=2169&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=306344&status=done&style=none&taskId=u46973c13-7ff3-433a-a113-ef167d046d5&title=&width=867.6)
<a name="s4fIG"></a>
#### 运行输出
```
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds1 ::: insert into t_order_1 (order_id,user_id,price) values (?, ?, ?) ::: [1, 1, 100]
1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds1 ::: insert into t_order_0 (order_id,user_id,price) values (?, ?, ?) ::: [2, 1, 100]
1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds0 ::: insert into t_order_1 (order_id,user_id,price) values (?, ?, ?) ::: [1, 2, 100]
1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds0 ::: insert into t_order_0 (order_id,user_id,price) values (?, ?, ?) ::: [2, 2, 100]
1
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697959812532-16fe9756-eed0-4e42-8c91-1fd6f31d0cc3.png#averageHue=%23fcfbfa&clientId=udd55f3a3-ef66-4&from=paste&height=91&id=uabbcfe2c&originHeight=227&originWidth=2158&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=155448&status=done&style=none&taskId=u96903d5e-6ac1-41d2-884d-680f968a9af&title=&width=863.2)
<a name="LqyMm"></a>
### 案例3：单库无分表规则
若表未指定分片规则，则直接路由到对应的表。
<a name="QX2cc"></a>
#### SQL脚本
sj_ds0 库中有 t_user 表
```sql
drop database if exists sj_ds0;
create database sj_ds0;
use sj_ds0;

drop table if exists t_user;
create table t_user(
  id bigint not null primary key auto_increment,
  name varchar(128) not null
);
```
<a name="wzM42"></a>
#### Java代码
```java
import com.zaxxer.hikari.HikariDataSource;
import org.apache.shardingsphere.api.config.sharding.ShardingRuleConfiguration;
import org.apache.shardingsphere.shardingjdbc.api.ShardingDataSourceFactory;
import org.apache.shardingsphere.underlying.common.config.properties.ConfigurationPropertyKey;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public class Demo3 {
    public static void main(String[] args) throws SQLException {
        /**
         * 1.配置真实数据源
         */
        Map<String, DataSource> dataSourceMap = new HashMap<>();
        dataSourceMap.put("ds0", dataSource1());

        /**
         * 2、无分片规则
         */

        /**
         * 3、分片规则
         */
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();

        /**
         * 4、配置一些属性
         */
        Properties props = new Properties();
        //输出sql
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), true);

        /**
         * 5、创建数据源
         */
        DataSource dataSource = ShardingDataSourceFactory.
        createDataSource(dataSourceMap, shardingRuleConfig, props);

        Connection connection = dataSource.getConnection();
        connection.setAutoCommit(false);

        PreparedStatement ps = connection.prepareStatement("insert into t_user (name) values (?)");
        ps.setString(1, "张三");
        System.out.println(ps.executeUpdate());

        connection.commit();
        ps.close();
        connection.close();
    }


    private static DataSource dataSource1() {
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/sj_ds0?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        return dataSource1;
    }


}
```
<a name="z8GFV"></a>
#### 运行输出
```
Logic SQL: insert into t_user (name) values (?)
Actual SQL: ds0 ::: insert into t_user (name) values (?) ::: [张三]
```
<a name="rW548"></a>
### 多库无分表规则
<a name="ASaRv"></a>
#### 需求
2个库：sj_ds0、sj_ds1<br />2个库中都包含表：t_user<br />t_user表不指定路由规则的情况下，向t_user表写入数据会落入哪个库呢？来看下效果
<a name="JGoFr"></a>
#### SQL脚本
```sql
drop database if exists sj_ds0;
create database sj_ds0;
use sj_ds0;

drop table if exists t_user;
create table t_user(
  id bigint not null primary key auto_increment,
  name varchar(128) not null
);

drop database if exists sj_ds1;
create database sj_ds1;
use sj_ds1;

drop table if exists t_user;
create table t_user(
  id bigint not null primary key auto_increment,
  name varchar(128) not null
);
```
<a name="wb0sX"></a>
#### Java代码
下面配置2个数据源，向t_user表插入数据，看看数据会落在哪个库？
```java
import com.zaxxer.hikari.HikariDataSource;
import org.apache.shardingsphere.api.config.sharding.ShardingRuleConfiguration;
import org.apache.shardingsphere.shardingjdbc.api.ShardingDataSourceFactory;
import org.apache.shardingsphere.underlying.common.config.properties.ConfigurationPropertyKey;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Properties;

public class Demo4 {
    public static void main(String[] args) throws SQLException {
        /**
         * 1.配置2个数据源
         */
        Map<String, DataSource> dataSourceMap = new LinkedHashMap<>();
        dataSourceMap.put("ds0", dataSource1());
        dataSourceMap.put("ds1", dataSource2());

        /**
         * 2、无分片规则
         */

        /**
         * 3、分片规则
         */
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();

        /**
         * 4、配置一些属性
         */
        Properties props = new Properties();
        //输出sql
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), true);

        /**
         * 5、创建数据源
         */
        DataSource dataSource = ShardingDataSourceFactory.
        createDataSource(dataSourceMap, shardingRuleConfig, props);

        Connection connection = dataSource.getConnection();
        connection.setAutoCommit(false);

        //插入4条数据，测试效果
        for (int i = 0; i < 4; i++) {
            PreparedStatement ps = connection.prepareStatement("insert into t_user (name) values (?)");
            ps.setString(1, "张三");
            System.out.println(ps.executeUpdate());
        }

        connection.commit();
        connection.close();
    }


    private static DataSource dataSource1() {
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/sj_ds0?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        return dataSource1;
    }

    private static DataSource dataSource2() {
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/sj_ds1?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        return dataSource1;
    }
}
```
<a name="ZYmhU"></a>
#### 运行输出
输出如下，落入的库是不确定的。
```
Logic SQL: insert into t_user (name) values (?)
Actual SQL: ds1 ::: insert into t_user (name) values (?) ::: [张三]
1
Logic SQL: insert into t_user (name) values (?)
Actual SQL: ds1 ::: insert into t_user (name) values (?) ::: [张三]
1
Logic SQL: insert into t_user (name) values (?)
Actual SQL: ds0 ::: insert into t_user (name) values (?) ::: [张三]
1
Logic SQL: insert into t_user (name) values (?)
Actual SQL: ds1 ::: insert into t_user (name) values (?) ::: [张三]
1
```
<a name="RWvGN"></a>
## 分片问题
上面介绍的案例，db的路由、表的路由都是采用取模的方式，这种方式存在一个问题：<br />当查询条件是>, <, >=, <=、BETWEEN AND的时候，就无能为力了，此时要用其他的分片策略来解决，下面来看看如何解决。
<a name="mf38b"></a>
## 分片介绍
<a name="olQ4a"></a>
### 1、分片键
用于分片的数据库字段，是将数据库(表)水平拆分的关键字段。例：将订单表中的订单主键的尾数取模分片，则订单主键为分片字段。 SQL中如果无分片字段，将执行全路由，性能较差。 除了对单分片字段的支持，ShardingSphere也支持根据多个字段进行分片。
<a name="jxk8l"></a>
### 2、分片算法
通过分片算法将数据分片，支持通过=、>=、<=、>、<、BETWEEN和IN分片。分片算法需要应用方开发者自行实现，可实现的灵活度非常高。<br />目前提供4种分片算法。由于分片算法和业务实现紧密相关，因此并未提供内置分片算法，而是通过分片策略将各种场景提炼出来，提供更高层级的抽象，并提供接口让应用开发者自行实现分片算法。
<a name="ooD86"></a>
#### 精确分片算法
对应`PreciseShardingAlgorithm`，用于处理使用单一键作为分片键的=与IN进行分片的场景。需要配合`StandardShardingStrategy`使用。
<a name="JP8nj"></a>
#### 范围分片算法
对应`RangeShardingAlgorithm`，用于处理使用单一键作为分片键的BETWEEN AND、>、<、>=、<=进行分片的场景。需要配合`StandardShardingStrategy`使用。
<a name="QQhBW"></a>
#### 复合分片算法
对应`ComplexKeysShardingAlgorithm`，用于处理使用多键作为分片键进行分片的场景，包含多个分片键的逻辑较复杂，需要应用开发者自行处理其中的复杂度。需要配合`ComplexShardingStrategy`使用。
<a name="SFY8Y"></a>
#### Hint分片算法
对应`HintShardingAlgorithm`，用于处理使用Hint行分片的场景。需要配合`HintShardingStrategy`使用。
<a name="UyvHm"></a>
### 3、5种分片策略
包含分片键和分片算法，由于分片算法的独立性，将其独立抽离。真正可用于分片操作的是分片键 + 分片算法，也就是分片策略。目前提供5种分片策略。
<a name="rrmBb"></a>
#### 行表达式分片策略（`InlineShardingStrategy`）
对应`InlineShardingStrategy`。使用Groovy的表达式，提供对SQL语句中的=和IN的分片操作支持，只支持单分片键。对于简单的分片算法，可以通过简单的配置使用，从而避免繁琐的Java代码开发，如：`t_user_$->{u_id % 8}` 表示t_user表根据u_id模8，而分成8张表，表名称为t_user_0到t_user_7。
<a name="sQSuZ"></a>
#### 标准分片策略（`StandardShardingStrategy`）
对应`StandardShardingStrategy`。提供对SQL语句中的=, >, <, >=, <=, IN和BETWEEN AND的分片操作支持。`StandardShardingStrategy`只支持单分片键，提供`PreciseShardingAlgorithm`和`RangeShardingAlgorithm`两个分片算法。PreciseShardingAlgorithm是必选的，用于处理=和IN的分片。`RangeShardingAlgorithm`是可选的，用于处理BETWEEN AND, >, <, >=, <=分片，如果不配置`RangeShardingAlgorithm`，SQL中的BETWEEN AND将按照全库路由处理。
<a name="uVhz3"></a>
#### 复合分片策略（`ComplexShardingStrategy`）
对应`ComplexShardingStrategy`。复合分片策略。提供对SQL语句中的=, >, <, >=, <=, IN和BETWEEN AND的分片操作支持。`ComplexShardingStrategy`支持多分片键，由于多分片键之间的关系复杂，因此并未进行过多的封装，而是直接将分片键值组合以及分片操作符透传至分片算法，完全由应用开发者实现，提供最大的灵活度。
<a name="D0fQU"></a>
#### Hint分片策略（`HintShardingStrategy`）
对应`HintShardingStrategy`。通过Hint指定分片值而非从SQL中提取分片值的方式进行分片的策略。
<a name="Gu90U"></a>
#### 不分片策略
对应`NoneShardingStrategy`。不分片的策略。
<a name="e7hrR"></a>
### 4、SQL Hint
对于分片字段非SQL决定，而由其他外置条件决定的场景，可使用SQL Hint灵活的注入分片字段。例：内部系统，按照员工登录主键分库，而数据库中并无此字段。SQL Hint支持通过Java API和SQL注释(待实现)两种方式使用。<br />下面通过案例来介绍每种分片策略的使用以及场景。
<a name="PRDUU"></a>
### 5、白话解释分片策略
当使用分库分表的时候，目标库和表都存在多个，此时执行sql，那么sql最后会落到哪个库？那个表呢？<br />这就是分片策略需要解决的问题，主要解决2个问题：

1. sql应该到哪个库去执行？这个就是数据库路由策略决定的
2. sql应该到哪个表去执行呢？这个就是表的路由策略决定的

所以如果要对某个表进行分库分表，需要指定则两个策略

1. db路由策略，通过`TableRuleConfiguration#setDatabaseShardingStrategyConfig`进行设置
2. table路由策略，通过`TableRuleConfiguration#setTableShardingStrategyConfig`进行设置
<a name="SIeDp"></a>
## 广播表
<a name="OElJD"></a>
### 1、广播表是什么？
有时候，某些表需要在所有库中都有一个，且数据是一样的，比如字典表，这种表，插入表，所有表都会写入数据，而查询时，选择一个就可以了，这种场景需要用到shardingsphere中的广播表。
<a name="O2YRs"></a>
### 2、如何使用？
需要广播的表，需要调用`shardingRuleConfig.setBroadcastTable`进行设置。<br />代码如下，广播的表，插入数据会同时落到所有的库，查询只会落到一个库。
```java
ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();
// 这里需要指定t_dict为广播模式
shardingRuleConfig.setBroadcastTables(Arrays.asList("t_dict"));
```
<a name="XxQA5"></a>
### 3、示例
<a name="tCpDl"></a>
#### 1）需求
准备2个db：sj_ds0，sj_ds1<br />2个db中都包含字典表：t_dict<br />要求向t_dict写入数据的时候，将数据同时写2个库中的t_dict表，查询的时候随便选择一个即可
<a name="SmBcI"></a>
#### 2）sql脚本
```sql
drop database if exists sj_ds0;
create database sj_ds0;
use sj_ds0;

drop table if exists t_dict;
create table t_dict(
  id bigint not null primary key auto_increment,
  code varchar(64),
  k varchar(64) not null,
  v varchar(16) not null
);

drop database if exists sj_ds1;
create database sj_ds1;
use sj_ds1;

drop table if exists t_dict;
create table t_dict(
  id bigint not null primary key auto_increment,
  code varchar(64),
  k varchar(64) not null,
  v varchar(16) not null
);
```
<a name="guokS"></a>
#### 3）Java代码
```java
import com.zaxxer.hikari.HikariDataSource;
import org.apache.shardingsphere.api.config.sharding.ShardingRuleConfiguration;
import org.apache.shardingsphere.shardingjdbc.api.ShardingDataSourceFactory;
import org.apache.shardingsphere.underlying.common.config.properties.ConfigurationPropertyKey;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Properties;

public class Demo5 {
    public static void main(String[] args) throws SQLException {
        /**
         * 1.配置真实数据源
         */
        Map<String, DataSource> dataSourceMap = new LinkedHashMap<>();
        dataSourceMap.put("ds0", dataSource1());
        dataSourceMap.put("ds1", dataSource2());

        /**
         * 2、无分片规则
         */
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();
        // 这里需要指定t_dict为广播模式
        shardingRuleConfig.setBroadcastTables(Arrays.asList("t_dict"));
        /**
         * 4、配置一些属性
         */
        Properties props = new Properties();
        //输出sql
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), true);

        /**
         * 5、创建数据源
         */
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), true);
        DataSource dataSource = ShardingDataSourceFactory.createDataSource(dataSourceMap, shardingRuleConfig, props);
        Connection connection = dataSource.getConnection();

        System.out.println("测试插入数据：");
        String sql = "insert into t_dict (code,k,v) values ('gender','男','1'),('gender','女','2')";
        PreparedStatement ps = connection.prepareStatement(sql);
        System.out.println("插入记录数：" + ps.executeUpdate());

        System.out.println("测试查询数据：");
        ps = connection.prepareStatement("select count(*) from t_dict");
        ResultSet rs = ps.executeQuery();
        while (rs.next()) {
            System.out.println("count:" + rs.getInt(1));
        }

        ps.close();
        connection.close();
    }


    private static DataSource dataSource1() {
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/sj_ds0?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        return dataSource1;
    }

    private static DataSource dataSource2() {
        HikariDataSource dataSource1 = new HikariDataSource();
        dataSource1.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource1.setJdbcUrl("jdbc:mysql://localhost:3306/sj_ds1?characterEncoding=UTF-8");
        dataSource1.setUsername("root");
        dataSource1.setPassword("root123");
        return dataSource1;
    }
}
```
<a name="sz2pJ"></a>
#### 4）运行输出
广播的表，插入数据会同时落到所有的库，查询只会落到一个库
```
测试插入数据：
Logic SQL: insert into t_dict (code,k,v) values ('gender','男','1'),('gender','女','2')
Actual SQL: ds0 ::: insert into t_dict (code,k,v) values ('gender', '男', '1'), ('gender', '女', '2')
Actual SQL: ds1 ::: insert into t_dict (code,k,v) values ('gender', '男', '1'), ('gender', '女', '2')
插入记录数：2

测试查询数据：
Logic SQL: select count(*) from t_dict
Actual SQL: ds0 ::: select count(*) from t_dict
count:2
```
<a name="cJCjB"></a>
## 表关联
<a name="KzX0t"></a>
### 1、表关联是什么？
指分片规则一致的主表和子表。例如：t_order表和t_order_item表，均按照order_id分片，则此两张表互为绑定表关系。绑定表之间的多表关联查询不会出现笛卡尔积关联，关联查询效率将大大提升。举例说明，如果SQL为：
```sql
SELECT i.* FROM t_order o JOIN t_order_item i ON o.order_id=i.order_id WHERE o.order_id in (10, 11);
```
在不配置绑定表关系时，假设分片键order_id将数值10路由至第0片，将数值11路由至第1片，那么路由后的SQL应该为4条，它们呈现为笛卡尔积：
```sql
SELECT i.* FROM t_order_0 o JOIN t_order_item_0 i ON o.order_id=i.order_id WHERE o.order_id in (10, 11);

SELECT i.* FROM t_order_0 o JOIN t_order_item_1 i ON o.order_id=i.order_id WHERE o.order_id in (10, 11);

SELECT i.* FROM t_order_1 o JOIN t_order_item_0 i ON o.order_id=i.order_id WHERE o.order_id in (10, 11);

SELECT i.* FROM t_order_1 o JOIN t_order_item_1 i ON o.order_id=i.order_id WHERE o.order_id in (10, 11);
```
在配置绑定表关系后，路由的SQL应该为2条：
```sql
SELECT i.* FROM t_order_0 o JOIN t_order_item_0 i ON o.order_id=i.order_id WHERE o.order_id in (10, 11);

SELECT i.* FROM t_order_1 o JOIN t_order_item_1 i ON o.order_id=i.order_id WHERE o.order_id in (10, 11);
```
其中t_order在FROM的最左侧，ShardingSphere将会以它作为整个绑定表的主表。 所有路由计算将会只使用主表的策略，那么t_order_item表的分片计算将会使用t_order的条件。故绑定表之间的分区键要完全相同。
<a name="fP5YC"></a>
### 2、案例
<a name="EbPdK"></a>
#### 1）需求

- ds_order库有4张表，2张订单表，2张订单明细表
- 2张订单表：t_order_0（存放id为奇数的数据）、t_order_1（存放id为偶数的数据）
- 2张订单明细表：t_order_0（存放order_id为奇数的数据，和t_order_0分片规则一致）、t_order_1（存放order_id为偶数的数据，和t_order_1分片规则一致）
<a name="yjUv0"></a>
#### 2）sql脚本
```sql
drop database if exists ds_order;
create database ds_order;
use ds_order;

drop table if exists t_order_0;
create table t_order_0(
  order_id bigint not null primary key,
  price int not null
);


drop table if exists t_order_1;
create table t_order_1(
  order_id bigint not null primary key,
  price int not null
);


drop table if exists t_order_item_0;
create table t_order_item_0(
  id bigint not null primary key,
  order_id bigint not null,
  price int not null
);

create index idx_order_id on t_order_item_0(order_id);


drop table if exists t_order_item_1;
create table t_order_item_1(
  id bigint not null primary key,
  order_id bigint not null,
  price int not null
);

create index idx_order_id on t_order_item_1(order_id);

insert into t_order_0 values (1,20);
insert into t_order_1 values (2,30);

insert into t_order_item_0 values (1,1,5),(2,1,15);
insert into t_order_item_1 values (3,2,10),(4,2,20);
```
<a name="V5TGL"></a>
#### 3）Java代码：BindingTableGroupsTest
代码如下，关键代码就是配置了2张表(t_order、t_order_item)的分片策略
```java
@Slf4j
public class BindingTableGroupsTest {
    private static DataSource dataSource;

    @BeforeAll
    public static void init() throws SQLException {
        HikariDataSource ds = new HikariDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setJdbcUrl("jdbc:mysql://localhost:3306/ds_order?characterEncoding=UTF-8");
        ds.setUsername("root");
        ds.setPassword("root123");

        /**
         * 1.配置真实数据源
         */
        Map<String, DataSource> dataSourceMap = new LinkedHashMap<>();
        dataSourceMap.put("ds", ds);

        /**
         * 2、配置2个表的分片规则
         */
        //t_order分片规则
        TableRuleConfiguration orderRuleConfiguration =
        new TableRuleConfiguration("t_order", "ds.t_order_$->{0..1}");
        InlineShardingStrategyConfiguration orderTableShardingStrategy =
        new InlineShardingStrategyConfiguration("order_id", "t_order_$->{(order_id + 1) % 2}");
        orderRuleConfiguration.setTableShardingStrategyConfig(orderTableShardingStrategy);

        //t_order_item分片规则
        TableRuleConfiguration orderItemRuleConfiguration =
        new TableRuleConfiguration("t_order_item", "ds.t_order_item_$->{0..1}");
        InlineShardingStrategyConfiguration orderItemTableShardingStrategy =
        new InlineShardingStrategyConfiguration("order_id", "t_order_item_$->{(order_id + 1) % 2}");
        orderItemRuleConfiguration.setTableShardingStrategyConfig(orderItemTableShardingStrategy);

        /**
         * 3、加入表的分片规则
         */
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();
        shardingRuleConfig.getTableRuleConfigs().add(orderRuleConfiguration);
        shardingRuleConfig.getTableRuleConfigs().add(orderItemRuleConfiguration);

        /**
         * 4、配置一些属性
         */
        Properties props = new Properties();
        //输出sql
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), true);

        /**
         * 5、创建数据源
         */
        dataSource = ShardingDataSourceFactory.createDataSource(dataSourceMap, shardingRuleConfig, props);
    }

}
```
<a name="CEIBG"></a>
#### 4）重点代码
上面代码中重点代码就下面这些，不要搞错了<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697982437913-de0a2088-5340-4a0c-876d-521ab9dc5341.png#averageHue=%23fcfbf9&clientId=u5b7fc306-fb2f-4&from=paste&height=298&id=ue24ccd91&originHeight=746&originWidth=2160&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=531009&status=done&style=none&taskId=u1a431a28-da7e-4edb-9309-b5c777c3e1d&title=&width=864)
<a name="KSA5e"></a>
#### 5）测试：查询
BindingTableGroupsTest中添加下面，用来查询order_id为1的订单明细，order_id为1的数据，实际上是在t_order_0和t_order_item_0表中，稍后大家注意看实际的sql
```java
@Test
public void test1() throws SQLException {
    String sql = "select a.order_id,b.id as order_item_id,b.price " +
        "from t_order a,t_order_item b " +
        "where a.order_id = b.order_id and a.order_id = 1";
    try (Connection connection = dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);
         ResultSet rs = ps.executeQuery();) {
        while (rs.next()) {
            Long order_id = rs.getLong("order_id");
            Long order_item_id = rs.getLong("order_item_id");
            Integer price = rs.getInt("price");
            System.out.println(String.format("order_id：%s，order_item_id：%s, price：%s", order_id, order_item_id, price));
        }
    }
}
```
运行输出，如下，产生了2条sql，路由了3张表：t_order_0、t_order_item_0、t_order_item_1，实际上t_order_item_1表中是没有数据的，没必要路由，如何解决这个问题？设置表关联。
```
Logic SQL: select a.order_id,b.id as order_item_id,b.price from t_order a,t_order_item b where a.order_id = b.order_id and a.order_id = 1
Actual SQL: ds ::: select a.order_id,b.id as order_item_id,b.price from t_order_0 a,t_order_item_1 b where a.order_id = b.order_id and a.order_id = 1
Actual SQL: ds ::: select a.order_id,b.id as order_item_id,b.price from t_order_0 a,t_order_item_0 b where a.order_id = b.order_id and a.order_id = 1
order_id：1，order_item_id：1, price：5
order_id：1，order_item_id：2, price：15
```
<a name="UCtNl"></a>
#### 6）设置2个表关联
当2个表的路由字段相同的时候，可以设置表关联，可以避免笛卡尔积查询，下面设置t_order和t_order_item关联
```java
shardingRuleConfig.setBindingTableGroups(Arrays.asList("t_order","t_order_item"));
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697982661274-681397bc-79cf-44f2-9c21-b7cb05ff8579.png#averageHue=%23fbf9f6&clientId=u5b7fc306-fb2f-4&from=paste&height=212&id=u3060098e&originHeight=530&originWidth=2144&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=418517&status=done&style=none&taskId=u1e9c9ec0-f0e1-4b66-ba66-b56f86cb224&title=&width=857.6)
<a name="Hz2XR"></a>
#### 7）再次运行测试用例
输出如下，这次只有一条sql了，查询被路由到t_order_0和t_order_item_1了，符合预期。
```
Logic SQL: select a.order_id,b.id as order_item_id,b.price from t_order a,t_order_item b where a.order_id = b.order_id and a.order_id = 1
Actual SQL: ds ::: select a.order_id,b.id as order_item_id,b.price from t_order_0 a,t_order_item_0 b where a.order_id = b.order_id and a.order_id = 1
order_id：1，order_item_id：1, price：5
order_id：1，order_item_id：2, price：15
```
<a name="Kdsw0"></a>
## 读写分离+分片
<a name="bq2Er"></a>
### 1、背景
面对日益增加的系统访问量，数据库的吞吐量面临着巨大瓶颈。 对于同一时刻有大量并发读操作和较少写操作类型的应用系统来说，将数据库拆分为主库和从库，主库负责处理事务性的增删改操作，从库负责处理查询操作，能够有效的避免由数据更新导致的行锁，使得整个系统的查询性能得到极大的改善。<br />通过一主多从的配置方式，可以将查询请求均匀的分散到多个数据副本，能够进一步的提升系统的处理能力。 使用多主多从的方式，不但能够提升系统的吞吐量，还能够提升系统的可用性，可以达到在任何一个数据库宕机，甚至磁盘物理损坏的情况下仍然不影响系统的正常运行。<br />与将数据根据分片键打散至各个数据节点的水平分片不同，读写分离则是根据SQL语义的分析，将读操作和写操作分别路由至主库与从库。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697983523481-95ac76cf-65e9-477e-95aa-365e18deaeb2.png#averageHue=%23f9f3ee&clientId=u4273a39e-c36f-4&from=paste&height=604&id=uc320e116&originHeight=1511&originWidth=2140&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=310566&status=done&style=none&taskId=ua37ced24-5bad-4543-b1ad-081917decaa&title=&width=856)<br />读写分离的数据节点中的数据内容是一致的，而水平分片的每个数据节点的数据内容却并不相同。将水平分片和读写分离联合使用，能够更加有效的提升系统性能。<br />读写分离虽然可以提升系统的吞吐量和可用性，但同时也带来了数据不一致的问题。 这包括多个主库之间的数据一致性，以及主库与从库之间的数据一致性的问题。 并且，读写分离也带来了与数据分片同样的问题，它同样会使得应用开发和运维人员对数据库的操作和运维变得更加复杂。 下图展现了将分库分表与读写分离一同使用时，应用程序与数据库集群之间的复杂拓扑关系。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697983803913-8393df2e-e5bc-4e11-959e-bbe00697cc92.png#averageHue=%23fdefe2&clientId=u4273a39e-c36f-4&from=paste&height=605&id=u8a23a507&originHeight=1513&originWidth=2262&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=561034&status=done&style=none&taskId=u8a218337-57db-4956-8145-6c81b609935&title=&width=904.8)
<a name="YDxYA"></a>
### 2、案例：实现2主2从+分片
<a name="WpN5A"></a>
#### 1）需求

- 2个主库：ds_master_0，ds_master_1
- 2个从库：ds_slave_0，ds_slave_1，分别是上面2个主库的从库
- 4个库中都有一个t_user表(id,name)
- ds_master_0和其从库ds_slave_0存放t_user表id为偶数的数据，ds_master_1和其从库ds_slave_1存放t_user表id为基数的数据
- 稍后会演示常见的使用场景：写入数据的场景、无事务读取数据的场景、有事务读取的场景、强制将读路由到主库的场景，大家一定要注意观察sql路由情况
- 本案例不介绍主从如何同步，需要了解的朋友自行查找相关资料
<a name="d9e23"></a>
#### 2）准备sql
```sql
drop database if exists ds_master_0;
create database ds_master_0;
drop database if exists ds_master_1;
create database ds_master_1;
drop database if exists ds_slave_0;
create database ds_slave_0;
drop database if exists ds_slave_1;
create database ds_slave_1;

use ds_master_0;
drop table if exists t_user;
create table t_user(
  id bigint not null primary key,
  name varchar(64) not null
);
insert into t_user VALUES (1,'我是ds_master_0');

use ds_master_1;
drop table if exists t_user;
create table t_user(
  id bigint not null primary key,
  name varchar(64) not null
);
insert into t_user VALUES (2,'我是ds_master_1');

use ds_slave_0;
drop table if exists t_user;
create table t_user(
  id bigint not null primary key,
  name varchar(64) not null
);
insert into t_user VALUES (1,'我是ds_slave_0');

use ds_slave_1;
drop table if exists t_user;
create table t_user(
  id bigint not null primary key,
  name varchar(64) not null
);
insert into t_user VALUES (2,'我是ds_slave_1');
```
<a name="mg33V"></a>
#### 3）创建测试类
```java
public class MasterSlaveTests {

    private static DataSource dataSource;

    @BeforeAll
    public static void init() throws SQLException {
        /**
         * 1、配置真实数据源
         */
        HikariDataSource ds_master_0 = new HikariDataSource();
        ds_master_0.setDriverClassName("com.mysql.jdbc.Driver");
        ds_master_0.setJdbcUrl("jdbc:mysql://localhost:3306/ds_master_0?characterEncoding=UTF-8");
        ds_master_0.setUsername("root");
        ds_master_0.setPassword("root123");

        HikariDataSource ds_slave_0 = new HikariDataSource();
        ds_slave_0.setDriverClassName("com.mysql.jdbc.Driver");
        ds_slave_0.setJdbcUrl("jdbc:mysql://localhost:3306/ds_slave_0?characterEncoding=UTF-8");
        ds_slave_0.setUsername("root");
        ds_slave_0.setPassword("root123");

        HikariDataSource ds_master_1 = new HikariDataSource();
        ds_master_1.setDriverClassName("com.mysql.jdbc.Driver");
        ds_master_1.setJdbcUrl("jdbc:mysql://localhost:3306/ds_master_1?characterEncoding=UTF-8");
        ds_master_1.setUsername("root");
        ds_master_1.setPassword("root123");

        HikariDataSource ds_slave_1 = new HikariDataSource();
        ds_slave_1.setDriverClassName("com.mysql.jdbc.Driver");
        ds_slave_1.setJdbcUrl("jdbc:mysql://localhost:3306/ds_slave_1?characterEncoding=UTF-8");
        ds_slave_1.setUsername("root");
        ds_slave_1.setPassword("root123");

        // 将4个数据源加入 dataSourceMap
        Map<String, DataSource> dataSourceMap = new HashMap<>();
        dataSourceMap.put("ds_master_0", ds_master_0);
        dataSourceMap.put("ds_slave_0", ds_slave_0);
        dataSourceMap.put("ds_master_1", ds_master_1);
        dataSourceMap.put("ds_slave_1", ds_slave_1);

        // 主从规则配置，就是配置主从关系，让系统知道哪个库是主库、他的从库列表是哪些？
        MasterSlaveRuleConfiguration master0SlaveRuleConfig =
                new MasterSlaveRuleConfiguration(
                        "ds0",
                        "ds_master_0",  //dataSourceMap中主库的key
                        Arrays.asList("ds_slave_0")); // dataSourceMap中ds_master_0从库的key
        // 配置读写分离规则
        MasterSlaveRuleConfiguration master1SlaveRuleConfig =
                new MasterSlaveRuleConfiguration(
                        "ds1",
                        "ds_master_1",  //dataSourceMap中主库的key
                        Arrays.asList("ds_slave_1")); // dataSourceMap中ds_master_1从库的key

        /**
         * 2、配置t_user分片规则
         */
        TableRuleConfiguration userTableRuleConfiguration =
                new TableRuleConfiguration("t_user", "ds$->{0..1}.t_user");
        //设置t_user表的分库规则
        final InlineShardingStrategyConfiguration userTableShardingStrategy =
                new InlineShardingStrategyConfiguration("id", "ds$->{(id+1) % 2}");
        userTableRuleConfiguration.setDatabaseShardingStrategyConfig(userTableShardingStrategy);

        /**
         * 3、创建分片配置对象ShardingRuleConfiguration
         */
        ShardingRuleConfiguration shardingRuleConfig = new ShardingRuleConfiguration();
        //将userTableRuleConfiguration放入表规则配置列表
        shardingRuleConfig.getTableRuleConfigs().add(userTableRuleConfiguration);
        //设置主从规则配置
        shardingRuleConfig.setMasterSlaveRuleConfigs(Arrays.asList(master0SlaveRuleConfig, master1SlaveRuleConfig));

        /**
         * 4、配置一些属性
         */
        Properties props = new Properties();
        //输出sql
        props.put(ConfigurationPropertyKey.SQL_SHOW.getKey(), true);


        /**
         * 5、创建数据源
         */
        dataSource = ShardingDataSourceFactory.createDataSource(dataSourceMap, shardingRuleConfig, props);
    }

}
```
<a name="flm5Z"></a>
#### 4）关键代码
下图配置了4个数据源，以及配置了数据源之间的主从关系，要让shardingsphere知道他们的主从关系<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697984198247-47996cb1-2cb2-4f03-89b6-a5761804a808.png#averageHue=%23fbf9f3&clientId=u4273a39e-c36f-4&from=paste&height=490&id=ua014b52e&originHeight=1224&originWidth=2086&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1128535&status=done&style=none&taskId=u3cf28680-75a5-4f3b-8a36-bca5d109ff7&title=&width=834.4)<br />下图中配置了表的分片规则<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697984627472-6637e54b-f6a2-45b1-b5d6-56ec6771db0c.png#averageHue=%23fbfaf7&clientId=u4273a39e-c36f-4&from=paste&height=494&id=u59a6badc&originHeight=1234&originWidth=2165&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1189859&status=done&style=none&taskId=u72cec2c6-ab31-40f8-b729-d30c78ec64d&title=&width=866)<br />还有一行关键代码，如下图<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697985614038-6a80fc80-ed77-4822-8e2c-cbef7721de31.png#averageHue=%23fbfaf8&clientId=u4273a39e-c36f-4&from=paste&height=202&id=u7d3be234&originHeight=504&originWidth=2162&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=412774&status=done&style=none&taskId=uce4aedce-4e90-4446-b90f-6ddf9609174&title=&width=864.8)<br />下面上测试案例，案例都位于上面这个类中，一定要注意看案例了。
<a name="MBD7q"></a>
#### 5）案例1：无事务读取落入从库
```java
/**
 * 无事务查询
 *
 * @throws SQLException
 */
@Test
public void test1() throws SQLException {
    String sql = "select id,name from t_user where id = 1";
    try (Connection connection = dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);
         ResultSet rs = ps.executeQuery();) {
        while (rs.next()) {
            final long id = rs.getLong("id");
            final String name = rs.getString("name");
            System.out.println(String.format("id:%s,name:%s", id, name));
        }
    }
}
```
运行输出，如下
```
Logic SQL: select id,name from t_user where id = 1
Actual SQL: ds_slave_0 ::: select id,name from t_user where id = 1
id:1,name:我是ds_slave_0
```
**结论：**无事务查询，会落入从库。
<a name="cZGRo"></a>
#### 6）案例2：事务中直接读取落入从库
下面将连接设置为手动提交，然后读取id为2的记录
```java
/**
 * 有事务查询
 *
 * @throws SQLException
 */
@Test
public void test2() throws SQLException {
    try (Connection connection = dataSource.getConnection();) {
        //手动开启事务
        connection.setAutoCommit(false);
        String sql = "select id,name from t_user where id = 2";
        PreparedStatement ps = connection.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();
        while (rs.next()) {
            final long id = rs.getLong("id");
            final String name = rs.getString("name");
            System.out.println(String.format("id:%s,name:%s", id, name));
        }
        connection.commit();
    }
}
```
运行输出，如下，落入从库ds_slave_1了
```
Logic SQL: select id,name from t_user where id = 2
Actual SQL: ds_slave_1 ::: select id,name from t_user where id = 2
id:2,name:我是ds_slave_1
```
**结论：**开启事务，直接读取数据，会落入从库。
<a name="vbQhh"></a>
#### 7）案例3：事务中写入之后读取落入主库
开启事务，然后写入id为3的数据，然后读取这条数据，然后读取已经存在的一条id为2的数据，看看效果
```java
/**
 * 有事务，写入数据，然后查询，（写入 & 查询都落入主库）
 *
 * @throws SQLException
 */
@Test
public void test3() throws SQLException {
    try (Connection connection = dataSource.getConnection();) {
        connection.setAutoCommit(false);
        System.out.println("-----------插入id为3数据-----------");
        String sql = "insert into t_user values (3,'张三')";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.executeUpdate();

        System.out.println("-----------查询刚插入的数据-----------");
        sql = "select id,name from t_user where id = 3";
        ps = connection.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();
        while (rs.next()) {
            final long id = rs.getLong("id");
            final String name = rs.getString("name");
            System.out.println(String.format("id:%s,name:%s", id, name));
        }

        System.out.println("上面id为3的在t_master_0,下面来看看读取id为2的，看看会读取主库还是从库？");
        System.out.println("-----------查询id为2的数据-----------");

        sql = "select id,name from t_user where id = 2";
        ps = connection.prepareStatement(sql);
        rs = ps.executeQuery();
        while (rs.next()) {
            final long id = rs.getLong("id");
            final String name = rs.getString("name");
            System.out.println(String.format("id:%s,name:%s", id, name));
        }
        connection.commit();
    }
}
```
运行输出，结果如下，3个sql都落入了主库。
```
-----------插入id为3数据-----------
Logic SQL: insert into t_user values (3,'张三')
Actual SQL: ds_master_0 ::: insert into t_user values (3, '张三')

-----------查询刚插入的数据-----------
Logic SQL: select id,name from t_user where id = 3
Actual SQL: ds_master_0 ::: select id,name from t_user where id = 3
id:3,name:张三

上面id为3的在t_master_0,下面来看看读取id为2的，看看会读取主库还是从库？
-----------查询id为2的数据-----------
Logic SQL: select id,name from t_user where id = 2
Actual SQL: ds_master_1 ::: select id,name from t_user where id = 2
id:2,name:我是ds_master_1
```
**结论：只要开启了手动事务，且第一个sql为insert，后面的不管路由到哪个库，都会落入主库**
<a name="frOcz"></a>
#### 8）案例4：通过HintManager强制查询走主库
可以通过`hintManager.setMasterRouteOnly()`强制走主库，代码如下
```java
/**
 * 通过hintManager.setMasterRouteOnly()强制走主库
 *
 * @throws SQLException
 */
@Test
public void test4() throws SQLException {
    String sql = "select id,name from t_user where id = 1";
    try (Connection connection = dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);) {

        HintManager hintManager = null;
        try {
            //通过HintManager.setMasterRouteOnly()强制走主库，注意在finally中释放HintManager.close();
            hintManager = HintManager.getInstance();
            hintManager.setMasterRouteOnly();

            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                final long id = rs.getLong("id");
                final String name = rs.getString("name");
                System.out.println(String.format("id:%s,name:%s", id, name));
            }
        } finally {
            if (hintManager != null) {
                hintManager.close();
            }
        }
    }
}
```
运行输出，如下，走主库了
```
Logic SQL: select id,name from t_user where id = 1
Actual SQL: ds_master_0 ::: select id,name from t_user where id = 1
id:1,name:我是ds_master_0
```
