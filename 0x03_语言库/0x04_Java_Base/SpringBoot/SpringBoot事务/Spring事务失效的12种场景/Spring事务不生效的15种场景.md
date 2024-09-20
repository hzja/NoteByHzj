JavaSpring事务
<a name="kx8ly"></a>
## 前言
日常开发中，经常使用到Spring事务。那么**Spring 事务在哪几种情况下会不生效？** 来总结一下Spring事务**不生效**的15种场景。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680485231512-304f8881-7a27-4168-bac3-4cfe221c9719.jpeg)
<a name="WkcVO"></a>
## 1、service类没有被Spring管理
```java
//@Service （注释了@Service）
public class TianLuoServiceImpl implements TianLuoService {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional
    public void addTianLuo(TianLuo tianluo) {
        //保存tianluo实体数据库记录
        tianLuoMapper.save(tianluo);
        //保存tianluo流水数据库记录
        tianLuoFlowMapper.saveFlow(buildFlowByTianLuo(tianluo));
    }
}
```

- **事务不生效的原因**：上面例子中， `@Service`注解注释之后，spring事务（`@Transactional`）没有生效，因为Spring事务是由AOP机制实现的，也就是说从Spring IOC容器获取bean时，Spring会为目标类创建代理，来支持事务的。但是`@Service`被注释后，service类都不是spring管理的，**那怎么创建代理类来支持事务呢**。
- **解决方案**：加上`@Service`注解。
<a name="tLURb"></a>
## 2、没有在Spring配置文件中启用事务管理器
```java
@Configuration
public class AppConfig {
    // 没有配置事务管理器
}

@Service
public class MyService {
    @Transactional
    public void doSomething() {
        // ...
    }
}
```

- **事务不生效的原因**：没有在`AppConfig`中配置事务管理器，因此Spring无法创建事务代理对象，导致事务不生效。即使在MyService中添加了`@Transactional`注解，该方法也不会被Spring管理的事务代理拦截。
- **解决方案**：为了解决这个问题，应该在AppConfig中配置一个事务管器。例如：
```java
@Configuration
public class AppConfig {
    @Bean
    public PlatformTransactionManager transactionManager() {
        return new DataSourceTransactionManager(dataSource());
    }
}

@Service
public class MyService {
    @Transactional
    public void doSomething() {
        // ...
    }
}
```
如果是Spring Boot项目，它默认会自动配置事务管理器并开启事务支持。
<a name="ybUJ5"></a>
## 3、事务方法被`final`、`static`关键字修饰
```java
@Service
public class TianLuoServiceImpl  {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional
    public final void addTianLuo(TianLuo tianluo) {
        //保存tianluo实体数据库记录
        tianLuoMapper.save(tianluo);
        //保存tianluo流水数据库记录
        tianLuoFlowMapper.saveFlow(buildFlowByTianLuo(tianluo));
    }
}
```

- **事务不生效的原因**：如果一个方法被声明为`final`或者`static`，则该方法不能被子类重写，也就是说无法在该方法上进行动态代理，这会导致Spring无法生成事务代理对象来管理事务。
- **解决方案**：`addTianLuo`**事务方法**不要用`final`修饰或者`static`修饰。
<a name="KWV3O"></a>
## 4、同一个类中，方法内部调用
```java
@Service
public class TianLuoServiceImpl implements TianLuoService {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    public void addTianLuo(TianLuo tianluo){
        // 调用内部的事务方法
        this.executeAddTianLuo(tianluo);
    }

    @Transactional
    public void executeAddTianLuo(TianLuo tianluo) {
        tianLuoMapper.save(tianluo);
        tianLuoFlowMapper.saveFlow(buildFlowByTianLuo(tianluo));
    }
}
```

- **事务不生效的原因**: 事务是通过Spring AOP代理来实现的，而在同一个类中，一个方法调用另一个方法时，**调用方法直接调用目标方法的代码，而不是通过代理类进行调用**。即以上代码，调用目标`executeAddTianLuo`方法不是通过代理类进行的，因此事务不生效。
- **解决方案**：可以新建多一个类，让这两个方法分开，分别在不同的类中。如下：
```java
@Service
public class TianLuoExecuteServiceImpl implements TianLuoExecuteService {

    @Autowired
    private TianLuoMapper tianLuoMapper;
    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional
    public void executeAddTianLuo(TianLuo tianluo) {
        tianLuoMapper.save(tianluo);
        tianLuoFlowMapper.saveFlow(buildFlowByTianLuo(tianluo));
    }
}

@Service
public class TianLuoAddServiceImpl implements TianLuoAddService {

    @Autowired
    private TianLuoExecuteService tianLuoExecuteService;

    public void addTianLuo(User user){
        tianLuoExecuteService.executeAddTianLuo(user);
    }
}
```
当然，有时候也可以在该 Service 类中注入自己，或者通过`AopContext.currentProxy()`获取代理对象。
<a name="JTfbd"></a>
## 5、方法的访问权限不是`public`
```java
@Service
public class TianLuoServiceImpl implements TianLuoService {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional
    private void addTianLuo(TianLuo tianluo) {
        tianLuoMapper.save(tianluo);
        tianLuoFlowMapper.saveFlow(buildFlowByTianLuo(tianluo));
    }
}
```

