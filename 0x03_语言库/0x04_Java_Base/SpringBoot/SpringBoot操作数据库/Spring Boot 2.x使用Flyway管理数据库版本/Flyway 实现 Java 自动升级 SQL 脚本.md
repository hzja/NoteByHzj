Java Flyway
<a name="frE8q"></a>
## 为什么要用Flyway
在日常开发中，经常会遇到下面的问题：

1. 自己写的SQL忘了在所有环境执行；
2. 别人写的SQL不能确定是否都在所有环境执行过了；
3. 有人修改了已经执行过的SQL，期望再次执行；
4. 需要新增环境做数据迁移；
5. 每次发版需要手动控制先发DB版本，再发布应用版本；
6. 其它场景...

由于项目需求的变化，或者前期设计缺陷，导致在后期需要修改数据库，这应该是一个比较常见的事情，如果项目还没上线，可能把表删除了重新创建，但是如果项目已经上线了，就不能这样简单粗暴了，每次运维部署项目，还得手动执行一遍SQL文件。需要通过 SQL 脚本在已有数据表的基础上进行升级。<br />有了flyway，这些问题都能得到很好的解决。<br />使用了 Flyway 之后，如果再想进行数据库版本升级，就不用该以前的数据库脚本了，直接创建新的数据库脚本，项目在启动时检测了有新的更高版本的脚本，就会自动执行，这样，在和其他同事配合工作时，也会方便很多。因为正常都是从 Git 上拉代码下来，不拉数据库脚本，这样要是有人更新了数据库，其他同事不一定能够收到最新的通知，使用了 Flyway 就可以有效避免这个问题了。<br />所有的脚本，一旦执行了，就会在 flyway_schema_history 表中有记录，如果不小心搞错了，可以手动从 flyway_schema_history 表中删除记录，然后修改 SQL 脚本后再重新启动（生产环境不建议）。
<a name="x1sh7"></a>
## Flyway是如何工作的
Flyway工作流程如下：

