JavaSpringBoot<br />在日常的接口开发中，为了保证接口的稳定安全，一般需要在接口逻辑中处理两种校验：

1. 参数校验
2. 业务规则校验

首先看看参数校验。
<a name="PUlv4"></a>
## 参数校验
参数校验很好理解，比如登录的时候需要校验用户名密码是否为空，创建用户的时候需要校验邮件、手机号码格式是否准确。<br />而实现参数校验也非常简单，只需要使用Bean Validation校验框架即可，借助它提供的校验注解可以非常方便的完成参数校验。<br />常见的校验注解有：<br />`@Null`、`@NotNull`、`@AssertTrue`、`@AssertFalse`、`@Min`、`@Max`、`@DecimalMin`、`@DecimalMax`、`@Negative`、`@NegativeOrZero`、`@Positive`、`@PositiveOrZero`、`@Size`、`@Digits`、`@Past`、`@PastOrPresent`、`@Future`、`@FutureOrPresent`、`@Pattern`、`@NotEmpty`、`@NotBlank`、`@Email`<br />接下来再看看业务规则校验。
<a name="SnuvA"></a>
## 业务规则校验
业务规则校验指接口需要满足某些特定的业务规则，举个例子：业务系统的用户需要保证其唯一性，用户属性不能与其他用户产生冲突，不允许与数据库中任何已有用户的用户名称、手机号码、邮箱产生重复。<br />这就要求在**创建用户时需要校验用户名称、手机号码、邮箱是否被注册**；**编辑用户时不能将信息修改成已有用户的属性**。<br />95%的程序员当面对这种业务规则校验时往往选择写在service逻辑中，常见的代码逻辑如下：
```java
public void create(User user) {
    Account account = accountDao.queryByUserNameOrPhoneOrEmail(user.getName(),user.getPhone(),user.getEmail());
    if (account != null) {
        throw new IllegalArgumentException("用户已存在，请重新输入");
    }
}
```
虽然使用`Assert`来优化代码可以使其看上去更简洁，但是将简单的校验交给 Bean Validation，而把复杂的校验留给自己，这简直是买椟还珠故事的程序员版本。<br />**最优雅的实现方法应该是参考 Bean Validation 的标准方式，借助自定义校验注解完成业务规则校验。**<br />接下来通过上面提到的用户接口案例，通过自定义注解完成业务规则校验。
<a name="fUk6F"></a>
## 代码实战
需求很容易理解，注册新用户时，应约束不与任何已有用户的关键信息重复；而修改自己的信息时，只能与自己的信息重复，不允许修改成已有用户的信息。<br />这些约束规则不仅仅为这两个方法服务，它们可能会在用户资源中的其他入口被使用到，乃至在其他分层的代码中被使用到，在 Bean 上做校验就能全部覆盖上述这些使用场景。
<a name="W4Anj"></a>
### 自定义注解
首先需要创建两个自定义注解，用于业务规则校验：

- `UniqueUser`：表示一个用户是唯一的，唯一性包含：用户名，手机号码、邮箱
```java
@Documented
@Retention(RUNTIME)
@Target({FIELD, METHOD, PARAMETER, TYPE})
@Constraint(validatedBy = UserValidation.UniqueUserValidator.class)
public @interface UniqueUser {

    String message() default "用户名、手机号码、邮箱不允许与现存用户重复";

    Class<?>[] groups() default {};

    Class<? extends Payload>[] payload() default {};
}
```

