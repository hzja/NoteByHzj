Java SpringBoot
<a name="UA4PN"></a>
## 一、简介
基于 SpringBoot 平台开发的项目数不胜数，与常规的基于Spring开发的项目最大的不同之处，SpringBoot 里面提供了大量的注解用于快速开发，而且非常简单，基本可以做到开箱即用！<br />那 SpringBoot 为开发者提供了多少注解呢？该如何使用？这里进行了一番整理，内容如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648084142797-1218700f-84ec-4611-bc4f-13cc7f4c423e.png#clientId=ube0537fb-9f4a-4&from=paste&id=ub0fe57d0&originHeight=702&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4d2d6a75-5343-4d74-8aec-439422544e0&title=)
<a name="AVMgD"></a>
## 二、注解总结
<a name="gVfgb"></a>
### 2.1、SpringMVC 相关注解
<a name="WkoMQ"></a>
#### `@Controller`
通常用于修饰Controller层的组件，由控制器负责将用户发来的URL请求转发到对应的服务接口，通常还需要配合注解`@RequestMapping`使用。
<a name="JLLCE"></a>
#### `@RequestMapping`
提供路由信息，负责URL到Controller中具体函数的映射，当用于方法上时，可以指定请求协议，比如GET、POST、PUT、DELETE等等。
<a name="RerYL"></a>
#### `@RequestBody`
表示请求体的Content-Type必须为application/json格式的数据，接收到数据之后会自动将数据绑定到Java对象上去
<a name="y8nOi"></a>
#### `@ResponseBody`
表示该方法的返回结果直接写入HTTP response body中，返回数据的格式为application/json<br />比如，请求参数为json格式，返回参数也为json格式，示例代码如下：
```java
/**
 * 登录服务
 */
@Controller
@RequestMapping("api")
public class LoginController {
 
    /**
     * 登录请求，post请求协议，请求参数数据格式为json
     * @param request
     */
    @RequestMapping(value = "login", method = RequestMethod.POST)
    @ResponseBody
    public ResponseEntity login(@RequestBody UserLoginDTO request){
     //...业务处理
        return new ResponseEntity(HttpStatus.OK);
    }
}
```
<a name="xyeWu"></a>
#### `@RestController`
和`@Controller`一样，用于标注控制层组件，不同的地方在于：它是`@ResponseBody`和`@Controller`的合集，也就是说，在当`@RestController`用在类上时，表示当前类里面所有对外暴露的接口方法，返回数据的格式都为application/json，示范代码如下：
```java
@RestController
@RequestMapping("api")
public class LoginController {
 
    /**
     * 登录请求，post请求协议，请求参数数据格式为json
     * @param request
     */
    @RequestMapping(value = "login", method = RequestMethod.POST)
    public ResponseEntity login(@RequestBody UserLoginDTO request){
        //...业务处理
        return new ResponseEntity(HttpStatus.OK);
    }
}
```
<a name="TCVFo"></a>
#### `@RequestParam`
用于接收请求参数为表单类型的数据，通常用在方法的参数前面，示范代码如下：
```java
/**
 * 登录请求，post请求协议，请求参数数据格式为表单
 */
@RequestMapping(value = "login", method = RequestMethod.POST)
@ResponseBody
public ResponseEntity login(@RequestParam(value = "userName",required = true) String userName,
                            @RequestParam(value = "userPwd",required = true) String userPwd){
    //...业务处理
    return new ResponseEntity(HttpStatus.OK);
}
```
<a name="mNPeW"></a>
#### `@PathVariable`
用于获取请求路径中的参数，通常用于restful风格的api上，示范代码如下：
```java
/**
 * restful风格的参数请求
 * @param id
 */
@RequestMapping(value = "queryProduct/{id}", method = RequestMethod.POST)
@ResponseBody
public ResponseEntity queryProduct(@PathVariable("id") String id){
    //...业务处理
    return new ResponseEntity(HttpStatus.OK);
}
```
<a name="IHSGm"></a>
#### `@GetMapping`
除了`@RequestMapping`可以指定请求方式之外，还有一些其他的注解，可以用于标注接口路径请求，比如GetMapping用在方法上时，表示只支持get请求方法，等价于`@RequestMapping(value="/get",method=RequestMethod.GET)`
```java
@GetMapping("get")
public ResponseEntity get(){
    return new ResponseEntity(HttpStatus.OK);
}
```
<a name="URpMG"></a>
#### `@PostMapping`
用在方法上，表示只支持post方式的请求。
```java
@PostMapping("post")
public ResponseEntity post(){
    return new ResponseEntity(HttpStatus.OK);
}
```
<a name="m8duG"></a>
#### `@PutMapping`
用在方法上，表示只支持put方式的请求，通常表示更新某些资源的意思
```java
@PutMapping("put")
public ResponseEntity put(){
    return new ResponseEntity(HttpStatus.OK);
}
```
<a name="n46UY"></a>
#### `@DeleteMapping`
用在方法上，表示只支持delete方式的请求，通常表示删除某些资源的意思
```java
@DeleteMapping("delete")
public ResponseEntity delete(){
    return new ResponseEntity(HttpStatus.OK);
}
```
<a name="vxm3i"></a>
### 2.2、bean 相关注解
<a name="tzv5y"></a>
#### `@Service`
通常用于修饰service层的组件，声明一个对象，会将类对象实例化并注入到bean容器里面
```java
@Service
public class DeptService {
 
 //具体的方法
}
```
<a name="Lxxbp"></a>
#### `@Component`
泛指组件，当组件不好归类的时候，可以使用这个注解进行标注，功能类似于于`@Service`
```java
@Component
public class DeptService {
 
 //具体的方法
}
```
<a name="A9puh"></a>
#### `@Repository`
通常用于修饰dao层的组件，<br />`@Repository`注解属于Spring里面最先引入的一批注解，它用于将数据访问层 (DAO层 ) 的类标识为Spring Bean，具体只需将该注解标注在 DAO类上即可，示例代码如下：
```java
@Repository
public interface RoleRepository extends JpaRepository<Role,Long> {

 //具体的方法
}
```
为什么现在使用的很少呢？<br />主要是因为当配置服务启动自动扫描dao层包时，Spring会自动创建一个实现类，然后注入到bean容器里面。当某些类无法被扫描到时，可以显式的在数据持久类上标注`@Repository`注解，Spring会自动声明对象。
<a name="cBvi2"></a>
#### `@Bean`
相当于 xml 中配置 Bean，意思是产生一个 bean 对象，并交给spring管理，示例代码如下：
```java
@Configuration
public class AppConfig {
 
   //相当于 xml 中配置 Bean
    @Bean
    public Uploader initFileUploader() {
        return new FileUploader();
    }

}
```
<a name="iUpLW"></a>
#### `@Autowired`
自动导入依赖的bean对象，默认时按照`byType`方式导入对象，而且导入的对象必须存在，当需要导入的对象并不存在时，可以通过配置`required = false`来关闭强制验证。
```java
@Autowired
private DeptService deptService;
```
<a name="V1ezZ"></a>
#### `@Resource`
也是自动导入依赖的bean对象，**由JDK提供**，默认是按照`byName`方式导入依赖的对象；而`@Autowired`默认时按照byType方式导入对象，当然`@Resource`还可以配置成通过`byType`方式导入对象。
```java
/**
 * 通过名称导入（默认通过名称导入依赖对象）
 */
@Resource(name = "deptService")
private DeptService deptService;

/**
 * 通过类型导入
 */
@Resource(type = RoleRepository.class)
private DeptService deptService;
```
<a name="p2ZDR"></a>
#### `@Qualifier`
当有多个同一类型的bean时，使用`@Autowired`导入会报错，提示当前对象并不是唯一，Spring不知道导入哪个依赖，这个时候，可以使用`@Qualifier`进行更细粒度的控制，选择其中一个候选者，一般于`@Autowired`搭配使用，示例如下：
```java
@Autowired
@Qualifier("deptService")
private DeptService deptService;
```
<a name="ArVmi"></a>
#### `@Scope`
用于生命一个spring bean的作用域，作用的范围一共有以下几种：