1. 项目启动，应用程序完成数据库连接池的建立后，Flyway自动运行。
2. 初次使用时，Flyway会创建一个flyway_schema_history表，用于记录sql执行记录。
3. Flyway会扫描项目指定路径下(默认是classpath:db/migration)的所有sql脚本，与flyway_schema_history表脚本记录进行比对。如果数据库记录执行过的脚本记录，与项目中的sql脚本不一致，Flyway会报错并停止项目执行。
4. 如果校验通过，则根据表中的sql记录最大版本号，忽略所有版本号不大于该版本的脚本。再按照版本号从小到大，逐个执行其余脚本。
<a name="hvs8d"></a>
## 项目中使用Flyway
首先，在pom文件中引入flyway的核心依赖包：
<a name="hIAKL"></a>
### 1. 引入核心依赖包
```xml
<dependency>
  <groupId>org.flywaydb</groupId>
  <artifactId>flyway-core</artifactId>
  <version>5.2.4</version>
</dependency>
```
![2021-07-09-23-57-40-299551.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846500117-3802396f-d0b0-4bb5-881f-e2eb91fb9ead.png#clientId=u9448840c-4908-4&from=ui&id=u255dc3ce&originHeight=520&originWidth=1032&originalType=binary&ratio=1&size=389935&status=done&style=shadow&taskId=udf2de758-a101-4155-9512-5e30e78e4a7)<br />这里使用5.2.4版本。经测试7.0.0版本与目前使用的SpringBoot版本有冲突，会导致flyway不执行。因此尽量不要使用高版本的flyway。
<a name="h4ial"></a>
### 2. 配置文件
简单配置一个属性即可使用
```yaml
# flyway 配置
spring:
  flyway:
    # 启用或禁用 flyway
    enabled: true
    # flyway 的 clean 命令会删除指定 schema 下的所有 table, 生产务必禁掉。这个默认值是 false 理论上作为默认配置是不科学的。
    clean-disabled: true
    # SQL 脚本的目录,多个路径使用逗号分隔 默认值 classpath:db/migration
    locations: classpath:db/migration
    #  metadata 版本控制信息表 默认 flyway_schema_history
    table: flyway_schema_history
    # 如果没有 flyway_schema_history 这个 metadata 表， 在执行 flyway migrate 命令之前, 必须先执行 flyway baseline 命令
    # 设置为 true 后 flyway 将在需要 baseline 的时候, 自动执行一次 baseline。
    baseline-on-migrate: true
    # 指定 baseline 的版本号,默认值为 1, 低于该版本号的 SQL 文件, migrate 时会被忽略
    baseline-version: 1
    # 字符编码 默认 UTF-8
    encoding: UTF-8
    # 是否允许不按顺序迁移 开发建议 true  生产建议 false
    out-of-order: false
    # 需要 flyway 管控的 schema list,这里我们配置为flyway  缺省的话, 使用spring.datasource.url 配置的那个 schema,
    # 可以指定多个schema, 但仅会在第一个schema下建立 metadata 表, 也仅在第一个schema应用migration sql 脚本.
    # 但flyway Clean 命令会依次在这些schema下都执行一遍. 所以 确保生产 spring.flyway.clean-disabled 为 true
    schemas: flyway
    # 执行迁移时是否自动调用验证   当你的 版本不符合逻辑 比如 你先执行了 DML 而没有 对应的DDL 会抛出异常
    validate-on-migrate: true
```
flyway的properties配置清单(属性未测试)：
```
# 对执行迁移时基准版本的描述.
flyway.baseline-description
#当迁移时发现目标schema非空，而且带有没有元数据的表时，是否自动执行基准迁移，默认false.
flyway.baseline-on-migrate =false
#开始执行基准迁移时对现有的schema的版本打标签，默认值为1.
flyway.baseline-version=1
#检查迁移脚本的位置是否存在，默认false.
flyway.check-location=false
#当发现校验错误时是否自动调用clean，默认false.
flyway.clean-on-validation-error=false
#是否开启flywary，默认true.
flyway.enabled=true
#设置迁移时的编码，默认UTF-8.
flyway.encoding
#当读取元数据表时是否忽略错误的迁移，默认false.
flyway.ignore-failed-future-migration
#当初始化好连接时要执行的SQL.
flyway.init-sqls
#迁移脚本的位置，默认db/migration.
flyway.locations
#是否允许无序的迁移，默认false.
flyway.out-of-order
#目标数据库的密码.
flyway.password
#设置每个placeholder的前缀，默认${.
flyway.placeholder-prefix
#是否要被替换，默认true.
flyway.placeholder-replacementplaceholders
#设置每个placeholder的后缀，默认}.
flyway.placeholder-suffix
#设置placeholder的value
flyway.placeholders.[placeholder name]
#设定需要flywary迁移的schema，大小写敏感，默认为连接默认的schema.
flyway.schemas
#迁移文件的前缀，默认为V.
flyway.sql-migration-prefix
#迁移脚本的文件名分隔符，默认__
flyway.sql-migration-separator
#迁移脚本的后缀，默认为.sql
flyway.sql-migration-suffix
#使用的元数据表名，默认为schema_version
flyway.tableflyway
#迁移时使用的目标版本，默认为latest version
flyway.target
#迁移时使用的JDBC URL，如果没有指定的话，将使用配置的主数据源
flyway.url
#迁移数据库的用户名
flyway.user
#迁移时是否校验，默认为true
flyway.validate-on-migrate
```
flyway的yml配置清单(已测试，没问题，推荐使用yml格式的配置文件)
```yaml
# flyway 配置
spring:
  flyway:
    # 启用或禁用 flyway
    enabled: true
    # flyway 的 clean 命令会删除指定 schema 下的所有 table, 生产务必禁掉。这个默认值是 false 理论上作为默认配置是不科学的。
    clean-disabled: true
    # SQL 脚本的目录,多个路径使用逗号分隔 默认值 classpath:db/migration
    locations: classpath:db/migration
    #  metadata 版本控制信息表 默认 flyway_schema_history
    table: flyway_schema_history
    # 如果没有 flyway_schema_history 这个 metadata 表， 在执行 flyway migrate 命令之前, 必须先执行 flyway baseline 命令
    # 设置为 true 后 flyway 将在需要 baseline 的时候, 自动执行一次 baseline。
    baseline-on-migrate: true
    # 指定 baseline 的版本号,默认值为 1, 低于该版本号的 SQL 文件, migrate 时会被忽略
    baseline-version: 1
    # 字符编码 默认 UTF-8
    encoding: UTF-8
    # 是否允许不按顺序迁移 开发建议 true  生产建议 false
    out-of-order: false
    # 需要 flyway 管控的 schema list,这里我们配置为flyway  缺省的话, 使用spring.datasource.url 配置的那个 schema,
    # 可以指定多个schema, 但仅会在第一个schema下建立 metadata 表, 也仅在第一个schema应用migration sql 脚本.
    # 但flyway Clean 命令会依次在这些schema下都执行一遍. 所以 确保生产 spring.flyway.clean-disabled 为 true
    schemas: flyway
    # 执行迁移时是否自动调用验证   当你的 版本不符合逻辑 比如 你先执行了 DML 而没有 对应的DDL 会抛出异常
    validate-on-migrate: true
```
`spring.flyway.clean-disabled`：这个属性非常关键，它表示是否要清除已有库下的表，如果执行的脚本是 V1__xxx.sql，那么会先清除已有库下的表，然后再执行脚本，这在开发环境下还挺方便，但是在生产环境下就要命了，而且它默认就是要清除，生产环境一定要自己配置设置为 `true`。
<a name="EypmT"></a>
### 3. 创建db/migration
因为flyway默认是读取resources/db/migration下的文件夹，如果需要修改这个路径，可以在配置文件中实现<br />![2021-07-09-23-57-40-379557.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846531404-8612656f-875f-4df3-ba34-f55ff9757527.png#clientId=u9448840c-4908-4&from=ui&id=uea419b1f&originHeight=195&originWidth=334&originalType=binary&ratio=1&size=38336&status=done&style=none&taskId=ue9a1954a-cb69-4385-81ae-d5c407be1cd)
<a name="bcnMk"></a>
### 4. 编写sql文件
此处的SQL语句命名需要遵从一定的规范，否则运行的时候flyway会报错。命名规则主要有两种：

- 仅需要被执行一次的SQL命名以大写的"V"开头，后面跟上"0~9"数字的组合,数字之间可以用“.”或者下划线"_"分割开，然后再以两个下划线 __ 分割，其后跟文件名称，最后以.sql结尾。比如，V20210707__create_user.sql、V20210707__add_user.sql。
- 可重复运行的SQL，则以大写的“R”开头，后面再以两个下划线分割，其后跟文件名称，最后以.sql结尾。。比如，R__truncate_user_dml.sql。

其中，V开头的SQL执行优先级要比R开头的SQL优先级高。<br />V: 固定大写<br />20210707.01：20210707是日期，后面用.01代表序号<br />因为flyway的执行是有个顺序的，比如执行了V2021__create_user，又执行V2020_update_user。就会报错，原因就是2020<2021。所以要保证序号是依次增大。<br />Flyway 是如何比较两个 SQL 文件的先后顺序呢？它采用 采用左对齐原则, 缺位用 0 代替 。举几个例子：<br /> 1.0.1.1 比 1.0.1 版本高。<br /> 1.0.10 比 1.0.9.4 版本高。<br /> 1.0.10 和 1.0.010 版本号一样高, 每个版本号部分的前导 0 会被忽略。<br />`__`:这个是两个 `_`<br />create_user是一个简单的sql描述<br />.sql：以.sql结尾的文件后缀是约定<br />![2021-07-09-23-57-40-553553.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846568083-8abd37a1-a8ba-44a1-83ea-12820f40f9b5.png#clientId=u9448840c-4908-4&from=ui&id=ub128e38a&originHeight=135&originWidth=341&originalType=binary&ratio=1&size=28210&status=done&style=none&taskId=ud1fa4991-c371-4948-8cef-4aa106e3546)![2021-07-09-23-57-40-682551.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846568464-74882aa6-1f4a-491a-acce-f12af603f3da.png#clientId=u9448840c-4908-4&from=ui&id=ua104adc3&originHeight=347&originWidth=1080&originalType=binary&ratio=1&size=300761&status=done&style=none&taskId=u23488ea9-81b2-4784-8346-9a05de53cc4)![2021-07-09-23-57-41-138120.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846568367-c12d7e7d-c973-4a50-857e-b36812434049.png#clientId=u9448840c-4908-4&from=ui&id=u7255d8e8&originHeight=333&originWidth=1080&originalType=binary&ratio=1&size=276059&status=done&style=none&taskId=u07c071bb-02c2-4676-bde6-2c3a0f65614)![2021-07-09-23-57-41-275111.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846568316-b5ca9e57-1288-4ad3-ae68-c169d483efa7.png#clientId=u9448840c-4908-4&from=ui&id=u3ce75c7c&originHeight=240&originWidth=1080&originalType=binary&ratio=1&size=191307&status=done&style=none&taskId=uc1148a9f-2e62-4800-811e-76940abe0a5)![2021-07-09-23-57-41-365116.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846568134-3b58b44e-c2a1-4cda-9805-1a3b2cfe7107.png#clientId=u9448840c-4908-4&from=ui&id=ua16692c5&originHeight=232&originWidth=935&originalType=binary&ratio=1&size=7501&status=done&style=none&taskId=u69900302-aa8b-4b1e-b696-fd73d0753dd)<br />只要在数据库中创建flyway这个数据库，启动项目，flyway就会执行sql文件，创建user表，并且会自动生成一个flyway_schema_history表<br />![2021-07-09-23-57-41-628112.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846581124-d29a0f61-98d1-4b03-9b0d-39e85b109215.png#clientId=u9448840c-4908-4&from=ui&id=u6fa85fb6&originHeight=137&originWidth=1080&originalType=binary&ratio=1&size=155861&status=done&style=none&taskId=udaedb951-c0e5-43e3-acd8-434c09eae0f)<br />从这段启动日志中，可以看到 Flyway 的执行信息，数据库脚本的执行执行，同时这里还说了，Flyway 还给创建了一个 flyway_schema_history 表，这个表用来记录数据库的更新历史。<br />![2021-07-09-23-57-41-740478.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846594442-2ddaed9b-bae8-4abc-9c4e-2d01ff537f58.png#clientId=u9448840c-4908-4&from=ui&id=u0e57bee3&originHeight=62&originWidth=281&originalType=binary&ratio=1&size=1424&status=done&style=shadow&taskId=u58634892-f574-4978-bd34-5d9ed9aeb71)<br />flyway_schema_history里面会去记录sql文件的执行记录，每次启动项目，都会去flyway_schema_history看sql是否执行过，如果没有执行过，说明这个sql是新的sql，就会自动执行，并且记录到表里面。<br />![2021-07-09-23-57-41-901470.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1625846607113-47ce8a55-66cb-4cfd-bbf8-6a7f9592c927.png#clientId=u9448840c-4908-4&from=ui&id=u34631a78&originHeight=95&originWidth=1080&originalType=binary&ratio=1&size=42461&status=done&style=none&taskId=ufb4958eb-7abe-4c5a-84be-3e237ca728c)<br />有了这条记录，下次再启动项目，V20210707.01、V20210707.02、V20210708.01 这个三个脚本文件就不会执行了，因为系统知道这个脚本已经执行过了，如果还想让这些脚本再执行一遍，需要手动删除 flyway_schema_history 表中的对应记录，那么项目启动时，这个脚本就会被执行了。<br />R开头的文件和V开头的文件略有不同，R开头的文件只要发送修改，都会执行一遍。V开头的文件如果执行过一般，在发送修改，就会报错。为了控制版本，尽量使用V开头的文件，这样也可以很清楚的看到每个版本中的sql文件。
<a name="S62ei"></a>
## 常见问题
<a name="qEx0V"></a>
### 问题1
flyway遇到的问题Caused by: java.lang.ClassNotFoundException: org.flywaydb.core.api.callback.FlywayCallbac<br />原因：SpringBoot版本和flyway版本不一致，一般是flyway版本过高。<br />解决办法：将SpringBoot的版本降到5.2.4就ok了
<a name="hmJT6"></a>
### 问题2
SpringBoot整合flyway 但是不生效，flyway不会自动执行sql<br />原因：如上<br />原因2：项目中没有配置数据库，没有引入sq依赖或者配置<br />解决办法：如上<br />解决办法2：引入sql依赖，在yml文件中配置sql信息
<a name="HP0Sw"></a>
### 问题3
flyway出错 FlywayException: Validate failed: Detected failed migration to version<br />原因：sql脚本和数据库中有冲突，需要检查sql脚本哪里错了。简单的说就是V开头的sql文件，已经执行过了，在 flyway_schema_history 表里面有这个数据，但是又改动了sql文件，导致再次执行的时候报错。<br />解决办法：新建一个sql文件，不要修改原来以V开头的文件或者在flyway_schema_history表中找到文件相关执行记录，删掉重新执行。
