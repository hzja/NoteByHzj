Mybatis
<a name="Bdrn5"></a>
## 快速入门
<a name="wMKAn"></a>
### 准备数据库
MySQL中执行下面sql：
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
上面创建了一个数据库：javacode2018，一个用户表t_user。
<a name="qKkeR"></a>
### 需求
使用Mybatis来实现对t_user表增删改查。
<a name="iYVSc"></a>
### 使用IDEA创建项目
在mybatis-series项目中创建另外一个模块chat02，过程如下：<br />选中mybatis-series，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690297664577-3dc03788-749d-485e-ac8d-db19c76cda83.png#averageHue=%23f9f3f2&clientId=u9f557eb0-20b1-4&from=paste&id=u37278530&originHeight=296&originWidth=546&originalType=url&ratio=1.5&rotation=0&showTitle=false&status=done&style=none&taskId=u099ed8a6-3ab8-4d5d-983a-62c99d06bce&title=)<br />点击右键->New->Module，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690297664397-598f1146-a69f-4f97-8dbb-c8627efb9381.png#averageHue=%23e6e5e4&clientId=u9f557eb0-20b1-4&from=paste&id=u60e2ea72&originHeight=421&originWidth=929&originalType=url&ratio=1.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8286bc03-9119-4323-9e75-0397208607b&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690297664510-545e7db4-c330-41b9-8460-096b214646c1.jpeg#averageHue=%23e5e5e5&clientId=u9f557eb0-20b1-4&from=paste&id=uf19a3c75&originHeight=696&originWidth=1080&originalType=url&ratio=1.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufa4be689-239d-4cfd-9944-fc6c068f540&title=)<br />选中上图中的Maven，点击Next，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690297664592-e255082d-4484-488f-87e3-f084ea5dbb22.jpeg#averageHue=%23e6e2e2&clientId=u9f557eb0-20b1-4&from=paste&id=u766dae37&originHeight=696&originWidth=1080&originalType=url&ratio=1.5&rotation=0&showTitle=false&status=done&style=none&taskId=u328e71ca-a56d-4e4b-bbee-6902ce96b84&title=)<br />出现下面窗口：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690297664507-aeb463ea-fbb4-41a1-86e1-f3491849e871.png#averageHue=%23e7e5e5&clientId=u9f557eb0-20b1-4&from=paste&id=u062afa23&originHeight=696&originWidth=1080&originalType=url&ratio=1.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0ae43e73-092e-4521-815c-04d94379928&title=)<br />上图中输入ArtifactId为chat02，点击Next，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690297665164-5264d29f-3c12-461f-97ac-bff11cd30610.png#averageHue=%23e9e7e6&clientId=u9f557eb0-20b1-4&from=paste&id=u1f47ddca&originHeight=696&originWidth=1080&originalType=url&ratio=1.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub7028610-48c4-4695-9db0-eeb40ef69ba&title=)<br />点击上图中的Finish完成chat02模块的创建，项目结构如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690297665366-72a12fa9-9c06-4bf0-b0e5-e89d1d09600c.png#averageHue=%23f4f3f1&clientId=u9f557eb0-20b1-4&from=paste&id=u9990c0eb&originHeight=510&originWidth=1034&originalType=url&ratio=1.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4476726e-8531-41e3-8bad-c284238a718&title=)
<a name="P2n21"></a>
### pom.xml中引入Mybatis依赖
```xml
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
```
上面引入了依赖Mybatis、MySQL驱动、Lombok支持、junit、Logback支持，其实运行Mybatis只需要引入下面这一个构件就行了：
```xml
<dependency>
  <groupId>org.mybatis</groupId>
  <artifactId>mybatis</artifactId>
</dependency>
```
注意：上面pom引入的构建中没有写版本号，是因为构件的版本号在父pom.xml中已经声明了，所以chat03/pom.xml中就不需要再去写了。
<a name="QawPi"></a>
### 配置Mybatis全局配置文件
使用Mybatis操作数据库，那么当然需要配置数据库相关信息，这个需要在Mybatis全局配置文件中进行配置。<br />Mybatis需提供一个全局配置的xml文件，可以在这个配置文件中对Mybatis进行配置，如事务的支持，数据源的配置等等，这个属于配置文件，一般放在main/resource中。<br />在chat03/src/main/resource中创建mybatis-config.xml文件，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
  <!-- 环境配置，可以配置多个环境 -->
  <environments default="chat03">
    <!-- 
    environment用来对某个环境进行配置
    id：环境标识，唯一
    -->
    <environment id="chat03">
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
</configuration>
```
解释一下。
<a name="D8a1x"></a>
#### configuration元素
这个是Mybatis全局配置文件的根元素，每个配置文件只有一个
<a name="TlWq3"></a>
#### environments元素
用来配置Mybatis的环境信息，什么是环境？比如开发环境、测试环境、线上环境，这3个环境中的数据库可能是不一样的，可能还有更多的环境。<br />environments元素中用来配置多个环境的，具体的一个环境使用environment元素进行配置，environment元素有个id用来标识某个具体的环境。<br />配置了这么多环境，那么Mybatis具体会使用哪个呢？<br />environments元素有个default属性，用来指定默认使用哪个环境，如上面默认使用的是chat03。
<a name="QtZCw"></a>
#### environment元素
用来配置具体的环境信息，这个元素下面有两个子元素：`**transactionManager**`**和dataSource**

- transactionManager元素用来配置事务工厂的，有个type属性，type的值必须是`org.apache.ibatis.transaction.TransactionFactory`接口的实现类，`TransactionFactory`看名字就知道是一个工厂，用来创建事务管理器`org.apache.ibatis.transaction.Transaction`对象的，`TransactionFactory`接口默认有2个实现：
```
org.apache.ibatis.transaction.managed.ManagedTransactionFactory
org.apache.ibatis.transaction.jdbc.JdbcTransactionFactory
```
一般情况下使用`org.apache.ibatis.transaction.jdbc.JdbcTransactionFactory`这个，Mybatis和其他框架集成，比如和spring集成，事务交由spring去控制，spring中有`TransactionFactory`接口的一个实现`org.mybatis.spring.transaction.SpringManagedTransactionFactory`，有兴趣的朋友可以去研究一下，这个到时候讲到spring的使用会详细说。

- dataSource元素这个用来配置数据源的，type属性的值必须为接口`org.apache.ibatis.datasource.DataSourceFactory`的实现类，`DataSourceFactory`也是一个工厂，用来创建数据源javax.sql.DataSource对象的，Mybatis中这个接口默认有3个实现类：
```
org.apache.ibatis.datasource.jndi.JndiDataSourceFactory
org.apache.ibatis.datasource.pooled.PooledDataSourceFactory
org.apache.ibatis.datasource.unpooled.UnpooledDataSourceFactory
```
使用第2个`org.apache.ibatis.datasource.pooled.PooledDataSourceFactory`，这个用来创建一个数据库连接池类型的数据源，可以实现数据库连接共用，减少连接重复创建销毁的时间。配置数据源需要指定数据库连接的属性信息，比如：驱动、连接db的url、用户名、密码，这个在dataSource元素下面的property中配置，property元素的格式：
<a name="PCNPA"></a>
### 创建Mapper xml文件
需要对t_user表进行操作，需要写sql，sql写在什么地方呢？<br />在Mybatis中一般将一个表的所有sql操作写在一个mapper xml中，一般命名为XXXMapper.xml格式。<br />创建文件chat02/src/main/resource/mapper/UserMapper.xml，内容如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.javacode2018.chat02.UserMapper">
</mapper>
```
mapper xml根元素为mapper，这个元素有个namespace属性，系统中会有很多表，每个表对应一个Mapper xml，为了防止mapper文件重复，需要给每个mapper xml文件需要指定一个namespace，通过这个可以区分每个mapper xml文件，上面指定为com.javacode2018.chat02.UserMapper。<br />一会对t_user表的所有操作相关的sql，都会写在上面这个xml中。
<a name="Ee96R"></a>
### Mybatis全局配置文件中引入Mapper xml文件
UserMapper.xml写好了，如何让Mybatis知道这个文件呢，此时需要在mybatis-config.xml全局配置文件中引入UserMapper.xml，在mybatis-config.xml加入下面配置：
```xml
<mappers>
  <mapper resource="mapper/UserMapper.xml" />
</mappers>
```
mappers元素下面有多个mapper元素，通过mapper元素的resource属性可以引入Mapper xml文件，resource是相对于classes的路径。<br />上面说的都是一些配置文件，配置文件都ok了，下面就需要将Mybatis跑起来了，此时需要使用到Mybatis中的一些java对象了。
<a name="z20U7"></a>
### 构建`SqlSessionFactory`对象
```java
//指定mybatis全局配置文件
String resource = "mybatis-config.xml";
//读取全局配置文件
InputStream inputStream = Resources.getResourceAsStream(resource);
//构建SqlSessionFactory对象
SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
```
`SqlSessionFactory`是一个接口，是一个重量级的对象，`SqlSessionFactoryBuilder`通过读取全局配置文件来创建一个`SqlSessionFactory`，创建这个对象是比较耗时的，主要耗时在对Mybatis全局配置文件的解析上面，全局配置文件中包含很多内容，`SqlSessionFactoryBuilder`通过解析这些内容，创建了一个复杂的`SqlSessionFactory`对象，这个对象的生命周期一般和应用的生命周期是一样的，随着应用的启动而创建，随着应用的停止而结束，所以一般是一个全局对象，一般情况下一个db对应一个`SqlSessionFactory`对象。
<a name="oF6Ef"></a>
### 构建`SqlSession`对象
`SqlSession`相当于jdbc中的`Connection`对象，相当于数据库的一个连接，可以用SqlSession来对db进行操作：**如执行sql、提交事务、关闭连接等等**，需要通过`SqlSessionFactory`来创建`SqlSession`对象，`SqlSessionFactory`中常用的有2个方法来创建`SqlSession`对象，如下：
```java
//创建一个SqlSession，默认不会自动提交事务
SqlSession openSession();
//创建一个SqlSession,autoCommit：指定是否自动提交事务
SqlSession openSession(boolean autoCommit);
```
SqlSession接口中很多方法，直接用来操作db，方法清单如下，大家眼熟一下：
```java
<T> T selectOne(String statement);
<T> T selectOne(String statement, Object parameter);
<E> List<E> selectList(String statement);
<E> List<E> selectList(String statement, Object parameter);
<E> List<E> selectList(String statement, Object parameter, RowBounds rowBounds);
<K, V> Map<K, V> selectMap(String statement, String mapKey);
<K, V> Map<K, V> selectMap(String statement, Object parameter, String mapKey);
<K, V> Map<K, V> selectMap(String statement, Object parameter, String mapKey, RowBounds rowBounds);
<T> Cursor<T> selectCursor(String statement);
<T> Cursor<T> selectCursor(String statement, Object parameter);
<T> Cursor<T> selectCursor(String statement, Object parameter, RowBounds rowBounds);
void select(String statement, Object parameter, ResultHandler handler);
void select(String statement, ResultHandler handler);
void select(String statement, Object parameter, RowBounds rowBounds, ResultHandler handler);
int insert(String statement);
int insert(String statement, Object parameter);
int update(String statement);
int update(String statement, Object parameter);
int delete(String statement);
int delete(String statement, Object parameter);
void commit();
void commit(boolean force);
void rollback();
void rollback(boolean force);
List<BatchResult> flushStatements();
void close();
void clearCache();
Configuration getConfiguration();
<T> T getMapper(Class<T> type);
Connection getConnection();
```
上面以select开头的可以对db进行查询操作，insert相关的可以对db进行插入操作，update相关的可以对db进行更新操作。
<a name="zvCSr"></a>
### 引入lombok支持（非必须）
**声明一下：lombok不是Mybatis必须的，为了简化代码而使用的，以后会经常使用。**<br />Lombok能以简单的注解形式来简化java代码，提高开发人员的开发效率。例如开发中经常需要写的JavaBean，都需要花时间去添加相应的getter/setter，也许还要去写构造器、equals等方法，而且需要维护，当属性多时会出现大量的getter/setter方法，这些显得很冗长也没有太多技术含量，一旦修改属性，就容易出现忘记修改对应方法的失误。<br />Lombok能通过注解的方式，在编译时自动为属性生成构造器、getter/setter、equals、hashcode、toString方法。出现的神奇就是在源码中没有getter和setter方法，但是在编译生成的字节码文件中有getter和setter方法。这样就省去了手动重建这些代码的麻烦，使代码看起来更简洁些。
<a name="kwFpL"></a>
#### lombok的使用步骤

