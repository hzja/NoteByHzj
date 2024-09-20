JavaSpringBootMybatis<br />序言：使用MyBatis3提供的注解可以逐步取代XML，例如使用`@Select`注解直接编写SQL完成数据查询，使用`@SelectProvider`高级注解还可以编写动态SQL，以应对复杂的业务需求。
<a name="bBh8N"></a>
## 基础注解
MyBatis 主要提供了以下CRUD注解：

- `@Select`
- `@Insert`
- `@Update`
- `@Delete`

增删改查占据了绝大部分的业务操作，掌握这些基础注解的使用还是很有必要的，例如下面这段代码无需XML即可完成数据查询：
```java
@Mapper
public interface UserMapper {
    @Select("select * from t_user")
    List<User> list();
}
```
使用过Hibernate的同学可能会好奇，这里为什么没有配置映射关系也能完成属性注入？在传统项目中使用过Mybatis的童鞋可能很快就反应过来，是因为在配置文件中开启了全局驼峰映射，SpringBoot中同样能够做到，并且更为简单快捷。<br />虽然开启了全局驼峰映射，但你可能还会质疑，如果不符合下划线转驼峰规则的字段，拿查询回来的实体对象属性将获取为null，比如上述User对象属性mobileNum和对应的数据库字段phoneNum，则查询结果为：
```json
[
  {
    "userId": "1",
    "username": "admin",
    "password": "admin",
    "mobileNum": null
  },
  {
    "userId": "2",
    "username": "roots",
    "password": "roots",
    "mobileNum": null
  }
]
```
为了解决对象属性和字段驼峰不一致的问题，可以使用映射注解`@Results`来指定映射关系。
<a name="JOqMJ"></a>
## 映射注解
Mybatis主要提供这些映射注解：

- `@Results`  用于填写结果集的多个字段的映射关系
- `@Result`  用于填写结果集的单个字段的映射关系
- `@ResultMap` 根据ID关联XML里面`<resultMap>`

例如上面的list方法，可以在查询SQL的基础上，指定返回的结果集的映射关系，其中property表示实体对象的属性名，column表示对应的数据库字段名。
```java
@Results({
        @Result(property = "userId", column = "USER_ID"),
        @Result(property = "username", column = "USERNAME"),
        @Result(property = "password", column = "PASSWORD"),
        @Result(property = "mobileNum", column = "PHONE_NUM")
})
@Select("select * from t_user")
List<User> list();
```
查询结果如下：
```json
[
  {
    "userId": "1",
    "username": "admin",
    "password": "admin",
    "mobileNum": "15011791234"
  },
  {
    "userId": "2",
    "username": "roots",
    "password": "roots",
    "mobileNum": "18812342017"
  }
]
```
为了方便演示和免除手工编写映射关系的烦恼，这里提供了一个快速生成映射结果集的方法，具体内容如下：
```java
/**
 * 1.用于获取结果集的映射关系
 */
public static String getResultsStr(Class origin) {
    StringBuilder stringBuilder = new StringBuilder();
    stringBuilder.append("@Results({\n");
    for (Field field : origin.getDeclaredFields()) {
        String property = field.getName();
        //映射关系：对象属性(驼峰)->数据库字段(下划线)
        String column = new PropertyNamingStrategy.SnakeCaseStrategy().translate(field.getName()).toUpperCase();
        stringBuilder.append(String.format("@Result(property = \"%s\", column = \"%s\"),\n", property, column));
    }
    stringBuilder.append("})");
    return stringBuilder.toString();
}
```
在当前Main方法执行效果如下：然后将控制台这段打印信息复制到接口方法上即可。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669536967327-f4af61a3-164b-412a-a5a2-e33fb3a2dff6.jpeg#averageHue=%233a3d3a&clientId=uf79f4911-3bd0-4&from=paste&id=uc81cb584&originHeight=191&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u912d23f5-6cc6-45ff-b3cf-f22dd4e1b46&title=)
<a name="cHJa3"></a>
## 高级注解
MyBatis-3 主要提供了以下CRUD的高级注解：

- `@SelectProvider`
- `@InsertProvider`
- `@UpdateProvider`
- `@DeleteProvider`

