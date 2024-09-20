JavaSpring
<a name="rRZeq"></a>
## 什么是事务传播行为？
事务的传播行为用来描述：系统中的一些方法交由Spring来管理事务，当这些方法之间出现嵌套调用的时候，事务所表现出来的行为是什么样的？<br />比如下面2个类，Service1中的m1方法和Service2中的m2方法上面都有`@Transactional`注解，说明这2个方法由Spring来控制事务。<br />但是注意m1中2行代码，先执行了一个insert，然后调用service2中的m2方法，service2中的m2方法也执行了一个insert。<br />那么大家觉得这2个insert会在一个事务中运行么？也就是说此时事务的表现行为是什么样的呢？这个就是Spring事务的传播行为来控制的事情，不同的传播行为，表现会不一样，可能他们会在一个事务中执行，也可能不会在一个事务中执行，这就需要看传播行为的配置了。
```java
@Component
public class Service1 {
    @Autowired
    private Service2 service2;

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Transactional
    public void m1() {
        this.jdbcTemplate.update("INSERT into t1 values ('m1')");
        this.service2.m2();
    }
}

@Component
public class Service2 {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Transactional
    public void m2() {
        this.jdbcTemplate.update("INSERT into t1 values ('m2')");
    }
}
```
<a name="IvjPI"></a>
## 如何配置事务传播行为？
通过`@Transactional`注解中的`propagation`属性来指定事务的传播行为：
```java
Propagation propagation() default Propagation.REQUIRED;
```
<a name="BHKro"></a>
### 7种传播行为
`Propagation`是个枚举，有7种值，如下：

| **事务传播行为类型** | **说明** |
| --- | --- |
| `**REQUIRED**` | 如果当前事务管理器中没有事务，就新建一个事务，如果已经存在一个事务中，加入到这个事务中。这是最常见的选择，是默认的传播行为。 |
| `**SUPPORTS**` | 支持当前事务，如果当前事务管理器中没有事务，就以非事务方式执行。 |
| `**MANDATORY**` | 使用当前的事务，如果当前事务管理器中没有事务，就抛出异常。 |
| `**REQUIRES_NEW**` | 新建事务，如果当前事务管理器中存在事务，把当前事务挂起，然后会新建一个事务。 |
| `**NOT_SUPPORTED**` | 以非事务方式执行操作，如果当前事务管理器中存在事务，就把当前事务挂起。 |
| `**NEVER**` | 以非事务方式执行，如果当前事务管理器中存在事务，则抛出异常。 |
| `**NESTED**` | 如果当前事务管理器中存在事务，则在嵌套事务内执行；如果当前事务管理器中没有事务，则执行与`PROPAGATION_REQUIRED`类似的操作。 |

**注意：这7种传播行为有个前提，他们的事务管理器是同一个的时候，才会有上面描述中的表现行为。**<br />下面通过案例对7中表现行为来做说明，在看案例之前，先来回顾几个知识点
<a name="XFSQr"></a>
### 1、Spring声明式事务处理事务的过程
Spring声明式事务是通过事务拦截器`TransactionInterceptor`拦截目标方法，来实现事务管理的功能的，事务管理器处理过程大致如下：
```
1、获取事务管理器
2、通过事务管理器开启事务
try{
    3、调用业务方法执行db操作
    4、提交事务
}catch(RuntimeException | Error){
    5、回滚事务
}
```
<a name="gMxD7"></a>
### 2、何时事务会回滚？
默认情况下，目标方法抛出`RuntimeException`或者Error的时候，事务会被回滚。
<a name="E2xeN"></a>
### 3、Spring事务管理器中的`Connection`和业务中操作db的`Connection`如何使用同一个的？
以`DataSourceTransactionManager`为事务管理器，操作db使用`JdbcTemplate`来说明一下。<br />创建`DataSourceTransactionManager`和`JdbcTemplate`的时候都需要指定dataSource，需要将他俩的dataSource指定为同一个对象。<br />当事务管理器开启事务的时候，会通过`dataSource.getConnection()`方法获取一个db连接`connection`，然后会将dataSource->connection丢到一个Map中，然后将map放到ThreadLocal中。<br />当`JdbcTemplate`执行sql的时候，以`JdbcTemplate.dataSource`去上面的`ThreadLocal`中查找，是否有可用的连接，如果有，就直接拿来用了，否则调用`JdbcTemplate.dataSource.getConnection()`方法获取一个连接来用。<br />所以Spring中可以确保事务管理器中的`Connection`和`JdbcTemplate`中操作db的`Connection`是同一个，这样才能确保Spring可以控制事务。
<a name="bYX3p"></a>
## 代码验证
<a name="k1Ekt"></a>
### 准备db
```sql
DROP DATABASE IF EXISTS javacode2018;
CREATE DATABASE if NOT EXISTS javacode2018;

USE javacode2018;
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
<a name="MoYzA"></a>
### Spring配置类MainConfig6
准备JdbcTemplate和事务管理器。
```java
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
public class MainConfig6 {
    //定义一个数据源
    @Bean
    public DataSource dataSource() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }

    //定义一个JdbcTemplate,用来执行db操作
    @Bean
    public JdbcTemplate jdbcTemplate(DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    //定义一个事务管理器
    @Bean
    public PlatformTransactionManager transactionManager(DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }
}
```
<a name="QJaax"></a>
### 来3个service
后面的案例中会在这3个service中使用Spring的事务来演示效果。
<a name="BvWo2"></a>
#### User1Service
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;

@Component
public class User1Service {
    @Autowired
    private JdbcTemplate jdbcTemplate;
}
```
<a name="lpfcP"></a>
#### User2Service
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;

