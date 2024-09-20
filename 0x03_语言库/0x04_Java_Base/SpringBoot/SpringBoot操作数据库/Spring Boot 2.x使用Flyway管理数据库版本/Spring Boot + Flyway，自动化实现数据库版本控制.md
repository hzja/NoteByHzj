JavaSpringBootFlyway
<a name="L1eX1"></a>
## 为什么使用Flyway
最简单的一个项目是一个软件连接到一个数据库，但是大多数项目中不仅要处理开发环境的副本，还需要处理其他很多副本。例如：开发环境、测试环境、生产环境。想到数据库管理，立刻就能想到一系列问题

- 如何快速收集执行脚本的清单
- 执行的脚本总要人工执行，是否可以通过机器执行
- 执行的脚本是否已经在数据库执行过
- 执行的脚本是否全部在数据库中执行过
- 执行的脚本如何回退
- 如何初始化一个空数据库实例

Flyway是一款数据库版本控制管理工具，它可以简单的、可靠的升级你的数据库。它能帮助解决上面的问题。 Flyway核心是记录所有版本演化和状态的MetaData，首次启动创建默认名为SCHEMA_VERSION的元素表。表中保存了版本，描述，要执行的sql脚本等信息。<br />Flyway已经支持数据库包括:Oracle, SQL Server, SQL Azure, DB2, DB2 z/OS, MySQL (including Amazon RDS), MariaDB, Google Cloud SQL, PostgreSQL (including Amazon RDS and Heroku), Redshift, Vertica, H2, Hsql, Derby, SQLite, SAP HANA, solidDB, Sybase ASE and Phoeni<br />官网链接：[https://flywaydb.org/](https://flywaydb.org/)
<a name="i0HE6"></a>
## SpringBoot集成Flyway
<a name="thfSF"></a>
### 2.1 简单示例
参考版本信息<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667801384282-6dc8965b-6c1e-446a-9f9b-661026308dc5.png#averageHue=%23f9f9f9&clientId=u2099bfe2-e1d3-4&from=paste&id=u821b510e&originHeight=156&originWidth=499&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc0596631-c5c9-41b5-8feb-4ef0434be79&title=)<br />参考目录结构<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667801384309-1165fbaf-81ca-4e63-926d-508bcf192759.png#averageHue=%23fcfcfb&clientId=u2099bfe2-e1d3-4&from=paste&id=u4790e0ae&originHeight=261&originWidth=501&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u17e04979-e649-4066-b0ec-292cb10c3fe&title=)
<a name="Xcii9"></a>
#### 1.创建SpringBoot应用，并添加flyway-core依赖
本例中将实现初始化脚本到mysql数据库，因此同时引入了驱动依赖 mysql-connector-java
```xml
<dependency>
  <groupId>org.flywaydb</groupId>
  <artifactId>flyway-core</artifactId>
  <version>7.15.0</version>
</dependency>
```
参考pom.xml依赖如下
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
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-jdbc</artifactId>
  </dependency>

  <dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
    <version>8.0.28</version>
  </dependency>

  <dependency>
    <groupId>org.flywaydb</groupId>
    <artifactId>flyway-core</artifactId>
    <version>7.15.0</version>
  </dependency>

  <dependency>
    <groupId>org.junit.jupiter</groupId>
    <artifactId>junit-jupiter-api</artifactId>
    <version>5.8.2</version>
    <scope>test</scope>
  </dependency>

</dependencies>
```
<a name="ShqtA"></a>
#### 2.在application.properties中设置flyway信息
```
server.port=7002
 
##是否启动,默认开启
spring.flyway.enabled = true
##脚本存放路径
spring.flyway.locations = classpath:db/migration
##当flyway第一次运行时，会在对应的数据库中新建一个记录脚本运行情况的
spring.flyway.table=flyway_schema_history
## flyway指向的数据库链接
spring.datasource.url=jdbc:mysql://127.0.0.1:3306/runoob?useUnicode=true&characterEncoding=utf8
## 用户名
spring.flyway.user=nacos
## 密码
spring.flyway.password=nacos
## 数据库驱动
spring.flyway.driver-class-name=com.mysql.cj.jdbc.Driver
```
<a name="YU4Vn"></a>
#### 3.脚本整理
将脚本整理到resource/db.migration路径下，例如<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667801384284-fcd672fb-ef04-4e8b-868f-6c9888dce9af.png#averageHue=%23f2f1f0&clientId=u2099bfe2-e1d3-4&from=paste&id=u3793e2e2&originHeight=98&originWidth=500&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5e56e196-c549-44b8-98ad-89affe18ab9&title=)<br />参考SQL脚本信息如下
```sql
//V1.20190621.1854__CREATE_PERSION_TABLE.sql脚本内容
create table PERSON (
  ID int not null,
  NAME varchar(100) not null
);

//V1.20190621.1904__INIT_PERSION.sql 脚本内容
insert into PERSON (ID, NAME) values (1, 'Axel');
insert into PERSON (ID, NAME) values (2, 'Mr. Foo');
insert into PERSON (ID, NAME) values (3, 'Ms. Bar');
```
sql 目录中存放脚本文件，脚本名称命名方式

- **版本化迁移**：执行一遍，版本号唯一，有重复会报错： 格式：V+版本号 +双下划线+描述+结束符
- **重复的迁移**，不需要版本号，脚本发生变化启动就会执行：格式：R+双下划线+描述+结束符
- **撤消迁移**：格式：U+版本号 +双下划线+描述+结束符

![2022-11-07-14-16-22.141639300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1667801831285-8dd99eb3-3aff-4f4f-bd5c-e8262c3ea2e9.png#averageHue=%23fafaf9&clientId=u2099bfe2-e1d3-4&from=ui&id=uccd10212&originHeight=291&originWidth=869&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42488&status=done&style=none&taskId=u6b043d3c-7ccd-4c3e-8be9-0f3afd8f75d&title=)
<a name="Rzu7y"></a>
#### 4.运行启动主类，运行日志如下，从日志中可以看到如下信息

- 启动后正确链接到数据库runoob
- 验证2个迁移脚本成功
- 使用命令行的方式创建了一张名称为 flyway_schema_history 的记录表，这里要注意，所有脚本一旦执行了就会在 flyway_schema_history中创建记录， 如果出错引发问题，可以删除表中记录，反正启动的时候还会再执行，当然生产环境不建议此方法，但生产环境上部署的包都是验证过无问题的包也不会出现此问题
- 执行了resource/db.migration 目录下的两个脚本，并执行成功
```
INFO 190688 --- [           main] o.f.c.internal.license.VersionPrinter    : Flyway Community Edition 7.15.0 by Redgate
INFO 190688 --- [           main] o.f.c.i.database.base.BaseDatabaseType   : Database: jdbc:mysql://127.0.0.1:3306/runoob (MySQL 5.7)
INFO 190688 --- [           main] o.f.core.internal.command.DbValidate     : Successfully validated 2 migrations (execution time 00:00.016s)
INFO 190688 --- [           main] o.f.c.i.s.JdbcTableSchemaHistory         : Creating Schema History table `runoob`.`flyway_schema_history` with baseline ...
INFO 190688 --- [           main] o.f.core.internal.command.DbBaseline     : Successfully baselined schema with version: 1
INFO 190688 --- [           main] o.f.core.internal.command.DbMigrate      : Current version of schema `runoob`: 1
INFO 190688 --- [           main] o.f.core.internal.command.DbMigrate      : Migrating schema `runoob` to version "1.20190621.1854 - CREATE PERSION TABLE"
INFO 190688 --- [           main] o.f.core.internal.command.DbMigrate      : Migrating schema `runoob` to version "1.20190621.1904 - INIT PERSION"
INFO 190688 --- [           main] o.f.core.internal.command.DbMigrate      : Successfully applied 2 migrations to schema `runoob`, now at version v1.20190621.1904 (execution time 00:00.225s)
```
停止服务后，重新运行日志如下，从日志中可以看到信息

- 启动后正确链接到数据库runoob
- 验证2个迁移脚本成功
- 本次没有重复执行脚本， 日志中打印当前脚本编号20190621.1904， 即最后1次执行的脚本
```
INFO 193184 --- [           main] o.f.c.internal.license.VersionPrinter    : Flyway Community Edition 7.15.0 by Redgate
INFO 193184 --- [           main] o.f.c.i.database.base.BaseDatabaseType   : Database: jdbc:mysql://127.0.0.1:3306/runoob (MySQL 5.7)
INFO 193184 --- [           main] o.f.core.internal.command.DbValidate     : Successfully validated 3 migrations (execution time 00:00.024s)
INFO 193184 --- [           main] o.f.core.internal.command.DbMigrate      : Current version of schema `runoob`: 1.20190621.1904
INFO 193184 --- [           main] o.f.core.internal.command.DbMigrate      : Schema `runoob` is up to date. No migration necessary.
```
查看Mysql数据库<br />![2022-11-07-14-16-22.339477000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1667801831291-64eadde4-221b-496a-9ea0-8ed1e4c3460b.png#averageHue=%23faf7f7&clientId=u2099bfe2-e1d3-4&from=ui&id=nGjtW&originHeight=341&originWidth=1526&originalType=binary&ratio=1&rotation=0&showTitle=false&size=51137&status=done&style=none&taskId=u2e7ea6e7-a534-4c0d-9878-f17cca7c7c1&title=)
<a name="aNIMK"></a>
### 2.2 常见问题
<a name="h5k1d"></a>
#### 1.Caused by: org.flywaydb.core.api.FlywayException: Found non-empty schema(s)
```
Caused by: org.flywaydb.core.api.FlywayException: Found non-empty schema(s) `runoob` but no schema history table. Use baseline() or set baselineOnMigrate to true to initialize the schema history table.
 at org.flywaydb.core.Flyway$1.execute(Flyway.java:200) ~[flyway-core-7.15.0.jar:na]
 at org.flywaydb.core.Flyway$1.execute(Flyway.java:170) ~[flyway-core-7.15.0.jar:na]
 at org.flywaydb.core.Flyway.execute(Flyway.java:586) ~[flyway-core-7.15.0.jar:na]
```
问题原因：第一执行的时候没有找到schema history table ，这张表其实就是application.properties文件中spring.flyway.table属性配置的表，因此要么使用命令创建一个或者在application.properties文件中设置 `spring.flyway.baseline-on-migrate=true`，
<a name="qawgZ"></a>
#### 2.Caused by: org.flywaydb.core.api.FlywayException: Unsupported Database: MySQL 5.7
```
Caused by: org.flywaydb.core.api.FlywayException: Unsupported Database: MySQL 5.7
 at org.flywaydb.core.internal.database.DatabaseTypeRegister.getDatabaseTypeForConnection(DatabaseTypeRegister.java:106) ~[flyway-core-8.4.2.jar:na]
 at org.flywaydb.core.internal.jdbc.JdbcConnectionFactory.<init>(JdbcConnectionFactory.java:75) ~[flyway-core-8.4.2.jar:na]
 at org.flywaydb.core.FlywayExecutor.execute(FlywayExecutor.java:143) ~[flyway-core-8.4.2.jar:na]
 at org.flywaydb.core.Flyway.migrate(Flyway.java:124) ~[flyway-core-8.4.2.jar:na]
```
问题原因：flyway-core对数据库版本有要求，例如flyway-core的当前最高版本V8.4.3，不能使用 MySQL 5.7， 当flyway-core 降低到V7.15.0后 问题解决，所以匹配flyway-core和数据库版本后问题即可解决
<a name="S1YlN"></a>
### 2.3 源码参考
[https://github.com/PNZBEIJINGL/springboot](https://github.com/PNZBEIJINGL/springboot)
<a name="juJkH"></a>
## 总结
这里介绍了Springboot集成flyway方式

- 使用Flyway之前部署脚本方式一般为开发人员按照顺序汇总数据库的升级脚， 然后DBA或者售后在生产库中按照顺序执行升级脚本。
- 使用Flyway之后部署脚本方式就变更为开发人员将脚本构建到程序包中， 部署程序包后启动时Flyway自动执行脚本升级
