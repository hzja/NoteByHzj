Java Mybatis<br />fastmybatis 是一个 mybatis 开发框架，其宗旨为：简单、快速、有效。零配置快速上手，无需编写 xml 文件即可完成 CRUD 操作。同时支持 mysql、sqlserver、oracle、postgresql、sqlite。<br />支持自定义 SQL，对于基本的增删改查不需要写 SQL，对于其它特殊 SQL（如统计 SQL ）可写在 xml 中，支持与 Spring-Boot 集成，依赖 starter 即可，支持插件编写，支持 ActiveRecord 模式，提供通用 Service，轻量级，无侵入性，是官方 MyBatis 的一种扩展
<a name="ZOGZK"></a>
## 快速开始（SpringBoot）
<a name="OIOrg"></a>
### 新建一个 Spring boot 项目
<a name="lwrVK"></a>
### pom.xml 添加 fastmybatis-spring-boot-starter
```xml
<dependency>
	<groupId>net.oschina.durcframework</groupId>
	<artifactId>fastmybatis-spring-boot-starter</artifactId>
	<version>最新版本</version>
</dependency>
```
<a name="hEA7P"></a>
### 增删改查例子
假设数据库有张 t_user 表，添加对应的实体类 TUser.java和 Mapper 类TUserMapper.java
```java
/**
 * 增删改查例子
 */
@RestController
public class CrudController {

    @Autowired
    private UserService userService;


    /**
     * 分页查询
     * http://localhost:8080/user/page?id=10
     * http://localhost:8080/user/page?pageIndex=1&pageSize=5
     *
     * @param param
     * @return
     */
    @GetMapping("/user/page")
    public Result<PageInfo<TUser>> page(UserParam param) {
        Query query = param.toQuery();
        PageInfo<TUser> pageInfo = userService.page(query);
        return Result.ok(pageInfo);
    }

    /**
     * 新增记录，这里为了方便演示用了GET方法，实际上应该使用POST
     * http://localhost:8080/user/save?username=jim
     *
     * @param user
     * @return
     */
    @GetMapping("/user/save")
    public Result<Integer> save(TUser user) {
        userService.saveIgnoreNull(user);
        // 返回添加后的主键值
        return Result.ok(user.getId());
    }

    /**
     * 修改记录，这里为了方便演示用了GET方法，实际上应该使用POST
     * http://localhost:8080/user/update?id=10&username=jim
     *
     * @param user 表单数据
     * @return
     */
    @GetMapping("/user/update")
    public Result<?> update(TUser user) {
        userService.updateIgnoreNull(user);
        return Result.ok();
    }

    /**
     * 删除记录，这里为了方便演示用了GET方法，实际上应该使用DELETE
     * http://localhost:8080/user/delete?id=10
     *
     * @param id 主键id
     * @return
     */
    @GetMapping("/user/delete")
    public Result<?> delete(Integer id) {
        userService.deleteById(id);
        return Result.ok();
    }
}
```
<a name="h1r88"></a>
### UserService.java
```java
// 实现通用接口
@Service
public class UserService implements IService<TUser/*实体类*/, Integer/*主键类型*/> {

}
```
<a name="T2D9H"></a>
### TUserMapper.java
```java
public interface TUserMapper extends CrudMapper<TUser/*实体类*/, Integer/*主键类型*/> {

}
```
service和mapper不用写一行代码就能实现各种数据库操作，非常方便。
<a name="KI4J0"></a>
## Mapper方法列表
| 方法 | 说明 |
| --- | --- |
| `E getByColumn(String column, Object value)` | 根据字段查询一条记录 |
| `E getById(I id)` | 根据主键查询 |
| `E getByQuery(Query query)` | 根据条件查找单条记录 |
| `E getBySpecifiedColumns(List<String> columns, Query query)` | 查询单条数据并返回指定字段 |
| `<T> T getBySpecifiedColumns(List<String> columns, Query query, Class<T> clazz)` | 查询单条数据返回指定字段并转换到指定类中 |
| `<T> T getColumnValue(String column, Query query, Class<T> clazz)` | 查询某一行某个字段值 |
| `long getCount(Query query)` | 查询总记录数 |
| `List<E> list(Query query)` | 查询结果集 |
| `List<E> listByArray(String column, Object[] values)` | 根据多个字段值查询结果集 |
| `List<E> listByCollection(String column, Collection<?> values)` | 根据字段多个值查询结果集 |
| `List<E> listByColumn(String column, Object value)` | 根据字段查询结果集 |
| `List<E> listByIds(Collection<I> ids)` | 根据多个主键查询 |
| `List<E> listBySpecifiedColumns(List<String> columns, Query query)` | 查询返回指定的列，返回实体类集合 |
| `<T> List<T> listBySpecifiedColumns(List<String> columns, Query query, Class<T> clazz)` | 查询返回指定的列，返指定类集合 |
| `<T> List<T> listColumnValues(String column, Query query, Class<T> clazz)` | 查询指定列，返指定列集合 |
| `PageInfo<E> page(Query query)` | 分页查询 |
| `<R> PageInfo<R> page(Query query, Function<E, R> converter)` | 查询结果集，并转换结果集中的记录，转换处理每一行 |
| `<R> PageInfo<R> page(Query query, Supplier<R> target, Consumer<R> format)` | 查询结果集，并转换结果集中的记录，并对记录进行额外处理 |
| `<T> PageInfo<T> page(Query query, Supplier<T> target)` | 查询结果集，并转换结果集中的记录 |
| `<R> PageInfo<R> pageAndConvert(Query query, Function<List<E>, List<R>> converter)` | 查询结果集，并转换结果集中的记录，转换处理list |
| `<T> PageInfo<T> pageBySpecifiedColumns(List<String> columns, Query query, Class<T> clazz)` | 查询返回指定的列，返回分页数据 |
| `PageEasyui<E> pageEasyui(Query query)` | 查询返回easyui结果集 |
| `<T> PageEasyui<T> pageEasyui(Query query, Class<T> clazz)` | 查询返回easyui结果集，并转换结果集中的记录 |
| `E forceById(I id)` | 根据主键查询强制查询，忽略逻辑删除字段 |
| `int save(E entity)` | 保存，保存所有字段 |
| `int saveBatch(Collection<E> entitys)` | 批量保存 |
| `int saveIgnoreNull(E entity)` | 保存，忽略null字段 |
| `int saveMultiSet(Collection<E> entitys)` | 批量保存,兼容更多的数据库版本,忽略重复行，此方式采用union的方式批量insert |
| `int saveOrUpdate(E entity)` | 保存或修改，当数据库存在记录执行UPDATE，否则执行INSERT |
| `int saveOrUpdateIgnoreNull(E entity)` | 保存或修改，忽略null字段，当数据库存在记录执行UPDATE，否则执行INSERT |
| `int saveUnique(Collection<E> entitys)` | 批量保存，去除重复行，通过对象是否相对判断重复数据，实体类需要实现equals方法 |
| `int saveUnique(Collection<E> entitys, Comparator<E> comparator)` | 批量保存，去除重复行，指定比较器判断 |
| `int update(E entity)` | 更新，更新所有字段 |
| `int updateByQuery(E entity, Query query)` | 根据条件更新 |
| `int updateIgnoreNull(E entity)` | 更新，忽略null字段 |
| `int updateByMap(Map<String, Object> map, Query query)` | 根据条件更新，map中的数据转化成update语句set部分，key为数据库字段名 |
| `int delete(E entity)` | 删除，在有逻辑删除字段的情况下，做UPDATE操作 |
| `int deleteByColumn(String column, Object value)` | 根据指定字段值删除，在有逻辑删除字段的情况下，做UPDATE操作 |
| `int deleteById(I id)` | 根据id删除，在有逻辑删除字段的情况下，做UPDATE操作 |
| `int deleteByIds(Collection<I> ids)` | 根据多个主键id删除，在有逻辑删除字段的情况下，做UPDATE操作 |
| `int deleteByQuery(Query query)` | 根据条件删除，在有逻辑删除字段的情况下，做UPDATE操作 |
| `int forceDelete(E entity)` | 强制删除（底层根据id删除），忽略逻辑删除字段，执行DELETE语句 |
| `int forceDeleteById(I id)` | 根据id强制删除，忽略逻辑删除字段，执行DELETE语句 |
| `int forceDeleteByQuery(Query query)` | 根据条件强制删除，忽略逻辑删除字段，执行DELETE语句 |

