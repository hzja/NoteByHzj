Mybatis
<a name="kWD8k"></a>
## Mybatis内部参数设置和结果的处理
先来看一个案例：<br />jdbc的方式插入用户信息，参数为UserModel：
```java
public class UserModel {
    private Integer id;
    private String name;
    private Integer age;
    //省略了get set方法
}

public static int insert(UserModel userModel) throws SQLException {
    Connection connection = null;
    String sql = "insert into t_user (id,name,age) values (?,?,?)";
    PreparedStatement preparedStatement = connection.prepareStatement(sql);
    preparedStatement.setInt(1,userModel.getId());
    preparedStatement.setString(2,userModel.getName());
    preparedStatement.setInt(3,userModel.getAge());
    return preparedStatement.executeUpdate();
}
```
上面调用了`preparedStatement`的几个方法来设置参数，如下：
```java
preparedStatement.setInt(1,userModel.getId());
preparedStatement.setString(2,userModel.getName());
preparedStatement.setInt(3,userModel.getAge());
```
当使用Mybatis插入用户信息的时候，Mybatis底层也会调用`PreparedStatement`的这些设置参数的方法，Mybatis底层是如何判断调用哪个方法的呢？<br />是调用`setInt`方法还是`setString`方法的呢？<br />再来看一个jdbc查询的案例，查询出t_user表所有数据，将其转换为UserModel对象，核心代码如下：
```java
public static List<UserModel> getList() throws Exception {
    List<UserModel> result = new ArrayList<>();
    Connection connection = null;
    String sql = "select id,name,age from t_user";
    PreparedStatement preparedStatement = connection.prepareStatement(sql);
    ResultSet rs = preparedStatement.executeQuery();
    while (rs.next()) {
        UserModel userModel = new UserModel();
        userModel.setId(rs.getInt(1));
        userModel.setName(rs.getString(2));
        userModel.setAge(rs.getInt(3));
        result.add(userModel);
    }
    return result;
}
```
上面有几行从ResultSet获取数据的方法，如下：
```java
userModel.setId(rs.getInt(1));
userModel.setName(rs.getString(2));
userModel.setAge(rs.getInt(3));
```
如果使用Mybatis实现，Mybatis可以将sql结果自动映射到UserModel中的属性中，Mybatis内部给UserModel的id设置值的时候，Mybatis内部是如何知道是调用rs.getInt还是调用`rs.getString`来获取id列的值的呢？<br />这些就是Mybatis中类型转换器做的事情，类型转换器主要有2个功能：

1. 参数设置，即设置参数的时候需要调用`PreparedStatement`中哪个set方法去设置参数，比如：插入用户信息id字段，id字段是Java中的Integer类型的，Mybatis内部知道需要调用`setInt`方法去给id字段设置参数。
2. 将sql查询结果转换为对应的Java类型，即调用`ResultSet`中的哪个get方法去获取参数，比如：id在数据中是int类型的，读取的时候会调用`ResultSet`的getInt方法去读取，而name字段在db中是varchar类型的，读取的时候会调用`getString`方法去读取，而不是调用`getInt`方法去读取。

