JavaMybatis
<a name="F9QBN"></a>
## 前言
大家都知道，Mybatis内置参数，形如`#{xxx}`的，均采用了sql预编译的形式，大致知道Mybatis底层使用`PreparedStatement`，过程是先将带有占位符（即”?”）的sql模板发送至MySQL服务器，由服务器对此无参数的sql进行编译后，将编译结果缓存，然后直接执行带有真实参数的sql。如果基本结论也是如此，那就大错特错了。
<a name="i31L9"></a>
## 目录

1. MySQL是否默认开启了预编译功能？
2. 预编译缓存是服务端还是客户端缓存？
3. 开启预编译性能更高？
4. 从源码中验证
5. 总结
<a name="kKt6e"></a>
## 1、MySQL是否默认开启了预编译功能？
MySQL是否支持预编译有两层意思：

- db是否支持预编译
- 连接数据库的url是否指定了需要预编译，比如：`jdbc:mysql://127.0.0.1:3306/user?useServerPrepStmts=true`，`useServerPrepStmts=true`是非常非常重要的参数。如果不配置`PreparedStatement` 实际是个假的 `PreparedStatement`
```sql
SELECT VERSION();   // 5.6.24-log

SHOW GLOBAL STATUS LIKE '%prepare%'; //Com_stmt_prepare 4  代表被执行预编译次数

//开启server日志
SHOW VARIABLES LIKE '%general_log%';
SHOW VARIABLES LIKE 'log_output';

SET GLOBAL general_log = ON;
SET GLOBAL log_output='table';

TRUNCATE TABLE mysql.general_log;    
SELECT * FROM mysql.general_log;  // 有Prepare命令
```
注意：MySQL预编译功能有版本要求，包括server版本和mysql.jar包版本。以前的版本默认`useServerPrepStmts=true`，5.0.5以后的版本默认`useServerPrepStmts=false`
<a name="UlCm7"></a>
## 2、预编译缓存是服务端还是客户端缓存？
开启缓存：`useServerPrepStmts=true&cachePrepStmts=true`，设置了`useServerPrepStmts=true`，虽然可以一次编译，多次执行它可以提高性能，但缓存是针对连接的，即每个连接的缓存都是独立的，并且缓存主要是由mysql-connector-java.jar实现的。<br />当手动调用`prepareStatement.close()`时`PrepareStatement`对象只会将关闭状态置为关闭，并不会向MySQL发送关闭请求，`prepareStatement`对象会被缓存起来，等下次使用的时候直接从缓存中取出来使用。没有开启缓存，则会向MySQL发送`closeStmt`的请求。
<a name="kfjJu"></a>
## 3、开启预编译性能更高？
也就是说预编译比非预编译更好？其实不然，不行自己可试试看。
```java
public class PreparedStatement_test {
    private String url = "jdbc:mysql://localhost:3306/batch";
    private String sql = "SELECT * FROM export_request WHERE id = ?";
    private int maxTimes = 100000;
 
    @Test
    public void go_driver() throws SQLException, ClassNotFoundException {
        Class.forName("com.mysql.jdbc.Driver");
        Connection conn = (Connection) DriverManager.getConnection(url, "root", "123456");
        // PreparedStatement
        Stopwatch stopwatch = Stopwatch.createStarted();
        for (int i = 0; i < maxTimes; i++) {
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setLong(1, Math.abs(new Random().nextLong()));
            // execute
            stmt.executeQuery();
        }
        System.out.println("go_driver:" + stopwatch);
    }
 
    @Test
    public void go_setPre() throws SQLException, ClassNotFoundException {
        Class.forName("com.mysql.jdbc.Driver");
        Connection conn = (Connection) DriverManager.getConnection(url + "?useServerPrepStmts=true", "root", "123456");
        // PreparedStatement
        Stopwatch stopwatch = Stopwatch.createStarted();
        for (int i = 0; i < maxTimes; i++) {
            PreparedStatement stmt = conn.prepareStatement(sql);
            stmt.setLong(1, Math.abs(new Random().nextLong()));
            // execute
            stmt.executeQuery();
        }
        System.out.println("go_setPre:" + stopwatch);
    }
 
    @Test
    public void go_setPreCache() throws SQLException, ClassNotFoundException {
        Class.forName("com.mysql.jdbc.Driver");
        Connection conn = (Connection) DriverManager.getConnection(url + "?useServerPrepStmts=true&cachePrepStmts=true", "root", "123456");
        // PreparedStatement
        PreparedStatement stmt = conn.prepareStatement(sql);
        stmt.setLong(1, Math.abs(new Random().nextLong()));
        // execute
        stmt.executeQuery();
        stmt.close();//非常重要的，一定要调用才会缓存
        Stopwatch stopwatch = Stopwatch.createStarted();
        for (int i = 0; i < maxTimes; i++) {
            stmt = conn.prepareStatement(sql);
            stmt.setLong(1, Math.abs(new Random().nextLong()));
            // execute
            stmt.executeQuery();
        }
        System.out.println("go_setPreCache:" + stopwatch);
    }
}
```
基准为10w次单线程：

