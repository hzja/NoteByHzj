JavaSpring
<a name="fAvtq"></a>
## 什么是声明式事务？
所谓声明式事务，就是通过配置的方式，比如通过配置文件（xml）或者注解的方式，告诉Spring，哪些方法需要Spring帮忙管理事务，然后开发者只用关注业务代码，而事务的事情Spring自动控制。<br />比如注解的方式，只需在方法上面加一个`@Transactional`注解，那么方法执行之前Spring会自动开启一个事务，方法执行完毕之后，会自动提交或者回滚事务，而方法内部没有任何事务相关代码，用起来特别的方法。
```java
@Transactional
public void insert(String userName){
    this.jdbcTemplate.update("insert into t_user (name) values (?)", userName);
}
```
<a name="iR4sN"></a>
## 声明式事务的2种实现方式

1. **配置文件的方式**，即在spring xml文件中进行统一配置，开发者基本上就不用关注事务的事情了，代码中无需关心任何和事务相关的代码，一切交给Spring处理。
2. **注解的方式**，只需在需要Spring来帮忙管理事务的方法上加上`@Transactional`注解就可以了，注解的方式相对来说更简洁一些，都需要开发者自己去进行配置，可能有些同学对Spring不是太熟悉，所以配置这个有一定的风险，做好代码review就可以了。

配置文件的方式用的相对比较少，主要掌握注解的方式如何使用，就可以了。
<a name="ccJ6Z"></a>
## 声明式事务注解方式5个步骤
<a name="EFI5n"></a>
### 1、启用Spring的注释驱动事务管理功能
在Spring配置类上加上`@EnableTransactionManagement`注解
```java
@EnableTransactionManagement
public class MainConfig4 {
}
```
简要介绍一下原理：**当Spring容器启动的时候，发现有**`**@EnableTransactionManagement**`**注解，此时会拦截所有bean的创建，扫描看一下bean上是否有**`**@Transactional**`**注解（类、或者父类、或者接口、或者方法中有这个注解都可以），如果有这个注解，Spring会通过aop的方式给bean生成代理对象，代理对象中会增加一个拦截器，拦截器会拦截bean中**`**public**`**方法执行，会在方法执行之前启动事务，方法执行完毕之后提交或者回滚事务。稍后会专门有一篇文章带大家看这块的源码。**<br />如果有兴趣的可以自己先去读一下源码，主要是下面这个这方法
```
org.springframework.transaction.interceptor.TransactionInterceptor#invoke
```
再来看看 `EnableTransactionManagement` 的源码
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Import(TransactionManagementConfigurationSelector.class)
public @interface EnableTransactionManagement {

    /**
  * spring是通过aop的方式对bean创建代理对象来实现事务管理的
  * 创建代理对象有2种方式，jdk动态代理和cglib代理
  * proxyTargetClass：为true的时候，就是强制使用cglib来创建代理
  */
    boolean proxyTargetClass() default false;

    /**
  * 用来指定事务拦截器的顺序
  * 一个方法上可以添加很多拦截器，拦截器是可以指定顺序的
  * 比如可以自定义一些拦截器，放在事务拦截器之前或者之后执行，就可以通过order来控制
  */
    int order() default Ordered.LOWEST_PRECEDENCE;
}
```
<a name="uTDxU"></a>
### 2、定义事务管理器
事务交给Spring管理，那么肯定要创建一个或者多个事务管理者，有这些管理者来管理具体的事务，比如启动事务、提交事务、回滚事务，这些都是管理者来负责的。<br />Spring中使用`PlatformTransactionManager`这个接口来表示事务管理者。<br />`PlatformTransactionManager`多个实现类，用来应对不同的环境<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690010345603-3165b616-3d4f-4891-9e28-7f81a31195d0.png#averageHue=%23fbfbf1&clientId=ua228f0c6-5b65-4&from=paste&id=udb2f81b9&originHeight=172&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u02fab467-ab94-4c32-aa5e-d7cd6efb276&title=)<br />`**JpaTransactionManager**`：如果用jpa来操作db，那么需要用这个管理器来控制事务。<br />`**DataSourceTransactionManager**`：如果用指定数据源的方式，比如操作数据库用的是：JdbcTemplate、Mybatis、ibatis，那么需要用这个管理器来控制事务。<br />`**HibernateTransactionManager**`：如果用hibernate来操作db，那么需要用这个管理器来控制事务。<br />`JtaTransactionManager`：如果用的是Java中的jta来操作db，这种通常是分布式事务，此时需要用这种管理器来控制事务。<br />比如用Mybatis或者jJdbcTemplate，那么通过下面方式定义一个事务管理器。
```java
@Bean
public PlatformTransactionManager transactionManager(DataSource dataSource) {
    return new DataSourceTransactionManager(dataSource);
}
```
<a name="Y54ql"></a>
### 3、需使用事务的目标上加`@Transactional`注解

- `@Transactional`放在接口上，那么接口的实现类中所有`public`都被Spring自动加上事务
- `@Transactional`放在类上，那么当前类以及其下无限级子类中所有`pubilc`方法将被Spring自动加上事务
- `@Transactional`放在`public`方法上，那么该方法将被Spring自动加上事务
- 注意：`**@Transactional**`**只对**`**public**`**方法有效**

下面看一下`@Transactional`源码：
```java
@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Inherited
@Documented
public @interface Transactional {

