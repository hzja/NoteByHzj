JavaSpringBoot<br />Spring Boot非常简单容易上手，它隐藏了很多内容而不需要去关心。但对于一个好的开发人员也许希望知道Spring Boot自动配置背后到底发生了什么？<br />Spring Boot并不属于一种新的技术，只不过Spring Boot的启动器配置了若干个被Spring管理的bean，当项目依赖这些jar并启动Spring应用时，Spring的Container容器已经把jar包下的对象加以创建及管理了。<br />简而言之，Spring Boot自动配置代表了一种基于类路径上存在的依赖关系自动配置Spring应用程序的方法。还可以通过定义消除自动配置类中包含的某些bean。这些可以使开发更快更容易。
<a name="cQSRp"></a>
## 1、通过启动类创建Spring Boot应用
创建Spring Boot应用非常简单，只要创建一个包含main的启动类即可。
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
 
@SpringBootApplication
public class App{
    public static void main(String[] args){
        ApplicationContext ctx = SpringApplication.run(App.class, args);
    }
}
```
上面这个类被称为Spring Boot应用的启动类，它通过一个java的`main()`方法来引导和启动一个Spring应用。它通常包含了以下内容：

- **创建一个Spring ApplicationContext实例。**
- **接收命令行参数并将其转为Spring属性。**
- **按照配置加载所有Spring Bean。可以根据项目需求进行其他操作。**
<a name="Dwdkt"></a>
## 2、`@SpringBootApplication`注解
这个注解其实是一个应用了3个注解的快捷方式。
<a name="Mf5kM"></a>
### **2.1 **`**@SpringBootConfiguration**`
`@SpringBootConfiguration`是在Spring Boot2中出现的一个新的注解。之前都是使用的 `@Configuration`注解，可以用 `@Configuration`来替换它，2个都是实现同样的功能。<br />它表示该类是一个配置类，应该对其进行扫描，以获得进一步的配置和bean定义。
<a name="aL8el"></a>
### **2.2 **`**@EnableAutoConfiguration**`
此注解用于启用Spring Application Context的自动配置，尝试猜测和配置可能需要的bean。自动配置类通常基于类路径以及定义的bean来应用。<br />自动配置尝试尽可能智能，并在定义更多自己的配置时进行后退。始终可以使用两种方法来手动排除任何不想应用的配置：

- **使用**`**excludeName()**`
- **使用**`**spring.autoconfigure.exclude**`**属性文件中的属性。**
<a name="QaRWP"></a>
### **2.3 **`**@ComponentScan**`
此注解提供了与Spring XML `context:component-scan`元素并行的支持。<br />无论是`basePackageClasses()`或`basePackages()`可以定义特定的软件包进行扫描。如果未定义特定包，则将从声明此注解的类的包进行扫描。
<a name="gEc6v"></a>
## 3、自定义自动配置
要创建自定义自动配置，需要创建一个注释为`@Configuration`的类并注册它。<br />为MySQL数据源创建自定义配置：
```java
@Configuration
public class MySQLAutoconfiguration {
  //...
}
```
下一个必须的步骤是通过在标准文件资源/META-INF/spring.factories中的属性org.springframework.boot.autoconfigure.EnableAutoConfiguration下添加类的名称，将类注册为自动配置候选者：
```java
org.springframework.boot.autoconfigure.EnableAutoConfiguration=com.peterwanghao.samples.springboot.autoconfiguration.MySQLAutoconfiguration
```
如果希望自动配置类优先于其他自动配置候选者，可以添加`@AutoConfigureOrder(Ordered.HIGHEST_PRECEDENCE)`注解。<br />自动配置是使用标有`@Conditional`注解的类和bean设计的，以便可以替换自动配置或其特定部分。<br />请注意，只有当应用程序中未定义自动配置的bean时，自动配置才有效。如果您定义了bean，那么将覆盖默认值。
<a name="bvqsA"></a>
### **3.1 基于类的条件注解**
Class conditions允许指定使用`@ConditionalOnClass`注解指定的类，或者使用`@ConditionalOnMissingClass`注解来指定不存在于 classpath 上的类。<br />指定只有存在类`DataSource`的情况下才会加载`MySQLConfiguration`，在这种情况下可以假设应用程序将使用数据库：
```java
@Configuration
@ConditionalOnClass(DataSource.class)
public class MySQLAutoconfiguration {
    //...
}
```
<a name="sRYVP"></a>
### **3.2 基于Bean的条件注解**
如果只想在指定的bean存在的情况下包含bean，可以使用`@ConditionalOnBean`和`@ConditionalOnMissingBean`注解。<br />举例说明，将一个entityManagerFactory bean 添加到配置类中，并指定如果存在一个名为dataSource的bean 并且尚未定义一个名为`entityManagerFactory`的 bean，就创建这个bean ：
```java
@Bean
@ConditionalOnBean(name = "dataSource")
@ConditionalOnMissingBean
public LocalContainerEntityManagerFactoryBean entityManagerFactory() {
    final LocalContainerEntityManagerFactoryBean em = new LocalContainerEntityManagerFactoryBean();
    em.setDataSource(dataSource());
    em.setPackagesToScan("com.peterwanghao.samples.springboot.autoconfiguration.example");
    em.setJpaVendorAdapter(new HibernateJpaVendorAdapter());
    if (additionalProperties() != null) {
        em.setJpaProperties(additionalProperties());
    }
    return em;
}
```
配置一个只在尚未定义类型为`JpaTransactionManager`的bean时才会加载的transactionManager bean ：
```java
@Bean
@ConditionalOnMissingBean(type = "JpaTransactionManager")
JpaTransactionManager transactionManager(final EntityManagerFactory entityManagerFactory) {
    final JpaTransactionManager transactionManager = new JpaTransactionManager();
    transactionManager.setEntityManagerFactory(entityManagerFactory);
    return transactionManager;
}
```
<a name="twqJC"></a>
### **3.3 基于属性的条件注解**
`@ConditionalOnProperty`注解用于指定是否配置将基于Spring环境属性的存在和值被加载。<br />首先，为配置添加一个属性源文件，以确定从哪里读取属性：
```java
@PropertySource("classpath:mysql.properties")
public class MySQLAutoconfiguration {
    //...
}
```
可以配置主DataSource bean，它将用于创建与数据库的连接，只有在存在名为usemysql的属性时才会加载它。<br />可以使用属性havingValue来指定必须匹配的usemysql属性的某些值。<br />如果usemysql属性设置为local，使用默认值定义dataSource bean，该默认值连接到名为myDb的本地数据库：
```java
@Bean
@ConditionalOnProperty(name = "usemysql", havingValue = "local")
@ConditionalOnMissingBean
public DataSource dataSource() {
    final DriverManagerDataSource dataSource = new DriverManagerDataSource();

    dataSource.setDriverClassName("com.mysql.cj.jdbc.Driver");
    dataSource.setUrl("jdbc:mysql://localhost:3306/myDb?createDatabaseIfNotExist=true&&serverTimezone=GMT%2B8");
    dataSource.setUsername("root");
    dataSource.setPassword("123456");

    return dataSource;
}
```
如果usemysql属性设置为自定义，则数据源 bean将使用自定义属性值的数据库URL，用户和密码进行配置：
```java
@Bean(name = "dataSource")
@ConditionalOnProperty(name = "usemysql", havingValue = "custom")
@ConditionalOnMissingBean
public DataSource dataSource2() {
    final DriverManagerDataSource dataSource = new DriverManagerDataSource();

    dataSource.setDriverClassName("com.mysql.cj.jdbc.Driver");
    dataSource.setUrl(env.getProperty("mysql.url"));
    dataSource.setUsername(env.getProperty("mysql.user") != null ? env.getProperty("mysql.user") : "");
    dataSource.setPassword(env.getProperty("mysql.pass") != null ? env.getProperty("mysql.pass") : "");

    return dataSource;
}
```
该mysql.properties文件将包含usemysql属性：
```
usemysql=local
```
如果使用`MySQLAutoconfiguration`的应用程序希望覆盖默认属性，则它需要做的就是为mysql.properties文件中的mysql.url，mysql.user和mysql.pass属性添加不同的值以及添加usemysql = custom行。
<a name="k8VSB"></a>
### **3.4 基于资源的条件注解**
添加`@ConditionalOnResource`注解意味着仅在存在指定资源时才加载配置。<br />让定义一个名为`additionalProperties()`的方法，该方法将返回一个Properties对象，该对象包含entityManagerFactory bean 使用的特定于Hibernate的属性，仅当存在资源文件mysql.properties时：
```java
@ConditionalOnResource(resources = "classpath:mysql.properties")
@Conditional(HibernateCondition.class)
final Properties additionalProperties() {
    final Properties hibernateProperties = new Properties();

    hibernateProperties.setProperty("hibernate.hbm2ddl.auto", env.getProperty("mysql-hibernate.hbm2ddl.auto"));
    hibernateProperties.setProperty("hibernate.dialect", env.getProperty("mysql-hibernate.dialect"));
    hibernateProperties.setProperty("hibernate.show_sql",
                                    env.getProperty("mysql-hibernate.show_sql") != null ? env.getProperty("mysql-hibernate.show_sql")
                                    : "false");

    return hibernateProperties;
}
```
可以将Hibernate特定的属性添加到mysql.properties文件中：
```
mysql-hibernate.dialect=org.hibernate.dialect.MySQL5InnoDBDialect
mysql-hibernate.show_sql=true
mysql-hibernate.hbm2ddl.auto=create-drop
```
<a name="kbKsk"></a>
### **3.5 自定义条件**
如果不想使用Spring Boot中的任何可用条件，还可以通过扩展`SpringBootCondition`类并重写`getMatchOutcome()`方法来定义自定义条件。<br />为`additionalProperties()`方法创建一个名为`HibernateCondition`的条件，该方法将验证类路径上是否存在`HibernateEntityManager`类：
```java
static class HibernateCondition extends SpringBootCondition {

