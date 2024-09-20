Java Spring 事务<br />在Spring中事务管理的方式有两种，编程式事务和声明式事务。先详细介绍一下两种事务的实现方式配置类。
<a name="KSB6G"></a>
## 编程式事务
```java
@Configuration
@EnableTransactionManagement
@ComponentScan("com.javashitang")
public class AppConfig {

    @Bean
    public DruidDataSource dataSource() {
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/test?characterEncoding=utf8&useSSL=true");
        ds.setUsername("test");
        ds.setPassword("test");
        ds.setInitialSize(5);
        return ds;
    }

    @Bean
    public DataSourceTransactionManager dataSourceTransactionManager() {
        return new DataSourceTransactionManager(dataSource());
    }

    @Bean
    public JdbcTemplate jdbcTemplate(DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    @Bean
    public TransactionTemplate transactionTemplate() {
        return new TransactionTemplate(dataSourceTransactionManager());
    }
}

public interface UserService {
    void addUser(String name, String location);
    default void doAdd(String name) {};
}

@Service
public class UserServiceV1Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private TransactionTemplate transactionTemplate;

    @Override
    public void addUser(String name, String location) {
        transactionTemplate.execute(new TransactionCallbackWithoutResult() {

            @Override
            protected void doInTransactionWithoutResult(TransactionStatus status) {
                try {
                    String sql = "insert into user (`name`) values (?)";
                    jdbcTemplate.update(sql, new Object[]{name});
                    throw new RuntimeException("保存用户信息失败");
                } catch (Exception e) {
                    e.printStackTrace();
                    status.setRollbackOnly();
                }
            }
        });
    }
}
```
可以看到编程式事务的方式并不优雅，因为业务代码和事务代码耦合到一块，当发生异常的时候还得需要手动回滚事务（比使用JDBC方便多类，JDBC得先关闭自动自动提交，然后根据情况手动提交或者回滚事务）<br />如果优化事务方法的执行？如何做？<br />**「其实完全可以用AOP来优化这种代码，设置好切点，当方法执行成功时提交事务，当方法发生异常时回滚事务，这就是声明式事务的实现原理」**<br />使用AOP后，当调用事务方法时，会调用到生成的代理对象，代理对象中加入了事务提交和回滚的逻辑。
<a name="euWJQ"></a>
## 声明式事务
Spring aop动态代理的方式有如下几种方法

1. JDK动态代理实现（基于接口）（JdkDynamicAopProxy）
2. CGLIB动态代理实现（动态生成子类的方式）（CglibAopProxy）
3. AspectJ适配实现

Spring aop默认只会使用JDK和CGLIB来生成代理对象
<a name="KLB9e"></a>
### `@Transactional`可以用在哪里？
`@Transactional`可以用在类，方法，接口上

1. 用在类上，该类的所有`public`方法都具有事务
2. 用在方法上，方法具有事务。当类和方法同时配置事务的时候，方法的属性会覆盖类的属性
3. 用在接口上，一般不建议这样使用，因为只有基于接口的代理会生效，如果Spring AOP使用cglib来实现动态代理，会导致事务失效（因为注解不能被继承）
<a name="v81Vn"></a>
### `@Transactional`失效的场景

1. `@Transactional`注解应用到非public方法（除非特殊配置，例如使用AspectJ 静态织入实现 AOP）
2. 自调用，因为`@Transactional`是基于动态代理实现的
3. 异常在代码中被try catch了
4. 异常类型不正确，默认只支持`RuntimeException`和Error，不支持检查异常
5. 事务传播配置不符合业务逻辑
<a name="PqAte"></a>
#### `@Transactional`注解应用到非`public`方法
参考Spring官方文档介绍，摘要、译文如下：
> When using proxies, you should apply the @Transactional annotation only to methods with public visibility. If you do annotate protected, private or package-visible methods with the @Transactional annotation, no error is raised, but the annotated method does not exhibit the configured transactional settings. Consider the use of AspectJ (see below) if you need to annotate non-public methods.