1. 先在IDEA中安装lombok插件打开idea，点击File->Settings->plugins，然后搜索Lombok Plugin，点击安装就可以了。
2. Maven中引入lombok支持
```xml
<dependency>
  <groupId>org.projectlombok</groupId>
  <artifactId>lombok</artifactId>
  <version>1.18.10</version>
  <scope>provided</scope>
</dependency>
```

3. 代码中使用lombok相关功能
<a name="HhSHm"></a>
### 引入logback（非必须）
**声明一下：日志框架Mybatis中也不是必须的，不用配置也可以正常运行。**<br />为了方便查看Mybatis运行过程中产生的日志，比如：执行的sql、sql的参数、sql的执行结果等等调试信息，需要引入日志框架的支持，logback是一个很好的日志框架，此处就使用这个
<a name="Vmgba"></a>
#### Mybatis中集成logback步骤

1. maven中引入logback支持
```xml
<dependency>
  <groupId>ch.qos.logback</groupId>
  <artifactId>logback-classic</artifactId>
  <version>1.2.3</version>
</dependency>
```

2. src/main/resources中创建logback.xml文件：
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
logback.xml具体的写法不是本文讨论的范围，有兴趣的朋友可以去研究一下logback具体的用法。<br />上面xml中配置了com.javacode2018包中所有的类，使用logback输出日志的时候，debug级别及以上级别的日志会输出到控制台，方便查看。
<a name="BF29B"></a>
### 写一个测试用例
在chat02/src/test下创建一个类:
```
com.javacode2018.chat02.UserTest
```
内容如下：
```java
package com.javacode2018.chat02;

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
public class UserTest {

    private SqlSessionFactory sqlSessionFactory;

    @Before
    public void before() throws IOException {
        //指定mybatis全局配置文件
        String resource = "mybatis-config.xml";
        //读取全局配置文件
        InputStream inputStream = Resources.getResourceAsStream(resource);
        //构建SqlSessionFactory对象
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        this.sqlSessionFactory = sqlSessionFactory;
    }

    @Test
    public void sqlSession() {
        SqlSession sqlSession = this.sqlSessionFactory.openSession();
        log.info("{}", sqlSession);
    }

}
```
上面代码中有个`@Slf4j`注解，这个是lombok提供的，可以在这个类中生成下面代码：
```java
private static final org.slf4j.Logger log = org.slf4j.LoggerFactory.getLogger(UserTest.class);
```
运行一下上面的用例：sqlSession方法，输出如下：
```
45:51.289 [main] INFO  com.javacode2018.chat02.UserTest - org.apache.ibatis.session.defaults.DefaultSqlSession@1f021e6c
```
<a name="ICrVo"></a>
## 使用`SqlSesion`执行sql操作
<a name="j7RTE"></a>
### `SqlSession`常见的用法
`SqlSession`相当于一个连接，可以使用这个对象对db执行增删改查操作，操作完毕之后需要关闭，使用步骤：

