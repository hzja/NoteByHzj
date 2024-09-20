Java Mybatis 缓存
<a name="r2HFg"></a>
## 1、MyBatis缓存介绍
Mybatis提供对缓存的支持，但是在没有配置的默认情况下，它只开启一级缓存，二级缓存需要手动开启。<br />**一级缓存只是相对于同一个SqlSession而言。** 也就是针对于同一事务，多次执行同一Mapper的相同查询方法，第一查询后，MyBatis会将查询结果放入缓存，在中间不涉及相应Mapper的数据更新（Insert，Update和Delete）操作的情况下，后续的查询将会从缓存中获取，而不会查询数据库。<br />**二级缓存是针对于应用级别的缓存，也就是针对不同的SqlSession做到缓存。 **当开启二级缓存时，MyBatis会将首次查询结果存入对于Mapper的全局缓存，如果中间不执行该Mapper的数据更新操作，那么后续的相同查询都将会从缓存中获取。
<a name="kvatj"></a>
## 2、二级缓存问题
根据二级缓存的介绍发现，如果Mapper只是单表查询，并不会出现问题，但是如果Mapper涉及的查询出现 联表 查询，如 UserMapper 在查询 user 信息时需要关联查询 组织信息，也就是需要 user 表和 organization 表关联，OrganizationMapper 在执行更新时并不会更新 UserMapper 的缓存，结果会导致在使用相同条件 使用 UserMapper 查询 user 信息时，会等到未更新前的 organization 信息，造成数据不一致的情况。
<a name="vSNa6"></a>
### 2.1、数据不一致问题验证
查询SQL
```plsql
SELECT
 u.*, o.name org_name 
FROM
 user u
 LEFT JOIN organization o ON u.org_id = o.id 
WHERE
 u.id = #{userId}
```
UserMapper
```java
UserInfo queryUserInfo(@Param("userId") String userId);
```
UserService
```java
public UserEntity queryUser(String userId) {

    UserInfo userInfo = userMapper.queryUserInfo(userId);

    return userInfo;
}
```
调用查询，得到查询结果（多次查询，得到缓存数据），这里 userId = 1，data为user查询结果
```json
{
  "code": "1",
  "message": null,
  "data": {
    "id": "1",
    "username": "admin",
    "password": "admin",
    "orgName": "组织1"
  }
}
```
查询 对应 organization 信息，结果
```json
{
  "code": "1",
  "message": null,
  "data": {
    "id": "1",
    "name": "组织1"
  }
}
```
发现和user缓存数据一致。<br />执行更新 organization 操作，将 组织1 改为 组织2，再次查询组织信息
```json
{
  "code": "1",
  "message": null,
  "data": {
    "id": "1",
    "name": "组织2"
  }
}
```
再次查询user信息，发现依旧从缓存中获取
```json
{
  "code": "1",
  "message": null,
  "data": {
    "id": "1",
    "username": "admin",
    "password": "admin",
    "orgName": "组织1"
  }
}
```
造成此问题原因为 organization 数据信息更新只会自己Mapper对应的缓存数据，而不会通知到关联表organization 的一些Mapper更新对应的缓存数据。
<a name="C9dk8"></a>
### 2.2、问题处理思路

- 在 Mapper1 定义时，手动配置 相应的关联 Mapper2
- 在 Mapper1 缓存 cache1 实例化时，读取 所关联的 Mapper2 的缓存 cache2相关信息
- 在 cache1 中存储 cache2 的引用信息
- cache1 执行clear时，同步操作 cache2 执行clear
<a name="NYv5Q"></a>
## 3、关联缓存刷新实现
打开二级缓存，本地项目使用 MyBatis Plus
```
mybatis-plus.configuration.cache-enabled=true
```
主要用到自定义注解`CacheRelations`，自定义缓存实现`RelativeCache`和缓存上下文`RelativeCacheContext`。<br />注解`CacheRelations`，使用时需标注在对应mapper上
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface CacheRelations {
    // from中mapper class对应的缓存更新时，需要更新当前注解标注mapper的缓存
    Class<?>[] from() default {};
    // 当前注解标注mapper的缓存更新时，需要更新to中mapper class对应的缓存
    Class<?>[] to() default {};
}
```
自定义缓存`RelativeCache`实现 MyBatis Cache 接口
```java
public class RelativeCache implements Cache {

