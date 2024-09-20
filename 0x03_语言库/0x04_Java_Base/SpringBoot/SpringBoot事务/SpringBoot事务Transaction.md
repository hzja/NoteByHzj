Java Spring 事务
<a name="Hufxq"></a>
## 1、Transaction
<a name="CqiHM"></a>
### 1.1 事务管理方式
在Spring中，事务有两种实现方式，分别是编程式事务管理和声明式事务管理两种方式。

- 编程式事务管理：编程式事务管理使用TransactionTemplate或者直接使用底层的PlatformTransactionManager。对于编程式事务管理，Spring推荐使用TransactionTemplate。
- 声明式事务管理：建立在AOP之上的。其本质是对方法前后进行拦截，然后在目标方法开始之前创建或者加入一个事务，在执行完目标方法之后根据执行情况提交或者回滚事务。声明式事务管理不需要入侵代码，通过`@Transactional`就可以进行事务操作，更快捷而且简单，推荐使用。
<a name="orGVP"></a>
### 1.2 事务提交方式
默认情况下，数据库处于自动提交模式。每一条语句处于一个单独的事务中，在这条语句执行完毕时，如果执行成功则隐式的提交事务，如果执行失败则隐式的回滚事务。对于正常的事务管理，是一组相关的操作处于一个事务之中，因此必须关闭数据库的自动提交模式。不过，这个不用担心，Spring会将底层连接的自动提交特性设置为false。也就是在使用Spring进行事务管理的时候，Spring会将是否自动提交设置为false，等价于JDBC中的 `connection.setAutoCommit(false);`，在执行完之后在进行提交，`connection.commit();` 。
<a name="oUi9N"></a>
### 1.3 事务隔离级别
隔离级别是指若干个并发的事务之间的隔离程度。TransactionDefinition 接口中定义了五个表示隔离级别的常量：

- `TransactionDefinition.ISOLATION_DEFAULT`：这是默认值，表示使用底层数据库的默认隔离级别。对大部分数据库而言，通常这值就是`TransactionDefinition.ISOLATION_READ_COMMITTED`。
- `TransactionDefinition.ISOLATION_READ_UNCOMMITTED`：该隔离级别表示一个事务可以读取另一个事务修改但还没有提交的数据。该级别不能防止脏读，不可重复读和幻读，因此很少使用该隔离级别。比如PostgreSQL实际上并没有此级别。
- `TransactionDefinition.ISOLATION_READ_COMMITTED`：该隔离级别表示一个事务只能读取另一个事务已经提交的数据。该级别可以防止脏读，这也是大多数情况下的推荐值。
- `TransactionDefinition.ISOLATION_REPEATABLE_READ`：该隔离级别表示一个事务在整个过程中可以多次重复执行某个查询，并且每次返回的记录都相同。该级别可以防止脏读和不可重复读。
- `TransactionDefinition.ISOLATION_SERIALIZABLE`：所有的事务依次逐个执行，这样事务之间就完全不可能产生干扰，也就是说，该级别可以防止脏读、不可重复读以及幻读。但是这将严重影响程序的性能。通常情况下也不会用到该级别。
<a name="v7g9M"></a>
### 1.4 事务传播行为
所谓事务的传播行为是指，如果在开始当前事务之前，一个事务上下文已经存在，此时有若干选项可以指定一个事务性方法的执行行为。在TransactionDefinition定义中包括了如下几个表示传播行为的常量：

