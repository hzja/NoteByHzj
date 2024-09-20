JavaSpringBoot
<a name="xtoaZ"></a>
## 1、前言
因为网络传输的不可靠性，以及前端数据控制的可篡改性，后端的参数校验是必须的，应用程序必须通过某种手段来确保输入进来的数据从语义上来讲是正确的。
<a name="viK0A"></a>
## 2、数据校验的痛点
为了保证数据语义的正确，需要进行大量的判断来处理验证逻辑。而且项目的分层也会造成一些重复的校验，产生大量与业务无关的代码。不利于代码的维护，增加了开发人员的工作量。
<a name="wW4HO"></a>
## 3、JSR 303 校验规范及其实现
为了解决上面的痛点，将验证逻辑与相应的领域模型进行绑定是十分有必要的。为此产生了JSR 303 – Bean Validation 规范。Hibernate Validator 是**JSR-303**的参考实现，它提供了**JSR 303**规范中所有的约束（constraint）的实现，同时也增加了一些扩展。
<a name="xr6wn"></a>
### Hibernate Validator 提供的常用约束注解
| **约束注解** | **详细信息** |
| --- | --- |
| `@Null` | 被注释的元素必须为 null |
| `@NotNull` | 被注释的元素必须不为 null |
| `@AssertTrue` | 被注释的元素必须为 true |
| `@AssertFalse` | 被注释的元素必须为 false |
| `@Min(value)` | 被注释的元素必须是一个数字，其值必须大于等于指定的最小值 |
| `@Max(value)` | 被注释的元素必须是一个数字，其值必须小于等于指定的最大值 |
| `@DecimalMin(value)` | 被注释的元素必须是一个数字，其值必须大于等于指定的最小值 |
| `@DecimalMax(value)` | 被注释的元素必须是一个数字，其值必须小于等于指定的最大值 |
| `@Size(max, min)` | 被注释的元素的大小必须在指定的范围内 |
| `@Digits (integer, fraction)` | 被注释的元素必须是一个数字，其值必须在可接受的范围内 |
| `@Past` | 被注释的元素必须是一个过去的日期 |
| `@Future` | 被注释的元素必须是一个将来的日期 |
| `@Pattern(value)` | 被注释的元素必须符合指定的正则表达式 |
| `@Email` | 被注释的元素必须是电子邮箱地址 |
| `@Length` | 被注释的字符串的大小必须在指定的范围内 |
| `@NotEmpty` | 被注释的字符串的必须非空 |
| `@Range` | 被注释的元素必须在合适的范围内 |

<a name="CRJZD"></a>
## 4、验证注解的使用
在**Spring Boot**开发中使用**Hibernate Validator**是非常容易的，引入下面的**starter**就可以了：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-validation</artifactId>
</dependency>
```
一种可以实现接口来定制Validator，一种是使用约束注解。胖哥觉得注解可以满足绝大部分的需求，所以建议使用注解来进行数据校验。而且注解更加灵活，控制的粒度也更加细。接下来学习如何使用注解进行数据校验。
<a name="G5oH2"></a>
### 4.1 约束注解的基本使用
对需要校验的方法入参进行注解约束标记，例子如下：
```java
@Data
public class Student {

    @NotBlank(message = "姓名必须填")
    private String name;
    @NotNull(message = "年龄必须填写")
    @Range(min = 1,max =50, message = "年龄取值范围1-50")
    private Integer age;
    @NotEmpty(message = "成绩必填")
    private List<Double> scores;
}
```
<a name="Isjvd"></a>
#### POST 请求
然后定义一个**POST**请求的**Spring MVC**接口：
```java
@RestController
@RequestMapping("/student")
public class StudentController {

    @PostMapping("/add")
    public Rest<?> addStudent(@Valid @RequestBody Student student) {
        return RestBody.okData(student);
    }
}
```
通过对`addStudent`方法入参添加`@Valid`来启用参数校验。当使用下面数据进行请求将会抛出`MethodArgumentNotValidException`异常，提示age范围超出1-50。
```http
POST /student/add HTTP/1.1
Host: localhost:8888
Content-Type: application/json

