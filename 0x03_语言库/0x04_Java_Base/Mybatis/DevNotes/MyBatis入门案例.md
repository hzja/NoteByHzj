JavaMybatis
<a name="s9ymE"></a>
## 案例
<a name="ZQEVP"></a>
### 准备数据库
MySQL中运行下面脚本：
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
  `salary` DECIMAL(12,2) NOT NULL DEFAULT 0 COMMENT '薪水'
) COMMENT '用户表';

SELECT * FROM t_user;
```
上面脚本中，创建了一个javacode2018数据库，然后创建了一个用户表，里面有4个字段，id为主键自动增长。
<a name="KfaI4"></a>
### 需求
对t_user表，有以下这些需求：

1. 实现一个通用的插入操作：支持动态插入，可以根据传入的字段的值，动态生成所需要的各种insert语句
2. 批量插入功能
3. 实现一个通用的更新操作：支持动态更新操作，可以根据传入的字段，动态生成所需要的各种update语句
4. 实现一个通用的查询操作：支持各种组合条件查询、支撑排序、分页、支持返回列的控制等各种复杂的查询需求

下面就来一个案例，将上面这些需求通过MyBatis实现，先见证一下MyBatis的强大之处。
<a name="ca0AS"></a>
### 创建Maven项目
IDEA中创建Maven项目，项目采用Maven中聚合及继承的方式来管理。
<a name="A4gja"></a>
#### 创建父项目
先创建父项目mybatis-series，父项目的坐标信息：
```xml
<groupId>com.javacode2018</groupId>
<artifactId>mybatis-series</artifactId>
<version>1.0-SNAPSHOT</version>
```
<a name="nan4M"></a>
#### 创建子项目
创建一个子模块chat01，子模块的坐标信息：
```xml
<groupId>com.javacode2018</groupId>
<artifactId>chat01</artifactId>
<version>1.0-SNAPSHOT</version>
```
<a name="GzR8G"></a>
#### 项目结构
如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690295033380-8be4bdae-8c0c-49c9-ab98-44dffc31c0c2.png#averageHue=%23fbfaf7&clientId=uaff525b9-0730-4&from=paste&id=ua2957e3d&originHeight=442&originWidth=493&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u801f5744-2c6b-47f1-ad68-66fa2ec88d4&title=)
<a name="sqXXt"></a>
### 引入MyBatis依赖
mybatis-series/pom.xml内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>mybatis-series</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>pom</packaging>

  <modules>
    <module>chat01</module>
  </modules>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <!-- 配置maven编译的时候采用的编译器版本 -->
    <maven.compiler.compilerVersion>1.8</maven.compiler.compilerVersion>
    <!-- 指定源代码是什么版本的，如果源码和这个版本不符将报错，maven中执行编译的时候会用到这个配置，默认是1.5，这个相当于javac命令后面的-source参数 -->
    <maven.compiler.source>1.8</maven.compiler.source>
    <!-- 该命令用于指定生成的class文件将保证和哪个版本的虚拟机进行兼容，maven中执行编译的时候会用到这个配置，默认是1.5，这个相当于javac命令后面的-target参数 -->
    <maven.compiler.target>1.8</maven.compiler.target>
    <mybatis.version>3.5.3</mybatis.version>
    <mysql.version>5.1.47</mysql.version>
    <lombok.version>1.18.10</lombok.version>
  </properties>

  <dependencyManagement>
    <dependencies>
      <dependency>
        <groupId>org.mybatis</groupId>
        <artifactId>mybatis</artifactId>
        <version>${mybatis.version}</version>
      </dependency>
      <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
        <version>${mysql.version}</version>
      </dependency>
      <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <version>${lombok.version}</version>
        <scope>provided</scope>
      </dependency>
      <dependency>
        <groupId>junit</groupId>
        <artifactId>junit</artifactId>
        <version>4.12</version>
        <scope>test</scope>
      </dependency>
      <dependency>
        <groupId>ch.qos.logback</groupId>
        <artifactId>logback-classic</artifactId>
        <version>1.2.3</version>

      </dependency>
    </dependencies>
  </dependencyManagement>

</project>
```
chat01/pom.xml内容如下：
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
  <artifactId>chat01</artifactId>

  <dependencies>
    <dependency>
      <groupId>org.mybatis</groupId>
      <artifactId>mybatis</artifactId>
    </dependency>
    <dependency>
      <groupId>mysql</groupId>
      <artifactId>mysql-connector-java</artifactId>
    </dependency>
    <dependency>
      <groupId>org.projectlombok</groupId>
      <artifactId>lombok</artifactId>
    </dependency>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
    </dependency>
    <dependency>
      <groupId>ch.qos.logback</groupId>
      <artifactId>logback-classic</artifactId>
    </dependency>
  </dependencies>