译文
> 使用代理时，您应该只将`@Transactional`注释应用于具有公共可见性的方法。如果使用`@Transactional`注释对受保护的、私有的或包可见的方法进行注释，则不会引发错误，但带注释的方法不会显示配置的事务设置。如果需要注释非公共方法，请考虑使用AspectJ（见下文）。

简言之：`@Transactional` 只能用于 `public` 的方法上，否则事务不会失效，如果要用在非 `public` 方法上，可以开启 AspectJ 代理模式。<br />目前，如果`@Transactional`注解作用在非`public`方法上，编译器也会给与明显的提示。<br />**「为什么只有**`**public**`**方法上的**`**@Transactional**`**注解才会生效？」**<br />首相JDK动态代理肯定只能是`public`，因为接口的权限修饰符只能是`public`。cglib代理的方式是可以代理`protected`方法的（`private`不行，子类访问不了父类的`private`方法）如果支持`protected`，可能会造成当切换代理的实现方式时表现不同，增大出现bug的可能性，所以统一一下。<br />**「如果想让非**`**public**`**方法也生效，可以考虑使用AspectJ」**
<a name="IISi7"></a>
#### 自调用，因为`@Transactional`是基于动态代理实现的
当自调用时，方法执行不会经过代理对象，所以会导致事务失效。例如通过如下方式调用`addUser`方法时，事务会失效
```java
// 事务失效
@Service
public class UserServiceV2Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Override
    public void addUser(String name, String location) {
        doAdd(name);
    }

    @Transactional
    public void doAdd(String name) {
        String sql = "insert into user (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{name});
        throw new RuntimeException("保存用户失败");
    }
}
```
可以通过如下方式解决

