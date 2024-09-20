JavaSpringBootFlyway
<a name="kBTjS"></a>
## **什么是Flyway? 要解决什么问题？**
Flyway是一款数据库迁移（migration）工具。简单点说，就是在部署应用的时候，执行数据库脚本的工具。<br />Flyway支持SQL和Java两种类型的脚本，可以将脚本打包到应用程序中，在应用程序启动时，由Flyway来管理这些脚本的执行，这些脚本被Flyway称之为migration。<br />_PS：本质上和liquibase机制一致_。<br />按照verion的顺序（和数据库中的更新记录对比，找到未更新的），更新如下<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364339648-fe9c6f94-dde2-4d5f-beba-402554fe57c0.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=u4961e62c&originHeight=152&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u820aaa01-a998-4a9a-bdaf-16e8a54da60&title=)<br />更新记录如下<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364339695-0a2346d2-06aa-4db8-9dce-41195b7021c9.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=ue534357d&originHeight=104&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u92d60fbc-2d55-4cb8-84a1-5a9c7247ef5&title=)
<a name="c8pm9"></a>
### Flyway中的变迁(migrations)
对于Flyway，对数据库的所有更改都称为变迁(migrations)，等同于liquibase中的changeset。<br />在Flyway中变迁(migrations)定义的更细，包含如下三种：

1. **版本变迁(Versioned Migrations)：每个版本执行一次，包含有版本、描述和校验和；常用于创建，修改，删除表；插入，修改数据等**
2. **撤销变迁(Undo Migrations)：版本变迁(Versioned Migrations)的反操作。**
3. **可重复变迁(Repeatable Migrations)：可以执行多次，包含描述和校验和（没有版本）；主要用于视图，存储过程，函数等**

这三种类型对应的格式如下：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364339857-9be08ae9-c4b8-4236-b908-255cb3690f4b.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=u7494cc1e&originHeight=194&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u075302a1-6cc9-42ac-9d01-366592a1ee7&title=)

1. **前缀：V 代表版本变迁(Versioned Migrations)，U 代表撤销变迁(Undo Migrations)， R 代表可重复变迁(Repeatable Migrations)**
2. **版本号：唯一的版本号，比如V1.0.1**
3. **分隔符：**`**__**`** (两个下划线)**
4. **描述信息：描述信息**
5. **后缀：.sql**

