Java Mybatis
<a name="sLhlO"></a>
### 区别

1. `#`将传入的数据都当成一个字符串，会对自动传入的数据加一个双引号。如：`order by #user_id#`，如果传入的值是111,那么解析成sql时的值为`order by "111"`, 如果传入的值是id，则解析成的sql为`order by "id"`。
2. 将传入的数据直接显示生成在sql中。如：orderby将传入的数据直接显示生成在sql中。如：`orderbyuser_id$`，如果传入的值是111,那么解析成sql时的值为`order by user_id`, 如果传入的值是id，则解析成的sql为`order by id`.
3. `#`方式能够很大程度防止sql注入。
4. `$`方式无法防止SQL注入。
5. `$`方式一般用于传入数据库对象，例如传入表名.
6. 一般能用`#`的就别用`$`。
:::tips
MyBatis排序时使用`order by` 动态参数时需要注意，用`$`而不是`#`
:::
<a name="FtjiN"></a>
### 实例讲解
动态 sql 是 Mybatis的主要特性之一，在 mapper 中定义的参数传到 xml 中之后，在查询之前 Mybatis会对其进行动态解析。Mybatis提供了两种支持动态 sql 的语法：`#{}` 以及 `${}`。<br />在下面的语句中，如果 name 的值为 zhangsan，则两种方式无任何区别：
```sql
select * from user where name = #{name};select * from user where name = ${name};
```
其解析之后的结果均为
```sql
select * from user where name = 'zhangsan';
```
但是 `#{}` 和 `${}` 在预编译中的处理是不一样的。<br />`#{}` 在预处理时，会把参数部分用一个占位符 `?` 代替，变成如下的 sql 语句：
```sql
select * from user where name = ?;
```
而 `${}` 则只是简单的字符串替换，在动态解析阶段，该 sql 语句会被解析成
```sql
select * from user where name = 'zhangsan';
```
以上，`#{}` 的参数替换是发生在 DBMS 中，而 `${}` 则发生在动态解析过程中。<br />那么，在使用过程中应该使用哪种方式呢？<br />答案是：优先使用 `#{}`。因为 `${}` 会导致 sql 注入的问题。<br />看下面的例子：
```sql
select * from ${tableName} where name = #{name}
```
在这个例子中，如果表名为
```sql
user; delete user; --
```
则动态解析之后 sql 如下：
```sql
select * from user; delete user; -- where name = ?;
```
`--`之后的语句被注释掉，而原本查询用户的语句变成了查询所有用户信息+删除用户表的语句，会对数据库造成重大损伤，极大可能导致服务器宕机。<br />但是表名用参数传递进来的时候，只能使用 `${}` 。这也提醒在这种用法中要小心sql注入的问题。