@Component
public class User2Service {
    @Autowired
    private JdbcTemplate jdbcTemplate;
}
```
<a name="nYtYp"></a>
#### TxService
```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class TxService {
    @Autowired
    private User1Service user1Service;
    @Autowired
    private User2Service user2Service;
}
```
<a name="p7cAQ"></a>
### 测试用例Demo6Test
before方法会在每个`@Test`标注的方法之前执行一次，这个方法主要用来做一些准备工作：启动Spring容器、清理2个表中的数据；after方法会在每个`@Test`标注的方法执行完毕之后执行一次，在这个里面输出2个表的数据；方便查看的测试用例效果。
```java
import org.junit.Before;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class Demo6Test {

    private TxService txService;
    private JdbcTemplate jdbcTemplate;

    //每个@Test用例执行之前先启动一下spring容器，并清理一下user1、user2中的数据
    @Before
    public void before() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig6.class);
        txService = context.getBean(TxService.class);
        jdbcTemplate = context.getBean(JdbcTemplate.class);
        jdbcTemplate.update("truncate table user1");
        jdbcTemplate.update("truncate table user2");
    }

    @After
    public void after() {
        System.out.println("user1表数据：" + jdbcTemplate.queryForList("SELECT * from user1"));
        System.out.println("user2表数据：" + jdbcTemplate.queryForList("SELECT * from user2"));
    }

}
```
<a name="oG0ew"></a>
## 1、`REQUIRED`
<a name="DENmc"></a>
### User1Service
添加1个方法，事务传播行为：`REQUIRED`
```java
@Transactional(propagation = Propagation.REQUIRED)
public void required(String name) {
    this.jdbcTemplate.update("insert into user1(name) VALUES (?)", name);
}
```
<a name="DEBlf"></a>
### User2Service
添加2个方法，事务传播行为：REQUIRED，注意第2个方法内部最后一行会抛出一个异常。
```java
@Transactional(propagation = Propagation.REQUIRED)
public void required(String name) {
    this.jdbcTemplate.update("insert into user1(name) VALUES (?)", name);
}

