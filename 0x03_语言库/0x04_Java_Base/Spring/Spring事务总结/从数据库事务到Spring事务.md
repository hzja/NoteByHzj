Java 事务 Spring
<a name="z3EpM"></a>
## 数据库的事务
构成单一逻辑工作单元的**操作集合**称作事务（transaction）。即使有故障，数据库系统也必须保证事务的正确执行——要么执行整个事务，要么属于该事务的操作一个也不执行。以资金转账为例，应该保证支票账户支出金额的操作和储蓄账户的存入金额的操作在同一个逻辑工作单元内完成。简言之，**事务是访问并可能更新各种数据项的一个程序执行单元（unit）**。<br />数据库事务（Transaction，简写为 TX）是数据库管理系统执行过程中的一个逻辑单位，是可以提交或回滚的工作的原子单元。当事务对数据库进行多次更改时，要么在提交事务时所有更改都成功，要么在回滚事务时所有更改都被撤消。<br />数据库（包括但不限于关系型）事务一般拥有以下 4 个特性，称之为 ACID 特性
<a name="neNAp"></a>
## ACID

- 原子性（`Atomicity`）：事务作为一个整体被执行，包含在其中的对数据库的操作要么全部被执行，要么都不执行。
- 一致性（`Consistency`）：事务应确保数据库的状态从一个一致状态转变为另一个一致状态。_一致状态_的含义是数据库中的数据应满足完整性约束。
- 隔离性（`Isolation`）：多个事务并发执行时，一个事务的执行不应影响其他事务的执行。
- 持久性（`Durability`）：已被提交的事务对数据库的修改应该永久保存在数据库中。
<a name="Ka6w8"></a>
## 事务的隔离级别
在实际应用中，数据库中的数据是要被多个用户共同访问的，在多个用户同时操作相同的数据时，可能就会出现一些**事务并发**的问题：

1. **脏读（Dirty Read）**。一个事务读取到另一个事务未提交的数据。
2. **不可重复读（Non-repeatable Read）**。一个事务对**同一行数据**重复读取两次，但得到的结果不同。
3. **虚读/幻读（Phantom Read）**。一个事务执行两次**查询**，但第二次查询的结果包含了第一次查询中未出现的数据。
4. **丢失更新（Lost Update）**。丢失更新可分为两类，分别是第一类丢失更新和第二类丢失更新。第一类丢失更新是指两个事务同时操作同一个数据时，当第一个事务撤销时，把已经提交的第二个事务的更新数据覆盖了，第二个事务就造成了数据丢失。第二类丢失更新是指当两个事务同时操作同一个数据时，第一个事务将修改结果成功提交后，对第二个事务已经提交的修改结果进行了覆盖，对第二个事务造成了数据丢失。

为了避免上述事务并发问题的出现，在标准的 SQL 规范中定义了四种事务隔离级别，不同的隔离级别对事务的处理有所不同：

1. **Serializable（可串行化）**提供严格的事务隔离。它要求事务序列化执行，事务只能一个接一个地执行，不能并发执行。此隔离级别可有效防止脏读、不可重复读、幻读。但这个级别可能导致大量的超时现象和锁竞争，在实际应用中很少使用。
2. **Repeatable Read（可重复读）**一个事务在执行过程中，可以访问其他事务成功提交的新插入的数据，但不可以访问成功修改的数据。读取数据的事务将会禁止写事务（但允许读事务），写事务则禁止任何其他事务。此隔离级别可有效防止不可重复读和脏读。
3. **Committed Read（已提交读）**一个事务在执行过程中，既可以访问其他事务成功提交的新插入的数据，又可以访问成功修改的数据。读取数据的事务允许其他事务继续访问该行数据，但是未提交的写事务将会禁止其他事务访问该行。此隔离级别可有效防止脏读。
4. **Uncommitted Read（未提交读）**一个事务在执行过程中，既可以访问其他事务未提交的新插入的数据，又可以访问未提交的修改数据。如果一个事务已经开始写数据，则另外一个事务不允许同时进行写操作，但允许其他事务读此行数据。此隔离级别可防止丢失更新。

