Java JDBC <br />数据库连接的时候创建一个对象，然后通过对象去调用`executeQuery`方法来执行sql语句
<a name="TZmVL"></a>
### Statement
```java
String sql = "select * from users";
Statement stmt = db.conn.createStatement();
ResultSet rs = stmt.executeQuery(sql);
```
<a name="oaVAJ"></a>
### PrepareStatement
```java
String sql = "select * from users where  username=? and userpwd=?";
PreparedStatement preStmt = db.conn.prepareStatement(sql);
pstmt.setString(1, username);
pstmt.setString(2, userpwd);
preStmt.executeUpdate();
preStmt.close();
```
`PrepareStatement`继承自`Statement`。
<a name="r7g8E"></a>
### 可读性
`PrepareStatement`可以将SQL语句中的变量抽象出来，这样可以提高代码的可读性 ；
<a name="LvVJd"></a>
### 效率
`Statement` 执行每一条SQL语句时，都是编译+执行，相当于静态SQL；<br />`PrepareStatement`执行的SQL语句，会进行预编译，SQL中可以包含动态参数"?"，在执行时可以为"?"动态设置参数值，当下次执行相同类型的SQL语句时，可以解析并直接执行编译好的SQL语句，从而减少编译次数提高数据库的性能，相当于动态SQL；另一方面，他还有Batch功能可以进行批量请求，从而减少了JDBC驱动和数据库的连接请求次数，eg:
```java
PreparedStatement pstmt = conn.prepareStatement("INSERT into users values (?, ?, ?)");  
for (i = 0; i < list.length(); i++) {  
    pstmt.setLong(list[i].id);
    pstmt.setString(list[i].name);  
    pstmt.setString(list[i].phone);
    pstmt.addBatch();  
}  
pstmt.executeBatch();
```
~`Statement`每次执行sql语句，数据库都要执行sql语句的编译 ，最好用于仅执行一次查询并返回结果的情形时，效率高于`PreparedStatement`。 
<a name="DFGrl"></a>
### 安全性
`Statement`：SQL语句每次都是重新编译执行的，这就导致你传进去的参数和SQL语句是有联系，可以改变原先SQL的功能结构
```sql
select * from user where username= '"+name+"' and userpwd='"+passwd+"'
```
如果传进去的passwd参数值是`or '1' = '1'`那SQL就变成了永真式：
```sql
select * from user where username= '"+name+"' and userpwd='' or '1' = '1'
```
权限就这样被轻易获取了； <br />`PrepareStatement`：SQL的参数会被强制类型转换成表中的列属性值，由于SQL语句已经实现被编译过，参数和原来SQL语句不会有交集，也就避免了这种情况的发生。
