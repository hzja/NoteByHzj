Java SpringBoot
<a name="y7BOD"></a>
## 弃用内容
这次重新设计的只是对Datasource脚本初始化机制的重新设计。先来看看这次被弃用部分的内容（位于`org.springframework.boot.autoconfigure.jdbc.DataSourceProperties`），如果有用过这些配置内容，那么新配置就很容易理解了。
```java
 /**
  * Mode to apply when determining if DataSource initialization should be performed
  * using the available DDL and DML scripts.
  */
 @Deprecated
 private DataSourceInitializationMode initializationMode = DataSourceInitializationMode.EMBEDDED;

 /**
  * Platform to use in the DDL or DML scripts (such as schema-${platform}.sql or
  * data-${platform}.sql).
  */
 @Deprecated
 private String platform = "all";

 /**
  * Schema (DDL) script resource references.
  */
 private List<String> schema;

 /**
  * Username of the database to execute DDL scripts (if different).
  */
 @Deprecated
 private String schemaUsername;

 /**
  * Password of the database to execute DDL scripts (if different).
  */
 @Deprecated
 private String schemaPassword;

 /**
  * Data (DML) script resource references.
  */
 @Deprecated
 private List<String> data;

 /**
  * Username of the database to execute DML scripts (if different).
  */
 @Deprecated
 private String dataUsername;

 /**
  * Password of the database to execute DML scripts (if different).
  */
 @Deprecated
 private String dataPassword;

 /**
  * Whether to stop if an error occurs while initializing the database.
  */
 @Deprecated
 private boolean continueOnError = false;

 /**
  * Statement separator in SQL initialization scripts.
  */
 @Deprecated
 private String separator = ";";

 /**
  * SQL scripts encoding.
  */
 @Deprecated
 private Charset sqlScriptEncoding;

```
对应到配置文件里的属性如下（这里仅列出部分，就不全部列出了，主要就是对应上面源码中的属性）：
```java
spring.datasource.schema=
spring.datasource.schema-username=
spring.datasource.schema-password=
...
```
这些配置主要用来指定数据源初始化之后要用什么用户、去执行哪些脚本、遇到错误是否继续等功能。
<a name="s1zdH"></a>
## 新的设计
Spring Boot 2.5.0开始，启用了全新的配置方式，可以从这个类`org.springframework.boot.autoconfigure.sql.init.SqlInitializationProperties`里看到详情。<br />下面通过一个简单的例子来体验这个功能的作用。<br />创建一个Spring Boot的基础应用，并在pom.xml中引入和JDBC的依赖：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-jdbc</artifactId>
</dependency>

<dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
</dependency>
```
在配置文件中增加数据源和初始化数据源的配置，具体如下：
```java
spring.datasource.url=jdbc:mysql://localhost:3306/test
spring.datasource.username=root
spring.datasource.password=
spring.datasource.driver-class-name=com.mysql.cj.jdbc.Driver

# Spring Boot 2.5.0 init schema & data
# 执行初始化脚本的用户名称
spring.sql.init.username=root
# 执行初始化脚本的用户密码
spring.sql.init.password=
# 初始化的schema脚本位置
spring.sql.init.schema-locations=classpath*:schema-all.sql
```
根据上面配置的定义，接下来就在resource目录下，创建脚本文件schema-all.sql，并写入一些初始化表结构的脚本
```sql
create table test.user_info(
    id          int unsigned auto_increment comment '用户id'
        primary key,
    open_id     varchar(255)     default '' null comment '微信小程序openid',
    nick_name   varchar(255)     default '' null comment '微信名',
    head_img    varchar(255)     default '' null comment '微信头像',
    sex         varchar(255)     default '' null comment '性别',
    phone       varchar(255)     default '' null comment '手机',
    province    varchar(255)     default '' null comment '注册地址：省',
    city        varchar(255)     default '' null comment '注册地址：城市',
    country     varchar(255)     default '' null comment '注册地址：县/区',
    status      tinyint unsigned default 0  not null comment '是否标记删除 0：否 1：是',
    create_time datetime                    not null comment '创建时间',
    update_time datetime                    not null comment '更新时间'
)
comment '用户表';
```
完成上面步骤之后，启动应用。然后打开MySQL客户端，可以看到在test库下，多了一个user_info表<br />通过上面的例子，不难想到这样的功能主要可以用来管理应用启动与数据库配置的自动执行，以减少应用部署过程中手工执行的内容，降低应用部署的执行步骤。
<a name="dsXhY"></a>
## 配置详解
除了上面用到的配置属性之外，还有一些其他的配置，下面详细讲解一下作用。

- `spring.sql.init.enabled`：是否启动初始化的开关，默认是true。如果不想执行初始化脚本，设置为false即可。通过`-D`的命令行参数会更容易控制。
- `spring.sql.init.username`和`spring.sql.init.password`：配置执行初始化脚本的用户名与密码。这个非常有必要，因为安全管理要求，通常给业务应用分配的用户对一些建表删表等命令没有权限。这样就可以与datasource中的用户分开管理。
- `spring.sql.init.schema-locations`：配置与schema变更相关的sql脚本，可配置多个（默认用`;`分割）
- `spring.sql.init.data-locations`：用来配置与数据相关的sql脚本，可配置多个（默认用`;`分割）
- `spring.sql.init.encoding`：配置脚本文件的编码
- `spring.sql.init.separator`：配置多个sql文件的分隔符，默认是`;`
- `spring.sql.init.continue-on-error`：如果执行脚本过程中碰到错误是否继续，默认是false`；所以，上面的例子第二次执行的时候会报错并启动失败，因为第一次执行的时候表已经存在。
<a name="HGSgB"></a>
## 应用建议
关于这些配置的应用，把它与数据库的版本管理联系起来（因为可以自动的执行脚本）。<br />那么依靠这些配置，是否可以胜任业务应用部署时候数据库初始化的自动化实现呢？<br />就上述所介绍的配置，虽然具备了一定的自动执行能力。但由于缺失对当前环境的判断能力，所以要应对实际的部署场景来说，还是远远不够的。<br />如果要自动化的管理数据库表结构、初始化数据的话，建议是：

1. 默认提供的这个初始化功能可以且仅用于单元测试，自动创建数据库结构与初始化数据，使用完毕后销毁。可以方便的控制每次单元测试的执行环境一致。
2. 应用在环境部署的时候，使用Flyway来实现。
3. 联合Flyway一同使用，通过org.springframework.jdbc.datasource.init.DataSourceInitializer来定义更复杂的执行逻辑。