    /**
     * 指定事务管理器的bean名称，如果容器中有多事务管理器PlatformTransactionManager，
     * 那么得告诉spring，当前配置需要使用哪个事务管理器
     */
    @AliasFor("transactionManager")
    String value() default "";

    /**
     * 同value，value和transactionManager选配一个就行，也可以为空，如果为空，默认会从容器中按照类型查找一个事务管理器bean
     */
    @AliasFor("value")
    String transactionManager() default "";

    /**
     * 事务的传播属性
     */
    Propagation propagation() default Propagation.REQUIRED;

    /**
     * 事务的隔离级别，就是制定数据库的隔离级别，数据库隔离级别大家知道么？不知道的可以去补一下
     */
    Isolation isolation() default Isolation.DEFAULT;

    /*
     * 事务执行的超时时间（秒），执行一个方法，比如有问题，那不可能等一天吧，可能最多只能等10秒
     * 10秒后，还没有执行完毕，就弹出一个超时异常吧
     */
    int timeout() default TransactionDefinition.TIMEOUT_DEFAULT;

    /**
     * 是否是只读事务，比如某个方法中只有查询操作，可以指定事务是只读的
     * 设置了这个参数，可能数据库会做一些性能优化，提升查询速度
     */
    boolean readOnly() default false;

    /**
     * 定义零(0)个或更多异常类，这些异常类必须是Throwable的子类，当方法抛出这些异常及其子类异常的时候，spring会让事务回滚
     * 如果不配做，那么默认会在 RuntimeException 或者 Error 情况下，事务才会回滚 
     */
    Class<? extends Throwable>[] rollbackFor() default {};

    /**
     * 和 rollbackFor 作用一样，只是这个地方使用的是类名
     */
    String[] rollbackForClassName() default {};

    /**
     * 定义零(0)个或更多异常类，这些异常类必须是Throwable的子类，当方法抛出这些异常的时候，事务不会回滚
     */
    Class<? extends Throwable>[] noRollbackFor() default {};

