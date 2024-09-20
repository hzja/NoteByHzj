Java Spring 事务
<a name="iwkMc"></a>
## 1、引言
Spring 的声明式事务极大地方便了日常的事务相关代码编写，它的设计如此巧妙，以至于在使用中几乎感觉不到它的存在，只需要优雅地加一个 `@Transactional` 注解，一切就都顺理成章地完成了！<br />毫不夸张地讲，Spring 的声明式事务实在是太好用了，以至于大多数人都忘记了编程式事务应该怎么写。<br />不过，越是认为理所应当的事情，如果出了问题，就越难排查。不知道有没有遇到过 `@Transactional` 失效的场景，这不但是日常开发中常踩的坑，也是面试中的高频问题。<br />其实这些失效场景不用死记硬背，如果搞明白了它的工作原理，再结合源码，需要用到的时候 Debug 一下就能自己分析出来。<br />这里将结合几个常见失效情况，从源码层面解释其失效原因。认真读完本文，相信对声明式事务有更深刻的认识。
<a name="sOgOM"></a>
## 2、回顾手写事务
数据库层面的事务，有 ACID 四个特性，他们共同保证了数据库中数据的准确性。事务的原理并不是本文的重点，只需要知道样例中用的 H2 数据库完全实现了对事务的支持（read committed）。<br />编写 Java 代码时，使用 JDBC 接口与数据库交互，完成事务的相关指令，伪代码如下：
```java
//获取用于和数据库交互的连接
Connection conn = DriverManager.getConnection();
try {
    // 关闭自动提交:
    conn.setAutoCommit(false);
    // 执行多条SQL语句:
    insert(); 
    update(); 
    delete();
    // 提交事务:
    conn.commit();
} catch (SQLException e) {
    // 如果出现异常，回滚事务:
    conn.rollback();
} finally {
    //释放资源
    conn.close();
}
```
这是典型的编程式事务代码流程：开始前先关闭自动提交，因为默认情况下，自动提交是开启的，每条语句都会开启新事务，执行完毕后自动提交。<br />关闭事务的自动提交，是为了让多个 SQL 语句在同一个事务中。代码正常运行，就提交事务，出现异常，就整体回滚，以此保证多条 SQL 语句的整体性。<br />除了事务提交，数据库还支持保存点的概念，在一个物理事务中，可以设置多个保存点，方便回滚到指定保存点（其类似玩单机游戏时的存档，可以在角色挂掉后随时回到上次的存档）设置和回滚到保存点的代码如下：
```java
//设置保存点    
Savepoint savepoint = connection.setSavepoint();
//回滚到指定的保存点
connection.rollback(savepoint);
//回滚到保存点后按需提交/回滚前面的事务
conn.commit();//conn.rollback();
```
Spring 声明式事务所做的工作，就是围绕着 提交/回滚 事务，设置/回滚到保存点 这两对命令进行的。为了尽可能地少写代码，Spring 定义了几种传播属性将事务做了进一步的抽象。注意哦，Spring 的事务传播(Propagation) 只是 Spring 定义的一层抽象而已，和数据库没啥关系，不要和数据库的事务隔离级别混淆。
<a name="n8HT3"></a>
## 3、Spring 的事务传播（Transaction Propagation）
观察传统事务代码：
```java
conn.setAutoCommit(false);
// 执行多条SQL语句:
insert(); 
update(); 
delete();
// 提交事务:
conn.commit();
```
这段代码表达的是三个 SQL 语句在同一个事务里。<br />他们可能是同一个类中的不同方法，也可能是不同类中的不同方法。如何来表达诸如事务方法加入别的事务、新建自己的事务、嵌套事务等等概念呢？这就要靠 Spring 的事务传播机制了。<br />事务传播（Transaction Propagation）就是字面意思：事务的传播/传递 方式。<br />在 Spring 源码的`TransactionDefinition`接口中，定义了 7 种传播属性，官网对其中的 3 个做了说明，只要搞懂了这 3 个，剩下的 4 个就是举一反三的事了。
<a name="nz3iJ"></a>
### 1)、`PROPAGATION_REQUIRED`
字面意思：传播-必须<br />`PROPAGATION_REQUIRED`是其默认传播属性，强制开启事务，如果之前的方法已经开启了事务，则加入前一个事务，二者在物理上属于同一个事务。<br />一图胜千言，下图表示它俩物理上是在同一个事务内：<br />![2021-09-06-23-47-20-468525.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630943537932-915a7a11-47f0-44fa-8e65-e8619745f705.png#clientId=u47e17932-f52c-4&from=ui&id=u95fb3bf2&originHeight=341&originWidth=800&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46974&status=done&style=shadow&taskId=u8d8fbace-2ed6-4d66-a6dd-22ca4c08129&title=)<br />上图翻译成伪代码是这样的：
```java
try {
    conn.setAutoCommit(false);
    transactionalMethod1(); 
    transactionalMethod2();
    conn.commit();
} catch (SQLException e) {
    conn.rollback();
} finally {
    conn.close();
}
```
既然在同一个物理事务中，那如果`transactionalMethod2()`发生了异常，导致需要回滚，那么请问`transactionalMethod1()`是否也要回滚呢？<br />得益于上面的图解和伪代码，可以很容易地得出答案，`transactionalMethod1()`肯定回滚了。<br />这里抛一个问题：<br />事务方法里面的异常被 try catch 吃了，事务还能回滚吗？<br />先别着急出结论， 看下面两段代码示例。
<a name="IEB5f"></a>
#### 示例一：不会回滚的情况（事务失效）
观察下面的代码，`methodThrowsException()`什么也没干，就抛了个异常，调用方将其抛出的异常`try catch` 住了，该场景下是不会触发回滚的
```java
@Transactional(rollbackFor = Exception.class)
public void tryCatchRollBackFail(String name) {
    jdbcTemplate.execute("INSERT INTO USER (NAME) VALUES ('" + name + "')");
    try {
        methodThrowsException();
    } catch (RollBackException e) {
        //do nothing
    }
}

public void methodThrowsException() throws RollBackException {
    throw new RollBackException(ROLL_BACK_MESSAGE);
}
```
<a name="OIhbO"></a>
#### 示例二：会回滚的情况（事务生效）
再看这个例子，同样是 try catch 了异常，结果却截然相反
```java
@Transactional(rollbackFor = Throwable.class)
public void tryCatchRollBackSuccess(String name, String anotherName) {
    jdbcTemplate.execute("INSERT INTO USER (NAME) VALUES ('" + name + "')");
    try {
        // 带事务，抛异常回滚
        userService.insertWithTxThrowException(anotherName);
    } catch (RollBackException e) {
        // do nothing
    }
}

@Transactional(rollbackFor = Throwable.class)
public void insertWithTxThrowException(String name) throws RollBackException {
    jdbcTemplate.execute("INSERT INTO USER (NAME) VALUES ('" + name + "')");
    throw new RollBackException(ROLL_BACK_MESSAGE);
}
```
本例中，两个方法的事务都没有设置`propagation`属性，默认都是`PROPAGATION_REQUIRED`。即前者开启事务，后者加入前面开启的事务，二者同属于一个物理事务。`insertWithTxThrowException()`方法抛出异常，将事务标记为回滚。既然大家是在一条船上，那么后者打翻了船，前者肯定也不能幸免。<br />所以`tryCatchRollBackSuccess()`所执行的SQL也必将回滚，执行此用例可以查看结果<br />访问 http://localhost:8080/h2-console/ ，连接信息如下：<br />![2021-09-06-23-47-20-585515.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630943465629-0b0e97be-2c6a-4c9a-aaea-8b59ffb81b42.png#clientId=u47e17932-f52c-4&from=ui&id=ue9067de8&originHeight=310&originWidth=445&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13942&status=done&style=none&taskId=uf231cf2b-99ee-4127-9b6a-7c23cd49024&title=)<br />点击Connect进入控制台即可查看表中数据：<br />![2021-09-06-23-47-20-674548.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630943465649-46d7b22f-43f1-4568-84b9-314ad6e555b0.png#clientId=u47e17932-f52c-4&from=ui&id=sFD9J&originHeight=376&originWidth=579&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17409&status=done&style=shadow&taskId=u80b5b6c6-ddc1-48ef-aacc-a9369c4c178&title=)<br />USER 表确实没有插入数据，证明了结论，并且可以看到日志报错：<br />Transaction rolled back because it has been marked as rollback-only事务已经回滚，因为它被标记为必须回滚。<br />也就是后面方法触发的事务回滚，让前面方法的插入也回滚了。<br />看到这里，应该能把默认的传播类型`PROPAGATION_REQUIRED`理解透彻了，本例中是因两个方法在同一个物理事务下，相互影响从而回滚。<br />可能会问，那如果想让前后两个开启了事务的方法互不影响该怎么办呢？<br />这就要用到下面要说的传播类型了。
<a name="GA2Ts"></a>
### 2)、`PROPAGATION_REQUIRES_NEW`
字面意思：传播- 必须-新的<br />`PROPAGATION_REQUIRES_NEW`与`PROPAGATION_REQUIRED`不同的是，其总是开启独立的事务，不会参与到已存在的事务中，这就保证了两个事务的状态相互独立，互不影响，不会因为一方的回滚而干扰到另一方。<br />一图胜千言，下图表示他俩物理上不在同一个事务内：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630943230012-dc61db34-2541-4032-ac5b-4ae111a0eb87.webp#clientId=u47e17932-f52c-4&from=paste&id=ufb35d78d&originHeight=277&originWidth=801&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4da6d92d-341f-47c1-984c-215c36e84e7&title=)<br />上图翻译成伪代码是这样的：
```java
//Transaction1
try {
    conn.setAutoCommit(false);
    transactionalMethod1(); 
    conn.commit();
} catch (SQLException e) {
    conn.rollback();
} finally {
    conn.close();
}
//Transaction2
try {
    conn.setAutoCommit(false); 
    transactionalMethod2();
    conn.commit();
} catch (SQLException e) {
    conn.rollback();
} finally {
    conn.close();
}
```
`TransactionalMethod1` 开启新事务，当他调用同样需要事务的`TransactionalMethod2`时，由于后者的传播属性设置了`PROPAGATION_REQUIRES_NEW`，所以挂起前面的事务（至于如何挂起，后面会从源码中窥见），并开启一个物理上独立于前者的新事务，这样二者的事务回滚就不会相互干扰了。<br />还是前面的例子，只需要把`insertWithTxThrowException()`方法的事务传播属性设置为`Propagation.REQUIRES_NEW`就可以互不影响了：
```java
@Transactional(rollbackFor = Throwable.class)
public void tryCatchRollBackSuccess(String name, String anotherName) {
    jdbcTemplate.execute("INSERT INTO USER (NAME) VALUES ('" + name + "')");
    try {
        // 带事务，抛异常回滚
        userService.insertWithTxThrowException(anotherName);
    } catch (RollBackException e) {
        // do nothing
    }
}

@Transactional(rollbackFor = Throwable.class, propagation = Propagation.REQUIRES_NEW)
public void insertWithTxThrowException(String name) throws RollBackException {
    jdbcTemplate.execute("INSERT INTO USER (NAME) VALUES ('" + name + "')");
    throw new RollBackException(ROLL_BACK_MESSAGE);
}
```
`PROPAGATION_REQUIRED`和`Propagation.REQUIRES_NEW`已经足以应对大部分应用场景了，这也是开发中常用的事务传播类型。前者要求基于同一个物理事务，要回滚一起回滚，后者是大家使用独立事务互不干涉。还有一个场景就是：外部方法和内部方法共享一个事务，但是内部事务的回滚不影响外部事务，外部事务的回滚可以影响内部事务。这就是嵌套这种传播类型的使用场景。
<a name="tuj6p"></a>
### 3)、`PROPAGATION_NESTED`
字面意思：传播-嵌套<br />`PROPAGATION_NESTED`可以在一个已存在的物理事务上设置多个供回滚使用的保存点。这种部分回滚可以让内部事务在其自己的作用域内回滚，与此同时，外部事务可以在某些操作回滚后继续执行。其底层实现就是数据库的`savepoint`。<br />这种传播机制比前面两种都要灵活，看下面的代码：
```java
@Transactional(rollbackFor = Throwable.class)
public void invokeNestedTx(String name,String otherName) {
    jdbcTemplate.execute("INSERT INTO USER (NAME) VALUES ('" + name + "')");
    try {
        userService.insertWithTxNested(otherName);
    } catch (RollBackException e) {
        // do nothing
    }
    // 如果这里抛出异常，将导致两个方法都回滚
    // throw new RollBackException(ROLL_BACK_MESSAGE);
}

@Transactional(rollbackFor = Throwable.class,propagation = Propagation.NESTED)
public void insertWithTxNested(String name) throws RollBackException {
    jdbcTemplate.execute("INSERT INTO USER (NAME) VALUES ('" + name + "')");
    throw new RollBackException(ROLL_BACK_MESSAGE);
}
```
外部事务方法`invokeNestedTx()`开启事务，内部事务方法`insertWithTxNested`标记为嵌套事务，内部事务的回滚通过保存点完成，不会影响外部事务。而外部方法的回滚，则会连带内部方法一块回滚。<br />小结：本小节介绍了 3 种常见的Spring 声明式事务传播属性，结合样例代码，相信也对其有所了解了，接下来从源码层面看一看，Spring 是如何简化事务样板代码，解放生产力的。
<a name="GqYWI"></a>
## 4、源码窥探
在阅读源码前，先分析一个问题：要给一个方法添加事务，需要做哪些工作呢？<br />就算自己手写，也至少得需要这么四步：

- 开启事务
- 执行方法
- 遇到异常就回滚事务
- 正常执行后提交事务

这不就是典型的AOP~<br />没错，Spring 就是通过 AOP，将事务方法增强，从而完成了事务的相关操作。下面给出几个关键类及其关键方法的源码走读。<br />既然是 AOP 那肯定要给事务写一个切面来做这个事，这个类就是 `TransactionAspectSupport`，从命名可以看出，这就是“事务切面支持类”，他的主要工作就是实现事务的执行流程，其主要实现方法为`invokeWithinTransaction`：
```java
protected Object invokeWithinTransaction(Method method, @Nullable Class<?> targetClass,
                                         final InvocationCallback invocation) throws Throwable {

    // 省略代码...
    // Standard transaction demarcation with getTransaction and commit/rollback calls.
    // 1、开启事务
    TransactionInfo txInfo = createTransactionIfNecessary(ptm, txAttr, joinpointIdentification);
    try {
        // This is an around advice: Invoke the next interceptor in the chain.
        // This will normally result in a target object being invoked.
        //2、执行方法
        retVal = invocation.proceedWithInvocation();
    }
    catch (Throwable ex) {
        // target invocation exception
        // 3、捕获异常时的处理
        completeTransactionAfterThrowing(txInfo, ex);
        throw ex;
    }
    finally {
        cleanupTransactionInfo(txInfo);
    }

    if (retVal != null && vavrPresent && VavrDelegate.isVavrTry(retVal)) {
        // Set rollback-only in case of Vavr failure matching our rollback rules...
        TransactionStatus status = txInfo.getTransactionStatus();
        if (status != null && txAttr != null) {
            retVal = VavrDelegate.evaluateTryFailure(retVal, txAttr, status);
        }
    }
    //4、执行成功，提交事务
    commitTransactionAfterReturning(txInfo);
    return retVal;
    // 省略代码...
```
结合增加的这四步注释，相信很容易就能看明白。<br />搞懂了事务的主要流程，它的传播机制又是怎么实现的呢？这就要看`AbstractPlatformTransactionManager`这个类了，从命名就能看出， 它负责事务管理，其中的`handleExistingTransaction`方法实现了事务传播逻辑，这里挑`PROPAGATION_REQUIRES_NEW`的实现跟一下代码：
```java
private TransactionStatus handleExistingTransaction(
    TransactionDefinition definition, Object transaction, boolean debugEnabled)
    throws TransactionException {
    // 省略代码...
    if (definition.getPropagationBehavior() == TransactionDefinition.PROPAGATION_REQUIRES_NEW) {
        if (debugEnabled) {
            logger.debug("Suspending current transaction, creating new transaction with name [" +
                         definition.getName() + "]");
        }
        // 事务挂起
        SuspendedResourcesHolder suspendedResources = suspend(transaction);
        try {
            return startTransaction(definition, transaction, debugEnabled, suspendedResources);
        }
        catch (RuntimeException | Error beginEx) {
            resumeAfterBeginException(transaction, suspendedResources, beginEx);
            throw beginEx;
        }
    }
    // 省略代码...
}
```
前文知道`PROPAGATION_REQUIRES_NEW`会将前一个事务挂起，并开启独立的新事务，而数据库是不支持事务的挂起的，Spring 是如何实现这一特性的呢？<br />通过源码可以看到，这里调用了返回值为`SuspendedResourcesHolder`的`suspend(transaction)`方法，它的实际逻辑由其内部的`doSuspend(transaction)`抽象方法实现。这里使用的是`JDBC`连接数据库，自然要选择`DataSourceTransactionManager`这个子类去查看其实现，代码如下：
```java
protected Object doSuspend(Object transaction) {
    DataSourceTransactionObject txObject = (DataSourceTransactionObject) transaction;
    txObject.setConnectionHolder(null);
    return TransactionSynchronizationManager.unbindResource(obtainDataSource());
}
```
这里是把已有事务的`connection`解除，并返回被挂起的资源。在接下来开启事务时，会将该挂起资源一并传入，这样当内层事务执行完成后，可以继续执行外层被挂起的事务。<br />那么，什么时候来继续执行被挂起的事务呢？<br />事务的流程，虽然是由`TransactionAspectSupport`实现的，但是真正的提交，回滚，是由`AbstractPlatformTransactionManager`来完成，在其`processCommit(DefaultTransactionStatus status)`方法最后的`finally`块中，执行了`cleanupAfterCompletion(status)`:
```java
private void cleanupAfterCompletion(DefaultTransactionStatus status) {
    status.setCompleted();
    if (status.isNewSynchronization()) {
        TransactionSynchronizationManager.clear();
    }
    if (status.isNewTransaction()) {
        doCleanupAfterCompletion(status.getTransaction());
    }
    // 有挂起事务则获取挂起的资源，继续执行
    if (status.getSuspendedResources() != null) {
        if (status.isDebug()) {
            logger.debug("Resuming suspended transaction after completion of inner transaction");
        }
        Object transaction = (status.hasTransaction() ? status.getTransaction() : null);

        resume(transaction, (SuspendedResourcesHolder) status.getSuspendedResources());
    }
}
```
这里判断有挂起的资源将会恢复执行，至此完成挂起和恢复事务的逻辑。<br />这里只给出了大概处理流程，源码里有大量细节，需要自己去体验，有了上文介绍的主逻辑框架基础，跟踪源码查看其他实现应该不怎么费劲了。
<a name="CHIv9"></a>
## 5、常见失效场景
下面简单列举几个失效场景：
<a name="jVXvA"></a>
### 1）非 public 方法不生效
官网有说明：
> Method visibility and `@Transactional`
> When you use transactional proxies with Spring’s standard configuration, you should apply the `@Transactional` annotation only to methods with `public` visibility.

<a name="ZUv4T"></a>
### 2）Spring 不支持 redis 集群中的事务
redis事务开启命令是`multi`，但是 Spring Data Redis 不支持 redis 集群中的 multi 命令，如果使用了声明式事务，将会报错：MULTI is currently not supported in cluster mode.
<a name="xW2mt"></a>
### 3）多数据源情况下需要为每个数据源配置TransactionManager，并指定transactionManager参数
第四部分源码窥探中已经看到实际执行事务操作的是`AbstractPlatformTransactionManager`，其为`TransactionManager`的实现类，每个事务的`connection`连接都受其管理，如果没有配置，无法完成事务操作。单数据源的情况下正常运行，是因为 SpringBoot 的`DataSourceTransactionManagerAutoConfiguration`自动配置了。
<a name="t5noE"></a>
### 4）rollbackFor 设置错误
默认情况下只回滚非受检异常（也就是，`java.lang.RuntimeException`的子类）和java.lang.Error，如果明确知道抛异常就要回滚，建议设置为`@Transactional(rollbackFor = Throwable.class)`
<a name="IeMOQ"></a>
### 5）AOP不生效问题
其他诸如 MyISAM 不支持，es 不支持等等就不一一列举了。
