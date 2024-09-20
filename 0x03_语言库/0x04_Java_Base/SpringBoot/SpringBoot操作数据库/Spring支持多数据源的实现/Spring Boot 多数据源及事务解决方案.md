JavaSpringBoot<br />一个主库和N个应用库的数据源，并且会同时操作主库和应用库的数据，需要解决以下两个问题：

- 如何动态管理多个数据源以及切换？
- 如何保证多数据源场景下的数据一致性(事务)？

本文主要探讨这两个问题的解决方案，希望能对读者有一定的启发。
<a name="tNelv"></a>
## 1、数据源切换原理
通过扩展Spring提供的抽象类`AbstractRoutingDataSource`，可以实现切换数据源。其类结构如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240419339-c148a359-3bec-4529-a130-ca52df6e1c81.png#averageHue=%23f4f4f4&clientId=u777f422b-bb5c-4&from=paste&id=u29bb22c0&originHeight=777&originWidth=410&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9d0ff48d-5469-43f6-abe6-4a2befa803b&title=)

- `targetDataSources`&`defaultTargetDataSource`

项目上需要使用的所有数据源和默认数据源。

- `resolvedDataSources`&`resolvedDefaultDataSource`

当Spring容器创建`AbstractRoutingDataSource`对象时，通过调用`afterPropertiesSet`复制上述目标数据源。由此可见，一旦数据源实例对象创建完毕，业务无法再添加新的数据源。

- `determineCurrentLookupKey`

此方法为抽象方法，通过扩展这个方法来实现数据源的切换。目标数据源的结构为：`Map<Object, DataSource>`其key为lookup key。<br />来看官方对这个方法的注释：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240419388-d90efeb6-3489-42b5-8bb9-5e813e6c37c6.png#averageHue=%23fbfaf9&clientId=u777f422b-bb5c-4&from=paste&id=u61b7d2bf&originHeight=151&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud5548311-4341-433e-bdd8-086225a7891&title=)<br />lookup key通常是绑定在线程上下文中，根据这个key去`resolvedDataSources`中取出DataSource。<br />根据目标数据源的管理方式不同，可以使用基于配置文件和数据库表两种方式。基于配置文件管理方案无法后续添加新的数据源，而基于数据库表方案管理，则更加灵活。
<a name="HTHkd"></a>
## 2、配置文件解决方案
根据上面的分析，可以按照下面的步骤去实现：

- 定义`DynamicDataSource`类继承`AbstractRoutingDataSource`，重写`determineCurrentLookupKey()`方法。
- 配置多个数据源注入`targetDataSources`和`defaultTargetDataSource`，通过`afterPropertiesSet()`方法将数据源写入`resolvedDataSources`和`resolvedDefaultDataSource`。
- 调用`AbstractRoutingDataSource`的`getConnection()`方法时，`determineTargetDataSource()`方法返回`DataSource`执行底层的`getConnection()`。

