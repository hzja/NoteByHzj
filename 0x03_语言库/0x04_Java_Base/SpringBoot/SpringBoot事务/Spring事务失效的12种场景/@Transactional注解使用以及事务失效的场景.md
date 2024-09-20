Java
<a name="Iqni0"></a>
## 一、事务的本质
<a name="Pgq49"></a>
### 1、何为事务管理
数据库事务，是指作为单个逻辑工作单元执行的一系列操作，要么完全地执行，要么完全地不执行。<br />事务处理可以确保除非事务性单元内的所有操作都成功完成，否则不会永久更新面向数据的资源。通过将一组相关操作组合为一个要么全部成功要么全部失败的单元，可以简化错误恢复并使应用程序更加可靠。<br />一个逻辑工作单元要成为事务，必须满足所谓的 ACID （原子性、一致性、隔离性和持久性）属性。事务是数据库运行中的逻辑工作单位。
<a name="dY1Xw"></a>
### 2、 Spring 中的事务管理
实际工作中更多的是结合 Spring 来做项目的这时要满足的情况是这种。
```
Controller层：
  UserService:addUser();
  Service层(UserService):
   addUser(): insertUser()+ insertLog()
  Dao层:
   UserDao：insertUser();
   LogDao: insertDao();
```
可以看出在 Service 中是可能调用多个 Dao 的方法来操作数据库中的数据的，要做的就是要保证 UserService 中的 `addUser()` 方法中的相关操作满足事务的要求<br />常见的开启 Spring 事务方式：`@Transactional`
<a name="PIOIA"></a>
## 二、 `@Transactional` 注解
<a name="gz9eR"></a>
### 1、原理：
事务开启后，通过 AOP 机制生成一个代理数据库连接对象并将其放入 DataSource 实例的某个 `DataSourceTransactionManager` 相关对象容器中。在整个事务中，业务代码中所有的数据库连接都应该是同一个连接，不使用该连接的 Sql 是不会被回滚的。业务代码出现异常时会执行回滚操作<br />底层实现：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1695866695643-0ca398f8-cd04-476c-aa34-3d2d75ba0c9f.jpeg#averageHue=%232f322d&clientId=u89b42374-c9c3-4&from=paste&id=u99f38c45&originHeight=572&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u95e88d49-d5d0-47fc-a4ec-9b376cbadb0&title=)
<a name="U9Fkd"></a>
###  2、属性介绍：
隔离级别（ `@Transactional(isolation = Isolation.DEFAULT)` ）: 为了解决数据库容易出现的问题，分级加锁处理策略

| 隔离级别 | 描述 |
| --- | --- |
| `ISOLATION_DEFAULT` | Spring 默认隔离级别，以连接的数据库的事务隔离级别为准；Mysql （可重复读） |
| `ISOLATION_READ_UNCOMMITTED` | 读未提交：最低的隔离级别，其含义是允许一个事务读取另外一个事务没有提交的数据。未提交读是一种危险的隔离级别，所以一般在实际的开发中应用不广，但是它的优点在于并发能力高，适合那些对数据一致性没有要求而追求高并发的场景，它的最大坏处是出现脏读 |
| `ISOLATION_READ_COMMITTED` | 读已提交：是指一个事务只能读取另外一个事务已经提交的数据，不能读取未提交的数据 |
| `ISOLATION_REPEATABLE_READ` | 可重复读：目标是克服读写提交中出现的不可重复读的现象，因为在读写提交的时候，可能出现一些值的变化，影响当前事务的执行 |
| `ISOLATION_SERIALIZABLE` | 串行化，数据库最高的隔离级别，它会要求所有的 SQL 都会按照顺序执行，这样就可以克服上述隔离级别出现的各种问题，所以它能够完全保证数据的一致性 |

超时时间（`@Transactional(timeout = 30)`）：定义一个事务执行过程多久算超时，以便超时后回滚。可以防止长期运行的事务占用资源.对应注解中的属性 timeout （注意点：这个超时时间在数据库事务超时的范畴内的）<br />是否只读（`@Transactional(readOnly = true)`）：表示这个事务只读取数据但不更新数据<br />回滚机制（`@Transactional(rollbackFor = Exception.class)`）：定义遇到异常时回滚策略<br />传播机制（`@Transactional(propagation = Propagation.REQUIRED)` ）：对事务的传播特性进行定义，共有 7 种类型 （一个事务内调用另外一个事务）

| 事务行为 | 说明 |
| --- | --- |
| `PROPAGATION_REQUIRED` | 如果当前上下文中存在事务，那么加入该事务，如果不存在事务，创建一个事务，这是默认的传播属性值 |
| `PROPAGATION_SUPPORTS` | 如果当前上下文存在事务，则支持事务加入事务，如果不存在事务，则使用非事务的方式执行 |
| `PROPAGATION_MANDATORY` | 支持当前事务，假设当前没有事务，就抛出异常 |
| `PROPAGATION_REQUIRES_NEW` | 每次都会新建一个事务，并且同时将上下文中的事务挂起，执行当前新建事务完成以后，上下文事务恢复再执行 |
| `PROPAGATION_NOT_SUPPORTED` | 如果当前上下文中存在事务，则挂起当前事务，然后新的方法在没有事务的环境中执行 |
| `PROPAGATION_NEVER` | 如果当前上下文中存在事务，则抛出异常，否则在无事务环境上执行代码 |
| `PROPAGATION_NESTED` | 如果当前存在事务，则在嵌套事务内执行。如果当前没有事务，则执行与 `PROPAGATION_REQUIRED` 类似的操作。 |

