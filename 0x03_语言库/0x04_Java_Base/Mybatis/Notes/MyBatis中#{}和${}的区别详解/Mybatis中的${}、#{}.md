Mybatis<br />Mybatis作为一个轻量级的ORM框架，应用广泛，其上手使用也比较简单；一个成熟的框架，必然有精巧的设计，值得学习。<br />在使用Mybatis框架时，在sql语句中获取传入的参数有如下两种方式：

- `${paramName}`
- `#{paramName}`

那如何理解这两种传参方式呢？<br />先来回顾下原生Jdbc查询：
```java
public static void main(String[] args) throws Exception {

    // sql语句
    String sql = "select id,name from customer limit 2";
    // 1.加载驱动, 此处使用的mysql驱动包是8.0版本, 若为5.0+版本, 请修改以下类路径
    Class.forName("com.mysql.cj.jdbc.Driver");
    // 2.获取数据库连接
    String url = "jdbc:mysql://localhost:3306/work?useSSL=false&useUnicode=true" +
        "&characterEncoding=UTF-8&useJDBCCompliantTimezoneShift=true" +
        "&useLegacyDatetimeCode=false&serverTimezone=UTC";
    Connection conn = DriverManager.getConnection(url,"root", "123456");
    // 3、获得可以执行sql语句的对象
    Statement st = conn.createStatement();
    // 4、使用对象去执行SQL语句
    ResultSet rs = st.executeQuery(sql);
    // 5、处理sql语句返回的结果集
    while(rs.next()){
        // 获得一行数据
        Integer id = rs.getInt("id");
        String name = rs.getString("name");
        System.out.println("sql查询: id = " + id + " , name = " + name);
    }
    // 6、释放资源
    rs.close();
    st.close();
    conn.close();
}
```
控制台打印：
```
sql查询: id = 1 , name = 李白
sql查询: id = 2 , name = 杜甫
```
了解Jdbc的人会知道，其中第3、4步两条语句也可以换成如下两条：
```java
// 3.创建 PreparedStatement 对象去执行sql
PreparedStatement preparedStatement = conn.prepareStatement(sql);
// 4.执行sql语句
ResultSet rs = preparedStatement.executeQuery();
```
来比较下区别：

- 创建 `PreparedStatement` 对象时就把sql语句传入，在执行语句时就不用传入sql了；而 `Statement` 则刚好相反

这就引出了预编译的概念：

- 如果使用`PreparedStatement`对象，那么在执行第3步时，既然已经传入了sql，则相当于这条sql会被数据库编译（数据库对sql语句的编译也是相当复杂的），所以在第4步执行的时候就不用再传入sql了，因为数据库已经知道要执行的sql了，只需要传入参数即可；
- 如果使用`Statement`对象，那容易理解，数据库就没有提前去解析sql，因为创建对象时都没有传入；当执行sql时，数据库再编译与执行。

看到这里，可能也仅仅只记住了一个预先编译sql了，一个没有预先编译，并没有了解到对于实际开发中的区别，以下将会举例说明。<br />那是否`PreparedStatement`对象这种方式就一定比`Statement`对象方式好？<br />没有那么绝对的事，大家要理解：<br />`PreparedStatement`对象的好处是，sql已经提前编译好，剩下的工作就是传入参数即可，编译好的sql可以复用，传入不同的参数，则数据库就将相应的参数填入编译好的sql。而`Statement`对象就是每次都要传入sql，丢给数据库去编译再执行；但是创建`PreparedStatement`对象的开销是比`Statement`对象大的。<br />回归到日常开发中，以上的区别压根也不用在意，事实上，百分之九十的场景使用的是`PreparedStatement`对象的方式，可能平时没有感知到，因为这是框架已经封装了。再者，当系统出现性能问题时，也绝对不会是因为这两个对象的原因。<br />以上简单回顾了下Jdbc中`PreparedStatement`与`Statement`对象；<br />可以预料，Mybatis中`${}` 与 `#{}` 这两种取值方式就是相当于对应着`PreparedStatement`和`Statement`对象的区别了。

