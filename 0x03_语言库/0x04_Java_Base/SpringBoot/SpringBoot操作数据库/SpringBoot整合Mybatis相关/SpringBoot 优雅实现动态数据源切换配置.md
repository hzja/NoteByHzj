JavaSpringBoot
<a name="Bf2CM"></a>
### 前言
随着应用用户数量的增加，相应的并发请求的数量也会跟着不断增加，慢慢地，单个数据库已经没有办法满足频繁的数据库操作请求了，在某些场景下，可能会需要配置多个数据源，使用多个数据源(例如实现数据库的读写分离)来缓解系统的压力等。<br />同样的，Springboot官方提供了相应的实现来帮助开发者们配置多数据源，一般分为两种方式(目前所了解到的)，分包和AOP，在之前Springboot +Mybatis实现多数据源配置中，实现了静态多数据源的配置，但是这种方式怎么说呢，在实际的使用中不够灵活。<br />为了解决这个问题，可以使用上文提到的第二种方法，即使用AOP面向切面编程的方式配合自定义注解来实现在不同数据源之间动态切换的目的。
<a name="PRZpq"></a>
### 1、数据库准备
数据库准备仍然和之前的例子相同，具体建表sql语句则不再详细说明，表格如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655259545396-b41e3fa6-b1b2-4f0d-b6a0-56a3cb4f2663.png#clientId=u3c52838c-b10d-4&from=paste&id=ua5158be2&originHeight=151&originWidth=564&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u871dd2ab-7d76-486e-a300-94cb1fbe6c6&title=)<br />并分别插入两条记录，为了方便对比，其中testdatasource1为芳年25岁的张三，  testdatasource2为芳年30岁的李四。
<a name="xhFHL"></a>
### 2、环境准备
首先新建一个Springboot项目，这里版本是2.1.7.RELEASE，并在pom文件中引入相关依赖，和上次相比，这次主要额外新增了aop相关的依赖，如下:
```xml
<dependency>
	<groupId>mysql</groupId>
	<artifactId>mysql-connector-java</artifactId>
</dependency>

<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-jdbc</artifactId>
</dependency>


<dependency>
	<groupId>org.springframework</groupId>
	<artifactId>spring-aop</artifactId>
	<version>5.1.5.RELEASE</version>
</dependency>
<dependency>
	<groupId>junit</groupId>
	<artifactId>junit</artifactId>
</dependency>
<dependency>
	<groupId>org.aspectj</groupId>
	<artifactId>aspectjweaver</artifactId>
	<version>1.9.2</version>
</dependency>
```
<a name="Lg3Gk"></a>
### 3、代码部分
首先在Springboot的配置文件中配置datasourse，和以往不一样的是，因为有两个数据源，所以要指定相关数据库的名称，其中主数据源为primary，次数据源为secondary如下:
```
#配置主数据库
spring.datasource.primary.jdbc-url=jdbc:mysql://localhost:3306/testdatasource1?useUnicode=true&characterEncoding=UTF-8&serverTimezone=UTC&useSSL=false
spring.datasource.primary.username=root
spring.datasource.primary.password=root
spring.datasource.primary.driver-class-name=com.mysql.cj.jdbc.Driver

##配置次数据库
spring.datasource.secondary.jdbc-url=jdbc:mysql://localhost:3306/testdatasource2?useUnicode=true&characterEncoding=UTF-8&serverTimezone=UTC&useSSL=false
spring.datasource.secondary.username=root
spring.datasource.secondary.password=root
spring.datasource.secondary.driver-class-name=com.mysql.cj.jdbc.Driver


spring.http.encoding.charset=UTF-8
spring.http.encoding.enabled=true
spring.http.encoding.force=true
```
需要注意的是，Springboot2.0 在配置数据库连接的时候需要使用jdbc-url，如果只使用url的话会报jdbcUrl is required with driverClassName.错误。<br />新建一个配置文件，`DynamicDataSourceConfig` 用来配置相关的bean，代码如下
```java
@Configuration
@MapperScan(basePackages = "com.mzd.multipledatasources.mapper", sqlSessionFactoryRef = "SqlSessionFactory") //basePackages 我们接口文件的地址
public class DynamicDataSourceConfig {

    // 将这个对象放入Spring容器中
    @Bean(name = "PrimaryDataSource")
    // 表示这个数据源是默认数据源
    @Primary
    // 读取application.properties中的配置参数映射成为一个对象
    // prefix表示参数的前缀
    @ConfigurationProperties(prefix = "spring.datasource.primary")
    public DataSource getDateSource1() {
        return DataSourceBuilder.create().build();
    }


    @Bean(name = "SecondaryDataSource")
    @ConfigurationProperties(prefix = "spring.datasource.secondary")
    public DataSource getDateSource2() {
        return DataSourceBuilder.create().build();
    }

    
    @Bean(name = "dynamicDataSource")
    public DynamicDataSource DataSource(@Qualifier("PrimaryDataSource") DataSource primaryDataSource,
                                        @Qualifier("SecondaryDataSource") DataSource secondaryDataSource) {
        
        //这个地方是比较核心的targetDataSource 集合是我们数据库和名字之间的映射
        Map<Object, Object> targetDataSource = new HashMap<>();
        targetDataSource.put(DataSourceType.DataBaseType.Primary, primaryDataSource);
        targetDataSource.put(DataSourceType.DataBaseType.Secondary, secondaryDataSource);
        DynamicDataSource dataSource = new DynamicDataSource(); 
        dataSource.setTargetDataSources(targetDataSource);
        dataSource.setDefaultTargetDataSource(primaryDataSource);//设置默认对象
        return dataSource;
    }
    
    
    @Bean(name = "SqlSessionFactory")
    public SqlSessionFactory SqlSessionFactory(@Qualifier("dynamicDataSource") DataSource dynamicDataSource)
            throws Exception {
        SqlSessionFactoryBean bean = new SqlSessionFactoryBean();
        bean.setDataSource(dynamicDataSource);
        bean.setMapperLocations(
                new PathMatchingResourcePatternResolver().getResources("classpath*:mapping/*/*.xml"));//设置我们的xml文件路径
        return bean.getObject();
    }
}
```
而在这所有的配置中，最核心的地方就是`DynamicDataSource`这个类了，`DynamicDataSource`是自定义的动态切换数据源的类，该类继承了`AbstractRoutingDataSource` 类并重写了它的`determineCurrentLookupKey()`方法。<br />`AbstractRoutingDataSource` 类内部维护了一个名为`targetDataSources`的Map，并提供的setter方法用于设置数据源关键字与数据源的关系，实现类被要求实现其`determineCurrentLookupKey()`方法，由此方法的返回值决定具体从哪个数据源中获取连接。同时`AbstractRoutingDataSource`类提供了程序运行时动态切换数据源的方法，在dao类或方法上标注需要访问数据源的关键字，路由到指定数据源，获取连接。<br />`DynamicDataSource`代码如下:
```java
public class DynamicDataSource extends AbstractRoutingDataSource {

    @Override
    protected Object determineCurrentLookupKey() {
        DataSourceType.DataBaseType dataBaseType = DataSourceType.getDataBaseType();
        return dataBaseType;
    }

}
```
`DataSourceType`类的代码如下:
```java
public class DataSourceType {

    //内部枚举类，用于选择特定的数据类型
    public enum DataBaseType {
        Primary, Secondary
    }

    // 使用ThreadLocal保证线程安全
    private static final ThreadLocal<DataBaseType> TYPE = new ThreadLocal<DataBaseType>();

    // 往当前线程里设置数据源类型
    public static void setDataBaseType(DataBaseType dataBaseType) {
        if (dataBaseType == null) {
            throw new NullPointerException();
        }
        TYPE.set(dataBaseType);
    }

    // 获取数据源类型
    public static DataBaseType getDataBaseType() {
        DataBaseType dataBaseType = TYPE.get() == null ? DataBaseType.Primary : TYPE.get();
        return dataBaseType;
    }

    // 清空数据类型
    public static void clearDataBaseType() {
        TYPE.remove();
    }

}
```
接下来编写相关的Mapper和xml文件，代码如下:
```java
@Component
@Mapper
public interface PrimaryUserMapper {
	
	List<User> findAll();
}
```
```java
@Component
@Mapper
public interface SecondaryUserMapper {

    List<User> findAll();
}
```
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
				PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
				"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.jdkcb.mybatisstuday.mapper.one.PrimaryUserMapper">
	
	<select id="findAll" resultType="com.jdkcb.mybatisstuday.pojo.User">
		select * from sys_user;
	</select>
	
