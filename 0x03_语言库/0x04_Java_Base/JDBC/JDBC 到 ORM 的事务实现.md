Java JDBC ORM
<a name="mnWUY"></a>
## 一、JDBC
早期SUN公司想编写一套可以连接天下所有数据库的API，但是当他们刚刚开始时就发现这是不可完成的任务，因为各个厂商的数据库服务器差异太大了。后来SUN开始与数据库厂商们讨论，最终得出的结论是，由SUN提供一套访问数据库的规范（就是一组接口），并提供连接数据库的协议标准，然后各个数据库厂商会遵循SUN的规范提供一套访问自己公司的数据库服务器的API出现。SUN提供的规范命名为JDBC，而各个厂商提供的，遵循了JDBC规范的，可以访问自己数据库的API被称之为驱动。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624373756178-c751ee0f-ca6d-4113-9997-49606484e373.webp#clientId=ufe628b52-f141-4&from=paste&id=u0d27a3d6&originHeight=359&originWidth=580&originalType=url&ratio=3&status=done&style=none&taskId=u17b33d42-f453-4499-94c9-4c3c0dbe5cd)<br />数据库连接池：<br />C3P0、DBCP-- Apache CommonPool、Druid、Hikari
<a name="qGXZp"></a>
## 二、ORM
<a name="YjNWB"></a>
### Hibernate
Hibernate 是一个开源的对象关系映射框架，它对JDBC 进行了非常轻量级的对象封装，它将 POJO 与数据库表建立映射关系，是一个全自动的 orm 框架，hibernate 可以自动生成 SQL 语句，自动执行，使得 Java 程序员可以使用面向对象的思维来操纵数据库。Hibernate 需要定义实体类和 hbm 映射关系文件（IDE 一般有工具生成）。Hibernate 可以使用 HQL、Criteria、Native SQL三种方式操作数据库。也可以作为 JPA 适配实现，使用 JPA 接口操作。
<a name="MHSZn"></a>
### Mybatis
MyBatis 是一款优秀的持久层框架，它支持定制化 SQL、存储过程以及高级映射。MyBatis避免了几乎所有的JDBC 代码和手动设置参数以及获取结果集。Mybatis 可以使用简单的XML或注解来配置和映射原生信息，将接口和 Java的POJOs(Plain Old Java Objects,普通的Java对象)映射成数据库中的记录
<a name="OWVvs"></a>
### Mybatis与Hibernate的区别？
Hibernate是全自动，Mybatis是半自动。<br />Mybatis

- 优点：原生SQL（XML语法），直观，容易优化
- 缺点：繁琐，可以用Mybatis-generator、Mybatis-Plus之类的插件弥补

Hibernate

- 优点：简单场景不用写SQL（HQL、Cretiria、SQL）
- 缺点：不好优化sql，对DBA不友好
<a name="uQOn5"></a>
## Spring管理事务
先看看JDBC如何操作事务？
```java
public void updatePrice() throws SQLException {
    try {
        Connection conn = getConnection();
        //关闭自动提交
        conn.setAutoCommit(false);
        String sql = "update goods set price =? where id=? ";
        PreparedStatement ptmt = conn.prepareStatement(sql); 
        ptmt.setDouble(1, 3500);
        ptmt.setString(2, "1");
        //执行
        ptmt.execute();
        //提交事务
        conn.commit();
    } catch (Exception e) {
        //回滚事务
        conn.rollback();
        e.printStackTrace();
    }
}
```
再看看Spring是如何无侵入的进行事务管理的？<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624373756352-2371da51-39d6-4963-aa03-bc96f15cdc31.png#clientId=ufe628b52-f141-4&from=paste&id=u65892c22&originHeight=475&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ue4654b25-5418-4605-a272-1482a3ba7ec)<br />实现原理：事务管理器+AOP
<a name="PTWs0"></a>
### 源码分析Spring事务实现过程
示例代码：<br />在`goodsService.updatePrice`方法上加了事务注解。
```java
@RequestMapping("/updateprice")
public String updateprice(Double price,Integer age){
    goodsService.updatePrice(1,price);
    int i=10/0;
    userService.updateAge(1,age);
    return "sucess";
}
```

1. 请求进入Controller，调用`goodsService`的时候，调用的实际上是`goodsService`的代理对象

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624373755931-5d9aa198-f879-4905-affc-a45f88225938.png#clientId=ufe628b52-f141-4&from=paste&id=u38eda322&originHeight=396&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ud908402f-a680-4462-b7f7-ecfc43f3792)

2. 到代理类的方法中`org.springframework.aop.framework.ReflectiveMethodInvocation#proceed`
3. 进入事务拦截器的方法

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624373756162-fc202b38-b57f-474d-896c-747866d99842.png#clientId=ufe628b52-f141-4&from=paste&id=u33694ebc&originHeight=452&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=uf584ba28-295c-474a-9ab8-b87bea66129)<br />里面有个`TransactionInterceptor`

