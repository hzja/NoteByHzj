<a name="oDH3K"></a>
## 背景
Spring 框架，广泛应用于 JAVA 企业级开发中，包含了一套实用的字段校验机制： Spring Validation。这个机制融合了 JSR380 规范，即 Bean Validation 2.0。本文将介绍 Spring Validation 的使用方法，包括基础注解的应用以及进阶使用技巧。
<a name="BuSMP"></a>
## 常用注解
<a name="to0WJ"></a>
### Bean Validation 2.0 注解
<a name="Q8SQH"></a>
#### 校验空值

- `@Null`：验证对象是否为 null
- `@NotNull`：验证对象是否不为 null
- `@NotEmpty`：验证对象不为 null，且长度（数组、集合、字符串等）大于 0
- `@NotBlank`：验证字符串不为 null，且去除两端空白字符后长度大于 0
<a name="W9UP4"></a>
#### 校验大小

- `@Size(min=, max=)`：验证对象（数组、集合、字符串等）长度是否在给定的范围之内
- `@Min(value)`：验证数值（整数或浮点数）是否大于等于指定的最小值
- `@Max(value)`：验证数值是否小于等于指定的最大值
<a name="ZI47X"></a>
#### 校验布尔值

- `@AssertTrue`：验证 Boolean 对象是否为 true
- `@AssertFalse`：验证 Boolean 对象是否为 false
<a name="jJCU5"></a>
#### 校验日期和时间

- `@Past`：验证 Date 和 Calendar 对象是否在当前时间之前
- `@Future`：验证 Date 和 Calendar 对象是否在当前时间之后
- `@PastOrPresent`：验证日期是否是过去或现在的时间
- `@FutureOrPresent`：验证日期是否是现在或将来的时间
<a name="feujT"></a>
#### 正则表达式

- `@Pattern(regexp=, flags=)`：验证 String 对象是否符合正则表达式的规则
<a name="Dyk76"></a>
### Hibernate Validation 拓展

- `@Length(min=, max=)`：验证字符串的大小是否在指定的范围内
- `@Range(min=, max=)`：验证数值是否在合适的范围内
- `@UniqueElements`：校验集合中的值是否唯一，依赖于 equals 方法
- `@ScriptAssert`：利用脚本进行校验
<a name="TbjYE"></a>
### `@Valid` 和 `@Validated`
这两个注解是校验的入口，作用相似但用法上存在差异。 
<a name="AcWRn"></a>
#### `@Validated`
```java
// 用于类/接口/枚举，方法以及参数
@Target({ElementType.TYPE, ElementType.METHOD, ElementType.PARAMETER})  
@Retention(RetentionPolicy.RUNTIME)
@Documented  
public @interface Validated {  
    // 校验时启动的分组  
    Class<?>[] value() default {};  
}
```
<a name="DAp0Q"></a>
#### `@Valid`
```java
// 用于方法，字段，构造函数，参数，以及泛型类型  
@Target({ METHOD, FIELD, CONSTRUCTOR, PARAMETER, TYPE_USE })  
@Retention(RUNTIME)  
@Documented
public @interface Valid {  
    // 未提供其他属性  
}
```

1. **作用范围不同**：`@Validated` 无法作用在于字段， `@Valid` 无法作用于类；
2. **注解中的属性不同**：`@Validated` 中提供了指定校验分组的属性，而 `@Valid` 没有这个功能，因为 `@Valid` 不能进行分组校验。
<a name="msdHw"></a>
## 字段校验场景及使用示例
常见的校验场景有三种： Controller 层的校验、编程式校验、 Dubbo 接口校验。
<a name="WzT5R"></a>
### Controller层 的校验
<a name="eA58D"></a>
#### 使用方式

1. 当方法入参为 `@RequestBody` 注解的 JavaBean，可在入参前使用 `@Validated` 或 `@Valid` 注解开启校验。
```java
@PostMapping("/save")
public Response<Boolean> saveNotice(@Validated @RequestBody NoticeDTO noticeDTO) {
    // noticeDTO中各字段校验通过，才会执行后续业务逻辑
    return Response.ok(true);
}
```

2. 当方法入参为 `@PathVariable`、 `@RequestParam` 注解的简单参数时，需要在 Controller 加上 `@Validated` 注解开启校验。
```java
@RequestMapping("/notice")
@RestController
// 必须加上该注解
@Validated
public class UserController {
    // 路径变量
    @GetMapping("{id}")
    public Reponse<NoticeDTO> detail(@PathVariable("id") @Min(1L) Long noticeId) {
        // 参数noticeId校验通过，执行后续业务逻辑
        return Reponse.ok();
    }

    // 请求参数
    @GetMapping("getByTitle")
    public Result getByTitle(@RequestParam("title") @Length(min = 1, max = 20) String  title) {
        // 参数title校验通过，执行后续业务逻辑
        return Result.ok();
    }
}
```
<a name="zx8v7"></a>
#### 原理