- `TransactionDefinition.PROPAGATION_REQUIRED`：如果当前存在事务，则加入该事务；如果当前没有事务，则创建一个新的事务。这是默认值。
- `TransactionDefinition.PROPAGATION_REQUIRES_NEW`：创建一个新的事务，如果当前存在事务，则把当前事务挂起。
- `TransactionDefinition.PROPAGATION_SUPPORTS`：如果当前存在事务，则加入该事务；如果当前没有事务，则以非事务的方式继续运行。
- `TransactionDefinition.PROPAGATION_NOT_SUPPORTED`：以非事务方式运行，如果当前存在事务，则把当前事务挂起。
- `TransactionDefinition.PROPAGATION_NEVER`：以非事务方式运行，如果当前存在事务，则抛出异常。
- `TransactionDefinition.PROPAGATION_MANDATORY`：如果当前存在事务，则加入该事务；如果当前没有事务，则抛出异常。
- `TransactionDefinition.PROPAGATION_NESTED`：如果当前存在事务，则创建一个事务作为当前事务的嵌套事务来运行；如果当前没有事务，则该取值等价于`TransactionDefinition.PROPAGATION_REQUIRED`。
<a name="iMV0o"></a>
### 1.5 事务回滚规则
指示Spring事务管理器回滚一个事务的推荐方法是在当前事务的上下文内抛出异常。Spring事务管理器会捕捉任何未处理的异常，然后依据规则决定是否回滚抛出异常的事务。默认配置下，Spring只有在抛出的异常为运行时unchecked异常时才回滚该事务，也就是抛出的异常为RuntimeException的子类(Errors也会导致事务回滚)，而抛出checked异常则不会导致事务回滚。可以明确的配置在抛出那些异常时回滚事务，包括checked异常。也可以明确定义那些异常抛出时不回滚事务。
<a name="JlcZu"></a>
### 1.6 事务常用配置

- readOnly：该属性用于设置当前事务是否为只读事务，设置为true表示只读，false则表示可读写，默认值为false。例如：`@Transactional(readOnly=true)`；
- rollbackFor：该属性用于设置需要进行回滚的异常类数组，当方法中抛出指定异常数组中的异常时，则进行事务回滚。例如：指定单一异常类：`@Transactional(rollbackFor=RuntimeException.class)`指定多个异常类：`@Transactional(rollbackFor={RuntimeException.class, Exception.class})`；
- rollbackForClassName：该属性用于设置需要进行回滚的异常类名称数组，当方法中抛出指定异常名称数组中的异常时，则进行事务回滚。例如：指定单一异常类名称`@Transactional(rollbackForClassName="RuntimeException")`指定多个异常类名称：`@Transactional(rollbackForClassName={"RuntimeException","Exception"})`。
- noRollbackFor：该属性用于设置不需要进行回滚的异常类数组，当方法中抛出指定异常数组中的异常时，不进行事务回滚。例如：指定单一异常类：`@Transactional(noRollbackFor=RuntimeException.class)`指定多个异常类：`@Transactional(noRollbackFor={RuntimeException.class, Exception.class})`。
- noRollbackForClassName：该属性用于设置不需要进行回滚的异常类名称数组，当方法中抛出指定异常名称数组中的异常时，不进行事务回滚。例如：指定单一异常类名称：`@Transactional(noRollbackForClassName="RuntimeException")`指定多个异常类名称：`@Transactional(noRollbackForClassName={"RuntimeException","Exception"})`。
- propagation ：该属性用于设置事务的传播行为。例如：`@Transactional(propagation=Propagation.NOT_SUPPORTED,readOnly=true)`。
- isolation：该属性用于设置底层数据库的事务隔离级别，事务隔离级别用于处理多事务并发的情况，通常使用数据库的默认隔离级别即可，基本不需要进行设置。
- timeout：该属性用于设置事务的超时秒数，默认值为-1表示永不超时。
<a name="Nm41t"></a>
### 1.7 事务注意事项