</project>
```
上面引入了MyBatis需要的包、mysql jdbc驱动、Lombok、单元测试需要的junit包、日志输出需要的logback包。<br />Lombok可以自动生成JavaBean的一些代码，比如get、set方法，可以节省开发编写代码的量。
<a name="zo3TX"></a>
### 配置logback
MyBatis在运行过程中会输出一些日志，比如sql信息、sql的参数信息、执行的结果等信息，MyBatis中会通过logback输出出来。<br />在chat01/src/main/resources目录中新建文件logback.xml，内容如下：
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
<a name="wbWES"></a>
### 创建MyBatis相关文件
<a name="UwuzD"></a>
#### user.xml
chat01/src/main/resources目录中新建user.xml，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.mybatis.chat01.UserMapper">

  <!-- 插入 -->
  <insert id="insert" parameterType="com.javacode2018.mybatis.chat01.UserModel" keyProperty="id" useGeneratedKeys="true">
    <![CDATA[ INSERT INTO `t_user` ]]>
    <trim prefix="(" suffix=")" suffixOverrides=",">
      <if test="id!=null">
        <![CDATA[ `id`, ]]>
      </if>
      <if test="name!=null">
        <![CDATA[ `name`, ]]>
      </if>
      <if test="age!=null">
        <![CDATA[ `age`, ]]>
      </if>
      <if test="salary!=null">
        <![CDATA[ `salary`, ]]>
      </if>
    </trim>
    <![CDATA[ VALUES ]]>
    <trim prefix="(" suffix=")" suffixOverrides=",">
      <if test="id!=null">
        <![CDATA[ #{id}, ]]>
      </if>
      <if test="name!=null">
        <![CDATA[ #{name}, ]]>
      </if>
      <if test="age!=null">
        <![CDATA[ #{age}, ]]>
      </if>
      <if test="salary!=null">
        <![CDATA[ #{salary}, ]]>
      </if>
    </trim>
  </insert>

  <!-- 批量插入 -->
  <insert id="insertBatch" parameterType="map">
    <![CDATA[ INSERT INTO `t_user` (`id`, `name`, `age`, `salary`) VALUES ]]>
    <foreach collection="list" separator="," item="item">
      (#{item.id}, #{item.name}, #{item.age}, #{item.salary})
    </foreach>
  </insert>

  <!-- 更新 -->
  <update id="update" parameterType="com.javacode2018.mybatis.chat01.UserModel">
    <![CDATA[ UPDATE `t_user` ]]>
    <set>
      <if test="name!=null">
        <![CDATA[ `name` = #{name}, ]]>
      </if>
      <if test="age!=null">
        <![CDATA[ `age` = #{age}, ]]>
      </if>
      <if test="salary!=null">
        <![CDATA[ `salary` = #{salary}, ]]>
      </if>
    </set>
    <where>
      <if test="id!=null">
        <![CDATA[ AND `id` = #{id} ]]>
      </if>
    </where>
  </update>

  <!-- 更新 -->
  <update id="updateByMap" parameterType="map">
    <![CDATA[ UPDATE `t_user` ]]>
    <set>
      <if test="name!=null">
        <![CDATA[ `name` = #{name}, ]]>
      </if>
      <if test="age!=null">
        <![CDATA[ `age` = #{age}, ]]>
      </if>
      <if test="salary!=null">
        <![CDATA[ `salary` = #{salary}, ]]>
      </if>
    </set>
    <where>
      <if test="id!=null">
        <![CDATA[ AND `id` = #{id} ]]>
      </if>
    </where>
  </update>

  <!-- 删除 -->
  <delete id="delete" parameterType="map">
    <![CDATA[
    DELETE FROM `t_user`
    ]]>
    <where>
      <if test="id!=null">
        <![CDATA[ AND `id` = #{id} ]]>
      </if>
      </where>
      </delete>


        <!-- 查询记录 -->
        <select id="getModelList" parameterType="map" resultType="com.javacode2018.mybatis.chat01.UserModel">
        <![CDATA[
        SELECT
        ]]>
        <choose>
        <when test="tableColumnList!=null and tableColumnList.size() >= 1">
        <foreach collection="tableColumnList" item="item" separator=",">
        <![CDATA[ ${item} ]]>
      </foreach>
      </when>
        <otherwise>
        <![CDATA[
        `id`,
        `name`,
        `age`,
        `salary`
        ]]>
      </otherwise>
      </choose>
        <![CDATA[
        FROM
        `t_user` a
        ]]>
        <where>
        <if test="id!=null and id.toString()!=''">
        <![CDATA[ AND a.`id` = #{id} ]]>
      </if>
        <if test="idList!=null and idList.size() >= 1">
        <![CDATA[ AND a.`id` IN ]]>
        <foreach collection="idList" item="item" open="(" separator="," close=")">
        <![CDATA[ #{item} ]]>
      </foreach>
      </if>
        <if test="name!=null and name.toString()!=''">
        <![CDATA[ AND a.`name` = #{name} ]]>
      </if>
        <if test="age!=null and age.toString()!=''">
        <![CDATA[ AND a.`age` = #{age} ]]>
      </if>
        <if test="salary!=null and salary.toString()!=''">
        <![CDATA[ AND a.`salary` = #{salary} ]]>
      </if>
        <if test="nameLike!=null and nameLike.toString()!=''">
        <![CDATA[ AND a.`name` like '%${nameLike}%' ]]>
      </if>
        <if test="salaryGte!=null and salaryGte.toString()!=''">
        <![CDATA[ AND a.`salary` >= #{salaryGte} ]]>
      </if>
      </where>
        <if test="sort!=null and sort.toString()!=''">
        <![CDATA[ order by ${sort} ]]>
      </if>
        <if test="skip!=null and pageSize!=null">
        <![CDATA[ LIMIT #{skip},#{pageSize} ]]>
      </if>
      </select>

      </mapper>
```
<a name="kY6NO"></a>
#### mybatis-config.xml
chat01/src/main/resources目录中新建mybatis-config.xml，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
  <properties>
    <property name="driver" value="com.mysql.jdbc.Driver"/>
    <property name="url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
    <property name="username" value="root"/>
    <property name="password" value="root123"/>
  </properties>
  <environments default="development">
    <environment id="development">
      <transactionManager type="JDBC"/>
      <dataSource type="POOLED">
        <property name="driver" value="${driver}"/>
        <property name="url" value="${url}"/>
        <property name="username" value="${username}"/>
        <property name="password" value="${password}"/>
      </dataSource>
    </environment>
  </environments>
  <mappers>
    <mapper resource="mapper/user.xml"/>
  </mappers>