- **事务不生效的原因**：Spring事务方法`addTianLuo`的访问权限不是`public`，所以事务就不生效，因为Spring事务是由AOP机制实现的，AOP机制的本质就是动态代理，而代理的事务方法不是`public`的话，`computeTransactionAttribute()`就会返回null，也就是这时事务属性不存在了。大家可以看下`AbstractFallbackTransactionAttributeSource`的源码：

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680484372451-638ed150-24f7-4920-bf7a-963ef1b34c5c.png#averageHue=%23312d2c&clientId=u8df42cb9-e5cc-4&from=paste&id=u95442d97&originHeight=560&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u972df55d-5340-48ba-970d-07961eb7f06&title=)

- **解决方案**：`addTianLuo`事务方法的访问权限修改为`public`。
<a name="otj2c"></a>
## 6、数据库的存储引擎不支持事务
Spring事务的底层，还是依赖于数据库本身的事务支持。在MySQL中，MyISAM存储引擎是不支持事务的，InnoDB引擎才支持事务。因此开发阶段设计表的时候，**确认选择的数据库存储引擎是支持事务的**。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680484372378-20e93ad5-e2c2-4fe6-8854-58f072ac4af3.png#averageHue=%23292923&clientId=u8df42cb9-e5cc-4&from=paste&id=u5c70261c&originHeight=311&originWidth=1049&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud191b1be-c5f2-4ab2-9a3e-097c7941e66&title=)
<a name="Z5PxQ"></a>
## 7、配置错误的 `@Transactional` 注解
```java
@Transactional(readOnly = true)
public void updateUser(User user) {
    userDao.updateUser(user);
}
```

- **事务不生效的原因**：虽然使用了`@Transactional`注解，但是注解中的`readOnly=true`属性指示这是一个只读事务，因此在更新User实体时会抛出异常。
- **解决方案**：将`readOnly`属性设置为`false`，或者移除了`@Transactional`注解中的`readOnly`属性。
<a name="Xv6pD"></a>
## 8、事务超时时间设置过短
```java
@Transactional(timeout = 1)
public void doSomething() {
    //...
}
```

- **事务不生效的原因**：在上面的例子中，timeout属性被设置为1秒，这意味着如果事务在1 秒内无法完成，则报事务超时了。
<a name="IYpqY"></a>
## 9、使用了错误的事务传播机制
```java
@Service
public class TianLuoServiceImpl {

    @Autowired
    private TianLuoMapper tianLuoMapper;
    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional(propagation = Propagation.NOT_SUPPORTED)
    public  void doInsertTianluo(TianLuo tianluo) throws Exception {
        tianLuoMapper.save(tianluo);
        tianLuoFlowMapper.saveFlow(buildFlowByTianLuo(tianluo));
    }
}
```

- **事务不生效的原因**：`Propagation.NOT_SUPPORTED`传播特性不支持事务。
- **解决方案**：选择正确的事务传播机制。

帮大家复习一下，Spring提供了七种事务传播机制。它们分别是：

- `REQUIRED`（默认）：如果当前存在一个事务，则加入该事务；否则，创建一个新事务。该传播级别表示方法必须在事务中执行。
- `SUPPORTS`：如果当前存在一个事务，则加入该事务；否则，以非事务的方式继续执行。
- `MANDATORY`：如果当前存在一个事务，则加入该事务；否则，抛出异常。
- `REQUIRES_NEW`：创建一个新的事务，并且如果存在一个事务，则将该事务挂起。
- `NOT_SUPPORTED`：以非事务方式执行操作，如果当前存在一个事务，则将该事务挂起。
- `NEVER`：以非事务方式执行操作，如果当前存在一个事务，则抛出异常。
- `NESTED`：如果当前存在一个事务，则在嵌套事务内执行。如果没有事务，则按`REQUIRED`传播级别执行。嵌套事务是外部事务的一部分，可以在外部事务提交或回滚时部分提交或回滚。
<a name="I4Wik"></a>
## 10、`rollbackFor`属性配置错误
```java
@Service
public class TianLuoServiceImpl implements TianLuoService {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional(rollbackFor = Error.class)
    public void addTianLuo(TianLuo tianluo) {
        //保存tianluo数据库记录
        tianLuoMapper.save(tianluo);
        //保存tianluo流水数据库记录
        tianLuoFlowMapper.saveFlow(tianluo);
        //模拟异常抛出
        throw new Exception();
    }
}
```