@Transactional(propagation = Propagation.REQUIRED)
public void required_exception(String name) {
    this.jdbcTemplate.update("insert into user1(name) VALUES (?)", name);
    throw new RuntimeException();
}
```
<a name="F83sV"></a>
### 场景1（1-1）
外围方法没有事务，外围方法内部调用2个`REQUIRED`级别的事务方法。<br />案例中都是在TxService的方法中去调用另外2个service，所以TxService中的方法统称外围方法，另外2个service中的方法称内部方法。
<a name="nsaC4"></a>
#### 验证方法1
<a name="mo4tj"></a>
##### TxService添加
```java
public void notransaction_exception_required_required() {
    this.user1Service.required("张三");
    this.user2Service.required("李四");
    throw new RuntimeException();
}
```
<a name="UdxxS"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void notransaction_exception_required_required() {
    txService.notransaction_exception_required_required();
}
```
<a name="n2UXu"></a>
##### 运行输出
```
user1表数据：[{id=1, name=张三}]
user2表数据：[{id=1, name=李四}]
```
<a name="Odhv8"></a>
#### 验证方法2
<a name="G9RHY"></a>
##### TxService添加
```java
public void notransaction_required_required_exception() {
    this.user1Service.required("张三");
    this.user2Service.required_exception("李四");
}
```
<a name="Xo9x0"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void notransaction_required_required_exception() {
    txService.notransaction_required_required_exception();
}
```
<a name="B67da"></a>
##### 运行输出
```
user1表数据：[{id=1, name=张三}]
user2表数据：[]
```
<a name="VEJfp"></a>
#### 结果分析
| **验证方法序号** | **数据库结果** | **结果分析** |
| --- | --- | --- |
| 1 | “张三”、“李四”均插入。 | 外围方法未开启事务，插入“张三”、“李四”方法在自己的事务中独立运行，外围方法异常不影响内部插入“张三”、“李四”方法独立的事务。 |
| 2 | “张三”插入，“李四”未插入。 | 外围方法没有事务，插入“张三”、“李四”方法都在自己的事务中独立运行，所以插入“李四”方法抛出异常只会回滚插入“李四”方法，插入“张三”方法不受影响。 |

<a name="FaTcE"></a>
#### 结论
**通过这两个方法证明了在外围方法未开启事务的情况下**`**Propagation.REQUIRED**`**修饰的内部方法会新开启自己的事务，且开启的事务相互独立，互不干扰。**
<a name="NEgEQ"></a>
### 场景2（1-2）
外围方法开启事务（`Propagation.REQUIRED`），这个使用频率特别高。
<a name="SZYNt"></a>
#### 验证方法1
<a name="ckHmx"></a>
##### TxService添加
```java
@Transactional(propagation = Propagation.REQUIRED)
public void transaction_exception_required_required() {
    user1Service.required("张三");
    user2Service.required("李四");
    throw new RuntimeException();
}
```
<a name="yJkWN"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void transaction_exception_required_required() {
    txService.transaction_exception_required_required();
}
```
<a name="p9ZBL"></a>
##### 运行输出
```
user1表数据：[]
user2表数据：[]
```
<a name="MnjBf"></a>
#### 验证方法2
<a name="pCIcX"></a>
##### TxService添加
```java
@Transactional(propagation = Propagation.REQUIRED)
public void transaction_required_required_exception() {
    user1Service.required("张三");
    user2Service.required_exception("李四");
}
```
<a name="l47G1"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void transaction_required_required_exception() {
    txService.transaction_required_required_exception();
}
```
<a name="YbMUD"></a>
##### 运行输出
```
user1表数据：[]
user2表数据：[]
```
<a name="qkfIr"></a>
#### 验证方法3
<a name="p1NDr"></a>
##### TxService添加
```java
@Transactional(propagation = Propagation.REQUIRED)
public void transaction_required_required_exception_try() {
    user1Service.required("张三");
    try {
        user2Service.required_exception("李四");
    } catch (Exception e) {
        System.out.println("方法回滚");
    }
}
```
<a name="JeheJ"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void transaction_required_required_exception_try() {
    txService.transaction_required_required_exception_try();
}
```
<a name="mgMyd"></a>
##### 运行输出
```
方法回滚
user1表数据：[]
user2表数据：[]
```
<a name="XA5aD"></a>
#### 结果分析
| **验证方法序号** | **数据库结果** | **结果分析** |
| --- | --- | --- |
| 1 | “张三”、“李四”均未插入。 | 外围方法开启事务，内部方法加入外围方法事务，外围方法回滚，内部方法也要回滚。 |
| 2 | “张三”、“李四”均未插入。 | 外围方法开启事务，内部方法加入外围方法事务，内部方法抛出异常回滚，外围方法感知异常致使整体事务回滚。 |
| 3 | “张三”、“李四”均未插入。 | 外围方法开启事务，内部方法加入外围方法事务，内部方法抛出异常回滚，即使方法被catch不被外围方法感知，整个事务依然回滚。 |