1. 获取`SqlSession`对象：通过该`sqlSessionFactory.openSession`方法获取SqlSession对象
2. 对db进行操作：使用`SqlSession`对象进行db操作
3. 关闭`SqlSession`对象：`sqlSession.close();`

常见的使用方式如下：
```java
//获取SqlSession
SqlSession sqlSession = this.sqlSessionFactory.openSession();
try {
    //执行业务操作，如：增删改查
} finally {
    //关闭SqlSession
    sqlSession.close();
}
```
上面将SqlSession的关闭放在`finally`块中，确保close()一定会执行。更简单的方式是使用java中的`try()`的方式，如下：
```java
try (SqlSession sqlSession = this.sqlSessionFactory.openSession();) {
    //执行业务操作，如：增删改查
}
```
<a name="ME1W5"></a>
### 新增操作
需求：传入UserModel对象，然后将这个对象的数据插入到t_user表中。
<a name="pSHVW"></a>
#### 创建一个`UserModel`
新建一个com.javacode2018.chat02.UserModel类，代码如下：
```java
package com.javacode2018.chat02;

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
这个类的字段和t_user表对应。
<a name="D3CYm"></a>
#### UserMapper.xml中定义插入操作
对t_user表的所有sql操作，都放在UserMapper.xml中，在UserMapper.xml中加入下面配置，使用insert元素定义插入操作：
```xml
<!-- insert用来定义一个插入操作
id：操作的具体标识
parameterType：指定插入操作接受的参数类型
-->
<insert id="insertUser" parameterType="com.javacode2018.chat02.UserModel">
  <![CDATA[
  INSERT INTO t_user (id,name,age,salary,sex) VALUES (#{id},#{name},#{age},#{salary},#{sex})
  ]]>
</insert>
```
insert元素用来定义了一个对db的insert操作<br />**id**：是这个操作的一个标识，一会通过Mybatis执行操作的时候会通过这个namespace和id引用到这个insert操作，<br />**parameterType**：用来指定这个insert操作接受的参数的类型，可以是：**各种JavaBean、map、list、collection类型的java对象**，这个插入接受的是UserModel对象。<br />insert元素内部定义了具体的sql，可以看到是一个insert的sql，向t_user表插入数据。<br />需要插入的值从UserModel对象中获取，取UserModel对象的的字段，使用**#{字段}**这种格式可以获取到UserModel中字段的值。
<a name="b9tCa"></a>
#### 调用SqlSession.insert方法执行插入操作
t_user插入的sql已经在UserMapper中写好，此时怎么调用呢？<br />需要调用`SqlSession.insert`方法：
```java
int insert(String statement, Object parameter)
```
这个方法有2个参数:<br />**statement**：表示那个操作，值为Mapper xml的namespace.具体操作的id，如需要调用UserMapper.xml中的insertUser操作，这个值就是：
```
com.javacode2018.chat02.UserMapper.insertUser
```
**parameter**：insert操作的参数，和Mapper xml中的insert中的parameterType指定的类型一致。<br />返回值为插入的行数。<br />UserTest类中新增一个测试用例：
```java
@Test
public void insertUser() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(false);) {
        //创建UserModel对象
        UserModel userModel = UserModel.builder().id(2L).name("javacode2018").age(30).salary(50000D).sex(1).build();
        //执行插入操作
        int result = sqlSession.insert("com.javacode2018.chat02.UserMapper.insertUser", userModel);
        log.info("插入影响行数：{}", result);
        //提交事务
        sqlSession.commit();
    }
}
```
运行输出如下：
```
01:46.683 [main] DEBUG c.j.chat02.UserMapper.insertUser - ==>  Preparing: INSERT INTO t_user (id,name,age,salary,sex) VALUES (?,?,?,?,?) 
01:46.745 [main] DEBUG c.j.chat02.UserMapper.insertUser - ==> Parameters: 2(Long), javacode2018(String), 30(Integer), 50000.0(Double), 1(Integer)
01:46.751 [main] DEBUG c.j.chat02.UserMapper.insertUser - <==    Updates: 1
01:46.751 [main] INFO  com.javacode2018.chat02.UserTest - 影响行数：1
```
输出中打印了详细的sql语句，以及sql的参数信息，可以看到Mapper xml中的#{}被替换为了?，这个使用到了jdbc中的`PreparedStatement`来对参数设置值。<br />输出中的第二行详细列出了参数的值以及每个值的类型。<br />第三行输出了insert的结果为1，表示插入成功了1行记录。<br />去db中看一下，如下，插入成功：
```
mysql> SELECT * FROM t_user;
+----+---------------+-----+----------+-----+
| id | name          | age | salary   | sex |
+----+---------------+-----+----------+-----+
|  1 | Tom     |  30 | 50000.00 |   1 |
+----+---------------+-----+----------+-----+
1 row in set (0.00 sec)
```
上面代码中创建SqlSession，使用的是`sqlSessionFactory.openSession()`创建的，这个方法创建的SqlSession，内部事务是非自动提交的方式，所以需要手动提交：
```java
sqlSession.commit();
```
如果想自动提交事务，可以将上面的测试用例改成下面这样：
```java
@Test
public void insertUser() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        //创建UserModel对象
        UserModel userModel = UserModel.builder().id(1L).name("Tom").age(30).salary(50000D).sex(1).build();
        //执行插入操作
        int result = sqlSession.insert("com.javacode2018.chat02.UserMapper.insertUser", userModel);
        log.info("影响行数：{}", result);
    }
}
```
上面在创建SqlSession的时候调用了`sqlSessionFactory.openSession(true)`，指定事务为自动提交模式，所以最后不需要手动提交事务了。
<a name="L63nS"></a>
### 更新操作
需求：传入UserModel对象，然后通过id更新数据。
<a name="ViCVL"></a>
#### UserMapper.xml中定义Update操作
使用update定义更新操作：
```xml
<!-- update用来定义一个更新操作
id：操作的具体标识
parameterType：指定操作接受的参数类型
-->
<update id="updateUser" parameterType="com.javacode2018.chat02.UserModel">
  <![CDATA[
  UPDATE t_user SET name = #{name},age = #{age},salary = #{salary},sex = #{sex} WHERE id = #{id}
  ]]>
