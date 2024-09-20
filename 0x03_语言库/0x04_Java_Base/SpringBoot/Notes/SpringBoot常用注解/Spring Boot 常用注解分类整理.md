Java SpringBoot
<a name="hMXQe"></a>
### 1、目录
![2021-09-25-17-07-48-353861.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632560953597-961b4575-2db2-49a2-9a34-c0592ff9501c.png#clientId=u44afa1d0-809e-4&from=ui&id=u99b57de0&originHeight=1375&originWidth=1080&originalType=binary&ratio=1&size=4463662&status=done&style=none&taskId=u412fb380-d97e-46a2-a334-46ba115832a)<br />Spring Boot 常用注解
<a name="rncKC"></a>
### 2、Bean处理
下面这些注解都是处理 Spring 中 Bean 相关的。
<a name="I9Ic5"></a>
#### `@Resource`
依赖注入，自动导入标注的对象到当前类中，比如 Controller 类通常要导入 Service 类（需要注意这里 Service 类要被 Spring 容器管理）。
<a name="IYKZo"></a>
#### `@Component`、`@Repository`、`@Service`、`@Controller`、`@RestController`
一般使用 `@Resource` 注解让 Spring 容器自动装配 bean。要想把类标识成可用于 `@Resource` 注解自动装配的 bean 就需要使用这几个注解了，它们表示对应的类将被 Spring 容器管理。

- `@Component` ：通用的注解，可标注任意类为 Spring 组件。如果一个 Bean 不知道属于哪个层，可以使用`@Component` 注解标注；
- `@Repository` : 对应持久层即 Dao 层，主要用于数据库相关操作；
- `@Service` : 对应服务层，主要涉及一些复杂的逻辑，需要用到 Dao 层；
- `@Controller` : 对应 Spring MVC 控制层，一般需要注入 Service 类返回结果数据；
- `@RestController` : 继承于 `@Controller`，区别在于标注后整个类所有方法将直接返回 JSON 数据，不再需要视图解析处理，目前前后端分离的项目后端都是直接用这个注解的；
<a name="GwRkR"></a>
#### `@Configuration`
标注是 Java 代码的配置类， Spring Boot 中推荐这种做法不再使用 xml 配置了；
```java
@Configuration
public class AppConfig {
   //相当于 xml 中配置 Bean
    @Bean 
    public TransferService transferService() {
        return new TransferServiceImpl();
    }

}
```
<a name="DeCmL"></a>
#### `@Scope`
声明 Spring Bean 的作用域，作用于一共有以下几种：

- `singleton`：唯一 bean 实例，Spring 中的 bean 默认都是单例的。
- `prototype`：每次请求都会创建一个新的 bean 实例。
- `request`：每一次 HTTP 请求都会产生一个新的 bean，该 bean 仅在当前 HTTP request 内有效。
- `session`：每一次 HTTP 请求都会产生一个新的 bean，该 bean 仅在当前 HTTP session 内有效。
<a name="MfNzO"></a>
### 3、HTTP请求
下面这些注解对应 Http 常见的请求方式，用于标注接口路径请求。
<a name="D3qBR"></a>
#### `@GetMapping`
等价于 `@RequestMapping(value="/test",method=RequestMethod.GET)`
<a name="M4c4Y"></a>
#### `@PostMapping`
```java
@PostMapping("/users")
public ResponseEntity<User> createUser(@RequestBody @Valid User user) {
    userService.save(user);
    return new ResponseEntity(HttpStatus.OK);
}
```
<a name="kYkeP"></a>
#### `@PutMapping`
put 方式的请求，常用语更新服务器上的资源。举个例子：PUT /users/7（更新编号为 7 的学生）
<a name="AEn5W"></a>
#### `@DeleteMapping`
DELETE 请求，从服务器删除特定的资源。
<a name="LOETs"></a>
### 4、前后端参数传递
<a name="Mjzov"></a>
#### `@RequestParam` & `@PathVariable`
`@RequestParam` 用在方法的参数前面，获取请求中表单类型的key=value格式的数据。<br />`@PathVariable` 用于获取请求路径中的参数。
```java
@PostMapping("/{id}")
public ResponseEntity<List<User>> getUser(@PathVariable long id,@RequestParam String macAddress) {
    return new ResponseEntity(userService.findById(id),HttpStatus.OK);
}
```
<a name="W1v5x"></a>
#### `@RequestBody`
获取请求 body 中的数据，常用于搭配 `@PostMapping` 请求来提交对象数据.  请求体 的Content-Type 必须为 application/json 格式的数据，接收到数据之后会自动将数据绑定到 Java 对象上去。<br />系统会使用 `HttpMessageConverter` 或者自定义的 `HttpMessageConverter`将请求的 body 中的 json 字符串转换为 java 对象。
```java
@PostMapping("/users")
public ResponseEntity<User> createUser(@RequestBody @Valid User user) {
    userService.save(user);
    return new ResponseEntity(HttpStatus.OK);
}
```
强调一下：请求方法只可以有一个`@RequestBody`，但是可以有多个 `@RequestParam` 和 `@PathVariable`。 如果方法必须要用两个 `@RequestBody` 来接受数据的话，那就只思考下重新设计了，或者套娃形式的。
<a name="xJ17d"></a>
#### `@ResponseBody`
表示该方法的返回结果直接写入 HTTP response body 中，格式为 json。上面提到的 `@RestController` 其实就是 `@Controller` 和 `@ResponseBody` 两个结合起来的。
<a name="tP6cf"></a>
### 5、读取配置
<a name="Thlkk"></a>
#### `@value`
可以在任意 Spring 管理的 Bean 中通过这个注解获取任何来源配置的属性值。比如application.properties 或者 application.yml 配置文件中配置的属性值，当然也可以是 JVM 属性配置或者系统环境变量配置的值。
```java
@Value("${who}")
String name;
```
<a name="Qxt67"></a>
#### `@ConfigurationProperties`(重点)
上面 `@Value` 在每个类中获取属性配置值的做法其实是不推荐的，一般在企业项目开发中，不会使用那么杂乱无章的写法而且维护也麻烦，通过就是一次性读取到一个 Java 配置类，然后需要使用的地方直接引用这个类就可以多次访问了，方便维护。
```java
@Component
@ConfigurationProperties(prefix = "my", ignoreUnknownFields = false)
@Data
public class MyProperties implements Serializable {

    private String name;

    private String username;

    private List<Book> books;

    @Data
    static class Book implements Serializable{
        private int id;
        private String bookName;
    }

}
```
<a name="sdDPD"></a>
#### `@PropertySource`
这个注解是用来指定读取自定义的配置文件的。
```java
@Component
@ConfigurationProperties(prefix= "my" )
@PropertySource(value = {"classpath:my.properties"})
@Data
public class MyProperties {
    private int maxValue= 0;
}
```
<a name="QPx61"></a>
### 6、参数校验
数据的校验的重要性就不用说了，即使在前端对数据进行校验的情况下，还是要对传入后端的数据再进行一遍校验，避免用户绕过浏览器直接通过一些 HTTP 工具直接向后端请求一些违法数据。<br />JSR(Java Specification Requests） 是一套 JavaBean 参数校验的标准，它定义了很多常用的校验注解，可以直接将这些注解加在 JavaBean 的属性上面，这样就可以在需要校验的时候进行校验了，非常方便！<br />校验的时候实际用的是 Hibernate Validator 框架。Hibernate Validator 是 Hibernate 团队最初的数据校验框架，Hibernate Validator 4.x 是 Bean Validation 1.0（JSR 303）的参考实现，Hibernate Validator 5.x 是 Bean Validation 1.1（JSR 349）的参考实现，目前最新版的 Hibernate Validator 6.x 是 Bean Validation 2.0（JSR 380）的参考实现。<br />需要注意的是从 Spring Boot 2.3开始，还需要显式地添加 spring-boot-starter-validation 依赖项，在此之前的 Spring Boot 版本是在 spring-boot-starter-web 依赖项中包含的不需要额外引入。<br />现在使用的是 Spring Boot 2.5.2 版本了，所以手动增加依赖：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-validation</artifactId>
    <version>2.5.2</version>
</dependency>
```
👉 需要注意的是： 所有的注解，推荐使用 JSR 注解，即 `javax.validation.constraints`，而不是 `org.hibernate.validator.constraints`
<a name="G7bG3"></a>
#### Bean字段验证注解

- `@NotEmpty` 被注释的字符串的不能为 null 也不能为空
- `@NotBlank` 被注释的字符串非 null，并且必须包含一个非空白字符
- `@Null` 被注释的元素必须为 null
- `@NotNull` 被注释的元素必须不为 null
- `@AssertTrue` 被注释的元素必须为 true
- `@AssertFalse` 被注释的元素必须为 false
- `@Pattern(regex=,flag=)`被注释的元素必须符合指定的正则表达式
- `@Email` 被注释的元素必须是 Email 格式。
- `@Min(value)`被注释的元素必须是一个数字，其值必须大于等于指定的最小值
- `@Max(value)`被注释的元素必须是一个数字，其值必须小于等于指定的最大值
- `@DecimalMin(value)`被注释的元素必须是一个数字，其值必须大于等于指定的最小值
- `@DecimalMax(value)` 被注释的元素必须是一个数字，其值必须小于等于指定的最大值
- `@Size(max=, min=)`被注释的元素的大小必须在指定的范围内
- `@Digits (integer, fraction)`被注释的元素必须是一个数字，其值必须在可接受的范围内
- `@Past`被注释的元素必须是一个过去的日期
- `@Future` 被注释的元素必须是一个将来的日期
<a name="yZXhJ"></a>
#### 验证请求体(`RequestBody`)
```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class User {

    @Min(100)
    private Long id;

    @NotNull(message = "name 不能为空")
    @NotEmpty
    @JsonProperty(value = "name")
    private String name;

}
```
只需要在请求处理方法中需要验证的参数前加上 `@Valid` 注解就会开启校验了，如果验证失败将抛出异常：`MethodArgumentNotValidException`。
```java
@PostMapping("/users")
public ResponseEntity<User> createUser(@RequestBody @Valid User user) {
    userService.save(user);
    return new ResponseEntity(HttpStatus.OK);
}
```
<a name="SFGrj"></a>
#### `@Validated`
如果入参不是用一个 Java 对象来接收的话，比如用 `@PathVariables` 和 `@RequestParam` 注解来获取入参，这种情况下要校验参数不要忘记在类的头上加 `@Validated` 注解，这个参数可以告诉 Spring 去校验方法参数。
```java
@RestController
@RequestMapping("/user")
@Validated
public class UserController {
   @GetMapping("/{id}")
    public ResponseEntity<List<User>> findById( @PathVariable @Max(value = 5,message = "超过 id 的范围了") long id) {
        return new ResponseEntity(userService.findById(id),HttpStatus.OK);
    }

}
```
<a name="aKo9o"></a>
### 7、统一异常处理
异常处理通常也是在开发中一定要做的，因为不可能将一些程序员才能看懂的异常抛出到前端去（总会有不靠谱的同事忘记捕获异常的），所以一般都会加上统一的异常处理，在对用户不友好的错误堆栈转换为普通用户可以看懂的文案提示。<br />这里涉及到的注解只有两个：

1. `@ControllerAdvice` : 定义全局异常处理类，包含 `@Component` 所以可以被 Spring 扫描到。
2. `@ExceptionHandler` : 声明异常处理方法，表示遇到这个异常，就执行标注的方法。
```java
@ControllerAdvice
@ResponseBody
class GlobalDefaultExceptionHandler {
    // 如果是遇到注解中指定的异常就会进这个方法处理后返回
    @ExceptionHandler({MethodArgumentNotValidException.class, ConstraintViolationException.class})
    public Map argumentErrorHandler(HttpServletRequest req, Exception e) throws Exception {
        Map map = new HashMap();
        map.put("error", "参数不合法");
        map.put("param", req.getParameterMap());
        return map;
    }
}
```
<a name="Q98hp"></a>
### 8、JPA数据持久化
JPA是Java Persistence API的简称，中文名Java持久层API，是JDK 5.O注解或XML描述对象―关系表的映射关系，并将运行期的实体对象持久化到数据库中。<br />Sun引入新的JPA ORM规范出于两个原因：其一，简化现有Java EE和Java SE应用开发工作；其二，Sun希望整合ORM技术，实现天下归一。<br />只能说在实际项目开发中还是用的不多，大家还是喜欢用 Mybatis，不过相比更加简洁的技术还是要学习下的。
<a name="mhnuG"></a>
#### 创建表

- `@Entity`
- `@Table`

这俩是放在 Java Bean 上面用来标注数据库实体对象和表明之间的映射关系的。
```java
@Data
@Entity
@Table("user")
public class User {
    @Id
    @GeneratedValue
    @JsonProperty(value = "id")
    private Long id;
}
```
<a name="Ulym9"></a>
#### 创建主键

- `@Id`
- `@GeneratedValue`

这俩主键放在 Bean 的字段上，用来标识数据库的主键以及主键自动生成的。通过 `@GeneratedValue`直接使用 JPA 内置提供的四种主键生成策略来指定。

- `GenerationType.TABLE` 持久化引擎通过关系数据库的一张特定的表格来生成主键；
- `GenerationType.SEQUENCE` 随机序列；
- `GenerationType.IDENTITY` 主键自增长；
- `GenerationType.AUTO` 持久化引擎会根据数据库在以上三种主键生成策略中选择，默认选择的就是这个策略；
```java
@Id
@GeneratedValue(strategy = GenerationType.IDENTITY)
private Long id;
```
<a name="X8d9C"></a>
#### 设置字段类型
`@Column` 声明字段。<br />比如：Java Bean 属性名为 nickName 对应的数据库字段名为 user_name，长度为 32，非空。
```java
@Column(name = "user_name", nullable = false, length=32)
private String nickName;
```
设置字段类型并且加默认值，这个还是挺常用的。
```java
Column(columnDefinition = "tinyint(1) default 1")
private Boolean enabled;
```
<a name="XdKnv"></a>
#### 指定不持久化特定字段
`@Transient` ：有一些字段有时间不需要落库，就可以加上这个注解就好了。
<a name="PdKsp"></a>
#### `@Transactional`声明事务
这个注解可以作用于类上，也可以作用于方法上。前者表示所有该类的 `public` 方法都配置相同的事务属性信息，后者表示当类配置了`@Transactional`，方法也配置了`@Transactional`，方法的事务会覆盖类的事务配置信息。<br />Exception 分为运行时异常 `RuntimeException` 和非运行时异常。在 `@Transactional` 注解中如果不配置 `rollbackFor` 属性，那么事物只会在遇到 `RuntimeException` 的时候才会回滚，加上 `rollbackFor=Exception.class `，可以让事物在遇到非运行时异常时也回滚。
```java
@Transactional(rollbackFor = Exception.class)
public void save() {
  ......
}
```
<a name="Ob0yj"></a>
### 9、JSON格式处理
<a name="PVFxu"></a>
#### 指定过滤字段

- `@JsonIgnoreProperties` 作用在类上用于过滤掉特定字段不返回或者不解析。
- `@JsonIgnore`一般用于类的属性上，作用和上面的`@JsonIgnoreProperties` 一样。
```java
//生成json时将userRoles属性过滤
@JsonIgnoreProperties({"password"})
public class User {
    private String userName;
    private String fullName;
    @JsonIgnore
    private String password;
}
```
<a name="bL5lO"></a>
#### 格式化 json 数据
```java
@JsonFormat(pattern="yyyy-MM-dd HH:mm:ss",timezone="GMT+8")
private Date createDate;
```
要注意的是，它只会在类似 `@ResponseBody` 返回 json 数据的时候，才会返回格式化的 `yyyy-MM-dd HH:mm:ss` 时间，直接使用`System.out.println()`输出的话，仍然是类似 `Fri Dec 01 21:05:20 CST 2017` 这样的时间样式。
<a name="lwK8i"></a>
#### 扁平化对象
```java
@JsonUnwrapped
```
这个可能不太好说，大家直接看代码就懂了：
```java
@Getter
@Setter
@ToString
public class Account {
    @JsonUnwrapped
    private Location location;
    @JsonUnwrapped
    private PersonInfo personInfo;

    @Getter
    @Setter
    @ToString
    public static class Location {
        private String provinceName;
        private String countyName;
    }
    @Getter
    @Setter
    @ToString
    public static class PersonInfo {
        private String userName;
        private String fullName;
    }
}
```
未扁平化之前：
```json
{
    "location": {
        "provinceName":"上海",
        "countyName":"上海"
    },
    "personInfo": {
        "userName": "Fcant",
        "fullName": "Fcc"
    }
}
```
使用 `@JsonUnwrapped` 扁平对象之后：
```java
@Getter
@Setter
@ToString
public class Account {
    @JsonUnwrapped
    private Location location;
    @JsonUnwrapped
    private PersonInfo personInfo;
    ......
}
```
```json
{
  "provinceName":"上海",
  "countyName":"上海",
  "userName": "Fcant",
  "fullName": "Fcc"
}
```
<a name="Kz44Y"></a>
### 10、测试处理
`@ActiveProfiles`一般作用于测试类上， 用于声明生效的 Spring 配置文件。<br />`@Test`声明一个方法为测试方法。<br />`@Transactional`被声明的测试方法执行后数据会被回滚，避免污染测试数据。<br />`@WithMockUser` Spring Security 提供的，用来模拟一个真实用户，并且可以赋予权限。
<a name="uQs0A"></a>
### 11、配置启动
`@SpringBootApplication` : 等价于使用 `@Configuration`、`@EnableAutoConfiguration`、`@ComponentScan`  三个注解。<br />`@Configuration`：声明是是一个 Java 形式的配置类，Spring Boot 提倡基于 Java 的配置，相当于之前在 xml 中配置 bean；<br />`@EnableAutoConfiguration`：类级别的注解，这个注解告诉 Spring Boot 根据添加的 jar 依赖猜测想如何配置 Spring，也就是 Spring 实现自动配置的开关。<br />`@ComponentScan`：标注哪些路径下的类需要被Spring扫描。<br />`@Conditional`：Spring4 新提供的注解，通过 `@Conditional` 注解可以根据代码中设置的条件装载不同的 bean，也是SpringBoot实现自动配置的基石。<br />Spring Boot 源码中大量扩展了 `@Conditional` 注解，用于实现智能的自动化配置，满足各种使用场景。下面列举下：

- `@ConditionalOnBean`：配置了某个特定的Bean时条件为真。
- `@ConditionalOnMissingBean`：没有配置特定的Bean时条件为真。
- `@ConditionalOnClass`：Classpath里有指定的类时条件为真。
- `@ConditionalOnMissingClass` ：Classpath里没有指定的类时条件为真。
- `@ConditionalOnExpression`：给定的SpEL表达式计算结果为true。
- `@ConditionalOnJava`：Java的版本匹配特定值或者一个范围值。
- `@ConditionalOnJndi`：参数中给定的 JNDI 位置必须存在一个，如果没有给参数，则要有 JNDI InitialContext。
- `@ConditionalOnProperty`：指定的配置属性要有一个明确的值。
- `@ConditionalOnResource`：Classpath里没有指定的资源。
- `@ConditionalOnWebApplication`：这是一个Web应用程序。
- `@ConditionalOnNotWebApplication`：这不是一个Web应用程序。