<a name="jm5zR"></a>
#### 结论
**以上试验结果证明在外围方法开启事务的情况下**`**Propagation.REQUIRED**`**修饰的内部方法会加入到外围方法的事务中，所有**`**Propagation.REQUIRED**`**修饰的内部方法和外围方法均属于同一事务，只要一个方法回滚，整个事务均回滚。**
<a name="R6ZwP"></a>
## 2、`PROPAGATION_REQUIRES_NEW`
<a name="UrK04"></a>
### User1Service
添加1个方法，事务传播行为：`REQUIRES_NEW`
```java
@Transactional(propagation = Propagation.REQUIRES_NEW)
public void requires_new(String name) {
    this.jdbcTemplate.update("insert into user1(name) VALUES (?)", name);
}
```
<a name="pHgOR"></a>
### User2Service
添加2个方法，事务传播行为：`REQUIRES_NEW`，注意第2个方法内部最后一行会抛出一个异常。
```java
@Transactional(propagation = Propagation.REQUIRES_NEW)
public void requires_new(String name) {
    this.jdbcTemplate.update("insert into user2(name) VALUES (?)", name);
}

@Transactional(propagation = Propagation.REQUIRES_NEW)
public void requires_new_exception(String name) {
    this.jdbcTemplate.update("insert into user2(name) VALUES (?)", name);
    throw new RuntimeException();
}
```
<a name="OLxz5"></a>
### 场景1（2-1）
外围方法没有事务。
<a name="LrbDZ"></a>
#### 验证方法1
<a name="KfBfy"></a>
##### TxService添加
```java
public void notransaction_exception_requiresNew_requiresNew(){
    user1Service.requires_new("张三");
    user2Service.requires_new("李四");
    throw new RuntimeException();
}
```
<a name="fvvC2"></a>
##### Demo6Test中添加
```java
@Test
public void notransaction_exception_requiresNew_requiresNew() {
    txService.notransaction_exception_requiresNew_requiresNew();
}
```
<a name="Ydnlx"></a>
##### 运行输出
```
user1表数据：[{id=1, name=张三}]
user2表数据：[{id=1, name=李四}]
```
<a name="odX9a"></a>
#### 验证方法2
<a name="ohRjj"></a>
##### TxService添加
```java
public void notransaction_requiresNew_requiresNew_exception(){
    user1Service.requires_new("张三");
    user2Service.requires_new_exception("李四");
}
```
<a name="a8cgU"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void notransaction_requiresNew_requiresNew_exception() {
    txService.notransaction_requiresNew_requiresNew_exception();
}
```
<a name="zbMlO"></a>
##### 运行输出
```
user1表数据：[{id=1, name=张三}]
user2表数据：[]
```
<a name="Xnha4"></a>
#### 结果分析
| **验证方法序号** | **数据库结果** | **结果分析** |
| --- | --- | --- |
| 1 | “张三”插入，“李四”插入。 | 外围方法没有事务，插入“张三”、“李四”方法都在自己的事务中独立运行,外围方法抛出异常回滚不会影响内部方法。 |
| 2 | “张三”插入，“李四”未插入 | 外围方法没有开启事务，插入“张三”方法和插入“李四”方法分别开启自己的事务，插入“李四”方法抛出异常回滚，其他事务不受影响。 |

<a name="f6Pop"></a>
#### 结论
**通过这两个方法证明了在外围方法未开启事务的情况下**`**Propagation.REQUIRES_NEW**`**修饰的内部方法会新开启自己的事务，且开启的事务相互独立，互不干扰。**
<a name="BuNls"></a>
### 场景2（2-2）
外围方法开启事务。
<a name="KojYE"></a>
#### 验证方法1
<a name="TA2wr"></a>
##### TxService添加
```java
@Transactional(propagation = Propagation.REQUIRED)
public void transaction_exception_required_requiresNew_requiresNew() {
    user1Service.required("张三");
    user2Service.requires_new("李四");
    user2Service.requires_new("王五");
    throw new RuntimeException();
}
```
<a name="mBoA2"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void transaction_exception_required_requiresNew_requiresNew() {
    txService.transaction_exception_required_requiresNew_requiresNew();
}
```
<a name="RietT"></a>
##### 运行输出
```
user1表数据：[]
user2表数据：[{id=1, name=李四}, {id=2, name=王五}]
```
<a name="Q9266"></a>
#### 验证方法2
<a name="i8A0h"></a>
##### TxService添加
```java
@Transactional(propagation = Propagation.REQUIRED)
public void transaction_required_requiresNew_requiresNew_exception() {
    user1Service.required("张三");
    user2Service.requires_new("李四");
    user2Service.requires_new_exception("王五");
}
```
<a name="sh02n"></a>
##### Demo6Test中添加
```java
@Test
public void transaction_required_requiresNew_requiresNew_exception() {
    txService.transaction_required_requiresNew_requiresNew_exception();
}
```
<a name="kP6ZX"></a>
##### 运行输出
```
user1表数据：[]
user2表数据：[{id=1, name=李四}]
```
<a name="Z1x5J"></a>
#### 验证方法3
<a name="UikVV"></a>
##### TxService添加
```java
@Transactional(propagation = Propagation.REQUIRED)
public void transaction_required_requiresNew_requiresNew_exception_try(){
    user1Service.required("张三");
    user2Service.requires_new("李四");
    try {
        user2Service.requires_new_exception("王五");
    } catch (Exception e) {
        System.out.println("回滚");
    }
}
```
<a name="gSgyO"></a>
##### Demo6Test中添加
```java
@Test
public void transaction_required_requiresNew_requiresNew_exception_try() {
    txService.transaction_required_requiresNew_requiresNew_exception_try();
}
```
<a name="Inxc3"></a>
##### 运行输出
```
回滚
user1表数据：[{id=1, name=张三}]
user2表数据：[{id=1, name=李四}]
```
<a name="FRYaL"></a>
#### 结果分析
| **验证方法序号** | **数据库结果** | **结果分析** |
| --- | --- | --- |
| 1 | “张三”未插入，“李四”插入，“王五”插入。 | 外围方法开启事务，插入“张三”方法和外围方法一个事务，插入“李四”方法、插入“王五”方法分别在独立的新建事务中，外围方法抛出异常只回滚和外围方法同一事务的方法，故插入“张三”的方法回滚。 |
| 2 | “张三”未插入，“李四”插入，“王五”未插入。 | 外围方法开启事务，插入“张三”方法和外围方法一个事务，插入“李四”方法、插入“王五”方法分别在独立的新建事务中。插入“王五”方法抛出异常，首先插入 “王五”方法的事务被回滚，异常继续抛出被外围方法感知，外围方法事务亦被回滚，故插入“张三”方法也被回滚。 |
| 3 | “张三”插入，“李四”插入，“王五”未插入。 | 外围方法开启事务，插入“张三”方法和外围方法一个事务，插入“李四”方法、插入“王五”方法分别在独立的新建事务中。插入“王五”方法抛出异常，首先插入“王五”方法的事务被回滚，异常被catch不会被外围方法感知，外围方法事务不回滚，故插入“张三”方法插入成功。 |

