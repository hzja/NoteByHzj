JavaSpringMyBatis<br />主要介绍Mybatis和Spring集成的两种方式。<br />目前注解的方式用的比较多，所以主要介绍注解的方式，xml的方式这里就暂时不介绍了。<br />注解的方式Mybatis集成Spring主要有2种方式：<br />1、方式1：mapper xml文件放在resource目录，和Mapper接口不在一个目录的情况<br />2、方式2：mapper xml文件和Mapper接口在同一个目录<br />还会介绍另外一个点：**多数据库的时候，如何配置？**
<a name="dyMTA"></a>
## 准备db脚本
```sql
/*创建数据库javacode2018*/
DROP DATABASE IF EXISTS `javacode2018`;
CREATE DATABASE `javacode2018`;
USE `javacode2018`;

/*创建表结构*/
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE t_user (
  id BIGINT AUTO_INCREMENT PRIMARY KEY COMMENT '主键，用户id，自动增长',
  `name` VARCHAR(32) NOT NULL DEFAULT '' COMMENT '姓名'
) COMMENT '用户表';

SELECT * FROM t_user;
```
下面来通过Mybatis对上面的表执行插入和查询的操作。
<a name="GYfkQ"></a>
## 方式1
<a name="i09rK"></a>
### 项目结构
这种方式，Mapper接口和mapper xml文件放在不同的位置，mapper xml文件放在resource/mapper目录，如下图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690122623980-58d845c7-c324-463b-b11d-951d2e57980c.png#averageHue=%23fdfdfa&clientId=u573bfe35-7b66-4&from=paste&id=uce002fec&originHeight=569&originWidth=700&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub87cc6e8-db73-4ef4-9dae-db9cda08d59&title=)
<a name="k2Pqn"></a>
### UserModel
```java
package com.javacode2018.mybatis.model;

import lombok.*;

@Getter
@Setter
@Builder
@NoArgsConstructor
@AllArgsConstructor
@ToString
public class UserModel {
    private Long id;
    private String name;
}
```
<a name="jD5YY"></a>
### UserMapper
这个是mapper接口，类上面需要添加`@Mapper`注解，用来标注这是一个Mapper接口
```java
package com.javacode2018.mapper;

import com.javacode2018.mybatis.model.UserModel;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface UserMapper {
    void insert(UserModel userModel);
    List<UserModel> getList();
}
```
<a name="TtGbH"></a>
### UserMapper.xml
在spring-series\lesson-005-mybatis\src\main\resources\mapper中添加UserMapper.xml文件，这个文件名大家可以随意起名，不一定非要叫UserMapper，具体位置大家可以看看上面的项目结构图
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.mapper.UserMapper">

  <insert id="insert" parameterType="com.javacode2018.mybatis.model.UserModel" keyProperty="id" useGeneratedKeys="true">
    <CDATA[ INSERT INTO `t_user` (name) VALUES (#{name})]]>
    </insert>

  <select id="getList" resultType="com.javacode2018.mybatis.model.UserModel">
    <CDATA[
      SELECT id,name FROM t_user
      ]]>
    </select>

</mapper>
```
<a name="SRyeA"></a>
### IUserService
```java
package com.javacode2018.service;

import com.javacode2018.mybatis.model.UserModel;

import java.util.List;

public interface IUserService {
    /**
     * 插入用户信息
     *
     * @param userModel
     * @return
     */
    UserModel insert(UserModel userModel);

    /**
     * 查询用户所有记录
     *
     * @return
     */
    List<UserModel> getList();
}
```
<a name="yQeoE"></a>
### UserServiceImpl
IUserService的实现类，内部将Usermapper通过`@Autowired`注入进来，通过userMapper来访数据库，userMapper实际上是Mybatis创建的一个代理对象。
```java
package com.javacode2018.service;

import com.javacode2018.mapper.UserMapper;
import com.javacode2018.mybatis.model.UserModel;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
public class UserServiceImpl implements IUserService {
    @Autowired
    private UserMapper userMapper;

    @Transactional(rollbackFor = Exception.class)
    @Override
    public UserModel insert(UserModel userModel) {
        userMapper.insert(userModel);
        return userModel;
    }

    @Transactional(propagation = Propagation.SUPPORTS, readOnly = true)
    @Override
    public List<UserModel> getList() {
        return userMapper.getList();
    }
}
```
<a name="JIQFp"></a>
### Spring配置类MainConfig
```java
package com.javacode2018;

import com.javacode2018.mapper.UserMapper;
import org.apache.ibatis.annotations.Mapper;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.Resource;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.sql.DataSource;
import java.io.IOException;

@EnableTransactionManagement //开启事务支持
@ComponentScan
@Configuration
// @MapperScan这个注解是关键，会扫描标记有@Mapper注解的Mapper接口，将其注册到spring容器中
@MapperScan(basePackageClasses = {UserMapper.class}, annotationClass = Mapper.class)
public class MainConfig {
    //定义数据源
    @Bean
    public DataSource dataSource() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }

    //定义事务管理器
    @Bean
    public TransactionManager transactionManager(DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }

    //定义SqlSessionFactoryBean，用来创建SqlSessionFactory
    @Bean
    public SqlSessionFactoryBean sqlSessionFactoryBean(DataSource dataSource) throws IOException {
        SqlSessionFactoryBean sqlSessionFactoryBean = new SqlSessionFactoryBean();
        sqlSessionFactoryBean.setDataSource(dataSource);
        //获取所有mapper.xml文件
        Resource[] resources = new PathMatchingResourcePatternResolver().getResources("classpath*:mapper/**/*.xml");
        sqlSessionFactoryBean.setMapperLocations(resources);
        return sqlSessionFactoryBean;
    }
}
```
这个类中有2个关键信息：<br />1、`**@MapperScan**`**注解**：这个注解是关键，会扫描标记有`@Mapper`注解的Mapper接口类，然后给Mapper接口生成代理对象，将其注册到Spring容器中，这个注解有几个属性需要注意下

- value或者`basePackages`：String类型的数组，用来指定扫描的包
- `basePackageClasses`：可以指定一个类，扫描范围为这个类所在的包及其所有子包
- `sqlSessionFactoryRef`：用来指定`sqlSessionFactory`的bean名称，当系统中需要操作多个库的时候，每个库对应一个`SqlSessionFactory`，此时可以通过这个属性指定需要使用哪个`SqlSessionFactory`。

2、**定义**`**SqlSessionFactoryBean**`：通过名字可以看出，这个是用来生成`SqlSessionFactory`的，内部需要指定数据源和本地mapper xml的位置，将mapper xml文件放在resouce/mapper文件中，此处采用通配符的方式，加载classpath中mapper目录及子目录中的所有xml文件
```java
Resource[] resources = new PathMatchingResourcePatternResolver().getResources("classpath*:mapper/**/*.xml");
sqlSessionFactoryBean.setMapperLocations(resources);
```
<a name="sRQrH"></a>
### 测试效果
来个测试类
```java
package com.javacode2018;