    /**
     * 和 noRollbackFor 作用一样，只是这个地方使用的是类名
     */
    String[] noRollbackForClassName() default {};

}
```
参数介绍

| 参数 | 描述 |
| --- | --- |
| `value` | 指定事务管理器的bean名称，如果容器中有多事务管理器`PlatformTransactionManager`，那么得告诉Spring，当前配置需要使用哪个事务管理器 |
| `transactionManager` | 同value，value和`transactionManager`选配一个就行，也可以为空，如果为空，默认会从容器中按照类型查找一个事务管理器bean |
| `propagation` | 事务的传播属性 |
| `isolation` | 事务的隔离级别，就是制定数据库的隔离级别，数据库隔离级别大家知道么？不知道的可以去补一下 |
| `timeout` | 事务执行的超时时间（秒），执行一个方法，比如有问题，那我不可能等你一天吧，可能最多我只能等你10秒 10秒后，还没有执行完毕，就弹出一个超时异常吧 |
| `readOnly` | 是否是只读事务，比如某个方法中只有查询操作，我们可以指定事务是只读的 设置了这个参数，可能数据库会做一些性能优化，提升查询速度 |
| `rollbackFor` | 定义零(0)个或更多异常类，这些异常类必须是Throwable的子类，当方法抛出这些异常及其子类异常的时候，Spring会让事务回滚 如果不配做，那么默认会在 RuntimeException 或者 Error 情况下，事务才会回滚 |
| `rollbackForClassName` | 同 `rollbackFor`，只是这个地方使用的是类名 |
| `noRollbackFor` | 定义零(0)个或更多异常类，这些异常类必须是Throwable的子类，当方法抛出这些异常的时候，事务不会回滚 |
| `noRollbackForClassName` | 同 `noRollbackFor`，只是这个地方使用的是类名 |

<a name="G8GmB"></a>
### 4、执行db业务操作
在`@Transactional`标注类或者目标方法上执行业务操作，此时这些方法会自动被Spring进行事务管理。<br />如，下面的`insertBatch`操作，先删除数据，然后批量插入数据，方法上加上了`@Transactional`注解，此时这个方法会自动受Spring事务控制，要么都成功，要么都失败。
```java
@Component
public class UserService {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    //先清空表中数据，然后批量插入数据，要么都成功要么都失败
    @Transactional
    public void insertBatch(String... names) {
        jdbcTemplate.update("truncate table t_user");
        for (String name : names) {
            jdbcTemplate.update("INSERT INTO t_user(name) VALUES (?)", name);
        }
    }
}
```
<a name="bFzlg"></a>
### 5、启动Spring容器，使用bean执行业务操作
```java
@Test
public void test1() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig4.class);
    context.refresh();

    UserService userService = context.getBean(UserService.class);
    userService.insertBatch("java高并发系列", "mysql系列", "maven系列", "mybatis系列");
}
```
<a name="eb6SL"></a>
## 案例1
<a name="pjRk1"></a>
### 准备数据库
```sql
DROP DATABASE IF EXISTS javacode2018;
CREATE DATABASE if NOT EXISTS javacode2018;

USE javacode2018;
DROP TABLE IF EXISTS t_user;
CREATE TABLE t_user(
  id int PRIMARY KEY AUTO_INCREMENT,
  name varchar(256) NOT NULL DEFAULT '' COMMENT '姓名'
);
```
<a name="dd13N"></a>
### Spring配置类
```java
package com.javacode2018.tx.demo4;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;
import org.springframework.transaction.support.TransactionTemplate;

import javax.sql.DataSource;

@EnableTransactionManagement //@1
@Configuration
@ComponentScan
public class MainConfig4 {
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

    //定义一个事物管理器
    @Bean
    public PlatformTransactionManager transactionManager(DataSource dataSource) { //@2
        return new DataSourceTransactionManager(dataSource);
    }
}
```
**@1**：使用`@EnableTransactionManagement`注解开启Spring事务管理<br />**@2**：定义事务管理器
<a name="kJa3u"></a>
### 来个业务类
```java
package com.javacode2018.tx.demo4;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Map;

@Component
public class UserService {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    //先清空表中数据，然后批量插入数据，要么都成功要么都失败
    @Transactional //@1
    public int insertBatch(String... names) {
        int result = 0;
        jdbcTemplate.update("truncate table t_user");
        for (String name : names) {
            result += jdbcTemplate.update("INSERT INTO t_user(name) VALUES (?)", name);
        }
        return result;
    }

    //获取所有用户信息
    public List<Map<String, Object>> userList() {
        return jdbcTemplate.queryForList("SELECT * FROM t_user");
    }
}
```
**@1**：`insertBatch`方法上加上了`@Transactional`注解，让Spring来自动为这个方法加上事务
<a name="N59nD"></a>
### 测试类
```java
package com.javacode2018.tx.demo4;