1. Spring 框架中的 `HandlerMethodArgumentResolver` 策略接口，负责将方法参数解析为特定请求中的参数值。
```java
public interface HandlerMethodArgumentResolver {  
    // 判断当前解析器是否支持给定的方法参数
    boolean supportsParameter(MethodParameter var1);  

    @Nullable  
    // 实际解析参数的方法
    Object resolveArgument(MethodParameter var1, @Nullable ModelAndViewContainer var2, NativeWebRequest var3, @Nullable WebDataBinderFactory var4) throws Exception;
}
```

2. 上述接口针对 `@RequestBody` 的实现类 `RequestResponseBodyMethodProcessor` 中，存在字段校验逻辑，调用 `validateIfApplicable` 方法校验参数。
```java
// RequestResponseBodyMethodProcessor
public Object resolveArgument(MethodParameter parameter, @Nullable ModelAndViewContainer mavContainer, NativeWebRequest webRequest, @Nullable WebDataBinderFactory binderFactory) throws Exception {  
    // 前置处理
    // 校验逻辑
    if (binderFactory != null) {  
        WebDataBinder binder = binderFactory.createBinder(webRequest, arg, name);  
        if (arg != null) {
            //调用校验函数
            this.validateIfApplicable(binder, parameter);  
            if (binder.getBindingResult().hasErrors() && this.isBindExceptionRequired(binder, parameter)) {  
                throw new MethodArgumentNotValidException(parameter, binder.getBindingResult());  
            }  
        }
        // 数据绑定逻辑
    }
    // 返回处理结果
    return this.adaptArgumentIfNecessary(arg, parameter);  
}
```

3. `validateIfApplicable` 方法中，根据方法参数上的注解，决定是否进行字段校验：当存在 `@Validated` 或以 `Valid` 开头的注解时，进行校验。
```java
protected void validateIfApplicable(WebDataBinder binder, MethodParameter parameter) {
    // 获取参数上的注解
    Annotation[] annotations = parameter.getParameterAnnotations();  
    Annotation[] var4 = annotations;  
    int var5 = annotations.length;  

    // 遍历注解
    for(int var6 = 0; var6 < var5; ++var6) {  
        Annotation ann = var4[var6];  
        // 获取 @Validated 注解
        Validated validatedAnn = (Validated)AnnotationUtils.getAnnotation(ann, Validated.class); 
        // 或者注解以 Valid 开头 
        if (validatedAnn != null || ann.annotationType().getSimpleName().startsWith("Valid")) {
            // 开启校验
            Object hints = validatedAnn != null ? validatedAnn.value() : AnnotationUtils.getValue(ann);  
            Object[] validationHints = hints instanceof Object[] ? (Object[])((Object[])hints) : new Object[]{hints};  
            binder.validate(validationHints);  
            break;  
        }  
    }  
}
```

4. `@PathVariable` 和 `@RequestParam` 对应的实现类中，则没有相应字段校验逻辑，因此需要在 Controller 上使用 `@Validated`，开启字段校验。
<a name="pXHxT"></a>
### 编程式校验

1. 配置 Validator
```java
@Configuration  
public class ValidatorConfiguration {  
    @Bean  
    public Validator validator() {  
        ValidatorFactory validatorFactory = Validation.byProvider(HibernateValidator.class)  
        .configure()  
        // 设置是否开启快速失败模式  
        //.failFast(true)  
        .buildValidatorFactory();  
        return validatorFactory.getValidator();  
    }  
}
```

2. 获取 validator 并校验
```java
public class TestValidator {
    // 注入验证器
    @Resource
    private javax.validation.Validator validator;

    public String testMethod(TestRequest request) {
        // 进行校验，获取校验结果
        Set<ConstraintViolation<TestRequest>> constraintViolations = validator.validate(request);
        // 组装校验信息并返回
        return res;
    }
}
```
<a name="imEeb"></a>
### Dubbo 接口校验

1. 可在 `@DubboService` 注解中，设置 validation 参数为 true，开启生产者的字段验证。
```java
@DubboService(version = "1.0.0", validation="true")
public class DubboApiImpl implements DubboApi {
    ....
}
```

