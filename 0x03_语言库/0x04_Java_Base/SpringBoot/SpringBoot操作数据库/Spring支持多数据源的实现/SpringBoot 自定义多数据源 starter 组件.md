JavaSpringBoot<br />使用多数据源封装成一个starter组件，以方便使用多数据源访问数据库的操作<br />创建一个普通Java项目，引入SpringBoot相关的依赖<br />pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.6.3</version>
    <relativePath/>
  </parent>
  <groupId>com.gitee.kenewstar.multi.datasource</groupId>
  <artifactId>multi-datasource-spring-boot-starter</artifactId>
  <version>0.0.1</version>
  <name>multi-datasource-spring-boot-starter</name>
  <description>multi-datasource-spring-boot-starter</description>
  <properties>
    <java.version>1.8</java.version>
  </properties>
  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-configuration-processor</artifactId>
      <optional>true</optional>
    </dependency>

    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-jdbc</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-aop</artifactId>
    </dependency>
  </dependencies>

</project>
```
<a name="OFQJl"></a>
### 创建常量类和注解
```java
public interface Const {

    String DEFAULT = "default";

    String MULTI_DS = "multiDataSource";

    String CONFIG_PREFIX = "spring.datasource.multi";

}
```
数据源注解
```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface DataSource {

    String value() default Const.DEFAULT;

}
```
<a name="EvqHH"></a>
### 创建多数据源属性类
主要用于存储SpringBoot配置文件中配置的数据源属性
```java
@Component
@ConfigurationProperties(prefix = Const.CONFIG_PREFIX)
public class MultiDataSourceProperties {

    private Map<String, DataSourceProp> dataSourcePropMap;

    public Map<String, DataSourceProp> getDataSourcePropMap() {
        return dataSourcePropMap;
    }

    public void setDataSourcePropMap(Map<String, DataSourceProp> dataSourcePropMap) {
        this.dataSourcePropMap = dataSourcePropMap;
    }
}


public class DataSourceProp extends HashMap<String, String> {


}
```
<a name="VUzW5"></a>
### 创建数据源key的切换工具
主要用于设置当前线程下数据源切换时的数据源唯一标识key，以便获取指定的数据源
```java
public class DynamicDataSourceHolder {

    private static final ThreadLocal<String> DATA_SOURCE_THEAD_LOCAL =
            ThreadLocal.withInitial(() -> Const.DEFAULT);


    public static String getDataSource() {
        return DATA_SOURCE_THEAD_LOCAL.get();
    }

    public static void setDataSource(String dataSource) {
        DATA_SOURCE_THEAD_LOCAL.set(dataSource);
    }

    public static void remove() {
        DATA_SOURCE_THEAD_LOCAL.remove();
    }

}
```
<a name="w5fMr"></a>
### 创建多数据源类
创建多数据源类继承`AbstractRoutingDataSource`类，重写`determineCurrentLookupKey()`方法，用于获取当前线程中的指定的数据源key，通过该key拿到对应的数据源对象
```java
public class MultiDataSource extends AbstractRoutingDataSource {

    private static final Logger LOGGER = Logger.getLogger(MultiDataSource.class.getName());

    @Override
    protected Object determineCurrentLookupKey() {
        String key = DynamicDataSourceHolder.getDataSource();
        LOGGER.info("DataSource key ---> " + key);
        return key;
    }

}
```
<a name="GEbpP"></a>
### 创建多数据源的切面类
切面类主要用于获取被数据与注解指定的方法，拿到其注解中的属性值，再设置到数据源key设置组件中，方便数据源类获取该key<br />需使用`@Order`设置切面优先级，否则设置无效
```java
@Aspect
@Order(100)
public class DynamicDataSourceAdviser {

    @Pointcut("@annotation(com.gitee.kenewstar.multi.datasource.common.DataSource)")
    public void pointcut(){};

    @Around("pointcut()")
    public Object around(ProceedingJoinPoint point) throws Throwable {

        try {
            MethodSignature methodSignature = (MethodSignature) point.getSignature();
            //获取被代理的方法对象
            Method targetMethod = methodSignature.getMethod();
            // 获取数据源注解
            DataSource ds = targetMethod.getAnnotation(DataSource.class);
            if (Objects.nonNull(ds)) {
                DynamicDataSourceHolder.setDataSource(ds.value());
            }
            return point.proceed();
        } finally {
            DynamicDataSourceHolder.remove();
        }


    }

}
```
<a name="dVrma"></a>
### 创建数据源配置类
```java
@Configuration
@EnableConfigurationProperties(MultiDataSourceProperties.class)
public class MultiDataSourceConfig {

    public static final String DS_TYPE = "dsType";

    @Resource
    private MultiDataSourceProperties multiDataSourceProperties;