- **事务不生效的原因**：其实`rollbackFor`属性指定的异常必须是`Throwable`或者其子类。默认情况下，`RuntimeException`和Error两种异常都是会自动回滚的。但是因为以上的代码例子，指定了`rollbackFor = Error.class`，但是抛出的异常又是`Exception`，而`Exception`和Error没有任何什么继承关系，因此事务就不生效。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680485532356-adbb8887-478f-42a6-861c-f59c5a2ca733.jpeg)<br />可以看一下`Transactional`注解源码：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680484372388-19a36bd2-8d84-424d-99b5-26a0e5bd0de7.png#averageHue=%23744131&clientId=u8df42cb9-e5cc-4&from=paste&id=uda7e695c&originHeight=806&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1f328579-606c-427d-9413-0feff543257&title=)

- **解决方案**：`rollbackFor`属性指定的异常与抛出的异常匹配。
<a name="pVh5e"></a>
## 11、事务注解被覆盖导致事务失效
```java
public interface MyRepository {
    @Transactional
    void save(String data);
}

public class MyRepositoryImpl implements MyRepository {
    @Override
    public void save(String data) {
        // 数据库操作
    }
}

public class MyService {

    @Autowired
    private MyRepository myRepository;

    @Transactional
    public void doSomething(String data) {
        myRepository.save(data);
    }
}

public class MyTianluoService extends MyService {
    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void doSomething(String data) {
        super.doSomething(data);
    }
}
```

- **事务失效的原因**：`MyTianluoService`是MyService的子类，并且覆盖了`doSomething()`方法。在该方法中，使用了不同的传播行为（`REQUIRES_NEW`）来覆盖父类的`@Transactional`注解。在这种情况下，当调用`MyTianluoService`的`doSomething()`方法时，由于子类方法中的注解覆盖了父类的注解，Spring框架**将不会在父类的方法中启动事务**。因此，当`MyRepository`的`save()`方法被调用时，事务将不会被启动，也不会回滚。这将导致数据不一致的问题，因为在`MyRepository`的`save()`方法中进行的数据库操作将不会回滚。
<a name="rPzGg"></a>
## 12、嵌套事务的坑
```java
@Service
public class TianLuoServiceInOutService {

    @Autowired
    private TianLuoFlowService tianLuoFlowService;
    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Transactional
    public void addTianLuo(TianLuo tianluo) throws Exception {
        tianLuoMapper.save(tianluo);
        tianLuoFlowService.saveFlow(tianluo);
    }
}

@Service
public class TianLuoFlowService {

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional(propagation = Propagation.NESTED)
    public void saveFlow(TianLuo tianLuo) {
        tianLuoFlowMapper.save(tianLuo);
        throw new RuntimeException();
    }
}
```
以上代码使用了嵌套事务，如果`saveFlow`出现运行时异常，会继续往上抛，到外层`addTianLuo`的方法，导致`tianLuoMapper.save`也会回滚。**如果不想因为被内部嵌套的事务影响**，可以用try-catch包住，如下：
```java
@Transactional
public void addTianLuo(TianLuo tianluo) throws Exception {
    tianLuoMapper.save(tianluo);
    try {
        tianLuoFlowService.saveFlow(tianluo);
    } catch (Exception e) {
        log.error("save tian luo flow fail,message:{}",e.getMessage());
    }
}
```
<a name="emaXg"></a>
## 13、事务多线程调用
```java
@Service
public class TianLuoService {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowService tianLuoFlowService;

    @Transactional
    public void addTianLuo(TianLuo tianluo) {
        //保存tianluo数据库记录
        tianLuoMapper.save(tianluo);
        //多线程调用
        new Thread(() -> {
            tianLuoFlowService.saveFlow(tianluo);
        }).start();
    }
}

@Service
public class TianLuoFlowService {

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional
    public void save(TianLuo tianLuo) {
        tianLuoFlowMapper.saveFlow(tianLuo);
    }
}
```

- **事务不生效原因**：这是因为Spring事务是基于线程绑定的，**每个线程都有自己的事务上下文**，而多线程环境下可能会存在多个线程共享同一个事务上下文的情况，导致事务不生效。Spring事务管理器通过使用线程本地变量（`ThreadLocal`）来实现线程安全。大家有兴趣的话，可以去看下源码。

