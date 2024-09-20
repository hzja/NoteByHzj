Java Spring
<a name="H8xCX"></a>
## 动态数据源是什么？解决了什么问题？
在实际的开发中，同一个项目中使用多个数据源是很常见的场景。比如，一个读写分离的项目存在主数据源与读数据源。<br />所谓动态数据源，就是通过Spring的一些配置来自动控制某段数据操作逻辑是走哪一个数据源。举个读写分离的例子，项目中引用了两个数据源，master、slave。通过Spring配置或扩展能力来使得一个接口中调用了查询方法会自动使用slave数据源。<br />一般实现这种效果可以通过：

1. 使用`@MapperScan`注解指定某个包下的所有方法走固定的数据源（这个比较死板些，会产生冗余代码，到也可以达到效果，可以作为临时方案使用）;
2. 使用注解+AOP+`AbstractRoutingDataSource`的形式来指定某个方法下的数据库操作是走那个数据源。
3. 通过 Sharding-JDBC 组件来实现（需要引入外部依赖，如果项目本身引用了该组件，建议用这种方式实现）
<a name="uAGHS"></a>
## 关键核心类
这里主要介绍通过注解+AOP+`AbstractRoutingDataSource`的联动来实现动态数据源的方式。<br />一切的起点是`AbstractRoutingDataSource`这个类，此类实现了 `DataSource` 接口
```java
public abstract class AbstractRoutingDataSource extends AbstractDataSource implements InitializingBean {

    // .... 省略 ... 

    @Nullable
    private Map<Object, Object> targetDataSources;

    @Nullable
    private Map<Object, DataSource> resolvedDataSources;


    public void setTargetDataSources(Map<Object, Object> targetDataSources) {
        this.targetDataSources = targetDataSources;
    }

    public void setDefaultTargetDataSource(Object defaultTargetDataSource) {
        this.defaultTargetDataSource = defaultTargetDataSource;
    }

    @Override
    public void afterPropertiesSet() {

        // 初始化 targetDataSources、resolvedDataSources
        if (this.targetDataSources == null) {
            throw new IllegalArgumentException("Property 'targetDataSources' is required");
        }
        this.resolvedDataSources = new HashMap<>(this.targetDataSources.size());
        this.targetDataSources.forEach((key, value) -> {
            Object lookupKey = resolveSpecifiedLookupKey(key);
            DataSource dataSource = resolveSpecifiedDataSource(value);
            this.resolvedDataSources.put(lookupKey, dataSource);
        });
        if (this.defaultTargetDataSource != null) {
            this.resolvedDefaultDataSource = resolveSpecifiedDataSource(this.defaultTargetDataSource);
        }
    }


    @Override
    public Connection getConnection() throws SQLException {
        return determineTargetDataSource().getConnection();
    }

    @Override
    public Connection getConnection(String username, String password) throws SQLException {
        return determineTargetDataSource().getConnection(username, password);
    }


    /**
  * Retrieve the current target DataSource. Determines the
  * {@link #determineCurrentLookupKey() current lookup key}, performs
  * a lookup in the {@link #setTargetDataSources targetDataSources} map,
  * falls back to the specified
  * {@link #setDefaultTargetDataSource default target DataSource} if necessary.
  * @see #determineCurrentLookupKey()
  */
    protected DataSource determineTargetDataSource() {
        Assert.notNull(this.resolvedDataSources, "DataSource router not initialized");

        // @1 start
        Object lookupKey = determineCurrentLookupKey();
        DataSource dataSource = this.resolvedDataSources.get(lookupKey);
        // @1 end

        if (dataSource == null && (this.lenientFallback || lookupKey == null)) {
            dataSource = this.resolvedDefaultDataSource;
        }
        if (dataSource == null) {
            throw new IllegalStateException("Cannot determine target DataSource for lookup key [" + lookupKey + "]");
        }
        return dataSource;
    }

    /**
  * 返回一个key，这个key用来从 resolvedDataSources 数据源中获取具体的数据源对象 见 @1
  */
    @Nullable
    protected abstract Object determineCurrentLookupKey();

}
```
可以看到 `AbstractRoutingDataSource` 中有个可扩展抽象方法 `determineCurrentLookupKey()`，利用这个方法可以来实现动态数据源效果。
<a name="RezdX"></a>
## 从0写一个简单动态数据源组件
通过实现`AbstractRoutingDataSource`的 `determineCurrentLookupKey()` 方法动态设置一个key，然后在配置类下通过`setTargetDataSources()`方法设置提前准备好的DataSource Map。
<a name="Xavf5"></a>
### 注解、常量定义、ThreadLocal 准备
```java
/**
 * @Summary 动态数据源注解定义
 */
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface MyDS {
    String value() default "default";
}

/**
 * @author axin
 * @Summary 动态数据源常量
 */
public interface DSConst {

    String 默认 = "default";

    String 主库 = "master";

    String 从库 = "slave";

    String 统计 = "stat";
}

```
```java
/**
 * @Summary 动态数据源 ThreadLocal 工具
 */
public class DynamicDataSourceHolder {

    //保存当前线程所指定的DataSource
    private static final ThreadLocal<String> THREAD_DATA_SOURCE = new ThreadLocal<>();

    public static String getDataSource() {
        return THREAD_DATA_SOURCE.get();
    }

    public static void setDataSource(String dataSource) {
        THREAD_DATA_SOURCE.set(dataSource);
    }

    public static void removeDataSource() {
        THREAD_DATA_SOURCE.remove();
    }
}
```
<a name="pbGEg"></a>
### 自定义一个 `AbstractRoutingDataSource` 类
```java

/**
 * @Summary 动态数据源
 */
public class DynamicDataSource extends AbstractRoutingDataSource {

    /**
     * 从数据源中获取目标数据源的key
     * @return
     */
    @Override
    protected Object determineCurrentLookupKey() {
        // 从ThreadLocal中获取key
        String dataSourceKey = DynamicDataSourceHolder.getDataSource();
        if (StringUtils.isEmpty(dataSourceKey)) {
            return DSConst.默认;
        }
        return dataSourceKey;
    }
}
```
<a name="kXMdA"></a>
### AOP 实现
```java

/**
 * @Summary 数据源切换AOP
 */
@Slf4j
@Aspect
@Service
public class DynamicDataSourceAOP {

    public DynamicDataSourceAOP() {
        log.info("/*---------------------------------------*/");
        log.info("/*----------                   ----------*/");
        log.info("/*---------- 动态数据源初始化... ----------*/");
        log.info("/*----------                   ----------*/");
        log.info("/*---------------------------------------*/");
    }

    /**
     * 切点
     */
    @Pointcut(value = "@annotation(xxx.xxx.MyDS)")
    private void method(){}

    /**
     * 方法执行前，切换到指定的数据源
     * @param point
     */
    @Before("method()")
    public void before(JoinPoint point) {
        MethodSignature methodSignature = (MethodSignature) point.getSignature();
        //获取被代理的方法对象
        Method targetMethod = methodSignature.getMethod();
        //获取被代理方法的注解信息
        CultureDS cultureDS = AnnotationUtils.findAnnotation(targetMethod, CultureDS.class);

        // 方法链条最外层的动态数据源注解优先级最高
        String key = DynamicDataSourceHolder.getDataSource();

        if (!StringUtils.isEmpty(key)) {
            log.warn("提醒：动态数据源注解调用链上出现覆盖场景，请确认是否无问题");
            return;
        }

        if (cultureDS != null ) {
            //设置数据库标志
            DynamicDataSourceHolder.setDataSource(MyDS.value());
        }
    }

    /**
     * 释放数据源
     */
    @AfterReturning("method()")
    public void doAfter() {
        DynamicDataSourceHolder.removeDataSource();
    }
}
```
<a name="kXaMu"></a>
### `DataSourceConfig` 配置
通过以下代码来将动态数据源配置到 SqlSession 中去
```java

/**
 * 数据源的一些配置，主要是配置读写分离的sqlsession，这里没有使用mybatis annotation
 *
@Configuration
@EnableTransactionManagement
@EnableAspectJAutoProxy
class DataSourceConfig {

    /** 可读写的SQL Session */
public static final String BEANNAME_SQLSESSION_COMMON = "sqlsessionCommon";
/** 事务管理器的名称，如果有多个事务管理器时，需要指定beanName */
public static final String BEANNAME_TRANSACTION_MANAGER = "transactionManager";

/** 主数据源，必须配置，spring启动时会执行初始化数据操作（无论是否真的需要），选择查找DataSource class类型的数据源 配置通用数据源，可读写，连接的是主库 */
@Bean
@Primary
@ConfigurationProperties(prefix = "datasource.common")
public DataSource datasourceCommon() {
    // 数据源配置 可更换为其他实现方式
    return DataSourceBuilder.create().build();
}

/**
     * 动态数据源
     * @returnr
     */
@Bean
public DynamicDataSource dynamicDataSource() {
    DynamicDataSource dynamicDataSource = new DynamicDataSource();
    LinkedHashMap<Object, Object> hashMap = Maps.newLinkedHashMap();
    hashMap.put(DSConst.默认, datasourceCommon());
    hashMap.put(DSConst.主库, datasourceCommon());
    hashMap.put(DSConst.从库, datasourceReadOnly());
    hashMap.put(DSConst.统计, datasourceStat());

    // 初始化数据源 Map
    dynamicDataSource.setTargetDataSources(hashMap);
    dynamicDataSource.setDefaultTargetDataSource(datasourceCommon());
    return dynamicDataSource;
}

/**
     * 配置事务管理器
     */
@Primary
@Bean(name = BEANNAME_TRANSACTION_MANAGER)
public DataSourceTransactionManager createDataSourceTransactionManager2() {
    DataSource dataSource = this.dynamicDataSource();
    DataSourceTransactionManager manager = new DataSourceTransactionManager(dataSource);
    return manager;
}

/**
     * 配置读写sqlsession
     */
@Primary
@Bean(name = BEANNAME_SQLSESSION_COMMON)
public SqlSession readWriteSqlSession() throws Exception {
    SqlSessionFactoryBean factory = new SqlSessionFactoryBean();

    // 设置动态数据源
    factory.setDataSource(this.dynamicDataSource());
    PathMatchingResourcePatternResolver resolver = new PathMatchingResourcePatternResolver();
    factory.setConfigLocation(resolver.getResource("mybatis/mybatis-config.xml"));
    factory.setMapperLocations(resolver.getResources("mybatis/mappers/**/*.xml"));
    return new SqlSessionTemplate(factory.getObject());
}
}
```
<a name="VzBno"></a>
## 总结
综上，利用AOP+注解实现了一个简单的Spring动态数据源功能，使用的时候，仅需要在目标方法上加上 `@MyDS` 注解即可。许多开源组件，会在现有的基础上增加一个扩展功能，比如路由策略等等。<br />顺便聊一下 sharding-jdbc 的实现方式，更新写入类sql自动走主库，查询类自动走读库，如果是新项目无历史债务的话，是可以使用该方案的。如果是在原有旧的项目上进行读写分离改造，那如果使用了 sharding-jdbc 读写分离方案，就必须梳理已有代码逻辑中的sql调用情况，来避免主从延迟造成数据不一致对业务的影响。<br />主从延迟造成读取数据不一致的情况是指：主从在同步的时候是有一定的延迟时间的，不管是什么网络的情况，这个延迟的值都是存在的，一般在毫秒级左右。这个时候如果使用sharding-jdbc进行读写分离处理，进行实时数据插入并查询判断的时候，就会出现判断异常的情况。
