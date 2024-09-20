Mybatis-plus<br />mybatis-mate 为 mp 企业级模块，支持分库分表，数据审计、数据敏感词过滤（AC算法），字段加密，字典回写（数据绑定），数据权限，表结构自动生成 SQL 维护等，旨在更敏捷优雅处理数据。
<a name="RZRmU"></a>
### 1、主要功能

- 字典绑定
- 字段加密
- 数据脱敏
- 表结构动态维护
- 数据审计记录
- 数据范围（数据权限）
- 数据库分库分表、动态据源、读写分离、数据库健康检查自动切换。
<a name="qWH2o"></a>
### 2、使用
<a name="DBZsE"></a>
#### 2.1 依赖导入
Spring Boot 引入自动依赖注解包
```xml
<dependency>
  <groupId>com.baomidou</groupId>
  <artifactId>mybatis-mate-starter</artifactId>
  <version>1.0.8</version>
</dependency>
```
注解（实体分包使用）
```xml
<dependency>
  <groupId>com.baomidou</groupId>
  <artifactId>mybatis-mate-annotation</artifactId>
  <version>1.0.8</version>
</dependency>
```
<a name="sChV6"></a>
#### 2.2 字段数据绑定（字典回写）
例如 `user_sex` 类型 sex 字典结果映射到 sexText 属性
```java
@FieldDict(type = "user_sex", target = "sexText")
private Integer sex;

private String sexText;
```
实现 IDataDict 接口提供字典数据源，注入到 Spring 容器即可。
```java
@Component
public class DataDict implements IDataDict {

    /**
     * 从数据库或缓存中获取
     */
    private Map<String, String> SEX_MAP = new ConcurrentHashMap<String, String>() {{
        put("0", "女");
        put("1", "男");
    }};

    @Override
    public String getNameByCode(FieldDict fieldDict, String code) {
        System.err.println("字段类型：" + fieldDict.type() + "，编码：" + code);
        return SEX_MAP.get(code);
    }
}
```
<a name="VU8hh"></a>
#### 2.3 字段加密
属性 `@FieldEncrypt` 注解即可加密存储，会自动解密查询结果，支持全局配置加密密钥算法，及注解密钥算法，可以实现 IEncryptor 注入自定义算法。
```java
@FieldEncrypt(algorithm = Algorithm.PBEWithMD5AndDES)
private String password;
```
<a name="ewkQE"></a>
#### 2.4 字段脱敏
属性 `@FieldSensitive` 注解即可自动按照预设策略对源数据进行脱敏处理，默认 SensitiveType 内置 9 种常用脱敏策略。<br />例如：中文名、银行卡账号、手机号码等 脱敏策略。<br />也可以自定义策略如下：
```java
@FieldSensitive(type = "testStrategy")
private String username;

@FieldSensitive(type = SensitiveType.mobile)
private String mobile;
```
自定义脱敏策略 testStrategy 添加到默认策略中注入 Spring 容器即可。
```java
@Configuration
public class SensitiveStrategyConfig {

    /**
     * 注入脱敏策略
     */
    @Bean
    public ISensitiveStrategy sensitiveStrategy() {
        // 自定义 testStrategy 类型脱敏处理
        return new SensitiveStrategy().addStrategy("testStrategy", t -> t + "***test***");
    }
}
```
例如文章敏感词过滤
```java
/**
 * 演示文章敏感词过滤
 */
@RestController
public class ArticleController {
    @Autowired
    private SensitiveWordsMapper sensitiveWordsMapper;

    // 测试访问下面地址观察请求地址、界面返回数据及控制台（ 普通参数 ）
    // 无敏感词 http://localhost:8080/info?content=tom&see=1&age=18
    // 英文敏感词 http://localhost:8080/info?content=my%20content%20is%20tomcat&see=1&age=18
    // 汉字敏感词 http://localhost:8080/info?content=%E7%8E%8B%E5%AE%89%E7%9F%B3%E5%94%90%E5%AE%8B%E5%85%AB%E5%A4%A7%E5%AE%B6&see=1
    // 多个敏感词 http://localhost:8080/info?content=%E7%8E%8B%E5%AE%89%E7%9F%B3%E6%9C%89%E4%B8%80%E5%8F%AA%E7%8C%ABtomcat%E6%B1%A4%E5%A7%86%E5%87%AF%E7%89%B9&see=1&size=6
    // 插入一个字变成非敏感词 http://localhost:8080/info?content=%E7%8E%8B%E7%8C%AB%E5%AE%89%E7%9F%B3%E6%9C%89%E4%B8%80%E5%8F%AA%E7%8C%ABtomcat%E6%B1%A4%E5%A7%86%E5%87%AF%E7%89%B9&see=1&size=6
    @GetMapping("/info")
    public String info(Article article) throws Exception {
        return ParamsConfig.toJson(article);
    }


    // 添加一个敏感词然后再去观察是否生效 http://localhost:8080/add
    // 观察【猫】这个词被过滤了 http://localhost:8080/info?content=%E7%8E%8B%E5%AE%89%E7%9F%B3%E6%9C%89%E4%B8%80%E5%8F%AA%E7%8C%ABtomcat%E6%B1%A4%E5%A7%86%E5%87%AF%E7%89%B9&see=1&size=6
    // 嵌套敏感词处理 http://localhost:8080/info?content=%E7%8E%8B%E7%8C%AB%E5%AE%89%E7%9F%B3%E6%9C%89%E4%B8%80%E5%8F%AA%E7%8C%ABtomcat%E6%B1%A4%E5%A7%86%E5%87%AF%E7%89%B9&see=1&size=6
    // 多层嵌套敏感词 http://localhost:8080/info?content=%E7%8E%8B%E7%8E%8B%E7%8C%AB%E5%AE%89%E7%9F%B3%E5%AE%89%E7%9F%B3%E6%9C%89%E4%B8%80%E5%8F%AA%E7%8C%ABtomcat%E6%B1%A4%E5%A7%86%E5%87%AF%E7%89%B9&see=1&size=6
    @GetMapping("/add")
    public String add() throws Exception {
        Long id = 3L;
        if (null == sensitiveWordsMapper.selectById(id)) {
            System.err.println("插入一个敏感词：" + sensitiveWordsMapper.insert(new SensitiveWords(id, "猫")));
            // 插入一个敏感词，刷新算法引擎敏感词
            SensitiveWordsProcessor.reloadSensitiveWords();
        }
        return "ok";
    }

    // 测试访问下面地址观察控制台（ 请求json参数 ）
    // idea 执行 resources 目录 TestJson.http 文件测试
    @PostMapping("/json")
    public String json(@RequestBody Article article) throws Exception {
        return ParamsConfig.toJson(article);
    }
}
```
<a name="X8SJU"></a>
#### 2.5 DDL 数据结构自动维护
解决升级表结构初始化，版本发布更新 SQL 维护问题，目前支持 MySql、PostgreSQL。
```java
@Component
public class PostgresDdl implements IDdl {

    /**
     * 执行 SQL 脚本方式
     */
    @Override
    public List<String> getSqlFiles() {
        return Arrays.asList(
                // 内置包方式
                "db/tag-schema.sql",
                // 文件绝对路径方式
                "D:\\db\\tag-data.sql"
        );
    }
}
```
不仅仅可以固定执行，也可以动态执行！！
```java
ddlScript.run(new StringReader("DELETE FROM user;\n" +
                "INSERT INTO user (id, username, password, sex, email) VALUES\n" +
                "(20, 'Duo', '123456', 0, 'Duo@baomidou.com');"));
```
它还支持多数据源执行！！！
```java
@Component
public class MysqlDdl implements IDdl {

    @Override
    public void sharding(Consumer<IDdl> consumer) {
        // 多数据源指定，主库初始化从库自动同步
        String group = "mysql";
        ShardingGroupProperty sgp = ShardingKey.getDbGroupProperty(group);
        if (null != sgp) {
            // 主库
            sgp.getMasterKeys().forEach(key -> {
                ShardingKey.change(group + key);
                consumer.accept(this);
            });
            // 从库
            sgp.getSlaveKeys().forEach(key -> {
                ShardingKey.change(group + key);
                consumer.accept(this);
            });
        }
    }

    /**
     * 执行 SQL 脚本方式
     */
    @Override
    public List<String> getSqlFiles() {
        return Arrays.asList("db/user-mysql.sql");
    }
}
```
<a name="X51Ky"></a>
#### 2.6 动态多数据源主从自由切换
`@Sharding` 注解使数据源不限制随意使用切换，可以在 mapper 层添加注解，按需求指哪打哪！！
```java
@Mapper
@Sharding("mysql")
public interface UserMapper extends BaseMapper<User> {

    @Sharding("postgres")
    Long selectByUsername(String username);
}
```
也可以自定义策略统一调兵遣将
```java
@Component
public class MyShardingStrategy extends RandomShardingStrategy {

    /**
     * 决定切换数据源 key {@link ShardingDatasource}
     *
     * @param group          动态数据库组
     * @param invocation     {@link Invocation}
     * @param sqlCommandType {@link SqlCommandType}
     */
    @Override
    public void determineDatasourceKey(String group, Invocation invocation, SqlCommandType sqlCommandType) {
        // 数据源组 group 自定义选择即可， keys 为数据源组内主从多节点，可随机选择或者自己控制
        this.changeDatabaseKey(group, sqlCommandType, keys -> chooseKey(keys, invocation));
    }
}
```
可以开启主从策略，当然也是可以开启健康检查！！！<br />具体配置：
```yaml
mybatis-mate:
  sharding:
    health: true # 健康检测
    primary: mysql # 默认选择数据源
    datasource:
      mysql: # 数据库组
        - key: node1
          ...
        - key: node2
          cluster: slave # 从库读写分离时候负责 sql 查询操作，主库 master 默认可以不写
          ...
      postgres:
        - key: node1 # 数据节点
          ...
```
<a name="Te5rM"></a>
#### 2.7 分布式事务日志打印
部分配置如下：
```java
/**
 * <p>
 * 性能分析拦截器，用于输出每条 SQL 语句及其执行时间
 * </p>
 */
@Slf4j
@Component
@Intercepts({@Signature(type = StatementHandler.class, method = "query", args = {Statement.class, ResultHandler.class}),
        @Signature(type = StatementHandler.class, method = "update", args = {Statement.class}),
        @Signature(type = StatementHandler.class, method = "batch", args = {Statement.class})})
public class PerformanceInterceptor implements Interceptor {
    /**
     * SQL 执行最大时长，超过自动停止运行，有助于发现问题。
     */
    private long maxTime = 0;
    /**
     * SQL 是否格式化
     */
    private boolean format = false;
    /**
     * 是否写入日志文件<br>
     * true 写入日志文件，不阻断程序执行！<br>
     * 超过设定的最大执行时长异常提示！
     */
    private boolean writeInLog = false;

    @Override
    public Object intercept(Invocation invocation) throws Throwable {
        Statement statement;
        Object firstArg = invocation.getArgs()[0];
        if (Proxy.isProxyClass(firstArg.getClass())) {
            statement = (Statement) SystemMetaObject.forObject(firstArg).getValue("h.statement");
        } else {
            statement = (Statement) firstArg;
        }
        MetaObject stmtMetaObj = SystemMetaObject.forObject(statement);
        try {
            statement = (Statement) stmtMetaObj.getValue("stmt.statement");
        } catch (Exception e) {
            // do nothing
        }
        if (stmtMetaObj.hasGetter("delegate")) {//Hikari
            try {
                statement = (Statement) stmtMetaObj.getValue("delegate");
            } catch (Exception e) {

            }
        }

        String originalSql = null;
        if (originalSql == null) {
            originalSql = statement.toString();
        }
        originalSql = originalSql.replaceAll("[\\s]+", " ");
        int index = indexOfSqlStart(originalSql);
        if (index > 0) {
            originalSql = originalSql.substring(index);
        }

        // 计算执行 SQL 耗时
        long start = SystemClock.now();
        Object result = invocation.proceed();
        long timing = SystemClock.now() - start;

        // 格式化 SQL 打印执行结果
        Object target = PluginUtils.realTarget(invocation.getTarget());
        MetaObject metaObject = SystemMetaObject.forObject(target);
        MappedStatement ms = (MappedStatement) metaObject.getValue("delegate.mappedStatement");
        StringBuilder formatSql = new StringBuilder();
        formatSql.append(" Time：").append(timing);
        formatSql.append(" ms - ID：").append(ms.getId());
        formatSql.append("\n Execute SQL：").append(sqlFormat(originalSql, format)).append("\n");
        if (this.isWriteInLog()) {
            if (this.getMaxTime() >= 1 && timing > this.getMaxTime()) {
                log.error(formatSql.toString());
            } else {
                log.debug(formatSql.toString());
            }
        } else {
            System.err.println(formatSql);
            if (this.getMaxTime() >= 1 && timing > this.getMaxTime()) {
                throw new RuntimeException(" The SQL execution time is too large, please optimize ! ");
            }
        }
        return result;
    }

    @Override
    public Object plugin(Object target) {
        if (target instanceof StatementHandler) {
            return Plugin.wrap(target, this);
        }
        return target;
    }

    @Override
    public void setProperties(Properties prop) {
        String maxTime = prop.getProperty("maxTime");
        String format = prop.getProperty("format");
        if (StringUtils.isNotEmpty(maxTime)) {
            this.maxTime = Long.parseLong(maxTime);
        }
        if (StringUtils.isNotEmpty(format)) {
            this.format = Boolean.valueOf(format);
        }
    }

    public long getMaxTime() {
        return maxTime;
    }

    public PerformanceInterceptor setMaxTime(long maxTime) {
        this.maxTime = maxTime;
        return this;
    }

    public boolean isFormat() {
        return format;
    }

    public PerformanceInterceptor setFormat(boolean format) {
        this.format = format;
        return this;
    }

    public boolean isWriteInLog() {
        return writeInLog;
    }

    public PerformanceInterceptor setWriteInLog(boolean writeInLog) {
        this.writeInLog = writeInLog;
        return this;
    }

    public Method getMethodRegular(Class<?> clazz, String methodName) {
        if (Object.class.equals(clazz)) {
            return null;
        }
        for (Method method : clazz.getDeclaredMethods()) {
            if (method.getName().equals(methodName)) {
                return method;
            }
        }
        return getMethodRegular(clazz.getSuperclass(), methodName);
    }

    /**
     * 获取sql语句开头部分
     *
     * @param sql
     * @return
     */
    private int indexOfSqlStart(String sql) {
        String upperCaseSql = sql.toUpperCase();
        Set<Integer> set = new HashSet<>();
        set.add(upperCaseSql.indexOf("SELECT "));
        set.add(upperCaseSql.indexOf("UPDATE "));
        set.add(upperCaseSql.indexOf("INSERT "));
        set.add(upperCaseSql.indexOf("DELETE "));
        set.remove(-1);
        if (CollectionUtils.isEmpty(set)) {
            return -1;
        }
        List<Integer> list = new ArrayList<>(set);
        Collections.sort(list, Integer::compareTo);
        return list.get(0);
    }

    private final static SqlFormatter sqlFormatter = new SqlFormatter();

    /**
     * 格式sql
     *
     * @param boundSql
     * @param format
     * @return
     */
    public static String sqlFormat(String boundSql, boolean format) {
        if (format) {
            try {
                return sqlFormatter.format(boundSql);
            } catch (Exception ignored) {
            }
        }
        return boundSql;
    }
}
```
使用：
```java
@RestController
@AllArgsConstructor
public class TestController {
    private BuyService buyService;

    // 数据库 test 表 t_order 在事务一致情况无法插入数据，能够插入说明多数据源事务无效
    // 测试访问 http://localhost:8080/test
    // 制造事务回滚 http://localhost:8080/test?error=true 也可通过修改表结构制造错误
    // 注释 ShardingConfig 注入 dataSourceProvider 可测试事务无效情况
    @GetMapping("/test")
    public String test(Boolean error) {
        return buyService.buy(null != error && error);
    }
}
```
<a name="iRYja"></a>
#### 2.8 数据权限
mapper 层添加注解:
```java
// 测试 test 类型数据权限范围，混合分页模式
@DataScope(type = "test", value = {
        // 关联表 user 别名 u 指定部门字段权限
        @DataColumn(alias = "u", name = "department_id"),
        // 关联表 user 别名 u 指定手机号字段（自己判断处理）
        @DataColumn(alias = "u", name = "mobile")
})
@Select("select u.* from user u")
List<User> selectTestList(IPage<User> page, Long id, @Param("name") String username);
```
模拟业务处理逻辑：
```java
@Bean
public IDataScopeProvider dataScopeProvider() {
    return new AbstractDataScopeProvider() {
        @Override
        protected void setWhere(PlainSelect plainSelect, Object[] args, DataScopeProperty dataScopeProperty) {
            // args 中包含 mapper 方法的请求参数，需要使用可以自行获取
            /*
                // 测试数据权限，最终执行 SQL 语句
                SELECT u.* FROM user u WHERE (u.department_id IN ('1', '2', '3', '5'))
                AND u.mobile LIKE '%1533%'
             */
            if ("test".equals(dataScopeProperty.getType())) {
                // 业务 test 类型
                List<DataColumnProperty> dataColumns = dataScopeProperty.getColumns();
                for (DataColumnProperty dataColumn : dataColumns) {
                    if ("department_id".equals(dataColumn.getName())) {
                        // 追加部门字段 IN 条件，也可以是 SQL 语句
                        Set<String> deptIds = new HashSet<>();
                        deptIds.add("1");
                        deptIds.add("2");
                        deptIds.add("3");
                        deptIds.add("5");
                        ItemsList itemsList = new ExpressionList(deptIds.stream().map(StringValue::new).collect(Collectors.toList()));
                        InExpression inExpression = new InExpression(new Column(dataColumn.getAliasDotName()), itemsList);
                        if (null == plainSelect.getWhere()) {
                            // 不存在 where 条件
                            plainSelect.setWhere(new Parenthesis(inExpression));
                        } else {
                            // 存在 where 条件 and 处理
                            plainSelect.setWhere(new AndExpression(plainSelect.getWhere(), inExpression));
                        }
                    } else if ("mobile".equals(dataColumn.getName())) {
                        // 支持一个自定义条件
                        LikeExpression likeExpression = new LikeExpression();
                        likeExpression.setLeftExpression(new Column(dataColumn.getAliasDotName()));
                        likeExpression.setRightExpression(new StringValue("%1533%"));
                        plainSelect.setWhere(new AndExpression(plainSelect.getWhere(), likeExpression));
                    }
                }
            }
        }
    };
}
```
最终执行 SQL 输出：
```sql
SELECT u.* FROM user u 
  WHERE (u.department_id IN ('1', '2', '3', '5')) 
  AND u.mobile LIKE '%1533%' LIMIT 1, 10
```
目前仅有付费版本，了解更多 mybatis-mate 使用示例详见：<br />[https://gitee.com/baomidou/mybatis-mate-examples](https://gitee.com/baomidou/mybatis-mate-examples)