以上所有隔离性级别都不允许脏写（Dirty Write）。<br />一般来说，事务的隔离级别越高，越能保证数据库的**完整性**和**一致性**，但相对来说，隔离级别越高，对并发性能的影响也越大。因此，通常将**数据库的默认隔离级别设置为已提交读** （Committed Read），它既能防止脏读，又能有较好的并发性能。虽然这种隔离级别会导致不可重复读、幻读和第二类丢失更新这些并发问题，但可通过在应用程序中采用悲观锁或乐观锁加以控制。
<a name="W43Le"></a>
## 事务的七种传播行为
什么是事务的传播行为：事务传播行为用来描述由某一个事务传播行为修饰的方法被嵌套进另一个方法的时事务如何传播。

- **PROPAGATION_REQUIRED** 表示当前方法必须在一个具有事务的上下文中运行，如有客户端有事务在进行，那么被调用端将在该事务中运行，否则的话重新开启一个事务。( 如果被调用端发生异常，那么调用端和被调用端事务都将回滚)
- **PROPAGATION_SUPPORTS** 表示当前方法不必需要具有一个事务上下文，但是如果有一个事务的话，它也可以在这个事务中运行
- **PROPAGATION_MANDATORY** 表示当前方法必须在一个事务中运行，如果没有事务，将抛出异常
- **PROPAGATION_REQUIRES_NEW** 总是开启一个新的事务。如果一个事务已经存在，则将这个存在的事务挂起。
- **PROPAGATION_NOT_SUPPORTED** 总是非事务地执行，并挂起任何存在的事务。
- **PROPAGATION_NEVER** 总是非事务地执行，如果存在一个活动事务，则抛出异常
- **PROPAGATION_NESTED **表示如果当前方法正有一个事务在运行中，则该方法应该运行在一个嵌套事务中，被嵌套的事务可以独立于被封装的事务中进行提交或者回滚。如果封装事务存在，并且外层事务抛出异常回滚，那么内层事务必须回滚，反之，内层事务并不影响外层事务。如果封装事务不存在，则同`propagation.required`的一样
<a name="g7hpZ"></a>
## MySQL 中的事务
```plsql
START TRANSACTION
    [transaction_characteristic [, transaction_characteristic] ...]

transaction_characteristic: {
    WITH CONSISTENT SNAPSHOT
  | READ WRITE
  | READ ONLY
}

BEGIN [WORK]
COMMIT [WORK] [AND [NO] CHAIN] [[NO] RELEASE]
ROLLBACK [WORK] [AND [NO] CHAIN] [[NO] RELEASE]
SET autocommit = {0 | 1}
```

- `START TRANSACTION`或 `BEGIN`开始新事务。
- `COMMIT` 提交当前事务。
- `ROLLBACK` 回滚当前事务。
- `SET autocommit` 禁用或启用当前会话的默认自动提交模式。

