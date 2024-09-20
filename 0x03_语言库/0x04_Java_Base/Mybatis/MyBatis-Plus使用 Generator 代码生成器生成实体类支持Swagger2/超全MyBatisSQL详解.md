JavaMybatis-plus<br />**MyBatis-Plus（简称 MP），是一个 MyBatis 的增强工具，在 MyBatis 的基础上只做增强不做改变，为简化开发、提高效率而生**。<br />首先说一下**MyBatis-Plus**的愿景是什么？<br />**MyBatis-Plus**的愿景是成为 MyBatis 最好的搭档，就像 魂斗罗 中的 1P、2P，基友搭配，效率翻倍。
<a name="XDjfc"></a>
### **一、特性**

- **无侵入：只做增强不做改变，引入它不会对现有工程产生影响，如丝般顺滑**
- **损耗小：启动即会自动注入基本 CURD，性能基本无损耗，直接面向对象操作**
- **强大的 CRUD 操作：内置通用 Mapper、通用 Service，仅仅通过少量配置即可实现单表大部分 CRUD 操作，更有强大的条件构造器，满足各类使用需求**
- **支持 Lambda 形式调用：通过 Lambda 表达式，方便地编写各类查询条件，无需再担心字段写错**
- **支持主键自动生成：支持多达 4 种主键策略（内含分布式唯一 ID 生成器 - Sequence），可自由配置，完美解决主键问题**
- **支持 ActiveRecord 模式：支持 ActiveRecord 形式调用，实体类只需继承 Model 类即可进行强大的 CRUD 操作**
- **支持自定义全局通用操作：支持全局通用方法注入（ Write once, use anywhere ）**
- **内置代码生成器：采用代码或者 Maven 插件可快速生成 Mapper 、 Model 、 Service 、 Controller 层代码，支持模板引擎，更有超多自定义配置等您来使用**
- **内置分页插件：基于 MyBatis 物理分页，开发者无需关心具体操作，配置好插件之后，写分页等同于普通 List 查询**
- **分页插件支持多种数据库：支持 MySQL、MariaDB、Oracle、DB2、H2、HSQL、SQLite、Postgre、SQLServer 等多种数据库**
- **内置性能分析插件：可输出 SQL 语句以及其执行时间，建议开发测试时启用该功能，能快速揪出慢查询**
- **内置全局拦截插件：提供全表 delete 、 update 操作智能分析阻断，也可自定义拦截规则，预防误操作**
<a name="JC8m9"></a>
### **二、支持的数据库**
任何能使用 MyBatis 进行 CRUD, 并且支持标准 SQL 的数据库，具体支持情况如下。<br />MySQL，Oracle，DB2，H2，HSQL，SQLite，PostgreSQL，SQLServer，Phoenix，Gauss ，ClickHouse，Sybase，OceanBase，Firebird，Cubrid，Goldilocks，csiidb<br />达梦数据库，虚谷数据库，人大金仓数据库，南大通用(华库)数据库，南大通用数据库，神通数据库，瀚高数据库
<a name="a025d"></a>
### **三、框架结构**
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657847850348-b82a890a-ba10-4e11-89f7-d70a2a792dde.jpeg#clientId=uccfeec3b-e835-4&from=paste&id=ua2a37d22&originHeight=692&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc4af9665-d976-4a51-8f91-4289ad08a18&title=)
<a name="YksLq"></a>
### **四、快速开始**
将通过一个简单的 Demo 来阐述 MyBatis-Plus 的强大功能，在此之前，假设已经：

- **拥有 Java 开发环境以及相应 IDE**
- **熟悉 Spring Boot**
- **熟悉 Maven**

---

现有一张 User 表，其表结构如下：

| **id** | **name** | **age** | **email** |
| --- | --- | --- | --- |
| 1 | Jone | 18 | test1@baomidou.com |
| 2 | Jack | 20 | test2@baomidou.com |
| 3 | Tom | 28 | test3@baomidou.com |
| 4 | Sandy | 21 | test4@baomidou.com |
| 5 | Billie | 24 | test5@baomidou.com |

其对应的数据库 Schema 脚本如下：
```sql
DROP TABLE IF EXISTS user;

CREATE TABLE user
(
    id BIGINT(20) NOT NULL COMMENT '主键ID',
    name VARCHAR(30) NULL DEFAULT NULL COMMENT '姓名',
    age INT(11) NULL DEFAULT NULL COMMENT '年龄',
    email VARCHAR(50) NULL DEFAULT NULL COMMENT '邮箱',
    PRIMARY KEY (id)
);
```
其对应的数据库 Data 脚本如下：
```sql
DELETE FROM user;

INSERT INTO user (id, name, age, email) VALUES
(1, 'Jone', 18, 'test1@baomidou.com'),
(2, 'Jack', 20, 'test2@baomidou.com'),
(3, 'Tom', 28, 'test3@baomidou.com'),
(4, 'Sandy', 21, 'test4@baomidou.com'),
(5, 'Billie', 24, 'test5@baomidou.com');
```