    private Map<Object, Object> CACHE_MAP = new ConcurrentHashMap<>();

    private List<RelativeCache> relations = new ArrayList<>();

    private ReadWriteLock readWriteLock = new ReentrantReadWriteLock(true);

    private String id;
    private Class<?> mapperClass;
    private boolean clearing;

    public RelativeCache(String id) throws Exception {
        this.id = id;
        this.mapperClass = Class.forName(id);
        RelativeCacheContext.putCache(mapperClass, this);
        loadRelations();
    }

    @Override
    public String getId() {
        return id;
    }

    @Override
    public void putObject(Object key, Object value) {
        CACHE_MAP.put(key, value);
    }

    @Override
    public Object getObject(Object key) {
        return CACHE_MAP.get(key);
    }

    @Override
    public Object removeObject(Object key) {
        return CACHE_MAP.remove(key);
    }

    @Override
    public void clear() {
        ReadWriteLock readWriteLock = getReadWriteLock();
        Lock lock = readWriteLock.writeLock();
        lock.lock();
        try {
            // 判断 当前缓存是否正在清空，如果正在清空，取消本次操作
            // 避免缓存出现 循环 relation，造成递归无终止，调用栈溢出
            if (clearing) {
                return;
            }
            clearing = true;
            try {
                CACHE_MAP.clear();
                relations.forEach(RelativeCache::clear);
            } finally {
                clearing = false;
            }
        } finally {
            lock.unlock();
        }


    }

    @Override
    public int getSize() {
        return CACHE_MAP.size();
    }

    @Override
    public ReadWriteLock getReadWriteLock() {
        return readWriteLock;
    }

    public void addRelation(RelativeCache relation) {
        if (relations.contains(relation)){
            return;
        }
        relations.add(relation);
    }

    void loadRelations() {
        // 加载 其他缓存更新时 需要更新此缓存的 caches
        // 将 此缓存 加入至这些 caches 的 relations 中
        List<RelativeCache> to = UN_LOAD_TO_RELATIVE_CACHES_MAP.get(mapperClass);
        if (to != null) {
            to.forEach(relativeCache -> this.addRelation(relativeCache));
        }
        // 加载 此缓存更新时 需要更新的一些缓存 caches
        // 将这些缓存 caches 加入 至 此缓存 relations 中
        List<RelativeCache> from = UN_LOAD_FROM_RELATIVE_CACHES_MAP.get(mapperClass);
        if (from != null) {
            from.forEach(relativeCache -> relativeCache.addRelation(this));
        }

        CacheRelations annotation = AnnotationUtils.findAnnotation(mapperClass, CacheRelations.class);
        if (annotation == null) {
            return;
        }

        Class<?>[] toMappers = annotation.to();
        Class<?>[] fromMappers = annotation.from();

        if (toMappers != null && toMappers.length > 0) {
            for (Class c : toMappers) {
                RelativeCache relativeCache = MAPPER_CACHE_MAP.get(c);
                if (relativeCache != null) {
                    // 将找到的缓存添加到当前缓存的relations中
                    this.addRelation(relativeCache);
                } else {
                    // 如果找不到 to cache，证明to cache还未加载，这时需将对应关系存放到 UN_LOAD_FROM_RELATIVE_CACHES_MAP
                    // 也就是说 c 对应的 cache 需要 在 当前缓存更新时 进行更新
                    List<RelativeCache> relativeCaches = UN_LOAD_FROM_RELATIVE_CACHES_MAP.putIfAbsent(c, new ArrayList<RelativeCache>());
                    relativeCaches.add(this);
                }
            }
        }

        if (fromMappers != null && fromMappers.length > 0) {
            for (Class c : fromMappers) {
                RelativeCache relativeCache = MAPPER_CACHE_MAP.get(c);
                if (relativeCache != null) {
                    // 将找到的缓存添加到当前缓存的relations中
                    relativeCache.addRelation(this);
                } else {
                    // 如果找不到 from cache，证明from cache还未加载，这时需将对应关系存放到 UN_LOAD_TO_RELATIVE_CACHES_MAP
                    // 也就是说 c 对应的 cache 更新时需要更新当前缓存
                    List<RelativeCache> relativeCaches = UN_LOAD_TO_RELATIVE_CACHES_MAP.putIfAbsent(c, new ArrayList<RelativeCache>());
                    relativeCaches.add(this);
                }
            }
        }
    }

}
```
缓存上下文`RelativeCacheContext`
```java
public class RelativeCacheContext {

