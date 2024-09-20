Java SpringCloud
<a name="JksL7"></a>
## 1、Spring/SpringBoot
<a name="884a2640"></a>
### `@SpringBootApplication`
包含`@Configuration`、`@EnableAutoConfiguration`、`@ComponentScan`通常用在主类上；
<a name="e2287389"></a>
### `@Repository`
用于标注数据访问组件，即DAO组件；
<a name="4b308f93"></a>
### `@Service`
用于标注业务层组件；
<a name="zEE1x"></a>
### `@RestController`
用于标注控制层组件(如struts中的action)，包含`@Controller`和`@ResponseBody`；
<a name="dee1a9df"></a>
### `@Controller`
用于标注是控制层组件，需要返回页面时请用`@Controller`而不是`@RestController`；
<a name="MeFm8"></a>
### `@Component`
泛指组件，当组件不好归类的时候，可以使用这个注解进行标注；
<a name="Nzo86"></a>
### `@ResponseBody`
表示该方法的返回结果直接写入HTTP response body中，一般在异步获取数据时使用，在使用`@RequestMapping`后，返回值通常解析为跳转路径，加上`@ResponseBody`后返回结果不会被解析为跳转路径，而是直接写入HTTP response body中；比如异步获取json数据，加上`@ResponseBody`后，会直接返回json数据；
<a name="LpiDy"></a>
### `@RequestBody`
参数前加上这个注解之后，认为该参数必填。表示接受json字符串转为对象 List等；
<a name="826233fa"></a>
### `@ComponentScan`
组件扫描。如果扫描到有`@Component` `@Controller` `@Service`等这些注解的类，则把这些类注册为bean*；
<a name="0d7f3caa"></a>
### `@Configuration`
指出该类是 Bean 配置的信息源，相当于XML中的，一般加在主类上；
<a name="wDQQf"></a>
### `@Bean`
相当于XML中的,放在方法的上面，而不是类，意思是产生一个bean,并交给spring管理；
<a name="7jZit"></a>
### `@EnableAutoConfiguration`
让 Spring Boot 根据应用所声明的依赖来对 Spring 框架进行自动配置，一般加在主类上；
<a name="4c487e18"></a>
### `@AutoWired`
byType方式。把配置好的Bean拿来用，完成属性、方法的组装，它可以对类成员变量、方法及构造函数进行标注，完成自动装配的工作；当加上（required=false）时，就算找不到bean也不报错；
<a name="AHk4Y"></a>
### `@Qualifier`
当有多个同一类型的Bean时，可以用`@Qualifier("name")`来指定。与`@Autowired`配合使用；
<a name="uCoIU"></a>
### `@Resource(name="name",type="type")`
没有括号内内容的话，默认byName。与@Autowired干类似的事；
<a name="fe7b8332"></a>
### `@RequestMapping`
 RequestMapping是一个用来处理请求地址映射的注解，可用于类或方法上。用于类上，表示类中的所有响应请求的方法都是以该地址作为父路径；<br />该注解有六个属性: <br />params:指定request中必须包含某些参数值是，才让该方法处理。<br />headers:指定request中必须包含某些指定的header值，才能让该方法处理请求。  <br />value:指定请求的实际地址，指定的地址可以是URI Template 模式  <br />method:指定请求的method类型， GET、POST、PUT、DELETE等  <br />consumes:指定处理请求的提交内容类型（Content-Type），如application/json,text/html;  <br />produces:指定返回的内容类型，仅当request请求头中的(Accept)类型中包含该指定类型才返回。<br />`@GetMapping`、`@PostMapping`等:<br />相当于`@RequestMapping（value="/"，method=RequestMethod.Get\Post\Put\Delete`等） 。是个组合注解；