</update>
```
写法和insert操作的写法类似，指定id标识、parameterType指定操作的参数类型，元素体中是具体的sql语句。
<a name="Vc0n7"></a>
#### 调用`SqlSession.update`方法执行更新操作
需要调用`SqlSession.update`方法：
```java
int update(String statement, Object parameter)
```
这个方法有2个参数:<br />**statement**：表示哪个操作，值为Mapper xml的namespace.具体操作的id，如需要调用UserMapper.xml中的updateUser操作，这个值就是：
```
com.javacode2018.chat02.UserMapper.updateUser
```
**parameter**：update操作的参数，和Mapper xml中的update中的parameterType指定的类型一致。<br />返回值为update影响行数。<br />UserTest类中新增一个测试用例：
```java
@Test
public void updateUser() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        //创建UserModel对象
        UserModel userModel = UserModel.builder().id(1L).name("Tom").age(18).salary(5000D).sex(0).build();
        //执行更新操作
        int result = sqlSession.update("com.javacode2018.chat02.UserMapper.updateUser", userModel);
        log.info("影响行数：{}", result);
    }
}
```
运行输出：
```
14:09.051 [main] DEBUG c.j.chat02.UserMapper.updateUser - ==>  Preparing: UPDATE t_user SET name = ?,age = ?,salary = ?,sex = ? WHERE id = ? 
14:09.095 [main] DEBUG c.j.chat02.UserMapper.updateUser - ==> Parameters: Tom(String), 18(Integer), 5000.0(Double), 0(Integer), 1(Long)
14:09.100 [main] DEBUG c.j.chat02.UserMapper.updateUser - <==    Updates: 1
14:09.101 [main] INFO  com.javacode2018.chat02.UserTest - 影响行数：1
```
db中去看一下：
```
mysql> SELECT * FROM t_user;
+----+------------------------+-----+----------+-----+
| id | name                   | age | salary   | sex |
+----+------------------------+-----+----------+-----+
|  1 | Tom                    |  18 |  5000.00 |   0 |
|  2 | javacode2018           |  30 | 50000.00 |   1 |
+----+------------------------+-----+----------+-----+
2 rows in set (0.00 sec)
```
<a name="K3i94"></a>
### 删除操作
需求：根据用户的id删除对应的用户记录
<a name="NOCJ4"></a>
#### UserMapper.xml中定义Delete操作
使用update元素定义删除操作：
```xml
<!-- update用来定义一个删除操作
id：操作的具体标识
parameterType：指定操作接受的参数类型
-->
<update id="deleteUser" parameterType="java.lang.Long">
  <![CDATA[
  DELETE FROM t_user WHERE id = #{id}
  ]]>
