JavaMybatis-Plus
<a name="e8nmd"></a>
## 一、引言
先解释一下什么叫多租户，什么场景下使用多租户。<br />多租户是一种软件架构技术，在多用户的环境下，共有同一套系统，并且要注意数据之间的隔离性。<br />举个实际例子：曾经开发过一套H5程序，这套程序应用在不同医院的APP上，当医院患者下载医院APP，并且进入相对应的H5页面，APP则会把用户相关数据传输到开发者这里。在传输的时候需要带上医院标识（租户ID），以便将数据进行隔离。<br />当不同的租户使用同一套程序，这里就需要考虑一个数据隔离的情况。<br />数据隔离有三种方案：

- **独立数据库：简单来说就是一个租户使用一个数据库，这种数据隔离级别最高，安全性最好，但是提高成本。**
- **共享数据库、隔离数据架构：多租户使用同一个数据裤，但是每个租户对应一个Schema(数据库user)。**
- **共享数据库、共享数据架构：使用同一个数据库，同一个Schema，但是在表中增加了租户ID的字段，这种共享数据程度最高，隔离级别最低。**
<a name="jMTJ4"></a>
## 二、具体实现
这里采用方案三，即共享数据库，共享数据架构，因为这种方案服务器成本最低，但是提高了开发成本。<br />所以MP就提供了一种多租户的解决方案，实现方式是基于分页插件进行实现的，具体实现代码如下：
```java
/**
 * @Description: MybatisPlus配置类
 */
@Configuration
public class MyBatisPlusConfig {
 
 
    /**
     * 分页插件
     *
     * @return
     */
    @Bean
    public PaginationInterceptor paginationInterceptor() {
        PaginationInterceptor paginationInterceptor = new PaginationInterceptor();
 
        // 创建SQL解析器集合
        List<ISqlParser> sqlParserList = new ArrayList<>();
 
        // 创建租户SQL解析器
        TenantSqlParser tenantSqlParser = new TenantSqlParser();
 
        // 设置租户处理器
        tenantSqlParser.setTenantHandler(new TenantHandler() {
            @Override
            public Expression getTenantId() {
                // 设置当前租户ID，实际情况你可以从cookie、或者缓存中拿都行
                return new StringValue("jiannan");
            }
 
            @Override
            public String getTenantIdColumn() {
                // 对应数据库租户ID的列名
                return "tenant_id";
            }
 
            @Override
            public boolean doTableFilter(String tableName) {
                // 是否需要需要过滤某一张表
              /*  List<String> tableNameList = Arrays.asList("sys_user");
                if (tableNameList.contains(tableName)){
                    return true;
                }*/
                return false;
            }
        });
 
        sqlParserList.add(tenantSqlParser);
        paginationInterceptor.setSqlParserList(sqlParserList);
 
 
        return paginationInterceptor;
    }
 
}
```
配置好之后，不管是查询、新增、修改删除方法，MP都会自动加上租户ID的标识，测试如下：
```java
@Test
public void select(){
    List<User> users = userMapper.selectList(Wrappers.<User>lambdaQuery().eq(User::getAge, 18));
    users.forEach(System.out::println);
}
```
```java
DEBUG==>  Preparing: SELECT id, login_name, name, password, email, salt, sex, age, phone, user_type, status, organization_id, create_time, update_time, version, tenant_id FROM sys_user WHERE sys_user.tenant_id = 'jiannan' AND is_delete = '0' AND age = ? 
DEBUG==> Parameters: 18(Integer)
DEBUG<==      Total: 0
```
<a name="Fg9UC"></a>
## 三、特定SQL过滤
如果在程序中，有部分SQL不需要加上租户ID的表示，需要过滤特定的sql，可以通过如下两种方式：<br />方式一：在配置分页插件中加上配置`ISqlParserFilter`解析器，如果配置SQL很多，比较麻烦，不建议。
```java
paginationInterceptor.setSqlParserFilter(new ISqlParserFilter() {
	@Override
	public boolean doFilter(MetaObject metaObject) {
		MappedStatement ms = SqlParserHelper.getMappedStatement(metaObject);
		// 对应Mapper、dao中的方法
		if("com.example.demo.mapper.UserMapper.selectList".equals(ms.getId())){
			return true;
		}
		return false;
	}
});
```
方式二：通过租户注解的形式，目前只能作用于Mapper的方法上。
```java
/**
 * <p>
 * 用户 Mapper 接口
 * </p>
 */
public interface UserMapper extends BaseMapper<User> {
 
 
    /**
     * 自定Wrapper修改
     *
     * @param userWrapper 条件构造器
     * @param user        修改的对象参数
     * @return
     */
    @SqlParser(filter = true)
    int updateByMyWrapper(@Param(Constants.WRAPPER) Wrapper<User> userWrapper, @Param("user") User user);
 
}
```
```yaml
# 开启 SQL 解析缓存注解生效，如果MP版本在3.1.1及以上则不需要配置
mybatis-plus:
  global-config:
    sql-parser-cache: true
```
