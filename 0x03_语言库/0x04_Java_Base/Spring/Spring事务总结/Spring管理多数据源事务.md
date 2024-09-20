JavaSpring<br />Spring中通过事务管理器来控制事务，每个数据源都需要指定一个事务管理器，如果项目中需要操作多个数据库，那么需要配置多个数据源，也就需要配置多个数据管理器。
<a name="hTBkL"></a>
## 多数据源事务使用2个步骤
<a name="NJU7r"></a>
### 1、为每个数据源定义一个事务管理器
如下面代码，有2个数据源分别连接数据库ds1和ds2，然后为每个数据源定义了1个事务管理器，此时Spring容器中有2个数据源和2个事务管理器。
```java
//数据源1
@Bean
public DataSource dataSource1() {
    org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
    dataSource.setDriverClassName("com.mysql.jdbc.Driver");
    dataSource.setUrl("jdbc:mysql://localhost:3306/ds1?characterEncoding=UTF-8");
    dataSource.setUsername("root");
    dataSource.setPassword("root123");
    dataSource.setInitialSize(5);
    return dataSource;
}

//事务管理器1，对应数据源1
@Bean
public PlatformTransactionManager transactionManager1(@Qualifier("dataSource1")DataSource dataSource) {
    return new DataSourceTransactionManager(dataSource);
}

//数据源2
@Bean
public DataSource dataSource2() {
    org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
    dataSource.setDriverClassName("com.mysql.jdbc.Driver");
    dataSource.setUrl("jdbc:mysql://localhost:3306/ds2?characterEncoding=UTF-8");
    dataSource.setUsername("root");
    dataSource.setPassword("root123");
    dataSource.setInitialSize(5);
    return dataSource;
}

//事务管理器2，对应数据源2
@Bean
public PlatformTransactionManager transactionManager2(@Qualifier("dataSource2")DataSource dataSource) {
    return new DataSourceTransactionManager(dataSource);
}
```
<a name="RXmCW"></a>
### 2、指定事务的管理器bean名称
使用`@Transactional`中时，需通过`@Transactional`注解的`value`或`transactionManager`属性指定事务管理器bean名称，如：
```java
@Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
public void required(String name) {
    this.jdbcTemplate1.update("insert into user1(name) VALUES (?)", name);
}
```
这里补充一下，之前使用`@Transactional`的时候，并没有通过`value`或者`transactionManager`设置事务管理器，这是为什么？<br />这是因为在Spring容器中只定义了一个事务管理器，Spring启动事务的时候，默认会按类型在容器中查找事务管理器，刚好容器中只有一个，就拿过来用了，如果有多个的时候，如果不指定，Spring是不知道具体要用哪个事务管理器的。<br />多数据源事务的使用就这么简单，下面来看案例，案例才是精华。
<a name="LuPmu"></a>
## 事务管理器运行过程
这里先解释一下`REQUIRED`传播行为下，事务管理器的大致的运行过程，方便理解后面的案例代码。
```java
// Service1中：
@Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
public void m1(){
    this.jdbcTemplate1.update("insert into user1(name) VALUES ('张三')");
    service2.m2();
}

// Service2中：
@Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
public void m2(){
    this.jdbcTemplate1.update("insert into user1(name) VALUES ('李四')");
}
```
Spring事务中有个resources的ThreadLocal，static修饰的，用来存放共享的资源，稍后过程中会用到。
```java
private static final ThreadLocal<Map<Object, Object>> resources = new NamedThreadLocal<>("Transactional resources");
```
下面看m1方法简化版的事务过程：<br />1、`TransactionInterceptor`拦截m1方法<br />2、获取m1方法的事务配置信息：事务管理器bean名称：transactionManager1，事务传播行为：REQUIRED<br />3、从Spring容器中找到事务管理器transactionManager1，然后问一下transactionManager1，当前上下文中有没有事务，显然现在是没有的<br />4、创建一个新的事务
```java
//获取事务管理器对应的数据源，即dataSource1
DataSource dataSource1 = transactionManager1.getDataSource();
//即从dataSource1中获取一个连接
Connection conn = transactionManager1.dataSource1.getConnection();
//开启事务手动提交
conn.setAutoCommit(false);
//将dataSource1->conn放入map中
map.put(dataSource1,conn);
//将map丢到上面的resources ThreadLocal中
resources.set(map);
```
5、下面来带m1放的第一行代码：`this.jdbcTemplate1.update("insert into user1(name) VALUES ('张三')");`<br />6、jdbctemplate内部需要获取数据连接，获取连接的过程
```java
//从resources这个ThreadLocal中获取到map
Map map = resources.get();
//通过jdbcTemplate1.datasource从map看一下没有可用的连接
Connection conn = map.get(jdbcTemplate1.datasource);
//如果从map没有找到连接，那么重新从jdbcTemplate1.datasource中获取一个
//大家应该可以看出来，jdbcTemplate1和transactionManager1指定的是同一个dataSource，索引这个地方conn是不为null的
if(conn==null){
    conn = jdbcTemplate1.datasource.getConnection();
}
```
7、通过上面第6步获取的conn执行db操作，插入张三<br />8、下面来到m1方法的第2行代码：service2.m2();<br />9、m2方法上面也有`@Transactional`，`TransactionInterceptor`拦截m2方法<br />10、获取m2方法的事务配置信息：事务管理器bean名称：transactionManager1，事务传播行为：REQUIRED<br />11、从Spring容器中找到事务管理器transactionManager1，然后问一下transactionManager1，当前上下文中有没有事务，显然是是有的，m1开启的事务正在执行中，所以m2方法就直接加入这个事务了<br />12、下面来带m2放的第一行代码：`this.jdbcTemplate1.update("insert into user1(name) VALUES ('李四')");`<br />13、jdbctemplate内部需要获取数据连接，获取连接的过程
```java
//从resources这个ThreadLocal中获取到map
Map map = resources.get();
//通过jdbcTemplate1.datasource从map看一下没有可用的连接
Connection conn = map.get(jdbcTemplate1.datasource);
//如果从map没有找到连接，那么重新从jdbcTemplate1.datasource中获取一个
//大家应该可以看出来，jdbcTemplate1和transactionManager1指定的是同一个dataSource，索引这个地方conn是不为null的
if(conn==null){
    conn = jdbcTemplate1.datasource.getConnection();
}
```
14、通过第13步获取的conn执行db操作，插入李四<br />15、最终`TransactionInterceptor`发现2个方法都执行完毕了，没有异常，执行事务提交操作，如下
```java
//获取事务管理器对应的数据源，即dataSource1
DataSource dataSource1 = transactionManager1.getDataSource();
//从resources这个ThreadLocal中获取到map
Map map = resources.get();
//通过map拿到事务管理器开启的连接
Connection conn = map.get(dataSource1);
//通过conn提交事务
conn.commit();
//管理连接
conn.close();
```
16、清理`ThreadLocal`中的连接：通过`map.remove(dataSource1)`将连接从resource ThreadLocal中移除<br />17、清理事务<br />**从上面代码中可以看出：整个过程中有2个地方需要用到数据库连接Connection对象，第1个地方是：Spring事务拦截器启动事务的时候会从datasource中获取一个连接，通过这个连接开启事务手动提交，第2个地方是：最终执行sql操作的时候，也需要用到一个连接。那么必须确保这两个连接必须是同一个连接的时候，执行sql的操作才会受Spring事务控制，那么如何确保这2个是同一个连接呢？从代码中可以看出必须让事务管理器中的datasource和JdbcTemplate中的datasource必须是同一个，那么最终2个连接就是同一个对象。**<br />**什么是事务挂起操作？**<br />这里以事务传播行为`REQUIRED_NEW`为例说明一下，`REQUIRED_NEW`表示不管当前事务管理器中是否有事务，都会重新开启一个事务，如果当前事务管理器中有事务，会把当前事务挂起。<br />所谓挂起，可以这么理解：**对当前存在事务的现场生成一个快照，然后将事务现场清理干净，然后重新开启一个新事务，新事务执行完毕之后，将事务现场清理干净，然后再根据前面的快照恢复旧事务**。<br />下面再回到本文的内容，多数据源事务管理。
<a name="rFwW1"></a>
## 事务管理器如何判断当前是否有事务？
简化版的过程如下：
```java
Map map=resource的ThreadLocal.get();
DataSource datasource = transactionManager.getDataSource();
Connection conn = map.get(datasource);
//如果conn不为空，就表示当前有事务
if(conn!=null){
}
```
**从这段代码可以看出：判断是否存在事务，主要和datasource有关，和事务管理器无关，即使是不同的事务管理器，只要事务管理器的datasource是一样的，那么就可以发现当前存在的事务。**<br />事务管理器的运行过程和如何判断是否有事务，这2点大家一定要理解，这个理解了，后面的案例理解起来会容易很多。<br />**下面上案例。**
<a name="zKYO1"></a>
## 案例1
<a name="bkOL8"></a>
### 准备案例代码
<a name="Prtsk"></a>
#### 1、准备db
2个数据库：ds1、ds2<br />每个库中2个表：user1、user2
```sql
DROP DATABASE IF EXISTS ds1;
CREATE DATABASE if NOT EXISTS ds1;

USE ds1;
DROP TABLE IF EXISTS user1;
CREATE TABLE user1(
  id int PRIMARY KEY AUTO_INCREMENT,
  name varchar(64) NOT NULL DEFAULT '' COMMENT '姓名'
);

DROP TABLE IF EXISTS user2;
CREATE TABLE user2(
  id int PRIMARY KEY AUTO_INCREMENT,
  name varchar(64) NOT NULL DEFAULT '' COMMENT '姓名'
);

DROP DATABASE IF EXISTS ds2;
CREATE DATABASE if NOT EXISTS ds2;

USE ds2;
DROP TABLE IF EXISTS user1;
CREATE TABLE user1(
  id int PRIMARY KEY AUTO_INCREMENT,
  name varchar(64) NOT NULL DEFAULT '' COMMENT '姓名'
);

DROP TABLE IF EXISTS user2;
CREATE TABLE user2(
  id int PRIMARY KEY AUTO_INCREMENT,
  name varchar(64) NOT NULL DEFAULT '' COMMENT '姓名'
);
```
<a name="PsrNi"></a>
#### 2、Spring配置类
**定义2个数据源**：dataSource1、dataSource2，分别用来连接数据库ds1和ds2<br />**定义2个JdbcTemplate**：jdbcTemplate1、jdbcTemplate2，分别关联dataSource1和dataSource2<br />**2个数据源对应2个事务管理器**：transactionManager1、transactionManager2，分别用来管理2个数据源的事务<br />**6个bean的名称**