在Spring事务管理器中，通过`TransactionSynchronizationManager`类来管理事务上下文。`TransactionSynchronizationManager`内部维护了一个`ThreadLocal`对象，用来存储当前线程的事务上下文。在事务开始时，`TransactionSynchronizationManager`会将事务上下文绑定到当前线程的`ThreadLocal`对象中，当事务结束时，`TransactionSynchronizationManager`会将事务上下文从`ThreadLocal`对象中移除。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680484373042-e6614c18-8263-4223-afe3-9240a614fe4e.png#averageHue=%232e2d2c&clientId=u8df42cb9-e5cc-4&from=paste&id=ubeda2341&originHeight=647&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u914e0dda-21b2-4871-bf93-38c8a57cc8f&title=)
<a name="XTwqZ"></a>
## 14、异常被捕获并处理了，没有重新抛出
```java
@Service
public class TianLuoServiceImpl implements TianLuoService {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional
    public void addTianLuo(TianLuo tianluo) {
        try {
            //保存tianluo数据库记录
            tianLuoMapper.save(tianluo);
            //保存tianluo flow数据库记录
            tianLuoFlowMapper.saveFlow(tianluo);
        } catch (Exception e) {
            log.error("add TianLuo error,id:{},message:{}", tianluo.getId(),e.getMessage());
        }
    }

}
```

- **事务不生效的原因**：事务中的异常已经被业务代码捕获并处理，而没有被正确地传播回事务管理器，事务将无法回滚。可以从spring源码(`TransactionAspectSupport`这个类)中找到答案：
```java
public abstract class TransactionAspectSupport implements BeanFactoryAware, InitializingBean {

    //这方法会省略部分代码，只留关键代码
    @Nullable
    protected Object invokeWithinTransaction(Method method, @Nullable Class<?> targetClass, final InvocationCallback invocation) throws Throwable {

        if (txAttr == null || !(ptm instanceof CallbackPreferringPlatformTransactionManager)) {

            TransactionInfo txInfo = createTransactionIfNecessary(ptm, txAttr, joinpointIdentification);
            Object retVal;
            try {
                //Spring AOP中MethodInterceptor接口的一个方法，它允许拦截器在执行被代理方法之前和之后执行额外的逻辑。
                retVal = invocation.proceedWithInvocation();
            }
            catch (Throwable ex) {
                //用于在发生异常时完成事务(如果Spring catch不到对应的异常的话，就不会进入回滚事务的逻辑)
                completeTransactionAfterThrowing(txInfo, ex);
                throw ex;
            }
            finally {
                cleanupTransactionInfo(txInfo);
            }

            //用于在方法正常返回后提交事务。
            commitTransactionAfterReturning(txInfo);
            return retVal;
        }
    }
```
在`invokeWithinTransaction`方法中，当Spring catch到Throwable异常的时候，就会调用`completeTransactionAfterThrowing()`方法进行事务回滚的逻辑。但是，在`TianLuoServiceImpl`类的spring事务方法`addTianLuo`中，直接把异常catch住了，并没有重新throw出来，因此 Spring自然就catch不到异常，因此事务回滚的逻辑就不会执行，事务就失效了。

- **解决方案**：在spring事务方法中，当使用了try-catch，如果catch住异常，记录完异常日志什么的，一定要重新把异常抛出来，正例如下：
```java
@Service
public class TianLuoServiceImpl implements TianLuoService {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional(rollbackFor = Exception.class)
    public void addTianLuo(TianLuo tianluo) {
        try {
            //保存tianluo数据库记录
            tianLuoMapper.save(tianluo);
            //保存tianluo flow数据库记录
            tianLuoFlowMapper.saveFlow(tianluo);
        } catch (Exception e) {
            log.error("add TianLuo error,id:{},message:{}", tianluo.getId(),e.getMessage());
            throw e;
        }
    }
}
```
<a name="I0I8E"></a>
## 15、手动抛了别的异常
```java
@Service
public class TianLuoServiceImpl implements TianLuoService {

    @Autowired
    private TianLuoMapper tianLuoMapper;

    @Autowired
    private TianLuoFlowMapper tianLuoFlowMapper;

    @Transactional
    public void addTianLuo(TianLuo tianluo) throws Exception {
        //保存tianluo数据库记录
        tianLuoMapper.save(tianluo);
        //保存tianluo流水数据库记录
        tianLuoFlowMapper.saveFlow(tianluo);
        throw new Exception();
    }
}
```

- **失效的原因**：上面的代码例子中，手动抛了`Exception`异常，但是是不会回滚的，因为Spring默认只处理`RuntimeException`和Error，对于普通的`Exception`不会回滚，除非，用`rollbackFor`属性指定配置。
- 解决方案：添加属性配置`@Transactional(rollbackFor = Exception.class)`。

注解为事务范围的方法中，事务的回滚仅仅对于unchecked的异常有效。对于checked异常无效。也就是说事务回滚仅仅发生在，出现RuntimeException或Error的时候。通俗一点就是：代码中出现的空指针等异常，会被回滚。而文件读写、网络超时问题等，spring就没法回滚了。