    private static final String[] CLASS_NAMES = { "org.hibernate.ejb.HibernateEntityManager",
                                                 "org.hibernate.jpa.HibernateEntityManager" };

    @Override
    public ConditionOutcome getMatchOutcome(ConditionContext context, AnnotatedTypeMetadata metadata) {
        ConditionMessage.Builder message = ConditionMessage.forCondition("Hibernate");

        return Arrays.stream(CLASS_NAMES)
            .filter(className -> ClassUtils.isPresent(className, context.getClassLoader()))
            .map(className -> ConditionOutcome.match(message.found("class").items(Style.NORMAL, className)))
            .findAny().orElseGet(() -> ConditionOutcome.noMatch(
                message.didNotFind("class", "classes").items(Style.NORMAL, Arrays.asList(CLASS_NAMES))));
    }

}
```
然后可以将条件添加到`additionalProperties()`方法：
```java
@Conditional(HibernateCondition.class)
Properties additionalProperties() {
  //...
}
```
<a name="U7Na9"></a>
### **3.6 申请条件**
还可以通过添加`@ConditionalOnWebApplication`或`@ConditionalOnNotWebApplication`注释来指定只能在Web上下文内部/外部加载配置。
<a name="OKLNQ"></a>
## 4、测试自动配置
创建一个非常简单的例子来测试自动配置。将使用Spring Data 创建一个名为MyUser的实体类和一个`MyUserRepository`接口：
```java
@Entity
public class MyUser {
    @Id
    private String email;

