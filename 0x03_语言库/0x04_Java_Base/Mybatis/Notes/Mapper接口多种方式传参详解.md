Mybatis<br />使用Mybatis开发项目的中，基本上都是使用mapper接口的方式来执行db操作，下面来看一下mapper接口传递参数的几种方式及需要注意的地方。
<a name="KmDCG"></a>
## 传递一个参数
<a name="JZHfO"></a>
### 用法
Mapper接口方法中只有一个参数，如：
```java
UserModel getByName(String name);
```
Mapper xml引用这个name参数：
```
#{任意合法名称}
```
如：#{name}、#{val}、${x}等等写法都可以引用上面name参数的值。
<a name="CoZIc"></a>
### 案例
创建UserModel类，如下：
```java
package com.javacode2018.chat03.demo4.model;

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
创建Mapper接口UserMapper，如下：
```java
package com.javacode2018.chat03.demo4.mapper;

import com.javacode2018.chat03.demo4.model.UserModel;

import java.util.List;
import java.util.Map;

public interface UserMapper {
    /**
     * 通过name查询
     *
     * @param name
     * @return
     */
    UserModel getByName(String name);
}
```
注意上面有个`getByName`方法，这个方法传递一个参数。<br />创建Mapper xml文件UserMapper.xml，mybatis-series\chat03\src\main\resources\com\javacode2018\chat03\demo4\mapper目录创建UserMapper.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.chat03.demo4.mapper.UserMapper">

  <!-- 通过name查询 -->
  <select id="getByName" resultType="com.javacode2018.chat03.demo4.model.UserModel">
    <![CDATA[
    SELECT * FROM t_user WHERE name = #{value} LIMIT 1
    ]]>
  </select>

</mapper>
```
上面有个`getByName`通过用户名查询，通过`#{value}`引用传递进来的name参数，当一个参数的时候`#{变量名称}`中变量名称可以随意写，都可以取到传入的参数。<br />创建属性配置文件，mybatis-series\chat03\src\main\resources目录创建jdbc.properties，如下：
```
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8
jdbc.username=root
jdbc.password=root123
```
上面是本地db配置，可以根据自己db信息做对应修改。<br />创建Mybatis全局配置文件，mybatis-series\chat03\src\main\resources\demo4目录创建mybatis-config.xml，如下：
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
    <package name="com.javacode2018.chat03.demo4.mapper"/>
  </mappers>
</configuration>
```
上面通过properties的resource属性引入了jdbc配置文件。<br />package属性的name指定了mapper接口和mapper xml文件所在的包，Mybatis会扫描这个包，自动注册mapper接口和mapper xml文件。<br />创建测试用例Demo4Test，如下：
```java
package com.javacode2018.chat03.demo4;

import com.javacode2018.chat03.demo4.mapper.UserMapper;
import com.javacode2018.chat03.demo4.model.UserModel;
import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Slf4j
public class Demo4Test {
    private SqlSessionFactory sqlSessionFactory;

    @Before
    public void before() throws IOException {
        //指定mybatis全局配置文件
        String resource = "demo4/mybatis-config.xml";
        //读取全局配置文件
        InputStream inputStream = Resources.getResourceAsStream(resource);
        //构建SqlSessionFactory对象
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        this.sqlSessionFactory = sqlSessionFactory;
    }