见名知意，这些高级注解主要用于动态SQL，这里以`@SelectProvider` 为例，主要包含两个注解属性，其中type表示工具类，method 表示工具类的某个方法，用于返回具体的SQL。
```java
@Mapper
public interface UserMapper {
    @SelectProvider(type = UserSqlProvider.class, method = "list222")
    List<User> list2();
}
```
工具类代码如下：
```java
public class UserSqlProvider {
public String list222() {
    return "select * from t_user ;
}
```
<a name="ctptg"></a>
## 详细教程
对上述注解有所了解之后，以具体项目案例来进一步巩固这些注解的实际使用。
<a name="GWQVN"></a>
### 1、引入依赖
为了方便演示，首选搭建Web环境，另外数据库选择Mysql 5.5+。
```xml
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
```
<a name="QAmkl"></a>
### 2、添加配置
这里主要是添加数据源，配置驼峰映射和开启SQL日志的控制台打印。在项目的资源目录中，添加 application.yml 配置如下：
```yaml
spring:
  datasource:
    #连接MySQL
    url: jdbc:mysql://localhost:3306/socks?useSSL=false
    username: root
    password: root
    driver-class-name: com.mysql.jdbc.Driver

mybatis:
  configuration:
   #配置项：开启下划线到驼峰的自动转换. 作用：将数据库字段根据驼峰规则自动注入到对象属性。
   map-underscore-to-camel-case: true

logging:
  level:
   #打印SQL信息
   com.hehe.mapper: debug
```
<a name="fzKyI"></a>
### 3、编写数据层代码
这里以熟悉的用户信息为例，编写`UserMapper`接口和本案例使用的`UserSqlProvider`。
<a name="LoJKa"></a>
#### 3.1 `UserMapper`
添加`UserMapper`接口用于数据查询：
```java

package com.hehe.mapper;
@Mapper
public interface UserMapper {
    /**
     * 方式1：使用注解编写SQL。
     */
    @Select("select * from t_user")
    List<User> list();

    /**
     * 方式2：使用注解指定某个工具类的方法来动态编写SQL.
     */
    @SelectProvider(type = UserSqlProvider.class, method = "listByUsername")
    List<User> listByUsername(String username);

    /**
     * 延伸：上述两种方式都可以附加@Results注解来指定结果集的映射关系.
     *
     * PS：如果符合下划线转驼峰的匹配项可以直接省略不写。
     */
    @Results({
            @Result(property = "userId", column = "USER_ID"),
            @Result(property = "username", column = "USERNAME"),
            @Result(property = "password", column = "PASSWORD"),
            @Result(property = "mobileNum", column = "PHONE_NUM")
    })
    @Select("select * from t_user")
    List<User> listSample();

    /**
     * 延伸：无论什么方式,如果涉及多个参数,则必须加上@Param注解,否则无法使用EL表达式获取参数。
     */
    @Select("select * from t_user where username like #{username} and password like #{password}")
    User get(@Param("username") String username, @Param("password") String password);

    @SelectProvider(type = UserSqlProvider.class, method = "getBadUser")
    User getBadUser(@Param("username") String username, @Param("password") String password);

}
```
<a name="iR3Ks"></a>
#### 3.2 `UserSqlProvider`
添加`UserSqlProvider`，用于生成SQL的工具类 。
```java

package com.hehe.mapper;

/**
 * 主要用途：根据复杂的业务需求来动态生成SQL.
 * <p>
 * 目标：使用Java工具类来替代传统的XML文件.(例如：UserSqlProvider.java <-- UserMapper.xml)
 */
public class UserSqlProvider {
    /**
     * 方式1：在工具类的方法里,可以自己手工编写SQL。
     */
    public String listByUsername(String username) {
        return "select * from t_user where username =#{username}";
    }

    /**
     * 方式2：也可以根据官方提供的API来编写动态SQL。
     */
    public String getBadUser(@Param("username") String username, @Param("password") String password) {
        return new SQL() {{
            SELECT("*");
            FROM("t_user");
            if (username != null && password != null) {
                WHERE("username like #{username} and password like #{password}");
            } else {
                WHERE("1=2");
            }
        }}.toString();
    }
}
```
<a name="J4mbv"></a>
#### 3.3 实体类User
添加实体类User
```java
public class User {
    private String userId;
    private String username;
    private String password;
    private String mobileNum;
    //Getters & Setters
}
```
<a name="nvHtA"></a>
#### 3.4 添加数据库记录
打开Navicat 查询窗口，然后只需下面这段脚本。
```sql
USE `SOCKS`;
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
  `USER_ID` varchar(50) ,
  `USERNAME` varchar(50) ,
  `PASSWORD` varchar(50) ,
  `PHONE_NUM` varchar(15) 
) ;

INSERT INTO `t_user` VALUES ('1', 'admin', 'admin','15011791234');
INSERT INTO `t_user` VALUES ('2', 'roots', 'roots','18812342017');
```
<a name="SDhnv"></a>
### 4、编写控制层代码
```java
package com.hehe.controller;

@RestController
@RequestMapping("/user/*")
public class UserController {

    @SuppressWarnings("all")
    @Autowired
    UserMapper userMapper;

    @GetMapping("list")
    public List<User> list() {
        return userMapper.list();
    }

    @GetMapping("list/{username}")
    public List<User> listByUsername(@PathVariable("username") String username) {
        return userMapper.listByUsername(username);
    }

    @GetMapping("get/{username}/{password}")
    public User get(@PathVariable("username") String username, @PathVariable("password") String password) {
        return userMapper.get(username, password);
    }

    @GetMapping("get/bad/{username}/{password}")
    public User getBadUser(@PathVariable("username") String username, @PathVariable("password") String password) {
        return userMapper.getBadUser(username, password);
    }

}
```
<a name="XSDaf"></a>
### 5、启动和测试
启动工程后，访问 http://localhost:8080/user/list 可以查看用户列表如下：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669536967309-08b091bd-9147-4ac5-b02a-28bfd340cd77.jpeg#averageHue=%23fbfbfa&clientId=uf79f4911-3bd0-4&from=paste&id=u7afa66b5&originHeight=342&originWidth=291&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u272d0f20-5ca1-4d60-a859-9064e865a87&title=)<br />访问 http://localhost:8080/user/list/admin 可以查询用户名为admin的信息：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669536967392-8f491878-3c89-4ef5-8e03-aedcc6f69489.jpeg#averageHue=%23fbfbfb&clientId=uf79f4911-3bd0-4&from=paste&id=ud22c9f6f&originHeight=203&originWidth=255&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9b1835e5-6afd-4581-a9a2-56b4567cf2e&title=)
