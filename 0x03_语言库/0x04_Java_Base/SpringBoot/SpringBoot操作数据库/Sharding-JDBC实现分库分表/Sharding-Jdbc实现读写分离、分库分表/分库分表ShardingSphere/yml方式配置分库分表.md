纯Java api的方式，写起来比较繁琐，shardingsphere提供了更简单的方式：yml配置文件的方式。
<a name="yqRfx"></a>
## 1、使用步骤

- 创建一个yml格式的文件，将分库分表信息配置到yml中
- 通过yml文件创建DataSource
- 使用DataSource执行db操作

下面整个案例感受下效果。
<a name="afg4n"></a>
## 2、需求
2个库：sj_ds0、sj_ds1<br />2个库中都包含2个表：t_order_0，t_order_1<br />根据 user_id%2 路由库，根据 order_id%2路由表。
<a name="uM3lf"></a>
## 3、执行sql
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
<a name="mjFhD"></a>
## 4、test.yml
resource目录创建test.yml，和SpringBoot项目的application.properties同一个目录，内容如下，将所有分库分表信息都丢到这个文件中
```yaml
dataSources:
  ds0: !!com.zaxxer.hikari.HikariDataSource
    driverClassName: com.mysql.jdbc.Driver
    jdbcUrl: jdbc:mysql://localhost:3306/sj_ds0?characterEncoding=UTF-8
    username: root
    password: root123
  ds1: !!com.zaxxer.hikari.HikariDataSource
    driverClassName: com.mysql.jdbc.Driver
    jdbcUrl: jdbc:mysql://localhost:3306/sj_ds1?characterEncoding=UTF-8
    username: root
    password: root123
shardingRule:
  tables:
    t_order:
      actualDataNodes: ds$->{0..1}.t_order_$->{0..1}
      databaseStrategy:
        inline:
          algorithmExpression: ds$->{user_id % 2}
          shardingColumn: user_id
      logicTable: t_order
      tableStrategy:
        inline:
          algorithmExpression: t_order_$->{order_id % 2}
          shardingColumn: order_id
```
<a name="HtnG8"></a>
## 5、Java代码
代码如下，通过test.yml来创建dataSource，是不是方便了很多。
```java
import org.apache.shardingsphere.shardingjdbc.api.yaml.YamlShardingDataSourceFactory;

import javax.sql.DataSource;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class YmlShardingTest {

    public static void main(String[] args) throws IOException, SQLException {
        //1.读取demo2.yml文件
        InputStream inputStream = ClassLoader.getSystemResourceAsStream("test.yml");
        byte[] bytes = new byte[inputStream.available()];
        inputStream.read(bytes);

        //2.创建数据源
        DataSource dataSource = YamlShardingDataSourceFactory.createDataSource(bytes);

        /**
         * 3、获取连接，执行sql
         */
        String sql = "insert into t_order (order_id,user_id,price) values (?,?,?)";
        try (Connection connection = dataSource.getConnection();
             PreparedStatement ps = connection.prepareStatement(sql);) {
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
        }
    }
}
```
<a name="TB6Ps"></a>
## 6、运行输出
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
<a name="NAQlo"></a>
## 7、yml中可以配置哪些？
有2种方法可以知道。
<a name="ka9HN"></a>
### 1）方法1：官方文档
[https://shardingsphere.apache.org/document/legacy/4.x/document/cn/manual/sharding-jdbc/configuration/config-yaml/](https://shardingsphere.apache.org/document/legacy/4.x/document/cn/manual/sharding-jdbc/configuration/config-yaml/)
<a name="Z2K21"></a>
### 2）方法2：看源码
yml配置的方式主要在下面代码中<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697986056902-b80d1b6f-695a-4272-8470-048d02fb024d.png#averageHue=%23d0bba3&clientId=u7f52f017-23b3-4&from=paste&height=92&id=u5062bd6b&originHeight=229&originWidth=2139&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=141179&status=done&style=none&taskId=uc30a13cd-7fd6-4d81-9dbc-7b2ec495f63&title=&width=855.6)<br />进入`org.apache.shardingsphere.shardingjdbc.api.yaml.YamlShardingDataSourceFactory#createDataSource(byte[])`源码，如下，这个类，会读取yaml配置，将其转换为`YamlRootShardingConfiguration`，那么看这个类就知道yaml中如何配置了。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697986027442-528546dd-293e-4116-a27e-4f49716adf1e.png#averageHue=%23e2ceb1&clientId=u7f52f017-23b3-4&from=paste&height=106&id=u3cc1d4b1&originHeight=264&originWidth=2177&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=318905&status=done&style=none&taskId=u0cc48c89-65a8-4e82-8191-73736bd9e9e&title=&width=870.8)<br />进入`YamlRootShardingConfiguration`中看看，如下，就是普通的javabean，这个类中有的属性，都是yaml中可以配置的类容，很简单，大家自行摸索摸索。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697985949689-4a9a8f31-37da-4fcc-b15e-5d1641cb0d99.png#averageHue=%23fdfcfb&clientId=u7f52f017-23b3-4&from=paste&height=171&id=u379c1227&originHeight=427&originWidth=2177&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=218564&status=done&style=none&taskId=u1baee98e-a509-4ef1-9710-b365e5762a9&title=&width=870.8)
