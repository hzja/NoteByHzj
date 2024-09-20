SpringBoot 3.2 引入了JdbcClient API组件可以对数据库操作，实际上JdbcClient是在JdbcTemplate基础上进行了封装。<br />SpringBoot 3.2引入了新的 JdbcClient 用户数据库操作，JdbcClient对JdbcTemplate进行了封装，采用了 fluent API 的风格，可以进行链式调用。<br />自此，Spring自带的数据库操作有了4种方式：JdbcTemplate、JdbcClient、SpringDataJdbc、SpringDataJpa。<br />对于不适合使用复杂的ORM框架，或者需要编写复杂的SQL的场景，可以使用JdbcClient自己编写SQL来操作数据库。不过JdbcClient不支持数据的批量操作和存储过程调用，对于这种情况就需要使用JdbcTemplate。
<a name="WWhWy"></a>
## **一、概述**
JdbcClient是一个轻量的数据库操作框架，采用 fluent API 风格，简单灵活，易于阅读和维护，支持编写复杂的SQL。
<a name="HLngy"></a>
## **二、引入 JDBCCLIENT**
首先引入 spring-data-jdbc 依赖。<br />在 build.gradle 中增加一行代码：
```groovy
implementation 'org.springframework.boot:spring-boot-starter-data-jdbc'
```
在Service中直接注入JdbcClient即可：
```java
@Component
public class DbService {
    @Autowired
    private JdbcClient jdbcClient;
}
```
<a name="zX4a2"></a>
## **三、查询操作**
通过JdbcClient，可以按照主键查数据，也可以按照自定义查询条件查数据。<br />按照主键查数据：
```java
public MyData findDataById(Long id) {
   return jdbcClient.sql("select * from my_data where id = ?")
       .params(id)
       .query(MyData.class)
       .single();
}
```
按照自定义查询条件查数据：
```java
public List<MyData> findDataByName(String name) {
   return jdbcClient.sql("select * from my_data where name = ?")
       .params(name)
       .query(MyData.class)
       .list();
}
```
以上两种查询方式，查询条件中的变量使用的是占位符，JdbcClient也支持按照参数名进行查询：
```java
public Integer insertDataWithNamedParam(MyData myData) {
   Integer rowsAffected = jdbcClient.sql("insert into my_data values(:id,:name) ")
       .param("id", myData.id())
       .param("name", myData.name())
       .update();
   return rowsAffected;
}
```
当参数比较多时，可以将参数放到一个Map中，用Map进行查询：
```java
public List<MyData> findDataByParamMap(Map<String, ?> paramMap) {
    return jdbcClient.sql("select * from my_data where name = :name")
    .params(paramMap)
    .query(MyData.class)
    .list();
}
```
当查询返回的结果不能简单的映射到一个类时，可以编写RowMapper，适用于SQL语句比较复杂的场景：
```java
public List<MyData> findDataWithRowMapper() {
    return jdbcClient.sql("select * from my_data")
    .query((rs, rowNum) -> new MyData(rs.getLong("id"), rs.getString("name")))
    .list();
}
```
同时也支持查询记录数：
```java
public Integer countByName(String name) {
   return jdbcClient.sql("select count(*) from my_data where name = ?")
       .params(name)
       .query(Integer.class)
       .single();
}
```
<a name="iQv8v"></a>
## **四、插入数据**
可以使用 JdbcClient 的 update 方法进行数据的插入和更新。<br />通过占位符参数插入数据：
```java
public Integer insertDataWithParam(MyData myData) {
   Integer rowsAffected = jdbcClient.sql("insert into my_data values(?,?) ")
       .param(myData.id())
       .param(myData.name())
       .update();
   return rowsAffected;
}
```
通过命名参数插入数据：
```java
public Integer insertDataWithNamedParam(MyData myData) {
   Integer rowsAffected = jdbcClient.sql("insert into my_data values(:id,:name) ")
       .param("id", myData.id())
       .param("name", myData.name())
       .update();
   return rowsAffected;
}
```
直接插入整个对象：
```java
public Integer insertDataWithObject(MyData myData) {
    Integer rowsAffected = jdbcClient.sql("insert into my_data values(:id,:name) ")
    .paramSource(myData)
    .update();
    return rowsAffected;
}
```
<a name="blm0S"></a>
## **五、总结**
通过上述的示例，可以看到基本的数据库操作都可以用 JdbcClient 实现，避免了复杂的ORM框架的使用，切操作要比ORM框架简单灵活的多。fluent API 的风格也更容易编写和阅读。<br />完整的数据库操作方法的调用演示：
```java
@Slf4j
@SpringBootApplication
public class Application implements CommandLineRunner {

    @Autowired
    private DbService dbService;

    @Override
    public void run(String... args) {
        MyData myData = new MyData(1L, "test");
        log.info("insert rows: {}", dbService.insertDataWithObject(myData));

        MyData myData2 = new MyData(2L, "test");
        dbService.insertDataWithParam(myData2);

        MyData myData3 = new MyData(3L, "author");
        dbService.insertDataWithNamedParam(myData3);

        log.info("findDataById: {}", dbService.findDataById(1L));
        log.info("findDataByName: {}", dbService.findDataByName("test"));
        log.info("findDataWithRowMapper: {}", dbService.findDataWithRowMapper());
        log.info("findDataByParamMap: {}", dbService.findDataByParamMap(Map.of("name", "author")));
        log.info("countByName: {}", dbService.countByName("test"));
    }

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
}
```
