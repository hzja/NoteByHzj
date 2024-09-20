Java SpringBoot JPA
<a name="waWga"></a>
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

# 日志打印执行的SQL
spring.jpa.show-sql=true
# Hibernate的DDL策略
spring.jpa.hibernate.ddl-auto=create-drop
```
这里除了JPA自身相关的配置之外，与JdbcTemplate配置时候的数据源配置完全是一致的
<a name="PsR5C"></a>
### 说明与注意：

1. 多数据源配置的时候，与单数据源不同点在于`spring.datasource`之后多设置一个数据源名称primary和secondary来区分不同的数据源配置，这个前缀将在后续初始化数据源的时候用到。
2. 数据源连接配置2.x和1.x的配置项是有区别的：2.x使用`spring.datasource.secondary.jdbc-url`，而1.x版本使用`spring.datasource.secondary.url`。如果在配置的时候发生了这个报错`java.lang.IllegalArgumentException: jdbcUrl is required with driverClassName.`，那么就是这个配置项的问题。
<a name="fZYij"></a>
## 初始化数据源与JPA配置
完成多数据源的配置信息之后，就来创建个配置类来加载这些配置信息，初始化数据源，以及初始化每个数据源要用的JdbcTemplate。<br />由于JPA的配置要比JdbcTemplate的负责很多，所以将配置拆分一下来处理：
<a name="lwZ6z"></a>
### 单独建一个多数据源的配置类，比如下面这样：
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

}
```
可以看到内容跟JdbcTemplate时候是一模一样的。通过`@ConfigurationProperties`可以知道这两个数据源分别加载了`spring.datasource.primary.*`和`spring.datasource.secondary.*`的配置。`@Primary`注解指定了主数据源，就是当不特别指定哪个数据源的时候，就会使用这个Bean真正差异部分在下面的JPA配置上。
<a name="qBM6o"></a>
### 分别创建两个数据源的JPA配置。
<a name="OskMC"></a>
#### Primary数据源的JPA配置：
```java
@Configuration
@EnableTransactionManagement
@EnableJpaRepositories(
        entityManagerFactoryRef="entityManagerFactoryPrimary",
        transactionManagerRef="transactionManagerPrimary",
        basePackages= { "com.fcant.repository.primary" }) //设置Repository所在位置
public class PrimaryConfig {

    @Autowired
    @Qualifier("primaryDataSource")
    private DataSource primaryDataSource;

    @Autowired
    private JpaProperties jpaProperties;
    @Autowired
    private HibernateProperties hibernateProperties;

    private Map<String, Object> getVendorProperties() {
        return hibernateProperties.determineHibernateProperties(jpaProperties.getProperties(), new HibernateSettings());
    }

    @Primary
    @Bean(name = "entityManagerPrimary")
    public EntityManager entityManager(EntityManagerFactoryBuilder builder) {
        return entityManagerFactoryPrimary(builder).getObject().createEntityManager();
    }

    @Primary
    @Bean(name = "entityManagerFactoryPrimary")
    public LocalContainerEntityManagerFactoryBean entityManagerFactoryPrimary (EntityManagerFactoryBuilder builder) {
        return builder
                .dataSource(primaryDataSource)
                .packages("com.fcant.entity.primary") //设置实体类所在位置
                .persistenceUnit("primaryPersistenceUnit")
                .properties(getVendorProperties())
                .build();
    }

    @Primary
    @Bean(name = "transactionManagerPrimary")
    public PlatformTransactionManager transactionManagerPrimary(EntityManagerFactoryBuilder builder) {
        return new JpaTransactionManager(entityManagerFactoryPrimary(builder).getObject());
    }

}
```
<a name="AbT6b"></a>
#### Secondary数据源的JPA配置：
```java
@Configuration
@EnableTransactionManagement
@EnableJpaRepositories(
        entityManagerFactoryRef="entityManagerFactorySecondary",
        transactionManagerRef="transactionManagerSecondary",
        basePackages= { "com.fcant.repository.secondary" }) //设置Repository所在位置
public class SecondaryConfig {

    @Autowired
    @Qualifier("secondaryDataSource")
    private DataSource secondaryDataSource;

    @Autowired
    private JpaProperties jpaProperties;
    @Autowired
    private HibernateProperties hibernateProperties;

    private Map<String, Object> getVendorProperties() {
        return hibernateProperties.determineHibernateProperties(jpaProperties.getProperties(), new HibernateSettings());
    }

    @Bean(name = "entityManagerSecondary")
    public EntityManager entityManager(EntityManagerFactoryBuilder builder) {
        return entityManagerFactorySecondary(builder).getObject().createEntityManager();
    }

    @Bean(name = "entityManagerFactorySecondary")
    public LocalContainerEntityManagerFactoryBean entityManagerFactorySecondary (EntityManagerFactoryBuilder builder) {
        return builder
                .dataSource(secondaryDataSource)
                .packages("com.fcant.entity.secondary") //设置实体类所在位置
                .persistenceUnit("secondaryPersistenceUnit")
                .properties(getVendorProperties())
                .build();
    }

    @Bean(name = "transactionManagerSecondary")
    PlatformTransactionManager transactionManagerSecondary(EntityManagerFactoryBuilder builder) {
        return new JpaTransactionManager(entityManagerFactorySecondary(builder).getObject());
    }

}
```
说明与注意：

- 在使用JPA的时候，需要为不同的数据源创建不同的package来存放对应的Entity和Repository，以便于配置类的分区扫描
- 类名上的注解`@EnableJpaRepositories`中指定Repository的所在位置
- `LocalContainerEntityManagerFactoryBean`创建的时候，指定Entity所在的位置
- 其他主要注意在互相注入时候，不同数据源不同配置的命名，基本就没有什么大问题了
<a name="F9Rbh"></a>
## 测试一下
完成了上面之后，就可以写个测试类来尝试一下上面的多数据源配置是否正确了，比如下面这样：
```java
@Slf4j
@RunWith(SpringRunner.class)
@SpringBootTest
public class Chapter38ApplicationTests {

    @Autowired
    private UserRepository userRepository;
    @Autowired
    private MessageRepository messageRepository;

    @Test
    public void test() throws Exception {
        userRepository.save(new User("aaa", 10));
        userRepository.save(new User("bbb", 20));
        userRepository.save(new User("ccc", 30));
        userRepository.save(new User("ddd", 40));
        userRepository.save(new User("eee", 50));

        Assert.assertEquals(5, userRepository.findAll().size());

        messageRepository.save(new Message("o1", "aaaaaaaaaa"));
        messageRepository.save(new Message("o2", "bbbbbbbbbb"));
        messageRepository.save(new Message("o3", "cccccccccc"));

        Assert.assertEquals(3, messageRepository.findAll().size());
    }

}
```
说明与注意：

- 测试验证的逻辑很简单，就是通过不同的Repository往不同的数据源插入数据，然后查询一下总数是否是对的
- 这里省略了Entity和Repository的细节
