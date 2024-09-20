项目的代码里有很多`@Transactional(readOnly = true)`，用过的同学都说`@Transactional(readOnly = true)`提高了性能。先思考以下几点：

- `@Transactional(readOnly = true)`是如何工作的，为什么使用它可以提高性能？
- 当使用 JPA 时，是否应该总是将`@Transactional(readOnly = true)`添加到服务层的只读方法？有什么取舍吗？

在开始之前，使用 Hibernate 来实现 JPA。
<a name="hZAYV"></a>
### 1、`@Transactional(readOnly = true)`是如何工作的，为什么使用它可以提高性能？
首先看一下事务接口。
```java
/**
* A boolean flag that can be set to {@code true} if the transaction is
* effectively read-only, allowing for corresponding optimizations at runtime.
* <p>Defaults to {@code false}.
* <p>This just serves as a hint for the actual transaction subsystem;
* it will <i>not necessarily</i> cause failure of write access attempts.
* A transaction manager which cannot interpret the read-only hint will
* <i>not</i> throw an exception when asked for a read-only transaction
* but rather silently ignore the hint.
* @see org.springframework.transaction.interceptor.TransactionAttribute#isReadOnly()
* @see org.springframework.transaction.support.TransactionSynchronizationManager#isCurrentTransactionReadOnly()
*/
boolean readOnly() default false;
```
可以看到 `readOnly = true` 选项允许优化。事务管理器将使用只读选项作为提示。看看用于事务管理器的`JpaTransactionManager`。
```java
@Override
protected void doBegin(Object transaction, TransactionDefinition definition) {
    JpaTransactionObject txObject = (JpaTransactionObject) transaction;
    // .
    // Delegate to JpaDialect for actual transaction begin.
    int timeoutToUse = determineTimeout(definition);
    Object transactionData = getJpaDialect().beginTransaction(em,
                                                              new JpaTransactionDefinition(definition, timeoutToUse, txObject.isNewEntityManagerHolder()));
    //...
}
```
在`JpaTransactionManager`中，`doBegin`方法委托`JpaDialect`来开始实际的事务，并在`JpaDialect`中调用`beginTransaction`。来看看`HibernateJpaDialect`类。
```java
@Override
public Object beginTransaction(EntityManager entityManager, TransactionDefinition definition)
throws PersistenceException, SQLException, TransactionException {
    // ...
    // Adapt flush mode and store previous isolation level, if any.
    FlushMode previousFlushMode = prepareFlushMode(session, definition.isReadOnly());
    if (definition instanceof ResourceTransactionDefinition &&
        ((ResourceTransactionDefinition) definition).isLocalResource()) {
        // As of 5.1, we explicitly optimize for a transaction-local EntityManager,
        // aligned with native HibernateTransactionManager behavior.
        previousFlushMode = null;
        if (definition.isReadOnly()) {
            session.setDefaultReadOnly(true);
        }
    }
    // ...
}

protected FlushMode prepareFlushMode(Session session, boolean readOnly) throws PersistenceException {
    FlushMode flushMode = session.getHibernateFlushMode();
    if (readOnly) {
        // We should suppress flushing for a read-only transaction.
        if (!flushMode.equals(FlushMode.MANUAL)) {
            session.setHibernateFlushMode(Flusode.MANUAL);
            return flushMode;
        }
    }
    else {
        // We need AUTO or COMMIT for a non-read-only transaction.
        if (flushMode.lessThan(FlushMode.COMMIT)) {
            session.setHibernateFlushMode(FlushMode.AUTO);
            return flushMode;
        }
    }
    // No FlushMode change needed...
    return null;
}
```
在`JpaDialect`中，可以看到`JpaDialect`使用只读选项准备刷新模式。当 `readOnly = true` 时， `JpaDialect` 禁止刷新。此外，还可以看到，在准备刷新模式后，`session.setDefaultReadOnly(true)`将session的readOnly属性设置为true。
```java
/**
 * Change the default for entities and proxies loaded into this session
 * from modifiable to read-only mode, or from modifiable to read-only mode.
 *
 * Read-only entities are not dirty-checked and snapshots of persistent
 * state are not maintained. Read-only entities can be modified, but
 * changes are not persisted.
 *
 * When a proxy is initialized, the loaded entity will have the same
 * read-only/modifiable setting as the uninitialized
 * proxy has, regardless of the session's current setting.
 *
 * To change the read-only/modifiable setting for a particular entity
 * or proxy that is already in this session:
 * @see Session#setReadOnly(Object,boolean)
 *
 * To override this session's read-only/modifiable setting for entities
 * and proxies loaded by a Query:
 * @see Query#setReadOnly(boolean)
 *
 * @param readOnly true, the default for loaded entities/proxies is read-only;
 *                 false, the default for loaded entities/proxies is modifiable
 */
void setDefaultReadOnly(boolean readOnly);
```
在Session接口中，通过将`readOnly`属性设置为true，将不会对只读实体进行脏检查，也不会维护持久状态的快照。此外，只读实体的更改也不会持久化。<br />总而言之，这些是在 Hibernate 中使用`@Transactional(readOnly = true)`所得到的结果

