Spring 注解
<a name="ajZOi"></a>
### 参数校验注解
字段注解一般是用于校验字段是否满足要求，`hibernate-validate`依赖就提供了很多校验注解 ，如`@NotNull`、`@Range`等，但是这些注解并不是能够满足所有业务场景的。<br />比如希望传入的参数在指定的`String`集合中，那么已有的注解就不能满足需求了，需要自己实现。
<a name="wXLSt"></a>
#### Check注解
定义一个`@Check`注解，通过`@interface`声明一个注解
```java
@Target({ ElementType.FIELD}) //只允许用在类的字段上
@Retention(RetentionPolicy.RUNTIME) //注解保留在程序运行期间，此时可以通过反射获得定义在某个类上的所有注解
@Constraint(validatedBy = ParamConstraintValidated.class)
public @interface Check {
    /**
     * 合法的参数值
     * */
    String[] paramValues();

    /**
     * 提示信息
     * */
    String message() default "参数不为指定值";

    Class<?>[] groups() default {};

    Class<? extends Payload>[] payload() default {};
}
```
`**@Target**`** **定义注解的使用位置，用来说明该注解可以被声明在那些元素之前。<br />`ElementType.TYPE`：说明该注解只能被声明在一个类前。<br />`ElementType.FIELD`：说明该注解只能被声明在一个类的字段前。<br />`ElementType.METHOD`：说明该注解只能被声明在一个类的方法前。<br />`ElementType.PARAMETER`：说明该注解只能被声明在一个方法参数前。<br />`ElementType.CONSTRUCTOR`：说明该注解只能声明在一个类的构造方法前。<br />`ElementType.LOCAL_VARIABLE`：说明该注解只能声明在一个局部变量前。<br />`ElementType.ANNOTATION_TYPE`：说明该注解只能声明在一个注解类型前。<br />`ElementType.PACKAGE`：说明该注解只能声明在一个包名前<br />`**@Constraint**`** **通过使用validatedBy来指定与注解关联的验证器<br />`**@Retention**`** **用来说明该注解类的生命周期。<br />`RetentionPolicy.SOURCE`: 注解只保留在源文件中 <br />`RetentionPolicy.CLASS` : 注解保留在class文件中，在加载到JVM虚拟机时丢弃<br />`RetentionPolicy.RUNTIME`: 注解保留在程序运行期间，此时可以通过反射获得定义在某个类上的所有注解。
<a name="iymQb"></a>
#### 验证器类
验证器类需要实现`ConstraintValidator`泛型接口
```java
public class ParamConstraintValidated implements ConstraintValidator<Check, Object> {
    /**
     * 合法的参数值，从注解中获取
     * */
    private List<String> paramValues;

    @Override
    public void initialize(Check constraintAnnotation) {
        //初始化时获取注解上的值
        paramValues = Arrays.asList(constraintAnnotation.paramValues());
    }

    public boolean isValid(Object o, ConstraintValidatorContext constraintValidatorContext) {
        if (paramValues.contains(o)) {
            return true;
        }

        //不在指定的参数列表中
        return false;
    }
}
```
第一个泛型参数类型`Check`：注解，第二个泛型参数`Object`：校验字段类型。需要实现`initialize`和`isValid`方法，`isValid`方法为校验逻辑，`initialize`方法初始化工作
<a name="y0uMm"></a>
#### 使用
```java
@Data
public class User {
    /**
     * 姓名
     * */
    private String name;

    /**
     * 性别 man or women
     * */
    @Check(paramValues = {"man", "woman"})
    private String sex;
}
```
对`sex`字段加校验，其值必须为`woman`或者`man`
<a name="npbq3"></a>
#### 测试
```java
@RestController("/api/test")
public class TestController {
    @PostMapping
    public Object test(@Validated @RequestBody User user) {
        return "hello world";
    }
}
```
:::warning
注意需要在`User`对象上加上`@Validated`注解，这里也可以使用`@Valid`注解。
:::
<a name="qWivj"></a>
### 权限校验注解
在开发过程中遇到过这样的需求，如只有有权限的用户的才能访问这个类中的方法或某个具体的方法、查找数据的时候先不从数据库查找，先从`guava cache`中查找，在从`redis`查找，最后查找`mysql`（多级缓存）。<br />这时候可以自定义注解去完成这个要求，第一个场景就是定义一个权限校验的注解，第二个场景就是定义`spring-data-redis`包下类似`@Cacheable`的注解。
<a name="QG6Tq"></a>
#### PermissionCheck注解
该注解的作用范围为类或者方法上
```java
@Target({ ElementType.METHOD, ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
public @interface PermissionCheck {
    /**
     * 资源key
     * */
    String resourceKey();
}
```
<a name="EcX0I"></a>
#### 拦截器类
权限校验的逻辑就是有权限就可以访问，没有就不允许访问，本质其实就是一个拦截器。首先需要拿到注解，然后获取注解上的字段进行校验，校验通过返回`true`，否则返回`false`
```java
public class PermissionCheckInterceptor extends HandlerInterceptorAdapter {
    /**
     * 处理器处理之前调用
     */
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response,
                             Object handler) throws Exception {
        HandlerMethod handlerMethod = (HandlerMethod)handler;
        PermissionCheck permission = findPermissionCheck(handlerMethod);

        //如果没有添加权限注解则直接跳过允许访问
        if (permission == null) {
            return true;
        }

        //获取注解中的值
        String resourceKey = permission.resourceKey();

        //TODO 权限校验一般需要获取用户信息，通过查询数据库进行权限校验
        //TODO 这里只进行简单演示，如果resourceKey为testKey则校验通过，否则不通过
        if ("testKey".equals(resourceKey)) {
            return true;
        }

        return false;
    }

    /**
     * 根据handlerMethod返回注解信息
     *
     * @param handlerMethod 方法对象
     * @return PermissionCheck注解
     */
    private PermissionCheck findPermissionCheck(HandlerMethod handlerMethod) {
        //在方法上寻找注解
        PermissionCheck permission = handlerMethod.getMethodAnnotation(PermissionCheck.class);
        if (permission == null) {
            //在类上寻找注解
            permission = handlerMethod.getBeanType().getAnnotation(PermissionCheck.class);
        }

        return permission;
    }
}
```
<a name="9hLcz"></a>
#### 测试
该方法需要进行权限校验所以添加了`PermissionCheck`注解。
```java
@GetMapping("/api/test")
@PermissionCheck(resourceKey = "test")
public Object testPermissionCheck() {
    return "hello world";
}
```
<a name="BCIDd"></a>
### 缓存注解
<a name="k1v4U"></a>
#### CustomCache注解
缓存注解一般是使用在方法上的。
```java
@Target({ ElementType.METHOD, ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
public @interface CustomCache {
    /**
     * 缓存的key值
     * */
    String key();
}
```
<a name="ARLtH"></a>
#### 切面
因为缓存注解需要在方法执行之前有返回值，所以没有通过拦截器处理这个注解，而是通过使用切面在执行方法之前对注解进行处理。<br />如果注解没有返回值，将会返回方法中的值
```java
@Aspect
@Component
public class CustomCacheAspect {
    /**
     * 在方法执行之前对注解进行处理
     *
     * @param pjd
     * @param customCache 注解
     * @return 返回中的值
     * */
    @Around("@annotation(com.cqupt.annotation.CustomCache) && @annotation(customCache)")
    public Object dealProcess(ProceedingJoinPoint pjd, CustomCache customCache) {
        Object result = null;

        if (customCache.key() == null) {
            //TODO throw error
        }

        //TODO 业务场景会比这个复杂的多，会涉及参数的解析如key可能是#{id}这些，数据查询
        //TODO 这里做简单演示，如果key为testKey则返回hello world
        if ("testKey".equals(customCache.key())) {
            return "hello word";
        }

        //执行目标方法
        try {
            result = pjd.proceed();
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
        return result;
    }
}
```
<a name="wmeCO"></a>
#### 测试
```java
@GetMapping("/api/cache")
@CustomCache(key = "test")
public Object testCustomCache() {
    return "don't hit cache";
}
```