- `NotConflictUser`：表示一个用户的信息是无冲突的，无冲突是指该用户的敏感信息与其他用户不重合
```java
@Documented
@Retention(RUNTIME)
@Target({FIELD, METHOD, PARAMETER, TYPE})
@Constraint(validatedBy = UserValidation.NotConflictUserValidator.class)
public @interface NotConflictUser {
    String message() default "用户名称、邮箱、手机号码与现存用户产生重复";

    Class<?>[] groups() default {};

    Class<? extends Payload>[] payload() default {};
}
```
<a name="beVEw"></a>
### 实现业务校验规则
想让自定义验证注解生效，需要实现 `ConstraintValidator` 接口。接口的第一个参数是 **自定义注解类型**，第二个参数是 **被注解字段的类**，因为需要校验多个参数，直接传入用户对象。需要提到的一点是 `ConstraintValidator` 接口的实现类无需添加 `@Component` 它在启动的时候就已经被加载到容器中了。
```java
@Slf4j
public class UserValidation<T extends Annotation> implements ConstraintValidator<T, User> {

    protected Predicate<User> predicate = c -> true;

    @Resource
    protected UserRepository userRepository;

    @Override
    public boolean isValid(User user, ConstraintValidatorContext constraintValidatorContext) {
        return userRepository == null || predicate.test(user);
    }

    /**
     * 校验用户是否唯一
     * 即判断数据库是否存在当前新用户的信息，如用户名，手机，邮箱
     */
    public static class UniqueUserValidator extends UserValidation<UniqueUser>{
        @Override
        public void initialize(UniqueUser uniqueUser) {
            predicate = c -> !userRepository.existsByUserNameOrEmailOrTelphone(c.getUserName(),c.getEmail(),c.getTelphone());
        }
    }

    /**
     * 校验是否与其他用户冲突
     * 将用户名、邮件、电话改成与现有完全不重复的，或者只与自己重复的，就不算冲突
     */
    public static class NotConflictUserValidator extends UserValidation<NotConflictUser>{
        @Override
        public void initialize(NotConflictUser notConflictUser) {
            predicate = c -> {
                log.info("user detail is {}",c);
                Collection<User> collection = userRepository.findByUserNameOrEmailOrTelphone(c.getUserName(), c.getEmail(), c.getTelphone());
                // 将用户名、邮件、电话改成与现有完全不重复的，或者只与自己重复的，就不算冲突
                return collection.isEmpty() || (collection.size() == 1 && collection.iterator().next().getId().equals(c.getId()));
            };
        }
    }

}
```
这里使用`Predicate`函数式接口对业务规则进行判断。
<a name="EDh3W"></a>
### 使用
```java
@RestController
@RequestMapping("/senior/user")
@Slf4j
@Validated
public class UserController {
	@Autowired
	private UserRepository userRepository;
	
	
	@PostMapping
	public User createUser(@UniqueUser @Valid User user){
		User savedUser = userRepository.save(user);
		log.info("save user id is {}",savedUser.getId());
		return savedUser;
	}
	
	@SneakyThrows
	@PutMapping
	public User updateUser(@NotConflictUser @Valid @RequestBody User user){
		User editUser = userRepository.save(user);
		log.info("update user is {}",editUser);
		return editUser;
	}
}
```
使用很简单，只需要在方法上加入自定义注解即可，业务逻辑中不需要添加任何业务规则的代码。
<a name="Wy6ae"></a>
### 测试
调用接口后出现如下错误，说明业务规则校验生效。
```json
{
	"status": 400,
	"message": "用户名、手机号码、邮箱不允许与现存用户重复",
	"data": null,
	"timestamp": 1644309081037
}
```
<a name="ZiyIq"></a>
### 小结
通过上面几步操作，业务校验便和业务逻辑就完全分离开来，在需要校验时用`@Validated`注解自动触发，或者通过代码手动触发执行，可根据项目的要求，将这些注解应用于控制器、服务层、持久层等任何层次的代码之中。<br />这种方式比任何业务规则校验的方法都优雅，推荐大家在项目中使用。在开发时可以将不带业务含义的格式校验注解放到 Bean 的类定义之上，将带业务逻辑的校验放到 Bean 的类定义的外面。**这两者的区别是放在类定义中的注解能够自动运行，而放到类外面则需要像前面代码那样，明确标出注解时才会运行。**