{
    "name": "fc",
    "age": 77,
    "scores": [
        55
    ]
}
```
<a name="HNv5C"></a>
#### GET 请求
如法炮制，定义一个**GET**请求的接口：
```java
@GetMapping("/get")
public Rest<?> getStudent(@Valid Student student) {
    return RestBody.okData(student);
}
```
使用下面的请求可以正确对学生分数scores进行了校验，但是抛出的并不是`MethodArgumentNotValidException`异常，而是`BindException`异常。这和使用`@RequestBody`注解有关系，这对后面的统一处理非常十分重要。
```http
GET /student/get?name=fc&age=12 HTTP/1.1
Host: localhost:8888
```
<a name="tbpZ3"></a>
#### 自定义注解
可能有些同学注意到上面的年龄进行了这样的标记：
```java
@NotNull(message = "年龄必须填写")
@Range(min = 1,max =50, message = "年龄取值范围1-50")
private Integer age;
```
这是因为`@Range`不会去校验为空的情况，它只处理非空的时候是否符合范围约束。所以要用多个注解来约束。如果某些场景需要重复的捆绑多个注解来使用时，可以使用自定义注解将它们封装起来组合使用，下面这个注解就是将`@NotNull`和`@Range`进行了组合，可以仿一个出来用用看。
```java
import org.hibernate.validator.constraints.Range;

import javax.validation.Constraint;
import javax.validation.Payload;
import javax.validation.ReportAsSingleViolation;
import javax.validation.constraints.NotNull;
import javax.validation.constraintvalidation.SupportedValidationTarget;
import javax.validation.constraintvalidation.ValidationTarget;
import java.lang.annotation.*;

@Constraint(
        validatedBy = {}
)
@SupportedValidationTarget({ValidationTarget.ANNOTATED_ELEMENT})
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD, ElementType.FIELD,
        ElementType.ANNOTATION_TYPE, ElementType.CONSTRUCTOR,
        ElementType.PARAMETER, ElementType.TYPE_USE})
@NotNull
@Range(min = 1, max = 50)
@Documented
@ReportAsSingleViolation
public @interface Age {
    // message 必须有
    String message() default "年龄必须填写，且范围为 1-50 ";

    // 可选
    Class<?>[] groups() default {};

    // 可选
    Class<? extends Payload>[] payload() default {};
}
```
还有一种情况，在后台定义了枚举值来进行状态的流转，也是需要校验的，比如定义了颜色枚举：
```java
public enum Colors {

    RED, YELLOW, BLUE

}
```
希望入参不能超出Colors的范围["RED", "YELLOW", "BLUE"]，这就需要实现`ConstraintValidator<A extends Annotation, T>`接口来定义一个颜色约束了，其中泛型A为自定义的约束注解，泛型T为入参的类型，这里使用字符串，然后实现如下：
```java
public class ColorConstraintValidator implements ConstraintValidator<Color, String> {
    private static final Set<String> COLOR_CONSTRAINTS = new HashSet<>();

    @Override
    public void initialize(Color constraintAnnotation) {
        Colors[] value = constraintAnnotation.value();
        List<String> list = Arrays.stream(value)
            .map(Enum::name)
            .collect(Collectors.toList());
        COLOR_CONSTRAINTS.addAll(list);

    }

    @Override
    public boolean isValid(String value, ConstraintValidatorContext context) {
        return COLOR_CONSTRAINTS.contains(value);
    }
}
```
然后声明对应的约束注解Color，需要在元注解`@Constraint`中指明使用上面定义好的处理类`ColorConstraintValidator`进行校验。
```java
@Constraint(validatedBy = ColorConstraintValidator.class)
@Documented
@Target({ElementType.METHOD, ElementType.FIELD,
        ElementType.ANNOTATION_TYPE, ElementType.CONSTRUCTOR,
        ElementType.PARAMETER, ElementType.TYPE_USE})