| **数据源** | **JdbcTemplate** | **事务管理器** |
| --- | --- | --- |
| dataSource1 | jdbcTemplate1 | transactionManager1 |
| dataSource2 | jdbcTemplate2 | transactionManager2 |

**源码如下：**
```java
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.sql.DataSource;

@EnableTransactionManagement //开启spring事务管理功能
@Configuration //指定当前类是一个spring配置类
@ComponentScan //开启bean扫描注册
public class MainConfig7 {
    //定义数据源1，连接数据库：ds1
    @Bean
    public DataSource dataSource1() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/ds1?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }

    //定义一个JdbcTemplate，对应数据源dataSource1，用来操作数据库：ds1
    @Bean
    public JdbcTemplate jdbcTemplate1(@Qualifier("dataSource1") DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    //定义事务管理器transactionManager1，对应数据源dataSource1，用来管理数据库ds1中的事务
    @Bean
    public PlatformTransactionManager transactionManager1(@Qualifier("dataSource1") DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }

    //定义数据源2，连接数据库：ds2
    @Bean
    public DataSource dataSource2() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/ds2?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }

    //定义一个JdbcTemplate，对应数据源dataSource2，用来操作数据库：ds2
    @Bean
    public JdbcTemplate jdbcTemplate2(@Qualifier("dataSource2") DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    //定义事务管理器transactionManager2，对应数据源dataSource2，用来管理数据库ds2中的事务
    @Bean
    public PlatformTransactionManager transactionManager2(@Qualifier("dataSource2") DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }
}
```
下面来定义4个service，分别用来操作2个库中的4张表。
<a name="KWZEa"></a>
#### 3、Ds1User1Service
用来操作**ds1.user1**表，注意下面代码中`@Transactional`注解中transactionManager的值为**transactionManager1**
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class Ds1User1Service {
    @Autowired
    private JdbcTemplate jdbcTemplate1;

    @Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
    public void required(String name) {
        this.jdbcTemplate1.update("insert into user1(name) VALUES (?)", name);
    }

}
```
同样，再来定义操作另外3个表的service
<a name="io7cA"></a>
#### 4、Ds1User2Service
用来操作**ds1.user2**表，事务管理器也是**transactionManager1**
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class Ds1User2Service {
    @Autowired
    private JdbcTemplate jdbcTemplate1;

    @Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
    public void required(String name) {
        this.jdbcTemplate1.update("insert into user2(name) VALUES (?)", name);
    }

}
```
<a name="UHPrf"></a>
#### 5、Ds2User1Service
用来操作**ds2.user1**表，对应事务管理器**transactionManager2**
```java
package com.javacode2018.tx.demo7;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class Ds2User1Service {
    @Autowired
    private JdbcTemplate jdbcTemplate2;

    @Transactional(transactionManager = "transactionManager2", propagation = Propagation.REQUIRED)
    public void required(String name) {
        this.jdbcTemplate2.update("insert into user1(name) VALUES (?)", name);
    }

}
```
<a name="iKdJF"></a>
#### 6、Ds2User2Service
用来操作**ds2.user2**表，对应事务管理器**transactionManager2**
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class Ds2User2Service {
    @Autowired
    private JdbcTemplate jdbcTemplate2;

    @Transactional(transactionManager = "transactionManager2", propagation = Propagation.REQUIRED)
    public void required(String name) {
        this.jdbcTemplate2.update("insert into user2(name) VALUES (?)", name);
    }

}
```
<a name="OmTeH"></a>
#### 7、Tx1Service
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Tx1Service {
    @Autowired
    private Ds1User1Service ds1User1Service;
    @Autowired
    private Ds1User2Service ds1User2Service;
}
```
<a name="Rja81"></a>
#### 8、Tx2Service
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class Tx2Service {
    @Autowired
    private Ds2User1Service ds2User1Service;
    @Autowired
    private Ds2User2Service ds2User2Service;
}
```
<a name="c9kQI"></a>
#### 9、测试类Demo7Test
```java
import org.junit.After;
import org.junit.Before;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;

