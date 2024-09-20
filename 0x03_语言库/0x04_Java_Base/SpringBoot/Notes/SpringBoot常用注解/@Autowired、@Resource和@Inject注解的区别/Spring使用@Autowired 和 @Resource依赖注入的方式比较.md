Java Spring @Autowired @Resource
<a name="b00LD"></a>
### 注入方式
虽然当前有关Spring Framework（5.0.3）的文档仅定义了两种主要的注入类型，但实际上有三种：
<a name="Ak7dV"></a>
#### 基于构造函数的依赖注入
```java
public class UserServiceImpl implents UserService{
    private UserDao userDao;

    @Autowire
    public UserServiceImpl(UserDao userDao){
        this.userDao = userDao;
    }
}
```
<a name="G6lHY"></a>
#### 基于Setter的依赖注入
```java
public class UserServiceImpl implents UserService{
    private UserDao userDao;

    @Autowire
    public serUserDao(UserDao userDao){
        this.userDao = userDao;
    }
}
```
<a name="PDXbq"></a>
#### 基于字段的依赖注入
```java
public class UserServiceImpl implents UserService{
    @Autowire
    private UserDao userDao;
}
```
基于字段的依赖注入方式会在Idea当中吃到黄牌警告，但是这种使用方式使用的也最广泛，因为简洁方便。甚至可以在一些Spring指南中看到这种注入方法，尽管在文档中不建议这样做。
<a name="Tv7bd"></a>
### 基于字段的依赖注入缺点
<a name="Cp030"></a>
#### 对于有`final`修饰的变量不好使
Spring的IOC对待属性的注入使用的是set形式，但是`final`类型的变量在调用class的构造函数的这个过程当中就得初始化完成，这个是基于字段的依赖注入做不到的地方。只能使用基于构造函数的依赖注入的方式
<a name="x1fhj"></a>
#### 掩盖单一职责的设计思想
在OOP的设计当中有一个单一职责思想，如果采用的是基于构造函数的依赖注入的方式来使用Spring的IOC的时候，当注入的太多的时候，这个构造方法的参数就会很庞大，类似于下面。<br />当看到这个类的构造方法那么多参数的时候，自然而然的会想一下：这个类是不是违反了单一职责思想?但是使用基于字段的依赖注入不会让你察觉，你会沉浸在`@Autowire`当中
```java
public class VerifyServiceImpl implents VerifyService{

    private AccountService accountService;
    private UserService userService;
    private IDService idService;
    private RoleService roleService;
    private PermissionService permissionService;
    private EnterpriseService enterpriseService;
    private EmployeeService employService;
    private TaskService taskService;
    private RedisService redisService;
    private MQService mqService;

    public SystemLogDto(AccountService accountService, 
                        UserService userService, 
                        IDService idService, 
                        RoleService roleService, 
                        PermissionService permissionService, 
                        EnterpriseService enterpriseService, 
                        EmployeeService employService, 
                        TaskService taskService, 
                        RedisService redisService, 
                        MQService mqService) {
        this.accountService = accountService;
        this.userService = userService;
        this.idService = idService;
        this.roleService = roleService;
        this.permissionService = permissionService;
        this.enterpriseService = enterpriseService;
        this.employService = employService;
        this.taskService = taskService;
        this.redisService = redisService;
        this.mqService = mqService;
    }
}
```
<a name="XkZPA"></a>
#### 与Spring的IOC机制紧密耦合
当使用基于字段的依赖注入方式的时候，确实可以省略构造方法和`setter`这些个模板类型的方法，但是，把控制权全给Spring的IOC了，别的类想重新设置下某个注入属性，没法处理(当然反射可以做到)。<br />本身Spring的目的就是解藕和依赖反转，结果通过再次与类注入器（在本例中为Spring）耦合，失去了通过自动装配类字段而实现的对类的解耦，从而使类在Spring容器之外无效。
<a name="MdjzN"></a>
#### 隐藏依赖性
当使用Spring的IOC的时候，被注入的类应当使用一些`public`类型(构造方法，和`setter`类型方法)的方法来向外界表达：需要什么依赖。但是基于字段的依赖注入的方式，基本都是`private`形式的，`private`把属性都给封印到class当中了。
<a name="C3IMe"></a>
#### 无法对注入的属性进行安检
基于字段的依赖注入方式，在程序启动的时候无法拿到这个类，只有在真正的业务使用的时候才会拿到，一般情况下，这个注入的都是非null的，万一要是null怎么办，在业务处理的时候错误才爆出来，时间有点晚了，如果在启动的时候就暴露出来，那么bug就可以很快得到修复(当然可以加注解校验)。<br />如果想在属性注入的时候，想根据这个注入的对象操作点东西，无法办到。碰到过的例子：一些配置信息，有些人总是会配错误，等到了自己测试业务阶段才知道配错了，例如线程初始个数不小心配置成了3000，这个时候就需要再某些Value注入的时候做一个检测机制。
<a name="aUqph"></a>
### 结论
通过上面，可以看到，基于字段的依赖注入方式有很多缺点，应当避免使用基于字段的依赖注入。推荐的方法是使用基于构造函数和基于`setter`的依赖注入。对于必需的依赖项，建议使用基于构造函数的注入，以使它们成为不可变的，并防止它们为null。对于可选的依赖项，建议使用基于`Setter`的注入。
