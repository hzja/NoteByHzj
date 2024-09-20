Java Mybatis<br />MyBatis是一款优秀的持久层框架，它支持自定义SQL、存储过程以及高级映射。MyBatis免除了几乎所有的JDBC代码以及设置参数和获取结果集的工作。MyBatis可以通过简单的XML或注解来配置和映射原始类型、接口和Java POJO（Plain Old Java Objects，普通老式Java对象）为数据库中的记录。
<a name="xtKaS"></a>
## 测试环境
<a name="ljNzy"></a>
### POM文件
```xml
<dependencies>
  <dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
    <version>8.0.17</version>
    <scope>runtime</scope>
  </dependency>
  <dependency>
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis</artifactId>
    <version>3.5.1</version>
  </dependency>
  <dependency>
    <groupId>junit</groupId>
    <artifactId>junit</artifactId>
    <version>4.12</version>
  </dependency>
  <dependency>
    <groupId>commons-logging</groupId>
    <artifactId>commons-logging</artifactId>
    <version>1.2</version>
  </dependency>
  <dependency>
    <groupId>log4j</groupId>
    <artifactId>log4j</artifactId>
    <version>1.2.17</version>
  </dependency>
</dependencies>
```
<a name="TtBdL"></a>
### 测试代码
```java
public class ExecutorTest {
    private SqlSessionFactory factory;
    private Configuration configuration;
    private JdbcTransaction jdbcTransaction;
    private Connection connection;
    private MappedStatement ms;

    @Before
    public void init() throws SQLException {
        // 获取构建器
        SqlSessionFactoryBuilder factoryBuilder = new SqlSessionFactoryBuilder();
        // 解析XML 并构造会话工厂
        factory = factoryBuilder.build(ExecutorTest.class.getResourceAsStream("/mybatis-config.xml"));
        configuration = factory.getConfiguration();
        // 创建事务
        jdbcTransaction = new JdbcTransaction(factory.openSession().getConnection());
    }

    @Test
    public void sessionTest() {
        // 开启会话
        SqlSession sqlSession = factory.openSession(true);
        // 查询数据
        List<Object> list = sqlSession.selectList("com.coderead.mybatis.UserMapper.selectByid", 10);
        // 输出查询结果
        System.out.println(list.get(0));
    }
}
```
<a name="TmI8s"></a>
## 执行过程源码跟踪
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679141729689-f9ae8273-9ced-419a-887f-8dbe4a1f9f0c.png#averageHue=%23fdfdfd&clientId=u7e97a910-a73d-4&from=paste&id=u7fa944d2&originHeight=477&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u142130d5-e3e1-4d86-8d48-bcdce7fe2f0&title=)<br />经过跟踪分析调用过程后，可以把MyBatis的调用分解为三个过程
<a name="MpZOd"></a>
### 开启会话：创建事务、创建Executor
```java
private SqlSession openSessionFromDataSource(ExecutorType execType, TransactionIsolationLevel level, boolean autoCommit) {
    Transaction tx = null;
    try {
        // 获取环境变量
        final Environment environment = configuration.getEnvironment();
        final TransactionFactory transactionFactory = getTransactionFactoryFromEnvironment(environment);
        // 创建事务
        tx = transactionFactory.newTransaction(environment.getDataSource(), level, autoCommit);
        // 创建Executor
        final Executor executor = configuration.newExecutor(tx, execType);
        return new DefaultSqlSession(configuration, executor, autoCommit);
    } catch (Exception e) {
        closeTransaction(tx); // may have fetched a connection so lets call close()
        throw ExceptionFactory.wrapException("Error opening session.  Cause: " + e, e);
    } finally {
        ErrorContext.instance().reset();
    }
}
```
<a name="arT6m"></a>
### Executor：管理缓存、创建`StatementHandler`
<a name="U1Oky"></a>
#### `CachingExecutor`管理二级缓存
```java
public <E> List<E> query(MappedStatement ms, Object parameterObject, RowBounds rowBounds, ResultHandler resultHandler, CacheKey key, BoundSql boundSql)
    throws SQLException {
    Cache cache = ms.getCache();
    if (cache != null) {
        // 当ms.isFlushCacheRequired = true清空二级缓存
        flushCacheIfRequired(ms);
        if (ms.isUseCache() && resultHandler == null) {
            ensureNoOutParams(ms, boundSql);
            @SuppressWarnings("unchecked")
                List<E> list = (List<E>) tcm.getObject(cache, key);
            if (list == null) {
                // 未命中二级缓存，调用BaseExecutor.query()查询数据
                list = delegate.query(ms, parameterObject, rowBounds, resultHandler, key, boundSql);
                // 二级缓存记录新数据
                tcm.putObject(cache, key, list); // issue #578 and #116
            }
            return list;
        }
    }
    // 未命中二级缓存，调用BaseExecutor.query()查询数据
    return delegate.query(ms, parameterObject, rowBounds, resultHandler, key, boundSql);
}
```
<a name="DpAj8"></a>
#### `BaseExecutor`管理一级缓存
```java
public <E> List<E> query(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, CacheKey key, BoundSql boundSql) throws SQLException {
    ErrorContext.instance().resource(ms.getResource()).activity("executing a query").object(ms.getId());
    if (closed) {
        throw new ExecutorException("Executor was closed.");
    }
    // queryStack记录子嵌套查询深度
    if (queryStack == 0 && ms.isFlushCacheRequired()) {
        clearLocalCache();
    }
    List<E> list;
    try {
        queryStack++;
        // 从一级缓存查询数据
        list = resultHandler == null ? (List<E>) localCache.getObject(key) : null;
        if (list != null) {
            handleLocallyCachedOutputParameters(ms, key, parameter, boundSql);
        } else {
            // 未命中一级缓存，调用SimpleExecutor.doQuery()查询数据
            list = queryFromDatabase(ms, parameter, rowBounds, resultHandler, key, boundSql);
        }
    } finally {
        queryStack--;
    }
    if (queryStack == 0) {
        for (DeferredLoad deferredLoad : deferredLoads) {
            deferredLoad.load();
        }
        // issue #601
        deferredLoads.clear();
        // 当一级缓存作用域为STATEMENT时，清空一级缓存。可选值为SESSION
        if (configuration.getLocalCacheScope() == LocalCacheScope.STATEMENT) {
            // issue #482
            clearLocalCache();
        }
    }
    return list;
}
```
<a name="UvGep"></a>
#### `SimpleExecutor`创建`StatementHandler`和与之类型匹配的`Statement`
```java
public <E> List<E> doQuery(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) throws SQLException {
    Statement stmt = null;
    try {
        Configuration configuration = ms.getConfiguration();
        // 创建StatementHandler
        StatementHandler handler = configuration.newStatementHandler(wrapper, ms, parameter, rowBounds, resultHandler, boundSql);
        // 创建Statement
        stmt = prepareStatement(handler, ms.getStatementLog());
        // 调用preparedStatement.execute()查询数据
        return handler.query(stmt, resultHandler);
    } finally {
        closeStatement(stmt);
    }
}
```
<a name="dx9js"></a>
### `Statement`：获取并返回查询结果
```java
public <E> List<E> query(Statement statement, ResultHandler resultHandler) throws SQLException {
    PreparedStatement ps = (PreparedStatement) statement;
    // 查询数据
    ps.execute();
    // 返回数据结果
    return resultSetHandler.handleResultSets(ps);
}
```
<a name="YJJL7"></a>
## MyBatis设计模式
<a name="ADmsa"></a>
### `Executor`
![Executor类图](https://cdn.nlark.com/yuque/0/2023/png/396745/1679141729676-1a95516e-f419-43f8-935b-a54da81397fb.png#averageHue=%23f4f2fd&clientId=u7e97a910-a73d-4&from=paste&id=uf4c17be1&originHeight=772&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u9eb68c43-5792-4b8a-81ea-ed361642cbe&title=Executor%E7%B1%BB%E5%9B%BE "Executor类图")
<a name="xBH7e"></a>
#### 源码展示
```java
public Executor newExecutor(Transaction transaction, ExecutorType executorType) {
    executorType = executorType == null ? defaultExecutorType : executorType;
    executorType = executorType == null ? ExecutorType.SIMPLE : executorType;
    Executor executor;
    if (ExecutorType.BATCH == executorType) {
        executor = new BatchExecutor(this, transaction);
    } else if (ExecutorType.REUSE == executorType) {
        executor = new ReuseExecutor(this, transaction);
    } else {
        executor = new SimpleExecutor(this, transaction);
    }
    if (cacheEnabled) {
        executor = new CachingExecutor(executor);
    }
    executor = (Executor) interceptorChain.pluginAll(executor);
    return executor;
}
```
<a name="bN49B"></a>
### `StatementHandler`
![StatementHandler类图](https://cdn.nlark.com/yuque/0/2023/png/396745/1679141729634-d4196fe1-ebba-4eb8-92f8-1b87717d0780.png#averageHue=%23f3f1fc&clientId=u7e97a910-a73d-4&from=paste&id=u9e169fc2&originHeight=604&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u591a7f1a-8c19-44ad-9830-28bb4a01f44&title=StatementHandler%E7%B1%BB%E5%9B%BE "StatementHandler类图")
<a name="J015k"></a>
#### 源码展示
```java
public RoutingStatementHandler(Executor executor, MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) {
    switch (ms.getStatementType()) {
        case STATEMENT:
            delegate = new SimpleStatementHandler(executor, ms, parameter, rowBounds, resultHandler, boundSql);
            break;
        case PREPARED:
            delegate = new PreparedStatementHandler(executor, ms, parameter, rowBounds, resultHandler, boundSql);
            break;
        case CALLABLE:
            delegate = new CallableStatementHandler(executor, ms, parameter, rowBounds, resultHandler, boundSql);
            break;
        default:
            throw new ExecutorException("Unknown statement type: " + ms.getStatementType());
    }
}
```
<a name="uvznq"></a>
### `Statement`
![Statement类图](https://cdn.nlark.com/yuque/0/2023/png/396745/1679141729867-0cb41d86-8fd4-448f-b0c7-814fb6cc8a72.png#averageHue=%23ecebe9&clientId=u7e97a910-a73d-4&from=paste&id=u22602744&originHeight=969&originWidth=933&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uabdca1db-d0b7-476e-877c-4b08f30e4b2&title=Statement%E7%B1%BB%E5%9B%BE "Statement类图")
<a name="BdUgn"></a>
#### 源码展示
```java
protected Statement instantiateStatement(Connection connection) throws SQLException {
    String sql = boundSql.getSql();
    if (mappedStatement.getKeyGenerator() instanceof Jdbc3KeyGenerator) {
        String[] keyColumnNames = mappedStatement.getKeyColumns();
        if (keyColumnNames == null) {
            return connection.prepareStatement(sql, PreparedStatement.RETURN_GENERATED_KEYS);
        } else {
            return connection.prepareStatement(sql, keyColumnNames);
        }
    } else if (mappedStatement.getResultSetType() == ResultSetType.DEFAULT) {
        return connection.prepareStatement(sql);
    } else {
        return connection.prepareStatement(sql, mappedStatement.getResultSetType().getValue(), ResultSet.CONCUR_READ_ONLY);
    }
}
```
<a name="MDADW"></a>
## 后记
此次跟踪MyBatis的一次查询过程，均采用MyBatis默认配置，例如文中提到的`ExecutorType`默认为**SIMPLE**，可选值为[SIMPLE, REUSE, BATCH]；`StatementType`默认为**PREPARED**，可选值为[STATEMENT, PREPARED, CALLABLE]，`LocalCacheScope`默认为**SESSION**，可选值为[SESSION, STATEMENT]。 Executor和`StatementHandler`的实现采用**装饰器模式**。 为了聚焦于**查询过程**，也选择性忽略了一部分MyBatis的重要功能，如**日志**模块。
