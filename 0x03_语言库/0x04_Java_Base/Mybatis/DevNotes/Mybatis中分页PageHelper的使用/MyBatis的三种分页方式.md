JavaMybatis
<a name="cdv00"></a>
### 前言
分页是在开发中绕不过去的一个坎！当数据量大了的时候，一次性将所有数据查出来不现实，所以一般都是分页查询的，减轻服务端的压力，提升了速度和效率！也减轻了前端渲染的压力！<br />注意：由于 java 允许的最大整数为 2147483647，所以 limit 能使用的最大整数也是 2147483647，一次性取出大量数据可能引起内存溢出，所以在大数据查询场合慎重使用！
<a name="M77dK"></a>
### 一、Limit分页
语法：
```java
limit ${startPos},${pageSize}
```
在实际项目中一般会加上为空为null判断，如下：
```xml
<if test="startPos!=null and pageSize!=null">
	limit ${startPos},${pageSize}
</if>
```
业务层代码：
```xml
<select id="getUserInfo1" parameterType="map" resultType="dayu">
    select * from user
    <if test="startPos!=null and pageSize!=null">
        limit ${startPos},${pageSize}
    </if>
</select>
```
```java
List<User> getUserInfo1(Map<String,Object> map);
```
```java
@Test
 public void selectUser() {
     SqlSession session = MybatisUtils.getSession();
     UserMapper mapper = session.getMapper(UserMapper.class);
     //这里塞值
      Map<String,Object> parms = new HashMap<>();
      parms.put("startPos","0");
      parms.put("pageSize","5");
     List<User> users = mapper.getUserInfo1(parms);
     for (User map: users){
         System.out.println(map);
    }
     session.close();
}
```
执行结果：<br />![2022-05-29-21-10-58-116383.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653829915273-64aa483d-987c-4a1c-9100-060f019a6ce0.png#clientId=ub328c593-b707-4&from=ui&id=u7201b2aa&originHeight=357&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1158988&status=done&style=none&taskId=u45940133-ba02-4b8a-bdb6-1342cd75487&title=)<br />传入0，10时：<br />![2022-05-29-21-10-58-211917.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653829915315-929dc9d8-deb9-43ce-a777-8c6a76a78e54.png#clientId=ub328c593-b707-4&from=ui&id=zBzae&originHeight=387&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1256377&status=done&style=none&taskId=u2edc15dc-ff76-4527-8725-cea444514fd&title=)<br />总结：

- limit 0,10;
- 0 代表从第0条数据开始
- 10 代表查10条数据
- 等到第二页的时候就是 limit 10,10;

以此类推！<br />这些内容其实就时MySQL中的内容，不作再详细讲解了。
<a name="oFTuV"></a>
### 二、RowBounds分页（不推荐使用）
RowBounds省略了limit的内容，只需要在业务层关注分页即可！无须再传入指定数据！<br />但是，这个属于逻辑分页，即实际上sql查询的是所有的数据，在业务层进行了分页而已，比较占用内存，而且数据更新不及时，可能会有一定的滞后性！不推荐使用！<br />RowBounds对象有2个属性，offset和limit。

- offset：起始行数
- limit：需要的数据行数

因此，取出来的数据就是：从第offset+1行开始，取limit行<br />业务层代码：
```java
@Test
public void selectUserRowBounds() {
    SqlSession session = MybatisUtils.getSession();
    UserMapper mapper = session.getMapper(UserMapper.class);
    // List<User> users = session.selectList("com.dy.mapper.UserMapper.getUserInfoRowBounds",null,new RowBounds(0, 5));
    List<User> users = mapper.getUserInfoRowBounds(new RowBounds(0,5));
    for (User map: users){
        System.out.println(map);
    }
    session.close();
}
```
```java
List<User> getUserInfoRowBounds(RowBounds rowBounds);
```
```xml
<select id="getUserInfoRowBounds" resultType="dayu">
   select * from user
</select>
```
执行查看结果：<br />![2022-05-29-21-10-58-273165.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653829935829-3fa7fa14-c591-45a4-8e17-8e400dcbd7a3.png#clientId=ub328c593-b707-4&from=ui&id=u06a31d9f&originHeight=354&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1149253&status=done&style=none&taskId=u7af2d915-867f-44b9-b514-d46947541ef&title=)
<a name="iecZj"></a>
### 三、Mybatis_PageHelper分页插件
官方GitHub地址：[https://github.com/pagehelper/Mybatis-PageHelper](https://github.com/pagehelper/Mybatis-PageHelper)<br />引入jar包
```xml
<dependency>
   <groupId>com.github.pagehelper</groupId>
   <artifactId>pagehelper</artifactId>
   <version>5.1.7</version>
</dependency>
```
配置MyBatis核心配置文件
```xml
<plugins>
    <plugin interceptor="com.github.pagehelper.PageInterceptor" />
</plugins>
```
业务层代码
```java
@Test
public void selectUserPageHelper() {
    SqlSession session = MybatisUtils.getSession();
    UserMapper mapper = session.getMapper(UserMapper.class);
    //第二种，Mapper接口方式的调用，推荐这种使用方式。
    PageHelper.startPage(1, 3);
    List<User> list = mapper.getUserInfo();
    //用PageInfo将包装起来
    PageInfo page = new PageInfo(list);
    for (User map: list){
        System.out.println(map);
    }
    System.out.println("page:---"+page);
    session.close();
}
```
执行结果<br />![2022-05-29-21-10-58-336684.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653829935805-1758b074-6bb3-4acb-9f2f-2b7b5c50c447.png#clientId=ub328c593-b707-4&from=ui&id=skyUj&originHeight=272&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=883050&status=done&style=none&taskId=u7c024a97-6c94-432b-a7c2-dfdd6ceea67&title=)<br />总结：<br />PageHelper还是很好用的，也是物理分页！<br />实际上一般用第二种比较多：Mapper接口方式的调用
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
//存在以下 Mapper 接口方法，不需要在 xml 处理后两个参数
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
//存在以下 Mapper 接口方法，不需要在 xml 处理后两个参数
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
拓展
```java
//获取第1页，10条内容，默认查询总数count
PageHelper.startPage(1, 10);
List<User> list = userMapper.selectAll();
//用PageInfo对结果进行包装
PageInfo page = new PageInfo(list);
//测试PageInfo全部属性
//PageInfo包含了非常全面的分页属性
assertEquals(1, page.getPageNum());
assertEquals(10, page.getPageSize());
assertEquals(1, page.getStartRow());
assertEquals(10, page.getEndRow());
assertEquals(183, page.getTotal());
assertEquals(19, page.getPages());
assertEquals(1, page.getFirstPage());
assertEquals(8, page.getLastPage());
assertEquals(true, page.isFirstPage());
assertEquals(false, page.isLastPage());
assertEquals(false, page.isHasPreviousPage());
assertEquals(true, page.isHasNextPage());
```
这种方式十分方便快捷好用！推荐使用！