<a name="3NuaD"></a>
### `@RequestParam`
用在方法的参数前面。相当于 `request.getParameter()`；
<a name="85OQa"></a>
### `@PathVariable`
路径变量。如 
```java
RequestMapping("user/get/mac/{macAddress}")
public String getByMacAddress(@PathVariable("macAddress") String macAddress){
    //do something;
}
```
参数与大括号里的名字相同的话，注解后括号里的内容可以不填。
<a name="qddhw"></a>
## 2、Jpa
<a name="dIQII"></a>
### `@Entity`
<a name="BrVp7"></a>
### `@Table(name="")`
表明这是一个实体类。一般用于jpa ，这两个注解一般一块使用，但是如果表名和实体类名相同的话，@Table可以省略；
<a name="NL5KC"></a>
### `@MappedSuperClass`
用在确定是父类的entity上。父类的属性子类可以继承；
<a name="lNDJh"></a>
### `@NoRepositoryBean`
一般用作父类的repository，有这个注解，spring不会去实例化该repository；
<a name="yfBKB"></a>
### `@Column`
如果字段名与列名相同，则可以省略；
<a name="eaa78a44"></a>
### `@Id`
表示该属性为主键；
<a name="pml6y"></a>
### `@GeneratedValue(strategy=GenerationType.SEQUENCE,generator ="repair_seq")`
表示主键生成策略是sequence（可以为Auto、IDENTITY、native等，Auto表示可在多个数据库间切换），指定sequence的名字是repair_seq；
<a name="0dGEr"></a>
### `@SequenceGeneretor(name = "repair_seq", sequenceName ="seq_repair", allocationSize = 1)`
name为sequence的名称，以便使用，sequenceName为数据库的sequence名称，两个名称可以一致；
<a name="ecb5c52f"></a>
### `@Transient`
表示该属性并非一个到数据库表的字段的映射,ORM框架将忽略该属性. <br />如果一个属性并非数据库表的字段映射,就务必将其标示为`@Transient`,否则,ORM框架默认其注解为`@Basic`；
<a name="WbiIU"></a>
### `@Basic(fetch=FetchType.LAZY)`
标记可以指定实体属性的加载方式；
<a name="48db337b"></a>
### `@JsonIgnore`
作用是json序列化时将java bean中的一些属性忽略掉,序列化和反序列化都受影响；
<a name="3ceH3"></a>
### `@JoinColumn(name="loginId")`
一对一：本表中指向另一个表的外键。<br />一对多：另一个表指向本表的外键。
<a name="aef21f10"></a>
### `@OneToOne`、`@OneToMany`、`@ManyToOne`
对应Hibernate配置文件中的一对一，一对多，多对一。
<a name="IxveA"></a>
## 3、全局异常处理
<a name="ej5Cu"></a>
### `@ControllerAdvice`
包含`@Component`。可以被扫描到。统一处理异常；
<a name="p3nqA"></a>
### `@ExceptionHandler(Exception.class)`
用在方法上面表示遇到这个异常就执行以下方法。
<a name="HAvkK"></a>
## 4、SpringCloud
<a name="L4BTs"></a>
### `@EnableEurekaServer`
用在SpringBoot启动类上，表示这是一个eureka服务注册中心；
<a name="839242ff"></a>
### `@EnableDiscoveryClient`
用在SpringBoot启动类上，表示这是一个服务，可以被注册中心找到；
<a name="GHdhR"></a>
### `@LoadBalanced`
开启负载均衡能力；
<a name="B6i24"></a>
### `@EnableCircuitBreaker`
用在启动类上，开启断路器功能；
<a name="6lkFe"></a>
### `@HystrixCommand(fallbackMethod="backMethod")`
用在方法上，fallbackMethod指定断路回调方法；
<a name="8sKj9"></a>
### `@EnableConfigServer`
用在启动类上，表示这是一个配置中心，开启Config Server；
<a name="6fad010b"></a>
### `@EnableZuulProxy`
开启zuul路由，用在启动类上；
<a name="6333c525"></a>
### `@SpringCloudApplication`
包含`@SpringBootApplication`和`@EnableDiscovertyClient`
<a name="AwkAK"></a>
### `@EnableCircuitBreaker`
分别是SpringBoot注解、注册服务中心Eureka注解、断路器注解。对于SpringCloud来说，这是每一微服务必须应有的三个注解，所以才推出了`@SpringCloudApplication`这一注解集合。