    /**
     * 通过map给Mapper接口的方法传递参数
     */
    @Test
    public void getByName() {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
            UserModel userModel = userMapper.getByName("Java");
            log.info("{}", userModel);
        }
    }

}
```
注意上面的`getByName`方法，会调用UserMapper接口的`getByName`方法通过用户名查询用户信息，运行一下这个方法，输出如下：
```
44:55.747 [main] DEBUG c.j.c.d.mapper.UserMapper.getByName - ==>  Preparing: SELECT * FROM t_user WHERE name = ? LIMIT 1 
44:55.779 [main] DEBUG c.j.c.d.mapper.UserMapper.getByName - ==> Parameters: Java(String)
44:55.797 [main] DEBUG c.j.c.d.mapper.UserMapper.getByName - <==      Total: 1
44:55.798 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
```
这个案例中新增的几个文件结构如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690552038546-9122866c-8a37-48a3-adfc-21428000a9b6.png#averageHue=%23eff7e2&clientId=u9a5272de-1569-4&from=paste&id=ua6028a96&originHeight=768&originWidth=549&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u983d7718-a43b-49dc-8ea7-5bd61629d69&title=)
<a name="OOX71"></a>
## 传递一个Map参数
<a name="kHpGQ"></a>
### 用法
如果需要传递的参数比较多，参数个数是动态的，那么可以将这些参数放在一个map中，key为参数名称，value为参数的值。<br />Mapper接口中可以这么定义，如：
```java
List<UserModel> getByMap(Map<String,Object> map);
```
如传递：
```java
Map<String, Object> map = new HashMap<>();
map.put("id", 1L);
map.put("name", "张学友");
```
对应的mapper xml中可以通过#{map中的key}可以获取key在map中对应的value的值作为参数，如：
```sql
SELECT * FROM t_user WHERE id=#{id} OR name = #{name}
```
<a name="Dg7zk"></a>
### 案例
下面通过map传递多个参数来按照id或者用户名进行查询。<br />com.javacode2018.chat03.demo4.mapper.UserMapper中新增一个方法，和上面UserMapper.xml中的对应，如下：
```java
/**
 * 通过map查询
 * @param map
 * @return
 */
List<UserModel> getByMap(Map<String,Object> map);
```
注意上面的方法由2个参数，参数名称分别为id、name，下面在对应的mapper xml中写对应的操作<br />chat03\src\main\resources\com\javacode2018\chat03\demo4\mapper\UserMapper.xml中新增下面代码：
```xml
<!-- 通过map查询 -->
<select id="getByMap" resultType="com.javacode2018.chat03.demo4.model.UserModel">
  <![CDATA[
  SELECT * FROM t_user WHERE id=#{id} OR name = #{name}
  ]]>