- 非预编译：:23.78 s
- 预编译：41.86 s
- 预编译缓存：20.55 s

经过实践测试，对于频繁适用的语句，使用预编译+缓存确实能够得到可观的提升，但对于不频繁适用的语句，服务端编译会增加额外的round-trip。开发实践中要视情况而定。
<a name="n7k3z"></a>
## 4、从源码中验证
预编译原理（connection -> prepareStatement ）<br />预编译：`JDBC42ServerPreparedStatement`（需将对应占位符）<br />非预编译：`JDBC42PreparedStatement`（完整的SQL）
```java
//com.mysql.jdbc.ConnectionImpl中的代码片段
/**
 * JDBC 2.0 Same as prepareStatement() above, but allows the default result
 * set type and result set concurrency type to be overridden.
 *
 * @param sql
 *            the SQL query containing place holders
 * @param resultSetType
 *            a result set type, see ResultSet.TYPE_XXX
 * @param resultSetConcurrency
 *            a concurrency type, see ResultSet.CONCUR_XXX
 * @return a new PreparedStatement object containing the pre-compiled SQL
 *         statement
 * @exception SQLException
 *                if a database-access error occurs.
 */
public java.sql.PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency) throws SQLException {
	synchronized (getConnectionMutex()) {
		checkClosed();
		//
		// FIXME: Create warnings if can't create results of the given type or concurrency
		//当Client开启 useServerPreparedStmts 并且Server支持 ServerPrepare
		PreparedStatement pStmt = null;
		boolean canServerPrepare = true;
		String nativeSql = getProcessEscapeCodesForPrepStmts() ? nativeSQL(sql) : sql;
		if (this.useServerPreparedStmts && getEmulateUnsupportedPstmts()) {
			canServerPrepare = canHandleAsServerPreparedStatement(nativeSql);
		}

		if (this.useServerPreparedStmts && canServerPrepare) {// 从缓存中获取 pStmt
			if (this.getCachePreparedStatements()) {
				synchronized (this.serverSideStatementCache) {
					pStmt = (com.mysql.jdbc.ServerPreparedStatement) this.serverSideStatementCache
						.remove(makePreparedStatementCacheKey(this.database, sql));

					if (pStmt != null) {
						((com.mysql.jdbc.ServerPreparedStatement) pStmt).setClosed(false);
						pStmt.clearParameters();// 清理上次留下的参数
					}

					if (pStmt == null) {
						try {// 向Server提交 SQL 预编译，实例是JDBC42ServerPreparedStatement
							pStmt = ServerPreparedStatement.getInstance(getMultiHostSafeProxy(), nativeSql, this.database, resultSetType,
																		resultSetConcurrency);
							if (sql.length() < getPreparedStatementCacheSqlLimit()) {
								((com.mysql.jdbc.ServerPreparedStatement) pStmt).isCached = true;
							}

							pStmt.setResultSetType(resultSetType);
							pStmt.setResultSetConcurrency(resultSetConcurrency);
						} catch (SQLException sqlEx) {
							// Punt, if necessary
							if (getEmulateUnsupportedPstmts()) {
								pStmt = (PreparedStatement) clientPrepareStatement(nativeSql, resultSetType, resultSetConcurrency, false);

								if (sql.length() < getPreparedStatementCacheSqlLimit()) {
									this.serverSideStatementCheckCache.put(sql, Boolean.FALSE);
								}
							} else {
								throw sqlEx;
							}
						}
					}
				}
			} else {
				try {    // 向Server提交 SQL 预编译。
					pStmt = ServerPreparedStatement.getInstance(getMultiHostSafeProxy(), nativeSql, this.database, resultSetType, resultSetConcurrency);

					pStmt.setResultSetType(resultSetType);
					pStmt.setResultSetConcurrency(resultSetConcurrency);
				} catch (SQLException sqlEx) {
					// Punt, if necessary
					if (getEmulateUnsupportedPstmts()) {
						pStmt = (PreparedStatement) clientPrepareStatement(nativeSql, resultSetType, resultSetConcurrency, false);
					} else {
						throw sqlEx;
					}
				}
			}
		} else {// Server不支持 ServerPrepare，实例是JDBC42PreparedStatement
			pStmt = (PreparedStatement) clientPrepareStatement(nativeSql, resultSetType, resultSetConcurrency, false);
		}
		return pStmt;
	}
}
```
JDBC42ServerPreparedStatement->close，缓存
```java
//com.mysql.jdbc.ServerPreparedStatement中选取代码
@Override
public void close() throws SQLException {
	MySQLConnection locallyScopedConn = this.connection;

	if (locallyScopedConn == null) {
		return; // already closed
	}

	synchronized (locallyScopedConn.getConnectionMutex()) {
		if (this.isCached && isPoolable() && !this.isClosed) {
			clearParameters();// 若开启缓存，则只会将状态位设为已关闭，并且刷新缓存
			this.isClosed = true;
			this.connection.recachePreparedStatement(this);
			return;
		}
		//没有开启缓存，则会向mysql发送closeStmt的请求
		realClose(true, true);
	}
}
public void recachePreparedStatement(ServerPreparedStatement pstmt) throws SQLException {
	synchronized (getConnectionMutex()) {
		if (getCachePreparedStatements() && pstmt.isPoolable()) {
			synchronized (this.serverSideStatementCache) {
				Object oldServerPrepStmt = this.serverSideStatementCache.put(makePreparedStatementCacheKey(pstmt.currentCatalog, pstmt.originalSql), pstmt);
				if (oldServerPrepStmt != null) {// 将sql语句作为key，reparedStatement对象作为value存放到缓存中
					((ServerPreparedStatement) oldServerPrepStmt).isCached = false;
					((ServerPreparedStatement) oldServerPrepStmt).realClose(true, true);
				}
			}
		}
	}
}
```
<a name="BeRDB"></a>
## 5、总结

