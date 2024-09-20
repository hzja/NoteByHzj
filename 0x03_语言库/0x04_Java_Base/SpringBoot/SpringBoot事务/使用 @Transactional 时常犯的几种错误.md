Java Spring 事务<br />`@Transactional`是在用Spring时候几乎逃不掉的一个注解，该注解主要用来声明事务。它的实现原理是通过Spring AOP在注解修饰方法的前后织入事务管理的实现语句，所以开发者只需要通过一个注解就能代替一系列繁琐的事务开始、事务关闭等重复性的编码任务。<br />编码方式确实简单了，但也因为隐藏了直观的实现逻辑，一些错误的编码方法可能会让`@Transactional`注解失效，达不到事务的作用。最直接的表现就是：方法执行过程中抛出了异常，但事务没有回滚，最终导致了脏数据的产生。<br />`@Transactional`声明事务失效的情况真的是多种多样！
<a name="VhUNU"></a>
### 1、在同一个类中调用
错误案例：
```java
public class A {
    
    public void methodA() {
        methodB();
        
        // 其他操作
    }

    @Transactional
    public void methodB() {
        // 写数据库操作
    }
    
}
```
这类错误适用于所有基于Spring AOP实现的注解，比如：使用`@Async`实现异步调用中的`@Async`注解，使用`@Scheduled`实现定时任务中的`@Scheduled`注解，还有Spring缓存注解的使用中的`@Cacheable`注解等。<br />解决这个问题的方法比较简单，还是合理规划好层次关系即可，比如这样：
```java
@Service
@AllArgsConstructor
public class A {
    
    private B b;
    
    public void methodA() {
        b.methodB();
        // 其他操作
    }
}

@Service
public class B {

    @Transactional
    public void methodB() {
        // 写数据库操作
    }
    
}
```
:::tips
注意：这里A类用了构造器注入B的实现，构造函数用Lombok的`@AllArgsConstructor`生成。
:::
<a name="CHIu1"></a>
### 2、`@Transactional`修饰方法不是`public`
错误案例：
```java
public class TransactionalMistake {
    
    @Transactional
    private void method() {
        // 写数据库操作
    }
    
}
```
这也是基于Spring AOP实现的注解所要满足的要求。这个最简单，很好理解，也很直观。直接把方法访问类型改成`public`即可。
<a name="XL1B0"></a>
### 3、不同的数据源
错误案例：
```java
public class TransactionalMistake {

    @Transactional
    public void createOrder(Order order) {
        orderRepo1.save(order);
        orderRepo2.save(order);
    }

}
```
有的时候，一个操作可能会同时写多个数据源，比如上面这个例子里的orderRepo1和orderRepo2是连接的两个不同数据源。默认情况下，这种跨数据源的事务是不会成功的。
<a name="mewJ8"></a>
### 4、回滚异常配置不正确
默认情况下，仅对`RuntimeException`和`Error`进行回滚。如果不是的它们及它们的子孙异常的话，就不会回滚。<br />所以，在自定义异常的时候，要做好适当的规划，如果要影响事务回滚，可以定义为`RuntimeException`的子类；如果不是`RuntimeException`，但也希望触发回滚，那么可以使用`rollbackFor`属性来指定要回滚的异常。
```java
public class TransactionalMistake {

    @Transactional(rollbackFor = XXXException.class)
    public void method() throws XXXException {

    }
}
```
<a name="KscQb"></a>
### 5、数据库引擎不支持事务
```
spring.jpa.database-platform=org.hibernate.dialect.MySQL5InnoDBDialect
```
这里的`spring.jpa.database-platform`配置主要用来设置hibernate使用的方言。这里特地采用了`MySQL5InnoDBDialect`，主要为了保障在使用Spring Data JPA时候，Hibernate自动创建表的时候使用InnoDB存储引擎，不然就会以默认存储引擎MyISAM来建表，而MyISAM存储引擎是没有事务的。<br />如果事务没有生效，那么可以看看创建的表，是不是使用了MyISAM存储引擎，如果是的话，那就是这个原因了！
