Mybatis<br />Mybatis中一个比较强大的功能就是动态sql，使用jdbc开发系统，在java代码中搞了很多判断去拼接sql，代码看起来比较乱，也不方便维护和扩展。<br />Mybatis在处理sql的拼接这块简直是福音，基本上sql拼接的所有的痛点，Mybatis都解决了。下面来学一下Mybatis中各种动态sql的用法。
<a name="vR1XW"></a>
## 案例sql脚本
```sql
DROP DATABASE IF EXISTS `javacode2018`;
CREATE DATABASE `javacode2018`;

USE `javacode2018`;

DROP TABLE IF EXISTS t_user;
CREATE TABLE t_user(
  id int AUTO_INCREMENT PRIMARY KEY COMMENT '用户id',
  name VARCHAR(32) NOT NULL DEFAULT '' COMMENT '用户名',
  age SMALLINT NOT NULL DEFAULT 1 COMMENT '年龄'
) COMMENT '用户表';
INSERT INTO t_user VALUES (1,'Java',30),(2,'张学友',50),(3,'刘德华',50);
```
<a name="mZMGo"></a>
## if元素
相当于java中的if判断，语法：
```xml
<if test="判断条件">
  需要追加的sql
</if>
```
test的值为一个判断表达式，写法上采用OGNL表达式的方式。<br />当test成立的时候，if体内部的sql会被拼接上。<br />如：
```xml
<select id="getList1" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  WHERE 1 = 1
  <if test="id!=null">
    AND id = #{id}
  </if>
  <if test="name!=null and name.toString()!=''">
    AND name = #{name}
  </if>
  <if test="age!=null">
    AND age = #{age}
  </if>
</select>
```
上面查询用户列表，参数为一个map，当map中id不为空的时候，将其作为条件查询，如果name不为空，将name也作为条件，如果age不为空，将age也作为条件进行查询<br />当只传入id的时候，sql如下：
```sql
SELECT id,name,age FROM t_user WHERE 1 = 1 AND id = ?
```
当3个参数都传了，sql如下：
```sql
SELECT id,name,age FROM t_user WHERE 1 = 1 AND id = ? AND name = ? AND age = ?
```
上面这种写法相对于java代码看起来是不是清爽了很多，也更方便维护，大家注意一下sql中有个`WHERE 1=1`，如果没有这个，上面单通过if元素就不好实现了，Mybatis也有解决方案，稍后会说明。
<a name="pAJwV"></a>
## choose/when/otherwise元素
这个相当于java中的if..else if..else，语法：
```xml
<choose>
  <when test="条件1">
    满足条件1追加的sql
  </when>
  <when test="条件2">
    满足条件2追加的sql
  </when>
  <when test="条件n">
    满足条件n追加的sql
  </when>
  <otherwise>
    都不满足追加的sql
  </otherwise>
</choose>
```
choose内部的条件满足一个，choose内部的sql拼接就会结束。<br />otherwise属于可选的，当所有条件都不满足的时候，otherwise将起效。<br />如：<br />传入id、name、age作为条件，按顺序进行判断，如果id不为空，将id作为条件，忽略其他条件，如果id为空，会判断name是否为空，name不为空将name作为条件，如果name为空，再看看age是否为空，如果age不为空，将age作为条件。
```xml
<select id="getList2" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  WHERE 1 = 1
  <choose>
    <when test="id!=null">
      AND id = #{id}
    </when>
    <when test="name!=null and name.toString()!=''">
      AND name = #{name}
    </when>
    <when test="age!=null">
      AND age = #{age}
    </when>
  </choose>
</select>
```
如果id、name、age都传了，sql如下：
```sql
SELECT id,name,age FROM t_user WHERE 1 = 1 AND id = ?
```
如果值传递了name、age，sql如下：
```sql
SELECT id,name,age FROM t_user WHERE 1 = 1 AND name = ?
```
name判断在age前面，所以name条件先匹配上了。
<a name="YXhvm"></a>
## where元素
上面2个案例的sql中都有`where 1=1`这部分代码，虽然可以解决问题，但是看起来不美观，如果将`where 1=1`中`1=1`这部分干掉，上面的两个案例都会出问题，where后面会多一个AND符号，Mybatis中已经考虑到这种问题了，属于通用性的问题，Mybatis中通过where 元素来解决，当使用where元素的时候，Mybatis会将where内部拼接的sql进行处理，会将这部分sql前面的AND 或者 OR给去掉，并在前面追加一个where，使用where元素来对上面的案例1进行改造，如下：
```xml
<select id="getList1" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
  </where>
</select>
```
`where 1=1`被替换成了where 元素。<br />当传入id、name的时候，where内部的sql会变成这样：
```sql
AND id = ? AND name = ?
```
Mybatis会对上面这个sql进行处理，将前面的AND给去掉，并在前面追加一个where，变成了下面这样
```sql
where id = ? AND name = ?
```
案例2也用where改造一下，变成了下面这样：
```xml
<select id="getList2" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <where>
    <choose>
      <when test="id!=null">
        AND id = #{id}
      </when>
      <when test="name!=null and name.toString()!=''">
        AND name = #{name}
      </when>
      <when test="age!=null">
        AND age = #{age}
      </when>
    </choose>
  </where>
</select>
```
这下看起来是不是舒服很多了。
<a name="CPrNN"></a>
## set元素
现在想通过用户id更新用户信息，参数为UserModel对象，对象中的属性如果不为空，就进行更新，可以这么写：
```xml
<update id="update1" parameterType="com.javacode2018.chat05.demo8.model.UserModel">
  UPDATE t_user SET
  <if test="name!=null">
    name = #{name},
  </if>
  <if test="age!=null">
    age = #{age},
  </if>
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
  </where>
</update>
```
来看一下，当所有属性都传值了，sql变成了下面这样：
```sql
UPDATE t_user SET name = ?, age = ?, where id = ?
```
上面这个sql是有问题的，where前面多了一个逗号，得想办法将这个逗号去掉，这个逗号属于最后一个需要更新的字段后面的逗号，属于多余的，Mybatis中提供了set元素来解决这个问题，将上面的代码改成下面这样：
```xml
<update id="update1" parameterType="com.javacode2018.chat05.demo8.model.UserModel">
  UPDATE t_user
  <set>
    <if test="name!=null">
      name = #{name},
    </if>
    <if test="age!=null">
      age = #{age},
    </if>
  </set>
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
  </where>
</update>
```
将sql中的set去掉了，加了个set元素，set元素会对其内部拼接的sql进行处理，**会将这部分sql前后的逗号给去掉并在前面加上set**。<br />当传入id和age的时候，生成的sql：
```sql
UPDATE t_user SET age = ? where id = ?
```
<a name="EaGfO"></a>
## trim元素
这个元素的功能比较强大，先看一下他的语法：
```xml
<trim prefix="" prefixOverrides="" suffix="" suffixOverrides="">
</trim>
```
trim元素内部可以包含各种动态sql，如where、chose、sql等各种元素，使用trim包含的元素，Mybatis处理过程：