    // 存储全量缓存的映射关系
    public static final Map<Class<?>, RelativeCache> MAPPER_CACHE_MAP = new ConcurrentHashMap<>();
    // 存储 Mapper 对应缓存 需要to更新缓存，但是此时 Mapper 对应缓存还未加载
    // 也就是 Class<?> 对应的缓存更新时，需要更新 List<RelativeCache> 中的缓存
    public static final Map<Class<?>, List<RelativeCache>> UN_LOAD_TO_RELATIVE_CACHES_MAP = new ConcurrentHashMap<>();
    // 存储 Mapper 对应缓存 需要from更新缓存，但是在 加载 Mapper 缓存时，这些缓存还未加载
    // 也就是 List<RelativeCache> 中的缓存更新时，需要更新 Class<?> 对应的缓存
    public static final Map<Class<?>, List<RelativeCache>> UN_LOAD_FROM_RELATIVE_CACHES_MAP = new ConcurrentHashMap<>();

    public static void putCache(Class<?> clazz, RelativeCache cache) {
        MAPPER_CACHE_MAP.put(clazz, cache);
    }

    public static void getCache(Class<?> clazz) {
        MAPPER_CACHE_MAP.get(clazz);
    }

}
```
<a name="I0Ioc"></a>
### 使用方式
UserMapper.java
```java
@Repository
@CacheNamespace(implementation = RelativeCache.class, eviction = RelativeCache.class, flushInterval = 30 * 60 * 1000)
@CacheRelations(from = OrganizationMapper.class)
public interface UserMapper extends BaseMapper<UserEntity> {
    UserInfo queryUserInfo(@Param("userId") String userId);
}
```
queryUserInfo是xml实现的接口，所以需要在对应xml中配置`<cache-ref namespace="com.mars.system.dao.UserMapper"/>`，不然查询结果不会被缓存化。如果接口为 BaseMapper实现，查询结果会自动缓存化。<br />UserMapper.xml
```xml
<mapper namespace="com.mars.system.dao.UserMapper">
    <cache-ref namespace="com.mars.system.dao.UserMapper"/>
    <select id="queryUserInfo" resultType="com.mars.system.model.UserInfo">
        select u.*, o.name org_name from user u left join organization o on u.org_id = o.id
        where u.id = #{userId}
    </select>
</mapper>
```
OrganizationMapper.java
```java
@Repository
@CacheNamespace(implementation = RelativeCache.class, eviction = RelativeCache.class, flushInterval = 30 * 60 * 1000)
public interface OrganizationMapper extends BaseMapper<OrganizationEntity> {
}
```
`CacheNamespace`中flushInterval在默认情况下是无效的，也就是说缓存并不会定时清理。`ScheduledCache`是对flushInterval功能的实现，MyBatis 的缓存体系是用装饰器进行功能扩展的，所以，如果需要定时刷新，需要使用`ScheduledCache`给到 `RelativeCache`添加装饰。<br />至此，配置和编码完成。<br />开始验证：<br />查询 userId=1的用户信息
```json
{
    "code":"1",
    "message":null,
    "data":{
        "id":"1",
        "username":"admin",
        "password":"admin",
        "orgName":"组织1"
    }
}
```
更新组织信息，将 组织1 改为 组织2
```json
{
    "code":"1",
    "message":null,
    "data":{
        "id":"1",
        "name":"组织2"
    }
}
```
再次查询用户信息
```json
{
    "code":"1",
    "message":null,
    "data":{
        "id":"1",
        "username":"admin",
        "password":"admin",
        "orgName":"组织2"
    }
}
```
符合预期。