<a name="H8ILy"></a>
#### 结论
**在外围方法开启事务的情况下**`**Propagation.REQUIRES_NEW**`**修饰的内部方法依然会单独开启独立事务，且与外部方法事务也独立，内部方法之间、内部方法和外部方法事务均相互独立，互不干扰。**
<a name="nAwHA"></a>
## 3、`PROPAGATION_NESTED`
<a name="zGMk3"></a>
### User1Service
添加1个方法，事务传播行为：`NESTED`
```java
@Transactional(propagation = Propagation.NESTED)
public void nested(String name) {
    this.jdbcTemplate.update("insert into user1(name) VALUES (?)", name);
}
```
<a name="FOkYS"></a>
### User2Service
添加2个方法，事务传播行为：NESTED，注意第2个方法内部最后一行会抛出一个异常。
```java
@Transactional(propagation = Propagation.NESTED)
public void nested(String name) {
    this.jdbcTemplate.update("insert into user2(name) VALUES (?)", name);
}

@Transactional(propagation = Propagation.NESTED)
public void nested_exception(String name) {
    this.jdbcTemplate.update("insert into user2(name) VALUES (?)", name);
    throw new RuntimeException();
}
```
<a name="JHDZq"></a>
### 场景1（3-1）
外围方法没有事务。
<a name="ZGmWE"></a>
#### 验证方法1
<a name="lvDHC"></a>
##### TxService添加
```java
public void notransaction_exception_nested_nested(){
    user1Service.nested("张三");
    user2Service.nested("李四");
    throw new RuntimeException();
}
```
<a name="mVqGT"></a>
##### Demo6Test中添加
```java
@Test
public void notransaction_exception_nested_nested() {
    txService.notransaction_exception_nested_nested();
}
```
<a name="BHxra"></a>
##### 运行输出
```
user1表数据：[{id=1, name=张三}]
user2表数据：[{id=1, name=李四}]
```
<a name="VtZvl"></a>
#### 验证方法2
<a name="NHbft"></a>
##### TxService添加
```java
public void notransaction_nested_nested_exception(){
    user1Service.nested("张三");
    user2Service.nested_exception("李四");
}
```
<a name="J7wTO"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void notransaction_nested_nested_exception() {
    txService.notransaction_nested_nested_exception();
}
```
<a name="g5OYB"></a>
##### 运行输出
```
user1表数据：[{id=1, name=张三}]
user2表数据：[]
```
<a name="N0Oar"></a>
#### 结果分析
| **验证方法序号** | **数据库结果** | **结果分析** |
| --- | --- | --- |
| 1 | “张三”、“李四”均插入。 | 外围方法未开启事务，插入“张三”、“李四”方法在自己的事务中独立运行，外围方法异常不影响内部插入“张三”、“李四”方法独立的事务。 |
| 2 | “张三”插入，“李四”未插入。 | 外围方法没有事务，插入“张三”、“李四”方法都在自己的事务中独立运行,所以插入“李四”方法抛出异常只会回滚插入“李四”方法，插入“张三”方法不受影响。 |

<a name="ZqH3n"></a>
#### 结论
**通过这两个方法证明了在外围方法未开启事务的情况下**`**Propagation.NESTED**`**和**`**Propagation.REQUIRED**`**作用相同，修饰的内部方法都会新开启自己的事务，且开启的事务相互独立，互不干扰。**
<a name="jQIil"></a>
### 场景2（3-1）
外围方法开启事务。
<a name="E4QDx"></a>
#### 验证方法1
<a name="A6oxg"></a>
##### TxService添加
```java
@Transactional
public void transaction_exception_nested_nested(){
    user1Service.nested("张三");
    user2Service.nested("李四");
    throw new RuntimeException();
}
```
<a name="LppCU"></a>
##### 测试用例，Demo6Test中添加
```java
@Test
public void transaction_exception_nested_nested() {
    txService.transaction_exception_nested_nested();
}
```
<a name="dj4Wu"></a>
##### 运行输出
```
user1表数据：[]
user2表数据：[]
```
<a name="TStuf"></a>
#### 验证方法2
<a name="UpZNm"></a>
##### TxService添加
```java
@Transactional
public void transaction_nested_nested_exception(){
    user1Service.nested("张三");
    user2Service.nested_exception("李四");
}
```
<a name="nBlKO"></a>
##### Demo6Test中添加
```java
@Test
public void transaction_nested_nested_exception() {
    txService.transaction_nested_nested_exception();
}
```
<a name="E16E2"></a>
##### 运行输出
```
user1表数据：[]
user2表数据：[]
```
<a name="j82kE"></a>
#### 验证方法3
<a name="joDDd"></a>
##### TxService添加
```java
@Transactional
public void transaction_nested_nested_exception_try(){
    user1Service.nested("张三");
    try {
        user2Service.nested_exception("李四");
    } catch (Exception e) {
        System.out.println("方法回滚");
    }
}
```
<a name="oqZ1z"></a>
##### Demo6Test中添加
```java
@Test
public void transaction_nested_nested_exception_try() {
    txService.transaction_nested_nested_exception_try();
}
```
<a name="K02RJ"></a>
##### 运行输出
```
方法回滚
user1表数据：[{id=1, name=张三}]
user2表数据：[]
```
<a name="aU8qU"></a>
#### 结果分析
| **验证方法序号** | **数据库结果** | **结果分析** |
| --- | --- | --- |
| 1 | “张三”、“李四”均未插入。 | 外围方法开启事务，内部事务为外围事务的子事务，外围方法回滚，内部方法也要回滚。 |
| 2 | “张三”、“李四”均未插入。 | 外围方法开启事务，内部事务为外围事务的子事务，内部方法抛出异常回滚，且外围方法感知异常致使整体事务回滚。 |
| 3 | “张三”插入、“李四”未插入。 | 外围方法开启事务，内部事务为外围事务的子事务，插入“李四”内部方法抛出异常，可以单独对子事务回滚。 |

<a name="SnJPE"></a>
#### 结论
**以上试验结果证明在外围方法开启事务的情况下Propagation.NESTED修饰的内部方法属于外部事务的子事务，外围主事务回滚，子事务一定回滚，而内部子事务可以单独回滚而不影响外围主事务和其他子事务。**
<a name="paUJ8"></a>
#### 内部事务原理
以MySQL为例，MySQL中有个savepoint的功能，`NESTED`内部事务就是通过这个实现的。
<a name="XI0kX"></a>
## `REQUIRED`，`REQUIRES_NEW`，`NESTED`比较
由“场景2（1-2）”和“场景2（3-2）”对比，可知：<br />`**REQUIRED**`**和**`**NESTED**`**修饰的内部方法都属于外围方法事务，如果外围方法抛出异常，这两种方法的事务都会被回滚。但是**`**REQUIRED**`**是加入外围方法事务，所以和外围事务同属于一个事务，一旦**`**REQUIRED**`**事务抛出异常被回滚，外围方法事务也将被回滚。而NESTED是外围方法的子事务，有单独的保存点，所以**`**NESTED**`**方法抛出异常被回滚，不会影响到外围方法的事务。**<br />由“场景2（2-2）”和“场景2（3-2）”对比可知：<br />`**REQUIRES_NEW**`**和NESTED都可以做到内部方法事务回滚而不影响外围方法事务。但是因为NESTED是嵌套事务，所以外围方法回滚之后，作为外围方法事务的子事务也会被回滚。而**`**REQUIRES_NEW**`**是通过开启新的事务实现的，内部事务和外围事务是两个事务，外围事务回滚不会影响内部事务。**
<a name="iny5d"></a>
## 其他几个传播行为
`REQUIRED`，`REQUIRES_NEW`，`NESTED`这几个算是比较特殊的，比较常用的，剩下的5个传播行为，大家可以自己练练。
<a name="kw40N"></a>
## 模拟用例
介绍了这么多事务传播行为，在实际工作中如何应用呢？下面来举一个示例：<br />假设有一个注册的方法，方法中调用添加积分的方法，如果希望添加积分不会影响注册流程（即添加积分执行失败回滚不能使注册方法也回滚），会这样写：
```java
@Service
public class UserServiceImpl implements UserService {