- **singleton：唯一 bean 实例，Spring 中的 bean 默认都是单例的。**
- **prototype：每次请求都会创建一个新的 bean 实例，对象多例。**
- **request：每一次 HTTP 请求都会产生一个新的 bean，该 bean 仅在当前 HTTP request 内有效。**
- **session：每一次 HTTP 请求都会产生一个新的 bean，该 bean 仅在当前 HTTP session 内有效。**
```java
/**
 * 单例对象
 */
@RestController
@Scope("singleton")
public class HelloController {

}
```
<a name="ZdkZZ"></a>
### 2.3、JPA 相关注解
<a name="vz6Un"></a>
#### `@Entity`和`@Table`
表明这是一个实体类，这两个注解一般一块使用，但是如果表名和实体类名相同的话，`@Table`可以省略。
<a name="e8v08"></a>
#### `@Id`
表示该属性字段对应数据库表中的主键字段。
<a name="d6p7B"></a>
#### `@Column`
表示该属性字段对应的数据库表中的列名，如果字段名与列名相同，则可以省略。
<a name="H9Gmk"></a>
#### `@GeneratedValue`
表示主键的生成策略，有四个选项，分别如下：

- **AUTO：表示由程序控制，是默认选项 ，不设置就是这个**
- **IDENTITY：表示由数据库生成，采用数据库自增长，Oracle 不支持这种方式**
- **SEQUENCE：表示通过数据库的序列生成主键ID，MYSQL 不支持**
- **Table：表示由特定的数据库产生主键，该方式有利于数据库的移植**
<a name="zkPIh"></a>
#### `@SequenceGeneretor`
用来定义一个生成主键的序列，它需要与`@GeneratedValue`联合使用才有效，以`TB_ROLE`表为例，对应的注解配置如下：
```java
@Entity
@Table(name = "TB_ROLE")
@SequenceGenerator(name = "id_seq", sequenceName = "seq_repair",allocationSize = 1)
public class Role implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键ID，采用【id_seq】序列函数自增长
     */
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE,generator = "id_seq")
    private Long id;


    /* 角色名称
     */
    @Column(nullable = false)
    private String roleName;

    /**
     * 角色类型
     */
    @Column(nullable = false)
    private String roleType;
}
```
<a name="Gl52E"></a>
#### `@Transient`
表示该属性并非与数据库表的字段进行映射，ORM 框架会将忽略该属性。
```java
/**
 * 忽略该属性
 */
@Column(nullable = false)
@Transient
private String lastTime;
```
<a name="DKGYL"></a>
#### `@Basic(fetch=FetchType.LAZY)`
用在某些属性上，可以实现懒加载的效果，也就是当用到这个字段的时候，才会装载这个属性，如果配置成`fetch=FetchType.EAGER`，表示即时加载，也是默认的加载方式！
```java
/**
 * 延迟加载该属性
 */
@Column(nullable = false)
@Basic(fetch = FetchType.LAZY)
private String roleType;
```
<a name="k2mLR"></a>
#### `@JoinColumn`
用于标注表与表之间关系的字段，通常与`@OneToOne`、`@OneToMany`搭配使用，例如如下
```java
@Entity
@Table(name = "tb_login_log")
public class LoginLog implements Serializable {
 
 /**
  * 查询登录的用户信息
  */
 @OneToOne
 @JoinColumn(name = "user_id")
 private User user;
 
 //...get、set
}
```
<a name="ZQnTj"></a>
#### `@OneToOne`、`@OneToMany`和`@ManyToOne`
这三个注解，相当于hibernate配置文件中的一对一，一对多，多对一配置，比如下面的客户地址表，通过客户 ID，实现客户信息的查询。
```java
@Entity
@Table(name="address")
public class AddressEO implements java.io.Serializable {
 
  @ManyToOne(cascade = { CascadeType.ALL })
  @JoinColumn(name="customer_id")
  private CustomerEO customer;
 
  //...get、set
}
```
<a name="SwJDZ"></a>
### 2.4、配置相关注解
<a name="FBmU3"></a>
#### `@Configuration`
表示声明一个 Java 形式的配置类，Spring Boot 提倡基于 Java 的配置，相当于之前在 xml 中配置 bean，比如声明一个配置类`AppConfig`，然后初始化一个`Uploader`对象。
```java
@Configuration
public class AppConfig {

    @Bean
    public Uploader initOSSUploader() {
        return new OSSUploader();
    }

}
```
<a name="p2aUu"></a>
#### `@EnableAutoConfiguration`
`@EnableAutoConfiguration`可以帮助SpringBoot应用将所有符合条件的`@Configuration`配置类，全部都加载到当前SpringBoot里，并创建对应配置类的Bean，并把该Bean实体交给IoC容器进行管理。<br />某些场景下，如果想要避开某些配置类的扫描（包括避开一些第三方jar包下面的配置，可以这样处理。
```java
@Configuration
@EnableAutoConfiguration(exclude = { org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration.class})
public class AppConfig {

 //具有业务方法
}
```
<a name="I8DED"></a>
#### `@ComponentScan`
标注哪些路径下的类需要被Spring扫描，用于自动发现和装配一些Bean对象，默认配置是扫描当前文件夹下和子目录下的所有类，如果想指定扫描某些包路径，可以这样处理。
```java
@ComponentScan(basePackages = {"com.xxx.a", "com.xxx.b", "com.xxx.c"})
```
<a name="HWrPo"></a>
#### `@SpringBootApplication`
等价于使用`@Configuration`、`@EnableAutoConfiguration`、`@ComponentScan`这三个注解，通常用于全局启动类上，示例如下：
```java
@SpringBootApplication
public class PropertyApplication {

    public static void main(String[] args) {
        SpringApplication.run(PropertyApplication.class, args);
    }
}
```
把`@SpringBootApplication`换成`@Configuration`、`@EnableAutoConfiguration`、`@ComponentScan`这三个注解，一样可以启动成功，`@SpringBootApplication`只是将这三个注解进行了简化！
<a name="Tf1rv"></a>
#### `@EnableTransactionManagement`
表示开启事务支持，等同于 xml 配置方式的`<tx:annotation-driven />`
```java
@SpringBootApplication
@EnableTransactionManagement`
public class PropertyApplication {

