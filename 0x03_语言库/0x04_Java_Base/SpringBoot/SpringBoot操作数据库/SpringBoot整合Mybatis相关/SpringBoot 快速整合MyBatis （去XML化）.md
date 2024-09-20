JavaSpringBootMybatis<br />主要通过XML来书写SQL和填补对象映射关系。在SpringBoot中可以通过注解来快速编写SQL并实现数据访问。（仅需配置：`mybatis.configuration.map-underscore-to-camel-case=true`）。
<a name="eR3Mp"></a>
## 具体步骤
<a name="B1IfP"></a>
### 1、引入依赖
在pom.xml 引入ORM框架（Mybaits-Starter）和数据库驱动（MySQL-Conn）的依赖。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <!--继承信息 -->
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.0.0.M4</version>
        <relativePath/>
    </parent>

    <!--依赖管理 -->
    <dependencies>
        <dependency> <!--添加Web依赖 -->
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency> <!--添加Mybatis依赖 -->
            <groupId>org.mybatis.spring.boot</groupId>
            <artifactId>mybatis-spring-boot-starter</artifactId>
            <version>1.3.1</version>
        </dependency>
        <dependency><!--添加MySQL驱动依赖 -->
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>
        <dependency><!--添加Test依赖 -->
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>
</project>
```
<a name="GCPrm"></a>
### 2、添加数据源
在application.yml 添加数据源，以及开启Mybaits的驼峰映射功能。
```yaml
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/socks?useSSL=false
    username: root
    password: root
    driver-class-name: com.mysql.jdbc.Driver

mybatis:
  configuration:
   map-underscore-to-camel-case: true #开启驼峰映射
```
<a name="uB3O2"></a>
### 3、编写数据层代码
```java
// POJO类如下：
public class User {
    private String userId;
    private String username;
    private String password;
    // Getters & Setters ..
}
// 数据层代码如下：
public interface UserMapper {

    @Select("select * from t_user where 1=1")
    List<User> list();

    @Select("select * from t_user where username like #{username}")
    List<User> findByUsername(String username);

    @Select("select * from t_user where user_id like #{userId}")
    User getOne(String userId);

    @Delete("delete from t_user where user_id like #{userId}")
    int delete(String userId);
}
```
<a name="ijcJj"></a>
### 4、添加数据库记录
在Navicat 连接本地数据库，随便打开查询窗口，复制下面这段脚本，点击执行即可。
```sql
DROP DATABASE IF EXISTS `socks`;
CREATE DATABASE `socks`;
USE `SOCKS`;
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
  `USER_ID` varchar(50) ,
  `USERNAME` varchar(50) ,
  `PASSWORD` varchar(50) 
) ;

INSERT INTO `t_user` VALUES ('1', 'admin', 'admin');
INSERT INTO `t_user` VALUES ('2', 'yizhiwazi', '123456');
```
<a name="sw1RO"></a>
### 5、启动项目
```java
@SpringBootApplication
@MapperScan("com.hehe.mapper")  //扫描Mapper接口
public class MybatisApplication {

    public static void main(String[] args) {
        SpringApplication.run(MybatisApplication.class, args);
    }
}
```
<a name="m24Q9"></a>
### 6、单元测试
```java
import static org.assertj.core.api.Assertions.assertThat;

@RunWith(SpringRunner.class)
@SpringBootTest
public class MybatisApplicationTest {

    @SuppressWarnings("all")
    @Autowired
    UserMapper userMapper;

    @Test
    public void test_db() {
        //开始进行测试
        assertThat(userMapper.list().size()).isGreaterThan(1);
        assertThat(userMapper.getOne("1")).isNotEqualTo(null);
        assertThat(userMapper.getOne("xxx")).isEqualTo(null);
    }
}
```