1. 先对trim内部的sql进行拼接，比如这部分sql叫做sql1
2. 将sql1字符串前面的部分中包含trim的`prefixOverrides`指定的部分给去掉，得到sql2
3. 将sql2字符串后面的部分中包含trim的`suffixOverrides`指定的部分给去掉，得到sql3
4. 在sql3前面追加trim中prefix指定的值，得到sql4
5. 在sql4后面追加trim中suffix指定的值，得到最终需要拼接的sql5

了解了这个过程之后，说明可以通过trim来代替where和set，使用trim来改造一下案例1，如下：
```xml
<select id="getList1" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <trim prefix="where" prefixOverrides="and|or">
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
  </trim>
</select>
```
**注意上面的**`**prefixOverrides**`**的值的写法，如果有多个需要覆盖的之间用|进行分割，**`**suffixOverrides**`**写法和**`**prefixOverrides**`**的写法类似。**<br />用trim来改造一下上面的update中的，如下：
```xml
<update id="update1" parameterType="com.javacode2018.chat05.demo8.model.UserModel">
  UPDATE t_user
  <trim prefix="SET" prefixOverrides="," suffixOverrides=",">
    <if test="name!=null">
      name = #{name},
    </if>
    <if test="age!=null">
      age = #{age},
    </if>
  </trim>
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
  </where>
</update>
```
上面的`prefixOverrides`和`suffixOverrides`都设置的是逗号，表示trim内部的sql前后的逗号会被去掉，最后会在前面拼接一个prefix指定的set。<br />大家有兴趣的可以去看一下trim的java实现，代码下面这个类中：
```
org.apache.ibatis.scripting.xmltags.TrimSqlNode
```
实际上where和set的实现是继承了`TrimSqlNode`，where对应的java代码：
```java
public class WhereSqlNode extends TrimSqlNode {

    private static List<String> prefixList = Arrays.asList("AND ","OR ","AND\n", "OR\n", "AND\r", "OR\r", "AND\t", "OR\t");

    public WhereSqlNode(Configuration configuration, SqlNode contents) {
        super(configuration, contents, "WHERE", prefixList, null, null);
    }

}
```
set对应的 java代码：
```java
public class SetSqlNode extends TrimSqlNode {

    private static final List<String> COMMA = Collections.singletonList(",");

    public SetSqlNode(Configuration configuration,SqlNode contents) {
        super(configuration, contents, "SET", COMMA, null, COMMA);
    }

}
```
最后都是依靠`TrimSqlNode`来实现的。
<a name="vWjgT"></a>
## foreach元素
相当于Java中的循环，可以用来遍历数组、集合、map等。
<a name="XY1RF"></a>
### 语法
```xml
<foreach collection="需要遍历的集合" item="集合中当前元素" index="" open="" separator="每次遍历的分隔符" close="">
  动态sql部分
</foreach>
```