1. 要根据实际的需求来决定是否要使用事务，最好是在编码之前就考虑好，不然到以后就难以维护；
2. 如果使用了事务，请务必进行事务测试，因为很多情况下以为事务是生效的，但是实际上可能未生效！
3. 事务`@Transactional`的使用要放在类的**公共(**`**public**`**)方法**中，需要注意的是在 `protected`、`private` 方法上使用 `@Transactional` 注解，它也不会报错(IDEA会有提示)，但事务无效。
4. 事务`@Transactional`是不会对该方法里面的子方法生效！也就是在公共方法A声明的事务`@Transactional`，但是在A方法中有个子方法B和C，其中方法B进行了数据操作，但是该异常被B自己处理了，这样的话事务是不会生效的！反之B方法声明的事务`@Transactional`，但是公共方法A却未声明事务的话，也是不会生效的！如果想事务生效，需要将子方法的事务控制交给调用的方法，在子方法中使用`rollbackFor`注解指定需要回滚的异常或者将异常抛出交给调用的方法处理。一句话就是在使用事务的异常由调用者进行处理！
5. 事务`@Transactional`由Spring控制的时候，它会在抛出异常的时候进行回滚。如果自己使用catch捕获了处理了，是不生效的，如果想生效可以进行手动回滚或者在catch里面将异常抛出，比如`throw new RuntimeException();`。
<a name="T4oFj"></a>
## 2、SpringBoot中开启事务以及事务的使用
SpringBoot在使用事务Transactional的时候，要在main方法上加上 `@EnableTransactionManagement` 注解开启事务声明，在使用的service层的公共方法加上 `@Transactional` (Spring)注解。
<a name="FVGeO"></a>
### 2.1 事务的使用示例一：添加注解将事务交给Spring控制
公共方法上面添加 `@Transactional` 注解，由Spring控制事务。
```java
@Transactional
public boolean test1(User user) throws Exception {
    long id = user.getId();
    System.out.println("查询的数据1:" + udao.findById(id));
    // 新增两次，会出现主键ID冲突，看是否可以回滚该条数据
    udao.insert(user);
    System.out.println("查询的数据2:" + udao.findById(id));
    udao.insert(user);
    return false;
}
```
<a name="eokPj"></a>
### 2.2 事务的使用示例二：手动触发事务
在使用事务 `@Transactional` 的时候，想自己对异常进行处理的话，那么可以进行手动回滚事务。在catch中加上 `TransactionAspectSupport.currentTransactionStatus().setRollbackOnly();` 方法进行手动回滚。不过需要注意的是发生异常需要第一时间进行手动回滚事务，也就是要在异常抛出之前！
```java
@Transactional
public boolean test2(User user) {

  long id = user.getId();
  try {
       System.out.println("查询的数据1:" + udao.findById(id));
       // 新增两次，会出现主键ID冲突，看是否可以回滚该条数据
       udao.insert(user);
       System.out.println("查询的数据2:" + udao.findById(id));
       udao.insert(user);
  } catch (Exception e) {
       System.out.println("发生异常,进行手动回滚！");
       // 手动回滚事务
       TransactionAspectSupport.currentTransactionStatus().setRollbackOnly();
       e.printStackTrace();
  }
  return false;
}
```
<a name="Z2EkV"></a>
### 2.3 事务的使用示例三：指定触发事务回滚的异常类型
在使用事务 `@Transactional` 的时候，调用了其他的子方法进行了数据库的操作，如果想使其事务生效的话，可以使用`rollbackFor`注解或者将该子方法的异常抛出由调用的方法进行处理，不过这里需要注意的是，子方法也必须是公共的方法！
```java
@Transactional
public boolean test3(User user) {

    /*
       * 子方法出现异常进行回滚
       */
    try {
        System.out.println("查询的数据1:" + udao.findById(user.getId()));
        deal1(user);
        deal2(user);
        deal3(user);
    } catch (Exception e) {
        System.out.println("发生异常,进行手动回滚！");
        // 手动回滚事务
        TransactionAspectSupport.currentTransactionStatus().setRollbackOnly();
        e.printStackTrace();
    } 
    return false;
}

public void deal1(User user) throws SQLException {
    udao.insert(user);
    System.out.println("查询的数据2:" + udao.findById(user.getId()));
}

public void deal2(User user)  throws SQLException{
    if(user.getAge()<20){
        //SQL异常
        udao.insert(user);
    }else{
        user.setAge(21);
        udao.update(user);
        System.out.println("查询的数据3:" + udao.findById(user.getId()));
    }
}

@Transactional(rollbackFor = SQLException.class)
public void deal3(User user)  {
    if(user.getAge()>20){
        //SQL异常
        udao.insert(user);
    }
}
```
更多参考<br />[在事务注解@Transactional中指定rollbackFor](https://www.yuque.com/fcant/java/mm3res?view=doc_embed)
<a name="e5dc9a09"></a>
### 2.4 事务的使用示例四：自定义事务
如果不想使用事务 `@Transactional` 注解，想自己进行事务控制(编程事务管理)，控制某一段的代码事务生效，但是又不想去编写那么多的代码，那么可以使用SpringBoot中的`DataSourceTransactionManager`和`TransactionDefinition`这两个类来结合使用，能够达到手动控制事务的提交回滚。不过在进行使用的时候，需要注意在回滚的时候，要确保开启了事务但是未提交，如果未开启或已提交的时候进行回滚是会在catch里面发生异常的！
```java
@Autowired
private DataSourceTransactionManager dataSourceTransactionManager;
@Autowired
private TransactionDefinition transactionDefinition;

public boolean test4(User user) {
    /*
       * 手动进行事务控制
       */
    TransactionStatus transactionStatus=null;
    boolean isCommit = false;
    try {
        transactionStatus = dataSourceTransactionManager.getTransaction(transactionDefinition);
        System.out.println("查询的数据1:" + udao.findById(user.getId()));
        // 进行新增/修改
        udao.insert(user);
        System.out.println("查询的数据2:" + udao.findById(user.getId()));
        if(user.getAge()<20) {
            user.setAge(user.getAge()+2);
            udao.update(user);
            System.out.println("查询的数据3:" + udao.findById(user.getId()));
        }else {
            throw new Exception("模拟一个异常!");
        }
        //手动提交
        dataSourceTransactionManager.commit(transactionStatus);
        isCommit= true;
        System.out.println("手动提交事务成功!");
        throw new Exception("模拟第二个异常!");

    } catch (Exception e) {
        //如果未提交就进行回滚
        if(!isCommit){
            System.out.println("发生异常,进行手动回滚！");
            //手动回滚事务
            dataSourceTransactionManager.rollback(transactionStatus);
        }
        e.printStackTrace();
    }
    return false;
}
```
<a name="tBNZi"></a>
### 2.5 事务的使用示例五：设置断点进行回滚
Spring里面还有一种事务的控制方法，就是设置断点进行回滚。
```java
Object savePoint =null;
try{
    //设置回滚点
    savePoint = TransactionAspectSupport.currentTransactionStatus().createSavepoint();
}catch(Exception e){
    //出现异常回滚到savePoint。
    TransactionAspectSupport.currentTransactionStatus().rollbackToSavepoint(savePoint);
}
```
<a name="UPN65"></a>
## 3、SpringBoot事务常见问题总结
Spring Boot 中使用事务非常简单，使用`@Transactional` 注解即可。但是在实际项目中，是有很多小坑，这些小坑在写代码的时候没有注意到，而且正常情况下不容易发现这些小坑，等项目写大了，某一天突然出问题了，排查问题非常困难，需要费很大的精力去排查问题。
<a name="DPw4M"></a>
### 3.1 异常并没有被 “捕获” 到
异常并没有被“捕获”到，导致事务并没有回滚。在业务层代码中，也许已经考虑到了异常的存在，或者编辑器已经提示需要抛出异常，但是这里面有个需要注意的地方：并不是说把异常抛出来了，有异常了事务就会回滚，来看一个例子：
```java
@Service
public class UserServiceImpl implements UserService {
    @Resource
    private UserMapper userMapper;

    @Override
    @Transactional
    public void isertUser2(User user) throws Exception {
        // 插入用户信息
        userMapper.insertUser(user);
        // 手动抛出异常
        throw new SQLException("数据库异常");
    }
}
```
上面这个代码，其实并没有什么问题，手动抛出一个 `SQLException` 来模拟实际中操作数据库发生的异常，在这个方法中，既然抛出了异常，那么事务应该回滚，实际却不如此，测试一下就会发现，仍然是可以插入一条用户数据的。<br />那么问题出在哪呢？因为 Spring Boot 默认的事务规则是遇到运行异常（RuntimeException）和程序错误（Error）才会回滚。比如上面的例子中抛出的 RuntimeException 就没有问题，但是抛出 SQLException 就无法回滚了。针对非检测异常，如果要进行事务回滚的话，可以在 `@Transactional` 注解中使用 `rollbackFor` 属性来指定异常，比如 `@Transactional(rollbackFor = Exception.class)`，这样就没有问题了。
:::danger
所以在实际项目中，一定要指定异常。
:::
<a name="2dbRa"></a>
### 3.2 异常被“吃”掉
在处理异常时，有两种方式，要么抛出去，让上一层来捕获处理；要么把异常 try catch 掉，在异常出现的地方给处理掉。就因为有这中 try...catch，所以导致异常被“吃”掉，事务无法回滚。简单修改一下上面那个例子的代码：
```java
@Service
public class UserServiceImpl implements UserService {
    @Resource
    private UserMapper userMapper;
    @Override
    @Transactional(rollbackFor = Exception.class)
    public void isertUser3(User user) {
        try {
            // 插入用户信息
            userMapper.insertUser(user);
            // 手动抛出异常
            throw new SQLException("数据库异常");
        } catch (Exception e) {
            // 异常处理逻辑
        }
    }
}
```
测试一下，就会发现，仍然是可以插入一条用户数据，说明事务并没有因为抛出异常而回滚。这个细节往往比上面那个更难以发现，因为编码时很容易导致 try...catch 代码的产生，一旦出现这种问题，往往排查起来比较费劲，所以平时在写代码时，一定要多思考，多注意这种细节，尽量避免给自己埋坑。<br />那这种怎么解决呢？直接往上抛，给上一层来处理即可，千万不要在事务中把异常自己“吃”掉。
<a name="042bJ"></a>
### 3.3 事务的范围
事务范围这个东西比上面两个坑埋的更深！根据案例看一下：
```java
@Service
public class UserServiceImpl implements UserService {
    @Resource
    private UserMapper userMapper;
    @Override
    @Transactional(rollbackFor = Exception.class)
    public synchronized void isertUser4(User user) {
        // 实际中的具体业务……
        userMapper.insertUser(user);
    }
}
```
可以看到，因为要考虑并发问题，在业务层代码的方法上加了个 `synchronized` 关键字。举个实际的场景，比如一个数据库中，针对某个用户，只有一条记录，下一个插入动作过来，会先判断该数据库中有没有相同的用户，如果有就不插入，就更新，没有才插入，所以理论上，数据库中永远就一条同一用户信息，不会出现同一数据库中插入了两条相同用户的信息。<br />但是在压测时，就会出现上面的问题，数据库中确实有两条同一用户的信息，分析其原因，在于事务的范围和锁的范围问题。<br />从上面方法中可以看到，方法上是加了事务的，那么也就是说，在执行该方法开始时，事务启动，执行完了后，事务关闭。但是 `synchronized` 没有起作用，其实根本原因是因为事务的范围比锁的范围大。也就是说，在加锁的那部分代码执行完之后，锁释放掉了，但是事务还没结束，此时另一个线程进来了，事务没结束的话，第二个线程进来时，数据库的状态和第一个线程刚进来是一样的。即由于MySQL Innodb引擎的默认隔离级别是可重复读（在同一个事务里，SELECT的结果是事务开始时时间点的状态），线程二事务开始的时候，线程一还没提交完成，导致读取的数据还没更新。第二个线程也做了插入动作，导致了脏数据。<br />这个问题可以避免，第一，把事务去掉即可（不推荐）；第二，在调用该 service 的地方加锁，保证锁的范围比事务的范围大即可。
