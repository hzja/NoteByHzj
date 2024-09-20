Java Mybatis
<a name="UFXT9"></a>
## Mybatis-PageHelper
目前Mybatis-PageHelper支持对以下种类繁多的的数据库物理分页：
```java
static {
    //注册别名
    registerDialectAlias("hsqldb", HsqldbDialect.class);
    registerDialectAlias("h2", HsqldbDialect.class);
    registerDialectAlias("phoenix", HsqldbDialect.class);
    registerDialectAlias("postgresql", PostgreSqlDialect.class);
    registerDialectAlias("mysql", MySqlDialect.class); 
    registerDialectAlias("mariadb", MySqlDialect.class); 
    registerDialectAlias("sqlite", MySqlDialect.class);
    registerDialectAlias("herddb", HerdDBDialect.class);
    registerDialectAlias("oracle", OracleDialect.class); 
    registerDialectAlias("oracle9i", Oracle9iDialect.class); 
    registerDialectAlias("db2", Db2Dialect.class); 
    registerDialectAlias("informix", InformixDialect.class); 
    //解决 informix-sqli #129，仍然保留上面的 
    registerDialectAlias("informix-sqli", InformixDialect.class);
    registerDialectAlias("sqlserver", SqlServerDialect.class); 
    registerDialectAlias("sqlserver2012", SqlServer2012Dialect.class);
    registerDialectAlias("derby", SqlServer2012Dialect.class); 
    //达梦数据库,https://github.com/mybatis-book/book/issues/43 
    registerDialectAlias("dm", OracleDialect.class); 
    //阿里云PPAS数据库,https://github.com/pagehelper/Mybatis-PageHelper/issues/281 
    registerDialectAlias("edb", OracleDialect.class); 
    //神通数据库 
    registerDialectAlias("oscar", OscarDialect.class); 
    registerDialectAlias("clickhouse", MySqlDialect.class); 
    //瀚高数据库 
    registerDialectAlias("highgo", HsqldbDialect.class); 
    //虚谷数据库 
    registerDialectAlias("xugu", HsqldbDialect.class); 
}
```
当然，如果使用的数据库恰好不在这些范围之内，别着急，Mybatis-PageHelper还支持自定义配置，只需要配置 `dialectAlias` 参数，配置的内容根据 JDBCURL 自动获取对应实现，允许通过此种方式覆盖已有的实现，切记多个配置时使用分号隔开，举例如下：
```xml
<property name="dialectAlias" value="oracle=com.github.pagehelper.dialect.helper.OracleDialect"/>
```
<a name="haTeG"></a>
## 如何使用？
<a name="ERZwl"></a>
### 引入方式
两种，一种是直接引入jar包，一种是使用 Maven方式引入。<br />如果是后者，只需要在 pom.xml 中添加下面的依赖：
```xml
<dependency>
    <groupId>com.github.pagehelper</groupId>
    <artifactId>pagehelper</artifactId>
    <version>最新版本</version>
</dependency>
```
<a name="WtMeI"></a>
### 配置拦截器
<a name="eFJ3v"></a>
#### 1、在MyBatis配置xml文件中配置拦截器插件
```xml
<!-- 
    plugins在配置文件中的位置必须符合要求，否则会报错，顺序如下:
    properties?, settings?, 
    typeAliases?, typeHandlers?, 
    objectFactory?,objectWrapperFactory?, 
    plugins?, 
    environments?, databaseIdProvider?, mappers?
-->
<plugins>
    <!-- com.github.pagehelper为PageHelper类所在包名 -->
    <plugin interceptor="com.github.pagehelper.PageInterceptor">
        <!-- 使用下面的方式配置参数，后面会有所有的参数介绍 -->
        <property name="param1" value="value1"/>
 </plugin>
</plugins>
```
<a name="SSyDK"></a>
#### 2、在Spring配置文件中配置拦截器插件
```xml
<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
  <!-- 注意其他配置 -->
  <property name="plugins">
    <array>
      <bean class="com.github.pagehelper.PageInterceptor">
        <property name="properties">
          <!--使用下面的方式配置参数，一行配置一个 -->
          <value>
            params=value1
          </value>
        </property>
      </bean>
    </array>
  </property>
</bean>
```
<a name="rcLVW"></a>
### 插件参数
Mybatis-PageHelper提供了多个可选参数，如果小伙伴们想要实现自己的分页逻辑，也可以通过实现 `Dialect`(`com.github.pagehelper.Dialect`) 接口，来自定义 dialect 实现及参数。<br />下面介绍几个自带默认使用的常见参数：
<a name="GjnPa"></a>
##### 1、`helperDialect`
分页插件会自动检测当前的数据库链接，自动选择合适的分页方式。通过配置`helperDialect`属性来指定分页插件使用哪种语言。
<a name="PtEyj"></a>
##### 2、`offsetAsPageNum`
该参数对使用 `RowBounds` 作为分页参数时有效。当该参数设置为 `true` 时，会将 `RowBounds` 中的 `offset` 参数当成 `pageNum` 使用，可以用页码和页面大小两个参数进行分页。
<a name="mbeIb"></a>
##### 3、 `rowBoundsWithCount`
该参数对使用 `RowBounds` 作为分页参数时有效。当该参数设置为`true`时，使用 `RowBounds` 分页会进行 count 查询。
<a name="PWItr"></a>
##### 4、`pageSizeZero`
该参数设置为 `true` 时，如果 `pageSize=0` 或者 `RowBounds.limit = 0` 就会查询出全部的结果（相当于没有执行分页查询，但是返回结果仍然是 `Page` 类型）。
<a name="CdcoM"></a>
##### 5、 `reasonable`
分页合理化参数，当该参数设置为 `true` 时，`pageNum<=0` 时会查询第一页；`pageNum>pages`（超过总数时），会查询最后一页。
<a name="w9HAw"></a>
##### 6、`params`
该参数来配置参数映射，用于从对象中根据属性名取值。
<a name="SYNqN"></a>
##### 7、`supportMethodsArguments`
支持通过 Mapper 接口参数来传递分页参数，分页插件会从查询方法的参数值中，自动根据上面第6点 `params` 配置的字段中取值，查找到合适的值时就会自动分页。
<a name="QB1Vr"></a>
##### 8、`autoRuntimeDialect`
允许在运行时根据多数据源自动识别对应方言的分页
<a name="OVslv"></a>
##### 9、`closeConn`
当使用运行时动态数据源或没有设置 `helperDialect` 属性自动获取数据库类型时，会自动获取一个数据库连接，通过该属性来设置是否关闭获取的这个连接。
<a name="gw2YQ"></a>
### 调用方式
下面是6种不同的调用方式：
```java
//第一种，RowBounds方式的调用
List<User> list = sqlSession.selectList("x.y.selectIf", null, new RowBounds(0, 10));

//第二种，Mapper接口方式的调用，推荐这种使用方式。
PageHelper.startPage(1, 10);
List<User> list = userMapper.selectIf(1);

//第三种，Mapper接口方式的调用，推荐这种使用方式。
PageHelper.offsetPage(1, 10);
List<User> list = userMapper.selectIf(1);

//第四种，参数方法调用
//存在以下 Mapper 接口方法，你不需要在 xml 处理后两个参数
public interface CountryMapper {
    List<User> selectByPageNumSize(
            @Param("user") User user,
            @Param("pageNum") int pageNum, 
            @Param("pageSize") int pageSize);
}
//配置supportMethodsArguments=true
//在代码中直接调用：
List<User> list = userMapper.selectByPageNumSize(user, 1, 10);

//第五种，参数对象
//如果 pageNum 和 pageSize 存在于 User 对象中，只要参数有值，也会被分页
//有如下 User 对象
public class User {
    //其他fields
    //下面两个参数名和 params 配置的名字一致
    private Integer pageNum;
    private Integer pageSize;
}
//存在以下 Mapper 接口方法，你不需要在 xml 处理后两个参数
public interface CountryMapper {
    List<User> selectByPageNumSize(User user);
}
//当 user 中的 pageNum!= null && pageSize!= null 时，会自动分页
List<User> list = userMapper.selectByPageNumSize(user);

//第六种，ISelect 接口方式
//jdk6,7用法，创建接口
Page<User> page = PageHelper.startPage(1, 10).doSelectPage(new ISelect() {
    @Override
    public void doSelect() {
        userMapper.selectGroupBy();
    }
});
//jdk8 lambda用法
Page<User> page = PageHelper.startPage(1, 10).doSelectPage(()-> userMapper.selectGroupBy());

//也可以直接返回PageInfo，注意doSelectPageInfo方法和doSelectPage
pageInfo = PageHelper.startPage(1, 10).doSelectPageInfo(new ISelect() {
    @Override
    public void doSelect() {
        userMapper.selectGroupBy();
    }
});
//对应的lambda用法
pageInfo = PageHelper.startPage(1, 10).doSelectPageInfo(() -> userMapper.selectGroupBy());

//count查询，返回一个查询语句的count数
long total = PageHelper.count(new ISelect() {
    @Override
    public void doSelect() {
        userMapper.selectLike(user);
    }
});
//lambda
total = PageHelper.count(()->userMapper.selectLike(user));
```
