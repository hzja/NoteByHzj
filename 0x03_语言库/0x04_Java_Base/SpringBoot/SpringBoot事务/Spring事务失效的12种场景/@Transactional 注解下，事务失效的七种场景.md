JavaSpringBoot<br />`@Transactional`是一种基于注解管理事务的方式，spring通过动态代理的方式为目标方法实现事务管理的增强。<br />`@Transactional`使用起来方便，但也需要注意引起`@Transactional`失效的场景，本文总结了七种情况，下面进行逐一分析。
<a name="QA59x"></a>
### 1、异常被捕获后没有抛出
当异常被捕获后，并且没有再抛出，那么deleteUserA是不会回滚的。
```java
@Transactional
public void deleteUser() {
    userMapper.deleteUserA();
    try {
        int i = 1 / 0;
        userMapper.deleteUserB();
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
<a name="vdCQA"></a>
### 2、抛出非运行时异常
异步虽然抛出了，但是抛出的是非RuntimeException类型的异常，依旧不会生效。
```java
@Transactional
public void deleteUser() throws MyException{
    userMapper.deleteUserA();
    try {
        int i = 1 / 0;
        userMapper.deleteUserB();
    } catch (Exception e) {
        throw new MyException();
    }
}
```
如果指定了回滚异常类型为Exception，那么就可以回滚非RuntimeException类型异常了。
```java
@Transactional(rollbackFor = Exception.class)
```
<a name="Tay9J"></a>
### 3、方法内部直接调用
如果先调用`deleteUser()`，那么`deleteUserA()`是不会回滚的，其原因就是`@Transactional`根本没生成代理，如果直接调用`deleteUser2()`那么没问题，`deleteUserA()`会回滚。
```java
public void deleteUser() throws MyException{
    deleteUser2();
}

@Transactional
public void deleteUser2() throws MyException{
    userMapper.deleteUserA();
    int i = 1 / 0;
    userMapper.deleteUserB();
}
```
修改方式，把当前类自己注入一下调用即可。
```java
@Service
public class UserService {
    @Autowired
    private UserMapper userMapper;
    //自己注入自己
    @Autowired
    UserService userService;

    public void deleteUser() throws MyException{
        userService.deleteUser2();
    }

    @Transactional
    public void deleteUser2() throws MyException{
        userMapper.deleteUserA();
        int i = 1 / 0;
        userMapper.deleteUserB();
    }
}
```
<a name="h8xw3"></a>
### 4、新开启一个线程
如下的方式`deleteUserA()`也不会回滚，因为Spring实现事务的原理是通过ThreadLocal把数据库连接绑定到当前线程中，新开启一个线程获取到的连接就不是同一个了。
```java
@Transactional
public void deleteUser() throws MyException{
    userMapper.deleteUserA();
 try {
  //休眠1秒，保证deleteUserA先执行
        Thread.sleep(1000);
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    new Thread(() -> {
        int i = 1/0;
        userMapper.deleteUserB();
    }).start();    
}
```
<a name="d98fu"></a>
### 5、注解到`private`方法上
IDEA直接会给出提示Methods annotated with '@Transactional' must be overridable ，原理很简单，`private`修饰的方式，Spring无法生成动态代理。
```java
@Transactional
private void deleteUser() throws MyException{
    userMapper.deleteUserA();
    int i = 1/0;
    userMapper.deleteUserB();
}
```
<a name="J7DxS"></a>
### 6、数据库本身不支持
MySQL数据库，必须设置数据库引擎为InnoDB。
<a name="qsGVT"></a>
### 7、事务传播属性设置错误
注意传播属性的设置，比如设置了：`PROPAGATION_NOT_SUPPORIED`（以非事务的方式执行，如果当前有事务则把当前事务挂起）。
