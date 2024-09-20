Mybatis
<a name="GxRoy"></a>
## 建库建表
```sql
/*创建数据库javacode2018*/
DROP DATABASE IF EXISTS `javacode2018`;
CREATE DATABASE `javacode2018`;
USE `javacode2018`;

/*创建表结构*/
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE t_user (
  id BIGINT AUTO_INCREMENT PRIMARY KEY COMMENT '主键，用户id，自动增长',
  `name` VARCHAR(32) NOT NULL DEFAULT '' COMMENT '姓名',
  `age` SMALLINT NOT NULL DEFAULT 1 COMMENT '年龄',
  `salary` DECIMAL(12,2) NOT NULL DEFAULT 0 COMMENT '薪水',
  `sex` TINYINT NOT NULL DEFAULT 0 COMMENT '性别,0:未知,1:男,2:女'
) COMMENT '用户表';

SELECT * FROM t_user;
```
<a name="mtvEW"></a>
## 增删改返回值说明
Mybatis中对db执行增删改操作，不管是新增、删除、还是修改，最后都会去调用jdbc中对应的方法，要么是调用`java.sql.Statement`的`executeUpdate`的方法，要么是调用`java.sql.PreparedStatement`的`executeUpdate`方法，这2个类的方法名称都是`executeUpdate`，他们的参数可能不一样，但是他们的返回值都是int，说明增删改的返回值都是int类型的，表示影响的行数，比如插入成功1行返回结果就是1，删除了10行记录，返回就是10，更新了5行记录，返回的就是5。<br />那么通过Mybatis中的Mapper接口来对db增删改的时候，Mybatis的返回值支持哪些类型呢？<br />int类型那肯定是支持的，jdbc执行增删改默认返回int类型，那Mybatis当然也支持这个类型。<br />但是Mybatis的返回值比jdbc更强大，对于增删改还支持下面几种类型：
```
int
Integer
long 
Long
boolean
Boolean
void
```
mapper的增删改方法返回值必须为上面的类型，Mybatis内部将jdbc返回的int类型转换为上面列表中指定的类型，来看一下Mybatis这块的源码，源码在下面的方法中：
```
org.apache.ibatis.binding.MapperMethod#rowCountResult
```
来看一下这个方法的源码：
```java
private Object rowCountResult(int rowCount) {
    final Object result;
    if (method.returnsVoid()) {
      result = null;
    } else if (Integer.class.equals(method.getReturnType()) || Integer.TYPE.equals(method.getReturnType())) {
      result = rowCount;
    } else if (Long.class.equals(method.getReturnType()) || Long.TYPE.equals(method.getReturnType())) {
      result = (long)rowCount;
    } else if (Boolean.class.equals(method.getReturnType()) || Boolean.TYPE.equals(method.getReturnType())) {
      result = rowCount > 0;
    } else {
      throw new BindingException("Mapper method '" + command.getName() + "' has an unsupported return type: " + method.getReturnType());
    }
    return result;
  }
```
Mybatis中会使用上面这个方法最后会对jdbc 增删改返回的int结果进行处理，处理为mapper接口中增删改方法返回值的类型。<br />**int、Integer、long、Long就不说了，主要说一下返回值是boolean、Boolean类型，如果影响的行数大于0了，将返回true。**<br />下面来创建一个工程感受一下增删改各种返回值。
<a name="a4F6A"></a>
## 创建案例
新建一个模块chat04，模块坐标如下：
```xml
<groupId>com.javacode2018</groupId>
<artifactId>chat04</artifactId>
<version>1.0-SNAPSHOT</version>
```
下面通过Mybatis快速来实现对t_user表增删改。
<a name="fwrTa"></a>
### 创建UserModel类
mybatis-series\chat04\src\main\java\com\javacode2018\chat04\demo1\model目录创建UserModel.java，如下：
```java
package com.javacode2018.chat04.demo1.model;

import lombok.*;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Builder
@ToString
public class UserModel {
    private Long id;
    private String name;
    private Integer age;
    private Double salary;
    private Integer sex;
}
```
<a name="REPhZ"></a>
### 创建UserMapper接口
mybatis-series\chat04\src\main\java\com\javacode2018\chat04\demo1\mapper目录创建UserMapper.java，如下：
```java
package com.javacode2018.chat04.demo1.mapper;

import com.javacode2018.chat04.demo1.model.UserModel;

public interface UserMapper {

    /**
     * 插入用户信息，返回影响行数
     *
     * @param model
     * @return
     */
    int insertUser(UserModel model);

    /**
     * 更新用户信息，返回影响行数
     *
     * @param model
     * @return
     */
    long updateUser(UserModel model);

    /**
     * 根据用户id删除用户信息，返回删除是否成功
     *
     * @param userId
     * @return
     */
    boolean deleteUser(Long userId);

}
```
注意上面3个操作的返回类型，体验一下int、long、boolean类型的返回值。
<a name="nPWuE"></a>
### 创建UserMapper.xml文件
mybatis-series\chat04\src\main\resources\demo1目录创建，UserMapper.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.chat04.demo1.mapper.UserMapper">

    <insert id="insertUser" parameterType="com.javacode2018.chat04.demo1.model.UserModel">
        <![CDATA[
        INSERT INTO t_user (id,name,age,salary,sex) VALUES (#{id},#{name},#{age},#{salary},#{sex})
         ]]>
    </insert>

    <update id="updateUser" parameterType="com.javacode2018.chat04.demo1.model.UserModel">
        <![CDATA[
        UPDATE t_user SET name = #{name},age = #{age},salary = #{salary},sex = #{sex} WHERE id = #{id}
        ]]>
    </update>

    <update id="deleteUser" parameterType="java.lang.Long">
        <![CDATA[
        DELETE FROM t_user WHERE id = #{id}
        ]]>
    </update>

</mapper>
```
<a name="bK4La"></a>
### 创建属性配置文件
mybatis-series\chat04\src\main\resources目录中创建jdbc.properties，如下：
```
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8
jdbc.username=root
jdbc.password=root123
```
<a name="bKoAq"></a>
### 创建Mybatis全局配置文件
mybatis-series\chat04\src\main\resources\demo1目录创建，mybatis-config.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!-- 引入外部jdbc配置 -->
    <properties resource="jdbc.properties"/>
    <!-- 环境配置，可以配置多个环境 -->
    <environments default="demo4">
        <environment id="demo4">
            <!-- 事务管理器工厂配置 -->
            <transactionManager type="JDBC"/>
            <!-- 数据源工厂配置，使用工厂来创建数据源 -->
            <dataSource type="POOLED">
                <property name="driver" value="${jdbc.driver}"/>
                <property name="url" value="${jdbc.url}"/>
                <property name="username" value="${jdbc.username}"/>
                <property name="password" value="${jdbc.password}"/>
            </dataSource>
        </environment>
    </environments>

    <mappers>
        <mapper resource="demo1/UserMapper.xml" />
    </mappers>
</configuration>
```
<a name="LuifE"></a>
### 引入logback日志支持
chat04\src\main\resources目录创建logback.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
  <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <encoder>
      <pattern>%d{mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</pattern>
    </encoder>
  </appender>

  <logger name="com.javacode2018" level="debug" additivity="false">
    <appender-ref ref="STDOUT" />
  </logger>

</configuration>
```
<a name="jJm0P"></a>
### 创建测试用例Demo1Test
mybatis-series\chat04\src\test\java\com\javacode2018\chat04目录创建Demo1Test.java，如下：
```java
package com.javacode2018.chat04;

import com.javacode2018.chat04.demo1.mapper.UserMapper;
import com.javacode2018.chat04.demo1.model.UserModel;
import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;

@Slf4j
public class Demo1Test {
    private SqlSessionFactory sqlSessionFactory;

    @Before
    public void before() throws IOException {
        //指定mybatis全局配置文件
        String resource = "demo1/mybatis-config.xml";
        //读取全局配置文件
        InputStream inputStream = Resources.getResourceAsStream(resource);
        //构建SqlSessionFactory对象
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        this.sqlSessionFactory = sqlSessionFactory;
    }

    @Test
    public void insertUser() {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            //创建UserModel对象
            UserModel userModel = UserModel.builder().id(1L).name("Java").age(30).salary(50000D).sex(1).build();
            //执行插入操作
            int insert = mapper.insertUser(userModel);
            log.info("影响行数：{}", insert);
        }
    }

    @Test
    public void updateUser() {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            //创建UserModel对象
            UserModel userModel = UserModel.builder().id(1L).name("Hi").age(18).salary(5000D).sex(0).build();
            //执行更新操作
            long result = mapper.updateUser(userModel);
            log.info("影响行数：{}", result);
        }
    }

    @Test
    public void deleteUser() {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            //定义需要删除的用户id
            Long userId = 1L;
            //执行删除操作
            boolean result = mapper.deleteUser(userId);
            log.info("第1次删除:id={},返回值:{}", userId, result);
            result = mapper.deleteUser(userId);
            log.info("第2次删除:id={},返回值:{}", userId, result);
        }
    }
}
```
<a name="dElxA"></a>
### 项目结构如下图
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690553406613-5e54d168-00e4-4b20-a98a-8efd34cb5a1c.png#averageHue=%23f0f5de&clientId=ub71da492-1e04-4&from=paste&id=ud649ff33&originHeight=816&originWidth=577&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf03a1306-bd84-439f-8a1d-ef6fe06667a&title=)<br />注意项目结构如下图，跑起来有问题的可以对照一下。
<a name="mNUO9"></a>
### 运行测试用例
<a name="oMp7K"></a>
#### 测试int类型返回值
运行com.javacode2018.chat04.Demo1Test#insertUser，插入一条用户信息，输出如下：
```
16:35.821 [main] DEBUG c.j.c.d.mapper.UserMapper.insertUser - ==>  Preparing: INSERT INTO t_user (id,name,age,salary,sex) VALUES (?,?,?,?,?) 
16:35.858 [main] DEBUG c.j.c.d.mapper.UserMapper.insertUser - ==> Parameters: 1(Long), Java(String), 30(Integer), 50000.0(Double), 1(Integer)
16:35.865 [main] DEBUG c.j.c.d.mapper.UserMapper.insertUser - <==    Updates: 1
16:35.865 [main] INFO  com.javacode2018.chat04.Demo1Test - 影响行数：1
```
<a name="VH1v9"></a>
#### 测试long类型返回值
运行com.javacode2018.chat04.Demo1Test#updateUser，通过用户id更新用户信息，输出如下：
```
17:49.084 [main] DEBUG c.j.c.d.mapper.UserMapper.updateUser - ==>  Preparing: UPDATE t_user SET name = ?,age = ?,salary = ?,sex = ? WHERE id = ? 
17:49.127 [main] DEBUG c.j.c.d.mapper.UserMapper.updateUser - ==> Parameters: Hi(String), 18(Integer), 5000.0(Double), 0(Integer), 1(Long)
17:49.135 [main] DEBUG c.j.c.d.mapper.UserMapper.updateUser - <==    Updates: 1
17:49.135 [main] INFO  com.javacode2018.chat04.Demo1Test - 影响行数：1
```
<a name="gjBGj"></a>
#### 测试boolean类型返回值
运行com.javacode2018.chat04.Demo1Test#deleteUser，根据用户id删除用户信息，删除2次，输出如下：
```
20:37.745 [main] DEBUG c.j.c.d.mapper.UserMapper.deleteUser - ==>  Preparing: DELETE FROM t_user WHERE id = ? 
20:37.785 [main] DEBUG c.j.c.d.mapper.UserMapper.deleteUser - ==> Parameters: 1(Long)
20:37.790 [main] DEBUG c.j.c.d.mapper.UserMapper.deleteUser - <==    Updates: 0
20:37.791 [main] INFO  com.javacode2018.chat04.Demo1Test - 第1次删除:id=1,返回值:false
20:37.793 [main] DEBUG c.j.c.d.mapper.UserMapper.deleteUser - ==>  Preparing: DELETE FROM t_user WHERE id = ? 
20:37.794 [main] DEBUG c.j.c.d.mapper.UserMapper.deleteUser - ==> Parameters: 1(Long)
20:37.795 [main] DEBUG c.j.c.d.mapper.UserMapper.deleteUser - <==    Updates: 0
20:37.795 [main] INFO  com.javacode2018.chat04.Demo1Test - 第2次删除:id=1,返回值:false
```
第一次删除成功，再次删除数据已经不存在了，返回false。
<a name="OzyaC"></a>
## jdbc获取主键的几种方式
上面的案例中`inserUser`会向t_user表插入数据，t_user表的id是自动增长的，插入数据的时候不指定id的值，看看插入成功之后userModel对象和db中插入的记录是什么样的。<br />com.javacode2018.chat04.Demo1Test#insertUser代码改成下面这样：
```java
@Test
public void insertUser() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //创建UserModel对象
        UserModel userModel = UserModel.builder().name("郭富城").age(30).salary(50000D).sex(1).build();
        //执行插入操作
        int insert = mapper.insertUser(userModel);
        log.info("影响行数：{}", insert);
        log.info("{}", userModel);
    }
}
```
执行一下，输出：
```
36:10.673 [main] DEBUG c.j.c.d.mapper.UserMapper.insertUser - ==>  Preparing: INSERT INTO t_user (id,name,age,salary,sex) VALUES (?,?,?,?,?) 
36:10.715 [main] DEBUG c.j.c.d.mapper.UserMapper.insertUser - ==> Parameters: null, 郭富城(String), 30(Integer), 50000.0(Double), 1(Integer)
36:10.721 [main] DEBUG c.j.c.d.mapper.UserMapper.insertUser - <==    Updates: 1
36:10.722 [main] INFO  com.javacode2018.chat04.Demo1Test - 影响行数：1
36:10.723 [main] INFO  com.javacode2018.chat04.Demo1Test - UserModel(id=null, name=郭富城, age=30, salary=50000.0, sex=1)
```
输出中插入成功1行，最后一行日志中输出了userModel对象所有属性信息，id是null的，去db中看一下这条记录：
```sql
mysql> SELECT * FROM t_user;
+----+-----------+-----+----------+-----+
| id | name      | age | salary   | sex |
+----+-----------+-----+----------+-----+
|  2 | 郭富城    |  30 | 50000.00 |   1 |
+----+-----------+-----+----------+-----+
1 row in set (0.00 sec)
```
db中插入的这条郭富城的id是2，当没有指定id，或者指定的id为null的时候，MySQL会自动生成id的值。<br />那么如何从MySQL中获取这个自动增长的值呢？先看看jdbc是如何实现的
<a name="Bd1Bk"></a>
### 方式1：jdbc内置的方式
<a name="S1ulN"></a>
#### 用法
jdbc的api中提供了获取自动生成主键的值，具体看这个方法：
```
java.sql.Statement#getGeneratedKeys
```
看一下这个方法的定义：
```java
/**
* Retrieves any auto-generated keys created as a result of executing this
* <code>Statement</code> object. If this <code>Statement</code> object did
* not generate any keys, an empty <code>ResultSet</code>
* object is returned.
*
*<p><B>Note:</B>If the columns which represent the auto-generated keys were not specified,
* the JDBC driver implementation will determine the columns which best represent the auto-generated keys.
*
* @return a <code>ResultSet</code> object containing the auto-generated key(s)
*         generated by the execution of this <code>Statement</code> object
* @exception SQLException if a database access error occurs or
* this method is called on a closed <code>Statement</code>
* @throws SQLFeatureNotSupportedException  if the JDBC driver does not support this method
* @since 1.4
*/
ResultSet getGeneratedKeys() throws SQLException;
```
这个方法会返回一个结果集，从这个结果集中可以获取自增主键的值。<br />不过使用这个方法有个前提，执行sql的时候需要做一个设置。<br />如果是通过java.sql.Statement执行sql，需要调用下面这个方法：
```java
int executeUpdate(String sql, int autoGeneratedKeys) throws SQLException
```
注意上面这个方法的第二个参数需要设置为`java.sql.Statement.RETURN_GENERATED_KEYS`，表示需要返回自增列的值。<br />不过多数情况下，会使用`java.sql.PreparedStatement`对象来执行sql，如果想获取自增值，创建这个对象需要设置第2个参数的值，如下：
```java
PreparedStatement preparedStatement = connection.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
```
然后就可以通过`getGeneratedKeys`返回的`ResultSet`对象获取自动增长的值了，如下：
```java
ResultSet generatedKeys = preparedStatement.getGeneratedKeys();
if (generatedKeys!=null && generatedKeys.next()) {
    log.info("自增值为：{}", generatedKeys.getInt(1));
}
```
<a name="ATBeU"></a>
#### 案例
com.javacode2018.chat04.Demo1Test中新增一个测试用例，如下代码：
```java
private String jdbcDriver = "com.mysql.jdbc.Driver";
private String jdbcUrl = "jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8";
private String jdbcUserName = "root";
private String jdbcPassword = "root123";