其流程如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240419348-8ceba420-bb57-4676-8ab9-059193a83997.png#averageHue=%23fbfbfb&clientId=u777f422b-bb5c-4&from=paste&id=u78c012db&originHeight=817&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0bb9c880-97e9-4338-b572-d24385c4f27&title=)
<a name="dlh3t"></a>
### 2.1 创建数据源
`DynamicDataSource`数据源的注入，目前业界主流实现步骤如下：<br />在配置文件中定义数据源
```
spring.datasource.type=com.alibaba.druid.pool.DruidDataSource
spring.datasource.driverClassName=com.mysql.jdbc.Driver
# 主数据源
spring.datasource.druid.master.url=jdbcUrl
spring.datasource.druid.master.username=***
spring.datasource.druid.master.password=***
# 其他数据源
spring.datasource.druid.second.url=jdbcUrl
spring.datasource.druid.second.username=***
spring.datasource.druid.second.password=***
```
在代码中配置Bean
```java
@Configuration
public class DynamicDataSourceConfig {
    @Bean
    @ConfigurationProperties("spring.datasource.druid.master")
    public DataSource firstDataSource(){
        return DruidDataSourceBuilder.create().build();
    }

    @Bean
    @ConfigurationProperties("spring.datasource.druid.second")
    public DataSource secondDataSource(){
        return DruidDataSourceBuilder.create().build();
    }

    @Bean
    @Primary
    public DynamicDataSource dataSource(DataSource firstDataSource, DataSource secondDataSource) {
        Map<Object, Object> targetDataSources = new HashMap<>(5);
        targetDataSources.put(DataSourceNames.FIRST, firstDataSource);
        targetDataSources.put(DataSourceNames.SECOND, secondDataSource);
        return new DynamicDataSource(firstDataSource, targetDataSources);
    }
}
```
<a name="uUTE0"></a>
### 2.2 AOP处理
通过`DataSourceAspect`切面技术来简化业务上的使用，只需要在业务方法添加`@SwitchDataSource`注解即可完成动态切换：
```java
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD})
public @interface SwitchDataSource {
    String value();
}
```
`DataSourceAspect`拦截业务方法，更新当前线程上下文`DataSourceContextHolder`中存储的key，即可实现数据源切换。
<a name="Z1IBL"></a>
### 2.3 方案不足
基于`AbstractRoutingDataSource`的多数据源动态切换，有个明显的缺点，无法动态添加和删除数据源。在产品中，不能把应用数据源写死在配置文件。接下来分享一下基于数据库表的实现方案。
<a name="cn4cA"></a>
## 3、数据库表解决方案
需要实现可视化的数据源管理，并实时查看数据源的运行状态。所以不能把数据源全部配置在文件中，应该将数据源定义保存到数据库表。参考`AbstractRoutingDataSource`的设计思路，实现自定义数据源管理。
<a name="LAh5H"></a>
### 3.1 设计数据源表
主库的数据源信息仍然配置在项目配置文件中，应用库数据源配置参数，则设计对应的数据表。表结构如下所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240419349-5f14a9cd-0cda-43c6-b033-9bc1ed196b17.png#averageHue=%23e9e9e9&clientId=u777f422b-bb5c-4&from=paste&id=uc6e1f577&originHeight=786&originWidth=341&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud2b1cbb4-fbee-4493-a19f-bb2d6467ff6&title=)<br />这个表主要就是DataSource的相关配置参数，其相应的ORM操作代码在此不再赘述，主要是实现数据源的增删改查操作。
<a name="H8FTk"></a>
### 3.2 自定义数据源管理
<a name="ADzAk"></a>
#### 3.2.1 定义管理接口
通过继承`AbstractDataSource`即可实现`DynamicDataSource`。为了方便对数据源进行操作，定义一个接口`DataSourceManager`，为业务提供操作数据源的统一接口。
```java
public interface DataSourceManager {
    void put(String var1, DataSource var2);

    DataSource get(String var1);

    Boolean hasDataSource(String var1);

    void remove(String var1);

    void closeDataSource(String var1);

    Collection<DataSource> all();
}
```
该接口主要是对数据表中定义的数据源，提供基础管理功能。
<a name="FQH4f"></a>
#### 3.2.2 自定义数据源
`DynamicDataSource`的实现如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240419383-7f75b979-bf23-4789-9db4-ff3407461e01.png#averageHue=%23f4f4f3&clientId=u777f422b-bb5c-4&from=paste&id=u3ecbbf07&originHeight=861&originWidth=567&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3b56a300-f898-4c22-a186-241db5f65c8&title=)<br />根据前面的分析，`AbstractRoutingDataSource`是在容器启动的时候，执行`afterPropertiesSet`注入数据源对象，完成之后无法对数据源进行修改。`DynamicDataSource`则实现`DataSourceManager`接口，可以将数据表中的数据源加载到dataSources。
<a name="xYJAr"></a>
#### 3.2.3 切面处理
这一块的处理跟配置文件数据源方案处理方式相同，都是通过AOP技术切换lookup key。
```java
public DataSource determineTargetDataSource() {
    String lookupKey = DataSourceContextHolder.getKey();
    DataSource dataSource = Optional.ofNullable(lookupKey)
    .map(dataSources::get)
    .orElse(defaultDataSource);
    if (dataSource == null) {
        throw new IllegalStateException("Cannot determine DataSource for lookup key [" + lookupKey + "]");
    }
    return dataSource;
}
```
<a name="m7KBs"></a>
#### 4.2.4 管理数据源状态
在项目启动的时候，加载数据表中的所有数据源，并执行初始化。初始化操作主要是使用SpringBoot提供的`DataSourceBuilder`类，根据数据源表的定义创建DataSource。在项目运行过程中，可以使用定时任务对数据源进行保活，为了提升性能再添加一层缓存。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240419786-e313dc6b-e779-4fa5-a22e-3023ea1c4f28.png#averageHue=%23fbfafa&clientId=u777f422b-bb5c-4&from=paste&id=u7db50b33&originHeight=713&originWidth=802&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uea9c441f-187e-44c3-91f3-eee459a308d&title=)<br />`AbstractRoutingDataSource`只支持单库事务，切换数据源是在开启事务之前执行。 Spring使用 `DataSourceTransactionManager`进行事务管理。开启事务，会将数据源缓存到`DataSourceTransactionObject`对象中，后续的commit和 rollback事务操作实际上是使用的同一个数据源。<br />如何解决切库事务问题？借助Spring的声明式事务处理，可以在多次切库操作时强制开启新的事务：
```java
@SwitchDataSource    
@Transactional(rollbackFor = Exception.class, propagation = Propagation.REQUIRES_NEW)
```
这样的话，执行切库操作的时候强制启动新事务，便可实现多次切库而且事务能够生效。但是这种事务方式，存在数据一致性问题：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240443588-0a77b873-f175-46b9-93fc-fb2b134135cb.png#averageHue=%23f8f8f8&clientId=u777f422b-bb5c-4&from=paste&id=u892c9e8d&originHeight=692&originWidth=683&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9bb6e988-201a-4f77-8803-2dcb996da63&title=)<br />假若ServiceB正常执行提交事务，接着返回ServiceA执行并且发生异常。因为两次处理是不同的事务，ServiceA这个事务执行回滚，而ServiceA事务已经提交。这样的话，数据就不一致了。接下来，主要讨论如何解决多库的事务问题。
<a name="b03EW"></a>
## 4、多库事务处理
<a name="efO9q"></a>
### 4.1 关于事务的理解
首先有必要理解事务的本质。