1. `@Autowired`注入自己，假如为self，然后通过self调用方法
2. `@Autowired ApplicationContext`，从`ApplicationContext`通过`getBean`获取自己，然后再调用
```java
// 事务生效
@Service
public class UserServiceV2Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private UserService userService;

    @Override
    public void addUser(String name, String location) {
        userService.doAdd(name);
    }

    @Override
    @Transactional
    public void doAdd(String name) {
        String sql = "insert into user (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{name});
        throw new RuntimeException("保存用户失败");
    }
}
```
<a name="1jixK"></a>
#### 异常在代码中被try catch了
这个逻辑从源码理解比较清晰，只有当执行事务抛出异常才能进入`completeTransactionAfterThrowing`方法，这个方法里面有回滚的逻辑，如果事务方法都没抛出异常就只会正常提交
```java
// org.springframework.transaction.interceptor.TransactionAspectSupport#invokeWithinTransaction

try {
  // This is an around advice: Invoke the next interceptor in the chain.
  // This will normally result in a target object being invoked.
  // 执行事务方法
  retVal = invocation.proceedWithInvocation();
}
catch (Throwable ex) {
  // target invocation exception
  completeTransactionAfterThrowing(txInfo, ex);
  throw ex;
}
finally {
  cleanupTransactionInfo(txInfo);
}
```
<a name="N2gaX"></a>
#### 非Spring容器管理的bean
基于这种失效场景，有工作经验的基本上是不会存在这种错误的；`@Service` 注解注释，`StudentServiceImpl` 类则不会被Spring容器管理，因此即使方法被`@Transactional`注解修饰，事务也亦然不会生效。<br />简单举例如下：
```java
//@Service
public class StudentServiceImpl implements StudentService {

    @Autowired
    private StudentMapper studentMapper;

    @Autowired
    private ClassService classService;

    @Override
    @Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
    public void insertClassByException(StudentDo studentDo) throws CustomException {
        studentMapper.insertStudent(studentDo);
        throw new CustomException();
    }
}
```
<a name="R8kwF"></a>
#### 注解修饰的方法被类内部方法调用
这种失效场景是日常开发中最常踩坑的地方；在类A里面有方法a 和方法b， 然后方法b上面用 `@Transactional`加了方法级别的事务，在方法a里面 调用了方法b， 方法b里面的事务不会生效。为什么会失效呢？：<br />其实原因很简单，Spring在扫描Bean的时候会自动为标注了`@Transactional`注解的类生成一个代理类（proxy），当有注解的方法被调用的时候，实际上是代理类调用的，代理类在调用之前会开启事务，执行事务的操作，但是同类中的方法互相调用，相当于`this.B()`，此时的B方法并非是代理类调用，而是直接通过原有的Bean直接调用，所以注解会失效。
```java
@Service
public class ClassServiceImpl implements ClassService {

    @Autowired
    private ClassMapper classMapper;

    public void insertClass(ClassDo classDo) throws CustomException {
        insertClassByException(classDo);
    }

    @Override
    @Transactional(propagation = Propagation.REQUIRED)
    public void insertClassByException(ClassDo classDo) throws CustomException {
        classMapper.insertClass(classDo);
        throw new RuntimeException();
    }
}

//测试用例：
@Test
    public void insertInnerExceptionTest() throws CustomException {
       classDo.setClassId(2);
       classDo.setClassName("java_2");
       classDo.setClassNo("java_2");

       classService.insertClass(classDo);
    }
```
测试结果：
```java
java.lang.RuntimeException
 at com.qxy.common.service.impl.ClassServiceImpl.insertClassByException(ClassServiceImpl.java:34)
 at com.qxy.common.service.impl.ClassServiceImpl.insertClass(ClassServiceImpl.java:27)
 at com.qxy.common.service.impl.ClassServiceImpl$$FastClassBySpringCGLIB$$a1c03d8.invoke(<generated>) 
```
虽然业务代码报错了，但是数据库中已经成功插入数据，事务并未生效；
<a name="wG1lq"></a>
##### 解决方案
类内部使用其代理类调用事务方法：以上方法略作改动
```java
public void insertClass(ClassDo classDo) throws CustomException {
    // insertClassByException(classDo);
    ((ClassServiceImpl)AopContext.currentProxy()).insertClassByException(classDo);
}
//测试用例：
@Test
public void insertInnerExceptionTest() throws CustomException {
    classDo.setClassId(3);
    classDo.setClassName("java_3");
    classDo.setClassNo("java_3");

    classService.insertClass(classDo);
}
```
业务代码抛出异常，数据库未插入新数据，达到目的，成功解决一个事务失效问题；<br />数据库数据未发生改变；<br />注意：一定要注意启动类上要添加`@EnableAspectJAutoProxy(exposeProxy = true)`注解，否则启动报错：
```java
java.lang.IllegalStateException: Cannot find current proxy: Set 'exposeProxy' property on Advised to 'true' to make it available.

 at org.springframework.aop.framework.AopContext.currentProxy(AopContext.java:69)
 at com.qxy.common.service.impl.ClassServiceImpl.insertClass(ClassServiceImpl.java:28)
```
<a name="bSPA9"></a>
#### 异常类型不正确，默认只支持`RuntimeException`和`Error`，不支持检查异常
异常体系图如下。当抛出检查异常时，Spring事务不会回滚。如果抛出任何异常都回滚，可以配置`rollbackFor`为`Exception`
```java
@Transactional(rollbackFor = Exception.class)
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1618449992758-d108f237-9f00-4364-ab13-a2f3b1a483d8.webp#clientId=uadb52352-0d72-4&from=paste&id=udff829de&originHeight=140&originWidth=140&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uae5ee531-d85e-4123-b674-a22deb0ff29&title=)
<a name="PbFQR"></a>
##### 解决方案：
`@Transactional`注解修饰的方法，加上`rollbackfor`属性值，指定回滚异常类型：`@Transactional(propagation = Propagation.REQUIRED,rollbackFor = Exception.class)`
```java
@Override
@Transactional(propagation = Propagation.REQUIRED,rollbackFor = Exception.class)
public void insertClassByException(ClassDo classDo) throws Exception {
    classMapper.insertClass(classDo);
    throw new Exception();
}
```
<a name="CogGI"></a>
#### 捕获异常后，却未抛出异常
在事务方法中使用try-catch，导致异常无法抛出，自然会导致事务失效。
```java
@Service
public class ClassServiceImpl implements ClassService {

    @Autowired
    private ClassMapper classMapper;

    //    @Override
    public void insertClass(ClassDo classDo) {
        ((ClassServiceImpl)AopContext.currentProxy()).insertClassByException(classDo);

    }

