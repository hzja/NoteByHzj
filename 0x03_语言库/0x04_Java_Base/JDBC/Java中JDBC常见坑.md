Java JDBC
<a name="Ylxo5"></a>
## `Connection.setAutoCommit(boolean)`
用于事务提交。<br />`setAutoCommit(true)`，则执行的所有sql执行都会作为单个事务直接提交并运行<br />`setAutoCommit(false)`，则必须等调用`conn.commit()`才会提交运行
<a name="lueaI"></a>
### Q：`setAutoCommit`默认是`true`还是`false`
A：默认是`true`。
<a name="ZlJhO"></a>
### Q：`setAutoCommit(true)`的缺点是什么？
A：如果一次性执行多个sql语句，中间sql出错时，就会造成脏数据。
<a name="hRS9y"></a>
### Q：`setAutoCommit(false)`后，如果出了错却没有在`catch`中进行`Connection`的`rollBack`操作，会发生什么？
A：操作的表就会被锁住，造成数据库死锁
<a name="fvMIk"></a>
## `fetchSize`
`fetchSize`是设定JDBC的`Statement`读取数据的时候每次从数据库中取出的记录条数

- fetchSize越大，客户端内存占用越大，读取数据库次数越少，速度越快。
<a name="XPKPV"></a>
### Q：Oracle和Mysql中的`fetchSize`有什么区别？
A：Oracle会每次网络传输`fetchSize`条数据到客户端，MYSQL则会一次性全部传送到客户端，因此Mysql中的`fetchSize`是一种模拟游标。
<a name="gjKix"></a>
## `PreparedStatement`
<a name="DiQCI"></a>
### Q：相比`Statement`的好处？
A：

1. `PreparedStatement`是预编译的，比`Statement`速度快，执行效率高，因此即使sql中不带参数也最好使用`PreparedStatement`
2. 代码的可读性和可维护性更好(相比于sql拼接）
3. `PreparedStatement`可以防止SQL注入攻击，而`Statement`却不能

---

<a name="EL14k"></a>
### Q：`prepareStatement`是`statement`接口的实现吗？
A：<br />`prepareStatement`不是实现，而是继承的接口<br />![2021-06-20-11-29-26-103562.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624159805369-05f54656-623e-41cc-b50b-b07bc94fe627.png#averageHue=%2330302f&clientId=ue3b16981-b294-4&from=ui&id=uda51126a&originHeight=466&originWidth=760&originalType=binary&ratio=3&rotation=0&showTitle=false&size=1064738&status=done&style=none&taskId=u3a79d2be-764a-469e-8035-4751f238d30&title=)
<a name="Het08"></a>
## `CallableStatement`

- `CallableStatement`继承自`PreparedStatement`
- `CallableStatement`接口添加了调用存储过程核函数以及处理输出参数(`INOUT`)的方法。
- 即存储过程就用`CallableStatement`
<a name="dVqQr"></a>
## Connection Pool
连接池优点：

1. 减少连接创建次数
2. 更快的系统整体响应速度
3. 统一连接管理，减少失误性的连接未关闭。
<a name="dClwW"></a>
## ResultSet
作用：缓存数据结果集
```java
Statement st = conn. createStatement (int resultSetType, int resultSetConcurrency)
ResultSet rs = st.executeQuery(sqlStr)
```
滚动，就是指调用`.next()`或者`.previous()`或者移动到对应行<br />`resultSetType`是设置`ResultSet`对象的类型可滚动，或者是不可滚动。取值如下（见单词知意）：

- `ResultSet.TYPE_FORWARD_ONLY`只能向前滚动
- `ResultSet.TYPE_SCROLL_INSENSITIVE`，支持前后滚动，对修改不敏感
- `ResultSet.TYPE_SCROLL_SENSITIVE`支持前后滚动，对修改敏感

`resultSetConcurency`是设置 ResultSet 对象能够修改的，取值如下：

- `ResultSet.CONCUR_READ_ONLY`设置为只读类型的参数。
- `ResultSet.CONCUR_UPDATABLE`设置为可修改类型的参数。

---

<a name="skIVH"></a>
### Q：`Connection`、`statement`、`ResultSet`的关闭顺序是？
A：<br />先`ResultSet`、再`Statement`、最后再`connection`。<br />因为这种操作很麻烦，最好使用jdbc连接池，或者`try-with-resource`
<a name="OABaD"></a>
## 数据库知识
<a name="IGIaS"></a>
### Q：事务的ACID属性是什么？
A：

- A是`atomicity`原子性，事务内的行为一次性执行完，要么就回退
- C是`consistency`一致性有a+b=c的限制条件，然后a变化的同时，b也必须跟着变化
- I是`isolation`隔离性事务隔离，即事务的中间执行过程，对另外一个事务不可见。
- D是`durability`持久性提交i成功后，修改不会改变，也会被记录。

---

<a name="hgqRv"></a>
### Q：脏读、不可重复读和幻读是什么？
A：

- 脏读：数据被更新了，但是还没提交，然后另一个事务读到了更新后的数据，结果事务回滚了，导致读的数据其实是脏数据，
- 不可重复读：1个事务要读2次数据（注意是单条数据），结果第一次读和第二次读数据不一致了。
- 幻读：1个事务读了2次数据，发现2次的记录数不一致（注意事项记录数）
