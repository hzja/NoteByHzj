<a name="HVhhf"></a>
# MyBatis 缓存问题
<a name="jq4Ur"></a>
## 引言
很多 Java 后端服务都选择使用 MyBatis 作为它们的 ORM 框架，帮助简化开发。但是基本上大家都不会太关心 MyBatis 的缓存机制，基本都在使用 MyBatis 缓存的默认配置，在不了解 MyBatis 缓存机制的情况下进行开发，可能就会发生一些意料之外的事情。下面帮助大家了解 MyBatis 一级缓存和二级缓存的机制，以及如何避免 MyBatis 缓存中的坑。
<a name="TvzZx"></a>
## 缓存
众所周知数据库的请求中大部分都是查询请求，使用缓存可以大大减少数据库的压力，提高系统的性能，但是如果使用不当可能就会产生数据一致性的问题。
<a name="WkSen"></a>
### 一级缓存
MyBatis 的一级缓存又被叫做本地缓存，一级缓存默认作用在 Session 级别，并且不能被关闭，只能修改一级缓存的作用域。<br />一级缓存的作用域有两种，分别是 SESSION 和 STATEMENT，可以通过修改配置项 `localCacheScope` 来设置，默认的 SESSION 会缓存一个会话中执行的所有查询，用来加速重复的嵌套查询，当在这个会话中执行更新操作时则会清除缓存。如果设置成 STATEMENT 则值作用在执行语句中，当语句执行完成就会清除缓存。<br />**问题**<br />下面看一下两个一级缓存导致的问题：<br />**问题 1**<br />当 `localCacheScope` 被设置为 SESSION 的时候，并且当前服务有多个实例时就可能会导致查询到的数据不一致。
```java
// 会话 1
SqlSession session1 = factory.openSession(true);
// 会话 2
SqlSession session2 = factory.openSession(true);

TemplateInfoMapper templateInfoMapper1 = session1.getMapper(TemplateInfoMapper.class);
TemplateInfoMapper templateInfoMapper2 = session2.getMapper(TemplateInfoMapper.class);

TemplateInfo a1 = templateInfoMapper1.get(1L);
TemplateInfo a2 = templateInfoMapper2.get(1L);
// 数据一致
Assert.assertEquals(a1.getTemplateName(), a2.getTemplateName());

a1.setTemplateName("a1");
templateInfoMapper1.updateByPrimaryKey(a1);

TemplateInfo b1 = templateInfoMapper1.get(1L);
TemplateInfo b2 = templateInfoMapper2.get(1L);
// 数据不一致
Assert.assertEquals(b1.getTemplateName(), b2.getTemplateName());
```
上面这段代码模拟了多实例情况下数据不一致的场景。<br />**问题 2**<br />这个问题也是在被配置为 SESSION 时导致的问题，不过这个问题是在同一个会话当中发生的，下面看一段代码：
```java
SqlSession session = factory.openSession(true);
TemplateInfoMapper templateInfoMapper = session.getMapper(TemplateInfoMapper.class);
TemplateInfo a1 = templateInfoMapper.get(1L);
a1.setTemplateName("a1");
TemplateInfo a2 = templateInfoMapper.get(1L);
a2.setTemplateName("a2");
// 数据一致
Assert.assertEquals(a1.getTemplateName(), a2.getTemplateName());
session.close();
```
上面这段代码分别获得结果集 a1 和 a2，并且分别对 a1 和 a2 结果集中的 templateName 进行了修改，但是最后比较时 a1 和 a2 的 templateName 反而数据是一致的。<br />**原因**<br />问题 1 中开启了两个会话（在分布式环境下，一个服务有多个实例很常见，这里可以把会话看作实例），每个会话都会有自己的一级缓存，也就是两个会话都会缓存 ID 等于 1 的数据到一级缓存当中，但是当 会话 1 去更新了 ID 为 1 的数据时，会话 1 中的一级缓存会被清理，会话 1 再去查询 ID 为 1 的数据时就会查询数据库，但是会话 2 中去查询 ID 为 1 的数据时还是会命中缓存，所以就会导致两个数据不一致的问题。
![](https://cdn.nlark.com/yuque/__puml/5d01609fa0e80681f3a7e23e26bf2dd8.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxuYWN0b3IgXCLnlKjmiLdcIiBhcyBVc2VyXG5wYXJ0aWNpcGFudCBcIlNlc3Npb24xXCJcbnBhcnRpY2lwYW50IFwiU2Vzc2lvbjJcIlxucGFydGljaXBhbnQgXCJEYXRhYmFzZVwiICNvcmFuZ2VcbnBhcnRpY2lwYW50IFwiQ2FjaGVcIiAjc2t5Ymx1ZVxuXG5Vc2VyIC0-IFNlc3Npb24xOiDlj5Hotbfor7fmsYJcblVzZXIgLT4gU2Vzc2lvbjI6IOWPkei1t-ivt-axglxuU2Vzc2lvbjEgLT4gRGF0YWJhc2U6IGdldCgxKVxuU2Vzc2lvbjIgLT4gRGF0YWJhc2U6IGdldCgyKVxuRGF0YWJhc2UgLS0-IFNlc3Npb24xOiDov5Tlm57nu5Pmnpzpm4ZhMVxuRGF0YWJhc2UgLS0-IFNlc3Npb24yOiDov5Tlm57nu5Pmnpzpm4ZhMlxuU2Vzc2lvbjEgLT4gQ2FjaGU6IOiuvue9rue8k-WtmFxuU2Vzc2lvbjIgLT4gQ2FjaGU6IOiuvue9rue8k-WtmFxuU2Vzc2lvbjEgLT4gRGF0YWJhc2U6IOabtOaWsGExXG5TZXNzaW9uMSAtLT4gQ2FjaGU6IOa4hemZpOe8k-WtmFxuU2Vzc2lvbjEgLT4gRGF0YWJhc2U6IGdldCgxKVxuU2Vzc2lvbjIgLT4gQ2FjaGU6IGdldCgxKVxuU2Vzc2lvbjEgPC0tIERhdGFiYXNlOiBiMVxuQ2FjaGUgLS0-IFNlc3Npb24yOiBiMlxubm90ZSBvdmVyIFNlc3Npb24xLFNlc3Npb24yOiDmn6Xor6Lnu5PmnpxiMSxiMuS4jeS4gOiHtFxuXG5AZW5kdW1sIiwidXJsIjoiaHR0cHM6Ly9jZG4ubmxhcmsuY29tL3l1cXVlL19fcHVtbC81ZDAxNjA5ZmEwZTgwNjgxZjNhN2UyM2UyNmJmMmRkOC5zdmciLCJpZCI6InVXVVV2IiwibWFyZ2luIjp7InRvcCI6dHJ1ZSwiYm90dG9tIjp0cnVlfSwiY2FyZCI6ImRpYWdyYW0ifQ==)问题 2 在同一个会话当中第一次查询 ID 为 1 的数据时会把查询到的结果集对象放到一级缓存当中，当第二次查询 ID 为 1 的数据时会把缓存的对象直接返回，因为 MyBatis 的一级缓存使用的是 Java 的 HashMap 缓存数据这里返回的都是对象的引用地址，这就导致结果集 a1 和 a2 其实都指向了同一个对象，所以不管 a1 还是 a2 修改了对象的字段都会导致双方的数据被修改。
![](https://cdn.nlark.com/yuque/__puml/334a866e4ad23d814ab2246abc8e845e.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxuYWN0b3IgXCLnlKjmiLdcIiBhcyBVc2VyXG5wYXJ0aWNpcGFudCBcIlNlc3Npb25cIlxucGFydGljaXBhbnQgXCJEYXRhYmFzZVwiICNvcmFuZ2VcbnBhcnRpY2lwYW50IFwiQ2FjaGVcIiAjc2t5Ymx1ZVxuXG5Vc2VyIC0-IFNlc3Npb246IOivt-axglxuU2Vzc2lvbiAtPiBEYXRhYmFzZTogZ2V0KDEpXG5EYXRhYmFzZSAtLT4gU2Vzc2lvbjog6L-U5Zue57uT5p6c6ZuGYTFcblNlc3Npb24gLT4gQ2FjaGU6IOiuvue9rue8k-WtmFxuU2Vzc2lvbiAtPiBDYWNoZTogZ2V0KDEpXG5DYWNoZSAtLT4gU2Vzc2lvbjog6L-U5Zue57uT5p6c6ZuGYTJcbm5vdGUgcmlnaHQgb2YgQ2FjaGU6IOS9v-eUqEphdmHnmoRIYXNoTWFw57yT5a2Y5pWw5o2uXG5cbkBlbmR1bWwiLCJ1cmwiOiJodHRwczovL2Nkbi5ubGFyay5jb20veXVxdWUvX19wdW1sLzMzNGE4NjZlNGFkMjNkODE0YWIyMjQ2YWJjOGU4NDVlLnN2ZyIsImlkIjoid3FYQVEiLCJtYXJnaW4iOnsidG9wIjp0cnVlLCJib3R0b20iOnRydWV9LCJjYXJkIjoiZGlhZ3JhbSJ9)**原理**<br />这里先简单了解一下使用 MyBatis 操作数据库时大概的一个流程：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1703040861933-483f59eb-a278-4748-bb1c-50c90f6d654d.jpeg)<br />基于上面这个流程图可以看出 SqlSession 用于创建和连接数据库 Executor 用于执行 SQL，那 MyBatis 的一级缓存在哪里呢？下面看一下类图和源码。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1703035059624-a83aeaa9-7983-40d1-9803-dfe0e623def6.png#averageHue=%23f6f6f6&clientId=u1528f4a3-9153-4&from=paste&id=u7e64a386&originHeight=837&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4bbb1796-ddfa-44e4-b1d7-817a091f20c&title=)
```java
public class DefaultSqlSession implements SqlSession {

    private final Configuration configuration;
    private final Executor executor;
    private final boolean autoCommit;
    private boolean dirty;
    private List<Cursor<?>> cursorList;

    public DefaultSqlSession(Configuration configuration, Executor executor, boolean autoCommit) {
        this.configuration = configuration;
        this.executor = executor;
        this.dirty = false;
        this.autoCommit = autoCommit;
    }

    public DefaultSqlSession(Configuration configuration, Executor executor) {
        this(configuration, executor, false);
    }

    private <E> List<E> selectList(String statement, Object parameter, RowBounds rowBounds, ResultHandler handler) {
        try {
            MappedStatement ms = configuration.getMappedStatement(statement);
            dirty |= ms.isDirtySelect();
            return executor.query(ms, wrapCollection(parameter), rowBounds, handler);
        } catch (Exception e) {
            throw ExceptionFactory.wrapException("Error querying database.  Cause: " + e, e);
        } finally {
            ErrorContext.instance().reset();
        }
    }

    @Override
    public void clearCache() {
        executor.clearLocalCache();
    }
}
```
```java
public abstract class BaseExecutor implements Executor {

    private static final Log log = LogFactory.getLog(BaseExecutor.class);
    protected Transaction transaction;
    protected Executor wrapper;
    protected ConcurrentLinkedQueue<DeferredLoad> deferredLoads;
    protected PerpetualCache localCache;
    protected PerpetualCache localOutputParameterCache;
    protected Configuration configuration;
    protected int queryStack;
    private boolean closed;

    protected BaseExecutor(Configuration configuration, Transaction transaction) {
        this.transaction = transaction;
        this.deferredLoads = new ConcurrentLinkedQueue<>();
        this.localCache = new PerpetualCache("LocalCache");
        this.localOutputParameterCache = new PerpetualCache("LocalOutputParameterCache");
        this.closed = false;
        this.configuration = configuration;
        this.wrapper = this;
    }

    @SuppressWarnings("unchecked")
    @Override
    public <E> List<E> query(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler,
                             CacheKey key, BoundSql boundSql) throws SQLException {
        ErrorContext.instance().resource(ms.getResource()).activity("executing a query").object(ms.getId());
        if (closed) {
            throw new ExecutorException("Executor was closed.");
        }
        if (queryStack == 0 && ms.isFlushCacheRequired()) {
            clearLocalCache();
        }
        List<E> list;
        try {
            queryStack++;
            // 使用本地缓存中的数据。
            list = resultHandler == null ? (List<E>) localCache.getObject(key) : null;
            if (list != null) {
                handleLocallyCachedOutputParameters(ms, key, parameter, boundSql);
            } else {
                list = queryFromDatabase(ms, parameter, rowBounds, resultHandler, key, boundSql);
            }
        } finally {
            queryStack--;
        }
        if (queryStack == 0) {
            for (DeferredLoad deferredLoad : deferredLoads) {
                deferredLoad.load();
            }
            deferredLoads.clear();
            // 当 localCacheScope 设置为 statement 时，查询后会清除本地缓存
            if (configuration.getLocalCacheScope() == LocalCacheScope.STATEMENT) {
                clearLocalCache();
            }
        }
        return list;
    }

    @Override
    public void clearLocalCache() {
        // 清除本地缓存
        if (!closed) {
            localCache.clear();
            localOutputParameterCache.clear();
        }
    }
}
```
```java
public class PerpetualCache implements Cache {

    private final String id;
    // 使用 Map 来缓存数据。
    private final Map<Object, Object> cache = new HashMap<>();

    public PerpetualCache(String id) {
        this.id = id;
    }

    @Override
    public String getId() {
        return id;
    }

    @Override
    public int getSize() {
        return cache.size();
    }

    @Override
    public void putObject(Object key, Object value) {
        cache.put(key, value);
    }

    @Override
    public Object getObject(Object key) {
        return cache.get(key);
    }

    @Override
    public Object removeObject(Object key) {
        return cache.remove(key);
    }

    @Override
    public void clear() {
        cache.clear();
    }

    @Override
    public boolean equals(Object o) {
        if (getId() == null) {
            throw new CacheException("Cache instances require an ID.");
        }
        if (this == o) {
            return true;
        }
        if (!(o instanceof Cache)) {
            return false;
        }

        Cache otherCache = (Cache) o;
        return getId().equals(otherCache.getId());
    }

    @Override
    public int hashCode() {
        if (getId() == null) {
            throw new CacheException("Cache instances require an ID.");
        }
        return getId().hashCode();
    }

}
```
`DefaultSqlSession` 和 `BaseExecutor` 分别是 `SqlSession` 和 Executor 的默认实现。PerpetualCache 则提供了基础的缓存功能，使用 HashMap 存放缓存对象，MyBatis 给 `PerpetualCache` 提供了多种装饰器类用于增强 `PerpetualCache` 的功能，但是一级缓存都不会用到，这里就先不介绍了。<br />通过这个类图可以了解到 SqlSession 会使用 Executor 去执行 SQL，而 Executor 中存在 localCache 对象，通过源码可以了解到 Executor 会先尝试从缓存中获取，如果获取不到才会查询数据库。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1703040221104-e6a49689-f9bd-47a6-83e1-be128796a0d3.jpeg)<br />通过上面这个流程图，可以了解到在同一个 SqlSession 下相同的查询只有第一次会查询数据库，后续的重复查询或者嵌套查询都会使用缓存。但是当 `localCacheScope` 设置为 `STATEMENT` 时在返回查询结果前反而会去清空缓存，到这里基本介绍完了 MyBatis 一级缓存的所有内容。<br />这里拓展一点 MyBatis 在 Spring 当中的情况，因为上面都是在说 SqlSession，但是在 Spring 当中使用 MyBatis 的时候大部分都是直接使用 Mapper 类或者使用` SqlSessionTemplate` 来操作数据库，那在 Spring 中 SqlSession 去哪里了呢？其实在 Spring 当中 MyBatis 的 SqlSession 基本和 Spring 的事务进行了绑定，下面看一下基本的流程。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1703039612860-288a9059-9749-456d-8547-3e1c6827425d.jpeg)
<a name="qPVlG"></a>
## 总结
最后了解一下如何避免一级缓存的坑：

1. 最简单的办法，把 `localCacheScope` 设置为 `STATEMENT` 这样每次执行完查询后都会清除缓存，基本上就是把一级缓存关闭了，就不会导致上述的两个问题了。分布式应用推荐都把 `localCacheScope` 设置为 `STATEMENT`。
2. 如果只是要避免问题 2 也可以简单的把查询的数据进行深拷贝，避免因为浅拷贝的问题造成数据异常的问题。但是最好还是在写代码的时候注意不要直接修改任何查询到的数据，因为大部分的本地缓存框架都会使用 HashMap 进行数据缓存，这样的缓存都会存在浅拷贝的问题。