</update>
```
写法和update操作的写法类似，指定id标识、parameterType指定操作的参数类型，用户id为Long类型的，元素体中是具体的delete语句。
<a name="anYDv"></a>
#### 调用SqlSession.update方法执行更新操作
需要调用SqlSession.delete方法：
```java
int delete(String statement, Object parameter)
```
这个方法有2个参数:<br />**statement**：表示哪个操作，值为Mapper xml的namespace.具体操作的id，如需要调用UserMapper.xml中的deleteUser操作，这个值就是：
```
com.javacode2018.chat02.UserMapper.
```
**parameter**：delete操作的参数，和Mapper xml中的delete中的parameterType指定的类型一致。<br />返回值为delete影响行数。<br />UserTest类中新增一个测试用例：
```java
@Test
public void deleteUser() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        //定义需要删除的用户id
        Long userId = 1L;
        //执行删除操作
        int result = sqlSession.delete("com.javacode2018.chat02.UserMapper.deleteUser", userId);
        log.info("影响行数：{}", result);
    }
}
```
运行输出：
```
24:45.427 [main] DEBUG c.j.chat02.UserMapper.deleteUser - ==>  Preparing: DELETE FROM t_user WHERE id = ? 
24:45.476 [main] DEBUG c.j.chat02.UserMapper.deleteUser - ==> Parameters: 1(Long)
24:45.485 [main] DEBUG c.j.chat02.UserMapper.deleteUser - <==    Updates: 1
24:45.485 [main] INFO  com.javacode2018.chat02.UserTest - 影响行数：1
```
<a name="SBDfC"></a>
### 执行查询
需求：查询所有用户信息
<a name="bE1al"></a>
#### UserMapper.xml中定义Select操作
```xml
<!-- select用来定义一个查询操作
id：操作的具体标识
resultType：指定查询结果保存的类型
-->
<select id="getUserList" resultType="com.javacode2018.chat02.UserModel">
  <![CDATA[
  SELECT * FROM t_user
  ]]>