    public static void main(String[] args) {
        SpringApplication.run(PropertyApplication.class, args);
    }
}
```
<a name="a96Qk"></a>
#### `@Conditional`
从 Spring4 开始，可以通过`@Conditional`注解实现按条件装载bean对象，目前 Spring Boot 源码中大量扩展了`@Condition`注解，用于实现智能的自动化配置，满足各种使用场景。下面列举几个常用的注解：

- `**@ConditionalOnBean**`**：当某个特定的**`**Bean**`**存在时，配置生效**
- `**@ConditionalOnMissingBean**`**：当某个特定的**`**Bean**`**不存在时，配置生效**
- `**@ConditionalOnClass**`**：当**`**Classpath**`**里存在指定的类，配置生效**
- `**@ConditionalOnMissingClass**`**：当**`**Classpath**`**里不存在指定的类，配置生效**
- `**@ConditionalOnExpression**`**：当给定的**`**SpEL**`**表达式计算结果为**`**true**`**，配置生效**
- `**@ConditionalOnProperty**`**：当指定的配置属性有一个明确的值并匹配，配置生效**

具体的应用案例如下：
```java
@Configuration
public class ConditionalConfig {


    /**
     * 当AppConfig对象存在时，创建一个A对象
     * @return
     */
    @ConditionalOnBean(AppConfig.class)
    @Bean
    public A createA(){
        return new A();
    }

