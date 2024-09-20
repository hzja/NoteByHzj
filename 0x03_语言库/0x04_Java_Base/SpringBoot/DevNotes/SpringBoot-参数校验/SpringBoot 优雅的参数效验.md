Java SpringBoot 参数校验<br />Spring框架已经封装了一套校验组件：validation。其特点是简单易用，自由度高。接下来使用springboot-2.3.1.RELEASE搭建一个简单的 Web 工程，在开发过程中如何优雅地做参数校验。
<a name="kgkQ8"></a>
## 1、环境搭建
从springboot-2.3开始，校验包被独立成了一个starter组件，所以需要引入如下依赖：
```xml
<!--校验组件-->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-validation</artifactId>
</dependency>
<!--web组件-->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
而springboot-2.3之前的版本只需要引入 web 依赖就可以了。
<a name="Gv2eP"></a>
## 2、小试牛刀
参数校验非常简单，首先在待校验字段上增加校验规则注解
```java
public class UserVO {
    @NotNull(message = "age 不能为空")
    private Integer age;
}
```
然后在`controller`方法中添加`@Validated`和用于接收错误信息的`BindingResult`就可以了，于是有了第一版：
```java
public String add1(@Validated UserVO userVO, BindingResult result) {
    List<FieldError> fieldErrors = result.getFieldErrors();
    if(!fieldErrors.isEmpty()){
        return fieldErrors.get(0).getDefaultMessage();
    }
    return "OK";
}
```
通过工具(postman)去请求接口，如果参数不符合规则，会将相应的 message信息返回：
```xml
age 不能为空
```
内置的校验注解有很多，罗列如下：

| 注解 | 校验功能 |
| --- | --- |
| `@AssertFalse` | 必须是false |
| `@AssertTrue` | 必须是true |
| `@DecimalMax` | 小于等于给定的值 |
| `@DecimalMin` | 大于等于给定的值 |
| `@Digits` | 可设定最大整数位数和最大小数位数 |
| `@Email` | 校验是否符合Email格式 |
| `@Future` | 必须是将来的时间 |
| `@FutureOrPresent` | 当前或将来时间 |
| `@Max` | 最大值 |
| `@Min` | 最小值 |
| `@Negative` | 负数（不包括0） |
| `@NegativeOrZero` | 负数或0 |
| `@NotBlank` | 不为null并且包含至少一个非空白字符 |
| `@NotEmpty` | 不为null并且不为空 |
| `@NotNull` | 不为null |
| `@Null` | 为null |
| `@Past` | 必须是过去的时间 |
| `@PastOrPresent` | 必须是过去的时间，包含现在 |
| `@PositiveOrZero` | 正数或0 |
| `@Size` | 校验容器的元素个数 |

<a name="gIMgk"></a>
## 3、规范返回值
待校验参数多了之后我们希望一次返回所有校验失败信息，方便接口调用方进行调整，这就需要统一返回格式，常见的就是封装一个结果类。
```java
public class ResultInfo<T>{
    private Integer status;
    private String message;
    private T response;
    // 省略其他代码...
}
```
改造一下controller 方法，第二版：
```java
public ResultInfo add2(@Validated UserVO userVO, BindingResult result) {
    List<FieldError> fieldErrors = result.getFieldErrors();
    List<String> collect = fieldErrors.stream()
            .map(o -> o.getDefaultMessage())
            .collect(Collectors.toList());
    return new ResultInfo<>().success(400,"请求参数错误",collect);
}
```
请求该方法时，所有的错误参数就都返回了：
```json
{
    "status": 400,
    "message": "请求参数错误",
    "response": [
        "年龄必须在[1,120]之间",
        "bg 字段的整数位最多为3位，小数位最多为1位",
        "name 不能为空",
        "email 格式错误"
    ]
}
```
<a name="maffg"></a>
## 4、全局异常处理
每个`Controller`方法中如果都写一遍`BindingResult`信息的处理，使用起来还是很繁琐。可以通过全局异常处理的方式统一处理校验异常。<br />当写了`@validated`注解，不写`BindingResult`的时候，Spring 就会抛出异常。由此，可以写一个全局异常处理类来统一处理这种校验异常，从而免去重复组织异常信息的代码。<br />全局异常处理类只需要在类上标注`@RestControllerAdvice`，并在处理相应异常的方法上使用`@ExceptionHandler`注解，写明处理哪个异常即可。
```java
@RestControllerAdvice
public class GlobalControllerAdvice {
    private static final String BAD_REQUEST_MSG = "客户端请求参数错误";
    // <1> 处理 form data方式调用接口校验失败抛出的异常 
    @ExceptionHandler(BindException.class)
    public ResultInfo bindExceptionHandler(BindException e) {
        List<FieldError> fieldErrors = e.getBindingResult().getFieldErrors();
        List<String> collect = fieldErrors.stream()
                .map(o -> o.getDefaultMessage())
                .collect(Collectors.toList());
        return new ResultInfo().success(HttpStatus.BAD_REQUEST.value(), BAD_REQUEST_MSG, collect);
    }
    // <2> 处理 json 请求体调用接口校验失败抛出的异常 
    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResultInfo methodArgumentNotValidExceptionHandler(MethodArgumentNotValidException e) {
        List<FieldError> fieldErrors = e.getBindingResult().getFieldErrors();
        List<String> collect = fieldErrors.stream()
                .map(o -> o.getDefaultMessage())
                .collect(Collectors.toList());
        return new ResultInfo().success(HttpStatus.BAD_REQUEST.value(), BAD_REQUEST_MSG, collect);
    }
    // <3> 处理单个参数校验失败抛出的异常
    @ExceptionHandler(ConstraintViolationException.class)
    public ResultInfo constraintViolationExceptionHandler(ConstraintViolationException e) {
        Set<ConstraintViolation<?>> constraintViolations = e.getConstraintViolations();
        List<String> collect = constraintViolations.stream()
                .map(o -> o.getMessage())
                .collect(Collectors.toList());
        return new ResultInfo().success(HttpStatus.BAD_REQUEST.value(), BAD_REQUEST_MSG, collect);
    }
    

}
```
事实上，在全局异常处理类中，我们可以写多个异常处理方法，课代表总结了三种参数校验时可能引发的异常：

1. 使用form data方式调用接口，校验异常抛出 `BindException`
2. 使用 json 请求体调用接口，校验异常抛出 `MethodArgumentNotValidException`
3. 单个参数校验异常抛出`ConstraintViolationException`

注：单个参数校验需要在参数上增加校验注解，并在类上标注`@Validated`。<br />全局异常处理类可以添加各种需要处理的异常，比如添加一个对`Exception.class`的异常处理，当所有`ExceptionHandler`都无法处理时，由其记录异常信息，并返回友好提示。
<a name="z5SMc"></a>
## 5、分组校验
如果同一个参数，需要在不同场景下应用不同的校验规则，就需要用到分组校验了。比如：新注册用户还没起名字，允许name字段为空，但是不允许将名字更新为空字符。<br />分组校验有三个步骤：

1. 定义一个分组类（或接口）
2. 在校验注解上添加`groups`属性指定分组
3. `Controller`方法的`@Validated`注解添加分组类
```java
public interface Update extends Default{
}