public class Demo7Test {

    private Tx1Service txService1;
    private JdbcTemplate jdbcTemplate1;
    private JdbcTemplate jdbcTemplate2;

    //@Before标注的方法会在任意@Test方法执行之前执行，在这里清理一下2库中4张表的数据
    @Before
    public void before() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig7.class);
        txService1 = context.getBean(Tx1Service.class);
        this.jdbcTemplate1 = context.getBean("jdbcTemplate1", JdbcTemplate.class);
        this.jdbcTemplate2 = context.getBean("jdbcTemplate2", JdbcTemplate.class);
        jdbcTemplate1.update("truncate table ds1.user1");
        jdbcTemplate1.update("truncate table ds1.user2");
        jdbcTemplate2.update("truncate table ds2.user1");
        jdbcTemplate2.update("truncate table ds2.user2");
    }

    //@After标注的方法会在任意@Test方法执行完毕之后执行，在此处输出4张表的数据，用来查看测试案例之后，表中的数据清空
    @After
    public void after() {
        System.out.println("ds1.user1表数据：" + this.jdbcTemplate1.queryForList("SELECT * from user1"));
        System.out.println("ds1.user2表数据：" + this.jdbcTemplate1.queryForList("SELECT * from user2"));
        System.out.println("ds2.user1表数据：" + this.jdbcTemplate2.queryForList("SELECT * from user1"));
        System.out.println("ds2.user2表数据：" + this.jdbcTemplate2.queryForList("SELECT * from user2"));
    }


}
```
<a name="p4XwM"></a>
### 代码验证
<a name="taGDa"></a>
#### 1、场景1
外围方法和内部方法使用相同的事务管理器，传播行为都是`REQUIRED`。
<a name="TUR71"></a>
##### Tx1Service中添加代码
```java
@Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
public void test1() {
    this.ds1User1Service.required("张三");
    this.ds1User2Service.required("李四");
    throw new RuntimeException();
}
```
方法、事务管理器、事务管理器对应数据源、操作db的jdbctemplate中数据源对应关系。

| **方法** | **事务管理器** | **事务管理器对应数据源** | **jdbctemplate对应数据源** |
| --- | --- | --- | --- |
| test1 | transactionManager1 | datasource1 | - |
| ds1User1Service.required | transactionManager1 | datasource1 | datasource1 |
| this.ds1User2Service.required | transactionManager1 | datasource1 | datasource1 |

<a name="kCBua"></a>
##### Demo7Test中添加测试用例
```java
@Test
public void test1() {
    this.txService1.test1();
}
```
<a name="IeNq6"></a>
##### 运行输出
```
ds1.user1表数据：[]
ds1.user2表数据：[]
ds2.user1表数据：[]
ds2.user2表数据：[]
```
<a name="jTlvn"></a>
##### 结论分析
| **数据库结果** | **结果分析** |
| --- | --- |
| “张三”、“李四”均为插入 | 外围方法和内部方法使用同一个事务管理器transactionManager1，且事务管理器和jdbctemplate的datasource都是同一个，外围方法会开启事务，内部方法加入外围方法事务，外围方法弹出异常导致事务回滚，内部方法跟着回滚了。 |

<a name="KlExA"></a>
#### 2、场景2
外部方法和内部方法使用不同的事务管理器。
<a name="tVNyL"></a>
##### Tx1Service中添加代码
```java
@Transactional(transactionManager = "transactionManager2", propagation = Propagation.REQUIRED)
public void test2() {
    this.ds1User1Service.required("张三");
    this.ds1User2Service.required("李四");
    throw new RuntimeException();
}
```
方法、事务管理器、事务管理器对应数据源、操作db的jdbctemplate中数据源对应关系。

| **方法** | **事务管理器** | **事务管理器对应数据源** | **jdbctemplate对应数据源** |
| --- | --- | --- | --- |
| test2 | transactionManager2 | datasource2 | - |
| ds1User1Service.required(“张三”); | transactionManager1 | datasource1 | datasource1 |
| this.ds1User2Service.required(“李四”); | transactionManager1 | datasource1 | datasource1 |

<a name="rZgL2"></a>
##### Demo7Test中添加测试用例
```java
@Test
public void test2() {
    this.txService1.test2();
}
```
<a name="MXgTR"></a>
##### 运行输出
```
ds1.user1表数据：[{id=1, name=张三}]
ds1.user2表数据：[{id=1, name=李四}]
ds2.user1表数据：[]
ds2.user2表数据：[]
```
<a name="dYbcc"></a>
##### 结论分析
| **数据库结果** | **结果分析** |
| --- | --- |
| “张三”、”李四”均插入 | 外围方法test2和内部两个required方法用到的不是同一个事务管理器，内部的2个方法在各自的事务中执行，不受外部方法事务的控制。 |

<a name="goyAb"></a>
#### 3、场景3
<a name="wbBgt"></a>
##### Tx1Service中添加代码
```java
@Autowired
private Ds2User1Service ds2User1Service;
@Autowired
private Ds2User2Service ds2User2Service;

@Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
public void test3() {
    this.ds1User1Service.required("张三");
    this.ds1User2Service.required("李四");
    this.ds2User1Service.required("王五");
    this.ds2User2Service.required("赵六");
    throw new RuntimeException();
}
```
方法、事务管理器、事务管理器对应数据源、操作db的jdbctemplate中数据源对应关系。

| **方法** | **事务管理器** | **事务管理器对应数据源** | **jdbctemplate对应数据源** |
| --- | --- | --- | --- |
| test3 | transactionManager1 | datasource1 | - |
| this.ds1User1Service.required(“张三”); | transactionManager1 | datasource1 | datasource1 |
| this.ds1User2Service.required(“李四”); | transactionManager1 | datasource1 | datasource1 |
| this.ds2User1Service.required(“王五”); | transactionManager2 | datasource2 | datasource2 |
| this.ds2User2Service.required(“赵六”); | transactionManager2 | datasource2 | datasource2 |

<a name="gTiF0"></a>
##### Demo7Test中添加测试用例
```java
@Test
public void test3() {
    this.txService1.test3();
}
```
<a name="KLwOh"></a>
##### 运行输出
```
ds1.user1表数据：[]
ds1.user2表数据：[]
ds2.user1表数据：[{id=1, name=王五}]
ds2.user2表数据：[{id=1, name=赵六}]
```
<a name="smMxO"></a>
##### 结论分析
“张三”、”李四”都未插入，”王五”、“赵六”插入成功。<br />外围方法和内部的前2个required方法事务管理器都是transactionManager1，所以他们3个在一个事务中执行；而内部的后2个required方法事务管理器是transactionManager2，他们分别在自己的事务中执行，不受外围方法事务的控制，外围方法感受到了异常，回滚事务，只会导致内部的前2个required方法回滚。
<a name="zV3tV"></a>
#### 4、场景4
<a name="AXHrH"></a>
##### Tx2Service中加入代码
```java
@Transactional(transactionManager = "transactionManager2", propagation = Propagation.REQUIRED)
public void test1() {
    this.ds2User1Service.required("王五");
    this.ds2User2Service.required("赵六");
}
```
<a name="F1o5P"></a>
##### Tx1Service中加入代码
```java
@Autowired
private Tx2Service tx2Service;

@Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
public void test4() {
    this.ds1User1Service.required("张三");
    this.ds1User2Service.required("李四");
    this.tx2Service.test1();
    throw new RuntimeException();
}
```
方法、事务管理器、事务管理器对应数据源、操作db的jdbctemplate中数据源对应关系。

| **方法** | **事务管理器** | **事务管理器对应数据源** | **jdbctemplate对应数据源** |
| --- | --- | --- | --- |
| test4 | transactionManager1 | datasource1 | - |
| this.ds1User1Service.required | transactionManager1 | datasource1 | datasource1 |
| this.ds1User2Service.required | transactionManager1 | datasource1 | datasource1 |
| this.tx2Service.test1() | transactionManager2 | datasource2 | - |
| this.ds2User1Service.required | transactionManager2 | datasource2 | datasource2 |
| this.ds2User2Service.required | transactionManager2 | datasource2 | datasource2 |

<a name="p3LDF"></a>
##### Demo7Test中添加测试用例
```java
@Test
public void test4() {
    this.txService1.test4();
}
```
<a name="kYMBg"></a>
##### 运行输出
```
ds1.user1表数据：[]
ds1.user2表数据：[]
ds2.user1表数据：[{id=1, name=王五}]
ds2.user2表数据：[{id=1, name=赵六}]
```
<a name="Ouj2M"></a>
##### 结论分析
“张三”、”李四”都未插入，”王五”、“赵六”插入成功。<br />分析一下过程<br />1、test4在事务管理器transactionManager1中开启事务tm1，并将连接放入resourceThreadLocal中(datasource1->conn1)<br />2、`this.ds1User1Service.required("张三")`事务管理器是transactionManager1，所以会加入事务tm1中，通过jdbctemplate1插入张三，由于jdbctemplate1.datasource是datasource1，所以会获取到threadlocal中的conn1来插入数据<br />3、`this.ds1User2Service.required("李四")`事务管理器是transactionManager1，所以会加入事务tm1中，通过jdbctemplate1插入张三，由于jdbctemplate1.datasource是datasource1，所以会获取到threadlocal中的conn1来插入数据<br />4、执行this.tx2Service.test1()，这个方法事务管理器是transactionManager2，所以会重新开启一个事务tm2，并将连接放入`resourceThreadLocal`中(datasource2->conn2)<br />5、`this.ds2User1Service.required("王五")`事务管理器是transactionManager2，通过所以会加入事务tm2中，通过jdbctemplate2插入王五，由于jdbctemplate2.datasource是datasource2，所以会获取到threadlocal中的conn2来插入数据<br />6、`this.ds2User2Service.required("赵六")`事务管理器是transactionManager2，所以会加入事务tm2中，通过jdbctemplate2插入赵六，由于jdbctemplate2.datasource是datasource2，所以会获取到threadlocal中的conn2来插入数据<br />7、tm2提交<br />8、tm1发现test4抛出异常，tm1执行回滚
<a name="K8MRm"></a>
#### 5、场景5
<a name="JftUu"></a>
##### Tx2Service中加入代码
```java
@Transactional(transactionManager = "transactionManager2", propagation = Propagation.REQUIRED)
public void test2() {
    this.ds2User1Service.required("王五");
    this.ds2User2Service.required("赵六");
    throw new RuntimeException();
}
```
<a name="CiQyC"></a>
##### Tx1Service中加入代码
```java
@Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
public void test5() {
    this.ds1User1Service.required("张三");
    this.ds1User2Service.required("李四");
    this.tx2Service.test2();
}
```
方法、事务管理器、事务管理器对应数据源、操作db的jdbctemplate中数据源对应关系。

| **方法** | **事务管理器** | **事务管理器对应数据源** | **jdbctemplate对应数据源** |
| --- | --- | --- | --- |
| test4 | transactionManager1 | datasource1 | - |
| this.ds1User1Service.required | transactionManager1 | datasource1 | datasource1 |
| this.ds1User2Service.required | transactionManager1 | datasource1 | datasource1 |
| this.tx2Service.test1() | transactionManager2 | datasource2 | - |
| this.ds2User1Service.required | transactionManager2 | datasource2 | datasource2 |
| this.ds2User2Service.required | transactionManager2 | datasource2 | datasource2 |

<a name="uO9jy"></a>
##### Demo7Test中添加测试用例
```java
@Test
public void test5() {
    this.txService1.test5();
}
```
<a name="MMUQp"></a>
##### 运行输出
```
ds1.user1表数据：[]
ds1.user2表数据：[]
ds2.user1表数据：[]
ds2.user2表数据：[]
```
<a name="h7PQB"></a>
##### 结论分析
4个表都未插入数据。<br />外围方法test5通过事务管理器transactionManager1开启了事务tm1，内部方法插入“张三”，“李四”加入了tm1事务，而test2通过事务管理器transactionManager2又开启了一个事务tm2，test2内部方法插入“王五”，“赵六”加入了tm2事务，test2内部抛出了异常，tm2和tm1都感受到了这个异常，所以2个事务都进行了回滚操作。
<a name="rScMw"></a>
## 案例2
<a name="zUDpb"></a>
### Spring配置类
```java
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.sql.DataSource;