@Test
public void jdbcInsertUser1() throws Exception {
    Connection connection = null;
    PreparedStatement preparedStatement = null;
    ResultSet generatedKeys = null;
    try {

        UserModel userModel = UserModel.builder().name("黎明").age(30).salary(50000D).sex(1).build();
        //执行jdbc插入数据操作
        Class.forName(jdbcDriver);
        connection = DriverManager.getConnection(jdbcUrl, jdbcUserName, jdbcPassword);
        //注意创建PreparedStatement的时候，使用prepareStatement方法的第二个参数需要指定Statement.RETURN_GENERATED_KEYS
        preparedStatement = connection.prepareStatement("INSERT INTO t_user (name,age,salary,sex) VALUES (?,?,?,?)", Statement.RETURN_GENERATED_KEYS);
        int parameterIndex = 1;
        preparedStatement.setString(parameterIndex++, userModel.getName());
        preparedStatement.setInt(parameterIndex++, userModel.getAge());
        preparedStatement.setDouble(parameterIndex++, userModel.getSalary());
        preparedStatement.setInt(parameterIndex++, userModel.getSex());
        int count = preparedStatement.executeUpdate();
        log.info("影响行数：{}", count);

        //获取自增值
        generatedKeys = preparedStatement.getGeneratedKeys();
        if (generatedKeys != null && generatedKeys.next()) {
            log.info("自增值为：{}", generatedKeys.getInt(1));
        }
    } finally {
        if (generatedKeys != null && generatedKeys.isClosed()) {
            generatedKeys.close();
        }
        if (preparedStatement != null && preparedStatement.isClosed()) {
            preparedStatement.close();
        }
        if (connection != null && connection.isClosed()) {
            connection.close();
        }
    }
}
```
上面代码中插入了一条用户的信息，没有指定用户的id，执行输出：
```
21:22.410 [main] INFO  com.javacode2018.chat04.Demo1Test - 影响行数：1
21:22.414 [main] INFO  com.javacode2018.chat04.Demo1Test - 自增值为：5
```
去db中看一下这个记录的id，如下，确实是5：
```sql
mysql> SELECT * FROM t_user;
+----+--------+-----+----------+-----+
| id | name   | age | salary   | sex |
+----+--------+-----+----------+-----+
|  5 | 黎明   |  30 | 50000.00 |   1 |
+----+--------+-----+----------+-----+
1 row in set (0.00 sec)
```
<a name="x1eE0"></a>
### 方式2：插入之后查询获取
<a name="M1FF8"></a>
#### 用法
MySQL中插入一条数据之后，可以通过下面的sql获取最新插入记录的id的值：
```sql
SELECT LAST_INSERT_ID()
```
那么可以在插入之后，立即使用当前连接发送上面这条sql去获取自增列的值就可以。
<a name="DCCys"></a>
#### 案例
创建测试用例com.javacode2018.chat04.Demo1Test#jdbcInsertUser2,代码如下：
```java
@Test
public void jdbcInsertUser2() throws Exception {
    Connection connection = null;
    PreparedStatement preparedStatement = null;
    ResultSet rs = null;
    try {
        UserModel userModel = UserModel.builder().name("梁朝伟").age(30).salary(50000D).sex(1).build();
        //执行jdbc插入数据操作
        Class.forName(jdbcDriver);
        connection = DriverManager.getConnection(jdbcUrl, jdbcUserName, jdbcPassword);
        //注意创建PreparedStatement的时候，使用prepareStatement方法的第二个参数需要指定Statement.RETURN_GENERATED_KEYS
        preparedStatement = connection.prepareStatement("INSERT INTO t_user (name,age,salary,sex) VALUES (?,?,?,?)", Statement.RETURN_GENERATED_KEYS);
        int parameterIndex = 1;
        preparedStatement.setString(parameterIndex++, userModel.getName());
        preparedStatement.setInt(parameterIndex++, userModel.getAge());
        preparedStatement.setDouble(parameterIndex++, userModel.getSalary());
        preparedStatement.setInt(parameterIndex++, userModel.getSex());
        int count = preparedStatement.executeUpdate();
        log.info("影响行数：{}", count);

        //通过查询获取自增值
        rs = connection.prepareStatement("SELECT LAST_INSERT_ID()").executeQuery();
        if (rs != null && rs.next()) {
            log.info("自增值为：{}", rs.getInt(1));
        }
    } finally {
        if (rs != null && rs.isClosed()) {
            rs.close();
        }
        if (preparedStatement != null && preparedStatement.isClosed()) {
            preparedStatement.close();
        }
        if (connection != null && connection.isClosed()) {
            connection.close();
        }
    }
}
```
运行输出：
```
26:55.407 [main] INFO  com.javacode2018.chat04.Demo1Test - 影响行数：1
26:55.414 [main] INFO  com.javacode2018.chat04.Demo1Test - 自增值为：6
```
db中去看一下，梁朝伟的id是6，如下：
```sql
mysql> SELECT * FROM t_user;
+----+-----------+-----+----------+-----+
| id | name      | age | salary   | sex |
+----+-----------+-----+----------+-----+
|  5 | 黎明      |  30 | 50000.00 |   1 |
|  6 | 梁朝伟    |  30 | 50000.00 |   1 |
+----+-----------+-----+----------+-----+
2 rows in set (0.00 sec)
```
<a name="QlymD"></a>
### 方式3：插入之前获取
Oracle中没有MySQL中自动增长列，但是Oracle有个功能可以实现自动增长，这个功能就是序列，序列就相当于一个自增器一样，有个初始值，每次递增的步长，当然这个序列提供了一些功能给开发者使用，可以获取序列的当前值、下一个值，使用方式如下：

1. 先定义一个序列
2. 获取下一个值：`SELECT 序列名.NEXTVAL FROM dual;`

这个案例只说一下具体步骤，代码就不写了，步骤：

1. 通过jdbc执行`SELECT 序列名.NEXTVAL FROM dual`获取序列的下一个值，如nextId
2. 在代码中使用nextId的值

上面就是jdbc获取值增值的几种方式，jdbc中的这3中方式，Mybatis中都提供了对应的 支持，下面来看Mybatis中是如何实现的。
<a name="WGd8e"></a>
## Mybatis获取主键的3种方式
<a name="dBsi6"></a>
### 方式1：内部使用jdbc内置的方式
<a name="WqsNm"></a>
#### 用法
Mybatis这个方式内部采用的是上面说的jdbc内置的方式。<br />需要在Mapper xml中进行配置，如：
```java
<insert id="insertUser1" parameterType="com.javacode2018.chat04.demo1.model.UserModel" useGeneratedKeys="true" keyProperty="id">
    <![CDATA[
    INSERT INTO t_user (name,age,salary,sex) VALUES (#{name},#{age},#{salary},#{sex})
     ]]>
</insert>
```
有2个关键参数必须要设置：

- `useGeneratedKeys`：设置为true
- `keyProperty`：参数对象中的属性名称，最后插入成功之后，Mybatis会通过反射将自增值设置给keyProperty指定的这个属性
<a name="vYmho"></a>
#### 案例
mybatis-series\chat04\src\main\resources\demo1\UserMapper.xml中新增代码：
```xml
<!-- 插入的时候获取值增值，必须需指定2个属性
useGeneratedKeys：设置为true
keyProperty：参数对象中的属性，插入成功之后会将值增值设置给这个属性
-->
<insert id="insertUser1" parameterType="com.javacode2018.chat04.demo1.model.UserModel" useGeneratedKeys="true" keyProperty="id">
  <![CDATA[
  INSERT INTO t_user (name,age,salary,sex) VALUES (#{name},#{age},#{salary},#{sex})
  ]]>
</insert>
```
Mapper接口中也新增代码，com.javacode2018.chat04.demo1.mapper.UserMapper中新增一个方法，如下：
```java
int insertUser1(UserModel userModel);
```
创建测试用例方法com.javacode2018.chat04.Demo1Test#insertUser1，如下：
```java
@Test
public void insertUser1() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //创建UserModel对象
        UserModel userModel = UserModel.builder().name("陈宝国").age(30).salary(50000D).sex(1).build();
        //执行插入操作
        int insert = mapper.insertUser1(userModel);
        log.info("影响行数：{}", insert);
        log.info("{}", userModel);
    }
}
```
注意上面的userModel对象，id没有设置值，运行输出：
```
59:44.412 [main] DEBUG c.j.c.d.m.UserMapper.insertUser1 - ==>  Preparing: INSERT INTO t_user (name,age,salary,sex) VALUES (?,?,?,?) 
59:44.444 [main] DEBUG c.j.c.d.m.UserMapper.insertUser1 - ==> Parameters: 陈宝国(String), 30(Integer), 50000.0(Double), 1(Integer)
59:44.451 [main] DEBUG c.j.c.d.m.UserMapper.insertUser1 - <==    Updates: 1
59:44.453 [main] INFO  com.javacode2018.chat04.Demo1Test - 影响行数：1
59:44.455 [main] INFO  com.javacode2018.chat04.Demo1Test - UserModel(id=8, name=陈宝国, age=30, salary=50000.0, sex=1)
```
看上面最后一行输出，id的值为8，去db中看一下，如下：
```sql
mysql> SELECT * FROM t_user;
+----+-----------+-----+----------+-----+
| id | name      | age | salary   | sex |
+----+-----------+-----+----------+-----+
|  5 | 黎明      |  30 | 50000.00 |   1 |
|  6 | 梁朝伟    |  30 | 50000.00 |   1 |
|  7 | 陈宝国    |  30 | 50000.00 |   1 |
|  8 | 陈宝国    |  30 | 50000.00 |   1 |
+----+-----------+-----+----------+-----+
4 rows in set (0.00 sec)
```
<a name="pRKIv"></a>
### 方式2：插入后查询获取主键
<a name="XgVbg"></a>
#### 用法
这个方式和上面介绍的jdbc的第二种方式一样，插入之后通过查询获取主键的值然后填充给指定的属性，mapper xml配置如下：
```xml
<insert id="insertUser2" parameterType="com.javacode2018.chat04.demo1.model.UserModel">
  <selectKey keyProperty="id" order="AFTER" resultType="long">
    <![CDATA[
    SELECT LAST_INSERT_ID()
    ]]>
  </selectKey>
  <![CDATA[
  INSERT INTO t_user (name,age,salary,sex) VALUES (#{name},#{age},#{salary},#{sex})
  ]]>
</insert>
```
关键代码是selectKey元素包含的部分，这个元素内部可以包含一个sql，这个sql可以在插入之前或者插入之后运行（之前还是之后通过order属性配置），然后会将sql运行的结果设置给keyProperty指定的属性，selectKey元素有3个属性需要指定：

- `keyProperty`：参数对象中的属性名称，最后插入成功之后，Mybatis会通过反射将自增值设置给keyProperty指定的这个属性
- `order`：指定selectKey元素中的sql是在插入之前运行还是插入之后运行，可选值（BEFORE|AFTER），这种方式中选择AFTER
- `resultType`：keyProperty指定的属性对应的类型，如上面的id对应的类型是java.lang.Long，直接写的是别名long
<a name="oItX9"></a>
#### 案例
mybatis-series\chat04\src\main\resources\demo1\UserMapper.xml中新增代码：
```xml
<insert id="insertUser2" parameterType="com.javacode2018.chat04.demo1.model.UserModel">
  <selectKey keyProperty="id" order="AFTER" resultType="long">
    <![CDATA[
    SELECT LAST_INSERT_ID()
    ]]>
  </selectKey>
  <![CDATA[
  INSERT INTO t_user (name,age,salary,sex) VALUES (#{name},#{age},#{salary},#{sex})
  ]]>
</insert>
```
Mapper接口中也新增代码，com.javacode2018.chat04.demo1.mapper.UserMapper中新增一个方法，如下：
```java
int insertUser2(UserModel userModel);
```
创建测试用例方法com.javacode2018.chat04.Demo1Test#insertUser2，如下：
```java
@Test
public void insertUser2() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //创建UserModel对象
        UserModel userModel = UserModel.builder().name("周润发").age(30).salary(50000D).sex(1).build();
        //执行插入操作
        int insert = mapper.insertUser2(userModel);
        log.info("影响行数：{}", insert);
        log.info("{}", userModel);
    }
}
```
注意上面的userModel对象，id没有设置值，运行输出：
```
22:18.140 [main] DEBUG c.j.c.d.m.UserMapper.insertUser2 - ==>  Preparing: INSERT INTO t_user (name,age,salary,sex) VALUES (?,?,?,?) 
22:18.173 [main] DEBUG c.j.c.d.m.UserMapper.insertUser2 - ==> Parameters: 周润发(String), 30(Integer), 50000.0(Double), 1(Integer)
22:18.180 [main] DEBUG c.j.c.d.m.UserMapper.insertUser2 - <==    Updates: 1
22:18.183 [main] DEBUG c.j.c.d.m.U.insertUser2!selectKey - ==>  Preparing: SELECT LAST_INSERT_ID() 
22:18.183 [main] DEBUG c.j.c.d.m.U.insertUser2!selectKey - ==> Parameters: 
22:18.197 [main] DEBUG c.j.c.d.m.U.insertUser2!selectKey - <==      Total: 1
22:18.198 [main] INFO  com.javacode2018.chat04.Demo1Test - 影响行数：1
22:18.200 [main] INFO  com.javacode2018.chat04.Demo1Test - UserModel(id=11, name=周润发, age=30, salary=50000.0, sex=1)
```
上面输出中执行了2条sql，先执行的插入，然后执行了一个查询获取自增值id，最后一行输出的id为11.<br />去db中看一下，如下：
```sql
mysql> SELECT * FROM t_user order by id desc limit 1;
+----+-----------+-----+----------+-----+
| id | name      | age | salary   | sex |
+----+-----------+-----+----------+-----+
| 11 | 周润发    |  30 | 50000.00 |   1 |
+----+-----------+-----+----------+-----+
1 row in set (0.00 sec)
```
<a name="THTYJ"></a>
### 方式2：插入前查询获取主键
<a name="FKhJK"></a>
#### 用法
这个方式和上面介绍的jdbc的第3种方式一样，会在插入之前先通过一个查询获取主键的值然后填充给指定的属性，然后在执行插入，mapper xml配置如下：
```xml
<insert id="insertUser3" parameterType="com.javacode2018.chat04.demo1.model.UserModel">
  <selectKey keyProperty="id" order="BEFORE" resultType="long">
    <![CDATA[ 获取主键的select语句 ]]>
  </selectKey>
  <![CDATA[
  INSERT INTO t_user (name,age,salary,sex) VALUES (#{name},#{age},#{salary},#{sex})
  ]]>
</insert>
```
关键代码是selectKey元素包含的部分，这个元素内部可以包含一个sql，这个sql可以在插入之前或者插入之后运行（之前还是之后通过order属性配置），然后会将sql运行的结果设置给keyProperty指定的属性，selectKey元素有3个属性需要指定：

- `keyProperty`：参数对象中的属性名称，最后插入成功之后，Mybatis会通过反射将自增值设置给keyProperty指定的这个属性
- `order`：指定selectKey元素中的sql是在插入之前运行还是插入之后运行，可选值（BEFORE|AFTER），这种方式中选择BEFORE
- `resultType`：`keyProperty`指定的属性对应的类型，如上面的id对应的类型是java.lang.Long，直接写的是别名long
<a name="jNGMo"></a>
#### 案例
这个案例就不写了，可以拿Oracle的序列去练习一下这个案例。
<a name="WJBOH"></a>
### 源码
Mybatis处理自动生产主键值的代码，主要看下面这个接口：
```
org.apache.ibatis.executor.keygen.KeyGenerator
```
看一下这个接口的定义：
```java
public interface KeyGenerator {

    void processBefore(Executor executor, MappedStatement ms, Statement stmt, Object parameter);

    void processAfter(Executor executor, MappedStatement ms, Statement stmt, Object parameter);

}
```
有2个方法，根据方法名称就可以知道，一个是插入sql执行之前调用的，一个是之后调用的，通过这2个方法Mybatis完成了获取主键的功能。<br />这个接口默认有3个实现类：
```
org.apache.ibatis.executor.keygen.Jdbc3KeyGenerator
org.apache.ibatis.executor.keygen.SelectKeyGenerator
org.apache.ibatis.executor.keygen.NoKeyGenerator
```
Mybatis中获取主键的第一种方式就是在Jdbc3KeyGenerator类中实现的，其他2种方式是在第2个类中实现的，大家可以去看一下代码，设置断点感受一下，第3个类2个方法是空实现。