public class UserVO {
    @NotBlank(message = "name 不能为空",groups = Update.class)
    private String name;
    // 省略其他代码...
}

@PostMapping("update")
public ResultInfo update(@Validated({Update.class}) UserVO userVO) {
    return new ResultInfo().success(userVO);
}
```
细心的同学可能已经注意到，自定义的`Update`分组接口继承了`Default`接口。校验注解(如：`@NotBlank`)和`@validated`默认都属于`Default.class`分组，这一点在`javax.validation.groups.Default`注释中有说明
```java
/**
 * Default Jakarta Bean Validation group.
 * <p>
 * Unless a list of groups is explicitly defined:
 * <ul>
 *     <li>constraints belong to the {@code Default} group</li>
 *     <li>validation applies to the {@code Default} group</li>
 * </ul>
 * Most structural constraints should belong to the default group.
 *
 * @author Emmanuel Bernard
 */
public interface Default {
}
```
在编写`Update`分组接口时，如果继承了`Default`，下面两个写法就是等效的：
```java
@Validated({Update.class})
@Validated({Update.class,Default.class})
```
请求一下`/update`接口可以看到，不仅校验了name字段，也校验了其他默认属于`Default.class`分组的字段
```json
{
    "status": 400,
    "message": "客户端请求参数错误",
    "response": [
        "name 不能为空",
        "age 不能为空",
        "email 不能为空"
    ]
}
```
如果`Update`不继承`Default`，`@Validated({Update.class})`就只会校验属于`Update.class`分组的参数字段，修改后再次请求该接口得到如下结果，可以看到， 其他字段没有参与校验：
```json
{
    "status": 400,
    "message": "客户端请求参数错误",
    "response": [
        "name 不能为空"
    ]
}
```
<a name="xXPIN"></a>
## 6、递归校验
如果 UserVO 类中增加一个 OrderVO 类的属性，而 OrderVO 中的属性也需要校验，就用到递归校验了，只要在相应属性上增加`@Valid`注解即可实现（对于集合同样适用）<br />OrderVO类如下
```java
public class OrderVO {
    @NotNull
    private Long id;
    @NotBlank(message = "itemName 不能为空")
    private String itemName;
    // 省略其他代码...
}
```
在 UserVO 类中增加一个 OrderVO 类型的属性
```java
public class UserVO {
    @NotBlank(message = "name 不能为空",groups = Update.class)
    private String name;
    //需要递归校验的OrderVO
    @Valid
    private OrderVO orderVO;
    // 省略其他代码...
}
```
调用请求验证如下：<br />![2021-09-21-23-03-21-923605.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632236616759-27e563b0-a236-41fc-b00b-82f12fb5954a.png#clientId=u3d2b6e1f-a843-4&from=ui&id=u8457fc8c&originHeight=522&originWidth=600&originalType=binary&ratio=1&size=32522&status=done&style=shadow&taskId=uf0657ddb-ae33-4faa-945b-83d4c114436)
<a name="WkUgT"></a>
## 7、自定义校验
Spring 的 validation 提供了这么多特性，几乎可以满足日常开发中绝大多数参数校验场景了。但是，一个好的框架一定是方便扩展的。有了扩展能力，就能应对更多复杂的业务场景，毕竟在开发过程中，唯一不变的就是变化本身。<br />Spring Validation允许用户自定义校验，实现很简单，分两步：

1. 自定义校验注解
2. 编写校验者类

代码也很简单，结合注释一看就能懂
```java
@Target({METHOD, FIELD, ANNOTATION_TYPE, CONSTRUCTOR, PARAMETER})
@Retention(RUNTIME)
@Documented
@Constraint(validatedBy = {HaveNoBlankValidator.class})// 标明由哪个类执行校验逻辑
public @interface HaveNoBlank {
 
