Java PageHelper <br />项目一直使用的是PageHelper实现分页功能，项目前期数据量较少一直没有什么问题。随着业务扩增，数据库扩增PageHelper出现了明显的性能问题。<br />几十万甚至上百万的单表数据查询性能缓慢，需要几秒乃至十几秒的查询时间。研究了一下PageHelper源码，查找PageHelper分页的实现方式。<br />一段较为简单的查询，跟随debug开始源码探寻之旅。
```java
public ResultContent select(Integer id) {
    Page<Test> blogPage = PageHelper.startPage(1,3).doSelectPage( () -> testDao.select(id));
    List<Test> test = (List<Test>)blogPage.getResult();
    return new ResultContent(0, "success", test);
}
```
主要保存由前端传入的pageNum(页数)、pageSize(每页显示数量)和count(是否进行count(0)查询)信息。<br />这里是简单的创建page并保存当前线程的变量副本。
```java
public static <E> Page<E> startPage(int pageNum, int pageSize) {
    return startPage(pageNum, pageSize, DEFAULT_COUNT);
}

public static <E> Page<E> startPage(int pageNum, int pageSize, boolean count) {
    return startPage(pageNum, pageSize, count, (Boolean)null, (Boolean)null);
}

public static <E> Page<E> startPage(int pageNum, int pageSize, String orderBy) {
    Page<E> page = startPage(pageNum, pageSize);
    page.setOrderBy(orderBy);
    return page;
}

public static <E> Page<E> startPage(int pageNum, int pageSize, boolean count, Boolean reasonable, Boolean pageSizeZero) {
    Page<E> page = new Page(pageNum, pageSize, count);
    page.setReasonable(reasonable);
    page.setPageSizeZero(pageSizeZero);
    Page<E> oldPage = getLocalPage();
    if(oldPage != null && oldPage.isOrderByOnly()) {
        page.setOrderBy(oldPage.getOrderBy());
    }

    setLocalPage(page);
    return page;
}
```
开始执行真正的select语句
```java
public <E> Page<E> doSelectPage(ISelect select) {
    select.doSelect();
    return this;
}
```
进入MapperProxy类执行invoke方法获取到方法名称及参数值
```java
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
    if (Object.class.equals(method.getDeclaringClass())) {
        try {
            return method.invoke(this, args);
        } catch (Throwable t) {
            throw ExceptionUtil.unwrapThrowable(t);
        }
    }
    final MapperMethod mapperMethod = cachedMapperMethod(method);
    return mapperMethod.execute(sqlSession, args);
}
```
接着是MapperMethod方法执行execute语句，判断是增、删、改、查。判断返回值是多个，进入executeForMany方法
```java
public Object execute(SqlSession sqlSession, Object[] args) {
    Object result;
    if (SqlCommandType.INSERT == command.getType()) {
        Object param = method.convertArgsToSqlCommandParam(args);
        result = rowCountResult(sqlSession.insert(command.getName(), param));
    } else if (SqlCommandType.UPDATE == command.getType()) {
        Object param = method.convertArgsToSqlCommandParam(args);
        result = rowCountResult(sqlSession.update(command.getName(), param));
    } else if (SqlCommandType.DELETE == command.getType()) {
        Object param = method.convertArgsToSqlCommandParam(args);
        result = rowCountResult(sqlSession.delete(command.getName(), param));
    } else if (SqlCommandType.SELECT == command.getType()) {
        if (method.returnsVoid() && method.hasResultHandler()) {
            executeWithResultHandler(sqlSession, args);
            result = null;
        } else if (method.returnsMany()) {
            result = executeForMany(sqlSession, args);
        } else if (method.returnsMap()) {
            result = executeForMap(sqlSession, args);
        } else {
            Object param = method.convertArgsToSqlCommandParam(args);
            result = sqlSession.selectOne(command.getName(), param);
        }
    } else if (SqlCommandType.FLUSH == command.getType()) {
        result = sqlSession.flushStatements();
    } else {
        throw new BindingException("Unknown execution method for: " + command.getName());
    }
    if (result == null && method.getReturnType().isPrimitive() && !method.returnsVoid()) {
        throw new BindingException("Mapper method '" + command.getName() 
                                   + " attempted to return null from a method with a primitive return type (" + method.getReturnType() + ").");
    }
    return result;
}
```
这个方法开始调用SqlSessionTemplate、DefaultSqlSession等类获取到Mapper.xml文件的SQL语句
```java
private <E> Object executeForMany(SqlSession sqlSession, Object[] args) {
    List<E> result;
    Object param = method.convertArgsToSqlCommandParam(args);
    if (method.hasRowBounds()) {
        RowBounds rowBounds = method.extractRowBounds(args);
        result = sqlSession.<E>selectList(command.getName(), param, rowBounds);
    } else {
        result = sqlSession.<E>selectList(command.getName(), param);
    }
    // issue #510 Collections & arrays support
    if (!method.getReturnType().isAssignableFrom(result.getClass())) {
        if (method.getReturnType().isArray()) {
            return convertToArray(result);
        } else {
            return convertToDeclaredCollection(sqlSession.getConfiguration(), result);
        }
    }
    return result;
}
```
开始进入PageHelper的真正实现，Plugin通过实现InvocationHandler进行动态代理获取到相关信息
```java
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
    try {
        Set<Method> methods = signatureMap.get(method.getDeclaringClass());
        if (methods != null && methods.contains(method)) {
            return interceptor.intercept(new Invocation(target, method, args));
        }
        return method.invoke(target, args);
    } catch (Exception e) {
        throw ExceptionUtil.unwrapThrowable(e);
    }
}
```
PageInterceptor 实现Mybatis的Interceptor 接口，进行拦截
```java
public Object intercept(Invocation invocation) throws Throwable {
    try {
        Object[] args = invocation.getArgs();
        MappedStatement ms = (MappedStatement)args[0];
        Object parameter = args[1];
        RowBounds rowBounds = (RowBounds)args[2];
        ResultHandler resultHandler = (ResultHandler)args[3];
        Executor executor = (Executor)invocation.getTarget();
        CacheKey cacheKey;
        BoundSql boundSql;
        if(args.length == 4) {
            boundSql = ms.getBoundSql(parameter);
            cacheKey = executor.createCacheKey(ms, parameter, rowBounds, boundSql);
        } else {
            cacheKey = (CacheKey)args[4];
            boundSql = (BoundSql)args[5];
        }

        this.checkDialectExists();
        List resultList;
        if(!this.dialect.skip(ms, parameter, rowBounds)) {
            if(this.dialect.beforeCount(ms, parameter, rowBounds)) {
                Long count = this.count(executor, ms, parameter, rowBounds, resultHandler, boundSql);
                if(!this.dialect.afterCount(count.longValue(), parameter, rowBounds)) {
                    Object var12 = this.dialect.afterPage(new ArrayList(), parameter, rowBounds);
                    return var12;
                }
            }

            resultList = ExecutorUtil.pageQuery(this.dialect, executor, ms, parameter, rowBounds, resultHandler, boundSql, cacheKey);
        } else {
            resultList = executor.query(ms, parameter, rowBounds, resultHandler, cacheKey, boundSql);
        }

        Object var16 = this.dialect.afterPage(resultList, parameter, rowBounds);
        return var16;
    } finally {
        this.dialect.afterAll();
    }
}
```
转到ExecutorUtil抽象类的`pageQuery`方法
```java
public static <E> List<E> pageQuery(Dialect dialect, Executor executor, MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql, CacheKey cacheKey) throws SQLException {
    if(!dialect.beforePage(ms, parameter, rowBounds)) {
        return executor.query(ms, parameter, RowBounds.DEFAULT, resultHandler, cacheKey, boundSql);
    } else {
        parameter = dialect.processParameterObject(ms, parameter, boundSql, cacheKey);
        String pageSql = dialect.getPageSql(ms, boundSql, parameter, rowBounds, cacheKey);
        BoundSql pageBoundSql = new BoundSql(ms.getConfiguration(), pageSql, boundSql.getParameterMappings(), parameter);
        Map<String, Object> additionalParameters = getAdditionalParameter(boundSql);
        Iterator var12 = additionalParameters.keySet().iterator();

        while(var12.hasNext()) {
            String key = (String)var12.next();
            pageBoundSql.setAdditionalParameter(key, additionalParameters.get(key));
        }

        return executor.query(ms, parameter, RowBounds.DEFAULT, resultHandler, cacheKey, pageBoundSql);
    }
}
```
在抽象类AbstractHelperDialect的`getPageSql`获取到对应的Page对象
```java
public String getPageSql(MappedStatement ms, BoundSql boundSql, Object parameterObject, RowBounds rowBounds, CacheKey pageKey) {
    String sql = boundSql.getSql();
    Page page = this.getLocalPage();
    String orderBy = page.getOrderBy();
    if(StringUtil.isNotEmpty(orderBy)) {
        pageKey.update(orderBy);
        sql = OrderByParser.converToOrderBySql(sql, orderBy);
    }
    return page.isOrderByOnly()?sql:this.getPageSql(sql, page, pageKey);
}
```
进入到MySqlDialect类的`getPageSql`方法进行SQL封装，根据page对象信息增加Limit。分页的信息就是这么拼装起来的
```java
public String getPageSql(String sql, Page page, CacheKey pageKey) {
    StringBuilder sqlBuilder = new StringBuilder(sql.length() + 14);
    sqlBuilder.append(sql);
    if(page.getStartRow() == 0) {
        sqlBuilder.append(" LIMIT ? ");
    } else {
        sqlBuilder.append(" LIMIT ?, ? ");
    }
    return sqlBuilder.toString();
}
```
将最后拼装好的SQL返回给DefaultSqlSession执行查询并返回
```java
public <E> List<E> selectList(String statement, Object parameter, RowBounds rowBounds) {
    try {
        MappedStatement ms = configuration.getMappedStatement(statement);
        return executor.query(ms, wrapCollection(parameter), rowBounds, Executor.NO_RESULT_HANDLER);
    } catch (Exception e) {
        throw ExceptionFactory.wrapException("Error querying database.  Cause: " + e, e);
    } finally {
        ErrorContext.instance().reset();
    }
}
```
至此整个查询过程完成，原来PageHelper的分页功能是通过Limit拼接SQL实现的。查询效率低的问题也找出来了，那么应该如何解决。<br />首先分析SQL语句，limit在数据量少或者页数比较靠前的时候查询效率是比较高的。(单表数据量百万进行测试)
```sql
select * from user where age = 10 limit 1,10;
```
结果显示0.43s<br />当where条件后的结果集较大并且页数达到一个量级整个SQL的查询效率就十分低下(哪怕where的条件加上了索引也不行)。
```sql
select * from user where age = 10 limit 100000,10;
```
结果显示4.73s<br />那有什么解决方案呢？MySQL就不能单表数据量超百万乃至千万嘛？<br />答案是NO，显然是可以的。
```sql
SELECT a.* FROM USER a
INNER JOIN 
    (SELECT id FROM USER WHERE age = 10 LIMIT 100000,10) b 
ON a.id = b.id;
```
结果0.53s<br />完美解决了查询效率问题！其中需要对where条件增加索引，id因为是主键自带索引。select返回减少回表可以提升查询性能，所以采用查询主键字段后进行关联大幅度提升了查询效率。<br />PageHelper想要优化需要在拦截器的拼接SQL部分进行重构。