默认情况下，MySQL是自动提交的模式，所有语句会立即提交
<a name="zQb4w"></a>
## JDBC 中的事务
JDBC 是 Java 语言中用来规范客户端程序如何来访问数据库的应用程序接口，提供了查询和更新数据库中数据的方法。JDBC 也是 Sun Microsystems 的商标（现在属于 Oracle），是面向关系型数据库的。<br />上面说到，MySQL 是默认自动提交的，所以 JDBC 中事务事务的第一步，需要禁用自动提交：
```java
con.setAutoCommit(false);
```
提交事务：
```java
con.commit();
```
回滚事务：
```java
con.rollback();
```
一个完整流程的例子（摘自 Oracle JDBC 文档）：
```java
public void updateCoffeeSales(HashMap<String, Integer> salesForWeek)
    throws SQLException {

    PreparedStatement updateSales = null;
    PreparedStatement updateTotal = null;

    String updateString =
        "update " + dbName + ".COFFEES " +
        "set SALES = ? where COF_NAME = ?";

    String updateStatement =
        "update " + dbName + ".COFFEES " +
        "set TOTAL = TOTAL + ? " +
        "where COF_NAME = ?";

    try {
        con.setAutoCommit(false);
        updateSales = con.prepareStatement(updateString);
        updateTotal = con.prepareStatement(updateStatement);

        for (Map.Entry<String, Integer> e : salesForWeek.entrySet()) {
            updateSales.setInt(1, e.getValue().intValue());
            updateSales.setString(2, e.getKey());
            updateSales.executeUpdate();
            updateTotal.setInt(1, e.getValue().intValue());
            updateTotal.setString(2, e.getKey());
            updateTotal.executeUpdate();
            con.commit();
        }
    } catch (SQLException e ) {
        JDBCTutorialUtilities.printSQLException(e);
        if (con != null) {
            try {
                System.err.print("Transaction is being rolled back");
                con.rollback();
            } catch(SQLException excep) {
                JDBCTutorialUtilities.printSQLException(excep);
            }
        }
    } finally {
        if (updateSales != null) {
            updateSales.close();
        }
        if (updateTotal != null) {
            updateTotal.close();
        }
        con.setAutoCommit(true);
    }
}
```
```java
import java.sql.Connection;
import java.sql.DriverManager;

public class DemoApplication {

    public static void main(String[] args) {
        // 1.获取连接
        Connection conn = DriverManager.getConnection();
        try {
            // 2.将自动提交设置为false
            conn.setAutoCommit(false);  
            /*-----------------*/
            // 3.执行一到多个CRUD操作
            /*-----------------*/
            // 4.1手动提交
            conn.commit();
        } catch (Exception e) {
            // 4.2一旦其中一个操作出错都将回滚，所有操作都不成功
            conn.rollback();
        } finally {
            // 5.关闭连接
            conn.colse();
        }
    }
}
```
<a name="IL68V"></a>
## 为什么需要事务管理器
如果没有事务管理器的话，程序可能是这样：
```java
Connection connection = acquireConnection();
try{
    int updated = connection.prepareStatement().executeUpdate();
    connection.commit();
}catch (Exception e){
    rollback(connection);
}finally {
    releaseConnection(connection);
}
```
也有可能是这样 "优雅的事务"：
```java
execute(new TxCallback() {
    @Override
    public Object doInTx(Connection var1) {
        //do something...
        return null;
    }
});
public void execute(TxCallback txCallback){
    Connection connection = acquireConnection();
    try{
        txCallback.doInTx(connection);
        connection.commit();
    }catch (Exception e){
        rollback(connection);
    }finally {
        releaseConnection(connection);
    }
}

# lambda版
execute(connection -> {
    //do something...
    return null;
});
```
但是以上两种方式，针对一些复杂的场景是很不方便的。在实际的业务场景中，往往有比较复杂的业务逻辑，代码冗长，逻辑关联复杂，如果一个大操作中又全是这种代码的话开发人员可能会疯。更不用提定制化的隔离级别，以及嵌套 / 独立事务的处理了。
<a name="AXr1Z"></a>
## Spring 事务管理器（Transaction Manager）简介
Spring 作为 Java 最强框架，事务管理也是其核心功能之一。Spring 为事务管理提供了统一的抽象，有以下优点：

- 跨不同事务 API（例如 Java 事务 API（JTA），JDBC，Hibernate，Java 持久性 API（JPA）和 Java 数据对象（JDO））的一致编程模型。
- 支持声明式事务管理（注解形式）
- 与 JTA 之类的复杂事务 API 相比， 用于程序化事务管理的 API 更简单
- 和 Spring 的 Data 层抽象集成方便（比如 Spring - Hibernate/Jdbc/Mybatis/Jpa...）