    public MyUser() {
    }

    public MyUser(String email) {
        super();
        this.email = email;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

}

public interface MyUserRepository extends JpaRepository<MyUser, String> {

}
```
要启用自动配置，可以使用`@SpringBootApplication`或`@EnableAutoConfiguration`注解：
```java
@SpringBootApplication
public class AutoconfigurationApplication {

    public static void main(String[] args) {
        SpringApplication.run(AutoconfigurationApplication.class, args);
    }
}
```
接下来，编写一个保存MyUser实体的JUnit测试：
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = AutoconfigurationApplication.class)
@EnableJpaRepositories(basePackages = { "com.peterwanghao.samples.springboot.autoconfiguration.example" })
public class AutoconfigurationLiveTest {

    @Autowired
    private MyUserRepository userRepository;

    @Test
    public void whenSaveUser_thenOk() {
        MyUser user = new MyUser("user@email.com");
        userRepository.save(user);
    }

}
```
由于尚未定义DataSource配置，因此应用程序将使用创建的自动配置连接到名为myDb的MySQL数据库。<br />连接字符串包含`createDatabaseIfNotExist = true`属性，因此数据库不需要存在。但是，需要创建用户mysqluser或通过mysql.user属性指定的用户mysqluser。<br />可以检查应用程序日志，看看是否正在使用MySQL数据源：
```java
10:31:47.092 [main] INFO  org.hibernate.Version - HHH000412: Hibernate Core {5.3.7.Final}
10:31:47.094 [main] INFO  org.hibernate.cfg.Environment - HHH000206: hibernate.properties not found
10:31:47.227 [main] INFO  o.h.annotations.common.Version - HCANN000001: Hibernate Commons Annotations {5.0.4.Final}
10:31:48.039 [main] INFO  org.hibernate.dialect.Dialect - HHH000400: Using dialect: org.hibernate.dialect.MySQL5InnoDBDialect
Hibernate: drop table if exists MyUser
Hibernate: create table MyUser (email varchar(255) not null, primary key (email)) engine=InnoDB
10:31:48.655 [main] INFO  o.h.t.s.internal.SchemaCreatorImpl - HHH000476: Executing import script 'org.hibernate.tool.schema.internal.exec.ScriptSourceInputNonExistentImpl@3a0b6a'
10:31:48.666 [main] INFO  o.s.o.j.LocalContainerEntityManagerFactoryBean - Initialized JPA EntityManagerFactory for persistence unit 'default'
10:31:49.496 [main] INFO  o.s.s.c.ThreadPoolTaskExecutor - Initializing ExecutorService 'applicationTaskExecutor'
10:31:49.569 [main] WARN  o.s.b.a.o.j.JpaBaseConfiguration$JpaWebConfiguration$JpaWebMvcConfiguration - spring.jpa.open-in-view is enabled by default. Therefore, database queries may be performed during view rendering. Explicitly configure spring.jpa.open-in-view to disable this warning
10:31:49.701 [main] WARN  o.s.b.a.t.ThymeleafAutoConfiguration$DefaultTemplateResolverConfiguration - Cannot find template location: classpath:/templates/ (please add some templates or check your Thymeleaf configuration)
10:31:50.091 [main] INFO  c.p.s.s.a.AutoconfigurationLiveTest - Started AutoconfigurationLiveTest in 4.803 seconds (JVM running for 5.519)
Hibernate: select myuser0_.email as email1_0_0_ from MyUser myuser0_ where myuser0_.email=?
Hibernate: insert into MyUser (email) values (?)
10:31:50.279 [Thread-2] INFO  o.s.s.c.ThreadPoolTaskExecutor - Shutting down ExecutorService 'applicationTaskExecutor'
10:31:50.281 [Thread-2] INFO  o.s.o.j.LocalContainerEntityManagerFactoryBean - Closing JPA EntityManagerFactory for persistence unit 'default'
10:31:50.282 [Thread-2] INFO  o.h.t.s.i.SchemaDropperImpl$DelayedDropActionImpl - HHH000477: Starting delayed evictData of schema as part of SessionFactory shut-down'
Hibernate: drop table if exists MyUser
```
<a name="P4jmv"></a>
## 5、禁用自动配置类
如果想要从加载中排除自动配置，可以将带有`exclude`或`excludeName`属性的`@EnableAutoConfiguration`注解添加到配置类：
```java
@Configuration
@EnableAutoConfiguration(
  exclude={MySQLAutoconfiguration.class})
public class AutoconfigurationApplication {
    //...
}
```
禁用特定自动配置的另一个方法是设置`spring.autoconfigure.exclude`属性：
```
spring.autoconfigure.exclude=com.peterwanghao.samples.springboot.autoconfiguration.MySQLAutoconfiguration
```
<a name="SfY80"></a>
## 6、结论
介绍了Spring Boot是如何自动加载配置类，以及背后所隐藏的具体实现。展示了如何创建自定义Spring Boot自动配置。
