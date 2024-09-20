Java SpringBoot JDBCTemplate
<a name="jmhfi"></a>
## 添加多数据源的配置
先在Spring Boot的配置文件application.properties中设置两个要链接的数据库配置，比如这样：
```
spring.datasource.primary.jdbc-url=jdbc:mysql://localhost:3306/test1
spring.datasource.primary.username=root
spring.datasource.primary.password=123456
spring.datasource.primary.driver-class-name=com.mysql.cj.jdbc.Driver

spring.datasource.secondary.jdbc-url=jdbc:mysql://localhost:3306/test2
spring.datasource.secondary.username=root
spring.datasource.secondary.password=123456
spring.datasource.secondary.driver-class-name=com.mysql.cj.jdbc.Driver
```
说明与注意：

1. 多数据源配置的时候，与单数据源不同点在于`spring.datasource`之后多设置一个数据源名称`primary`和`secondary`来区分不同的数据源配置，这个前缀将在后续初始化数据源的时候用到。
2. 数据源连接配置2.x和1.x的配置项是有区别的：2.x使用`spring.datasource.secondary.jdbc-url`，而1.x版本使用`spring.datasource.secondary.url`。
3. 如果在配置的时候发生了这个报错`java.lang.IllegalArgumentException: jdbcUrl is required with driverClassName.`，那么就是这个配置项的问题。
<a name="iue7E"></a>
## 初始化数据源与JdbcTemplate
完成多数据源的配置信息之后，就来创建个配置类来加载这些配置信息，初始化数据源，以及初始化每个数据源要用的JdbcTemplate。只需要在Spring Boot应用下添加下面的这个配置类即可完成！
```java
@Configuration
public class DataSourceConfiguration {

    @Primary
    @Bean
    @ConfigurationProperties(prefix = "spring.datasource.primary")
    public DataSource primaryDataSource() {
        return DataSourceBuilder.create().build();
    }

    @Bean
    @ConfigurationProperties(prefix = "spring.datasource.secondary")
    public DataSource secondaryDataSource() {
        return DataSourceBuilder.create().build();
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
说明与注意：

1. 前两个Bean是数据源的创建，通过`@ConfigurationProperties`可以知道这两个数据源分别加载了`spring.datasource.primary.*`和`spring.datasource.secondary.*`的配置。
2. `@Primary`注解指定了主数据源，就是当不特别指定哪个数据源的时候，就会使用这个Bean
3. 后两个Bean是每个数据源对应的`JdbcTemplate`。可以看到这两个`JdbcTemplate`创建的时候，分别注入了`primaryDataSource`数据源和`secondaryDataSource`数据源
<a name="UE0fT"></a>
## 测试一下
完成了上面之后，就可以写个测试类来尝试一下上面的多数据源配置是否正确了，比如下面这样：
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class Chapter37ApplicationTests {

    @Autowired
    protected JdbcTemplate primaryJdbcTemplate;

    @Autowired
    protected JdbcTemplate secondaryJdbcTemplate;

    @Before
    public void setUp() {
        primaryJdbcTemplate.update("DELETE  FROM  USER ");
        secondaryJdbcTemplate.update("DELETE  FROM  USER ");
    }

    @Test
    public void test() throws Exception {
        // 往第一个数据源中插入 2 条数据
        primaryJdbcTemplate.update("insert into user(name,age) values(?, ?)", "aaa", 20);
        primaryJdbcTemplate.update("insert into user(name,age) values(?, ?)", "bbb", 30);

        // 往第二个数据源中插入 1 条数据，若插入的是第一个数据源，则会主键冲突报错
        secondaryJdbcTemplate.update("insert into user(name,age) values(?, ?)", "ccc", 20);

        // 查一下第一个数据源中是否有 2 条数据，验证插入是否成功
        Assert.assertEquals("2", primaryJdbcTemplate.queryForObject("select count(1) from user", String.class));

        // 查一下第一个数据源中是否有 1 条数据，验证插入是否成功
        Assert.assertEquals("1", secondaryJdbcTemplate.queryForObject("select count(1) from user", String.class));
    }

}
```
说明与注意：

1. 可能这里有两个JdbcTemplate，为什么不用`@Qualifier`指定？这里顺带说个小知识点，当不指定的时候，会采用参数的名字来查找Bean，存在的话就注入。
2. 这两个JdbcTemplate创建的时候，也没指定名字，它们是如何匹配上的？这里也是一个小知识点，当创建Bean的时候，默认会使用方法名称来作为Bean的名称，所以这里就对应上了。不妨回头看看两个名称是不是一致的。
