Mybatis
<a name="12xjX"></a>
## 数据读写的本质
不管是哪种ORM框架，数据读写其本质都是对JDBC的封装，其目的主要都是简化JDBC的开发流程，进而让开发人员更关注业务。下面是JDBC的核心流程：

1. 注册 JDBC 驱动（Class.forName("XXX");）
2. 打开连接（DriverManager.getConnection("url","name","password")）
3. 根据连接，创建 Statement（conn.prepareStatement(sql)）
4. 设置参数（stmt.setString(1, "wyf");）
5. 执行查询（stmt.executeQuery();）
6. 处理结果，结果集映射（resultSet.next()）
7. **关闭资源（finally）**

Mybatis也是在对JDBC进行封装，它将**注册驱动**和**打开连接**交给了数据库连接池来负责，Mybatis支持第三方数据库连接池，也可以使用自带的数据库连接池；**创建 Statement** 和**执行查询**交给了`StatementHandler`来负责；**设置参数**交给`ParameterHandler`来负责；最后两步交给了`ResultSetHandler`来负责。
<a name="ykZXl"></a>
## Executor内部运作过程
> 测试方法`org.apache.ibatis.binding.BindingTest#shouldFindThreeSpecificPosts`

下面是一个简单的数据读写过程，在宏观上先来了解一下每一个组件在整个数据读写上的作用：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1615727960930-7ce12589-0b5d-471f-ad62-07f41a074e03.webp#align=left&display=inline&height=610&originHeight=610&originWidth=707&size=0&status=done&style=shadow&width=707)<br />Mybatis的数据读写主要是通Excuter来协调StatementHandler、ParameterHandler和ResultSetHandler三个组件来实现的：

- StatementHandler：它的作用是使用数据库的Statement或PrepareStatement执行操作，启承上启下作用；
- ParameterHandler：对预编译的SQL语句进行参数设置，SQL语句中的的占位符“？”都对应BoundSql.parameterMappings集合中的一个元素，在该对象中记录了对应的参数名称以及该参数的相关属性
- ResultSetHandler：对数据库返回的结果集（ResultSet）进行封装，返回用户指定的实体类型；
<a name="F2sq7"></a>
## StatementHandler
<a name="YSJC7"></a>
### StatementHandler类图
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1615727960930-dcee9141-445b-40a6-b2bf-e4fbc1785fc0.webp#align=left&display=inline&height=400&originHeight=400&originWidth=927&size=0&status=done&style=none&width=927)
<a name="JRedc"></a>
## RoutingStatementHandler
通过`StatementType`来创建`StatementHandler`，使用静态代理模式来完成方法的调用，主要起到路由作用。它是Excutor组件真正实例化的组件。
```java
public class RoutingStatementHandler implements StatementHandler {
    /**
   * 静态代理模式
   */
    private final StatementHandler delegate;
    public RoutingStatementHandler(Executor executor, MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) {
        // 根据{@link org.apache.ibatis.mapping.StatementType} 来创建不同的实现类 （策略模式）
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
    @Override
    public Statement prepare(Connection connection, Integer transactionTimeout) throws SQLException {
        return delegate.prepare(connection, transactionTimeout);
    }
    ...
    }
```
<a name="AEdr9"></a>
### BaseStatementHandler
所有子类的抽象父类，定义了初始化statement的操作顺序，由子类实现具体的实例化不同的statement（模板模式）。
```java
@Override
public Statement prepare(Connection connection, Integer transactionTimeout) throws SQLException {
    ErrorContext.instance().sql(boundSql.getSql());
    Statement statement = null;
    try {
        // 实例化Statement（由子类实现）【模板方法+策略模式】
        statement = instantiateStatement(connection);
        // 设置超时时间
        setStatementTimeout(statement, transactionTimeout);
        // 设置获取数据记录条数
        setFetchSize(statement);
        return statement;
    } catch (SQLException e) {
        closeStatement(statement);
        throw e;
    } catch (Exception e) {
        closeStatement(statement);
        throw new ExecutorException("Error preparing statement.  Cause: " + e, e);
    }
}
protected abstract Statement instantiateStatement(Connection connection) throws SQLException;
```
`instantiateStatement()`就是一个模板方法，由子类实现。
<a name="Biet0"></a>
### SimpleStatementHandler
使用JDBC`Statement`执行模式，不需要做参数处理，源码如下：
```java
@Override
protected Statement instantiateStatement(Connection connection) throws SQLException {
    // 实例化Statement
    if (mappedStatement.getResultSetType() == ResultSetType.DEFAULT) {
        return connection.createStatement();
    } else {
        return connection.createStatement(mappedStatement.getResultSetType().getValue(), ResultSet.CONCUR_READ_ONLY);
    }
}
@Override
public void parameterize(Statement statement) {
    // N/A
    // 使用Statement是直接执行sql 所以没有参数
}
```
<a name="TCfwd"></a>
### PreparedStatementHandler
使用JDBC`PreparedStatement`预编译执行模式。
```java
@Override
protected Statement instantiateStatement(Connection connection) throws SQLException {
    // 实例化PreparedStatement
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
@Override
public void parameterize(Statement statement) throws SQLException {
    // 参数处理
    parameterHandler.setParameters((PreparedStatement) statement);
}
```
<a name="eetEH"></a>
### CallableStatementHandler
使用JDBC`CallableStatement`执行模式，用来调用存储过程。现在很少用。
<a name="4coy9"></a>
## ParameterHandler
主要作用是给`PreparedStatement`设置参数，源码如下：
```java
@Override
public void setParameters(PreparedStatement ps) {
    ErrorContext.instance().activity("setting parameters").object(mappedStatement.getParameterMap().getId());
    // 获取参数映射关系
    List<ParameterMapping> parameterMappings = boundSql.getParameterMappings();
    if (parameterMappings != null) {
        // 循环获取处理参数
        for (int i = 0; i < parameterMappings.size(); i++) {
            // 获取对应索引位的参数
            ParameterMapping parameterMapping = parameterMappings.get(i);
            if (parameterMapping.getMode() != ParameterMode.OUT) {
                Object value;
                // 获取参数名称
                String propertyName = parameterMapping.getProperty();
                // 判断是否是附加参数
                if (boundSql.hasAdditionalParameter(propertyName)) { // issue #448 ask first for additional params
                    value = boundSql.getAdditionalParameter(propertyName);
                }
                // 判断是否是没有参数
                else if (parameterObject == null) {
                    value = null;
                }
                // 判断参数是否有相应的 TypeHandler
                else if (typeHandlerRegistry.hasTypeHandler(parameterObject.getClass())) {
                    value = parameterObject;
                } else {
                    // 以上都不是，通过反射获取value值
                    MetaObject metaObject = configuration.newMetaObject(parameterObject);
                    value = metaObject.getValue(propertyName);
                }
                // 获取参数的类型处理器
                TypeHandler typeHandler = parameterMapping.getTypeHandler();
                JdbcType jdbcType = parameterMapping.getJdbcType();
                if (value == null && jdbcType == null) {
                    jdbcType = configuration.getJdbcTypeForNull();
                }
                try {
                    // 根据TypeHandler设置参数
                    typeHandler.setParameter(ps, i + 1, value, jdbcType);
                } catch (TypeException | SQLException e) {
                    throw new TypeException("Could not set parameters for mapping: " + parameterMapping + ". Cause: " + e, e);
                }
            }
        }
    }
}
```