<a name="Bzsxh"></a>
## 三、常见的 `@Transactional` 注解 事务没生效的场景
<a name="P63Bt"></a>
###  1、访问权限问题 (只有 `public` 方法会生效)
示例代码：
```java
@Service
public class UserService {
    @Transactional
    private void add(UserModel userModel) {
        saveData(userModel);
        updateData(userModel);
    }
}
```
原因：Spring 要求被代理方法必须得是 `public` 的<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1695866695575-73eb37e3-4d99-48fa-9775-351aff8f9df1.jpeg#averageHue=%234b4630&clientId=u89b42374-c9c3-4&from=paste&id=u52cadf2b&originHeight=234&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc45b723f-a9e6-4638-a335-7dac4360603&title=)<br />也就是说，如果自定义的事务方法（即目标方法），它的访问权限不是 `public`，而是 `private`、 `default` 或 `protected` 的话， Spring 则不会提供事务功能
<a name="kWU54"></a>
### 2、方法用 `final` 修饰，不会生效
示例代码：
```java
@Service
public class UserService {
    @Transactional
    public final void add(UserModel userModel){
        saveData(userModel);
        updateData(userModel);
    }
}
```
原因：Spring 事务底层使用了 AOP，也就是通过 JDK 动态代理或者 CGLIB，生成了代理类，在代理类中实现的事务功能。但如果某个方法用 `final` 修饰了，那么在它的代理类中，就无法重写该方法，而添加事务功能。<br />注意：如果某个方法是 `static` 修饰的，同样无法通过动态代理，变成事务方法。
<a name="gOJZt"></a>
### 3、同一个方法内直接调用，会造成事务失效
示例代码：
```java
@Service
public class UserService {
    @Autowired
    private UserMapper userMapper;

    public void add(UserModel userModel) {
        userMapper.insertUser(userModel);
        updateStatus(userModel);
    }

    @Transactional
    public void updateStatus(UserModel userModel) {
        doSameThing();
    }
}
```
原因：在事务方法 add 中，直接调用事务方法 updateStatus。从前面介绍的内容可以知道， updateStatus 方法拥有事务的能力是因为 Spring AOP 生成代理了对象，但是这种方法直接调用了 this 对象的方法，并不会从 IOC 拿到加上 AOP 事务相关方法的动态代理对象 所以 updateStatus 方法不会生成事务
<a name="F9sKM"></a>
### 4、(类本身) 未被 Spring 管理
示例代码：
```java
//@Service
public class UserService {

    @Transactional
    public void add(UserModel userModel) {
        saveData(userModel);
        updateData(userModel);
    }    
}
```
原因：使用 Spring 事务的前提是：对象要被 Spring IOC 管理，需要创建 bean 实例
<a name="kb7hf"></a>
### 5、多线程调用
示例代码：
```java
@Service
public class UserService {

    @Autowired
    private UserMapper userMapper;
    @Autowired
    private RoleService roleService;

    @Transactional
    public void add(UserModel userModel) throws Exception {
        userMapper.insertUser(userModel);
        new Thread(() -> {
            roleService.doOtherThing();
        }).start();
    }
}

@Service
public class RoleService {
    @Transactional
    public void doOtherThing() {
    }
}
```
原因：同一个事务，其实是指同一个数据库连接，只有拥有同一个数据库连接才能同时提交和回滚。如果在不同的线程，拿到的数据库连接肯定是不一样的，所以是不同的事务。
<a name="wkAcQ"></a>
### 6、错误的传播特性
示例代码：
```java
@Service
public class UserService {
    @Autowired
    private UserMapper userMapper;
    @Autowired
    private RoleService roleService;

    @Transactional(propagation = Propagation.REQUIRED)
    public void add(UserModel userModel) throws Exception {
        userMapper.insertUser(userModel);
        new Thread(() -> {
            roleService.doOtherThing();
        }).start();
    }
}

@Service
public class RoleService {
    @Transactional(propagation = Propagation.NEVER)
    public void doOtherThing() {
    }
}
```
原因：RoleService 中 doOtherThing() 方法上设置的事物传播类型为 `Propagation.NEVER`，即存在事务就抛出异常
<a name="aIR8w"></a>
### 7、自己吞了异常
示例代码：
```java
@Slf4j
@Service
public class UserService {
    @Transactional
    public void add(UserModel userModel) {
        try {
            saveData(userModel);
            updateData(userModel);
        } catch (Exception e) {
            log.error(e.getMessage(), e);
        }
    }
}
```
原因：如果想要 Spring 事务能够正常回滚，必须抛出它能够处理的异常。如果没有抛异常，则 Spring 认为程序是正常的。
<a name="PEzdL"></a>
### 8、抛出的异常，事务管理器处理不了，则不会回滚
示例代码：
```java
@Slf4j
@Service
public class UserService {
    @Transactional
    public void add(UserModel userModel) throws Exception {
        saveData(userModel);
        updateData(userModel);
    }
}
```
原因：`@Transactional` 默认的异常类型是 RuntimeException，如果出现非 RuntimeException，则 Spring 事务处理不了对应的异常，认为程序是正常的，则不会回滚事务，此时可以指定异常类型如 `@Transactional(rollbackFor = Exception.class)`
<a name="muzT4"></a>
### 9、数据库引擎不支持事务
比如 Mysql 中的 MyISAM 引擎是不支持事务操作的， InnoDB 才是支持事务的引擎
<a name="XTjqp"></a>
## 四、总结
本文通过对 `@Transactional` 注解相关介绍，列举出可能会出现事务失效的场景。发生最多就是自身调用、异常被吃、异常抛出类型不匹配这三个。由于平时业务繁重，有时候会忽视 `@Transactional` 注解使用规范，导致事务没有生效或者没有正常回滚，造成较大的数据异常。希望可以帮助大家日常使用 `@Transactional` 时避坑。