<a name="k60YJ"></a>
## Query查询对象
<a name="GrDdG"></a>
### 查询姓名为张三，并且年龄为22岁的用户：
```java
Query query = new Query().eq("username","张三").eq("age",22);
List<User> users = mapper.list(query);
```
<a name="YctZw"></a>
### 查询年龄为10,20,30的用户：
```java
Query query = new Query().in("age",Arrays.asList(10,20,30));
List<User> users = mapper.list(query);
```
<a name="kUbNM"></a>
### 查询注册日期大于2017-11-11的用户：
```java
Date regDate = ...
Query query = new Query().gt("reg_date",regDate);
List<User> users = mapper.list(query);
```
<a name="aPh9y"></a>
### 查询性别为男的，年龄大于等于20岁的用户，按年龄降序：
```java
Query query = new Query().eq("gender",1).ge("age",20).orderby("age",Sort.DESC);
List<User> users = mapper.list(query);
```
<a name="Wz9vi"></a>
### 分页查询：
```java
Query query = new Query().eq("age",10).page(1,10); // 第一页，每页10条数据
List<User> users = mapper.list(query);
```
<a name="QQhwj"></a>
### 查询总记录数：
```java
Query query = new Query().eq("age",10).page(1,10); // 第一页，每页10条数据
long total = mapper.getCount(query); // 该条件下总记录数
```