</select>
```
大家注意一下上面的取值是使用`#{id}`取id参数的值，`#{name}`取name参数的值，下面创建测试用例，看看是否可以正常运行？<br />Demo4Test中新增下面方法：
```java
/**
 * 通过map给Mapper接口的方法传递参数
 */
@Test
public void getByName() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        UserModel userModel = userMapper.getByName("Java");
        log.info("{}", userModel);
    }
}
```
运行一下上面的这个测试用例，输出：
```
01:28.242 [main] DEBUG c.j.c.d.mapper.UserMapper.getByMap - ==>  Preparing: SELECT * FROM t_user WHERE id=? OR name = ? 
01:28.277 [main] DEBUG c.j.c.d.mapper.UserMapper.getByMap - ==> Parameters: 1(Long), 张学友(String)
01:28.296 [main] DEBUG c.j.c.d.mapper.UserMapper.getByMap - <==      Total: 2
01:28.297 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
01:28.298 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
```
<a name="cdkIa"></a>
## 传递一个Java对象参数
当参数比较多，但是具体有多少个参数是确定的时候，可以将这些参数放在一个javabean对象中。<br />如果想通过userId和userName查询，可以定义一个dto对象，属性添加对应的get、set方法，如：
```java
@Getter
@Setter
@ToString
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class UserFindDto {
    private Long userId;
    private String userName;
}
```
注意上面的get、set方法通过lombok自动生成的。<br />UserMapper中新增一个方法，将UserFindDto作为参数：
```java
/**
 * 通过UserFindDto进行查询
 * @param userFindDto
 * @return
 */
List<UserModel> getListByUserFindDto(UserFindDto userFindDto);
```
对应的UserMapper.xml中这么写，如下：
```xml
<!-- 通过map查询 -->
<select id="getListByUserFindDto" parameterType="com.javacode2018.chat03.demo4.dto.UserFindDto" resultType="com.javacode2018.chat03.demo4.model.UserModel">
  <![CDATA[
  SELECT * FROM t_user WHERE id=#{userId} OR name = #{userName}
  ]]>
</select>
```
Demo4Test中创建一个测试用例来调用一下新增的这个mapper接口中的方法，如下：
```java
@Test
public void getListByUserFindDto() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        UserFindDto userFindDto = UserFindDto.builder().userId(1L).userName("张学友").build();
        List<UserModel> userModelList = userMapper.getListByUserFindDto(userFindDto);
        userModelList.forEach(item -> {
            log.info("{}", item);
        });
    }
}
```
上面通过传递一个userFindDto对象进行查询，运行输出：
```
20:59.454 [main] DEBUG c.j.c.d.m.U.getListByUserFindDto - ==>  Preparing: SELECT * FROM t_user WHERE id=? OR name = ? 
20:59.487 [main] DEBUG c.j.c.d.m.U.getListByUserFindDto - ==> Parameters: 1(Long), 张学友(String)
20:59.508 [main] DEBUG c.j.c.d.m.U.getListByUserFindDto - <==      Total: 2
20:59.509 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
20:59.511 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
```
**传递Java对象的方式相对于map的方式更清晰一些，可以明确知道具体有哪些参数，而传递map，是不知道这个map中具体需要哪些参数的，map对参数也没有约束，参数可以随意传，建议多个参数的情况下选择通过java对象进行传参。**
<a name="JmCIQ"></a>
## 传递多个参数
上面介绍的都是传递一个参数，那么是否可以传递多个参数呢？来试试吧。
<a name="NBC9H"></a>
### 案例
来新增一个通过用户id或用户名查询的操作。<br />com.javacode2018.chat03.demo4.mapper.UserMapper中新增一个方法，和上面UserMapper.xml中的对应，如下：
```java
/**
 * 通过id或者name查询
 *
 * @param id
 * @param name
 * @return
 */
UserModel getByIdOrName(Long id, String name);
```
注意上面的方法由2个参数，参数名称分别为id、name，下面在对应的mapper xml中写对应的操作<br />chat03\src\main\resources\com\javacode2018\chat03\demo4\mapper\UserMapper.xml中新增下面代码：
```xml
<!-- 通过id或者name查询 -->
<select id="getByIdOrName" resultType="com.javacode2018.chat03.demo4.model.UserModel">
  <![CDATA[
  SELECT * FROM t_user WHERE id=#{id} OR name = #{name} LIMIT 1
  ]]>
</select>
```
**大家注意一下上面的取值是使用**`**#{id}**`**取id参数的值，**`**#{name}**`**取name参数的值，下面创建测试用例，看看是否可以正常运行？**<br />Demo4Test中新增下面方法：
```java
/**
 * 通过map给Mapper接口的方法传递参数
 */
@Test
public void getByIdOrName() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        UserModel userModel = userMapper.getByIdOrName(1L, "Java");
        log.info("{}", userModel);
    }
}
```
运行一下上面的这个测试用例，报错了，截取部分主要的错误信息，如下：
```
org.apache.ibatis.exceptions.PersistenceException: 
### Error querying database.  Cause: org.apache.ibatis.binding.BindingException: Parameter 'id' not found. Available parameters are [arg1, arg0, param1, param2]
### Cause: org.apache.ibatis.binding.BindingException: Parameter 'id' not found. Available parameters are [arg1, arg0, param1, param2]

    at org.apache.ibatis.exceptions.ExceptionFactory.wrapException(ExceptionFactory.java:30)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectList(DefaultSqlSession.java:149)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectList(DefaultSqlSession.java:140)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectOne(DefaultSqlSession.java:76)
```
上面报错，给大家解释一下，sql中通过`#{id}`去获取id参数的值，但是Mybatis无法通过`#{id}`获取到id的值，所以报错了，从上错误中看到有这样的一段：
```
Available parameters are [arg1, arg0, param1, param2]
```
上面表示可用的参数名称列表，可以在sql中通过`#{参数名称}`来引参数列表中的参数，先不说这4个参数具体怎么来的，那么将sql改成下面这样试试：
```sql
SELECT * FROM t_user WHERE id=#{arg0} OR name = #{arg1} LIMIT 1
```
再运行一下测试用例，看看效果：
```
46:07.533 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - ==>  Preparing: SELECT * FROM t_user WHERE id=? OR name = ? LIMIT 1 
46:07.566 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - ==> Parameters: 1(Long), Java(String)
46:07.585 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - <==      Total: 1
46:07.586 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
```
这下正常了。<br />将sql再修改一下，修改成下面这样：
```sql
SELECT * FROM t_user WHERE id=#{param1} OR name = #{param2} LIMIT 1
```
运行一下测试用例，输出：
```
47:19.935 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - ==>  Preparing: SELECT * FROM t_user WHERE id=? OR name = ? LIMIT 1 
47:19.966 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - ==> Parameters: 1(Long), Java(String)
47:19.984 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - <==      Total: 1
47:19.985 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
```
也是正常的。<br />来分析一下Mybatis对于这种多个参数是如何处理的？
<a name="ZF9ob"></a>
### 多参数Mybatis的处理
Mybatis处理多个参数的时候，会将多个参数封装到一个map中，map的key为参数的名称，java可以通过反射获取方法参数的名称，下面这个方法：
```java
UserModel getByIdOrName(Long id, String name);
```
编译之后，方法参数的名称通过反射获取的并不是id、name，而是arg0、arg1，也就是说编译之后，方法真实的参数名称会丢失，会变成arg+参数下标的格式。<br />所以上面传递的参数相当于传递了下面这样的一个map：
```java
Map<String,Object> map = new HashMap<>();
map.put("arg0",id);
map.put("arg1",name);
```
那么参数中的param1、param2又是什么呢？<br />上面的map中会放入按照参数名称->参数的值的方式将其放入map中，通过反射的方式获取的参数名称是可能会发生变化的，编译java代码使用javac命令，javac命令有个-parameters参数，当编译代码的时候加上这个参数，方法的实际名称会被编译到class字节码文件中，当通过反射获取方法名称的时候就不是arg0、arg1这种格式了，而是真实的参数名称：id、name了，来修改一下Maven的配置让Maven编译代码的时候加上这个参数，修改chat03/pom.xml中的build元素，这个元素中加入下面代码：
```xml
<plugins>
  <plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-compiler-plugin</artifactId>
    <version>3.3</version>
    <configuration>
      <compilerArgs>
        <arg>-parameters</arg>
      </compilerArgs>
    </configuration>
  </plugin>
</plugins>
```
idea中编译代码也加一下这个参数，操作如下：<br />点击File->Settings->Build,Execution,Deployment->Java Compiler，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690552038534-a5ca9198-4898-46a1-bc4b-43a876d39732.png#averageHue=%23b59c7f&clientId=u9a5272de-1569-4&from=paste&id=u3d35e6fd&originHeight=705&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0546dd75-2254-473d-aa03-07d66468a16&title=)<br />下面将demo4/UserMapper.xml中的`getByIdOrName`对应的sql修改成下面这样：
```sql
SELECT * FROM t_user WHERE id=#{arg0} OR name = #{arg1} LIMIT 1
```
使用Maven命令重新编译一下chat03的代码，cmd命令中mybatis-series/pom.xml所在目录执行下面命令，如下：
```
D:\code\IdeaProjects\mybatis-series>mvn clean compile -pl :chat03
[INFO] Scanning for projects...
[INFO]
[INFO] ----------------------< com.javacode2018:chat03 >-----------------------
[INFO] Building chat03 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ chat03 ---
[INFO] Deleting D:\code\IdeaProjects\mybatis-series\chat03\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ chat03 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 1 resource
[INFO] Copying 11 resources
[INFO]
[INFO] --- maven-compiler-plugin:3.3:compile (default-compile) @ chat03 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 13 source files to D:\code\IdeaProjects\mybatis-series\chat03\target\classes
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.532 s
[INFO] Finished at: 2019-12-10T13:41:05+08:00
[INFO] ------------------------------------------------------------------------
```
再运行一下com.javacode2018.chat03.demo4.Demo4Test#getByIdOrName，输出如下：
```
org.apache.ibatis.exceptions.PersistenceException: 
### Error querying database.  Cause: org.apache.ibatis.binding.BindingException: Parameter 'arg0' not found. Available parameters are [name, id, param1, param2]
### Cause: org.apache.ibatis.binding.BindingException: Parameter 'arg0' not found. Available parameters are [name, id, param1, param2]

    at org.apache.ibatis.exceptions.ExceptionFactory.wrapException(ExceptionFactory.java:30)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectList(DefaultSqlSession.java:149)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectList(DefaultSqlSession.java:140)
    at org.apache.ibatis.session.defaults.DefaultSqlSession.selectOne(DefaultSqlSession.java:76)
```
又报错了，这次错误信息变了，注意有这么一行：
```
Parameter 'arg0' not found. Available parameters are [name, id, param1, param2]
```
参数名称变成了真实的名称了，但是还是有param1、param2，方法参数名称不管怎么变，编译方式如何变化，param1, param2始终在这里，这个param1, param2就是为了应对不同的编译方式导致参数名称而发生变化的，Mybatis内部除了将参数按照名称->值的方式放入map外，还会按照参数的顺序放入一些值，这些值的key就是param+参数位置，这个位置从1开始的，所以id是第一个参数，对应的key是param1，name对应的key是param2，value对应的还是参数的值，所以Mybatis对于参数的处理相当于下面过程：
```java
Map<String,Object> map = new HashMap<>();
map.put("反射获取的参数id的名称",id);
map.put("反射获取的参数name的名称",name);
map.put("param1",id);
map.put("param2",name);
```
将demo4/UserMaper.xml中的`getByIdOrName`对应的sql改成param的方式，如下：
```sql
SELECT * FROM t_user WHERE id=#{param1} OR name = #{param2} LIMIT 1
```
再运行一下com.javacode2018.chat03.demo4.Demo4Test#getByIdOrName，输出如下，正常了：
```
51:12.588 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - ==>  Preparing: SELECT * FROM t_user WHERE id=? OR name = ? LIMIT 1 
51:12.619 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - ==> Parameters: 1(Long), Java(String)
51:12.634 [main] DEBUG c.j.c.d.m.UserMapper.getByIdOrName - <==      Total: 1
51:12.635 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
```
<a name="HBAEn"></a>
### 使用注意

