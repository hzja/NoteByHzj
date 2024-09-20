Mybatis
<a name="ejXe6"></a>
## 1、简介
fastmybatis是一个mybatis开发框架，其宗旨为：简单、快速、有效。

- 零配置快速上手
- 无需编写xml文件即可完成CRUD操作
- 支持mysql，sqlserver，oracle，postgresql，sqlite
- 支持自定义sql，sql语句可写在注解中或xml中
- 支持与spring-boot集成，依赖starter即可
- 轻量级，无侵入性，是官方mybatis的一种扩展
<a name="TqQ0P"></a>
## 2、快速开始（SpringBoot）

- 新建一个springboot项目
- pom.xml添加fastmybatis-spring-boot-starter
```xml
<dependency>
    <groupId>net.oschina.durcframework</groupId>
    <artifactId>fastmybatis-spring-boot-starter</artifactId>
    <version>最新版本（见changelog.md）</version>
</dependency>
```

- 假设数据库有张t_user表，添加对应的实体类TUser.java和MapperTUserMapper.java(可用fastmybatis-generator来生成)
- 在application.propertis中配置数据库连接
- 编写测试用例
```java
@Autowired
TUserMapper mapper;
// 根据主键查询
@Test
public void testGetById() {
    TUser user = mapper.getById(3);
    System.out.println(user);
}
```
<a name="xmqG4"></a>
## 3、查询
主要讲解fastmybatis的查询功能。fastmybatis提供丰富的查询方式，满足日常查询所需。
<a name="iyiwo"></a>
### 分页查询
<a name="MwYFj"></a>
#### 方式1
前端传递两个分页参数pageIndex，pageSize
```java
// http://localhost:8080/page1?pageIndex=1&pageSize=10
@GetMapping("page1")
public List<TUser> page1(int pageIndex,int pageSize) {
    Query query = new Query();
    query.page(pageIndex, pageSize);
    List<TUser> list = mapper.list(query);
    return list;
}
```
<a name="kJJoM"></a>
#### 方式2
PageParam里面封装了pageIndex，pageSize参数
```java
// http://localhost:8080/page2?pageIndex=1&pageSize=10
@GetMapping("page2")
public List<TUser> page2(PageParam param) {
    Query query = param.toQuery();
    List<TUser> list = mapper.list(query);
    return list;
}
```
<a name="TnMRS"></a>
#### 返回结果集和总记录数
方式1和方式2只能查询结果集，通常查询还需返回记录总数并返回给前端，fastmybatis的处理方式如下：
```java
// http://localhost:8080/page3?pageIndex=1&pageSize=10
@GetMapping("page3")
public Map<String,Object> page3(PageParam param) {
    Query query = param.toQuery();
    List<TUser> list = mapper.list(query);
    long total = mapper.getCount(query);
    Map<String,Object> result = new HashMap<String, Object>();
    result.put("list", list);
    result.put("total", total);
    return result;
}
```
fastmybatis提供一种更简洁的方式来处理：
```java
// http://localhost:8080/page4?pageIndex=1&pageSize=10
@GetMapping("page4")
public PageInfo<TUser> page4(PageParam param) {
    PageInfo<TUser> pageInfo = MapperUtil.query(mapper, query);
    return result;
}
```
PageInfo里面包含了List，total信息，还包含了一些额外信息，完整数据如下：
```json
{
	"currentPageIndex": 1, // 当前页
	"firstPageIndex": 1, // 首页
	"lastPageIndex": 2, // 尾页
	"list": [ // 结果集
		{},
		{}
	],
	"nextPageIndex": 2, // 下一页
	"pageCount": 2, // 总页数
	"pageIndex": 1, // 当前页
	"pageSize": 10, // 每页记录数
	"prePageIndex": 1, // 上一页
	"start": 0,
	"total": 20 // 总记录数
}
```
<a name="un7Qj"></a>
### 根据参数字段查询
<a name="M97C7"></a>
#### 查询钱最多的前三名
```java
// http://localhost:8080/sch4
@GetMapping("sch4")
public List<TUser> sch4() {
    Query query = new Query();
    query.orderby("money", Sort.DESC) // 按金额降序
        .page(1, 3);
    List<TUser> list = mapper.list(query);
    return list;
}
```
<a name="rmnfK"></a>
#### 将参数放在对象中查询
```java
// http://localhost:8080/sch5?username=张三
@GetMapping("sch5")
public List<TUser> sch5(UserParam userParam) {
    Query query = userParam.toQuery();
    query.eq("username", userParam.getUsername());
    List<TUser> list = mapper.list(query);
    return list;
}
```
UserParam继承PageSortParam类，表示支持分页和排序查询
<a name="T4Sxt"></a>
#### 使用普通bean查询
假设有个User类如下
```java
public class User {
    private Integer id;
    private String userName;
    public Integer getId() {
        return id;
    }
    public void setId(Integer id) {
        this.id = id;
    }
    public String getUserName() {
        return userName;
    }
    public void setUserName(String userName) {
        this.userName = userName;
    }
}
```
将这个类作为查询参数，那么在SpringMVC中可以这样写:
```java
@GetMapping(path="findUserBean.do")
public List<User> findUser(User user) {
    Query query = Query.build(user);
    List<User> list = dao.find(query);
    return list;
}
```
`Query query = Query.build(user);`这句是将User中的属性转换成对应条件，假设userName的值为“jim”，那么会封装成一个条件`where user_name='jim'`<br />浏览器输入链接：http://localhost:8080/fastmybatis-springmvc/findUserBean.do?userName=jim<br />后台将会执行如下SQL:
```sql
SELECT id,user_name FROM user t WHERE t.user_name = ?
```
?的值为jim
<a name="RECoX"></a>
#### `@Condition`注解
`@Condition`注解用来强化查询，有了这个注解可以生成各种查询条件。<br />`@Condition`注解有三个属性：