</configuration>
```
<a name="BjUjJ"></a>
#### UserMapper接口
```java
package com.javacode2018.mybatis.chat01;

import java.util.List;
import java.util.Map;

public interface UserMapper {

    /**
     * 插入用户信息
     *
     * @param userModel
     * @return
     */
    void insert(UserModel userModel);

    /**
     * 批量插入用户信息
     *
     * @param userModelList
     */
    void insertBatch(List<UserModel> userModelList);

    /**
     * 更新用户信息
     *
     * @param userModel
     * @return
     */
    int update(UserModel userModel);

    /**
     * 通过map来更新用户记录
     *
     * @param map
     * @return
     */
    int updateByMap(Map<String, Object> map);

    /**
     * 通过map来删除用户记录
     *
     * @param map
     * @return
     */
    int delete(Map<String, Object> map);

    /**
     * 查询用户列表
     *
     * @param map
     * @return
     */
    List<UserModel> getModelList(Map<String, Object> map);

}
```
<a name="xo0ao"></a>
#### UserModel类
```java
package com.javacode2018.mybatis.chat01;

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
}
```
这个类上面的注解都是都是Lombok中的，通过这些注解，Lombok可以自动生成上面4个字段的get方法、set方法、无参构造方法、有参有参构造方法、builder模式构建对象的代码、重写toString方法，这些都在代码编译为字节码之前会写进去，通过Lombok代码是不是精简了很多，最后生成的代码大家可以反编译一下UserModel.class去看一下，感受一下，此处就不贴出来了。
<a name="HUD7V"></a>
#### UserUtil类
```java
package com.javacode2018.mybatis.chat01;

import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;

import java.io.IOException;

@Slf4j
public class UserUtil {
    private static SqlSessionFactory sqlSessionFactory = build();

    public static SqlSessionFactory build() {
        try {
            return new SqlSessionFactoryBuilder().build(Resources.getResourceAsStream("mybatis-config.xml"));
        } catch (IOException e) {
            log.error(e.getMessage(), e);
            throw new RuntimeException(e);
        }
    }

    @FunctionalInterface
    public interface SessionCall<O> {
        O call(SqlSession session) throws Exception;
    }

    @FunctionalInterface
    public interface MapperCall<T, O> {
        O call(T mapper) throws Exception;
    }

    public static <T, O> O callMapper(Class<T> tClass, MapperCall<T, O> mapper) throws Exception {
        return call(session -> mapper.call(session.getMapper(tClass)));
    }

    public static <O> O call(SessionCall<O> sessionCall) throws Exception {
        try (SqlSession session = sqlSessionFactory.openSession(true);) {
            return sessionCall.call(session);
        }
    }
}
```
<a name="UeHdM"></a>
### 创建单元测试类UserMapperTest
chat01\src\test\java\com\javacode2018\mybatis\chat01中创建UserMapperTest，代码如下：
```java
package com.javacode2018.mybatis.chat01;

import lombok.extern.slf4j.Slf4j;
import org.junit.Test;

import java.util.*;
import java.util.stream.Collectors;

@Slf4j
public class UserMapperTest {

    //动态插入
    @Test
    public void insert() throws Exception {
        UserModel userModel1 = UserModel.builder().name("Tom").build();
        UserUtil.callMapper(UserMapper.class, mapper -> {
            mapper.insert(userModel1);
            return null;
        });
        log.info("插入结果：{}", this.getModelById(userModel1.getId()));
        log.info("---------------------");
        UserModel userModel2 = UserModel.builder().name("Tom").age(30).salary(50000.00).build();
        UserUtil.callMapper(UserMapper.class, mapper -> {
            mapper.insert(userModel2);
            return null;
        });
        log.info("插入结果：{}", this.getModelById(userModel2.getId()));
    }