    @Override
    @Transactional(propagation = Propagation.REQUIRED,rollbackFor = Exception.class)
    public void insertClassByException(ClassDo classDo) {
        classMapper.insertClass(classDo);
        try {
            int i = 1 / 0;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

// 测试用例：
@Test
public void insertInnerExceptionTest() {
    classDo.setClassId(4);
    classDo.setClassName("java_4");
    classDo.setClassNo("java_4");

    classService.insertClass(classDo);
}
```
<a name="YJBok"></a>
##### 解决方案：捕获异常并抛出异常
```java
@Override
@Transactional(propagation = Propagation.REQUIRED,rollbackFor = Exception.class)
public void insertClassByException(ClassDo classDo) {
    classMapper.insertClass(classDo);
    try {
        int i = 1 / 0;
    } catch (Exception e) {
        e.printStackTrace();
        throw new RuntimeException();
    }
}
```
<a name="a59Sm"></a>
#### 事务传播配置不符合业务逻辑
假如说有这样一个场景，用户注册，依次保存用户基本信息到user表中，用户住址信息到地址表中，当保存用户住址信息失败时，也要保证用户信息注册成功。
```java
public interface LocationService {
    void addLocation(String location);
}

@Service
public class LocationServiceImpl implements LocationService {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Override
    @Transactional
    public void addLocation(String location) {
        String sql = "insert into location (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{location});
        throw new RuntimeException("保存地址异常");
    }
}

@Service
public class UserServiceV3Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private LocationService locationService;

    @Override
    @Transactional
    public void addUser(String name, String location) {
        String sql = "insert into user (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{name});
        locationService.addLocation(location);
    }
}
```
调用发现user表和location表都没有插入数据，并不符合期望，可能会说抛出异常了，事务当然回滚了。把调用`locationService`的部分加上`try catch`
```java
@Service
public class UserServiceV3Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private LocationService locationService;

    @Override
    @Transactional
    public void addUser(String name, String location) {
        String sql = "insert into user (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{name});
        try {
            locationService.addLocation(location);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
调用发现user表和location表还是都没有插入数据。这是因为在LocationServiceImpl中事务已经被标记成回滚了，所以最终事务还会回滚。<br />要想最终解决就不得不提到Spring的事务传播行为了。<br />`Transactional`的事务传播行为默认为`Propagation.REQUIRED`。**「如果当前存在事务，则加入该事务。如果当前没有事务，则创建一个新的事务」**<br />此时把LocationServiceImpl中Transactional的事务传播行为改成`Propagation.REQUIRES_NEW`即可<br />**「创建一个新事务，如果当前存在事务，则把当前事务挂起」**<br />所以最终的解决代码如下
```java
@Service
public class UserServiceV3Impl implements UserService {

    @Autowired
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private LocationService locationService;

    @Override
    @Transactional
    public void addUser(String name, String location) {
        String sql = "insert into user (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{name});
        try {
            locationService.addLocation(location);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

@Service
public class LocationServiceImpl implements LocationService {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Override
    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void addLocation(String location) {
        String sql = "insert into location (`name`) values (?)";
        jdbcTemplate.update(sql, new Object[]{location});
        throw new RuntimeException("保存地址异常");
    }
}
```
<a name="lvQwl"></a>
#### 事务传播行为设置异常
此种事务传播行为不是特殊自定义设置，基本上不会使用`Propagation.NOT_SUPPORTED`，不支持事务
```java
@Transactional(propagation = Propagation.NOT_SUPPORTED,rollbackFor = Exception.class)
public void insertClassByException(ClassDo classDo) {
    classMapper.insertClass(classDo);
    try {
        int i = 1 / 0;
    } catch (Exception e) {
        e.printStackTrace();
        throw new RuntimeException();
    }
}
```
<a name="mYOec"></a>
#### 数据库存储引擎不支持事务
以MySQL关系型数据为例，如果其存储引擎设置为 MyISAM，则事务失效，因为MyISMA 引擎是不支持事务操作的；<br />故若要事务生效，则需要设置存储引擎为InnoDB ；目前 MySQL 从5.5.5版本开始默认存储引擎是：InnoDB；
