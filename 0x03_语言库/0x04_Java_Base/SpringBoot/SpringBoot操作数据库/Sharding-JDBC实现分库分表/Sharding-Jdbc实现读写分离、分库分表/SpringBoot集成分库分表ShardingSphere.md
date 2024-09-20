JavaSpringBoot
<a name="pNR7H"></a>
## 1、需求
SpringBoot中使用shardingsphere实现下面功能

- 2个库：sj_ds0、sj_ds1
- 2个库中都包含2个表：t_order_0，t_order_1
- 根据 user_id%2 路由库，根据 order_id%2路由表
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
<a name="tc2et"></a>
## 3、创建SpringBoot项目
<a name="aaPQG"></a>
## 4、引入Shardingsphere Maven配置
```xml
<dependency>
  <groupId>org.apache.shardingsphere</groupId>
  <artifactId>sharding-jdbc-spring-boot-starter</artifactId>
  <version>4.1.1</version>
</dependency>
```
<a name="e7JR5"></a>
## 5、application.properties中配置分库分表信息
```
spring.shardingsphere.datasource.names=ds0,ds1

spring.shardingsphere.datasource.ds0.type=com.zaxxer.hikari.HikariDataSource
spring.shardingsphere.datasource.ds0.driver-class-name=com.mysql.jdbc.Driver
spring.shardingsphere.datasource.ds0.jdbc-url=jdbc:mysql://localhost:3306/sj_ds0?characterEncoding=UTF-8
spring.shardingsphere.datasource.ds0.username=root
spring.shardingsphere.datasource.ds0.password=root123

spring.shardingsphere.datasource.ds1.type=com.zaxxer.hikari.HikariDataSource
spring.shardingsphere.datasource.ds1.driver-class-name=com.mysql.jdbc.Driver
spring.shardingsphere.datasource.ds1.jdbc-url=jdbc:mysql://localhost:3306/sj_ds1?characterEncoding=UTF-8
spring.shardingsphere.datasource.ds1.username=root
spring.shardingsphere.datasource.ds1.password=root123

spring.shardingsphere.sharding.tables.t_order.actual-data-nodes=ds$->{0..1}.t_order_$->{0..1}
spring.shardingsphere.sharding.tables.t_order.database-strategy.inline.sharding-column=user_id
spring.shardingsphere.sharding.tables.t_order.database-strategy.inline.algorithm-expression=ds$->{user_id % 2}
spring.shardingsphere.sharding.tables.t_order.table-strategy.inline.sharding-column=order_id
spring.shardingsphere.sharding.tables.t_order.table-strategy.inline.algorithm-expression=t_order_$->{order_id % 2}

spring.shardingsphere.props.sql.show: true
```
<a name="dT9Vl"></a>
## 6、测试效果
<a name="ssOuM"></a>
### 1）创建测试类
```java
import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import javax.sql.DataSource;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

@SpringBootTest
@Slf4j
class SpringbootDemoApplicationTests {

    @Autowired
    private DataSource dataSource;

}
```
下面来测试5种场景，注意观察每种场景的输出，观察sql路由的db和表。<br />每个测试方法均放入上面的测试类中。
<a name="DHXTh"></a>
### 2）测试1：插入数据
下面插入4个表，根据路由规则，2个库4张表中每个表会落入一条数据。
```java
@Test
void test1() throws Exception {
    String sql = "insert into t_order (order_id,user_id,price) values (?,?,?)";
    try (Connection connection = this.dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);) {
        // 插入4条数据测试,每个表会落入1条数据
        for (long user_id = 1; user_id <= 2; user_id++) {
            for (long order_id = 1; order_id <= 2; order_id++) {
                int j = 1;
                ps.setLong(j++, order_id);
                ps.setLong(j++, user_id);
                ps.setLong(j, 100);
                log.info("count:{}", ps.executeUpdate());
            }
        }
    }
}
```
运行输出如下，可以详细看到每条sql路由信息（路由到哪个库的哪张表），Logic SQL表示逻辑sql，Actual SQL表示实际执行的sql
```
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds1 ::: insert into t_order_1 (order_id,user_id,price) values (?, ?, ?) ::: [1, 1, 100]
count:1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds1 ::: insert into t_order_0 (order_id,user_id,price) values (?, ?, ?) ::: [2, 1, 100]
count:1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds0 ::: insert into t_order_1 (order_id,user_id,price) values (?, ?, ?) ::: [1, 2, 100]
count:1
Logic SQL: insert into t_order (order_id,user_id,price) values (?,?,?)
Actual SQL: ds0 ::: insert into t_order_0 (order_id,user_id,price) values (?, ?, ?) ::: [2, 2, 100]
count:1
```
<a name="Xjm8y"></a>
### 3）测试2：无条件查询
下面查询t_order表所有记录数，没有查询条件查询会被路由到所有db中所有表。<br />shardingsphere会对4个表进行查询，最后将结果汇总。
```java
@Test
void test2() throws SQLException {
    String sql = "select count(*) from t_order";
    try (Connection connection = this.dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);) {
        long st = System.currentTimeMillis();
        final ResultSet rs = ps.executeQuery();
        while (rs.next()) {
            log.info("记录数：{}", rs.getInt(1));
        }
    }
}
```
运行输出
```
Logic SQL: select count(*) from t_order
Actual SQL: ds0 ::: select count(*) from t_order_0
Actual SQL: ds0 ::: select count(*) from t_order_1
Actual SQL: ds1 ::: select count(*) from t_order_0
Actual SQL: ds1 ::: select count(*) from t_order_1
记录数：4
```
<a name="ASZcA"></a>
### 4）测试3：查询某个用户的订单量
由于 user_id 是 db 路由的字段，order_id 是表的路由字段，若条件中只有 user_id，则会路由到对应库的所有表。
```java
@Test
void test3() throws SQLException {
    String sql = "select count(*) from t_order where user_id = 1";
    try (Connection connection = this.dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);) {
        long st = System.currentTimeMillis();
        final ResultSet rs = ps.executeQuery();
        while (rs.next()) {
            log.info("记录数：{}", rs.getInt(1));
        }
    }
}
```
运行输出，如下，sql被路由到ds1中的2个t_order表
```
Logic SQL: select count(*) from t_order where user_id = 1
Actual SQL: ds1 ::: select count(*) from t_order_0 where user_id = 1
Actual SQL: ds1 ::: select count(*) from t_order_1 where user_id = 1
记录数：2
```
<a name="IY1Mh"></a>
### 5）测试4：查询某个用户的某个订单
若查询条件中同时含有db路由字段user_id，以及表的路由字段order_id，那么可以确定最后被路由的表。
```java
@Test
void test4() throws SQLException {
    String sql = "select count(*) from t_order where user_id = 1 and order_id = 1";
    try (Connection connection = this.dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);) {
        long st = System.currentTimeMillis();
        final ResultSet rs = ps.executeQuery();
        while (rs.next()) {
            log.info("记录数：{}", rs.getInt(1));
        }
    }
}
```
运行输出
```
Logic SQL: select count(*) from t_order where user_id = 1 and order_id = 1
Actual SQL: ds1 ::: select count(*) from t_order_1 where user_id = 1 and order_id = 1
记录数：1
```
<a name="huSXK"></a>
### 6）测试5：in查询
```java
@Test
void test5() throws SQLException {
    String sql = "select count(*) from t_order where user_id = 1 and order_id in (1,2)";
    try (Connection connection = this.dataSource.getConnection();
         PreparedStatement ps = connection.prepareStatement(sql);) {
        long st = System.currentTimeMillis();
        final ResultSet rs = ps.executeQuery();
        while (rs.next()) {
            log.info("记录数：{}", rs.getInt(1));
        }
    }
}
```
运行输出
```
Logic SQL: select count(*) from t_order where user_id = 1 and order_id in (1,2)
Actual SQL: ds1 ::: select count(*) from t_order_0 where user_id = 1 and order_id in (1,2)
Actual SQL: ds1 ::: select count(*) from t_order_1 where user_id = 1 and order_id in (1,2)
记录数：2
```
<a name="IXy26"></a>
## 7、其他的用法
其他用法使用不同的分片策略，进行改造。
<a name="WTfSY"></a>
## 8、配置文件如何配置？
有2种方法可以知道。
<a name="T3QH7"></a>
### 1）方法1：官方文档
[https://shardingsphere.apache.org/document/legacy/4.x/document/cn/manual/sharding-jdbc/configuration/config-spring-boot/](https://shardingsphere.apache.org/document/legacy/4.x/document/cn/manual/sharding-jdbc/configuration/config-spring-boot/)
<a name="yVj9i"></a>
### 2）方法2：看源码
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697986696053-da7f94fd-638c-487b-8ddc-1adb3173c000.png#averageHue=%23eee8d5&clientId=u9e430002-9c38-4&from=paste&height=438&id=u885cfea7&originHeight=1095&originWidth=2147&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1160453&status=done&style=none&taskId=uc40d0cb1-3cbe-4fc0-97b5-bc63e21f748&title=&width=858.8)<br />主要就分2部分：

- 第1部分：数据源的配置，这个就看数据源对应的类中有哪些属性，就可以配置那些信息
- 第2部分：分片规则的一些配置，这个对应的配置类是`org.apache.shardingsphere.shardingjdbc.spring.boot.sharding.SpringBootShardingRuleConfigurationProperties`，进去看源码，有哪些属性就有哪些配置
