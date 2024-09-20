JavaMybatis
<a name="HNVSc"></a>
## Mybatis中实体类属性和数据列之间映射的四种办法
Mybatis不像Hibernate中那么自动化，通过`@Column`注解或者直接使用实体类的属性名作为数据列名，而是需要自己指定实体类属性和 数据表中列名之间的映射关系，这一点让用惯了Hibernate的人很不习惯，所幸经过探索找到了建立映射关系的三种办法，其中总也有比较简单的。<br />首先先定义一个实体类，如下：
```java
public class User implements Serializable {
    private Integer userId;
    private String userName;
    ......
}
```
<a name="ylYa6"></a>
## 1、通过XML映射文件中的resultMap
这种方式是最常见的，类似如下：
```xml
<mapper namespace="data.UserMapper">
  <resultMap type="data.User" id="userResultMap">
    <!-- 用id属性来映射主键字段 -->
    <id property="id" column="user_id"/>
    <!-- 用result属性来映射非主键字段 -->
    <result property="userName" column="user_name"/>
  </resultMap>
</mapper>
```
通过里面的id标签和result标签来建立映射关系，由property和column分别指定实体类属性和数据表的列名。
<a name="vKw65"></a>
## 2、通过注解`@Results`和`@Result`
这两个注解是与XML文件中的标签相对应的：

- `@Results`对应resultMap
- `@Result`对应result

这两个注解是**「应用在方法的级别」** 上的，也就是在mapper方法上，如下：
```java
@Select("select * from t_user where user_name = #{userName}")
@Results(
        @Result(property = "userId", column = "user_id"),
        @Result(property = "userName", column = "user_name")
)
User getUserByName(@Param("userName") String userName);
```
缺点：

- 由于注解是针对方法的，对于Mapper中的每个操作数据库的方法都必须有相同的注解完成映射关系的建立，导致很多的配置是重复的；
- 如果要避免配置重复的问题，可以采用在XML配置文件中配置这个`resultMap`，然后再`@Result`中通过id属性引用这个`resultMap`， 但是这样感觉很麻烦（由于使用了两种配置方式），不如直接使用基于XML的`resultMap`配置方式；
<a name="CXI4p"></a>
## 3、通过属性配置完成映射
使用者最陌生的是通过配置属性来完成映射，Mybatis给提供了一种映射方式，如果属性的命名是遵从驼峰命名法的，数据列名遵从下划线命名， 那么可以使用这种方式，类似如下：

- userName对应user_name;
- userId对应user_id;

配置代码如下：
```java
SqlSessionFactoryBean sqlSessionFactoryBean = new SqlSessionFactoryBean();
Configuration configuration = new Configuration();
configuration.setMapUnderscoreToCamelCase(true);
sqlSessionFactoryBean.setConfiguration(configuration);
```
<a name="nqhwf"></a>
## 4、通过使用在SQL语句中定义别名完成映射
这种方式最直接，直接在SQL语句中建立别名来完成映射，如下：
```java
@Select("select user_name as userName, user_id as userId from t_user where user_name = #{userName}")
User getUserByName(@Param("userName") String userName);
```