- joint：表达式之间的连接符，AND|OR，默认AND
- column：数据库字段名，可选
- operator：连接符枚举，存放了等于、大于、小于等连接符

如果要查询id大于2的用户只需在get方法上加上一个`@Condition`注解即可:
```java
@Condition(operator=Operator.gt)
public Integer getId() {
    return this.id;
}
```
这样，当id有值时，会封装成一个where id>2的条件

- 需要注意的是，如果不指定column属性，系统会默认取get方法中属性名，然后转换成数据库字段名。如果需要指定数据库字段名的话，可以使用`@Condition`的column属性。
```java
public Integer get++UserName++() {
    return this.userName;
}
```
这种情况下会取下划线部分字段，然后转换成数据库字段名。
```java
@Condition(column="username") // 显示指定字段名
public Integer getUserName() {
    return this.userName;
}
```
使用`@Condition`可以生产更加灵活的条件查询，比如需要查询日期为2017-12-1~2017-12-10日的记录，可以这样写:
```java
@Condition(column="add_date",operator=Operator.ge)
public Date getStartDate() {
    return this.startDate;
}
@Condition(column="add_date",operator=Operator.lt)
public Date getEndDate() {
    return this.endDate;
}
```
转换成SQL语句：
```sql
t.add_date>='2017-12-1' AND t.add_date<'2017-12-10'
```
<a name="fg0ep"></a>
### IN查询
假设前端页面传来多个值比如checkbox勾选多个`id=[1,2]`，那么在User类里面可以用`Integer[]`或List来接收。
```java
private Integer[] idArr;
public void setIdArr(Integer[] idArr) {this.idArr = idArr;}
@Condition(column="id")
public Integer[] getIdArr() {return this.idArr;}
```
这样会生成`where id IN(1,2)`条件。
<a name="oqOhW"></a>
### 排序查询
```java
// 根据添加时间倒序
Query query = new Query();
query.orderby("create_time",Sort.DESC);
dao.find(query);
```
<a name="D8Lj9"></a>
### 多表关联查询
多表关联查询使用的地方很多，比如需要关联第二张表，获取第二张表的几个字段，然后返回给前端。<br />fastmybatis的用法如下：<br />假如需要关联第二张表user_info，筛选出user_info中的城市为杭州的数据。
```java
Query query = new Query()
        // 左连接查询,主表的alias默认为t
        .join("LEFT JOIN user_info t2 ON t.id = t2.user_id").page(1, 5)
        .eq("t2.city","杭州");
List<TUser> list = mapper.list(query);
System.out.println("==============");
for (TUser user : list) {
    System.out.println(user.getId() + " " + user.getUsername());
}
System.out.println("==============");
```
<a name="TvvfT"></a>
#### 多表关联返回指定字段
有时候不需要全部字段，需要取表1中的几个字段，然后取表2中的几个字段，fastmybatis实现方式如下：
```java
Query query = new Query();
// 左连接查询,主表的alias默认为t
query.join("LEFT JOIN user_info t2 ON t.id = t2.user_id");
// 指定返回字段
List<String> column = Arrays.asList("t2.user_id as userId", "t.username", "t2.city");
// 查询结果返回到map中
List<Map<String, Object>> mapList = mapper.listMap(column, query);
// 再将map转换成实体bean
List<UserInfoVo> list = MyBeanUtil.mapListToObjList(mapList, UserInfoVo.class);
```
执行的SQL语句对应如下：
```sql
SELECT t2.user_id as userId , t.username , t2.city
FROM `t_user` t
LEFT JOIN user_info t2 ON t.id = t2.user_id
```
<a name="HTa6q"></a>
### 使用`@Select`查询
`@Select`注解是mybatis官方提供的一个功能，fastmybatis可以理解为是官方的一种扩展，因此同样支持此功能。<br />在Mapper中添加如下代码：
```java
@Select("select * from t_user where id=#{id}")
TUser selectById(@Param("id") int id);
```
编写测试用例
```java
@Test
public void testSelectById() {
    TUser user = dao.selectById(3);
    System.out.println(user.getUsername());
}
```
对于简单的SQL，可以用这种方式实现。除了@Select之外，还有@Update，@Insert，@Delete，这里就不多做演示了。
<a name="URzNb"></a>
### Query类详解
Query是一个查询参数类，配合Mapper一起使用。
<a name="tjlaP"></a>
#### 参数介绍
Query里面封装了一系列查询参数，主要分为以下几类：

