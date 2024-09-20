Java Spring @Transactional<br />数据事务是企业应用关注的核心内容，也是开发者最容易犯错的地方。<br />本文就来讲解**一些使用中的不良习惯**，注意，它们可以避免一些错误和性能的丢失。
<a name="riGKc"></a>
## 错误使用Service
互联网往往采用模型—视图—控制器（Model View Controller，MVC）来搭建开发环境，因此在Controller中使用Service是十分常见的。<br />为了方便测试，角色服务（roleService）和角色列表（RoleListService）进行测。假设想在一个Controller中插入两个角色，并且两个角色需要在同一个事务中处理，下面先给出错误使用Service的Controller，代码如下。
```java
package com.learn.ssm.chapter14.controller;
/****************** imports ******************/
@Controller    
public class RoleController {
    @Autowired
    private RoleService roleService = null;
    
    @Autowired
    private RoleListService roleListService = null;
    
    // 方法无事务
    public void errerUseServices() {
        Role role1 = new Role();
        role1.setRoleName("role_name_1");
        role1.setNote("role_note_1");
        // 带事务方法
        roleService.insertRole(role1);
        Role role2 = new Role();
        role2.setRoleName("role_name_2");
        role2.setNote("role_note_2");
        // 带事务方法
        roleService.insertRole(role2);
    }
}
```
类似的代码在工作中常常出现，甚至拥有多年开发经验的开发人员也会犯这类错误。这里存在的问题是两个insertRole方法根本不在同一个事务里的问题。<br />当一个Controller使用Service方法时，如果这个Service方法标注有`@Transactional`，那么它就会启用一个事务，而一个Service方法完成后，它就会释放该事务，所以前后两个insertRole的方法是在两个不同的事务中完成的。<br />下面是测试这段代码的日志，可以清晰地看出它们并不存在于同一个事务中。
```java
org.mybatis.logging.Logger: Creating a new SqlSession
org.mybatis.logging.Logger: Registering transaction synchronization for SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@53f0a4cb]
org.mybatis.logging.Logger: JDBC Connection [1746570062, URL=jdbc:mysql://localhost:3306/ssm, UserName=root@localhost, MySQL Connector Java] will be managed by Spring
org.apache.ibatis.logging.jdbc.BaseJdbcLogger: ==>  Preparing: insert into t_role(role_name, note) values (?, ?)
org.apache.ibatis.logging.jdbc.BaseJdbcLogger: ==> Parameters: role_name_1(String), role_note_1(String)
org.apache.ibatis.logging.jdbc.BaseJdbcLogger: <==    Updates: 1
org.mybatis.logging.Logger: Releasing transactional SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@53f0a4cb]
org.mybatis.logging.Logger: Transaction synchronization committing SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@53f0a4cb]
org.mybatis.logging.Logger: Transaction synchronization deregistering SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@53f0a4cb]
org.mybatis.logging.Logger: Transaction synchronization closing SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@53f0a4cb]
org.mybatis.logging.Logger: Creating a new SqlSession
org.mybatis.logging.Logger: Registering transaction synchronization for SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@640f11a1]
org.mybatis.logging.Logger: JDBC Connection [2059592603, URL=jdbc:mysql://localhost:3306/ssm, UserName=root@localhost, MySQL Connector Java] will be managed by Spring
org.apache.ibatis.logging.jdbc.BaseJdbcLogger: ==>  Preparing: insert into t_role(role_name, note) values (?, ?)
org.apache.ibatis.logging.jdbc.BaseJdbcLogger: ==> Parameters: role_name_2(String), role_note_2(String)
org.apache.ibatis.logging.jdbc.BaseJdbcLogger: <==    Updates: 1
org.mybatis.logging.Logger: Releasing transactional SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@640f11a1]
```
这样如果第一个插入成功了，而第二个插入失败了，就会使数据库数据不完全同时成功或者失败，可能产生严重的数据不一致的问题，给生产带来严重的损失。<br />这个例子明确地告诉大家使用带有事务的Service方法时，应该只有一个入口，然后使用传播行为来定义事务策略。<br />如果错误地进行多次调用，就不会在同一个事务中，这会造成不同时提交或回滚的数据一致性问题。<br />每一个Java EE开发者都要注意这类问题，以避免一些不必要的错误。
<a name="AzUG5"></a>
## 过长时间占用事务
在企业的生产系统中，数据库事务资源是最宝贵的资源之一，使用了数据库事务之后，要及时释放数据库事务资源，甚至要评估数据库事务处理业务的耗时。<br />换言之，应该尽可能地缩短使用数据库事务资源的时间去完成所需工作，为此需要区分哪些业务是需要事务的，哪些又是不需要的，而不需要的耗时又如何？<br />比如，在工作中需要使用到文件、对外连接调用等操作，而这些操作往往会占用较长时间，且不需要事务，在这样的场景下，如果开发者不注意细节，就很容易出现系统宕机的问题。<br />假设在插入角色后还需要操作一个文件，那么要改造insertRole方法，如代码如下。
```java
@Override
@Transactional(propagation = Propagation.REQUIRED,
        isolation= Isolation.READ_COMMITTED)
public int insertRole(Role role) {
    int result = roleMapper.insertRole(role);
    // 操作一些与数据库无关的文件操作
    doSomethingForFile();
    return result;
}
```
假设doSomethingForFile方法是一个与数据库事务无关的操作，比如处理图片的上传之类的操作，但是笔者必须告诉读者这是一段糟糕的代码。<br />当insertRole方法结束后，Spring才会释放数据库事务资源，也就是说在运行doSomethingForFile方法时，Spring并没有释放数据库事务资源，而等到doSomethingForFile方法运行完成返回result后，才会关闭数据库资源。<br />在大型互联网系统中，一个数据库的链接可能也就是50条左右，然而同时并发的请求则可能是成百上千。<br />而其中大部分的并发请求都在等待这50条占有数据库连接资源的文件操作，假如平均一个doSomethingForFile的操作需要1秒，对于同时出现1 000条并发请求的网站，就会出现请求卡顿的状况。<br />因为大部分的请求都在等待数据库事务资源的分配，这是一个糟糕的结果，如图1所示。<br />![图1  在事务中的文件操作](https://cdn.nlark.com/yuque/0/2023/png/396745/1676855912199-bac8711e-cbc8-449c-9931-09ad8068bd07.png#averageHue=%23fcfcfb&clientId=u49ab0cd0-05bf-4&from=paste&id=u07dead86&originHeight=430&originWidth=938&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ufcbbfa87-9bf7-4091-8cfa-5d1e7336e7c&title=%E5%9B%BE1%20%20%E5%9C%A8%E4%BA%8B%E5%8A%A1%E4%B8%AD%E7%9A%84%E6%96%87%E4%BB%B6%E6%93%8D%E4%BD%9C "图1  在事务中的文件操作")<br />从图1可以看出，当操作文件这步占用较长时间时，数据库事务将长期得不到释放，这个时候如果发生高并发的情况，会造成大量的并发请求得不到数据库的事务资源而导致的系统宕机。<br />对此，应该修改为在Controller层中操作文件，代码如下。
```java
@Autowired
private RoleService roleService = null;

public Role addRole(Role role) {
    // 带事务方法
    roleService.insertRole(role);
    // 不需事务方法
    doSomethingForFile();
    return role;
}
```
注意，当程序运行完insertRole方法后，Spring会释放数据库事务资源。<br />对于doSomethingForFile方法而言，已经在一个没有事务的环境中运行了，这样当前的请求就不会长期占用数据库事务资源，使得其他并发的请求被迫等待其释放了，这个改写分析如图2所示。<br />![图2  不在事务中的文件操作](https://cdn.nlark.com/yuque/0/2023/png/396745/1676855912203-460e2ec5-72ef-4a0e-9032-490a2af840d2.png#averageHue=%23fbfbfb&clientId=u49ab0cd0-05bf-4&from=paste&id=uaf6be967&originHeight=350&originWidth=1056&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u3007597d-df27-4313-a282-10ddf194533&title=%E5%9B%BE2%20%20%E4%B8%8D%E5%9C%A8%E4%BA%8B%E5%8A%A1%E4%B8%AD%E7%9A%84%E6%96%87%E4%BB%B6%E6%93%8D%E4%BD%9C "图2  不在事务中的文件操作")<br />从图2可以看出，在操作文件时，事务早已被关闭了，这时操作文件就避免了数据库事务资源被当前请求占用，从而导致其他请求得不到事务的情况发生了。<br />其实，不仅是文件操作，还有一些系统之间的通信及一些可能需要花费较长时间的操作，都要注意这个问题。
<a name="RPKsq"></a>
## 错误捕捉异常
模拟一段购买商品的代码，其中ProductService是产品服务类，而TransactionService是记录交易信息，需求显然就是产品减库存和保存交易在同一个事务里面，要么同时成功，要么同时失败，并且假设减库存和保存交易的传播行为都为REQUIRED，现在来看如下代码。
```java
@Autowired
private PrudoctService prudoctService;

@Autowired
Private TransactionService transactionService;

@Override
@Transactional(propagation = Propagation.REQUIRED,
        isolation = Isolation.READ_COMMITTED)
public int doTransaction(TransactionBean trans) {
    int resutl = 0;
    try {
        // 减少库存
        int result =  prudoctService.decreaseStock(
            trans.getProductId, trans.getQuantity());
        // 如果减少库存成功则保存记录
        if (result >0) {
            transactionService.save(trans);
        }
    } catch(Exception ex) {
        // 自行处理异常代码
        // 记录异常日志
        log.info(ex);
    }
    return result;
}
```
这里的问题是方法已经存在异常了，由于开发者不了解Spring的事务约定，在两个操作的方法里面加入了自己的try...catch...语句。<br />这样实际也没有什么错误，只是显得冗余，之前分析过当PrudoctService的decreaseStock方法没有异常，而TransactionService的save方法发生异常时，也会发生事务的回滚，只是它会抛出“Transaction rolled back because it has been marked as rollback-only（事务看起来已经标注了只能回滚）”的异常，对于一些初级开发者可能就难以找到发生异常的原因了。<br />在那些需要处理大量异常的代码中，要小心这样的问题，避免代码复杂化，让定位问题出现很大的困难。有时候也确实需要自己处理异常，为此对上述代码进行改造，代码如下。
```java

@Autowired
private PrudoctService prudoctService;


@Autowired
Private TransactionService transactionService;


@Override
@Transactional(propagation = Propagation.REQUIRED,
        isolation = Isolation.READ_COMMITTED)
public int doTransaction(TransactionBean trans) {
    int resutl = 0;
    try {
        // 减少库存
        int result =  prudoctService.decreaseStock(
            trans.getProductId, trans.getQuantity());
        // 如果减少库存成功则保存记录
        if (result >0) {
            transactionService.save(trans);
        }
    } catch(Exception ex) {
        // 自行处理异常代码
        // 记录异常日志
        log.info(ex);
        // 自行抛出异常，让Spring事务管理流程获取异常，进行事务管理
        throw new RuntimeException(ex);
    }
    return result;
}
```
注意，它抛出了一个运行异常，这样在Spring的事务流程中，就会捕捉到抛出的这个异常，进行事务回滚。这样在发生异常时，会更有利于定位，这才是合适使用数据库事务的方式。