（PS：撤销变迁(Undo Migrations)在收费版本中）
<a name="DO6s3"></a>
### Flyway中常用命令
Flyway中的常用commands有哪些？什么含义？<br />**Migrate**：是Flyway工作流的核心。它将扫描文件系统或类路径以查找可用的Migrate。它将把它们与已应用于数据库的Migrate进行比较。如果发现任何差异则迁移数据。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364339693-c87feeb1-e93a-4585-aa46-4a308504bec4.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=u72bd79dc&originHeight=160&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue4cbf6e2-ebf8-4e7b-a8c6-26344ab100f&title=)<br />**Clean**：清除掉对应数据库Schema中所有的对象，包括表结构，视图，存储过程等，clean操作在dev 和 test阶段很好用；(PS：不能用在product环境)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364339698-c23b032e-3ddb-41cc-a7bd-ad2a468da6a5.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=uaf9f9961&originHeight=212&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u480a3b42-5b47-4ca9-9aa7-1d7bacbe724&title=)<br />**Info**：用于打印所有的Migrations的详细和状态信息，也是通过MetaData和Migrations完成的，可以快速定位当前的数据库版本；<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340138-5ec4e642-c6a4-4810-985e-09d61e129337.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=ud7e29e78&originHeight=138&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud4f0f634-f4fe-4637-9e8c-8b0418fd00f&title=)<br />**Validate**：验证以及apply的Migrations是否有变更，默认开启的；原理是对比MetaData表与本地Migrations的checkNum值，如果值相同则验证通过，否则失败。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340304-8aabaf0c-a6e2-4053-94a8-9ff05a3ff5c4.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=u77952829&originHeight=189&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0ac23602-5c26-4042-a506-772b439fa3f&title=)<br />**Undo**：Migrate的反操作，即回滚操作，这是收费功能<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340241-9ae7f5c2-3bb3-4b56-84b6-5225ae564b29.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=u022b2edf&originHeight=160&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u664f2e2a-fb10-4cac-b4d8-668dd967504&title=)<br />**BaseLine**：对已经存在数据库Schema结构的数据库一种解决方案。实现在非空数据库新建MetaData表，并把Migrations应用到该数据库；也可以应用到已有表结构的数据库中也可以实现添加Metadata表。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340369-ea8cd15a-4fce-47b7-846d-740ffc41f12d.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=u4cb1936a&originHeight=209&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9bbf5b4f-db48-4aee-aa05-4295db1b562&title=)<br />**Repair**：repair操作能够修复metaData表，该操作在metadata出现错误时很有用<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340400-34113e46-71ee-4b8b-bee9-0d985845af22.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=u698f6754&originHeight=209&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u318c1179-0ffe-44f8-ac9b-c19fd1ef79b&title=)
<a name="f739r"></a>
## **简单示例**
这里主要介绍基于SpringBoot集成flyway来管理数据库的变更。
<a name="bL24X"></a>
### POM依赖
Maven 包的依赖，主要包含MySQL驱动，JDBC(这里spring-boot-starter-data-jpa包含了jdbc包，当然直接引入jdbc包也行)，以及flyway包。
```xml
<dependency>
	<groupId>mysql</groupId>
	<artifactId>mysql-connector-java</artifactId>
	<version>8.0.28</version>
</dependency>
<dependency>
	<groupId>com.github.wenhao</groupId>
	<artifactId>jpa-spec</artifactId>
	<version>3.1.0</version>
</dependency>
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>

<dependency>
	<groupId>org.flywaydb</groupId>
	<artifactId>flyway-mysql</artifactId>
	<version>8.5.7</version>
</dependency>
```
<a name="sgVQH"></a>
### yml配置
SpringBoot AutoConfig默认已经包含了对flyway的配置，在spring.flyway配置下
```yaml
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/test_db_flyway?useSSL=false&autoReconnect=true&characterEncoding=utf8
    driver-class-name: com.mysql.cj.jdbc.Driver
    username: root
    password: bfXa4Pt2lUUScy8jakXf
  flyway:
    enabled: true
    encoding: UTF-8
    # 可以支持多个location, 用','隔开
    locations: classpath:db/migration
    # migrate是否校验
    validate-on-migrate: true
```
在开发时，更多的配置可以从如下SpringBoot AutoConfig中找到。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340568-900c0d8f-1b2b-4133-a70c-cf18cd1e832e.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=u55f1ab2a&originHeight=336&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0dfd3153-8efc-4058-8f2c-be81fef4e02&title=)
<a name="GK0lw"></a>
### Migrate配置
这里准备两个Versioned Migration<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340699-942861e8-e13c-43ea-abf9-ca2d7bf36dee.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=udd9d3374&originHeight=338&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8ea1da95-19aa-4b02-b856-c5f01094efa&title=)

- **V1.0__Init_DB.sql**
```sql
DROP TABLE IF EXISTS `tb_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_user` (
	`id` int(11) NOT NULL AUTO_INCREMENT,
	`user_name` varchar(45) NOT NULL,
	`password` varchar(45) NOT NULL,
	`email` varchar(45) DEFAULT NULL,
	`phone_number` int(11) DEFAULT NULL,
	`description` varchar(255) DEFAULT NULL,
	`create_time` datetime DEFAULT NULL,
	`update_time` datetime DEFAULT NULL,
	PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
```