2. 该方式返回的信息对使用者不友好，可通过 Dubbo 的 filter 自定义校验逻辑和返回信息。需要注意的是，在 Dubbo 中有自己的 IOC 实现来控制容器，因此需提供 setter 方法，供 Dubbo 调用。
```java
@Activate(  
    group = {"provider"},  
    value = {"customValidationFilter"},  
    order = 10000  
)  
@Slf4j  
public class CustomValidationFilter implements Filter {  

    private javax.validation.Validator validator;  

    // duubo会调用setter获取bean
    public void setValidator(javax.validation.Validator validator) {  
        this.validator = validator;  
    } 

    public Result invoke(Invoker<?> invoker, Invocation invocation) throws RpcException {  
        if (this.validator != null && !invocation.getMethodName().startsWith("$")) {  
            // 补充字段校验，返回信息的组装以及异常处理
        }  
        return invoker.invoke(invocation);  
    }  
}
```
<a name="VsF37"></a>
## 进阶使用
<a name="ZuB6d"></a>
### 分组校验
对于同一个 DTO， 不同场景下对其校验规则可能不同， `@Validted` 支持按照分组分别验证，示例代码如下：

1. 校验注解的 groups 属性中添加分组
```java
@Data
public class NoticeDTO {

    @Min(value = 0L, groups = Update.class)
    private Long id;

    @NotNull(groups = {Save.class, Update.class})
    @Length(min = 2, max = 10, groups = {Save.class, Update.class})
    private String title;

    // 保存的时候校验分组
    public interface Save {
    }

    // 更新的时候校验分组
    public interface Update {
    }
}
```

2. `@Validted` 上指定分组
```java
@PostMapping("/save")
public Response<Boolean> saveNotice(@RequestBody @Validated(NoticeDTO.Save.class) NoticeDTO noticeDTO) {
    // 分组为Save.class的校验通过，执行后续逻辑
    return Response.ok();
}

@PostMapping("/update")
public Response<Boolean> updateNotice(@RequestBody @Validated(NoticeDTO.Update.class) NoticeDTO noticeDTO) {
    // 分组为Update.class的校验通过，执行后续逻辑
    return Response.ok();
}
```
<a name="IDPly"></a>
### 自定义校验注解
如果想自定义实现一些验证逻辑，可以使用自定义注解，主要包括两部分：实现自定义注解，实现对应的校验器 validator。下面尝试实现一个注解，用于校验集合中的指定属性是否存在重复，代码如下：

1. 实现校验注解，主要需要包含 `message()`、 `filed()`、 `groups()` 三个方法，功能如注释所示。
```java
@Target({ElementType.FIELD, ElementType.PARAMETER})  
@Retention(RetentionPolicy.RUNTIME)  
@Documented  
// 指定校验器
@Constraint(validatedBy = UniqueValidator.class)  
public @interface Unique {  

    // 用于自定义验证信息
    String message() default "字段存在重复";  

    // 指定集合中的待校验字段
    String[] field();  

    // 指定分组
    Class<?>[] groups() default {};  
}
```

2. 实现对应的校验器，主要校验逻辑在 isValid 方法：获取集合中指定字段，并组装为 set，比较 set 和集合的长度，以判断集合中指定字段是否存在重复。
```java
// 实现ConstraintValidator<T, R>接口，T为注解的类型，R为注解的字段类型
public class UniqueValidator implements ConstraintValidator<Unique, Collection<?>> {  

    private Unique unique;  

    @Override  
    public void initialize(Unique constraintAnnotation) {  
        this.unique = constraintAnnotation;  
    }  

    @Override  
    public boolean isValid(Collection collection, ConstraintValidatorContext constraintValidatorContext) {
        // 集合为空直接校验通过
        if (collection == null || collection.size() == 0) {  
            return Boolean.TRUE;  
        }  
        // 从集合中获取filed中指定的待校验字段，看是否存在重复
        return Arrays.stream(unique.field())  
        .filter(fieldName -> fieldName != null && !"".equals(fieldName.trim()))  
        .allMatch(fieldName -> {
            // 收集集合collection中字段为fieldName的值，存入set并计算set的元素个数count
            int count = (int) collection.stream()  
                         .filter(Objects::nonNull)  
                         .map(item -> {  
                             Class<?> clazz = item.getClass();  
                             Field field;  
                             try {  
                                 field = clazz.getField(fieldName);  
                                 field.setAccessible(true);  
                                 return field.get(item);  
                             } catch (Exception e) {  
                                 return null;  
                             }  
                         })  
                         .collect(Collectors.collectingAndThen(Collectors.toSet(), Set::size)); 
            // set中元素个数count与集合长度比较，若不相等则说明collection中字段存在重复，校验不通过
            if (count != collection.size()) {  
                return false;  
            }  
            return true;  
        });  
    }  
}
```
<a name="p5czW"></a>
## 总结
通过本文得以了解 Spring Validation 的机理及其在实际项目中的应用。无论是标准的校验注解，还是自定义的校验逻辑， Spring Validation 都为开发者提供了高效且强大的校验工具。总的来说， Spring Validation 是任何 Spring 应用不可或缺的一部分，对于追求高质量代码的 JAVA 开发者而言，掌握其用法和最佳实践至关重要。
