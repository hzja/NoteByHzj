Mybatis
<a name="D5AqO"></a>
## 前言
这里列举了MyBatis和MyBatis-Plus常用的五种批量插入的方式，进行了详细的总结归纳。

- 准备工作
- MyBatis利用For循环批量插入
- MyBatis的手动批量提交
- MyBatis以集合方式批量新增（推荐）
- MyBatis-Plus提供的`SaveBatch`方法
- MyBatis-Plus提供的`InsertBatchSomeColumn`方法（推荐）
- 总结
<a name="fbinc"></a>
## 一、准备工作
<a name="EMMIe"></a>
### 1、导入pom.xml依赖
```xml
<dependency>
  <groupId>mysql</groupId>
  <artifactId>mysql-connector-java</artifactId>
  <scope>runtime</scope>
</dependency>

<!--Mybatis依赖-->
<dependency>
  <groupId>org.mybatis.spring.boot</groupId>
  <artifactId>mybatis-spring-boot-starter</artifactId>
  <version>2.2.2</version>
</dependency>

<!--Mybatis-Plus依赖-->
<dependency>
  <groupId>com.baomidou</groupId>
  <artifactId>mybatis-plus-boot-starter</artifactId>
  <version>3.5.2</version>
</dependency>

<dependency>
  <groupId>org.projectlombok</groupId>
  <artifactId>lombok</artifactId>
  <optional>true</optional>
</dependency>
```
<a name="EfIGK"></a>
### 2、配置yml文件
```yaml
server:
  port: 8080
 
spring:
  datasource:
    username: mysql用户名
    password: mysql密码
    url: jdbc:mysql://localhost:3306/数据库名字?useUnicode=true&characterEncoding=utf-8&useSSL=false&serverTimezone=UTC
    driver-class-name: com.mysql.cj.jdbc.Driver
 
mybatis:
  mapper-locations: classpath:mapping/*.xml
```
<a name="yWs2g"></a>
### 3、公用的User类
```java
@Data
public class User {
 
    private int id;
    private String username;
    private String password;
}
```
<a name="pAMlA"></a>
## 二、MyBatis利用For循环批量插入
<a name="cbSDM"></a>
### 1、编写UserService服务类，测试一万条数据耗时情况
```java
@Service
public class UserService {
 
    @Resource
    private UserMapper userMapper;
 
    public void InsertUsers(){
        long start = System.currentTimeMillis();
        for(int i = 0 ;i < 10000; i++) {
            User user = new User();
            user.setUsername("name" + i);
            user.setPassword("password" + i);
            userMapper.insertUsers(user);
        }
        long end = System.currentTimeMillis();
        System.out.println("一万条数据总耗时：" + (end-start) + "ms" );
    }
 
}
```
<a name="ubmLh"></a>
### 2、编写UserMapper接口
```java
@Mapper
public interface UserMapper {
 
    Integer insertUsers(User user);
}
```
<a name="XluJq"></a>
### 3、编写UserMapper.xml文件
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.ithuang.demo.mapper.UserMapper">
    <insert id="insertUsers">
        INSERT INTO user (username, password)
        VALUES(#{username}, #{password})
    </insert>
</mapper>
```
<a name="L25HY"></a>
### 4、进行单元测试
```java
@SpringBootTest
class DemoApplicationTests {
 
    @Resource
    private UserService userService;
 
    @Test
    public void insert(){
        userService.InsertUsers();
    }
 
}
```
<a name="XJ1PK"></a>
### 5、结果输出
一万条数据总耗时：26348ms
<a name="eWKeo"></a>
## 三、MyBatis的手动批量提交
<a name="dQ6NE"></a>
### 1、其他保持不变，Service层作稍微的变化
```java
@Service
public class UserService {
 
    @Resource
    private UserMapper userMapper;
 
    @Resource
    private SqlSessionTemplate sqlSessionTemplate;
 
    public void InsertUsers(){
        //关闭自动提交
        SqlSession sqlSession = sqlSessionTemplate.getSqlSessionFactory().openSession(ExecutorType.BATCH, false);
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        long start = System.currentTimeMillis();
        for(int i = 0 ;i < 10000; i++) {
            User user = new User();
            user.setUsername("name" + i);
            user.setPassword("password" + i);
            userMapper.insertUsers(user);
        }
        sqlSession.commit();
        long end = System.currentTimeMillis();
        System.out.println("一万条数据总耗时：" + (end-start) + "ms" );
    }
 
}
```
<a name="KHFtV"></a>
### 2、结果输出
一万条数据总耗时：24516ms
<a name="jniOm"></a>
## 四、MyBatis以集合方式批量新增（推荐）
<a name="FJnpT"></a>
### 1、编写UserService服务类
```java
@Service
public class UserService {
 
    @Resource
    private UserMapper userMapper;
 
    public void InsertUsers(){
        long start = System.currentTimeMillis();
        List<User> userList = new ArrayList<>();
        User user;
        for(int i = 0 ;i < 10000; i++) {
            user = new User();
            user.setUsername("name" + i);
            user.setPassword("password" + i);
            userList.add(user);
        }
        userMapper.insertUsers(userList);
        long end = System.currentTimeMillis();
        System.out.println("一万条数据总耗时：" + (end-start) + "ms" );
    }
 
}
```
<a name="BQfMN"></a>
### 2、编写UserMapper接口
```java
@Mapper
public interface UserMapper {
 
    Integer insertUsers(List<User> userList);
}
```
<a name="UWyVG"></a>
### 3、编写UserMapper.xml文件
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.ithuang.demo.mapper.UserMapper">
    <insert id="insertUsers">
        INSERT INTO user (username, password)
        VALUES
        <foreach collection ="userList" item="user" separator =",">
            (#{user.username}, #{user.password})
        </foreach>
    </insert>
</mapper>
```
<a name="ZBzQI"></a>
### 4、输出结果
一万条数据总耗时：521ms
<a name="WzeGK"></a>
## 五、MyBatis-Plus提供的`SaveBatch`方法
<a name="nw9eT"></a>
### 1、编写UserService服务
```java
@Service
public class UserService extends ServiceImpl<UserMapper, User> implements IService<User> {
 
    public void InsertUsers(){
        long start = System.currentTimeMillis();
        List<User> userList = new ArrayList<>();
        User user;
        for(int i = 0 ;i < 10000; i++) {
            user = new User();
            user.setUsername("name" + i);
            user.setPassword("password" + i);
            userList.add(user);
        }
        saveBatch(userList);
        long end = System.currentTimeMillis();
        System.out.println("一万条数据总耗时：" + (end-start) + "ms" );
    }
}
```
<a name="VTq3f"></a>
### 2、编写UserMapper接口
```java
@Mapper
public interface UserMapper extends BaseMapper<User> {
 
}
```
<a name="TpmRC"></a>
### 3、单元测试结果
一万条数据总耗时：24674ms
<a name="PCohS"></a>
## 六、MyBatis-Plus提供的`InsertBatchSomeColumn`方法（推荐）
<a name="xYm8I"></a>
### 1、编写`EasySqlInjector` 自定义类
```
public class EasySqlInjector extends DefaultSqlInjector {
 
 
    @Override
    public List<AbstractMethod> getMethodList(Class<?> mapperClass, TableInfo tableInfo) {
        // 注意：此SQL注入器继承了DefaultSqlInjector(默认注入器)，调用了DefaultSqlInjector的getMethodList方法，保留了mybatis-plus的自带方法
        List<AbstractMethod> methodList = super.getMethodList(mapperClass, tableInfo);
        methodList.add(new InsertBatchSomeColumn(i -> i.getFieldFill() != FieldFill.UPDATE));
        return methodList;
    }
 
}
```
<a name="s5frO"></a>
### 2、定义核心配置类注入此Bean
```java
@Configuration
public class MybatisPlusConfig {
 
    @Bean
    public EasySqlInjector sqlInjector() {
        return new EasySqlInjector();
    }
}
```
<a name="yy0qv"></a>
### 3、编写UserService服务类
```java
public class UserService{

    @Resource
    private UserMapper userMapper;
    public void InsertUsers(){
        long start = System.currentTimeMillis();
        List<User> userList = new ArrayList<>();
        User user;
        for(int i = 0 ;i < 10000; i++) {
            user = new User();
            user.setUsername("name" + i);
            user.setPassword("password" + i);
            userList.add(user);
        }
        userMapper.insertBatchSomeColumn(userList);
        long end = System.currentTimeMillis();
        System.out.println("一万条数据总耗时：" + (end-start) + "ms" );
    }
}
```
<a name="Cl9TF"></a>
### 4、编写EasyBaseMapper接口
```java
public interface EasyBaseMapper<T> extends BaseMapper<T> {
    /**
     * 批量插入 仅适用于mysql
     *
     * @param entityList 实体列表
     * @return 影响行数
     */
    Integer insertBatchSomeColumn(Collection<T> entityList);
}
```
<a name="xJNBQ"></a>
### 5、编写UserMapper接口
```java
@Mapper
public interface UserMapper<T> extends EasyBaseMapper<User> {
    
}
```
<a name="io3Kq"></a>
### 6、单元测试结果
一万条数据总耗时：575ms
<a name="NfeHk"></a>
## 七、总结
以上就是对目前MyBatis常用的批量插入方法的总结
