Java SpringBoot JTA 分布式事务
<a name="hkX0u"></a>
## 什么是JTA
JTA，全称：Java Transaction API。JTA事务比JDBC事务更强大。一个JTA事务可以有多个参与者，而一个JDBC事务则被限定在一个单一的数据库连接。所以，当在同时操作多个数据库的时候，使用JTA事务就可以弥补JDBC事务的不足。<br />在Spring Boot 2.x中，整合了这两个JTA的实现：

- Atomikos：可以通过引入spring-boot-starter-jta-atomikos依赖来使用
- Bitronix：可以通过引入spring-boot-starter-jta-bitronix依赖来使用

由于Bitronix自Spring Boot 2.3.0开始不推荐使用，所以在下面的动手环节中，将使用`Atomikos`作为例子来介绍JTA的使用。
<a name="Nb0mk"></a>
## 动手试试
下面就来实操一下，如何在Spring Boot中使用JTA来实现多数据源下的事务管理。
<a name="oBVo9"></a>
### 准备工作

1. 这里将使用最基础的JdbcTemplate来实现数据访问。
2. 场景设定：
- 假设有两个库，分别为：test1和test2
- 这两个库中都有一张User表，希望这两张表中的数据是一致的
- 假设这两张表中都已经有一条数据：name=aaa，age=30；因为这两张表中数据是一致的，所以要update的时候，就必须两个库中的User表更新时候，要么都成功，要么都失败。
<a name="vRn1s"></a>
### 操作详细
<a name="SimF6"></a>
#### 在pom.xml中加入JTA的实现Atomikos的Starter
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-jta-atomikos</artifactId>
</dependency>
```
<a name="zNVt8"></a>
#### 在`application.properties`配置文件中配置两个test1和test2数据源
```
spring.jta.enabled=true

spring.jta.atomikos.datasource.primary.xa-properties.url=jdbc:mysql://localhost:3306/test1
spring.jta.atomikos.datasource.primary.xa-properties.user=root
spring.jta.atomikos.datasource.primary.xa-properties.password=12345678
spring.jta.atomikos.datasource.primary.xa-data-source-class-name=com.mysql.cj.jdbc.MysqlXADataSource
spring.jta.atomikos.datasource.primary.unique-resource-name=test1
spring.jta.atomikos.datasource.primary.max-pool-size=25
spring.jta.atomikos.datasource.primary.min-pool-size=3
spring.jta.atomikos.datasource.primary.max-lifetime=20000
spring.jta.atomikos.datasource.primary.borrow-connection-timeout=10000

spring.jta.atomikos.datasource.secondary.xa-properties.url=jdbc:mysql://localhost:3306/test2
spring.jta.atomikos.datasource.secondary.xa-properties.user=root
spring.jta.atomikos.datasource.secondary.xa-properties.password=12345678
spring.jta.atomikos.datasource.secondary.xa-data-source-class-name=com.mysql.cj.jdbc.MysqlXADataSource
spring.jta.atomikos.datasource.secondary.unique-resource-name=test2
spring.jta.atomikos.datasource.secondary.max-pool-size=25
spring.jta.atomikos.datasource.secondary.min-pool-size=3
spring.jta.atomikos.datasource.secondary.max-lifetime=20000
spring.jta.atomikos.datasource.secondary.borrow-connection-timeout=10000
```
<a name="U0PX9"></a>
#### 创建多数据源配置类
```java
@Configuration
public class DataSourceConfiguration {

    @Primary
    @Bean
    @ConfigurationProperties(prefix = "spring.jta.atomikos.datasource.primary")
    public DataSource primaryDataSource() {
        return new AtomikosDataSourceBean();
    }

    @Bean
    @ConfigurationProperties(prefix = "spring.jta.atomikos.datasource.secondary")
    public DataSource secondaryDataSource() {
        return new AtomikosDataSourceBean();
    }

    @Bean
    public JdbcTemplate primaryJdbcTemplate(@Qualifier("primaryDataSource") DataSource primaryDataSource) {
        return new JdbcTemplate(primaryDataSource);
    }

    @Bean
    public JdbcTemplate secondaryJdbcTemplate(@Qualifier("secondaryDataSource") DataSource secondaryDataSource) {
        return new JdbcTemplate(secondaryDataSource);
    }

}
```
:::tips
注意，这里除了家在的配置不同之外，`DataSource`也采用了`AtomikosDataSourceBean`注意与之前配置多数据源使用的配置和实现类的区别。
:::
<a name="A7HSr"></a>
#### 创建一个Service实现，模拟两种不同的情况。
```java
@Service
public class TestService {

    private JdbcTemplate primaryJdbcTemplate;
    private JdbcTemplate secondaryJdbcTemplate;

    public TestService(JdbcTemplate primaryJdbcTemplate, JdbcTemplate secondaryJdbcTemplate) {
        this.primaryJdbcTemplate = primaryJdbcTemplate;
        this.secondaryJdbcTemplate = secondaryJdbcTemplate;
    }

    @Transactional
    public void tx() {
        // 修改test1库中的数据
        primaryJdbcTemplate.update("update user set age = ? where name = ?", 30, "aaa");
        // 修改test2库中的数据
        secondaryJdbcTemplate.update("update user set age = ? where name = ?", 30, "aaa");
    }

