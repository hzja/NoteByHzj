Java SpringBoot 参数校验
<a name="dv0KP"></a>
## 一、SpringBoot之参数校验
<a name="j1FGY"></a>
### 1、SpringBoot提供的参数校验注解
除了`@NotEmpty`和`@NotBlank`将 null 值认为是非法的之外，其它注解如`@Size`, `@Max`, `@Min`等都将 null 认为是有效的，如果不允许 null 值，则需要额外添加`@NotNull`注解。

| **注解** | **说明** |
| --- | --- |
| `**@NotEmpty**` | **字符串、集合、Map、数组等不能为 null 或空** |
| `**@NotBlank**` | **字符串不能为 null，且至少包含一个非空字符** |
| `**@NotNull**` | **任意类型 不能为 null** |
| `**@Size**` | **字符串、集合、Map、数组等元素的个数必须在指定的 min 和 max 范围内** |
| `**@Email**` | **字符串是有效的邮箱** |
| `**@Digits**` | **字符串、整数、浮点数是一个数字，参数 integer 表示整数位的数字个数，fraction 表示小数位的数字个数** |
| `**@CreditCardNumber**` | **字符串是有效的信用卡数字，不校验信用卡本身的有效性** |
| `**@AssertTrue**` | **布尔类型必须是 true，null 值被认为是有效的** |
| `**@Max**` | **整数、浮点数必须小于等于指定的最大值** |
| `**@Min**` | **整数、浮点数必须大于等于指定的最小值** |
| `**@Range**` | **字符串、数字必须在指定的 min 和 max 范围内** |
| `**@Pattern**` | **字符串必须匹配指定的正则表达式** |


```java
@Data
public class Employee implements Serializable {

  private static final long serialVersionUID = -8224860450904540019L;

  @NotEmpty(message = "名字不能为空")
  @UTF8Size(max = 16, message = "name should be short than 128")
  private String name;
    
  @Email
  private String email;

  @NotBlank(message = "city is required")
  @Size(max = 128, message = "city should be short than 128")
  private String city;

  @CreditCardNumber(message = "invalid credit card number")
  private String ccNumber;

  @Pattern(regexp = "^(0[1-9]|1[0-2])([\\\\/])([1-9][0-9])$", message = "required format MM/YY")
  private String ccExpiration;

  @Digits(integer = 3, fraction = 0, message = "invalid CVV")
  private String ccCVV;

}
```
<a name="LAA90"></a>
### 2、自定义校验注解
`@Size`并不能支持中文字符，可以自定义如下然后在需要校验字符（中英文）的字段上使用`@UTF8Size`即可。
```java
@Documented
@Constraint(validatedBy = Utf8SizeValidator.class)
@Target({METHOD, FIELD})
@Retention(RetentionPolicy.RUNTIME)
public @interface UTF8Size {
  String message() default "{javax.validation.constraints.Size.message}";

  int min() default 0;
  int max() default Integer.MAX_VALUE;

  Class<?>[] groups() default {};

  Class<? extends Payload>[] payload() default {};

}
```
```java
public class Utf8SizeValidator implements ConstraintValidator<UTF8Size, String> {

  private int maxCharSize;

  @Override
  public void initialize(UTF8Size constraintAnnotation) {
    this.maxCharSize = constraintAnnotation.max();
  }

  @Override
  public boolean isValid(String value, ConstraintValidatorContext context) {
    if (Objects.isNull(value)) {
      return true;
    }
    return value.getBytes(Charset.forName("GB18030")).length <= maxCharSize;
  }
}
```
<a name="nDBUB"></a>
## 二、字段校验的使用
使用 Bean Validation API 的`@Valid`注解，或者 Spring Context 提供的`@Validated`注解启用对 Bean 的校验。主要区别是，`@Validated`是`@Valid`的变体，支持分组校验（validation groups）。
<a name="wvmvu"></a>
### A、这种写法会将异常信息抛给全局异常处理
```java
@PostMapping("/emp")
public String addEmploy(@RequestBody  @Valid Employee employee) {
  log.info("employee to create: {}", employee);

  String employeeId = UUID.randomUUID().toString();
  return employeeId;
}
```
<a name="bVVRZ"></a>
### B、如果需要捕获参数校验的异常结果，写法如下
:::danger
**这种写法不会将异常处理的结果返回给全局异常处理**
:::
```java
@PostMapping()
UserInfo addUser(@RequestBody @Valid UserInfo userInfo, BindingResult bindingResult) {
    userService.addUser(userInfo);
    return userInfo;
}
```
<a name="BEvXv"></a>
## 三、字段校验的全局异常处理
```java
package org.hand.train.springboot.springboot.exception;

import lombok.extern.slf4j.Slf4j;
import org.springframework.http.HttpStatus;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;
import org.springframework.web.bind.MethodArgumentNotValidException;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.ResponseStatus;

import java.util.stream.Collectors;

/**
 * GlobalExceptionHandler
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 10:02 2019/12/4
 */
@Slf4j
@ControllerAdvice
public class GlobalExceptionHandler {
    @ExceptionHandler(MethodArgumentNotValidException.class)
    @ResponseBody
    @ResponseStatus(HttpStatus.BAD_REQUEST)
    public String handleMethodArgumentNotValidException(
            MethodArgumentNotValidException exception) {
        log.error("method argument not valid: {}", exception.getMessage());
        String errorMessage = "field error";

        BindingResult bindingResult = exception.getBindingResult();
        if (bindingResult.hasErrors()) {
            errorMessage = bindingResult.getFieldErrors().stream()
                    .map(FieldError::getDefaultMessage)
                    .collect(Collectors.joining(" | "));
        }

        return errorMessage;
    }
}
```
<a name="554Ct"></a>
## 四、分组校验
相同字段在不同场景需要不同的验证策略
<a name="6Mh8m"></a>
### A、组接口
```java
public class GroupVaildDTO {

    public interface SaveGroup extends Default {}

    public interface UpdateGroup extends Default {}
}
```
<a name="QVaOh"></a>
### B、校验的JavaBean属性
```java
public class GroupsValidForm {

    @Null(message = "id必须为null", groups = {GroupVaildDTO.SaveGroup.class})
    @NotNull(message = "id不能为null", groups = {GroupVaildDTO.UpdateGroup.class})
    private Integer id;

    @NotBlank(message = "用户名不能为空")
    private String userName;

    @Override
    public String toString() {
        final StringBuffer sb = new StringBuffer("GroupsValidForm{");
        sb.append("id=").append(id);
        sb.append(", userName='").append(userName).append('\'');
        sb.append('}');
        return sb.toString();
    }
}
```
<a name="oQ1NB"></a>
### C、Controller层分组校验的使用
相同字段在不同场景需要不同的验证策略
```java
@PostMapping(value = "update")
    public ServerResponse update(@RequestBody @Validated(value = GroupVaildDTO.UpdateGroup.class)
                                            GroupsValidForm groupsValidForm, BindingResult results) {
    if (results.hasErrors()) {
        return ServerResponse.createByErrorMessage(results.getFieldError().getDefaultMessage());
    }
    return ServerResponse.createBySuccess();
}
```