1. 提到Spring事务，就离不开事务的四大特性和隔离级别、七大传播特性。

事务特性和离级别是属于数据库范畴。Spring事务的七大传播特性是什么呢？它是Spring在当前线程内，处理多个事务操作时的事务应用策略，数据库事务本身并不存在传播特性。

2. Spring事务的定义包括：begin、commit、rollback、close、suspend、resume等动作。
- begin(事务开始)： 可以认为存在于数据库的命令中，比如Mysql的`start transaction`命令，但是在JDBC编程方式中不存在。
- close(事务关闭)： Spring事务的close()方法，是把Connection对象归还给数据库连接池，与事务无关。
- suspend(事务挂起)： Spring中事务挂起的语义是：需要新事务时，将现有的Connection保存起来(还有尚未提交的事务)，然后创建新的Connection2，Connection2提交、回滚、关闭完毕后，再把Connection1取出来继续执行。
- resume(事务恢复)： 嵌套事务执行完毕，返回上层事务重新绑定连接对象到事务管理器的过程。

实际上，只有commit、rollback、close是在JDBC真实存在的，而其他动作都是应用的语意，而非JDBC事务的真实命令。因此，事务真实存在的方法是：`setAutoCommit()`、`commit()`、`rollback()`。<br />close()语义为：

- 关闭一个数据库连接，这已经不再是事务的方法了。