1. **使用参数名称的方式对编译环境有很强的依赖性，如果编译中加上了`**`**-parameters**`**`参数，参数实际名称可以直接使用，如果没有加，参数名称就变成`arg下标`的格式了，这种很容易出错**
2. **sql中使用`param1、param2、paramN`这种方式来引用多参数，对参数的顺序依赖性特别强，如果有人把参数的顺序调整了或者调整了参数的个数，后果就是灾难性的，所以这种方式不建议大家使用。**
<a name="AArzR"></a>
## 多参数中用`@Param`指定参数名称
刚才上面讲了多参数传递的使用上面，对参数名称和顺序有很强的依赖性，容易导致一些严重的错误。<br />Mybatis也考虑到了这种情况，可以让我们自己去指定参数的名称，通过`@Param("参数名称")`来给参数指定名称。<br />com.javacode2018.chat03.demo4.mapper.UserMapper#getByIdOrName做一下修改：
```java
/**
 * 通过id或者name查询
 *
 * @param id
 * @param name
 * @return
 */
UserModel getByIdOrName(@Param("userId") Long id, @Param("userName") String name);
```
上面通过`@Param`注解给两个参数明确指定了名称，分别是userId、userName，对应的UserMapper.xml中也做一下调整，如下：
```xml
<!-- 通过id或者name查询 -->
<select id="getByIdOrName" resultType="com.javacode2018.chat03.demo4.model.UserModel">
  <![CDATA[
  SELECT * FROM t_user WHERE id=#{userId} OR name = #{userName} LIMIT 1
  ]]>
</select>
```
运行com.javacode2018.chat03.demo4.Demo4Test#getByMap，输出：
```
13:25.431 [main] DEBUG c.j.c.d.mapper.UserMapper.getByMap - ==>  Preparing: SELECT * FROM t_user WHERE id=? OR name = ? 
13:25.460 [main] DEBUG c.j.c.d.mapper.UserMapper.getByMap - ==> Parameters: null, 张学友(String)
13:25.477 [main] DEBUG c.j.c.d.mapper.UserMapper.getByMap - <==      Total: 1
13:25.478 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
```
<a name="YsViu"></a>
## Mybatis参数处理相关源码
上面参数的解析过程代码在`org.apache.ibatis.reflection.ParamNameResolver`类中，主要看下面的2个方法：
```java
public ParamNameResolver(Configuration config, Method method)
public Object getNamedParams(Object[] args)
```
这2个方法建议大家都设置一下断点细看一下整个过程，方法的实现不复杂。
<a name="Wl13N"></a>
## 传递1个Collection参数
当传递的参数类型是java.util.Collection的时候，会被放在map中，key为collection，value为参数的值，如下面的查询方法：
```java
/**
 * 查询用户id列表
 *
 * @param idCollection
 * @return
 */
List<UserModel> getListByIdCollection(Collection<Long> idCollection);
```
上面的查询方法，Mybatis内部会将idList做一下处理：
```java
Map<String,Object> map = new HashMap<>();
map.put("collection",idCollection)
```
所以在mapper xml中使用的使用，需要通过collection名称来引用idCollection参数，如下：
```xml
<!-- 通过用户id列表查询 -->
<select id="getListByIdCollection" resultType="com.javacode2018.chat03.demo4.model.UserModel">
  <![CDATA[
  SELECT * FROM t_user WHERE id IN (#{collection[0]},#{collection[1]})
  ]]>
</select>
```
com.javacode2018.chat03.demo4.Demo4Test中写个测试用例`getListByIdList`，查询2个用户信息，如下：
```java
@Test
public void getListByIdCollection() {
    log.info("----------");
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        List<Long> userIdList = Arrays.asList(1L, 3L);
        List<UserModel> userModelList = userMapper.getListByIdCollection(userIdList);
        userModelList.forEach(item -> {
            log.info("{}", item);
        });
    }
}
```
运行输出：
```
26:15.774 [main] INFO  c.j.chat03.demo4.Demo4Test - ----------
26:16.055 [main] DEBUG c.j.c.d.m.U.getListByIdCollection - ==>  Preparing: SELECT * FROM t_user WHERE id IN (?,?) 
26:16.083 [main] DEBUG c.j.c.d.m.U.getListByIdCollection - ==> Parameters: 1(Long), 3(Long)
26:16.102 [main] DEBUG c.j.c.d.m.U.getListByIdCollection - <==      Total: 2
26:16.103 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
26:16.105 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
```
<a name="BTkV4"></a>
## Mybatis中集合参数处理了源码解析
集合参数，Mybatis会进行一些特殊处理，代码在下面的方法中：
```
org.apache.ibatis.session.defaults.DefaultSqlSession#wrapCollection
```
这个方法的源码如下：
```java
private Object wrapCollection(final Object object) {
    if (object instanceof Collection) {
        StrictMap<Object> map = new StrictMap<>();
        map.put("collection", object);
        if (object instanceof List) {
            map.put("list", object);
        }
        return map;
    } else if (object != null && object.getClass().isArray()) {
        StrictMap<Object> map = new StrictMap<>();
        map.put("array", object);
        return map;
    }
    return object;
}
```
源码解释：<br />判断参数是否是java.util.Collection类型，如果是，会放在map中，key为collection。<br />如果参数是java.util.List类型的，会在map中继续放一个list作为key来引用这个对象。<br />如果参数是数组类型的，会通过array来引用这个对象。<br />传递1个List参数<br />从上面源码中可知，List类型的参数会被放在map中，可以通过2个key（collection和list）都可以引用到这个List对象。<br />com.javacode2018.chat03.demo4.mapper.UserMapper中新增一个方法：
```java
/**
 * 查询用户id列表
 *
 * @param idList
 * @return
 */
List<UserModel> getListByIdList(List<Long> idList);
```
对应的demo4/UserMaper.xml中增加一个操作，如下：
```xml
<!-- 通过用户id列表查询 -->
<select id="getListByIdList" resultType="com.javacode2018.chat03.demo4.model.UserModel">
  <![CDATA[
  SELECT * FROM t_user WHERE id IN (#{list[0]},#{collection[1]})
  ]]>
</select>
```
注意上面使用了2中方式获取参数，通过list、collection都可以引用List类型的参数。<br />新增一个测试用例com.javacode2018.chat03.demo4.Demo4Test#getListByIdList，如下：
```java
@Test
public void getListByIdList() {
    log.info("----------");
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        List<Long> userIdList = Arrays.asList(1L, 3L);
        List<UserModel> userModelList = userMapper.getListByIdList(userIdList);
        userModelList.forEach(item -> {
            log.info("{}", item);
        });
    }
}
```
运行输出：
```
33:17.871 [main] INFO  c.j.chat03.demo4.Demo4Test - ----------
33:18.153 [main] DEBUG c.j.c.d.m.UserMapper.getListByIdList - ==>  Preparing: SELECT * FROM t_user WHERE id IN (?,?) 
33:18.185 [main] DEBUG c.j.c.d.m.UserMapper.getListByIdList - ==> Parameters: 1(Long), 3(Long)
33:18.207 [main] DEBUG c.j.c.d.m.UserMapper.getListByIdList - <==      Total: 2
33:18.208 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
33:18.210 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=3, name=张学友, age=56, salary=500000.0, sex=1)
```
<a name="b8p4c"></a>
## 传递1个数组参数
数组类型的参数从上面源码中可知，sql中需要通过array来进行引用，这个就不写了，案例中也是有的，大家可以去看一下com.javacode2018.chat03.demo4.Demo4Test#getListByIdArray这个方法。
<a name="zLDtk"></a>
## `ResultHandler`作为参数
<a name="ChbSE"></a>
### 用法
查询的数量比较大的时候，返回一个List集合占用的内存还是比较多的，比如想导出很多数据，实际上如果通过jdbc的方式，遍历`ResultSet`的`next`方法，一条条处理，而不用将其存到List集合中再取处理。<br />Mybatis中也支持这么做，可以使用`ResultHandler`对象，犹如其名，这个接口是用来处理结果的，先看一下其定义：
```java
public interface ResultHandler<T> {

    void handleResult(ResultContext<? extends T> resultContext);

}
```
里面有1个方法，方法的参数是`ResultContext`类型的，这个也是一个接口，看一下源码：
```java
public interface ResultContext<T> {

    T getResultObject();

    int getResultCount();

    boolean isStopped();

    void stop();

}
```
4个方法：