    //批量插入
    @Test
    public void insertBatch() throws Exception {
        List<UserModel> userModelList = new ArrayList<>();
        for (int i = 1; i <= 5; i++) {
            userModelList.add(UserModel.builder().name("Tom-" + i).age(30 + i).salary(10000.00 * i).build());
            userModelList.add(UserModel.builder().name("javacode2018-" + i).age(30 + i).salary(10000.00 * i).build());
        }
        UserUtil.callMapper(UserMapper.class, mapper -> {
            mapper.insertBatch(userModelList);
            return null;
        });

        List<UserModel> userModelList1 = UserUtil.callMapper(UserMapper.class, mapper -> mapper.getModelList(null));
        log.info("结果:{}", userModelList1);
    }

    //根据用户id删除数据
    @Test
    public void delete() throws Exception {
        Map<String, Object> map = new HashMap<>();
        //需要删除的用户id
        map.put("id", 1);
        Integer count = UserUtil.callMapper(UserMapper.class, mapper -> mapper.delete(map));
        log.info("删除行数：{}", count);
    }

    //动态更新
    @Test
    public void update() throws Exception {
        //将userId=2的name修改为：Tom
        Long userId1 = 2L;
        Integer count = UserUtil.callMapper(UserMapper.class, mapper -> mapper.update(UserModel.builder().id(userId1).name("ready").build()));
        log.info("更新行数：{}", count);

        log.info("---------------------");
        //将userId=3的name修改为：Tom,薪水为：1000.88
        Long userId2 = 3L;
        count = UserUtil.callMapper(UserMapper.class, mapper -> mapper.update(UserModel.builder().id(userId2).name("ready").salary(1000.88D).build()));
        log.info("更新行数：{}", count);
    }

    //按用户id查询
    public UserModel getModelById(Long userId) throws Exception {
        //查询指定id的数据
        Map<String, Object> map = new HashMap<>();
        map.put("id", userId);
        return UserUtil.callMapper(UserMapper.class, mapper -> {
            List<UserModel> userModelList = mapper.getModelList(map);
            if (userModelList.size() == 1) {
                return userModelList.get(0);
            }
            return null;
        });
    }

    //查询所有数据
    @Test
    public void getModelList1() throws Exception {
        List<UserModel> userModelList = UserUtil.callMapper(UserMapper.class, mapper -> mapper.getModelList(null));
        log.info("结果:{}", userModelList);
    }

    //查询多个用户id对应的数据
    @Test
    public void getModelListByIds() throws Exception {
        List<Integer> idList = Arrays.asList(2, 3, 4).stream().collect(Collectors.toList());
        Map<String, Object> map = new HashMap<>();
        map.put("idList", idList);

        List<UserModel> userModelList = UserUtil.callMapper(UserMapper.class, mapper -> mapper.getModelList(map));
        log.info("结果:{}", userModelList);
    }

    //多条件 & 指定返回的列
    @Test
    public void getModelList2() throws Exception {
        //查询姓名中包含Tom以及薪资大于3万的用户id、姓名
        Map<String, Object> map = new HashMap<>();
        map.put("nameLike", "Tom");
        map.put("salaryGte", 30000.00D);
        //需要返回的列
        List<String> tableColumnList = new ArrayList<>();
        tableColumnList.add("id");
        tableColumnList.add("name");
        map.put("tableColumnList", tableColumnList);

        List<UserModel> userModelList = UserUtil.callMapper(UserMapper.class, mapper -> mapper.getModelList(map));
        log.info("结果:{}", userModelList);
    }