使用DataSource并不会执行物理关闭，只是归还给连接池。
<a name="VjMV1"></a>
### 4.2 自定义管理事务
为了保证在多个数据源中事务的一致性，可以手动管理`Connetion`的事务提交和回滚。考虑到不同ORM框架的事务管理实现差异，要求实现自定义事务管理不影响框架层的事务。<br />这可以通过使用装饰器设计模式，对`Connection`进行包装重写commit和rolllback屏蔽其默认行为，这样就不会影响到原生`Connection`和ORM框架的默认事务行为。其整体思路如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240443640-a2329c1b-b2d7-4afb-b885-c9a324c88583.png#averageHue=%23fafafa&clientId=u777f422b-bb5c-4&from=paste&id=u49a152e2&originHeight=774&originWidth=618&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc70ce056-998b-4acc-9e70-524dbacf2e0&title=)<br />这里并没有使用前面提到的`@SwitchDataSource`，这是因为在`TransactionAop`中已经执行了lookupKey的切换。
<a name="RN5Lz"></a>
#### 4.2.1 定义多事务注解
```java
@Target({ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface MultiTransaction {
    String transactionManager() default "multiTransactionManager";
    // 默认数据隔离级别，随数据库本身默认值
    IsolationLevel isolationLevel() default IsolationLevel.DEFAULT;
    // 默认为主库数据源
    String datasourceId() default "default";
    // 只读事务，若有更新操作会抛出异常
    boolean readOnly() default false;
```
业务方法只需使用该注解即可开启事务，`datasourceId`指定事务用到的数据源，不指定默认为主库。
<a name="fi8a7"></a>
#### 4.2.2 包装`Connection`
自定义事务使用包装过的`Connection`，屏蔽其中的commit&rollback方法。这样就可以在主事务里进行统一的事务提交和回滚操作。
```java
public class ConnectionProxy implements Connection {

    private final Connection connection;

    public ConnectionProxy(Connection connection) {
        this.connection = connection;
    }

    @Override
    public void commit() throws SQLException {
        // connection.commit();
    }

    public void realCommit() throws SQLException {
        connection.commit();
    }

    @Override
    public void close() throws SQLException {
        //connection.close();
    }

    public void realClose() throws SQLException {
        if (!connection.getAutoCommit()) {
            connection.setAutoCommit(true);
        }
        connection.close();
    }

    @Override
    public void rollback() throws SQLException {
        if(!connection.isClosed())
            connection.rollback();
    }
    ...
}
```
这里commit&close方法不执行操作，`rollback`执行的前提是连接执行close才生效。这样不管是使用哪个ORM框架，其自身事务管理都将失效。事务的控制就交由`MultiTransaction`控制了。
<a name="NKIgf"></a>
#### 4.2.3 事务上下文管理
```java
public class TransactionHolder {
    // 是否开启了一个MultiTransaction
    private boolean isOpen;
    // 是否只读事务
    private boolean readOnly;
    // 事务隔离级别
    private IsolationLevel isolationLevel;
    // 维护当前线程事务ID和连接关系
    private ConcurrentHashMap<String, ConnectionProxy> connectionMap;
    // 事务执行栈
    private Stack<String> executeStack;
    // 数据源切换栈
    private Stack<String> datasourceKeyStack;
    // 主事务ID
    private String mainTransactionId;
    // 执行次数
    private AtomicInteger transCount;

    // 事务和数据源key关系
    private ConcurrentHashMap<String, String> executeIdDatasourceKeyMap;

}
```
每开启一个事物，生成一个事务ID并绑定一个`ConnectionProxy`。事务嵌套调用，保存事务ID和`lookupKey`至栈中，当内层事务执行完毕执行pop。这样的话，外层事务只需在栈中执行peek即可获取事务ID和`lookupKey`。
<a name="n2zyR"></a>
#### 4.2.4 数据源兼容处理
为了不影响原生事务的使用，需要重写`getConnection`方法。当前线程没有启动自定义事务，则直接从数据源中返回连接。
```java
@Override
public Connection getConnection() throws SQLException {
    TransactionHolder transactionHolder = MultiTransactionManager.TRANSACTION_HOLDER_THREAD_LOCAL.get();
    if (Objects.isNull(transactionHolder)) {
        return determineTargetDataSource().getConnection();
    }
    ConnectionProxy ConnectionProxy = transactionHolder.getConnectionMap()
        .get(transactionHolder.getExecuteStack().peek());
    if (ConnectionProxy == null) {
        // 没开跨库事务，直接返回
        return determineTargetDataSource().getConnection();
    } else {
        transactionHolder.addCount();
        // 开了跨库事务，从当前线程中拿包装过的Connection
        return ConnectionProxy;
    }
}
```
<a name="z6KEd"></a>
#### 4.2.5 切面处理
切面处理的核心逻辑是：维护一个嵌套事务栈，当业务方法执行结束，或者发生异常时，判断当前栈顶事务ID是否为主事务ID。如果是的话这时候已经到了最外层事务，这时才执行提交和回滚。详细流程如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687240443598-8e24bd51-3d2e-42c2-890d-40c74d91d745.png#averageHue=%23fafafa&clientId=u777f422b-bb5c-4&from=paste&id=u5606ac23&originHeight=764&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u336344cb-0be4-40f7-abc5-54218fc45c3&title=)
```java
package com.github.mtxn.transaction.aop;

import com.github.mtxn.application.Application;
import com.github.mtxn.transaction.MultiTransactionManager;
import com.github.mtxn.transaction.annotation.MultiTransaction;
import com.github.mtxn.transaction.context.DataSourceContextHolder;
import com.github.mtxn.transaction.support.IsolationLevel;
import com.github.mtxn.transaction.support.TransactionHolder;
import com.github.mtxn.utils.ExceptionUtils;
import lombok.extern.slf4j.Slf4j;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.aspectj.lang.reflect.MethodSignature;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;

import java.lang.reflect.Method;


@Aspect
@Component
@Slf4j
@Order(99999)
public class MultiTransactionAop {

    @Pointcut("@annotation(com.github.mtxn.transaction.annotation.MultiTransaction)")
    public void pointcut() {
        if (log.isDebugEnabled()) {
            log.debug("start in transaction pointcut...");
        }
    }


    @Around("pointcut()")
    public Object aroundTransaction(ProceedingJoinPoint point) throws Throwable {
        MethodSignature signature = (MethodSignature) point.getSignature();
        // 从切面中获取当前方法
        Method method = signature.getMethod();
        MultiTransaction multiTransaction = method.getAnnotation(MultiTransaction.class);
        if (multiTransaction == null) {
            return point.proceed();
        }
        IsolationLevel isolationLevel = multiTransaction.isolationLevel();
        boolean readOnly = multiTransaction.readOnly();
        String prevKey = DataSourceContextHolder.getKey();
        MultiTransactionManager multiTransactionManager = Application.resolve(multiTransaction.transactionManager());
        // 切数据源，如果失败使用默认库
        if (multiTransactionManager.switchDataSource(point, signature, multiTransaction)) return point.proceed();
        // 开启事务栈
        TransactionHolder transactionHolder = multiTransactionManager.startTransaction(prevKey, isolationLevel, readOnly, multiTransactionManager);
        Object proceed;

        try {
            proceed = point.proceed();
            multiTransactionManager.commit();
        } catch (Throwable ex) {
            log.error("execute method:{}#{},err:", method.getDeclaringClass(), method.getName(), ex);
            multiTransactionManager.rollback();
            throw ExceptionUtils.api(ex, "系统异常：%s", ex.getMessage());
        } finally {
            // 当前事务结束出栈
            String transId = multiTransactionManager.getTrans().getExecuteStack().pop();
            transactionHolder.getDatasourceKeyStack().pop();
            // 恢复上一层事务
            DataSourceContextHolder.setKey(transactionHolder.getDatasourceKeyStack().peek());
            // 最后回到主事务，关闭此次事务
            multiTransactionManager.close(transId);
        }
        return proceed;

    }

}
```
<a name="lWuQp"></a>
## 5、总结
本文主要介绍了多数据源管理的解决方案(应用层事务，而非XA二段提交保证)，以及对多个库同时操作的事务管理。<br />需要注意的是，这种方式只适用于单体架构的应用。因为多个库的事务参与者都是运行在同一个JVM进行。如果是在微服务架构的应用中，则需要使用分布式事务管理(譬如：Seata)。