- 预编译显式开启（在url中指定`useServerPrepStmts=true`），否则`PreparedStatement`不会向MySQL发送预编译（Prepare命令）的请求；
- 每次向MySQL发送预编译请求，不管之前有没有执行过此SQL语句，只要请求的命令是Prepare或Query，MySQL就会重新编译一次SQL语句，并返回此链接当前唯一的Statement ID，后续执行SQL语句的时候，程序只需拿着Statement ID和参数就可以了；
- 当预编译的SQL语句有语法错误，则MySQL的响应会携带错误信息，但此错误信息JDBC感知不到(或者说mysql-connetor-java.jar包里的实现将其忽略掉了)，此时还会继续往下执行代码，当执行到`executeXxx()`方法时，由于没有Statement ID（所以就会将拼接完整的SQL语句值已经将占位符(?)替换掉再次发给MySQL请求执行，此时MySQL响应有语法错误，这时JDBC就会抛出语法错误异常），所以检查语法那一步实在mysql-server中做的(通过抓包可以看到)；
- `PreparedStatement`对性能的提高是利用缓存实现的，需要显式开启（在url中指定`cachePrepStmts=true`），此缓存是mysql-connetor-java.jar包里实现的（非mysql-server中的缓存），缓存的key是完整的sql语句，value是PreparedStatement对象。放入缓存是`PreparedStatement.close()`触发的，所以只要缓存`PreparedStatement`对象没有关闭，不管调用多少次`connection.prapareStatement(sql)`对相同的sql语句进行预编译，都会将预编译的请求发给MySQL，MySQL也会对每一个sql语句不管是否相同进行预编译，并生成一个唯一的Statement ID并返回；
- 缓存是针对链接的，每个链接都是独立的，不共享缓存。