- 分页参数：设置分页
- 排序参数：设置排序字段
- 条件参数：设置查询条件
- 字段参数：可返回指定字段

下面逐个讲解每个参数的用法。
<a name="uVtWM"></a>
#### 分页参数
一般来说分页的使用比较简单，通常是两个参数，

1. pageIndex：当前页索引，
2. pageSize：每页几条数据。

Query类使用`page(pageIdnex, pageSize)`方法来设置。<br />假如要查询第二页，每页10条数据，代码可以这样写：
```java
Query query = new Query();
query.page(2, 10);
List<User> list = dao.find(query);
```
如果要实现不规则分页，可以这样写：
```java
Query query = new Query();
query.limit(3, 5) // 对应mysql：limit 3,5
```
<a name="vkDkx"></a>
#### 排序参数
```java
orderby(String sortname, Sort sort)
```
其中sortname为数据库字段，非javaBean属性<br />- orderby(String sortname, Sort sort)则可以指定排序方式，Sort为排序方式枚举<br />假如要按照添加时间倒序，可以这样写：
```java
Query query = new Query();
query.orderby("create_time",Sort.DESC);
mapper.list(query);
```
添加多个排序字段可以在后面追加：
```java
query.orderby("create_time",Sort.DESC).orderby("id",Sort.ASC);
```
<a name="yKlAG"></a>
#### 条件参数
条件参数是用的最多一个，因为在查询中往往需要加入各种条件。<br />fastmybatis在条件查询上面做了一些封装，这里不做太多讲解，这里只说下基本的用法，感兴趣的同学可以自行查看源码，也不难理解。<br />条件参数使用非常简单，Query对象封装一系列常用条件查询。

- 等值查询`eq(String columnName, Object value)`，columnName为数据库字段名，value为查询的值

假设要查询姓名为张三的用户，可以这样写：
```java
Query query = new Query();
query.eq("username","张三");
List<User> list = mapper.list(query);
```
通过方法名即可知道eq表示等于‘=’，同理lt表示小于<，gt表示大于>