    /**
     * 当AppConfig对象不存在时，创建一个B对象
     * @return
     */
    @ConditionalOnMissingBean(AppConfig.class)
    @Bean
    public B createB(){
        return new B();
    }


    /**
     * 当KafkaTemplate类存在时，创建一个C对象
     * @return
     */
    @ConditionalOnClass(KafkaTemplate.class)
    @Bean
    public C createC(){
        return new C();
    }

    /**
     * 当KafkaTemplate类不存在时，创建一个D对象
     * @return
     */
    @ConditionalOnMissingClass(KafkaTemplate.class)
    @Bean
    public D createD(){
        return new D();
    }


    /**
     * 当enableConfig的配置为true，创建一个E对象
     * @return
     */
    @ConditionalOnExpression("${enableConfig:false}")
    @Bean
    public E createE(){
        return new E();
    }


    /**
     * 当filter.loginFilter的配置为true，创建一个F对象
     * @return
     */
    @ConditionalOnProperty(prefix = "filter",name = "loginFilter",havingValue = "true")
    @Bean
    public F createF(){
        return new F();
    }
}
```
<a name="KPeb4"></a>
#### `@Value`
可以在任意 Spring 管理的 Bean 中通过这个注解获取任何来源配置的属性值，比如在application.properties文件里，定义了一个参数变量！
```
config.name=zhangsan
```
在任意的bean容器里面，可以通过`@Value`注解注入参数，获取参数变量值。
```java
@RestController
public class HelloController {

