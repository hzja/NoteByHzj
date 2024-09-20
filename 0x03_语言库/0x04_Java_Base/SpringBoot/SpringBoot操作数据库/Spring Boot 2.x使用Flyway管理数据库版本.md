Java SpringBoot
<a name="CJyzE"></a>
## Flyway简介
Flyway是一个简单开源数据库版本控制器（约定大于配置），主要提供migrate、clean、info、validate、baseline、repair等命令。它支持SQL（PL/SQL、T-SQL）方式和Java方式，支持命令行客户端等，还提供一系列的插件支持（Maven、Gradle、SBT、ANT等）。<br />官方网站：https://flywaydb.org/<br />本文对于Flyway的自身功能不做过多的介绍，读者可以通过阅读官方文档或利用搜索引擎获得更多资料。下面具体说说在Spring Boot应用中的应用，如何使用Flyway来创建数据库以及结构不一致的检查。
<a name="F3mN6"></a>
## Flyway的使用
下面先预设一个开发目标：

1. 假设需要开发一个用户管理系统，那么必然要设计一张用户表，并实现对用户表的增删改查操作。
2. 在任务1的功能完成之后，又有一个新需求，需要对用户表增加了一个字段，看看如何实现对数据库表结构的更改。
<a name="xnJnk"></a>
### 目标 1 的实现
<a name="AU4dy"></a>
#### 第一步：创建一个基础的Spring Boot项目
在pom.xml中加入Flyway、MySQL连接和数据访问相关的必要依赖（这里选用spring-boot-starter-jdbc作为例子）
```xml
<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-jdbc</artifactId>
    </dependency>

    <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
    </dependency>

    <dependency>
        <groupId>org.flywaydb</groupId>
        <artifactId>flyway-core</artifactId>
    </dependency>

    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <scope>provided</scope>
    </dependency>

    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-test</artifactId>
        <scope>test</scope>
    </dependency>
</dependencies>
```
<a name="OG5FV"></a>
#### 第二步：按Flyway的规范创建版本化的SQL脚本。

- 在工程的src/main/resources目录下创建db目录，在db目录下再创建migration目录
- 在migration目录下创建版本化的SQL脚本V1__Base_version.sql
```sql
DROP TABLE IF EXISTS user ;
CREATE TABLE `user` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `name` varchar(20) NOT NULL COMMENT '姓名',
  `age` int(5) DEFAULT NULL COMMENT '年龄',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```