    // 校验出错时默认返回的消息
    String message() default "字符串中不能含有空格";

    Class<?>[] groups() default { };

    Class<? extends Payload>[] payload() default { };

    /**
     * 同一个元素上指定多个该注解时使用
     */
    @Target({ METHOD, FIELD, ANNOTATION_TYPE, CONSTRUCTOR, PARAMETER, TYPE_USE })
    @Retention(RUNTIME)
    @Documented
    public @interface List {
        NotBlank[] value();
    }
}

public class HaveNoBlankValidator implements ConstraintValidator<HaveNoBlank, String> {
    @Override
    public boolean isValid(String value, ConstraintValidatorContext context) {
        // null 不做检验
        if (value == null) {
            return true;
        }
        if (value.contains(" ")) {
            // 校验失败
            return false;
        }
        // 校验成功
        return true;
    }
}
```
自定义校验注解使用起来和内置注解无异，在需要的字段上添加相应注解即可，同学们可以自行验证
<a name="GUUjO"></a>
## 回顾
以上就是如何使用 Spring Validation 优雅地校验参数的全部内容，下面重点总结一下文中提到的校验特性

1. 内置多种常用校验注解
2. 支持单个参数校验
3. 结合全局异常处理自动组装校验异常
4. 分组校验
5. 支持递归校验
6. 自定义校验