- 性能改进：只读实体不进行脏检查
- 节省内存：不维护持久状态的快照
- 数据一致性：只读实体的更改不会持久化
- 当使用主从或读写副本集（或集群）时，`@Transactional(readOnly = true)`使我们能够连接到只读数据库
<a name="xR64k"></a>
### 2、当使用 JPA 时，是否应该总是将`@Transactional(readOnly = true)`添加到服务层的只读方法？有什么取舍吗？
当使用`@Transactional(readOnly = true)`时，可以有很多优势。但是，将`@Transactional(readOnly = true)`添加到服务层的只读方法是否合适？以下是担心的事情

1. 无限制地使用事务可能会导致数据库死锁、性能和吞吐量下降。
2. 由于一个事务占用一个DB连接，所以`@Transactional(readOnly = true)`添加到Service层的方法可能会导致DB连接饥饿。

第一个问题很难重现，所以做了一些测试来检查第二个问题。
```java
@Transactional(readOnly = true)
public List<UserDto> transactionalReadOnlyOnService(){
    List<UserDto> userDtos = userRepository.findAll().stream()
            .map(userMapper::toDto)
            .toList();
    timeSleepAndPrintConnection();
    return userDtos;
}

public List<UserDto> transactionalReadOnlyOnRepository(){
    List<UserDto> userDtos = userRepository.findAll().stream()
            .map(userMapper::toDto)
            .toList();
    timeSleepAndPrintConnection();
    return userDtos;
}
```
在服务层测试了两个方法，一个是`@Transactional(readOnly = true)`，另一个是存储库层中的`@Transactional (readOnly = true)`（在 `SimpleJpaRepository` 中，它是 Jpa Respitory 的默认实现，在类的顶部有`@Transformational(readOnly = true)`，因此 `findAll()`方法在默认情况下有`@Transformational(readOnly = true)`。<br />从DB中获取userInfo并保持线程5秒钟，然后检查该方法何时释放连接。<br />结果如下：<br />对于服务层方法中的`@Transactional(readOnly = true)`，
```
activeConnections:0, IdleConnections:10, TotalConnections:10
start transactionalReadOnlyOnService!!
Hibernate: 
    select
        u1_0.id,
        u1_0.email,
        u1_0.name,
        u1_0.profile_file_name 
    from
        users u1_0
activeConnections:1, IdleConnections:9, TotalConnections:10
activeConnections:1, IdleConnections:9, TotalConnections:10
activeConnections:1, IdleConnections:9, TotalConnections:10
activeConnections:1, IdleConnections:9, TotalConnections:10
activeConnections:1, IdleConnections:9, TotalConnections:10
end transactionalReadOnlyOnService!!
activeConnections:0, IdleConnections:10, TotalConnections:10
```
对于存储库层方法中的`@Transactional(readOnly = true)`，
```
activeConnections:0, IdleConnections:10, TotalConnections:10
start transactionalReadOnlyOnRepository!!
Hibernate: 
    select
        u1_0.id,
        u1_0.email,
        u1_0.name,
        u1_0.profile_file_name 
    from
        users u1_0
activeConnections:0, IdleConnections:10, TotalConnections:10
activeConnections:0, IdleConnections:10, TotalConnections:10
activeConnections:0, IdleConnections:10, TotalConnections:10
activeConnections:0, IdleConnections:10, TotalConnections:10
activeConnections:0, IdleConnections:10, TotalConnections:10
end transactionalReadOnlyOnRepository!!
activeConnections:0, IdleConnections:10, TotalConnections:10
```
正如看到的，`@Transactional(readOnly = true)`一旦查询结果到达，存储库层就会释放连接。<br />然而，`@Transactional(readOnly = true)`在服务层的方法中直到服务层的方法结束才释放连接。<br />因此，当服务层的方法有需要大量时间的逻辑时要小心，因为它可以长时间持有数据库连接，这可能会导致数据库连接匮乏。
<a name="Vmzba"></a>
### 3、回顾
很明显，`@Transactional(readOnly = true)`有很多优点。

- 性能改进：只读实体不进行脏检查
- 节省内存：不维护持久状态的快照
- 数据一致性：只读实体的更改不会持久化
- 当使用主从或读写副本集（或集群）时，`@Transactional(readOnly = true)`使我们能够连接到只读数据库

但是，您还应该记住，`@Transactional(readOnly = true)`在服务层的方法中可能会导致数据库死锁、性能低下和数据库连接匮乏！<br />当需要将只读查询仅仅作为一个事务执行时，请毫不犹豫选择的在服务层的方法中使用`@Transactional(readOnly = true)`，如果你的服务层的方法中有大量其他逻辑方法时，就要做取舍了！