1. 获取参数映射关系
2. 获取参数名称
3. 根据参数名称获取参数值
4. 获取参数的类型处理器
5. 根据TypeHandler设置参数值

通过上面的流程可以发现，真正设置参数是由TypeHandler来实现的。
<a name="72JWY"></a>
### TypeHandler
Mybatis基本上提供了所需要用到的所有TypeHandler，当然也可以自己实现。TypeHandler的主要作用是：

1. 设置PreparedStatement参数值
2. 获取查询结果值
```java
public interface TypeHandler<T> {
    /**
   * 给{@link PreparedStatement}设置参数值
   *
   * @param ps        {@link PreparedStatement}
   * @param i         参数的索引位
   * @param parameter 参数值
   * @param jdbcType  参数类型
   * @throws SQLException
   */
    void setParameter(PreparedStatement ps, int i, T parameter, JdbcType jdbcType) throws SQLException;
    /**
   * 根据列名获取结果值
   *
   * @param columnName Colunm name, when configuration <code>useColumnLabel</code> is <code>false</code>
   */
    T getResult(ResultSet rs, String columnName) throws SQLException;
    /**
   * 根据索引位获取结果值
   */
    T getResult(ResultSet rs, int columnIndex) throws SQLException;
    /**
   * 获取存储过程结果值
   */
    T getResult(CallableStatement cs, int columnIndex) throws SQLException;
}
```
> TypeHandler的本质就是对JDBC中`stmt.setString(1, "wyf");`和`resultSet.getString("name")`的封装，JDBC完整代码可以查看JDBC 面试要点。

<a name="ResultSetHandler"></a>
## ResultSetHandler
`ResultSetHandler`主要作用是：对数据库返回的结果集（ResultSet）进行封装，通过通过`ResultMap`配置和反射完成自动映射，返回用户指定的实体类型；核心思路如下：

1. 根据`RowBounds`做分页处理
2. 根据`ResultMap`配置的返回值类型和`constructor`配置信息实例化目标类
3. 根据`ResultMap`配置的映射关系，获取到`TypeHandler`，进而从`ResultSet`中获取值
4. 根据`ResultMap`配置的映射关系，获取到目标类的属性名称，然后通过反射给目标类赋值

源码太多了，这里就不发了，有兴趣就在下面的源码上看注释吧，下面的流程图会画出方法的调用栈。
> Mybatis自带的`RowBounds`分页是逻辑分页，数据量大了有可能会内存溢出，所以不建议使用Mybatis默认分页。

<a name="GmDgf"></a>
## 数据读取流程图
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1615727960941-77390a4d-0391-4182-8da5-2aa0c01ba3e5.webp#align=left&display=inline&height=1946&originHeight=1946&originWidth=1080&size=0&status=done&style=shadow&width=1080)![](https://cdn.nlark.com/yuque/0/2021/png/396745/1615727961019-9a48e822-0152-4eb8-a799-e711311721e0.png#align=left&display=inline&height=577&originHeight=577&originWidth=1080&size=0&status=done&style=none&width=1080)
<a name="rE2OZ"></a>
## 总结
Mybatis的整个数据读取流程其实就是对JDBC的一个标准实现。
