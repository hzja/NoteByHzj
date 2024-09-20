JavaMybatis
<a name="M90X0"></a>
## 什么是延迟加载？
延迟加载也称为懒加载、惰性加载，使用延迟加载可以提高程序的运行效率，针对数据持久层的操作，在某些特定查询的情况下去访问特定的数据库，在其他情况下可以不访问某些数据表，尽量减少 SQL 的执行，从而达到提高速度的目的，是对数据库操作的一种优化。
<a name="hHkAr"></a>
### 什么情况下会出现延迟加载的情况
来举个简单的例子<br />在一对多中，当有一个用户，它有个100个订单 在查询用户的时候，要不要把关联的订单查出来？ 在查询订单的时候，要不要把关联的用户查出来？<br />那这个时候，答案就很清晰了，肯定要查出来的，但是，是在需要的地方。<br />在查询用户时，用户下的订单应该是，什么时候用，什么时候查询。<br />在查询订单时，订单所属的用户信息应该是随着订单一起查询出来<br />这个时候就会用到延迟加载了，用的时候就查询，不用的时候，就不再继续的进行查询了。<br />**注意**：延迟加载的应用要求：关联对象的查询与主加载对象的查询必须是分别进行的select语句，不能是使用多表连接所进行的select查询。
<a name="ODgvk"></a>
### 延迟加载分类
其实延迟加载也是有区分对的，而这个区分就是，**全局的延迟加载**和**局部的延迟加载**。
<a name="b2KB5"></a>
### 全局延迟加载
全局延迟加载的实现，实际上是通过修改配置文件来进行实现的，只要改了对应的配置配置文件，重启之后，肯定直接全局实现，所有需要用到全局加载的 xml 文件，都可以进行实现了。<br />修改内容如下：
```xml
<settings>
  <!--开启全局延迟加载功能-->
  <setting name="lazyLoadingEnabled" value="true"/>
</settings>
```
<a name="V1riU"></a>
### 局部延迟加载
在`association`和`collection`标签中都有⼀个`fetchType`属性，通过修改它的值，可以修改局部的加载策略。<br />实现方式如下：
```xml
<!-- 开启⼀对多 延迟加载 -->
<resultMap id="userMap" type="user">
    <id column="id" property="id"></id>
    <result column="username" property="username"></result>
    <result column="password" property="password"></result>
    <result column="birthday" property="birthday"></result>
<!--
fetchType="lazy" 懒加载策略
fetchType="eager" ⽴即加载策略
-->
    <collection property="orderList" ofType="order" column="id"
        select="com.lagou.dao.OrderMapper.findByUid" fetchType="lazy">
    </collection>
</resultMap>
<select id="findAll" resultMap="userMap">
    SELECT * FROM `user`
</select>
```
其实在这里，需要注意一个内容，那就是延迟加载的时候：局部的加载策略的优先级高于全局的加载策略。<br />这个也是面试的时候，经常会被问到的内容。<br />那么就得来看看这个延迟加载的具体实现：<br />**Spring配置加载**
```java
public class Configuration {
    /**
     * aggressiveLazyLoading：
     * 当开启时，任何⽅法的调⽤都会加载该对象的所有属性。否则，每个属性会按需加载（参考lazyLoadTriggerMethods).
     * 默认为true
     */
    protected boolean aggressiveLazyLoading;
    /**
     * 延迟加载触发⽅法
     */
    protected Set<String> lazyLoadTriggerMethods = new HashSet<String>(Arrays.asList(new String[]{"equals", "clone", "hashCode", "toString" }));
    /**
     * 是否开启延迟加载
     */
    protected boolean lazyLoadingEnabled = false;

    /**
     * 默认使⽤Javassist代理⼯⼚
     *
     * @param proxyFactory
     */
    public void setProxyFactory(ProxyFactory proxyFactory) {
        if (proxyFactory == null) {
            proxyFactory = new JavassistProxyFactory();
        }
        this.proxyFactory = proxyFactory;
    }
    //省略...
}
```
Spring的加载已经看到了，接下来还得看看延迟加载代理对象创建，他都是怎么来创建的，说到创建类，那么就得找到这个 `ResultSetHandler` 这个类了，内部是有有个 `handleResultSets` 的方法，而方法内部，就有加载的过程。默认采用javassistProxy进行代理对象的创建
```java
// 创建映射后的结果对象
private Object createResultObject(ResultSetWrapper rsw, ResultMap resultMap, ResultLoaderMap lazyLoader, String columnPrefix) throws SQLException {
    // useConstructorMappings ，表示是否使用构造方法创建该结果对象。此处将其重置
    this.useConstructorMappings = false; // reset previous mapping result
    final List<Class<?>> constructorArgTypes = new ArrayList<>(); // 记录使用的构造方法的参数类型的数组
    final List<Object> constructorArgs = new ArrayList<>(); // 记录使用的构造方法的参数值的数组
    // 创建映射后的结果对象
    Object resultObject = createResultObject(rsw, resultMap, constructorArgTypes, constructorArgs, columnPrefix);
    if (resultObject != null && !hasTypeHandlerForResultObject(rsw, resultMap.getType())) {
        // 如果有内嵌的查询，并且开启延迟加载，则创建结果对象的代理对象
        final List<ResultMapping> propertyMappings = resultMap.getPropertyResultMappings();
        for (ResultMapping propertyMapping : propertyMappings) {
            // issue gcode #109 && issue #149
            if (propertyMapping.getNestedQueryId() != null && propertyMapping.isLazy()) {
                // 创建延迟加载代理对象
                resultObject = configuration.getProxyFactory().createProxy(resultObject, lazyLoader, configuration, objectFactory, constructorArgTypes, constructorArgs);
                break;
            }
        }
    }
    // 判断是否使用构造方法创建该结果对象
    this.useConstructorMappings = resultObject != null && !constructorArgTypes.isEmpty(); // set current mapping result
    return resultObject;
}
```
如果想要在代码中验证延迟加载是否生效的话，那么有一个很简单的方法，开启日志的 SQL 打印功能，那么就可以直接验证延迟加载是否生效了。
<a name="tnLLA"></a>
## 为什么需要延迟加载
其实这也是延迟加载的优点，优点如下：<br />先从单表查询，需要时再从关联表去关联查询，大大提高数据库性能，因为查询单表要比关联查询多张表速度要快。<br />但是缺点也很明显：<br />有当需要用到数据时，才会进行数据库查询，这样在大批量数据查询时，因为查询工作也要消耗时间，所以可能造成⽤户等待时间变长，造成用户体验下降。