</select>
```
写法和update操作的写法类似，指定id标识、parameterType指定操作的参数类型，resultType指定查询结果的类型，元素体中是具体的select语句。
<a name="yYQ3J"></a>
#### 调用SqlSession.select方法执行更新操作
UserTest添加一个用例：
```java
@Test
public void getUserList() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        //执行查询操作
        List<UserModel> userModelList = sqlSession.selectList("com.javacode2018.chat02.UserMapper.getUserList");
        log.info("结果：{}", userModelList);
    }
}
```
多插入几行，然后运行上面的用例，输出如下：
```
36:39.015 [main] DEBUG c.j.chat02.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
36:39.048 [main] DEBUG c.j.chat02.UserMapper.getUserList - ==> Parameters: 
36:39.066 [main] DEBUG c.j.chat02.UserMapper.getUserList - <==      Total: 3
36:39.067 [main] INFO  com.javacode2018.chat02.UserTest - UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1)
36:39.069 [main] INFO  com.javacode2018.chat02.UserTest - UserModel(id=1575621274235, name=Tom, age=30, salary=50000.0, sex=1)
36:39.069 [main] INFO  com.javacode2018.chat02.UserTest - UserModel(id=1575621329823, name=Tom, age=30, salary=50000.0, sex=1)
```
<a name="caqdj"></a>
## Mapper接口的使用
<a name="ji3oB"></a>
### 为什么需要Mapper接口
上面说了对一个表的增删改查操作，都是通过调用`SqlSession`中的方法来完成的，再来看一下`SqlSession`接口中刚才用到的几个方法的定义：
```java
int insert(String statement, Object parameter);
int update(String statement, Object parameter);
int delete(String statement, Object parameter);
<E> List<E> selectList(String statement);
```
这些方法的特点来看一下：

1. 调用这些方法，需要明确知道statement的值，statement的值为namespace.具体操作的id，这些需要打开Mapper xml中去查看了才知道，写起来不方便
2. parameter参数都是Object类型的，根本不知道这个操作具体类型是什么，需要查看Mapper xml才知道，随便传递个值，可能类型不匹配，但是只有在运行的时候才知道有问题
3. selectList方法返回的是一个泛型类型的，通过这个方法根本不知道返回的结果的具体类型，也需要去查看Mapper xml才知道

以上这几点使用都不是太方便，有什么方法能解决上面这些问题么？<br />有，这就是Mybatis中的Mapper接口，可以定义一个interface，然后和Mapper xml关联起来，Mapper xml中的操作和Mapper接口中的方法会进行绑定，当调用Mapper接口的方法的时候，会间接调用到Mapper xml中的操作，接口的完整类名需要和Mapper xml中的namespace一致。
<a name="qfvcz"></a>
### Mapper接口的用法（三步）
<a name="xAlzQ"></a>
#### 步骤1：定义Mapper接口
去看一下，UserMapper.xml中的namespace，是：
```xml
<mapper namespace="com.javacode2018.chat02.UserMapper">
```
创建的接口完整的名称需要和上面的namespace的值一样，下面创建一个接口com.javacode2018.chat02.UserMapper，如下：
```java
package com.javacode2018.chat02;