    @Value("${config.name}")
    private String config;

    @GetMapping("config")
    public String config(){
        return JSON.toJSONString(config);
    }
}
```
<a name="L1ZfT"></a>
#### `@ConfigurationProperties`
上面`@Value`在每个类中获取属性配置值的做法，其实是不推荐的。<br />一般在企业项目开发中，不会使用那么杂乱无章的写法而且维护也麻烦，通常会一次性读取一个 Java 配置类，然后在需要使用的地方直接引用这个类就可以多次访问了，方便维护，示例如下：<br />首先，在application.properties文件里定义好参数变量。
```
config.name=demo_1
config.value=demo_value_1
```
然后，创建一个 Java 配置类，将参数变量注入即可！
```java
@Component
@ConfigurationProperties(prefix = "config")
public class Config {

    public String name;

    public String value;

    //...get、set
}
```
最后，在需要使用的地方，通过ioc注入Config对象即可！
<a name="pY4e8"></a>
#### `@PropertySource`
这个注解是用来读取自定义的配置文件的，比如导入test.properties和bussiness.properties两个配置文件，用法如下：
```java
@SpringBootApplication
@PropertySource(value = {"test.properties","bussiness.properties"})
public class PropertyApplication {

    public static void main(String[] args) {
        SpringApplication.run(PropertyApplication.class, args);
    }
}
```
<a name="hr6Te"></a>
#### `@ImportResource`
用来加载 xml 配置文件，比如导入自定义的aaa.xml文件，用法如下：
```java
@ImportResource(locations = "classpath:aaa.xml")
@SpringBootApplication
public class PropertyApplication {

    public static void main(String[] args) {
        SpringApplication.run(PropertyApplication.class, args);
    }
}
```
<a name="f7Mmk"></a>
### 2.5、异常处理相关注解
<a name="jQt9m"></a>
#### `@ControllerAdvice`和`@ExceptionHandler`
通常组合使用，用于处理全局异常，示例代码如下：
```java
@ControllerAdvice
@Configuration
@Slf4j
public class GlobalExceptionConfig {
 
 private static final Integer GLOBAL_ERROR_CODE = 500;
 
 @ExceptionHandler(value = Exception.class)
    @ResponseBody
    public void exceptionHandler(HttpServletRequest request, HttpServletResponse response, Exception e) throws Exception {
        log.error("【统一异常处理器】", e);
        ResultMsg<Object> resultMsg = new ResultMsg<>();
        resultMsg.setCode(GLOBAL_ERROR_CODE);
        if (e instanceof CommonException) {
           CommonException ex = (CommonException) e;
           if(ex.getErrCode() != 0) {
                resultMsg.setCode(ex.getErrCode());
            }
            resultMsg.setMsg(ex.getErrMsg());
        }else {
            resultMsg.setMsg(CommonErrorMsg.SYSTEM_ERROR.getMessage());
        }
        WebUtil.buildPrintWriter(response, resultMsg);
    }
 
 
}
```
<a name="oCbo8"></a>
### 2.6、测试相关注解
<a name="a4DBm"></a>
#### `@ActiveProfiles`
一般作用于测试类上， 用于声明生效的 Spring 配置文件，比如指定application-dev.properties配置文件。
<a name="OjQdF"></a>
#### `@RunWith`和`@SpringBootTest`
一般作用于测试类上， 用于单元测试用，示例如下：
```java
@ActiveProfiles("dev")
@RunWith(SpringRunner.class)
@SpringBootTest
public class TestJunit {
    
    @Test
    public void executeTask() {
        //测试...
    }
}
```
