JavaSpring
<a name="Euhdz"></a>
## 什么是JdbcTemplate？
Java中操作db最原始的方式就是纯jdbc了，是不是每次操作db都需要加载数据库驱动、获取连接、获取`PreparedStatement`、执行sql、关闭`PreparedStatement`、关闭连接等等，操作还是比较繁琐的，Spring中提供了一个模块，对jdbc操作进行了封装，使其更简单，就是JdbcTemplate，JdbcTemplate是Spring对JDBC的封装，目的是使JDBC更加易于使用。<br />下面来看一下JdbcTemplate到底怎么玩的？
<a name="BEsvJ"></a>
## JdbcTemplate使用步骤

1. 创建数据源DataSource
2. 创建JdbcTemplate，`new JdbcTemplate(dataSource)`
3. 调用JdbcTemplate的方法操作db，如增删改查
```java
public class DataSourceUtils {
    public static DataSource getDataSource() {
        org.apache.tomcat.jdbc.pool.DataSource dataSource = new org.apache.tomcat.jdbc.pool.DataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/javacode2018?characterEncoding=UTF-8");
        dataSource.setUsername("root");
        dataSource.setPassword("root123");
        dataSource.setInitialSize(5);
        return dataSource;
    }
}

@Test
public void test0() {
    //1.创建数据源DataSource
    DataSource dataSource = DataSourceUtils.getDataSource();
    //2.创建JdbcTemplate，new JdbcTemplate(dataSource)
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    //3.调用JdbcTemplate的方法操作db，如增删改查
    List<Map<String, Object>> maps = jdbcTemplate.queryForList("select * from t_user");
    System.out.println(maps);
}
```
输出
```
[{id=114, name=Tom}, {id=115, name=java}, {id=116, name=spring}]
```
t_user表数据
```sql
mysql> select id,name from t_user;
+-----+---------------+
| id  | name          |
+-----+---------------+
| 114 | Tom           |
| 115 | java          |
| 116 | spring        |
+-----+---------------+
3 rows in set (0.00 sec)
```
上面查询返回了t_user表所有的记录，返回了一个集合，集合中是一个Map，Map表示一行记录，key为列名，value为列对应的值。<br />有没有感觉到特别的方便，只需要`jdbcTemplate.queryForList("select * from t_user")`这么简单的一行代码，数据就被获取到了。<br />下面继续探索更强大更好用的功能。
<a name="MF3JW"></a>
## 增加、删除、修改操作
JdbcTemplate中以`update`开头的方法，用来执行增、删、改操作，下面来看几个常用的。
<a name="jZrN8"></a>
### 无参情况
<a name="zFGpN"></a>
#### Api
```java
int update(final String sql)
```
<a name="udH9W"></a>
#### 案例
```java
@Test
public void test1() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    int updateRows = jdbcTemplate.update("INSERT INTO t_user (name) VALUE ('maven系列')");
    System.out.println("影响行数：" + updateRows);
}
```
<a name="C9qyK"></a>
### 有参情况1
<a name="m1AK6"></a>
#### Api
```java
int update(String sql, Object... args)
```
<a name="xqknE"></a>
#### 案例
sql中使用`?`作为占位符。
```java
@Test
public void test2() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    int updateRows = jdbcTemplate.update("INSERT INTO t_user (name) VALUE (?)", "mybatis系列");
    System.out.println("影响行数：" + updateRows);
}
```
<a name="i36ei"></a>
### 有参情况2
<a name="rSn5q"></a>
#### Api
```java
int update(String sql, PreparedStatementSetter pss)
```
通过`PreparedStatementSetter`来设置参数，是个函数式接口，内部有个`setValues`方法会传递一个`PreparedStatement`参数，可以通这个参数手动的设置参数的值。
<a name="jVfhz"></a>
#### 案例
```java
@Test
public void test3() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    int updateRows = jdbcTemplate.update("INSERT INTO t_user (name) VALUE (?)", new PreparedStatementSetter() {
        @Override
        public void setValues(PreparedStatement ps) throws SQLException {
            ps.setString(1, "mysql");
        }
    });
    System.out.println("影响行数：" + updateRows);
}
```
<a name="tWgg4"></a>
## 获取自增列的值
<a name="sisWw"></a>
### Api
```java
public int update(final PreparedStatementCreator psc, final KeyHolder generatedKeyHolder)
```
<a name="bY7pi"></a>
### 案例
```java
@Test
public void test4() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String sql = "INSERT INTO t_user (name) VALUE (?)";
    KeyHolder keyHolder = new GeneratedKeyHolder();
    int rowCount = jdbcTemplate.update(new PreparedStatementCreator() {
        @Override
        public PreparedStatement createPreparedStatement(Connection con) throws SQLException {
            //手动创建PreparedStatement，注意第二个参数：Statement.RETURN_GENERATED_KEYS
            PreparedStatement ps = con.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);
            ps.setString(1, "获取自增列的值");
            return ps;
        }
    }, keyHolder);
    System.out.println("新记录id：" + keyHolder.getKey().intValue());
}
```
输出
```
新记录id：122
```
```sql
mysql> select id,name from t_user;
+-----+-----------------------+
| id  | name                  |
+-----+-----------------------+
| 114 | Tom                   |
| 115 | java                  |
| 116 | spring                |
| 117 | maven                 |
| 118 | mysql                 |
| 122 | 获取自增列的值         |
+-----+-----------------------+
6 rows in set (0.00 sec)
```
<a name="MUlG4"></a>
## 批量增删改操作
<a name="Eb11p"></a>
### Api
```java
int[] batchUpdate(final String[] sql);
int[] batchUpdate(String sql, List<Object[]> batchArgs);
int[] batchUpdate(String sql, List<Object[]> batchArgs, int[] argTypes);
```
<a name="BhvrZ"></a>
### 案例
```java
@Test
public void test5() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    List<Object[]> list = Arrays.asList(
            new Object[]{"刘德华"}, 
            new Object[]{"郭富城"}, 
            new Object[]{"张学友"}, 
            new Object[]{"黎明"});
    int[] updateRows = jdbcTemplate.batchUpdate("INSERT INTO t_user (name) VALUE (?)", list);
    for (int updateRow : updateRows) {
        System.out.println(updateRow);
    }
}
```
<a name="mzVjW"></a>
## 查询操作
<a name="JrVnJ"></a>
### 查询一列单行
<a name="EUaLU"></a>
#### Api
```java
/**
 * sql：执行的sql，如果有参数，参数占位符?
 * requiredType：返回的一列数据对应的java类型，如String
 * args：?占位符对应的参数列表
 **/
<T> T queryForObject(String sql, Class<T> requiredType, @Nullable Object... args)
```
<a name="zSH1M"></a>
#### 案例
```java
@Test
public void test6() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String name = jdbcTemplate.queryForObject("select name from t_user where id = ?", String.class, 114);
    System.out.println(name);
}
```
输出
```
路人
```
db中对应数据
```sql
mysql> select name from t_user where id = 114;
+--------+
| name   |
+--------+
| Tom    |
+--------+
1 row in set (0.00 sec)
```
<a name="Tt0V1"></a>
#### 使用注意
若`queryForObject`中sql查询无结果时，会报错<br />如id为0的记录不存在
```sql
mysql> select name from t_user where id = 0;
Empty set (0.00 sec)
```
```java
@Test
public void test7() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String name = jdbcTemplate.queryForObject("select name from t_user where id = ?", String.class, 0);
    System.out.println(name);
}
```
运行，会弹出一个异常`EmptyResultDataAccessException`，期望返回一条记录，但实际上却没有找到记录，和期望结果不符，所以报错了
```
org.springframework.dao.EmptyResultDataAccessException: Incorrect result size: expected 1, actual 0

 at org.springframework.dao.support.DataAccessUtils.nullableSingleResult(DataAccessUtils.java:97)
 at org.springframework.jdbc.core.JdbcTemplate.queryForObject(JdbcTemplate.java:784)
```
这种如何解决呢，需要用到查询多行的方式来解决了，即下面要说到的`queryForList`相关的方法，无结果的时候会返回一个空的List，可以在这个空的List上面做文章。
<a name="K1LBN"></a>
### 查询一列多行
<a name="Vbj0y"></a>
#### Api
以`queryForList`开头的方法。
```java
<T> List<T> queryForList(String sql, Class<T> elementType);
<T> List<T> queryForList(String sql, Class<T> elementType, @Nullable Object... args);
<T> List<T> queryForList(String sql, Object[] args, Class<T> elementType);
<T> List<T> queryForList(String sql, Object[] args, int[] argTypes, Class<T> elementType);
```
注意：<br />上面这个T虽然是泛型，但是只支持Integer.class String.class 这种单数据类型的，自己定义的Bean不支持。（所以用来查询单列数据）<br />elementType：查询结果需要转换为哪种类型？如String、Integer、Double。
<a name="fee6u"></a>
#### 案例
```java
@Test
public void test8() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    //<T> List<T> queryForList(String sql, Class<T> elementType);
    List<String> list1 = jdbcTemplate.queryForList("select name from t_user where id>131", String.class);
    System.out.println("list1:" + list1);

    //<T> List<T> queryForList(String sql, Class<T> elementType, @Nullable Object... args);
    List<String> list2 = jdbcTemplate.queryForList("select name from t_user where id>?", String.class, 131);
    System.out.println("list2:" + list2);

    //<T> List<T> queryForList(String sql, Object[] args, Class<T> elementType);
    List<String> list3 = jdbcTemplate.queryForList("select name from t_user where id>?", new Object[]{131}, String.class);
    System.out.println("list3:" + list3);

    //<T> List<T> queryForList(String sql, Object[] args, int[] argTypes, Class<T> elementType);
    List<String> list4 = jdbcTemplate.queryForList("select name from t_user where id>?", new Object[]{131}, new int[]{java.sql.Types.INTEGER}, String.class);
    System.out.println("list4:" + list4);
}
```
输出
```
list1:[郭富城, 张学友, 黎明]
list2:[郭富城, 张学友, 黎明]
list3:[郭富城, 张学友, 黎明]
list4:[郭富城, 张学友, 黎明]
```
sql结果：
```sql
mysql> select name from t_user where id>131;
+-----------+
| name      |
+-----------+
| 郭富城    |
| 张学友    |
| 黎明      |
+-----------+
3 rows in set (0.00 sec)
```
<a name="qSCpH"></a>
### 查询单行记录，将记录转换成一个对象
<a name="d1oDN"></a>
#### Api
```java
<T> T queryForObject(String sql, RowMapper<T> rowMapper);
<T> T queryForObject(String sql, Object[] args, RowMapper<T> rowMapper);
<T> T queryForObject(String sql, Object[] args, int[] argTypes, RowMapper<T> rowMapper);
<T> T queryForObject(String sql, RowMapper<T> rowMapper, Object... args);
```
上面这些方法的参数中都有一个`rowMapper`参数，行映射器，可以将当前行的结果映射为一个自定义的对象。
```java
@FunctionalInterface
public interface RowMapper<T> {

    /**
  	 * @param ResultSet 结果集
  	 * @param 当前结果集的第几行
  	 * @return 当前行的结果对象，将当前行的结果映射为一个自定义的对象返回
 	 */
    @Nullable
    T mapRow(ResultSet rs, int rowNum) throws SQLException;

}
```
JdbcTemplate内部会遍历ResultSet，然后循环调用`RowMapper#mapRow`，得到当前行的结果，将其丢到List中返回，如下：
```java
List<T> results = new ArrayList<>();
int rowNum = 0;
while (rs.next()) {
    results.add(this.rowMapper.mapRow(rs, rowNum++));
}
return results;
```
<a name="KiEGD"></a>
#### 案例
```java
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@ToString
public class User {
    private Integer id;
    private String name;
}
```
```java
@Test
public void test9() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String sql = "select id,name from t_user where id = ?";
    //查询id为34的用户信息
    User user = jdbcTemplate.queryForObject(sql, new RowMapper<User>() {
        @Nullable
        @Override
        public User mapRow(ResultSet rs, int rowNum) throws SQLException {
            User user = new User();
            user.setId(rs.getInt(1));
            user.setName(rs.getString(1));
            return user;
        }
    }, 134);
    System.out.println(user);
}
```
输出
```
User(id=134, name=134)
```
<a name="VVKyK"></a>
#### 使用注意
当`queryForObject`中sql查询无结果的时候，会报错，必须要返回一行记录
<a name="yGNMk"></a>
### 查询单行记录，返回指定的Java Bean
`RowMapper` 有个实现了类 `BeanPropertyRowMapper`，可以将结果映射为Java Bean。
```java
@Test
public void test10() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String sql = "select id,name from t_user where id = ?";
    //查询id为34的用户信息
    RowMapper<User> rowMapper = new BeanPropertyRowMapper<>(User.class);
    User user = jdbcTemplate.queryForObject(sql, rowMapper, 134);
    System.out.println(user);
}
```
<a name="jXiJs"></a>
### 查询多列多行，每行结果为一个Map
<a name="l9AQA"></a>
#### Api
```java
List<Map<String, Object>> queryForList(String sql);
List<Map<String, Object>> queryForList(String sql, Object... args);
```
每行结果为一个Map，key为列名小写，value为列对应的值。
<a name="A5zcm"></a>
#### 案例
```java
@Test
public void test11() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String sql = "select id,name from t_user where id>?";
    List<Map<String, Object>> maps = jdbcTemplate.queryForList(sql, 130);
    System.out.println(maps);
}
```
输出
```
[{id=131, name=刘德华}, {id=132, name=郭富城}, {id=133, name=张学友}, {id=134, name=黎明}]
```
<a name="E6oj1"></a>
### 查询多列多行，将结果映射为Java Bean
<a name="WPf2P"></a>
#### Api
```java
<T> List<T> query(String sql, RowMapper<T> rowMapper, @Nullable Object... args)
```
<a name="Lxkqb"></a>
#### 案例
```java
@Test
public void test12() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String sql = "select id,name from t_user where id>?";
    List<User> maps = jdbcTemplate.query(sql, new RowMapper<User>() {
        @Nullable
        @Override
        public User mapRow(ResultSet rs, int rowNum) throws SQLException {
            User user = new User();
            user.setId(rs.getInt(1));
            user.setName(rs.getString(1));
            return user;
        }
    }, 130);
    System.out.println(maps);
}
```
运行输出
```
[User(id=131, name=刘德华), User(id=132, name=郭富城), User(id=133, name=张学友), User(id=134, name=黎明)]
```
更简单的方式，使用`BeanPropertyRowMapper`
```java
@Test
public void test13() {
    DataSource dataSource = DataSourceUtils.getDataSource();
    JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
    String sql = "select id,name from t_user where id>?";
    List<User> maps = jdbcTemplate.query(sql, new BeanPropertyRowMapper<User>(User.class), 130);
    System.out.println(maps);
}
```
输出
```
[User(id=131, name=刘德华), User(id=132, name=郭富城), User(id=133, name=张学友), User(id=134, name=黎明)]
```
<a name="H12hY"></a>
## 总结

1. 使用注意：JdbcTemplate中的`getObject`开头的方法，要求sql必须返回一条记录，否则会报错
2. `BeanPropertyRowMapper`可以将行记录映射为Java Bean
3. JdbcTemplate采用模板的方式操作jdbc变的特别的容易，代码特别的简洁，不过其内部没有动态sql的功能，即通过参数，动态生成指定的sql，Mybatis在动态sql方面做的比较好，大家用的时候可以根据需求进行选择。