Mybatis中定义了一个类型转换器的接口：
```java
public interface TypeHandler<T> {

    void setParameter(PreparedStatement ps, int i, T parameter, JdbcType jdbcType) throws SQLException;

    /**
	 * @param columnName Colunm name, when configuration <code>useColumnLabel</code> is <code>false</code>
 	 */
    T getResult(ResultSet rs, String columnName) throws SQLException;

    T getResult(ResultSet rs, int columnIndex) throws SQLException;

    T getResult(CallableStatement cs, int columnIndex) throws SQLException;

}
```
第一个方法用于通过`PreparedStatement`设置参数的，即内部会根据参数的类型，去调用`PreparedStatement`中对应的方法去设置参数的值，比如是调用`setInt`方法呢还是`setString`方法，每个类型转换器中实现的是不同的。其他3个方法是从结果集中读取数据的，内部具体是调用结果集的`getInt`方法还是`getString`方法或者是`getObject`方法，每个转换器内部实现也是不一样的。<br />Mybatis内部默认实现了很多类型转换器，基本上够用了，比如`IntegerTypeHandler`转换器，`IntegerTypeHandler`主要用来处理java中的Integer、int类型参数的，所以会调用setInt方法设置参数；读取的时候，对应的jdbc中的类型是`JdbcType.INTEGER`，所以会调用getInt方法读取数据库返回的值，读取的结果是`Integer`类型的。看一下其源码：
```java
public class IntegerTypeHandler extends BaseTypeHandler<Integer> {

    @Override
    public void setNonNullParameter(PreparedStatement ps, int i, Integer parameter, JdbcType jdbcType)
    throws SQLException {
        ps.setInt(i, parameter);
    }

    @Override
    public Integer getNullableResult(ResultSet rs, String columnName)
    throws SQLException {
        int result = rs.getInt(columnName);
        return result == 0 && rs.wasNull() ? null : result;
    }

    @Override
    public Integer getNullableResult(ResultSet rs, int columnIndex)
    throws SQLException {
        int result = rs.getInt(columnIndex);
        return result == 0 && rs.wasNull() ? null : result;
    }

    @Override
    public Integer getNullableResult(CallableStatement cs, int columnIndex)
    throws SQLException {
        int result = cs.getInt(columnIndex);
        return result == 0 && cs.wasNull() ? null : result;
    }
}
```
Mybatis内部默认实现了很多类型转换器，每种类型转换器能够处理哪些java类型以及能够处理的JdbcType的类型，这些都在`TypeHandlerRegistry`进行注册的，大家可以去看一下这个类源码，`IntegerTypeHandler`也是在这个类中进行注册的，代码如下：
```java
register(Integer.class, new IntegerTypeHandler());
register(int.class, new IntegerTypeHandler());
register(JdbcType.INTEGER, new IntegerTypeHandler());
```
所以当参数是Integer或者int类型的时候，Mybatis会调用`IntegerTypeHandler`转换器中的setInt方法去设置参数，上面UserModel的id字段是Integer类型的，所以插入数据的时候会调用`IntegerTypeHandler`处理器中的setInt方法去设置参数，当Mybatis将t_user表的数据自动映射为UserModel类型的时候，Mybatis会发现id类型是Integer类型，然后会找到`IntegerTypeHandler`中对应的读取结果集的方法去读取数据，即调用ResultSet的getInt方法读取id字段的数据，然后赋值给UserModel中的id属性。
<a name="Dlwsm"></a>
## 自定义类型转换器
在用户表新增一个性别字段sex，Java中通过一个枚举来表示这个类型，db脚本如下：
```sql
DROP DATABASE IF EXISTS `javacode2018`;
CREATE DATABASE `javacode2018`;

USE `javacode2018`;

DROP TABLE IF EXISTS t_user;
CREATE TABLE t_user(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '用户id',
  name VARCHAR(32) NOT NULL DEFAULT '' COMMENT '用户名',
  age SMALLINT NOT NULL DEFAULT 1 COMMENT '年龄',
  sex SMALLINT DEFAULT 0 COMMENT '性别，0：未知，1：男，2：女'
) COMMENT '用户表';
INSERT INTO t_user VALUES (1,'Java',30,1),(2,'林志玲',45,2);
```
t_user对应的Model如下：
```java
@Getter
@Setter
@Builder
@ToString
@NoArgsConstructor
@AllArgsConstructor
public class UserModel implements Serializable {
    private Integer id;
    private String name;
    private Integer age;
    private SexEnum sex;
}
```
sex是一个枚举类型，枚举定义如下：
```java
/**
 * 性别
 */
public enum SexEnum {
    UNKNOW(0, "未知"),
    MAN(1, "男"),
    WOMAN(2, "女");

    private Integer value;
    private String name;

    SexEnum(Integer value, String name) {
        this.value = value;
        this.name = name;
    }

    public Integer getValue() {
        return value;
    }

    public String getName() {
        return name;
    }

    public static SexEnum getByValue(Integer value) {
        for (SexEnum item : values()) {
            if (item.getValue().equals(value)) {
                return item;
            }
        }
        return null;
    }
}
```
来写一个查询如下：
```xml
<select id="getList1" resultType="com.javacode2018.chat05.demo10.model.UserModel">
  SELECT id,name,age,sex FROM t_user
</select>
```
查询t_user表，每行结果转换为一个UserModel对象，最后返回一个List集合，这个查询运行会不会有什么问题？<br />确实会报错，如下：
```
### Cause: org.apache.ibatis.executor.result.ResultMapException: Error attempting to get column 'sex' from result set.  Cause: java.lang.IllegalArgumentException: No enum constant com.javacode2018.chat05.demo10.enums.SexEnum.1

    at org.apache.ibatis.exceptions.ExceptionFactory.wrapException(ExceptionFactory.java:30)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectList(DefaultSqlSession.java:149)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectList(DefaultSqlSession.java:140)
    at org.apache.ibatis.binding.MapperMethod.executeForMany(MapperMethod.java:147)
    at org.apache.ibatis.binding.MapperMethod.execute(MapperMethod.java:80)
    at org.apache.ibatis.binding.MapperProxy.invoke(MapperProxy.java:93)
```
sex字段在t_user中是数字类型的，最后通过Mybatis查询，需要赋值给UserModel中的sex字段，这个字段是一个枚举类型的，Mybatis不知道这两者之间如何进行转换，所以报错了。<br />此时需要指定一个转换规则，来告知Mybatis如何进行转换，需要用到自定义类型转换器了。<br />自定义一个类型转换器`SexEnumTypeHandle`，用来处理sex字段和SexEnum枚举之间的相互转换，代码如下：
```java
package com.javacode2018.chat05.demo10.typehandle;

import com.javacode2018.chat05.demo10.enums.SexEnum;
import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.type.BaseTypeHandler;
import org.apache.ibatis.type.JdbcType;

import java.sql.CallableStatement;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

@Slf4j
public class SexEnumTypeHandle extends BaseTypeHandler<SexEnum> {
    @Override
    public void setNonNullParameter(PreparedStatement ps, int i, SexEnum parameter, JdbcType jdbcType) throws SQLException {
        ps.setInt(i, parameter.getValue());
        log.info("{}", parameter);
    }

    @Override
    public SexEnum getNullableResult(ResultSet rs, String columnName) throws SQLException {
        log.info("{}", columnName);
        Object object = rs.getObject(columnName);
        Integer sex = object != null && object instanceof Integer ? (Integer) object : null;
        return SexEnum.getByValue(sex);
    }

    @Override
    public SexEnum getNullableResult(ResultSet rs, int columnIndex) throws SQLException {
        log.info("{}", columnIndex);
        Object object = rs.getObject(columnIndex);
        Integer sex = object != null && object instanceof Integer ? (Integer) object : null;
        return SexEnum.getByValue(sex);
    }

    @Override
    public SexEnum getNullableResult(CallableStatement cs, int columnIndex) throws SQLException {
        log.info("{}", columnIndex);
        Object object = cs.getObject(columnIndex);
        Integer sex = object != null && object instanceof Integer ? (Integer) object : null;
        return SexEnum.getByValue(sex);
    }
}
```
自定义类型转换器可以直接继承`BaseTypeHandler`，后面有个泛型，泛型类型为需要处理的java类型，即SexEnum。<br />类型转换器定义好了，需要将其注册到Mybatis中，需要在Mybatis配置文件中加入：
```xml
<typeHandlers>
  <typeHandler handler="com.javacode2018.chat05.demo10.typehandle.SexEnumTypeHandle"/>
</typeHandlers>
```
再次运行一下上面的查询，对应测试用例：
```java
@Test
public void getModelList() throws IOException {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        Map<String, Object> paramMap = new HashMap<>();
        List<UserModel> userModelList = mapper.getList1(paramMap);
        log.info("{}", userModelList);
    }
}
```
运行结果：
```
26:26.144 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age,sex FROM t_user 
26:26.167 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
26:26.187 [main] INFO  c.j.c.d.typehandle.SexEnumTypeHandle - sex
26:26.189 [main] INFO  c.j.c.d.typehandle.SexEnumTypeHandle - sex
26:26.189 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 2
26:26.190 [main] INFO  c.j.chat05.demo10.Demo10Test - [UserModel(id=1, name=Java, age=30, sex=MAN), UserModel(id=2, name=林志玲, age=45, sex=WOMAN)]
```
这下正常了，看一下输出中的sex属性，对应的为枚举的名称。<br />在此案例代码的基础上，再来一个案例，向用户表插入用户信息，参数为一个map，每个字段的值丢在map中，key为字段的名称，sex传递SexEnum枚举类型。<br />对应的Java测试用例代码如下：
```java
@Test
public void insert1() throws IOException {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        Map<String, Object> map = new HashMap<>();
        Integer id = Integer.valueOf(Calendar.getInstance().getTime().getTime() / 1000 + "");
        map.put("id", id);
        map.put("name", id.toString());
        map.put("age", 30);
        map.put("sex", SexEnum.WOMAN);
        int result = mapper.insert1(map);
        log.info("{}", result);
    }
}
```
对应mapper xml按照下面这种写法，如：
```xml
<insert id="insert1" parameterType="map">
  insert into t_user (id,name,age,sex)
  VALUE (#{id},#{name},#{age},#{sex})
</insert>
```
这么写会不会有问题？来运行一下：
```
32:36.343 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - ==>  Preparing: insert into t_user (id,name,age,sex) VALUE (?,?,?,?) 
32:36.378 [main] INFO  c.j.c.d.typehandle.SexEnumTypeHandle - WOMAN
32:36.381 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - ==> Parameters: 1578889956(Integer), 1578889956(String), 30(Integer), 2(Integer)
32:36.387 [main] DEBUG c.j.c.d.mapper.UserMapper.insert1 - <==    Updates: 1
32:36.387 [main] INFO  c.j.chat05.demo10.Demo10Test - 1
```
插入成功了，map中sex对应的是`SexEnum.WOMAN`，Mybatis中判断map中sex对应的是SexEnum类型的，所以会找到其对应的类型转换器`SexEnumTypeHandle`，注意上面日志中的第二行，这个就是`SexEnumTypeHandle`转换器中输出的日志，说明设置sex参数的时候执行了`SexEnumTypeHandle`中的代码。<br />但是当不传递sex的值的时候，或者说sex传递为null的时候，此时Mybatis是无法判断map中sex对应的具体类型的，Mybatis内部将无法判断sex参数的设置的时候，无法找到具体用哪个类型转换器给sex设置参数，这个在MySQL中不会报错，但是在Oracle中会报错，有Oracle环境的，可以去试试下面代码运行结果：
```java
@Test
public void insert1() throws IOException {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        Map<String, Object> map = new HashMap<>();
        Integer id = Integer.valueOf(Calendar.getInstance().getTime().getTime() / 1000 + "");
        map.put("id", id);
        map.put("name", id.toString());
        map.put("age", 30);
        map.put("sex", null);
        int result = mapper.insert1(map);
        log.info("{}", result);
    }
}
```
上面代码Oracle中执行会报错，需要修改对应的mapper xml中的代码，需要让Mybatis明确知道sex字段需要用哪个类型转换器进行处理，有2种写法。<br />第一种，通过`typeHandler`属性明确指定参数的类型转换器，如下
```xml
<insert id="insert1" parameterType="map">
  insert into t_user (id,name,age,sex)
  VALUE (#{id},#{name},#{age},#{sex,typeHandler=com.javacode2018.chat05.demo10.typehandle.SexEnumTypeHandle})
</insert>
```
第二种，可以指定sex字段的类型，需要通过`javaType`属性来指定参数的具体类型，如下：
```xml
<insert id="insert1" parameterType="map">
  insert into t_user (id,name,age,sex)
  VALUE (#{id},#{name},#{age},#{sex,javaType=com.javacode2018.chat05.demo10.enums.SexEnum})
</insert>
```
Mybatis通过Java的类型就可以找到对应的类型转换器，所以方式2也是正常的。<br />**再来总结一下自定义类型转换器的使用步骤：**

1. **自定义类型转换器，继承**`**BaseTypeHandler**`
2. **将自定义类型转换器注册到Mybatis中，需要在Mybatis配置文件中通过**`**typeHandler**`**元素进行引入**
3. **mapper xml中就可以使用了。**