Spring都**使用统一的编程模型**，使得应用程序可以很容易地在不同的事务框架之间进行切换。这也符合面向接口编程思想。Spring事务框架的代码在**org.springframework:spring-tx**中。Spring事务抽象的核心类图如下：<br />![2021-05-12-09-14-54-850013.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620782369306-ad8044f4-78be-4512-8e78-b96e7c16abd2.png#clientId=u534ec505-4ef8-4&from=ui&id=u72dba670&originHeight=628&originWidth=960&originalType=binary&ratio=1&size=1812267&status=done&style=shadow&taskId=u8559f0b5-7d07-4a59-8ee7-5fb6211edb0)<br />Spring 的事务管理器只是一个接口 / 抽象，不同的 DB 层框架（其实不光是 DB 类框架，支持事务模型的理论上都可以使用这套抽象） 可能都需要实现此标准才可以更好的工作，核心接口是`org.springframework.transaction.support.AbstractPlatformTransactionManager`，其代码位于`spring-tx`模块中，比如 Hibernate 中的实现为：`org.springframework.orm.hibernate4.HibernateTransactionManager`<br />接口`PlatformTransactionManager`定义事务操作的行为，`PlatformTransactionManager`依赖`TransactionDefinition`和`TransactionStatus`接口。`TransactionDefinition`接口定义与Spring兼容的事务属性（如隔离级别、事务传播行为等）。`TransactionStatus`接口则定义事务的状态（如是否回滚、是否完成、是否包含安全点（Save Point）、将基础会话刷新到数据存储区（如果适用）等）。
<a name="vUCxx"></a>
### PlatformTransactionManager简介
PlatformTransactionManager是Spring事务框架的核心接口。应用程序可以直接使用PlatformTransactionManager，但它并不是主要用于API：应用程序将借助事务模板（TransactionTemplate）或声明式事务（Declarative Transaction）。对于需要实现PlatformTransactionManager接口的应用程序，可通过继承AbstractPlatformTransactionManager抽象类的方式实现。AbstractPlatformTransactionManager类已实现事务传播行为和事务同步处理。子类需要实现针对事务特定状态（如：begin，suspend，resume，commit）的模板方法。Spring事务框架已经实现了JtaTransactionManager（JPA）和DataSourceTransactionManager（JDBC）。应用程序可以参考以上方法实现事务管理器。PlatformTransactionManager事务继承示例如下：<br />![2021-05-12-09-14-54-962520.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620782307865-b9a85308-3c70-49d6-8685-fdc067b35f55.png#clientId=u534ec505-4ef8-4&from=ui&id=ud5a07079&originHeight=445&originWidth=729&originalType=binary&ratio=1&size=975310&status=done&style=shadow&taskId=u25f077cb-7bbe-4b3e-b996-8d597efa0d9)
<a name="tBrYp"></a>
### Spring事务隔离级别和传播级别
`TransactionDefinition`接口中定义了Spring事务隔离级别和Spring事务传播级别。隔离级别主要控制事务并发访问时隔离程度。Spring支持的隔离级别如下：

| 隔离级别 | 描述 |
| --- | --- |
| `DEFAULT` | 使用数据库本身使用的隔离级别ORACLE(读已提交)MySQL(可重复读) |
| `READ_UNCOMITTED` | 读未提交(脏读）最低的隔离级别，一切皆有可能。 |
| `READ_COMMITED` | 读已提交，ORACLE默认隔离级别，有幻读以及不可重复读风险。 |
| `REPEATABLE_READ` | 可重复读，解决不可重复读的隔离级别，但还是有幻读风险。 |
| `SERLALIZABLE` | 串行化，最高的事务隔离级别，不管多少事务，挨个运行完一个事务的所有子事务之后才可以执行另外一个事务里面的所有子事务，这样就解决了脏读、不可重复读和幻读的问题了 |

除了使用`ISOLATION_DEFAULT`表示使用数据库默认的隔离级别外，其余四个隔离级别与数据库规范的隔离级别一致。需要注意的是，隔离级别越高，意味着数据库事务并发执行性能越差。JDBC规范虽然定义了事务支持的以上行为，但是各个JDBC驱动、数据库厂商对事务的支持程度可能各不相同。出于性能的考虑我们一般设置READ_COMMITTED级别。针对READ_COMMITTED隔离级别无法避免的脏读，通常使用数据库的锁来处理。传播级别主要控制含事务方法的调用（如一个事务方法调用另一个事务方法）时，Spring对事务的处理方式。Spring事务传播级别共七类。它们是：<br />(1)`**PROPAGATION_REQUIRED**`：支持当前事务，如果当前有事务则加入，如果当前没有事务则新建一个。这种方式是**默认的事务传播方式**。<br />(2)`**PROPAGATION_SUPPORTS**`：支持当前事务，如果当前有事务则加入，如果当前没有事务则以非事务方式执行。<br />(3)`**PROPAGATION_MANDATORY**`：支持当前事务，如果当前有事务则加入，如果当前没有事务则抛出异常。（当前必须有事务）<br />(4)`**PROPAGATION_REQUIRES_NEW**`：支持当前事务，如果当前有事务则挂起当前事务，然后新创建一个事务，如果当前没有事务则自己创建一个事务。<br />(5)`**Propagation_NOT_SUPPORTED**`：不支持当前事务，如果当前有事务则把当前事务挂起，执行完后恢复事务（忽略当前事务）。<br />(6)`**PROPAGATION_NEVER**`：不支持当前事务，如果当前存在事务，则抛出异常。（当前必须不能有事务）<br />(7)`**PROPAGATION_NESTED**`：如果当前存在事务，则嵌套在当前事务中。如果当前没有事务，则新建一个事务自己执行。对嵌套事务来说，内部事务回滚时不会影响外部事务的提交；但是外部事务回滚会把内部事务一起回滚回去。（这个和新建一个事务的区别）
<a name="ztKaV"></a>
### 使用方式
事务，自然是控制业务的，在一个业务流程内，往往希望保证原子性，要么全成功要么全失败。所以事务一般是加载`@Service`层，一个 Service 内调用了多个操作数据库的操作（比如 Dao），在 Service 结束后事务自动提交，如有异常抛出则事务回滚。<br />这也是 Spring 事务管理的基本使用原则。
<a name="wPKd9"></a>
#### 注解
在被 Spring 管理的类头上增加`@Transactional`注解，即可对该类下的所有方法开启事务管理。事务开启后，方法内的操作无需手动开启 / 提交 / 回滚事务，一切交给 Spring 管理即可。
```java
@Service
@Transactional
public class TxTestService{
    
    @Autowired
    private OrderRepo orderRepo;

    public void submit(Order order){
        orderRepo.save(order);
    }
}
```
也可以只在方法上配置，方法配置的优先级是大于类的
```java
@Service
public class TxTestService{
    
    @Autowired
    private OrderRepo orderRepo;


    @Transactional
    public void submit(Order order){
        orderRepo.save(order);
    }
}
```
<a name="qAtAA"></a>
#### `TransactionTemplate`
`TransactionTemplate` 这中方式，其实和使用注解形式的区别不大，其核心功能也是由 `TransactionManager` 实现的，这里只是换了个入口
```java
public <T> T execute(TransactionCallback<T> action) throws TransactionException {
    if (this.transactionManager instanceof CallbackPreferringPlatformTransactionManager) {
        return ((CallbackPreferringPlatformTransactionManager) this.transactionManager).execute(this, action);
    }
    else {
        //获取事务信息
        TransactionStatus status = this.transactionManager.getTransaction(this);
        T result;
        try {
            //执行业务代码
            result = action.doInTransaction(status);
        }
        //处理异常回滚
        catch (RuntimeException ex) {
            // Transactional code threw application exception -> rollback
            rollbackOnException(status, ex);
            throw ex;
        }
        catch (Error err) {
            // Transactional code threw error -> rollback
            rollbackOnException(status, err);
            throw err;
        }
        catch (Exception ex) {
            // Transactional code threw unexpected exception -> rollback
            rollbackOnException(status, ex);
            throw new UndeclaredThrowableException(ex, "TransactionCallback threw undeclared checked exception");
        }
        //提交事务
        this.transactionManager.commit(status);
        return result;
    }
}
```
<a name="qfmwz"></a>
#### XML 配置 `tx:advice`
过于古老，不做解释
<a name="xBZ9Z"></a>
#### 隔离级别 (Isolation Level)
事务隔离级别是数据库最重要的特性之一，他保证了脏读 / 幻读等问题不会发生。作为一个事务管理框架自然也是支持此配置的，在 `@Transactional` 注解中有一个 `isolation` 配置，可以很方便的配置各个事务的隔离级别，等同于`connection.setTransactionIsolation()`
```java
Isolation {
    DEFAULT(-1),
    READ_UNCOMMITTED(1),
    READ_COMMITTED(2),
    REPEATABLE_READ(4),
    SERIALIZABLE(8);
}
```
<a name="obZwQ"></a>
#### 传播行为 (Propagation behavior)
可能没有接触过 Spring 的人听到传播行为会奇怪，这是个什么东西。<br />其实这个传播行为和数据库功能无关，只是事务管理器为了处理复杂业务而设计的一个机制。比如现在有这样一个调用场景，A Service -> B Service -> C Service，但是希望 A/B 在一个事务内，C 是一个独立的事务，同时 C 如果出错，不影响 AB 所在的事务。<br />此时，就可以通过传播行为来处理；将 C Service 的事务配置为`@Transactional(propagation = Propagation.REQUIRES_NEW)`即可<br />Spring 支持以下几种传播行为：

- `REQUIRED` 默认策略，优先使用当前事务（及当前线程绑定的事务资源），如果不存在事务，则开启新事务
- `SUPPORTS` 优先使用当前的事务（及当前线程绑定的事务资源），如果不存在事务，则以无事务方式运行
- `MANDATORY` 优先使用当前的事务，如果不存在事务，则抛出异常
- `REQUIRES_NEW` 创建一个新事务，如果存在当前事务，则挂起（Suspend）
- `NOT_SUPPORTED` 以非事务方式执行，如果当前事务存在，则挂起当前事务。
- `NEVER` 以非事务方式执行，如果当前事务存在，则抛出异常
<a name="Flpqs"></a>
#### 回滚策略
`@Transactional` 中有 4 个配置回滚策略的属性，分为 `Rollback` 策略，和 `NoRollback` 策略<br />默认情况下，`RuntimeException` 和 `Error` 这两种异常会导致事务回滚，普通的 `Exception`（需要 Catch 的）异常不会回滚。
<a name="nHs4A"></a>
##### `Rollback`
配置需要回滚的异常类
```java
# 异常类Class
Class<? extends Throwable>[] rollbackFor() default {};
# 异常类ClassName，可以是FullName/SimpleName
String[] rollbackForClassName() default {};
```
<a name="ALz1b"></a>
##### `NoRollback`
针对一些要特殊处理的业务逻辑，比如插一些日志表，或者不重要的业务流程，希望就算出错也不影响事务的提交。<br />可以通过配置 `NoRollbackFor` 来实现，让某些异常不影响事务的状态。
```java
# 异常类Class
Class<? extends Throwable>[] noRollbackFor() default {};
# 异常类ClassName，可以是FullName/SimpleName
String[] noRollbackForClassName() default {};
```
<a name="lkPVW"></a>
#### 只读控制
设置当时事务的只读标示，等同于`connection.setReadOnly()`<br />关键名词解释

| 名词 | 概念 |
| --- | --- |
| `PlatformTransactionManager` | 事务管理器,管理事务的各生命周期方法，简称`TxMgr` |
| `TransactionAttribute` | 事务属性，包含隔离级别，传播行为，是否只读等信息，简称`TxAttr` |
| `TransactionStatus` | 事务状态，包含当前事务、挂起等信息，简称`TxStatus` |
| `Transactionlnfo` | 事务信息，内含`TxMgr`，`TxAttr`，`TxStatus`等信息，简称`Txlnfo` |
| `Transactionsynchronization` | 事务同步回调，内含多个钩子方法，简称`TxSync` / `transaction synchronization` |
| `TransactionSynchronizationManag` | 事务同步管理器，维护当前线程事务资源，信息以及`TxSync` 集合 |

<a name="IaStb"></a>
#### 基本原理
```java
public void execute(TxCallback txCallback){
    //获取连接
    Connection connection = acquireConnection();
    try{
        //执行业务代码
        doInService();
        //提交事务
        connection.commit();
    }catch (Exception e){
        //回滚事务
        rollback(connection);
    }finally {
        //释放连接
        releaseConnection(connection);
    }
}
```
Spring 事务管理的基本原理就是以上代码，获取连接 -> 执行代码 -> 提交 / 回滚事务。Spring 只是将这个流程给抽象出来了，所有事务相关的操作都交由 `TransactionManager` 去实现，然后封装一个模板形式的入口来执行 t<br />比如`org.springframework.transaction.support.TransactionTemplate`的实现：
```java
@Override
public <T> T execute(TransactionCallback<T> action) throws TransactionException {
    if (this.transactionManager instanceof CallbackPreferringPlatformTransactionManager) {
        return ((CallbackPreferringPlatformTransactionManager) this.transactionManager).execute(this, action);
    }
    else {
        //通过事务管理器获取事务
        TransactionStatus status = this.transactionManager.getTransaction(this);
        T result;
        try {
            //执行业务代码
            result = action.doInTransaction(status);
        }
        //处理异常回滚
        catch (RuntimeException ex) {
            // Transactional code threw application exception -> rollback
            rollbackOnException(status, ex);
            throw ex;
        }
        catch (Error err) {
            // Transactional code threw error -> rollback
            rollbackOnException(status, err);
            throw err;
        }
        catch (Exception ex) {
            // Transactional code threw unexpected exception -> rollback
            rollbackOnException(status, ex);
            throw new UndeclaredThrowableException(ex, "TransactionCallback threw undeclared checked exception");
        }
        //提交事务
        this.transactionManager.commit(status);
        return result;
    }
}
```
注解形式的事务（`@Transactional`），实现机制也是一样，基于 Spring 的 AOP，将上面 Template 的模式换成了自动的 AOP，在 AOP 的 Interceptor（`org.springframework.transaction.interceptor.TransactionInterceptor`）中来执行这套流程：
```java
protected Object invokeWithinTransaction(Method method, Class<?> targetClass, final InvocationCallback invocation)
            throws Throwable {

        // If the transaction attribute is null, the method is non-transactional.
        final TransactionAttribute txAttr = getTransactionAttributeSource().getTransactionAttribute(method, targetClass);
        //获取事务管理器
        final PlatformTransactionManager tm = determineTransactionManager(txAttr);
        final String joinpointIdentification = methodIdentification(method, targetClass);

        if (txAttr == null || !(tm instanceof CallbackPreferringPlatformTransactionManager)) {
            // Standard transaction demarcation with getTransaction and commit/rollback calls.
            //创建事务
            TransactionInfo txInfo = createTransactionIfNecessary(tm, txAttr, joinpointIdentification);
            Object retVal = null;
            try {
                // This is an around advice: Invoke the next interceptor in the chain.
                // This will normally result in a target object being invoked.
                //执行被“AOP”的代码
                retVal = invocation.proceedWithInvocation();
            }
            catch (Throwable ex) {
                // target invocation exception
                //处理异常回滚
                completeTransactionAfterThrowing(txInfo, ex);
                throw ex;
            }
            finally {
                //清除资源
                cleanupTransactionInfo(txInfo);
            }
            
            //提交事务
            commitTransactionAfterReturning(txInfo);
            return retVal;
        }
   ....
}
```
<a name="sRoRG"></a>
#### 复杂流程下的事务传播 / 保持相同事务的关键：
对于复杂一些的业务流程，会出现各种类之间的调用，Spring 是如何做到保持同一个事务的？<br />其实基本原理很简单，只需要将当前事务（`Connection`）隐式的保存至事务管理器内，后续方法在执行 JDBC 操作前，从事务管理器内获取即可：<br />比如`HibernateTemplate`中的`SessionFactory`中的`getCurrentSession`，这里的`getCurrentSession`就是从（可能是间接的）Spring 事务管理器中获取的 Spring 事务管理器将处理事务时的相关临时资源（Connection 等）存在`org.springframework.transaction.support.TransactionSynchronizationManager`中，通过 `ThreadLocal` 维护
```java
public abstract class TransactionSynchronizationManager {

    private static final Log logger = LogFactory.getLog(TransactionSynchronizationManager.class);

    private static final ThreadLocal<Map<Object, Object>> resources =
            new NamedThreadLocal<Map<Object, Object>>("Transactional resources");

    private static final ThreadLocal<Set<TransactionSynchronization>> synchronizations =
            new NamedThreadLocal<Set<TransactionSynchronization>>("Transaction synchronizations");

    private static final ThreadLocal<String> currentTransactionName =
            new NamedThreadLocal<String>("Current transaction name");

    private static final ThreadLocal<Boolean> currentTransactionReadOnly =
            new NamedThreadLocal<Boolean>("Current transaction read-only status");

    private static final ThreadLocal<Integer> currentTransactionIsolationLevel =
            new NamedThreadLocal<Integer>("Current transaction isolation level");

    private static final ThreadLocal<Boolean> actualTransactionActive =
            new NamedThreadLocal<Boolean>("Actual transaction active");
    ...
}
```
针对一些复杂场景，嵌套事务 + 独立事务，涉及到挂起（`suspend`），恢复（`resume`）的情况，相关资源也是存储在`TransactionSynchronizationManager`中的，方便嵌套事务的处理。比如 A->B 时，A 方法已经开启了事务，并将当前事务资源绑定在`TransactionSynchronizationManager`，那么执行 B 之前，会检测当前是否已经存在事务；检测方式就是从`TransactionSynchronizationManager`查找并检测状态，如果已经在事务内，那么就根据不同的传播行为配置来执行不同的逻辑，对于 `REQUIRES_NEW`等传播行为的处理会麻烦一些，会涉及到 “挂起（`suspend`）” 和恢复 (`resume`) 的操作，原理打通小异，这里就不做过多解释了
<a name="cCeQM"></a>
#### 常见问题
事务没生效<br />有下列代码，入口为 test 方法，在 `testTx` 方法中配置了 `@Transactional` 注解，同时在插入数据后抛出 `RuntimeException` 异常，但是方法执行后插入的数据并没有回滚，竟然插入成功了
```java
public void test(){
    testTx();
}

@Transactional
public void testTx(){
    UrlMappingEntity urlMappingEntity = new UrlMappingEntity();
    urlMappingEntity.setUrl("http://www.baidu.com");
    urlMappingEntity.setExpireIn(777l);
    urlMappingEntity.setCreateTime(new Date());
    urlMappingRepository.save(urlMappingEntity);
    if(true){
        throw new RuntimeException();
    }
}
```
这里不生效的原因是因为入口的方法 / 类没有增加 `@Transaction` 注解，由于 Spring 的事务管理器也是基于 AOP 实现的，不管是 Cglib(ASM) 还是 Jdk 的动态代理，本质上也都是子类机制；在同类之间的方法调用会直接调用本类代码，不会执行动态代理曾的代码；所以在这个例子中，由于入口方法test没有增加代理注解，所以`textTx`方法上增加的事务注解并不会生效
<a name="gv17s"></a>
#### 异步后事务失效
比如在一个事务方法中，开启了子线程操作库，那么此时子线程的事务和主线程事务是不同的。<br />因为在 Spring 的事务管理器中，事务相关的资源（连接，session，事务状态之类）都是存放在 `TransactionSynchronizationManager` 中的，通过 ThreadLocal 存放，如果跨线程的话就无法保证一个事务了
```java
# TransactionSynchronizationManager.java
private static final ThreadLocal<Map<Object, Object>> resources =
        new NamedThreadLocal<>("Transactional resources");
private static final ThreadLocal<Set<TransactionSynchronization>> synchronizations =
        new NamedThreadLocal<>("Transaction synchronizations");
private static final ThreadLocal<String> currentTransactionName =
        new NamedThreadLocal<>("Current transaction name");
private static final ThreadLocal<Boolean> currentTransactionReadOnly =
        new NamedThreadLocal<>("Current transaction read-only status");
private static final ThreadLocal<Integer> currentTransactionIsolationLevel =
        new NamedThreadLocal<>("Current transaction isolation level");
private static final ThreadLocal<Boolean> actualTransactionActive =
        new NamedThreadLocal<>("Actual transaction active");
```
事务提交失败
```java
org.springframework.transaction.UnexpectedRollbackException: 
Transaction silently rolled back because it has been marked as rollback-only
```
这个异常是由于在同一个事务内，多个事务方法之间调用，子方法抛出异常，但又被父方法忽略了导致的。<br />因为子方法抛出了异常，Spring 事务管理器会将当前事务标为失败状态，准备进行回滚，可是当子方法执行完毕出栈后，父方法又忽略了此异常，待方法执行完毕后正常提交时，事务管理器会检查回滚状态，若有回滚标示则抛出此异常。<br />具体可以参考`org.springframework.transaction.support.AbstractPlatformTransactionManager#processCommit`<br />示例代码：
```java
A -> B
# A Service(@Transactional):
public void testTx(){
    urlMappingRepo.deleteById(98l);
    try{
        txSubService.testSubTx();
    }catch (Exception e){
        e.printStackTrace();
    }
}

# B Service(@Transactional)
public void testSubTx(){
    if(true){
        throw new RuntimeException();
    }
}
```