- **V1.1__Init_Data.sql**
```sql
LOCK TABLES `tb_user` WRITE;
/*!40000 ALTER TABLE `tb_user` DISABLE KEYS */;
INSERT INTO `tb_user` VALUES (1,'pdai','dfasdf','suzhou.daipeng@gmail.com',1212121213,'afsdfsaf','2021-09-08 17:09:15','2021-09-08 17:09:15');
/*!40000 ALTER TABLE `tb_user` ENABLE KEYS */;
UNLOCK TABLES;
```
<a name="OL95a"></a>
### 测试
启动springBootApplication，可以看到如下log
```
2022-04-13 07:56:56.122  INFO 86030 --- [           main] o.f.c.i.database.base.DatabaseType       : Database: jdbc:mysql://localhost:3306/test_db_flyway (MySQL 8.0)
2022-04-13 07:56:56.220  INFO 86030 --- [           main] o.f.core.internal.command.DbValidate     : Successfully validated 2 migrations (execution time 00:00.074s)
2022-04-13 07:56:56.245  INFO 86030 --- [           main] o.f.c.i.s.JdbcTableSchemaHistory         : Creating Schema History table `test_db_flyway`.`flyway_schema_history` ...
2022-04-13 07:56:56.270  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.282  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.292  INFO 86030 --- [           main] o.f.core.internal.command.DbMigrate      : Current version of schema `test_db_flyway`: << Empty Schema >>
2022-04-13 07:56:56.297  INFO 86030 --- [           main] o.f.core.internal.command.DbMigrate      : Migrating schema `test_db_flyway` to version "1.0 - Init DB"
2022-04-13 07:56:56.309  WARN 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : DB: Unknown table 'test_db_flyway.tb_user' (SQL State: 42S02 - Error Code: 1051)
2022-04-13 07:56:56.309  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.309  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.310  WARN 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : DB: 'utf8' is currently an alias for the character set UTF8MB3, but will be an alias for UTF8MB4 in a future release. Please consider using UTF8MB4 in order to be unambiguous. (SQL State: HY000 - Error Code: 3719)
2022-04-13 07:56:56.310  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.317  WARN 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : DB: Integer display width is deprecated and will be removed in a future release. (SQL State: HY000 - Error Code: 1681)
2022-04-13 07:56:56.317  WARN 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : DB: Integer display width is deprecated and will be removed in a future release. (SQL State: HY000 - Error Code: 1681)
2022-04-13 07:56:56.317  WARN 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : DB: 'utf8' is currently an alias for the character set UTF8MB3, but will be an alias for UTF8MB4 in a future release. Please consider using UTF8MB4 in order to be unambiguous. (SQL State: HY000 - Error Code: 3719)
2022-04-13 07:56:56.317  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.318  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.333  INFO 86030 --- [           main] o.f.core.internal.command.DbMigrate      : Migrating schema `test_db_flyway` to version "1.1 - Init Data"
2022-04-13 07:56:56.334  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.335  WARN 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : DB: Table storage engine for 'tb_user' doesn't have this option (SQL State: HY000 - Error Code: 1031)
2022-04-13 07:56:56.335  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.335  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 1 rows affected
2022-04-13 07:56:56.336  WARN 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : DB: Table storage engine for 'tb_user' doesn't have this option (SQL State: HY000 - Error Code: 1031)
2022-04-13 07:56:56.337  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.337  INFO 86030 --- [           main] o.f.c.i.s.DefaultSqlScriptExecutor       : 0 rows affected
2022-04-13 07:56:56.346  INFO 86030 --- [           main] o.f.core.internal.command.DbMigrate      : Successfully applied 2 migrations to schema `test_db_flyway`, now at version v1.1 (execution time 00:00.058s)
```
生成的flyway更新的记录，test_db_flyway.flyway_schema_history<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340726-9218617f-3486-4d79-af4e-a93dd0033d93.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=ub232322e&originHeight=231&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8f4685e6-c116-4a6d-ad6e-db8bc8c29e9&title=)<br />已经user表结构和数据<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658364340766-2ce6eaea-8338-4113-849b-4a7f26c9397e.jpeg#clientId=u2e7431e0-1dc3-4&from=paste&id=ub6161bd6&originHeight=219&originWidth=660&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4cc27235-34bd-4913-9c50-86a174fa3a3&title=)