    //条件过滤 & 排序 & 分页查询数据 & 只返回用户id、salary
    @Test
    public void getPage() throws Exception {
        //查询姓名中包含Tom以及薪资大于3万的用户id，按照薪资倒叙，每页5条取第1页
        Map<String, Object> map = new HashMap<>();
        map.put("nameLike", "Tom");
        map.put("salaryGte", 30000.00D);

        //加入排序参数
        map.put("sort", "salary desc");

        //加入分页参数
        int page = 1;
        int pageSize = 5;
        map.put("skip", (page - 1) * pageSize);
        map.put("pageSize", pageSize);

        //加入需要返回的列
        List<String> tableColumnList = new ArrayList<>();
        tableColumnList.add("id");
        tableColumnList.add("salary");
        map.put("tableColumnList", tableColumnList);

        List<UserModel> userModelList = UserUtil.callMapper(UserMapper.class, mapper -> mapper.getModelList(map));
        log.info("结果:{}", userModelList);
    }
}
```
<a name="HnUQo"></a>
### 项目最终结构如下
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690295033476-78da5efa-559b-4063-a2fa-56d15869275c.png#averageHue=%23f9f7f5&clientId=uaff525b9-0730-4&from=paste&id=u81ff1f7d&originHeight=836&originWidth=483&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc44b78a6-0787-4c5b-a26b-8cc5bef5604&title=)
<a name="RkqOi"></a>
### 用例：动态插入
运行UserMapperTest#insert，输出如下：
```
37:58.556 [main] DEBUG c.j.mybatis.chat01.UserMapper.insert - ==>  Preparing: INSERT INTO `t_user` ( `name` ) VALUES ( ? ) 
37:58.605 [main] DEBUG c.j.mybatis.chat01.UserMapper.insert - ==> Parameters: Tom(String)
37:58.613 [main] DEBUG c.j.mybatis.chat01.UserMapper.insert - <==    Updates: 1
37:58.641 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==>  Preparing: SELECT `id`, `name`, `age`, `salary` FROM `t_user` a WHERE a.`id` = ? 
37:58.641 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==> Parameters: 1(Long)
37:58.663 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - <==      Total: 1
37:58.664 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 插入结果：UserModel(id=1, name=Tom, age=1, salary=0.0)
37:58.667 [main] INFO  c.j.mybatis.chat01.UserMapperTest - ---------------------
37:58.668 [main] DEBUG c.j.mybatis.chat01.UserMapper.insert - ==>  Preparing: INSERT INTO `t_user` ( `name`, `age`, `salary` ) VALUES ( ?, ?, ? ) 
37:58.675 [main] DEBUG c.j.mybatis.chat01.UserMapper.insert - ==> Parameters: Tom(String), 30(Integer), 50000.0(Double)
37:58.679 [main] DEBUG c.j.mybatis.chat01.UserMapper.insert - <==    Updates: 1
37:58.681 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==>  Preparing: SELECT `id`, `name`, `age`, `salary` FROM `t_user` a WHERE a.`id` = ? 
37:58.681 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==> Parameters: 2(Long)
37:58.683 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - <==      Total: 1
37:58.683 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 插入结果：UserModel(id=2, name=Tom, age=30, salary=50000.0)
```
UserMapperTest#insert这个方法主要有4步操作：<br />步骤1：插入一条用户记录，用户记录只有name字段有值<br />步骤2：去db中查询步骤1中插入的记录<br />步骤3：插入一条用户记录，这次插入的记录所有字段都指定了值<br />步骤4：去db中查询步骤3中插入的记录<br />**重点来了：**看一下UserMapperTest#insert方法的代码，两个插入调用都是mapper.insert方法，传入的都是UserModel对象，唯一不同的是这个对象构建的时候字段的值不一样，最后再认真看一下上面输出的sql，产生的2个insert也是不一样的，这个mapper.insert方法可以根据UserModel对象字段是否有值来组装sql，这就是动态插入。
<a name="Q66DU"></a>
### 用例：批量插入
运行UserMapperTest#insertBatch，输出如下：
```
38:12.425 [main] DEBUG c.j.m.chat01.UserMapper.insertBatch - ==>  Preparing: INSERT INTO `t_user` (`id`, `name`, `age`, `salary`) VALUES (?, ?, ?, ?) , (?, ?, ?, ?) , (?, ?, ?, ?) , (?, ?, ?, ?) , (?, ?, ?, ?) , (?, ?, ?, ?) , (?, ?, ?, ?) , (?, ?, ?, ?) , (?, ?, ?, ?) , (?, ?, ?, ?) 
38:12.476 [main] DEBUG c.j.m.chat01.UserMapper.insertBatch - ==> Parameters: null, Tom-1(String), 31(Integer), 10000.0(Double), null, javacode2018-1(String), 31(Integer), 10000.0(Double), null, Tom-2(String), 32(Integer), 20000.0(Double), null, javacode2018-2(String), 32(Integer), 20000.0(Double), null, Tom-3(String), 33(Integer), 30000.0(Double), null, javacode2018-3(String), 33(Integer), 30000.0(Double), null, Tom-4(String), 34(Integer), 40000.0(Double), null, javacode2018-4(String), 34(Integer), 40000.0(Double), null, Tom-5(String), 35(Integer), 50000.0(Double), null, javacode2018-5(String), 35(Integer), 50000.0(Double)
38:12.484 [main] DEBUG c.j.m.chat01.UserMapper.insertBatch - <==    Updates: 10
38:12.502 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==>  Preparing: SELECT `id`, `name`, `age`, `salary` FROM `t_user` a 
38:12.502 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==> Parameters: 
38:12.521 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - <==      Total: 12
38:12.521 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 结果:[UserModel(id=1, name=Tom, age=1, salary=0.0), UserModel(id=2, name=Tom, age=30, salary=50000.0), UserModel(id=3, name=Tom-1, age=31, salary=10000.0), UserModel(id=4, name=javacode2018-1, age=31, salary=10000.0), UserModel(id=5, name=Tom-2, age=32, salary=20000.0), UserModel(id=6, name=javacode2018-2, age=32, salary=20000.0), UserModel(id=7, name=Tom-3, age=33, salary=30000.0), UserModel(id=8, name=javacode2018-3, age=33, salary=30000.0), UserModel(id=9, name=Tom-4, age=34, salary=40000.0), UserModel(id=10, name=javacode2018-4, age=34, salary=40000.0), UserModel(id=11, name=Tom-5, age=35, salary=50000.0), UserModel(id=12, name=javacode2018-5, age=35, salary=50000.0)]
```
这次批量插入了10条用户记录，可以看到有这样的输出：
```
40:40.727 [main] DEBUG c.j.m.chat01.UserMapper.insertBatch - <==    Updates: 10
```
上面这个表示插入影响的行数，10表示插入了10行。<br />批量插入之后，又执行了全表查询，这次插入了10条，加上前面的2个单条插入，表中总计12条记录。
<a name="BBxzP"></a>
### 用例：根据用户id删除数据
运行UserMapperTest#delete，输出如下：
```
38:36.498 [main] DEBUG c.j.mybatis.chat01.UserMapper.delete - ==>  Preparing: DELETE FROM `t_user` WHERE `id` = ? 
38:36.551 [main] DEBUG c.j.mybatis.chat01.UserMapper.delete - ==> Parameters: 1(Integer)
38:36.560 [main] DEBUG c.j.mybatis.chat01.UserMapper.delete - <==    Updates: 1
38:36.561 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 删除行数：1
```
<a name="Wx5gS"></a>
### 用例：动态更新
运行UserMapperTest#update，输出如下：
```
38:51.289 [main] DEBUG c.j.mybatis.chat01.UserMapper.update - ==>  Preparing: UPDATE `t_user` SET `name` = ? WHERE `id` = ? 
38:51.347 [main] DEBUG c.j.mybatis.chat01.UserMapper.update - ==> Parameters: ready(String), 2(Long)
38:51.355 [main] DEBUG c.j.mybatis.chat01.UserMapper.update - <==    Updates: 1
38:51.356 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 更新行数：1
38:51.358 [main] INFO  c.j.mybatis.chat01.UserMapperTest - ---------------------
38:51.359 [main] DEBUG c.j.mybatis.chat01.UserMapper.update - ==>  Preparing: UPDATE `t_user` SET `name` = ?, `salary` = ? WHERE `id` = ? 
38:51.360 [main] DEBUG c.j.mybatis.chat01.UserMapper.update - ==> Parameters: ready(String), 1000.88(Double), 3(Long)
38:51.363 [main] DEBUG c.j.mybatis.chat01.UserMapper.update - <==    Updates: 1
38:51.364 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 更新行数：1
```
UserMapperTest#update方法，大家也认真看一下，2个更新，调用都是mapper.update方法，传入的都是UserModel类型的参数，只是2个UserModel对象的字段值不一样，最后产生的2个update语句也是不一样的，这个update语句是MyBatis动态组装的，MyBatis可以根据UserModel中字段是否为NULL，来拼装sql，这个更新是不是很强大。
<a name="vRd96"></a>
### 用例：动态查询
<a name="Oe8uP"></a>
#### 查询所有数据
运行UserMapperTest#getModelList1，输出如下：
```
39:10.552 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==>  Preparing: SELECT `id`, `name`, `age`, `salary` FROM `t_user` a 
39:10.611 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==> Parameters: 
39:10.639 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - <==      Total: 11
39:10.639 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 结果:[UserModel(id=2, name=ready, age=30, salary=50000.0), UserModel(id=3, name=ready, age=31, salary=1000.88), UserModel(id=4, name=javacode2018-1, age=31, salary=10000.0), UserModel(id=5, name=Tom-2, age=32, salary=20000.0), UserModel(id=6, name=javacode2018-2, age=32, salary=20000.0), UserModel(id=7, name=Tom-3, age=33, salary=30000.0), UserModel(id=8, name=javacode2018-3, age=33, salary=30000.0), UserModel(id=9, name=Tom-4, age=34, salary=40000.0), UserModel(id=10, name=javacode2018-4, age=34, salary=40000.0), UserModel(id=11, name=Tom-5, age=35, salary=50000.0), UserModel(id=12, name=javacode2018-5, age=35, salary=50000.0)]
```
可以看到sql是没有查询条件的。
<a name="SQ06B"></a>
#### 查询多个用户id对应的数据
运行UserMapperTest#getModelListByIds，输出如下：
```
39:38.000 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==>  Preparing: SELECT `id`, `name`, `age`, `salary` FROM `t_user` a WHERE a.`id` IN ( ? , ? , ? ) 
39:38.064 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==> Parameters: 2(Integer), 3(Integer), 4(Integer)
39:38.096 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - <==      Total: 3
39:38.097 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 结果:[UserModel(id=2, name=ready, age=30, salary=50000.0), UserModel(id=3, name=ready, age=31, salary=1000.88), UserModel(id=4, name=javacode2018-1, age=31, salary=10000.0)]
```
上面这个按照id列表查询也是比较常用的，比如在电商中查询订单列表，还需要查询每个订单对应的商品，此时可以先查询订单列表，然后在通过订单列表拿到所有的商品id集合，然后通过商品id集合去通过上面的方式检索商品信息，只需要2次查询就可以查询出订单及商品的信息了。
<a name="vzIwO"></a>
#### 多条件 & 指定返回的列
运行UserMapperTest#getModelList2，查询姓名中包含Tom以及薪资大于3万的用户id、姓名，输出如下：
```
41:12.185 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==>  Preparing: SELECT id , name FROM `t_user` a WHERE a.`name` like '%Tom%' AND a.`salary` >= ? 
41:12.275 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==> Parameters: 30000.0(Double)
41:12.311 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - <==      Total: 3
41:12.312 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 结果:[UserModel(id=7, name=Tom-3, age=null, salary=null), UserModel(id=9, name=Tom-4, age=null, salary=null), UserModel(id=11, name=Tom-5, age=null, salary=null)]
```
看一下上面select语句，select后面只有id,name2个字段，where后面有多个条件，这种查询也是比较常用的，有些表可能有几十个字段，可能只需要几个字段，就可以使用上面这种查询。
<a name="GoEQc"></a>
#### 条件过滤 & 排序 & 分页查询数据 & 只返回用户id、salary
运行UserMapperTest#getModelList3，查询姓名中包含Tom以及薪资大于3万的用户id，按照薪资倒叙，每页5条取第1页，输出如下：
```
44:00.719 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==>  Preparing: SELECT id , salary FROM `t_user` a WHERE a.`name` like '%Tom%' AND a.`salary` >= ? order by salary desc LIMIT ?,? 
44:00.775 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - ==> Parameters: 30000.0(Double), 0(Integer), 5(Integer)
44:00.805 [main] DEBUG c.j.m.chat01.UserMapper.getModelList - <==      Total: 3
44:00.806 [main] INFO  c.j.mybatis.chat01.UserMapperTest - 结果:[UserModel(id=11, name=null, age=null, salary=50000.0), UserModel(id=9, name=null, age=null, salary=40000.0), UserModel(id=7, name=null, age=null, salary=30000.0)]
```
大家主要看一下输出的sql，如下：
```
SELECT id , salary FROM `t_user` a WHERE a.`name` like '%Tom%' AND a.`salary` >= ? order by salary desc LIMIT ?,?
```
这个sql会根据查询条件，自动构建出sql，这点上面是最厉害的。
<a name="A9djy"></a>
### 案例总结
上面列举的一些用例基本上包含了对db所需的大部分操作，动态sql处理方面体现的最为强劲，如果自己写，需要写很多判断，而用MyBatis这么简单就实现了，在java代码中没有看到一个判断拼接语句，而这些sql的判断拼接都在一个文件中：user.xml中，这个就是MyBatis中核心的文件，需要写的sql及判断逻辑基本上都在这个xml中，大家可以认真去看一下这个xml文件。
<a name="uLAVb"></a>
## MyBatis开发项目的具体步骤
<a name="c4T8m"></a>
### 项目中引入MyBatis Maven配置
```xml
<dependency>
  <groupId>org.mybatis</groupId>
  <artifactId>mybatis</artifactId>
  <version>${mybatis.version}</version>