</mapper>
```
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
				PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
				"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.jdkcb.mybatisstuday.mapper.two.SecondaryUserMapper">
	
	<select id="findAll" resultType="com.jdkcb.mybatisstuday.pojo.User">
		select * from sys_user2;
	</select>
	
	
</mapper>
```
相关接口文件编写好之后，就可以编写aop代码了:
```java
@Aspect
@Component
public class DataSourceAop {
    //在primary方法前执行
    @Before("execution(* com.jdkcb.mybatisstuday.controller.UserController.primary(..))")
    public void setDataSource2test01() {
        System.err.println("Primary业务");
        DataSourceType.setDataBaseType(DataSourceType.DataBaseType.Primary);
    }

    //在secondary方法前执行
    @Before("execution(* com.jdkcb.mybatisstuday.controller.UserController.secondary(..))")
    public void setDataSource2test02() {
        System.err.println("Secondary业务");
        DataSourceType.setDataBaseType(DataSourceType.DataBaseType.Secondary);
    }
}
```
编写测试的 UserController:。<br />代码如下:
```java
@RestController
public class UserController {

    @Autowired
    private PrimaryUserMapper primaryUserMapper;
    @Autowired
    private SecondaryUserMapper secondaryUserMapper;


    @RequestMapping("primary")
    public Object primary(){
        List<User> list = primaryUserMapper.findAll();
        return list;
    }
    @RequestMapping("secondary")
    public Object secondary(){
        List<User> list = secondaryUserMapper.findAll();
        return list;
    }

}
```
<a name="kvOGS"></a>
### 4、测试
启动项目，在浏览器中分别输入http://127.0.0.1:8080/primary 和http://127.0.0.1:8080/primary ，结果如下:
```json
[{"user_id":1,"user_name":"张三","user_age":25}]

[{"user_id":1,"user_name":"李四","user_age":30}]
```
<a name="dn78Z"></a>
### 5、等等
AOP也有AOP的好处，比如两个包下的代码分别用两个不同的数据源，就可以直接用aop表达式就可以完成了，但是，如果想本例中方法级别的拦截，就显得优点不太灵活了，这个适合就需要注解上场了。
<a name="DOiet"></a>
### 6、配合注解实现
首先自定义注解 `@DataSource`
```java
/**
 * 切换数据注解 可以用于类或者方法级别 方法级别优先级 > 类级别
 */
@Target({ElementType.METHOD, ElementType.TYPE, ElementType.PARAMETER})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface DataSource {
	String value() default "primary"; //该值即key值，默认使用默认数据库
}
```
通过使用aop拦截，获取注解的属性value的值。如果value的值并没有在DataBaseType里面，则使用默认的数据源，如果有的话，则切换为相应的数据源。
```java
@Aspect
@Component
public class DynamicDataSourceAspect {
    private static final Logger logger = LoggerFactory.getLogger(DynamicDataSourceAspect.class);

    @Before("@annotation(dataSource)")//拦截我们的注解
    public void changeDataSource(JoinPoint point, DataSource dataSource) throws Throwable {
        String value = dataSource.value();
        if (value.equals("primary")){
            DataSourceType.setDataBaseType(DataSourceType.DataBaseType.Primary);
        }else if (value.equals("secondary")){
            DataSourceType.setDataBaseType(DataSourceType.DataBaseType.Secondary);
        }else {
            DataSourceType.setDataBaseType(DataSourceType.DataBaseType.Primary);//默认使用主数据库
        }

    }

    @After("@annotation(dataSource)") //清除数据源的配置
    public void restoreDataSource(JoinPoint point, DataSource dataSource) {
        DataSourceType.clearDataBaseType();


    }
}
```
<a name="lTIjJ"></a>
### 7、测试
修改mapper，添加自定义的`@DataSouse`注解，并注解掉`DataSourceAop`类里面的内容。如下:
```java
@Component
@Mapper
public interface PrimaryUserMapper {
	
	@DataSource
	List<User> findAll();
}

@Component
@Mapper
public interface SecondaryUserMapper {
	
	@DataSource("secondary")//指定数据源为:secondary
	List<User> findAll();
}
```
启动项目，在浏览器中分别输入http://127.0.0.1:8080/primary 和http://127.0.0.1:8080/primary ，结果如下:
```json
[{"user_id":1,"user_name":"张三","user_age":25}]

[{"user_id":1,"user_name":"李四","user_age":30}]
```
到此，就算真正的大功告成。