4. `TransactionInterceptor`方法里面进行了事务管理
```java
protected Object invokeWithinTransaction(Method method, @Nullable Class<?> targetClass,
                                         final InvocationCallback invocation) throws Throwable {

    // If the transaction attribute is null, the method is non-transactional.
    TransactionAttributeSource tas = getTransactionAttributeSource();
    final TransactionAttribute txAttr = (tas != null ? tas.getTransactionAttribute(method, targetClass) : null);
    final PlatformTransactionManager tm = determineTransactionManager(txAttr);
    final String joinpointIdentification = methodIdentification(method, targetClass, txAttr);

    if (txAttr == null || !(tm instanceof CallbackPreferringPlatformTransactionManager)) {
        // Standard transaction demarcation with getTransaction and commit/rollback calls.
        TransactionInfo txInfo = createTransactionIfNecessary(tm, txAttr, joinpointIdentification);
        Object retVal = null;
        try {
            // This is an around advice: Invoke the next interceptor in the chain.
            // This will normally result in a target object being invoked.
            //执行目标方法
            retVal = invocation.proceedWithInvocation();
        }
        catch (Throwable ex) {
            //回滚
            exception
                completeTransactionAfterThrowing(txInfo, ex);
            throw ex;
        }
        finally {
            cleanupTransactionInfo(txInfo);
        }
        //提交事务
        commitTransactionAfterReturning(txInfo);
        return retVal;
    }
    ...
```
上述是简单场景的事务处理，如果是多个`service`方法，并且都加了`@Transactional`注解，那事务怎么算呢？那就需要学习Spring里的事务传播了。
```java
public Class ServiceA{

    @Transactional
    void methodA(){
        //....    
    }
}


public Class ServiceB{

    @Transactional
    void methodB(){
        //....    
    }
}
```
<a name="tP18c"></a>
### 七种事务的传播行为：

- `PROPAGATION_REQUIRED` (默认) 表示当前方法必须在一个具有事务的上下文中运行，如有客户端有事务在进行，那么被调用端将在该事务中运行，否则的话重新开启一个事务。
- `PROPAGATION_SUPPORTS` 表示当前方法不必须在一个具有事务的上下文中运行，如：`ServiceA.methodA()`调用`ServiceB.methodB()`，如果`methodA`方法上有事务，那么`methodB`加入他的事务，如果`methodA`上没有事务，那么`methodB`也不开事务
- `PROPAGATION_MANDATORY` 必须被开启事务的方法调用，否则报错
- `PROPAGATION_REQUIRES_NEW` 强制自己开启一个新的事务，如果一个事务已经存在，那么将这个事务挂起.如`ServiceA.methodA()`调用`ServiceB.methodB()`，`methodB()`上的传播级别是`PROPAGATION_REQUIRES_NEW`的话，那么如果`methodA`报错，不影响`methodB`的事务，如果`methodB`报错，那么`methodA`是可以选择是回滚或者提交的，就看是否将`methodB`报的错误抛出还是try catch了.
- `PROPAGATION_NOT_SUPPORTED` 总是非事务的执行，并且挂起任何事务.就是如果`methodA`方法执行到`methodB`这里了，`methodA`的事务就被挂起，然后`methodB`非事务的执行，然后等`methodB`方法运行结束，`methodA`的事务再继续.这个的好处就是`methodB`报错了不会让`methodA`回滚.
- `PROPAGATION_NEVER` 总是非事务地执行，如果存在一个活动事务，则抛出异常
- `PROPAGATION_NESTED` 表示如果当前方法正有一个事务在运行中，则该方法应该运行在一个嵌套事务中，被嵌套的事务可以独立于被封装的事务中进行提交或者回滚。如果封装事务存在，并且外层事务抛出异常回滚，那么内层事务必须回滚，反之，内层事务并不影响外层事务。如果封装事务不存在，则同`propagation`. `required`的一样
<a name="XNlnZ"></a>
### 事务失效的几个原因：

1. Spring的事务注解`@Transactional`只能放在public修饰的方法上才起作用，如果放在其他非`public`（`private`，`protected`）方法上，虽然不报错，但是事务不起作用
2. 如果采用Spring+SpringMVC，则`context:component-scan`重复扫描问题可能会引起事务失败。如果spring和mvc的配置文件中都扫描了`service`层，那么事务就会失效。<br />原因：因为按照Spring配置文件的加载顺序来讲，先加载SpringMVC配置文件，再加载Spring配置文件，事物一般都在srping配置文件中进行配置，如果此时在加载srpingMVC配置文件的时候，把service也给注册了，但是此时事物还没加载，也就导致后面的事物无法成功注入到`service`中。所以把对`service`的扫描放在Spring配置文件中或是其他配置文件中。
3. 如使用mysql且引擎是MyISAM，则事务会不起作用，原因是MyISAM不支持事务，可以改成InnoDB引擎
4. `@Transactional`注解开启配置，必须放到`listener`里加载，如果放到`DispatcherServlet`的配置里，事务也是不起作用的。
5. Spring团队建议在具体的类（或类的方法）上使用 `@Transactional` 注解，而不要使用在类所要实现的任何接口上。在接口上使用 `@Transactional`注解，只能当设置了基于接口的代理时它才生效。因为注解是不能继承 的，这就意味着如果正在使用基于类的代理时，那么事务的设置将不能被基于类的代理所识别，而且对象也将不会被事务代理所包装。
6. 在业务代码中如果抛出`RuntimeException`异常，事务回滚；但是抛出`Exception`，事务不回滚；默认对`RuntimeException`回滚
7. 如果在加有事务的方法内，使用了try...catch..语句块对异常进行了捕获，而catch语句块没有`throw new RuntimeExecption`异常，事务也不会回滚
8. 在类A里面有方法a 和方法b， 然后方法b上面用 `@Transactional`加了方法级别的事务，在方法a里面 调用了方法b，方法b里面的事务不会生效。原因是在同一个类之中，方法互相调用，切面无效，而不仅仅是事务。这里事务之所以无效，是因为Spring的事务是通过aop实现的。

代码示例：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624373757086-d98a3fa7-b118-409f-b454-106bdfc139e7.webp#clientId=ufe628b52-f141-4&from=paste&id=u674a95e5&originHeight=697&originWidth=924&originalType=url&ratio=3&status=done&style=shadow&taskId=uecc3a309-b10d-4796-a9de-058df8e566e)<br />可以看出这个`this`，并不是代理对象，事务也就不能生效了。