- collection：可以是一个List、Set、Map或者数组
- item：集合中的当前元素的引用
- index：用来访问当前元素在集合中的位置
- separator：各个元素之间的分隔符
- open和close用来配置最后用什么前缀和后缀将foreach内部所有拼接的sql给包装起来。
<a name="NXQI2"></a>
### 案例：in多值查询
对案例1做个改造，map中支持放入用户的id列表（ArrayList），对应的key为idList，然后支持多个用户id查询，此时需要用in来查询，实现如下：
```xml
<select id="getList1" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
    <if test="idList!=null and idList.size()>=1">
      <foreach collection="idList" item="id" open="AND id in (" separator="," close=")">
        #{id}
      </foreach>
    </if>
  </where>
</select>
```
大家看一下上面idList那部分判断，判断这个参数不为空，并且`size()`大于1，表示这个集合不为空，然后会走if元素内部的foreach元素。<br />比如传递的idList对应的是[1,2]，最后产生的sql如下：
```sql
SELECT id,name,age FROM t_user WHERE id in ( ? , ? )
```
<a name="W2Agq"></a>
### 案例：批量插入
传入UserModel List集合，使用foreach实现批量插入，如下：
```xml
<insert id="insertBatch" parameterType="list">
  INSERT INTO t_user (id,name,age) VALUES
  <foreach collection="collection" separator="," item="item">
    (#{item.id}, #{item.name}, #{item.age})
  </foreach>
</insert>
```
测试用例
```java
@Test
public void insertBatch() throws IOException {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        List<UserModel> userModelList = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            userModelList.add(UserModel.builder().id(10 + i).name("mybatis-" + i).age(20 + i).build());
        }
        int count = mapper.insertBatch(userModelList);
        log.info("{}", count);
    }
}
```
传入了3个用户信息，运行输出
```
39:52.241 [main] DEBUG c.j.c.d.m.UserMapper.insertBatch - ==>  Preparing: INSERT INTO t_user (id,name,age) VALUES (?, ?, ?) , (?, ?, ?) , (?, ?, ?) 
39:52.317 [main] DEBUG c.j.c.d.m.UserMapper.insertBatch - ==> Parameters: 10(Integer), mybatis-0(String), 20(Integer), 11(Integer), mybatis-1(String), 21(Integer), 12(Integer), mybatis-2(String), 22(Integer)
39:52.327 [main] DEBUG c.j.c.d.m.UserMapper.insertBatch - <==    Updates: 3
39:52.327 [main] INFO  c.j.chat05.demo8.Demo8Test - 3
```
<a name="ZUFHJ"></a>
## sql/include元素
这两2个元素一般进行配合使用，可以实现代码重用的效果。<br />sql元素可以用来定义一段动态sql，语法如下：
```xml
<sql id="sql片段id">
  各种动态sql
</sql>
```
其他地方需要使用的时候需要通过include关键字进行引入：
```xml
<include refid="需要引入的sql片段的id"/>
```
**注意：refid值的写法，refid的值为mapper xml的namespace的值.sql的id，如果在同一个mapper中，namespace可以省略，直接写对应的sql的id就可以了，**如：
```xml
<include refid="findSql"/>
<include refid="com.javacode2018.chat05.demo8.mapper.UserMapper.findSql"/>
```
来个案例<br />下面定义2个查询，他们的查询条件一样，最后将条件抽出来用sql元素定义了一个片段，然后进行共用。
```xml
<sql id="findSql">
  <where>
    <if test="id!=null">
      AND id = #{id}
    </if>
    <if test="name!=null and name.toString()!=''">
      AND name = #{name}
    </if>
    <if test="age!=null">
      AND age = #{age}
    </if>
    <if test="idList!=null and idList.size()>=1">
      <foreach collection="idList" item="id" open="AND id in (" separator="," close=")">
        #{id}
      </foreach>
    </if>
  </where>
</sql>

<select id="getList1" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <include refid="com.javacode2018.chat05.demo8.mapper.UserMapper.findSql" />
</select>

<select id="getList1Count" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT count(*) FROM t_user
  <include refid="findSql" />
</select>
```
<a name="uQBDb"></a>
## bind元素
bind元素允许通过ognl表达式在上下文中自定义一个变量，最后在动态sql中可以使用这个变量。
<a name="Eg4CV"></a>
### 语法
```xml
<bind name="变量名称" value="ognl表达式">
```
<a name="uPln8"></a>
### 案例
对sql、include中的案例进行扩展，添加一个按照用户名模糊查询，用户名在map中对应的key为likeName，主要修改上面sql片段部分，在sql中加入下面部分：
```xml
<if test="likeName!=null and likeName.trim()!=''">
  <bind name="nameLike" value="'%'+likeName.trim()+'%'" />
  AND name like #{nameLike}
</if>
```
先判断传入的参数likeName是否不为空字符串，然后使用bind元素创建了一个变量nameLike，值为'`%'+likeName.trim()+'%'`。<br />对应的测试用例：
```java
@Test
public void getModelList() throws IOException {
    try (SqlSession sqlSession = this.sqlSessionFactory.openSession(true);) {
        UserMapper mapper = sqlSession.getMapper(UserMapper.class);
        Map<String, Object> paramMap = new HashMap<>();
        paramMap.put("likeName","java");
        List<UserModel> userModelList = mapper.getList1(paramMap);
        log.info("{}", userModelList);
    }
}
```
运行输出：
```
06:25.633 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user WHERE name like ? 
06:25.671 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: %java%(String)
06:25.690 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 1
06:25.691 [main] INFO  c.j.chat05.demo8.Demo8Test - [UserModel(id=1, name=Java, age=31)]
```
注意输出中的第二部分，参数的值为`%java%`。
<a name="JfiTT"></a>
## #和$
#和$一般都是结合变量来使用，如：`#{}`、`${}`这种来进行使用。<br />`**#{}**`**：为参数占位符？，即sql预编译，相当于使用jdbc中的**`**PreparedStatement**`**中的sql占位符，可以防止sql注入**<br />`**${}**`**：为字符串替换， 即字符串拼接，不能防止sql注入。**<br />#{}的用法上面已经有很多案例了，此处来一个`${}`的案例。<br />下面通过orderSql变量传入任意的排序sql，如下：
```xml
<select id="getList1" resultType="com.javacode2018.chat05.demo8.model.UserModel" parameterType="map">
  SELECT id,name,age FROM t_user
  <if test="orderSql">
    ${orderSql}
  </if>
</select>
```
传入值：
```
orderSql = "order by id asc,age desc"
```
最后运行产生的sql如下：
```
20:32.138 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==>  Preparing: SELECT id,name,age FROM t_user order by id asc,age desc 
20:32.173 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - ==> Parameters: 
20:32.196 [main] DEBUG c.j.c.d.mapper.UserMapper.getList1 - <==      Total: 6
20:32.197 [main] INFO  c.j.chat05.demo8.Demo8Test - [UserModel(id=1, name=Java, age=31), UserModel(id=2, name=张学友, age=50), UserModel(id=3, name=刘德华, age=50), UserModel(id=10, name=mybatis-0, age=20), UserModel(id=11, name=mybatis-1, age=21), UserModel(id=12, name=mybatis-2, age=22)]
```
Mybatis会对$包含部分进行sql替换。
