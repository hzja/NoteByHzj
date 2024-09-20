JavaMybatis<br />刚入行的同学，看到在 SQL 语句中出现 `where 1 = 1` 这样的条件可能会有所困惑，而长时间这样使用的朋友可能又习以为常。<br />那么，是否还记得当初为什么要这样写？是否有性能问题？有没有更好的写法？<br />来从头到尾梳理一下 `where 1 = 1` 的使用及改进，或许能从中得到更多意想不到的收获。
<a name="pOqWy"></a>
## `where 1=1` 的作用
如果要问在 SQL 语句的 where 条件中多加 1=1 目的是什么，很简单：**使得 where 条件语句永远为真。** 本质上就是虽然加了 where 条件，但实际上永远为真，也就相当于没有加任何约束条件。<br />使用该语句的场景主要是：**动态构建 SQL 语句。**
```java
String sql  =  "select * from t_user  where 1=1 "; 
if(!b.equals("")){
    sql += "and  b='"+b+"'";
}
```
在上述语句拼写时，使用 `where 1=1`，当 b 不等于空时，可以直接拼接 “and” 语句，而不会导致语法错误。如果没有 `where 1=1`，原来的 SQL 语句就变成（假设 b 传入了 "abc"）：
```sql
select * from t_user  where and  b= 'abc'; 
```
很明显，上述 SQL 语句会有语法错误。所以，之所以添加 1=1，就是为了方便 SQL 拼接。<br />从另外一个角度来讲，不仅仅 1=1 可以这样使用，像：1<>2、2>1、'a'='a' 等表达式，只要结果为 true，都可以如此使用。
<a name="YBupu"></a>
## `where 1<>1`
上面讲了 `where 1=1` 的来历及使用，那么是否使用过 `where 1<>1` 的形式呢？<br />`where 1<>1` 也是有使用场景的，比如：只获取表结构而不取数据。
```sql
create table t_temp as select * from t_user  where 1<>1
```
上述语句，创建了一个与 t_user 表结构一样但没有任何数据的新表 t_temp。<br />当然，除了表结构，其他的结构也可以如此使用。
<a name="ErzwL"></a>
## 性能问题
有人说，使用 where 1=1 可能会有性能问题，直接来实验一下。<br />MySQL 8.0.18，t_user 表，id_no 字段有索引：
```sql
explain select * from t_user where id_no = 'Tom25';
explain select * from t_user where 1=1 and id_no = 'Tom25';
```
执行上述两行 SQL 语句，explain 结果都是如下：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652746891368-6e0a11ce-d0cf-4d9a-a2e6-331b54576541.jpeg#clientId=uc9515b15-e079-4&from=paste&id=ub67a97d5&originHeight=133&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5f71a217-0def-4fd1-8d0b-a2f47e3bdc1&title=)<br />也就是说，1=1 这样条件，并不影响索引和性能，从 explain 结果上可以看出两者并无本质区别。<br />之所以不同的 SQL 语句，呈现了相同的结果，这是因为被 Mysql 优化了。Mysql 在处理指令时，会对 1=1 这类无效的条件进行优化处理。这个与 Java 的编译器有些像，很多无效的判断或语句，在编译成字节码时，编译器会进行优化处理。
<a name="tOd1Z"></a>
## 改进
虽然说 1=1 会被优化器优化掉，但优化操作本身还是会消耗 MySQL 的性能的，如果能够从根本上避免这种情况的出现，那不就更好了。<br />以 Mybatis 为例，在使用 `where 1=1` 时，通常会是如下写法：
```xml
<select id="queryUser" parameterType="com.choupangxia.entity.User" resultType="java.lang.Integer">
  select count(id) from t_user u where 1=1
<if test="username !=null and username !='' ">
  AND u.username = #{username} 
</if> 
<if test="userNo !=null and userNo !='' ">
  AND u.user_no = #{userNo}
</if> 
</select>
```
这里 `where 1=1` 的作用同上。但如果更进一步去了解 Mybatis 的语法及标签，可以使用标签来代替 `where 1=1`：
```xml
<select id="queryUser" parameterType="com.choupangxia.entity.User" resultType="java.lang.Integer">
  select count(id) from t_user u
  <where>
    <if test="username !=null and username !='' ">
      u.username = #{username} 
    </if>
    <if test="userNo !=null and userNo !='' "> 
      AND u.user_no = #{userNo}
    </if>
  </where> 
</select>
```
这样，在查询数据比较大的情况下，可减少 MySQL 为了优化 1=1 这样的条件而损失的性能。