@Retention(RetentionPolicy.RUNTIME)
public @interface Color {
    // 错误提示信息
    String message() default "颜色不符合规格";

    Class<?>[] groups() default {};

    Class<? extends Payload>[] payload() default {};

    // 约束的类型
    Colors[] value();
}
```
然后来试一下，先对参数进行约束：
```java
@Data
public class Param {
    @Color({Colors.BLUE,Colors.YELLOW})
   private String color;
}
```
接口跟上面几个一样，调用下面的接口将抛出BindException异常：
```http
GET /student/color?color=CAY HTTP/1.1
Host: localhost:8888
```
当把参数color赋值为BLUE或者YELLOW后，能够成功得到响应。
<a name="Svj4y"></a>
### 4.2 常见问题
在实际使用起来会遇到一些问题，这里总结了一些常见的问题和处理方式。
<a name="XZm2s"></a>
#### 检验基础类型不生效的问题
上面为了校验颜色声明了一个Param对象来包装唯一的字符串参数color，为什么直接使用下面的方式定义呢？
```java
@GetMapping("/color")
public Rest<?> color(@Valid @Color({Colors.BLUE,Colors.YELLOW}) String color) {
    return RestBody.okData(color);
}
```
或者使用路径变量：
```java
@GetMapping("/rest/{color}")
public Rest<?> rest(@Valid @Color({Colors.BLUE, Colors.YELLOW}) @PathVariable String color) {
    return RestBody.okData(color);
}
```
**上面两种方式是不会生效的**。不信可以试一试，起码在**Spring Boot 2.3.1.RELEASE**是不会直接生效的。<br />使以上两种生效的方法是在类上添加`@Validated`注解。**注意一定要添加到方法所在的类上才行**。这时候会抛出`ConstraintViolationException`异常。
<a name="q3tGB"></a>
#### 集合类型参数中的元素不生效的问题
就像下面的写法，方法的参数为集合时，如何检验元素的约束呢？
```java
/**
 * 集合类型参数元素.
 *
 * @param student the student
 * @return the rest
 */
@PostMapping("/batchadd")
public Rest<?> batchAddStudent(@Valid @RequestBody List<Student> student) {
    return RestBody.okData(student);
}
```
同样是在类上添加`@Validated`注解。**注意一定要添加到方法所在的类上才行**。这时候会抛出`ConstraintViolationException`异常。
<a name="WpsJi"></a>
#### 嵌套校验不生效
嵌套的结构如何校验呢？打个比方，如果在学生类Student中添加了其所属的学校信息School并希望对School的属性进行校验。
```java
@Data
public class Student {

    @NotBlank(message = "姓名必须填")
    private String name;
    @Age
    private Integer age;
    @NotEmpty(message = "成绩必填")
    private List<Double> scores;
    @NotNull(message = "学校不能为空")
    private School school;
}


@Data
public class School {
    @NotBlank(message = "学校名称不能为空")
    private String name;
    @Min(value = 0,message ="校龄大于0" )
    private Integer age;
}
```
当 **GET**请求时正常校验了School的属性，但是**POST**请求却无法对School的属性进行校验。这时只需要在该属性上加上`@Valid`注解即可。
```java
@Data
public class Student {

    @NotBlank(message = "姓名必须填")
    private String name;
    @Age
    private Integer age;
    @NotEmpty(message = "成绩必填")
    private List<Double> scores;
    @Valid
    @NotNull(message = "学校不能为空")
    private School school;
}
```
每加一层嵌套都需要加一层`@Valid`注解。通常在校验对象属性时，`@NotNull`、`@NotEmpty`和`@Valid`配合才能起到校验效果。
<a name="edpWx"></a>
## 5、总结
通过校验框架可以专心于业务开发，本文对**Hibernate Validator**的使用和一些常见问题进行了梳理。可以通过Spring Boot 统一异常处理来解决参数校验的异常信息的提示问题。
