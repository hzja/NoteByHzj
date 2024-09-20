Java SpringBoot
<a name="EDU5F"></a>
### 1、前言
在控制器类的方法里自己写校验逻辑代码当然也可以，只是代码比较丑陋，有点“low”。业界有更好的处理方法，分别阐述如下。
<a name="kyq3O"></a>
### 2、`PathVariable`校验
```java
@GetMapping("/path/{group:[a-zA-Z0-9_]+}/{userid}")
@ResponseBody
public String path(@PathVariable("group") String group, @PathVariable("userid") Integer userid) {
    return group + ":" + userid;
}
```
用法是：路径变量:正则表达式。当请求URI不满足正则表达式时，客户端将收到404错误码。不方便的地方是，不能通过捕获异常的方式，向前端返回统一的、自定义格式的响应参数。
<a name="PAEXc"></a>
### 3、方法参数校验
```java
@GetMapping("/validate1")
@ResponseBody
public String validate1(
        @Size(min = 1,max = 10,message = "姓名长度必须为1到10")@RequestParam("name") String name,
        @Min(value = 10,message = "年龄最小为10")@Max(value = 100,message = "年龄最大为100") @RequestParam("age") Integer age) {
    return "validate1";
}
```
如果前端传递的参数不满足规则，则抛出异常。注解Size、Min、Max来自validation-api.jar，更多注解参见相关标准小节。
<a name="KESRo"></a>
### 4、表单对象/VO对象校验
当参数是VO时，可以在VO类的属性上添加校验注解。
```java
public class User {
    @Size(min = 1,max = 10,message = "姓名长度必须为1到10")
    private String name;

    @NotEmpty
    private String firstName;

    @Min(value = 10,message = "年龄最小为10")@Max(value = 100,message = "年龄最大为100")
    private Integer age;

    @Future
    @JSONField(format="yyyy-MM-dd HH:mm:ss")
    private Date birth;
    。。。
}
```
其中，`Future`注解要求必须是相对当前时间来讲“未来的”某个时间。
```java
@PostMapping("/validate2")
@ResponseBody
public User validate2(@Valid @RequestBody User user){
    return user;
}
```
<a name="OMk4O"></a>
### 5、自定义校验规则
<a name="XTGHY"></a>
#### 5.1 自定义注解校验
需要自定义一个注解类和一个校验类。
```java
import javax.validation.Constraint;
import javax.validation.Payload;
import java.lang.annotation.*;

@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.PARAMETER,ElementType.FIELD})
@Constraint(validatedBy = FlagValidatorClass.class)
public @interface FlagValidator {
    // flag的有效值，多个使用,隔开
    String values();

    // flag无效时的提示内容
    String message() default "flag必须是预定义的那几个值，不能随便写";

    Class<?>[] groups() default {};

    Class<? extends Payload>[] payload() default {};
}
```
```java
import javax.validation.ConstraintValidator;
import javax.validation.ConstraintValidatorContext;

public class FlagValidatorClass implements ConstraintValidator<FlagValidator,Object> {
    /**
     * FlagValidator注解规定的那些有效值
     */
    private String values;

    @Override
    public void initialize(FlagValidator flagValidator) {
        this.values = flagValidator.values();
    }

    /**
     * 用户输入的值，必须是FlagValidator注解规定的那些值其中之一。
     * 否则，校验不通过。
     * @param value 用户输入的值，如从前端传入的某个值
     */
    @Override
    public boolean isValid(Object value, ConstraintValidatorContext constraintValidatorContext) {
        // 切割获取值
        String[] value_array = values.split(",");
        Boolean isFlag = false;

        for (int i = 0; i < value_array.length; i++){
            // 存在一致就跳出循环
            if (value_array[i] .equals(value)){
                isFlag = true; break;
            }
        }

        return isFlag;
    }
}
```
使用自定义的注解：
```java
public class User {
    // 前端传入的flag值必须是1或2或3，否则校验失败
    @FlagValidator(values = "1,2,3")
    private String flag ;
    。。。
}
```
<a name="rBRUh"></a>
#### 5.2 分组校验
```java
import org.hibernate.validator.constraints.Length;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

public class Resume {
    public interface Default {
    }

    public interface Update {
    }

    @NotNull(message = "id不能为空", groups = Update.class)
    private Long id;

    @NotNull(message = "名字不能为空", groups = Default.class)
    @Length(min = 4, max = 10, message = "name 长度必须在 {min} - {max} 之间", groups = Default.class)
    private String name;

    @NotNull(message = "年龄不能为空", groups = Default.class)
    @Min(value = 18, message = "年龄不能小于18岁", groups = Default.class)
    private Integer age;
    。。。
}

    /**
     * 使用Defaul分组进行验证
     * @param resume
     * @return
     */
    @PostMapping("/validate5")
    public String addUser(@Validated(value = Resume.Default.class) @RequestBody Resume resume) {
        return "validate5";
    }

    /**
     * 使用Default、Update分组进行验证
     * @param resume
     * @return
     */
    @PutMapping("/validate6")
    public String updateUser(@Validated(value = {Resume.Update.class, Resume.Default.class}) @RequestBody Resume resume) {
        return "validate6";
    }
}
```
建立了两个分组，名称分别为Default、Update。POST方法提交时使用Defaut分组的校验规则，PUT方法提交时同时使用两个分组规则。
<a name="Neypf"></a>
### 6、异常拦截器
通过设置全局异常处理器，统一向前端返回校验失败信息。
```java
import com.scj.springbootdemo.WebResult;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.util.CollectionUtils;
import org.springframework.validation.ObjectError;
import org.springframework.web.bind.MethodArgumentNotValidException;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.validation.ConstraintViolation;
import javax.validation.ConstraintViolationException;
import java.util.List;
import java.util.Set;

/**
 * 全局异常处理器
 */
@ControllerAdvice
public class GlobalExceptionHandler {

    private Logger logger = LoggerFactory.getLogger(GlobalExceptionHandler.class);

    /**
     * 用来处理bean validation异常
     * @param ex
     * @return
     */
    @ExceptionHandler(ConstraintViolationException.class)
    @ResponseBody
    public  WebResult resolveConstraintViolationException(ConstraintViolationException ex){
        WebResult errorWebResult = new WebResult(WebResult.FAILED);
        Set<ConstraintViolation<?>> constraintViolations = ex.getConstraintViolations();
        if(!CollectionUtils.isEmpty(constraintViolations)){
            StringBuilder msgBuilder = new StringBuilder();
            for(ConstraintViolation constraintViolation :constraintViolations){
                msgBuilder.append(constraintViolation.getMessage()).append(",");
            }
            String errorMessage = msgBuilder.toString();
            if(errorMessage.length()>1){
                errorMessage = errorMessage.substring(0,errorMessage.length()-1);
            }
            errorWebResult.setInfo(errorMessage);
            return errorWebResult;
        }
        errorWebResult.setInfo(ex.getMessage());
        return errorWebResult;
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    @ResponseBody
    public WebResult resolveMethodArgumentNotValidException(MethodArgumentNotValidException ex){
        WebResult errorWebResult = new WebResult(WebResult.FAILED);
        List<ObjectError>  objectErrors = ex.getBindingResult().getAllErrors();
        if(!CollectionUtils.isEmpty(objectErrors)) {
            StringBuilder msgBuilder = new StringBuilder();
            for (ObjectError objectError : objectErrors) {
                msgBuilder.append(objectError.getDefaultMessage()).append(",");
            }
            String errorMessage = msgBuilder.toString();
            if (errorMessage.length() > 1) {
                errorMessage = errorMessage.substring(0, errorMessage.length() - 1);
            }
            errorWebResult.setInfo(errorMessage);
            return errorWebResult;
        }
        errorWebResult.setInfo(ex.getMessage());
        return errorWebResult;
    }
}
```
<a name="GjYeb"></a>
### 7、相关标准
JSR 303 是Bean验证的规范 ，Hibernate Validator 是该规范的参考实现，它除了实现规范要求的注解外，还额外实现了一些注解。<br />validation-api-1.1.0.jar 包括如下约束注解：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638237355748-8d1150d7-b9d0-4f04-bde9-d266991829d3.webp#clientId=u4b2ae14e-a604-4&from=paste&id=ud9ac687f&originHeight=549&originWidth=962&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7041cc75-3fa6-4fda-92b3-c029532db7e&title=)<br />hibernate-validator-5.3.6.jar 包括如下约束注解：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638237355897-12340a8b-198d-476f-90bf-25dfd7994106.webp#clientId=u4b2ae14e-a604-4&from=paste&id=u609fc43c&originHeight=353&originWidth=961&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9a790f2f-73c9-41cb-b9b4-4c16e193b6f&title=)
<a name="Otnki"></a>
### 8、同时校验2个或更多个字段/参数
常见的场景之一是，查询某信息时要输入开始时间和结束时间。显然，结束时间要≥开始时间。可以在查询VO类上使用自定义注解，下面的例子来自这里。划重点：`@ValidAddress`使用在类上。
```java
@ValidAddress
public class Address {

    @NotNull
    @Size(max = 50)
    private String street1;

    @Size(max = 50)
    private String street2;

    @NotNull
    @Size(max = 10)
    private String zipCode;

    @NotNull
    @Size(max = 20)
    private String city;

    @Valid
    @NotNull
    private Country country;

    // Getters and setters
}

public class Country {

    @NotNull
    @Size(min = 2, max = 2)
    private String iso2;

    // Getters and setters
}

@Documented
@Target(TYPE)
@Retention(RUNTIME)
@Constraint(validatedBy = { MultiCountryAddressValidator.class })
public @interface ValidAddress {

    String message() default "{com.example.validation.ValidAddress.message}";

    Class<?>[] groups() default {};

    Class<? extends Payload>[] payload() default {};
}

public class MultiCountryAddressValidator 
       implements ConstraintValidator<ValidAddress, Address> {

    public void initialize(ValidAddress constraintAnnotation) {

    }

    @Override
    public boolean isValid(Address address, 
                           ConstraintValidatorContext constraintValidatorContext) {

        Country country = address.getCountry();
        if (country == null || country.getIso2() == null || address.getZipCode() == null) {
            return true;
        }

        switch (country.getIso2()) {
            case "FR":
                return // Check if address.getZipCode() is valid for France
            case "GR":
                return // Check if address.getZipCode() is valid for Greece
            default:
                return true;
        }
    }
}
```