@EnableTransactionManagement //开启spring事务管理功能
@Configuration //指定当前类是一个spring配置类
@ComponentScan //开启bean扫描注册
public class MainConfig8 {
    //定义数据源1，连接数据库：ds1
    @Bean
    public DataSource dataSource1() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/ds1?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }

    //定义一个jdbcTemplate1
    @Bean
    public JdbcTemplate jdbcTemplate1(DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    //定义事务管理器transactionManager1
    @Bean
    public PlatformTransactionManager transactionManager1(DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }

    //定义jdbcTemplate2
    @Bean
    public JdbcTemplate jdbcTemplate2(DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    //定义事务管理器transactionManager2
    @Bean
    public PlatformTransactionManager transactionManager2(DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }
}
```
上面代码中

- 定义了1个数据源：dataSource1
- 2个jdbctemplate：jdbcTemplate1和jdbcTemplate2，他们的datasource都是dataSource1
- 2个事务管理器：transactionManager1和transactionManager2，他们的datasource都是dataSource1

有同学发现这样写是不是很奇怪，不是说一个数据源定义一个事务管理器么，这什么操作？<br />不急，这样写，是为了让你更深入了解其原理。
<a name="EXYQT"></a>
### User2Service
内部的required方法操作db用的是jdbcTemplate2，事务管理器为transactionManager2
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class User2Service {
    @Autowired
    private JdbcTemplate jdbcTemplate2;

    @Transactional(transactionManager = "transactionManager2", propagation = Propagation.REQUIRED)
    public void required() {
        this.jdbcTemplate2.update("insert into user2(name) VALUES (?)", "李四");
    }

}
```
<a name="eBYtM"></a>
### User1Service
内部的`required`方法操作db用的是jdbcTemplate1，事务管理器为transactionManager1，并且会调用user2Service的`required`方法。
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

@Component
public class User1Service {
    @Autowired
    private JdbcTemplate jdbcTemplate1;

    @Autowired
    private User2Service user2Service;

    @Transactional(transactionManager = "transactionManager1", propagation = Propagation.REQUIRED)
    public void required() {
        this.jdbcTemplate1.update("insert into user1(name) VALUES (?)", "张三");
        this.user2Service.required();
        throw new RuntimeException();
    }

}
```
大家觉得required方法执行完毕之后，会是什么结果？
```
A：张三未插入、李四插入成功
B：张三、李四均为插入
```
大家先思考一下，先别看下面的执行结果，可以参考事务管理器的执行过程分析一下结果。<br />好了，上测试用例。
<a name="Bvmq8"></a>
### Demo8Test
```java
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;

public class Demo8Test {

    private User1Service user1Service;
    private JdbcTemplate jdbcTemplate1;

    //@Before标注的方法会在任意@Test方法执行之前执行，在这里清理一下2库中4张表的数据
    @Before
    public void before() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig8.class);
        this.user1Service = context.getBean(User1Service.class);
        this.jdbcTemplate1 = context.getBean("jdbcTemplate1", JdbcTemplate.class);
        jdbcTemplate1.update("truncate table ds1.user1");
        jdbcTemplate1.update("truncate table ds1.user2");
    }

    //@After标注的方法会在任意@Test方法执行完毕之后执行，在此处输出4张表的数据，用来查看测试案例之后，表中的数据清空
    @After
    public void after() {
        System.out.println("ds1.user1表数据：" + this.jdbcTemplate1.queryForList("SELECT * from user1"));
        System.out.println("ds1.user2表数据：" + this.jdbcTemplate1.queryForList("SELECT * from user2"));
    }

    @Test
    public void test1() {
        this.user1Service.required();
    }

}
```
<a name="ZoeX0"></a>
### 运行输出
```
ds1.user1表数据：[]
ds1.user2表数据：[]
```
结果是都没有插入。
<a name="IqFOQ"></a>
### 结果分析
分析一下执行过程<br />1、`this.user1Service.required();`<br />2、事务拦截器拦截`user1Service.required()`方法，事务配置信息：（事务管理器：transactionManager1，传播行为REQUIRED）<br />3、问一下transactionManager1，当前是否有事务，transactionManager2看了一下，发现没有，那么重新创建一个事务tm1，通过transactionManager1中的datasource，即datasource1重新获取一个连接：conn1，然后丢到`resourceThreadLocal`中(datasource1->conn1)<br />4、执行`this.jdbcTemplate1.update("insert into user1(name) VALUES (?)", "张三")`，由于jdbcTemplate1中的datasource是datasource1，所以会从`resourceThreadLocal`中拿到conn1连接来执行sql<br />5、执行`this.user2Service.required();`<br />6、事务拦截器拦截`user1Service.required()`方法，事务配置信息：（事务管理器：`transactionManager2`，传播行为REQUIRED）<br />7、问一下transactionManager2，当前是否有事务？大家在回头看一下事务管理器是如何判断当前是否有事务的，由于transactionManager2和transactionManager1用到的都是datasource1，所以transactionManager2会发现当前是存在事务的，即tm1<br />8、执行`this.jdbcTemplate2.update("insert into user2(name) VALUES (?)", "李四")`，由于jdbcTemplate2中的datasource也是datasource1，所以会从`resourceThreadLocal`中拿到conn1连接来执行sql<br />9、最终整个操作过程中只有一个事务tm1，一个连接conn1，通过conn1执行2个插入操作<br />10、执行`throw new RuntimeException();`抛出异常<br />11、tm1感受到了异常，所以会执行回滚操作，最终都插入失败
<a name="z2jfW"></a>
## 总结一下
1、本文介绍了多数据源事务的使用，2个步骤：先为每个数据源定义一个事务管理器，然后在`@Transactional`中指定具体要使用哪个事务管理器。<br />2、**事务管理器运行过程、事务管理器如何判断当前是否有事务，这2点非常非常重要，可以再看一下**。