import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class Demo4Test {
    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(MainConfig4.class);
        context.refresh();

        UserService userService = context.getBean(UserService.class);
        //先执行插入操作
        int count = userService.insertBatch(
            "java高并发",
            "mysql",
            "maven",
            "mybatis");
        System.out.println("插入成功（条）：" + count);
        //然后查询一下
        System.out.println(userService.userList());
    }
}
```
<a name="ejGFX"></a>
### 运行输出
```
插入成功（条）：4
[{id=1, name=java高并发}, {id=2, name=mysql}, {id=3, name=maven}, {id=4, name=mybatis}]
```
**有些朋友可能会问，如何知道这个被调用的方法有没有使用事务？** 下面就来看一下。
<a name="sQr13"></a>
## 如何确定方法有没有用到Spring事务
<a name="HKhzD"></a>
### 方式1：断点调试
Spring事务是由`TransactionInterceptor`拦截器处理的，最后会调用下面这个方法，设置个断点就可以看到详细过程了。
```
org.springframework.transaction.interceptor.TransactionAspectSupport#invokeWithinTransaction
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690010345458-91691789-66cb-42bb-8cfe-d05a9be327ea.png#averageHue=%23eeebe6&clientId=ua228f0c6-5b65-4&from=paste&id=u012b51e9&originHeight=787&originWidth=887&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8c40888d-4fd2-43b8-a85e-8ccd59f6cd9&title=)
<a name="X4wVi"></a>
### 方式2：看日志
Spring处理事务的过程，有详细的日志输出，开启日志，控制台就可以看到事务的详细过程了。
<a name="uJTa8"></a>
#### 添加Maven配置
```xml
<dependency>
  <groupId>ch.qos.logback</groupId>
  <artifactId>logback-classic</artifactId>
  <version>1.2.3</version>
</dependency>
```
<a name="bDQ5N"></a>
#### src\main\resources新建logback.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
  <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <encoder>
      <pattern>[%d{MM-dd HH:mm:ss.SSS}][%thread{20}:${PID:- }][%X{trace_id}][%level][%logger{56}:%line:%method\(\)]:%msg%n##########**********##########%n</pattern>
    </encoder>
  </appender>

  <logger name="org.springframework" level="debug">
    <appender-ref ref="STDOUT" />
  </logger>