:::tips
注意：如果不想将SQL脚本放到其他目录，可以用`spring.flyway.locations`参数来配置。这里不同于1.x版本的配置项`flyway.locations`
:::
<a name="q2eKF"></a>
#### 第三步：根据User表的结构，编写对应的实体定义
```java
@Data
@NoArgsConstructor
public class User {

    private Long id;
    private String name;
    private Integer age;

}
```
<a name="y4FN4"></a>
#### 第四步：编写用户操作接口和实现
```java
public interface UserService {

    /**
     * 新增一个用户
     *
     * @param name
     * @param age
     */
    int create(String name, Integer age);

    /**
     * 根据name查询用户
     *
     * @param name
     * @return
     */
    List<User> getByName(String name);

    /**
     * 根据name删除用户
     *
     * @param name
     */
    int deleteByName(String name);

    /**
     * 获取用户总量
     */
    int getAllUsers();

    /**
     * 删除所有用户
     */
    int deleteAllUsers();

}
```
```java
@Service
public class UserServiceImpl implements UserService {

    private JdbcTemplate jdbcTemplate;

    UserServiceImpl(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }

    @Override
    public int create(String name, Integer age) {
        return jdbcTemplate.update("insert into USER(NAME, AGE) values(?, ?)", name, age);
    }

    @Override
    public List<User> getByName(String name) {
        List<User> users = jdbcTemplate.query("select * from USER where NAME = ?", (resultSet, i) -> {
            User user = new User();
            user.setId(resultSet.getLong("ID"));
            user.setName(resultSet.getString("NAME"));
            user.setAge(resultSet.getInt("AGE"));
            return user;
        }, name);
        return users;
    }

    @Override
    public int deleteByName(String name) {
        return jdbcTemplate.update("delete from USER where NAME = ?", name);
    }

    @Override
    public int getAllUsers() {
        return jdbcTemplate.queryForObject("select count(1) from USER", Integer.class);
    }

    @Override
    public int deleteAllUsers() {
        return jdbcTemplate.update("delete from USER");
    }

}
```
这里主要介绍Flyway的应用，所以采用这种比较简单的编写方式，实际项目应用中，还是推荐MyBatis的具体操作实现。
<a name="qOcsK"></a>
#### 第五步：编写测试用例
```java
@Slf4j
@SpringBootTest
public class Chapter311ApplicationTests {

    @Autowired
    private UserService userSerivce;

    @Test
    public void test() throws Exception {
        userSerivce.deleteAllUsers();

        // 插入5个用户
        userSerivce.create("Tom", 10);
        userSerivce.create("Mike", 11);
        userSerivce.create("Didispace", 30);
        userSerivce.create("Oscar", 21);
        userSerivce.create("Linda", 17);

        // 查询名为Oscar的用户，判断年龄是否匹配
        List<User> userList = userSerivce.getByName("Oscar");
        Assertions.assertEquals(21, userList.get(0).getAge().intValue());

        // 查数据库，应该有5个用户
        Assertions.assertEquals(5, userSerivce.getAllUsers());

        // 删除两个用户
        userSerivce.deleteByName("Tom");
        userSerivce.deleteByName("Mike");

        // 查数据库，应该有5个用户
        Assertions.assertEquals(3, userSerivce.getAllUsers());
    }

}
```
注意由于Spring Boot 2.4应用的junit版本与之前Spring Boot 1.x版本中的不同，因此单元测试的编写略有区别。<br />第六步：运行上面编写的单元测试，验证一下效果。<br />不出意外，单元测试运行ok的话<br />![2021-05-24-17-35-53-222343.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621849911744-721989da-d2da-4538-8bbc-b079b153a5b9.png#clientId=u69fcfcee-c926-4&from=ui&id=u3b21f8d4&originHeight=287&originWidth=800&originalType=binary&size=54928&status=done&style=none&taskId=u393b3ee3-553b-460c-bf4c-2ef5872ce91)<br />连上数据库看看。此时应该多出了这两张表：<br />![2021-05-24-17-35-53-362929.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621849919271-78bc924c-8cb6-4737-89ad-edee645021c6.png#clientId=u69fcfcee-c926-4&from=ui&id=ub309f532&originHeight=948&originWidth=682&originalType=binary&size=103463&status=done&style=none&taskId=u5daa0a99-518a-499b-ab30-f17d6284456)

- user表就是维护在SQL脚本中要创建的表
- `flyway_schema_history`表是flyway的管理表，用来记录在这个数据库上跑过的脚本，以及每个脚本的检查依据。这样每次应用启动的时候，就可以知道哪个脚本需要运行，或者哪个脚本发生了变动，运行基础可能不对，造成数据结构的混乱而阻止运行。
<a name="IUMdO"></a>
### 目标 2 的实现
有了上面的基础之后，来说说后续要做表结构的表变动该怎么操作，这也是之前读者出现问题最多的情况，所以在2.x版本教程中特地讲一讲。<br />首先，在开始使用Flyway之后，对于数据库表接口的变更就要关闭这几个途径：

1. 直接通过工具登录数据去修改表结构
2. 已经发布的sql脚本不允许修改

正确的表结构调整途径：在flyway脚本配置路径下编写新的脚本，启动程序来执行变更。这样可以获得几个很大的好处：

1. 脚本受Git版本管理控制，可以方便的找到过去的历史
2. 脚本在程序启动的时候先加载，再提供接口服务，一起完成部署步骤
3. 所有表结构的历史变迁，在管理目录中根据版本号就能很好的追溯

下面根据一个实际需求来具体操作下。假设现在想对User表增加一个字段：address，用来存储用户的通讯地址，那就需要这样操作实现。
<a name="SIOxo"></a>
#### 第一步：创建脚本文件V1_1__alter_table_user.sql，并写入增加address列的语句
```sql
ALTER TABLE `user` ADD COLUMN `address` VARCHAR(20) DEFAULT NULL;
```
对于脚本文件名的基本规则是：版本号__描述.sql。当然如果有更细致的要求，那么可以做更细致的文件名规划。
<a name="fzTVn"></a>
#### 第二步：再次执行单元测试，在控制台中可以看到如下日志：
```java
2021-01-11 16:58:12.025  INFO 37330 --- [           main] o.f.c.i.database.base.DatabaseType       : Database: jdbc:mysql://localhost:3306/test (MySQL 8.0)
2021-01-11 16:58:12.063  INFO 37330 --- [           main] o.f.core.internal.command.DbValidate     : Successfully validated 2 migrations (execution time 00:00.020s)
2021-01-11 16:58:12.075  INFO 37330 --- [           main] o.f.core.internal.command.DbMigrate      : Current version of schema `test`: 1
2021-01-11 16:58:12.082  INFO 37330 --- [           main] o.f.core.internal.command.DbMigrate      : Migrating schema `test` to version "1.1 - alter table user"
2021-01-11 16:58:12.113  INFO 37330 --- [           main] o.f.core.internal.command.DbMigrate      : Successfully applied 1 migration to schema `test` (execution time 00:00.045s)
```
再查看一下数据中的内容：<br />![2021-05-24-17-35-53-534764.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621849945742-a528b2b4-ff01-431f-98b3-1f9dfeb63913.png#clientId=u69fcfcee-c926-4&from=ui&id=u64fe528a&originHeight=450&originWidth=1080&originalType=binary&size=97242&status=done&style=shadow&taskId=u2c483c75-27dc-4426-bbed-6f834306430)
