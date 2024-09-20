JavaSpring事务
<a name="XANTu"></a>
### 1、数据库引擎不支持事务
这里以 MySQL为例，MyISAM引擎是不支持事务操作的，一般要支持事务都会使用InnoDB引擎，根据MySQL 的官方文档说明，从MySQL 5.5.5 开始的默认存储引擎是 InnoDB，之前默认的都是 MyISAM，所以这一点要值得注意，如果底层引擎不支持事务，那么再怎么设置也没有用。
<a name="tHjV7"></a>
### 2、没有被 Spring 管理
示例如下:
```java
public class OrderServiceImpl implements OrderService{
    @Transactional
    public void updateOrder(Order order){
        //update order
    }
}
```
如果此时把`@Service`注解注释掉，那么这个类就不会被加载成一个Bean，这个类就不会Spring管理了，事务自然就失效了。
<a name="xTT4d"></a>
### 3、方法不是 `public` 的
`@Transactional`注解只能用干`public` 的方法上，否则事多不会生效，如果要用在非`public`的方法上，则可以开启基于 AspcetJ 框架的静态代理模式。
<a name="EhXBB"></a>
### 4、发生自身调用
示例如下:
```java
@Service
public class OrderServiceImpl implements OrderService {
    public void update(Order order) {
        updateOrder(order);
    }
}

@Transactional
public void updateOrder(0rder order) {
    // update order
}
}
```
update 方法上面没有加 `@Transactional` 注解，如果调用有 `@Transactional` 注解的`updateOrder` 方法，那么 `updateOrder` 方法上的事务还可以生效吗?   这里大家可以先想一想，后面会揭晓答案。<br />再来看下面这个例子:
```java
@Service
public class OrderServiceImpl implements OrderService {
    @Transactional
    public void update(Order order) {
        updateOrder(order);
    }
}

@Transactional(propagation = Propagation.REQUIRES_NEW)
public void updateOrder(0rder order) {
    updateOrder(order);
}
```
这次在 update 方法上加了 `@Transactional`，如果在 updateOrder 上加了 `REOUIRES_NEW`新开启一个事务，那么新开启的事务可以生效吗?<br />这两个例子中的事务都不会生效，因为它们发生了自身调用，就调用了该类自己的方法，而没有经过Spring的代理类，默认只有调用外部代理类的方法，事务才会生效，这也是老生常谈的问题了。<br />这个问题的解决方案之一就是在事务所在的类中注入自己，用往入的对象再调用另外一个方法，这个不太优雅，在Spring 中可以在当前线程中暴露并获取当前代理类，通过在启动类上添加以下注解来启用暴露代理类，如下面的示例所示。
```java
@EnableAspectJAutoProxy(exposeProxy = true)
```
然后通过以下代码获取当前代理类，并调用代理类的事务方法：
```java
((0rderService) AopContext.currentProxy()).updateOrder();
```
**Spring 默认只有调用 Spring代理类的public 方法，事务才能生效。**
<a name="Imaez"></a>
### 5、没有配置事务管理器
如果没有配置以下`DataSourceTransactionManager`数据源事务管理器，那么事务也不会生效 ：
```java
@Bean
public PlatformTransactionManager transactionManager(DataSource dataSource) {
	return new DataSourceTransactionManager(dataSource);
}
```
但在 Spring Boot 中只要引入了 spring-boot-starter-data-jdbc 启动器依赖就会自动配置`DataSourceTransactionManager`数据源事务管理器，所以 Spring Boot框架不存在这个问题，但在传统的 Spring 框架中需要注意。
<a name="gcdyL"></a>
### 6、设置了不支持事务
示例如下:
```java
@Service
public class OrderServiceImpl implements OrderService {
    @Transactional
    public void update(Order order) {
        updateOrder(order);
    }

    @Transactional(propagation = Propagation.NOT_SUPPORTED)
    public void updateOrder(Order order) {
        //update order
    }
}
```
这里的`Propagation.NOT_SUPPORTED`表示当前方法不以事务方式运行，当前若存在事务则挂起，这就是主动不支持以事务方式运行了。
<a name="iMtsw"></a>
### 7、异常没有被抛出
示例如下:
```java
@Service
public class OrderServiceImpl implements OrderService {
    @Transactional
    public void update(Order order) {
        try{
            // update order
        }catch{

        }
    }
}
```
这个方法把异常给捕获了，但没有抛出来，所以事务不会回滚，只有捕捉到异常事务才会生效。
<a name="JHclX"></a>
### 8、异常类型不匹配
示例如下：
```java
@Service
public class OrderServiceImpl implements OrderService {
    @Transactional
    public void update(Order order) {
        try{
            // update order
        }catch{
            throw new Exception("更新失败");
        }
    }
}
```
因为 Spring 默认回滚的是 `RuntimeException` 异常，和程序抛出的 Exception 异常不匹配，所以事务也是不生效的。如果要触发默认 `RuntimeException`之外异常的回滚，则需要在 `@Transactional`事务注解上指定异常类，示例如下:
```java
@Transactional(rollbackFor = Exception.class)
```
总结了使用 `@Transactional`注解导致事务失效的几个常见场景，如果 `@Transactional`事务不生效，则可以根据这几种情形排查一下，其实次数最多的也就是发生自身调用、异常被捕获、异常抛出类型不匹配这几种场景。