</configuration>
```
再来运行一下案例1
```
[09-10 11:20:38.830][main: ][][DEBUG][o.s.jdbc.datasource.DataSourceTransactionManager:370:getTransaction()]:Creating new transaction with name [com.javacode2018.tx.demo4.UserService.insertBatch]: PROPAGATION_REQUIRED,ISOLATION_DEFAULT
##########**********##########
[09-10 11:20:39.120][main: ][][DEBUG][o.s.jdbc.datasource.DataSourceTransactionManager:265:doBegin()]:Acquired Connection [ProxyConnection[PooledConnection[com.mysql.jdbc.JDBC4Connection@65fe9e33]]] for JDBC transaction
##########**********##########
[09-10 11:20:39.125][main: ][][DEBUG][o.s.jdbc.datasource.DataSourceTransactionManager:283:doBegin()]:Switching JDBC Connection [ProxyConnection[PooledConnection[com.mysql.jdbc.JDBC4Connection@65fe9e33]]] to manual commit
##########**********##########
[09-10 11:20:39.139][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:502:update()]:Executing SQL update [truncate table t_user]
##########**********##########
[09-10 11:20:39.169][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:860:update()]:Executing prepared SQL update
##########**********##########
[09-10 11:20:39.169][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:609:execute()]:Executing prepared SQL statement [INSERT INTO t_user(name) VALUES (?)]
##########**********##########
[09-10 11:20:39.234][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:860:update()]:Executing prepared SQL update
##########**********##########
[09-10 11:20:39.235][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:609:execute()]:Executing prepared SQL statement [INSERT INTO t_user(name) VALUES (?)]
##########**********##########
[09-10 11:20:39.236][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:860:update()]:Executing prepared SQL update
##########**********##########
[09-10 11:20:39.237][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:609:execute()]:Executing prepared SQL statement [INSERT INTO t_user(name) VALUES (?)]
##########**********##########
[09-10 11:20:39.238][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:860:update()]:Executing prepared SQL update
##########**********##########
[09-10 11:20:39.239][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:609:execute()]:Executing prepared SQL statement [INSERT INTO t_user(name) VALUES (?)]
##########**********##########
[09-10 11:20:39.241][main: ][][DEBUG][o.s.jdbc.datasource.DataSourceTransactionManager:741:processCommit()]:Initiating transaction commit
##########**********##########
[09-10 11:20:39.241][main: ][][DEBUG][o.s.jdbc.datasource.DataSourceTransactionManager:328:doCommit()]:Committing JDBC transaction on Connection [ProxyConnection[PooledConnection[com.mysql.jdbc.JDBC4Connection@65fe9e33]]]
##########**********##########
[09-10 11:20:39.244][main: ][][DEBUG][o.s.jdbc.datasource.DataSourceTransactionManager:387:doCleanupAfterCompletion()]:Releasing JDBC Connection [ProxyConnection[PooledConnection[com.mysql.jdbc.JDBC4Connection@65fe9e33]]] after transaction
##########**********##########
插入成功（条）：4
[09-10 11:20:39.246][main: ][][DEBUG][org.springframework.jdbc.core.JdbcTemplate:427:query()]:Executing SQL query [SELECT * FROM t_user]
##########**********##########
[09-10 11:20:39.247][main: ][][DEBUG][org.springframework.jdbc.datasource.DataSourceUtils:115:doGetConnection()]:Fetching JDBC Connection from DataSource
##########**********##########
[{id=1, name=java高并发系列}, {id=2, name=mysql系列}, {id=3, name=maven系列}, {id=4, name=mybatis系列}]
```
<a name="JXA1j"></a>
#### 来理解一下日志
`insertBatch`方法上有`@Transactional`注解，所以会被拦截器拦截，下面是在`insertBatch`方法调用之前，创建了一个事务。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690010345462-57df0612-5366-413c-901e-208f1a85aff5.png#averageHue=%23efeded&clientId=ua228f0c6-5b65-4&from=paste&id=u6fa3f41a&originHeight=217&originWidth=859&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u343d1f2c-ce25-4e37-abe4-9e938dd5043&title=)<br />`insertBatch`方法上`@Transactional`注解参数都是默认值，`@Transactional`注解中可以通过value或者`transactionManager`来指定事务管理器，但是没有指定，此时Spring会在容器中按照事务管理器类型找一个默认的，刚好在Spring容器中定义了一个，所以直接拿来用了。事务管理器用的是`new DataSourceTransactionManager(dataSource)`，从事务管理器的DataSource中获取一个数据库连接，然后通过连接设置事务为手动提交，然后将（DataSource->这个连接)丢到ThreadLocal中了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690010345513-fa432ef3-5d5e-45d9-b77e-adaa1ede5af9.png#averageHue=%23f0f0f0&clientId=ua228f0c6-5b65-4&from=paste&id=ue9b914c0&originHeight=199&originWidth=810&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9d9aa8fa-2af7-4d9d-b936-35598f63644&title=)<br />下面就正是进入`insertBatch`方法内部了，通过jdbctemplate执行一些db操作，jdbctemplate内部会通过DataSource到上面的threadlocal中拿到Spring事务那个连接，然后执行db操作。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690010345455-62c34fa4-6050-4052-bed4-bb3b84db3435.png#averageHue=%23f0f0f0&clientId=ua228f0c6-5b65-4&from=paste&id=u2e9bbdf3&originHeight=563&originWidth=803&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub5e36d71-5fe0-4bf9-a49f-e789dab3fd0&title=)<br />最后`insertBatch`方法执行完毕之后，没有任何异常，那么Spring就开始通过数据库连接提交事务了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690010346046-7bf8fb94-cb2b-4092-b7c4-593f42ad9f4e.png#averageHue=%23efefef&clientId=ua228f0c6-5b65-4&from=paste&id=u9c682918&originHeight=243&originWidth=804&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud581b644-ce3b-4d60-bf64-246c0583257&title=)
<a name="CdwGr"></a>
## 总结
本文讲解了一下Spring中编程式事务的使用步骤。<br />主要涉及到了2个注解：<br />`@EnableTransactionManagement`：开启Spring事务管理功能<br />`@Transactional`：将其加在需要Spring管理事务的类、方法、接口上，只会对`public`方法有效。
