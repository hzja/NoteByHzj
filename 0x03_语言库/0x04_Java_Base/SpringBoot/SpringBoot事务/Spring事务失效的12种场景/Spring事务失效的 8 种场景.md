JavaSpring事务
<a name="BbTuo"></a>
### 情况 1：未启用 Spring 事务管理配置
看一下代码中是否有 `@EnableTransactionManagement` 注解，且这个注解需要被 Spring 处理，这个注解可以启用 Spring 事务功能。
<a name="AXmTT"></a>
### 情况 2：方法必须是 `public` 类型的
检查一下方法是否是 `public` 类型的，Spring 事务只能对 `public` 方法起效。
<a name="eRyPM"></a>
### 情况 3：未配置事务管理器 Bean
Spring 内部是依靠事务管理器（`TransactionManager`）来操作事务的，所以 Spring 容器中必须要有事务管理器这个 bean
```java
@Bean
public TransactionManager transactionManager(DataSource dataSource) {
    return new DataSourceTransactionManager(dataSource);
}
```
<a name="LRlUs"></a>
### 情况 4：事务管理器的数据源和代码中操作 db 的数据源不一致
事务方法所用到的事务管理器的数据源和方法内部执行 sql 所用到的数据源必须一致，否则事务会失效。<br />创建事务管理器的时候，可以指定数据源，如下：
```java
@Bean
public TransactionManager transactionManager(DataSource dataSource) {
	return new DataSourceTransactionManager(dataSource);
}
```
而通常会使用`@Transaction` 注解，放在目标方法上，添加事务功能，这个注解有个 `transactionManager` 属性，可以用来指定事务管理器的 bean 名称，当系统中存在多个事务管理器的时候，可以通过这个属性来指定当前方法具体使用哪个事务管理器，需确保方法内部执行 sql 的数据源和事务管理器中用到的是同一个数据源。
<a name="SwQrM"></a>
### 情况 5：方法调用问题
比如下面代码，m2 方法上添加了事务功能，但是此时如果外面直接调用 m1 方法，此时 m2 的事务将失效，因为必须通过代理对象直接调用`@Transaction` 的方法时，事务才会起效，而 m1 方法中是通过 this 来调用 m2 方法的，而 this 是当前对象，并不是代理对象，那么如果想让 m2 方法事务生效，而又不想在 m1 方法添加`@Transaction` 注解，有什么办法么？
```java
@Component
public class DemoService {
    public void m1(){
        this.m2();
    }

    @Transactional
    public void m2(){
        //执行db操作
    }
}

```
<a name="SoTL2"></a>
### 情况 6：业务方法内部抛出的异常类型和事务回滚的异常类型不匹配
`@Transactional` 注解内部有个 `rollbackFor` 属性，用来指定事务回滚的异常类型，当方法抛出的异常类型和 `rollbackFor` 类型匹配的时候，事务才能够回滚，而 `rollbackFor` 的默认值是 `RuntimeException` 和 Error，如果方法内部抛出的异常类型不匹配，事务是无法回滚的，会让您失望的，所以建议大家使用的时候请务必设置下这个属性的值。
```java
@Transactional(rollbackFor = {异常类型列表})
```
<a name="p0jnz"></a>
### 情况 7：业务代码将异常生吞了
Spring 是依靠监听业务方法异常，来决定是回滚事务还是提交事务的，比如下面代码，程序将异常吞了，此时事务是无法回滚的，不管任何使用，建议不要吞掉异常。
```java
@Transactional
public void xxx(){
    事务操作1
    try{
        事务操作2，内部抛出了异常，但是被捕获吞掉了
    }catch(Exception e){
    }
}
```
<a name="wQp7t"></a>
### 情况 8：业务代码和 Spring 事务未在一个线程中
比如下面代码，在新的线程中去执行 db 操作，此时事务是失效的，程序需要确保 db 操作的代码和 Spring 事务必须在同一个线程中，否则事务不受 Spring 控制。
```java
@Transactional
public void m1() {
    new Thread() {
        一系列事务操作
    }.start();
}
```