import com.javacode2018.mybatis.model.UserModel;
import com.javacode2018.service.IUserService;
import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.util.List;

public class MybatisSpringTest {
    @Test
    public void insert() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig.class);
        IUserService userService = context.getBean(IUserService.class);
        UserModel userModel = UserModel.builder().name("张三").build();
        userService.insert(userModel);
        System.out.println(userModel);
    }

    @Test
    public void getList() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig.class);
        IUserService userService = context.getBean(IUserService.class);
        List<UserModel> userModelList = userService.getList();
        System.out.println(userModelList);
    }
}
```
insert方法用来插入数据，执行输出
```
UserModel(id=1, name=张三)
```
getList方法用来查询表中所有数据，运行输出
```
[UserModel(id=1, name=张三)]
```
<a name="IfbNW"></a>
### 小结：回顾步骤
主要分为3步<br />1、Mapper接口上添加`@Mapper`注解
```java
@Mapper
public interface UserMapper
```
2、定义mapper xml文件，如将user.xml文件放在了spring-series\lesson-005-mybatis\src\main\resources\mapper目录<br />3、Spring配置类中添加`@MapperScan`注解，用来扫描`@Mapper`标注的类，将其注册到Spring容器中
```java
@MapperScan(basePackageClasses = {UserMapper.class}, annotationClass = Mapper.class)
```
4、定义`SqlSessionFactoryBean`
```java
@Bean
public SqlSessionFactoryBean sqlSessionFactoryBean(DataSource dataSource) throws IOException {
    SqlSessionFactoryBean sqlSessionFactoryBean = new SqlSessionFactoryBean();
    //指定数据源
    sqlSessionFactoryBean.setDataSource(dataSource);
    //获取所有mapper.xml文件
    Resource[] resources = new PathMatchingResourcePatternResolver().getResources("classpath*:mapper/**/*.xml");
    //设置mapper的位置
    sqlSessionFactoryBean.setMapperLocations(resources);
    return sqlSessionFactoryBean;
}
```
<a name="OwfqY"></a>
## 方式2
<a name="ZkR9H"></a>
### 项目结构
这种方式，将mapper xml文件和mapper接口放在同一个目录，下面新建一个项目lesson-006-mybatis，结构如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690122623959-0fee24bd-dde0-4a94-a4ae-cabdac8a5933.png#averageHue=%23fcfaf9&clientId=u573bfe35-7b66-4&from=paste&id=u9de63768&originHeight=575&originWidth=512&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf687fd7f-8073-4527-9121-d9a40dc57fc&title=)<br />对比两个图，主要的区别：方式1中将UserMapper.xml文件放在了src\main\resources\mapper目录中，而这种将UserMapper.xml和UserMapper接口放在了同一个目录。
<a name="Ivz4H"></a>
### 修改`SqlSessionFactoryBean`的定义
需要修改MainConfig中`SqlSessionFactoryBean`的定义，如下，更简洁了，不需要在指定mapper xml的位置了，这里需要注意一点，方式2中将mapper xml文件和mapper接口放在一个目录的时候，**这2个文件的名字必须一样**，这样在定义`SqlSessionFactoryBean`的时候才不需要指定mapper xml的位置。
```java
@Bean
public SqlSessionFactoryBean sqlSessionFactoryBean(DataSource dataSource) throws IOException {
    SqlSessionFactoryBean sqlSessionFactoryBean = new SqlSessionFactoryBean();
    sqlSessionFactoryBean.setDataSource(dataSource);
    return sqlSessionFactoryBean;
}
```
<a name="TZ1HV"></a>
### 调整一下pom.xml配置
pom文件中需要加入下面配置，这样Maven在打包的时候，才会将src/main/java目录中的xml打包目录目录中，否则打包之后这些xml都丢失了，这个一定要注意。
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
<a name="mwEEa"></a>
### 小结
如果使用方式2需要注意下面2点：<br />1、mapper接口和mapper xml必须同名，且放在同一个目录<br />2、需要在pom.xml文件中加入指定的配置，否则打包之后xml丢失了
<a name="CnYi8"></a>
## 集成多个数据源
有时候系统中需要用到多个数据源，每个数据源对应一个`SqlSessionFactory`，`@MapperScan`注解中可以通过`sqlSessionTemplateRef`来指定`SqlSessionFactory`的bean名称。
<a name="hLxcP"></a>
### 多数据源开发步骤
<a name="l8jz8"></a>
#### 步骤1：`@MapperScan`中指定`SqlSessionFactory`的bean名称
每个db对应一个模块，通过包区分不同的模块，每个模块中指定一个Spring的配置类，配置类需配置3个bean：数据源、事务管理器、`SqlSessionFactory`，下面是一个模块的Spring配置类，注意下面代码的`@MapperScan`注解，当系统中有多个`sqlSessionFactory`的时候需要用过`sqlSessionFactoryRef`属性指定了`sqlSessionFactory`的bean名称。<br />由于有多个数据源，所以代码中需要通过`@Qualifier(DATASOURCE_BEAN_NAME)`来限定注入的datasource的bean名称。
```java
package com.javacode2018.mybatis.module1;