- `getResultObject`：获取当前行的结果
- `getResultCount`：获取当前结果到第几行了
- `isStopped`：判断是否需要停止遍历结果集
- `stop`：停止遍历结果集

`ResultContext`接口有一个实现类`org.apache.ibatis.executor.result.DefaultResultContext`，Mybatis中默认会使用这个类。
<a name="qEOi7"></a>
### 案例
遍历t_user表的所有记录，第2条遍历结束之后，停止遍历，实现如下：<br />新增一个方法com.javacode2018.chat03.demo4.mapper.UserMapper#getList，如下：
```java
void getList(ResultHandler<UserModel> resultHandler);
```
对应的UserMapper.xml新增sql操作，如下：
```xml
<select id="getList" resultType="com.javacode2018.chat03.demo4.model.UserModel">
  <![CDATA[
  SELECT * FROM t_user
  ]]>
</select>
```
新增测试用例com.javacode2018.chat03.demo4.Demo4Test#getList，如下：
```java
@Test
public void getList() {
    log.info("----------");
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        userMapper.getList(context -> {
            //将context参数转换为DefaultResultContext对象
            DefaultResultContext<UserModel> defaultResultContext = (DefaultResultContext<UserModel>) context;
            log.info("{}", defaultResultContext.getResultObject());
            //遍历到第二条之后停止
            if (defaultResultContext.getResultCount() == 2) {
                //调用stop方法停止遍历，stop方法会更新内部的一个标志，置为停止遍历
                defaultResultContext.stop();
            }
        });
    }
}
```
运行输出：
```
07:05.561 [main] INFO  c.j.chat03.demo4.Demo4Test - ----------
07:05.816 [main] DEBUG c.j.c.d.mapper.UserMapper.getList - ==>  Preparing: SELECT * FROM t_user 
07:05.845 [main] DEBUG c.j.c.d.mapper.UserMapper.getList - ==> Parameters: 
07:05.864 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=1, name=Java, age=30, salary=50000.0, sex=1)
07:05.867 [main] INFO  c.j.chat03.demo4.Demo4Test - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
```
