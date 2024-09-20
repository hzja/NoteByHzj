JavaSpringBoot事务
<a name="wUl2N"></a>
## 1、事务失效的7种情况

1. 未启用Spring事务管理功能
2. 方法不是public类型的
3. 数据源未配置事务管理器
4. 自身调用问题
5. 异常类型错误
6. 异常被吞了
7. 业务和Spring事务代码必须在一个线程中
<a name="wSpOq"></a>
### 1.1、未启用Spring事务管理功能
`@EnableTransactionManagement` 注解用来启用Spring事务自动管理事务的功能，这个注解千万不要忘记写了。
<a name="RHfFh"></a>
### 1.2、方法不是`public`类型的
`@Transaction` 可以用在类上、接口上、`public`方法上，如果将`@Trasaction`用在了非`public`方法上，事务将无效。
<a name="XYYe7"></a>
### 1.3、数据源未配置事务管理器
Spring是通过事务管理器了来管理事务的，一定不要忘记配置事务管理器了，要注意为每个数据源配置一个事务管理器：
```java
@Bean
public PlatformTransactionManager transactionManager(DataSource dataSource) {
    return new DataSourceTransactionManager(dataSource);
}
```
<a name="deP54"></a>
###  1.4、自身调用问题
Spring是通过aop的方式，对需要Spring管理事务的bean生成了代理对象，然后通过代理对象拦截了目标方法的执行，在方法前后添加了事务的功能，所以必须通过代理对象调用目标方法的时候，事务才会起效。<br />看下面代码，大家思考一个问题：当外部直接调用m1的时候，m2方法的事务会生效么？
```java
@Component
public class UserService {
    public void m1(){
        this.m2();
    }

    @Transactional
    public void m2(){
        //执行db操作
    }
}
```
显然不会生效，因为m1中通过this的方式调用了m2方法，而this并不是代理对象，this.m2()不会被事务拦截器，所以事务是无效的，如果外部直接调用通过UserService这个bean来调用m2方法，事务是有效的，上面代码可以做一下调整，如下，@1在UserService中注入了自己，此时m1中的m2事务是生效的
```java
@Component
public class UserService {
    @Autowired //@1
    private UserService userService;

    public void m1() {
        this.userService.m2();
    }

    @Transactional
    public void m2() {
        //执行db操作
    }
}
```
**重点：必须通过代理对象访问方法，事务才会生效。**
<a name="yZBYF"></a>
### 1.5、异常类型错误
Spring事务回滚的机制：对业务方法进行try catch，当捕获到有指定的异常时，Spring自动对事务进行回滚，那么问题来了，哪些异常Spring会回滚事务呢？<br />并不是任何异常情况下，Spring都会回滚事务，默认情况下，RuntimeException和Error的情况下，Spring事务才会回滚。<br />也可以自定义回滚的异常类型：
```java
@Transactional(rollbackFor = {异常类型列表})
```
<a name="Sky4U"></a>
### 1.6、异常被吞了
当业务方法抛出异常，Spring感知到异常的时候，才会做事务回滚的操作，若方法内部将异常给吞了，那么事务无法感知到异常了，事务就不会回滚了。<br />如下代码，事务操作2发生了异常，但是被捕获了，此时事务并不会被回滚
```java
@Transactional
public void m1(){
    事务操作1
    try{
        事务操作2，内部抛出了异常
    }catch(Exception e){
        
    }
}
```
<a name="M8iSA"></a>
### 1.7、业务和Spring事务代码必须在一个线程中
Spring事务实现中使用了`ThreadLocal`，`ThreadLocal`大家应该知道吧，可以实现同一个线程中数据共享，必须是同一个线程的时候，数据才可以共享，这就要求业务代码必须和Spring事务的源码执行过程必须在一个线程中，才会受spring事务的控制，比如下面代码，方法内部的子线程内部执行的事务操作将不受m1方法上Spring事务的控制，这个大家一定要注意
```java
@Transactional
public void m1() {
    new Thread() {
        一系列事务操作
    }.start();
}
```
<a name="Hw4iO"></a>
## 2、如何快速定位事务相关bug？
2种方式
<a name="fcnue"></a>
### 方式1：看日志
如果使用了logback或者log4j来输出日志，可以修改一下日志级别为debug模式，可以看到事务的详细执行日志，以便定位错误。
<a name="W1sGj"></a>
### 方式2：调试代码
如果对源码比较了解，那么知道被Spring管理事务的业务方法，执行的时候都会被`TransactionInterceptor`拦截器拦截，会进入到它的`invoke`方法中，可以在`invoke`方法中设置一些断点，可以看到详细的执行过程，排错也就比较容易了。<br />整体上来说，还是需要深入理解原理，原理了解了，写代码的时候本身就会避免很多坑。