</dependency>
```
上面的`mybatis.version`版本。
<a name="YVFJA"></a>
### 创建MyBatis配置文件
MyBatis配置文件为xml格式，可以放在resource目录下面，如上面案例中的mybatis-config.xml，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
  <properties>
    <property name="driver" value="com.mysql.jdbc.Driver"/>
    <property name="url" value="jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8"/>
    <property name="username" value="root"/>
    <property name="password" value="root123"/>
  </properties>
  <environments default="development">
    <environment id="development">
      <transactionManager type="JDBC"/>
      <dataSource type="POOLED">
        <property name="driver" value="${driver}"/>
        <property name="url" value="${url}"/>
        <property name="username" value="${username}"/>
        <property name="password" value="${password}"/>
      </dataSource>
    </environment>
  </environments>
  <mappers>
    <mapper resource="mapper/user.xml"/>
  </mappers>
</configuration>
```
这个文件主要是对MyBatis进行全局配置，比如数据源、事务的配置，如上面的datasource元素用来配置数据源，数据源中就需要指定数据库的一些配置信息；还有其他更多的配置，此处先不做具体说明，后面慢慢来，整个系列完成之后，这些配置大家都会懂的。
<a name="ie330"></a>
### 创建mapper xml文件
如上面案例中的user.xml，大家在打开看看，需要对t_user表所有操作sql就写在这个文件中，user.xml文件是对t_user表的所有操作一般都会放在这个里面，MyBatis如何使用到这个文件呢，需要在上面的MyBatis配置文件中引入这个mapper文件，如案例中在mybatis-config.xml有下面这样的内容：
```xml
<mappers>
  <mapper resource="mapper/user.xml"/>
</mappers>
```
mappers元素中可以有多个mapper文件，开发的项目中可能有很多表需要操作，那么对应会有很多mapper xml文件，都需要在mappers元素中进行引入，然后MyBatis才会使用到。
<a name="tB8RJ"></a>
### 创建Mapper接口
开发者如何去调用user.xml中的各种操作去执行sql呢，这时就需要一个Mapper接口了，Mapper接口会和mapper xml建立映射关系，当调用Mapper接口中的方法的时候，会间接的调用到mapper xml中的各种数据的sql操作，Mapper接口如何和Mapper xml文件关联的呢？<br />大家去看一下user.xml文件中有个这样的一个配置：
```xml
<mapper namespace="com.javacode2018.mybatis.chat01.UserMapper">
```
注意上面的namespace的值，对应的是UserMapper这个接口完整的引用，通过这个namespace，UserMapper接口就可以user.xml建立了映射关系。<br />user.xml中又有很多db操作，这些操作会和UserMapper接口中的方法建立映射关系，当调用UserMapper中的方法的时候，间接的会调用到user.xml中对应的操作。<br />如user.xml中有下面一段配置：
```xml
<!-- 批量插入 -->
<insert id="insertBatch" parameterType="map">
  <![CDATA[ INSERT INTO `t_user` (`id`, `name`, `age`, `salary`) VALUES ]]>
  <foreach collection="list" separator="," item="item">
    (#{item.id}, #{item.name}, #{item.age}, #{item.salary})
  </foreach>
</insert>
```
而UserMapper中有个insertBatch方法和上面这个insert批量插入对应，如下：
```java
/**
 * 批量插入用户信息
 *
 * @param userModelList
 */
void insertBatch(List<UserModel> userModelList);
```
所以当调用UserMapper中的insertBatch方法的时候，会间接调用到user.xml中的 id="insertBatch"这个操作。<br />提示一下：接口和mapper xml映射起来间接调用，是通过java动态代理实现的。<br />下面就可以使用MyBatis来操作db了。
<a name="u3azS"></a>
### 通过MyBatis获取Mapper接口执行对db的操作
可以通过mapper接口来执行对db的操作，获取Mapper的主要代码如下：
```java
SqlSessionFactoryBuilder sqlSessionFactoryBuilder = new SqlSessionFactoryBuilder();
SqlSessionFactory sqlSessionFactory = sqlSessionFactoryBuilder.build(Resources.getResourceAsStream("mybatis-config.xml"));
SqlSession sqlSession = sqlSessionFactory.openSession(true);
UserMapper mapper = sqlSession.getMapper(UserMapper.class);
```
上面代码中使用到了MyBatis中的核心组件，具体来看一下。
<a name="O22k9"></a>
### Mybatis核心对象介绍
<a name="m2IUK"></a>
#### `SqlSessionFactoryBuilder`
这个是一个构建器，通过名字大家也可以感觉到`SqlSessionFactoryBuilder`构建器，是用来构建`SqlSessionFactory`对象的，`SqlSessionFactoryBuilder`可以通过读取MyBatis的配置文件，然后构建一个`SqlSessionFactory`对象，一个项目中有很多mapper xml文件，如果每次操作都去重新解析是非常慢的，那么怎么办？<br />能不能第一次解析好然后放在内存中，以后直接使用，SqlSessionFactoryBuilder就是搞这个事情的，将MyBatis配置文件、mapper xml文件、mapper xml文件和Mapper 接口的映射关系，这些都先给解析好，然后放在java对象中，java对象存在于内存中，内存中访问会非常快的，那么每次去用的时候就不需要重新去解析xml了，`SqlSessionFactoryBuilder`解析配置之后，生成的对象就是`SqlSessionFactory`，这个是一个重量级的对象，创建他是比较耗时的，所以一般一个db会创建一个`SqlSessionFactory`对象，然后在系统运行过程中会一直存在，而`SqlSessionFactoryBuilder`用完了就可以释放了。
<a name="mKYyR"></a>
#### `SqlSessionFactory`
通过名字可以知道，这个是一个工厂，是用来创建`SqlSession`的工厂，`SqlSessionFactory`是一个重量级的对象，一般一个db对应一个`SqlSessionFactory`对象，系统运行过程中会一直存在。<br />`SqlSessionFactory`是一个接口，这个接口有2个实现`DefaultSqlSessionFactory`和`SqlSessionManager`，一般都是通过`SqlSessionFactoryBuilder`来创建`SqlSessionFactory`对象。<br />通过`SqlSessionFactoryBuilder`来创建`SqlSessionFactory`对象主要有2种方式，一种通过读取MyBatis配置文件的方式，另外一种是硬编码的方式。
<a name="bp5NO"></a>
#### `SqlSession`
通过jdbc操作数据库需要先获取一个`Connection`连接，然后拿着这个连接去对db进行操作，在MyBatis中`SqlSession`就类似于jdbc中`Connection`连接对象，在MyBatis中叫做Sql会话对象，一般一个db操作使用一个`SqlSession`对象，所以这个对象一般是方法级别的，方法结束之后，这个对象就销毁了，这个对象可以调用`sqlSessionFactory.openSession`的方法来进行获取。<br />可以直接通过`SqlSession`对象来调用mapper xml中各种db操作，需要指定具体的操作的id，id的格式为namespace.操作的id。
<a name="zLDmB"></a>
#### Mapper接口
可以通过`SqlSession`直接调用mapper xml中的db操作，不过更简单的以及推荐的方式是使用Mapper接口，Mapper接口中的方法和mapper xml文件中的各种db操作建立了映射关系，是通过Mapper接口完整名称+方法名称和mapper xml中的namespace+具体操作的id来进行关联的，然后直接调用Mapper接口中的方法就可以间接的操作db了，使用想当方便，Mapper接口需要通过`SqlSession`获取，传入Mapper接口对应的Class对象，然后会返回这个接口的实例，如：
```java
UserMapper mapper = sqlSession.getMapper(UserMapper.class);
```