    private DataSource createDs(DataSourceProp dsProp) {
        DataSource dataSource = null;
        try {
            Class<?> dsClass = Class.forName(dsProp.get(DS_TYPE));
            if (DataSource.class.isAssignableFrom(dsClass)) {
                dataSource = (DataSource) dsClass.getConstructor().newInstance();

                DataSource finalDataSource = dataSource;
                ReflectionUtils.doWithFields(dsClass, field -> {
                    field.setAccessible(true);
                    field.set(finalDataSource, dsProp.get(field.getName()));
                }, field -> {
                    if (Objects.equals(dsProp.get(DS_TYPE), field.getName())) {
                        return false;
                    }
                    return Objects.nonNull(dsProp.get(field.getName()));
                });

            }

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        return dataSource;
    }

    @Bean(Const.MULTI_DS)
    @Primary
    public DataSource multiDataSource() {
        MultiDataSource multiDataSource = new MultiDataSource();

        Map<Object, Object> dataSourceMap = new HashMap<>(multiDataSourceProperties.getDataSourcePropMap().size());
        Map<String, DataSourceProp> dataSourcePropMap = multiDataSourceProperties.getDataSourcePropMap();
        dataSourcePropMap.forEach((lookupKey,dsProp) -> {
            dataSourceMap.put(lookupKey, createDs(dsProp));
        });

        multiDataSource.setTargetDataSources(dataSourceMap);
        multiDataSource.setDefaultTargetDataSource(dataSourceMap.get(Const.DEFAULT));
        return multiDataSource;
    }

    @Bean
    public DataSourceTransactionManager dataSourceTransactionManager(
            @Qualifier(Const.MULTI_DS) DataSource multiDataSource) {

        DataSourceTransactionManager tx = new DataSourceTransactionManager();
        tx.setDataSource(multiDataSource);
        return tx;
    }

    @Bean
    public DynamicDataSourceAdviser dynamicDataSourceAdviser() {
        return new DynamicDataSourceAdviser();
    }

}
```
<a name="qexuV"></a>
### 配置spring.factories文件
在resources目录下创建META-INF目录，在该目录创建spring.factories<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668236143746-bd7f90d7-f380-4ac9-826a-35249584a191.png#averageHue=%233e4244&clientId=u85c14c2b-5033-4&from=paste&id=u0d89e747&originHeight=120&originWidth=363&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u347d5e0c-1d6a-45ad-9db0-b53da73f848&title=)<br />文件内容如下：<br />设置key为开启自动配置的注解全路径名，后面的value值为配置类全路径名，本starter组件中为数据源配置类，如有多个配置类，则以逗号分隔，以反斜杆表示忽略换行
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
com.gitee.kenewstar.multi.datasource.config.MultiDataSourceConfig
```
这样封装的一个简单的多数据源starter组件就完成了，只需进行maven打包即可在本地使用<br />maven命令：`mvn clean install`
<a name="DRwUu"></a>
### 使用示例
引入打包后的依赖
```xml
<dependency>
  <groupId>com.gitee.kenewstar.multi.datasource</groupId>
  <artifactId>multi-datasource-spring-boot-starter</artifactId>
  <version>0.0.1</version>
</dependency>
```
修改SpringBoot全局配置文件<br />default为默认数据源，必须配置， master为可选数据源，名称可自定义。<br />数据源的属性名称为对应的dsType数据源类型的属性字段
```yaml
spring:
  datasource:
    multi:
      data-source-prop-map:
        default:
          dsType: com.zaxxer.hikari.HikariDataSource
          jdbcUrl: jdbc:mysql://localhost:3306/test
          username: root
          password: kenewstar
        master:
          dsType: com.zaxxer.hikari.HikariDataSource
          jdbcUrl: jdbc:mysql://localhost:3306/test2
          username: root
          password: kenewstar
```
使用数据源<br />直接在指定的方法上添加`@DataSource`注解即可，注解的默认值为default，数据源的切换通过注解的值进行切换。值为application.yml中配置的default，master等
```java
@Service
public class PersonService {

    @Resource
    private PersonMapper personMapper;

    @DataSource("master")
    @Transactional(rollbackFor = Exception.class)
    public void insertPerson() {
        personMapper.insert(new Person(null, "kk", 12));
        personMapper.insert(new Person(null, "kk", 12));
    }

}
```
```java
@Service
public class PersonService {

    @Resource
    private PersonMapper personMapper;

    @DataSource("master")
    @Transactional(rollbackFor = Exception.class)
    public void insertPerson() {
        personMapper.insert(new Person(null, "kk", 12));
        personMapper.insert(new Person(null, "kk", 12));
    }
}
```
多数据源starter组件源码地址：[https://gitee.com/kenewstar/multi-datasource-spring-boot-starter](https://gitee.com/kenewstar/multi-datasource-spring-boot-starter)