- `#{}` 传参，代表sql已经预编译好了，传入的参数真的就仅仅是参数！
- `${}` 传参，随便传，传完了之后再统一编译

那具体在使用中有什么不同呢？理解如下两种场景：

1. 看如下service和sql语句
```java
@Override
public List<Map<String, Object>> listUser() {
    String param = " and name = '李白'";
    return indexMapper.listUser(param);
}
```
```xml
<select id="listUser" resultType="map">
  select * from customer
  where 1 = 1 #{param}
</select>
```
以上代码能正常查询吗？
```
### Error querying database.  Cause: java.sql.SQLSyntaxErrorException: You have 
  an error in your SQL syntax; check the manual that corresponds to your MySQL 
    server version for the right syntax to use near '' and name = \'李白\''
```
不能！会报sql语句规则错误，之前说了，`#{}` 取值代表sql已经编译好了，传入的仅仅是参数<br />对应上面示例：<br />sql是指：`select * from customer where 1 = 1`<br />参数是指：`and name = '李白'`<br />此时sql可以正确运行，但是带上传入的参数就不行了，要理解传入的真的仅仅是参数，不要和前面的sql混了。<br />但是这明显不对，因为想表达的其实是这样：<br />sql是指：
```sql
select * from customer where 1 = 1 and name = ?
```
参数是指：'李白'<br />此时把参数替换进占位符是可以正常运行整个语句的。<br />所以此时应该用 {}就没有提前编译好哪些是属于sql。<br />此示例表明：当传入的参数不仅仅是参数，其实是一小段sql，想和原sql拼接在一起时，那就得用${}传参，相当于拼接好了之后丢给<br />数据库去解析整个语句；<br />sql中的问号代表参数占位符，这也是`PreparedStatement`对象特点之一，会将传入的参数一一替换进占位符<br />反之如下：
```java
@Override
public List<Map<String, Object>> listUser() {
    String param = "李白";
    return indexMapper.listUser(param);
}
```
```xml
<select id="listUser" resultType="map">
  select * from customer
  where 1 = 1 and name = #{param}
</select>
```
这种情况使用 `#{}` 就是对的了，因为传入的参数仅仅就是参数，替换进sql语句中即可。

2. 对参数类型的影响
```java
@Override
public List<Map<String, Object>> listUser() {
    String param = "李白";
    return indexMapper.listUser(param);
}
```
```xml
<select id="listUser" resultType="map">
  select * from customer
  where 1 = 1 and name = ${param}
</select>
```
以上代码能执行成功吗？<br />按理说，传入的仅仅是参数，不管是否预编译都应该能执行，但是实际还是会报错。<br />这是执行时打印出的sql语句：
```sql
select * from customer where 1 = 1 and name = 李白
```
显然，问题就在于参数没有加单引号，name字段是字符串类型，传入的也是字符串，偏偏Mybatis转换之后没有加单引号。<br />所以当传入字符串类型参数时，应该用 `#{}` 取值，此时会自动加上单引号。<br />再看下面这种语句：
```java
@Override
public List<Map<String, Object>> listUser() {
    String param = "name";
    return indexMapper.listUser(param);
}
```
```xml
<select id="listUser" resultType="map">
  select * from customer
  where 1 = 1
  order by ${param} desc
</select>
```
此时传入的参数是要排序的字段名称，之前说了，如果采用`#{}` 取值，则实际是会自动加上单引号的，但是`order by`后面的排序字段需要单引号吗？<br />不需要，所以这种情况只能使用 `${}` 取值。<br />可能会发现此处用 `#{}` 取值也不会报错，那是因为MySQL支持这种写法，但是查询的结果并不对。<br />日常开发中，只要能理解上述两种情形，那么就能正确使用 `${}` 和 `#{}`，由于这两种方式取值原理的区别，也容易明白 `#{}` 这种方式是可以防止sql注入的。
