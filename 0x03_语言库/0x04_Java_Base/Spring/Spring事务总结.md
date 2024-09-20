Spring 事务
<a name="yxJpo"></a>
## 一、Spring事务管理的几种方式
Spring事务在具体使用方式上可分为两大类：
<a name="R97Hl"></a>
### 1.  声明式

- 基于 `TransactionProxyFactoryBean`的声明式事务管理
- 基于 `<tx>` 和 `<aop>` 命名空间的事务管理
- 基于 `@Transactional` 的声明式事务管理
<a name="nxrzu"></a>
### 2.  编程式

- 基于事务管理器API 的编程式事务管理
- 基于`TransactionTemplate` 的编程式事务管理

目前大部分项目使用的是声明式的后两种：

- 基于 `<tx>` 和 `<aop>` 命名空间的声明式事务管理可以充分利用切点表达式的强大支持，使得管理事务更加灵活。
- 基于 `@Transactional` 的方式需要实施事务管理的方法或者类上使用 `@Transactional` 指定事务规则即可实现事务管理，在Spring Boot中通常也建议使用这种注解方式来标记事务。
<a name="3FftB"></a>
## 二、Spring事务实现机制
详细看下Spring事务的源代码，进而了解其工作原理。从`<tx>`标签的解析类开始：
```java
@Override
public void init() {
        registerBeanDefinitionParser("advice", new TxAdviceBeanDefinitionParser());
        registerBeanDefinitionParser("annotation-driven", new AnnotationDrivenBeanDefinitionParser());
        registerBeanDefinitionParser("jta-transaction-manager", new JtaTransactionManagerBeanDefinitionParser());
    }
}
```
```java
class TxAdviceBeanDefinitionParser extends AbstractSingleBeanDefinitionParser {
    @Override
    protected Class<?> getBeanClass(Element element) {
        return TransactionInterceptor.class;
    }
}
```
由此可看到Spring事务的核心实现类`TransactionInterceptor`及其父类`TransactionAspectSupport`，其实现了事务的开启、数据库操作、事务提交、回滚等。平时在开发时如果想确定是否在事务中，也可以在该方法进行断点调试。<br />`**TransactionInterceptor**`**：**
```java
public Object invoke(final MethodInvocation invocation) throws Throwable {
    Class<?> targetClass = (invocation.getThis() != null ? AopUtils.getTargetClass(invocation.getThis()) : null);
    // Adapt to TransactionAspectSupport's invokeWithinTransaction...
    return invokeWithinTransaction(invocation.getMethod(), targetClass, new InvocationCallback() {
        @Override
        public Object proceedWithInvocation() throws Throwable {
            return invocation.proceed();
        }
    });
}
```
`**TransactionAspectSupport**`
```java
protected Object invokeWithinTransaction(Method method, Class<?> targetClass, final InvocationCallback invocation)
    throws Throwable {
    // If the transaction attribute is null, the method is non-transactional.
    final TransactionAttribute txAttr = getTransactionAttributeSource().getTransactionAttribute(method, targetClass);
    final PlatformTransactionManager tm = determineTransactionManager(txAttr);
    final String joinpointIdentification = methodIdentification(method, targetClass, txAttr);
    if (txAttr == null || !(tm instanceof CallbackPreferringPlatformTransactionManager)) {
        // Standard transaction demarcation with getTransaction and commit/rollback calls.
        TransactionInfo txInfo = createTransactionIfNecessary(tm, txAttr, joinpointIdentification);
        Object retVal = null;
        try {
            // This is an around advice: Invoke the next interceptor in the chain.
            // This will normally result in a target object being invoked.
            retVal = invocation.proceedWithInvocation();
        }
        catch (Throwable ex) {
            // target invocation exception
            completeTransactionAfterThrowing(txInfo, ex);
            throw ex;
        }
        finally {
            cleanupTransactionInfo(txInfo);
        }
        commitTransactionAfterReturning(txInfo);
        return retVal;
    }
}
```
至此了解事务的整个调用流程，但还有一个重要的机制没分析到，那就是Spring 事务针对不同的传播级别控制当前获取的数据库连接。接下来看下Spring获取连接的工具类DataSourceUtils，JdbcTemplate、Mybatis-Spring也都是通过该类获取Connection。
```java
public abstract class DataSourceUtils {
    …
   public static Connection getConnection(DataSource dataSource) throws CannotGetJdbcConnectionException {
        try {
            return doGetConnection(dataSource);
        }
        catch (SQLException ex) {
            throw new CannotGetJdbcConnectionException("Could not get JDBC Connection", ex);
        }
    }
    public static Connection doGetConnection(DataSource dataSource) throws SQLException {
        Assert.notNull(dataSource, "No DataSource specified");
        ConnectionHolder conHolder = (ConnectionHolder) TransactionSynchronizationManager.getResource(dataSource);
        if (conHolder != null && (conHolder.hasConnection() || conHolder.isSynchronizedWithTransaction())) {
            conHolder.requested();
            if (!conHolder.hasConnection()) {
                logger.debug("Fetching resumed JDBC Connection from DataSource");
                conHolder.setConnection(dataSource.getConnection());
            }
            return conHolder.getConnection();
        }
    …
}
```
`TransactionSynchronizationManager`也是一个事务同步管理的核心类，它实现了事务同步管理的职能，包括记录当前连接持有connection holder。<br />`**TransactionSynchronizationManager**`
```java
private static final ThreadLocal<Map<Object, Object>> resources =
    new NamedThreadLocal<Map<Object, Object>>("Transactional resources");
	…
	public static Object getResource(Object key) {
        Object actualKey = TransactionSynchronizationUtils.unwrapResourceIfNecessary(key);
        Object value = doGetResource(actualKey);
        if (value != null && logger.isTraceEnabled()) {
            logger.trace("Retrieved value [" + value + "] for key [" + actualKey + "] bound to thread [" +
                    Thread.currentThread().getName() + "]");
        }
        return value;
    }
    /**
     * Actually check the value of the resource that is bound for the given key.
     */
    private static Object doGetResource(Object actualKey) {
        Map<Object, Object> map = resources.get();
        if (map == null) {
            return null;
        }
        Object value = map.get(actualKey);
        // Transparently remove ResourceHolder that was marked as void...
        if (value instanceof ResourceHolder && ((ResourceHolder) value).isVoid()) {
            map.remove(actualKey);
            // Remove entire ThreadLocal if empty...
            if (map.isEmpty()) {
                resources.remove();
            }
            value = null;
        }
        return value;
    }
```
在事务管理器类`AbstractPlatformTransactionManager`中，`getTransaction`获取事务时，会处理不同的事务传播行为，例如当前存在事务，但调用方法事务传播级别为`REQUIRES_NEW`、`PROPAGATION_NOT_SUPPORTED`时，对当前事务进行挂起、恢复等操作，以此保证了当前数据库操作获取正确的`Connection`。<br />具体是在子事务提交的最后会将挂起的事务恢复，恢复时重新调用`TransactionSynchronizationManager`. bindResource设置之前的connection holder，这样再获取的连接就是被恢复的数据库连接， `TransactionSynchronizationManager`当前激活的连接只能是一个。<br />`**AbstractPlatformTransactionManager**`
```java
private TransactionStatus handleExistingTransaction(
    TransactionDefinition definition, Object transaction, boolean debugEnabled)
    throws TransactionException {
    …
        if (definition.getPropagationBehavior() == TransactionDefinition.PROPAGATION_REQUIRES_NEW) {
            if (debugEnabled) {
                logger.debug("Suspending current transaction, creating new transaction with name [" +
                             definition.getName() + "]");
            }
            SuspendedResourcesHolder suspendedResources = suspend(transaction);
            try {
                boolean newSynchronization = (getTransactionSynchronization() != SYNCHRONIZATION_NEVER);
                DefaultTransactionStatus status = newTransactionStatus(
                    definition, transaction, true, newSynchronization, debugEnabled, suspendedResources);
                doBegin(transaction, definition);
                prepareSynchronization(status, definition);
                return status;
            }
            catch (RuntimeException beginEx) {
                resumeAfterBeginException(transaction, suspendedResources, beginEx);
                throw beginEx;
            }
            catch (Error beginErr) {
                resumeAfterBeginException(transaction, suspendedResources, beginErr);
                throw beginErr;
            }
        }
    /**
     * Clean up after completion, clearing synchronization if necessary,
     * and invoking doCleanupAfterCompletion.
     * @param status object representing the transaction
     * @see #doCleanupAfterCompletion
     */
    private void cleanupAfterCompletion(DefaultTransactionStatus status) {
        status.setCompleted();
        if (status.isNewSynchronization()) {
            TransactionSynchronizationManager.clear();
        }
        if (status.isNewTransaction()) {
            doCleanupAfterCompletion(status.getTransaction());
        }
        if (status.getSuspendedResources() != null) {
            if (status.isDebug()) {
                logger.debug("Resuming suspended transaction after completion of inner transaction");
            }
            resume(status.getTransaction(), (SuspendedResourcesHolder) status.getSuspendedResources());
        }
    }
```
Spring的事务是通过AOP代理类中的一个Advice（TransactionInterceptor）进行生效的，而传播级别定义了事务与子事务获取连接、事务提交、回滚的具体方式。<br />AOP（Aspect Oriented Programming），即面向切面编程。Spring AOP技术实现上其实就是代理类，具体可分为静态代理和动态代理两大类，其中静态代理是指使用 AOP 框架提供的命令进行编译，从而在编译阶段就可生成 AOP 代理类，因此也称为编译时增强；(AspectJ)；而动态代理则在运行时借助于 默写类库在内存中“临时”生成 AOP 动态代理类，因此也被称为运行时增强。其中java是使用的动态代理模式 (JDK+CGLIB)。<br />**JDK动态代理** JDK动态代理主要涉及到java.lang.reflect包中的两个类：Proxy和InvocationHandler。InvocationHandler是一个接口，通过实现该接口定义横切逻辑，并通过反射机制调用目标类的代码，动态将横切逻辑和业务逻辑编制在一起。Proxy利用InvocationHandler动态创建一个符合某一接口的实例，生成目标类的代理对象。<br />**CGLIB动态代理** CGLIB全称为Code Generation Library，是一个强大的高性能，高质量的代码生成类库，可以在运行期扩展Java类与实现Java接口，CGLIB封装了asm，可以再运行期动态生成新的class。和JDK动态代理相比较：JDK创建代理有一个限制，就是只能为接口创建代理实例，而对于没有通过接口定义业务方法的类，则可以通过CGLIB创建动态代理。<br />CGLIB 创建代理的速度比较慢，但创建代理后运行的速度却非常快，而 JDK 动态代理正好相反。如果在运行的时候不断地用 CGLIB 去创建代理，系统的性能会大打折扣。因此如果有接口，Spring默认使用JDK 动态代理，源代码如下：
```java
public class DefaultAopProxyFactory implements AopProxyFactory, Serializable {
    @Override
    public AopProxy createAopProxy(AdvisedSupport config) throws AopConfigException {
        if (config.isOptimize() || config.isProxyTargetClass() || hasNoUserSuppliedProxyInterfaces(config)) {
            Class<?> targetClass = config.getTargetClass();
            if (targetClass == null) {
                throw new AopConfigException("TargetSource cannot determine target class: " +
                        "Either an interface or a target is required for proxy creation.");
            }
            if (targetClass.isInterface() || Proxy.isProxyClass(targetClass)) {
                return new JdkDynamicAopProxy(config);
            }
            return new ObjenesisCGLIBAopProxy(config);
        }   
        else {
            return new JdkDynamicAopProxy(config);
        }
    }
}
```
在了解Spring代理的两种特点后，也就知道在做事务切面配置时的一些注意事项，例如JDK代理时方法必须是public，CGLIB代理时必须是public、protected，且类不能是final的；在依赖注入时，如果属性类型定义为实现类，JDK代理时会报如下注入异常：
```
org.springframework.beans.factory.UnsatisfiedDependencyException: Error creating bean with name 'com.wwb.test.TxTestAop': Unsatisfied dependency expressed through field 'service'; nested exception is org.springframework.beans.factory.BeanNotOfRequiredTypeException: Bean named 'stockService' is expected to be of type 'com.wwb.service.StockProcessServiceImpl' but was actually of type 'com.sun.proxy.$Proxy14'
```
但如果修改为CGLIB代理时则会成功注入，所以如果有接口，建议注入时该类属性都定义为接口。另外事务切点都配置在实现类和接口都可以生效，但建议加在实现类上。<br />官网关于Spring AOP的详细介绍
> [https://docs.spring.io/spring/docs/current/spring-framework-reference/core.html%23aop](https://docs.spring.io/spring/docs/current/spring-framework-reference/core.html%2523aop)

<a name="AmJch"></a>
## 三、Spring事务的那些坑
<a name="F8B3U"></a>
### 3.1 事务不生效
测试代码，事务AOP配置：
```xml
<tx:advice id="txAdvice" transaction-manager="myTxManager">
  <tx:attributes>
    <!-- 指定在连接点方法上应用的事务属性 -->
    <tx:method name="openAccount" isolation="DEFAULT" propagation="REQUIRED"/>
    <tx:method name="openStock" isolation="DEFAULT" propagation="REQUIRED"/>
    <tx:method name="openStockInAnotherDb" isolation="DEFAULT" propagation="REQUIRES_NEW"/>
    <tx:method name="openTx" isolation="DEFAULT" propagation="REQUIRED"/>
    <tx:method name="openWithoutTx" isolation="DEFAULT" propagation="NEVER"/>
    <tx:method name="openWithMultiTx" isolation="DEFAULT" propagation="REQUIRED"/>
  </tx:attributes>
</tx:advice>
```
```java
public class StockProcessServiceImpl implements IStockProcessService{
    @Autowired
    private IAccountDao accountDao;
    @Autowired
    private IStockDao stockDao;

    @Override
    public void openAccount(String aname, double money) {
        accountDao.insertAccount(aname, money);
    }
    @Override
    public void openStock(String sname, int amount) {
        stockDao.insertStock(sname, amount);
    }

    @Override
    public void openStockInAnotherDb(String sname, int amount) {
        stockDao.insertStock(sname, amount);
    }
}
public void insertAccount(String aname, double money) {
    String sql = "insert into account(aname, balance) values(?,?)";
    this.getJdbcTemplate().update(sql, aname, money);
    DbUtils.printDBConnectionInfo("insertAccount",getDataSource());
} 
public void insertStock(String sname, int amount) {
    String sql = "insert into stock(sname, count) values (?,?)";
    this.getJdbcTemplate().update(sql , sname, amount);
    DbUtils.printDBConnectionInfo("insertStock",getDataSource());
}
public static void printDBConnectionInfo(String methodName,DataSource ds) {
    Connection connection = DataSourceUtils.getConnection(ds);
    System.out.println(methodName+" connection hashcode="+connection.hashCode());
}
```
```java
//调用同类方法，外围配置事务
public void openTx(String aname, double money) {
    openAccount(aname,money);
    openStock(aname,11);
}
```
1.运行输出：
> insertAccount connection hashcode=319558327
> insertStock connection hashcode=319558327

```java
//调用同类方法，外围未配置事务
public void openWithoutTx(String aname, double money) {
    openAccount(aname,money);
    openStock(aname,11);
}
```
2.运行输出：
> insertAccount connection hashcode=1333810223
> insertStock connection hashcode=1623009085

```java
//通过AopContext.currentProxy()方法获取代理
@Override
public void openWithMultiTx(String aname, double money) {
    openAccount(aname,money);  
    openStockInAnotherDb(aname, 11);//传播级别为REQUIRES_NEW
}
```
3.运行输出：
> insertAccount connection hashcode=303240439
> insertStock connection hashcode=303240439

可以看到2、3测试方法跟事务预期的一样，结论：**调用方法未配置事务、本类方法直接调用，事务都不生效！**<br />究其原因，还是因为Spring的事务本质上是个代理类，而本类方法直接调用时其对象本身并不是织入事务的代理，所以事务切面并未生效。<br />Spring也提供了判断是否为代理的方法：
```java
public static void printProxyInfo(Object bean) {
    System.out.println("isAopProxy"+AopUtils.isAopProxy(bean));
    System.out.println("isCGLIBProxy="+AopUtils.isCGLIBProxy(bean));
    System.out.println("isJdkProxy="+AopUtils.isJdkDynamicProxy(bean));
}
```
那如何修改为代理类调用呢？最直接的想法是注入自身，代码如下：
```java
@Autowired
private IStockProcessService stockProcessService;
//注入自身类，循环依赖，亲测可以 
public void openTx(String aname, double money) {
    stockProcessService.openAccount(aname,money);
    stockProcessService.openStockInAnotherDb (aname,11);
}
```
当然Spring提供了获取当前代理的方法：代码如下：
```java
//通过AopContext.currentProxy()方法获取代理
@Override
public void openWithMultiTx(String aname, double money) {
    ((IStockProcessService)AopContext.currentProxy()).openAccount(aname,money);
    ((IStockProcessService)AopContext.currentProxy()).openStockInAnotherDb(aname, 11);
}
```
另外Spring是通过TransactionSynchronizationManager类中线程变量来获取事务中数据库连接，所以如果是**多线程调用**或者**绕过Spring获取数据库连接**，都会导致Spring事务配置失效。<br />最后Spring事务配置失效的场景：

1. 事务切面未配置正确
2. 本类方法调用
3. 多线程调用
4. 绕开Spring获取数据库连接
<a name="MiKDg"></a>
### 3.2 事务不回滚
测试代码：
```xml
<tx:advice id="txAdvice" transaction-manager="myTxManager">
  <tx:attributes>
    <!-- 指定在连接点方法上应用的事务属性 -->
    <tx:method name="buyStock" isolation="DEFAULT" propagation="REQUIRED"/>
  </tx:attributes>
</tx:advice>
```
```java
public void buyStock(String aname, double money, String sname, int amount) throws StockException {
    boolean isBuy = true;
    accountDao.updateAccount(aname, money, isBuy);
    // 故意抛出异常
    if (true) {
        throw new StockException("购买股票异常");
    }
    stockDao.updateStock(sname, amount, isBuy);
}
```
```java
@Test
public void testBuyStock() {
    try {
        service.openAccount("dcbs", 10000);
        service.buyStock("dcbs", 2000, "dap", 5);
    } catch (StockException e) {
        e.printStackTrace();
    }
    double accountBalance = service.queryAccountBalance("dcbs");
    System.out.println("account balance is " + accountBalance);
}
```
输出结果：
> insertAccount connection hashcode=656479172
> updateAccount connection hashcode=517355658
> account balance is 8000.0

应用抛出异常，但accountDao.updateAccount却进行了提交。究其原因，直接看Spring源代码：<br />**TransactionAspectSupport**
```java
protected void completeTransactionAfterThrowing(TransactionInfo txInfo, Throwable ex) {
    if (txInfo != null && txInfo.hasTransaction()) {
        if (logger.isTraceEnabled()) {
            logger.trace("Completing transaction for [" + txInfo.getJoinpointIdentification() +
                         "] after exception: " + ex);
        }
        if (txInfo.transactionAttribute.rollbackOn(ex)) {
            try {
                txInfo.getTransactionManager().rollback(txInfo.getTransactionStatus());
            }
            catch (TransactionSystemException ex2) {
                logger.error("Application exception overridden by rollback exception", ex);
                ex2.initApplicationException(ex);
                throw ex2;
            }
        …
        }
        public class DefaultTransactionAttribute extends DefaultTransactionDefinition implements TransactionAttribute {
            @Override
            public boolean rollbackOn(Throwable ex) {
                return (ex instanceof RuntimeException || ex instanceof Error);
            }
            …
        }
```
由代码可见，**Spring事务默认只对RuntimeException和Error进行回滚**，如果应用需要对指定的异常类进行回滚，可配置rollback-for=属性，例如：
```xml
<!-- 注册事务通知 -->
<tx:advice id="txAdvice" transaction-manager="myTxManager">
  <tx:attributes>
    <!-- 指定在连接点方法上应用的事务属性 -->
    <tx:method name="buyStock" isolation="DEFAULT" propagation="REQUIRED" rollback-for="StockException"/>
  </tx:attributes>
</tx:advice>
```
事务不回滚的原因：

1. 事务配置切面未生效
2. 应用方法中将异常捕获
3. 抛出的异常不属于运行时异常（例如IOException），
4. rollback-for属性配置不正确
<a name="3Tjra"></a>
### 3.3 事务超时不生效
测试代码：
```xml
<!-- 注册事务通知 -->
<tx:advice id="txAdvice" transaction-manager="myTxManager">
  <tx:attributes>
    <tx:method name="openAccountForLongTime" isolation="DEFAULT" propagation="REQUIRED" timeout="3"/>
  </tx:attributes>
</tx:advice>
```
```java
@Override
public void openAccountForLongTime(String aname, double money) {
    accountDao.insertAccount(aname, money);
    try {
        Thread.sleep(5000L);//在数据库操作之后超时
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
}
```
```java
@Test
public void testTimeout() {
    service.openAccountForLongTime("dcbs", 10000);
}
```
正常运行，事务超时未生效
```java
public void openAccountForLongTime(String aname, double money) {
    try {
        Thread.sleep(5000L); //在数据库操作之前超时
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    accountDao.insertAccount(aname, money);
}
```
抛出事务超时异常，超时生效
> org.springframework.transaction.TransactionTimedOutException: Transaction timed out: deadline was Fri Nov 23 17:03:02 CST 2018
> at org.springframework.transaction.support.ResourceHolderSupport.checkTransactionTimeout(ResourceHolderSupport.java:141)
> …

通过源码看看Spring事务超时的判断机制：<br />**ResourceHolderSupport**
```java
/**
  * Return the time to live for this object in milliseconds.
  * @return number of millseconds until expiration
  * @throws TransactionTimedOutException if the deadline has already been reached
  */
public long getTimeToLiveInMillis() throws TransactionTimedOutException{
    if (this.deadline == null) {
        throw new IllegalStateException("No timeout specified for this resource holder");
    }
    long timeToLive = this.deadline.getTime() - System.currentTimeMillis();
    checkTransactionTimeout(timeToLive <= 0);
    return timeToLive;
}
/**
 * Set the transaction rollback-only if the deadline has been reached,
 * and throw a TransactionTimedOutException.
 */
private void checkTransactionTimeout(boolean deadlineReached) throws TransactionTimedOutException {
    if (deadlineReached) {
        setRollbackOnly();
        throw new TransactionTimedOutException("Transaction timed out: deadline was " + this.deadline);
    }
}
```
通过查看getTimeToLiveInMillis方法的Call Hierarchy，可以看到被DataSourceUtils的applyTimeout所调用， 继续看applyTimeout的Call Hierarchy，可以看到有两处调用，一个是JdbcTemplate，一个是TransactionAwareInvocationHandler类，后者是只有TransactionAwareDataSourceProxy类调用，该类为DataSource的事务代理类，一般并不会用到。难道超时只能在这调用JdbcTemplate中生效？写代码亲测：
```xml
<!-- 注册事务通知 -->
<tx:advice id="txAdvice" transaction-manager="myTxManager">
  <tx:attributes>
    <tx:method name="openAccountForLongTimeWithoutJdbcTemplate" isolation="DEFAULT" propagation="REQUIRED" timeout="3"/>
  </tx:attributes>
</tx:advice>
```
```java
public void openAccountForLongTimeWithoutJdbcTemplate(String aname, double money) {
    try {
        Thread.sleep(5000L);
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    accountDao.queryAccountBalanceWithoutJdbcTemplate(aname);
}
public double queryAccountBalanceWithoutJdbcTemplate(String aname) {
    String sql = "select balance from account where aname = ?";
    PreparedStatement prepareStatement;
    try {
        prepareStatement = this.getConnection().prepareStatement(sql);
        prepareStatement.setString(1, aname);
        ResultSet executeQuery = prepareStatement.executeQuery();
        while(executeQuery.next()) {
            return executeQuery.getDouble(1);
        }
    } catch (CannotGetJdbcConnectionException | SQLException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }
    return 0;
}
```
> 运行正常，事务超时失效

由上可见：**Spring事务超时判断在通过JdbcTemplate的数据库操作时，所以如果超时后未有JdbcTemplate方法调用，则无法准确判断超时**。另外也可以得知，如果**通过Mybatis等操作数据库，Spring的事务超时是无效的**。鉴于此，Spring的事务超时谨慎使用。
<a name="GAEY4"></a>
## 四、 总结
JDBC规范中Connection 的setAutoCommit是原生控制手动事务的方法，但传播行为、异常回滚、连接管理等很多技术问题都需要开发者自己处理，而Spring事务通过AOP方式非常优雅的屏蔽了这些技术复杂度，使得事务管理变的异常简单。<br />但凡事有利弊，如果对实现机制理解不透彻，很容易掉坑里。最后总结下**Spring事务的可能踩的坑：**
<a name="UWW8B"></a>
### 4.1 Spring事务未生效

- 调用方法本身未正确配置事务
- 本类方法直接调用
- 数据库操作未通过Spring的DataSourceUtils获取Connection
- 多线程调用
<a name="xEcco"></a>
### 4.2 Spring事务回滚失效

- 未准确配置rollback-for属性
- 异常类不属于RuntimeException与Error
- 应用捕获了异常未抛出
<a name="DvlFi"></a>
### 4.3 Spring事务超时不准确或失效

- 超时发生在最后一次JdbcTemplate操作之后
- 通过非JdbcTemplate操作数据库，例如Mybatis