import org.apache.ibatis.annotations.Mapper;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.TransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import javax.sql.DataSource;
import java.io.IOException;

@Configuration
@MapperScan(annotationClass = Mapper.class, sqlSessionFactoryRef = Module1SpringConfig.SQL_SESSION_FACTORY_BEAN_NAME)
@ComponentScan
@EnableTransactionManagement
public class Module1SpringConfig {
    public final static String DATASOURCE_BEAN_NAME = "dataSourcemodule1";
    public final static String TRANSACTION_MANAGER_BEAN_NAME = "transactionManagermodule1";
    public final static String SQL_SESSION_FACTORY_BEAN_NAME = "sqlSessionFactorymodule1";

    //定义数据源
    @Bean
    public DataSource dataSourcemodule1() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/module1?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }

    //定义事务管理器
    @Bean
    public TransactionManager transactionManagermodule1(@Qualifier(DATASOURCE_BEAN_NAME) DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }

    //定义SqlSessionFactoryBean，用来创建SqlSessionFactory
    @Bean
    public SqlSessionFactoryBean sqlSessionFactorymodule1(@Qualifier(DATASOURCE_BEAN_NAME) DataSource dataSource) throws IOException {
        SqlSessionFactoryBean sqlSessionFactoryBean = new SqlSessionFactoryBean();
        sqlSessionFactoryBean.setDataSource(dataSource);
        return sqlSessionFactoryBean;
    }
}
```
<a name="mUunU"></a>
#### 步骤2：指定事务管理器
每个db对应一个datasource，每个datasource需要指定一个事务管理器，通过`@Transactional`注解的`transactionManager`属性指定事务管理器，如下
```java
@Transactional(transactionManager = Module1SpringConfig.TRANSACTION_MANAGER_BEAN_NAME, rollbackFor = Exception.class)
```
<a name="fQnuM"></a>
### 案例
下面通过案例来感受一下多个db的时候如何实现的。
<a name="x3ITf"></a>
#### 项目结构图
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690122624025-a794b0b7-b673-4041-bdcb-6994787ba731.png#averageHue=%23faf9f8&clientId=u573bfe35-7b66-4&from=paste&id=u37be442f&originHeight=629&originWidth=463&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8786a0f2-f6d6-4890-9655-c29b0a61ab0&title=)

- 案例中准备了2个数据库：module1和module2，每个库中创建了一个t_user表
- module1库对应的所有类放在com.javacode2018.mybatis.module1包中
- module2库对应的所有类放在com.javacode2018.mybatis.module2包中
- Module1SpringConfig是module1的Spring配置类
- Module2SpringConfig是module2的Spring配置类
<a name="XL0uz"></a>
#### 项目运行
**1、导入项目到idea中**<br />**2、执行sql脚本**
```sql
/*创建2个库：module1和module2，每个库中创建一个t_user表*/
DROP DATABASE IF EXISTS `module1`;
CREATE DATABASE `module1`;