    @Transactional
    public void tx2() {
        // 修改test1库中的数据
        primaryJdbcTemplate.update("update user set age = ? where name = ?", 40, "aaa");
        // 模拟：修改test2库之前抛出异常
        throw new RuntimeException();
    }

}
```
这里tx函数，是两句update操作，一般都会成功；而tx2函数中，人为的制造了一个异常，这个异常是在test1库中的数据更新后才产生的，这样就可以测试一下test1更新成功，之后是否还能在JTA的帮助下实现回滚。
<a name="jd99j"></a>
#### 创建测试类，编写测试用例
```java
@SpringBootTest(classes = Chapter312Application.class)
public class Chapter312ApplicationTests {

    @Autowired
    protected JdbcTemplate primaryJdbcTemplate;
    @Autowired
    protected JdbcTemplate secondaryJdbcTemplate;

    @Autowired
    private TestService testService;

    @Test
    public void test1() throws Exception {
        // 正确更新的情况
        testService.tx();
        Assertions.assertEquals(30, primaryJdbcTemplate.queryForObject("select age from user where name=?", Integer.class, "aaa"));
        Assertions.assertEquals(30, secondaryJdbcTemplate.queryForObject("select age from user where name=?", Integer.class, "aaa"));
    }

    @Test
    public void test2() throws Exception {
        // 更新失败的情况
        try {
            testService.tx2();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            // 部分更新失败，test1中的更新应该回滚
            Assertions.assertEquals(30, primaryJdbcTemplate.queryForObject("select age from user where name=?", Integer.class, "aaa"));
            Assertions.assertEquals(30, secondaryJdbcTemplate.queryForObject("select age from user where name=?", Integer.class, "aaa"));
        }
    }

}
```
这里有两个测试用例：

- test1：因为没有故意制造的异常，不出意外两个库的update都会成功，所以根据name=aaa去把两个数据查出来，看age是否都被更新到了30。
- test2：tx2函数会把test1中name=aaa的用户age更新为40，然后抛出异常，JTA事务生效的话，会把age回滚回30，所以这里的检查也是两个库的aaa用户的age应该都为30，这样就意味着JTA事务生效，保证了test1和test2两个库中的User表数据更新一致，没有制造出脏数据。
<a name="fF7pj"></a>
## 测试验证
将上面编写的单元测试运行起来:<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630916338516-fff5c1f6-58dc-4378-a953-44dd505e650f.webp#clientId=u984c032c-1056-4&from=paste&id=u51993408&originHeight=262&originWidth=744&originalType=url&ratio=1&status=done&style=none&taskId=ua322b63e-5748-4202-9c3c-1c88118d3b6)<br />观察一下启动阶段的日志，可以看到这些Atomikos初始化日志输出：
```
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.default_max_wait_time_on_shutdown = 9223372036854775807
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.allow_subtransactions = true
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.recovery_delay = 10000
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.automatic_resource_registration = true
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.oltp_max_retries = 5
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.client_demarcation = false
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.threaded_2pc = false
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.serial_jta_transactions = true
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.log_base_dir = /Users/didi/Documents/GitHub/SpringBoot-Learning/2.x/chapter3-12/transaction-logs
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.rmi_export_class = none
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.max_actives = 50
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.checkpoint_interval = 500
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.enable_logging = true
2021-02-02 19:00:36.145  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.log_base_name = tmlog
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.max_timeout = 300000
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.trust_client_tm = false
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: java.naming.factory.initial = com.sun.jndi.rmi.registry.RegistryContextFactory
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.tm_unique_name = 127.0.0.1.tm
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.forget_orphaned_log_entries_delay = 86400000
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.oltp_retry_interval = 10000
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: java.naming.provider.url = rmi://localhost:1099
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.force_shutdown_on_vm_exit = false
2021-02-02 19:00:36.146  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : USING: com.atomikos.icatch.default_jta_timeout = 10000
2021-02-02 19:00:36.147  INFO 8868 --- [           main] c.a.icatch.provider.imp.AssemblerImp     : Using default (local) logging and recovery...
2021-02-02 19:00:36.184  INFO 8868 --- [           main] c.a.d.xa.XATransactionalResource         : test1: refreshed XAResource
2021-02-02 19:00:36.203  INFO 8868 --- [           main] c.a.d.xa.XATransactionalResource         : test2: refreshed XAResource
```
同时，在transaction-logs目录下，还能找到关于事务的日志信息：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630916338263-d2fba8ea-8d95-484c-b336-aefb2da6249d.webp#clientId=u984c032c-1056-4&from=paste&id=u55bbb16f&originHeight=334&originWidth=618&originalType=url&ratio=1&status=done&style=none&taskId=ue54efd02-b299-45f9-b18e-1d848a77840)
```json
{"id":"127.0.0.1.tm161226409083100001","wasCommitted":true,"participants":[{"uri":"127.0.0.1.tm1","state":"COMMITTING","expires":1612264100801,"resourceName":"test1"},{"uri":"127.0.0.1.tm2","state":"COMMITTING","expires":1612264100801,"resourceName":"test2"}]}
{"id":"127.0.0.1.tm161226409083100001","wasCommitted":true,"participants":[{"uri":"127.0.0.1.tm1","state":"TERMINATED","expires":1612264100804,"resourceName":"test1"},{"uri":"127.0.0.1.tm2","state":"TERMINATED","expires":1612264100804,"resourceName":"test2"}]}
{"id":"127.0.0.1.tm161226409092800002","wasCommitted":false,"participants":[{"uri":"127.0.0.1.tm3","state":"TERMINATED","expires":1612264100832,"resourceName":"test1"}]}
```