public interface UserMapper {
}
```
UserMapper.xml中有4个操作，需要在UserMapper接口中也定义4个操作，和UserMapper.xml的4个操作对应，如下：
```java
package com.javacode2018.chat02;

import java.util.List;

public interface UserMapper {

    int insertUser(UserModel model);

    int updateUser(UserModel model);

    int deleteUser(Long userId);

    List<UserModel> getUserList();
}
```
UserMapper接口中定义了4个方法，方法的名称需要和UserMapper.xml具体操作的id值一样，这样调用UserMapper接口中的方法的时候，才会对应的找到UserMapper.xml中具体的操作。<br />比如调用UserMapper接口中的insertUser方法，Mybatis查找的规则是：通过接口完整名称.方法名称去Mapper xml中找到对应的操作。
<a name="XvHa5"></a>
#### 步骤2：通过SqlSession获取Mapper接口对象
SqlSession中有个`getMapper`方法，可以传入接口的类型，获取具体的Mapper接口对象，如下：
```java
/**
   * Retrieves a mapper.
   * @param <T> the mapper type
   * @param type Mapper interface class
   * @return a mapper bound to this SqlSession
   */
<T> T getMapper(Class<T> type);
```
如获取UserMapper接口对象：
```java
UserMapper mapper = sqlSession.getMapper(UserMapper.class);
```
<a name="pO96j"></a>
#### 步骤3：调用Mapper接口的方法对db进行操作
如调用UserMapper接口的insert操作：
```java
@Test
public void insertUser() {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        //创建UserModel对象
        UserModel userModel = UserModel.builder().id(System.currentTimeMillis()).name("Tom").age(30).salary(50000D).sex(1).build();
        //执行插入操作
        int insert = mapper.insertUser(userModel);
        log.info("影响行数：{}", insert);
    }
}
```
<a name="RXKHn"></a>
### 案例：使用Mapper接口来实现增删改查
chat02/src/test/java中创建一个测试类，代码如下：
```java
package com.javacode2018.chat02;

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
        String resource = "mybatis-config.xml";
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
            UserModel userModel = UserModel.builder().id(System.currentTimeMillis()).name("Tom").age(30).salary(50000D).sex(1).build();
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
            UserModel userModel = UserModel.builder().id(1L).name("Tom，你好").age(18).salary(5000D).sex(0).build();
            //执行更新操作
            int result = mapper.updateUser(userModel);
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
            int result = mapper.deleteUser(userId);
            log.info("影响行数：{}", result);
        }
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
大家认真看一下上面的代码，这次使用了UserMapper来间接调用UserMapper.xml中对应的操作，可以去运行一下感受一下效果。
<a name="iMNIk"></a>
### Mapper接口使用时注意的几点