DROP DATABASE IF EXISTS `module2`;
CREATE DATABASE `module2`;

/*创建表结构*/
DROP TABLE IF EXISTS module1.t_user;
CREATE TABLE module1.t_user (
  id BIGINT AUTO_INCREMENT PRIMARY KEY COMMENT '主键，用户id，自动增长',
  `name` VARCHAR(32) NOT NULL DEFAULT '' COMMENT '姓名'
) COMMENT '用户表';

DROP TABLE IF EXISTS module2.t_user;
CREATE TABLE module2.t_user (
  id BIGINT AUTO_INCREMENT PRIMARY KEY COMMENT '主键，用户id，自动增长',
  `name` VARCHAR(32) NOT NULL DEFAULT '' COMMENT '姓名'
) COMMENT '用户表';

SELECT * FROM module1.t_user;
SELECT * FROM module2.t_user;
```
**3、运行测试用例**
```java
public class MybatisSpringTest {
    @Test
    public void testModel1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig.class);
        IUser1Service user1Service = context.getBean(IUser1Service.class);
        User1Model userModel = User1Model.builder().name("张三").build();
        user1Service.insert(userModel);
        System.out.println(userModel);
        System.out.println(user1Service.getList());
    }

    @Test
    public void testModel2() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(MainConfig.class);
        IUser2Service user2Service = context.getBean(IUser2Service.class);
        User2Model userModel = User2Model.builder().name("李四").build();
        user2Service.insert(userModel);
        System.out.println(userModel);
        System.out.println(user2Service.getList());
    }
}
```
这个用例中有2个方法，大家可以运行一下，看看2个db的时候，效果如何。
<a name="t8vgr"></a>
### 小结
系统中集成多个数据源这种方式，大家作为了解就可以了，不建议这么使用，通常一个业务库对应一个module，模块与模块之间相互隔离，独立部署，便于维护扩展。