---

Question<br />如果从零开始用 MyBatis-Plus 来实现该表的增删改查需要做什么呢？
<a name="LEZ8c"></a>
#### 1、初始化工程
创建一个空的 Spring Boot 工程（工程将以 H2 作为默认数据库进行演示）<br />可以使用 [Spring Initializer]([https://start.spring.io/](https://start.spring.io/))快速初始化一个 Spring Boot 工程
<a name="sv9cZ"></a>
#### 2、添加依赖
引入 Spring Boot Starter 父工程：
```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>latest-springboot-version</version>
    <relativePath/>
</parent>
```
引入 spring-boot-starter、spring-boot-starter-test、mybatis-plus-boot-starter、h2 依赖：
```xml
<dependencies>
	<dependency>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter</artifactId>
	</dependency>
	<dependency>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-test</artifactId>
		<scope>test</scope>
	</dependency>
	<dependency>
		<groupId>com.baomidou</groupId>
		<artifactId>mybatis-plus-boot-starter</artifactId>
		<version>latest-version</version>
	</dependency>
	<dependency>
		<groupId>com.h2database</groupId>
		<artifactId>h2</artifactId>
		<scope>runtime</scope>
	</dependency>
</dependencies>
```
<a name="eoe24"></a>
#### 3、配置
在 application.yml 配置文件中添加 H2 数据库的相关配置：
```yaml
# DataSource Config
spring:
  datasource:
    driver-class-name: org.h2.Driver
    schema: classpath:db/schema-h2.sql
    data: classpath:db/data-h2.sql
    url: jdbc:h2:mem:test
    username: root
    password: test
```
在 Spring Boot 启动类中添加 `@MapperScan` 注解，扫描 Mapper 文件夹：
```java
@SpringBootApplication
@MapperScan("com.baomidou.mybatisplus.samples.quickstart.mapper")
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }

}
```
<a name="zu1zj"></a>
#### 4、编码
编写实体类 User.java（此处使用了 Lombok (opens new window)简化代码）
```java
@Data
public class User {
    private Long id;
    private String name;
    private Integer age;
    private String email;
}
```
编写 Mapper 包下的 `UserMapper`接口
```java
public interface UserMapper extends BaseMapper<User> {

}
```
<a name="bSKqZ"></a>
#### 5、开始使用
添加测试类，进行功能测试：
```java
@SpringBootTest
public class SampleTest {
	
	@Autowired
	private UserMapper userMapper;
	
	@Test
	public void testSelect() {
		System.out.println(("----- selectAll method test ------"));
		List<User> userList = userMapper.selectList(null);
		Assert.assertEquals(5, userList.size());
		userList.forEach(System.out::println);
	}
	
}
```
UserMapper 中的 `selectList()` 方法的参数为 MP 内置的条件封装器 `Wrapper`，所以不填写就是无任何条件<br />控制台输出：
```
User(id=1, name=Jone, age=18, email=test1@baomidou.com)
User(id=2, name=Jack, age=20, email=test2@baomidou.com)
User(id=3, name=Tom, age=28, email=test3@baomidou.com)
User(id=4, name=Sandy, age=21, email=test4@baomidou.com)
User(id=5, name=Billie, age=24, email=test5@baomidou.com)
```
完整的代码示例请移步：[Spring Boot 快速启动示例 ]([https://github.com/baomidou/mybatis-plus-samples/tree/master/mybatis-plus-sample-quickstart](https://github.com/baomidou/mybatis-plus-samples/tree/master/mybatis-plus-sample-quickstart))|<br /> [Spring MVC 快速启动示例(opens new window)]([https://github.com/baomidou/mybatis-plus-samples/tree/master/mybatis-plus-sample-quickstart-springmvc](https://github.com/baomidou/mybatis-plus-samples/tree/master/mybatis-plus-sample-quickstart-springmvc))
<a name="I1Yi0"></a>
### **五、总结**
通过以上几个简单的步骤，就实现了 User 表的 CRUD 功能，甚至连 XML 文件都不用编写！<br />从以上步骤中，可以看到集成MyBatis-Plus非常的简单，只需要引入 starter 工程，并配置 mapper 扫描路径即可。<br />但 MyBatis-Plus 的强大远不止这些功能。
