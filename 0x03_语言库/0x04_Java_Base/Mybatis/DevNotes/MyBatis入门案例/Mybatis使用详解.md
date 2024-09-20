Mybatis
<a name="kEMrS"></a>
## 创建案例
<a name="wifvr"></a>
### 建库建表
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

DROP TABLE IF EXISTS `t_order`;
CREATE TABLE t_order (
  id BIGINT AUTO_INCREMENT PRIMARY KEY COMMENT '主键，订单id，自动增长',
  `user_id` BIGINT NOT NULL DEFAULT 0 COMMENT '用户id',
  `price` DECIMAL(12,2) NOT NULL DEFAULT 0 COMMENT '订单金额'
) COMMENT '订单表';

/*插入几条测试数据*/
INSERT INTO t_user (`name`,`age`,`salary`,`sex`)
VALUES
  ('Java',30,50000,1),
  ('javacode2018',30,50000,1),
  ('张学友',56,500000,1),
  ('林志玲',45,88888.88,2);


INSERT INTO t_order (`user_id`,`price`)
VALUES
  (1,88.88),
  (2,666.66);

SELECT * FROM t_user;
SELECT * FROM t_order;
```
<a name="aDCdS"></a>
### 创建工程
在项目中新建一个模块chat03，模块坐标如下：
```xml
<groupId>com.javacode2018</groupId>
<artifactId>chat03</artifactId>
<version>1.0-SNAPSHOT</version>
```
下面通过Mybatis快速来实现对t_user表增删改查。
<a name="hyRh3"></a>
#### 创建Mybatis配置文件
chat03\src\main\resources\demo1目录创建，mybatis-config.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <typeAliases></typeAliases>
    <!-- 环境配置，可以配置多个环境 -->
    <environments default="chat04-demo1">
        <!-- 
            environment用来对某个环境进行配置
            id：环境标识，唯一
         -->
        <environment id="chat04-demo1">
            <!-- 事务管理器工厂配置 -->
            <transactionManager type="org.apache.ibatis.transaction.jdbc.JdbcTransactionFactory"/>
            <!-- 数据源工厂配置，使用工厂来创建数据源 -->
            <dataSource type="org.apache.ibatis.datasource.pooled.PooledDataSourceFactory">
                <property name="driver" value="com.mysql.jdbc.Driver"/>
                <property name="url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
                <property name="username" value="root"/>
                <property name="password" value="root123"/>
            </dataSource>
        </environment>
    </environments>

    <mappers>
        <mapper resource="demo1/mapper/UserMapper.xml"/>
    </mappers>
</configuration>
```
<a name="K5ZYV"></a>
#### 创建UserMapper.xml文件
chat03\src\main\resources\demo1\mapper目录创建，UserMapper.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.chat03.demo1.UserMapper">
    <!-- insert用来定义一个插入操作
         id：操作的具体标识
         parameterType：指定插入操作接受的参数类型
     -->
    <insert id="insertUser" parameterType="com.javacode2018.chat03.demo1.UserModel" useGeneratedKeys="true">
        <![CDATA[
        INSERT INTO t_user (id,name,age,salary,sex) VALUES (#{id},#{name},#{age},#{salary},#{sex})
         ]]>
    </insert>

    <!-- update用来定义一个更新操作
         id：操作的具体标识
         parameterType：指定操作接受的参数类型
     -->
    <update id="updateUser" parameterType="com.javacode2018.chat03.demo1.UserModel">
        <![CDATA[
        UPDATE t_user SET name = #{name},age = #{age},salary = #{salary},sex = #{sex} WHERE id = #{id}
        ]]>
    </update>

    <!-- update用来定义一个删除操作
         id：操作的具体标识
         parameterType：指定操作接受的参数类型
     -->
    <update id="deleteUser" parameterType="java.lang.Long">
        <![CDATA[
        DELETE FROM t_user WHERE id = #{id}
        ]]>
    </update>

    <!-- select用来定义一个查询操作
         id：操作的具体标识
         resultType：指定查询结果保存的类型
     -->
    <select id="getUserList" resultType="com.javacode2018.chat03.demo1.UserModel">
        <![CDATA[
        SELECT * FROM t_user
        ]]>
    </select>

</mapper>
```
<a name="tfeuL"></a>
#### 创建UserModel类
chat03\src\main\java\com\javacode2018\chat03\demo1目录创建UserModel.java，如下：
```java
package com.javacode2018.chat03.demo1;

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
<a name="Vb7SH"></a>
#### 创建UserMapper接口
chat03\src\main\java\com\javacode2018\chat03\demo1目录创建UserMapper.java，如下：
```java
package com.javacode2018.chat03.demo1;

import java.util.List;

public interface UserMapper {

    int insertUser(UserModel model);

    int updateUser(UserModel model);

    int deleteUser(Long userId);

    List<UserModel> getUserList();
}
```
<a name="TTb55"></a>
#### 引入logback日志支持
chat03\src\main\resources目录创建logback.xml，如下：
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
<a name="a680r"></a>
#### 创建测试用例UserMapperTest
chat03\src\test\java\com\javacode2018\chat03\demo1目录创建UserMapperTest.java，如下：
```java
package com.javacode2018.chat03.demo1;

import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

@Slf4j
public class UserMapperTest {
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
    public void getUserList() {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            //执行查询操作
            List<UserModel> userModelList = mapper.getUserList();
            userModelList.forEach(item -> {
                log.info("{}", item);
            });
        }
    }

}
```
代码解释一下：<br />上面的`before()`方法上面有个`@Before`注解，这个是junit提供的一个注解，通过junit运行每个`@Test`标注的方法之前，会先运行被`@before`标注的方法，`before()`方法中创建了`SqlSessionFactory`对象，所以其他的`@Test`标注的方法中可以直接使用`sqlSessionFactory`对象了。
<a name="TmQx5"></a>
#### 项目结构如下图
注意项目结构如下图，跑起来有问题的可以对照一下。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690549557029-476e369e-732f-4faf-902a-3d9b0ac2158f.png#averageHue=%23f1f9e4&clientId=u8a864d1a-b908-4&from=paste&id=u2113f338&originHeight=799&originWidth=559&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3b7b6d17-8d23-4efc-98be-8b30f8d8594&title=)
<a name="YMdYD"></a>
#### 运行一下测试用例看效果
运行一下`UserMapperTest.getUserList()`方法，输出如下：
```
32:21.991 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
32:22.028 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
32:22.052 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
32:22.053 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
32:22.056 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
32:22.056 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
32:22.056 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
上面是Mybatis开发项目的一个玩转的步骤，希望大家都能够熟练掌握，下面来在这个示例的基础上讲解本章的知识点。
<a name="z7YYu"></a>
## 别名
<a name="jyUVr"></a>
### 为什么需要使用别名?
大家打开chat03\src\main\resources\demo1\mapper\UserMapper.xml文件看一下，是不是有很多下面这样的代码：
```
parameterType="com.javacode2018.chat03.demo1.UserModel"
resultType="com.javacode2018.chat03.demo1.UserModel"
```
parameterType是指定参数的类型，resultType是指定查询结果返回值的类型，他们的值都是UserModel类完整的类名，比较长，Mybatis支持给某个类型起一个别名，然后通过别名可以访问到指定的类型。
<a name="npM63"></a>
### 别名的用法
使用别名之前需要先在Mybatis中注册别名，先说通过Mybatis全局配置文件中注册别名，通过Mybatis配置文件注册别名有3种方式。
<a name="pi1ut"></a>
### 方式1
<a name="Suymr"></a>
#### 使用`typeAlias`元素进行注册
如下：
```xml
<typeAliases>
  <typeAlias type="玩转的类型名称" alias="别名" />
</typeAliases>
```
`**typeAliases**`**元素中可以包含多个**`**typeAlias**`**子元素，每个**`**typeAlias**`**可以给一个类型注册别名，有2个属性需要指定：**<br />**type：完整的类型名称**<br />**alias：别名**<br />如上面给UserModel起了一个别名为user。
<a name="cIqRg"></a>
#### 案例
给UserModel注册一个别名user
```
chat03\src\main\resources\demo1\mapper\UserMapper.xml
```
中加入下面配置：
```xml
<typeAliases>
  <typeAlias type="com.javacode2018.chat03.demo1.UserModel" alias="user" />
</typeAliases>
```
UserMapper.xml中使用别名，将chat03\src\main\resources\demo1\mapper\UserMapper.xml中getUserList的resultType的值改为user，如下：
```xml
<select id="getUserList" resultType="user">
  <![CDATA[
  SELECT * FROM t_user
  ]]>
</select>
```
运行com.javacode2018.chat03.demo1.UserMapperTest#getUserList，如下：
```
07:35.477 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
07:35.505 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
07:35.527 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
07:35.527 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
07:35.529 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
07:35.529 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
07:35.529 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
`getUserList`中使用的别名，运行是正常的，说明可以通过别名user直接访问UserModel。
<a name="biaGz"></a>
### 方式2
<a name="GhXiw"></a>
#### 通过packege元素批量注册
上面通过`typeAlias`元素可以注册一个别名，如果有很多类需要注册，需要写很多`typeAlias`配置。<br />Mybatis提供了批量注册别名的方式，通过package元素，如下：
```xml
<typeAliases>
  <package name="需要扫描的包"/>
</typeAliases>
```
这个也是在typeAliases元素下面，不过这次使用的是package元素，package有个name属性，可以指定一个包名，Mybatis会加载这个包以及子包中所有的类型，给这些类型都注册别名，别名名称默认会采用类名小写的方式，如UserModel的别名为usermodel
<a name="gYexf"></a>
#### 案例
下面将demo1/mybatis-config.xml中`typeAliases`元素的值改为下面这样：
```xml
<typeAliases>
  <package name="com.javacode2018.chat03.demo1"/>
</typeAliases>
```
Mybatis会给com.javacode2018.chat03.demo1包及子包中的所有类型注册别名，UserModel类在这个包中，会被注册，别名为usermodel<br />UserMapper.xml中使用别名，将chat03\src\main\resources\demo1\mapper\UserMapper.xml中getUserList的resultType的值改为usermodel，如下：
```xml
<select id="getUserList" resultType="usermodel">
  <![CDATA[
  SELECT * FROM t_user
  ]]>
</select>
```
上面将返回值的类型resultType的值改为了usermodel<br />来运行com.javacode2018.chat03.demo1.UserMapperTest#getUserList，如下：
```
26:08.267 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
26:08.296 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
26:08.318 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
26:08.319 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
26:08.320 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
26:08.320 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
26:08.320 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
看到了么，`getUserList`中使用的别名usermodel，运行也是正常的。
<a name="J72l0"></a>
### 方式3
<a name="zb7Qm"></a>
#### package结合`@Alias`批量注册并指定别名
方式2中通过package可以批量注册别名，如果指定的包中包含了多个类名相同的类，会怎么样呢？<br />在com.javacode2018.chat03.demo1.model包中创建一个和UserModel同名的类，如下：
```java
package com.javacode2018.chat03.demo1.model;

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
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690549556892-8490b514-75ea-44b4-a2e5-ad59823cfe78.png#averageHue=%23fbfbfa&clientId=u8a864d1a-b908-4&from=paste&id=u0757d008&originHeight=333&originWidth=565&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8fdd65dd-2418-41d5-a0ff-c3bbc60ec28&title=)<br />现在com.javacode2018.demo1包中有2个UserModel类了<br />运行com.javacode2018.chat03.demo1.UserMapperTest#getUserList，如下：
```
org.apache.ibatis.exceptions.PersistenceException: 
### Error building SqlSession.
### The error may exist in SQL Mapper Configuration
### Cause: org.apache.ibatis.builder.BuilderException: Error parsing SQL Mapper Configuration. Cause: org.apache.ibatis.type.TypeException: The alias 'UserModel' is already mapped to the value 'com.javacode2018.chat03.demo1.model.UserModel'.

    at org.apache.ibatis.exceptions.ExceptionFactory.wrapException(ExceptionFactory.java:30)
    at org.apache.ibatis.session.SqlSessionFactoryBuilder.build(SqlSessionFactoryBuilder.java:80)
    at org.apache.ibatis.session.SqlSessionFactoryBuilder.build(SqlSessionFactoryBuilder.java:64)
    at com.javacode2018.chat03.demo1.UserMapperTest.before(UserMapperTest.java:29)
    at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
    at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
    at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
    at java.lang.reflect.Method.invoke(Method.java:498)
    at org.junit.runners.model.FrameworkMethod$1.runReflectiveCall(FrameworkMethod.java:50)
    at org.junit.internal.runners.model.ReflectiveCallable.run(ReflectiveCallable.java:12)
    at org.junit.runners.model.FrameworkMethod.invokeExplosively(FrameworkMethod.java:47)
    at org.junit.internal.runners.statements.RunBefores.evaluate(RunBefores.java:24)
    at org.junit.runners.ParentRunner.runLeaf(ParentRunner.java:325)
    at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:78)
    at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:57)
    at org.junit.runners.ParentRunner$3.run(ParentRunner.java:290)
    at org.junit.runners.ParentRunner$1.schedule(ParentRunner.java:71)
    at org.junit.runners.ParentRunner.runChildren(ParentRunner.java:288)
    at org.junit.runners.ParentRunner.access$000(ParentRunner.java:58)
    at org.junit.runners.ParentRunner$2.evaluate(ParentRunner.java:268)
    at org.junit.runners.ParentRunner.run(ParentRunner.java:363)
    at org.junit.runner.JUnitCore.run(JUnitCore.java:137)
    at com.intellij.junit4.JUnit4IdeaTestRunner.startRunnerWithArgs(JUnit4IdeaTestRunner.java:68)
    at com.intellij.rt.execution.junit.IdeaTestRunner$Repeater.startRunnerWithArgs(IdeaTestRunner.java:51)
    at com.intellij.rt.execution.junit.JUnitStarter.prepareStreamsAndStart(JUnitStarter.java:242)
    at com.intellij.rt.execution.junit.JUnitStarter.main(JUnitStarter.java:70)
Caused by: org.apache.ibatis.builder.BuilderException: Error parsing SQL Mapper Configuration. Cause: org.apache.ibatis.type.TypeException: The alias 'UserModel' is already mapped to the value 'com.javacode2018.chat03.demo1.model.UserModel'.
    at org.apache.ibatis.builder.xml.XMLConfigBuilder.parseConfiguration(XMLConfigBuilder.java:121)
    at org.apache.ibatis.builder.xml.XMLConfigBuilder.parse(XMLConfigBuilder.java:98)
    at org.apache.ibatis.session.SqlSessionFactoryBuilder.build(SqlSessionFactoryBuilder.java:78)
    ... 24 more
Caused by: org.apache.ibatis.type.TypeException: The alias 'UserModel' is already mapped to the value 'com.javacode2018.chat03.demo1.model.UserModel'.
    at org.apache.ibatis.type.TypeAliasRegistry.registerAlias(TypeAliasRegistry.java:157)
    at org.apache.ibatis.type.TypeAliasRegistry.registerAlias(TypeAliasRegistry.java:147)
    at org.apache.ibatis.type.TypeAliasRegistry.registerAliases(TypeAliasRegistry.java:136)
    at org.apache.ibatis.type.TypeAliasRegistry.registerAliases(TypeAliasRegistry.java:125)
    at org.apache.ibatis.builder.xml.XMLConfigBuilder.typeAliasesElement(XMLConfigBuilder.java:164)
    at org.apache.ibatis.builder.xml.XMLConfigBuilder.parseConfiguration(XMLConfigBuilder.java:109)
    ... 26 more
```
报错了，2个类的类名一样了，默认都会使用usermodel作为别名，别名重复了Mybatis会报错，那么此时怎么办呢？<br />package方式批量注册别名的时候，可以给类中添加一个`@Alias`注解来给这个类指定别名：
```java
@Alias("user")
public class UserModel {
}
```
当Mybatis扫描类的时候，发现类上有Alias注解，会取这个注解的value作为别名，如果没有这个注解，会将类名小写作为别名，如同方式2。
<a name="VdOkX"></a>
#### 案例
在com.javacode2018.chat03.demo1.UserModel类上加上下面注解：
```java
@Alias("use")
public class UserModel {
}
```
修改demo1/mapper/UserMapper.xml，将resultType的值设置为user：
```xml
<select id="getUserList" resultType="user">
  <![CDATA[
  SELECT * FROM t_user
  ]]>
</select>
```
再来运行com.javacode2018.chat03.demo1.UserMapperTest#getUserList，如下：
```
18:51.219 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
18:51.250 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
18:51.271 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
18:51.272 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
18:51.274 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
18:51.274 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
18:51.274 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
输出正常。
<a name="JXqLu"></a>
### 别名不区分大小写
可以将上面UserMapper.xml中的use别名改成大写的：USER，如下：
```xml
<select id="getUserList" resultType="USER">
  <![CDATA[
  SELECT * FROM t_user
  ]]>
</select>
```
然后再运行一下com.javacode2018.chat03.demo1.UserMapperTest#getUserList，如下：
```
42:49.474 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
42:49.509 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
42:49.527 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
42:49.528 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
42:49.530 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
42:49.530 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
42:49.531 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
也是正常的，**说明别名使用时是不区分大小写的。**
<a name="LgqGi"></a>
### Mybatis内置的别名
Mybatis默认为很多类型提供了别名，如下：

| 别名 | 对应的实际类型 |
| --- | --- |
| _byte | byte |
| _long | long |
| _short | short |
| _int | int |
| _integer | int |
| _double | double |
| _float | float |
| _boolean | boolean |
| string | String |
| byte | Byte |
| long | Long |
| short | Short |
| int | Integer |
| integer | Integer |
| double | Double |
| float | Float |
| boolean | Boolean |
| date | Date |
| decimal | BigDecimal |
| bigdecimal | BigDecimal |
| object | Object |
| map | Map |
| hashmap | HashMap |
| list | List |
| arraylist | ArrayList |
| collection | Collection |
| iterator | Iterator |

上面这些默认都是在`org.apache.ibatis.type.TypeAliasRegistry`类中进行注册的，这个类就是Mybatis注册别名使用的，别名和具体的类型关联是放在这个类的一个map属性（typeAliases）中，贴一部分代码大家感受一下：
```java
public class TypeAliasRegistry {

    private final Map<String, Class<?>> typeAliases = new HashMap<>();

    public TypeAliasRegistry() {
        registerAlias("string", String.class);

        registerAlias("byte", Byte.class);
        registerAlias("long", Long.class);
        registerAlias("short", Short.class);
        registerAlias("int", Integer.class);
        registerAlias("integer", Integer.class);
        registerAlias("double", Double.class);
        registerAlias("float", Float.class);
        registerAlias("boolean", Boolean.class);

        registerAlias("byte[]", Byte[].class);
        registerAlias("long[]", Long[].class);
        registerAlias("short[]", Short[].class);
        registerAlias("int[]", Integer[].class);
        registerAlias("integer[]", Integer[].class);
        registerAlias("double[]", Double[].class);
        registerAlias("float[]", Float[].class);
        registerAlias("boolean[]", Boolean[].class);

        registerAlias("_byte", byte.class);
        registerAlias("_long", long.class);
        registerAlias("_short", short.class);
        registerAlias("_int", int.class);
        registerAlias("_integer", int.class);
        registerAlias("_double", double.class);
        registerAlias("_float", float.class);
        registerAlias("_boolean", boolean.class);

        registerAlias("_byte[]", byte[].class);
        registerAlias("_long[]", long[].class);
        registerAlias("_short[]", short[].class);
        registerAlias("_int[]", int[].class);
        registerAlias("_integer[]", int[].class);
        registerAlias("_double[]", double[].class);
        registerAlias("_float[]", float[].class);
        registerAlias("_boolean[]", boolean[].class);

        registerAlias("date", Date.class);
        registerAlias("decimal", BigDecimal.class);
        registerAlias("bigdecimal", BigDecimal.class);
        registerAlias("biginteger", BigInteger.class);
        registerAlias("object", Object.class);

        registerAlias("date[]", Date[].class);
        registerAlias("decimal[]", BigDecimal[].class);
        registerAlias("bigdecimal[]", BigDecimal[].class);
        registerAlias("biginteger[]", BigInteger[].class);
        registerAlias("object[]", Object[].class);

        registerAlias("map", Map.class);
        registerAlias("hashmap", HashMap.class);
        registerAlias("list", List.class);
        registerAlias("arraylist", ArrayList.class);
        registerAlias("collection", Collection.class);
        registerAlias("iterator", Iterator.class);

        registerAlias("ResultSet", ResultSet.class);
    }
}
```
Mybatis启动的时候会加载全局配置文件，会将其转换为一个`org.apache.ibatis.session.Configuration`对象，存储在内存中，`Configuration`类中也注册了一些别名，代码如下：
```java
typeAliasRegistry.registerAlias("JDBC", JdbcTransactionFactory.class);
typeAliasRegistry.registerAlias("MANAGED", ManagedTransactionFactory.class);

typeAliasRegistry.registerAlias("JNDI", JndiDataSourceFactory.class);
typeAliasRegistry.registerAlias("POOLED", PooledDataSourceFactory.class);
typeAliasRegistry.registerAlias("UNPOOLED", UnpooledDataSourceFactory.class);

typeAliasRegistry.registerAlias("PERPETUAL", PerpetualCache.class);
typeAliasRegistry.registerAlias("FIFO", FifoCache.class);
typeAliasRegistry.registerAlias("LRU", LruCache.class);
typeAliasRegistry.registerAlias("SOFT", SoftCache.class);
typeAliasRegistry.registerAlias("WEAK", WeakCache.class);

typeAliasRegistry.registerAlias("DB_VENDOR", VendorDatabaseIdProvider.class);

typeAliasRegistry.registerAlias("XML", XMLLanguageDriver.class);
typeAliasRegistry.registerAlias("RAW", RawLanguageDriver.class);

typeAliasRegistry.registerAlias("SLF4J", Slf4jImpl.class);
typeAliasRegistry.registerAlias("COMMONS_LOGGING", JakartaCommonsLoggingImpl.class);
typeAliasRegistry.registerAlias("LOG4J", Log4jImpl.class);
typeAliasRegistry.registerAlias("LOG4J2", Log4j2Impl.class);
typeAliasRegistry.registerAlias("JDK_LOGGING", Jdk14LoggingImpl.class);
typeAliasRegistry.registerAlias("STDOUT_LOGGING", StdOutImpl.class);
typeAliasRegistry.registerAlias("NO_LOGGING", NoLoggingImpl.class);

typeAliasRegistry.registerAlias("CGLIB", CglibProxyFactory.class);
typeAliasRegistry.registerAlias("JAVASSIST", JavassistProxyFactory.class);
```
上面有2行如下：
```
typeAliasRegistry.registerAlias("JDBC", JdbcTransactionFactory.class);
typeAliasRegistry.registerAlias("POOLED", PooledDataSourceFactory.class);
```
上面这2行，注册了2个别名，别名和类型映射关系如下：
```
JDBC -> JdbcTransactionFactory
POOLED -> PooledDataSourceFactory
```
上面这2个对象，大家应该比较熟悉吧，Mybatis全局配置文件（chat03\src\main\resources\demo1\mybatis-config.xml）中用到过，再去看一下，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690549556977-276067f4-9707-44f9-a93a-6c420fb2410c.jpeg#averageHue=%23ebe1d3&clientId=u8a864d1a-b908-4&from=paste&id=u10e0c7d6&originHeight=658&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u88e6acab-d1ef-47ba-b821-339ceb1d9c0&title=)<br />上面2个红框的是不是就是上面注册的2个类型，上面xml中写的是完整类型名称，可以将其改为别名的方式也是可以的，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690549556954-36828f06-2e0e-4531-9ce2-54c801d8ce4b.png#averageHue=%23f9f7f4&clientId=u8a864d1a-b908-4&from=paste&id=u01d9b6fc&originHeight=306&originWidth=821&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u717b0140-d31a-4e1d-9647-11eff981b80&title=)<br />来运行com.javacode2018.chat03.demo1.UserMapperTest#getUserList，看一下能否正常运行，输出如下：
```
44:10.886 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
44:10.929 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
44:10.947 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
44:10.948 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
44:10.950 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
44:10.950 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
44:10.950 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
很好，一切正常的。
<a name="q1DLe"></a>
### 别名的原理
Mybatis允许给某种类型注册一个别名，别名和类型之间会建立映射关系，这个映射关系存储在一个map对象中，key为别名的名称，value为具体的类型，当通过一个名称访问某种类型的时候，Mybatis根据类型的名称，先在别名和类型映射的map中按照key进行查找，如果找到了直接返回对应的类型，如果没找到，会将这个名称当做完整的类名去解析成Class对象，如果这2步解析都无法识别这种类型，就会报错。<br />**Mybatis和别名相关的操作都位于**`**org.apache.ibatis.type.TypeAliasRegistry**`**类中，包含别名的注册、解析等各种操作。**<br />来看一下别名解析的方法，如下：
```java
public <T> Class<T> resolveAlias(String string) {
    try {
        if (string == null) {
            return null;
        }
        // issue #748
        String key = string.toLowerCase(Locale.ENGLISH);
        Class<T> value;
        if (typeAliases.containsKey(key)) {
            value = (Class<T>) typeAliases.get(key);
        } else {
            value = (Class<T>) Resources.classForName(string);
        }
        return value;
    } catch (ClassNotFoundException e) {
        throw new TypeException("Could not resolve type alias '" + string + "'.  Cause: " + e, e);
    }
}
```
有一个`typeAliases`对象，看一下其定义：
```java
private final Map<String, Class<?>> typeAliases = new HashMap<>();
```
这个对象就是存放别名和具体类型映射关系的，从上面代码中可以看出，通过传入的参数解析对应的类型的时候，会先从typeAliases中查找，如果找不到会调用下面代码：
```java
value = (Class<T>) Resources.classForName(string);
```
上面这个方法里面具体是使用下面代码去通过名称解析成类型的：
```java
Class.forName(类名完整名称)
```
Class.forName大家应该是很熟悉的，可以获取一个字符串对应的Class对象，如果找不到这个对象，会报错。
<a name="CDJ1X"></a>
### 别名使用建议
别名的方式可以简化类型的写法，原本很长一串的UserModel对象，现在只用写个user就行了，用起来是不是挺爽的？<br />从写法上面来说，确实省了一些代码，但是从维护上面来讲，不是很方便。<br />如Mapper xml直接写别名，看代码的时候，很难知道这个别名对应的具体类型，还需要去注册的地方找一下，不是太方便，如果在idea中写完整的类名，还可以按住Ctrl健，然后用鼠标左键点击类型直接可以跳到对应的类定义中去，如果使用别名是无法导航过去的。<br />整体上来说开发和看代码都不是太方便，只是写法上比价简单。<br />**所以建议自定义的类尽量别使用别名，而对Mybatis中内置的一些别名需要知道。**
<a name="Ebhwm"></a>
## 属性配置文件详解
大家看一下chat03\src\main\resources\demo1\mybatis-config.xml中下面这一部分的配置：
```xml
<dataSource type="POOLED">
  <property name="driver" value="com.mysql.jdbc.Driver"/>
  <property name="url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
  <property name="username" value="root"/>
  <property name="password" value="root123"/>
</dataSource>
```
这个连接数据库的配置是直接写在Mybatis全局配置文件中的，上面这是本地测试库的db信息，上线之后，需要修改为线上的db配置信息，db配置信息一般由运维去修改，让运维去修改这个xml配置文件？<br />这样不是太好，通常将一些需要运维修改的配置信息（如：db配置、邮件配置、redis配置等等各种配置）放在一个properties配文件中，然后上线时，只需要运维去修改这个配置文件就可以了，根本不用他们去修改和代码相关的文件。<br />Mybatis也支持通过外部properties文件来配置一些属性信息。<br />Mybatis配置属性信息有3种方式。
<a name="H25LH"></a>
### 方式1：property元素中定义属性
<a name="ut7h8"></a>
#### 属性定义
Mybatis全局配置文件中通过properties元素来定义属性信息，如下：
```xml
<configuration>
  <properties>
    <property name="属性名称" value="属性对应的值"/>
  </properties>
</configuration>
```
上面通过property元素的方式进行配置属性信息：<br />name：属性的名称<br />value：属性的值。<br />如：
```xml
<property name="jdbc.driver" value="com.mysql.jdbc.Driver"/>
```
<a name="pxzHY"></a>
#### 使用`${属性名称}`引用属性的值
属性已经定义好了，可以通过${属性名称}引用定义好的属性的值，如：
```xml
<property name="driver" value="${jdbc.driver}"/>
```
<a name="nRwkn"></a>
#### 案例
在demo1/mapper/mybatis-config.xml的configuration元素中加入下面配置：
```xml
<properties>
  <property name="jdbc.driver" value="com.mysql.jdbc.Driver"/>
  <property name="jdbc.url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
  <property name="jdbc.username" value="root"/>
  <property name="jdbc.password" value="root123"/>
</properties>
```
修改datasource的配置：
```xml
<dataSource type="POOLED">
  <property name="driver" value="${jdbc.driver}"/>
  <property name="url" value="${jdbc.url}"/>
  <property name="username" value="${jdbc.username}"/>
  <property name="password" value="${jdbc.password}"/>
</dataSource>
```
运行com.javacode2018.chat03.demo1.UserMapperTest#getUserList，如下：
```
40:22.274 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
40:22.307 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
40:22.330 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
40:22.331 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
40:22.332 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
40:22.332 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
40:22.332 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
运行正常。
<a name="NNkfj"></a>
### 方式2：resource引入配置文件
方式1中，配置文件还是写在全局配置文件中，Mybatis支持从外部引入配置文件，可以把配置文件写在其他外部文件中，然后进行引入。
<a name="iU9hr"></a>
#### 引入classes路径中的配置文件
```xml
<configuration>
  <properties resource="配置文件路径"/>
</configuration>
```
properties元素有个resource属性，值为配置文件相对于classes的路径，配置文件一般放在src/main/resource目录，这个目录的文件编译之后会放在classes路径中。
<a name="DHrJF"></a>
#### 案例
下面将上面db的配置放在外部的config.properties文件中。<br />在chat03\src\main\resources\demo1目录新建一个配置文件config.properties，内容如下：
```
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8
jdbc.username=root
jdbc.password=root123
```
demo1/mapper/mybatis-config.xml中引入上面配置文件：
```xml
<!-- 引入外部配置文件 -->
<properties resource="demo1/mapper/config.properties"/>
```
目前demo1/mapper/mybatis-config.xml文件内容如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!-- 引入外部配置文件 -->
    <properties resource="demo1/config.properties"/>
    <typeAliases>
        <package name="com.javacode2018.chat03.demo1"/>
    </typeAliases>
    <!-- 环境配置，可以配置多个环境 -->
    <environments default="chat04-demo1">
        <!-- 
            environment用来对某个环境进行配置
            id：环境标识，唯一
         -->
        <environment id="chat04-demo1">
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
        <mapper resource="demo1/mapper/UserMapper.xml"/>
    </mappers>
</configuration>
```
运行com.javacode2018.chat03.demo1.UserMapperTest#getUserList，如下：
```
57:40.405 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
57:40.436 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
57:40.454 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
57:40.455 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
57:40.457 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
57:40.457 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
57:40.457 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
运行正常。
<a name="m0oQV"></a>
### 方式3：url的方式引入远程配置文件
Mybatis还提供了引入远程配置文件的方式，如下：
```xml
<properties url="远程配置文件的路径" />
```
这次还是使用properties元素，不过使用的是url属性，如：
```xml
<properties url="http://properties.com/properties/config.properties" />
```
这种方式的案例就不提供了，有兴趣的可以自己去玩玩。
<a name="i8y0a"></a>
### 属性配置文件使用建议
上面说了3中方式，第2中方式是比较常见的做法，建议大家可以使用第二种方式来引入外部资源配置文件。
<a name="UEzYR"></a>
### 问题
**如果3种方式如果都写了，Mybatis会怎么走？**<br />下面修改一下resources/demo1/mybatis-config.xml，使用第一种方式定义属性，如下：
```xml
<properties>
  <property name="jdbc.driver" value="com.mysql.jdbc.Driver"/>
  <property name="jdbc.url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
  <property name="jdbc.username" value="root"/>
  <property name="jdbc.password" value="root"/>
</properties>
```
将password的值改为了root，正确的是root123，运行测试用例，报错如下：
```
org.apache.ibatis.exceptions.PersistenceException: 
### Error querying database.  Cause: java.sql.SQLException: Access denied for user 'root'@'localhost' (using password: YES)
### The error may exist in demo1/mapper/UserMapper.xml
### The error may involve com.javacode2018.chat03.demo1.UserMapper.getUserList
### The error occurred while executing a query
### Cause: java.sql.SQLException: Access denied for user 'root'@'localhost' (using password: YES)

    at org.apache.ibatis.exceptions.ExceptionFactory.wrapException(ExceptionFactory.java:30)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectList(DefaultSqlSession.java:149)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectList(DefaultSqlSession.java:140)
    at org.apache.ibatis.binding.MapperMethod.executeForMany(MapperMethod.java:147)
    at org.apache.ibatis.binding.MapperMethod.execute(MapperMethod.java:80)
    at org.apache.ibatis.binding.MapperProxy.invoke(MapperProxy.java:93)
    at com.sun.proxy.$Proxy6.getUserList(Unknown Source)
    at com.javacode2018.chat03.demo1.UserMapperTest.getUserList(UserMapperTest.java:38)
    at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
    at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
    at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
    at java.lang.reflect.Method.invoke(Method.java:498)
    at org.junit.runners.model.FrameworkMethod$1.runReflectiveCall(FrameworkMethod.java:50)
    at org.junit.internal.runners.model.ReflectiveCallable.run(ReflectiveCallable.java:12)
    at org.junit.runners.model.FrameworkMethod.invokeExplosively(FrameworkMethod.java:47)
    at org.junit.internal.runners.statements.InvokeMethod.evaluate(InvokeMethod.java:17)
    at org.junit.internal.runners.statements.RunBefores.evaluate(RunBefores.java:26)
    at org.junit.runners.ParentRunner.runLeaf(ParentRunner.java:325)
    at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:78)
    at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:57)
    at org.junit.runners.ParentRunner$3.run(ParentRunner.java:290)
    at org.junit.runners.ParentRunner$1.schedule(ParentRunner.java:71)
    at org.junit.runners.ParentRunner.runChildren(ParentRunner.java:288)
    at org.junit.runners.ParentRunner.access$000(ParentRunner.java:58)
    at org.junit.runners.ParentRunner$2.evaluate(ParentRunner.java:268)
    at org.junit.runners.ParentRunner.run(ParentRunner.java:363)
    at org.junit.runner.JUnitCore.run(JUnitCore.java:137)
    at com.intellij.junit4.JUnit4IdeaTestRunner.startRunnerWithArgs(JUnit4IdeaTestRunner.java:68)
    at com.intellij.rt.execution.junit.IdeaTestRunner$Repeater.startRunnerWithArgs(IdeaTestRunner.java:51)
    at com.intellij.rt.execution.junit.JUnitStarter.prepareStreamsAndStart(JUnitStarter.java:242)
    at com.intellij.rt.execution.junit.JUnitStarter.main(JUnitStarter.java:70)
Caused by: java.sql.SQLException: Access denied for user 'root'@'localhost' (using password: YES)
```
提示密码错误。<br />下面将第2种方式也加入，修改配置：
```xml
<properties resource="demo1/config.properties">
  <property name="jdbc.driver" value="com.mysql.jdbc.Driver"/>
  <property name="jdbc.url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
  <property name="jdbc.username" value="root"/>
  <property name="jdbc.password" value="root"/>
</properties>
```
再运行一下测试用例，如下：
```
18:59.436 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
18:59.462 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - ==> Parameters: 
18:59.481 [main] DEBUG c.j.c.demo1.UserMapper.getUserList - <==      Total: 4
18:59.482 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
18:59.485 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
18:59.485 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
18:59.485 [main] INFO  c.j.chat03.demo1.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
这次正常了。<br />**可以看出方式1和方式2都存在的时候，方式2的配置会覆盖方式1的配置。**<br />Mybatis这块的源码在org.apache.ibatis.builder.xml.XMLConfigBuilder#propertiesElement方法中，如下：
```java
private void propertiesElement(XNode context) throws Exception {
    if (context != null) {
        Properties defaults = context.getChildrenAsProperties();
        String resource = context.getStringAttribute("resource");
        String url = context.getStringAttribute("url");
        if (resource != null && url != null) {
            throw new BuilderException("The properties element cannot specify both a URL and a resource based property file reference.  Please specify one or the other.");
        }
        if (resource != null) {
            defaults.putAll(Resources.getResourceAsProperties(resource));
        } else if (url != null) {
            defaults.putAll(Resources.getUrlAsProperties(url));
        }
        Properties vars = configuration.getVariables();
        if (vars != null) {
            defaults.putAll(vars);
        }
        parser.setVariables(defaults);
        configuration.setVariables(defaults);
    }
}
```
**从上面代码中也可以看出，如果方式2和方式3都存在的时候，方式3会失效，Mybatis会先读取方式1的配置，然后读取方式2或者方式3的配置，会将1中相同的配置给覆盖。**
<a name="Bm9ky"></a>
## Mybatis中引入mapper的3种方式
mapper xml文件是非常重要的，写的sql基本上都在里面，使用Mybatis开发项目的时候，和Mybatis相关的大部分代码就是写sql，基本上都是和mapper xml打交道。<br />编写好的mapper xml需要让Mybatis知道，怎么让Mybatis知道呢？<br />可以通过Mybatis全局配置文件进行引入，主要有3种方式。
<a name="NSlVQ"></a>
### 方式1：使用mapper resouce属性注册mapper xml文件
目前所涉及到的各种例子都是采用的这种方式，使用下面的方法进行引入：
```xml
<mappers>
  <mapper resource="Mapper xml的路径（相对于classes的路径）"/>
</mappers>
```
再来说一下这种方式的一些注意点：

1. 一般情况下，会创建一个和Mapper xml中namespace同名的Mapper接口，Mapper接口会和Mapper xml文件进行绑定
2. Mybatis加载mapper xml的时候，会去查找namespace对应的Mapper接口，然后进行注册，可以通过Mapper接口的方式去访问Mapper xml中的具体操作
3. Mapper xml和Mapper 接口配合的方式是比较常见的做法，也是强烈建议大家使用的
<a name="laBzJ"></a>
### 方式2：使用mapper class属性注册Mapper接口
<a name="c1EiT"></a>
#### 引入Mapper接口
Mybatis全局配置文件中引入mapper接口，如下：
```xml
<mappers>
  <mapper class="接口的完整类名" />
</mappers>
```
**这种情况下，Mybatis会去加载class对应的接口，然后还会去加载和这个接口同一个目录的同名的xml文件。**<br />如：
```xml
<mappers>
  <mapper class="com.javacode2018.chat03.demo1.UserMapper" />
</mappers>
```
上面这种写法，Mybatis会自动去注册UserMapper接口，还会去查找下面的文件：
```
com/javacode2018/chat03/demo1/UserMapper.xml
```
大家以后开发项目的时候估计也会看到这种写法，Mapper接口和Mapper xml文件放在同一个包中。
<a name="hJMXx"></a>
#### 案例
下面重新创建一个案例，都放在demo2包中。<br />新建com.javacode2018.chat03.demo2.UserModel，如下：
```java
package com.javacode2018.chat03.demo2;

import lombok.*;
import org.apache.ibatis.type.Alias;

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
新建com.javacode2018.chat03.demo2.UserMapper，如下：
```java
package com.javacode2018.chat03.demo2;

import java.util.List;

public interface UserMapper {

    List<UserModel> getUserList();
}
```
chat03\src\main\java\com\javacode2018\chat03\demo2中创建UserMapper.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.chat03.demo2.UserMapper">
  <!-- select用来定义一个查询操作
  id：操作的具体标识
  resultType：指定查询结果保存的类型
  -->
  <select id="getUserList" resultType="com.javacode2018.chat03.demo2.UserModel">
    <![CDATA[
    SELECT * FROM t_user
    ]]>
  </select>

</mapper>
```
下面重点来了。<br />创建Mybatis全局配置文件，在chat03\src\main\resources\demo2目录中创建mybatis-config.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <properties>
        <property name="jdbc.driver" value="com.mysql.jdbc.Driver"/>
        <property name="jdbc.url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
        <property name="jdbc.username" value="root"/>
        <property name="jdbc.password" value="root123"/>
    </properties>
    <!-- 环境配置，可以配置多个环境 -->
    <environments default="chat04-demo2">
        <!-- 
            environment用来对某个环境进行配置
            id：环境标识，唯一
         -->
        <environment id="chat04-demo2">
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

</configuration>
```
chat03\src\test\java目录创建测试用例com.javacode2018.chat03.demo2.UserMapperTest，如下：
```java
package com.javacode2018.chat03.demo2;

import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

@Slf4j
public class UserMapperTest {
    private SqlSessionFactory sqlSessionFactory;

    @Before
    public void before() throws IOException {
        //指定mybatis全局配置文件
        String resource = "demo2/mybatis-config.xml";
        //读取全局配置文件
        InputStream inputStream = Resources.getResourceAsStream(resource);
        //构建SqlSessionFactory对象
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        this.sqlSessionFactory = sqlSessionFactory;
    }

    @Test
    public void getUserList() {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper mapper = sqlSession.getMapper(UserMapper.class);
            //执行查询操作
            List<UserModel> userModelList = mapper.getUserList();
            userModelList.forEach(item -> {
                log.info("{}", item);
            });
        }
    }

}
```
注意这次上面使用的是demo2/mybatis-config.xml配置文件。<br />先来看一下项目结构，4个文件：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690549615886-24a139d7-3e13-4d47-b70f-b6be514a2092.png#averageHue=%23fbf6d7&clientId=u8a864d1a-b908-4&from=paste&id=u7e617579&originHeight=541&originWidth=530&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7b137445-fe8a-4531-9bfc-df6433bcf00&title=)<br />注意一下UserMapper接口所在的包中有个同名的UserMapper.xml文件，这个如果按照方式2中所说的，会自动加载。<br />下面来运行一下com.javacode2018.chat03.demo2.UserMapperTest#getUserList，输出：
```
org.apache.ibatis.binding.BindingException: Type interface com.javacode2018.chat03.demo2.UserMapper is not known to the MapperRegistry.

    at org.apache.ibatis.binding.MapperRegistry.getMapper(MapperRegistry.java:47)
    at org.apache.ibatis.session.Configuration.getMapper(Configuration.java:779)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.getMapper(DefaultSqlSession.java:291)
    at com.javacode2018.chat03.demo2.UserMapperTest.getUserList(UserMapperTest.java:36)
    at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
    at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
    at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
    at java.lang.reflect.Method.invoke(Method.java:498)
    at org.junit.runners.model.FrameworkMethod$1.runReflectiveCall(FrameworkMethod.java:50)
    at org.junit.internal.runners.model.ReflectiveCallable.run(ReflectiveCallable.java:12)
    at org.junit.runners.model.FrameworkMethod.invokeExplosively(FrameworkMethod.java:47)
    at org.junit.internal.runners.statements.InvokeMethod.evaluate(InvokeMethod.java:17)
    at org.junit.internal.runners.statements.RunBefores.evaluate(RunBefores.java:26)
    at org.junit.runners.ParentRunner.runLeaf(ParentRunner.java:325)
    at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:78)
    at org.junit.runners.BlockJUnit4ClassRunner.runChild(BlockJUnit4ClassRunner.java:57)
    at org.junit.runners.ParentRunner$3.run(ParentRunner.java:290)
    at org.junit.runners.ParentRunner$1.schedule(ParentRunner.java:71)
    at org.junit.runners.ParentRunner.runChildren(ParentRunner.java:288)
    at org.junit.runners.ParentRunner.access$000(ParentRunner.java:58)
    at org.junit.runners.ParentRunner$2.evaluate(ParentRunner.java:268)
    at org.junit.runners.ParentRunner.run(ParentRunner.java:363)
    at org.junit.runner.JUnitCore.run(JUnitCore.java:137)
    at com.intellij.junit4.JUnit4IdeaTestRunner.startRunnerWithArgs(JUnit4IdeaTestRunner.java:68)
    at com.intellij.rt.execution.junit.IdeaTestRunner$Repeater.startRunnerWithArgs(IdeaTestRunner.java:51)
    at com.intellij.rt.execution.junit.JUnitStarter.prepareStreamsAndStart(JUnitStarter.java:242)
    at com.intellij.rt.execution.junit.JUnitStarter.main(JUnitStarter.java:70)
```
从输出中可以看到，UserMapper找不到。<br />去看一下demo2/mybatis-config.xml这个配置文件，这个文件中需要使用方式2引入UserMapper接口，在demo2/mybatis-config.xml中加入下面配置：
```xml
<mappers>
  <mapper class="com.javacode2018.chat03.demo2.UserMapper" />
</mappers>
```
再运行一下，还是报错，如下，还是找不到对应的UserMapper：
```
org.apache.ibatis.binding.BindingException: Invalid bound statement (not found): com.javacode2018.chat03.demo2.UserMapper.getUserList

    at org.apache.ibatis.binding.MapperMethod$SqlCommand.<init>(MapperMethod.java:235)
    at org.apache.ibatis.binding.MapperMethod.<init>(MapperMethod.java:53)
```
还是有问题，看一下target/classes中demo2包的内容，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690549615822-da868ed1-39a6-4517-a41b-62f31dab92de.png#averageHue=%23fefee3&clientId=u8a864d1a-b908-4&from=paste&id=ub0430c8f&originHeight=419&originWidth=526&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud6e2769e-219a-42fe-a20f-7ee08792458&title=)<br />编译之后的文件中少了UserMapper.xml，这个和Maven有关，Maven编译src/java代码的时候，默认只会对java文件进行编译然后放在target/classes目录，需要在chat03/pom.xml中加入下面配置：
```xml
<build>
  <resources>
    <resource>
      <directory>${project.basedir}/src/main/java</directory>
      <includes>
        <include>**/*.xml</include>
      </includes>
    </resource>
    <resource>
      <directory>${project.basedir}/src/main/resources</directory>
      <includes>
        <include>**/*</include>
      </includes>
    </resource>
  </resources>
</build>
```
最终chat03/pom.xml内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <parent>
        <artifactId>mybatis-series</artifactId>
        <groupId>com.javacode2018</groupId>
        <version>1.0-SNAPSHOT</version>
    </parent>
    <modelVersion>4.0.0</modelVersion>

    <artifactId>chat03</artifactId>

    <dependencies>
        <!-- mybatis依赖 -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
        </dependency>
        <!-- mysql 驱动 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
        </dependency>
        <!-- lombok支持 -->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
        </dependency>
        <!-- 单元测试junit支持 -->
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
        </dependency>
        <!-- 引入logback用来输出日志 -->
        <dependency>
            <groupId>ch.qos.logback</groupId>
            <artifactId>logback-classic</artifactId>
        </dependency>
    </dependencies>

    <build>
        <resources>
            <resource>
                <directory>${project.basedir}/src/main/java</directory>
                <includes>
                    <include>**/*.xml</include>
                </includes>
            </resource>
            <resource>
                <directory>${project.basedir}/src/main/resources</directory>
                <includes>
                    <include>**/*</include>
                </includes>
            </resource>
        </resources>
    </build>

</project>
```
加了这个之后UserMapper.xml就会被放到target的classes中去了，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690549615894-44a61d72-2cbf-4c22-9671-d0aed2765bfb.png#averageHue=%23fefde1&clientId=u8a864d1a-b908-4&from=paste&id=uafda4ea0&originHeight=484&originWidth=529&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua75b06e1-d16a-4204-aecb-13ff854b32b&title=)<br />再次运行一下测试用例com.javacode2018.chat03.demo2.UserMapperTest#getUserList，效果如下：
```
24:37.814 [main] DEBUG c.j.c.demo2.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
24:37.852 [main] DEBUG c.j.c.demo2.UserMapper.getUserList - ==> Parameters: 
24:37.875 [main] DEBUG c.j.c.demo2.UserMapper.getUserList - <==      Total: 4
24:37.876 [main] INFO  c.j.chat03.demo2.UserMapperTest - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
24:37.879 [main] INFO  c.j.chat03.demo2.UserMapperTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
24:37.879 [main] INFO  c.j.chat03.demo2.UserMapperTest - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
24:37.879 [main] INFO  c.j.chat03.demo2.UserMapperTest - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
```
这次正常了。
<a name="Dx9QI"></a>
#### 源码
方式2对应的源码大家可以去看下面这个方法：
```
org.apache.ibatis.builder.xml.XMLConfigBuilder#mapperElement
```
方法中会去加载mapper元素中class属性指定的Mapper接口，然后进行注册，随后会在接口同目录中查找同名的mapper xml文件，将解析这个xml文件，如果mapper xml文件不存在，也不会报错，源码还是比较简单的，大家可以去看一下，加深理解。
<a name="RjiBM"></a>
### 方式3：使用package元素批量注册Mapper接口
<a name="JiuIv"></a>
#### 批量注册Mapper接口
上面说2种方式都是一个个注册mapper的，如果写了很多mapper，是否能够批量注册呢？<br />Mybatis提供了扫描包批量注册的方式，需要在Mybatis全局配置文件中加入下面配置：
```xml
<mappers>
  <package name="需要扫描的包" />
</mappers>
```
Mybatis会扫描package元素中name属性指定的包及子包中的所有接口，将其当做Mapper 接口进行注册，所以一般会创建一个mapper包，里面放Mapper接口和同名的Mapper xml文件。<br />大家来看一个案例，理解一下。
<a name="NmurL"></a>
#### 案例
这个案例中将对t_user、t_order两个表进行查询操作，采用方式3中的package批量引入mapper 接口和xml文件。<br />所有代码放在demo3包中，大家先看下文件所在的目录：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690549615859-9377ea29-442e-4cf0-86fb-f4515cfb601d.png#averageHue=%23eef9e5&clientId=u8a864d1a-b908-4&from=paste&id=u2e0ac993&originHeight=817&originWidth=528&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4d93f839-5329-4754-8a67-aec16600583&title=)<br />创建UserModel类，如下：
```java
package com.javacode2018.chat03.demo3.model;

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
创建OrderModel类，如下：
```java
package com.javacode2018.chat03.demo3.model;

import lombok.*;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Builder
@ToString
public class OrderModel {
    private Long id;
    private Long user_id;
    private Double price;
}
```
创建UserMapper接口，如下：
```java
package com.javacode2018.chat03.demo3.mapper;

import com.javacode2018.chat03.demo3.model.UserModel;

import java.util.List;

public interface UserMapper {

    List<UserModel> getList();
}
```
创建OrderMapper接口，如下：
```java
package com.javacode2018.chat03.demo3.mapper;

import com.javacode2018.chat03.demo3.model.OrderModel;
import com.javacode2018.chat03.demo3.model.UserModel;

import java.util.List;

public interface OrderMapper {

    List<OrderModel> getList();
}
```
创建UserMapper.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.chat03.demo3.mapper.OrderMapper">
  <!-- select用来定义一个查询操作
  id：操作的具体标识
  resultType：指定查询结果保存的类型
  -->
  <select id="getList" resultType="com.javacode2018.chat03.demo3.model.OrderModel">
    <![CDATA[
    SELECT * FROM t_order
    ]]>
  </select>

</mapper>
```
上面写了一个查询t_user数据的sql<br />创建OrderMapper.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.chat03.demo3.mapper.OrderMapper">
  <!-- select用来定义一个查询操作
  id：操作的具体标识
  resultType：指定查询结果保存的类型
  -->
  <select id="getList" resultType="com.javacode2018.chat03.demo3.model.OrderModel">
    <![CDATA[
    SELECT * FROM t_order
    ]]>
  </select>

</mapper>
```
上面写了一个查询t_order数据的sql<br />创建resources/demo3/mybatis-config.xml配置文件，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
  <properties>
    <property name="jdbc.driver" value="com.mysql.jdbc.Driver"/>
    <property name="jdbc.url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
    <property name="jdbc.username" value="root"/>
    <property name="jdbc.password" value="root123"/>
  </properties>
  <!-- 环境配置，可以配置多个环境 -->
  <environments default="chat04-demo3">
    <!-- 
    environment用来对某个环境进行配置
    id：环境标识，唯一
    -->
    <environment id="chat04-demo3">
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
    <package name="com.javacode2018.chat03.demo3.mapper"/>
  </mappers>
</configuration>
```
注意这次使用package来让Mybatis加载com.javacode2018.chat03.demo3.mapper包下面所有的Mapper接口和Mapper xml文件。<br />创建测试用例Demo3Test，如下：
```java
package com.javacode2018.chat03.demo3;

import com.javacode2018.chat03.demo3.mapper.OrderMapper;
import com.javacode2018.chat03.demo3.mapper.UserMapper;
import com.javacode2018.chat03.demo3.model.OrderModel;
import com.javacode2018.chat03.demo3.model.UserModel;
import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

@Slf4j
public class Demo3Test {
    private SqlSessionFactory sqlSessionFactory;

    @Before
    public void before() throws IOException {
        //指定mybatis全局配置文件
        String resource = "demo3/mybatis-config.xml";
        //读取全局配置文件
        InputStream inputStream = Resources.getResourceAsStream(resource);
        //构建SqlSessionFactory对象
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        this.sqlSessionFactory = sqlSessionFactory;
    }

    @Test
    public void test() {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
            //执行查询操作
            List<UserModel> userModelList = userMapper.getList();
            userModelList.forEach(item -> {
                log.info("{}", item);
            });

            log.info("----------------------------------");
            OrderMapper orderMapper = sqlSession.getMapper(OrderMapper.class);
            //执行查询操作
            List<OrderModel> orderModelList = orderMapper.getList();
            orderModelList.forEach(item -> {
                log.info("{}", item);
            });
        }
    }

}
```
运行com.javacode2018.chat03.demo3.Demo3Test#test，输出如下：
```
48:39.280 [main] DEBUG c.j.c.d.mapper.UserMapper.getList - ==>  Preparing: SELECT * FROM t_user 
48:39.315 [main] DEBUG c.j.c.d.mapper.UserMapper.getList - ==> Parameters: 
48:39.339 [main] DEBUG c.j.c.d.mapper.UserMapper.getList - <==      Total: 4
48:39.340 [main] INFO  c.j.chat03.demo3.Demo3Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
48:39.343 [main] INFO  c.j.chat03.demo3.Demo3Test - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
48:39.343 [main] INFO  c.j.chat03.demo3.Demo3Test - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
48:39.343 [main] INFO  c.j.chat03.demo3.Demo3Test - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
48:39.343 [main] INFO  c.j.chat03.demo3.Demo3Test - ----------------------------------
48:39.344 [main] DEBUG c.j.c.d.mapper.OrderMapper.getList - ==>  Preparing: SELECT * FROM t_order 
48:39.345 [main] DEBUG c.j.c.d.mapper.OrderMapper.getList - ==> Parameters: 
48:39.351 [main] DEBUG c.j.c.d.mapper.OrderMapper.getList - <==      Total: 2
48:39.351 [main] INFO  c.j.chat03.demo3.Demo3Test - OrderModel(id=1, user_id=1, price=88.88)
48:39.351 [main] INFO  c.j.chat03.demo3.Demo3Test - OrderModel(id=2, user_id=2, price=666.66)
```
这种批量的方式是不是用着挺爽的，不过有点不是太好，mapper xml和mapper接口放在了一个目录中，目录中既有java代码又有xml文件，看起来也挺别扭的，其实可以这样：<br />一般将配置文件放在resource目录，可以在resource目录中创建下面子目录：
```
com/javacode2018/chat03/demo3/mapper
```
然后将com.javacode2018.chat03.demo3.mapper中的2个xml文件移到上面新创建的目录中去，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690549615868-3bf19414-5154-4c21-a0d8-4c28973b5e7e.png#averageHue=%23d2a668&clientId=u8a864d1a-b908-4&from=paste&id=u825b30fa&originHeight=400&originWidth=543&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3d7e42cb-83cd-456a-9c5f-c0e4c10d469&title=)<br />在去运行一下com.javacode2018.chat03.demo3.Demo3Test#test，输出如下：
```
56:22.669 [main] DEBUG c.j.c.d.mapper.UserMapper.getList - ==>  Preparing: SELECT * FROM t_user 
56:22.700 [main] DEBUG c.j.c.d.mapper.UserMapper.getList - ==> Parameters: 
56:22.721 [main] DEBUG c.j.c.d.mapper.UserMapper.getList - <==      Total: 4
56:22.722 [main] INFO  c.j.chat03.demo3.Demo3Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
56:22.725 [main] INFO  c.j.chat03.demo3.Demo3Test - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
56:22.725 [main] INFO  c.j.chat03.demo3.Demo3Test - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
56:22.725 [main] INFO  c.j.chat03.demo3.Demo3Test - UserModel(id=4, name=林志玲, age=45, salary=88888.88, sex=2)
56:22.725 [main] INFO  c.j.chat03.demo3.Demo3Test - ----------------------------------
56:22.727 [main] DEBUG c.j.c.d.mapper.OrderMapper.getList - ==>  Preparing: SELECT * FROM t_order 
56:22.727 [main] DEBUG c.j.c.d.mapper.OrderMapper.getList - ==> Parameters: 
56:22.732 [main] DEBUG c.j.c.d.mapper.OrderMapper.getList - <==      Total: 2
56:22.732 [main] INFO  c.j.chat03.demo3.Demo3Test - OrderModel(id=1, user_id=1, price=88.88)
56:22.732 [main] INFO  c.j.chat03.demo3.Demo3Test - OrderModel(id=2, user_id=2, price=666.66)
```
也是可以的。
<a name="vB6y2"></a>
### 使用注意
**方式3会扫描指定包中所有的接口，把这些接口作为Mapper接口进行注册，扫描到的类型只要是接口就会被注册，所以指定的包中通常只放Mapper接口，避免存放一些不相干的类或者接口。**
<a name="N2Rat"></a>
## 关于配置和源码
本次讲解到的一些配置都是在Mybatis全局配置文件中进行配置的，这些元素配置是有先后顺序的，具体元素是在下面的dtd文件中定义的：http://mybatis.org/dtd/mybatis-3-config.dtd<br />建议大家去看一下这个dtd配置文件。<br />Mybatis解析这个配置文件的入口是在下面的方法中：
```
org.apache.ibatis.builder.xml.XMLConfigBuilder#parseConfiguration
```
代码的部分实现如下：
```java
private void parseConfiguration(XNode root) {
    try {
        //issue #117 read properties first
        propertiesElement(root.evalNode("properties"));
        Properties settings = settingsAsProperties(root.evalNode("settings"));
        loadCustomVfs(settings);
        loadCustomLogImpl(settings);
        typeAliasesElement(root.evalNode("typeAliases"));
        pluginElement(root.evalNode("plugins"));
        objectFactoryElement(root.evalNode("objectFactory"));
        objectWrapperFactoryElement(root.evalNode("objectWrapperFactory"));
        reflectorFactoryElement(root.evalNode("reflectorFactory"));
        settingsElement(settings);
        // read it after objectFactory and objectWrapperFactory issue #631
        environmentsElement(root.evalNode("environments"));
        databaseIdProviderElement(root.evalNode("databaseIdProvider"));
        typeHandlerElement(root.evalNode("typeHandlers"));
        mapperElement(root.evalNode("mappers"));
    } catch (Exception e) {
        throw new BuilderException("Error parsing SQL Mapper Configuration. Cause: " + e, e);
    }
}
```
可以看到Mybatis启动的时候会按顺序加载上面的标签。
<a name="nANQA"></a>
## 总结

1. 掌握别名注册的3种方式，建议大家尽量少使用自定义别名
2. 掌握属性配置3种方式
3. 掌握mapper注册的3种方式及需要注意的地方