1. Mapper接口的完整类名必须和对应的Mapper xml中的namespace的值一致
2. Mapper接口中方法的名称需要和Mapper xml中具体操作的id值一致
3. Mapper接口中方法的参数、返回值可以不和Mapper xml中的一致
<a name="BUK4L"></a>
### Mapper接口的原理
这个使用java中的动态代理实现的，Mybatis启动的时候会加载全局配置文件mybatis-config.xml，然后解析这个文件中的mapper元素指定的UserMapper.xml，会根据UserMapper.xml的namespace的值创建这个接口的一个动态代理，具体可以去看一下Mybatis的源码，主要使用java中的Proxy实现的，使用java.lang.reflect.Proxy类中的newProxyInstance方法，可以创建任意一个接口的一个代理对象：
```java
public static Object newProxyInstance(ClassLoader loader,
                                      Class<?>[] interfaces,
                                      InvocationHandler h)
```
使用Proxy来模仿Mapper接口的实现：
```java
package com.javacode2018.chat02;

import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.List;

@Slf4j
public class ProxyTest {

    public static class UserMapperProxy implements InvocationHandler {
        private SqlSession sqlSession;

        private Class<?> mapperClass;

        public UserMapperProxy(SqlSession sqlSession, Class<?> mapperClass) {
            this.sqlSession = sqlSession;
            this.mapperClass = mapperClass;
        }

        @Override
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            log.debug("invoke start");
            String statement = mapperClass.getName() + "." + method.getName();
            List<Object> result = sqlSession.selectList(statement);
            log.debug("invoke end");
            return result;
        }
    }

    private SqlSessionFactory sqlSessionFactory;

    @Before
    public void before() throws IOException {
        //指定mybatis全局配置文件
        String resource = "mybatis-config.xml";
        //读取全局配置文件
        InputStream inputStream = Resources.getResourceAsStream(resource);
        //构建SqlSessionFactory对象
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        this.sqlSessionFactory = sqlSessionFactory;
    }

    @Test
    public void test1() {
        try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
            UserMapper userMapper = (UserMapper) Proxy.newProxyInstance(ProxyTest.class.getClassLoader(), new Class[]{UserMapper.class}, new UserMapperProxy(sqlSession, UserMapper.class));
            log.info("{}", userMapper.getUserList());
        }
    }
}
```
上面代码中：UserMapper是没有实现类的，可以通过`Proxy.newProxyInstance`给UserMapper接口创建一个代理对象，当调用UserMapper接口的方法的时候，会调用到`UserMapperProxy`对象的`invoke`方法。<br />运行一下test1用例，输出如下：
```
16:34.288 [main] DEBUG com.javacode2018.chat02.ProxyTest - invoke start
16:34.555 [main] DEBUG c.j.chat02.UserMapper.getUserList - ==>  Preparing: SELECT * FROM t_user 
16:34.580 [main] DEBUG c.j.chat02.UserMapper.getUserList - ==> Parameters: 
16:34.597 [main] DEBUG c.j.chat02.UserMapper.getUserList - <==      Total: 4
16:34.597 [main] DEBUG com.javacode2018.chat02.ProxyTest - invoke end
16:34.597 [main] INFO  com.javacode2018.chat02.ProxyTest - [UserModel(id=2, name=javacode2018, age=30, salary=50000.0, sex=1), UserModel(id=1575621274235, name=Tom, age=30, salary=50000.0, sex=1), UserModel(id=1575621329823, name=Tom, age=30, salary=50000.0, sex=1), UserModel(id=1575623283897, name=Tom, age=30, salary=50000.0, sex=1)]
```
注意上面输出的invoke start和invoke end，可以看到调用userMapper.getUserList时候，被UserMapperProxy#invoke方法处理了。<br />Mybatis中创建Mapper接口代理对象使用的是下面这个类，大家可以去研究一下：
```java
public class MapperProxyFactory<T> {

    private final Class<T> mapperInterface;
    private final Map<Method, MapperMethod> methodCache = new ConcurrentHashMap<Method, MapperMethod>();

    public MapperProxyFactory(Class<T> mapperInterface) {
        this.mapperInterface = mapperInterface;
    }

    public Class<T> getMapperInterface() {
        return mapperInterface;
    }

    public Map<Method, MapperMethod> getMethodCache() {
        return methodCache;
    }

    @SuppressWarnings("unchecked")
    protected T newInstance(MapperProxy<T> mapperProxy) {
        return (T) Proxy.newProxyInstance(mapperInterface.getClassLoader(), new Class[] { mapperInterface }, mapperProxy);
    }

    public T newInstance(SqlSession sqlSession) {
        final MapperProxy<T> mapperProxy = new MapperProxy<T>(sqlSession, mapperInterface, methodCache);
        return newInstance(mapperProxy);
    }

}
```