    @Transactional
    public void register(User user){

        try {
            membershipPointService.addPoint(Point point);
        } catch (Exception e) {
            //省略...
        }
        //省略...
    }
    //省略...
}
```
还规定注册失败要影响`addPoint()`方法（注册方法回滚添加积分方法也需要回滚），那么`addPoint()`方法就需要这样实现：
```java
@Service
public class MembershipPointServiceImpl implements MembershipPointService{

    @Transactional(propagation = Propagation.NESTED)
    public void addPoint(Point point){
        try {
            recordService.addRecord(Record record);
        } catch (Exception e) {
            //省略...
        }
        //省略...
    }
    //省略...
}
```
注意到了在`addPoint()`中还调用了`addRecord()`方法，这个方法用来记录日志。他的实现如下：
```java
@Service
public class RecordServiceImpl implements RecordService{

    @Transactional(propagation = Propagation.NOT_SUPPORTED)
    public void addRecord(Record record){

        //省略...
    }
    //省略...
}
```
注意到`addRecord()`方法中`propagation = Propagation.NOT_SUPPORTED`，因为对于日志无所谓精确，可以多一条也可以少一条，所以`addRecord()`方法本身和外围`addPoint()`方法抛出异常都不会使`addRecord()`方法回滚，并且`addRecord()`方法抛出异常也不会影响外围`addPoint()`方法的执行。<br />通过这个例子相信大家对事务传播行为的使用有了更加直观的认识，通过各种属性的组合确实能让业务实现更加灵活多样。
