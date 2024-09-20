Java SpringBoot 参数校验
<a name="LrltA"></a>
## 校验应该在那一层
一般推荐与业务无关的放在Controller层中进行校验，而与业务有关的放在Service层中进行校验。那么如何将参数校验写的优雅美观呢？
<a name="B4GIN"></a>
## 常用校验工具类
使用Hibernate Validate
<a name="STlh2"></a>
### 引入依赖
```xml
<dependency>
    <groupId>org.hibernate</groupId>
    <artifactId>hibernate-validator</artifactId>
    <version>4.3.1.Final</version> 
</dependency>
```
<a name="xGvO8"></a>
### 常用注解说明
| 注解 | 说明 |
| --- | --- |
| `@Length(min=,max=)` | 检查所屋的字段的长度是否在min和max之间，只能用于字符串 |
| `@Range(min=,max=,message=)` | 被注释的元素必须在合适的范围内 |
| `@Max` | 该字段的值只能小于或等于该值 |
| `@Min` | 该字段的值只能大于或等于该值 |
| `@NotNull` | 不能为null |
| `@NotBlank` | 不能为空，检查时会将空格忽略 |
| `@NotEmpty` | 不能为空，这里的空是指空字符串 |
| `@Pattern(regex=,flag=)` | 被注释的元素必须符合指定的正则表达式 |

<a name="RbhG0"></a>
### 使用方式
需要搭配在Controller中搭配`@Validated`或`@Valid`注解一起使用，`@Validated`和`@Valid`注解区别不是很大，一般情况下任选一个即可，区别如下：

| 注解 | `@Validated` | `@Valid` |
| --- | --- | --- |
| 所属的包 | 属于org.springframework.validation.annotation包下的，是Spring提供的 | 属于javax.validation包下，是jdk提供的 |
| 是否支持分组和排序 | 是 | 否 |

虽然`@Validated`比`@Valid`更加强大，在`@Valid`之上提供了分组功能和验证排序功能，不过在实际项目中一直没有用到过<br />Hibernate-validate框架中的注解是需要加在实体中一起使用的

- 定义一个实体
```java
public class DataSetSaveVO {
    //唯一标识符为空
    @NotBlank(message = "user uuid is empty")
    //用户名称只能是字母和数字
    @Pattern(regexp = "^[a-z0-9]+$", message = "user names can only be alphabetic and numeric")
    @Length(max = 48, message = "user uuid length over 48 byte")
    private String userUuid;

    //数据集名称只能是字母和数字
    @Pattern(regexp = "^[A-Za-z0-9]+$", message = "data set names can only be letters and Numbers")
    //文件名称过长
    @Length(max = 48, message = "file name too long")
    //文件名称为空
    @NotBlank(message = "file name is empty")
    private String name;

    //数据集描述最多为256字节
    @Length(max = 256, message = "data set description length over 256 byte")
    //数据集描述为空
    @NotBlank(message = "data set description is null")
    private String description;
}
```
说明：message字段为不符合校验规则时抛出的异常信息

- Controller层中的方法
```java
@PostMapping
public ResponseVO createDataSet(@Valid @RequestBody DataSetSaveVO dataSetVO) {
    return ResponseUtil.success(dataSetService.saveDataSet(dataSetVO));
}
```
说明：在校验的实体`DataSetSaveVO`旁边添加`@Valid`或`@Validated`注解
<a name="e5bXe"></a>
## SpringBoot封装的`Validated`普通校验
普通检验是基础用法，非常容易，首先需要在SpringBootWeb项目中添加数据校验相关的依赖：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-validation</artifactId>
</dependency>
```
查看`LocalValidatorFactoryBean`类的源码，发现默认的`ValidationMessageSource`（校验出错时的提示文件） 是`resources`目录下的`ValidationMessages.properties` 文件，因此在`resources`目录下创建`ValidationMessages.properties`文件，内容如下
```
user.name.size=用户名长度介于5到10个字符之间
user.address.notnull=用户地址不能为空
user.age.size=年龄输入不正确
user.email.notnull=邮箱不能为空
user.email.pattern=邮箱格式不正确
```
创建User类，配置数据校验
```java
public class User {
    private Integer id;
    @Size(min = 5, max = 10, message = "{user.name.size}")
    private String name;
    @NotNull(message = "{user.address.notnull}")
    private String address;
    @DecimalMin(value = "1", message = "{user.age.size}")
    @DecimalMax(value = "200", message = "{user.age.size}")
    private Integer age;
    @Email(message = "{user.email.pattern}")
    @NotNull(message = "{user.email.notnull}")
    private String email;
    //省略getter和setter
}
```

- `@Size`表示一个字符串的长度或者一个集合的大小，必须在某一个范围中；`min`参数表示范围的下限；`max`参数表示范围的上限；`message`表示校验失败时的提示信息。
- `@NotNull`注解表示该字段不能为空
- `@DecimalMin`注解表示对应属性值的下限
- `@DecimalMax`注解表示对应属性值的上限
- `@Email`注解表示对应属性格式是一个Email

创建UserController
```java
@RestController
public class UserController {
    @PostMapping("/user")
    public List<String> addUser(@Validated User user, BindingResult result){
        List<String> errors = new ArrayList<>();
        if(result.hasErrors()){
            List<ObjectError> allErrors = result.getAllErrors();
            for(ObjectError error : allErrors){
                errors.add(error.getDefaultMessage());
            }
        }
        return errors;
    }
}
```

- 给User参数添加`@Validated`注解，表示需要对该参数做校验，紧接着的`BindingResult`参数表示在校验出错时保存的出错信息。
- 如果`BindingResult`中的`haiErrors`方法返回true，表示有错误信息，此时遍历错误信息，将之返回给前端
<a name="8fpyv"></a>
## 分组校验
在实际开发中，可能对于不同的情况，需要做不同的校验，这时候分组校验就起作用了<br />首先创建两个分组接口
```java
public interface ValidationGroup1 {
}
public interface ValidationGroup2 {
}
```
在实体类中添加分组信息
```java
public class User {
    private Integer id;
  //groups属性，表示该校验属性规则所属的分组
    @Size(min = 5, max = 10, message = "{user.name.size}", groups = ValidationGroup1.class)
    private String name;
    @NotNull(message = "{user.address.notnull}", groups = ValidationGroup2.class)
    private String address;
    @DecimalMin(value = "1", message = "{user.age.size}")
    @DecimalMax(value = "200", message = "{user.age.size}")
    private Integer age;
    @Email(message = "{user.email.pattern}")
    @NotNull(message = "{user.email.notnull}", groups = {ValidationGroup1.class, ValidationGroup2.class})
    private String email;
}
```
在`@Validated`注解中指定校验分组
```java
@RestController
public class UserController {
    //@Validated(ValidationGroup2.class) 表示这里的校验使用ValidationGroup2分组的校验规则，即只校验邮箱地址是否为空、用户地址是否为空
    @PostMapping("/user")
    public List<String> addUser(@Validated(ValidationGroup2.class) User user, BindingResult result){
        List<String> errors = new ArrayList<>();
        if(result.hasErrors()){
            List<ObjectError> allErrors = result.getAllErrors();
            for(ObjectError error : allErrors){
                errors.add(error.getDefaultMessage());
            }
        }
        return errors;
    }
}
```
<a name="qvV4E"></a>
## 自定义注解
当上面的方面都无法满足校验的需求以后，可以考虑使用自定义注解。