| 查询方式 | 说明 |
| --- | --- |
| eq | 等于= |
| gt | 大于> |
| lt | 小于< |
| ge | 大于等于>= |
| le | 小于等于<= |
| notEq | 不等于<> |
| like | 模糊查询 |
| in | `in()`查询 |
| notIn | `not in()`查询 |
| isNull | NULL值查询 |
| notNull | Is NOT NULL |
| notEmpty | 字段不为空，非NULL且有内容 |
| isEmpty | 字段为NULL或者为" |

如果上述方法还不能满足查询需求的话，可以使用自定sql的方式来编写查询条件，方法为：
```java
Query query = new Query();
query.sql(" username='Jim' OR username='Tom'");
```
注意：`sql()`方法不会处理sql注入问题，因此尽量少用。
<a name="NUN2P"></a>
### 自定义SQL

- 方式1

直接写在Mapper.java中
```java
public interface TUserMapper extends CrudMapper<TUser, Integer> {
    // 自定义sql，官方自带，不需要写xml
    /**
     * 修改用户名
     * @param id
     * @param username
     * @return 返回影响行数
     */
    @Update("update t_user set username = #{username} where id = #{id}")
    int updateById(@Param("id") int id, @Param("username") String username);
}
```
简单SQL可采用这种形式。

- 方式2

fastmybatis提供的Mapper已经满足大部分的操作需求，但是有些复杂的sql语句还是需要写在xml文件中。fastmybatis同样支持将sql语句写在xml中，具体配置如下：

- 在application.properties添加一句，指定xml文件存放路径
```
mybatis.mapper-locations=classpath:/mybatis/mapper/*.xml
```

- 在resources/mybatis/mapper目录下新建一个xml文件TUserMapper.xml，内容如下：
```xml
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.mayapp.mapper.TUserMapper">
    <select id="selectByName" parameterType="String" resultMap="baseResultMap">
        select * from t_user t where t.username = #{username} limit 1
    </select>
</mapper>
```
这个xml文件跟其它的mybatis配置文件一样，baseResultMap没有看到定义，但是确实存在，因为这个是fastmybatis提供的一个内置resultMap。

- 在TUseroMapper.java中添加：
```java
TUser selectByName(@Param("username")String username);
```

- 编写单元测试用例
```java
@Test
public void testSelectByName() {
    TUser user = dao.selectByName("张三");
    System.out.println(user.getUsername());
}
```
多文件同一个namespace<br />在以往的开发过程中，一个Mapper对应一个xml文件（namespace）。如果多人同时在一个xml中写SQL的话会造成各种冲突（虽然能够最终被解决）。<br />fastmybatis打破这种常规，允许不同的xml文件定义相同的namespace，程序启动时会自动把他们的内容合并到同一个文件当中去。

- 张三的UserMapper_zs.xml
```xml
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.mayapp.mapper.TUserMapper">
    <select id="selectByName" parameterType="String" resultMap="baseResultMap">
        select * from t_user t where t.username = #{username} limit 1
    </select>
</mapper>
```

- 李四的UserMapper_ls.xml
```xml
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.mayapp.mapper.TUserMapper">
    <select id="updateUser" parameterType="String" resultMap="baseResultMap">
        update t_user set username = #{username} where id=#{id}
    </select>
</mapper>
```
最终会合并成
```xml
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.mayapp.mapper.TUserMapper">
  <!-- 张三部分 -->
  <select id="selectByName" parameterType="String" resultMap="baseResultMap">
    select * from t_user t where t.username = #{username} limit 1
  </select>
  <!-- 李四部分 -->
  <select id="updateUser" parameterType="String" resultMap="baseResultMap">
    update t_user set username = #{username} where id=#{id}
  </select>
</mapper>
```
这样也体现了开闭原则，即新增一个功能只需要新增一个文件就行，不需要修改原来的文件。<br />如果SQL写多了还可以把它们进行分类，放到不同的xml中，便于管理。<br />注：合并动作是在启动时进行的，并不会生成一个真实的文件。
<a name="EV88M"></a>
## 4、最后
当然，还有其他更多的一些功能，大家感兴趣了，可以去项目开源地址学习，了解，这里不在过多的介绍。<br />项目开源地址：[https://gitee.com/durcframework/fastmybatis](https://gitee.com/durcframework/fastmybatis)
